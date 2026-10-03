"""First-draft TASM 2.0 source for a DOS assembly segment, from the EXE bytes.
usage: asmgen.py SEGNAME [--fix] [--keep-db] [--no-near-names] > src/SEGxxx.ASM

The target table (targets/SEGNAME.tsv) gives base, size, org and the procs; the IDA listing
(uw2_asm.asm, or UW2_ASM) tells code from data; symbols.tsv names far calls, segment values,
near data and the procs themselves. Resident segments only: relocations come from the MZ
header. Notes on stderr: procs whose symbols.tsv name differs from the table, names longer
than match.py looks up, and table procs that fall inside an instruction.

- Code is decoded with iced-x86 where IDA has instructions and at every table proc; the rest
  is db/dw, split at labels, with relocated words as `dw seg NAME`. IDA's undefined `db` runs
  that disassemble cleanly up to a return or jump (jmp $+2 doesn't count) are decoded as code
  too (unreached handlers); --keep-db leaves them as data. Anything beyond 386/387 stops a
  decode (data that looks like code).
- Labels: table procs (public, `_name`, near or far by their first return), near branch
  targets, cs: data references inside the segment and far calls into it (L + segment
  offset in hex); a target inside an instruction becomes `L equ $+n`.
- TASM 2.0 single pass: a forward jmp/jcc followed by its nop padding is written plainly and
  the nops dropped; an unpadded forward one is written `short`; backward ones plainly, except a
  near one TASM would shorten, written as bytes (`db 0E9h` / `dw L-$-2`). A forward
  `push cs; call near; nop` is `call far ptr P`, a backward `push cs; call near` to a far proc
  is `call P`, a near call to a far proc is `call near ptr P`. A far call (9A) into the segment
  itself is written as bytes with `dw offset`/`dw seg`, since TASM always shortens it.
- One-line sources TASM expands: the 8086 `push imm` (push ax; push bp; mov bp,sp;
  mov [bp+2],imm; pop bp) when no .186 is needed, and POP_F (80 CF 00 0E E8 FA FF, the
  80286 popf workaround), defined as a macro in the header when used.
- Spelling TASM needs for the same bytes: `retn` for C3 in a far proc, negative constants for
  sign-extended byte immediates (`and cx,-4`; `0FFFCh` gives the 81 form), explicit `ptr`
  sizes everywhere.
- Encodings TASM never produces (`01 /r` register adds, disp16 for a small displacement,
  redundant prefixes, FPU forms without fwait, ...) can't be predicted reliably. --fix
  assembles the draft in DOS (tools/tcc.mjs), compares it item by item with the EXE, and
  rewrites what differs: jumps get `short` or bytes, anything else becomes `db` with the
  instruction as a comment (fixups kept as dw/dd fields). Assembly errors on a line turn that
  item into `db` too. It repeats until nothing changes (two or three rounds, about 7 s each).
- Far calls and segment values become extrn symbols named from symbols.tsv; unknown ones get
  provisional names from the IDA proc name at that address, else IDA segment + offset
  (`_seg003_0272_7A8`, `dseg062_62a6` for a bare segment). Near data with no segment
  override is named from symbols.tsv where it names that exact DS address, otherwise left
  numeric; --no-near-names leaves it all numeric, for modules that point DS at their own
  segment (seg021).
"""
import sys, os, re, struct, subprocess, tempfile, shutil
from iced_x86 import (Decoder, Formatter, FormatterSyntax, OpKind, Register, FlowControl,
                      CpuidFeature, MemorySizeOptions, Mnemonic)
here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
sys.path.insert(0, here)
from match import load_targets, EXE

ASM = os.path.expanduser(os.environ.get('UW2_ASM', '~/UWReverseEngineering/uw2_asm.asm'))
DGROUP_PARA = 0x65E9          # verify.py: DS_FILE 0x68A90 = header + 0x65E9 * 16
IDA_BIAS = 0x1ED              # IDA's segment names carry load paragraph + 0x1ED

exe = open(EXE, 'rb').read()
hdr = struct.unpack_from('<H', exe, 8)[0] * 16
word = lambda o: struct.unpack_from('<H', exe, o)[0]


def relocs():
    n = word(6); t = word(0x18)
    return {hdr + word(t + 4 * i + 2) * 16 + word(t + 4 * i) for i in range(n)}


def load_symbols():
    ds, far = {}, {}
    for l in open(os.path.join(root, 'symbols.tsv')):
        if l.startswith('#') or not l.strip(): continue
        n, a = l.rstrip('\n').split('\t')[:2]
        s, o = a.split(':')
        if s == 'DS': ds.setdefault(int(o, 16), n)
        else: far.setdefault((int(s, 16), int(o, 16)), n)
    return ds, far


