/* grentry_x.c: replaces src/gfx/GRENTRY.ASM (seg003_0272_6E2, 06E2..0D1E of its
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

uint32_t asm_mod_GRENTRY(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x06E2: goto L06E2;
    case 0x06E4: goto L06E4;
    case 0x06E6: goto L06E6;
    case 0x06E7: goto L06E7;
    case 0x06EA: goto L06EA;
    case 0x06EC: goto L06EC;
    case 0x06EF: goto L06EF;
    case 0x06F1: goto L06F1;
    case 0x06F4: goto L06F4;
    case 0x06F6: goto L06F6;
    case 0x06F7: goto L06F7;
    case 0x06F8: goto L06F8;
    case 0x06F9: goto L06F9;
    case 0x06FA: goto L06FA;
    case 0x06FF: goto L06FF;
    case 0x0704: goto L0704;
    case 0x0706: goto L0706;
    case 0x0708: goto L0708;
    case 0x070A: goto L070A;
    case 0x070C: goto L070C;
    case 0x0711: goto L0711;
    case 0x0713: goto L0713;
    case 0x0715: goto L0715;
    case 0x0717: goto L0717;
    case 0x0719: goto L0719;
    case 0x071B: goto L071B;
    case 0x071C: goto L071C;
    case 0x071E: goto L071E;
    case 0x0720: goto L0720;
    case 0x0721: goto L0721;
    case 0x0723: goto L0723;
    case 0x0724: goto L0724;
    case 0x0726: goto L0726;
    case 0x0727: goto L0727;
    case 0x0728: goto L0728;
    case 0x0729: goto L0729;
    case 0x072A: goto L072A;
    case 0x072B: goto L072B;
    case 0x0730: goto L0730;
    case 0x0735: goto L0735;
    case 0x0737: goto L0737;
    case 0x0739: goto L0739;
    case 0x073B: goto L073B;
    case 0x073D: goto L073D;
    case 0x0742: goto L0742;
    case 0x0744: goto L0744;
    case 0x0746: goto L0746;
    case 0x0748: goto L0748;
    case 0x074A: goto L074A;
    case 0x074C: goto L074C;
    case 0x074D: goto L074D;
    case 0x074F: goto L074F;
    case 0x0751: goto L0751;
    case 0x0752: goto L0752;
    case 0x0754: goto L0754;
    case 0x0755: goto L0755;
    case 0x0757: goto L0757;
    case 0x0759: goto L0759;
    case 0x075A: goto L075A;
    case 0x075B: goto L075B;
    case 0x075C: goto L075C;
    case 0x075E: goto L075E;
    case 0x075F: goto L075F;
    case 0x0761: goto L0761;
    case 0x0762: goto L0762;
    case 0x0763: goto L0763;
    case 0x0764: goto L0764;
    case 0x0765: goto L0765;
    case 0x0766: goto L0766;
    case 0x0769: goto L0769;
    case 0x076B: goto L076B;
    case 0x076E: goto L076E;
    case 0x0770: goto L0770;
    case 0x0772: goto L0772;
    case 0x0774: goto L0774;
    case 0x0777: goto L0777;
    case 0x0779: goto L0779;
    case 0x077B: goto L077B;
    case 0x0780: goto L0780;
    case 0x0782: goto L0782;
    case 0x0783: goto L0783;
    case 0x0785: goto L0785;
    case 0x0786: goto L0786;
    case 0x0787: goto L0787;
    case 0x0788: goto L0788;
    case 0x0789: goto L0789;
    case 0x078C: goto L078C;
    case 0x078E: goto L078E;
    case 0x0791: goto L0791;
    case 0x0793: goto L0793;
    case 0x0795: goto L0795;
    case 0x079A: goto L079A;
    case 0x079C: goto L079C;
    case 0x079D: goto L079D;
    case 0x079F: goto L079F;
    case 0x07A0: goto L07A0;
    case 0x07A2: goto L07A2;
    case 0x07A3: goto L07A3;
    case 0x07A8: goto L07A8;
    case 0x07AB: goto L07AB;
    case 0x07AE: goto L07AE;
    case 0x07B0: goto L07B0;
    case 0x07B2: goto L07B2;
    case 0x07B5: goto L07B5;
    case 0x07B6: goto L07B6;
    case 0x07B8: goto L07B8;
    case 0x07BA: goto L07BA;
    case 0x07BD: goto L07BD;
    case 0x07C0: goto L07C0;
    case 0x07C2: goto L07C2;
    case 0x07C3: goto L07C3;
    case 0x07C7: goto L07C7;
    case 0x07C9: goto L07C9;
    case 0x07CD: goto L07CD;
    case 0x07CF: goto L07CF;
    case 0x07D5: goto L07D5;
    case 0x07D7: goto L07D7;
    case 0x07DA: goto L07DA;
    case 0x07DE: goto L07DE;
    case 0x07E0: goto L07E0;
    case 0x07E4: goto L07E4;
    case 0x07E7: goto L07E7;
    case 0x07EB: goto L07EB;
    case 0x07ED: goto L07ED;
    case 0x07F0: goto L07F0;
    case 0x07F4: goto L07F4;
    case 0x07F8: goto L07F8;
    case 0x07F9: goto L07F9;
    case 0x07FA: goto L07FA;
    case 0x07FB: goto L07FB;
    case 0x07FC: goto L07FC;
    case 0x07FD: goto L07FD;
    case 0x07FE: goto L07FE;
    case 0x0801: goto L0801;
    case 0x0803: goto L0803;
    case 0x0805: goto L0805;
    case 0x0807: goto L0807;
    case 0x080C: goto L080C;
    case 0x0811: goto L0811;
    case 0x0812: goto L0812;
    case 0x0814: goto L0814;
    case 0x0818: goto L0818;
    case 0x0819: goto L0819;
    case 0x081C: goto L081C;
    case 0x081E: goto L081E;
    case 0x0822: goto L0822;
    case 0x0825: goto L0825;
    case 0x0829: goto L0829;
    case 0x082C: goto L082C;
    case 0x082E: goto L082E;
    case 0x0832: goto L0832;
    case 0x0834: goto L0834;
    case 0x0838: goto L0838;
    case 0x083A: goto L083A;
    case 0x083B: goto L083B;
    case 0x083D: goto L083D;
    case 0x0840: goto L0840;
    case 0x0842: goto L0842;
    case 0x0845: goto L0845;
    case 0x0847: goto L0847;
    case 0x0848: goto L0848;
    case 0x084A: goto L084A;
    case 0x084D: goto L084D;
    case 0x084F: goto L084F;
    case 0x0852: goto L0852;
    case 0x0854: goto L0854;
    case 0x0855: goto L0855;
    case 0x0857: goto L0857;
    case 0x085A: goto L085A;
    case 0x085C: goto L085C;
    case 0x085F: goto L085F;
    case 0x0861: goto L0861;
    case 0x0862: goto L0862;
    case 0x0864: goto L0864;
    case 0x0866: goto L0866;
    case 0x0869: goto L0869;
    case 0x086C: goto L086C;
    case 0x086E: goto L086E;
    case 0x086F: goto L086F;
    case 0x0874: goto L0874;
    case 0x0876: goto L0876;
    case 0x0877: goto L0877;
    case 0x087C: goto L087C;
    case 0x0881: goto L0881;
    case 0x0882: goto L0882;
    case 0x0883: goto L0883;
    case 0x0884: goto L0884;
    case 0x0885: goto L0885;
    case 0x0886: goto L0886;
    case 0x0887: goto L0887;
    case 0x0888: goto L0888;
    case 0x0889: goto L0889;
    case 0x088C: goto L088C;
    case 0x088D: goto L088D;
    case 0x0890: goto L0890;
    case 0x0891: goto L0891;
    case 0x0894: goto L0894;
    case 0x0895: goto L0895;
    case 0x0898: goto L0898;
    case 0x0899: goto L0899;
    case 0x089C: goto L089C;
    case 0x089D: goto L089D;
    case 0x08A0: goto L08A0;
    case 0x08A1: goto L08A1;
    case 0x08A4: goto L08A4;
    case 0x08A5: goto L08A5;
    case 0x08A8: goto L08A8;
    case 0x08A9: goto L08A9;
    case 0x08AC: goto L08AC;
    case 0x08AD: goto L08AD;
    case 0x08B0: goto L08B0;
    case 0x08B1: goto L08B1;
    case 0x08B4: goto L08B4;
    case 0x08B5: goto L08B5;
    case 0x08B8: goto L08B8;
    case 0x08B9: goto L08B9;
    case 0x08BC: goto L08BC;
    case 0x08BD: goto L08BD;
    case 0x08C0: goto L08C0;
    case 0x08C1: goto L08C1;
    case 0x08C4: goto L08C4;
    case 0x08C5: goto L08C5;
    case 0x08C8: goto L08C8;
    case 0x08C9: goto L08C9;
    case 0x08CC: goto L08CC;
    case 0x08CD: goto L08CD;
    case 0x08D0: goto L08D0;
    case 0x08D1: goto L08D1;
    case 0x08D4: goto L08D4;
    case 0x08D5: goto L08D5;
    case 0x08D8: goto L08D8;
    case 0x08D9: goto L08D9;
    case 0x08DC: goto L08DC;
    case 0x08DD: goto L08DD;
    case 0x08E0: goto L08E0;
    case 0x08E1: goto L08E1;
    case 0x08E4: goto L08E4;
    case 0x08E5: goto L08E5;
    case 0x08E8: goto L08E8;
    case 0x08E9: goto L08E9;
    case 0x08EC: goto L08EC;
    case 0x08ED: goto L08ED;
    case 0x08F0: goto L08F0;
    case 0x08F1: goto L08F1;
    case 0x08F4: goto L08F4;
    case 0x08F5: goto L08F5;
    case 0x08F8: goto L08F8;
    case 0x08F9: goto L08F9;
    case 0x08FC: goto L08FC;
    case 0x08FD: goto L08FD;
    case 0x0900: goto L0900;
    case 0x0901: goto L0901;
    case 0x0904: goto L0904;
    case 0x0905: goto L0905;
    case 0x0908: goto L0908;
    case 0x0909: goto L0909;
    case 0x090C: goto L090C;
    case 0x090D: goto L090D;
    case 0x0910: goto L0910;
    case 0x0911: goto L0911;
    case 0x0914: goto L0914;
    case 0x0915: goto L0915;
    case 0x0918: goto L0918;
    case 0x0919: goto L0919;
    case 0x091C: goto L091C;
    case 0x091D: goto L091D;
    case 0x0920: goto L0920;
    case 0x0921: goto L0921;
    case 0x0924: goto L0924;
    case 0x0925: goto L0925;
    case 0x0928: goto L0928;
    case 0x0929: goto L0929;
    case 0x092C: goto L092C;
    case 0x092D: goto L092D;
    case 0x0930: goto L0930;
    case 0x0931: goto L0931;
    case 0x0934: goto L0934;
    case 0x0935: goto L0935;
    case 0x0938: goto L0938;
    case 0x0939: goto L0939;
    case 0x093C: goto L093C;
    case 0x093D: goto L093D;
    case 0x0940: goto L0940;
    case 0x0941: goto L0941;
    case 0x0944: goto L0944;
    case 0x0945: goto L0945;
    case 0x0948: goto L0948;
    case 0x0949: goto L0949;
    case 0x094C: goto L094C;
    case 0x094D: goto L094D;
    case 0x0950: goto L0950;
    case 0x0951: goto L0951;
    case 0x0954: goto L0954;
    case 0x0955: goto L0955;
    case 0x0958: goto L0958;
    case 0x0959: goto L0959;
    case 0x095C: goto L095C;
    case 0x095D: goto L095D;
    case 0x0960: goto L0960;
    case 0x0961: goto L0961;
    case 0x0964: goto L0964;
    case 0x0965: goto L0965;
    case 0x0968: goto L0968;
    case 0x0969: goto L0969;
    case 0x096C: goto L096C;
    case 0x096D: goto L096D;
    case 0x0970: goto L0970;
    case 0x0971: goto L0971;
    case 0x0974: goto L0974;
    case 0x0975: goto L0975;
    case 0x0978: goto L0978;
    case 0x0979: goto L0979;
    case 0x097C: goto L097C;
    case 0x097D: goto L097D;
    case 0x0980: goto L0980;
    case 0x0981: goto L0981;
    case 0x0984: goto L0984;
    case 0x0985: goto L0985;
    case 0x0988: goto L0988;
    case 0x0989: goto L0989;
    case 0x098C: goto L098C;
    case 0x098D: goto L098D;
    case 0x0990: goto L0990;
    case 0x0991: goto L0991;
    case 0x0994: goto L0994;
    case 0x0995: goto L0995;
    case 0x0998: goto L0998;
    case 0x0999: goto L0999;
    case 0x099C: goto L099C;
    case 0x099D: goto L099D;
    case 0x09A0: goto L09A0;
    case 0x09A1: goto L09A1;
    case 0x09A4: goto L09A4;
    case 0x09A5: goto L09A5;
    case 0x09A8: goto L09A8;
    case 0x09A9: goto L09A9;
    case 0x09AC: goto L09AC;
    case 0x09AD: goto L09AD;
    case 0x09B0: goto L09B0;
    case 0x09B1: goto L09B1;
    case 0x09B4: goto L09B4;
    case 0x09B5: goto L09B5;
    case 0x09B8: goto L09B8;
    case 0x09B9: goto L09B9;
    case 0x09BC: goto L09BC;
    case 0x09BD: goto L09BD;
    case 0x09C0: goto L09C0;
    case 0x09C1: goto L09C1;
    case 0x09C4: goto L09C4;
    case 0x09C5: goto L09C5;
    case 0x09C8: goto L09C8;
    case 0x09C9: goto L09C9;
    case 0x09CA: goto L09CA;
    case 0x09CE: goto L09CE;
    case 0x09D2: goto L09D2;
    case 0x09D3: goto L09D3;
    case 0x09D5: goto L09D5;
    case 0x09D7: goto L09D7;
    case 0x09D9: goto L09D9;
    case 0x09DD: goto L09DD;
    case 0x09DE: goto L09DE;
    case 0x09E0: goto L09E0;
    case 0x09E2: goto L09E2;
    case 0x09E3: goto L09E3;
    case 0x09E5: goto L09E5;
    case 0x09E7: goto L09E7;
    case 0x09EA: goto L09EA;
    case 0x09EB: goto L09EB;
    case 0x09ED: goto L09ED;
    case 0x09EF: goto L09EF;
    case 0x09F1: goto L09F1;
    case 0x09F2: goto L09F2;
    case 0x09F3: goto L09F3;
    case 0x09F5: goto L09F5;
    case 0x09F6: goto L09F6;
    case 0x09F8: goto L09F8;
    case 0x09FA: goto L09FA;
    case 0x09FC: goto L09FC;
    case 0x09FF: goto L09FF;
    case 0x0A01: goto L0A01;
    case 0x0A05: goto L0A05;
    case 0x0A06: goto L0A06;
    case 0x0A07: goto L0A07;
    case 0x0A0B: goto L0A0B;
    case 0x0A0F: goto L0A0F;
    case 0x0A12: goto L0A12;
    case 0x0A14: goto L0A14;
    case 0x0A16: goto L0A16;
    case 0x0A19: goto L0A19;
    case 0x0A1C: goto L0A1C;
    case 0x0A1F: goto L0A1F;
    case 0x0A22: goto L0A22;
    case 0x0A25: goto L0A25;
    case 0x0A28: goto L0A28;
    case 0x0A2A: goto L0A2A;
    case 0x0A2F: goto L0A2F;
    case 0x0A31: goto L0A31;
    case 0x0A32: goto L0A32;
    case 0x0A35: goto L0A35;
    case 0x0A37: goto L0A37;
    case 0x0A39: goto L0A39;
    case 0x0A3A: goto L0A3A;
    case 0x0A3B: goto L0A3B;
    case 0x0A3E: goto L0A3E;
    case 0x0A40: goto L0A40;
    case 0x0A42: goto L0A42;
    case 0x0A43: goto L0A43;
    case 0x0A45: goto L0A45;
    case 0x0A48: goto L0A48;
    case 0x0A4A: goto L0A4A;
    case 0x0A4C: goto L0A4C;
    case 0x0A4D: goto L0A4D;
    case 0x0A4E: goto L0A4E;
    case 0x0A51: goto L0A51;
    case 0x0A53: goto L0A53;
    case 0x0A55: goto L0A55;
    case 0x0A56: goto L0A56;
    case 0x0A58: goto L0A58;
    case 0x0A5B: goto L0A5B;
    case 0x0A5D: goto L0A5D;
    case 0x0A5F: goto L0A5F;
    case 0x0A60: goto L0A60;
    case 0x0A61: goto L0A61;
    case 0x0A64: goto L0A64;
    case 0x0A66: goto L0A66;
    case 0x0A68: goto L0A68;
    case 0x0A69: goto L0A69;
    case 0x0A6B: goto L0A6B;
    case 0x0A6E: goto L0A6E;
    case 0x0A70: goto L0A70;
    case 0x0A72: goto L0A72;
    case 0x0A73: goto L0A73;
    case 0x0A74: goto L0A74;
    case 0x0A77: goto L0A77;
    case 0x0A79: goto L0A79;
    case 0x0A7A: goto L0A7A;
    case 0x0A7C: goto L0A7C;
    case 0x0A7D: goto L0A7D;
    case 0x0A7E: goto L0A7E;
    case 0x0A7F: goto L0A7F;
    case 0x0A80: goto L0A80;
    case 0x0A81: goto L0A81;
    case 0x0A85: goto L0A85;
    case 0x0A89: goto L0A89;
    case 0x0A8B: goto L0A8B;
    case 0x0A8E: goto L0A8E;
    case 0x0A90: goto L0A90;
    case 0x0A92: goto L0A92;
    case 0x0A95: goto L0A95;
    case 0x0A9A: goto L0A9A;
    case 0x0A9D: goto L0A9D;
    case 0x0AA0: goto L0AA0;
    case 0x0AA3: goto L0AA3;
    case 0x0AA5: goto L0AA5;
    case 0x0AA8: goto L0AA8;
    case 0x0AA9: goto L0AA9;
    case 0x0AAB: goto L0AAB;
    case 0x0AAC: goto L0AAC;
    case 0x0AAE: goto L0AAE;
    case 0x0AB1: goto L0AB1;
    case 0x0AB6: goto L0AB6;
    case 0x0ABB: goto L0ABB;
    case 0x0ABE: goto L0ABE;
    case 0x0AC1: goto L0AC1;
    case 0x0AC2: goto L0AC2;
    case 0x0AC3: goto L0AC3;
    case 0x0AC6: goto L0AC6;
    case 0x0AC8: goto L0AC8;
    case 0x0ACA: goto L0ACA;
    case 0x0ACD: goto L0ACD;
    case 0x0AD0: goto L0AD0;
    case 0x0AD2: goto L0AD2;
    case 0x0AD5: goto L0AD5;
    case 0x0AD7: goto L0AD7;
    case 0x0AD8: goto L0AD8;
    case 0x0AD9: goto L0AD9;
    case 0x0ADB: goto L0ADB;
    case 0x0ADE: goto L0ADE;
    case 0x0AE0: goto L0AE0;
    case 0x0AE1: goto L0AE1;
    case 0x0AE2: goto L0AE2;
    case 0x0AE4: goto L0AE4;
    case 0x0AE7: goto L0AE7;
    case 0x0AE9: goto L0AE9;
    case 0x0AEE: goto L0AEE;
    case 0x0AF3: goto L0AF3;
    case 0x0AF6: goto L0AF6;
    case 0x0AF9: goto L0AF9;
    case 0x0AFA: goto L0AFA;
    case 0x0AFB: goto L0AFB;
    case 0x0AFE: goto L0AFE;
    case 0x0B00: goto L0B00;
    case 0x0B02: goto L0B02;
    case 0x0B05: goto L0B05;
    case 0x0B08: goto L0B08;
    case 0x0B0A: goto L0B0A;
    case 0x0B0D: goto L0B0D;
    case 0x0B0F: goto L0B0F;
    case 0x0B10: goto L0B10;
    case 0x0B11: goto L0B11;
    case 0x0B13: goto L0B13;
    case 0x0B16: goto L0B16;
    case 0x0B18: goto L0B18;
    case 0x0B19: goto L0B19;
    case 0x0B1A: goto L0B1A;
    case 0x0B1C: goto L0B1C;
    case 0x0B1E: goto L0B1E;
    case 0x0B23: goto L0B23;
    case 0x0B28: goto L0B28;
    case 0x0B2B: goto L0B2B;
    case 0x0B2E: goto L0B2E;
    case 0x0B2F: goto L0B2F;
    case 0x0B30: goto L0B30;
    case 0x0B33: goto L0B33;
    case 0x0B35: goto L0B35;
    case 0x0B37: goto L0B37;
    case 0x0B3A: goto L0B3A;
    case 0x0B3D: goto L0B3D;
    case 0x0B3F: goto L0B3F;
    case 0x0B42: goto L0B42;
    case 0x0B44: goto L0B44;
    case 0x0B45: goto L0B45;
    case 0x0B46: goto L0B46;
    case 0x0B48: goto L0B48;
    case 0x0B4B: goto L0B4B;
    case 0x0B4D: goto L0B4D;
    case 0x0B4E: goto L0B4E;
    case 0x0B4F: goto L0B4F;
    case 0x0B51: goto L0B51;
    case 0x0B53: goto L0B53;
    case 0x0B58: goto L0B58;
    case 0x0B5D: goto L0B5D;
    case 0x0B60: goto L0B60;
    case 0x0B63: goto L0B63;
    case 0x0B64: goto L0B64;
    case 0x0B65: goto L0B65;
    case 0x0B68: goto L0B68;
    case 0x0B6A: goto L0B6A;
    case 0x0B6C: goto L0B6C;
    case 0x0B6F: goto L0B6F;
    case 0x0B72: goto L0B72;
    case 0x0B74: goto L0B74;
    case 0x0B77: goto L0B77;
    case 0x0B79: goto L0B79;
    case 0x0B7A: goto L0B7A;
    case 0x0B7B: goto L0B7B;
    case 0x0B7D: goto L0B7D;
    case 0x0B80: goto L0B80;
    case 0x0B82: goto L0B82;
    case 0x0B83: goto L0B83;
    case 0x0B84: goto L0B84;
    case 0x0B86: goto L0B86;
    case 0x0B88: goto L0B88;
    case 0x0B89: goto L0B89;
    case 0x0B8C: goto L0B8C;
    case 0x0B8F: goto L0B8F;
    case 0x0B92: goto L0B92;
    case 0x0B93: goto L0B93;
    case 0x0B94: goto L0B94;
    case 0x0B95: goto L0B95;
    case 0x0B96: goto L0B96;
    case 0x0B99: goto L0B99;
    case 0x0B9B: goto L0B9B;
    case 0x0B9D: goto L0B9D;
    case 0x0B9F: goto L0B9F;
    case 0x0BA1: goto L0BA1;
    case 0x0BA5: goto L0BA5;
    case 0x0BA8: goto L0BA8;
    case 0x0BAC: goto L0BAC;
    case 0x0BB3: goto L0BB3;
    case 0x0BB6: goto L0BB6;
    case 0x0BB8: goto L0BB8;
    case 0x0BBC: goto L0BBC;
    case 0x0BBF: goto L0BBF;
    case 0x0BC0: goto L0BC0;
    case 0x0BC7: goto L0BC7;
    case 0x0BC8: goto L0BC8;
    case 0x0BC9: goto L0BC9;
    case 0x0BCC: goto L0BCC;
    case 0x0BCE: goto L0BCE;
    case 0x0BD2: goto L0BD2;
    case 0x0BD6: goto L0BD6;
    case 0x0BD8: goto L0BD8;
    case 0x0BDD: goto L0BDD;
    case 0x0BE0: goto L0BE0;
    case 0x0BE4: goto L0BE4;
    case 0x0BE7: goto L0BE7;
    case 0x0BEA: goto L0BEA;
    case 0x0BEB: goto L0BEB;
    case 0x0BEC: goto L0BEC;
    case 0x0BF0: goto L0BF0;
    case 0x0BF2: goto L0BF2;
    case 0x0BF3: goto L0BF3;
    case 0x0BF4: goto L0BF4;
    case 0x0BF9: goto L0BF9;
    case 0x0BFE: goto L0BFE;
    case 0x0C01: goto L0C01;
    case 0x0C03: goto L0C03;
    case 0x0C05: goto L0C05;
    case 0x0C07: goto L0C07;
    case 0x0C0C: goto L0C0C;
    case 0x0C0E: goto L0C0E;
    case 0x0C0F: goto L0C0F;
    case 0x0C11: goto L0C11;
    case 0x0C12: goto L0C12;
    case 0x0C13: goto L0C13;
    case 0x0C14: goto L0C14;
    case 0x0C18: goto L0C18;
    case 0x0C1A: goto L0C1A;
    case 0x0C1C: goto L0C1C;
    case 0x0C1E: goto L0C1E;
    case 0x0C23: goto L0C23;
    case 0x0C24: goto L0C24;
    case 0x0C25: goto L0C25;
    case 0x0C26: goto L0C26;
    case 0x0C27: goto L0C27;
    case 0x0C28: goto L0C28;
    case 0x0C29: goto L0C29;
    case 0x0C2A: goto L0C2A;
    case 0x0C2C: goto L0C2C;
    case 0x0C2E: goto L0C2E;
    case 0x0C30: goto L0C30;
    case 0x0C32: goto L0C32;
    case 0x0C34: goto L0C34;
    case 0x0C35: goto L0C35;
    case 0x0C3A: goto L0C3A;
    case 0x0C3F: goto L0C3F;
    case 0x0C41: goto L0C41;
    case 0x0C42: goto L0C42;
    case 0x0C44: goto L0C44;
    case 0x0C46: goto L0C46;
    case 0x0C4B: goto L0C4B;
    case 0x0C4D: goto L0C4D;
    case 0x0C4F: goto L0C4F;
    case 0x0C51: goto L0C51;
    case 0x0C53: goto L0C53;
    case 0x0C55: goto L0C55;
    case 0x0C57: goto L0C57;
    case 0x0C59: goto L0C59;
    case 0x0C5B: goto L0C5B;
    case 0x0C5C: goto L0C5C;
    case 0x0C5E: goto L0C5E;
    case 0x0C60: goto L0C60;
    case 0x0C62: goto L0C62;
    case 0x0C64: goto L0C64;
    case 0x0C66: goto L0C66;
    case 0x0C68: goto L0C68;
    case 0x0C6A: goto L0C6A;
    case 0x0C6E: goto L0C6E;
    case 0x0C70: goto L0C70;
    case 0x0C72: goto L0C72;
    case 0x0C74: goto L0C74;
    case 0x0C75: goto L0C75;
    case 0x0C77: goto L0C77;
    case 0x0C79: goto L0C79;
    case 0x0C7B: goto L0C7B;
    case 0x0C7D: goto L0C7D;
    case 0x0C7F: goto L0C7F;
    case 0x0C81: goto L0C81;
    case 0x0C86: goto L0C86;
    case 0x0C88: goto L0C88;
    case 0x0C89: goto L0C89;
    case 0x0C8E: goto L0C8E;
    case 0x0C93: goto L0C93;
    case 0x0C94: goto L0C94;
    case 0x0C95: goto L0C95;
    case 0x0C97: goto L0C97;
    case 0x0C99: goto L0C99;
    case 0x0C9B: goto L0C9B;
    case 0x0C9D: goto L0C9D;
    case 0x0CA0: goto L0CA0;
    case 0x0CA3: goto L0CA3;
    case 0x0CA8: goto L0CA8;
    case 0x0CAA: goto L0CAA;
    case 0x0CAC: goto L0CAC;
    case 0x0CAF: goto L0CAF;
    case 0x0CB1: goto L0CB1;
    case 0x0CB6: goto L0CB6;
    case 0x0CB9: goto L0CB9;
    case 0x0CBB: goto L0CBB;
    case 0x0CBE: goto L0CBE;
    case 0x0CC0: goto L0CC0;
    case 0x0CC2: goto L0CC2;
    case 0x0CC3: goto L0CC3;
    case 0x0CC4: goto L0CC4;
    case 0x0CC6: goto L0CC6;
    case 0x0CC8: goto L0CC8;
    case 0x0CCB: goto L0CCB;
    case 0x0CCD: goto L0CCD;
    case 0x0CCF: goto L0CCF;
    case 0x0CD0: goto L0CD0;
    case 0x0CD1: goto L0CD1;
    case 0x0CD3: goto L0CD3;
    case 0x0CD4: goto L0CD4;
    case 0x0CD5: goto L0CD5;
    case 0x0CD6: goto L0CD6;
    case 0x0CD9: goto L0CD9;
    case 0x0CDA: goto L0CDA;
    case 0x0CDC: goto L0CDC;
    case 0x0CDE: goto L0CDE;
    case 0x0CE0: goto L0CE0;
    case 0x0CE3: goto L0CE3;
    case 0x0CE6: goto L0CE6;
    case 0x0CEB: goto L0CEB;
    case 0x0CED: goto L0CED;
    case 0x0CEF: goto L0CEF;
    case 0x0CF1: goto L0CF1;
    case 0x0CF2: goto L0CF2;
    case 0x0CF5: goto L0CF5;
    case 0x0CF7: goto L0CF7;
    case 0x0CFA: goto L0CFA;
    case 0x0CFC: goto L0CFC;
    case 0x0CFE: goto L0CFE;
    case 0x0D01: goto L0D01;
    case 0x0D03: goto L0D03;
    case 0x0D05: goto L0D05;
    case 0x0D06: goto L0D06;
    case 0x0D07: goto L0D07;
    case 0x0D09: goto L0D09;
    case 0x0D0B: goto L0D0B;
    case 0x0D0D: goto L0D0D;
    case 0x0D10: goto L0D10;
    case 0x0D12: goto L0D12;
    case 0x0D14: goto L0D14;
    case 0x0D15: goto L0D15;
    case 0x0D16: goto L0D16;
    case 0x0D18: goto L0D18;
    case 0x0D19: goto L0D19;
    case 0x0D1A: goto L0D1A;
    case 0x0D1B: goto L0D1B;
    default: asm_bad_entry("GRENTRY.ASM", entry);
    }

    /* seg003_0272_6E2  (+6E2)
       clear_fbuf (FM Towns): fill the frame buffer with colour 0 (falls into _6E4). */
