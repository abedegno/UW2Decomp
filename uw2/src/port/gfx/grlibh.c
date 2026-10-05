/* grlibh.c: replaces src/gfx/GRLIBH.ASM (seg003_0272_375C, 375C..38B3 of its
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

uint32_t asm_mod_GRLIBH(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x375C: goto L375C;
    case 0x3760: goto L3760;
    case 0x3764: goto L3764;
    case 0x3766: goto L3766;
    case 0x3768: goto L3768;
    case 0x3769: goto L3769;
    case 0x376B: goto L376B;
    case 0x376C: goto L376C;
    case 0x376E: goto L376E;
    case 0x3770: goto L3770;
    case 0x3776: goto L3776;
    case 0x3779: goto L3779;
    case 0x377C: goto L377C;
    case 0x377F: goto L377F;
    case 0x3782: goto L3782;
    case 0x3784: goto L3784;
    case 0x378A: goto L378A;
    case 0x378D: goto L378D;
    case 0x3790: goto L3790;
    case 0x3793: goto L3793;
    case 0x3796: goto L3796;
    case 0x3799: goto L3799;
    case 0x379A: goto L379A;
    case 0x379E: goto L379E;
    case 0x37A2: goto L37A2;
    case 0x37A6: goto L37A6;
    case 0x37AA: goto L37AA;
    case 0x37AD: goto L37AD;
    case 0x37B1: goto L37B1;
    case 0x37B5: goto L37B5;
    case 0x37B7: goto L37B7;
    case 0x37B9: goto L37B9;
    case 0x37BB: goto L37BB;
    case 0x37BD: goto L37BD;
    case 0x37BE: goto L37BE;
    case 0x37C2: goto L37C2;
    case 0x37C4: goto L37C4;
    case 0x37C6: goto L37C6;
    case 0x37CA: goto L37CA;
    case 0x37CC: goto L37CC;
    case 0x37CE: goto L37CE;
    case 0x37D0: goto L37D0;
    case 0x37D2: goto L37D2;
    case 0x37D6: goto L37D6;
    case 0x37D8: goto L37D8;
    case 0x37DC: goto L37DC;
    case 0x37DE: goto L37DE;
    case 0x37E0: goto L37E0;
    case 0x37E1: goto L37E1;
    case 0x37E4: goto L37E4;
    case 0x37E6: goto L37E6;
    case 0x37E8: goto L37E8;
    case 0x37E9: goto L37E9;
    case 0x37EA: goto L37EA;
    case 0x37EC: goto L37EC;
    case 0x37ED: goto L37ED;
    case 0x37EF: goto L37EF;
    case 0x37F1: goto L37F1;
    case 0x37F3: goto L37F3;
    case 0x37F5: goto L37F5;
    case 0x37F6: goto L37F6;
    case 0x37F8: goto L37F8;
    case 0x37FA: goto L37FA;
    case 0x37FC: goto L37FC;
    case 0x37FD: goto L37FD;
    case 0x3800: goto L3800;
    case 0x3803: goto L3803;
    case 0x3805: goto L3805;
    case 0x3807: goto L3807;
    case 0x380B: goto L380B;
    case 0x380E: goto L380E;
    case 0x3811: goto L3811;
    case 0x3814: goto L3814;
    case 0x3816: goto L3816;
    case 0x3818: goto L3818;
    case 0x381C: goto L381C;
    case 0x381E: goto L381E;
    case 0x3820: goto L3820;
    case 0x3821: goto L3821;
    case 0x3823: goto L3823;
    case 0x3826: goto L3826;
    case 0x3828: goto L3828;
    case 0x382A: goto L382A;
    case 0x382B: goto L382B;
    case 0x382D: goto L382D;
    case 0x3830: goto L3830;
    case 0x3832: goto L3832;
    case 0x3834: goto L3834;
    case 0x3835: goto L3835;
    case 0x3837: goto L3837;
    case 0x383A: goto L383A;
    case 0x383C: goto L383C;
    case 0x383E: goto L383E;
    case 0x383F: goto L383F;
    case 0x3841: goto L3841;
    case 0x3844: goto L3844;
    case 0x3846: goto L3846;
    case 0x3848: goto L3848;
    case 0x3849: goto L3849;
    case 0x384B: goto L384B;
    case 0x384E: goto L384E;
    case 0x3850: goto L3850;
    case 0x3852: goto L3852;
    case 0x3853: goto L3853;
    case 0x3855: goto L3855;
    case 0x3858: goto L3858;
    case 0x385A: goto L385A;
    case 0x385C: goto L385C;
    case 0x385D: goto L385D;
    case 0x385F: goto L385F;
    case 0x3862: goto L3862;
    case 0x3864: goto L3864;
    case 0x3866: goto L3866;
    case 0x3867: goto L3867;
    case 0x3869: goto L3869;
    case 0x386C: goto L386C;
    case 0x386E: goto L386E;
    case 0x3870: goto L3870;
    case 0x3871: goto L3871;
    case 0x3873: goto L3873;
    case 0x3876: goto L3876;
    case 0x3878: goto L3878;
    case 0x387A: goto L387A;
    case 0x387B: goto L387B;
    case 0x387D: goto L387D;
    case 0x3880: goto L3880;
    case 0x3882: goto L3882;
    case 0x3884: goto L3884;
    case 0x3885: goto L3885;
    case 0x3887: goto L3887;
    case 0x388A: goto L388A;
    case 0x388C: goto L388C;
    case 0x388E: goto L388E;
    case 0x388F: goto L388F;
    case 0x3891: goto L3891;
    case 0x3894: goto L3894;
    case 0x3896: goto L3896;
    case 0x3898: goto L3898;
    case 0x3899: goto L3899;
    case 0x389B: goto L389B;
    case 0x389E: goto L389E;
    case 0x38A0: goto L38A0;
    case 0x38A2: goto L38A2;
    case 0x38A3: goto L38A3;
    case 0x38A5: goto L38A5;
    case 0x38A8: goto L38A8;
    case 0x38AA: goto L38AA;
    case 0x38AC: goto L38AC;
    case 0x38AD: goto L38AD;
    case 0x38AF: goto L38AF;
    case 0x38B2: goto L38B2;
    default: asm_bad_entry("GRLIBH.ASM", entry);
    }

    /* seg003_0272_375C  (+375C)
       uline: order the ends so BX is the upper (y counts up), pick which side of each record the
       stepped x goes to by the line's direction, fill the records (_37BE), close the list and jump
       to the span writer. */
