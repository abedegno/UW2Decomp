/* imath.c: replaces part of src/sys/IMATH.ASM (seg021 module 14) and its C entries in
   C3DENTRY.ASM (cSinCos, cFstSinCos, cAtan2, cSqRt): the integer sines and cosines and the
   integer square root, which both the C and the translated renderer use (docs/PORT.md, "One
   implementation per routine"); the arcsine, arccosine and arctangent (_B78, _BA2, _BD4) are
   the translation's (sys/imath_x.c), which cAtan2 calls. The tables are seg021's own, in
   dseg062_62a6 as the EXE has them: 05B0 the sine at each 1/256 turn (257 words; 0630 is the
   cosine). An angle is a word, 10000h to the turn; a sine is 1.15. The arithmetic is the
   8086's: imul's 32-bit product taken from bit 8 (`mov al,ah; mov ah,dl`), and div with its
   17-bit carry folded back by rcr. The routines leave the registers as the assembly did, for
   the renderer's far calls (_A30, _A34), which reach them through x86/glue.c. */
#include "compat.h"
#include "sys.h"
#include "port.h"
#include "x86/asmrt.h"

#define D dseg062_62a6
#define TW(o) ((uint16_t)(D[(o)] | D[(o) + 1] << 8))
#define SEG021_DS (0x60B9u + PORT_LOAD_SEG)     /* FD71, seg021's data: dseg062_62a6 */

/* t0 + (t1 - t0) * frac, the product's middle word, as imul and the byte moves give it; *dx
   is the product's high word, as imul leaves DX */
static uint16_t lerp(uint16_t at, uint16_t frac, uint16_t *dx)
{
    uint16_t t0 = TW(at), t1 = TW(at + 2);
    int32_t p = (int32_t)(int16_t)(uint16_t)(t1 - t0) * (int16_t)frac;
    *dx = (uint16_t)((uint32_t)p >> 16);
    return (uint16_t)((uint16_t)(p >> 8) + t0);
}

/* _A38, sincos: AX the sine and BX the cosine of the angle in BX, interpolated by its low
   byte; CX is the low byte, DX the second product's high word, BP the cosine table's entry */
static void sincos(void)
{
    uint16_t cx = BX & 0xFF, i = (uint16_t)((BX >> 8) << 1), dx, s, c;
    s = lerp((uint16_t)(0x5B0 + i), cx, &dx);
    c = lerp((uint16_t)(0x630 + i), cx, &dx);
    AX = s;
    BX = c;
    CX = cx;
    DX = dx;
    BP = TW(0x630 + i);
}

void cSinCos(int angle, int16 *x, int16 *y)
{
    struct asm_state s;
    asm_save(&s);
    BX = (uint16_t)angle;
    sincos();
    *x = (int16)AX;
    *y = (int16)BX;
    asm_restore(&s);
}

/* _A69, fast_sincos: the entry for the angle's high byte. */
void cFstSinCos(int angle, int16 *a, int16 *b)
{
    uint16_t i = (uint16_t)((((uint16_t)angle) >> 8) << 1);
    *a = (int16)TW(0x5B0 + i);
    *b = (int16)TW(0x630 + i);
}

/* _A78, lsqrt: DI = the square root of the 32-bit value CX:BX, five Newton steps
   (g + v / g) >> 1 from a guess by the highest non-zero byte, the sum's carry kept as rcr
   keeps it. AX and DX are the last division's, and in the 16- and 8-bit cases CX is the root.
   A quotient over 16 bits faults in DOS (div): no value the game passes does, and the port
   stops rather than guess what the fault handler of the moment would do. */
static void lsqrt(void)
{
    uint32_t v = (uint32_t)CX << 16 | BX, q;
    uint16_t g;
    int i;
    if (CX) {
        g = CX >> 8 ? (CX < 0x4000 ? 0x4000 : 0xFFFF) : 0x400;
        for (i = 0; i < 5; i++) {
            q = v / g;
            if (q > 0xFFFF) port_halt("lsqrt: a divide overflow (DOS faults)");
            DX = (uint16_t)(v % g);
            AX = (uint16_t)q;
            g = (uint16_t)(((uint32_t)g + AX) >> 1);
        }
        DI = g;
        return;
    }
    if (BX >> 8) {
        g = 0x40;
        for (i = 0; i < 5; i++) {
            AX = (uint16_t)(BX / g);
            DX = (uint16_t)(BX % g);
            g = (uint16_t)(((uint32_t)g + AX) >> 1);
        }
        CX = DI = g;
        return;
    }
    if (BX & 0xFF) {
        uint8_t c = 4;
        for (i = 0; i < 5; i++) {
            AX = (uint16_t)((BX % c) << 8 | (BX / c & 0xFF));     /* div cl: AH the rest, AL the quotient */
            c = (uint8_t)(((uint16_t)c + (AX & 0xFF)) >> 1);
        }
        CX = DI = c;
        return;
    }
    DI = 0;
}

int cSqRt(int32 v)
{
    struct asm_state s;
    int r;
    asm_save(&s);
    CX = (uint16_t)((uint32_t)v >> 16);
    BX = (uint16_t)v;
    lsqrt();
    r = DI;
    asm_restore(&s);
    return r;
}

/* cAtan2: the angle of the unit vector (sine x, cosine y): the translation's _BD4 (atan2),
   with seg021's data as DS and, outside the renderer, seg021's private stack (C3DENTRY.ASM) */
int cAtan2(int x, int y)
{
    struct asm_state s;
    int r;
    asm_save(&s);
    if (!asm_level) {
        SET_SS(SEG021_DS);
        SP = 0x510;
    }
    SET_DS(SEG021_DS);
    AX = (uint16_t)x;
    BX = (uint16_t)y;
    asm_run_near(0x2110, 0x0BD4);
    r = (int16_t)CX;
    asm_restore(&s);
    return r;
}

/* the renderer's far calls (x86/glue.c): _A30, lsqrt, and _A34, sincos, each `call; retf` */
uint32_t glue_imath_a30(void)
{
    lsqrt();
    return asm_glue_retf();
}

uint32_t glue_imath_a34(void)
{
    sincos();
    return asm_glue_retf();
}
