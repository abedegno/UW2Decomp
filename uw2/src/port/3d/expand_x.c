/* expand_x.c: replaces src/3d/EXPAND.ASM (seg004_0849_0, 0000..0256 of its
   segment). Written by tools/asm2c.py from the assembly and the bytes it assembles to; do not
   edit by hand: change the tool, or its OVERRIDES, and run it again. Each instruction is the
   C below its source line; the comments before a routine are the .ASM file's. */
#include "x86/asmrt.h"

static int asm_jcc(uint8_t op)
{
    switch (op & 0x0F) {
    case 0x0: return OF; case 0x1: return !OF; case 0x2: return CF; case 0x3: return !CF;
    case 0x4: return ZF; case 0x5: return !ZF; case 0x6: return CF || ZF; case 0x7: return !CF && !ZF;
    case 0x8: return SF; case 0x9: return !SF; case 0xC: return SF != OF; case 0xD: return SF == OF;
    case 0xE: return ZF || SF != OF; case 0xF: return !ZF && SF == OF;
    default: port_halt("a patched jump on the parity flag");
    }
}

uint32_t asm_mod_EXPAND(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x0020: goto L0020;
    case 0x0023: goto L0023;
    case 0x0025: goto L0025;
    case 0x0028: goto L0028;
    case 0x002A: goto L002A;
    case 0x002D: goto L002D;
    case 0x002E: goto L002E;
    case 0x0030: goto L0030;
    case 0x0031: goto L0031;
    case 0x0033: goto L0033;
    case 0x0034: goto L0034;
    case 0x0036: goto L0036;
    case 0x0038: goto L0038;
    case 0x00CB: goto L00CB;
    case 0x00CE: goto L00CE;
    case 0x00D0: goto L00D0;
    case 0x00D2: goto L00D2;
    case 0x00D6: goto L00D6;
    case 0x00D8: goto L00D8;
    case 0x011C: goto L011C;
    case 0x011F: goto L011F;
    case 0x0122: goto L0122;
    case 0x0124: goto L0124;
    case 0x0127: goto L0127;
    case 0x0129: goto L0129;
    case 0x012C: goto L012C;
    case 0x012D: goto L012D;
    case 0x012F: goto L012F;
    case 0x0132: goto L0132;
    case 0x0135: goto L0135;
    case 0x0137: goto L0137;
    case 0x0138: goto L0138;
    case 0x013A: goto L013A;
    case 0x013D: goto L013D;
    case 0x013E: goto L013E;
    case 0x013F: goto L013F;
    case 0x0142: goto L0142;
    case 0x0145: goto L0145;
    case 0x0146: goto L0146;
    case 0x0149: goto L0149;
    case 0x014B: goto L014B;
    case 0x014C: goto L014C;
    case 0x014D: goto L014D;
    case 0x0150: goto L0150;
    case 0x0153: goto L0153;
    case 0x0154: goto L0154;
    case 0x0155: goto L0155;
    case 0x0158: goto L0158;
    case 0x015B: goto L015B;
    case 0x015C: goto L015C;
    case 0x015E: goto L015E;
    case 0x0161: goto L0161;
    case 0x0162: goto L0162;
    case 0x0163: goto L0163;
    case 0x0166: goto L0166;
    case 0x0169: goto L0169;
    case 0x016A: goto L016A;
    case 0x016D: goto L016D;
    case 0x016E: goto L016E;
    case 0x0170: goto L0170;
    case 0x0172: goto L0172;
    case 0x0175: goto L0175;
    case 0x0177: goto L0177;
    case 0x017A: goto L017A;
    case 0x017D: goto L017D;
    case 0x017F: goto L017F;
    case 0x0182: goto L0182;
    case 0x0185: goto L0185;
    case 0x0187: goto L0187;
    case 0x018A: goto L018A;
    case 0x018B: goto L018B;
    case 0x018C: goto L018C;
    default: asm_bad_entry("EXPAND.ASM", entry);
    }

    /* seg004_0849_20  (+20)
       exp_8str: format 4, 8-bit uncompressed. In: AX = image paragraph, BP = offset of its size
       word, DH = lightabs row. Copies `size` bytes to cmpbuf1_start:0, each translated through
       lightabs row DH (BX from seg004_0849_CB; xlat reads cs:BX+AL). Out: AX = cmpbuf1_start,
       ES:DI past the last pixel. The aux palette is not used: 8-bit pixels are already colours. */
L0020: /* _exp_8str */
    /* 0020  call    _seg004_0849_CB */
    if ((c = asm_call(ASM_JMP(0x065C, 0x00CB), 0x0023)) != 0) return c;
L0023:
    /* 0023  mov     si,bp */
    SI = BP;
L0025:
    /* 0025  mov     dx,seg _cmpbuf1_start */
    DX = (uint16_t)(0x43A0 + PORT_LOAD_SEG);
L0028:
    /* 0028  mov     es,dx */
    SET_ES(DX);
L002A:
    /* 002A  mov     di,0 */
    DI = 0x0;
L002D:
    /* 002D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L002E:
    /* 002E  mov     cx,ax */
    CX = AX;
L0030: /* L0030 */
    /* 0030  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0031:
    /* 0031  xlat    byte ptr cs:_uncmp_pal[bx] */
    AL = rb(CODE004, BX + AL);
L0033:
    /* 0033  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0034:
    /* 0034  loop    L0030 */
    if (--CX) goto L0030;
L0036:
    /* 0036  mov     ax,dx */
    AX = DX;
