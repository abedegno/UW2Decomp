/* grlibf_x.c: replaces src/gfx/GRLIBF.ASM (seg003_0272_3206, 3206..3685 of its
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

uint32_t asm_mod_GRLIBF(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x321B: goto L321B;
    case 0x321F: goto L321F;
    case 0x3221: goto L3221;
    case 0x3222: goto L3222;
    case 0x3223: goto L3223;
    case 0x3226: goto L3226;
    case 0x3227: goto L3227;
    case 0x3228: goto L3228;
    case 0x322C: goto L322C;
    case 0x3232: goto L3232;
    case 0x3235: goto L3235;
    case 0x3239: goto L3239;
    case 0x323A: goto L323A;
    case 0x323E: goto L323E;
    case 0x3244: goto L3244;
    case 0x3247: goto L3247;
    case 0x324B: goto L324B;
    case 0x324C: goto L324C;
    case 0x3250: goto L3250;
    case 0x3256: goto L3256;
    case 0x3259: goto L3259;
    case 0x325D: goto L325D;
    case 0x325E: goto L325E;
    case 0x325F: goto L325F;
    case 0x3262: goto L3262;
    case 0x3263: goto L3263;
    case 0x3266: goto L3266;
    case 0x3269: goto L3269;
    case 0x326A: goto L326A;
    case 0x3299: goto L3299;
    case 0x329A: goto L329A;
    case 0x329F: goto L329F;
    case 0x32A1: goto L32A1;
    case 0x32A3: goto L32A3;
    case 0x32A7: goto L32A7;
    case 0x32A9: goto L32A9;
    case 0x32AD: goto L32AD;
    case 0x32AF: goto L32AF;
    case 0x32B1: goto L32B1;
    case 0x32B4: goto L32B4;
    case 0x32B8: goto L32B8;
    case 0x32BA: goto L32BA;
    case 0x32BE: goto L32BE;
    case 0x32C0: goto L32C0;
    case 0x32C4: goto L32C4;
    case 0x32C6: goto L32C6;
    case 0x32CA: goto L32CA;
    case 0x32CC: goto L32CC;
    case 0x32D2: goto L32D2;
    case 0x32D4: goto L32D4;
    case 0x32D7: goto L32D7;
    case 0x32D9: goto L32D9;
    case 0x32DA: goto L32DA;
    case 0x32DB: goto L32DB;
    case 0x32DC: goto L32DC;
    case 0x32DD: goto L32DD;
    case 0x32DE: goto L32DE;
    case 0x32E2: goto L32E2;
    case 0x32E6: goto L32E6;
    case 0x32E8: goto L32E8;
    case 0x32EC: goto L32EC;
    case 0x32EE: goto L32EE;
    case 0x32F0: goto L32F0;
    case 0x32F2: goto L32F2;
    case 0x32F4: goto L32F4;
    case 0x32F8: goto L32F8;
    case 0x32FA: goto L32FA;
    case 0x32FC: goto L32FC;
    case 0x32FE: goto L32FE;
    case 0x3300: goto L3300;
    case 0x3302: goto L3302;
    case 0x3306: goto L3306;
    case 0x3308: goto L3308;
    case 0x330A: goto L330A;
    case 0x330C: goto L330C;
    case 0x330E: goto L330E;
    case 0x3310: goto L3310;
    case 0x3311: goto L3311;
    case 0x3312: goto L3312;
    case 0x3313: goto L3313;
    case 0x3316: goto L3316;
    case 0x3319: goto L3319;
    case 0x331A: goto L331A;
    case 0x331D: goto L331D;
    case 0x3320: goto L3320;
    case 0x3321: goto L3321;
    case 0x3371: goto L3371;
    case 0x3374: goto L3374;
    case 0x3375: goto L3375;
    case 0x3376: goto L3376;
    case 0x3377: goto L3377;
    case 0x3378: goto L3378;
    case 0x3379: goto L3379;
    case 0x337A: goto L337A;
    case 0x337B: goto L337B;
    case 0x337E: goto L337E;
    case 0x3382: goto L3382;
    case 0x3386: goto L3386;
    case 0x3389: goto L3389;
    case 0x338C: goto L338C;
    case 0x3390: goto L3390;
    case 0x3394: goto L3394;
    case 0x3397: goto L3397;
    case 0x339A: goto L339A;
    case 0x339E: goto L339E;
    case 0x33A2: goto L33A2;
    case 0x33A5: goto L33A5;
    case 0x33A8: goto L33A8;
    case 0x33AC: goto L33AC;
    case 0x33B0: goto L33B0;
    case 0x33B3: goto L33B3;
    case 0x33B4: goto L33B4;
    case 0x33B7: goto L33B7;
    case 0x33BA: goto L33BA;
    case 0x33BC: goto L33BC;
    case 0x347E: goto L347E;
    case 0x347F: goto L347F;
    case 0x3483: goto L3483;
    case 0x3485: goto L3485;
    case 0x3489: goto L3489;
    case 0x348B: goto L348B;
    case 0x348D: goto L348D;
    case 0x348F: goto L348F;
    case 0x3490: goto L3490;
    case 0x3494: goto L3494;
    case 0x3496: goto L3496;
    case 0x3498: goto L3498;
    case 0x349A: goto L349A;
    case 0x349C: goto L349C;
    case 0x349E: goto L349E;
    case 0x34A2: goto L34A2;
    case 0x34A4: goto L34A4;
    case 0x34A6: goto L34A6;
    case 0x34A8: goto L34A8;
    case 0x34AA: goto L34AA;
    case 0x34AC: goto L34AC;
    case 0x34C2: goto L34C2;
    case 0x34C5: goto L34C5;
    case 0x34C7: goto L34C7;
    case 0x34CA: goto L34CA;
    case 0x34CD: goto L34CD;
    case 0x34CE: goto L34CE;
    case 0x34CF: goto L34CF;
    case 0x34D0: goto L34D0;
    case 0x34D1: goto L34D1;
    case 0x34D4: goto L34D4;
    case 0x34D6: goto L34D6;
    case 0x34D8: goto L34D8;
    case 0x34DA: goto L34DA;
    case 0x34DC: goto L34DC;
    case 0x34DD: goto L34DD;
    case 0x34DE: goto L34DE;
    case 0x34E1: goto L34E1;
    case 0x34E5: goto L34E5;
    case 0x34E9: goto L34E9;
    case 0x34EC: goto L34EC;
    case 0x34ED: goto L34ED;
    case 0x34EE: goto L34EE;
    case 0x34EF: goto L34EF;
    case 0x34F0: goto L34F0;
    case 0x34F2: goto L34F2;
    case 0x34F4: goto L34F4;
    case 0x34F6: goto L34F6;
    case 0x34F8: goto L34F8;
    case 0x34FA: goto L34FA;
    case 0x34FC: goto L34FC;
    case 0x34FD: goto L34FD;
    case 0x34FF: goto L34FF;
    case 0x3505: goto L3505;
    case 0x350B: goto L350B;
    case 0x3511: goto L3511;
    case 0x3514: goto L3514;
    case 0x3517: goto L3517;
    case 0x3519: goto L3519;
    case 0x351D: goto L351D;
    case 0x3520: goto L3520;
    case 0x3526: goto L3526;
    case 0x3529: goto L3529;
    case 0x352C: goto L352C;
    case 0x352E: goto L352E;
    case 0x3532: goto L3532;
    case 0x3535: goto L3535;
    case 0x3539: goto L3539;
    case 0x353D: goto L353D;
    case 0x3540: goto L3540;
    case 0x3541: goto L3541;
    case 0x3542: goto L3542;
    case 0x3543: goto L3543;
    case 0x3544: goto L3544;
    case 0x3546: goto L3546;
    case 0x3548: goto L3548;
    case 0x354A: goto L354A;
    case 0x354C: goto L354C;
    case 0x354E: goto L354E;
    case 0x3550: goto L3550;
    case 0x3551: goto L3551;
    case 0x3553: goto L3553;
    case 0x3559: goto L3559;
    case 0x355F: goto L355F;
    case 0x3565: goto L3565;
    case 0x3568: goto L3568;
    case 0x356B: goto L356B;
    case 0x356D: goto L356D;
    case 0x3571: goto L3571;
    case 0x3574: goto L3574;
    case 0x357A: goto L357A;
    case 0x357D: goto L357D;
    case 0x3580: goto L3580;
    case 0x3582: goto L3582;
    case 0x3586: goto L3586;
    case 0x3589: goto L3589;
    case 0x358B: goto L358B;
    case 0x358C: goto L358C;
    case 0x358D: goto L358D;
    case 0x358E: goto L358E;
    case 0x358F: goto L358F;
    case 0x3591: goto L3591;
    case 0x3592: goto L3592;
    case 0x3596: goto L3596;
    case 0x359A: goto L359A;
    case 0x359E: goto L359E;
    case 0x35A2: goto L35A2;
    case 0x35A7: goto L35A7;
    case 0x35A9: goto L35A9;
    case 0x35AD: goto L35AD;
    case 0x35AE: goto L35AE;
    case 0x35B0: goto L35B0;
    case 0x35B2: goto L35B2;
    case 0x35B5: goto L35B5;
    case 0x35B6: goto L35B6;
    case 0x35B7: goto L35B7;
    case 0x35B8: goto L35B8;
    case 0x35BC: goto L35BC;
    case 0x35BE: goto L35BE;
    case 0x35C0: goto L35C0;
    case 0x35C3: goto L35C3;
    case 0x35C7: goto L35C7;
    case 0x35CB: goto L35CB;
    case 0x35CD: goto L35CD;
    case 0x35D1: goto L35D1;
    case 0x35D2: goto L35D2;
    case 0x35D3: goto L35D3;
    case 0x35D4: goto L35D4;
    case 0x35D5: goto L35D5;
    case 0x35D6: goto L35D6;
    case 0x35D7: goto L35D7;
    case 0x35DA: goto L35DA;
    case 0x35DE: goto L35DE;
    case 0x35E4: goto L35E4;
    case 0x35E8: goto L35E8;
    case 0x35E9: goto L35E9;
    case 0x35EA: goto L35EA;
    case 0x35ED: goto L35ED;
    case 0x35F1: goto L35F1;
    case 0x35F7: goto L35F7;
    case 0x35FB: goto L35FB;
    case 0x3601: goto L3601;
    case 0x3605: goto L3605;
    case 0x3609: goto L3609;
    case 0x360C: goto L360C;
    case 0x360D: goto L360D;
    case 0x3611: goto L3611;
    case 0x3612: goto L3612;
    case 0x3614: goto L3614;
    case 0x3615: goto L3615;
    case 0x3616: goto L3616;
    case 0x3617: goto L3617;
    case 0x3618: goto L3618;
    case 0x3619: goto L3619;
    case 0x361A: goto L361A;
    case 0x361B: goto L361B;
    case 0x361C: goto L361C;
    case 0x361F: goto L361F;
    case 0x3621: goto L3621;
    case 0x3623: goto L3623;
    case 0x3625: goto L3625;
    case 0x3627: goto L3627;
    case 0x3629: goto L3629;
    case 0x362B: goto L362B;
    case 0x362D: goto L362D;
    case 0x362F: goto L362F;
    case 0x3633: goto L3633;
    case 0x3635: goto L3635;
    case 0x3639: goto L3639;
    case 0x363B: goto L363B;
    case 0x363F: goto L363F;
    case 0x3640: goto L3640;
    case 0x3641: goto L3641;
    case 0x3642: goto L3642;
    case 0x3644: goto L3644;
    case 0x3646: goto L3646;
    case 0x3648: goto L3648;
    case 0x364C: goto L364C;
    case 0x364D: goto L364D;
    case 0x364E: goto L364E;
    case 0x364F: goto L364F;
    case 0x3650: goto L3650;
    case 0x3651: goto L3651;
    case 0x3652: goto L3652;
    case 0x3653: goto L3653;
    case 0x3654: goto L3654;
    case 0x3657: goto L3657;
    case 0x3658: goto L3658;
    case 0x365A: goto L365A;
    case 0x365C: goto L365C;
    case 0x365E: goto L365E;
    case 0x3660: goto L3660;
    case 0x3662: goto L3662;
    case 0x3664: goto L3664;
    case 0x3666: goto L3666;
    case 0x3668: goto L3668;
    case 0x366A: goto L366A;
    case 0x366E: goto L366E;
    case 0x3670: goto L3670;
    case 0x3674: goto L3674;
    case 0x3676: goto L3676;
    case 0x367A: goto L367A;
    case 0x367B: goto L367B;
    case 0x367C: goto L367C;
    case 0x367D: goto L367D;
    case 0x367E: goto L367E;
    case 0x367F: goto L367F;
    case 0x3681: goto L3681;
    case 0x3683: goto L3683;
    default: asm_bad_entry("GRLIBF.ASM", entry);
    }

    /* seg003_0272_321B  (+321B)
       Give video memory back: set the bump pointer to AX, unless AX is below the floor 4106 (carry
       set). GRCORE's _49F4 is the C wrapper.

       The unlabelled code after the ret (and so after this proc's first return) is four small
       routines that nothing in the sources jumps to by name: a clipped line drawn with span writer
       52CC, the same with 52D2, an unclipped line with 52CF, and a page-flipped clear (flip, _326A,
       flip back). */
