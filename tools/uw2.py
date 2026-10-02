"""The build driver behind the Makefile: the modding build, the exact link, the full gate, a boot.

    python3 tools/uw2.py game            link.py --mod; prints the EXE's path
    python3 tools/uw2.py exact           link.py, passing when only the two known bytes differ
    python3 tools/uw2.py check [--all]   the gate every change must pass (below)
    python3 tools/uw2.py boot [EXE]      boot the modding build (or EXE): title, intro and menu screenshots

The gate, `check`:
1. Every source with a `/* target: */` line is compiled, several to a DOS session and three
   sessions at once, each with its own `/* opts: */`. A source is recompiled only when its text
   (with the src/include headers it includes), its object in build/, or the toolchain differs from the last check it passed (build/check/
   state.json); --all recompiles everything. Objects land in build/STEM as match.py leaves them.
   A source that fails from a batch is built again on its own by match.py before it counts.
2. match.py (--no-build) must say WHOLE SEGMENT MATCHES and verify.py "fixups and data
   verified" for each.
3. symbols.tsv rebuilt from scratch (verify.py --update over every source in segment order,
   retrying any that fail until a round adds nothing, into an empty file in a scratch directory) must hold the same names at the same addresses as the committed
   file, in any order. A name the committed file marks 'library' by hand may come out
   'provisional', since the mark is not in any object.
4. The exact link must equal UW2.EXE except the two known bytes (0x6676C, 0x66774), and the
   modding build with no source changed must be byte-identical to it.
Exit status 0 only when everything passes.

Sources are found in every directory under src/ but src/include (tools/sources.py).
src/sys/FARDATA.ASM has no target line (link.py assembles it) and is skipped.
"""
import sys, os, re, json, glob, time, shutil, hashlib, subprocess, tempfile, io, contextlib
from concurrent.futures import ThreadPoolExecutor

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
EXE = os.path.expanduser(os.environ.get('UW2_EXE', '~/UWGOG/UW2/UW2.EXE'))
PY = os.path.join(root, '.venv', 'bin', 'python')
if not os.path.exists(PY): PY = sys.executable
DEFAULT_OPTS = '-mm -1 -G -O -Z'          # match.py's default for a C file
SESSIONS = 3                              # DOS sessions at once, as buildd.py runs
BATCH = 8                                 # sources per DOS session
KNOWN = {0x6676C: (0x00, 0x01), 0x66774: (0x00, 0x01)}   # the overlay table's code flag for seg003 and seg004
STATE = os.path.join(root, 'build', 'check', 'state.json')


def sha1(path):
    with open(path, 'rb') as f: return hashlib.sha1(f.read()).hexdigest()


def rel(p): return os.path.relpath(p, root)


sys.path.insert(0, here)
from srcdeps import source_hash     # a source's text plus the src/include headers it includes
from sources import all_sources, stem as stem_of, target as target_of     # every source under src/, by stem


def sources():
    """[(stem, path, opts)] for every source with a target line."""
    out = []
    for src in all_sources():
        head = open(src, encoding='latin1').read(3000)
        if not re.search(r'/\*\s*target:\s*\w+\s*\*/', head): continue
        opts = re.search(r'/\*\s*opts:\s*([^*]+?)\s*\*/', head)
        out.append((stem_of(src), src, opts.group(1) if opts else DEFAULT_OPTS))
    return out


def toolchain():
    h = hashlib.sha1()
    for p in (os.path.join(here, 'tcc.mjs'), os.path.join(root, 'TC', 'TCC.EXE'), os.path.join(root, 'TASM', 'TASM.EXE')):
        if os.path.exists(p): h.update(sha1(p).encode())
    return h.hexdigest()


