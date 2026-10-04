"""UW1's headers from UW2's, updated in place (readability pass, step 2).

    hdr.py PLAN.toml BASE --stages drop,dups,retype,add [--loop N] [--reset] [--nocheck]

The tree under src/ is rebuilt from BASE (a copy of src taken before the step) on every run.
Stages, cumulative:
  drop    header declarations of names UW1 neither defines nor uses (and the plan's drop)
  dups    sources' declarations that repeat a header's in the same form
  retype  header declarations in their definition's form (or the plan's force); the sources'
          copies and rename tricks (#define N UW2_N) go; exclude leaves the headers and is
          declared in each file that uses it, as that file compiled it
  add     names declared only in sources go to their owner's header
With --loop, failing sources' suspect names are added to loop_excl.json and the run repeats.
"""
import sys, os, re, glob, json, shutil, subprocess, tomllib
from collections import defaultdict, Counter
H = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, H)
sys.path.insert(0, os.path.expanduser('~/Exhume/tools'))
import config, sources, cparse, headergen, sight
from declinv import n2, order_key

ROOT = os.path.expanduser('~/UW1Decomp')
cfg = config.load(os.path.join(ROOT, 'exhume.toml'))
SRC = cfg.src; INC = cfg.include
key = order_key(cfg)
TYPEDEFS = {'InputFn': 'void (far*)(NEARPTR)', 'BablFn': 'void (far*)()',
            'ArtAllocFn': 'void far*(far*)(int)', 'ArtMoveFn': 'unsigned char (far*)(void far*, int, int)',
            'WhoamiFn': 'char (far*)(struct Object far*, NEARPTR)'}


def rd(p): return open(p, encoding='latin1').read()
def wr(p, t): open(p, 'w', encoding='latin1', newline='').write(t)


def cmpnorm(s):
    s = re.sub(r'\bHOST_LAYOUT_(BEGIN|END)\b', '', s)
    for k, v in TYPEDEFS.items(): s = re.sub(r'\b%s\b' % k, v, s)
    s = re.sub(r'(struct \w+ far\*)\w+', r'\1', s)
    s = re.sub(r'NEARPTR \w+\)', 'NEARPTR)', s)
    return re.sub(r'\s+', ' ', s).strip()


def same(a, b):
    if a == b: return True
    da = re.findall(r'\[([^\]]*)\]', a); db = re.findall(r'\[([^\]]*)\]', b)
    if re.sub(r'\[[^\]]*\]', '[]', a) != re.sub(r'\[[^\]]*\]', '[]', b) or len(da) != len(db): return False
    return all(x.strip() == y.strip() or not x.strip() or not y.strip() for x, y in zip(da, db))


def real_start(raw, s, e):
    i = s
    while i < e:
        m = re.match(r'\s+|/\*.*?\*/|HOST_LAYOUT_(BEGIN|END)\b|;', raw[i:e], re.S)
        if not m or m.end() == 0: break
        i += m.end()
    return i


def sanitize(raw):
    s = re.sub(r'(?<!define )\bHOST_LAYOUT_(BEGIN|END)\b', lambda m: ';' + ' ' * (len(m.group(0)) - 1), raw)
    olds = set(re.findall(r'\b(\w+)\s+OLDSTYLE\(', s)) - {'define'}
    s = re.sub(r'(?<!define)(?<=\w)(\s+)OLDSTYLE\(', lambda m: m.group(1) + ' ' * 8 + '(', s)
    return s, olds


