/* grentry.c: replaces src/gfx/GRENTRY.ASM (seg003 module 3), the 3D view's frame buffer: a
   linear buffer, one byte a pixel, in stdat's segment (its segment at 370D:0958, its row
   offsets at 095C), cleared, filled, dimmed and lit, copied to the planar screen, and drawn into
   by its own span writers, including the 3D view's Gouraud spans. Each is written from the
   assembly; the comments give its labels (GRENTRY.ASM's header has the data map).

   Two pieces of the assembly modify its own code, and are state here instead: setup_frame_buf
   writes a ret into the unrolled copier _888 after one plane's share of the frame's width
   (copier_bytes), and seg004 points the smooth-span vectors L0BC3 and L0BC5 at the solid or
   the translucent span routines (smooth_same, smooth_vary, by their DOS offsets). */
#include <stdio.h>
#include "grlib.h"

extern unsigned char lightabs[];                /* seg004:6D3E to the segment's end (mem/fardata.c) */
#define LIGHTABS_REACH 0x92C2
extern unsigned char seg004_0849_6D20[];        /* PGCACHE.ASM: [0] the smooth spans' base colour */
extern unsigned char seg049_3EE2[];             /* the frame buffer's segment: stdat's */
extern unsigned char cXfer[];                   /* SCALEBM.ASM: XFER.DAT's colour tables */

#define FB ((uint8_t *)port_mk_fp(W(0x958), 0))
#define ROW(y) W(0x95C + (uint16_t)((y) << 1))

static int copier_ret = -1;            /* the byte offset of the ret written into _888, or -1 */
static uint16_t span_y;                 /* L0BC1: the current smooth span's y */
static uint16_t smooth_same = 0x0BC7;   /* L0BC3 */
static uint16_t smooth_vary = 0x0C93;   /* L0BC5 */

/* seg004 sets the smooth-span vectors (INTERP.ASM's opcodes D6h and D8h, SMOOTH.ASM) */
void seg003_smooth_vectors(uint16_t same, uint16_t vary)
{
    smooth_same = same;
    smooth_vary = vary;
}

/* _6E2, clear_fbuf: _6E4 with colour 0. _6E4, fill_fbuf (cFillFB): 34E9h words of AL from
   offset 2 of the frame buffer's segment (seg049_3EE2, stdat's). */
void seg003_0272_6E4(uint8_t al)
{
    uint8_t *fb = seg049_3EE2;
    uint16_t i;
    for (i = 0; i < 0x34E9 * 2; i++) fb[2 + i] = al;
}

void seg003_0272_6E2(void) { seg003_0272_6E4(0); }

/* _6F8 (fbuf_draw_ylrpp): linear bitmap rows (8-byte records; source segment 55EC) into the
   frame buffer; _729 (fbuf_draw_ylrpp_x) skips colour 0. */
static void fb_rows(uint16_t si, int transparent)
{
    uint8_t *fb = FB, *src = port_mk_fp(W(0x55EC), 0);
    uint16_t ax, di, cx, s;
    if (!fb || !src) port_fatal("fbshow: a segment in no region");
    for (;;) {
        ax = W(si); si += 2;
        if (ax & 0x8000) return;
        di = ROW(ax);
        ax = W(si); si += 2;
        di = (uint16_t)(di + ax);
        cx = (uint16_t)(W(si) - ax + 1); si += 2;
        s = W(si); si += 2;
        do {
            uint8_t v = src[s++];
            if (!transparent || v) fb[di] = v;
            di++;
        } while (--cx);
    }
}

void seg003_0272_6F8(uint16_t si) { fb_rows(si, 0); }
void seg003_0272_729(uint16_t si) { fb_rows(si, 1); }

/* _764, cDimFB(CL): every pixel from 6AA6h down to 1 through lightabs row CL. */
void seg003_0272_764(uint16_t cx)
{
    uint8_t *fb = seg049_3EE2;
    uint16_t si = (uint16_t)(((cx & 0xFF) << 8) | (cx >> 8)), di, k;
    for (di = 0x6AA6; di; di--) {
        k = (uint16_t)(fb[di] + si);
        if (k >= LIGHTABS_REACH) port_halt("cDimFB: a shade row past seg004's end (DOS wraps in the segment)");
        fb[di] = lightabs[k];
    }
}

