/* vidmode.c: replaces part of src/gfx/VIDMODE.ASM (seg003 module 5), the VGA itself: setting
   mode X (SetVideoMode), the mode's size and pages (_283D, _286C), the row table Ytab (_2AED),
   the edge masks (_2B44), page flips (_2ABE, _2AC1) and the display start (_295F), the window
   and its edge guards (_31E7, _2C44, _2BF9), pens (_3195, _31BE), the solid span writer
   (_2D83), the copies and saves of spans (_2F18, _2DF3, _2F96), plotting and reading pixels
   (_3094, _30AC, _30FB), the virtual screen (_2977, _2A0D), and the bitmap routines' clipping
   and row records (_21D4, _21ED, _2214, _2250, L2296, L2373, L243F). Each is written from the
   assembly, register for register where it matters; the comments give the assembly's labels.
   VIDMODE.ASM's header has the data map. y counts up from the bottom of the screen. The rest
   of the module (the concave polygon fill, the other span writers, _222B, _2242, _2B62, _30E3,
   the vertical lines _3121 and _3164) is the translation's, gfx/vidmode_x.c (docs/PORT.md, "One
   implementation per routine"). */
#include <stdio.h>
#include "grlib.h"

/* _2B7A, is there a VGA: the port always has one. */

/* SetVideoMode (_2B9B): mode 13h, then unchained: chain-4 off, odd/even off, bit mask and map
   mask all on, all 256 KB cleared, byte addressing. */
static void set_video_mode(void)
{
    unsigned i;
    vga_set_mode(0x13);
    vga_outb(SC_INDEX, 4);
    vga_outb(SC_DATA, (uint8_t)((vga_inb(SC_DATA) & 0xF7) | 4));
    vga_outb(GC_INDEX, 5);
    vga_outb(GC_DATA, vga_inb(GC_DATA) & 0xEF);
    vga_outb(GC_INDEX, 6);
    vga_outb(GC_DATA, vga_inb(GC_DATA) & 0xFD);
    vga_outw(GC_INDEX, 0xFF08);
    vga_outw(SC_INDEX, 0x0F02);
    for (i = 0; i < 0x10000; i++) vga_write((uint16_t)i, 0);
    vga_outb(CRTC_INDEX, 0x14);
    vga_outb(CRTC_INDEX + 1, vga_inb(CRTC_INDEX + 1) & 0xBF);
    vga_outb(CRTC_INDEX, 0x17);
    vga_outb(CRTC_INDEX + 1, vga_inb(CRTC_INDEX + 1) | 0x40);
    vga_outb(SC_INDEX, 2);
}

/* _2B53: four bytes from si, 80 times, to di */
static void edge_table(uint16_t si, uint16_t di)
{
    int i, k;
    for (i = 0; i < 0x50; i++)
        for (k = 0; k < 4; k++) SETB(di++, B(si + k));
}

/* _283D, init_graphics */
void seg003_0272_283D(void)
{
    SETW(0x36A2, dseg062_62a6[0x110] | dseg062_62a6[0x111] << 8);
    SETW(0x36A4, 0xFFFF);
    set_video_mode();
    vga_outb(CRTC_INDEX, 9);
    SETB(0x2D35, vga_inb(CRTC_INDEX + 1));
    edge_table(0x2D2C, 0x3B64);     /* _2B44 */
    edge_table(0x2D31, 0x3CA8);
    seg003_0272_286C();
}

