/* grlibg.c: replaces src/gfx/GRLIBG.ASM (seg003_3F06, 3F06..3FDB of its
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

uint32_t asm_mod_GRLIBG(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x3F06: goto L3F06;
    case 0x3F07: goto L3F07;
    case 0x3F08: goto L3F08;
    case 0x3F09: goto L3F09;
    case 0x3F0A: goto L3F0A;
    case 0x3F0B: goto L3F0B;
    case 0x3F0C: goto L3F0C;
    case 0x3F0D: goto L3F0D;
    case 0x3F0E: goto L3F0E;
    case 0x3F10: goto L3F10;
    case 0x3F12: goto L3F12;
    case 0x3F14: goto L3F14;
    case 0x3F17: goto L3F17;
    case 0x3F1B: goto L3F1B;
    case 0x3F1D: goto L3F1D;
    case 0x3F1F: goto L3F1F;
    case 0x3F21: goto L3F21;
    case 0x3F24: goto L3F24;
    case 0x3F28: goto L3F28;
    case 0x3F2A: goto L3F2A;
    case 0x3F2C: goto L3F2C;
    case 0x3F2E: goto L3F2E;
    case 0x3F30: goto L3F30;
    case 0x3F34: goto L3F34;
    case 0x3F36: goto L3F36;
    case 0x3F39: goto L3F39;
    case 0x3F3D: goto L3F3D;
    case 0x3F3F: goto L3F3F;
    case 0x3F42: goto L3F42;
    case 0x3F46: goto L3F46;
    case 0x3F48: goto L3F48;
    case 0x3F4B: goto L3F4B;
    case 0x3F4F: goto L3F4F;
    case 0x3F51: goto L3F51;
    case 0x3F54: goto L3F54;
    case 0x3F56: goto L3F56;
    case 0x3F5A: goto L3F5A;
    case 0x3F5C: goto L3F5C;
    case 0x3F5E: goto L3F5E;
    case 0x3F62: goto L3F62;
    case 0x3F64: goto L3F64;
    case 0x3F66: goto L3F66;
    case 0x3F6A: goto L3F6A;
    case 0x3F6C: goto L3F6C;
    case 0x3F6E: goto L3F6E;
    case 0x3F72: goto L3F72;
    case 0x3F74: goto L3F74;
    case 0x3F76: goto L3F76;
    case 0x3F78: goto L3F78;
    case 0x3F7A: goto L3F7A;
    case 0x3F7C: goto L3F7C;
    case 0x3F7E: goto L3F7E;
    case 0x3F80: goto L3F80;
    case 0x3F82: goto L3F82;
    case 0x3F85: goto L3F85;
    case 0x3F86: goto L3F86;
    case 0x3F88: goto L3F88;
    case 0x3F8A: goto L3F8A;
    case 0x3F8C: goto L3F8C;
    case 0x3F8E: goto L3F8E;
    case 0x3F90: goto L3F90;
    case 0x3F92: goto L3F92;
    case 0x3F94: goto L3F94;
    case 0x3F98: goto L3F98;
    case 0x3F9A: goto L3F9A;
    case 0x3F9E: goto L3F9E;
    case 0x3FA1: goto L3FA1;
    case 0x3FA3: goto L3FA3;
    case 0x3FA6: goto L3FA6;
    case 0x3FA8: goto L3FA8;
    case 0x3FAA: goto L3FAA;
    case 0x3FAE: goto L3FAE;
    case 0x3FB2: goto L3FB2;
    case 0x3FB4: goto L3FB4;
    case 0x3FB6: goto L3FB6;
    case 0x3FBA: goto L3FBA;
    case 0x3FBC: goto L3FBC;
    case 0x3FBF: goto L3FBF;
    case 0x3FC1: goto L3FC1;
    case 0x3FC4: goto L3FC4;
    case 0x3FC6: goto L3FC6;
    case 0x3FC8: goto L3FC8;
    case 0x3FCC: goto L3FCC;
    case 0x3FD0: goto L3FD0;
    case 0x3FD2: goto L3FD2;
    case 0x3FD4: goto L3FD4;
    case 0x3FD8: goto L3FD8;
    default: asm_bad_entry("GRLIBG.ASM", entry);
    }
L3F06: /* _seg003_3F06 */
    /* 3F06  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F07:
    /* 3F07  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L3F08:
    /* 3F08  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F09:
    /* 3F09  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3F0A:
    /* 3F0A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F0B:
    /* 3F0B  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3F0C:
    /* 3F0C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F0D:
    /* 3F0D  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }

    /* seg003_3F0E  (+3F0E)
       cline. The out codes are 2 left, 4 right, 8 below, 10h above. Both ends inside: jump to uline.
       Both outside on the same side: return. Otherwise move the outside end onto the edge its lowest
       set bit names (the slope from dx and dy by imul/idiv) and test again. */
