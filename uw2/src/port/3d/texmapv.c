/* texmapv.c: replaces src/3d/TEXMAPV.ASM (seg004_0849_6240, 6240..6D1B of its
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

uint32_t asm_mod_TEXMAPV(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x6240: goto L6240;
    case 0x6243: goto L6243;
    case 0x6244: goto L6244;
    case 0x6246: goto L6246;
    case 0x6249: goto L6249;
    case 0x624A: goto L624A;
    case 0x624D: goto L624D;
    case 0x624F: goto L624F;
    case 0x6252: goto L6252;
    case 0x6253: goto L6253;
    case 0x6256: goto L6256;
    case 0x6259: goto L6259;
    case 0x625B: goto L625B;
    case 0x625E: goto L625E;
    case 0x6261: goto L6261;
    case 0x6263: goto L6263;
    case 0x6266: goto L6266;
    case 0x6267: goto L6267;
    case 0x626B: goto L626B;
    case 0x626F: goto L626F;
    case 0x6273: goto L6273;
    case 0x6277: goto L6277;
    case 0x627D: goto L627D;
    case 0x6281: goto L6281;
    case 0x6283: goto L6283;
    case 0x6289: goto L6289;
    case 0x628B: goto L628B;
    case 0x6291: goto L6291;
    case 0x6295: goto L6295;
    case 0x6299: goto L6299;
    case 0x629B: goto L629B;
    case 0x629F: goto L629F;
    case 0x62A3: goto L62A3;
    case 0x62A7: goto L62A7;
    case 0x62AB: goto L62AB;
    case 0x62AF: goto L62AF;
    case 0x62B3: goto L62B3;
    case 0x62B8: goto L62B8;
    case 0x62BA: goto L62BA;
    case 0x62BE: goto L62BE;
    case 0x62C2: goto L62C2;
    case 0x62C7: goto L62C7;
    case 0x62CC: goto L62CC;
    case 0x62CE: goto L62CE;
    case 0x62D2: goto L62D2;
    case 0x62D6: goto L62D6;
    case 0x62DA: goto L62DA;
    case 0x62DE: goto L62DE;
    case 0x62E2: goto L62E2;
    case 0x62E6: goto L62E6;
    case 0x62EA: goto L62EA;
    case 0x62EE: goto L62EE;
    case 0x62F1: goto L62F1;
    case 0x62F5: goto L62F5;
    case 0x62F9: goto L62F9;
    case 0x62FB: goto L62FB;
    case 0x62FE: goto L62FE;
    case 0x6302: goto L6302;
    case 0x6306: goto L6306;
    case 0x630A: goto L630A;
    case 0x630E: goto L630E;
    case 0x6312: goto L6312;
    case 0x6315: goto L6315;
    case 0x6319: goto L6319;
    case 0x631D: goto L631D;
    case 0x6321: goto L6321;
    case 0x6325: goto L6325;
    case 0x6327: goto L6327;
    case 0x632C: goto L632C;
    case 0x6330: goto L6330;
    case 0x6334: goto L6334;
    case 0x6338: goto L6338;
    case 0x633C: goto L633C;
    case 0x6340: goto L6340;
    case 0x6342: goto L6342;
    case 0x6347: goto L6347;
    case 0x634B: goto L634B;
    case 0x634F: goto L634F;
    case 0x6353: goto L6353;
    case 0x6357: goto L6357;
    case 0x6359: goto L6359;
    case 0x635D: goto L635D;
    case 0x6360: goto L6360;
    case 0x6364: goto L6364;
    case 0x6368: goto L6368;
    case 0x636C: goto L636C;
    case 0x636E: goto L636E;
    case 0x6372: goto L6372;
    case 0x6375: goto L6375;
    case 0x6379: goto L6379;
    case 0x637D: goto L637D;
    case 0x6381: goto L6381;
    case 0x6383: goto L6383;
    case 0x6388: goto L6388;
    case 0x638C: goto L638C;
    case 0x6390: goto L6390;
    case 0x6394: goto L6394;
    case 0x6398: goto L6398;
    case 0x639A: goto L639A;
    case 0x639E: goto L639E;
    case 0x63A1: goto L63A1;
    case 0x63A5: goto L63A5;
    case 0x63A9: goto L63A9;
    case 0x63AD: goto L63AD;
    case 0x63AF: goto L63AF;
    case 0x63B3: goto L63B3;
    case 0x63B6: goto L63B6;
    case 0x63BA: goto L63BA;
    case 0x63BE: goto L63BE;
    case 0x63C2: goto L63C2;
    case 0x63C4: goto L63C4;
    case 0x63C9: goto L63C9;
    case 0x63CD: goto L63CD;
    case 0x63D3: goto L63D3;
    case 0x63D7: goto L63D7;
    case 0x63D9: goto L63D9;
    case 0x63DF: goto L63DF;
    case 0x63E1: goto L63E1;
    case 0x63E7: goto L63E7;
    case 0x63EB: goto L63EB;
    case 0x63EF: goto L63EF;
    case 0x63F1: goto L63F1;
    case 0x63F5: goto L63F5;
    case 0x63F8: goto L63F8;
    case 0x63FE: goto L63FE;
    case 0x6402: goto L6402;
    case 0x6404: goto L6404;
    case 0x640A: goto L640A;
    case 0x640C: goto L640C;
    case 0x6412: goto L6412;
    case 0x6416: goto L6416;
    case 0x641A: goto L641A;
    case 0x641C: goto L641C;
    case 0x6420: goto L6420;
    case 0x6423: goto L6423;
    case 0x6427: goto L6427;
    case 0x642B: goto L642B;
    case 0x642D: goto L642D;
    case 0x6432: goto L6432;
    case 0x6436: goto L6436;
    case 0x6437: goto L6437;
    case 0x643A: goto L643A;
    case 0x643B: goto L643B;
    case 0x643E: goto L643E;
    case 0x643F: goto L643F;
    case 0x6441: goto L6441;
    case 0x6444: goto L6444;
    case 0x6445: goto L6445;
    case 0x6447: goto L6447;
    case 0x644A: goto L644A;
    case 0x644D: goto L644D;
    case 0x644E: goto L644E;
    case 0x6451: goto L6451;
    case 0x6453: goto L6453;
    case 0x6454: goto L6454;
    case 0x6457: goto L6457;
    case 0x6458: goto L6458;
    case 0x645A: goto L645A;
    case 0x645D: goto L645D;
    case 0x645E: goto L645E;
    case 0x6461: goto L6461;
    case 0x6464: goto L6464;
    case 0x6465: goto L6465;
    case 0x6468: goto L6468;
    case 0x646C: goto L646C;
    case 0x6470: goto L6470;
    case 0x6474: goto L6474;
    case 0x647A: goto L647A;
    case 0x647E: goto L647E;
    case 0x6480: goto L6480;
    case 0x6486: goto L6486;
    case 0x6488: goto L6488;
    case 0x648E: goto L648E;
    case 0x6492: goto L6492;
    case 0x6496: goto L6496;
    case 0x6498: goto L6498;
    case 0x649C: goto L649C;
    case 0x64A0: goto L64A0;
    case 0x64A4: goto L64A4;
    case 0x64A8: goto L64A8;
    case 0x64AC: goto L64AC;
    case 0x64B0: goto L64B0;
    case 0x64B5: goto L64B5;
    case 0x64B7: goto L64B7;
    case 0x64BB: goto L64BB;
    case 0x64BF: goto L64BF;
    case 0x64C4: goto L64C4;
    case 0x64C9: goto L64C9;
    case 0x64CB: goto L64CB;
    case 0x64CF: goto L64CF;
    case 0x64D3: goto L64D3;
    case 0x64D7: goto L64D7;
    case 0x64DB: goto L64DB;
    case 0x64DF: goto L64DF;
    case 0x64E3: goto L64E3;
    case 0x64E7: goto L64E7;
    case 0x64EB: goto L64EB;
    case 0x64EE: goto L64EE;
    case 0x64F2: goto L64F2;
    case 0x64F6: goto L64F6;
    case 0x64F8: goto L64F8;
    case 0x64FB: goto L64FB;
    case 0x64FF: goto L64FF;
    case 0x6503: goto L6503;
    case 0x6507: goto L6507;
    case 0x650B: goto L650B;
    case 0x650F: goto L650F;
    case 0x6512: goto L6512;
    case 0x6516: goto L6516;
    case 0x651A: goto L651A;
    case 0x651E: goto L651E;
    case 0x6522: goto L6522;
    case 0x6524: goto L6524;
    case 0x6529: goto L6529;
    case 0x652D: goto L652D;
    case 0x6531: goto L6531;
    case 0x6535: goto L6535;
    case 0x6539: goto L6539;
    case 0x653D: goto L653D;
    case 0x653F: goto L653F;
    case 0x6544: goto L6544;
    case 0x6548: goto L6548;
    case 0x654C: goto L654C;
    case 0x6550: goto L6550;
    case 0x6554: goto L6554;
    case 0x6556: goto L6556;
    case 0x655A: goto L655A;
    case 0x655D: goto L655D;
    case 0x6561: goto L6561;
    case 0x6565: goto L6565;
    case 0x6569: goto L6569;
    case 0x656B: goto L656B;
    case 0x656F: goto L656F;
    case 0x6572: goto L6572;
    case 0x6576: goto L6576;
    case 0x657A: goto L657A;
    case 0x657E: goto L657E;
    case 0x6580: goto L6580;
    case 0x6585: goto L6585;
    case 0x6589: goto L6589;
    case 0x658D: goto L658D;
    case 0x658F: goto L658F;
    case 0x6594: goto L6594;
    case 0x6598: goto L6598;
    case 0x659C: goto L659C;
    case 0x659F: goto L659F;
    case 0x65A3: goto L65A3;
    case 0x65A5: goto L65A5;
    case 0x65AA: goto L65AA;
    case 0x65AE: goto L65AE;
    case 0x65B2: goto L65B2;
    case 0x65B5: goto L65B5;
    case 0x65B9: goto L65B9;
    case 0x65BD: goto L65BD;
    case 0x65BF: goto L65BF;
    case 0x65C4: goto L65C4;
    case 0x65C8: goto L65C8;
    case 0x65CE: goto L65CE;
    case 0x65D2: goto L65D2;
    case 0x65D4: goto L65D4;
    case 0x65DA: goto L65DA;
    case 0x65DC: goto L65DC;
    case 0x65E2: goto L65E2;
    case 0x65E6: goto L65E6;
    case 0x65EA: goto L65EA;
    case 0x65EC: goto L65EC;
    case 0x65F0: goto L65F0;
    case 0x65F3: goto L65F3;
    case 0x65F9: goto L65F9;
    case 0x65FD: goto L65FD;
    case 0x65FF: goto L65FF;
    case 0x6605: goto L6605;
    case 0x6607: goto L6607;
    case 0x660D: goto L660D;
    case 0x6611: goto L6611;
    case 0x6615: goto L6615;
    case 0x6617: goto L6617;
    case 0x661B: goto L661B;
    case 0x661E: goto L661E;
    case 0x6622: goto L6622;
    case 0x6626: goto L6626;
    case 0x6628: goto L6628;
    case 0x662D: goto L662D;
    case 0x6631: goto L6631;
    case 0x6632: goto L6632;
    case 0x663B: goto L663B;
    case 0x6644: goto L6644;
    case 0x6647: goto L6647;
    case 0x6648: goto L6648;
    case 0x664A: goto L664A;
    case 0x664E: goto L664E;
    case 0x6653: goto L6653;
    case 0x6655: goto L6655;
    case 0x6659: goto L6659;
    case 0x665D: goto L665D;
    case 0x6662: goto L6662;
    case 0x6664: goto L6664;
    case 0x6668: goto L6668;
    case 0x6669: goto L6669;
    case 0x666C: goto L666C;
    case 0x666F: goto L666F;
    case 0x6670: goto L6670;
    case 0x6672: goto L6672;
    case 0x6675: goto L6675;
    case 0x6678: goto L6678;
    case 0x667B: goto L667B;
    case 0x6680: goto L6680;
    case 0x6683: goto L6683;
    case 0x6684: goto L6684;
    case 0x6688: goto L6688;
    case 0x668C: goto L668C;
    case 0x6690: goto L6690;
    case 0x6694: goto L6694;
    case 0x6695: goto L6695;
    case 0x6698: goto L6698;
    case 0x6699: goto L6699;
    case 0x669B: goto L669B;
    case 0x669D: goto L669D;
    case 0x669F: goto L669F;
    case 0x66A2: goto L66A2;
    case 0x66A5: goto L66A5;
    case 0x66A6: goto L66A6;
    case 0x66AA: goto L66AA;
    case 0x66AE: goto L66AE;
    case 0x66B2: goto L66B2;
    case 0x66B4: goto L66B4;
    case 0x66B6: goto L66B6;
    case 0x66BA: goto L66BA;
    case 0x66BE: goto L66BE;
    case 0x66BF: goto L66BF;
    case 0x66C1: goto L66C1;
    case 0x66C4: goto L66C4;
    case 0x66C5: goto L66C5;
    case 0x66C8: goto L66C8;
    case 0x66CB: goto L66CB;
    case 0x66CC: goto L66CC;
    case 0x66D0: goto L66D0;
    case 0x66D4: goto L66D4;
    case 0x66D8: goto L66D8;
    case 0x66DA: goto L66DA;
    case 0x66DC: goto L66DC;
    case 0x66DF: goto L66DF;
    case 0x66E2: goto L66E2;
    case 0x66E6: goto L66E6;
    case 0x66E8: goto L66E8;
    case 0x66EB: goto L66EB;
    case 0x66EF: goto L66EF;
    case 0x66F3: goto L66F3;
    case 0x66F5: goto L66F5;
    case 0x66F9: goto L66F9;
    case 0x66FD: goto L66FD;
    case 0x66FF: goto L66FF;
    case 0x6701: goto L6701;
    case 0x6703: goto L6703;
    case 0x6705: goto L6705;
    case 0x6707: goto L6707;
    case 0x670A: goto L670A;
    case 0x670D: goto L670D;
    case 0x6711: goto L6711;
    case 0x6715: goto L6715;
    case 0x6717: goto L6717;
    case 0x6719: goto L6719;
    case 0x671C: goto L671C;
    case 0x671F: goto L671F;
    case 0x6720: goto L6720;
    case 0x6721: goto L6721;
    case 0x6723: goto L6723;
    case 0x6724: goto L6724;
    case 0x6725: goto L6725;
    case 0x6726: goto L6726;
    case 0x6729: goto L6729;
    case 0x672B: goto L672B;
    case 0x6730: goto L6730;
    case 0x6732: goto L6732;
    case 0x6735: goto L6735;
    case 0x6738: goto L6738;
    case 0x673A: goto L673A;
    case 0x673C: goto L673C;
    case 0x673D: goto L673D;
    case 0x673E: goto L673E;
    case 0x6740: goto L6740;
    case 0x6741: goto L6741;
    case 0x6744: goto L6744;
    case 0x6747: goto L6747;
    case 0x6749: goto L6749;
    case 0x674C: goto L674C;
    case 0x674F: goto L674F;
    case 0x6755: goto L6755;
    case 0x6758: goto L6758;
    case 0x675B: goto L675B;
    case 0x675E: goto L675E;
    case 0x6764: goto L6764;
    case 0x6767: goto L6767;
    case 0x676A: goto L676A;
    case 0x676D: goto L676D;
    case 0x6770: goto L6770;
    case 0x6773: goto L6773;
    case 0x6776: goto L6776;
    case 0x677A: goto L677A;
    case 0x677B: goto L677B;
    case 0x677D: goto L677D;
    case 0x6780: goto L6780;
    case 0x6783: goto L6783;
    case 0x6786: goto L6786;
    case 0x678A: goto L678A;
    case 0x678E: goto L678E;
    case 0x6792: goto L6792;
    case 0x6796: goto L6796;
    case 0x6798: goto L6798;
    case 0x679B: goto L679B;
    case 0x679F: goto L679F;
    case 0x67A1: goto L67A1;
    case 0x67A4: goto L67A4;
    case 0x67A8: goto L67A8;
    case 0x67AC: goto L67AC;
    case 0x67AD: goto L67AD;
    case 0x67B0: goto L67B0;
    case 0x67B2: goto L67B2;
    case 0x67B4: goto L67B4;
    case 0x67B7: goto L67B7;
    case 0x67BA: goto L67BA;
    case 0x67BE: goto L67BE;
    case 0x67C2: goto L67C2;
    case 0x67C6: goto L67C6;
    case 0x67CA: goto L67CA;
    case 0x67CC: goto L67CC;
    case 0x67CF: goto L67CF;
    case 0x67D3: goto L67D3;
    case 0x67D6: goto L67D6;
    case 0x67D9: goto L67D9;
    case 0x67DB: goto L67DB;
    case 0x67E1: goto L67E1;
    case 0x67E6: goto L67E6;
    case 0x67E8: goto L67E8;
    case 0x67E9: goto L67E9;
    case 0x67EA: goto L67EA;
    case 0x67ED: goto L67ED;
    case 0x67F1: goto L67F1;
    case 0x67F6: goto L67F6;
    case 0x67FA: goto L67FA;
    case 0x67FF: goto L67FF;
    case 0x6803: goto L6803;
    case 0x6808: goto L6808;
    case 0x680C: goto L680C;
    case 0x6811: goto L6811;
    case 0x6815: goto L6815;
    case 0x681A: goto L681A;
    case 0x681E: goto L681E;
    case 0x6823: goto L6823;
    case 0x6827: goto L6827;
    case 0x682C: goto L682C;
    case 0x6830: goto L6830;
    case 0x6835: goto L6835;
    case 0x6839: goto L6839;
    case 0x683E: goto L683E;
    case 0x6842: goto L6842;
    case 0x6847: goto L6847;
    case 0x684B: goto L684B;
    case 0x684E: goto L684E;
    case 0x6852: goto L6852;
    case 0x6856: goto L6856;
    case 0x6859: goto L6859;
    case 0x685F: goto L685F;
    case 0x6861: goto L6861;
    case 0x6862: goto L6862;
    case 0x6863: goto L6863;
    case 0x6864: goto L6864;
    case 0x6865: goto L6865;
    case 0x6866: goto L6866;
    case 0x686C: goto L686C;
    case 0x686E: goto L686E;
    case 0x6877: goto L6877;
    case 0x687D: goto L687D;
    case 0x687F: goto L687F;
    case 0x6888: goto L6888;
    case 0x688B: goto L688B;
    case 0x688D: goto L688D;
    case 0x6891: goto L6891;
    case 0x6893: goto L6893;
    case 0x6896: goto L6896;
    case 0x689A: goto L689A;
    case 0x689E: goto L689E;
    case 0x68A0: goto L68A0;
    case 0x68A1: goto L68A1;
    case 0x68A4: goto L68A4;
    case 0x68A7: goto L68A7;
    case 0x68A9: goto L68A9;
    case 0x68AD: goto L68AD;
    case 0x68AF: goto L68AF;
    case 0x68B0: goto L68B0;
    case 0x68B1: goto L68B1;
    case 0x68B4: goto L68B4;
    case 0x68B5: goto L68B5;
    case 0x68B9: goto L68B9;
    case 0x68BD: goto L68BD;
    case 0x68C1: goto L68C1;
    case 0x68C5: goto L68C5;
    case 0x68C9: goto L68C9;
    case 0x68CD: goto L68CD;
    case 0x68D0: goto L68D0;
    case 0x68D4: goto L68D4;
    case 0x68D8: goto L68D8;
    case 0x68DC: goto L68DC;
    case 0x68DE: goto L68DE;
    case 0x68E3: goto L68E3;
    case 0x68E7: goto L68E7;
    case 0x68EB: goto L68EB;
    case 0x68EF: goto L68EF;
    case 0x68F3: goto L68F3;
    case 0x68F5: goto L68F5;
    case 0x68FA: goto L68FA;
    case 0x68FE: goto L68FE;
    case 0x6902: goto L6902;
    case 0x6906: goto L6906;
    case 0x690A: goto L690A;
    case 0x690C: goto L690C;
    case 0x6911: goto L6911;
    case 0x6915: goto L6915;
    case 0x6919: goto L6919;
    case 0x691D: goto L691D;
    case 0x6921: goto L6921;
    case 0x6923: goto L6923;
    case 0x6928: goto L6928;
    case 0x692C: goto L692C;
    case 0x6930: goto L6930;
    case 0x6934: goto L6934;
    case 0x6938: goto L6938;
    case 0x693C: goto L693C;
    case 0x6940: goto L6940;
    case 0x6944: goto L6944;
    case 0x6948: goto L6948;
    case 0x694C: goto L694C;
    case 0x694D: goto L694D;
    case 0x6950: goto L6950;
    case 0x6951: goto L6951;
    case 0x6952: goto L6952;
    case 0x6955: goto L6955;
    case 0x6959: goto L6959;
    case 0x695C: goto L695C;
    case 0x695F: goto L695F;
    case 0x6962: goto L6962;
    case 0x6963: goto L6963;
    case 0x6965: goto L6965;
    case 0x6968: goto L6968;
    case 0x696B: goto L696B;
    case 0x696E: goto L696E;
    case 0x6971: goto L6971;
    case 0x6974: goto L6974;
    case 0x6978: goto L6978;
    case 0x697B: goto L697B;
    case 0x697E: goto L697E;
    case 0x6981: goto L6981;
    case 0x6982: goto L6982;
    case 0x6985: goto L6985;
    case 0x6987: goto L6987;
    case 0x6989: goto L6989;
    case 0x698C: goto L698C;
    case 0x698F: goto L698F;
    case 0x6992: goto L6992;
    case 0x6996: goto L6996;
    case 0x699A: goto L699A;
    case 0x699E: goto L699E;
    case 0x69A2: goto L69A2;
    case 0x69A6: goto L69A6;
    case 0x69AA: goto L69AA;
    case 0x69AE: goto L69AE;
    case 0x69B2: goto L69B2;
    case 0x69B4: goto L69B4;
    case 0x69B6: goto L69B6;
    case 0x69B7: goto L69B7;
    case 0x69BB: goto L69BB;
    case 0x69BF: goto L69BF;
    case 0x69C3: goto L69C3;
    case 0x69C7: goto L69C7;
    case 0x69CB: goto L69CB;
    case 0x69CF: goto L69CF;
    case 0x69D3: goto L69D3;
    case 0x69D7: goto L69D7;
    case 0x69DB: goto L69DB;
    case 0x69DF: goto L69DF;
    case 0x69E1: goto L69E1;
    case 0x69E3: goto L69E3;
    case 0x69E4: goto L69E4;
    case 0x69E8: goto L69E8;
    case 0x69EC: goto L69EC;
    case 0x69F0: goto L69F0;
    case 0x69F4: goto L69F4;
    case 0x69F9: goto L69F9;
    case 0x69FD: goto L69FD;
    case 0x6A01: goto L6A01;
    case 0x6A04: goto L6A04;
    case 0x6A08: goto L6A08;
    case 0x6A0C: goto L6A0C;
    case 0x6A0E: goto L6A0E;
    case 0x6A13: goto L6A13;
    case 0x6A17: goto L6A17;
    case 0x6A1B: goto L6A1B;
    case 0x6A1F: goto L6A1F;
    case 0x6A24: goto L6A24;
    case 0x6A28: goto L6A28;
    case 0x6A2C: goto L6A2C;
    case 0x6A2F: goto L6A2F;
    case 0x6A33: goto L6A33;
    case 0x6A37: goto L6A37;
    case 0x6A39: goto L6A39;
    case 0x6A3E: goto L6A3E;
    case 0x6A42: goto L6A42;
    case 0x6A46: goto L6A46;
    case 0x6A4A: goto L6A4A;
    case 0x6A4E: goto L6A4E;
    case 0x6A52: goto L6A52;
    case 0x6A58: goto L6A58;
    case 0x6A5C: goto L6A5C;
    case 0x6A5E: goto L6A5E;
    case 0x6A64: goto L6A64;
    case 0x6A66: goto L6A66;
    case 0x6A6C: goto L6A6C;
    case 0x6A70: goto L6A70;
    case 0x6A74: goto L6A74;
    case 0x6A76: goto L6A76;
    case 0x6A7A: goto L6A7A;
    case 0x6A7E: goto L6A7E;
    case 0x6A82: goto L6A82;
    case 0x6A87: goto L6A87;
    case 0x6A89: goto L6A89;
    case 0x6A8D: goto L6A8D;
    case 0x6A91: goto L6A91;
    case 0x6A96: goto L6A96;
    case 0x6A9B: goto L6A9B;
    case 0x6A9D: goto L6A9D;
    case 0x6AA1: goto L6AA1;
    case 0x6AA7: goto L6AA7;
    case 0x6AAB: goto L6AAB;
    case 0x6AAD: goto L6AAD;
    case 0x6AB3: goto L6AB3;
    case 0x6AB5: goto L6AB5;
    case 0x6ABB: goto L6ABB;
    case 0x6ABF: goto L6ABF;
    case 0x6AC3: goto L6AC3;
    case 0x6AC5: goto L6AC5;
    case 0x6AC9: goto L6AC9;
    case 0x6ACC: goto L6ACC;
    case 0x6AD2: goto L6AD2;
    case 0x6AD6: goto L6AD6;
    case 0x6AD8: goto L6AD8;
    case 0x6ADE: goto L6ADE;
    case 0x6AE0: goto L6AE0;
    case 0x6AE6: goto L6AE6;
    case 0x6AEA: goto L6AEA;
    case 0x6AEE: goto L6AEE;
    case 0x6AF0: goto L6AF0;
    case 0x6AF4: goto L6AF4;
    case 0x6AF7: goto L6AF7;
    case 0x6AFB: goto L6AFB;
    case 0x6AFF: goto L6AFF;
    case 0x6B01: goto L6B01;
    case 0x6B06: goto L6B06;
    case 0x6B0A: goto L6B0A;
    case 0x6B0E: goto L6B0E;
    case 0x6B12: goto L6B12;
    case 0x6B16: goto L6B16;
    case 0x6B1A: goto L6B1A;
    case 0x6B1C: goto L6B1C;
    case 0x6B21: goto L6B21;
    case 0x6B25: goto L6B25;
    case 0x6B29: goto L6B29;
    case 0x6B2D: goto L6B2D;
    case 0x6B31: goto L6B31;
    case 0x6B33: goto L6B33;
    case 0x6B37: goto L6B37;
    case 0x6B3A: goto L6B3A;
    case 0x6B3E: goto L6B3E;
    case 0x6B42: goto L6B42;
    case 0x6B46: goto L6B46;
    case 0x6B48: goto L6B48;
    case 0x6B4C: goto L6B4C;
    case 0x6B4F: goto L6B4F;
    case 0x6B53: goto L6B53;
    case 0x6B57: goto L6B57;
    case 0x6B5B: goto L6B5B;
    case 0x6B5D: goto L6B5D;
    case 0x6B62: goto L6B62;
    case 0x6B66: goto L6B66;
    case 0x6B6A: goto L6B6A;
    case 0x6B6E: goto L6B6E;
    case 0x6B72: goto L6B72;
    case 0x6B74: goto L6B74;
    case 0x6B78: goto L6B78;
    case 0x6B7B: goto L6B7B;
    case 0x6B7F: goto L6B7F;
    case 0x6B83: goto L6B83;
    case 0x6B87: goto L6B87;
    case 0x6B89: goto L6B89;
    case 0x6B8D: goto L6B8D;
    case 0x6B90: goto L6B90;
    case 0x6B94: goto L6B94;
    case 0x6B98: goto L6B98;
    case 0x6B9C: goto L6B9C;
    case 0x6B9E: goto L6B9E;
    case 0x6BA3: goto L6BA3;
    case 0x6BA7: goto L6BA7;
    case 0x6BAB: goto L6BAB;
    case 0x6BAF: goto L6BAF;
    case 0x6BB3: goto L6BB3;
    case 0x6BB9: goto L6BB9;
    case 0x6BBD: goto L6BBD;
    case 0x6BBF: goto L6BBF;
    case 0x6BC5: goto L6BC5;
    case 0x6BC7: goto L6BC7;
    case 0x6BCD: goto L6BCD;
    case 0x6BD1: goto L6BD1;
    case 0x6BD5: goto L6BD5;
    case 0x6BD7: goto L6BD7;
    case 0x6BDB: goto L6BDB;
    case 0x6BDF: goto L6BDF;
    case 0x6BE3: goto L6BE3;
    case 0x6BE8: goto L6BE8;
    case 0x6BEA: goto L6BEA;
    case 0x6BEE: goto L6BEE;
    case 0x6BF2: goto L6BF2;
    case 0x6BF7: goto L6BF7;
    case 0x6BFC: goto L6BFC;
    case 0x6BFE: goto L6BFE;
    case 0x6C02: goto L6C02;
    case 0x6C06: goto L6C06;
    case 0x6C0C: goto L6C0C;
    case 0x6C10: goto L6C10;
    case 0x6C12: goto L6C12;
    case 0x6C18: goto L6C18;
    case 0x6C1A: goto L6C1A;
    case 0x6C20: goto L6C20;
    case 0x6C24: goto L6C24;
    case 0x6C28: goto L6C28;
    case 0x6C2A: goto L6C2A;
    case 0x6C2E: goto L6C2E;
    case 0x6C31: goto L6C31;
    case 0x6C37: goto L6C37;
    case 0x6C3B: goto L6C3B;
    case 0x6C3D: goto L6C3D;
    case 0x6C43: goto L6C43;
    case 0x6C45: goto L6C45;
    case 0x6C4B: goto L6C4B;
    case 0x6C4F: goto L6C4F;
    case 0x6C53: goto L6C53;
    case 0x6C55: goto L6C55;
    case 0x6C59: goto L6C59;
    case 0x6C5C: goto L6C5C;
    case 0x6C60: goto L6C60;
    case 0x6C64: goto L6C64;
    case 0x6C66: goto L6C66;
    case 0x6C6B: goto L6C6B;
    case 0x6C6F: goto L6C6F;
    case 0x6C73: goto L6C73;
    case 0x6C77: goto L6C77;
    case 0x6C7B: goto L6C7B;
    case 0x6C7F: goto L6C7F;
    case 0x6C81: goto L6C81;
    case 0x6C86: goto L6C86;
    case 0x6C8A: goto L6C8A;
    case 0x6C8E: goto L6C8E;
    case 0x6C92: goto L6C92;
    case 0x6C96: goto L6C96;
    case 0x6C98: goto L6C98;
    case 0x6C9C: goto L6C9C;
    case 0x6C9F: goto L6C9F;
    case 0x6CA3: goto L6CA3;
    case 0x6CA7: goto L6CA7;
    case 0x6CAB: goto L6CAB;
    case 0x6CAD: goto L6CAD;
    case 0x6CB1: goto L6CB1;
    case 0x6CB4: goto L6CB4;
    case 0x6CB8: goto L6CB8;
    case 0x6CBC: goto L6CBC;
    case 0x6CC0: goto L6CC0;
    case 0x6CC2: goto L6CC2;
    case 0x6CC7: goto L6CC7;
    case 0x6CCB: goto L6CCB;
    case 0x6CCF: goto L6CCF;
    case 0x6CD3: goto L6CD3;
    case 0x6CD7: goto L6CD7;
    case 0x6CD9: goto L6CD9;
    case 0x6CDD: goto L6CDD;
    case 0x6CE0: goto L6CE0;
    case 0x6CE4: goto L6CE4;
    case 0x6CE8: goto L6CE8;
    case 0x6CEC: goto L6CEC;
    case 0x6CEE: goto L6CEE;
    case 0x6CF2: goto L6CF2;
    case 0x6CF5: goto L6CF5;
    case 0x6CF9: goto L6CF9;
    case 0x6CFD: goto L6CFD;
    case 0x6D01: goto L6D01;
    case 0x6D03: goto L6D03;
    case 0x6D08: goto L6D08;
    case 0x6D0C: goto L6D0C;
    case 0x6D0D: goto L6D0D;
    case 0x6D0F: goto L6D0F;
    case 0x6D11: goto L6D11;
    case 0x6D13: goto L6D13;
    case 0x6D14: goto L6D14;
    case 0x6D1A: goto L6D1A;
    default: asm_bad_entry("TEXMAPV.ASM", entry);
    }

    /* seg004_0849_6240  (+6240)
       seg004_0849_6240: step the edge that walks the vertices backwards (index - 1, wrapping) to its
       next vertex: its starting y (D010), w, l, u * w and v * w, and its steps per column (CF84 ..
       CFA4), each difference divided by the edge's width in columns. A vertex at z <= 1/256 sets
       CF44 and the polygon is abandoned. In: BP = _asm_texture_map_v's frame. */
