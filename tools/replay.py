"""Record a session in DOS and replay it in DOS and in the port, comparing the state dumps
(docs/PORT.md, "The differential test"; src/replay/REPLAY.C has the formats).

    python3 tools/replay.py build                       the replay DOS build: build/replay/UW2.EXE
    python3 tools/replay.py record OUT [step ...]       record a session in DOS (steps: tools/replaydos.mjs)
    python3 tools/replay.py record OUT --session NAME   ... with a session named below (SESSIONS)
    python3 tools/replay.py dos REC OUT                 replay REC (a RECORD.OUT) in DOS
    python3 tools/replay.py port REC OUT [--debug]      replay REC in the port (build/port/uw2port, or
                                                        make port-debug's UBSan build)
    python3 tools/replay.py compare A B [--same-build]  compare two STATE.OUT files (or their directories)
    python3 tools/replay.py log REC                     what a recording holds
    python3 tools/replay.py show STATE [PNGDIR]         list a dump's checkpoints; write each full one's screen as a PNG
    python3 tools/replay.py nulls DIR                   the null-pointer write check of a DOS run's directory
    python3 tools/replay.py check REC OUT               replay REC in DOS twice and in the port, compare all

The replay DOS build is the modding build with every source that uses the hooks of
src/include/portable.h compiled with -DREPLAY (and every source with a NULLTRAP mark with
-DNULLTRAP, so the marked null pointers it reaches go to NULLTRAP.LOG), and src/replay/REPLAY.C
linked in as one more resident module (tools/link.py --mod --add). Recording and the DOS
replays run in js-dos (tools/replaydos.mjs); the port replays with --replay.

compare walks two dumps checkpoint by checkpoint. Each checkpoint names the event it was taken
at (the count of hook calls) and the clock, so a checkpoint whose header differs means the two
runs went different ways before it, and the comparison stops there. Within a checkpoint each
section is compared byte for byte, the screen also as the CRT controller would show it.
Sections in IGNORE are not compared between DOS and the port (their bytes are the program's
own machinery, not game state); --same-build compares everything, for two runs of one build.

Everything goes under build/replay unless an OUT says otherwise. The game data comes from
UW2_DIR, else the directory of UW2_EXE, else ~/UWGOG/UW2.
"""
import os, sys, re, json, struct, zlib, hashlib, shutil, subprocess

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
sys.path.insert(0, here)
OUT = os.path.join(root, 'build', 'replay')
PY = os.path.join(root, '.venv', 'bin', 'python')
if not os.path.exists(PY): PY = sys.executable
DATA = os.environ.get('UW2_DIR') or (os.path.dirname(os.environ['UW2_EXE']) if os.environ.get('UW2_EXE')
                                     else os.path.expanduser('~/UWGOG/UW2'))
HOOKS = re.compile(r'\b(GAME_TIME|KEY|MOUSE|MBUTTONS|JOY_READ|JOY_BUTTONS|WALL_TIME|SRAND|CHECKPOINT|STACK_JUNK)\(')
KINDS = {1: 'checkpoint', 2: 'periodic', 3: 'input', 4: 'end', 5: 'DESYNC'}
STREAMS = {1: 'TIME', 2: 'KEY', 3: 'MOUSE', 4: 'BUTTONS', 5: 'JOY', 6: 'JOYB', 7: 'MISC'}
# Byte ranges of a section that are the program's machinery rather than game state:
# (section, start, end, why). ALWAYS differ between any two runs, even of one build; CROSS
# between DOS and the port.
ALWAYS = [
    ('GFX ', 0x4E00, 0x4FA8, "seg003's private stack (GRCORE.ASM switches to 370D:4FA8): "
                             "interrupts push onto it whenever they come"),
    ('GFX ', 0x5500, 0x5548, "the stack GRENTRY.ASM and GRDISP.ASM switch to (SP 5548h, saved at 5588h)"),
    ('GFX ', 0x652, 0x654, "GRMISC.ASM's _46: the low word of the real clock after each 3D frame, "
                           "for the retrace wait's time-out"),
]
CROSS = [
    ('GFX ', 0x558A, 0x558E, "GRDISP.ASM's saved SS:SP"),
]

