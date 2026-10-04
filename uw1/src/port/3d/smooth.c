/* smooth.c: replaces src/3d/SMOOTH.ASM (seg004_260, 0260..0A0B of its
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

uint32_t asm_mod_SMOOTH(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x0260: goto L0260;
    case 0x0266: goto L0266;
    case 0x0268: goto L0268;
    case 0x026A: goto L026A;
    case 0x0270: goto L0270;
    case 0x0275: goto L0275;
    case 0x0277: goto L0277;
    case 0x027A: goto L027A;
    case 0x027D: goto L027D;
    case 0x0281: goto L0281;
    case 0x0286: goto L0286;
    case 0x0288: goto L0288;
    case 0x0289: goto L0289;
    case 0x028C: goto L028C;
    case 0x028E: goto L028E;
    case 0x0291: goto L0291;
    case 0x0294: goto L0294;
    case 0x0298: goto L0298;
    case 0x029C: goto L029C;
    case 0x029D: goto L029D;
    case 0x02A0: goto L02A0;
    case 0x02A2: goto L02A2;
    case 0x02A4: goto L02A4;
    case 0x02A7: goto L02A7;
    case 0x02A8: goto L02A8;
    case 0x02A9: goto L02A9;
    case 0x02AD: goto L02AD;
    case 0x02AF: goto L02AF;
    case 0x02B2: goto L02B2;
    case 0x02B3: goto L02B3;
    case 0x02B6: goto L02B6;
    case 0x02B9: goto L02B9;
    case 0x02BC: goto L02BC;
    case 0x02BF: goto L02BF;
    case 0x02C3: goto L02C3;
    case 0x02C5: goto L02C5;
    case 0x02C9: goto L02C9;
    case 0x02CD: goto L02CD;
    case 0x02CF: goto L02CF;
    case 0x02D1: goto L02D1;
    case 0x02D3: goto L02D3;
    case 0x02D5: goto L02D5;
    case 0x02D9: goto L02D9;
    case 0x02DD: goto L02DD;
    case 0x02E2: goto L02E2;
    case 0x02E4: goto L02E4;
    case 0x02E6: goto L02E6;
    case 0x02E7: goto L02E7;
    case 0x02E8: goto L02E8;
    case 0x02E9: goto L02E9;
    case 0x02ED: goto L02ED;
    case 0x02EF: goto L02EF;
    case 0x02F0: goto L02F0;
    case 0x02F7: goto L02F7;
    case 0x02FA: goto L02FA;
    case 0x02FC: goto L02FC;
    case 0x02FF: goto L02FF;
    case 0x0302: goto L0302;
    case 0x0306: goto L0306;
    case 0x030A: goto L030A;
    case 0x030B: goto L030B;
    case 0x030E: goto L030E;
    case 0x0310: goto L0310;
    case 0x0312: goto L0312;
    case 0x0315: goto L0315;
    case 0x0317: goto L0317;
    case 0x031A: goto L031A;
    case 0x031B: goto L031B;
    case 0x031C: goto L031C;
    case 0x0320: goto L0320;
    case 0x0322: goto L0322;
    case 0x0325: goto L0325;
    case 0x0327: goto L0327;
    case 0x032A: goto L032A;
    case 0x032B: goto L032B;
    case 0x032E: goto L032E;
    case 0x0331: goto L0331;
    case 0x0334: goto L0334;
    case 0x0337: goto L0337;
    case 0x033B: goto L033B;
    case 0x033D: goto L033D;
    case 0x0341: goto L0341;
    case 0x0345: goto L0345;
    case 0x0347: goto L0347;
    case 0x0349: goto L0349;
    case 0x034B: goto L034B;
    case 0x034F: goto L034F;
    case 0x0353: goto L0353;
    case 0x0358: goto L0358;
    case 0x035A: goto L035A;
    case 0x035C: goto L035C;
    case 0x035D: goto L035D;
    case 0x035E: goto L035E;
    case 0x035F: goto L035F;
    case 0x0363: goto L0363;
    case 0x036A: goto L036A;
    case 0x036F: goto L036F;
    case 0x0371: goto L0371;
    case 0x0372: goto L0372;
    case 0x0375: goto L0375;
    case 0x0376: goto L0376;
    case 0x037B: goto L037B;
    case 0x037D: goto L037D;
    case 0x0382: goto L0382;
    case 0x0384: goto L0384;
    case 0x0386: goto L0386;
    case 0x0389: goto L0389;
    case 0x038C: goto L038C;
    case 0x0391: goto L0391;
    case 0x0393: goto L0393;
    case 0x0394: goto L0394;
    case 0x0397: goto L0397;
    case 0x0398: goto L0398;
    case 0x039D: goto L039D;
    case 0x039F: goto L039F;
    case 0x03A4: goto L03A4;
    case 0x03A6: goto L03A6;
    case 0x03A8: goto L03A8;
    case 0x03AB: goto L03AB;
    case 0x03AE: goto L03AE;
    case 0x03B3: goto L03B3;
    case 0x03B5: goto L03B5;
    case 0x03B6: goto L03B6;
    case 0x03B9: goto L03B9;
    case 0x03BA: goto L03BA;
    case 0x03BF: goto L03BF;
    case 0x03C1: goto L03C1;
    case 0x03C6: goto L03C6;
    case 0x03C8: goto L03C8;
    case 0x03CA: goto L03CA;
    case 0x03CD: goto L03CD;
    case 0x03D0: goto L03D0;
    case 0x03D5: goto L03D5;
    case 0x03D7: goto L03D7;
    case 0x03D8: goto L03D8;
    case 0x03DB: goto L03DB;
    case 0x03DC: goto L03DC;
    case 0x03E1: goto L03E1;
    case 0x03E3: goto L03E3;
    case 0x03E8: goto L03E8;
    case 0x03EA: goto L03EA;
    case 0x03EC: goto L03EC;
    case 0x03EF: goto L03EF;
    case 0x03F2: goto L03F2;
    case 0x03F3: goto L03F3;
    case 0x03F4: goto L03F4;
    case 0x03F8: goto L03F8;
    case 0x03F9: goto L03F9;
    case 0x03FC: goto L03FC;
    case 0x0403: goto L0403;
    case 0x0405: goto L0405;
    case 0x0407: goto L0407;
    case 0x040C: goto L040C;
    case 0x040E: goto L040E;
    case 0x0411: goto L0411;
    case 0x0416: goto L0416;
    case 0x0418: goto L0418;
    case 0x041D: goto L041D;
    case 0x041F: goto L041F;
    case 0x0422: goto L0422;
    case 0x0427: goto L0427;
    case 0x0429: goto L0429;
    case 0x042E: goto L042E;
    case 0x0430: goto L0430;
    case 0x0433: goto L0433;
    case 0x0438: goto L0438;
    case 0x043A: goto L043A;
    case 0x043F: goto L043F;
    case 0x0441: goto L0441;
    case 0x0444: goto L0444;
    case 0x0449: goto L0449;
    case 0x044B: goto L044B;
    case 0x044E: goto L044E;
    case 0x044F: goto L044F;
    case 0x0452: goto L0452;
    case 0x0454: goto L0454;
    case 0x0456: goto L0456;
    case 0x0457: goto L0457;
    case 0x0458: goto L0458;
    case 0x045C: goto L045C;
    case 0x0461: goto L0461;
    case 0x0466: goto L0466;
    case 0x046A: goto L046A;
    case 0x046E: goto L046E;
    case 0x0471: goto L0471;
    case 0x0473: goto L0473;
    case 0x0478: goto L0478;
    case 0x047C: goto L047C;
    case 0x047F: goto L047F;
    case 0x0483: goto L0483;
    case 0x0485: goto L0485;
    case 0x0488: goto L0488;
    case 0x048C: goto L048C;
    case 0x048F: goto L048F;
    case 0x0493: goto L0493;
    case 0x0495: goto L0495;
    case 0x0498: goto L0498;
    case 0x049B: goto L049B;
    case 0x049D: goto L049D;
    case 0x049F: goto L049F;
    case 0x04A3: goto L04A3;
    case 0x04A7: goto L04A7;
    case 0x04AA: goto L04AA;
    case 0x04AC: goto L04AC;
    case 0x04AE: goto L04AE;
    case 0x04B2: goto L04B2;
    case 0x04B4: goto L04B4;
    case 0x04B7: goto L04B7;
    case 0x04BA: goto L04BA;
    case 0x04BC: goto L04BC;
    case 0x04BF: goto L04BF;
    case 0x04C2: goto L04C2;
    case 0x04C5: goto L04C5;
    case 0x04C8: goto L04C8;
    case 0x04C9: goto L04C9;
    case 0x04CB: goto L04CB;
    case 0x04CD: goto L04CD;
    case 0x04D0: goto L04D0;
    case 0x04D4: goto L04D4;
    case 0x04D6: goto L04D6;
    case 0x04D9: goto L04D9;
    case 0x04DB: goto L04DB;
    case 0x04DD: goto L04DD;
    case 0x04DF: goto L04DF;
    case 0x04E1: goto L04E1;
    case 0x04E3: goto L04E3;
    case 0x04E5: goto L04E5;
    case 0x04E6: goto L04E6;
    case 0x04E8: goto L04E8;
    case 0x04EB: goto L04EB;
    case 0x04EC: goto L04EC;
    case 0x04EE: goto L04EE;
    case 0x04F1: goto L04F1;
    case 0x04F4: goto L04F4;
    case 0x04F6: goto L04F6;
    case 0x04F8: goto L04F8;
    case 0x04FA: goto L04FA;
    case 0x04FC: goto L04FC;
    case 0x04FE: goto L04FE;
    case 0x0500: goto L0500;
    case 0x0501: goto L0501;
    case 0x0503: goto L0503;
    case 0x0506: goto L0506;
    case 0x0507: goto L0507;
    case 0x0508: goto L0508;
    case 0x050A: goto L050A;
    case 0x050C: goto L050C;
    case 0x050F: goto L050F;
    case 0x0510: goto L0510;
    case 0x0511: goto L0511;
    case 0x0515: goto L0515;
    case 0x0519: goto L0519;
    case 0x051D: goto L051D;
    case 0x051F: goto L051F;
    case 0x0522: goto L0522;
    case 0x0525: goto L0525;
    case 0x0528: goto L0528;
    case 0x052A: goto L052A;
    case 0x052D: goto L052D;
    case 0x0530: goto L0530;
    case 0x0533: goto L0533;
    case 0x0536: goto L0536;
    case 0x0537: goto L0537;
    case 0x0539: goto L0539;
    case 0x053B: goto L053B;
    case 0x053E: goto L053E;
    case 0x0542: goto L0542;
    case 0x0545: goto L0545;
    case 0x0547: goto L0547;
    case 0x0549: goto L0549;
    case 0x054B: goto L054B;
    case 0x054D: goto L054D;
    case 0x054F: goto L054F;
    case 0x0551: goto L0551;
    case 0x0553: goto L0553;
    case 0x0554: goto L0554;
    case 0x0556: goto L0556;
    case 0x0558: goto L0558;
    case 0x0559: goto L0559;
    case 0x055B: goto L055B;
    case 0x055E: goto L055E;
    case 0x0561: goto L0561;
    case 0x0563: goto L0563;
    case 0x0565: goto L0565;
    case 0x0567: goto L0567;
    case 0x0569: goto L0569;
    case 0x056B: goto L056B;
    case 0x056D: goto L056D;
    case 0x056E: goto L056E;
    case 0x0570: goto L0570;
    case 0x0573: goto L0573;
    case 0x0574: goto L0574;
    case 0x0575: goto L0575;
    case 0x0577: goto L0577;
    case 0x0579: goto L0579;
    case 0x057C: goto L057C;
    case 0x057D: goto L057D;
    case 0x057E: goto L057E;
    case 0x0582: goto L0582;
    case 0x0586: goto L0586;
    case 0x0589: goto L0589;
    case 0x058D: goto L058D;
    case 0x058E: goto L058E;
    case 0x0593: goto L0593;
    case 0x0598: goto L0598;
    case 0x059C: goto L059C;
    case 0x05A0: goto L05A0;
    case 0x05A3: goto L05A3;
    case 0x05A5: goto L05A5;
    case 0x05AA: goto L05AA;
    case 0x05AE: goto L05AE;
    case 0x05B1: goto L05B1;
    case 0x05B5: goto L05B5;
    case 0x05B7: goto L05B7;
    case 0x05BA: goto L05BA;
    case 0x05BE: goto L05BE;
    case 0x05C1: goto L05C1;
    case 0x05C5: goto L05C5;
    case 0x05C7: goto L05C7;
    case 0x05CA: goto L05CA;
    case 0x05CD: goto L05CD;
    case 0x05CF: goto L05CF;
    case 0x05D1: goto L05D1;
    case 0x05D5: goto L05D5;
    case 0x05D9: goto L05D9;
    case 0x05DC: goto L05DC;
    case 0x05DE: goto L05DE;
    case 0x05E0: goto L05E0;
    case 0x05E4: goto L05E4;
    case 0x05E6: goto L05E6;
    case 0x05E9: goto L05E9;
    case 0x05EC: goto L05EC;
    case 0x05EE: goto L05EE;
    case 0x05F1: goto L05F1;
    case 0x05F4: goto L05F4;
    case 0x05F7: goto L05F7;
    case 0x05FA: goto L05FA;
    case 0x05FB: goto L05FB;
    case 0x05FD: goto L05FD;
    case 0x05FF: goto L05FF;
    case 0x0602: goto L0602;
    case 0x0606: goto L0606;
    case 0x0608: goto L0608;
    case 0x060B: goto L060B;
    case 0x060D: goto L060D;
    case 0x060F: goto L060F;
    case 0x0611: goto L0611;
    case 0x0613: goto L0613;
    case 0x0615: goto L0615;
    case 0x0617: goto L0617;
    case 0x0618: goto L0618;
    case 0x061A: goto L061A;
    case 0x061D: goto L061D;
    case 0x061E: goto L061E;
    case 0x0620: goto L0620;
    case 0x0623: goto L0623;
    case 0x0626: goto L0626;
    case 0x0628: goto L0628;
    case 0x062A: goto L062A;
    case 0x062C: goto L062C;
    case 0x062E: goto L062E;
    case 0x0630: goto L0630;
    case 0x0632: goto L0632;
    case 0x0633: goto L0633;
    case 0x0635: goto L0635;
    case 0x0638: goto L0638;
    case 0x0639: goto L0639;
    case 0x063B: goto L063B;
    case 0x063D: goto L063D;
    case 0x063E: goto L063E;
    case 0x0640: goto L0640;
    case 0x0643: goto L0643;
    case 0x0644: goto L0644;
    case 0x0645: goto L0645;
    case 0x0649: goto L0649;
    case 0x064D: goto L064D;
    case 0x0651: goto L0651;
    case 0x0653: goto L0653;
    case 0x0656: goto L0656;
    case 0x0659: goto L0659;
    case 0x065C: goto L065C;
    case 0x065E: goto L065E;
    case 0x0661: goto L0661;
    case 0x0664: goto L0664;
    case 0x0667: goto L0667;
    case 0x066A: goto L066A;
    case 0x066B: goto L066B;
    case 0x066D: goto L066D;
    case 0x066F: goto L066F;
    case 0x0672: goto L0672;
    case 0x0676: goto L0676;
    case 0x0679: goto L0679;
    case 0x067B: goto L067B;
    case 0x067D: goto L067D;
    case 0x067F: goto L067F;
    case 0x0681: goto L0681;
    case 0x0683: goto L0683;
    case 0x0685: goto L0685;
    case 0x0687: goto L0687;
    case 0x0688: goto L0688;
    case 0x068A: goto L068A;
    case 0x068C: goto L068C;
    case 0x068D: goto L068D;
    case 0x068F: goto L068F;
    case 0x0692: goto L0692;
    case 0x0695: goto L0695;
    case 0x0697: goto L0697;
    case 0x0699: goto L0699;
    case 0x069B: goto L069B;
    case 0x069D: goto L069D;
    case 0x069F: goto L069F;
    case 0x06A1: goto L06A1;
    case 0x06A2: goto L06A2;
    case 0x06A4: goto L06A4;
    case 0x06A7: goto L06A7;
    case 0x06A8: goto L06A8;
    case 0x06AA: goto L06AA;
    case 0x06AC: goto L06AC;
    case 0x06AD: goto L06AD;
    case 0x06AF: goto L06AF;
    case 0x06B2: goto L06B2;
    case 0x06B3: goto L06B3;
    case 0x06B4: goto L06B4;
    case 0x06B8: goto L06B8;
    case 0x06BC: goto L06BC;
    case 0x06BF: goto L06BF;
    case 0x06C3: goto L06C3;
    case 0x06C4: goto L06C4;
    case 0x06C9: goto L06C9;
    case 0x06CE: goto L06CE;
    case 0x06D2: goto L06D2;
    case 0x06D6: goto L06D6;
    case 0x06D9: goto L06D9;
    case 0x06DB: goto L06DB;
    case 0x06E0: goto L06E0;
    case 0x06E4: goto L06E4;
    case 0x06E7: goto L06E7;
    case 0x06EB: goto L06EB;
    case 0x06ED: goto L06ED;
    case 0x06F0: goto L06F0;
    case 0x06F4: goto L06F4;
    case 0x06F7: goto L06F7;
    case 0x06FB: goto L06FB;
    case 0x06FD: goto L06FD;
    case 0x0700: goto L0700;
    case 0x0703: goto L0703;
    case 0x0705: goto L0705;
    case 0x0707: goto L0707;
    case 0x070B: goto L070B;
    case 0x070F: goto L070F;
    case 0x0712: goto L0712;
    case 0x0714: goto L0714;
    case 0x0716: goto L0716;
    case 0x071A: goto L071A;
    case 0x071C: goto L071C;
    case 0x071F: goto L071F;
    case 0x0722: goto L0722;
    case 0x0724: goto L0724;
    case 0x0727: goto L0727;
    case 0x0729: goto L0729;
    case 0x072C: goto L072C;
    case 0x072F: goto L072F;
    case 0x0730: goto L0730;
    case 0x0732: goto L0732;
    case 0x0734: goto L0734;
    case 0x0737: goto L0737;
    case 0x073B: goto L073B;
    case 0x073D: goto L073D;
    case 0x0740: goto L0740;
    case 0x0742: goto L0742;
    case 0x0744: goto L0744;
    case 0x0746: goto L0746;
    case 0x0748: goto L0748;
    case 0x074A: goto L074A;
    case 0x074C: goto L074C;
    case 0x074D: goto L074D;
    case 0x074F: goto L074F;
    case 0x0752: goto L0752;
    case 0x0753: goto L0753;
    case 0x0755: goto L0755;
    case 0x0758: goto L0758;
    case 0x075B: goto L075B;
    case 0x075D: goto L075D;
    case 0x075F: goto L075F;
    case 0x0761: goto L0761;
    case 0x0763: goto L0763;
    case 0x0765: goto L0765;
    case 0x0767: goto L0767;
    case 0x0768: goto L0768;
    case 0x076A: goto L076A;
    case 0x076D: goto L076D;
    case 0x076E: goto L076E;
    case 0x0770: goto L0770;
    case 0x0772: goto L0772;
    case 0x0773: goto L0773;
    case 0x0775: goto L0775;
    case 0x0778: goto L0778;
    case 0x0779: goto L0779;
    case 0x077A: goto L077A;
    case 0x077E: goto L077E;
    case 0x0782: goto L0782;
    case 0x0786: goto L0786;
    case 0x0788: goto L0788;
    case 0x078B: goto L078B;
    case 0x078D: goto L078D;
    case 0x0790: goto L0790;
    case 0x0792: goto L0792;
    case 0x0795: goto L0795;
    case 0x0798: goto L0798;
    case 0x079B: goto L079B;
    case 0x079E: goto L079E;
    case 0x079F: goto L079F;
    case 0x07A1: goto L07A1;
    case 0x07A3: goto L07A3;
    case 0x07A6: goto L07A6;
    case 0x07AA: goto L07AA;
    case 0x07AD: goto L07AD;
    case 0x07AF: goto L07AF;
    case 0x07B1: goto L07B1;
    case 0x07B3: goto L07B3;
    case 0x07B5: goto L07B5;
    case 0x07B7: goto L07B7;
    case 0x07B9: goto L07B9;
    case 0x07BB: goto L07BB;
    case 0x07BC: goto L07BC;
    case 0x07BE: goto L07BE;
    case 0x07C0: goto L07C0;
    case 0x07C1: goto L07C1;
    case 0x07C3: goto L07C3;
    case 0x07C6: goto L07C6;
    case 0x07C9: goto L07C9;
    case 0x07CB: goto L07CB;
    case 0x07CD: goto L07CD;
    case 0x07CF: goto L07CF;
    case 0x07D1: goto L07D1;
    case 0x07D3: goto L07D3;
    case 0x07D5: goto L07D5;
    case 0x07D6: goto L07D6;
    case 0x07D8: goto L07D8;
    case 0x07DB: goto L07DB;
    case 0x07DC: goto L07DC;
    case 0x07DE: goto L07DE;
    case 0x07E0: goto L07E0;
    case 0x07E1: goto L07E1;
    case 0x07E3: goto L07E3;
    case 0x07E6: goto L07E6;
    case 0x07E7: goto L07E7;
    case 0x07E8: goto L07E8;
    case 0x07EC: goto L07EC;
    case 0x07F0: goto L07F0;
    case 0x07F3: goto L07F3;
    case 0x07F7: goto L07F7;
    case 0x07F8: goto L07F8;
    case 0x07FD: goto L07FD;
    case 0x0802: goto L0802;
    case 0x0806: goto L0806;
    case 0x080A: goto L080A;
    case 0x080D: goto L080D;
    case 0x080F: goto L080F;
    case 0x0814: goto L0814;
    case 0x0818: goto L0818;
    case 0x081B: goto L081B;
    case 0x081F: goto L081F;
    case 0x0821: goto L0821;
    case 0x0824: goto L0824;
    case 0x0828: goto L0828;
    case 0x082B: goto L082B;
    case 0x082F: goto L082F;
    case 0x0831: goto L0831;
    case 0x0834: goto L0834;
    case 0x0837: goto L0837;
    case 0x0839: goto L0839;
    case 0x083B: goto L083B;
    case 0x083F: goto L083F;
    case 0x0843: goto L0843;
    case 0x0846: goto L0846;
    case 0x0848: goto L0848;
    case 0x084A: goto L084A;
    case 0x084E: goto L084E;
    case 0x0850: goto L0850;
    case 0x0853: goto L0853;
    case 0x0856: goto L0856;
    case 0x0858: goto L0858;
    case 0x085B: goto L085B;
    case 0x085D: goto L085D;
    case 0x0860: goto L0860;
    case 0x0863: goto L0863;
    case 0x0864: goto L0864;
    case 0x0866: goto L0866;
    case 0x0868: goto L0868;
    case 0x086B: goto L086B;
    case 0x086F: goto L086F;
    case 0x0871: goto L0871;
    case 0x0874: goto L0874;
    case 0x0876: goto L0876;
    case 0x0878: goto L0878;
    case 0x087A: goto L087A;
    case 0x087C: goto L087C;
    case 0x087E: goto L087E;
    case 0x0880: goto L0880;
    case 0x0881: goto L0881;
    case 0x0883: goto L0883;
    case 0x0886: goto L0886;
    case 0x0887: goto L0887;
    case 0x0889: goto L0889;
    case 0x088C: goto L088C;
    case 0x088F: goto L088F;
    case 0x0891: goto L0891;
    case 0x0893: goto L0893;
    case 0x0895: goto L0895;
    case 0x0897: goto L0897;
    case 0x0899: goto L0899;
    case 0x089B: goto L089B;
    case 0x089C: goto L089C;
    case 0x089E: goto L089E;
    case 0x08A1: goto L08A1;
    case 0x08A2: goto L08A2;
    case 0x08A4: goto L08A4;
    case 0x08A6: goto L08A6;
    case 0x08A8: goto L08A8;
    case 0x08A9: goto L08A9;
    case 0x08AB: goto L08AB;
    case 0x08AE: goto L08AE;
    case 0x08AF: goto L08AF;
    case 0x08B0: goto L08B0;
    case 0x08B4: goto L08B4;
    case 0x08B8: goto L08B8;
    case 0x08BC: goto L08BC;
    case 0x08BE: goto L08BE;
    case 0x08C1: goto L08C1;
    case 0x08C4: goto L08C4;
    case 0x08C6: goto L08C6;
    case 0x08C8: goto L08C8;
    case 0x08CB: goto L08CB;
    case 0x08CE: goto L08CE;
    case 0x08D1: goto L08D1;
    case 0x08D4: goto L08D4;
    case 0x08D5: goto L08D5;
    case 0x08D7: goto L08D7;
    case 0x08D9: goto L08D9;
    case 0x08DC: goto L08DC;
    case 0x08E0: goto L08E0;
    case 0x08E3: goto L08E3;
    case 0x08E5: goto L08E5;
    case 0x08E7: goto L08E7;
    case 0x08E9: goto L08E9;
    case 0x08EB: goto L08EB;
    case 0x08ED: goto L08ED;
    case 0x08EF: goto L08EF;
    case 0x08F1: goto L08F1;
    case 0x08F2: goto L08F2;
    case 0x08F4: goto L08F4;
    case 0x08F6: goto L08F6;
    case 0x08F7: goto L08F7;
    case 0x08F9: goto L08F9;
    case 0x08FC: goto L08FC;
    case 0x08FF: goto L08FF;
    case 0x0901: goto L0901;
    case 0x0903: goto L0903;
    case 0x0905: goto L0905;
    case 0x0907: goto L0907;
    case 0x0909: goto L0909;
    case 0x090B: goto L090B;
    case 0x090C: goto L090C;
    case 0x090E: goto L090E;
    case 0x0911: goto L0911;
    case 0x0912: goto L0912;
    case 0x0914: goto L0914;
    case 0x0916: goto L0916;
    case 0x0918: goto L0918;
    case 0x0919: goto L0919;
    case 0x091B: goto L091B;
    case 0x091E: goto L091E;
    case 0x091F: goto L091F;
    case 0x0920: goto L0920;
    case 0x0924: goto L0924;
    case 0x0928: goto L0928;
    case 0x092B: goto L092B;
    case 0x092F: goto L092F;
    case 0x0930: goto L0930;
    case 0x0933: goto L0933;
    case 0x0935: goto L0935;
    case 0x0937: goto L0937;
    case 0x093A: goto L093A;
    case 0x093D: goto L093D;
    case 0x093F: goto L093F;
    case 0x0940: goto L0940;
    case 0x0941: goto L0941;
    case 0x0944: goto L0944;
    case 0x0946: goto L0946;
    case 0x0947: goto L0947;
    case 0x094A: goto L094A;
    case 0x094C: goto L094C;
    case 0x094E: goto L094E;
    case 0x0950: goto L0950;
    case 0x0951: goto L0951;
    case 0x0954: goto L0954;
    case 0x0956: goto L0956;
    case 0x0958: goto L0958;
    case 0x095A: goto L095A;
    case 0x095B: goto L095B;
    case 0x095E: goto L095E;
    case 0x0960: goto L0960;
    case 0x0962: goto L0962;
    case 0x0964: goto L0964;
    case 0x0966: goto L0966;
    case 0x0969: goto L0969;
    case 0x096B: goto L096B;
    case 0x096D: goto L096D;
    case 0x096F: goto L096F;
    case 0x0971: goto L0971;
    case 0x0973: goto L0973;
    case 0x0975: goto L0975;
    case 0x0977: goto L0977;
    case 0x0979: goto L0979;
    case 0x097B: goto L097B;
    case 0x097D: goto L097D;
    case 0x097F: goto L097F;
    case 0x0981: goto L0981;
    case 0x0983: goto L0983;
    case 0x0985: goto L0985;
    case 0x0987: goto L0987;
    case 0x0989: goto L0989;
    case 0x098B: goto L098B;
    case 0x098D: goto L098D;
    case 0x098F: goto L098F;
    case 0x0991: goto L0991;
    case 0x0993: goto L0993;
    case 0x0997: goto L0997;
    case 0x0999: goto L0999;
    case 0x099C: goto L099C;
    case 0x099E: goto L099E;
    case 0x09A2: goto L09A2;
    case 0x09A4: goto L09A4;
    case 0x09A6: goto L09A6;
    case 0x09AA: goto L09AA;
    case 0x09AC: goto L09AC;
    case 0x09AE: goto L09AE;
    case 0x09B0: goto L09B0;
    case 0x09B1: goto L09B1;
    case 0x09B2: goto L09B2;
    case 0x09B4: goto L09B4;
    case 0x09B7: goto L09B7;
    case 0x09B9: goto L09B9;
    case 0x09BC: goto L09BC;
    case 0x09BE: goto L09BE;
    case 0x09C5: goto L09C5;
    case 0x09CC: goto L09CC;
    case 0x09CE: goto L09CE;
    case 0x09D1: goto L09D1;
    case 0x09D3: goto L09D3;
    case 0x09D5: goto L09D5;
    case 0x09D9: goto L09D9;
    case 0x09DE: goto L09DE;
    case 0x09DF: goto L09DF;
    case 0x09E5: goto L09E5;
    case 0x09EA: goto L09EA;
    case 0x09EC: goto L09EC;
    case 0x09F0: goto L09F0;
    case 0x09F2: goto L09F2;
    case 0x09F4: goto L09F4;
    case 0x09F6: goto L09F6;
    case 0x09F8: goto L09F8;
    case 0x09FA: goto L09FA;
    case 0x09FF: goto L09FF;
    case 0x0A00: goto L0A00;
    case 0x0A03: goto L0A03;
    case 0x0A05: goto L0A05;
    case 0x0A0A: goto L0A0A;
    default: asm_bad_entry("SMOOTH.ASM", entry);
    }

    /* seg004_260  (+260)
       Draws the Gouraud polygon in the vertex buffer (ds:307h..305h). Raises int 2 if the buffer
       has run past 0C69h. Codes AND nonzero: skipped. Codes OR zero: projected (the add
       immediates at seg004_2A4 and 2AF are the screen centre, patched by render_3d) with
       the shades copied to seg048:55ECh, then drawn with BP = ds:[15F8h]. Codes OR with bit 80h
       (behind the eye): clipped against top, bottom, left and right in turn, retrying after each.
       Otherwise projected with overflow checks (immediates at 312 and 322), drawn with BP =
       ds:[15F6h], or clipped on overflow (L03FC). Continues with the next opcode. */
