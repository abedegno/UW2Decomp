/* grlibm.c: replaces src/gfx/GRLIBM.ASM (seg003_0272_5934, 5934..5B4F of its
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

uint32_t asm_mod_GRLIBM(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x5934: goto L5934;
    case 0x5936: goto L5936;
    case 0x5939: goto L5939;
    case 0x593C: goto L593C;
    case 0x593F: goto L593F;
    case 0x5943: goto L5943;
    case 0x5946: goto L5946;
    case 0x5949: goto L5949;
    case 0x594B: goto L594B;
    case 0x594E: goto L594E;
    case 0x5950: goto L5950;
    case 0x5953: goto L5953;
    case 0x5955: goto L5955;
    case 0x5956: goto L5956;
    case 0x5959: goto L5959;
    case 0x595C: goto L595C;
    case 0x595E: goto L595E;
    case 0x5960: goto L5960;
    case 0x5962: goto L5962;
    case 0x5964: goto L5964;
    case 0x5967: goto L5967;
    case 0x5969: goto L5969;
    case 0x596A: goto L596A;
    case 0x596B: goto L596B;
    case 0x596C: goto L596C;
    case 0x5970: goto L5970;
    case 0x5973: goto L5973;
    case 0x5976: goto L5976;
    case 0x5979: goto L5979;
    case 0x597D: goto L597D;
    case 0x597F: goto L597F;
    case 0x5983: goto L5983;
    case 0x5984: goto L5984;
    case 0x5987: goto L5987;
    case 0x5989: goto L5989;
    case 0x598B: goto L598B;
    case 0x598D: goto L598D;
    case 0x5990: goto L5990;
    case 0x5993: goto L5993;
    case 0x5995: goto L5995;
    case 0x5997: goto L5997;
    case 0x5999: goto L5999;
    case 0x599B: goto L599B;
    case 0x599F: goto L599F;
    case 0x59A3: goto L59A3;
    case 0x59A5: goto L59A5;
    case 0x59A7: goto L59A7;
    case 0x59AB: goto L59AB;
    case 0x59AD: goto L59AD;
    case 0x59AF: goto L59AF;
    case 0x59B1: goto L59B1;
    case 0x59B5: goto L59B5;
    case 0x59B7: goto L59B7;
    case 0x59BB: goto L59BB;
    case 0x59BD: goto L59BD;
    case 0x59C0: goto L59C0;
    case 0x59C3: goto L59C3;
    case 0x59C6: goto L59C6;
    case 0x59C9: goto L59C9;
    case 0x59CC: goto L59CC;
    case 0x59CE: goto L59CE;
    case 0x59D0: goto L59D0;
    case 0x59D1: goto L59D1;
    case 0x59D4: goto L59D4;
    case 0x59D7: goto L59D7;
    case 0x59D9: goto L59D9;
    case 0x59DC: goto L59DC;
    case 0x59DF: goto L59DF;
    case 0x59E1: goto L59E1;
    case 0x59E4: goto L59E4;
    case 0x59E5: goto L59E5;
    case 0x59E6: goto L59E6;
    case 0x59E7: goto L59E7;
    case 0x59EA: goto L59EA;
    case 0x59EB: goto L59EB;
    case 0x59ED: goto L59ED;
    case 0x59F1: goto L59F1;
    case 0x59F4: goto L59F4;
    case 0x59F6: goto L59F6;
    case 0x59F9: goto L59F9;
    case 0x59FC: goto L59FC;
    case 0x5A00: goto L5A00;
    case 0x5A02: goto L5A02;
    case 0x5A04: goto L5A04;
    case 0x5A06: goto L5A06;
    case 0x5A09: goto L5A09;
    case 0x5A0B: goto L5A0B;
    case 0x5A0D: goto L5A0D;
    case 0x5A0F: goto L5A0F;
    case 0x5A11: goto L5A11;
    case 0x5A14: goto L5A14;
    case 0x5A18: goto L5A18;
    case 0x5A1B: goto L5A1B;
    case 0x5A1F: goto L5A1F;
    case 0x5A22: goto L5A22;
    case 0x5A26: goto L5A26;
    case 0x5A28: goto L5A28;
    case 0x5A29: goto L5A29;
    case 0x5A2A: goto L5A2A;
    case 0x5A2C: goto L5A2C;
    case 0x5A2F: goto L5A2F;
    case 0x5A31: goto L5A31;
    case 0x5A33: goto L5A33;
    case 0x5A35: goto L5A35;
    case 0x5A37: goto L5A37;
    case 0x5A38: goto L5A38;
    case 0x5A3A: goto L5A3A;
    case 0x5A3C: goto L5A3C;
    case 0x5A3F: goto L5A3F;
    case 0x5A43: goto L5A43;
    case 0x5A45: goto L5A45;
    case 0x5A46: goto L5A46;
    case 0x5A48: goto L5A48;
    case 0x5A4B: goto L5A4B;
    case 0x5A4D: goto L5A4D;
    case 0x5A4F: goto L5A4F;
    case 0x5A51: goto L5A51;
    case 0x5A53: goto L5A53;
    case 0x5A54: goto L5A54;
    case 0x5A56: goto L5A56;
    case 0x5A58: goto L5A58;
    case 0x5A5B: goto L5A5B;
    case 0x5A5F: goto L5A5F;
    case 0x5A65: goto L5A65;
    case 0x5A6B: goto L5A6B;
    case 0x5A6F: goto L5A6F;
    case 0x5A73: goto L5A73;
    case 0x5A75: goto L5A75;
    case 0x5A79: goto L5A79;
    case 0x5A7B: goto L5A7B;
    case 0x5A7D: goto L5A7D;
    case 0x5A7F: goto L5A7F;
    case 0x5A83: goto L5A83;
    case 0x5A85: goto L5A85;
    case 0x5A89: goto L5A89;
    case 0x5A8A: goto L5A8A;
    case 0x5A8D: goto L5A8D;
    case 0x5A8E: goto L5A8E;
    case 0x5A92: goto L5A92;
    case 0x5A96: goto L5A96;
    case 0x5A99: goto L5A99;
    case 0x5A9D: goto L5A9D;
    case 0x5AA1: goto L5AA1;
    case 0x5AA3: goto L5AA3;
    case 0x5AA4: goto L5AA4;
    case 0x5AA8: goto L5AA8;
    case 0x5AAA: goto L5AAA;
    case 0x5AAE: goto L5AAE;
    case 0x5AB2: goto L5AB2;
    case 0x5AB5: goto L5AB5;
    case 0x5AB7: goto L5AB7;
    case 0x5ABA: goto L5ABA;
    case 0x5ABD: goto L5ABD;
    case 0x5AC0: goto L5AC0;
    case 0x5AC2: goto L5AC2;
    case 0x5AC5: goto L5AC5;
    case 0x5AC8: goto L5AC8;
    case 0x5ACB: goto L5ACB;
    case 0x5ACF: goto L5ACF;
    case 0x5AD1: goto L5AD1;
    case 0x5AD2: goto L5AD2;
    case 0x5AD3: goto L5AD3;
    case 0x5AD7: goto L5AD7;
    case 0x5AD9: goto L5AD9;
    case 0x5ADC: goto L5ADC;
    case 0x5AE0: goto L5AE0;
    case 0x5AE4: goto L5AE4;
    case 0x5AE6: goto L5AE6;
    case 0x5AE8: goto L5AE8;
    case 0x5AEC: goto L5AEC;
    case 0x5AF0: goto L5AF0;
    case 0x5AF2: goto L5AF2;
    case 0x5AF6: goto L5AF6;
    case 0x5AF9: goto L5AF9;
    case 0x5AFD: goto L5AFD;
    case 0x5B00: goto L5B00;
    case 0x5B01: goto L5B01;
    case 0x5B04: goto L5B04;
    case 0x5B05: goto L5B05;
    case 0x5B09: goto L5B09;
    case 0x5B0A: goto L5B0A;
    case 0x5B0C: goto L5B0C;
    case 0x5B10: goto L5B10;
    case 0x5B12: goto L5B12;
    case 0x5B14: goto L5B14;
    case 0x5B16: goto L5B16;
    case 0x5B1A: goto L5B1A;
    case 0x5B1C: goto L5B1C;
    case 0x5B20: goto L5B20;
    case 0x5B22: goto L5B22;
    case 0x5B25: goto L5B25;
    case 0x5B27: goto L5B27;
    case 0x5B2A: goto L5B2A;
    case 0x5B2D: goto L5B2D;
    case 0x5B2F: goto L5B2F;
    case 0x5B31: goto L5B31;
    case 0x5B32: goto L5B32;
    case 0x5B34: goto L5B34;
    case 0x5B37: goto L5B37;
    case 0x5B39: goto L5B39;
    case 0x5B3C: goto L5B3C;
    case 0x5B3F: goto L5B3F;
    case 0x5B40: goto L5B40;
    case 0x5B42: goto L5B42;
    case 0x5B45: goto L5B45;
    case 0x5B47: goto L5B47;
    case 0x5B4A: goto L5B4A;
    case 0x5B4D: goto L5B4D;
    default: asm_bad_entry("GRLIBM.ASM", entry);
    }
L5934: /* L5934 */
    /* 5934  jne     L593C */
    if (!ZF) goto L593C;