# Recorded sessions: steps for tools/replaydos.mjs. Timings are generous: the recording does
# not depend on them, only on the inputs arriving where the game reads them.
SESSIONS = {
    # boot, the title cutscene, the main menu, a new character, into the game
    # (no saved game, so the introduction starts after the title: Escape skips its two
    # cutscenes), then Create Character with Enter for every default (male, right-handed,
    # fighter, two skill picks, the first portrait, standard difficulty), the name Avatar,
    # Yes, and sixteen seconds in the game; tools/replaydos.mjs then sends F12
    'newgame': ['w:14000', 'k:Escape', 'w:2500', 'k:Escape', 'w:3500', 'k:Enter', 'w:4000'] +
               ['k:Enter', 'w:2000'] * 8 + ['t:Avatar', 'w:1000', 'k:Enter', 'w:2000', 'k:Enter',
               'w:4000', 's:ingame1', 'w:6000', 's:ingame2', 'w:6000'],
}


def opts_of(src):
    m = re.search(r'/\*\s*opts:\s*([^*]+?)\s*\*/', open(src, encoding='latin1').read(3000))
    return m.group(1) if m else '-mm -1 -G -O -Z'


def build():
    """Compile the hook users with -DREPLAY (the NULLTRAP users with -DNULLTRAP too) and
    REPLAY.C, and link them as the modding build with REPLAY added. Cached by the sources'
    hashes."""
    from sources import all_sources, replay_sources, stem
    from srcdeps import source_hash
    from dosbatch import compile_many
    todo = []
    for src in all_sources() + replay_sources():
        if not src.upper().endswith('.C'): continue
        text = open(src, encoding='latin1').read()
        defs = []
        if HOOKS.search(text) or src in replay_sources(): defs.append('-DREPLAY')
        if 'NULLTRAP(' in text: defs.append('-DNULLTRAP')
        if defs: todo.append((stem(src), src, (opts_of(src) if src not in replay_sources() else '-mm -1 -G -O -Y -d') + ' ' + ' '.join(defs)))
    key = hashlib.sha1(json.dumps([(s, source_hash(p), o) for s, p, o in todo]).encode()).hexdigest()
    exe = os.path.join(OUT, 'UW2.EXE'); stamp = os.path.join(OUT, 'build.sha1')
    if os.path.exists(exe) and os.path.exists(stamp) and open(stamp).read() == key:
        print(f'replay build: {os.path.relpath(exe, root)} (up to date)'); return exe
    objdir = lambda s: os.path.join(OUT, 'obj', s)
    print(f'replay build: compiling {len(todo)} sources with -DREPLAY or -DNULLTRAP')
    res = compile_many(todo, objdir)
    again = [t for t in todo if res.get(t[0])]
    if again: res.update(compile_many(again, objdir, batch=1))
    bad = {k: v for k, v in res.items() if v}
    if bad: sys.exit('replay build failed:\n' + '\n'.join(f'{k}: {v}' for k, v in bad.items()))
    cmd = [PY, os.path.join(here, 'link.py'), '--mod', '--out', os.path.join(OUT, 'link')]
    for s, _, _ in todo:
        obj = os.path.join(objdir(s), s + '.OBJ')
        cmd += ['--add' if s == 'REPLAY' else '--obj', f'{s}={obj}']
    subprocess.run(cmd, check=True)
    shutil.copy(os.path.join(OUT, 'link', 'UW2.EXE'), exe)
    open(stamp, 'w').write(key)
    print(f'replay build: {os.path.relpath(exe, root)}, {os.path.getsize(exe)} bytes')
    return exe


# ---- reading the files --------------------------------------------------------------------

def read_dump(path):
    """STATE.OUT as a list of checkpoints: dicts with kind, n, events, time and sections."""
    if os.path.isdir(path): path = os.path.join(path, 'STATE.OUT')
    d = open(path, 'rb').read(); p = 0; out = []
    while p + 20 <= len(d):
        if d[p:p + 4] != b'CKPT': raise SystemExit(f'{path}: no checkpoint at byte {p}')
        kind, n, ev, t, ns = struct.unpack_from('<HHIIH', d, p + 4); p += 18
        secs = {}
        for _ in range(ns):
            if p + 8 > len(d): break
            tag = d[p:p + 4].decode('latin1'); ln = struct.unpack_from('<I', d, p + 4)[0]; p += 8
            secs[tag] = d[p:p + ln]; p += ln
        out.append(dict(kind=kind, n=n, events=ev, time=t, secs=secs))
    return out


