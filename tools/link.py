"""Link UW2.EXE from the matched objects with Turbo Link 3.01, headless in DOS, and compare.

    python3 tools/link.py [--no-extract] [--out DIR] [--obj STEM=PATH ...]

--obj links another build of one object in place of build/STEM/STEM.OBJ (to try a changed
source without disturbing the matched build).

1. tools/extract.py writes the data-only modules, manifest.json and renames.json under
   build/LINK (skip with --no-extract when they are current).
2. C0UW2.ASM is Turbo C++'s own TC/C0.ASM with the changes UW2's startup code shows (see
   c0_source), assembled as BUILD-C0.BAT does for the medium model.
3. The objects are copied to build/LINK/obj, changed only in what they tell the linker where
   the sources and the EXE disagree (see patch_object and extract.py's report): extern
   renames, alignments, publics UW2 had static, the order of overlay publics, and three
   missing segment fixups.
4. In DOS: the date is set to 12 May 1993 (TLINK records it), TASM assembles the generated
   modules, TLIB puts seg003, seg004, seg045 and the second library's data into UWLIB.LIB,
   and TLINK links from LINK.RSP:

     TLINK /c /m /s XORDER C0UW2 <resident objects> /o <overlay objects> /o-,
           uwedit.exe, uwedit.map, CM.LIB UWLIB.LIB OVERLAY.LIB

   /c is case-sensitive (TCC -Y passes /c/x), /o overlays the modules that follow, /m /s
   write the map. The name is uwedit.exe because TLINK stores it in the EXE (__EXENAME__)
   and UW2.EXE has that. UW2 uses no floating point, so EMU.LIB and MATHM.LIB (which TCC
   would add) contribute nothing; the C library's modules come before the second library's
   and the overlay manager's in UW2.EXE, hence the library order.
5. The EXE and map land in build/LINK/out (or --out) as UW2.EXE and UW2.MAP, and
   tools/exediff.py compares the EXE with yours.
"""
import sys, os, json, subprocess, shutil, re
here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
LINKDIR = os.path.join(root, 'build', 'LINK')

def c0_source():
    """TC/C0.ASM with UW2's differences. UW2's C0 (seg005 up to offset 0x249) is the same code
    except that (1) the self-modifying patch of the exit-table scan comes after main returns,
    not before main is called, and (2) the null-pointer checksum starts at DATASEG@ (DS:4 in
    UW2, where _DATA begins) through `mov ax,offset DATASEG@; mov si,ax`, instead of at 0.
    And (3) its _FARDATA is word-aligned: segment table entry 49 is an empty segment at 3704:4,
    the first FAR_DATA segment, right after _OVRTEXT_ ends at an odd address."""
    tc = os.environ.get('UW2DECOMP_TC', os.path.join(root, 'TC'))
    s = open(os.path.join(tc, 'C0.ASM'), encoding='latin1').read().replace('\r\n', '\n')
    def sub(old, new):
        nonlocal s
        if s.count(old) != 1: sys.exit('C0.ASM is not the Turbo C++ 1.01 one: cannot find\n' + old)
        s = s.replace(old, new)
    patch = ("                mov     byte ptr cs:JA_JB,72h\n"
             "                mov     byte ptr cs:StartExit+1,0\n")
    sub(patch, '')
    sub("                call    _main\n", "                call    _main\n" + patch)
    sub("                xor     ax, ax\n                mov     si, ax\n                mov     cx, lgth_CopyRight\n",
        "                mov     ax, offset DGROUP:DATASEG@\n                mov     si, ax\n"
        "                xor     ax, ax\n                mov     cx, lgth_CopyRight\n")
    sub("_FARDATA\tSEGMENT PARA PUBLIC 'FAR_DATA'", "_FARDATA\tSEGMENT WORD PUBLIC 'FAR_DATA'")
    return s.replace('\n', '\r\n')

# ---- patching copies of objects ------------------------------------------------------
def _records(d):
    p = 0; out = []
    while p + 3 <= len(d):
        t = d[p]; n = d[p + 1] | d[p + 2] << 8
        out.append([t, bytearray(d[p + 3:p + 2 + n])]); p += 3 + n
        if t in (0x8A, 0x8B): break
    return out

def _idx(b, i):
    return ((b[i] & 0x7F) << 8 | b[i + 1], i + 2) if b[i] & 0x80 else (b[i], i + 1)

