"""Check what match.py masks: every fixup, and the file's initialised data.
usage: verify.py src/FILE.C [--update]      (run match.py first; this reads its build)

- An extern must resolve to one address everywhere it is used: a DS offset for near data,
  segment:offset for far calls. Two externs resolving to one address are reported too. Each
  segment word and each offset word of a far address is checked, not just one of each.
- The file's own publics are placed too (_DATA and _BSS from their bases, resident code at
  its paragraph, overlay code at its stub entry) and checked against symbols.tsv like the
  externs, so one name cannot mean two addresses in two files. A public of an overlay that
  the EXE's stub table has no entry for was static in the original.
- A reference into this file's own code must land where the object file says: calls to
  functions later in the file (rewritten by the linker to nop; push cs; call near) and jump
  tables. A function's address stored as data points at its overlay stub entry instead,
  which the linker assigns; those must be one per function.
- References into this file's _DATA must agree on one base, and the _DATA bytes must equal
  the EXE's data segment at that base. References into _BSS must agree on one base.
Names are always checked against symbols.tsv (one name per address, one address per
name); --update also merges them in."""
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
    syms = {}; problems = []; data_bases = set(); cs_values = set(); internal = 0; entries = {}; bss_bases = set(); halves = {}; datafx = []; segonly = {}

    def note(name, val, where):
        prev = syms.setdefault(name, (val, where))
        if prev[0] != val:
            problems.append(f'{name}: {fmt(prev[0])} at +{prev[1]:X} but {fmt(val)} at +{where:X}')

    for f in o['fixups']:
        if f['seg'] == datas:
            datafx.append(f); continue      # checked once the data base is known
        if f['seg'] != code:
            problems.append(f'fixup in segment {segs[f["seg"]][0]} not checked'); continue
        at = f['off']; obj = o['data'][code]; loc = f['loc']; tm, ti = f['target']
        add = word(obj, at) + f['disp']
        if tm == 2:                                     # extern
            name = o['ext'][ti]
            if loc in (1, 5) and f['frame'] and f['frame'][0] == 5:
                # the offset half of a far address used as data (say, a callback argument)
                halves.setdefault(name, {}).setdefault('off', []).append(((word(exe, base + at) - add) & 0xFFFF, at))
            elif loc in (1, 5):
                note(name, ('DS', (word(exe, base + at) - add) & 0xFFFF), at)
            elif loc == 2:
                halves.setdefault(name, {}).setdefault('seg', []).append((para_of(word(exe, base + at)), at))
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
            elif loc == 5 and base < mzend:
                # a function's address taken in resident code: its real offset
                if word(exe, base + at) != add + org:
                    problems.append(f'+{at:X}: function address {word(exe, base + at):X}, object says {add + org:X}')
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

    for name, h in halves.items():
        # every half must agree: one name used for two variables shows up here
        for half in ('seg', 'off'):
            vals = sorted({v for v, _ in h.get(half, [])})
            if len(vals) > 1:
                problems.append(f'{name}: {half} halves differ: ' + ', '.join(
                    f'{v:04X} at +' + '/+'.join(f'{w:X}' for u, w in h[half] if u == v) for v in vals))
        if 'off' in h and 'seg' in h: note(name, ('FAR', h['seg'][0][0], h['off'][0][0]), h['off'][0][1])
        elif 'seg' in h:
            # only the segment is used, as in FP_SEG(x) + 1: check it against the map
            segonly[name] = h['seg'][0][0]
        else: problems.append(f'{name}: only the offset half of its far address is referenced')
    # _BSS has no bytes to compare; every reference must agree on one base
    if len(bss_bases) > 1:
        problems.append('_BSS references disagree on the base: ' + ', '.join(f'DS:{b:X}' for b in sorted(bss_bases)))
    elif bss_bases:
        print(f'_BSS: {segs[bss][2]} bytes at DS:{min(bss_bases):X}')
    if len(cs_values) > 1: problems.append(f'own segment referenced as {sorted(cs_values)}')
    data = bytes(o['data'][datas])
    if len(data_bases) > 1:
        problems.append('_DATA references disagree on the base: ' + ', '.join(f'DS:{b:X}' for b in sorted(data_bases)))
    db = None
    if len(data_bases) == 1:
        db = min(data_bases)
    elif datafx and not data_bases:
        problems.append('_DATA holds fixups but nothing in the code locates it')
    if db is not None:
        theirs = exe[DS_FILE + db:DS_FILE + db + len(data)]
        # fixups inside the data (pointer tables): check each, then leave its bytes out
        masked = set()
        para = (base - org - hdr) // 16
        for f in datafx:
            at = f['off']; loc = f['loc']; tm, ti = f['target']; add = word(data, at) + f['disp']
            here = DS_FILE + db + at
            masked.update(range(at, at + {0: 1, 1: 2, 2: 2, 3: 4, 4: 1, 5: 2}[loc]))
            if tm == 0 and ti == code and loc == 3:
                internal += 1
                if base < mzend and (word(exe, here) != add + org or word(exe, here + 2) != para):
                    problems.append(f'_DATA+{at:X}: pointer to {word(exe, here + 2):04X}:{word(exe, here):04X}, expected {para:04X}:{add + org:04X}')
                elif base >= mzend:
                    entries.setdefault(add, set()).add(word(exe, here))
            elif tm == 0 and ti == bss and loc in (1, 5):
                # a pointer into this file's own _BSS, as in `p = &static_var`
                internal += 1
                if len(bss_bases) != 1:
                    problems.append(f'_DATA+{at:X}: points into _BSS, whose base is not known')
                elif word(exe, here) != (min(bss_bases) + add) & 0xFFFF:
                    problems.append(f'_DATA+{at:X}: points to DS:{word(exe, here):X}, expected DS:{(min(bss_bases) + add) & 0xFFFF:X}')
            elif tm == 0 and ti == datas and loc in (1, 5):
                internal += 1
                if word(exe, here) != (db + add) & 0xFFFF:
                    problems.append(f'_DATA+{at:X}: points to DS:{word(exe, here):X}, expected DS:{(db + add) & 0xFFFF:X}')
            elif tm == 2 and loc == 3:
                # DGROUP is resident, so segment words in data are real paragraphs
                note(o['ext'][ti], ('FAR', word(exe, here + 2), (word(exe, here) - add) & 0xFFFF), at)
            elif tm == 2 and loc in (1, 5):
                note(o['ext'][ti], ('DS', (word(exe, here) - add) & 0xFFFF), at)
            else:
                problems.append(f'_DATA+{at:X}: fixup to {target_name(o, f["target"])} loc {loc} not handled')
        bad = [k for k in range(len(data)) if k not in masked and (k >= len(theirs) or data[k] != theirs[k])]
        if bad:
            problems.append(f'_DATA at DS:{db:X} differs at {len(bad)} bytes:\n    obj {data.hex(" ")}\n    exe {theirs.hex(" ")}')
        else:
            print(f'_DATA: {len(data)} bytes match at DS:{db:X}' + (f' ({len(datafx)} pointers checked)' if datafx else ''))

    for fn, e in sorted(entries.items()):
        name = next((r[0] for r in rows if r[2] == fn), f'+{fn:X}')
        if len(e) > 1: problems.append(f'{name} has several overlay entries: {sorted(e)}')
        else: print(f'overlay entry: {name} at stub +{min(e):X}')
    flat = [min(e) for e in entries.values()]
    if len(set(flat)) < len(flat): problems.append('two functions share an overlay entry')
    byval = {}
    for n, (v, _) in syms.items(): byval.setdefault(v, []).append(n)
    for v, ns in byval.items():
        if len(ns) > 1: problems.append(f'{", ".join(ns)} all resolve to {fmt(v)}')

    if segonly:
        path = os.path.join(root, 'symbols.tsv'); known = {}
        if os.path.exists(path):
            for l in open(path):
                if not l.startswith('#') and l.strip(): n, v = l.split('\t')[:2]; known[n] = v
        for name, para in segonly.items():
            v = known.get(name) or (fmt(syms[name][0]) if name in syms else None)
            if v and ':' in v and not v.startswith('DS:') and int(v.split(':')[0], 16) != para:
                problems.append(f'{name}: segment {para:04X} referenced, but its address is {v}')
            else:
                print(f'{name}: segment {para:04X} referenced on its own' + (f', agreeing with {v}' if v else ''))
    # the file's own publics, where the EXE has them
    pubs = {}
    para = (base - org - hdr) // 16
    stub = None
    if base >= mzend and re.fullmatch(r'ovr\d{3}', seg):
        # the overlay's stub: segment table entry N for ovrN, entries of INT 3Fh, offset, 0 after 0x20
        n = int(seg[3:]); sp, smax = struct.unpack_from('<HH', exe, segtab + 8 * n)
        lo = hdr + sp * 16
        stub = (sp, {word(exe, lo + e + 2): e for e in range(0x20, smax, 5) if exe[lo + e:lo + e + 2] == b'\xcd\x3f'})
    for name, (si, off) in o['pubs'].items():
        if si == datas and db is not None: pubs[name] = ('DS', (db + off) & 0xFFFF)
        elif si == bss and len(bss_bases) == 1: pubs[name] = ('DS', (min(bss_bases) + off) & 0xFFFF)
        elif si == code and base < mzend: pubs[name] = ('FAR', para, org + off)
        elif si == code and stub:
            if off in stub[1]: pubs[name] = ('FAR', stub[0], stub[1][off])
            else: problems.append(f'{name} is public, but the overlay stub has no entry for +{off:X}: UW2 had it static')
    print(f'{len(o["fixups"])} fixups: {len(syms)} externs resolved, {internal} internal references, {len(pubs)} publics placed')
    # without --update, still check the names against symbols.tsv, read-only. The publics
    # go through a global so that update keeps the signature other tools replace it with.
    global PUBS
    PUBS = pubs
    update(syms, problems, write='--update' in a)
    for p in problems: print('PROBLEM', p)
    print('-- fixups and data verified' if not problems else f'-- {len(problems)} problems')
    return 1 if problems else 0

