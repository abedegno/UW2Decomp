/* perspmap.c: perspective-correct texturing for the port, --enhance perspective (docs/ENHANCEMENTS.md).
   Port only; the DOS game has none of it.

   UW1's two texture mappers, gfx_texture_poly_affine (WALLMAP.ASM, entry 01DA: floors, ceilings and
   most of what is not a wall) and gfx_texture_poly_wall (POLYFILL.ASM, entry 0545), step u and v
   linearly across the screen (the wall one takes u from a per-column table), which is why floors
   swim and walls bend when the view pitches. persp_walk does their work with u and v
   perspective-correct, entered by asm2c overrides at the first instruction of each body (01F5 and
   056B, after the original's own stack switch) and leaving by the original's exit (0363, 0898).

   It is a C translation of the walk in SiENcE's uwpatch (https://github.com/SiENcE/uwpatch,
   patch/perspective.asm, MIT; it builds on OpenAbyss's src/uw_texmap.c), routine for routine,
   keeping its arithmetic: what it keeps of the original is the walk, which decides the pixels a
   face covers, so every face covers exactly the pixels it did; what it changes is the texel each
   pixel takes. Along the edges and across each span it carries Q = 2^30 / z, S = u * Q >> 14 and
   T = v * Q >> 14, which are linear on the screen, and recovers u and v by division every 16
   pixels, stepping them linearly between; u and v are clamped to the texture, and a division whose
   quotient would not fit saturates.

   Its inputs, all where the original mappers read them (each offset is read by both in the
   generated wallmap.c and polyfill.c): CX the vertex count; in the module's data segment (DS), the
   projected vertices at 0659 (14 bytes each: +0 screen x, +2 screen y, +4 u, +6 v, +8 x, +0A y,
   +0C z), the frame buffer's segment at 095A and its row offsets at 095E; ES:[B07C] the shape
   record (+0 the width, +2 the height part of v's limit, +4 the texture's segment, +6 v's mask).
   It writes, as the originals do, the ring's end (07AB), its last vertex (07A9), the vertices the
   walk may still use (07D1) and the row (07CF). */
#include <stdint.h>
#include <stdlib.h>
#include "x86/asmrt.h"

/* A test switch, UW1PORT_TEXTURE_PROBE (Exhume's tools/enhcheck.py coverage): each face, from either
   mapper, is drawn in one colour of its own (a count of the faces, 1..254), so a frame shows which
   pixels each face covers; the original mappers and this one must give the same frame. */
int persp_probe(void)
{
    static int on = -1;
    if (on < 0) on = getenv("UW1PORT_TEXTURE_PROBE") != NULL;
    return on;
}

uint8_t persp_probe_colour;

void persp_probe_next(void)
{
    if (persp_probe()) persp_probe_colour = (uint8_t)(persp_probe_colour % 254 + 1);
}

#define RING        0x659u
#define RING_END    0x7ABu
#define RING_LAST   0x7A9u
#define ROW         0x7CFu
#define LEFT_N      0x7D1u
#define FB_SEG      0x95Au
#define ROW_TAB     0x95Eu
#define SHAPE       0xB07Cu
#define SPAN        16

struct edge {
    uint16_t ptr;               /* the vertex it heads for */
    int16_t end;                /* the row it ends at */
    int32_t x, dx;              /* x, 16.16, and its step per row */
    int32_t q[3], dq[3];        /* Q, S, T and their steps */
};

static struct {
    struct edge e[2];           /* 0 the left edge, 1 the right */
    int32_t qc[3], dqc[3];      /* the span's Q, S, T at the current pixel, and their steps */
    int wall;                   /* the wall mapper's rules */
    int32_t u1, v1;             /* u*256 and v*256 at the current pixel */
    int32_t ulim, vlim;         /* the largest the texture has */
    uint16_t texseg, mask;
    int16_t count;              /* the span's pixels still to draw */
} f;