L375C: /* _seg003_0272_375C */
    /* 375C  mov     si,word ptr ds:[49ACh] */
    SI = rw(pDS, 0x49AC);
L3760:
    /* 3760  and     word ptr [si],7FFFh */
    ww(pDS, SI, (uint16_t)(rw(pDS, SI) & 0x7FFF));
L3764:
    /* 3764  cmp     bx,dx */
    sub16(BX, DX, 0);
L3766:
    /* 3766  jge     L376B */
    if (SF == OF) goto L376B;
L3768:
    /* 3768  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3769:
    /* 3769  xchg    bx,dx */
    { uint16_t t_ = DX;
    DX = BX;
    BX = t_; }
L376B: /* L376B */
    /* 376B  dec     dx */
    DX = (uint16_t)(DX - 1);
L376C:
    /* 376C  cmp     ax,cx */
    sub16(AX, CX, 0);
L376E:
    /* 376E  jle     L3784 */
    if (ZF || SF != OF) goto L3784;
L3770:
    /* 3770  mov     word ptr ds:[49B0h],2 */
    ww(pDS, 0x49B0, 0x2);
L3776:
    /* 3776  mov     di,44D2h */
    DI = 0x44D2;
L3779:
    /* 3779  mov     bp,0FFF6h */
    BP = 0xFFF6;
L377C:
    /* 377C  call    _seg003_0272_37BE */
    if ((c = asm_call(ASM_JMP(0x0085, 0x37BE), 0x377F)) != 0) return c;
L377F:
    /* 377F  add     di,-8 */
    DI = (uint16_t)(DI + 0xFFF8);
L3782:
    /* 3782  jmp     short L3796 */
    goto L3796;
L3784: /* L3784 */
    /* 3784  mov     word ptr ds:[49B0h],0 */
    ww(pDS, 0x49B0, 0x0);
L378A:
    /* 378A  mov     di,44D0h */
    DI = 0x44D0;
L378D:
    /* 378D  mov     bp,0FFFAh */
    BP = 0xFFFA;
L3790:
    /* 3790  call    _seg003_0272_37BE */
    if ((c = asm_call(ASM_JMP(0x0085, 0x37BE), 0x3793)) != 0) return c;
L3793:
    /* 3793  add     di,-4 */
    DI = (uint16_t)(DI + 0xFFFC);
L3796: /* L3796 */
    /* 3796  mov     ax,word ptr ds:[49B4h] */
    AX = rw(pDS, 0x49B4);
L3799:
    /* 3799  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L379A:
    /* 379A  add     di,word ptr ds:[49B0h] */
    DI = (uint16_t)(DI + rw(pDS, 0x49B0));
