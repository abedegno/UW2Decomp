/* smooth.c: replaces src/3d/SMOOTH.ASM (seg004_0849_600, 0600..0DA9 of its
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
    case 0x0600: goto L0600;
    case 0x0606: goto L0606;
    case 0x0608: goto L0608;
    case 0x060A: goto L060A;
    case 0x0610: goto L0610;
    case 0x0615: goto L0615;
    case 0x0617: goto L0617;
    case 0x061A: goto L061A;
    case 0x061D: goto L061D;
    case 0x0621: goto L0621;
    case 0x0626: goto L0626;
    case 0x0628: goto L0628;
    case 0x0629: goto L0629;
    case 0x062C: goto L062C;
    case 0x062E: goto L062E;
    case 0x0631: goto L0631;
    case 0x0634: goto L0634;
    case 0x0638: goto L0638;
    case 0x063C: goto L063C;
    case 0x063D: goto L063D;
    case 0x0640: goto L0640;
    case 0x0642: goto L0642;
    case 0x0644: goto L0644;
    case 0x0647: goto L0647;
    case 0x0648: goto L0648;
    case 0x0649: goto L0649;
    case 0x064D: goto L064D;
    case 0x064F: goto L064F;
    case 0x0652: goto L0652;
    case 0x0653: goto L0653;
    case 0x0656: goto L0656;
    case 0x0659: goto L0659;
    case 0x065C: goto L065C;
    case 0x065F: goto L065F;
    case 0x0663: goto L0663;
    case 0x0665: goto L0665;
    case 0x0669: goto L0669;
    case 0x066D: goto L066D;
    case 0x066F: goto L066F;
    case 0x0671: goto L0671;
    case 0x0673: goto L0673;
    case 0x0675: goto L0675;
    case 0x0679: goto L0679;
    case 0x067D: goto L067D;
    case 0x0682: goto L0682;
    case 0x0684: goto L0684;
    case 0x0686: goto L0686;
    case 0x0687: goto L0687;
    case 0x0688: goto L0688;
    case 0x0689: goto L0689;
    case 0x068D: goto L068D;
    case 0x068F: goto L068F;
    case 0x0690: goto L0690;
    case 0x0697: goto L0697;
    case 0x069A: goto L069A;
    case 0x069C: goto L069C;
    case 0x069F: goto L069F;
    case 0x06A2: goto L06A2;
    case 0x06A6: goto L06A6;
    case 0x06AA: goto L06AA;
    case 0x06AB: goto L06AB;
    case 0x06AE: goto L06AE;
    case 0x06B0: goto L06B0;
    case 0x06B2: goto L06B2;
    case 0x06B5: goto L06B5;
    case 0x06B7: goto L06B7;
    case 0x06BA: goto L06BA;
    case 0x06BB: goto L06BB;
    case 0x06BC: goto L06BC;
    case 0x06C0: goto L06C0;
    case 0x06C2: goto L06C2;
    case 0x06C5: goto L06C5;
    case 0x06C7: goto L06C7;
    case 0x06CA: goto L06CA;
    case 0x06CB: goto L06CB;
    case 0x06CE: goto L06CE;
    case 0x06D1: goto L06D1;
    case 0x06D4: goto L06D4;
    case 0x06D7: goto L06D7;
    case 0x06DB: goto L06DB;
    case 0x06DD: goto L06DD;
    case 0x06E1: goto L06E1;
    case 0x06E5: goto L06E5;
    case 0x06E7: goto L06E7;
    case 0x06E9: goto L06E9;
    case 0x06EB: goto L06EB;
    case 0x06EF: goto L06EF;
    case 0x06F3: goto L06F3;
    case 0x06F8: goto L06F8;
    case 0x06FA: goto L06FA;
    case 0x06FC: goto L06FC;
    case 0x06FD: goto L06FD;
    case 0x06FE: goto L06FE;
    case 0x06FF: goto L06FF;
    case 0x0703: goto L0703;
    case 0x070A: goto L070A;
    case 0x070F: goto L070F;
    case 0x0711: goto L0711;
    case 0x0712: goto L0712;
    case 0x0715: goto L0715;
    case 0x0716: goto L0716;
    case 0x071B: goto L071B;
    case 0x071D: goto L071D;
    case 0x0722: goto L0722;
    case 0x0724: goto L0724;
    case 0x0726: goto L0726;
    case 0x0729: goto L0729;
    case 0x072C: goto L072C;
    case 0x0731: goto L0731;
    case 0x0733: goto L0733;
    case 0x0734: goto L0734;
    case 0x0737: goto L0737;
    case 0x0738: goto L0738;
    case 0x073D: goto L073D;
    case 0x073F: goto L073F;
    case 0x0744: goto L0744;
    case 0x0746: goto L0746;
    case 0x0748: goto L0748;
    case 0x074B: goto L074B;
    case 0x074E: goto L074E;
    case 0x0753: goto L0753;
    case 0x0755: goto L0755;
    case 0x0756: goto L0756;
    case 0x0759: goto L0759;
    case 0x075A: goto L075A;
    case 0x075F: goto L075F;
    case 0x0761: goto L0761;
    case 0x0766: goto L0766;
    case 0x0768: goto L0768;
    case 0x076A: goto L076A;
    case 0x076D: goto L076D;
    case 0x0770: goto L0770;
    case 0x0775: goto L0775;
    case 0x0777: goto L0777;
    case 0x0778: goto L0778;
    case 0x077B: goto L077B;
    case 0x077C: goto L077C;
    case 0x0781: goto L0781;
    case 0x0783: goto L0783;
    case 0x0788: goto L0788;
    case 0x078A: goto L078A;
    case 0x078C: goto L078C;
    case 0x078F: goto L078F;
    case 0x0792: goto L0792;
    case 0x0793: goto L0793;
    case 0x0794: goto L0794;
    case 0x0798: goto L0798;
    case 0x0799: goto L0799;
    case 0x079C: goto L079C;
    case 0x07A3: goto L07A3;
    case 0x07A5: goto L07A5;
    case 0x07A7: goto L07A7;
    case 0x07AC: goto L07AC;
    case 0x07AE: goto L07AE;
    case 0x07B1: goto L07B1;
    case 0x07B6: goto L07B6;
    case 0x07B8: goto L07B8;
    case 0x07BD: goto L07BD;
    case 0x07BF: goto L07BF;
    case 0x07C2: goto L07C2;
    case 0x07C7: goto L07C7;
    case 0x07C9: goto L07C9;
    case 0x07CE: goto L07CE;
    case 0x07D0: goto L07D0;
    case 0x07D3: goto L07D3;
    case 0x07D8: goto L07D8;
    case 0x07DA: goto L07DA;
    case 0x07DF: goto L07DF;
    case 0x07E1: goto L07E1;
    case 0x07E4: goto L07E4;
    case 0x07E9: goto L07E9;
    case 0x07EB: goto L07EB;
    case 0x07EE: goto L07EE;
    case 0x07EF: goto L07EF;
    case 0x07F2: goto L07F2;
    case 0x07F4: goto L07F4;
    case 0x07F6: goto L07F6;
    case 0x07F7: goto L07F7;
    case 0x07F8: goto L07F8;
    case 0x07FC: goto L07FC;
    case 0x0801: goto L0801;
    case 0x0806: goto L0806;
    case 0x080A: goto L080A;
    case 0x080E: goto L080E;
    case 0x0811: goto L0811;
    case 0x0813: goto L0813;
    case 0x0818: goto L0818;
    case 0x081C: goto L081C;
    case 0x081F: goto L081F;
    case 0x0823: goto L0823;
    case 0x0825: goto L0825;
    case 0x0828: goto L0828;
    case 0x082C: goto L082C;
    case 0x082F: goto L082F;
    case 0x0833: goto L0833;
    case 0x0835: goto L0835;
    case 0x0838: goto L0838;
    case 0x083B: goto L083B;
    case 0x083D: goto L083D;
    case 0x083F: goto L083F;
    case 0x0843: goto L0843;
    case 0x0847: goto L0847;
    case 0x084A: goto L084A;
    case 0x084C: goto L084C;
    case 0x084E: goto L084E;
    case 0x0852: goto L0852;
    case 0x0854: goto L0854;
    case 0x0857: goto L0857;
    case 0x085A: goto L085A;
    case 0x085C: goto L085C;
    case 0x085F: goto L085F;
    case 0x0862: goto L0862;
    case 0x0865: goto L0865;
    case 0x0868: goto L0868;
    case 0x0869: goto L0869;
    case 0x086B: goto L086B;
    case 0x086D: goto L086D;
    case 0x0870: goto L0870;
    case 0x0874: goto L0874;
    case 0x0876: goto L0876;
    case 0x0879: goto L0879;
    case 0x087B: goto L087B;
    case 0x087D: goto L087D;
    case 0x087F: goto L087F;
    case 0x0881: goto L0881;
    case 0x0883: goto L0883;
    case 0x0885: goto L0885;
    case 0x0886: goto L0886;
    case 0x0888: goto L0888;
    case 0x088B: goto L088B;
    case 0x088C: goto L088C;
    case 0x088E: goto L088E;
    case 0x0891: goto L0891;
    case 0x0894: goto L0894;
    case 0x0896: goto L0896;
    case 0x0898: goto L0898;
    case 0x089A: goto L089A;
    case 0x089C: goto L089C;
    case 0x089E: goto L089E;
    case 0x08A0: goto L08A0;
    case 0x08A1: goto L08A1;
    case 0x08A3: goto L08A3;
    case 0x08A6: goto L08A6;
    case 0x08A7: goto L08A7;
    case 0x08A8: goto L08A8;
    case 0x08AA: goto L08AA;
    case 0x08AC: goto L08AC;
    case 0x08AF: goto L08AF;
    case 0x08B0: goto L08B0;
    case 0x08B1: goto L08B1;
    case 0x08B5: goto L08B5;
    case 0x08B9: goto L08B9;
    case 0x08BD: goto L08BD;
    case 0x08BF: goto L08BF;
    case 0x08C2: goto L08C2;
    case 0x08C5: goto L08C5;
    case 0x08C8: goto L08C8;
    case 0x08CA: goto L08CA;
    case 0x08CD: goto L08CD;
    case 0x08D0: goto L08D0;
    case 0x08D3: goto L08D3;
    case 0x08D6: goto L08D6;
    case 0x08D7: goto L08D7;
    case 0x08D9: goto L08D9;
    case 0x08DB: goto L08DB;
    case 0x08DE: goto L08DE;
    case 0x08E2: goto L08E2;
    case 0x08E5: goto L08E5;
    case 0x08E7: goto L08E7;
    case 0x08E9: goto L08E9;
    case 0x08EB: goto L08EB;
    case 0x08ED: goto L08ED;
    case 0x08EF: goto L08EF;
    case 0x08F1: goto L08F1;
    case 0x08F3: goto L08F3;
    case 0x08F4: goto L08F4;
    case 0x08F6: goto L08F6;
    case 0x08F8: goto L08F8;
    case 0x08F9: goto L08F9;
    case 0x08FB: goto L08FB;
    case 0x08FE: goto L08FE;
    case 0x0901: goto L0901;
    case 0x0903: goto L0903;
    case 0x0905: goto L0905;
    case 0x0907: goto L0907;
    case 0x0909: goto L0909;
    case 0x090B: goto L090B;
    case 0x090D: goto L090D;
    case 0x090E: goto L090E;
    case 0x0910: goto L0910;
    case 0x0913: goto L0913;
    case 0x0914: goto L0914;
    case 0x0915: goto L0915;
    case 0x0917: goto L0917;
    case 0x0919: goto L0919;
    case 0x091C: goto L091C;
    case 0x091D: goto L091D;
    case 0x091E: goto L091E;
    case 0x0922: goto L0922;
    case 0x0926: goto L0926;
    case 0x0929: goto L0929;
    case 0x092D: goto L092D;
    case 0x092E: goto L092E;
    case 0x0933: goto L0933;
    case 0x0938: goto L0938;
    case 0x093C: goto L093C;
    case 0x0940: goto L0940;
    case 0x0943: goto L0943;
    case 0x0945: goto L0945;
    case 0x094A: goto L094A;
    case 0x094E: goto L094E;
    case 0x0951: goto L0951;
    case 0x0955: goto L0955;
    case 0x0957: goto L0957;
    case 0x095A: goto L095A;
    case 0x095E: goto L095E;
    case 0x0961: goto L0961;
    case 0x0965: goto L0965;
    case 0x0967: goto L0967;
    case 0x096A: goto L096A;
    case 0x096D: goto L096D;
    case 0x096F: goto L096F;
    case 0x0971: goto L0971;
    case 0x0975: goto L0975;
    case 0x0979: goto L0979;
    case 0x097C: goto L097C;
    case 0x097E: goto L097E;
    case 0x0980: goto L0980;
    case 0x0984: goto L0984;
    case 0x0986: goto L0986;
    case 0x0989: goto L0989;
    case 0x098C: goto L098C;
    case 0x098E: goto L098E;
    case 0x0991: goto L0991;
    case 0x0994: goto L0994;
    case 0x0997: goto L0997;
    case 0x099A: goto L099A;
    case 0x099B: goto L099B;
    case 0x099D: goto L099D;
    case 0x099F: goto L099F;
    case 0x09A2: goto L09A2;
    case 0x09A6: goto L09A6;
    case 0x09A8: goto L09A8;
    case 0x09AB: goto L09AB;
    case 0x09AD: goto L09AD;
    case 0x09AF: goto L09AF;
    case 0x09B1: goto L09B1;
    case 0x09B3: goto L09B3;
    case 0x09B5: goto L09B5;
    case 0x09B7: goto L09B7;
    case 0x09B8: goto L09B8;
    case 0x09BA: goto L09BA;
    case 0x09BD: goto L09BD;
    case 0x09BE: goto L09BE;
    case 0x09C0: goto L09C0;
    case 0x09C3: goto L09C3;
    case 0x09C6: goto L09C6;
    case 0x09C8: goto L09C8;
    case 0x09CA: goto L09CA;
    case 0x09CC: goto L09CC;
    case 0x09CE: goto L09CE;
    case 0x09D0: goto L09D0;
    case 0x09D2: goto L09D2;
    case 0x09D3: goto L09D3;
    case 0x09D5: goto L09D5;
    case 0x09D8: goto L09D8;
    case 0x09D9: goto L09D9;
    case 0x09DB: goto L09DB;
    case 0x09DD: goto L09DD;
    case 0x09DE: goto L09DE;
    case 0x09E0: goto L09E0;
    case 0x09E3: goto L09E3;
    case 0x09E4: goto L09E4;
    case 0x09E5: goto L09E5;
    case 0x09E9: goto L09E9;
    case 0x09ED: goto L09ED;
    case 0x09F1: goto L09F1;
    case 0x09F3: goto L09F3;
    case 0x09F6: goto L09F6;
    case 0x09F9: goto L09F9;
    case 0x09FC: goto L09FC;
    case 0x09FE: goto L09FE;
    case 0x0A01: goto L0A01;
    case 0x0A04: goto L0A04;
    case 0x0A07: goto L0A07;
    case 0x0A0A: goto L0A0A;
    case 0x0A0B: goto L0A0B;
    case 0x0A0D: goto L0A0D;
    case 0x0A0F: goto L0A0F;
    case 0x0A12: goto L0A12;
    case 0x0A16: goto L0A16;
    case 0x0A19: goto L0A19;
    case 0x0A1B: goto L0A1B;
    case 0x0A1D: goto L0A1D;
    case 0x0A1F: goto L0A1F;
    case 0x0A21: goto L0A21;
    case 0x0A23: goto L0A23;
    case 0x0A25: goto L0A25;
    case 0x0A27: goto L0A27;
    case 0x0A28: goto L0A28;
    case 0x0A2A: goto L0A2A;
    case 0x0A2C: goto L0A2C;
    case 0x0A2D: goto L0A2D;
    case 0x0A2F: goto L0A2F;
    case 0x0A32: goto L0A32;
    case 0x0A35: goto L0A35;
    case 0x0A37: goto L0A37;
    case 0x0A39: goto L0A39;
    case 0x0A3B: goto L0A3B;
    case 0x0A3D: goto L0A3D;
    case 0x0A3F: goto L0A3F;
    case 0x0A41: goto L0A41;
    case 0x0A42: goto L0A42;
    case 0x0A44: goto L0A44;
    case 0x0A47: goto L0A47;
    case 0x0A48: goto L0A48;
    case 0x0A4A: goto L0A4A;
    case 0x0A4C: goto L0A4C;
    case 0x0A4D: goto L0A4D;
    case 0x0A4F: goto L0A4F;
    case 0x0A52: goto L0A52;
    case 0x0A53: goto L0A53;
    case 0x0A54: goto L0A54;
    case 0x0A58: goto L0A58;
    case 0x0A5C: goto L0A5C;
    case 0x0A5F: goto L0A5F;
    case 0x0A63: goto L0A63;
    case 0x0A64: goto L0A64;
    case 0x0A69: goto L0A69;
    case 0x0A6E: goto L0A6E;
    case 0x0A72: goto L0A72;
    case 0x0A76: goto L0A76;
    case 0x0A79: goto L0A79;
    case 0x0A7B: goto L0A7B;
    case 0x0A80: goto L0A80;
    case 0x0A84: goto L0A84;
    case 0x0A87: goto L0A87;
    case 0x0A8B: goto L0A8B;
    case 0x0A8D: goto L0A8D;
    case 0x0A90: goto L0A90;
    case 0x0A94: goto L0A94;
    case 0x0A97: goto L0A97;
    case 0x0A9B: goto L0A9B;
    case 0x0A9D: goto L0A9D;
    case 0x0AA0: goto L0AA0;
    case 0x0AA3: goto L0AA3;
    case 0x0AA5: goto L0AA5;
    case 0x0AA7: goto L0AA7;
    case 0x0AAB: goto L0AAB;
    case 0x0AAF: goto L0AAF;
    case 0x0AB2: goto L0AB2;
    case 0x0AB4: goto L0AB4;
    case 0x0AB6: goto L0AB6;
    case 0x0ABA: goto L0ABA;
    case 0x0ABC: goto L0ABC;
    case 0x0ABF: goto L0ABF;
    case 0x0AC2: goto L0AC2;
    case 0x0AC4: goto L0AC4;
    case 0x0AC7: goto L0AC7;
    case 0x0AC9: goto L0AC9;
    case 0x0ACC: goto L0ACC;
    case 0x0ACF: goto L0ACF;
    case 0x0AD0: goto L0AD0;
    case 0x0AD2: goto L0AD2;
    case 0x0AD4: goto L0AD4;
    case 0x0AD7: goto L0AD7;
    case 0x0ADB: goto L0ADB;
    case 0x0ADD: goto L0ADD;
    case 0x0AE0: goto L0AE0;
    case 0x0AE2: goto L0AE2;
    case 0x0AE4: goto L0AE4;
    case 0x0AE6: goto L0AE6;
    case 0x0AE8: goto L0AE8;
    case 0x0AEA: goto L0AEA;
    case 0x0AEC: goto L0AEC;
    case 0x0AED: goto L0AED;
    case 0x0AEF: goto L0AEF;
    case 0x0AF2: goto L0AF2;
    case 0x0AF3: goto L0AF3;
    case 0x0AF5: goto L0AF5;
    case 0x0AF8: goto L0AF8;
    case 0x0AFB: goto L0AFB;
    case 0x0AFD: goto L0AFD;
    case 0x0AFF: goto L0AFF;
    case 0x0B01: goto L0B01;
    case 0x0B03: goto L0B03;
    case 0x0B05: goto L0B05;
    case 0x0B07: goto L0B07;
    case 0x0B08: goto L0B08;
    case 0x0B0A: goto L0B0A;
    case 0x0B0D: goto L0B0D;
    case 0x0B0E: goto L0B0E;
    case 0x0B10: goto L0B10;
    case 0x0B12: goto L0B12;
    case 0x0B13: goto L0B13;
    case 0x0B15: goto L0B15;
    case 0x0B18: goto L0B18;
    case 0x0B19: goto L0B19;
    case 0x0B1A: goto L0B1A;
    case 0x0B1E: goto L0B1E;
    case 0x0B22: goto L0B22;
    case 0x0B26: goto L0B26;
    case 0x0B28: goto L0B28;
    case 0x0B2B: goto L0B2B;
    case 0x0B2D: goto L0B2D;
    case 0x0B30: goto L0B30;
    case 0x0B32: goto L0B32;
    case 0x0B35: goto L0B35;
    case 0x0B38: goto L0B38;
    case 0x0B3B: goto L0B3B;
    case 0x0B3E: goto L0B3E;
    case 0x0B3F: goto L0B3F;
    case 0x0B41: goto L0B41;
    case 0x0B43: goto L0B43;
    case 0x0B46: goto L0B46;
    case 0x0B4A: goto L0B4A;
    case 0x0B4D: goto L0B4D;
    case 0x0B4F: goto L0B4F;
    case 0x0B51: goto L0B51;
    case 0x0B53: goto L0B53;
    case 0x0B55: goto L0B55;
    case 0x0B57: goto L0B57;
    case 0x0B59: goto L0B59;
    case 0x0B5B: goto L0B5B;
    case 0x0B5C: goto L0B5C;
    case 0x0B5E: goto L0B5E;
    case 0x0B60: goto L0B60;
    case 0x0B61: goto L0B61;
    case 0x0B63: goto L0B63;
    case 0x0B66: goto L0B66;
    case 0x0B69: goto L0B69;
    case 0x0B6B: goto L0B6B;
    case 0x0B6D: goto L0B6D;
    case 0x0B6F: goto L0B6F;
    case 0x0B71: goto L0B71;
    case 0x0B73: goto L0B73;
    case 0x0B75: goto L0B75;
    case 0x0B76: goto L0B76;
    case 0x0B78: goto L0B78;
    case 0x0B7B: goto L0B7B;
    case 0x0B7C: goto L0B7C;
    case 0x0B7E: goto L0B7E;
    case 0x0B80: goto L0B80;
    case 0x0B81: goto L0B81;
    case 0x0B83: goto L0B83;
    case 0x0B86: goto L0B86;
    case 0x0B87: goto L0B87;
    case 0x0B88: goto L0B88;
    case 0x0B8C: goto L0B8C;
    case 0x0B90: goto L0B90;
    case 0x0B93: goto L0B93;
    case 0x0B97: goto L0B97;
    case 0x0B98: goto L0B98;
    case 0x0B9D: goto L0B9D;
    case 0x0BA2: goto L0BA2;
    case 0x0BA6: goto L0BA6;
    case 0x0BAA: goto L0BAA;
    case 0x0BAD: goto L0BAD;
    case 0x0BAF: goto L0BAF;
    case 0x0BB4: goto L0BB4;
    case 0x0BB8: goto L0BB8;
    case 0x0BBB: goto L0BBB;
    case 0x0BBF: goto L0BBF;
    case 0x0BC1: goto L0BC1;
    case 0x0BC4: goto L0BC4;
    case 0x0BC8: goto L0BC8;
    case 0x0BCB: goto L0BCB;
    case 0x0BCF: goto L0BCF;
    case 0x0BD1: goto L0BD1;
    case 0x0BD4: goto L0BD4;
    case 0x0BD7: goto L0BD7;
    case 0x0BD9: goto L0BD9;
    case 0x0BDB: goto L0BDB;
    case 0x0BDF: goto L0BDF;
    case 0x0BE3: goto L0BE3;
    case 0x0BE6: goto L0BE6;
    case 0x0BE8: goto L0BE8;
    case 0x0BEA: goto L0BEA;
    case 0x0BEE: goto L0BEE;
    case 0x0BF0: goto L0BF0;
    case 0x0BF3: goto L0BF3;
    case 0x0BF6: goto L0BF6;
    case 0x0BF8: goto L0BF8;
    case 0x0BFB: goto L0BFB;
    case 0x0BFD: goto L0BFD;
    case 0x0C00: goto L0C00;
    case 0x0C03: goto L0C03;
    case 0x0C04: goto L0C04;
    case 0x0C06: goto L0C06;
    case 0x0C08: goto L0C08;
    case 0x0C0B: goto L0C0B;
    case 0x0C0F: goto L0C0F;
    case 0x0C11: goto L0C11;
    case 0x0C14: goto L0C14;
    case 0x0C16: goto L0C16;
    case 0x0C18: goto L0C18;
    case 0x0C1A: goto L0C1A;
    case 0x0C1C: goto L0C1C;
    case 0x0C1E: goto L0C1E;
    case 0x0C20: goto L0C20;
    case 0x0C21: goto L0C21;
    case 0x0C23: goto L0C23;
    case 0x0C26: goto L0C26;
    case 0x0C27: goto L0C27;
    case 0x0C29: goto L0C29;
    case 0x0C2C: goto L0C2C;
    case 0x0C2F: goto L0C2F;
    case 0x0C31: goto L0C31;
    case 0x0C33: goto L0C33;
    case 0x0C35: goto L0C35;
    case 0x0C37: goto L0C37;
    case 0x0C39: goto L0C39;
    case 0x0C3B: goto L0C3B;
    case 0x0C3C: goto L0C3C;
    case 0x0C3E: goto L0C3E;
    case 0x0C41: goto L0C41;
    case 0x0C42: goto L0C42;
    case 0x0C44: goto L0C44;
    case 0x0C46: goto L0C46;
    case 0x0C48: goto L0C48;
    case 0x0C49: goto L0C49;
    case 0x0C4B: goto L0C4B;
    case 0x0C4E: goto L0C4E;
    case 0x0C4F: goto L0C4F;
    case 0x0C50: goto L0C50;
    case 0x0C54: goto L0C54;
    case 0x0C58: goto L0C58;
    case 0x0C5C: goto L0C5C;
    case 0x0C5E: goto L0C5E;
    case 0x0C61: goto L0C61;
    case 0x0C64: goto L0C64;
    case 0x0C66: goto L0C66;
    case 0x0C68: goto L0C68;
    case 0x0C6B: goto L0C6B;
    case 0x0C6E: goto L0C6E;
    case 0x0C71: goto L0C71;
    case 0x0C74: goto L0C74;
    case 0x0C75: goto L0C75;
    case 0x0C77: goto L0C77;
    case 0x0C79: goto L0C79;
    case 0x0C7C: goto L0C7C;
    case 0x0C80: goto L0C80;
    case 0x0C83: goto L0C83;
    case 0x0C85: goto L0C85;
    case 0x0C87: goto L0C87;
    case 0x0C89: goto L0C89;
    case 0x0C8B: goto L0C8B;
    case 0x0C8D: goto L0C8D;
    case 0x0C8F: goto L0C8F;
    case 0x0C91: goto L0C91;
    case 0x0C92: goto L0C92;
    case 0x0C94: goto L0C94;
    case 0x0C96: goto L0C96;
    case 0x0C97: goto L0C97;
    case 0x0C99: goto L0C99;
    case 0x0C9C: goto L0C9C;
    case 0x0C9F: goto L0C9F;
    case 0x0CA1: goto L0CA1;
    case 0x0CA3: goto L0CA3;
    case 0x0CA5: goto L0CA5;
    case 0x0CA7: goto L0CA7;
    case 0x0CA9: goto L0CA9;
    case 0x0CAB: goto L0CAB;
    case 0x0CAC: goto L0CAC;
    case 0x0CAE: goto L0CAE;
    case 0x0CB1: goto L0CB1;
    case 0x0CB2: goto L0CB2;
    case 0x0CB4: goto L0CB4;
    case 0x0CB6: goto L0CB6;
    case 0x0CB8: goto L0CB8;
    case 0x0CB9: goto L0CB9;
    case 0x0CBB: goto L0CBB;
    case 0x0CBE: goto L0CBE;
    case 0x0CBF: goto L0CBF;
    case 0x0CC0: goto L0CC0;
    case 0x0CC4: goto L0CC4;
    case 0x0CC8: goto L0CC8;
    case 0x0CCB: goto L0CCB;
    case 0x0CCF: goto L0CCF;
    case 0x0CD0: goto L0CD0;
    case 0x0CD3: goto L0CD3;
    case 0x0CD5: goto L0CD5;
    case 0x0CD7: goto L0CD7;
    case 0x0CDA: goto L0CDA;
    case 0x0CDD: goto L0CDD;
    case 0x0CDF: goto L0CDF;
    case 0x0CE0: goto L0CE0;
    case 0x0CE1: goto L0CE1;
    case 0x0CE4: goto L0CE4;
    case 0x0CE6: goto L0CE6;
    case 0x0CE7: goto L0CE7;
    case 0x0CEA: goto L0CEA;
    case 0x0CEC: goto L0CEC;
    case 0x0CEE: goto L0CEE;
    case 0x0CF0: goto L0CF0;
    case 0x0CF1: goto L0CF1;
    case 0x0CF4: goto L0CF4;
    case 0x0CF6: goto L0CF6;
    case 0x0CF8: goto L0CF8;
    case 0x0CFA: goto L0CFA;
    case 0x0CFB: goto L0CFB;
    case 0x0CFE: goto L0CFE;
    case 0x0D00: goto L0D00;
    case 0x0D02: goto L0D02;
    case 0x0D04: goto L0D04;
    case 0x0D06: goto L0D06;
    case 0x0D09: goto L0D09;
    case 0x0D0B: goto L0D0B;
    case 0x0D0D: goto L0D0D;
    case 0x0D0F: goto L0D0F;
    case 0x0D11: goto L0D11;
    case 0x0D13: goto L0D13;
    case 0x0D15: goto L0D15;
    case 0x0D17: goto L0D17;
    case 0x0D19: goto L0D19;
    case 0x0D1B: goto L0D1B;
    case 0x0D1D: goto L0D1D;
    case 0x0D1F: goto L0D1F;
    case 0x0D21: goto L0D21;
    case 0x0D23: goto L0D23;
    case 0x0D25: goto L0D25;
    case 0x0D27: goto L0D27;
    case 0x0D29: goto L0D29;
    case 0x0D2B: goto L0D2B;
    case 0x0D2D: goto L0D2D;
    case 0x0D2F: goto L0D2F;
    case 0x0D31: goto L0D31;
    case 0x0D33: goto L0D33;
    case 0x0D37: goto L0D37;
    case 0x0D39: goto L0D39;
    case 0x0D3C: goto L0D3C;
    case 0x0D3E: goto L0D3E;
    case 0x0D42: goto L0D42;
    case 0x0D44: goto L0D44;
    case 0x0D46: goto L0D46;
    case 0x0D4A: goto L0D4A;
    case 0x0D4C: goto L0D4C;
    case 0x0D4E: goto L0D4E;
    case 0x0D50: goto L0D50;
    case 0x0D51: goto L0D51;
    case 0x0D52: goto L0D52;
    case 0x0D54: goto L0D54;
    case 0x0D57: goto L0D57;
    case 0x0D59: goto L0D59;
    case 0x0D5C: goto L0D5C;
    case 0x0D5E: goto L0D5E;
    case 0x0D65: goto L0D65;
    case 0x0D6C: goto L0D6C;
    case 0x0D6E: goto L0D6E;
    case 0x0D71: goto L0D71;
    case 0x0D73: goto L0D73;
    case 0x0D75: goto L0D75;
    case 0x0D79: goto L0D79;
    case 0x0D7E: goto L0D7E;
    case 0x0D83: goto L0D83;
    case 0x0D88: goto L0D88;
    case 0x0D8A: goto L0D8A;
    case 0x0D8E: goto L0D8E;
    case 0x0D90: goto L0D90;
    case 0x0D92: goto L0D92;
    case 0x0D94: goto L0D94;
    case 0x0D96: goto L0D96;
    case 0x0D98: goto L0D98;
    case 0x0D9D: goto L0D9D;
    case 0x0D9E: goto L0D9E;
    case 0x0DA1: goto L0DA1;
    case 0x0DA3: goto L0DA3;
    case 0x0DA8: goto L0DA8;
    default: asm_bad_entry("SMOOTH.ASM", entry);
    }

    /* seg004_0849_600  (+600)
       Draws the Gouraud polygon in the vertex buffer (ds:1B8h..1B6h). Raises int 2 if the buffer
       has run past 0B1Ah. Codes AND nonzero: skipped. Codes OR zero: projected (the add
       immediates at seg004_0849_644 and 64F are the screen centre, patched by render_3d) with
       the shades copied to seg_370D:55EEh, then drawn with BP = ds:[14A8h]. Codes OR with bit 80h
       (behind the eye): clipped against top, bottom, left and right in turn, retrying after each.
       Otherwise projected with overflow checks (immediates at 6B2 and 6C2), drawn with BP =
       ds:[14A6h], or clipped on overflow (L079C). Continues with the next opcode. */