L6240: /* _seg004_0849_6240 */
    /* 6240  mov     ax,word ptr ds:[0CF52h] */
    AX = rw(pDS, 0xCF52);
L6243:
    /* 6243  dec     ax */
    AX = dec16(AX);
L6244:
    /* 6244  jns     short L624A */
    if (!SF) goto L624A;
L6246:
    /* 6246  add     ax,word ptr [bp+8] */
    AX = (uint16_t)(AX + rw(pSS, BP + 0x8));
L6249:
    /* 6249  nop */
    ;
L624A: /* L624A */
    /* 624A  imul    ax,20h */
    AX = imul16x(AX, 0x20);
L624D:
    /* 624D  mov     bx,ax */
    BX = AX;
L624F:
    /* 624F  add     ax,word ptr [bp+6] */
    AX = (uint16_t)(AX + rw(pSS, BP + 0x6));
L6252:
    /* 6252  nop */
    ;
L6253:
    /* 6253  mov     word ptr ds:[0D01Ch],ax */
    ww(pDS, 0xD01C, AX);
L6256:
    /* 6256  sub     bx,20h */
    BX = sub16(BX, 0x20, 0);
L6259:
    /* 6259  jns     short L6263 */
    if (!SF) goto L6263;
L625B:
    /* 625B  mov     ax,word ptr [bp+8] */
    AX = rw(pSS, BP + 0x8);