L379E:
    /* 379E  mov     word ptr ds:[49ACh],di */
    ww(pDS, 0x49AC, DI);
L37A2:
    /* 37A2  or      word ptr [di],8000h */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | 0x8000));
L37A6:
    /* 37A6  mov     si,word ptr ds:[49B6h] */
    SI = rw(pDS, 0x49B6);
L37AA:
    /* 37AA  sub     si,2 */
    SI = (uint16_t)(SI - 0x2);
L37AD:
    /* 37AD  sub     si,word ptr ds:[49B0h] */
    SI = sub16(SI, rw(pDS, 0x49B0), 0);
L37B1:
    /* 37B1  jmp     word ptr ds:[4112h] */
    return ASM_JMP(0x0085, rw(pDS, 0x4112));
L37B5: /* L37B5 */
    /* 37B5  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L37B7:
    /* 37B7  sub     cx,cx */
    CX = (uint16_t)(CX - CX);
L37B9:
    /* 37B9  sub     dx,dx */
    DX = (uint16_t)(DX - DX);
L37BB:
    /* 37BB  jmp     short L3800 */
    goto L3800;
L37BD: /* L37BD */
    /* 37BD  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_37BE  (+37BE)
       _37BE: fill the span records for rows BX down to DX (DI = the first record, BP = the offset
       from one record's stepped x to the other x). The slope (CX - AX) / rows is formed as a 16.16
       value in BX:CX by two divides; a vertical line takes slope 0. Rows are filled fifteen at a
       time by _381C, then the remainder by jumping into it through 498A. */
L37BE: /* _seg003_0272_37BE */
    /* 37BE  mov     word ptr ds:[49B4h],cx */
    ww(pDS, 0x49B4, CX);
L37C2:
    /* 37C2  mov     si,bx */
    SI = BX;
L37C4:
    /* 37C4  neg     si */
    SI = (uint16_t)-SI;
L37C6:
    /* 37C6  add     si,0C7h */
    SI = (uint16_t)(SI + 0xC7);
L37CA:
    /* 37CA  shl     si,1 */
    SI = (uint16_t)(SI << 1);
L37CC:
    /* 37CC  add     di,si */
    DI = (uint16_t)(DI + SI);
L37CE:
    /* 37CE  shl     si,1 */
    SI = (uint16_t)(SI << 1);
L37D0:
    /* 37D0  add     di,si */
    DI = (uint16_t)(DI + SI);
L37D2:
    /* 37D2  mov     word ptr ds:[49B6h],di */
    ww(pDS, 0x49B6, DI);
L37D6:
    /* 37D6  sub     bx,dx */
    BX = sub16(BX, DX, 0);
L37D8:
    /* 37D8  mov     word ptr ds:[49B2h],bx */
    ww(pDS, 0x49B2, BX);
L37DC:
    /* 37DC  mov     si,bx */
    SI = BX;
L37DE:
    /* 37DE  jle     L37BD */
    if (ZF || SF != OF) goto L37BD;
L37E0:
    /* 37E0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L37E1:
    /* 37E1  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L37E4:
    /* 37E4  sub     cx,ax */
    CX = sub16(CX, AX, 0);
L37E6:
    /* 37E6  je      L37B5 */
    if (ZF) goto L37B5;
L37E8:
    /* 37E8  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L37E9:
    /* 37E9  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L37EA:
    /* 37EA  idiv    si */
    if (asm_idiv16(SI) && (c = asm_divfault(0x0085, 0x37EA, 2)) != 0) return c;
L37EC:
    /* 37EC  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L37ED:
    /* 37ED  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L37EF:
    /* 37EF  sar     dx,1 */
    DX = sar16(DX, 1);
L37F1:
    /* 37F1  rcr     ax,1 */
    AX = rcr16(AX, 1);
L37F3:
    /* 37F3  idiv    si */
    if (asm_idiv16(SI) && (c = asm_divfault(0x0085, 0x37F3, 2)) != 0) return c;
L37F5:
    /* 37F5  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L37F6:
    /* 37F6  shl     ax,1 */
    AX = shl16(AX, 1);
L37F8:
    /* 37F8  rcl     dx,1 */
    DX = rcl16(DX, 1);
L37FA:
    /* 37FA  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L37FC:
    /* 37FC  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L37FD:
    /* 37FD  mov     dx,7FFFh */
    DX = 0x7FFF;
L3800: /* L3800 */
    /* 3800  cmp     si,10h */
    sub16(SI, 0x10, 0);
L3803:
    /* 3803  ja      L380B */
    if (!CF && !ZF) goto L380B;
L3805:
    /* 3805  shl     si,1 */
    SI = shl16(SI, 1);
