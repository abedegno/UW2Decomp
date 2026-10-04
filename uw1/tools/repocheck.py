"""Checks that need neither the toolchain nor the program, for hosted CI.

    python3 tools/repocheck.py [--root DIR] [--allow REGEX]... [--ban EXT]...

The repository checked is DIR (default: the git work tree of the current directory). Hosted
CI cannot run the gate (tools/gate.py), because the compiler and the program must never be on
GitHub, so this is what CI runs; tools/templates/repocheck.yml is a workflow for it. The gate
itself runs locally, from the pre-push hook (tools/install-hooks.sh).

Tracked means committed or not ignored, so a new file is checked before it is added.

- Every tracked .py compiles, every .mjs and .js passes `node --check` (when node is on
  PATH) and every .sh passes `sh -n`.
- Every relative link in a tracked Markdown file names a file or directory in the repository
  (links inside code spans and code blocks are ignored; web links are not fetched).
- Nothing tracked is a binary of the kind a decompilation must never publish: no DOS
  executable, object, library, overlay or disk image by name (BANNED below, plus --ban), and
  no file starting with an MZ header or an OMF record, whatever its name. Game data formats
  differ by game: add a project's own extensions with --ban (UW2: ARK, GR, BYT ...), or list
  them in exhume.toml as [repocheck] ban = ["ARK", "GR"] and allow = ["^docs/img/"].
  --allow exempts paths matching a regular expression from the binary checks.
Exit status 0 when all pass.
"""
import os, re, subprocess, sys, shutil

BANNED = ['EXE', 'COM', 'OBJ', 'LIB', 'OVL', 'OVR', 'IMG', 'IMA', 'ISO', 'ZIP', 'DSK', 'BIN', 'EXP', 'SYS', 'DLL']


def tracked(root):
    r = subprocess.run(['git', 'ls-files', '-z', '--cached', '--others', '--exclude-standard'],
                       cwd=root, capture_output=True, text=True, check=True)
    return [p for p in r.stdout.split('\0') if p and os.path.isfile(os.path.join(root, p))]


def check_code(root, files, problems):
    node = shutil.which('node')
    for p in files:
        full = os.path.join(root, p)
        if p.endswith('.py'):
            try: compile(open(full, 'rb').read(), p, 'exec')
            except SyntaxError as e: problems.append(f'{p}: line {e.lineno}: {e.msg}')
        elif p.endswith(('.mjs', '.js')) and node:
            r = subprocess.run([node, '--check', full], capture_output=True, text=True)
            if r.returncode: problems.append(f'{p}: node --check: {r.stderr.strip().splitlines()[-1:]}')
        elif p.endswith('.sh'):
            r = subprocess.run(['sh', '-n', full], capture_output=True, text=True)
            if r.returncode: problems.append(f'{p}: sh -n: {r.stderr.strip()}')


def check_links(root, files, problems):
    for p in files:
        if not p.lower().endswith('.md'): continue
        text = open(os.path.join(root, p), encoding='utf-8', errors='replace').read()
        text = re.sub(r'(?ms)^\s*```.*?^\s*```', '', text)   # code blocks
        text = re.sub(r'`[^`\n]*`', '', text)                 # code spans
        for target in re.findall(r'\]\(([^)\s]+)(?:\s+"[^"]*")?\)', text):
            if re.match(r'[a-z][a-z0-9+.-]*:', target, re.I) or target.startswith('#'): continue
            path = target.split('#')[0]
            if not os.path.exists(os.path.normpath(os.path.join(root, os.path.dirname(p), path))):
                problems.append(f'{p}: link to {target}, which does not exist')


def check_payload(root, files, banned, allow, problems):
    rx = re.compile(r'\.(' + '|'.join(re.escape(e) for e in banned) + r')$', re.I)
    for p in files:
        if any(re.search(a, p) for a in allow): continue
        if rx.search(p):
            problems.append(f'{p}: a binary file type this repository must not hold'); continue
        head = open(os.path.join(root, p), 'rb').read(4)
        if head[:2] == b'MZ' or head[:1] in (b'\x80', b'\x82'):     # MZ header, or an OMF THEADR/LHEADR
            problems.append(f'{p}: looks like a DOS executable or object file')


def project_lists(root):
    """[repocheck] ban and allow from an exhume.toml at the root, if there is one."""
    p = os.path.join(root, 'exhume.toml')
    if not os.path.exists(p): return [], []
    import tomllib
    r = tomllib.load(open(p, 'rb')).get('repocheck', {})
    return r.get('ban', []), r.get('allow', [])


def main(argv):
    a = list(argv)
    def take(flag):
        out = []
        while flag in a:
            i = a.index(flag); out.append(a[i + 1]); del a[i:i + 2]
        return out
    roots = take('--root'); allow = take('--allow'); ban = take('--ban')
    if a: print(__doc__); return 2
    root = roots[0] if roots else subprocess.run(['git', 'rev-parse', '--show-toplevel'], capture_output=True,
                                                 text=True, check=True).stdout.strip()
    pban, pallow = project_lists(root)
    problems = []
    files = tracked(root)
    check_code(root, files, problems)
    check_links(root, files, problems)
    check_payload(root, files, BANNED + ban + pban, allow + pallow, problems)
    for x in problems: print('PROBLEM', x)
    print(f'{len(files)} tracked files: ' + ('all checks pass' if not problems else f'{len(problems)} problems'))
    return 1 if problems else 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