/* _286C: apply mode 3DEA (unless it is the current one, 2CD8). */
void seg003_0272_286C(void)
{
    uint16_t bx = W(0x3DEA), ax, page;
    int i;
    if (bx == W(0x2CD8)) return;
    SETW(0x2CD8, bx);
    ax = (uint16_t)(bx << 1);
    bx = (uint16_t)(ax * 3);
    for (i = 0; i < 6; i++) SETB(0x3DEC + i, B(0x2CDE + bx + i));
    for (i = 0; i < 6; i++) dseg062_62a6[0xC38 + i] = B(0x3DEA + i);
    vga_outb(CRTC_INDEX, 9);
    if (ax == 0) vga_outb(CRTC_INDEX + 1, B(0x2D35));
    else vga_outb(CRTC_INDEX + 1, vga_inb(CRTC_INDEX + 1) & 0xE0);
    SETW(0x3DF4, 0);
    SETW(0x3DFA, 0);
    SETW(0x3DF8, W(0x3DEC));
    SETW(0x3DF6, W(0x3DEE));
    seg003_0272_31E7(0x3DF4);
    SETW(0x36A8, (uint16_t)((uint16_t)(W(0x3DEC) + 1) >> 2));
    ax = (uint16_t)((uint16_t)(W(0x3DEE) + 1) * W(0x36A8));
    page = ax;
    ax = (uint16_t)((ax + 0xFF) & 0xFF00);
    SETW(0x2CD4, ax);
    ax = (uint16_t)(ax + page);
    SETW(0x2CD6, ax);
    ax = (uint16_t)(ax + W(0x36A8));
    SETW(0x4106, ax);
    SETW(0x410A, ax);
    seg003_0272_2AED(0);
    seg003_0272_31BE();
    seg003_0272_2ABE();
    /* nullsub_2 */
    seg003_0272_3BE2();
}

/* _295F: the display start (CRTC 0Ch, 0Dh) at the drawing page's highest row. */
void seg003_0272_295F(void)
{
    uint16_t ax = W(0x36AC + (uint16_t)(W(0x3DEE) << 1));
    vga_outw(CRTC_INDEX, (uint16_t)((ax & 0xFF00) | 0x0C));
    vga_outw(CRTC_INDEX, (uint16_t)((ax << 8) | 0x0D));
}

/* _2ABE, grPageFlip: show the drawing page, then _2AC1. _2AC1, grSoftPageFlip: keep the
   current rows as the other page's (3840), and draw on the other page from now on. */
void seg003_0272_2ABE(void)
{
    seg003_0272_295F();
    seg003_0272_2AC1();
}

void seg003_0272_2AC1(void)
{
    uint16_t n = (uint16_t)(W(0x3DEE) + 1), i, ax, bx;
    for (i = 0; i < n; i++) SETW(0x3840 + 2 * i, W(0x36AC + 2 * i));
    ax = (uint16_t)(W(0x2CD4) - W(0x36AC + (uint16_t)(W(0x3DEE) << 1)));
    bx = (uint16_t)(-(uint16_t)(ax << 1) + W(0x2CD4));
    SETW(0x410C, bx);
    seg003_0272_2AED(ax);
}

/* _2AED: Ytab from AX at the highest row, each lower row 36A8 more, ten rows a pass (so up to
   nine rows below row 0 are written too, as the assembly does). */
void seg003_0272_2AED(uint16_t ax)
{
    uint16_t di = (uint16_t)(0x36AC + (uint16_t)(W(0x3DEE) << 1)), bx = W(0x36A8);
    int16_t cx = (int16_t)(W(0x3DEE) + 1);
    int k;
    do {
        for (k = 0; k < 10; k++) {
            SETW(di, ax);
            di -= 2;
            ax = (uint16_t)(ax + bx);
        }
        cx = (int16_t)(cx - 10);
    } while ((uint16_t)(cx + 10) > 10);
    SETW(0x2D1C, 0xFFFF);
}

/* _2C44: take the old window's edge guards out. */
static void unguard(void)
{
    uint16_t bx = (uint16_t)(W(0x3DF4) - 1), bp = (uint16_t)(0x3B64 + bx);
    SETB(bp, B(0x2D2C + (bx & 3)));
    bx = (uint16_t)(W(0x3DF8) + 1);
    bp = (uint16_t)(0x3CA8 + bx);
    SETB(bp, B(0x2D31 + (bx & 3)));
    bx = (uint16_t)(W(0x3DF6) << 1);
    SETW(0x36AE + bx, (uint16_t)(W(0x36AC + bx) - W(0x36A8)));
    bx = (uint16_t)(W(0x3DFA) << 1);
    SETW(0x36AA + bx, (uint16_t)(W(0x36AC + bx) + W(0x36A8)));
}

