/* divfault.c: asm_divfault for x86/asmrt.c: the divide fault handlers the 3D renderer
   installs. In DOS a divide that overflows raises int 0, and the renderer points int 0 at the
   handler of the moment, whose offset it keeps at FD71:05A5: saturate, halve and divide again,
   flag the polygon, or drop the interrupt frame and jump into a clipping path. Kept apart from
   asmrt.c, the machine, which is Exhume's runtime/port/x86/asmrt.c (Exhume's
   examples/uw2/prove-port.sh uses this file with it). */
#include <stdio.h>
#include "asmrt.h"

/* The int 0 handlers of seg004 (INSTANCE.ASM, SMOOTH.ASM, INTERP.ASM, PROJPOLY.ASM), by the
   offset the renderer keeps at FD71:05A5. Each skips the faulting instruction by a fixed
   length; a fault in an instruction of another length would resume in its middle in DOS, so
   the port stops there instead. */
#define D71 ((uint8_t *)dseg062_62a6)

uint32_t asm_divfault(uint16_t seg, uint16_t ip, unsigned len)
{
    uint16_t h = rw(D71, 0x5A5);
    switch (h) {
    case 0x3C91:                       /* overflow_handler_reg: AX = +-7FFFh by DX's sign */
        if (len != 2) break;
        AX = (int16_t)DX < 0 ? 0x8001 : 0x7FFF;
        DX = (int16_t)AX < 0 ? 0xFFFF : 0;
        ww(CODE004, 0x3C8F, (uint16_t)(rw(CODE004, 0x3C8F) + 1));   /* overflow_count */
        return 0;
    case 0x3CB5:                       /* overflow_handler_mem */
        if (len != 4) break;
        AX = 0x7FFF;
        DX = 0;
        return 0;
    case 0x3CCB:                       /* overflow_handler_special_bp: by DX xor BP's sign */
        if (len != 2) break;
        AX = (int16_t)(DX ^ BP) < 0 ? 0x8001 : 0x7FFF;
        DX = 0;
        return 0;
    case 0x0D83:                       /* SMOOTH.ASM: halve the dividend and divide by CX again */
        if (len != 2) break;
        for (;;) {
            uint32_t n = (uint32_t)DX << 16 | AX;
            n = (uint32_t)((int32_t)n >> 1);
            DX = (uint16_t)(n >> 16);
            AX = (uint16_t)n;
            if (DX == CX) { AX = 0x7FFF; DX = 0; return 0; }
            if (!asm_idiv16(CX)) return 0;
        }
    case 0x4EFF:                       /* divide_overflow_handler (PROJPOLY.ASM): flag the polygon */
        wb(CODE004, 0x4EFE, 1);        /* _divide_overflow_has_occurred */
        EAX = 0x30000000u;
        return 0;
    case 0x267D:                       /* clip_overflow (INTERP.ASM) */
    case 0x0798:                       /* SMOOTH.ASM's recovery */
    case 0x11AB: case 0x11AD:          /* do_lnres's */
        /* the CPU pushes FLAGS, CS and IP; the handler drops them (sti; add sp,6) */
        push16(asm_flags());
        push16(seg + PORT_LOAD_SEG);
        push16(ip);
        return ASM_JMP(0x065C, h);
    default:
        break;
    }
    {
        char why[120];
        snprintf(why, sizeof why, "divide fault with handler %04X at FD71:05A5 (instruction of %u bytes)", h, len);
        asm_halt_at(seg, ip, why);
    }
}
