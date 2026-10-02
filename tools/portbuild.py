"""Build the native port (`make port`, docs/PORT.md): compile every game C source for the host as
tools/portcheck.py does, compile the port's own C under src/port (the link stubs today), and
link them into build/port/uw2port.

    python3 tools/portbuild.py          build and link; exit 1 if any step fails
    python3 tools/portbuild.py --run    then run it (Milestone 2: it stops at the first stub)

At Milestone 2 the binary links but does not run: the assembly modules, Borland's library,
the AIL API and the platform layer are stubs that abort (src/port/stubs, written by
tools/portstubs.py). The DOS build is untouched.
"""
import os, re, sys, argparse, subprocess
from concurrent.futures import ThreadPoolExecutor

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
sys.path.insert(0, here)
import sources, portcheck

OUT = os.path.join(root, 'build', 'port')
PORT = os.path.join(root, 'src', 'port')
EXE = os.path.join(OUT, 'uw2port')
PORT_FLAGS = ['-x', 'c', '-std=gnu99', '-Wall', '-Wno-unused-function']


def port_sources():
    out = []
    for d, _, fs in os.walk(PORT):
        if os.path.join(PORT, 'include') in d: continue
        out += [os.path.join(d, f) for f in sorted(fs) if f.endswith('.c')]
    return sorted(out)


def compile_port(cc, path):
    obj = os.path.join(OUT, 'port', os.path.relpath(path, PORT).replace(os.sep, '_')[:-2] + '.o')
    os.makedirs(os.path.dirname(obj), exist_ok=True)
    r = subprocess.run([cc] + PORT_FLAGS + ['-c', '-o', obj, path], capture_output=True, text=True, cwd=root)
    return path, r.returncode, r.stderr, obj if r.returncode == 0 else None


def main(argv):
    ap = argparse.ArgumentParser(description='Build and link the native port.')
    ap.add_argument('--cc', default=os.environ.get('CC', 'cc'))
    ap.add_argument('--run', action='store_true')
    a = ap.parse_args(argv)
    os.makedirs(OUT, exist_ok=True)
    game = [p for p in sources.all_sources() if p.upper().endswith('.C') and not portcheck.dos_only(p)]
    with ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as ex:
        gres = list(ex.map(lambda p: portcheck.compile_one(a.cc, p), game))
        pres = list(ex.map(lambda p: compile_port(a.cc, p), port_sources()))
    bad = [(p, e) for p, rc, e, o in gres + pres if not o]
    for p, e in bad:
        print(f'{os.path.relpath(p, root)}: does not compile\n' + '\n'.join(l for l in e.split('\n') if 'error' in l)[:2000])
    if bad: return 1
    objs = [o for p, rc, e, o in gres + pres]
    print(f'compiled {len(gres)} game sources and {len(pres)} port sources')
    r = subprocess.run([a.cc, '-o', EXE] + objs, capture_output=True, text=True, cwd=root)
    if r.returncode:
        und = sorted(set(re.findall(r'"_?([A-Za-z_]\w*)", referenced from', r.stderr)))
        print(f'link failed: {len(und)} undefined names' + (': ' + ' '.join(und) if und else ''))
        print(r.stderr[-3000:])
        return 1
    print(f'linked {os.path.relpath(EXE, root)} ({os.path.getsize(EXE)} bytes)')
    if a.run:
        r = subprocess.run([EXE], cwd=root)
        print(f'exit status {r.returncode}')
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
