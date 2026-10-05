"""The name crosswalk: every function of the target tables, by the disassembly listing's name
and address, with the name the matched source gives it and where the source defines it.

    python3 tools/crosswalk.py [--root DIR] [--out map/crosswalk.tsv]
    python3 tools/crosswalk.py --find NAME_OR_ADDRESS ...     look up in map/crosswalk.tsv

Other projects that work from the same listing (UnderworldGodot cites IDA names such as
`Bouncing_seg031_2CFA_EE2` and addresses such as `seg031_2CFA_EE2`) can look a routine up here
and read the matched C or assembly for it.

Columns: the listing's name; its address as the listing writes it (segment, underscore,
offset in hex); the size in bytes; the matched source's name; the source file and the
line that defines it (empty where no source defines it by that name, such as a routine
inside another's code). Reads targets/*.tsv and the sources' `target:` lines. The matched
name is the public the built object (build/STEM/STEM.OBJ, from `make check`) has at the
row's offset, through Exhume's omf.py ($EXHUME, else a checkout named Exhume beside this
one); without a build, or for a static function, it is the table's name."""
import os, re, sys, glob

HERE = os.path.dirname(os.path.abspath(__file__))
EXHUME = os.environ.get('EXHUME') or os.path.join(os.path.dirname(os.path.dirname(HERE)), 'Exhume')
try:
    sys.path.insert(0, os.path.join(EXHUME, 'tools'))
    import omf
except ImportError:
    omf = None


def object_names(root, rel, rows):
    """offset -> public name in the source's built object, in the segment whose publics
    fall on the table's rows; {} without a build."""
    if not omf or not rel: return {}
    stem = os.path.splitext(os.path.basename(rel))[0].upper()
    p = os.path.join(root, 'build', stem, stem + '.OBJ')
    if not os.path.exists(p): return {}
    pubs = omf.publics(open(p, 'rb').read())
    offs = {r for r in rows}
    best = {}
    for name, (si, off) in pubs.items():
        best.setdefault(si, {})[off] = name[1:] if name.startswith('_') else name
    seg = max(best, key=lambda si: len(offs & set(best[si])), default=None)
    return best.get(seg, {}) if seg is not None else {}


def args():
    a = sys.argv[1:]
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    out = None; find = []
    while a:
        k = a.pop(0)
        if k == '--root': root = a.pop(0)
        elif k == '--out': out = a.pop(0)
        elif k == '--find': find += a; a = []
        else: sys.exit(__doc__)
    return root, out or os.path.join(root, 'map', 'crosswalk.tsv'), find


def lookup(path, keys):
    """Each key, a listing name or an address (seg019_C20, ovr119_3CE, or a name ending in
    one, as Bouncing_seg031_2CFA_EE2 does): the row whose name it is, else the row whose
    function holds that address."""
    rows = [l.rstrip('\n').split('\t') for l in open(path) if not l.startswith('#')]
    byname = {r[0]: r for r in rows}
    spans = {}
    for r in rows:
        seg, off = r[1].rsplit('_', 1)
        spans.setdefault(seg.lower(), []).append((int(off, 16), int(r[2], 16), r))
        # the listing may name a segment with its paragraph (seg032_2DCA) where a reference
        # gives only the segment (seg032_6A9)
        short = re.match(r'^((?:seg|ovr)\d{3})_[0-9A-Fa-f]{4}$', seg)
        if short: spans.setdefault(short.group(1).lower(), []).append((int(off, 16), int(r[2], 16), r))
    for k in keys:
        r = byname.get(k)
        if not r:
            m = re.search(r'((?:seg|ovr)\d{3}(?:_[0-9A-Fa-f]{4})?)_([0-9A-Fa-f]+)$', k)
            if m:
                at = int(m.group(2), 16)
                for start, size, row in spans.get(m.group(1).lower(), []):
                    if start <= at < start + max(size, 1): r = row; break
        print(f'{k}: ' + (f'{r[3]} at {r[4] or "(no source line)"} (listing {r[0]}, {r[1]})' if r else 'not found'))


