"""The port's Milestone 1 measurement: compile every game C source for the host, compile only.

    python3 tools/portcheck.py              summary: per category, per file, unresolved names
    python3 tools/portcheck.py --diag FILE  every diagnostic for one source (path or stem)
    python3 tools/portcheck.py --list CAT   every diagnostic in one category
    python3 tools/portcheck.py --json       also write build/port/summary.json
    python3 tools/portcheck.py --cc CC      another compiler (default cc, Apple clang on macOS)

Each .C under src/ (tools/sources.py: not src/include, not src/port) is compiled with the host
C compiler as C89 with GNU extensions, with src/port/compat.h force-included and the port's
stand-ins for Borland's headers (src/port/include) ahead of the host's. No game source is
edited. Objects and the full log go to build/port/; nothing else is written.

Diagnostics are counted once each (a header's are reported where they occur, not once per
source that includes it) and put in categories by message, warning flag and the source line.
Then the objects' undefined names, less everything the objects define and everything the host
C library resolves, are the link's unresolved names: the assembly modules' entry points and
data (to be replaced by port C), the far data taken from the EXE, Borland's library (to be
provided by the port), and the port's own platform names (port_, bc_). That list is the
platform layer's and the assembly replacement's workload; docs/PORT.md has the baseline.

The DOS build is untouched: tools/tcc.mjs stages src/include only, tools/srcdeps.py hashes
src/include only, and tools/sources.py skips src/port.
"""
import os, re, sys, json, ctypes, argparse, subprocess, collections
from concurrent.futures import ThreadPoolExecutor

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
sys.path.insert(0, here)
import sources

OUT = os.path.join(root, 'build', 'port')
PORT = os.path.join(root, 'src', 'port')
FLAGS = ['-x', 'c', '-std=gnu89', '-fsigned-char', '-D_POSIX_C_SOURCE=200809L', '-ferror-limit=0', '-fno-color-diagnostics',
         '-fdiagnostics-show-option', '-fno-caret-diagnostics',
         '-include', os.path.join(PORT, 'compat.h'), '-I', os.path.join(PORT, 'include'),
         '-iquote', os.path.join(root, 'src', 'include'),
         # widths: what a 16-bit int and 32-bit long become on a 64-bit host
         '-Wpointer-to-int-cast', '-Wint-to-pointer-cast', '-Wshorten-64-to-32',
         '-Wno-unused-value', '-Wno-parentheses', '-Wno-dangling-else',
         '-Wno-logical-op-parentheses', '-Wno-bitwise-op-parentheses', '-Wno-shift-op-parentheses',
         # compat.h sets #pragma pack(1) for the game's structs on purpose
         '-Wno-pragma-pack']
DIAG = re.compile(r'^(?P<file>[^:\n]+):(?P<line>\d+):(?P<col>\d+): (?P<kind>error|warning): '
                  r'(?P<msg>.*?)(?: \[(?P<flag>-W[^\]]+)\])?$')

