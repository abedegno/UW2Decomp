r"""The port's coverage over the replay sessions and the routine fuzzing (make coverage), written to
docs/COVERAGE.md (docs/BUILDING.md, "Testing").

    python3 tools/coverage.py [--no-build] [--deep]

It builds the port with clang's source-based coverage (tools/portbuild.py --coverage, in
build/port-cov), replays every session in it against its golden (tools/replay.py verify --cov;
a session that differs from DOS stops the report), runs tools/fuzzasm.py on the same objects
(--deep for its long run), merges the profiles (llvm-profdata) and has llvm-cov count, for
every source of the port, the lines, functions and regions run (xcrun finds both on macOS; on
Linux they are the versioned ones of the clang in use). docs/COVERAGE.md gets the totals, a
table per directory and per file, the functions only the fuzzing reaches, and the functions
nothing reaches: the list to record new sessions from.
"""
import os, re, sys, json, glob, shutil, subprocess, argparse, collections

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
sys.path.insert(0, here)
PY = os.path.join(root, '.venv', 'bin', 'python')
if not os.path.exists(PY): PY = sys.executable
COV = os.path.join(root, 'build', 'coverage')
DOC = os.path.join(root, 'docs', 'COVERAGE.md')


def run(cmd, **kw):
    r = subprocess.run(cmd, **kw)
    if r.returncode: sys.exit(f'coverage.py: {" ".join(cmd[:3])} ... failed ({r.returncode})')
    return r


def llvm(tool):
    """An LLVM tool of the clang the port is built with: through xcrun on macOS; elsewhere on the
    PATH, else the versioned one beside the clang (llvm-cov-18, /usr/lib/llvm-18/bin on Ubuntu)."""
    if sys.platform == 'darwin': return ['xcrun', tool]
    import portcheck
    cc = portcheck.host_cc()
    m = re.match(r'(\d+)', subprocess.run([cc, '-dumpversion'], capture_output=True, text=True).stdout.strip())
    major = m.group(1) if m else ''
    for c in ([f'{tool}-{major}', f'/usr/lib/llvm-{major}/bin/{tool}'] if major else []) + [tool]:
        if shutil.which(c): return [c]
    sys.exit(f'coverage.py: no {tool} for clang {major or "?"} (install llvm)')


def export(profdata, objects):
    cmd = llvm('llvm-cov') + ['export', '-format=text', '-skip-expansions', f'-instr-profile={profdata}', objects[0]]
    for o in objects[1:]: cmd += ['-object', o]
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode: sys.exit('coverage.py: llvm-cov export failed\n' + r.stderr[-2000:])
    return json.loads(r.stdout)['data'][0]


def area(path):
    """The directory a source is counted under: src/<subsystem> for the game's C, src/port/<dir>
    for the port's; None for what is not ours (Nuked OPL3, the SDK)."""
    rel = os.path.relpath(path, root)
    if rel.startswith('..') or rel.startswith('tools'): return None
    parts = rel.split(os.sep)
    if parts[:2] == ['src', 'port']: return '/'.join(parts[:3]) if len(parts) > 3 else 'src/port'
    return '/'.join(parts[:2])


def translated(path):
    try: return 'tools/asm2c.py' in open(path, encoding='latin1').read(600)
    except OSError: return False


def pct(c, n): return f'{100.0 * c / n:.1f}%' if n else '-'


