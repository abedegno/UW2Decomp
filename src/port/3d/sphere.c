/* sphere.c: replaces src/3d/SPHERE.ASM (seg004_0849_260, 0260..05FB of its
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

uint32_t asm_mod_SPHERE(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x0260: goto L0260;
    case 0x0264: goto L0264;
    case 0x0266: goto L0266;
    case 0x0268: goto L0268;
    case 0x026A: goto L026A;
    case 0x026C: goto L026C;
    case 0x026E: goto L026E;
    case 0x0270: goto L0270;
    case 0x0272: goto L0272;
    case 0x0274: goto L0274;
    case 0x0276: goto L0276;
    case 0x027A: goto L027A;
    case 0x027C: goto L027C;
    case 0x027E: goto L027E;
    case 0x0282: goto L0282;
    case 0x0284: goto L0284;
    case 0x0286: goto L0286;
    case 0x028A: goto L028A;
    case 0x028C: goto L028C;
    case 0x0290: goto L0290;
    case 0x0292: goto L0292;
    case 0x0294: goto L0294;
    case 0x0296: goto L0296;
    case 0x0299: goto L0299;
    case 0x029C: goto L029C;
    case 0x029E: goto L029E;
    case 0x02A2: goto L02A2;
    case 0x02A4: goto L02A4;
    case 0x02A6: goto L02A6;
    case 0x02AA: goto L02AA;
    case 0x02AC: goto L02AC;
    case 0x02AE: goto L02AE;
    case 0x02B2: goto L02B2;
    case 0x02B4: goto L02B4;
    case 0x02B6: goto L02B6;
    case 0x02BA: goto L02BA;
    case 0x02BD: goto L02BD;
    case 0x02BF: goto L02BF;
    case 0x02C1: goto L02C1;
    case 0x02C3: goto L02C3;
    case 0x02C5: goto L02C5;
    case 0x02C8: goto L02C8;
    case 0x02CB: goto L02CB;
    case 0x02CD: goto L02CD;
    case 0x02CF: goto L02CF;
    case 0x02D2: goto L02D2;
    case 0x02D6: goto L02D6;
    case 0x02D8: goto L02D8;
    case 0x02DA: goto L02DA;
    case 0x02DC: goto L02DC;
    case 0x02DF: goto L02DF;
    case 0x02E2: goto L02E2;
    case 0x02E4: goto L02E4;
    case 0x02E6: goto L02E6;
    case 0x02E9: goto L02E9;
    case 0x02EB: goto L02EB;
    case 0x02EF: goto L02EF;
    case 0x02F1: goto L02F1;
    case 0x02F3: goto L02F3;
    case 0x02F7: goto L02F7;
    case 0x02F9: goto L02F9;
    case 0x02FB: goto L02FB;
    case 0x02FF: goto L02FF;
    case 0x0301: goto L0301;
    case 0x0303: goto L0303;
    case 0x0307: goto L0307;
    case 0x030A: goto L030A;
    case 0x030C: goto L030C;
    case 0x030E: goto L030E;
    case 0x0310: goto L0310;
    case 0x0312: goto L0312;
    case 0x0315: goto L0315;
    case 0x0318: goto L0318;
    case 0x031A: goto L031A;
    case 0x031C: goto L031C;
    case 0x031F: goto L031F;
    case 0x0323: goto L0323;
    case 0x0325: goto L0325;
    case 0x0327: goto L0327;
    case 0x0329: goto L0329;
    case 0x032C: goto L032C;
    case 0x032F: goto L032F;
    case 0x0331: goto L0331;
    case 0x0333: goto L0333;
    case 0x0336: goto L0336;
    case 0x0339: goto L0339;
    case 0x033B: goto L033B;
    case 0x033D: goto L033D;
    case 0x033F: goto L033F;
    case 0x0341: goto L0341;
    case 0x0345: goto L0345;
    case 0x0347: goto L0347;
    case 0x034D: goto L034D;
    case 0x034F: goto L034F;
    case 0x0353: goto L0353;
    case 0x0355: goto L0355;
    case 0x0357: goto L0357;
    case 0x0359: goto L0359;
    case 0x035B: goto L035B;
    case 0x035D: goto L035D;
    case 0x035F: goto L035F;
    case 0x0362: goto L0362;
    case 0x0364: goto L0364;
    case 0x0366: goto L0366;
    case 0x0368: goto L0368;
    case 0x036A: goto L036A;
    case 0x036B: goto L036B;
    case 0x036E: goto L036E;
    case 0x036F: goto L036F;
    case 0x0371: goto L0371;
    case 0x0375: goto L0375;
    case 0x0377: goto L0377;
    case 0x0379: goto L0379;
    case 0x037D: goto L037D;
    case 0x037F: goto L037F;
    case 0x0381: goto L0381;
    case 0x0385: goto L0385;
    case 0x0387: goto L0387;
    case 0x0389: goto L0389;
    case 0x038D: goto L038D;
    case 0x0390: goto L0390;
    case 0x0392: goto L0392;
    case 0x0394: goto L0394;
    case 0x0396: goto L0396;
    case 0x0398: goto L0398;
    case 0x039C: goto L039C;
    case 0x039E: goto L039E;
    case 0x03A0: goto L03A0;
    case 0x03A2: goto L03A2;
    case 0x03A4: goto L03A4;
    case 0x03A8: goto L03A8;
    case 0x03AA: goto L03AA;
    case 0x03AC: goto L03AC;
    case 0x03B0: goto L03B0;
    case 0x03B2: goto L03B2;
    case 0x03B4: goto L03B4;
    case 0x03B8: goto L03B8;
    case 0x03BA: goto L03BA;
    case 0x03BC: goto L03BC;
    case 0x03C0: goto L03C0;
    case 0x03C3: goto L03C3;
    case 0x03C5: goto L03C5;
    case 0x03C7: goto L03C7;
    case 0x03C9: goto L03C9;
    case 0x03CB: goto L03CB;
    case 0x03CF: goto L03CF;
    case 0x03D1: goto L03D1;
    case 0x03D3: goto L03D3;
    case 0x03D5: goto L03D5;
    case 0x03D8: goto L03D8;
    case 0x03DA: goto L03DA;
    case 0x03DC: goto L03DC;
    case 0x03DE: goto L03DE;
    case 0x03E0: goto L03E0;
    case 0x03E4: goto L03E4;
    case 0x03E6: goto L03E6;
    case 0x03EC: goto L03EC;
    case 0x03EE: goto L03EE;
    case 0x03F2: goto L03F2;
    case 0x03F4: goto L03F4;
    case 0x03F6: goto L03F6;
    case 0x03F8: goto L03F8;
    case 0x03FA: goto L03FA;
    case 0x03FC: goto L03FC;
    case 0x03FE: goto L03FE;
    case 0x0401: goto L0401;
    case 0x0403: goto L0403;
    case 0x0405: goto L0405;
    case 0x0407: goto L0407;
    case 0x0409: goto L0409;
    case 0x040A: goto L040A;
    case 0x040C: goto L040C;
    case 0x040D: goto L040D;
    case 0x040E: goto L040E;
    case 0x040F: goto L040F;
    case 0x0410: goto L0410;
    case 0x0414: goto L0414;
    case 0x0417: goto L0417;
    case 0x0418: goto L0418;
    case 0x041A: goto L041A;
    case 0x041C: goto L041C;
    case 0x041E: goto L041E;
    case 0x0420: goto L0420;
    case 0x0423: goto L0423;
    case 0x0425: goto L0425;
    case 0x0429: goto L0429;
    case 0x042C: goto L042C;
    case 0x042D: goto L042D;
    case 0x042F: goto L042F;
    case 0x0431: goto L0431;
    case 0x0433: goto L0433;
    case 0x0435: goto L0435;
    case 0x0438: goto L0438;
    case 0x043A: goto L043A;
    case 0x043C: goto L043C;
    case 0x043E: goto L043E;
    case 0x0440: goto L0440;
    case 0x0442: goto L0442;
    case 0x0443: goto L0443;
    case 0x0445: goto L0445;
    case 0x0447: goto L0447;
    case 0x0449: goto L0449;
    case 0x044B: goto L044B;
    case 0x044D: goto L044D;
    case 0x044F: goto L044F;
    case 0x0453: goto L0453;
    case 0x0456: goto L0456;
    case 0x0457: goto L0457;
    case 0x0459: goto L0459;
    case 0x045B: goto L045B;
    case 0x045D: goto L045D;
    case 0x045F: goto L045F;
    case 0x0462: goto L0462;
    case 0x0464: goto L0464;
    case 0x0466: goto L0466;
    case 0x0468: goto L0468;
    case 0x046A: goto L046A;
    case 0x046C: goto L046C;
    case 0x046E: goto L046E;
    case 0x0470: goto L0470;
    case 0x0472: goto L0472;
    case 0x0474: goto L0474;
    case 0x0476: goto L0476;
    case 0x0478: goto L0478;
    case 0x047A: goto L047A;
    case 0x047C: goto L047C;
    case 0x047E: goto L047E;
    case 0x0480: goto L0480;
    case 0x0482: goto L0482;
    case 0x0484: goto L0484;
    case 0x0486: goto L0486;
    case 0x0488: goto L0488;
    case 0x048A: goto L048A;
    case 0x048C: goto L048C;
    case 0x048E: goto L048E;
    case 0x0490: goto L0490;
    case 0x0492: goto L0492;
    case 0x0494: goto L0494;
    case 0x0496: goto L0496;
    case 0x0498: goto L0498;
    case 0x049A: goto L049A;
    case 0x049B: goto L049B;
    case 0x049C: goto L049C;
    case 0x04A2: goto L04A2;
    case 0x04A3: goto L04A3;
    case 0x04A5: goto L04A5;
    case 0x04A7: goto L04A7;
    case 0x04A8: goto L04A8;
    case 0x04AB: goto L04AB;
    case 0x04AD: goto L04AD;
    case 0x04AE: goto L04AE;
    case 0x04B0: goto L04B0;
    case 0x04B1: goto L04B1;
    case 0x04B5: goto L04B5;
    case 0x04B8: goto L04B8;
    case 0x04B9: goto L04B9;
    case 0x04BA: goto L04BA;
    case 0x04BB: goto L04BB;
    case 0x04BF: goto L04BF;
    case 0x04C3: goto L04C3;
    case 0x04C5: goto L04C5;
    case 0x04C7: goto L04C7;
    case 0x04C9: goto L04C9;
    case 0x04CB: goto L04CB;
    case 0x04CD: goto L04CD;
    case 0x04CF: goto L04CF;
    case 0x04D1: goto L04D1;
    case 0x04D3: goto L04D3;
    case 0x04D5: goto L04D5;
    case 0x04D7: goto L04D7;
    case 0x04D9: goto L04D9;
    case 0x04DA: goto L04DA;
    case 0x04DC: goto L04DC;
    case 0x04DE: goto L04DE;
    case 0x04E1: goto L04E1;
    case 0x04E3: goto L04E3;
    case 0x04E6: goto L04E6;
    case 0x04E7: goto L04E7;
    case 0x04E9: goto L04E9;
    case 0x04EB: goto L04EB;
    case 0x04ED: goto L04ED;
    case 0x04EF: goto L04EF;
    case 0x04F2: goto L04F2;
    case 0x04F3: goto L04F3;
    case 0x04F5: goto L04F5;
    case 0x04F6: goto L04F6;
    case 0x04FA: goto L04FA;
    case 0x04FD: goto L04FD;
    case 0x04FE: goto L04FE;
    case 0x04FF: goto L04FF;
    case 0x0500: goto L0500;
    case 0x0504: goto L0504;
    case 0x0508: goto L0508;
    case 0x050C: goto L050C;
    case 0x050E: goto L050E;
    case 0x0510: goto L0510;
    case 0x0512: goto L0512;
    case 0x0514: goto L0514;
    case 0x0516: goto L0516;
    case 0x0518: goto L0518;
    case 0x051A: goto L051A;
    case 0x051C: goto L051C;
    case 0x051E: goto L051E;
    case 0x0520: goto L0520;
    case 0x0522: goto L0522;
    case 0x0524: goto L0524;
    case 0x0525: goto L0525;
    case 0x0527: goto L0527;
    case 0x0529: goto L0529;
    case 0x052C: goto L052C;
    case 0x052E: goto L052E;
    case 0x0531: goto L0531;
    case 0x0532: goto L0532;
    case 0x0534: goto L0534;
    case 0x0536: goto L0536;
    case 0x0538: goto L0538;
    case 0x053A: goto L053A;
    case 0x053D: goto L053D;
    case 0x053F: goto L053F;
    case 0x0540: goto L0540;
    case 0x0542: goto L0542;
    case 0x0543: goto L0543;
    case 0x0547: goto L0547;
    case 0x054A: goto L054A;
    case 0x054B: goto L054B;
    case 0x054C: goto L054C;
    case 0x054D: goto L054D;
    case 0x0551: goto L0551;
    case 0x0555: goto L0555;
    case 0x0559: goto L0559;
    case 0x055B: goto L055B;
    case 0x055D: goto L055D;
    case 0x055F: goto L055F;
    case 0x0561: goto L0561;
    case 0x0563: goto L0563;
    case 0x0565: goto L0565;
    case 0x0567: goto L0567;
    case 0x0569: goto L0569;
    case 0x056B: goto L056B;
    case 0x056D: goto L056D;
    case 0x056F: goto L056F;
    case 0x0571: goto L0571;
    case 0x0572: goto L0572;
    case 0x0574: goto L0574;
    case 0x0576: goto L0576;
    case 0x0578: goto L0578;
    case 0x057B: goto L057B;
    case 0x057C: goto L057C;
    case 0x057E: goto L057E;
    case 0x0580: goto L0580;
    case 0x0582: goto L0582;
    case 0x0584: goto L0584;
    case 0x0586: goto L0586;
    case 0x0587: goto L0587;
    case 0x0588: goto L0588;
    case 0x0589: goto L0589;
    case 0x058A: goto L058A;
    case 0x058C: goto L058C;
    case 0x0590: goto L0590;
    case 0x0592: goto L0592;
    case 0x0598: goto L0598;
    case 0x059A: goto L059A;
    case 0x059D: goto L059D;
    case 0x059F: goto L059F;
    case 0x05A1: goto L05A1;
    case 0x05A3: goto L05A3;
    case 0x05A4: goto L05A4;
    case 0x05A6: goto L05A6;
    case 0x05A7: goto L05A7;
    case 0x05A9: goto L05A9;
    case 0x05AC: goto L05AC;
    case 0x05AE: goto L05AE;
    case 0x05B0: goto L05B0;
    case 0x05B3: goto L05B3;
    case 0x05B5: goto L05B5;
    case 0x05B7: goto L05B7;
    case 0x05B9: goto L05B9;
    case 0x05BA: goto L05BA;
    case 0x05BC: goto L05BC;
    case 0x05BD: goto L05BD;
    case 0x05BF: goto L05BF;
    case 0x05C1: goto L05C1;
    case 0x05C5: goto L05C5;
    case 0x05C9: goto L05C9;
    case 0x05CC: goto L05CC;
    case 0x05CF: goto L05CF;
    case 0x05D0: goto L05D0;
    case 0x05D2: goto L05D2;
    case 0x05D5: goto L05D5;
    case 0x05D8: goto L05D8;
    case 0x05D9: goto L05D9;
    case 0x05DC: goto L05DC;
    case 0x05DD: goto L05DD;
    case 0x05DE: goto L05DE;
    case 0x05DF: goto L05DF;
    case 0x05E3: goto L05E3;
    case 0x05E9: goto L05E9;
    case 0x05EB: goto L05EB;
    case 0x05EC: goto L05EC;
    case 0x05ED: goto L05ED;
    case 0x05F1: goto L05F1;
    case 0x05F2: goto L05F2;
    case 0x05F3: goto L05F3;
    case 0x05F5: goto L05F5;
    case 0x05F6: goto L05F6;
    case 0x05F7: goto L05F7;
    default: asm_bad_entry("SPHERE.ASM", entry);
    }

    /* seg004_0849_260  (+260)
       Bounding-sphere test against the view frustum. In: BX, AX, BP = the object origin words
       (25E6h, 25E8h, 25EAh: negated eye-relative x, y, z), SI = radius, CL = shift. Shifts all
       four by CL and negates the position, takes its depth along the matrix's z column (kept at
       ds:0Eh, the shift at ds:10h), and tests it against the near plane and the four side planes
       (the x and y columns, with the radius scaled by 24E8h and 24EAh). Out: CF set when the
       sphere is wholly outside one plane. Otherwise CF clear, SI = -1 when the sphere is wholly
       inside (the first path) or 0 when it crosses a plane (the second, from L036F), and AX =
       the depth shifted back by the original shift (scaled by 8000h / -24E6h when 24E6h is
       negative, 7FFFh if that overflows) and halved; 0 when the centre is behind the eye. Called
       only by do_obj. */