def patch_object(d, renames, align=None, statics=(), order=None, retarget=(), addfix=()):
    """A copy of object d, changed only in what it tells the linker:
    renames:  extern names to rename; a value [name, delta] also moves every reference by delta
    align:    {class name: new alignment (1 byte, 2 word, 3 para)} for its segments
    statics:  publics to drop, as if the source had said static
    order:    the code segment's publics in the order TLINK should number their overlay stub
              entries (it numbers them from the last listed to the first)
    retarget: [code offset, old name, new name]: fixups at those offsets refer to another name
    addfix:   [code offset, name]: a segment fixup to name's segment where the code has a
              constant (the constant is cleared, since TLINK adds the segment to it)"""
    recs = _records(d); ext = [None]; delta = {}; lnames = [None]; segs = [None]
    for r in recs:
        t, b = r
        if t == 0x96:
            i = 0
            while i < len(b): lnames.append(b[i + 1:i + 1 + b[i]].decode('latin1')); i += 1 + b[i]
        elif t == 0x8C:
            i = 0; nb = bytearray()
            while i < len(b):
                l = b[i]; name = b[i + 1:i + 1 + l].decode('latin1'); i += 1 + l
                ti, j = _idx(b, i); tb = b[i:j]; i = j
                new = renames.get(name, name)
                if isinstance(new, list): delta[len(ext)] = new[1]; new = new[0]
                ext.append(new); nb += bytes([len(new)]) + new.encode('latin1') + tb
            r[1] = nb
        elif t in (0x90, 0x91) and statics:
            gi, i = _idx(b, 0); si, i = _idx(b, i)
            if si == 0: i += 2
            keep = bytearray(b[:i])
            while i < len(b):
                l = b[i]; name = b[i + 1:i + 1 + l].decode('latin1'); j = i + 1 + l + 2
                _, j = _idx(b, j)
                if name not in statics: keep += b[i:j]
                i = j
            r[1] = keep
        elif t in (0x98, 0x99):
            j = 1 + (3 if (b[0] >> 5) == 0 else 0) + 2
            sn, j = _idx(b, j); cn, j = _idx(b, j)
            segs.append(lnames[cn])
            if align and lnames[cn] in align: b[0] = (b[0] & 0x1F) | (align[lnames[cn]] << 5)
    code = segs.index('CODE')
    # new externs: appended in a record of their own after the last EXTDEF
    newext = []
    for _, old, new in retarget:
        if new not in ext and new not in newext: newext.append(new)
    for _, name in addfix:
        if name not in ext and name not in newext: newext.append(name)
    if newext:
        last = max(k for k, r in enumerate(recs) if r[0] == 0x8C)
        recs.insert(last + 1, [0x8C, bytearray(b''.join(bytes([len(n)]) + n.encode('latin1') + b'\0' for n in newext))])
        ext += newext
    def enc(ix): return bytes([ix]) if ix < 0x80 else bytes([0x80 | ix >> 8, ix & 0xFF])
    retgt = {off: ext.index(new) for off, old, new in retarget}
    # walk LEDATA and FIXUPP records together
    tthr = [None] * 4; last = None; out_recs = []
    pending = {}                       # added fixups by LEDATA record id
    for off, name in addfix:
        for r in recs:
            if r[0] == 0xA0:
                si, j = _idx(r[1], 0); lo = r[1][j] | r[1][j + 1] << 8; n = len(r[1]) - j - 2
                if si == code and lo <= off < lo + n:
                    at = j + 2 + off - lo; r[1][at] = r[1][at + 1] = 0
                    o = off - lo
                    pending.setdefault(id(r), []).append(bytes([0xC0 | 2 << 2 | o >> 8, o & 0xFF, 0x56]) + enc(ext.index(name)))
                    break
        else: sys.exit(f'no data record holds offset {off:X}')
    for k, r in enumerate(recs):
        t, b = r
        out_recs.append(r)
        if t in (0xA0, 0xA1):
            last = r
            if id(r) in pending and not (k + 1 < len(recs) and recs[k + 1][0] == 0x9C):
                out_recs.append([0x9C, bytearray(b''.join(pending.pop(id(r))))])
            continue
        if t != 0x9C: continue
        nb = bytearray(); i = 0
        si, j = _idx(last[1], 0); ledata_off = last[1][j] | last[1][j + 1] << 8
        while i < len(b):
            c = b[i]
            if not c & 0x80:                    # THREAD
                meth = (c >> 2) & 7; num = c & 3; s0 = i; i += 1; ix = None
                if meth < 3: ix, i = _idx(b, i)
                if not c & 0x40: tthr[num] = (meth & 3, ix)
                nb += b[s0:i]; continue
            s0 = i; loc_off = ((c & 3) << 8) | b[i + 1]; i += 2
            fd = b[i]; i += 1
            fr = b''
            if not fd & 0x80 and ((fd >> 4) & 7) < 3: fs = i; _, i = _idx(b, i); fr = b[fs:i]
            if fd & 0x08: tm, ti = tthr[fd & 3]; tbytes = b''
            else: tm = fd & 3; ts = i; ti, i = _idx(b, i); tbytes = b[ts:i]
            disp = b''
            if not fd & 0x04: disp = b[i:i + 2]; i += 2
            if tm == 2 and ti in delta:
                if disp:
                    v = (disp[0] | disp[1] << 8) + delta[ti]; disp = bytes([v & 0xFF, (v >> 8) & 0xFF])
                else:                           # the displacement is in the data at the location
                    lb = last[1]; at = j + 2 + loc_off
                    v = (lb[at] | lb[at + 1] << 8) + delta[ti]; lb[at] = v & 0xFF; lb[at + 1] = (v >> 8) & 0xFF
            if si == code and tm == 2 and ledata_off + loc_off in retgt:
                # explicit target: EXTDEF, keeping the frame and the displacement form
                ti = retgt[ledata_off + loc_off]; tbytes = enc(ti); fd = (fd & 0xF4) | 2
            nb += b[s0:s0 + 2] + bytes([fd]) + fr + tbytes + disp
        if id(last) in pending: nb += b''.join(pending.pop(id(last)))
        r[1] = nb
    if pending: sys.exit('added fixups were not placed')
    recs = out_recs
    if order:
        rank = {n: k for k, n in enumerate(order)}; first = None; ents = []; hdr = None; kept = []
        for r in recs:
            t, b = r
            if t in (0x90, 0x91):
                gi, i = _idx(b, 0); si, i = _idx(b, i)
                names = []; j = i
                while j < len(b):
                    l = b[j]; nm = b[j + 1:j + 1 + l].decode('latin1'); k = j + 1 + l + 2; _, k = _idx(b, k)
                    names.append((nm, b[j:k])); j = k
                if names and all(nm in rank for nm, _ in names):
                    ents += names; hdr = b[:i]
                    if first is None: first = len(kept); kept.append(None)
                    continue
            kept.append(r)
        if first is not None:
            ents.sort(key=lambda e: -rank[e[0]])      # TLINK numbers them last to first
            kept[first:first + 1] = [[0x90, bytearray(hdr) + b''.join(raw for _, raw in ents[k:k + 32])]
                                     for k in range(0, len(ents), 32)]
            recs = kept
    out = bytearray()
    for t, b in recs:
        rec = bytes([t, (len(b) + 1) & 0xFF, (len(b) + 1) >> 8]) + bytes(b)
        out += rec + bytes([(-sum(rec)) & 0xFF])
    return bytes(out)

