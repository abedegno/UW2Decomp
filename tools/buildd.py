"""Build queue for agents whose sandbox cannot start the emulator (Codex's cannot).
Run outside the sandbox:  .venv/bin/python tools/buildd.py
An agent writes build/queue/<id>.req holding one line, "match src/FILE.C [--dis NAME]"
or "verify src/FILE.C", and waits for build/queue/<id>.out (tools/remote.sh does both).
Only those two commands, src/NAME.C paths and the --dis/--no-build options are accepted."""
import os, re, subprocess, time, glob
from concurrent.futures import ThreadPoolExecutor
root = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); q = os.path.join(root, 'build', 'queue')
py = os.path.join(root, '.venv', 'bin', 'python')
OK = re.compile(r'^(match|verify) (src/[A-Za-z0-9_]+\.C)((?: --dis [A-Za-z_][A-Za-z0-9_]*| --no-build)*)$')

def run(req):
    out = req[:-4] + '.out'
    line = open(req).read().strip()
    m = OK.match(line)
    if not m:
        text = f'rejected: {line!r}\n'
    else:
        cmd, src, opts = m.groups()
        tool = 'match.py' if cmd == 'match' else 'verify.py'
        if cmd == 'verify': opts = ''
        r = subprocess.run([py, os.path.join('tools', tool), src] + opts.split(), cwd=root,
                           capture_output=True, text=True, timeout=900)
        text = r.stdout + r.stderr + f'\n[exit {r.returncode}]\n'
    open(out + '.tmp', 'w').write(text); os.replace(out + '.tmp', out); os.remove(req)

busy = set()
with ThreadPoolExecutor(3) as pool:
    while True:
        for req in glob.glob(os.path.join(q, '*.req')):
            if req not in busy:
                busy.add(req); pool.submit(run, req).add_done_callback(lambda f, r=req: busy.discard(r))
        time.sleep(1)