L06E2: /* _seg003_0272_6E2 */
    /* 06E2  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);

    /* seg003_0272_6E4  (+6E4)
       fill_fbuf (far, cFillFB): AL = the colour. Fills 34E9h words of the frame buffer segment from
       offset 2.

       The two labels inside are span transfers into the frame buffer for VIDMODE.ASM's fbshow: _6F8
       (fbuf_draw_ylrpp) copies linear bitmap rows (8-byte records: y, left, right, source offset;
       source segment in 55EC) into the frame buffer, and _729 (fbuf_draw_ylrpp_x) does the same
       skipping colour 0. */
L06E4: /* _seg003_0272_6E4 */
    /* 06E4  mov     ah,al */
    AH = AL;
L06E6:
    /* 06E6  push    es */
    push16(asm_es);
L06E7:
    /* 06E7  mov     cx,seg seg049_3EE2 */
    CX = (uint16_t)(0x3CF5 + PORT_LOAD_SEG);
L06EA:
    /* 06EA  mov     es,cx */
    SET_ES(CX);
L06EC:
    /* 06EC  mov     cx,34E9h */
    CX = 0x34E9;
L06EF:
    /* 06EF  xor     di,di */
    DI = (uint16_t)(DI ^ DI);
L06F1:
    /* 06F1  add     di,2 */
    DI = add16(DI, 0x2, 0);
