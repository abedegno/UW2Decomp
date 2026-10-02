"""Compiling many sources in headless DOS at once: several to a DOS session, several sessions
at a time. Used by uw2.py (the gate) and link.py --mod.

    from dosbatch import compile_many, backend, sessions
    compile_many([(stem, src, opts), ...], dest)   # dest(stem) -> the directory for STEM.OBJ

The DOS is the one tools/dosbackend.mjs picks (UW2_DOS: emu2, dosbox-x, staging, jsdos, or
auto for the first installed of emu2, dosbox-x, js-dos). backend() asks it once and puts the
answer in UW2_DOS, so every tcc.mjs and dosrun.mjs this process starts uses the same one.
sessions() is how many DOS sessions run at once: 3 for js-dos (each is a headless Chrome), one
per core up to 12 for the native ones (more gains nothing measurable on 14 cores);
UW2_DOS_SESSIONS overrides it. BATCH sources go to one session.
"""
import os, re, shutil, subprocess, tempfile
from concurrent.futures import ThreadPoolExecutor

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
BATCH = 8
_backend = None


def backend():
    global _backend
    if _backend is None:
        r = subprocess.run(['node', os.path.join(here, 'dosbackend.mjs')], capture_output=True, text=True)
        if r.returncode: raise SystemExit('tools/dosbackend.mjs: ' + (r.stderr.strip() or f'exit {r.returncode}'))
        _backend = r.stdout.strip()
        os.environ['UW2_DOS'] = _backend
    return _backend


def sessions():
    if os.environ.get('UW2_DOS_SESSIONS'): return max(1, int(os.environ['UW2_DOS_SESSIONS']))
    return 3 if backend() == 'jsdos' else max(1, min(os.cpu_count() or 4, 12))


def build_batch(n, opts, items, tmp, dest):
    """Compile items [(stem, src)] in one DOS session; move each object and its part of the
    log to dest(stem) as STEM.OBJ and BUILD.LOG. Returns {stem: error text or None}."""
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
        out = dest(stem); os.makedirs(out, exist_ok=True)
        # replace, not rewrite, so anything reading the destination never sees half a file
        open(os.path.join(d, stem + '.LOG'), 'w', encoding='latin1').write(text)
        os.replace(os.path.join(d, stem + '.LOG'), os.path.join(out, 'BUILD.LOG'))
        os.replace(obj, os.path.join(out, stem + '.OBJ'))
        res[stem] = None
    return res


def compile_many(todo, dest, batch=BATCH):
    """Compile [(stem, src, opts)] batched by options, batch to a session, sessions() at once.
    Returns {stem: error or None}."""
    groups = {}
    for stem, src, opts in todo: groups.setdefault(opts, []).append((stem, src))
    jobs = []
    for opts, items in sorted(groups.items()):
        for i in range(0, len(items), batch): jobs.append((opts, items[i:i + batch]))
    os.makedirs(os.path.join(root, 'build'), exist_ok=True)
    tmp = tempfile.mkdtemp(prefix='batch-', dir=os.path.join(root, 'build'))
    res = {}
    try:
        with ThreadPoolExecutor(sessions()) as pool:
            for r in pool.map(lambda a: build_batch(a[0], *a[1], tmp, dest), enumerate(jobs)): res.update(r)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    return res
