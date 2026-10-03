/* scanline.c: replaces src/3d/SCANLINE.ASM (seg004_0849_51B0, 51B0..54B8 of its
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

uint32_t asm_mod_SCANLINE(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x51B0: goto L51B0;
    case 0x51B4: goto L51B4;
    case 0x51B8: goto L51B8;
    case 0x51BC: goto L51BC;
    case 0x51C0: goto L51C0;
    case 0x51C4: goto L51C4;
    case 0x51C8: goto L51C8;
    case 0x51CD: goto L51CD;
    case 0x51D1: goto L51D1;
    case 0x51D5: goto L51D5;
    case 0x51D9: goto L51D9;
    case 0x51DD: goto L51DD;
    case 0x51E1: goto L51E1;
    case 0x51E5: goto L51E5;
    case 0x51E8: goto L51E8;
    case 0x51EB: goto L51EB;
    case 0x51EF: goto L51EF;
    case 0x51F0: goto L51F0;
    case 0x51F3: goto L51F3;
    case 0x51F5: goto L51F5;
    case 0x51F7: goto L51F7;
    case 0x51FB: goto L51FB;
    case 0x51FC: goto L51FC;
    case 0x5200: goto L5200;
    case 0x5204: goto L5204;
    case 0x5209: goto L5209;
    case 0x520E: goto L520E;
    case 0x5213: goto L5213;
    case 0x5217: goto L5217;
    case 0x521A: goto L521A;
    case 0x521E: goto L521E;
    case 0x5221: goto L5221;
    case 0x5225: goto L5225;
    case 0x5229: goto L5229;
    case 0x522D: goto L522D;
    case 0x5233: goto L5233;
    case 0x5237: goto L5237;
    case 0x523B: goto L523B;
    case 0x523F: goto L523F;
    case 0x5241: goto L5241;
    case 0x5244: goto L5244;
    case 0x5247: goto L5247;
    case 0x524B: goto L524B;
    case 0x5250: goto L5250;
    case 0x5254: goto L5254;
    case 0x5256: goto L5256;
    case 0x5257: goto L5257;
    case 0x5258: goto L5258;
    case 0x525C: goto L525C;
    case 0x5261: goto L5261;
    case 0x5264: goto L5264;
    case 0x5269: goto L5269;
    case 0x526B: goto L526B;
    case 0x526F: goto L526F;
    case 0x5273: goto L5273;
    case 0x5278: goto L5278;
    case 0x527C: goto L527C;
    case 0x527E: goto L527E;
    case 0x527F: goto L527F;
    case 0x5280: goto L5280;
    case 0x5284: goto L5284;
    case 0x5289: goto L5289;
    case 0x528C: goto L528C;
    case 0x5291: goto L5291;
    case 0x5293: goto L5293;
    case 0x5297: goto L5297;
    case 0x529B: goto L529B;
    case 0x52A0: goto L52A0;
    case 0x52A3: goto L52A3;
    case 0x52A8: goto L52A8;
    case 0x52AA: goto L52AA;
    case 0x52AE: goto L52AE;
    case 0x52B2: goto L52B2;
    case 0x52B6: goto L52B6;
    case 0x52BB: goto L52BB;
    case 0x52C0: goto L52C0;
    case 0x52C5: goto L52C5;
    case 0x52C6: goto L52C6;
    case 0x52CA: goto L52CA;
    case 0x52CF: goto L52CF;
    case 0x52D2: goto L52D2;
    case 0x52D3: goto L52D3;
    case 0x52D9: goto L52D9;
    case 0x52DD: goto L52DD;
    case 0x52E1: goto L52E1;
    case 0x52E5: goto L52E5;
    case 0x52E8: goto L52E8;
    case 0x52EC: goto L52EC;
    case 0x52EE: goto L52EE;
    case 0x52F0: goto L52F0;
    case 0x52F1: goto L52F1;
    case 0x52F2: goto L52F2;
    case 0x52FB: goto L52FB;
    case 0x52FD: goto L52FD;
    case 0x5306: goto L5306;
    case 0x530C: goto L530C;
    case 0x5310: goto L5310;
    case 0x5313: goto L5313;
    case 0x5317: goto L5317;
    case 0x531B: goto L531B;
    case 0x531E: goto L531E;
    case 0x5320: goto L5320;
    case 0x5321: goto L5321;
    case 0x5323: goto L5323;
    case 0x5327: goto L5327;
    case 0x5329: goto L5329;
    case 0x532D: goto L532D;
    case 0x5330: goto L5330;
    case 0x5334: goto L5334;
    case 0x5339: goto L5339;
    case 0x533C: goto L533C;
    case 0x5340: goto L5340;
    case 0x5345: goto L5345;
    case 0x534A: goto L534A;
    case 0x534E: goto L534E;
    case 0x5350: goto L5350;
    case 0x5354: goto L5354;
    case 0x5357: goto L5357;
    case 0x535B: goto L535B;
    case 0x5360: goto L5360;
    case 0x5363: goto L5363;
    case 0x5367: goto L5367;
    case 0x536C: goto L536C;
    case 0x5371: goto L5371;
    case 0x5372: goto L5372;
    case 0x5374: goto L5374;
    case 0x5375: goto L5375;
    case 0x5377: goto L5377;
    case 0x5378: goto L5378;
    case 0x5379: goto L5379;
    case 0x537D: goto L537D;
    case 0x537F: goto L537F;
    case 0x5383: goto L5383;
    case 0x5386: goto L5386;
    case 0x538A: goto L538A;
    case 0x538F: goto L538F;
    case 0x5392: goto L5392;
    case 0x5396: goto L5396;
    case 0x539A: goto L539A;
    case 0x539E: goto L539E;
    case 0x53A0: goto L53A0;
    case 0x53A4: goto L53A4;
    case 0x53A8: goto L53A8;
    case 0x53AB: goto L53AB;
    case 0x53AF: goto L53AF;
    case 0x53B4: goto L53B4;
    case 0x53B7: goto L53B7;
    case 0x53B8: goto L53B8;
    case 0x53BC: goto L53BC;
    case 0x53C0: goto L53C0;
    case 0x53C4: goto L53C4;
    case 0x53C7: goto L53C7;
    case 0x53C9: goto L53C9;
    case 0x53CA: goto L53CA;
    case 0x53CC: goto L53CC;
    case 0x53D0: goto L53D0;
    case 0x53D2: goto L53D2;
    case 0x53D6: goto L53D6;
    case 0x53D9: goto L53D9;
    case 0x53DB: goto L53DB;
    case 0x53E0: goto L53E0;
    case 0x53E3: goto L53E3;
    case 0x53E5: goto L53E5;
    case 0x53EA: goto L53EA;
    case 0x53EE: goto L53EE;
    case 0x53F0: goto L53F0;
    case 0x53F4: goto L53F4;
    case 0x53F7: goto L53F7;
    case 0x53F9: goto L53F9;
    case 0x53FE: goto L53FE;
    case 0x5401: goto L5401;
    case 0x5403: goto L5403;
    case 0x5408: goto L5408;
    case 0x5409: goto L5409;
    case 0x540B: goto L540B;
    case 0x540C: goto L540C;
    case 0x540E: goto L540E;
    case 0x540F: goto L540F;
    case 0x5410: goto L5410;
    case 0x5414: goto L5414;
    case 0x5416: goto L5416;
    case 0x541A: goto L541A;
    case 0x541D: goto L541D;
    case 0x541F: goto L541F;
    case 0x5424: goto L5424;
    case 0x5427: goto L5427;
    case 0x5429: goto L5429;
    case 0x542D: goto L542D;
    case 0x5431: goto L5431;
    case 0x5433: goto L5433;
    case 0x5437: goto L5437;
    case 0x543B: goto L543B;
    case 0x543E: goto L543E;
    case 0x5442: goto L5442;
    case 0x5447: goto L5447;
    case 0x544A: goto L544A;
    case 0x544B: goto L544B;
    case 0x544F: goto L544F;
    case 0x5452: goto L5452;
    case 0x5454: goto L5454;
    case 0x5455: goto L5455;
    case 0x5457: goto L5457;
    case 0x545B: goto L545B;
    case 0x545D: goto L545D;
    case 0x5461: goto L5461;
    case 0x5464: goto L5464;
    case 0x5467: goto L5467;
    case 0x5469: goto L5469;
    case 0x546E: goto L546E;
    case 0x5472: goto L5472;
    case 0x5474: goto L5474;
    case 0x5478: goto L5478;
    case 0x547B: goto L547B;
    case 0x547E: goto L547E;
    case 0x5480: goto L5480;
    case 0x5485: goto L5485;
    case 0x5486: goto L5486;
    case 0x5488: goto L5488;
    case 0x5489: goto L5489;
    case 0x548B: goto L548B;
    case 0x548C: goto L548C;
    case 0x548D: goto L548D;
    case 0x5491: goto L5491;
    case 0x5493: goto L5493;
    case 0x5497: goto L5497;
    case 0x549A: goto L549A;
    case 0x549D: goto L549D;
    case 0x549F: goto L549F;
    case 0x54A3: goto L54A3;
    case 0x54A7: goto L54A7;
    case 0x54A9: goto L54A9;
    case 0x54AD: goto L54AD;
    case 0x54B1: goto L54B1;
    case 0x54B4: goto L54B4;
    case 0x54B7: goto L54B7;
    default: asm_bad_entry("SCANLINE.ASM", entry);
    }

    /* seg004_0849_51B0  (+51B0)
       _asm_texture_map_scanline_lin_tlpv (probable name): draw one shaded vertical column. No
       register inputs (see the module header); changes EAX..EDX, ESI, EDI, EBP. */