class Tree:
    def __init__(self):
        self.files = sorted(p for p in glob.glob(os.path.join(SRC, '**', '*.C'), recursive=True)
                            if not p.startswith(INC + os.sep))
        self.headers = sorted(glob.glob(os.path.join(INC, '*.h')))
        self.raw = {p: rd(p) for p in self.files + self.headers}
        self.oldstyle = set(); self.inv = {}
        for p in self.files + self.headers:
            s, olds = sanitize(self.raw[p]); self.oldstyle |= olds
            ents = []
            for e in cparse.analyse_text(s):
                if e.get('name') == '?': continue
                if e['kind'] in ('extern', 'var_def', 'static_var') and all(x['name'] == '?' for x in e['names']): continue
                e['text'] = self.raw[p][e['start']:e['end']]
                ents.append(e)
            self.inv[p] = ents
        self.defs = {}; self.statics = set(); self.other = set()
        self.ldecl = defaultdict(list); self.hdecl = defaultdict(list); self.ndefs = Counter()
        for f in self.files + self.headers:
            ish = f in self.headers
            for i, e in enumerate(self.inv[f]):
                k = e['kind']
                if k == 'func_def':
                    if e['static']: self.statics.add(e['name'])
                    elif not ish: self.ndefs[e['name']] += 1; self.defs.setdefault(e['name'], (f, i, None))
                elif k == 'proto':
                    if e['static']: self.statics.add(e['name'])
                    else: (self.hdecl if ish else self.ldecl)[e['name']].append((f, i, None))
                elif k in ('extern', 'var_def', 'static_var'):
                    for j, x in enumerate(e['names']):
                        nm = x['name']
                        if nm == '?': continue
                        if k == 'static_var': self.statics.add(nm)
                        elif k == 'var_def' and not ish: self.ndefs[nm] += 1; self.defs.setdefault(nm, (f, i, j))
                        elif k == 'extern': (self.hdecl if ish else self.ldecl)[nm].append((f, i, j))
                elif k in ('typedef', 'pp_define', 'struct_def', 'union_def', 'enum_def') and e.get('name'):
                    if not (k == 'pp_define' and re.match(r'\w+\s+(UW2|uw2)_', e['norm'])):
                        self.other.add(e['name'])
        self.asm = {}
        for p in sources.all_sources(cfg):
            if p.upper().endswith('.ASM'):
                for m in re.finditer(r'(?im)^\s*public\s+([^;\n]+)', rd(p)):
                    for n in m.group(1).split(','):
                        n = n.strip()
                        if n.startswith('_'): self.asm[n[1:]] = p
        self.code = {}; self.users = defaultdict(set)
        for f in self.files:
            t = cparse.strip_comments(self.raw[f]); t = re.sub(r'"(\\.|[^"\\])*"', '""', t)
            self.code[f] = t
            for n in set(re.findall(r'[A-Za-z_]\w*', t)): self.users[n].add(f)
        self.hmacro = set()
        for h in self.headers:
            for e in self.inv[h]:
                if e['kind'] == 'pp_define': self.hmacro |= set(re.findall(r'[A-Za-z_]\w*', e['norm']))
        self.tricks = defaultdict(set)
        for f in self.files:
            for m in re.finditer(r'^#define (\w+) (?:UW2|uw2)_\w+', self.raw[f], re.M): self.tricks[f].add(m.group(1))
        self.incl = {p: re.findall(r'^#include "([^"]+)"', self.raw[p], re.M) for p in self.files + self.headers}
        self.tdefs_src = {e['name'] for f in self.files for e in self.inv[f] if e['kind'] == 'typedef'}
        self.htags = {e['name'] for h in self.headers for e in self.inv[h] if e['kind'] in ('struct_def', 'union_def') and e['name']}
        self.ccnames = set()
        for h in glob.glob(os.path.join(cfg.home, '*.[Hh]')) + glob.glob(os.path.join(cfg.home, 'INCLUDE', '*.[Hh]')):
            t = rd(h)
            self.ccnames.update(re.findall(r'\b([A-Za-z_]\w*)\s*\(', t))
            self.ccnames.update(re.findall(r'\bextern\b[^;(]*?\b([A-Za-z_]\w*)\s*[;\[]', t))

    def closure(self, f):
        out = []; seen = set()
        def go(h):
            if h in seen: return
            seen.add(h); p = os.path.join(INC, h)
            if not os.path.exists(p): return
            for s in self.incl.get(p, []): go(s)
            out.append(h)
        for h in self.incl[f]: go(h)
        return out

    def text_of(self, ref):
        f, i, j = ref; e = self.inv[f][i]
        if j is None:
            t = e['text']; return headergen.clean(t[real_start(t, 0, len(t)):])
        return 'extern ' + e['names'][j]['decl']

    def norm_of(self, ref):
        f, i, j = ref; e = self.inv[f][i]
        return cmpnorm(n2(e['norm'] if j is None else e['names'][j]['decl']))

    def canon_text(self, n, force):
        if n in force: return force[n].rstrip(';').strip()
        if n in self.defs:
            f, i, j = self.defs[n]; e = self.inv[f][i]
            if j is None:
                t = e['text']; t = t[real_start(t, 0, len(t)):]
                return headergen.clean(t[:t.find('{')])
            return 'extern ' + e['names'][j]['decl']
        if self.hdecl.get(n): return self.text_of(self.hdecl[n][0])
        return Counter(self.text_of(r) for r in self.ldecl[n]).most_common(1)[0][0]

    def canon_norm(self, n, force):
        t = self.canon_text(n, force)
        e = cparse.analyse_text(sanitize(t + ';')[0])[-1]
        if e['kind'] == 'proto': return cmpnorm(n2(e['norm']))
        for x in e.get('names', []):
            if x['name'] == n: return cmpnorm(n2(x['decl']))
        return cmpnorm(t)

    def is_far_var(self, n):
        if n not in self.defs or self.defs[n][2] is None: return False
        f, i, j = self.defs[n]; d = self.inv[f][i]['names'][j]['decl']
        pre = d[:d.rfind(n)]
        return '(' not in pre and bool(re.search(r'\bfar\b', pre.rsplit('*', 1)[-1]))

    def name_of(self, ref):
        f, i, j = ref; e = self.inv[f][i]
        return e['name'] if j is None else e['names'][j]['name']