L321B: /* _seg003_0272_321B */
    /* 321B  cmp     ax,word ptr ds:[4106h] */
    sub16(AX, rw(pDS, 0x4106), 0);
L321F:
    /* 321F  jae     L3223 */
    if (!CF) goto L3223;
L3221:
    /* 3221  stc */
    CF = 1;
L3222:
    /* 3222  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3223: /* L3223 */
    /* 3223  mov     word ptr ds:[410Ah],ax */
    ww(pDS, 0x410A, AX);
L3226:
    /* 3226  clc */
    CF = 0;
L3227:
    /* 3227  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3228:
    /* 3228  push    word ptr ds:[4112h] */
    push16(rw(pDS, 0x4112));
L322C:
    /* 322C  mov     word ptr ds:[4112h],offset _seg003_0272_52CC */
    ww(pDS, 0x4112, 0x52CC);
L3232:
    /* 3232  call    _seg003_0272_368E */
    if ((c = asm_call(ASM_JMP(0x0085, 0x368E), 0x3235)) != 0) return c;
L3235:
    /* 3235  pop     word ptr ds:[4112h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x4112, t_); }
L3239:
    /* 3239  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L323A:
    /* 323A  push    word ptr ds:[4112h] */
    push16(rw(pDS, 0x4112));
L323E:
    /* 323E  mov     word ptr ds:[4112h],offset _seg003_0272_52D2 */
    ww(pDS, 0x4112, 0x52D2);
