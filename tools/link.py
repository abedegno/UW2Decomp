"""Link UW2.EXE from the matched objects with Turbo Link 3.01, headless in DOS, and compare.

    python3 tools/link.py [--no-extract] [--out DIR] [--obj STEM=PATH ...]

--obj links another build of one object in place of build/STEM/STEM.OBJ (to try a changed
source without disturbing the matched build).

1. tools/extract.py writes the data-only modules, manifest.json and renames.json under
   build/LINK (skip with --no-extract when they are current).
2. C0UW2.ASM is Turbo C++'s own TC/C0.ASM with the changes UW2's startup code shows (see
   c0_source), assembled as BUILD-C0.BAT does for the medium model.
3. The objects are copied to build/LINK/obj; an overlay's copy changes only in the order it
   lists its publics (see patch_object). Nothing else is adjusted: a source whose names,
   statics, alignment or segment references disagree with the EXE (extract.py reports them)
   stops the link until it is corrected.
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

def patch_object(d, order):
    """A copy of object d with its code segment's publics listed in the order TLINK should
    number their overlay stub entries (it numbers them from the last listed to the first).

    Turbo C lists a file's publics in descending order of the key tools/bssorder.py computes
    from each name, names with equal keys in the reverse of the order they were first seen;
    so the EXE's stub order is a constraint on the names. Where it is not met the file has
    provisional names whose keys sort differently from the original ones (ties can be put
    right with a prototype; see OVR138.C), and only renaming those functions to names chosen
    for their keys would make the copy unnecessary."""
    recs = _records(d)
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

# what extract.py reports that the link used to patch into copies of the objects, and now
# leaves to the sources: each is a defect in one
SOURCE_DEFECTS = {'bytealigned': 'its _DATA or _BSS starts at an odd address: a leading item of its data is missing',
                  'statics': 'publics with no overlay stub entry: UW2 had them static',
                  'retarget': 'one name used for two variables',
                  'addfix': 'a constant where UW2 has a segment relocation'}

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
    bad = [f'{k}: refers to {n} where the EXE has {v}' for k, rs in renames.items() for n, v in rs.items()]
    for x, why in SOURCE_DEFECTS.items():
        bad += [f'{k}: {why}' for k in man.get(x) or ()]      # a list or a dict by object
    if bad: sys.exit('sources to correct before linking:\n  ' + '\n  '.join(bad))
    override = dict(x.split('=', 1) for k, x in enumerate(a) if k and a[k - 1] == '--obj')
    objdir = os.path.join(LINKDIR, 'obj'); os.makedirs(objdir, exist_ok=True)
    for k, p in man['objects'].items():
        if k in man['resident'] or k in man['overlays'] or k in man['late']:
            d = open(override.get(k, os.path.join(root, p)), 'rb').read()
            if man['stuborder'].get(k): d = patch_object(d, man['stuborder'][k])
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
