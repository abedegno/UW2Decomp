"""Where Exhume's runtime is: the porting harness the port, the replay DOS build and the gate
compile in place (docs/PORT.md, "The runtime"; docs/BUILDING.md, "Exhume").

    EXHUME      the Exhume checkout: $EXHUME; else the repository's submodule, ../exhume (the
                pinned commit: git records it); else .exhume here; else ~/Exhume
    RUNTIME     EXHUME/runtime
    PORT        RUNTIME/port: the portability layer, the platform layer and its SDL3 backend,
                the paragraph map and the EMS memory, the VGA, Borland's library, the black box,
                the PIT, the sound library and the x86 machine
    INCLUDE     RUNTIME/include: portable.h, which the DOS build stages beside src/include
    REPLAY      RUNTIME/replay/replay.c, compiled by the replay DOS build and the port
    need()      exits with a message when there is no runtime at EXHUME

tools/tcc.mjs and the Makefile find the checkout the same way.
"""
import os, subprocess, sys

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)


def _find():
    """$EXHUME; else the repository's submodule, ../exhume; else .exhume here; else ~/Exhume."""
    e = os.environ.get('EXHUME')
    if e: return os.path.abspath(os.path.expanduser(e))
    for d in (os.path.join(os.path.dirname(root), 'exhume'), os.path.join(root, '.exhume')):
        if os.path.isfile(os.path.join(d, 'tools', 'gate.py')): return d
    return os.path.join(os.path.expanduser('~'), 'Exhume')


EXHUME = _find()
RUNTIME = os.path.join(EXHUME, 'runtime')
PORT = os.path.join(RUNTIME, 'port')
INCLUDE = os.path.join(RUNTIME, 'include')
REPLAY = os.path.join(RUNTIME, 'replay', 'replay.c')


def need():
    if not os.path.isfile(os.path.join(INCLUDE, 'portable.h')) or not os.path.isfile(REPLAY):
        raise SystemExit(f'no Exhume runtime at {EXHUME}: clone https://github.com/abedegno/Exhume there, '
                         'or set EXHUME to a checkout (docs/BUILDING.md, "Exhume")')


