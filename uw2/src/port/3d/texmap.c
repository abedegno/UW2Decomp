/* texmap.c: replaces src/3d/TEXMAP.ASM (seg004_0849_54C0, 54C0..6236 of its
   segment). Written by tools/asm2c.py from the assembly and the bytes it assembles to; do not
   edit by hand: change the tool, or its OVERRIDES, and run it again. Each instruction is the
   C below its source line; the comments before a routine are the .ASM file's. */
#include "x86/asmrt.h"

#include "portgame.h"
#include "sys/enhance.h"     /* ENHANCED */
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

uint32_t asm_mod_TEXMAP(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x54C0: goto L54C0;
    case 0x54C3: goto L54C3;
    case 0x54C4: goto L54C4;
    case 0x54C6: goto L54C6;
    case 0x54C9: goto L54C9;
    case 0x54CA: goto L54CA;
    case 0x54CD: goto L54CD;
    case 0x54CF: goto L54CF;
    case 0x54D2: goto L54D2;
    case 0x54D3: goto L54D3;
    case 0x54D6: goto L54D6;
    case 0x54D9: goto L54D9;
    case 0x54DB: goto L54DB;
    case 0x54DE: goto L54DE;
    case 0x54E1: goto L54E1;
    case 0x54E3: goto L54E3;
    case 0x54E6: goto L54E6;
    case 0x54E7: goto L54E7;
    case 0x54EB: goto L54EB;
    case 0x54EF: goto L54EF;
    case 0x54F3: goto L54F3;
    case 0x54F7: goto L54F7;
    case 0x54FB: goto L54FB;
    case 0x54FC: goto L54FC;
    case 0x54FE: goto L54FE;
    case 0x5507: goto L5507;
    case 0x5509: goto L5509;
    case 0x550F: goto L550F;
    case 0x5513: goto L5513;
    case 0x5515: goto L5515;
    case 0x551B: goto L551B;
    case 0x551D: goto L551D;
    case 0x5523: goto L5523;
    case 0x5527: goto L5527;
    case 0x552B: goto L552B;
    case 0x552D: goto L552D;
    case 0x5531: goto L5531;
    case 0x5535: goto L5535;
    case 0x5539: goto L5539;
    case 0x553D: goto L553D;
    case 0x5541: goto L5541;
    case 0x5545: goto L5545;
    case 0x554A: goto L554A;
    case 0x554C: goto L554C;
    case 0x5550: goto L5550;
    case 0x5554: goto L5554;
    case 0x5559: goto L5559;
    case 0x555E: goto L555E;
    case 0x5560: goto L5560;
    case 0x5564: goto L5564;
    case 0x5568: goto L5568;
    case 0x556C: goto L556C;
    case 0x5570: goto L5570;
    case 0x5574: goto L5574;
    case 0x5578: goto L5578;
    case 0x557C: goto L557C;
    case 0x5580: goto L5580;
    case 0x5583: goto L5583;
    case 0x5587: goto L5587;
    case 0x558B: goto L558B;
    case 0x558D: goto L558D;
    case 0x5590: goto L5590;
    case 0x5594: goto L5594;
    case 0x5598: goto L5598;
    case 0x559C: goto L559C;
    case 0x55A0: goto L55A0;
    case 0x55A4: goto L55A4;
    case 0x55A7: goto L55A7;
    case 0x55AB: goto L55AB;
    case 0x55AF: goto L55AF;
    case 0x55B3: goto L55B3;
    case 0x55B7: goto L55B7;
    case 0x55B9: goto L55B9;
    case 0x55BE: goto L55BE;
    case 0x55C2: goto L55C2;
    case 0x55C6: goto L55C6;
    case 0x55CA: goto L55CA;
    case 0x55CE: goto L55CE;
    case 0x55D2: goto L55D2;
    case 0x55D4: goto L55D4;
    case 0x55D9: goto L55D9;
    case 0x55DD: goto L55DD;
    case 0x55E1: goto L55E1;
    case 0x55E2: goto L55E2;
    case 0x55E4: goto L55E4;
    case 0x55E8: goto L55E8;
    case 0x55EC: goto L55EC;
    case 0x55F0: goto L55F0;
    case 0x55F4: goto L55F4;
    case 0x55F6: goto L55F6;
    case 0x55FB: goto L55FB;
    case 0x55FF: goto L55FF;
    case 0x5603: goto L5603;
    case 0x5607: goto L5607;
    case 0x560B: goto L560B;
    case 0x560F: goto L560F;
    case 0x5611: goto L5611;
    case 0x5616: goto L5616;
    case 0x561A: goto L561A;
    case 0x5623: goto L5623;
    case 0x5626: goto L5626;
    case 0x562A: goto L562A;
    case 0x562E: goto L562E;
    case 0x5632: goto L5632;
    case 0x5634: goto L5634;
    case 0x5638: goto L5638;
    case 0x563B: goto L563B;
    case 0x563F: goto L563F;
    case 0x5643: goto L5643;
    case 0x5647: goto L5647;
    case 0x5649: goto L5649;
    case 0x564D: goto L564D;
    case 0x5650: goto L5650;
    case 0x5654: goto L5654;
    case 0x5658: goto L5658;
    case 0x565C: goto L565C;
    case 0x565E: goto L565E;
    case 0x5663: goto L5663;
    case 0x5667: goto L5667;
    case 0x566B: goto L566B;
    case 0x566F: goto L566F;
    case 0x5673: goto L5673;
    case 0x5675: goto L5675;
    case 0x5679: goto L5679;
    case 0x567C: goto L567C;
    case 0x5680: goto L5680;
    case 0x5684: goto L5684;
    case 0x5688: goto L5688;
    case 0x568A: goto L568A;
    case 0x568E: goto L568E;
    case 0x5691: goto L5691;
    case 0x5695: goto L5695;
    case 0x5699: goto L5699;
    case 0x569D: goto L569D;
    case 0x569F: goto L569F;
    case 0x56A4: goto L56A4;
    case 0x56A8: goto L56A8;
    case 0x56AE: goto L56AE;
    case 0x56B2: goto L56B2;
    case 0x56B4: goto L56B4;
    case 0x56BA: goto L56BA;
    case 0x56BC: goto L56BC;
    case 0x56C2: goto L56C2;
    case 0x56C6: goto L56C6;
    case 0x56CA: goto L56CA;
    case 0x56CC: goto L56CC;
    case 0x56D0: goto L56D0;
    case 0x56D3: goto L56D3;
    case 0x56D9: goto L56D9;
    case 0x56DD: goto L56DD;
    case 0x56DF: goto L56DF;
    case 0x56E5: goto L56E5;
    case 0x56E7: goto L56E7;
    case 0x56ED: goto L56ED;
    case 0x56F1: goto L56F1;
    case 0x56F5: goto L56F5;
    case 0x56F7: goto L56F7;
    case 0x56FB: goto L56FB;
    case 0x56FE: goto L56FE;
    case 0x5702: goto L5702;
    case 0x5706: goto L5706;
    case 0x5708: goto L5708;
    case 0x570D: goto L570D;
    case 0x5711: goto L5711;
    case 0x5712: goto L5712;
    case 0x5715: goto L5715;
    case 0x5716: goto L5716;
    case 0x5719: goto L5719;
    case 0x571A: goto L571A;
    case 0x571C: goto L571C;
    case 0x571F: goto L571F;
    case 0x5720: goto L5720;
    case 0x5722: goto L5722;
    case 0x5725: goto L5725;
    case 0x5728: goto L5728;
    case 0x5729: goto L5729;
    case 0x572C: goto L572C;
    case 0x572E: goto L572E;
    case 0x572F: goto L572F;
    case 0x5732: goto L5732;
    case 0x5733: goto L5733;
    case 0x5735: goto L5735;
    case 0x5738: goto L5738;
    case 0x5739: goto L5739;
    case 0x573C: goto L573C;
    case 0x573F: goto L573F;
    case 0x5740: goto L5740;
    case 0x5743: goto L5743;
    case 0x5747: goto L5747;
    case 0x574B: goto L574B;
    case 0x574F: goto L574F;
    case 0x5753: goto L5753;
    case 0x5754: goto L5754;
    case 0x5756: goto L5756;
    case 0x575F: goto L575F;
    case 0x5761: goto L5761;
    case 0x5767: goto L5767;
    case 0x576B: goto L576B;
    case 0x576D: goto L576D;
    case 0x5773: goto L5773;
    case 0x5775: goto L5775;
    case 0x577B: goto L577B;
    case 0x577F: goto L577F;
    case 0x5783: goto L5783;
    case 0x5785: goto L5785;
    case 0x5789: goto L5789;
    case 0x578D: goto L578D;
    case 0x5791: goto L5791;
    case 0x5795: goto L5795;
    case 0x5799: goto L5799;
    case 0x579D: goto L579D;
    case 0x57A2: goto L57A2;
    case 0x57A4: goto L57A4;
    case 0x57A8: goto L57A8;
    case 0x57AC: goto L57AC;
    case 0x57B1: goto L57B1;
    case 0x57B6: goto L57B6;
    case 0x57B8: goto L57B8;
    case 0x57BC: goto L57BC;
    case 0x57C0: goto L57C0;
    case 0x57C4: goto L57C4;
    case 0x57C8: goto L57C8;
    case 0x57CC: goto L57CC;
    case 0x57D0: goto L57D0;
    case 0x57D4: goto L57D4;
    case 0x57D8: goto L57D8;
    case 0x57DB: goto L57DB;
    case 0x57DF: goto L57DF;
    case 0x57E3: goto L57E3;
    case 0x57E5: goto L57E5;
    case 0x57E8: goto L57E8;
    case 0x57EC: goto L57EC;
    case 0x57F0: goto L57F0;
    case 0x57F4: goto L57F4;
    case 0x57F8: goto L57F8;
    case 0x57FC: goto L57FC;
    case 0x57FF: goto L57FF;
    case 0x5803: goto L5803;
    case 0x5807: goto L5807;
    case 0x580B: goto L580B;
    case 0x580F: goto L580F;
    case 0x5811: goto L5811;
    case 0x5816: goto L5816;
    case 0x581A: goto L581A;
    case 0x581E: goto L581E;
    case 0x5822: goto L5822;
    case 0x5826: goto L5826;
    case 0x582A: goto L582A;
    case 0x582C: goto L582C;
    case 0x5831: goto L5831;
    case 0x5835: goto L5835;
    case 0x5839: goto L5839;
    case 0x583A: goto L583A;
    case 0x583C: goto L583C;
    case 0x5840: goto L5840;
    case 0x5844: goto L5844;
    case 0x5848: goto L5848;
    case 0x584C: goto L584C;
    case 0x584E: goto L584E;
    case 0x5853: goto L5853;
    case 0x5857: goto L5857;
    case 0x585B: goto L585B;
    case 0x585F: goto L585F;
    case 0x5863: goto L5863;
    case 0x5867: goto L5867;
    case 0x5869: goto L5869;
    case 0x586E: goto L586E;
    case 0x5872: goto L5872;
    case 0x5875: goto L5875;
    case 0x5879: goto L5879;
    case 0x587C: goto L587C;
    case 0x5880: goto L5880;
    case 0x5884: goto L5884;
    case 0x5888: goto L5888;
    case 0x588A: goto L588A;
    case 0x588E: goto L588E;
    case 0x5891: goto L5891;
    case 0x5895: goto L5895;
    case 0x5899: goto L5899;
    case 0x589D: goto L589D;
    case 0x589F: goto L589F;
    case 0x58A3: goto L58A3;
    case 0x58A6: goto L58A6;
    case 0x58AA: goto L58AA;
    case 0x58AE: goto L58AE;
    case 0x58B2: goto L58B2;
    case 0x58B4: goto L58B4;
    case 0x58B9: goto L58B9;
    case 0x58BD: goto L58BD;
    case 0x58C1: goto L58C1;
    case 0x58C3: goto L58C3;
    case 0x58C8: goto L58C8;
    case 0x58CC: goto L58CC;
    case 0x58D0: goto L58D0;
    case 0x58D3: goto L58D3;
    case 0x58D7: goto L58D7;
    case 0x58D9: goto L58D9;
    case 0x58DE: goto L58DE;
    case 0x58E2: goto L58E2;
    case 0x58E6: goto L58E6;
    case 0x58E9: goto L58E9;
    case 0x58ED: goto L58ED;
    case 0x58F1: goto L58F1;
    case 0x58F3: goto L58F3;
    case 0x58F8: goto L58F8;
    case 0x58FC: goto L58FC;
    case 0x5902: goto L5902;
    case 0x5906: goto L5906;
    case 0x5908: goto L5908;
    case 0x590E: goto L590E;
    case 0x5910: goto L5910;
    case 0x5916: goto L5916;
    case 0x591A: goto L591A;
    case 0x591E: goto L591E;
    case 0x5920: goto L5920;
    case 0x5924: goto L5924;
    case 0x5927: goto L5927;
    case 0x592D: goto L592D;
    case 0x5931: goto L5931;
    case 0x5933: goto L5933;
    case 0x5939: goto L5939;
    case 0x593B: goto L593B;
    case 0x5941: goto L5941;
    case 0x5945: goto L5945;
    case 0x5949: goto L5949;
    case 0x594B: goto L594B;
    case 0x594F: goto L594F;
    case 0x5952: goto L5952;
    case 0x5956: goto L5956;
    case 0x595A: goto L595A;
    case 0x595C: goto L595C;
    case 0x5961: goto L5961;
    case 0x5965: goto L5965;
    case 0x5966: goto L5966;
    case 0x596F: goto L596F;
    case 0x5978: goto L5978;
    case 0x597B: goto L597B;
    case 0x597C: goto L597C;
    case 0x597E: goto L597E;
    case 0x5982: goto L5982;
    case 0x5987: goto L5987;
    case 0x5989: goto L5989;
    case 0x598D: goto L598D;
    case 0x5991: goto L5991;
    case 0x5996: goto L5996;
    case 0x5998: goto L5998;
    case 0x599C: goto L599C;
    case 0x599D: goto L599D;
    case 0x59A0: goto L59A0;
    case 0x59A3: goto L59A3;
    case 0x59A4: goto L59A4;
    case 0x59A6: goto L59A6;
    case 0x59A9: goto L59A9;
    case 0x59AC: goto L59AC;
    case 0x59AF: goto L59AF;
    case 0x59B4: goto L59B4;
    case 0x59B7: goto L59B7;
    case 0x59B8: goto L59B8;
    case 0x59BC: goto L59BC;
    case 0x59C0: goto L59C0;
    case 0x59C4: goto L59C4;
    case 0x59C8: goto L59C8;
    case 0x59C9: goto L59C9;
    case 0x59CC: goto L59CC;
    case 0x59CD: goto L59CD;
    case 0x59CF: goto L59CF;
    case 0x59D1: goto L59D1;
    case 0x59D3: goto L59D3;
    case 0x59D6: goto L59D6;
    case 0x59D9: goto L59D9;
    case 0x59DA: goto L59DA;
    case 0x59DE: goto L59DE;
    case 0x59E2: goto L59E2;
    case 0x59E6: goto L59E6;
    case 0x59E8: goto L59E8;
    case 0x59EA: goto L59EA;
    case 0x59EE: goto L59EE;
    case 0x59F2: goto L59F2;
    case 0x59F3: goto L59F3;
    case 0x59F5: goto L59F5;
    case 0x59F8: goto L59F8;
    case 0x59F9: goto L59F9;
    case 0x59FC: goto L59FC;
    case 0x59FF: goto L59FF;
    case 0x5A00: goto L5A00;
    case 0x5A04: goto L5A04;
    case 0x5A08: goto L5A08;
    case 0x5A0C: goto L5A0C;
    case 0x5A0E: goto L5A0E;
    case 0x5A10: goto L5A10;
    case 0x5A13: goto L5A13;
    case 0x5A16: goto L5A16;
    case 0x5A1A: goto L5A1A;
    case 0x5A1C: goto L5A1C;
    case 0x5A1F: goto L5A1F;
    case 0x5A23: goto L5A23;
    case 0x5A27: goto L5A27;
    case 0x5A29: goto L5A29;
    case 0x5A2D: goto L5A2D;
    case 0x5A31: goto L5A31;
    case 0x5A33: goto L5A33;
    case 0x5A35: goto L5A35;
    case 0x5A37: goto L5A37;
    case 0x5A39: goto L5A39;
    case 0x5A3B: goto L5A3B;
    case 0x5A3E: goto L5A3E;
    case 0x5A41: goto L5A41;
    case 0x5A45: goto L5A45;
    case 0x5A49: goto L5A49;
    case 0x5A4B: goto L5A4B;
    case 0x5A4D: goto L5A4D;
    case 0x5A50: goto L5A50;
    case 0x5A53: goto L5A53;
    case 0x5A54: goto L5A54;
    case 0x5A55: goto L5A55;
    case 0x5A57: goto L5A57;
    case 0x5A58: goto L5A58;
    case 0x5A59: goto L5A59;
    case 0x5A5A: goto L5A5A;
    case 0x5A5D: goto L5A5D;
    case 0x5A5F: goto L5A5F;
    case 0x5A64: goto L5A64;
    case 0x5A66: goto L5A66;
    case 0x5A69: goto L5A69;
    case 0x5A6C: goto L5A6C;
    case 0x5A6F: goto L5A6F;
    case 0x5A72: goto L5A72;
    case 0x5A75: goto L5A75;
    case 0x5A77: goto L5A77;
    case 0x5A7A: goto L5A7A;
    case 0x5A7D: goto L5A7D;
    case 0x5A83: goto L5A83;
    case 0x5A86: goto L5A86;
    case 0x5A89: goto L5A89;
    case 0x5A8C: goto L5A8C;
    case 0x5A92: goto L5A92;
    case 0x5A95: goto L5A95;
    case 0x5A98: goto L5A98;
    case 0x5A9B: goto L5A9B;
    case 0x5A9E: goto L5A9E;
    case 0x5AA1: goto L5AA1;
    case 0x5AA4: goto L5AA4;
    case 0x5AA8: goto L5AA8;
    case 0x5AAC: goto L5AAC;
    case 0x5AB0: goto L5AB0;
    case 0x5AB4: goto L5AB4;
    case 0x5AB8: goto L5AB8;
    case 0x5ABC: goto L5ABC;
    case 0x5AC0: goto L5AC0;
    case 0x5AC4: goto L5AC4;
    case 0x5AC8: goto L5AC8;
    case 0x5AC9: goto L5AC9;
    case 0x5ACB: goto L5ACB;
    case 0x5ACE: goto L5ACE;
    case 0x5AD1: goto L5AD1;
    case 0x5AD4: goto L5AD4;
    case 0x5AD8: goto L5AD8;
    case 0x5ADC: goto L5ADC;
    case 0x5AE0: goto L5AE0;
    case 0x5AE4: goto L5AE4;
    case 0x5AE6: goto L5AE6;
    case 0x5AE9: goto L5AE9;
    case 0x5AED: goto L5AED;
    case 0x5AEF: goto L5AEF;
    case 0x5AF2: goto L5AF2;
    case 0x5AF6: goto L5AF6;
    case 0x5AFA: goto L5AFA;
    case 0x5AFE: goto L5AFE;
    case 0x5B02: goto L5B02;
    case 0x5B06: goto L5B06;
    case 0x5B0A: goto L5B0A;
    case 0x5B0B: goto L5B0B;
    case 0x5B0E: goto L5B0E;
    case 0x5B10: goto L5B10;
    case 0x5B12: goto L5B12;
    case 0x5B15: goto L5B15;
    case 0x5B18: goto L5B18;
    case 0x5B1C: goto L5B1C;
    case 0x5B20: goto L5B20;
    case 0x5B24: goto L5B24;
    case 0x5B28: goto L5B28;
    case 0x5B2A: goto L5B2A;
    case 0x5B2D: goto L5B2D;
    case 0x5B31: goto L5B31;
    case 0x5B34: goto L5B34;
    case 0x5B37: goto L5B37;
    case 0x5B39: goto L5B39;
    case 0x5B3F: goto L5B3F;
    case 0x5B43: goto L5B43;
    case 0x5B47: goto L5B47;
    case 0x5B4B: goto L5B4B;
    case 0x5B4F: goto L5B4F;
    case 0x5B52: goto L5B52;
    case 0x5B55: goto L5B55;
    case 0x5B5B: goto L5B5B;
    case 0x5B5D: goto L5B5D;
    case 0x5B62: goto L5B62;
    case 0x5B66: goto L5B66;
    case 0x5B6A: goto L5B6A;
    case 0x5B6E: goto L5B6E;
    case 0x5B70: goto L5B70;
    case 0x5B75: goto L5B75;
    case 0x5B79: goto L5B79;
    case 0x5B7D: goto L5B7D;
    case 0x5B81: goto L5B81;
    case 0x5B85: goto L5B85;
    case 0x5B87: goto L5B87;
    case 0x5B8C: goto L5B8C;
    case 0x5B90: goto L5B90;
    case 0x5B94: goto L5B94;
    case 0x5B98: goto L5B98;
    case 0x5B9C: goto L5B9C;
    case 0x5B9E: goto L5B9E;
    case 0x5BA3: goto L5BA3;
    case 0x5BA7: goto L5BA7;
    case 0x5BAB: goto L5BAB;
    case 0x5BAF: goto L5BAF;
    case 0x5BB3: goto L5BB3;
    case 0x5BB5: goto L5BB5;
    case 0x5BBA: goto L5BBA;
    case 0x5BBE: goto L5BBE;
    case 0x5BC3: goto L5BC3;
    case 0x5BC7: goto L5BC7;
    case 0x5BCA: goto L5BCA;
    case 0x5BCB: goto L5BCB;
    case 0x5BCF: goto L5BCF;
    case 0x5BD0: goto L5BD0;
    case 0x5BD4: goto L5BD4;
    case 0x5BD9: goto L5BD9;
    case 0x5BDD: goto L5BDD;
    case 0x5BE2: goto L5BE2;
    case 0x5BE6: goto L5BE6;
    case 0x5BEB: goto L5BEB;
    case 0x5BEF: goto L5BEF;
    case 0x5BF4: goto L5BF4;
    case 0x5BF8: goto L5BF8;
    case 0x5BFD: goto L5BFD;
    case 0x5C01: goto L5C01;
    case 0x5C06: goto L5C06;
    case 0x5C0C: goto L5C0C;
    case 0x5C0E: goto L5C0E;
    case 0x5C12: goto L5C12;
    case 0x5C17: goto L5C17;
    case 0x5C1B: goto L5C1B;
    case 0x5C20: goto L5C20;
    case 0x5C24: goto L5C24;
    case 0x5C29: goto L5C29;
    case 0x5C2D: goto L5C2D;
    case 0x5C32: goto L5C32;
    case 0x5C34: goto L5C34;
    case 0x5C38: goto L5C38;
    case 0x5C3D: goto L5C3D;
    case 0x5C41: goto L5C41;
    case 0x5C46: goto L5C46;
    case 0x5C4A: goto L5C4A;
    case 0x5C4F: goto L5C4F;
    case 0x5C53: goto L5C53;
    case 0x5C58: goto L5C58;
    case 0x5C5C: goto L5C5C;
    case 0x5C5F: goto L5C5F;
    case 0x5C63: goto L5C63;
    case 0x5C67: goto L5C67;
    case 0x5C6D: goto L5C6D;
    case 0x5C6F: goto L5C6F;
    case 0x5C73: goto L5C73;
    case 0x5C77: goto L5C77;
    case 0x5C7B: goto L5C7B;
    case 0x5C7D: goto L5C7D;
    case 0x5C82: goto L5C82;
    case 0x5C86: goto L5C86;
    case 0x5C8A: goto L5C8A;
    case 0x5C8E: goto L5C8E;
    case 0x5C92: goto L5C92;
    case 0x5C94: goto L5C94;
    case 0x5C99: goto L5C99;
    case 0x5C9D: goto L5C9D;
    case 0x5CA1: goto L5CA1;
    case 0x5CA5: goto L5CA5;
    case 0x5CA9: goto L5CA9;
    case 0x5CAB: goto L5CAB;
    case 0x5CB0: goto L5CB0;
    case 0x5CB4: goto L5CB4;
    case 0x5CB8: goto L5CB8;
    case 0x5CBC: goto L5CBC;
    case 0x5CC0: goto L5CC0;
    case 0x5CC2: goto L5CC2;
    case 0x5CC7: goto L5CC7;
    case 0x5CCB: goto L5CCB;
    case 0x5CCE: goto L5CCE;
    case 0x5CD2: goto L5CD2;
    case 0x5CD8: goto L5CD8;
    case 0x5CDA: goto L5CDA;
    case 0x5CDB: goto L5CDB;
    case 0x5CDC: goto L5CDC;
    case 0x5CDD: goto L5CDD;
    case 0x5CDE: goto L5CDE;
    case 0x5CDF: goto L5CDF;
    case 0x5CE5: goto L5CE5;
    case 0x5CE7: goto L5CE7;
    case 0x5CF0: goto L5CF0;
    case 0x5CF6: goto L5CF6;
    case 0x5CF8: goto L5CF8;
    case 0x5D01: goto L5D01;
    case 0x5D04: goto L5D04;
    case 0x5D06: goto L5D06;
    case 0x5D0A: goto L5D0A;
    case 0x5D0C: goto L5D0C;
    case 0x5D0F: goto L5D0F;
    case 0x5D13: goto L5D13;
    case 0x5D17: goto L5D17;
    case 0x5D19: goto L5D19;
    case 0x5D1A: goto L5D1A;
    case 0x5D1D: goto L5D1D;
    case 0x5D20: goto L5D20;
    case 0x5D22: goto L5D22;
    case 0x5D26: goto L5D26;
    case 0x5D28: goto L5D28;
    case 0x5D29: goto L5D29;
    case 0x5D2A: goto L5D2A;
    case 0x5D2D: goto L5D2D;
    case 0x5D2E: goto L5D2E;
    case 0x5D34: goto L5D34;
    case 0x5D36: goto L5D36;
    case 0x5D37: goto L5D37;
    case 0x5D38: goto L5D38;
    case 0x5D3C: goto L5D3C;
    case 0x5D3E: goto L5D3E;
    case 0x5D43: goto L5D43;
    case 0x5D45: goto L5D45;
    case 0x5D48: goto L5D48;
    case 0x5D4A: goto L5D4A;
    case 0x5D4D: goto L5D4D;
    case 0x5D4F: goto L5D4F;
    case 0x5D51: goto L5D51;
    case 0x5D56: goto L5D56;
    case 0x5D58: goto L5D58;
    case 0x5D5B: goto L5D5B;
    case 0x5D5D: goto L5D5D;
    case 0x5D60: goto L5D60;
    case 0x5D62: goto L5D62;
    case 0x5D65: goto L5D65;
    case 0x5D66: goto L5D66;
    case 0x5D69: goto L5D69;
    case 0x5D6D: goto L5D6D;
    case 0x5D70: goto L5D70;
    case 0x5D73: goto L5D73;
    case 0x5D76: goto L5D76;
    case 0x5D77: goto L5D77;
    case 0x5D79: goto L5D79;
    case 0x5D7C: goto L5D7C;
    case 0x5D7F: goto L5D7F;
    case 0x5D82: goto L5D82;
    case 0x5D85: goto L5D85;
    case 0x5D88: goto L5D88;
    case 0x5D8C: goto L5D8C;
    case 0x5D8F: goto L5D8F;
    case 0x5D92: goto L5D92;
    case 0x5D95: goto L5D95;
    case 0x5D96: goto L5D96;
    case 0x5D99: goto L5D99;
    case 0x5D9B: goto L5D9B;
    case 0x5D9D: goto L5D9D;
    case 0x5DA0: goto L5DA0;
    case 0x5DA3: goto L5DA3;
    case 0x5DA6: goto L5DA6;
    case 0x5DAA: goto L5DAA;
    case 0x5DAE: goto L5DAE;
    case 0x5DB2: goto L5DB2;
    case 0x5DB6: goto L5DB6;
    case 0x5DBA: goto L5DBA;
    case 0x5DBE: goto L5DBE;
    case 0x5DC2: goto L5DC2;
    case 0x5DC6: goto L5DC6;
    case 0x5DC8: goto L5DC8;
    case 0x5DCA: goto L5DCA;
    case 0x5DCB: goto L5DCB;
    case 0x5DCF: goto L5DCF;
    case 0x5DD3: goto L5DD3;
    case 0x5DD7: goto L5DD7;
    case 0x5DDB: goto L5DDB;
    case 0x5DDF: goto L5DDF;
    case 0x5DE3: goto L5DE3;
    case 0x5DE7: goto L5DE7;
    case 0x5DEB: goto L5DEB;
    case 0x5DEF: goto L5DEF;
    case 0x5DF3: goto L5DF3;
    case 0x5DF5: goto L5DF5;
    case 0x5DF7: goto L5DF7;
    case 0x5DF8: goto L5DF8;
    case 0x5DFC: goto L5DFC;
    case 0x5E00: goto L5E00;
    case 0x5E04: goto L5E04;
    case 0x5E08: goto L5E08;
    case 0x5E0D: goto L5E0D;
    case 0x5E11: goto L5E11;
    case 0x5E15: goto L5E15;
    case 0x5E18: goto L5E18;
    case 0x5E1C: goto L5E1C;
    case 0x5E20: goto L5E20;
    case 0x5E22: goto L5E22;
    case 0x5E27: goto L5E27;
    case 0x5E2B: goto L5E2B;
    case 0x5E2F: goto L5E2F;
    case 0x5E33: goto L5E33;
    case 0x5E38: goto L5E38;
    case 0x5E3C: goto L5E3C;
    case 0x5E40: goto L5E40;
    case 0x5E43: goto L5E43;
    case 0x5E47: goto L5E47;
    case 0x5E4B: goto L5E4B;
    case 0x5E4D: goto L5E4D;
    case 0x5E52: goto L5E52;
    case 0x5E56: goto L5E56;
    case 0x5E5A: goto L5E5A;
    case 0x5E5E: goto L5E5E;
    case 0x5E62: goto L5E62;
    case 0x5E66: goto L5E66;
    case 0x5E6A: goto L5E6A;
    case 0x5E6E: goto L5E6E;
    case 0x5E77: goto L5E77;
    case 0x5E7B: goto L5E7B;
    case 0x5E7F: goto L5E7F;
    case 0x5E83: goto L5E83;
    case 0x5E87: goto L5E87;
    case 0x5E8A: goto L5E8A;
    case 0x5E8E: goto L5E8E;
    case 0x5E92: goto L5E92;
    case 0x5E96: goto L5E96;
    case 0x5E9A: goto L5E9A;
    case 0x5E9E: goto L5E9E;
    case 0x5EA0: goto L5EA0;
    case 0x5EA5: goto L5EA5;
    case 0x5EA9: goto L5EA9;
    case 0x5EAD: goto L5EAD;
    case 0x5EB1: goto L5EB1;
    case 0x5EB5: goto L5EB5;
    case 0x5EB9: goto L5EB9;
    case 0x5EBB: goto L5EBB;
    case 0x5EC0: goto L5EC0;
    case 0x5EC4: goto L5EC4;
    case 0x5EC8: goto L5EC8;
    case 0x5ECC: goto L5ECC;
    case 0x5ED0: goto L5ED0;
    case 0x5ED4: goto L5ED4;
    case 0x5ED6: goto L5ED6;
    case 0x5EDB: goto L5EDB;
    case 0x5EDF: goto L5EDF;
    case 0x5EE8: goto L5EE8;
    case 0x5EEC: goto L5EEC;
    case 0x5EF0: goto L5EF0;
    case 0x5EF4: goto L5EF4;
    case 0x5EF8: goto L5EF8;
    case 0x5EFC: goto L5EFC;
    case 0x5F00: goto L5F00;
    case 0x5F04: goto L5F04;
    case 0x5F07: goto L5F07;
    case 0x5F0B: goto L5F0B;
    case 0x5F0C: goto L5F0C;
    case 0x5F10: goto L5F10;
    case 0x5F14: goto L5F14;
    case 0x5F18: goto L5F18;
    case 0x5F1C: goto L5F1C;
    case 0x5F20: goto L5F20;
    case 0x5F22: goto L5F22;
    case 0x5F27: goto L5F27;
    case 0x5F2B: goto L5F2B;
    case 0x5F2F: goto L5F2F;
    case 0x5F33: goto L5F33;
    case 0x5F37: goto L5F37;
    case 0x5F3B: goto L5F3B;
    case 0x5F3D: goto L5F3D;
    case 0x5F42: goto L5F42;
    case 0x5F46: goto L5F46;
    case 0x5F4A: goto L5F4A;
    case 0x5F4E: goto L5F4E;
    case 0x5F52: goto L5F52;
    case 0x5F56: goto L5F56;
    case 0x5F58: goto L5F58;
    case 0x5F5D: goto L5F5D;
    case 0x5F61: goto L5F61;
    case 0x5F62: goto L5F62;
    case 0x5F65: goto L5F65;
    case 0x5F69: goto L5F69;
    case 0x5F6D: goto L5F6D;
    case 0x5F73: goto L5F73;
    case 0x5F77: goto L5F77;
    case 0x5F79: goto L5F79;
    case 0x5F7F: goto L5F7F;
    case 0x5F81: goto L5F81;
    case 0x5F87: goto L5F87;
    case 0x5F8B: goto L5F8B;
    case 0x5F8F: goto L5F8F;
    case 0x5F91: goto L5F91;
    case 0x5F95: goto L5F95;
    case 0x5F99: goto L5F99;
    case 0x5F9D: goto L5F9D;
    case 0x5FA2: goto L5FA2;
    case 0x5FA4: goto L5FA4;
    case 0x5FA8: goto L5FA8;
    case 0x5FAC: goto L5FAC;
    case 0x5FB1: goto L5FB1;
    case 0x5FB6: goto L5FB6;
    case 0x5FB8: goto L5FB8;
    case 0x5FBC: goto L5FBC;
    case 0x5FC2: goto L5FC2;
    case 0x5FC6: goto L5FC6;
    case 0x5FC8: goto L5FC8;
    case 0x5FCE: goto L5FCE;
    case 0x5FD0: goto L5FD0;
    case 0x5FD6: goto L5FD6;
    case 0x5FDA: goto L5FDA;
    case 0x5FDE: goto L5FDE;
    case 0x5FE0: goto L5FE0;
    case 0x5FE4: goto L5FE4;
    case 0x5FE7: goto L5FE7;
    case 0x5FED: goto L5FED;
    case 0x5FF1: goto L5FF1;
    case 0x5FF3: goto L5FF3;
    case 0x5FF9: goto L5FF9;
    case 0x5FFB: goto L5FFB;
    case 0x6001: goto L6001;
    case 0x6005: goto L6005;
    case 0x6009: goto L6009;
    case 0x600B: goto L600B;
    case 0x600F: goto L600F;
    case 0x6012: goto L6012;
    case 0x6016: goto L6016;
    case 0x601A: goto L601A;
    case 0x601C: goto L601C;
    case 0x6021: goto L6021;
    case 0x6025: goto L6025;
    case 0x6029: goto L6029;
    case 0x602D: goto L602D;
    case 0x6031: goto L6031;
    case 0x6035: goto L6035;
    case 0x6037: goto L6037;
    case 0x603C: goto L603C;
    case 0x6040: goto L6040;
    case 0x6044: goto L6044;
    case 0x6048: goto L6048;
    case 0x604C: goto L604C;
    case 0x604E: goto L604E;
    case 0x6052: goto L6052;
    case 0x6055: goto L6055;
    case 0x6059: goto L6059;
    case 0x605D: goto L605D;
    case 0x6061: goto L6061;
    case 0x6063: goto L6063;
    case 0x6067: goto L6067;
    case 0x606A: goto L606A;
    case 0x606E: goto L606E;
    case 0x6072: goto L6072;
    case 0x6076: goto L6076;
    case 0x6078: goto L6078;
    case 0x607D: goto L607D;
    case 0x6081: goto L6081;
    case 0x6085: goto L6085;
    case 0x6089: goto L6089;
    case 0x608D: goto L608D;
    case 0x608F: goto L608F;
    case 0x6093: goto L6093;
    case 0x6096: goto L6096;
    case 0x609A: goto L609A;
    case 0x609E: goto L609E;
    case 0x60A2: goto L60A2;
    case 0x60A4: goto L60A4;
    case 0x60A8: goto L60A8;
    case 0x60AB: goto L60AB;
    case 0x60AF: goto L60AF;
    case 0x60B3: goto L60B3;
    case 0x60B7: goto L60B7;
    case 0x60B9: goto L60B9;
    case 0x60BE: goto L60BE;
    case 0x60C2: goto L60C2;
    case 0x60C6: goto L60C6;
    case 0x60CA: goto L60CA;
    case 0x60CE: goto L60CE;
    case 0x60D4: goto L60D4;
    case 0x60D8: goto L60D8;
    case 0x60DA: goto L60DA;
    case 0x60E0: goto L60E0;
    case 0x60E2: goto L60E2;
    case 0x60E8: goto L60E8;
    case 0x60EC: goto L60EC;
    case 0x60F0: goto L60F0;
    case 0x60F2: goto L60F2;
    case 0x60F6: goto L60F6;
    case 0x60FA: goto L60FA;
    case 0x60FE: goto L60FE;
    case 0x6103: goto L6103;
    case 0x6105: goto L6105;
    case 0x6109: goto L6109;
    case 0x610D: goto L610D;
    case 0x6112: goto L6112;
    case 0x6117: goto L6117;
    case 0x6119: goto L6119;
    case 0x611D: goto L611D;
    case 0x6121: goto L6121;
    case 0x6127: goto L6127;
    case 0x612B: goto L612B;
    case 0x612D: goto L612D;
    case 0x6133: goto L6133;
    case 0x6135: goto L6135;
    case 0x613B: goto L613B;
    case 0x613F: goto L613F;
    case 0x6143: goto L6143;
    case 0x6145: goto L6145;
    case 0x6149: goto L6149;
    case 0x614C: goto L614C;
    case 0x6152: goto L6152;
    case 0x6156: goto L6156;
    case 0x6158: goto L6158;
    case 0x615E: goto L615E;
    case 0x6160: goto L6160;
    case 0x6166: goto L6166;
    case 0x616A: goto L616A;
    case 0x616E: goto L616E;
    case 0x6170: goto L6170;
    case 0x6174: goto L6174;
    case 0x6177: goto L6177;
    case 0x617B: goto L617B;
    case 0x617F: goto L617F;
    case 0x6181: goto L6181;
    case 0x6186: goto L6186;
    case 0x618A: goto L618A;
    case 0x618E: goto L618E;
    case 0x6192: goto L6192;
    case 0x6196: goto L6196;
    case 0x619A: goto L619A;
    case 0x619C: goto L619C;
    case 0x61A1: goto L61A1;
    case 0x61A5: goto L61A5;
    case 0x61A9: goto L61A9;
    case 0x61AD: goto L61AD;
    case 0x61B1: goto L61B1;
    case 0x61B3: goto L61B3;
    case 0x61B7: goto L61B7;
    case 0x61BA: goto L61BA;
    case 0x61BE: goto L61BE;
    case 0x61C2: goto L61C2;
    case 0x61C6: goto L61C6;
    case 0x61C8: goto L61C8;
    case 0x61CC: goto L61CC;
    case 0x61CF: goto L61CF;
    case 0x61D3: goto L61D3;
    case 0x61D7: goto L61D7;
    case 0x61DB: goto L61DB;
    case 0x61DD: goto L61DD;
    case 0x61E2: goto L61E2;
    case 0x61E6: goto L61E6;
    case 0x61EA: goto L61EA;
    case 0x61EE: goto L61EE;
    case 0x61F2: goto L61F2;
    case 0x61F4: goto L61F4;
    case 0x61F8: goto L61F8;
    case 0x61FB: goto L61FB;
    case 0x61FF: goto L61FF;
    case 0x6203: goto L6203;
    case 0x6207: goto L6207;
    case 0x6209: goto L6209;
    case 0x620D: goto L620D;
    case 0x6210: goto L6210;
    case 0x6214: goto L6214;
    case 0x6218: goto L6218;
    case 0x621C: goto L621C;
    case 0x621E: goto L621E;
    case 0x6223: goto L6223;
    case 0x6227: goto L6227;
    case 0x6228: goto L6228;
    case 0x622A: goto L622A;
    case 0x622C: goto L622C;
    case 0x622E: goto L622E;
    case 0x622F: goto L622F;
    case 0x6235: goto L6235;
    default: asm_bad_entry("TEXMAP.ASM", entry);
    }

    /* seg004_0849_54C0  (+54C0)
       seg004_0849_54C0: step the left edge to its next vertex. The left edge walks the vertices
       backwards (index - 1, wrapping): CF4C becomes the vertex at _cur_left_ind - 1 and CF4E the one
       before it. Loads the edge's start (x, l, u * w, v * w and w, or u, v with w = 1.0 when linear)
       and works out its steps per row by dividing each difference by the edge's height in rows
       (CF48). A zero-height edge gets zero steps. In: BP = _asm_texture_map's frame. Changes EAX,
       EBX, ECX, EDX, SI. */