def header_for_file(plan, f):
    b = os.path.basename(f)
    for h, stems in plan.get('stems', {}).items():
        if b.upper() in [s.upper() for s in stems]: return h
    d = os.path.relpath(f, SRC).split(os.sep)[0]
    return plan['dirs'].get(d, plan.get('default_header', 'sys.h'))


def section_title(T, f):
    t = rd(f); b = os.path.basename(f)
    for m in re.finditer(r'/\*(.*?)\*/', t[:5000], re.S):
        c = ' '.join(m.group(1).split())
        if re.match(r'(target|opts):', c): continue
        head = re.split(r'(?<=[a-z0-9)])[.;](\s|$)', c)[0].strip()
        if 3 < len(head) <= 80: return f'{b}: {head[0].lower() + head[1:] if not head[:2].isupper() else head}'
        break
    return b


def reset(base):
    for d in os.listdir(SRC):
        p = os.path.join(SRC, d); shutil.rmtree(p) if os.path.isdir(p) else os.remove(p)
    for d in os.listdir(base):
        s = os.path.join(base, d)
        (shutil.copytree if os.path.isdir(s) else shutil.copyfile)(s, os.path.join(SRC, d))


def run(plan, base, stages, extra_excl=()):
    reset(base)
    T = Tree()
    force = plan.get('force', {}); excl = set(plan.get('exclude', [])) | set(extra_excl)
    keep = set(plan.get('keep', [])) | T.oldstyle; drop = set(plan.get('drop', []))
    rel = lambda p: os.path.relpath(p, SRC)
    known = lambda n: n in T.defs or n in T.asm or n in T.users or n in T.hmacro
    report = defaultdict(list); edits = defaultdict(list); suspects = defaultdict(set)
    inheader = {n: refs[0][0] for n, refs in T.hdecl.items()}
    deleted_h = set()
    # drop
    if 'drop' in stages:
        for n, refs in T.hdecl.items():
            if n in keep: continue
            if n in drop or not known(n):
                for ref in refs: edits[ref[0]].append(('del', ref))
                deleted_h.add(n); report['dropped from headers'].append(n)
    # dups
    if 'dups' in stages:
        for n, refs in T.ldecl.items():
            if n in keep or n not in T.hdecl or n in deleted_h or n in excl: continue
            hn = T.norm_of(T.hdecl[n][0])
            for ref in refs:
                if same(T.norm_of(ref), hn) and n not in T.tricks[ref[0]] \
                   and os.path.basename(T.hdecl[n][0][0]) in T.closure(ref[0]):
                    edits[ref[0]].append(('del', ref)); report['local copies removed'].append(f'{n}:{rel(ref[0])}')
    # retype
    if 'retype' in stages:
        for n in T.hdecl:
            if T.is_far_var(n) and n not in keep: excl.add(n); report['far variable, per file'].append(n)
        for n, refs in sorted(T.hdecl.items()):
            if n in keep or n in deleted_h: continue
            if n in excl:
                for ref in refs: edits[ref[0]].append(('del', ref))
                deleted_h.add(n); report['excluded'].append(n)
                old = T.text_of(refs[0])
                for f in sorted(T.users.get(n, ())):
                    if any(r[0] == f for r in T.ldecl[n]) or n in T.tricks[f]: continue
                    if n in T.defs and T.defs[n][0] == f:
                        dpos = T.inv[f][T.defs[n][1]]['start']
                        if not re.search(r'\b%s\b' % re.escape(n), T.code[f][:dpos]): continue
                    own = T.canon_text(n, force) if n in T.defs and T.defs[n][0] == f else old
                    edits[f].append(('ins', (refs[0][0], T.inv[refs[0][0]][refs[0][1]]['start'], own + ';'))); report['declared per file'].append(f'{n}:{rel(f)}')
                continue
            cn = T.canon_norm(n, force)
            if not same(T.norm_of(refs[0]), cn):
                edits[refs[0][0]].append(('repl', refs[0], T.canon_text(n, force)))
                report['retyped'].append(n)
                for f in T.users.get(n, ()):
                    if not any(r[0] == f for r in T.ldecl[n]) and n not in T.tricks[f]: suspects[f].add(n)
            for ref in refs[1:]:
                edits[ref[0]].append(('del', ref)); report['duplicate in headers'].append(n)
            for ref in T.ldecl.get(n, []):
                if ('del', ref) in edits[ref[0]]: continue
                edits[ref[0]].append(('del', ref))
                if not same(T.norm_of(ref), cn): suspects[ref[0]].add(n)
                report['local copies removed'].append(f'{n}:{rel(ref[0])}')
        for f, tr in T.tricks.items():
            for n in tr:
                if n in keep: continue
                edits[f].append(('untrick', n)); suspects[f].add(n); report['rename tricks removed'].append(f'{n}:{rel(f)}')
    # add
    adds = defaultdict(list)
    if 'add' in stages:
        for n in sorted(T.ldecl):
            if n in keep or n in excl or n in T.hdecl: continue
            why = None; t = T.canon_text(n, force)
            if n in T.statics: why = 'static'
            elif n in T.other: why = 'macro/typedef/tag'
            elif n in T.ccnames: why = 'compiler name'
            elif T.ndefs[n] > 1: why = 'defined twice'
            elif T.is_far_var(n): why = 'far variable'
            elif any(re.search(r'\b%s\b' % re.escape(td), t) for td in T.tdefs_src): why = 'file typedef'
            else:
                for m in re.finditer(r'\b(struct|union)\s+(\w+)\s*(far\s+|near\s+)?(?![\s\w]*\*)', t):
                    if m.group(2) not in T.htags: why = 'struct object type'
            if why: report['not shared: ' + why].append(n); continue
            owner = T.defs[n][0] if n in T.defs else T.asm.get(n)
            h = os.path.join(INC, header_for_file(plan, owner or T.ldecl[n][0][0]))
            adds[h].append((n, owner)); inheader[n] = h; report['added to headers'].append(n)
            cn = T.canon_norm(n, force)
            for ref in T.ldecl[n]:
                edits[ref[0]].append(('del', ref))
                if not same(T.norm_of(ref), cn): suspects[ref[0]].add(n)
    # includes
    for f in T.files:
        need = set()
        for ed in edits.get(f, []):
            if ed[0] == 'del':
                n = T.name_of(ed[1])
                if n in inheader and n not in deleted_h: need.add(os.path.basename(inheader[n]))
        for h in sorted(need - set(T.closure(f))):
            edits[f].append(('include', h)); report['includes added'].append(f'{h}:{rel(f)}')
            for e in T.inv[f]:
                if e['kind'] == 'func_def' and not e['static']: suspects[f].add(e['name'])
    apply(T, plan, edits, adds)
    if 'retype' in stages or 'add' in stages:
        dedupe_structs(report); forward_tags(report)
    for fx in plan.get('fixup', []):
        p = os.path.join(SRC, fx['file']); s = rd(p); L = s.split('\n')
        a = [q for q, l in enumerate(L) if fx['move'] in l]; b = [q for q, l in enumerate(L) if fx['before'] in l]
        if a and b and a[0] > b[0]:
            line = L.pop(a[0]); L.insert(b[0], line); wr(p, '\n'.join(L)); report['fixups'].append(fx['file'])
    for f, ns in sight.changed_ties(base, SRC, key).items(): suspects[os.path.join(SRC, f)] |= ns
    return {k: sorted(set(v)) for k, v in report.items()}, {rel(f): sorted(v) for f, v in suspects.items()}


