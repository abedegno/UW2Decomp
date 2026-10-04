/* polyfill.c: replaces src/gfx/POLYFILL.ASM (seg003_422, 0422..08E7 of its
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

uint32_t asm_mod_POLYFILL(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x0422: goto L0422;
    case 0x0426: goto L0426;
    case 0x0429: goto L0429;
    case 0x042C: goto L042C;
    case 0x042E: goto L042E;
    case 0x0431: goto L0431;
    case 0x0434: goto L0434;
    case 0x0437: goto L0437;
    case 0x043A: goto L043A;
    case 0x043D: goto L043D;
    case 0x0441: goto L0441;
    case 0x0443: goto L0443;
    case 0x0446: goto L0446;
    case 0x044A: goto L044A;
    case 0x044F: goto L044F;
    case 0x0453: goto L0453;
    case 0x0455: goto L0455;
    case 0x0458: goto L0458;
    case 0x045C: goto L045C;
    case 0x045F: goto L045F;
    case 0x0463: goto L0463;
    case 0x0467: goto L0467;
    case 0x0469: goto L0469;
    case 0x046B: goto L046B;
    case 0x046D: goto L046D;
    case 0x0471: goto L0471;
    case 0x0472: goto L0472;
    case 0x0474: goto L0474;
    case 0x0478: goto L0478;
    case 0x047A: goto L047A;
    case 0x047C: goto L047C;
    case 0x047E: goto L047E;
    case 0x0480: goto L0480;
    case 0x0481: goto L0481;
    case 0x0483: goto L0483;
    case 0x0485: goto L0485;
    case 0x0489: goto L0489;
    case 0x048E: goto L048E;
    case 0x0491: goto L0491;
    case 0x0495: goto L0495;
    case 0x0496: goto L0496;
    case 0x0498: goto L0498;
    case 0x049C: goto L049C;
    case 0x049E: goto L049E;
    case 0x04A0: goto L04A0;
    case 0x04A2: goto L04A2;
    case 0x04A4: goto L04A4;
    case 0x04A5: goto L04A5;
    case 0x04A7: goto L04A7;
    case 0x04A9: goto L04A9;
    case 0x04AD: goto L04AD;
    case 0x04B2: goto L04B2;
    case 0x04B8: goto L04B8;
    case 0x04B9: goto L04B9;
    case 0x04BD: goto L04BD;
    case 0x04C0: goto L04C0;
    case 0x04C3: goto L04C3;
    case 0x04C5: goto L04C5;
    case 0x04C8: goto L04C8;
    case 0x04CB: goto L04CB;
    case 0x04CE: goto L04CE;
    case 0x04D1: goto L04D1;
    case 0x04D4: goto L04D4;
    case 0x04D8: goto L04D8;
    case 0x04DA: goto L04DA;
    case 0x04DE: goto L04DE;
    case 0x04E2: goto L04E2;
    case 0x04E6: goto L04E6;
    case 0x04E8: goto L04E8;
    case 0x04EB: goto L04EB;
    case 0x04EF: goto L04EF;
    case 0x04F3: goto L04F3;
    case 0x04F5: goto L04F5;
    case 0x04F7: goto L04F7;
    case 0x04F9: goto L04F9;
    case 0x04FD: goto L04FD;
    case 0x04FE: goto L04FE;
    case 0x0500: goto L0500;
    case 0x0504: goto L0504;
    case 0x0506: goto L0506;
    case 0x0508: goto L0508;
    case 0x050A: goto L050A;
    case 0x050C: goto L050C;
    case 0x050D: goto L050D;
    case 0x050F: goto L050F;
    case 0x0511: goto L0511;
    case 0x0515: goto L0515;
    case 0x051A: goto L051A;
    case 0x051D: goto L051D;
    case 0x0521: goto L0521;
    case 0x0522: goto L0522;
    case 0x0524: goto L0524;
    case 0x0528: goto L0528;
    case 0x052A: goto L052A;
    case 0x052C: goto L052C;
    case 0x052E: goto L052E;
    case 0x0530: goto L0530;
    case 0x0531: goto L0531;
    case 0x0533: goto L0533;
    case 0x0535: goto L0535;
    case 0x0539: goto L0539;
    case 0x053E: goto L053E;
    case 0x0544: goto L0544;
    case 0x0545: goto L0545;
    case 0x054A: goto L054A;
    case 0x054C: goto L054C;
    case 0x0550: goto L0550;
    case 0x0551: goto L0551;
    case 0x0552: goto L0552;
    case 0x0555: goto L0555;
    case 0x0557: goto L0557;
    case 0x0559: goto L0559;
    case 0x055E: goto L055E;
    case 0x0563: goto L0563;
    case 0x0564: goto L0564;
    case 0x0566: goto L0566;
    case 0x056A: goto L056A;
    case 0x056B: goto L056B;
    case 0x056D: goto L056D;
    case 0x056F: goto L056F;
    case 0x0571: goto L0571;
    case 0x0573: goto L0573;
    case 0x0575: goto L0575;
    case 0x0577: goto L0577;
    case 0x0578: goto L0578;
    case 0x0579: goto L0579;
    case 0x057B: goto L057B;
    case 0x057E: goto L057E;
    case 0x0580: goto L0580;
    case 0x0582: goto L0582;
    case 0x0585: goto L0585;
    case 0x0588: goto L0588;
    case 0x058A: goto L058A;
    case 0x058C: goto L058C;
    case 0x058D: goto L058D;
    case 0x058E: goto L058E;
    case 0x0591: goto L0591;
    case 0x0594: goto L0594;
    case 0x0597: goto L0597;
    case 0x059A: goto L059A;
    case 0x059B: goto L059B;
    case 0x059F: goto L059F;
    case 0x05A4: goto L05A4;
    case 0x05A9: goto L05A9;
    case 0x05AB: goto L05AB;
    case 0x05AC: goto L05AC;
    case 0x05AF: goto L05AF;
    case 0x05B2: goto L05B2;
    case 0x05B4: goto L05B4;
    case 0x05B7: goto L05B7;
    case 0x05B9: goto L05B9;
    case 0x05BD: goto L05BD;
    case 0x05C0: goto L05C0;
    case 0x05C3: goto L05C3;
    case 0x05C6: goto L05C6;
    case 0x05C8: goto L05C8;
    case 0x05C9: goto L05C9;
    case 0x05CA: goto L05CA;
    case 0x05CC: goto L05CC;
    case 0x05CE: goto L05CE;
    case 0x05D0: goto L05D0;
    case 0x05D2: goto L05D2;
    case 0x05D4: goto L05D4;
    case 0x05D6: goto L05D6;
    case 0x05D8: goto L05D8;
    case 0x05DC: goto L05DC;
    case 0x05DD: goto L05DD;
    case 0x05E0: goto L05E0;
    case 0x05E2: goto L05E2;
    case 0x05E5: goto L05E5;
    case 0x05E8: goto L05E8;
    case 0x05EA: goto L05EA;
    case 0x05EE: goto L05EE;
    case 0x05F3: goto L05F3;
    case 0x05F8: goto L05F8;
    case 0x05FD: goto L05FD;
    case 0x0601: goto L0601;
    case 0x0603: goto L0603;
    case 0x0607: goto L0607;
    case 0x060B: goto L060B;
    case 0x060C: goto L060C;
    case 0x0610: goto L0610;
    case 0x0611: goto L0611;
    case 0x0614: goto L0614;
    case 0x0618: goto L0618;
    case 0x061F: goto L061F;
    case 0x0626: goto L0626;
    case 0x062D: goto L062D;
    case 0x0634: goto L0634;
    case 0x0638: goto L0638;
    case 0x063B: goto L063B;
    case 0x063E: goto L063E;
    case 0x0640: goto L0640;
    case 0x0641: goto L0641;
    case 0x0643: goto L0643;
    case 0x0645: goto L0645;
    case 0x0646: goto L0646;
    case 0x0648: goto L0648;
    case 0x064A: goto L064A;
    case 0x064C: goto L064C;
    case 0x064E: goto L064E;
    case 0x0651: goto L0651;
    case 0x0653: goto L0653;
    case 0x0656: goto L0656;
    case 0x0658: goto L0658;
    case 0x065B: goto L065B;
    case 0x065F: goto L065F;
    case 0x0661: goto L0661;
    case 0x0664: goto L0664;
    case 0x0667: goto L0667;
    case 0x0669: goto L0669;
    case 0x066D: goto L066D;
    case 0x0670: goto L0670;
    case 0x0673: goto L0673;
    case 0x0675: goto L0675;
    case 0x0676: goto L0676;
    case 0x0678: goto L0678;
    case 0x067A: goto L067A;
    case 0x067C: goto L067C;
    case 0x067F: goto L067F;
    case 0x0682: goto L0682;
    case 0x0684: goto L0684;
    case 0x0685: goto L0685;
    case 0x0687: goto L0687;
    case 0x0688: goto L0688;
    case 0x068A: goto L068A;
    case 0x068C: goto L068C;
    case 0x068E: goto L068E;
    case 0x0691: goto L0691;
    case 0x0694: goto L0694;
    case 0x0697: goto L0697;
    case 0x0699: goto L0699;
    case 0x069B: goto L069B;
    case 0x069D: goto L069D;
    case 0x06A1: goto L06A1;
    case 0x06A4: goto L06A4;
    case 0x06A8: goto L06A8;
    case 0x06AB: goto L06AB;
    case 0x06AE: goto L06AE;
    case 0x06B1: goto L06B1;
    case 0x06B4: goto L06B4;
    case 0x06B7: goto L06B7;
    case 0x06BD: goto L06BD;
    case 0x06C0: goto L06C0;
    case 0x06C3: goto L06C3;
    case 0x06C9: goto L06C9;
    case 0x06CC: goto L06CC;
    case 0x06CF: goto L06CF;
    case 0x06D0: goto L06D0;
    case 0x06D2: goto L06D2;
    case 0x06D5: goto L06D5;
    case 0x06D7: goto L06D7;
    case 0x06D9: goto L06D9;
    case 0x06DB: goto L06DB;
    case 0x06DD: goto L06DD;
    case 0x06DE: goto L06DE;
    case 0x06E0: goto L06E0;
    case 0x06E2: goto L06E2;
    case 0x06E5: goto L06E5;
    case 0x06E9: goto L06E9;
    case 0x06EC: goto L06EC;
    case 0x06EF: goto L06EF;
    case 0x06F0: goto L06F0;
    case 0x06F2: goto L06F2;
    case 0x06F5: goto L06F5;
    case 0x06F7: goto L06F7;
    case 0x06F9: goto L06F9;
    case 0x06FB: goto L06FB;
    case 0x06FD: goto L06FD;
    case 0x06FE: goto L06FE;
    case 0x0700: goto L0700;
    case 0x0702: goto L0702;
    case 0x0705: goto L0705;
    case 0x0709: goto L0709;
    case 0x070C: goto L070C;
    case 0x070F: goto L070F;
    case 0x0713: goto L0713;
    case 0x0717: goto L0717;
    case 0x071A: goto L071A;
    case 0x071E: goto L071E;
    case 0x0722: goto L0722;
    case 0x0724: goto L0724;
    case 0x0729: goto L0729;
    case 0x072E: goto L072E;
    case 0x0730: goto L0730;
    case 0x0734: goto L0734;
    case 0x0738: goto L0738;
    case 0x073B: goto L073B;
    case 0x073F: goto L073F;
    case 0x0743: goto L0743;
    case 0x0748: goto L0748;
    case 0x074C: goto L074C;
    case 0x0750: goto L0750;
    case 0x0753: goto L0753;
    case 0x0757: goto L0757;
    case 0x075B: goto L075B;
    case 0x075D: goto L075D;
    case 0x0761: goto L0761;
    case 0x0763: goto L0763;
    case 0x0767: goto L0767;
    case 0x076B: goto L076B;
    case 0x076D: goto L076D;
    case 0x0770: goto L0770;
    case 0x0772: goto L0772;
    case 0x0774: goto L0774;
    case 0x0778: goto L0778;
    case 0x0779: goto L0779;
    case 0x077B: goto L077B;
    case 0x077D: goto L077D;
    case 0x077F: goto L077F;
    case 0x0781: goto L0781;
    case 0x0784: goto L0784;
    case 0x0786: goto L0786;
    case 0x0788: goto L0788;
    case 0x0789: goto L0789;
    case 0x078D: goto L078D;
    case 0x0791: goto L0791;
    case 0x0793: goto L0793;
    case 0x0795: goto L0795;
    case 0x0797: goto L0797;
    case 0x0799: goto L0799;
    case 0x079C: goto L079C;
    case 0x079E: goto L079E;
    case 0x07A0: goto L07A0;
    case 0x07A2: goto L07A2;
    case 0x07A4: goto L07A4;
    case 0x07A6: goto L07A6;
    case 0x07A8: goto L07A8;
    case 0x07AB: goto L07AB;
    case 0x07AF: goto L07AF;
    case 0x07B1: goto L07B1;
    case 0x07B3: goto L07B3;
    case 0x07B7: goto L07B7;
    case 0x07BA: goto L07BA;
    case 0x07BC: goto L07BC;
    case 0x07C0: goto L07C0;
    case 0x07C2: goto L07C2;
    case 0x07C4: goto L07C4;
    case 0x07C6: goto L07C6;
    case 0x07C9: goto L07C9;
    case 0x07CB: goto L07CB;
    case 0x07CE: goto L07CE;
    case 0x07D1: goto L07D1;
    case 0x07D3: goto L07D3;
    case 0x07D6: goto L07D6;
    case 0x07DA: goto L07DA;
    case 0x07DE: goto L07DE;
    case 0x07E0: goto L07E0;
    case 0x07E4: goto L07E4;
    case 0x07E8: goto L07E8;
    case 0x07EA: goto L07EA;
    case 0x07EE: goto L07EE;
    case 0x07F0: goto L07F0;
    case 0x07F2: goto L07F2;
    case 0x07F6: goto L07F6;
    case 0x07F9: goto L07F9;
    case 0x07FB: goto L07FB;
    case 0x07FC: goto L07FC;
    case 0x07FE: goto L07FE;
    case 0x0802: goto L0802;
    case 0x0804: goto L0804;
    case 0x0806: goto L0806;
    case 0x0808: goto L0808;
    case 0x080A: goto L080A;
    case 0x080B: goto L080B;
    case 0x080D: goto L080D;
    case 0x080F: goto L080F;
    case 0x0813: goto L0813;
    case 0x0818: goto L0818;
    case 0x0819: goto L0819;
    case 0x081B: goto L081B;
    case 0x081E: goto L081E;
    case 0x0820: goto L0820;
    case 0x0824: goto L0824;
    case 0x0828: goto L0828;
    case 0x082C: goto L082C;
    case 0x082E: goto L082E;
    case 0x0832: goto L0832;
    case 0x0834: goto L0834;
    case 0x0835: goto L0835;
    case 0x0836: goto L0836;
    case 0x083A: goto L083A;
    case 0x083E: goto L083E;
    case 0x0840: goto L0840;
    case 0x0845: goto L0845;
    case 0x0849: goto L0849;
    case 0x084F: goto L084F;
    case 0x0855: goto L0855;
    case 0x085B: goto L085B;
    case 0x0861: goto L0861;
    case 0x0867: goto L0867;
    case 0x086D: goto L086D;
    case 0x0873: goto L0873;
    case 0x0879: goto L0879;
    case 0x087D: goto L087D;
    case 0x0881: goto L0881;
    case 0x0883: goto L0883;
    case 0x0887: goto L0887;
    case 0x0889: goto L0889;
    case 0x088C: goto L088C;
    case 0x0890: goto L0890;
    case 0x0893: goto L0893;
    case 0x0895: goto L0895;
    case 0x0898: goto L0898;
    case 0x0899: goto L0899;
    case 0x089E: goto L089E;
    case 0x08A3: goto L08A3;
    case 0x08A4: goto L08A4;
    case 0x08A5: goto L08A5;
    case 0x08A6: goto L08A6;
    case 0x08A7: goto L08A7;
    case 0x08A8: goto L08A8;
    case 0x08A9: goto L08A9;
    case 0x08AC: goto L08AC;
    case 0x08AE: goto L08AE;
    case 0x08B1: goto L08B1;
    case 0x08B4: goto L08B4;
    case 0x08B7: goto L08B7;
    case 0x08B9: goto L08B9;
    case 0x08BB: goto L08BB;
    case 0x08BF: goto L08BF;
    case 0x08C1: goto L08C1;
    case 0x08C3: goto L08C3;
    case 0x08C5: goto L08C5;
    case 0x08C9: goto L08C9;
    case 0x08CD: goto L08CD;
    case 0x08CE: goto L08CE;
    case 0x08CF: goto L08CF;
    case 0x08D0: goto L08D0;
    case 0x08D1: goto L08D1;
    case 0x08D5: goto L08D5;
    case 0x08D8: goto L08D8;
    case 0x08DC: goto L08DC;
    case 0x08DE: goto L08DE;
    case 0x08E0: goto L08E0;
    default: asm_bad_entry("POLYFILL.ASM", entry);
    }

    /* seg003_422  (+422)
       _422 (get_wright, FM Towns): advance the right-hand edge to the next vertex (forwards through
       the list, wrapping at 7AB): load its x and texture coordinate, and patch the per-row steps of
       both (16.16, from two divides by the edge's height) into the drawing loop. Out: ZF set when no
       vertices are left. L043A is the re-entry used while drawing. */
