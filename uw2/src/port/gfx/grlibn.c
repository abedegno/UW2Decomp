/* grlibn.c: replaces src/gfx/GRLIBN.ASM (seg003_0272_5B50, 5B50..5D6D of its
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

uint32_t asm_mod_GRLIBN(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x5B50: goto L5B50;
    case 0x5B53: goto L5B53;
    case 0x5B56: goto L5B56;
    case 0x5B58: goto L5B58;
    case 0x5B5A: goto L5B5A;
    case 0x5B5C: goto L5B5C;
    case 0x5B5E: goto L5B5E;
    case 0x5B60: goto L5B60;
    case 0x5B61: goto L5B61;
    case 0x5B62: goto L5B62;
    case 0x5B63: goto L5B63;
    case 0x5B66: goto L5B66;
    case 0x5B6A: goto L5B6A;
    case 0x5B6E: goto L5B6E;
    case 0x5B71: goto L5B71;
    case 0x5B72: goto L5B72;
    case 0x5B73: goto L5B73;
    case 0x5B76: goto L5B76;
    case 0x5B78: goto L5B78;
    case 0x5B7A: goto L5B7A;
    case 0x5B7C: goto L5B7C;
    case 0x5B7E: goto L5B7E;
    case 0x5B80: goto L5B80;
    case 0x5B82: goto L5B82;
    case 0x5B83: goto L5B83;
    case 0x5B85: goto L5B85;
    case 0x5B8B: goto L5B8B;
    case 0x5B91: goto L5B91;
    case 0x5B97: goto L5B97;
    case 0x5B9A: goto L5B9A;
    case 0x5B9D: goto L5B9D;
    case 0x5B9F: goto L5B9F;
    case 0x5BA3: goto L5BA3;
    case 0x5BA6: goto L5BA6;
    case 0x5BAC: goto L5BAC;
    case 0x5BAF: goto L5BAF;
    case 0x5BB2: goto L5BB2;
    case 0x5BB4: goto L5BB4;
    case 0x5BB8: goto L5BB8;
    case 0x5BBB: goto L5BBB;
    case 0x5BBF: goto L5BBF;
    case 0x5BC3: goto L5BC3;
    case 0x5BC6: goto L5BC6;
    case 0x5BC7: goto L5BC7;
    case 0x5BC8: goto L5BC8;
    case 0x5BCB: goto L5BCB;
    case 0x5BCD: goto L5BCD;
    case 0x5BCF: goto L5BCF;
    case 0x5BD1: goto L5BD1;
    case 0x5BD3: goto L5BD3;
    case 0x5BD5: goto L5BD5;
    case 0x5BD7: goto L5BD7;
    case 0x5BD8: goto L5BD8;
    case 0x5BDA: goto L5BDA;
    case 0x5BE0: goto L5BE0;
    case 0x5BE6: goto L5BE6;
    case 0x5BEC: goto L5BEC;
    case 0x5BEF: goto L5BEF;
    case 0x5BF2: goto L5BF2;
    case 0x5BF4: goto L5BF4;
    case 0x5BF8: goto L5BF8;
    case 0x5BFB: goto L5BFB;
    case 0x5C01: goto L5C01;
    case 0x5C04: goto L5C04;
    case 0x5C07: goto L5C07;
    case 0x5C09: goto L5C09;
    case 0x5C0D: goto L5C0D;
    case 0x5C10: goto L5C10;
    case 0x5C12: goto L5C12;
    case 0x5C13: goto L5C13;
    case 0x5C14: goto L5C14;
    case 0x5C15: goto L5C15;
    case 0x5C16: goto L5C16;
    case 0x5C18: goto L5C18;
    case 0x5C19: goto L5C19;
    case 0x5C1D: goto L5C1D;
    case 0x5C21: goto L5C21;
    case 0x5C25: goto L5C25;
    case 0x5C29: goto L5C29;
    case 0x5C2E: goto L5C2E;
    case 0x5C30: goto L5C30;
    case 0x5C34: goto L5C34;
    case 0x5C35: goto L5C35;
    case 0x5C37: goto L5C37;
    case 0x5C39: goto L5C39;
    case 0x5C3C: goto L5C3C;
    case 0x5C3F: goto L5C3F;
    case 0x5C40: goto L5C40;
    case 0x5C44: goto L5C44;
    case 0x5C46: goto L5C46;
    case 0x5C48: goto L5C48;
    case 0x5C4B: goto L5C4B;
    case 0x5C4F: goto L5C4F;
    case 0x5C53: goto L5C53;
    case 0x5C55: goto L5C55;
    case 0x5C59: goto L5C59;
    case 0x5C5A: goto L5C5A;
    case 0x5C5B: goto L5C5B;
    case 0x5C5C: goto L5C5C;
    case 0x5C5D: goto L5C5D;
    case 0x5C5E: goto L5C5E;
    case 0x5C5F: goto L5C5F;
    case 0x5C60: goto L5C60;
    case 0x5C61: goto L5C61;
    case 0x5C64: goto L5C64;
    case 0x5C68: goto L5C68;
    case 0x5C6E: goto L5C6E;
    case 0x5C72: goto L5C72;
    case 0x5C73: goto L5C73;
    case 0x5C74: goto L5C74;
    case 0x5C75: goto L5C75;
    case 0x5C78: goto L5C78;
    case 0x5C7C: goto L5C7C;
    case 0x5C82: goto L5C82;
    case 0x5C86: goto L5C86;
    case 0x5C8C: goto L5C8C;
    case 0x5C90: goto L5C90;
    case 0x5C94: goto L5C94;
    case 0x5C97: goto L5C97;
    case 0x5C98: goto L5C98;
    case 0x5C9C: goto L5C9C;
    case 0x5C9D: goto L5C9D;
    case 0x5C9F: goto L5C9F;
    case 0x5CA0: goto L5CA0;
    case 0x5CA1: goto L5CA1;
    case 0x5CA2: goto L5CA2;
    case 0x5CA3: goto L5CA3;
    case 0x5CA4: goto L5CA4;
    case 0x5CA7: goto L5CA7;
    case 0x5CA8: goto L5CA8;
    case 0x5CA9: goto L5CA9;
    case 0x5CAA: goto L5CAA;
    case 0x5CAD: goto L5CAD;
    case 0x5CAF: goto L5CAF;
    case 0x5CB1: goto L5CB1;
    case 0x5CB3: goto L5CB3;
    case 0x5CB5: goto L5CB5;
    case 0x5CB7: goto L5CB7;
    case 0x5CB8: goto L5CB8;
    case 0x5CB9: goto L5CB9;
    case 0x5CBB: goto L5CBB;
    case 0x5CBD: goto L5CBD;
    case 0x5CBF: goto L5CBF;
    case 0x5CC3: goto L5CC3;
    case 0x5CC5: goto L5CC5;
    case 0x5CC9: goto L5CC9;
    case 0x5CCB: goto L5CCB;
    case 0x5CCF: goto L5CCF;
    case 0x5CD0: goto L5CD0;
    case 0x5CD1: goto L5CD1;
    case 0x5CD2: goto L5CD2;
    case 0x5CD3: goto L5CD3;
    case 0x5CD6: goto L5CD6;
    case 0x5CD9: goto L5CD9;
    case 0x5CDB: goto L5CDB;
    case 0x5CDD: goto L5CDD;
    case 0x5CE0: goto L5CE0;
    case 0x5CE1: goto L5CE1;
    case 0x5CE2: goto L5CE2;
    case 0x5CE3: goto L5CE3;
    case 0x5CE4: goto L5CE4;
    case 0x5CE6: goto L5CE6;
    case 0x5CE8: goto L5CE8;
    case 0x5CEC: goto L5CEC;
    case 0x5CF0: goto L5CF0;
    case 0x5CF5: goto L5CF5;
    case 0x5CF6: goto L5CF6;
    case 0x5CF7: goto L5CF7;
    case 0x5CF8: goto L5CF8;
    case 0x5CF9: goto L5CF9;
    case 0x5CFC: goto L5CFC;
    case 0x5CFD: goto L5CFD;
    case 0x5CFE: goto L5CFE;
    case 0x5CFF: goto L5CFF;
    case 0x5D02: goto L5D02;
    case 0x5D03: goto L5D03;
    case 0x5D05: goto L5D05;
    case 0x5D07: goto L5D07;
    case 0x5D09: goto L5D09;
    case 0x5D0B: goto L5D0B;
    case 0x5D0D: goto L5D0D;
    case 0x5D0F: goto L5D0F;
    case 0x5D10: goto L5D10;
    case 0x5D11: goto L5D11;
    case 0x5D13: goto L5D13;
    case 0x5D15: goto L5D15;
    case 0x5D17: goto L5D17;
    case 0x5D1B: goto L5D1B;
    case 0x5D1D: goto L5D1D;
    case 0x5D21: goto L5D21;
    case 0x5D23: goto L5D23;
    case 0x5D27: goto L5D27;
    case 0x5D28: goto L5D28;
    case 0x5D2C: goto L5D2C;
    case 0x5D2D: goto L5D2D;
    case 0x5D2E: goto L5D2E;
    case 0x5D2F: goto L5D2F;
    case 0x5D30: goto L5D30;
    case 0x5D33: goto L5D33;
    case 0x5D36: goto L5D36;
    case 0x5D38: goto L5D38;
    case 0x5D3A: goto L5D3A;
    case 0x5D3D: goto L5D3D;
    case 0x5D3E: goto L5D3E;
    case 0x5D3F: goto L5D3F;
    case 0x5D40: goto L5D40;
    case 0x5D41: goto L5D41;
    case 0x5D43: goto L5D43;
    case 0x5D45: goto L5D45;
    case 0x5D47: goto L5D47;
    case 0x5D4A: goto L5D4A;
    case 0x5D4D: goto L5D4D;
    case 0x5D50: goto L5D50;
    case 0x5D51: goto L5D51;
    case 0x5D52: goto L5D52;
    case 0x5D53: goto L5D53;
    case 0x5D55: goto L5D55;
    case 0x5D58: goto L5D58;
    case 0x5D59: goto L5D59;
    case 0x5D5B: goto L5D5B;
    case 0x5D5C: goto L5D5C;
    case 0x5D5D: goto L5D5D;
    case 0x5D5F: goto L5D5F;
    case 0x5D61: goto L5D61;
    case 0x5D63: goto L5D63;
    case 0x5D66: goto L5D66;
    case 0x5D69: goto L5D69;
    case 0x5D6B: goto L5D6B;
    case 0x5D6C: goto L5D6C;
    default: asm_bad_entry("GRLIBN.ASM", entry);
    }

    /* seg003_0272_5B50  (+5B50)
       s_shclip: attach intensities (_5D47), close the list, then clip top and bottom (skipped when
       every vertex is inside them) and left and right. L5C16 is one pass; L5C9C and L5CF0 work out
       an intersection with a horizontal or vertical edge (imul/idiv, both terms halved on overflow)
       and nudge it one pixel inward unless it lies on the edge. */