L06F4:
    /* 06F4  rep stosw */
    while (CX) { ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2)); CX--; }
L06F6:
    /* 06F6  pop     es */
    SET_ES(pop16());
L06F7:
    /* 06F7  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;
L06F8: /* _seg003_0272_6F8 */
    /* 06F8  push    es */
    push16(asm_es);
L06F9:
    /* 06F9  push    ds */
    push16(asm_ds);
L06FA:
    /* 06FA  mov     es,word ptr ss:[958h] */
    SET_ES(rw(pSS, 0x958));
L06FF:
    /* 06FF  mov     ds,word ptr ss:[55ECh] */
    SET_DS(rw(pSS, 0x55EC));
L0704: /* L0704 */
    /* 0704  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0706:
    /* 0706  shl     ax,1 */
    AX = shl16(AX, 1);
L0708:
    /* 0708  jb      L0726 */
    if (CF) goto L0726;
L070A:
    /* 070A  mov     bx,ax */
    BX = AX;
L070C:
    /* 070C  mov     di,word ptr ss:[bx+95Ch] */
    DI = rw(pSS, BX + 0x95C);
L0711:
    /* 0711  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0713:
    /* 0713  add     di,ax */
    DI = (uint16_t)(DI + AX);
L0715:
    /* 0715  mov     cx,ax */
    CX = AX;
L0717:
    /* 0717  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0719:
    /* 0719  sub     ax,cx */
    AX = (uint16_t)(AX - CX);
L071B:
    /* 071B  inc     ax */
    AX = (uint16_t)(AX + 1);
L071C:
    /* 071C  mov     cx,ax */
    CX = AX;
L071E:
    /* 071E  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0720:
    /* 0720  xchg    si,ax */
    { uint16_t t_ = SI;
    SI = AX;
    AX = t_; }
L0721:
    /* 0721  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L0723:
    /* 0723  xchg    si,ax */
    { uint16_t t_ = SI;
    SI = AX;
    AX = t_; }
L0724:
    /* 0724  jmp     L0704 */
    goto L0704;
L0726: /* L0726 */
    /* 0726  pop     ds */
    SET_DS(pop16());
L0727:
    /* 0727  pop     es */
    SET_ES(pop16());
L0728:
    /* 0728  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0729: /* _seg003_0272_729 */
    /* 0729  push    es */
    push16(asm_es);
L072A:
    /* 072A  push    ds */
    push16(asm_ds);
L072B:
    /* 072B  mov     es,word ptr ss:[958h] */
    SET_ES(rw(pSS, 0x958));
L0730:
    /* 0730  mov     ds,word ptr ss:[55ECh] */
    SET_DS(rw(pSS, 0x55EC));
L0735: /* L0735 */
    /* 0735  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0737:
    /* 0737  shl     ax,1 */
    AX = shl16(AX, 1);
L0739:
    /* 0739  jb      L0761 */
    if (CF) goto L0761;
L073B:
    /* 073B  mov     bx,ax */
    BX = AX;
L073D:
    /* 073D  mov     di,word ptr ss:[bx+95Ch] */
    DI = rw(pSS, BX + 0x95C);
L0742:
    /* 0742  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0744:
    /* 0744  add     di,ax */
    DI = (uint16_t)(DI + AX);
