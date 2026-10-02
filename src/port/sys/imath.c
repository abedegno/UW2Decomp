/* imath.c: replaces src/sys/IMATH.ASM (seg021 module 14) and its C entries in C3DENTRY.ASM
   (cSinCos, cFstSinCos, cAtan2, cSqRt): the integer sines and cosines, arcsines, the arctangent
   of a unit vector and the integer square root. The tables are seg021's own, in dseg062_62a6
   as the EXE has them: 05B0 the sine at each 1/256 turn (257 words; 0630 is the cosine), 0832
   the arcsine and 0934 the arccosine of 0 .. 1.0 in 128 steps. An angle is a word, 10000h to
   the turn; a sine is 1.15. The arithmetic is the 8086's: imul's 32-bit product taken from
   bit 8 (`mov al,ah; mov ah,dl`), and div with its 17-bit carry folded back by rcr. */
#include "compat.h"
#include "sys.h"
#include "port.h"

#define D dseg062_62a6
#define TW(o) ((uint16_t)(D[(o)] | D[(o) + 1] << 8))

/* (t0 + (t1 - t0) * frac) with the product's middle word, as imul and the byte moves give it */
static uint16_t lerp(uint16_t at, uint16_t frac)
{
    uint16_t t0 = TW(at), t1 = TW(at + 2);
    int32_t p = (int32_t)(int16_t)(uint16_t)(t1 - t0) * (int16_t)frac;
    return (uint16_t)((uint16_t)(p >> 8) + t0);
}

/* _A38, sincos: the sine and cosine of bx, interpolated by its low byte. */
static void sincos(uint16_t bx, uint16_t *ax, uint16_t *bxo)
{
    uint16_t cx = bx & 0xFF, i = (uint16_t)((bx >> 8) << 1);
    *ax = lerp((uint16_t)(0x5B0 + i), cx);
    *bxo = lerp((uint16_t)(0x630 + i), cx);
}

void cSinCos(int angle, int16 *x, int16 *y)
{
    uint16_t a, b;
    sincos((uint16_t)angle, &a, &b);
    *x = (int16)a;
    *y = (int16)b;
}

/* _A69, fast_sincos: the entry for the angle's high byte. */
void cFstSinCos(int angle, int16 *a, int16 *b)
{
    uint16_t i = (uint16_t)((((uint16_t)angle) >> 8) << 1);
    *a = (int16)TW(0x5B0 + i);
    *b = (int16)TW(0x630 + i);
}

/* one Newton step: (g + v / g) >> 1, the sum's carry kept as rcr keeps it */
static uint16_t step16(uint32_t v, uint16_t g)
{
    uint32_t q = v / g;
    return (uint16_t)(((uint32_t)g + (uint16_t)q) >> 1);
}

/* _A78, lsqrt: the square root of the 32-bit value CX:BX, five Newton steps from a guess by
   the highest non-zero byte. */
static uint16_t lsqrt(uint32_t v)
{
    uint16_t cx = (uint16_t)(v >> 16), bx = (uint16_t)v, g;
    int i;
    if (cx >> 8) {
        g = cx < 0x4000 ? 0x4000 : 0xFFFF;
        for (i = 0; i < 5; i++) g = step16(v, g);
        return g;
    }
    if (cx & 0xFF) {
        g = 0x400;
        for (i = 0; i < 5; i++) g = step16(v, g);
        return g;
    }
    if (bx >> 8) {
        g = 0x40;
        for (i = 0; i < 5; i++) g = step16(bx, g);
        return g;
    }
    if (bx & 0xFF) {
        /* div cl: an 8-bit quotient; add cl,al and rcr cl,1 keep 9 bits */
        uint8_t c = 4;
        for (i = 0; i < 5; i++) c = (uint8_t)(((uint16_t)c + (uint8_t)(bx / c)) >> 1);
        return c;
    }
    return 0;
}

int cSqRt(int32 v)
{
    return (int)lsqrt((uint32_t)v);
}

/* _B78: the arcsine of ax, interpolated, with ax's sign. */
static uint16_t asin16(uint16_t ax)
{
    uint16_t dx = (int16_t)ax < 0 ? 0xFFFF : 0, cx;
    ax = (uint16_t)((ax ^ dx) - dx);
    cx = lerp((uint16_t)(0x832 + ((ax >> 8) << 1)), ax & 0xFF);
    return (uint16_t)((cx ^ dx) - dx);
}

/* _BA2: the arccosine of bx, interpolated; a negative bx moves to the other half circle. */
static uint16_t acos16(uint16_t bx)
{
    uint16_t ax = bx, dx = (int16_t)ax < 0 ? 0xFFFF : 0, cx;
    ax = (uint16_t)((ax ^ dx) - dx);
    cx = lerp((uint16_t)(0x934 + ((ax >> 8) << 1)), ax & 0xFF);
    cx = (uint16_t)((cx ^ dx) - dx);
    return (uint16_t)(cx + (dx & 0x8000));
}

/* _BD4, atan2: the angle of the unit vector (sine ax, cosine bx). */
int cAtan2(int x, int y)
{
    int16_t ax = (int16_t)x, bx = (int16_t)y;
    uint16_t cx, dx;
    if (ax <= 0x5A82 && ax >= (int16_t)0xA57E) {
        cx = asin16((uint16_t)ax);
        if (bx < 0) cx = (uint16_t)-(uint16_t)(cx - 0x8000);
        return (int)(int16_t)cx;
    }
    dx = ax < 0 ? 0xFFFF : 0;
    cx = acos16((uint16_t)bx);
    return (int)(int16_t)(uint16_t)((cx ^ dx) - dx);
}
