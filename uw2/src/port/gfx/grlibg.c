/* grlibg.c: replaces src/gfx/GRLIBG.ASM (seg003_0272_3686, 3686..375B of its
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
    case 0x3686: goto L3686;
    case 0x3687: goto L3687;
    case 0x3688: goto L3688;
    case 0x3689: goto L3689;
    case 0x368A: goto L368A;
    case 0x368B: goto L368B;
    case 0x368C: goto L368C;
    case 0x368D: goto L368D;
    case 0x368E: goto L368E;
    case 0x3690: goto L3690;
    case 0x3692: goto L3692;
    case 0x3694: goto L3694;
    case 0x3697: goto L3697;
    case 0x369B: goto L369B;
    case 0x369D: goto L369D;
    case 0x369F: goto L369F;
    case 0x36A1: goto L36A1;
    case 0x36A4: goto L36A4;
    case 0x36A8: goto L36A8;
    case 0x36AA: goto L36AA;
    case 0x36AC: goto L36AC;
    case 0x36AE: goto L36AE;
    case 0x36B0: goto L36B0;
    case 0x36B4: goto L36B4;
    case 0x36B6: goto L36B6;
    case 0x36B9: goto L36B9;
    case 0x36BD: goto L36BD;
    case 0x36BF: goto L36BF;
    case 0x36C2: goto L36C2;
    case 0x36C6: goto L36C6;
    case 0x36C8: goto L36C8;
    case 0x36CB: goto L36CB;
    case 0x36CF: goto L36CF;
    case 0x36D1: goto L36D1;
    case 0x36D4: goto L36D4;
    case 0x36D6: goto L36D6;
    case 0x36DA: goto L36DA;
    case 0x36DC: goto L36DC;
    case 0x36DE: goto L36DE;
    case 0x36E2: goto L36E2;
    case 0x36E4: goto L36E4;
    case 0x36E6: goto L36E6;
    case 0x36EA: goto L36EA;
    case 0x36EC: goto L36EC;
    case 0x36EE: goto L36EE;
    case 0x36F2: goto L36F2;
    case 0x36F4: goto L36F4;
    case 0x36F6: goto L36F6;
    case 0x36F8: goto L36F8;
    case 0x36FA: goto L36FA;
    case 0x36FC: goto L36FC;
    case 0x36FE: goto L36FE;
    case 0x3700: goto L3700;
    case 0x3702: goto L3702;
    case 0x3705: goto L3705;
    case 0x3706: goto L3706;
    case 0x3708: goto L3708;
    case 0x370A: goto L370A;
    case 0x370C: goto L370C;
    case 0x370E: goto L370E;
    case 0x3710: goto L3710;
    case 0x3712: goto L3712;
    case 0x3714: goto L3714;
    case 0x3718: goto L3718;
    case 0x371A: goto L371A;
    case 0x371E: goto L371E;
    case 0x3721: goto L3721;
    case 0x3723: goto L3723;
    case 0x3726: goto L3726;
    case 0x3728: goto L3728;
    case 0x372A: goto L372A;
    case 0x372E: goto L372E;
    case 0x3732: goto L3732;
    case 0x3734: goto L3734;
    case 0x3736: goto L3736;
    case 0x373A: goto L373A;
    case 0x373C: goto L373C;
    case 0x373F: goto L373F;
    case 0x3741: goto L3741;
    case 0x3744: goto L3744;
    case 0x3746: goto L3746;
    case 0x3748: goto L3748;
    case 0x374C: goto L374C;
    case 0x3750: goto L3750;
    case 0x3752: goto L3752;
    case 0x3754: goto L3754;
    case 0x3758: goto L3758;
    default: asm_bad_entry("GRLIBG.ASM", entry);
    }
L3686: /* _seg003_0272_3686 */
    /* 3686  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3687:
    /* 3687  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L3688:
    /* 3688  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3689:
    /* 3689  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L368A:
    /* 368A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L368B:
    /* 368B  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L368C:
    /* 368C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L368D:
    /* 368D  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }

    /* seg003_0272_368E  (+368E)
       cline. The out codes are 2 left, 4 right, 8 below, 10h above. Both ends inside: jump to uline.
       Both outside on the same side: return. Otherwise move the outside end onto the edge its lowest
       set bit names (the slope from dx and dy by imul/idiv) and test again. */
L368E: /* _seg003_0272_368E */
    /* 368E  mov     si,cx */
    SI = CX;
L3690:
    /* 3690  sub     si,ax */
    SI = sub16(SI, AX, 0);
