"""Build a target table for one DOS segment from the IDA listing.
usage: targets.py SEGNAME FILEBASE_HEX > targets/SEG.tsv
Rows: cname  idaname  offset  size. cname defaults to the IDA name; replace it with the
original (FM Towns) name when known. Sizes run to the next proc, the last to segment end."""
import sys,re,os
seg=sys.argv[1]; base=int(sys.argv[2],16)
asm=os.path.expanduser(os.environ.get('UW2_ASM','~/UWReverseEngineering/uw2_asm.asm'))
procs=[]; on=False; last=0
for line in open(asm,encoding='latin1'):
    if re.match(rf'^{seg}\s+segment\b',line): on=True; continue
    if not on: continue
    if re.match(rf'^{seg}\s+ends\b',line): break
    m=re.match(rf'^((?:\w+_)?{seg}_([0-9A-F]+))\s+proc\b',line)
    if m: procs.append((m.group(1),int(m.group(2),16)))
    m=re.match(rf'^(?:\w+_)?{seg}_([0-9A-F]+):',line)
    if m: last=int(m.group(1),16)
# the segment end is not labelled: take the last label and walk the EXE to the far return
exe=open(os.path.expanduser(os.environ.get('UW2_EXE','~/UWGOG/UW2/UW2.EXE')),'rb').read()
end=exe.index(b'\xcb',base+last)-base+1
print(f'# segment {seg} base 0x{base:X} size 0x{end:X}')
for i,(n,o) in enumerate(procs):
    nxt=procs[i+1][1] if i+1<len(procs) else end
    print(f'{n}\t{n}\t0x{o:X}\t0x{nxt-o:X}')