L3244:
    /* 3244  call    _seg003_0272_368E */
    if ((c = asm_call(ASM_JMP(0x0085, 0x368E), 0x3247)) != 0) return c;
L3247:
    /* 3247  pop     word ptr ds:[4112h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x4112, t_); }
L324B:
    /* 324B  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L324C:
    /* 324C  push    word ptr ds:[4112h] */
    push16(rw(pDS, 0x4112));
L3250:
    /* 3250  mov     word ptr ds:[4112h],offset _seg003_0272_52CF */
    ww(pDS, 0x4112, 0x52CF);
L3256:
    /* 3256  call    _seg003_0272_375C */
    if ((c = asm_call(ASM_JMP(0x0085, 0x375C), 0x3259)) != 0) return c;
L3259:
    /* 3259  pop     word ptr ds:[4112h] */
    { uint16_t t_ = pop16(); ww(pDS, 0x4112, t_); }
L325D:
    /* 325D  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L325E:
    /* 325E  push    ax */
    push16(AX);
L325F:
    /* 325F  call    _seg003_0272_5299 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x5299), 0x3262)) != 0) return c;
L3262:
    /* 3262  pop     ax */
    AX = pop16();
L3263:
    /* 3263  call    _seg003_0272_326A */
    if ((c = asm_call(ASM_JMP(0x0085, 0x326A), 0x3266)) != 0) return c;
L3266:
    /* 3266  call    _seg003_0272_5299 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x5299), 0x3269)) != 0) return c;
L3269:
    /* 3269  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_326A  (+326A)
       _326A: set the colour (AX, through set_the_color's vector 52B7) and fall into _326D. */
L326A: /* _seg003_0272_326A */
    /* 326A  call    _seg003_0272_52B7 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x52B7), 0x326D)) != 0) return c;
    return ASM_JMP(0x0085, 0x326D);   /* into hand-written C */
L3299: /* L3299 */
    /* 3299  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L329A: /* L329A */
    /* 329A  cmp     word ptr ds:[5046h],0 */
    sub16(rw(pDS, 0x5046), 0x0, 0);
L329F:
    /* 329F  je      L32B1 */
    if (ZF) goto L32B1;
L32A1:
    /* 32A1  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L32A3:
    /* 32A3  cmp     ax,word ptr [bx+5046h] */
    sub16(AX, rw(pDS, BX + 0x5046), 0);
L32A7:
    /* 32A7  jge     L32AF */
    if (SF == OF) goto L32AF;
L32A9:
    /* 32A9  cmp     ax,word ptr [bx+5046h] */
    sub16(AX, rw(pDS, BX + 0x5046), 0);
L32AD:
    /* 32AD  jge     L3299 */
    if (SF == OF) goto L3299;
L32AF: /* L32AF */
    /* 32AF  shr     bx,1 */
    BX = shr16(BX, 1);
