r"""Differential fuzzing of single routines: the original's bytes against the port's C
(docs/BUILDING.md, "Testing").

    python3 tools/fuzzasm.py [--quick | --deep] [--seed N] [--only NAME[,NAME...]] [--list]
                             [--coverage] [-v]

The replay sessions test the routines the game reaches in them. This tests routines one at a
time, on inputs no session gives them: for each target below it runs the routine's own bytes
(from the user's UW2.EXE, which the gate proves our matched sources build byte for byte) in an
x86 emulator, Unicorn, and the port's code for the same routine (its translation by
tools/asm2c.py, or its hand-written C through the glue, or a C entry such as cSqRt) in
tools/fuzzhost.c, on the same registers and memory, and compares what each leaves: the
registers, the flags the routine's callers read, and every byte of memory it changed. The
inputs are random (seeded, so a run is reproducible: the seed is printed, and --seed repeats
it) and edge cases (0, 1, -1, 7FFFh, 8000h, FFFFh, the boundaries each routine has). --quick
(the default, a few seconds) runs a few hundred cases of each target, --deep tens of
thousands. A difference is reported with its inputs, and the run exits 1.

Both sides start each case from the same memory: the EXE's image loaded at the port's load
segment (PORT_LOAD_SEG, 800h), so segment values are the same on both sides, with two scratch
segments, E000 for data and F000 for the stack. A divide that faults stops both (Unicorn takes
the interrupt; the port stops where DOS would have run the handler), and that counts as
agreement. Memory below the stack pointer at the end is not compared: it is what the routine
pushed and popped, which hand-written C does not reproduce.

Needs Unicorn in the .venv (pip install unicorn==2.1.4, as tools/ailcheck.py) and the port's
objects (make port; --coverage uses make's coverage build in build/port-cov and writes a
.profraw for tools/coverage.py).
"""
import os, sys, re, time, zlib, random, struct, argparse, subprocess

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
sys.path.insert(0, here)
try:
    from unicorn import Uc, UcError, UC_ARCH_X86, UC_MODE_16, UC_HOOK_INTR, UC_HOOK_INSN
    from unicorn import x86_const as X
except ImportError:
    sys.exit('fuzzasm.py: needs Unicorn (.venv/bin/pip install unicorn==2.1.4)')

DATA = os.environ.get('UW2_DIR') or (os.path.dirname(os.environ['UW2_EXE']) if os.environ.get('UW2_EXE')
                                     else os.path.expanduser('~/UWGOG/UW2'))
EXE = os.environ.get('UW2_EXE') or os.path.join(DATA, 'UW2.EXE')
LOAD = 0x800                           # PORT_LOAD_SEG
SCRATCH, STACK = 0xE000, 0xF000        # fuzzhost.c's scratch segments
MEMTOP = 0x110000
FD71 = 0x60B9 + LOAD                   # seg021's data, dseg062_62a6
SEG004_DATA = 0x4FAF + LOAD            # seg052_519C, the renderer's data
CMPBUF = 0x43A0 + LOAD                 # cmpbuf1_start
RET_NEAR = 0xFFF0                      # a near call's return address (in the routine's CS)
RET_CS, RET_IP = 0x0010, 0x0000        # a far call's (linear 100h, never code)
# the regions the port has (fuzzhost.c's R[]): memory outside them that the original changes is
# memory the port cannot model, and is reported
REGIONS = [(0x37050 + LOAD * 16, 0x6061C + LOAD * 16), (FD71 * 16, FD71 * 16 + 0xC40),
           (0x850 + LOAD * 16, 0x850 + LOAD * 16 + 0x065C0 - 0x850 + 0x10010),
           (SCRATCH * 16, SCRATCH * 16 + 0x10000), (STACK * 16, STACK * 16 + 0x10000)]
REGS = ['eax', 'ebx', 'ecx', 'edx', 'esi', 'edi', 'ebp', 'esp', 'ds', 'es', 'ss', 'fs', 'gs', 'flags']
UREG = [X.UC_X86_REG_EAX, X.UC_X86_REG_EBX, X.UC_X86_REG_ECX, X.UC_X86_REG_EDX, X.UC_X86_REG_ESI,
        X.UC_X86_REG_EDI, X.UC_X86_REG_EBP, X.UC_X86_REG_ESP, X.UC_X86_REG_DS, X.UC_X86_REG_ES,
        X.UC_X86_REG_SS, X.UC_X86_REG_FS, X.UC_X86_REG_GS, X.UC_X86_REG_EFLAGS]
FLAGBITS = {'cf': 0, 'zf': 6, 'sf': 7, 'df': 10, 'of': 11}
ALLFLAGS = ('cf', 'zf', 'sf', 'of', 'df')
GPR = ('eax', 'ebx', 'ecx', 'edx', 'esi', 'edi', 'ebp', 'esp', 'ds', 'es', 'ss', 'fs', 'gs')


# ---- the two machines ------------------------------------------------------------------------

def load_image():
    """The EXE's load module at LOAD:0 with its relocations applied, as DOS loads it, in a 1 MB
    (and 64 KB) memory."""
    exe = open(EXE, 'rb').read()
    hdr = struct.unpack_from('<H', exe, 8)[0] * 16
    nrel, relat = struct.unpack_from('<H', exe, 6)[0], struct.unpack_from('<H', exe, 0x18)[0]
    img = bytearray(exe[hdr:])
    for i in range(nrel):
        off, seg = struct.unpack_from('<HH', exe, relat + 4 * i)
        lin = seg * 16 + off
        if lin + 2 <= len(img):
            struct.pack_into('<H', img, lin, (struct.unpack_from('<H', img, lin)[0] + LOAD) & 0xFFFF)
    mem = bytearray(MEMTOP)
    mem[LOAD * 16:LOAD * 16 + len(img)] = img
    return bytes(mem)