L0422: /* _seg003_422 */
    /* 0422  mov     bx,word ptr ds:[7B5h] */
    BX = rw(pDS, 0x7B5);
L0426:
    /* 0426  mov     ax,word ptr [bx+6] */
    AX = rw(pDS, BX + 0x6);
L0429:
    /* 0429  mov     word ptr ds:[7C9h],ax */
    ww(pDS, 0x7C9, AX);
L042C:
    /* 042C  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L042E:
    /* 042E  mov     word ptr ds:[7C3h],ax */
    ww(pDS, 0x7C3, AX);
L0431:
    /* 0431  mov     ax,8000h */
    AX = 0x8000;
L0434:
    /* 0434  mov     word ptr ds:[7C5h],ax */
    ww(pDS, 0x7C5, AX);
L0437:
    /* 0437  mov     word ptr ds:[7CDh],ax */
    ww(pDS, 0x7CD, AX);
L043A: /* L043A */
    /* 043A  add     bx,0Eh */
    BX = (uint16_t)(BX + 0xE);
L043D:
    /* 043D  cmp     bx,word ptr ds:[7ABh] */
    sub16(BX, rw(pDS, 0x7AB), 0);
L0441:
    /* 0441  jne     L0446 */
    if (!ZF) goto L0446;
L0443:
    /* 0443  mov     bx,659h */
    BX = 0x659;