L625E:
    /* 625E  imul    ax,20h */
    AX = imul16x(AX, 0x20);
L6261:
    /* 6261  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L6263: /* L6263 */
    /* 6263  add     bx,word ptr [bp+6] */
    BX = (uint16_t)(BX + rw(pSS, BP + 0x6));
L6266:
    /* 6266  nop */
    ;
L6267:
    /* 6267  mov     word ptr ds:[0D01Eh],bx */
    ww(pDS, 0xD01E, BX);
L626B:
    /* 626B  mov     bx,word ptr ds:[0D01Ch] */
    BX = rw(pDS, 0xD01C);
L626F:
    /* 626F  mov     eax,dword ptr [bx+10h] */
    EAX = rd(pDS, BX + 0x10);
L6273:
    /* 6273  mov     dword ptr ds:[0D010h],eax */
    wd(pDS, 0xD010, EAX);
L6277:
    /* 6277  mov     eax,100h */
    EAX = 0x100;
L627D:
    /* 627D  cmp     dword ptr [bx+8],eax */
    sub32(rd(pDS, BX + 0x8), EAX, 0);
L6281:
    /* 6281  ja      short L628B */
    if (!CF && !ZF) goto L628B;
L6283:
    /* 6283  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L6289:
    /* 6289  jmp     short L629F */
    goto L629F;
L628B: /* L628B */
    /* 628B  mov     eax,800000h */
    EAX = 0x800000;
L6291:
    /* 6291  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6295:
    /* 6295  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6299:
    /* 6299  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L629B:
    /* 629B  idiv    dword ptr [bx+8] */
    if (asm_idiv32(rd(pDS, BX + 0x8)) && (c = asm_divfault(0x065C, 0x629B, 4)) != 0) return c;
L629F: /* L629F */
    /* 629F  mov     dword ptr ds:[0CF74h],eax */
    wd(pDS, 0xCF74, EAX);
L62A3:
    /* 62A3  mov     eax,dword ptr [bx+1Ch] */
    EAX = rd(pDS, BX + 0x1C);
L62A7:
    /* 62A7  mov     dword ptr ds:[0CF7Ch],eax */
    wd(pDS, 0xCF7C, EAX);
L62AB:
    /* 62AB  mov     eax,dword ptr ds:[0CF74h] */
    EAX = rd(pDS, 0xCF74);
L62AF:
    /* 62AF  imul    dword ptr [bx+14h] */
    imul32(rd(pDS, BX + 0x14));
L62B3:
    /* 62B3  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L62B8:
    /* 62B8  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L62BA:
    /* 62BA  mov     dword ptr ds:[0CF60h],eax */
    wd(pDS, 0xCF60, EAX);
L62BE:
    /* 62BE  mov     eax,dword ptr [bx+18h] */
    EAX = rd(pDS, BX + 0x18);
L62C2:
    /* 62C2  imul    dword ptr ds:[0CF74h] */
    imul32(rd(pDS, 0xCF74));
L62C7:
    /* 62C7  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L62CC:
    /* 62CC  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L62CE:
    /* 62CE  mov     dword ptr ds:[0CF64h],eax */
    wd(pDS, 0xCF64, EAX);
L62D2:
    /* 62D2  mov     si,word ptr ds:[0D01Eh] */
    SI = rw(pDS, 0xD01E);
L62D6:
    /* 62D6  mov     eax,dword ptr [si+0Ch] */
    EAX = rd(pDS, SI + 0xC);
L62DA:
    /* 62DA  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L62DE:
    /* 62DE  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L62E2:
    /* 62E2  mov     ecx,dword ptr [bx+0Ch] */
    ECX = rd(pDS, BX + 0xC);
L62E6:
    /* 62E6  add     ecx,41h */
    ECX = (uint32_t)(ECX + 0x41);
L62EA:
    /* 62EA  sar     ecx,10h */
    ECX = (uint32_t)((int32_t)ECX >> 16);
L62EE:
    /* 62EE  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L62F1:
    /* 62F1  shl     eax,10h */
    EAX = shl32(EAX, 16);
L62F5:
    /* 62F5  mov     dword ptr ds:[0D018h],eax */
    wd(pDS, 0xD018, EAX);
L62F9:
    /* 62F9  jne     short L6315 */
    if (!ZF) goto L6315;
L62FB:
    /* 62FB  sub     eax,eax */
    EAX = sub32(EAX, EAX, 0);
L62FE:
    /* 62FE  mov     dword ptr ds:[0CF84h],eax */
    wd(pDS, 0xCF84, EAX);
L6302:
    /* 6302  mov     dword ptr ds:[0CF88h],eax */
    wd(pDS, 0xCF88, EAX);
L6306:
    /* 6306  mov     dword ptr ds:[0CF8Ch],eax */
    wd(pDS, 0xCF8C, EAX);
L630A:
    /* 630A  mov     dword ptr ds:[0CF9Ch],eax */
    wd(pDS, 0xCF9C, EAX);
L630E:
    /* 630E  mov     dword ptr ds:[0CFA4h],eax */
    wd(pDS, 0xCFA4, EAX);
L6312:
    /* 6312  jmp     L6436 */
    goto L6436;
L6315: /* L6315 */
    /* 6315  mov     eax,dword ptr [si+1Ch] */
    EAX = rd(pDS, SI + 0x1C);
L6319:
    /* 6319  sub     eax,dword ptr [bx+1Ch] */
    EAX = (uint32_t)(EAX - rd(pDS, BX + 0x1C));
L631D:
    /* 631D  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6321:
    /* 6321  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6325:
    /* 6325  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6327:
    /* 6327  idiv    dword ptr ds:[0D018h] */
    if (asm_idiv32(rd(pDS, 0xD018)) && (c = asm_divfault(0x065C, 0x6327, 5)) != 0) return c;
L632C:
    /* 632C  mov     dword ptr ds:[0CFA4h],eax */
    wd(pDS, 0xCFA4, EAX);
L6330:
    /* 6330  mov     eax,dword ptr [si+10h] */
    EAX = rd(pDS, SI + 0x10);
L6334:
    /* 6334  sub     eax,dword ptr [bx+10h] */
    EAX = (uint32_t)(EAX - rd(pDS, BX + 0x10));
L6338:
    /* 6338  rol     eax,10h */
    EAX = rol32(EAX, 16);
L633C:
    /* 633C  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6340:
    /* 6340  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6342:
    /* 6342  idiv    dword ptr ds:[0D018h] */
    if (asm_idiv32(rd(pDS, 0xD018)) && (c = asm_divfault(0x065C, 0x6342, 5)) != 0) return c;
L6347:
    /* 6347  mov     dword ptr ds:[0CF84h],eax */
    wd(pDS, 0xCF84, EAX);
L634B:
    /* 634B  mov     eax,dword ptr [bx+14h] */
    EAX = rd(pDS, BX + 0x14);
L634F:
    /* 634F  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6353:
    /* 6353  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6357:
    /* 6357  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6359:
    /* 6359  idiv    dword ptr [bx+8] */
    if (asm_idiv32(rd(pDS, BX + 0x8)) && (c = asm_divfault(0x065C, 0x6359, 4)) != 0) return c;
L635D:
    /* 635D  mov     ecx,eax */
    ECX = EAX;
L6360:
    /* 6360  mov     eax,dword ptr [si+14h] */
    EAX = rd(pDS, SI + 0x14);
L6364:
    /* 6364  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6368:
    /* 6368  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L636C:
    /* 636C  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L636E:
    /* 636E  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x636E, 4)) != 0) return c;
L6372:
    /* 6372  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L6375:
    /* 6375  shl     eax,7 */
    EAX = (uint32_t)(EAX << 7);
L6379:
    /* 6379  rol     eax,10h */
    EAX = rol32(EAX, 16);
L637D:
    /* 637D  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6381:
    /* 6381  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6383:
    /* 6383  idiv    dword ptr ds:[0D018h] */
    if (asm_idiv32(rd(pDS, 0xD018)) && (c = asm_divfault(0x065C, 0x6383, 5)) != 0) return c;
L6388:
    /* 6388  mov     dword ptr ds:[0CF88h],eax */
    wd(pDS, 0xCF88, EAX);
L638C:
    /* 638C  mov     eax,dword ptr [bx+18h] */
    EAX = rd(pDS, BX + 0x18);
L6390:
    /* 6390  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6394:
    /* 6394  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6398:
    /* 6398  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L639A:
    /* 639A  idiv    dword ptr [bx+8] */
    if (asm_idiv32(rd(pDS, BX + 0x8)) && (c = asm_divfault(0x065C, 0x639A, 4)) != 0) return c;
L639E:
    /* 639E  mov     ecx,eax */
    ECX = EAX;
L63A1:
    /* 63A1  mov     eax,dword ptr [si+18h] */
    EAX = rd(pDS, SI + 0x18);
L63A5:
    /* 63A5  rol     eax,10h */
    EAX = rol32(EAX, 16);
L63A9:
    /* 63A9  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L63AD:
    /* 63AD  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L63AF:
    /* 63AF  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x63AF, 4)) != 0) return c;
L63B3:
    /* 63B3  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L63B6:
    /* 63B6  shl     eax,7 */
    EAX = (uint32_t)(EAX << 7);
L63BA:
    /* 63BA  rol     eax,10h */
    EAX = rol32(EAX, 16);
L63BE:
    /* 63BE  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L63C2:
    /* 63C2  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L63C4:
    /* 63C4  idiv    dword ptr ds:[0D018h] */
    if (asm_idiv32(rd(pDS, 0xD018)) && (c = asm_divfault(0x065C, 0x63C4, 5)) != 0) return c;