# ---- compiling ------------------------------------------------------------------------
def build_batch(n, opts, items, tmp):
    """Compile items [(stem, src)] in one DOS session; place each object in build/STEM.
    Returns {stem: error text or None}."""
    d = os.path.join(tmp, f'batch{n:03d}')
    r = subprocess.run(['node', os.path.join(here, 'tcc.mjs'), d, opts] + [s for _, s in items],
                       capture_output=True, text=True)
    logpath = os.path.join(d, 'BUILD.LOG')
    log = open(logpath, encoding='latin1').read() if os.path.exists(logpath) else ''
    # one section per file, each starting at the compiler's or assembler's banner
    parts = re.split(r'(?m)^(?=Turbo (?:C\+\+|Assembler)\s+Version)', log)
    res = {}
    for stem, src in items:
        name = os.path.basename(src).lower()
        mine = [p for p in parts if re.search(r'(?im)^\s*(?:Assembling file:\s+)?' + re.escape(name) + r'\b', p)]
        text = mine[0] if len(mine) == 1 else ''
        obj = os.path.join(d, stem + '.OBJ')
        errs = [l for l in text.splitlines() if re.search(r'Error|Fatal', l) and not re.search(r'messages:\s+None', l)]
        if not os.path.exists(obj) or not text or errs:
            res[stem] = '\n'.join(errs) or f'no object or log from the batch (tcc.mjs: {r.stdout.strip()[-200:]})'
            continue
        out = os.path.join(root, 'build', stem); os.makedirs(out, exist_ok=True)
        # replace, not rewrite, so anything reading build/STEM never sees half a file
        open(os.path.join(d, stem + '.LOG'), 'w', encoding='latin1').write(text)
        os.replace(os.path.join(d, stem + '.LOG'), os.path.join(out, 'BUILD.LOG'))
        os.replace(obj, os.path.join(out, stem + '.OBJ'))
        res[stem] = None
    return res


def compile_all(todo):
    """Compile [(stem, src, opts)] batched by options. Returns {stem: error or None}."""
    groups = {}
    for stem, src, opts in todo: groups.setdefault(opts, []).append((stem, src))
    jobs = []
    for opts, items in sorted(groups.items()):
        for i in range(0, len(items), BATCH): jobs.append((opts, items[i:i + BATCH]))
    tmp = tempfile.mkdtemp(prefix='batch-', dir=os.path.join(root, 'build', 'check'))
    res = {}
    try:
        with ThreadPoolExecutor(SESSIONS) as pool:
            for r in pool.map(lambda a: build_batch(a[0], *a[1], tmp), enumerate(jobs)): res.update(r)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    return res


def match_verify(src, build=False):
    """(ok, text) from match.py and verify.py; build=True lets match.py compile it itself."""
    cmd = [PY, os.path.join(here, 'match.py'), src] + ([] if build else ['--no-build'])
    m = subprocess.run(cmd, capture_output=True, text=True, cwd=root)
    mt = m.stdout + m.stderr
    if m.returncode or 'WHOLE SEGMENT MATCHES' not in mt:
        bad = [l for l in mt.splitlines() if not l.rstrip().endswith('MATCH') and not l.startswith('Warning')]
        return False, 'match.py: ' + ('\n    '.join(bad[-12:]) or f'exit {m.returncode}')
    v = subprocess.run([PY, os.path.join(here, 'verify.py'), src], capture_output=True, text=True, cwd=root)
    vt = v.stdout + v.stderr
    if v.returncode or '-- fixups and data verified' not in vt:
        bad = [l for l in vt.splitlines() if 'PROBLEM' in l or l.startswith('--') or 'Error' in l]
        return False, 'verify.py: ' + ('\n    '.join(bad[-12:]) or f'exit {v.returncode}')
    return True, ''


# ---- symbols.tsv ----------------------------------------------------------------------
def rows(path):
    return {f[0]: (f[1], f[2] if len(f) > 2 else '') for f in
            (l.rstrip('\n').split('\t') for l in open(path) if l.strip() and not l.startswith('#'))}