L0600: /* _smooth_closure */
    /* 0600  cmp     word ptr ds:[1B6h],0B1Ah */
    sub16(rw(pDS, 0x1B6), 0xB1A, 0);
L0606:
    /* 0606  jle     short L060A */
    if (ZF || SF != OF) goto L060A;
L0608:
    /* 0608  int     2 */
    asm_halt_at(0x065C, 0x0608, "int 2h, the debugger break");
L060A: /* L060A */
    /* 060A  mov     word ptr ds:[25E4h],0 */
    ww(pDS, 0x25E4, 0x0);
L0610:
    /* 0610  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L0615:
    /* 0615  je      short L061A */
    if (ZF) goto L061A;
L0617:
    /* 0617  jmp     L0792 */
    goto L0792;
L061A: /* L061A */
    /* 061A  mov     di,1BAh */
    DI = 0x1BA;
L061D:
    /* 061D  mov     word ptr ds:[1B8h],di */
    ww(pDS, 0x1B8, DI);
L0621:
    /* 0621  test    byte ptr ds:[14CAh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0xFF));
L0626:
    /* 0626  jne     short L068D */
    if (!ZF) goto L068D;
L0628: /* L0628 */
    /* 0628  push    si */
    push16(SI);
L0629:
    /* 0629  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L062C:
    /* 062C  mov     es,ax */
    SET_ES(AX);