L0260: /* _sphere_check */
    /* 0260  mov     word ptr ds:[10h],cx */
    ww(pDS, 0x10, CX);
L0264:
    /* 0264  shl     si,cl */
    SI = shl16(SI, CL);
L0266:
    /* 0266  shl     bx,cl */
    BX = (uint16_t)(BX << (CL & 31));
L0268:
    /* 0268  neg     bx */
    BX = neg16(BX);
L026A:
    /* 026A  shl     bp,cl */
    BP = (uint16_t)(BP << (CL & 31));
L026C:
    /* 026C  neg     bp */
    BP = neg16(BP);
L026E:
    /* 026E  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L0270:
    /* 0270  neg     ax */
    AX = (uint16_t)-AX;
L0272:
    /* 0272  mov     cx,ax */
    CX = AX;
L0274:
    /* 0274  mov     ax,bx */
    AX = BX;
L0276:
    /* 0276  imul    word ptr ds:[14B6h] */
    imul16(rw(pDS, 0x14B6));
L027A:
    /* 027A  mov     di,dx */
    DI = DX;
L027C:
    /* 027C  mov     ax,cx */
    AX = CX;
L027E:
    /* 027E  imul    word ptr ds:[14BCh] */
    imul16(rw(pDS, 0x14BC));
L0282:
    /* 0282  add     di,dx */
    DI = (uint16_t)(DI + DX);