L0446: /* L0446 */
    /* 0446  mov     word ptr ds:[7B5h],bx */
    ww(pDS, 0x7B5, BX);
L044A:
    /* 044A  mov     word ptr cs:_seg003_3BC,bx */
    ww(CODE003, 0x3BC, BX);
L044F:
    /* 044F  dec     word ptr ds:[7D1h] */
    ww(pDS, 0x7D1, dec16(rw(pDS, 0x7D1)));
L0453:
    /* 0453  je      L04B8 */
    if (ZF) goto L04B8;
L0455:
    /* 0455  mov     ax,word ptr ds:[7D1h] */
    AX = rw(pDS, 0x7D1);
L0458:
    /* 0458  mov     word ptr cs:_seg003_3B4,ax */
    ww(CODE003, 0x3B4, AX);
L045C:
    /* 045C  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L045F:
    /* 045F  mov     word ptr cs:L0885,ax */
    ww(CODE003, 0x885, AX);
L0463:
    /* 0463  mov     cx,word ptr ds:[7CFh] */
    CX = rw(pDS, 0x7CF);
L0467:
    /* 0467  sub     cx,ax */
    CX = sub16(CX, AX, 0);
L0469:
    /* 0469  je      _seg003_422 */
    if (ZF) goto L0422;
L046B:
    /* 046B  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L046D:
    /* 046D  sub     ax,word ptr ds:[7C3h] */
    AX = (uint16_t)(AX - rw(pDS, 0x7C3));
L0471:
    /* 0471  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0472:
    /* 0472  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x0472, 2)) != 0) return c;
L0474:
    /* 0474  mov     word ptr cs:L0877,ax */
    ww(CODE003, 0x877, AX);
L0478:
    /* 0478  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L047A:
    /* 047A  sar     dx,1 */
    DX = sar16(DX, 1);
L047C:
    /* 047C  rcr     ax,1 */
    AX = rcr16(AX, 1);
L047E:
    /* 047E  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x047E, 2)) != 0) return c;
L0480:
    /* 0480  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0481:
    /* 0481  shl     ax,1 */
    AX = shl16(AX, 1);
L0483:
    /* 0483  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0485:
    /* 0485  mov     word ptr cs:L0871,ax */
    ww(CODE003, 0x871, AX);
L0489:
    /* 0489  add     word ptr cs:L0877,dx */
    ww(CODE003, 0x877, (uint16_t)(rw(CODE003, 0x877) + DX));
L048E:
    /* 048E  mov     ax,word ptr [bx+6] */
    AX = rw(pDS, BX + 0x6);
L0491:
    /* 0491  sub     ax,word ptr ds:[7C9h] */
    AX = (uint16_t)(AX - rw(pDS, 0x7C9));
L0495:
    /* 0495  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0496:
    /* 0496  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x0496, 2)) != 0) return c;
L0498:
    /* 0498  mov     word ptr cs:L086B,ax */
    ww(CODE003, 0x86B, AX);
L049C:
    /* 049C  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L049E:
    /* 049E  sar     dx,1 */
    DX = sar16(DX, 1);
L04A0:
    /* 04A0  rcr     ax,1 */
    AX = rcr16(AX, 1);
L04A2:
    /* 04A2  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x04A2, 2)) != 0) return c;
L04A4:
    /* 04A4  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L04A5:
    /* 04A5  shl     ax,1 */
    AX = shl16(AX, 1);
L04A7:
    /* 04A7  rcl     dx,1 */
    DX = rcl16(DX, 1);
L04A9:
    /* 04A9  mov     word ptr cs:L0865,ax */
    ww(CODE003, 0x865, AX);
L04AD:
    /* 04AD  add     word ptr cs:L086B,dx */
    ww(CODE003, 0x86B, (uint16_t)(rw(CODE003, 0x86B) + DX));
L04B2:
    /* 04B2  test    word ptr ds:[7D1h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x7D1) & 0xFFFF));
L04B8: /* L04B8 */
    /* 04B8  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_4B9  (+4B9)
       _4B9 (get_wleft, FM Towns): the same for the left-hand edge, backwards through the list. After
       its ret is wmap_bitmap (+545, far): save the caller's stack and switch to the library's, copy
       the vertex list to MAPDATA's _3CE, find the extremes, build the column table, then draw row by
       row until both edges run out, and return far on the caller's stack. */
L04B9: /* _seg003_4B9 */
    /* 04B9  mov     bx,word ptr ds:[7B3h] */
    BX = rw(pDS, 0x7B3);
L04BD:
    /* 04BD  mov     ax,word ptr [bx+6] */
    AX = rw(pDS, BX + 0x6);
L04C0:
    /* 04C0  mov     word ptr ds:[7BDh],ax */
    ww(pDS, 0x7BD, AX);