L0260: /* _smooth_closure */
    /* 0260  cmp     word ptr ds:[VB_END],VBUF1 */
    sub16(rw(pDS, 0x305), 0xC69, 0);
L0266:
    /* 0266  jle     short L026A */
    if (ZF || SF != OF) goto L026A;
L0268:
    /* 0268  int     2 */
    asm_halt_at(0x06E7, 0x0268, "int 2h, the debugger break");
L026A: /* L026A */
    /* 026A  mov     word ptr ds:[2884h],0 */
    ww(pDS, 0x2884, 0x0);
L0270:
    /* 0270  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L0275:
    /* 0275  je      short L027A */
    if (ZF) goto L027A;
L0277:
    /* 0277  jmp     L03F2 */
    goto L03F2;
L027A: /* L027A */
    /* 027A  mov     di,VBUF0 */
    DI = 0x309;
L027D:
    /* 027D  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L0281:
    /* 0281  test    byte ptr ds:[CLIP_OR],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0xFF));
L0286:
    /* 0286  jne     short L02ED */
    if (!ZF) goto L02ED;
L0288: /* L0288 */
    /* 0288  push    si */
    push16(SI);
L0289:
    /* 0289  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L028C:
    /* 028C  mov     es,ax */
    SET_ES(AX);
L028E:
    /* 028E  mov     di,415Ch */
    DI = 0x415C;