L0284:
    /* 0284  mov     ax,bp */
    AX = BP;
L0286:
    /* 0286  imul    word ptr ds:[14C2h] */
    imul16(rw(pDS, 0x14C2));
L028A:
    /* 028A  add     di,dx */
    DI = add16(DI, DX, 0);
L028C:
    /* 028C  mov     word ptr ds:[0Eh],di */
    ww(pDS, 0xE, DI);
L0290:
    /* 0290  jns     short L029C */
    if (!SF) goto L029C;
L0292:
    /* 0292  add     di,si */
    DI = add16(DI, SI, 0);
L0294:
    /* 0294  jns     short L0299 */
    if (!SF) goto L0299;
L0296:
    /* 0296  jmp     L040D */
    goto L040D;
L0299: /* L0299 */
    /* 0299  jmp     L036F */
    goto L036F;
L029C: /* L029C */
    /* 029C  mov     ax,bx */
    AX = BX;
L029E:
    /* 029E  imul    word ptr ds:[14B2h] */
    imul16(rw(pDS, 0x14B2));
L02A2:
    /* 02A2  mov     di,dx */
    DI = DX;
L02A4:
    /* 02A4  mov     ax,cx */
    AX = CX;
L02A6:
    /* 02A6  imul    word ptr ds:[14B8h] */
    imul16(rw(pDS, 0x14B8));
L02AA:
    /* 02AA  add     di,dx */
    DI = (uint16_t)(DI + DX);
L02AC:
    /* 02AC  mov     ax,bp */
    AX = BP;
L02AE:
    /* 02AE  imul    word ptr ds:[14BEh] */
    imul16(rw(pDS, 0x14BE));
L02B2:
    /* 02B2  add     di,dx */
    DI = (uint16_t)(DI + DX);
L02B4:
    /* 02B4  mov     ax,si */
    AX = SI;
L02B6:
    /* 02B6  mul     word ptr ds:[24E8h] */
    mul16(rw(pDS, 0x24E8));
L02BA:
    /* 02BA  mov     ax,word ptr ds:[0Eh] */
    AX = rw(pDS, 0xE);
L02BD:
    /* 02BD  sub     ax,di */
    AX = sub16(AX, DI, 0);
L02BF:
    /* 02BF  jns     short L02CB */
    if (!SF) goto L02CB;
L02C1:
    /* 02C1  add     ax,dx */
    AX = add16(AX, DX, 0);