L5936:
    /* 5936  mov     si,415Eh */
    SI = 0x415E;
L5939:
    /* 5939  jmp     _seg003_0272_3686 */
    return ASM_JMP(0x0085, 0x3686);
L593C: /* L593C */
    /* 593C  mov     ax,word ptr ds:[415Eh] */
    AX = rw(pDS, 0x415E);
L593F:
    /* 593F  mov     bx,word ptr ds:[4160h] */
    BX = rw(pDS, 0x4160);
L5943:
    /* 5943  jmp     _seg003_0272_32B4 */
    return ASM_JMP(0x0085, 0x32B4);
L5946: /* _seg003_0272_5946 */
    /* 5946  cmp     cx,63h */
    sub16(CX, 0x63, 0);
L5949:
    /* 5949  ja      L5955 */
    if (!CF && !ZF) goto L5955;
L594B:
    /* 594B  cmp     cx,2 */
    sub16(CX, 0x2, 0);
L594E:
    /* 594E  jbe     L5934 */
    if (CF || ZF) goto L5934;
L5950:
    /* 5950  call    _seg003_0272_5B50 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x5B50), 0x5953)) != 0) return c;
L5953:
    /* 5953  jmp     short _seg003_0272_5959 */
    goto L5959;
L5955: /* L5955 */
    /* 5955  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L5956: /* _seg003_0272_5956 */
    /* 5956  call    _seg003_0272_5D47 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x5D47), 0x5959)) != 0) return c;
L5959: /* _seg003_0272_5959 */
    /* 5959  db      0E9h */
    goto L595C;

    /* Fill: close the vertex list by copying the first vertex after the last, find the top and
       bottom, initialise every row record to an empty span (left 3E8h, right 0FC18h), then step each
       edge (horizontal edges are handled at L5B0A), and finally mark the end of the table (top bit
       of the row after the last) and call _C13. */
