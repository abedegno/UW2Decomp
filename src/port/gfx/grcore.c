/* grcore.c: replaces part of src/gfx/GRCORE.ASM (seg003 module 10): the graphics globals C
   reaches seg_370D through, and the C entry points the boot path calls. Each entry loads the
   C arguments into what the library routine takes, as the assembly loaded registers, and calls
   the C written for that routine; the switch to seg003's own stack is gone. The jump table at
   _5278 .. _52EA is direct calls here, and seg003_call dispatches the near offsets seg003's
   data holds (grlib.h). The other entries are still the generated stubs. */
#include "compat.h"
#include "gfx.h"
#include "grlib.h"

/* GRCORE.ASM's _DATA: far pointers into seg_370D (the symbol is 370D:0008, so seg_370D + X is
   370D:X+8, as `seg_370D+X` was in the assembly). */
unsigned char *palette = seg_370D + 0x5040;
int16 *Color_data_ptr = (int16 *)(seg_370D + 0x42C);
unsigned char *foreground_color = seg_370D + 0x2D32;
unsigned char *background_color = seg_370D + 0x2D33;
struct FontInfo *cur_font = (struct FontInfo *)(seg_370D + 0x4D00);
unsigned char *bytefont = seg_370D + 0x1C4C;
int16 *wtop = (int16 *)(seg_370D + 0x3DEE);
int16 *wbot = (int16 *)(seg_370D + 0x3DF2);
int16 *wright = (int16 *)(seg_370D + 0x3DF0);
int16 *wleft = (int16 *)(seg_370D + 0x3DEC);
unsigned char *pixel_color = seg_370D + 0x4109;
uint16 *dseg_67d6_21F4 = (uint16 *)(seg_370D + 0x3832);
uint16 *Ytab = (uint16 *)(seg_370D + 0x36A4);

void seg003_call(uint16_t off, uint16_t si)
{
    char why[64];
    switch (off) {
    case 0x2D83: seg003_0272_2D83(si); return;
    case 0x5372: seg003_0272_5372(si); return;
    case 0x5467: seg003_0272_5467(si); return;
    /* rets: _283C, nullsub_1 (31E5, 31E6), the five at 2C95 .. 2C99 */
    case 0x283C: case 0x31E5: case 0x31E6:
    case 0x2C95: case 0x2C96: case 0x2C97: case 0x2C98: case 0x2C99:
        return;
    default:
        snprintf(why, sizeof why, "seg003 routine at 0085:%04X has no C yet", off);
        port_halt(why);
    }
}

void setup_font(void) { seg003_0272_3BFD(); }
void init_graphics(void) { seg003_0272_283D(); }
void grPageFlip(void)
{
    seg003_0272_2ABE();
    port_on_flip();                     /* the --shot-at-flip debug screenshots */
}
void grSoftPageFlip(void) { seg003_0272_2AC1(); }
void set_the_color(int c) { seg003_0272_3195((uint16_t)c); }
void init_colors(void) { seg003_0272_31BE(); }
void clear_window(void) { seg003_0272_3423(); }

void set_the_window(int x0, int y0, int x1, int y1)
{
    SETW(0x5040, y0);
    SETW(0x503E, x0);
    SETW(0x5044, y1);
    SETW(0x5042, x1);
    seg003_0272_31E7(0x503E);
}

/* _341E: clip_rect (GRLIBF.ASM's _33BE), then urectangle; nothing when it is all outside. */
static int clip_rect(int16_t *ax, int16_t *bx, int16_t *cx, int16_t *dx)
{
    int16_t t;
    if (*bx <= *dx) { t = *bx; *bx = *dx; *dx = t; }
    if (*ax >= *cx) { t = *ax; *ax = *cx; *cx = t; }
    if (*cx < SW(0x3DF4)) return 0;
    if (*ax < SW(0x3DF4)) *ax = SW(0x3DF4);
    if (*ax > SW(0x3DF8)) return 0;
    if (*cx > SW(0x3DF8)) *cx = SW(0x3DF8);
    if (*bx < SW(0x3DFA)) return 0;
    if (*dx < SW(0x3DFA)) *dx = SW(0x3DFA);
    if (*dx > SW(0x3DF6)) return 0;
    if (*bx > SW(0x3DF6)) *bx = SW(0x3DF6);
    return 1;
}

void rectangle(int x, int y, int w, int h)
{
    int16_t ax = (int16_t)x, bx = (int16_t)y, cx = (int16_t)w, dx = (int16_t)h;
    if (clip_rect(&ax, &bx, &cx, &dx)) seg003_0272_342E(ax, bx, cx, dx);
}

void urectangle(int a, int b, int c, int d)
{
    seg003_0272_342E((int16_t)a, (int16_t)b, (int16_t)c, (int16_t)d);
}

/* show: AX, BX the position, DI:SI the bitmap's offset and segment, BP the height, CX the
   width, 0DC6 and 0DC8 the offsets into the bitmap. The bitmap is a far pointer, so it goes
   through the paragraph map as DS:SI did. */
void show(int x, int y, unsigned char *bm, int a, int b, int c, int d)
{
    SETW(0x0DC6, c);
    SETW(0x0DC8, d);
    seg003_0272_21D4((int16_t)x, (int16_t)y, (uint16_t)FP_OFF(bm), (uint16_t)FP_SEG(bm), (int16_t)b, (int16_t)a);
}

/* _49AE: claim n bytes of video memory from the bump pointer (GRLIBF.ASM's _3206); the old
   pointer, or 0 when it would reach the limit 4108. */
unsigned seg003_0272_49AE(int n)
{
    uint16_t ax = W(0x410A), bx = (uint16_t)(n + ax);
    if (bx < ax || bx >= W(0x4108)) return 0;
    SETW(0x410A, bx);
    return ax;
}