def main(argv):
    ap = argparse.ArgumentParser(description='The port\'s coverage over the sessions and the fuzzing.')
    ap.add_argument('--no-build', action='store_true')
    ap.add_argument('--deep', action='store_true')
    a = ap.parse_args(argv)
    if not a.no_build: run([PY, os.path.join(here, 'portbuild.py'), '--coverage'], stdout=subprocess.DEVNULL)
    shutil.rmtree(COV, ignore_errors=True); os.makedirs(COV)
    env = dict(os.environ, LLVM_PROFILE_FILE=os.path.join(COV, 'replay-%p.profraw'))
    run([PY, os.path.join(here, 'replay.py'), 'verify', 'all', '--cov'], env=env)
    run([PY, os.path.join(here, 'fuzzasm.py'), '--coverage'] + (['--deep'] if a.deep else []))
    replay_raw = sorted(glob.glob(os.path.join(COV, 'replay-*.profraw')))
    fuzz_raw = sorted(glob.glob(os.path.join(COV, 'fuzz-*.profraw')))
    for name, raws in (('replay', replay_raw), ('all', replay_raw + fuzz_raw)):
        run(llvm('llvm-profdata') + ['merge', '-sparse', '-o', os.path.join(COV, name + '.profdata')] + raws)
    objs = [os.path.join(root, 'build', 'port-cov', 'uw2port'), os.path.join(root, 'build', 'fuzz-cov', 'fuzzhost')]
    rep = export(os.path.join(COV, 'replay.profdata'), objs)
    allc = export(os.path.join(COV, 'all.profdata'), objs)

    files = {}
    for f in allc['files']:
        ar = area(f['filename'])
        if not ar: continue
        s = f['summary']
        files[os.path.relpath(f['filename'], root)] = dict(area=ar, lines=(s['lines']['covered'], s['lines']['count']),
                                                           funcs=(s['functions']['covered'], s['functions']['count']),
                                                           regions=(s['regions']['covered'], s['regions']['count']))
    rlines = {os.path.relpath(f['filename'], root): f['summary']['lines']['covered'] for f in rep['files']}
    for p, d in files.items(): d['fuzz'] = d['lines'][0] - rlines.get(p, 0)

    def funcs(data):
        out = {}
        for fn in data['functions']:
            fname = os.path.relpath(fn['filenames'][0], root)
            if fname not in files: continue
            name = fn['name'].split(':')[-1]
            out[(fname, name)] = max(out.get((fname, name), 0), fn['count'])
        return out
    fr, fa = funcs(rep), funcs(allc)
    fuzz_only = sorted(k for k, c in fa.items() if c and not fr.get(k))
    never = sorted(k for k, c in fa.items() if not c)

    def total(sel):
        t = collections.Counter()
        for p, d in files.items():
            if sel(p, d):
                for k in ('lines', 'funcs', 'regions'):
                    t[k + 'c'] += d[k][0]; t[k + 'n'] += d[k][1]
        return t
    groups = [('The game\'s C (src/*, shared with DOS)', lambda p, d: not d['area'].startswith('src/port')),
              ('The port\'s C written by hand', lambda p, d: d['area'].startswith('src/port') and not translated(p)
               and not d['area'].endswith('stubs')),
              ('The port\'s C translated from the assembly (tools/asm2c.py)', lambda p, d: translated(p)),
              ('Everything', lambda p, d: True)]
    out = ['# Coverage', '',
           'Generated by `make coverage` (tools/coverage.py); do not edit. It is the port\'s coverage, '
           'measured with clang\'s source-based coverage on the coverage build (`tools/portbuild.py --coverage`), '
           f'over the {len(replay_raw)} replay sessions (`tools/replay.py verify --cov`, each identical to its DOS golden) '
           f'and the routine fuzzing (`tools/fuzzasm.py --coverage{" --deep" if a.deep else ""}`). A line or function '
           'counts as covered when either ran it. [BUILDING.md](BUILDING.md#testing) has how the tests work; the '
           'functions nothing reaches, at the end, are where new sessions are worth recording.', '',
           '## Totals', '', '| Code | Lines | Functions | Regions |', '| --- | --- | --- | --- |']
    for name, sel in groups:
        t = total(sel)
        out.append(f"| {name} | {pct(t['linesc'], t['linesn'])} of {t['linesn']:,} | "
                   f"{t['funcsc']:,} of {t['funcsn']:,} ({pct(t['funcsc'], t['funcsn'])}) | {pct(t['regionsc'], t['regionsn'])} |")
    add = sum(d['fuzz'] for d in files.values())
    addt = sum(d['fuzz'] for p, d in files.items() if translated(p))
    out += ['', f'The fuzzing runs {add:,} lines no session does ({addt:,} of them in the translated modules) and reaches '
            f'{len(fuzz_only)} functions no session does. A translated module is one C function, so its share of lines '
            'is the measure for it, not its functions.', '',
            '## By directory', '', '| Directory | Files | Lines | Functions | Regions |', '| --- | --- | --- | --- | --- |']
    byarea = collections.defaultdict(list)
    for p, d in files.items(): byarea[d['area']].append(p)
    for ar in sorted(byarea):
        t = total(lambda p, d: d['area'] == ar)
        out.append(f"| `{ar}` | {len(byarea[ar])} | {pct(t['linesc'], t['linesn'])} | {t['funcsc']} of {t['funcsn']} | "
                   f"{pct(t['regionsc'], t['regionsn'])} |")
    out += ['', '## By file', '', 'Sorted by the share of lines run, lowest first.', '',
            '| File | Lines | Lines only the fuzzing runs | Functions | Regions |', '| --- | --- | --- | --- | --- |']
    for p, d in sorted(files.items(), key=lambda x: (x[1]['lines'][0] / x[1]['lines'][1] if x[1]['lines'][1] else 1, x[0])):
        out.append(f"| `{p}` | {pct(*d['lines'])} of {d['lines'][1]} | {d['fuzz'] or ''} | {d['funcs'][0]} of {d['funcs'][1]} | "
                   f"{pct(*d['regions'])} |")
    out += ['', '## Functions only the fuzzing reaches', '']
    byf = collections.defaultdict(list)
    for f, n in fuzz_only: byf[f].append(n)
    out += [f"- `{f}`: {', '.join(sorted(v))}" for f, v in sorted(byf.items())] or ['None.']
    out += ['', '## Functions nothing reaches', '',
            'By file. In the game\'s C these are what new sessions would test; in the port\'s, the C behind them.', '']
    byf = collections.defaultdict(list)
    for f, n in never: byf[f].append(n)
    out += [f"- `{f}` ({len(v)}): {', '.join(sorted(v))}" for f, v in sorted(byf.items())] or ['None.']
    open(DOC, 'w').write('\n'.join(out) + '\n')
    t = total(lambda p, d: True)
    print(f"coverage: {pct(t['linesc'], t['linesn'])} of lines, {t['funcsc']} of {t['funcsn']} functions; "
          f"the fuzzing adds {add} lines and {len(fuzz_only)} functions; written to {os.path.relpath(DOC, root)}")
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
