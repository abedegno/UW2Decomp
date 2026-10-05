"""Link UW.EXE from the matched objects with Turbo Link 3.01, headless in DOS, and compare.

    python3 tools/link.py [--config PATH] [--no-extract] [--out DIR] [--obj STEM=PATH ...]
    python3 tools/link.py --mod [--config PATH] [--out DIR] [--obj STEM=PATH ...] [--add STEM=PATH ...]

Adapted from Exhume's examples/uw2/link.py (UW2Decomp's). The general parts (the date and
name TLINK stores, the response file, library order, checking public order against overlay
stubs) are described in Exhume's docs/link.md and profiles/borland-tc101/linker.md; the
facts here are UW.EXE's, measured (docs/LINKING.md). Exhume's tools are found by
tools/exhume.py.

--obj links another build of one object in place of <build>/STEM/STEM.OBJ (to try a changed
source without disturbing the matched build).

--add (with --mod only) links one more resident module that no source in the matched build
has, after the last resident code module.

--mod is the modding build (Exhume's docs/link.md, "The modding build"): sources may change
by any size. The layout comes from the last exact run (<build>/LINK/base, written by
extract.py when every object verified), the sources whose text differs from that run's are
compiled into <build>/MODLINK/src (the matched objects in <build> are left alone; Exhume's
tools/modding.py), and the EXE goes to <build>/MODLINK/out. Nothing is compared with your
EXE, and an overlay's publics may come in any order (TLINK then numbers its stub entries
differently, which nothing depends on: every call to them is a fixup). The paragraph padding
after each overlay is set to 0, as UW.EXE and the exact link have it (clear_overlay_padding).
With no source changed it gives the exact link's EXE.

1. tools/extract.py writes the data-only modules, manifest.json and renames.json under
   build/LINK (skip with --no-extract when they are current).
2. C0UW1.ASM is Turbo C++'s own TC/C0.ASM with the three changes UW1's startup code shows,
   the same as UW2's (see c0_source), assembled as BUILD-C0.BAT does for the medium model.
3. The objects are copied to build/LINK/obj as they are. Nothing is adjusted: a source whose
   names, statics, alignment or segment references disagree with the EXE (extract.py reports
   them), or an overlay whose publics are listed out of the EXE's stub order (see
   stub_order_wrong), stops the link until it is corrected.
4. In DOS (Exhume's tools/dosrun.mjs, in the DOS tools/dosbackend.mjs picks; with emu2 the
   date needs tools/emu2-date.patch): the date is set to 12 May 1993 (TLINK records it),
   TASM assembles the generated modules, TLIB puts the second library's modules (seg019's
   but its first, seg004's, seg003's and seg045) into UWLIB.LIB in the manifest's order,
   and TLINK links from LINK.RSP:

     TLINK /c /m /s /i XORDER C0UW1 <resident objects> /o <overlay objects> /o-,
           uwedit.exe, uwedit.map, CM.LIB UWLIB.LIB OVERLAY.LIB

   /c is case-sensitive (TCC -Y passes /c/x), /o overlays the modules that follow, /m /s
   write the map. /i ("initialize all segments") makes TLINK write every byte of the image
   it lays out, zero where no record gives one. Without it, the bytes no record covers
   (alignment padding between modules' _DATA, and data a module leaves uninitialised, such
   as part of CM.LIB's SETARGV) come from TLINK's buffers as it left them, and hold stale
   bytes of an object it read earlier; which ones, and whether any, changed with the
   emulator's memory size and with any change to the module list (16 to 40 bytes in
   DGROUP, tested under emu2 with 512 and 640 KB). With /i they are 0, as in UW.EXE, under
   emu2 with either memory size and under DOSBox-X, and the EXE is otherwise the same,
   byte for byte and in size.
   The name is uwedit.exe because TLINK stores it in the EXE (__EXENAME__) and UW.EXE has
   that, as UW2.EXE does. UW1 uses no floating point, so EMU.LIB and MATHM.LIB
   (which TCC would add) contribute nothing; the C library's modules come before the second
   library's and the overlay manager's in UW.EXE, hence the library order.
5. The EXE and map land in <build>/LINK/out (or --out) as UW.EXE and UW.MAP, and Exhume's
   tools/exediff.py compares the EXE with yours.
"""
import sys, os, json, subprocess, shutil, re
here = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.dirname(here)
# Exhume's tools: tools/exhume.py finds the checkout ($EXHUME, else the repository's submodule
# ../exhume, else .exhume, else ~/Exhume)
sys.path.insert(0, here)
import exhume
EXHUME = exhume.EXHUME
TOOLS = os.path.join(EXHUME, 'tools')
sys.path.insert(0, TOOLS)
import config, modding
CFGPATH, ARGS = config.pop_config(sys.argv[1:])
CFG = config.load(CFGPATH or os.path.join(ROOT, 'exhume.toml'))
root = CFG.root
LINKDIR = os.path.join(CFG.build, 'LINK')
LINKDIR_EXACT = LINKDIR