L0746:
    /* 0746  mov     cx,ax */
    CX = AX;
L0748:
    /* 0748  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L074A:
    /* 074A  sub     ax,cx */
    AX = (uint16_t)(AX - CX);
L074C:
    /* 074C  inc     ax */
    AX = (uint16_t)(AX + 1);
L074D:
    /* 074D  mov     cx,ax */
    CX = AX;
L074F:
    /* 074F  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0751:
    /* 0751  push    si */
    push16(SI);
L0752:
    /* 0752  mov     si,ax */
    SI = AX;
L0754: /* L0754 */
    /* 0754  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0755:
    /* 0755  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L0757:
    /* 0757  je      L075B */
    if (ZF) goto L075B;
L0759:
    /* 0759  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L075A:
    /* 075A  dec     di */
    DI = (uint16_t)(DI - 1);
L075B: /* L075B */
    /* 075B  inc     di */
    DI = (uint16_t)(DI + 1);
L075C:
    /* 075C  loop    L0754 */
    if (--CX) goto L0754;
L075E:
    /* 075E  pop     si */
    SI = pop16();
L075F:
    /* 075F  jmp     L0735 */
    goto L0735;
L0761: /* L0761 */
    /* 0761  pop     ds */
    SET_DS(pop16());
L0762:
    /* 0762  pop     es */
    SET_ES(pop16());
L0763:
    /* 0763  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_764  (+764)
       cDimFB (FM Towns; far): remap every frame-buffer pixel through row CL of lightabs
       (seg004:6D3E, PGCACHE.ASM), so the whole view darkens by that distance shade (CH and CL are
       swapped to index by row). */
L0764: /* _seg003_0272_764 */
    /* 0764  push    ds */
    push16(asm_ds);
L0765:
    /* 0765  push    es */
    push16(asm_es);
L0766:
    /* 0766  mov     bx,seg seg049_3EE2 */
    BX = (uint16_t)(0x3CF5 + PORT_LOAD_SEG);
L0769:
    /* 0769  mov     ds,bx */
    SET_DS(BX);
L076B:
    /* 076B  mov     bx,seg seg004_0849 */
    BX = (uint16_t)(0x065C + PORT_LOAD_SEG);
L076E:
    /* 076E  mov     es,bx */
    SET_ES(BX);
L0770:
    /* 0770  xchg    ch,cl */
    { uint8_t t_ = CL;
    CL = CH;
    CH = t_; }
L0772:
    /* 0772  mov     si,cx */
    SI = CX;
L0774:
    /* 0774  mov     di,6AA6h */
    DI = 0x6AA6;
L0777:
    /* 0777  xor     bx,bx */
    BX = logic16((uint16_t)(BX ^ BX));
L0779: /* L0779 */
    /* 0779  mov     bl,byte ptr [di] */
    BL = rb(pDS, DI);
L077B:
    /* 077B  mov     bl,byte ptr es:[bx+si+6D3Eh] */
    BL = rb(pES, BX + SI + 0x6D3E);
L0780:
    /* 0780  mov     byte ptr [di],bl */
    wb(pDS, DI, BL);
L0782:
    /* 0782  dec     di */
    DI = dec16(DI);
L0783:
    /* 0783  jne     L0779 */
    if (!ZF) goto L0779;
L0785:
    /* 0785  pop     es */
    SET_ES(pop16());
L0786:
    /* 0786  pop     ds */
    SET_DS(pop16());
L0787:
    /* 0787  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_0272_788  (+788)
       cLiteFB (FM Towns; far): remap every frame-buffer pixel through the colour table at cXfer+300h
       (SCALEBM.ASM), CX times. L07A4 and L07A6 hold the caller's stack for cFBtoScreen. */
L0788: /* _seg003_0272_788 */
    /* 0788  push    ds */
    push16(asm_ds);
L0789:
    /* 0789  mov     bx,seg seg049_3EE2 */
    BX = (uint16_t)(0x3CF5 + PORT_LOAD_SEG);
L078C:
    /* 078C  mov     ds,bx */
    SET_DS(BX);
L078E: /* L078E */
    /* 078E  mov     si,69D2h */
    SI = 0x69D2;
L0791:
    /* 0791  xor     bx,bx */
    BX = logic16((uint16_t)(BX ^ BX));
L0793: /* L0793 */
    /* 0793  mov     bl,byte ptr [si] */
    BL = rb(pDS, SI);
L0795:
    /* 0795  mov     bl,byte ptr cs:_cXfer[bx+300h] */
    BL = rb(CODE003, BX + 0x1173);
L079A:
    /* 079A  mov     byte ptr [si],bl */
    wb(pDS, SI, BL);
L079C:
    /* 079C  dec     si */
    SI = dec16(SI);
L079D:
    /* 079D  jne     L0793 */
    if (!ZF) goto L0793;
L079F:
    /* 079F  dec     cx */
    CX = dec16(CX);
L07A0:
    /* 07A0  jne     L078E */
    if (!ZF) goto L078E;
L07A2:
    /* 07A2  pop     ds */
    SET_DS(pop16());
L07A3:
    /* 07A3  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_0272_7A8  (+7A8)
       setup_frame_buf (FM Towns; far, cPlaceFB): BX = the frame's width, CX = its height. Builds the
       row offset table at 095C (rows of width + 2 bytes from offset 2; rows past the height all
       point at 69D4h), records the last row in 0AF0, and patches the unrolled copier _888 so that it
       copies exactly one plane's share of the width: the old ret becomes a movsb again and a ret
       (C3h) is written after the new width, for widths below 320. */
L07A8: /* _seg003_0272_7A8 */
    /* 07A8  add     bx,2 */
    BX = (uint16_t)(BX + 0x2);
L07AB:
    /* 07AB  mov     di,95Ch */
    DI = 0x95C;
L07AE:
    /* 07AE  mov     dx,cx */
    DX = CX;
L07B0:
    /* 07B0  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L07B2:
    /* 07B2  add     ax,2 */
    AX = (uint16_t)(AX + 0x2);
L07B5: /* L07B5 */
    /* 07B5  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L07B6:
    /* 07B6  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L07B8:
    /* 07B8  loop    L07B5 */
    if (--CX) goto L07B5;
L07BA:
    /* 07BA  mov     ax,69D4h */
    AX = 0x69D4;
L07BD:
    /* 07BD  mov     cx,0C8h */
    CX = 0xC8;
L07C0:
    /* 07C0  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L07C2:
    /* 07C2  dec     dx */
    DX = (uint16_t)(DX - 1);
L07C3:
    /* 07C3  mov     word ptr ds:[0AF0h],dx */
    ww(pDS, 0xAF0, DX);
L07C7:
    /* 07C7  rep stosw */
    while (CX) { ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2)); CX--; }
L07C9:
    /* 07C9  cmp     bx,word ptr ds:[0AEEh] */
    sub16(BX, rw(pDS, 0xAEE), 0);
L07CD:
    /* 07CD  je      L07F8 */
    if (ZF) goto L07F8;
L07CF:
    /* 07CF  cmp     word ptr ds:[0AEEh],140h */
    sub16(rw(pDS, 0xAEE), 0x140, 0);
L07D5:
    /* 07D5  jge     L07E4 */
    if (SF == OF) goto L07E4;
L07D7:
    /* 07D7  mov     di,offset _seg003_0272_888 */
    DI = 0x888;
L07DA:
    /* 07DA  mov     dx,word ptr ds:[0AEEh] */
    DX = rw(pDS, 0xAEE);
L07DE:
    /* 07DE  add     di,dx */
    DI = (uint16_t)(DI + DX);
L07E0:
    /* 07E0  mov     byte ptr cs:[di],0A4h */
    wb(CODE003, DI, 0xA4);
L07E4: /* L07E4 */
    /* 07E4  sub     bx,2 */
    BX = (uint16_t)(BX - 0x2);
L07E7:
    /* 07E7  cmp     bx,140h */
    sub16(BX, 0x140, 0);
L07EB:
    /* 07EB  jge     L07F8 */
    if (SF == OF) goto L07F8;
L07ED:
    /* 07ED  mov     di,offset _seg003_0272_888 */
    DI = 0x888;
L07F0:
    /* 07F0  mov     byte ptr cs:[bx+di],0C3h */
    wb(CODE003, BX + DI, 0xC3);
L07F4:
    /* 07F4  mov     word ptr ds:[0AEEh],bx */
    ww(pDS, 0xAEE, BX);
L07F8: /* L07F8 */
    /* 07F8  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* DRAW_RELATED_seg003_0272_7F9  (+7F9)
       cFBtoScreen (FM Towns; far): copy the frame buffer to the screen at A000:[94E] + [383A] (the
       page being drawn). Switches to the library's stack, then for each row from the last (the top
       line of the screen, since y counts up) down to the window's bottom edge (3DFA), four passes,
       one per plane (map masks 8, 2, 4, 1 with source offsets 3, 1, 2, 0), each through the unrolled
       copier _888; screen rows are 80 bytes apart. */
L07F9: /* _DRAW_RELATED_seg003_0272_7F9 */
    /* 07F9  push    bp */
    push16(BP);
L07FA:
    /* 07FA  push    es */
    push16(asm_es);
L07FB:
    /* 07FB  push    ds */
    push16(asm_ds);
L07FC:
    /* 07FC  push    si */
    push16(SI);
L07FD:
    /* 07FD  push    di */
    push16(DI);
L07FE:
    /* 07FE  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L0801:
    /* 0801  mov     ds,ax */
    SET_DS(AX);
L0803:
    /* 0803  mov     es,ax */
    SET_ES(AX);
L0805:
    /* 0805  mov     bx,ss */
    BX = asm_ss;
L0807:
    /* 0807  mov     word ptr cs:L07A4,bx */
    ww(CODE003, 0x7A4, BX);
L080C:
    /* 080C  mov     word ptr cs:L07A6,sp */
    ww(CODE003, 0x7A6, SP);
L0811:
    /* 0811  cli */
    ;
L0812:
    /* 0812  mov     ss,ax */
    SET_SS(AX);
L0814:
    /* 0814  mov     sp,word ptr ds:[5588h] */
    SP = rw(pDS, 0x5588);
L0818:
    /* 0818  sti */
    ;
L0819:
    /* 0819  mov     ax,0A000h */
    AX = 0xA000;
L081C:
    /* 081C  mov     es,ax */
    SET_ES(AX);
L081E:
    /* 081E  mov     bp,word ptr ds:[0AF0h] */
    BP = rw(pDS, 0xAF0);
L0822:
    /* 0822  mov     dx,SC_DATA */
    DX = 0x3C5;
L0825:
    /* 0825  mov     bx,word ptr ds:[94Eh] */
    BX = rw(pDS, 0x94E);
L0829:
    /* 0829  mov     ax,word ptr ds:[383Ah] */
    AX = rw(pDS, 0x383A);
L082C:
    /* 082C  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L082E:
    /* 082E  mov     ds,word ptr ds:[958h] */
    SET_DS(rw(pDS, 0x958));
L0832: /* L0832 */
    /* 0832  shl     bp,1 */
    BP = (uint16_t)(BP << 1);
L0834:
    /* 0834  mov     cx,word ptr [bp+95Ch] */
    CX = rw(pSS, BP + 0x95C);
L0838:
    /* 0838  mov     al,8 */
    AL = 0x8;
L083A:
    /* 083A  out     dx,al */
    asm_out8(DX, AL);
L083B:
    /* 083B  mov     di,bx */
    DI = BX;
L083D:
    /* 083D  mov     si,3 */
    SI = 0x3;
L0840:
    /* 0840  add     si,cx */
    SI = (uint16_t)(SI + CX);
L0842:
    /* 0842  call    _seg003_0272_888 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x0888), 0x0845)) != 0) return c;
L0845:
    /* 0845  mov     al,2 */
    AL = 0x2;