L63C9:
    /* 63C9  mov     dword ptr ds:[0CF8Ch],eax */
    wd(pDS, 0xCF8C, EAX);
L63CD:
    /* 63CD  mov     eax,100h */
    EAX = 0x100;
L63D3:
    /* 63D3  cmp     dword ptr [bx+8],eax */
    sub32(rd(pDS, BX + 0x8), EAX, 0);
L63D7:
    /* 63D7  ja      short L63E1 */
    if (!CF && !ZF) goto L63E1;
L63D9:
    /* 63D9  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L63DF:
    /* 63DF  jmp     short L63F5 */
    goto L63F5;
L63E1: /* L63E1 */
    /* 63E1  mov     eax,800000h */
    EAX = 0x800000;
L63E7:
    /* 63E7  rol     eax,10h */
    EAX = rol32(EAX, 16);
L63EB:
    /* 63EB  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L63EF:
    /* 63EF  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L63F1:
    /* 63F1  idiv    dword ptr [bx+8] */
    if (asm_idiv32(rd(pDS, BX + 0x8)) && (c = asm_divfault(0x065C, 0x63F1, 4)) != 0) return c;
L63F5: /* L63F5 */
    /* 63F5  mov     ecx,eax */
    ECX = EAX;
L63F8:
    /* 63F8  mov     eax,100h */
    EAX = 0x100;
L63FE:
    /* 63FE  cmp     dword ptr [si+8],eax */
    sub32(rd(pDS, SI + 0x8), EAX, 0);
L6402:
    /* 6402  ja      short L640C */
    if (!CF && !ZF) goto L640C;
L6404:
    /* 6404  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L640A:
    /* 640A  jmp     short L6420 */
    goto L6420;
L640C: /* L640C */
    /* 640C  mov     eax,800000h */
    EAX = 0x800000;
L6412:
    /* 6412  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6416:
    /* 6416  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L641A:
    /* 641A  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L641C:
    /* 641C  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x641C, 4)) != 0) return c;
L6420: /* L6420 */
    /* 6420  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L6423:
    /* 6423  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6427:
    /* 6427  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L642B:
    /* 642B  sub     ax,ax */
    AX = sub16(AX, AX, 0);
L642D:
    /* 642D  idiv    dword ptr ds:[0D018h] */
    if (asm_idiv32(rd(pDS, 0xD018)) && (c = asm_divfault(0x065C, 0x642D, 5)) != 0) return c;
L6432:
    /* 6432  mov     dword ptr ds:[0CF9Ch],eax */
    wd(pDS, 0xCF9C, EAX);
L6436: /* L6436 */
    /* 6436  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_6437  (+6437)
       seg004_0849_6437: as seg004_0849_6240 for the edge that walks forwards (index + 1, wrapping),
       into D014 and CF68 .. CF80, CF90 .. CFA8. */
L6437: /* _seg004_0849_6437 */
    /* 6437  mov     ax,word ptr ds:[0CF54h] */
    AX = rw(pDS, 0xCF54);
L643A:
    /* 643A  inc     ax */
    AX = (uint16_t)(AX + 1);
L643B:
    /* 643B  cmp     ax,word ptr [bp+8] */
    sub16(AX, rw(pSS, BP + 0x8), 0);
L643E:
    /* 643E  nop */
    ;
L643F:
    /* 643F  jb      short L6445 */
    if (CF) goto L6445;
L6441:
    /* 6441  sub     ax,word ptr [bp+8] */
    AX = (uint16_t)(AX - rw(pSS, BP + 0x8));
L6444:
    /* 6444  nop */
    ;
L6445: /* L6445 */
    /* 6445  mov     bx,ax */
    BX = AX;
L6447:
    /* 6447  imul    ax,20h */
    AX = imul16x(AX, 0x20);
L644A:
    /* 644A  add     ax,word ptr [bp+6] */
    AX = (uint16_t)(AX + rw(pSS, BP + 0x6));
L644D:
    /* 644D  nop */
    ;
L644E:
    /* 644E  mov     word ptr ds:[0D01Ch],ax */
    ww(pDS, 0xD01C, AX);
L6451:
    /* 6451  mov     ax,bx */
    AX = BX;
L6453:
    /* 6453  inc     ax */
    AX = (uint16_t)(AX + 1);
L6454:
    /* 6454  cmp     ax,word ptr [bp+8] */
    sub16(AX, rw(pSS, BP + 0x8), 0);
L6457:
    /* 6457  nop */
    ;
L6458:
    /* 6458  jb      short L645E */
    if (CF) goto L645E;
L645A:
    /* 645A  sub     ax,word ptr [bp+8] */
    AX = (uint16_t)(AX - rw(pSS, BP + 0x8));
L645D:
    /* 645D  nop */
    ;
L645E: /* L645E */
    /* 645E  imul    ax,20h */
    AX = imul16x(AX, 0x20);
L6461:
    /* 6461  add     ax,word ptr [bp+6] */
    AX = (uint16_t)(AX + rw(pSS, BP + 0x6));
L6464:
    /* 6464  nop */
    ;
L6465:
    /* 6465  mov     word ptr ds:[0D01Eh],ax */
    ww(pDS, 0xD01E, AX);
L6468:
    /* 6468  mov     bx,word ptr ds:[0D01Ch] */
    BX = rw(pDS, 0xD01C);
L646C:
    /* 646C  mov     eax,dword ptr [bx+10h] */
    EAX = rd(pDS, BX + 0x10);
L6470:
    /* 6470  mov     dword ptr ds:[0D014h],eax */
    wd(pDS, 0xD014, EAX);
L6474:
    /* 6474  mov     eax,100h */
    EAX = 0x100;
L647A:
    /* 647A  cmp     dword ptr [bx+8],eax */
    sub32(rd(pDS, BX + 0x8), EAX, 0);
L647E:
    /* 647E  ja      short L6488 */
    if (!CF && !ZF) goto L6488;
L6480:
    /* 6480  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L6486:
    /* 6486  jmp     short L649C */
    goto L649C;
L6488: /* L6488 */
    /* 6488  mov     eax,800000h */
    EAX = 0x800000;
L648E:
    /* 648E  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6492:
    /* 6492  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6496:
    /* 6496  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6498:
    /* 6498  idiv    dword ptr [bx+8] */
    if (asm_idiv32(rd(pDS, BX + 0x8)) && (c = asm_divfault(0x065C, 0x6498, 4)) != 0) return c;
L649C: /* L649C */
    /* 649C  mov     dword ptr ds:[0CF78h],eax */
    wd(pDS, 0xCF78, EAX);
L64A0:
    /* 64A0  mov     eax,dword ptr [bx+1Ch] */
    EAX = rd(pDS, BX + 0x1C);
L64A4:
    /* 64A4  mov     dword ptr ds:[0CF80h],eax */
    wd(pDS, 0xCF80, EAX);
L64A8:
    /* 64A8  mov     eax,dword ptr ds:[0CF78h] */
    EAX = rd(pDS, 0xCF78);
L64AC:
    /* 64AC  imul    dword ptr [bx+14h] */
    imul32(rd(pDS, BX + 0x14));
L64B0:
    /* 64B0  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L64B5:
    /* 64B5  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L64B7:
    /* 64B7  mov     dword ptr ds:[0CF6Ch],eax */
    wd(pDS, 0xCF6C, EAX);
L64BB:
    /* 64BB  mov     eax,dword ptr [bx+18h] */
    EAX = rd(pDS, BX + 0x18);
L64BF:
    /* 64BF  imul    dword ptr ds:[0CF78h] */
    imul32(rd(pDS, 0xCF78));
L64C4:
    /* 64C4  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L64C9:
    /* 64C9  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L64CB:
    /* 64CB  mov     dword ptr ds:[0CF70h],eax */
    wd(pDS, 0xCF70, EAX);
L64CF:
    /* 64CF  mov     si,word ptr ds:[0D01Eh] */
    SI = rw(pDS, 0xD01E);
L64D3:
    /* 64D3  mov     eax,dword ptr [si+0Ch] */
    EAX = rd(pDS, SI + 0xC);
L64D7:
    /* 64D7  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L64DB:
    /* 64DB  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L64DF:
    /* 64DF  mov     ecx,dword ptr [bx+0Ch] */
    ECX = rd(pDS, BX + 0xC);
L64E3:
    /* 64E3  add     ecx,41h */
    ECX = (uint32_t)(ECX + 0x41);
L64E7:
    /* 64E7  sar     ecx,10h */
    ECX = (uint32_t)((int32_t)ECX >> 16);
L64EB:
    /* 64EB  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L64EE:
    /* 64EE  shl     eax,10h */
    EAX = shl32(EAX, 16);
L64F2:
    /* 64F2  mov     dword ptr ds:[0D018h],eax */
    wd(pDS, 0xD018, EAX);
L64F6:
    /* 64F6  jne     short L6512 */
    if (!ZF) goto L6512;
L64F8:
    /* 64F8  sub     eax,eax */
    EAX = sub32(EAX, EAX, 0);
L64FB:
    /* 64FB  mov     dword ptr ds:[0CF90h],eax */
    wd(pDS, 0xCF90, EAX);
L64FF:
    /* 64FF  mov     dword ptr ds:[0CF94h],eax */
    wd(pDS, 0xCF94, EAX);
L6503:
    /* 6503  mov     dword ptr ds:[0CF98h],eax */
    wd(pDS, 0xCF98, EAX);
L6507:
    /* 6507  mov     dword ptr ds:[0CFA0h],eax */
    wd(pDS, 0xCFA0, EAX);
L650B:
    /* 650B  mov     dword ptr ds:[0CFA8h],eax */
    wd(pDS, 0xCFA8, EAX);
L650F:
    /* 650F  jmp     L6631 */
    goto L6631;
L6512: /* L6512 */
    /* 6512  mov     eax,dword ptr [si+1Ch] */
    EAX = rd(pDS, SI + 0x1C);
L6516:
    /* 6516  sub     eax,dword ptr [bx+1Ch] */
    EAX = (uint32_t)(EAX - rd(pDS, BX + 0x1C));
L651A:
    /* 651A  rol     eax,10h */
    EAX = rol32(EAX, 16);
L651E:
    /* 651E  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6522:
    /* 6522  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6524:
    /* 6524  idiv    dword ptr ds:[0D018h] */
    if (asm_idiv32(rd(pDS, 0xD018)) && (c = asm_divfault(0x065C, 0x6524, 5)) != 0) return c;
L6529:
    /* 6529  mov     dword ptr ds:[0CFA8h],eax */
    wd(pDS, 0xCFA8, EAX);
L652D:
    /* 652D  mov     eax,dword ptr [si+10h] */
    EAX = rd(pDS, SI + 0x10);
L6531:
    /* 6531  sub     eax,dword ptr [bx+10h] */
    EAX = (uint32_t)(EAX - rd(pDS, BX + 0x10));
L6535:
    /* 6535  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6539:
    /* 6539  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L653D:
    /* 653D  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L653F:
    /* 653F  idiv    dword ptr ds:[0D018h] */
    if (asm_idiv32(rd(pDS, 0xD018)) && (c = asm_divfault(0x065C, 0x653F, 5)) != 0) return c;
L6544:
    /* 6544  mov     dword ptr ds:[0CF90h],eax */
    wd(pDS, 0xCF90, EAX);
L6548:
    /* 6548  mov     eax,dword ptr [bx+14h] */
    EAX = rd(pDS, BX + 0x14);
L654C:
    /* 654C  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6550:
    /* 6550  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6554:
    /* 6554  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6556:
    /* 6556  idiv    dword ptr [bx+8] */
    if (asm_idiv32(rd(pDS, BX + 0x8)) && (c = asm_divfault(0x065C, 0x6556, 4)) != 0) return c;
L655A:
    /* 655A  mov     ecx,eax */
    ECX = EAX;
L655D:
    /* 655D  mov     eax,dword ptr [si+14h] */
    EAX = rd(pDS, SI + 0x14);
L6561:
    /* 6561  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6565:
    /* 6565  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6569:
    /* 6569  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L656B:
    /* 656B  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x656B, 4)) != 0) return c;
L656F:
    /* 656F  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L6572:
    /* 6572  shl     eax,7 */
    EAX = (uint32_t)(EAX << 7);
L6576:
    /* 6576  rol     eax,10h */
    EAX = rol32(EAX, 16);
L657A:
    /* 657A  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L657E:
    /* 657E  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6580:
    /* 6580  idiv    dword ptr ds:[0D018h] */
    if (asm_idiv32(rd(pDS, 0xD018)) && (c = asm_divfault(0x065C, 0x6580, 5)) != 0) return c;
L6585:
    /* 6585  mov     dword ptr ds:[0CF94h],eax */
    wd(pDS, 0xCF94, EAX);
L6589:
    /* 6589  mov     eax,dword ptr [bx+18h] */
    EAX = rd(pDS, BX + 0x18);
L658D:
    /* 658D  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L658F:
    /* 658F  shld    edx,eax,17h */
    EDX = shld32(EDX, EAX, 23);
L6594:
    /* 6594  shl     eax,17h */
    EAX = (uint32_t)(EAX << 23);
L6598:
    /* 6598  idiv    dword ptr [bx+8] */
    if (asm_idiv32(rd(pDS, BX + 0x8)) && (c = asm_divfault(0x065C, 0x6598, 4)) != 0) return c;
L659C:
    /* 659C  mov     ecx,eax */
    ECX = EAX;
L659F:
    /* 659F  mov     eax,dword ptr [si+18h] */
    EAX = rd(pDS, SI + 0x18);
L65A3:
    /* 65A3  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L65A5:
    /* 65A5  shld    edx,eax,17h */
    EDX = shld32(EDX, EAX, 23);
L65AA:
    /* 65AA  shl     eax,17h */
    EAX = (uint32_t)(EAX << 23);
L65AE:
    /* 65AE  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x65AE, 4)) != 0) return c;
L65B2:
    /* 65B2  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L65B5:
    /* 65B5  rol     eax,10h */
    EAX = rol32(EAX, 16);
L65B9:
    /* 65B9  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L65BD:
    /* 65BD  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L65BF:
    /* 65BF  idiv    dword ptr ds:[0D018h] */
    if (asm_idiv32(rd(pDS, 0xD018)) && (c = asm_divfault(0x065C, 0x65BF, 5)) != 0) return c;
L65C4:
    /* 65C4  mov     dword ptr ds:[0CF98h],eax */
    wd(pDS, 0xCF98, EAX);
L65C8:
    /* 65C8  mov     eax,100h */
    EAX = 0x100;