/* _2BF9: guard the new window: the rows just above and below it point at the spare area
   (2CD6), the masks just left and right of it are partial. */
static void guard(void)
{
    uint16_t bx, bp, ax;
    bx = (uint16_t)(0x36AE + (uint16_t)(W(0x3DF6) << 1));
    ax = W(0x2CD6);
    SETW(bx, ax);
    bx = (uint16_t)(0x36AA + (uint16_t)(W(0x3DFA) << 1));
    SETW(bx, ax);
    bx = (uint16_t)(W(0x3DF4) - 1);
    bp = (uint16_t)(0x3B64 + bx);
    SETB(bp, B(0x2D2D + (bx & 3)));
    bx = (uint16_t)(W(0x3DF8) + 1);
    bp = (uint16_t)(0x3CA8 + bx);
    SETB(bp, B(0x2D30 + (bx & 3)));
}

/* _31E7, set_the_window: SI = left, top, right, bottom. Also copied to FD71:0C30 for the 3D
   renderer. */
void seg003_0272_31E7(uint16_t si)
{
    uint16_t w[4];
    int i;
    unguard();
    for (i = 0; i < 4; i++) w[i] = W(si + 2 * i);
    for (i = 0; i < 4; i++) SETW(0x3DF4 + 2 * i, w[i]);
    guard();
    for (i = 0; i < 4; i++) {
        dseg062_62a6[0xC30 + 2 * i] = (uint8_t)w[i];
        dseg062_62a6[0xC31 + 2 * i] = (uint8_t)(w[i] >> 8);
    }
}

/* _3195, set_the_color: pen AX's colour (4110, the colour in 4111), its second word (4116)
   and, for a mode below 14h, its span writer (4112). */
void seg003_0272_3195(uint16_t ax)
{
    uint16_t bx;
    if (ax & 0x8000) return;
    bx = (uint16_t)(ax << 1);
    SETW(0x4110, W(0x21E + bx));
    SETW(0x4116, W(0x434 + bx));
    bx = W(0x8 + bx);
    if (bx >= 0x14) return;
    SETW(0x4112, (uint16_t)(W(0x2CEE + bx) - W(0x5046)));
}

/* _31BE, init_colors: each pen's set-up routine (2D04, by mode), then the video memory limit
   4108 from DI. */
void seg003_0272_31BE(void)
{
    uint16_t bp, bx;
    int cx;
    for (cx = (0x21E - 8) >> 1, bp = 0; cx; cx--, bp += 2) {
        bx = W(0x8 + bp);
        if (bx < 0x14) seg003_call(W(0x2D04 + bx), 0);
    }
    SETW(0x4108, 0xFFFF);
}

/* _2D83: fill the spans at SI with the colour in 4111 (FM solid_ylr). A span is y, left x,
   right x; the list ends with a y whose top bit is set. Each span writes its left and right
   partial groups with the edge masks and the whole groups between with all four planes. */
void seg003_0272_2D83(uint16_t si)
{
    uint8_t bl = B(0x4111), al, bh, t;
    uint16_t ax, di, bp;
    int16_t cx;
    for (;;) {
        ax = W(si); si += 2;
        if (ax & 0x8000) return;
        ax = (uint16_t)(ax << 1);
        di = W(si); si += 2;
        bp = W(si); si += 2;
        bh = B(0x3CA8 + bp);
        al = B(0x3B64 + di);
        cx = (int16_t)bp;
        bp = ax;
        di = (uint16_t)((int16_t)di >> 2);
        cx = (int16_t)((cx >> 2) - (int16_t)di);
        if (cx <= 0) {
            if (cx == 0) {
                di = (uint16_t)(di + W(0x36AC + bp));
                vga_outb(SC_DATA, al & bh);
                vga_write(di, bl);
                continue;
            }
            /* L2D91: the right end's group is left of the left end's */
            t = (uint8_t)~(uint8_t)(bh >> 1);
            bh = (uint8_t)~(uint8_t)(al << 1);
            al = t;
            di = (uint16_t)(di + cx);
            cx = (int16_t)-cx;
        }
        vga_outb(SC_DATA, al);
        di = (uint16_t)(di + W(0x36AC + bp));
        vga_write(di++, bl);
        cx--;
        vga_outb(SC_DATA, 0x0F);
        while (cx-- > 0) vga_write(di++, bl);
        vga_outb(SC_DATA, bh);
        vga_write(di++, bl);
    }
}

