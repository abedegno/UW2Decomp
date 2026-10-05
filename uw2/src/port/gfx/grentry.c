/* grentry.c: replaces part of src/gfx/GRENTRY.ASM (seg003 module 3), the 3D view's frame
   buffer: a linear buffer, one byte a pixel, in stdat's segment (its segment at 370D:0958, its
   row offsets at 095C), placed and copied to the planar screen, saved to video memory, and the
   pen for the span writers. Each is written from the assembly; the comments give its labels
   (GRENTRY.ASM's header has the data map). The rest of the module (filling, dimming and lighting
   the frame buffer, its solid and smooth span writers, the copy to the screen by rows) is the
   translation's, gfx/grentry_x.c, which the renderer runs and the C calls here (docs/PORT.md,
   "One implementation per routine").

   setup_frame_buf writes a ret into the unrolled copier _888 after one plane's share of the
   frame's width; here that is state (copier_ret), as the copier is C too. */
#include <stdio.h>
#include "grlib.h"

#define FB ((uint8_t *)port_mk_fp(W(0x958), 0))
#define ROW(y) W(0x95C + (uint16_t)((y) << 1))

static int copier_ret = -1;            /* the byte offset of the ret written into _888, or -1 */

/* _729 (fbuf_draw_ylrpp_x): linear bitmap rows (8-byte records; source segment 55EC) into the
   frame buffer, colour 0 skipped. */
void seg003_0272_729(uint16_t si)
{
    uint8_t *fb = FB, *src = port_mk_fp(W(0x55EC), 0);
    uint16_t ax, di, cx, s;
    if (!fb || !src) port_fatal("fbshow: a segment in no region");
    for (;;) {
        ax = W(si); si += 2;
        if (ax & 0x8000) return;
        di = ROW(ax);
        ax = W(si); si += 2;
        di = (uint16_t)(di + ax);
        cx = (uint16_t)(W(si) - ax + 1); si += 2;
        s = W(si); si += 2;
        do {
            uint8_t v = src[s++];
            if (v) fb[di] = v;
            di++;
        } while (--cx);
    }
}

/* _7A8, setup_frame_buf (cPlaceFB): BX = the width, CX = the height. Rows of width + 2 bytes
   from offset 2, the rows from the height up all at 69D4h, the last row in 0AF0, and the
   copier's ret moved to after the new width (when it is below 320). The width is compared with
   0AEE before 2 is taken off it again, so the copier is always re-patched. */
void seg003_0272_7A8(uint16_t bx, uint16_t cx)
{
    uint16_t di = 0x95C, dx = cx, ax = 2;
    bx = (uint16_t)(bx + 2);
    while (cx--) { SETW(di, ax); di += 2; ax = (uint16_t)(ax + bx); }
    cx = (uint16_t)(0xC8 - dx);
    SETW(0xAF0, (uint16_t)(dx - 1));
    while (cx--) { SETW(di, 0x69D4); di += 2; }
    if (bx == W(0xAEE)) return;
    if ((int16_t)W(0xAEE) < 0x140) copier_ret = -1;      /* the old ret back to movsb */
    bx = (uint16_t)(bx - 2);
    if ((int16_t)bx >= 0x140) return;
    if (bx & 3) port_halt("setup_frame_buf: a width not a multiple of 4 puts the ret inside an instruction");
    copier_ret = bx;
    SETW(0xAEE, bx);
}

/* _888: every fourth byte of a frame-buffer row to the screen, 80 times, or up to the ret */
static void copier(const uint8_t *fb, uint16_t si, uint16_t di)
{
    int n = copier_ret < 0 ? 80 : copier_ret / 4, i;
    for (i = 0; i < n; i++) {
        vga_write(di++, fb[si]);
        si = (uint16_t)(si + 4);
    }
}

/* _7F9, cFBtoScreen: from the frame's last row down to the window's bottom edge (3DFA), each
   row to the screen at A000:[94E] + [383A], 80 bytes a row, four passes with map masks 8, 2, 4
   and 1 from source offsets 3, 1, 2 and 0. */
void seg003_0272_7F9(void)
{
    uint8_t *fb = FB;
    int16_t bp = (int16_t)W(0xAF0);
    uint16_t bx = (uint16_t)(W(0x94E) + W(0x383A)), cx;
    do {
        cx = ROW(bp);
        vga_outb(SC_DATA, 8); copier(fb, (uint16_t)(cx + 3), bx);
        vga_outb(SC_DATA, 2); copier(fb, (uint16_t)(cx + 1), bx);
        vga_outb(SC_DATA, 4); copier(fb, (uint16_t)(cx + 2), bx);
        vga_outb(SC_DATA, 1); copier(fb, cx, bx);
        bx = (uint16_t)(bx + 0x50);
        bp--;
    } while (bp > SW(0x3DFA));
}

/* _9FC (fbuf_save_vylr): frame-buffer spans, widened to whole groups, to video memory from 4116,
   one plane at a time. */
static void fb_save(uint16_t bp)
{
    uint8_t *fb = FB, al;
    uint16_t bx, si, cx, di = W(0x4116), start, n;
    int p;
    for (;;) {
        bx = W(bp);
        if (bx & 0x8000) return;
        bx = (uint16_t)(bx << 1);
        si = (uint16_t)(W(bp + 2) & 0xFFFC);
        cx = (uint16_t)((W(bp + 4) & 0xFFFC) + 4);
        bp += 6;
        cx = (uint16_t)(cx - si);
        si = (uint16_t)(si + W(0x95C + bx));
        bx = cx;
        start = di;
        al = 1;
        for (p = 0; p < 4; p++) {
            if (p) { si = (uint16_t)(si - bx + 1); di = start; al = (uint8_t)(al << 1); }
            vga_outb(SC_DATA, al);
            for (n = (uint16_t)(bx >> 2); n; n--) { vga_write(di++, fb[si]); si = (uint16_t)(si + 4); }
        }
    }
}

/* _B9B, fbuf_setcolor: pen AX's colour (4110) and second word (4116), and for a mode below 14h
   the frame buffer's span writer from the table at 0AF2. */
void seg003_0272_B9B(uint16_t ax)
{
    uint16_t bx;
    if (ax & 0x8000) return;
    bx = (uint16_t)(ax << 1);
    SETW(0x4110, W(0x21E + bx));
    SETW(0x4116, W(0x434 + bx));
    bx = W(0x8 + bx);
    if (bx >= 0x14) return;
    SETW(0x4112, W(0xAF2 + bx));
}

/* the frame buffer's span writers for seg003_call (grcore.c) */
int seg003_fb_span(uint16_t off, uint16_t si)
{
    switch (off) {
    case 0x9FC: fb_save(si); return 1;
    case 0x729: seg003_0272_729(si); return 1;
    default: return 0;
    }
}

/* the translation's routines the C calls (c3dentry.c's cFillFB, cDimFB and cLiteFB) */
void seg003_0272_6E4(uint8_t al)
{
    struct seg003_regs r = { 0 };
    r.ax = al;
    seg003_asm_far(0x6E4, r);
}

void seg003_0272_764(uint16_t cx)
{
    struct seg003_regs r = { 0 };
    r.cx = cx;
    seg003_asm_far(0x764, r);
}

void seg003_0272_788(uint16_t cx)
{
    struct seg003_regs r = { 0 };
    r.cx = cx;
    seg003_asm_far(0x788, r);
}

