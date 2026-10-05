/* imath_x.c: replaces src/sys/IMATH.ASM (seg021_22FD_A30, 0A30..0BF9 of its
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

uint32_t asm_mod_IMATH(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x0B78: goto L0B78;
    case 0x0B79: goto L0B79;
    case 0x0B7B: goto L0B7B;
    case 0x0B7D: goto L0B7D;
    case 0x0B7E: goto L0B7E;
    case 0x0B80: goto L0B80;
    case 0x0B82: goto L0B82;
    case 0x0B84: goto L0B84;
    case 0x0B86: goto L0B86;
    case 0x0B88: goto L0B88;
    case 0x0B8C: goto L0B8C;
    case 0x0B90: goto L0B90;
    case 0x0B92: goto L0B92;
    case 0x0B94: goto L0B94;
    case 0x0B96: goto L0B96;
    case 0x0B98: goto L0B98;
    case 0x0B9A: goto L0B9A;
    case 0x0B9C: goto L0B9C;
    case 0x0B9D: goto L0B9D;
    case 0x0B9F: goto L0B9F;
    case 0x0BA1: goto L0BA1;
    case 0x0BA2: goto L0BA2;
    case 0x0BA4: goto L0BA4;
    case 0x0BA5: goto L0BA5;
    case 0x0BA7: goto L0BA7;
    case 0x0BA9: goto L0BA9;
    case 0x0BAA: goto L0BAA;
    case 0x0BAC: goto L0BAC;
    case 0x0BAE: goto L0BAE;
    case 0x0BB0: goto L0BB0;
    case 0x0BB2: goto L0BB2;
    case 0x0BB4: goto L0BB4;
    case 0x0BB8: goto L0BB8;
    case 0x0BBC: goto L0BBC;
    case 0x0BBE: goto L0BBE;
    case 0x0BC0: goto L0BC0;
    case 0x0BC2: goto L0BC2;
    case 0x0BC4: goto L0BC4;
    case 0x0BC6: goto L0BC6;
    case 0x0BC8: goto L0BC8;
    case 0x0BC9: goto L0BC9;
    case 0x0BCB: goto L0BCB;
    case 0x0BCD: goto L0BCD;
    case 0x0BD1: goto L0BD1;
    case 0x0BD3: goto L0BD3;
    case 0x0BD4: goto L0BD4;
    case 0x0BD7: goto L0BD7;
    case 0x0BD9: goto L0BD9;
    case 0x0BDC: goto L0BDC;
    case 0x0BDE: goto L0BDE;
    case 0x0BDF: goto L0BDF;
    case 0x0BE2: goto L0BE2;
    case 0x0BE3: goto L0BE3;
    case 0x0BE5: goto L0BE5;
    case 0x0BE7: goto L0BE7;
    case 0x0BEB: goto L0BEB;
    case 0x0BED: goto L0BED;
    case 0x0BEE: goto L0BEE;
    case 0x0BEF: goto L0BEF;
    case 0x0BF0: goto L0BF0;
    case 0x0BF3: goto L0BF3;
    case 0x0BF4: goto L0BF4;
    case 0x0BF6: goto L0BF6;
    case 0x0BF8: goto L0BF8;
    default: asm_bad_entry("IMATH.ASM", entry);
    }

    /* seg021_22FD_B78  (+B78)
       _B78: CX = the arcsine of AX (1.15), interpolated, with AX's sign. */
L0B78: /* _seg021_22FD_B78 */
    /* 0B78  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0B79:
    /* 0B79  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L0B7B:
    /* 0B7B  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L0B7D:
    /* 0B7D  push    dx */
    push16(DX);
L0B7E:
    /* 0B7E  mov     bl,ah */
    BL = AH;
L0B80:
    /* 0B80  xor     bh,bh */
    BH = (uint8_t)(BH ^ BH);
L0B82:
    /* 0B82  mov     cx,ax */
    CX = AX;
L0B84:
    /* 0B84  xor     ch,ch */
    CH = (uint8_t)(CH ^ CH);
L0B86:
    /* 0B86  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L0B88:
    /* 0B88  mov     bp,word ptr [bx+832h] */
    BP = rw(pDS, BX + 0x832);
L0B8C:
    /* 0B8C  mov     ax,word ptr [bx+834h] */
    AX = rw(pDS, BX + 0x834);
L0B90:
    /* 0B90  sub     ax,bp */
    AX = (uint16_t)(AX - BP);
L0B92:
    /* 0B92  imul    cx */
    imul16(CX);
L0B94:
    /* 0B94  mov     al,ah */
    AL = AH;
L0B96:
    /* 0B96  mov     ah,dl */
    AH = DL;
L0B98:
    /* 0B98  add     ax,bp */
    AX = (uint16_t)(AX + BP);
L0B9A:
    /* 0B9A  mov     cx,ax */
    CX = AX;