/* _21D4 and the labels after it (VIDMODE.ASM): the bitmap routines. Each chooses its transfer
   (0DC2) and builds the row records: L2296 for a bitmap (show, fbshow, _5025, _511C), L2373 for
   vcopyfb and vcopy, L243F for vcopy upwards.
   In: AX, BX the position, DI and SI the source's offset and segment, CX the width, DX the
   height, 0DC6 and 0DC8 the offsets into the source. */
void seg003_l2296(int16_t ax, int16_t bx, uint16_t di, uint16_t si, int16_t cx, int16_t dx)
{
    uint16_t src_off = di, rec, rows, stride, srcoff, first;
    int16_t bp, s, width;
    uint32_t prod;
    SETW(0x55EC, si);
    SETW(W(0x141B), W(W(0x141B)) & 0x7FFF);
    bp = (int16_t)W(0x0DC6);
    di = W(0x0DC8);
    stride = (uint16_t)cx;
    cx = (int16_t)(cx - bp);
    dx = (int16_t)(dx - (int16_t)di);
    if (B(0x0DC4)) {
        s = (int16_t)(ax + cx - 1 - SW(0x3DF4));
        if (s < 0) return;
        s = (int16_t)(SW(0x3DF4) - ax);
        if (s > 0) { ax = (int16_t)(ax + s); cx = (int16_t)(cx - s); bp = (int16_t)(bp + s); }
        s = (int16_t)(SW(0x3DF8) - ax);
        if (s < 0) return;
        s++;
        if (cx > s) cx = s;
        s = (int16_t)(bx - dx + 1 - SW(0x3DF6));
        if (s > 0) return;
        s = (int16_t)(SW(0x3DF6) - bx);
        if (s < 0) { di = (uint16_t)(di - s); dx = (int16_t)(dx + s); bx = (int16_t)(bx + s); }
        s = (int16_t)(bx - SW(0x3DFA));
        if (s <= 0) return;
        if (dx > s) dx = s;
    }
    /* L230E */
    width = cx;
    rows = (uint16_t)dx;
    if (!rows) port_halt("show: no rows (loop would run 65536 times over the record table)");
    rec = (uint16_t)(((uint16_t)(0xC7 - bx) << 3) + 0x0DCB);
    {
        int16_t right = (int16_t)(width + ax - 1);
        int16_t left = ax;
        prod = (uint32_t)(uint16_t)di * stride;
        if (W(0x55EC) == 0xA000) {
            srcoff = (uint16_t)(((uint16_t)bp >> 2) + ((uint16_t)((uint16_t)prod + 3) >> 2) + src_off);
            stride = (uint16_t)((stride + 3) >> 2);
        } else {
            srcoff = (uint16_t)((uint16_t)bp + (uint16_t)prod + src_off);
        }
        first = rec;
        while (rows--) {
            SETW(rec + 2, left);
            SETW(rec + 4, right);
            SETW(rec + 6, srcoff);
            srcoff = (uint16_t)(srcoff + stride);
            rec += 8;
        }
    }
    SETW(rec, W(rec) | 0x8000);
    SETW(0x141B, rec);
    seg003_call(W(0x0DC2), first);
}

/* L2373: whole groups of four pixels, (AX, BX) w CX by DX rows, the records' last word DI and
   on by the groups a row (vcopyfb, 0DCA 0) or the screen's stride (vcopy, 0DCA 1) */
