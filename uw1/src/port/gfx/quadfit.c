/* quadfit.c: replaces src/gfx/QUADFIT.ASM (seg003_8E8, 08E8..0A45 of its
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

uint32_t asm_mod_QUADFIT(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x08E8: goto L08E8;
    case 0x08EB: goto L08EB;
    case 0x08EF: goto L08EF;
    case 0x08F3: goto L08F3;
    case 0x08F6: goto L08F6;
    case 0x08FA: goto L08FA;
    case 0x08FE: goto L08FE;
    case 0x0901: goto L0901;
    case 0x0905: goto L0905;
    case 0x0909: goto L0909;
    case 0x090B: goto L090B;
    case 0x090D: goto L090D;
    case 0x0910: goto L0910;
    case 0x0912: goto L0912;
    case 0x0915: goto L0915;
    case 0x0917: goto L0917;
    case 0x0919: goto L0919;
    case 0x091C: goto L091C;
    case 0x091E: goto L091E;
    case 0x0920: goto L0920;
    case 0x0923: goto L0923;
    case 0x0926: goto L0926;
    case 0x092A: goto L092A;
    case 0x092E: goto L092E;
    case 0x0930: goto L0930;
    case 0x0932: goto L0932;
    case 0x0935: goto L0935;
    case 0x0937: goto L0937;
    case 0x093A: goto L093A;
    case 0x093C: goto L093C;
    case 0x093E: goto L093E;
    case 0x0941: goto L0941;
    case 0x0943: goto L0943;
    case 0x0945: goto L0945;
    case 0x0948: goto L0948;
    case 0x094B: goto L094B;
    case 0x094F: goto L094F;
    case 0x0951: goto L0951;
    case 0x0953: goto L0953;
    case 0x0956: goto L0956;
    case 0x095A: goto L095A;
    case 0x095C: goto L095C;
    case 0x095F: goto L095F;
    case 0x0963: goto L0963;
    case 0x0965: goto L0965;
    case 0x0967: goto L0967;
    case 0x096A: goto L096A;
    case 0x096E: goto L096E;
    case 0x0970: goto L0970;
    case 0x0974: goto L0974;
    case 0x0978: goto L0978;
    case 0x097C: goto L097C;
    case 0x0980: goto L0980;
    case 0x0982: goto L0982;
    case 0x0984: goto L0984;
    case 0x0986: goto L0986;
    case 0x0988: goto L0988;
    case 0x098B: goto L098B;
    case 0x098C: goto L098C;
    case 0x098F: goto L098F;
    case 0x0993: goto L0993;
    case 0x0997: goto L0997;
    case 0x099A: goto L099A;
    case 0x099E: goto L099E;
    case 0x09A2: goto L09A2;
    case 0x09A5: goto L09A5;
    case 0x09A9: goto L09A9;
    case 0x09AD: goto L09AD;
    case 0x09AF: goto L09AF;
    case 0x09B1: goto L09B1;
    case 0x09B4: goto L09B4;
    case 0x09B6: goto L09B6;
    case 0x09B9: goto L09B9;
    case 0x09BB: goto L09BB;
    case 0x09BD: goto L09BD;
    case 0x09C0: goto L09C0;
    case 0x09C2: goto L09C2;
    case 0x09C4: goto L09C4;
    case 0x09C7: goto L09C7;
    case 0x09CA: goto L09CA;
    case 0x09CE: goto L09CE;
    case 0x09D2: goto L09D2;
    case 0x09D4: goto L09D4;
    case 0x09D6: goto L09D6;
    case 0x09D8: goto L09D8;
    case 0x09DB: goto L09DB;
    case 0x09DD: goto L09DD;
    case 0x09DF: goto L09DF;
    case 0x09E2: goto L09E2;
    case 0x09E4: goto L09E4;
    case 0x09E6: goto L09E6;
    case 0x09E9: goto L09E9;
    case 0x09EC: goto L09EC;
    case 0x09F0: goto L09F0;
    case 0x09F2: goto L09F2;
    case 0x09F4: goto L09F4;
    case 0x09F7: goto L09F7;
    case 0x09FB: goto L09FB;
    case 0x09FD: goto L09FD;
    case 0x0A00: goto L0A00;
    case 0x0A04: goto L0A04;
    case 0x0A06: goto L0A06;
    case 0x0A08: goto L0A08;
    case 0x0A0B: goto L0A0B;
    case 0x0A0F: goto L0A0F;
    case 0x0A11: goto L0A11;
    case 0x0A15: goto L0A15;
    case 0x0A19: goto L0A19;
    case 0x0A1D: goto L0A1D;
    case 0x0A21: goto L0A21;
    case 0x0A23: goto L0A23;
    case 0x0A25: goto L0A25;
    case 0x0A27: goto L0A27;
    case 0x0A29: goto L0A29;
    case 0x0A2C: goto L0A2C;
    case 0x0A2D: goto L0A2D;
    case 0x0A2F: goto L0A2F;
    case 0x0A31: goto L0A31;
    case 0x0A33: goto L0A33;
    case 0x0A35: goto L0A35;
    case 0x0A38: goto L0A38;
    case 0x0A39: goto L0A39;
    case 0x0A3B: goto L0A3B;
    case 0x0A3D: goto L0A3D;
    case 0x0A3F: goto L0A3F;
    case 0x0A41: goto L0A41;
    case 0x0A44: goto L0A44;
    default: asm_bad_entry("QUADFIT.ASM", entry);
    }

    /* seg003_8E8  (+8E8) */
