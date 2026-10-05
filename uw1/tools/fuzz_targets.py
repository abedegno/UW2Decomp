"""UW1's targets for Exhume's tools/fuzzasm.py ([fuzz] targets in exhume.toml): routines of the
translated modules, run on the original's bytes (the user's UW.EXE) in Unicorn and on the port
in tools/fuzzhost.c with tools/fuzzhost-uw1.c (Exhume's docs/port.md, "Fuzzing single
routines"). UW2's are Exhume's examples/uw2/port/fuzz_targets.py; these are the same routines
in UW1, at UW1's addresses, with UW1's data offsets.

| Module | Targets | What they test |
| --- | --- | --- |
| IMATH (seg019 module 14) | 12 | sincos, fast_sincos, lsqrt, the arcsine, arccosine and atan2, near and far; cSinCos, cFstSinCos, cSqRt and cAtan2, the C entries through C3DENTRY's translation |
| INSTANCE (seg004) | 5 | mm3x9_bpsi, mm3x9t, mm9x9, code_pnt, mxmul |
| SPHERE (seg004) | 2 | sphere_check; get_dist |
| GRENTRY (seg003) | 7 | the frame buffer's span writers on a frame buffer laid out as setup_frame_buf lays it out, fill_fbuf, cDimFB, cLiteFB, and setup_frame_buf itself |
| EXPAND (seg004) | 7 | every image decoder, the palette builder and the record decoder |

In UW1 every one of these modules is translated by Exhume's tools/asm2c.py (tools/asm2c_spec.py),
so the fuzzing checks the translation and the spec's overrides (EXPAND's L01A6, INSTANCE's mxmul
body, GRENTRY's copier) on inputs no replay session gives them.

Each T gives the routine's code segment (an EXE paragraph) and offset, how DOS calls it and how
the port does, a generator of registers and memory, and what to compare.
"""
from fuzzasm import T, base_regs, setw, word, EDGE16, ALLFLAGS, GPR, LOAD, SCRATCH, STACK
import struct

S19 = 0x1F3A                           # seg019, the system library (IMATH, C3DENTRY)
S4 = 0x06E7                            # seg004, the 3D renderer
S3 = 0x0090                            # seg003, the graphics library
FD72 = 0x5624 + LOAD                   # seg063: IMATH's tables (DS while it runs)
SEG051 = 0x4723 + LOAD                 # seg051 (FD58), the renderer's data
SEG048 = 0x3963 + LOAD                 # seg048 (FD52), the graphics library's data
STDAT = 0x3F4B + LOAD                  # stdat (FD53), the frame buffer's segment
CMPBUF = 0x4423 + LOAD                 # cmpbuf1_start (FD54)
# the regions the port has (fuzzhost-uw1.c's fuzz_regions): memory outside them that the
# original changes is memory the port cannot model, and is reported
REGIONS = [(0x395B0 + LOAD * 16, 0x55DC4 + LOAD * 16), (FD72 * 16, FD72 * 16 + 0xAB0),
           (LOAD * 16, LOAD * 16 + 0x2F3A0),
           (SCRATCH * 16, SCRATCH * 16 + 0x10000), (STACK * 16, STACK * 16 + 0x10000)]


def g_imath(inputs):
    """IMATH: DS = seg063, the given 16-bit inputs, edge values first."""
    def gen(rng, k):
        r = base_regs(rng); r['ds'] = FD72
        for i, name in enumerate(inputs):
            v = word(rng, (k + i * 7) % (len(EDGE16) * 2) if k < len(EDGE16) * 2 else 99)
            setw(r, name, v)
        return r, []
    return gen