L3807:
    /* 3807  jmp     word ptr [si+498Ah] */
    return ASM_JMP(0x0085, rw(pDS, SI + 0x498A));
L380B: /* L380B */
    /* 380B  call    _seg003_0272_381C */
    if ((c = asm_call(ASM_JMP(0x0085, 0x381C), 0x380E)) != 0) return c;
L380E:
    /* 380E  sub     si,0Fh */
    SI = (uint16_t)(SI - 0xF);
L3811:
    /* 3811  cmp     si,10h */
    sub16(SI, 0x10, 0);
L3814:
    /* 3814  ja      L380B */
    if (!CF && !ZF) goto L380B;
L3816:
    /* 3816  shl     si,1 */
    SI = shl16(SI, 1);
L3818:
    /* 3818  jmp     word ptr [si+498Ah] */
    return ASM_JMP(0x0085, rw(pDS, SI + 0x498A));

    /* seg003_0272_381C  (+381C)
       _381C: fifteen rows of the line, unrolled: add the slope to the 16.16 x (AX:DX), store it as
       this record's x and the previous record's other x. */
L381C: /* _seg003_0272_381C */
    /* 381C  add     dx,cx */
    DX = add16(DX, CX, 0);
L381E:
    /* 381E  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3820:
    /* 3820  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3821:
    /* 3821  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L3823:
    /* 3823  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3826:
    /* 3826  add     dx,cx */
    DX = add16(DX, CX, 0);
L3828:
    /* 3828  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L382A:
    /* 382A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L382B:
    /* 382B  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L382D:
    /* 382D  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3830:
    /* 3830  add     dx,cx */
    DX = add16(DX, CX, 0);
L3832:
    /* 3832  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3834:
    /* 3834  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3835:
    /* 3835  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L3837:
    /* 3837  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L383A:
    /* 383A  add     dx,cx */
    DX = add16(DX, CX, 0);
L383C:
    /* 383C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L383E:
    /* 383E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L383F:
    /* 383F  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L3841:
    /* 3841  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3844:
    /* 3844  add     dx,cx */
    DX = add16(DX, CX, 0);
L3846:
    /* 3846  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3848:
    /* 3848  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3849:
    /* 3849  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L384B:
    /* 384B  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L384E:
    /* 384E  add     dx,cx */
    DX = add16(DX, CX, 0);
L3850:
    /* 3850  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3852:
    /* 3852  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3853:
    /* 3853  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L3855:
    /* 3855  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3858:
    /* 3858  add     dx,cx */
    DX = add16(DX, CX, 0);
L385A:
    /* 385A  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L385C:
    /* 385C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L385D:
    /* 385D  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L385F:
    /* 385F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3862:
    /* 3862  add     dx,cx */
    DX = add16(DX, CX, 0);
L3864:
    /* 3864  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3866:
    /* 3866  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3867:
    /* 3867  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L3869:
    /* 3869  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L386C:
    /* 386C  add     dx,cx */
    DX = add16(DX, CX, 0);
L386E:
    /* 386E  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3870:
    /* 3870  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3871:
    /* 3871  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L3873:
    /* 3873  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3876:
    /* 3876  add     dx,cx */
    DX = add16(DX, CX, 0);
L3878:
    /* 3878  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L387A:
    /* 387A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L387B:
    /* 387B  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L387D:
    /* 387D  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3880:
    /* 3880  add     dx,cx */
    DX = add16(DX, CX, 0);
L3882:
    /* 3882  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3884:
    /* 3884  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3885:
    /* 3885  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L3887:
    /* 3887  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L388A:
    /* 388A  add     dx,cx */
    DX = add16(DX, CX, 0);
L388C:
    /* 388C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L388E:
    /* 388E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L388F:
    /* 388F  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L3891:
    /* 3891  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3894:
    /* 3894  add     dx,cx */
    DX = add16(DX, CX, 0);
L3896:
    /* 3896  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3898:
    /* 3898  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3899:
    /* 3899  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L389B:
    /* 389B  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L389E:
    /* 389E  add     dx,cx */
    DX = add16(DX, CX, 0);
L38A0:
    /* 38A0  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L38A2:
    /* 38A2  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L38A3:
    /* 38A3  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L38A5:
    /* 38A5  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L38A8:
    /* 38A8  add     dx,cx */
    DX = add16(DX, CX, 0);
L38AA:
    /* 38AA  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L38AC:
    /* 38AC  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L38AD:
    /* 38AD  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L38AF:
    /* 38AF  add     di,4 */
    DI = add16(DI, 0x4, 0);
L38B2:
    /* 38B2  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
