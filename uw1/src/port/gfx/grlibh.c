/* grlibh.c: replaces src/gfx/GRLIBH.ASM (seg003_3FDC, 3FDC..4133 of its
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
    case 0x3FDC: goto L3FDC;
    case 0x3FE0: goto L3FE0;
    case 0x3FE4: goto L3FE4;
    case 0x3FE6: goto L3FE6;
    case 0x3FE8: goto L3FE8;
    case 0x3FE9: goto L3FE9;
    case 0x3FEB: goto L3FEB;
    case 0x3FEC: goto L3FEC;
    case 0x3FEE: goto L3FEE;
    case 0x3FF0: goto L3FF0;
    case 0x3FF6: goto L3FF6;
    case 0x3FF9: goto L3FF9;
    case 0x3FFC: goto L3FFC;
    case 0x3FFF: goto L3FFF;
    case 0x4002: goto L4002;
    case 0x4004: goto L4004;
    case 0x400A: goto L400A;
    case 0x400D: goto L400D;
    case 0x4010: goto L4010;
    case 0x4013: goto L4013;
    case 0x4016: goto L4016;
    case 0x4019: goto L4019;
    case 0x401A: goto L401A;
    case 0x401E: goto L401E;
    case 0x4022: goto L4022;
    case 0x4026: goto L4026;
    case 0x402A: goto L402A;
    case 0x402D: goto L402D;
    case 0x4031: goto L4031;
    case 0x4035: goto L4035;
    case 0x4037: goto L4037;
    case 0x4039: goto L4039;
    case 0x403B: goto L403B;
    case 0x403D: goto L403D;
    case 0x403E: goto L403E;
    case 0x4042: goto L4042;
    case 0x4044: goto L4044;
    case 0x4046: goto L4046;
    case 0x404A: goto L404A;
    case 0x404C: goto L404C;
    case 0x404E: goto L404E;
    case 0x4050: goto L4050;
    case 0x4052: goto L4052;
    case 0x4056: goto L4056;
    case 0x4058: goto L4058;
    case 0x405C: goto L405C;
    case 0x405E: goto L405E;
    case 0x4060: goto L4060;
    case 0x4061: goto L4061;
    case 0x4064: goto L4064;
    case 0x4066: goto L4066;
    case 0x4068: goto L4068;
    case 0x4069: goto L4069;
    case 0x406A: goto L406A;
    case 0x406C: goto L406C;
    case 0x406D: goto L406D;
    case 0x406F: goto L406F;
    case 0x4071: goto L4071;
    case 0x4073: goto L4073;
    case 0x4075: goto L4075;
    case 0x4076: goto L4076;
    case 0x4078: goto L4078;
    case 0x407A: goto L407A;
    case 0x407C: goto L407C;
    case 0x407D: goto L407D;
    case 0x4080: goto L4080;
    case 0x4083: goto L4083;
    case 0x4085: goto L4085;
    case 0x4087: goto L4087;
    case 0x408B: goto L408B;
    case 0x408E: goto L408E;
    case 0x4091: goto L4091;
    case 0x4094: goto L4094;
    case 0x4096: goto L4096;
    case 0x4098: goto L4098;
    case 0x409C: goto L409C;
    case 0x409E: goto L409E;
    case 0x40A0: goto L40A0;
    case 0x40A1: goto L40A1;
    case 0x40A3: goto L40A3;
    case 0x40A6: goto L40A6;
    case 0x40A8: goto L40A8;
    case 0x40AA: goto L40AA;
    case 0x40AB: goto L40AB;
    case 0x40AD: goto L40AD;
    case 0x40B0: goto L40B0;
    case 0x40B2: goto L40B2;
    case 0x40B4: goto L40B4;
    case 0x40B5: goto L40B5;
    case 0x40B7: goto L40B7;
    case 0x40BA: goto L40BA;
    case 0x40BC: goto L40BC;
    case 0x40BE: goto L40BE;
    case 0x40BF: goto L40BF;
    case 0x40C1: goto L40C1;
    case 0x40C4: goto L40C4;
    case 0x40C6: goto L40C6;
    case 0x40C8: goto L40C8;
    case 0x40C9: goto L40C9;
    case 0x40CB: goto L40CB;
    case 0x40CE: goto L40CE;
    case 0x40D0: goto L40D0;
    case 0x40D2: goto L40D2;
    case 0x40D3: goto L40D3;
    case 0x40D5: goto L40D5;
    case 0x40D8: goto L40D8;
    case 0x40DA: goto L40DA;
    case 0x40DC: goto L40DC;
    case 0x40DD: goto L40DD;
    case 0x40DF: goto L40DF;
    case 0x40E2: goto L40E2;
    case 0x40E4: goto L40E4;
    case 0x40E6: goto L40E6;
    case 0x40E7: goto L40E7;
    case 0x40E9: goto L40E9;
    case 0x40EC: goto L40EC;
    case 0x40EE: goto L40EE;
    case 0x40F0: goto L40F0;
    case 0x40F1: goto L40F1;
    case 0x40F3: goto L40F3;
    case 0x40F6: goto L40F6;
    case 0x40F8: goto L40F8;
    case 0x40FA: goto L40FA;
    case 0x40FB: goto L40FB;
    case 0x40FD: goto L40FD;
    case 0x4100: goto L4100;
    case 0x4102: goto L4102;
    case 0x4104: goto L4104;
    case 0x4105: goto L4105;
    case 0x4107: goto L4107;
    case 0x410A: goto L410A;
    case 0x410C: goto L410C;
    case 0x410E: goto L410E;
    case 0x410F: goto L410F;
    case 0x4111: goto L4111;
    case 0x4114: goto L4114;
    case 0x4116: goto L4116;
    case 0x4118: goto L4118;
    case 0x4119: goto L4119;
    case 0x411B: goto L411B;
    case 0x411E: goto L411E;
    case 0x4120: goto L4120;
    case 0x4122: goto L4122;
    case 0x4123: goto L4123;
    case 0x4125: goto L4125;
    case 0x4128: goto L4128;
    case 0x412A: goto L412A;
    case 0x412C: goto L412C;
    case 0x412D: goto L412D;
    case 0x412F: goto L412F;
    case 0x4132: goto L4132;
    default: asm_bad_entry("GRLIBH.ASM", entry);
    }

    /* seg003_3FDC  (+3FDC)
       uline: order the ends so BX is the upper (y counts up), pick which side of each record the
       stepped x goes to by the line's direction, fill the records (_403E), close the list and jump
       to the span writer. */