L5B50: /* _seg003_0272_5B50 */
    /* 5B50  call    _seg003_0272_5D47 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x5D47), 0x5B53)) != 0) return c;
L5B53:
    /* 5B53  mov     si,415Eh */
    SI = 0x415E;
L5B56:
    /* 5B56  mov     di,cx */
    DI = CX;
L5B58:
    /* 5B58  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L5B5A:
    /* 5B5A  add     di,cx */
    DI = (uint16_t)(DI + CX);
L5B5C:
    /* 5B5C  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L5B5E:
    /* 5B5E  add     di,si */
    DI = (uint16_t)(DI + SI);
L5B60:
    /* 5B60  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5B61:
    /* 5B61  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5B62:
    /* 5B62  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5B63:
    /* 5B63  sub     si,6 */
    SI = (uint16_t)(SI - 0x6);
L5B66:
    /* 5B66  mov     bx,word ptr ds:[3DF6h] */
    BX = rw(pDS, 0x3DF6);
L5B6A:
    /* 5B6A  mov     dx,word ptr ds:[3DFAh] */
    DX = rw(pDS, 0x3DFA);
L5B6E:
    /* 5B6E  mov     si,4160h */
    SI = 0x4160;
L5B71:
    /* 5B71  push    cx */
    push16(CX);
