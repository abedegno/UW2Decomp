"""Checks that need neither the toolchain nor the game, for hosted CI (.github/workflows).

    python3 tools/repocheck.py

Tracked means committed or not ignored, so a new file is checked before it is added.

- Every tracked .py compiles, every .mjs passes `node --check` (when node is on PATH) and
  every .sh passes `sh -n`.
- Every relative link in a tracked Markdown file names a file or directory in the repository
  (links inside code spans and code blocks are ignored; web links are not fetched).
- Nothing tracked is game data or Borland software: no DOS executable, object, library or
  disk image by name, and no file starting with an MZ header or an OMF record.
Exit status 0 when all pass.
"""
import os, re, subprocess, sys, shutil

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
BANNED = re.compile(r'\.(EXE|COM|OBJ|LIB|OVL|IMG|ZIP|DAT|ARK|GR|BYT|CRT|PAL|TR|SYS|BIN|EXP)$', re.I)
problems = []


def tracked():
    r = subprocess.run(['git', 'ls-files', '-z', '--cached', '--others', '--exclude-standard'],
                       cwd=root, capture_output=True, text=True, check=True)
    return [p for p in r.stdout.split('\0') if p]


def check_code(files):
    node = shutil.which('node')
    for p in files:
        full = os.path.join(root, p)
        if p.endswith('.py'):
            try: compile(open(full, 'rb').read(), p, 'exec')
            except SyntaxError as e: problems.append(f'{p}: line {e.lineno}: {e.msg}')
        elif p.endswith('.mjs') and node:
            r = subprocess.run([node, '--check', full], capture_output=True, text=True)
            if r.returncode: problems.append(f'{p}: node --check: {r.stderr.strip().splitlines()[-1:]}')
        elif p.endswith('.sh'):
            r = subprocess.run(['sh', '-n', full], capture_output=True, text=True)
            if r.returncode: problems.append(f'{p}: sh -n: {r.stderr.strip()}')


def check_links(files):
    for p in files:
        if not p.lower().endswith('.md'): continue
        text = open(os.path.join(root, p), encoding='utf-8').read()
        text = re.sub(r'(?ms)^```.*?^```', '', text)        # code blocks
        text = re.sub(r'`[^`\n]*`', '', text)                # code spans
        for target in re.findall(r'\]\(([^)\s]+)(?:\s+"[^"]*")?\)', text):
            if re.match(r'[a-z][a-z0-9+.-]*:', target, re.I) or target.startswith('#'): continue
            path = target.split('#')[0]
            if not os.path.exists(os.path.normpath(os.path.join(root, os.path.dirname(p), path))):
                problems.append(f'{p}: link to {target}, which does not exist')


def check_payload(files):
    for p in files:
        if BANNED.search(p):
            problems.append(f'{p}: a binary file type this repository must not hold'); continue
        full = os.path.join(root, p)
        if not os.path.isfile(full): continue
        head = open(full, 'rb').read(4)
        if head[:2] == b'MZ' or head[:1] in (b'\x80', b'\x82'):     # MZ header, or an OMF THEADR/LHEADR
            problems.append(f'{p}: looks like a DOS executable or object file')


def main():
    files = tracked()
    check_code(files)
    check_links(files)
    check_payload(files)
    for x in problems: print('PROBLEM', x)
    print(f'{len(files)} tracked files: ' + ('all checks pass' if not problems else f'{len(problems)} problems'))
    return 1 if problems else 0


if __name__ == '__main__':
    sys.exit(main())