L062E:
    /* 062E  mov     di,415Eh */
    DI = 0x415E;
L0631:
    /* 0631  mov     bx,55EEh */
    BX = 0x55EE;
L0634:
    /* 0634  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L0638:
    /* 0638  mov     cx,word ptr ds:[2472h] */
    CX = rw(pDS, 0x2472);
L063C: /* L063C */
    /* 063C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L063D:
    /* 063D  mov     bp,word ptr [si+2] */
    BP = rw(pDS, SI + 0x2);
L0640:
    /* 0640  imul    cx */
    imul16(CX);
L0642:
    /* 0642  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x0642, 2)) != 0) return c;
L0644: /* _seg004_0849_644 */
    /* 0644  add     ax,1234h */
    AX = (uint16_t)(AX + rw(CODE004, 0x0645));
L0647:
    /* 0647  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0648:
    /* 0648  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0649:
    /* 0649  imul    word ptr ds:[2470h] */
    imul16(rw(pDS, 0x2470));
L064D:
    /* 064D  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x064D, 2)) != 0) return c;
L064F: /* _seg004_0849_64F */
    /* 064F  add     ax,1234h */
    AX = (uint16_t)(AX + rw(CODE004, 0x0650));
L0652:
    /* 0652  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0653:
    /* 0653  mov     al,byte ptr [si+3] */
    AL = rb(pDS, SI + 0x3);
L0656:
    /* 0656  mov     byte ptr es:[bx],al */
    wb(pES, BX, AL);
L0659:
    /* 0659  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L065C:
    /* 065C  add     bx,2 */
    BX = (uint16_t)(BX + 0x2);
L065F:
    /* 065F  cmp     si,word ptr ds:[1B6h] */
    sub16(SI, rw(pDS, 0x1B6), 0);
L0663:
    /* 0663  jne     L063C */
    if (!ZF) goto L063C;
L0665:
    /* 0665  mov     bx,word ptr ds:[1B6h] */
    BX = rw(pDS, 0x1B6);
L0669:
    /* 0669  sub     bx,word ptr ds:[1B8h] */
    BX = (uint16_t)(BX - rw(pDS, 0x1B8));
L066D:
    /* 066D  shr     bx,1 */
    BX = (uint16_t)(BX >> 1);
L066F:
    /* 066F  shr     bx,1 */
    BX = (uint16_t)(BX >> 1);
L0671:
    /* 0671  shr     bx,1 */
    BX = (uint16_t)(BX >> 1);
L0673:
    /* 0673  mov     cx,bx */
    CX = BX;
L0675:
    /* 0675  inc     word ptr ds:[25E4h] */
    ww(pDS, 0x25E4, (uint16_t)(rw(pDS, 0x25E4) + 1));
L0679:
    /* 0679  mov     bp,word ptr ds:[14A8h] */
    BP = rw(pDS, 0x14A8);
L067D:
    /* 067D  call    far ptr _seg003_0272_5311 */
    if ((c = asm_callf(ASM_JMP(0x0085, 0x5311), 0x065C + PORT_LOAD_SEG, 0x0682)) != 0) return c;
L0682:
    /* 0682  mov     ax,ds */
    AX = asm_ds;
L0684:
    /* 0684  mov     es,ax */
    SET_ES(AX);