L3FDC: /* _seg003_3FDC */
    /* 3FDC  mov     si,word ptr ds:[49AAh] */
    SI = rw(pDS, 0x49AA);
L3FE0:
    /* 3FE0  and     word ptr [si],7FFFh */
    ww(pDS, SI, (uint16_t)(rw(pDS, SI) & 0x7FFF));
L3FE4:
    /* 3FE4  cmp     bx,dx */
    sub16(BX, DX, 0);
L3FE6:
    /* 3FE6  jge     L3FEB */
    if (SF == OF) goto L3FEB;
L3FE8:
    /* 3FE8  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3FE9:
    /* 3FE9  xchg    bx,dx */
    { uint16_t t_ = DX;
    DX = BX;
    BX = t_; }
L3FEB: /* L3FEB */
    /* 3FEB  dec     dx */
    DX = (uint16_t)(DX - 1);
L3FEC:
    /* 3FEC  cmp     ax,cx */
    sub16(AX, CX, 0);
L3FEE:
    /* 3FEE  jle     L4004 */
    if (ZF || SF != OF) goto L4004;
L3FF0:
    /* 3FF0  mov     word ptr ds:[49AEh],2 */
    ww(pDS, 0x49AE, 0x2);
L3FF6:
    /* 3FF6  mov     di,44D0h */
    DI = 0x44D0;
L3FF9:
    /* 3FF9  mov     bp,0FFF6h */
    BP = 0xFFF6;
L3FFC:
    /* 3FFC  call    _seg003_403E */
    if ((c = asm_call(ASM_JMP(0x0090, 0x403E), 0x3FFF)) != 0) return c;
L3FFF:
    /* 3FFF  add     di,-8 */
    DI = (uint16_t)(DI + 0xFFF8);
L4002:
    /* 4002  jmp     short L4016 */
    goto L4016;
L4004: /* L4004 */
    /* 4004  mov     word ptr ds:[49AEh],0 */
    ww(pDS, 0x49AE, 0x0);
L400A:
    /* 400A  mov     di,44CEh */
    DI = 0x44CE;
L400D:
    /* 400D  mov     bp,0FFFAh */
    BP = 0xFFFA;
L4010:
    /* 4010  call    _seg003_403E */
    if ((c = asm_call(ASM_JMP(0x0090, 0x403E), 0x4013)) != 0) return c;
L4013:
    /* 4013  add     di,-4 */
    DI = (uint16_t)(DI + 0xFFFC);
L4016: /* L4016 */
    /* 4016  mov     ax,word ptr ds:[49B2h] */
    AX = rw(pDS, 0x49B2);
L4019:
    /* 4019  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L401A:
    /* 401A  add     di,word ptr ds:[49AEh] */
    DI = (uint16_t)(DI + rw(pDS, 0x49AE));
