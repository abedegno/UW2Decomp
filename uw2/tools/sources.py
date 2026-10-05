"""Where the sources are, for every tool that looks one up.

The sources live in subsystem directories under src/ (src/game, src/obj, src/3d ...; headers
in src/include). A source is known by its stem, the file name without the extension in upper
case (the stem names its object, build/STEM/STEM.OBJ, and its module in the link), so stems
are unique across the whole tree. A source's DOS segment is its `/* target: */` line: tools
that need a particular segment's source (the link order, the overlay manager) ask for it by
segment, never by file name, so renaming or moving a file needs no change here.

    all_sources()        every .C and .ASM under src/ except src/include and src/port (the
                         port's own code, docs/PORT.md, which the DOS build never compiles),
                         sorted by path
    replay_sources()     the record and replay code, which only the replay DOS build and the
                         port compile: Exhume's runtime/replay/replay.c (tools/exhume.py)
    stem(path)           'GAMESTRN' for src/ui/GAMESTRN.C
    target(path)         the /* target: */ segment, or None
    by_stem()            {stem: path}
    by_segment(seg)      the stem whose target is seg, or seg plus its paragraph
                         ('seg039' finds seg039_3452; 'ovr154' finds ovr154)
    family(seg)          the stems of a segment split into several modules, in segment
                         order: targets seg_PARA_OFFSET ('seg003' gives seg003_0272_0,
                         seg003_0272_EC, ...)
"""
import os, re, glob

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
SRC = os.path.join(root, 'src')
INCLUDE = os.path.join(SRC, 'include')
PORT = os.path.join(SRC, 'port')
_TARGET = re.compile(r'/\*\s*target:\s*(\w+)\s*\*/')


def all_sources():
    out = []
    for ext in ('C', 'ASM'):
        for p in glob.glob(os.path.join(SRC, '**', '*.' + ext), recursive=True):
            if os.path.commonpath([p, INCLUDE]) == INCLUDE: continue
            if os.path.commonpath([p, PORT]) == PORT: continue
            out.append(p)
    out.sort()
    seen = {}
    for p in out:
        s = stem(p)
        if s in seen: raise SystemExit(f'two sources with the stem {s}: {seen[s]} and {p}')
        seen[s] = p
    return out


def replay_sources():
    import exhume
    exhume.need()
    return [exhume.REPLAY]


def stem(path):
    return os.path.splitext(os.path.basename(path))[0].upper()


def head(path, n=3000):
    return open(path, encoding='latin1').read(n)


def target(path):
    m = _TARGET.search(head(path))
    return m.group(1) if m else None


def by_stem():
    return {stem(p): p for p in all_sources()}


def _targets():
    return {stem(p): target(p) for p in all_sources()}


def by_segment(seg, targets=None):
    t = targets or _targets()
    hits = [s for s, x in t.items() if x and re.fullmatch(re.escape(seg) + r'(?:_[0-9A-F]{4})?', x)]
    if len(hits) != 1: raise SystemExit(f'sources.py: {len(hits)} sources have the target {seg}: {hits}')
    return hits[0]


def family(seg, targets=None):
    t = targets or _targets()
    parts = []
    for s, x in t.items():
        m = x and re.fullmatch(re.escape(seg) + r'_[0-9A-F]{4}_([0-9A-F]+)', x)
        if m: parts.append((int(m.group(1), 16), s))
    return [s for _, s in sorted(parts)]
