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
    case 0x0A30: goto L0A30;
    case 0x0A33: goto L0A33;
    case 0x0A34: goto L0A34;
    case 0x0A37: goto L0A37;
    case 0x0A38: goto L0A38;
    case 0x0A3A: goto L0A3A;
    case 0x0A3C: goto L0A3C;
    case 0x0A3E: goto L0A3E;
    case 0x0A40: goto L0A40;
    case 0x0A42: goto L0A42;
    case 0x0A46: goto L0A46;
    case 0x0A4A: goto L0A4A;
    case 0x0A4C: goto L0A4C;
    case 0x0A4E: goto L0A4E;
    case 0x0A50: goto L0A50;
    case 0x0A52: goto L0A52;
    case 0x0A54: goto L0A54;
    case 0x0A55: goto L0A55;
    case 0x0A59: goto L0A59;
    case 0x0A5D: goto L0A5D;
    case 0x0A5F: goto L0A5F;
    case 0x0A61: goto L0A61;
    case 0x0A63: goto L0A63;
    case 0x0A65: goto L0A65;
    case 0x0A67: goto L0A67;
    case 0x0A68: goto L0A68;
    case 0x0A69: goto L0A69;
    case 0x0A6B: goto L0A6B;
    case 0x0A6D: goto L0A6D;
    case 0x0A6F: goto L0A6F;
    case 0x0A73: goto L0A73;
    case 0x0A77: goto L0A77;
    case 0x0A78: goto L0A78;
    case 0x0A7A: goto L0A7A;
    case 0x0A7C: goto L0A7C;
    case 0x0A7F: goto L0A7F;
    case 0x0A81: goto L0A81;
    case 0x0A83: goto L0A83;
    case 0x0A86: goto L0A86;
    case 0x0A88: goto L0A88;
    case 0x0A8A: goto L0A8A;
    case 0x0A8C: goto L0A8C;
    case 0x0A8E: goto L0A8E;
    case 0x0A90: goto L0A90;
    case 0x0A92: goto L0A92;
    case 0x0A94: goto L0A94;
    case 0x0A96: goto L0A96;
    case 0x0A98: goto L0A98;
    case 0x0A9A: goto L0A9A;
    case 0x0A9C: goto L0A9C;
    case 0x0A9E: goto L0A9E;
    case 0x0AA0: goto L0AA0;
    case 0x0AA2: goto L0AA2;
    case 0x0AA4: goto L0AA4;
    case 0x0AA6: goto L0AA6;
    case 0x0AA8: goto L0AA8;
    case 0x0AAA: goto L0AAA;
    case 0x0AAC: goto L0AAC;
    case 0x0AAE: goto L0AAE;
    case 0x0AB0: goto L0AB0;
    case 0x0AB2: goto L0AB2;
    case 0x0AB4: goto L0AB4;
    case 0x0AB6: goto L0AB6;
    case 0x0AB8: goto L0AB8;
    case 0x0AB9: goto L0AB9;
    case 0x0ABB: goto L0ABB;
    case 0x0ABD: goto L0ABD;
    case 0x0AC0: goto L0AC0;
    case 0x0AC2: goto L0AC2;
    case 0x0AC4: goto L0AC4;
    case 0x0AC6: goto L0AC6;
    case 0x0AC8: goto L0AC8;
    case 0x0ACA: goto L0ACA;
    case 0x0ACC: goto L0ACC;
    case 0x0ACE: goto L0ACE;
    case 0x0AD0: goto L0AD0;
    case 0x0AD2: goto L0AD2;
    case 0x0AD4: goto L0AD4;
    case 0x0AD6: goto L0AD6;
    case 0x0AD8: goto L0AD8;
    case 0x0ADA: goto L0ADA;
    case 0x0ADC: goto L0ADC;
    case 0x0ADE: goto L0ADE;
    case 0x0AE0: goto L0AE0;
    case 0x0AE2: goto L0AE2;
    case 0x0AE4: goto L0AE4;
    case 0x0AE6: goto L0AE6;
    case 0x0AE8: goto L0AE8;
    case 0x0AEA: goto L0AEA;
    case 0x0AEC: goto L0AEC;
    case 0x0AEE: goto L0AEE;
    case 0x0AF0: goto L0AF0;
    case 0x0AF2: goto L0AF2;
    case 0x0AF3: goto L0AF3;
    case 0x0AF5: goto L0AF5;
    case 0x0AF7: goto L0AF7;
    case 0x0AFA: goto L0AFA;
    case 0x0AFC: goto L0AFC;
    case 0x0AFE: goto L0AFE;
    case 0x0B00: goto L0B00;
    case 0x0B02: goto L0B02;
    case 0x0B04: goto L0B04;
    case 0x0B06: goto L0B06;
    case 0x0B08: goto L0B08;
    case 0x0B0A: goto L0B0A;
    case 0x0B0C: goto L0B0C;
    case 0x0B0E: goto L0B0E;
    case 0x0B10: goto L0B10;
    case 0x0B12: goto L0B12;
    case 0x0B14: goto L0B14;
    case 0x0B16: goto L0B16;
    case 0x0B18: goto L0B18;
    case 0x0B1A: goto L0B1A;
    case 0x0B1C: goto L0B1C;
    case 0x0B1E: goto L0B1E;
    case 0x0B20: goto L0B20;
    case 0x0B22: goto L0B22;
    case 0x0B24: goto L0B24;
    case 0x0B26: goto L0B26;
    case 0x0B28: goto L0B28;
    case 0x0B2A: goto L0B2A;
    case 0x0B2C: goto L0B2C;
    case 0x0B2E: goto L0B2E;
    case 0x0B2F: goto L0B2F;
    case 0x0B31: goto L0B31;
    case 0x0B33: goto L0B33;
    case 0x0B36: goto L0B36;
    case 0x0B38: goto L0B38;
    case 0x0B3A: goto L0B3A;
    case 0x0B3C: goto L0B3C;
    case 0x0B3E: goto L0B3E;
    case 0x0B40: goto L0B40;
    case 0x0B42: goto L0B42;
    case 0x0B44: goto L0B44;
    case 0x0B46: goto L0B46;
    case 0x0B48: goto L0B48;
    case 0x0B4A: goto L0B4A;
    case 0x0B4C: goto L0B4C;
    case 0x0B4E: goto L0B4E;
    case 0x0B50: goto L0B50;
    case 0x0B52: goto L0B52;
    case 0x0B54: goto L0B54;
    case 0x0B56: goto L0B56;
    case 0x0B58: goto L0B58;
    case 0x0B5A: goto L0B5A;
    case 0x0B5C: goto L0B5C;
    case 0x0B5E: goto L0B5E;
    case 0x0B60: goto L0B60;
    case 0x0B61: goto L0B61;
    case 0x0B63: goto L0B63;
    case 0x0B64: goto L0B64;
    case 0x0B65: goto L0B65;
    case 0x0B67: goto L0B67;
    case 0x0B69: goto L0B69;
    case 0x0B6B: goto L0B6B;
    case 0x0B6D: goto L0B6D;
    case 0x0B6F: goto L0B6F;
    case 0x0B73: goto L0B73;
    case 0x0B75: goto L0B75;
    case 0x0B77: goto L0B77;
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

    /* SquareRoot_seg021_22FD_A30  (+A30)
       _A30: far entry to _A78 (square root). */