class Dos:
    """The original: Unicorn on the EXE's image."""

    def __init__(self, image=None):
        self.pristine = image or load_image()
        self.mu = Uc(UC_ARCH_X86, UC_MODE_16)
        self.mu.mem_map(0, MEMTOP)
        self.mu.mem_write(0, self.pristine)
        self.mu.hook_add(UC_HOOK_INTR, self.on_int)
        self.mu.hook_add(UC_HOOK_INSN, lambda uc, port, size, v, ud: None, None, 1, 0, X.UC_X86_INS_OUT)
        self.mu.hook_add(UC_HOOK_INSN, lambda uc, port, size, ud: 0, None, 1, 0, X.UC_X86_INS_IN)
        self.dirty = False

    def on_int(self, uc, n, ud):
        self.intr = n
        uc.emu_stop()

    def run(self, regs, mem, kind, seg, off):
        if getattr(self, 'fresh', False):
            # a machine stopped inside an interrupt does not always start cleanly again
            self.__init__(self.pristine)
        mu = self.mu
        if self.dirty: mu.mem_write(0, self.pristine)
        self.dirty = True
        for lin, b in mem: mu.mem_write(lin, bytes(b))
        r = dict(regs)
        ss, sp = r['ss'], r['esp'] & 0xFFFF
        if kind == 'far':
            sp = (sp - 4) & 0xFFFF
            mu.mem_write(ss * 16 + sp, struct.pack('<HH', RET_IP, RET_CS))
            stop = RET_CS * 16 + RET_IP
        else:
            sp = (sp - 2) & 0xFFFF
            mu.mem_write(ss * 16 + sp, struct.pack('<H', RET_NEAR))
            stop = (seg + LOAD) * 16 + RET_NEAR
        r['esp'] = (r['esp'] & 0xFFFF0000) | sp
        for k, u in zip(REGS, UREG):
            mu.reg_write(u, r[k] | (0x2 if k == 'flags' else 0))
        mu.reg_write(X.UC_X86_REG_CS, seg + LOAD)
        self.intr = None
        status = 'ret'
        try:
            mu.emu_start((seg + LOAD) * 16 + off, stop, count=50_000_000)
        except UcError as e:
            status = f'error {e}'
        if self.intr is not None:
            status = f'int {self.intr:X}'
            self.fresh = True
        elif status == 'ret' and (mu.reg_read(X.UC_X86_REG_CS) * 16 + mu.reg_read(X.UC_X86_REG_IP)) != stop:
            status = 'runaway'
        out = {k: mu.reg_read(u) for k, u in zip(REGS, UREG)}
        after = bytes(mu.mem_read(0, MEMTOP))
        return status, out, changed(self.pristine, after, mem)


def changed(before, after, preset):
    """{linear: byte} of the bytes that differ, before being the image with the case's own
    memory laid over it."""
    base = bytearray(before)
    for lin, b in preset: base[lin:lin + len(b)] = b
    out = {}
    if bytes(base) == after: return out
    C = 4096
    for c in range(0, MEMTOP, C):
        x, y = base[c:c + C], after[c:c + C]
        if x != y:
            for i in range(len(x)):
                if x[i] != y[i]: out[c + i] = y[i]
    return out


class Port:
    """The port: tools/fuzzhost.c, linked with the port's objects."""

    def __init__(self, exe):
        self.p = subprocess.Popen([exe, EXE], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True, bufsize=1)
        if self.p.stdout.readline().strip() != 'READY': sys.exit('fuzzasm.py: fuzzhost did not start')

    def run(self, regs, mem, kind, seg, off):
        w = self.p.stdin
        w.write('R ' + ' '.join(f'{regs[k]:x}' for k in REGS) + '\n')
        for lin, b in mem:
            w.write(f'M {lin:x} {bytes(b).hex()}\n')
        rcs, rip = (RET_CS, RET_IP) if kind == 'far' else (0, RET_NEAR)
        w.write(f'C {kind} {seg:x} {off:x} {rcs:x} {rip:x}\n')
        w.flush()
        rl = self.p.stdout.readline().split()
        if not rl: sys.exit('fuzzasm.py: fuzzhost stopped (it crashed; run the case in a debugger)')
        out = {k: int(v, 16) for k, v in zip(REGS, rl[1:])}
        st = self.p.stdout.readline().rstrip('\n').split(' ', 2)
        status = 'ret' if st[1] == '0' else 'halt ' + (st[2] if len(st) > 2 else '')
        mem_out = {}
        base = {}
        for lin, b in mem:
            for i, v in enumerate(b): base[lin + i] = v
        while True:
            l = self.p.stdout.readline()
            if l.startswith('E') or not l: break
            _, a, h = l.split()
            a = int(a, 16); b = bytes.fromhex(h)
            for i, v in enumerate(b): mem_out[a + i] = v
        # fuzzhost reports changes against the image; the case's own memory is the baseline here
        for a, v in base.items():
            if a in mem_out and mem_out[a] == v: del mem_out[a]
            elif a not in mem_out and v != self.image_byte(a): mem_out[a] = self.image_byte(a)
        return status, out, mem_out

    image = None

    def image_byte(self, a): return Port.image[a]

    def close(self):
        self.p.stdin.close(); self.p.wait()


# ---- inputs ------------------------------------------------------------------------------------