def del_lines(raw, s, en):
    """(cut start, cut end) for removing a declaration with its line and the comment above."""
    ls = raw.rfind('\n', 0, s) + 1
    le = raw.find('\n', en); le = len(raw) if le < 0 else le
    if raw[ls:s].strip() or not re.match(r'\s*(/\*.*?\*/)?\s*$', raw[en:le]): return s, en
    cs = ls; prev = raw[:ls].split('\n')[:-1]; k = len(prev) - 1
    if k >= 0 and prev[k].rstrip().endswith('*/'):
        q = k
        while q >= 0 and '/*' not in prev[q]: q -= 1
        nxt = raw[le + 1:raw.find('\n', le + 1)] if le < len(raw) else ''
        if q >= 0 and prev[q].lstrip().startswith('/*') \
           and not re.match(r'\s*/\* [A-Za-z0-9_]+\.(C|ASM|H)\b', prev[q]) \
           and not re.match(r'\s*/\* (Defined|Declared|UW1: callees|ovr\d+:)', prev[q]) \
           and (not nxt.strip() or nxt.lstrip().startswith(('/*', '#'))):
            cs = max(0, ls - len('\n'.join(prev[q:k + 1])) - 1)
    return cs, min(le + 1, len(raw))


def apply(T, plan, edits, adds):
    for f in T.files + T.headers:
        es = edits.get(f, [])
        if not es and f not in adds: continue
        raw = T.raw[f]; spans = []; ins = []; incs = []; untricks = set()
        byent = defaultdict(list)
        for ed in es:
            if ed[0] in ('del', 'repl'): byent[ed[1][1]].append(ed)
            elif ed[0] == 'ins': ins.append(ed[1])
            elif ed[0] == 'include': incs.append(ed[1])
            elif ed[0] == 'untrick': untricks.add(ed[1])
        for i, eds in byent.items():
            e = T.inv[f][i]; s = real_start(raw, e['start'], e['end']); en = e['end']
            if e['kind'] == 'extern' and len(e['names']) > 1:
                gone = {ed[1][2] for ed in eds if ed[0] == 'del'}
                repl = {ed[1][2]: ed[2] for ed in eds if ed[0] == 'repl'}
                kept = [(j, x) for j, x in enumerate(e['names']) if j not in gone]
                if not kept: spans.append((s, en, None)); continue
                spans.append((s, en, ' '.join(repl.get(j, 'extern ' + x['decl']) + ';' for j, x in kept)))
            else:
                ed = [x for x in eds if x[0] == 'del'] or eds
                ed = ed[0]
                spans.append((s, en, None if ed[0] == 'del' else headergen.wrap(ed[2] + ';')))
        out = []; pos = 0
        for s, en, rep in sorted(spans):
            if s < pos: continue
            if rep is None:
                cs, ce = del_lines(raw, s, en)
                cs = max(cs, pos); out.append(raw[pos:cs]); pos = ce; continue
            out.append(raw[pos:s]); out.append(rep); pos = en
        out.append(raw[pos:])
        t = ''.join(out)
        for n in untricks:
            t = re.sub(r'^#define %s (?:UW2|uw2)_\w+[^\n]*\n' % re.escape(n), '', t, flags=re.M)
            t = re.sub(r'^#undef %s\b[^\n]*\n' % re.escape(n), '', t, flags=re.M)
        if untricks:
            t = re.sub(r'/\*(?:(?!\*/).)*?(renamed out of|out of the way)(?:(?!\*/).)*?\*/\n(?=#include|\n#include)', '', t, flags=re.S)
        if f in adds: t = add_to_header(T, plan, f, t, adds[f])
        if ins or incs:
            lines = t.split('\n'); last = None
            for q, l in enumerate(lines[:250]):
                if re.match(r'\s*#\s*(include|undef)\b', l): last = q
            at = (last + 1) if last is not None else 0
            block = ['#include "%s"' % h for h in sorted(incs)]
            if ins: block += ['', '/* Declared in each file that uses it, its own way (no header). */'] + [x[2] for x in sorted(set(ins))]
            lines[at:at] = block
            t = '\n'.join(lines)
        wr(f, re.sub(r'\n{3,}', '\n\n', t))