L0847:
    /* 0847  out     dx,al */
    asm_out8(DX, AL);
L0848:
    /* 0848  mov     di,bx */
    DI = BX;
L084A:
    /* 084A  mov     si,1 */
    SI = 0x1;
L084D:
    /* 084D  add     si,cx */
    SI = (uint16_t)(SI + CX);
L084F:
    /* 084F  call    _seg003_0272_888 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x0888), 0x0852)) != 0) return c;
L0852:
    /* 0852  mov     al,4 */
    AL = 0x4;
L0854:
    /* 0854  out     dx,al */
    asm_out8(DX, AL);
L0855:
    /* 0855  mov     di,bx */
    DI = BX;
L0857:
    /* 0857  mov     si,2 */
    SI = 0x2;
L085A:
    /* 085A  add     si,cx */
    SI = (uint16_t)(SI + CX);
L085C:
    /* 085C  call    _seg003_0272_888 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x0888), 0x085F)) != 0) return c;
L085F:
    /* 085F  mov     al,1 */
    AL = 0x1;
L0861:
    /* 0861  out     dx,al */
    asm_out8(DX, AL);
L0862:
    /* 0862  mov     di,bx */
    DI = BX;
L0864:
    /* 0864  mov     si,cx */
    SI = CX;
L0866:
    /* 0866  call    _seg003_0272_888 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x0888), 0x0869)) != 0) return c;
L0869:
    /* 0869  add     bx,50h */
    BX = (uint16_t)(BX + 0x50);
L086C:
    /* 086C  shr     bp,1 */
    BP = (uint16_t)(BP >> 1);
L086E:
    /* 086E  dec     bp */
    BP = (uint16_t)(BP - 1);
L086F:
    /* 086F  cmp     bp,word ptr ss:[3DFAh] */
    sub16(BP, rw(pSS, 0x3DFA), 0);
L0874:
    /* 0874  jg      L0832 */
    if (!ZF && SF == OF) goto L0832;
L0876:
    /* 0876  cli */
    ;
L0877:
    /* 0877  mov     ss,word ptr cs:L07A4 */
    SET_SS(rw(CODE003, 0x7A4));
L087C:
    /* 087C  mov     sp,word ptr cs:L07A6 */
    SP = rw(CODE003, 0x7A6);
L0881:
    /* 0881  sti */
    ;
L0882:
    /* 0882  pop     di */
    DI = pop16();
L0883:
    /* 0883  pop     si */
    SI = pop16();
L0884:
    /* 0884  pop     ds */
    SET_DS(pop16());
L0885:
    /* 0885  pop     es */
    SET_ES(pop16());
L0886:
    /* 0886  pop     bp */
    BP = pop16();
L0887:
    /* 0887  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_0272_888  (+888)
       _888: copy every fourth byte of a frame-buffer row to the screen (movsb; add si,3, unrolled
       for 320 pixels; setup_frame_buf patches in the ret). */
L0888: /* _seg003_0272_888 */
    /* 0888  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0889:
    /* 0889  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L088C:
    /* 088C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L088D:
    /* 088D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0890:
    /* 0890  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0891:
    /* 0891  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0894:
    /* 0894  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0895:
    /* 0895  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0898:
    /* 0898  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0899:
    /* 0899  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L089C:
    /* 089C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L089D:
    /* 089D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08A0:
    /* 08A0  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08A1:
    /* 08A1  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08A4:
    /* 08A4  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08A5:
    /* 08A5  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08A8:
    /* 08A8  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08A9:
    /* 08A9  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08AC:
    /* 08AC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08AD:
    /* 08AD  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08B0:
    /* 08B0  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08B1:
    /* 08B1  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08B4:
    /* 08B4  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08B5:
    /* 08B5  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08B8:
    /* 08B8  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08B9:
    /* 08B9  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08BC:
    /* 08BC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08BD:
    /* 08BD  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08C0:
    /* 08C0  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08C1:
    /* 08C1  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08C4:
    /* 08C4  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08C5:
    /* 08C5  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08C8:
    /* 08C8  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08C9:
    /* 08C9  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08CC:
    /* 08CC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08CD:
    /* 08CD  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08D0:
    /* 08D0  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08D1:
    /* 08D1  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08D4:
    /* 08D4  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08D5:
    /* 08D5  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08D8:
    /* 08D8  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08D9:
    /* 08D9  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08DC:
    /* 08DC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08DD:
    /* 08DD  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08E0:
    /* 08E0  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08E1:
    /* 08E1  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08E4:
    /* 08E4  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08E5:
    /* 08E5  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08E8:
    /* 08E8  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08E9:
    /* 08E9  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08EC:
    /* 08EC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08ED:
    /* 08ED  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08F0:
    /* 08F0  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08F1:
    /* 08F1  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08F4:
    /* 08F4  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08F5:
    /* 08F5  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08F8:
    /* 08F8  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08F9:
    /* 08F9  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L08FC:
    /* 08FC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L08FD:
    /* 08FD  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0900:
    /* 0900  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0901:
    /* 0901  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0904:
    /* 0904  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0905:
    /* 0905  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0908:
    /* 0908  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0909:
    /* 0909  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L090C:
    /* 090C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L090D:
    /* 090D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0910:
    /* 0910  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0911:
    /* 0911  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0914:
    /* 0914  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0915:
    /* 0915  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0918:
    /* 0918  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0919:
    /* 0919  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L091C:
    /* 091C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L091D:
    /* 091D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0920:
    /* 0920  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0921:
    /* 0921  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0924:
    /* 0924  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0925:
    /* 0925  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0928:
    /* 0928  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0929:
    /* 0929  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L092C:
    /* 092C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L092D:
    /* 092D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0930:
    /* 0930  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0931:
    /* 0931  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0934:
    /* 0934  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0935:
    /* 0935  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0938:
    /* 0938  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0939:
    /* 0939  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L093C:
    /* 093C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L093D:
    /* 093D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0940:
    /* 0940  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0941:
    /* 0941  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0944:
    /* 0944  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0945:
    /* 0945  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0948:
    /* 0948  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0949:
    /* 0949  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L094C:
    /* 094C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L094D:
    /* 094D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0950:
    /* 0950  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0951:
    /* 0951  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0954:
    /* 0954  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0955:
    /* 0955  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0958:
    /* 0958  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0959:
    /* 0959  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L095C:
    /* 095C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L095D:
    /* 095D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0960:
    /* 0960  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0961:
    /* 0961  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0964:
    /* 0964  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0965:
    /* 0965  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0968:
    /* 0968  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0969:
    /* 0969  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L096C:
    /* 096C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L096D:
    /* 096D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0970:
    /* 0970  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0971:
    /* 0971  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0974:
    /* 0974  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0975:
    /* 0975  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0978:
    /* 0978  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0979:
    /* 0979  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L097C:
    /* 097C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L097D:
    /* 097D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0980:
    /* 0980  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0981:
    /* 0981  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0984:
    /* 0984  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0985:
    /* 0985  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0988:
    /* 0988  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0989:
    /* 0989  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L098C:
    /* 098C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L098D:
    /* 098D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0990:
    /* 0990  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0991:
    /* 0991  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0994:
    /* 0994  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0995:
    /* 0995  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0998:
    /* 0998  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0999:
    /* 0999  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L099C:
    /* 099C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L099D:
    /* 099D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L09A0:
    /* 09A0  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L09A1:
    /* 09A1  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L09A4:
    /* 09A4  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L09A5:
    /* 09A5  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L09A8:
    /* 09A8  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L09A9:
    /* 09A9  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L09AC:
    /* 09AC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L09AD:
    /* 09AD  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L09B0:
    /* 09B0  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L09B1:
    /* 09B1  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L09B4:
    /* 09B4  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L09B5:
    /* 09B5  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L09B8:
    /* 09B8  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L09B9:
    /* 09B9  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L09BC:
    /* 09BC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L09BD:
    /* 09BD  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L09C0:
    /* 09C0  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L09C1:
    /* 09C1  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L09C4:
    /* 09C4  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L09C5:
    /* 09C5  add     si,3 */
    SI = add16(SI, 0x3, 0);
L09C8:
    /* 09C8  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_9C9  (+9C9)
       fake_fb_solid_ylr (FM Towns): the solid span writer for the frame buffer: fill each 6-byte
       span record (y, left, right; either order) with the colour in 4111. */
L09C9: /* _seg003_0272_9C9 */
    /* 09C9  push    es */
    push16(asm_es);
L09CA:
    /* 09CA  mov     bx,word ptr ds:[3DF4h] */
    BX = rw(pDS, 0x3DF4);
L09CE:
    /* 09CE  mov     es,word ptr ds:[958h] */
    SET_ES(rw(pDS, 0x958));
L09D2: /* L09D2 */
    /* 09D2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L09D3:
    /* 09D3  shl     ax,1 */
    AX = shl16(AX, 1);
L09D5:
    /* 09D5  jb      L09F1 */
    if (CF) goto L09F1;
L09D7:
    /* 09D7  mov     di,ax */
    DI = AX;
L09D9:
    /* 09D9  mov     di,word ptr [di+95Ch] */
    DI = rw(pDS, DI + 0x95C);
L09DD:
    /* 09DD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L09DE:
    /* 09DE  mov     cx,ax */
    CX = AX;
L09E0:
    /* 09E0  add     di,ax */
    DI = (uint16_t)(DI + AX);
L09E2:
    /* 09E2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L09E3:
    /* 09E3  sub     cx,ax */
    CX = (uint16_t)(CX - AX);
L09E5:
    /* 09E5  neg     cx */
    CX = (uint16_t)-CX;
L09E7:
    /* 09E7  mov     al,byte ptr ds:[4111h] */
    AL = rb(pDS, 0x4111);
L09EA:
    /* 09EA  inc     cx */
    CX = inc16(CX);
L09EB:
    /* 09EB  js      L09F3 */
    if (SF) goto L09F3;
L09ED:
    /* 09ED  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L09EF:
    /* 09EF  jmp     L09D2 */
    goto L09D2;
L09F1: /* L09F1 */
    /* 09F1  pop     es */
    SET_ES(pop16());
L09F2:
    /* 09F2  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L09F3: /* L09F3 */
    /* 09F3  add     di,cx */
    DI = (uint16_t)(DI + CX);
L09F5:
    /* 09F5  dec     cx */
    CX = (uint16_t)(CX - 1);
L09F6:
    /* 09F6  neg     cx */
    CX = (uint16_t)-CX;
L09F8:
    /* 09F8  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L09FA:
    /* 09FA  jmp     L09D2 */
    goto L09D2;

    /* seg003_0272_9FC  (+9FC)
       fbuf_save_vylr (FM Towns): copy frame-buffer spans (6-byte records, widened to whole groups of
       four pixels) to video memory at the offset in 4116, one plane at a time. Probably the save
       half of VALLOC.ASM's save_rect when the frame buffer is the target. */
L09FC: /* _seg003_0272_9FC */
    /* 09FC  mov     dx,SC_DATA */
    DX = 0x3C5;
L09FF:
    /* 09FF  mov     bp,si */
    BP = SI;
L0A01:
    /* 0A01  mov     di,word ptr ds:[4116h] */
    DI = rw(pDS, 0x4116);
L0A05:
    /* 0A05  push    ds */
    push16(asm_ds);
L0A06:
    /* 0A06  push    es */
    push16(asm_es);
L0A07:
    /* 0A07  mov     es,word ptr ds:[3DFCh] */
    SET_ES(rw(pDS, 0x3DFC));
L0A0B:
    /* 0A0B  mov     ds,word ptr ds:[958h] */
    SET_DS(rw(pDS, 0x958));
L0A0F: /* L0A0F */
    /* 0A0F  mov     bx,word ptr [bp] */
    BX = rw(pSS, BP);
