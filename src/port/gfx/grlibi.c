/* grlibi.c: replaces part of src/gfx/GRLIBI.ASM (seg003 module 9): the text masks (_3BE2) and
   setup_font (_3BFD). The text drawing itself is still the generated stubs. */
#include "grlib.h"

/* _3BE2: the 256-entry table at 4A26 from the 16 plane masks at 4A16: for each high nibble,
   its mask then each low nibble's, two bytes an entry. */
void seg003_0272_3BE2(void)
{
    uint16_t si = 0x4A16, di = 0x4A26;
    int i, k;
    for (i = 0; i < 16; i++) {
        uint8_t hi = B(si++);
        for (k = 0; k < 16; k++) {
            SETB(di++, hi);
            SETB(di++, B(0x4A16 + k));
        }
    }
}

/* _3BFD, setup_font: the glyph pointer table at 4D18 for characters 0 to 127, from 1C54h in
   steps of (4D0E << (4D10 - 1)) + 4D08, and the font's height and row width for string_width
   (2D3C, 2D3E, 2D40). The font header (cur_font) is at 4D08. */
void seg003_0272_3BFD(void)
{
    uint16_t bx = W(0x4D0E), ax = 0x1C54, di = 0x4D18;
    uint8_t cl = (uint8_t)(W(0x4D10) - 1);
    int i;
    bx = (uint16_t)(bx << (cl & 31));
    bx = (uint16_t)(bx + W(0x4D08));
    for (i = 0; i < 0x80; i++) {
        SETW(di, ax);
        di += 2;
        ax = (uint16_t)(ax + bx);
    }
    SETW(0x2D3C, W(0x4D0E));
    SETW(0x2D3E, (uint16_t)(W(0x4D10) - 1));
    SETW(0x2D40, W(0x4D12));
}

/* ---- text (GRLIBI.ASM's _3B1E .. _43C5) ------------------------------------------------- */

/* _43C5, string_width: the string at SI (in seg_370D): the sum of its characters' widths, the
   byte just past each glyph's rows (height << (rowwidth - 1) bytes in), stopping at the 0 or
   after 54 characters. No range check: a character from 80h up reads past the 128 glyph
   pointers, as in DOS. */
uint16_t seg003_0272_43C5(uint16_t si)
{
    uint16_t bx = (uint16_t)(W(0x2D3C) << (W(0x2D3E) & 31)), dx = 0;
    int cx;
    for (cx = 0x36; cx; cx--) {
        uint8_t al = B(si++);
        if (!al) break;
        dx = (uint16_t)(dx + B((uint16_t)(W(0x4D18 + 2 * al) + bx)));
    }
    return dx;
}

/* the rasteriser at 4225h (4D14): the string at SI into the one-bit buffer at 2D44 (4D06), 4D04
   bytes a row (the width in bytes, with the x mod 4 shift, plus one) and font height rows, the
   buffer (and the bytes either side) cleared first; each glyph's rows ORed in at the running bit
   position, a glyph wider than 8 in two halves (ch = 8 while the second is due). Characters
   outside 20h..7Ah are drawn as '?'. */
static void raster(uint16_t si)
{
    uint16_t dx, cx, bp, ax, di, bx, gs, entry;
    uint32_t prod;
    uint8_t cl, ch, al;
    int rows, r;
    dx = seg003_0272_43C5(si);
    dx = (uint16_t)(dx + (W(0x4A0E) & 3));
    dx = (uint16_t)(dx >> 3);
    cx = (uint16_t)(dx + 1);
    SETW(0x4D04, cx);
    bp = cx;
    prod = (uint32_t)W(0x4D0E) * cx;
    ax = (uint16_t)prod;
    cx = (uint16_t)(ax + 1);
    SETW(0x4D02, cx);
    di = 0x2D43;
    dx = (uint16_t)(di + 1);
    SETW(0x4D06, dx);
    SETW(0x4D00, (uint16_t)(ax + dx));
    cx = (uint16_t)((uint16_t)(cx + 1) >> 1);
    while (cx--) { SETW(di, 0); di += 2; }
    /* the unrolled row loop is entered so as to run the font height's rows (L43A5) */
    entry = W(0x4D0E);
    rows = entry >= 1 && entry <= 16 ? entry : 0;
    if (!rows) port_halt("string rasteriser: a font height outside 1..16 enters L43A5's table out of range");
    bx = si;
    cl = (uint8_t)(W(0x4A0E) & 3);
    ch = 0;
    for (;;) {
        /* L42A1 */
        al = B(bx);
        if (!al) return;
        if ((int8_t)al < 0x20 || al > 0x7A) al = 0x3F;
        gs = W(0x4D18 + 2 * al);
        if (ch == 8) gs++;
        di = dx;
        for (r = 0; r < rows; r++) {
            uint16_t v = B(gs);
            gs = (uint16_t)(gs + 1 + W(0x2D3E));
            v = (uint16_t)((v >> (cl & 15)) | (v << ((16 - (cl & 15)) & 15)));
            SETW(di, W(di) | v);
            di = (uint16_t)(di + bp);
        }
        if (ch == 8) gs--;
        al = B(gs);
        /* L427E */
        if ((int8_t)al > 8) {
            if (ch == 8) { al = (uint8_t)(al - ch); ch = 0; }
            else { al = 8; ch = al; }
        }
        cl = (uint8_t)(cl + al);
        if (cl >= 8) { dx++; cl = (uint8_t)(cl - 8); }
        if (al != ch) bx++;
    }
}