L51B0: /* _seg004_0849_51B0 */
    /* 51B0  mov     eax,dword ptr ds:[0CFE8h] */
    EAX = rd(pDS, 0xCFE8);
L51B4:
    /* 51B4  mov     dword ptr ds:[0CF10h],eax */
    wd(pDS, 0xCF10, EAX);
L51B8:
    /* 51B8  mov     eax,dword ptr ds:[0CFF0h] */
    EAX = rd(pDS, 0xCFF0);
L51BC:
    /* 51BC  mov     dword ptr ds:[0CF14h],eax */
    wd(pDS, 0xCF14, EAX);
L51C0:
    /* 51C0  mov     eax,dword ptr ds:[0CFF8h] */
    EAX = rd(pDS, 0xCFF8);
L51C4:
    /* 51C4  mov     dword ptr ds:[0CF1Ch],eax */
    wd(pDS, 0xCF1C, EAX);
L51C8:
    /* 51C8  mov     ebx,dword ptr ds:[0CFE4h] */
    EBX = rd(pDS, 0xCFE4);
L51CD:
    /* 51CD  add     ebx,41h */
    EBX = (uint32_t)(EBX + 0x41);
L51D1:
    /* 51D1  sar     ebx,10h */
    EBX = (uint32_t)((int32_t)EBX >> 16);
