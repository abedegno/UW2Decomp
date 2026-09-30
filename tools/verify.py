"""Check what match.py masks: every fixup, and the file's initialised data.
usage: verify.py src/FILE.C [--update]      (run match.py first; this reads its build)

- An extern must resolve to one address everywhere it is used: a DS offset for near data,
  segment:offset for far calls. Two externs resolving to one address are reported too.
- A reference into this file's own code must land where the object file says: calls to
  functions later in the file (rewritten by the linker to nop; push cs; call near) and jump
  tables. A function's address stored as data points at its overlay stub entry instead,
  which the linker assigns; those must be one per function.
- References into this file's _DATA must agree on one base, and the _DATA bytes must equal
  the EXE's data segment at that base. References into _BSS must agree on one base.
--update merges the resolved externs into symbols.tsv, refusing any conflict."""
import sys, os, re, struct
here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
sys.path.insert(0, here)
from fixups import fixups, target_name
from match import load_targets, EXE

# DGROUP's file offset: "trap" is at DS:1AEB (RemoveTrap) and at file 0x6A57B.
DS_FILE = 0x68A90

def main():
    a = sys.argv[1:]; src = a[0]
    stem = os.path.splitext(os.path.basename(src))[0].upper()
    seg = re.search(r'/\*\s*target:\s*(\w+)\s*\*/', open(src, encoding='latin1').read()).group(1)
    o = fixups(open(os.path.join(root, 'build', stem, stem + '.OBJ'), 'rb').read())
    word = lambda b, i: struct.unpack_from('<H', b, i)[0]
    exe = open(EXE, 'rb').read(); hdr = struct.unpack_from('<H', exe, 8)[0] * 16
    # overlay code names other segments by their byte offset in the overlay manager's
    # segment table (8-byte entries, the first word being the paragraph); the FBOV header
    # after the load image gives the table's file offset
    cblp, cp = struct.unpack_from('<HH', exe, 2); mzend = (cp - 1) * 512 + cblp
    segtab = struct.unpack_from('<I', exe, mzend + 8)[0]
    para_of = lambda v: word(exe, segtab + v) if base >= mzend else v
    base, size, rows, org = load_targets(seg)
    segs = o['segs']
    code = next(i for i, s in enumerate(segs) if s and s[1] == 'CODE')
    datas = next(i for i, s in enumerate(segs) if s and s[0] == '_DATA')
    bss = next((i for i, s in enumerate(segs) if s and s[0] == '_BSS'), None)
    syms = {}; problems = []; data_bases = set(); cs_values = set(); internal = 0; entries = {}; bss_bases = set()

    def note(name, val, where):
        prev = syms.setdefault(name, (val, where))
        if prev[0] != val:
            problems.append(f'{name}: {fmt(prev[0])} at +{prev[1]:X} but {fmt(val)} at +{where:X}')

    for f in o['fixups']:
        if f['seg'] != code:
            problems.append(f'fixup in non-code segment {segs[f["seg"]][0]} not checked'); continue
        at = f['off']; obj = o['data'][code]; loc = f['loc']; tm, ti = f['target']
        add = word(obj, at) + f['disp']
        if tm == 2:                                     # extern
            name = o['ext'][ti]
            if loc in (1, 5):
                note(name, ('DS', (word(exe, base + at) - add) & 0xFFFF), at)
            elif loc == 3:
                note(name, ('FAR', para_of(word(exe, base + at + 2)), (word(exe, base + at) - add) & 0xFFFF), at)
            else:
                problems.append(f'+{at:X}: {name} loc {loc} not handled')
        elif tm == 0 and ti == code:                    # this file's own code
            internal += 1
            if loc == 3:
                if exe[base + at - 1:base + at + 2] == b'\x90\x0e\xe8':
                    # in an overlay the linker rewrites it to nop; push cs; call near rel16
                    dest = (at + 4 + word(exe, base + at + 2)) & 0xFFFF
                    if dest != add: problems.append(f'+{at:X}: call lands at +{dest:X}, object says +{add:X}')
                else:
                    # in resident code it stays a far call to this segment's own paragraph
                    para = (base - org - hdr) // 16
                    if word(exe, base + at) != add + org or word(exe, base + at + 2) != para:
                        problems.append(f'+{at:X}: far self-call goes to {word(exe, base + at + 2):04X}:{word(exe, base + at):04X}, '
                                        f'expected {para:04X}:{add + org:04X}')
            elif loc == 5:
                # a far function's address taken in an overlay (-Y): the linker points it
                # at the function's entry in the overlay's stub table, which it assigns
                entries.setdefault(add, set()).add(word(exe, base + at))
            elif loc == 1:
                # absolute code offsets count from the segment's paragraph, org bytes before the file
                if word(exe, base + at) != add + org:
                    problems.append(f'+{at:X}: code offset {word(exe, base + at):X}, object says {add + org:X}')
            elif loc == 2:
                cs_values.add(word(exe, base + at))
        elif tm == 0 and ti == bss:                    # this file's uninitialised data
            internal += 1
            bss_bases.add((word(exe, base + at) - add) & 0xFFFF)
        elif tm == 0 and ti == datas or tm == 1:       # this file's data, through DGROUP
            internal += 1
            data_bases.add((word(exe, base + at) - add) & 0xFFFF)
        else:
            problems.append(f'+{at:X}: target {target_name(o, f["target"])} loc {loc} not handled')

    for fn, e in sorted(entries.items()):
        name = next((r[0] for r in rows if r[2] == fn), f'+{fn:X}')
        if len(e) > 1: problems.append(f'{name} has several overlay entries: {sorted(e)}')
        else: print(f'overlay entry: {name} at stub +{min(e):X}')
    flat = [min(e) for e in entries.values()]
    if len(set(flat)) < len(flat): problems.append('two functions share an overlay entry')
    # _BSS has no bytes to compare; every reference must agree on one base
    if len(bss_bases) > 1:
        problems.append('_BSS references disagree on the base: ' + ', '.join(f'DS:{b:X}' for b in sorted(bss_bases)))
    elif bss_bases:
        print(f'_BSS: {segs[bss][2]} bytes at DS:{min(bss_bases):X}')
    if len(cs_values) > 1: problems.append(f'own segment referenced as {sorted(cs_values)}')
    data = bytes(o['data'][datas])
    if len(data_bases) > 1:
        problems.append('_DATA references disagree on the base: ' + ', '.join(f'DS:{b:X}' for b in sorted(data_bases)))
    elif data_bases:
        db = data_bases.pop(); theirs = exe[DS_FILE + db:DS_FILE + db + len(data)]
        if theirs != data:
            problems.append(f'_DATA at DS:{db:X} differs:\n    obj {data.hex(" ")}\n    exe {theirs.hex(" ")}')
        else:
            print(f'_DATA: {len(data)} bytes match at DS:{db:X}')

    byval = {}
    for n, (v, _) in syms.items(): byval.setdefault(v, []).append(n)
    for v, ns in byval.items():
        if len(ns) > 1: problems.append(f'{", ".join(ns)} all resolve to {fmt(v)}')

    print(f'{len(o["fixups"])} fixups: {len(syms)} externs resolved, {internal} internal references')
    if '--update' in a: update(syms, problems)
    for p in problems: print('PROBLEM', p)
    print('-- fixups and data verified' if not problems else f'-- {len(problems)} problems')
    return 1 if problems else 0