L54C0: /* _seg004_0849_54C0 */
    /* 54C0  mov     ax,word ptr ds:[0CF52h] */
    AX = rw(pDS, 0xCF52);
L54C3:
    /* 54C3  dec     ax */
    AX = dec16(AX);
L54C4:
    /* 54C4  jns     short L54CA */
    if (!SF) goto L54CA;
L54C6:
    /* 54C6  add     ax,word ptr [bp+8] */
    AX = (uint16_t)(AX + rw(pSS, BP + 0x8));
L54C9:
    /* 54C9  nop */
    ;
L54CA: /* L54CA */
    /* 54CA  imul    ax,20h */
    AX = imul16x(AX, 0x20);
L54CD:
    /* 54CD  mov     bx,ax */
    BX = AX;
L54CF:
    /* 54CF  add     ax,word ptr [bp+6] */
    AX = (uint16_t)(AX + rw(pSS, BP + 0x6));
L54D2:
    /* 54D2  nop */
    ;
L54D3:
    /* 54D3  mov     word ptr ds:[0CF4Ch],ax */
    ww(pDS, 0xCF4C, AX);
L54D6:
    /* 54D6  sub     bx,20h */
    BX = sub16(BX, 0x20, 0);
L54D9:
    /* 54D9  jns     short L54E3 */
    if (!SF) goto L54E3;