def g_sqrt(rng, k):
    """CX:BX for the square root: every magnitude, the four guesses' boundaries, the largest
    values (whose first quotient overflows: DOS faults)."""
    r = base_regs(rng); r['ds'] = FD72
    edges = [0, 1, 2, 3, 0xFF, 0x100, 0x101, 0xFFFF, 0x10000, 0xFFFFFF, 0x1000000, 0x3FFFFFFF, 0x40000000,
             0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, 0xFFFE0001, 0xFFFE0000]
    if k < len(edges): v = edges[k]
    else: v = rng.getrandbits(rng.choice([8, 16, 24, 32]))
    setw(r, 'cx', v >> 16); setw(r, 'bx', v)
    return r, []


def g_matrix(transposed=False):
    """mm3x9_bpsi and mm3x9t: a row of three 1.15 words and a 3 x 3 matrix, mostly in range,
    with overflowing products and 8000h now and then (the saturation paths)."""
    def gen(rng, k):
        r = base_regs(rng); r['ds'] = SCRATCH
        def v():
            return rng.choice([0x7FFF, 0x8000, 0x8001, 0xFFFF, 0]) if rng.random() < 0.25 else rng.getrandbits(16)
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
    """code_pnt and mxmul: a point in BX, CX, BP, edge values often, DS the renderer's data."""
    r = base_regs(rng); r['ds'] = SEG051
    for n in ('bx', 'cx', 'bp'):
        setw(r, n, word(rng, rng.randrange(len(EDGE16) * 2)))
    return r, []


def g_dist(rng, k):
    """get_dist: three 32-bit eye-relative coordinates at seg051:1D6h, 1DAh, 1DEh (UW2 ds:16h)."""
    r = base_regs(rng); r['ds'] = SEG051
    vals = [rng.choice([0, 1, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, rng.getrandbits(32),
                        rng.getrandbits(20), -rng.getrandbits(20) & 0xFFFFFFFF]) for _ in range(3)]
    return r, [(SEG051 * 16 + 0x1D6, struct.pack('<3I', *vals))]


def g_sphere(rng, k):
    """sphere_check: BX, AX, BP the object origin, SI the radius, CL the shift, against the view
    matrix the EXE's data holds."""
    r = base_regs(rng); r['ds'] = SEG051
    for n in ('bx', 'ax', 'bp'): setw(r, n, word(rng, rng.randrange(len(EDGE16) * 2)))
    setw(r, 'si', rng.choice([0, 1, 0x40, 0x100, 0x7FFF, rng.getrandbits(16)]))
    setw(r, 'cl', rng.randrange(0, 9))
    return r, []


# ---- GRENTRY: the frame buffer -----------------------------------------------------------------
# UW1's frame buffer is stdat's segment (seg048:095A holds it), 2667h words from offset 2, with
# the row table at seg048:095E (row y at 2 + y * (w + 2), rows past the height at 4CD0h, 200
# entries); the library's data sits 2 bytes above UW2's to 3838, 2 below from there on.
FB_END = 0x4CD0