L65CE:
    /* 65CE  cmp     dword ptr [bx+8],eax */
    sub32(rd(pDS, BX + 0x8), EAX, 0);
L65D2:
    /* 65D2  ja      short L65DC */
    if (!CF && !ZF) goto L65DC;
L65D4:
    /* 65D4  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L65DA:
    /* 65DA  jmp     short L65F0 */
    goto L65F0;
L65DC: /* L65DC */
    /* 65DC  mov     eax,800000h */
    EAX = 0x800000;
L65E2:
    /* 65E2  rol     eax,10h */
    EAX = rol32(EAX, 16);
L65E6:
    /* 65E6  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L65EA:
    /* 65EA  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L65EC:
    /* 65EC  idiv    dword ptr [bx+8] */
    if (asm_idiv32(rd(pDS, BX + 0x8)) && (c = asm_divfault(0x065C, 0x65EC, 4)) != 0) return c;
L65F0: /* L65F0 */
    /* 65F0  mov     ecx,eax */
    ECX = EAX;
L65F3:
    /* 65F3  mov     eax,100h */
    EAX = 0x100;
L65F9:
    /* 65F9  cmp     dword ptr [si+8],eax */
    sub32(rd(pDS, SI + 0x8), EAX, 0);
L65FD:
    /* 65FD  ja      short L6607 */
    if (!CF && !ZF) goto L6607;
L65FF:
    /* 65FF  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L6605:
    /* 6605  jmp     short L661B */
    goto L661B;
L6607: /* L6607 */
    /* 6607  mov     eax,800000h */
    EAX = 0x800000;
L660D:
    /* 660D  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6611:
    /* 6611  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6615:
    /* 6615  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6617:
    /* 6617  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x6617, 4)) != 0) return c;
L661B: /* L661B */
    /* 661B  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L661E:
    /* 661E  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6622:
    /* 6622  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6626:
    /* 6626  sub     ax,ax */
    AX = sub16(AX, AX, 0);
L6628:
    /* 6628  idiv    dword ptr ds:[0D018h] */
    if (asm_idiv32(rd(pDS, 0xD018)) && (c = asm_divfault(0x065C, 0x6628, 5)) != 0) return c;
L662D:
    /* 662D  mov     dword ptr ds:[0CFA0h],eax */
    wd(pDS, 0xCFA0, EAX);
L6631: /* L6631 */
    /* 6631  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_6632  (+6632)
       seg004_0849_6632: compute_min_max_y's counterpart for columns: finds the least and greatest
       sx, starts both edges at the leftmost vertex (or at the two ends of a vertical left side), and
       clamps the first and last column to 0 .. 2 * scrw - 1 into D022 and D024. */
L6632: /* _seg004_0849_6632 */
    /* 6632  mov     dword ptr ds:[0D008h],7FFFFFFFh */
    wd(pDS, 0xD008, 0x7FFFFFFF);
L663B:
    /* 663B  mov     dword ptr ds:[0D00Ch],80000001h */
    wd(pDS, 0xD00C, 0x80000001);
L6644:
    /* 6644  mov     bx,word ptr [bp+6] */
    BX = rw(pSS, BP + 0x6);
L6647:
    /* 6647  nop */
    ;
L6648:
    /* 6648  sub     dx,dx */
    DX = (uint16_t)(DX - DX);
L664A: /* L664A */
    /* 664A  mov     eax,dword ptr [bx+0Ch] */
    EAX = rd(pDS, BX + 0xC);
L664E:
    /* 664E  cmp     eax,dword ptr ds:[0D008h] */
    sub32(EAX, rd(pDS, 0xD008), 0);
L6653:
    /* 6653  jge     short L665D */
    if (SF == OF) goto L665D;
L6655:
    /* 6655  mov     dword ptr ds:[0D008h],eax */
    wd(pDS, 0xD008, EAX);
L6659:
    /* 6659  mov     word ptr ds:[0CF52h],dx */
    ww(pDS, 0xCF52, DX);
L665D: /* L665D */
    /* 665D  cmp     eax,dword ptr ds:[0D00Ch] */
    sub32(EAX, rd(pDS, 0xD00C), 0);
L6662:
    /* 6662  jle     short L6668 */
    if (ZF || SF != OF) goto L6668;
L6664:
    /* 6664  mov     dword ptr ds:[0D00Ch],eax */
    wd(pDS, 0xD00C, EAX);
L6668: /* L6668 */
    /* 6668  inc     dx */
    DX = (uint16_t)(DX + 1);
L6669:
    /* 6669  add     bx,20h */
    BX = (uint16_t)(BX + 0x20);
L666C:
    /* 666C  cmp     dx,word ptr [bp+8] */
    sub16(DX, rw(pSS, BP + 0x8), 0);
L666F:
    /* 666F  nop */
    ;
L6670:
    /* 6670  jb      L664A */
    if (CF) goto L664A;
L6672:
    /* 6672  mov     ax,word ptr ds:[0CF52h] */
    AX = rw(pDS, 0xCF52);
L6675:
    /* 6675  mov     word ptr ds:[0CF54h],ax */
    ww(pDS, 0xCF54, AX);
L6678:
    /* 6678  mov     bx,20h */
    BX = 0x20;
L667B:
    /* 667B  imul    bx,word ptr ds:[0CF52h] */
    BX = imul16x(BX, rw(pDS, 0xCF52));
L6680:
    /* 6680  add     bx,word ptr [bp+6] */
    BX = (uint16_t)(BX + rw(pSS, BP + 0x6));
L6683:
    /* 6683  nop */
    ;
L6684:
    /* 6684  mov     edx,dword ptr [bx+0Ch] */
    EDX = rd(pDS, BX + 0xC);
L6688:
    /* 6688  add     edx,41h */
    EDX = (uint32_t)(EDX + 0x41);
L668C:
    /* 668C  sar     edx,10h */
    EDX = (uint32_t)((int32_t)EDX >> 16);
L6690:
    /* 6690  mov     bx,word ptr ds:[0CF52h] */
    BX = rw(pDS, 0xCF52);
L6694:
    /* 6694  inc     bx */
    BX = (uint16_t)(BX + 1);
L6695:
    /* 6695  cmp     bx,word ptr [bp+8] */
    sub16(BX, rw(pSS, BP + 0x8), 0);
L6698:
    /* 6698  nop */
    ;
L6699:
    /* 6699  jb      short L669D */
    if (CF) goto L669D;
L669B:
    /* 669B  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L669D: /* L669D */
    /* 669D  mov     cx,bx */
    CX = BX;
L669F:
    /* 669F  imul    bx,20h */
    BX = imul16x(BX, 0x20);
L66A2:
    /* 66A2  add     bx,word ptr [bp+6] */
    BX = (uint16_t)(BX + rw(pSS, BP + 0x6));
L66A5:
    /* 66A5  nop */
    ;
L66A6:
    /* 66A6  mov     eax,dword ptr [bx+0Ch] */
    EAX = rd(pDS, BX + 0xC);
L66AA:
    /* 66AA  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L66AE:
    /* 66AE  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L66B2:
    /* 66B2  cmp     ax,dx */
    sub16(AX, DX, 0);
L66B4:
    /* 66B4  jne     short L66BA */
    if (!ZF) goto L66BA;
L66B6:
    /* 66B6  mov     word ptr ds:[0CF54h],cx */
    ww(pDS, 0xCF54, CX);
L66BA: /* L66BA */
    /* 66BA  mov     bx,word ptr ds:[0CF52h] */
    BX = rw(pDS, 0xCF52);
L66BE:
    /* 66BE  dec     bx */
    BX = dec16(BX);
L66BF:
    /* 66BF  jns     short L66C5 */
    if (!SF) goto L66C5;
L66C1:
    /* 66C1  add     bx,word ptr [bp+8] */
    BX = (uint16_t)(BX + rw(pSS, BP + 0x8));
L66C4:
    /* 66C4  nop */
    ;
L66C5: /* L66C5 */
    /* 66C5  imul    bx,20h */
    BX = imul16x(BX, 0x20);
L66C8:
    /* 66C8  add     bx,word ptr [bp+6] */
    BX = (uint16_t)(BX + rw(pSS, BP + 0x6));
L66CB:
    /* 66CB  nop */
    ;
L66CC:
    /* 66CC  mov     eax,dword ptr [bx+0Ch] */
    EAX = rd(pDS, BX + 0xC);
L66D0:
    /* 66D0  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L66D4:
    /* 66D4  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L66D8:
    /* 66D8  cmp     ax,dx */
    sub16(AX, DX, 0);
L66DA:
    /* 66DA  jne     short L66EF */
    if (!ZF) goto L66EF;
L66DC:
    /* 66DC  mov     ax,word ptr ds:[0CF52h] */
    AX = rw(pDS, 0xCF52);
L66DF:
    /* 66DF  mov     word ptr ds:[0CF54h],ax */
    ww(pDS, 0xCF54, AX);
L66E2:
    /* 66E2  dec     word ptr ds:[0CF52h] */
    ww(pDS, 0xCF52, dec16(rw(pDS, 0xCF52)));
L66E6:
    /* 66E6  jns     short L66EF */
    if (!SF) goto L66EF;
L66E8:
    /* 66E8  mov     ax,word ptr [bp+8] */
    AX = rw(pSS, BP + 0x8);
L66EB:
    /* 66EB  add     word ptr ds:[0CF52h],ax */
    ww(pDS, 0xCF52, (uint16_t)(rw(pDS, 0xCF52) + AX));
L66EF: /* L66EF */
    /* 66EF  mov     bx,word ptr ds:[2472h] */
    BX = rw(pDS, 0x2472);
L66F3:
    /* 66F3  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L66F5:
    /* 66F5  mov     eax,dword ptr ds:[0D008h] */
    EAX = rd(pDS, 0xD008);
L66F9:
    /* 66F9  shr     eax,10h */
    EAX = (uint32_t)(EAX >> 16);
L66FD:
    /* 66FD  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L66FF:
    /* 66FF  jns     short L6703 */
    if (!SF) goto L6703;
L6701:
    /* 6701  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6703: /* L6703 */
    /* 6703  cmp     ax,bx */
    sub16(AX, BX, 0);
L6705:
    /* 6705  jl      short L670A */
    if (SF != OF) goto L670A;
L6707:
    /* 6707  lea     ax,[bx-1] */
    AX = (uint16_t)(BX + 0xFFFF);
L670A: /* L670A */
    /* 670A  mov     word ptr ds:[0D022h],ax */
    ww(pDS, 0xD022, AX);
L670D:
    /* 670D  mov     eax,dword ptr ds:[0D00Ch] */
    EAX = rd(pDS, 0xD00C);
L6711:
    /* 6711  shr     eax,10h */
    EAX = (uint32_t)(EAX >> 16);
L6715:
    /* 6715  cmp     ax,bx */
    sub16(AX, BX, 0);
L6717:
    /* 6717  jl      short L671C */
    if (SF != OF) goto L671C;
L6719:
    /* 6719  lea     ax,[bx-1] */
    AX = (uint16_t)(BX + 0xFFFF);
L671C: /* L671C */
    /* 671C  mov     word ptr ds:[0D024h],ax */
    ww(pDS, 0xD024, AX);
L671F:
    /* 671F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_6720  (+6720)
       _asm_texture_map_v(descriptor, vertices, count, mapping, shaded): draw the polygon into the
       frame buffer (ES = seg_370D:0958, GS = the texture's segment) one column at a time from D022
       to D024, advancing either edge to its next vertex when the column passes it, drawing the
       column (seg004_0849_68B5) and stepping both edges. The last column is drawn after the loop.
       Keeps SI, DI, ES, GS, BP; clears CF44 on the way out. */
L6720: /* __asm_texture_map_v */
    /* 6720  push    bp */
    push16(BP);
L6721:
    /* 6721  mov     bp,sp */
    BP = SP;
L6723:
    /* 6723  push    si */
    push16(SI);
L6724:
    /* 6724  push    di */
    push16(DI);
L6725:
    /* 6725  push    es */
    push16(asm_es);
L6726:
    /* 6726  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L6729:
    /* 6729  mov     es,ax */
    SET_ES(AX);
L672B:
    /* 672B  mov     es,word ptr es:[958h] */
    SET_ES(rw(pES, 0x958));
L6730:
    /* 6730  push    gs */
    push16(asm_gs);
L6732:
    /* 6732  mov     bx,word ptr [bp+4] */
    BX = rw(pSS, BP + 0x4);
L6735:
    /* 6735  mov     ax,word ptr [bx+4] */
    AX = rw(pDS, BX + 0x4);
L6738:
    /* 6738  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L673A:
    /* 673A  je      L6741 */
    if (ZF) goto L6741;
L673C:
    /* 673C  nop */
    ;
L673D:
    /* 673D  nop */
    ;
L673E:
    /* 673E  int     2 */
    asm_halt_at(0x065C, 0x673E, "int 2h, the debugger break");
L6740:
    /* 6740  nop */
    ;
L6741: /* L6741 */
    /* 6741  mov     word ptr ds:[0CF3Ch],ax */
    ww(pDS, 0xCF3C, AX);
L6744:
    /* 6744  mov     gs,word ptr [bx+6] */
    SET_GS(rw(pDS, BX + 0x6));
L6747:
    /* 6747  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L6749:
    /* 6749  mov     word ptr ds:[0CF3Eh],ax */
    ww(pDS, 0xCF3E, AX);
L674C:
    /* 674C  mov     word ptr ds:[0CF36h],ax */
    ww(pDS, 0xCF36, AX);
L674F:
    /* 674F  mov     word ptr ds:[0CF34h],0 */
    ww(pDS, 0xCF34, 0x0);
L6755:
    /* 6755  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L6758:
    /* 6758  mov     word ptr ds:[0CF40h],ax */
    ww(pDS, 0xCF40, AX);
L675B:
    /* 675B  mov     word ptr ds:[0CF3Ah],ax */
    ww(pDS, 0xCF3A, AX);
L675E:
    /* 675E  mov     word ptr ds:[0CF38h],0 */
    ww(pDS, 0xCF38, 0x0);
L6764:
    /* 6764  mov     al,byte ptr [bx+8] */
    AL = rb(pDS, BX + 0x8);
L6767:
    /* 6767  mov     byte ptr ds:[0CF46h],al */
    wb(pDS, 0xCF46, AL);
L676A:
    /* 676A  call    _seg004_0849_6632 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x6632), 0x676D)) != 0) return c;
L676D:
    /* 676D  call    _seg004_0849_6952 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x6952), 0x6770)) != 0) return c;
L6770:
    /* 6770  mov     ax,word ptr ds:[0D022h] */
    AX = rw(pDS, 0xD022);