L0686:
    /* 0686  pop     si */
    SI = pop16();
L0687:
    /* 0687  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0688:
    /* 0688  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L0689:
    /* 0689  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L068D: /* L068D */
    /* 068D  js      short L0703 */
    if (SF) goto L0703;
L068F:
    /* 068F  push    si */
    push16(SI);
L0690: /* L0690 */
    /* 0690  mov     word ptr ss:[5A5h],offset _seg004_0849_798 */
    ww(pSS, 0x5A5, 0x798);
L0697:
    /* 0697  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L069A:
    /* 069A  mov     es,ax */
    SET_ES(AX);
L069C:
    /* 069C  mov     di,415Eh */
    DI = 0x415E;
L069F:
    /* 069F  mov     bx,55EEh */
    BX = 0x55EE;
L06A2:
    /* 06A2  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L06A6:
    /* 06A6  mov     cx,word ptr ds:[2472h] */
    CX = rw(pDS, 0x2472);
L06AA: /* L06AA */
    /* 06AA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L06AB:
    /* 06AB  mov     bp,word ptr [si+2] */
    BP = rw(pDS, SI + 0x2);
L06AE:
    /* 06AE  imul    cx */
    imul16(CX);
L06B0:
    /* 06B0  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x06B0, 2)) != 0) return c;
L06B2: /* _seg004_0849_6B2 */
    /* 06B2  add     ax,1234h */
    AX = add16(AX, rw(CODE004, 0x06B3), 0);
L06B5:
    /* 06B5  jno     short L06BA */
    if (!OF) goto L06BA;
L06B7:
    /* 06B7  jmp     L079C */
    goto L079C;
L06BA: /* L06BA */
    /* 06BA  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L06BB:
    /* 06BB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L06BC:
    /* 06BC  imul    word ptr ds:[2470h] */
    imul16(rw(pDS, 0x2470));
L06C0:
    /* 06C0  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x065C, 0x06C0, 2)) != 0) return c;
L06C2: /* _seg004_0849_6C2 */
    /* 06C2  add     ax,1234h */
    AX = add16(AX, rw(CODE004, 0x06C3), 0);
L06C5:
    /* 06C5  jno     short L06CA */
    if (!OF) goto L06CA;
L06C7:
    /* 06C7  jmp     L079C */
    goto L079C;
L06CA: /* L06CA */
    /* 06CA  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L06CB:
    /* 06CB  mov     al,byte ptr [si+3] */
    AL = rb(pDS, SI + 0x3);
L06CE:
    /* 06CE  mov     byte ptr es:[bx],al */
    wb(pES, BX, AL);
L06D1:
    /* 06D1  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L06D4:
    /* 06D4  add     bx,2 */
    BX = (uint16_t)(BX + 0x2);
L06D7:
    /* 06D7  cmp     si,word ptr ds:[1B6h] */
    sub16(SI, rw(pDS, 0x1B6), 0);
L06DB:
    /* 06DB  jne     L06AA */
    if (!ZF) goto L06AA;
L06DD:
    /* 06DD  mov     cx,word ptr ds:[1B6h] */
    CX = rw(pDS, 0x1B6);
L06E1:
    /* 06E1  sub     cx,word ptr ds:[1B8h] */
    CX = (uint16_t)(CX - rw(pDS, 0x1B8));
L06E5:
    /* 06E5  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L06E7:
    /* 06E7  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L06E9:
    /* 06E9  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L06EB:
    /* 06EB  inc     word ptr ds:[25E4h] */
    ww(pDS, 0x25E4, (uint16_t)(rw(pDS, 0x25E4) + 1));
L06EF:
    /* 06EF  mov     bp,word ptr ds:[14A6h] */
    BP = rw(pDS, 0x14A6);
L06F3:
    /* 06F3  call    far ptr _seg003_0272_5311 */
    if ((c = asm_callf(ASM_JMP(0x0085, 0x5311), 0x065C + PORT_LOAD_SEG, 0x06F8)) != 0) return c;
L06F8:
    /* 06F8  mov     ax,ds */
    AX = asm_ds;
L06FA:
    /* 06FA  mov     es,ax */
    SET_ES(AX);
L06FC:
    /* 06FC  pop     si */
    SI = pop16();
L06FD:
    /* 06FD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L06FE:
    /* 06FE  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L06FF:
    /* 06FF  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L0703: /* L0703 */
    /* 0703  mov     word ptr ss:[5A5h],offset _seg004_0849_D83 */
    ww(pSS, 0x5A5, 0xD83);
L070A:
    /* 070A  test    byte ptr ds:[14CAh],4 */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0x4));
L070F:
    /* 070F  je      short L072C */
    if (ZF) goto L072C;
L0711:
    /* 0711  push    si */
    push16(SI);
L0712:
    /* 0712  call    _seg004_0849_7FC */
    if ((c = asm_call(ASM_JMP(0x065C, 0x07FC), 0x0715)) != 0) return c;
L0715:
    /* 0715  pop     si */
    SI = pop16();
L0716:
    /* 0716  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L071B:
    /* 071B  jne     short L0792 */
    if (!ZF) goto L0792;
L071D:
    /* 071D  test    byte ptr ds:[14CAh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0xFF));
L0722:
    /* 0722  js      short L072C */
    if (SF) goto L072C;
L0724:
    /* 0724  jne     short L0729 */
    if (!ZF) goto L0729;
L0726:
    /* 0726  jmp     L0628 */
    goto L0628;
L0729: /* L0729 */
    /* 0729  jmp     L068D */
    goto L068D;
L072C: /* L072C */
    /* 072C  test    byte ptr ds:[14CAh],8 */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0x8));
L0731:
    /* 0731  je      short L074E */
    if (ZF) goto L074E;
L0733:
    /* 0733  push    si */
    push16(SI);
L0734:
    /* 0734  call    _seg004_0849_92E */
    if ((c = asm_call(ASM_JMP(0x065C, 0x092E), 0x0737)) != 0) return c;
L0737:
    /* 0737  pop     si */
    SI = pop16();
L0738:
    /* 0738  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L073D:
    /* 073D  jne     short L0792 */
    if (!ZF) goto L0792;
L073F:
    /* 073F  test    byte ptr ds:[14CAh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0xFF));
L0744:
    /* 0744  js      short L074E */
    if (SF) goto L074E;
L0746:
    /* 0746  jne     short L074B */
    if (!ZF) goto L074B;
L0748:
    /* 0748  jmp     L0628 */
    goto L0628;
L074B: /* L074B */
    /* 074B  jmp     L068D */
    goto L068D;
L074E: /* L074E */
    /* 074E  test    byte ptr ds:[14CAh],1 */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0x1));
L0753:
    /* 0753  je      short L0770 */
    if (ZF) goto L0770;
L0755:
    /* 0755  push    si */
    push16(SI);
L0756:
    /* 0756  call    _seg004_0849_B98 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x0B98), 0x0759)) != 0) return c;
L0759:
    /* 0759  pop     si */
    SI = pop16();
L075A:
    /* 075A  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L075F:
    /* 075F  jne     short L0792 */
    if (!ZF) goto L0792;
L0761:
    /* 0761  test    byte ptr ds:[14CAh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0xFF));
L0766:
    /* 0766  js      short L0770 */
    if (SF) goto L0770;
L0768:
    /* 0768  jne     short L076D */
    if (!ZF) goto L076D;
L076A:
    /* 076A  jmp     L0628 */
    goto L0628;
L076D: /* L076D */
    /* 076D  jmp     L068D */
    goto L068D;
L0770: /* L0770 */
    /* 0770  test    byte ptr ds:[14CAh],2 */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0x2));
L0775:
    /* 0775  je      short L0792 */
    if (ZF) goto L0792;
L0777:
    /* 0777  push    si */
    push16(SI);
L0778:
    /* 0778  call    _seg004_0849_A64 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x0A64), 0x077B)) != 0) return c;
L077B:
    /* 077B  pop     si */
    SI = pop16();
L077C:
    /* 077C  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L0781:
    /* 0781  jne     short L0792 */
    if (!ZF) goto L0792;
L0783:
    /* 0783  test    byte ptr ds:[14CAh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0xFF));
L0788:
    /* 0788  js      short L0792 */
    if (SF) goto L0792;
L078A:
    /* 078A  jne     short L078F */
    if (!ZF) goto L078F;
L078C:
    /* 078C  jmp     L0628 */
    goto L0628;
L078F: /* L078F */
    /* 078F  jmp     L068D */
    goto L068D;
L0792: /* L0792 */
    /* 0792  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0793:
    /* 0793  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L0794:
    /* 0794  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_798  (+798)
       Recovery entry for the divide overflow during smooth_closure's checked projection (it is
       stored at ss:[5A5h]): drops the int 0 frame, then (L079C) clips against right, left, top
       and bottom as the codes require and projects again; gives up when nothing is left,
       restoring DS and ES to seg052_519C. The counterpart of INTERP's clip_overflow and
       do_3d_clip. */
L0798: /* _seg004_0849_798 */
    /* 0798  sti */
    ;
L0799:
    /* 0799  add     sp,6 */
    SP = (uint16_t)(SP + 0x6);
L079C: /* L079C */
    /* 079C  mov     word ptr ss:[5A5h],offset _seg004_0849_D83 */
    ww(pSS, 0x5A5, 0xD83);
L07A3:
    /* 07A3  mov     ax,ds */
    AX = asm_ds;
L07A5:
    /* 07A5  mov     es,ax */
    SET_ES(AX);
L07A7:
    /* 07A7  test    byte ptr ds:[14CAh],2 */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0x2));
L07AC:
    /* 07AC  je      short L07B8 */
    if (ZF) goto L07B8;
L07AE:
    /* 07AE  call    _seg004_0849_A64 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x0A64), 0x07B1)) != 0) return c;
L07B1:
    /* 07B1  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L07B6:
    /* 07B6  jne     short L07EE */
    if (!ZF) goto L07EE;
L07B8: /* L07B8 */
    /* 07B8  test    byte ptr ds:[14CAh],1 */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0x1));
L07BD:
    /* 07BD  je      short L07C9 */
    if (ZF) goto L07C9;
L07BF:
    /* 07BF  call    _seg004_0849_B98 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x0B98), 0x07C2)) != 0) return c;
L07C2:
    /* 07C2  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L07C7:
    /* 07C7  jne     short L07EE */
    if (!ZF) goto L07EE;
L07C9: /* L07C9 */
    /* 07C9  test    byte ptr ds:[14CAh],4 */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0x4));
L07CE:
    /* 07CE  je      short L07DA */
    if (ZF) goto L07DA;
L07D0:
    /* 07D0  call    _seg004_0849_7FC */
    if ((c = asm_call(ASM_JMP(0x065C, 0x07FC), 0x07D3)) != 0) return c;
L07D3:
    /* 07D3  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L07D8:
    /* 07D8  jne     short L07EE */
    if (!ZF) goto L07EE;
L07DA: /* L07DA */
    /* 07DA  test    byte ptr ds:[14CAh],8 */
    logic8((uint8_t)(rb(pDS, 0x14CA) & 0x8));
L07DF:
    /* 07DF  je      short L07EB */
    if (ZF) goto L07EB;
L07E1:
    /* 07E1  call    _seg004_0849_92E */
    if ((c = asm_call(ASM_JMP(0x065C, 0x092E), 0x07E4)) != 0) return c;
L07E4:
    /* 07E4  test    byte ptr ds:[14CBh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x14CB) & 0xFF));
L07E9:
    /* 07E9  jne     short L07EE */
    if (!ZF) goto L07EE;
L07EB: /* L07EB */
    /* 07EB  jmp     L0690 */
    goto L0690;
L07EE: /* L07EE */
    /* 07EE  pop     si */
    SI = pop16();
L07EF:
    /* 07EF  mov     ax,seg seg052_519C */
    AX = (uint16_t)(0x4FAF + PORT_LOAD_SEG);
L07F2:
    /* 07F2  mov     ds,ax */
    SET_DS(AX);
L07F4:
    /* 07F4  mov     es,ax */
    SET_ES(AX);
L07F6:
    /* 07F6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L07F7:
    /* 07F7  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L07F8:
    /* 07F8  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_7FC  (+7FC)
       Clips the Gouraud polygon against y = z (code bit 4), like INTERP's clip_top, also
       interpolating each new vertex's shade (+7). */
L07FC: /* _seg004_0849_7FC */
    /* 07FC  mov     byte ptr ds:[14CAh],0 */
    wb(pDS, 0x14CA, 0x0);