L0038:
    /* 0038  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_CB  (+CB)
       exp_8str's set-up: BX = offset lightabs + DH * 256 (the shading row), DS = AX (the image).
       The cmp's result is overwritten by the add, so DH = 0FFh is not treated specially here
       (it would index past lightabs' 16 rows); probably no caller asks for an unshaded 8-bit
       image, but that is not checked. */
L00CB: /* _seg004_0849_CB */
    /* 00CB  cmp     dh,0FFh */
L00CE:
    /* 00CE  mov     bh,dh */
    BH = DH;
L00D0:
    /* 00D0  xor     bl,bl */
    BL = (uint8_t)(BL ^ BL);
L00D2:
    /* 00D2  add     bx,offset lightabs */
    BX = add16(BX, 0x6D3E, 0);
L00D6:
    /* 00D6  mov     ds,ax */
    SET_DS(AX);
L00D8:
    /* 00D8  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_11C  (+11C)
       exp_5run: format 6, 5-bit run-length (UW-Formats: some critter frames). In and out as exp_4run, with a
       32-entry uncmp_pal (CX = 2 rows). Unpacks the bit stream, most significant bit first, five
       bytes into eight 5-bit words per iteration, (size + 7) / 8 iterations, with rotates rather
       than a bit loop; then decodes as exp_4run does. */
L011C: /* _exp_5run */
    /* 011C  mov     cx,2 */
    CX = 0x2;
L011F:
    /* 011F  call    _seg004_0849_63 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x0063), 0x0122)) != 0) return c;
L0122:
    /* 0122  mov     si,bp */
    SI = BP;
L0124:
    /* 0124  mov     dx,seg _cmpbuf1_start */
    DX = (uint16_t)(0x43A0 + PORT_LOAD_SEG);
L0127:
    /* 0127  mov     es,dx */
    SET_ES(DX);
L0129:
    /* 0129  mov     di,0 */
    DI = 0x0;
L012C:
    /* 012C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L012D:
    /* 012D  mov     dx,ax */
    DX = AX;
L012F:
    /* 012F  add     ax,7 */
    AX = (uint16_t)(AX + 0x7);
L0132:
    /* 0132  shr     ax,3 */
    AX = (uint16_t)(AX >> 3);
L0135:
    /* 0135  mov     cx,ax */
    CX = AX;
L0137: /* L0137 */
    /* 0137  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0138:
    /* 0138  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L013A:
    /* 013A  ror     ax,3 */
    AX = ror16(AX, 3);
L013D:
    /* 013D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L013E:
    /* 013E  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L013F:
    /* 013F  shr     ah,5 */
    AH = (uint8_t)(AH >> 5);
L0142:
    /* 0142  ror     ax,6 */
    AX = ror16(AX, 6);
L0145:
    /* 0145  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0146:
    /* 0146  shr     ax,3 */
    AX = (uint16_t)(AX >> 3);
L0149:
    /* 0149  xchg    ah,al */
    { uint8_t t_ = AL;
    AL = AH;
    AH = t_; }
L014B:
    /* 014B  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L014C:
    /* 014C  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L014D:
    /* 014D  shr     ah,7 */
    AH = (uint8_t)(AH >> 7);
L0150:
    /* 0150  ror     ax,4 */
    AX = ror16(AX, 4);
L0153:
    /* 0153  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0154:
    /* 0154  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0155:
    /* 0155  shr     ah,4 */
    AH = (uint8_t)(AH >> 4);
L0158:
    /* 0158  ror     ax,7 */
    AX = ror16(AX, 7);
L015B:
    /* 015B  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L015C:
    /* 015C  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L015E:
    /* 015E  rol     ax,5 */
    AX = rol16(AX, 5);
L0161:
    /* 0161  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0162:
    /* 0162  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0163:
    /* 0163  shr     ah,6 */
    AH = (uint8_t)(AH >> 6);
L0166:
    /* 0166  ror     ax,5 */
    AX = ror16(AX, 5);
L0169:
    /* 0169  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L016A:
    /* 016A  shr     ax,0Bh */
    AX = (uint16_t)(AX >> 11);
L016D:
    /* 016D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L016E:
    /* 016E  loop    L0137 */
    if (--CX) goto L0137;
L0170:
    /* 0170  mov     cx,dx */
    CX = DX;
L0172:
    /* 0172  mov     ax,seg _cmpbuf1_start */
    AX = (uint16_t)(0x43A0 + PORT_LOAD_SEG);
L0175:
    /* 0175  mov     ds,ax */
    SET_DS(AX);
L0177:
    /* 0177  mov     si,0 */
    SI = 0x0;
L017A:
    /* 017A  mov     ax,seg _cmpbuf1_start */
    AX = (uint16_t)(0x43A0 + PORT_LOAD_SEG);
L017D:
    /* 017D  mov     es,ax */
    SET_ES(AX);
L017F:
    /* 017F  mov     di,5400h */
    DI = 0x5400;
L0182:
    /* 0182  call    _seg004_0849_18D */
    if ((c = asm_call(ASM_JMP(0x065C, 0x018D), 0x0185)) != 0) return c;
L0185:
    /* 0185  mov     ax,es */
    AX = asm_es;
L0187:
    /* 0187  add     ax,540h */
    AX = add16(AX, 0x540, 0);
L018A:
    /* 018A  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_18B  (+18B)
       exp_8run: format 2 has no decoder; it returns at once, leaving AX as it was. Its second
       `ret` (L018C) is the end-of-input exit of the record decoder below. */
L018B: /* _exp_8run */
    /* 018B  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L018C: /* L018C */
    /* 018C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