/* _788, cLiteFB(CX): every pixel from 69D2h down to 1 through cXfer + 300h, CX times. */
void seg003_0272_788(uint16_t cx)
{
    uint8_t *fb = seg049_3EE2;
    uint16_t si;
    do {
        for (si = 0x69D2; si; si--) fb[si] = cXfer[0x300 + fb[si]];
    } while (--cx);
}

/* _7A8, setup_frame_buf (cPlaceFB): BX = the width, CX = the height. Rows of width + 2 bytes
   from offset 2, the rows from the height up all at 69D4h, the last row in 0AF0, and the
   copier's ret moved to after the new width (when it is below 320). The width is compared with
   0AEE before 2 is taken off it again, so the copier is always re-patched. */
void seg003_0272_7A8(uint16_t bx, uint16_t cx)
{
    uint16_t di = 0x95C, dx = cx, ax = 2;
    bx = (uint16_t)(bx + 2);
    while (cx--) { SETW(di, ax); di += 2; ax = (uint16_t)(ax + bx); }
    cx = (uint16_t)(0xC8 - dx);
    SETW(0xAF0, (uint16_t)(dx - 1));
    while (cx--) { SETW(di, 0x69D4); di += 2; }
    if (bx == W(0xAEE)) return;
    if ((int16_t)W(0xAEE) < 0x140) copier_ret = -1;      /* the old ret back to movsb */
    bx = (uint16_t)(bx - 2);
    if ((int16_t)bx >= 0x140) return;
    if (bx & 3) port_halt("setup_frame_buf: a width not a multiple of 4 puts the ret inside an instruction");
    copier_ret = bx;
    SETW(0xAEE, bx);
}

/* _888: every fourth byte of a frame-buffer row to the screen, 80 times, or up to the ret */
static void copier(const uint8_t *fb, uint16_t si, uint16_t di)
{
    int n = copier_ret < 0 ? 80 : copier_ret / 4, i;
    for (i = 0; i < n; i++) {
        vga_write(di++, fb[si]);
        si = (uint16_t)(si + 4);
    }
}

/* _7F9, cFBtoScreen: from the frame's last row down to the window's bottom edge (3DFA), each
   row to the screen at A000:[94E] + [383A], 80 bytes a row, four passes with map masks 8, 2, 4
   and 1 from source offsets 3, 1, 2 and 0. */
void seg003_0272_7F9(void)
{
    uint8_t *fb = FB;
    int16_t bp = (int16_t)W(0xAF0);
    uint16_t bx = (uint16_t)(W(0x94E) + W(0x383A)), cx;
    do {
        cx = ROW(bp);
        vga_outb(SC_DATA, 8); copier(fb, (uint16_t)(cx + 3), bx);
        vga_outb(SC_DATA, 2); copier(fb, (uint16_t)(cx + 1), bx);
        vga_outb(SC_DATA, 4); copier(fb, (uint16_t)(cx + 2), bx);
        vga_outb(SC_DATA, 1); copier(fb, cx, bx);
        bx = (uint16_t)(bx + 0x50);
        bp--;
    } while (bp > SW(0x3DFA));
}

/* _9C9 (fake_fb_solid_ylr): fill each span (y, left, right, either order) with 4111. */
static void fb_solid(uint16_t si)
{
    uint8_t *fb = FB, al;
    uint16_t ax, di, cx;
    int16_t n;
    for (;;) {
        ax = W(si); si += 2;
        if (ax & 0x8000) return;
        di = ROW(ax);
        ax = W(si); si += 2;
        cx = ax;
        di = (uint16_t)(di + ax);
        ax = W(si); si += 2;
        n = (int16_t)(ax - cx + 1);
        al = B(0x4111);
        if (n >= 0) {
            while (n--) fb[di++] = al;
        } else {
            di = (uint16_t)(di + n);
            cx = (uint16_t)(1 - n);
            while (cx--) fb[di++] = al;
        }
    }
}

/* _9FC (fbuf_save_vylr): frame-buffer spans, widened to whole groups, to video memory from 4116,
   one plane at a time. */