# Categories, tried in order; the first that matches a diagnostic takes it. Each test sees the
# message, the warning flag and the text of the source line the diagnostic points at.
CATEGORIES = [
    ('inline asm', lambda m, f, t: re.search(r'^\s*asm\b', t) or "'asm'" in m),
    ('far pointers and segments', lambda m, f, t: re.search(r'\b(MK_FP|FP_SEG|FP_OFF|movedata)\b', t)),
    ('near pointer held in an int', lambda m, f, t:
        f in ('-Wint-to-pointer-cast', '-Wpointer-to-int-cast', '-Wint-conversion',
              '-Wvoid-pointer-to-int-cast')
        or re.search(r'integer to pointer|pointer to integer|pointer from integer|integer from pointer'
                     r'|to smaller integer type|from smaller integer type', m)),
    ('64-bit long and pointer differences', lambda m, f, t:
        f == '-Wshorten-64-to-32' or re.search(r"argument of type 'long'", m)),
    ('Borland library internals', lambda m, f, t:
        re.search(r"no member named 'fd'|undeclared identifier '_(ctype|IS_\w+)'", m)),
    ('missing declaration', lambda m, f, t:
        'implicit declaration of function' in m or 'call to undeclared function' in m
        or 'incomplete type' in m),
    ('char parameters against old-style declarations', lambda m, f, t:
        'conflicting types' in m or 'incompatible function pointer' in m
        or f == '-Wincompatible-function-pointer-types'),
    ('calls without prototypes', lambda m, f, t:
        f in ('-Wimplicit-int', '-Wdeprecated-non-prototype', '-Wstrict-prototypes',
              '-Wknr-promoted-parameter')
        or re.search(r'type specifier missing|without a prototype|old-style', m)),
    ('pointer signedness and types', lambda m, f, t:
        f in ('-Wpointer-sign', '-Wincompatible-pointer-types',
              '-Wincompatible-pointer-types-discards-qualifiers')
        or 'incompatible pointer' in m),
    ('bitfields', lambda m, f, t: 'bit-field' in m or 'bitfield' in (f or '')),
    ('constants, shifts and overflow', lambda m, f, t:
        f in ('-Wconstant-conversion', '-Wshift-count-overflow', '-Wshift-count-negative',
              '-Winteger-overflow', '-Wshift-overflow', '-Wimplicit-int-conversion',
              '-Wtautological-constant-out-of-range-compare', '-Wconstant-logical-operand',
              '-Wshift-negative-value')
        or re.search(r'overflow|changes value|out of range', m)),
    ('unsigned abs and always-true tests', lambda m, f, t:
        f in ('-Wabsolute-value', '-Wtautological-pointer-compare',
              '-Wtautological-unsigned-zero-compare', '-Wtautological-compare')),
    ('empty if bodies (kept for matching)', lambda m, f, t: f == '-Wempty-body'),
    ('return and control flow', lambda m, f, t:
        f in ('-Wreturn-type', '-Wreturn-mismatch', '-Wsometimes-uninitialized', '-Wuninitialized')
        or re.search(r'should (not )?return|does not return', m)),
    ('comments', lambda m, f, t: f == '-Wcomment'),
]
OTHER = 'other'


def category(msg, flag, text):
    for name, test in CATEGORIES:
        if test(msg, flag, text): return name
    return OTHER


def dos_only(path):
    """A source whose header comment says `port: dos-only` is DOS-specific (EMS.C's int 67h
    calls); the port replaces it in src/port/ and never compiles it."""
    return 'port: dos-only' in open(path, encoding='latin1').read(4000)


def compile_one(cc, path):
    stem = sources.stem(path)
    obj = os.path.join(OUT, stem + '.o')
    if os.path.exists(obj): os.remove(obj)
    r = subprocess.run([cc] + FLAGS + ['-c', '-o', obj, path], capture_output=True, text=True,
                       cwd=root)
    return path, r.returncode, r.stderr, obj if r.returncode == 0 and os.path.exists(obj) else None


_lines = {}
def source_line(path, n):
    if path not in _lines:
        try: _lines[path] = open(path, encoding='latin1').read().split('\n')
        except OSError: _lines[path] = []
    ls = _lines[path]
    return ls[n - 1] if 0 < n <= len(ls) else ''


def rel(p):
    p = os.path.normpath(os.path.join(root, p))
    return os.path.relpath(p, root)


def nm(obj, flag):
    r = subprocess.run(['nm', flag, obj], capture_output=True, text=True)
    out = set()
    for l in r.stdout.split('\n'):
        l = l.strip()
        if not l: continue
        name = l.split()[-1]
        out.add(name[1:] if name.startswith('_') else name)
    return out


def asm_publics():
    """{C name: .ASM path} for every public of an assembly module (the C name drops TASM's _)."""
    out = {}
    for p in sources.all_sources():
        if not p.upper().endswith('.ASM'): continue
        for m in re.finditer(r'(?im)^\s*public\s+(\S+)', open(p, encoding='latin1').read()):
            for n in m.group(1).split(','):
                n = n.strip()
                if n: out[(n[1:] if n.startswith('_') else n)[:TC_NAME]] = p
    return out


def symbol_table():
    out = {}
    for l in open(os.path.join(root, 'symbols.tsv'), encoding='latin1'):
        if l.startswith('#'): continue
        f = l.rstrip('\n').split('\t')
        if len(f) >= 2: out[(f[0][1:] if f[0].startswith('_') else f[0])[:TC_NAME]] = f[1]
    return out