void seg003_l2373(int16_t ax, int16_t bx, int16_t cx, int16_t dx, uint16_t di)
{
    int16_t si, bp;
    uint16_t groups, rec, first;
    cx = (int16_t)(cx + ax - 1);
    ax = (int16_t)(ax & 0xFFFC);
    cx = (int16_t)((cx & ~3) + 4 - ax);
    groups = (uint16_t)cx >> 2;
    if (B(0x0DC4)) {
        si = SW(0x3DF6);
        bp = (int16_t)(bx - dx + 1);
        if (si < bp) return;
        si = (int16_t)(si - bx);
        if (si < 0) {
            dx = (int16_t)(dx + si);
            bx = (int16_t)(bx + si);
            di = (uint16_t)(di + (uint16_t)(-si) * (uint16_t)((uint16_t)cx >> 2));
        }
        si = (int16_t)(bx - SW(0x3DFA));
        if (si <= 0) return;
        if (dx > si) dx = si;
        si = (int16_t)(SW(0x3DF8) - ax);
        if (si < 0) return;
        si++;
        if (cx > si) cx = si;
        si = SW(0x3DF4);
        bp = (int16_t)(ax + cx - 1);
        if (si > bp) return;
        si = (int16_t)(si - ax);
        if (si > 0) {
            ax = (int16_t)(ax + si);
            cx = (int16_t)(cx - si);
            bp = (int16_t)((-si) & 3);
            si = (int16_t)(si + bp);
            di = (uint16_t)(di + (uint16_t)(si >> 2));
        }
    }
    /* L23F7 */
    SETW(W(0x141B), W(W(0x141B)) & 0x7FFF);
    cx = (int16_t)(cx + ax - 1);
    rec = (uint16_t)(((uint16_t)(0xC7 - bx) << 3) + 0x0DCB);
    first = rec;
    bp = (int16_t)(B(0x0DCA) ? W(0x36A8) : groups);
    do {
        SETW(rec + 2, ax);
        SETW(rec + 4, cx);
        SETW(rec + 6, di);
        di = (uint16_t)(di + (uint16_t)bp);
        rec += 8;
    } while (--dx);
    SETW(rec, W(rec) | 0x8000);
    SETW(0x141B, rec);
    seg003_call(W(0x0DC2), first);
}

/* L243F: vcopy upwards: the records from the bottom row up with their own y, the destination
   a screen row further up each time; the y words put back after */
static void l243f(int16_t ax, int16_t bx, int16_t cx, int16_t dx, uint16_t di)
{
    uint16_t si, bp, n;
    cx = (int16_t)(cx + ax - 1);
    ax = (int16_t)(ax & 0xFFFC);
    cx = (int16_t)((cx & ~3) + 4 - ax);
    SETW(W(0x141B), W(W(0x141B)) & 0x7FFF);
    cx = (int16_t)(cx + ax - 1);
    bx = (int16_t)(bx - dx + 1);
    si = 0x0DCB;
    bp = W(0x36A8);
    n = (uint16_t)dx;
    do {
        SETW(si, bx);
        SETW(si + 2, ax);
        SETW(si + 4, cx);
        SETW(si + 6, di);
        bx++;
        di = (uint16_t)(di - bp);
        si += 8;
    } while (--n);
    SETW(si, W(si) | 0x8000);
    SETW(0x141B, si);
    seg003_call(W(0x0DC2), 0x0DCB);
    si = 0x0DCB;
    bp = 0xC7;
    n = (uint16_t)dx;
    do { SETW(si, bp); bp--; si += 8; } while (--n);
}

/* _21D4, show: a linear bitmap to the screen, _5372 or (Transparency) _5467 */
void seg003_0272_21D4(int16_t ax, int16_t bx, uint16_t di, uint16_t si, int16_t cx, int16_t dx)
{
    SETW(0x0DC2, B(0x0DC5) ? 0x5467 : 0x5372);
    seg003_l2296(ax, bx, di, si, cx, dx);
}

/* _21ED: a bitmap in video memory (SI A000h): _56E3 transparent, else _568B when x is a
   multiple of 4 (the planes line up), else _5578 */
void seg003_0272_21ED(int16_t ax, int16_t bx, uint16_t di, uint16_t si, int16_t cx, int16_t dx)
{
    SETW(0x0DC2, B(0x0DC5) ? 0x56E3 : (ax & 3) ? 0x5578 : 0x568B);
    seg003_l2296(ax, bx, di, si, cx, dx);
}