L595C: /* L595C */
    /* 595C  mov     di,cx */
    DI = CX;
L595E:
    /* 595E  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L5960:
    /* 5960  add     di,cx */
    DI = (uint16_t)(DI + CX);
L5962:
    /* 5962  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L5964:
    /* 5964  mov     si,415Eh */
    SI = 0x415E;
L5967:
    /* 5967  add     di,si */
    DI = (uint16_t)(DI + SI);
L5969:
    /* 5969  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L596A:
    /* 596A  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L596B:
    /* 596B  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L596C:
    /* 596C  mov     word ptr ds:[5680h],cx */
    ww(pDS, 0x5680, CX);
L5970:
    /* 5970  mov     bx,0FC18h */
    BX = 0xFC18;
L5973:
    /* 5973  mov     dx,3E8h */
    DX = 0x3E8;
L5976:
    /* 5976  mov     si,4160h */
    SI = 0x4160;
L5979: /* L5979 */
    /* 5979  test    word ptr [si],0FFFFh */
    logic16((uint16_t)(rw(pDS, SI) & 0xFFFF));
L597D:
    /* 597D  jns     L5983 */
    if (!SF) goto L5983;
L597F:
    /* 597F  mov     word ptr [si],0 */
    ww(pDS, SI, 0x0);