L54DB:
    /* 54DB  mov     ax,word ptr [bp+8] */
    AX = rw(pSS, BP + 0x8);
L54DE:
    /* 54DE  imul    ax,20h */
    AX = imul16x(AX, 0x20);
L54E1:
    /* 54E1  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L54E3: /* L54E3 */
    /* 54E3  add     bx,word ptr [bp+6] */
    BX = (uint16_t)(BX + rw(pSS, BP + 0x6));
L54E6:
    /* 54E6  nop */
    ;
L54E7:
    /* 54E7  mov     word ptr ds:[0CF4Eh],bx */
    ww(pDS, 0xCF4E, BX);
L54EB:
    /* 54EB  mov     bx,word ptr ds:[0CF4Ch] */
    BX = rw(pDS, 0xCF4C);
L54EF:
    /* 54EF  mov     eax,dword ptr [bx+0Ch] */
    EAX = rd(pDS, BX + 0xC);
L54F3:
    /* 54F3  mov     dword ptr ds:[0CF5Ch],eax */
    wd(pDS, 0xCF5C, EAX);
L54F7:
    /* 54F7  cmp     word ptr [bp+0Ah],1 */
    sub16(rw(pSS, BP + 0xA), 0x1, 0);
L54FB:
    /* 54FB  nop */
    ;
L54FC:
    /* 54FC  ja      short L5509 */
    if (!CF && !ZF) goto L5509;
L54FE:
    /* 54FE  mov     dword ptr ds:[0CF74h],10000h */
    wd(pDS, 0xCF74, 0x10000);
L5507:
    /* 5507  jmp     short L5535 */
    goto L5535;
L5509: /* L5509 */
    /* 5509  mov     eax,100h */
    EAX = 0x100;
L550F:
    /* 550F  cmp     dword ptr [bx+8],eax */
    sub32(rd(pDS, BX + 0x8), EAX, 0);
L5513:
    /* 5513  ja      short L551D */
    if (!CF && !ZF) goto L551D;
L5515:
    /* 5515  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L551B:
    /* 551B  jmp     short L5531 */
    goto L5531;
L551D: /* L551D */
    /* 551D  mov     eax,800000h */
    EAX = 0x800000;
L5523:
    /* 5523  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5527:
    /* 5527  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L552B:
    /* 552B  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L552D:
    /* 552D  idiv    dword ptr [bx+8] */
    if (asm_idiv32(rd(pDS, BX + 0x8)) && (c = asm_divfault(0x065C, 0x552D, 4)) != 0) return c;
L5531: /* L5531 */
    /* 5531  mov     dword ptr ds:[0CF74h],eax */
    wd(pDS, 0xCF74, EAX);
L5535: /* L5535 */
    /* 5535  mov     eax,dword ptr [bx+1Ch] */
    EAX = rd(pDS, BX + 0x1C);
L5539:
    /* 5539  mov     dword ptr ds:[0CF7Ch],eax */
    wd(pDS, 0xCF7C, EAX);
L553D:
    /* 553D  mov     eax,dword ptr ds:[0CF74h] */
    EAX = rd(pDS, 0xCF74);
L5541:
    /* 5541  imul    dword ptr [bx+14h] */
    imul32(rd(pDS, BX + 0x14));
L5545:
    /* 5545  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L554A:
    /* 554A  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L554C:
    /* 554C  mov     dword ptr ds:[0CF60h],eax */
    wd(pDS, 0xCF60, EAX);
L5550:
    /* 5550  mov     eax,dword ptr [bx+18h] */
    EAX = rd(pDS, BX + 0x18);
L5554:
    /* 5554  imul    dword ptr ds:[0CF74h] */
    imul32(rd(pDS, 0xCF74));
L5559:
    /* 5559  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L555E:
    /* 555E  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L5560:
    /* 5560  mov     dword ptr ds:[0CF64h],eax */
    wd(pDS, 0xCF64, EAX);
L5564:
    /* 5564  mov     si,word ptr ds:[0CF4Eh] */
    SI = rw(pDS, 0xCF4E);
L5568:
    /* 5568  mov     eax,dword ptr [si+10h] */
    EAX = rd(pDS, SI + 0x10);
L556C:
    /* 556C  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L5570:
    /* 5570  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L5574:
    /* 5574  mov     ecx,dword ptr [bx+10h] */
    ECX = rd(pDS, BX + 0x10);
L5578:
    /* 5578  add     ecx,41h */
    ECX = (uint32_t)(ECX + 0x41);
L557C:
    /* 557C  sar     ecx,10h */
    ECX = (uint32_t)((int32_t)ECX >> 16);
L5580:
    /* 5580  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L5583:
    /* 5583  shl     eax,10h */
    EAX = shl32(EAX, 16);
L5587:
    /* 5587  mov     dword ptr ds:[0CF48h],eax */
    wd(pDS, 0xCF48, EAX);
L558B:
    /* 558B  jne     short L55A7 */
    if (!ZF) goto L55A7;
L558D:
    /* 558D  sub     eax,eax */
    EAX = sub32(EAX, EAX, 0);
L5590:
    /* 5590  mov     dword ptr ds:[0CF84h],eax */
    wd(pDS, 0xCF84, EAX);
L5594:
    /* 5594  mov     dword ptr ds:[0CF88h],eax */
    wd(pDS, 0xCF88, EAX);
L5598:
    /* 5598  mov     dword ptr ds:[0CF8Ch],eax */
    wd(pDS, 0xCF8C, EAX);
L559C:
    /* 559C  mov     dword ptr ds:[0CF9Ch],eax */
    wd(pDS, 0xCF9C, EAX);
L55A0:
    /* 55A0  mov     dword ptr ds:[0CFA4h],eax */
    wd(pDS, 0xCFA4, EAX);
L55A4:
    /* 55A4  jmp     L5711 */
    goto L5711;
L55A7: /* L55A7 */
    /* 55A7  mov     eax,dword ptr [si+1Ch] */
    EAX = rd(pDS, SI + 0x1C);
L55AB:
    /* 55AB  sub     eax,dword ptr [bx+1Ch] */
    EAX = (uint32_t)(EAX - rd(pDS, BX + 0x1C));
L55AF:
    /* 55AF  rol     eax,10h */
    EAX = rol32(EAX, 16);
L55B3:
    /* 55B3  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L55B7:
    /* 55B7  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L55B9:
    /* 55B9  idiv    dword ptr ds:[0CF48h] */
    if (asm_idiv32(rd(pDS, 0xCF48)) && (c = asm_divfault(0x065C, 0x55B9, 5)) != 0) return c;
L55BE:
    /* 55BE  mov     dword ptr ds:[0CFA4h],eax */
    wd(pDS, 0xCFA4, EAX);
L55C2:
    /* 55C2  mov     eax,dword ptr [si+0Ch] */
    EAX = rd(pDS, SI + 0xC);
L55C6:
    /* 55C6  sub     eax,dword ptr [bx+0Ch] */
    EAX = (uint32_t)(EAX - rd(pDS, BX + 0xC));
L55CA:
    /* 55CA  rol     eax,10h */
    EAX = rol32(EAX, 16);
L55CE:
    /* 55CE  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L55D2:
    /* 55D2  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L55D4:
    /* 55D4  idiv    dword ptr ds:[0CF48h] */
    if (asm_idiv32(rd(pDS, 0xCF48)) && (c = asm_divfault(0x065C, 0x55D4, 5)) != 0) return c;
L55D9:
    /* 55D9  mov     dword ptr ds:[0CF84h],eax */
    wd(pDS, 0xCF84, EAX);
L55DD:
    /* 55DD  cmp     word ptr [bp+0Ah],1 */
    sub16(rw(pSS, BP + 0xA), 0x1, 0);
L55E1:
    /* 55E1  nop */
    ;
L55E2:
    /* 55E2  ja      short L5626 */
    if (!CF && !ZF) goto L5626;
L55E4:
    /* 55E4  mov     eax,dword ptr [si+14h] */
    EAX = rd(pDS, SI + 0x14);
L55E8:
    /* 55E8  sub     eax,dword ptr [bx+14h] */
    EAX = (uint32_t)(EAX - rd(pDS, BX + 0x14));
L55EC:
    /* 55EC  rol     eax,10h */
    EAX = rol32(EAX, 16);
L55F0:
    /* 55F0  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L55F4:
    /* 55F4  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L55F6:
    /* 55F6  idiv    dword ptr ds:[0CF48h] */
    if (asm_idiv32(rd(pDS, 0xCF48)) && (c = asm_divfault(0x065C, 0x55F6, 5)) != 0) return c;
L55FB:
    /* 55FB  mov     dword ptr ds:[0CF88h],eax */
    wd(pDS, 0xCF88, EAX);
L55FF:
    /* 55FF  mov     eax,dword ptr [si+18h] */
    EAX = rd(pDS, SI + 0x18);
L5603:
    /* 5603  sub     eax,dword ptr [bx+18h] */
    EAX = (uint32_t)(EAX - rd(pDS, BX + 0x18));
L5607:
    /* 5607  rol     eax,10h */
    EAX = rol32(EAX, 16);
L560B:
    /* 560B  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L560F:
    /* 560F  sub     ax,ax */
    AX = sub16(AX, AX, 0);
L5611:
    /* 5611  idiv    dword ptr ds:[0CF48h] */
    if (asm_idiv32(rd(pDS, 0xCF48)) && (c = asm_divfault(0x065C, 0x5611, 5)) != 0) return c;
L5616:
    /* 5616  mov     dword ptr ds:[0CF8Ch],eax */
    wd(pDS, 0xCF8C, EAX);
L561A:
    /* 561A  mov     dword ptr ds:[0CF9Ch],0 */
    wd(pDS, 0xCF9C, 0x0);
L5623:
    /* 5623  jmp     L5711 */
    goto L5711;
L5626: /* L5626 */
    /* 5626  mov     eax,dword ptr [bx+14h] */
    EAX = rd(pDS, BX + 0x14);
L562A:
    /* 562A  rol     eax,10h */
    EAX = rol32(EAX, 16);
L562E:
    /* 562E  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5632:
    /* 5632  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5634:
    /* 5634  idiv    dword ptr [bx+8] */
    if (asm_idiv32(rd(pDS, BX + 0x8)) && (c = asm_divfault(0x065C, 0x5634, 4)) != 0) return c;
L5638:
    /* 5638  mov     ecx,eax */
    ECX = EAX;
L563B:
    /* 563B  mov     eax,dword ptr [si+14h] */
    EAX = rd(pDS, SI + 0x14);
L563F:
    /* 563F  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5643:
    /* 5643  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5647:
    /* 5647  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5649:
    /* 5649  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x5649, 4)) != 0) return c;
L564D:
    /* 564D  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L5650:
    /* 5650  shl     eax,7 */
    EAX = (uint32_t)(EAX << 7);
L5654:
    /* 5654  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5658:
    /* 5658  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L565C:
    /* 565C  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L565E:
    /* 565E  idiv    dword ptr ds:[0CF48h] */
    if (asm_idiv32(rd(pDS, 0xCF48)) && (c = asm_divfault(0x065C, 0x565E, 5)) != 0) return c;
L5663:
    /* 5663  mov     dword ptr ds:[0CF88h],eax */
    wd(pDS, 0xCF88, EAX);
L5667:
    /* 5667  mov     eax,dword ptr [bx+18h] */
    EAX = rd(pDS, BX + 0x18);
L566B:
    /* 566B  rol     eax,10h */
    EAX = rol32(EAX, 16);
L566F:
    /* 566F  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5673:
    /* 5673  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5675:
    /* 5675  idiv    dword ptr [bx+8] */
    if (asm_idiv32(rd(pDS, BX + 0x8)) && (c = asm_divfault(0x065C, 0x5675, 4)) != 0) return c;
L5679:
    /* 5679  mov     ecx,eax */
    ECX = EAX;
L567C:
    /* 567C  mov     eax,dword ptr [si+18h] */
    EAX = rd(pDS, SI + 0x18);
L5680:
    /* 5680  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5684:
    /* 5684  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5688:
    /* 5688  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L568A:
    /* 568A  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x568A, 4)) != 0) return c;
L568E:
    /* 568E  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L5691:
    /* 5691  shl     eax,7 */
    EAX = (uint32_t)(EAX << 7);
L5695:
    /* 5695  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5699:
    /* 5699  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L569D:
    /* 569D  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L569F:
    /* 569F  idiv    dword ptr ds:[0CF48h] */
    if (asm_idiv32(rd(pDS, 0xCF48)) && (c = asm_divfault(0x065C, 0x569F, 5)) != 0) return c;
L56A4:
    /* 56A4  mov     dword ptr ds:[0CF8Ch],eax */
    wd(pDS, 0xCF8C, EAX);
L56A8:
    /* 56A8  mov     eax,100h */
    EAX = 0x100;
L56AE:
    /* 56AE  cmp     dword ptr [bx+8],eax */
    sub32(rd(pDS, BX + 0x8), EAX, 0);