L401E:
    /* 401E  mov     word ptr ds:[49AAh],di */
    ww(pDS, 0x49AA, DI);
L4022:
    /* 4022  or      word ptr [di],8000h */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | 0x8000));
L4026:
    /* 4026  mov     si,word ptr ds:[49B4h] */
    SI = rw(pDS, 0x49B4);
L402A:
    /* 402A  sub     si,2 */
    SI = (uint16_t)(SI - 0x2);
L402D:
    /* 402D  sub     si,word ptr ds:[49AEh] */
    SI = sub16(SI, rw(pDS, 0x49AE), 0);
L4031:
    /* 4031  jmp     word ptr ds:[SPAN_WRITER] */
    return ASM_JMP(0x0090, rw(pDS, 0x4110));
L4035: /* L4035 */
    /* 4035  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4037:
    /* 4037  sub     cx,cx */
    CX = (uint16_t)(CX - CX);
L4039:
    /* 4039  sub     dx,dx */
    DX = (uint16_t)(DX - DX);
L403B:
    /* 403B  jmp     short L4080 */
    goto L4080;
L403D: /* L403D */
    /* 403D  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_403E  (+403E)
       _403E: fill the span records for rows BX down to DX (DI = the first record, BP = the offset
       from one record's stepped x to the other x). The slope (CX - AX) / rows is formed as a 16.16
       value in BX:CX by two divides; a vertical line takes slope 0. Rows are filled fifteen at a
       time by _409C, then the remainder by jumping into it through 4988. */
L403E: /* _seg003_403E */
    /* 403E  mov     word ptr ds:[49B2h],cx */
    ww(pDS, 0x49B2, CX);
L4042:
    /* 4042  mov     si,bx */
    SI = BX;
L4044:
    /* 4044  neg     si */
    SI = (uint16_t)-SI;
L4046:
    /* 4046  add     si,0C7h */
    SI = (uint16_t)(SI + 0xC7);
L404A:
    /* 404A  shl     si,1 */
    SI = (uint16_t)(SI << 1);
L404C:
    /* 404C  add     di,si */
    DI = (uint16_t)(DI + SI);
L404E:
    /* 404E  shl     si,1 */
    SI = (uint16_t)(SI << 1);
L4050:
    /* 4050  add     di,si */
    DI = (uint16_t)(DI + SI);
L4052:
    /* 4052  mov     word ptr ds:[49B4h],di */
    ww(pDS, 0x49B4, DI);
L4056:
    /* 4056  sub     bx,dx */
    BX = sub16(BX, DX, 0);
L4058:
    /* 4058  mov     word ptr ds:[49B0h],bx */
    ww(pDS, 0x49B0, BX);
L405C:
    /* 405C  mov     si,bx */
    SI = BX;
L405E:
    /* 405E  jle     L403D */
    if (ZF || SF != OF) goto L403D;
L4060:
    /* 4060  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4061:
    /* 4061  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L4064:
    /* 4064  sub     cx,ax */
    CX = sub16(CX, AX, 0);
L4066:
    /* 4066  je      L4035 */
    if (ZF) goto L4035;
L4068:
    /* 4068  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L4069:
    /* 4069  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L406A:
    /* 406A  idiv    si */
    if (asm_idiv16(SI) && (c = asm_divfault(0x0090, 0x406A, 2)) != 0) return c;
L406C:
    /* 406C  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L406D:
    /* 406D  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L406F:
    /* 406F  sar     dx,1 */
    DX = sar16(DX, 1);
L4071:
    /* 4071  rcr     ax,1 */
    AX = rcr16(AX, 1);
L4073:
    /* 4073  idiv    si */
    if (asm_idiv16(SI) && (c = asm_divfault(0x0090, 0x4073, 2)) != 0) return c;
L4075:
    /* 4075  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L4076:
    /* 4076  shl     ax,1 */
    AX = shl16(AX, 1);
L4078:
    /* 4078  rcl     dx,1 */
    DX = rcl16(DX, 1);
L407A:
    /* 407A  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L407C:
    /* 407C  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L407D:
    /* 407D  mov     dx,7FFFh */
    DX = 0x7FFF;
L4080: /* L4080 */
    /* 4080  cmp     si,10h */
    sub16(SI, 0x10, 0);
L4083:
    /* 4083  ja      L408B */
    if (!CF && !ZF) goto L408B;
L4085:
    /* 4085  shl     si,1 */
    SI = shl16(SI, 1);
L4087:
    /* 4087  jmp     word ptr [si+4988h] */
    return ASM_JMP(0x0090, rw(pDS, SI + 0x4988));