L51D5:
    /* 51D5  mov     word ptr ds:[0CF26h],bx */
    ww(pDS, 0xCF26, BX);
L51D9:
    /* 51D9  mov     eax,dword ptr ds:[0CFE0h] */
    EAX = rd(pDS, 0xCFE0);
L51DD:
    /* 51DD  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L51E1:
    /* 51E1  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L51E5:
    /* 51E5  mov     word ptr ds:[0CF24h],ax */
    ww(pDS, 0xCF24, AX);
L51E8:
    /* 51E8  mov     word ptr ds:[0CF5Ah],ax */
    ww(pDS, 0xCF5A, AX);
L51EB:
    /* 51EB  mov     di,word ptr ds:[0CFE2h] */
    DI = rw(pDS, 0xCFE2);
L51EF:
    /* 51EF  push    ds */
    push16(asm_ds);
L51F0:
    /* 51F0  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L51F3:
    /* 51F3  mov     ds,ax */
    SET_DS(AX);
L51F5:
    /* 51F5  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L51F7:
    /* 51F7  mov     di,word ptr [di+95Ch] */
    DI = rw(pDS, DI + 0x95C);
L51FB:
    /* 51FB  pop     ds */
    SET_DS(pop16());
L51FC:
    /* 51FC  add     di,word ptr ds:[0CCC4h] */
    DI = (uint16_t)(DI + rw(pDS, 0xCCC4));
L5200:
    /* 5200  mov     eax,dword ptr ds:[0CFE8h] */
    EAX = rd(pDS, 0xCFE8);
L5204:
    /* 5204  or      eax,dword ptr ds:[0CFECh] */
    EAX = (uint32_t)(EAX | rd(pDS, 0xCFEC));
L5209:
    /* 5209  or      eax,dword ptr ds:[0CFF0h] */
    EAX = (uint32_t)(EAX | rd(pDS, 0xCFF0));
L520E:
    /* 520E  or      eax,dword ptr ds:[0CFF4h] */
    EAX = logic32((uint32_t)(EAX | rd(pDS, 0xCFF4)));
L5213:
    /* 5213  js      L53B7 */
    if (SF) goto L53B7;
L5217:
    /* 5217  mov     ax,word ptr ds:[0CF12h] */
    AX = rw(pDS, 0xCF12);
L521A:
    /* 521A  and     ax,word ptr ds:[0CCE8h] */
    AX = (uint16_t)(AX & rw(pDS, 0xCCE8));
L521E:
    /* 521E  mov     word ptr ds:[0CF2Ch],ax */
    ww(pDS, 0xCF2C, AX);
L5221:
    /* 5221  sub     bx,word ptr ds:[0CF5Ah] */
    BX = sub16(BX, rw(pDS, 0xCF5A), 0);
L5225:
    /* 5225  je      L5396 */
    if (ZF) goto L5396;
L5229:
    /* 5229  js      L5396 */
    if (SF) goto L5396;
L522D:
    /* 522D  mov     eax,10000h */
    EAX = 0x10000;
L5233:
    /* 5233  shl     ebx,10h */
    EBX = (uint32_t)(EBX << 16);
