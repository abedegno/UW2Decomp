/* sprite.c: replaces src/gfx/SPRITE.ASM (seg000), the sprites: pictures the game redraws in place
   over the screen (the flasks, the compass, the eyes), on four layers. Written from the
   assembly, whose header has the data: sp_inf_tab (5EFD:0000), 40h records of 10h bytes (+0
   flags: 1 in use, 2 visible, 4 nothing saved, 8 destroyed, 10h transparent; +2 x; +4 y, a
   byte; +5 width; +7 height, a byte; +8 the byte set_yoff sets; +9 the layer, a word; +0Bh
   the picture; +0Dh the valloc block holding what it covers), then at +400h four redraw
   lists and at +500h four lists of the sprites on screen, 40h bytes each (a count of bytes,
   then sprite numbers), and at +600h and +602h the two words the overlap scan keeps.

   The quirks are the assembly's: a sprite number up to 40h is accepted (40h addresses the
   redraw lists), the overlap scan compares y as signed bytes, and after it queues an
   overlapping sprite it resumes from the start of the list it is scanning (BX was reused). */
#include "compat.h"
#include <stdarg.h>
#include "gfx.h"
#include "ui.h"
#include "grlib.h"

extern unsigned char sp_inf_tab[];
#define S(o) (sp_inf_tab + (uint16_t)(o))
#define SW_(o) ((uint16_t)(S(o)[0] | S(o)[1] << 8))
#define SSETW(o, v) port_setw(S(o), (uint16_t)(v))
#define INF_END 0x400
#define MAP_TAB 0x500
#define SP_LIST 0x600
#define SP_LEFT 0x602

int seg001_023B_5A(int w, int h);              /* valloc's register entry (valloc.c) */
int vfree_regs(uint16_t ax);
int save_rect_regs(int block, int x, int y, int w, int h);
int restore_rect_regs(int block);

static uint16_t sp_dirty;               /* SEG000's word at CS:0 */

/* repne scasw over a list's entries for ax; whether it was found, and where (the entry) */
static int find(uint16_t list, uint16_t ax, uint16_t *at)
{
    uint16_t n = (uint16_t)(SW_(list) >> 1), di = (uint16_t)(list + 2);
    while (n--) {
        if (SW_(di) == ax) { *at = di; return 1; }
        di += 2;
    }
    *at = di;
    return 0;
}

/* sp_changed: AX the sprite. See the header for the overlap scan's quirk. */
static void sp_changed(uint16_t ax)
{
    uint16_t si = (uint16_t)(ax << 4), bx, at, other, list;
    int8_t al, dl, cl;
    int16_t x, dx, cx;
    sp_dirty = 1;
    bx = (uint16_t)(SW_(si + 9) << 6);
    if (!find((uint16_t)(INF_END + bx), ax, &at)) {
        SSETW(INF_END + bx, SW_(INF_END + bx) + 2);
        SSETW(at, ax);
    }
    bx = (uint16_t)(SW_(si + 9) << 6);
    if (!find((uint16_t)(MAP_TAB + bx), ax, &at)) {
        if (SW_(si) & 2) {
            SSETW(MAP_TAB + bx, SW_(MAP_TAB + bx) + 2);
            SSETW(at, ax);
        }
    } else if (!(SW_(si) & 2)) {
        SSETW(at, SW_(MAP_TAB + bx + SW_(MAP_TAB + bx)));
        SSETW(MAP_TAB + bx, SW_(MAP_TAB + bx) - 2);
    }
    /* L029F: every visible sprite of a higher layer that overlaps it, to its redraw list */
    bx = (uint16_t)(SW_(si + 9) << 6);
    for (;;) {
        bx = (uint16_t)(bx + 0x40);
        SSETW(SP_LIST, bx);
        if (bx >= 0x100) return;
        if (!(SW_(MAP_TAB + bx) >> 1)) continue;
        SSETW(SP_LEFT, SW_(MAP_TAB + bx) >> 1);
        do {
            bx += 2;
            other = (uint16_t)(SW_(MAP_TAB + bx) << 4);
            x = (int16_t)SW_(other + 2);
            dx = (int16_t)SW_(si + 2);
            cx = (int16_t)(dx + SW_(si + 5));
            if (x > cx) goto next;
            x = (int16_t)(x + SW_(other + 5));
            if (x < dx) goto next;
            al = (int8_t)S(other + 4)[0];
            dl = (int8_t)S(si + 4)[0];
            cl = (int8_t)(dl - S(si + 7)[0]);
            if (cl > al) goto next;
            al = (int8_t)(al - S(other + 7)[0]);
            if (dl < al) goto next;
            ax = (uint16_t)(other >> 4);
            bx = (uint16_t)(SW_(other + 9) << 6);
            list = (uint16_t)(INF_END + bx);
            if (!find(list, ax, &at)) {
                SSETW(list, SW_(list) + 2);
                SSETW(at, ax);
            }
        next:
            SSETW(SP_LEFT, SW_(SP_LEFT) - 1);
        } while (SW_(SP_LEFT));
        bx = SW_(SP_LIST);
    }
}

/* create_sprite(layer, a, b): a free record, in use with nothing saved (transparent when
   Transparency is set), with the block valloc(a, b) unless layer is 0; its number, or -1 */