def add_to_header(T, plan, h, t, news):
    force = plan.get('force', {})
    byowner = defaultdict(list)
    for n, owner in news: byowner[owner].append(n)
    for owner, ns in sorted(byowner.items(), key=lambda kv: kv[0] or ''):
        ns = sorted(ns, key=lambda n: (T.inv[T.defs[n][0]][T.defs[n][1]]['start'] if n in T.defs else 0, n))
        decls = [headergen.wrap(T.canon_text(n, force) + ';') for n in ns]
        b = os.path.basename(owner) if owner else None
        lines = t.split('\n'); at = None
        if b:
            for q, l in enumerate(lines):
                if re.match(r'/\* %s(:| |\*)' % re.escape(b), l):
                    r = q + 1
                    while r < len(lines) and lines[r].strip() and not re.match(r'/\* [A-Za-z0-9_]+\.(C|ASM)\b', lines[r]) \
                          and not lines[r].startswith('#endif'):
                        r += 1
                    at = r; break
        if at is not None:
            lines[at:at] = decls; t = '\n'.join(lines)
        else:
            title = section_title(T, owner) if owner else 'Defined where no source has it yet: data the link takes from the program'
            m = re.search(r'^#define \w+_H_COMPLETE', t, re.M)
            k = m.start() if m else t.rfind('#endif')
            t = t[:k].rstrip('\n') + '\n\n' + '\n'.join(headergen.wrap_comment(title) + decls) + '\n\n' + t[k:]
    return t


