/* scalebm_code.c: replaces the code SCALEBM.ASM generates at run time (seg003's buffer L1F3A).
   _seg003_1082 has one of its generators (after L1E06 for four-bit pictures,
   after L1E8C for bytes) write a straight-line scaler for the sprite's width into L1F3A, ending
   it with a ret, and calls it once for each source row it needs. The port cannot run written
   code (no JIT, docs/PORT.md), so it reads the same bytes and does what each instruction says,
   on the emulated registers (x86/asmrt.h). The generators write only these instructions:

     AC lodsb   AA stosb   A4 movsb   46 inc si   4E dec si   83 C6 ib add si,ib
     51 push cx   59 pop cx   36 D7 xlat ss:   88 E0 mov al,ah   88 C4 mov ah,al
     D2 E8 shr al,cl   20 E8 and al,ch   C3 ret

   so anything else stops the port with its address. */
#include <stdio.h>
#include "x86/asmrt.h"

void scalebm_run_generated(uint16_t ip)
{
    const uint8_t *code = CODE003;
    uint16_t at = ip;
    char why[80];
    for (;;) {
        uint8_t op = code[at];
        switch (op) {
        case 0xAC: AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1)); at++; break;
        case 0xAA: wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); at++; break;
        case 0xA4: wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); at++; break;
        case 0x46: SI = inc16(SI); at++; break;
        case 0x4E: SI = dec16(SI); at++; break;
        case 0x83:
            if (code[at + 1] != 0xC6) goto bad;
            SI = add16(SI, (uint16_t)(int16_t)(int8_t)code[at + 2], 0);
            at = (uint16_t)(at + 3);
            break;
        case 0x51: push16(CX); at++; break;
        case 0x59: CX = pop16(); at++; break;
        case 0x36:
            if (code[at + 1] != 0xD7) goto bad;
            AL = rb(pSS, BX + AL);
            at = (uint16_t)(at + 2);
            break;
        case 0x88:
            if (code[at + 1] == 0xE0) AL = AH;
            else if (code[at + 1] == 0xC4) AH = AL;
            else goto bad;
            at = (uint16_t)(at + 2);
            break;
        case 0xD2:
            if (code[at + 1] != 0xE8) goto bad;
            AL = shr8(AL, CL);
            at = (uint16_t)(at + 2);
            break;
        case 0x20:
            if (code[at + 1] != 0xE8) goto bad;
            AL = logic8((uint8_t)(AL & CH));
            at = (uint16_t)(at + 2);
            break;
        case 0xC3:
            return;
        default:
            goto bad;
        }
    }
bad:
    snprintf(why, sizeof why, "SCALEBM's generated code: an instruction the generators do not write (%02X %02X)",
             code[at], code[at + 1]);
    asm_halt_at(0x0090, at, why);
}
