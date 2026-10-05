"""Where Exhume is: the checkout whose tools the gate runs (the Makefile, tools/link.py,
tools/extract.py) and whose runtime the DOS build compiles in place: runtime/include/portable.h,
staged beside src/include for every compile, and runtime/replay/replay.c, which the replay DOS
build links (docs/BUILDING.md, "Exhume"). Adapted from UW2Decomp's tools/exhume.py.

    EXHUME      the Exhume checkout: $EXHUME; else the repository's submodule, ../exhume (the
                pinned commit: git records it); else .exhume here; else ~/Exhume

    python3 tools/exhume.py     prints the checkout
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


if __name__ == '__main__':
    print(EXHUME)
