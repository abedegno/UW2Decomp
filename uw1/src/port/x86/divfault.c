/* divfault.c: asm_divfault for Exhume's x86/asmrt.c: the divide fault handlers the 3D renderer
   installs. In DOS a divide that overflows raises int 0; cRender (C3DENTRY.ASM) points int 0 at
   seg063:04D0, a jmp through the far pointer at seg063:04D5, whose offset the renderer sets to
   the handler of the moment (`mov word ptr ss:[4D5h],...`, SS = seg063; INTERP.ASM's header).
   So the handler's offset in seg004 is the word at seg063:04D5.

   INSTANCE.ASM's three return to the faulting code past the divide (they add 2 or 4 to the IP
   the CPU pushed), with AX = 7FFFh (or its negative) and DX = 0, and leave the flags as the
   iret restores them, the divide's; they are modelled here, and a fault in an instruction of
   another length (which in DOS would resume in its middle) stops the port. The others are
   recovery paths that drop the interrupt frame and jump on (INTERP.ASM, SMOOTH.ASM,
   TMAPOPS.ASM): the CPU's frame is pushed and the handler runs as translated code. */
#include <stdio.h>
#include "x86/asmrt.h"

#define SEG004 0x06E7u

uint32_t asm_divfault(uint16_t seg, uint16_t ip, unsigned len)
{
    uint16_t h = (uint16_t)(seg063[0x4D5] | seg063[0x4D6] << 8);
    char why[120];
    switch (h) {
    case 0x5BD1:                        /* overflow_handler_reg: +2, AX 7FFFh, DX 0, counted */
        if (len != 2) break;
        ww(CODE004, 0x5BCD, BP);        /* L5BCD, where the handler keeps BP */
        AX = 0x7FFF;
        DX = 0;
        ww(CODE004, 0x5BCF, (uint16_t)(rw(CODE004, 0x5BCF) + 1));   /* overflow_count */
        return 0;
    case 0x5BEE:                        /* overflow_handler_mem: +4, AX 7FFFh, DX 0 */
        if (len != 4) break;
        ww(CODE004, 0x5BCD, BP);
        AX = 0x7FFF;
        DX = 0;
        return 0;
    case 0x5C04:                        /* overflow_handler_special_bp: +2, +-7FFFh by DX xor BP */
        if (len != 2) break;
        ww(CODE004, 0x5BCD, BP);
        AX = (int16_t)(DX ^ BP) < 0 ? 0x8001 : 0x7FFF;
        DX = 0;
        return 0;
    default:
        /* the CPU pushes FLAGS, CS and IP; the recovery path drops them itself */
        push16(asm_flags());
        push16((uint16_t)(seg + PORT_LOAD_SEG));
        push16(ip);
        return ASM_JMP(SEG004, h);
    }
    snprintf(why, sizeof why, "divide fault with handler %04X at seg063:04D5 (instruction of %u bytes)", h, len);
    asm_halt_at(seg, ip, why);
}
