"""Where Exhume is: the checkout whose tools the gate runs (the Makefile, tools/link.py,
tools/extract.py) and whose runtime the DOS build compiles in place: runtime/include/portable.h,
staged beside src/include for every compile, and runtime/replay/replay.c, which the replay DOS
build links (docs/BUILDING.md, "Exhume"). Adapted from UW2Decomp's tools/exhume.py.

    EXHUME      the Exhume checkout: $EXHUME; else .exhume in this repository (where CI would
                check it out); else a checkout named Exhume beside this one; else ~/Exhume
    PIN         tools/exhume-ref: the Exhume commit this tree was proved with
    check_pin() a warning when EXHUME's checkout does not contain the pinned commit (advisory:
                it never stops the build, and says nothing when git is not on PATH)

    python3 tools/exhume.py     prints the checkout and checks the pin
"""
import os, subprocess, sys

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)


def _find():
    e = os.environ.get('EXHUME')
    if e: return os.path.abspath(os.path.expanduser(e))
    for d in (os.path.join(root, '.exhume'), os.path.join(os.path.dirname(root), 'Exhume')):
        if os.path.isdir(os.path.join(d, 'tools')): return d
    return os.path.join(os.path.expanduser('~'), 'Exhume')


EXHUME = _find()
PIN = os.path.join(here, 'exhume-ref')


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
        return True
    if r.returncode == 0: return True
    print(f'warning: Exhume at {EXHUME} does not contain {pin[:12]}, the commit tools/exhume-ref names '
          f'(git -C {EXHUME} pull)', file=out)
    return False


if __name__ == '__main__':
    print(EXHUME)
    sys.exit(0 if check_pin() else 1)