L5983: /* L5983 */
    /* 5983  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5984:
    /* 5984  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L5987:
    /* 5987  cmp     ax,bx */
    sub16(AX, BX, 0);
L5989:
    /* 5989  jl      L5993 */
    if (SF != OF) goto L5993;
L598B:
    /* 598B  mov     bx,ax */
    BX = AX;
L598D:
    /* 598D  mov     bp,word ptr [si-8] */
    BP = rw(pDS, SI + 0xFFF8);
L5990:
    /* 5990  mov     di,word ptr [si-4] */
    DI = rw(pDS, SI + 0xFFFC);
L5993: /* L5993 */
    /* 5993  cmp     ax,dx */
    sub16(AX, DX, 0);
L5995:
    /* 5995  jg      L5999 */
    if (!ZF && SF == OF) goto L5999;
L5997:
    /* 5997  mov     dx,ax */
    DX = AX;
L5999: /* L5999 */
    /* 5999  loop    L5979 */
    if (--CX) goto L5979;
L599B:
    /* 599B  mov     word ptr ds:[5670h],bx */
    ww(pDS, 0x5670, BX);
L599F:
    /* 599F  mov     word ptr ds:[566Eh],dx */
    ww(pDS, 0x566E, DX);
L59A3:
    /* 59A3  mov     ax,bx */
    AX = BX;
L59A5:
    /* 59A5  mov     si,bx */
    SI = BX;
L59A7:
    /* 59A7  sub     si,word ptr ds:[566Eh] */
    SI = (uint16_t)(SI - rw(pDS, 0x566E));
L59AB:
    /* 59AB  shl     si,1 */
    SI = (uint16_t)(SI << 1);
L59AD:
    /* 59AD  shl     si,1 */
    SI = (uint16_t)(SI << 1);
L59AF:
    /* 59AF  add     si,bx */
    SI = (uint16_t)(SI + BX);
L59B1:
    /* 59B1  sub     si,word ptr ds:[566Eh] */
    SI = (uint16_t)(SI - rw(pDS, 0x566E));
L59B5:
    /* 59B5  shl     si,1 */
    SI = (uint16_t)(SI << 1);
L59B7:
    /* 59B7  add     si,5692h */
    SI = (uint16_t)(SI + 0x5692);
L59BB:
    /* 59BB  mov     word ptr [si],ax */
    ww(pDS, SI, AX);
L59BD:
    /* 59BD  mov     word ptr [si+2],bp */
    ww(pDS, SI + 0x2, BP);
L59C0:
    /* 59C0  mov     word ptr [si+4],bp */
    ww(pDS, SI + 0x4, BP);
L59C3:
    /* 59C3  mov     word ptr [si+6],di */
    ww(pDS, SI + 0x6, DI);
L59C6:
    /* 59C6  mov     word ptr [si+8],di */
    ww(pDS, SI + 0x8, DI);
L59C9:
    /* 59C9  lea     di,[si-8] */
    DI = (uint16_t)(SI + 0xFFF8);
L59CC:
    /* 59CC  mov     cx,bx */
    CX = BX;
L59CE:
    /* 59CE  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L59D0:
    /* 59D0  inc     cx */
    CX = (uint16_t)(CX + 1);
L59D1:
    /* 59D1  mov     ax,3E8h */
    AX = 0x3E8;
L59D4:
    /* 59D4  mov     bx,0FC18h */
    BX = 0xFC18;
