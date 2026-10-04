"""Where Exhume's runtime is: the porting harness the port, the replay DOS build and the gate
compile in place (docs/PORT.md, "The runtime"; docs/BUILDING.md, "Exhume").

    EXHUME      the Exhume checkout: $EXHUME; else .exhume in this repository (where CI checks
                it out); else ~/Exhume
    RUNTIME     EXHUME/runtime
    PORT        RUNTIME/port: the portability layer, the platform layer and its SDL3 backend,
                the paragraph map and the EMS memory, the VGA, Borland's library, the black box,
                the PIT, the sound library and the x86 machine
    INCLUDE     RUNTIME/include: portable.h, which the DOS build stages beside src/include
    REPLAY      RUNTIME/replay/replay.c, compiled by the replay DOS build and the port
    PIN         tools/exhume-ref: the Exhume commit this tree was proved with
    need()      exits with a message when there is no runtime at EXHUME
    check_pin() a warning when EXHUME's checkout does not contain the pinned commit

tools/tcc.mjs finds the checkout the same way.
"""
import os, subprocess, sys

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)


def _find():
    e = os.environ.get('EXHUME')
    if e: return os.path.abspath(os.path.expanduser(e))
    local = os.path.join(root, '.exhume')
    if os.path.isdir(os.path.join(local, 'runtime')): return local
    return os.path.join(os.path.expanduser('~'), 'Exhume')


EXHUME = _find()
RUNTIME = os.path.join(EXHUME, 'runtime')
PORT = os.path.join(RUNTIME, 'port')
INCLUDE = os.path.join(RUNTIME, 'include')
REPLAY = os.path.join(RUNTIME, 'replay', 'replay.c')
PIN = os.path.join(here, 'exhume-ref')


def need():
    if not os.path.isfile(os.path.join(INCLUDE, 'portable.h')) or not os.path.isfile(REPLAY):
        raise SystemExit(f'no Exhume runtime at {EXHUME}: clone https://github.com/abedegno/Exhume there, '
                         'or set EXHUME to a checkout (docs/BUILDING.md, "Exhume")')


def pinned():
    try: return open(PIN).read().split()[0]
    except (OSError, IndexError): return None


def check_pin(out=sys.stderr):
    """Warns when the checkout at EXHUME does not contain the commit tools/exhume-ref names;
    True when it does (or cannot be told: not a git checkout, or no git on PATH)."""
    pin = pinned()
    if not pin or not os.path.isdir(os.path.join(EXHUME, '.git')): return True
    try:
        r = subprocess.run(['git', '-C', EXHUME, 'merge-base', '--is-ancestor', pin, 'HEAD'], capture_output=True)
    except OSError:
        return True                     # no git on PATH (MSYS2's Python on Windows CI): cannot tell
    if r.returncode == 0: return True
    print(f'warning: Exhume at {EXHUME} does not contain {pin[:12]}, the commit tools/exhume-ref names '
          f'(git -C {EXHUME} pull)', file=out)
    return False
