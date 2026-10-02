"""Build the native port (`make port`, docs/PORT.md): compile every game C source for the host as
tools/portcheck.py does, compile the port's own C under src/port (the link stubs today), and
link them into build/port/uw2port.

    python3 tools/portbuild.py          build and link; exit 1 if any step fails
    python3 tools/portbuild.py --run    then run it, passing UW2PORT_ARGS (docs/BUILDING.md)

The port's own C is compiled with its headers and the game's; the platform backend (SDL3,
src/port/platform/sdl3) with SDL's flags from pkg-config, and the link takes SDL's libraries.
What the port does not replace yet is still the generated stubs (src/port/stubs, written by
tools/portstubs.py), and the port stops at the first one it reaches. The DOS build is
untouched.
"""
import os, re, sys, argparse, subprocess
from concurrent.futures import ThreadPoolExecutor

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
sys.path.insert(0, here)
import sources, portcheck

OUT = os.path.join(root, 'build', 'port')
PORT = os.path.join(root, 'src', 'port')
EXE = os.path.join(OUT, 'uw2port')
# The port's own C: C11, with the port's headers and the game's (port C that includes a game
# header includes compat.h first, and gets Borland's stand-in headers through src/port/include).
PORT_FLAGS = ['-x', 'c', '-std=gnu11', '-fsigned-char', '-D_POSIX_C_SOURCE=200809L', '-Wall', '-Wno-comment', '-Wno-unused-function',
              '-Wno-pragma-pack', '-I', PORT, '-I', os.path.join(PORT, 'platform'),
              '-I', os.path.join(PORT, 'include'), '-iquote', os.path.join(root, 'src', 'include')]
# The one backend the port builds with today (docs/PORT.md, "The platform layer"): only files
# under src/port/platform/<backend>/ see its headers, and the link takes its libraries.
BACKEND = 'sdl3'


def pkg_config(*args):
    r = subprocess.run(['pkg-config'] + list(args) + [BACKEND], capture_output=True, text=True)
    if r.returncode:
        raise SystemExit(f'portbuild.py: pkg-config cannot find {BACKEND} (install SDL3: brew install sdl3)')
    return r.stdout.split()


def port_sources():
    """Every .c under src/port but the stand-in headers, and the platform backends other than
    BACKEND."""
    out = []
    plat = os.path.join(PORT, 'platform')
    for d, _, fs in os.walk(PORT):
        if os.path.join(PORT, 'include') in d: continue
        if d.startswith(plat + os.sep) and os.path.relpath(d, plat).split(os.sep)[0] != BACKEND: continue
        out += [os.path.join(d, f) for f in sorted(fs) if f.endswith('.c')]
    return sorted(out)


def is_backend(path):
    return os.path.join(PORT, 'platform', BACKEND) + os.sep in path


def compile_port(cc, path):
    obj = os.path.join(OUT, 'port', os.path.relpath(path, PORT).replace(os.sep, '_')[:-2] + '.o')
    os.makedirs(os.path.dirname(obj), exist_ok=True)
    extra = pkg_config('--cflags') if is_backend(path) else []
    r = subprocess.run([cc] + PORT_FLAGS + extra + ['-c', '-o', obj, path], capture_output=True, text=True, cwd=root)
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
    warn = [(p, e) for p, rc, e, o in pres if o and 'warning' in e]
    for p, e in warn:
        print(f'{os.path.relpath(p, root)}: warnings\n' + '\n'.join(l for l in e.split('\n') if 'warning' in l)[:2000])
    r = subprocess.run([a.cc, '-o', EXE] + objs + pkg_config('--libs'), capture_output=True, text=True, cwd=root)
    if r.returncode:
        und = sorted(set(re.findall(r'"_?([A-Za-z_]\w*)", referenced from', r.stderr)))
        print(f'link failed: {len(und)} undefined names' + (': ' + ' '.join(und) if und else ''))
        print(r.stderr[-3000:])
        return 1
    print(f'linked {os.path.relpath(EXE, root)} ({os.path.getsize(EXE)} bytes)')
    if a.run:
        r = subprocess.run([EXE] + os.environ.get('UW2PORT_ARGS', '').split(), cwd=root)
        print(f'exit status {r.returncode}')
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