L56B2:
    /* 56B2  ja      short L56BC */
    if (!CF && !ZF) goto L56BC;
L56B4:
    /* 56B4  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L56BA:
    /* 56BA  jmp     short L56D0 */
    goto L56D0;
L56BC: /* L56BC */
    /* 56BC  mov     eax,800000h */
    EAX = 0x800000;
L56C2:
    /* 56C2  rol     eax,10h */
    EAX = rol32(EAX, 16);
L56C6:
    /* 56C6  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L56CA:
    /* 56CA  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L56CC:
    /* 56CC  idiv    dword ptr [bx+8] */
    if (asm_idiv32(rd(pDS, BX + 0x8)) && (c = asm_divfault(0x065C, 0x56CC, 4)) != 0) return c;
L56D0: /* L56D0 */
    /* 56D0  mov     ecx,eax */
    ECX = EAX;
L56D3:
    /* 56D3  mov     eax,100h */
    EAX = 0x100;
L56D9:
    /* 56D9  cmp     dword ptr [si+8],eax */
    sub32(rd(pDS, SI + 0x8), EAX, 0);
L56DD:
    /* 56DD  ja      short L56E7 */
    if (!CF && !ZF) goto L56E7;
L56DF:
    /* 56DF  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L56E5:
    /* 56E5  jmp     short L56FB */
    goto L56FB;
L56E7: /* L56E7 */
    /* 56E7  mov     eax,800000h */
    EAX = 0x800000;
L56ED:
    /* 56ED  rol     eax,10h */
    EAX = rol32(EAX, 16);
L56F1:
    /* 56F1  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L56F5:
    /* 56F5  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L56F7:
    /* 56F7  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x56F7, 4)) != 0) return c;
L56FB: /* L56FB */
    /* 56FB  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L56FE:
    /* 56FE  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5702:
    /* 5702  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5706:
    /* 5706  sub     ax,ax */
    AX = sub16(AX, AX, 0);
L5708:
    /* 5708  idiv    dword ptr ds:[0CF48h] */
    if (asm_idiv32(rd(pDS, 0xCF48)) && (c = asm_divfault(0x065C, 0x5708, 5)) != 0) return c;
L570D:
    /* 570D  mov     dword ptr ds:[0CF9Ch],eax */
    wd(pDS, 0xCF9C, EAX);
L5711: /* L5711 */
    /* 5711  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_5712  (+5712)
       seg004_0849_5712: as seg004_0849_54C0 for the right edge, which walks the vertices forwards
       (index + 1, wrapping), into CF68 .. CF80 and CF90 .. CFA8. */
L5712: /* _seg004_0849_5712 */
    /* 5712  mov     ax,word ptr ds:[0CF54h] */
    AX = rw(pDS, 0xCF54);
L5715:
    /* 5715  inc     ax */
    AX = (uint16_t)(AX + 1);
L5716:
    /* 5716  cmp     ax,word ptr [bp+8] */
    sub16(AX, rw(pSS, BP + 0x8), 0);
L5719:
    /* 5719  nop */
    ;
L571A:
    /* 571A  jb      short L5720 */
    if (CF) goto L5720;
L571C:
    /* 571C  sub     ax,word ptr [bp+8] */
    AX = (uint16_t)(AX - rw(pSS, BP + 0x8));
L571F:
    /* 571F  nop */
    ;
L5720: /* L5720 */
    /* 5720  mov     bx,ax */
    BX = AX;
L5722:
    /* 5722  imul    ax,20h */
    AX = imul16x(AX, 0x20);
L5725:
    /* 5725  add     ax,word ptr [bp+6] */
    AX = (uint16_t)(AX + rw(pSS, BP + 0x6));
L5728:
    /* 5728  nop */
    ;
L5729:
    /* 5729  mov     word ptr ds:[0CF4Ch],ax */
    ww(pDS, 0xCF4C, AX);
L572C:
    /* 572C  mov     ax,bx */
    AX = BX;
L572E:
    /* 572E  inc     ax */
    AX = (uint16_t)(AX + 1);
L572F:
    /* 572F  cmp     ax,word ptr [bp+8] */
    sub16(AX, rw(pSS, BP + 0x8), 0);
L5732:
    /* 5732  nop */
    ;
L5733:
    /* 5733  jb      short L5739 */
    if (CF) goto L5739;
L5735:
    /* 5735  sub     ax,word ptr [bp+8] */
    AX = (uint16_t)(AX - rw(pSS, BP + 0x8));
L5738:
    /* 5738  nop */
    ;
L5739: /* L5739 */
    /* 5739  imul    ax,20h */
    AX = imul16x(AX, 0x20);
L573C:
    /* 573C  add     ax,word ptr [bp+6] */
    AX = (uint16_t)(AX + rw(pSS, BP + 0x6));
L573F:
    /* 573F  nop */
    ;
L5740:
    /* 5740  mov     word ptr ds:[0CF4Eh],ax */
    ww(pDS, 0xCF4E, AX);
L5743:
    /* 5743  mov     bx,word ptr ds:[0CF4Ch] */
    BX = rw(pDS, 0xCF4C);
L5747:
    /* 5747  mov     eax,dword ptr [bx+0Ch] */
    EAX = rd(pDS, BX + 0xC);
L574B:
    /* 574B  mov     dword ptr ds:[0CF68h],eax */
    wd(pDS, 0xCF68, EAX);
L574F:
    /* 574F  cmp     word ptr [bp+0Ah],1 */
    sub16(rw(pSS, BP + 0xA), 0x1, 0);
L5753:
    /* 5753  nop */
    ;
L5754:
    /* 5754  ja      short L5761 */
    if (!CF && !ZF) goto L5761;
L5756:
    /* 5756  mov     dword ptr ds:[0CF78h],10000h */
    wd(pDS, 0xCF78, 0x10000);
L575F:
    /* 575F  jmp     short L578D */
    goto L578D;
L5761: /* L5761 */
    /* 5761  mov     eax,100h */
    EAX = 0x100;
L5767:
    /* 5767  cmp     dword ptr [bx+8],eax */
    sub32(rd(pDS, BX + 0x8), EAX, 0);
L576B:
    /* 576B  ja      short L5775 */
    if (!CF && !ZF) goto L5775;
L576D:
    /* 576D  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L5773:
    /* 5773  jmp     short L5789 */
    goto L5789;
L5775: /* L5775 */
    /* 5775  mov     eax,800000h */
    EAX = 0x800000;
L577B:
    /* 577B  rol     eax,10h */
    EAX = rol32(EAX, 16);
L577F:
    /* 577F  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5783:
    /* 5783  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5785:
    /* 5785  idiv    dword ptr [bx+8] */
    if (asm_idiv32(rd(pDS, BX + 0x8)) && (c = asm_divfault(0x065C, 0x5785, 4)) != 0) return c;
L5789: /* L5789 */
    /* 5789  mov     dword ptr ds:[0CF78h],eax */
    wd(pDS, 0xCF78, EAX);
L578D: /* L578D */
    /* 578D  mov     eax,dword ptr [bx+1Ch] */
    EAX = rd(pDS, BX + 0x1C);
L5791:
    /* 5791  mov     dword ptr ds:[0CF80h],eax */
    wd(pDS, 0xCF80, EAX);
L5795:
    /* 5795  mov     eax,dword ptr ds:[0CF78h] */
    EAX = rd(pDS, 0xCF78);
L5799:
    /* 5799  imul    dword ptr [bx+14h] */
    imul32(rd(pDS, BX + 0x14));
L579D:
    /* 579D  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L57A2:
    /* 57A2  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L57A4:
    /* 57A4  mov     dword ptr ds:[0CF6Ch],eax */
    wd(pDS, 0xCF6C, EAX);
L57A8:
    /* 57A8  mov     eax,dword ptr [bx+18h] */
    EAX = rd(pDS, BX + 0x18);
L57AC:
    /* 57AC  imul    dword ptr ds:[0CF78h] */
    imul32(rd(pDS, 0xCF78));
L57B1:
    /* 57B1  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L57B6:
    /* 57B6  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L57B8:
    /* 57B8  mov     dword ptr ds:[0CF70h],eax */
    wd(pDS, 0xCF70, EAX);
L57BC:
    /* 57BC  mov     si,word ptr ds:[0CF4Eh] */
    SI = rw(pDS, 0xCF4E);
L57C0:
    /* 57C0  mov     eax,dword ptr [si+10h] */
    EAX = rd(pDS, SI + 0x10);
L57C4:
    /* 57C4  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L57C8:
    /* 57C8  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L57CC:
    /* 57CC  mov     ecx,dword ptr [bx+10h] */
    ECX = rd(pDS, BX + 0x10);
L57D0:
    /* 57D0  add     ecx,41h */
    ECX = (uint32_t)(ECX + 0x41);
L57D4:
    /* 57D4  sar     ecx,10h */
    ECX = (uint32_t)((int32_t)ECX >> 16);
L57D8:
    /* 57D8  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L57DB:
    /* 57DB  shl     eax,10h */
    EAX = shl32(EAX, 16);
L57DF:
    /* 57DF  mov     dword ptr ds:[0CF48h],eax */
    wd(pDS, 0xCF48, EAX);
L57E3:
    /* 57E3  jne     short L57FF */
    if (!ZF) goto L57FF;
L57E5:
    /* 57E5  sub     eax,eax */
    EAX = sub32(EAX, EAX, 0);
L57E8:
    /* 57E8  mov     dword ptr ds:[0CF90h],eax */
    wd(pDS, 0xCF90, EAX);
L57EC:
    /* 57EC  mov     dword ptr ds:[0CF94h],eax */
    wd(pDS, 0xCF94, EAX);
L57F0:
    /* 57F0  mov     dword ptr ds:[0CF98h],eax */
    wd(pDS, 0xCF98, EAX);
L57F4:
    /* 57F4  mov     dword ptr ds:[0CFA0h],eax */
    wd(pDS, 0xCFA0, EAX);
L57F8:
    /* 57F8  mov     dword ptr ds:[0CFA8h],eax */
    wd(pDS, 0xCFA8, EAX);
L57FC:
    /* 57FC  jmp     L5965 */
    goto L5965;
L57FF: /* L57FF */
    /* 57FF  mov     eax,dword ptr [si+1Ch] */
    EAX = rd(pDS, SI + 0x1C);
L5803:
    /* 5803  sub     eax,dword ptr [bx+1Ch] */
    EAX = (uint32_t)(EAX - rd(pDS, BX + 0x1C));
L5807:
    /* 5807  rol     eax,10h */
    EAX = rol32(EAX, 16);
L580B:
    /* 580B  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L580F:
    /* 580F  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5811:
    /* 5811  idiv    dword ptr ds:[0CF48h] */
    if (asm_idiv32(rd(pDS, 0xCF48)) && (c = asm_divfault(0x065C, 0x5811, 5)) != 0) return c;
L5816:
    /* 5816  mov     dword ptr ds:[0CFA8h],eax */
    wd(pDS, 0xCFA8, EAX);
L581A:
    /* 581A  mov     eax,dword ptr [si+0Ch] */
    EAX = rd(pDS, SI + 0xC);
L581E:
    /* 581E  sub     eax,dword ptr [bx+0Ch] */
    EAX = (uint32_t)(EAX - rd(pDS, BX + 0xC));
L5822:
    /* 5822  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5826:
    /* 5826  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L582A:
    /* 582A  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L582C:
    /* 582C  idiv    dword ptr ds:[0CF48h] */
    if (asm_idiv32(rd(pDS, 0xCF48)) && (c = asm_divfault(0x065C, 0x582C, 5)) != 0) return c;
L5831:
    /* 5831  mov     dword ptr ds:[0CF90h],eax */
    wd(pDS, 0xCF90, EAX);
L5835:
    /* 5835  cmp     word ptr [bp+0Ah],1 */
    sub16(rw(pSS, BP + 0xA), 0x1, 0);
L5839:
    /* 5839  nop */
    ;
L583A:
    /* 583A  ja      short L587C */
    if (!CF && !ZF) goto L587C;
L583C:
    /* 583C  mov     eax,dword ptr [si+14h] */
    EAX = rd(pDS, SI + 0x14);
L5840:
    /* 5840  sub     eax,dword ptr [bx+14h] */
    EAX = (uint32_t)(EAX - rd(pDS, BX + 0x14));
L5844:
    /* 5844  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5848:
    /* 5848  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L584C:
    /* 584C  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L584E:
    /* 584E  idiv    dword ptr ds:[0CF48h] */
    if (asm_idiv32(rd(pDS, 0xCF48)) && (c = asm_divfault(0x065C, 0x584E, 5)) != 0) return c;
L5853:
    /* 5853  mov     dword ptr ds:[0CF94h],eax */
    wd(pDS, 0xCF94, EAX);
L5857:
    /* 5857  mov     eax,dword ptr [si+18h] */
    EAX = rd(pDS, SI + 0x18);
L585B:
    /* 585B  sub     eax,dword ptr [bx+18h] */
    EAX = (uint32_t)(EAX - rd(pDS, BX + 0x18));
L585F:
    /* 585F  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5863:
    /* 5863  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5867:
    /* 5867  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5869:
    /* 5869  idiv    dword ptr ds:[0CF48h] */
    if (asm_idiv32(rd(pDS, 0xCF48)) && (c = asm_divfault(0x065C, 0x5869, 5)) != 0) return c;
L586E:
    /* 586E  mov     dword ptr ds:[0CF98h],eax */
    wd(pDS, 0xCF98, EAX);
L5872:
    /* 5872  sub     eax,eax */
    EAX = sub32(EAX, EAX, 0);
L5875:
    /* 5875  mov     dword ptr ds:[0CFA0h],eax */
    wd(pDS, 0xCFA0, EAX);
L5879:
    /* 5879  jmp     L5965 */
    goto L5965;
L587C: /* L587C */
    /* 587C  mov     eax,dword ptr [bx+14h] */
    EAX = rd(pDS, BX + 0x14);
L5880:
    /* 5880  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5884:
    /* 5884  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5888:
    /* 5888  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L588A:
    /* 588A  idiv    dword ptr [bx+8] */
    if (asm_idiv32(rd(pDS, BX + 0x8)) && (c = asm_divfault(0x065C, 0x588A, 4)) != 0) return c;
L588E:
    /* 588E  mov     ecx,eax */
    ECX = EAX;
L5891:
    /* 5891  mov     eax,dword ptr [si+14h] */
    EAX = rd(pDS, SI + 0x14);
L5895:
    /* 5895  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5899:
    /* 5899  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L589D:
    /* 589D  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L589F:
    /* 589F  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x589F, 4)) != 0) return c;
L58A3:
    /* 58A3  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L58A6:
    /* 58A6  shl     eax,7 */
    EAX = (uint32_t)(EAX << 7);
L58AA:
    /* 58AA  rol     eax,10h */
    EAX = rol32(EAX, 16);
L58AE:
    /* 58AE  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L58B2:
    /* 58B2  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L58B4:
    /* 58B4  idiv    dword ptr ds:[0CF48h] */
    if (asm_idiv32(rd(pDS, 0xCF48)) && (c = asm_divfault(0x065C, 0x58B4, 5)) != 0) return c;
L58B9:
    /* 58B9  mov     dword ptr ds:[0CF94h],eax */
    wd(pDS, 0xCF94, EAX);
L58BD:
    /* 58BD  mov     eax,dword ptr [bx+18h] */
    EAX = rd(pDS, BX + 0x18);
L58C1:
    /* 58C1  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L58C3:
    /* 58C3  shld    edx,eax,17h */
    EDX = shld32(EDX, EAX, 23);
L58C8:
    /* 58C8  shl     eax,17h */
    EAX = (uint32_t)(EAX << 23);
L58CC:
    /* 58CC  idiv    dword ptr [bx+8] */
    if (asm_idiv32(rd(pDS, BX + 0x8)) && (c = asm_divfault(0x065C, 0x58CC, 4)) != 0) return c;
L58D0:
    /* 58D0  mov     ecx,eax */
    ECX = EAX;
L58D3:
    /* 58D3  mov     eax,dword ptr [si+18h] */
    EAX = rd(pDS, SI + 0x18);
L58D7:
    /* 58D7  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L58D9:
    /* 58D9  shld    edx,eax,17h */
    EDX = shld32(EDX, EAX, 23);
L58DE:
    /* 58DE  shl     eax,17h */
    EAX = (uint32_t)(EAX << 23);
L58E2:
    /* 58E2  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x58E2, 4)) != 0) return c;
L58E6:
    /* 58E6  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L58E9:
    /* 58E9  rol     eax,10h */
    EAX = rol32(EAX, 16);
L58ED:
    /* 58ED  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L58F1:
    /* 58F1  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L58F3:
    /* 58F3  idiv    dword ptr ds:[0CF48h] */
    if (asm_idiv32(rd(pDS, 0xCF48)) && (c = asm_divfault(0x065C, 0x58F3, 5)) != 0) return c;
L58F8:
    /* 58F8  mov     dword ptr ds:[0CF98h],eax */
    wd(pDS, 0xCF98, EAX);
L58FC:
    /* 58FC  mov     eax,100h */
    EAX = 0x100;
L5902:
    /* 5902  cmp     dword ptr [bx+8],eax */
    sub32(rd(pDS, BX + 0x8), EAX, 0);
L5906:
    /* 5906  ja      short L5910 */
    if (!CF && !ZF) goto L5910;
L5908:
    /* 5908  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L590E:
    /* 590E  jmp     short L5924 */
    goto L5924;
L5910: /* L5910 */
    /* 5910  mov     eax,800000h */
    EAX = 0x800000;
L5916:
    /* 5916  rol     eax,10h */
    EAX = rol32(EAX, 16);
L591A:
    /* 591A  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L591E:
    /* 591E  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5920:
    /* 5920  idiv    dword ptr [bx+8] */
    if (asm_idiv32(rd(pDS, BX + 0x8)) && (c = asm_divfault(0x065C, 0x5920, 4)) != 0) return c;
L5924: /* L5924 */
    /* 5924  mov     ecx,eax */
    ECX = EAX;
L5927:
    /* 5927  mov     eax,100h */
    EAX = 0x100;
L592D:
    /* 592D  cmp     dword ptr [si+8],eax */
    sub32(rd(pDS, SI + 0x8), EAX, 0);
L5931:
    /* 5931  ja      short L593B */
    if (!CF && !ZF) goto L593B;
L5933:
    /* 5933  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L5939:
    /* 5939  jmp     short L594F */
    goto L594F;
L593B: /* L593B */
    /* 593B  mov     eax,800000h */
    EAX = 0x800000;