def c0_source():
    """TC/C0.ASM with UW1's differences, which are UW2's. UW1's C0 (segment table entry 6 up to
    offset 0x249, where the C library's code starts) is the same code except that (1) the
    self-modifying patch of the exit-table scan comes after main returns, not before main is
    called, and (2) the null-pointer checksum starts at DATASEG@ (DS:4, where _DATA begins)
    through `mov ax,offset DATASEG@; mov si,ax`, instead of at 0. And (3) its _FARDATA is not
    paragraph-aligned: segment table entry 50 is an empty segment at 395A:A, right after
    _OVRTEXT_ ends (word as in UW2; byte would do as well at that even address). Each was
    tested by linking without it: (1) moves one relocation and 28 bytes of C0, (2) changes
    12,361 bytes and 88 relocations, (3) three bytes of the segment table."""
    s = open(os.path.join(CFG.home, 'C0.ASM'), encoding='latin1').read().replace('\r\n', '\n')
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

# ---- checking objects --------------------------------------------------------------
def _records(d):
    p = 0; out = []
    while p + 3 <= len(d):
        t = d[p]; n = d[p + 1] | d[p + 2] << 8
        out.append([t, bytearray(d[p + 3:p + 2 + n])]); p += 3 + n
        if t in (0x8A, 0x8B): break
    return out

def _idx(b, i):
    return ((b[i] & 0x7F) << 8 | b[i + 1], i + 2) if b[i] & 0x80 else (b[i], i + 1)

def stub_order_wrong(d, order):
    """True unless object d lists the publics in order (the EXE's overlay stub order, entry 0
    first) from last to first: TLINK numbers stub entries from the last public listed.

    Turbo C lists a file's publics in descending order of the key tools/bssorder.py computes
    from each name, names with equal keys in the reverse of the order they were first seen
    (a prototype counts; see CUTS.C); so the EXE's stub order is a constraint on the names.
    A file that breaks it has a name whose key sorts differently from the original's."""
    want = set(order); listed = []
    for t, b in _records(d):
        if t in (0x90, 0x91):
            gi, i = _idx(b, 0); si, i = _idx(b, i)
            if si == 0: i += 2
            while i < len(b):
                l = b[i]; nm = b[i + 1:i + 1 + l].decode('latin1'); i += 1 + l + 2; _, i = _idx(b, i)
                if nm in want: listed.append(nm)
    return listed != list(reversed(order))

# what extract.py reports that the link used to patch into copies of the objects, and now
# leaves to the sources: each is a defect in one
SOURCE_DEFECTS = {'bytealigned': 'its _DATA or _BSS starts at an odd address: a leading item of its data is missing',
                  'statics': 'publics with no overlay stub entry: UW1 had them static',
                  'retarget': 'one name used for two variables',
                  'addfix': 'a constant where UW.EXE has a segment relocation'}

