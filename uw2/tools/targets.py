"""Build a target table for one DOS segment from the map.
usage: targets.py SEGNAME > targets/SEGNAME.tsv
Base from map/segments.tsv, verified proc offsets from map/procs.tsv, and each function's
original name from map/functions.tsv where it is anchored or confirmed (otherwise the IDA
name, to be replaced once known). Sizes run to the next proc; the last runs to the segment's
final far return."""
import sys, os
root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
M = lambda p: [l.rstrip('\n').split('\t') for l in open(os.path.join(root, 'map', p)) if not l.startswith('#')]
seg = sys.argv[1]
base = next(int(r[1], 16) for r in M('segments.tsv') if r[0] == seg and r[1])
procs = [(r[1], int(r[2], 16), r[3]) for r in M('procs.tsv') if r[0] == seg]
names = {r[1]: (r[4].rstrip('_'), r[5]) for r in M('functions.tsv') if r[0] == seg}
procs.sort(key=lambda p: p[1])
exe = open(os.path.expanduser(os.environ.get('UW2_EXE', '~/UWGOG/UW2/UW2.EXE')), 'rb').read()
if any(st == 'unverified' for _, _, st in procs):
    print(f'# warning: unverified offsets in {seg}', file=sys.stderr)
# segments are byte-aligned, so a file's code starts at its first function, possibly a few
# bytes past the paragraph; offsets below are from that start, and org is the difference
org = procs[0][1]
last = procs[-1][1]
end = exe.index(b'\xcb', base + last) - base + 1
print(f'# segment {seg} base 0x{base + org:X} size 0x{end - org:X} org 0x{org:X}')
print('# cname: original FM Towns name where the map anchors or confirms it, else the IDA name')
for i, (ida, o, st) in enumerate(procs):
    nxt = procs[i + 1][1] if i + 1 < len(procs) else end
    orig, how = names.get(ida, ('', ''))
    c = orig if orig and how in ('anchor', 'confirmed') else ida
    print(f'{c}\t{ida}\t0x{o - org:X}\t0x{nxt - o:X}')