L04C3:
    /* 04C3  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L04C5:
    /* 04C5  mov     word ptr ds:[7B7h],ax */
    ww(pDS, 0x7B7, AX);
L04C8:
    /* 04C8  mov     ax,8000h */
    AX = 0x8000;
L04CB:
    /* 04CB  mov     word ptr ds:[7B9h],ax */
    ww(pDS, 0x7B9, AX);
L04CE:
    /* 04CE  mov     word ptr ds:[7C1h],ax */
    ww(pDS, 0x7C1, AX);
L04D1: /* L04D1 */
    /* 04D1  sub     bx,0Eh */
    BX = (uint16_t)(BX - 0xE);
L04D4:
    /* 04D4  cmp     bx,64Bh */
    sub16(BX, 0x64B, 0);
L04D8:
    /* 04D8  jne     L04DE */
    if (!ZF) goto L04DE;
L04DA:
    /* 04DA  mov     bx,word ptr ds:[7A9h] */
    BX = rw(pDS, 0x7A9);
L04DE: /* L04DE */
    /* 04DE  mov     word ptr ds:[7B3h],bx */
    ww(pDS, 0x7B3, BX);
L04E2:
    /* 04E2  dec     word ptr ds:[7D1h] */
    ww(pDS, 0x7D1, dec16(rw(pDS, 0x7D1)));
L04E6:
    /* 04E6  je      L04B8 */
    if (ZF) goto L04B8;
L04E8:
    /* 04E8  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L04EB:
    /* 04EB  mov     word ptr cs:L087F,ax */
    ww(CODE003, 0x87F, AX);
L04EF:
    /* 04EF  mov     cx,word ptr ds:[7CFh] */
    CX = rw(pDS, 0x7CF);
L04F3:
    /* 04F3  sub     cx,ax */
    CX = sub16(CX, AX, 0);
L04F5:
    /* 04F5  je      _seg003_4B9 */
    if (ZF) goto L04B9;
L04F7:
    /* 04F7  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L04F9:
    /* 04F9  sub     ax,word ptr ds:[7B7h] */
    AX = (uint16_t)(AX - rw(pDS, 0x7B7));
L04FD:
    /* 04FD  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L04FE:
    /* 04FE  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x04FE, 2)) != 0) return c;
L0500:
    /* 0500  mov     word ptr cs:L085F,ax */
    ww(CODE003, 0x85F, AX);
L0504:
    /* 0504  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L0506:
    /* 0506  sar     dx,1 */
    DX = sar16(DX, 1);
L0508:
    /* 0508  rcr     ax,1 */
    AX = rcr16(AX, 1);
L050A:
    /* 050A  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x050A, 2)) != 0) return c;
L050C:
    /* 050C  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L050D:
    /* 050D  shl     ax,1 */
    AX = shl16(AX, 1);
L050F:
    /* 050F  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0511:
    /* 0511  mov     word ptr cs:L0859,ax */
    ww(CODE003, 0x859, AX);
L0515:
    /* 0515  add     word ptr cs:L085F,dx */
    ww(CODE003, 0x85F, (uint16_t)(rw(CODE003, 0x85F) + DX));
L051A:
    /* 051A  mov     ax,word ptr [bx+6] */
    AX = rw(pDS, BX + 0x6);
L051D:
    /* 051D  sub     ax,word ptr ds:[7BDh] */
    AX = (uint16_t)(AX - rw(pDS, 0x7BD));
L0521:
    /* 0521  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0522:
    /* 0522  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x0522, 2)) != 0) return c;
L0524:
    /* 0524  mov     word ptr cs:L0853,ax */
    ww(CODE003, 0x853, AX);
L0528:
    /* 0528  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L052A:
    /* 052A  sar     dx,1 */
    DX = sar16(DX, 1);
L052C:
    /* 052C  rcr     ax,1 */
    AX = rcr16(AX, 1);
L052E:
    /* 052E  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x052E, 2)) != 0) return c;
L0530:
    /* 0530  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0531:
    /* 0531  shl     ax,1 */
    AX = shl16(AX, 1);
L0533:
    /* 0533  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0535:
    /* 0535  mov     word ptr cs:L084D,ax */
    ww(CODE003, 0x84D, AX);
L0539:
    /* 0539  add     word ptr cs:L0853,dx */
    ww(CODE003, 0x853, (uint16_t)(rw(CODE003, 0x853) + DX));
L053E:
    /* 053E  test    word ptr ds:[7D1h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x7D1) & 0xFFFF));
L0544:
    /* 0544  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* wmap_bitmap's far entry (+545), named because TMAPOPS stores its offset (the link needs the
       name to take this module from the library) */
L0545: /* _seg003_545 */
    /* 0545  mov     word ptr cs:_seg003_3CA,cx */
    ww(CODE003, 0x3CA, CX);
L054A:
    /* 054A  mov     ax,es */
    AX = asm_es;
L054C:
    /* 054C  mov     word ptr cs:_seg003_3CC,ax */
    ww(CODE003, 0x3CC, AX);
L0550:
    /* 0550  push    es */
    push16(asm_es);
L0551:
    /* 0551  push    ds */
    push16(asm_ds);
L0552:
    /* 0552  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L0555:
    /* 0555  mov     ds,ax */
    SET_DS(AX);
L0557:
    /* 0557  mov     bx,ss */
    BX = asm_ss;
L0559:
    /* 0559  mov     word ptr cs:L08E3,bx */
    ww(CODE003, 0x8E3, BX);
L055E:
    /* 055E  mov     word ptr cs:L08E5,sp */
    ww(CODE003, 0x8E5, SP);
L0563:
    /* 0563  cli */
    ;
L0564:
    /* 0564  mov     ss,ax */
    SET_SS(AX);
L0566:
    /* 0566  mov     sp,word ptr ds:[5586h] */
    SP = rw(pDS, 0x5586);
L056A:
    /* 056A  sti */
    ;
L056B:
    /* 056B  mov     ax,cx */
    AX = CX;
L056D:
    /* 056D  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L056F:
    /* 056F  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L0571:
    /* 0571  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L0573:
    /* 0573  sub     ax,cx */
    AX = (uint16_t)(AX - CX);
L0575:
    /* 0575  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L0577:
    /* 0577  push    ax */
    push16(AX);
L0578:
    /* 0578  push    cx */
    push16(CX);
L0579:
    /* 0579  mov     bx,es */
    BX = asm_es;
L057B:
    /* 057B  mov     cx,54h */
    CX = 0x54;
L057E:
    /* 057E  mov     ax,cs */
    AX = (uint16_t)(0x0090 + PORT_LOAD_SEG);
L0580:
    /* 0580  mov     es,ax */
    SET_ES(AX);
L0582:
    /* 0582  mov     di,offset _seg003_3CE */
    DI = 0x3CE;
L0585:
    /* 0585  mov     si,659h */
    SI = 0x659;
L0588:
    /* 0588  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L058A:
    /* 058A  mov     es,bx */
    SET_ES(BX);
L058C:
    /* 058C  pop     cx */
    CX = pop16();
L058D:
    /* 058D  pop     ax */
    AX = pop16();
L058E:
    /* 058E  add     ax,659h */
    AX = (uint16_t)(AX + 0x659);
L0591:
    /* 0591  mov     word ptr ds:[7ABh],ax */
    ww(pDS, 0x7AB, AX);
L0594:
    /* 0594  sub     ax,0Eh */
    AX = (uint16_t)(AX - 0xE);
L0597:
    /* 0597  mov     word ptr ds:[7A9h],ax */
    ww(pDS, 0x7A9, AX);
L059A:
    /* 059A  inc     cx */
    CX = (uint16_t)(CX + 1);
L059B:
    /* 059B  mov     word ptr ds:[7D1h],cx */
    ww(pDS, 0x7D1, CX);
L059F:
    /* 059F  mov     word ptr cs:_seg003_3B4,cx */
    ww(CODE003, 0x3B4, CX);