/* _2214, fbshow: into the frame buffer, _6F8 or (Transparency) _729 */
void seg003_0272_2214(int16_t ax, int16_t bx, uint16_t di, uint16_t si, int16_t cx, int16_t dx)
{
    SETW(0x0DC2, B(0x0DC5) ? 0x729 : 0x6F8);
    seg003_l2296(ax, bx, di, si, cx, dx);
}

/* _2250, vcopy: screen to screen, (AX, BX) to (SI, DI), CX by DX: upwards (L243F) when the
   destination row is above, right to left (_586D) on the same rows to the right */
void seg003_0272_2250(int16_t ax, int16_t bx, int16_t cx, int16_t dx, uint16_t si, uint16_t di)
{
    int up = 0;
    if (bx != (int16_t)di) {
        SETW(0x0DC2, 0x581B);
        up = bx > (int16_t)di;
    } else
        SETW(0x0DC2, ax > (int16_t)si ? 0x581B : 0x586D);
    if (up) di = (uint16_t)(di - (uint16_t)dx + 1);
    di = (uint16_t)(W(0x36AC + (uint16_t)(di << 1)) + (si >> 2));
    SETB(0x0DCA, 1);
    if (up) l243f(ax, bx, cx, dx, di);
    else seg003_l2373(ax, bx, cx, dx, di);
}

/* ---- the other span writers (VIDMODE.ASM after _2D83's ret) ------------------------------
   Each takes the span list at SI (y, left x, right x; ended by a y with its top bit set) and
   does to video memory what the assembly does, out for out: the copies set the graphics
   controller's bit mask (index 8, where the library leaves it) to 0 so that each write stores
   the latches the read before it loaded, and put it back to FFh. */

/* one span's groups: the left edge mask al, the right edge mask ah, the first and last group;
   the shape of every span writer's head (the L2D91/L2F2B case swaps the masks when the right
   end's group is left of the left end's). Returns the count of groups between, or -1 for a
   span within one group (with al already the joint mask). */
static int span_groups(uint16_t left, uint16_t right, uint8_t *al, uint8_t *ah, uint16_t *di)
{
    int16_t cx;
    uint8_t t;
    *al = B(0x3B64 + left);
    *ah = B(0x3CA8 + right);
    *di = (uint16_t)((int16_t)left >> 2);
    cx = (int16_t)(((int16_t)right >> 2) - (int16_t)*di);
    if (cx > 0) return cx;
    if (cx == 0) { *al &= *ah; return -1; }
    t = (uint8_t)~(uint8_t)(*ah >> 1);
    *ah = (uint8_t)~(uint8_t)(*al << 1);
    *al = t;
    *di = (uint16_t)(*di + cx);
    return -cx;
}

/* a latched copy of the spans (masks and all) from off bytes away, on the row table rows: _2F18
   (36AC, from the other page at 410C) */
static void copy_rows(uint16_t bx, uint16_t rows, uint16_t off)
{
    uint8_t al, ah;
    uint16_t y, di;
    int cx;
    vga_outb(GC_DATA, 0);
    for (;;) {
        y = W(bx);
        if (y & 0x8000) break;
        cx = span_groups(W(bx + 2), W(bx + 4), &al, &ah, &di);
        bx += 6;
        di = (uint16_t)(di + W(rows + (uint16_t)(y << 1)));
        vga_outb(SC_DATA, al);
        vga_write(di, vga_read((uint16_t)(di + off)));
        di++;
        if (cx < 0) continue;
        cx--;
        vga_outb(SC_DATA, 0x0F);
        while (cx-- > 0) { vga_write(di, vga_read((uint16_t)(di + off))); di++; }
        vga_outb(SC_DATA, ah);
        vga_write(di, vga_read((uint16_t)(di + off)));
    }
    vga_outb(GC_DATA, 0xFF);
}

/* _2F18 (FM copy_ylr): copy the spans from the other page. */
static void seg003_0272_2F18(uint16_t si)
{
    copy_rows(si, 0x36AC, W(0x410C));
}

/* whole groups of each span between the screen and a run of video memory: _2DF3 saves them to
   4116 on, _2F96 restores them from there */