L0291:
    /* 0291  mov     bx,55ECh */
    BX = 0x55EC;
L0294:
    /* 0294  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L0298:
    /* 0298  mov     cx,word ptr ds:[PROJ_X] */
    CX = rw(pDS, 0x26B2);
L029C: /* L029C */
    /* 029C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L029D:
    /* 029D  mov     bp,word ptr [si+2] */
    BP = rw(pDS, SI + 0x2);
L02A0:
    /* 02A0  imul    cx */
    imul16(CX);
L02A2:
    /* 02A2  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x02A2, 2)) != 0) return c;
L02A4: /* _seg004_2A4 */
    /* 02A4  add     ax,1234h */
    AX = (uint16_t)(AX + rw(CODE004, 0x02A5));
L02A7:
    /* 02A7  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L02A8:
    /* 02A8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L02A9:
    /* 02A9  imul    word ptr ds:[PROJ_Y] */
    imul16(rw(pDS, 0x26B0));
L02AD:
    /* 02AD  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x02AD, 2)) != 0) return c;
L02AF: /* _seg004_2AF */
    /* 02AF  add     ax,1234h */
    AX = (uint16_t)(AX + rw(CODE004, 0x02B0));
L02B2:
    /* 02B2  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L02B3:
    /* 02B3  mov     al,byte ptr [si+3] */
    AL = rb(pDS, SI + 0x3);
L02B6:
    /* 02B6  mov     byte ptr es:[bx],al */
    wb(pES, BX, AL);
L02B9:
    /* 02B9  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L02BC:
    /* 02BC  add     bx,2 */
    BX = (uint16_t)(BX + 0x2);
L02BF:
    /* 02BF  cmp     si,word ptr ds:[VB_END] */
    sub16(SI, rw(pDS, 0x305), 0);
L02C3:
    /* 02C3  jne     L029C */
    if (!ZF) goto L029C;
L02C5:
    /* 02C5  mov     bx,word ptr ds:[VB_END] */
    BX = rw(pDS, 0x305);
L02C9:
    /* 02C9  sub     bx,word ptr ds:[VB_START] */
    BX = (uint16_t)(BX - rw(pDS, 0x307));
L02CD:
    /* 02CD  shr     bx,1 */
    BX = (uint16_t)(BX >> 1);
L02CF:
    /* 02CF  shr     bx,1 */
    BX = (uint16_t)(BX >> 1);
L02D1:
    /* 02D1  shr     bx,1 */
    BX = (uint16_t)(BX >> 1);
L02D3:
    /* 02D3  mov     cx,bx */
    CX = BX;
L02D5:
    /* 02D5  inc     word ptr ds:[2884h] */
    ww(pDS, 0x2884, (uint16_t)(rw(pDS, 0x2884) + 1));