L32B1: /* L32B1 */
    /* 32B1  jmp     _seg003_0272_52A8 */
    return ASM_JMP(0x0085, 0x52A8);
L32B4: /* _seg003_0272_32B4 */
    /* 32B4  cmp     bx,word ptr ds:[3DF6h] */
    sub16(BX, rw(pDS, 0x3DF6), 0);
L32B8:
    /* 32B8  jg      L3299 */
    if (!ZF && SF == OF) goto L3299;
L32BA:
    /* 32BA  cmp     bx,word ptr ds:[3DFAh] */
    sub16(BX, rw(pDS, 0x3DFA), 0);
L32BE:
    /* 32BE  jl      L3299 */
    if (SF != OF) goto L3299;
L32C0:
    /* 32C0  cmp     ax,word ptr ds:[3DF8h] */
    sub16(AX, rw(pDS, 0x3DF8), 0);
L32C4:
    /* 32C4  jg      L3299 */
    if (!ZF && SF == OF) goto L3299;
L32C6:
    /* 32C6  cmp     ax,word ptr ds:[3DF4h] */
    sub16(AX, rw(pDS, 0x3DF4), 0);
L32CA:
    /* 32CA  jl      L3299 */
    if (SF != OF) goto L3299;
L32CC:
    /* 32CC  cmp     word ptr ds:[4112h],offset _seg003_0272_52C9 */
    sub16(rw(pDS, 0x4112), 0x52C9, 0);
L32D2:
    /* 32D2  je      L329A */
    if (ZF) goto L329A;
L32D4:
    /* 32D4  mov     di,414Ah */
    DI = 0x414A;
L32D7:
    /* 32D7  mov     si,di */
    SI = DI;
L32D9:
    /* 32D9  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L32DA:
    /* 32DA  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L32DB:
    /* 32DB  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L32DC:
    /* 32DC  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L32DD:
    /* 32DD  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L32DE:
    /* 32DE  jmp     word ptr ds:[4112h] */
    return ASM_JMP(0x0085, rw(pDS, 0x4112));

    /* seg003_0272_32E2  (+32E2)
       _32E2: clip a vertical line (x AX, y from BX to DX) to the window. Returns with BX >= DX
       inside the window, or discards its caller's return address (pop ax; ret) so the caller returns
       at once when the line is wholly outside. The code after it is a clipped vertical line through
       vector 52B1. */
L32E2: /* _seg003_0272_32E2 */
    /* 32E2  cmp     ax,word ptr ds:[3DF8h] */
    sub16(AX, rw(pDS, 0x3DF8), 0);
L32E6:
    /* 32E6  jg      L3311 */
    if (!ZF && SF == OF) goto L3311;
L32E8:
    /* 32E8  cmp     ax,word ptr ds:[3DF4h] */
    sub16(AX, rw(pDS, 0x3DF4), 0);
L32EC:
    /* 32EC  jl      L3311 */
    if (SF != OF) goto L3311;
L32EE:
    /* 32EE  cmp     bx,dx */
    sub16(BX, DX, 0);
L32F0:
    /* 32F0  jg      L32F4 */
    if (!ZF && SF == OF) goto L32F4;
L32F2:
    /* 32F2  xchg    bx,dx */
    { uint16_t t_ = DX;
    DX = BX;
    BX = t_; }
L32F4: /* L32F4 */
    /* 32F4  mov     si,word ptr ds:[3DFAh] */
    SI = rw(pDS, 0x3DFA);
L32F8:
    /* 32F8  cmp     bx,si */
    sub16(BX, SI, 0);
L32FA:
    /* 32FA  jl      L3311 */
    if (SF != OF) goto L3311;
L32FC:
    /* 32FC  cmp     dx,si */
    sub16(DX, SI, 0);
L32FE:
    /* 32FE  jg      L3302 */
    if (!ZF && SF == OF) goto L3302;
L3300:
    /* 3300  mov     dx,si */
    DX = SI;
L3302: /* L3302 */
    /* 3302  mov     si,word ptr ds:[3DF6h] */
    SI = rw(pDS, 0x3DF6);
L3306:
    /* 3306  cmp     dx,si */
    sub16(DX, SI, 0);
L3308:
    /* 3308  jg      L3311 */
    if (!ZF && SF == OF) goto L3311;
L330A:
    /* 330A  cmp     bx,si */
    sub16(BX, SI, 0);
L330C:
    /* 330C  jl      L3310 */
    if (SF != OF) goto L3310;
L330E:
    /* 330E  mov     bx,si */
    BX = SI;
L3310: /* L3310 */
    /* 3310  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3311: /* L3311 */
    /* 3311  pop     ax */
    AX = pop16();
L3312:
    /* 3312  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3313:
    /* 3313  call    _seg003_0272_32E2 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x32E2), 0x3316)) != 0) return c;
L3316:
    /* 3316  call    _seg003_0272_52B1 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x52B1), 0x3319)) != 0) return c;
L3319:
    /* 3319  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_331A  (+331A)
       _331A: a clipped vertical line through the vector at 52B4. */
L331A: /* _seg003_0272_331A */
    /* 331A  call    _seg003_0272_32E2 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x32E2), 0x331D)) != 0) return c;
L331D:
    /* 331D  call    _seg003_0272_52B4 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x52B4), 0x3320)) != 0) return c;
L3320:
    /* 3320  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_3321  (+3321)
       vline: clip (_32E2) and fall into uvline. */
L3321: /* _seg003_0272_3321 */
    /* 3321  call    _seg003_0272_32E2 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x32E2), 0x3324)) != 0) return c;
    return ASM_JMP(0x0085, 0x3324);   /* into hand-written C */

    /* seg003_0272_3371  (+3371)
       ubox: an outline rectangle with corners (AX, BX) and (CX, DX): two horizontal lines (uhline)
       and two vertical ones (uvline). The code after it is the same with the corners read from SI. */
L3371: /* _seg003_0272_3371 */
    /* 3371  mov     di,4152h */
    DI = 0x4152;