EDGE16 = [0, 1, 2, 0x7F, 0x80, 0xFF, 0x100, 0x3FFF, 0x4000, 0x5A82, 0x7FFE, 0x7FFF, 0x8000, 0x8001,
          0xC000, 0xFF00, 0xFFFE, 0xFFFF]


def word(rng, k):
    return EDGE16[k] if k < len(EDGE16) else rng.getrandbits(16)


def base_regs(rng):
    """Random general registers (the routine must not depend on what it does not take), the
    scratch segment in DS and ES, the stack segment with SP near its top."""
    r = {k: rng.getrandbits(32) for k in ('eax', 'ebx', 'ecx', 'edx', 'esi', 'edi', 'ebp')}
    r.update(esp=0xFF00, ds=SCRATCH, es=SCRATCH, ss=STACK, fs=SCRATCH, gs=SCRATCH,
             flags=rng.choice([0, 1, 0x40, 0x80, 0x800, 0x8C1]))
    return r


def setw(r, name, v):
    big = {'ax': 'eax', 'bx': 'ebx', 'cx': 'ecx', 'dx': 'edx', 'si': 'esi', 'di': 'edi', 'bp': 'ebp', 'sp': 'esp'}
    if name in big: r[big[name]] = (r[big[name]] & 0xFFFF0000) | (v & 0xFFFF)
    elif name in ('dh', 'bh', 'ch', 'ah'):
        e = 'e' + name[0] + 'x'; r[e] = (r[e] & ~0xFF00) | (v & 0xFF) << 8
    elif name in ('dl', 'bl', 'cl', 'al'):
        e = 'e' + name[0] + 'x'; r[e] = (r[e] & ~0xFF) | (v & 0xFF)
    else: r[name] = v


# ---- the targets ------------------------------------------------------------------------------

class T:
    """A routine: name, its code segment (EXE paragraph) and offset, how it is called in DOS
    (near, far) and in the port (the same, or a C entry's kind), its inputs (a function of the
    random generator and the case number giving registers and memory), and what to compare."""

    def __init__(self, name, seg, off, call, gen, port=None, regs=GPR, flags=ALLFLAGS, what='',
                 mem=True, ignore=(), note='', heavy=False):
        self.name, self.seg, self.off, self.call, self.gen = name, seg, off, call, gen
        self.port = port or call
        self.regs, self.flags, self.what, self.mem = regs, flags, what, mem
        self.ignore, self.note, self.heavy = ignore, note, heavy


def g_imath(inputs):
    """IMATH: DS = seg021's data, the given 16-bit inputs, edge values first."""
    def gen(rng, k):
        r = base_regs(rng); r['ds'] = FD71
        for i, name in enumerate(inputs):
            v = word(rng, (k + i * 7) % (len(EDGE16) * 2) if k < len(EDGE16) * 2 else 99)
            setw(r, name, v)
        return r, []
    return gen


def g_sqrt(rng, k):
    """CX:BX for the square root: every magnitude, the four guesses' boundaries, the largest
    values (whose first quotient overflows: DOS faults)."""
    r = base_regs(rng); r['ds'] = FD71
    edges = [0, 1, 2, 3, 0xFF, 0x100, 0x101, 0xFFFF, 0x10000, 0xFFFFFF, 0x1000000, 0x3FFFFFFF, 0x40000000,
             0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, 0xFFFE0001, 0xFFFE0000]
    if k < len(edges): v = edges[k]
    else: v = rng.getrandbits(rng.choice([8, 16, 24, 32]))
    setw(r, 'cx', v >> 16); setw(r, 'bx', v)
    return r, []


def g_matrix(row_at_bp_ss=True, transposed=False):
    """mm3x9_bpsi and mm3x9t: a row of three 1.15 words and a 3 x 3 matrix, mostly in range,
    with overflowing products and 8000h now and then (the saturation paths)."""
    def gen(rng, k):
        r = base_regs(rng); r['ds'] = SCRATCH
        def v():
            c = rng.random()
            return rng.choice([0x7FFF, 0x8000, 0x8001, 0xFFFF, 0]) if c < 0.25 else rng.getrandbits(16)
        a = struct.pack('<3H', *[v() for _ in range(3)])
        m = struct.pack('<9H', *[v() for _ in range(9)])
        setw(r, 'bp', 0x200); setw(r, 'si', 0x100)
        # [bp] is SS-relative: the row (or the matrix, transposed) in the stack segment
        return r, [(STACK * 16 + 0x200, m if transposed else a), (SCRATCH * 16 + 0x100, a if transposed else m)]
    return gen


def g_mm9x9(rng, k):
    r = base_regs(rng); r['ds'] = SCRATCH; r['es'] = SCRATCH
    m1 = struct.pack('<9H', *[rng.getrandbits(16) for _ in range(9)])
    m2 = struct.pack('<9H', *[rng.getrandbits(16) for _ in range(9)])
    setw(r, 'bp', 0x200); setw(r, 'si', 0x100)
    return r, [(STACK * 16 + 0x200, m1), (SCRATCH * 16 + 0x100, m2)]


def g_point(rng, k):
    r = base_regs(rng); r['ds'] = SEG004_DATA
    for n in ('bx', 'cx', 'bp'):
        setw(r, n, word(rng, rng.randrange(len(EDGE16) * 2)))
    return r, []