L0801:
    /* 0801  mov     byte ptr ds:[14CBh],0FFh */
    wb(pDS, 0x14CB, 0xFF);
L0806:
    /* 0806  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L080A:
    /* 080A  mov     di,word ptr ds:[1B6h] */
    DI = rw(pDS, 0x1B6);
L080E:
    /* 080E  mov     cx,8 */
    CX = 0x8;
L0811:
    /* 0811  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0813:
    /* 0813  add     word ptr ds:[1B6h],8 */
    ww(pDS, 0x1B6, (uint16_t)(rw(pDS, 0x1B6) + 0x8));
L0818:
    /* 0818  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L081C:
    /* 081C  mov     di,0B1Ah */
    DI = 0xB1A;
L081F:
    /* 081F  cmp     si,1BAh */
    sub16(SI, 0x1BA, 0);
L0823:
    /* 0823  je      short L0828 */
    if (ZF) goto L0828;
L0825:
    /* 0825  mov     di,1BAh */
    DI = 0x1BA;
L0828: /* L0828 */
    /* 0828  mov     word ptr ds:[1B8h],di */
    ww(pDS, 0x1B8, DI);
L082C: /* L082C */
    /* 082C  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L082F: /* L082F */
    /* 082F  cmp     si,word ptr ds:[1B6h] */
    sub16(SI, rw(pDS, 0x1B6), 0);
L0833:
    /* 0833  jne     short L0838 */
    if (!ZF) goto L0838;
L0835:
    /* 0835  jmp     L0929 */
    goto L0929;
L0838: /* L0838 */
    /* 0838  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L083B:
    /* 083B  test    al,4 */
    logic8((uint8_t)(AL & 0x4));
L083D:
    /* 083D  jne     short L084E */
    if (!ZF) goto L084E;
L083F:
    /* 083F  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L0843:
    /* 0843  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L0847:
    /* 0847  mov     cx,4 */
    CX = 0x4;
L084A:
    /* 084A  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L084C:
    /* 084C  jmp     L082F */
    goto L082F;
L084E: /* L084E */
    /* 084E  test    byte ptr [si-2],4 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFE) & 0x4));
L0852:
    /* 0852  jne     short L08B9 */
    if (!ZF) goto L08B9;
L0854:
    /* 0854  mov     bp,word ptr [si-4] */
    BP = rw(pDS, SI + 0xFFFC);
L0857:
    /* 0857  sub     bp,word ptr [si-6] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0xFFFA));
L085A:
    /* 085A  mov     cx,bp */
    CX = BP;
L085C:
    /* 085C  sub     cx,word ptr [si+4] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x4));
L085F:
    /* 085F  add     cx,word ptr [si+2] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0x2));
L0862:
    /* 0862  mov     al,byte ptr [si+7] */
    AL = rb(pDS, SI + 0x7);
L0865:
    /* 0865  sub     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL - rb(pDS, SI + 0xF));
L0868:
    /* 0868  cbw */
    AX = (uint16_t)(int8_t)AL;
L0869:
    /* 0869  imul    bp */
    imul16(BP);
L086B:
    /* 086B  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x086B, 2)) != 0) return c;
L086D:
    /* 086D  add     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL + rb(pDS, SI + 0xF));
L0870:
    /* 0870  mov     byte ptr es:[di+7],al */
    wb(pES, DI + 0x7, AL);
L0874:
    /* 0874  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L0876:
    /* 0876  sub     ax,word ptr [si-8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF8));
L0879:
    /* 0879  imul    bp */
    imul16(BP);
L087B:
    /* 087B  shl     ax,1 */
    AX = shl16(AX, 1);
L087D:
    /* 087D  rcl     dx,1 */
    DX = rcl16(DX, 1);
L087F:
    /* 087F  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x087F, 2)) != 0) return c;
L0881:
    /* 0881  sar     ax,1 */
    AX = sar16(AX, 1);
L0883:
    /* 0883  jae     short L0888 */
    if (!CF) goto L0888;
L0885:
    /* 0885  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0886:
    /* 0886  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L0888: /* L0888 */
    /* 0888  add     ax,word ptr [si-8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF8));
L088B:
    /* 088B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L088C:
    /* 088C  mov     bx,ax */
    BX = AX;
L088E:
    /* 088E  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L0891:
    /* 0891  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L0894:
    /* 0894  imul    bp */
    imul16(BP);
L0896:
    /* 0896  shl     ax,1 */
    AX = shl16(AX, 1);
L0898:
    /* 0898  rcl     dx,1 */
    DX = rcl16(DX, 1);
L089A:
    /* 089A  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x089A, 2)) != 0) return c;
L089C:
    /* 089C  sar     ax,1 */
    AX = sar16(AX, 1);
L089E:
    /* 089E  jae     short L08A3 */
    if (!CF) goto L08A3;
L08A0:
    /* 08A0  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L08A1:
    /* 08A1  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L08A3: /* L08A3 */
    /* 08A3  add     ax,word ptr [si-6] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFA));
L08A6:
    /* 08A6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L08A7:
    /* 08A7  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L08A8:
    /* 08A8  mov     cx,ax */
    CX = AX;
L08AA:
    /* 08AA  mov     bp,ax */
    BP = AX;
L08AC:
    /* 08AC  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x08AF)) != 0) return c;
L08AF:
    /* 08AF  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L08B0:
    /* 08B0  inc     di */
    DI = (uint16_t)(DI + 1);
L08B1:
    /* 08B1  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L08B5:
    /* 08B5  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L08B9: /* L08B9 */
    /* 08B9  test    byte ptr [si+0Eh],4 */
    logic8((uint8_t)(rb(pDS, SI + 0xE) & 0x4));
L08BD:
    /* 08BD  je      short L08C2 */
    if (ZF) goto L08C2;
L08BF:
    /* 08BF  jmp     L082C */
    goto L082C;
L08C2: /* L08C2 */
    /* 08C2  mov     bp,word ptr [si+4] */
    BP = rw(pDS, SI + 0x4);
L08C5:
    /* 08C5  sub     bp,word ptr [si+2] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0x2));
L08C8:
    /* 08C8  mov     cx,bp */
    CX = BP;
L08CA:
    /* 08CA  add     cx,word ptr [si+0Ah] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0xA));
L08CD:
    /* 08CD  sub     cx,word ptr [si+0Ch] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xC));
L08D0:
    /* 08D0  mov     al,byte ptr [si+7] */
    AL = rb(pDS, SI + 0x7);
L08D3:
    /* 08D3  sub     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL - rb(pDS, SI + 0xF));
L08D6:
    /* 08D6  cbw */
    AX = (uint16_t)(int8_t)AL;
L08D7:
    /* 08D7  imul    bp */
    imul16(BP);
L08D9:
    /* 08D9  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x08D9, 2)) != 0) return c;
L08DB:
    /* 08DB  add     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL + rb(pDS, SI + 0xF));
L08DE:
    /* 08DE  mov     byte ptr es:[di+7],al */
    wb(pES, DI + 0x7, AL);
L08E2:
    /* 08E2  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L08E5:
    /* 08E5  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L08E7:
    /* 08E7  imul    bp */
    imul16(BP);
L08E9:
    /* 08E9  shl     ax,1 */
    AX = shl16(AX, 1);
L08EB:
    /* 08EB  rcl     dx,1 */
    DX = rcl16(DX, 1);
L08ED:
    /* 08ED  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x08ED, 2)) != 0) return c;
L08EF:
    /* 08EF  sar     ax,1 */
    AX = sar16(AX, 1);
L08F1:
    /* 08F1  jae     short L08F6 */
    if (!CF) goto L08F6;
L08F3:
    /* 08F3  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L08F4:
    /* 08F4  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L08F6: /* L08F6 */
    /* 08F6  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L08F8:
    /* 08F8  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L08F9:
    /* 08F9  mov     bx,ax */
    BX = AX;
L08FB:
    /* 08FB  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L08FE:
    /* 08FE  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L0901:
    /* 0901  imul    bp */
    imul16(BP);
L0903:
    /* 0903  shl     ax,1 */
    AX = shl16(AX, 1);
L0905:
    /* 0905  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0907:
    /* 0907  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x0907, 2)) != 0) return c;
L0909:
    /* 0909  sar     ax,1 */
    AX = sar16(AX, 1);
L090B:
    /* 090B  jae     short L0910 */
    if (!CF) goto L0910;
L090D:
    /* 090D  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L090E:
    /* 090E  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L0910: /* L0910 */
    /* 0910  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L0913:
    /* 0913  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0914:
    /* 0914  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0915:
    /* 0915  mov     cx,ax */
    CX = AX;
L0917:
    /* 0917  mov     bp,ax */
    BP = AX;
L0919:
    /* 0919  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x091C)) != 0) return c;
L091C:
    /* 091C  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L091D:
    /* 091D  inc     di */
    DI = (uint16_t)(DI + 1);
L091E:
    /* 091E  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L0922:
    /* 0922  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L0926:
    /* 0926  jmp     L082C */
    goto L082C;
L0929: /* L0929 */
    /* 0929  mov     word ptr ds:[1B6h],di */
    ww(pDS, 0x1B6, DI);
L092D:
    /* 092D  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_92E  (+92E)
       Gouraud clip against y = -z (code bit 8), like clip_bot. */
L092E: /* _seg004_0849_92E */
    /* 092E  mov     byte ptr ds:[14CAh],0 */
    wb(pDS, 0x14CA, 0x0);
L0933:
    /* 0933  mov     byte ptr ds:[14CBh],0FFh */
    wb(pDS, 0x14CB, 0xFF);
L0938:
    /* 0938  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L093C:
    /* 093C  mov     di,word ptr ds:[1B6h] */
    DI = rw(pDS, 0x1B6);
L0940:
    /* 0940  mov     cx,8 */
    CX = 0x8;
L0943:
    /* 0943  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0945:
    /* 0945  add     word ptr ds:[1B6h],8 */
    ww(pDS, 0x1B6, (uint16_t)(rw(pDS, 0x1B6) + 0x8));
L094A:
    /* 094A  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L094E:
    /* 094E  mov     di,0B1Ah */
    DI = 0xB1A;
L0951:
    /* 0951  cmp     si,1BAh */
    sub16(SI, 0x1BA, 0);
L0955:
    /* 0955  je      short L095A */
    if (ZF) goto L095A;
L0957:
    /* 0957  mov     di,1BAh */
    DI = 0x1BA;
L095A: /* L095A */
    /* 095A  mov     word ptr ds:[1B8h],di */
    ww(pDS, 0x1B8, DI);
L095E: /* L095E */
    /* 095E  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L0961: /* L0961 */
    /* 0961  cmp     si,word ptr ds:[1B6h] */
    sub16(SI, rw(pDS, 0x1B6), 0);
L0965:
    /* 0965  jne     short L096A */
    if (!ZF) goto L096A;
L0967:
    /* 0967  jmp     L0A5F */
    goto L0A5F;
L096A: /* L096A */
    /* 096A  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L096D:
    /* 096D  test    al,8 */
    logic8((uint8_t)(AL & 0x8));
L096F:
    /* 096F  jne     short L0980 */
    if (!ZF) goto L0980;
L0971:
    /* 0971  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L0975:
    /* 0975  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L0979:
    /* 0979  mov     cx,4 */
    CX = 0x4;
L097C:
    /* 097C  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L097E:
    /* 097E  jmp     L0961 */
    goto L0961;
L0980: /* L0980 */
    /* 0980  test    byte ptr [si-2],8 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFE) & 0x8));
L0984:
    /* 0984  jne     short L09ED */
    if (!ZF) goto L09ED;
L0986:
    /* 0986  mov     bp,word ptr [si-4] */
    BP = rw(pDS, SI + 0xFFFC);
L0989:
    /* 0989  add     bp,word ptr [si-6] */
    BP = (uint16_t)(BP + rw(pDS, SI + 0xFFFA));
L098C:
    /* 098C  mov     cx,bp */
    CX = BP;
L098E:
    /* 098E  sub     cx,word ptr [si+4] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x4));
L0991:
    /* 0991  sub     cx,word ptr [si+2] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x2));
L0994:
    /* 0994  mov     al,byte ptr [si+7] */
    AL = rb(pDS, SI + 0x7);
L0997:
    /* 0997  sub     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL - rb(pDS, SI + 0xF));
L099A:
    /* 099A  cbw */
    AX = (uint16_t)(int8_t)AL;
L099B:
    /* 099B  imul    bp */
    imul16(BP);
