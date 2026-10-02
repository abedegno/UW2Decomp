/* vga.c: replaces the VGA (docs/PORT.md, "The screen"). The parts the game uses: 256 KB of
   video memory in four planes, the sequencer (map mask, memory mode), the graphics controller
   (set/reset, logical function, read map, write mode, bit mask), the latches, the CRT
   controller (display start, offset, maximum scan line, line compare), the attribute
   controller's pixel panning, the DAC with 6-bit colour, and input status 1's retrace bit.

   The game draws through vga_write and vga_read (seg003's routines, the C written for them,
   and later CUTS.C's planar writes) and programs the registers through outportb. The platform
   layer calls vga_scanout from its own thread whenever it presents a frame, and gets what the
   CRT controller would send to the monitor: the visible page as an indexed picture, and the DAC.
   So the port's screenshots compare with DOS's as indexed pictures, byte for byte. */
#include <string.h>
#include "port.h"
#include "plat.h"

static uint8_t mem[4][0x10000];
static uint8_t latch[4];
static uint8_t seq_index, seq[8];
static uint8_t gc_index, gc[16];
static uint8_t crtc_index, crtc[32];
static uint8_t attr_index, attr[32], attr_flip;
static uint8_t dac[768];
static uint8_t dac_windex, dac_wsub, dac_rindex, dac_rsub;
static int mode = 3;

void vga_set_mode(int m)
{
    int i;
    mode = m;
    if (m != 0x13) return;
    seq[2] = 0x0F; seq[4] = 0x0E;
    memset(gc, 0, sizeof gc);
    gc[5] = 0x40; gc[6] = 0x05; gc[7] = 0x0F; gc[8] = 0xFF;
    memset(crtc, 0, sizeof crtc);
    crtc[0x01] = 0x4F; crtc[0x07] = 0x1F; crtc[0x09] = 0x41; crtc[0x12] = 0x8F;
    crtc[0x13] = 0x28; crtc[0x14] = 0x40; crtc[0x17] = 0xA3; crtc[0x18] = 0xFF;
    attr[0x10] = 0x41; attr[0x13] = 0;
    /* the BIOS clears the screen; its default palette is not reproduced */
    for (i = 0; i < 4; i++) memset(mem[i], 0, sizeof mem[i]);
}

void vga_outb(unsigned port, uint8_t v)
{
    switch (port) {
    case 0x3C0:
        if (!attr_flip) attr_index = v & 0x3F;
        else if ((attr_index & 0x1F) < 0x15) attr[attr_index & 0x1F] = v;
        attr_flip ^= 1;
        break;
    case 0x3C4: seq_index = v & 7; break;
    case 0x3C5: seq[seq_index] = v; break;
    case 0x3C7: dac_rindex = v; dac_rsub = 0; break;
    case 0x3C8: dac_windex = v; dac_wsub = 0; break;
    case 0x3C9:
        dac[dac_windex * 3 + dac_wsub] = v & 0x3F;
        if (++dac_wsub == 3) { dac_wsub = 0; dac_windex++; }
        break;
    case 0x3CE: gc_index = v & 15; break;
    case 0x3CF: gc[gc_index] = v; break;
    case 0x3D4: crtc_index = v & 31; break;
    case 0x3D5: crtc[crtc_index] = v; break;
    default: break;
    }
}

void vga_outw(unsigned port, uint16_t v)
{
    vga_outb(port, (uint8_t)v);
    vga_outb(port + 1, (uint8_t)(v >> 8));
}

/* Input status 1: bit 3 vertical retrace, for the last 1.4 ms of each 14.3 ms frame (70 Hz),
   bit 0 not in the display; from the counter, so a wait for retrace takes as long as on the
   real card. */
static uint8_t status1(void)
{
    uint64_t hz = plat_counter_hz(), t = plat_counter();
    uint64_t frame = hz * 1000 / 70086, ph = t % (frame ? frame : 1);
    uint8_t s = 0;
    if (ph >= frame - frame / 10) s |= 0x09;
    else if ((t / (hz / 31469 ? hz / 31469 : 1)) % 10 >= 8) s |= 0x01;
    return s;
}