def g_image(fmt):
    """An image for the EXPAND decoders: its size word and data at E000:0100 (AX the paragraph,
    BP the size word's offset), a 16 or 32 byte aux palette at E000:0000 (DS:SI), and DH a
    lightabs row (0..15, or FFh for none). The run-length forms get streams of valid words with
    every record kind: repeats, runs, the extended counts and the repeat-count records."""
    def stream(rng, nbits, n):
        out = []
        while len(out) < n:
            c = rng.random()
            if c < 0.5: out += [rng.randrange(3, 1 << nbits), rng.randrange(1 << nbits)]
            elif c < 0.6: out += [1]
            elif c < 0.7: out += [0, rng.randrange(1, 1 << nbits), rng.randrange(1 << nbits), rng.randrange(1 << nbits)]
            elif c < 0.75: out += [2, rng.randrange(1, 3), rng.randrange(3, 1 << nbits), rng.randrange(1 << nbits), 1]
            run = rng.randrange(0, 1 << nbits)
            out += [run] + [rng.randrange(1 << nbits) for _ in range(run)]
        return out[:n]

    def gen(rng, k):
        r = base_regs(rng)
        size = [1, 2, 3, 7, 8, 9][k] if k < 6 else rng.randrange(1, 600)
        dh = rng.choice([0xFF, 0, 5, 15]) if k % 3 else 0xFF
        if fmt == 4: data = bytes(rng.getrandbits(8) for _ in range(size))
        elif fmt == 10: data = bytes(rng.getrandbits(8) for _ in range(size))
        elif fmt == 8:
            w = stream(rng, 4, size); w += [0] * (len(w) & 1)
            data = bytes(w[i] << 4 | w[i + 1] for i in range(0, len(w), 2))
        else:
            w = stream(rng, 5, size); w += [0] * (-len(w) % 8)
            bits = ''.join(f'{x:05b}' for x in w)
            data = bytes(int(bits[i:i + 8], 2) for i in range(0, len(bits), 8))
        img = struct.pack('<H', size) + data
        pal = bytes(rng.getrandbits(8) for _ in range(32))
        r['ds'] = SCRATCH
        setw(r, 'si', 0); setw(r, 'ax', SCRATCH + 0x10); setw(r, 'bp', 0); setw(r, 'dh', dh)
        setw(r, 'bx', fmt)
        return r, [(SCRATCH * 16, pal), ((SCRATCH + 0x10) * 16, img)]
    return gen


def g_pal(rng, k):
    r = base_regs(rng); r['ds'] = SCRATCH
    setw(r, 'si', rng.randrange(0, 0x100)); setw(r, 'cx', rng.choice([1, 2]))
    setw(r, 'ax', SCRATCH + 0x20); setw(r, 'dh', rng.choice([0xFF, 0, 1, 7, 15]))
    return r, [(SCRATCH * 16, bytes(rng.getrandbits(8) for _ in range(0x140)))]


def g_records(nbits):
    """The record decoder alone (_18D): DS:SI the unpacked words, CX their number, ES:DI the
    output, BX 0 (uncmp_pal)."""
    def gen(rng, k):
        r = base_regs(rng)
        n = rng.randrange(1, 400)
        words = []
        while len(words) < n:
            c = rng.random()
            if c < 0.5: words += [rng.randrange(3, 1 << nbits), rng.randrange(1 << nbits)]
            elif c < 0.6: words += [1]
            elif c < 0.7: words += [0, rng.randrange(1, 1 << nbits), rng.randrange(1 << nbits)]
            elif c < 0.75: words += [2, rng.randrange(1, 3), rng.randrange(3, 1 << nbits), rng.randrange(1 << nbits), 1]
            run = rng.randrange(0, 1 << nbits)
            words += [run] + [rng.randrange(1 << nbits) for _ in range(run)]
        words = words[:n]
        # the buffers exp_4run and exp_5run give it, the only ones 3d/expand.c's _18D takes
        r['ds'] = CMPBUF; r['es'] = CMPBUF
        setw(r, 'si', 0); setw(r, 'cx', len(words)); setw(r, 'di', 0x5400); setw(r, 'bx', 0)
        return r, [(CMPBUF * 16, bytes(words))]
    return gen


def g_clip(rng, k):
    """_asm_clip_polygon: CX vertices of 32 bytes (x, y, z, sx, sy, u, v, l, 16.16) at DS:DC00
    (seg004's data), around the view frustum: mostly in front of the eye, some behind it, some
    on its planes."""
    r = base_regs(rng); r['ds'] = SEG004_DATA
    n = [3, 3, 4, 16][k] if k < 4 else rng.randrange(3, 17)
    vs = b''
    for _ in range(n):
        z = rng.choice([rng.randrange(1, 0x400), rng.randrange(-0x40, 0x40), 0x100])
        x = rng.choice([rng.randrange(-3 * abs(z) - 1, 3 * abs(z) + 2), z, -z])
        y = rng.choice([rng.randrange(-3 * abs(z) - 1, 3 * abs(z) + 2), z, -z])
        f = lambda v: (v << 16 | rng.getrandbits(16)) & 0xFFFFFFFF
        vs += struct.pack('<8I', f(x), f(y), f(z), 0, 0, f(rng.randrange(0, 64)), f(rng.randrange(0, 64)),
                          rng.getrandbits(20))
    setw(r, 'si', 0xDC00); setw(r, 'cx', n)
    return r, [(SEG004_DATA * 16 + 0xDC00, vs)]


def g_dist(rng, k):
    """get_dist: three 32-bit eye-relative coordinates at seg004's ds:16h, 1Ah, 1Eh."""
    r = base_regs(rng); r['ds'] = SEG004_DATA
    vals = [rng.choice([0, 1, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, rng.getrandbits(32),
                        rng.getrandbits(20), -rng.getrandbits(20) & 0xFFFFFFFF]) for _ in range(3)]
    return r, [(SEG004_DATA * 16 + 0x16, struct.pack('<3I', *vals))]