L02D9:
    /* 02D9  mov     bp,word ptr ds:[POLY_UFILL] */
    BP = rw(pDS, 0x15F8);
L02DD:
    /* 02DD  call    far ptr _seg003_5B0B */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x5B0B), 0x06E7 + PORT_LOAD_SEG, 0x02E2)) != 0) return c;
L02E2:
    /* 02E2  mov     ax,ds */
    AX = asm_ds;
L02E4:
    /* 02E4  mov     es,ax */
    SET_ES(AX);
L02E6:
    /* 02E6  pop     si */
    SI = pop16();
L02E7:
    /* 02E7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L02E8:
    /* 02E8  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L02E9:
    /* 02E9  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L02ED: /* L02ED */
    /* 02ED  js      short L0363 */
    if (SF) goto L0363;
L02EF:
    /* 02EF  push    si */
    push16(SI);
L02F0: /* L02F0 */
    /* 02F0  mov     word ptr ss:[4D5h],offset _seg004_3F8 */
    ww(pSS, 0x4D5, 0x3F8);
L02F7:
    /* 02F7  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L02FA:
    /* 02FA  mov     es,ax */
    SET_ES(AX);
L02FC:
    /* 02FC  mov     di,415Ch */
    DI = 0x415C;
L02FF:
    /* 02FF  mov     bx,55ECh */
    BX = 0x55EC;
L0302:
    /* 0302  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L0306:
    /* 0306  mov     cx,word ptr ds:[PROJ_X] */
    CX = rw(pDS, 0x26B2);
L030A: /* L030A */
    /* 030A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L030B:
    /* 030B  mov     bp,word ptr [si+2] */
    BP = rw(pDS, SI + 0x2);
L030E:
    /* 030E  imul    cx */
    imul16(CX);
L0310:
    /* 0310  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x0310, 2)) != 0) return c;
L0312: /* _seg004_312 */
    /* 0312  add     ax,1234h */
    AX = add16(AX, rw(CODE004, 0x0313), 0);
L0315:
    /* 0315  jno     short L031A */
    if (!OF) goto L031A;
L0317:
    /* 0317  jmp     L03FC */
    goto L03FC;
L031A: /* L031A */
    /* 031A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L031B:
    /* 031B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L031C:
    /* 031C  imul    word ptr ds:[PROJ_Y] */
    imul16(rw(pDS, 0x26B0));
L0320:
    /* 0320  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x0320, 2)) != 0) return c;
L0322: /* _seg004_322 */
    /* 0322  add     ax,1234h */
    AX = add16(AX, rw(CODE004, 0x0323), 0);
L0325:
    /* 0325  jno     short L032A */
    if (!OF) goto L032A;
L0327:
    /* 0327  jmp     L03FC */
    goto L03FC;
L032A: /* L032A */
    /* 032A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L032B:
    /* 032B  mov     al,byte ptr [si+3] */
    AL = rb(pDS, SI + 0x3);
L032E:
    /* 032E  mov     byte ptr es:[bx],al */
    wb(pES, BX, AL);
L0331:
    /* 0331  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L0334:
    /* 0334  add     bx,2 */
    BX = (uint16_t)(BX + 0x2);
L0337:
    /* 0337  cmp     si,word ptr ds:[VB_END] */
    sub16(SI, rw(pDS, 0x305), 0);
L033B:
    /* 033B  jne     L030A */
    if (!ZF) goto L030A;
L033D:
    /* 033D  mov     cx,word ptr ds:[VB_END] */
    CX = rw(pDS, 0x305);
L0341:
    /* 0341  sub     cx,word ptr ds:[VB_START] */
    CX = (uint16_t)(CX - rw(pDS, 0x307));
L0345:
    /* 0345  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L0347:
    /* 0347  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L0349:
    /* 0349  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L034B:
    /* 034B  inc     word ptr ds:[2884h] */
    ww(pDS, 0x2884, (uint16_t)(rw(pDS, 0x2884) + 1));
L034F:
    /* 034F  mov     bp,word ptr ds:[POLY_FILL] */
    BP = rw(pDS, 0x15F6);
L0353:
    /* 0353  call    far ptr _seg003_5B0B */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x5B0B), 0x06E7 + PORT_LOAD_SEG, 0x0358)) != 0) return c;
L0358:
    /* 0358  mov     ax,ds */
    AX = asm_ds;
L035A:
    /* 035A  mov     es,ax */
    SET_ES(AX);
L035C:
    /* 035C  pop     si */
    SI = pop16();
L035D:
    /* 035D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L035E:
    /* 035E  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L035F:
    /* 035F  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L0363: /* L0363 */
    /* 0363  mov     word ptr ss:[4D5h],offset _seg004_9E5 */
    ww(pSS, 0x4D5, 0x9E5);
L036A:
    /* 036A  test    byte ptr ds:[CLIP_OR],4 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x4));
L036F:
    /* 036F  je      short L038C */
    if (ZF) goto L038C;
L0371:
    /* 0371  push    si */
    push16(SI);
L0372:
    /* 0372  call    _seg004_45C */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x045C), 0x0375)) != 0) return c;
L0375:
    /* 0375  pop     si */
    SI = pop16();
L0376:
    /* 0376  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L037B:
    /* 037B  jne     short L03F2 */
    if (!ZF) goto L03F2;
L037D:
    /* 037D  test    byte ptr ds:[CLIP_OR],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0xFF));
L0382:
    /* 0382  js      short L038C */
    if (SF) goto L038C;
L0384:
    /* 0384  jne     short L0389 */
    if (!ZF) goto L0389;
L0386:
    /* 0386  jmp     L0288 */
    goto L0288;
L0389: /* L0389 */
    /* 0389  jmp     L02ED */
    goto L02ED;
L038C: /* L038C */
    /* 038C  test    byte ptr ds:[CLIP_OR],8 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x8));
L0391:
    /* 0391  je      short L03AE */
    if (ZF) goto L03AE;
L0393:
    /* 0393  push    si */
    push16(SI);
L0394:
    /* 0394  call    _seg004_58E */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x058E), 0x0397)) != 0) return c;
L0397:
    /* 0397  pop     si */
    SI = pop16();
L0398:
    /* 0398  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L039D:
    /* 039D  jne     short L03F2 */
    if (!ZF) goto L03F2;
L039F:
    /* 039F  test    byte ptr ds:[CLIP_OR],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0xFF));
L03A4:
    /* 03A4  js      short L03AE */
    if (SF) goto L03AE;
L03A6:
    /* 03A6  jne     short L03AB */
    if (!ZF) goto L03AB;
L03A8:
    /* 03A8  jmp     L0288 */
    goto L0288;
L03AB: /* L03AB */
    /* 03AB  jmp     L02ED */
    goto L02ED;
L03AE: /* L03AE */
    /* 03AE  test    byte ptr ds:[CLIP_OR],1 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x1));
L03B3:
    /* 03B3  je      short L03D0 */
    if (ZF) goto L03D0;
L03B5:
    /* 03B5  push    si */
    push16(SI);
L03B6:
    /* 03B6  call    _seg004_7F8 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x07F8), 0x03B9)) != 0) return c;
L03B9:
    /* 03B9  pop     si */
    SI = pop16();
L03BA:
    /* 03BA  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L03BF:
    /* 03BF  jne     short L03F2 */
    if (!ZF) goto L03F2;
L03C1:
    /* 03C1  test    byte ptr ds:[CLIP_OR],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0xFF));
L03C6:
    /* 03C6  js      short L03D0 */
    if (SF) goto L03D0;
L03C8:
    /* 03C8  jne     short L03CD */
    if (!ZF) goto L03CD;
L03CA:
    /* 03CA  jmp     L0288 */
    goto L0288;
L03CD: /* L03CD */
    /* 03CD  jmp     L02ED */
    goto L02ED;
L03D0: /* L03D0 */
    /* 03D0  test    byte ptr ds:[CLIP_OR],2 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x2));
L03D5:
    /* 03D5  je      short L03F2 */
    if (ZF) goto L03F2;
L03D7:
    /* 03D7  push    si */
    push16(SI);
L03D8:
    /* 03D8  call    _seg004_6C4 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x06C4), 0x03DB)) != 0) return c;
L03DB:
    /* 03DB  pop     si */
    SI = pop16();
L03DC:
    /* 03DC  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L03E1:
    /* 03E1  jne     short L03F2 */
    if (!ZF) goto L03F2;
L03E3:
    /* 03E3  test    byte ptr ds:[CLIP_OR],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0xFF));
L03E8:
    /* 03E8  js      short L03F2 */
    if (SF) goto L03F2;
L03EA:
    /* 03EA  jne     short L03EF */
    if (!ZF) goto L03EF;
L03EC:
    /* 03EC  jmp     L0288 */
    goto L0288;
L03EF: /* L03EF */
    /* 03EF  jmp     L02ED */
    goto L02ED;
L03F2: /* L03F2 */
    /* 03F2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L03F3:
    /* 03F3  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L03F4:
    /* 03F4  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3F8  (+3F8)
       Recovery entry for the divide overflow during smooth_closure's checked projection (it is
       stored at ss:[4D5h]): drops the int 0 frame, then (L03FC) clips against right, left, top
       and bottom as the codes require and projects again; gives up when nothing is left,
       restoring DS and ES to seg051. The counterpart of INTERP's clip_overflow and
       do_3d_clip. */
L03F8: /* _seg004_3F8 */
    /* 03F8  sti */
    ;
L03F9:
    /* 03F9  add     sp,6 */
    SP = (uint16_t)(SP + 0x6);
L03FC: /* L03FC */
    /* 03FC  mov     word ptr ss:[4D5h],offset _seg004_9E5 */
    ww(pSS, 0x4D5, 0x9E5);
L0403:
    /* 0403  mov     ax,ds */
    AX = asm_ds;
L0405:
    /* 0405  mov     es,ax */
    SET_ES(AX);
L0407:
    /* 0407  test    byte ptr ds:[CLIP_OR],2 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x2));
L040C:
    /* 040C  je      short L0418 */
    if (ZF) goto L0418;
L040E:
    /* 040E  call    _seg004_6C4 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x06C4), 0x0411)) != 0) return c;
L0411:
    /* 0411  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L0416:
    /* 0416  jne     short L044E */
    if (!ZF) goto L044E;
L0418: /* L0418 */
    /* 0418  test    byte ptr ds:[CLIP_OR],1 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x1));
L041D:
    /* 041D  je      short L0429 */
    if (ZF) goto L0429;
L041F:
    /* 041F  call    _seg004_7F8 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x07F8), 0x0422)) != 0) return c;
L0422:
    /* 0422  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L0427:
    /* 0427  jne     short L044E */
    if (!ZF) goto L044E;
L0429: /* L0429 */
    /* 0429  test    byte ptr ds:[CLIP_OR],4 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x4));
L042E:
    /* 042E  je      short L043A */
    if (ZF) goto L043A;
L0430:
    /* 0430  call    _seg004_45C */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x045C), 0x0433)) != 0) return c;
L0433:
    /* 0433  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L0438:
    /* 0438  jne     short L044E */
    if (!ZF) goto L044E;
L043A: /* L043A */
    /* 043A  test    byte ptr ds:[CLIP_OR],8 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x8));
L043F:
    /* 043F  je      short L044B */
    if (ZF) goto L044B;
L0441:
    /* 0441  call    _seg004_58E */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x058E), 0x0444)) != 0) return c;
L0444:
    /* 0444  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L0449:
    /* 0449  jne     short L044E */
    if (!ZF) goto L044E;
L044B: /* L044B */
    /* 044B  jmp     L02F0 */
    goto L02F0;
L044E: /* L044E */
    /* 044E  pop     si */
    SI = pop16();
