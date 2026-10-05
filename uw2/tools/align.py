"""Align every DOS function with the FM Towns build, to recover original names and files.
usage: align.py [--holdout WHY]    (drop anchors of that kind, then score on them)

Both builds were linked from the same object list, so the game's functions appear in the
same order in both. Anchors (map/anchors.tsv) that keep that order form a chain; between
consecutive anchors the DOS and FM Towns functions are aligned by size with a dynamic
programme that allows functions present in only one build. The call graphs then check each
pairing: a DOS function's callers and callees should pair with the FM Towns function's.
Well-confirmed pairings become anchors and the alignment is repeated until it settles.
Writes map/functions.tsv."""
import os, sys, math
root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
M = lambda *p: os.path.join(root, 'map', *p)
dos = [(s, n, int(o, 16), max(int(z, 16), 1)) for s, n, o, z in
       (l.rstrip('\n').split('\t') for l in open(M('dos_code.tsv')))]
dosidx = {d[1]: i for i, d in enumerate(dos)}
CODE_END = 0x61dd5            # last FM Towns code symbol; data symbols lie above
fmsyms = sorted((int(a, 16), n) for n, a, s in (l.rstrip('\n').split('\t') for l in open(os.path.join(root, 'fmtowns', 'syms.tsv'))))
code = [(a, n) for a, n in fmsyms if a <= CODE_END]
fm = [(n, a, (code[i + 1][0] if i + 1 < len(code) else a + 16) - a) for i, (a, n) in enumerate(code)]
fmidx = {f[0]: i for i, f in enumerate(fm)}
LIB = {'seg000', 'seg005_105F'}      # startup and the C library

def graph(path, idx):
    out = {}; inn = {}
    for l in open(path):
        a, b = l.rstrip('\n').split('\t')
        if a in idx and b in idx:
            out.setdefault(idx[a], set()).add(idx[b]); inn.setdefault(idx[b], set()).add(idx[a])
    return out, inn
dcall, dcaller = graph(M('dos_calls.tsv'), dosidx)
fcall, fcaller = graph(M('fm_calls.tsv'), fmidx)

GAP = -1.2
def score(dz, fz):
    return 2.0 - 2.5 * abs(math.log(max(fz, 1) / dz))

def align(d0, d1, f0, f1):
    """Align dos[d0:d1] with fm[f0:f1]; return list of (dos index, fm index)."""
    n, m = d1 - d0, f1 - f0
    if n <= 0 or m <= 0: return []
    S = [[0.0] * (m + 1) for _ in range(n + 1)]; B = [[0] * (m + 1) for _ in range(n + 1)]
    for i in range(1, n + 1): S[i][0] = S[i - 1][0] + (0 if dos[d0 + i - 1][0] in LIB else GAP); B[i][0] = 1
    for j in range(1, m + 1): S[0][j] = j * GAP; B[0][j] = 2
    for i in range(1, n + 1):
        dz = dos[d0 + i - 1][3]; lib = dos[d0 + i - 1][0] in LIB
        for j in range(1, m + 1):
            c = (S[i - 1][j - 1] + (-99 if lib else score(dz, fm[f0 + j - 1][2])), 0)
            c = max(c, (S[i - 1][j] + (0 if lib else GAP), 1), (S[i][j - 1] + GAP, 2))
            S[i][j], B[i][j] = c
    out = []; i, j = n, m
    while i > 0 and j > 0:
        if B[i][j] == 0: out.append((d0 + i - 1, f0 + j - 1)); i -= 1; j -= 1
        elif B[i][j] == 1: i -= 1
        else: j -= 1
    return out[::-1]

def chain_of(anchors):
    """Longest chain of anchors increasing in both builds."""
    a = sorted(set(anchors)); best = [1] * len(a); prev = [-1] * len(a)
    for i in range(len(a)):
        for j in range(i):
            if a[j][1] < a[i][1] and best[j] + 1 > best[i]: best[i], prev[i] = best[j] + 1, j
    if not a: return []
    i = max(range(len(a)), key=lambda k: best[k]); ch = []
    while i >= 0: ch.append(a[i]); i = prev[i]
    return ch[::-1]

def check(pairs):
    """Per pairing: (agreeing, checkable) over callees and callers."""
    res = {}
    for d, f in pairs.items():
        agree = total = 0
        for dn, fn in ((dcall, fcall), (dcaller, fcaller)):
            for c in dn.get(d, ()):
                if c in pairs:
                    total += 1; agree += pairs[c] in fn.get(f, ())
        res[d] = (agree, total)
    return res

def run(anchors):
    chain = chain_of(anchors)
    pairs = {d: f for d, f in chain}
    ends = [(-1, -1)] + chain + [(len(dos), len(fm))]
    for (da, fa), (db, fb) in zip(ends, ends[1:]):
        for d, f in align(da + 1, db, fa + 1, fb): pairs[d] = f
    return chain, pairs

def confirmed(c):
    agree, total = c
    return agree >= 2 and agree * 2 > total

args = sys.argv[1:]
hold = args[args.index('--holdout') + 1] if '--holdout' in args else None
seed = []; held = []
for l in open(M('anchors.tsv')):
    d, f, why = l.rstrip('\n').split('\t')
    if dos[dosidx[d]][0] in LIB: continue
    (held if why == hold else seed).append((dosidx[d], fmidx[f]))

anchors = list(seed)
for rnd in range(10):
    chain, pairs = run(anchors)
    c = check(pairs)
    new = sorted(set(anchors) | {(d, f) for d, f in pairs.items() if confirmed(c[d])})
    if new == sorted(set(anchors)): break
    anchors = new
c = check(pairs)
how = {}
for d, f in pairs.items():
    a, t = c[d]
    how[d] = 'confirmed' if confirmed(c[d]) else ('size, calls disagree' if t and a * 2 <= t else 'size only')
for d, f in seed:
    if pairs.get(d) == f: how[d] = 'anchor'

if hold:
    right = sum(1 for d, f in held if pairs.get(d) == f)
    print(f'{rnd + 1} rounds; held-out {hold}: {right}/{len(held)} recovered')
    for d, f in held:
        got = pairs.get(d)
        if got != f: print(f'  {dos[d][1]}: want {fm[f][0]}, got {fm[got][0] if got is not None else "-"} ({how.get(d, "")})')
    from collections import Counter
    wrong = Counter(how.get(d, '-') for d, f in held if pairs.get(d) not in (None, f))
    ok = Counter(how.get(d, '-') for d, f in held if pairs.get(d) == f)
    print('  right by kind:', dict(ok), ' wrong by kind:', dict(wrong))
    sys.exit()

with open(M('functions.tsv'), 'w') as out:
    out.write('# segment\tida_name\toffset\tsize\toriginal_name\thow\tcalls_agree\tcalls_checkable\n')
    for i, (s, n, o, z) in enumerate(dos):
        f = pairs.get(i)
        h = 'library' if s in LIB else how.get(i, '')
        a, t = c.get(i, (0, 0))
        out.write(f'{s}\t{n}\t{o:X}\t{z:X}\t{fm[f][0] if f is not None else ""}\t{h}\t{a}\t{t}\n')
from collections import Counter
print(f'{rnd + 1} rounds, {len(chain)} chained anchors')
print(Counter(how.get(i, 'library' if dos[i][0] in LIB else 'unpaired') for i in range(len(dos))))