L3374:
    /* 3374  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3375:
    /* 3375  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3376:
    /* 3376  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3377:
    /* 3377  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3378:
    /* 3378  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3379:
    /* 3379  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L337A:
    /* 337A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L337B: /* L337B */
    /* 337B  mov     ax,word ptr ds:[4152h] */
    AX = rw(pDS, 0x4152);
L337E:
    /* 337E  mov     bx,word ptr ds:[4154h] */
    BX = rw(pDS, 0x4154);
L3382:
    /* 3382  mov     cx,word ptr ds:[4156h] */
    CX = rw(pDS, 0x4156);
L3386:
    /* 3386  call    _seg003_0272_34AE */
    if ((c = asm_call(ASM_JMP(0x0085, 0x34AE), 0x3389)) != 0) return c;
L3389:
    /* 3389  mov     ax,word ptr ds:[4152h] */
    AX = rw(pDS, 0x4152);
L338C:
    /* 338C  mov     bx,word ptr ds:[4158h] */
    BX = rw(pDS, 0x4158);
L3390:
    /* 3390  mov     cx,word ptr ds:[4156h] */
    CX = rw(pDS, 0x4156);
L3394:
    /* 3394  call    _seg003_0272_34AE */
    if ((c = asm_call(ASM_JMP(0x0085, 0x34AE), 0x3397)) != 0) return c;
L3397:
    /* 3397  mov     ax,word ptr ds:[4152h] */
    AX = rw(pDS, 0x4152);
L339A:
    /* 339A  mov     bx,word ptr ds:[4154h] */
    BX = rw(pDS, 0x4154);
L339E:
    /* 339E  mov     dx,word ptr ds:[4158h] */
    DX = rw(pDS, 0x4158);
L33A2:
    /* 33A2  call    _seg003_0272_3324 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x3324), 0x33A5)) != 0) return c;
L33A5:
    /* 33A5  mov     ax,word ptr ds:[4156h] */
    AX = rw(pDS, 0x4156);
L33A8:
    /* 33A8  mov     bx,word ptr ds:[4154h] */
    BX = rw(pDS, 0x4154);
L33AC:
    /* 33AC  mov     dx,word ptr ds:[4158h] */
    DX = rw(pDS, 0x4158);
L33B0:
    /* 33B0  call    _seg003_0272_3324 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x3324), 0x33B3)) != 0) return c;
L33B3:
    /* 33B3  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L33B4:
    /* 33B4  mov     di,4152h */
    DI = 0x4152;
L33B7:
    /* 33B7  mov     cx,4 */
    CX = 0x4;
L33BA:
    /* 33BA  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L33BC:
    /* 33BC  jmp     L337B */
    goto L337B;
L347E: /* L347E */
    /* 347E  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_347F  (+347F)
       hline: clip a horizontal line (y BX, x from AX to CX) to the window and draw it as one span;
       returns at once when it is outside. */
L347F: /* _seg003_0272_347F */
    /* 347F  cmp     bx,word ptr ds:[3DF6h] */
    sub16(BX, rw(pDS, 0x3DF6), 0);
L3483:
    /* 3483  jg      L347E */
    if (!ZF && SF == OF) goto L347E;
L3485:
    /* 3485  cmp     bx,word ptr ds:[3DFAh] */
    sub16(BX, rw(pDS, 0x3DFA), 0);
L3489:
    /* 3489  jl      L347E */
    if (SF != OF) goto L347E;
L348B:
    /* 348B  cmp     ax,cx */
    sub16(AX, CX, 0);
L348D:
    /* 348D  jl      L3490 */
    if (SF != OF) goto L3490;
L348F:
    /* 348F  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3490: /* L3490 */
    /* 3490  mov     dx,word ptr ds:[3DF4h] */
    DX = rw(pDS, 0x3DF4);
L3494:
    /* 3494  cmp     cx,dx */
    sub16(CX, DX, 0);
L3496:
    /* 3496  jl      L347E */
    if (SF != OF) goto L347E;
L3498:
    /* 3498  cmp     ax,dx */
    sub16(AX, DX, 0);
L349A:
    /* 349A  jg      L349E */
    if (!ZF && SF == OF) goto L349E;
L349C:
    /* 349C  mov     ax,dx */
    AX = DX;
L349E: /* L349E */
    /* 349E  mov     dx,word ptr ds:[3DF8h] */
    DX = rw(pDS, 0x3DF8);
L34A2:
    /* 34A2  cmp     ax,dx */
    sub16(AX, DX, 0);
L34A4:
    /* 34A4  jg      L347E */
    if (!ZF && SF == OF) goto L347E;
L34A6:
    /* 34A6  cmp     cx,dx */
    sub16(CX, DX, 0);
L34A8:
    /* 34A8  jl      L34AC */
    if (SF != OF) goto L34AC;
L34AA:
    /* 34AA  mov     cx,dx */
    CX = DX;
L34AC: /* L34AC */
    /* 34AC  jmp     short L34B3 */
    return ASM_JMP(0x0085, 0x34B3);

    /* seg003_0272_34C2  (+34C2)
       concave_shclip (FM Towns, 7 bytes before shclip there as here): shclip with the handler set at
       449A in place of 4492. */
L34C2: /* _seg003_0272_34C2 */
    /* 34C2  mov     si,449Ah */
    SI = 0x449A;
L34C5:
    /* 34C5  jmp     short L34CA */
    goto L34CA;

    /* seg003_0272_34C7  (+34C7)
       shclip: clip a polygon to the window, Sutherland-Hodgman, one edge at a time. In: CX = the
       vertex count, the vertices (x, y words) at 415E. A pass is skipped when every vertex is
       already inside on that axis. Each pass (L358F) walks the edges, classifies both ends by
       patching the comparison at L35B0 and L35BE (7Ch jl or 7Fh jg, followed by a far jump to flush
       the prefetch queue), and calls one of four handlers through 448A by the in/out case: keep, add
       the intersection, or drop. The intersection (L3611, L364C) is computed with imul/idiv, halving
       both terms on overflow, and nudged one pixel inward unless it lies on the window edge. Out: CX
       = the new count, the vertices back at 415E; when nothing is left it discards the caller's
       return address. */
