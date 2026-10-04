"""Generate data-only TASM modules from your own UW.EXE for everything the link needs that
no source in src/ produces yet, plus the manifest tools/link.py links by.

    python3 tools/extract.py [--config PATH]
        writes <build>/LINK/*.ASM, manifest.json and renames.json
    python3 tools/extract.py --mod [--config PATH]
        the same for the modding build, in <build>/MODLINK

Adapted from Exhume's examples/uw2/extract.py (UW2Decomp's). The rules are TLINK's and the
same for both games (Exhume's docs/link.md); the facts are UW.EXE's, measured again, and
docs/LINKING.md lists them with the evidence: the header and segment table, the module order
from the relocation table, the C library's data range from TLINK's map, the pieces of code
and data no source holds. Exhume's tools are found through $EXHUME, or a checkout named
Exhume beside this one.

The modules hold game bytes, so they live under build/ and are never committed.

When every object verifies, the exact run also keeps what the modding build needs in
<build>/LINK/base (Exhume's tools/modding.py): a copy of each matched object, the SHA-1 of
the source it came from, and the places verify.py found for its data (layout.json). --mod
works out the same layout from those instead of from <build>, where an object may now come
from a changed source.

What is extracted, and the evidence for each piece:

- Code with no source: none. Every byte of code is a source's: the last pieces were seg016
  (sys/INT0TRAP.ASM), SetPnt (3d/SETPNT.ASM), seg018 (gfx/PLANECPY.ASM) and the routine after
  VALLOC.ASM's code in seg001 (gfx/VSTATS.ASM). Code no source covers is found by
  subtracting what the objects cover from the segment extents in the overlay manager's
  segment table (__SEGTABLE__: A1h entries of {paragraph, end, flags, start} that TLINK
  writes for every segment, in order); any such range now stops the tool. Zero bytes between
  the modules of seg003 (one, to a word), seg004 and seg019 (to a paragraph), and the 15
  ending seg019's segment, are TLINK's alignment padding, not extracted.
- The far data segments between C0's _FARDATA (entry 50) and the overlay manager's data
  (entries 51 to 76), each with the alignment its start implies: para when it starts on a
  fresh paragraph after a gap, byte when it starts exactly where the previous one ended, word
  when it skips one byte to an even address. Empty entries are kept: the original link had
  them. Entries a C file defines (its own far segments, FILE<n>_FAR, placed and compared by
  verify.py) are not declared here: TLINK places a segment where it first sees its name, so
  the C file's place in the link puts them where the EXE has them. The entries before one of
  those that no C file defines are declared, empty, in a module linked just before that C
  file (XFnnn); XFAR, after the resident objects, holds the bytes of the rest. Entries a
  source marked /* fardata */ fills (src/sys/FARDATA.ASM: entries 51, 53, 54 and 60 to 63,
  the zero-filled buffers and the checkerboard) are declared empty in XFAR, and the source,
  linked right after it, fills them.
  Entry 52 (seg048, the graphics modules' data) is filled by those modules' sources from
  their first relocation onwards (pieces placed by '# far' lines in their target tables);
  XFAR holds the EXE's bytes before the first piece. The sources name the segment's frame
  seg048 = 3963:0000, 8 bytes before the segment starts (it follows entry 51 in the same
  paragraph), so XFAR defines seg048 as an equate 10000h-8 bytes on from the segment's start,
  which TLINK resolves to 3963:0000 in 16-bit arithmetic.
  Entries 58 and 72 (the 3D modules' data) are filled the same way, from INSTANCE.ASM's and
  TMAPOPS.ASM's first relocated pointers in them onwards; XFAR holds the EXE's bytes before
  those pieces.
- DGROUP gaps: _DATA bytes and _BSS space between the objects' own data, as verify.py places
  it (or, for a file whose own code never refers to its data, as its publics' addresses in
  symbols.tsv and the other objects place it). Each gap becomes a byte-aligned module linked
  just before the next object whose data follows it, so TLINK lays it where the EXE has it.
  The second library's _DATA comes from its modules' sources; XLIBD, put last into that
  library, would hold any end of it they do not.
- Empty overlays (ovr097, ovr102, ...): their stubs have no entries and codesize 0, so the
  original had modules with an empty code segment in the overlay list.
- XORDER, XEMPTY16 and XSEG019 declare empty code segments early, because TLINK places
  segments in the order it first sees their names: seg000..seg004 must come before C0's
  _TEXT, seg019 and seg020 start on paragraphs, and segment table entries 5, 16 and 23 are
  empty code segments that the original link had. XORDER declares seg000..seg004 with their
  objects' classes (ASMCODE, then code), since TLINK orders segments by class first and sets
  the code flag only for a class ending in upper-case CODE (Exhume's
  profiles/borland-tc101/linker.md, "Segment classes"); the rest are CODE.
- XPULL, an empty module, names one public of each library module nothing else names
  (QUADFIT.ASM), and of any XP module: TLINK takes a library module only for a reference to
  it by name.

Names: every name an object references (its externs, resolved against the EXE by verify.py's
own code) and every symbols.tsv name is placed. If its address is inside an extracted range,
the module there defines it. A name that resolves where another name already is (an object
calling a library routine by its IDA name, a name for the middle of another file's array)
is recorded in renames.json, and link.py stops on it: it is a defect in the source. Every MZ
relocation inside an extracted range becomes a fixup: a far pointer (`dd NAME`) when the
offset before it is a known address (in code, only right after a far call or jump opcode),
otherwise a segment word (`dw seg NAME`) naming something in the same frame.
"""
import sys, os, re, struct, json, glob, io, contextlib
here = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.dirname(here)
# Exhume's tools: $EXHUME, or a checkout beside this one
EXHUME = os.environ.get('EXHUME') or os.path.join(os.path.dirname(ROOT), 'Exhume')
sys.path.insert(0, os.path.join(EXHUME, 'tools'))
import config, modding
from fixups import fixups
from sources import all_sources, stem as stem_of, target as target_of, by_segment as _by_segment

CFGPATH, _rest = config.pop_config(sys.argv[1:])
CFG = config.load(CFGPATH or os.path.join(ROOT, 'exhume.toml'))
root = CFG.root
# --mod: the modding build (docs/link.md). The layout is worked out from the matched objects
# and their places as the last exact run recorded them (BASE), not from the objects in
# <build>, which may hold changed sources; the modules go to <build>/MODLINK.
MOD = '--mod' in _rest
BASE = os.path.join(CFG.build, 'LINK', 'base')
NO_BASE = 'run the exact link (python3 tools/link.py) once while every source matches'
OUT = os.path.join(CFG.build, 'MODLINK' if MOD else 'LINK')
TC = CFG.home
exe = CFG.exe
w16 = lambda b, i: struct.unpack_from('<H', b, i)[0]

# ---- MZ header, relocations, overlay segment table -------------------------------------
HDR = w16(exe, 8) * 16
cblp, cp = w16(exe, 2), w16(exe, 4)
MZEND = (cp - 1) * 512 + cblp if cblp else cp * 512
assert exe[MZEND:MZEND + 4] == b'FBOV', 'expected one FBOV block after the load image'
SEGTAB = struct.unpack_from('<I', exe, MZEND + 8)[0]; NSEG = struct.unpack_from('<I', exe, MZEND + 12)[0]
# ---- UW.EXE's facts, measured (docs/LINKING.md) -----------------------------------------
# segment table entries: 0..49 code (0..4 before C0, 5 empty, 6 C0's _TEXT with the C library,
# 49 the overlay manager's _OVRTEXT_), 50 C0's _FARDATA, 51..76 far data, 89..154 the stubs
# of ovr089..ovr154, 155 DGROUP
assert (HDR, NSEG) == (0x3200, 0xA1), 'not the UW.EXE this was written for'
E_TEXT, E_OVRTEXT, E_FARDATA, E_DGROUP = 6, 49, 50, 155
E_FAR = range(51, 77)
E_STUBS = range(89, 155)
E_PAD = (3, 4, 21)        # seg003, seg004 and seg019: libraries of modules TLINK padded apart
RELOCS = {}
for i in range(w16(exe, 6)):
    o, s = struct.unpack_from('<HH', exe, w16(exe, 0x18) + 4 * i)
    loc = HDR + s * 16 + o; RELOCS[loc] = w16(exe, loc)