L5B72: /* L5B72 */
    /* 5B72  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5B73:
    /* 5B73  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L5B76:
    /* 5B76  cmp     ax,bx */
    sub16(AX, BX, 0);
L5B78:
    /* 5B78  jg      L5B80 */
    if (!ZF && SF == OF) goto L5B80;
L5B7A:
    /* 5B7A  cmp     ax,dx */
    sub16(AX, DX, 0);
L5B7C:
    /* 5B7C  jl      L5B80 */
    if (SF != OF) goto L5B80;
L5B7E:
    /* 5B7E  loop    L5B72 */
    if (--CX) goto L5B72;
L5B80: /* L5B80 */
    /* 5B80  test    cx,cx */
    logic16((uint16_t)(CX & CX));
L5B82:
    /* 5B82  pop     cx */
    CX = pop16();
L5B83:
    /* 5B83  je      L5BBB */
    if (ZF) goto L5BBB;
L5B85:
    /* 5B85  mov     word ptr ds:[4482h],2 */
    ww(pDS, 0x4482, 0x2);
L5B8B:
    /* 5B8B  mov     word ptr ds:[4484h],0Ah */
    ww(pDS, 0x4484, 0xA);
L5B91:
    /* 5B91  mov     word ptr ds:[4486h],offset L5CF0 */
    ww(pDS, 0x4486, 0x5CF0);