L5941:
    /* 5941  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5945:
    /* 5945  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5949:
    /* 5949  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L594B:
    /* 594B  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x594B, 4)) != 0) return c;
L594F: /* L594F */
    /* 594F  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L5952:
    /* 5952  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5956:
    /* 5956  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L595A:
    /* 595A  sub     ax,ax */
    AX = sub16(AX, AX, 0);
L595C:
    /* 595C  idiv    dword ptr ds:[0CF48h] */
    if (asm_idiv32(rd(pDS, 0xCF48)) && (c = asm_divfault(0x065C, 0x595C, 5)) != 0) return c;
L5961:
    /* 5961  mov     dword ptr ds:[0CFA0h],eax */
    wd(pDS, 0xCFA0, EAX);
L5965: /* L5965 */
    /* 5965  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_5966  (+5966)
       compute_min_max_y: find the polygon's first and last rows. Sets CFCC and CFD0 to the least and
       greatest sy (16.16) and both edge indices to the vertex with the least sy; if that vertex
       shares its rounded row with the next or previous vertex, that one becomes the start of the
       right or left edge instead, so a flat first edge starts both edges at its two ends. The top
       and bottom rows are clamped to 0 .. 2 * scrh - 1 into CF56 and CF58 (scrh, DS:2470, is half
       the view height). In: BP = _asm_texture_map's frame. */
L5966: /* _compute_min_max_y */
    /* 5966  mov     dword ptr ds:[0CFCCh],7FFFFFFFh */
    wd(pDS, 0xCFCC, 0x7FFFFFFF);
L596F:
    /* 596F  mov     dword ptr ds:[0CFD0h],80000001h */
    wd(pDS, 0xCFD0, 0x80000001);
L5978:
    /* 5978  mov     bx,word ptr [bp+6] */
    BX = rw(pSS, BP + 0x6);
L597B:
    /* 597B  nop */
    ;
L597C:
    /* 597C  sub     dx,dx */
    DX = (uint16_t)(DX - DX);
L597E: /* L597E */
    /* 597E  mov     eax,dword ptr [bx+10h] */
    EAX = rd(pDS, BX + 0x10);
L5982:
    /* 5982  cmp     eax,dword ptr ds:[0CFCCh] */
    sub32(EAX, rd(pDS, 0xCFCC), 0);
L5987:
    /* 5987  jge     short L5991 */
    if (SF == OF) goto L5991;
L5989:
    /* 5989  mov     dword ptr ds:[0CFCCh],eax */
    wd(pDS, 0xCFCC, EAX);
L598D:
    /* 598D  mov     word ptr ds:[0CF52h],dx */
    ww(pDS, 0xCF52, DX);
L5991: /* L5991 */
    /* 5991  cmp     eax,dword ptr ds:[0CFD0h] */
    sub32(EAX, rd(pDS, 0xCFD0), 0);
L5996:
    /* 5996  jle     short L599C */
    if (ZF || SF != OF) goto L599C;
L5998:
    /* 5998  mov     dword ptr ds:[0CFD0h],eax */
    wd(pDS, 0xCFD0, EAX);
L599C: /* L599C */
    /* 599C  inc     dx */
    DX = (uint16_t)(DX + 1);
L599D:
    /* 599D  add     bx,20h */
    BX = (uint16_t)(BX + 0x20);
L59A0:
    /* 59A0  cmp     dx,word ptr [bp+8] */
    sub16(DX, rw(pSS, BP + 0x8), 0);
L59A3:
    /* 59A3  nop */
    ;
L59A4:
    /* 59A4  jb      L597E */
    if (CF) goto L597E;
L59A6:
    /* 59A6  mov     ax,word ptr ds:[0CF52h] */
    AX = rw(pDS, 0xCF52);
L59A9:
    /* 59A9  mov     word ptr ds:[0CF54h],ax */
    ww(pDS, 0xCF54, AX);
L59AC:
    /* 59AC  mov     bx,20h */
    BX = 0x20;
L59AF:
    /* 59AF  imul    bx,word ptr ds:[0CF52h] */
    BX = imul16x(BX, rw(pDS, 0xCF52));
L59B4:
    /* 59B4  add     bx,word ptr [bp+6] */
    BX = (uint16_t)(BX + rw(pSS, BP + 0x6));
L59B7:
    /* 59B7  nop */
    ;
L59B8:
    /* 59B8  mov     edx,dword ptr [bx+10h] */
    EDX = rd(pDS, BX + 0x10);
L59BC:
    /* 59BC  add     edx,41h */
    EDX = (uint32_t)(EDX + 0x41);
L59C0:
    /* 59C0  sar     edx,10h */
    EDX = (uint32_t)((int32_t)EDX >> 16);
L59C4:
    /* 59C4  mov     bx,word ptr ds:[0CF52h] */
    BX = rw(pDS, 0xCF52);
L59C8:
    /* 59C8  inc     bx */
    BX = (uint16_t)(BX + 1);
L59C9:
    /* 59C9  cmp     bx,word ptr [bp+8] */
    sub16(BX, rw(pSS, BP + 0x8), 0);
L59CC:
    /* 59CC  nop */
    ;
L59CD:
    /* 59CD  jb      short L59D1 */
    if (CF) goto L59D1;
L59CF:
    /* 59CF  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L59D1: /* L59D1 */
    /* 59D1  mov     cx,bx */
    CX = BX;
L59D3:
    /* 59D3  imul    bx,20h */
    BX = imul16x(BX, 0x20);
L59D6:
    /* 59D6  add     bx,word ptr [bp+6] */
    BX = (uint16_t)(BX + rw(pSS, BP + 0x6));
L59D9:
    /* 59D9  nop */
    ;
L59DA:
    /* 59DA  mov     eax,dword ptr [bx+10h] */
    EAX = rd(pDS, BX + 0x10);
L59DE:
    /* 59DE  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L59E2:
    /* 59E2  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L59E6:
    /* 59E6  cmp     ax,dx */
    sub16(AX, DX, 0);
L59E8:
    /* 59E8  jne     short L59EE */
    if (!ZF) goto L59EE;
L59EA:
    /* 59EA  mov     word ptr ds:[0CF54h],cx */
    ww(pDS, 0xCF54, CX);
L59EE: /* L59EE */
    /* 59EE  mov     bx,word ptr ds:[0CF52h] */
    BX = rw(pDS, 0xCF52);
L59F2:
    /* 59F2  dec     bx */
    BX = dec16(BX);
L59F3:
    /* 59F3  jns     short L59F9 */
    if (!SF) goto L59F9;
L59F5:
    /* 59F5  add     bx,word ptr [bp+8] */
    BX = (uint16_t)(BX + rw(pSS, BP + 0x8));
L59F8:
    /* 59F8  nop */
    ;
L59F9: /* L59F9 */
    /* 59F9  imul    bx,20h */
    BX = imul16x(BX, 0x20);
L59FC:
    /* 59FC  add     bx,word ptr [bp+6] */
    BX = (uint16_t)(BX + rw(pSS, BP + 0x6));
L59FF:
    /* 59FF  nop */
    ;
L5A00:
    /* 5A00  mov     eax,dword ptr [bx+10h] */
    EAX = rd(pDS, BX + 0x10);
L5A04:
    /* 5A04  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L5A08:
    /* 5A08  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L5A0C:
    /* 5A0C  cmp     ax,dx */
    sub16(AX, DX, 0);
L5A0E:
    /* 5A0E  jne     short L5A23 */
    if (!ZF) goto L5A23;
L5A10:
    /* 5A10  mov     ax,word ptr ds:[0CF52h] */
    AX = rw(pDS, 0xCF52);
L5A13:
    /* 5A13  mov     word ptr ds:[0CF54h],ax */
    ww(pDS, 0xCF54, AX);
L5A16:
    /* 5A16  dec     word ptr ds:[0CF52h] */
    ww(pDS, 0xCF52, dec16(rw(pDS, 0xCF52)));
L5A1A:
    /* 5A1A  jns     short L5A23 */
    if (!SF) goto L5A23;
L5A1C:
    /* 5A1C  mov     ax,word ptr [bp+8] */
    AX = rw(pSS, BP + 0x8);
L5A1F:
    /* 5A1F  add     word ptr ds:[0CF52h],ax */
    ww(pDS, 0xCF52, (uint16_t)(rw(pDS, 0xCF52) + AX));
L5A23: /* L5A23 */
    /* 5A23  mov     bx,word ptr ds:[2470h] */
    BX = rw(pDS, 0x2470);
L5A27:
    /* 5A27  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L5A29:
    /* 5A29  mov     eax,dword ptr ds:[0CFCCh] */
    EAX = rd(pDS, 0xCFCC);
L5A2D:
    /* 5A2D  shr     eax,10h */
    EAX = (uint32_t)(EAX >> 16);
L5A31:
    /* 5A31  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L5A33:
    /* 5A33  jns     short L5A37 */
    if (!SF) goto L5A37;
L5A35:
    /* 5A35  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5A37: /* L5A37 */
    /* 5A37  cmp     ax,bx */
    sub16(AX, BX, 0);
L5A39:
    /* 5A39  jb      short L5A3E */
    if (CF) goto L5A3E;
L5A3B:
    /* 5A3B  lea     ax,[bx-1] */
    AX = (uint16_t)(BX + 0xFFFF);
L5A3E: /* L5A3E */
    /* 5A3E  mov     word ptr ds:[0CF56h],ax */
    ww(pDS, 0xCF56, AX);
L5A41:
    /* 5A41  mov     eax,dword ptr ds:[0CFD0h] */
    EAX = rd(pDS, 0xCFD0);
L5A45:
    /* 5A45  shr     eax,10h */
    EAX = (uint32_t)(EAX >> 16);
L5A49:
    /* 5A49  cmp     ax,bx */
    sub16(AX, BX, 0);
L5A4B:
    /* 5A4B  jb      short L5A50 */
    if (CF) goto L5A50;
L5A4D:
    /* 5A4D  lea     ax,[bx-1] */
    AX = (uint16_t)(BX + 0xFFFF);
L5A50: /* L5A50 */
    /* 5A50  mov     word ptr ds:[0CF58h],ax */
    ww(pDS, 0xCF58, AX);
L5A53:
    /* 5A53  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_5A54  (+5A54)
       _asm_texture_map(descriptor, vertices, count, mapping, shaded): draw the polygon into the
       frame buffer (ES = seg_370D:0958, GS = the texture's segment). Chooses the span drawer, finds
       the first row, sets up both edges (seg004_0849_5D66), then for each row: advance an edge to
       its next vertex when the row passes it, for floors divide u * w and v * w by w at both ends,
       clamp the span to the screen (seg004_0849_5CDF), call the span drawer and step both edges. The
       last row is drawn after the loop. Keeps SI, DI, ES, GS, BP; clears CF44 on the way out. */
L5A54: /* __asm_texture_map */
    /* 5A54  push    bp */
    push16(BP);
L5A55:
    /* 5A55  mov     bp,sp */
    BP = SP;
L5A57:
    /* 5A57  push    si */
    push16(SI);
L5A58:
    /* 5A58  push    di */
    push16(DI);
L5A59:
    /* 5A59  push    es */
    push16(asm_es);
L5A5A:
    /* 5A5A  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L5A5D:
    /* 5A5D  mov     es,ax */
    SET_ES(AX);
L5A5F:
    /* 5A5F  mov     es,word ptr es:[958h] */
    SET_ES(rw(pES, 0x958));
L5A64:
    /* 5A64  push    gs */
    push16(asm_gs);
L5A66:
    /* 5A66  call    _seg004_0849_5D2E */
    if ((c = asm_call(ASM_JMP(0x065C, 0x5D2E), 0x5A69)) != 0) return c;
L5A69:
    /* 5A69  mov     bx,word ptr [bp+4] */
    BX = rw(pSS, BP + 0x4);
L5A6C:
    /* 5A6C  mov     ax,word ptr [bx+4] */
    AX = rw(pDS, BX + 0x4);
L5A6F:
    /* 5A6F  mov     word ptr ds:[0CF3Ch],ax */
    ww(pDS, 0xCF3C, AX);
L5A72:
    /* 5A72  mov     gs,word ptr [bx+6] */
    SET_GS(rw(pDS, BX + 0x6));
L5A75:
    /* 5A75  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L5A77:
    /* 5A77  mov     word ptr ds:[0CF3Eh],ax */
    ww(pDS, 0xCF3E, AX);
L5A7A:
    /* 5A7A  mov     word ptr ds:[0CF36h],ax */
    ww(pDS, 0xCF36, AX);
L5A7D:
    /* 5A7D  mov     word ptr ds:[0CF34h],0 */
    ww(pDS, 0xCF34, 0x0);
L5A83:
    /* 5A83  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L5A86:
    /* 5A86  mov     word ptr ds:[0CF40h],ax */
    ww(pDS, 0xCF40, AX);
L5A89:
    /* 5A89  mov     word ptr ds:[0CF3Ah],ax */
    ww(pDS, 0xCF3A, AX);
L5A8C:
    /* 5A8C  mov     word ptr ds:[0CF38h],0 */
    ww(pDS, 0xCF38, 0x0);
L5A92:
    /* 5A92  mov     al,byte ptr [bx+8] */
    AL = rb(pDS, BX + 0x8);
L5A95:
    /* 5A95  mov     byte ptr ds:[0CF46h],al */
    wb(pDS, 0xCF46, AL);
L5A98:
    /* 5A98  call    _compute_min_max_y */
    if ((c = asm_call(ASM_JMP(0x065C, 0x5966), 0x5A9B)) != 0) return c;
L5A9B:
    /* 5A9B  call    _seg004_0849_5D66 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x5D66), 0x5A9E)) != 0) return c;
L5A9E:
    /* 5A9E  mov     ax,word ptr ds:[0CF56h] */
    AX = rw(pDS, 0xCF56);
L5AA1:
    /* 5AA1  mov     word ptr ds:[0CF5Ah],ax */
    ww(pDS, 0xCF5A, AX);
L5AA4:
    /* 5AA4  mov     eax,dword ptr ds:[0CF60h] */
    EAX = rd(pDS, 0xCF60);
L5AA8:
    /* 5AA8  mov     dword ptr ds:[0CFACh],eax */
    wd(pDS, 0xCFAC, EAX);
L5AAC:
    /* 5AAC  mov     eax,dword ptr ds:[0CF64h] */
    EAX = rd(pDS, 0xCF64);
L5AB0:
    /* 5AB0  mov     dword ptr ds:[0CFB4h],eax */
    wd(pDS, 0xCFB4, EAX);
L5AB4:
    /* 5AB4  mov     eax,dword ptr ds:[0CF6Ch] */
    EAX = rd(pDS, 0xCF6C);
L5AB8:
    /* 5AB8  mov     dword ptr ds:[0CFB0h],eax */
    wd(pDS, 0xCFB0, EAX);
L5ABC:
    /* 5ABC  mov     eax,dword ptr ds:[0CF70h] */
    EAX = rd(pDS, 0xCF70);
L5AC0:
    /* 5AC0  mov     dword ptr ds:[0CFB8h],eax */
    wd(pDS, 0xCFB8, EAX);
L5AC4: /* L5AC4 */
    /* 5AC4  mov     si,word ptr ds:[0CF52h] */
    SI = rw(pDS, 0xCF52);
L5AC8:
    /* 5AC8  dec     si */
    SI = dec16(SI);
L5AC9:
    /* 5AC9  jns     short L5ACE */
    if (!SF) goto L5ACE;
L5ACB:
    /* 5ACB  add     si,word ptr [bp+8] */
    SI = (uint16_t)(SI + rw(pSS, BP + 0x8));
L5ACE: /* L5ACE */
    /* 5ACE  imul    si,20h */
    SI = imul16x(SI, 0x20);
L5AD1:
    /* 5AD1  mov     bx,word ptr [bp+6] */
    BX = rw(pSS, BP + 0x6);
L5AD4:
    /* 5AD4  mov     eax,dword ptr [bx+si+10h] */
    EAX = rd(pDS, BX + SI + 0x10);
L5AD8:
    /* 5AD8  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L5ADC:
    /* 5ADC  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L5AE0:
    /* 5AE0  cmp     ax,word ptr ds:[0CF5Ah] */
    sub16(AX, rw(pDS, 0xCF5A), 0);
L5AE4:
    /* 5AE4  jg      short L5B06 */
    if (!ZF && SF == OF) goto L5B06;
L5AE6:
    /* 5AE6  call    _seg004_0849_54C0 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x54C0), 0x5AE9)) != 0) return c;
L5AE9:
    /* 5AE9  dec     word ptr ds:[0CF52h] */
    ww(pDS, 0xCF52, dec16(rw(pDS, 0xCF52)));
L5AED:
    /* 5AED  jns     short L5AF6 */
    if (!SF) goto L5AF6;
L5AEF:
    /* 5AEF  mov     ax,word ptr [bp+8] */
    AX = rw(pSS, BP + 0x8);
L5AF2:
    /* 5AF2  add     word ptr ds:[0CF52h],ax */
    ww(pDS, 0xCF52, (uint16_t)(rw(pDS, 0xCF52) + AX));
L5AF6: /* L5AF6 */
    /* 5AF6  mov     eax,dword ptr ds:[0CF60h] */
    EAX = rd(pDS, 0xCF60);
L5AFA:
    /* 5AFA  mov     dword ptr ds:[0CFACh],eax */
    wd(pDS, 0xCFAC, EAX);
L5AFE:
    /* 5AFE  mov     eax,dword ptr ds:[0CF64h] */
    EAX = rd(pDS, 0xCF64);
L5B02:
    /* 5B02  mov     dword ptr ds:[0CFB4h],eax */
    wd(pDS, 0xCFB4, EAX);
L5B06: /* L5B06 */
    /* 5B06  mov     bx,word ptr ds:[0CF54h] */
    BX = rw(pDS, 0xCF54);
L5B0A:
    /* 5B0A  inc     bx */
    BX = (uint16_t)(BX + 1);
L5B0B:
    /* 5B0B  cmp     bx,word ptr [bp+8] */
    sub16(BX, rw(pSS, BP + 0x8), 0);
L5B0E:
    /* 5B0E  jb      short L5B12 */
    if (CF) goto L5B12;
L5B10:
    /* 5B10  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L5B12: /* L5B12 */
    /* 5B12  imul    bx,20h */
    BX = imul16x(BX, 0x20);
L5B15:
    /* 5B15  add     bx,word ptr [bp+6] */
    BX = (uint16_t)(BX + rw(pSS, BP + 0x6));