L02C3:
    /* 02C3  jns     short L02C8 */
    if (!SF) goto L02C8;
L02C5:
    /* 02C5  jmp     L040D */
    goto L040D;
L02C8: /* L02C8 */
    /* 02C8  jmp     L0398 */
    goto L0398;
L02CB: /* L02CB */
    /* 02CB  sub     ax,dx */
    AX = sub16(AX, DX, 0);
L02CD:
    /* 02CD  jns     short L02D2 */
    if (!SF) goto L02D2;
L02CF:
    /* 02CF  jmp     L0398 */
    goto L0398;
L02D2: /* L02D2 */
    /* 02D2  add     di,word ptr ds:[0Eh] */
    DI = add16(DI, rw(pDS, 0xE), 0);
L02D6:
    /* 02D6  jns     short L02E2 */
    if (!SF) goto L02E2;
L02D8:
    /* 02D8  add     di,dx */
    DI = add16(DI, DX, 0);
L02DA:
    /* 02DA  jns     short L02DF */
    if (!SF) goto L02DF;
L02DC:
    /* 02DC  jmp     L040D */
    goto L040D;
L02DF: /* L02DF */
    /* 02DF  jmp     L03A2 */
    goto L03A2;
L02E2: /* L02E2 */
    /* 02E2  sub     di,dx */
    DI = sub16(DI, DX, 0);
L02E4:
    /* 02E4  jns     short L02E9 */
    if (!SF) goto L02E9;
L02E6:
    /* 02E6  jmp     L03A2 */
    goto L03A2;
L02E9: /* L02E9 */
    /* 02E9  mov     ax,bx */
    AX = BX;
L02EB:
    /* 02EB  imul    word ptr ds:[14B4h] */
    imul16(rw(pDS, 0x14B4));
L02EF:
    /* 02EF  mov     di,dx */
    DI = DX;
L02F1:
    /* 02F1  mov     ax,cx */
    AX = CX;
L02F3:
    /* 02F3  imul    word ptr ds:[14BAh] */
    imul16(rw(pDS, 0x14BA));
L02F7:
    /* 02F7  add     di,dx */
    DI = (uint16_t)(DI + DX);
L02F9:
    /* 02F9  mov     ax,bp */
    AX = BP;
L02FB:
    /* 02FB  imul    word ptr ds:[14C0h] */
    imul16(rw(pDS, 0x14C0));
L02FF:
    /* 02FF  add     di,dx */
    DI = (uint16_t)(DI + DX);
L0301:
    /* 0301  mov     ax,si */
    AX = SI;
L0303:
    /* 0303  mul     word ptr ds:[24EAh] */
    mul16(rw(pDS, 0x24EA));
L0307:
    /* 0307  mov     ax,word ptr ds:[0Eh] */
    AX = rw(pDS, 0xE);
L030A:
    /* 030A  sub     ax,di */
    AX = sub16(AX, DI, 0);
L030C:
    /* 030C  jns     short L0318 */
    if (!SF) goto L0318;
L030E:
    /* 030E  add     ax,dx */
    AX = add16(AX, DX, 0);
L0310:
    /* 0310  jns     short L0315 */
    if (!SF) goto L0315;
L0312:
    /* 0312  jmp     L040D */
    goto L040D;
L0315: /* L0315 */
    /* 0315  jmp     L03CB */
    goto L03CB;
L0318: /* L0318 */
    /* 0318  sub     ax,dx */
    AX = sub16(AX, DX, 0);
L031A:
    /* 031A  jns     short L031F */
    if (!SF) goto L031F;
L031C:
    /* 031C  jmp     L03CB */
    goto L03CB;
L031F: /* L031F */
    /* 031F  add     di,word ptr ds:[0Eh] */
    DI = add16(DI, rw(pDS, 0xE), 0);
L0323:
    /* 0323  jns     short L032F */
    if (!SF) goto L032F;
L0325:
    /* 0325  add     di,dx */
    DI = add16(DI, DX, 0);
L0327:
    /* 0327  jns     short L032C */
    if (!SF) goto L032C;
L0329:
    /* 0329  jmp     L040D */
    goto L040D;
L032C: /* L032C */
    /* 032C  jmp     L03D5 */
    goto L03D5;
L032F: /* L032F */
    /* 032F  sub     di,dx */
    DI = sub16(DI, DX, 0);
L0331:
    /* 0331  jns     short L0336 */
    if (!SF) goto L0336;
L0333:
    /* 0333  jmp     L03D5 */
    goto L03D5;
L0336: /* L0336 */
    /* 0336  mov     ax,word ptr ds:[0Eh] */
    AX = rw(pDS, 0xE);
L0339:
    /* 0339  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L033B:
    /* 033B  jns     short L0341 */
    if (!SF) goto L0341;
L033D:
    /* 033D  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L033F:
    /* 033F  jmp     short L0368 */
    goto L0368;
L0341: /* L0341 */
    /* 0341  mov     cx,word ptr ds:[10h] */
    CX = rw(pDS, 0x10);
L0345:
    /* 0345  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L0347:
    /* 0347  test    word ptr ds:[24E6h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x24E6) & 0xFFFF));
L034D:
    /* 034D  jns     short L0368 */
    if (!SF) goto L0368;
L034F:
    /* 034F  mov     cx,word ptr ds:[24E6h] */
    CX = rw(pDS, 0x24E6);
L0353:
    /* 0353  neg     cx */
    CX = (uint16_t)-CX;
L0355:
    /* 0355  mov     dx,ax */
    DX = AX;
L0357:
    /* 0357  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L0359:
    /* 0359  sar     dx,1 */
    DX = (uint16_t)((int16_t)DX >> 1);
L035B:
    /* 035B  cmp     dx,cx */
    sub16(DX, CX, 0);
L035D:
    /* 035D  jl      short L0364 */
    if (SF != OF) goto L0364;
L035F:
    /* 035F  mov     ax,7FFFh */
    AX = 0x7FFF;
L0362:
    /* 0362  jmp     short L0368 */
    goto L0368;
L0364: /* L0364 */
    /* 0364  rcr     ax,1 */
    AX = rcr16(AX, 1);
L0366:
    /* 0366  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x065C, 0x0366, 2)) != 0) return c;
L0368: /* L0368 */
    /* 0368  shr     ax,1 */
    AX = shr16(AX, 1);
L036A:
    /* 036A  clc */
    CF = 0;
L036B:
    /* 036B  mov     si,0FFFFh */
    SI = 0xFFFF;
L036E:
    /* 036E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L036F: /* L036F */
    /* 036F  mov     ax,bx */
    AX = BX;
L0371:
    /* 0371  imul    word ptr ds:[14B2h] */
    imul16(rw(pDS, 0x14B2));
L0375:
    /* 0375  mov     di,dx */
    DI = DX;
L0377:
    /* 0377  mov     ax,cx */
    AX = CX;
L0379:
    /* 0379  imul    word ptr ds:[14B8h] */
    imul16(rw(pDS, 0x14B8));
L037D:
    /* 037D  add     di,dx */
    DI = (uint16_t)(DI + DX);