L08E8: /* _seg003_8E8 */
    /* 08E8  mov     ax,word ptr ds:[93Eh] */
    AX = rw(pDS, 0x93E);
L08EB:
    /* 08EB  sub     word ptr ds:[940h],ax */
    ww(pDS, 0x940, (uint16_t)(rw(pDS, 0x940) - AX));
L08EF:
    /* 08EF  sub     word ptr ds:[942h],ax */
    ww(pDS, 0x942, (uint16_t)(rw(pDS, 0x942) - AX));
L08F3:
    /* 08F3  mov     ax,word ptr ds:[934h] */
    AX = rw(pDS, 0x934);
L08F6:
    /* 08F6  sub     word ptr ds:[936h],ax */
    ww(pDS, 0x936, (uint16_t)(rw(pDS, 0x936) - AX));
L08FA:
    /* 08FA  sub     word ptr ds:[938h],ax */
    ww(pDS, 0x938, (uint16_t)(rw(pDS, 0x938) - AX));
L08FE:
    /* 08FE  mov     ax,word ptr ds:[938h] */
    AX = rw(pDS, 0x938);
L0901:
    /* 0901  sub     ax,word ptr ds:[936h] */
    AX = (uint16_t)(AX - rw(pDS, 0x936));
L0905:
    /* 0905  imul    word ptr ds:[938h] */
    imul16(rw(pDS, 0x938));
L0909:
    /* 0909  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L090B:
    /* 090B  jne     L0910 */
    if (!ZF) goto L0910;
L090D:
    /* 090D  jmp     L0A2D */
    goto L0A2D;
L0910: /* L0910 */
    /* 0910  mov     cx,ax */
    CX = AX;
L0912:
    /* 0912  mov     ax,word ptr ds:[942h] */
    AX = rw(pDS, 0x942);
L0915:
    /* 0915  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L0917:
    /* 0917  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x0090, 0x0917, 2)) != 0) return c;
L0919:
    /* 0919  mov     word ptr ds:[94Ah],ax */
    ww(pDS, 0x94A, AX);
L091C:
    /* 091C  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L091E:
    /* 091E  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x0090, 0x091E, 2)) != 0) return c;
L0920:
    /* 0920  mov     word ptr ds:[948h],ax */
    ww(pDS, 0x948, AX);