L408B: /* L408B */
    /* 408B  call    _seg003_409C */
    if ((c = asm_call(ASM_JMP(0x0090, 0x409C), 0x408E)) != 0) return c;
L408E:
    /* 408E  sub     si,0Fh */
    SI = (uint16_t)(SI - 0xF);
L4091:
    /* 4091  cmp     si,10h */
    sub16(SI, 0x10, 0);
L4094:
    /* 4094  ja      L408B */
    if (!CF && !ZF) goto L408B;
L4096:
    /* 4096  shl     si,1 */
    SI = shl16(SI, 1);
L4098:
    /* 4098  jmp     word ptr [si+4988h] */
    return ASM_JMP(0x0090, rw(pDS, SI + 0x4988));

    /* seg003_409C  (+409C)
       _409C: fifteen rows of the line, unrolled: add the slope to the 16.16 x (AX:DX), store it as
       this record's x and the previous record's other x. */
L409C: /* _seg003_409C */
    /* 409C  add     dx,cx */
    DX = add16(DX, CX, 0);
L409E:
    /* 409E  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L40A0:
    /* 40A0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L40A1:
    /* 40A1  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L40A3:
    /* 40A3  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L40A6:
    /* 40A6  add     dx,cx */
    DX = add16(DX, CX, 0);
L40A8:
    /* 40A8  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L40AA:
    /* 40AA  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L40AB:
    /* 40AB  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L40AD:
    /* 40AD  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L40B0:
    /* 40B0  add     dx,cx */
    DX = add16(DX, CX, 0);
L40B2:
    /* 40B2  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L40B4:
    /* 40B4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L40B5:
    /* 40B5  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L40B7:
    /* 40B7  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L40BA:
    /* 40BA  add     dx,cx */
    DX = add16(DX, CX, 0);
L40BC:
    /* 40BC  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L40BE:
    /* 40BE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L40BF:
    /* 40BF  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L40C1:
    /* 40C1  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L40C4:
    /* 40C4  add     dx,cx */
    DX = add16(DX, CX, 0);
L40C6:
    /* 40C6  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L40C8:
    /* 40C8  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L40C9:
    /* 40C9  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L40CB:
    /* 40CB  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L40CE:
    /* 40CE  add     dx,cx */
    DX = add16(DX, CX, 0);
L40D0:
    /* 40D0  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L40D2:
    /* 40D2  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L40D3:
    /* 40D3  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L40D5:
    /* 40D5  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L40D8:
    /* 40D8  add     dx,cx */
    DX = add16(DX, CX, 0);
L40DA:
    /* 40DA  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L40DC:
    /* 40DC  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L40DD:
    /* 40DD  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L40DF:
    /* 40DF  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L40E2:
    /* 40E2  add     dx,cx */
    DX = add16(DX, CX, 0);
L40E4:
    /* 40E4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L40E6:
    /* 40E6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L40E7:
    /* 40E7  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L40E9:
    /* 40E9  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L40EC:
    /* 40EC  add     dx,cx */
    DX = add16(DX, CX, 0);
L40EE:
    /* 40EE  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L40F0:
    /* 40F0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L40F1:
    /* 40F1  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L40F3:
    /* 40F3  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L40F6:
    /* 40F6  add     dx,cx */
    DX = add16(DX, CX, 0);
L40F8:
    /* 40F8  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L40FA:
    /* 40FA  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L40FB:
    /* 40FB  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L40FD:
    /* 40FD  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L4100:
    /* 4100  add     dx,cx */
    DX = add16(DX, CX, 0);
L4102:
    /* 4102  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L4104:
    /* 4104  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4105:
    /* 4105  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L4107:
    /* 4107  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L410A:
    /* 410A  add     dx,cx */
    DX = add16(DX, CX, 0);
L410C:
    /* 410C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L410E:
    /* 410E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L410F:
    /* 410F  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L4111:
    /* 4111  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L4114:
    /* 4114  add     dx,cx */
    DX = add16(DX, CX, 0);
L4116:
    /* 4116  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L4118:
    /* 4118  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4119:
    /* 4119  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L411B:
    /* 411B  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L411E:
    /* 411E  add     dx,cx */
    DX = add16(DX, CX, 0);
L4120:
    /* 4120  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L4122:
    /* 4122  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4123:
    /* 4123  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L4125:
    /* 4125  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L4128:
    /* 4128  add     dx,cx */
    DX = add16(DX, CX, 0);
L412A:
    /* 412A  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L412C:
    /* 412C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L412D:
    /* 412D  mov     word ptr [bp+di],ax */
    ww(pSS, BP + DI, AX);
L412F:
    /* 412F  add     di,4 */
    DI = add16(DI, 0x4, 0);
L4132:
    /* 4132  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