L05A4:
    /* 05A4  mov     si,word ptr es:[0B07Ch] */
    SI = rw(pES, 0xB07C);
L05A9:
    /* 05A9  lods    word ptr es:[si] */
    AX = rw(pES, SI); SI = (uint16_t)(SI + STEP(2));
L05AB:
    /* 05AB  dec     ax */
    AX = (uint16_t)(AX - 1);
L05AC:
    /* 05AC  mov     word ptr ds:[7ADh],ax */
    ww(pDS, 0x7AD, AX);
L05AF:
    /* 05AF  add     si,2 */
    SI = (uint16_t)(SI + 0x2);
L05B2:
    /* 05B2  lods    word ptr es:[si] */
    AX = rw(pES, SI); SI = (uint16_t)(SI + STEP(2));
L05B4:
    /* 05B4  mov     word ptr ds:[7AFh],ax */
    ww(pDS, 0x7AF, AX);
L05B7:
    /* 05B7  lods    word ptr es:[si] */
    AX = rw(pES, SI); SI = (uint16_t)(SI + STEP(2));
L05B9:
    /* 05B9  mov     word ptr cs:L0830,ax */
    ww(CODE003, 0x830, AX);
L05BD:
    /* 05BD  mov     dx,8001h */
    DX = 0x8001;
L05C0:
    /* 05C0  mov     bp,7FFFh */
    BP = 0x7FFF;
L05C3:
    /* 05C3  mov     si,659h */
    SI = 0x659;
L05C6:
    /* 05C6  mov     di,si */
    DI = SI;
L05C8:
    /* 05C8  dec     cx */
    CX = (uint16_t)(CX - 1);
L05C9: /* L05C9 */
    /* 05C9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L05CA:
    /* 05CA  cmp     dx,ax */
    sub16(DX, AX, 0);
L05CC:
    /* 05CC  jge     L05D2 */
    if (SF == OF) goto L05D2;
L05CE:
    /* 05CE  mov     bx,si */
    BX = SI;
L05D0:
    /* 05D0  mov     dx,ax */
    DX = AX;
L05D2: /* L05D2 */
    /* 05D2  cmp     bp,ax */
    sub16(BP, AX, 0);
L05D4:
    /* 05D4  jle     L05DC */
    if (ZF || SF != OF) goto L05DC;
L05D6:
    /* 05D6  mov     bp,ax */
    BP = AX;
L05D8:
    /* 05D8  mov     word ptr ds:[7D8h],si */
    ww(pDS, 0x7D8, SI);
L05DC: /* L05DC */
    /* 05DC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L05DD:
    /* 05DD  cmp     ax,word ptr [di+2] */
    sub16(AX, rw(pDS, DI + 0x2), 0);
L05E0:
    /* 05E0  jle     L05E5 */
    if (ZF || SF != OF) goto L05E5;
L05E2:
    /* 05E2  lea     di,[si-4] */
    DI = (uint16_t)(SI + 0xFFFC);
L05E5: /* L05E5 */
    /* 05E5  add     si,0Ah */
    SI = (uint16_t)(SI + 0xA);
L05E8:
    /* 05E8  loop    L05C9 */
    if (--CX) goto L05C9;
L05EA:
    /* 05EA  mov     word ptr ds:[932h],dx */
    ww(pDS, 0x932, DX);
L05EE:
    /* 05EE  mov     word ptr cs:_seg003_3B0,dx */
    ww(CODE003, 0x3B0, DX);
L05F3:
    /* 05F3  mov     word ptr cs:_seg003_3B2,bp */
    ww(CODE003, 0x3B2, BP);
L05F8:
    /* 05F8  mov     word ptr cs:_seg003_3B6,di */
    ww(CODE003, 0x3B6, DI);
L05FD:
    /* 05FD  mov     word ptr ds:[7B1h],di */
    ww(pDS, 0x7B1, DI);
L0601:
    /* 0601  mov     si,di */
    SI = DI;
L0603:
    /* 0603  mov     word ptr ds:[7B3h],si */
    ww(pDS, 0x7B3, SI);
L0607:
    /* 0607  mov     word ptr ds:[7B5h],si */
    ww(pDS, 0x7B5, SI);
L060B:
    /* 060B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L060C:
    /* 060C  mov     word ptr cs:_seg003_3B8,ax */
    ww(CODE003, 0x3B8, AX);
L0610:
    /* 0610  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0611:
    /* 0611  mov     word ptr ds:[7CFh],ax */
    ww(pDS, 0x7CF, AX);
L0614:
    /* 0614  mov     word ptr cs:_seg003_3BA,ax */
    ww(CODE003, 0x3BA, AX);
L0618:
    /* 0618  mov     word ptr cs:_seg003_3BE,1234h */
    ww(CODE003, 0x3BE, 0x1234);
L061F:
    /* 061F  mov     word ptr cs:_seg003_3C0,1234h */
    ww(CODE003, 0x3C0, 0x1234);
L0626:
    /* 0626  mov     word ptr cs:_seg003_3C4,1234h */
    ww(CODE003, 0x3C4, 0x1234);
L062D:
    /* 062D  mov     word ptr cs:_seg003_3C2,1234h */
    ww(CODE003, 0x3C2, 0x1234);
L0634:
    /* 0634  mov     bp,word ptr ds:[7D8h] */
    BP = rw(pDS, 0x7D8);
L0638:
    /* 0638  mov     ax,word ptr [bx+0Ah] */
    AX = rw(pDS, BX + 0xA);
L063B:
    /* 063B  mov     cx,word ptr [bp+0Ah] */
    CX = rw(pSS, BP + 0xA);
L063E:
    /* 063E  sub     ax,cx */
    AX = (uint16_t)(AX - CX);
L0640:
    /* 0640  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0641:
    /* 0641  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L0643:
    /* 0643  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L0645:
    /* 0645  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0646:
    /* 0646  shl     ax,1 */
    AX = shl16(AX, 1);
L0648:
    /* 0648  rcl     dx,1 */
    DX = rcl16(DX, 1);
L064A:
    /* 064A  shl     ax,1 */
    AX = shl16(AX, 1);
L064C:
    /* 064C  rcl     dx,1 */
    DX = rcl16(DX, 1);
L064E:
    /* 064E  cmp     cx,word ptr [bx+0Ah] */
    sub16(CX, rw(pDS, BX + 0xA), 0);
L0651:
    /* 0651  jle     L0656 */
    if (ZF || SF != OF) goto L0656;
L0653:
    /* 0653  mov     cx,word ptr [bx+0Ah] */
    CX = rw(pDS, BX + 0xA);
L0656: /* L0656 */
    /* 0656  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x0656, 2)) != 0) return c;
L0658:
    /* 0658  cmp     ax,2 */
    sub16(AX, 0x2, 0);
L065B:
    /* 065B  mov     word ptr cs:_seg003_3BE,ax */
    ww(CODE003, 0x3BE, AX);
L065F:
    /* 065F  jg      L0691 */
    if (!ZF && SF == OF) goto L0691;
L0661: /* L0661 */
    /* 0661  mov     cx,word ptr [bx-2] */
    CX = rw(pDS, BX + 0xFFFE);
L0664:
    /* 0664  mov     di,word ptr [bp-2] */
    DI = rw(pSS, BP + 0xFFFE);
L0667:
    /* 0667  sub     cx,di */
    CX = (uint16_t)(CX - DI);
L0669:
    /* 0669  add     di,7F2h */
    DI = (uint16_t)(DI + 0x7F2);
L066D:
    /* 066D  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L0670:
    /* 0670  mov     bx,word ptr [bp+2] */
    BX = rw(pSS, BP + 0x2);
L0673:
    /* 0673  sub     ax,bx */
    AX = (uint16_t)(AX - BX);
L0675:
    /* 0675  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0676:
    /* 0676  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L0678:
    /* 0678  jg      L0682 */
    if (!ZF && SF == OF) goto L0682;