L0A30: /* _SquareRoot_seg021_22FD_A30 */
    /* 0A30  call    _SquareRoot_seg021_22FD_A78 */
    if ((c = asm_call(ASM_JMP(0x2110, 0x0A78), 0x0A33)) != 0) return c;
L0A33:
    /* 0A33  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* sincos  (+A34)
       sincos (FM Towns name): far entry to _A38. */
L0A34: /* _sincos */
    /* 0A34  call    _seg021_22FD_A38 */
    if ((c = asm_call(ASM_JMP(0x2110, 0x0A38), 0x0A37)) != 0) return c;
L0A37:
    /* 0A37  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* HeadingLookupCalc_seg021_22FD_A38  (+A38)
       _A38 (sincos): BX = an angle. Out: AX = its sine, BX = its cosine, each interpolated linearly
       between the table's two entries by the angle's low byte. Changes CX, DX, BP. */
L0A38: /* _seg021_22FD_A38 */
    /* 0A38  mov     cx,bx */
    CX = BX;
L0A3A:
    /* 0A3A  xor     ch,ch */
    CH = (uint8_t)(CH ^ CH);
L0A3C:
    /* 0A3C  mov     bl,bh */
    BL = BH;
L0A3E:
    /* 0A3E  xor     bh,bh */
    BH = (uint8_t)(BH ^ BH);
