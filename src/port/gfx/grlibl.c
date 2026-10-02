/* grlibl.c: replaces part of src/gfx/GRLIBL.ASM (seg003 module 12), the bitmap span transfers:
   _5372 (a linear bitmap's rows to the planar screen, opaque) and _5467 (the same, colour 0
   skipped), which show runs on the row records VIDMODE.ASM's L2296 writes (y, left x, right x,
   source offset). Each row goes one plane at a time: the map mask for the plane, then every
   fourth source byte. The source is the far segment in 55EC (DS in the assembly), reached
   through the paragraph map, its offset wrapping at 64 KB as SI did. */
#include "grlib.h"

static void rows(uint16_t si, int transparent)
{
    uint16_t ax, bx, cx, di, bp, n, s;
    const uint8_t *src;
    int p;
    for (;;) {
        ax = W(si); si += 2;
        if (ax & 0x8000) return;
        ax = (uint16_t)(ax << 1);
        SETW(0x55E2, W(0x36AC + ax));
        bx = W(si); si += 2;
        SETW(0x55E6, W(si)); si += 2;
        cx = W(si); si += 2;
        src = port_mk_fp(W(0x55EC), 0);
        if (!src) port_fatal("show: the bitmap's segment %04X is in no region", W(0x55EC));
        for (p = 0; p < 4; p++) {
            bp = bx;
            vga_outb(SC_DATA, (uint8_t)(B(0x3B64 + bp) & B(0x3CA8 + bp)));
            di = (uint16_t)(((int16_t)bp >> 2) + W(0x55E2));
            bp = (uint16_t)((int16_t)(W(0x55E6) - bp) >> 2);
            s = cx;
            bx++;
            cx++;
            bp++;
            for (n = bp; n; n--) {
                uint8_t v = src[s];
                if (!transparent || v) vga_write(di, v);
                di++;
                s = (uint16_t)(s + 4);
            }
        }
    }
}

void seg003_0272_5372(uint16_t si) { rows(si, 0); }
void seg003_0272_5467(uint16_t si) { rows(si, 1); }

/* ---- the transfers from video memory, within the screen, and into a linear buffer ---------- */

/* _5578: rows from video memory (the record's offset) when the planes do not line up: up to
   four passes, one per destination pixel's plane (the map mask from 3B64 and 3CA8 at x), each
   reading plane 0, 1, 2, 3 in turn (read map select) from the same source offset. _56E3 the
   same with colour 0 skipped (and the read plane kept within 0..3). */
static void vmem_planes(uint16_t si, int transparent)
{
    uint16_t bx, di, src, cx, n, s, d;
    uint8_t ah, v;
    int pass;
    for (;;) {
        bx = W(si);
        if (bx & 0x8000) break;
        bx = (uint16_t)(bx << 1);
        SETW(0x55E2, W(0x36AC + bx));
        bx = W(si + 2);
        di = W(si + 4);
        src = W(si + 6);
        si += 8;
        SETW(0x55E8, (uint16_t)(di - bx + 1));
        ah = 0;
        vga_outb(GC_INDEX, 4);
        for (pass = 0; pass < 4; pass++) {
            vga_outb(SC_DATA, (uint8_t)(B(0x3B64 + bx) & B(0x3CA8 + bx)));
            vga_outb(GC_DATA, ah);
            cx = (uint16_t)((uint16_t)(W(0x55E8) + 3) >> 2);
            s = src;
            d = (uint16_t)((bx >> 2) + W(0x55E2));
            for (n = cx; n; n--) {
                v = vga_read(s++);
                if (!transparent || v) vga_write(d, v);
                d++;
            }
            bx++;
            ah++;
            if (transparent) ah &= 3;
            SETW(0x55E8, W(0x55E8) - 1);
            if (!W(0x55E8)) break;
        }
    }
    vga_outw(GC_INDEX, 0xFF08);
}

/* _568B: rows from video memory, plane-aligned: a latched copy of the whole groups with all
   planes, then the last partial group with the right edge's mask (none when it ends a group). */