static void fb_save(uint16_t bp)
{
    uint8_t *fb = FB, al;
    uint16_t bx, si, cx, di = W(0x4116), start, n;
    int p;
    for (;;) {
        bx = W(bp);
        if (bx & 0x8000) return;
        bx = (uint16_t)(bx << 1);
        si = (uint16_t)(W(bp + 2) & 0xFFFC);
        cx = (uint16_t)((W(bp + 4) & 0xFFFC) + 4);
        bp += 6;
        cx = (uint16_t)(cx - si);
        si = (uint16_t)(si + W(0x95C + bx));
        bx = cx;
        start = di;
        al = 1;
        for (p = 0; p < 4; p++) {
            if (p) { si = (uint16_t)(si - bx + 1); di = start; al = (uint8_t)(al << 1); }
            vga_outb(SC_DATA, al);
            for (n = (uint16_t)(bx >> 2); n; n--) { vga_write(di++, fb[si]); si = (uint16_t)(si + 4); }
        }
    }
}

/* _A7F (fbcopy_vylrpp, vcopyfb's transfer): frame-buffer rows (y, left, right, destination) to
   the planar screen, up to four passes of every fourth pixel from x on, each pass's plane from
   the edge tables at its first x; then the bit mask back to FFh. */
void seg003_0272_A7F(uint16_t bp)
{
    uint8_t *fb = FB;
    uint16_t si, bx, cx, di, n, k;
    int pass;
    for (;;) {
        bx = W(bp);
        if (bx & 0x8000) break;
        bx = (uint16_t)(bx << 1);
        si = W(0x95C + bx);
        bx = W(bp + 2);
        cx = W(bp + 4);
        di = W(bp + 6);
        si = (uint16_t)(si + bx);
        bp += 8;
        n = (uint16_t)(cx - bx + 1);
        for (pass = 0; pass < 4; pass++) {
            vga_outb(SC_DATA, (uint8_t)(B(0x3B64 + bx) & B(0x3CA8 + bx)));
            cx = (uint16_t)((uint16_t)(n + 3) >> 2);
            for (k = cx; k; k--) { vga_write(di++, fb[si]); si = (uint16_t)(si + 4); }
            di = (uint16_t)(di - cx);
            si = (uint16_t)(si - (uint16_t)(cx << 2) + 1);
            bx++;
            if (!(bx & 3)) di++;
            if (!--n) break;
        }
    }
    vga_outw(GC_INDEX, 0xFF08);
}

/* _B9B, fbuf_setcolor: pen AX's colour (4110) and second word (4116), and for a mode below 14h
   the frame buffer's span writer from the table at 0AF2. */
void seg003_0272_B9B(uint16_t ax)
{
    uint16_t bx;
    if (ax & 0x8000) return;
    bx = (uint16_t)(ax << 1);
    SETW(0x4110, W(0x21E + bx));
    SETW(0x4116, W(0x434 + bx));
    bx = W(0x8 + bx);
    if (bx >= 0x14) return;
    SETW(0x4112, W(0xAF2 + bx));
}

/* ---- the smooth spans (_C13 and the routines L0BC3 and L0BC5 point at) ------------------ */

/* lightabs[row][colour]: es:[bx + 6D3Eh] with BH the row; a row from 16 up reads seg004's code
   after the table, as in DOS (lightabs runs to the segment's end here) */
static uint8_t shade(uint8_t row, uint8_t colour)
{
    uint16_t k = (uint16_t)(row * 256 + colour);
    if (k >= LIGHTABS_REACH) port_halt("a shade row past seg004's end (DOS wraps in the segment)");
    return lightabs[k];
}

/* L0BC7 (smooth_span_same, solid): the span in one colour, lightabs[shade][base colour],
   through fake_fb_solid_ylr on the record itself, ended there by setting the top bit of the
   word after its right x (the record's left intensity) for the while */
static void span_same_solid(uint16_t si)
{
    SETB(0x4111, shade(B(0xB08), seg004_0849_6D20[0]));
    SETB(si - 3, B(si - 3) | 0x80);
    fb_solid((uint16_t)(si - 0x0A));
    SETB(si - 3, B(si - 3) & 0x7F);
}