def read_log(path):
    """RECORD.OUT (src/replay/REPLAY.C): the streams' runs decoded. Returns {stream: [runs]},
    each run (count, value), and the call count the recording stopped at."""
    d = open(path, 'rb').read()
    if d[:4] != b'UW2R' or d[4] != 2: raise SystemExit(f'{path}: not a version 2 recording')
    stop = struct.unpack_from('<I', d, 8)[0]
    data = {}; p = 12
    while p + 3 <= len(d):
        s, n = d[p], struct.unpack_from('<H', d, p + 1)[0]; p += 3
        data.setdefault(s, bytearray()).extend(d[p:p + n]); p += n
    out = {}
    for s, b in data.items():
        runs = []; q = 0; t = 0
        while q < len(b):
            if s == 7:
                tag = b[q]; q += 1
                if tag == 7: runs.append(('WALL', struct.unpack_from('<I', b, q)[0])); q += 4
                else: runs.append(({8: 'SRAND', 9: 'CKPT'}.get(tag, tag), struct.unpack_from('<H', b, q)[0])); q += 2
                continue
            c = struct.unpack_from('<H', b, q)[0]; q += 2
            if s == 1:
                dt = b[q]; q += 1
                if dt == 0xFF: t = struct.unpack_from('<I', b, q)[0]; q += 4
                else: t += dt
                runs.append((c, t))
            elif s == 2:
                r, n = struct.unpack_from('<HB', b, q); q += 3
                q += 0x89 if n == 0xFF else 2 * n
                runs.append((c, r))
            elif s == 4: runs.append((c, struct.unpack_from('<H', b, q)[0])); q += 2
            else: runs.append((c, struct.unpack_from('<hh', b, q))); q += 4
        out[STREAMS.get(s, s)] = runs
    return out, stop


def log_summary(path):
    runs, stop = read_log(path)
    parts = [f'stopped at call {stop}' if stop else 'never stopped']
    for k, v in runs.items():
        if k == 'MISC': parts.append('MISC ' + ' '.join(f'{a}:{b:X}' for a, b in v)); continue
        parts.append(f'{k} {sum(c for c, _ in v)} calls in {len(v)} runs')
    keys = [r for c, r in runs.get('KEY', []) if r]
    if keys: parts.append('keys ' + ' '.join(f'{k:04X}' for k in keys))
    if 'TIME' in runs: parts.append(f"clock {runs['TIME'][0][1]:X} to {runs['TIME'][-1][1]:X}")
    return '; '.join(parts)


def scanout(secs):
    """The visible screen from the VGA and CRTC sections, as the CRT controller shows it
    (src/port/gfx/vga.c's vga_scanout without pixel panning, which the dump does not hold):
    320 by 200 (or 400) palette indices."""
    vga, c = secs['VGA '], secs['CRTC']
    r07, r09, r0c, r0d, r13, r18 = c
    msl = (r09 & 0x1F) + 1; rows = min(400 // msl, 400)
    start = r0c << 8 | r0d; pitch = r13 * 2
    lc = r18 | (r07 & 0x10) << 4 | (r09 & 0x40) << 3
    pix = bytearray(320 * rows); addr = start
    planes = [vga[i * 0x10000:(i + 1) * 0x10000] for i in range(4)]
    for y in range(rows):
        sl = y * msl
        if y and sl > lc and sl - msl <= lc: addr = 0
        for x in range(320):
            pix[y * 320 + x] = planes[x & 3][(addr + (x >> 2)) & 0xFFFF]
        addr += pitch
    return bytes(pix), rows


def write_png(path, pix, w, h, pal6):
    pal = bytes(min(255, (v << 2) | (v >> 4)) for v in pal6)
    raw = b''.join(b'\0' + pix[y * w:(y + 1) * w] for y in range(h))
    def chunk(t, b): return struct.pack('>I', len(b)) + t + b + struct.pack('>I', zlib.crc32(t + b) & 0xFFFFFFFF)
    png = b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 3, 0, 0, 0)) + \
        chunk(b'PLTE', pal) + chunk(b'IDAT', zlib.compress(raw, 9)) + chunk(b'IEND', b'')
    open(path, 'wb').write(png)