L5B18:
    /* 5B18  mov     eax,dword ptr [bx+10h] */
    EAX = rd(pDS, BX + 0x10);
L5B1C:
    /* 5B1C  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L5B20:
    /* 5B20  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L5B24:
    /* 5B24  cmp     ax,word ptr ds:[0CF5Ah] */
    sub16(AX, rw(pDS, 0xCF5A), 0);
L5B28:
    /* 5B28  jg      short L5B4F */
    if (!ZF && SF == OF) goto L5B4F;
L5B2A:
    /* 5B2A  call    _seg004_0849_5712 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x5712), 0x5B2D)) != 0) return c;
L5B2D:
    /* 5B2D  inc     word ptr ds:[0CF54h] */
    ww(pDS, 0xCF54, (uint16_t)(rw(pDS, 0xCF54) + 1));
L5B31:
    /* 5B31  mov     ax,word ptr ds:[0CF54h] */
    AX = rw(pDS, 0xCF54);
L5B34:
    /* 5B34  cmp     ax,word ptr [bp+8] */
    sub16(AX, rw(pSS, BP + 0x8), 0);
L5B37:
    /* 5B37  jb      short L5B3F */
    if (CF) goto L5B3F;
L5B39:
    /* 5B39  mov     word ptr ds:[0CF54h],0 */
    ww(pDS, 0xCF54, 0x0);
L5B3F: /* L5B3F */
    /* 5B3F  mov     eax,dword ptr ds:[0CF6Ch] */
    EAX = rd(pDS, 0xCF6C);
L5B43:
    /* 5B43  mov     dword ptr ds:[0CFB0h],eax */
    wd(pDS, 0xCFB0, EAX);
L5B47:
    /* 5B47  mov     eax,dword ptr ds:[0CF70h] */
    EAX = rd(pDS, 0xCF70);
L5B4B:
    /* 5B4B  mov     dword ptr ds:[0CFB8h],eax */
    wd(pDS, 0xCFB8, EAX);
L5B4F: /* L5B4F */
    /* 5B4F  mov     al,byte ptr ds:[0CF31h] */
    AL = rb(pDS, 0xCF31);
L5B52:
    /* 5B52  mov     byte ptr ds:[0CF30h],al */
    wb(pDS, 0xCF30, AL);
L5B55:
    /* 5B55  test    word ptr ds:[0CCF0h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0xCCF0) & 0xFFFF));
L5B5B:
    /* 5B5B  je      short L5BBE */
    if (ZF) goto L5BBE;
L5B5D:
    /* 5B5D  mov     byte ptr ds:[0CF30h],5 */
    wb(pDS, 0xCF30, 0x5);
L5B62:
    /* 5B62  mov     eax,dword ptr ds:[0CFB0h] */
    EAX = rd(pDS, 0xCFB0);
L5B66:
    /* 5B66  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5B6A:
    /* 5B6A  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5B6E:
    /* 5B6E  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5B70:
    /* 5B70  idiv    dword ptr ds:[0CF78h] */
    if (asm_idiv32(rd(pDS, 0xCF78)) && (c = asm_divfault(0x065C, 0x5B70, 5)) != 0) return c;
L5B75:
    /* 5B75  mov     dword ptr ds:[0CF6Ch],eax */
    wd(pDS, 0xCF6C, EAX);
L5B79:
    /* 5B79  mov     eax,dword ptr ds:[0CFACh] */
    EAX = rd(pDS, 0xCFAC);
L5B7D:
    /* 5B7D  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5B81:
    /* 5B81  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5B85:
    /* 5B85  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5B87:
    /* 5B87  idiv    dword ptr ds:[0CF74h] */
    if (asm_idiv32(rd(pDS, 0xCF74)) && (c = asm_divfault(0x065C, 0x5B87, 5)) != 0) return c;
L5B8C:
    /* 5B8C  mov     dword ptr ds:[0CF60h],eax */
    wd(pDS, 0xCF60, EAX);
L5B90:
    /* 5B90  mov     eax,dword ptr ds:[0CFB8h] */
    EAX = rd(pDS, 0xCFB8);
L5B94:
    /* 5B94  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5B98:
    /* 5B98  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5B9C:
    /* 5B9C  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5B9E:
    /* 5B9E  idiv    dword ptr ds:[0CF78h] */
    if (asm_idiv32(rd(pDS, 0xCF78)) && (c = asm_divfault(0x065C, 0x5B9E, 5)) != 0) return c;
L5BA3:
    /* 5BA3  mov     dword ptr ds:[0CF70h],eax */
    wd(pDS, 0xCF70, EAX);
L5BA7:
    /* 5BA7  mov     eax,dword ptr ds:[0CFB4h] */
    EAX = rd(pDS, 0xCFB4);
L5BAB:
    /* 5BAB  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5BAF:
    /* 5BAF  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5BB3:
    /* 5BB3  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5BB5:
    /* 5BB5  idiv    dword ptr ds:[0CF74h] */
    if (asm_idiv32(rd(pDS, 0xCF74)) && (c = asm_divfault(0x065C, 0x5BB5, 5)) != 0) return c;
L5BBA:
    /* 5BBA  mov     dword ptr ds:[0CF64h],eax */
    wd(pDS, 0xCF64, EAX);
L5BBE: /* L5BBE */
    /* 5BBE  cmp     word ptr ds:[0CF44h],1 */
    sub16(rw(pDS, 0xCF44), 0x1, 0);
L5BC3:
    /* 5BC3  je      L5CD2 */
    if (ZF) goto L5CD2;
L5BC7:
    /* 5BC7  call    _seg004_0849_5CDF */
    if ((c = asm_call(ASM_JMP(0x065C, 0x5CDF), 0x5BCA)) != 0) return c;
L5BCA:
    /* 5BCA  push    bp */
    push16(BP);
L5BCB:
    /* 5BCB  call    word ptr ds:[0CF42h] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, 0xCF42)), 0x5BCF)) != 0) return c;
L5BCF:
    /* 5BCF  pop     bp */
    BP = pop16();
L5BD0:
    /* 5BD0  mov     eax,dword ptr ds:[0CF84h] */
    EAX = rd(pDS, 0xCF84);
L5BD4:
    /* 5BD4  add     dword ptr ds:[0CF5Ch],eax */
    wd(pDS, 0xCF5C, (uint32_t)(rd(pDS, 0xCF5C) + EAX));
L5BD9:
    /* 5BD9  mov     eax,dword ptr ds:[0CF9Ch] */
    EAX = rd(pDS, 0xCF9C);
L5BDD:
    /* 5BDD  add     dword ptr ds:[0CF74h],eax */
    wd(pDS, 0xCF74, (uint32_t)(rd(pDS, 0xCF74) + EAX));
L5BE2:
    /* 5BE2  mov     eax,dword ptr ds:[0CFA4h] */
    EAX = rd(pDS, 0xCFA4);
L5BE6:
    /* 5BE6  add     dword ptr ds:[0CF7Ch],eax */
    wd(pDS, 0xCF7C, (uint32_t)(rd(pDS, 0xCF7C) + EAX));
L5BEB:
    /* 5BEB  mov     eax,dword ptr ds:[0CF90h] */
    EAX = rd(pDS, 0xCF90);
L5BEF:
    /* 5BEF  add     dword ptr ds:[0CF68h],eax */
    wd(pDS, 0xCF68, (uint32_t)(rd(pDS, 0xCF68) + EAX));
L5BF4:
    /* 5BF4  mov     eax,dword ptr ds:[0CFA0h] */
    EAX = rd(pDS, 0xCFA0);
L5BF8:
    /* 5BF8  add     dword ptr ds:[0CF78h],eax */
    wd(pDS, 0xCF78, (uint32_t)(rd(pDS, 0xCF78) + EAX));
L5BFD:
    /* 5BFD  mov     eax,dword ptr ds:[0CFA8h] */
    EAX = rd(pDS, 0xCFA8);
L5C01:
    /* 5C01  add     dword ptr ds:[0CF80h],eax */
    wd(pDS, 0xCF80, (uint32_t)(rd(pDS, 0xCF80) + EAX));
L5C06:
    /* 5C06  test    word ptr ds:[0CCF0h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0xCCF0) & 0xFFFF));
L5C0C:
    /* 5C0C  jne     short L5C34 */
    if (!ZF) goto L5C34;
L5C0E:
    /* 5C0E  mov     eax,dword ptr ds:[0CF88h] */
    EAX = rd(pDS, 0xCF88);
L5C12:
    /* 5C12  add     dword ptr ds:[0CF60h],eax */
    wd(pDS, 0xCF60, (uint32_t)(rd(pDS, 0xCF60) + EAX));
L5C17:
    /* 5C17  mov     eax,dword ptr ds:[0CF8Ch] */
    EAX = rd(pDS, 0xCF8C);
L5C1B:
    /* 5C1B  add     dword ptr ds:[0CF64h],eax */
    wd(pDS, 0xCF64, (uint32_t)(rd(pDS, 0xCF64) + EAX));
L5C20:
    /* 5C20  mov     eax,dword ptr ds:[0CF94h] */
    EAX = rd(pDS, 0xCF94);
L5C24:
    /* 5C24  add     dword ptr ds:[0CF6Ch],eax */
    wd(pDS, 0xCF6C, (uint32_t)(rd(pDS, 0xCF6C) + EAX));
L5C29:
    /* 5C29  mov     eax,dword ptr ds:[0CF98h] */
    EAX = rd(pDS, 0xCF98);
L5C2D:
    /* 5C2D  add     dword ptr ds:[0CF70h],eax */
    wd(pDS, 0xCF70, (uint32_t)(rd(pDS, 0xCF70) + EAX));
L5C32:
    /* 5C32  jmp     short L5C58 */
    goto L5C58;
L5C34: /* L5C34 */
    /* 5C34  mov     eax,dword ptr ds:[0CF88h] */
    EAX = rd(pDS, 0xCF88);
L5C38:
    /* 5C38  add     dword ptr ds:[0CFACh],eax */
    wd(pDS, 0xCFAC, (uint32_t)(rd(pDS, 0xCFAC) + EAX));
L5C3D:
    /* 5C3D  mov     eax,dword ptr ds:[0CF8Ch] */
    EAX = rd(pDS, 0xCF8C);
L5C41:
    /* 5C41  add     dword ptr ds:[0CFB4h],eax */
    wd(pDS, 0xCFB4, (uint32_t)(rd(pDS, 0xCFB4) + EAX));
L5C46:
    /* 5C46  mov     eax,dword ptr ds:[0CF94h] */
    EAX = rd(pDS, 0xCF94);
L5C4A:
    /* 5C4A  add     dword ptr ds:[0CFB0h],eax */
    wd(pDS, 0xCFB0, (uint32_t)(rd(pDS, 0xCFB0) + EAX));
L5C4F:
    /* 5C4F  mov     eax,dword ptr ds:[0CF98h] */
    EAX = rd(pDS, 0xCF98);
L5C53:
    /* 5C53  add     dword ptr ds:[0CFB8h],eax */
    wd(pDS, 0xCFB8, (uint32_t)(rd(pDS, 0xCFB8) + EAX));
L5C58: /* L5C58 */
    /* 5C58  inc     word ptr ds:[0CF5Ah] */
    ww(pDS, 0xCF5A, (uint16_t)(rw(pDS, 0xCF5A) + 1));
L5C5C:
    /* 5C5C  mov     ax,word ptr ds:[0CF5Ah] */
    AX = rw(pDS, 0xCF5A);
L5C5F:
    /* 5C5F  cmp     ax,word ptr ds:[0CF58h] */
    sub16(AX, rw(pDS, 0xCF58), 0);
L5C63:
    /* 5C63  jl      L5AC4 */
    if (SF != OF) goto L5AC4;
L5C67:
    /* 5C67  test    word ptr ds:[0CCF0h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0xCCF0) & 0xFFFF));
L5C6D:
    /* 5C6D  je      short L5CCB */
    if (ZF) goto L5CCB;
L5C6F:
    /* 5C6F  mov     eax,dword ptr ds:[0CFB0h] */
    EAX = rd(pDS, 0xCFB0);
L5C73:
    /* 5C73  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5C77:
    /* 5C77  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5C7B:
    /* 5C7B  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5C7D:
    /* 5C7D  idiv    dword ptr ds:[0CF78h] */
    if (asm_idiv32(rd(pDS, 0xCF78)) && (c = asm_divfault(0x065C, 0x5C7D, 5)) != 0) return c;
L5C82:
    /* 5C82  mov     dword ptr ds:[0CF6Ch],eax */
    wd(pDS, 0xCF6C, EAX);
L5C86:
    /* 5C86  mov     eax,dword ptr ds:[0CFACh] */
    EAX = rd(pDS, 0xCFAC);
L5C8A:
    /* 5C8A  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5C8E:
    /* 5C8E  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5C92:
    /* 5C92  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5C94:
    /* 5C94  idiv    dword ptr ds:[0CF74h] */
    if (asm_idiv32(rd(pDS, 0xCF74)) && (c = asm_divfault(0x065C, 0x5C94, 5)) != 0) return c;
L5C99:
    /* 5C99  mov     dword ptr ds:[0CF60h],eax */
    wd(pDS, 0xCF60, EAX);
L5C9D:
    /* 5C9D  mov     eax,dword ptr ds:[0CFB8h] */
    EAX = rd(pDS, 0xCFB8);
L5CA1:
    /* 5CA1  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5CA5:
    /* 5CA5  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5CA9:
    /* 5CA9  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5CAB:
    /* 5CAB  idiv    dword ptr ds:[0CF78h] */
    if (asm_idiv32(rd(pDS, 0xCF78)) && (c = asm_divfault(0x065C, 0x5CAB, 5)) != 0) return c;
L5CB0:
    /* 5CB0  mov     dword ptr ds:[0CF70h],eax */
    wd(pDS, 0xCF70, EAX);
L5CB4:
    /* 5CB4  mov     eax,dword ptr ds:[0CFB4h] */
    EAX = rd(pDS, 0xCFB4);
L5CB8:
    /* 5CB8  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5CBC:
    /* 5CBC  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5CC0:
    /* 5CC0  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5CC2:
    /* 5CC2  idiv    dword ptr ds:[0CF74h] */
    if (asm_idiv32(rd(pDS, 0xCF74)) && (c = asm_divfault(0x065C, 0x5CC2, 5)) != 0) return c;
L5CC7:
    /* 5CC7  mov     dword ptr ds:[0CF64h],eax */
    wd(pDS, 0xCF64, EAX);
L5CCB: /* L5CCB */
    /* 5CCB  call    _seg004_0849_5CDF */
    if ((c = asm_call(ASM_JMP(0x065C, 0x5CDF), 0x5CCE)) != 0) return c;
L5CCE:
    /* 5CCE  call    word ptr ds:[0CF42h] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(pDS, 0xCF42)), 0x5CD2)) != 0) return c;
L5CD2: /* L5CD2 */
    /* 5CD2  mov     word ptr ds:[0CF44h],0 */
    ww(pDS, 0xCF44, 0x0);
L5CD8:
    /* 5CD8  pop     gs */
    SET_GS(pop16());
L5CDA:
    /* 5CDA  pop     es */
    SET_ES(pop16());
L5CDB:
    /* 5CDB  pop     di */
    DI = pop16();
L5CDC:
    /* 5CDC  pop     si */
    SI = pop16();
L5CDD:
    /* 5CDD  pop     bp */
    BP = pop16();
L5CDE:
    /* 5CDE  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_5CDF  (+5CDF)
       seg004_0849_5CDF: clamp the row's span: x ends below 0 become 0, x ends at or past 2 * scrw
       (DS:2472 * 2, the view width) become 2 * scrw - 1, and a row at or past 2 * scrh becomes the
       last row. Changes AX. */
L5CDF: /* _seg004_0849_5CDF */
    /* 5CDF  test    word ptr ds:[0CF5Eh],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0xCF5E) & 0xFFFF));
L5CE5:
    /* 5CE5  jns     short L5CF0 */
    if (!SF) goto L5CF0;
L5CE7:
    /* 5CE7  mov     dword ptr ds:[0CF5Ch],0 */
    wd(pDS, 0xCF5C, 0x0);
L5CF0: /* L5CF0 */
    /* 5CF0  test    word ptr ds:[0CF6Ah],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0xCF6A) & 0xFFFF));
L5CF6:
    /* 5CF6  jns     short L5D01 */
    if (!SF) goto L5D01;
L5CF8:
    /* 5CF8  mov     dword ptr ds:[0CF68h],0 */
    wd(pDS, 0xCF68, 0x0);
L5D01: /* L5D01 */
    /* 5D01  mov     ax,word ptr ds:[2472h] */
    AX = rw(pDS, 0x2472);
L5D04:
    /* 5D04  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5D06:
    /* 5D06  cmp     word ptr ds:[0CF5Eh],ax */
    sub16(rw(pDS, 0xCF5E), AX, 0);
L5D0A:
    /* 5D0A  jb      short L5D13 */
    if (CF) goto L5D13;
L5D0C:
    /* 5D0C  mov     word ptr ds:[0CF5Eh],ax */
    ww(pDS, 0xCF5E, AX);
L5D0F:
    /* 5D0F  dec     word ptr ds:[0CF5Eh] */
    ww(pDS, 0xCF5E, (uint16_t)(rw(pDS, 0xCF5E) - 1));
L5D13: /* L5D13 */
    /* 5D13  cmp     word ptr ds:[0CF6Ah],ax */
    sub16(rw(pDS, 0xCF6A), AX, 0);
L5D17:
    /* 5D17  jb      short L5D1D */
    if (CF) goto L5D1D;
L5D19:
    /* 5D19  dec     ax */
    AX = (uint16_t)(AX - 1);
L5D1A:
    /* 5D1A  mov     word ptr ds:[0CF6Ah],ax */
    ww(pDS, 0xCF6A, AX);
L5D1D: /* L5D1D */
    /* 5D1D  mov     ax,word ptr ds:[2470h] */
    AX = rw(pDS, 0x2470);
L5D20:
    /* 5D20  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5D22:
    /* 5D22  cmp     word ptr ds:[0CF5Ah],ax */
    sub16(rw(pDS, 0xCF5A), AX, 0);
L5D26:
    /* 5D26  jae     short L5D29 */
    if (!CF) goto L5D29;
L5D28:
    /* 5D28  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L5D29: /* L5D29 */
    /* 5D29  dec     ax */
    AX = dec16(AX);
L5D2A:
    /* 5D2A  mov     word ptr ds:[0CF5Ah],ax */
    ww(pDS, 0xCF5A, AX);