L6773:
    /* 6773  mov     word ptr ds:[0CCC4h],ax */
    ww(pDS, 0xCCC4, AX);
L6776: /* L6776 */
    /* 6776  mov     si,word ptr ds:[0CF52h] */
    SI = rw(pDS, 0xCF52);
L677A:
    /* 677A  dec     si */
    SI = dec16(SI);
L677B:
    /* 677B  jns     short L6780 */
    if (!SF) goto L6780;
L677D:
    /* 677D  add     si,word ptr [bp+8] */
    SI = (uint16_t)(SI + rw(pSS, BP + 0x8));
L6780: /* L6780 */
    /* 6780  imul    si,20h */
    SI = imul16x(SI, 0x20);
L6783:
    /* 6783  mov     bx,word ptr [bp+6] */
    BX = rw(pSS, BP + 0x6);
L6786:
    /* 6786  mov     eax,dword ptr [bx+si+0Ch] */
    EAX = rd(pDS, BX + SI + 0xC);
L678A:
    /* 678A  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L678E:
    /* 678E  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L6792:
    /* 6792  cmp     ax,word ptr ds:[0CCC4h] */
    sub16(AX, rw(pDS, 0xCCC4), 0);
L6796:
    /* 6796  jg      short L67A8 */
    if (!ZF && SF == OF) goto L67A8;
L6798:
    /* 6798  call    _seg004_0849_6240 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x6240), 0x679B)) != 0) return c;
L679B:
    /* 679B  dec     word ptr ds:[0CF52h] */
    ww(pDS, 0xCF52, dec16(rw(pDS, 0xCF52)));
L679F:
    /* 679F  jns     short L67A8 */
    if (!SF) goto L67A8;
L67A1:
    /* 67A1  mov     ax,word ptr [bp+8] */
    AX = rw(pSS, BP + 0x8);
L67A4:
    /* 67A4  add     word ptr ds:[0CF52h],ax */
    ww(pDS, 0xCF52, (uint16_t)(rw(pDS, 0xCF52) + AX));
L67A8: /* L67A8 */
    /* 67A8  mov     bx,word ptr ds:[0CF54h] */
    BX = rw(pDS, 0xCF54);
L67AC:
    /* 67AC  inc     bx */
    BX = (uint16_t)(BX + 1);
L67AD:
    /* 67AD  cmp     bx,word ptr [bp+8] */
    sub16(BX, rw(pSS, BP + 0x8), 0);
L67B0:
    /* 67B0  jb      short L67B4 */
    if (CF) goto L67B4;
L67B2:
    /* 67B2  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L67B4: /* L67B4 */
    /* 67B4  imul    bx,20h */
    BX = imul16x(BX, 0x20);
L67B7:
    /* 67B7  add     bx,word ptr [bp+6] */
    BX = (uint16_t)(BX + rw(pSS, BP + 0x6));
L67BA:
    /* 67BA  mov     eax,dword ptr [bx+0Ch] */
    EAX = rd(pDS, BX + 0xC);
L67BE:
    /* 67BE  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L67C2:
    /* 67C2  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L67C6:
    /* 67C6  cmp     ax,word ptr ds:[0CCC4h] */
    sub16(AX, rw(pDS, 0xCCC4), 0);
L67CA:
    /* 67CA  jg      short L67E1 */
    if (!ZF && SF == OF) goto L67E1;
L67CC:
    /* 67CC  call    _seg004_0849_6437 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x6437), 0x67CF)) != 0) return c;
L67CF:
    /* 67CF  inc     word ptr ds:[0CF54h] */
    ww(pDS, 0xCF54, (uint16_t)(rw(pDS, 0xCF54) + 1));
L67D3:
    /* 67D3  mov     ax,word ptr ds:[0CF54h] */
    AX = rw(pDS, 0xCF54);
L67D6:
    /* 67D6  cmp     ax,word ptr [bp+8] */
    sub16(AX, rw(pSS, BP + 0x8), 0);
L67D9:
    /* 67D9  jb      short L67E1 */
    if (CF) goto L67E1;
L67DB:
    /* 67DB  mov     word ptr ds:[0CF54h],0 */
    ww(pDS, 0xCF54, 0x0);
L67E1: /* L67E1 */
    /* 67E1  cmp     word ptr ds:[0CF44h],1 */
    sub16(rw(pDS, 0xCF44), 0x1, 0);
L67E6:
    /* 67E6  je      L6859 */
    if (ZF) goto L6859;
L67E8:
    /* 67E8  nop */
    ;
L67E9:
    /* 67E9  nop */
    ;
L67EA:
    /* 67EA  call    _seg004_0849_68B5 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x68B5), 0x67ED)) != 0) return c;
L67ED:
    /* 67ED  mov     eax,dword ptr ds:[0CF84h] */
    EAX = rd(pDS, 0xCF84);
L67F1:
    /* 67F1  add     dword ptr ds:[0D010h],eax */
    wd(pDS, 0xD010, (uint32_t)(rd(pDS, 0xD010) + EAX));
L67F6:
    /* 67F6  mov     eax,dword ptr ds:[0CF88h] */
    EAX = rd(pDS, 0xCF88);
L67FA:
    /* 67FA  add     dword ptr ds:[0CF60h],eax */
    wd(pDS, 0xCF60, (uint32_t)(rd(pDS, 0xCF60) + EAX));
L67FF:
    /* 67FF  mov     eax,dword ptr ds:[0CF8Ch] */
    EAX = rd(pDS, 0xCF8C);
L6803:
    /* 6803  add     dword ptr ds:[0CF64h],eax */
    wd(pDS, 0xCF64, (uint32_t)(rd(pDS, 0xCF64) + EAX));
L6808:
    /* 6808  mov     eax,dword ptr ds:[0CF9Ch] */
    EAX = rd(pDS, 0xCF9C);
L680C:
    /* 680C  add     dword ptr ds:[0CF74h],eax */
    wd(pDS, 0xCF74, (uint32_t)(rd(pDS, 0xCF74) + EAX));
L6811:
    /* 6811  mov     eax,dword ptr ds:[0CFA4h] */
    EAX = rd(pDS, 0xCFA4);
L6815:
    /* 6815  add     dword ptr ds:[0CF7Ch],eax */
    wd(pDS, 0xCF7C, (uint32_t)(rd(pDS, 0xCF7C) + EAX));
L681A:
    /* 681A  mov     eax,dword ptr ds:[0CF90h] */
    EAX = rd(pDS, 0xCF90);
L681E:
    /* 681E  add     dword ptr ds:[0D014h],eax */
    wd(pDS, 0xD014, (uint32_t)(rd(pDS, 0xD014) + EAX));
L6823:
    /* 6823  mov     eax,dword ptr ds:[0CF94h] */
    EAX = rd(pDS, 0xCF94);
L6827:
    /* 6827  add     dword ptr ds:[0CF6Ch],eax */
    wd(pDS, 0xCF6C, (uint32_t)(rd(pDS, 0xCF6C) + EAX));
L682C:
    /* 682C  mov     eax,dword ptr ds:[0CF98h] */
    EAX = rd(pDS, 0xCF98);
L6830:
    /* 6830  add     dword ptr ds:[0CF70h],eax */
    wd(pDS, 0xCF70, (uint32_t)(rd(pDS, 0xCF70) + EAX));
L6835:
    /* 6835  mov     eax,dword ptr ds:[0CFA0h] */
    EAX = rd(pDS, 0xCFA0);
L6839:
    /* 6839  add     dword ptr ds:[0CF78h],eax */
    wd(pDS, 0xCF78, (uint32_t)(rd(pDS, 0xCF78) + EAX));
L683E:
    /* 683E  mov     eax,dword ptr ds:[0CFA8h] */
    EAX = rd(pDS, 0xCFA8);
L6842:
    /* 6842  add     dword ptr ds:[0CF80h],eax */
    wd(pDS, 0xCF80, (uint32_t)(rd(pDS, 0xCF80) + EAX));
L6847:
    /* 6847  inc     word ptr ds:[0CCC4h] */
    ww(pDS, 0xCCC4, (uint16_t)(rw(pDS, 0xCCC4) + 1));
L684B:
    /* 684B  mov     ax,word ptr ds:[0CCC4h] */
    AX = rw(pDS, 0xCCC4);
L684E:
    /* 684E  cmp     ax,word ptr ds:[0D024h] */
    sub16(AX, rw(pDS, 0xD024), 0);
L6852:
    /* 6852  jl      L6776 */
    if (SF != OF) goto L6776;
L6856:
    /* 6856  call    _seg004_0849_68B5 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x68B5), 0x6859)) != 0) return c;
L6859: /* L6859 */
    /* 6859  mov     word ptr ds:[0CF44h],0 */
    ww(pDS, 0xCF44, 0x0);
L685F:
    /* 685F  pop     gs */
    SET_GS(pop16());
L6861:
    /* 6861  pop     es */
    SET_ES(pop16());
L6862:
    /* 6862  pop     di */
    DI = pop16();
L6863:
    /* 6863  pop     si */
    SI = pop16();
L6864:
    /* 6864  pop     bp */
    BP = pop16();
L6865:
    /* 6865  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_6866  (+6866)
       seg004_0849_6866: clamp the column: y ends below 0 become 0, y ends at or past 2 * scrh become
       2 * scrh - 1, and a column at or past 2 * scrw becomes the last one. Changes AX. */
L6866: /* _seg004_0849_6866 */
    /* 6866  test    word ptr ds:[0CFE6h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0xCFE6) & 0xFFFF));
L686C:
    /* 686C  jns     short L6877 */
    if (!SF) goto L6877;
L686E:
    /* 686E  mov     dword ptr ds:[0CFE4h],0 */
    wd(pDS, 0xCFE4, 0x0);
L6877: /* L6877 */
    /* 6877  test    word ptr ds:[0CFE2h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0xCFE2) & 0xFFFF));
L687D:
    /* 687D  jns     short L6888 */
    if (!SF) goto L6888;
L687F:
    /* 687F  mov     dword ptr ds:[0CFE0h],0 */
    wd(pDS, 0xCFE0, 0x0);
L6888: /* L6888 */
    /* 6888  mov     ax,word ptr ds:[2470h] */
    AX = rw(pDS, 0x2470);
L688B:
    /* 688B  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L688D:
    /* 688D  cmp     word ptr ds:[0CFE6h],ax */
    sub16(rw(pDS, 0xCFE6), AX, 0);
L6891:
    /* 6891  jb      short L689A */
    if (CF) goto L689A;
L6893:
    /* 6893  mov     word ptr ds:[0CFE6h],ax */
    ww(pDS, 0xCFE6, AX);
L6896:
    /* 6896  dec     word ptr ds:[0CFE6h] */
    ww(pDS, 0xCFE6, (uint16_t)(rw(pDS, 0xCFE6) - 1));
L689A: /* L689A */
    /* 689A  cmp     word ptr ds:[0CFE2h],ax */
    sub16(rw(pDS, 0xCFE2), AX, 0);
L689E:
    /* 689E  jb      short L68A4 */
    if (CF) goto L68A4;
L68A0:
    /* 68A0  dec     ax */
    AX = (uint16_t)(AX - 1);
L68A1:
    /* 68A1  mov     word ptr ds:[0CFE2h],ax */
    ww(pDS, 0xCFE2, AX);
L68A4: /* L68A4 */
    /* 68A4  mov     ax,word ptr ds:[2472h] */
    AX = rw(pDS, 0x2472);
L68A7:
    /* 68A7  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L68A9:
    /* 68A9  cmp     word ptr ds:[0CCC4h],ax */
    sub16(rw(pDS, 0xCCC4), AX, 0);
L68AD:
    /* 68AD  jae     short L68B0 */
    if (!CF) goto L68B0;
L68AF:
    /* 68AF  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L68B0: /* L68B0 */
    /* 68B0  dec     ax */
    AX = dec16(AX);
L68B1:
    /* 68B1  mov     word ptr ds:[0CCC4h],ax */
    ww(pDS, 0xCCC4, AX);
L68B4:
    /* 68B4  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_68B5  (+68B5)
       seg004_0849_68B5: draw the current column. Its ends are the two edges' y, widened by a pixel
       each way (one end's integer part less one, the other's plus one) and clamped
       (seg004_0849_6866); u, v at both ends are u * w / w and v * w / w; l and w are copied; then
       the span drawer _seg004_0849_51B0 (SCANLINE.ASM) is called. Keeps BP. */
L68B5: /* _seg004_0849_68B5 */
    /* 68B5  mov     eax,dword ptr ds:[0D010h] */
    EAX = rd(pDS, 0xD010);
L68B9:
    /* 68B9  mov     dword ptr ds:[0CFE4h],eax */
    wd(pDS, 0xCFE4, EAX);
L68BD:
    /* 68BD  mov     eax,dword ptr ds:[0D014h] */
    EAX = rd(pDS, 0xD014);
L68C1:
    /* 68C1  mov     dword ptr ds:[0CFE0h],eax */
    wd(pDS, 0xCFE0, EAX);
L68C5:
    /* 68C5  dec     word ptr ds:[0CFE2h] */
    ww(pDS, 0xCFE2, (uint16_t)(rw(pDS, 0xCFE2) - 1));
L68C9:
    /* 68C9  inc     word ptr ds:[0CFE6h] */
    ww(pDS, 0xCFE6, (uint16_t)(rw(pDS, 0xCFE6) + 1));
L68CD:
    /* 68CD  call    _seg004_0849_6866 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x6866), 0x68D0)) != 0) return c;
L68D0:
    /* 68D0  mov     eax,dword ptr ds:[0CF6Ch] */
    EAX = rd(pDS, 0xCF6C);
L68D4:
    /* 68D4  rol     eax,10h */
    EAX = rol32(EAX, 16);
L68D8:
    /* 68D8  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L68DC:
    /* 68DC  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L68DE:
    /* 68DE  idiv    dword ptr ds:[0CF78h] */
    if (asm_idiv32(rd(pDS, 0xCF78)) && (c = asm_divfault(0x065C, 0x68DE, 5)) != 0) return c;
L68E3:
    /* 68E3  mov     dword ptr ds:[0CFE8h],eax */
    wd(pDS, 0xCFE8, EAX);
L68E7:
    /* 68E7  mov     eax,dword ptr ds:[0CF60h] */
    EAX = rd(pDS, 0xCF60);
L68EB:
    /* 68EB  rol     eax,10h */
    EAX = rol32(EAX, 16);
L68EF:
    /* 68EF  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L68F3:
    /* 68F3  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L68F5:
    /* 68F5  idiv    dword ptr ds:[0CF74h] */
    if (asm_idiv32(rd(pDS, 0xCF74)) && (c = asm_divfault(0x065C, 0x68F5, 5)) != 0) return c;