L044F:
    /* 044F  mov     ax,seg seg051 */
    AX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L0452:
    /* 0452  mov     ds,ax */
    SET_DS(AX);
L0454:
    /* 0454  mov     es,ax */
    SET_ES(AX);
L0456:
    /* 0456  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0457:
    /* 0457  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L0458:
    /* 0458  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_45C  (+45C)
       Clips the Gouraud polygon against y = z (code bit 4), like INTERP's clip_top, also
       interpolating each new vertex's shade (+7). */
L045C: /* _seg004_45C */
    /* 045C  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L0461:
    /* 0461  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L0466:
    /* 0466  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L046A:
    /* 046A  mov     di,word ptr ds:[VB_END] */
    DI = rw(pDS, 0x305);
L046E:
    /* 046E  mov     cx,8 */
    CX = 0x8;
L0471:
    /* 0471  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0473:
    /* 0473  add     word ptr ds:[VB_END],8 */
    ww(pDS, 0x305, (uint16_t)(rw(pDS, 0x305) + 0x8));
L0478:
    /* 0478  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L047C:
    /* 047C  mov     di,VBUF1 */
    DI = 0xC69;
L047F:
    /* 047F  cmp     si,VBUF0 */
    sub16(SI, 0x309, 0);
L0483:
    /* 0483  je      short L0488 */
    if (ZF) goto L0488;
L0485:
    /* 0485  mov     di,VBUF0 */
    DI = 0x309;
L0488: /* L0488 */
    /* 0488  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L048C: /* L048C */
    /* 048C  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L048F: /* L048F */
    /* 048F  cmp     si,word ptr ds:[VB_END] */
    sub16(SI, rw(pDS, 0x305), 0);
L0493:
    /* 0493  jne     short L0498 */
    if (!ZF) goto L0498;
L0495:
    /* 0495  jmp     L0589 */
    goto L0589;
L0498: /* L0498 */
    /* 0498  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L049B:
    /* 049B  test    al,4 */
    logic8((uint8_t)(AL & 0x4));
L049D:
    /* 049D  jne     short L04AE */
    if (!ZF) goto L04AE;
L049F:
    /* 049F  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L04A3:
    /* 04A3  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L04A7:
    /* 04A7  mov     cx,4 */
    CX = 0x4;
L04AA:
    /* 04AA  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L04AC:
    /* 04AC  jmp     L048F */
    goto L048F;
L04AE: /* L04AE */
    /* 04AE  test    byte ptr [si-2],4 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFE) & 0x4));
L04B2:
    /* 04B2  jne     short L0519 */
    if (!ZF) goto L0519;
L04B4:
    /* 04B4  mov     bp,word ptr [si-4] */
    BP = rw(pDS, SI + 0xFFFC);
L04B7:
    /* 04B7  sub     bp,word ptr [si-6] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0xFFFA));
L04BA:
    /* 04BA  mov     cx,bp */
    CX = BP;
L04BC:
    /* 04BC  sub     cx,word ptr [si+4] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x4));
L04BF:
    /* 04BF  add     cx,word ptr [si+2] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0x2));
L04C2:
    /* 04C2  mov     al,byte ptr [si+7] */
    AL = rb(pDS, SI + 0x7);
L04C5:
    /* 04C5  sub     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL - rb(pDS, SI + 0xF));
L04C8:
    /* 04C8  cbw */
    AX = (uint16_t)(int8_t)AL;
L04C9:
    /* 04C9  imul    bp */
    imul16(BP);
L04CB:
    /* 04CB  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x04CB, 2)) != 0) return c;
L04CD:
    /* 04CD  add     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL + rb(pDS, SI + 0xF));
L04D0:
    /* 04D0  mov     byte ptr es:[di+7],al */
    wb(pES, DI + 0x7, AL);
L04D4:
    /* 04D4  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L04D6:
    /* 04D6  sub     ax,word ptr [si-8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF8));
L04D9:
    /* 04D9  imul    bp */
    imul16(BP);
L04DB:
    /* 04DB  shl     ax,1 */
    AX = shl16(AX, 1);
L04DD:
    /* 04DD  rcl     dx,1 */
    DX = rcl16(DX, 1);
L04DF:
    /* 04DF  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x04DF, 2)) != 0) return c;
L04E1:
    /* 04E1  sar     ax,1 */
    AX = sar16(AX, 1);
L04E3:
    /* 04E3  jae     short L04E8 */
    if (!CF) goto L04E8;
L04E5:
    /* 04E5  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L04E6:
    /* 04E6  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L04E8: /* L04E8 */
    /* 04E8  add     ax,word ptr [si-8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF8));
L04EB:
    /* 04EB  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L04EC:
    /* 04EC  mov     bx,ax */
    BX = AX;
L04EE:
    /* 04EE  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L04F1:
    /* 04F1  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L04F4:
    /* 04F4  imul    bp */
    imul16(BP);
L04F6:
    /* 04F6  shl     ax,1 */
    AX = shl16(AX, 1);
L04F8:
    /* 04F8  rcl     dx,1 */
    DX = rcl16(DX, 1);
L04FA:
    /* 04FA  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x04FA, 2)) != 0) return c;
L04FC:
    /* 04FC  sar     ax,1 */
    AX = sar16(AX, 1);
L04FE:
    /* 04FE  jae     short L0503 */
    if (!CF) goto L0503;
L0500:
    /* 0500  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0501:
    /* 0501  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L0503: /* L0503 */
    /* 0503  add     ax,word ptr [si-6] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFA));
L0506:
    /* 0506  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0507:
    /* 0507  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0508:
    /* 0508  mov     cx,ax */
    CX = AX;
L050A:
    /* 050A  mov     bp,ax */
    BP = AX;
L050C:
    /* 050C  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x050F)) != 0) return c;
L050F:
    /* 050F  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0510:
    /* 0510  inc     di */
    DI = (uint16_t)(DI + 1);
L0511:
    /* 0511  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L0515:
    /* 0515  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L0519: /* L0519 */
    /* 0519  test    byte ptr [si+0Eh],4 */
    logic8((uint8_t)(rb(pDS, SI + 0xE) & 0x4));
L051D:
    /* 051D  je      short L0522 */
    if (ZF) goto L0522;
L051F:
    /* 051F  jmp     L048C */
    goto L048C;
L0522: /* L0522 */
    /* 0522  mov     bp,word ptr [si+4] */
    BP = rw(pDS, SI + 0x4);
L0525:
    /* 0525  sub     bp,word ptr [si+2] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0x2));
L0528:
    /* 0528  mov     cx,bp */
    CX = BP;
L052A:
    /* 052A  add     cx,word ptr [si+0Ah] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0xA));
L052D:
    /* 052D  sub     cx,word ptr [si+0Ch] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xC));
L0530:
    /* 0530  mov     al,byte ptr [si+7] */
    AL = rb(pDS, SI + 0x7);
L0533:
    /* 0533  sub     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL - rb(pDS, SI + 0xF));
L0536:
    /* 0536  cbw */
    AX = (uint16_t)(int8_t)AL;
L0537:
    /* 0537  imul    bp */
    imul16(BP);
L0539:
    /* 0539  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x0539, 2)) != 0) return c;
L053B:
    /* 053B  add     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL + rb(pDS, SI + 0xF));
L053E:
    /* 053E  mov     byte ptr es:[di+7],al */
    wb(pES, DI + 0x7, AL);
L0542:
    /* 0542  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L0545:
    /* 0545  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L0547:
    /* 0547  imul    bp */
    imul16(BP);
L0549:
    /* 0549  shl     ax,1 */
    AX = shl16(AX, 1);
L054B:
    /* 054B  rcl     dx,1 */
    DX = rcl16(DX, 1);
L054D:
    /* 054D  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x054D, 2)) != 0) return c;
L054F:
    /* 054F  sar     ax,1 */
    AX = sar16(AX, 1);
L0551:
    /* 0551  jae     short L0556 */
    if (!CF) goto L0556;
L0553:
    /* 0553  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0554:
    /* 0554  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L0556: /* L0556 */
    /* 0556  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L0558:
    /* 0558  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0559:
    /* 0559  mov     bx,ax */
    BX = AX;
L055B:
    /* 055B  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L055E:
    /* 055E  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L0561:
    /* 0561  imul    bp */
    imul16(BP);
L0563:
    /* 0563  shl     ax,1 */
    AX = shl16(AX, 1);
L0565:
    /* 0565  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0567:
    /* 0567  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x0567, 2)) != 0) return c;
L0569:
    /* 0569  sar     ax,1 */
    AX = sar16(AX, 1);
L056B:
    /* 056B  jae     short L0570 */
    if (!CF) goto L0570;
L056D:
    /* 056D  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L056E:
    /* 056E  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L0570: /* L0570 */
    /* 0570  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L0573:
    /* 0573  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0574:
    /* 0574  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0575:
    /* 0575  mov     cx,ax */
    CX = AX;
L0577:
    /* 0577  mov     bp,ax */
    BP = AX;
L0579:
    /* 0579  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x057C)) != 0) return c;
L057C:
    /* 057C  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L057D:
    /* 057D  inc     di */
    DI = (uint16_t)(DI + 1);
L057E:
    /* 057E  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L0582:
    /* 0582  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L0586:
    /* 0586  jmp     L048C */
    goto L048C;
L0589: /* L0589 */
    /* 0589  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L058D:
    /* 058D  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_58E  (+58E)
       Gouraud clip against y = -z (code bit 8), like clip_bot. */
L058E: /* _seg004_58E */
    /* 058E  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L0593:
    /* 0593  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L0598:
    /* 0598  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L059C:
    /* 059C  mov     di,word ptr ds:[VB_END] */
    DI = rw(pDS, 0x305);
L05A0:
    /* 05A0  mov     cx,8 */
    CX = 0x8;
L05A3:
    /* 05A3  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L05A5:
    /* 05A5  add     word ptr ds:[VB_END],8 */
    ww(pDS, 0x305, (uint16_t)(rw(pDS, 0x305) + 0x8));
L05AA:
    /* 05AA  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L05AE:
    /* 05AE  mov     di,VBUF1 */
    DI = 0xC69;
L05B1:
    /* 05B1  cmp     si,VBUF0 */
    sub16(SI, 0x309, 0);
L05B5:
    /* 05B5  je      short L05BA */
    if (ZF) goto L05BA;
L05B7:
    /* 05B7  mov     di,VBUF0 */
    DI = 0x309;
L05BA: /* L05BA */
    /* 05BA  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L05BE: /* L05BE */
    /* 05BE  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L05C1: /* L05C1 */
    /* 05C1  cmp     si,word ptr ds:[VB_END] */
    sub16(SI, rw(pDS, 0x305), 0);
L05C5:
    /* 05C5  jne     short L05CA */
    if (!ZF) goto L05CA;
L05C7:
    /* 05C7  jmp     L06BF */
    goto L06BF;
L05CA: /* L05CA */
    /* 05CA  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L05CD:
    /* 05CD  test    al,8 */
    logic8((uint8_t)(AL & 0x8));
L05CF:
    /* 05CF  jne     short L05E0 */
    if (!ZF) goto L05E0;
L05D1:
    /* 05D1  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L05D5:
    /* 05D5  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L05D9:
    /* 05D9  mov     cx,4 */
    CX = 0x4;
L05DC:
    /* 05DC  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L05DE:
    /* 05DE  jmp     L05C1 */
    goto L05C1;
L05E0: /* L05E0 */
    /* 05E0  test    byte ptr [si-2],8 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFE) & 0x8));
L05E4:
    /* 05E4  jne     short L064D */
    if (!ZF) goto L064D;
