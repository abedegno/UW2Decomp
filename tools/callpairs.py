"""Pair the original names of a matched file's far callees with IDA's DOS names.
usage: callpairs.py src/FILE.C   (after match.py). Prints  ida_name <TAB> original_name.
Both lists are in address order: the object file's far-call fixups to externs, and the
IDA listing's non-near call instructions in the same segment."""
import sys, os, re
here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
sys.path.insert(0, here)
from fixups import fixups
src = sys.argv[1]; stem = os.path.splitext(os.path.basename(src))[0].upper()
seg = re.search(r'/\*\s*target:\s*(\w+)\s*\*/', open(src, encoding='latin1').read()).group(1)
o = fixups(open(os.path.join(root, 'build', stem, stem + '.OBJ'), 'rb').read())
ours = [o['ext'][f['target'][1]] for f in sorted(o['fixups'], key=lambda f: f['off'])
        if f['loc'] == 3 and f['target'][0] == 2]
asm = os.path.expanduser(os.environ.get('UW2_ASM', '~/UWReverseEngineering/uw2_asm.asm'))
ida = []; on = False
for l in open(asm, encoding='latin1'):
    if re.match(rf'^{seg}\s+segment\b', l): on = True; continue
    if on and re.match(rf'^{seg}\s+ends\b', l): break
    m = on and re.match(r'^\s*call\s+(?:far ptr\s+)?(\S+)', l)
    if m and m.group(1) != 'near': ida.append(m.group(1))
if len(ours) != len(ida):
    sys.exit(f'{len(ours)} far-call fixups but {len(ida)} IDA far calls; not pairing')
pairs = {}
for a, b in zip(ida, ours):
    b = b[1:] if b.startswith('_') else b
    if pairs.setdefault(a, b) != b: sys.exit(f'{a} pairs with both {pairs[a]} and {b}')
back = {}
for a, b in pairs.items(): back.setdefault(b, []).append(a)
for b, a in back.items():
    if len(a) > 1: print(f'# {b} is called through {", ".join(a)}', file=sys.stderr)
for a, b in sorted(pairs.items()): print(f'{a}\t{b}')