L037F:
    /* 037F  mov     ax,bp */
    AX = BP;
L0381:
    /* 0381  imul    word ptr ds:[14BEh] */
    imul16(rw(pDS, 0x14BE));
L0385:
    /* 0385  add     di,dx */
    DI = (uint16_t)(DI + DX);
L0387:
    /* 0387  mov     ax,si */
    AX = SI;
L0389:
    /* 0389  mul     word ptr ds:[24E8h] */
    mul16(rw(pDS, 0x24E8));
L038D:
    /* 038D  mov     ax,word ptr ds:[0Eh] */
    AX = rw(pDS, 0xE);
L0390:
    /* 0390  sub     ax,di */
    AX = sub16(AX, DI, 0);
L0392:
    /* 0392  jns     short L0398 */
    if (!SF) goto L0398;
L0394:
    /* 0394  add     ax,dx */
    AX = add16(AX, DX, 0);
L0396:
    /* 0396  js      short L040D */
    if (SF) goto L040D;
L0398: /* L0398 */
    /* 0398  add     di,word ptr ds:[0Eh] */
    DI = add16(DI, rw(pDS, 0xE), 0);
L039C:
    /* 039C  jns     short L03A2 */
    if (!SF) goto L03A2;
L039E:
    /* 039E  add     di,dx */
    DI = add16(DI, DX, 0);
L03A0:
    /* 03A0  js      short L040D */
    if (SF) goto L040D;
L03A2: /* L03A2 */
    /* 03A2  mov     ax,bx */
    AX = BX;
L03A4:
    /* 03A4  imul    word ptr ds:[14B4h] */
    imul16(rw(pDS, 0x14B4));
L03A8:
    /* 03A8  mov     di,dx */
    DI = DX;
L03AA:
    /* 03AA  mov     ax,cx */
    AX = CX;
L03AC:
    /* 03AC  imul    word ptr ds:[14BAh] */
    imul16(rw(pDS, 0x14BA));
L03B0:
    /* 03B0  add     di,dx */
    DI = (uint16_t)(DI + DX);
L03B2:
    /* 03B2  mov     ax,bp */
    AX = BP;
L03B4:
    /* 03B4  imul    word ptr ds:[14C0h] */
    imul16(rw(pDS, 0x14C0));
L03B8:
    /* 03B8  add     di,dx */
    DI = (uint16_t)(DI + DX);
L03BA:
    /* 03BA  mov     ax,si */
    AX = SI;
L03BC:
    /* 03BC  mul     word ptr ds:[24EAh] */
    mul16(rw(pDS, 0x24EA));
L03C0:
    /* 03C0  mov     ax,word ptr ds:[0Eh] */
    AX = rw(pDS, 0xE);
L03C3:
    /* 03C3  sub     ax,di */
    AX = sub16(AX, DI, 0);
L03C5:
    /* 03C5  jns     short L03CB */
    if (!SF) goto L03CB;
L03C7:
    /* 03C7  add     ax,dx */
    AX = add16(AX, DX, 0);
L03C9:
    /* 03C9  js      short L040D */
    if (SF) goto L040D;
L03CB: /* L03CB */
    /* 03CB  add     di,word ptr ds:[0Eh] */
    DI = add16(DI, rw(pDS, 0xE), 0);
L03CF:
    /* 03CF  jns     short L03D5 */
    if (!SF) goto L03D5;
L03D1:
    /* 03D1  add     di,dx */
    DI = add16(DI, DX, 0);
L03D3:
    /* 03D3  js      short L040D */
    if (SF) goto L040D;
L03D5: /* L03D5 */
    /* 03D5  mov     ax,word ptr ds:[0Eh] */
    AX = rw(pDS, 0xE);
L03D8:
    /* 03D8  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L03DA:
    /* 03DA  jns     short L03E0 */
    if (!SF) goto L03E0;
L03DC:
    /* 03DC  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L03DE:
    /* 03DE  jmp     short L0407 */
    goto L0407;
L03E0: /* L03E0 */
    /* 03E0  mov     cx,word ptr ds:[10h] */
    CX = rw(pDS, 0x10);
L03E4:
    /* 03E4  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L03E6:
    /* 03E6  test    word ptr ds:[24E6h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x24E6) & 0xFFFF));
L03EC:
    /* 03EC  jns     short L0407 */
    if (!SF) goto L0407;
L03EE:
    /* 03EE  mov     cx,word ptr ds:[24E6h] */
    CX = rw(pDS, 0x24E6);
L03F2:
    /* 03F2  neg     cx */
    CX = (uint16_t)-CX;
L03F4:
    /* 03F4  mov     dx,ax */
    DX = AX;
L03F6:
    /* 03F6  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L03F8:
    /* 03F8  sar     dx,1 */
    DX = (uint16_t)((int16_t)DX >> 1);
L03FA:
    /* 03FA  cmp     dx,cx */
    sub16(DX, CX, 0);
L03FC:
    /* 03FC  jl      short L0403 */
    if (SF != OF) goto L0403;
L03FE:
    /* 03FE  mov     ax,7FFFh */
    AX = 0x7FFF;
L0401:
    /* 0401  jmp     short L0407 */
    goto L0407;
L0403: /* L0403 */
    /* 0403  rcr     ax,1 */
    AX = rcr16(AX, 1);
L0405:
    /* 0405  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x065C, 0x0405, 2)) != 0) return c;
L0407: /* L0407 */
    /* 0407  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L0409:
    /* 0409  clc */
    CF = 0;
L040A:
    /* 040A  xor     si,si */
    SI = logic16((uint16_t)(SI ^ SI));
L040C:
    /* 040C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L040D: /* L040D */
    /* 040D  stc */
    CF = 1;
L040E:
    /* 040E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_40F  (+40F)
       Distance estimate from the three 32-bit eye-relative coordinates do_obj leaves at ds:16h,
       1Ah and 1Eh: takes absolute values and combines the largest with fractions of the others
       (halvings and quarterings, an octagonal approximation). Out: BP:BX. Nothing in the EXE calls
       it. In the y term the absolute value's correction is added to CX, not SI (`add cx,dx`
       after `xor si,dx`), which looks like a slip; it would matter only if something called it. */
L040F: /* _get_dist */
    /* 040F  push    si */
    push16(SI);
L0410:
    /* 0410  mov     bx,word ptr ds:[16h] */
    BX = rw(pDS, 0x16);
L0414:
    /* 0414  mov     ax,word ptr ds:[18h] */
    AX = rw(pDS, 0x18);
L0417:
    /* 0417  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0418:
    /* 0418  xor     bx,dx */
    BX = (uint16_t)(BX ^ DX);
L041A:
    /* 041A  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L041C:
    /* 041C  neg     dx */
    DX = (uint16_t)-DX;
L041E:
    /* 041E  add     bx,dx */
    BX = add16(BX, DX, 0);
L0420:
    /* 0420  adc     ax,0 */
    AX = (uint16_t)(AX + 0x0 + CF);