/* _3C61, the single-colour blitter: the bit buffer to the screen at (4A0E, 4A10), y the top
   row: each buffer byte is eight pixels, two four-pixel groups whose plane masks come from the
   table at 4A26; each mask goes to the map mask and the colour is stored. */
static void blit(uint16_t colour)
{
    uint16_t di, bx, bp, si;
    uint8_t ch, cl;
    int k;
    SETW(0x4A0C, colour);
    di = (uint16_t)((W(0x4A0E) >> 2) + W(0x36AC + (uint16_t)(W(0x4A10) << 1)));
    bx = (uint16_t)(W(0x4D04) << 1);
    bp = (uint16_t)(W(0x36A8) - bx);
    SETW(0x4CFA, W(0x4C8E + bx));
    if (W(0x4D04) == 0 || W(0x4D04) > 0x35) port_halt("text blitter: a row width outside its unrolled loop");
    ch = B(0x4D0E);
    si = W(0x4D06);
    cl = B(0x4A0C);
    do {
        for (k = 0; k < W(0x4D04); k++) {
            uint8_t bits = B(si);
            uint16_t m = W(0x4A26 + 2 * bits);
            si++;
            vga_outb(SC_DATA, (uint8_t)m);
            vga_write(di++, cl);
            vga_outb(SC_DATA, (uint8_t)(m >> 8));
            vga_write(di++, cl);
        }
        di = (uint16_t)(di + bp);
    } while (--ch);
}

/* the text routines' two vectors, 4D14 (the rasteriser) and 4D16 (the blitter), by the
   offsets DOS keeps there */
static void text_raster(uint16_t si)
{
    if (W(0x4D14) != 0x4225) port_halt("the text rasteriser vector (4D14) is not 4225h");
    raster(si);
}

static void text_blit(uint16_t colour)
{
    if (W(0x4D16) == 0x3C61) { blit(colour); return; }
    port_halt("the bitmap text blitter (L4126, through _222B) has no C yet");
}

/* _4150 (FM shift_left_1): the text's x back one pixel (a byte back in the buffer when x was
   a multiple of 4) and y up one, and the whole buffer, 4D02 + 1 bytes ending at 4D00, shifted
   one bit left as one bit string, the lowest address the most significant (rcl from the end
   down, 53 bytes a pass through _4186). */
static void shift_left_1(void)
{
    uint16_t si, n, total;
    unsigned carry = 0;
    if (!(W(0x4A0E) & 3)) {
        SETW(0x4D06, W(0x4D06) - 1);
        SETW(0x4A0E, W(0x4A0E) - 4);
    }
    SETW(0x4A10, W(0x4A10) + 1);
    SETW(0x4A0E, W(0x4A0E) - 1);
    total = (uint16_t)(W(0x4D02) + 1);
    /* the passes: whole ones of 53, then the rest through the table at 4C24, whose entry 0 is
       0 and whose entry 53 is the byte before _4186 (dec sp): DOS would go astray on either */
    n = total;
    while (n > 0x35) n -= 0x35;
    if (n == 0 || n == 0x35) port_halt("shift_left_1: a buffer length that enters the table at 4C24 badly");
    si = W(0x4D00);
    for (n = total; n; n--, si--) {
        uint8_t b = B(si);
        unsigned c = b >> 7;
        SETB(si, (uint8_t)((b << 1) | carry));
        carry = c;
    }
}

/* _3B36 (string_to_screen), _3B3E (unclipped), _3B8F (shadowed), _3BC8 (shadowed, unclipped):
   the string at SI, top-left at (AX, BX). The clipped entries draw nothing unless the whole
   box, the font's height down from y and x, is inside the window. */
static int text_box(uint16_t ax, uint16_t bx)
{
    int16_t v;
    SETW(0x4A0E, ax);
    SETW(0x4A10, bx);
    v = SW(0x4A10);
    if (v < SW(0x3DFA) || v > SW(0x3DF6)) return 0;
    v = (int16_t)(v - SW(0x4D0E) + 1);
    SETW(0x4A14, v);
    if (v < SW(0x3DFA) || v > SW(0x3DF6)) return 0;
    v = SW(0x4A0E);
    if (v < SW(0x3DF4) || v > SW(0x3DF8)) return 0;
    return 1;
}

void seg003_0272_3B36(uint16_t ax, uint16_t bx, uint16_t si)
{
    SETW(0x4D16, 0x3C61);
    if (!text_box(ax, bx)) return;
    text_raster(si);
    text_blit(W(0x2D3A));
}

void seg003_0272_3B3E(uint16_t ax, uint16_t bx, uint16_t si)
{
    SETW(0x4D16, 0x3C61);
    SETW(0x4A0E, ax);
    SETW(0x4A10, bx);
    text_raster(si);
    text_blit(W(0x2D3A));
}

static void shadowed(uint16_t si)
{
    SETW(0x4A0E, W(0x4A0E) + 1);
    SETW(0x4A10, W(0x4A10) - 1);
    text_raster(si);
    blit(W(0x2D36));
    shift_left_1();
    blit(W(0x2D3A));
}

void seg003_0272_3B8F(uint16_t ax, uint16_t bx, uint16_t si)
{
    if (!text_box(ax, bx)) return;
    shadowed(si);
}

void seg003_0272_3BC8(uint16_t ax, uint16_t bx, uint16_t si)
{
    SETW(0x4A0E, ax);
    SETW(0x4A10, bx);
    shadowed(si);
}