def label(ck):
    k = KINDS.get(ck['kind'], str(ck['kind']))
    n = ck['n']
    if ck['kind'] == 3: n = f"buttons {n & 0xFF}" if n & 0x8000 else f"key {n:04X}"
    return f"{k} {n} at event {ck['events']}, clock {ck['time']:X}"


# ---- comparing ------------------------------------------------------------------------------

def ranges(a, b, skip=()):
    """The differing byte ranges of two equal-length byte strings, merged when close."""
    out = []
    for i in range(min(len(a), len(b))):
        if a[i] != b[i] and not any(lo <= i < hi for lo, hi in skip):
            if out and i - out[-1][1] <= 8: out[-1][1] = i + 1
            else: out.append([i, i + 1])
    return out


def segment_words(x, y, sa, sb):
    """The words where x holds one far block's segment in its build and y the same block's in
    the other (SEGS, src/replay/REPLAY.C): stored far pointers, equal in meaning."""
    na, nb = struct.unpack(f'<{len(sa) // 2}H', sa), struct.unpack(f'<{len(sb) // 2}H', sb)
    pairs = {(u, v) for u, v in zip(na, nb) if u != v}
    # The EXE's own segments (the far data from 3705 to 6061, seg021's, DGROUP) all move by one
    # load segment, which seg_370D's pair gives: a word that is an EXE paragraph plus the load
    # segment in each build is one.
    delta = (na[0] - nb[0]) & 0xFFFF
    lo_a, lo_b = (na[0] - 0x370D) & 0xFFFF, (nb[0] - 0x370D) & 0xFFFF
    def same(u, v):
        return (u, v) in pairs or ((u - v) & 0xFFFF == delta and 0x3705 <= (u - lo_a) & 0xFFFF < 0x7600
                                   and 0x3705 <= (v - lo_b) & 0xFFFF < 0x7600)
    out = []; i = 0
    while i + 1 < len(x):
        if x[i:i + 2] != y[i:i + 2] and same(x[i] | x[i + 1] << 8, y[i] | y[i + 1] << 8):
            out.append((i, i + 2)); i += 2
        else: i += 1
    return out