def fmt(v):
    return f'DS:{v[1]:04X}' if v[0] == 'DS' else f'{v[1]:04X}:{v[2]:04X}'

def update(syms, problems):
    path = os.path.join(root, 'symbols.tsv'); known = {}
    if os.path.exists(path):
        for l in open(path):
            if l.startswith('#') or not l.strip(): continue
            n, v = l.rstrip('\n').split('\t')[:2]; known[n] = v
    for n, (v, _) in syms.items():
        if n in known and known[n] != fmt(v):
            problems.append(f'{n}: symbols.tsv has {known[n]}, this file gives {fmt(v)}')
        known[n] = fmt(v)
    # names found in the FM Towns symbol table are the originals; the rest are provisional
    fm = os.path.join(root, 'fmtowns', 'syms.tsv'); orig = set()
    if os.path.exists(fm):
        for l in open(fm):
            n = l.split('\t')[0]; orig.add(n); orig.add('_' + n.rstrip('_'))
    with open(path, 'w') as f:
        f.write('# name\taddress\tname source\n# address: DS:offset for near data, segment:offset for far code (load-relative paragraphs)\n')
        for n in sorted(known, key=str.lower):
            # Turbo C keeps 32 characters of an identifier
            src = 'FM Towns' if n in orig or len(n) >= 32 and any(o.startswith(n) for o in orig) else ('library' if n[:2] in ('F_', 'N_') or n.endswith('@') else 'provisional')
            f.write(f'{n}\t{known[n]}\t{src}\n')

if __name__ == '__main__':
    sys.exit(main())
