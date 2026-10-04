/* grcore.c: replaces the C side of src/gfx/GRCORE.ASM (seg003 module 13): the graphics globals
   C reaches seg048 through (its _DATA), and the C entry points. GRCORE.ASM itself is translated
   (gfx/grcore_x.c, Exhume's tools/asm2c.py), so each entry here does what the game's C did in
   DOS: it pushes the C arguments onto the stack, as Turbo C's far call pushed them, and calls the
   entry's translation, which reads them from the stack, switches to seg048's own stack, runs the
   library routine and returns. So the library runs the original's instructions from the C's
   call on (the arguments into registers, the string copied to 3963:4FA6 with its 0 and one byte
   more), and its data in seg048 is what DOS's is.

   The arguments go on a stack that stands for DGROUP's (x86/entry.c). The entries' offsets are
   symbols.tsv's (the link's map). */
#include "compat.h"
#include "gfx.h"
#include "x86/asmrt.h"
#include "x86/entry.h"

#define SEG003 0x0090u                          /* seg003's EXE paragraph (asmgame.h) */

/* GRCORE.ASM's _DATA: far pointers into seg048 (3963:0000), as `seg048+X` in the assembly. */
unsigned char *palette = seg048 + 0x5046;
int16 *Color_data_ptr = (int16 *)(seg048 + 0x434);
unsigned char *foreground_color = seg048 + 0x2D38;
unsigned char *background_color = seg048 + 0x2D39;
struct FontInfo *cur_font = (struct FontInfo *)(seg048 + 0x4D06);
unsigned char *bytefont = seg048 + 0x1C56;
int16 *wtop = (int16 *)(seg048 + 0x3DF4);
int16 *wbot = (int16 *)(seg048 + 0x3DF8);
int16 *wright = (int16 *)(seg048 + 0x3DF6);
int16 *wleft = (int16 *)(seg048 + 0x3DF2);
unsigned char *pixel_color = seg048 + 0x410F;
uint16 *dseg_5c99_2404 = (uint16 *)(seg048 + 0x3838);
uint16 *Ytab = (uint16 *)(seg048 + 0x36AA);

static uint16_t call_entry(uint16_t off, const uint16_t *w, int n, uint16_t *dx)
{
    uint32_t r = port_far_entry(SEG003, off, w, n);
    if (dx) *dx = (uint16_t)(r >> 16);
    return (uint16_t)r;
}

#define far_arg port_far_arg
#define ENTRY0(name, off) \
    void name(void) { call_entry(off, 0, 0, 0); }
#define W W16

ENTRY0(setup_font, 0x4CB1)
ENTRY0(init_graphics, 0x4EA9)
ENTRY0(init_colors, 0x51B3)
ENTRY0(clear_window, 0x557F)
ENTRY0(copy_hidden_to_visible, 0x5314)
ENTRY0(copy_visible_to_hidden, 0x5350)

void grPageFlip(void)
{
    call_entry(0x4F2A, 0, 0, 0);
    port_on_flip();                     /* the --shot-at-flip debug screenshots */
}

ENTRY0(grSoftPageFlip, 0x4F66)

void string_to_screen(char *s, int x, int y)
{
    uint16_t w[4];
    far_arg(s, w);
    w[2] = W(x); w[3] = W(y);
    call_entry(0x4CED, w, 4, 0);
}

int string_width(char *s)
{
    uint16_t w[2];
    far_arg(s, w);
    return (int16_t)call_entry(0x4DCB, w, 2, 0);
}

unsigned char gr_read_pixel(int x, int y)
{
    uint16_t w[2] = { W(x), W(y) };
    return (unsigned char)call_entry(0x4FDE, w, 2, 0);
}

void plot_pixel(int x, int y)
{
    uint16_t w[2] = { W(x), W(y) };
    call_entry(0x5062, w, 2, 0);
}

void set_the_color(int c)
{
    uint16_t w[1] = { W(c) };
    call_entry(0x5174, w, 1, 0);
}

void set_the_window(int x0, int y0, int x1, int y1)
{
    uint16_t w[4] = { W(x0), W(y0), W(x1), W(y1) };
    call_entry(0x52BA, w, 4, 0);
}

void uvline(int x, int y1, int y2)
{
    uint16_t w[3] = { W(x), W(y1), W(y2) };
    call_entry(0x54A4, w, 3, 0);
}

void uhline(int x1, int y, int x2)
{
    uint16_t w[3] = { W(x1), W(y), W(x2) };
    call_entry(0x5418, w, 3, 0);
}

void rectangle(int x, int y, int w_, int h)
{
    uint16_t w[4] = { W(x), W(y), W(w_), W(h) };
    call_entry(0x54EB, w, 4, 0);
}

void urectangle(int a, int b, int c, int d)
{
    uint16_t w[4] = { W(a), W(b), W(c), W(d) };
    call_entry(0x5535, w, 4, 0);
}

void box(int a, int b, int c, int d)
{
    uint16_t w[4] = { W(a), W(b), W(c), W(d) };
    call_entry(0x55BB, w, 4, 0);
}

void show(int x, int y, unsigned char *bm, int a, int b, int c, int d)
{
    uint16_t w[8];
    w[0] = W(x); w[1] = W(y);
    far_arg(bm, w + 2);
    w[4] = W(a); w[5] = W(b); w[6] = W(c); w[7] = W(d);
    call_entry(0x57BB, w, 8, 0);
}

void seg003_581E(int handle, int x, int y, int w_, int h, int sx, int sy)
{
    uint16_t w[7] = { W(handle), W(x), W(y), W(w_), W(h), W(sx), W(sy) };
    call_entry(0x581E, w, 7, 0);
}

void fbshow(void *data, int x, int y, int width, int height)
{
    uint16_t w[6];
    far_arg(data, w);
    w[2] = W(x); w[3] = W(y); w[4] = W(width); w[5] = W(height);
    call_entry(0x5881, w, 6, 0);
}

void vcopyfb(int x, int y, int w_, int h, int handle)
{
    uint16_t w[5] = { W(x), W(y), W(w_), W(h), W(handle) };
    call_entry(0x5974, w, 5, 0);
}

void vcopy(int sx, int sy, int w_, int h, int x, int y)
{
    uint16_t w[6] = { W(sx), W(sy), W(w_), W(h), W(x), W(y) };
    call_entry(0x59C1, w, 6, 0);
}

void fbuf_setcolor(int c)
{
    uint16_t w[1] = { W(c) };
    call_entry(0x5A32, w, 1, 0);
}
