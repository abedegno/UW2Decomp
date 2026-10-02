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