L5237:
    /* 5237  rol     eax,10h */
    EAX = rol32(EAX, 16);
L523B:
    /* 523B  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L523F:
    /* 523F  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5241:
    /* 5241  idiv    ebx */
    if (asm_idiv32(EBX) && (c = asm_divfault(0x065C, 0x5241, 3)) != 0) return c;
L5244:
    /* 5244  mov     ebx,eax */
    EBX = EAX;
L5247:
    /* 5247  mov     eax,dword ptr ds:[0CFECh] */
    EAX = rd(pDS, 0xCFEC);
L524B:
    /* 524B  xor     eax,dword ptr ds:[0CFE8h] */
    EAX = (uint32_t)(EAX ^ rd(pDS, 0xCFE8));
L5250:
    /* 5250  shr     eax,10h */
    EAX = shr32(EAX, 16);
L5254:
    /* 5254  je      L526B */
    if (ZF) goto L526B;
L5256:
    /* 5256  nop */
    ;
L5257:
    /* 5257  nop */
    ;
L5258:
    /* 5258  mov     eax,dword ptr ds:[0CFECh] */
    EAX = rd(pDS, 0xCFEC);
L525C:
    /* 525C  sub     eax,dword ptr ds:[0CFE8h] */
    EAX = (uint32_t)(EAX - rd(pDS, 0xCFE8));
L5261:
    /* 5261  imul    ebx */
    imul32(EBX);
L5264:
    /* 5264  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L5269:
    /* 5269  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L526B: /* L526B */
    /* 526B  mov     dword ptr ds:[0CF00h],eax */
    wd(pDS, 0xCF00, EAX);
L526F:
    /* 526F  mov     eax,dword ptr ds:[0CFF4h] */
    EAX = rd(pDS, 0xCFF4);
L5273:
    /* 5273  xor     eax,dword ptr ds:[0CFF0h] */
    EAX = (uint32_t)(EAX ^ rd(pDS, 0xCFF0));
L5278:
    /* 5278  shr     eax,10h */
    EAX = shr32(EAX, 16);
L527C:
    /* 527C  je      L5293 */
    if (ZF) goto L5293;
L527E:
    /* 527E  nop */
    ;
L527F:
    /* 527F  nop */
    ;
L5280:
    /* 5280  mov     eax,dword ptr ds:[0CFF4h] */
    EAX = rd(pDS, 0xCFF4);
L5284:
    /* 5284  sub     eax,dword ptr ds:[0CFF0h] */
    EAX = (uint32_t)(EAX - rd(pDS, 0xCFF0));
L5289:
    /* 5289  imul    ebx */
    imul32(EBX);
L528C:
    /* 528C  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L5291:
    /* 5291  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L5293: /* L5293 */
    /* 5293  mov     dword ptr ds:[0CF04h],eax */
    wd(pDS, 0xCF04, EAX);
L5297:
    /* 5297  mov     eax,dword ptr ds:[0CFFCh] */
    EAX = rd(pDS, 0xCFFC);
L529B:
    /* 529B  sub     eax,dword ptr ds:[0CFF8h] */
    EAX = (uint32_t)(EAX - rd(pDS, 0xCFF8));
L52A0:
    /* 52A0  imul    ebx */
    imul32(EBX);
L52A3:
    /* 52A3  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L52A8:
    /* 52A8  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L52AA:
    /* 52AA  mov     dword ptr ds:[0CF0Ch],eax */
    wd(pDS, 0xCF0C, EAX);
L52AE:
    /* 52AE  mov     cx,word ptr ds:[0CF26h] */
    CX = rw(pDS, 0xCF26);
L52B2:
    /* 52B2  sub     cx,word ptr ds:[0CF5Ah] */
    CX = sub16(CX, rw(pDS, 0xCF5A), 0);
L52B6:
    /* 52B6  mov     esi,dword ptr ds:[0CF00h] */
    ESI = rd(pDS, 0xCF00);
L52BB:
    /* 52BB  mov     ebp,dword ptr ds:[0CF04h] */
    EBP = rd(pDS, 0xCF04);
L52C0:
    /* 52C0  mov     edx,dword ptr ds:[0CF0Ch] */
    EDX = rd(pDS, 0xCF0C);
L52C5:
    /* 52C5  push    cx */
    push16(CX);
L52C6:
    /* 52C6  mov     cx,word ptr ds:[0CCECh] */
    CX = rw(pDS, 0xCCEC);
L52CA:
    /* 52CA  shl     dword ptr ds:[0CF14h],cl */
    wd(pDS, 0xCF14, shl32(rd(pDS, 0xCF14), CL));
L52CF:
    /* 52CF  shl     ebp,cl */
    EBP = (uint32_t)(EBP << (CL & 31));