L68FA:
    /* 68FA  mov     dword ptr ds:[0CFECh],eax */
    wd(pDS, 0xCFEC, EAX);
L68FE:
    /* 68FE  mov     eax,dword ptr ds:[0CF70h] */
    EAX = rd(pDS, 0xCF70);
L6902:
    /* 6902  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6906:
    /* 6906  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L690A:
    /* 690A  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L690C:
    /* 690C  idiv    dword ptr ds:[0CF78h] */
    if (asm_idiv32(rd(pDS, 0xCF78)) && (c = asm_divfault(0x065C, 0x690C, 5)) != 0) return c;
L6911:
    /* 6911  mov     dword ptr ds:[0CFF0h],eax */
    wd(pDS, 0xCFF0, EAX);
L6915:
    /* 6915  mov     eax,dword ptr ds:[0CF64h] */
    EAX = rd(pDS, 0xCF64);
L6919:
    /* 6919  rol     eax,10h */
    EAX = rol32(EAX, 16);
L691D:
    /* 691D  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6921:
    /* 6921  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6923:
    /* 6923  idiv    dword ptr ds:[0CF74h] */
    if (asm_idiv32(rd(pDS, 0xCF74)) && (c = asm_divfault(0x065C, 0x6923, 5)) != 0) return c;
L6928:
    /* 6928  mov     dword ptr ds:[0CFF4h],eax */
    wd(pDS, 0xCFF4, EAX);
L692C:
    /* 692C  mov     eax,dword ptr ds:[0CF80h] */
    EAX = rd(pDS, 0xCF80);
L6930:
    /* 6930  mov     dword ptr ds:[0CFF8h],eax */
    wd(pDS, 0xCFF8, EAX);
L6934:
    /* 6934  mov     eax,dword ptr ds:[0CF7Ch] */
    EAX = rd(pDS, 0xCF7C);
L6938:
    /* 6938  mov     dword ptr ds:[0CFFCh],eax */
    wd(pDS, 0xCFFC, EAX);
L693C:
    /* 693C  mov     eax,dword ptr ds:[0CF78h] */
    EAX = rd(pDS, 0xCF78);
L6940:
    /* 6940  mov     dword ptr ds:[0D000h],eax */
    wd(pDS, 0xD000, EAX);
L6944:
    /* 6944  mov     eax,dword ptr ds:[0CF74h] */
    EAX = rd(pDS, 0xCF74);
L6948:
    /* 6948  mov     dword ptr ds:[0D004h],eax */
    wd(pDS, 0xD004, EAX);
L694C:
    /* 694C  push    bp */
    push16(BP);
L694D:
    /* 694D  call    _seg004_0849_51B0 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x51B0), 0x6950)) != 0) return c;
L6950:
    /* 6950  pop     bp */
    BP = pop16();
L6951:
    /* 6951  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_6952  (+6952)
       seg004_0849_6952: set up both edges from the leftmost vertex at once (TEXMAP.ASM's
       seg004_0849_5D66 turned on its side; an edge of zero columns is taken as one column). */
L6952: /* _seg004_0849_6952 */
    /* 6952  mov     ax,20h */
    AX = 0x20;
L6955:
    /* 6955  imul    word ptr ds:[0CF52h] */
    imul16(rw(pDS, 0xCF52));
L6959:
    /* 6959  add     ax,word ptr [bp+6] */
    AX = (uint16_t)(AX + rw(pSS, BP + 0x6));
L695C:
    /* 695C  mov     word ptr ds:[0CFC4h],ax */
    ww(pDS, 0xCFC4, AX);
L695F:
    /* 695F  mov     ax,word ptr ds:[0CF52h] */
    AX = rw(pDS, 0xCF52);
L6962:
    /* 6962  dec     ax */
    AX = dec16(AX);
L6963:
    /* 6963  jns     short L6968 */
    if (!SF) goto L6968;
L6965:
    /* 6965  add     ax,word ptr [bp+8] */
    AX = (uint16_t)(AX + rw(pSS, BP + 0x8));
L6968: /* L6968 */
    /* 6968  imul    ax,20h */
    AX = imul16x(AX, 0x20);
L696B:
    /* 696B  add     ax,word ptr [bp+6] */
    AX = (uint16_t)(AX + rw(pSS, BP + 0x6));
L696E:
    /* 696E  mov     word ptr ds:[0CFC6h],ax */
    ww(pDS, 0xCFC6, AX);
L6971:
    /* 6971  mov     ax,20h */
    AX = 0x20;
L6974:
    /* 6974  imul    word ptr ds:[0CF54h] */
    imul16(rw(pDS, 0xCF54));
L6978:
    /* 6978  add     ax,word ptr [bp+6] */
    AX = (uint16_t)(AX + rw(pSS, BP + 0x6));
L697B:
    /* 697B  mov     word ptr ds:[0CFC8h],ax */
    ww(pDS, 0xCFC8, AX);
L697E:
    /* 697E  mov     ax,word ptr ds:[0CF54h] */
    AX = rw(pDS, 0xCF54);
L6981:
    /* 6981  inc     ax */
    AX = (uint16_t)(AX + 1);
L6982:
    /* 6982  cmp     ax,word ptr [bp+8] */
    sub16(AX, rw(pSS, BP + 0x8), 0);
L6985:
    /* 6985  jb      short L6989 */
    if (CF) goto L6989;
L6987:
    /* 6987  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6989: /* L6989 */
    /* 6989  imul    ax,20h */
    AX = imul16x(AX, 0x20);
L698C:
    /* 698C  add     ax,word ptr [bp+6] */
    AX = (uint16_t)(AX + rw(pSS, BP + 0x6));
L698F:
    /* 698F  mov     word ptr ds:[0CFCAh],ax */
    ww(pDS, 0xCFCA, AX);
L6992:
    /* 6992  mov     si,word ptr ds:[0CFC6h] */
    SI = rw(pDS, 0xCFC6);
L6996:
    /* 6996  mov     eax,dword ptr [si+0Ch] */
    EAX = rd(pDS, SI + 0xC);
L699A:
    /* 699A  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L699E:
    /* 699E  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L69A2:
    /* 69A2  mov     si,word ptr ds:[0CFC4h] */
    SI = rw(pDS, 0xCFC4);
L69A6:
    /* 69A6  mov     ecx,dword ptr [si+0Ch] */
    ECX = rd(pDS, SI + 0xC);
L69AA:
    /* 69AA  add     ecx,41h */
    ECX = (uint32_t)(ECX + 0x41);
L69AE:
    /* 69AE  sar     ecx,10h */
    ECX = (uint32_t)((int32_t)ECX >> 16);
L69B2:
    /* 69B2  sub     ax,cx */
    AX = sub16(AX, CX, 0);
L69B4:
    /* 69B4  jne     short L69B7 */
    if (!ZF) goto L69B7;
L69B6:
    /* 69B6  inc     ax */
    AX = (uint16_t)(AX + 1);
L69B7: /* L69B7 */
    /* 69B7  shl     eax,10h */
    EAX = (uint32_t)(EAX << 16);
L69BB:
    /* 69BB  mov     dword ptr ds:[0CFBCh],eax */
    wd(pDS, 0xCFBC, EAX);
L69BF:
    /* 69BF  mov     si,word ptr ds:[0CFCAh] */
    SI = rw(pDS, 0xCFCA);
L69C3:
    /* 69C3  mov     eax,dword ptr [si+0Ch] */
    EAX = rd(pDS, SI + 0xC);
L69C7:
    /* 69C7  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L69CB:
    /* 69CB  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L69CF:
    /* 69CF  mov     si,word ptr ds:[0CFC8h] */
    SI = rw(pDS, 0xCFC8);
L69D3:
    /* 69D3  mov     ecx,dword ptr [si+0Ch] */
    ECX = rd(pDS, SI + 0xC);
L69D7:
    /* 69D7  add     ecx,41h */
    ECX = (uint32_t)(ECX + 0x41);
L69DB:
    /* 69DB  sar     ecx,10h */
    ECX = (uint32_t)((int32_t)ECX >> 16);
L69DF:
    /* 69DF  sub     ax,cx */
    AX = sub16(AX, CX, 0);
L69E1:
    /* 69E1  jne     short L69E4 */
    if (!ZF) goto L69E4;
L69E3:
    /* 69E3  inc     ax */
    AX = (uint16_t)(AX + 1);
L69E4: /* L69E4 */
    /* 69E4  shl     eax,10h */
    EAX = (uint32_t)(EAX << 16);
L69E8:
    /* 69E8  mov     dword ptr ds:[0CFC0h],eax */
    wd(pDS, 0xCFC0, EAX);
L69EC:
    /* 69EC  mov     si,word ptr ds:[0CFC4h] */
    SI = rw(pDS, 0xCFC4);
L69F0:
    /* 69F0  mov     ecx,dword ptr [si+10h] */
    ECX = rd(pDS, SI + 0x10);
L69F4:
    /* 69F4  mov     dword ptr ds:[0D010h],ecx */
    wd(pDS, 0xD010, ECX);
L69F9:
    /* 69F9  mov     si,word ptr ds:[0CFC6h] */
    SI = rw(pDS, 0xCFC6);
L69FD:
    /* 69FD  mov     eax,dword ptr [si+10h] */
    EAX = rd(pDS, SI + 0x10);
L6A01:
    /* 6A01  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L6A04:
    /* 6A04  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6A08:
    /* 6A08  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6A0C:
    /* 6A0C  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6A0E:
    /* 6A0E  idiv    dword ptr ds:[0CFBCh] */
    if (asm_idiv32(rd(pDS, 0xCFBC)) && (c = asm_divfault(0x065C, 0x6A0E, 5)) != 0) return c;
L6A13:
    /* 6A13  mov     dword ptr ds:[0CF84h],eax */
    wd(pDS, 0xCF84, EAX);
L6A17:
    /* 6A17  mov     si,word ptr ds:[0CFC8h] */
    SI = rw(pDS, 0xCFC8);
L6A1B:
    /* 6A1B  mov     ecx,dword ptr [si+10h] */
    ECX = rd(pDS, SI + 0x10);
L6A1F:
    /* 6A1F  mov     dword ptr ds:[0D014h],ecx */
    wd(pDS, 0xD014, ECX);
L6A24:
    /* 6A24  mov     si,word ptr ds:[0CFCAh] */
    SI = rw(pDS, 0xCFCA);
L6A28:
    /* 6A28  mov     eax,dword ptr [si+10h] */
    EAX = rd(pDS, SI + 0x10);
L6A2C:
    /* 6A2C  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L6A2F:
    /* 6A2F  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6A33:
    /* 6A33  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6A37:
    /* 6A37  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6A39:
    /* 6A39  idiv    dword ptr ds:[0CFC0h] */
    if (asm_idiv32(rd(pDS, 0xCFC0)) && (c = asm_divfault(0x065C, 0x6A39, 5)) != 0) return c;
L6A3E:
    /* 6A3E  mov     dword ptr ds:[0CF90h],eax */
    wd(pDS, 0xCF90, EAX);
L6A42:
    /* 6A42  mov     si,word ptr ds:[0CFC4h] */
    SI = rw(pDS, 0xCFC4);
L6A46:
    /* 6A46  mov     di,word ptr ds:[0CFC6h] */
    DI = rw(pDS, 0xCFC6);
L6A4A:
    /* 6A4A  mov     eax,dword ptr [si+1Ch] */
    EAX = rd(pDS, SI + 0x1C);
L6A4E:
    /* 6A4E  mov     dword ptr ds:[0CF7Ch],eax */
    wd(pDS, 0xCF7C, EAX);
L6A52:
    /* 6A52  mov     eax,100h */
    EAX = 0x100;
L6A58:
    /* 6A58  cmp     dword ptr [si+8],eax */
    sub32(rd(pDS, SI + 0x8), EAX, 0);
L6A5C:
    /* 6A5C  ja      short L6A66 */
    if (!CF && !ZF) goto L6A66;
L6A5E:
    /* 6A5E  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L6A64:
    /* 6A64  jmp     short L6A7A */
    goto L6A7A;
L6A66: /* L6A66 */
    /* 6A66  mov     eax,800000h */
    EAX = 0x800000;
L6A6C:
    /* 6A6C  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6A70:
    /* 6A70  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6A74:
    /* 6A74  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6A76:
    /* 6A76  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x6A76, 4)) != 0) return c;
L6A7A: /* L6A7A */
    /* 6A7A  mov     dword ptr ds:[0CF74h],eax */
    wd(pDS, 0xCF74, EAX);
L6A7E:
    /* 6A7E  imul    dword ptr [si+14h] */
    imul32(rd(pDS, SI + 0x14));
L6A82:
    /* 6A82  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L6A87:
    /* 6A87  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L6A89:
    /* 6A89  mov     dword ptr ds:[0CF60h],eax */
    wd(pDS, 0xCF60, EAX);
L6A8D:
    /* 6A8D  mov     eax,dword ptr [si+18h] */
    EAX = rd(pDS, SI + 0x18);
L6A91:
    /* 6A91  imul    dword ptr ds:[0CF74h] */
    imul32(rd(pDS, 0xCF74));
L6A96:
    /* 6A96  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L6A9B:
    /* 6A9B  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L6A9D:
    /* 6A9D  mov     dword ptr ds:[0CF64h],eax */
    wd(pDS, 0xCF64, EAX);
L6AA1:
    /* 6AA1  mov     eax,100h */
    EAX = 0x100;
L6AA7:
    /* 6AA7  cmp     dword ptr [si+8],eax */
    sub32(rd(pDS, SI + 0x8), EAX, 0);
L6AAB:
    /* 6AAB  ja      short L6AB5 */
    if (!CF && !ZF) goto L6AB5;
L6AAD:
    /* 6AAD  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L6AB3:
    /* 6AB3  jmp     short L6AC9 */
    goto L6AC9;
L6AB5: /* L6AB5 */
    /* 6AB5  mov     eax,800000h */
    EAX = 0x800000;
L6ABB:
    /* 6ABB  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6ABF:
    /* 6ABF  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6AC3:
    /* 6AC3  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6AC5:
    /* 6AC5  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x6AC5, 4)) != 0) return c;
L6AC9: /* L6AC9 */
    /* 6AC9  mov     ecx,eax */
    ECX = EAX;
L6ACC:
    /* 6ACC  mov     eax,100h */
    EAX = 0x100;
L6AD2:
    /* 6AD2  cmp     dword ptr [di+8],eax */
    sub32(rd(pDS, DI + 0x8), EAX, 0);