def symbols_from_scratch(srcs):
    """verify.py --update over every source into an empty symbols.tsv in a scratch root (the
    build/, targets/, fmtowns/ there are links to the real ones); compare with the committed one."""
    sys.path.insert(0, here)
    import verify
    d = tempfile.mkdtemp(prefix='syms-', dir=os.path.join(root, 'build', 'check'))
    saved = verify.root
    try:
        for x in ('build', 'targets', 'fmtowns'):
            if os.path.exists(os.path.join(root, x)): os.symlink(os.path.join(root, x), os.path.join(d, x))
        verify.root = d
        # A file can need names another file merges first (a far address it refers to by its
        # offset half alone, data placed by its publics), so the sources go in segment order
        # and any that fail are tried again after the rest, until a round adds nothing.
        todo = sorted((s for _, s, _ in srcs), key=lambda s: (not target_of(s).startswith('ovr'), target_of(s)))
        failed = []
        while todo:
            failed = []
            for src in todo:
                with contextlib.redirect_stdout(io.StringIO()):
                    if verify.main([src, '--update']): failed.append(src)
            if len(failed) == len(todo): break
            todo = failed
        if failed: return False, 'verify --update failed from scratch for ' + ' '.join(rel(s) for s in failed)
        new, old = rows(os.path.join(d, 'symbols.tsv')), rows(os.path.join(root, 'symbols.tsv'))
    finally:
        verify.root = saved
        shutil.rmtree(d, ignore_errors=True)
    probs = [f'only in the committed file: {n} {old[n][0]}' for n in sorted(set(old) - set(new))]
    probs += [f'only in the rebuild: {n} {new[n][0]}' for n in sorted(set(new) - set(old))]
    for n in sorted(set(old) & set(new)):
        if old[n][0] != new[n][0]: probs.append(f'{n}: committed {old[n][0]}, rebuilt {new[n][0]}')
        elif old[n][1] != new[n][1] and not (old[n][1] == 'library' and new[n][1] == 'provisional'):
            probs.append(f'{n}: committed source {old[n][1]!r}, rebuilt {new[n][1]!r}')
    if probs: return False, '\n    '.join(probs[:30]) + (f'\n    ... {len(probs) - 30} more' if len(probs) > 30 else '')
    return True, f'{len(new)} names'


# ---- linking --------------------------------------------------------------------------
def link(mod):
    """Run link.py (--mod or exact) from a clean output; returns (EXE path or None, output)."""
    out = os.path.join(root, 'build', 'MODLINK' if mod else 'LINK', 'out', 'UW2.EXE')
    if os.path.exists(out): os.remove(out)     # never judge a stale EXE
    r = subprocess.run([sys.executable, os.path.join(here, 'link.py')] + (['--mod'] if mod else []),
                       capture_output=True, text=True, cwd=root)
    text = r.stdout + r.stderr
    if mod and r.returncode: return None, text
    return (out if os.path.exists(out) else None), text


def exact_diff(path):
    """None when path is UW2.EXE but for the known bytes, else what differs."""
    a = open(EXE, 'rb').read(); b = open(path, 'rb').read()
    if len(a) != len(b): return f'size {len(b):#x}, UW2.EXE {len(a):#x}'
    diff = [i for i in range(len(a)) if a[i] != b[i]] if a != b else []
    extra = [i for i in diff if KNOWN.get(i) != (a[i], b[i])]
    if extra: return f'{len(extra)} bytes differ beyond the known two, first at {extra[0]:#x}'
    if len(diff) != len(KNOWN): return f'expected the 2 known bytes to differ, {len(diff)} do (has TLINK or the EXE changed?)'
    return None


def cmd_exact():
    exe, text = link(False)
    print(text.rstrip())
    if not exe: print('FAIL: the exact link produced no EXE'); return 1
    d = exact_diff(exe)
    print(f'{rel(exe)}: ' + ('identical to UW2.EXE except the two known bytes (0x6676C, 0x66774)' if not d else 'FAIL: ' + d))
    return 1 if d else 0


def cmd_game():
    exe, text = link(True)
    print(text.rstrip())
    if not exe: print('FAIL: the modding build failed'); return 1
    print(f'modding build: {exe}')
    return 0


def cmd_boot(a):
    exe = a[0] if a else os.path.join(root, 'build', 'MODLINK', 'out', 'UW2.EXE')
    if not os.path.exists(exe): print(f'{rel(exe)} does not exist: run make game first'); return 1
    d = os.path.join(root, 'build', 'boot'); os.makedirs(d, exist_ok=True)
    for p in glob.glob(os.path.join(d, '*.png')): os.remove(p)
    r = subprocess.run(['node', os.path.join(here, 'rungame.mjs'), exe, os.path.join(d, 'shot-'),
                        'w:5000', 's:title', 'w:15000', 's:intro', 'k:Escape', 'w:3000', 'k:Escape', 'w:3000', 's:menu'],
                       cwd=root)
    shots = sorted(glob.glob(os.path.join(d, '*.png')))
    for s in shots: print('screenshot:', s)
    return r.returncode or (0 if shots else 1)