L52D2:
    /* 52D2  pop     cx */
    CX = pop16();
L52D3:
    /* 52D3  shl     dword ptr ds:[0CF1Ch],8 */
    wd(pDS, 0xCF1C, (uint32_t)(rd(pDS, 0xCF1C) << 8));
L52D9:
    /* 52D9  shl     edx,8 */
    EDX = (uint32_t)(EDX << 8);
L52DD:
    /* 52DD  mov     eax,dword ptr ds:[0CF1Ch] */
    EAX = rd(pDS, 0xCF1C);
L52E1:
    /* 52E1  mov     dword ptr ds:[0CF20h],eax */
    wd(pDS, 0xCF20, EAX);
L52E5:
    /* 52E5  mov     ax,word ptr ds:[0CFE2h] */
    AX = rw(pDS, 0xCFE2);
L52E8:
    /* 52E8  xor     ax,word ptr ds:[0CCC4h] */
    AX = (uint16_t)(AX ^ rw(pDS, 0xCCC4));
L52EC:
    /* 52EC  shr     ax,1 */
    AX = shr16(AX, 1);
L52EE:
    /* 52EE  jb      L52FD */
    if (CF) goto L52FD;
L52F0:
    /* 52F0  nop */
    ;
L52F1:
    /* 52F1  nop */
    ;
L52F2:
    /* 52F2  add     dword ptr ds:[0CF20h],800000h */
    wd(pDS, 0xCF20, (uint32_t)(rd(pDS, 0xCF20) + 0x800000));
L52FB:
    /* 52FB  jmp     short L5306 */
    goto L5306;
L52FD: /* L52FD */
    /* 52FD  add     dword ptr ds:[0CF1Ch],800000h */
    wd(pDS, 0xCF1C, (uint32_t)(rd(pDS, 0xCF1C) + 0x800000));
L5306: /* L5306 */
    /* 5306  test    word ptr ds:[2688h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x2688) & 0xFFFF));
L530C:
    /* 530C  je      L544B */
    if (ZF) goto L544B;
L5310:
    /* 5310  mov     ax,word ptr ds:[0CFFAh] */
    AX = rw(pDS, 0xCFFA);
L5313:
    /* 5313  cmp     ax,word ptr ds:[0CFFEh] */
    sub16(AX, rw(pDS, 0xCFFE), 0);
L5317:
    /* 5317  je      L53B8 */
    if (ZF) goto L53B8;
L531B:
    /* 531B  mov     ax,word ptr ds:[0CCEAh] */
    AX = rw(pDS, 0xCCEA);
L531E:
    /* 531E  shr     cx,1 */
    CX = shr16(CX, 1);
L5320:
    /* 5320  pushf */
    push16(asm_flags());
L5321:
    /* 5321  je      short L5374 */
    if (ZF) goto L5374;
L5323: /* L5323 */
    /* 5323  mov     bx,word ptr ds:[0CF16h] */
    BX = rw(pDS, 0xCF16);
L5327:
    /* 5327  and     bx,ax */
    BX = (uint16_t)(BX & AX);
L5329:
    /* 5329  add     bx,word ptr ds:[0CF2Ch] */
    BX = (uint16_t)(BX + rw(pDS, 0xCF2C));
L532D:
    /* 532D  mov     bl,byte ptr gs:[bx] */
    BL = rb(pGS, BX);
L5330:
    /* 5330  mov     bh,byte ptr ds:[0CF1Fh] */
    BH = rb(pDS, 0xCF1F);
L5334:
    /* 5334  mov     bl,byte ptr cs:lightabs[bx] */
    BL = rb(CODE004, BX + 0x6D3E);
L5339:
    /* 5339  mov     byte ptr es:[di],bl */
    wb(pES, DI, BL);
L533C:
    /* 533C  add     di,word ptr ds:[0CEFCh] */
    DI = (uint16_t)(DI + rw(pDS, 0xCEFC));
L5340:
    /* 5340  add     dword ptr ds:[0CF14h],ebp */
    wd(pDS, 0xCF14, (uint32_t)(rd(pDS, 0xCF14) + EBP));
L5345:
    /* 5345  add     dword ptr ds:[0CF1Ch],edx */
    wd(pDS, 0xCF1C, (uint32_t)(rd(pDS, 0xCF1C) + EDX));
L534A:
    /* 534A  mov     bx,word ptr ds:[0CF16h] */
    BX = rw(pDS, 0xCF16);
L534E:
    /* 534E  and     bx,ax */
    BX = (uint16_t)(BX & AX);
L5350:
    /* 5350  add     bx,word ptr ds:[0CF2Ch] */
    BX = (uint16_t)(BX + rw(pDS, 0xCF2C));
L5354:
    /* 5354  mov     bl,byte ptr gs:[bx] */
    BL = rb(pGS, BX);