L6AD6:
    /* 6AD6  ja      short L6AE0 */
    if (!CF && !ZF) goto L6AE0;
L6AD8:
    /* 6AD8  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L6ADE:
    /* 6ADE  jmp     short L6AF4 */
    goto L6AF4;
L6AE0: /* L6AE0 */
    /* 6AE0  mov     eax,800000h */
    EAX = 0x800000;
L6AE6:
    /* 6AE6  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6AEA:
    /* 6AEA  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6AEE:
    /* 6AEE  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6AF0:
    /* 6AF0  idiv    dword ptr [di+8] */
    if (asm_idiv32(rd(pDS, DI + 0x8)) && (c = asm_divfault(0x065C, 0x6AF0, 4)) != 0) return c;
L6AF4: /* L6AF4 */
    /* 6AF4  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L6AF7:
    /* 6AF7  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6AFB:
    /* 6AFB  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6AFF:
    /* 6AFF  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6B01:
    /* 6B01  idiv    dword ptr ds:[0CFBCh] */
    if (asm_idiv32(rd(pDS, 0xCFBC)) && (c = asm_divfault(0x065C, 0x6B01, 5)) != 0) return c;
L6B06:
    /* 6B06  mov     dword ptr ds:[0CF9Ch],eax */
    wd(pDS, 0xCF9C, EAX);
L6B0A:
    /* 6B0A  mov     eax,dword ptr [di+1Ch] */
    EAX = rd(pDS, DI + 0x1C);
L6B0E:
    /* 6B0E  sub     eax,dword ptr [si+1Ch] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x1C));
L6B12:
    /* 6B12  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6B16:
    /* 6B16  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6B1A:
    /* 6B1A  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6B1C:
    /* 6B1C  idiv    dword ptr ds:[0CFBCh] */
    if (asm_idiv32(rd(pDS, 0xCFBC)) && (c = asm_divfault(0x065C, 0x6B1C, 5)) != 0) return c;
L6B21:
    /* 6B21  mov     dword ptr ds:[0CFA4h],eax */
    wd(pDS, 0xCFA4, EAX);
L6B25:
    /* 6B25  mov     eax,dword ptr [si+14h] */
    EAX = rd(pDS, SI + 0x14);
L6B29:
    /* 6B29  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6B2D:
    /* 6B2D  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6B31:
    /* 6B31  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6B33:
    /* 6B33  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x6B33, 4)) != 0) return c;
L6B37:
    /* 6B37  mov     ecx,eax */
    ECX = EAX;
L6B3A:
    /* 6B3A  mov     eax,dword ptr [di+14h] */
    EAX = rd(pDS, DI + 0x14);
L6B3E:
    /* 6B3E  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6B42:
    /* 6B42  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6B46:
    /* 6B46  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6B48:
    /* 6B48  idiv    dword ptr [di+8] */
    if (asm_idiv32(rd(pDS, DI + 0x8)) && (c = asm_divfault(0x065C, 0x6B48, 4)) != 0) return c;
L6B4C:
    /* 6B4C  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L6B4F:
    /* 6B4F  shl     eax,7 */
    EAX = (uint32_t)(EAX << 7);
L6B53:
    /* 6B53  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6B57:
    /* 6B57  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6B5B:
    /* 6B5B  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6B5D:
    /* 6B5D  idiv    dword ptr ds:[0CFBCh] */
    if (asm_idiv32(rd(pDS, 0xCFBC)) && (c = asm_divfault(0x065C, 0x6B5D, 5)) != 0) return c;
L6B62:
    /* 6B62  mov     dword ptr ds:[0CF88h],eax */
    wd(pDS, 0xCF88, EAX);
L6B66:
    /* 6B66  mov     eax,dword ptr [si+18h] */
    EAX = rd(pDS, SI + 0x18);
L6B6A:
    /* 6B6A  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6B6E:
    /* 6B6E  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6B72:
    /* 6B72  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6B74:
    /* 6B74  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x6B74, 4)) != 0) return c;
L6B78:
    /* 6B78  mov     ecx,eax */
    ECX = EAX;
L6B7B:
    /* 6B7B  mov     eax,dword ptr [di+18h] */
    EAX = rd(pDS, DI + 0x18);
L6B7F:
    /* 6B7F  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6B83:
    /* 6B83  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6B87:
    /* 6B87  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6B89:
    /* 6B89  idiv    dword ptr [di+8] */
    if (asm_idiv32(rd(pDS, DI + 0x8)) && (c = asm_divfault(0x065C, 0x6B89, 4)) != 0) return c;
L6B8D:
    /* 6B8D  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L6B90:
    /* 6B90  shl     eax,7 */
    EAX = (uint32_t)(EAX << 7);
L6B94:
    /* 6B94  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6B98:
    /* 6B98  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6B9C:
    /* 6B9C  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6B9E:
    /* 6B9E  idiv    dword ptr ds:[0CFBCh] */
    if (asm_idiv32(rd(pDS, 0xCFBC)) && (c = asm_divfault(0x065C, 0x6B9E, 5)) != 0) return c;
L6BA3:
    /* 6BA3  mov     dword ptr ds:[0CF8Ch],eax */
    wd(pDS, 0xCF8C, EAX);
L6BA7:
    /* 6BA7  mov     si,word ptr ds:[0CFC8h] */
    SI = rw(pDS, 0xCFC8);
L6BAB:
    /* 6BAB  mov     eax,dword ptr [si+1Ch] */
    EAX = rd(pDS, SI + 0x1C);
L6BAF:
    /* 6BAF  mov     dword ptr ds:[0CF80h],eax */
    wd(pDS, 0xCF80, EAX);
L6BB3:
    /* 6BB3  mov     eax,100h */
    EAX = 0x100;
L6BB9:
    /* 6BB9  cmp     dword ptr [si+8],eax */
    sub32(rd(pDS, SI + 0x8), EAX, 0);
L6BBD:
    /* 6BBD  ja      short L6BC7 */
    if (!CF && !ZF) goto L6BC7;
L6BBF:
    /* 6BBF  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L6BC5:
    /* 6BC5  jmp     short L6BDB */
    goto L6BDB;
L6BC7: /* L6BC7 */
    /* 6BC7  mov     eax,800000h */
    EAX = 0x800000;
L6BCD:
    /* 6BCD  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6BD1:
    /* 6BD1  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6BD5:
    /* 6BD5  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6BD7:
    /* 6BD7  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x6BD7, 4)) != 0) return c;
L6BDB: /* L6BDB */
    /* 6BDB  mov     dword ptr ds:[0CF78h],eax */
    wd(pDS, 0xCF78, EAX);
L6BDF:
    /* 6BDF  imul    dword ptr [si+14h] */
    imul32(rd(pDS, SI + 0x14));
L6BE3:
    /* 6BE3  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L6BE8:
    /* 6BE8  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L6BEA:
    /* 6BEA  mov     dword ptr ds:[0CF6Ch],eax */
    wd(pDS, 0xCF6C, EAX);
L6BEE:
    /* 6BEE  mov     eax,dword ptr [si+18h] */
    EAX = rd(pDS, SI + 0x18);
L6BF2:
    /* 6BF2  imul    dword ptr ds:[0CF78h] */
    imul32(rd(pDS, 0xCF78));
L6BF7:
    /* 6BF7  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L6BFC:
    /* 6BFC  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L6BFE:
    /* 6BFE  mov     dword ptr ds:[0CF70h],eax */
    wd(pDS, 0xCF70, EAX);
L6C02:
    /* 6C02  mov     di,word ptr ds:[0CFCAh] */
    DI = rw(pDS, 0xCFCA);
L6C06:
    /* 6C06  mov     eax,100h */
    EAX = 0x100;
L6C0C:
    /* 6C0C  cmp     dword ptr [si+8],eax */
    sub32(rd(pDS, SI + 0x8), EAX, 0);
L6C10:
    /* 6C10  ja      short L6C1A */
    if (!CF && !ZF) goto L6C1A;
L6C12:
    /* 6C12  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L6C18:
    /* 6C18  jmp     short L6C2E */
    goto L6C2E;
L6C1A: /* L6C1A */
    /* 6C1A  mov     eax,800000h */
    EAX = 0x800000;
L6C20:
    /* 6C20  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6C24:
    /* 6C24  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6C28:
    /* 6C28  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6C2A:
    /* 6C2A  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x6C2A, 4)) != 0) return c;
L6C2E: /* L6C2E */
    /* 6C2E  mov     ecx,eax */
    ECX = EAX;
L6C31:
    /* 6C31  mov     eax,100h */
    EAX = 0x100;
L6C37:
    /* 6C37  cmp     dword ptr [di+8],eax */
    sub32(rd(pDS, DI + 0x8), EAX, 0);
L6C3B:
    /* 6C3B  ja      short L6C45 */
    if (!CF && !ZF) goto L6C45;
L6C3D:
    /* 6C3D  mov     word ptr ds:[0CF44h],1 */
    ww(pDS, 0xCF44, 0x1);
L6C43:
    /* 6C43  jmp     short L6C59 */
    goto L6C59;
L6C45: /* L6C45 */
    /* 6C45  mov     eax,800000h */
    EAX = 0x800000;
L6C4B:
    /* 6C4B  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6C4F:
    /* 6C4F  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6C53:
    /* 6C53  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6C55:
    /* 6C55  idiv    dword ptr [di+8] */
    if (asm_idiv32(rd(pDS, DI + 0x8)) && (c = asm_divfault(0x065C, 0x6C55, 4)) != 0) return c;
L6C59: /* L6C59 */
    /* 6C59  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L6C5C:
    /* 6C5C  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6C60:
    /* 6C60  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6C64:
    /* 6C64  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6C66:
    /* 6C66  idiv    dword ptr ds:[0CFC0h] */
    if (asm_idiv32(rd(pDS, 0xCFC0)) && (c = asm_divfault(0x065C, 0x6C66, 5)) != 0) return c;
L6C6B:
    /* 6C6B  mov     dword ptr ds:[0CFA0h],eax */
    wd(pDS, 0xCFA0, EAX);
L6C6F:
    /* 6C6F  mov     eax,dword ptr [di+1Ch] */
    EAX = rd(pDS, DI + 0x1C);
L6C73:
    /* 6C73  sub     eax,dword ptr [si+1Ch] */
    EAX = (uint32_t)(EAX - rd(pDS, SI + 0x1C));
L6C77:
    /* 6C77  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6C7B:
    /* 6C7B  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6C7F:
    /* 6C7F  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6C81:
    /* 6C81  idiv    dword ptr ds:[0CFC0h] */
    if (asm_idiv32(rd(pDS, 0xCFC0)) && (c = asm_divfault(0x065C, 0x6C81, 5)) != 0) return c;
L6C86:
    /* 6C86  mov     dword ptr ds:[0CFA8h],eax */
    wd(pDS, 0xCFA8, EAX);
L6C8A:
    /* 6C8A  mov     eax,dword ptr [si+14h] */
    EAX = rd(pDS, SI + 0x14);
L6C8E:
    /* 6C8E  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6C92:
    /* 6C92  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6C96:
    /* 6C96  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6C98:
    /* 6C98  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x6C98, 4)) != 0) return c;
L6C9C:
    /* 6C9C  mov     ecx,eax */
    ECX = EAX;
L6C9F:
    /* 6C9F  mov     eax,dword ptr [di+14h] */
    EAX = rd(pDS, DI + 0x14);
L6CA3:
    /* 6CA3  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6CA7:
    /* 6CA7  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6CAB:
    /* 6CAB  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6CAD:
    /* 6CAD  idiv    dword ptr [di+8] */
    if (asm_idiv32(rd(pDS, DI + 0x8)) && (c = asm_divfault(0x065C, 0x6CAD, 4)) != 0) return c;
L6CB1:
    /* 6CB1  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L6CB4:
    /* 6CB4  shl     eax,7 */
    EAX = (uint32_t)(EAX << 7);
L6CB8:
    /* 6CB8  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6CBC:
    /* 6CBC  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6CC0:
    /* 6CC0  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6CC2:
    /* 6CC2  idiv    dword ptr ds:[0CFC0h] */
    if (asm_idiv32(rd(pDS, 0xCFC0)) && (c = asm_divfault(0x065C, 0x6CC2, 5)) != 0) return c;
L6CC7:
    /* 6CC7  mov     dword ptr ds:[0CF94h],eax */
    wd(pDS, 0xCF94, EAX);
L6CCB:
    /* 6CCB  mov     eax,dword ptr [si+18h] */
    EAX = rd(pDS, SI + 0x18);
L6CCF:
    /* 6CCF  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6CD3:
    /* 6CD3  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6CD7:
    /* 6CD7  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6CD9:
    /* 6CD9  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x6CD9, 4)) != 0) return c;
L6CDD:
    /* 6CDD  mov     ecx,eax */
    ECX = EAX;
L6CE0:
    /* 6CE0  mov     eax,dword ptr [di+18h] */
    EAX = rd(pDS, DI + 0x18);
L6CE4:
    /* 6CE4  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6CE8:
    /* 6CE8  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6CEC:
    /* 6CEC  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L6CEE:
    /* 6CEE  idiv    dword ptr [di+8] */
    if (asm_idiv32(rd(pDS, DI + 0x8)) && (c = asm_divfault(0x065C, 0x6CEE, 4)) != 0) return c;
L6CF2:
    /* 6CF2  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L6CF5:
    /* 6CF5  shl     eax,7 */
    EAX = (uint32_t)(EAX << 7);
L6CF9:
    /* 6CF9  rol     eax,10h */
    EAX = rol32(EAX, 16);
L6CFD:
    /* 6CFD  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L6D01:
    /* 6D01  sub     ax,ax */
    AX = sub16(AX, AX, 0);
L6D03:
    /* 6D03  idiv    dword ptr ds:[0CFC0h] */
    if (asm_idiv32(rd(pDS, 0xCFC0)) && (c = asm_divfault(0x065C, 0x6D03, 5)) != 0) return c;
L6D08:
    /* 6D08  mov     dword ptr ds:[0CF98h],eax */
    wd(pDS, 0xCF98, EAX);
L6D0C:
    /* 6D0C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L6D0D:
    /* 6D0D  neg     ax */
    AX = neg16(AX);
L6D0F:
    /* 6D0F  je      short L6D14 */
    if (ZF) goto L6D14;
L6D11:
    /* 6D11  cwde */
    EAX = (uint32_t)(int16_t)AX;
L6D13:
    /* 6D13  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L6D14: /* L6D14 */
    /* 6D14  mov     eax,10000h */
    EAX = 0x10000;
L6D1A:
    /* 6D1A  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