def fb_frame(rng):
    """A frame w by h that fits the buffer, and its row table. Returns (w, h, preset)."""
    w = rng.choice([176, 176, 32, rng.randrange(8, 321)])
    hmax = min(200, (FB_END - 2) // (w + 2))
    h = rng.choice([min(112, hmax), hmax, rng.randrange(1, hmax + 1)])
    rows = [2 + y * (w + 2) for y in range(h)] + [FB_END] * (200 - h)
    return w, h, [(SEG048 * 16 + 0x95E, struct.pack('<200H', *rows))]


def spans(rng, w, h, n, extra):
    """n span records (y, left, right, then the extra words), left and right in either order,
    ended by a word with its top bit set."""
    out = b''
    for _ in range(n):
        a, b = rng.randrange(w), rng.randrange(w)
        out += struct.pack('<3H', rng.randrange(h), a, b) + struct.pack(f'<{len(extra)}H', *[f(rng) for f in extra])
    return out + struct.pack('<H', 0x8000 | rng.getrandbits(15))


def g_fb_solid(rng, k):
    """_D2D (fake_fb_solid_ylr), the solid span writer: 6-byte spans at seg048:4C00, the colour
    at 410F."""
    r = base_regs(rng); r['ds'] = SEG048
    w, h, pre = fb_frame(rng)
    sp = spans(rng, w, h, rng.randrange(0, 24), [])
    setw(r, 'si', 0x4C00)
    return r, pre + [(SEG048 * 16 + 0x4C00, sp), (SEG048 * 16 + 0x410F, bytes([rng.getrandbits(8)]))]


def g_fb_rows(rng, k):
    """_A5C and _A8D, linear bitmap rows into the frame buffer (8-byte records: y, left, right,
    the source's offset in the segment at seg048:55EA), read through SS = seg048, as the library
    runs them; the source, with zeros for _A8D's transparency, in the scratch segment."""
    r = base_regs(rng); r['ds'] = SCRATCH; r['ss'] = SEG048; r['esp'] = 0x4F9E
    w, h, pre = fb_frame(rng)
    recs = b''
    for _ in range(rng.randrange(0, 16)):
        a = rng.randrange(w); b = rng.randrange(a, w)
        recs += struct.pack('<4H', rng.randrange(h), a, b, rng.randrange(0, 0x0F00))
    recs += struct.pack('<H', 0x8000)
    src = bytes(rng.choice([0, 0, rng.getrandbits(8)]) for _ in range(0x1000))
    setw(r, 'si', 0x4C00)
    return r, pre + [(SEG048 * 16 + 0x4C00, recs), (SEG048 * 16 + 0x55EA, struct.pack('<H', SCRATCH)),
                     (SCRATCH * 16, src)]


def g_fb_smooth(rng, k):
    """_F77 (vga_smooth_ylrii), Gouraud spans into the frame buffer: 10-byte records (y, left,
    right, left and right intensity, 8.8 with the lightabs row in the high byte) at DS:SI on
    seg048 as the stack, the base colour at seg004:6950 (UW2 seg004:6D20)."""
    r = base_regs(rng); r['ds'] = SEG048; r['ss'] = SEG048; r['esp'] = 0x4F9E
    w, h, pre = fb_frame(rng)
    inten = lambda rng: rng.choice([0, 0x0F00, rng.randrange(0, 0x1000)])
    sp = spans(rng, w, h, rng.randrange(0, 12), [inten, inten])
    setw(r, 'si', 0x4C00)
    return r, pre + [(SEG048 * 16 + 0x4C00, sp), ((S4 + LOAD) * 16 + 0x6950, bytes([rng.getrandbits(8)]))]


def fb_pixels(rng):
    """Pixels in thirteen places of the frame buffer, for the dim and light passes."""
    return [(STDAT * 16 + 2 + i * 0x5E0, bytes(rng.getrandbits(8) for _ in range(64))) for i in range(13)]


def g_fb_al(rng, k):
    r = base_regs(rng); r['ds'] = SEG048
    setw(r, 'ax', rng.getrandbits(16))
    return r, []


def g_fb_dim(rng, k):
    r = base_regs(rng); r['ds'] = SEG048
    setw(r, 'cl', rng.randrange(0, 16)); setw(r, 'ch', rng.getrandbits(8))
    return r, fb_pixels(rng)


def g_fb_lite(rng, k):
    r = base_regs(rng); r['ds'] = SEG048
    setw(r, 'cx', rng.choice([1, 1, 2, 3]))
    return r, fb_pixels(rng)


def g_fb_setup(rng, k):
    """_B0C, setup_frame_buf (cPlaceFB): BX the width, CX the height (at least 1, at most the
    table's 200 rows), ES = DS = seg048; it patches the copier _BEC in the code segment."""
    r = base_regs(rng); r['ds'] = SEG048; r['es'] = SEG048
    setw(r, 'bx', rng.choice([176, 320, 32, rng.randrange(8, 321)]))
    setw(r, 'cx', rng.choice([112, 1, 200, rng.randrange(1, 201)]))
    return r, []


# ---- EXPAND: the image decoders --------------------------------------------------------------

def g_image(fmt):
    """An image for the EXPAND decoders: its size word and data at E000:0100 (AX the paragraph,
    BP the size word's offset), a 32 byte aux palette at E000:0000 (DS:SI), and DH a lightabs
    row (0..15, or FFh for none). The run-length forms get streams of valid words with every
    record kind: repeats, runs, the extended counts and the repeat-count records."""
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
        if fmt in (4, 10): data = bytes(rng.getrandbits(8) for _ in range(size))
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
    output (cmpbuf1_start:1800h, where exp_4run and exp_5run decode; UW2 5400h), BX 0."""
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
        r['ds'] = CMPBUF; r['es'] = CMPBUF
        setw(r, 'si', 0); setw(r, 'cx', len(words)); setw(r, 'di', 0x1800); setw(r, 'bx', 0)
        return r, [(CMPBUF * 16, bytes(words))]
    return gen


# L01A6, the byte the record decoder patches to a ret and back: the code has 2Bh there (sub ax,ax
# as 2B C0) and the decoder writes back 29h (sub ax,ax as 29 C0), so after the first repeat-count
# record DOS's code segment holds the other encoding of the same instruction; the translation
# (tools/asm2c_spec.py's override) tests the byte for C3h and leaves it as it was otherwise
L01A6 = [((S4 + LOAD) * 16 + 0x1A6, (S4 + LOAD) * 16 + 0x1A7)]
L01A6_NOTE = ('the byte at L01A6 is not compared: DOS restores it as 29h where the code had 2Bh, the same '
              'instruction (sub ax,ax) in its other encoding')
DECODER_NOTE = ('only AX (the pixels\' paragraph) and the memory are compared: every caller of uncmp_tab '
                '(do_uwobj, do_uwcrit, cFrmtoRaw) reads AX alone after the call')
NOFLAGS = ()
TARGETS = [
    # IMATH (seg019 module 14): the integer sines, square roots and arctangents
    T('imath.sincos_far', S19, 0xA34, 'far', g_imath(['bx']), what='sincos (_A34 -> _A38): the translation (sys/imath.c)'),
    T('imath.lsqrt_far', S19, 0xA30, 'far', g_sqrt, what='lsqrt (_A30 -> _A78): the translation'),
    T('imath.sincos', S19, 0xA38, 'near', g_imath(['bx']), what='_A38, sincos: the translation'),
    T('imath.fast_sincos', S19, 0xA69, 'near', g_imath(['bx']), what='_A69, fast_sincos: the translation'),
    T('imath.lsqrt', S19, 0xA78, 'near', g_sqrt, what='_A78, lsqrt: the translation'),
    T('imath.asin', S19, 0xB78, 'near', g_imath(['ax']), what='_B78, arcsine: the translation'),
    T('imath.acos', S19, 0xBA2, 'near', g_imath(['bx']), what='_BA2, arccosine: the translation'),
    T('imath.atan2', S19, 0xBD4, 'near', g_imath(['ax', 'bx']), what='_BD4, atan2: the translation'),
    # their C entries (sys/c3dentry.c into C3DENTRY's translation), against the routines
    T('imath.cSinCos', S19, 0xA38, 'near', g_imath(['bx']), port='csincos', regs=('eax', 'ebx'), flags=NOFLAGS,
      mem=False, what="cSinCos: sys/c3dentry.c's C entry, against _A38 (AX, BX the 16-bit results)",
      note='the C entry runs on the C stack and C3DENTRY\'s, not the caller\'s: only the results are compared'),
    T('imath.cFstSinCos', S19, 0xA69, 'near', g_imath(['bx']), port='cfst', regs=('eax', 'ebx'), flags=NOFLAGS,
      mem=False, what="cFstSinCos: the C entry, against _A69"),
    T('imath.cSqRt', S19, 0xA78, 'near', g_sqrt, port='csqrt', regs=('edi',), flags=NOFLAGS, mem=False,
      what="cSqRt: the C entry, against _A78 (DI)"),
    T('imath.cAtan2', S19, 0xBD4, 'near', g_imath(['ax', 'bx']), port='catan2', regs=('ecx',), flags=NOFLAGS,
      mem=False, what="cAtan2: the C entry, against _BD4 (CX)"),
    # INSTANCE (seg004 module 7): the 3 x 3 matrix products and the clip codes
    T('instance.mm3x9_bpsi', S4, 0x59DC, 'near', g_matrix(), what='mm3x9_bpsi: the translation (3d/instance.c)'),
    T('instance.mm3x9t', S4, 0x5B28, 'near', g_matrix(transposed=True), what='mm3x9t: the translation'),
    T('instance.mm9x9', S4, 0x58FA, 'near', g_mm9x9, what='mm9x9: the translation'),
    T('instance.code_pnt', S4, 0x587E, 'near', g_point, what='code_pnt, the clip codes: the translation'),
    T('instance.mxmul', S4, 0x50D8, 'near', g_point, what="mxmul with the EXE's body: the translation"),
    # SPHERE (seg004 module 4): the bounding-sphere test and the distance estimate
    T('sphere.sphere_check', S4, 0x1370, 'near', g_sphere, what='sphere_check: the translation (3d/sphere.c)'),
    T('sphere.get_dist', S4, 0x151F, 'near', g_dist, what='get_dist: the translation'),
    # GRENTRY (seg003 module 6): the frame buffer
    T('grentry.solid_spans', S3, 0xD2D, 'near', g_fb_solid, what='_D2D, the solid span writer: the translation (gfx/grentry.c)'),
    T('grentry.rows', S3, 0xA5C, 'near', g_fb_rows, what='_A5C, bitmap rows into the frame buffer: the translation'),
    T('grentry.rows_x', S3, 0xA8D, 'near', g_fb_rows, what='_A8D, the same skipping colour 0'),
    T('grentry.smooth', S3, 0xF77, 'near', g_fb_smooth, what='_F77, Gouraud spans: the translation'),
    T('grentry.fill', S3, 0xA48, 'far', g_fb_al, heavy=True, what='_A48, fill_fbuf (cFillFB): the translation'),
    T('grentry.dim', S3, 0xAC8, 'far', g_fb_dim, heavy=True, what='_AC8, cDimFB: the translation'),
    T('grentry.lite', S3, 0xAEC, 'far', g_fb_lite, heavy=True, what='_AEC, cLiteFB: the translation'),
    T('grentry.setup', S3, 0xB0C, 'far', g_fb_setup, what='_B0C, setup_frame_buf (cPlaceFB): the translation, '
      'with the copier patch the override handles'),
    # EXPAND (seg004 module 1): the image decoders
    T('expand.pal_63', S4, 0x63, 'near', g_pal, what='_63, uncmp_pal: the translation (3d/expand.c)'),
    T('expand.exp_8str', S4, 0x20, 'near', g_image(4), regs=('eax',), flags=NOFLAGS,
      what='exp_8str: the translation', note=DECODER_NOTE),
    T('expand.exp_4str', S4, 0x39, 'near', g_image(10), regs=('eax',), flags=NOFLAGS,
      what='exp_4str: the translation', note=DECODER_NOTE),
    T('expand.exp_4run', S4, 0xD9, 'near', g_image(8), regs=('eax',), flags=NOFLAGS, ignore=L01A6,
      what='exp_4run: the translation', note=DECODER_NOTE, heavy=True),
    T('expand.exp_5run', S4, 0x11C, 'near', g_image(6), regs=('eax',), flags=NOFLAGS, ignore=L01A6,
      what='exp_5run: the translation', note=DECODER_NOTE, heavy=True),
    T('expand.records4', S4, 0x18D, 'near', g_records(4), ignore=L01A6,
      what='_18D, the record decoder: the translation', note=L01A6_NOTE, heavy=True),
    T('expand.records5', S4, 0x18D, 'near', g_records(5), ignore=L01A6,
      what='_18D on 5-bit words', note=L01A6_NOTE, heavy=True),
]