L59D7: /* L59D7 */
    /* 59D7  mov     word ptr [di],ax */
    ww(pDS, DI, AX);
L59D9:
    /* 59D9  mov     word ptr [di+2],bx */
    ww(pDS, DI + 0x2, BX);
L59DC:
    /* 59DC  sub     di,0Ah */
    DI = (uint16_t)(DI - 0xA);
L59DF:
    /* 59DF  loop    L59D7 */
    if (--CX) goto L59D7;
L59E1:
    /* 59E1  mov     si,415Eh */
    SI = 0x415E;
L59E4: /* L59E4 */
    /* 59E4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L59E5:
    /* 59E5  xchg    di,ax */
    { uint16_t t_ = DI;
    DI = AX;
    AX = t_; }
L59E6:
    /* 59E6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L59E7:
    /* 59E7  add     si,2 */
    SI = (uint16_t)(SI + 0x2);
L59EA:
    /* 59EA  push    si */
    push16(SI);
L59EB:
    /* 59EB  mov     bp,word ptr [si] */
    BP = rw(pDS, SI);
L59ED:
    /* 59ED  mov     word ptr ds:[5672h],bp */
    ww(pDS, 0x5672, BP);
L59F1:
    /* 59F1  mov     dx,word ptr [si+2] */
    DX = rw(pDS, SI + 0x2);
L59F4:
    /* 59F4  sub     di,bp */
    DI = (uint16_t)(DI - BP);
L59F6:
    /* 59F6  mov     bp,word ptr [si-2] */
    BP = rw(pDS, SI + 0xFFFE);
L59F9:
    /* 59F9  mov     bx,word ptr [si+4] */
    BX = rw(pDS, SI + 0x4);
L59FC:
    /* 59FC  mov     word ptr ds:[5686h],bx */
    ww(pDS, 0x5686, BX);
L5A00:
    /* 5A00  sub     bp,bx */
    BP = (uint16_t)(BP - BX);
L5A02:
    /* 5A02  sub     ax,dx */
    AX = sub16(AX, DX, 0);
L5A04:
    /* 5A04  jne     L5A09 */
    if (!ZF) goto L5A09;
L5A06:
    /* 5A06  jmp     L5B0A */
    goto L5B0A;
L5A09: /* L5A09 */
    /* 5A09  js      L5A22 */
    if (SF) goto L5A22;
L5A0B:
    /* 5A0B  neg     bp */
    BP = (uint16_t)-BP;
L5A0D:
    /* 5A0D  neg     ax */
    AX = (uint16_t)-AX;
L5A0F:
    /* 5A0F  neg     di */
    DI = (uint16_t)-DI;
L5A11:
    /* 5A11  mov     cx,word ptr [si-6] */
    CX = rw(pDS, SI + 0xFFFA);
L5A14:
    /* 5A14  mov     word ptr ds:[5672h],cx */
    ww(pDS, 0x5672, CX);
L5A18:
    /* 5A18  mov     cx,word ptr [si-2] */
    CX = rw(pDS, SI + 0xFFFE);
L5A1B:
    /* 5A1B  mov     word ptr ds:[5686h],cx */
    ww(pDS, 0x5686, CX);
L5A1F:
    /* 5A1F  mov     dx,word ptr [si-4] */
    DX = rw(pDS, SI + 0xFFFC);
L5A22: /* L5A22 */
    /* 5A22  mov     word ptr ds:[5676h],dx */
    ww(pDS, 0x5676, DX);
L5A26:
    /* 5A26  neg     ax */
    AX = (uint16_t)-AX;
L5A28:
    /* 5A28  xchg    di,ax */
    { uint16_t t_ = DI;
    DI = AX;
    AX = t_; }
L5A29:
    /* 5A29  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L5A2A:
    /* 5A2A  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x0085, 0x5A2A, 2)) != 0) return c;
L5A2C:
    /* 5A2C  mov     word ptr ds:[567Ah],ax */
    ww(pDS, 0x567A, AX);
L5A2F:
    /* 5A2F  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5A31:
    /* 5A31  sar     dx,1 */
    DX = sar16(DX, 1);
