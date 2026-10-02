"""Compare the native port's screens with DOS's (docs/PORT.md, Milestone 3's exit test).

    python3 tools/portshot.py            build what is missing, take both sets of screenshots, compare
    python3 tools/portshot.py --rebuild  build the held DOS EXEs again

For each screen below, a DOS build that stops on it: UWEDIT.C with `for (;;) ;` after the call
that shows it, compiled with Turbo C (tools/tcc.mjs) and linked as the modding build with that
object in place of UWEDIT's (tools/link.py --mod --obj UWEDIT=...), then booted in headless
DOS and screenshotted once the screen is up (tools/rungame.mjs). The port shows the same
screens at its first and second page flips (build/port/uw2port --shot-at-flip). js-dos shows
mode X doubled to 640 by 400, so every second pixel of its picture is compared with the port's
320 by 200; both convert the DAC's 6-bit colour the same way, so equal pixels mean equal
indices under equal palettes. Exits 0 when every screen matches to the pixel.

Everything goes under build/portshot. The game data comes from UW2_DIR, else the directory of
UW2_EXE, else ~/UWGOG/UW2, as tools/rungame.mjs finds it.
"""
import os, sys, struct, zlib, hashlib, shutil, subprocess

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
OUT = os.path.join(root, 'build', 'portshot')
PY = os.path.join(root, '.venv', 'bin', 'python')
if not os.path.exists(PY): PY = sys.executable
DATA = os.environ.get('UW2_DIR') or (os.path.dirname(os.environ['UW2_EXE']) if os.environ.get('UW2_EXE')
                                     else os.path.expanduser('~/UWGOG/UW2'))
SCREENS = [   # name, the call in init_world (UWEDIT.C) that shows it, the port's flip number
    ('origin', 'display_screen(5, 6);', 1),
    ('lookingglass', 'display_screen(6, 7);', 2),
]


def load(path):
    """A PNG as rows of RGB bytes, sampled to 320 by 200."""
    d = open(path, 'rb').read(); i = 8; idat = b''
    while i < len(d):
        n = struct.unpack('>I', d[i:i + 4])[0]; t = d[i + 4:i + 8]; c = d[i + 8:i + 8 + n]; i += 12 + n
        if t == b'IHDR': w, h, bd, ct = struct.unpack('>IIBB', c[:10])
        elif t == b'IDAT': idat += c
    if bd != 8 or ct not in (2, 6): sys.exit(f'{path}: only 8-bit RGB or RGBA PNGs are read')
    raw = zlib.decompress(idat); bpp = 3 if ct == 2 else 4; stride = w * bpp
    rows = []; prev = bytearray(stride); p = 0
    for _ in range(h):
        f = raw[p]; line = bytearray(raw[p + 1:p + 1 + stride]); p += 1 + stride
        for x in range(stride):
            a = line[x - bpp] if x >= bpp else 0; b = prev[x]; c = prev[x - bpp] if x >= bpp else 0
            if f == 1: line[x] = (line[x] + a) & 255
            elif f == 2: line[x] = (line[x] + b) & 255
            elif f == 3: line[x] = (line[x] + (a + b) // 2) & 255
            elif f == 4:
                pa, pb, pc = abs(b - c), abs(a - c), abs(a + b - 2 * c)
                line[x] = (line[x] + (a if pa <= pb and pa <= pc else b if pb <= pc else c)) & 255
        rows.append(line); prev = line
    sx, sy = w // 320, h // 200
    return [[bytes(rows[y * sy][x * sx * bpp:x * sx * bpp + 3]) for x in range(320)] for y in range(200)]


def dos_shot(name, call, rebuild):
    d = os.path.join(OUT, name); os.makedirs(d, exist_ok=True)
    src = open(os.path.join(root, 'src', 'game', 'UWEDIT.C'), encoding='latin1').read()
    if call not in src: sys.exit(f'portshot.py: UWEDIT.C has no {call}')
    held = src.replace(call, call + '\n    for (;;) ;', 1)
    key = hashlib.sha1(held.encode('latin1')).hexdigest()
    exe, png, stamp = os.path.join(d, 'link', 'UW2.EXE'), os.path.join(d, 'dos.png'), os.path.join(d, 'source.sha1')
    if not rebuild and os.path.exists(png) and os.path.exists(stamp) and open(stamp).read() == key:
        return png
    open(os.path.join(d, 'UWEDIT.C'), 'w', encoding='latin1', newline='').write(held)
    subprocess.run(['node', os.path.join(here, 'tcc.mjs'), os.path.join(d, 'obj'), '-mm -1 -G -O -Y -d',
                    os.path.join(d, 'UWEDIT.C')], check=True)
    subprocess.run([PY, os.path.join(here, 'link.py'), '--mod', '--out', os.path.join(d, 'link'),
                    '--obj', 'UWEDIT=' + os.path.join(d, 'obj', 'UWEDIT.OBJ')], check=True)
    subprocess.run(['node', os.path.join(here, 'rungame.mjs'), exe, os.path.join(d, ''), 'w:6000', 's:dos'], check=True)
    open(stamp, 'w').write(key)
    return png


def main(argv):
    rebuild = '--rebuild' in argv
    exe = os.path.join(root, 'build', 'port', 'uw2port')
    if not os.path.exists(exe): sys.exit('portshot.py: build the port first (make port)')
    home = os.path.join(OUT, 'home')
    shutil.rmtree(home, ignore_errors=True)
    cmd = [exe, '--data', DATA, '--home', home, '--hidden', '--exit-on-halt', '--exit-after', '30000']
    for name, call, flip in SCREENS: cmd += ['--shot-at-flip', f'{flip}:' + os.path.join(OUT, name, 'port.png')]
    for name, call, flip in SCREENS: os.makedirs(os.path.join(OUT, name), exist_ok=True)
    subprocess.run(cmd)
    bad = 0
    for name, call, flip in SCREENS:
        port = os.path.join(OUT, name, 'port.png')
        if not os.path.exists(port):
            print(f'{name}: the port never showed it (flip {flip})'); bad += 1; continue
        a, b = load(port), load(dos_shot(name, call, rebuild))
        diff = sum(1 for y in range(200) for x in range(320) if a[y][x] != b[y][x])
        print(f'{name} ({call}): {diff} of 64000 pixels differ  ({os.path.relpath(port, root)} against DOS)')
        bad += diff != 0
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