L3F0E: /* _seg003_3F0E */
    /* 3F0E  mov     si,cx */
    SI = CX;
L3F10:
    /* 3F10  sub     si,ax */
    SI = sub16(SI, AX, 0);
L3F12:
    /* 3F12  jne     L3F17 */
    if (!ZF) goto L3F17;
L3F14:
    /* 3F14  jmp     _seg003_3BA1 */
    return ASM_JMP(0x0090, 0x3BA1);
L3F17: /* L3F17 */
    /* 3F17  mov     word ptr ds:[44A0h],si */
    ww(pDS, 0x44A0, SI);
L3F1B:
    /* 3F1B  mov     si,dx */
    SI = DX;
L3F1D:
    /* 3F1D  sub     si,bx */
    SI = sub16(SI, BX, 0);
L3F1F:
    /* 3F1F  jne     L3F24 */
    if (!ZF) goto L3F24;
L3F21:
    /* 3F21  jmp     _seg003_3CFF */
    return ASM_JMP(0x0090, 0x3CFF);
L3F24: /* L3F24 */
    /* 3F24  mov     word ptr ds:[44A2h],si */
    ww(pDS, 0x44A2, SI);
L3F28:
    /* 3F28  mov     di,ax */
    DI = AX;
L3F2A:
    /* 3F2A  mov     si,bx */
    SI = BX;
L3F2C:
    /* 3F2C  mov     bp,dx */
    BP = DX;
L3F2E:
    /* 3F2E  mov     bh,0 */
    BH = 0x0;
L3F30:
    /* 3F30  cmp     cx,word ptr ds:[WIN_LEFT] */
    sub16(CX, rw(pDS, 0x3DF2), 0);
L3F34:
    /* 3F34  jge     L3F39 */
    if (SF == OF) goto L3F39;
L3F36:
    /* 3F36  or      bh,2 */
    BH = (uint8_t)(BH | 0x2);
L3F39: /* L3F39 */
    /* 3F39  cmp     cx,word ptr ds:[WIN_RIGHT] */
    sub16(CX, rw(pDS, 0x3DF6), 0);
L3F3D:
    /* 3F3D  jle     L3F42 */
    if (ZF || SF != OF) goto L3F42;
L3F3F:
    /* 3F3F  or      bh,4 */
    BH = (uint8_t)(BH | 0x4);
L3F42: /* L3F42 */
    /* 3F42  cmp     bp,word ptr ds:[WIN_BOTTOM] */
    sub16(BP, rw(pDS, 0x3DF8), 0);
L3F46:
    /* 3F46  jge     L3F4B */
    if (SF == OF) goto L3F4B;
L3F48:
    /* 3F48  or      bh,8 */
    BH = (uint8_t)(BH | 0x8);
L3F4B: /* L3F4B */
    /* 3F4B  cmp     bp,word ptr ds:[WIN_TOP] */
    sub16(BP, rw(pDS, 0x3DF4), 0);
L3F4F:
    /* 3F4F  jle     L3F54 */
    if (ZF || SF != OF) goto L3F54;
L3F51:
    /* 3F51  or      bh,10h */
    BH = (uint8_t)(BH | 0x10);
L3F54: /* L3F54 */
    /* 3F54  mov     al,0 */
    AL = 0x0;
L3F56:
    /* 3F56  cmp     di,word ptr ds:[WIN_LEFT] */
    sub16(DI, rw(pDS, 0x3DF2), 0);
L3F5A:
    /* 3F5A  jge     L3F5E */
    if (SF == OF) goto L3F5E;
L3F5C:
    /* 3F5C  or      al,2 */
    AL = (uint8_t)(AL | 0x2);
L3F5E: /* L3F5E */
    /* 3F5E  cmp     di,word ptr ds:[WIN_RIGHT] */
    sub16(DI, rw(pDS, 0x3DF6), 0);
L3F62:
    /* 3F62  jle     L3F66 */
    if (ZF || SF != OF) goto L3F66;
L3F64:
    /* 3F64  or      al,4 */
    AL = (uint8_t)(AL | 0x4);
L3F66: /* L3F66 */
    /* 3F66  cmp     si,word ptr ds:[WIN_BOTTOM] */
    sub16(SI, rw(pDS, 0x3DF8), 0);
L3F6A:
    /* 3F6A  jge     L3F6E */
    if (SF == OF) goto L3F6E;
L3F6C:
    /* 3F6C  or      al,8 */
    AL = (uint8_t)(AL | 0x8);
L3F6E: /* L3F6E */
    /* 3F6E  cmp     si,word ptr ds:[WIN_TOP] */
    sub16(SI, rw(pDS, 0x3DF4), 0);
L3F72:
    /* 3F72  jle     L3F76 */
    if (ZF || SF != OF) goto L3F76;
L3F74:
    /* 3F74  or      al,10h */
    AL = (uint8_t)(AL | 0x10);
