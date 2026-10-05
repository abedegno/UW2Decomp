"""Link UW2.EXE from the matched objects with Turbo Link 3.01, headless in DOS, and compare.

    python3 tools/link.py [--no-extract] [--out DIR] [--obj STEM=PATH ...]
    python3 tools/link.py --mod [--out DIR] [--obj STEM=PATH ...] [--add STEM=PATH ...]

--obj links another build of one object in place of build/STEM/STEM.OBJ (to try a changed
source without disturbing the matched build).

--add (with --mod only) links one more resident module that no source in the matched build has,
after the last resident code module: the replay DOS build adds the record and replay code
(Exhume's runtime/replay/replay.c, tools/replay.py) this way.

--mod is the modding build (docs/LINKING.md, "The modding build"): sources may change by any size. The
layout comes from the last exact run (build/LINK/base, written by extract.py when every object
verified), the sources whose text differs from that run's are compiled into build/MODLINK/src
(the matched objects in build/ are left alone; several to a DOS session and several sessions
at once, as the gate compiles, tools/dosbatch.py), and the EXE goes to build/MODLINK/out. Nothing
is compared with your EXE, and an overlay's publics may come in any order (TLINK then numbers
its stub entries differently, which nothing depends on). The paragraph padding after each
overlay is set to 0, as UW2.EXE and the exact link have it (clear_overlay_padding).

1. tools/extract.py writes the data-only modules, manifest.json and renames.json under
   build/LINK (skip with --no-extract when they are current). Before it, a far data source
   (an .ASM marked /* fardata */, src/sys/FARDATA.ASM) is assembled into build/STEM when its
   object is missing or older, since extract.py compares its segments with the EXE.
2. C0UW2.ASM is Turbo C++'s own TC/C0.ASM with the changes UW2's startup code shows (see
   c0_source), assembled as BUILD-C0.BAT does for the medium model.
3. The objects are copied to build/LINK/obj as they are. Nothing is adjusted: a source whose
   names, statics, alignment or segment references disagree with the EXE (extract.py reports
   them), or an overlay whose publics are listed out of the EXE's stub order (see
   stub_order_wrong), stops the link until it is corrected.
4. In DOS (tools/dosrun.mjs, in the DOS tools/dosbackend.mjs picks; with emu2 the date
   needs tools/emu2-date.patch): the date is set to 12 May 1993 (TLINK records it), TASM assembles the generated
   modules, TLIB puts the second library's modules (seg003's, seg004's, seg021's but its
   first, and seg045's) into UWLIB.LIB in the manifest's order,
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
EXE = os.environ.get('UW2_EXE', os.path.expanduser('~/UWGOG/UW2/UW2.EXE'))
LINKDIR = os.path.join(root, 'build', 'LINK')
LINKDIR_EXACT = LINKDIR

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
                  'statics': 'publics with no overlay stub entry: UW2 had them static',
                  'retarget': 'one name used for two variables',
                  'addfix': 'a constant where UW2 has a segment relocation'}

def build_fardata():
    """Assemble each far data source (marked /* fardata */) whose object is missing or older."""
    from sources import all_sources, stem as stem_of
    for src in all_sources():
        if not src.upper().endswith('.ASM'): continue
        text = open(src, encoding='latin1').read(3000)
        if not re.search(r'/\*\s*fardata\s*\*/', text): continue
        stem = stem_of(src)
        outdir = os.path.join(root, 'build', stem); obj = os.path.join(outdir, stem + '.OBJ')
        if os.path.exists(obj) and os.path.getmtime(obj) >= os.path.getmtime(src): continue
        opts = re.search(r'/\*\s*opts:\s*([^*]+?)\s*\*/', text)
        subprocess.run(['node', os.path.join(here, 'tcc.mjs'), outdir, opts.group(1) if opts else '/ml', src], check=True)
        log = open(os.path.join(outdir, 'BUILD.LOG'), encoding='latin1').read()
        if not re.search(r'Error messages:\s+None', log) or not os.path.exists(obj):
            sys.exit(f'{stem}: assembly failed\n{log}')

def changed_sources():
    """--mod: {stem: object} for each source whose text (with the src/include headers it
    includes) is not what the last exact run built, compiled (unless the object there was built
    from this same text) into build/MODLINK/src/STEM
    with the source's own /* opts: */ (match.py's defaults otherwise). The compiles are
    batched and run in parallel as the gate's are (tools/dosbatch.py); a source that fails in
    a batch is compiled once more on its own before it counts as failed."""
    from srcdeps import source_hash
    from sources import all_sources, stem as stem_of, by_segment
    from dosbatch import compile_many, backend, sessions
    overlay_manager = by_segment('seg046')     # linked from OVERLAY.LIB, not from its source
    lay = os.path.join(LINKDIR_EXACT, 'base', 'layout.json')
    if not os.path.exists(lay):
        sys.exit('no build/LINK/base: run the exact link (python3 tools/link.py) once while every source matches')
    known = json.load(open(lay))['sources']
    out, todo, shas = {}, [], {}
    dest = lambda stem: os.path.join(LINKDIR, 'src', stem)
    for src in all_sources():
        text = open(src, 'rb').read()
        head = text[:3000].decode('latin1')
        if not re.search(r'/\*\s*(target:\s*\w+|fardata)\s*\*/', head): continue
        stem = stem_of(src)
        if stem == overlay_manager: continue
        if stem not in known: sys.exit(f'{stem}: a source the exact run did not have; --mod links the existing files only')
        sha = source_hash(src)          # the text and the src/include headers it includes
        if sha == known[stem][1]: continue
        d = dest(stem); obj = os.path.join(d, stem + '.OBJ')
        shafile = os.path.join(d, 'SOURCE.SHA1')
        if not os.path.exists(obj) or not os.path.exists(shafile) or open(shafile).read() != sha:
            opts = re.search(r'/\*\s*opts:\s*([^*]+?)\s*\*/', head)
            opts = opts.group(1) if opts else ('/ml' if src.upper().endswith('.ASM') else '-mm -1 -G -O -Z')
            print(f'compiling {os.path.relpath(src, root)} ({opts})')
            if os.path.exists(obj): os.remove(obj)
            if os.path.exists(shafile): os.remove(shafile)
            todo.append((stem, src, opts)); shas[stem] = sha
        out[stem] = obj
    if todo:
        if len(todo) > 1: print(f'{len(todo)} sources in {backend()}, {sessions()} sessions at once')
        res = compile_many(todo, dest)
        again = [t for t in todo if res.get(t[0])]
        if again: res.update(compile_many(again, dest, batch=1))     # failed in a batch: once more alone
        for stem, src, opts in todo:
            if res.get(stem): sys.exit(f'{stem}: build failed\n{res[stem]}')
            open(os.path.join(dest(stem), 'SOURCE.SHA1'), 'w').write(shas[stem])
    return out

def clear_overlay_padding(path):
    """Zero the padding TLINK leaves after each overlay's code and fixup list, up to the next
    overlay's paragraph (or the end of the FBOV area), and return how many bytes were not 0.
    Nothing reads those bytes: the overlay manager loads an overlay's code and fixups by the
    sizes in its stub. TLINK fills them from a buffer it does not clear, so what lands there
    depends on its heap: in the exact link (and in UW2.EXE) they are all 0, but the modding
    link, whose XFAR names near code offsets in seg004 (`dw offset NAME`) where the exact
    link has bytes, leaves 4 bytes of old code after ovr167's fixups once seg003 and seg004
    have their own class (docs/LINKING.md, "Segment classes"). Only --mod does this; the
    exact link's EXE is TLINK's own."""
    import struct
    d = bytearray(open(path, 'rb').read())
    w16 = lambda i: struct.unpack_from('<H', d, i)[0]
    hdr = w16(8) * 16; end = (w16(4) - 1) * 512 + w16(2) if w16(2) else w16(4) * 512
    if d[end:end + 4] != b'FBOV': return 0
    size, segtab, nseg = struct.unpack_from('<III', d, end + 4); base = end + 16
    ovl = sorted((struct.unpack_from('<I', d, hdr + para * 16 + 4)[0], w16(hdr + para * 16 + 8), w16(hdr + para * 16 + 10))
                 for para, _, fl, _ in (struct.unpack_from('<4H', d, segtab + 8 * i) for i in range(nseg)) if fl == 3)
    n = 0
    for k, (at, code, fix) in enumerate(ovl):
        lo, hi = base + at + code + fix, base + (ovl[k + 1][0] if k + 1 < len(ovl) else size)
        n += sum(1 for b in d[lo:hi] if b); d[lo:hi] = bytes(hi - lo)
    if n: open(path, 'wb').write(d)
    return n

def main():
    global LINKDIR
    a = sys.argv[1:]
    mod = '--mod' in a
    if mod: LINKDIR = os.path.join(root, 'build', 'MODLINK')
    out = os.path.join(LINKDIR, 'out')
    if '--out' in a: out = a[a.index('--out') + 1]
    changed = {}
    if mod:
        changed = changed_sources()
        print('changed sources:', ' '.join(sorted(changed)) or 'none')
        subprocess.run([sys.executable, os.path.join(here, 'extract.py'), '--mod'], check=True)
    elif '--no-extract' not in a:
        build_fardata()
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
    override = {**changed, **override}
    objdir = os.path.join(LINKDIR, 'obj'); os.makedirs(objdir, exist_ok=True)
    for k, p in man['objects'].items():
        if k in man['resident'] or k in man['overlays'] or k in man['late']:
            d = open(override.get(k, os.path.join(root, p)), 'rb').read()
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
    if mod:
        if not os.path.exists(os.path.join(out, 'UW2.EXE')) or errs: sys.exit('the modding link failed')
        n = clear_overlay_padding(os.path.join(out, 'UW2.EXE'))
        if n: print(f'modding build: {n} stale bytes in the overlays\' paragraph padding set to 0 (clear_overlay_padding)')
        print(f'modding build: {os.path.relpath(os.path.join(out, "UW2.EXE"), root)}, '
              f'{os.path.getsize(os.path.join(out, "UW2.EXE"))} bytes (yours: {os.path.getsize(EXE)})')
        return
    sys.exit(subprocess.run([sys.executable, os.path.join(here, 'exediff.py'), os.path.join(out, 'UW2.EXE')]).returncode)

if __name__ == '__main__':
    main()