TC_NAME = 31

BORLAND_HEADERS = ('dos.h', 'alloc.h', 'mem.h', 'io.h', 'stat.h', 'dir.h', 'conio.h', 'bios.h')


def borland_names():
    """Names declared by the port's stand-in headers and compat.h's Borland extensions."""
    names = {}
    for h in BORLAND_HEADERS + ('../compat.h',):
        p = os.path.normpath(os.path.join(PORT, 'include', h))
        for m in re.finditer(r'\b(\w+)\s*\(|\bextern\b[^;(]*\b(\w+)\s*\[', open(p).read()):
            n = m.group(1) or m.group(2)
            if n in ('if', 'while', 'sizeof', 'return', 'defined'): continue
            names.setdefault(n, os.path.basename(h))
    return names


def libc_has(name):
    try:
        ctypes.CDLL(None)[name]; return True
    except (AttributeError, OSError):
        return False


def main(argv):
    ap = argparse.ArgumentParser(description='Compile the game sources for the host (Milestone 1).')
    ap.add_argument('--cc', default=os.environ.get('CC', 'cc'))
    ap.add_argument('--diag', metavar='FILE')
    ap.add_argument('--list', metavar='CATEGORY')
    ap.add_argument('--json', action='store_true', help='also write build/port/summary.json')
    a = ap.parse_args(argv)
    os.makedirs(OUT, exist_ok=True)
    srcs = [p for p in sources.all_sources() if p.upper().endswith('.C') and not dos_only(p)]
    if a.diag:
        want = a.diag.upper()
        srcs = [p for p in srcs if sources.stem(p) == os.path.splitext(os.path.basename(want))[0]]
        if not srcs: raise SystemExit(f'portcheck.py: no source {a.diag}')
    with ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as ex:
        results = list(ex.map(lambda p: compile_one(a.cc, p), srcs))
    if not a.diag:                     # the full run's log stays the full run's
      with open(os.path.join(OUT, 'portcheck.log'), 'w') as log:
        for path, rc, err, obj in results:
            log.write(f'=== {rel(path)} (exit {rc})\n{err}\n')

    seen = set(); diags = []
    for path, rc, err, obj in results:
        for l in err.split('\n'):
            m = DIAG.match(l)
            if not m: continue
            f = rel(m['file']); line = int(m['line'])
            key = (f, line, int(m['col']), m['kind'], m['msg'])
            if key in seen: continue
            seen.add(key)
            text = source_line(os.path.join(root, f), line)
            diags.append(dict(file=f, line=line, kind=m['kind'], msg=m['msg'], flag=m['flag'],
                              cat=category(m['msg'], m['flag'], text), text=text.strip()))

    if a.diag:
        for d in diags:
            print(f"{d['file']}:{d['line']}: {d['kind']}: [{d['cat']}] {d['msg']}"
                  + (f" [{d['flag']}]" if d['flag'] else ''))
        return 0
    if a.list:
        for d in diags:
            if d['cat'].startswith(a.list):
                print(f"{d['file']}:{d['line']}: {d['kind']}: {d['msg']}\n    {d['text']}")
        return 0

    # the summary
    ok = [p for p, rc, e, o in results if o]
    clean = [p for p, rc, e, o in results if o and not any(d['file'] == rel(p) for d in diags)]
    print(f'{len(srcs)} C sources: {len(ok)} compile, {len(srcs) - len(ok)} fail; '
          f'{len(clean)} with no diagnostic in the file itself')
    errs = [d for d in diags if d['kind'] == 'error']; warns = [d for d in diags if d['kind'] == 'warning']
    print(f'{len(errs)} errors and {len(warns)} warnings (each counted once)\n')
    bycat = collections.OrderedDict((c, [0, 0]) for c, _ in CATEGORIES)
    bycat[OTHER] = [0, 0]
    for d in diags: bycat[d['cat']][0 if d['kind'] == 'error' else 1] += 1
    print(f"{'category':44} {'errors':>7} {'warnings':>9}")
    for c, (e, w) in bycat.items():
        if e or w: print(f'{c:44} {e:7} {w:9}')
    print()
    byfile = collections.defaultdict(lambda: collections.Counter())
    for d in diags: byfile[d['file']][d['kind']] += 1
    failed = {rel(p) for p, rc, e, o in results if not o}
    print(f"{'file':32} {'errors':>7} {'warnings':>9}  top categories")
    for f in sorted(byfile, key=lambda f: (-byfile[f]['error'], -byfile[f]['warning'], f)):
        cats = collections.Counter(d['cat'] for d in diags if d['file'] == f)
        top = ', '.join(f'{c} {n}' for c, n in cats.most_common(3))
        print(f"{f:32} {byfile[f]['error']:7} {byfile[f]['warning']:9}  {top}"
              + ('  (no object)' if f in failed else ''))
    nodiag_fail = sorted(failed - set(byfile))
    for f in nodiag_fail: print(f'{f:32}  failed with no parsed diagnostic; see build/port/portcheck.log')
    print()

    # what a link would still need
    defined, undefined = set(), collections.defaultdict(set)
    for p, rc, e, o in results:
        if not o: continue
        defined |= nm(o, '-gU')
        for n in nm(o, '-u'): undefined[n].add(sources.stem(p))
    unresolved = {n: s for n, s in undefined.items() if n not in defined}
    pubs, syms, bor = asm_publics(), symbol_table(), borland_names()
    failed_text = {rel(p): open(p, encoding='latin1').read() for p, rc, e, o in results if not o}
    dosonly_text = {rel(p): open(p, encoding='latin1').read() for p in sources.all_sources()
                    if p.upper().endswith('.C') and dos_only(p)}
    def defined_in(n, texts):
        for f, text in texts.items():
            if re.search(r'(?m)^(?!\s|#|/\*|extern\b|static\b)[^;=(]*\b' + re.escape(n) + r'\s*(\(|\[|=|;|,)', text):
                return f
    def defined_in_failed(n):
        for f, text in failed_text.items():
            if re.search(r'(?m)^(?!\s|#|/\*|extern\b|static\b)[^;=(]*\b' + re.escape(n) + r'\s*(\(|\[|=|;|,)', text):
                return f
    groups = collections.defaultdict(list)
    for n in sorted(unresolved):
        t = n[:TC_NAME]                    # Turbo C and TASM keep 32 characters with the _
        if n.startswith('port_') or n.startswith('bc_'): g = 'port layer (compat.h: port_, bc_)'
        elif t in pubs: g = 'asm: ' + rel(pubs[t])
        elif n in bor: g = f'Borland library ({bor[n]})'
        elif libc_has(n): continue
        elif defined_in_failed(n): g = 'C that does not compile yet: ' + defined_in_failed(n)
        elif defined_in(n, dosonly_text): g = 'C replaced by the port (dos-only): ' + defined_in(n, dosonly_text)
        elif t in syms and syms[t].startswith('DS:'): g = 'no source: DGROUP gaps placed by tools/extract.py'
        elif t in syms: g = 'no source: far data taken from the EXE, or a name inside another array (' + syms[t].split(':')[0] + ')'
        else: g = 'unknown (not in symbols.tsv)'
        groups[g].append(n + ('' if t == n else f' (DOS: {t})'))
    total = sum(len(v) for v in groups.values())
    print(f'{total} unresolved names for a link, by where they come from:')
    order = sorted(groups, key=lambda g: (not g.startswith('asm'), g))
    for g in order:
        print(f'  {g}: {len(groups[g])}')
        print('    ' + ' '.join(groups[g]))
    clash = sorted(n for n in defined if libc_has(n))
    print(f'\n{len(clash)} names the game defines that the host C library also has '
          '(the game\'s definition wins in the link, but the port should rename them):')
    print('    ' + ' '.join(clash))
    if a.json:
        with open(os.path.join(OUT, 'summary.json'), 'w') as f:
            json.dump(dict(sources=len(srcs), compiled=len(ok), clean=len(clean),
                           categories=bycat, files={k: dict(v) for k, v in byfile.items()},
                           unresolved={g: v for g, v in groups.items()}, clashes=clash,
                           callers={n: sorted(s) for n, s in unresolved.items()}), f, indent=1)
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