def build_fardata():
    """Assemble each far data source (marked /* fardata */) whose object is missing or older."""
    import build, sources
    for src in sources.all_sources(CFG):
        if not src.upper().endswith('.ASM'): continue
        text = open(src, encoding='latin1').read(3000)
        if not re.search(r'/\*\s*fardata\s*\*/', text): continue
        stem = CFG.stem(src); obj = CFG.obj(stem)
        if os.path.exists(obj) and os.path.getmtime(obj) >= os.path.getmtime(src): continue
        ok, msgs = build.build(CFG, [src])[stem]
        if not ok: sys.exit(f'{stem}: assembly failed\n' + '\n'.join(msgs))

def changed_sources():
    """--mod: {stem: object} for each source whose text (with the shared headers it includes)
    is not what the last exact run built, compiled into <build>/MODLINK/src/STEM with the
    source's own /* opts: */."""
    import build, sources
    overlay_manager = sources.by_segment('seg046', CFG)      # linked from OVERLAY.LIB, not its source
    srcs = [s for s in build.all_sources(CFG) if CFG.stem(s) != overlay_manager]
    return modding.changed_sources(CFG, os.path.join(LINKDIR_EXACT, 'base'), os.path.join(LINKDIR, 'src'), srcs,
                                   'run the exact link (python3 tools/link.py) once while every source matches')