L5B97:
    /* 5B97  mov     si,415Eh */
    SI = 0x415E;
L5B9A:
    /* 5B9A  mov     di,42EEh */
    DI = 0x42EE;
L5B9D:
    /* 5B9D  mov     al,7Ch */
    AL = 0x7C;
L5B9F:
    /* 5B9F  mov     dx,word ptr ds:[3DF6h] */
    DX = rw(pDS, 0x3DF6);
L5BA3:
    /* 5BA3  call    L5C16 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x5C16), 0x5BA6)) != 0) return c;
L5BA6:
    /* 5BA6  mov     word ptr ds:[4486h],offset L5CEC */
    ww(pDS, 0x4486, 0x5CEC);
L5BAC:
    /* 5BAC  mov     si,42EEh */
    SI = 0x42EE;
L5BAF:
    /* 5BAF  mov     di,415Eh */
    DI = 0x415E;
L5BB2:
    /* 5BB2  mov     al,7Fh */
    AL = 0x7F;
L5BB4:
    /* 5BB4  mov     dx,word ptr ds:[3DFAh] */
    DX = rw(pDS, 0x3DFA);
L5BB8:
    /* 5BB8  call    L5C16 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x5C16), 0x5BBB)) != 0) return c;
L5BBB: /* L5BBB */
    /* 5BBB  mov     bx,word ptr ds:[3DF8h] */
    BX = rw(pDS, 0x3DF8);
L5BBF:
    /* 5BBF  mov     dx,word ptr ds:[3DF4h] */
    DX = rw(pDS, 0x3DF4);
L5BC3:
    /* 5BC3  mov     si,415Eh */
    SI = 0x415E;
L5BC6:
    /* 5BC6  push    cx */
    push16(CX);
L5BC7: /* L5BC7 */
    /* 5BC7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5BC8:
    /* 5BC8  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L5BCB:
    /* 5BCB  cmp     ax,bx */
    sub16(AX, BX, 0);
L5BCD:
    /* 5BCD  jg      L5BD5 */
    if (!ZF && SF == OF) goto L5BD5;
L5BCF:
    /* 5BCF  cmp     ax,dx */
    sub16(AX, DX, 0);
L5BD1:
    /* 5BD1  jl      L5BD5 */
    if (SF != OF) goto L5BD5;
L5BD3:
    /* 5BD3  loop    L5BC7 */
    if (--CX) goto L5BC7;
L5BD5: /* L5BD5 */
    /* 5BD5  test    cx,cx */
    logic16((uint16_t)(CX & CX));
L5BD7:
    /* 5BD7  pop     cx */
    CX = pop16();
L5BD8:
    /* 5BD8  je      L5C10 */
    if (ZF) goto L5C10;
L5BDA:
    /* 5BDA  mov     word ptr ds:[4482h],0 */
    ww(pDS, 0x4482, 0x0);
L5BE0:
    /* 5BE0  mov     word ptr ds:[4484h],8 */
    ww(pDS, 0x4484, 0x8);
L5BE6:
    /* 5BE6  mov     word ptr ds:[4486h],offset L5C9C */
    ww(pDS, 0x4486, 0x5C9C);
