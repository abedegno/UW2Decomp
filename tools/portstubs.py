"""Write the port's link stubs (docs/PORT.md, Milestone 2 step 8): one definition for every
name the game's C uses that no C source defines and the host C library does not provide.

    python3 tools/portstubs.py           write src/port/stubs/*.c from the current tree
    python3 tools/portstubs.py --check   exit 1 if the stubs on disk are out of date

The names and where they come from are tools/portcheck.py's (the assembly modules, Borland's
library, the far data taken from the EXE, the DGROUP gaps, the DOS-only C, and the port
layer's own port_ and bc_ names). Whether a name is a function or data, and its type, comes
from clang's AST of the sources that use it.

Names the port's own C under src/port (its replacements, outside src/port/stubs) already
defines get no stub, so the stub list shrinks as replacements land.

Each function becomes `void NAME(void)` that reports itself and stops (port_stub, in
src/port/stubs/stub.c): the stubs exist so that the port links, and every one of them is
work for Milestone 3 and later. Each variable becomes a zeroed byte array of the size of its
declared type (64 KB when the type is an array of unknown length). The stub files include no
game header, so a stub's own type never has to agree with the game's declaration; the real
replacements, written in later milestones, will. One file per source of the names, so that a
replacement deletes its stubs a file at a time.
"""
import os, re, sys, json, argparse, subprocess, collections
from concurrent.futures import ThreadPoolExecutor

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
sys.path.insert(0, here)
import sources, portcheck

STUBS = os.path.join(root, 'src', 'port', 'stubs')
OUT = os.path.join(root, 'build', 'port')
UNKNOWN_ARRAY = 0x10000


def port_defined():
    """Every name a C file under src/port defines, the stubs aside: the replacements."""
    import portbuild
    srcs = [p for p in portbuild.port_sources() if os.sep + 'stubs' + os.sep not in p]
    with ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as ex:
        res = list(ex.map(lambda p: portbuild.compile_port('cc', p), srcs))
    bad = [p for p, rc, e, o in res if not o]
    if bad:
        raise SystemExit('portstubs.py: these port sources do not compile: ' + ' '.join(os.path.relpath(p, root) for p in bad)
                         + '\n' + '\n'.join(e for p, rc, e, o in res if not o)[-3000:])
    out = set()
    for p, rc, e, o in res: out |= portcheck.nm(o, '-gU')
    return out


def file_of_group(g):
    """The stub file for one of portcheck's groups."""
    if g.startswith('asm: '): return 'asm_' + os.path.splitext(os.path.basename(g[5:]))[0].lower() + '.c'
    if g.startswith('Borland library'): return 'borland.c'
    if g.startswith('port layer'): return 'portlayer.c'
    if g.startswith('C replaced by the port'): return 'dosonly_' + os.path.splitext(os.path.basename(g.split(': ')[-1]))[0].lower() + '.c'
    if 'far data' in g: return 'exedata.c'
    if 'DGROUP' in g: return 'dgroup.c'
    return 'other.c'


def describe(f, g):
    if g.startswith('asm: '): return 'the assembly module ' + g[5:]
    if g.startswith('C replaced by the port'): return 'the DOS-only C file ' + g.split(': ')[-1]
    return {'exedata.c': 'the far data tools/extract.py takes from UW2.EXE (renderer tables, model '
                         'data, stdat and the names inside it)',
            'dgroup.c': 'the DGROUP gaps tools/extract.py places (data no C source defines yet)',
            'borland.c': "Borland's C library (the port's stand-in headers and compat.h)",
            'portlayer.c': "the portability layer's own names (port_ and bc_, compat.h and portable.h)"}.get(f, g)


def declarations(names):
    """{name: ('fn' or 'var', qualType)} from the ASTs of the sources."""
    want = set(names); found = {}
    flags = [f for f in portcheck.FLAGS if not f.startswith('-W') and f != '-ferror-limit=0']
    srcs = [p for p in sources.all_sources() if p.upper().endswith('.C') and not portcheck.dos_only(p)]

    def one(p):
        r = subprocess.run(['cc'] + flags + ['-w', '-fsyntax-only', '-Xclang', '-ast-dump=json', p],
                           capture_output=True, text=True, cwd=root)
        out = {}
        if r.returncode: return out
        def walk(n):
            if isinstance(n, dict):
                if n.get('kind') in ('FunctionDecl', 'VarDecl') and n.get('name') in want:
                    t = (n.get('type') or {}).get('qualType', '')
                    k = 'fn' if n['kind'] == 'FunctionDecl' else 'var'
                    if n['name'] not in out or ('[]' in out[n['name']][1] and '[]' not in t):
                        out[n['name']] = (k, t)
                for c in n.get('inner', []) or []: walk(c)
        walk(json.loads(r.stdout))
        return out
    with ThreadPoolExecutor(max_workers=min(8, os.cpu_count() or 4)) as ex:
        for d in ex.map(one, srcs):
            for k, v in d.items():
                if k not in found or ('[]' in found[k][1] and '[]' not in v[1]): found[k] = v
    return found