static uint16_t ds16(uint32_t a) { return rw(pDS, a); }
static int16_t row(void) { return (int16_t)rw(pDS, ROW); }

/* a 32-bit difference as the assembly's sub eax takes it (wrapping), for its idiv */
static int32_t diff32(int32_t a, int32_t b) { return (int32_t)((uint32_t)a - (uint32_t)b); }

/* the assembly's idiv: the same quotient wherever it gives one; where x86 would fault (INT32_MIN
   by -1) the quotient wraps instead of stopping the program, as C's own division would */
static int32_t div32(int32_t a, int32_t b) { return (int32_t)(uint32_t)((int64_t)a / b); }

/* vattr: Q, S, T of the vertex at bx */
static void vattr(uint16_t bx, int32_t out[3])
{
    uint32_t z = ds16(bx + 0xC), q;
    if (z == 0) z = 1;                          /* z = 0 never survives the clip */
    q = 0x40000000u / z;
    out[0] = (int32_t)q;
    out[1] = (int32_t)(uint32_t)(((uint64_t)ds16(bx + 4) * q) >> 14);
    out[2] = (int32_t)(uint32_t)(((uint64_t)ds16(bx + 6) * q) >> 14);
}

/* seed: edge i takes its next vertex; from_qst, the wall rules' reseed, keeps its x. 0 when the
   ring is used up. */
static int seed(int i, int from_qst)
{
    struct edge *e = &f.e[i];
    for (;;) {
        uint16_t bx = e->ptr;
        int16_t cx;
        int32_t t[3];
        int k;
        if (!from_qst) e->x = (int32_t)(((uint32_t)ds16(bx) << 16) | 0x8000u);   /* one half */
        from_qst = 0;
        vattr(bx, e->q);
        bx = (uint16_t)(bx + (i ? 14 : -14));                                  /* right forward, left back */
        if (bx < RING) bx = ds16(RING_LAST);
        if (bx == ds16(RING_END)) bx = RING;
        e->ptr = bx;
        ww(pDS, LEFT_N, (uint16_t)(ds16(LEFT_N) - 1));
        if (ds16(LEFT_N) == 0) return 0;
        e->end = (int16_t)ds16(bx + 2);
        cx = (int16_t)(row() - e->end);
        if (cx == 0) continue;                  /* no rows: the next vertex */
        {
            /* x's step as the original takes it: the whole part, then the remainder halved into the
               high word, divided, doubled */
            int16_t d = (int16_t)(ds16(bx) - (uint16_t)((uint32_t)e->x >> 16));
            int16_t q1 = (int16_t)(d / cx), r1 = (int16_t)(d % cx);
            int16_t fr = (int16_t)(((int32_t)r1 * 0x8000) / cx);
            e->dx = (int32_t)((uint32_t)fr * 2u + ((uint32_t)(uint16_t)q1 << 16));
        }
        vattr(bx, t);
        for (k = 0; k < 3; k++) e->dq[k] = div32(diff32(t[k], e->q[k]), cx);
        return 1;
    }
}

static int reseed(int i) { return seed(i, f.wall); }

static void advance(int i)
{
    struct edge *e = &f.e[i];
    int k;
    e->x = (int32_t)((uint32_t)e->x + (uint32_t)e->dx);
    for (k = 0; k < 3; k++) e->q[k] = (int32_t)((uint32_t)e->q[k] + (uint32_t)e->dq[k]);
}

/* sample: S or T at the current pixel to u*256 or v*256, clamped to 0..lim */
static int32_t sample(int32_t value, int32_t lim)
{
    int32_t hi = value >> 10, half = f.qc[0] >> 1;
    int64_t q;
    if (hi >= half) return lim;                 /* the quotient would not fit */
    if (hi <= -half) return 0;
    q = ((int64_t)value * (1 << 22)) / f.qc[0];
    if ((int32_t)q < 0) return 0;
    if ((uint32_t)(int32_t)q > (uint32_t)lim) return lim;
    return (int32_t)q;
}