L099D:
    /* 099D  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x099D, 2)) != 0) return c;
L099F:
    /* 099F  add     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL + rb(pDS, SI + 0xF));
L09A2:
    /* 09A2  mov     byte ptr es:[di+7],al */
    wb(pES, DI + 0x7, AL);
L09A6:
    /* 09A6  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L09A8:
    /* 09A8  sub     ax,word ptr [si-8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF8));
L09AB:
    /* 09AB  imul    bp */
    imul16(BP);
L09AD:
    /* 09AD  shl     ax,1 */
    AX = shl16(AX, 1);
L09AF:
    /* 09AF  rcl     dx,1 */
    DX = rcl16(DX, 1);
L09B1:
    /* 09B1  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x09B1, 2)) != 0) return c;
L09B3:
    /* 09B3  sar     ax,1 */
    AX = sar16(AX, 1);
L09B5:
    /* 09B5  jae     short L09BA */
    if (!CF) goto L09BA;
L09B7:
    /* 09B7  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L09B8:
    /* 09B8  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L09BA: /* L09BA */
    /* 09BA  add     ax,word ptr [si-8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF8));
L09BD:
    /* 09BD  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L09BE:
    /* 09BE  mov     bx,ax */
    BX = AX;
L09C0:
    /* 09C0  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L09C3:
    /* 09C3  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L09C6:
    /* 09C6  imul    bp */
    imul16(BP);
L09C8:
    /* 09C8  shl     ax,1 */
    AX = shl16(AX, 1);
L09CA:
    /* 09CA  rcl     dx,1 */
    DX = rcl16(DX, 1);
L09CC:
    /* 09CC  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x09CC, 2)) != 0) return c;
L09CE:
    /* 09CE  sar     ax,1 */
    AX = sar16(AX, 1);
L09D0:
    /* 09D0  jae     short L09D5 */
    if (!CF) goto L09D5;
L09D2:
    /* 09D2  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L09D3:
    /* 09D3  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L09D5: /* L09D5 */
    /* 09D5  add     ax,word ptr [si-6] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFA));
L09D8:
    /* 09D8  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L09D9:
    /* 09D9  mov     cx,ax */
    CX = AX;
L09DB:
    /* 09DB  neg     ax */
    AX = (uint16_t)-AX;
L09DD:
    /* 09DD  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L09DE:
    /* 09DE  mov     bp,ax */
    BP = AX;
L09E0:
    /* 09E0  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x09E3)) != 0) return c;
L09E3:
    /* 09E3  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L09E4:
    /* 09E4  inc     di */
    DI = (uint16_t)(DI + 1);
L09E5:
    /* 09E5  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L09E9:
    /* 09E9  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L09ED: /* L09ED */
    /* 09ED  test    byte ptr [si+0Eh],8 */
    logic8((uint8_t)(rb(pDS, SI + 0xE) & 0x8));
L09F1:
    /* 09F1  je      short L09F6 */
    if (ZF) goto L09F6;
L09F3:
    /* 09F3  jmp     L095E */
    goto L095E;
L09F6: /* L09F6 */
    /* 09F6  mov     bp,word ptr [si+4] */
    BP = rw(pDS, SI + 0x4);
L09F9:
    /* 09F9  add     bp,word ptr [si+2] */
    BP = (uint16_t)(BP + rw(pDS, SI + 0x2));
L09FC:
    /* 09FC  mov     cx,bp */
    CX = BP;
L09FE:
    /* 09FE  sub     cx,word ptr [si+0Ah] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xA));
L0A01:
    /* 0A01  sub     cx,word ptr [si+0Ch] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xC));
L0A04:
    /* 0A04  mov     al,byte ptr [si+7] */
    AL = rb(pDS, SI + 0x7);
L0A07:
    /* 0A07  sub     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL - rb(pDS, SI + 0xF));
L0A0A:
    /* 0A0A  cbw */
    AX = (uint16_t)(int8_t)AL;
L0A0B:
    /* 0A0B  imul    bp */
    imul16(BP);
L0A0D:
    /* 0A0D  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x0A0D, 2)) != 0) return c;
L0A0F:
    /* 0A0F  add     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL + rb(pDS, SI + 0xF));
L0A12:
    /* 0A12  mov     byte ptr es:[di+7],al */
    wb(pES, DI + 0x7, AL);
L0A16:
    /* 0A16  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L0A19:
    /* 0A19  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L0A1B:
    /* 0A1B  imul    bp */
    imul16(BP);
L0A1D:
    /* 0A1D  shl     ax,1 */
    AX = shl16(AX, 1);
L0A1F:
    /* 0A1F  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0A21:
    /* 0A21  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x0A21, 2)) != 0) return c;
L0A23:
    /* 0A23  sar     ax,1 */
    AX = sar16(AX, 1);
L0A25:
    /* 0A25  jae     short L0A2A */
    if (!CF) goto L0A2A;
L0A27:
    /* 0A27  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0A28:
    /* 0A28  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L0A2A: /* L0A2A */
    /* 0A2A  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L0A2C:
    /* 0A2C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0A2D:
    /* 0A2D  mov     bx,ax */
    BX = AX;
L0A2F:
    /* 0A2F  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L0A32:
    /* 0A32  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L0A35:
    /* 0A35  imul    bp */
    imul16(BP);
L0A37:
    /* 0A37  shl     ax,1 */
    AX = shl16(AX, 1);
L0A39:
    /* 0A39  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0A3B:
    /* 0A3B  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x0A3B, 2)) != 0) return c;
L0A3D:
    /* 0A3D  sar     ax,1 */
    AX = sar16(AX, 1);
L0A3F:
    /* 0A3F  jae     short L0A44 */
    if (!CF) goto L0A44;
L0A41:
    /* 0A41  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0A42:
    /* 0A42  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L0A44: /* L0A44 */
    /* 0A44  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L0A47:
    /* 0A47  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0A48:
    /* 0A48  mov     cx,ax */
    CX = AX;
L0A4A:
    /* 0A4A  neg     ax */
    AX = (uint16_t)-AX;
L0A4C:
    /* 0A4C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0A4D:
    /* 0A4D  mov     bp,ax */
    BP = AX;
L0A4F:
    /* 0A4F  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x0A52)) != 0) return c;
L0A52:
    /* 0A52  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0A53:
    /* 0A53  inc     di */
    DI = (uint16_t)(DI + 1);
L0A54:
    /* 0A54  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L0A58:
    /* 0A58  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L0A5C:
    /* 0A5C  jmp     L095E */
    goto L095E;
L0A5F: /* L0A5F */
    /* 0A5F  mov     word ptr ds:[1B6h],di */
    ww(pDS, 0x1B6, DI);
L0A63:
    /* 0A63  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_A64  (+A64)
       Gouraud clip against x = z (code bit 2), like clip_right. */
L0A64: /* _seg004_0849_A64 */
    /* 0A64  mov     byte ptr ds:[14CAh],0 */
    wb(pDS, 0x14CA, 0x0);
L0A69:
    /* 0A69  mov     byte ptr ds:[14CBh],0FFh */
    wb(pDS, 0x14CB, 0xFF);
L0A6E:
    /* 0A6E  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L0A72:
    /* 0A72  mov     di,word ptr ds:[1B6h] */
    DI = rw(pDS, 0x1B6);
L0A76:
    /* 0A76  mov     cx,8 */
    CX = 0x8;
L0A79:
    /* 0A79  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0A7B:
    /* 0A7B  add     word ptr ds:[1B6h],8 */
    ww(pDS, 0x1B6, (uint16_t)(rw(pDS, 0x1B6) + 0x8));
L0A80:
    /* 0A80  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L0A84:
    /* 0A84  mov     di,0B1Ah */
    DI = 0xB1A;
L0A87:
    /* 0A87  cmp     si,1BAh */
    sub16(SI, 0x1BA, 0);
L0A8B:
    /* 0A8B  je      short L0A90 */
    if (ZF) goto L0A90;
L0A8D:
    /* 0A8D  mov     di,1BAh */
    DI = 0x1BA;
L0A90: /* L0A90 */
    /* 0A90  mov     word ptr ds:[1B8h],di */
    ww(pDS, 0x1B8, DI);
L0A94: /* L0A94 */
    /* 0A94  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L0A97: /* L0A97 */
    /* 0A97  cmp     si,word ptr ds:[1B6h] */
    sub16(SI, rw(pDS, 0x1B6), 0);
L0A9B:
    /* 0A9B  jne     short L0AA0 */
    if (!ZF) goto L0AA0;
L0A9D:
    /* 0A9D  jmp     L0B93 */
    goto L0B93;
L0AA0: /* L0AA0 */
    /* 0AA0  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L0AA3:
    /* 0AA3  test    al,2 */
    logic8((uint8_t)(AL & 0x2));
L0AA5:
    /* 0AA5  jne     short L0AB6 */
    if (!ZF) goto L0AB6;
L0AA7:
    /* 0AA7  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L0AAB:
    /* 0AAB  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L0AAF:
    /* 0AAF  mov     cx,4 */
    CX = 0x4;
L0AB2:
    /* 0AB2  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0AB4:
    /* 0AB4  jmp     L0A97 */
    goto L0A97;
L0AB6: /* L0AB6 */
    /* 0AB6  test    byte ptr [si-2],2 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFE) & 0x2));
L0ABA:
    /* 0ABA  jne     short L0B22 */
    if (!ZF) goto L0B22;
L0ABC:
    /* 0ABC  mov     bp,word ptr [si-8] */
    BP = rw(pDS, SI + 0xFFF8);
L0ABF:
    /* 0ABF  sub     bp,word ptr [si-4] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0xFFFC));
L0AC2:
    /* 0AC2  mov     cx,bp */
    CX = BP;
L0AC4:
    /* 0AC4  add     cx,word ptr [si+4] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0x4));
L0AC7:
    /* 0AC7  sub     cx,word ptr [si] */
    CX = (uint16_t)(CX - rw(pDS, SI));
L0AC9:
    /* 0AC9  mov     al,byte ptr [si+7] */
    AL = rb(pDS, SI + 0x7);
L0ACC:
    /* 0ACC  sub     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL - rb(pDS, SI + 0xF));
L0ACF:
    /* 0ACF  cbw */
    AX = (uint16_t)(int8_t)AL;
L0AD0:
    /* 0AD0  imul    bp */
    imul16(BP);
L0AD2:
    /* 0AD2  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x0AD2, 2)) != 0) return c;
L0AD4:
    /* 0AD4  add     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL + rb(pDS, SI + 0xF));
L0AD7:
    /* 0AD7  mov     byte ptr es:[di+7],al */
    wb(pES, DI + 0x7, AL);
L0ADB:
    /* 0ADB  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L0ADD:
    /* 0ADD  sub     ax,word ptr [si-8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF8));
L0AE0:
    /* 0AE0  imul    bp */
    imul16(BP);
L0AE2:
    /* 0AE2  shl     ax,1 */
    AX = shl16(AX, 1);
L0AE4:
    /* 0AE4  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0AE6:
    /* 0AE6  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x0AE6, 2)) != 0) return c;
L0AE8:
    /* 0AE8  sar     ax,1 */
    AX = sar16(AX, 1);
L0AEA:
    /* 0AEA  jae     short L0AEF */
    if (!CF) goto L0AEF;
L0AEC:
    /* 0AEC  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0AED:
    /* 0AED  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L0AEF: /* L0AEF */
    /* 0AEF  add     ax,word ptr [si-8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF8));
L0AF2:
    /* 0AF2  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0AF3:
    /* 0AF3  mov     bx,ax */
    BX = AX;
L0AF5:
    /* 0AF5  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L0AF8:
    /* 0AF8  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L0AFB:
    /* 0AFB  imul    bp */
    imul16(BP);
L0AFD:
    /* 0AFD  shl     ax,1 */
    AX = shl16(AX, 1);
L0AFF:
    /* 0AFF  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0B01:
    /* 0B01  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x0B01, 2)) != 0) return c;
L0B03:
    /* 0B03  sar     ax,1 */
    AX = sar16(AX, 1);
L0B05:
    /* 0B05  jae     short L0B0A */
    if (!CF) goto L0B0A;
L0B07:
    /* 0B07  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0B08:
    /* 0B08  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L0B0A: /* L0B0A */
    /* 0B0A  add     ax,word ptr [si-6] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFA));
L0B0D:
    /* 0B0D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0B0E:
    /* 0B0E  mov     cx,ax */
    CX = AX;
L0B10:
    /* 0B10  mov     ax,bx */
    AX = BX;
L0B12:
    /* 0B12  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0B13:
    /* 0B13  mov     bp,ax */
    BP = AX;
