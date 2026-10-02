"""What a source's object depends on besides its own text: the shared headers in src/include.

    source_hash(path)   SHA-1 of the source's bytes, followed by the name and bytes of each
                        header it includes from src/include (#include "name.h", recursively,
                        each once, in order of first inclusion). A source that includes none
                        hashes to plain sha1(bytes), as before headers existed.
    headers(path)       those headers' paths, in the same order.

uw2.py (the gate's state), link.py (--mod: which sources changed) and extract.py (the base
layout that --mod compares with) all use it, so editing a header recompiles every source that
includes it. tcc.mjs copies src/include into the DOS build directory beside the source.
"""
import os, re, hashlib

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
INCLUDE = os.path.join(root, 'src', 'include')
_INC = re.compile(rb'^[ \t]*#[ \t]*include[ \t]*"([^"]+)"', re.M)


def headers(path):
    seen, order = set(), []
    def walk(p):
        for m in _INC.finditer(open(p, 'rb').read()):
            name = m.group(1).decode('latin1').lower()
            if name in seen: continue
            seen.add(name)
            h = os.path.join(INCLUDE, name)
            if not os.path.exists(h): continue     # the compiler reports it
            order.append(h); walk(h)
    walk(path)
    return order


def source_hash(path):
    h = hashlib.sha1(open(path, 'rb').read())
    for p in headers(path):
        h.update(b'\0' + os.path.basename(p).encode() + b'\0' + open(p, 'rb').read())
    return h.hexdigest()