L0423:
    /* 0423  mov     bp,ax */
    BP = AX;
L0425:
    /* 0425  mov     cx,word ptr ds:[1Eh] */
    CX = rw(pDS, 0x1E);
L0429:
    /* 0429  mov     ax,word ptr ds:[20h] */
    AX = rw(pDS, 0x20);
L042C:
    /* 042C  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L042D:
    /* 042D  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L042F:
    /* 042F  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L0431:
    /* 0431  neg     dx */
    DX = (uint16_t)-DX;
L0433:
    /* 0433  add     cx,dx */
    CX = add16(CX, DX, 0);
L0435:
    /* 0435  adc     ax,0 */
    AX = (uint16_t)(AX + 0x0 + CF);
L0438:
    /* 0438  cmp     bp,ax */
    sub16(BP, AX, 0);
L043A:
    /* 043A  jl      short L0445 */
    if (SF != OF) goto L0445;
L043C:
    /* 043C  jg      short L0442 */
    if (!ZF && SF == OF) goto L0442;
L043E:
    /* 043E  cmp     bx,cx */
    sub16(BX, CX, 0);
L0440:
    /* 0440  jl      short L0445 */
    if (SF != OF) goto L0445;
L0442: /* L0442 */
    /* 0442  xchg    bp,ax */
    { uint16_t t_ = BP;
    BP = AX;
    AX = t_; }
L0443:
    /* 0443  xchg    bx,cx */
    { uint16_t t_ = CX;
    CX = BX;
    BX = t_; }
L0445: /* L0445 */
    /* 0445  sar     bp,1 */
    BP = sar16(BP, 1);
L0447:
    /* 0447  rcr     bx,1 */
    BX = rcr16(BX, 1);
L0449:
    /* 0449  sar     bp,1 */
    BP = sar16(BP, 1);
L044B:
    /* 044B  rcr     bx,1 */
    BX = rcr16(BX, 1);
L044D:
    /* 044D  mov     di,ax */
    DI = AX;
L044F:
    /* 044F  mov     si,word ptr ds:[1Ah] */
    SI = rw(pDS, 0x1A);
L0453:
    /* 0453  mov     ax,word ptr ds:[1Ch] */
    AX = rw(pDS, 0x1C);
L0456:
    /* 0456  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0457:
    /* 0457  xor     si,dx */
    SI = (uint16_t)(SI ^ DX);
L0459:
    /* 0459  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L045B:
    /* 045B  neg     dx */
    DX = (uint16_t)-DX;
L045D:
    /* 045D  add     cx,dx */
    CX = add16(CX, DX, 0);
L045F:
    /* 045F  adc     ax,0 */
    AX = (uint16_t)(AX + 0x0 + CF);
L0462:
    /* 0462  cmp     ax,di */
    sub16(AX, DI, 0);
L0464:
    /* 0464  jl      short L047A */
    if (SF != OF) goto L047A;
L0466:
    /* 0466  jg      short L046C */
    if (!ZF && SF == OF) goto L046C;
L0468:
    /* 0468  cmp     si,cx */
    sub16(SI, CX, 0);
L046A:
    /* 046A  jl      short L047A */
    if (SF != OF) goto L047A;
L046C: /* L046C */
    /* 046C  add     bx,cx */
    BX = add16(BX, CX, 0);
L046E:
    /* 046E  adc     bp,di */
    BP = (uint16_t)(BP + DI + CF);
L0470:
    /* 0470  sar     bp,1 */
    BP = sar16(BP, 1);
L0472:
    /* 0472  rcr     bx,1 */
    BX = rcr16(BX, 1);
L0474:
    /* 0474  sar     bp,1 */
    BP = sar16(BP, 1);
L0476:
    /* 0476  rcr     bx,1 */
    BX = rcr16(BX, 1);
L0478:
    /* 0478  jmp     short L0496 */
    goto L0496;
L047A: /* L047A */
    /* 047A  sar     ax,1 */
    AX = sar16(AX, 1);
L047C:
    /* 047C  rcr     si,1 */
    SI = rcr16(SI, 1);
L047E:
    /* 047E  cmp     ax,bp */
    sub16(AX, BP, 0);
L0480:
    /* 0480  jl      short L048E */
    if (SF != OF) goto L048E;
L0482:
    /* 0482  jg      short L0488 */
    if (!ZF && SF == OF) goto L0488;
L0484:
    /* 0484  cmp     si,bx */
    sub16(SI, BX, 0);
L0486:
    /* 0486  jl      short L048E */
    if (SF != OF) goto L048E;
L0488: /* L0488 */
    /* 0488  sar     bp,1 */
    BP = sar16(BP, 1);
L048A:
    /* 048A  rcr     bx,1 */
    BX = rcr16(BX, 1);
L048C:
    /* 048C  jmp     short L0492 */
    goto L0492;
L048E: /* L048E */
    /* 048E  sar     ax,1 */
    AX = sar16(AX, 1);
L0490:
    /* 0490  rcr     si,1 */
    SI = rcr16(SI, 1);
L0492: /* L0492 */
    /* 0492  add     si,cx */
    SI = add16(SI, CX, 0);
L0494:
    /* 0494  adc     ax,di */
    AX = (uint16_t)(AX + DI + CF);
L0496: /* L0496 */
    /* 0496  add     bx,si */
    BX = add16(BX, SI, 0);
L0498:
    /* 0498  adc     bp,ax */
    BP = add16(BP, AX, CF);
L049A:
    /* 049A  pop     si */
    SI = pop16();
L049B:
    /* 049B  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_49C  (+49C)
       Opcode 38h, the object header. Operands: skip (the offset from after the operand to the
       end of the object's program), shift, then three (extent, 32-bit coordinate) pairs for x,
       y and z, then the radius, then the object's program. Clears 25E2h; computes each
       coordinate relative to the eye (247Ah..2484h) and shifts it by `shift` (right when
       positive, left when negative); if any relative coordinate does not fit in 16 bits the
       object is skipped with 25E2h = -1 (L05E3). Stores the negated values as the object origin
       (25E6h..25EAh). The extents are added to the coordinates' magnitudes and ORed together,
       and the bit-length table at ds:22h turns that into 25E0h, 15 minus its bit length: the
       left shift the point loaders apply, so a small or near object keeps the most precision.
       A first cull at modify_axis2 (FM name; the add or sub and the axis address are patched by
       seg004_0849_35FF to the axis the view looks most along) skips the object if its radius
       plus that coordinate is negative, behind the eye. Then sphere_check; if it passes, AX
       goes to 25FAh, self_modify loads the origin into the loaders, set_accept gets sphere_check's
       SI, and the program after the radius is run. Every rejection dispatches the opcode after
       the object (SI = the skip target). */
L049C: /* do_obj */
    /* 049C  mov     word ptr ds:[25E2h],0 */
    ww(pDS, 0x25E2, 0x0);
L04A2:
    /* 04A2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L04A3:
    /* 04A3  mov     di,ax */
    DI = AX;
