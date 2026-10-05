"""Compare a linked EXE with UW2.EXE: header, relocations, resident image segment by segment
(the overlay manager's segment table names the segments), and the FBOV area overlay by overlay.

    python3 tools/exediff.py [build/LINK/out/UW2.EXE] [--all]

Prints the first differences in each region that differs (--all: every differing byte run).
Exit status 0 when the files are identical."""
import sys, os, struct
here = os.path.dirname(os.path.abspath(__file__)); root = os.path.dirname(here)
EXE = os.environ.get('UW2_EXE', os.path.expanduser('~/UWGOG/UW2/UW2.EXE'))
w16 = lambda b, i: struct.unpack_from('<H', b, i)[0]

def parse(d):
    h = struct.unpack_from('<14H', d, 0)
    hdr = h[4] * 16
    end = (h[2] - 1) * 512 + h[1] if h[1] else h[2] * 512
    rel = [struct.unpack_from('<HH', d, h[12] + 4 * i) for i in range(h[3])]
    fb = d[end:end + 16]
    segtab = nseg = None
    if fb[:4] == b'FBOV': segtab, nseg = struct.unpack_from('<II', d, end + 8)
    segs = []
    if segtab:
        for i in range(nseg):
            para, mx, fl, mn = struct.unpack_from('<4H', d, segtab + 8 * i)
            segs.append((i, para, mn, mx, fl))
    return dict(h=h, hdr=hdr, end=end, rel=rel, segtab=segtab, nseg=nseg, segs=segs)

def runs(a, b, lo, hi):
    """Differing byte runs in [lo, hi)."""
    out = []; i = lo
    while i < hi:
        if i >= len(a) or i >= len(b) or a[i] != b[i]:
            j = i
            while j < hi and (j >= len(a) or j >= len(b) or a[j] != b[j]): j += 1
            out.append((i, j)); i = j
        else: i += 1
    return out

def main():
    args = [x for x in sys.argv[1:] if not x.startswith('--')]
    mine_path = args[0] if args else os.path.join(root, 'build', 'LINK', 'out', 'UW2.EXE')
    allruns = '--all' in sys.argv
    A = open(EXE, 'rb').read(); B = open(mine_path, 'rb').read()
    pa, pb = parse(A), parse(B)
    same = True
    print(f'sizes: original {len(A):#x}, linked {len(B):#x}')
    names = ['signature', 'last page', 'pages', 'relocations', 'header paras', 'min alloc', 'max alloc', 'SS', 'SP',
             'checksum', 'IP', 'CS', 'reloc table', 'overlay no']
    for k, n in enumerate(names):
        if pa['h'][k] != pb['h'][k]:
            print(f'header {n}: {pa["h"][k]:#06x} vs {pb["h"][k]:#06x}'); same = False
    if A[0x1C:pa['h'][12]] != B[0x1C:pb['h'][12]]:
        print(f'header bytes 0x1C..: {A[0x1C:pa["h"][12]].hex()} vs {B[0x1C:pb["h"][12]].hex()}'); same = False
    # relocations: as a set of load-image addresses, then their order
    la = [s * 16 + o for o, s in pa['rel']]; lb = [s * 16 + o for o, s in pb['rel']]
    sa, sb = set(la), set(lb)
    if sa != sb:
        same = False
        print(f'relocations: {len(sa - sb)} only in the original, {len(sb - sa)} only in the linked EXE')
        for x in sorted(sa - sb)[:10]: print(f'  only original: load {x:#07x} (file {x + pa["hdr"]:#07x})')
        for x in sorted(sb - sa)[:10]: print(f'  only linked:   load {x:#07x} (file {x + pb["hdr"]:#07x})')
    elif la != lb:
        same = False
        moved = sum(1 for x, y in zip(la, lb) if x != y)
        print(f'relocations: the same {len(sa)} addresses, {moved} entries in a different order')
    if pa['rel'] != pb['rel'] and sa == sb and la == lb:
        print('relocations: same addresses written as different segment:offset pairs'); same = False
    # the resident image, segment by segment, between the headers and the load module's end
    hdr = pa['hdr']
    if pb['hdr'] != hdr: print('header sizes differ: image comparison is offset'); same = False
    bounds = []
    for i, para, mn, mx, fl in pa['segs']:
        lo = hdr + para * 16 + mn
        bounds.append((lo, i, para))
    bounds.sort()
    image = runs(A, B, hdr, max(pa['end'], pb['end']))
    if image:
        same = False
        shown = {}
        for lo, hi in image:
            seg = max((b for b in bounds if b[0] <= lo), default=(hdr, -1, 0))
            key = seg[1]
            shown.setdefault(key, []).append((lo, hi))
        print(f'resident image: {sum(h - l for l, h in image)} bytes differ in {len(image)} runs, in {len(shown)} segments')
        for key, rs in shown.items():
            para = next((b[2] for b in bounds if b[1] == key), 0)
            print(f'  segment #{key} ({para:04X}): {len(rs)} runs, {sum(h - l for l, h in rs)} bytes;' +
                  ' '.join(f' {l:#x}..{h:#x}' for l, h in (rs if allruns else rs[:4])))
            if not allruns:
                l, h = rs[0]
                print(f'    original {A[l:min(h, l + 16)].hex(" ")}\n    linked   {B[l:min(h, l + 16)].hex(" ")}')
    # FBOV: the overlays, using the original's stubs (entry 91.. are stubs of ovr091..)
    fa, fb = A[pa['end']:], B[pb['end']:]
    if fa != fb:
        same = False
        print(f'FBOV: {len(fa):#x} vs {len(fb):#x} bytes')
        stubs = [(s[1], s[0]) for s in pa['segs'] if s[4] == 3]
        offs = []
        for para, i in stubs:
            st = hdr + para * 16
            offs.append((struct.unpack_from('<I', A, st + 4)[0], i, w16(A, st + 8), w16(A, st + 10)))
        offs.sort()
        rs = runs(fa, fb, 0, max(len(fa), len(fb)))
        per = {}
        for lo, hi in rs:
            ov = max((o for o in offs if o[0] <= lo), default=(0, -1, 0, 0))
            per.setdefault(ov[1], []).append((lo, hi, ov))
        for i, rr in per.items():
            lo, hi, ov = rr[0]
            what = 'code' if lo - ov[0] < ov[2] else 'fixups'
            print(f'  ovr{i:03d}: {len(rr)} runs, {sum(h - l for l, h, _ in rr)} bytes, first at +{lo - ov[0]:#x} ({what})'
                  f'\n    original {fa[lo:min(hi, lo + 16)].hex(" ")}\n    linked   {fb[lo:min(hi, lo + 16)].hex(" ")}')
    print('IDENTICAL' if same and A == B else 'DIFFERENT')
    return 0 if A == B else 1

if __name__ == '__main__':
    sys.exit(main())