L067A:
    /* 067A  mov     byte ptr [di],bh */
    wb(pDS, DI, BH);
L067C:
    /* 067C  mov     byte ptr [di+2],bh */
    wb(pDS, DI + 0x2, BH);
L067F:
    /* 067F  jmp     L07C6 */
    goto L07C6;
L0682: /* L0682 */
    /* 0682  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x0682, 2)) != 0) return c;
L0684:
    /* 0684  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L0685: /* L0685 */
    /* 0685  mov     byte ptr [di],ah */
    wb(pDS, DI, AH);
L0687:
    /* 0687  inc     di */
    DI = (uint16_t)(DI + 1);
L0688:
    /* 0688  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L068A:
    /* 068A  loop    L0685 */
    if (--CX) goto L0685;
L068C:
    /* 068C  mov     byte ptr [di],ah */
    wb(pDS, DI, AH);
L068E:
    /* 068E  jmp     L07C6 */
    goto L07C6;
L0691: /* L0691 */
    /* 0691  mov     cx,word ptr [bx+2] */
    CX = rw(pDS, BX + 0x2);
L0694:
    /* 0694  sub     cx,word ptr [bp+2] */
    CX = sub16(CX, rw(pSS, BP + 0x2), 0);
L0697:
    /* 0697  je      L0661 */
    if (ZF) goto L0661;
L0699:
    /* 0699  mov     cl,ch */
    CL = CH;
L069B:
    /* 069B  xor     ch,ch */
    CH = (uint8_t)(CH ^ CH);
L069D:
    /* 069D  mov     ax,word ptr es:[26B4h] */
    AX = rw(pES, 0x26B4);
L06A1:
    /* 06A1  mov     word ptr ds:[7EEh],ax */
    ww(pDS, 0x7EE, AX);
L06A4:
    /* 06A4  mov     ax,word ptr es:[26B2h] */
    AX = rw(pES, 0x26B2);
L06A8:
    /* 06A8  mov     word ptr ds:[7F0h],ax */
    ww(pDS, 0x7F0, AX);
L06AB:
    /* 06AB  mov     ax,word ptr [bp-2] */
    AX = rw(pSS, BP + 0xFFFE);
L06AE:
    /* 06AE  mov     word ptr ds:[7DAh],ax */
    ww(pDS, 0x7DA, AX);
L06B1:
    /* 06B1  mov     ax,word ptr [bp+0Ah] */
    AX = rw(pSS, BP + 0xA);
L06B4:
    /* 06B4  mov     word ptr ds:[7DEh],ax */
    ww(pDS, 0x7DE, AX);
L06B7:
    /* 06B7  mov     word ptr ds:[7DCh],0 */
    ww(pDS, 0x7DC, 0x0);
L06BD:
    /* 06BD  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L06C0:
    /* 06C0  mov     word ptr ds:[7E2h],ax */
    ww(pDS, 0x7E2, AX);
L06C3:
    /* 06C3  mov     word ptr ds:[7E0h],0 */
    ww(pDS, 0x7E0, 0x0);
L06C9:
    /* 06C9  mov     ax,word ptr [bx+6] */
    AX = rw(pDS, BX + 0x6);
L06CC:
    /* 06CC  sub     ax,word ptr [bp+6] */
    AX = (uint16_t)(AX - rw(pSS, BP + 0x6));
L06CF:
    /* 06CF  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L06D0:
    /* 06D0  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x06D0, 2)) != 0) return c;
L06D2:
    /* 06D2  mov     word ptr ds:[7E6h],ax */
    ww(pDS, 0x7E6, AX);
L06D5:
    /* 06D5  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L06D7:
    /* 06D7  sar     dx,1 */
    DX = sar16(DX, 1);
L06D9:
    /* 06D9  rcr     ax,1 */
    AX = rcr16(AX, 1);
L06DB:
    /* 06DB  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x06DB, 2)) != 0) return c;
L06DD:
    /* 06DD  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L06DE:
    /* 06DE  shl     ax,1 */
    AX = shl16(AX, 1);
L06E0:
    /* 06E0  rcl     dx,1 */
    DX = rcl16(DX, 1);
L06E2:
    /* 06E2  mov     word ptr ds:[7E4h],ax */
    ww(pDS, 0x7E4, AX);
L06E5:
    /* 06E5  add     word ptr ds:[7E6h],dx */
    ww(pDS, 0x7E6, (uint16_t)(rw(pDS, 0x7E6) + DX));
L06E9:
    /* 06E9  mov     ax,word ptr [bx+0Ah] */
    AX = rw(pDS, BX + 0xA);
L06EC:
    /* 06EC  sub     ax,word ptr [bp+0Ah] */
    AX = (uint16_t)(AX - rw(pSS, BP + 0xA));
L06EF:
    /* 06EF  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L06F0:
    /* 06F0  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x06F0, 2)) != 0) return c;
L06F2:
    /* 06F2  mov     word ptr ds:[7EAh],ax */
    ww(pDS, 0x7EA, AX);
L06F5:
    /* 06F5  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L06F7:
    /* 06F7  sar     dx,1 */
    DX = sar16(DX, 1);
L06F9:
    /* 06F9  rcr     ax,1 */
    AX = rcr16(AX, 1);
L06FB:
    /* 06FB  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x06FB, 2)) != 0) return c;
L06FD:
    /* 06FD  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L06FE:
    /* 06FE  shl     ax,1 */
    AX = shl16(AX, 1);
L0700:
    /* 0700  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0702:
    /* 0702  mov     word ptr ds:[7E8h],ax */
    ww(pDS, 0x7E8, AX);
L0705:
    /* 0705  add     word ptr ds:[7EAh],dx */
    ww(pDS, 0x7EA, (uint16_t)(rw(pDS, 0x7EA) + DX));
L0709:
    /* 0709  mov     ax,word ptr [bp+2] */
    AX = rw(pSS, BP + 0x2);
L070C:
    /* 070C  mov     word ptr ds:[7ECh],ax */
    ww(pDS, 0x7EC, AX);
L070F:
    /* 070F  mov     bp,word ptr ds:[7DEh] */
    BP = rw(pDS, 0x7DE);
L0713:
    /* 0713  mov     si,word ptr ds:[7DCh] */
    SI = rw(pDS, 0x7DC);
L0717:
    /* 0717  mov     bx,word ptr [bx+2] */
    BX = rw(pDS, BX + 0x2);
L071A:
    /* 071A  mov     bl,byte ptr ds:[7EDh] */
    BL = rb(pDS, 0x7ED);
L071E:
    /* 071E  mov     es,word ptr ds:[7D4h] */
    SET_ES(rw(pDS, 0x7D4));
L0722:
    /* 0722  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L0724:
    /* 0724  mov     word ptr cs:_seg003_3C0,cx */
    ww(CODE003, 0x3C0, CX);
L0729:
    /* 0729  mov     word ptr cs:_seg003_3C2,bx */
    ww(CODE003, 0x3C2, BX);
L072E:
    /* 072E  jle     L07A6 */
    if (ZF || SF != OF) goto L07A6;
L0730: /* L0730 */
    /* 0730  add     si,word ptr ds:[7E8h] */
    SI = add16(SI, rw(pDS, 0x7E8), 0);
L0734:
    /* 0734  adc     bp,word ptr ds:[7EAh] */
    BP = (uint16_t)(BP + rw(pDS, 0x7EA) + CF);
L0738:
    /* 0738  mov     ax,word ptr ds:[7E2h] */
    AX = rw(pDS, 0x7E2);
L073B:
    /* 073B  mov     dx,word ptr ds:[7E0h] */
    DX = rw(pDS, 0x7E0);
L073F:
    /* 073F  mov     word ptr cs:_seg003_3C6,ax */
    ww(CODE003, 0x3C6, AX);
L0743:
    /* 0743  mov     word ptr cs:_seg003_3C8,dx */
    ww(CODE003, 0x3C8, DX);
