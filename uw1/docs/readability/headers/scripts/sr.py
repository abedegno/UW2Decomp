"""structrec.py with UW1's typedefs (portable.h) known: a wrapper, the tool unchanged."""
import sys, os, re
sys.path.insert(0, os.path.expanduser('~/Exhume/tools'))
import cparse, structrec
T = {'int16': 'int', 'uint16': 'unsigned', 'int32': 'long', 'uint32': 'unsigned long',
     'NEARPTR': 'int', 'UNEARPTR': 'unsigned'}
for k, v in T.items(): cparse.SIZES[k] = cparse.SIZES[v]
_tn = structrec.tnorm
def tnorm(t):
    for k, v in T.items(): t = re.sub(r'\b%s\b' % k, v, t)
    return _tn(t)
structrec.tnorm = tnorm
def sig(leaf):
    path, ab, nb, unit, typ, size, cont = leaf
    typ = tnorm(typ)
    if unit.startswith('bf'): return (ab, nb, unit, not re.search(r'unsigned', typ))
    return (ab, nb, unit, typ)
structrec.sig = sig
sys.exit(structrec.main(sys.argv[1:]))
