/* asmentry.c: replaces the C side of src/gfx/SPRITE.ASM (seg000), VALLOC.ASM (seg001),
   LPFDELTA.ASM (seg002) and MODEX.ASM (seg015_1F9B): the routines the game's C calls with C
   arguments. The modules are translated (gfx/sprite.c, valloc.c, lpfdelta.c, modex.c, Exhume's
   tools/asm2c.py), so each entry pushes
   the arguments as Turbo C's far call did and runs the routine's translation (x86/entry.c).
   The offsets are symbols.tsv's (the link's map). */
#include "compat.h"
#include "gfx.h"
#include "sys.h"
#include "x86/asmrt.h"
#include "x86/entry.h"

#include <stdarg.h>

#define SEG000 0x0000u                          /* SPRITE.ASM */
#define SEG001 0x004Eu                          /* VALLOC.ASM, VSTATS.ASM */
#define SEG002 0x0086u                          /* LPFDELTA.ASM */
#define SEG015 0x1DAEu                          /* MODEX.ASM, the listing's seg015_1F9B */

#define W W16

/* MODEX.ASM */
void grab(void *dst, int x, int y, int w_, int h)
{
    uint16_t w[6];
    port_far_arg(dst, w);
    w[2] = W(x); w[3] = W(y); w[4] = W(w_); w[5] = W(h);
    port_far_entry(SEG015, 0x000E, w, 6);
}

void seg015_1F9B_E8(char *text)
{
    uint16_t w[2];
    port_far_arg(text, w);
    port_far_entry(SEG015, 0x00E8, w, 2);
}

void mem_set(void *p, int value, int count)
{
    uint16_t w[4];
    port_far_arg(p, w);
    w[2] = W(value); w[3] = W(count);
    port_far_entry(SEG015, 0x00F8, w, 4);
}

int str_len(char *s)
{
    uint16_t w[2];
    port_far_arg(s, w);
    return (int16_t)port_far_entry(SEG015, 0x010E, w, 2);
}

void str_ncopy(char *dst, char *src, int n)
{
    uint16_t w[5];
    port_far_arg(dst, w);
    port_far_arg(src, w + 2);
    w[4] = W(n);
    port_far_entry(SEG015, 0x0124, w, 5);
}

char *str_copy(char *dst, char *src)
{
    uint16_t w[4];
    port_far_arg(dst, w);
    port_far_arg(src, w + 2);
    return port_far_result(port_far_entry(SEG015, 0x0144, w, 4));
}

char *FindStringDelimiter(char *s, int c)
{
    uint16_t w[3];
    port_far_arg(s, w);
    w[2] = W(c);
    return port_far_result(port_far_entry(SEG015, 0x0179, w, 3));
}

char *str_str(char *s, char *find)
{
    uint16_t w[4];
    port_far_arg(s, w);
    port_far_arg(find, w + 2);
    return port_far_result(port_far_entry(SEG015, 0x01A1, w, 4));
}

int str_cmp(char *a, char *b)
{
    uint16_t w[4];
    port_far_arg(a, w);
    port_far_arg(b, w + 2);
    return (int16_t)port_far_entry(SEG015, 0x01FC, w, 4);
}

void local_do_palette(int count, unsigned char first)
{
    uint16_t w[2] = { W(count), first };
    port_far_entry(SEG015, 0x0225, w, 2);
}

void seg015_1F9B_25A(int x, int y, int color)
{
    uint16_t w[3] = { W(x), W(y), W(color) };
    port_far_entry(SEG015, 0x025A, w, 3);
}

void seg015_1F9B_2A7(unsigned char *src, unsigned dst, int w_, int h)
{
    uint16_t w[5];
    port_far_arg(src, w);
    w[2] = (uint16_t)dst; w[3] = W(w_); w[4] = W(h);
    port_far_entry(SEG015, 0x02A7, w, 5);
}

void seg015_1F9B_325(unsigned offset, int16 *width, int16 *height)
{
    uint16_t w[5];
    w[0] = (uint16_t)offset;
    port_far_arg(width, w + 1);
    port_far_arg(height, w + 3);
    port_far_entry(SEG015, 0x0325, w, 5);
}

int seg015_1F9B_366(unsigned char *src, unsigned n, int unused)
{
    uint16_t w[4];
    port_far_arg(src, w);
    w[2] = (uint16_t)n; w[3] = W(unused);
    return (int16_t)port_far_entry(SEG015, 0x0366, w, 4);
}

/* VALLOC.ASM */
int valloc(int w_, int h)
{
    uint16_t w[2] = { W(w_), W(h) };
    return (int16_t)port_far_entry(SEG001, 0x004A, w, 2);
}

void vfree(int handle)
{
    uint16_t w[1] = { W(handle) };
    port_far_entry(SEG001, 0x0105, w, 1);
}

void save_rect(int handle, int x, int y, int w_, int h)
{
    uint16_t w[5] = { W(handle), W(x), W(y), W(w_), W(h) };
    port_far_entry(SEG001, 0x01D5, w, 5);
}

void restore_rect(int handle)
{
    uint16_t w[1] = { W(handle) };
    port_far_entry(SEG001, 0x025B, w, 1);
}

void seg001_023B_C(void)
{
    port_far_entry(SEG001, 0x000C, 0, 0);
}

/* LPFDELTA.ASM */
void seg002_A(unsigned char *src, unsigned char *dst)
{
    uint16_t w[4];
    port_far_arg(src, w);
    port_far_arg(dst, w + 2);
    port_far_entry(SEG002, 0x000A, w, 4);
}

/* SPRITE.ASM */
int create_sprite(int layer, ...)
{
    uint16_t w[3] = { W(layer), 0, 0 };
    if (layer) {                        /* layers 1 to 3 pass the block's width and height */
        va_list ap;
        va_start(ap, layer);
        w[1] = W(va_arg(ap, int));
        w[2] = W(va_arg(ap, int));
        va_end(ap);
    }
    return (int16_t)port_far_entry(SEG000, 0x0002, w, layer ? 3 : 1);
}

void destroy_sprite(int spr)
{
    uint16_t w[1] = { W(spr) };
    port_far_entry(SEG000, 0x0073, w, 1);
}

void change_sprite(int spr, int x, int y, int w_, int h)
{
    uint16_t w[5] = { W(spr), W(x), W(y), W(w_), W(h) };
    port_far_entry(SEG000, 0x00A6, w, 5);
}

void draw_sprite(int spr, int frame)
{
    uint16_t w[2] = { W(spr), W(frame) };
    port_far_entry(SEG000, 0x00EB, w, 2);
}

void draw_mask(int spr, int frame)
{
    uint16_t w[2] = { W(spr), W(frame) };
    port_far_entry(SEG000, 0x0120, w, 2);
}

void erase_sprite(int spr)
{
    uint16_t w[1] = { W(spr) };
    port_far_entry(SEG000, 0x0155, w, 1);
}

void move_sprite(int spr, int x, int y)
{
    uint16_t w[3] = { W(spr), W(x), W(y) };
    port_far_entry(SEG000, 0x018B, w, 3);
}

void resize_sprite(int spr, int w_, int h)
{
    uint16_t w[3] = { W(spr), W(w_), W(h) };
    port_far_entry(SEG000, 0x01C2, w, 3);
}

void set_yoff(int spr, int yoff)
{
    uint16_t w[2] = { W(spr), W(yoff) };
    port_far_entry(SEG000, 0x01F9, w, 2);
}

void update_sprites(void)
{
    port_far_entry(SEG000, 0x0330, 0, 0);
}