L5A33:
    /* 5A33  rcr     ax,1 */
    AX = rcr16(AX, 1);
L5A35:
    /* 5A35  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x0085, 0x5A35, 2)) != 0) return c;
L5A37:
    /* 5A37  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L5A38:
    /* 5A38  shl     ax,1 */
    AX = shl16(AX, 1);
L5A3A:
    /* 5A3A  rcl     dx,1 */
    DX = rcl16(DX, 1);
L5A3C:
    /* 5A3C  mov     word ptr ds:[5678h],ax */
    ww(pDS, 0x5678, AX);
L5A3F:
    /* 5A3F  add     word ptr ds:[567Ah],dx */
    ww(pDS, 0x567A, (uint16_t)(rw(pDS, 0x567A) + DX));
L5A43:
    /* 5A43  mov     ax,bp */
    AX = BP;
L5A45:
    /* 5A45  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L5A46:
    /* 5A46  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x0085, 0x5A46, 2)) != 0) return c;
L5A48:
    /* 5A48  mov     word ptr ds:[567Eh],ax */
    ww(pDS, 0x567E, AX);
L5A4B:
    /* 5A4B  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5A4D:
    /* 5A4D  sar     dx,1 */
    DX = sar16(DX, 1);
L5A4F:
    /* 5A4F  rcr     ax,1 */
    AX = rcr16(AX, 1);
L5A51:
    /* 5A51  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x0085, 0x5A51, 2)) != 0) return c;
L5A53:
    /* 5A53  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L5A54:
    /* 5A54  shl     ax,1 */
    AX = shl16(AX, 1);
L5A56:
    /* 5A56  rcl     dx,1 */
    DX = rcl16(DX, 1);
L5A58:
    /* 5A58  mov     word ptr ds:[567Ch],ax */
    ww(pDS, 0x567C, AX);
L5A5B:
    /* 5A5B  add     word ptr ds:[567Eh],dx */
    ww(pDS, 0x567E, (uint16_t)(rw(pDS, 0x567E) + DX));
L5A5F:
    /* 5A5F  mov     word ptr ds:[5674h],8000h */
    ww(pDS, 0x5674, 0x8000);
L5A65:
    /* 5A65  mov     word ptr ds:[5684h],8000h */
    ww(pDS, 0x5684, 0x8000);
L5A6B:
    /* 5A6B  mov     word ptr ds:[5682h],di */
    ww(pDS, 0x5682, DI);
L5A6F:
    /* 5A6F  mov     bx,word ptr ds:[5676h] */
    BX = rw(pDS, 0x5676);
L5A73:
    /* 5A73  mov     di,bx */
    DI = BX;
L5A75:
    /* 5A75  sub     di,word ptr ds:[566Eh] */
    DI = (uint16_t)(DI - rw(pDS, 0x566E));
L5A79:
    /* 5A79  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L5A7B:
    /* 5A7B  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L5A7D:
    /* 5A7D  add     di,bx */
    DI = (uint16_t)(DI + BX);
L5A7F:
    /* 5A7F  sub     di,word ptr ds:[566Eh] */
    DI = (uint16_t)(DI - rw(pDS, 0x566E));
L5A83:
    /* 5A83  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L5A85:
    /* 5A85  add     di,5692h */
    DI = (uint16_t)(DI + 0x5692);
L5A89:
    /* 5A89  dec     bx */
    BX = (uint16_t)(BX - 1);
L5A8A:
    /* 5A8A  sub     di,0Ah */
    DI = (uint16_t)(DI - 0xA);
L5A8D:
    /* 5A8D  push    cx */
    push16(CX);
L5A8E:
    /* 5A8E  mov     dx,word ptr ds:[567Ah] */
    DX = rw(pDS, 0x567A);
L5A92:
    /* 5A92  mov     bp,word ptr ds:[5674h] */
    BP = rw(pDS, 0x5674);
L5A96:
    /* 5A96  mov     ax,word ptr ds:[5672h] */
    AX = rw(pDS, 0x5672);