L0A12:
    /* 0A12  shl     bx,1 */
    BX = shl16(BX, 1);
L0A14:
    /* 0A14  jb      L0A7C */
    if (CF) goto L0A7C;
L0A16:
    /* 0A16  mov     si,word ptr [bp+2] */
    SI = rw(pSS, BP + 0x2);
L0A19:
    /* 0A19  and     si,-4 */
    SI = (uint16_t)(SI & 0xFFFC);
L0A1C:
    /* 0A1C  mov     cx,word ptr [bp+4] */
    CX = rw(pSS, BP + 0x4);
L0A1F:
    /* 0A1F  and     cx,-4 */
    CX = (uint16_t)(CX & 0xFFFC);
L0A22:
    /* 0A22  add     cx,4 */
    CX = (uint16_t)(CX + 0x4);
L0A25:
    /* 0A25  add     bp,6 */
    BP = (uint16_t)(BP + 0x6);
L0A28:
    /* 0A28  sub     cx,si */
    CX = (uint16_t)(CX - SI);
L0A2A:
    /* 0A2A  add     si,word ptr ss:[bx+95Ch] */
    SI = (uint16_t)(SI + rw(pSS, BX + 0x95C));
L0A2F:
    /* 0A2F  mov     bx,cx */
    BX = CX;
L0A31:
    /* 0A31  push    bp */
    push16(BP);
L0A32:
    /* 0A32  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0A35:
    /* 0A35  mov     bp,di */
    BP = DI;
L0A37:
    /* 0A37  mov     al,1 */
    AL = 0x1;
L0A39:
    /* 0A39  out     dx,al */
    asm_out8(DX, AL);
L0A3A: /* L0A3A */
    /* 0A3A  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0A3B:
    /* 0A3B  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0A3E:
    /* 0A3E  loop    L0A3A */
    if (--CX) goto L0A3A;
L0A40:
    /* 0A40  sub     si,bx */
    SI = (uint16_t)(SI - BX);
L0A42:
    /* 0A42  inc     si */
    SI = (uint16_t)(SI + 1);
L0A43:
    /* 0A43  mov     cx,bx */
    CX = BX;
L0A45:
    /* 0A45  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0A48:
    /* 0A48  mov     di,bp */
    DI = BP;
L0A4A:
    /* 0A4A  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L0A4C:
    /* 0A4C  out     dx,al */
    asm_out8(DX, AL);
L0A4D: /* L0A4D */
    /* 0A4D  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0A4E:
    /* 0A4E  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0A51:
    /* 0A51  loop    L0A4D */
    if (--CX) goto L0A4D;
L0A53:
    /* 0A53  sub     si,bx */
    SI = (uint16_t)(SI - BX);
L0A55:
    /* 0A55  inc     si */
    SI = (uint16_t)(SI + 1);
L0A56:
    /* 0A56  mov     cx,bx */
    CX = BX;
L0A58:
    /* 0A58  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0A5B:
    /* 0A5B  mov     di,bp */
    DI = BP;
L0A5D:
    /* 0A5D  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L0A5F:
    /* 0A5F  out     dx,al */
    asm_out8(DX, AL);
L0A60: /* L0A60 */
    /* 0A60  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0A61:
    /* 0A61  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0A64:
    /* 0A64  loop    L0A60 */
    if (--CX) goto L0A60;
L0A66:
    /* 0A66  sub     si,bx */
    SI = (uint16_t)(SI - BX);
L0A68:
    /* 0A68  inc     si */
    SI = (uint16_t)(SI + 1);
L0A69:
    /* 0A69  mov     cx,bx */
    CX = BX;
L0A6B:
    /* 0A6B  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0A6E:
    /* 0A6E  mov     di,bp */
    DI = BP;
L0A70:
    /* 0A70  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L0A72:
    /* 0A72  out     dx,al */
    asm_out8(DX, AL);
L0A73: /* L0A73 */
    /* 0A73  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0A74:
    /* 0A74  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0A77:
    /* 0A77  loop    L0A73 */
    if (--CX) goto L0A73;
L0A79:
    /* 0A79  pop     bp */
    BP = pop16();
L0A7A:
    /* 0A7A  jmp     L0A0F */
    goto L0A0F;
L0A7C: /* L0A7C */
    /* 0A7C  pop     es */
    SET_ES(pop16());
L0A7D:
    /* 0A7D  pop     ds */
    SET_DS(pop16());
L0A7E:
    /* 0A7E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_A7F  (+A7F)
       fbcopy_vylrpp (FM Towns): copy frame-buffer rows to the planar screen (8-byte records: y,
       left, right, destination offset), plane by plane with the edge masks from 3B64 and 3CA8;
       VIDMODE's vcopyfb uses it. Ends by setting the bit mask back to 0FFh. */
L0A7F: /* _seg003_0272_A7F */
    /* 0A7F  push    ds */
    push16(asm_ds);
L0A80:
    /* 0A80  push    es */
    push16(asm_es);
L0A81:
    /* 0A81  mov     es,word ptr ds:[3DFCh] */
    SET_ES(rw(pDS, 0x3DFC));
L0A85:
    /* 0A85  mov     ds,word ptr ds:[958h] */
    SET_DS(rw(pDS, 0x958));
L0A89:
    /* 0A89  mov     bp,si */
    BP = SI;
L0A8B: /* L0A8B */
    /* 0A8B  mov     bx,word ptr [bp] */
    BX = rw(pSS, BP);
L0A8E:
    /* 0A8E  shl     bx,1 */
    BX = shl16(BX, 1);
L0A90:
    /* 0A90  jae     L0A95 */
    if (!CF) goto L0A95;
L0A92:
    /* 0A92  jmp     L0B8C */
    goto L0B8C;
L0A95: /* L0A95 */
    /* 0A95  mov     si,word ptr ss:[bx+95Ch] */
    SI = rw(pSS, BX + 0x95C);
L0A9A:
    /* 0A9A  mov     bx,word ptr [bp+2] */
    BX = rw(pSS, BP + 0x2);
L0A9D:
    /* 0A9D  mov     cx,word ptr [bp+4] */
    CX = rw(pSS, BP + 0x4);
L0AA0:
    /* 0AA0  mov     di,word ptr [bp+6] */
    DI = rw(pSS, BP + 0x6);
L0AA3:
    /* 0AA3  add     si,bx */
    SI = (uint16_t)(SI + BX);
L0AA5:
    /* 0AA5  add     bp,8 */
    BP = (uint16_t)(BP + 0x8);
L0AA8:
    /* 0AA8  push    bp */
    push16(BP);
L0AA9:
    /* 0AA9  sub     cx,bx */
    CX = (uint16_t)(CX - BX);
L0AAB:
    /* 0AAB  inc     cx */
    CX = (uint16_t)(CX + 1);
L0AAC:
    /* 0AAC  mov     bp,cx */
    BP = CX;
L0AAE:
    /* 0AAE  mov     dx,SC_DATA */
    DX = 0x3C5;
L0AB1:
    /* 0AB1  mov     al,byte ptr ss:[bx+3B64h] */
    AL = rb(pSS, BX + 0x3B64);
L0AB6:
    /* 0AB6  and     al,byte ptr ss:[bx+3CA8h] */
    AL = (uint8_t)(AL & rb(pSS, BX + 0x3CA8));
L0ABB:
    /* 0ABB  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0ABE:
    /* 0ABE  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0AC1:
    /* 0AC1  out     dx,al */
    asm_out8(DX, AL);
L0AC2: /* L0AC2 */
    /* 0AC2  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0AC3:
    /* 0AC3  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0AC6:
    /* 0AC6  loop    L0AC2 */
    if (--CX) goto L0AC2;
L0AC8:
    /* 0AC8  mov     cx,bp */
    CX = BP;
L0ACA:
    /* 0ACA  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0ACD:
    /* 0ACD  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0AD0:
    /* 0AD0  sub     di,cx */
    DI = (uint16_t)(DI - CX);
L0AD2:
    /* 0AD2  shl     cx,2 */
    CX = (uint16_t)(CX << 2);
L0AD5:
    /* 0AD5  sub     si,cx */
    SI = (uint16_t)(SI - CX);
L0AD7:
    /* 0AD7  inc     si */
    SI = (uint16_t)(SI + 1);
L0AD8:
    /* 0AD8  inc     bx */
    BX = (uint16_t)(BX + 1);
L0AD9:
    /* 0AD9  mov     cx,bx */
    CX = BX;
L0ADB:
    /* 0ADB  and     cx,3 */
    CX = logic16((uint16_t)(CX & 0x3));
L0ADE:
    /* 0ADE  jne     L0AE1 */
    if (!ZF) goto L0AE1;
L0AE0:
    /* 0AE0  inc     di */
    DI = (uint16_t)(DI + 1);
L0AE1: /* L0AE1 */
    /* 0AE1  dec     bp */
    BP = dec16(BP);
L0AE2:
    /* 0AE2  jne     L0AE7 */
    if (!ZF) goto L0AE7;
L0AE4:
    /* 0AE4  jmp     L0B88 */
    goto L0B88;
L0AE7: /* L0AE7 */
    /* 0AE7  mov     cx,bp */
    CX = BP;
L0AE9:
    /* 0AE9  mov     al,byte ptr ss:[bx+3B64h] */
    AL = rb(pSS, BX + 0x3B64);
L0AEE:
    /* 0AEE  and     al,byte ptr ss:[bx+3CA8h] */
    AL = (uint8_t)(AL & rb(pSS, BX + 0x3CA8));
L0AF3:
    /* 0AF3  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0AF6:
    /* 0AF6  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0AF9:
    /* 0AF9  out     dx,al */
    asm_out8(DX, AL);
L0AFA: /* L0AFA */
    /* 0AFA  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0AFB:
    /* 0AFB  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0AFE:
    /* 0AFE  loop    L0AFA */
    if (--CX) goto L0AFA;
L0B00:
    /* 0B00  mov     cx,bp */
    CX = BP;
L0B02:
    /* 0B02  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0B05:
    /* 0B05  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0B08:
    /* 0B08  sub     di,cx */
    DI = (uint16_t)(DI - CX);
L0B0A:
    /* 0B0A  shl     cx,2 */
    CX = (uint16_t)(CX << 2);
L0B0D:
    /* 0B0D  sub     si,cx */
    SI = (uint16_t)(SI - CX);
L0B0F:
    /* 0B0F  inc     si */
    SI = (uint16_t)(SI + 1);
L0B10:
    /* 0B10  inc     bx */
    BX = (uint16_t)(BX + 1);
L0B11:
    /* 0B11  mov     cx,bx */
    CX = BX;
L0B13:
    /* 0B13  and     cx,3 */
    CX = logic16((uint16_t)(CX & 0x3));
L0B16:
    /* 0B16  jne     L0B19 */
    if (!ZF) goto L0B19;
L0B18:
    /* 0B18  inc     di */
    DI = (uint16_t)(DI + 1);
L0B19: /* L0B19 */
    /* 0B19  dec     bp */
    BP = dec16(BP);
L0B1A:
    /* 0B1A  je      L0B88 */
    if (ZF) goto L0B88;
L0B1C:
    /* 0B1C  mov     cx,bp */
    CX = BP;
L0B1E:
    /* 0B1E  mov     al,byte ptr ss:[bx+3B64h] */
    AL = rb(pSS, BX + 0x3B64);
L0B23:
    /* 0B23  and     al,byte ptr ss:[bx+3CA8h] */
    AL = (uint8_t)(AL & rb(pSS, BX + 0x3CA8));
L0B28:
    /* 0B28  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0B2B:
    /* 0B2B  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0B2E:
    /* 0B2E  out     dx,al */
    asm_out8(DX, AL);
L0B2F: /* L0B2F */
    /* 0B2F  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0B30:
    /* 0B30  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0B33:
    /* 0B33  loop    L0B2F */
    if (--CX) goto L0B2F;