L5BEC:
    /* 5BEC  mov     si,415Eh */
    SI = 0x415E;
L5BEF:
    /* 5BEF  mov     di,42EEh */
    DI = 0x42EE;
L5BF2:
    /* 5BF2  mov     al,7Fh */
    AL = 0x7F;
L5BF4:
    /* 5BF4  mov     dx,word ptr ds:[3DF4h] */
    DX = rw(pDS, 0x3DF4);
L5BF8:
    /* 5BF8  call    L5C16 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x5C16), 0x5BFB)) != 0) return c;
L5BFB:
    /* 5BFB  mov     word ptr ds:[4486h],offset L5C98 */
    ww(pDS, 0x4486, 0x5C98);
L5C01:
    /* 5C01  mov     si,42EEh */
    SI = 0x42EE;
L5C04:
    /* 5C04  mov     di,415Eh */
    DI = 0x415E;
L5C07:
    /* 5C07  mov     al,7Ch */
    AL = 0x7C;
L5C09:
    /* 5C09  mov     dx,word ptr ds:[3DF8h] */
    DX = rw(pDS, 0x3DF8);
L5C0D:
    /* 5C0D  call    L5C16 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x5C16), 0x5C10)) != 0) return c;
L5C10: /* L5C10 */
    /* 5C10  jcxz    L5C14 */
    if (!CX) goto L5C14;
L5C12:
    /* 5C12  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L5C13: /* L5C13 */
    /* 5C13  pop     ax */
    AX = pop16();
L5C14: /* L5C14 */
    /* 5C14  pop     ax */
    AX = pop16();
L5C15:
    /* 5C15  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L5C16: /* L5C16 */
    /* 5C16  jcxz    L5C13 */
    if (!CX) goto L5C13;
L5C18:
    /* 5C18  push    di */
    push16(DI);
L5C19:
    /* 5C19  mov     byte ptr cs:L5C37,al */
    wb(CODE003, 0x5C37, AL);
L5C1D:
    /* 5C1D  mov     byte ptr cs:L5C46,al */
    wb(CODE003, 0x5C46, AL);
L5C21:
    /* 5C21  mov     word ptr ds:_seg_370D_7D0,cx */
    ww(pDS, 0x7D0, CX);
L5C25:
    /* 5C25  mov     word ptr ds:[447Eh],cx */
    ww(pDS, 0x447E, CX);
L5C29:
    /* 5C29  db      0EAh */
    return ASM_JMP(0x0085, 0x5C2E);
L5C2E: /* L5C2E */
    /* 5C2E  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L5C30:
    /* 5C30  add     si,word ptr ds:[4482h] */
    SI = (uint16_t)(SI + rw(pDS, 0x4482));
L5C34:
    /* 5C34  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5C35:
    /* 5C35  cmp     ax,dx */
    sub16(AX, DX, 0);
L5C37: /* L5C37 */
    /* 5C37  jg      L5C3C */
    if (asm_jcc(CODE003[0x5C37])) goto L5C3C;
L5C39:
    /* 5C39  mov     bx,4 */
    BX = 0x4;
L5C3C: /* L5C3C */
    /* 5C3C  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L5C3F:
    /* 5C3F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5C40:
    /* 5C40  sub     si,word ptr ds:[4484h] */
    SI = (uint16_t)(SI - rw(pDS, 0x4484));
L5C44:
    /* 5C44  cmp     ax,dx */
    sub16(AX, DX, 0);
L5C46: /* L5C46 */
    /* 5C46  jg      L5C4B */
    if (asm_jcc(CODE003[0x5C46])) goto L5C4B;
L5C48:
    /* 5C48  or      bx,2 */
    BX = (uint16_t)(BX | 0x2);
L5C4B: /* L5C4B */
    /* 5C4B  call    word ptr [bx+5E76h] */
    if ((c = asm_call(ASM_JMP(0x0085, rw(pDS, BX + 0x5E76)), 0x5C4F)) != 0) return c;
L5C4F:
    /* 5C4F  dec     word ptr ds:_seg_370D_7D0 */
    ww(pDS, 0x7D0, dec16(rw(pDS, 0x7D0)));