def main():
    global LINKDIR
    a = ARGS
    mod = '--mod' in a
    if mod: LINKDIR = os.path.join(CFG.build, 'MODLINK')
    out = os.path.join(LINKDIR, 'out')
    if '--out' in a: out = a[a.index('--out') + 1]
    changed = {}
    if mod:
        changed = changed_sources()
        print('changed sources:', ' '.join(sorted(changed)) or 'none')
        subprocess.run([sys.executable, os.path.join(here, 'extract.py'), '--mod', '--config', CFG.file], check=True)
    elif '--no-extract' not in a:
        build_fardata()
        subprocess.run([sys.executable, os.path.join(here, 'extract.py'), '--config', CFG.file], check=True)
    man = json.load(open(os.path.join(LINKDIR, 'manifest.json')))
    open(os.path.join(LINKDIR, 'C0UW1.ASM'), 'w', newline='').write(c0_source())
    files = []; batch = []
    for g in man['generated']:
        files.append(os.path.join(LINKDIR, g + '.ASM'))
        batch.append(f'TASM /ml {g}')
    # TLINK stores the link date in the overlay data (__EXEDATE__): UW.EXE's is 12 May 1993, as UW2.EXE's
    open(os.path.join(LINKDIR, 'SETDATE.ASM'), 'w', newline='').write('\r\n'.join([
        '; sets the DOS date for TLINK: 12 May 1993, as UW.EXE records', '        .model tiny', '        .code',
        '        org 100h', 'start:  mov ah,2Bh', '        mov cx,1993', '        mov dx,050Ch', '        int 21h',
        '        mov ax,4C00h', '        int 21h', '        end start', '']))
    files.append(os.path.join(LINKDIR, 'SETDATE.ASM'))
    batch += ['TASM SETDATE', 'TLINK /t SETDATE', 'SETDATE']
    files.append(os.path.join(LINKDIR, 'C0UW1.ASM'))
    batch.append('TASM /D__MEDIUM__ /MX C0UW1')
    renames = json.load(open(os.path.join(LINKDIR, 'renames.json')))
    bad = [f'{k}: refers to {n} where the EXE has {v}' for k, rs in renames.items() for n, v in rs.items()]
    for x, why in SOURCE_DEFECTS.items():
        bad += [f'{k}: {why}' for k in man.get(x) or ()]      # a list or a dict by object
    if bad: sys.exit('sources to correct before linking:\n  ' + '\n  '.join(bad))
    override = dict(x.split('=', 1) for k, x in enumerate(a) if k and a[k - 1] == '--obj')
    override = {**changed, **override}
    objdir = os.path.join(LINKDIR, 'obj'); os.makedirs(objdir, exist_ok=True)
    for k, p in man['objects'].items():
        if k in man['resident'] or k in man['overlays'] or k in man['late']:
            d = open(override.get(k, p), 'rb').read()
            if not mod and man['stuborder'].get(k) and stub_order_wrong(d, man['stuborder'][k]):
                bad.append(f'{k}: publics listed out of the EXE\'s overlay stub order: a name whose '
                           'tools/bssorder.py key sorts differently from the original\'s')
            open(os.path.join(objdir, k + '.OBJ'), 'wb').write(d)
            files.append(os.path.join(objdir, k + '.OBJ'))
    added = [x.split('=', 1) for k, x in enumerate(a) if k and a[k - 1] == '--add']
    if added and not mod: sys.exit('--add needs --mod: the exact link has only the matched objects')
    resident = list(man['resident'])
    for k, p in added:
        open(os.path.join(objdir, k + '.OBJ'), 'wb').write(open(p, 'rb').read())
        files.append(os.path.join(objdir, k + '.OBJ'))
        resident.insert(resident.index('XFAR') if 'XFAR' in resident else len(resident), k)
    if bad: sys.exit('sources to correct before linking:\n  ' + '\n  '.join(bad))
    # through a response file: the module list is longer than a DOS command line
    late = ['+' + k for k in man['late']]
    open(os.path.join(LINKDIR, 'LIB.RSP'), 'w', newline='').write(
        ''.join(' '.join(late[i:i + 8]) + (' &\r\n' if i + 8 < len(late) else '\r\n') for i in range(0, len(late), 8)))
    files.append(os.path.join(LINKDIR, 'LIB.RSP'))
    batch.append('TLIB UWLIB @LIB.RSP')
    objs = resident + ['/o'] + man['overlays'] + ['/o-']
    lines = []
    for i in range(0, len(objs), 8):
        lines.append(' '.join(objs[i:i + 8]) + (' +' if i + 8 < len(objs) else ''))
    # the EXE's own name is stored too (__EXENAME__): uwedit.exe, in lower case
    rsp = '/c /m /s /i ' + '\r\n'.join(lines) + '\r\nuwedit.exe\r\nuwedit.map\r\nCM.LIB UWLIB.LIB OVERLAY.LIB\r\n'
    open(os.path.join(LINKDIR, 'LINK.RSP'), 'w', newline='').write(rsp)
    files.append(os.path.join(LINKDIR, 'LINK.RSP'))
    batch.append('TLINK @LINK.RSP')
    cmd = ['node', os.path.join(TOOLS, 'dosrun.mjs'), out, '-t', '600']
    for s in CFG.stage: cmd += ['--stage', s]
    for f in files: cmd += ['-f', f]
    for b in batch: cmd += ['-c', b]
    cmd += ['-o', 'UWEDIT.EXE', '-o', 'UWEDIT.MAP']
    # the bytes no record covers are 0 through TLINK's /i (above), whatever the DOS's memory
    r = subprocess.run(cmd)
    for x in ('EXE', 'MAP'):
        if os.path.exists(os.path.join(out, 'UWEDIT.' + x)):
            os.replace(os.path.join(out, 'UWEDIT.' + x), os.path.join(out, 'UW.' + x))
    log = open(os.path.join(out, 'RUN.LOG'), encoding='latin1').read()
    errs = [l for l in log.splitlines() if re.search(r'Error|Fatal|Undefined|Warning', l) and not re.search(r'messages: +None', l)]
    print('\n'.join(errs[:60]))
    if len(errs) > 60: print(f'... {len(errs) - 60} more')
    if r.returncode: sys.exit(r.returncode)
    if mod:
        if not os.path.exists(os.path.join(out, 'UW.EXE')) or errs: sys.exit('the modding link failed')
        n = modding.clear_overlay_padding(os.path.join(out, 'UW.EXE'))
        if n: print(f'modding build: {n} stale bytes in the overlays\' paragraph padding set to 0 (tools/modding.py, clear_overlay_padding)')
        print(f'modding build: {os.path.join(out, "UW.EXE")}, '
              f'{os.path.getsize(os.path.join(out, "UW.EXE"))} bytes (yours: {len(CFG.exe)})')
        return
    sys.exit(subprocess.run([sys.executable, os.path.join(TOOLS, 'exediff.py'), os.path.join(out, 'UW.EXE'), '--config', CFG.file]).returncode)

if __name__ == '__main__':
    main()
