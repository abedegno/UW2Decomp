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
#include <stdatomic.h>
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

extern int16_t rp_request;              /* src/replay/REPLAY.C: 2 when replaying */
static void latch_sync(void);
static void latch_publish(void);

void vga_outb(unsigned port, uint8_t v)
{
    if (port == 0x3D5 || (port == 0x3C0 && attr_flip)) latch_sync();
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
    if (port == 0x3D5 || port == 0x3C0) latch_publish();
}

void vga_outw(unsigned port, uint16_t v)
{
    vga_outb(port, (uint8_t)v);
    vga_outb(port + 1, (uint8_t)(v >> 8));
}

/* What the monitor shows, latched as a real VGA latches it. The CRT controller takes the
   display start (0Ch, 0Dh) at the start of each vertical retrace, and the game sets the pixel
   panning during the retrace after (vscreen_focus: the start, a wait for the next retrace, then
   the panning), so the new start and its panning reach the screen together, on the frame after
   that retrace. The platform layer scans out from its own thread at the host's refresh, at no
   particular moment in the game's 70 Hz frame, so reading the registers as they stand would
   often show a new start with the old panning, a picture up to three pixels out for one frame,
   which makes a horizontal pan judder (and the start's high byte without its low byte). So the
   scan-out shows the frame after the last retrace to have ended: the start as it was when that
   retrace began and the panning as it was when it ended; during a retrace the frame before
   stays. The game thread keeps the latches (latch_sync, before each register write and each
   read of input status 1) and publishes them with the values since (latch_publish); the
   scan-out, from the counter, takes the latched or the later values. A replay keeps the
   registers as they stand. */
struct latched { uint16_t start, disp_start, cur_start; uint8_t disp_pan, cur_pan; uint64_t rs, re; };
static struct latched lt;
static _Atomic uint32_t lt_seq;
static struct latched lt_pub;

static void retrace_index(uint64_t *rs, uint64_t *re)
{
    uint64_t hz = plat_counter_hz(), t = plat_counter(), frame = hz * 1000 / 70086;
    if (!frame) frame = 1;
    *rs = (t + frame / 10) / frame;     /* retraces begun: each starts 1/10 frame before the frame ends */
    *re = t / frame;                    /* retraces ended */
}

static void latch_sync(void)
{
    uint64_t rs, re;
    uint16_t cur = (uint16_t)(crtc[0x0C] << 8 | crtc[0x0D]);
    if (rp_request == 2) return;
    retrace_index(&rs, &re);
    if (re != lt.re) {                  /* a retrace has ended: its frame is on the screen */
        lt.disp_start = lt.rs == re ? lt.start : cur;
        lt.disp_pan = attr[0x13];
        lt.re = re;
    }
    if (rs != lt.rs) { lt.start = cur; lt.rs = rs; }
}

static void latch_publish(void)
{
    if (rp_request == 2) return;
    lt.cur_start = (uint16_t)(crtc[0x0C] << 8 | crtc[0x0D]);
    lt.cur_pan = attr[0x13];
    atomic_fetch_add_explicit(&lt_seq, 1, memory_order_acq_rel);
    lt_pub = lt;
    atomic_fetch_add_explicit(&lt_seq, 1, memory_order_release);
}

/* the start and panning on the screen now, from the scan-out's thread */
static void latch_read(unsigned *start, uint8_t *pan)
{
    struct latched l;
    uint32_t a, b;
    uint64_t rs, re;
    do {
        a = atomic_load_explicit(&lt_seq, memory_order_acquire);
        l = lt_pub;
        atomic_thread_fence(memory_order_acquire);
        b = atomic_load_explicit(&lt_seq, memory_order_relaxed);
    } while (a != b || (a & 1));
    retrace_index(&rs, &re);
    if (re == l.re) { *start = l.disp_start; *pan = l.disp_pan; return; }
    *start = l.rs == re ? l.start : l.cur_start;
    *pan = l.cur_pan;
}