L0923:
    /* 0923  mov     ax,word ptr ds:[938h] */
    AX = rw(pDS, 0x938);
L0926:
    /* 0926  sub     ax,word ptr ds:[936h] */
    AX = (uint16_t)(AX - rw(pDS, 0x936));
L092A:
    /* 092A  imul    word ptr ds:[936h] */
    imul16(rw(pDS, 0x936));
L092E:
    /* 092E  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L0930:
    /* 0930  jne     L0935 */
    if (!ZF) goto L0935;
L0932:
    /* 0932  jmp     L0A2D */
    goto L0A2D;
L0935: /* L0935 */
    /* 0935  mov     cx,ax */
    CX = AX;
L0937:
    /* 0937  mov     ax,word ptr ds:[940h] */
    AX = rw(pDS, 0x940);
L093A:
    /* 093A  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L093C:
    /* 093C  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x0090, 0x093C, 2)) != 0) return c;
L093E:
    /* 093E  mov     word ptr ds:[94Eh],ax */
    ww(pDS, 0x94E, AX);
L0941:
    /* 0941  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L0943:
    /* 0943  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x0090, 0x0943, 2)) != 0) return c;
L0945:
    /* 0945  mov     word ptr ds:[94Ch],ax */
    ww(pDS, 0x94C, AX);
L0948:
    /* 0948  mov     ax,word ptr ds:[938h] */
    AX = rw(pDS, 0x938);
L094B:
    /* 094B  mul     word ptr ds:[94Ch] */
    mul16(rw(pDS, 0x94C));
L094F:
    /* 094F  mov     bp,dx */
    BP = DX;
L0951:
    /* 0951  mov     si,ax */
    SI = AX;
L0953:
    /* 0953  mov     ax,word ptr ds:[938h] */
    AX = rw(pDS, 0x938);
L0956:
    /* 0956  mul     word ptr ds:[94Eh] */
    mul16(rw(pDS, 0x94E));
L095A:
    /* 095A  add     bp,ax */
    BP = (uint16_t)(BP + AX);
L095C:
    /* 095C  mov     ax,word ptr ds:[936h] */
    AX = rw(pDS, 0x936);
L095F:
    /* 095F  mul     word ptr ds:[948h] */
    mul16(rw(pDS, 0x948));
L0963:
    /* 0963  sub     si,ax */
    SI = sub16(SI, AX, 0);
L0965:
    /* 0965  sbb     bp,dx */
    BP = (uint16_t)(BP - DX - CF);
L0967:
    /* 0967  mov     ax,word ptr ds:[936h] */
    AX = rw(pDS, 0x936);
L096A:
    /* 096A  imul    word ptr ds:[94Ah] */
    imul16(rw(pDS, 0x94A));
L096E:
    /* 096E  sub     bp,ax */
    BP = (uint16_t)(BP - AX);
L0970:
    /* 0970  mov     bx,word ptr ds:[94Ah] */
    BX = rw(pDS, 0x94A);
L0974:
    /* 0974  mov     dx,word ptr ds:[948h] */
    DX = rw(pDS, 0x948);
L0978:
    /* 0978  sub     dx,word ptr ds:[94Ch] */
    DX = sub16(DX, rw(pDS, 0x94C), 0);
L097C:
    /* 097C  sbb     bx,word ptr ds:[94Eh] */
    BX = (uint16_t)(BX - rw(pDS, 0x94E) - CF);
L0980:
    /* 0980  add     si,dx */
    SI = add16(SI, DX, 0);
L0982:
    /* 0982  adc     bp,bx */
    BP = (uint16_t)(BP + BX + CF);
L0984:
    /* 0984  shl     dx,1 */
    DX = shl16(DX, 1);
L0986:
    /* 0986  rcl     bx,1 */
    BX = rcl16(BX, 1);