def sizes(decls):
    """{name: bytes} for the variables, measured on the host with the game's headers."""
    vars_ = {n: t for n, (k, t) in decls.items() if k == 'var'}
    out = {}
    probe = os.path.join(OUT, 'stubsizes.c')
    hs = [h for h in sorted(os.listdir(os.path.join(root, 'src', 'include'))) if h.endswith('.h') and h != 'portable.h']
    lines = [f'#include "{h}"' for h in hs] + ['#undef main', 'int main(void)', '{']
    for n, t in sorted(vars_.items()):
        if re.search(r'\[\]', t) or not t: continue
        lines.append(f'    printf("%s %lu\\n", "{n}", (unsigned long)sizeof({t}));')
    lines += ['    return 0;', '}']
    os.makedirs(OUT, exist_ok=True)
    open(probe, 'w').write('\n'.join(lines) + '\n')
    exe = os.path.join(OUT, 'stubsizes')
    flags = [f for f in portcheck.FLAGS if not f.startswith('-W') and f != '-ferror-limit=0']
    r = subprocess.run(['cc'] + flags + ['-w', '-o', exe, probe], capture_output=True, text=True, cwd=root)
    if r.returncode: raise SystemExit('portstubs.py: the size probe does not build\n' + r.stderr[-3000:])
    for l in subprocess.run([exe], capture_output=True, text=True).stdout.split('\n'):
        if l.strip():
            n, s = l.split(); out[n] = int(s)
    for n, t in vars_.items():
        out.setdefault(n, UNKNOWN_ARRAY)
    return out


def generate():
    summary = os.path.join(OUT, 'summary.json')
    r = subprocess.run([sys.executable, os.path.join(here, 'portcheck.py'), '--json'], capture_output=True,
                       text=True, cwd=root)
    if r.returncode: raise SystemExit('portstubs.py: portcheck failed\n' + r.stdout[-2000:] + r.stderr[-2000:])
    groups = json.load(open(summary))['unresolved']
    names = {}
    for g, ns in groups.items():
        for n in ns: names[n.split(' ')[0]] = g
    # what the port's own C already defines (its replacements) needs no stub
    replaced = port_defined()
    names = {n: g for n, g in names.items() if n not in replaced}
    decls = declarations(names)
    size = sizes(decls)
    files = collections.defaultdict(list)
    for n, g in sorted(names.items()):
        k, t = decls.get(n, ('fn', ''))
        files[file_of_group(g)].append((n, g, k, t))
    out = {}
    for f, items in sorted(files.items()):
        g = describe(f, items[0][1])
        lines = [f'/* {f}: link stubs for {g}.', '   Generated by tools/portstubs.py; do not edit by hand.',
                 '   Each function reports itself and stops the game there (port_stub); each variable is zeroed storage of its',
                 '   declared size. A real replacement takes these names out of this file. */',
                 '#include "stub.h"', '']
        for n, g2, k, t in items:
            if k == 'var':
                note = f'  /* {t} */' if t else ''
                lines.append(f'unsigned char {n}[{size.get(n, UNKNOWN_ARRAY):#x}] STUB_ALIGN;{note}')
            else:
                lines.append(f'void {n}(void) {{ port_stub("{n}"); }}' + (f'  /* {t} */' if t else ''))
        out[f] = '\n'.join(lines) + '\n'
    return out


def main(argv):
    ap = argparse.ArgumentParser(description='Write the port link stubs.')
    ap.add_argument('--check', action='store_true')
    a = ap.parse_args(argv)
    out = generate()
    os.makedirs(STUBS, exist_ok=True)
    keep = {'stub.c', 'stub.h'}
    old = {f for f in os.listdir(STUBS) if f.endswith('.c') and f not in keep}
    if a.check:
        stale = [f for f in out if not os.path.exists(os.path.join(STUBS, f))
                 or open(os.path.join(STUBS, f)).read() != out[f]] + sorted(old - set(out))
        print('stubs ' + ('out of date: ' + ' '.join(stale) if stale else 'up to date'))
        return 1 if stale else 0
    for f in old - set(out): os.remove(os.path.join(STUBS, f))
    for f, text in out.items(): open(os.path.join(STUBS, f), 'w').write(text)
    n = sum(t.count('port_stub("') for t in out.values()); v = sum(t.count('STUB_ALIGN') for t in out.values())
    print(f'{len(out)} stub files in src/port/stubs: {n} functions, {v} variables')
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
