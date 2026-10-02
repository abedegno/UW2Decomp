/* expand.c: replaces src/3d/EXPAND.ASM (seg004 module 1), the image decoders, and PGCACHE.ASM's
   uncmp_tab that chooses one by an image's format byte. Each is written from the assembly; the
   comments give its labels. EXPAND.ASM's header has the formats.

   The decoders read the image at AX:BP (its size word first) and write one byte per pixel to
   cmpbuf1_start (the uncompressed forms at :0; the run-length forms unpack their words one per
   byte to :0 and decode to :5400h), translating each pixel through uncmp_pal (filled from the
   auxiliary palette, shaded through a lightabs row unless DH is FFh) or, for 8-bit pixels,
   through a lightabs row. They return the output's paragraph, as AX did. Offsets wrap at
   64 KB as SI and DI did. The record decoder's self-modifying ret (L01A6) is a flag. */
#include <stdio.h>
#include "port.h"

/* EXPAND.ASM's _uncmp_pal, at seg004's CS:0: the 32-entry translation of a 4- or 5-bit image */
unsigned char uncmp_pal[32] = {
    0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,
    0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,
};

extern unsigned char cmpbuf1_start[];
extern unsigned char lightabs[];

static uint8_t *src_seg;                /* DS: the image, then cmpbuf1_start */
static uint8_t *out;                    /* ES: cmpbuf1_start */
static uint16_t si, di, bp_end;
static int patched;                     /* L01A6 is a ret */

#define LODSB() (src_seg[si++])
#define STOSB(v) (out[di++] = (uint8_t)(v))

/* _63: uncmp_pal from the aux palette (rows of 16), through lightabs row DH unless DH is FFh */
static void build_pal(const uint8_t *pal, int rows, uint8_t dh)
{
    int i;
    if (dh != 0xFF && dh >= 0x92) port_halt("an image shading row past seg004's end");
    for (i = 0; i < rows * 16; i++)
        uncmp_pal[i] = dh == 0xFF ? pal[i] : lightabs[dh * 256 + pal[i]];
}

/* the size word at AX:BP */
static uint16_t start(uint16_t ax, uint16_t bp)
{
    uint16_t n;
    src_seg = port_mk_fp(ax, 0);
    if (!src_seg) port_fatal("image decoder: the image's segment %04X is in no region", ax);
    out = cmpbuf1_start;
    si = bp;
    di = 0;
    n = (uint16_t)(LODSB());
    n |= (uint16_t)(LODSB() << 8);
    return n;
}

/* exp_8str (format 4): each byte through lightabs row DH (exp_8str's xlat reads CS:BX + AL with
   BX = lightabs + DH * 256: with DH = FFh that is past lightabs, which the port has not got) */
static uint16_t exp_8str(uint16_t ax, uint16_t bp, uint8_t dh)
{
    uint16_t n = start(ax, bp);
    if (dh >= 0x92) port_halt("exp_8str: a shading row past seg004's end (DOS wraps in the segment)");
    do STOSB(lightabs[dh * 256 + LODSB()]); while (--n);
    return port_fp_seg(cmpbuf1_start);
}

/* exp_4str (format 0Ah): two pixels a byte, high nibble first */
static uint16_t exp_4str(uint16_t ax, uint16_t bp, const uint8_t *pal, uint8_t dh)
{
    uint16_t n;
    uint8_t b;
    build_pal(pal, 1, dh);
    n = start(ax, bp);
    do {
        b = LODSB();
        STOSB(uncmp_pal[b >> 4]);
        STOSB(uncmp_pal[b & 15]);
    } while (--n);
    return port_fp_seg(cmpbuf1_start);
}

static void decode(void);

/* the four-word extended count of L01B5, L01DD and L0230: the byte loaded into AL, then three
   more ORed into AL after each shift of AX by 4, SI past them */
static uint16_t count4(void)
{
    uint16_t ax = LODSB();
    ax = (uint16_t)(ax << 4); ax = (uint16_t)((ax & 0xFF00) | ((ax | src_seg[si]) & 0xFF));
    ax = (uint16_t)(ax << 4); ax = (uint16_t)((ax & 0xFF00) | ((ax | src_seg[(uint16_t)(si + 1)]) & 0xFF));
    ax = (uint16_t)(ax << 4); ax = (uint16_t)((ax & 0xFF00) | ((ax | src_seg[(uint16_t)(si + 2)]) & 0xFF));
    si = (uint16_t)(si + 3);
    return ax;
}

/* a two-word extended count: (X << 4) | Y */
static uint16_t count2(uint8_t x)
{
    uint16_t cx = (uint16_t)(x << 4);
    return (uint16_t)(cx | LODSB());
}