static void groups(uint16_t bx, int how)
{
    uint16_t bp, s, d, buf = W(0x4116);
    int16_t cx;
    vga_outb(GC_DATA, 0);
    vga_outb(SC_DATA, 0x0F);
    for (;;) {
        bp = W(bx);
        if (bp & 0x8000) break;
        bp = (uint16_t)(bp << 1);
        s = (uint16_t)((int16_t)W(bx + 2) >> 2);
        cx = (int16_t)(((int16_t)W(bx + 4) >> 2) - (int16_t)s + 1);
        bx += 6;
        s = (uint16_t)(s + W(0x36AC + bp));
        if (how == 0) { d = buf; buf = (uint16_t)(buf + cx); }      /* save: screen to buf */
        else { d = s; s = buf; buf = (uint16_t)(buf + cx); }        /* restore */
        while (cx-- > 0) { vga_write(d, vga_read(s)); d++; s++; }
    }
    vga_outb(GC_DATA, 0xFF);
}

static void seg003_0272_2DF3(uint16_t si) { groups(si, 0); }
static void seg003_0272_2F96(uint16_t si) { groups(si, 1); }

/* _3094 (plot): (AX, BX) in the colour 4111 if inside the window; _30AC without the test. */
void seg003_0272_30AC(uint16_t ax, uint16_t bx)
{
    uint16_t di = ax;
    vga_outb(SC_DATA, (uint8_t)(B(0x3B64 + di) & B(0x3CA8 + di)));
    di = (uint16_t)((di >> 2) + W(0x36AC + (uint16_t)(bx << 1)));
    vga_write(di, B(0x4111));
}

void seg003_0272_3094(int16_t ax, int16_t bx)
{
    if (bx > SW(0x3DF6) || bx < SW(0x3DFA) || ax > SW(0x3DF8) || ax < SW(0x3DF4)) return;
    seg003_0272_30AC((uint16_t)ax, (uint16_t)bx);
}

/* _30FB: the pixel at (AX, BX), read through the read map (graphics controller 4), leaving
   the index at 8. */
uint16_t seg003_0272_30FB(uint16_t ax, uint16_t bx)
{
    uint16_t di = ax;
    uint8_t v;
    vga_outw(GC_INDEX, (uint16_t)(((ax & 3) << 8) | 4));
    di = (uint16_t)((di >> 2) + W(0x36AC + (uint16_t)(bx << 1)));
    v = vga_read(di);
    vga_outb(GC_INDEX, 8);
    return v;
}


/* ---- the virtual screen (_2977, vscreen_focus) ------------------------------------------ */

/* _2A8F: the line compare, ten bits: CRTC 18h, and the overflow bits in 7 (bit 4) and 9 (bit 6). */
static void line_compare(uint16_t ax)
{
    uint8_t v;
    vga_outb(CRTC_INDEX, 0x18);
    vga_outb(CRTC_INDEX + 1, (uint8_t)ax);
    vga_outb(CRTC_INDEX, 7);
    v = vga_inb(CRTC_INDEX + 1);
    vga_outb(CRTC_INDEX + 1, (ax & 0x100) ? (uint8_t)(v | 0x10) : (uint8_t)(v & 0xEF));
    vga_outb(CRTC_INDEX, 9);
    v = vga_inb(CRTC_INDEX + 1);
    vga_outb(CRTC_INDEX + 1, (ax & 0x200) ? (uint8_t)(v | 0x40) : (uint8_t)(v & 0xBF));
}

/* _2B2C: Ytab rows CX up to BX, the highest getting AX and each lower one 36A8 more. */
static void rows_from(uint16_t ax, uint16_t bx, uint16_t cx)
{
    uint16_t n = (uint16_t)(bx - cx + 1);
    bx = (uint16_t)(bx << 1);
    do {
        SETW(0x36AC + bx, ax);
        ax = (uint16_t)(ax + W(0x36A8));
        bx -= 2;
    } while (--n);
}

/* the retrace waits: for the start of the next vertical retrace, and for its end */
static void retrace_start(void)
{
    while (!(vga_inb(INPUT_STATUS) & 8)) ;
    while (vga_inb(INPUT_STATUS) & 8) ;
}