L3692:
    /* 3692  jne     L3697 */
    if (!ZF) goto L3697;
L3694:
    /* 3694  jmp     _seg003_0272_3321 */
    return ASM_JMP(0x0085, 0x3321);
L3697: /* L3697 */
    /* 3697  mov     word ptr ds:[44A2h],si */
    ww(pDS, 0x44A2, SI);
L369B:
    /* 369B  mov     si,dx */
    SI = DX;
L369D:
    /* 369D  sub     si,bx */
    SI = sub16(SI, BX, 0);
L369F:
    /* 369F  jne     L36A4 */
    if (!ZF) goto L36A4;
L36A1:
    /* 36A1  jmp     _seg003_0272_347F */
    return ASM_JMP(0x0085, 0x347F);
L36A4: /* L36A4 */
    /* 36A4  mov     word ptr ds:[44A4h],si */
    ww(pDS, 0x44A4, SI);
L36A8:
    /* 36A8  mov     di,ax */
    DI = AX;
L36AA:
    /* 36AA  mov     si,bx */
    SI = BX;
L36AC:
    /* 36AC  mov     bp,dx */
    BP = DX;
L36AE:
    /* 36AE  mov     bh,0 */
    BH = 0x0;
L36B0:
    /* 36B0  cmp     cx,word ptr ds:[3DF4h] */
    sub16(CX, rw(pDS, 0x3DF4), 0);
L36B4:
    /* 36B4  jge     L36B9 */
    if (SF == OF) goto L36B9;
L36B6:
    /* 36B6  or      bh,2 */
    BH = (uint8_t)(BH | 0x2);
L36B9: /* L36B9 */
    /* 36B9  cmp     cx,word ptr ds:[3DF8h] */
    sub16(CX, rw(pDS, 0x3DF8), 0);
L36BD:
    /* 36BD  jle     L36C2 */
    if (ZF || SF != OF) goto L36C2;
L36BF:
    /* 36BF  or      bh,4 */
    BH = (uint8_t)(BH | 0x4);
L36C2: /* L36C2 */
    /* 36C2  cmp     bp,word ptr ds:[3DFAh] */
    sub16(BP, rw(pDS, 0x3DFA), 0);
L36C6:
    /* 36C6  jge     L36CB */
    if (SF == OF) goto L36CB;
L36C8:
    /* 36C8  or      bh,8 */
    BH = (uint8_t)(BH | 0x8);
L36CB: /* L36CB */
    /* 36CB  cmp     bp,word ptr ds:[3DF6h] */
    sub16(BP, rw(pDS, 0x3DF6), 0);
L36CF:
    /* 36CF  jle     L36D4 */
    if (ZF || SF != OF) goto L36D4;
L36D1:
    /* 36D1  or      bh,10h */
    BH = (uint8_t)(BH | 0x10);
L36D4: /* L36D4 */
    /* 36D4  mov     al,0 */
    AL = 0x0;
L36D6:
    /* 36D6  cmp     di,word ptr ds:[3DF4h] */
    sub16(DI, rw(pDS, 0x3DF4), 0);
L36DA:
    /* 36DA  jge     L36DE */
    if (SF == OF) goto L36DE;
L36DC:
    /* 36DC  or      al,2 */
    AL = (uint8_t)(AL | 0x2);
L36DE: /* L36DE */
    /* 36DE  cmp     di,word ptr ds:[3DF8h] */
    sub16(DI, rw(pDS, 0x3DF8), 0);
L36E2:
    /* 36E2  jle     L36E6 */
    if (ZF || SF != OF) goto L36E6;
L36E4:
    /* 36E4  or      al,4 */
    AL = (uint8_t)(AL | 0x4);
L36E6: /* L36E6 */
    /* 36E6  cmp     si,word ptr ds:[3DFAh] */
    sub16(SI, rw(pDS, 0x3DFA), 0);
L36EA:
    /* 36EA  jge     L36EE */
    if (SF == OF) goto L36EE;
L36EC:
    /* 36EC  or      al,8 */
    AL = (uint8_t)(AL | 0x8);
L36EE: /* L36EE */
    /* 36EE  cmp     si,word ptr ds:[3DF6h] */
    sub16(SI, rw(pDS, 0x3DF6), 0);
L36F2:
    /* 36F2  jle     L36F6 */
    if (ZF || SF != OF) goto L36F6;
L36F4:
    /* 36F4  or      al,10h */
    AL = (uint8_t)(AL | 0x10);