L34C7: /* _seg003_0272_34C7 */
    /* 34C7  mov     si,4492h */
    SI = 0x4492;
L34CA: /* L34CA */
    /* 34CA  mov     di,448Ah */
    DI = 0x448A;
L34CD:
    /* 34CD  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L34CE:
    /* 34CE  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L34CF:
    /* 34CF  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L34D0:
    /* 34D0  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L34D1:
    /* 34D1  mov     si,415Eh */
    SI = 0x415E;
L34D4:
    /* 34D4  mov     di,cx */
    DI = CX;
L34D6:
    /* 34D6  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L34D8:
    /* 34D8  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L34DA:
    /* 34DA  add     di,si */
    DI = (uint16_t)(DI + SI);
L34DC:
    /* 34DC  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L34DD:
    /* 34DD  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L34DE:
    /* 34DE  sub     si,4 */
    SI = (uint16_t)(SI - 0x4);
L34E1:
    /* 34E1  mov     bx,word ptr ds:[3DF6h] */
    BX = rw(pDS, 0x3DF6);
L34E5:
    /* 34E5  mov     dx,word ptr ds:[3DFAh] */
    DX = rw(pDS, 0x3DFA);
L34E9:
    /* 34E9  mov     si,4160h */
    SI = 0x4160;
L34EC:
    /* 34EC  push    cx */
    push16(CX);
L34ED: /* L34ED */
    /* 34ED  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L34EE:
    /* 34EE  inc     si */
    SI = (uint16_t)(SI + 1);
L34EF:
    /* 34EF  inc     si */
    SI = (uint16_t)(SI + 1);
L34F0:
    /* 34F0  cmp     ax,bx */
    sub16(AX, BX, 0);
L34F2:
    /* 34F2  jg      L34FA */
    if (!ZF && SF == OF) goto L34FA;
L34F4:
    /* 34F4  cmp     ax,dx */
    sub16(AX, DX, 0);
L34F6:
    /* 34F6  jl      L34FA */
    if (SF != OF) goto L34FA;
L34F8:
    /* 34F8  loop    L34ED */
    if (--CX) goto L34ED;
L34FA: /* L34FA */
    /* 34FA  test    cx,cx */
    logic16((uint16_t)(CX & CX));
L34FC:
    /* 34FC  pop     cx */
    CX = pop16();
L34FD:
    /* 34FD  je      L3535 */
    if (ZF) goto L3535;
L34FF:
    /* 34FF  mov     word ptr ds:[4482h],2 */
    ww(pDS, 0x4482, 0x2);
L3505:
    /* 3505  mov     word ptr ds:[4484h],8 */
    ww(pDS, 0x4484, 0x8);
L350B:
    /* 350B  mov     word ptr ds:[4486h],offset L364C */
    ww(pDS, 0x4486, 0x364C);
L3511:
    /* 3511  mov     si,415Eh */
    SI = 0x415E;
L3514:
    /* 3514  mov     di,42EEh */
    DI = 0x42EE;
L3517:
    /* 3517  mov     al,7Ch */
    AL = 0x7C;
L3519:
    /* 3519  mov     dx,word ptr ds:[3DF6h] */
    DX = rw(pDS, 0x3DF6);
L351D:
    /* 351D  call    L358F */
    if ((c = asm_call(ASM_JMP(0x0085, 0x358F), 0x3520)) != 0) return c;
L3520:
    /* 3520  mov     word ptr ds:[4486h],offset L3648 */
    ww(pDS, 0x4486, 0x3648);
L3526:
    /* 3526  mov     si,42EEh */
    SI = 0x42EE;
L3529:
    /* 3529  mov     di,415Eh */
    DI = 0x415E;
L352C:
    /* 352C  mov     al,7Fh */
    AL = 0x7F;
L352E:
    /* 352E  mov     dx,word ptr ds:[3DFAh] */
    DX = rw(pDS, 0x3DFA);
L3532:
    /* 3532  call    L358F */
    if ((c = asm_call(ASM_JMP(0x0085, 0x358F), 0x3535)) != 0) return c;
L3535: /* L3535 */
    /* 3535  mov     bx,word ptr ds:[3DF8h] */
    BX = rw(pDS, 0x3DF8);
L3539:
    /* 3539  mov     dx,word ptr ds:[3DF4h] */
    DX = rw(pDS, 0x3DF4);
L353D:
    /* 353D  mov     si,415Eh */
    SI = 0x415E;
L3540:
    /* 3540  push    cx */
    push16(CX);
L3541: /* L3541 */
    /* 3541  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3542:
    /* 3542  inc     si */
    SI = (uint16_t)(SI + 1);
L3543:
    /* 3543  inc     si */
    SI = (uint16_t)(SI + 1);
L3544:
    /* 3544  cmp     ax,bx */
    sub16(AX, BX, 0);
L3546:
    /* 3546  jg      L354E */
    if (!ZF && SF == OF) goto L354E;
L3548:
    /* 3548  cmp     ax,dx */
    sub16(AX, DX, 0);
L354A:
    /* 354A  jl      L354E */
    if (SF != OF) goto L354E;
L354C:
    /* 354C  loop    L3541 */
    if (--CX) goto L3541;
L354E: /* L354E */
    /* 354E  test    cx,cx */
    logic16((uint16_t)(CX & CX));
L3550:
    /* 3550  pop     cx */
    CX = pop16();
L3551:
    /* 3551  je      L3589 */
    if (ZF) goto L3589;
L3553:
    /* 3553  mov     word ptr ds:[4482h],0 */
    ww(pDS, 0x4482, 0x0);
L3559:
    /* 3559  mov     word ptr ds:[4484h],6 */
    ww(pDS, 0x4484, 0x6);