L0748:
    /* 0748  add     dx,word ptr ds:[7E4h] */
    DX = add16(DX, rw(pDS, 0x7E4), 0);
L074C:
    /* 074C  adc     ax,word ptr ds:[7E6h] */
    AX = (uint16_t)(AX + rw(pDS, 0x7E6) + CF);
L0750:
    /* 0750  mov     word ptr ds:[7E2h],ax */
    ww(pDS, 0x7E2, AX);
L0753:
    /* 0753  mov     word ptr ds:[7E0h],dx */
    ww(pDS, 0x7E0, DX);
L0757:
    /* 0757  imul    word ptr ds:[7F0h] */
    imul16(rw(pDS, 0x7F0));
L075B:
    /* 075B  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x0090, 0x075B, 2)) != 0) return c;
L075D:
    /* 075D  add     ax,word ptr ds:[7EEh] */
    AX = add16(AX, rw(pDS, 0x7EE), 0);
L0761:
    /* 0761  js      L07A2 */
    if (SF) goto L07A2;
L0763:
    /* 0763  mov     di,word ptr ds:[7DAh] */
    DI = rw(pDS, 0x7DA);
L0767:
    /* 0767  cmp     ax,word ptr ds:[932h] */
    sub16(AX, rw(pDS, 0x932), 0);
L076B:
    /* 076B  jle     L0781 */
    if (ZF || SF != OF) goto L0781;
L076D:
    /* 076D  mov     ax,word ptr ds:[932h] */
    AX = rw(pDS, 0x932);
L0770:
    /* 0770  sub     ax,di */
    AX = sub16(AX, DI, 0);
L0772:
    /* 0772  jl      L077F */
    if (SF != OF) goto L077F;
L0774:
    /* 0774  add     di,7F2h */
    DI = (uint16_t)(DI + 0x7F2);
L0778:
    /* 0778  inc     ax */
    AX = (uint16_t)(AX + 1);
L0779:
    /* 0779  mov     cx,ax */
    CX = AX;
L077B:
    /* 077B  mov     al,bl */
    AL = BL;
L077D:
    /* 077D  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L077F: /* L077F */
    /* 077F  jmp     short L07C6 */
    goto L07C6;
L0781: /* L0781 */
    /* 0781  mov     word ptr ds:[7DAh],ax */
    ww(pDS, 0x7DA, AX);
L0784:
    /* 0784  sub     ax,di */
    AX = sub16(AX, DI, 0);
L0786:
    /* 0786  jl      L07A2 */
    if (SF != OF) goto L07A2;
L0788:
    /* 0788  inc     ax */
    AX = (uint16_t)(AX + 1);
L0789:
    /* 0789  mov     word ptr cs:_seg003_3C4,ax */
    ww(CODE003, 0x3C4, AX);
L078D:
    /* 078D  add     di,7F2h */
    DI = (uint16_t)(DI + 0x7F2);
L0791:
    /* 0791  mov     cx,ax */
    CX = AX;
L0793:
    /* 0793  mov     al,bl */
    AL = BL;
L0795:
    /* 0795  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L0797:
    /* 0797  inc     bl */
    BL = (uint8_t)(BL + 1);
L0799:
    /* 0799  mov     byte ptr [di-1],bl */
    wb(pDS, DI + 0xFFFF, BL);
L079C: /* L079C */
    /* 079C  cmp     bl,bh */
    sub8(BL, BH, 0);
L079E:
    /* 079E  jl      L0730 */
    if (SF != OF) goto L0730;
L07A0:
    /* 07A0  jmp     short L07C6 */
    goto L07C6;
L07A2: /* L07A2 */
    /* 07A2  inc     bl */
    BL = inc8(BL);
L07A4:
    /* 07A4  jne     L079C */
    if (!ZF) goto L079C;
L07A6: /* L07A6 */
    /* 07A6  int     2 */
    asm_halt_at(0x0090, 0x07A6, "int 2h, the debugger break");
L07A8:
    /* 07A8  mov     ax,word ptr ds:[932h] */
    AX = rw(pDS, 0x932);
L07AB:
    /* 07AB  mov     di,word ptr ds:[7DAh] */
    DI = rw(pDS, 0x7DA);
L07AF:
    /* 07AF  sub     ax,di */
    AX = sub16(AX, DI, 0);
L07B1:
    /* 07B1  jge     L07BC */
    if (SF == OF) goto L07BC;
L07B3:
    /* 07B3  mov     di,word ptr ds:[932h] */
    DI = rw(pDS, 0x932);
L07B7:
    /* 07B7  mov     ax,word ptr ds:[7DAh] */
    AX = rw(pDS, 0x7DA);
L07BA:
    /* 07BA  sub     ax,di */
    AX = (uint16_t)(AX - DI);
L07BC: /* L07BC */
    /* 07BC  add     di,7F2h */
    DI = (uint16_t)(DI + 0x7F2);
L07C0:
    /* 07C0  mov     cx,ax */
    CX = AX;
L07C2:
    /* 07C2  mov     al,bl */
    AL = BL;
L07C4:
    /* 07C4  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L07C6: /* L07C6 */
    /* 07C6  call    _seg003_4B9 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x04B9), 0x07C9)) != 0) return c;
L07C9:
    /* 07C9  jne     L07CE */
    if (!ZF) goto L07CE;
L07CB:
    /* 07CB  jmp     L0898 */
    goto L0898;
L07CE: /* L07CE */
    /* 07CE  call    _seg003_422 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x0422), 0x07D1)) != 0) return c;
L07D1:
    /* 07D1  jne     L07D6 */
    if (!ZF) goto L07D6;
L07D3:
    /* 07D3  jmp     L0898 */
    goto L0898;
L07D6: /* L07D6 */
    /* 07D6  mov     es,word ptr ds:[95Ah] */
    SET_ES(rw(pDS, 0x95A));
L07DA: /* L07DA */
    /* 07DA  mov     di,word ptr ds:[7CFh] */
    DI = rw(pDS, 0x7CF);
L07DE:
    /* 07DE  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L07E0:
    /* 07E0  mov     di,word ptr [di+95Eh] */
    DI = rw(pDS, DI + 0x95E);
L07E4:
    /* 07E4  mov     si,word ptr ds:[7B7h] */
    SI = rw(pDS, 0x7B7);
L07E8:
    /* 07E8  add     di,si */
    DI = (uint16_t)(DI + SI);
L07EA:
    /* 07EA  mov     cx,word ptr ds:[7C3h] */
    CX = rw(pDS, 0x7C3);
L07EE:
    /* 07EE  sub     cx,si */
    CX = sub16(CX, SI, 0);
L07F0:
    /* 07F0  je      L0818 */
    if (ZF) goto L0818;
L07F2:
    /* 07F2  mov     bp,word ptr ds:[7BDh] */
    BP = rw(pDS, 0x7BD);
L07F6:
    /* 07F6  mov     ax,word ptr ds:[7C9h] */
    AX = rw(pDS, 0x7C9);
L07F9:
    /* 07F9  sub     ax,bp */
    AX = (uint16_t)(AX - BP);
L07FB:
    /* 07FB  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L07FC:
    /* 07FC  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x07FC, 2)) != 0) return c;
L07FE:
    /* 07FE  mov     word ptr cs:L083C,ax */
    ww(CODE003, 0x83C, AX);
L0802:
    /* 0802  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L0804:
    /* 0804  sar     dx,1 */
    DX = sar16(DX, 1);
L0806:
    /* 0806  rcr     ax,1 */
    AX = rcr16(AX, 1);
L0808:
    /* 0808  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x0808, 2)) != 0) return c;
L080A:
    /* 080A  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L080B:
    /* 080B  shl     ax,1 */
    AX = shl16(AX, 1);
L080D:
    /* 080D  rcl     dx,1 */
    DX = rcl16(DX, 1);
L080F:
    /* 080F  mov     word ptr cs:L0838,ax */
    ww(CODE003, 0x838, AX);