L5D2D:
    /* 5D2D  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_5D2E  (+5D2E)
       seg004_0849_5D2E: choose the span drawer into CF42: linear (_lin_) for a floor or ceiling
       (CCF0) or for a polygon with mapping 0 or 1, else perspective (_per_); shaded (tlp) or not
       (tp) by the shaded argument. Changes AX. */
L5D2E: /* _seg004_0849_5D2E */
    /* 5D2E  test    word ptr ds:[0CCF0h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0xCCF0) & 0xFFFF));
L5D34:
    /* 5D34  jne     L5D3E */
    if (!ZF) goto L5D3E;
L5D36:
    /* 5D36  nop */
    ;
L5D37:
    /* 5D37  nop */
    ;
L5D38:
    /* 5D38  cmp     word ptr [bp+0Ah],1 */
    sub16(rw(pSS, BP + 0xA), 0x1, 0);
L5D3C:
    /* 5D3C  ja      short L5D51 */
    if (!CF && !ZF) goto L5D51;
L5D3E: /* L5D3E */
    /* 5D3E  test    word ptr [bp+0Ch],0FFFFh */
    logic16((uint16_t)(rw(pSS, BP + 0xC) & 0xFFFF));
L5D43:
    /* 5D43  je      short L5D4A */
    if (ZF) goto L5D4A;
L5D45:
    /* 5D45  mov     ax,offset _asm_texture_map_scanline_lin_tlp */
    AX = 0x4925;
L5D48:
    /* 5D48  jmp     short L5D4F */
    goto L5D4F;
L5D4A: /* L5D4A */
    /* 5D4A  mov     ax,offset _asm_texture_map_scanline_lin_tp */
    AX = 0x4CBA;
L5D4D:
    /* 5D4D  jmp     short L5D4F */
    goto L5D4F;
L5D4F: /* L5D4F */
    /* 5D4F  jmp     short L5D62 */
    goto L5D62;
L5D51: /* L5D51 */
    /* 5D51  test    word ptr [bp+0Ch],0FFFFh */
    logic16((uint16_t)(rw(pSS, BP + 0xC) & 0xFFFF));
L5D56:
    /* 5D56  je      short L5D5D */
    if (ZF) goto L5D5D;
L5D58:
    /* 5D58  mov     ax,offset _asm_texture_map_scanline_per_tlp */
    AX = 0x46F0;
L5D5B:
    /* 5D5B  jmp     short L5D62 */
    goto L5D62;
L5D5D: /* L5D5D */
    /* 5D5D  mov     ax,offset _asm_texture_map_scanline_per_tp */
    AX = 0x4B30;
L5D60:
    /* 5D60  jmp     short L5D62 */
    goto L5D62;
L5D62: /* L5D62 */
    /* 5D62  mov     word ptr ds:[0CF42h],ax */
    ww(pDS, 0xCF42, AX);
L5D65:
    /* 5D65  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_5D66  (+5D66)
       seg004_0849_5D66: set up both edges from the first vertex at once: the left edge from
       _cur_left_ind to the vertex before it, the right edge from _cur_right_ind to the one after,
       with the starting values and per-row steps of seg004_0849_54C0 and 5712 (an edge of zero rows
       is taken as one row here). In: BP = _asm_texture_map's frame. */
L5D66: /* _seg004_0849_5D66 */
    /* 5D66  mov     ax,20h */
    AX = 0x20;
L5D69:
    /* 5D69  imul    word ptr ds:[0CF52h] */
    imul16(rw(pDS, 0xCF52));
L5D6D:
    /* 5D6D  add     ax,word ptr [bp+6] */
    AX = (uint16_t)(AX + rw(pSS, BP + 0x6));
L5D70:
    /* 5D70  mov     word ptr ds:[0CFC4h],ax */
    ww(pDS, 0xCFC4, AX);
L5D73:
    /* 5D73  mov     ax,word ptr ds:[0CF52h] */
    AX = rw(pDS, 0xCF52);
L5D76:
    /* 5D76  dec     ax */
    AX = dec16(AX);
L5D77:
    /* 5D77  jns     short L5D7C */
    if (!SF) goto L5D7C;
L5D79:
    /* 5D79  add     ax,word ptr [bp+8] */
    AX = (uint16_t)(AX + rw(pSS, BP + 0x8));
L5D7C: /* L5D7C */
    /* 5D7C  imul    ax,20h */
    AX = imul16x(AX, 0x20);
L5D7F:
    /* 5D7F  add     ax,word ptr [bp+6] */
    AX = (uint16_t)(AX + rw(pSS, BP + 0x6));
L5D82:
    /* 5D82  mov     word ptr ds:[0CFC6h],ax */
    ww(pDS, 0xCFC6, AX);
L5D85:
    /* 5D85  mov     ax,20h */
    AX = 0x20;
L5D88:
    /* 5D88  imul    word ptr ds:[0CF54h] */
    imul16(rw(pDS, 0xCF54));
L5D8C:
    /* 5D8C  add     ax,word ptr [bp+6] */
    AX = (uint16_t)(AX + rw(pSS, BP + 0x6));
L5D8F:
    /* 5D8F  mov     word ptr ds:[0CFC8h],ax */
    ww(pDS, 0xCFC8, AX);
L5D92:
    /* 5D92  mov     ax,word ptr ds:[0CF54h] */
    AX = rw(pDS, 0xCF54);
L5D95:
    /* 5D95  inc     ax */
    AX = (uint16_t)(AX + 1);
L5D96:
    /* 5D96  cmp     ax,word ptr [bp+8] */
    sub16(AX, rw(pSS, BP + 0x8), 0);
L5D99:
    /* 5D99  jb      short L5D9D */
    if (CF) goto L5D9D;
L5D9B:
    /* 5D9B  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5D9D: /* L5D9D */
    /* 5D9D  imul    ax,20h */
    AX = imul16x(AX, 0x20);
L5DA0:
    /* 5DA0  add     ax,word ptr [bp+6] */
    AX = (uint16_t)(AX + rw(pSS, BP + 0x6));
L5DA3:
    /* 5DA3  mov     word ptr ds:[0CFCAh],ax */
    ww(pDS, 0xCFCA, AX);
L5DA6:
    /* 5DA6  mov     si,word ptr ds:[0CFC6h] */
    SI = rw(pDS, 0xCFC6);
L5DAA:
    /* 5DAA  mov     eax,dword ptr [si+10h] */
    EAX = rd(pDS, SI + 0x10);
L5DAE:
    /* 5DAE  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L5DB2:
    /* 5DB2  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L5DB6:
    /* 5DB6  mov     si,word ptr ds:[0CFC4h] */
    SI = rw(pDS, 0xCFC4);
L5DBA:
    /* 5DBA  mov     ecx,dword ptr [si+10h] */
    ECX = rd(pDS, SI + 0x10);
L5DBE:
    /* 5DBE  add     ecx,41h */
    ECX = (uint32_t)(ECX + 0x41);
L5DC2:
    /* 5DC2  sar     ecx,10h */
    ECX = (uint32_t)((int32_t)ECX >> 16);
L5DC6:
    /* 5DC6  sub     ax,cx */
    AX = sub16(AX, CX, 0);
L5DC8:
    /* 5DC8  jne     short L5DCB */
    if (!ZF) goto L5DCB;
L5DCA:
    /* 5DCA  inc     ax */
    AX = (uint16_t)(AX + 1);
L5DCB: /* L5DCB */
    /* 5DCB  shl     eax,10h */
    EAX = (uint32_t)(EAX << 16);
L5DCF:
    /* 5DCF  mov     dword ptr ds:[0CFBCh],eax */
    wd(pDS, 0xCFBC, EAX);
L5DD3:
    /* 5DD3  mov     si,word ptr ds:[0CFCAh] */
    SI = rw(pDS, 0xCFCA);
L5DD7:
    /* 5DD7  mov     eax,dword ptr [si+10h] */
    EAX = rd(pDS, SI + 0x10);
L5DDB:
    /* 5DDB  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L5DDF:
    /* 5DDF  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L5DE3:
    /* 5DE3  mov     si,word ptr ds:[0CFC8h] */
    SI = rw(pDS, 0xCFC8);
L5DE7:
    /* 5DE7  mov     ecx,dword ptr [si+10h] */
    ECX = rd(pDS, SI + 0x10);
L5DEB:
    /* 5DEB  add     ecx,41h */
    ECX = (uint32_t)(ECX + 0x41);
L5DEF:
    /* 5DEF  sar     ecx,10h */
    ECX = (uint32_t)((int32_t)ECX >> 16);
L5DF3:
    /* 5DF3  sub     ax,cx */
    AX = sub16(AX, CX, 0);
L5DF5:
    /* 5DF5  jne     short L5DF8 */
    if (!ZF) goto L5DF8;
L5DF7:
    /* 5DF7  inc     ax */
    AX = (uint16_t)(AX + 1);
L5DF8: /* L5DF8 */
    /* 5DF8  shl     eax,10h */
    EAX = (uint32_t)(EAX << 16);
L5DFC:
    /* 5DFC  mov     dword ptr ds:[0CFC0h],eax */
    wd(pDS, 0xCFC0, EAX);
L5E00:
    /* 5E00  mov     si,word ptr ds:[0CFC4h] */
    SI = rw(pDS, 0xCFC4);
L5E04:
    /* 5E04  mov     ecx,dword ptr [si+0Ch] */
    ECX = rd(pDS, SI + 0xC);
L5E08:
    /* 5E08  mov     dword ptr ds:[0CF5Ch],ecx */
    wd(pDS, 0xCF5C, ECX);
L5E0D:
    /* 5E0D  mov     si,word ptr ds:[0CFC6h] */
    SI = rw(pDS, 0xCFC6);
L5E11:
    /* 5E11  mov     eax,dword ptr [si+0Ch] */
    EAX = rd(pDS, SI + 0xC);
L5E15:
    /* 5E15  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L5E18:
    /* 5E18  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5E1C:
    /* 5E1C  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5E20:
    /* 5E20  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5E22:
    /* 5E22  idiv    dword ptr ds:[0CFBCh] */
    if (asm_idiv32(rd(pDS, 0xCFBC)) && (c = asm_divfault(0x065C, 0x5E22, 5)) != 0) return c;
L5E27:
    /* 5E27  mov     dword ptr ds:[0CF84h],eax */
    wd(pDS, 0xCF84, EAX);
L5E2B:
    /* 5E2B  mov     si,word ptr ds:[0CFC8h] */
    SI = rw(pDS, 0xCFC8);
L5E2F:
    /* 5E2F  mov     ecx,dword ptr [si+0Ch] */
    ECX = rd(pDS, SI + 0xC);
L5E33:
    /* 5E33  mov     dword ptr ds:[0CF68h],ecx */
    wd(pDS, 0xCF68, ECX);
L5E38:
    /* 5E38  mov     si,word ptr ds:[0CFCAh] */
    SI = rw(pDS, 0xCFCA);
L5E3C:
    /* 5E3C  mov     eax,dword ptr [si+0Ch] */
    EAX = rd(pDS, SI + 0xC);
L5E40:
    /* 5E40  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L5E43:
    /* 5E43  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5E47:
    /* 5E47  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5E4B:
    /* 5E4B  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5E4D:
    /* 5E4D  idiv    dword ptr ds:[0CFC0h] */
    if (asm_idiv32(rd(pDS, 0xCFC0)) && (c = asm_divfault(0x065C, 0x5E4D, 5)) != 0) return c;
L5E52:
    /* 5E52  mov     dword ptr ds:[0CF90h],eax */
    wd(pDS, 0xCF90, EAX);
L5E56:
    /* 5E56  mov     si,word ptr ds:[0CFC4h] */
    SI = rw(pDS, 0xCFC4);
L5E5A:
    /* 5E5A  mov     di,word ptr ds:[0CFC6h] */
    DI = rw(pDS, 0xCFC6);
L5E5E:
    /* 5E5E  cmp     word ptr [bp+0Ah],1 */
    sub16(rw(pSS, BP + 0xA), 0x1, 0);
L5E62:
    /* 5E62  ja      L5F65 */
    if (!CF && !ZF) goto L5F65;
L5E66:
    /* 5E66  mov     eax,dword ptr [si+1Ch] */
    EAX = rd(pDS, SI + 0x1C);
L5E6A:
    /* 5E6A  mov     dword ptr ds:[0CF7Ch],eax */
    wd(pDS, 0xCF7C, EAX);
L5E6E:
    /* 5E6E  mov     dword ptr ds:[0CF74h],10000h */
    wd(pDS, 0xCF74, 0x10000);
L5E77:
    /* 5E77  mov     eax,dword ptr [si+14h] */
    EAX = rd(pDS, SI + 0x14);
L5E7B:
    /* 5E7B  mov     dword ptr ds:[0CF60h],eax */
    wd(pDS, 0xCF60, EAX);
L5E7F:
    /* 5E7F  mov     eax,dword ptr [si+18h] */
    EAX = rd(pDS, SI + 0x18);
L5E83:
    /* 5E83  mov     dword ptr ds:[0CF64h],eax */
    wd(pDS, 0xCF64, EAX);
L5E87:
    /* 5E87  sub     eax,eax */
    EAX = (uint32_t)(EAX - EAX);
L5E8A:
    /* 5E8A  mov     dword ptr ds:[0CF9Ch],eax */
    wd(pDS, 0xCF9C, EAX);
L5E8E:
    /* 5E8E  mov     eax,dword ptr [di+14h] */
    EAX = rd(pDS, DI + 0x14);
L5E92:
    /* 5E92  sub     eax,dword ptr [si+14h] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x14));
L5E96:
    /* 5E96  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5E9A:
    /* 5E9A  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5E9E:
    /* 5E9E  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5EA0:
    /* 5EA0  idiv    dword ptr ds:[0CFBCh] */
    if (asm_idiv32(rd(pDS, 0xCFBC)) && (c = asm_divfault(0x065C, 0x5EA0, 5)) != 0) return c;
L5EA5:
    /* 5EA5  mov     dword ptr ds:[0CF88h],eax */
    wd(pDS, 0xCF88, EAX);
L5EA9:
    /* 5EA9  mov     eax,dword ptr [di+18h] */
    EAX = rd(pDS, DI + 0x18);
L5EAD:
    /* 5EAD  sub     eax,dword ptr [si+18h] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x18));
L5EB1:
    /* 5EB1  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5EB5:
    /* 5EB5  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5EB9:
    /* 5EB9  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5EBB:
    /* 5EBB  idiv    dword ptr ds:[0CFBCh] */
    if (asm_idiv32(rd(pDS, 0xCFBC)) && (c = asm_divfault(0x065C, 0x5EBB, 5)) != 0) return c;
L5EC0:
    /* 5EC0  mov     dword ptr ds:[0CF8Ch],eax */
    wd(pDS, 0xCF8C, EAX);
L5EC4:
    /* 5EC4  mov     eax,dword ptr [di+1Ch] */
    EAX = rd(pDS, DI + 0x1C);
L5EC8:
    /* 5EC8  sub     eax,dword ptr [si+1Ch] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x1C));
L5ECC:
    /* 5ECC  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5ED0:
    /* 5ED0  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5ED4:
    /* 5ED4  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5ED6:
    /* 5ED6  idiv    dword ptr ds:[0CFBCh] */
    if (asm_idiv32(rd(pDS, 0xCFBC)) && (c = asm_divfault(0x065C, 0x5ED6, 5)) != 0) return c;
L5EDB:
    /* 5EDB  mov     dword ptr ds:[0CFA4h],eax */
    wd(pDS, 0xCFA4, EAX);
L5EDF:
    /* 5EDF  mov     dword ptr ds:[0CF78h],10000h */
    wd(pDS, 0xCF78, 0x10000);
L5EE8:
    /* 5EE8  mov     bx,word ptr ds:[0CFC8h] */
    BX = rw(pDS, 0xCFC8);
L5EEC:
    /* 5EEC  mov     eax,dword ptr [bx+1Ch] */
    EAX = rd(pDS, BX + 0x1C);
L5EF0:
    /* 5EF0  mov     dword ptr ds:[0CF80h],eax */
    wd(pDS, 0xCF80, EAX);
L5EF4:
    /* 5EF4  mov     eax,dword ptr [bx+14h] */
    EAX = rd(pDS, BX + 0x14);
L5EF8:
    /* 5EF8  mov     dword ptr ds:[0CF6Ch],eax */
    wd(pDS, 0xCF6C, EAX);
L5EFC:
    /* 5EFC  mov     eax,dword ptr [bx+18h] */
    EAX = rd(pDS, BX + 0x18);
L5F00:
    /* 5F00  mov     dword ptr ds:[0CF70h],eax */
    wd(pDS, 0xCF70, EAX);
L5F04:
    /* 5F04  sub     eax,eax */
    EAX = (uint32_t)(EAX - EAX);
L5F07:
    /* 5F07  mov     dword ptr ds:[0CFA0h],eax */
    wd(pDS, 0xCFA0, EAX);
L5F0B:
    /* 5F0B  push    si */
    push16(SI);
L5F0C:
    /* 5F0C  mov     si,word ptr ds:[0CFCAh] */
    SI = rw(pDS, 0xCFCA);
L5F10:
    /* 5F10  mov     eax,dword ptr [si+14h] */
    EAX = rd(pDS, SI + 0x14);
L5F14:
    /* 5F14  sub     eax,dword ptr [bx+14h] */
    EAX = (uint32_t)(EAX - rd(pDS, BX + 0x14));
L5F18:
    /* 5F18  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5F1C:
    /* 5F1C  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5F20:
    /* 5F20  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5F22:
    /* 5F22  idiv    dword ptr ds:[0CFC0h] */
    if (asm_idiv32(rd(pDS, 0xCFC0)) && (c = asm_divfault(0x065C, 0x5F22, 5)) != 0) return c;
L5F27:
    /* 5F27  mov     dword ptr ds:[0CF94h],eax */
    wd(pDS, 0xCF94, EAX);
L5F2B:
    /* 5F2B  mov     eax,dword ptr [si+18h] */
    EAX = rd(pDS, SI + 0x18);
L5F2F:
    /* 5F2F  sub     eax,dword ptr [bx+18h] */
    EAX = (uint32_t)(EAX - rd(pDS, BX + 0x18));
L5F33:
    /* 5F33  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5F37:
    /* 5F37  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5F3B:
    /* 5F3B  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5F3D:
    /* 5F3D  idiv    dword ptr ds:[0CFC0h] */
    if (asm_idiv32(rd(pDS, 0xCFC0)) && (c = asm_divfault(0x065C, 0x5F3D, 5)) != 0) return c;