L5C53:
    /* 5C53  jg      L5C2E */
    if (!ZF && SF == OF) goto L5C2E;
L5C55:
    /* 5C55  mov     cx,word ptr ds:[447Eh] */
    CX = rw(pDS, 0x447E);
L5C59:
    /* 5C59  pop     si */
    SI = pop16();
L5C5A:
    /* 5C5A  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5C5B:
    /* 5C5B  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5C5C:
    /* 5C5C  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5C5D:
    /* 5C5D  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L5C5E:
    /* 5C5E  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5C5F:
    /* 5C5F  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5C60:
    /* 5C60  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5C61:
    /* 5C61  sub     si,6 */
    SI = sub16(SI, 0x6, 0);
L5C64:
    /* 5C64  inc     word ptr ds:[447Eh] */
    ww(pDS, 0x447E, inc16(rw(pDS, 0x447E)));
L5C68:
    /* 5C68  mov     word ptr ds:[4488h],0 */
    ww(pDS, 0x4488, 0x0);
L5C6E:
    /* 5C6E  jmp     word ptr ds:[4486h] */
    return ASM_JMP(0x0085, rw(pDS, 0x4486));
L5C72:
    /* 5C72  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5C73:
    /* 5C73  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5C74:
    /* 5C74  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5C75:
    /* 5C75  sub     si,6 */
    SI = sub16(SI, 0x6, 0);
L5C78:
    /* 5C78  inc     word ptr ds:[447Eh] */
    ww(pDS, 0x447E, inc16(rw(pDS, 0x447E)));
L5C7C:
    /* 5C7C  mov     word ptr ds:[4488h],0 */
    ww(pDS, 0x4488, 0x0);
L5C82:
    /* 5C82  jmp     word ptr ds:[4486h] */
    return ASM_JMP(0x0085, rw(pDS, 0x4486));
L5C86:
    /* 5C86  mov     word ptr ds:[4488h],1 */
    ww(pDS, 0x4488, 0x1);
L5C8C:
    /* 5C8C  jmp     word ptr ds:[4486h] */
    return ASM_JMP(0x0085, rw(pDS, 0x4486));
L5C90:
    /* 5C90  dec     word ptr ds:[447Eh] */
    ww(pDS, 0x447E, (uint16_t)(rw(pDS, 0x447E) - 1));
L5C94:
    /* 5C94  add     si,6 */
    SI = add16(SI, 0x6, 0);
L5C97:
    /* 5C97  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L5C98: /* L5C98 */
    /* 5C98  neg     word ptr ds:[4488h] */
    ww(pDS, 0x4488, (uint16_t)-rw(pDS, 0x4488));
L5C9C: /* L5C9C */
    /* 5C9C  push    dx */
    push16(DX);
L5C9D:
    /* 5C9D  mov     ax,dx */
    AX = DX;
L5C9F:
    /* 5C9F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5CA0:
    /* 5CA0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5CA1:
    /* 5CA1  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L5CA2:
    /* 5CA2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5CA3:
    /* 5CA3  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L5CA4:
    /* 5CA4  add     si,2 */
    SI = (uint16_t)(SI + 0x2);
L5CA7:
    /* 5CA7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5CA8:
    /* 5CA8  xchg    bp,ax */
    { uint16_t t_ = BP;
    BP = AX;
    AX = t_; }
L5CA9:
    /* 5CA9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5CAA:
    /* 5CAA  sub     si,4 */
    SI = (uint16_t)(SI - 0x4);
L5CAD:
    /* 5CAD  sub     ax,bx */
    AX = (uint16_t)(AX - BX);
L5CAF:
    /* 5CAF  sub     dx,cx */
    DX = (uint16_t)(DX - CX);
L5CB1:
    /* 5CB1  neg     cx */
    CX = (uint16_t)-CX;
L5CB3:
    /* 5CB3  add     cx,bp */
    CX = add16(CX, BP, 0);
L5CB5:
    /* 5CB5  jo      L5CE4 */
    if (OF) goto L5CE4;
L5CB7: /* L5CB7 */
    /* 5CB7  push    dx */
    push16(DX);
L5CB8:
    /* 5CB8  push    cx */
    push16(CX);
L5CB9:
    /* 5CB9  imul    dx */
    imul16(DX);