/* Input status 1: bit 3 vertical retrace, for the last 1.4 ms of each 14.3 ms frame (70 Hz),
   bit 0 not in the display; from the counter, so a wait for retrace takes as long as on the
   real card. Under replay (--replay) nothing the game keeps depends on how long a wait for
   retrace takes (the game clock is the recording's), so the retrace comes from a count of the
   reads instead: of every eight reads the sixth is out of the display and the last two are in
   the retrace, and a replay runs as fast as the host can run it (docs/BUILDING.md, "Testing"). */
static uint8_t status1(void)
{
    static uint32_t reads;
    uint64_t hz, t, frame, ph;
    uint8_t s = 0;
    if (rp_request == 2) {
        uint32_t k = ++reads & 7;
        return k >= 6 ? 0x09 : k == 5 ? 0x01 : 0;
    }
    hz = plat_counter_hz(); t = plat_counter();
    frame = hz * 1000 / 70086; ph = t % (frame ? frame : 1);
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
    case 0x3DA: attr_flip = 0; latch_sync(); return status1();
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

/* The CPU's window onto video memory, A000:0000, for C that builds pointers into it (CUTS.C's
   delta decoder, through PLANAR_STORE): a 64 KB region of the paragraph map whose bytes are
   never read; a store through a pointer into it is a CPU write at that offset. A far pointer's
   arithmetic wraps its offset at 64 KB, and the decoder's row arithmetic relies on that (a
   row address minus a few bytes, a row table entry plus a column past FFFFh), so the window
   has 64 KB of slack on each side, and a pointer anywhere in the three is the offset it
   would have in DOS, modulo 64 KB. */
static uint8_t window[0x30000];
#define WINDOW (window + 0x10000)

void vga_window_init(void)
{
    pm_add("A000 the VGA window", WINDOW, 0x10000, 0xA000);
}

int vga_in_window(const volatile void *p)
{
    return (const volatile uint8_t *)p >= window && (const volatile uint8_t *)p < window + sizeof window;
}

void port_vga_store(volatile void *p, unsigned char v)
{
    if (!vga_in_window(p)) {
        if (p) {
            port_log("planar store outside the window: %+td from A000:0000\n", (volatile uint8_t *)p - WINDOW);
            *(volatile unsigned char *)p = v;
            return;
        }
        port_fatal("a planar store through a null pointer (MK_FP(0xA000, 0) found no region?)");
    }
    vga_write((uint16_t)((volatile uint8_t *)p - WINDOW), v);
}

/* For the state dump (src/replay/REPLAY.C): a plane of video memory, a CRTC register. */
const uint8_t *vga_plane(int p)
{
    return mem[p & 3];
}

uint8_t vga_reg_crtc(int i)
{
    return crtc[i & 31];
}

/* What the CRT controller shows: 320 pixels across; 200 rows when each row is scanned twice
   (maximum scan line 1, mode 13h and the game's mode 0) or 400 when once (mode 1). Each row
   starts at the display start plus the offset register's words, the pixel panning shifts it,
   and at the line compare the address starts again from 0 (the split screen). Text mode shows
   black. */
static void scan(uint8_t *pix, int *w, int *h, uint8_t rgb6[768], int latched)
{
    int msl = (crtc[9] & 0x1F) + 1, rows = 400 / msl, y, x;
    unsigned start = (unsigned)crtc[0x0C] << 8 | crtc[0x0D], pitch = (unsigned)crtc[0x13] * 2;
    unsigned lc = crtc[0x18] | (crtc[7] & 0x10) << 4 | (crtc[9] & 0x40) << 3;
    uint8_t pel = attr[0x13];
    unsigned pan, addr;
    if (latched && rp_request != 2) latch_read(&start, &pel);
    pan = (pel & 7) >> 1;
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

void vga_scanout(uint8_t *pix, int *w, int *h, uint8_t rgb6[768]) { scan(pix, w, h, rgb6, 1); }

/* the registers as they stand, for a screenshot taken on the game's thread (--shot-at-flip) */
void vga_scanout_now(uint8_t *pix, int *w, int *h, uint8_t rgb6[768]) { scan(pix, w, h, rgb6, 0); }