SEGS = []    # (index, paragraph, file start, file end, flags)
for i in range(NSEG):
    para, mx, fl, mn = struct.unpack_from('<4H', exe, SEGTAB + 8 * i)
    SEGS.append((i, para, HDR + para * 16 + mn, HDR + para * 16 + mx if mx != 0xFFFF else None, fl))
DS_PARA = SEGS[E_DGROUP][1]; DS_FILE = HDR + DS_PARA * 16
assert DS_PARA == CFG.dgroup_para == 0x5AAC
TEXT_PARA = SEGS[E_TEXT][1]                 # C0's _TEXT, with the C library's code appended
file_of = lambda para, off: HDR + para * 16 + off
def addr_file(a):
    return DS_FILE + a[1] if a[0] == 'DS' else file_of(a[1], a[2])
# overlay stubs: segment table entries 89..154 are the stubs of ovr089..ovr154. After a
# 0x20-byte header each entry is INT 3Fh, the function's offset in its overlay, and a zero.
STUB = {SEGS[i][1]: i for i in E_STUBS}

# UW1's targets for the modules of a split segment are SEG_OFFSET (seg003_1082), not UW2's
# SEG_PARA_OFFSET, so the segment's modules are found here rather than by sources.family
def family(seg, tmap):
    parts = []
    for s_, x in tmap.items():
        m_ = x and re.fullmatch(re.escape(seg) + r'_([0-9A-Fa-f]+)', x)
        if m_ and seg in ('seg003', 'seg004', 'seg019'): parts.append((int(m_.group(1), 16), s_))
    return [s_ for _, s_ in sorted(parts)]
def by_segment(seg, tmap):
    if seg in ('seg003', 'seg004', 'seg019'): sys.exit(f'{seg} is several modules: use M()')
    return _by_segment(seg, tmap=tmap)

# ---- objects --------------------------------------------------------------------------
OBJS = {}
SOURCES = all_sources(CFG)
TARGETS = {stem_of(p): target_of(p) for p in SOURCES}
for src in SOURCES:
    m = re.search(r'/\*\s*target:\s*(\w+)\s*\*/', open(src, encoding='latin1').read(3000))
    if not m: continue
    stem = stem_of(src)
    if stem == by_segment('seg046', TARGETS): continue      # the overlay manager: linked from OVERLAY.LIB itself
    obj = CFG.obj(stem)
    if MOD:
        obj = os.path.join(BASE, stem + '.OBJ')
        if not os.path.exists(obj): sys.exit(f'{stem}: no matched object in {BASE}: {NO_BASE}')
    o = fixups(open(obj, 'rb').read())
    hdr = open(os.path.join(CFG.targets, m.group(1) + '.tsv')).readline()
    base = int(re.search(r'base 0x([0-9A-Fa-f]+)', hdr).group(1), 16)
    code = [s for s in o['segs'][1:] if CFG.is_code_class(s[1])]
    seglen = lambda n: sum(s[2] for s in o['segs'][1:] if s[0] == n)
    OBJS[stem] = dict(src=src, obj=obj, target=m.group(1), base=base, codeseg=code[0][0], codecls=code[0][1], codelen=code[0][2],
                      datalen=seglen('_DATA'), bsslen=seglen('_BSS'), o=o, data=None, bss=None)

# what verify.py resolves for each object: its _DATA and _BSS bases, every extern's address
import verify
_captured = {}
ALL_VERIFIED = True
for stem, ob in OBJS.items():
    if MOD: break
    _captured.clear(); buf = io.StringIO()
    # verify's main takes its arguments directly and hands over the names it resolved;
    # nothing here touches sys.argv
    with contextlib.redirect_stdout(buf):
        rc = verify.main([ob['src'], '--config', CFG.file], on_names=lambda syms: _captured.update({n: v for n, (v, _) in syms.items()}))
    ALL_VERIFIED = ALL_VERIFIED and not rc
    v = buf.getvalue()
    for kind, n, at in re.findall(r'^_(DATA|BSS): (\d+) bytes.*?DS:([0-9A-F]+)', v, re.M):
        ob[kind.lower()] = int(at, 16)
    ob['refs'] = {n: (a[0],) + tuple(a[1:]) for n, a in _captured.items()}
    for n, p in re.findall(r'^(\S+): segment ([0-9A-F]{4}) referenced on its own', v, re.M):
        ob['refs'].setdefault(n, ('SEG', int(p, 16)))
    # the file's own far segments, where verify.py placed them
    ob['far'] = [(n, int(p, 16), int(q, 16), int(ln)) for n, ln, p, q in
                 re.findall(r'^(\S+): (\d+) bytes match at ([0-9A-F]{4}):([0-9A-F]{4})', v, re.M)]

if MOD:
    # each object's places as the exact run found them (verify.py needs the EXE's bytes at the
    # object's own offsets, which a changed object no longer has)
    _lay = modding.load_snapshot(BASE, NO_BASE)
    for stem, ob in OBJS.items():
        if stem not in _lay['objects']: sys.exit(f'{stem}: not in {BASE}/layout.json')
        L = _lay['objects'][stem]
        ob['data'], ob['bss'] = L['data'], L['bss']
        ob['refs'] = {n: tuple(a) for n, a in L['refs'].items()}
        ob['far'] = [tuple(x) for x in L['far']]

# A file whose own code never refers to its _DATA or _BSS (data defined for other files; UW2's
# example was its seg033's ActDoors) is placed by its publics instead: where other files' references and
# symbols.tsv put them. They must agree on one base.
def _place_by_publics():
    known = {}
    for l in open(CFG.symbols):
        if l.startswith('#') or not l.strip(): continue
        n, a = l.split('\t')[:2]
        if a.startswith('DS:'): known.setdefault(n, set()).add(int(a[3:], 16))
    for ob in OBJS.values():
        for n, a in ob['refs'].items():
            if a[0] == 'DS': known.setdefault(n, set()).add(a[1])
            elif a[0] == 'FAR' and a[1] == DS_PARA: known.setdefault(n, set()).add(a[2])
    for stem, ob in OBJS.items():
        for kind, sn in (('data', '_DATA'), ('bss', '_BSS')):
            if not ob[kind + 'len'] or ob[kind] is not None: continue
            o = ob['o']
            bases = {(at - off) & 0xFFFF for n, (si, off) in o['pubs'].items()
                     if si and o['segs'][si][0] == sn for at in known.get(n, ())}
            if len(bases) != 1:
                sys.exit(f'{stem}: verify.py did not place its {sn}, and its publics give ' +
                         (', '.join(f'DS:{b:X}' for b in sorted(bases)) or 'no address'))
            ob[kind] = bases.pop()
_place_by_publics()

