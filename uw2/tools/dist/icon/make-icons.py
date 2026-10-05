"""Make the port's icon files from the two SVG sources (docs/BUILDING.md, "Releases").

    python3 tools/dist/icon/make-icons.py       (needs node with puppeteer: npm install; iconutil on macOS)

uw2.svg is the icon, drawn for this project: a stylised ankh on a stone tile. uw2-small.svg
is the same simplified for 16 to 32 pixels. Written here, and committed, so that no build
needs a renderer:

  png/uw2-N.png     N = 16, 24, 32, 48, 64, 128, 256, 512, 1024 (16..32 from uw2-small.svg)
  uw2.ico           Windows: 16, 24, 32, 48, 64 and 256 (the program's resource, portbuild.py)
  uw2.icns          macOS: the .app's icon (iconutil, so only on macOS; package.py copies it)
  uw2.png           Linux: 256 px for the .desktop file and the AppImage (package.py)
  ../../../src/port/platform/sdl3/icon.h   48 px as RGBA bytes, for SDL_SetWindowIcon
"""
import os, sys, struct, zlib, shutil, subprocess, tempfile

here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(os.path.dirname(os.path.dirname(here)))
SMALL = (16, 24, 32)
LARGE = (48, 64, 128, 256, 512, 1024)
WINDOW = 48


def render(svg, outdir, sizes):
    subprocess.run(['node', os.path.join(here, 'render-icon.mjs'), os.path.join(here, svg), outdir] +
                   [str(s) for s in sizes], check=True, cwd=root)


def read_rgba(path):
    """An 8-bit RGBA, non-interlaced PNG (as Chrome writes them): (w, h, bytes)."""
    d = open(path, 'rb').read(); p = 8; idat = b''; w = h = 0
    while p < len(d):
        n = struct.unpack_from('>I', d, p)[0]; t = d[p + 4:p + 8]; b = d[p + 8:p + 8 + n]; p += 12 + n
        if t == b'IHDR':
            w, h, depth, ctype, _, _, inter = struct.unpack('>IIBBBBB', b)
            if (depth, ctype, inter) != (8, 6, 0): sys.exit(f'{path}: not 8-bit RGBA without interlace')
        elif t == b'IDAT': idat += b
    raw = zlib.decompress(idat); bpp = 4; stride = w * bpp; out = bytearray(); prev = bytearray(stride)
    for y in range(h):
        f = raw[y * (stride + 1)]; line = bytearray(raw[y * (stride + 1) + 1:(y + 1) * (stride + 1)])
        for x in range(stride):
            a = line[x - bpp] if x >= bpp else 0; b = prev[x]; c = prev[x - bpp] if x >= bpp else 0
            if f == 1: line[x] = (line[x] + a) & 255
            elif f == 2: line[x] = (line[x] + b) & 255
            elif f == 3: line[x] = (line[x] + (a + b) // 2) & 255
            elif f == 4:
                pa, pb, pc = abs(b - c), abs(a - c), abs(a + b - 2 * c)
                line[x] = (line[x] + (a if pa <= pb and pa <= pc else b if pb <= pc else c)) & 255
        out += line; prev = line
    return w, h, bytes(out)


def write_ico(path, pngs):
    """An .ico of PNG entries (Windows Vista and later read them at every size)."""
    data = [open(p, 'rb').read() for _, p in pngs]
    head = struct.pack('<HHH', 0, 1, len(pngs)); off = 6 + 16 * len(pngs); dirs = b''
    for (s, _), d in zip(pngs, data):
        dirs += struct.pack('<BBBBHHII', s % 256, s % 256, 0, 0, 1, 32, len(d), off); off += len(d)
    open(path, 'wb').write(head + dirs + b''.join(data))


def write_header(path, w, h, rgba):
    rows = [', '.join(f'0x{b:02x}' for b in rgba[i:i + 16]) for i in range(0, len(rgba), 16)]
    with open(path, 'w') as f:
        f.write('/* icon.h: written by tools/dist/icon/make-icons.py from uw2.svg; do not edit. The window\n'
                f'   icon (plat_sdl3.c, SDL_SetWindowIcon): {w} by {h} pixels, RGBA, rows from the top. */\n'
                f'#define PLAT_ICON_W {w}\n#define PLAT_ICON_H {h}\n'
                'static const unsigned char plat_icon_rgba[] = {\n    ' + ',\n    '.join(rows) + '\n};\n')


def main():
    tmp = tempfile.mkdtemp()
    try:
        render('uw2-small.svg', tmp, SMALL)
        render('uw2.svg', tmp, LARGE)
        pd = os.path.join(here, 'png'); os.makedirs(pd, exist_ok=True)
        png = {s: os.path.join(pd, f'uw2-{s}.png') for s in SMALL + LARGE}
        for s, p in png.items(): shutil.copy(os.path.join(tmp, f'{s}.png'), p)
        write_ico(os.path.join(here, 'uw2.ico'), [(s, png[s]) for s in (16, 24, 32, 48, 64, 256)])
        shutil.copy(png[256], os.path.join(here, 'uw2.png'))
        w, h, rgba = read_rgba(png[WINDOW])
        write_header(os.path.join(root, 'src', 'port', 'platform', 'sdl3', 'icon.h'), w, h, rgba)
        if shutil.which('iconutil'):
            iset = os.path.join(tmp, 'uw2.iconset'); os.makedirs(iset)
            for s in (16, 32, 128, 256, 512):
                shutil.copy(png[s], os.path.join(iset, f'icon_{s}x{s}.png'))
                shutil.copy(png[s * 2] if s * 2 in png else png[1024], os.path.join(iset, f'icon_{s}x{s}@2x.png'))
            subprocess.run(['iconutil', '-c', 'icns', iset, '-o', os.path.join(here, 'uw2.icns')], check=True)
        else:
            print('make-icons.py: no iconutil (macOS only); uw2.icns not remade')
        for f in sorted(os.listdir(here)) + ['png/' + f for f in sorted(os.listdir(pd))]:
            p = os.path.join(here, f)
            if os.path.isfile(p) and not f.endswith(('.py', '.mjs', '.svg')): print(f'{f}: {os.path.getsize(p)} bytes')
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


if __name__ == '__main__':
    main()