L0813:
    /* 0813  add     word ptr cs:L083C,dx */
    ww(CODE003, 0x83C, (uint16_t)(rw(CODE003, 0x83C) + DX));
L0818: /* L0818 */
    /* 0818  inc     cx */
    CX = inc16(CX);
L0819:
    /* 0819  jg      L081E */
    if (!ZF && SF == OF) goto L081E;
L081B:
    /* 081B  jmp     L08E0 */
    goto L08E0;
L081E: /* L081E */
    /* 081E  mov     dx,bp */
    DX = BP;
L0820:
    /* 0820  add     si,7F2h */
    SI = (uint16_t)(SI + 0x7F2);
L0824:
    /* 0824  mov     bp,word ptr ds:[7C1h] */
    BP = rw(pDS, 0x7C1);
L0828:
    /* 0828  mov     ds,word ptr ds:[7AFh] */
    SET_DS(rw(pDS, 0x7AF));
L082C: /* L082C */
    /* 082C  mov     bx,dx */
    BX = DX;
L082E:
    /* 082E  and     bx,1234h */
    BX = (uint16_t)(BX & rw(CODE003, 0x0830));
L0832:
    /* 0832  lods    byte ptr ss:[si] */
    AL = rb(pSS, SI); SI = (uint16_t)(SI + STEP(1));
L0834:
    /* 0834  xlatb */
    AL = rb(pDS, BX + AL);
L0835:
    /* 0835  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0836:
    /* 0836  add     bp,1234h */
    BP = add16(BP, rw(CODE003, 0x0838), 0);
L083A:
    /* 083A  adc     dx,1234h */
    DX = (uint16_t)(DX + rw(CODE003, 0x083C) + CF);
L083E:
    /* 083E  loop    L082C */
    if (--CX) goto L082C;
L0840:
    /* 0840  mov     ds,word ptr ss:[7D4h] */
    SET_DS(rw(pSS, 0x7D4));
L0845: /* L0845 */
    /* 0845  dec     word ptr ds:[7CFh] */
    ww(pDS, 0x7CF, (uint16_t)(rw(pDS, 0x7CF) - 1));
L0849:
    /* 0849  add     word ptr ds:[7C1h],1234h */
    ww(pDS, 0x7C1, add16(rw(pDS, 0x7C1), rw(CODE003, 0x084D), 0));
L084F:
    /* 084F  adc     word ptr ds:[7BDh],1234 */
    ww(pDS, 0x7BD, (uint16_t)(rw(pDS, 0x7BD) + rw(CODE003, 0x0853) + CF));
L0855:
    /* 0855  add     word ptr ds:[7B9h],1234h */
    ww(pDS, 0x7B9, add16(rw(pDS, 0x7B9), rw(CODE003, 0x0859), 0));
L085B:
    /* 085B  adc     word ptr ds:[7B7h],1234h */
    ww(pDS, 0x7B7, (uint16_t)(rw(pDS, 0x7B7) + rw(CODE003, 0x085F) + CF));
L0861:
    /* 0861  add     word ptr ds:[7CDh],1234h */
    ww(pDS, 0x7CD, add16(rw(pDS, 0x7CD), rw(CODE003, 0x0865), 0));
L0867:
    /* 0867  adc     word ptr ds:[7C9h],1234 */
    ww(pDS, 0x7C9, (uint16_t)(rw(pDS, 0x7C9) + rw(CODE003, 0x086B) + CF));
L086D:
    /* 086D  add     word ptr ds:[7C5h],1234h */
    ww(pDS, 0x7C5, add16(rw(pDS, 0x7C5), rw(CODE003, 0x0871), 0));
L0873:
    /* 0873  adc     word ptr ds:[7C3h],1234h */
    ww(pDS, 0x7C3, (uint16_t)(rw(pDS, 0x7C3) + rw(CODE003, 0x0877) + CF));
L0879:
    /* 0879  mov     cx,word ptr ds:[7CFh] */
    CX = rw(pDS, 0x7CF);
L087D:
    /* 087D  cmp     cx,1234h */
    sub16(CX, rw(CODE003, 0x087F), 0);
L0881:
    /* 0881  jle     L08D1 */
    if (ZF || SF != OF) goto L08D1;
L0883: /* L0883 */
    /* 0883  cmp     cx,1234h */
    sub16(CX, rw(CODE003, 0x0885), 0);
L0887:
    /* 0887  jle     L088C */
    if (ZF || SF != OF) goto L088C;
L0889:
    /* 0889  jmp     L07DA */
    goto L07DA;
L088C: /* L088C */
    /* 088C  mov     bx,word ptr ds:[7B5h] */
    BX = rw(pDS, 0x7B5);
L0890:
    /* 0890  call    L043A */
    if ((c = asm_call(ASM_JMP(0x0090, 0x043A), 0x0893)) != 0) return c;
L0893:
    /* 0893  je      L0898 */
    if (ZF) goto L0898;
L0895:
    /* 0895  jmp     L07DA */
    goto L07DA;
L0898: /* L0898 */
    /* 0898  cli */
    ;
L0899:
    /* 0899  mov     ss,word ptr cs:L08E3 */
    SET_SS(rw(CODE003, 0x8E3));
L089E:
    /* 089E  mov     sp,word ptr cs:L08E5 */
    SP = rw(CODE003, 0x8E5);
L08A3:
    /* 08A3  sti */
    ;
L08A4:
    /* 08A4  pop     ds */
    SET_DS(pop16());
L08A5:
    /* 08A5  pop     es */
    SET_ES(pop16());
L08A6:
    /* 08A6  push    es */
    push16(asm_es);
L08A7:
    /* 08A7  push    di */
    push16(DI);
L08A8:
    /* 08A8  push    ax */
    push16(AX);
L08A9:
    /* 08A9  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L08AC:
    /* 08AC  mov     es,ax */
    SET_ES(AX);
L08AE:
    /* 08AE  mov     ax,0 */
    AX = 0x0;
L08B1:
    /* 08B1  mov     cx,0FFh */
    CX = 0xFF;
L08B4:
    /* 08B4  mov     di,8 */
    DI = 0x8;
L08B7:
    /* 08B7  repe scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (!ZF) break; }
L08B9:
    /* 08B9  jcxz    L08CD */
    if (!CX) goto L08CD;
L08BB:
    /* 08BB  mov     al,byte ptr es:[658h] */
    AL = rb(pES, 0x658);
L08BF:
    /* 08BF  test    al,0FFh */
    logic8((uint8_t)(AL & 0xFF));
L08C1:
    /* 08C1  jne     L08C9 */
    if (!ZF) goto L08C9;
L08C3:
    /* 08C3  mov     al,0A0h */
    AL = 0xA0;
L08C5:
    /* 08C5  mov     byte ptr es:[658h],al */
    wb(pES, 0x658, AL);
L08C9: /* L08C9 */
    /* 08C9  mov     byte ptr es:[PEN_COLOR],al */
    wb(pES, 0x410F, AL);
L08CD: /* L08CD */
    /* 08CD  pop     ax */
    AX = pop16();
L08CE:
    /* 08CE  pop     di */
    DI = pop16();
L08CF:
    /* 08CF  pop     es */
    SET_ES(pop16());
L08D0:
    /* 08D0  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;
L08D1: /* L08D1 */
    /* 08D1  mov     bx,word ptr ds:[7B3h] */
    BX = rw(pDS, 0x7B3);
L08D5:
    /* 08D5  call    L04D1 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x04D1), 0x08D8)) != 0) return c;
L08D8:
    /* 08D8  mov     cx,word ptr ds:[7CFh] */
    CX = rw(pDS, 0x7CF);
L08DC:
    /* 08DC  jne     L0883 */
    if (!ZF) goto L0883;
L08DE:
    /* 08DE  jmp     L0898 */
    goto L0898;
L08E0: /* L08E0 */
    /* 08E0  jmp     L0845 */
    goto L0845;
}
