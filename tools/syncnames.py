"""Rename target-table rows to the names a matched file actually defines.
usage: syncnames.py src/FILE.C      (after match.py has built it)
For each public function in the object, the row at the same offset takes its name (minus
the underscore). Prints each change; rows with no public at their offset (statics) are left."""
import sys, os, re
here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here); sys.path.insert(0, here)
from match import publics, load_targets
src = sys.argv[1]; stem = os.path.splitext(os.path.basename(src))[0].upper()
seg = re.search(r'/\*\s*target:\s*(\w+)\s*\*/', open(src, encoding='latin1').read()).group(1)
pubs = publics(open(os.path.join(root, 'build', stem, stem + '.OBJ'), 'rb').read())
# only publics in the code segment name functions; a data public can share an offset
from omf import module_masked
_, segs, _, _, _ = module_masked(open(os.path.join(root, 'build', stem, stem + '.OBJ'), 'rb').read())
code = next(i for i, x in enumerate(segs, 1) if x[1] == 'CODE')
at = {o: n[1:] if n.startswith('_') else n for n, (s, o) in pubs.items() if s == code}
t = os.path.join(root, 'targets', seg + '.tsv'); lines = open(t).read().split('\n')
for j, l in enumerate(lines):
    if not l or l.startswith('#'): continue
    f = l.split('\t'); off = int(f[2], 16)
    # a 32-character name is only Turbo C truncating the table's longer one
    if off in at and at[off] != f[0] and not (len(at[off]) == 32 and f[0].startswith(at[off])):
        print(f'{f[0]} -> {at[off]}'); f[0] = at[off]; lines[j] = '\t'.join(f)
open(t, 'w').write('\n'.join(lines))