L04A5:
    /* 04A5  add     di,si */
    DI = (uint16_t)(DI + SI);
L04A7:
    /* 04A7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L04A8:
    /* 04A8  mov     word ptr ds:[24C8h],ax */
    ww(pDS, 0x24C8, AX);
L04AB:
    /* 04AB  mov     cx,ax */
    CX = AX;
L04AD:
    /* 04AD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L04AE:
    /* 04AE  mov     bp,ax */
    BP = AX;
L04B0:
    /* 04B0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L04B1:
    /* 04B1  sub     ax,word ptr ds:[247Ah] */
    AX = sub16(AX, rw(pDS, 0x247A), 0);
L04B5:
    /* 04B5  mov     word ptr ds:[16h],ax */
    ww(pDS, 0x16, AX);
L04B8:
    /* 04B8  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L04B9:
    /* 04B9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L04BA:
    /* 04BA  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L04BB:
    /* 04BB  sbb     bx,word ptr ds:[247Ch] */
    BX = (uint16_t)(BX - rw(pDS, 0x247C) - CF);
L04BF:
    /* 04BF  mov     word ptr ds:[18h],bx */
    ww(pDS, 0x18, BX);
L04C3:
    /* 04C3  jcxz    L04D9 */
    if (!CX) goto L04D9;
L04C5:
    /* 04C5  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L04C7:
    /* 04C7  jns     short L04D3 */
    if (!SF) goto L04D3;
L04C9:
    /* 04C9  neg     cx */
    CX = (uint16_t)-CX;
L04CB: /* L04CB */
    /* 04CB  shl     ax,1 */
    AX = shl16(AX, 1);
L04CD:
    /* 04CD  rcl     bx,1 */
    BX = rcl16(BX, 1);
L04CF:
    /* 04CF  loop    L04CB */
    if (--CX) goto L04CB;
L04D1:
    /* 04D1  jmp     short L04D9 */
    goto L04D9;
L04D3: /* L04D3 */
    /* 04D3  sar     bx,1 */
    BX = sar16(BX, 1);
L04D5:
    /* 04D5  rcr     ax,1 */
    AX = rcr16(AX, 1);
L04D7:
    /* 04D7  loop    L04D3 */
    if (--CX) goto L04D3;
L04D9: /* L04D9 */
    /* 04D9  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L04DA:
    /* 04DA  cmp     bx,dx */
    sub16(BX, DX, 0);
L04DC:
    /* 04DC  je      short L04E1 */
    if (ZF) goto L04E1;
L04DE:
    /* 04DE  jmp     L05E3 */
    goto L05E3;
L04E1: /* L04E1 */
    /* 04E1  neg     ax */
    AX = (uint16_t)-AX;
L04E3:
    /* 04E3  mov     word ptr ds:[25E6h],ax */
    ww(pDS, 0x25E6, AX);
L04E6:
    /* 04E6  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L04E7:
    /* 04E7  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L04E9:
    /* 04E9  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L04EB:
    /* 04EB  add     bp,ax */
    BP = add16(BP, AX, 0);
L04ED:
    /* 04ED  jno     short L04F2 */
    if (!OF) goto L04F2;
L04EF:
    /* 04EF  jmp     L05E3 */
    goto L05E3;
L04F2: /* L04F2 */
    /* 04F2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L04F3:
    /* 04F3  mov     dx,ax */
    DX = AX;
L04F5:
    /* 04F5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L04F6:
    /* 04F6  sub     ax,word ptr ds:[247Eh] */
    AX = sub16(AX, rw(pDS, 0x247E), 0);
L04FA:
    /* 04FA  mov     word ptr ds:[1Ah],ax */
    ww(pDS, 0x1A, AX);
L04FD:
    /* 04FD  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L04FE:
    /* 04FE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L04FF:
    /* 04FF  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L0500:
    /* 0500  sbb     bx,word ptr ds:[2480h] */
    BX = (uint16_t)(BX - rw(pDS, 0x2480) - CF);
L0504:
    /* 0504  mov     word ptr ds:[1Ch],bx */
    ww(pDS, 0x1C, BX);
L0508:
    /* 0508  mov     cx,word ptr ds:[24C8h] */
    CX = rw(pDS, 0x24C8);
L050C:
    /* 050C  jcxz    L0522 */
    if (!CX) goto L0522;
L050E:
    /* 050E  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L0510:
    /* 0510  jns     short L051C */
    if (!SF) goto L051C;
L0512:
    /* 0512  neg     cx */
    CX = (uint16_t)-CX;
L0514: /* L0514 */
    /* 0514  shl     ax,1 */
    AX = shl16(AX, 1);
L0516:
    /* 0516  rcl     bx,1 */
    BX = rcl16(BX, 1);
L0518:
    /* 0518  loop    L0514 */
    if (--CX) goto L0514;
L051A:
    /* 051A  jmp     short L0522 */
    goto L0522;
L051C: /* L051C */
    /* 051C  sar     bx,1 */
    BX = sar16(BX, 1);
L051E:
    /* 051E  rcr     ax,1 */
    AX = rcr16(AX, 1);
L0520:
    /* 0520  loop    L051C */
    if (--CX) goto L051C;
L0522: /* L0522 */
    /* 0522  mov     cx,dx */
    CX = DX;
L0524:
    /* 0524  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0525:
    /* 0525  cmp     bx,dx */
    sub16(BX, DX, 0);
L0527:
    /* 0527  je      short L052C */
    if (ZF) goto L052C;
L0529:
    /* 0529  jmp     L05E3 */
    goto L05E3;
L052C: /* L052C */
    /* 052C  neg     ax */
    AX = (uint16_t)-AX;
L052E:
    /* 052E  mov     word ptr ds:[25E8h],ax */
    ww(pDS, 0x25E8, AX);
L0531:
    /* 0531  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0532:
    /* 0532  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L0534:
    /* 0534  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L0536:
    /* 0536  add     ax,cx */
    AX = add16(AX, CX, 0);
L0538:
    /* 0538  jno     short L053D */
    if (!OF) goto L053D;
L053A:
    /* 053A  jmp     L05E3 */
    goto L05E3;
L053D: /* L053D */
    /* 053D  or      bp,ax */
    BP = (uint16_t)(BP | AX);
L053F:
    /* 053F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0540:
    /* 0540  mov     dx,ax */
    DX = AX;
L0542:
    /* 0542  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0543:
    /* 0543  sub     ax,word ptr ds:[2482h] */
    AX = sub16(AX, rw(pDS, 0x2482), 0);
L0547:
    /* 0547  mov     word ptr ds:[1Eh],ax */
    ww(pDS, 0x1E, AX);
L054A:
    /* 054A  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L054B:
    /* 054B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L054C:
    /* 054C  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L054D:
    /* 054D  sbb     bx,word ptr ds:[2484h] */
    BX = (uint16_t)(BX - rw(pDS, 0x2484) - CF);
L0551:
    /* 0551  mov     word ptr ds:[20h],bx */
    ww(pDS, 0x20, BX);