L0B9C:
    /* 0B9C  pop     dx */
    DX = pop16();
L0B9D:
    /* 0B9D  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L0B9F:
    /* 0B9F  sub     cx,dx */
    CX = sub16(CX, DX, 0);
L0BA1:
    /* 0BA1  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg021_22FD_BA2  (+BA2)
       _BA2: CX = the arccosine of BX (1.15), interpolated; for a negative BX the result is moved
       into the other half of the circle (by adding 8000h to the negated value). */
L0BA2: /* _seg021_22FD_BA2 */
    /* 0BA2  mov     ax,bx */
    AX = BX;
L0BA4:
    /* 0BA4  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0BA5:
    /* 0BA5  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L0BA7:
    /* 0BA7  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L0BA9:
    /* 0BA9  push    dx */
    push16(DX);
L0BAA:
    /* 0BAA  mov     cx,ax */
    CX = AX;
L0BAC:
    /* 0BAC  xor     ch,ch */
    CH = (uint8_t)(CH ^ CH);
L0BAE:
    /* 0BAE  mov     bl,ah */
    BL = AH;
L0BB0:
    /* 0BB0  xor     bh,bh */
    BH = (uint8_t)(BH ^ BH);
L0BB2:
    /* 0BB2  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L0BB4:
    /* 0BB4  mov     bp,word ptr [bx+934h] */
    BP = rw(pDS, BX + 0x934);
L0BB8:
    /* 0BB8  mov     ax,word ptr [bx+936h] */
    AX = rw(pDS, BX + 0x936);
L0BBC:
    /* 0BBC  sub     ax,bp */
    AX = (uint16_t)(AX - BP);
L0BBE:
    /* 0BBE  imul    cx */
    imul16(CX);
L0BC0:
    /* 0BC0  mov     al,ah */
    AL = AH;
L0BC2:
    /* 0BC2  mov     ah,dl */
    AH = DL;
L0BC4:
    /* 0BC4  add     ax,bp */
    AX = (uint16_t)(AX + BP);
L0BC6:
    /* 0BC6  mov     cx,ax */
    CX = AX;
L0BC8:
    /* 0BC8  pop     dx */
    DX = pop16();
L0BC9:
    /* 0BC9  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L0BCB:
    /* 0BCB  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L0BCD:
    /* 0BCD  and     dx,8000h */
    DX = (uint16_t)(DX & 0x8000);
L0BD1:
    /* 0BD1  add     cx,dx */
    CX = add16(CX, DX, 0);
L0BD3:
    /* 0BD3  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* MaybeTangent_seg021_22FD_BD4  (+BD4)
       _BD4 (atan2, FM Towns, through cAtan2): CX = the angle of the unit vector (sine AX, cosine
       BX). Where the sine is small (|AX| up to sin 45 degrees, 5A82h) it uses the arcsine of AX,
       reflected when BX is negative; otherwise the arccosine of BX with AX's sign. Both are well
       conditioned in their range. */
L0BD4: /* _MaybeTangent_seg021_22FD_BD4 */
    /* 0BD4  cmp     ax,5A82h */
    sub16(AX, 0x5A82, 0);
L0BD7:
    /* 0BD7  jg      short L0BEE */
    if (!ZF && SF == OF) goto L0BEE;
L0BD9:
    /* 0BD9  cmp     ax,0A57Eh */
    sub16(AX, 0xA57E, 0);
L0BDC:
    /* 0BDC  jl      short L0BEE */
    if (SF != OF) goto L0BEE;
L0BDE:
    /* 0BDE  push    bx */
    push16(BX);
L0BDF:
    /* 0BDF  call    _seg021_22FD_B78 */
    if ((c = asm_call(ASM_JMP(0x2110, 0x0B78), 0x0BE2)) != 0) return c;
L0BE2:
    /* 0BE2  pop     ax */
    AX = pop16();
L0BE3:
    /* 0BE3  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L0BE5:
    /* 0BE5  jns     short L0BED */
    if (!SF) goto L0BED;
L0BE7:
    /* 0BE7  sub     cx,8000h */
    CX = (uint16_t)(CX - 0x8000);
L0BEB:
    /* 0BEB  neg     cx */
    CX = neg16(CX);
L0BED: /* L0BED */
    /* 0BED  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0BEE: /* L0BEE */
    /* 0BEE  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0BEF:
    /* 0BEF  push    dx */
    push16(DX);
L0BF0:
    /* 0BF0  call    _seg021_22FD_BA2 */
    if ((c = asm_call(ASM_JMP(0x2110, 0x0BA2), 0x0BF3)) != 0) return c;
L0BF3:
    /* 0BF3  pop     dx */
    DX = pop16();
L0BF4:
    /* 0BF4  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L0BF6:
    /* 0BF6  sub     cx,dx */
    CX = sub16(CX, DX, 0);
L0BF8:
    /* 0BF8  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