L0B35:
    /* 0B35  mov     cx,bp */
    CX = BP;
L0B37:
    /* 0B37  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0B3A:
    /* 0B3A  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0B3D:
    /* 0B3D  sub     di,cx */
    DI = (uint16_t)(DI - CX);
L0B3F:
    /* 0B3F  shl     cx,2 */
    CX = (uint16_t)(CX << 2);
L0B42:
    /* 0B42  sub     si,cx */
    SI = (uint16_t)(SI - CX);
L0B44:
    /* 0B44  inc     si */
    SI = (uint16_t)(SI + 1);
L0B45:
    /* 0B45  inc     bx */
    BX = (uint16_t)(BX + 1);
L0B46:
    /* 0B46  mov     cx,bx */
    CX = BX;
L0B48:
    /* 0B48  and     cx,3 */
    CX = logic16((uint16_t)(CX & 0x3));
L0B4B:
    /* 0B4B  jne     L0B4E */
    if (!ZF) goto L0B4E;
L0B4D:
    /* 0B4D  inc     di */
    DI = (uint16_t)(DI + 1);
L0B4E: /* L0B4E */
    /* 0B4E  dec     bp */
    BP = dec16(BP);
L0B4F:
    /* 0B4F  je      L0B88 */
    if (ZF) goto L0B88;
L0B51:
    /* 0B51  mov     cx,bp */
    CX = BP;
L0B53:
    /* 0B53  mov     al,byte ptr ss:[bx+3B64h] */
    AL = rb(pSS, BX + 0x3B64);
L0B58:
    /* 0B58  and     al,byte ptr ss:[bx+3CA8h] */
    AL = (uint8_t)(AL & rb(pSS, BX + 0x3CA8));
L0B5D:
    /* 0B5D  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0B60:
    /* 0B60  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0B63:
    /* 0B63  out     dx,al */
    asm_out8(DX, AL);
L0B64: /* L0B64 */
    /* 0B64  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0B65:
    /* 0B65  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0B68:
    /* 0B68  loop    L0B64 */
    if (--CX) goto L0B64;
L0B6A:
    /* 0B6A  mov     cx,bp */
    CX = BP;
L0B6C:
    /* 0B6C  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0B6F:
    /* 0B6F  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0B72:
    /* 0B72  sub     di,cx */
    DI = (uint16_t)(DI - CX);
L0B74:
    /* 0B74  shl     cx,2 */
    CX = (uint16_t)(CX << 2);
L0B77:
    /* 0B77  sub     si,cx */
    SI = (uint16_t)(SI - CX);
L0B79:
    /* 0B79  inc     si */
    SI = (uint16_t)(SI + 1);
L0B7A:
    /* 0B7A  inc     bx */
    BX = (uint16_t)(BX + 1);
L0B7B:
    /* 0B7B  mov     cx,bx */
    CX = BX;
L0B7D:
    /* 0B7D  and     cx,3 */
    CX = logic16((uint16_t)(CX & 0x3));
L0B80:
    /* 0B80  jne     L0B83 */
    if (!ZF) goto L0B83;
L0B82:
    /* 0B82  inc     di */
    DI = (uint16_t)(DI + 1);
L0B83: /* L0B83 */
    /* 0B83  dec     bp */
    BP = dec16(BP);
L0B84:
    /* 0B84  je      L0B88 */
    if (ZF) goto L0B88;
L0B86:
    /* 0B86  mov     cx,bp */
    CX = BP;
L0B88: /* L0B88 */
    /* 0B88  pop     bp */
    BP = pop16();
L0B89:
    /* 0B89  jmp     L0A8B */
    goto L0A8B;
L0B8C: /* L0B8C */
    /* 0B8C  mov     dx,GC_INDEX */
    DX = 0x3CE;
L0B8F:
    /* 0B8F  mov     ax,0FF08h */
    AX = 0xFF08;
L0B92:
    /* 0B92  out     dx,ax */
    asm_out16(DX, AX);
L0B93:
    /* 0B93  pop     es */
    SET_ES(pop16());
L0B94:
    /* 0B94  pop     ds */
    SET_DS(pop16());
L0B95:
    /* 0B95  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0B96:
    /* 0B96  add     sp,2 */
    SP = add16(SP, 0x2, 0);
L0B99:
    /* 0B99  jmp     L0B8C */
    goto L0B8C;

    /* seg003_0272_B9B  (+B9B)
       fbuf_setcolor (FM Towns): AX = a pen number (negative: no change). Sets the colour bytes 4110
       and 4116 from the tables at 21E and 434, and, when the pen's mode (the byte table at
       seg_370D:0008) is below 14h, the span writer 4112 from the table at 0AF2; so a pen chooses
       both a colour and a way of writing it (solid, copy, save, restore ...).

       After its ret: L0BC1 (the current span's y, whose low bit picks the dither phase) and the two
       smooth-span vectors L0BC3 and L0BC5, which seg004 sets (INTERP.ASM's opcode D6h to 0BC7h and
       0C93h, solid; opcode D8h and SMOOTH.ASM to 0BF2h and 0CD9h, translucent). L0BC7
       (smooth_span_same, solid, both ends the same shade) draws the span in one colour,
       lightabs[shade][base colour] with the base colour at seg004:6D20 (the byte PGCACHE.ASM keeps
       for seg003), through fake_fb_solid_ylr. The code at 0BF2 (trans_span_same) remaps the pixels
       already in the span through one lightabs row. */
L0B9B: /* _seg003_0272_B9B */
    /* 0B9B  shl     ax,1 */
    AX = shl16(AX, 1);
L0B9D:
    /* 0B9D  jb      L0BBF */
    if (CF) goto L0BBF;
L0B9F:
    /* 0B9F  mov     bx,ax */
    BX = AX;
L0BA1:
    /* 0BA1  mov     ax,word ptr [bx+21Eh] */
    AX = rw(pDS, BX + 0x21E);
L0BA5:
    /* 0BA5  mov     word ptr ds:[4110h],ax */
    ww(pDS, 0x4110, AX);
L0BA8:
    /* 0BA8  mov     ax,word ptr [bx+434h] */
    AX = rw(pDS, BX + 0x434);
L0BAC:
    /* 0BAC  mov     word ptr ds:[4116h],ax */
    ww(pDS, 0x4116, AX);
    asm_halt_at(0x0085, 0x0BAF, "ran off the code into data");
L0BB3:
    /* 0BB3  cmp     bx,14h */
    sub16(BX, 0x14, 0);
L0BB6:
    /* 0BB6  jae     L0BBF */
    if (!CF) goto L0BBF;
L0BB8:
    /* 0BB8  mov     ax,word ptr [bx+0AF2h] */
    AX = rw(pDS, BX + 0xAF2);
L0BBC:
    /* 0BBC  mov     word ptr ds:[4112h],ax */
    ww(pDS, 0x4112, AX);
L0BBF: /* L0BBF */
    /* 0BBF  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0BC0: /* L0BC0 */
    /* 0BC0  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0BC7: /* L0BC7 */
    /* 0BC7  push    si */
    push16(SI);
L0BC8:
    /* 0BC8  push    es */
    push16(asm_es);
L0BC9:
    /* 0BC9  mov     ax,seg seg004_0849 */
    AX = (uint16_t)(0x065C + PORT_LOAD_SEG);
L0BCC:
    /* 0BCC  mov     es,ax */
    SET_ES(AX);
L0BCE:
    /* 0BCE  mov     al,byte ptr es:[6D20h] */
    AL = rb(pES, 0x6D20);
L0BD2:
    /* 0BD2  mov     ah,byte ptr ds:[0B08h] */
    AH = rb(pDS, 0xB08);
L0BD6:
    /* 0BD6  mov     bx,ax */
    BX = AX;
L0BD8:
    /* 0BD8  mov     al,byte ptr es:[bx+6D3Eh] */
    AL = rb(pES, BX + 0x6D3E);
L0BDD:
    /* 0BDD  mov     byte ptr ds:[4111h],al */
    wb(pDS, 0x4111, AL);
L0BE0:
    /* 0BE0  or      byte ptr [si-3],80h */
    wb(pDS, SI + 0xFFFD, (uint8_t)(rb(pDS, SI + 0xFFFD) | 0x80));
L0BE4:
    /* 0BE4  sub     si,0Ah */
    SI = (uint16_t)(SI - 0xA);
L0BE7:
    /* 0BE7  call    _seg003_0272_9C9 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x09C9), 0x0BEA)) != 0) return c;
L0BEA:
    /* 0BEA  pop     es */
    SET_ES(pop16());
L0BEB:
    /* 0BEB  pop     si */
    SI = pop16();
L0BEC:
    /* 0BEC  and     byte ptr [si-3],7Fh */
    wb(pDS, SI + 0xFFFD, (uint8_t)(rb(pDS, SI + 0xFFFD) & 0x7F));
L0BF0:
    /* 0BF0  jmp     short _seg003_0272_C13 */
    goto L0C13;
L0BF2:
    /* 0BF2  push    ds */
    push16(asm_ds);
L0BF3:
    /* 0BF3  push    es */
    push16(asm_es);
L0BF4:
    /* 0BF4  mov     dh,byte ptr ss:[0B08h] */
    DH = rb(pSS, 0xB08);
L0BF9:
    /* 0BF9  mov     ds,word ptr ss:[958h] */
    SET_DS(rw(pSS, 0x958));
L0BFE:
    /* 0BFE  mov     ax,seg seg004_0849 */
    AX = (uint16_t)(0x065C + PORT_LOAD_SEG);
L0C01:
    /* 0C01  mov     es,ax */
    SET_ES(AX);
L0C03: /* L0C03 */
    /* 0C03  mov     dl,byte ptr [di] */
    DL = rb(pDS, DI);
L0C05:
    /* 0C05  mov     bx,dx */
    BX = DX;
L0C07:
    /* 0C07  mov     bh,byte ptr es:[bx+6D3Eh] */
    BH = rb(pES, BX + 0x6D3E);
L0C0C:
    /* 0C0C  mov     byte ptr [di],bh */
    wb(pDS, DI, BH);
L0C0E:
    /* 0C0E  inc     di */
    DI = (uint16_t)(DI + 1);
L0C0F:
    /* 0C0F  loop    L0C03 */
    if (--CX) goto L0C03;
L0C11:
    /* 0C11  pop     es */
    SET_ES(pop16());
L0C12:
    /* 0C12  pop     ds */
    SET_DS(pop16());

    /* seg003_0272_C13  (+C13)
       vga_smooth_ylrii (FM Towns): draw a table of Gouraud spans into the frame buffer. In: SI =
       10-byte records (y, left x, right x, left intensity, right intensity; GRLIBM.ASM's table at
       5692), ended by a y with its top bit set. For each, the intensity step per pixel is formed as
       8.8 and the span drawn through L0BC5, or through L0BC3 when both ends have the same intensity.

       L0C93 (do_filled_dither, solid) writes lightabs[intensity][base colour]; the code after L0CD3
       (do_trans_dither, translucent) writes lightabs[intensity][the pixel already there]. Both
       dither: two intensity accumulators 40h apart alternate pixel by pixel, and which starts
       depends on the row's parity, so a shade between two lightabs rows comes out as a checkerboard
       of both. */
L0C13: /* _seg003_0272_C13 */
    /* 0C13  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0C14:
    /* 0C14  mov     word ptr cs:L0BC1,ax */
    ww(CODE003, 0xBC1, AX);
L0C18:
    /* 0C18  shl     ax,1 */
    AX = shl16(AX, 1);
L0C1A:
    /* 0C1A  jb      L0BC0 */
    if (CF) goto L0BC0;
L0C1C:
    /* 0C1C  mov     di,ax */
    DI = AX;