int create_sprite(int layer, ...)
{
    uint16_t si;
    int a = 0, b = 0, block = 0;
    va_list ap;
    for (si = 0; ; si += 0x10) {
        if (!(SW_(si) & 1)) break;
        if ((uint16_t)(si + 0x10) >= INF_END) return -1;
    }
    SSETW(si, 5);
    if (Transparency) SSETW(si, SW_(si) | 0x10);
    SSETW(si + 9, layer);
    if (layer) {
        va_start(ap, layer);
        a = va_arg(ap, int);
        b = va_arg(ap, int);
        va_end(ap);
        if (!(block = seg001_023B_5A(a, b))) return -1;
    }
    SSETW(si + 0x0D, block);
    S(si + 8)[0] = 0;
    return si >> 4;
}

/* the C entries that change one field and queue the sprite; n up to 40h */
#define CHECK(n) if ((unsigned)(n) > 0x40) return; bx = (uint16_t)((n) << 4)

void destroy_sprite(int n)
{
    uint16_t bx;
    CHECK(n);
    SSETW(bx, (SW_(bx) | 8) & 0xFFFD);
    sp_changed((uint16_t)n);
}

void change_sprite(int n, int x, int y, int w, int h)
{
    uint16_t bx;
    CHECK(n);
    SSETW(bx + 2, x);
    S(bx + 4)[0] = (uint8_t)y;
    SSETW(bx + 5, w);
    S(bx + 7)[0] = (uint8_t)h;
    sp_changed((uint16_t)n);
}

void draw_sprite(int n, int pic)
{
    uint16_t bx;
    CHECK(n);
    SSETW(bx + 0x0B, pic);
    SSETW(bx, SW_(bx) | 2);
    sp_changed((uint16_t)n);
}

void draw_mask(int n, int pic)
{
    uint16_t bx;
    CHECK(n);
    SSETW(bx + 0x0B, pic);
    SSETW(bx, SW_(bx) | 0x12);
    sp_changed((uint16_t)n);
}

void erase_sprite(int n)
{
    uint16_t bx;
    CHECK(n);
    if (!(SW_(bx) & 2)) return;
    SSETW(bx, SW_(bx) & 0xFFFD);
    sp_changed((uint16_t)n);
}

void move_sprite(int n, int x, int y)
{
    uint16_t bx;
    CHECK(n);
    SSETW(bx + 2, x);
    S(bx + 4)[0] = (uint8_t)y;
    sp_changed((uint16_t)n);
}

void resize_sprite(int n, int w, int h)
{
    uint16_t bx;
    CHECK(n);
    SSETW(bx + 5, w);
    S(bx + 7)[0] = (uint8_t)h;
    sp_changed((uint16_t)n);
}

void set_yoff(int n, int yoff)
{
    uint16_t bx;
    CHECK(n);
    S(bx + 8)[0] = (uint8_t)yoff;
    sp_changed((uint16_t)n);
}

/* update_sprites: with the mouse hidden, from layer 3 down to 1 put back what the queued
   sprites covered (and free the blocks of destroyed ones); then from layer 0 up, layer by
   layer, save what each visible queued sprite will cover (layers 1 to 3) and draw it, and
   empty the list. */
void update_sprites(void)
{
    uint16_t bx, si, di, n;
    if (!sp_dirty) return;
    mouse_hide();
    for (bx = MAP_TAB - 0x40; bx > INF_END; bx -= 0x40) {
        for (n = (uint16_t)(SW_(bx) >> 1), si = 2; n; n--, si += 2) {
            di = (uint16_t)(SW_(bx + si) << 4);
            if (SW_(di) & 4) SSETW(di, SW_(di) & 0xFFFB);
            else restore_rect_regs(SW_(di + 0x0D));
            if (SW_(di) & 8) {
                SSETW(di, SW_(di) & 0xFFFE);
                if (SW_(di + 0x0D)) vfree_regs(SW_(di + 0x0D));
            }
        }
    }
    for (bx = INF_END; bx < MAP_TAB; bx += 0x40) {
        if (bx != INF_END) {
            /* L03B4: the save phase */
            for (n = (uint16_t)(SW_(bx) >> 1), si = 2; n; n--, si += 2) {
                di = (uint16_t)(SW_(bx + si) << 4);
                if (!(SW_(di) & 2)) SSETW(di, SW_(di) | 4);
                else save_rect_regs(SW_(di + 0x0D), (int16_t)SW_(di + 2), S(di + 4)[0], (int16_t)SW_(di + 5), S(di + 7)[0]);
            }
        }
        /* L03FC: draw, emptying the list */
        n = (uint16_t)(SW_(bx) >> 1);
        SSETW(bx, 0);
        for (si = 2; n; n--, si += 2) {
            di = (uint16_t)(SW_(bx + si) << 4);
            if (!(SW_(di) & 2)) continue;
            if (SW_(di) & 0x10) Transparency = 1;
            if (S(di + 8)[0])
                mask_to_screen((int16_t)SW_(di + 0x0B), (int16_t)SW_(di + 2), S(di + 4)[0], S(di + 7)[0],
                               (int16_t)SW_(di + 5), S(di + 8)[0]);
            else
                pic_to_screen((int16_t)SW_(di + 0x0B), (int16_t)SW_(di + 2), S(di + 4)[0], S(di + 7)[0],
                              (int16_t)SW_(di + 5));
            Transparency = 0;
        }
    }
    mouse_show();
    sp_dirty = 0;
}
