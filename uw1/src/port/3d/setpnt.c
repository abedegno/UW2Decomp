/* setpnt.c: replaces src/3d/SETPNT.ASM (UW2's byte for byte; UW2Decomp's setpnt.c), SetPnt(x, y, z): one do_minires record (opcode 0B6h) at
   dbptr: the word 0B6h, the bytes (x - 10h) << 3 and y << 3, then z << 1 and ptnuminq's low
   byte; dbptr moves on 6 bytes; returns ptnuminq before incrementing it. */
#include "compat.h"
#include "view3d.h"
#include "conv.h"
#include "sys.h"

extern int16 ptnuminq;                  /* GRIDDB.C */

int SetPnt(char x, char y, char z)
{
    unsigned char *p = (unsigned char *)dbptr;
    int n = ptnuminq;
    p[0] = 0xB6;
    p[1] = 0;
    p[2] = (unsigned char)((x - 0x10) << 3);
    p[3] = (unsigned char)(y << 3);
    p[4] = (unsigned char)(z << 1);
    p[5] = (unsigned char)n;
    dbptr = (int16 *)(p + 6);
    ptnuminq++;
    return (int16_t)n;
}