uint8_t vga_inb(unsigned port)
{
    uint8_t v;
    switch (port) {
    case 0x3C0: return attr_index;
    case 0x3C1: return attr[attr_index & 0x1F];
    case 0x3C5: return seq[seq_index];
    case 0x3C7: return 3;
    case 0x3C8: return dac_windex;
    case 0x3C9:
        v = dac[dac_rindex * 3 + dac_rsub];
        if (++dac_rsub == 3) { dac_rsub = 0; dac_rindex++; }
        return v;
    case 0x3CF: return gc[gc_index];
    case 0x3D5: return crtc[crtc_index];
    case 0x3DA: attr_flip = 0; return status1();
    default: return 0xFF;
    }
}

/* A CPU write to A000:off: chained (mode 13h) to plane off & 3; unchained to every plane the
   map mask enables, through set/reset, the logical function and the bit mask (write mode 0),
   or from the latches (write mode 1). Rotation is not used by the game. */
void vga_write(uint16_t off, uint8_t v)
{
    int p, wm = gc[5] & 3, fn = (gc[3] >> 3) & 3;
    uint8_t mask = seq[2] & 15, bm = gc[8];
    if (seq[4] & 8) {
        p = off & 3;
        if (mask & (1 << p)) mem[p][off & 0xFFFC] = v;
        return;
    }
    for (p = 0; p < 4; p++) {
        uint8_t d;
        if (!(mask & (1 << p))) continue;
        if (wm == 1) { mem[p][off] = latch[p]; continue; }
        d = (gc[1] & (1 << p)) ? ((gc[0] & (1 << p)) ? 0xFF : 0) : v;
        switch (fn) {
        case 1: d &= latch[p]; break;
        case 2: d |= latch[p]; break;
        case 3: d ^= latch[p]; break;
        default: break;
        }
        mem[p][off] = (uint8_t)((d & bm) | (latch[p] & ~bm));
    }
}

uint8_t vga_read(uint16_t off)
{
    int p;
    if (seq[4] & 8) return mem[off & 3][off & 0xFFFC];
    for (p = 0; p < 4; p++) latch[p] = mem[p][off];
    return mem[gc[4] & 3][off];
}

void vga_get_dac(uint8_t rgb6[768])
{
    memcpy(rgb6, dac, sizeof dac);
}

/* What the CRT controller shows: 320 pixels across; 200 rows when each row is scanned twice
   (maximum scan line 1, mode 13h and the game's mode 0) or 400 when once (mode 1). Each row
   starts at the display start plus the offset register's words, the pixel panning shifts it,
   and at the line compare the address starts again from 0 (the split screen). Text mode shows
   black. */
void vga_scanout(uint8_t *pix, int *w, int *h, uint8_t rgb6[768])
{
    int msl = (crtc[9] & 0x1F) + 1, rows = 400 / msl, y, x;
    unsigned start = (unsigned)crtc[0x0C] << 8 | crtc[0x0D], pitch = (unsigned)crtc[0x13] * 2;
    unsigned lc = crtc[0x18] | (crtc[7] & 0x10) << 4 | (crtc[9] & 0x40) << 3;
    unsigned pan = (attr[0x13] & 7) >> 1, addr;
    memcpy(rgb6, dac, sizeof dac);
    *w = 320;
    *h = rows > 480 ? 480 : rows;
    if (mode != 0x13) { memset(pix, 0, (size_t)*w * (size_t)*h); return; }
    if (seq[4] & 8) {           /* chained: mode 13h as the BIOS set it */
        for (y = 0; y < *h; y++)
            for (x = 0; x < 320; x++) {
                unsigned a = (start * 4 + (unsigned)y * pitch * 4 + (unsigned)x) & 0x3FFFF;
                pix[y * 320 + x] = mem[a & 3][(a >> 2) & 0xFFFC];
            }
        return;
    }
    addr = start;
    for (y = 0; y < *h; y++) {
        unsigned sl = (unsigned)y * (unsigned)msl;
        if (y && sl > lc && (sl - (unsigned)msl) <= lc) { addr = 0; if (attr[0x10] & 0x20) pan = 0; }
        for (x = 0; x < 320; x++) {
            unsigned px = (unsigned)x + pan;
            pix[y * 320 + x] = mem[px & 3][(addr + (px >> 2)) & 0xFFFF];
        }
        addr += pitch;
    }
}