L5F42:
    /* 5F42  mov     dword ptr ds:[0CF98h],eax */
    wd(pDS, 0xCF98, EAX);
L5F46:
    /* 5F46  mov     eax,dword ptr [si+1Ch] */
    EAX = rd(pDS, SI + 0x1C);
L5F4A:
    /* 5F4A  sub     eax,dword ptr [bx+1Ch] */
    EAX = (uint32_t)(EAX - rd(pDS, BX + 0x1C));
L5F4E:
    /* 5F4E  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5F52:
    /* 5F52  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5F56:
    /* 5F56  sub     ax,ax */
    AX = sub16(AX, AX, 0);
L5F58:
    /* 5F58  idiv    dword ptr ds:[0CFC0h] */
    if (asm_idiv32(rd(pDS, 0xCFC0)) && (c = asm_divfault(0x065C, 0x5F58, 5)) != 0) return c;
L5F5D:
    /* 5F5D  mov     dword ptr ds:[0CFA8h],eax */
    wd(pDS, 0xCFA8, EAX);
L5F61:
    /* 5F61  pop     si */
    SI = pop16();
L5F62:
    /* 5F62  jmp     L6227 */
    goto L6227;
L5F65: /* L5F65 */
    /* 5F65  mov     eax,dword ptr [si+1Ch] */
    EAX = rd(pDS, SI + 0x1C);
L5F69:
    /* 5F69  mov     dword ptr ds:[0CF7Ch],eax */
    wd(pDS, 0xCF7C, EAX);
L5F6D:
    /* 5F6D  mov     eax,100h */
    EAX = 0x100;
L5F73:
    /* 5F73  cmp     dword ptr [si+8],eax */
    sub32(rd(pDS, SI + 0x8), EAX, 0);
L5F77:
    /* 5F77  ja      short L5F81 */
    if (!CF && !ZF) goto L5F81;
L5F79:
    /* 5F79  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L5F7F:
    /* 5F7F  jmp     short L5F95 */
    goto L5F95;
L5F81: /* L5F81 */
    /* 5F81  mov     eax,800000h */
    EAX = 0x800000;
L5F87:
    /* 5F87  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5F8B:
    /* 5F8B  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5F8F:
    /* 5F8F  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5F91:
    /* 5F91  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x5F91, 4)) != 0) return c;
L5F95: /* L5F95 */
    /* 5F95  mov     dword ptr ds:[0CF74h],eax */
    wd(pDS, 0xCF74, EAX);
L5F99:
    /* 5F99  imul    dword ptr [si+14h] */
    imul32(rd(pDS, SI + 0x14));
L5F9D:
    /* 5F9D  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L5FA2:
    /* 5FA2  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L5FA4:
    /* 5FA4  mov     dword ptr ds:[0CF60h],eax */
    wd(pDS, 0xCF60, EAX);
L5FA8:
    /* 5FA8  mov     eax,dword ptr [si+18h] */
    EAX = rd(pDS, SI + 0x18);
L5FAC:
    /* 5FAC  imul    dword ptr ds:[0CF74h] */
    imul32(rd(pDS, 0xCF74));
L5FB1:
    /* 5FB1  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L5FB6:
    /* 5FB6  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L5FB8:
    /* 5FB8  mov     dword ptr ds:[0CF64h],eax */
    wd(pDS, 0xCF64, EAX);
L5FBC:
    /* 5FBC  mov     eax,100h */
    EAX = 0x100;
L5FC2:
    /* 5FC2  cmp     dword ptr [si+8],eax */
    sub32(rd(pDS, SI + 0x8), EAX, 0);
L5FC6:
    /* 5FC6  ja      short L5FD0 */
    if (!CF && !ZF) goto L5FD0;
L5FC8:
    /* 5FC8  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L5FCE:
    /* 5FCE  jmp     short L5FE4 */
    goto L5FE4;
L5FD0: /* L5FD0 */
    /* 5FD0  mov     eax,800000h */
    EAX = 0x800000;
L5FD6:
    /* 5FD6  rol     eax,10h */
    EAX = rol32(EAX, 16);
L5FDA:
    /* 5FDA  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L5FDE:
    /* 5FDE  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L5FE0:
    /* 5FE0  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x5FE0, 4)) != 0) return c;
L5FE4: /* L5FE4 */
    /* 5FE4  mov     ecx,eax */
    ECX = EAX;
L5FE7:
    /* 5FE7  mov     eax,100h */
    EAX = 0x100;
L5FED:
    /* 5FED  cmp     dword ptr [di+8],eax */
    sub32(rd(pDS, DI + 0x8), EAX, 0);
L5FF1:
    /* 5FF1  ja      short L5FFB */
    if (!CF && !ZF) goto L5FFB;
L5FF3:
    /* 5FF3  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L5FF9:
    /* 5FF9  jmp     short L600F */
    goto L600F;
L5FFB: /* L5FFB */
    /* 5FFB  mov     eax,800000h */
    EAX = 0x800000;
L6001:
    /* 6001  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6005:
    /* 6005  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6009:
    /* 6009  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L600B:
    /* 600B  idiv    dword ptr [di+8] */
    if (asm_idiv32(rd(pDS, DI + 0x8)) && (c = asm_divfault(0x065C, 0x600B, 4)) != 0) return c;
L600F: /* L600F */
    /* 600F  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L6012:
    /* 6012  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6016:
    /* 6016  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L601A:
    /* 601A  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L601C:
    /* 601C  idiv    dword ptr ds:[0CFBCh] */
    if (asm_idiv32(rd(pDS, 0xCFBC)) && (c = asm_divfault(0x065C, 0x601C, 5)) != 0) return c;
L6021:
    /* 6021  mov     dword ptr ds:[0CF9Ch],eax */
    wd(pDS, 0xCF9C, EAX);
L6025:
    /* 6025  mov     eax,dword ptr [di+1Ch] */
    EAX = rd(pDS, DI + 0x1C);
L6029:
    /* 6029  sub     eax,dword ptr [si+1Ch] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x1C));
L602D:
    /* 602D  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6031:
    /* 6031  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6035:
    /* 6035  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6037:
    /* 6037  idiv    dword ptr ds:[0CFBCh] */
    if (asm_idiv32(rd(pDS, 0xCFBC)) && (c = asm_divfault(0x065C, 0x6037, 5)) != 0) return c;
L603C:
    /* 603C  mov     dword ptr ds:[0CFA4h],eax */
    wd(pDS, 0xCFA4, EAX);
L6040:
    /* 6040  mov     eax,dword ptr [si+14h] */
    EAX = rd(pDS, SI + 0x14);
L6044:
    /* 6044  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6048:
    /* 6048  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L604C:
    /* 604C  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L604E:
    /* 604E  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x604E, 4)) != 0) return c;
L6052:
    /* 6052  mov     ecx,eax */
    ECX = EAX;
L6055:
    /* 6055  mov     eax,dword ptr [di+14h] */
    EAX = rd(pDS, DI + 0x14);
L6059:
    /* 6059  rol     eax,10h */
    EAX = rol32(EAX, 16);
L605D:
    /* 605D  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6061:
    /* 6061  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6063:
    /* 6063  idiv    dword ptr [di+8] */
    if (asm_idiv32(rd(pDS, DI + 0x8)) && (c = asm_divfault(0x065C, 0x6063, 4)) != 0) return c;
L6067:
    /* 6067  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L606A:
    /* 606A  shl     eax,7 */
    EAX = (uint32_t)(EAX << 7);
L606E:
    /* 606E  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6072:
    /* 6072  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6076:
    /* 6076  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6078:
    /* 6078  idiv    dword ptr ds:[0CFBCh] */
    if (asm_idiv32(rd(pDS, 0xCFBC)) && (c = asm_divfault(0x065C, 0x6078, 5)) != 0) return c;
L607D:
    /* 607D  mov     dword ptr ds:[0CF88h],eax */
    wd(pDS, 0xCF88, EAX);
L6081:
    /* 6081  mov     eax,dword ptr [si+18h] */
    EAX = rd(pDS, SI + 0x18);
L6085:
    /* 6085  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6089:
    /* 6089  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L608D:
    /* 608D  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L608F:
    /* 608F  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x608F, 4)) != 0) return c;
L6093:
    /* 6093  mov     ecx,eax */
    ECX = EAX;
L6096:
    /* 6096  mov     eax,dword ptr [di+18h] */
    EAX = rd(pDS, DI + 0x18);
L609A:
    /* 609A  rol     eax,10h */
    EAX = rol32(EAX, 16);
L609E:
    /* 609E  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L60A2:
    /* 60A2  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L60A4:
    /* 60A4  idiv    dword ptr [di+8] */
    if (asm_idiv32(rd(pDS, DI + 0x8)) && (c = asm_divfault(0x065C, 0x60A4, 4)) != 0) return c;
L60A8:
    /* 60A8  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L60AB:
    /* 60AB  shl     eax,7 */
    EAX = (uint32_t)(EAX << 7);
L60AF:
    /* 60AF  rol     eax,10h */
    EAX = rol32(EAX, 16);
L60B3:
    /* 60B3  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L60B7:
    /* 60B7  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L60B9:
    /* 60B9  idiv    dword ptr ds:[0CFBCh] */
    if (asm_idiv32(rd(pDS, 0xCFBC)) && (c = asm_divfault(0x065C, 0x60B9, 5)) != 0) return c;
L60BE:
    /* 60BE  mov     dword ptr ds:[0CF8Ch],eax */
    wd(pDS, 0xCF8C, EAX);
L60C2:
    /* 60C2  mov     si,word ptr ds:[0CFC8h] */
    SI = rw(pDS, 0xCFC8);
L60C6:
    /* 60C6  mov     eax,dword ptr [si+1Ch] */
    EAX = rd(pDS, SI + 0x1C);
L60CA:
    /* 60CA  mov     dword ptr ds:[0CF80h],eax */
    wd(pDS, 0xCF80, EAX);
L60CE:
    /* 60CE  mov     eax,100h */
    EAX = 0x100;
L60D4:
    /* 60D4  cmp     dword ptr [si+8],eax */
    sub32(rd(pDS, SI + 0x8), EAX, 0);
L60D8:
    /* 60D8  ja      short L60E2 */
    if (!CF && !ZF) goto L60E2;
L60DA:
    /* 60DA  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L60E0:
    /* 60E0  jmp     short L60F6 */
    goto L60F6;
L60E2: /* L60E2 */
    /* 60E2  mov     eax,800000h */
    EAX = 0x800000;
L60E8:
    /* 60E8  rol     eax,10h */
    EAX = rol32(EAX, 16);
L60EC:
    /* 60EC  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L60F0:
    /* 60F0  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L60F2:
    /* 60F2  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x60F2, 4)) != 0) return c;
L60F6: /* L60F6 */
    /* 60F6  mov     dword ptr ds:[0CF78h],eax */
    wd(pDS, 0xCF78, EAX);
L60FA:
    /* 60FA  imul    dword ptr [si+14h] */
    imul32(rd(pDS, SI + 0x14));
L60FE:
    /* 60FE  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L6103:
    /* 6103  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L6105:
    /* 6105  mov     dword ptr ds:[0CF6Ch],eax */
    wd(pDS, 0xCF6C, EAX);
L6109:
    /* 6109  mov     eax,dword ptr [si+18h] */
    EAX = rd(pDS, SI + 0x18);
L610D:
    /* 610D  imul    dword ptr ds:[0CF78h] */
    imul32(rd(pDS, 0xCF78));
L6112:
    /* 6112  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L6117:
    /* 6117  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L6119:
    /* 6119  mov     dword ptr ds:[0CF70h],eax */
    wd(pDS, 0xCF70, EAX);
L611D:
    /* 611D  mov     di,word ptr ds:[0CFCAh] */
    DI = rw(pDS, 0xCFCA);
L6121:
    /* 6121  mov     eax,100h */
    EAX = 0x100;
L6127:
    /* 6127  cmp     dword ptr [si+8],eax */
    sub32(rd(pDS, SI + 0x8), EAX, 0);
L612B:
    /* 612B  ja      short L6135 */
    if (!CF && !ZF) goto L6135;
L612D:
    /* 612D  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L6133:
    /* 6133  jmp     short L6149 */
    goto L6149;
L6135: /* L6135 */
    /* 6135  mov     eax,800000h */
    EAX = 0x800000;
L613B:
    /* 613B  rol     eax,10h */
    EAX = rol32(EAX, 16);
L613F:
    /* 613F  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6143:
    /* 6143  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6145:
    /* 6145  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x6145, 4)) != 0) return c;
L6149: /* L6149 */
    /* 6149  mov     ecx,eax */
    ECX = EAX;
L614C:
    /* 614C  mov     eax,100h */
    EAX = 0x100;
L6152:
    /* 6152  cmp     dword ptr [di+8],eax */
    sub32(rd(pDS, DI + 0x8), EAX, 0);
L6156:
    /* 6156  ja      short L6160 */
    if (!CF && !ZF) goto L6160;
L6158:
    /* 6158  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L615E:
    /* 615E  jmp     short L6174 */
    goto L6174;
L6160: /* L6160 */
    /* 6160  mov     eax,800000h */
    EAX = 0x800000;
L6166:
    /* 6166  rol     eax,10h */
    EAX = rol32(EAX, 16);
L616A:
    /* 616A  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L616E:
    /* 616E  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6170:
    /* 6170  idiv    dword ptr [di+8] */
    if (asm_idiv32(rd(pDS, DI + 0x8)) && (c = asm_divfault(0x065C, 0x6170, 4)) != 0) return c;
L6174: /* L6174 */
    /* 6174  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L6177:
    /* 6177  rol     eax,10h */
    EAX = rol32(EAX, 16);
L617B:
    /* 617B  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L617F:
    /* 617F  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6181:
    /* 6181  idiv    dword ptr ds:[0CFC0h] */
    if (asm_idiv32(rd(pDS, 0xCFC0)) && (c = asm_divfault(0x065C, 0x6181, 5)) != 0) return c;
L6186:
    /* 6186  mov     dword ptr ds:[0CFA0h],eax */
    wd(pDS, 0xCFA0, EAX);
L618A:
    /* 618A  mov     eax,dword ptr [di+1Ch] */
    EAX = rd(pDS, DI + 0x1C);
L618E:
    /* 618E  sub     eax,dword ptr [si+1Ch] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x1C));
L6192:
    /* 6192  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6196:
    /* 6196  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L619A:
    /* 619A  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L619C:
    /* 619C  idiv    dword ptr ds:[0CFC0h] */
    if (asm_idiv32(rd(pDS, 0xCFC0)) && (c = asm_divfault(0x065C, 0x619C, 5)) != 0) return c;
L61A1:
    /* 61A1  mov     dword ptr ds:[0CFA8h],eax */
    wd(pDS, 0xCFA8, EAX);
L61A5:
    /* 61A5  mov     eax,dword ptr [si+14h] */
    EAX = rd(pDS, SI + 0x14);
L61A9:
    /* 61A9  rol     eax,10h */
    EAX = rol32(EAX, 16);
L61AD:
    /* 61AD  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L61B1:
    /* 61B1  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L61B3:
    /* 61B3  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x61B3, 4)) != 0) return c;
L61B7:
    /* 61B7  mov     ecx,eax */
    ECX = EAX;
L61BA:
    /* 61BA  mov     eax,dword ptr [di+14h] */
    EAX = rd(pDS, DI + 0x14);
L61BE:
    /* 61BE  rol     eax,10h */
    EAX = rol32(EAX, 16);
L61C2:
    /* 61C2  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L61C6:
    /* 61C6  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L61C8:
    /* 61C8  idiv    dword ptr [di+8] */
    if (asm_idiv32(rd(pDS, DI + 0x8)) && (c = asm_divfault(0x065C, 0x61C8, 4)) != 0) return c;
L61CC:
    /* 61CC  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L61CF:
    /* 61CF  shl     eax,7 */
    EAX = (uint32_t)(EAX << 7);
L61D3:
    /* 61D3  rol     eax,10h */
    EAX = rol32(EAX, 16);
L61D7:
    /* 61D7  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L61DB:
    /* 61DB  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L61DD:
    /* 61DD  idiv    dword ptr ds:[0CFC0h] */
    if (asm_idiv32(rd(pDS, 0xCFC0)) && (c = asm_divfault(0x065C, 0x61DD, 5)) != 0) return c;
L61E2:
    /* 61E2  mov     dword ptr ds:[0CF94h],eax */
    wd(pDS, 0xCF94, EAX);
L61E6:
    /* 61E6  mov     eax,dword ptr [si+18h] */
    EAX = rd(pDS, SI + 0x18);
L61EA:
    /* 61EA  rol     eax,10h */
    EAX = rol32(EAX, 16);
L61EE:
    /* 61EE  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L61F2:
    /* 61F2  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L61F4:
    /* 61F4  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x61F4, 4)) != 0) return c;
L61F8:
    /* 61F8  mov     ecx,eax */
    ECX = EAX;
L61FB:
    /* 61FB  mov     eax,dword ptr [di+18h] */
    EAX = rd(pDS, DI + 0x18);
L61FF:
    /* 61FF  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6203:
    /* 6203  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6207:
    /* 6207  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6209:
    /* 6209  idiv    dword ptr [di+8] */
    if (asm_idiv32(rd(pDS, DI + 0x8)) && (c = asm_divfault(0x065C, 0x6209, 4)) != 0) return c;
L620D:
    /* 620D  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L6210:
    /* 6210  shl     eax,7 */
    EAX = (uint32_t)(EAX << 7);
L6214:
    /* 6214  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6218:
    /* 6218  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L621C:
    /* 621C  sub     ax,ax */
    AX = sub16(AX, AX, 0);
L621E:
    /* 621E  idiv    dword ptr ds:[0CFC0h] */
    if (asm_idiv32(rd(pDS, 0xCFC0)) && (c = asm_divfault(0x065C, 0x621E, 5)) != 0) return c;
L6223:
    /* 6223  mov     dword ptr ds:[0CF98h],eax */
    wd(pDS, 0xCF98, EAX);
L6227: /* L6227 */
    /* 6227  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L6228:
    /* 6228  neg     ax */
    AX = neg16(AX);
L622A:
    /* 622A  je      short L622F */
    if (ZF) goto L622F;
L622C:
    /* 622C  cwde */
    EAX = (uint32_t)(int16_t)AX;
L622E:
    /* 622E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L622F: /* L622F */
    /* 622F  mov     eax,10000h */
    EAX = 0x10000;
L6235:
    /* 6235  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