L5357:
    /* 5357  mov     bh,byte ptr ds:[0CF23h] */
    BH = rb(pDS, 0xCF23);
L535B:
    /* 535B  mov     bl,byte ptr cs:lightabs[bx] */
    BL = rb(CODE004, BX + 0x6D3E);
L5360:
    /* 5360  mov     byte ptr es:[di],bl */
    wb(pES, DI, BL);
L5363:
    /* 5363  add     di,word ptr ds:[0CEFCh] */
    DI = (uint16_t)(DI + rw(pDS, 0xCEFC));
L5367:
    /* 5367  add     dword ptr ds:[0CF14h],ebp */
    wd(pDS, 0xCF14, (uint32_t)(rd(pDS, 0xCF14) + EBP));
L536C:
    /* 536C  add     dword ptr ds:[0CF20h],edx */
    wd(pDS, 0xCF20, (uint32_t)(rd(pDS, 0xCF20) + EDX));
L5371:
    /* 5371  dec     cx */
    CX = dec16(CX);
L5372:
    /* 5372  jne     L5323 */
    if (!ZF) goto L5323;
L5374: /* L5374 */
    /* 5374  popf */
    asm_set_flags(pop16());
L5375:
    /* 5375  jae     L5396 */
    if (!CF) goto L5396;
L5377:
    /* 5377  nop */
    ;
L5378:
    /* 5378  nop */
    ;
L5379:
    /* 5379  mov     bx,word ptr ds:[0CF16h] */
    BX = rw(pDS, 0xCF16);
L537D:
    /* 537D  and     bx,ax */
    BX = (uint16_t)(BX & AX);
L537F:
    /* 537F  add     bx,word ptr ds:[0CF2Ch] */
    BX = (uint16_t)(BX + rw(pDS, 0xCF2C));
L5383:
    /* 5383  mov     bl,byte ptr gs:[bx] */
    BL = rb(pGS, BX);
L5386:
    /* 5386  mov     bh,byte ptr ds:[0CF1Fh] */
    BH = rb(pDS, 0xCF1F);
L538A:
    /* 538A  mov     bl,byte ptr cs:lightabs[bx] */
    BL = rb(CODE004, BX + 0x6D3E);
L538F:
    /* 538F  mov     byte ptr es:[di],bl */
    wb(pES, DI, BL);
L5392:
    /* 5392  add     di,word ptr ds:[0CEFCh] */
    DI = add16(DI, rw(pDS, 0xCEFC), 0);
L5396: /* L5396 */
    /* 5396  mov     si,word ptr ds:[0CFF6h] */
    SI = rw(pDS, 0xCFF6);
L539A:
    /* 539A  mov     cx,word ptr ds:[0CCECh] */
    CX = rw(pDS, 0xCCEC);
L539E:
    /* 539E  shl     si,cl */
    SI = (uint16_t)(SI << (CL & 31));
L53A0:
    /* 53A0  and     si,word ptr ds:[0CCEAh] */
    SI = (uint16_t)(SI & rw(pDS, 0xCCEA));
L53A4:
    /* 53A4  add     si,word ptr ds:[0CF2Ch] */
    SI = add16(SI, rw(pDS, 0xCF2C), 0);
L53A8:
    /* 53A8  mov     bl,byte ptr gs:[si] */
    BL = rb(pGS, SI);
L53AB:
    /* 53AB  mov     bh,byte ptr ds:[0CFFEh] */
    BH = rb(pDS, 0xCFFE);
L53AF:
    /* 53AF  mov     al,byte ptr cs:lightabs[bx] */
    AL = rb(CODE004, BX + 0x6D3E);
L53B4:
    /* 53B4  mov     byte ptr es:[di],al */
    wb(pES, DI, AL);
L53B7: /* L53B7 */
    /* 53B7  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L53B8: /* L53B8 */
    /* 53B8  mov     si,word ptr ds:[0CEFCh] */
    SI = rw(pDS, 0xCEFC);
L53BC:
    /* 53BC  mov     dl,byte ptr ds:[0CF1Fh] */
    DL = rb(pDS, 0xCF1F);
L53C0:
    /* 53C0  mov     dh,byte ptr ds:[0CF23h] */
    DH = rb(pDS, 0xCF23);
L53C4:
    /* 53C4  mov     ax,word ptr ds:[0CCEAh] */
    AX = rw(pDS, 0xCCEA);
L53C7:
    /* 53C7  shr     cx,1 */
    CX = shr16(CX, 1);
L53C9:
    /* 53C9  pushf */
    push16(asm_flags());
L53CA:
    /* 53CA  je      short L540B */
    if (ZF) goto L540B;
L53CC: /* L53CC */
    /* 53CC  mov     bx,word ptr ds:[0CF16h] */
    BX = rw(pDS, 0xCF16);