/* 0BF2 (trans_span_same): the pixels already in the span through one lightabs row */
static void span_same_trans(uint16_t di, uint16_t cx)
{
    uint8_t *fb = FB, dh = B(0xB08);
    do { fb[di] = shade(dh, fb[di]); di++; } while (--cx);
}

/* L0C93 (do_filled_dither) and 0CD9 (do_trans_dither): two 8.8 intensity accumulators 40h
   apart, alternating pixel by pixel, which starting by the row's parity */
static void span_vary(uint16_t di, uint16_t count, uint16_t bx, uint16_t bp, int trans)
{
    uint8_t *fb = FB, base = seg004_0849_6D20[0];
    uint16_t cx = bx, dx = bx;
    bp = (uint16_t)(bp << 1);
    dx = (uint16_t)(dx - 0x40);
    cx = (uint16_t)(cx + 0x40);
    if (span_y & 1) { uint16_t t = cx; cx = dx; dx = t; }
    span_y >>= 1;
    for (;;) {
        fb[di] = shade((uint8_t)(dx >> 8), trans ? fb[di] : base);
        dx = (uint16_t)(dx + bp);
        di++;
        if (!--count) return;
        fb[di] = shade((uint8_t)(cx >> 8), trans ? fb[di] : base);
        cx = (uint16_t)(cx + bp);
        di++;
        if (!--count) return;
    }
}

/* _C13 (vga_smooth_ylrii): 10-byte records (y, left x, right x, left intensity, right
   intensity), the step per pixel formed as 8.8 with idiv and div as the assembly does */
void seg003_0272_C13(uint16_t si)
{
    uint16_t ax, di, dx, cx, bx, bp, t, step;
    int32_t q;
    for (;;) {
        ax = W(si); si += 2;
        span_y = ax;
        if (ax & 0x8000) return;
        di = ROW(ax);
        dx = W(si); si += 2;
        cx = W(si); si += 2;
        bx = W(si); si += 2;
        ax = W(si); si += 2;
        bp = cx;
        cx = (uint16_t)(cx - dx);
        if ((int16_t)cx < 0) {
            cx = (uint16_t)-cx;
            t = bp; bp = dx; dx = t;
            t = bx; bx = ax; ax = t;
        }
        SETW(0xB08, bx);
        SETW(0xB0A, dx);
        di = (uint16_t)(di + dx);
        cx++;
        ax = (uint16_t)(ax - bx);
        if (!ax) {
            if (smooth_same == 0x0BC7) span_same_solid(si);
            else if (smooth_same == 0x0BF2) span_same_trans(di, cx);
            else port_halt("smooth spans: an L0BC3 vector with no C");
            continue;
        }
        {
            int neg = 0;
            int16_t a = (int16_t)ax, c = (int16_t)cx;
            uint16_t rem;
            if (a < 0) {
                if (c >= 0) { a = (int16_t)-a; neg = 1; }
                else { a = (int16_t)-a; c = (int16_t)-c; }
            } else if (c < 0) { c = (int16_t)-c; neg = 1; }
            q = (int32_t)a / c;
            rem = (uint16_t)((int32_t)a % c);
            step = (uint16_t)(((uint8_t)q) << 8);
            step |= (uint16_t)((((uint32_t)rem << 16) / (uint16_t)c) >> 8) & 0xFF;
            if (neg) step = (uint16_t)-step;
            cx = (uint16_t)c;
        }
        bp = step;
        bx = (uint16_t)((B(0xB08) << 8) | 0x80);
        if (smooth_vary == 0x0C93) span_vary(di, cx, bx, bp, 0);
        else if (smooth_vary == 0x0CD9) span_vary(di, cx, bx, bp, 1);
        else port_halt("smooth spans: an L0BC5 vector with no C");
    }
}

/* the frame buffer's span writers for seg003_call (grcore.c) */
int seg003_fb_span(uint16_t off, uint16_t si)
{
    switch (off) {
    case 0x9C9: fb_solid(si); return 1;
    case 0x9FC: fb_save(si); return 1;
    case 0x6F8: seg003_0272_6F8(si); return 1;
    case 0x729: seg003_0272_729(si); return 1;
    case 0xA7F: seg003_0272_A7F(si); return 1;
    case 0xC13: seg003_0272_C13(si); return 1;
    default: return 0;
    }
}