def g_sphere(rng, k):
    """sphere_check: BX, AX, BP the object origin, SI the radius, CL the shift, against the view
    matrix the EXE's data holds."""
    r = base_regs(rng); r['ds'] = SEG004_DATA
    for n in ('bx', 'ax', 'bp'): setw(r, n, word(rng, rng.randrange(len(EDGE16) * 2)))
    setw(r, 'si', rng.choice([0, 1, 0x40, 0x100, 0x7FFF, rng.getrandbits(16)]))
    setw(r, 'cl', rng.randrange(0, 9))
    return r, []


SEG003_DATA = 0x370D + LOAD            # seg_370D, the graphics library's data


def fb_frame(rng):
    """A frame buffer as cPlaceFB lays it out (setup_frame_buf, GRENTRY.ASM): w by h, the row
    table at seg_370D:095C (row y at 2 + y * (w + 2), rows past h at 69D4h), the frame buffer
    segment (stdat) at 0958 as the EXE has it. Returns (w, h, preset)."""
    w = rng.choice([320, 320, 32, rng.randrange(8, 321)])
    h = rng.choice([200, 112, rng.randrange(1, 201)])
    rows = [2 + y * (w + 2) for y in range(h)] + [0x69D4] * (200 - h)
    return w, h, [(SEG003_DATA * 16 + 0x95C, struct.pack('<200H', *rows))]


def spans(rng, w, h, n, extra):
    """n span records (y, left, right, then the extra words), left and right in either order,
    ended by a word with its top bit set."""
    out = b''
    for _ in range(n):
        a, b = rng.randrange(w), rng.randrange(w)
        out += struct.pack('<3H', rng.randrange(h), a, b) + struct.pack(f'<{len(extra)}H', *[f(rng) for f in extra])
    return out + struct.pack('<H', 0x8000 | rng.getrandbits(15))


def g_fb_solid(rng, k):
    """_9C9, the frame buffer's solid span writer: 6-byte spans at seg_370D:4E00, the colour at
    4111."""
    r = base_regs(rng); r['ds'] = SEG003_DATA
    w, h, pre = fb_frame(rng)
    sp = spans(rng, w, h, rng.randrange(0, 24), [])
    setw(r, 'si', 0x4E00)
    return r, pre + [(SEG003_DATA * 16 + 0x4E00, sp), (SEG003_DATA * 16 + 0x4111, bytes([rng.getrandbits(8)]))]


def g_fb_rows(rng, k):
    """_6F8 and _729, linear bitmap rows into the frame buffer (8-byte records: y, left, right,
    the source's offset in the segment at seg_370D:55EC), on seg_370D as their stack, as the
    library runs them; the source, with zeros for _729's transparency, in the scratch segment."""
    r = base_regs(rng); r['ds'] = SCRATCH; r['ss'] = SEG003_DATA; r['esp'] = 0x4FA0
    w, h, pre = fb_frame(rng)
    recs = b''
    for _ in range(rng.randrange(0, 16)):
        a = rng.randrange(w); b = rng.randrange(a, w)
        recs += struct.pack('<4H', rng.randrange(h), a, b, rng.randrange(0, 0x0F00))
    recs += struct.pack('<H', 0x8000)
    src = bytes(rng.choice([0, 0, rng.getrandbits(8)]) for _ in range(0x1000))
    setw(r, 'si', 0x4C00)
    return r, pre + [(SEG003_DATA * 16 + 0x4C00, recs), (SEG003_DATA * 16 + 0x55EC, struct.pack('<H', SCRATCH)),
                     (SCRATCH * 16, src)]


def g_fb_smooth(rng, k):
    """_C13, Gouraud spans into the frame buffer: 10-byte records (y, left, right, left and right
    intensity, 8.8 with the lightabs row in the high byte) at DS:SI, the solid vectors at
    L0BC3 and L0BC5 as the EXE has them, the base colour at seg004:6D20."""
    r = base_regs(rng); r['ds'] = SEG003_DATA; r['ss'] = SEG003_DATA; r['esp'] = 0x4FA0
    w, h, pre = fb_frame(rng)
    inten = lambda rng: rng.choice([0, 0x0F00, rng.randrange(0, 0x1000)])
    sp = spans(rng, w, h, rng.randrange(0, 12), [inten, inten])
    setw(r, 'si', 0x4C00)
    return r, pre + [(SEG003_DATA * 16 + 0x4C00, sp), ((S4 + LOAD) * 16 + 0x6D20, bytes([rng.getrandbits(8)]))]


def g_fb_al(rng, k):
    r = base_regs(rng); r['ds'] = SEG003_DATA
    setw(r, 'ax', rng.getrandbits(16))
    return r, []


def g_fb_dim(rng, k):
    r = base_regs(rng); r['ds'] = SEG003_DATA
    setw(r, 'cl', rng.randrange(0, 16)); setw(r, 'ch', rng.getrandbits(8))
    return r, [((0x3CF5 + LOAD) * 16 + 2 + i * 0x800, bytes(rng.getrandbits(8) for _ in range(64))) for i in range(13)]


def g_fb_lite(rng, k):
    r = base_regs(rng); r['ds'] = SEG003_DATA
    setw(r, 'cx', rng.choice([1, 1, 2, 3]))
    return r, [((0x3CF5 + LOAD) * 16 + 2 + i * 0x800, bytes(rng.getrandbits(8) for _ in range(64))) for i in range(13)]