static void uvc(void)
{
    f.u1 = sample(f.qc[1], f.ulim);
    f.v1 = sample(f.qc[2], f.vlim);
}

void persp_walk(int wall)
{
    uint16_t n = CX, si, top, sp_ = SHAPE;
    uint8_t *fb, *tex;
    int k;
    f.wall = wall;
    /* the ring: its end, its last vertex, the n+1 the walk may use */
    ww(pDS, RING_END, (uint16_t)(RING + n * 14u));
    ww(pDS, RING_LAST, (uint16_t)(RING + n * 14u - 14u));
    ww(pDS, LEFT_N, (uint16_t)(n + 1));
    /* the shape record: width, v's limit, the texture's segment, v's mask */
    si = rw(pES, sp_);
    f.ulim = (int32_t)(((uint32_t)rw(pES, si) << 16) - 1u);
    f.vlim = (int32_t)(((uint32_t)rw(pES, si + 2) << 8) | 0xFFu);
    f.texseg = rw(pES, si + 4);
    f.mask = rw(pES, si + 6);
    /* the vertex with the greatest y, the first of equals */
    top = RING;
    for (si = RING, k = 0; k < n; k++, si = (uint16_t)(si + 14))
        if ((int16_t)ds16(si + 2) > (int16_t)ds16(top + 2)) top = si;
    fb = asm_segbase(ds16(FB_SEG));
    tex = asm_segbase(f.texseg);
    f.e[0].ptr = f.e[1].ptr = top;
    ww(pDS, ROW, ds16(top + 2));
    if (!seed(0, 0) || !seed(1, 0)) return;
    for (;;) {
        uint16_t x0 = (uint16_t)((uint32_t)f.e[0].x >> 16);
        int16_t w = (int16_t)((uint16_t)((uint32_t)f.e[1].x >> 16) - x0);
        uint16_t di = (uint16_t)(rw(pDS, (uint16_t)(ROW_TAB + 2u * (uint16_t)row())) + x0);
        int32_t div = w ? w : 1;                /* a one-pixel span divides by one */
        int16_t count;
        for (k = 0; k < 3; k++) {
            f.qc[k] = f.e[0].q[k];
            f.dqc[k] = div32(diff32(f.e[1].q[k], f.e[0].q[k]), div);
        }
        count = (int16_t)(w + 1);               /* the width as a pixel count */
        if (count <= 0) {
            if (!f.wall) return;                /* backward: the affine mapper ends the face, */
            goto next;                          /* the wall's passes the row over */
        }
        f.count = count;
        uvc();
        do {
            int32_t u0 = f.u1, v0 = f.v1, du, dv, u, v;
            int16_t c = f.count > SPAN ? SPAN : f.count;
            f.count = (int16_t)(f.count - c);
            for (k = 0; k < 3; k++) f.qc[k] = (int32_t)((uint32_t)f.qc[k] + (uint32_t)f.dqc[k] * (uint32_t)c);
            uvc();
            du = div32(diff32(f.u1, u0), c);
            dv = div32(diff32(f.v1, v0), c);
            u = u0;
            v = v0;
            while (c-- > 0) {
                uint16_t t = (uint16_t)((uint16_t)((uint32_t)v >> 8) & f.mask);
                t = (uint16_t)(t + (uint16_t)((uint32_t)u >> 16));
                wb(fb, di, persp_probe() ? persp_probe_colour : rb(tex, t));
                di = (uint16_t)(di + 1);
                u = (int32_t)((uint32_t)u + (uint32_t)du);
                v = (int32_t)((uint32_t)v + (uint32_t)dv);
            }
        } while (f.count != 0);
next:
        /* the row done: both edges on, a reseed where an edge ends */
        ww(pDS, ROW, (uint16_t)(row() - 1));
        advance(0);
        advance(1);
        if (row() <= f.e[0].end && !reseed(0)) return;
        if (row() <= f.e[1].end && !reseed(1)) return;
    }
}