L0A40:
    /* 0A40  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L0A42:
    /* 0A42  mov     bp,word ptr [bx+5B0h] */
    BP = rw(pDS, BX + 0x5B0);
L0A46:
    /* 0A46  mov     ax,word ptr [bx+5B2h] */
    AX = rw(pDS, BX + 0x5B2);
L0A4A:
    /* 0A4A  sub     ax,bp */
    AX = (uint16_t)(AX - BP);
L0A4C:
    /* 0A4C  imul    cx */
    imul16(CX);
L0A4E:
    /* 0A4E  mov     al,ah */
    AL = AH;
L0A50:
    /* 0A50  mov     ah,dl */
    AH = DL;
L0A52:
    /* 0A52  add     ax,bp */
    AX = (uint16_t)(AX + BP);
L0A54:
    /* 0A54  push    ax */
    push16(AX);
L0A55:
    /* 0A55  mov     bp,word ptr [bx+630h] */
    BP = rw(pDS, BX + 0x630);
L0A59:
    /* 0A59  mov     ax,word ptr [bx+632h] */
    AX = rw(pDS, BX + 0x632);
L0A5D:
    /* 0A5D  sub     ax,bp */
    AX = (uint16_t)(AX - BP);
L0A5F:
    /* 0A5F  imul    cx */
    imul16(CX);
L0A61:
    /* 0A61  mov     bl,ah */
    BL = AH;
L0A63:
    /* 0A63  mov     bh,dl */
    BH = DL;
L0A65:
    /* 0A65  add     bx,bp */
    BX = add16(BX, BP, 0);
L0A67:
    /* 0A67  pop     ax */
    AX = pop16();
L0A68:
    /* 0A68  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* LookupHeadingTable_seg021_22FD_A69  (+A69)
       _A69 (fast_sincos, FM Towns): as _A38 without interpolation, the entry for the angle's high
       byte. */
L0A69: /* _seg021_22FD_A69 */
    /* 0A69  mov     bl,bh */
    BL = BH;
L0A6B:
    /* 0A6B  xor     bh,bh */
    BH = (uint8_t)(BH ^ BH);
L0A6D:
    /* 0A6D  shl     bx,1 */
    BX = shl16(BX, 1);
L0A6F:
    /* 0A6F  mov     ax,word ptr [bx+5B0h] */
    AX = rw(pDS, BX + 0x5B0);
L0A73:
    /* 0A73  mov     bx,word ptr [bx+630h] */
    BX = rw(pDS, BX + 0x630);
L0A77:
    /* 0A77  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* SquareRoot_seg021_22FD_A78  (+A78)
       _A78 (lsqrt, FM Towns): DI = the square root of the 32-bit value CX:BX, by five rounds of
       Newton's method from a starting guess chosen by the value's size (CH, CL, BH or BL the highest
       non-zero byte). Changes AX, CX, DX.

       The unlabelled code after its last ret is fast_asin (FM Towns), probably: CX = the arcsine of
       AX (1.15) from the table's entry for |AX|'s high byte, without interpolation, with AX's sign. */
L0A78: /* _SquareRoot_seg021_22FD_A78 */
    /* 0A78  or      ch,ch */
    CH = logic8((uint8_t)(CH | CH));
L0A7A:
    /* 0A7A  je      short L0AB9 */
    if (ZF) goto L0AB9;
L0A7C:
    /* 0A7C  mov     di,4000h */
    DI = 0x4000;
L0A7F:
    /* 0A7F  cmp     cx,di */
    sub16(CX, DI, 0);