IM, S4 = 0x2110, 0x065C
# L01A6, the byte the record decoder patches to a ret and back: the code has 2Bh there (sub ax,ax
# as 2B C0) and the decoder writes back 29h (sub ax,ax as 29 C0), so after the first repeat-count
# record DOS's code segment holds the other encoding of the same instruction; 3d/expand.c keeps
# the patch as a flag and leaves the byte as it was
L01A6 = [((S4 + LOAD) * 16 + 0x1A6, (S4 + LOAD) * 16 + 0x1A7)]
L01A6_NOTE = ('the byte at L01A6 is not compared: DOS restores it as 29h where the code had 2Bh, the same '
              'instruction (sub ax,ax) in its other encoding; the C keeps the patch as a flag')
DECODER_NOTE = ('only AX (the pixels\' paragraph) and the memory are compared: every caller of uncmp_tab '
                '(do_uwobj, do_uwcrit, cFrmtoRaw) reads AX alone after the call, and the C sets no other register')
NOFLAGS = ()
TARGETS = [
    # IMATH (seg021): the integer sines, square roots and arctangents
    T('imath.sincos_far', IM, 0xA34, 'far', g_imath(['bx']), flags=NOFLAGS,
      what='sincos (_A34 -> _A38): sys/imath.c through the glue'),
    T('imath.lsqrt_far', IM, 0xA30, 'far', g_sqrt, flags=NOFLAGS,
      what='lsqrt (_A30 -> _A78): sys/imath.c through the glue'),
    T('imath.cSinCos', IM, 0xA38, 'near', g_imath(['bx']), port='csincos', regs=('eax', 'ebx'), flags=NOFLAGS,
      what="cSinCos: sys/imath.c's C entry, against _A38 (AX, BX the 16-bit results)"),
    T('imath.cFstSinCos', IM, 0xA69, 'near', g_imath(['bx']), port='cfst', regs=('eax', 'ebx'), flags=NOFLAGS,
      what="cFstSinCos: sys/imath.c's C entry, against _A69"),
    T('imath.cSqRt', IM, 0xA78, 'near', g_sqrt, port='csqrt', regs=('edi',), flags=NOFLAGS,
      what="cSqRt: sys/imath.c's C entry, against _A78 (DI)"),
    T('imath.asin', IM, 0xB78, 'near', g_imath(['ax']), what='_B78, arcsine: the translation (sys/imath_x.c)'),
    T('imath.acos', IM, 0xBA2, 'near', g_imath(['bx']), what='_BA2, arccosine: the translation'),
    T('imath.atan2', IM, 0xBD4, 'near', g_imath(['ax', 'bx']), what='_BD4, atan2: the translation'),
    T('imath.cAtan2', IM, 0xBD4, 'near', g_imath(['ax', 'bx']), port='catan2', regs=('ecx',), flags=NOFLAGS,
      ignore=[(FD71 * 16 + 0x400, FD71 * 16 + 0x510)],
      what="cAtan2: sys/imath.c's C entry into the translation, against _BD4 (CX)",
      note="outside the renderer cAtan2 runs on seg021's private stack (FD71:0510 down), as "
           "C3DENTRY.ASM's entry does: what it pushes there is not compared"),
    # INSTANCE (seg004): the 3 x 3 matrix products and the clip codes
    T('instance.mm3x9_bpsi', S4, 0x3B41, 'near', g_matrix(), what='mm3x9_bpsi: the translation (3d/instance.c)'),
    T('instance.mm3x9t', S4, 0x3BE8, 'near', g_matrix(transposed=True), what='mm3x9t: the translation'),
    T('instance.mm9x9', S4, 0x3B04, 'near', g_mm9x9, what='mm9x9: the translation (writes ES:14B2..14C3)'),
    T('instance.code_pnt', S4, 0x3A88, 'near', g_point, what='code_pnt, the clip codes: the translation'),
    T('instance.mxmul', S4, 0x3534, 'near', g_point, what="mxmul with the EXE's body: the translation"),
    # SPHERE (seg004): the bounding-sphere test and the distance estimate nothing in the EXE calls
    T('sphere.sphere_check', S4, 0x260, 'near', g_sphere, what='sphere_check: the translation (3d/sphere.c)'),
    T('sphere.get_dist', S4, 0x40F, 'near', g_dist, what='get_dist (no caller in the EXE): the translation'),
    # GRENTRY (seg003): the frame buffer's span writers and fills, on a frame buffer laid out as
    # cPlaceFB lays it out
    T('grentry.solid_spans', 0x0085, 0x9C9, 'near', g_fb_solid, what='_9C9, the solid span writer: the translation (gfx/grentry_x.c)'),
    T('grentry.rows', 0x0085, 0x6F8, 'near', g_fb_rows, what='_6F8, bitmap rows into the frame buffer: the translation'),
    T('grentry.rows_x', 0x0085, 0x729, 'near', g_fb_rows, regs=('esp', 'ds', 'es', 'ss'), flags=NOFLAGS,
      what='_729, the same skipping colour 0: gfx/grentry.c through seg003_call',
      note='the span writers\' callers read no register they leave, so only the memory is compared'),
    T('grentry.smooth', 0x0085, 0xC13, 'near', g_fb_smooth, what='_C13, Gouraud spans: the translation'),
    T('grentry.fill', 0x0085, 0x6E4, 'far', g_fb_al, heavy=True, what='_6E4, cFillFB: the translation'),
    T('grentry.dim', 0x0085, 0x764, 'far', g_fb_dim, heavy=True, what='_764, cDimFB: the translation'),
    T('grentry.lite', 0x0085, 0x788, 'far', g_fb_lite, heavy=True, what='_788, cLiteFB: the translation'),
    # CLIP (seg004): the frustum clipper
    T('clip.polygon', S4, 0x4686, 'near', g_clip, what='_asm_clip_polygon: the translation (3d/clip.c)'),
    # EXPAND (seg004): the image decoders
    T('expand.pal_63', S4, 0x63, 'near', g_pal, regs=('ebx', 'ecx', 'esi', 'ds', 'es'), flags=NOFLAGS,
      what='_63, uncmp_pal: 3d/expand.c through the glue',
      note='AL (the last byte translated), DX (the image paragraph) and DI (past uncmp_pal) are not '
           'set by the C: every caller loads all three again before reading them'),
    T('expand.exp_8str', S4, 0x20, 'near', g_image(4), what='exp_8str: the translation (3d/expand_x.c)'),
    T('expand.exp_4str', S4, 0x39, 'near', g_image(10), regs=('eax',), flags=NOFLAGS,
      what='exp_4str: 3d/expand.c through the glue', note=DECODER_NOTE),
    T('expand.exp_4run', S4, 0xD9, 'near', g_image(8), regs=('eax',), flags=NOFLAGS, ignore=L01A6,
      what='exp_4run: 3d/expand.c through the glue', note=DECODER_NOTE, heavy=True),
    T('expand.exp_5run', S4, 0x11C, 'near', g_image(6), regs=('eax',), flags=NOFLAGS, ignore=L01A6,
      what='exp_5run: the translation, with _63 and _18D hand-written C', note=DECODER_NOTE, heavy=True),
    T('expand.uncmp4', S4, 0x20, 'near', g_image(4), port='uncmp', regs=('eax',), flags=NOFLAGS,
      ignore=[(FD71 * 16 + 0x400, FD71 * 16 + 0x510)],
      what="seg004_uncmp, format 4 (cFrmtoRaw's C into the translated exp_8str)", note=DECODER_NOTE),
    T('expand.uncmp6', S4, 0x11C, 'near', g_image(6), port='uncmp', regs=('eax',), flags=NOFLAGS,
      ignore=[(FD71 * 16 + 0x400, FD71 * 16 + 0x510)] + L01A6,
      what="seg004_uncmp, format 6 (cFrmtoRaw's C into the translated exp_5run)", note=DECODER_NOTE, heavy=True),
    T('expand.uncmp8', S4, 0xD9, 'near', g_image(8), port='uncmp', regs=('eax',), flags=NOFLAGS, ignore=L01A6,
      what="seg004_uncmp, format 8 (cFrmtoRaw's C exp_4run)", note=DECODER_NOTE, heavy=True),
    T('expand.uncmp10', S4, 0x39, 'near', g_image(10), port='uncmp', regs=('eax',), flags=NOFLAGS,
      what="seg004_uncmp, format 0Ah (cFrmtoRaw's C exp_4str)", note=DECODER_NOTE),
    T('expand.records4', S4, 0x18D, 'near', g_records(4), regs=('ebx', 'edx', 'esi', 'edi', 'ebp', 'ds', 'es'),
      flags=NOFLAGS, ignore=L01A6, what='_18D, the record decoder: 3d/expand.c through the glue',
      note='AX and CX are left as the last record had them in DOS and not set by the C: its callers '
           '(exp_4run, exp_5run) load AX from ES at once and never read CX', heavy=True),
    T('expand.records5', S4, 0x18D, 'near', g_records(5), regs=('ebx', 'edx', 'esi', 'edi', 'ebp', 'ds', 'es'),
      flags=NOFLAGS, ignore=L01A6, what='_18D on 5-bit words', heavy=True),
]


