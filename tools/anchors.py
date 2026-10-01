"""Collect DOS proc <-> original name anchors and check they keep link order.
Sources: targets/*.tsv for the files in matched.txt, far functions in symbols.tsv,, map/pairs_*.tsv (callpairs.py output), and the
string anchors from UWReverseEngineering's 'UW2 FM Towns' folder. Writes map/anchors.tsv."""
import os, re, glob
root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
procs = [l.rstrip('\n').split('\t') for l in open(os.path.join(root, 'map', 'dos_code.tsv'))]
dosidx = {p[1]: i for i, p in enumerate(procs)}
fm = {}
for l in open(os.path.join(root, 'fmtowns', 'syms.tsv')):
    n, a, s = l.rstrip('\n').split('\t'); fm[n] = int(a, 16)
anchors = {}
def add(ida, name, why):
    ida = re.sub(r'^j_', '', ida)
    fmn = name if name.endswith('_') else name + '_'
    if ida not in dosidx or fmn not in fm: return
    anchors.setdefault(ida, (fmn, why))
# only files that match and verify count; other target tables take their names from the map
done = {l.strip() for l in open(os.path.join(root, 'matched.txt')) if l.strip()}
for t in glob.glob(os.path.join(root, 'targets', '*.tsv')):
    if os.path.splitext(os.path.basename(t))[0] not in done: continue
    for l in open(t):
        if not l.startswith('#'): c, ida = l.split('\t')[:2]; add(ida, c, 'matched')
for p in glob.glob(os.path.join(root, 'map', 'pairs_*.tsv')):
    for l in open(p): ida, n = l.rstrip('\n').split('\t'); add(ida, n, 'call')
# far functions in symbols.tsv with original names: a resident address is a paragraph and an
# offset, which the located segments and verified proc offsets turn straight into a DOS proc
import struct
exe = open(os.path.expanduser(os.environ.get('UW2_EXE', '~/UWGOG/UW2/UW2.EXE')), 'rb').read()
hdr = struct.unpack_from('<H', exe, 8)[0] * 16
para = {}
for l in open(os.path.join(root, 'map', 'segments.tsv')):
    if l.startswith('#'): continue
    sg, b = l.rstrip('\n').split('\t')[:2]
    if b and not sg.startswith('ovr'): para[(int(b, 16) - hdr) // 16] = sg
at = {}
for l in open(os.path.join(root, 'map', 'procs.tsv')):
    if l.startswith('#'): continue
    sg, n, o = l.rstrip('\n').split('\t')[:3]
    if o != 'None': at[(sg, int(o, 16))] = n
sym = os.path.join(root, 'symbols.tsv')
if os.path.exists(sym):
    for l in open(sym):
        if l.startswith('#'): continue
        n, v, src = l.rstrip('\n').split('\t')
        if src != 'FM Towns' or v.startswith('DS:'): continue
        pg, off = (int(x, 16) for x in v.split(':'))
        if pg in para and (para[pg], off) in at:
            add(at[(para[pg], off)], n.lstrip('_'), 'symbol')
sa = os.path.expanduser('~/UWReverseEngineering/UW2 FM Towns/dos_to_fmtowns_anchors.tsv')
for l in open(sa).readlines()[1:]:
    d, f = l.split('\t')[:2]; add(d, f, 'string')
rows = sorted(((dosidx[d], fm[f], d, f, w) for d, (f, w) in anchors.items()))
bad = [(a, b) for a, b in zip(rows, rows[1:]) if b[1] < a[1]]
with open(os.path.join(root, 'map', 'anchors.tsv'), 'w') as f:
    for r in rows: f.write(f'{r[2]}\t{r[3]}\t{r[4]}\n')
print(len(rows), 'anchors;', len(bad), 'order inversions')
for a, b in bad: print(f'  {a[2]} -> {a[3]} (FM {a[1]:#x})  then  {b[2]} -> {b[3]} (FM {b[1]:#x})')
