/* grlibf.c: replaces part of src/gfx/GRLIBF.ASM (seg003 module 6): clear_window (_3423) and
   urectangle (_342E), which turn a filled rectangle into one span per row and run the current
   span writer (4112) on them, the whole screen (_326D) and the copies between the pages (_327D,
   _328F), and the unclipped lines (_3324, _34AE). clip_rect (_33BE) is in grcore.c with
   rectangle. The rest of the module (the clipped lines, ubox, the pixel, the polygons) is the
   translation's, gfx/grlibf_x.c (docs/PORT.md, "One implementation per routine"). */
#include "grlib.h"

/* _3423, clear_window: urectangle over the whole clip window (3DF4 left, 3DF6 top, 3DF8
   right, 3DFA bottom). */
void seg003_0272_3423(void)
{
    seg003_0272_342E(SW(0x3DF4), SW(0x3DF6), SW(0x3DF8), SW(0x3DFA));
}

/* _342E, urectangle: (AX, BX)-(CX, DX), corners in either order. The span table at 44CE has
   a 6-byte record per screen row, its y already filled in (row 0C7h - y first); the x range
   goes into the rows from the top one down, the record after the last gets the end mark, and
   the previous end mark (49AC) is cleared first. */
void seg003_0272_342E(int16_t ax, int16_t bx, int16_t cx, int16_t dx)
{
    int16_t t;
    uint16_t si, di, first, n;
    if (bx <= dx) { t = bx; bx = dx; dx = t; }
    if (ax >= cx) { t = ax; ax = cx; cx = t; }
    SETW(0x415A, ax);
    SETW(0x415C, cx);
    n = (uint16_t)(bx - dx + 1);
    si = W(0x49AC);
    SETW(si, W(si) & 0x7FFF);
    di = (uint16_t)(0x44CE + (uint16_t)(0xC7 - bx) * 6);
    first = di;
    while (n--) {
        di += 2;
        SETW(di, W(0x415A));
        SETW(di + 2, W(0x415C));
        di += 4;
    }
    SETW(0x49AC, di);
    SETW(di, W(di) | 0x8000);
    seg003_call(W(0x4112), first);
}

/* _326D: the whole screen, (0, 0) to (3DEC, 3DEE), through urectangle with the current span
   writer. */
void seg003_0272_326D(void)
{
    seg003_0272_342E(0, SW(0x3DEE), SW(0x3DEC), 0);
}

/* _327D, copy_visible_to_hidden: _326D with span writer 52CC (_2F18, the copy from the other
   page), the old writer put back after. */
void seg003_0272_327D(void)
{
    uint16_t old = W(0x4112);
    SETW(0x4112, 0x52CC);
    seg003_0272_326D();
    SETW(0x4112, old);
}

/* _328F, copy_hidden_to_visible: flip the pages (grSoftPageFlip), copy, flip back. */
void seg003_0272_328F(void)
{
    seg003_0272_2AC1();
    seg003_0272_327D();
    seg003_0272_2AC1();
}

/* _3324, uvline: x AX, y from BX to DX. The solid and copy writers have direct vertical-line
   routines (52B1, 52B4); any other writer gets one span a row at 2D42. */
void seg003_0272_3324(int16_t ax, int16_t bx, int16_t dx)
{
    int16_t t;
    uint16_t di, cx;
    if (W(0x5046) == 0) {
        if (W(0x4112) == 0x52C9) { seg003_0272_3164((uint16_t)ax, (uint16_t)bx, (uint16_t)dx); return; }
        if (W(0x4112) == 0x52CC) { seg003_0272_3121((uint16_t)ax, (uint16_t)bx, (uint16_t)dx); return; }
    }
    if (bx <= dx) { t = bx; bx = dx; dx = t; }
    cx = (uint16_t)(bx - dx + 1);
    di = 0x2D42;
    while (cx--) {
        SETW(di, bx);
        SETW(di + 2, ax);
        SETW(di + 4, ax);
        di += 6;
        bx--;
    }
    SETW(di, 0xFFFF);
    seg003_call(W(0x4112), 0x2D42);
}

/* _34AE, uhline: one span, y BX, x from AX to CX in either order. */
void seg003_0272_34AE(int16_t ax, int16_t bx, int16_t cx)
{
    int16_t t;
    if (ax >= cx) { t = ax; ax = cx; cx = t; }
    SETW(0x414A, bx);
    SETW(0x414C, ax);
    SETW(0x414E, cx);
    seg003_call(W(0x4112), 0x414A);
}