def compare(pa, pb, same_build=False, quiet=False):
    A, B = read_dump(pa), read_dump(pb)
    print(f'{pa}: {len(A)} checkpoints; {pb}: {len(B)}')
    worst = 0; first_bad = None
    for i in range(min(len(A), len(B))):
        a, b = A[i], B[i]
        if (a['kind'], a['n'], a['events'], a['time']) != (b['kind'], b['n'], b['events'], b['time']):
            print(f'checkpoint {i}: the runs went different ways: {label(a)} against {label(b)}')
            if 'CNTS' in a['secs'] and 'CNTS' in b['secs']:
                ca = struct.unpack('<7I', a['secs']['CNTS']); cb = struct.unpack('<7I', b['secs']['CNTS'])
                print('  calls by stream: ' + ', '.join(f'{STREAMS[k + 1]} {x}' + (f' against {y}' if x != y else '')
                                                    for k, (x, y) in enumerate(zip(ca, cb))))
            return 2
        notes = []
        for tag in sorted(set(a['secs']) | set(b['secs'])):
            if tag == 'NULL' and not same_build: continue
            x, y = a['secs'].get(tag), b['secs'].get(tag)
            if x is None or y is None: notes.append(f'{tag.strip()} only in one'); continue
            if tag == 'SEGS' and not same_build: continue
            skip = [(lo, hi) for s, lo, hi, _ in ALWAYS + ([] if same_build else CROSS) if s == tag]
            if len(x) != len(y): notes.append(f'{tag.strip()} lengths {len(x)} and {len(y)}'); continue
            if not same_build and 'SEGS' in a['secs'] and 'SEGS' in b['secs']:
                skip += segment_words(x, y, a['secs']['SEGS'], b['secs']['SEGS'])
            if tag == 'GFX ' and not same_build:
                # GRCORE's string entries copy the string, its 0 and one byte more to 370D:4FA8;
                # that byte is whatever followed the string in the caller's buffer, often stack junk
                z = x.find(b'\0', 0x4FA8, 0x4FA8 + 0x84)
                if z >= 0: skip.append((z + 1, z + 2))
            r = ranges(x, y, skip)
            if not r: continue
            if tag == 'VGA ':
                sa, ha = scanout(a['secs']); sb, hb = scanout(b['secs'])
                px = sum(1 for k in range(min(len(sa), len(sb))) if sa[k] != sb[k])
                notes.append(f'VGA {sum(h - l for l, h in r)} bytes differ, screen {px} of {len(sa)} pixels')
            else:
                notes.append(f'{tag.strip()} {sum(h - l for l, h in r)} bytes differ: ' +
                             ', '.join(f'{l:X}..{h - 1:X}' for l, h in r[:6]) + (' ...' if len(r) > 6 else ''))
        if notes:
            worst = 1
            if first_bad is None: first_bad = i
            print(f'checkpoint {i} ({label(a)}): ' + '; '.join(notes))
        elif not quiet and a['kind'] != 2:
            print(f'checkpoint {i} ({label(a)}): identical')
    if len(A) != len(B):
        print(f'one run has {abs(len(A) - len(B))} more checkpoints (the shorter stopped earlier)')
        worst = max(worst, 1)
    print('identical' if worst == 0 else f'differences, the first at checkpoint {first_bad}' if first_bad is not None else 'differences')
    return worst


def show(path, pngdir=None):
    for i, ck in enumerate(read_dump(path)):
        secs = ' '.join(f'{t.strip()}:{len(v)}' for t, v in ck['secs'].items())
        print(f'{i:4d} {label(ck)}  [{secs}]')
        if pngdir and 'VGA ' in ck['secs']:
            os.makedirs(pngdir, exist_ok=True)
            pix, h = scanout(ck['secs'])
            write_png(os.path.join(pngdir, f'ck{i:04d}.png'), pix, 320, h, ck['secs']['PAL '])


def nulls(d):
    """The null-pointer write check of a DOS run: the NULL sections of its dumps (DS:0..30h and
    the vector table as the game saw them, and C0's checksum) and dos-mcp's reads after the game
    exited (memory.json)."""
    bad = 0
    dump = read_dump(d)
    first = None
    for i, ck in enumerate(dump):
        s = ck['secs'].get('NULL')
        if not s: continue
        ds, chk, ivt = s[:0x31], struct.unpack_from('<H', s, 0x31)[0], s[0x33:]
        if first is None: first = (i, ds, ivt)
        if chk:
            print(f'checkpoint {i} ({label(ck)}): C0\'s checksum of DS:4..30h is off by {chk:04X}: a null pointer write'); bad = 1
        if ds[3] != 0 or not (ds[:3] == b"\x27\x06\x00" or ds[0] == 6):
            print(f'checkpoint {i}: DS:0..3 is {ds[:4].hex()}, neither form of ovr167\'s stub entry'); bad = 1
    if first:
        i, ds, ivt = first; last = dump[-1]['secs']['NULL'][0x33:]
        changed = [v for v in range(256) if ivt[v * 4:v * 4 + 4] != last[v * 4:v * 4 + 4]]
        print(f'vector table: {len(changed)} vectors changed between checkpoint {i} and the last: ' +
              ' '.join(f'{v:02X}' for v in changed))
        print(f'DS:0..3 at the first checkpoint {ds[:4].hex()}, at the last {dump[-1]["secs"]["NULL"][:4].hex()}')
    mj = os.path.join(d, 'memory.json')
    if os.path.exists(mj):
        m = json.load(open(mj))
        for g in m.get('groups', []):
            b = bytes.fromhex(g['ds0_3f'])
            print(f"after exit, DGROUP at linear {g['ds_linear']:X}: DS:0..3 {b[:4].hex()}, C0 checksum "
                  f"{(sum(b[4:0x31]) - 0xCA5) & 0xFFFF:04X} (0 when intact)")
    nl = os.path.join(d, 'NULLTRAP.LOG')
    if os.path.exists(nl):
        hits = open(nl, encoding='latin1').read().split()
        print(f'NULLTRAP.LOG: {len(hits)} null pointers met at marked sites: ' + ' '.join(sorted(set(hits))))
    else:
        print('NULLTRAP.LOG: none written (no marked null pointer met)')
    return bad