L05E6:
    /* 05E6  mov     bp,word ptr [si-4] */
    BP = rw(pDS, SI + 0xFFFC);
L05E9:
    /* 05E9  add     bp,word ptr [si-6] */
    BP = (uint16_t)(BP + rw(pDS, SI + 0xFFFA));
L05EC:
    /* 05EC  mov     cx,bp */
    CX = BP;
L05EE:
    /* 05EE  sub     cx,word ptr [si+4] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x4));
L05F1:
    /* 05F1  sub     cx,word ptr [si+2] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x2));
L05F4:
    /* 05F4  mov     al,byte ptr [si+7] */
    AL = rb(pDS, SI + 0x7);
L05F7:
    /* 05F7  sub     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL - rb(pDS, SI + 0xF));
L05FA:
    /* 05FA  cbw */
    AX = (uint16_t)(int8_t)AL;
L05FB:
    /* 05FB  imul    bp */
    imul16(BP);
L05FD:
    /* 05FD  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x05FD, 2)) != 0) return c;
L05FF:
    /* 05FF  add     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL + rb(pDS, SI + 0xF));
L0602:
    /* 0602  mov     byte ptr es:[di+7],al */
    wb(pES, DI + 0x7, AL);
L0606:
    /* 0606  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L0608:
    /* 0608  sub     ax,word ptr [si-8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF8));
L060B:
    /* 060B  imul    bp */
    imul16(BP);
L060D:
    /* 060D  shl     ax,1 */
    AX = shl16(AX, 1);
L060F:
    /* 060F  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0611:
    /* 0611  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x0611, 2)) != 0) return c;
L0613:
    /* 0613  sar     ax,1 */
    AX = sar16(AX, 1);
L0615:
    /* 0615  jae     short L061A */
    if (!CF) goto L061A;
L0617:
    /* 0617  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0618:
    /* 0618  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L061A: /* L061A */
    /* 061A  add     ax,word ptr [si-8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF8));
L061D:
    /* 061D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L061E:
    /* 061E  mov     bx,ax */
    BX = AX;
L0620:
    /* 0620  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L0623:
    /* 0623  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L0626:
    /* 0626  imul    bp */
    imul16(BP);
L0628:
    /* 0628  shl     ax,1 */
    AX = shl16(AX, 1);
L062A:
    /* 062A  rcl     dx,1 */
    DX = rcl16(DX, 1);
L062C:
    /* 062C  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x062C, 2)) != 0) return c;
L062E:
    /* 062E  sar     ax,1 */
    AX = sar16(AX, 1);
L0630:
    /* 0630  jae     short L0635 */
    if (!CF) goto L0635;
L0632:
    /* 0632  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0633:
    /* 0633  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L0635: /* L0635 */
    /* 0635  add     ax,word ptr [si-6] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFA));
L0638:
    /* 0638  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0639:
    /* 0639  mov     cx,ax */
    CX = AX;
L063B:
    /* 063B  neg     ax */
    AX = (uint16_t)-AX;
L063D:
    /* 063D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L063E:
    /* 063E  mov     bp,ax */
    BP = AX;
L0640:
    /* 0640  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x0643)) != 0) return c;
L0643:
    /* 0643  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0644:
    /* 0644  inc     di */
    DI = (uint16_t)(DI + 1);
L0645:
    /* 0645  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L0649:
    /* 0649  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L064D: /* L064D */
    /* 064D  test    byte ptr [si+0Eh],8 */
    logic8((uint8_t)(rb(pDS, SI + 0xE) & 0x8));
L0651:
    /* 0651  je      short L0656 */
    if (ZF) goto L0656;
L0653:
    /* 0653  jmp     L05BE */
    goto L05BE;
L0656: /* L0656 */
    /* 0656  mov     bp,word ptr [si+4] */
    BP = rw(pDS, SI + 0x4);
L0659:
    /* 0659  add     bp,word ptr [si+2] */
    BP = (uint16_t)(BP + rw(pDS, SI + 0x2));
L065C:
    /* 065C  mov     cx,bp */
    CX = BP;
L065E:
    /* 065E  sub     cx,word ptr [si+0Ah] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xA));
L0661:
    /* 0661  sub     cx,word ptr [si+0Ch] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xC));
L0664:
    /* 0664  mov     al,byte ptr [si+7] */
    AL = rb(pDS, SI + 0x7);
L0667:
    /* 0667  sub     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL - rb(pDS, SI + 0xF));
L066A:
    /* 066A  cbw */
    AX = (uint16_t)(int8_t)AL;
L066B:
    /* 066B  imul    bp */
    imul16(BP);
L066D:
    /* 066D  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x066D, 2)) != 0) return c;
L066F:
    /* 066F  add     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL + rb(pDS, SI + 0xF));
L0672:
    /* 0672  mov     byte ptr es:[di+7],al */
    wb(pES, DI + 0x7, AL);
L0676:
    /* 0676  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L0679:
    /* 0679  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L067B:
    /* 067B  imul    bp */
    imul16(BP);
L067D:
    /* 067D  shl     ax,1 */
    AX = shl16(AX, 1);
L067F:
    /* 067F  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0681:
    /* 0681  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x0681, 2)) != 0) return c;
L0683:
    /* 0683  sar     ax,1 */
    AX = sar16(AX, 1);
L0685:
    /* 0685  jae     short L068A */
    if (!CF) goto L068A;
L0687:
    /* 0687  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0688:
    /* 0688  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L068A: /* L068A */
    /* 068A  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L068C:
    /* 068C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L068D:
    /* 068D  mov     bx,ax */
    BX = AX;
L068F:
    /* 068F  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L0692:
    /* 0692  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L0695:
    /* 0695  imul    bp */
    imul16(BP);
L0697:
    /* 0697  shl     ax,1 */
    AX = shl16(AX, 1);
L0699:
    /* 0699  rcl     dx,1 */
    DX = rcl16(DX, 1);
L069B:
    /* 069B  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x069B, 2)) != 0) return c;
L069D:
    /* 069D  sar     ax,1 */
    AX = sar16(AX, 1);
L069F:
    /* 069F  jae     short L06A4 */
    if (!CF) goto L06A4;
L06A1:
    /* 06A1  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L06A2:
    /* 06A2  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L06A4: /* L06A4 */
    /* 06A4  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L06A7:
    /* 06A7  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L06A8:
    /* 06A8  mov     cx,ax */
    CX = AX;
L06AA:
    /* 06AA  neg     ax */
    AX = (uint16_t)-AX;
L06AC:
    /* 06AC  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L06AD:
    /* 06AD  mov     bp,ax */
    BP = AX;
L06AF:
    /* 06AF  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x06B2)) != 0) return c;
L06B2:
    /* 06B2  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L06B3:
    /* 06B3  inc     di */
    DI = (uint16_t)(DI + 1);
L06B4:
    /* 06B4  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L06B8:
    /* 06B8  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L06BC:
    /* 06BC  jmp     L05BE */
    goto L05BE;
L06BF: /* L06BF */
    /* 06BF  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L06C3:
    /* 06C3  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_6C4  (+6C4)
       Gouraud clip against x = z (code bit 2), like clip_right. */
L06C4: /* _seg004_6C4 */
    /* 06C4  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L06C9:
    /* 06C9  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L06CE:
    /* 06CE  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L06D2:
    /* 06D2  mov     di,word ptr ds:[VB_END] */
    DI = rw(pDS, 0x305);
L06D6:
    /* 06D6  mov     cx,8 */
    CX = 0x8;
L06D9:
    /* 06D9  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L06DB:
    /* 06DB  add     word ptr ds:[VB_END],8 */
    ww(pDS, 0x305, (uint16_t)(rw(pDS, 0x305) + 0x8));
L06E0:
    /* 06E0  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L06E4:
    /* 06E4  mov     di,VBUF1 */
    DI = 0xC69;
L06E7:
    /* 06E7  cmp     si,VBUF0 */
    sub16(SI, 0x309, 0);
L06EB:
    /* 06EB  je      short L06F0 */
    if (ZF) goto L06F0;
L06ED:
    /* 06ED  mov     di,VBUF0 */
    DI = 0x309;
L06F0: /* L06F0 */
    /* 06F0  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L06F4: /* L06F4 */
    /* 06F4  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L06F7: /* L06F7 */
    /* 06F7  cmp     si,word ptr ds:[VB_END] */
    sub16(SI, rw(pDS, 0x305), 0);
L06FB:
    /* 06FB  jne     short L0700 */
    if (!ZF) goto L0700;
L06FD:
    /* 06FD  jmp     L07F3 */
    goto L07F3;
L0700: /* L0700 */
    /* 0700  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L0703:
    /* 0703  test    al,2 */
    logic8((uint8_t)(AL & 0x2));
L0705:
    /* 0705  jne     short L0716 */
    if (!ZF) goto L0716;
L0707:
    /* 0707  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L070B:
    /* 070B  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L070F:
    /* 070F  mov     cx,4 */
    CX = 0x4;
L0712:
    /* 0712  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0714:
    /* 0714  jmp     L06F7 */
    goto L06F7;
L0716: /* L0716 */
    /* 0716  test    byte ptr [si-2],2 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFE) & 0x2));
L071A:
    /* 071A  jne     short L0782 */
    if (!ZF) goto L0782;
L071C:
    /* 071C  mov     bp,word ptr [si-8] */
    BP = rw(pDS, SI + 0xFFF8);
L071F:
    /* 071F  sub     bp,word ptr [si-4] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0xFFFC));
L0722:
    /* 0722  mov     cx,bp */
    CX = BP;
L0724:
    /* 0724  add     cx,word ptr [si+4] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0x4));
L0727:
    /* 0727  sub     cx,word ptr [si] */
    CX = (uint16_t)(CX - rw(pDS, SI));
L0729:
    /* 0729  mov     al,byte ptr [si+7] */
    AL = rb(pDS, SI + 0x7);
L072C:
    /* 072C  sub     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL - rb(pDS, SI + 0xF));
L072F:
    /* 072F  cbw */
    AX = (uint16_t)(int8_t)AL;
L0730:
    /* 0730  imul    bp */
    imul16(BP);
L0732:
    /* 0732  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x0732, 2)) != 0) return c;
L0734:
    /* 0734  add     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL + rb(pDS, SI + 0xF));
L0737:
    /* 0737  mov     byte ptr es:[di+7],al */
    wb(pES, DI + 0x7, AL);
L073B:
    /* 073B  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L073D:
    /* 073D  sub     ax,word ptr [si-8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF8));
L0740:
    /* 0740  imul    bp */
    imul16(BP);
L0742:
    /* 0742  shl     ax,1 */
    AX = shl16(AX, 1);
L0744:
    /* 0744  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0746:
    /* 0746  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x0746, 2)) != 0) return c;
L0748:
    /* 0748  sar     ax,1 */
    AX = sar16(AX, 1);
L074A:
    /* 074A  jae     short L074F */
    if (!CF) goto L074F;
L074C:
    /* 074C  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L074D:
    /* 074D  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L074F: /* L074F */
    /* 074F  add     ax,word ptr [si-8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF8));
L0752:
    /* 0752  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0753:
    /* 0753  mov     bx,ax */
    BX = AX;
L0755:
    /* 0755  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L0758:
    /* 0758  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L075B:
    /* 075B  imul    bp */
    imul16(BP);
L075D:
    /* 075D  shl     ax,1 */
    AX = shl16(AX, 1);
L075F:
    /* 075F  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0761:
    /* 0761  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x0761, 2)) != 0) return c;
L0763:
    /* 0763  sar     ax,1 */
    AX = sar16(AX, 1);
L0765:
    /* 0765  jae     short L076A */
    if (!CF) goto L076A;