L5A99:
    /* 5A99  mov     si,word ptr ds:[5684h] */
    SI = rw(pDS, 0x5684);
L5A9D:
    /* 5A9D  mov     cx,word ptr ds:[5686h] */
    CX = rw(pDS, 0x5686);
L5AA1: /* L5AA1 */
    /* 5AA1  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L5AA3:
    /* 5AA3  dec     bx */
    BX = (uint16_t)(BX - 1);
L5AA4:
    /* 5AA4  add     bp,word ptr ds:[5678h] */
    BP = add16(BP, rw(pDS, 0x5678), 0);
L5AA8:
    /* 5AA8  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L5AAA:
    /* 5AAA  add     si,word ptr ds:[567Ch] */
    SI = add16(SI, rw(pDS, 0x567C), 0);
L5AAE:
    /* 5AAE  adc     cx,word ptr ds:[567Eh] */
    CX = (uint16_t)(CX + rw(pDS, 0x567E) + CF);
L5AB2:
    /* 5AB2  cmp     ax,word ptr [di+2] */
    sub16(AX, rw(pDS, DI + 0x2), 0);
L5AB5:
    /* 5AB5  jge     L5ABD */
    if (SF == OF) goto L5ABD;
L5AB7:
    /* 5AB7  mov     word ptr [di+2],ax */
    ww(pDS, DI + 0x2, AX);
L5ABA:
    /* 5ABA  mov     word ptr [di+6],cx */
    ww(pDS, DI + 0x6, CX);
L5ABD: /* L5ABD */
    /* 5ABD  cmp     ax,word ptr [di+4] */
    sub16(AX, rw(pDS, DI + 0x4), 0);
L5AC0:
    /* 5AC0  jle     L5AC8 */
    if (ZF || SF != OF) goto L5AC8;
L5AC2:
    /* 5AC2  mov     word ptr [di+4],ax */
    ww(pDS, DI + 0x4, AX);
L5AC5:
    /* 5AC5  mov     word ptr [di+8],cx */
    ww(pDS, DI + 0x8, CX);
L5AC8: /* L5AC8 */
    /* 5AC8  sub     di,0Ah */
    DI = (uint16_t)(DI - 0xA);
L5ACB:
    /* 5ACB  dec     word ptr ds:[5682h] */
    ww(pDS, 0x5682, dec16(rw(pDS, 0x5682)));
L5ACF:
    /* 5ACF  jne     L5AA1 */
    if (!ZF) goto L5AA1;
L5AD1:
    /* 5AD1  pop     cx */
    CX = pop16();
L5AD2: /* L5AD2 */
    /* 5AD2  pop     si */
    SI = pop16();
L5AD3:
    /* 5AD3  dec     word ptr ds:[5680h] */
    ww(pDS, 0x5680, dec16(rw(pDS, 0x5680)));
L5AD7:
    /* 5AD7  je      L5ADC */
    if (ZF) goto L5ADC;
L5AD9:
    /* 5AD9  jmp     L59E4 */
    goto L59E4;
L5ADC: /* L5ADC */
    /* 5ADC  mov     di,word ptr ds:[5670h] */
    DI = rw(pDS, 0x5670);
L5AE0:
    /* 5AE0  sub     di,word ptr ds:[566Eh] */
    DI = (uint16_t)(DI - rw(pDS, 0x566E));
L5AE4:
    /* 5AE4  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L5AE6:
    /* 5AE6  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L5AE8:
    /* 5AE8  add     di,word ptr ds:[5670h] */
    DI = (uint16_t)(DI + rw(pDS, 0x5670));
L5AEC:
    /* 5AEC  sub     di,word ptr ds:[566Eh] */
    DI = (uint16_t)(DI - rw(pDS, 0x566E));
L5AF0:
    /* 5AF0  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L5AF2:
    /* 5AF2  add     di,5692h */
    DI = (uint16_t)(DI + 0x5692);
L5AF6:
    /* 5AF6  add     di,0Ah */
    DI = (uint16_t)(DI + 0xA);