def sources(root):
    """target segment -> source path (relative), from the sources' `target:` lines."""
    m = {}
    for p in glob.glob(os.path.join(root, 'src', '**', '*'), recursive=True):
        if not p.upper().endswith(('.C', '.ASM')): continue
        with open(p, encoding='latin1') as f:
            for line in f.readlines()[:5]:
                t = re.search(r'target:\s*([A-Za-z0-9_]+)', line)
                if t: m[t.group(1)] = os.path.relpath(p, root); break
    return m


def definitions(root, rel):
    """name -> line number of its definition in one source."""
    found = {}
    with open(os.path.join(root, rel), encoding='latin1') as f:
        lines = f.readlines()
    asm = rel.upper().endswith('.ASM')
    for i, line in enumerate(lines, 1):
        if asm:
            m = re.match(r'^_?([A-Za-z_]\w*)\s+(?:proc|label)\b', line) or re.match(r'^_?([A-Za-z_]\w*):', line)
            if m: found.setdefault(m.group(1), i)
            continue
        if not line or line[0] in ' \t#/*{}': continue
        if line.rstrip().endswith(';'): continue
        m = re.search(r'\b([A-Za-z_]\w*)\s*\(', line)
        if m and m.group(1) not in ('if', 'while', 'for', 'switch', 'return', 'sizeof'):
            found.setdefault(m.group(1), i)
    return found


def main():
    root, out, find = args()
    if find: return lookup(out, find)
    srcs = sources(root)
    defs = {}
    rows = []
    for t in sorted(glob.glob(os.path.join(root, 'targets', '*.tsv'))):
        table = os.path.basename(t)[:-4]
        with open(t) as f:
            head = f.readline()
            h = re.match(r'#\s*segment\s+(\S+)\s+base\s+\S+\s+size\s+\S+(?:\s+org\s+(\S+))?', head)
            if not h: continue
            org = int(h.group(2) or '0', 16)
            # the listing's segment: a table cut out of a segment at its org is named
            # <segment>_<org> (seg019_C20); its listing segment is the name without that
            seg = h.group(1)
            q = re.match(r'^(.*)_([0-9A-Fa-f]+)$', seg)
            if q and int(q.group(2), 16) == org and org != 0: seg = q.group(1)
            rel = srcs.get(table, '')
            if rel and rel not in defs: defs[rel] = definitions(root, rel)
            body = [l.rstrip('\n').split('\t') for l in f if not l.startswith('#') and l.strip()]
            body = [c for c in body if len(c) >= 4]
            objn = object_names(root, rel, [int(c[2], 16) for c in body])
            for c in body:
                cname, ida, off, size = c[0], c[1], int(c[2], 16), int(c[3], 16)
                cname = objn.get(off, cname)
                addr = f'{seg}_{org + off:X}'
                where = ''
                if rel:
                    n = defs[rel].get(cname)
                    if not n and len(cname) >= 31:
                        # Turbo C keeps 32 characters of a public name: the source's full name
                        full = [k for k in defs[rel] if k.startswith(cname)]
                        if len(full) == 1: cname, n = full[0], defs[rel][full[0]]
                    if n: where = f'{rel}:{n}'
                rows.append((ida, addr, size, cname, where))
    os.makedirs(os.path.dirname(out), exist_ok=True)
    with open(out, 'w') as f:
        f.write('# listing name\tlisting address\tsize\tmatched name\tdefined at (file:line)\n')
        for r in rows:
            f.write(f'{r[0]}\t{r[1]}\t{r[2]:#x}\t{r[3]}\t{r[4]}\n')
    found = sum(1 for r in rows if r[4])
    print(f'{len(rows)} functions, {found} with their definition located; written to {os.path.relpath(out, root)}')


if __name__ == '__main__':
    main()