def forward_tags(report):
    for h in sorted(glob.glob(os.path.join(INC, '*.h'))):
        t = rd(h); code = cparse.strip_comments(t)
        declared = set(re.findall(r'^\s*(?:HOST_LAYOUT_BEGIN\s+)?(?:struct|union)\s+(\w+)\s*[;{]', code, re.M))
        used = []
        for line in code.split('\n'):
            if not re.search(r'\w\s*\(', line) or line.lstrip().startswith('#'): continue
            for m in re.finditer(r'\b(struct|union)\s+(\w+)', line):
                u = (m.group(1), m.group(2))
                if u[1] not in declared and u not in used: used.append(u)
        if not used: continue
        lines = t.split('\n'); at = None
        for q, l in enumerate(lines[:80]):
            if re.match(r'(struct|union) \w+;$', l): at = q + 1
        if at is None:
            for q, l in enumerate(lines[:80]):
                if re.match(r'#include "uw2\.h"', l): at = q + 1
        add = ['%s %s;' % u for u in used]
        if re.match(r'#include', lines[at - 1]): add = [''] + add
        lines[at:at] = add
        wr(h, '\n'.join(lines))
        report['forward tags'] += [f'{u[1]}:{os.path.basename(h)}' for u in used]


def dedupe_structs(report):
    T = Tree(); hdefs = {}
    for h in T.headers:
        for e in T.inv[h]:
            if e['kind'] in ('struct_def', 'union_def') and e['name']:
                hdefs[(e['kind'], e['name'])] = (os.path.basename(h), re.sub(r'\s+', ' ', cparse.strip_comments(e['text'])).replace('HOST_LAYOUT_BEGIN', '').strip().rstrip(';').strip())
    for f in T.files:
        cl = set(T.closure(f)); raw = T.raw[f]; cut = []
        for e in T.inv[f]:
            k = (e['kind'], e.get('name'))
            if k in hdefs and hdefs[k][0] in cl:
                mine = re.sub(r'\s+', ' ', cparse.strip_comments(e['text'])).replace('HOST_LAYOUT_BEGIN', '').strip().rstrip(';').strip()
                if mine == hdefs[k][1]: cut.append(e)
        for e in sorted(cut, key=lambda e: -e['start']):
            s = real_start(raw, e['start'], e['end'])
            cs, ce = del_lines(raw, s, e['end'])
            raw = raw[:cs] + raw[ce:]
            report['local struct removed'].append(f'{e["name"]}:{os.path.relpath(f, SRC)}')
        if cut: wr(f, re.sub(r'\n{3,}', '\n\n', raw))