L5CBB:
    /* 5CBB  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0085, 0x5CBB, 2)) != 0) return c;
L5CBD:
    /* 5CBD  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L5CBF:
    /* 5CBF  cmp     ax,word ptr ds:[3DF6h] */
    sub16(AX, rw(pDS, 0x3DF6), 0);
L5CC3:
    /* 5CC3  jge     L5CCF */
    if (SF == OF) goto L5CCF;
L5CC5:
    /* 5CC5  cmp     ax,word ptr ds:[3DFAh] */
    sub16(AX, rw(pDS, 0x3DFA), 0);
L5CC9:
    /* 5CC9  jle     L5CCF */
    if (ZF || SF != OF) goto L5CCF;
L5CCB:
    /* 5CCB  add     ax,word ptr ds:[4488h] */
    AX = (uint16_t)(AX + rw(pDS, 0x4488));
L5CCF: /* L5CCF */
    /* 5CCF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5CD0:
    /* 5CD0  pop     cx */
    CX = pop16();
L5CD1:
    /* 5CD1  pop     dx */
    DX = pop16();
L5CD2:
    /* 5CD2  push    ax */
    push16(AX);
L5CD3:
    /* 5CD3  mov     ax,word ptr [si+4] */
    AX = rw(pDS, SI + 0x4);
L5CD6:
    /* 5CD6  sub     ax,word ptr [si-2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFE));
L5CD9:
    /* 5CD9  imul    dx */
    imul16(DX);
L5CDB:
    /* 5CDB  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0085, 0x5CDB, 2)) != 0) return c;
L5CDD:
    /* 5CDD  add     ax,word ptr [si-2] */
    AX = add16(AX, rw(pDS, SI + 0xFFFE), 0);
L5CE0:
    /* 5CE0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5CE1:
    /* 5CE1  pop     ax */
    AX = pop16();
L5CE2:
    /* 5CE2  pop     dx */
    DX = pop16();
L5CE3:
    /* 5CE3  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L5CE4: /* L5CE4 */
    /* 5CE4  rcr     cx,1 */
    CX = rcr16(CX, 1);
L5CE6:
    /* 5CE6  sar     dx,1 */
    DX = (uint16_t)((int16_t)DX >> 1);
L5CE8:
    /* 5CE8  jmp     L5CB7 */
    goto L5CB7;
L5CEC: /* L5CEC */
    /* 5CEC  neg     word ptr ds:[4488h] */
    ww(pDS, 0x4488, (uint16_t)-rw(pDS, 0x4488));
L5CF0: /* L5CF0 */
    /* 5CF0  mov     word ptr cs:L5CEA,dx */
    ww(CODE003, 0x5CEA, DX);
L5CF5:
    /* 5CF5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5CF6:
    /* 5CF6  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L5CF7:
    /* 5CF7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5CF8:
    /* 5CF8  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L5CF9:
    /* 5CF9  add     si,2 */
    SI = (uint16_t)(SI + 0x2);
L5CFC:
    /* 5CFC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5CFD:
    /* 5CFD  xchg    bp,ax */
    { uint16_t t_ = BP;
    BP = AX;
    AX = t_; }
L5CFE:
    /* 5CFE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5CFF:
    /* 5CFF  sub     si,4 */
    SI = (uint16_t)(SI - 0x4);
L5D02:
    /* 5D02  xchg    bp,ax */
    { uint16_t t_ = BP;
    BP = AX;
    AX = t_; }
L5D03:
    /* 5D03  xchg    bx,cx */
    { uint16_t t_ = CX;
    CX = BX;
    BX = t_; }
L5D05:
    /* 5D05  sub     ax,bx */
    AX = (uint16_t)(AX - BX);
L5D07:
    /* 5D07  sub     dx,cx */
    DX = (uint16_t)(DX - CX);
L5D09:
    /* 5D09  neg     cx */
    CX = (uint16_t)-CX;
L5D0B:
    /* 5D0B  add     cx,bp */
    CX = add16(CX, BP, 0);
L5D0D:
    /* 5D0D  jo      L5D41 */
    if (OF) goto L5D41;
L5D0F: /* L5D0F */
    /* 5D0F  push    dx */
    push16(DX);
L5D10:
    /* 5D10  push    cx */
    push16(CX);