L355F:
    /* 355F  mov     word ptr ds:[4486h],offset L3611 */
    ww(pDS, 0x4486, 0x3611);
L3565:
    /* 3565  mov     si,415Eh */
    SI = 0x415E;
L3568:
    /* 3568  mov     di,42EEh */
    DI = 0x42EE;
L356B:
    /* 356B  mov     al,7Fh */
    AL = 0x7F;
L356D:
    /* 356D  mov     dx,word ptr ds:[3DF4h] */
    DX = rw(pDS, 0x3DF4);
L3571:
    /* 3571  call    L358F */
    if ((c = asm_call(ASM_JMP(0x0085, 0x358F), 0x3574)) != 0) return c;
L3574:
    /* 3574  mov     word ptr ds:[4486h],offset L360D */
    ww(pDS, 0x4486, 0x360D);
L357A:
    /* 357A  mov     si,42EEh */
    SI = 0x42EE;
L357D:
    /* 357D  mov     di,415Eh */
    DI = 0x415E;
L3580:
    /* 3580  mov     al,7Ch */
    AL = 0x7C;
L3582:
    /* 3582  mov     dx,word ptr ds:[3DF8h] */
    DX = rw(pDS, 0x3DF8);
L3586:
    /* 3586  call    L358F */
    if ((c = asm_call(ASM_JMP(0x0085, 0x358F), 0x3589)) != 0) return c;
L3589: /* L3589 */
    /* 3589  jcxz    L358D */
    if (!CX) goto L358D;
L358B:
    /* 358B  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L358C: /* L358C */
    /* 358C  pop     ax */
    AX = pop16();
L358D: /* L358D */
    /* 358D  pop     ax */
    AX = pop16();
L358E:
    /* 358E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L358F: /* L358F */
    /* 358F  jcxz    L358C */
    if (!CX) goto L358C;
L3591:
    /* 3591  push    di */
    push16(DI);
L3592:
    /* 3592  mov     byte ptr cs:L35B0,al */
    wb(CODE003, 0x35B0, AL);
L3596:
    /* 3596  mov     byte ptr cs:L35BE,al */
    wb(CODE003, 0x35BE, AL);
L359A:
    /* 359A  mov     word ptr ds:[4480h],cx */
    ww(pDS, 0x4480, CX);
L359E:
    /* 359E  mov     word ptr ds:[447Eh],cx */
    ww(pDS, 0x447E, CX);
L35A2:
    /* 35A2  db      0EAh */
    return ASM_JMP(0x0085, 0x35A7);
L35A7: /* L35A7 */
    /* 35A7  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L35A9:
    /* 35A9  add     si,word ptr ds:[4482h] */
    SI = (uint16_t)(SI + rw(pDS, 0x4482));
L35AD:
    /* 35AD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L35AE:
    /* 35AE  cmp     ax,dx */
    sub16(AX, DX, 0);
L35B0: /* L35B0 */
    /* 35B0  jg      L35B5 */
    if (asm_jcc(CODE003[0x35B0])) goto L35B5;
L35B2:
    /* 35B2  mov     bx,4 */
    BX = 0x4;
L35B5: /* L35B5 */
    /* 35B5  inc     si */
    SI = (uint16_t)(SI + 1);
L35B6:
    /* 35B6  inc     si */
    SI = (uint16_t)(SI + 1);
L35B7:
    /* 35B7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L35B8:
    /* 35B8  sub     si,word ptr ds:[4484h] */
    SI = (uint16_t)(SI - rw(pDS, 0x4484));
L35BC:
    /* 35BC  cmp     ax,dx */
    sub16(AX, DX, 0);
L35BE: /* L35BE */
    /* 35BE  jg      L35C3 */
    if (asm_jcc(CODE003[0x35BE])) goto L35C3;
L35C0:
    /* 35C0  or      bx,2 */
    BX = (uint16_t)(BX | 0x2);
L35C3: /* L35C3 */
    /* 35C3  call    word ptr [bx+448Ah] */
    if ((c = asm_call(ASM_JMP(0x0085, rw(pDS, BX + 0x448A)), 0x35C7)) != 0) return c;
L35C7:
    /* 35C7  dec     word ptr ds:[4480h] */
    ww(pDS, 0x4480, dec16(rw(pDS, 0x4480)));
L35CB:
    /* 35CB  jg      L35A7 */
    if (!ZF && SF == OF) goto L35A7;
L35CD:
    /* 35CD  mov     cx,word ptr ds:[447Eh] */
    CX = rw(pDS, 0x447E);
L35D1:
    /* 35D1  pop     si */
    SI = pop16();
L35D2:
    /* 35D2  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L35D3:
    /* 35D3  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L35D4:
    /* 35D4  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L35D5:
    /* 35D5  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L35D6:
    /* 35D6  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L35D7:
    /* 35D7  sub     si,4 */
    SI = sub16(SI, 0x4, 0);
L35DA:
    /* 35DA  inc     word ptr ds:[447Eh] */
    ww(pDS, 0x447E, inc16(rw(pDS, 0x447E)));
L35DE:
    /* 35DE  mov     word ptr ds:[4488h],0 */
    ww(pDS, 0x4488, 0x0);
L35E4:
    /* 35E4  jmp     word ptr ds:[4486h] */
    return ASM_JMP(0x0085, rw(pDS, 0x4486));
L35E8:
    /* 35E8  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L35E9:
    /* 35E9  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L35EA:
    /* 35EA  sub     si,4 */
    SI = sub16(SI, 0x4, 0);
L35ED:
    /* 35ED  inc     word ptr ds:[447Eh] */
    ww(pDS, 0x447E, inc16(rw(pDS, 0x447E)));
L35F1:
    /* 35F1  mov     word ptr ds:[4488h],0 */
    ww(pDS, 0x4488, 0x0);
L35F7:
    /* 35F7  jmp     word ptr ds:[4486h] */
    return ASM_JMP(0x0085, rw(pDS, 0x4486));
