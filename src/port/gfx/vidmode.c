/* vidmode.c: replaces part of src/gfx/VIDMODE.ASM (seg003 module 5), the VGA itself: setting
   mode X (SetVideoMode), the mode's size and pages (_283D, _286C), the row table Ytab (_2AED),
   the edge masks (_2B44), page flips (_2ABE, _2AC1) and the display start (_295F), the window
   and its edge guards (_31E7, _2C44, _2BF9), pens (_3195, _31BE), the solid span writer
   (_2D83), and show's clipping and row records (_21D4, L2296). Each is written from the
   assembly, register for register where it matters; the comments give the assembly's labels.
   VIDMODE.ASM's header has the data map. y counts up from the bottom of the screen. */
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

/* _21D4, show: choose the transfer by Transparency (_5372 opaque, _5467 transparent, in
   GRLIBL.ASM) and run L2296: clip to the window when ShowClip is set, write one 8-byte record
   per row (y, left x, right x, source offset) at 0DCB, end it, and run the transfer.
   In: AX, BX the position, DI and SI the source's offset and segment, CX the width, DX the
   height, 0DC6 and 0DC8 the offsets into the source. */
void seg003_0272_21D4(int16_t ax, int16_t bx, uint16_t di, uint16_t si, int16_t cx, int16_t dx)
{
    uint16_t src_off = di, rec, rows, stride, srcoff, first;
    int16_t bp, s, width;
    uint32_t prod;
    SETW(0x0DC2, B(0x0DC5) ? 0x5467 : 0x5372);
    /* L2296 */
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
