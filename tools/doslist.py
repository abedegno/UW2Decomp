"""List every DOS code segment and its procedures from the IDA listing, in address order.
Output TSV: segment  proc  offset  size(to next proc; last one to the segment's last label)"""
import re, os, sys
asm = os.path.expanduser(os.environ.get('UW2_ASM', '~/UWReverseEngineering/uw2_asm.asm'))
seg = None; procs = []; last = 0; out = []
def flush():
    if seg and procs:
        for i, (n, o) in enumerate(procs):
            nxt = procs[i + 1][1] if i + 1 < len(procs) else max(last + 1, o + 1)
            out.append((seg, n, o, nxt - o))
for line in open(asm, encoding='latin1'):
    m = re.match(r'^(\w+)\s+segment\b', line)
    if m:
        flush(); seg = m.group(1); procs = []; last = 0; continue
    if seg is None: continue
    m = re.match(r'^(\w+)\s+proc\b', line)
    if m:
        n = m.group(1); mo = re.search(r'_([0-9A-F]{1,5})$', n)
        # names carry their offset as a hex suffix; a few don't, so fall back to the last label
        procs.append((n, int(mo.group(1), 16) if mo else last)); continue
    m = re.match(r'^\w*?_?([0-9A-F]{1,5}):', line)
    if m: last = int(m.group(1), 16)
flush()
for s, n, o, z in out: print(f'{s}\t{n}\t{o:X}\t{z:X}')