# ---- running ---------------------------------------------------------------------------------

def build_host(coverage=False):
    """fuzzhost.c compiled and linked with the port's objects (fuzzhost.c replaces main.c)."""
    import portbuild, portcheck, sources
    variant = 'port-cov' if coverage else 'port'
    out = os.path.join(root, 'build', variant)
    if not os.path.exists(os.path.join(out, 'uw2port')):
        sys.exit(f'fuzzasm.py: build the port first (make port{" coverage" if coverage else ""})')
    game = [p for p in sources.all_sources() if p.upper().endswith('.C') and not portcheck.dos_only(p)]
    game += sources.replay_sources()
    objs = [os.path.join(out, sources.stem(p) + '.o') for p in game]
    for p in portbuild.port_sources():
        if p.endswith(os.path.join('sys', 'main.c')): continue
        objs.append(os.path.join(out, 'port', os.path.relpath(p, portbuild.PORT).replace(os.sep, '_')[:-2] + '.o'))
    _, libs, extra = portbuild.sound_deps()
    objs += [os.path.join(out, 'deps', os.path.basename(p)[:-2] + '.o') for p in extra]
    host = os.path.join(root, 'build', 'fuzz' + ('-cov' if coverage else ''), 'fuzzhost')
    os.makedirs(os.path.dirname(host), exist_ok=True)
    src = os.path.join(here, 'fuzzhost.c')
    newest = max(os.path.getmtime(o) for o in objs + [src])
    if os.path.exists(host) and os.path.getmtime(host) >= newest: return host
    cov = ['-fprofile-instr-generate', '-fcoverage-mapping'] if coverage else []
    r = subprocess.run([portcheck.host_cc()] + portbuild.PORT_FLAGS + cov + ['-c', '-o', host + '.o', src], capture_output=True, text=True)
    if r.returncode: sys.exit('fuzzasm.py: fuzzhost.c does not compile:\n' + r.stderr[-3000:])
    r = subprocess.run([portcheck.host_cc(), '-o', host] + (['-fprofile-instr-generate'] if coverage else []) + [host + '.o'] + objs +
                       portbuild.pkg_config('--libs') + libs, capture_output=True, text=True)
    if r.returncode: sys.exit('fuzzasm.py: fuzzhost does not link:\n' + r.stderr[-3000:])
    return host