L5D11:
    /* 5D11  imul    dx */
    imul16(DX);
L5D13:
    /* 5D13  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0085, 0x5D13, 2)) != 0) return c;
L5D15:
    /* 5D15  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L5D17:
    /* 5D17  cmp     ax,word ptr ds:[3DF4h] */
    sub16(AX, rw(pDS, 0x3DF4), 0);
L5D1B:
    /* 5D1B  jle     L5D27 */
    if (ZF || SF != OF) goto L5D27;
L5D1D:
    /* 5D1D  cmp     ax,word ptr ds:[3DF8h] */
    sub16(AX, rw(pDS, 0x3DF8), 0);
L5D21:
    /* 5D21  jge     L5D27 */
    if (SF == OF) goto L5D27;
L5D23:
    /* 5D23  add     ax,word ptr ds:[4488h] */
    AX = (uint16_t)(AX + rw(pDS, 0x4488));
L5D27: /* L5D27 */
    /* 5D27  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5D28:
    /* 5D28  mov     ax,word ptr cs:L5CEA */
    AX = rw(CODE003, 0x5CEA);
L5D2C:
    /* 5D2C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5D2D:
    /* 5D2D  pop     cx */
    CX = pop16();
L5D2E:
    /* 5D2E  pop     dx */
    DX = pop16();
L5D2F:
    /* 5D2F  push    ax */
    push16(AX);
L5D30:
    /* 5D30  mov     ax,word ptr [si+4] */
    AX = rw(pDS, SI + 0x4);
L5D33:
    /* 5D33  sub     ax,word ptr [si-2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFE));
L5D36:
    /* 5D36  imul    dx */
    imul16(DX);
L5D38:
    /* 5D38  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0085, 0x5D38, 2)) != 0) return c;
L5D3A:
    /* 5D3A  add     ax,word ptr [si-2] */
    AX = add16(AX, rw(pDS, SI + 0xFFFE), 0);
L5D3D:
    /* 5D3D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5D3E:
    /* 5D3E  pop     ax */
    AX = pop16();
L5D3F:
    /* 5D3F  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L5D40:
    /* 5D40  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L5D41: /* L5D41 */
    /* 5D41  rcr     cx,1 */
    CX = rcr16(CX, 1);
L5D43:
    /* 5D43  sar     dx,1 */
    DX = (uint16_t)((int16_t)DX >> 1);
L5D45:
    /* 5D45  jmp     L5D0F */
    goto L5D0F;

    /* seg003_0272_5D47  (+5D47)
       _5D47 (probably FM Towns' pack_vertices, which comes next to s_shclip there): turn CX (x, y)
       vertices at 415E into (x, y, intensity) ones, taking the intensities in order from the word
       table at 55EE (assembled in 42EE and copied back). */
L5D47: /* _seg003_0272_5D47 */
    /* 5D47  mov     si,415Eh */
    SI = 0x415E;
L5D4A:
    /* 5D4A  mov     bx,55EEh */
    BX = 0x55EE;
L5D4D:
    /* 5D4D  mov     di,42EEh */
    DI = 0x42EE;
L5D50:
    /* 5D50  push    cx */
    push16(CX);
L5D51: /* L5D51 */
    /* 5D51  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5D52:
    /* 5D52  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5D53:
    /* 5D53  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L5D55:
    /* 5D55  add     bx,2 */
    BX = (uint16_t)(BX + 0x2);
L5D58:
    /* 5D58  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5D59:
    /* 5D59  loop    L5D51 */
    if (--CX) goto L5D51;
L5D5B:
    /* 5D5B  pop     cx */
    CX = pop16();
L5D5C:
    /* 5D5C  push    cx */
    push16(CX);
L5D5D:
    /* 5D5D  mov     ax,cx */
    AX = CX;
L5D5F:
    /* 5D5F  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5D61:
    /* 5D61  add     cx,ax */
    CX = add16(CX, AX, 0);
L5D63:
    /* 5D63  mov     si,42EEh */
    SI = 0x42EE;
L5D66:
    /* 5D66  mov     di,415Eh */
    DI = 0x415E;
L5D69:
    /* 5D69  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L5D6B:
    /* 5D6B  pop     cx */
    CX = pop16();
L5D6C:
    /* 5D6C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
