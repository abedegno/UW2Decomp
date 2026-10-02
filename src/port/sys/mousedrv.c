/* mousedrv.c: replaces src/sys/MOUSEDRV.ASM (seg021 module 10) and the DOS mouse driver under
   it (int 33h). The driver's state is built from the platform layer's pointer events: the
   position, in the driver's 640 by 200 range for mode 13h, the buttons, and the motion counters
   in mickeys, at the driver's default of 8 mickeys per 8 pixels across and 16 per 8 down, so a
   pointer moving one screen pixel gives two mickeys either way. The game reads only the motion
   and the buttons (MOUSE.C keeps its own cursor), so a touch screen drives it as a mouse does.

   MOUSEDRV.ASM's routines are kept: _6C8 resets the driver and stores the motion divisor (200)
   in FD71:0360 and MouseOn (FD71:0362); _6E7 reads the motion and scales it by 100 / 200;
   _703 gives the buttons. */
#include <stdatomic.h>
#include "port.h"
#include "plat.h"

#define D dseg062_62a6
#define W(o) ((int16_t)(D[(o)] | D[(o) + 1] << 8))
#define SETW(o, v) port_setw(&D[(o)], (uint16_t)(v))

static _Atomic int mick_x, mick_y, pos_x, pos_y, buttons;
static float frac_x, frac_y;

void mouse_event(const PlatPointer *ev)
{
    float mx = ev->dx * 2.0f + frac_x, my = ev->dy * 2.0f + frac_y;
    int ix = (int)mx, iy = (int)my, x = (int)(ev->x * 2.0f), y = (int)ev->y;
    frac_x = mx - (float)ix;
    frac_y = my - (float)iy;
    atomic_fetch_add(&mick_x, ix);
    atomic_fetch_add(&mick_y, iy);
    if (x < 0) x = 0;
    if (x > 639) x = 639;
    if (y < 0) y = 0;
    if (y > 199) y = 199;
    atomic_store(&pos_x, x);
    atomic_store(&pos_y, y);
    atomic_store(&buttons, (int)(ev->buttons & 3));
}

/* int 33h: AX the function; returns its results in the registers, as the driver did. */
int mouse_int33(uint16_t *ax, uint16_t *bx, uint16_t *cx, uint16_t *dx)
{
    int x, y;
    switch (*ax) {
    case 0x00:                          /* reset: installed, two buttons */
        *ax = 0xFFFF;
        *bx = 2;
        return 1;
    case 0x03:                          /* position and buttons */
        *bx = (uint16_t)atomic_load(&buttons);
        *cx = (uint16_t)atomic_load(&pos_x);
        *dx = (uint16_t)atomic_load(&pos_y);
        return 1;
    case 0x0B:                          /* motion since the last call, in mickeys */
        x = atomic_exchange(&mick_x, 0);
        y = atomic_exchange(&mick_y, 0);
        *cx = (uint16_t)(int16_t)x;
        *dx = (uint16_t)(int16_t)y;
        return 1;
    default:
        port_log("int 33h function %04Xh not emulated\n", *ax);
        return 0;
    }
}

/* _6C8 (initmouse): reset the driver; a driver that answers sets 0360 and 0362 to 200. */
void seg021_22FD_6C8(void)
{
    uint16_t ax = 0, bx = 0, cx = 0, dx = 0;
    mouse_int33(&ax, &bx, &cx, &dx);
    if (ax) ax = 0xC8;
    SETW(0x360, ax);
    SETW(0x362, ax);
}

/* _6F8: AX * 100 / [0360], as imul and idiv do it (a 16-bit quotient). */
static int16_t scale(int16_t v)
{
    int32_t p = (int32_t)v * 100;
    int16_t d = W(0x360);
    if (d == 0) port_fatal("mouse: divide by zero in _6F8 (no mouse driver)");
    return (int16_t)(p / d);
}

/* _6E7: the motion (function 0Bh), each axis scaled by _6F8. */
void Read_Mouse_Motion_seg021_6E7(int16_t *x, int16_t *y)
{
    uint16_t ax = 0x0B, bx = 0, cx = 0, dx = 0;
    mouse_int33(&ax, &bx, &cx, &dx);
    *x = scale((int16_t)cx);
    *y = scale((int16_t)dx);
}

/* _703: the buttons (function 3): left and right, bits 0 and 1. */
int seg021_22FD_703(void)
{
    uint16_t ax = 3, bx = 0, cx = 0, dx = 0;
    mouse_int33(&ax, &bx, &cx, &dx);
    return bx & 3;
}
