/* valloc.c: replaces src/gfx/VALLOC.ASM (seg001), the allocator of spare video memory and the
   save and restore of screen rectangles into its blocks. Its data stays in the far segment
   seg_5F5D at the assembly's offsets: [4] the pool's start, [6] its end, [8] the bump
   pointer, [0Ah] the end of the block records, which start at 0Ch, 0Dh bytes each (+0 video
   offset, +2 size, +4 1 if free, +5 x, +7 y, +9 w, +0Bh h). The register entries the assembly
   callers used (seg001_023B_5A, vfree's L0112, save_rect, restore_rect) are these same
   functions; the c_entry flag that chose how to return is gone. */
#include "compat.h"
#include "gfx.h"
#include "grlib.h"

extern unsigned char seg_5F5D[];
/* seg_5F5D is 5F5D:0004, so offset o of the segment is seg_5F5D[o - 4] */
#define V(o) (seg_5F5D + (uint16_t)(o) - 4)
#define VW(o) ((uint16_t)(V(o)[0] | V(o)[1] << 8))
#define VSETW(o, v) (V(o)[0] = (uint8_t)(v), V(o)[1] = (uint8_t)((uint16_t)(v) >> 8))
#define VB(o) (V(o)[0])

unsigned seg003_0272_49AE(int n);

/* +C: the pool is all the video memory the graphics library has not claimed, but one byte. */
void seg001_023B_C(void)
{
    uint16_t ax = (uint16_t)(W(0x4108) - W(0x410A) - 1);
    ax = (uint16_t)seg003_0272_49AE((int16_t)ax);
    VSETW(6, W(0x4108));
    VSETW(4, ax);
    VSETW(8, ax);
    VSETW(0x0A, 0x0C + 0x0D);
}

/* valloc (+4A, register entry +5A): a block of (w / 4 + 1) * h bytes; its video offset, or 0.
   First fit: a free record of that size, else a larger free one split, else the bump
   pointer. */
int seg001_023B_5A(int w, int h)
{
    uint16_t ax = (uint16_t)((uint16_t)(((uint16_t)w >> 2) + 1) * (uint16_t)h), bx, dx, cx;
    for (bx = 0x0C; bx < VW(0x0A); bx += 0x0D) {
        if (VB(bx + 4) == 0) continue;
        dx = VW(bx + 2);
        if (ax == dx) {
            VB(bx + 4) = 0;
            return VW(bx);
        }
        if (ax < dx) {
            /* L0088: move the records from here up one (std; rep movsb from [0A] down to bx+1),
               then split */
            cx = (uint16_t)(VW(0x0A) - bx);
            memmove(V(bx + 1 + 0x0D), V(bx + 1), cx);
            if (VW(0x0A) >= 0x104C) return 0;
            VSETW(0x0A, VW(0x0A) + 0x0D);
            VSETW(bx + 2, ax);
            VSETW(bx + 0x0F, (uint16_t)(dx - ax));
            VB(bx + 0x11) = 1;
            VSETW(bx + 0x0D, (uint16_t)(ax + VW(bx)));
            VB(bx + 4) = 0;
            return VW(bx);
        }
    }
    /* L00C6 */
    dx = VW(8);
    cx = (uint16_t)(dx + ax);
    if (cx < dx || cx > VW(6)) return 0;
    VSETW(8, cx);
    VSETW(bx, dx);
    VSETW(bx + 2, ax);
    VB(bx + 4) = 0;
    VSETW(0x0A, VW(0x0A) + 0x0D);
    return VW(bx);
}

int valloc(int w, int h)
{
    return seg001_023B_5A(w, h);
}

/* vfree (+105, register entry L0112): 1 if no record has that offset, else 0. Merges with a
   free record just before or after, else marks it free; a block that ends at the bump
   pointer goes back to it. */
int vfree_regs(uint16_t ax)
{
    uint16_t bx, si, di, cx, dx;
    for (bx = 0x0C; ; bx += 0x0D) {
        if (VW(bx) == ax) break;
        if ((uint16_t)(bx + 0x0D) == VW(0x0A)) return 1;
    }
    si = di = bx;
    bx = 0;
    cx = 0;
    if (VB(di - 9) != 0) {              /* the record before is free */
        dx = VW(di - 0x0D);
        bx = VW(di - 0x0B);             /* kept even when it is not adjacent, as in DOS */
        dx = (uint16_t)(dx + bx);
        if (ax == dx) {
            cx = (uint16_t)(VW(0x0A) - di);
            VSETW(0x0A, VW(0x0A) - 0x0D);
            si = (uint16_t)(si + 0x0D);
            goto L0163;
        }
    }
    if (VB(di + 0x11) == 0) {           /* L015D: nor the one after: just free it */
        VB(di + 4) = 1;
        goto L01A8;
    }
L0163:
    if (VB(di + 0x11) != 0) {
        ax = (uint16_t)(ax + VW(di + 2));
        if (ax == VW(di + 0x0D)) {      /* the record after is free and adjacent */
            VSETW(0x0A, VW(0x0A) - 0x0D);
            bx = (uint16_t)(bx + VW(di + 0x0F));
            si = (uint16_t)(si + 0x0D);
            if (cx == 0) {
                bx = VW(di + 2);
                si = (uint16_t)(si + 0x0D);
                cx = (uint16_t)(VW(0x0A) - di);
                di = (uint16_t)(di + 0x0D);
            }
        }
    }
    /* L0191: the record before takes the merged size; the rest move down over the gap */
    bx = (uint16_t)(bx + VW(di + 2));
    VSETW(di - 0x0B, bx);
    VB(di - 9) = 1;
    memmove(V(di), V(si), cx);
    di = (uint16_t)(di + cx - 0x0D);
L01A8:
    if ((uint16_t)(VW(di) + VW(di + 2)) == VW(8)) {
        VSETW(8, VW(8) - VW(di + 2));
        VSETW(0x0A, VW(0x0A) - 0x0D);
    }
    return 0;
}

void vfree(int block)
{
    vfree_regs((uint16_t)block);
}