L0988:
    /* 0988  mov     ax,word ptr ds:[93Eh] */
    AX = rw(pDS, 0x93E);
L098B:
    /* 098B  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L098C:
    /* 098C  mov     ax,word ptr ds:[942h] */
    AX = rw(pDS, 0x942);
L098F:
    /* 098F  sub     word ptr ds:[944h],ax */
    ww(pDS, 0x944, (uint16_t)(rw(pDS, 0x944) - AX));
L0993:
    /* 0993  sub     word ptr ds:[946h],ax */
    ww(pDS, 0x946, (uint16_t)(rw(pDS, 0x946) - AX));
L0997:
    /* 0997  mov     ax,word ptr ds:[938h] */
    AX = rw(pDS, 0x938);
L099A:
    /* 099A  sub     word ptr ds:[93Ah],ax */
    ww(pDS, 0x93A, (uint16_t)(rw(pDS, 0x93A) - AX));
L099E:
    /* 099E  sub     word ptr ds:[93Ch],ax */
    ww(pDS, 0x93C, (uint16_t)(rw(pDS, 0x93C) - AX));
L09A2:
    /* 09A2  mov     ax,word ptr ds:[93Ch] */
    AX = rw(pDS, 0x93C);
L09A5:
    /* 09A5  sub     ax,word ptr ds:[93Ah] */
    AX = (uint16_t)(AX - rw(pDS, 0x93A));
L09A9:
    /* 09A9  imul    word ptr ds:[93Ch] */
    imul16(rw(pDS, 0x93C));
L09AD:
    /* 09AD  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L09AF:
    /* 09AF  jne     L09B4 */
    if (!ZF) goto L09B4;
L09B1:
    /* 09B1  jmp     L0A39 */
    goto L0A39;
L09B4: /* L09B4 */
    /* 09B4  mov     cx,ax */
    CX = AX;
L09B6:
    /* 09B6  mov     ax,word ptr ds:[946h] */
    AX = rw(pDS, 0x946);
L09B9:
    /* 09B9  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L09BB:
    /* 09BB  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x0090, 0x09BB, 2)) != 0) return c;
L09BD:
    /* 09BD  mov     word ptr ds:[94Ah],ax */
    ww(pDS, 0x94A, AX);
L09C0:
    /* 09C0  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L09C2:
    /* 09C2  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x0090, 0x09C2, 2)) != 0) return c;
L09C4:
    /* 09C4  mov     word ptr ds:[948h],ax */
    ww(pDS, 0x948, AX);
L09C7:
    /* 09C7  mov     ax,word ptr ds:[93Ch] */
    AX = rw(pDS, 0x93C);
L09CA:
    /* 09CA  sub     ax,word ptr ds:[93Ah] */
    AX = (uint16_t)(AX - rw(pDS, 0x93A));
L09CE:
    /* 09CE  imul    word ptr ds:[93Ah] */
    imul16(rw(pDS, 0x93A));
L09D2:
    /* 09D2  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L09D4:
    /* 09D4  je      L0A39 */
    if (ZF) goto L0A39;
L09D6:
    /* 09D6  mov     cx,ax */
    CX = AX;
L09D8:
    /* 09D8  mov     ax,word ptr ds:[944h] */
    AX = rw(pDS, 0x944);
L09DB:
    /* 09DB  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L09DD:
    /* 09DD  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x0090, 0x09DD, 2)) != 0) return c;
L09DF:
    /* 09DF  mov     word ptr ds:[94Eh],ax */
    ww(pDS, 0x94E, AX);
L09E2:
    /* 09E2  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L09E4:
    /* 09E4  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x0090, 0x09E4, 2)) != 0) return c;
L09E6:
    /* 09E6  mov     word ptr ds:[94Ch],ax */
    ww(pDS, 0x94C, AX);
L09E9:
    /* 09E9  mov     ax,word ptr ds:[93Ch] */
    AX = rw(pDS, 0x93C);