L0B15:
    /* 0B15  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x0B18)) != 0) return c;
L0B18:
    /* 0B18  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0B19:
    /* 0B19  inc     di */
    DI = (uint16_t)(DI + 1);
L0B1A:
    /* 0B1A  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L0B1E:
    /* 0B1E  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L0B22: /* L0B22 */
    /* 0B22  test    byte ptr [si+0Eh],2 */
    logic8((uint8_t)(rb(pDS, SI + 0xE) & 0x2));
L0B26:
    /* 0B26  je      short L0B2B */
    if (ZF) goto L0B2B;
L0B28:
    /* 0B28  jmp     L0A94 */
    goto L0A94;
L0B2B: /* L0B2B */
    /* 0B2B  mov     bp,word ptr [si] */
    BP = rw(pDS, SI);
L0B2D:
    /* 0B2D  sub     bp,word ptr [si+4] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0x4));
L0B30:
    /* 0B30  mov     cx,bp */
    CX = BP;
L0B32:
    /* 0B32  add     cx,word ptr [si+0Ch] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0xC));
L0B35:
    /* 0B35  sub     cx,word ptr [si+8] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x8));
L0B38:
    /* 0B38  mov     al,byte ptr [si+7] */
    AL = rb(pDS, SI + 0x7);
L0B3B:
    /* 0B3B  sub     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL - rb(pDS, SI + 0xF));
L0B3E:
    /* 0B3E  cbw */
    AX = (uint16_t)(int8_t)AL;
L0B3F:
    /* 0B3F  imul    bp */
    imul16(BP);
L0B41:
    /* 0B41  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x0B41, 2)) != 0) return c;
L0B43:
    /* 0B43  add     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL + rb(pDS, SI + 0xF));
L0B46:
    /* 0B46  mov     byte ptr es:[di+7],al */
    wb(pES, DI + 0x7, AL);
L0B4A:
    /* 0B4A  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L0B4D:
    /* 0B4D  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L0B4F:
    /* 0B4F  imul    bp */
    imul16(BP);
L0B51:
    /* 0B51  shl     ax,1 */
    AX = shl16(AX, 1);
L0B53:
    /* 0B53  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0B55:
    /* 0B55  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x0B55, 2)) != 0) return c;
L0B57:
    /* 0B57  sar     ax,1 */
    AX = sar16(AX, 1);
L0B59:
    /* 0B59  jae     short L0B5E */
    if (!CF) goto L0B5E;
L0B5B:
    /* 0B5B  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0B5C:
    /* 0B5C  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L0B5E: /* L0B5E */
    /* 0B5E  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L0B60:
    /* 0B60  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0B61:
    /* 0B61  mov     bx,ax */
    BX = AX;
L0B63:
    /* 0B63  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L0B66:
    /* 0B66  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L0B69:
    /* 0B69  imul    bp */
    imul16(BP);
L0B6B:
    /* 0B6B  shl     ax,1 */
    AX = shl16(AX, 1);
L0B6D:
    /* 0B6D  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0B6F:
    /* 0B6F  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x0B6F, 2)) != 0) return c;
L0B71:
    /* 0B71  sar     ax,1 */
    AX = sar16(AX, 1);
L0B73:
    /* 0B73  jae     short L0B78 */
    if (!CF) goto L0B78;
L0B75:
    /* 0B75  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0B76:
    /* 0B76  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L0B78: /* L0B78 */
    /* 0B78  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L0B7B:
    /* 0B7B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0B7C:
    /* 0B7C  mov     cx,ax */
    CX = AX;
L0B7E:
    /* 0B7E  mov     ax,bx */
    AX = BX;
L0B80:
    /* 0B80  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0B81:
    /* 0B81  mov     bp,ax */
    BP = AX;
L0B83:
    /* 0B83  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x0B86)) != 0) return c;
L0B86:
    /* 0B86  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0B87:
    /* 0B87  inc     di */
    DI = (uint16_t)(DI + 1);
L0B88:
    /* 0B88  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L0B8C:
    /* 0B8C  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L0B90:
    /* 0B90  jmp     L0A94 */
    goto L0A94;
L0B93: /* L0B93 */
    /* 0B93  mov     word ptr ds:[1B6h],di */
    ww(pDS, 0x1B6, DI);
L0B97:
    /* 0B97  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_B98  (+B98)
       Gouraud clip against x = -z (code bit 1), like clip_left. */
L0B98: /* _seg004_0849_B98 */
    /* 0B98  mov     byte ptr ds:[14CAh],0 */
    wb(pDS, 0x14CA, 0x0);
L0B9D:
    /* 0B9D  mov     byte ptr ds:[14CBh],0FFh */
    wb(pDS, 0x14CB, 0xFF);
L0BA2:
    /* 0BA2  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L0BA6:
    /* 0BA6  mov     di,word ptr ds:[1B6h] */
    DI = rw(pDS, 0x1B6);
L0BAA:
    /* 0BAA  mov     cx,8 */
    CX = 0x8;
L0BAD:
    /* 0BAD  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0BAF:
    /* 0BAF  add     word ptr ds:[1B6h],8 */
    ww(pDS, 0x1B6, (uint16_t)(rw(pDS, 0x1B6) + 0x8));
L0BB4:
    /* 0BB4  mov     si,word ptr ds:[1B8h] */
    SI = rw(pDS, 0x1B8);
L0BB8:
    /* 0BB8  mov     di,0B1Ah */
    DI = 0xB1A;
L0BBB:
    /* 0BBB  cmp     si,1BAh */
    sub16(SI, 0x1BA, 0);
L0BBF:
    /* 0BBF  je      short L0BC4 */
    if (ZF) goto L0BC4;
L0BC1:
    /* 0BC1  mov     di,1BAh */
    DI = 0x1BA;
L0BC4: /* L0BC4 */
    /* 0BC4  mov     word ptr ds:[1B8h],di */
    ww(pDS, 0x1B8, DI);
L0BC8: /* L0BC8 */
    /* 0BC8  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L0BCB: /* L0BCB */
    /* 0BCB  cmp     si,word ptr ds:[1B6h] */
    sub16(SI, rw(pDS, 0x1B6), 0);
L0BCF:
    /* 0BCF  jne     short L0BD4 */
    if (!ZF) goto L0BD4;
L0BD1:
    /* 0BD1  jmp     L0CCB */
    goto L0CCB;
L0BD4: /* L0BD4 */
    /* 0BD4  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L0BD7:
    /* 0BD7  test    al,1 */
    logic8((uint8_t)(AL & 0x1));
L0BD9:
    /* 0BD9  jne     short L0BEA */
    if (!ZF) goto L0BEA;
L0BDB:
    /* 0BDB  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L0BDF:
    /* 0BDF  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L0BE3:
    /* 0BE3  mov     cx,4 */
    CX = 0x4;
L0BE6:
    /* 0BE6  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0BE8:
    /* 0BE8  jmp     L0BCB */
    goto L0BCB;
L0BEA: /* L0BEA */
    /* 0BEA  test    byte ptr [si-2],1 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFE) & 0x1));
L0BEE:
    /* 0BEE  jne     short L0C58 */
    if (!ZF) goto L0C58;
L0BF0:
    /* 0BF0  mov     bp,word ptr [si-4] */
    BP = rw(pDS, SI + 0xFFFC);
L0BF3:
    /* 0BF3  add     bp,word ptr [si-8] */
    BP = (uint16_t)(BP + rw(pDS, SI + 0xFFF8));
L0BF6:
    /* 0BF6  mov     cx,bp */
    CX = BP;
L0BF8:
    /* 0BF8  sub     cx,word ptr [si+4] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x4));
L0BFB:
    /* 0BFB  sub     cx,word ptr [si] */
    CX = (uint16_t)(CX - rw(pDS, SI));
L0BFD:
    /* 0BFD  mov     al,byte ptr [si+7] */
    AL = rb(pDS, SI + 0x7);
L0C00:
    /* 0C00  sub     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL - rb(pDS, SI + 0xF));
L0C03:
    /* 0C03  cbw */
    AX = (uint16_t)(int8_t)AL;
L0C04:
    /* 0C04  imul    bp */
    imul16(BP);
L0C06:
    /* 0C06  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x0C06, 2)) != 0) return c;
L0C08:
    /* 0C08  add     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL + rb(pDS, SI + 0xF));
L0C0B:
    /* 0C0B  mov     byte ptr es:[di+7],al */
    wb(pES, DI + 0x7, AL);
L0C0F:
    /* 0C0F  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L0C11:
    /* 0C11  sub     ax,word ptr [si-8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF8));
L0C14:
    /* 0C14  imul    bp */
    imul16(BP);
L0C16:
    /* 0C16  shl     ax,1 */
    AX = shl16(AX, 1);
L0C18:
    /* 0C18  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0C1A:
    /* 0C1A  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x0C1A, 2)) != 0) return c;
L0C1C:
    /* 0C1C  sar     ax,1 */
    AX = sar16(AX, 1);
L0C1E:
    /* 0C1E  jae     short L0C23 */
    if (!CF) goto L0C23;
L0C20:
    /* 0C20  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0C21:
    /* 0C21  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L0C23: /* L0C23 */
    /* 0C23  add     ax,word ptr [si-8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF8));
L0C26:
    /* 0C26  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0C27:
    /* 0C27  mov     bx,ax */
    BX = AX;
L0C29:
    /* 0C29  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L0C2C:
    /* 0C2C  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L0C2F:
    /* 0C2F  imul    bp */
    imul16(BP);
L0C31:
    /* 0C31  shl     ax,1 */
    AX = shl16(AX, 1);
L0C33:
    /* 0C33  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0C35:
    /* 0C35  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x0C35, 2)) != 0) return c;
L0C37:
    /* 0C37  sar     ax,1 */
    AX = sar16(AX, 1);
L0C39:
    /* 0C39  jae     short L0C3E */
    if (!CF) goto L0C3E;
L0C3B:
    /* 0C3B  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0C3C:
    /* 0C3C  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L0C3E: /* L0C3E */
    /* 0C3E  add     ax,word ptr [si-6] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFA));
L0C41:
    /* 0C41  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0C42:
    /* 0C42  mov     cx,ax */
    CX = AX;
L0C44:
    /* 0C44  mov     ax,bx */
    AX = BX;
L0C46:
    /* 0C46  neg     ax */
    AX = (uint16_t)-AX;
L0C48:
    /* 0C48  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0C49:
    /* 0C49  mov     bp,ax */
    BP = AX;
L0C4B:
    /* 0C4B  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x0C4E)) != 0) return c;
L0C4E:
    /* 0C4E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0C4F:
    /* 0C4F  inc     di */
    DI = (uint16_t)(DI + 1);
L0C50:
    /* 0C50  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L0C54:
    /* 0C54  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L0C58: /* L0C58 */
    /* 0C58  test    byte ptr [si+0Eh],1 */
    logic8((uint8_t)(rb(pDS, SI + 0xE) & 0x1));
L0C5C:
    /* 0C5C  je      short L0C61 */
    if (ZF) goto L0C61;
L0C5E:
    /* 0C5E  jmp     L0BC8 */
    goto L0BC8;
L0C61: /* L0C61 */
    /* 0C61  mov     bp,word ptr [si+4] */
    BP = rw(pDS, SI + 0x4);
L0C64:
    /* 0C64  add     bp,word ptr [si] */
    BP = (uint16_t)(BP + rw(pDS, SI));
L0C66:
    /* 0C66  mov     cx,bp */
    CX = BP;
L0C68:
    /* 0C68  sub     cx,word ptr [si+8] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x8));
L0C6B:
    /* 0C6B  sub     cx,word ptr [si+0Ch] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xC));
L0C6E:
    /* 0C6E  mov     al,byte ptr [si+7] */
    AL = rb(pDS, SI + 0x7);
L0C71:
    /* 0C71  sub     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL - rb(pDS, SI + 0xF));
L0C74:
    /* 0C74  cbw */
    AX = (uint16_t)(int8_t)AL;
L0C75:
    /* 0C75  imul    bp */
    imul16(BP);
L0C77:
    /* 0C77  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x0C77, 2)) != 0) return c;
L0C79:
    /* 0C79  add     al,byte ptr [si+0Fh] */
    AL = (uint8_t)(AL + rb(pDS, SI + 0xF));
L0C7C:
    /* 0C7C  mov     byte ptr es:[di+7],al */
    wb(pES, DI + 0x7, AL);
