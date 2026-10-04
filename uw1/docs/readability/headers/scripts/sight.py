"""First sight of every identifier in each source, through its includes, honouring the
rename tricks (#define N UW2_N ... #undef N); and the tie groups whose order changes."""
import os, re, glob, sys
sys.path.insert(0, os.path.expanduser('~/Exhume/tools'))
import cparse
from collections import defaultdict

def _strip(path, cache):
    if path not in cache:
        cache[path] = cparse.strip_comments(open(path, encoding='latin1').read())
    return cache[path]

def sights(root):
    inc = os.path.join(root, 'include'); cache = {}
    out = {}
    for f in sorted(glob.glob(os.path.join(root, '**', '*.C'), recursive=True)):
        first = {}; seen_h = set(); ren = set(); k = [0]
        def walk(p):
            for line in _strip(p, cache).split('\n'):
                m = re.match(r'\s*#\s*include\s+"([^"]+)"', line)
                if m:
                    h = m.group(1)
                    if h not in seen_h:
                        seen_h.add(h); hp = os.path.join(inc, h)
                        if os.path.exists(hp): walk(hp)
                    continue
                m = re.match(r'\s*#\s*define\s+(\w+)', line)
                if m and m.group(1) not in first: first[m.group(1)] = k[0]; k[0] += 1
                m = re.match(r'\s*#\s*define\s+(\w+)\s+(?:UW2|uw2)_\w+', line)
                if m: ren.add(m.group(1)); continue
                m = re.match(r'\s*#\s*undef\s+(\w+)', line)
                if m: ren.discard(m.group(1)); continue
                if re.match(r'\s*#', line): continue
                line = re.sub(r'"(\\.|[^"\\])*"', '""', line)
                for tok in re.findall(r'[A-Za-z_]\w*', line):
                    if tok in ren: continue
                    if tok not in first: first[tok] = k[0]
                    k[0] += 1
        walk(f)
        out[os.path.relpath(f, root)] = first
    return out

def groups(root, key):
    res = {}
    for f in sorted(glob.glob(os.path.join(root, '**', '*.C'), recursive=True)):
        ents = cparse.analyse(f)
        pubs, bss = [], []
        for e in ents:
            if e['kind'] == 'func_def' and not e['static']: pubs.append(e['name'])
            if e['kind'] in ('var_def', 'static_var'):
                for x in e['names']:
                    if e['kind'] == 'var_def': pubs.append(x['name'])
                    if not x['init']: bss.append(x['name'])
        gs = []
        for kind, g in (('publics', pubs), ('bss', bss)):
            by = defaultdict(list)
            for n in dict.fromkeys(g): by[key(n)].append(n)
            gs += [(kind, ns) for ns in by.values() if len(ns) > 1]
        res[os.path.relpath(f, root)] = gs
    return res

def changed_ties(base, new, key):
    sb, sn = sights(base), sights(new)
    out = defaultdict(set)
    for f, gs in groups(base, key).items():
        for kind, ns in gs:
            ob = sorted(ns, key=lambda n: sb.get(f, {}).get(n, 1e12))
            on = sorted(ns, key=lambda n: sn.get(f, {}).get(n, 1e12))
            if ob != on: out[f] |= set(ns)
    return out

if __name__ == '__main__':
    from declinv import order_key
    import config
    cfg = config.load(os.path.expanduser('~/UW1Decomp/exhume.toml'))
    for f, ns in sorted(changed_ties(sys.argv[1], sys.argv[2], order_key(cfg)).items()): print(f, sorted(ns))