L53D0:
    /* 53D0  and     bx,ax */
    BX = (uint16_t)(BX & AX);
L53D2:
    /* 53D2  add     bx,word ptr ds:[0CF2Ch] */
    BX = (uint16_t)(BX + rw(pDS, 0xCF2C));
L53D6:
    /* 53D6  mov     bl,byte ptr gs:[bx] */
    BL = rb(pGS, BX);
L53D9:
    /* 53D9  mov     bh,dl */
    BH = DL;
L53DB:
    /* 53DB  mov     bl,byte ptr cs:lightabs[bx] */
    BL = rb(CODE004, BX + 0x6D3E);
L53E0:
    /* 53E0  mov     byte ptr es:[di],bl */
    wb(pES, DI, BL);
L53E3:
    /* 53E3  add     di,si */
    DI = (uint16_t)(DI + SI);
L53E5:
    /* 53E5  add     dword ptr ds:[0CF14h],ebp */
    wd(pDS, 0xCF14, (uint32_t)(rd(pDS, 0xCF14) + EBP));
L53EA:
    /* 53EA  mov     bx,word ptr ds:[0CF16h] */
    BX = rw(pDS, 0xCF16);
L53EE:
    /* 53EE  and     bx,ax */
    BX = (uint16_t)(BX & AX);
L53F0:
    /* 53F0  add     bx,word ptr ds:[0CF2Ch] */
    BX = (uint16_t)(BX + rw(pDS, 0xCF2C));
L53F4:
    /* 53F4  mov     bl,byte ptr gs:[bx] */
    BL = rb(pGS, BX);
L53F7:
    /* 53F7  mov     bh,dh */
    BH = DH;
L53F9:
    /* 53F9  mov     bl,byte ptr cs:lightabs[bx] */
    BL = rb(CODE004, BX + 0x6D3E);
L53FE:
    /* 53FE  mov     byte ptr es:[di],bl */
    wb(pES, DI, BL);
L5401:
    /* 5401  add     di,si */
    DI = (uint16_t)(DI + SI);
L5403:
    /* 5403  add     dword ptr ds:[0CF14h],ebp */
    wd(pDS, 0xCF14, (uint32_t)(rd(pDS, 0xCF14) + EBP));
L5408:
    /* 5408  dec     cx */
    CX = dec16(CX);
L5409:
    /* 5409  jne     L53CC */
    if (!ZF) goto L53CC;
L540B: /* L540B */
    /* 540B  popf */
    asm_set_flags(pop16());
L540C:
    /* 540C  jae     L5429 */
    if (!CF) goto L5429;
L540E:
    /* 540E  nop */
    ;
L540F:
    /* 540F  nop */
    ;
L5410:
    /* 5410  mov     bx,word ptr ds:[0CF16h] */
    BX = rw(pDS, 0xCF16);
L5414:
    /* 5414  and     bx,ax */
    BX = (uint16_t)(BX & AX);
L5416:
    /* 5416  add     bx,word ptr ds:[0CF2Ch] */
    BX = (uint16_t)(BX + rw(pDS, 0xCF2C));
L541A:
    /* 541A  mov     bl,byte ptr gs:[bx] */
    BL = rb(pGS, BX);
L541D:
    /* 541D  mov     bh,dl */
    BH = DL;
L541F:
    /* 541F  mov     bl,byte ptr cs:lightabs[bx] */
    BL = rb(CODE004, BX + 0x6D3E);
L5424:
    /* 5424  mov     byte ptr es:[di],bl */
    wb(pES, DI, BL);
L5427:
    /* 5427  add     di,si */
    DI = add16(DI, SI, 0);
L5429: /* L5429 */
    /* 5429  mov     si,word ptr ds:[0CFF6h] */
    SI = rw(pDS, 0xCFF6);
L542D:
    /* 542D  mov     cx,word ptr ds:[0CCECh] */
    CX = rw(pDS, 0xCCEC);
L5431:
    /* 5431  shl     si,cl */
    SI = (uint16_t)(SI << (CL & 31));
L5433:
    /* 5433  and     si,word ptr ds:[0CCEAh] */
    SI = (uint16_t)(SI & rw(pDS, 0xCCEA));
L5437:
    /* 5437  add     si,word ptr ds:[0CF2Ch] */
    SI = add16(SI, rw(pDS, 0xCF2C), 0);
L543B:
    /* 543B  mov     bl,byte ptr gs:[si] */
    BL = rb(pGS, SI);
L543E:
    /* 543E  mov     bh,byte ptr ds:[0CFFEh] */
    BH = rb(pDS, 0xCFFE);
L5442:
    /* 5442  mov     al,byte ptr cs:lightabs[bx] */
    AL = rb(CODE004, BX + 0x6D3E);