L0555:
    /* 0555  mov     cx,word ptr ds:[24C8h] */
    CX = rw(pDS, 0x24C8);
L0559:
    /* 0559  jcxz    L056F */
    if (!CX) goto L056F;
L055B:
    /* 055B  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L055D:
    /* 055D  jns     short L0569 */
    if (!SF) goto L0569;
L055F:
    /* 055F  neg     cx */
    CX = (uint16_t)-CX;
L0561: /* L0561 */
    /* 0561  shl     ax,1 */
    AX = shl16(AX, 1);
L0563:
    /* 0563  rcl     bx,1 */
    BX = rcl16(BX, 1);
L0565:
    /* 0565  loop    L0561 */
    if (--CX) goto L0561;
L0567:
    /* 0567  jmp     short L056F */
    goto L056F;
L0569: /* L0569 */
    /* 0569  sar     bx,1 */
    BX = sar16(BX, 1);
L056B:
    /* 056B  rcr     ax,1 */
    AX = rcr16(AX, 1);
L056D:
    /* 056D  loop    L0569 */
    if (--CX) goto L0569;
L056F: /* L056F */
    /* 056F  mov     cx,dx */
    CX = DX;
L0571:
    /* 0571  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0572:
    /* 0572  cmp     bx,dx */
    sub16(BX, DX, 0);
L0574:
    /* 0574  jne     short L05E3 */
    if (!ZF) goto L05E3;
L0576:
    /* 0576  neg     ax */
    AX = (uint16_t)-AX;
L0578:
    /* 0578  mov     word ptr ds:[25EAh],ax */
    ww(pDS, 0x25EA, AX);
L057B:
    /* 057B  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L057C:
    /* 057C  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L057E:
    /* 057E  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L0580:
    /* 0580  add     ax,cx */
    AX = add16(AX, CX, 0);
L0582:
    /* 0582  jo      short L05E3 */
    if (OF) goto L05E3;
L0584:
    /* 0584  or      bp,ax */
    BP = (uint16_t)(BP | AX);
L0586:
    /* 0586  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0587:
    /* 0587  push    si */
    push16(SI);
L0588:
    /* 0588  push    di */
    push16(DI);
L0589:
    /* 0589  xchg    si,ax */
    { uint16_t t_ = SI;
    SI = AX;
    AX = t_; }
L058A:
    /* 058A  mov     ax,si */
    AX = SI;
L058C: /* modify_axis2 */
    /* 058C  add     ax,word ptr ds:[25E6h] */
    /* by hand: modify_axis2: seg004_0849_35FF writes the opcode (03h add, 2Bh sub) and the address */
    { uint16_t a = rw(CODE004, 0x058E), v = rw(pDS, a);
      if (CODE004[0x058C] == 0x2B) AX = sub16(AX, v, 0); else AX = add16(AX, v, 0); }
L0590:
    /* 0590  js      short L05F1 */
    if (SF) goto L05F1;
L0592:
    /* 0592  mov     word ptr ds:[4],0 */
    ww(pDS, 0x4, 0x0);
L0598:
    /* 0598  mov     ax,bp */
    AX = BP;
L059A:
    /* 059A  mov     bx,22h */
    BX = 0x22;
L059D:
    /* 059D  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L059F:
    /* 059F  je      short L05A6 */
    if (ZF) goto L05A6;
L05A1:
    /* 05A1  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L05A3:
    /* 05A3  xlatb */
    AL = rb(pDS, BX + AL);
L05A4:
    /* 05A4  jmp     short L05A9 */
    goto L05A9;
L05A6: /* L05A6 */
    /* 05A6  xlatb */
    AL = rb(pDS, BX + AL);
L05A7:
    /* 05A7  add     al,8 */
    AL = (uint8_t)(AL + 0x8);
L05A9: /* L05A9 */
    /* 05A9  mov     byte ptr ds:[25E0h],al */
    wb(pDS, 0x25E0, AL);
L05AC:
    /* 05AC  or      bp,si */
    BP = (uint16_t)(BP | SI);
L05AE:
    /* 05AE  mov     ax,bp */
    AX = BP;
L05B0:
    /* 05B0  mov     bx,22h */
    BX = 0x22;
L05B3:
    /* 05B3  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L05B5:
    /* 05B5  je      short L05BC */
    if (ZF) goto L05BC;
L05B7:
    /* 05B7  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L05B9:
    /* 05B9  xlatb */
    AL = rb(pDS, BX + AL);
L05BA:
    /* 05BA  jmp     short L05BF */
    goto L05BF;
L05BC: /* L05BC */
    /* 05BC  xlatb */
    AL = rb(pDS, BX + AL);
L05BD:
    /* 05BD  add     al,8 */
    AL = (uint8_t)(AL + 0x8);
L05BF: /* L05BF */
    /* 05BF  mov     cl,al */
    CL = AL;
L05C1:
    /* 05C1  mov     bx,word ptr ds:[25E6h] */
    BX = rw(pDS, 0x25E6);
L05C5:
    /* 05C5  mov     bp,word ptr ds:[25EAh] */
    BP = rw(pDS, 0x25EA);
L05C9:
    /* 05C9  mov     ax,word ptr ds:[25E8h] */
    AX = rw(pDS, 0x25E8);
L05CC:
    /* 05CC  call    _sphere_check */
    if ((c = asm_call(ASM_JMP(0x065C, 0x0260), 0x05CF)) != 0) return c;
L05CF:
    /* 05CF  pop     di */
    DI = pop16();
L05D0:
    /* 05D0  jb      short L05F2 */
    if (CF) goto L05F2;
L05D2:
    /* 05D2  mov     word ptr ds:[25FAh],ax */
    ww(pDS, 0x25FA, AX);
L05D5:
    /* 05D5  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34C5), 0x05D8)) != 0) return c;
L05D8:
    /* 05D8  xchg    si,ax */
    { uint16_t t_ = SI;
    SI = AX;
    AX = t_; }
L05D9:
    /* 05D9  call    _set_accept */
    if ((c = asm_call(ASM_JMP(0x065C, 0x34C4), 0x05DC)) != 0) return c;
L05DC:
    /* 05DC  pop     si */
    SI = pop16();
L05DD:
    /* 05DD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L05DE:
    /* 05DE  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L05DF:
    /* 05DF  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L05E3: /* L05E3 */
    /* 05E3  mov     word ptr ds:[25E2h],0FFFFh */
    ww(pDS, 0x25E2, 0xFFFF);
L05E9:
    /* 05E9  mov     si,di */
    SI = DI;
L05EB:
    /* 05EB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L05EC:
    /* 05EC  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L05ED:
    /* 05ED  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L05F1: /* L05F1 */
    /* 05F1  pop     di */
    DI = pop16();
L05F2: /* L05F2 */
    /* 05F2  pop     si */
    SI = pop16();
L05F3:
    /* 05F3  mov     si,di */
    SI = DI;
L05F5:
    /* 05F5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L05F6:
    /* 05F6  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L05F7:
    /* 05F7  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
}