/* _2977: a virtual screen AX pixels wide (CRTC offset). BX and CX are kept at 2CDC and 2CDA;
   CX, when not negative, is the split: the scrolling part starts that many rows into video
   memory and the line compare puts row 0 of memory below it (the assembly's header calls BX
   the split line, but the code tests and uses CX). */
void seg003_0272_2977(uint16_t ax, uint16_t bx, uint16_t cx)
{
    uint16_t start, di, top;
    int16_t split = (int16_t)cx;
    uint8_t v;
    ax = (uint16_t)(ax >> 2);
    SETW(0x36A8, ax);
    SETW(0x2CDC, bx);
    SETW(0x2CDA, cx);
    vga_outb(CRTC_INDEX, 0x13);
    vga_outb(CRTC_INDEX + 1, (uint8_t)(ax >> 1));
    vga_outb(0x3C0, 0x30);
    v = vga_inb(0x3C1);
    vga_outb(0x3C0, (uint8_t)(v | 0x20));
    if (split < 0) {
        line_compare(0x3FF);
        split = 0;
        start = 0;
    } else {
        start = (uint16_t)(W(0x36A8) * (uint16_t)split);
        line_compare((uint16_t)(((uint16_t)(W(0x3DEE) - (uint16_t)split) << 1) - 1));
    }
    /* L29C8: the display start, at the end of a retrace */
    retrace_start();
    vga_outb(CRTC_INDEX, 0x0C);
    vga_outb(CRTC_INDEX + 1, (uint8_t)(start >> 8));
    vga_outb(CRTC_INDEX, 0x0D);
    vga_outb(CRTC_INDEX + 1, (uint8_t)start);
    di = (uint16_t)split;
    top = W(0x3DEE);
    if (start) {
        rows_from((uint16_t)(start + W(0x36A8)), top, (uint16_t)split);
        rows_from(0, (uint16_t)(di - 1), 0);
    } else
        rows_from(start, top, (uint16_t)split);
}

/* _2A0D, vscreen_focus: scroll the virtual screen to (AX, BX). */
void seg003_0272_2A0D(uint16_t ax, uint16_t bx)
{
    uint16_t t = ax, start, top;
    uint32_t prod;
    ax = bx;
    bx = t;
    ax = (uint16_t)(-(uint16_t)(ax + 1) + W(0x2CDC));
    if ((int16_t)W(0x2CDA) >= 0) ax = (uint16_t)(ax + W(0x2CDA));
    prod = (uint32_t)ax * W(0x36A8);
    start = (uint16_t)((uint16_t)prod + (bx >> 2));
    top = W(0x3DEE);
    if ((int16_t)W(0x2CDA) >= 0) {
        rows_from((uint16_t)(start + W(0x36A8)), top, (uint16_t)(W(0x2CDA) + 1));
        rows_from(0, W(0x2CDA), 0);
        start = (uint16_t)(start + W(0x36A8));
    } else
        rows_from(start, top, 0);
    retrace_start();
    vga_outb(CRTC_INDEX, 0x0C);
    vga_outb(CRTC_INDEX + 1, (uint8_t)(start >> 8));
    vga_outb(CRTC_INDEX, 0x0D);
    vga_outb(CRTC_INDEX + 1, (uint8_t)start);
    while (vga_inb(INPUT_STATUS) & 8) ;
    while (!(vga_inb(INPUT_STATUS) & 8)) ;
    vga_outb(0x3C0, 0x33);
    vga_outb(0x3C0, (uint8_t)((bx & 3) << 1));
}

/* the span writers for seg003_call (grcore.c) */
void seg003_span(uint16_t off, uint16_t si)
{
    switch (off) {
    case 0x2D83: seg003_0272_2D83(si); return;
    case 0x2DF3: seg003_0272_2DF3(si); return;
    case 0x2F18: seg003_0272_2F18(si); return;
    case 0x2F96: seg003_0272_2F96(si); return;
    default: break;
    }
    {
        char why[80];
        snprintf(why, sizeof why, "seg003 span writer at %04X has no C yet", off);
        port_halt(why);
    }
}