L35FB:
    /* 35FB  mov     word ptr ds:[4488h],1 */
    ww(pDS, 0x4488, 0x1);
L3601:
    /* 3601  jmp     word ptr ds:[4486h] */
    return ASM_JMP(0x0085, rw(pDS, 0x4486));
L3605:
    /* 3605  dec     word ptr ds:[447Eh] */
    ww(pDS, 0x447E, (uint16_t)(rw(pDS, 0x447E) - 1));
L3609:
    /* 3609  add     si,4 */
    SI = add16(SI, 0x4, 0);
L360C:
    /* 360C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L360D: /* L360D */
    /* 360D  neg     word ptr ds:[4488h] */
    ww(pDS, 0x4488, (uint16_t)-rw(pDS, 0x4488));
L3611: /* L3611 */
    /* 3611  push    dx */
    push16(DX);
L3612:
    /* 3612  mov     ax,dx */
    AX = DX;
L3614:
    /* 3614  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3615:
    /* 3615  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3616:
    /* 3616  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3617:
    /* 3617  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3618:
    /* 3618  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3619:
    /* 3619  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L361A:
    /* 361A  xchg    bp,ax */
    { uint16_t t_ = BP;
    BP = AX;
    AX = t_; }
L361B:
    /* 361B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L361C:
    /* 361C  sub     si,4 */
    SI = (uint16_t)(SI - 0x4);
L361F:
    /* 361F  sub     ax,bx */
    AX = (uint16_t)(AX - BX);
L3621:
    /* 3621  sub     dx,cx */
    DX = (uint16_t)(DX - CX);
L3623:
    /* 3623  neg     cx */
    CX = (uint16_t)-CX;
L3625:
    /* 3625  add     cx,bp */
    CX = add16(CX, BP, 0);
L3627:
    /* 3627  jo      L3642 */
    if (OF) goto L3642;
L3629: /* L3629 */
    /* 3629  imul    dx */
    imul16(DX);
L362B:
    /* 362B  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0085, 0x362B, 2)) != 0) return c;
L362D:
    /* 362D  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L362F:
    /* 362F  cmp     ax,word ptr ds:[3DF6h] */
    sub16(AX, rw(pDS, 0x3DF6), 0);
L3633:
    /* 3633  jge     L363F */
    if (SF == OF) goto L363F;
L3635:
    /* 3635  cmp     ax,word ptr ds:[3DFAh] */
    sub16(AX, rw(pDS, 0x3DFA), 0);
L3639:
    /* 3639  jle     L363F */
    if (ZF || SF != OF) goto L363F;
L363B:
    /* 363B  add     ax,word ptr ds:[4488h] */
    AX = add16(AX, rw(pDS, 0x4488), 0);
L363F: /* L363F */
    /* 363F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3640:
    /* 3640  pop     dx */
    DX = pop16();
L3641:
    /* 3641  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3642: /* L3642 */
    /* 3642  rcr     cx,1 */
    CX = rcr16(CX, 1);
L3644:
    /* 3644  sar     dx,1 */
    DX = (uint16_t)((int16_t)DX >> 1);
L3646:
    /* 3646  jmp     L3629 */
    goto L3629;
L3648: /* L3648 */
    /* 3648  neg     word ptr ds:[4488h] */
    ww(pDS, 0x4488, (uint16_t)-rw(pDS, 0x4488));
L364C: /* L364C */
    /* 364C  push    dx */
    push16(DX);
L364D:
    /* 364D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L364E:
    /* 364E  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L364F:
    /* 364F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3650:
    /* 3650  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3651:
    /* 3651  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3652:
    /* 3652  xchg    bp,ax */
    { uint16_t t_ = BP;
    BP = AX;
    AX = t_; }
L3653:
    /* 3653  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3654:
    /* 3654  sub     si,4 */
    SI = (uint16_t)(SI - 0x4);
L3657:
    /* 3657  xchg    bp,ax */
    { uint16_t t_ = BP;
    BP = AX;
    AX = t_; }
L3658:
    /* 3658  xchg    bx,cx */
    { uint16_t t_ = CX;
    CX = BX;
    BX = t_; }
L365A:
    /* 365A  sub     ax,bx */
    AX = (uint16_t)(AX - BX);
L365C:
    /* 365C  sub     dx,cx */
    DX = (uint16_t)(DX - CX);
L365E:
    /* 365E  neg     cx */
    CX = (uint16_t)-CX;
L3660:
    /* 3660  add     cx,bp */
    CX = add16(CX, BP, 0);
L3662:
    /* 3662  jo      L367F */
    if (OF) goto L367F;
L3664: /* L3664 */
    /* 3664  imul    dx */
    imul16(DX);
L3666:
    /* 3666  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0085, 0x3666, 2)) != 0) return c;
L3668:
    /* 3668  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L366A:
    /* 366A  cmp     ax,word ptr ds:[3DF4h] */
    sub16(AX, rw(pDS, 0x3DF4), 0);
L366E:
    /* 366E  jle     L367A */
    if (ZF || SF != OF) goto L367A;
L3670:
    /* 3670  cmp     ax,word ptr ds:[3DF8h] */
    sub16(AX, rw(pDS, 0x3DF8), 0);
L3674:
    /* 3674  jge     L367A */
    if (SF == OF) goto L367A;
L3676:
    /* 3676  add     ax,word ptr ds:[4488h] */
    AX = add16(AX, rw(pDS, 0x4488), 0);
L367A: /* L367A */
    /* 367A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L367B:
    /* 367B  pop     ax */
    AX = pop16();
L367C:
    /* 367C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L367D:
    /* 367D  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L367E:
    /* 367E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L367F: /* L367F */
    /* 367F  rcr     cx,1 */
    CX = rcr16(CX, 1);
L3681:
    /* 3681  sar     dx,1 */
    DX = (uint16_t)((int16_t)DX >> 1);
L3683:
    /* 3683  jmp     L3664 */
    goto L3664;
}