def fmt(v):
    return f'DS:{v[1]:04X}' if v[0] == 'DS' else f'{v[1]:04X}:{v[2]:04X}'

PUBS = {}       # the verified file's publics, set by main for update

def update(syms, problems, write=True):
    path = os.path.join(root, 'symbols.tsv'); known = {}; label = {}
    if os.path.exists(path):
        for l in open(path):
            if l.startswith('#') or not l.strip(): continue
            f = l.rstrip('\n').split('\t'); known[f[0]] = f[1]; label[f[0]] = f[2] if len(f) > 2 else ''
    for n, (v, _) in syms.items():
        if n in known and known[n] != fmt(v):
            problems.append(f'{n}: symbols.tsv has {known[n]}, this file gives {fmt(v)}')
        known[n] = fmt(v)
    # this file's publics: the same rules, and merged too
    for n, v in PUBS.items():
        if n in known and known[n] != fmt(v):
            problems.append(f'{n}: symbols.tsv has {known[n]}, but this file defines it at {fmt(v)}')
        if n not in syms: syms = dict(syms); syms[n] = (v, None)
        known[n] = fmt(v)
    # one address, one name, across every file merged so far
    byaddr = {}
    for n, v in known.items(): byaddr.setdefault(v, []).append(n)
    for n, (v, _) in syms.items():
        others = [m for m in byaddr[fmt(v)] if m != n]
        if others: problems.append(f'{n} is at {fmt(v)}, which symbols.tsv already calls {", ".join(others)}')
    # names found in the FM Towns symbol table are the originals; the rest are provisional
    fm = os.path.join(root, 'fmtowns', 'syms.tsv'); orig = set()
    if os.path.exists(fm):
        for l in open(fm):
            n = l.split('\t')[0]; orig.add(n); orig.add('_' + n.rstrip('_'))
    if not write: return
    if problems:
        print('symbols.tsv not updated: fix the problems first'); return
    with open(path, 'w') as f:
        f.write('# name\taddress\tname source\n# address: DS:offset for near data, segment:offset for far code (load-relative paragraphs)\n')
        for n in sorted(known, key=str.lower):
            # Turbo C keeps 32 characters of an identifier
            # (a C library name marked 'library' by hand keeps that mark)
            src = 'FM Towns' if n in orig or len(n) >= 32 and any(o.startswith(n) for o in orig) else ('library' if n[:2] in ('F_', 'N_') or n.endswith('@') or label.get(n) == 'library' else 'provisional')
            f.write(f'{n}\t{known[n]}\t{src}\n')

if __name__ == '__main__':
    sys.exit(main())