# ---- the gate -------------------------------------------------------------------------
def cmd_check(force):
    t0 = time.time()
    os.makedirs(os.path.dirname(STATE), exist_ok=True)
    try: state = json.load(open(STATE))
    except (OSError, ValueError): state = {}
    tc = toolchain()
    if state.get('toolchain') != tc: state = {'toolchain': tc, 'sources': {}}
    known = state.setdefault('sources', {})
    srcs = sources()
    results = {}    # step -> (ok, detail)
    fails = []

    # 1. compile what changed
    todo = []
    for stem, src, opts in srcs:
        obj = os.path.join(root, 'build', stem, stem + '.OBJ')
        k = known.get(stem)
        if force or not k or k['src'] != source_hash(src) or k.get('opts') != opts or not os.path.exists(obj) or k['obj'] != sha1(obj):
            todo.append((stem, src, opts))
    t1 = time.time()
    built = compile_all(todo) if todo else {}
    print(f'compiled {len(todo)} of {len(srcs)} sources in {time.time() - t1:.0f}s'
          + (' (the rest are unchanged since they last passed)' if len(todo) < len(srcs) else ''))

    # 2. match and verify every source; a batch failure gets one build of its own
    def one(item):
        stem, src, opts = item
        if stem in built and built[stem]:
            ok, why = match_verify(src, build=True)
            return stem, ok, why if ok else f'{why}\n    (batch build: {built[stem]})'
        ok, why = match_verify(src)
        if not ok and stem in built:          # a fresh batch object that fails: rebuild it alone once
            ok, why = match_verify(src, build=True)
        return stem, ok, why
    with ThreadPoolExecutor(SESSIONS) as pool:
        per = list(pool.map(one, srcs))
    for (stem, ok, why), (_, src, opts) in zip(per, srcs):
        if ok:
            known[stem] = {'src': source_hash(src), 'opts': opts, 'obj': sha1(os.path.join(root, 'build', stem, stem + '.OBJ'))}
        else:
            known.pop(stem, None); fails.append(f'{rel(src)}: {why}')
    nok = sum(1 for _, ok, _ in per if ok)
    results['match + verify'] = (nok == len(srcs), f'{nok}/{len(srcs)} sources')
    json.dump(state, open(STATE + '.tmp', 'w'), indent=1); os.replace(STATE + '.tmp', STATE)

    # 3. symbols.tsv
    if nok == len(srcs):
        ok, why = symbols_from_scratch(srcs)
        results['symbols.tsv from scratch'] = (ok, why)
        if not ok: fails.append('symbols.tsv: ' + why)
    else:
        results['symbols.tsv from scratch'] = (False, 'skipped: not every source verifies')

    # 4. links
    exe, text = link(False)
    d = exact_diff(exe) if exe else 'no EXE: ' + '\n    '.join(text.strip().splitlines()[-10:])
    results['exact link'] = (not d, d or 'UW2.EXE except 0x6676C, 0x66774')
    if d: fails.append('exact link: ' + d)
    if not d:
        mexe, mtext = link(True)
        if not mexe: md = 'failed: ' + '\n    '.join(mtext.strip().splitlines()[-10:])
        elif open(mexe, 'rb').read() != open(exe, 'rb').read(): md = f'{rel(mexe)} differs from {rel(exe)}'
        else: md = None
        results['modding build = exact'] = (not md, md or 'byte-identical')
        if md: fails.append('modding build: ' + md)
    else:
        results['modding build = exact'] = (False, 'skipped: the exact link failed')

    print()
    for f in fails: print('FAIL', f)
    if fails: print()
    for k, (ok, why) in results.items(): print(f'{"pass" if ok else "FAIL"}  {k:26} {why}')
    passed = all(ok for ok, _ in results.values())
    print(f'\n{"CHECK PASSED" if passed else "CHECK FAILED"} in {time.time() - t0:.0f}s')
    return 0 if passed else 1


def main():
    a = sys.argv[1:]
    if not a or a[0] in ('-h', '--help'): print(__doc__); return 0
    c, rest = a[0], a[1:]
    if not os.path.exists(EXE): print(f'UW2.EXE not found at {EXE}: set UW2_EXE'); return 1
    if c == 'check': return cmd_check('--all' in rest)
    if c == 'exact': return cmd_exact()
    if c == 'game': return cmd_game()
    if c == 'boot': return cmd_boot(rest)
    print(__doc__); return 2


if __name__ == '__main__':
    sys.exit(main())