/* _194: one repeat record, then (at L01A6, unless patched) one run record, until SI reaches BP */
static void decode(void)
{
    uint16_t cx;
    uint8_t al, v;
    for (;;) {
        if (si >= bp_end) return;
        al = LODSB();
        if (al > 2) {
            cx = al;
            v = uncmp_pal[LODSB()];
            while (cx--) STOSB(v);
        } else if (al == 2) {
            /* L0213: a count of repeat records, each decoded by a call that stops at L01A6 */
            patched = 1;
            al = LODSB();
            cx = al;
            if (!cx) {
                al = LODSB();
                cx = al ? count2(al) : count4();
            }
            do decode(); while (--cx);
            patched = 0;
        } else if (al == 0) {
            al = LODSB();
            cx = al ? count2(al) : count4();
            v = uncmp_pal[LODSB()];
            while (cx--) STOSB(v);
        }
        /* L01A6 */
        if (patched) return;
        cx = LODSB();
        if (!cx) {
            al = LODSB();
            cx = al ? count2(al) : count4();
        }
        do STOSB(uncmp_pal[LODSB()]); while (--cx);
    }
}

/* _18D: decode the n unpacked words at cmpbuf1_start:0 into cmpbuf1_start:5400h */
static uint16_t run(uint16_t n)
{
    src_seg = cmpbuf1_start;
    out = cmpbuf1_start;
    si = 0;
    di = 0x5400;
    bp_end = n;
    patched = 0;
    decode();
    return (uint16_t)(port_fp_seg(cmpbuf1_start) + 0x540);
}

/* exp_4run (format 8): the size counts nibbles; unpack (size + 1) / 2 bytes, then decode */
static uint16_t exp_4run(uint16_t ax, uint16_t bp, const uint8_t *pal, uint8_t dh)
{
    uint16_t n, cx;
    uint8_t b;
    build_pal(pal, 1, dh);
    n = start(ax, bp);
    cx = (uint16_t)((uint16_t)(n + 1) >> 1);
    do {
        b = LODSB();
        STOSB(b >> 4);
        STOSB(b & 15);
    } while (--cx);
    return run(n);
}

static uint16_t ror16(uint16_t v, int n) { return (uint16_t)((v >> n) | (v << (16 - n))); }
static uint16_t rol16(uint16_t v, int n) { return (uint16_t)((v << n) | (v >> (16 - n))); }

/* exp_5run (format 6): five bytes to eight 5-bit words a pass, (size + 7) / 8 passes, with the
   assembly's rotates; then decode */
static uint16_t exp_5run(uint16_t ax, uint16_t bp, const uint8_t *pal, uint8_t dh)
{
    uint16_t n, cx, a;
    build_pal(pal, 2, dh);
    n = start(ax, bp);
    cx = (uint16_t)((uint16_t)(n + 7) >> 3);
    do {
        a = LODSB();                                    /* xor ah,ah */
        a = ror16(a, 3); STOSB(a);
        a = (uint16_t)((a & 0xFF00) | LODSB());
        a = (uint16_t)(((a >> 8) >> 5) << 8 | (a & 0xFF));  /* shr ah,5 */
        a = ror16(a, 6); STOSB(a);
        a = (uint16_t)(a >> 3);
        a = (uint16_t)((a << 8) | (a >> 8)); STOSB(a);  /* xchg ah,al */
        a = (uint16_t)((a & 0xFF00) | LODSB());
        a = (uint16_t)(((a >> 8) >> 7) << 8 | (a & 0xFF));  /* shr ah,7 */
        a = ror16(a, 4); STOSB(a);
        a = (uint16_t)((a & 0xFF00) | LODSB());
        a = (uint16_t)(((a >> 8) >> 4) << 8 | (a & 0xFF));  /* shr ah,4 */
        a = ror16(a, 7); STOSB(a);
        a = (uint16_t)(a & 0xFF00);                     /* xor al,al */
        a = rol16(a, 5); STOSB(a);
        a = (uint16_t)((a & 0xFF00) | LODSB());
        a = (uint16_t)(((a >> 8) >> 6) << 8 | (a & 0xFF));  /* shr ah,6 */
        a = ror16(a, 5); STOSB(a);
        a = (uint16_t)(a >> 11); STOSB(a);
    } while (--cx);
    return run(n);
}

/* PGCACHE.ASM's uncmp_tab: the decoder for format byte bx (2 exp_8run, which has no decoder
   and returns AX as it was; 4 exp_8str; 6 exp_5run; 8 exp_4run; 0Ah exp_4str; 0 int 3) */
uint16_t seg004_uncmp(uint16_t bx, uint16_t ax, uint16_t bp, const uint8_t *pal, uint8_t dh)
{
    char why[80];
    switch (bx) {
    case 2: return ax;
    case 4: return exp_8str(ax, bp, dh);
    case 6: return exp_5run(ax, bp, pal, dh);
    case 8: return exp_4run(ax, bp, pal, dh);
    case 0x0A: return exp_4str(ax, bp, pal, dh);
    default:
        snprintf(why, sizeof why, "uncmp_tab: image format %u (DOS breaks with int 3 or jumps astray)", bx);
        port_halt(why);
    }
}
