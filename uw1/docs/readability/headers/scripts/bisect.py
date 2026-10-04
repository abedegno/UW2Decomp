"""Put excluded names back by bisection: bisect.py PLAN BASE STAGES. Reads and rewrites
loop_excl.json; a name stays excluded only if putting it back fails the fast gate."""
import sys, os, json, tomllib
H = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, H)
import hdr
planf, base, stages = sys.argv[1], sys.argv[2], set(sys.argv[3].split(','))
plan = tomllib.load(open(planf, 'rb'))
xf = os.path.join(H, 'loop_excl.json')
excl = set(json.load(open(xf)))
def ok(ex):
    hdr.run(plan, base, stages, ex)
    return hdr.fastcheck()[0]
assert ok(excl), 'the exclude set does not pass'
cands = sorted(excl); needed = set()
def test(group, rest):
    # rest: names still excluded besides group
    if ok(rest | needed):
        print('back in:', group, flush=True); return
    if len(group) == 1:
        needed.add(group[0]); print('needed:', group[0], flush=True); return
    h = len(group) // 2
    test(group[:h], rest | set(group[h:]))
    test(group[h:], rest)
test(cands, set())
json.dump(sorted(needed), open(xf, 'w'), indent=0)
print('needed', sorted(needed))
ok(needed)