static void vmem_aligned(uint16_t bx)
{
    uint16_t bp, di, ax, cx, si;
    vga_outb(GC_DATA, 0);
    for (;;) {
        bp = W(bx);
        if (bp & 0x8000) break;
        bp = (uint16_t)(bp << 1);
        di = W(0x36AC + bp);
        ax = W(bx + 2);
        cx = W(bx + 4);
        bp = cx;
        si = W(bx + 6);
        bx += 8;
        cx = (uint16_t)(cx - ax + 1);
        di = (uint16_t)(di + (ax >> 2));
        cx = (uint16_t)(cx >> 2);
        if (cx) {
            vga_outb(SC_DATA, 0x0F);
            while (cx--) { vga_write(di, vga_read(si)); di++; si++; }
            if ((bp & 3) == 3) continue;
        }
        vga_outb(SC_DATA, B(0x3CA8 + bp));
        vga_write(di, vga_read(si));
    }
    vga_outb(GC_DATA, 0xFF);
}

/* _581B (vcopy, left to right): the row address is the source and the record's offset the
   destination. After the whole groups the last partial group is always copied as well (the
   je after rep movs tests the flags of the shr before it, which were not zero there). _586D:
   right to left, for a copy onto an overlapping area further right. */
static void screen_copy(uint16_t bx)
{
    uint16_t bp, si, ax, cx, di;
    vga_outb(GC_DATA, 0);
    for (;;) {
        bp = W(bx);
        if (bp & 0x8000) break;
        bp = (uint16_t)(bp << 1);
        si = W(0x36AC + bp);
        ax = W(bx + 2);
        cx = W(bx + 4);
        bp = cx;
        di = W(bx + 6);
        bx += 8;
        cx = (uint16_t)(cx - ax + 1);
        si = (uint16_t)(si + (ax >> 2));
        cx = (uint16_t)(cx >> 2);
        if (cx) {
            vga_outb(SC_DATA, 0x0F);
            while (cx--) { vga_write(di, vga_read(si)); di++; si++; }
        }
        vga_outb(SC_DATA, B(0x3CA8 + bp));
        vga_write(di, vga_read(si));
    }
    vga_outb(GC_DATA, 0xFF);
}

static void screen_copy_back(uint16_t bx)
{
    uint16_t bp, si, ax, cx, di;
    vga_outb(GC_DATA, 0);
    for (;;) {
        bp = W(bx);
        if (bp & 0x8000) break;
        bp = (uint16_t)(bp << 1);
        si = W(0x36AC + bp);
        ax = W(bx + 2);
        cx = W(bx + 4);
        di = W(bx + 6);
        bx += 8;
        cx = (uint16_t)(cx - ax + 1);
        bp = cx;
        si = (uint16_t)(si + (ax >> 2));
        cx = (uint16_t)((cx >> 2) - 1);
        si = (uint16_t)(si + cx);
        di = (uint16_t)(di + cx);
        if (bp & 3) {
            /* match the assembly: the edge mask is indexed by the pixel count, not an x */
            vga_outb(SC_DATA, B(0x3CA8 + bp));
            vga_write(di, vga_read(si));
            di--; si--;
            if (!cx) continue;
        }
        cx++;
        vga_outb(SC_DATA, 0x0F);
        while (cx--) { vga_write(di, vga_read(si)); di--; si--; }
    }
    vga_outb(GC_DATA, 0xFF);
}

/* _58C7: linear bitmap rows (source segment 55EC) into the linear buffer of _2B62 (segment
   39D0, row table 39D2); _58F8 the same with colour 0 skipped. */
static void linear_rows(uint16_t si, int transparent)
{
    uint8_t *dst = port_mk_fp(W(0x39D0), 0), *src = port_mk_fp(W(0x55EC), 0);
    uint16_t ax, di, cx, s;
    if (!dst || !src) port_fatal("linear bitmap transfer: a segment in no region");
    for (;;) {
        ax = W(si); si += 2;
        if (ax & 0x8000) return;
        di = W(0x39D2 + (uint16_t)(ax << 1));
        ax = W(si); si += 2;
        di = (uint16_t)(di + ax);
        cx = (uint16_t)(W(si) - ax + 1); si += 2;
        s = W(si); si += 2;
        do {
            uint8_t v = src[s++];
            if (!transparent || v) dst[di] = v;
            di++;
        } while (--cx);
    }
}

void seg003_0272_5578(uint16_t si) { vmem_planes(si, 0); }
void seg003_0272_56E3(uint16_t si) { vmem_planes(si, 1); }
void seg003_0272_568B(uint16_t si) { vmem_aligned(si); }
void seg003_0272_581B(uint16_t si) { screen_copy(si); }
void seg003_0272_586D(uint16_t si) { screen_copy_back(si); }
void seg003_0272_58C7(uint16_t si) { linear_rows(si, 0); }
void seg003_0272_58F8(uint16_t si) { linear_rows(si, 1); }