L0767:
    /* 0767  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0768:
    /* 0768  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L076A: /* L076A */
    /* 076A  add     ax,word ptr [si-6] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFA));
L076D:
    /* 076D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L076E:
    /* 076E  mov     cx,ax */
    CX = AX;
L0770:
    /* 0770  mov     ax,bx */
    AX = BX;
L0772:
    /* 0772  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0773:
    /* 0773  mov     bp,ax */
    BP = AX;
L0775:
    /* 0775  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x0778)) != 0) return c;
L0778:
    /* 0778  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0779:
    /* 0779  inc     di */
    DI = (uint16_t)(DI + 1);
L077A:
    /* 077A  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L077E:
    /* 077E  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L0782: /* L0782 */
    /* 0782  test    byte ptr [si+0Eh],2 */
    logic8((uint8_t)(rb(pDS, SI + 0xE) & 0x2));
L0786:
    /* 0786  je      short L078B */
    if (ZF) goto L078B;
L0788:
    /* 0788  jmp     L06F4 */
    goto L06F4;
L078B: /* L078B */
    /* 078B  mov     bp,word ptr [si] */
    BP = rw(pDS, SI);
L078D:
    /* 078D  sub     bp,word ptr [si+4] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0x4));
L0790:
    /* 0790  mov     cx,bp */
    CX = BP;
L0792:
    /* 0792  add     cx,word ptr [si+0Ch] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0xC));
L0795:
    /* 0795  sub     cx,word ptr [si+8] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x8));
L0798:
    /* 0798  mov     al,byte ptr [si+7] */
    AL = rb(pDS, SI + 0x7);
L079B:
    /* 079B  sub     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL - rb(pDS, SI + 0xF));
L079E:
    /* 079E  cbw */
    AX = (uint16_t)(int8_t)AL;
L079F:
    /* 079F  imul    bp */
    imul16(BP);
L07A1:
    /* 07A1  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x07A1, 2)) != 0) return c;
L07A3:
    /* 07A3  add     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL + rb(pDS, SI + 0xF));
L07A6:
    /* 07A6  mov     byte ptr es:[di+7],al */
    wb(pES, DI + 0x7, AL);
L07AA:
    /* 07AA  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L07AD:
    /* 07AD  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L07AF:
    /* 07AF  imul    bp */
    imul16(BP);
L07B1:
    /* 07B1  shl     ax,1 */
    AX = shl16(AX, 1);
L07B3:
    /* 07B3  rcl     dx,1 */
    DX = rcl16(DX, 1);
L07B5:
    /* 07B5  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x07B5, 2)) != 0) return c;
L07B7:
    /* 07B7  sar     ax,1 */
    AX = sar16(AX, 1);
L07B9:
    /* 07B9  jae     short L07BE */
    if (!CF) goto L07BE;
L07BB:
    /* 07BB  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L07BC:
    /* 07BC  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L07BE: /* L07BE */
    /* 07BE  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L07C0:
    /* 07C0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L07C1:
    /* 07C1  mov     bx,ax */
    BX = AX;
L07C3:
    /* 07C3  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L07C6:
    /* 07C6  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L07C9:
    /* 07C9  imul    bp */
    imul16(BP);
L07CB:
    /* 07CB  shl     ax,1 */
    AX = shl16(AX, 1);
L07CD:
    /* 07CD  rcl     dx,1 */
    DX = rcl16(DX, 1);
L07CF:
    /* 07CF  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x07CF, 2)) != 0) return c;
L07D1:
    /* 07D1  sar     ax,1 */
    AX = sar16(AX, 1);
L07D3:
    /* 07D3  jae     short L07D8 */
    if (!CF) goto L07D8;
L07D5:
    /* 07D5  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L07D6:
    /* 07D6  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L07D8: /* L07D8 */
    /* 07D8  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L07DB:
    /* 07DB  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L07DC:
    /* 07DC  mov     cx,ax */
    CX = AX;
L07DE:
    /* 07DE  mov     ax,bx */
    AX = BX;
L07E0:
    /* 07E0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L07E1:
    /* 07E1  mov     bp,ax */
    BP = AX;
L07E3:
    /* 07E3  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x07E6)) != 0) return c;
L07E6:
    /* 07E6  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L07E7:
    /* 07E7  inc     di */
    DI = (uint16_t)(DI + 1);
L07E8:
    /* 07E8  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L07EC:
    /* 07EC  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L07F0:
    /* 07F0  jmp     L06F4 */
    goto L06F4;
L07F3: /* L07F3 */
    /* 07F3  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L07F7:
    /* 07F7  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_7F8  (+7F8)
       Gouraud clip against x = -z (code bit 1), like clip_left. */
L07F8: /* _seg004_7F8 */
    /* 07F8  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L07FD:
    /* 07FD  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L0802:
    /* 0802  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L0806:
    /* 0806  mov     di,word ptr ds:[VB_END] */
    DI = rw(pDS, 0x305);
L080A:
    /* 080A  mov     cx,8 */
    CX = 0x8;
L080D:
    /* 080D  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L080F:
    /* 080F  add     word ptr ds:[VB_END],8 */
    ww(pDS, 0x305, (uint16_t)(rw(pDS, 0x305) + 0x8));
L0814:
    /* 0814  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L0818:
    /* 0818  mov     di,VBUF1 */
    DI = 0xC69;
L081B:
    /* 081B  cmp     si,VBUF0 */
    sub16(SI, 0x309, 0);
L081F:
    /* 081F  je      short L0824 */
    if (ZF) goto L0824;
L0821:
    /* 0821  mov     di,VBUF0 */
    DI = 0x309;
L0824: /* L0824 */
    /* 0824  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L0828: /* L0828 */
    /* 0828  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L082B: /* L082B */
    /* 082B  cmp     si,word ptr ds:[VB_END] */
    sub16(SI, rw(pDS, 0x305), 0);
L082F:
    /* 082F  jne     short L0834 */
    if (!ZF) goto L0834;
L0831:
    /* 0831  jmp     L092B */
    goto L092B;
L0834: /* L0834 */
    /* 0834  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L0837:
    /* 0837  test    al,1 */
    logic8((uint8_t)(AL & 0x1));
L0839:
    /* 0839  jne     short L084A */
    if (!ZF) goto L084A;
L083B:
    /* 083B  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L083F:
    /* 083F  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L0843:
    /* 0843  mov     cx,4 */
    CX = 0x4;
L0846:
    /* 0846  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0848:
    /* 0848  jmp     L082B */
    goto L082B;
L084A: /* L084A */
    /* 084A  test    byte ptr [si-2],1 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFE) & 0x1));
L084E:
    /* 084E  jne     short L08B8 */
    if (!ZF) goto L08B8;
L0850:
    /* 0850  mov     bp,word ptr [si-4] */
    BP = rw(pDS, SI + 0xFFFC);
L0853:
    /* 0853  add     bp,word ptr [si-8] */
    BP = (uint16_t)(BP + rw(pDS, SI + 0xFFF8));
L0856:
    /* 0856  mov     cx,bp */
    CX = BP;
L0858:
    /* 0858  sub     cx,word ptr [si+4] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x4));
L085B:
    /* 085B  sub     cx,word ptr [si] */
    CX = (uint16_t)(CX - rw(pDS, SI));
L085D:
    /* 085D  mov     al,byte ptr [si+7] */
    AL = rb(pDS, SI + 0x7);
L0860:
    /* 0860  sub     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL - rb(pDS, SI + 0xF));
L0863:
    /* 0863  cbw */
    AX = (uint16_t)(int8_t)AL;
L0864:
    /* 0864  imul    bp */
    imul16(BP);
L0866:
    /* 0866  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x0866, 2)) != 0) return c;
L0868:
    /* 0868  add     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL + rb(pDS, SI + 0xF));
L086B:
    /* 086B  mov     byte ptr es:[di+7],al */
    wb(pES, DI + 0x7, AL);
L086F:
    /* 086F  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L0871:
    /* 0871  sub     ax,word ptr [si-8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF8));
L0874:
    /* 0874  imul    bp */
    imul16(BP);
L0876:
    /* 0876  shl     ax,1 */
    AX = shl16(AX, 1);
L0878:
    /* 0878  rcl     dx,1 */
    DX = rcl16(DX, 1);
L087A:
    /* 087A  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x087A, 2)) != 0) return c;
L087C:
    /* 087C  sar     ax,1 */
    AX = sar16(AX, 1);
L087E:
    /* 087E  jae     short L0883 */
    if (!CF) goto L0883;
L0880:
    /* 0880  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0881:
    /* 0881  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L0883: /* L0883 */
    /* 0883  add     ax,word ptr [si-8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF8));
L0886:
    /* 0886  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0887:
    /* 0887  mov     bx,ax */
    BX = AX;
L0889:
    /* 0889  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L088C:
    /* 088C  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L088F:
    /* 088F  imul    bp */
    imul16(BP);
L0891:
    /* 0891  shl     ax,1 */
    AX = shl16(AX, 1);
L0893:
    /* 0893  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0895:
    /* 0895  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x0895, 2)) != 0) return c;
L0897:
    /* 0897  sar     ax,1 */
    AX = sar16(AX, 1);
L0899:
    /* 0899  jae     short L089E */
    if (!CF) goto L089E;
L089B:
    /* 089B  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L089C:
    /* 089C  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L089E: /* L089E */
    /* 089E  add     ax,word ptr [si-6] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFA));
L08A1:
    /* 08A1  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L08A2:
    /* 08A2  mov     cx,ax */
    CX = AX;
L08A4:
    /* 08A4  mov     ax,bx */
    AX = BX;
L08A6:
    /* 08A6  neg     ax */
    AX = (uint16_t)-AX;
L08A8:
    /* 08A8  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L08A9:
    /* 08A9  mov     bp,ax */
    BP = AX;
L08AB:
    /* 08AB  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x08AE)) != 0) return c;
L08AE:
    /* 08AE  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L08AF:
    /* 08AF  inc     di */
    DI = (uint16_t)(DI + 1);
L08B0:
    /* 08B0  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L08B4:
    /* 08B4  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L08B8: /* L08B8 */
    /* 08B8  test    byte ptr [si+0Eh],1 */
    logic8((uint8_t)(rb(pDS, SI + 0xE) & 0x1));
L08BC:
    /* 08BC  je      short L08C1 */
    if (ZF) goto L08C1;
L08BE:
    /* 08BE  jmp     L0828 */
    goto L0828;
L08C1: /* L08C1 */
    /* 08C1  mov     bp,word ptr [si+4] */
    BP = rw(pDS, SI + 0x4);
L08C4:
    /* 08C4  add     bp,word ptr [si] */
    BP = (uint16_t)(BP + rw(pDS, SI));
L08C6:
    /* 08C6  mov     cx,bp */
    CX = BP;
L08C8:
    /* 08C8  sub     cx,word ptr [si+8] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x8));
L08CB:
    /* 08CB  sub     cx,word ptr [si+0Ch] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xC));
L08CE:
    /* 08CE  mov     al,byte ptr [si+7] */
    AL = rb(pDS, SI + 0x7);
L08D1:
    /* 08D1  sub     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL - rb(pDS, SI + 0xF));
L08D4:
    /* 08D4  cbw */
    AX = (uint16_t)(int8_t)AL;
L08D5:
    /* 08D5  imul    bp */
    imul16(BP);
L08D7:
    /* 08D7  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x08D7, 2)) != 0) return c;
L08D9:
    /* 08D9  add     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL + rb(pDS, SI + 0xF));
L08DC:
    /* 08DC  mov     byte ptr es:[di+7],al */
    wb(pES, DI + 0x7, AL);
