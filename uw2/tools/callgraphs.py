"""Build both call graphs: map/dos_calls.tsv (IDA listing) and map/fm_calls.tsv (FM Towns image).
Each line: caller <TAB> callee. DOS callees are IDA proc names (j_ thunks resolved to the
overlay function); FM callees are symbol names from direct E8 calls."""
import os, re
from iced_x86 import Decoder, Mnemonic, OpKind
root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
asm = os.path.expanduser(os.environ.get('UW2_ASM', '~/UWReverseEngineering/uw2_asm.asm'))
procs = {l.split('\t')[1] for l in open(os.path.join(root, 'map', 'dos_code.tsv'))}
cur = None; out = set()
for l in open(asm, encoding='latin1'):
    m = re.match(r'^(\w+)\s+proc\b', l)
    if m: cur = m.group(1); continue
    if re.match(r'^(\w+)\s+endp\b', l): cur = None; continue
    m = cur and re.match(r'^\s*call\s+(?:far ptr\s+|near ptr\s+)?(\w+)', l)
    if m:
        t = re.sub(r'^j_', '', m.group(1))
        if t in procs and t != cur: out.add((cur, t))
with open(os.path.join(root, 'map', 'dos_calls.tsv'), 'w') as f:
    for a, b in sorted(out): f.write(f'{a}\t{b}\n')
print(len(out), 'DOS call edges')

img = open(os.path.join(root, 'fmtowns', 'uw2fmt.img'), 'rb').read()
syms = sorted((int(a, 16), n) for n, a, s in (l.rstrip('\n').split('\t') for l in open(os.path.join(root, 'fmtowns', 'syms.tsv'))))
code = [(a, n) for a, n in syms if a <= 0x61dd5]
byaddr = {a: n for a, n in code}
out = set()
for i, (a, n) in enumerate(code):
    end = code[i + 1][0] if i + 1 < len(code) else a + 16
    for ins in Decoder(32, img[a:end], ip=a):
        if ins.mnemonic == Mnemonic.CALL and ins.op0_kind == OpKind.NEAR_BRANCH32:
            t = byaddr.get(ins.near_branch_target)
            if t and t != n: out.add((n, t))
with open(os.path.join(root, 'map', 'fm_calls.tsv'), 'w') as f:
    for a, b in sorted(out): f.write(f'{a}\t{b}\n')
print(len(out), 'FM Towns call edges')