L0A81:
    /* 0A81  jb      short L0A86 */
    if (CF) goto L0A86;
L0A83:
    /* 0A83  mov     di,0FFFFh */
    DI = 0xFFFF;
L0A86: /* L0A86 */
    /* 0A86  mov     dx,cx */
    DX = CX;
L0A88:
    /* 0A88  mov     ax,bx */
    AX = BX;
L0A8A:
    /* 0A8A  div     di */
    if (asm_div16(DI) && (c = asm_divfault(0x2110, 0x0A8A, 2)) != 0) return c;
L0A8C:
    /* 0A8C  add     di,ax */
    DI = add16(DI, AX, 0);
L0A8E:
    /* 0A8E  rcr     di,1 */
    DI = rcr16(DI, 1);
L0A90:
    /* 0A90  mov     dx,cx */
    DX = CX;
L0A92:
    /* 0A92  mov     ax,bx */
    AX = BX;
L0A94:
    /* 0A94  div     di */
    if (asm_div16(DI) && (c = asm_divfault(0x2110, 0x0A94, 2)) != 0) return c;
L0A96:
    /* 0A96  add     di,ax */
    DI = add16(DI, AX, 0);
L0A98:
    /* 0A98  rcr     di,1 */
    DI = rcr16(DI, 1);
L0A9A:
    /* 0A9A  mov     dx,cx */
    DX = CX;
L0A9C:
    /* 0A9C  mov     ax,bx */
    AX = BX;
L0A9E:
    /* 0A9E  div     di */
    if (asm_div16(DI) && (c = asm_divfault(0x2110, 0x0A9E, 2)) != 0) return c;
L0AA0:
    /* 0AA0  add     di,ax */
    DI = add16(DI, AX, 0);
L0AA2:
    /* 0AA2  rcr     di,1 */
    DI = rcr16(DI, 1);
L0AA4:
    /* 0AA4  mov     dx,cx */
    DX = CX;
L0AA6:
    /* 0AA6  mov     ax,bx */
    AX = BX;
L0AA8:
    /* 0AA8  div     di */
    if (asm_div16(DI) && (c = asm_divfault(0x2110, 0x0AA8, 2)) != 0) return c;
L0AAA:
    /* 0AAA  add     di,ax */
    DI = add16(DI, AX, 0);
L0AAC:
    /* 0AAC  rcr     di,1 */
    DI = rcr16(DI, 1);
L0AAE:
    /* 0AAE  mov     dx,cx */
    DX = CX;
L0AB0:
    /* 0AB0  mov     ax,bx */
    AX = BX;
L0AB2:
    /* 0AB2  div     di */
    if (asm_div16(DI) && (c = asm_divfault(0x2110, 0x0AB2, 2)) != 0) return c;
L0AB4:
    /* 0AB4  add     di,ax */
    DI = add16(DI, AX, 0);
L0AB6:
    /* 0AB6  rcr     di,1 */
    DI = rcr16(DI, 1);
L0AB8:
    /* 0AB8  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0AB9: /* L0AB9 */
    /* 0AB9  or      cl,cl */
    CL = logic8((uint8_t)(CL | CL));
L0ABB:
    /* 0ABB  je      short L0AF3 */
    if (ZF) goto L0AF3;
L0ABD:
    /* 0ABD  mov     di,400h */
    DI = 0x400;
L0AC0:
    /* 0AC0  mov     dx,cx */
    DX = CX;
L0AC2:
    /* 0AC2  mov     ax,bx */
    AX = BX;
L0AC4:
    /* 0AC4  div     di */
    if (asm_div16(DI) && (c = asm_divfault(0x2110, 0x0AC4, 2)) != 0) return c;
L0AC6:
    /* 0AC6  add     di,ax */
    DI = add16(DI, AX, 0);
L0AC8:
    /* 0AC8  rcr     di,1 */
    DI = rcr16(DI, 1);
L0ACA:
    /* 0ACA  mov     dx,cx */
    DX = CX;
L0ACC:
    /* 0ACC  mov     ax,bx */
    AX = BX;
