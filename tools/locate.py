"""Find each DOS code segment's file offset in UW2.EXE, validated against the IDA listing.
Resident segments: header + paragraph*16, the paragraph from IDA's segNNN_PPPP name.
Overlays: code blocks in link order after the 'FBOV' header, each followed by relocation
data; the next overlay starts at the next place where its first function disassembles as
IDA lists it. A segment counts as located when the first instructions of up to five of
its functions match IDA's mnemonics. Writes map/segments.tsv."""
import os, re, struct
from iced_x86 import Decoder, Formatter, FormatterSyntax
root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
asm = os.path.expanduser(os.environ.get('UW2_ASM', '~/UWReverseEngineering/uw2_asm.asm'))
exe = open(os.path.expanduser(os.environ.get('UW2_EXE', '~/UWGOG/UW2/UW2.EXE')), 'rb').read()
hdr = struct.unpack_from('<H', exe, 8)[0] * 16
cblp, cp = struct.unpack_from('<HH', exe, 2); mzend = (cp - 1) * 512 + cblp
assert exe[mzend:mzend + 4] == b'FBOV'

# IDA and iced spell some mnemonics differently
SYN = {'jz': 'je', 'jnz': 'jne', 'jnb': 'jae', 'jnc': 'jae', 'jc': 'jb', 'jnae': 'jb', 'jna': 'jbe',
       'jnbe': 'ja', 'jnl': 'jge', 'jnge': 'jl', 'jng': 'jle', 'jnle': 'jg', 'retn': 'ret', 'sal': 'shl',
       'jpe': 'jp', 'jpo': 'jnp', 'repe': 'rep', 'repz': 'rep', 'repnz': 'repne', 'xlatb': 'xlat'}
norm = lambda m: SYN.get(m, m)

# IDA: per segment, procs with offsets and their first mnemonics
segs = {}; order = []; seg = None; cur = None
for l in open(asm, encoding='latin1'):
    m = re.match(r'^(\w+)\s+segment\b', l)
    if m: seg = m.group(1); order.append(seg); segs[seg] = []; continue
    m = re.match(r'^(\w+)\s+proc\b', l)
    if m and seg:
        mo = re.search(r'_([0-9A-F]{1,5})$', m.group(1))
        cur = [m.group(1), int(mo.group(1), 16) if mo else None, []]; segs[seg].append(cur); continue
    if re.match(r'^\w+\s+endp\b', l): cur = None; continue
    m = cur and len(cur[2]) < 6 and re.match(r'^\s*([a-z]{2,7})(?:\s|$)', l)
    if m and ':' not in l.split(';')[0].split()[0] and m.group(1) not in ('assume', 'align', 'db', 'dw', 'dd', 'public', 'extrn', 'org', 'var', 'arg'):
        cur[2].append(norm(m.group(1)))


def mnems(off, n):
    out = []
    for i in Decoder(16, exe[off:off + 64]):
        out.append(norm(Formatter(FormatterSyntax.MASM).format_mnemonic(i).split()[-1]))
        if len(out) == n: break
    return out

def score(seg, base):
    """How many of up to five procs disassemble as IDA lists them at base."""
    ps = [p for p in segs[seg] if p[1] is not None and len(p[2]) >= 3][:5]
    ok = sum(1 for n, o, m in ps if mnems(base + o, len(m)) == m)
    return ok, len(ps)

rows = []
for s in order:
    if s.startswith('stub') or s.startswith('ovr') or not segs[s]: continue
    m = re.search(r'_([0-9A-F]{4})$', s)
    if m:
        base = hdr + (int(m.group(1), 16) - 0x1ED) * 16; ok, n = score(s, base)
        rows.append((s, base, ok, n))
    else:
        rows.append((s, None, 0, 0))
# resident segments IDA left unnumbered: search between the located neighbours
for k, (s, b, ok, n) in enumerate(rows):
    if b is not None: continue
    lo = next((r[1] for r in reversed(rows[:k]) if r[1] is not None), hdr)
    hi = next((r[1] for r in rows[k + 1:] if r[1] is not None), mzend)
    for cand in range(lo, hi, 16):
        ok, n = score(s, cand)
        if n and ok == n: rows[k] = (s, cand, ok, n); break
pos = mzend + 16
for s in order:
    if not s.startswith('ovr'): continue
    first = next((p for p in segs[s] if p[1] == 0 and len(p[2]) >= 3), None)
    found = None
    for cand in range(pos, min(pos + 0x2000, len(exe))):
        if first and mnems(cand, len(first[2])) == first[2]:
            ok, n = score(s, cand)
            if ok == n: found = (cand, ok, n); break
    if not found: rows.append((s, None, 0, 0)); continue
    rows.append((s, found[0], found[1], found[2]))
    last = max(p[1] for p in segs[s] if p[1] is not None)
    pos = found[0] + last + 1
# every proc's offset, checked at the located base; stale name suffixes are corrected
# by searching a few bytes either side
def near(base, o, m):
    for d in (0, -1, 1, -2, 2, -3, 3, -4, 4, -6, 6, -8, 8):
        if o + d >= 0 and mnems(base + o + d, len(m)) == m: return d
with open(os.path.join(root, 'map', 'procs.tsv'), 'w') as f:
    f.write('# segment\tproc\toffset\tstatus\n')
    fixed = unver = 0
    for s, b, ok, n in rows:
        # a segment is located if most of its checked procs agree
        if b is None or not n or ok * 5 < n * 3: continue
        for name, o, m in segs[s]:
            if o is None or len(m) < 3: f.write(f'{s}\t{name}\t{o if o is None else format(o, "X")}\tunverified\n'); unver += 1; continue
            d = near(b, o, m)
            if d is None: f.write(f'{s}\t{name}\t{o:X}\tunverified\n'); unver += 1
            else:
                f.write(f'{s}\t{name}\t{o + d:X}\t{"ok" if d == 0 else f"corrected {d:+d}"}\n'); fixed += d != 0
print(f'procs: {fixed} offsets corrected, {unver} unverified')
with open(os.path.join(root, 'map', 'segments.tsv'), 'w') as f:
    f.write('# segment\tfile offset\tprocs checked\tprocs matching IDA\n')
    for s, b, ok, n in rows: f.write(f'{s}\t{b and hex(b) or ""}\t{n}\t{ok}\n')
good = sum(1 for r in rows if r[1] and r[3] and r[2] * 5 >= r[3] * 3)
print(f'{good}/{len(rows)} segments located and validated')
for s, b, ok, n in rows:
    if not (b and n and ok * 5 >= n * 3): print('  not located:', s, b and hex(b), f'{ok}/{n}')
print('ovr154:', next((hex(r[1]) for r in rows if r[0] == 'ovr154' and r[1]), None))