L0C1E:
    /* 0C1E  mov     di,word ptr ss:[di+95Ch] */
    DI = rw(pSS, DI + 0x95C);
L0C23:
    /* 0C23  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0C24:
    /* 0C24  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L0C25:
    /* 0C25  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0C26:
    /* 0C26  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L0C27:
    /* 0C27  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0C28:
    /* 0C28  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L0C29:
    /* 0C29  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0C2A:
    /* 0C2A  mov     bp,cx */
    BP = CX;
L0C2C:
    /* 0C2C  sub     cx,dx */
    CX = sub16(CX, DX, 0);
L0C2E:
    /* 0C2E  jge     L0C35 */
    if (SF == OF) goto L0C35;
L0C30:
    /* 0C30  neg     cx */
    CX = (uint16_t)-CX;
L0C32:
    /* 0C32  xchg    bp,dx */
    { uint16_t t_ = DX;
    DX = BP;
    BP = t_; }
L0C34:
    /* 0C34  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L0C35: /* L0C35 */
    /* 0C35  mov     word ptr ss:[0B08h],bx */
    ww(pSS, 0xB08, BX);
L0C3A:
    /* 0C3A  mov     word ptr ss:[0B0Ah],dx */
    ww(pSS, 0xB0A, DX);
L0C3F:
    /* 0C3F  add     di,dx */
    DI = (uint16_t)(DI + DX);
L0C41:
    /* 0C41  inc     cx */
    CX = (uint16_t)(CX + 1);
L0C42:
    /* 0C42  sub     ax,bx */
    AX = sub16(AX, BX, 0);
L0C44:
    /* 0C44  jne     L0C4B */
    if (!ZF) goto L0C4B;
L0C46:
    /* 0C46  jmp     word ptr cs:L0BC3 */
    return ASM_JMP(0x0085, rw(CODE003, 0xBC3));
L0C4B: /* L0C4B */
    /* 0C4B  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L0C4D:
    /* 0C4D  jns     L0C6A */
    if (!SF) goto L0C6A;
L0C4F:
    /* 0C4F  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L0C51:
    /* 0C51  jns     L0C59 */
    if (!SF) goto L0C59;
L0C53:
    /* 0C53  neg     ax */
    AX = (uint16_t)-AX;
L0C55:
    /* 0C55  neg     cx */
    CX = (uint16_t)-CX;
L0C57:
    /* 0C57  jmp     short L0C74 */
    goto L0C74;
L0C59: /* L0C59 */
    /* 0C59  neg     ax */
    AX = (uint16_t)-AX;
L0C5B: /* L0C5B */
    /* 0C5B  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0C5C:
    /* 0C5C  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0085, 0x0C5C, 2)) != 0) return c;
L0C5E:
    /* 0C5E  mov     bh,al */
    BH = AL;
L0C60:
    /* 0C60  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L0C62:
    /* 0C62  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x0085, 0x0C62, 2)) != 0) return c;
L0C64:
    /* 0C64  mov     bl,ah */
    BL = AH;
L0C66:
    /* 0C66  neg     bx */
    BX = neg16(BX);
L0C68:
    /* 0C68  jmp     short L0C7F */
    goto L0C7F;
L0C6A: /* L0C6A */
    /* 0C6A  test    cx,0FFFFh */
    logic16((uint16_t)(CX & 0xFFFF));
L0C6E:
    /* 0C6E  jns     L0C74 */
    if (!SF) goto L0C74;
L0C70:
    /* 0C70  neg     cx */
    CX = (uint16_t)-CX;
L0C72:
    /* 0C72  jmp     L0C5B */
    goto L0C5B;
L0C74: /* L0C74 */
    /* 0C74  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0C75:
    /* 0C75  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0085, 0x0C75, 2)) != 0) return c;
L0C77:
    /* 0C77  mov     bh,al */
    BH = AL;
L0C79:
    /* 0C79  sub     ax,ax */
    AX = sub16(AX, AX, 0);
L0C7B:
    /* 0C7B  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x0085, 0x0C7B, 2)) != 0) return c;
L0C7D:
    /* 0C7D  mov     bl,ah */
    BL = AH;
L0C7F: /* L0C7F */
    /* 0C7F  mov     bp,bx */
    BP = BX;
L0C81:
    /* 0C81  mov     bh,byte ptr ss:[0B08h] */
    BH = rb(pSS, 0xB08);
L0C86:
    /* 0C86  mov     bl,80h */
    BL = 0x80;
L0C88:
    /* 0C88  push    ds */
    push16(asm_ds);
L0C89:
    /* 0C89  mov     ds,word ptr ss:[958h] */
    SET_DS(rw(pSS, 0x958));
L0C8E:
    /* 0C8E  jmp     word ptr cs:L0BC5 */
    return ASM_JMP(0x0085, rw(CODE003, 0xBC5));
L0C93: /* L0C93 */
    /* 0C93  push    si */
    push16(SI);
L0C94:
    /* 0C94  push    es */
    push16(asm_es);
L0C95:
    /* 0C95  mov     ax,cx */
    AX = CX;
L0C97:
    /* 0C97  mov     cx,bx */
    CX = BX;
L0C99:
    /* 0C99  mov     dx,bx */
    DX = BX;
L0C9B:
    /* 0C9B  shl     bp,1 */
    BP = (uint16_t)(BP << 1);
L0C9D:
    /* 0C9D  sub     dx,40h */
    DX = (uint16_t)(DX - 0x40);
L0CA0:
    /* 0CA0  add     cx,40h */
    CX = (uint16_t)(CX + 0x40);
L0CA3:
    /* 0CA3  shr     word ptr cs:L0BC1,1 */
    ww(CODE003, 0xBC1, shr16(rw(CODE003, 0xBC1), 1));
L0CA8:
    /* 0CA8  jae     L0CAC */
    if (!CF) goto L0CAC;
L0CAA:
    /* 0CAA  xchg    cx,dx */
    { uint16_t t_ = DX;
    DX = CX;
    CX = t_; }
L0CAC: /* L0CAC */
    /* 0CAC  mov     bx,seg seg004_0849 */
    BX = (uint16_t)(0x065C + PORT_LOAD_SEG);
L0CAF:
    /* 0CAF  mov     es,bx */
    SET_ES(BX);
L0CB1:
    /* 0CB1  mov     bl,byte ptr es:[6D20h] */
    BL = rb(pES, 0x6D20);
L0CB6:
    /* 0CB6  mov     si,6D3Eh */
    SI = 0x6D3E;
L0CB9: /* L0CB9 */
    /* 0CB9  mov     bh,dh */
    BH = DH;
L0CBB:
    /* 0CBB  mov     bh,byte ptr es:[bx+si] */
    BH = rb(pES, BX + SI);
L0CBE:
    /* 0CBE  mov     byte ptr [di],bh */
    wb(pDS, DI, BH);
L0CC0:
    /* 0CC0  add     dx,bp */
    DX = (uint16_t)(DX + BP);
L0CC2:
    /* 0CC2  inc     di */
    DI = (uint16_t)(DI + 1);
L0CC3:
    /* 0CC3  dec     ax */
    AX = dec16(AX);
L0CC4:
    /* 0CC4  je      L0CD3 */
    if (ZF) goto L0CD3;
L0CC6:
    /* 0CC6  mov     bh,ch */
    BH = CH;
L0CC8:
    /* 0CC8  mov     bh,byte ptr es:[bx+si] */
    BH = rb(pES, BX + SI);
L0CCB:
    /* 0CCB  mov     byte ptr [di],bh */
    wb(pDS, DI, BH);
L0CCD:
    /* 0CCD  add     cx,bp */
    CX = (uint16_t)(CX + BP);
L0CCF:
    /* 0CCF  inc     di */
    DI = (uint16_t)(DI + 1);
L0CD0:
    /* 0CD0  dec     ax */
    AX = dec16(AX);
L0CD1:
    /* 0CD1  jne     L0CB9 */
    if (!ZF) goto L0CB9;
L0CD3: /* L0CD3 */
    /* 0CD3  pop     es */
    SET_ES(pop16());
L0CD4:
    /* 0CD4  pop     si */
    SI = pop16();
L0CD5:
    /* 0CD5  pop     ds */
    SET_DS(pop16());
L0CD6:
    /* 0CD6  jmp     _seg003_0272_C13 */
    goto L0C13;
L0CD9:
    /* 0CD9  push    si */
    push16(SI);
L0CDA:
    /* 0CDA  mov     ax,cx */
    AX = CX;
L0CDC:
    /* 0CDC  mov     cx,bx */
    CX = BX;
L0CDE:
    /* 0CDE  shl     bp,1 */
    BP = (uint16_t)(BP << 1);
L0CE0:
    /* 0CE0  sub     bx,40h */
    BX = (uint16_t)(BX - 0x40);
L0CE3:
    /* 0CE3  add     cx,40h */
    CX = (uint16_t)(CX + 0x40);
L0CE6:
    /* 0CE6  shr     word ptr cs:L0BC1,1 */
    ww(CODE003, 0xBC1, shr16(rw(CODE003, 0xBC1), 1));
L0CEB:
    /* 0CEB  jae     L0CEF */
    if (!CF) goto L0CEF;
L0CED:
    /* 0CED  xchg    cx,bx */
    { uint16_t t_ = BX;
    BX = CX;
    CX = t_; }
L0CEF: /* L0CEF */
    /* 0CEF  mov     dx,bx */
    DX = BX;
L0CF1:
    /* 0CF1  push    es */
    push16(asm_es);
L0CF2:
    /* 0CF2  mov     bx,seg seg004_0849 */
    BX = (uint16_t)(0x065C + PORT_LOAD_SEG);
L0CF5:
    /* 0CF5  mov     es,bx */
    SET_ES(BX);
L0CF7:
    /* 0CF7  mov     si,6D3Eh */
    SI = 0x6D3E;
L0CFA: /* L0CFA */
    /* 0CFA  mov     bl,byte ptr [di] */
    BL = rb(pDS, DI);
L0CFC:
    /* 0CFC  mov     bh,dh */
    BH = DH;
L0CFE:
    /* 0CFE  mov     bh,byte ptr es:[bx+si] */
    BH = rb(pES, BX + SI);
L0D01:
    /* 0D01  mov     byte ptr [di],bh */
    wb(pDS, DI, BH);
L0D03:
    /* 0D03  add     dx,bp */
    DX = (uint16_t)(DX + BP);
L0D05:
    /* 0D05  inc     di */
    DI = (uint16_t)(DI + 1);
L0D06:
    /* 0D06  dec     ax */
    AX = dec16(AX);
L0D07:
    /* 0D07  je      L0D18 */
    if (ZF) goto L0D18;
L0D09:
    /* 0D09  mov     bl,byte ptr [di] */
    BL = rb(pDS, DI);
L0D0B:
    /* 0D0B  mov     bh,ch */
    BH = CH;
L0D0D:
    /* 0D0D  mov     bh,byte ptr es:[bx+si] */
    BH = rb(pES, BX + SI);
L0D10:
    /* 0D10  mov     byte ptr [di],bh */
    wb(pDS, DI, BH);
L0D12:
    /* 0D12  add     cx,bp */
    CX = (uint16_t)(CX + BP);
L0D14:
    /* 0D14  inc     di */
    DI = (uint16_t)(DI + 1);
L0D15:
    /* 0D15  dec     ax */
    AX = dec16(AX);
L0D16:
    /* 0D16  jne     L0CFA */
    if (!ZF) goto L0CFA;
L0D18: /* L0D18 */
    /* 0D18  pop     es */
    SET_ES(pop16());
L0D19:
    /* 0D19  pop     si */
    SI = pop16();
L0D1A:
    /* 0D1A  pop     ds */
    SET_DS(pop16());
L0D1B:
    /* 0D1B  jmp     _seg003_0272_C13 */
    goto L0C13;
}