L09EC:
    /* 09EC  mul     word ptr ds:[94Ch] */
    mul16(rw(pDS, 0x94C));
L09F0:
    /* 09F0  mov     bp,dx */
    BP = DX;
L09F2:
    /* 09F2  mov     si,ax */
    SI = AX;
L09F4:
    /* 09F4  mov     ax,word ptr ds:[93Ch] */
    AX = rw(pDS, 0x93C);
L09F7:
    /* 09F7  mul     word ptr ds:[94Eh] */
    mul16(rw(pDS, 0x94E));
L09FB:
    /* 09FB  add     bp,ax */
    BP = (uint16_t)(BP + AX);
L09FD:
    /* 09FD  mov     ax,word ptr ds:[93Ah] */
    AX = rw(pDS, 0x93A);
L0A00:
    /* 0A00  mul     word ptr ds:[948h] */
    mul16(rw(pDS, 0x948));
L0A04:
    /* 0A04  sub     si,ax */
    SI = sub16(SI, AX, 0);
L0A06:
    /* 0A06  sbb     bp,dx */
    BP = (uint16_t)(BP - DX - CF);
L0A08:
    /* 0A08  mov     ax,word ptr ds:[93Ah] */
    AX = rw(pDS, 0x93A);
L0A0B:
    /* 0A0B  imul    word ptr ds:[94Ah] */
    imul16(rw(pDS, 0x94A));
L0A0F:
    /* 0A0F  sub     bp,ax */
    BP = (uint16_t)(BP - AX);
L0A11:
    /* 0A11  mov     bx,word ptr ds:[94Ah] */
    BX = rw(pDS, 0x94A);
L0A15:
    /* 0A15  mov     dx,word ptr ds:[948h] */
    DX = rw(pDS, 0x948);
L0A19:
    /* 0A19  sub     dx,word ptr ds:[94Ch] */
    DX = sub16(DX, rw(pDS, 0x94C), 0);
L0A1D:
    /* 0A1D  sbb     bx,word ptr ds:[94Eh] */
    BX = (uint16_t)(BX - rw(pDS, 0x94E) - CF);
L0A21:
    /* 0A21  add     si,dx */
    SI = add16(SI, DX, 0);
L0A23:
    /* 0A23  adc     bp,bx */
    BP = (uint16_t)(BP + BX + CF);
L0A25:
    /* 0A25  shl     dx,1 */
    DX = shl16(DX, 1);
L0A27:
    /* 0A27  rcl     bx,1 */
    BX = rcl16(BX, 1);
L0A29:
    /* 0A29  mov     ax,word ptr ds:[942h] */
    AX = rw(pDS, 0x942);
L0A2C:
    /* 0A2C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0A2D: /* L0A2D */
    /* 0A2D  xor     bp,bp */
    BP = (uint16_t)(BP ^ BP);
L0A2F:
    /* 0A2F  xor     bx,bx */
    BX = (uint16_t)(BX ^ BX);
L0A31:
    /* 0A31  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L0A33:
    /* 0A33  xor     si,si */
    SI = logic16((uint16_t)(SI ^ SI));
L0A35:
    /* 0A35  mov     ax,word ptr ds:[93Eh] */
    AX = rw(pDS, 0x93E);
L0A38:
    /* 0A38  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0A39: /* L0A39 */
    /* 0A39  xor     bp,bp */
    BP = (uint16_t)(BP ^ BP);
L0A3B:
    /* 0A3B  xor     bx,bx */
    BX = (uint16_t)(BX ^ BX);
L0A3D:
    /* 0A3D  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L0A3F:
    /* 0A3F  xor     si,si */
    SI = logic16((uint16_t)(SI ^ SI));
L0A41:
    /* 0A41  mov     ax,word ptr ds:[942h] */
    AX = rw(pDS, 0x942);
L0A44:
    /* 0A44  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