# ---- running ----------------------------------------------------------------------------------

def run_dos(out, rec=None, steps=(), timeout=900):
    exe = build()
    cmd = ['node', os.path.join(here, 'replaydos.mjs'), exe, out, '--timeout', str(timeout)]
    if rec: cmd += ['--replay', rec]
    r = subprocess.run(cmd + list(steps))
    return r.returncode


def run_port(rec, out, extra=()):
    extra = list(extra)
    debug = '--debug' in extra
    if debug: extra.remove('--debug')
    exe = os.path.join(root, 'build', 'port-debug' if debug else 'port', 'uw2port')
    if not os.path.exists(exe): sys.exit('replay.py: build the port first (make port)')
    home = os.path.join(out, 'home'); shutil.rmtree(home, ignore_errors=True); os.makedirs(home)
    cmd = [exe, '--data', DATA, '--home', home, '--hidden', '--exit-on-halt', '--exit-after', '600000',
           '--replay', os.path.abspath(rec)] + list(extra)
    r = subprocess.run(cmd, capture_output=True, text=True)
    open(os.path.join(out, 'port.log'), 'w').write(r.stdout + r.stderr)
    for f in ('STATE.OUT',):
        if os.path.exists(os.path.join(home, f)): shutil.copy(os.path.join(home, f), os.path.join(out, f))
    ub = [l for l in (r.stdout + r.stderr).splitlines() if 'runtime error' in l]
    if debug: print(f'port: UBSan reported {len(ub)} null dereferences' + ''.join('\n  ' + l for l in sorted(set(ub))[:20]))
    tail = [l for l in (r.stdout + r.stderr).splitlines() if 'uw2port' in l][-3:]
    print('port:', r.returncode, *tail, sep='\n  ')
    return r.returncode


def main(argv):
    if not argv: print(__doc__); return 2
    cmd, a = argv[0], argv[1:]
    if cmd == 'build': build(); return 0
    if cmd == 'record':
        out = a[0]; steps = a[1:]
        if steps[:1] == ['--session']: steps = SESSIONS[steps[1]]
        rc = run_dos(out, None, steps)
        if os.path.exists(os.path.join(out, 'RECORD.OUT')):
            print('recorded:', log_summary(os.path.join(out, 'RECORD.OUT')))
        return rc
    if cmd == 'log': print(log_summary(a[0])); return 0
    if cmd == 'dos': return run_dos(a[1], a[0])
    if cmd == 'port': return run_port(a[0], a[1], a[2:])
    if cmd == 'compare': return compare(a[0], a[1], '--same-build' in a)
    if cmd == 'show': show(a[0], a[1] if len(a) > 1 else None); return 0
    if cmd == 'nulls': return nulls(a[0])
    if cmd == 'check':
        rec, out = a[0], a[1]
        d1, d2, p = os.path.join(out, 'dos1'), os.path.join(out, 'dos2'), os.path.join(out, 'port')
        for d in (d1, d2, p): os.makedirs(d, exist_ok=True)
        run_dos(d1, rec); run_dos(d2, rec); run_port(rec, p)
        if os.path.exists(os.path.join(root, 'build', 'port-debug', 'uw2port')):
            pd = os.path.join(out, 'port-debug'); os.makedirs(pd, exist_ok=True); run_port(rec, pd, ['--debug'])
        print('\n== DOS against DOS'); r1 = compare(d1, d2, True, quiet=True)
        print('\n== DOS against the port'); r2 = compare(d1, p)
        print('\n== null pointers (DOS)'); r3 = nulls(d1)
        return max(r1, r2, r3)
    print(__doc__); return 2


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