L08E0:
    /* 08E0  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L08E3:
    /* 08E3  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L08E5:
    /* 08E5  imul    bp */
    imul16(BP);
L08E7:
    /* 08E7  shl     ax,1 */
    AX = shl16(AX, 1);
L08E9:
    /* 08E9  rcl     dx,1 */
    DX = rcl16(DX, 1);
L08EB:
    /* 08EB  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x08EB, 2)) != 0) return c;
L08ED:
    /* 08ED  sar     ax,1 */
    AX = sar16(AX, 1);
L08EF:
    /* 08EF  jae     short L08F4 */
    if (!CF) goto L08F4;
L08F1:
    /* 08F1  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L08F2:
    /* 08F2  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L08F4: /* L08F4 */
    /* 08F4  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L08F6:
    /* 08F6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L08F7:
    /* 08F7  mov     bx,ax */
    BX = AX;
L08F9:
    /* 08F9  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L08FC:
    /* 08FC  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L08FF:
    /* 08FF  imul    bp */
    imul16(BP);
L0901:
    /* 0901  shl     ax,1 */
    AX = shl16(AX, 1);
L0903:
    /* 0903  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0905:
    /* 0905  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x0905, 2)) != 0) return c;
L0907:
    /* 0907  sar     ax,1 */
    AX = sar16(AX, 1);
L0909:
    /* 0909  jae     short L090E */
    if (!CF) goto L090E;
L090B:
    /* 090B  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L090C:
    /* 090C  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L090E: /* L090E */
    /* 090E  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L0911:
    /* 0911  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0912:
    /* 0912  mov     cx,ax */
    CX = AX;
L0914:
    /* 0914  mov     ax,bx */
    AX = BX;
L0916:
    /* 0916  neg     ax */
    AX = (uint16_t)-AX;
L0918:
    /* 0918  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0919:
    /* 0919  mov     bp,ax */
    BP = AX;
L091B:
    /* 091B  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x091E)) != 0) return c;
L091E:
    /* 091E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L091F:
    /* 091F  inc     di */
    DI = (uint16_t)(DI + 1);
L0920:
    /* 0920  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L0924:
    /* 0924  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L0928:
    /* 0928  jmp     L0828 */
    goto L0828;
L092B: /* L092B */
    /* 092B  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L092F:
    /* 092F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* smooth_over  (+930): FM smooth_over, the same loop (copy the projected x and y, then
       an intensity from the vertex normal's length)
       In: CX = the vertex count. For each 14-byte record at seg048:659h it copies the screen x
       and y to 415Ch, takes the three words at +8 (each >> 5), and computes the length of that
       vector by four Newton steps from 64; the shade is the low bytes of the length and [64Ch]
       multiplied, >> 6, plus [650h] (at least 0) plus [64Eh], at most 15. Then it points
       seg003's handler pair at 0F27h/0F29h at 0F56h/103Dh (as do_transsurf does) and calls the
       polygon routine with BP = _seg003_6159 and CX = ds:[0B07Ah] of seg051. Called by
       TMAPOPS.ASM's seg004_6130 (UW1 only). */
L0930: /* _smooth_over */
    /* 0930  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L0933:
    /* 0933  mov     es,ax */
    SET_ES(AX);
L0935:
    /* 0935  mov     ds,ax */
    SET_DS(AX);
L0937:
    /* 0937  mov     di,415Ch */
    DI = 0x415C;
L093A:
    /* 093A  mov     si,659h */
    SI = 0x659;
L093D:
    /* 093D  mov     bp,cx */
    BP = CX;
L093F: /* L093F */
    /* 093F  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L0940:
    /* 0940  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L0941:
    /* 0941  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L0944:
    /* 0944  xor     cx,cx */
    CX = (uint16_t)(CX ^ CX);
L0946:
    /* 0946  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0947:
    /* 0947  sar     ax,5 */
    AX = (uint16_t)((int16_t)AX >> 5);
L094A:
    /* 094A  mov     dx,ax */
    DX = AX;
L094C:
    /* 094C  imul    dx */
    imul16(DX);
L094E:
    /* 094E  add     cx,ax */
    CX = (uint16_t)(CX + AX);
L0950:
    /* 0950  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0951:
    /* 0951  sar     ax,5 */
    AX = (uint16_t)((int16_t)AX >> 5);
L0954:
    /* 0954  mov     dx,ax */
    DX = AX;
L0956:
    /* 0956  imul    dx */
    imul16(DX);
L0958:
    /* 0958  add     cx,ax */
    CX = (uint16_t)(CX + AX);
L095A:
    /* 095A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L095B:
    /* 095B  sar     ax,5 */
    AX = (uint16_t)((int16_t)AX >> 5);
L095E:
    /* 095E  mov     dx,ax */
    DX = AX;
L0960:
    /* 0960  imul    dx */
    imul16(DX);
L0962:
    /* 0962  add     cx,ax */
    CX = (uint16_t)(CX + AX);
L0964:
    /* 0964  mov     bx,cx */
    BX = CX;
L0966:
    /* 0966  mov     cx,40h */
    CX = 0x40;
L0969:
    /* 0969  mov     ax,bx */
    AX = BX;
L096B:
    /* 096B  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L096D:
    /* 096D  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x06E7, 0x096D, 2)) != 0) return c;
L096F:
    /* 096F  add     cx,ax */
    CX = add16(CX, AX, 0);
L0971:
    /* 0971  rcr     cx,1 */
    CX = rcr16(CX, 1);
L0973:
    /* 0973  mov     ax,bx */
    AX = BX;
L0975:
    /* 0975  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L0977:
    /* 0977  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x06E7, 0x0977, 2)) != 0) return c;
L0979:
    /* 0979  add     cx,ax */
    CX = add16(CX, AX, 0);
L097B:
    /* 097B  rcr     cx,1 */
    CX = rcr16(CX, 1);
L097D:
    /* 097D  mov     ax,bx */
    AX = BX;
L097F:
    /* 097F  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L0981:
    /* 0981  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x06E7, 0x0981, 2)) != 0) return c;
L0983:
    /* 0983  add     cx,ax */
    CX = add16(CX, AX, 0);
L0985:
    /* 0985  rcr     cx,1 */
    CX = rcr16(CX, 1);
L0987:
    /* 0987  mov     ax,bx */
    AX = BX;
L0989:
    /* 0989  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L098B:
    /* 098B  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x06E7, 0x098B, 2)) != 0) return c;
L098D:
    /* 098D  add     cx,ax */
    CX = add16(CX, AX, 0);
L098F:
    /* 098F  rcr     cx,1 */
    CX = rcr16(CX, 1);
L0991:
    /* 0991  mov     ax,cx */
    AX = CX;
L0993:
    /* 0993  mov     cx,word ptr ds:[64Ch] */
    CX = rw(pDS, 0x64C);
L0997:
    /* 0997  mul     cl */
    mul8(CL);
L0999:
    /* 0999  shr     ax,6 */
    AX = (uint16_t)(AX >> 6);
L099C:
    /* 099C  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L099E:
    /* 099E  add     ax,word ptr ds:[650h] */
    AX = add16(AX, rw(pDS, 0x650), 0);
L09A2:
    /* 09A2  jns     short L09A6 */
    if (!SF) goto L09A6;
L09A4:
    /* 09A4  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L09A6: /* L09A6 */
    /* 09A6  add     ax,word ptr ds:[64Eh] */
    AX = (uint16_t)(AX + rw(pDS, 0x64E));
L09AA:
    /* 09AA  cmp     al,0Fh */
    sub8(AL, 0xF, 0);
L09AC:
    /* 09AC  jbe     short L09B0 */
    if (CF || ZF) goto L09B0;
L09AE:
    /* 09AE  mov     al,0Fh */
    AL = 0xF;
L09B0: /* L09B0 */
    /* 09B0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L09B1:
    /* 09B1  dec     bp */
    BP = dec16(BP);
L09B2:
    /* 09B2  jne     L093F */
    if (!ZF) goto L093F;
L09B4:
    /* 09B4  mov     bp,offset _seg003_6159 */
    BP = 0x6159;
L09B7:
    /* 09B7  mov     bx,es */
    BX = asm_es;
L09B9:
    /* 09B9  mov     ax,seg seg003 */
    AX = (uint16_t)(0x0090 + PORT_LOAD_SEG);
L09BC:
    /* 09BC  mov     es,ax */
    SET_ES(AX);
L09BE:
    /* 09BE  mov     word ptr es:[0F27h],0F56h */
    ww(pES, 0xF27, 0xF56);
L09C5:
    /* 09C5  mov     word ptr es:[0F29h],103Dh */
    ww(pES, 0xF29, 0x103D);
L09CC:
    /* 09CC  mov     es,bx */
    SET_ES(BX);
L09CE:
    /* 09CE  mov     bx,seg seg051 */
    BX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L09D1:
    /* 09D1  mov     es,bx */
    SET_ES(BX);
L09D3:
    /* 09D3  mov     ds,bx */
    SET_DS(BX);
L09D5:
    /* 09D5  mov     cx,word ptr ds:[0B07Ah] */
    CX = rw(pDS, 0xB07A);
L09D9:
    /* 09D9  call    far ptr _seg003_5B0B */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x5B0B), 0x06E7 + PORT_LOAD_SEG, 0x09DE)) != 0) return c;
L09DE:
    /* 09DE  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* UW1: the opcode table's do_uwsetup slot (0DAh) points here, at an int 2 (UW2's points at
       the data word below), so an opcode 0DAh traps. */
L09DF: /* do_uwsetup */
    /* 09DF  int     2 */
    asm_halt_at(0x06E7, 0x09DF, "int 2h, the debugger break");
    asm_halt_at(0x06E7, 0x09E1, "ran off the code into data");

    /* seg004_9E5  (+9E5): divide overflow handler, installed by the smooth shading code
       (stored at ss:[4D5h] while clipping). Skips the 2-byte idiv, then retries the divide with
       the dividend halved (sar dx; rcr ax), or returns 7FFFh when the halved high word equals CX
       (a partial overflow check). */
L09E5: /* _seg004_9E5 */
    /* 09E5  mov     word ptr cs:L09E1,bp */
    ww(CODE004, 0x9E1, BP);
L09EA:
    /* 09EA  mov     bp,sp */
    BP = SP;
L09EC:
    /* 09EC  add     word ptr [bp],2 */
    ww(pSS, BP, (uint16_t)(rw(pSS, BP) + 0x2));
L09F0:
    /* 09F0  sar     dx,1 */
    DX = sar16(DX, 1);
L09F2:
    /* 09F2  rcr     ax,1 */
    AX = rcr16(AX, 1);
L09F4:
    /* 09F4  cmp     dx,cx */
    sub16(DX, CX, 0);
L09F6:
    /* 09F6  je      short L0A00 */
    if (ZF) goto L0A00;
L09F8:
    /* 09F8  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x09F8, 2)) != 0) return c;
L09FA:
    /* 09FA  mov     bp,word ptr cs:L09E1 */
    BP = rw(CODE004, 0x9E1);
L09FF:
    /* 09FF  iret */
    SP = (uint16_t)(SP + 4); asm_set_flags(pop16()); return ASM_IRET;
L0A00: /* L0A00 */
    /* 0A00  mov     ax,7FFFh */
    AX = 0x7FFF;
L0A03:
    /* 0A03  xor     dx,dx */
    DX = logic16((uint16_t)(DX ^ DX));
L0A05:
    /* 0A05  mov     bp,word ptr cs:L09E1 */
    BP = rw(CODE004, 0x9E1);
L0A0A:
    /* 0A0A  iret */
    SP = (uint16_t)(SP + 4); asm_set_flags(pop16()); return ASM_IRET;
}
