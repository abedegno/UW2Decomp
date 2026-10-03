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
    /* GRCORE.ASM's jump table entries for the span writers, which some code stores in 4112 in
       place of the routine itself (copy_visible_to_hidden's 52CC) */
    case 0x52C9: off = 0x2D83; break;
    case 0x52CC: off = 0x2F18; break;
    case 0x52CF: off = 0x2D0D; break;
    case 0x52D2: off = 0x2E79; break;
    case 0x52D5: off = 0x2E3A; break;
    case 0x52D8: off = 0x303B; break;
    case 0x52DB: off = 0x2FD6; break;
    case 0x52DE: off = 0x2F96; break;
    case 0x52E1: off = 0x2DF3; break;
    case 0x52E7: off = 0x2C9A; break;
    case 0x52EA: off = 0x283C; break;
    default: break;
    }
    if (seg003_fb_span(off, si)) return;
    switch (off) {
    case 0x2C9A: case 0x2D0D: case 0x2DF3: case 0x2E3A: case 0x2E79: case 0x2F18: case 0x2F96:
        seg003_span(off, si); return;
    case 0x2D83: seg003_0272_2D83(si); return;
    case 0x5372: seg003_0272_5372(si); return;
    case 0x5467: seg003_0272_5467(si); return;
    case 0x5578: seg003_0272_5578(si); return;
    case 0x568B: seg003_0272_568B(si); return;
    case 0x56E3: seg003_0272_56E3(si); return;
    case 0x581B: seg003_0272_581B(si); return;
    case 0x586D: seg003_0272_586D(si); return;
    case 0x58C7: seg003_0272_58C7(si); return;
    case 0x58F8: seg003_0272_58F8(si); return;
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

/* A bitmap's far pointer split as the DOS caller made it. fbshow's callers pass pictures they
   made with MK_FP at a paragraph or a page (GRSPIC.C's seg009_7 and cFrmtoRaw, PANELS.C's
   weapon art), plus the header's few bytes; the paragraph map alone would give offsets from the
   EMS frame's or cmpbuf's own segment. seg003 keeps the offset in its row records, so it must
   be DOS's. */
static void bitmap_fp(const void *bm, uint16_t *off, uint16_t *seg)
{
    unsigned s, o;
    port_fp_split_recent(bm, &s, &o);
    *off = (uint16_t)o;
    *seg = (uint16_t)s;
}

/* show: AX, BX the position, DI:SI the bitmap's offset and segment, BP the height, CX the
   width, 0DC6 and 0DC8 the offsets into the bitmap. The bitmap's far pointer is split as its
   maker made it (bitmap_fp): a picture from GRSPIC.C's seg009_7, an object's or the mouse
   cursor's, is at offset 0 of a segment in DOS, and seg003 keeps that offset in its row
   records (the explore session's object cursor showed it). */
void show(int x, int y, unsigned char *bm, int a, int b, int c, int d)
{
    uint16_t off, seg;
    bitmap_fp(bm, &off, &seg);
    SETW(0x0DC6, c);
    SETW(0x0DC8, d);
    seg003_0272_21D4((int16_t)x, (int16_t)y, off, seg, (int16_t)b, (int16_t)a);
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

/* GRCORE.ASM's other entries, each loading the registers its routine takes. */
void copy_visible_to_hidden(void) { seg003_0272_327D(); }
void copy_hidden_to_visible(void) { seg003_0272_328F(); }

/* _4A3A -> _2977: a virtual screen (width, BX, CX: see vidmode.c) */
void seg003_0272_4A3A(int ax, int bx, int cx) { seg003_0272_2977((uint16_t)ax, (uint16_t)bx, (uint16_t)cx); }
void vscreen_focus(int x, int y) { seg003_0272_2A0D((uint16_t)x, (uint16_t)y); }

/* box -> _336E: clip_rect, then ubox */
void box(int x0, int y0, int x1, int y1)
{
    int16_t ax = (int16_t)x0, bx = (int16_t)y0, cx = (int16_t)x1, dx = (int16_t)y1;
    if (clip_rect(&ax, &bx, &cx, &dx)) seg003_0272_3371(ax, bx, cx, dx);
}

void uhline(int x0, int y, int x1) { seg003_0272_34AE((int16_t)x0, (int16_t)y, (int16_t)x1); }
/* uvline: x, y0, y1 (the wrapper puts the third argument in DX) */
void uvline(int x, int y0, int y1) { seg003_0272_3324((int16_t)x, (int16_t)y0, (int16_t)y1); }
void seg003_0272_4B93(int x, int y0, int y1) { seg003_0272_331A((int16_t)x, (int16_t)y0, (int16_t)y1); }
void seg003_0272_4BDA(int x0, int y, int x1) { seg003_0272_347F((int16_t)x0, (int16_t)y, (int16_t)x1); }
void seg003_0272_4C64(int x, int y0, int y1) { seg003_0272_3321((int16_t)x, (int16_t)y0, (int16_t)y1); }

/* gr_read_pixel -> 529F (_30FB, unclipped); _47A0 -> 52A2 (_30E3, -1 outside the window);
   plot_pixel -> 52A5 (_3094); _4824 -> 52A8 (_30AC) */
unsigned char gr_read_pixel(int x, int y) { return (unsigned char)seg003_0272_30FB((uint16_t)x, (uint16_t)y); }
int seg003_0272_47A0(int x, int y) { return (int16_t)seg003_0272_30E3((int16_t)x, (int16_t)y); }
void plot_pixel(int x, int y) { seg003_0272_3094((int16_t)x, (int16_t)y); }
void seg003_0272_4824(int x, int y) { seg003_0272_30AC((uint16_t)x, (uint16_t)y); }

/* The string entries copy the far string to 370D:4FA8 first (above the library's stack):
   repne scasb over at most 84h bytes for the 0, then rep movsb of the length plus two (the
   string, its 0 and the byte after), or 85h bytes when there is no 0 in the first 84h. */
static uint16_t copy_string(const char *s)
{
    uint16_t n = 0, len;
    while (n < 0x84 && s[n]) n++;
    len = n < 0x84 ? (uint16_t)(n + 2) : 0x85;
    for (n = 0; n < len; n++) SETB(0x4FA8 + n, (uint8_t)s[n]);
    return 0x4FA8;
}

uint16_t seg003_0272_43C5(uint16_t si);
void seg003_0272_3B36(uint16_t ax, uint16_t bx, uint16_t si);
void seg003_0272_3B8F(uint16_t ax, uint16_t bx, uint16_t si);

/* string_to_screen -> 527E (_3B36); _44DC -> 5287 (_3B8F, shadowed); string_width -> _43C5 */
void string_to_screen(char *s, int x, int y)
{
    uint16_t si = copy_string(s);
    seg003_0272_3B36((uint16_t)x, (uint16_t)y, si);
}

void seg003_0272_44DC(char *s, int x, int y)
{
    uint16_t si = copy_string(s);
    seg003_0272_3B8F((uint16_t)x, (uint16_t)y, si);
}

int string_width(char *s)
{
    return (int16_t)seg003_0272_43C5(copy_string(s));
}

/* The other bitmap entries: _5025 -> _21ED (from video memory), fbshow -> _2214 (into the frame
   buffer), _50E7 -> _2B62 and _511C -> _222B (the linear buffer), vcopyfb -> _2242, vcopy ->
   _2250; and fbuf_setcolor -> GRENTRY's _B9B. */
void seg003_0272_5025(int off, int x, int y, int w, int h, int xo, int yo)
{
    SETW(0x0DC6, xo);
    SETW(0x0DC8, yo);
    seg003_0272_21ED((int16_t)x, (int16_t)y, (uint16_t)off, 0xA000, (int16_t)w, (int16_t)h);
}

void fbshow(void *bm, int x, int y, int w, int h)
{
    uint16_t o, sg;
    bitmap_fp(bm, &o, &sg);
    SETW(0x0DC6, 0);
    SETW(0x0DC8, 0);
    seg003_0272_2214((int16_t)x, (int16_t)y, o, sg, (int16_t)w, (int16_t)h);
}

void seg003_0272_50E7(int ax, int dx, int bx, int cx)
{
    seg003_0272_2B62((uint16_t)ax, (uint16_t)bx, (uint16_t)cx, (uint16_t)dx);
}

void seg003_0272_511C(unsigned char *bm, int x, int y, int w, int h)
{
    SETW(0x0DC6, 0);
    SETW(0x0DC8, 0);
    seg003_0272_222B((int16_t)x, (int16_t)y, (uint16_t)FP_OFF(bm), (uint16_t)FP_SEG(bm), (int16_t)w, (int16_t)h);
}

void vcopyfb(int x, int y, int w, int h, int di)
{
    seg003_0272_2242((int16_t)x, (int16_t)y, (int16_t)w, (int16_t)h, (uint16_t)di);
}

void vcopy(int x, int y, int w, int h, int x2, int y2)
{
    seg003_0272_2250((int16_t)x, (int16_t)y, (int16_t)w, (int16_t)h, (uint16_t)x2, (uint16_t)y2);
}

void fbuf_setcolor(int c) { seg003_0272_B9B((uint16_t)c); }