L36F6: /* L36F6 */
    /* 36F6  mov     bl,al */
    BL = AL;
L36F8:
    /* 36F8  or      al,bh */
    AL = logic8((uint8_t)(AL | BH));
L36FA:
    /* 36FA  jne     L3706 */
    if (!ZF) goto L3706;
L36FC:
    /* 36FC  mov     ax,di */
    AX = DI;
L36FE:
    /* 36FE  mov     bx,si */
    BX = SI;
L3700:
    /* 3700  mov     dx,bp */
    DX = BP;
L3702:
    /* 3702  jmp     _seg003_0272_375C */
    return ASM_JMP(0x0085, 0x375C);
L3705: /* L3705 */
    /* 3705  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3706: /* L3706 */
    /* 3706  test    bl,bh */
    logic8((uint8_t)(BH & BL));
L3708:
    /* 3708  jne     L3705 */
    if (!ZF) goto L3705;
L370A:
    /* 370A  test    bl,bl */
    logic8((uint8_t)(BL & BL));
L370C:
    /* 370C  jne     L3714 */
    if (!ZF) goto L3714;
L370E:
    /* 370E  xchg    di,cx */
    { uint16_t t_ = CX;
    CX = DI;
    DI = t_; }
L3710:
    /* 3710  xchg    si,bp */
    { uint16_t t_ = BP;
    BP = SI;
    SI = t_; }
L3712:
    /* 3712  xchg    bl,bh */
    { uint8_t t_ = BH;
    BH = BL;
    BL = t_; }
L3714: /* L3714 */
    /* 3714  mov     byte ptr ds:[44C6h],bh */
    wb(pDS, 0x44C6, BH);
L3718:
    /* 3718  mov     bh,0 */
    BH = 0x0;
L371A:
    /* 371A  jmp     word ptr [bx+44A6h] */
    return ASM_JMP(0x0085, rw(pDS, BX + 0x44A6));
L371E:
    /* 371E  mov     ax,word ptr ds:[3DF4h] */
    AX = rw(pDS, 0x3DF4);
L3721:
    /* 3721  jmp     short L3726 */
    goto L3726;
L3723:
    /* 3723  mov     ax,word ptr ds:[3DF8h] */
    AX = rw(pDS, 0x3DF8);
L3726: /* L3726 */
    /* 3726  mov     bx,ax */
    BX = AX;
L3728:
    /* 3728  sub     ax,di */
    AX = (uint16_t)(AX - DI);
L372A:
    /* 372A  imul    word ptr ds:[44A4h] */
    imul16(rw(pDS, 0x44A4));
L372E:
    /* 372E  idiv    word ptr ds:[44A2h] */
    if (asm_idiv16(rw(pDS, 0x44A2)) && (c = asm_divfault(0x0085, 0x372E, 4)) != 0) return c;
L3732:
    /* 3732  mov     di,bx */
    DI = BX;
L3734:
    /* 3734  add     si,ax */
    SI = (uint16_t)(SI + AX);
L3736:
    /* 3736  mov     bh,byte ptr ds:[44C6h] */
    BH = rb(pDS, 0x44C6);
L373A:
    /* 373A  jmp     L36D4 */
    goto L36D4;
L373C:
    /* 373C  mov     ax,word ptr ds:[3DFAh] */
    AX = rw(pDS, 0x3DFA);
L373F:
    /* 373F  jmp     short L3744 */
    goto L3744;
L3741:
    /* 3741  mov     ax,word ptr ds:[3DF6h] */
    AX = rw(pDS, 0x3DF6);
L3744: /* L3744 */
    /* 3744  mov     bx,ax */
    BX = AX;
L3746:
    /* 3746  sub     ax,si */
    AX = (uint16_t)(AX - SI);
L3748:
    /* 3748  imul    word ptr ds:[44A2h] */
    imul16(rw(pDS, 0x44A2));
L374C:
    /* 374C  idiv    word ptr ds:[44A4h] */
    if (asm_idiv16(rw(pDS, 0x44A4)) && (c = asm_divfault(0x0085, 0x374C, 4)) != 0) return c;
L3750:
    /* 3750  add     di,ax */
    DI = (uint16_t)(DI + AX);
L3752:
    /* 3752  mov     si,bx */
    SI = BX;
L3754:
    /* 3754  mov     bh,byte ptr ds:[44C6h] */
    BH = rb(pDS, 0x44C6);
L3758:
    /* 3758  jmp     L36D4 */
    goto L36D4;
}
