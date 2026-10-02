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