L0ACE:
    /* 0ACE  div     di */
    if (asm_div16(DI) && (c = asm_divfault(0x2110, 0x0ACE, 2)) != 0) return c;
L0AD0:
    /* 0AD0  add     di,ax */
    DI = add16(DI, AX, 0);
L0AD2:
    /* 0AD2  rcr     di,1 */
    DI = rcr16(DI, 1);
L0AD4:
    /* 0AD4  mov     dx,cx */
    DX = CX;
L0AD6:
    /* 0AD6  mov     ax,bx */
    AX = BX;
L0AD8:
    /* 0AD8  div     di */
    if (asm_div16(DI) && (c = asm_divfault(0x2110, 0x0AD8, 2)) != 0) return c;
L0ADA:
    /* 0ADA  add     di,ax */
    DI = add16(DI, AX, 0);
L0ADC:
    /* 0ADC  rcr     di,1 */
    DI = rcr16(DI, 1);
L0ADE:
    /* 0ADE  mov     dx,cx */
    DX = CX;
L0AE0:
    /* 0AE0  mov     ax,bx */
    AX = BX;
L0AE2:
    /* 0AE2  div     di */
    if (asm_div16(DI) && (c = asm_divfault(0x2110, 0x0AE2, 2)) != 0) return c;
L0AE4:
    /* 0AE4  add     di,ax */
    DI = add16(DI, AX, 0);
L0AE6:
    /* 0AE6  rcr     di,1 */
    DI = rcr16(DI, 1);
L0AE8:
    /* 0AE8  mov     dx,cx */
    DX = CX;
L0AEA:
    /* 0AEA  mov     ax,bx */
    AX = BX;
L0AEC:
    /* 0AEC  div     di */
    if (asm_div16(DI) && (c = asm_divfault(0x2110, 0x0AEC, 2)) != 0) return c;
L0AEE:
    /* 0AEE  add     di,ax */
    DI = add16(DI, AX, 0);
L0AF0:
    /* 0AF0  rcr     di,1 */
    DI = rcr16(DI, 1);
L0AF2:
    /* 0AF2  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0AF3: /* L0AF3 */
    /* 0AF3  or      bh,bh */
    BH = logic8((uint8_t)(BH | BH));
L0AF5:
    /* 0AF5  je      short L0B2F */
    if (ZF) goto L0B2F;
L0AF7:
    /* 0AF7  mov     cx,40h */
    CX = 0x40;
L0AFA:
    /* 0AFA  mov     ax,bx */
    AX = BX;
L0AFC:
    /* 0AFC  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L0AFE:
    /* 0AFE  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x2110, 0x0AFE, 2)) != 0) return c;
L0B00:
    /* 0B00  add     cx,ax */
    CX = add16(CX, AX, 0);
L0B02:
    /* 0B02  rcr     cx,1 */
    CX = rcr16(CX, 1);
L0B04:
    /* 0B04  mov     ax,bx */
    AX = BX;
L0B06:
    /* 0B06  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L0B08:
    /* 0B08  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x2110, 0x0B08, 2)) != 0) return c;
L0B0A:
    /* 0B0A  add     cx,ax */
    CX = add16(CX, AX, 0);
L0B0C:
    /* 0B0C  rcr     cx,1 */
    CX = rcr16(CX, 1);
L0B0E:
    /* 0B0E  mov     ax,bx */
    AX = BX;
L0B10:
    /* 0B10  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L0B12:
    /* 0B12  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x2110, 0x0B12, 2)) != 0) return c;
L0B14:
    /* 0B14  add     cx,ax */
    CX = add16(CX, AX, 0);
L0B16:
    /* 0B16  rcr     cx,1 */
    CX = rcr16(CX, 1);
L0B18:
    /* 0B18  mov     ax,bx */
    AX = BX;
L0B1A:
    /* 0B1A  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L0B1C:
    /* 0B1C  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x2110, 0x0B1C, 2)) != 0) return c;
L0B1E:
    /* 0B1E  add     cx,ax */
    CX = add16(CX, AX, 0);
L0B20:
    /* 0B20  rcr     cx,1 */
    CX = rcr16(CX, 1);