# seg020 starts at an odd address in the EXE, so its segment was byte-aligned; SEG020.ASM's
# `.model medium` makes it word-aligned, which TLINK honours even after XSEG018 has declared
# the segment byte-aligned. The linked copy of its object is made byte-aligned. Likewise the
# objects whose _DATA verify.py places at an odd address (extract.py lists them as
# `bytealigned`): Turbo C always makes _DATA word-aligned, so in the original those bytes
# were preceded by data of the same file that our source does not have.
ALIGN = {'SEG020': {'CODE': 1}}

def main():
    a = sys.argv[1:]
    out = os.path.join(LINKDIR, 'out')
    if '--out' in a: out = a[a.index('--out') + 1]
    if '--no-extract' not in a:
        subprocess.run([sys.executable, os.path.join(here, 'extract.py')], check=True)
    man = json.load(open(os.path.join(LINKDIR, 'manifest.json')))
    open(os.path.join(LINKDIR, 'C0UW2.ASM'), 'w', newline='').write(c0_source())
    files = []; batch = []
    for g in man['generated']:
        files.append(os.path.join(LINKDIR, g + '.ASM'))
        batch.append(f'TASM /ml {g}')
    # TLINK stores the link date in the overlay data (__EXEDATE__): UW2's is 12 May 1993
    open(os.path.join(LINKDIR, 'SETDATE.ASM'), 'w', newline='').write('\r\n'.join([
        '; sets the DOS date for TLINK: 12 May 1993, as UW2.EXE records', '        .model tiny', '        .code',
        '        org 100h', 'start:  mov ah,2Bh', '        mov cx,1993', '        mov dx,050Ch', '        int 21h',
        '        mov ax,4C00h', '        int 21h', '        end start', '']))
    files.append(os.path.join(LINKDIR, 'SETDATE.ASM'))
    batch += ['TASM SETDATE', 'TLINK /t SETDATE', 'SETDATE']
    files.append(os.path.join(LINKDIR, 'C0UW2.ASM'))
    batch.append('TASM /D__MEDIUM__ /MX C0UW2')
    renames = json.load(open(os.path.join(LINKDIR, 'renames.json')))
    override = dict(x.split('=', 1) for k, x in enumerate(a) if k and a[k - 1] == '--obj')
    objdir = os.path.join(LINKDIR, 'obj'); os.makedirs(objdir, exist_ok=True)
    for k, p in man['objects'].items():
        if k in man['resident'] or k in man['overlays'] or k in man['late']:
            d = open(override.get(k, os.path.join(root, p)), 'rb').read()
            align = dict(ALIGN.get(k, {}))
            if k in man['bytealigned']: align.update({'DATA': 1, 'BSS': 1})
            if k in renames or align or any(k in man[x] for x in ('statics', 'stuborder', 'retarget', 'addfix')):
                d = patch_object(d, renames.get(k, {}), align or None, man['statics'].get(k, ()),
                                 man['stuborder'].get(k), man['retarget'].get(k, ()),
                                 [(o, n) for o, n, _ in man['addfix'].get(k, ())])
            open(os.path.join(objdir, k + '.OBJ'), 'wb').write(d)
            files.append(os.path.join(objdir, k + '.OBJ'))
    batch.append('TLIB UWLIB ' + ' '.join('+' + k for k in man['late']))
    objs = man['resident'] + ['/o'] + man['overlays'] + ['/o-']
    lines = []
    for i in range(0, len(objs), 8):
        lines.append(' '.join(objs[i:i + 8]) + (' +' if i + 8 < len(objs) else ''))
    # the EXE's own name is stored too (__EXENAME__): uwedit.exe, in lower case
    rsp = '/c /m /s ' + '\r\n'.join(lines) + '\r\nuwedit.exe\r\nuwedit.map\r\nCM.LIB UWLIB.LIB OVERLAY.LIB\r\n'
    open(os.path.join(LINKDIR, 'LINK.RSP'), 'w', newline='').write(rsp)
    files.append(os.path.join(LINKDIR, 'LINK.RSP'))
    batch.append('TLINK @LINK.RSP')
    cmd = ['node', os.path.join(here, 'dosrun.mjs'), out, '-t', '600']
    for f in files: cmd += ['-f', f]
    for b in batch: cmd += ['-c', b]
    cmd += ['-o', 'UWEDIT.EXE', '-o', 'UWEDIT.MAP']
    r = subprocess.run(cmd)
    for x in ('EXE', 'MAP'):
        if os.path.exists(os.path.join(out, 'UWEDIT.' + x)):
            os.replace(os.path.join(out, 'UWEDIT.' + x), os.path.join(out, 'UW2.' + x))
    log = open(os.path.join(out, 'RUN.LOG'), encoding='latin1').read()
    errs = [l for l in log.splitlines() if re.search(r'Error|Fatal|Undefined|Warning', l) and not re.search(r'messages: +None', l)]
    print('\n'.join(errs[:60]))
    if len(errs) > 60: print(f'... {len(errs) - 60} more')
    if r.returncode: sys.exit(r.returncode)
    sys.exit(subprocess.run([sys.executable, os.path.join(here, 'exediff.py'), os.path.join(out, 'UW2.EXE')]).returncode)

if __name__ == '__main__':
    main()
