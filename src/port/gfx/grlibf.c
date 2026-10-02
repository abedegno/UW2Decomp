/* grlibf.c: replaces part of src/gfx/GRLIBF.ASM (seg003 module 6): clear_window (_3423) and
   urectangle (_342E), which turn a filled rectangle into one span per row and run the current
   span writer (4112) on them. clip_rect (_33BE) is in grcore.c with rectangle. */
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
   writer; _326A sets the colour AX first. */
void seg003_0272_326D(void)
{
    seg003_0272_342E(0, SW(0x3DEE), SW(0x3DEC), 0);
}

void seg003_0272_326A(uint16_t ax)
{
    seg003_0272_3195(ax);
    seg003_0272_326D();
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

/* _32B4 (FM pixel): (AX, BX) inside the window: with the solid writer straight to the pixel
   writer (upixel, _30AC), unless a concave window (5046) excludes it; otherwise a one-span
   list at 414A through the span writer. */
void seg003_0272_32B4(int16_t ax, int16_t bx)
{
    uint16_t di;
    if (bx > SW(0x3DF6) || bx < SW(0x3DFA) || ax > SW(0x3DF8) || ax < SW(0x3DF4)) return;
    if (W(0x4112) == 0x52C9) {
        /* L329A: the concave window's test compares the same word twice, so it never excludes
           (as in the assembly) */
        seg003_0272_30AC((uint16_t)ax, (uint16_t)bx);
        return;
    }
    di = 0x414A;
    SETW(di, bx);
    SETW(di + 2, ax);
    SETW(di + 4, ax);
    seg003_call(W(0x4112), 0x414A);
}

/* _32E2: clip a vertical line (x AX, y from BX to DX) to the window: 0 when it is wholly
   outside (the assembly drops its caller's return address), else BX >= DX inside. */
static int vclip(int16_t ax, int16_t *bx, int16_t *dx)
{
    int16_t t, si;
    if (ax > SW(0x3DF8) || ax < SW(0x3DF4)) return 0;
    if (*bx <= *dx) { t = *bx; *bx = *dx; *dx = t; }
    si = SW(0x3DFA);
    if (*bx < si) return 0;
    if (*dx <= si) *dx = si;
    si = SW(0x3DF6);
    if (*dx > si) return 0;
    if (*bx >= si) *bx = si;
    return 1;
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

/* _3321, vline: clipped, then uvline. _331A: clipped, then the copy line (52B4). */
void seg003_0272_3321(int16_t ax, int16_t bx, int16_t dx)
{
    if (vclip(ax, &bx, &dx)) seg003_0272_3324(ax, bx, dx);
}

void seg003_0272_331A(int16_t ax, int16_t bx, int16_t dx)
{
    if (vclip(ax, &bx, &dx)) seg003_0272_3121((uint16_t)ax, (uint16_t)bx, (uint16_t)dx);
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

/* _347F, hline: clipped to the window, then as uhline. */
void seg003_0272_347F(int16_t ax, int16_t bx, int16_t cx)
{
    int16_t t, dx;
    if (bx > SW(0x3DF6) || bx < SW(0x3DFA)) return;
    if (ax >= cx) { t = ax; ax = cx; cx = t; }
    dx = SW(0x3DF4);
    if (cx < dx) return;
    if (ax <= dx) ax = dx;
    dx = SW(0x3DF8);
    if (ax > dx) return;
    if (cx >= dx) cx = dx;
    SETW(0x414A, bx);
    SETW(0x414C, ax);
    SETW(0x414E, cx);
    seg003_call(W(0x4112), 0x414A);
}

/* _3371, ubox: the outline of (AX, BX)-(CX, DX): two uhlines and two uvlines from the corners
   kept at 4152..4158. */
void seg003_0272_3371(int16_t ax, int16_t bx, int16_t cx, int16_t dx)
{
    SETW(0x4152, ax);
    SETW(0x4154, bx);
    SETW(0x4156, cx);
    SETW(0x4158, dx);
    seg003_0272_34AE(SW(0x4152), SW(0x4154), SW(0x4156));
    seg003_0272_34AE(SW(0x4152), SW(0x4158), SW(0x4156));
    seg003_0272_3324(SW(0x4152), SW(0x4154), SW(0x4158));
    seg003_0272_3324(SW(0x4156), SW(0x4154), SW(0x4158));
}