L5AF9:
    /* 5AF9  or      byte ptr [di+1],80h */
    wb(pDS, DI + 0x1, (uint8_t)(rb(pDS, DI + 0x1) | 0x80));
L5AFD:
    /* 5AFD  mov     si,5692h */
    SI = 0x5692;
L5B00:
    /* 5B00  push    di */
    push16(DI);
L5B01:
    /* 5B01  call    _seg003_0272_C13 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x0C13), 0x5B04)) != 0) return c;
L5B04:
    /* 5B04  pop     di */
    DI = pop16();
L5B05:
    /* 5B05  and     byte ptr [di+1],7Fh */
    wb(pDS, DI + 0x1, logic8((uint8_t)(rb(pDS, DI + 0x1) & 0x7F)));
L5B09:
    /* 5B09  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L5B0A: /* L5B0A */
    /* 5B0A  mov     di,dx */
    DI = DX;
L5B0C:
    /* 5B0C  sub     di,word ptr ds:[566Eh] */
    DI = (uint16_t)(DI - rw(pDS, 0x566E));
L5B10:
    /* 5B10  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L5B12:
    /* 5B12  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L5B14:
    /* 5B14  add     di,dx */
    DI = (uint16_t)(DI + DX);
L5B16:
    /* 5B16  sub     di,word ptr ds:[566Eh] */
    DI = (uint16_t)(DI - rw(pDS, 0x566E));
L5B1A:
    /* 5B1A  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L5B1C:
    /* 5B1C  add     di,5692h */
    DI = (uint16_t)(DI + 0x5692);
L5B20:
    /* 5B20  mov     word ptr [di],dx */
    ww(pDS, DI, DX);
L5B22:
    /* 5B22  mov     ax,word ptr [si-6] */
    AX = rw(pDS, SI + 0xFFFA);
L5B25:
    /* 5B25  mov     cx,word ptr [si] */
    CX = rw(pDS, SI);
L5B27:
    /* 5B27  mov     bx,word ptr [si-2] */
    BX = rw(pDS, SI + 0xFFFE);
L5B2A:
    /* 5B2A  mov     dx,word ptr [si+4] */
    DX = rw(pDS, SI + 0x4);
L5B2D:
    /* 5B2D  cmp     ax,cx */
    sub16(AX, CX, 0);
L5B2F:
    /* 5B2F  jle     L5B34 */
    if (ZF || SF != OF) goto L5B34;
L5B31:
    /* 5B31  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L5B32:
    /* 5B32  xchg    bx,dx */
    { uint16_t t_ = DX;
    DX = BX;
    BX = t_; }
L5B34: /* L5B34 */
    /* 5B34  cmp     ax,word ptr [di+2] */
    sub16(AX, rw(pDS, DI + 0x2), 0);
L5B37:
    /* 5B37  jg      L5B3F */
    if (!ZF && SF == OF) goto L5B3F;
L5B39:
    /* 5B39  mov     word ptr [di+2],ax */
    ww(pDS, DI + 0x2, AX);
L5B3C:
    /* 5B3C  mov     word ptr [di+6],bx */
    ww(pDS, DI + 0x6, BX);
L5B3F: /* L5B3F */
    /* 5B3F  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L5B40:
    /* 5B40  xchg    bx,dx */
    { uint16_t t_ = DX;
    DX = BX;
    BX = t_; }
L5B42:
    /* 5B42  cmp     ax,word ptr [di+4] */
    sub16(AX, rw(pDS, DI + 0x4), 0);
L5B45:
    /* 5B45  jl      L5B4D */
    if (SF != OF) goto L5B4D;
L5B47:
    /* 5B47  mov     word ptr [di+4],ax */
    ww(pDS, DI + 0x4, AX);
L5B4A:
    /* 5B4A  mov     word ptr [di+8],bx */
    ww(pDS, DI + 0x8, BX);
L5B4D: /* L5B4D */
    /* 5B4D  jmp     L5AD2 */
    goto L5AD2;
}