L3F76: /* L3F76 */
    /* 3F76  mov     bl,al */
    BL = AL;
L3F78:
    /* 3F78  or      al,bh */
    AL = logic8((uint8_t)(AL | BH));
L3F7A:
    /* 3F7A  jne     L3F86 */
    if (!ZF) goto L3F86;
L3F7C:
    /* 3F7C  mov     ax,di */
    AX = DI;
L3F7E:
    /* 3F7E  mov     bx,si */
    BX = SI;
L3F80:
    /* 3F80  mov     dx,bp */
    DX = BP;
L3F82:
    /* 3F82  jmp     _seg003_3FDC */
    return ASM_JMP(0x0090, 0x3FDC);
L3F85: /* L3F85 */
    /* 3F85  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3F86: /* L3F86 */
    /* 3F86  test    bl,bh */
    logic8((uint8_t)(BH & BL));
L3F88:
    /* 3F88  jne     L3F85 */
    if (!ZF) goto L3F85;
L3F8A:
    /* 3F8A  test    bl,bl */
    logic8((uint8_t)(BL & BL));
L3F8C:
    /* 3F8C  jne     L3F94 */
    if (!ZF) goto L3F94;
L3F8E:
    /* 3F8E  xchg    di,cx */
    { uint16_t t_ = CX;
    CX = DI;
    DI = t_; }
L3F90:
    /* 3F90  xchg    si,bp */
    { uint16_t t_ = BP;
    BP = SI;
    SI = t_; }
L3F92:
    /* 3F92  xchg    bl,bh */
    { uint8_t t_ = BH;
    BH = BL;
    BL = t_; }
L3F94: /* L3F94 */
    /* 3F94  mov     byte ptr ds:[44C4h],bh */
    wb(pDS, 0x44C4, BH);
L3F98:
    /* 3F98  mov     bh,0 */
    BH = 0x0;
L3F9A:
    /* 3F9A  jmp     word ptr [bx+44A4h] */
    return ASM_JMP(0x0090, rw(pDS, BX + 0x44A4));
L3F9E:
    /* 3F9E  mov     ax,word ptr ds:[WIN_LEFT] */
    AX = rw(pDS, 0x3DF2);
L3FA1:
    /* 3FA1  jmp     short L3FA6 */
    goto L3FA6;
L3FA3:
    /* 3FA3  mov     ax,word ptr ds:[WIN_RIGHT] */
    AX = rw(pDS, 0x3DF6);
L3FA6: /* L3FA6 */
    /* 3FA6  mov     bx,ax */
    BX = AX;
L3FA8:
    /* 3FA8  sub     ax,di */
    AX = (uint16_t)(AX - DI);
L3FAA:
    /* 3FAA  imul    word ptr ds:[44A2h] */
    imul16(rw(pDS, 0x44A2));
L3FAE:
    /* 3FAE  idiv    word ptr ds:[44A0h] */
    if (asm_idiv16(rw(pDS, 0x44A0)) && (c = asm_divfault(0x0090, 0x3FAE, 4)) != 0) return c;
L3FB2:
    /* 3FB2  mov     di,bx */
    DI = BX;
L3FB4:
    /* 3FB4  add     si,ax */
    SI = (uint16_t)(SI + AX);
L3FB6:
    /* 3FB6  mov     bh,byte ptr ds:[44C4h] */
    BH = rb(pDS, 0x44C4);
L3FBA:
    /* 3FBA  jmp     L3F54 */
    goto L3F54;
L3FBC:
    /* 3FBC  mov     ax,word ptr ds:[WIN_BOTTOM] */
    AX = rw(pDS, 0x3DF8);
L3FBF:
    /* 3FBF  jmp     short L3FC4 */
    goto L3FC4;
L3FC1:
    /* 3FC1  mov     ax,word ptr ds:[WIN_TOP] */
    AX = rw(pDS, 0x3DF4);
L3FC4: /* L3FC4 */
    /* 3FC4  mov     bx,ax */
    BX = AX;
L3FC6:
    /* 3FC6  sub     ax,si */
    AX = (uint16_t)(AX - SI);
L3FC8:
    /* 3FC8  imul    word ptr ds:[44A0h] */
    imul16(rw(pDS, 0x44A0));
L3FCC:
    /* 3FCC  idiv    word ptr ds:[44A2h] */
    if (asm_idiv16(rw(pDS, 0x44A2)) && (c = asm_divfault(0x0090, 0x3FCC, 4)) != 0) return c;
L3FD0:
    /* 3FD0  add     di,ax */
    DI = (uint16_t)(DI + AX);
L3FD2:
    /* 3FD2  mov     si,bx */
    SI = BX;
L3FD4:
    /* 3FD4  mov     bh,byte ptr ds:[44C4h] */
    BH = rb(pDS, 0x44C4);
L3FD8:
    /* 3FD8  jmp     L3F54 */
    goto L3F54;
}