# ---- libraries ------------------------------------------------------------------------
# Measured from TLINK's map of this link (build/LINK/out/UW.MAP): the C library's modules are
# pulled by what the objects reference, so the linker decides them. C0's _TEXT is 0x249 bytes
# (the C library's code follows it in segment table entry 6). The C library's _DATA starts at
# CLIB_DATA (ATEXIT's two bytes, then __ctype) and ends at LIBDATA_START, where the second
# library's starts; that runs to LIBDATA_END, where the overlay manager's __OvrSize is. The C
# library's _BSS starts at LIBBSS_START.
C0_TEXT_LEN = 0x249
CLIB_DATA, LIBDATA_START, LIBDATA_END, LIBBSS_START = 0x1DFE, 0x2258, 0x242C, 0x738E
def lib_modules(path):
    d = open(path, 'rb').read(); page = w16(d, 1) + 3; p = page
    while p < len(d) and d[p] == 0x80:
        q = p
        while True:
            t = d[q]; q += 3 + w16(d, q + 1)
            if t in (0x8A, 0x8B): break
        try: yield fixups(d[p:q])
        except (KeyError, IndexError): SKIPPED.append(d[p + 4:p + 4 + d[p + 3]].decode('latin1'))
        p = ((q + page - 1) // page) * page
SKIPPED = []
CMMODS = list(lib_modules(os.path.join(TC, 'CM.LIB')))
OVMODS = list(lib_modules(os.path.join(TC, 'OVERLAY.LIB')))
C0 = fixups(open(os.path.join(TC, 'C0M.OBJ'), 'rb').read())
if SKIPPED: print('library modules fixups.py cannot read (their names are not used):', ' '.join(SKIPPED))
LIBPUBS = set(C0['pubs']).union(*(set(m['pubs']) for m in CMMODS + OVMODS))
OBJPUBS = set().union(*(set(o['o']['pubs']) for o in OBJS.values()))

def masked_find(o, si, hay, lo=0):
    """Where segment si of module o occurs in hay, its fixups masked (None unless exactly once)."""
    data = bytes(o['data'][si]); mask = bytearray(len(data))
    for f in o['fixups']:
        if f['seg'] == si:
            for k in range({0: 1, 1: 2, 2: 2, 3: 4, 4: 1, 5: 2}.get(f['loc'], 2)):
                if f['off'] + k < len(mask): mask[f['off'] + k] = 1
    if not data: return None
    pat = b''.join(b'.' if mask[k] else re.escape(data[k:k + 1]) for k in range(len(data)))
    hits = [x.start() for x in re.finditer(pat, hay[lo:], re.S)]
    return lo + hits[0] if len(hits) == 1 else None

# ---- definers: address -> name ----------------------------------------------------------
DEFS = {}        # ('DS', off) or ('FAR', para, off) -> name
NAMEADDR = {}    # every defined name -> its address
def define(key, name): DEFS.setdefault(key, name); NAMEADDR.setdefault(name, key)
def seg_at(a):
    for s in SEGS:
        if s[3] is not None and s[2] <= a < s[3]: return s
ODEF = {}        # overlay number -> {offset: name}
for stem, ob in OBJS.items():
    o = ob['o']
    for name, (si, off) in o['pubs'].items():
        sn, cl, ln = o['segs'][si]
        if CFG.is_code_class(cl) and ob['base'] < MZEND:
            s = seg_at(ob["base"]) or sys.exit(f"{stem}: base {ob['base']:X} {ob['target']}"); define(('FAR', s[1], ob['base'] - file_of(s[1], 0) + off), name)
        elif CFG.is_code_class(cl):
            ODEF.setdefault(int(ob['target'][3:]), {})[off] = name
        elif sn == '_DATA': define(('DS', ob['data'] + off), name)
        elif sn == '_BSS': define(('DS', ob['bss'] + off), name)
for para, n in STUB.items():
    lo, hi = SEGS[n][2], SEGS[n][3]
    for e in range(0x20, hi - lo, 5):
        a = lo + e
        if exe[a:a + 2] == b'\xcd\x3f' and w16(exe, a + 2) in ODEF.get(n, {}):
            define(('FAR', para, e), ODEF[n][w16(exe, a + 2)])
# the C library's code and data
TEXT_FILE = file_of(TEXT_PARA, 0); seg5 = exe[TEXT_FILE:SEGS[E_TEXT][3]]
dsbytes = exe[DS_FILE:DS_FILE + LIBDATA_END + 0x10]
for o in CMMODS + OVMODS:
    for si in range(1, len(o['segs'])):
        sn, cl, ln = o['segs'][si]
        if not ln: continue
        if sn == '_TEXT': at, key = masked_find(o, si, seg5, C0_TEXT_LEN), lambda x: ('FAR', TEXT_PARA, x)
        elif sn == '_DATA': at, key = masked_find(o, si, dsbytes, CLIB_DATA), lambda x: ('DS', x)
        else: continue
        if at is None: continue
        for name, (psi, off) in o['pubs'].items():
            if psi == si: define(key(at + off), name)
# C0: its _DATA is at DS:4; its _TEXT routines where UW1's C0 has them, the same offsets as
# UW2's (the code is the same but for addresses; see link.py's c0_source)
for name, (si, off) in C0['pubs'].items():
    if si and C0['segs'][si][0] == '_DATA': define(('DS', 4 + off), name)
for name, off in (('__exitclean', 0x113), ('__exit', 0x133), ('__restorezero', 0x1B6), ('_abort', 0x22E),
                  ('DGROUP@', 0x245), ('__MMODEL', 0x247)):
    define(('FAR', TEXT_PARA, off), name)

# ---- far data with a source -------------------------------------------------------------
# A source marked /* fardata */ (src/sys/FARDATA.ASM) defines whole far data segments, each
# named FDnn after its segment table entry; link.py assembles it into build/STEM before
# this runs. Each segment must equal the EXE's bytes, and holds no relocations. XFAR then
# declares that entry empty (keeping the EXE's segment order) and the source fills it.
FARSRC = {}      # segment table entry -> stem
FAROBJS = {}     # stem -> object path
FARPATHS = {}    # stem -> source path
_tsvnames = {l.split('\t')[0] for l in open(CFG.symbols) if not l.startswith('#')}
for src in SOURCES:
    if not src.upper().endswith('.ASM'): continue
    if not re.search(r'/\*\s*fardata\s*\*/', open(src, encoding='latin1').read(3000)): continue
    stem = stem_of(src); FARPATHS[stem] = src
    obj = CFG.obj(stem)
    if MOD: obj = os.path.join(BASE, stem + '.OBJ')
    elif not os.path.exists(obj) or os.path.getmtime(obj) < os.path.getmtime(src):
        sys.exit(f'{stem}: {obj} is missing or older than its source (link.py builds it)')
    o = fixups(open(obj, 'rb').read()); FAROBJS[stem] = obj
    if o['fixups']: sys.exit(f'{stem}: far data with fixups is not handled yet')
    pubs_by_seg = {}
    for name, (si, off) in o['pubs'].items(): pubs_by_seg.setdefault(si, []).append((name, off))
    for si in range(1, len(o['segs'])):
        sn, cl, ln = o['segs'][si]
        m_ = re.fullmatch(r'FD(\d\d)', sn)
        if cl != 'FAR_DATA' or not m_ or int(m_.group(1)) not in E_FAR:
            sys.exit(f'{stem}: segment {sn} ({cl}) is not a far data segment FD{E_FAR[0]}..FD{E_FAR[-1]}')
        i = int(m_.group(1)); _, para, lo, hi, _ = SEGS[i]
        if i in FARSRC: sys.exit(f'{stem}: FD{i} is also defined by {FARSRC[i]}')
        if hi is None or hi - lo != ln or bytes(o['data'][si]) != exe[lo:hi]:
            sys.exit(f'{stem}: FD{i} does not equal segment table entry {i} of the EXE')
        FARSRC[i] = stem
        # names in symbols.tsv first, so that a pointer or segment word gets that name
        for name, off in sorted(pubs_by_seg.get(si, []), key=lambda p: (p[0] not in _tsvnames, p[1], p[0])):
            define(('FAR', para, lo - file_of(para, 0) + off), name)
    OBJPUBS |= set(o['pubs'])

# ---- far data the C sources define ------------------------------------------------------
# Each far variable of a C file is a segment of its own; verify.py has placed it and compared
# its bytes. It must be a whole entry of the segment table, and the files must be linked in
# the order of their entries, since TLINK places each segment where it first sees it.
FARSEGS = [SEGS[i] for i in E_FAR]
FAROWN = {}      # segment table entry -> stem
FARPIECES = {}   # segment table entry -> [(file lo, file hi, stem)]: pieces assembly modules contribute
for stem, ob in OBJS.items():
    o = ob['o']
    for sn, para, off, ln in ob['far']:
        hits = [s[0] for s in FARSEGS if s[1] == para and s[2] == file_of(para, off) and s[3] is not None and s[3] - s[2] == ln]
        if len(hits) != 1:
            # a piece of a segment that several modules contribute to (the graphics and 3D
            # modules' data, seg_370D, seg052_519C and dseg062_62a6)
            hits = [s[0] for s in FARSEGS if s[1] == para and s[3] is not None and s[2] <= file_of(para, off) and file_of(para, off) + ln <= s[3]]
            if len(hits) != 1: sys.exit(f'{stem}: its far segment {sn} ({ln} bytes at {para:04X}:{off:04X}) is not inside an entry of the segment table')
            i = hits[0]
            if i in FARSRC or i in FAROWN: sys.exit(f'{stem}: FD{i} is also defined by {FARSRC.get(i) or FAROWN[i]}')
            FARPIECES.setdefault(i, []).append((file_of(para, off), file_of(para, off) + ln, stem))
            si = next(k for k in range(1, len(o['segs'])) if o['segs'][k][0] == sn)
            for name, (psi, poff) in o['pubs'].items():
                if psi == si: define(('FAR', para, off + poff), name)
            continue
        i = hits[0]
        if i in FARSRC or i in FAROWN: sys.exit(f'{stem}: FD{i} is also defined by {FARSRC.get(i) or FAROWN[i]}')
        FAROWN[i] = stem
        si = next(k for k in range(1, len(o['segs'])) if o['segs'][k][0] == sn)
        for name, (psi, poff) in o['pubs'].items():
            if psi == si: define(('FAR', para, off + poff), name)

# The pieces of an entry must run without a gap to its end; the EXE's bytes before the first
# one stay in XFAR (FARCUT: where they stop).
FARCUT = {}
for i, ps in FARPIECES.items():
    ps.sort()
    for (a, b, k), (c, d, k2) in zip(ps, ps[1:]):
        if b != c: sys.exit(f'FD{i}: {k} ends at {b:X} but {k2} starts at {c:X}')
    if ps[-1][1] != SEGS[i][3]: sys.exit(f'FD{i}: its last piece ({ps[-1][2]}) ends at {ps[-1][1]:X}, the segment at {SEGS[i][3]:X}')
    FARCUT[i] = ps[0][0]

# ---- references: name -> address ---------------------------------------------------------
REFS = {}
for l in open(CFG.symbols):
    if l.startswith('#') or not l.strip(): continue
    n, a = l.split('\t')[:2]
    if a.startswith('DS:'): REFS[n] = ('DS', int(a[3:], 16))
    else: p, o = a.split(':'); REFS[n] = ('FAR', int(p, 16), int(o, 16))
for ob in OBJS.values():
    for n, a in ob['refs'].items():
        if a[0] == 'FAR' and a[1] == DS_PARA: a = ob['refs'][n] = ('DS', a[2])    # a far pointer to DGROUP data
        REFS[n] = a if a[0] != 'SEG' or n not in REFS else REFS[n]
for n, a in list(REFS.items()):
    if a[0] == 'FAR' and a[1] == DS_PARA: REFS[n] = ('DS', a[2])
# One name, two variables: verify.py keeps one address per name, so an extern whose segment
# words in one file point somewhere else than everyone else's goes unnoticed (UW2's CUTS used
# sound_fpage for a one-byte far variable, its SOUND and TMPALLOC for one in DGROUP; UW.EXE
# has no such case). Such a file gets its own alias for the name, at the address its own bytes give.
PRE = {}; RETARGET = {}
for stem, ob in OBJS.items():
    o = ob['o']; code = next(i for i, sg in enumerate(o['segs']) if sg and CFG.is_code_class(sg[1]))
    uses = {}
    for f in o['fixups']:
        if f['seg'] != code or f['target'][0] != 2: continue
        n = o['ext'][f['target'][1]]; at = ob['base'] + f['off']
        if f['loc'] == 2:
            v = w16(exe, at); uses.setdefault(n, []).append(('seg', w16(exe, SEGTAB + v) if ob['base'] >= MZEND else v, f['off']))
        elif f['loc'] in (1, 5) and f['frame'] and f['frame'][0] == 5:
            uses.setdefault(n, []).append(('off', (w16(exe, at) - w16(o['data'][code], f['off']) - f['disp']) & 0xFFFF, f['off']))
    for n, us in uses.items():
        a = REFS.get(n)
        if not a or a[0] not in ('FAR', 'DS'): continue
        para, off = (DS_PARA, a[1]) if a[0] == 'DS' else (a[1], a[2])
        odd = [u for u in us if (u[0] == 'seg' and u[1] != para) or (u[0] == 'off' and u[1] != off)]
        if not odd: continue
        ps = {u[1] for u in odd if u[0] == 'seg'} or {para}; os_ = {u[1] for u in odd if u[0] == 'off'} or {off}
        if len(ps) != 1 or len(os_) != 1: sys.exit(f'{stem}: {n} is used for several addresses')
        p, q = min(ps), min(os_); mine = ('DS', q) if p == DS_PARA else ('FAR', p, q)
        alias = f'{n}_{stem}'; REFS[alias] = mine; ob['refs'][alias] = mine
        if len(odd) == len(us): del ob['refs'][n]; PRE.setdefault(stem, {})[n] = alias
        else:
            # some of this file's uses mean another variable: retarget just those fixups
            seg_offs = {u[2] for u in odd}
            # the offset half that goes with each odd segment word, and vice versa, moves too
            RETARGET.setdefault(stem, []).extend([u[2], n, alias] for u in us
                                                 if u[2] in seg_offs or any(abs(u[2] - v) <= 8 for v in seg_offs))

# ---- the link order -------------------------------------------------------------------
# Module order, from the relocation table (TLINK writes relocations module by module): C0,
# seg006..seg015, seg016 (INT0TRAP, the divide-by-zero trap, one relocation), seg000, seg019's
# first module (STARTUP), seg001 (VALLOC) and VSTATS, the routine after it (one relocation),
# seg020 (AIL), seg021..seg031, seg017's C (GRIDDB, whose _DATA is between seg031's and
# seg032's too), seg032..seg044, the overlays in order, then the C library, then seg019's
# other modules, seg003's, seg004's and seg045 interleaved (a second library: their _DATA
# follows the C library's and seg045's _BSS is the last in DGROUP), then the overlay manager.
# seg002 carries no relocations and no data, so its place is free; it goes next to seg001.
# SetPnt (SETPNT.ASM, seg017's first 0x35 bytes) and seg018 (PLANECPY.ASM, no relocations)
# must be seen after seg016 and before STARTUP declares seg019's segment, since TLINK places
# segments in the order it first sees their names; they go right after seg016, where the
# extracted modules that held them were.
#
# The sources are named here by their DOS segment (their /* target: */ line, tools/sources.py),
# not by file name: S('seg039') is the stem of the file whose target is seg039_3495, and
# M('seg003', 'B') the second module of seg003's library (the modules in segment order, A
# first). So renaming or moving a source needs no change here.
def S(seg): return by_segment(seg, TARGETS)
def M(seg, letters): return [family(seg, TARGETS)[ord(c) - ord('A')] for c in letters]
RES = [S('seg%03d' % n) for n in range(6, 15)] + ['XEMPTY16', S('seg015'), S('seg016'), S('seg017_1FDD_1'), S('seg018'),
                                                   'XSEG019', S('seg000')] + \
      M('seg019', 'A') + [S('seg001'), S('seg001_2D1'), S('seg002'), S('seg020')] + \
      [S('seg%03d' % n) for n in range(21, 32)] + [S('seg017')] + \
      [S('seg%03d' % n) for n in range(32, 45)] + ['XFAR']
OVLNUMS = list(E_STUBS)
# The second library, after the C library, in the order of its modules' relocations (TLINK
# takes a library's modules in library order): seg003, seg004 and seg019 are each several
# modules (seg003's A..R, seg004's A..I, seg019's B..Q; seg019's A, the first module of its
# segment, is in the resident list above). Modules with no relocations sit next to their
# neighbours in the same segment.
LATE = M('seg019', 'BCDEFGHIJKLM') + M('seg004', 'ABCDEFG') + M('seg003', 'A') + \
       M('seg019', 'NOPQ') + [S('seg045')] + M('seg004', 'H') + M('seg003', 'BCDEFG') + \
       M('seg004', 'I') + M('seg003', 'HIJKLMNOPQR')
def ovl_module(n):
    for k, o in OBJS.items():
        if o['target'] == 'ovr%03d' % n: return k
    return 'XOVR%03d' % n
OVL = [ovl_module(n) for n in OVLNUMS]

# ---- extracted ranges -------------------------------------------------------------------
cover = sorted((o['base'], o['base'] + o['codelen'], k) for k, o in OBJS.items() if o['base'] < MZEND)
def uncovered(lo, hi):
    pieces = []; at = lo
    for a, b, k in cover:
        if b <= at or a >= hi: continue
        if a > at: pieces.append((at, a))
        at = max(at, b)
    if at < hi: pieces.append((at, hi))
    return pieces
# segment index -> (module, segment name) for code with no source, extracted from the EXE;
# empty now that seg016, SetPnt, seg018 and the routine after VALLOC.ASM have sources. An
# uncovered range anywhere else in the resident code would be a tail of the object before
# it (XT modules); there is none.
CODE_GAPS = {}
GAPS = {}; TAILS = {}
for i, para, lo, hi, fl in SEGS[:E_OVRTEXT]:
    if i in (5, E_TEXT) or hi is None or hi <= lo: continue        # C0 and the C library: _TEXT
    for a, b in uncovered(lo, hi):
        if i in CODE_GAPS: GAPS.setdefault(i, []).append((a, b)); continue
        # zero bytes before a paragraph (or one before a word) in seg003, seg004 and seg019: the
        # linker's padding before the next module, or before a later empty contribution
        if i in E_PAD and not any(exe[a:b]) and (b % 16 == 0 and b - a < 16 or b - a == 1 and b % 2 == 0):
            continue
        # bytes after an object's code inside its own segment: a later contribution to it
        owner = [k for x, y, k in cover if y == a]
        if len(owner) != 1 or owner[0] in LATE: sys.exit(f'unexpected uncovered code {a:X}-{b:X} in segment {i}')
        TAILS[owner[0]] = (i, a, b)
# entry 50, an empty segment right after _OVRTEXT_, is C0's _FARDATA
FAR = [SEGS[i] for i in E_FAR]

C0_DATA = (0x4, 0xAC)
ORDER_ALL = ['C0UW1'] + RES + OVL
def place_gaps(kind):
    """Gaps in _DATA or _BSS between the objects, as (lo, hi, slot) pieces; slot is ('after', P)
    or ('before', N) for the objects either side. Turbo C objects with no data still add an
    empty word-aligned contribution, which pads an odd position: so a gap starting at an odd
    address goes right after P, one ending at an odd address (where N's data starts, made
    byte-aligned by link.py) right before N, and one doing both is split after its first byte."""
    size = kind + 'len'
    have = [(OBJS[k][kind], OBJS[k][kind] + OBJS[k][size], k) for k in ORDER_ALL if k in OBJS and OBJS[k][size]]
    if kind == 'data': have.insert(0, (C0_DATA[0], C0_DATA[1], 'C0UW1'))
    for (a, b, k), (c, d, k2) in zip(have, have[1:]):
        assert c >= b, f'{kind}: {k2} at {c:X} overlaps {k} ending {b:X}: link order is wrong'
    pieces = []
    for (a, b, k), (c, d, k2) in zip(have, have[1:]):
        if c == b or (c - b == 1 and b % 2 and exe[DS_FILE + b] == 0): continue     # nothing, or word alignment
        if b % 2 and c % 2: pieces += [(b, b + 1, ('after', k)), (b + 1, c, ('before', k2))]
        elif b % 2: pieces.append((b, c, ('after', k)))
        else: pieces.append((b, c, ('before', k2)))
    return pieces, have[-1]
DPIECES, (_, data_end, data_last) = place_gaps('data')
BPIECES, (_, bss_end, bss_last) = place_gaps('bss')
assert data_end <= LIBDATA_START
# _BSS after the last object's and before the C library's: game globals
if LIBBSS_START > bss_end: BPIECES.append((bss_end, LIBBSS_START, ('after', bss_last)))
# objects whose _DATA or _BSS starts at an odd address: their copies are made byte-aligned
ODD = sorted({k for k, o in OBJS.items() if (o['datalen'] and o['data'] % 2) or (o['bsslen'] and o['bss'] % 2)})
def sfx(k):
    """A generated module's suffix for the object k: its segment's number, and for a module
    of a split segment its letter ('019A' for SetPnt, the first module of seg019's segment)."""
    if k == 'C0UW1': return 'C0'
    t = OBJS[k]['target']; m = re.match(r'(?:seg|ovr)(\d{3})', t)
    fam = family(t[:6], tmap=TARGETS)
    return m.group(1) + (chr(ord('A') + fam.index(k)) if k in fam else '')
def dname(slot): return ('XA' if slot[0] == 'after' else 'XB') + sfx(slot[1])

# every extracted range: (file lo, file hi, frame para or DS_PARA, module)
RANGES = []
# Far data pieces of library modules whose sources do not hold them: a library module's far
# data whose relocations sit in the relocation table among that module's own (TLINK writes a
# module's relocations together, its data's before its code's here) would go, from each
# piece's first relocated far pointer to the next piece (or the segment's end), into a module
# of its own, XPnnnL, put into the library right before the module (XPULL names it, so TLINK
# takes it). LIBFAR gives each such module's pieces by the relocation-table entry of their
# first relocation. Empty: the last ones, INSTANCE's in entries 72 and 58 (from entries 2639
# and 2640) and TMAPOPS's in entry 58 (from 2709), are in those sources now (3d/INSTANCE.ASM,
# 3d/TMAPOPS.ASM, placed by '# far' lines in their target tables, like the graphics modules'
# pieces of entry 52).
LIBFAR = {}
RELLIST = list(RELOCS)            # file offsets, in relocation table order
LIBPIECES = {}                    # module -> [(entry, file lo)], in relocation order
for k, firsts in LIBFAR.items():
    for t in firsts:
        loc = RELLIST[t]
        i = next(sg[0] for sg in FAR if sg[3] is not None and sg[2] <= loc < sg[3])
        if i in FARPIECES or i in FARSRC or i in FAROWN: sys.exit(f'FD{i}: {k}\'s piece and a source both fill it')
        start = loc - 2 if loc - 2 >= SEGS[i][2] and loc - 2 not in RELOCS else loc
        LIBPIECES.setdefault(k, []).append((i, start))
_starts = {}
for k, ps in LIBPIECES.items():
    for i, a in ps: _starts.setdefault(i, []).append(a)
LIBRANGES = {}                    # module -> [(entry, lo, hi)]
for k, ps in LIBPIECES.items():
    for i, a in ps:
        b = min([x for x in _starts[i] if x > a] + [SEGS[i][3]])
        LIBRANGES.setdefault(k, []).append((i, a, b))
        # the piece's relocations must be one run of the relocation table, from its first
        ts = sorted(RELLIST.index(r) for r in RELOCS if a <= r < b)
        if ts != list(range(ts[0], ts[0] + len(ts))) or ts[0] not in LIBFAR[k]:
            sys.exit(f'FD{i}: the piece {a:X}..{b:X} of {k} does not hold one run of relocations from its first')
for i, a_s in _starts.items(): FARCUT[i] = min(a_s)
for k, rs in LIBRANGES.items():
    for i, a, b in rs: RANGES.append((a, b, SEGS[i][1], 'XP' + sfx(k)))
    LATE.insert(LATE.index(k), 'XP' + sfx(k))
for i, pieces in GAPS.items():
    for a, b in pieces: RANGES.append((a, b, SEGS[i][1], CODE_GAPS[i][0]))
for k, (i, a, b) in TAILS.items(): RANGES.append((a, b, SEGS[i][1], 'XT' + sfx(k)))
for i, para, lo, hi, fl in FAR:
    if hi is not None and hi > lo and i not in FARSRC and i not in FAROWN and FARCUT.get(i, hi) > lo:
        RANGES.append((lo, FARCUT.get(i, hi), para, 'XFAR'))
for a, b, slot in DPIECES + BPIECES: RANGES.append((DS_FILE + a, DS_FILE + b, DS_PARA, dname(slot)))
# the second library's _DATA: what its modules' sources do not hold is XLIBD, linked last
_lib = sorted((OBJS[k]['data'], OBJS[k]['data'] + OBJS[k]['datalen'], k) for k in LATE if k in OBJS and OBJS[k]['datalen'])
for (a, b, k), (c, d, k2) in zip(_lib, _lib[1:]):
    if c < b: sys.exit(f'second library: {k2}\'s _DATA at {c:X} overlaps {k}\'s ending at {b:X}')
if [k for _, _, k in _lib] != [k for k in LATE if k in OBJS and OBJS[k]['datalen']]:
    sys.exit('second library: its modules\' _DATA is not in their link order')
LIBGAPS = []; _at = LIBDATA_START
for a, b, k in _lib:
    if a < LIBDATA_START or b > LIBDATA_END: sys.exit(f'{k}: its _DATA {a:X}..{b:X} is outside the second library\'s')
    if a > _at: LIBGAPS.append((_at, a))
    _at = b
if _at < LIBDATA_END: LIBGAPS.append((_at, LIBDATA_END))
if any(b != LIBDATA_END and _lib for a, b in LIBGAPS):
    sys.exit('second library: _DATA no source holds lies between its modules\' (XLIBD only fills the end): ' +
             ', '.join(f'DS:{a:X}..{b:X}' for a, b in LIBGAPS))
for a, b in LIBGAPS: RANGES.append((DS_FILE + a, DS_FILE + b, DS_PARA, 'XLIBD'))

def range_of(a):
    """The extracted range holding address a, if any."""
    f = addr_file(a); para = DS_PARA if a[0] == 'DS' else a[1]
    for r in RANGES:
        if r[0] <= f < r[1] and r[2] == para: return r

# names the extracted modules define: references into their ranges that nothing else defines
OWN = {}         # file offset -> [names]
NEG = {}         # file offset of a far segment's start -> [(name, bytes before it)]
OWNED = set()
for n, a in sorted(REFS.items()):
    if a[0] == 'SEG' or n in LIBPUBS or n in OBJPUBS: continue
    if range_of(a): OWN.setdefault(addr_file(a), []).append(n); OWNED.add(n); continue
    # a name for offset 0 of a far segment's frame when the segment starts later in that
    # paragraph (seg048 = 3963:0000; entry 52 starts at 3963:0008, after entry 51 ends there):
    # an equate at the extracted segment's start, k bytes back (the frame is the segment's)
    if a[0] == 'FAR':
        for r in RANGES:
            if r[2] == a[1] and r[3] == 'XFAR' and any(sg[2] == r[0] for sg in FAR) and file_of(a[1], a[2]) < r[0]:
                NEG.setdefault(r[0], []).append((n, r[0] - file_of(a[1], a[2]))); OWNED.add(n)
# references to a segment alone: a label at that segment's start
SEGLABELS = {}   # para -> [names]
for n, a in REFS.items():
    if a[0] == 'SEG' and n not in LIBPUBS | OBJPUBS | OWNED: SEGLABELS.setdefault(a[1], []).append(n)

# renames for the objects: a reference nothing defines by that name, to the name that is there
RENAMES = {}; UNRESOLVED = []; MISNAMED = []
DEFINED = LIBPUBS | OBJPUBS | OWNED | {n for v in SEGLABELS.values() for n in v}
for stem, ob in OBJS.items():
    for n, a in ob['refs'].items():
        if a[0] == 'SEG' and n in DEFINED: continue
        if n in DEFINED and (NAMEADDR.get(n) == a or DEFS.get(a) == n or n not in NAMEADDR and not DEFS.get(a)): continue
        if DEFS.get(a):
            RENAMES.setdefault(stem, {})[n] = DEFS[a]
            if n in DEFINED: MISNAMED.append((stem, n, DEFS[a]))
            continue
        if n in DEFINED and range_of(a):
            # the name is another file's variable; this file means one in an extracted range
            alias = f'{n}_{stem}'; OWN.setdefault(addr_file(a), []).append(alias); OWNED.add(alias); REFS[alias] = a
            RENAMES.setdefault(stem, {})[n] = alias; MISNAMED.append((stem, n, alias)); continue
        if n in DEFINED: UNRESOLVED.append((stem, n, a)); continue
        # no name at all there (inside another file's static data, or one byte into the C
        # library's _ctype): refer to the nearest DGROUP name below it plus the difference
        below = [k for k in DEFS if k[0] == 'DS' and a[0] == 'DS' and k[1] <= a[1]]
        if below:
            k = max(below, key=lambda k: k[1]); RENAMES.setdefault(stem, {})[n] = [DEFS[k], a[1] - k[1]]
        else: UNRESOLVED.append((stem, n, a))

for stem, m in PRE.items():
    for n, alias in m.items():
        RENAMES.setdefault(stem, {})[n] = RENAMES.get(stem, {}).get(alias, alias); MISNAMED.append((stem, n, alias))

NAME_AT = {}
for k, v in DEFS.items(): NAME_AT.setdefault(k, v)
for f, ns in OWN.items():
    for n in ns: NAME_AT.setdefault(REFS[n], n)

# overlay functions the EXE's stubs have no entry for: TLINK gives every public of an overlaid
# segment an entry, so the original had them static. Nothing else may refer to them.
STATICS = {}
REFNAMES = {n for ob in OBJS.values() for n in ob['refs']}
for stem, ob in OBJS.items():
    if ob['base'] < MZEND: continue
    n = int(ob['target'][3:]); para = SEGS[n][1]
    ents = {w16(exe, SEGS[n][2] + e + 2) for e in range(0x20, SEGS[n][3] - SEGS[n][2], 5)}
    for name, off in sorted((nm, off) for off, nm in ODEF.get(n, {}).items()):
        if off not in ents:
            if name in REFNAMES: sys.exit(f'{stem}: {name} has no stub entry in the EXE, but other files call it')
            STATICS.setdefault(stem, []).append(name)
# TLINK numbers a stub's entries in the order the object lists its publics, and Turbo C lists
# them in its symbol table's hash order, which depends on every name in the file. Ours are
# partly provisional, so the order differs; link.py lists them in the EXE's stub order.
STUBORDER = {}
for stem, ob in OBJS.items():
    if ob['base'] < MZEND: continue
    n = int(ob['target'][3:])
    offs = [w16(exe, SEGS[n][2] + e + 2) for e in range(0x20, SEGS[n][3] - SEGS[n][2], 5)]
    STUBORDER[stem] = [ODEF[n][o] for o in offs]

# relocations the EXE has inside an object's code where the object has no fixup: a constant in
# the source where the original had a segment (UW2's VIEW3D stored 0x5DFD three times, which
# the original wrote as a segment, as its relocations show). link.py stops on them.
ADDFIX = {}
for stem, ob in OBJS.items():
    if ob['base'] >= MZEND: continue
    o = ob['o']; code = next(i for i, sg in enumerate(o['segs']) if sg and CFG.is_code_class(sg[1]))
    have = set()
    for f in o['fixups']:
        if f['seg'] == code and f['loc'] == 2: have.add(f['off'])
        if f['seg'] == code and f['loc'] == 3: have.add(f['off'] + 2)
    for loc in sorted(RELOCS):
        if ob['base'] <= loc < ob['base'] + ob['codelen'] and loc - ob['base'] not in have:
            ADDFIX.setdefault(stem, []).append([loc - ob['base'], None, RELOCS[loc]])

# ---- emitting TASM --------------------------------------------------------------------
def fmt_db(bs):
    out = []; i = 0
    while i < len(bs):
        j = i
        while j < len(bs) and bs[j] == 0: j += 1
        if j - i >= 16: out.append(f'        db {j - i} dup (0)'); i = j; continue
        chunk = bs[i:i + 16]
        out.append('        db ' + ','.join('%03Xh' % c for c in chunk)); i += len(chunk)
    return out

class Module:
    def __init__(self, name):
        self.name = name; self.lines = []; self.externs = set(); self.publics = []
    def text(self):
        hdr = [f'; {self.name}: generated by tools/extract.py from UW.EXE. Game bytes: never commit.', '']
        ex = [f'        extrn   {n}:far' for n in sorted(self.externs) if n not in self.publics]
        pub = [f'        public  {n}' for n in self.publics]
        return '\r\n'.join(hdr + ex + pub + self.lines + ['        end', ''])
modules = {}
def module(name):
    if name not in modules: modules[name] = Module(name)
    return modules[name]

FRAME_LABEL = {}         # para -> a label this tool defines at a segment's start
def frame_name(para):
    """A name whose segment's frame is para."""
    for k, v in DEFS.items():
        if k[0] == 'FAR' and k[1] == para: return v
    if para == DS_PARA: return '__psp'          # C0's, in DGROUP
    for n in sorted(OWNED):
        if REFS[n][0] == 'FAR' and REFS[n][1] == para: return n
    if para in FRAME_LABEL: return FRAME_LABEL[para]
    if para in SEGLABELS: return SEGLABELS[para][0]
    sys.exit(f'no name for frame {para:04X}')
for i, para, lo, hi, fl in FAR + [SEGS[i] for i in CODE_GAPS]:
    if hi is not None and hi > lo: FRAME_LABEL.setdefault(para, 'XS%02d' % i)

def labels_at(m, f, para):
    out = []
    for n in OWN.get(f, []):
        a = REFS[n]
        if (a[0] == 'DS') == (para == DS_PARA) and n not in m.publics:
            out.append(f'{n} label byte'); m.publics.append(n)
    return out

# --mod: near code offsets held as plain words in the extracted bytes (tables of handlers the
# assembly jumps through). Each one the IDA listing types, and the rest of its table, becomes
# `dw offset NAME` (Exhume's tools/modding.py), so that it follows that code if the module
# holding it changes size. In UW2 that was the model interpreter's opcode table in extracted
# far data, and so is UW1's: seg051:2738 in the extracted head of FD58, which the listing
# types as `dw offset seg004_...` (UW1's listing names its segments without a paragraph:
# exhume.toml's [binary] listing_segments; docs/LAYOUT.md). The exact link keeps the bytes.
NEAR = {}
if MOD:
    NEAR = modding.code_offset_tables(
        modding.listing_code_offsets(CFG, exe), exe,
        inside=lambda f: any(r[0] <= f < r[1] - 1 for r in RANGES),
        name_at=lambda para, off: NAME_AT.get(('FAR', para, off)))
    if not CFG.listing or not os.path.exists(CFG.listing): print('no IDA listing: near code offsets in the extracted data stay numbers')
    print(f'--mod: {len(NEAR)} near code offsets in the extracted data written as names')

def body(m, lo, hi, para, code):
    """Bytes lo..hi of the EXE as db lines, with labels for the names defined there and a fixup
    at every relocation. para: the frame of the segment the range sits in."""
    out = []; i = lo; pend = bytearray()
    def flush():
        nonlocal pend
        out.extend(fmt_db(bytes(pend))); pend = bytearray()
    while i < hi:
        if i in NEG and i == lo:
            for n, k in NEG[i]:
                if n not in m.publics: out.append(f'{n} equ $+{0x10000 - k:05X}h'); m.publics.append(n)
        if i in OWN:
            flush(); out += labels_at(m, i, para)
        if i in NEAR and i + 1 not in OWN:
            flush(); m.externs.add(NEAR[i]); out.append(f'        dw offset {NEAR[i]}'); i += 2; continue
        if i + 2 in RELOCS and i + 2 < hi and i not in RELOCS and i + 1 not in OWN and i + 2 not in OWN and i + 3 not in OWN:
            off = w16(exe, i); seg = RELOCS[i + 2]
            ok_ptr = (not code) or (i >= lo + 1 and exe[i - 1] in (0x9A, 0xEA))
            n = NAME_AT.get(('DS', off) if seg == DS_PARA else ('FAR', seg, off))
            if n and ok_ptr:
                flush(); m.externs.add(n); out.append(f'        dd {n}'); i += 4; continue
        if i in RELOCS:
            flush(); n = frame_name(RELOCS[i]); m.externs.add(n)
            out.append(f'        dw seg {n}'); i += 2; continue
        pend.append(exe[i]); i += 1
    flush()
    return out

def seg_start_labels(m, para, idx):
    out = []
    names = list(SEGLABELS.get(para, []))
    if FRAME_LABEL.get(para) == 'XS%02d' % idx: names.append('XS%02d' % idx)
    for n in names:
        if n not in m.publics: out.append(f'{n} label byte'); m.publics.append(n)
    return out

# XORDER: the code segments before C0's _TEXT
m = module('XORDER')
m.lines += ['; The code segments that sit before C0\'s _TEXT, in EXE order and with their EXE',
            '; alignment, so TLINK places them first. No bytes. seg003 and seg004 are referred to by',
            '; segment alone from other modules, so their names are defined at their starts.']
# A segment's name is whatever its object calls it (TLINK joins segments by name, and the EXE
# keeps no names): FILE_TEXT for a C file or an assembly module written with .model and
# .code, which TASM names after the file the same way, and the name in its SEGMENT directive
# for the others.
# Each takes its object's class as well: TLINK orders segments in blocks by class, in the
# order it first sees each class, and sets a segment's code flag in the overlay manager's
# segment table only when its class ends in upper-case CODE (profiles/borland-tc101/linker.md,
# "Segment classes"). seg000..seg002 are class ASMCODE, seg003 and seg004 class code, and
# XEMPTY05, C0's _TEXT and everything after them CODE, so three blocks in that order.
def codeseg(k): return OBJS[k]['codeseg']
def codecls(k): return OBJS[k]['codecls']
for k, align, idx in ((S('seg000'), 'byte', 0), (S('seg001'), 'word', 1), (S('seg002'), 'word', 2),
                      (M('seg003', 'A')[0], 'para', 3), (M('seg004', 'A')[0], 'para', 4)):
    name = codeseg(k)
    m.lines.append(f'{name} segment {align} public \'{codecls(k)}\'')
    if idx in (3, 4): m.lines += seg_start_labels(m, SEGS[idx][1], idx)
    m.lines.append(f'{name} ends')
m.lines += ['; segment table entry 5: an empty code segment on a paragraph, just before _TEXT',
            'XEMPTY05 segment para public \'CODE\'', 'XEMPTY05 ends']

for i, (mname, segname) in CODE_GAPS.items():
    m = module(mname)
    for a, b in GAPS.get(i, []):
        m.lines.append(f'{segname} segment byte public \'CODE\'')
        if a == SEGS[i][2]: m.lines += seg_start_labels(m, SEGS[i][1], i)
        m.lines += body(m, a, b, SEGS[i][1], True)
        m.lines.append(f'{segname} ends')
m = module('XSEG019')
m.lines += ['; seg019 (entry 21) and seg020 (entry 22) start on paragraphs, and entry 23 is an empty',
            '; byte-aligned code segment between seg020 and seg021: declare them here, after seg018',
            '; and before their objects.']
for name, cls, align in ((codeseg(M('seg019', 'A')[0]), codecls(M('seg019', 'A')[0]), 'para'),
                         (codeseg(S('seg020')), codecls(S('seg020')), 'para'), ('XEMPTY23', 'CODE', 'byte')):
    m.lines += [f'{name} segment {align} public \'{cls}\'', f'{name} ends']
m = module('XEMPTY16')
m.lines += ['; segment table entry 16: an empty byte-aligned code segment between seg014 and seg015',
            'XEMPTY16 segment byte public \'CODE\'', 'XEMPTY16 ends']
for k, (i, a, b) in TAILS.items():
    m = module('XT' + sfx(k))
    m.lines.append(f'{OBJS[k]["codeseg"]} segment byte public \'{OBJS[k]["codecls"]}\'')
    m.lines += body(m, a, b, SEGS[i][1], True)
    m.lines.append(f'{OBJS[k]["codeseg"]} ends')
    RES.insert(RES.index(k) + 1, 'XT' + sfx(k))

# far data
ALIGN = {}
prev_end = SEGS[E_FARDATA][2]
for i, para, lo, hi, fl in FAR:
    if hi is None or hi < lo: hi = lo
    if lo == prev_end: ALIGN[i] = 'byte'
    elif lo % 2 == 0 and lo - prev_end == 1: ALIGN[i] = 'word'
    elif lo % 16 == 0: ALIGN[i] = 'para'
    else: sys.exit(f'far segment {i}: cannot place at {lo:X} after {prev_end:X}')
    prev_end = max(prev_end, hi)
# the C files that define far data must be linked in the order of their entries
_link = ['C0UW1'] + RES + OVL
_owned = sorted(FAROWN)
for a, b in zip(_owned, _owned[1:]):
    if _link.index(FAROWN[a]) > _link.index(FAROWN[b]):
        sys.exit(f'far data: entry {a} is {FAROWN[a]}\'s and entry {b} {FAROWN[b]}\'s, but {FAROWN[b]} is linked first')
# entries before a resident C file's far data that no C file defines: declared, empty, just
# before it, so that TLINK sees them first (those before an overlay's are declared in XFAR,
# which comes after every resident object)
for i, para, lo, hi, fl in FAR:
    if i in FAROWN: continue
    nxt = next((j for j in _owned if j > i), None)
    if nxt is None or FAROWN[nxt] not in RES: continue
    k = FAROWN[nxt]; name = 'XF' + sfx(k)
    if name not in modules:
        module(name).lines.append(f'; the far data segments before {k}\'s, declared so that TLINK places them first')
        RES.insert(RES.index(k), name)
    modules[name].lines += [f'FD{i:02d} segment {ALIGN[i]} public \'FAR_DATA\'', f'FD{i:02d} ends']
m = module('XFAR')
for i, para, lo, hi, fl in FAR:
    if hi is None or hi < lo: hi = lo
    align = ALIGN[i]
    if i in FAROWN:
        m.lines.append(f'; entry {i} is {FAROWN[i]}\'s own far segment'); continue
    m.lines.append(f'FD{i:02d} segment {align} public \'FAR_DATA\'')
    if i in FARSRC: m.lines.append(f'; {FARSRC[i]} fills this segment')
    else:
        if hi > lo: m.lines += seg_start_labels(m, para, i)
        if i in FARPIECES: m.lines.append(f'; the rest of this segment comes from {", ".join(k for _, _, k in FARPIECES[i])}')
        m.lines += body(m, lo, FARCUT.get(i, hi), para, False)
    m.lines.append(f'FD{i:02d} ends')
# the far data sources follow XFAR, which declares their segments in the EXE's order
RES[RES.index('XFAR') + 1:RES.index('XFAR') + 1] = sorted(FAROBJS)

# DGROUP gaps
def dgroup_module(name, dgaps, bgaps):
    m = module(name)
    for a, b in dgaps:
        m.lines += ['_DATA segment byte public \'DATA\'']
        m.lines += body(m, DS_FILE + a, DS_FILE + b, DS_PARA, False)
        m.lines += ['_DATA ends']
    for a, b in bgaps:
        at = a
        m.lines += ['_BSS segment byte public \'BSS\'']
        for f in sorted(x for x in OWN if DS_FILE + a <= x < DS_FILE + b):
            if f - DS_FILE > at: m.lines.append(f'        db {f - DS_FILE - at} dup (?)')
            m.lines += labels_at(m, f, DS_PARA); at = f - DS_FILE
        if b > at: m.lines.append(f'        db {b - at} dup (?)')
        m.lines += ['_BSS ends']
    m.lines += ['_DATA segment byte public \'DATA\'', '_DATA ends', '_BSS segment byte public \'BSS\'', '_BSS ends',
                'DGROUP group _DATA, _BSS']
slots = {}
for a, b, slot in DPIECES: slots.setdefault(slot, ([], []))[0].append((a, b))
for a, b, slot in BPIECES: slots.setdefault(slot, ([], []))[1].append((a, b))
after = {}; before = {}
for slot, (dg, bg) in slots.items():
    dgroup_module(dname(slot), dg, bg)
    (after if slot[0] == 'after' else before)[slot[1]] = dname(slot)
if LIBGAPS: dgroup_module('XLIBD', LIBGAPS, [])

# empty overlays
for n in OVLNUMS:
    k = 'XOVR%03d' % n
    if k in OVL:
        module(k).lines += [f'; ovr{n:03d}: its stub has no entries and its overlay no code',
                            f'OVR{n:03d}_TEXT segment byte public \'CODE\'', f'OVR{n:03d}_TEXT ends']

for k, rs in LIBRANGES.items():
    m = module('XP' + sfx(k))
    m.lines.append(f'; {k}\'s far data, from its first relocated pointer: a library module of its own')
    for n, (i, a, b) in enumerate(rs):
        m.lines.append(f'FD{i:02d} segment byte public \'FAR_DATA\'')
        if n == 0: m.lines.append(f'{m.name} label byte'); m.publics.append(m.name)
        m.lines += body(m, a, b, SEGS[i][1], False)
        m.lines.append(f'FD{i:02d} ends')

# Library modules no linked module names: TLINK takes a library module only for a reference to
# one of its publics, so the original link referred to these from somewhere no source shows
# yet. XPULL, an otherwise empty module linked after XFAR, names one public of each (UW1:
# QUADFIT, the quadratic fit no caller was found for). It adds no bytes and no relocations.
_named = {n for ob in OBJS.values() for n in ob['o']['ext'] if n} | {n for mm in modules.values() for n in mm.externs}
_named |= {n for k, ob in OBJS.items() if k not in LATE for n in ob['o']['pubs']}
PULL = [k for k in LATE if k in OBJS and not (set(OBJS[k]['o']['pubs']) & _named)]
if PULL or LIBRANGES:
    m = module('XPULL')
    m.lines.append('; library modules nothing else names, so that TLINK takes them from the library')
    for k in PULL:
        n = sorted(OBJS[k]['o']['pubs'])[0]
        m.externs.add(n); m.lines.append(f'; {k}')
    for k in LIBRANGES:
        m.externs.add('XP' + sfx(k)); m.lines.append(f'; XP{sfx(k)}, {k}\'s far data')
    RES.insert(RES.index('XFAR') + 1, 'XPULL')

# ---- write ----------------------------------------------------------------------------
os.makedirs(OUT, exist_ok=True)
for f in glob.glob(os.path.join(OUT, 'X*.ASM')): os.remove(f)
for name, m in modules.items():
    open(os.path.join(OUT, name + '.ASM'), 'w', newline='').write(m.text())
def expand(seq):
    r = []
    for k in seq:
        if k in before: r.append(before[k])
        r.append(k)
        if k in after: r.append(after[k])
    return r
manifest = dict(
    resident=['XORDER'] + expand(['C0UW1'] + RES),
    overlays=expand(OVL),
    late=[k for k in LATE if k in OBJS or k in modules] + (['XLIBD'] if LIBGAPS else []),
    bytealigned=ODD,
    statics=STATICS,
    stuborder=STUBORDER,
    retarget=RETARGET,
    addfix=ADDFIX,
    objects={**{k: o['obj'] for k, o in OBJS.items()},
             **{k: p for k, p in FAROBJS.items()}},
    generated=sorted(modules))
for fx in ADDFIX.values():
    for e in fx: e[1] = frame_name(e[2])
json.dump(manifest, open(os.path.join(OUT, 'manifest.json'), 'w'), indent=1)
if not MOD and ALL_VERIFIED:
    # the matched objects, the sources they came from and where they sit, for --mod
    modding.write_snapshot(CFG, BASE, {
        **{stem: (ob['obj'], ob['src'], dict(data=ob['data'], bss=ob['bss'], refs=ob['refs'], far=ob['far']))
           for stem, ob in OBJS.items()},
        **{stem: (p, FARPATHS[stem], None) for stem, p in FAROBJS.items()}})
json.dump(RENAMES, open(os.path.join(OUT, 'renames.json'), 'w'), indent=1, sort_keys=True)
print(f'{len(modules)} modules in {OUT}: {len(DPIECES)} _DATA gaps, {len(BPIECES)} _BSS gaps, '
      f'{len(OWNED)} names defined, {sum(map(len, RENAMES.values()))} renames')
# what the link has to work around in the sources (link.py applies these to object copies)
for stem, n, y in MISNAMED:
    print(f'misnamed: {stem} refers to {n}, but the EXE has {y} there')
for stem, rs in RETARGET.items():
    print(f'two variables: {stem} uses {rs[0][1]} for {rs[0][2]} too ({len(rs)} fixups retargeted)')
for stem in ODD: print(f'odd data: {stem}\'s data starts at an odd address; its copy is made byte-aligned')
for stem, ns in STATICS.items(): print(f'static: {stem} {" ".join(ns)} have no overlay stub entry; dropped from its publics')
for stem, fx in ADDFIX.items():
    print(f'constant segment: {stem} has {len(fx)} segment constant(s) where the EXE has relocations (+{", +".join("%X" % f[0] for f in fx)})')
print(f'renamed: {sum(map(len, RENAMES.values()))} references in {len(RENAMES)} objects (IDA names for library routines, '
      f'names cut to 32 characters, names that differ between files)')
for stem, n, a in UNRESOLVED:
    print(f'unresolved: {stem} refers to {n} at {a}')