L0B22:
    /* 0B22  mov     ax,bx */
    AX = BX;
L0B24:
    /* 0B24  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L0B26:
    /* 0B26  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x2110, 0x0B26, 2)) != 0) return c;
L0B28:
    /* 0B28  add     cx,ax */
    CX = add16(CX, AX, 0);
L0B2A:
    /* 0B2A  rcr     cx,1 */
    CX = rcr16(CX, 1);
L0B2C:
    /* 0B2C  mov     di,cx */
    DI = CX;
L0B2E:
    /* 0B2E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0B2F: /* L0B2F */
    /* 0B2F  or      bl,bl */
    BL = logic8((uint8_t)(BL | BL));
L0B31:
    /* 0B31  je      short L0B61 */
    if (ZF) goto L0B61;
L0B33:
    /* 0B33  mov     cx,4 */
    CX = 0x4;
L0B36:
    /* 0B36  mov     ax,bx */
    AX = BX;
L0B38:
    /* 0B38  div     cl */
    if (asm_div8(CL) && (c = asm_divfault(0x2110, 0x0B38, 2)) != 0) return c;
L0B3A:
    /* 0B3A  add     cl,al */
    CL = add8(CL, AL, 0);
L0B3C:
    /* 0B3C  rcr     cl,1 */
    CL = rcr8(CL, 1);
L0B3E:
    /* 0B3E  mov     ax,bx */
    AX = BX;
L0B40:
    /* 0B40  div     cl */
    if (asm_div8(CL) && (c = asm_divfault(0x2110, 0x0B40, 2)) != 0) return c;
L0B42:
    /* 0B42  add     cl,al */
    CL = add8(CL, AL, 0);
L0B44:
    /* 0B44  rcr     cl,1 */
    CL = rcr8(CL, 1);
L0B46:
    /* 0B46  mov     ax,bx */
    AX = BX;
L0B48:
    /* 0B48  div     cl */
    if (asm_div8(CL) && (c = asm_divfault(0x2110, 0x0B48, 2)) != 0) return c;
L0B4A:
    /* 0B4A  add     cl,al */
    CL = add8(CL, AL, 0);
L0B4C:
    /* 0B4C  rcr     cl,1 */
    CL = rcr8(CL, 1);
L0B4E:
    /* 0B4E  mov     ax,bx */
    AX = BX;
L0B50:
    /* 0B50  div     cl */
    if (asm_div8(CL) && (c = asm_divfault(0x2110, 0x0B50, 2)) != 0) return c;
L0B52:
    /* 0B52  add     cl,al */
    CL = add8(CL, AL, 0);
L0B54:
    /* 0B54  rcr     cl,1 */
    CL = rcr8(CL, 1);
L0B56:
    /* 0B56  mov     ax,bx */
    AX = BX;
L0B58:
    /* 0B58  div     cl */
    if (asm_div8(CL) && (c = asm_divfault(0x2110, 0x0B58, 2)) != 0) return c;
L0B5A:
    /* 0B5A  add     cl,al */
    CL = add8(CL, AL, 0);
L0B5C:
    /* 0B5C  rcr     cl,1 */
    CL = rcr8(CL, 1);
L0B5E:
    /* 0B5E  mov     di,cx */
    DI = CX;
L0B60:
    /* 0B60  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0B61: /* L0B61 */
    /* 0B61  xor     di,di */
    DI = logic16((uint16_t)(DI ^ DI));
L0B63:
    /* 0B63  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0B64:
    /* 0B64  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0B65:
    /* 0B65  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L0B67:
    /* 0B67  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L0B69:
    /* 0B69  mov     bl,ah */
    BL = AH;
L0B6B:
    /* 0B6B  xor     bh,bh */
    BH = (uint8_t)(BH ^ BH);
L0B6D:
    /* 0B6D  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L0B6F:
    /* 0B6F  mov     cx,word ptr [bx+832h] */
    CX = rw(pDS, BX + 0x832);
L0B73:
    /* 0B73  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L0B75:
    /* 0B75  sub     cx,dx */
    CX = sub16(CX, DX, 0);
L0B77:
    /* 0B77  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

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