L0C80:
    /* 0C80  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L0C83:
    /* 0C83  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L0C85:
    /* 0C85  imul    bp */
    imul16(BP);
L0C87:
    /* 0C87  shl     ax,1 */
    AX = shl16(AX, 1);
L0C89:
    /* 0C89  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0C8B:
    /* 0C8B  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x0C8B, 2)) != 0) return c;
L0C8D:
    /* 0C8D  sar     ax,1 */
    AX = sar16(AX, 1);
L0C8F:
    /* 0C8F  jae     short L0C94 */
    if (!CF) goto L0C94;
L0C91:
    /* 0C91  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0C92:
    /* 0C92  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L0C94: /* L0C94 */
    /* 0C94  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L0C96:
    /* 0C96  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0C97:
    /* 0C97  mov     bx,ax */
    BX = AX;
L0C99:
    /* 0C99  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L0C9C:
    /* 0C9C  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L0C9F:
    /* 0C9F  imul    bp */
    imul16(BP);
L0CA1:
    /* 0CA1  shl     ax,1 */
    AX = shl16(AX, 1);
L0CA3:
    /* 0CA3  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0CA5:
    /* 0CA5  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x0CA5, 2)) != 0) return c;
L0CA7:
    /* 0CA7  sar     ax,1 */
    AX = sar16(AX, 1);
L0CA9:
    /* 0CA9  jae     short L0CAE */
    if (!CF) goto L0CAE;
L0CAB:
    /* 0CAB  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0CAC:
    /* 0CAC  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L0CAE: /* L0CAE */
    /* 0CAE  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L0CB1:
    /* 0CB1  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0CB2:
    /* 0CB2  mov     cx,ax */
    CX = AX;
L0CB4:
    /* 0CB4  mov     ax,bx */
    AX = BX;
L0CB6:
    /* 0CB6  neg     ax */
    AX = (uint16_t)-AX;
L0CB8:
    /* 0CB8  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0CB9:
    /* 0CB9  mov     bp,ax */
    BP = AX;
L0CBB:
    /* 0CBB  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x065C, 0x3A88), 0x0CBE)) != 0) return c;
L0CBE:
    /* 0CBE  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0CBF:
    /* 0CBF  inc     di */
    DI = (uint16_t)(DI + 1);
L0CC0:
    /* 0CC0  or      byte ptr ds:[14CAh],al */
    wb(pDS, 0x14CA, (uint8_t)(rb(pDS, 0x14CA) | AL));
L0CC4:
    /* 0CC4  and     byte ptr ds:[14CBh],al */
    wb(pDS, 0x14CB, (uint8_t)(rb(pDS, 0x14CB) & AL));
L0CC8:
    /* 0CC8  jmp     L0BC8 */
    goto L0BC8;
L0CCB: /* L0CCB */
    /* 0CCB  mov     word ptr ds:[1B6h],di */
    ww(pDS, 0x1B6, DI);
L0CCF:
    /* 0CCF  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* smooth_over  (+CD0): FM smooth_over, the same loop (copy the projected x and y, then
       an intensity from the vertex normal's length)
       In: CX = the vertex count. For each 14-byte record at seg_370D:658h it copies the screen x
       and y to 415Eh, takes the three words at +8 (each >> 5), and computes the length of that
       vector by four Newton steps from 64; the shade is the low bytes of the length and [64Ch]
       multiplied, >> 6, plus [650h] (at least 0) plus [64Eh], at most 15. Then it points seg003's handler pair at 0BC3h/0BC5h
       at 0BF2h/0CD9h (as do_transsurf does) and calls the polygon routine with BP =
       _seg003_0272_5959 and CX = ds:[0C7FCh] of seg052_519C. Nothing in the EXE calls it. */
L0CD0: /* _smooth_over */
    /* 0CD0  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L0CD3:
    /* 0CD3  mov     es,ax */
    SET_ES(AX);
L0CD5:
    /* 0CD5  mov     ds,ax */
    SET_DS(AX);
L0CD7:
    /* 0CD7  mov     di,415Eh */
    DI = 0x415E;
L0CDA:
    /* 0CDA  mov     si,658h */
    SI = 0x658;
L0CDD:
    /* 0CDD  mov     bp,cx */
    BP = CX;
L0CDF: /* L0CDF */
    /* 0CDF  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L0CE0:
    /* 0CE0  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L0CE1:
    /* 0CE1  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L0CE4:
    /* 0CE4  xor     cx,cx */
    CX = (uint16_t)(CX ^ CX);
L0CE6:
    /* 0CE6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0CE7:
    /* 0CE7  sar     ax,5 */
    AX = (uint16_t)((int16_t)AX >> 5);
L0CEA:
    /* 0CEA  mov     dx,ax */
    DX = AX;
L0CEC:
    /* 0CEC  imul    dx */
    imul16(DX);
L0CEE:
    /* 0CEE  add     cx,ax */
    CX = (uint16_t)(CX + AX);
L0CF0:
    /* 0CF0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0CF1:
    /* 0CF1  sar     ax,5 */
    AX = (uint16_t)((int16_t)AX >> 5);
L0CF4:
    /* 0CF4  mov     dx,ax */
    DX = AX;
L0CF6:
    /* 0CF6  imul    dx */
    imul16(DX);
L0CF8:
    /* 0CF8  add     cx,ax */
    CX = (uint16_t)(CX + AX);
L0CFA:
    /* 0CFA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0CFB:
    /* 0CFB  sar     ax,5 */
    AX = (uint16_t)((int16_t)AX >> 5);
L0CFE:
    /* 0CFE  mov     dx,ax */
    DX = AX;
L0D00:
    /* 0D00  imul    dx */
    imul16(DX);
L0D02:
    /* 0D02  add     cx,ax */
    CX = (uint16_t)(CX + AX);
L0D04:
    /* 0D04  mov     bx,cx */
    BX = CX;
L0D06:
    /* 0D06  mov     cx,40h */
    CX = 0x40;
L0D09:
    /* 0D09  mov     ax,bx */
    AX = BX;
L0D0B:
    /* 0D0B  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L0D0D:
    /* 0D0D  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x065C, 0x0D0D, 2)) != 0) return c;
L0D0F:
    /* 0D0F  add     cx,ax */
    CX = add16(CX, AX, 0);
L0D11:
    /* 0D11  rcr     cx,1 */
    CX = rcr16(CX, 1);
L0D13:
    /* 0D13  mov     ax,bx */
    AX = BX;
L0D15:
    /* 0D15  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L0D17:
    /* 0D17  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x065C, 0x0D17, 2)) != 0) return c;
L0D19:
    /* 0D19  add     cx,ax */
    CX = add16(CX, AX, 0);
L0D1B:
    /* 0D1B  rcr     cx,1 */
    CX = rcr16(CX, 1);
L0D1D:
    /* 0D1D  mov     ax,bx */
    AX = BX;
L0D1F:
    /* 0D1F  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L0D21:
    /* 0D21  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x065C, 0x0D21, 2)) != 0) return c;
L0D23:
    /* 0D23  add     cx,ax */
    CX = add16(CX, AX, 0);
L0D25:
    /* 0D25  rcr     cx,1 */
    CX = rcr16(CX, 1);
L0D27:
    /* 0D27  mov     ax,bx */
    AX = BX;
L0D29:
    /* 0D29  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L0D2B:
    /* 0D2B  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x065C, 0x0D2B, 2)) != 0) return c;
L0D2D:
    /* 0D2D  add     cx,ax */
    CX = add16(CX, AX, 0);
L0D2F:
    /* 0D2F  rcr     cx,1 */
    CX = rcr16(CX, 1);
L0D31:
    /* 0D31  mov     ax,cx */
    AX = CX;
L0D33:
    /* 0D33  mov     cx,word ptr ds:[64Ch] */
    CX = rw(pDS, 0x64C);
L0D37:
    /* 0D37  mul     cl */
    mul8(CL);
L0D39:
    /* 0D39  shr     ax,6 */
    AX = (uint16_t)(AX >> 6);
L0D3C:
    /* 0D3C  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L0D3E:
    /* 0D3E  add     ax,word ptr ds:[650h] */
    AX = add16(AX, rw(pDS, 0x650), 0);
L0D42:
    /* 0D42  jns     short L0D46 */
    if (!SF) goto L0D46;
L0D44:
    /* 0D44  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L0D46: /* L0D46 */
    /* 0D46  add     ax,word ptr ds:[64Eh] */
    AX = (uint16_t)(AX + rw(pDS, 0x64E));
L0D4A:
    /* 0D4A  cmp     al,0Fh */
    sub8(AL, 0xF, 0);
L0D4C:
    /* 0D4C  jbe     short L0D50 */
    if (CF || ZF) goto L0D50;
L0D4E:
    /* 0D4E  mov     al,0Fh */
    AL = 0xF;
L0D50: /* L0D50 */
    /* 0D50  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0D51:
    /* 0D51  dec     bp */
    BP = dec16(BP);
L0D52:
    /* 0D52  jne     L0CDF */
    if (!ZF) goto L0CDF;
L0D54:
    /* 0D54  mov     bp,offset _seg003_0272_5959 */
    BP = 0x5959;
L0D57:
    /* 0D57  mov     bx,es */
    BX = asm_es;
L0D59:
    /* 0D59  mov     ax,seg seg003_0272 */
    AX = (uint16_t)(0x0085 + PORT_LOAD_SEG);
L0D5C:
    /* 0D5C  mov     es,ax */
    SET_ES(AX);
L0D5E:
    /* 0D5E  mov     word ptr es:[0BC3h],0BF2h */
    ww(pES, 0xBC3, 0xBF2);
L0D65:
    /* 0D65  mov     word ptr es:[0BC5h],0CD9h */
    ww(pES, 0xBC5, 0xCD9);
L0D6C:
    /* 0D6C  mov     es,bx */
    SET_ES(BX);
L0D6E:
    /* 0D6E  mov     bx,seg seg052_519C */
    BX = (uint16_t)(0x4FAF + PORT_LOAD_SEG);
L0D71:
    /* 0D71  mov     es,bx */
    SET_ES(BX);
L0D73:
    /* 0D73  mov     ds,bx */
    SET_DS(BX);
L0D75:
    /* 0D75  mov     cx,word ptr ds:[0C7FCh] */
    CX = rw(pDS, 0xC7FC);
L0D79:
    /* 0D79  call    far ptr _seg003_0272_5311 */
    if ((c = asm_callf(ASM_JMP(0x0085, 0x5311), 0x065C + PORT_LOAD_SEG, 0x0D7E)) != 0) return c;
L0D7E:
    /* 0D7E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_D83  (+D83): divide overflow handler, installed by the smooth shading code
       (stored at ss:[5A5h] while clipping). Skips the 2-byte idiv, then retries the divide with
       the dividend halved (sar dx; rcr ax), or returns 7FFFh when the halved high word equals CX
       (a partial overflow check). */
L0D83: /* _seg004_0849_D83 */
    /* 0D83  mov     word ptr cs:L0D7F,bp */
    ww(CODE004, 0xD7F, BP);
L0D88:
    /* 0D88  mov     bp,sp */
    BP = SP;
L0D8A:
    /* 0D8A  add     word ptr [bp],2 */
    ww(pSS, BP, (uint16_t)(rw(pSS, BP) + 0x2));
L0D8E:
    /* 0D8E  sar     dx,1 */
    DX = sar16(DX, 1);
L0D90:
    /* 0D90  rcr     ax,1 */
    AX = rcr16(AX, 1);
L0D92:
    /* 0D92  cmp     dx,cx */
    sub16(DX, CX, 0);
L0D94:
    /* 0D94  je      short L0D9E */
    if (ZF) goto L0D9E;
L0D96:
    /* 0D96  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x065C, 0x0D96, 2)) != 0) return c;
L0D98:
    /* 0D98  mov     bp,word ptr cs:L0D7F */
    BP = rw(CODE004, 0xD7F);
L0D9D:
    /* 0D9D  iret */
    SP = (uint16_t)(SP + 4); asm_set_flags(pop16()); return ASM_IRET;
L0D9E: /* L0D9E */
    /* 0D9E  mov     ax,7FFFh */
    AX = 0x7FFF;
L0DA1:
    /* 0DA1  xor     dx,dx */
    DX = logic16((uint16_t)(DX ^ DX));
L0DA3:
    /* 0DA3  mov     bp,word ptr cs:L0D7F */
    BP = rw(CODE004, 0xD7F);
L0DA8:
    /* 0DA8  iret */
    SP = (uint16_t)(SP + 4); asm_set_flags(pop16()); return ASM_IRET;
}
