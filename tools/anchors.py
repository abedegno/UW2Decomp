"""Collect DOS proc <-> original name anchors and check they keep link order.
Sources: targets/*.tsv (matched files), map/pairs_*.tsv (callpairs.py output), and the
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
for t in glob.glob(os.path.join(root, 'targets', '*.tsv')):
    for l in open(t):
        if not l.startswith('#'): c, ida = l.split('\t')[:2]; add(ida, c, 'matched')
for p in glob.glob(os.path.join(root, 'map', 'pairs_*.tsv')):
    for l in open(p): ida, n = l.rstrip('\n').split('\t'); add(ida, n, 'call')
sa = os.path.expanduser('~/UWReverseEngineering/UW2 FM Towns/dos_to_fmtowns_anchors.tsv')
for l in open(sa).readlines()[1:]:
    d, f = l.split('\t')[:2]; add(d, f, 'string')
rows = sorted(((dosidx[d], fm[f], d, f, w) for d, (f, w) in anchors.items()))
bad = [(a, b) for a, b in zip(rows, rows[1:]) if b[1] < a[1]]
with open(os.path.join(root, 'map', 'anchors.tsv'), 'w') as f:
    for r in rows: f.write(f'{r[2]}\t{r[3]}\t{r[4]}\n')
print(len(rows), 'anchors;', len(bad), 'order inversions')
for a, b in bad: print(f'  {a[2]} -> {a[3]} (FM {a[1]:#x})  then  {b[2]} -> {b[3]} (FM {b[1]:#x})')
