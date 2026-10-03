"""Build the native port (`make port`, docs/PORT.md): compile every game C source for the host as
tools/portcheck.py does, compile the port's own C under src/port (the link stubs today), and
link them into build/port/uw2port.

    python3 tools/portbuild.py          build and link; exit 1 if any step fails
    python3 tools/portbuild.py --run    then run it, passing UW2PORT_ARGS (docs/BUILDING.md)
    python3 tools/portbuild.py --debug  the debug build (make port-debug): -g and UBSan's
                                        -fsanitize=null, in build/port-debug/uw2port; a null
                                        dereference is reported with its file and line and the
                                        program goes on, so a replay lists every one it meets
    python3 tools/portbuild.py --coverage  clang's source-based coverage, in
                                        build/port-cov/uw2port: each run writes a .profraw
                                        (LLVM_PROFILE_FILE), for llvm-profdata and llvm-cov, to
                                        see which of the port's routines the replays run

The port's own C is compiled with its headers and the game's; the platform backend (SDL3,
src/port/platform/sdl3) with SDL's flags from pkg-config, and the link takes SDL's libraries.
What the port does not replace yet is still the generated stubs (src/port/stubs, written by
tools/portstubs.py), and the port stops at the first one it reaches. The DOS build is
untouched.

The sound hardware's emulators are not in the repository (docs/BUILDING.md, "Sound"): Nuked
OPL3 is compiled from tools/nuked-opl3 when tools/setup-sound.sh has fetched it, and libmt32emu
is linked when pkg-config finds it (brew install mt32emu); without either the port builds and
that chip is silent.
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


NUKED = os.path.join(root, 'tools', 'nuked-opl3')


def sound_deps():
    """The sound emulators found: (flags for src/port/sound/audio.c, link flags, extra sources)."""
    cflags, libs, extra = [], [], []
    if os.path.exists(os.path.join(NUKED, 'opl3.c')):
        cflags += ['-DUW2_HAVE_OPL', '-I', NUKED]
        extra.append(os.path.join(NUKED, 'opl3.c'))
    r = subprocess.run(['pkg-config', '--cflags', '--libs', 'mt32emu'], capture_output=True, text=True)
    if r.returncode == 0:
        flags = r.stdout.split()
        cflags += ['-DUW2_HAVE_MT32EMU'] + [f for f in flags if f.startswith('-I')]
        libs += [f for f in flags if not f.startswith('-I')]
    return cflags, libs, extra


def compile_port(cc, path, sound_cflags=()):
    if path.startswith(NUKED + os.sep):     # third-party: its own flags, optimised
        obj = os.path.join(OUT, 'deps', os.path.basename(path)[:-2] + '.o')
        os.makedirs(os.path.dirname(obj), exist_ok=True)
        r = subprocess.run([cc, '-x', 'c', '-std=c99', '-O2', '-w', '-c', '-o', obj, path],
                           capture_output=True, text=True, cwd=root)
        return path, r.returncode, r.stderr if r.returncode else '', obj if r.returncode == 0 else None
    obj = os.path.join(OUT, 'port', os.path.relpath(path, PORT).replace(os.sep, '_')[:-2] + '.o')
    os.makedirs(os.path.dirname(obj), exist_ok=True)
    extra = pkg_config('--cflags') if is_backend(path) else []
    if path.endswith(os.path.join('sound', 'audio.c')): extra = list(sound_cflags)
    r = subprocess.run([cc] + PORT_FLAGS + extra + ['-c', '-o', obj, path], capture_output=True, text=True, cwd=root)
    return path, r.returncode, r.stderr, obj if r.returncode == 0 else None


def main(argv):
    ap = argparse.ArgumentParser(description='Build and link the native port.')
    ap.add_argument('--cc', default=os.environ.get('CC', 'cc'))
    ap.add_argument('--run', action='store_true')
    ap.add_argument('--debug', action='store_true')
    ap.add_argument('--coverage', action='store_true')
    a = ap.parse_args(argv)
    global OUT, EXE
    link_extra = []
    if a.debug:
        OUT = os.path.join(root, 'build', 'port-debug')
        EXE = os.path.join(OUT, 'uw2port')
        portcheck.OUT = OUT
        san = ['-g', '-fsanitize=null']
        portcheck.FLAGS = portcheck.FLAGS + san
        PORT_FLAGS.extend(san)
        link_extra = ['-fsanitize=null']
    if a.coverage:
        OUT = os.path.join(root, 'build', 'port-cov')
        EXE = os.path.join(OUT, 'uw2port')
        portcheck.OUT = OUT
        cov = ['-fprofile-instr-generate', '-fcoverage-mapping']
        portcheck.FLAGS = portcheck.FLAGS + cov
        PORT_FLAGS.extend(cov)
        link_extra = ['-fprofile-instr-generate']
    os.makedirs(OUT, exist_ok=True)
    game = [p for p in sources.all_sources() if p.upper().endswith('.C') and not portcheck.dos_only(p)]
    game += sources.replay_sources()     # the record and replay hooks' code, shared with the replay DOS build
    snd_cflags, snd_libs, snd_extra = sound_deps()
    with ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as ex:
        gres = list(ex.map(lambda p: portcheck.compile_one(a.cc, p), game))
        pres = list(ex.map(lambda p: compile_port(a.cc, p, snd_cflags), port_sources() + snd_extra))
    bad = [(p, e) for p, rc, e, o in gres + pres if not o]
    for p, e in bad:
        print(f'{os.path.relpath(p, root)}: does not compile\n' + '\n'.join(l for l in e.split('\n') if 'error' in l)[:2000])
    if bad: return 1
    objs = [o for p, rc, e, o in gres + pres]
    print(f'compiled {len(gres)} game sources and {len(pres)} port sources')
    warn = [(p, e) for p, rc, e, o in pres if o and 'warning' in e]
    for p, e in warn:
        print(f'{os.path.relpath(p, root)}: warnings\n' + '\n'.join(l for l in e.split('\n') if 'warning' in l)[:2000])
    print('sound: ' + ', '.join([('Nuked OPL3' if '-DUW2_HAVE_OPL' in snd_cflags else 'no OPL emulator (tools/setup-sound.sh)'),
                                 ('libmt32emu' if '-DUW2_HAVE_MT32EMU' in snd_cflags else 'no libmt32emu (brew install mt32emu)')]))
    r = subprocess.run([a.cc, '-o', EXE] + link_extra + objs + pkg_config('--libs') + snd_libs, capture_output=True, text=True, cwd=root)
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