def fmt_regs(r, keys):
    return ' '.join(f'{k}={r[k]:x}' for k in keys)


def compare(t, regs_in, mem_in, dos, port):
    """'' when they agree, else what differs."""
    ds, dr, dm = dos
    ps, pr, pm = port
    if ds.startswith('int ') and ps.startswith('halt'): return ''    # a fault in both
    if ds != 'ret' or ps != 'ret': return f'DOS {ds}, port {ps}'
    bad = []
    for k in t.regs:
        a, b = dr[k], pr[k]
        if k in ('ds', 'es', 'ss', 'fs', 'gs'): a &= 0xFFFF; b &= 0xFFFF
        if a != b: bad.append(f'{k} {a:x} against {b:x}')
    for f in t.flags:
        a, b = dr['flags'] >> FLAGBITS[f] & 1, pr['flags'] >> FLAGBITS[f] & 1
        if a != b: bad.append(f'{f.upper()} {a} against {b}')
    if t.mem:
        sp = dr['esp'] & 0xFFFF
        ss = dr['ss'] & 0xFFFF
        dead = (ss * 16, ss * 16 + sp)     # below the final SP: what was pushed and popped
        def keep(a): return not (dead[0] <= a < dead[1]) and not any(lo <= a < hi for lo, hi in t.ignore)
        dk = {a: v for a, v in dm.items() if keep(a)}
        pk = {a: v for a, v in pm.items() if keep(a)}
        outside = [a for a in dk if not any(lo <= a < hi for lo, hi in REGIONS)]
        if outside: bad.append(f'DOS wrote outside the regions the port has: {min(outside):X}..{max(outside):X}')
        if dk != pk:
            diff = sorted(a for a in set(dk) | set(pk) if dk.get(a) != pk.get(a))
            bad.append(f'memory: {len(diff)} bytes differ, from {diff[0]:X} (DOS ' +
                       ' '.join(f'{dk.get(a, -1) & 0xFF:02x}' if a in dk else '--' for a in diff[:8]) + '; port ' +
                       ' '.join(f'{pk[a]:02x}' if a in pk else '--' for a in diff[:8]) + ')')
    return '; '.join(bad)


def main(argv):
    ap = argparse.ArgumentParser(description='Differential fuzzing of single routines, DOS bytes against the port.')
    ap.add_argument('--quick', action='store_true')
    ap.add_argument('--deep', action='store_true')
    ap.add_argument('--cases', type=int)
    ap.add_argument('--seed', type=int)
    ap.add_argument('--only')
    ap.add_argument('--list', action='store_true')
    ap.add_argument('--coverage', action='store_true')
    ap.add_argument('-v', action='store_true')
    a = ap.parse_args(argv)
    sys.stdout.reconfigure(line_buffering=True)
    if a.list:
        for t in TARGETS: print(f'{t.name:22s} {t.seg:04X}:{t.off:04X} {t.call:4s}  {t.what}')
        return 0
    n = a.cases or (10000 if a.deep else 200)
    seed = a.seed if a.seed is not None else (random.SystemRandom().getrandbits(31) if a.deep else 1)
    targets = [t for t in TARGETS if not a.only or any(t.name.startswith(o) for o in a.only.split(','))]
    host = build_host(a.coverage)
    env_cov = os.path.join(root, 'build', 'coverage', 'fuzz-%p.profraw')
    if a.coverage:
        os.makedirs(os.path.dirname(env_cov), exist_ok=True)
        os.environ['LLVM_PROFILE_FILE'] = env_cov
    t0 = time.time()
    dos = Dos(); port = Port(host); Port.image = dos.pristine
    print(f'fuzzasm: {len(targets)} routines, {n} cases each ({max(1, n // 5)} for the decoders), seed {seed}' +
          (' (--coverage)' if a.coverage else ''))
    total_bad = 0; faults = 0; cases_run = 0
    for t in targets:
        rng = random.Random(seed ^ zlib.crc32(t.name.encode()))
        bad = 0; tf = 0; wrote = 0; first = None; t1 = time.time()
        nt = max(1, n // 5) if t.heavy and not a.cases else n     # the decoders write up to 64 KB a case
        cases_run += nt
        for k in range(nt):
            regs, mem = t.gen(rng, k)
            d = dos.run(regs, mem, t.call, t.seg, t.off)
            p = port.run(regs, mem, t.port if t.port in ('near', 'far') else t.port, t.seg, t.off)
            if d[0].startswith('int'): tf += 1
            sp_end = d[1]['ss'] * 16 + (d[1]['esp'] & 0xFFFF)
            if any(not (d[1]['ss'] * 16 <= x < sp_end) for x in d[2]): wrote += 1
            why = compare(t, regs, mem, d, p)
            if why:
                bad += 1
                if first is None:
                    first = f'case {k}: in {fmt_regs(regs, ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp"))}' + \
                            (f', {sum(len(b) for _, b in mem)} bytes of memory' if mem else '') + f': {why}'
                if a.v: print(f'  {t.name} case {k}: {why}')
        faults += tf
        total_bad += bad
        print(f'{t.name:22s} {"ok  " if not bad else "DIFF"} {nt} cases{f", {tf} faulted in both" if tf else ""}'
              f'{f", {wrote} wrote memory" if wrote else ""}'
              f'{f", {bad} differ; first {first}" if bad else ""}  ({time.time() - t1:.1f} s)')
    port.close()
    print(f'fuzzasm: {len(targets)} routines, {cases_run} cases, {total_bad} differ, seed {seed}, '
          f'{time.time() - t0:.1f} s')
    return 1 if total_bad else 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