def ida_segments():
    """IDA segment name per load paragraph, and the listing's line range per segment."""
    names = {}; ranges = {}; cur = None
    for k, l in enumerate(open(ASM, encoding='latin1')):
        m = re.match(r'^(\w+)\s+segment\b', l)
        if m:
            cur = m.group(1); ranges[cur] = [k, None]
            p = re.search(r'_([0-9A-Fa-f]{4})$', cur)
            if p: names.setdefault(int(p.group(1), 16) - IDA_BIAS, cur)
            continue
        m = re.match(r'^(\w+)\s+ends\b', l)
        if m and m.group(1) in ranges: ranges[m.group(1)][1] = k
    for l in open(os.path.join(root, 'map', 'segments.tsv')):
        if l.startswith('#'): continue
        f = l.rstrip('\n').split('\t')
        if len(f) > 1 and f[1] and int(f[1], 16) < MZEND:
            names.setdefault((int(f[1], 16) - hdr) // 16, f[0])
    return names, ranges


cblp, cp = struct.unpack_from('<HH', exe, 2); MZEND = (cp - 1) * 512 + cblp


def ida_procs():
    by = {}
    for l in open(os.path.join(root, 'map', 'procs.tsv')):
        if l.startswith('#'): continue
        f = l.rstrip('\n').split('\t')
        if len(f) > 2 and f[2] and f[2] != 'None':
            by.setdefault(f[0], {})[int(f[2], 16)] = f[1]
    return by


def data_size(kind, body):
    """Bytes in an IDA db/dw/dd operand list, or None if it can't be parsed."""
    unit = {'db': 1, 'dw': 2, 'dd': 4}[kind]
    body = body.split(';')[0].strip()
    items = []; cur = ''; q = None
    for ch in body:
        if q:
            cur += ch
            if ch == q: q = None
        elif ch in '\'"': q = ch; cur += ch
        elif ch == ',': items.append(cur.strip()); cur = ''
        else: cur += ch
    if cur.strip(): items.append(cur.strip())
    n = 0
    for it in items:
        m = re.match(r'^([0-9A-F]+h?|\d+)\s+dup\s*\((.*)\)$', it, re.I)
        if m:
            c = m.group(1); c = int(c[:-1], 16) if c.lower().endswith('h') else int(c)
            inner = data_size(kind, m.group(2))
            if inner is None: return None
            n += c * inner
        elif it[:1] in '\'"' and kind == 'db':
            n += len(it[1:-1].replace(it[0] * 2, it[0]))
        elif it:
            n += unit
    return n


def parse_ida(seg, rng, segbase, lo, hi):
    """Offsets where IDA lists instructions, and its data items: (start, size, text)."""
    code = set(); data = []; cur = None; tag = seg.split('_')[0]
    lines = open(ASM, encoding='latin1').read().split('\n')[rng[0] + 1:rng[1]]
    def offname(n):
        m = re.search(r'_([0-9A-F]+)$', n)
        return int(m.group(1), 16) if m and tag in n else None
    for l in lines:
        s = l.split(';')[0].rstrip()
        if not s.strip():
            m = re.match(r'^;org\s+([0-9A-F]+)', l)
            if m: cur = int(m.group(1), 16)
            continue
        m = re.match(r'^(\w+):', s)
        if m:
            o = offname(m.group(1))
            if o is not None: cur = o
            s = s[m.end():].strip()
            if not s: continue
        m = re.match(r'^(\w+)\s+(proc|endp)\b', s)
        if m:
            if m.group(2) == 'proc':
                o = offname(m.group(1))
                if o is not None: cur = o
            continue
        m = re.match(r'^(?:(\w+)\s+)?(db|dw|dd)\s+(.*)$', s.strip())
        if m:
            if m.group(1):
                o = offname(m.group(1))
                if o is not None: cur = o
            if cur is None: continue
            n = data_size(m.group(2), m.group(3))
            if n is None: continue
            data.append((cur, n, s.strip())); cur += n
            continue
        w = s.split()
        if not w or w[0] in ('assume', 'public', 'extrn', 'org') or '=' in s: continue
        if w[0] == 'align':
            if cur is not None:
                a = int(w[1][:-1], 16) if w[1].lower().endswith('h') else int(w[1]); cur = (cur + a - 1) // a * a
            continue
        if cur is None: cur = lo
        if lo <= cur < hi:
            code.add(cur)
            i = next(iter(Decoder(16, exe[segbase + cur:segbase + hi], ip=cur)))
            cur += i.len
    return code, data


class Gen:
    def __init__(s, seg, keep_db=False, no_near_names=False):
        s.no_near_names = no_near_names
        s.seg = seg
        s.base, s.size, s.rows, s.org = load_targets(seg)
        s.para = (s.base - s.org - hdr) // 16
        s.segbase = s.base - s.org            # file offset of the segment's paragraph
        s.lo, s.hi = s.org, s.org + s.size    # segment offsets this file covers
        s.rel = {o - s.segbase for o in relocs() if s.base <= o < s.base + s.size}
        s.dsyms, s.fsyms = load_symbols()
        s.segnames, ranges = ida_segments()
        s.procs_by = ida_procs()
        if seg not in ranges: sys.exit(f'{seg}: not in the IDA listing')
        s.code, s.data = parse_ida(seg, ranges[seg], s.segbase, s.lo, s.hi)
        s.keep_db = keep_db
        s.fix = {}                            # segment offset -> 'db' | 'short' | 'plain'
        s.decode()

    def b(s, o, n=1): return exe[s.segbase + o:s.segbase + o + n]

    # ---------- decoding and classification
    def decode(s):
        starts = set(s.code)
        starts.update(r[2] + s.org for r in s.rows)        # the table's procs are code
        if not s.keep_db:
            # IDA's undefined byte runs (not dup fills) that decode cleanly to a return/jump
            # only at the start of a run: a named line, or one not continuing the previous
            prev_end = None
            for st, n, t in s.data:
                cont = st == prev_end and re.match(r'^db\s', t)
                prev_end = st + n
                if cont or not re.match(r'^(\w+\s+)?db\s', t) or "'" in t: continue
                if s.b(st)[0] == 0 or any(st <= c < st + n for c in s.code): continue
                s.try_code(st, starts)
        # decode every start, following fallthrough until a known start or data
        s.ins = {}
        datastart = {st for st, n, t in s.data}
        todo = sorted(starts)
        for st in todo:
            o = st
            while s.lo <= o < s.hi and o not in s.ins:
                i = next(iter(Decoder(16, s.b(o, s.hi - o), ip=o)))
                if not plausible(i) or o + i.len > s.hi: break
                s.ins[o] = i
                o += i.len
                if o in starts or o in datastart and o not in starts: break
                fc = i.flow_control
                if fc in (FlowControl.RETURN, FlowControl.UNCONDITIONAL_BRANCH, FlowControl.INDIRECT_BRANCH, FlowControl.INTERRUPT) and i.mnemonic in (Mnemonic.IRET, Mnemonic.RET, Mnemonic.RETF, Mnemonic.JMP, Mnemonic.JMPE):
                    if o not in starts: break
        # instruction coverage
        s.covered = {}
        for o, i in s.ins.items():
            for k in range(o, o + i.len): s.covered[k] = o

    def try_code(s, st, starts):
        o = st; got = []
        while s.lo <= o < s.hi:
            i = next(iter(Decoder(16, s.b(o, s.hi - o), ip=o)))
            if not plausible(i) or s.b(o, 2) == b'\0\0': return
            got.append(o); o += i.len
            # jmp $+2 is an I/O delay, not the end of the code
            if i.mnemonic == Mnemonic.JMP and i.op_count and i.op_kind(0) == OpKind.NEAR_BRANCH16 and i.near_branch_target == o: continue
            if i.mnemonic in (Mnemonic.IRET, Mnemonic.RET, Mnemonic.RETF, Mnemonic.JMP):
                starts.update(got); return
            if o in starts: starts.update(got); return

    # ---------- names
    def segname(s, para):
        if para == s.para: return '@code'
        if para == DGROUP_PARA: return '@data'
        return s.segnames.get(para, f'seg_{para:04X}')

    def farname(s, para, off):
        n = s.fsyms.get((para, off))
        if n: return n
        sn = s.segnames.get(para, f'seg_{para:04X}')
        p = s.procs_by.get(sn, {}).get(off)
        return '_' + (p if p else f'{sn}_{off:X}')

    def segref(s, para, nearby):
        """A name for a relocated segment word: a known symbol in that segment whose offset
        appears nearby, else the segment's provisional name."""
        if para in (s.para, DGROUP_PARA): return s.segname(para)
        for v in nearby:
            n = s.fsyms.get((para, v))
            if n: s.externs[n] = 'far'; return n
        n = s.segname(para); s.externs[n] = 'far'; return n

    # ---------- emit
    def run(s):
        lines = []; s.externs = {}; s.items = []; s.placed = set(); s.curdist = None
        procs = {r[2] + s.org: r for r in s.rows}
        # proc distance: far if it holds a far return before any near one
        s.dist = {}
        starts = sorted(procs)
        for k, st in enumerate(starts):
            end = starts[k + 1] if k + 1 < len(starts) else s.hi
            d = None
            for o in sorted(x for x in s.ins if st <= x < end):
                m = s.ins[o].mnemonic
                if m == Mnemonic.RETF: d = 'far'; break
                if m == Mnemonic.RET: d = 'near'; break
                if m == Mnemonic.IRET: d = 'far'; break
            # no return of its own (it runs on into the next): far only if called far
            s.dist[st] = d or ('far' if any(s.b(x)[0] == 0x0E and x + 1 in s.ins and s.branch_target(s.ins[x + 1]) == st for x in s.ins) else 'near')
        # symbols.tsv knows the names callers link against; the table may still hold IDA's
        s.pname = {o: s.fsyms.get((s.para, o), '_' + r[0]) for o, r in procs.items()}
        for o, r in procs.items():
            if s.pname[o] != '_' + r[0]:
                print(f'note: +{o:X} is {s.pname[o]} in symbols.tsv, {r[0]} in the table', file=sys.stderr)
            if len(s.pname[o]) > 33:
                print(f'note: {s.pname[o]} is longer than match.py looks up (32 characters)', file=sys.stderr)
        # labels: branch targets and cs: data references inside the segment
        cpu = s.cpu(); s.cpuname = cpu
        s.special = s.find_special()
        inside = {k for o, (kind, n, _) in s.special.items() for k in range(o + 1, o + n)}
        s.labels = {}
        for o, i in s.ins.items():
            if o in inside: continue
            t = s.branch_target(i)
            if t is not None and s.lo <= t < s.hi: s.labels[t] = s.lab(t)
            if s.memtarget(i) is not None: s.labels[s.memtarget(i)] = s.lab(s.memtarget(i))
            if i.op_count and i.op_kind(0) == OpKind.FAR_BRANCH16 and o + 3 in s.rel and i.far_branch_selector == s.para and s.lo <= i.far_branch16 < s.hi:
                s.labels[i.far_branch16] = s.lab(i.far_branch16)
        for st, n, t in s.data:
            for m in re.finditer(r'offset\s+(\w+)', t):
                mo = re.search(r'_([0-9A-F]+)$', m.group(1))
                if mo and s.lo <= int(mo.group(1), 16) < s.hi: s.labels[int(mo.group(1), 16)] = s.lab(int(mo.group(1), 16))
        body = []
        o = s.lo; curproc = None
        fmt = Formatter(FormatterSyntax.MASM)
        fmt.memory_size_options = MemorySizeOptions.ALWAYS
        fmt.uppercase_hex = True; fmt.hex_suffix = 'h'; fmt.hex_prefix = ''
        fmt.space_after_operand_separator = False
        fmt.show_branch_size = False
        s.fmt = fmt
        while o < s.hi:
            if o in procs:
                if curproc is not None: body.append(f'{s.pname[curproc]} endp'); body.append('')
                curproc = o; r = procs[o]
                body.append(f'; {r[1]}  (+{o:X})')
                body.append(f'{s.pname[o]} proc {s.dist[o]}'); s.placed.add(o); s.curdist = s.dist[o]
            elif o in s.labels and o not in procs:
                body.append(f'{s.labels[o]}:')
            s.line0 = len(body)
            if o in s.special and s.fix.get(o) is None:
                kind, n, txt = s.special[o]
                body.append('        ' + txt); s.items.append(('multi', o, n, s.line0))
            elif o in s.ins and s.fix.get(o) != 'data':
                for t2 in sorted(x for x in s.labels if o < x < o + s.ins[o].len):
                    body.append(f'{s.labels[t2]} equ $+{t2 - o}')
                n = s.emit_ins(o, body)
            else:
                n = s.emit_data(o, body)
            o += n
        if curproc is not None: body.append(f'{s.pname[curproc]} endp')
        out = [f'; /* target: {s.seg} */', '; /* opts: /ml */', ';', f'; First draft from tools/asmgen.py: {s.seg}, {s.size:#x} bytes at file {s.base:#x}, org {s.org:#x}.', '',
               '        .model  medium']
        if cpu: out.append(f'        {cpu}')
        out.append('')
        if any(k == 'popf' for k, n, _ in s.special.values()):
            out += ['; popf without the early 80286 bug: iret pops the flags pushf saved',
                    'POP_F   macro',
                    '        local   iret_op',
                    '        db      80h                     ; or bh,0 ...',
                    'iret_op:',
                    '        db      0CFh,0                  ; ... hiding an iret',
                    '        push    cs',
                    '        call    near ptr iret_op',
                    '        endm', '']
        fars = sorted(n for n, k in s.externs.items() if k == 'far')
        for n in fars: out.append(f'        extrn   {n}:far')
        near = sorted(n for n, k in s.externs.items() if k == 'near')
        if near:
            out += ['', '        .data']
            for n in near: out.append(f'        extrn   {n}:byte')
        out += ['', '        .code', '']
        for o2 in sorted(procs):
            if o2 in s.placed: out.append(f'        public  {s.pname[o2]}')
            else: print(f'note: {s.pname[o2]} (+{o2:X}) falls inside an instruction or data item; not placed', file=sys.stderr)
        out.append('')
        s.hdrlines = len(out)
        out += body
        out += ['', '        end']
        return '\n'.join(out) + '\n'

    def find_special(s):
        """Byte patterns TASM generates from one source line."""
        sp = {}
        for o in sorted(s.ins):
            b = s.b(o, 10)
            if b[:7] == bytes.fromhex('80CF000EE8FAFF'):
                sp[o] = ('popf', 7, 'POP_F')
            elif s.cpuname == '' and b[:7] == bytes.fromhex('50558BECC74602') and b[9] == 0x5D:
                # TASM's 8086 `push imm`: push ax; push bp; mov bp,sp; mov [bp+2],imm; pop bp
                v = b[7] | b[8] << 8
                if o + 7 in s.rel:
                    para = word(s.segbase + o + 7); nm = s.segref(para, [])
                    txt = f'push    {nm}' if nm.startswith('@') else f'push    seg {nm}'
                else:
                    txt = f'push    {hx(v)}'
                sp[o] = ('pushimm', 10, txt)
        return sp

    def lab(s, t):
        return s.pname.get(t) if t in getattr(s, 'pname', {}) else f'L{t:04X}'

    def cpu(s):
        lvl = 0
        for i in s.ins.values():
            for f in i.cpuid_features():
                if f in (CpuidFeature.INTEL386, CpuidFeature.INTEL486): lvl = max(lvl, 386)
                elif f == CpuidFeature.INTEL286: lvl = max(lvl, 286)
                elif f == CpuidFeature.INTEL186: lvl = max(lvl, 186)
            for k in range(i.op_count):
                if i.op_kind(k) == OpKind.REGISTER and Register.EAX <= i.op_register(k) <= Register.R15D: lvl = 386
            if i.memory_base in range(Register.EAX, Register.R15D + 1): lvl = 386
            if i.op_code().operand_size == 32 or i.op_code().address_size == 32: lvl = 386
        return {0: '', 186: '.186', 286: '.286', 386: '.386'}[lvl]

    def branch_target(s, i):
        if i.op_count and i.op_kind(0) in (OpKind.NEAR_BRANCH16, OpKind.NEAR_BRANCH32):
            return i.near_branch_target
        return None

    def memtarget(s, i):
        """A cs: memory reference to a place inside the segment."""
        for k in range(i.op_count):
            if i.op_kind(k) == OpKind.MEMORY and i.memory_segment == Register.CS and i.memory_base != Register.BP:
                d = i.memory_displacement & 0xFFFF
                if s.lo <= d < s.hi and (d not in s.covered or s.covered[d] == d): return d
        return None

    def emit_data(s, o, body):
        # up to the next label, instruction or relocated word boundary
        end = o + 1
        while end < s.hi and end not in s.labels and end not in s.procs_set() and (end not in s.ins or s.fix.get(end) == 'data'):
            end += 1
        n = end - o
        k = o; row = []
        def flush():
            if row: body.append('        db      ' + ','.join(row)); row.clear()
        while k < end:
            if k in s.rel or k + 1 in s.rel and k + 1 < end:
                if k + 1 in s.rel: row.append(hb(s.b(k)[0])); k += 1; continue
                flush()
                para = word(s.segbase + k)
                body.append(f'        dw      {s.dataseg(para)}')
                k += 2; continue
            # printable text as a string
            z = k
            while z < end and 0x20 <= s.b(z)[0] < 0x7F and s.b(z) != b"'" and z not in s.rel and z + 1 not in s.rel: z += 1
            txt = s.b(k, z - k).decode('latin1') if z > k else ''
            if z - k >= 4 and sum(c.isalpha() or c in " .,!?'-:" for c in txt) * 10 >= len(txt) * 8:
                flush(); body.append(f"        db      '{txt}'"); k = z; continue
            # long zero runs as dup
            z = k
            while z < end and s.b(z)[0] == 0 and z not in s.rel: z += 1
            if z - k >= 8:
                flush(); body.append(f'        db      {hx(z - k)} dup (0)'); k = z; continue
            row.append(hb(s.b(k)[0])); k += 1
            if len(row) == 12: flush()
        flush()
        s.items.append(('data', o, n, s.line0))
        return n

    def dataseg(s, para):
        if para == DGROUP_PARA: return 'DGROUP'
        if para == s.para: return '@code'
        n = s.segname(para); s.externs[n] = 'far'; return f'seg {n}'

    def procs_set(s):
        if not hasattr(s, '_ps'): s._ps = {r[2] + s.org for r in s.rows}
        return s._ps

    def emit_ins(s, o, body):
        i = s.ins[o]; raw = s.b(o, i.len); n = i.len
        f = s.fix.get(o)
        mn = i.mnemonic
        t = s.branch_target(i)
        # padding nops after a forward jump
        pad = 0
        if t is not None and t > o and mn != Mnemonic.CALL and not (Mnemonic.LOOP <= mn <= Mnemonic.LOOPNE) and mn not in (Mnemonic.JCXZ, Mnemonic.JECXZ):
            while s.b(o + n + pad)[0] == 0x90 and o + n + pad in s.ins and o + n + pad not in s.labels and pad < 2:
                pad += 1
            want = 1 if mn == Mnemonic.JMP else 2
            if raw[0] in (0xE9,) or raw[0] == 0x0F: pad = 0
            elif pad < want: pad = 0
            else: pad = want
        # push cs; call near
        merged = 0
        if f == 'db':
            body.append(s.dbline(o, i)); s.items.append(('ins', o, n, s.line0)); return n
        if raw[0] == 0x0E and o + 1 in s.ins and s.ins[o + 1].mnemonic == Mnemonic.CALL and s.b(o + 1)[0] == 0xE8 and f != 'plain':
            c = s.ins[o + 1]; ct = c.near_branch_target
            if ct in s.pname and s.dist.get(ct) == 'far':
                if ct < o:
                    body.append(f'        call    {s.pname[ct]}'); s.items.append(('ins', o, 4, s.line0)); return 4
                if s.b(o + 4)[0] == 0x90 and o + 4 in s.ins:
                    body.append(f'        call    far ptr {s.pname[ct]}'); s.items.append(('ins', o, 5, s.line0)); return 5
        if t is not None:
            name = s.labels.get(t) or s.pname.get(t)
            m = s.fmt.format_mnemonic(i)
            if name is None:
                body.append(s.dbline(o, i, '; target outside the segment')); s.items.append(('ins', o, n, s.line0)); return n
            if mn == Mnemonic.CALL:
                if t in s.pname and s.dist.get(t) == 'far': body.append(f'        call    near ptr {name}')
                else: body.append(f'        call    {name}')
            elif Mnemonic.LOOP <= mn <= Mnemonic.LOOPNE or mn in (Mnemonic.JCXZ, Mnemonic.JECXZ):
                body.append(f'        {m:<8}{name}')
            else:
                if f == 'nearbytes' or (t <= o and len(raw) > 2 and raw[0] in (0xE9, 0x0F) and -128 <= t - (o + 2) <= 127):
                    # TASM shortens backward jumps it can reach; a near one needs bytes
                    op = 'db      0E9h' if raw[0] == 0xE9 else f'db      0Fh,{hb(raw[1])}'
                    body.append(f'        {op}'); body.append(f'        dw      {name}-$-2')
                elif t > o and pad == 0 and len(raw) == 2 or f == 'short':
                    body.append(f'        {m:<8}short {name}')
                else:
                    body.append(f'        {m:<8}{name}')
            s.items.append(('ins', o, n + pad, s.line0)); return n + pad
        text = s.format(i, o)
        if text is None:
            body.append(s.dbline(o, i)); s.items.append(('ins', o, n, s.line0)); return n
        body.append('        ' + text)
        s.items.append(('ins', o, n, s.line0)); return n

    def fields(s, i, o):
        """Relocated or symbolic fields in an instruction: (byte offset, size, text)."""
        dec = Decoder(16, s.b(o, i.len), ip=o); co = dec.get_constant_offsets(dec.decode()); out = []
        if co.has_immediate:
            io = co.immediate_offset
            if i.op_count and i.op_kind(0) == OpKind.FAR_BRANCH16:
                off = i.far_branch16; para = i.far_branch_selector
                if o + 3 in s.rel and para == s.para and s.lo <= off < s.hi:
                    # a far call into this segment: TASM would make it push cs; call near
                    out.append((1, 4, s.labels.get(off) or s.lab(off), 'self'))
                elif o + 3 in s.rel:
                    n = s.farname(para, off); s.externs[n] = 'far'
                    out.append((1, 4, f'{n}', 'far'))
                return out
            if co.immediate_size == 2 and o + io in s.rel:
                para = word(s.segbase + o + io)
                near = [x.immediate16 for x in s.near_ins(o) if x.op_count > 1 and x.op_kind(1) == OpKind.IMMEDIATE16]
                out.append((io, 2, s.segref(para, near), 'seg'))
        if co.has_displacement and co.displacement_size == 2:
            do = co.displacement_offset
            if o + do in s.rel:
                out.append((do, 2, None, 'relocdisp'))
        return out

    def near_ins(s, o):
        ks = sorted(s.ins); j = ks.index(o)
        return [s.ins[k] for k in ks[max(0, j - 3):j + 4] if k != o]

    def format(s, i, o):
        fl = s.fields(i, o)
        if any(f[3] == 'relocdisp' for f in fl): return None
        if fl and fl[0][3] == 'self':
            return f'db      {hb(s.b(o)[0])}                     ; far {s.fmt.format_mnemonic(i)} into this segment, which TASM would shorten\n        dw      offset {fl[0][2]}\n        dw      seg {fl[0][2]}'
        if i.mnemonic == Mnemonic.CALL and i.op_count and i.op_kind(0) == OpKind.FAR_BRANCH16:
            if not fl: return None
            return f'call    far ptr {fl[0][2]}'
        if i.mnemonic == Mnemonic.JMP and i.op_count and i.op_kind(0) == OpKind.FAR_BRANCH16:
            if not fl: return None
            return f'jmp     far ptr {fl[0][2]}'
        txt = s.fmt.format(i)
        mn = s.fmt.format_mnemonic(i)
        if i.mnemonic == Mnemonic.RET and s.curdist == 'far':
            # a plain ret in a far proc assembles as retf
            return 'retn' + txt[3:]
        ops = txt[len(mn):].strip()
        for f in fl:
            if f[3] == 'seg':
                v = f'{i.immediate16:X}h'
                nm = f[2]
                rep = nm if nm.startswith('@') else f'seg {nm}'
                ops = re.sub(r'\b0?' + re.escape(v) + r'\b', rep, ops)
        # a sign-extended byte immediate: TASM picks the short form only for a constant
        # written in -128..127, so 0FFFCh must be spelled -4
        for k in range(i.op_count):
            if i.op_kind(k) in (OpKind.IMMEDIATE8TO16, OpKind.IMMEDIATE8TO32) and i.immediate8 >= 0x80:
                big = f'{i.immediate(k) & (0xFFFF if i.op_kind(k) == OpKind.IMMEDIATE8TO16 else 0xFFFFFFFF):X}h'
                ops = re.sub(r'\b0?' + big + r'(?![\w])', str(i.immediate8 - 256), ops)
        # memory operands
        m = re.search(r'(?:(byte|word|dword|qword|fword|tbyte) ptr )?(?:(\w\w):)?\[([^\]]*)\]', ops)
        if m and i.op_count and any(i.op_kind(k) == OpKind.MEMORY for k in range(i.op_count)):
            ops = ops[:m.start()] + s.memtext(i, m) + ops[m.end():]
        # string instructions: iced writes the operands; TASM wants the short form when plain
        if i.is_string_instruction if hasattr(i, 'is_string_instruction') else False: pass
        if mn in ('movs', 'lods', 'stos', 'scas', 'cmps', 'ins', 'outs'):
            if i.segment_prefix == Register.NONE:
                suf = {1: 'b', 2: 'w', 4: 'd'}[i.memory_size_info if False else s.strsize(i)]
                pre = 'rep ' if i.has_rep_prefix else 'repne ' if i.has_repne_prefix else ''
                return f'{pre}{mn}{suf}'
        line = f'{mn:<7} {ops}' if ops else mn
        return line

    def strsize(s, i):
        from iced_x86 import MemorySizeExt
        try: return MemorySizeExt.size(i.memory_size)
        except Exception: return 1

    def memtext(s, i, m):
        size, seg, inner = m.group(1), m.group(2), m.group(3)
        d = i.memory_displacement & 0xFFFF
        regs = [r for r in re.split(r'[+-]', inner) if r in ('bx', 'bp', 'si', 'di')]
        pfx = (size + ' ptr ') if size else ''
        segp = (seg + ':') if seg else ''
        name = None
        if i.memory_segment == Register.CS and s.memtarget(i) is not None:
            name = s.labels.get(d)
        elif i.memory_segment == Register.DS and i.segment_prefix == Register.NONE and i.memory_base != Register.BP:
            n = None if s.no_near_names else s.dsyms.get(d)
            if n and (i.memory_displ_size or not regs): name = n; s.externs[n] = 'near'; segp = ''
        if name is None:
            if not regs and not seg: segp = 'ds:'
            return pfx + segp + '[' + inner + ']'
        return pfx + segp + name + (('[' + '+'.join(regs) + ']') if regs else '')

    def dbline(s, o, i, note=''):
        raw = s.b(o, i.len); fl = s.fields(i, o)
        parts = []; k = 0
        for at, sz, txt, kind in sorted(fl):
            if k < at: parts.append('db      ' + ','.join(hb(x) for x in raw[k:at]))
            if kind == 'far': parts.append(f'dd      {txt}')
            elif kind == 'self': parts.append(f'dw      offset {txt}'); parts.append(f'dw      seg {txt}')
            elif kind == 'seg': parts.append(f'dw      {txt if txt.startswith("@") else "seg " + txt}')
            else: parts.append('db      ' + ','.join(hb(x) for x in raw[at:at + sz]))
            k = at + sz
        if k < len(raw): parts.append('db      ' + ','.join(hb(x) for x in raw[k:]))
        txt = s.fmt.format(i)
        return '\n'.join('        ' + p for p in parts[:-1]) + ('\n' if len(parts) > 1 else '') + f'        {parts[-1]:<40}; {txt} {note}'.rstrip()


OK_CPU = {CpuidFeature.INTEL8086, CpuidFeature.INTEL186, CpuidFeature.INTEL386,
          CpuidFeature.FPU, CpuidFeature.FPU287, CpuidFeature.FPU387}


def plausible(i):
    """An instruction the original tools could have written: valid, at most 386/387."""
    return i.code != 0 and i.mnemonic != Mnemonic.SALC and all(f in OK_CPU for f in i.cpuid_features())


def hx(v):
    if v < 10: return str(v)
    h = f'{v:X}h'
    return '0' + h if h[0] in 'ABCDEF' else h


def hb(x):
    h = f'{x:X}h'
    return '0' + h if h[0] in 'ABCDEF' else h


# ---------- --fix: assemble, compare, rewrite
def check(g, text):
    from omf import module_masked
    d = tempfile.mkdtemp(prefix='asmgen-'); src = os.path.join(d, 'DRAFT.ASM')
    open(src, 'w').write(text)
    out = os.path.join(d, 'out')
    subprocess.run(['node', os.path.join(here, 'tcc.mjs'), out, '/ml', src], capture_output=True)
    log = open(os.path.join(out, 'BUILD.LOG'), encoding='latin1').read()
    errs = [(int(m.group(1)), m.group(2)) for m in re.finditer(r'\*\*Error\*\* \S+\((\d+)\) (.*)', log)]
    obj = None if errs else open(os.path.join(out, 'DRAFT.OBJ'), 'rb').read()
    shutil.rmtree(d, ignore_errors=True)
    if errs: return errs, None
    _, segs, data, mask, _ = module_masked(obj)
    ci = next(k for k, (sn, cn, ln) in enumerate(segs, 1) if cn.upper().endswith('CODE'))
    return [], (bytes(data[ci]), bytes(mask[ci]))


def compare(g, obj, mask):
    """Item by item: the segment offsets whose assembly differs, and how."""
    bad = {}; p = 0
    for kind, o, n, _ in g.items:
        if p >= len(obj): bad[o] = 'short-obj'; break
        want = g.b(o, n)
        if kind in ('data', 'multi'):
            got = obj[p:p + n]
            if any(not mask[p + k] and got[k] != want[k] for k in range(min(n, len(got)))): bad[o] = 'data'
            p += n; continue
        i = next(iter(Decoder(16, obj[p:], ip=0)))
        L = i.len
        ori = g.ins.get(o)
        isjump = ori is not None and g.branch_target(ori) is not None and ori.mnemonic != Mnemonic.CALL
        iscall = ori is not None and g.branch_target(ori) is not None and ori.mnemonic == Mnemonic.CALL
        if iscall and n == L and obj[p] == want[0]:
            pass        # a near call: its displacement settles once every length agrees
        elif isjump:
            while p + L < len(obj) and obj[p + L] == 0x90 and L < n: L += 1
            if L != n or obj[p] != want[0]: bad[o] = 'longer' if L > n else 'shorter'
        else:
            # merged push cs; call: TASM writes it as one item
            if n > L and obj[p] == 0x0E:
                j = next(iter(Decoder(16, obj[p + 1:], ip=0))); L = 1 + j.len
                if n > L and obj[p + L] == 0x90: L += 1
            got = obj[p:p + L]
            if L != n or any(not mask[p + k] and got[k] != want[k] for k in range(L)): bad[o] = 'bytes'
        p += L
    return bad


def main():
    a = sys.argv[1:]
    g = Gen(a[0], keep_db='--keep-db' in a, no_near_names='--no-near-names' in a)
    text = g.run()
    if '--fix' in a:
        for rnd in range(8):
            errs, res = check(g, text)
            lines = text.split('\n')
            if errs:
                for ln, msg in errs:
                    print(f'round {rnd}: error line {ln}: {msg}: {lines[ln - 1].strip()}', file=sys.stderr)
                # map the line back to an item: the nearest instruction line above it
                changed = False
                for ln, msg in errs:
                    o = line_offset(g, lines, ln)
                    if o is not None and g.fix.get(o) != 'db': g.fix[o] = 'db'; changed = True
                if not changed: break
                text = g.run(); continue
            obj, mask = res
            bad = compare(g, obj, mask)
            print(f'round {rnd}: {len(bad)} items differ', file=sys.stderr)
            if not bad: break
            for o, why in bad.items():
                cur = g.fix.get(o)
                if why == 'longer' and cur is None: g.fix[o] = 'short'
                elif why == 'shorter' and cur is None: g.fix[o] = 'nearbytes'
                elif why in ('bytes', 'data') and o in g.ins: g.fix[o] = 'db'
                else: g.fix[o] = 'db'
            text = g.run()
    sys.stdout.write(text)


def line_offset(g, lines, ln):
    """The segment offset of the item a source line (1-based) belongs to."""
    j = ln - 1 - g.hdrlines; best = None
    for kind, o, n, l0 in g.items:
        if l0 <= j: best = o
        else: break
    return best


if __name__ == '__main__':
    main()