def fastcheck():
    r = subprocess.run(['make', 'fast'], cwd=ROOT, capture_output=True, text=True)
    return 'FAST CHECK PASSED' in r.stdout, re.findall(r'^FAIL (src/\S+?):', r.stdout, re.M), r.stdout


def fail_blocks(out):
    res = {}; cur = None
    for l in out.split('\n'):
        m = re.match(r'FAIL (src/\S+?):(.*)', l)
        if m: cur = m.group(1); res[cur] = m.group(2) + '\n'; continue
        if cur and l.startswith('    '): res[cur] += l + '\n'
        else: cur = None
    return res


if __name__ == '__main__':
    a = sys.argv[1:]
    planf, base = a[0], a[1]
    stages = set(a[a.index('--stages') + 1].split(','))
    loop = int(a[a.index('--loop') + 1]) if '--loop' in a else 1
    xf = os.path.join(H, 'loop_excl.json')
    extra = set(json.load(open(xf))) if os.path.exists(xf) and '--reset' not in a else set()
    plan = tomllib.load(open(planf, 'rb'))
    for it in range(loop):
        rep, sus = run(plan, base, stages, extra)
        json.dump({'report': rep, 'suspects': sus}, open(os.path.join(H, 'hdr_report.json'), 'w'), indent=1)
        print(' '.join(f'{k}: {len(v)};' for k, v in rep.items()), flush=True)
        if '--nocheck' in a: break
        ok, fails, out = fastcheck()
        open(os.path.join(H, 'fast_last.txt'), 'w').write(out)
        print(f'round {it}: FAST', 'PASSED' if ok else 'FAILED', len(fails), flush=True)
        if ok: break
        blocks = fail_blocks(out); add = set()
        T = Tree(); proc = set(T.hdecl) | set(T.ldecl) | set(T.defs)
        for f in fails:
            r = os.path.relpath(os.path.join(ROOT, f), SRC); txt = blocks.get(f, '')
            errn = set()
            for l in txt.split('\n'):
                if 'Error' in l and 'batch build' not in l:
                    errn |= set(re.findall(r"'(\w+)'", l)) | set(re.findall(r'call to (\w+)', l))
            errn &= set(sus.get(r, []))
            cand = (errn or set(sus.get(r, []))) - extra
            print('  ', f, sorted(cand), '' if cand else txt[:600], flush=True)
            add |= cand
        if not add: print('nothing to exclude'); break
        extra |= add
        json.dump(sorted(extra), open(xf, 'w'), indent=0)
