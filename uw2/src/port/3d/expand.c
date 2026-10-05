/* expand.c: replaces part of src/3d/EXPAND.ASM (seg004 module 1), the image decoders, and
   PGCACHE.ASM's uncmp_tab that chooses one by an image's format byte, for cFrmtoRaw. Each is
   written from the assembly; the comments give its labels. EXPAND.ASM's header has the formats.
   The renderer's decodes (PGCACHE.ASM's do_uwobj and do_uwcrit) reach the same C through
   x86/glue.c; exp_8str and exp_5run (formats 4 and 6), which no recorded session decodes, are
   the translation's (3d/expand_x.c), and cFrmtoRaw calls it for them (docs/PORT.md, "One
   implementation per routine").

   The decoders read the image at AX:BP (its size word first) and write one byte per pixel to
   cmpbuf1_start (the uncompressed forms at :0; the run-length forms unpack their words one per
   byte to :0 and decode to :5400h), translating each pixel through uncmp_pal (filled from the
   auxiliary palette, shaded through a lightabs row unless DH is FFh) or, for 8-bit pixels,
   through a lightabs row. They return the output's paragraph, as AX did. Offsets wrap at
   64 KB as SI and DI did. The record decoder's self-modifying ret (L01A6) is a flag. */
#include <stdio.h>
#include "port.h"
#include "x86/asmrt.h"

/* EXPAND.ASM's _uncmp_pal, at seg004's CS:0 (in the code block, mem/fardata.c): the 32-entry
   translation of a 4- or 5-bit image */
extern unsigned char uncmp_pal[];
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

/* a decoder the translation has (exp_8str at 20h, exp_5run at 11Ch), called as uncmp_tab's
   callers call it: AX the image, BP its size word, DS:SI the palette, DH the shading row; on
   seg021's private stack outside the renderer, as cFrmtoRaw is (C3DENTRY.ASM) */
static uint16_t translated(uint16_t entry, uint16_t ax, uint16_t bp, const uint8_t *pal, uint8_t dh)
{
    struct asm_state s;
    uint16_t r;
    asm_save(&s);
    if (!asm_level) {
        SET_SS(0x60B9u + PORT_LOAD_SEG);
        SP = 0x510;
    }
    AX = ax;
    BP = bp;
    SET_DS(pal ? port_fp_seg(pal) : 0);
    SI = pal ? (uint16_t)port_fp_off(pal) : 0;
    DH = dh;
    asm_run_near(0x065C, entry);
    r = AX;
    asm_restore(&s);
    return r;
}

/* PGCACHE.ASM's uncmp_tab: the decoder for format byte bx (2 exp_8run, which has no decoder
   and returns AX as it was; 4 exp_8str; 6 exp_5run; 8 exp_4run; 0Ah exp_4str; 0 int 3) */
uint16_t seg004_uncmp(uint16_t bx, uint16_t ax, uint16_t bp, const uint8_t *pal, uint8_t dh)
{
    char why[80];
    switch (bx) {
    case 2: return ax;
    case 4: return translated(0x0020, ax, bp, pal, dh);
    case 6: return translated(0x011C, ax, bp, pal, dh);
    case 8: return exp_4run(ax, bp, pal, dh);
    case 0x0A: return exp_4str(ax, bp, pal, dh);
    default:
        snprintf(why, sizeof why, "uncmp_tab: image format %u (DOS breaks with int 3 or jumps astray)", bx);
        port_halt(why);
    }
}

/* The renderer's calls (x86/glue.c): the decoders through uncmp_tab (AX the image, BP its size
   word, DS:SI the palette, DH the row; AX the pixels' paragraph back), and exp_5run's calls of
   _63 and _18D, with the registers each takes and leaves (EXPAND.ASM). */
uint32_t glue_expand_4str(void)
{
    AX = exp_4str(AX, BP, pDS + SI, DH);
    return asm_glue_ret();
}

uint32_t glue_expand_4run(void)
{
    AX = exp_4run(AX, BP, pDS + SI, DH);
    return asm_glue_ret();
}

/* _63: DS:SI the palette, CX its rows, AX the image, DH the row; out DS the image, BX 0 (the
   offset of uncmp_pal), SI past the palette, CX 0, ES seg004 */
uint32_t glue_expand_63(void)
{
    build_pal(pDS + SI, CX, DH);
    SI = (uint16_t)(SI + CX * 16);
    CX = 0;
    BX = 0;
    SET_DS(AX);
    SET_ES(SEG004_CS);
    return asm_glue_ret();
}

/* _18D: CX words at DS:SI (cmpbuf1_start:0) decoded to ES:DI (cmpbuf1_start:5400h), BX 0;
   out SI and DI past them, BP the input's end, DX 3 */
uint32_t glue_expand_18d(void)
{
    if (pDS != cmpbuf1_start || pES != cmpbuf1_start || SI != 0 || DI != 0x5400)
        port_halt("the record decoder called on other buffers than exp_4run's and exp_5run's");
    run(CX);
    SI = si;
    DI = di;
    BP = bp_end;
    DX = 3;
    return asm_glue_ret();
}