L5447:
    /* 5447  mov     byte ptr es:[di],al */
    wb(pES, DI, AL);
L544A:
    /* 544A  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L544B: /* L544B */
    /* 544B  mov     si,word ptr ds:[0CEFCh] */
    SI = rw(pDS, 0xCEFC);
L544F:
    /* 544F  mov     ax,word ptr ds:[0CCEAh] */
    AX = rw(pDS, 0xCCEA);
L5452:
    /* 5452  shr     cx,1 */
    CX = shr16(CX, 1);
L5454:
    /* 5454  pushf */
    push16(asm_flags());
L5455:
    /* 5455  je      short L5488 */
    if (ZF) goto L5488;
L5457: /* L5457 */
    /* 5457  mov     bx,word ptr ds:[0CF16h] */
    BX = rw(pDS, 0xCF16);
L545B:
    /* 545B  and     bx,ax */
    BX = (uint16_t)(BX & AX);
L545D:
    /* 545D  add     bx,word ptr ds:[0CF2Ch] */
    BX = (uint16_t)(BX + rw(pDS, 0xCF2C));
L5461:
    /* 5461  mov     bl,byte ptr gs:[bx] */
    BL = rb(pGS, BX);
L5464:
    /* 5464  mov     byte ptr es:[di],bl */
    wb(pES, DI, BL);
L5467:
    /* 5467  add     di,si */
    DI = (uint16_t)(DI + SI);
L5469:
    /* 5469  add     dword ptr ds:[0CF14h],ebp */
    wd(pDS, 0xCF14, (uint32_t)(rd(pDS, 0xCF14) + EBP));
L546E:
    /* 546E  mov     bx,word ptr ds:[0CF16h] */
    BX = rw(pDS, 0xCF16);
L5472:
    /* 5472  and     bx,ax */
    BX = (uint16_t)(BX & AX);
L5474:
    /* 5474  add     bx,word ptr ds:[0CF2Ch] */
    BX = (uint16_t)(BX + rw(pDS, 0xCF2C));
L5478:
    /* 5478  mov     bl,byte ptr gs:[bx] */
    BL = rb(pGS, BX);
L547B:
    /* 547B  mov     byte ptr es:[di],bl */
    wb(pES, DI, BL);
L547E:
    /* 547E  add     di,si */
    DI = (uint16_t)(DI + SI);
L5480:
    /* 5480  add     dword ptr ds:[0CF14h],ebp */
    wd(pDS, 0xCF14, (uint32_t)(rd(pDS, 0xCF14) + EBP));
L5485:
    /* 5485  dec     cx */
    CX = dec16(CX);
L5486:
    /* 5486  jne     L5457 */
    if (!ZF) goto L5457;
L5488: /* L5488 */
    /* 5488  popf */
    asm_set_flags(pop16());
L5489:
    /* 5489  jae     L549F */
    if (!CF) goto L549F;
L548B:
    /* 548B  nop */
    ;
L548C:
    /* 548C  nop */
    ;
L548D:
    /* 548D  mov     bx,word ptr ds:[0CF16h] */
    BX = rw(pDS, 0xCF16);
L5491:
    /* 5491  and     bx,ax */
    BX = (uint16_t)(BX & AX);
L5493:
    /* 5493  add     bx,word ptr ds:[0CF2Ch] */
    BX = (uint16_t)(BX + rw(pDS, 0xCF2C));
L5497:
    /* 5497  mov     al,byte ptr gs:[bx] */
    AL = rb(pGS, BX);
L549A:
    /* 549A  mov     byte ptr es:[di],al */
    wb(pES, DI, AL);
L549D:
    /* 549D  add     di,si */
    DI = add16(DI, SI, 0);
L549F: /* L549F */
    /* 549F  mov     si,word ptr ds:[0CFF6h] */
    SI = rw(pDS, 0xCFF6);
L54A3:
    /* 54A3  mov     cx,word ptr ds:[0CCECh] */
    CX = rw(pDS, 0xCCEC);
L54A7:
    /* 54A7  shl     si,cl */
    SI = (uint16_t)(SI << (CL & 31));
L54A9:
    /* 54A9  and     si,word ptr ds:[0CCEAh] */
    SI = (uint16_t)(SI & rw(pDS, 0xCCEA));
L54AD:
    /* 54AD  add     si,word ptr ds:[0CF2Ch] */
    SI = add16(SI, rw(pDS, 0xCF2C), 0);
L54B1:
    /* 54B1  mov     al,byte ptr gs:[si] */
    AL = rb(pGS, SI);
L54B4:
    /* 54B4  mov     byte ptr es:[di],al */
    wb(pES, DI, AL);
L54B7:
    /* 54B7  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
