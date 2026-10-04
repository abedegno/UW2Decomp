/* interp.c: replaces src/3d/INTERP.ASM (seg004_27D0, 27D0..4975 of its
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

uint32_t asm_mod_INTERP(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x27D0: goto L27D0;
    case 0x27D2: goto L27D2;
    case 0x27D3: goto L27D3;
    case 0x27D4: goto L27D4;
    case 0x27D8: goto L27D8;
    case 0x27D9: goto L27D9;
    case 0x27DA: goto L27DA;
    case 0x27DC: goto L27DC;
    case 0x27E0: goto L27E0;
    case 0x27E5: goto L27E5;
    case 0x27E7: goto L27E7;
    case 0x27EB: goto L27EB;
    case 0x27ED: goto L27ED;
    case 0x27F0: goto L27F0;
    case 0x27F3: goto L27F3;
    case 0x27F7: goto L27F7;
    case 0x27F9: goto L27F9;
    case 0x27FD: goto L27FD;
    case 0x27FF: goto L27FF;
    case 0x2801: goto L2801;
    case 0x2805: goto L2805;
    case 0x2807: goto L2807;
    case 0x280B: goto L280B;
    case 0x280C: goto L280C;
    case 0x2811: goto L2811;
    case 0x2812: goto L2812;
    case 0x2813: goto L2813;
    case 0x2814: goto L2814;
    case 0x2818: goto L2818;
    case 0x2819: goto L2819;
    case 0x281B: goto L281B;
    case 0x281C: goto L281C;
    case 0x281E: goto L281E;
    case 0x2822: goto L2822;
    case 0x2826: goto L2826;
    case 0x2828: goto L2828;
    case 0x2829: goto L2829;
    case 0x282D: goto L282D;
    case 0x282F: goto L282F;
    case 0x2833: goto L2833;
    case 0x2837: goto L2837;
    case 0x283B: goto L283B;
    case 0x283D: goto L283D;
    case 0x2841: goto L2841;
    case 0x2843: goto L2843;
    case 0x2847: goto L2847;
    case 0x284B: goto L284B;
    case 0x284D: goto L284D;
    case 0x2851: goto L2851;
    case 0x2853: goto L2853;
    case 0x2855: goto L2855;
    case 0x2859: goto L2859;
    case 0x285D: goto L285D;
    case 0x2861: goto L2861;
    case 0x2863: goto L2863;
    case 0x2867: goto L2867;
    case 0x2869: goto L2869;
    case 0x286D: goto L286D;
    case 0x2871: goto L2871;
    case 0x2873: goto L2873;
    case 0x2877: goto L2877;
    case 0x2879: goto L2879;
    case 0x287E: goto L287E;
    case 0x287F: goto L287F;
    case 0x2880: goto L2880;
    case 0x2881: goto L2881;
    case 0x2885: goto L2885;
    case 0x2889: goto L2889;
    case 0x288D: goto L288D;
    case 0x288F: goto L288F;
    case 0x2891: goto L2891;
    case 0x2898: goto L2898;
    case 0x289B: goto L289B;
    case 0x289D: goto L289D;
    case 0x28A1: goto L28A1;
    case 0x28A3: goto L28A3;
    case 0x28A7: goto L28A7;
    case 0x28A9: goto L28A9;
    case 0x28AC: goto L28AC;
    case 0x28B0: goto L28B0;
    case 0x28B2: goto L28B2;
    case 0x28B6: goto L28B6;
    case 0x28B8: goto L28B8;
    case 0x28BF: goto L28BF;
    case 0x28C2: goto L28C2;
    case 0x28C5: goto L28C5;
    case 0x28C9: goto L28C9;
    case 0x28CB: goto L28CB;
    case 0x28CF: goto L28CF;
    case 0x28D1: goto L28D1;
    case 0x28D3: goto L28D3;
    case 0x28D7: goto L28D7;
    case 0x28D9: goto L28D9;
    case 0x28DD: goto L28DD;
    case 0x28DF: goto L28DF;
    case 0x28E4: goto L28E4;
    case 0x28E5: goto L28E5;
    case 0x28E6: goto L28E6;
    case 0x28E7: goto L28E7;
    case 0x28EB: goto L28EB;
    case 0x28ED: goto L28ED;
    case 0x28EE: goto L28EE;
    case 0x28F1: goto L28F1;
    case 0x28F4: goto L28F4;
    case 0x28F6: goto L28F6;
    case 0x28F8: goto L28F8;
    case 0x28FB: goto L28FB;
    case 0x28FD: goto L28FD;
    case 0x28FF: goto L28FF;
    case 0x2902: goto L2902;
    case 0x2904: goto L2904;
    case 0x2906: goto L2906;
    case 0x2909: goto L2909;
    case 0x290C: goto L290C;
    case 0x290E: goto L290E;
    case 0x2910: goto L2910;
    case 0x2912: goto L2912;
    case 0x2915: goto L2915;
    case 0x2917: goto L2917;
    case 0x2919: goto L2919;
    case 0x2920: goto L2920;
    case 0x2923: goto L2923;
    case 0x2926: goto L2926;
    case 0x2929: goto L2929;
    case 0x292C: goto L292C;
    case 0x292F: goto L292F;
    case 0x2932: goto L2932;
    case 0x2934: goto L2934;
    case 0x2936: goto L2936;
    case 0x2938: goto L2938;
    case 0x293A: goto L293A;
    case 0x293C: goto L293C;
    case 0x293F: goto L293F;
    case 0x2942: goto L2942;
    case 0x2944: goto L2944;
    case 0x2946: goto L2946;
    case 0x2948: goto L2948;
    case 0x294A: goto L294A;
    case 0x294D: goto L294D;
    case 0x2950: goto L2950;
    case 0x2952: goto L2952;
    case 0x2954: goto L2954;
    case 0x2956: goto L2956;
    case 0x2958: goto L2958;
    case 0x295A: goto L295A;
    case 0x295C: goto L295C;
    case 0x295E: goto L295E;
    case 0x2960: goto L2960;
    case 0x2962: goto L2962;
    case 0x2965: goto L2965;
    case 0x2967: goto L2967;
    case 0x296A: goto L296A;
    case 0x296C: goto L296C;
    case 0x296E: goto L296E;
    case 0x2971: goto L2971;
    case 0x2973: goto L2973;
    case 0x2976: goto L2976;
    case 0x2979: goto L2979;
    case 0x297B: goto L297B;
    case 0x297D: goto L297D;
    case 0x2980: goto L2980;
    case 0x2982: goto L2982;
    case 0x2984: goto L2984;
    case 0x2986: goto L2986;
    case 0x2988: goto L2988;
    case 0x298A: goto L298A;
    case 0x298C: goto L298C;
    case 0x298E: goto L298E;
    case 0x2990: goto L2990;
    case 0x2992: goto L2992;
    case 0x2995: goto L2995;
    case 0x2998: goto L2998;
    case 0x299F: goto L299F;
    case 0x29A2: goto L29A2;
    case 0x29A5: goto L29A5;
    case 0x29A8: goto L29A8;
    case 0x29AB: goto L29AB;
    case 0x29AE: goto L29AE;
    case 0x29B1: goto L29B1;
    case 0x29B3: goto L29B3;
    case 0x29B5: goto L29B5;
    case 0x29B7: goto L29B7;
    case 0x29B9: goto L29B9;
    case 0x29BB: goto L29BB;
    case 0x29BE: goto L29BE;
    case 0x29C1: goto L29C1;
    case 0x29C3: goto L29C3;
    case 0x29C5: goto L29C5;
    case 0x29C7: goto L29C7;
    case 0x29C9: goto L29C9;
    case 0x29CC: goto L29CC;
    case 0x29CF: goto L29CF;
    case 0x29D1: goto L29D1;
    case 0x29D3: goto L29D3;
    case 0x29D5: goto L29D5;
    case 0x29D7: goto L29D7;
    case 0x29D9: goto L29D9;
    case 0x29DB: goto L29DB;
    case 0x29DD: goto L29DD;
    case 0x29E0: goto L29E0;
    case 0x29E2: goto L29E2;
    case 0x29E5: goto L29E5;
    case 0x29E7: goto L29E7;
    case 0x29E9: goto L29E9;
    case 0x29EC: goto L29EC;
    case 0x29EE: goto L29EE;
    case 0x29F1: goto L29F1;
    case 0x29F3: goto L29F3;
    case 0x29F5: goto L29F5;
    case 0x29F8: goto L29F8;
    case 0x29FA: goto L29FA;
    case 0x29FC: goto L29FC;
    case 0x29FE: goto L29FE;
    case 0x2A00: goto L2A00;
    case 0x2A02: goto L2A02;
    case 0x2A04: goto L2A04;
    case 0x2A06: goto L2A06;
    case 0x2A08: goto L2A08;
    case 0x2A0A: goto L2A0A;
    case 0x2A0D: goto L2A0D;
    case 0x2A10: goto L2A10;
    case 0x2A13: goto L2A13;
    case 0x2A1A: goto L2A1A;
    case 0x2A1D: goto L2A1D;
    case 0x2A1F: goto L2A1F;
    case 0x2A21: goto L2A21;
    case 0x2A23: goto L2A23;
    case 0x2A26: goto L2A26;
    case 0x2A29: goto L2A29;
    case 0x2A2B: goto L2A2B;
    case 0x2A2D: goto L2A2D;
    case 0x2A2F: goto L2A2F;
    case 0x2A31: goto L2A31;
    case 0x2A33: goto L2A33;
    case 0x2A35: goto L2A35;
    case 0x2A37: goto L2A37;
    case 0x2A39: goto L2A39;
    case 0x2A3B: goto L2A3B;
    case 0x2A3D: goto L2A3D;
    case 0x2A3F: goto L2A3F;
    case 0x2A42: goto L2A42;
    case 0x2A44: goto L2A44;
    case 0x2A46: goto L2A46;
    case 0x2A48: goto L2A48;
    case 0x2A4A: goto L2A4A;
    case 0x2A4D: goto L2A4D;
    case 0x2A50: goto L2A50;
    case 0x2A52: goto L2A52;
    case 0x2A54: goto L2A54;
    case 0x2A56: goto L2A56;
    case 0x2A58: goto L2A58;
    case 0x2A5B: goto L2A5B;
    case 0x2A5E: goto L2A5E;
    case 0x2A61: goto L2A61;
    case 0x2A63: goto L2A63;
    case 0x2A65: goto L2A65;
    case 0x2A68: goto L2A68;
    case 0x2A6A: goto L2A6A;
    case 0x2A6D: goto L2A6D;
    case 0x2A6F: goto L2A6F;
    case 0x2A71: goto L2A71;
    case 0x2A74: goto L2A74;
    case 0x2A76: goto L2A76;
    case 0x2A78: goto L2A78;
    case 0x2A7A: goto L2A7A;
    case 0x2A7C: goto L2A7C;
    case 0x2A7E: goto L2A7E;
    case 0x2A80: goto L2A80;
    case 0x2A82: goto L2A82;
    case 0x2A84: goto L2A84;
    case 0x2A87: goto L2A87;
    case 0x2A8A: goto L2A8A;
    case 0x2A8D: goto L2A8D;
    case 0x2A94: goto L2A94;
    case 0x2A96: goto L2A96;
    case 0x2A99: goto L2A99;
    case 0x2A9C: goto L2A9C;
    case 0x2A9F: goto L2A9F;
    case 0x2AA1: goto L2AA1;
    case 0x2AA3: goto L2AA3;
    case 0x2AA5: goto L2AA5;
    case 0x2AA7: goto L2AA7;
    case 0x2AA9: goto L2AA9;
    case 0x2AAB: goto L2AAB;
    case 0x2AAD: goto L2AAD;
    case 0x2AB0: goto L2AB0;
    case 0x2AB3: goto L2AB3;
    case 0x2AB5: goto L2AB5;
    case 0x2AB7: goto L2AB7;
    case 0x2AB9: goto L2AB9;
    case 0x2ABB: goto L2ABB;
    case 0x2ABE: goto L2ABE;
    case 0x2AC1: goto L2AC1;
    case 0x2AC3: goto L2AC3;
    case 0x2AC5: goto L2AC5;
    case 0x2AC7: goto L2AC7;
    case 0x2AC9: goto L2AC9;
    case 0x2ACB: goto L2ACB;
    case 0x2ACD: goto L2ACD;
    case 0x2ACF: goto L2ACF;
    case 0x2AD2: goto L2AD2;
    case 0x2AD4: goto L2AD4;
    case 0x2AD7: goto L2AD7;
    case 0x2AD9: goto L2AD9;
    case 0x2ADB: goto L2ADB;
    case 0x2ADE: goto L2ADE;
    case 0x2AE0: goto L2AE0;
    case 0x2AE3: goto L2AE3;
    case 0x2AE6: goto L2AE6;
    case 0x2AE8: goto L2AE8;
    case 0x2AEA: goto L2AEA;
    case 0x2AED: goto L2AED;
    case 0x2AEF: goto L2AEF;
    case 0x2AF1: goto L2AF1;
    case 0x2AF3: goto L2AF3;
    case 0x2AF5: goto L2AF5;
    case 0x2AF7: goto L2AF7;
    case 0x2AF9: goto L2AF9;
    case 0x2AFB: goto L2AFB;
    case 0x2AFE: goto L2AFE;
    case 0x2B01: goto L2B01;
    case 0x2B04: goto L2B04;
    case 0x2B05: goto L2B05;
    case 0x2B07: goto L2B07;
    case 0x2B08: goto L2B08;
    case 0x2B0A: goto L2B0A;
    case 0x2B0C: goto L2B0C;
    case 0x2B0E: goto L2B0E;
    case 0x2B10: goto L2B10;
    case 0x2B11: goto L2B11;
    case 0x2B12: goto L2B12;
    case 0x2B16: goto L2B16;
    case 0x2B17: goto L2B17;
    case 0x2B19: goto L2B19;
    case 0x2B1A: goto L2B1A;
    case 0x2B1B: goto L2B1B;
    case 0x2B1C: goto L2B1C;
    case 0x2B20: goto L2B20;
    case 0x2B21: goto L2B21;
    case 0x2B23: goto L2B23;
    case 0x2B25: goto L2B25;
    case 0x2B26: goto L2B26;
    case 0x2B27: goto L2B27;
    case 0x2B29: goto L2B29;
    case 0x2B2A: goto L2B2A;
    case 0x2B2B: goto L2B2B;
    case 0x2B2F: goto L2B2F;
    case 0x2B30: goto L2B30;
    case 0x2B31: goto L2B31;
    case 0x2B33: goto L2B33;
    case 0x2B34: goto L2B34;
    case 0x2B35: goto L2B35;
    case 0x2B37: goto L2B37;
    case 0x2B38: goto L2B38;
    case 0x2B39: goto L2B39;
    case 0x2B3D: goto L2B3D;
    case 0x2B3E: goto L2B3E;
    case 0x2B41: goto L2B41;
    case 0x2B42: goto L2B42;
    case 0x2B44: goto L2B44;
    case 0x2B48: goto L2B48;
    case 0x2B4C: goto L2B4C;
    case 0x2B4F: goto L2B4F;
    case 0x2B52: goto L2B52;
    case 0x2B56: goto L2B56;
    case 0x2B58: goto L2B58;
    case 0x2B5B: goto L2B5B;
    case 0x2B5E: goto L2B5E;
    case 0x2B61: goto L2B61;
    case 0x2B66: goto L2B66;
    case 0x2B6A: goto L2B6A;
    case 0x2B6C: goto L2B6C;
    case 0x2B6D: goto L2B6D;
    case 0x2B6E: goto L2B6E;
    case 0x2B72: goto L2B72;
    case 0x2B73: goto L2B73;
    case 0x2B75: goto L2B75;
    case 0x2B78: goto L2B78;
    case 0x2B79: goto L2B79;
    case 0x2B7B: goto L2B7B;
    case 0x2B7E: goto L2B7E;
    case 0x2B80: goto L2B80;
    case 0x2B84: goto L2B84;
    case 0x2B88: goto L2B88;
    case 0x2B8B: goto L2B8B;
    case 0x2B8E: goto L2B8E;
    case 0x2B92: goto L2B92;
    case 0x2B94: goto L2B94;
    case 0x2B97: goto L2B97;
    case 0x2B9A: goto L2B9A;
    case 0x2B9D: goto L2B9D;
    case 0x2BA2: goto L2BA2;
    case 0x2BA6: goto L2BA6;
    case 0x2BA8: goto L2BA8;
    case 0x2BA9: goto L2BA9;
    case 0x2BAA: goto L2BAA;
    case 0x2BAE: goto L2BAE;
    case 0x2BAF: goto L2BAF;
    case 0x2BB1: goto L2BB1;
    case 0x2BB4: goto L2BB4;
    case 0x2BB5: goto L2BB5;
    case 0x2BB7: goto L2BB7;
    case 0x2BB8: goto L2BB8;
    case 0x2BBB: goto L2BBB;
    case 0x2BBD: goto L2BBD;
    case 0x2BBE: goto L2BBE;
    case 0x2BC1: goto L2BC1;
    case 0x2BC2: goto L2BC2;
    case 0x2BC3: goto L2BC3;
    case 0x2BC7: goto L2BC7;
    case 0x2BC8: goto L2BC8;
    case 0x2BCA: goto L2BCA;
    case 0x2BCB: goto L2BCB;
    case 0x2BCD: goto L2BCD;
    case 0x2BCF: goto L2BCF;
    case 0x2BD4: goto L2BD4;
    case 0x2BD8: goto L2BD8;
    case 0x2BDB: goto L2BDB;
    case 0x2BDC: goto L2BDC;
    case 0x2BDE: goto L2BDE;
    case 0x2BDF: goto L2BDF;
    case 0x2BE1: goto L2BE1;
    case 0x2BE3: goto L2BE3;
    case 0x2BE5: goto L2BE5;
    case 0x2BE7: goto L2BE7;
    case 0x2BEA: goto L2BEA;
    case 0x2BEC: goto L2BEC;
    case 0x2BED: goto L2BED;
    case 0x2BF0: goto L2BF0;
    case 0x2BF1: goto L2BF1;
    case 0x2BF2: goto L2BF2;
    case 0x2BF6: goto L2BF6;
    case 0x2BF7: goto L2BF7;
    case 0x2BFA: goto L2BFA;
    case 0x2BFE: goto L2BFE;
    case 0x2C01: goto L2C01;
    case 0x2C02: goto L2C02;
    case 0x2C04: goto L2C04;
    case 0x2C08: goto L2C08;
    case 0x2C0C: goto L2C0C;
    case 0x2C10: goto L2C10;
    case 0x2C13: goto L2C13;
    case 0x2C17: goto L2C17;
    case 0x2C18: goto L2C18;
    case 0x2C19: goto L2C19;
    case 0x2C1D: goto L2C1D;
    case 0x2C20: goto L2C20;
    case 0x2C21: goto L2C21;
    case 0x2C23: goto L2C23;
    case 0x2C26: goto L2C26;
    case 0x2C28: goto L2C28;
    case 0x2C2C: goto L2C2C;
    case 0x2C30: goto L2C30;
    case 0x2C34: goto L2C34;
    case 0x2C37: goto L2C37;
    case 0x2C3B: goto L2C3B;
    case 0x2C3C: goto L2C3C;
    case 0x2C3D: goto L2C3D;
    case 0x2C41: goto L2C41;
    case 0x2C42: goto L2C42;
    case 0x2C44: goto L2C44;
    case 0x2C46: goto L2C46;
    case 0x2C4A: goto L2C4A;
    case 0x2C4D: goto L2C4D;
    case 0x2C4E: goto L2C4E;
    case 0x2C4F: goto L2C4F;
    case 0x2C50: goto L2C50;
    case 0x2C51: goto L2C51;
    case 0x2C52: goto L2C52;
    case 0x2C55: goto L2C55;
    case 0x2C58: goto L2C58;
    case 0x2C5C: goto L2C5C;
    case 0x2C5E: goto L2C5E;
    case 0x2C5F: goto L2C5F;
    case 0x2C60: goto L2C60;
    case 0x2C64: goto L2C64;
    case 0x2C65: goto L2C65;
    case 0x2C67: goto L2C67;
    case 0x2C69: goto L2C69;
    case 0x2C6D: goto L2C6D;
    case 0x2C71: goto L2C71;
    case 0x2C72: goto L2C72;
    case 0x2C73: goto L2C73;
    case 0x2C74: goto L2C74;
    case 0x2C75: goto L2C75;
    case 0x2C76: goto L2C76;
    case 0x2C7A: goto L2C7A;
    case 0x2C7E: goto L2C7E;
    case 0x2C82: goto L2C82;
    case 0x2C84: goto L2C84;
    case 0x2C85: goto L2C85;
    case 0x2C86: goto L2C86;
    case 0x2C8A: goto L2C8A;
    case 0x2C8F: goto L2C8F;
    case 0x2C94: goto L2C94;
    case 0x2C95: goto L2C95;
    case 0x2C97: goto L2C97;
    case 0x2C99: goto L2C99;
    case 0x2C9C: goto L2C9C;
    case 0x2C9D: goto L2C9D;
    case 0x2C9E: goto L2C9E;
    case 0x2CA0: goto L2CA0;
    case 0x2CA4: goto L2CA4;
    case 0x2CA5: goto L2CA5;
    case 0x2CA6: goto L2CA6;
    case 0x2CA7: goto L2CA7;
    case 0x2CA8: goto L2CA8;
    case 0x2CAC: goto L2CAC;
    case 0x2CB0: goto L2CB0;
    case 0x2CB1: goto L2CB1;
    case 0x2CB2: goto L2CB2;
    case 0x2CB4: goto L2CB4;
    case 0x2CB8: goto L2CB8;
    case 0x2CBA: goto L2CBA;
    case 0x2CBC: goto L2CBC;
    case 0x2CC0: goto L2CC0;
    case 0x2CC2: goto L2CC2;
    case 0x2CC4: goto L2CC4;
    case 0x2CC8: goto L2CC8;
    case 0x2CCB: goto L2CCB;
    case 0x2CCC: goto L2CCC;
    case 0x2CCD: goto L2CCD;
    case 0x2CCF: goto L2CCF;
    case 0x2CD3: goto L2CD3;
    case 0x2CD4: goto L2CD4;
    case 0x2CD6: goto L2CD6;
    case 0x2CD8: goto L2CD8;
    case 0x2CD9: goto L2CD9;
    case 0x2CDA: goto L2CDA;
    case 0x2CDC: goto L2CDC;
    case 0x2CDD: goto L2CDD;
    case 0x2CDF: goto L2CDF;
    case 0x2CE0: goto L2CE0;
    case 0x2CE1: goto L2CE1;
    case 0x2CE3: goto L2CE3;
    case 0x2CE4: goto L2CE4;
    case 0x2CE7: goto L2CE7;
    case 0x2CEB: goto L2CEB;
    case 0x2CEF: goto L2CEF;
    case 0x2CF0: goto L2CF0;
    case 0x2CF1: goto L2CF1;
    case 0x2CF4: goto L2CF4;
    case 0x2CF5: goto L2CF5;
    case 0x2CF7: goto L2CF7;
    case 0x2CF9: goto L2CF9;
    case 0x2CFA: goto L2CFA;
    case 0x2CFB: goto L2CFB;
    case 0x2CFD: goto L2CFD;
    case 0x2CFF: goto L2CFF;
    case 0x2D00: goto L2D00;
    case 0x2D01: goto L2D01;
    case 0x2D03: goto L2D03;
    case 0x2D04: goto L2D04;
    case 0x2D07: goto L2D07;
    case 0x2D0B: goto L2D0B;
    case 0x2D0F: goto L2D0F;
    case 0x2D10: goto L2D10;
    case 0x2D11: goto L2D11;
    case 0x2D12: goto L2D12;
    case 0x2D14: goto L2D14;
    case 0x2D16: goto L2D16;
    case 0x2D17: goto L2D17;
    case 0x2D18: goto L2D18;
    case 0x2D1A: goto L2D1A;
    case 0x2D1E: goto L2D1E;
    case 0x2D1F: goto L2D1F;
    case 0x2D20: goto L2D20;
    case 0x2D21: goto L2D21;
    case 0x2D22: goto L2D22;
    case 0x2D26: goto L2D26;
    case 0x2D2A: goto L2D2A;
    case 0x2D2B: goto L2D2B;
    case 0x2D2C: goto L2D2C;
    case 0x2D2E: goto L2D2E;
    case 0x2D32: goto L2D32;
    case 0x2D34: goto L2D34;
    case 0x2D36: goto L2D36;
    case 0x2D3A: goto L2D3A;
    case 0x2D3C: goto L2D3C;
    case 0x2D3E: goto L2D3E;
    case 0x2D42: goto L2D42;
    case 0x2D43: goto L2D43;
    case 0x2D44: goto L2D44;
    case 0x2D46: goto L2D46;
    case 0x2D4A: goto L2D4A;
    case 0x2D4B: goto L2D4B;
    case 0x2D4D: goto L2D4D;
    case 0x2D4F: goto L2D4F;
    case 0x2D50: goto L2D50;
    case 0x2D51: goto L2D51;
    case 0x2D53: goto L2D53;
    case 0x2D54: goto L2D54;
    case 0x2D56: goto L2D56;
    case 0x2D57: goto L2D57;
    case 0x2D58: goto L2D58;
    case 0x2D5A: goto L2D5A;
    case 0x2D5B: goto L2D5B;
    case 0x2D5E: goto L2D5E;
    case 0x2D62: goto L2D62;
    case 0x2D66: goto L2D66;
    case 0x2D67: goto L2D67;
    case 0x2D68: goto L2D68;
    case 0x2D6B: goto L2D6B;
    case 0x2D6C: goto L2D6C;
    case 0x2D6E: goto L2D6E;
    case 0x2D70: goto L2D70;
    case 0x2D71: goto L2D71;
    case 0x2D72: goto L2D72;
    case 0x2D74: goto L2D74;
    case 0x2D76: goto L2D76;
    case 0x2D77: goto L2D77;
    case 0x2D78: goto L2D78;
    case 0x2D7A: goto L2D7A;
    case 0x2D7B: goto L2D7B;
    case 0x2D7E: goto L2D7E;
    case 0x2D82: goto L2D82;
    case 0x2D86: goto L2D86;
    case 0x2D87: goto L2D87;
    case 0x2D88: goto L2D88;
    case 0x2D8C: goto L2D8C;
    case 0x2D8F: goto L2D8F;
    case 0x2D94: goto L2D94;
    case 0x2D99: goto L2D99;
    case 0x2D9A: goto L2D9A;
    case 0x2D9C: goto L2D9C;
    case 0x2D9E: goto L2D9E;
    case 0x2DA1: goto L2DA1;
    case 0x2DA2: goto L2DA2;
    case 0x2DA3: goto L2DA3;
    case 0x2DA5: goto L2DA5;
    case 0x2DA9: goto L2DA9;
    case 0x2DAA: goto L2DAA;
    case 0x2DAB: goto L2DAB;
    case 0x2DAC: goto L2DAC;
    case 0x2DAD: goto L2DAD;
    case 0x2DB1: goto L2DB1;
    case 0x2DB5: goto L2DB5;
    case 0x2DB6: goto L2DB6;
    case 0x2DB7: goto L2DB7;
    case 0x2DB9: goto L2DB9;
    case 0x2DBD: goto L2DBD;
    case 0x2DBF: goto L2DBF;
    case 0x2DC3: goto L2DC3;
    case 0x2DC6: goto L2DC6;
    case 0x2DC7: goto L2DC7;
    case 0x2DC8: goto L2DC8;
    case 0x2DCA: goto L2DCA;
    case 0x2DCE: goto L2DCE;
    case 0x2DCF: goto L2DCF;
    case 0x2DD1: goto L2DD1;
    case 0x2DD3: goto L2DD3;
    case 0x2DD4: goto L2DD4;
    case 0x2DD5: goto L2DD5;
    case 0x2DD7: goto L2DD7;
    case 0x2DD8: goto L2DD8;
    case 0x2DD9: goto L2DD9;
    case 0x2DDB: goto L2DDB;
    case 0x2DDC: goto L2DDC;
    case 0x2DDF: goto L2DDF;
    case 0x2DE3: goto L2DE3;
    case 0x2DE7: goto L2DE7;
    case 0x2DE8: goto L2DE8;
    case 0x2DEB: goto L2DEB;
    case 0x2DEC: goto L2DEC;
    case 0x2DEE: goto L2DEE;
    case 0x2DF0: goto L2DF0;
    case 0x2DF1: goto L2DF1;
    case 0x2DF2: goto L2DF2;
    case 0x2DF4: goto L2DF4;
    case 0x2DF5: goto L2DF5;
    case 0x2DF6: goto L2DF6;
    case 0x2DF8: goto L2DF8;
    case 0x2DF9: goto L2DF9;
    case 0x2DFC: goto L2DFC;
    case 0x2E00: goto L2E00;
    case 0x2E04: goto L2E04;
    case 0x2E05: goto L2E05;
    case 0x2E06: goto L2E06;
    case 0x2E07: goto L2E07;
    case 0x2E09: goto L2E09;
    case 0x2E0B: goto L2E0B;
    case 0x2E0C: goto L2E0C;
    case 0x2E0D: goto L2E0D;
    case 0x2E0F: goto L2E0F;
    case 0x2E13: goto L2E13;
    case 0x2E14: goto L2E14;
    case 0x2E15: goto L2E15;
    case 0x2E16: goto L2E16;
    case 0x2E17: goto L2E17;
    case 0x2E1B: goto L2E1B;
    case 0x2E1F: goto L2E1F;
    case 0x2E20: goto L2E20;
    case 0x2E21: goto L2E21;
    case 0x2E23: goto L2E23;
    case 0x2E27: goto L2E27;
    case 0x2E29: goto L2E29;
    case 0x2E2D: goto L2E2D;
    case 0x2E2E: goto L2E2E;
    case 0x2E2F: goto L2E2F;
    case 0x2E31: goto L2E31;
    case 0x2E35: goto L2E35;
    case 0x2E36: goto L2E36;
    case 0x2E38: goto L2E38;
    case 0x2E3A: goto L2E3A;
    case 0x2E3B: goto L2E3B;
    case 0x2E3C: goto L2E3C;
    case 0x2E3E: goto L2E3E;
    case 0x2E3F: goto L2E3F;
    case 0x2E40: goto L2E40;
    case 0x2E42: goto L2E42;
    case 0x2E43: goto L2E43;
    case 0x2E46: goto L2E46;
    case 0x2E4A: goto L2E4A;
    case 0x2E4E: goto L2E4E;
    case 0x2E4F: goto L2E4F;
    case 0x2E52: goto L2E52;
    case 0x2E53: goto L2E53;
    case 0x2E55: goto L2E55;
    case 0x2E57: goto L2E57;
    case 0x2E58: goto L2E58;
    case 0x2E59: goto L2E59;
    case 0x2E5B: goto L2E5B;
    case 0x2E5C: goto L2E5C;
    case 0x2E5D: goto L2E5D;
    case 0x2E5F: goto L2E5F;
    case 0x2E60: goto L2E60;
    case 0x2E63: goto L2E63;
    case 0x2E67: goto L2E67;
    case 0x2E6B: goto L2E6B;
    case 0x2E6C: goto L2E6C;
    case 0x2E6D: goto L2E6D;
    case 0x2E71: goto L2E71;
    case 0x2E73: goto L2E73;
    case 0x2E74: goto L2E74;
    case 0x2E76: goto L2E76;
    case 0x2E79: goto L2E79;
    case 0x2E7B: goto L2E7B;
    case 0x2E7D: goto L2E7D;
    case 0x2E80: goto L2E80;
    case 0x2E81: goto L2E81;
    case 0x2E82: goto L2E82;
    case 0x2E84: goto L2E84;
    case 0x2E85: goto L2E85;
    case 0x2E86: goto L2E86;
    case 0x2E87: goto L2E87;
    case 0x2E89: goto L2E89;
    case 0x2E8B: goto L2E8B;
    case 0x2E8D: goto L2E8D;
    case 0x2E8E: goto L2E8E;
    case 0x2E90: goto L2E90;
    case 0x2E92: goto L2E92;
    case 0x2E94: goto L2E94;
    case 0x2E9A: goto L2E9A;
    case 0x2E9B: goto L2E9B;
    case 0x2E9C: goto L2E9C;
    case 0x2EA0: goto L2EA0;
    case 0x2EA4: goto L2EA4;
    case 0x2EA8: goto L2EA8;
    case 0x2EAC: goto L2EAC;
    case 0x2EAF: goto L2EAF;
    case 0x2EB3: goto L2EB3;
    case 0x2EB9: goto L2EB9;
    case 0x2EBB: goto L2EBB;
    case 0x2EBE: goto L2EBE;
    case 0x2EC4: goto L2EC4;
    case 0x2EC9: goto L2EC9;
    case 0x2ECB: goto L2ECB;
    case 0x2ECE: goto L2ECE;
    case 0x2ED3: goto L2ED3;
    case 0x2ED5: goto L2ED5;
    case 0x2ED6: goto L2ED6;
    case 0x2ED9: goto L2ED9;
    case 0x2EDB: goto L2EDB;
    case 0x2EDE: goto L2EDE;
    case 0x2EE2: goto L2EE2;
    case 0x2EE6: goto L2EE6;
    case 0x2EEA: goto L2EEA;
    case 0x2EEB: goto L2EEB;
    case 0x2EEE: goto L2EEE;
    case 0x2EF0: goto L2EF0;
    case 0x2EF2: goto L2EF2;
    case 0x2EF5: goto L2EF5;
    case 0x2EF6: goto L2EF6;
    case 0x2EF8: goto L2EF8;
    case 0x2EFC: goto L2EFC;
    case 0x2EFE: goto L2EFE;
    case 0x2F01: goto L2F01;
    case 0x2F02: goto L2F02;
    case 0x2F05: goto L2F05;
    case 0x2F07: goto L2F07;
    case 0x2F09: goto L2F09;
    case 0x2F0D: goto L2F0D;
    case 0x2F0F: goto L2F0F;
    case 0x2F11: goto L2F11;
    case 0x2F13: goto L2F13;
    case 0x2F15: goto L2F15;
    case 0x2F19: goto L2F19;
    case 0x2F1D: goto L2F1D;
    case 0x2F22: goto L2F22;
    case 0x2F24: goto L2F24;
    case 0x2F26: goto L2F26;
    case 0x2F27: goto L2F27;
    case 0x2F28: goto L2F28;
    case 0x2F29: goto L2F29;
    case 0x2F2D: goto L2F2D;
    case 0x2F2F: goto L2F2F;
    case 0x2F30: goto L2F30;
    case 0x2F37: goto L2F37;
    case 0x2F3A: goto L2F3A;
    case 0x2F3C: goto L2F3C;
    case 0x2F3F: goto L2F3F;
    case 0x2F43: goto L2F43;
    case 0x2F47: goto L2F47;
    case 0x2F4B: goto L2F4B;
    case 0x2F4C: goto L2F4C;
    case 0x2F4F: goto L2F4F;
    case 0x2F51: goto L2F51;
    case 0x2F53: goto L2F53;
    case 0x2F56: goto L2F56;
    case 0x2F58: goto L2F58;
    case 0x2F5B: goto L2F5B;
    case 0x2F5C: goto L2F5C;
    case 0x2F5E: goto L2F5E;
    case 0x2F62: goto L2F62;
    case 0x2F64: goto L2F64;
    case 0x2F67: goto L2F67;
    case 0x2F69: goto L2F69;
    case 0x2F6C: goto L2F6C;
    case 0x2F6D: goto L2F6D;
    case 0x2F70: goto L2F70;
    case 0x2F72: goto L2F72;
    case 0x2F74: goto L2F74;
    case 0x2F76: goto L2F76;
    case 0x2F7A: goto L2F7A;
    case 0x2F7C: goto L2F7C;
    case 0x2F7E: goto L2F7E;
    case 0x2F80: goto L2F80;
    case 0x2F84: goto L2F84;
    case 0x2F88: goto L2F88;
    case 0x2F8D: goto L2F8D;
    case 0x2F8F: goto L2F8F;
    case 0x2F91: goto L2F91;
    case 0x2F92: goto L2F92;
    case 0x2F93: goto L2F93;
    case 0x2F94: goto L2F94;
    case 0x2F98: goto L2F98;
    case 0x2F9F: goto L2F9F;
    case 0x2FA4: goto L2FA4;
    case 0x2FA6: goto L2FA6;
    case 0x2FA7: goto L2FA7;
    case 0x2FAA: goto L2FAA;
    case 0x2FAB: goto L2FAB;
    case 0x2FB0: goto L2FB0;
    case 0x2FB2: goto L2FB2;
    case 0x2FB7: goto L2FB7;
    case 0x2FB9: goto L2FB9;
    case 0x2FBB: goto L2FBB;
    case 0x2FBE: goto L2FBE;
    case 0x2FC1: goto L2FC1;
    case 0x2FC6: goto L2FC6;
    case 0x2FC8: goto L2FC8;
    case 0x2FC9: goto L2FC9;
    case 0x2FCC: goto L2FCC;
    case 0x2FCD: goto L2FCD;
    case 0x2FD2: goto L2FD2;
    case 0x2FD4: goto L2FD4;
    case 0x2FD9: goto L2FD9;
    case 0x2FDB: goto L2FDB;
    case 0x2FDD: goto L2FDD;
    case 0x2FE0: goto L2FE0;
    case 0x2FE3: goto L2FE3;
    case 0x2FE8: goto L2FE8;
    case 0x2FEA: goto L2FEA;
    case 0x2FEB: goto L2FEB;
    case 0x2FEE: goto L2FEE;
    case 0x2FEF: goto L2FEF;
    case 0x2FF4: goto L2FF4;
    case 0x2FF6: goto L2FF6;
    case 0x2FFB: goto L2FFB;
    case 0x2FFD: goto L2FFD;
    case 0x2FFF: goto L2FFF;
    case 0x3002: goto L3002;
    case 0x3005: goto L3005;
    case 0x300A: goto L300A;
    case 0x300C: goto L300C;
    case 0x300D: goto L300D;
    case 0x3010: goto L3010;
    case 0x3011: goto L3011;
    case 0x3016: goto L3016;
    case 0x3018: goto L3018;
    case 0x301D: goto L301D;
    case 0x301F: goto L301F;
    case 0x3021: goto L3021;
    case 0x3024: goto L3024;
    case 0x3027: goto L3027;
    case 0x3028: goto L3028;
    case 0x3029: goto L3029;
    case 0x302D: goto L302D;
    case 0x302E: goto L302E;
    case 0x3030: goto L3030;
    case 0x3031: goto L3031;
    case 0x3032: goto L3032;
    case 0x3036: goto L3036;
    case 0x3037: goto L3037;
    case 0x3038: goto L3038;
    case 0x303A: goto L303A;
    case 0x303B: goto L303B;
    case 0x303C: goto L303C;
    case 0x3040: goto L3040;
    case 0x3041: goto L3041;
    case 0x3042: goto L3042;
    case 0x3043: goto L3043;
    case 0x3047: goto L3047;
    case 0x3048: goto L3048;
    case 0x304A: goto L304A;
    case 0x304B: goto L304B;
    case 0x304D: goto L304D;
    case 0x304E: goto L304E;
    case 0x3050: goto L3050;
    case 0x3052: goto L3052;
    case 0x3054: goto L3054;
    case 0x3055: goto L3055;
    case 0x3056: goto L3056;
    case 0x305A: goto L305A;
    case 0x305B: goto L305B;
    case 0x305D: goto L305D;
    case 0x305E: goto L305E;
    case 0x3060: goto L3060;
    case 0x3061: goto L3061;
    case 0x3063: goto L3063;
    case 0x3065: goto L3065;
    case 0x3067: goto L3067;
    case 0x3068: goto L3068;
    case 0x3069: goto L3069;
    case 0x306D: goto L306D;
    case 0x306E: goto L306E;
    case 0x306F: goto L306F;
    case 0x3070: goto L3070;
    case 0x3072: goto L3072;
    case 0x3074: goto L3074;
    case 0x3075: goto L3075;
    case 0x3076: goto L3076;
    case 0x3078: goto L3078;
    case 0x3079: goto L3079;
    case 0x307A: goto L307A;
    case 0x307E: goto L307E;
    case 0x307F: goto L307F;
    case 0x3080: goto L3080;
    case 0x3081: goto L3081;
    case 0x3083: goto L3083;
    case 0x3084: goto L3084;
    case 0x3085: goto L3085;
    case 0x3089: goto L3089;
    case 0x308A: goto L308A;
    case 0x308B: goto L308B;
    case 0x308C: goto L308C;
    case 0x3090: goto L3090;
    case 0x3091: goto L3091;
    case 0x3093: goto L3093;
    case 0x3095: goto L3095;
    case 0x3096: goto L3096;
    case 0x3097: goto L3097;
    case 0x3098: goto L3098;
    case 0x309A: goto L309A;
    case 0x309B: goto L309B;
    case 0x309C: goto L309C;
    case 0x30A0: goto L30A0;
    case 0x30A1: goto L30A1;
    case 0x30A2: goto L30A2;
    case 0x30A3: goto L30A3;
    case 0x30A7: goto L30A7;
    case 0x30A8: goto L30A8;
    case 0x30A9: goto L30A9;
    case 0x30AA: goto L30AA;
    case 0x30AE: goto L30AE;
    case 0x30AF: goto L30AF;
    case 0x30B1: goto L30B1;
    case 0x30B2: goto L30B2;
    case 0x30B6: goto L30B6;
    case 0x30B8: goto L30B8;
    case 0x30BA: goto L30BA;
    case 0x30BC: goto L30BC;
    case 0x30BD: goto L30BD;
    case 0x30BF: goto L30BF;
    case 0x30C0: goto L30C0;
    case 0x30C4: goto L30C4;
    case 0x30C6: goto L30C6;
    case 0x30C8: goto L30C8;
    case 0x30CA: goto L30CA;
    case 0x30CB: goto L30CB;
    case 0x30CD: goto L30CD;
    case 0x30CE: goto L30CE;
    case 0x30D2: goto L30D2;
    case 0x30D4: goto L30D4;
    case 0x30D6: goto L30D6;
    case 0x30D8: goto L30D8;
    case 0x30DA: goto L30DA;
    case 0x30DB: goto L30DB;
    case 0x30DC: goto L30DC;
    case 0x30DE: goto L30DE;
    case 0x30DF: goto L30DF;
    case 0x30E0: goto L30E0;
    case 0x30E4: goto L30E4;
    case 0x30E5: goto L30E5;
    case 0x30E6: goto L30E6;
    case 0x30E7: goto L30E7;
    case 0x30E9: goto L30E9;
    case 0x30EA: goto L30EA;
    case 0x30EB: goto L30EB;
    case 0x30EF: goto L30EF;
    case 0x30F0: goto L30F0;
    case 0x30F1: goto L30F1;
    case 0x30F2: goto L30F2;
    case 0x30F6: goto L30F6;
    case 0x30F7: goto L30F7;
    case 0x30F9: goto L30F9;
    case 0x30FA: goto L30FA;
    case 0x30FE: goto L30FE;
    case 0x3100: goto L3100;
    case 0x3102: goto L3102;
    case 0x3104: goto L3104;
    case 0x3105: goto L3105;
    case 0x3107: goto L3107;
    case 0x3108: goto L3108;
    case 0x310C: goto L310C;
    case 0x310E: goto L310E;
    case 0x3110: goto L3110;
    case 0x3112: goto L3112;
    case 0x3114: goto L3114;
    case 0x3117: goto L3117;
    case 0x3118: goto L3118;
    case 0x3119: goto L3119;
    case 0x311B: goto L311B;
    case 0x311C: goto L311C;
    case 0x311D: goto L311D;
    case 0x3121: goto L3121;
    case 0x3122: goto L3122;
    case 0x3123: goto L3123;
    case 0x3124: goto L3124;
    case 0x3126: goto L3126;
    case 0x3127: goto L3127;
    case 0x3128: goto L3128;
    case 0x312C: goto L312C;
    case 0x312D: goto L312D;
    case 0x312E: goto L312E;
    case 0x312F: goto L312F;
    case 0x3133: goto L3133;
    case 0x3134: goto L3134;
    case 0x3136: goto L3136;
    case 0x3137: goto L3137;
    case 0x313B: goto L313B;
    case 0x313D: goto L313D;
    case 0x313F: goto L313F;
    case 0x3141: goto L3141;
    case 0x3142: goto L3142;
    case 0x3144: goto L3144;
    case 0x3145: goto L3145;
    case 0x3149: goto L3149;
    case 0x314B: goto L314B;
    case 0x314D: goto L314D;
    case 0x314F: goto L314F;
    case 0x3151: goto L3151;
    case 0x3154: goto L3154;
    case 0x3155: goto L3155;
    case 0x3156: goto L3156;
    case 0x3158: goto L3158;
    case 0x3159: goto L3159;
    case 0x315A: goto L315A;
    case 0x315E: goto L315E;
    case 0x315F: goto L315F;
    case 0x3160: goto L3160;
    case 0x3161: goto L3161;
    case 0x3163: goto L3163;
    case 0x3164: goto L3164;
    case 0x3165: goto L3165;
    case 0x3169: goto L3169;
    case 0x316A: goto L316A;
    case 0x316B: goto L316B;
    case 0x316C: goto L316C;
    case 0x3170: goto L3170;
    case 0x3171: goto L3171;
    case 0x3173: goto L3173;
    case 0x3174: goto L3174;
    case 0x3178: goto L3178;
    case 0x317A: goto L317A;
    case 0x317C: goto L317C;
    case 0x317E: goto L317E;
    case 0x317F: goto L317F;
    case 0x3181: goto L3181;
    case 0x3182: goto L3182;
    case 0x3186: goto L3186;
    case 0x3188: goto L3188;
    case 0x318A: goto L318A;
    case 0x318C: goto L318C;
    case 0x318E: goto L318E;
    case 0x3191: goto L3191;
    case 0x3192: goto L3192;
    case 0x3193: goto L3193;
    case 0x3195: goto L3195;
    case 0x3196: goto L3196;
    case 0x3197: goto L3197;
    case 0x319B: goto L319B;
    case 0x319C: goto L319C;
    case 0x319D: goto L319D;
    case 0x319E: goto L319E;
    case 0x31A0: goto L31A0;
    case 0x31A1: goto L31A1;
    case 0x31A2: goto L31A2;
    case 0x31A6: goto L31A6;
    case 0x31A7: goto L31A7;
    case 0x31A8: goto L31A8;
    case 0x31A9: goto L31A9;
    case 0x31AD: goto L31AD;
    case 0x31B3: goto L31B3;
    case 0x31B4: goto L31B4;
    case 0x31B8: goto L31B8;
    case 0x31BA: goto L31BA;
    case 0x31BB: goto L31BB;
    case 0x31BC: goto L31BC;
    case 0x31C0: goto L31C0;
    case 0x31C1: goto L31C1;
    case 0x31C3: goto L31C3;
    case 0x31C5: goto L31C5;
    case 0x31C7: goto L31C7;
    case 0x31CA: goto L31CA;
    case 0x31CB: goto L31CB;
    case 0x31CF: goto L31CF;
    case 0x31D1: goto L31D1;
    case 0x31D2: goto L31D2;
    case 0x31D3: goto L31D3;
    case 0x31D7: goto L31D7;
    case 0x31D8: goto L31D8;
    case 0x31DA: goto L31DA;
    case 0x31DC: goto L31DC;
    case 0x31DE: goto L31DE;
    case 0x31E1: goto L31E1;
    case 0x31E2: goto L31E2;
    case 0x31E6: goto L31E6;
    case 0x31E8: goto L31E8;
    case 0x31E9: goto L31E9;
    case 0x31EA: goto L31EA;
    case 0x31EE: goto L31EE;
    case 0x31EF: goto L31EF;
    case 0x31F1: goto L31F1;
    case 0x31F3: goto L31F3;
    case 0x31F5: goto L31F5;
    case 0x31F8: goto L31F8;
    case 0x31FB: goto L31FB;
    case 0x31FC: goto L31FC;
    case 0x31FD: goto L31FD;
    case 0x3201: goto L3201;
    case 0x3207: goto L3207;
    case 0x3208: goto L3208;
    case 0x3209: goto L3209;
    case 0x320D: goto L320D;
    case 0x320E: goto L320E;
    case 0x3210: goto L3210;
    case 0x3211: goto L3211;
    case 0x3214: goto L3214;
    case 0x3215: goto L3215;
    case 0x3217: goto L3217;
    case 0x3219: goto L3219;
    case 0x321B: goto L321B;
    case 0x321C: goto L321C;
    case 0x321E: goto L321E;
    case 0x3220: goto L3220;
    case 0x3222: goto L3222;
    case 0x3224: goto L3224;
    case 0x3226: goto L3226;
    case 0x3229: goto L3229;
    case 0x322B: goto L322B;
    case 0x322D: goto L322D;
    case 0x322F: goto L322F;
    case 0x3232: goto L3232;
    case 0x3233: goto L3233;
    case 0x3234: goto L3234;
    case 0x3238: goto L3238;
    case 0x3239: goto L3239;
    case 0x323B: goto L323B;
    case 0x323C: goto L323C;
    case 0x3240: goto L3240;
    case 0x3242: goto L3242;
    case 0x3244: goto L3244;
    case 0x3246: goto L3246;
    case 0x3247: goto L3247;
    case 0x3249: goto L3249;
    case 0x324C: goto L324C;
    case 0x324E: goto L324E;
    case 0x3250: goto L3250;
    case 0x3251: goto L3251;
    case 0x3252: goto L3252;
    case 0x3254: goto L3254;
    case 0x3255: goto L3255;
    case 0x3257: goto L3257;
    case 0x3258: goto L3258;
    case 0x3259: goto L3259;
    case 0x325B: goto L325B;
    case 0x325C: goto L325C;
    case 0x325E: goto L325E;
    case 0x3260: goto L3260;
    case 0x3262: goto L3262;
    case 0x3264: goto L3264;
    case 0x3266: goto L3266;
    case 0x3268: goto L3268;
    case 0x3269: goto L3269;
    case 0x326B: goto L326B;
    case 0x326C: goto L326C;
    case 0x326D: goto L326D;
    case 0x3271: goto L3271;
    case 0x3273: goto L3273;
    case 0x3274: goto L3274;
    case 0x3276: goto L3276;
    case 0x3277: goto L3277;
    case 0x3278: goto L3278;
    case 0x327C: goto L327C;
    case 0x327E: goto L327E;
    case 0x3280: goto L3280;
    case 0x3282: goto L3282;
    case 0x3283: goto L3283;
    case 0x3285: goto L3285;
    case 0x3286: goto L3286;
    case 0x3287: goto L3287;
    case 0x328B: goto L328B;
    case 0x328C: goto L328C;
    case 0x328E: goto L328E;
    case 0x328F: goto L328F;
    case 0x3293: goto L3293;
    case 0x3295: goto L3295;
    case 0x3299: goto L3299;
    case 0x329D: goto L329D;
    case 0x32A1: goto L32A1;
    case 0x32A3: goto L32A3;
    case 0x32A7: goto L32A7;
    case 0x32A9: goto L32A9;
    case 0x32AC: goto L32AC;
    case 0x32AE: goto L32AE;
    case 0x32B0: goto L32B0;
    case 0x32B3: goto L32B3;
    case 0x32B5: goto L32B5;
    case 0x32B7: goto L32B7;
    case 0x32B8: goto L32B8;
    case 0x32BA: goto L32BA;
    case 0x32BE: goto L32BE;
    case 0x32C2: goto L32C2;
    case 0x32C6: goto L32C6;
    case 0x32C9: goto L32C9;
    case 0x32CD: goto L32CD;
    case 0x32CE: goto L32CE;
    case 0x32CF: goto L32CF;
    case 0x32D3: goto L32D3;
    case 0x32D6: goto L32D6;
    case 0x32D8: goto L32D8;
    case 0x32DB: goto L32DB;
    case 0x32DD: goto L32DD;
    case 0x32E0: goto L32E0;
    case 0x32E1: goto L32E1;
    case 0x32E3: goto L32E3;
    case 0x32E6: goto L32E6;
    case 0x32E8: goto L32E8;
    case 0x32EA: goto L32EA;
    case 0x32EC: goto L32EC;
    case 0x32EF: goto L32EF;
    case 0x32F4: goto L32F4;
    case 0x32F6: goto L32F6;
    case 0x32F8: goto L32F8;
    case 0x32FA: goto L32FA;
    case 0x32FC: goto L32FC;
    case 0x32FE: goto L32FE;
    case 0x3300: goto L3300;
    case 0x3303: goto L3303;
    case 0x3305: goto L3305;
    case 0x3308: goto L3308;
    case 0x330D: goto L330D;
    case 0x330F: goto L330F;
    case 0x3311: goto L3311;
    case 0x3313: goto L3313;
    case 0x3315: goto L3315;
    case 0x3317: goto L3317;
    case 0x3319: goto L3319;
    case 0x331C: goto L331C;
    case 0x3320: goto L3320;
    case 0x3324: goto L3324;
    case 0x3328: goto L3328;
    case 0x3329: goto L3329;
    case 0x332D: goto L332D;
    case 0x332E: goto L332E;
    case 0x3332: goto L3332;
    case 0x3333: goto L3333;
    case 0x3337: goto L3337;
    case 0x3338: goto L3338;
    case 0x333B: goto L333B;
    case 0x333D: goto L333D;
    case 0x333F: goto L333F;
    case 0x3343: goto L3343;
    case 0x3347: goto L3347;
    case 0x334B: goto L334B;
    case 0x334F: goto L334F;
    case 0x3353: goto L3353;
    case 0x3357: goto L3357;
    case 0x335B: goto L335B;
    case 0x335F: goto L335F;
    case 0x3363: goto L3363;
    case 0x3365: goto L3365;
    case 0x3366: goto L3366;
    case 0x3369: goto L3369;
    case 0x336A: goto L336A;
    case 0x336D: goto L336D;
    case 0x336E: goto L336E;
    case 0x336F: goto L336F;
    case 0x3373: goto L3373;
    case 0x3377: goto L3377;
    case 0x337B: goto L337B;
    case 0x337F: goto L337F;
    case 0x3383: goto L3383;
    case 0x3387: goto L3387;
    case 0x338B: goto L338B;
    case 0x338F: goto L338F;
    case 0x3393: goto L3393;
    case 0x3397: goto L3397;
    case 0x3398: goto L3398;
    case 0x339C: goto L339C;
    case 0x33A0: goto L33A0;
    case 0x33A4: goto L33A4;
    case 0x33A7: goto L33A7;
    case 0x33A8: goto L33A8;
    case 0x33A9: goto L33A9;
    case 0x33AD: goto L33AD;
    case 0x33B0: goto L33B0;
    case 0x33B1: goto L33B1;
    case 0x33B2: goto L33B2;
    case 0x33B6: goto L33B6;
    case 0x33B7: goto L33B7;
    case 0x33BB: goto L33BB;
    case 0x33BF: goto L33BF;
    case 0x33C3: goto L33C3;
    case 0x33C6: goto L33C6;
    case 0x33C7: goto L33C7;
    case 0x33C8: goto L33C8;
    case 0x33CC: goto L33CC;
    case 0x33CD: goto L33CD;
    case 0x33CF: goto L33CF;
    case 0x33D0: goto L33D0;
    case 0x33D1: goto L33D1;
    case 0x33D5: goto L33D5;
    case 0x33D9: goto L33D9;
    case 0x33DA: goto L33DA;
    case 0x33DB: goto L33DB;
    case 0x33DC: goto L33DC;
    case 0x33DD: goto L33DD;
    case 0x33DF: goto L33DF;
    case 0x33E0: goto L33E0;
    case 0x33E1: goto L33E1;
    case 0x33E5: goto L33E5;
    case 0x33E6: goto L33E6;
    case 0x33E8: goto L33E8;
    case 0x33E9: goto L33E9;
    case 0x33ED: goto L33ED;
    case 0x33EF: goto L33EF;
    case 0x33F3: goto L33F3;
    case 0x33F7: goto L33F7;
    case 0x33FB: goto L33FB;
    case 0x33FD: goto L33FD;
    case 0x3401: goto L3401;
    case 0x3403: goto L3403;
    case 0x3405: goto L3405;
    case 0x3409: goto L3409;
    case 0x340A: goto L340A;
    case 0x340C: goto L340C;
    case 0x3410: goto L3410;
    case 0x3414: goto L3414;
    case 0x3416: goto L3416;
    case 0x3418: goto L3418;
    case 0x341C: goto L341C;
    case 0x341E: goto L341E;
    case 0x3420: goto L3420;
    case 0x3422: goto L3422;
    case 0x3424: goto L3424;
    case 0x3425: goto L3425;
    case 0x3426: goto L3426;
    case 0x3428: goto L3428;
    case 0x342A: goto L342A;
    case 0x342C: goto L342C;
    case 0x342E: goto L342E;
    case 0x3430: goto L3430;
    case 0x3431: goto L3431;
    case 0x3433: goto L3433;
    case 0x3435: goto L3435;
    case 0x3437: goto L3437;
    case 0x343B: goto L343B;
    case 0x343C: goto L343C;
    case 0x343D: goto L343D;
    case 0x3441: goto L3441;
    case 0x3442: goto L3442;
    case 0x3444: goto L3444;
    case 0x3445: goto L3445;
    case 0x3449: goto L3449;
    case 0x344B: goto L344B;
    case 0x344F: goto L344F;
    case 0x3453: goto L3453;
    case 0x3457: goto L3457;
    case 0x3459: goto L3459;
    case 0x345D: goto L345D;
    case 0x345F: goto L345F;
    case 0x3462: goto L3462;
    case 0x3464: goto L3464;
    case 0x3466: goto L3466;
    case 0x3469: goto L3469;
    case 0x346B: goto L346B;
    case 0x346D: goto L346D;
    case 0x346E: goto L346E;
    case 0x3470: goto L3470;
    case 0x3474: goto L3474;
    case 0x3478: goto L3478;
    case 0x347C: goto L347C;
    case 0x347F: goto L347F;
    case 0x3483: goto L3483;
    case 0x3484: goto L3484;
    case 0x3485: goto L3485;
    case 0x3489: goto L3489;
    case 0x348A: goto L348A;
    case 0x348C: goto L348C;
    case 0x348D: goto L348D;
    case 0x3491: goto L3491;
    case 0x3493: goto L3493;
    case 0x3497: goto L3497;
    case 0x349B: goto L349B;
    case 0x349F: goto L349F;
    case 0x34A1: goto L34A1;
    case 0x34A5: goto L34A5;
    case 0x34A7: goto L34A7;
    case 0x34AA: goto L34AA;
    case 0x34AC: goto L34AC;
    case 0x34AD: goto L34AD;
    case 0x34AF: goto L34AF;
    case 0x34B3: goto L34B3;
    case 0x34B7: goto L34B7;
    case 0x34B9: goto L34B9;
    case 0x34BB: goto L34BB;
    case 0x34BF: goto L34BF;
    case 0x34C1: goto L34C1;
    case 0x34C3: goto L34C3;
    case 0x34C5: goto L34C5;
    case 0x34C7: goto L34C7;
    case 0x34C8: goto L34C8;
    case 0x34C9: goto L34C9;
    case 0x34CB: goto L34CB;
    case 0x34CD: goto L34CD;
    case 0x34CF: goto L34CF;
    case 0x34D1: goto L34D1;
    case 0x34D3: goto L34D3;
    case 0x34D4: goto L34D4;
    case 0x34D6: goto L34D6;
    case 0x34D8: goto L34D8;
    case 0x34DA: goto L34DA;
    case 0x34DE: goto L34DE;
    case 0x34DF: goto L34DF;
    case 0x34E0: goto L34E0;
    case 0x34E4: goto L34E4;
    case 0x34E5: goto L34E5;
    case 0x34E7: goto L34E7;
    case 0x34E8: goto L34E8;
    case 0x34EC: goto L34EC;
    case 0x34EE: goto L34EE;
    case 0x34F2: goto L34F2;
    case 0x34F6: goto L34F6;
    case 0x34FA: goto L34FA;
    case 0x34FC: goto L34FC;
    case 0x3500: goto L3500;
    case 0x3502: goto L3502;
    case 0x3505: goto L3505;
    case 0x3507: goto L3507;
    case 0x3509: goto L3509;
    case 0x350C: goto L350C;
    case 0x350E: goto L350E;
    case 0x3510: goto L3510;
    case 0x3511: goto L3511;
    case 0x3513: goto L3513;
    case 0x3517: goto L3517;
    case 0x351B: goto L351B;
    case 0x351F: goto L351F;
    case 0x3522: goto L3522;
    case 0x3526: goto L3526;
    case 0x3527: goto L3527;
    case 0x3528: goto L3528;
    case 0x352C: goto L352C;
    case 0x3530: goto L3530;
    case 0x3531: goto L3531;
    case 0x3533: goto L3533;
    case 0x3535: goto L3535;
    case 0x3539: goto L3539;
    case 0x353B: goto L353B;
    case 0x353E: goto L353E;
    case 0x3540: goto L3540;
    case 0x3542: goto L3542;
    case 0x3545: goto L3545;
    case 0x3547: goto L3547;
    case 0x3548: goto L3548;
    case 0x354A: goto L354A;
    case 0x354C: goto L354C;
    case 0x354E: goto L354E;
    case 0x3552: goto L3552;
    case 0x3554: goto L3554;
    case 0x3557: goto L3557;
    case 0x3559: goto L3559;
    case 0x355B: goto L355B;
    case 0x355E: goto L355E;
    case 0x3560: goto L3560;
    case 0x3562: goto L3562;
    case 0x3563: goto L3563;
    case 0x3565: goto L3565;
    case 0x3569: goto L3569;
    case 0x356D: goto L356D;
    case 0x3571: goto L3571;
    case 0x3572: goto L3572;
    case 0x3574: goto L3574;
    case 0x3578: goto L3578;
    case 0x357C: goto L357C;
    case 0x3580: goto L3580;
    case 0x3583: goto L3583;
    case 0x3587: goto L3587;
    case 0x3588: goto L3588;
    case 0x3589: goto L3589;
    case 0x358D: goto L358D;
    case 0x3591: goto L3591;
    case 0x3592: goto L3592;
    case 0x3594: goto L3594;
    case 0x3596: goto L3596;
    case 0x359A: goto L359A;
    case 0x359C: goto L359C;
    case 0x359F: goto L359F;
    case 0x35A1: goto L35A1;
    case 0x35A3: goto L35A3;
    case 0x35A6: goto L35A6;
    case 0x35A8: goto L35A8;
    case 0x35A9: goto L35A9;
    case 0x35AB: goto L35AB;
    case 0x35AD: goto L35AD;
    case 0x35AF: goto L35AF;
    case 0x35B3: goto L35B3;
    case 0x35B5: goto L35B5;
    case 0x35B8: goto L35B8;
    case 0x35BA: goto L35BA;
    case 0x35BC: goto L35BC;
    case 0x35BF: goto L35BF;
    case 0x35C1: goto L35C1;
    case 0x35C3: goto L35C3;
    case 0x35C4: goto L35C4;
    case 0x35C6: goto L35C6;
    case 0x35CA: goto L35CA;
    case 0x35CE: goto L35CE;
    case 0x35D2: goto L35D2;
    case 0x35D3: goto L35D3;
    case 0x35D5: goto L35D5;
    case 0x35D9: goto L35D9;
    case 0x35DD: goto L35DD;
    case 0x35E1: goto L35E1;
    case 0x35E4: goto L35E4;
    case 0x35E8: goto L35E8;
    case 0x35E9: goto L35E9;
    case 0x35EA: goto L35EA;
    case 0x35EE: goto L35EE;
    case 0x35F2: goto L35F2;
    case 0x35F3: goto L35F3;
    case 0x35F5: goto L35F5;
    case 0x35F7: goto L35F7;
    case 0x35FB: goto L35FB;
    case 0x35FD: goto L35FD;
    case 0x3600: goto L3600;
    case 0x3602: goto L3602;
    case 0x3604: goto L3604;
    case 0x3607: goto L3607;
    case 0x3609: goto L3609;
    case 0x360A: goto L360A;
    case 0x360C: goto L360C;
    case 0x360E: goto L360E;
    case 0x3610: goto L3610;
    case 0x3614: goto L3614;
    case 0x3616: goto L3616;
    case 0x3619: goto L3619;
    case 0x361B: goto L361B;
    case 0x361D: goto L361D;
    case 0x3620: goto L3620;
    case 0x3622: goto L3622;
    case 0x3624: goto L3624;
    case 0x3625: goto L3625;
    case 0x3627: goto L3627;
    case 0x362B: goto L362B;
    case 0x362F: goto L362F;
    case 0x3633: goto L3633;
    case 0x3634: goto L3634;
    case 0x3636: goto L3636;
    case 0x363A: goto L363A;
    case 0x363E: goto L363E;
    case 0x3642: goto L3642;
    case 0x3645: goto L3645;
    case 0x3649: goto L3649;
    case 0x364A: goto L364A;
    case 0x364B: goto L364B;
    case 0x364F: goto L364F;
    case 0x3653: goto L3653;
    case 0x3654: goto L3654;
    case 0x3656: goto L3656;
    case 0x3658: goto L3658;
    case 0x365C: goto L365C;
    case 0x365E: goto L365E;
    case 0x3661: goto L3661;
    case 0x3663: goto L3663;
    case 0x3665: goto L3665;
    case 0x3666: goto L3666;
    case 0x3668: goto L3668;
    case 0x366A: goto L366A;
    case 0x366E: goto L366E;
    case 0x3670: goto L3670;
    case 0x3673: goto L3673;
    case 0x3675: goto L3675;
    case 0x3677: goto L3677;
    case 0x3678: goto L3678;
    case 0x367A: goto L367A;
    case 0x367E: goto L367E;
    case 0x3682: goto L3682;
    case 0x3686: goto L3686;
    case 0x3687: goto L3687;
    case 0x3689: goto L3689;
    case 0x368D: goto L368D;
    case 0x3691: goto L3691;
    case 0x3695: goto L3695;
    case 0x3698: goto L3698;
    case 0x369C: goto L369C;
    case 0x369D: goto L369D;
    case 0x369E: goto L369E;
    case 0x36A2: goto L36A2;
    case 0x36A6: goto L36A6;
    case 0x36A7: goto L36A7;
    case 0x36A9: goto L36A9;
    case 0x36AB: goto L36AB;
    case 0x36AF: goto L36AF;
    case 0x36B1: goto L36B1;
    case 0x36B4: goto L36B4;
    case 0x36B6: goto L36B6;
    case 0x36B8: goto L36B8;
    case 0x36B9: goto L36B9;
    case 0x36BB: goto L36BB;
    case 0x36BD: goto L36BD;
    case 0x36BF: goto L36BF;
    case 0x36C0: goto L36C0;
    case 0x36C2: goto L36C2;
    case 0x36C6: goto L36C6;
    case 0x36CA: goto L36CA;
    case 0x36CE: goto L36CE;
    case 0x36CF: goto L36CF;
    case 0x36D1: goto L36D1;
    case 0x36D5: goto L36D5;
    case 0x36D9: goto L36D9;
    case 0x36DD: goto L36DD;
    case 0x36E0: goto L36E0;
    case 0x36E4: goto L36E4;
    case 0x36E5: goto L36E5;
    case 0x36E6: goto L36E6;
    case 0x36EA: goto L36EA;
    case 0x36EE: goto L36EE;
    case 0x36EF: goto L36EF;
    case 0x36F1: goto L36F1;
    case 0x36F3: goto L36F3;
    case 0x36F7: goto L36F7;
    case 0x36F9: goto L36F9;
    case 0x36FC: goto L36FC;
    case 0x36FE: goto L36FE;
    case 0x3700: goto L3700;
    case 0x3701: goto L3701;
    case 0x3703: goto L3703;
    case 0x3705: goto L3705;
    case 0x3707: goto L3707;
    case 0x3708: goto L3708;
    case 0x370A: goto L370A;
    case 0x370E: goto L370E;
    case 0x3712: goto L3712;
    case 0x3716: goto L3716;
    case 0x3717: goto L3717;
    case 0x3719: goto L3719;
    case 0x371D: goto L371D;
    case 0x3721: goto L3721;
    case 0x3725: goto L3725;
    case 0x3728: goto L3728;
    case 0x372C: goto L372C;
    case 0x372D: goto L372D;
    case 0x372E: goto L372E;
    case 0x3732: goto L3732;
    case 0x3733: goto L3733;
    case 0x3736: goto L3736;
    case 0x373A: goto L373A;
    case 0x373D: goto L373D;
    case 0x3741: goto L3741;
    case 0x3744: goto L3744;
    case 0x3746: goto L3746;
    case 0x374B: goto L374B;
    case 0x374E: goto L374E;
    case 0x3750: goto L3750;
    case 0x3751: goto L3751;
    case 0x3752: goto L3752;
    case 0x3756: goto L3756;
    case 0x3757: goto L3757;
    case 0x3759: goto L3759;
    case 0x375A: goto L375A;
    case 0x375C: goto L375C;
    case 0x375E: goto L375E;
    case 0x3760: goto L3760;
    case 0x3763: goto L3763;
    case 0x3764: goto L3764;
    case 0x3765: goto L3765;
    case 0x3766: goto L3766;
    case 0x3767: goto L3767;
    case 0x3769: goto L3769;
    case 0x376D: goto L376D;
    case 0x3771: goto L3771;
    case 0x3775: goto L3775;
    case 0x3779: goto L3779;
    case 0x377C: goto L377C;
    case 0x377D: goto L377D;
    case 0x377F: goto L377F;
    case 0x3780: goto L3780;
    case 0x3783: goto L3783;
    case 0x3784: goto L3784;
    case 0x3785: goto L3785;
    case 0x3787: goto L3787;
    case 0x378B: goto L378B;
    case 0x378D: goto L378D;
    case 0x378E: goto L378E;
    case 0x3790: goto L3790;
    case 0x3793: goto L3793;
    case 0x3795: goto L3795;
    case 0x3797: goto L3797;
    case 0x3798: goto L3798;
    case 0x379B: goto L379B;
    case 0x379F: goto L379F;
    case 0x37A3: goto L37A3;
    case 0x37A5: goto L37A5;
    case 0x37A6: goto L37A6;
    case 0x37A8: goto L37A8;
    case 0x37AB: goto L37AB;
    case 0x37AD: goto L37AD;
    case 0x37AF: goto L37AF;
    case 0x37B0: goto L37B0;
    case 0x37B3: goto L37B3;
    case 0x37B7: goto L37B7;
    case 0x37BB: goto L37BB;
    case 0x37BD: goto L37BD;
    case 0x37BE: goto L37BE;
    case 0x37C0: goto L37C0;
    case 0x37C3: goto L37C3;
    case 0x37C5: goto L37C5;
    case 0x37C7: goto L37C7;
    case 0x37C8: goto L37C8;
    case 0x37CB: goto L37CB;
    case 0x37CF: goto L37CF;
    case 0x37D3: goto L37D3;
    case 0x37D7: goto L37D7;
    case 0x37D9: goto L37D9;
    case 0x37DD: goto L37DD;
    case 0x37E0: goto L37E0;
    case 0x37E4: goto L37E4;
    case 0x37E8: goto L37E8;
    case 0x37EC: goto L37EC;
    case 0x37EF: goto L37EF;
    case 0x37F2: goto L37F2;
    case 0x37F6: goto L37F6;
    case 0x37FA: goto L37FA;
    case 0x37FE: goto L37FE;
    case 0x3801: goto L3801;
    case 0x3804: goto L3804;
    case 0x3808: goto L3808;
    case 0x380C: goto L380C;
    case 0x3810: goto L3810;
    case 0x3813: goto L3813;
    case 0x3816: goto L3816;
    case 0x381A: goto L381A;
    case 0x381E: goto L381E;
    case 0x3822: goto L3822;
    case 0x3826: goto L3826;
    case 0x3829: goto L3829;
    case 0x382A: goto L382A;
    case 0x382C: goto L382C;
    case 0x382D: goto L382D;
    case 0x382E: goto L382E;
    case 0x382F: goto L382F;
    case 0x3833: goto L3833;
    case 0x3837: goto L3837;
    case 0x3839: goto L3839;
    case 0x383B: goto L383B;
    case 0x383E: goto L383E;
    case 0x3842: goto L3842;
    case 0x3846: goto L3846;
    case 0x384A: goto L384A;
    case 0x384D: goto L384D;
    case 0x3850: goto L3850;
    case 0x3854: goto L3854;
    case 0x3858: goto L3858;
    case 0x385C: goto L385C;
    case 0x385F: goto L385F;
    case 0x3862: goto L3862;
    case 0x3866: goto L3866;
    case 0x386A: goto L386A;
    case 0x386E: goto L386E;
    case 0x3871: goto L3871;
    case 0x3875: goto L3875;
    case 0x3879: goto L3879;
    case 0x387D: goto L387D;
    case 0x3881: goto L3881;
    case 0x3884: goto L3884;
    case 0x3886: goto L3886;
    case 0x3887: goto L3887;
    case 0x3888: goto L3888;
    case 0x3889: goto L3889;
    case 0x388D: goto L388D;
    case 0x388E: goto L388E;
    case 0x3890: goto L3890;
    case 0x3892: goto L3892;
    case 0x3893: goto L3893;
    case 0x3894: goto L3894;
    case 0x3896: goto L3896;
    case 0x3897: goto L3897;
    case 0x389A: goto L389A;
    case 0x389C: goto L389C;
    case 0x389E: goto L389E;
    case 0x38A3: goto L38A3;
    case 0x38A4: goto L38A4;
    case 0x38A7: goto L38A7;
    case 0x38AD: goto L38AD;
    case 0x38B3: goto L38B3;
    case 0x38B4: goto L38B4;
    case 0x38B5: goto L38B5;
    case 0x38B9: goto L38B9;
    case 0x38BF: goto L38BF;
    case 0x38C1: goto L38C1;
    case 0x38C7: goto L38C7;
    case 0x38CD: goto L38CD;
    case 0x38CF: goto L38CF;
    case 0x38D2: goto L38D2;
    case 0x38D4: goto L38D4;
    case 0x38DB: goto L38DB;
    case 0x38E2: goto L38E2;
    case 0x38E4: goto L38E4;
    case 0x38E5: goto L38E5;
    case 0x38E6: goto L38E6;
    case 0x38EA: goto L38EA;
    case 0x38EC: goto L38EC;
    case 0x38EF: goto L38EF;
    case 0x38F1: goto L38F1;
    case 0x38F5: goto L38F5;
    case 0x38F7: goto L38F7;
    case 0x38F9: goto L38F9;
    case 0x38FC: goto L38FC;
    case 0x3902: goto L3902;
    case 0x3908: goto L3908;
    case 0x390A: goto L390A;
    case 0x390D: goto L390D;
    case 0x390F: goto L390F;
    case 0x3916: goto L3916;
    case 0x391D: goto L391D;
    case 0x391F: goto L391F;
    case 0x3920: goto L3920;
    case 0x3921: goto L3921;
    case 0x3925: goto L3925;
    case 0x392B: goto L392B;
    case 0x3931: goto L3931;
    case 0x3932: goto L3932;
    case 0x3933: goto L3933;
    case 0x3937: goto L3937;
    case 0x393D: goto L393D;
    case 0x3943: goto L3943;
    case 0x3944: goto L3944;
    case 0x3945: goto L3945;
    case 0x3949: goto L3949;
    case 0x394A: goto L394A;
    case 0x394B: goto L394B;
    case 0x394D: goto L394D;
    case 0x394E: goto L394E;
    case 0x3951: goto L3951;
    case 0x3952: goto L3952;
    case 0x3953: goto L3953;
    case 0x3957: goto L3957;
    case 0x3959: goto L3959;
    case 0x395B: goto L395B;
    case 0x395D: goto L395D;
    case 0x395F: goto L395F;
    case 0x3960: goto L3960;
    case 0x3962: goto L3962;
    case 0x3963: goto L3963;
    case 0x3965: goto L3965;
    case 0x3966: goto L3966;
    case 0x3968: goto L3968;
    case 0x396B: goto L396B;
    case 0x396F: goto L396F;
    case 0x3971: goto L3971;
    case 0x3974: goto L3974;
    case 0x3977: goto L3977;
    case 0x397C: goto L397C;
    case 0x3980: goto L3980;
    case 0x3982: goto L3982;
    case 0x3983: goto L3983;
    case 0x3984: goto L3984;
    case 0x3985: goto L3985;
    case 0x3989: goto L3989;
    case 0x398D: goto L398D;
    case 0x3991: goto L3991;
    case 0x3995: goto L3995;
    case 0x3998: goto L3998;
    case 0x399C: goto L399C;
    case 0x39A0: goto L39A0;
    case 0x39A4: goto L39A4;
    case 0x39A7: goto L39A7;
    case 0x39AA: goto L39AA;
    case 0x39AD: goto L39AD;
    case 0x39B0: goto L39B0;
    case 0x39B3: goto L39B3;
    case 0x39B6: goto L39B6;
    case 0x39B9: goto L39B9;
    case 0x39BC: goto L39BC;
    case 0x39BF: goto L39BF;
    case 0x39C2: goto L39C2;
    case 0x39C3: goto L39C3;
    case 0x39C5: goto L39C5;
    case 0x39C6: goto L39C6;
    case 0x39C7: goto L39C7;
    case 0x39C9: goto L39C9;
    case 0x39CA: goto L39CA;
    case 0x39CC: goto L39CC;
    case 0x39CD: goto L39CD;
    case 0x39D1: goto L39D1;
    case 0x39D3: goto L39D3;
    case 0x39D5: goto L39D5;
    case 0x39D7: goto L39D7;
    case 0x39D9: goto L39D9;
    case 0x39DA: goto L39DA;
    case 0x39DC: goto L39DC;
    case 0x39DE: goto L39DE;
    case 0x39E2: goto L39E2;
    case 0x39E3: goto L39E3;
    case 0x39E5: goto L39E5;
    case 0x39E7: goto L39E7;
    case 0x39EB: goto L39EB;
    case 0x39EC: goto L39EC;
    case 0x39EE: goto L39EE;
    case 0x39F0: goto L39F0;
    case 0x39F4: goto L39F4;
    case 0x39F7: goto L39F7;
    case 0x39F9: goto L39F9;
    case 0x39FC: goto L39FC;
    case 0x39FF: goto L39FF;
    case 0x3A02: goto L3A02;
    case 0x3A05: goto L3A05;
    case 0x3A07: goto L3A07;
    case 0x3A08: goto L3A08;
    case 0x3A0A: goto L3A0A;
    case 0x3A0C: goto L3A0C;
    case 0x3A10: goto L3A10;
    case 0x3A11: goto L3A11;
    case 0x3A13: goto L3A13;
    case 0x3A15: goto L3A15;
    case 0x3A19: goto L3A19;
    case 0x3A1A: goto L3A1A;
    case 0x3A1C: goto L3A1C;
    case 0x3A1E: goto L3A1E;
    case 0x3A22: goto L3A22;
    case 0x3A25: goto L3A25;
    case 0x3A27: goto L3A27;
    case 0x3A2A: goto L3A2A;
    case 0x3A2D: goto L3A2D;
    case 0x3A30: goto L3A30;
    case 0x3A32: goto L3A32;
    case 0x3A33: goto L3A33;
    case 0x3A35: goto L3A35;
    case 0x3A37: goto L3A37;
    case 0x3A3B: goto L3A3B;
    case 0x3A3C: goto L3A3C;
    case 0x3A3E: goto L3A3E;
    case 0x3A40: goto L3A40;
    case 0x3A44: goto L3A44;
    case 0x3A45: goto L3A45;
    case 0x3A47: goto L3A47;
    case 0x3A49: goto L3A49;
    case 0x3A4D: goto L3A4D;
    case 0x3A50: goto L3A50;
    case 0x3A53: goto L3A53;
    case 0x3A56: goto L3A56;
    case 0x3A59: goto L3A59;
    case 0x3A5C: goto L3A5C;
    case 0x3A5F: goto L3A5F;
    case 0x3A61: goto L3A61;
    case 0x3A63: goto L3A63;
    case 0x3A64: goto L3A64;
    case 0x3A65: goto L3A65;
    case 0x3A66: goto L3A66;
    case 0x3A68: goto L3A68;
    case 0x3A69: goto L3A69;
    case 0x3A6A: goto L3A6A;
    case 0x3A6E: goto L3A6E;
    case 0x3A6F: goto L3A6F;
    case 0x3A73: goto L3A73;
    case 0x3A77: goto L3A77;
    case 0x3A7B: goto L3A7B;
    case 0x3A7C: goto L3A7C;
    case 0x3A7D: goto L3A7D;
    case 0x3A81: goto L3A81;
    case 0x3A85: goto L3A85;
    case 0x3A89: goto L3A89;
    case 0x3A8B: goto L3A8B;
    case 0x3A8D: goto L3A8D;
    case 0x3A91: goto L3A91;
    case 0x3A93: goto L3A93;
    case 0x3A95: goto L3A95;
    case 0x3A98: goto L3A98;
    case 0x3A9A: goto L3A9A;
    case 0x3A9C: goto L3A9C;
    case 0x3A9E: goto L3A9E;
    case 0x3AA1: goto L3AA1;
    case 0x3AA5: goto L3AA5;
    case 0x3AA9: goto L3AA9;
    case 0x3AAD: goto L3AAD;
    case 0x3AB0: goto L3AB0;
    case 0x3AB1: goto L3AB1;
    case 0x3AB3: goto L3AB3;
    case 0x3AB4: goto L3AB4;
    case 0x3AB5: goto L3AB5;
    case 0x3AB7: goto L3AB7;
    case 0x3AB8: goto L3AB8;
    case 0x3ABA: goto L3ABA;
    case 0x3ABB: goto L3ABB;
    case 0x3ABF: goto L3ABF;
    case 0x3AC1: goto L3AC1;
    case 0x3AC3: goto L3AC3;
    case 0x3AC5: goto L3AC5;
    case 0x3AC7: goto L3AC7;
    case 0x3AC9: goto L3AC9;
    case 0x3ACA: goto L3ACA;
    case 0x3ACC: goto L3ACC;
    case 0x3ACE: goto L3ACE;
    case 0x3AD2: goto L3AD2;
    case 0x3AD3: goto L3AD3;
    case 0x3AD5: goto L3AD5;
    case 0x3AD7: goto L3AD7;
    case 0x3ADB: goto L3ADB;
    case 0x3ADC: goto L3ADC;
    case 0x3ADE: goto L3ADE;
    case 0x3AE0: goto L3AE0;
    case 0x3AE4: goto L3AE4;
    case 0x3AE7: goto L3AE7;
    case 0x3AE9: goto L3AE9;
    case 0x3AEC: goto L3AEC;
    case 0x3AEF: goto L3AEF;
    case 0x3AF2: goto L3AF2;
    case 0x3AF5: goto L3AF5;
    case 0x3AF7: goto L3AF7;
    case 0x3AF8: goto L3AF8;
    case 0x3AFA: goto L3AFA;
    case 0x3AFC: goto L3AFC;
    case 0x3B00: goto L3B00;
    case 0x3B01: goto L3B01;
    case 0x3B03: goto L3B03;
    case 0x3B05: goto L3B05;
    case 0x3B09: goto L3B09;
    case 0x3B0A: goto L3B0A;
    case 0x3B0C: goto L3B0C;
    case 0x3B0E: goto L3B0E;
    case 0x3B12: goto L3B12;
    case 0x3B15: goto L3B15;
    case 0x3B17: goto L3B17;
    case 0x3B1A: goto L3B1A;
    case 0x3B1D: goto L3B1D;
    case 0x3B20: goto L3B20;
    case 0x3B22: goto L3B22;
    case 0x3B23: goto L3B23;
    case 0x3B25: goto L3B25;
    case 0x3B27: goto L3B27;
    case 0x3B2B: goto L3B2B;
    case 0x3B2C: goto L3B2C;
    case 0x3B2E: goto L3B2E;
    case 0x3B30: goto L3B30;
    case 0x3B34: goto L3B34;
    case 0x3B35: goto L3B35;
    case 0x3B37: goto L3B37;
    case 0x3B39: goto L3B39;
    case 0x3B3D: goto L3B3D;
    case 0x3B40: goto L3B40;
    case 0x3B43: goto L3B43;
    case 0x3B46: goto L3B46;
    case 0x3B49: goto L3B49;
    case 0x3B4C: goto L3B4C;
    case 0x3B4F: goto L3B4F;
    case 0x3B51: goto L3B51;
    case 0x3B53: goto L3B53;
    case 0x3B54: goto L3B54;
    case 0x3B55: goto L3B55;
    case 0x3B56: goto L3B56;
    case 0x3B5A: goto L3B5A;
    case 0x3B5B: goto L3B5B;
    case 0x3B5D: goto L3B5D;
    case 0x3B5E: goto L3B5E;
    case 0x3B5F: goto L3B5F;
    case 0x3B63: goto L3B63;
    case 0x3B64: goto L3B64;
    case 0x3B66: goto L3B66;
    case 0x3B68: goto L3B68;
    case 0x3B69: goto L3B69;
    case 0x3B6A: goto L3B6A;
    case 0x3B6E: goto L3B6E;
    case 0x3B6F: goto L3B6F;
    case 0x3B73: goto L3B73;
    case 0x3B74: goto L3B74;
    case 0x3B78: goto L3B78;
    case 0x3B79: goto L3B79;
    case 0x3B7D: goto L3B7D;
    case 0x3B80: goto L3B80;
    case 0x3B81: goto L3B81;
    case 0x3B82: goto L3B82;
    case 0x3B86: goto L3B86;
    case 0x3B8A: goto L3B8A;
    case 0x3B8B: goto L3B8B;
    case 0x3B8D: goto L3B8D;
    case 0x3B8F: goto L3B8F;
    case 0x3B90: goto L3B90;
    case 0x3B92: goto L3B92;
    case 0x3B94: goto L3B94;
    case 0x3B95: goto L3B95;
    case 0x3B97: goto L3B97;
    case 0x3B99: goto L3B99;
    case 0x3B9C: goto L3B9C;
    case 0x3B9D: goto L3B9D;
    case 0x3B9F: goto L3B9F;
    case 0x3BA3: goto L3BA3;
    case 0x3BA7: goto L3BA7;
    case 0x3BAB: goto L3BAB;
    case 0x3BAC: goto L3BAC;
    case 0x3BAD: goto L3BAD;
    case 0x3BB1: goto L3BB1;
    case 0x3BB2: goto L3BB2;
    case 0x3BB4: goto L3BB4;
    case 0x3BB5: goto L3BB5;
    case 0x3BB7: goto L3BB7;
    case 0x3BBC: goto L3BBC;
    case 0x3BC0: goto L3BC0;
    case 0x3BC5: goto L3BC5;
    case 0x3BC9: goto L3BC9;
    case 0x3BCE: goto L3BCE;
    case 0x3BD2: goto L3BD2;
    case 0x3BD3: goto L3BD3;
    case 0x3BD5: goto L3BD5;
    case 0x3BD8: goto L3BD8;
    case 0x3BDC: goto L3BDC;
    case 0x3BE0: goto L3BE0;
    case 0x3BE4: goto L3BE4;
    case 0x3BE8: goto L3BE8;
    case 0x3BE9: goto L3BE9;
    case 0x3BEA: goto L3BEA;
    case 0x3BEE: goto L3BEE;
    case 0x3BEF: goto L3BEF;
    case 0x3BF1: goto L3BF1;
    case 0x3BF2: goto L3BF2;
    case 0x3BF4: goto L3BF4;
    case 0x3BF9: goto L3BF9;
    case 0x3BFD: goto L3BFD;
    case 0x3C02: goto L3C02;
    case 0x3C06: goto L3C06;
    case 0x3C0B: goto L3C0B;
    case 0x3C0F: goto L3C0F;
    case 0x3C10: goto L3C10;
    case 0x3C12: goto L3C12;
    case 0x3C15: goto L3C15;
    case 0x3C19: goto L3C19;
    case 0x3C1D: goto L3C1D;
    case 0x3C21: goto L3C21;
    case 0x3C25: goto L3C25;
    case 0x3C26: goto L3C26;
    case 0x3C27: goto L3C27;
    case 0x3C2B: goto L3C2B;
    case 0x3C2C: goto L3C2C;
    case 0x3C2E: goto L3C2E;
    case 0x3C30: goto L3C30;
    case 0x3C32: goto L3C32;
    case 0x3C33: goto L3C33;
    case 0x3C35: goto L3C35;
    case 0x3C39: goto L3C39;
    case 0x3C3D: goto L3C3D;
    case 0x3C41: goto L3C41;
    case 0x3C45: goto L3C45;
    case 0x3C49: goto L3C49;
    case 0x3C4D: goto L3C4D;
    case 0x3C51: goto L3C51;
    case 0x3C55: goto L3C55;
    case 0x3C59: goto L3C59;
    case 0x3C5D: goto L3C5D;
    case 0x3C61: goto L3C61;
    case 0x3C65: goto L3C65;
    case 0x3C68: goto L3C68;
    case 0x3C69: goto L3C69;
    case 0x3C6C: goto L3C6C;
    case 0x3C6D: goto L3C6D;
    case 0x3C70: goto L3C70;
    case 0x3C71: goto L3C71;
    case 0x3C72: goto L3C72;
    case 0x3C74: goto L3C74;
    case 0x3C75: goto L3C75;
    case 0x3C76: goto L3C76;
    case 0x3C7A: goto L3C7A;
    case 0x3C7B: goto L3C7B;
    case 0x3C7F: goto L3C7F;
    case 0x3C83: goto L3C83;
    case 0x3C87: goto L3C87;
    case 0x3C8B: goto L3C8B;
    case 0x3C8F: goto L3C8F;
    case 0x3C93: goto L3C93;
    case 0x3C97: goto L3C97;
    case 0x3C9B: goto L3C9B;
    case 0x3C9F: goto L3C9F;
    case 0x3CA3: goto L3CA3;
    case 0x3CA7: goto L3CA7;
    case 0x3CAB: goto L3CAB;
    case 0x3CAE: goto L3CAE;
    case 0x3CAF: goto L3CAF;
    case 0x3CB0: goto L3CB0;
    case 0x3CB4: goto L3CB4;
    case 0x3CB5: goto L3CB5;
    case 0x3CB7: goto L3CB7;
    case 0x3CB9: goto L3CB9;
    case 0x3CBB: goto L3CBB;
    case 0x3CBC: goto L3CBC;
    case 0x3CBE: goto L3CBE;
    case 0x3CC2: goto L3CC2;
    case 0x3CC6: goto L3CC6;
    case 0x3CCA: goto L3CCA;
    case 0x3CCE: goto L3CCE;
    case 0x3CD2: goto L3CD2;
    case 0x3CD6: goto L3CD6;
    case 0x3CDA: goto L3CDA;
    case 0x3CDE: goto L3CDE;
    case 0x3CE2: goto L3CE2;
    case 0x3CE6: goto L3CE6;
    case 0x3CEA: goto L3CEA;
    case 0x3CEE: goto L3CEE;
    case 0x3CF1: goto L3CF1;
    case 0x3CF2: goto L3CF2;
    case 0x3CF5: goto L3CF5;
    case 0x3CF6: goto L3CF6;
    case 0x3CF9: goto L3CF9;
    case 0x3CFA: goto L3CFA;
    case 0x3CFB: goto L3CFB;
    case 0x3CFD: goto L3CFD;
    case 0x3CFE: goto L3CFE;
    case 0x3CFF: goto L3CFF;
    case 0x3D03: goto L3D03;
    case 0x3D04: goto L3D04;
    case 0x3D08: goto L3D08;
    case 0x3D0C: goto L3D0C;
    case 0x3D10: goto L3D10;
    case 0x3D14: goto L3D14;
    case 0x3D18: goto L3D18;
    case 0x3D1C: goto L3D1C;
    case 0x3D20: goto L3D20;
    case 0x3D24: goto L3D24;
    case 0x3D28: goto L3D28;
    case 0x3D2C: goto L3D2C;
    case 0x3D30: goto L3D30;
    case 0x3D34: goto L3D34;
    case 0x3D37: goto L3D37;
    case 0x3D38: goto L3D38;
    case 0x3D39: goto L3D39;
    case 0x3D3D: goto L3D3D;
    case 0x3D3E: goto L3D3E;
    case 0x3D40: goto L3D40;
    case 0x3D42: goto L3D42;
    case 0x3D44: goto L3D44;
    case 0x3D45: goto L3D45;
    case 0x3D47: goto L3D47;
    case 0x3D4B: goto L3D4B;
    case 0x3D4F: goto L3D4F;
    case 0x3D53: goto L3D53;
    case 0x3D57: goto L3D57;
    case 0x3D5B: goto L3D5B;
    case 0x3D5F: goto L3D5F;
    case 0x3D63: goto L3D63;
    case 0x3D67: goto L3D67;
    case 0x3D6B: goto L3D6B;
    case 0x3D6F: goto L3D6F;
    case 0x3D73: goto L3D73;
    case 0x3D77: goto L3D77;
    case 0x3D7A: goto L3D7A;
    case 0x3D7B: goto L3D7B;
    case 0x3D7E: goto L3D7E;
    case 0x3D7F: goto L3D7F;
    case 0x3D82: goto L3D82;
    case 0x3D83: goto L3D83;
    case 0x3D84: goto L3D84;
    case 0x3D86: goto L3D86;
    case 0x3D87: goto L3D87;
    case 0x3D88: goto L3D88;
    case 0x3D8C: goto L3D8C;
    case 0x3D8D: goto L3D8D;
    case 0x3D91: goto L3D91;
    case 0x3D95: goto L3D95;
    case 0x3D99: goto L3D99;
    case 0x3D9D: goto L3D9D;
    case 0x3DA1: goto L3DA1;
    case 0x3DA5: goto L3DA5;
    case 0x3DA9: goto L3DA9;
    case 0x3DAD: goto L3DAD;
    case 0x3DB1: goto L3DB1;
    case 0x3DB5: goto L3DB5;
    case 0x3DB9: goto L3DB9;
    case 0x3DBD: goto L3DBD;
    case 0x3DC0: goto L3DC0;
    case 0x3DC1: goto L3DC1;
    case 0x3DC2: goto L3DC2;
    case 0x3DC6: goto L3DC6;
    case 0x3DC8: goto L3DC8;
    case 0x3DCB: goto L3DCB;
    case 0x3DCE: goto L3DCE;
    case 0x3DD0: goto L3DD0;
    case 0x3DD4: goto L3DD4;
    case 0x3DD5: goto L3DD5;
    case 0x3DD9: goto L3DD9;
    case 0x3DDB: goto L3DDB;
    case 0x3DDD: goto L3DDD;
    case 0x3DDE: goto L3DDE;
    case 0x3DE2: goto L3DE2;
    case 0x3DE4: goto L3DE4;
    case 0x3DE6: goto L3DE6;
    case 0x3DE7: goto L3DE7;
    case 0x3DEB: goto L3DEB;
    case 0x3DED: goto L3DED;
    case 0x3DEF: goto L3DEF;
    case 0x3DF2: goto L3DF2;
    case 0x3DF6: goto L3DF6;
    case 0x3DF7: goto L3DF7;
    case 0x3DFA: goto L3DFA;
    case 0x3DFC: goto L3DFC;
    case 0x3DFE: goto L3DFE;
    case 0x3DFF: goto L3DFF;
    case 0x3E02: goto L3E02;
    case 0x3E04: goto L3E04;
    case 0x3E06: goto L3E06;
    case 0x3E07: goto L3E07;
    case 0x3E0A: goto L3E0A;
    case 0x3E0C: goto L3E0C;
    case 0x3E0E: goto L3E0E;
    case 0x3E11: goto L3E11;
    case 0x3E12: goto L3E12;
    case 0x3E14: goto L3E14;
    case 0x3E17: goto L3E17;
    case 0x3E18: goto L3E18;
    case 0x3E1A: goto L3E1A;
    case 0x3E1D: goto L3E1D;
    case 0x3E20: goto L3E20;
    case 0x3E22: goto L3E22;
    case 0x3E24: goto L3E24;
    case 0x3E26: goto L3E26;
    case 0x3E27: goto L3E27;
    case 0x3E29: goto L3E29;
    case 0x3E2B: goto L3E2B;
    case 0x3E2F: goto L3E2F;
    case 0x3E33: goto L3E33;
    case 0x3E36: goto L3E36;
    case 0x3E37: goto L3E37;
    case 0x3E39: goto L3E39;
    case 0x3E3C: goto L3E3C;
    case 0x3E3F: goto L3E3F;
    case 0x3E41: goto L3E41;
    case 0x3E43: goto L3E43;
    case 0x3E45: goto L3E45;
    case 0x3E46: goto L3E46;
    case 0x3E48: goto L3E48;
    case 0x3E4A: goto L3E4A;
    case 0x3E4E: goto L3E4E;
    case 0x3E52: goto L3E52;
    case 0x3E55: goto L3E55;
    case 0x3E56: goto L3E56;
    case 0x3E58: goto L3E58;
    case 0x3E5B: goto L3E5B;
    case 0x3E5E: goto L3E5E;
    case 0x3E60: goto L3E60;
    case 0x3E62: goto L3E62;
    case 0x3E64: goto L3E64;
    case 0x3E65: goto L3E65;
    case 0x3E67: goto L3E67;
    case 0x3E69: goto L3E69;
    case 0x3E6D: goto L3E6D;
    case 0x3E71: goto L3E71;
    case 0x3E72: goto L3E72;
    case 0x3E74: goto L3E74;
    case 0x3E78: goto L3E78;
    case 0x3E79: goto L3E79;
    case 0x3E7B: goto L3E7B;
    case 0x3E7E: goto L3E7E;
    case 0x3E82: goto L3E82;
    case 0x3E84: goto L3E84;
    case 0x3E85: goto L3E85;
    case 0x3E88: goto L3E88;
    case 0x3E8C: goto L3E8C;
    case 0x3E8E: goto L3E8E;
    case 0x3E8F: goto L3E8F;
    case 0x3E92: goto L3E92;
    case 0x3E96: goto L3E96;
    case 0x3E98: goto L3E98;
    case 0x3E99: goto L3E99;
    case 0x3E9C: goto L3E9C;
    case 0x3E9D: goto L3E9D;
    case 0x3EA0: goto L3EA0;
    case 0x3EA4: goto L3EA4;
    case 0x3EA7: goto L3EA7;
    case 0x3EAB: goto L3EAB;
    case 0x3EAE: goto L3EAE;
    case 0x3EB2: goto L3EB2;
    case 0x3EB5: goto L3EB5;
    case 0x3EB9: goto L3EB9;
    case 0x3EBC: goto L3EBC;
    case 0x3EC0: goto L3EC0;
    case 0x3EC3: goto L3EC3;
    case 0x3EC7: goto L3EC7;
    case 0x3EC8: goto L3EC8;
    case 0x3ECA: goto L3ECA;
    case 0x3ECB: goto L3ECB;
    case 0x3ECC: goto L3ECC;
    case 0x3ECD: goto L3ECD;
    case 0x3ED1: goto L3ED1;
    case 0x3ED5: goto L3ED5;
    case 0x3ED9: goto L3ED9;
    case 0x3EDD: goto L3EDD;
    case 0x3EE1: goto L3EE1;
    case 0x3EE2: goto L3EE2;
    case 0x3EE3: goto L3EE3;
    case 0x3EE7: goto L3EE7;
    case 0x3EEB: goto L3EEB;
    case 0x3EEF: goto L3EEF;
    case 0x3EF3: goto L3EF3;
    case 0x3EF7: goto L3EF7;
    case 0x3EFA: goto L3EFA;
    case 0x3EFB: goto L3EFB;
    case 0x3EFC: goto L3EFC;
    case 0x3F00: goto L3F00;
    case 0x3F01: goto L3F01;
    case 0x3F03: goto L3F03;
    case 0x3F04: goto L3F04;
    case 0x3F06: goto L3F06;
    case 0x3F07: goto L3F07;
    case 0x3F0B: goto L3F0B;
    case 0x3F0D: goto L3F0D;
    case 0x3F0F: goto L3F0F;
    case 0x3F11: goto L3F11;
    case 0x3F12: goto L3F12;
    case 0x3F14: goto L3F14;
    case 0x3F15: goto L3F15;
    case 0x3F19: goto L3F19;
    case 0x3F1B: goto L3F1B;
    case 0x3F1D: goto L3F1D;
    case 0x3F1F: goto L3F1F;
    case 0x3F21: goto L3F21;
    case 0x3F23: goto L3F23;
    case 0x3F24: goto L3F24;
    case 0x3F25: goto L3F25;
    case 0x3F29: goto L3F29;
    case 0x3F2A: goto L3F2A;
    case 0x3F2C: goto L3F2C;
    case 0x3F2D: goto L3F2D;
    case 0x3F2F: goto L3F2F;
    case 0x3F30: goto L3F30;
    case 0x3F34: goto L3F34;
    case 0x3F36: goto L3F36;
    case 0x3F38: goto L3F38;
    case 0x3F3A: goto L3F3A;
    case 0x3F3B: goto L3F3B;
    case 0x3F3D: goto L3F3D;
    case 0x3F3E: goto L3F3E;
    case 0x3F42: goto L3F42;
    case 0x3F44: goto L3F44;
    case 0x3F46: goto L3F46;
    case 0x3F48: goto L3F48;
    case 0x3F4A: goto L3F4A;
    case 0x3F4C: goto L3F4C;
    case 0x3F4D: goto L3F4D;
    case 0x3F4E: goto L3F4E;
    case 0x3F52: goto L3F52;
    case 0x3F53: goto L3F53;
    case 0x3F55: goto L3F55;
    case 0x3F56: goto L3F56;
    case 0x3F58: goto L3F58;
    case 0x3F59: goto L3F59;
    case 0x3F5D: goto L3F5D;
    case 0x3F5F: goto L3F5F;
    case 0x3F61: goto L3F61;
    case 0x3F63: goto L3F63;
    case 0x3F64: goto L3F64;
    case 0x3F66: goto L3F66;
    case 0x3F67: goto L3F67;
    case 0x3F6B: goto L3F6B;
    case 0x3F6D: goto L3F6D;
    case 0x3F6F: goto L3F6F;
    case 0x3F71: goto L3F71;
    case 0x3F73: goto L3F73;
    case 0x3F75: goto L3F75;
    case 0x3F76: goto L3F76;
    case 0x3F77: goto L3F77;
    case 0x3F7B: goto L3F7B;
    case 0x3F7C: goto L3F7C;
    case 0x3F7E: goto L3F7E;
    case 0x3F7F: goto L3F7F;
    case 0x3F81: goto L3F81;
    case 0x3F82: goto L3F82;
    case 0x3F86: goto L3F86;
    case 0x3F88: goto L3F88;
    case 0x3F8A: goto L3F8A;
    case 0x3F8C: goto L3F8C;
    case 0x3F8D: goto L3F8D;
    case 0x3F8E: goto L3F8E;
    case 0x3F92: goto L3F92;
    case 0x3F93: goto L3F93;
    case 0x3F95: goto L3F95;
    case 0x3F96: goto L3F96;
    case 0x3F98: goto L3F98;
    case 0x3F99: goto L3F99;
    case 0x3F9D: goto L3F9D;
    case 0x3F9F: goto L3F9F;
    case 0x3FA1: goto L3FA1;
    case 0x3FA3: goto L3FA3;
    case 0x3FA4: goto L3FA4;
    case 0x3FA5: goto L3FA5;
    case 0x3FA9: goto L3FA9;
    case 0x3FAA: goto L3FAA;
    case 0x3FAC: goto L3FAC;
    case 0x3FAD: goto L3FAD;
    case 0x3FAF: goto L3FAF;
    case 0x3FB0: goto L3FB0;
    case 0x3FB4: goto L3FB4;
    case 0x3FB6: goto L3FB6;
    case 0x3FB8: goto L3FB8;
    case 0x3FBA: goto L3FBA;
    case 0x3FBB: goto L3FBB;
    case 0x3FBC: goto L3FBC;
    case 0x3FC0: goto L3FC0;
    case 0x3FC1: goto L3FC1;
    case 0x3FC3: goto L3FC3;
    case 0x3FC4: goto L3FC4;
    case 0x3FC6: goto L3FC6;
    case 0x3FC7: goto L3FC7;
    case 0x3FCB: goto L3FCB;
    case 0x3FCD: goto L3FCD;
    case 0x3FCF: goto L3FCF;
    case 0x3FD1: goto L3FD1;
    case 0x3FD2: goto L3FD2;
    case 0x3FD4: goto L3FD4;
    case 0x3FD5: goto L3FD5;
    case 0x3FD9: goto L3FD9;
    case 0x3FDB: goto L3FDB;
    case 0x3FDD: goto L3FDD;
    case 0x3FDF: goto L3FDF;
    case 0x3FE0: goto L3FE0;
    case 0x3FE2: goto L3FE2;
    case 0x3FE3: goto L3FE3;
    case 0x3FE7: goto L3FE7;
    case 0x3FE9: goto L3FE9;
    case 0x3FEB: goto L3FEB;
    case 0x3FED: goto L3FED;
    case 0x3FEF: goto L3FEF;
    case 0x3FF1: goto L3FF1;
    case 0x3FF2: goto L3FF2;
    case 0x3FF3: goto L3FF3;
    case 0x3FF7: goto L3FF7;
    case 0x3FF8: goto L3FF8;
    case 0x3FFB: goto L3FFB;
    case 0x4002: goto L4002;
    case 0x4004: goto L4004;
    case 0x4006: goto L4006;
    case 0x400B: goto L400B;
    case 0x400D: goto L400D;
    case 0x4010: goto L4010;
    case 0x4015: goto L4015;
    case 0x4017: goto L4017;
    case 0x401C: goto L401C;
    case 0x401E: goto L401E;
    case 0x4021: goto L4021;
    case 0x4026: goto L4026;
    case 0x4028: goto L4028;
    case 0x402D: goto L402D;
    case 0x402F: goto L402F;
    case 0x4032: goto L4032;
    case 0x4037: goto L4037;
    case 0x4039: goto L4039;
    case 0x403E: goto L403E;
    case 0x4040: goto L4040;
    case 0x4043: goto L4043;
    case 0x4048: goto L4048;
    case 0x404A: goto L404A;
    case 0x404D: goto L404D;
    case 0x404E: goto L404E;
    case 0x4051: goto L4051;
    case 0x4053: goto L4053;
    case 0x4055: goto L4055;
    case 0x4056: goto L4056;
    case 0x4057: goto L4057;
    case 0x405B: goto L405B;
    case 0x4060: goto L4060;
    case 0x4065: goto L4065;
    case 0x4069: goto L4069;
    case 0x406D: goto L406D;
    case 0x4070: goto L4070;
    case 0x4072: goto L4072;
    case 0x4077: goto L4077;
    case 0x407B: goto L407B;
    case 0x407E: goto L407E;
    case 0x4082: goto L4082;
    case 0x4084: goto L4084;
    case 0x4087: goto L4087;
    case 0x408B: goto L408B;
    case 0x408E: goto L408E;
    case 0x4092: goto L4092;
    case 0x4094: goto L4094;
    case 0x4097: goto L4097;
    case 0x409A: goto L409A;
    case 0x409C: goto L409C;
    case 0x409E: goto L409E;
    case 0x40A2: goto L40A2;
    case 0x40A6: goto L40A6;
    case 0x40A9: goto L40A9;
    case 0x40AB: goto L40AB;
    case 0x40AD: goto L40AD;
    case 0x40B1: goto L40B1;
    case 0x40B3: goto L40B3;
    case 0x40B6: goto L40B6;
    case 0x40B9: goto L40B9;
    case 0x40BB: goto L40BB;
    case 0x40BE: goto L40BE;
    case 0x40C1: goto L40C1;
    case 0x40C3: goto L40C3;
    case 0x40C6: goto L40C6;
    case 0x40C8: goto L40C8;
    case 0x40CA: goto L40CA;
    case 0x40CC: goto L40CC;
    case 0x40CE: goto L40CE;
    case 0x40D0: goto L40D0;
    case 0x40D2: goto L40D2;
    case 0x40D3: goto L40D3;
    case 0x40D5: goto L40D5;
    case 0x40D8: goto L40D8;
    case 0x40D9: goto L40D9;
    case 0x40DB: goto L40DB;
    case 0x40DE: goto L40DE;
    case 0x40E1: goto L40E1;
    case 0x40E3: goto L40E3;
    case 0x40E5: goto L40E5;
    case 0x40E7: goto L40E7;
    case 0x40E9: goto L40E9;
    case 0x40EB: goto L40EB;
    case 0x40ED: goto L40ED;
    case 0x40EE: goto L40EE;
    case 0x40F0: goto L40F0;
    case 0x40F3: goto L40F3;
    case 0x40F4: goto L40F4;
    case 0x40F5: goto L40F5;
    case 0x40F7: goto L40F7;
    case 0x40F9: goto L40F9;
    case 0x40FC: goto L40FC;
    case 0x40FD: goto L40FD;
    case 0x4101: goto L4101;
    case 0x4105: goto L4105;
    case 0x4109: goto L4109;
    case 0x410B: goto L410B;
    case 0x410E: goto L410E;
    case 0x4111: goto L4111;
    case 0x4113: goto L4113;
    case 0x4116: goto L4116;
    case 0x4119: goto L4119;
    case 0x411C: goto L411C;
    case 0x411E: goto L411E;
    case 0x4120: goto L4120;
    case 0x4122: goto L4122;
    case 0x4124: goto L4124;
    case 0x4126: goto L4126;
    case 0x4128: goto L4128;
    case 0x412A: goto L412A;
    case 0x412B: goto L412B;
    case 0x412D: goto L412D;
    case 0x412F: goto L412F;
    case 0x4130: goto L4130;
    case 0x4132: goto L4132;
    case 0x4135: goto L4135;
    case 0x4138: goto L4138;
    case 0x413A: goto L413A;
    case 0x413C: goto L413C;
    case 0x413E: goto L413E;
    case 0x4140: goto L4140;
    case 0x4142: goto L4142;
    case 0x4144: goto L4144;
    case 0x4145: goto L4145;
    case 0x4147: goto L4147;
    case 0x414A: goto L414A;
    case 0x414B: goto L414B;
    case 0x414C: goto L414C;
    case 0x414E: goto L414E;
    case 0x4150: goto L4150;
    case 0x4153: goto L4153;
    case 0x4154: goto L4154;
    case 0x4158: goto L4158;
    case 0x415C: goto L415C;
    case 0x415F: goto L415F;
    case 0x4163: goto L4163;
    case 0x4164: goto L4164;
    case 0x4169: goto L4169;
    case 0x416E: goto L416E;
    case 0x4172: goto L4172;
    case 0x4176: goto L4176;
    case 0x4179: goto L4179;
    case 0x417B: goto L417B;
    case 0x4180: goto L4180;
    case 0x4184: goto L4184;
    case 0x4187: goto L4187;
    case 0x418B: goto L418B;
    case 0x418D: goto L418D;
    case 0x4190: goto L4190;
    case 0x4194: goto L4194;
    case 0x4197: goto L4197;
    case 0x419B: goto L419B;
    case 0x419D: goto L419D;
    case 0x41A0: goto L41A0;
    case 0x41A3: goto L41A3;
    case 0x41A5: goto L41A5;
    case 0x41A7: goto L41A7;
    case 0x41AB: goto L41AB;
    case 0x41AF: goto L41AF;
    case 0x41B2: goto L41B2;
    case 0x41B4: goto L41B4;
    case 0x41B6: goto L41B6;
    case 0x41BA: goto L41BA;
    case 0x41BC: goto L41BC;
    case 0x41BF: goto L41BF;
    case 0x41C2: goto L41C2;
    case 0x41C4: goto L41C4;
    case 0x41C7: goto L41C7;
    case 0x41CA: goto L41CA;
    case 0x41CC: goto L41CC;
    case 0x41CF: goto L41CF;
    case 0x41D1: goto L41D1;
    case 0x41D3: goto L41D3;
    case 0x41D5: goto L41D5;
    case 0x41D7: goto L41D7;
    case 0x41D9: goto L41D9;
    case 0x41DB: goto L41DB;
    case 0x41DC: goto L41DC;
    case 0x41DE: goto L41DE;
    case 0x41E1: goto L41E1;
    case 0x41E2: goto L41E2;
    case 0x41E4: goto L41E4;
    case 0x41E7: goto L41E7;
    case 0x41EA: goto L41EA;
    case 0x41EC: goto L41EC;
    case 0x41EE: goto L41EE;
    case 0x41F0: goto L41F0;
    case 0x41F2: goto L41F2;
    case 0x41F4: goto L41F4;
    case 0x41F6: goto L41F6;
    case 0x41F7: goto L41F7;
    case 0x41F9: goto L41F9;
    case 0x41FC: goto L41FC;
    case 0x41FD: goto L41FD;
    case 0x41FF: goto L41FF;
    case 0x4201: goto L4201;
    case 0x4202: goto L4202;
    case 0x4204: goto L4204;
    case 0x4207: goto L4207;
    case 0x4208: goto L4208;
    case 0x420C: goto L420C;
    case 0x4210: goto L4210;
    case 0x4214: goto L4214;
    case 0x4216: goto L4216;
    case 0x4219: goto L4219;
    case 0x421C: goto L421C;
    case 0x421F: goto L421F;
    case 0x4221: goto L4221;
    case 0x4224: goto L4224;
    case 0x4227: goto L4227;
    case 0x422A: goto L422A;
    case 0x422C: goto L422C;
    case 0x422E: goto L422E;
    case 0x4230: goto L4230;
    case 0x4232: goto L4232;
    case 0x4234: goto L4234;
    case 0x4236: goto L4236;
    case 0x4238: goto L4238;
    case 0x4239: goto L4239;
    case 0x423B: goto L423B;
    case 0x423D: goto L423D;
    case 0x423E: goto L423E;
    case 0x4240: goto L4240;
    case 0x4243: goto L4243;
    case 0x4246: goto L4246;
    case 0x4248: goto L4248;
    case 0x424A: goto L424A;
    case 0x424C: goto L424C;
    case 0x424E: goto L424E;
    case 0x4250: goto L4250;
    case 0x4252: goto L4252;
    case 0x4253: goto L4253;
    case 0x4255: goto L4255;
    case 0x4258: goto L4258;
    case 0x4259: goto L4259;
    case 0x425B: goto L425B;
    case 0x425D: goto L425D;
    case 0x425E: goto L425E;
    case 0x4260: goto L4260;
    case 0x4263: goto L4263;
    case 0x4264: goto L4264;
    case 0x4268: goto L4268;
    case 0x426C: goto L426C;
    case 0x426F: goto L426F;
    case 0x4273: goto L4273;
    case 0x4274: goto L4274;
    case 0x4279: goto L4279;
    case 0x427E: goto L427E;
    case 0x4282: goto L4282;
    case 0x4286: goto L4286;
    case 0x4289: goto L4289;
    case 0x428B: goto L428B;
    case 0x4290: goto L4290;
    case 0x4294: goto L4294;
    case 0x4297: goto L4297;
    case 0x429B: goto L429B;
    case 0x429D: goto L429D;
    case 0x42A0: goto L42A0;
    case 0x42A4: goto L42A4;
    case 0x42A7: goto L42A7;
    case 0x42AB: goto L42AB;
    case 0x42AD: goto L42AD;
    case 0x42B0: goto L42B0;
    case 0x42B3: goto L42B3;
    case 0x42B5: goto L42B5;
    case 0x42B7: goto L42B7;
    case 0x42BB: goto L42BB;
    case 0x42BF: goto L42BF;
    case 0x42C2: goto L42C2;
    case 0x42C4: goto L42C4;
    case 0x42C6: goto L42C6;
    case 0x42CA: goto L42CA;
    case 0x42CC: goto L42CC;
    case 0x42CF: goto L42CF;
    case 0x42D2: goto L42D2;
    case 0x42D4: goto L42D4;
    case 0x42D7: goto L42D7;
    case 0x42D9: goto L42D9;
    case 0x42DB: goto L42DB;
    case 0x42DE: goto L42DE;
    case 0x42E0: goto L42E0;
    case 0x42E2: goto L42E2;
    case 0x42E4: goto L42E4;
    case 0x42E6: goto L42E6;
    case 0x42E8: goto L42E8;
    case 0x42EA: goto L42EA;
    case 0x42EB: goto L42EB;
    case 0x42ED: goto L42ED;
    case 0x42F0: goto L42F0;
    case 0x42F1: goto L42F1;
    case 0x42F3: goto L42F3;
    case 0x42F6: goto L42F6;
    case 0x42F9: goto L42F9;
    case 0x42FB: goto L42FB;
    case 0x42FD: goto L42FD;
    case 0x42FF: goto L42FF;
    case 0x4301: goto L4301;
    case 0x4303: goto L4303;
    case 0x4305: goto L4305;
    case 0x4306: goto L4306;
    case 0x4308: goto L4308;
    case 0x430B: goto L430B;
    case 0x430C: goto L430C;
    case 0x430E: goto L430E;
    case 0x4310: goto L4310;
    case 0x4311: goto L4311;
    case 0x4313: goto L4313;
    case 0x4316: goto L4316;
    case 0x4317: goto L4317;
    case 0x431B: goto L431B;
    case 0x431F: goto L431F;
    case 0x4323: goto L4323;
    case 0x4325: goto L4325;
    case 0x4328: goto L4328;
    case 0x432A: goto L432A;
    case 0x432D: goto L432D;
    case 0x432F: goto L432F;
    case 0x4332: goto L4332;
    case 0x4335: goto L4335;
    case 0x4338: goto L4338;
    case 0x433A: goto L433A;
    case 0x433C: goto L433C;
    case 0x433E: goto L433E;
    case 0x4340: goto L4340;
    case 0x4342: goto L4342;
    case 0x4344: goto L4344;
    case 0x4346: goto L4346;
    case 0x4347: goto L4347;
    case 0x4349: goto L4349;
    case 0x434B: goto L434B;
    case 0x434C: goto L434C;
    case 0x434E: goto L434E;
    case 0x4351: goto L4351;
    case 0x4354: goto L4354;
    case 0x4356: goto L4356;
    case 0x4358: goto L4358;
    case 0x435A: goto L435A;
    case 0x435C: goto L435C;
    case 0x435E: goto L435E;
    case 0x4360: goto L4360;
    case 0x4361: goto L4361;
    case 0x4363: goto L4363;
    case 0x4366: goto L4366;
    case 0x4367: goto L4367;
    case 0x4369: goto L4369;
    case 0x436B: goto L436B;
    case 0x436C: goto L436C;
    case 0x436E: goto L436E;
    case 0x4371: goto L4371;
    case 0x4372: goto L4372;
    case 0x4376: goto L4376;
    case 0x437A: goto L437A;
    case 0x437D: goto L437D;
    case 0x4381: goto L4381;
    case 0x4382: goto L4382;
    case 0x4387: goto L4387;
    case 0x438C: goto L438C;
    case 0x4390: goto L4390;
    case 0x4394: goto L4394;
    case 0x4397: goto L4397;
    case 0x4399: goto L4399;
    case 0x439E: goto L439E;
    case 0x43A2: goto L43A2;
    case 0x43A5: goto L43A5;
    case 0x43A9: goto L43A9;
    case 0x43AB: goto L43AB;
    case 0x43AE: goto L43AE;
    case 0x43B2: goto L43B2;
    case 0x43B5: goto L43B5;
    case 0x43B9: goto L43B9;
    case 0x43BB: goto L43BB;
    case 0x43BE: goto L43BE;
    case 0x43C1: goto L43C1;
    case 0x43C3: goto L43C3;
    case 0x43C5: goto L43C5;
    case 0x43C9: goto L43C9;
    case 0x43CD: goto L43CD;
    case 0x43D0: goto L43D0;
    case 0x43D2: goto L43D2;
    case 0x43D4: goto L43D4;
    case 0x43D8: goto L43D8;
    case 0x43DA: goto L43DA;
    case 0x43DD: goto L43DD;
    case 0x43E0: goto L43E0;
    case 0x43E2: goto L43E2;
    case 0x43E5: goto L43E5;
    case 0x43E7: goto L43E7;
    case 0x43E9: goto L43E9;
    case 0x43EC: goto L43EC;
    case 0x43EE: goto L43EE;
    case 0x43F0: goto L43F0;
    case 0x43F2: goto L43F2;
    case 0x43F4: goto L43F4;
    case 0x43F6: goto L43F6;
    case 0x43F8: goto L43F8;
    case 0x43F9: goto L43F9;
    case 0x43FB: goto L43FB;
    case 0x43FE: goto L43FE;
    case 0x43FF: goto L43FF;
    case 0x4401: goto L4401;
    case 0x4404: goto L4404;
    case 0x4407: goto L4407;
    case 0x4409: goto L4409;
    case 0x440B: goto L440B;
    case 0x440D: goto L440D;
    case 0x440F: goto L440F;
    case 0x4411: goto L4411;
    case 0x4413: goto L4413;
    case 0x4414: goto L4414;
    case 0x4416: goto L4416;
    case 0x4419: goto L4419;
    case 0x441A: goto L441A;
    case 0x441C: goto L441C;
    case 0x441E: goto L441E;
    case 0x4420: goto L4420;
    case 0x4421: goto L4421;
    case 0x4423: goto L4423;
    case 0x4426: goto L4426;
    case 0x4427: goto L4427;
    case 0x442B: goto L442B;
    case 0x442F: goto L442F;
    case 0x4433: goto L4433;
    case 0x4435: goto L4435;
    case 0x4438: goto L4438;
    case 0x443B: goto L443B;
    case 0x443D: goto L443D;
    case 0x443F: goto L443F;
    case 0x4442: goto L4442;
    case 0x4445: goto L4445;
    case 0x4448: goto L4448;
    case 0x444A: goto L444A;
    case 0x444C: goto L444C;
    case 0x444E: goto L444E;
    case 0x4450: goto L4450;
    case 0x4452: goto L4452;
    case 0x4454: goto L4454;
    case 0x4456: goto L4456;
    case 0x4457: goto L4457;
    case 0x4459: goto L4459;
    case 0x445B: goto L445B;
    case 0x445C: goto L445C;
    case 0x445E: goto L445E;
    case 0x4461: goto L4461;
    case 0x4464: goto L4464;
    case 0x4466: goto L4466;
    case 0x4468: goto L4468;
    case 0x446A: goto L446A;
    case 0x446C: goto L446C;
    case 0x446E: goto L446E;
    case 0x4470: goto L4470;
    case 0x4471: goto L4471;
    case 0x4473: goto L4473;
    case 0x4476: goto L4476;
    case 0x4477: goto L4477;
    case 0x4479: goto L4479;
    case 0x447B: goto L447B;
    case 0x447D: goto L447D;
    case 0x447E: goto L447E;
    case 0x4480: goto L4480;
    case 0x4483: goto L4483;
    case 0x4484: goto L4484;
    case 0x4488: goto L4488;
    case 0x448C: goto L448C;
    case 0x448F: goto L448F;
    case 0x4493: goto L4493;
    case 0x4494: goto L4494;
    case 0x4495: goto L4495;
    case 0x4497: goto L4497;
    case 0x449B: goto L449B;
    case 0x449C: goto L449C;
    case 0x449E: goto L449E;
    case 0x44A0: goto L44A0;
    case 0x44A4: goto L44A4;
    case 0x44A8: goto L44A8;
    case 0x44AA: goto L44AA;
    case 0x44AE: goto L44AE;
    case 0x44B2: goto L44B2;
    case 0x44B4: goto L44B4;
    case 0x44B8: goto L44B8;
    case 0x44BC: goto L44BC;
    case 0x44BD: goto L44BD;
    case 0x44BF: goto L44BF;
    case 0x44C1: goto L44C1;
    case 0x44C5: goto L44C5;
    case 0x44C9: goto L44C9;
    case 0x44CB: goto L44CB;
    case 0x44CF: goto L44CF;
    case 0x44D3: goto L44D3;
    case 0x44D5: goto L44D5;
    case 0x44D9: goto L44D9;
    case 0x44DD: goto L44DD;
    case 0x44DE: goto L44DE;
    case 0x44E0: goto L44E0;
    case 0x44E2: goto L44E2;
    case 0x44E6: goto L44E6;
    case 0x44EA: goto L44EA;
    case 0x44EC: goto L44EC;
    case 0x44F0: goto L44F0;
    case 0x44F4: goto L44F4;
    case 0x44F6: goto L44F6;
    case 0x44FA: goto L44FA;
    case 0x44FE: goto L44FE;
    case 0x4502: goto L4502;
    case 0x4504: goto L4504;
    case 0x4506: goto L4506;
    case 0x4509: goto L4509;
    case 0x450D: goto L450D;
    case 0x4511: goto L4511;
    case 0x4515: goto L4515;
    case 0x4519: goto L4519;
    case 0x451D: goto L451D;
    case 0x4521: goto L4521;
    case 0x4525: goto L4525;
    case 0x4529: goto L4529;
    case 0x452D: goto L452D;
    case 0x4531: goto L4531;
    case 0x4535: goto L4535;
    case 0x4539: goto L4539;
    case 0x453B: goto L453B;
    case 0x453E: goto L453E;
    case 0x4540: goto L4540;
    case 0x4542: goto L4542;
    case 0x4545: goto L4545;
    case 0x4547: goto L4547;
    case 0x4549: goto L4549;
    case 0x454C: goto L454C;
    case 0x454E: goto L454E;
    case 0x4550: goto L4550;
    case 0x4552: goto L4552;
    case 0x4555: goto L4555;
    case 0x4557: goto L4557;
    case 0x4559: goto L4559;
    case 0x455C: goto L455C;
    case 0x4560: goto L4560;
    case 0x4564: goto L4564;
    case 0x4568: goto L4568;
    case 0x456C: goto L456C;
    case 0x4570: goto L4570;
    case 0x4574: goto L4574;
    case 0x4578: goto L4578;
    case 0x457C: goto L457C;
    case 0x4580: goto L4580;
    case 0x4584: goto L4584;
    case 0x4588: goto L4588;
    case 0x458C: goto L458C;
    case 0x458E: goto L458E;
    case 0x4591: goto L4591;
    case 0x4593: goto L4593;
    case 0x4595: goto L4595;
    case 0x4598: goto L4598;
    case 0x459A: goto L459A;
    case 0x459C: goto L459C;
    case 0x459F: goto L459F;
    case 0x45A1: goto L45A1;
    case 0x45A3: goto L45A3;
    case 0x45A5: goto L45A5;
    case 0x45A8: goto L45A8;
    case 0x45AA: goto L45AA;
    case 0x45AC: goto L45AC;
    case 0x45AF: goto L45AF;
    case 0x45B3: goto L45B3;
    case 0x45B7: goto L45B7;
    case 0x45BB: goto L45BB;
    case 0x45BF: goto L45BF;
    case 0x45C3: goto L45C3;
    case 0x45C7: goto L45C7;
    case 0x45CB: goto L45CB;
    case 0x45CF: goto L45CF;
    case 0x45D3: goto L45D3;
    case 0x45D7: goto L45D7;
    case 0x45DB: goto L45DB;
    case 0x45DF: goto L45DF;
    case 0x45E1: goto L45E1;
    case 0x45E4: goto L45E4;
    case 0x45E6: goto L45E6;
    case 0x45E8: goto L45E8;
    case 0x45EB: goto L45EB;
    case 0x45ED: goto L45ED;
    case 0x45EF: goto L45EF;
    case 0x45F2: goto L45F2;
    case 0x45F4: goto L45F4;
    case 0x45F6: goto L45F6;
    case 0x45F8: goto L45F8;
    case 0x45FB: goto L45FB;
    case 0x45FD: goto L45FD;
    case 0x45FF: goto L45FF;
    case 0x4602: goto L4602;
    case 0x4606: goto L4606;
    case 0x460A: goto L460A;
    case 0x460E: goto L460E;
    case 0x4612: goto L4612;
    case 0x4616: goto L4616;
    case 0x461A: goto L461A;
    case 0x461E: goto L461E;
    case 0x4622: goto L4622;
    case 0x4626: goto L4626;
    case 0x462A: goto L462A;
    case 0x462E: goto L462E;
    case 0x4632: goto L4632;
    case 0x4634: goto L4634;
    case 0x4637: goto L4637;
    case 0x4639: goto L4639;
    case 0x463B: goto L463B;
    case 0x463E: goto L463E;
    case 0x4640: goto L4640;
    case 0x4642: goto L4642;
    case 0x4645: goto L4645;
    case 0x4647: goto L4647;
    case 0x4649: goto L4649;
    case 0x464B: goto L464B;
    case 0x464E: goto L464E;
    case 0x4650: goto L4650;
    case 0x4652: goto L4652;
    case 0x4655: goto L4655;
    case 0x4659: goto L4659;
    case 0x465D: goto L465D;
    case 0x4661: goto L4661;
    case 0x4665: goto L4665;
    case 0x4669: goto L4669;
    case 0x466D: goto L466D;
    case 0x4671: goto L4671;
    case 0x4675: goto L4675;
    case 0x4679: goto L4679;
    case 0x467D: goto L467D;
    case 0x4681: goto L4681;
    case 0x4685: goto L4685;
    case 0x4687: goto L4687;
    case 0x468A: goto L468A;
    case 0x468C: goto L468C;
    case 0x468E: goto L468E;
    case 0x4691: goto L4691;
    case 0x4693: goto L4693;
    case 0x4695: goto L4695;
    case 0x4698: goto L4698;
    case 0x469A: goto L469A;
    case 0x469C: goto L469C;
    case 0x469E: goto L469E;
    case 0x46A1: goto L46A1;
    case 0x46A3: goto L46A3;
    case 0x46A5: goto L46A5;
    case 0x46A8: goto L46A8;
    case 0x46AC: goto L46AC;
    case 0x46B0: goto L46B0;
    case 0x46B4: goto L46B4;
    case 0x46B8: goto L46B8;
    case 0x46BC: goto L46BC;
    case 0x46C0: goto L46C0;
    case 0x46C4: goto L46C4;
    case 0x46C8: goto L46C8;
    case 0x46CC: goto L46CC;
    case 0x46D0: goto L46D0;
    case 0x46D4: goto L46D4;
    case 0x46D8: goto L46D8;
    case 0x46DA: goto L46DA;
    case 0x46DD: goto L46DD;
    case 0x46DF: goto L46DF;
    case 0x46E1: goto L46E1;
    case 0x46E4: goto L46E4;
    case 0x46E6: goto L46E6;
    case 0x46E8: goto L46E8;
    case 0x46EB: goto L46EB;
    case 0x46ED: goto L46ED;
    case 0x46EF: goto L46EF;
    case 0x46F1: goto L46F1;
    case 0x46F4: goto L46F4;
    case 0x46F6: goto L46F6;
    case 0x46F8: goto L46F8;
    case 0x46FB: goto L46FB;
    case 0x46FF: goto L46FF;
    case 0x4703: goto L4703;
    case 0x4707: goto L4707;
    case 0x470B: goto L470B;
    case 0x470F: goto L470F;
    case 0x4713: goto L4713;
    case 0x4717: goto L4717;
    case 0x471B: goto L471B;
    case 0x471F: goto L471F;
    case 0x4723: goto L4723;
    case 0x4727: goto L4727;
    case 0x472B: goto L472B;
    case 0x472D: goto L472D;
    case 0x472F: goto L472F;
    case 0x4731: goto L4731;
    case 0x4733: goto L4733;
    case 0x4735: goto L4735;
    case 0x4737: goto L4737;
    case 0x4739: goto L4739;
    case 0x473B: goto L473B;
    case 0x473D: goto L473D;
    case 0x473F: goto L473F;
    case 0x4743: goto L4743;
    case 0x4747: goto L4747;
    case 0x474B: goto L474B;
    case 0x474F: goto L474F;
    case 0x4753: goto L4753;
    case 0x4757: goto L4757;
    case 0x475B: goto L475B;
    case 0x475F: goto L475F;
    case 0x4763: goto L4763;
    case 0x4767: goto L4767;
    case 0x476B: goto L476B;
    case 0x476F: goto L476F;
    case 0x4771: goto L4771;
    case 0x4773: goto L4773;
    case 0x4775: goto L4775;
    case 0x4777: goto L4777;
    case 0x4779: goto L4779;
    case 0x477B: goto L477B;
    case 0x477D: goto L477D;
    case 0x477F: goto L477F;
    case 0x4781: goto L4781;
    case 0x4783: goto L4783;
    case 0x4786: goto L4786;
    case 0x4789: goto L4789;
    case 0x478C: goto L478C;
    case 0x478D: goto L478D;
    case 0x478E: goto L478E;
    case 0x4792: goto L4792;
    case 0x4795: goto L4795;
    case 0x4796: goto L4796;
    case 0x4797: goto L4797;
    case 0x479B: goto L479B;
    case 0x479D: goto L479D;
    case 0x47A1: goto L47A1;
    case 0x47A5: goto L47A5;
    case 0x47A9: goto L47A9;
    case 0x47AD: goto L47AD;
    case 0x47B1: goto L47B1;
    case 0x47B5: goto L47B5;
    case 0x47B9: goto L47B9;
    case 0x47BD: goto L47BD;
    case 0x47C1: goto L47C1;
    case 0x47C5: goto L47C5;
    case 0x47C9: goto L47C9;
    case 0x47CD: goto L47CD;
    case 0x47D0: goto L47D0;
    case 0x47D2: goto L47D2;
    case 0x47D4: goto L47D4;
    case 0x47D8: goto L47D8;
    case 0x47DC: goto L47DC;
    case 0x47E0: goto L47E0;
    case 0x47E4: goto L47E4;
    case 0x47E8: goto L47E8;
    case 0x47EC: goto L47EC;
    case 0x47F0: goto L47F0;
    case 0x47F4: goto L47F4;
    case 0x47F8: goto L47F8;
    case 0x47FC: goto L47FC;
    case 0x4800: goto L4800;
    case 0x4804: goto L4804;
    case 0x4807: goto L4807;
    case 0x4809: goto L4809;
    case 0x480B: goto L480B;
    case 0x480F: goto L480F;
    case 0x4813: goto L4813;
    case 0x4817: goto L4817;
    case 0x481B: goto L481B;
    case 0x481F: goto L481F;
    case 0x4823: goto L4823;
    case 0x4827: goto L4827;
    case 0x482B: goto L482B;
    case 0x482F: goto L482F;
    case 0x4833: goto L4833;
    case 0x4837: goto L4837;
    case 0x483B: goto L483B;
    case 0x483E: goto L483E;
    case 0x4840: goto L4840;
    case 0x4842: goto L4842;
    case 0x4845: goto L4845;
    case 0x4849: goto L4849;
    case 0x484D: goto L484D;
    case 0x4851: goto L4851;
    case 0x4855: goto L4855;
    case 0x4859: goto L4859;
    case 0x485D: goto L485D;
    case 0x4861: goto L4861;
    case 0x4865: goto L4865;
    case 0x4869: goto L4869;
    case 0x486D: goto L486D;
    case 0x4871: goto L4871;
    case 0x4875: goto L4875;
    case 0x4878: goto L4878;
    case 0x487A: goto L487A;
    case 0x487C: goto L487C;
    case 0x487F: goto L487F;
    case 0x4883: goto L4883;
    case 0x4887: goto L4887;
    case 0x488B: goto L488B;
    case 0x488F: goto L488F;
    case 0x4893: goto L4893;
    case 0x4897: goto L4897;
    case 0x489B: goto L489B;
    case 0x489F: goto L489F;
    case 0x48A3: goto L48A3;
    case 0x48A7: goto L48A7;
    case 0x48AB: goto L48AB;
    case 0x48AF: goto L48AF;
    case 0x48B2: goto L48B2;
    case 0x48B4: goto L48B4;
    case 0x48B6: goto L48B6;
    case 0x48B9: goto L48B9;
    case 0x48BD: goto L48BD;
    case 0x48C1: goto L48C1;
    case 0x48C5: goto L48C5;
    case 0x48C9: goto L48C9;
    case 0x48CD: goto L48CD;
    case 0x48D1: goto L48D1;
    case 0x48D5: goto L48D5;
    case 0x48D9: goto L48D9;
    case 0x48DD: goto L48DD;
    case 0x48E1: goto L48E1;
    case 0x48E5: goto L48E5;
    case 0x48E9: goto L48E9;
    case 0x48EC: goto L48EC;
    case 0x48EE: goto L48EE;
    case 0x48F0: goto L48F0;
    case 0x48F3: goto L48F3;
    case 0x48F7: goto L48F7;
    case 0x48FB: goto L48FB;
    case 0x48FF: goto L48FF;
    case 0x4903: goto L4903;
    case 0x4907: goto L4907;
    case 0x490B: goto L490B;
    case 0x490F: goto L490F;
    case 0x4913: goto L4913;
    case 0x4917: goto L4917;
    case 0x491B: goto L491B;
    case 0x491F: goto L491F;
    case 0x4923: goto L4923;
    case 0x4926: goto L4926;
    case 0x4928: goto L4928;
    case 0x492A: goto L492A;
    case 0x492D: goto L492D;
    case 0x4931: goto L4931;
    case 0x4935: goto L4935;
    case 0x4939: goto L4939;
    case 0x493D: goto L493D;
    case 0x4941: goto L4941;
    case 0x4945: goto L4945;
    case 0x4949: goto L4949;
    case 0x494D: goto L494D;
    case 0x4951: goto L4951;
    case 0x4955: goto L4955;
    case 0x4959: goto L4959;
    case 0x495D: goto L495D;
    case 0x4960: goto L4960;
    case 0x4962: goto L4962;
    case 0x4964: goto L4964;
    case 0x4967: goto L4967;
    case 0x4968: goto L4968;
    case 0x496A: goto L496A;
    case 0x496C: goto L496C;
    case 0x496E: goto L496E;
    case 0x496F: goto L496F;
    case 0x4970: goto L4970;
    case 0x4974: goto L4974;
    default: asm_bad_entry("INTERP.ASM", entry);
    }

    /* seg004_27D0  (+27D0)
       Opcode for every unused table slot: `int 2`, then carries on with the next opcode. A
       program that reaches it is malformed; int 2 is NMI, so presumably a debugger catch. */
L27D0: /* _do_int2 */
    /* 27D0  int     2 */
    asm_halt_at(0x06E7, 0x27D0, "int 2h, the debugger break");
L27D2:
    /* 27D2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L27D3:
    /* 27D3  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L27D4:
    /* 27D4  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_27D8  (+27D8)
       Opcode 00h: end of a program or sub-program. Returns from the `call [bx+OPCODE_TABLE]` that
       started it (render_3d, do_sfcal, a sort or call opcode). */
L27D8: /* _do_eof */
    /* 27D8  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_27D9  (+27D9)
       Opcode 08h, operand: point. Projects the point and plots one pixel through seg003
       (_seg003_0) when it is in front of the eye with no clip code (or always, when byte 286Eh
       is set). */
L27D9: /* _do_pntres */
    /* 27D9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L27DA:
    /* 27DA  mov     di,ax */
    DI = AX;
L27DC:
    /* 27DC  add     di,PNT_X */
    DI = (uint16_t)(DI + 0x1620);
L27E0:
    /* 27E0  test    byte ptr ds:[286Eh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x286E) & 0xFF));
L27E5:
    /* 27E5  jne     short L27ED */
    if (!ZF) goto L27ED;
L27E7:
    /* 27E7  test    byte ptr [di+6],0FFh */
    logic8((uint8_t)(rb(pDS, DI + 0x6) & 0xFF));
L27EB:
    /* 27EB  jne     short L2812 */
    if (!ZF) goto L2812;
L27ED: /* L27ED */
    /* 27ED  mov     cx,word ptr [di+4] */
    CX = rw(pDS, DI + 0x4);
L27F0:
    /* 27F0  mov     ax,word ptr [di+2] */
    AX = rw(pDS, DI + 0x2);
L27F3:
    /* 27F3  imul    word ptr ds:[PROJ_Y] */
    imul16(rw(pDS, 0x26B0));
L27F7:
    /* 27F7  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x27F7, 2)) != 0) return c;
L27F9:
    /* 27F9  add     ax,word ptr ds:[CENTRE_Y] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B6));
L27FD:
    /* 27FD  mov     bx,ax */
    BX = AX;
L27FF:
    /* 27FF  mov     ax,word ptr [di] */
    AX = rw(pDS, DI);
L2801:
    /* 2801  imul    word ptr ds:[PROJ_X] */
    imul16(rw(pDS, 0x26B2));
L2805:
    /* 2805  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x2805, 2)) != 0) return c;
L2807:
    /* 2807  add     ax,word ptr ds:[CENTRE_X] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B4));
L280B:
    /* 280B  push    si */
    push16(SI);
L280C:
    /* 280C  call    far ptr _seg003_0 */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x0000), 0x06E7 + PORT_LOAD_SEG, 0x2811)) != 0) return c;
L2811:
    /* 2811  pop     si */
    SI = pop16();
L2812: /* L2812 */
    /* 2812  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2813:
    /* 2813  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2814:
    /* 2814  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2818  (+2818)
       Opcode 96h, operands: two points. Draws a line (seg003 _seg003_8, or _10 after
       clipping). Lines whose codes AND to nonzero are skipped; others are clipped in 3D here, one
       plane at a time, inline (FM has these sections as lclip_top, lclip_bot, lclip_left and
       lclip_right). The two entry points after the first dispatch (28EBh and 28EDh) are the
       divide-overflow recovery addresses stored at ss:[4D5h] while projecting each endpoint (FM
       line_overflow2): they drop the interrupt frame and clip that endpoint. The clipped endpoint
       is written to a scratch point at 15C9h or 15D1h. */
L2818: /* _do_lnres */
    /* 2818  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2819:
    /* 2819  mov     di,ax */
    DI = AX;
L281B:
    /* 281B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L281C:
    /* 281C  mov     bx,ax */
    BX = AX;
L281E:
    /* 281E  mov     al,byte ptr [di+PNT_CODES] */
    AL = rb(pDS, DI + 0x1626);
L2822:
    /* 2822  test    byte ptr [bx+PNT_CODES],al */
    logic8((uint8_t)(rb(pDS, BX + 0x1626) & AL));
L2826:
    /* 2826  jne     short L287F */
    if (!ZF) goto L287F;
L2828:
    /* 2828  push    si */
    push16(SI);
L2829:
    /* 2829  or      al,byte ptr [bx+PNT_CODES] */
    AL = logic8((uint8_t)(AL | rb(pDS, BX + 0x1626)));
L282D:
    /* 282D  jne     short L2885 */
    if (!ZF) goto L2885;
L282F:
    /* 282F  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L2833:
    /* 2833  mov     ax,word ptr [di+PNT_X] */
    AX = rw(pDS, DI + 0x1620);
L2837:
    /* 2837  imul    word ptr ds:[PROJ_X] */
    imul16(rw(pDS, 0x26B2));
L283B:
    /* 283B  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x283B, 2)) != 0) return c;
L283D:
    /* 283D  add     ax,word ptr ds:[CENTRE_X] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B4));
L2841:
    /* 2841  mov     cx,ax */
    CX = AX;
L2843:
    /* 2843  mov     ax,word ptr [di+PNT_Y] */
    AX = rw(pDS, DI + 0x1622);
L2847:
    /* 2847  imul    word ptr ds:[PROJ_Y] */
    imul16(rw(pDS, 0x26B0));
L284B:
    /* 284B  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x284B, 2)) != 0) return c;
L284D:
    /* 284D  add     ax,word ptr ds:[CENTRE_Y] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B6));
L2851:
    /* 2851  mov     si,ax */
    SI = AX;
L2853:
    /* 2853  mov     di,bx */
    DI = BX;
L2855:
    /* 2855  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L2859:
    /* 2859  mov     ax,word ptr [di+PNT_Y] */
    AX = rw(pDS, DI + 0x1622);
L285D:
    /* 285D  imul    word ptr ds:[PROJ_Y] */
    imul16(rw(pDS, 0x26B0));
L2861:
    /* 2861  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x2861, 2)) != 0) return c;
L2863:
    /* 2863  add     ax,word ptr ds:[CENTRE_Y] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B6));
L2867:
    /* 2867  mov     bx,ax */
    BX = AX;
L2869:
    /* 2869  mov     ax,word ptr [di+PNT_X] */
    AX = rw(pDS, DI + 0x1620);
L286D:
    /* 286D  imul    word ptr ds:[PROJ_X] */
    imul16(rw(pDS, 0x26B2));
L2871:
    /* 2871  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x2871, 2)) != 0) return c;
L2873:
    /* 2873  add     ax,word ptr ds:[CENTRE_X] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B4));
L2877:
    /* 2877  mov     dx,si */
    DX = SI;
L2879:
    /* 2879  call    far ptr _seg003_8 */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x0008), 0x06E7 + PORT_LOAD_SEG, 0x287E)) != 0) return c;
L287E: /* L287E */
    /* 287E  pop     si */
    SI = pop16();
L287F: /* L287F */
    /* 287F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2880:
    /* 2880  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2881:
    /* 2881  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L2885: /* L2885 */
    /* 2885  add     bx,PNT_X */
    BX = (uint16_t)(BX + 0x1620);
L2889:
    /* 2889  add     di,PNT_X */
    DI = (uint16_t)(DI + 0x1620);
L288D:
    /* 288D  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L288F:
    /* 288F  js      short L2909 */
    if (SF) goto L2909;
L2891: /* L2891 */
    /* 2891  mov     word ptr ss:[4D5h],28EBh */
    ww(pSS, 0x4D5, 0x28EB);
L2898:
    /* 2898  mov     bp,word ptr [bx+4] */
    BP = rw(pDS, BX + 0x4);
L289B:
    /* 289B  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L289D:
    /* 289D  imul    word ptr ds:[PROJ_X] */
    imul16(rw(pDS, 0x26B2));
L28A1:
    /* 28A1  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x28A1, 2)) != 0) return c;
L28A3:
    /* 28A3  add     ax,word ptr ds:[CENTRE_X] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B4));
L28A7:
    /* 28A7  mov     cx,ax */
    CX = AX;
L28A9:
    /* 28A9  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L28AC:
    /* 28AC  imul    word ptr ds:[PROJ_Y] */
    imul16(rw(pDS, 0x26B0));
L28B0:
    /* 28B0  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x28B0, 2)) != 0) return c;
L28B2:
    /* 28B2  add     ax,word ptr ds:[CENTRE_Y] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B6));
L28B6:
    /* 28B6  mov     si,ax */
    SI = AX;
L28B8:
    /* 28B8  mov     word ptr ss:[4D5h],28EDh */
    ww(pSS, 0x4D5, 0x28ED);
L28BF:
    /* 28BF  mov     bp,word ptr [di+4] */
    BP = rw(pDS, DI + 0x4);
L28C2:
    /* 28C2  mov     ax,word ptr [di+2] */
    AX = rw(pDS, DI + 0x2);
L28C5:
    /* 28C5  imul    word ptr ds:[PROJ_Y] */
    imul16(rw(pDS, 0x26B0));
L28C9:
    /* 28C9  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x28C9, 2)) != 0) return c;
L28CB:
    /* 28CB  add     ax,word ptr ds:[CENTRE_Y] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B6));
L28CF:
    /* 28CF  mov     bx,ax */
    BX = AX;
L28D1:
    /* 28D1  mov     ax,word ptr [di] */
    AX = rw(pDS, DI);
L28D3:
    /* 28D3  imul    word ptr ds:[PROJ_X] */
    imul16(rw(pDS, 0x26B2));
L28D7:
    /* 28D7  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x28D7, 2)) != 0) return c;
L28D9:
    /* 28D9  add     ax,word ptr ds:[CENTRE_X] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B4));
L28DD:
    /* 28DD  mov     dx,si */
    DX = SI;
L28DF:
    /* 28DF  call    far ptr _seg003_10 */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x0010), 0x06E7 + PORT_LOAD_SEG, 0x28E4)) != 0) return c;
L28E4:
    /* 28E4  pop     si */
    SI = pop16();
L28E5:
    /* 28E5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L28E6:
    /* 28E6  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L28E7:
    /* 28E7  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L28EB:
    /* 28EB  xchg    bx,di */
    { uint16_t t_ = DI;
    DI = BX;
    BX = t_; }
L28ED:
    /* 28ED  sti */
    ;
L28EE:
    /* 28EE  add     sp,6 */
    SP = (uint16_t)(SP + 0x6);
L28F1:
    /* 28F1  mov     al,byte ptr [di+6] */
    AL = rb(pDS, DI + 0x6);
L28F4:
    /* 28F4  test    al,1 */
    logic8((uint8_t)(AL & 0x1));
L28F6:
    /* 28F6  je      short L28FB */
    if (ZF) goto L28FB;
L28F8:
    /* 28F8  jmp     L2A13 */
    goto L2A13;
L28FB: /* L28FB */
    /* 28FB  test    al,2 */
    logic8((uint8_t)(AL & 0x2));
L28FD:
    /* 28FD  je      short L2902 */
    if (ZF) goto L2902;
L28FF:
    /* 28FF  jmp     L2A8D */
    goto L2A8D;
L2902: /* L2902 */
    /* 2902  test    al,4 */
    logic8((uint8_t)(AL & 0x4));
L2904:
    /* 2904  jne     short L2919 */
    if (!ZF) goto L2919;
L2906:
    /* 2906  jmp     L2998 */
    goto L2998;
L2909: /* L2909 */
    /* 2909  mov     al,byte ptr [di+6] */
    AL = rb(pDS, DI + 0x6);
L290C:
    /* 290C  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L290E:
    /* 290E  js      short L2915 */
    if (SF) goto L2915;
L2910:
    /* 2910  xchg    di,bx */
    { uint16_t t_ = BX;
    BX = DI;
    DI = t_; }
L2912:
    /* 2912  mov     al,byte ptr [di+6] */
    AL = rb(pDS, DI + 0x6);
L2915: /* L2915 */
    /* 2915  test    al,4 */
    logic8((uint8_t)(AL & 0x4));
L2917:
    /* 2917  je      short L2998 */
    if (ZF) goto L2998;
L2919: /* L2919 */
    /* 2919  mov     word ptr ss:[4D5h],offset _overflow_handler_reg */
    ww(pSS, 0x4D5, 0x5BD1);
L2920:
    /* 2920  mov     dx,word ptr [bx+4] */
    DX = rw(pDS, BX + 0x4);
L2923:
    /* 2923  sub     dx,word ptr [bx+2] */
    DX = (uint16_t)(DX - rw(pDS, BX + 0x2));
L2926:
    /* 2926  mov     cx,word ptr [di+2] */
    CX = rw(pDS, DI + 0x2);
L2929:
    /* 2929  sub     cx,word ptr [bx+2] */
    CX = (uint16_t)(CX - rw(pDS, BX + 0x2));
L292C:
    /* 292C  sub     cx,word ptr [di+4] */
    CX = (uint16_t)(CX - rw(pDS, DI + 0x4));
L292F:
    /* 292F  add     cx,word ptr [bx+4] */
    CX = (uint16_t)(CX + rw(pDS, BX + 0x4));
L2932:
    /* 2932  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L2934:
    /* 2934  sar     dx,1 */
    DX = sar16(DX, 1);
L2936:
    /* 2936  rcr     ax,1 */
    AX = rcr16(AX, 1);
L2938:
    /* 2938  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x2938, 2)) != 0) return c;
L293A:
    /* 293A  mov     bp,ax */
    BP = AX;
L293C:
    /* 293C  mov     ax,word ptr [di+2] */
    AX = rw(pDS, DI + 0x2);
L293F:
    /* 293F  sub     ax,word ptr [bx+2] */
    AX = (uint16_t)(AX - rw(pDS, BX + 0x2));
L2942:
    /* 2942  imul    bp */
    imul16(BP);
L2944:
    /* 2944  shl     ax,1 */
    AX = shl16(AX, 1);
L2946:
    /* 2946  rcl     dx,1 */
    DX = rcl16(DX, 1);
L2948:
    /* 2948  shl     ax,1 */
    AX = shl16(AX, 1);
L294A:
    /* 294A  adc     dx,0 */
    DX = (uint16_t)(DX + 0x0 + CF);
L294D:
    /* 294D  add     dx,word ptr [bx+2] */
    DX = (uint16_t)(DX + rw(pDS, BX + 0x2));
L2950:
    /* 2950  or      dx,dx */
    DX = logic16((uint16_t)(DX | DX));
L2952:
    /* 2952  js      short L2998 */
    if (SF) goto L2998;
L2954:
    /* 2954  mov     cx,dx */
    CX = DX;
L2956:
    /* 2956  mov     ax,word ptr [di] */
    AX = rw(pDS, DI);
L2958:
    /* 2958  sub     ax,word ptr [bx] */
    AX = (uint16_t)(AX - rw(pDS, BX));
L295A:
    /* 295A  imul    bp */
    imul16(BP);
L295C:
    /* 295C  shl     ax,1 */
    AX = shl16(AX, 1);
L295E:
    /* 295E  rcl     dx,1 */
    DX = rcl16(DX, 1);
L2960:
    /* 2960  shl     ax,1 */
    AX = shl16(AX, 1);
L2962:
    /* 2962  adc     dx,0 */
    DX = (uint16_t)(DX + 0x0 + CF);
L2965:
    /* 2965  add     dx,word ptr [bx] */
    DX = (uint16_t)(DX + rw(pDS, BX));
L2967:
    /* 2967  mov     di,15D1h */
    DI = 0x15D1;
L296A:
    /* 296A  cmp     di,bx */
    sub16(DI, BX, 0);
L296C:
    /* 296C  jne     short L2971 */
    if (!ZF) goto L2971;
L296E:
    /* 296E  mov     si,15C9h */
    SI = 0x15C9;
L2971: /* L2971 */
    /* 2971  mov     word ptr [di],dx */
    ww(pDS, DI, DX);
L2973:
    /* 2973  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L2976:
    /* 2976  mov     word ptr [di+4],cx */
    ww(pDS, DI + 0x4, CX);
L2979:
    /* 2979  jcxz    L297D */
    if (!CX) goto L297D;
L297B:
    /* 297B  jmp     short L2980 */
    goto L2980;
L297D: /* L297D */
    /* 297D  jmp     L287E */
    goto L287E;
L2980: /* L2980 */
    /* 2980  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L2982:
    /* 2982  cmp     dx,cx */
    sub16(DX, CX, 0);
L2984:
    /* 2984  jle     short L298A */
    if (ZF || SF != OF) goto L298A;
L2986:
    /* 2986  inc     al */
    AL = (uint8_t)(AL + 1);
L2988:
    /* 2988  inc     al */
    AL = (uint8_t)(AL + 1);
L298A: /* L298A */
    /* 298A  neg     cx */
    CX = (uint16_t)-CX;
L298C:
    /* 298C  cmp     dx,cx */
    sub16(DX, CX, 0);
L298E:
    /* 298E  jge     short L2992 */
    if (SF == OF) goto L2992;
L2990:
    /* 2990  inc     al */
    AL = (uint8_t)(AL + 1);
L2992: /* L2992 */
    /* 2992  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L2995:
    /* 2995  jmp     L2891 */
    goto L2891;
L2998: /* L2998 */
    /* 2998  mov     word ptr ss:[4D5h],offset _overflow_handler_reg */
    ww(pSS, 0x4D5, 0x5BD1);
L299F:
    /* 299F  mov     dx,word ptr [bx+4] */
    DX = rw(pDS, BX + 0x4);
L29A2:
    /* 29A2  add     dx,word ptr [bx+2] */
    DX = (uint16_t)(DX + rw(pDS, BX + 0x2));
L29A5:
    /* 29A5  mov     cx,word ptr [bx+2] */
    CX = rw(pDS, BX + 0x2);
L29A8:
    /* 29A8  sub     cx,word ptr [di+2] */
    CX = (uint16_t)(CX - rw(pDS, DI + 0x2));
L29AB:
    /* 29AB  sub     cx,word ptr [di+4] */
    CX = (uint16_t)(CX - rw(pDS, DI + 0x4));
L29AE:
    /* 29AE  add     cx,word ptr [bx+4] */
    CX = (uint16_t)(CX + rw(pDS, BX + 0x4));
L29B1:
    /* 29B1  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L29B3:
    /* 29B3  sar     dx,1 */
    DX = sar16(DX, 1);
L29B5:
    /* 29B5  rcr     ax,1 */
    AX = rcr16(AX, 1);
L29B7:
    /* 29B7  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x29B7, 2)) != 0) return c;
L29B9:
    /* 29B9  mov     bp,ax */
    BP = AX;
L29BB:
    /* 29BB  mov     ax,word ptr [di+2] */
    AX = rw(pDS, DI + 0x2);
L29BE:
    /* 29BE  sub     ax,word ptr [bx+2] */
    AX = (uint16_t)(AX - rw(pDS, BX + 0x2));
L29C1:
    /* 29C1  imul    bp */
    imul16(BP);
L29C3:
    /* 29C3  shl     ax,1 */
    AX = shl16(AX, 1);
L29C5:
    /* 29C5  rcl     dx,1 */
    DX = rcl16(DX, 1);
L29C7:
    /* 29C7  shl     ax,1 */
    AX = shl16(AX, 1);
L29C9:
    /* 29C9  adc     dx,0 */
    DX = (uint16_t)(DX + 0x0 + CF);
L29CC:
    /* 29CC  add     dx,word ptr [bx+2] */
    DX = (uint16_t)(DX + rw(pDS, BX + 0x2));
L29CF:
    /* 29CF  mov     cx,dx */
    CX = DX;
L29D1:
    /* 29D1  mov     ax,word ptr [di] */
    AX = rw(pDS, DI);
L29D3:
    /* 29D3  sub     ax,word ptr [bx] */
    AX = (uint16_t)(AX - rw(pDS, BX));
L29D5:
    /* 29D5  imul    bp */
    imul16(BP);
L29D7:
    /* 29D7  shl     ax,1 */
    AX = shl16(AX, 1);
L29D9:
    /* 29D9  rcl     dx,1 */
    DX = rcl16(DX, 1);
L29DB:
    /* 29DB  shl     ax,1 */
    AX = shl16(AX, 1);
L29DD:
    /* 29DD  adc     dx,0 */
    DX = (uint16_t)(DX + 0x0 + CF);
L29E0:
    /* 29E0  add     dx,word ptr [bx] */
    DX = (uint16_t)(DX + rw(pDS, BX));
L29E2:
    /* 29E2  mov     di,15D1h */
    DI = 0x15D1;
L29E5:
    /* 29E5  cmp     di,bx */
    sub16(DI, BX, 0);
L29E7:
    /* 29E7  jne     short L29EC */
    if (!ZF) goto L29EC;
L29E9:
    /* 29E9  mov     si,15C9h */
    SI = 0x15C9;
L29EC: /* L29EC */
    /* 29EC  mov     word ptr [di],dx */
    ww(pDS, DI, DX);
L29EE:
    /* 29EE  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L29F1:
    /* 29F1  jcxz    L29F5 */
    if (!CX) goto L29F5;
L29F3:
    /* 29F3  jmp     short L29F8 */
    goto L29F8;
L29F5: /* L29F5 */
    /* 29F5  jmp     L287E */
    goto L287E;
L29F8: /* L29F8 */
    /* 29F8  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L29FA:
    /* 29FA  cmp     dx,cx */
    sub16(DX, CX, 0);
L29FC:
    /* 29FC  jge     short L2A00 */
    if (SF == OF) goto L2A00;
L29FE:
    /* 29FE  inc     al */
    AL = (uint8_t)(AL + 1);
L2A00: /* L2A00 */
    /* 2A00  neg     cx */
    CX = (uint16_t)-CX;
L2A02:
    /* 2A02  cmp     dx,cx */
    sub16(DX, CX, 0);
L2A04:
    /* 2A04  jle     short L2A0A */
    if (ZF || SF != OF) goto L2A0A;
L2A06:
    /* 2A06  inc     al */
    AL = (uint8_t)(AL + 1);
L2A08:
    /* 2A08  inc     al */
    AL = (uint8_t)(AL + 1);
L2A0A: /* L2A0A */
    /* 2A0A  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L2A0D:
    /* 2A0D  mov     word ptr [di+4],cx */
    ww(pDS, DI + 0x4, CX);
L2A10:
    /* 2A10  jmp     L2891 */
    goto L2891;
L2A13: /* L2A13 */
    /* 2A13  mov     word ptr ss:[4D5h],offset _overflow_handler_reg */
    ww(pSS, 0x4D5, 0x5BD1);
L2A1A:
    /* 2A1A  mov     dx,word ptr [bx+4] */
    DX = rw(pDS, BX + 0x4);
L2A1D:
    /* 2A1D  add     dx,word ptr [bx] */
    DX = (uint16_t)(DX + rw(pDS, BX));
L2A1F:
    /* 2A1F  mov     cx,word ptr [bx] */
    CX = rw(pDS, BX);
L2A21:
    /* 2A21  sub     cx,word ptr [di] */
    CX = (uint16_t)(CX - rw(pDS, DI));
L2A23:
    /* 2A23  sub     cx,word ptr [di+4] */
    CX = (uint16_t)(CX - rw(pDS, DI + 0x4));
L2A26:
    /* 2A26  add     cx,word ptr [bx+4] */
    CX = (uint16_t)(CX + rw(pDS, BX + 0x4));
L2A29:
    /* 2A29  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L2A2B:
    /* 2A2B  sar     dx,1 */
    DX = sar16(DX, 1);
L2A2D:
    /* 2A2D  rcr     ax,1 */
    AX = rcr16(AX, 1);
L2A2F:
    /* 2A2F  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x2A2F, 2)) != 0) return c;
L2A31:
    /* 2A31  mov     bp,ax */
    BP = AX;
L2A33:
    /* 2A33  mov     ax,word ptr [di] */
    AX = rw(pDS, DI);
L2A35:
    /* 2A35  sub     ax,word ptr [bx] */
    AX = (uint16_t)(AX - rw(pDS, BX));
L2A37:
    /* 2A37  imul    bp */
    imul16(BP);
L2A39:
    /* 2A39  shl     ax,1 */
    AX = shl16(AX, 1);
L2A3B:
    /* 2A3B  rcl     dx,1 */
    DX = rcl16(DX, 1);
L2A3D:
    /* 2A3D  shl     ax,1 */
    AX = shl16(AX, 1);
L2A3F:
    /* 2A3F  adc     dx,0 */
    DX = (uint16_t)(DX + 0x0 + CF);
L2A42:
    /* 2A42  add     dx,word ptr [bx] */
    DX = (uint16_t)(DX + rw(pDS, BX));
L2A44:
    /* 2A44  or      dx,dx */
    DX = logic16((uint16_t)(DX | DX));
L2A46:
    /* 2A46  jns     short L2A8D */
    if (!SF) goto L2A8D;
L2A48:
    /* 2A48  mov     cx,dx */
    CX = DX;
L2A4A:
    /* 2A4A  mov     ax,word ptr [di+2] */
    AX = rw(pDS, DI + 0x2);
L2A4D:
    /* 2A4D  sub     ax,word ptr [bx+2] */
    AX = (uint16_t)(AX - rw(pDS, BX + 0x2));
L2A50:
    /* 2A50  imul    bp */
    imul16(BP);
L2A52:
    /* 2A52  shl     ax,1 */
    AX = shl16(AX, 1);
L2A54:
    /* 2A54  rcl     dx,1 */
    DX = rcl16(DX, 1);
L2A56:
    /* 2A56  shl     ax,1 */
    AX = shl16(AX, 1);
L2A58:
    /* 2A58  adc     dx,0 */
    DX = (uint16_t)(DX + 0x0 + CF);
L2A5B:
    /* 2A5B  add     dx,word ptr [bx+2] */
    DX = (uint16_t)(DX + rw(pDS, BX + 0x2));
L2A5E:
    /* 2A5E  mov     di,15D1h */
    DI = 0x15D1;
L2A61:
    /* 2A61  cmp     di,bx */
    sub16(DI, BX, 0);
L2A63:
    /* 2A63  jne     short L2A68 */
    if (!ZF) goto L2A68;
L2A65:
    /* 2A65  mov     si,15C9h */
    SI = 0x15C9;
L2A68: /* L2A68 */
    /* 2A68  mov     word ptr [di],cx */
    ww(pDS, DI, CX);
L2A6A:
    /* 2A6A  mov     word ptr [di+2],dx */
    ww(pDS, DI + 0x2, DX);
L2A6D:
    /* 2A6D  jcxz    L2A71 */
    if (!CX) goto L2A71;
L2A6F:
    /* 2A6F  jmp     short L2A74 */
    goto L2A74;
L2A71: /* L2A71 */
    /* 2A71  jmp     L287E */
    goto L287E;
L2A74: /* L2A74 */
    /* 2A74  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L2A76:
    /* 2A76  cmp     dx,cx */
    sub16(DX, CX, 0);
L2A78:
    /* 2A78  jge     short L2A7C */
    if (SF == OF) goto L2A7C;
L2A7A:
    /* 2A7A  or      al,8 */
    AL = (uint8_t)(AL | 0x8);
L2A7C: /* L2A7C */
    /* 2A7C  neg     cx */
    CX = (uint16_t)-CX;
L2A7E:
    /* 2A7E  cmp     dx,cx */
    sub16(DX, CX, 0);
L2A80:
    /* 2A80  jge     short L2A84 */
    if (SF == OF) goto L2A84;
L2A82:
    /* 2A82  or      al,4 */
    AL = (uint8_t)(AL | 0x4);
L2A84: /* L2A84 */
    /* 2A84  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L2A87:
    /* 2A87  mov     word ptr [di+4],cx */
    ww(pDS, DI + 0x4, CX);
L2A8A:
    /* 2A8A  jmp     L2891 */
    goto L2891;
L2A8D: /* L2A8D */
    /* 2A8D  mov     word ptr ss:[4D5h],offset _overflow_handler_reg */
    ww(pSS, 0x4D5, 0x5BD1);
L2A94:
    /* 2A94  mov     dx,word ptr [bx] */
    DX = rw(pDS, BX);
L2A96:
    /* 2A96  sub     dx,word ptr [bx+4] */
    DX = (uint16_t)(DX - rw(pDS, BX + 0x4));
L2A99:
    /* 2A99  mov     cx,word ptr [di+4] */
    CX = rw(pDS, DI + 0x4);
L2A9C:
    /* 2A9C  sub     cx,word ptr [bx+4] */
    CX = (uint16_t)(CX - rw(pDS, BX + 0x4));
L2A9F:
    /* 2A9F  add     cx,word ptr [bx] */
    CX = (uint16_t)(CX + rw(pDS, BX));
L2AA1:
    /* 2AA1  sub     cx,word ptr [di] */
    CX = (uint16_t)(CX - rw(pDS, DI));
L2AA3:
    /* 2AA3  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L2AA5:
    /* 2AA5  sar     dx,1 */
    DX = sar16(DX, 1);
L2AA7:
    /* 2AA7  rcr     ax,1 */
    AX = rcr16(AX, 1);
L2AA9:
    /* 2AA9  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x2AA9, 2)) != 0) return c;
L2AAB:
    /* 2AAB  mov     bp,ax */
    BP = AX;
L2AAD:
    /* 2AAD  mov     ax,word ptr [di+2] */
    AX = rw(pDS, DI + 0x2);
L2AB0:
    /* 2AB0  sub     ax,word ptr [bx+2] */
    AX = (uint16_t)(AX - rw(pDS, BX + 0x2));
L2AB3:
    /* 2AB3  imul    bp */
    imul16(BP);
L2AB5:
    /* 2AB5  shl     ax,1 */
    AX = shl16(AX, 1);
L2AB7:
    /* 2AB7  rcl     dx,1 */
    DX = rcl16(DX, 1);
L2AB9:
    /* 2AB9  shl     ax,1 */
    AX = shl16(AX, 1);
L2ABB:
    /* 2ABB  adc     dx,0 */
    DX = (uint16_t)(DX + 0x0 + CF);
L2ABE:
    /* 2ABE  add     dx,word ptr [bx+2] */
    DX = (uint16_t)(DX + rw(pDS, BX + 0x2));
L2AC1:
    /* 2AC1  mov     cx,dx */
    CX = DX;
L2AC3:
    /* 2AC3  mov     ax,word ptr [di] */
    AX = rw(pDS, DI);
L2AC5:
    /* 2AC5  sub     ax,word ptr [bx] */
    AX = (uint16_t)(AX - rw(pDS, BX));
L2AC7:
    /* 2AC7  imul    bp */
    imul16(BP);
L2AC9:
    /* 2AC9  shl     ax,1 */
    AX = shl16(AX, 1);
L2ACB:
    /* 2ACB  rcl     dx,1 */
    DX = rcl16(DX, 1);
L2ACD:
    /* 2ACD  shl     ax,1 */
    AX = shl16(AX, 1);
L2ACF:
    /* 2ACF  adc     dx,0 */
    DX = (uint16_t)(DX + 0x0 + CF);
L2AD2:
    /* 2AD2  add     dx,word ptr [bx] */
    DX = (uint16_t)(DX + rw(pDS, BX));
L2AD4:
    /* 2AD4  mov     di,15D1h */
    DI = 0x15D1;
L2AD7:
    /* 2AD7  cmp     di,bx */
    sub16(DI, BX, 0);
L2AD9:
    /* 2AD9  jne     short L2ADE */
    if (!ZF) goto L2ADE;
L2ADB:
    /* 2ADB  mov     si,15C9h */
    SI = 0x15C9;
L2ADE: /* L2ADE */
    /* 2ADE  mov     word ptr [di],dx */
    ww(pDS, DI, DX);
L2AE0:
    /* 2AE0  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L2AE3:
    /* 2AE3  mov     word ptr [di+4],dx */
    ww(pDS, DI + 0x4, DX);
L2AE6:
    /* 2AE6  jcxz    L2AEA */
    if (!CX) goto L2AEA;
L2AE8:
    /* 2AE8  jmp     short L2AED */
    goto L2AED;
L2AEA: /* L2AEA */
    /* 2AEA  jmp     L287E */
    goto L287E;
L2AED: /* L2AED */
    /* 2AED  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L2AEF:
    /* 2AEF  cmp     cx,dx */
    sub16(CX, DX, 0);
L2AF1:
    /* 2AF1  jle     short L2AF5 */
    if (ZF || SF != OF) goto L2AF5;
L2AF3:
    /* 2AF3  mov     al,4 */
    AL = 0x4;
L2AF5: /* L2AF5 */
    /* 2AF5  neg     dx */
    DX = (uint16_t)-DX;
L2AF7:
    /* 2AF7  cmp     cx,dx */
    sub16(CX, DX, 0);
L2AF9:
    /* 2AF9  jge     short L2AFE */
    if (SF == OF) goto L2AFE;
L2AFB:
    /* 2AFB  mov     ax,8 */
    AX = 0x8;
L2AFE: /* L2AFE */
    /* 2AFE  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L2B01:
    /* 2B01  jmp     L2891 */
    goto L2891;

    /* seg004_2B04  (+2B04)
       Opcode BEh, operands: a, b. Model variables: *[b] = [a] (b holds the address to store to). */
L2B04: /* _do_movei */
    /* 2B04  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2B05:
    /* 2B05  mov     di,ax */
    DI = AX;
L2B07:
    /* 2B07  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2B08:
    /* 2B08  mov     bx,ax */
    BX = AX;
L2B0A:
    /* 2B0A  mov     bx,word ptr [bx] */
    BX = rw(pDS, BX);
L2B0C:
    /* 2B0C  mov     ax,word ptr [di] */
    AX = rw(pDS, DI);
L2B0E:
    /* 2B0E  mov     word ptr [bx],ax */
    ww(pDS, BX, AX);
L2B10:
    /* 2B10  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2B11:
    /* 2B11  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2B12:
    /* 2B12  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2B16  (+2B16)
       Opcode 02h, operands: address, value. Stores the constant in the model variable at address
       (GRIDDB.C's Clk(n) slots). */
L2B16: /* _do_movec */
    /* 2B16  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2B17:
    /* 2B17  mov     di,ax */
    DI = AX;
L2B19:
    /* 2B19  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2B1A:
    /* 2B1A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2B1B:
    /* 2B1B  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2B1C:
    /* 2B1C  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2B20  (+2B20)
       Opcode 04h, operands: a, b. Model variables: [b] = [a]. */
L2B20: /* _do_movem */
    /* 2B20  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2B21:
    /* 2B21  mov     bx,ax */
    BX = AX;
L2B23:
    /* 2B23  mov     bx,word ptr [bx] */
    BX = rw(pDS, BX);
L2B25:
    /* 2B25  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2B26:
    /* 2B26  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2B27:
    /* 2B27  mov     word ptr [bx],ax */
    ww(pDS, BX, AX);
L2B29:
    /* 2B29  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2B2A:
    /* 2B2A  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2B2B:
    /* 2B2B  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2B2F  (+2B2F)
       Opcode 0Ah, operands: offset, address. [address] = ss:[BP + offset] (a [bp+di] operand,
       so the stack segment), with BP as the caller left it. */
L2B2F: /* _do_move_odata */
    /* 2B2F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2B30:
    /* 2B30  xchg    di,ax */
    { uint16_t t_ = DI;
    DI = AX;
    AX = t_; }
L2B31:
    /* 2B31  mov     bx,word ptr [bp+di] */
    BX = rw(pSS, BP + DI);
L2B33:
    /* 2B33  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2B34:
    /* 2B34  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2B35:
    /* 2B35  mov     word ptr [bx],ax */
    ww(pDS, BX, AX);
L2B37:
    /* 2B37  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2B38:
    /* 2B38  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2B39:
    /* 2B39  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2B3D  (+2B3D)
       Opcode 82h, operands: count, first point, then count (x, z, y) coordinate triples. Each
       is offset by the object origin, shifted, rotated (load_xlate_rotate_pnt), clip-coded
       (code_pnt) and stored in consecutive points. */
L2B3D: /* _do_multires */
    /* 2B3D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2B3E:
    /* 2B3E  mov     word ptr ds:[271Ch],ax */
    ww(pDS, 0x271C, AX);
L2B41:
    /* 2B41  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2B42:
    /* 2B42  mov     di,ax */
    DI = AX;
L2B44:
    /* 2B44  add     di,PNT_X */
    DI = (uint16_t)(DI + 0x1620);
L2B48:
    /* 2B48  mov     word ptr ds:[271Eh],di */
    ww(pDS, 0x271E, DI);
L2B4C: /* L2B4C */
    /* 2B4C  call    _load_xlate_rotate_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x50BE), 0x2B4F)) != 0) return c;
L2B4F:
    /* 2B4F  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x2B52)) != 0) return c;
L2B52:
    /* 2B52  mov     di,word ptr ds:[271Eh] */
    DI = rw(pDS, 0x271E);
L2B56:
    /* 2B56  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L2B58:
    /* 2B58  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L2B5B:
    /* 2B5B  mov     word ptr [di+4],bp */
    ww(pDS, DI + 0x4, BP);
L2B5E:
    /* 2B5E  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L2B61:
    /* 2B61  add     word ptr ds:[271Eh],8 */
    ww(pDS, 0x271E, add16(rw(pDS, 0x271E), 0x8, 0));
L2B66:
    /* 2B66  dec     word ptr ds:[271Ch] */
    ww(pDS, 0x271C, dec16(rw(pDS, 0x271C)));
L2B6A:
    /* 2B6A  jne     L2B4C */
    if (!ZF) goto L2B4C;
L2B6C:
    /* 2B6C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2B6D:
    /* 2B6D  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2B6E:
    /* 2B6E  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2B72  (+2B72)
       Opcode B8h: do_multires with byte operands: a byte count, a byte point number, and byte
       coordinates (mini_xlate_rotate_pnt scales each by 32). */
L2B72: /* _do_minimulti */
    /* 2B72  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L2B73:
    /* 2B73  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L2B75:
    /* 2B75  mov     word ptr ds:[271Ch],ax */
    ww(pDS, 0x271C, AX);
L2B78:
    /* 2B78  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L2B79:
    /* 2B79  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L2B7B:
    /* 2B7B  shl     ax,3 */
    AX = (uint16_t)(AX << 3);
L2B7E:
    /* 2B7E  mov     di,ax */
    DI = AX;
L2B80:
    /* 2B80  add     di,PNT_X */
    DI = (uint16_t)(DI + 0x1620);
L2B84:
    /* 2B84  mov     word ptr ds:[271Eh],di */
    ww(pDS, 0x271E, DI);
L2B88: /* L2B88 */
    /* 2B88  call    _mini_xlate_rotate_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5096), 0x2B8B)) != 0) return c;
L2B8B:
    /* 2B8B  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x2B8E)) != 0) return c;
L2B8E:
    /* 2B8E  mov     di,word ptr ds:[271Eh] */
    DI = rw(pDS, 0x271E);
L2B92:
    /* 2B92  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L2B94:
    /* 2B94  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L2B97:
    /* 2B97  mov     word ptr [di+4],bp */
    ww(pDS, DI + 0x4, BP);
L2B9A:
    /* 2B9A  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L2B9D:
    /* 2B9D  add     word ptr ds:[271Eh],8 */
    ww(pDS, 0x271E, add16(rw(pDS, 0x271E), 0x8, 0));
L2BA2:
    /* 2BA2  dec     word ptr ds:[271Ch] */
    ww(pDS, 0x271C, dec16(rw(pDS, 0x271C)));
L2BA6:
    /* 2BA6  jne     L2B88 */
    if (!ZF) goto L2B88;
L2BA8:
    /* 2BA8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2BA9:
    /* 2BA9  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2BAA:
    /* 2BAA  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2BAE  (+2BAE)
       Opcode CCh, operands: count, then count (point, shade byte) pairs, padded to a word. Sets
       each point's shade (+7) for Gouraud polygons. */
L2BAE: /* _do_setshade */
    /* 2BAE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2BAF:
    /* 2BAF  mov     cx,ax */
    CX = AX;
L2BB1:
    /* 2BB1  mov     di,PNT_X */
    DI = 0x1620;
L2BB4: /* L2BB4 */
    /* 2BB4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2BB5:
    /* 2BB5  mov     bx,ax */
    BX = AX;
L2BB7:
    /* 2BB7  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L2BB8:
    /* 2BB8  mov     byte ptr [bx+di+7],al */
    wb(pDS, BX + DI + 0x7, AL);
L2BBB:
    /* 2BBB  loop    L2BB4 */
    if (--CX) goto L2BB4;
L2BBD:
    /* 2BBD  inc     si */
    SI = (uint16_t)(SI + 1);
L2BBE:
    /* 2BBE  and     si,-2 */
    SI = logic16((uint16_t)(SI & 0xFFFE));
L2BC1:
    /* 2BC1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2BC2:
    /* 2BC2  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2BC3:
    /* 2BC3  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2BC7  (+2BC7)
       Opcode D4h (UW-specific), operands: count, the address of a model variable, then count
       (point, shade byte) pairs, padded to a word. Writes the variable's low byte to seg004:6950h
       (PGCACHE's first byte, which seg003 reads) and sets each point's shade to its byte plus the
       shade bias at 2934h, at most 0Eh. */
L2BC7: /* _do_uwshade */
    /* 2BC7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2BC8:
    /* 2BC8  mov     cx,ax */
    CX = AX;
L2BCA:
    /* 2BCA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2BCB:
    /* 2BCB  mov     bx,ax */
    BX = AX;
L2BCD:
    /* 2BCD  mov     dx,word ptr [bx] */
    DX = rw(pDS, BX);
L2BCF:
    /* 2BCF  mov     byte ptr cs:[6950h],dl */
    wb(CODE004, 0x6950, DL);
L2BD4:
    /* 2BD4  mov     dx,word ptr ds:[2934h] */
    DX = rw(pDS, 0x2934);
L2BD8:
    /* 2BD8  mov     di,PNT_X */
    DI = 0x1620;
L2BDB: /* L2BDB */
    /* 2BDB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2BDC:
    /* 2BDC  mov     bx,ax */
    BX = AX;
L2BDE:
    /* 2BDE  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L2BDF:
    /* 2BDF  add     al,dl */
    AL = (uint8_t)(AL + DL);
L2BE1:
    /* 2BE1  cmp     al,0Eh */
    sub8(AL, 0xE, 0);
L2BE3:
    /* 2BE3  jbe     short L2BE7 */
    if (CF || ZF) goto L2BE7;
L2BE5:
    /* 2BE5  mov     al,0Eh */
    AL = 0xE;
L2BE7: /* L2BE7 */
    /* 2BE7  mov     byte ptr [bx+di+7],al */
    wb(pDS, BX + DI + 0x7, AL);
L2BEA:
    /* 2BEA  loop    L2BDB */
    if (--CX) goto L2BDB;
L2BEC:
    /* 2BEC  inc     si */
    SI = (uint16_t)(SI + 1);
L2BED:
    /* 2BED  and     si,-2 */
    SI = logic16((uint16_t)(SI & 0xFFFE));
L2BF0:
    /* 2BF0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2BF1:
    /* 2BF1  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2BF2:
    /* 2BF2  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L2BF6:
    /* 2BF6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2BF7:
    /* 2BF7  mov     bx,word ptr [si+6] */
    BX = rw(pDS, SI + 0x6);
L2BFA:
    /* 2BFA  mov     byte ptr [bx+PNT_SHADE],al */
    wb(pDS, BX + 0x1627, AL);

    /* seg004_2BFE  (+2BFE)
       Opcode 7Ah, operands: (x, z, y), point. Transforms one coordinate triple and stores it
       with its clip code. The unlabelled entry just before it (2BF6h) is FM's do_defresg: it
       takes a shade word first and sets the point's shade from it, then falls in here; the opcode
       table has no entry for it, in UW1 as in UW2. */
L2BFE: /* _do_defres */
    /* 2BFE  call    _load_xlate_rotate_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x50BE), 0x2C01)) != 0) return c;
L2C01:
    /* 2C01  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2C02:
    /* 2C02  mov     di,ax */
    DI = AX;
L2C04:
    /* 2C04  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L2C08:
    /* 2C08  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L2C0C:
    /* 2C0C  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L2C10:
    /* 2C10  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x2C13)) != 0) return c;
L2C13:
    /* 2C13  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L2C17:
    /* 2C17  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2C18:
    /* 2C18  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2C19:
    /* 2C19  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2C1D  (+2C1D)
       Opcode B6h: do_defres with byte coordinates and a byte point number (GRIDDB's SetPnt
       records). */
L2C1D: /* _do_minires */
    /* 2C1D  call    _mini_xlate_rotate_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5096), 0x2C20)) != 0) return c;
L2C20:
    /* 2C20  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L2C21:
    /* 2C21  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L2C23:
    /* 2C23  shl     ax,3 */
    AX = (uint16_t)(AX << 3);
L2C26:
    /* 2C26  mov     di,ax */
    DI = AX;
L2C28:
    /* 2C28  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L2C2C:
    /* 2C2C  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L2C30:
    /* 2C30  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L2C34:
    /* 2C34  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x2C37)) != 0) return c;
L2C37:
    /* 2C37  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L2C3B:
    /* 2C3B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2C3C:
    /* 2C3C  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2C3D:
    /* 2C3D  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2C41  (+2C41)
       Opcode 7Ch, operand: point. Starts a polygon: copies the point to the vertex buffer at
       309h and sets the codes OR and AND from it. */
L2C41: /* _do_strres */
    /* 2C41  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2C42:
    /* 2C42  mov     dx,si */
    DX = SI;
L2C44:
    /* 2C44  mov     si,ax */
    SI = AX;
L2C46:
    /* 2C46  add     si,PNT_X */
    SI = add16(SI, 0x1620, 0);
L2C4A:
    /* 2C4A  mov     di,VBUF0 */
    DI = 0x309;
L2C4D:
    /* 2C4D  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2C4E:
    /* 2C4E  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2C4F:
    /* 2C4F  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2C50:
    /* 2C50  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L2C51:
    /* 2C51  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2C52:
    /* 2C52  mov     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, AL);
L2C55:
    /* 2C55  mov     byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, AL);
L2C58:
    /* 2C58  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L2C5C:
    /* 2C5C  mov     si,dx */
    SI = DX;
L2C5E:
    /* 2C5E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2C5F:
    /* 2C5F  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2C60:
    /* 2C60  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2C64  (+2C64)
       Opcode 8Eh, operand: point. Appends a point to the polygon do_strres started (falls into
       the dispatch; do_closure draws it). */
L2C64: /* _do_cntres */
    /* 2C64  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2C65:
    /* 2C65  mov     dx,si */
    DX = SI;
L2C67:
    /* 2C67  mov     si,ax */
    SI = AX;
L2C69:
    /* 2C69  add     si,PNT_X */
    SI = (uint16_t)(SI + 0x1620);
L2C6D:
    /* 2C6D  mov     di,word ptr ds:[VB_END] */
    DI = rw(pDS, 0x305);
L2C71:
    /* 2C71  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2C72:
    /* 2C72  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2C73:
    /* 2C73  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2C74:
    /* 2C74  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L2C75:
    /* 2C75  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2C76:
    /* 2C76  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L2C7A:
    /* 2C7A  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, logic8((uint8_t)(rb(pDS, 0x161A) | AL)));
L2C7E:
    /* 2C7E  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L2C82:
    /* 2C82  mov     si,dx */
    SI = DX;
L2C84: /* L2C84 */
    /* 2C84  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2C85:
    /* 2C85  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2C86:
    /* 2C86  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_2C8A  (+2C8A)
       Opcode 9Ch: a rod, a quadrilateral of fixed width between two points, facing the eye. For
       each end: operand 0 then a point (that end is a single vertex), or a half-width then a
       point (two vertices, offset sideways by the width rotated through 26D0h/26D2h, which
       scale_matrix computes). Then draws it with do_closure. */
L2C8A: /* _do_rod */
    /* 2C8A  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L2C8F:
    /* 2C8F  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L2C94:
    /* 2C94  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2C95:
    /* 2C95  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L2C97:
    /* 2C97  jne     short L2CB4 */
    if (!ZF) goto L2CB4;
L2C99:
    /* 2C99  mov     di,VBUF0 */
    DI = 0x309;
L2C9C:
    /* 2C9C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2C9D:
    /* 2C9D  push    si */
    push16(SI);
L2C9E:
    /* 2C9E  mov     si,ax */
    SI = AX;
L2CA0:
    /* 2CA0  add     si,PNT_X */
    SI = (uint16_t)(SI + 0x1620);
L2CA4:
    /* 2CA4  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2CA5:
    /* 2CA5  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2CA6:
    /* 2CA6  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2CA7:
    /* 2CA7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2CA8:
    /* 2CA8  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L2CAC:
    /* 2CAC  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L2CB0:
    /* 2CB0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2CB1:
    /* 2CB1  pop     si */
    SI = pop16();
L2CB2:
    /* 2CB2  jmp     short L2D11 */
    goto L2D11;
L2CB4: /* L2CB4 */
    /* 2CB4  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L2CB8:
    /* 2CB8  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2CBA:
    /* 2CBA  mov     cx,ax */
    CX = AX;
L2CBC:
    /* 2CBC  imul    word ptr ds:[26D2h] */
    imul16(rw(pDS, 0x26D2));
L2CC0:
    /* 2CC0  mov     ax,cx */
    AX = CX;
L2CC2:
    /* 2CC2  mov     cx,dx */
    CX = DX;
L2CC4:
    /* 2CC4  imul    word ptr ds:[26D0h] */
    imul16(rw(pDS, 0x26D0));
L2CC8:
    /* 2CC8  mov     di,VBUF0 */
    DI = 0x309;
L2CCB:
    /* 2CCB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2CCC:
    /* 2CCC  push    si */
    push16(SI);
L2CCD:
    /* 2CCD  mov     si,ax */
    SI = AX;
L2CCF:
    /* 2CCF  add     si,PNT_X */
    SI = (uint16_t)(SI + 0x1620);
L2CD3:
    /* 2CD3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2CD4:
    /* 2CD4  add     ax,dx */
    AX = (uint16_t)(AX + DX);
L2CD6:
    /* 2CD6  mov     bx,ax */
    BX = AX;
L2CD8:
    /* 2CD8  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2CD9:
    /* 2CD9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2CDA:
    /* 2CDA  add     ax,cx */
    AX = (uint16_t)(AX + CX);
L2CDC:
    /* 2CDC  push    cx */
    push16(CX);
L2CDD:
    /* 2CDD  mov     cx,ax */
    CX = AX;
L2CDF:
    /* 2CDF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2CE0:
    /* 2CE0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2CE1:
    /* 2CE1  mov     bp,ax */
    BP = AX;
L2CE3:
    /* 2CE3  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2CE4:
    /* 2CE4  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x2CE7)) != 0) return c;
L2CE7:
    /* 2CE7  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L2CEB:
    /* 2CEB  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L2CEF:
    /* 2CEF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2CF0:
    /* 2CF0  pop     cx */
    CX = pop16();
L2CF1:
    /* 2CF1  sub     si,6 */
    SI = (uint16_t)(SI - 0x6);
L2CF4:
    /* 2CF4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2CF5:
    /* 2CF5  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L2CF7:
    /* 2CF7  mov     bx,ax */
    BX = AX;
L2CF9:
    /* 2CF9  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2CFA:
    /* 2CFA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2CFB:
    /* 2CFB  sub     ax,cx */
    AX = (uint16_t)(AX - CX);
L2CFD:
    /* 2CFD  mov     cx,ax */
    CX = AX;
L2CFF:
    /* 2CFF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2D00:
    /* 2D00  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2D01:
    /* 2D01  mov     bp,ax */
    BP = AX;
L2D03:
    /* 2D03  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2D04:
    /* 2D04  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x2D07)) != 0) return c;
L2D07:
    /* 2D07  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L2D0B:
    /* 2D0B  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L2D0F:
    /* 2D0F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2D10:
    /* 2D10  pop     si */
    SI = pop16();
L2D11: /* L2D11 */
    /* 2D11  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2D12:
    /* 2D12  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L2D14:
    /* 2D14  jne     short L2D2E */
    if (!ZF) goto L2D2E;
L2D16:
    /* 2D16  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2D17:
    /* 2D17  push    si */
    push16(SI);
L2D18:
    /* 2D18  mov     si,ax */
    SI = AX;
L2D1A:
    /* 2D1A  add     si,PNT_X */
    SI = (uint16_t)(SI + 0x1620);
L2D1E:
    /* 2D1E  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2D1F:
    /* 2D1F  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2D20:
    /* 2D20  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2D21:
    /* 2D21  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2D22:
    /* 2D22  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L2D26:
    /* 2D26  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L2D2A:
    /* 2D2A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2D2B:
    /* 2D2B  pop     si */
    SI = pop16();
L2D2C:
    /* 2D2C  jmp     short L2D88 */
    goto L2D88;
L2D2E: /* L2D2E */
    /* 2D2E  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L2D32:
    /* 2D32  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2D34:
    /* 2D34  mov     cx,ax */
    CX = AX;
L2D36:
    /* 2D36  imul    word ptr ds:[26D2h] */
    imul16(rw(pDS, 0x26D2));
L2D3A:
    /* 2D3A  mov     ax,cx */
    AX = CX;
L2D3C:
    /* 2D3C  mov     cx,dx */
    CX = DX;
L2D3E:
    /* 2D3E  imul    word ptr ds:[26D0h] */
    imul16(rw(pDS, 0x26D0));
L2D42:
    /* 2D42  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2D43:
    /* 2D43  push    si */
    push16(SI);
L2D44:
    /* 2D44  mov     si,ax */
    SI = AX;
L2D46:
    /* 2D46  add     si,PNT_X */
    SI = (uint16_t)(SI + 0x1620);
L2D4A:
    /* 2D4A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2D4B:
    /* 2D4B  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L2D4D:
    /* 2D4D  mov     bx,ax */
    BX = AX;
L2D4F:
    /* 2D4F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2D50:
    /* 2D50  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2D51:
    /* 2D51  sub     ax,cx */
    AX = (uint16_t)(AX - CX);
L2D53:
    /* 2D53  push    cx */
    push16(CX);
L2D54:
    /* 2D54  mov     cx,ax */
    CX = AX;
L2D56:
    /* 2D56  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2D57:
    /* 2D57  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2D58:
    /* 2D58  mov     bp,ax */
    BP = AX;
L2D5A:
    /* 2D5A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2D5B:
    /* 2D5B  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x2D5E)) != 0) return c;
L2D5E:
    /* 2D5E  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L2D62:
    /* 2D62  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L2D66:
    /* 2D66  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2D67:
    /* 2D67  pop     cx */
    CX = pop16();
L2D68:
    /* 2D68  sub     si,6 */
    SI = (uint16_t)(SI - 0x6);
L2D6B:
    /* 2D6B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2D6C:
    /* 2D6C  add     ax,dx */
    AX = (uint16_t)(AX + DX);
L2D6E:
    /* 2D6E  mov     bx,ax */
    BX = AX;
L2D70:
    /* 2D70  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2D71:
    /* 2D71  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2D72:
    /* 2D72  add     ax,cx */
    AX = (uint16_t)(AX + CX);
L2D74:
    /* 2D74  mov     cx,ax */
    CX = AX;
L2D76:
    /* 2D76  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2D77:
    /* 2D77  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2D78:
    /* 2D78  mov     bp,ax */
    BP = AX;
L2D7A:
    /* 2D7A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2D7B:
    /* 2D7B  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x2D7E)) != 0) return c;
L2D7E:
    /* 2D7E  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L2D82:
    /* 2D82  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L2D86:
    /* 2D86  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2D87:
    /* 2D87  pop     si */
    SI = pop16();
L2D88: /* L2D88 */
    /* 2D88  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L2D8C:
    /* 2D8C  jmp     _do_closure */
    goto L2EAC;

    /* seg004_2D8F  (+2D8F)
       do_rod for a level view (check_flat installs it in the opcode table): the sideways
       offset is along x only, using 272Ch. */
L2D8F: /* _flat_rod */
    /* 2D8F  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L2D94:
    /* 2D94  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L2D99:
    /* 2D99  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2D9A:
    /* 2D9A  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L2D9C:
    /* 2D9C  jne     short L2DB9 */
    if (!ZF) goto L2DB9;
L2D9E:
    /* 2D9E  mov     di,VBUF0 */
    DI = 0x309;
L2DA1:
    /* 2DA1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2DA2:
    /* 2DA2  push    si */
    push16(SI);
L2DA3:
    /* 2DA3  mov     si,ax */
    SI = AX;
L2DA5:
    /* 2DA5  add     si,PNT_X */
    SI = (uint16_t)(SI + 0x1620);
L2DA9:
    /* 2DA9  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2DAA:
    /* 2DAA  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2DAB:
    /* 2DAB  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2DAC:
    /* 2DAC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2DAD:
    /* 2DAD  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L2DB1:
    /* 2DB1  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L2DB5:
    /* 2DB5  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2DB6:
    /* 2DB6  pop     si */
    SI = pop16();
L2DB7:
    /* 2DB7  jmp     short L2E06 */
    goto L2E06;
L2DB9: /* L2DB9 */
    /* 2DB9  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L2DBD:
    /* 2DBD  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2DBF:
    /* 2DBF  imul    word ptr ds:[272Ch] */
    imul16(rw(pDS, 0x272C));
L2DC3:
    /* 2DC3  mov     di,VBUF0 */
    DI = 0x309;
L2DC6:
    /* 2DC6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2DC7:
    /* 2DC7  push    si */
    push16(SI);
L2DC8:
    /* 2DC8  mov     si,ax */
    SI = AX;
L2DCA:
    /* 2DCA  add     si,PNT_X */
    SI = (uint16_t)(SI + 0x1620);
L2DCE:
    /* 2DCE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2DCF:
    /* 2DCF  add     ax,dx */
    AX = (uint16_t)(AX + DX);
L2DD1:
    /* 2DD1  mov     bx,ax */
    BX = AX;
L2DD3:
    /* 2DD3  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2DD4:
    /* 2DD4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2DD5:
    /* 2DD5  mov     cx,ax */
    CX = AX;
L2DD7:
    /* 2DD7  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2DD8:
    /* 2DD8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2DD9:
    /* 2DD9  mov     bp,ax */
    BP = AX;
L2DDB:
    /* 2DDB  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2DDC:
    /* 2DDC  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x2DDF)) != 0) return c;
L2DDF:
    /* 2DDF  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L2DE3:
    /* 2DE3  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L2DE7:
    /* 2DE7  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2DE8:
    /* 2DE8  sub     si,6 */
    SI = (uint16_t)(SI - 0x6);
L2DEB:
    /* 2DEB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2DEC:
    /* 2DEC  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L2DEE:
    /* 2DEE  mov     bx,ax */
    BX = AX;
L2DF0:
    /* 2DF0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2DF1:
    /* 2DF1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2DF2:
    /* 2DF2  mov     cx,ax */
    CX = AX;
L2DF4:
    /* 2DF4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2DF5:
    /* 2DF5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2DF6:
    /* 2DF6  mov     bp,ax */
    BP = AX;
L2DF8:
    /* 2DF8  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2DF9:
    /* 2DF9  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x2DFC)) != 0) return c;
L2DFC:
    /* 2DFC  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L2E00:
    /* 2E00  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L2E04:
    /* 2E04  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2E05:
    /* 2E05  pop     si */
    SI = pop16();
L2E06: /* L2E06 */
    /* 2E06  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2E07:
    /* 2E07  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L2E09:
    /* 2E09  jne     short L2E23 */
    if (!ZF) goto L2E23;
L2E0B:
    /* 2E0B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2E0C:
    /* 2E0C  push    si */
    push16(SI);
L2E0D:
    /* 2E0D  mov     si,ax */
    SI = AX;
L2E0F:
    /* 2E0F  add     si,PNT_X */
    SI = (uint16_t)(SI + 0x1620);
L2E13:
    /* 2E13  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2E14:
    /* 2E14  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2E15:
    /* 2E15  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2E16:
    /* 2E16  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2E17:
    /* 2E17  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L2E1B:
    /* 2E1B  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L2E1F:
    /* 2E1F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2E20:
    /* 2E20  pop     si */
    SI = pop16();
L2E21:
    /* 2E21  jmp     short L2E6D */
    goto L2E6D;
L2E23: /* L2E23 */
    /* 2E23  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L2E27:
    /* 2E27  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2E29:
    /* 2E29  imul    word ptr ds:[272Ch] */
    imul16(rw(pDS, 0x272C));
L2E2D:
    /* 2E2D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2E2E:
    /* 2E2E  push    si */
    push16(SI);
L2E2F:
    /* 2E2F  mov     si,ax */
    SI = AX;
L2E31:
    /* 2E31  add     si,PNT_X */
    SI = (uint16_t)(SI + 0x1620);
L2E35:
    /* 2E35  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2E36:
    /* 2E36  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L2E38:
    /* 2E38  mov     bx,ax */
    BX = AX;
L2E3A:
    /* 2E3A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2E3B:
    /* 2E3B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2E3C:
    /* 2E3C  mov     cx,ax */
    CX = AX;
L2E3E:
    /* 2E3E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2E3F:
    /* 2E3F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2E40:
    /* 2E40  mov     bp,ax */
    BP = AX;
L2E42:
    /* 2E42  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2E43:
    /* 2E43  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x2E46)) != 0) return c;
L2E46:
    /* 2E46  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L2E4A:
    /* 2E4A  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L2E4E:
    /* 2E4E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2E4F:
    /* 2E4F  sub     si,6 */
    SI = (uint16_t)(SI - 0x6);
L2E52:
    /* 2E52  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2E53:
    /* 2E53  add     ax,dx */
    AX = (uint16_t)(AX + DX);
L2E55:
    /* 2E55  mov     bx,ax */
    BX = AX;
L2E57:
    /* 2E57  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2E58:
    /* 2E58  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2E59:
    /* 2E59  mov     cx,ax */
    CX = AX;
L2E5B:
    /* 2E5B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2E5C:
    /* 2E5C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2E5D:
    /* 2E5D  mov     bp,ax */
    BP = AX;
L2E5F:
    /* 2E5F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2E60:
    /* 2E60  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x2E63)) != 0) return c;
L2E63:
    /* 2E63  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L2E67:
    /* 2E67  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L2E6B:
    /* 2E6B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2E6C:
    /* 2E6C  pop     si */
    SI = pop16();
L2E6D: /* L2E6D */
    /* 2E6D  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L2E71:
    /* 2E71  jmp     short _do_closure */
    goto L2EAC;

    /* seg004_2E73  (+2E73)
       Opcodes 22h and 7Eh, operands: count, then count points. Builds a polygon in the vertex
       buffer, with the codes OR in BH and AND in BL; if they AND to nonzero it is skipped,
       otherwise it falls into do_closure to be drawn. (System Shock: do_polyres.) */
L2E73: /* _do_polyres */
    /* 2E73  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2E74:
    /* 2E74  mov     cx,ax */
    CX = AX;
L2E76:
    /* 2E76  mov     di,VBUF0 */
    DI = 0x309;
L2E79:
    /* 2E79  xor     bx,bx */
    BX = (uint16_t)(BX ^ BX);
L2E7B:
    /* 2E7B  not     bl */
    BL = (uint8_t)~BL;
L2E7D:
    /* 2E7D  mov     bp,PNT_X */
    BP = 0x1620;
L2E80: /* L2E80 */
    /* 2E80  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2E81:
    /* 2E81  xchg    si,ax */
    { uint16_t t_ = SI;
    SI = AX;
    AX = t_; }
L2E82:
    /* 2E82  add     si,bp */
    SI = (uint16_t)(SI + BP);
L2E84:
    /* 2E84  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2E85:
    /* 2E85  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2E86:
    /* 2E86  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2E87:
    /* 2E87  mov     dl,byte ptr [si] */
    DL = rb(pDS, SI);
L2E89:
    /* 2E89  or      bh,dl */
    BH = (uint8_t)(BH | DL);
L2E8B:
    /* 2E8B  and     bl,dl */
    BL = logic8((uint8_t)(BL & DL));
L2E8D:
    /* 2E8D  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L2E8E:
    /* 2E8E  mov     si,ax */
    SI = AX;
L2E90:
    /* 2E90  loop    L2E80 */
    if (--CX) goto L2E80;
L2E92:
    /* 2E92  je      short L2EA0 */
    if (ZF) goto L2EA0;
L2E94:
    /* 2E94  mov     word ptr ds:[2884h],0 */
    ww(pDS, 0x2884, 0x0);
L2E9A:
    /* 2E9A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2E9B:
    /* 2E9B  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2E9C:
    /* 2E9C  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L2EA0: /* L2EA0 */
    /* 2EA0  mov     byte ptr ds:[CLIP_OR],bh */
    wb(pDS, 0x161A, BH);
L2EA4:
    /* 2EA4  mov     byte ptr ds:[CLIP_AND],bl */
    wb(pDS, 0x161B, BL);
L2EA8:
    /* 2EA8  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);

    /* seg004_2EAC  (+2EAC)
       Opcode 80h: draws the polygon in the vertex buffer. Gouraud polygons (15F6h = the seg003
       Gouraud routine) go to SMOOTH.ASM's smooth_closure. Otherwise: AND of codes nonzero, skip;
       OR zero, project each vertex (x*[26B2]/z + centre, y*[26B0]/z + centre) to seg048:415Ch
       and call seg003's polygon routine (_seg003_5B0B) with BP = the fill routine and CX =
       the vertex count; behind the eye (80h), clip in 3D first (must_clip_3d, FM); otherwise
       (clip_needed, FM) project with overflow checks and fall back to do_3d_clip if a screen
       coordinate overflows. The labels inside are FM names; the add immediates at modify_biasx,
       modify_biasy, modify_bx2 and modify_by2 are the screen centre, patched by render_3d. */
L2EAC: /* _do_closure */
    /* 2EAC  mov     di,VBUF0 */
    DI = 0x309;
L2EAF:
    /* 2EAF  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L2EB3:
    /* 2EB3  cmp     word ptr ds:[POLY_FILL],offset _seg003_6146 */
    sub16(rw(pDS, 0x15F6), 0x6146, 0);
L2EB9:
    /* 2EB9  jne     short draw_poly_buf_ptr */
    if (!ZF) goto L2EBE;
L2EBB:
    /* 2EBB  jmp     _smooth_closure */
    return ASM_JMP(0x06E7, 0x0260);
L2EBE: /* draw_poly_buf_ptr */
    /* 2EBE  mov     word ptr ds:[2884h],0 */
    ww(pDS, 0x2884, 0x0);
L2EC4:
    /* 2EC4  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L2EC9:
    /* 2EC9  je      short L2ECE */
    if (ZF) goto L2ECE;
L2ECB:
    /* 2ECB  jmp     L2C84 */
    goto L2C84;
L2ECE: /* L2ECE */
    /* 2ECE  test    byte ptr ds:[CLIP_OR],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0xFF));
L2ED3:
    /* 2ED3  jne     short clip_needed */
    if (!ZF) goto L2F2D;
L2ED5: /* L2ED5 */
    /* 2ED5  push    si */
    push16(SI);
L2ED6:
    /* 2ED6  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L2ED9:
    /* 2ED9  mov     es,ax */
    SET_ES(AX);
L2EDB:
    /* 2EDB  mov     di,415Ch */
    DI = 0x415C;
L2EDE:
    /* 2EDE  mov     bx,word ptr ds:[VB_END] */
    BX = rw(pDS, 0x305);
L2EE2:
    /* 2EE2  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L2EE6:
    /* 2EE6  mov     cx,word ptr ds:[PROJ_X] */
    CX = rw(pDS, 0x26B2);
L2EEA: /* L2EEA */
    /* 2EEA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2EEB:
    /* 2EEB  mov     bp,word ptr [si+2] */
    BP = rw(pDS, SI + 0x2);
L2EEE:
    /* 2EEE  imul    cx */
    imul16(CX);
L2EF0:
    /* 2EF0  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x2EF0, 2)) != 0) return c;
L2EF2: /* modify_biasx */
    /* 2EF2  add     ax,4D2h */
    AX = (uint16_t)(AX + rw(CODE004, 0x2EF3));
L2EF5:
    /* 2EF5  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2EF6:
    /* 2EF6  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L2EF8:
    /* 2EF8  imul    word ptr ds:[PROJ_Y] */
    imul16(rw(pDS, 0x26B0));
L2EFC:
    /* 2EFC  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x2EFC, 2)) != 0) return c;
L2EFE: /* modify_biasy */
    /* 2EFE  add     ax,4D2h */
    AX = (uint16_t)(AX + rw(CODE004, 0x2EFF));
L2F01:
    /* 2F01  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2F02:
    /* 2F02  add     si,6 */
    SI = (uint16_t)(SI + 0x6);
L2F05:
    /* 2F05  cmp     si,bx */
    sub16(SI, BX, 0);
L2F07:
    /* 2F07  jne     L2EEA */
    if (!ZF) goto L2EEA;
L2F09:
    /* 2F09  sub     bx,word ptr ds:[VB_START] */
    BX = (uint16_t)(BX - rw(pDS, 0x307));
L2F0D:
    /* 2F0D  shr     bx,1 */
    BX = (uint16_t)(BX >> 1);
L2F0F:
    /* 2F0F  shr     bx,1 */
    BX = (uint16_t)(BX >> 1);
L2F11:
    /* 2F11  shr     bx,1 */
    BX = (uint16_t)(BX >> 1);
L2F13:
    /* 2F13  mov     cx,bx */
    CX = BX;
L2F15:
    /* 2F15  inc     word ptr ds:[2884h] */
    ww(pDS, 0x2884, (uint16_t)(rw(pDS, 0x2884) + 1));
L2F19:
    /* 2F19  mov     bp,word ptr ds:[POLY_FILL] */
    BP = rw(pDS, 0x15F6);
L2F1D:
    /* 2F1D  call    far ptr _seg003_5B0B */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x5B0B), 0x06E7 + PORT_LOAD_SEG, 0x2F22)) != 0) return c;
L2F22:
    /* 2F22  mov     ax,ds */
    AX = asm_ds;
L2F24:
    /* 2F24  mov     es,ax */
    SET_ES(AX);
L2F26:
    /* 2F26  pop     si */
    SI = pop16();
L2F27:
    /* 2F27  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2F28:
    /* 2F28  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2F29:
    /* 2F29  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L2F2D: /* clip_needed */
    /* 2F2D  js      short must_clip_3d */
    if (SF) goto L2F98;
L2F2F:
    /* 2F2F  push    si */
    push16(SI);
L2F30: /* L2F30 */
    /* 2F30  mov     word ptr ss:[4D5h],offset _clip_overflow */
    ww(pSS, 0x4D5, 0x3FF7);
L2F37:
    /* 2F37  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L2F3A:
    /* 2F3A  mov     es,ax */
    SET_ES(AX);
L2F3C:
    /* 2F3C  mov     di,415Ch */
    DI = 0x415C;
L2F3F:
    /* 2F3F  mov     bx,word ptr ds:[VB_END] */
    BX = rw(pDS, 0x305);
L2F43:
    /* 2F43  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L2F47:
    /* 2F47  mov     cx,word ptr ds:[PROJ_X] */
    CX = rw(pDS, 0x26B2);
L2F4B: /* L2F4B */
    /* 2F4B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2F4C:
    /* 2F4C  mov     bp,word ptr [si+2] */
    BP = rw(pDS, SI + 0x2);
L2F4F:
    /* 2F4F  imul    cx */
    imul16(CX);
L2F51:
    /* 2F51  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x2F51, 2)) != 0) return c;
L2F53: /* modify_bx2 */
    /* 2F53  add     ax,4D2h */
    AX = add16(AX, rw(CODE004, 0x2F54), 0);
L2F56:
    /* 2F56  jno     short L2F5B */
    if (!OF) goto L2F5B;
L2F58:
    /* 2F58  jmp     _do_3d_clip */
    goto L3FFB;
L2F5B: /* L2F5B */
    /* 2F5B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2F5C:
    /* 2F5C  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L2F5E:
    /* 2F5E  imul    word ptr ds:[PROJ_Y] */
    imul16(rw(pDS, 0x26B0));
L2F62:
    /* 2F62  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x2F62, 2)) != 0) return c;
L2F64: /* modify_by2 */
    /* 2F64  add     ax,4D2h */
    AX = add16(AX, rw(CODE004, 0x2F65), 0);
L2F67:
    /* 2F67  jno     short L2F6C */
    if (!OF) goto L2F6C;
L2F69:
    /* 2F69  jmp     _do_3d_clip */
    goto L3FFB;
L2F6C: /* L2F6C */
    /* 2F6C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2F6D:
    /* 2F6D  add     si,6 */
    SI = (uint16_t)(SI + 0x6);
L2F70:
    /* 2F70  cmp     si,bx */
    sub16(SI, BX, 0);
L2F72:
    /* 2F72  jne     L2F4B */
    if (!ZF) goto L2F4B;
L2F74:
    /* 2F74  mov     cx,bx */
    CX = BX;
L2F76:
    /* 2F76  sub     cx,word ptr ds:[VB_START] */
    CX = (uint16_t)(CX - rw(pDS, 0x307));
L2F7A:
    /* 2F7A  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L2F7C:
    /* 2F7C  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L2F7E:
    /* 2F7E  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L2F80:
    /* 2F80  inc     word ptr ds:[2884h] */
    ww(pDS, 0x2884, (uint16_t)(rw(pDS, 0x2884) + 1));
L2F84:
    /* 2F84  mov     bp,word ptr ds:[POLY_FILL] */
    BP = rw(pDS, 0x15F6);
L2F88:
    /* 2F88  call    far ptr _seg003_5B0B */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x5B0B), 0x06E7 + PORT_LOAD_SEG, 0x2F8D)) != 0) return c;
L2F8D:
    /* 2F8D  mov     ax,ds */
    AX = asm_ds;
L2F8F:
    /* 2F8F  mov     es,ax */
    SET_ES(AX);
L2F91:
    /* 2F91  pop     si */
    SI = pop16();
L2F92:
    /* 2F92  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2F93:
    /* 2F93  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L2F94:
    /* 2F94  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L2F98: /* must_clip_3d */
    /* 2F98  mov     word ptr ss:[4D5h],offset _overflow_handler_reg */
    ww(pSS, 0x4D5, 0x5BD1);
L2F9F:
    /* 2F9F  test    byte ptr ds:[CLIP_OR],4 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x4));
L2FA4:
    /* 2FA4  je      short L2FC1 */
    if (ZF) goto L2FC1;
L2FA6:
    /* 2FA6  push    si */
    push16(SI);
L2FA7:
    /* 2FA7  call    _clip_top */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x405B), 0x2FAA)) != 0) return c;
L2FAA:
    /* 2FAA  pop     si */
    SI = pop16();
L2FAB:
    /* 2FAB  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L2FB0:
    /* 2FB0  jne     short L3027 */
    if (!ZF) goto L3027;
L2FB2:
    /* 2FB2  test    byte ptr ds:[CLIP_OR],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0xFF));
L2FB7:
    /* 2FB7  js      short L2FC1 */
    if (SF) goto L2FC1;
L2FB9:
    /* 2FB9  jne     short L2FBE */
    if (!ZF) goto L2FBE;
L2FBB:
    /* 2FBB  jmp     L2ED5 */
    goto L2ED5;
L2FBE: /* L2FBE */
    /* 2FBE  jmp     clip_needed */
    goto L2F2D;
L2FC1: /* L2FC1 */
    /* 2FC1  test    byte ptr ds:[CLIP_OR],8 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x8));
L2FC6:
    /* 2FC6  je      short L2FE3 */
    if (ZF) goto L2FE3;
L2FC8:
    /* 2FC8  push    si */
    push16(SI);
L2FC9:
    /* 2FC9  call    _clip_bot */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x4164), 0x2FCC)) != 0) return c;
L2FCC:
    /* 2FCC  pop     si */
    SI = pop16();
L2FCD:
    /* 2FCD  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L2FD2:
    /* 2FD2  jne     short L3027 */
    if (!ZF) goto L3027;
L2FD4:
    /* 2FD4  test    byte ptr ds:[CLIP_OR],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0xFF));
L2FD9:
    /* 2FD9  js      short L2FE3 */
    if (SF) goto L2FE3;
L2FDB:
    /* 2FDB  jne     short L2FE0 */
    if (!ZF) goto L2FE0;
L2FDD:
    /* 2FDD  jmp     L2ED5 */
    goto L2ED5;
L2FE0: /* L2FE0 */
    /* 2FE0  jmp     clip_needed */
    goto L2F2D;
L2FE3: /* L2FE3 */
    /* 2FE3  test    byte ptr ds:[CLIP_OR],1 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x1));
L2FE8:
    /* 2FE8  je      short L3005 */
    if (ZF) goto L3005;
L2FEA:
    /* 2FEA  push    si */
    push16(SI);
L2FEB:
    /* 2FEB  call    _clip_left */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x4382), 0x2FEE)) != 0) return c;
L2FEE:
    /* 2FEE  pop     si */
    SI = pop16();
L2FEF:
    /* 2FEF  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L2FF4:
    /* 2FF4  jne     short L3027 */
    if (!ZF) goto L3027;
L2FF6:
    /* 2FF6  test    byte ptr ds:[CLIP_OR],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0xFF));
L2FFB:
    /* 2FFB  js      short L3005 */
    if (SF) goto L3005;
L2FFD:
    /* 2FFD  jne     short L3002 */
    if (!ZF) goto L3002;
L2FFF:
    /* 2FFF  jmp     L2ED5 */
    goto L2ED5;
L3002: /* L3002 */
    /* 3002  jmp     clip_needed */
    goto L2F2D;
L3005: /* L3005 */
    /* 3005  test    byte ptr ds:[CLIP_OR],2 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x2));
L300A:
    /* 300A  je      short L3027 */
    if (ZF) goto L3027;
L300C:
    /* 300C  push    si */
    push16(SI);
L300D:
    /* 300D  call    _clip_right */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x4274), 0x3010)) != 0) return c;
L3010:
    /* 3010  pop     si */
    SI = pop16();
L3011:
    /* 3011  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L3016:
    /* 3016  jne     short L3027 */
    if (!ZF) goto L3027;
L3018:
    /* 3018  test    byte ptr ds:[CLIP_OR],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0xFF));
L301D:
    /* 301D  js      short L3027 */
    if (SF) goto L3027;
L301F:
    /* 301F  jne     short L3024 */
    if (!ZF) goto L3024;
L3021:
    /* 3021  jmp     L2ED5 */
    goto L2ED5;
L3024: /* L3024 */
    /* 3024  jmp     clip_needed */
    goto L2F2D;
L3027: /* L3027 */
    /* 3027  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3028:
    /* 3028  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3029:
    /* 3029  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_302D  (+302D)
       Opcode 1Ah, operand: address. Calls a near routine in this segment and continues. */
L302D: /* _do_asmcal */
    /* 302D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L302E:
    /* 302E  call    ax */
    if ((c = asm_call(ASM_JMP(0x06E7, AX), 0x3030)) != 0) return c;
L3030:
    /* 3030  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3031:
    /* 3031  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3032:
    /* 3032  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3036  (+3036)
       Opcode 12h, operand: offset. Runs the sub-program at SI + offset (it ends with do_eof),
       then continues after the operand. (System Shock: do_sfcal.) */
L3036: /* _do_sfcal */
    /* 3036  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3037:
    /* 3037  push    si */
    push16(SI);
L3038:
    /* 3038  add     si,ax */
    SI = (uint16_t)(SI + AX);
L303A:
    /* 303A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L303B:
    /* 303B  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L303C:
    /* 303C  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x3040)) != 0) return c;
L3040:
    /* 3040  pop     si */
    SI = pop16();
L3041: /* L3041 */
    /* 3041  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3042:
    /* 3042  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3043:
    /* 3043  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3047  (+3047)
       Opcode 16h, operands: skip, address, value. If [address] >= value, skips `skip` bytes of
       program; otherwise continues (through do_sfcal's dispatch at L3041). */
L3047: /* _do_ifge */
    /* 3047  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3048:
    /* 3048  mov     cx,ax */
    CX = AX;
L304A:
    /* 304A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L304B:
    /* 304B  mov     bx,ax */
    BX = AX;
L304D:
    /* 304D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L304E:
    /* 304E  cmp     word ptr [bx],ax */
    sub16(rw(pDS, BX), AX, 0);
L3050:
    /* 3050  jl      L3041 */
    if (SF != OF) goto L3041;
L3052:
    /* 3052  add     si,cx */
    SI = add16(SI, CX, 0);
L3054:
    /* 3054  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3055:
    /* 3055  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3056:
    /* 3056  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_305A  (+305A)
       Opcode 14h: as do_ifge, skipping when [address] <= value. */
L305A: /* _do_ifle */
    /* 305A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L305B:
    /* 305B  mov     cx,ax */
    CX = AX;
L305D:
    /* 305D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L305E:
    /* 305E  mov     bx,ax */
    BX = AX;
L3060:
    /* 3060  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3061:
    /* 3061  cmp     word ptr [bx],ax */
    sub16(rw(pDS, BX), AX, 0);
L3063:
    /* 3063  jg      L3041 */
    if (!ZF && SF == OF) goto L3041;
L3065:
    /* 3065  add     si,cx */
    SI = add16(SI, CX, 0);
L3067:
    /* 3067  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3068:
    /* 3068  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3069:
    /* 3069  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_306D  (+306D)
       Opcode 6Ch, operands: address, value, offset a, offset b. Runs two sub-programs in an
       order chosen by a variable: a then b when [address] >= value, else b then a (the shared
       tail L3090 runs the second-listed first). The model's own back-to-front order switch. */
L306D: /* _do_sortvar */
    /* 306D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L306E:
    /* 306E  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L306F:
    /* 306F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3070:
    /* 3070  cmp     word ptr [bx],ax */
    sub16(rw(pDS, BX), AX, 0);
L3072:
    /* 3072  jl      short L3090 */
    if (SF != OF) goto L3090;
L3074:
    /* 3074  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3075:
    /* 3075  push    si */
    push16(SI);
L3076:
    /* 3076  add     si,ax */
    SI = (uint16_t)(SI + AX);
L3078:
    /* 3078  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3079:
    /* 3079  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L307A:
    /* 307A  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x307E)) != 0) return c;
L307E:
    /* 307E  pop     si */
    SI = pop16();
L307F:
    /* 307F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3080:
    /* 3080  push    si */
    push16(SI);
L3081:
    /* 3081  add     si,ax */
    SI = (uint16_t)(SI + AX);
L3083:
    /* 3083  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3084:
    /* 3084  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3085:
    /* 3085  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x3089)) != 0) return c;
L3089:
    /* 3089  pop     si */
    SI = pop16();
L308A:
    /* 308A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L308B:
    /* 308B  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L308C:
    /* 308C  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L3090: /* L3090 */
    /* 3090  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3091:
    /* 3091  add     ax,si */
    AX = (uint16_t)(AX + SI);
L3093:
    /* 3093  mov     bx,ax */
    BX = AX;
L3095:
    /* 3095  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3096:
    /* 3096  push    si */
    push16(SI);
L3097:
    /* 3097  push    bx */
    push16(BX);
L3098:
    /* 3098  add     si,ax */
    SI = (uint16_t)(SI + AX);
L309A:
    /* 309A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L309B:
    /* 309B  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L309C:
    /* 309C  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x30A0)) != 0) return c;
L30A0:
    /* 30A0  pop     si */
    SI = pop16();
L30A1:
    /* 30A1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L30A2:
    /* 30A2  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L30A3:
    /* 30A3  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x30A7)) != 0) return c;
L30A7:
    /* 30A7  pop     si */
    SI = pop16();
L30A8:
    /* 30A8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L30A9:
    /* 30A9  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L30AA:
    /* 30AA  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_30AE  (+30AE)
       Opcode 06h, operands: three (n, p) word pairs for x, y and z, then two sub-program offsets
       a and b. Computes the sign of the sum of n * (p + origin) over the axes (the origin at
       2886h..288Ah is the negated eye-relative position, so this is the side of the plane the
       eye is on) and runs both sub-programs, a then b when it is non-negative, b then a
       otherwise: a BSP split, so that the half nearer the eye is drawn last (System Shock:
       do_sortnorm). */
L30AE: /* _do_sortnorm */
    /* 30AE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L30AF:
    /* 30AF  mov     cx,ax */
    CX = AX;
L30B1:
    /* 30B1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L30B2:
    /* 30B2  add     ax,word ptr ds:[OBJ_X] */
    AX = (uint16_t)(AX + rw(pDS, 0x2886));
L30B6:
    /* 30B6  imul    cx */
    imul16(CX);
L30B8:
    /* 30B8  mov     cx,dx */
    CX = DX;
L30BA:
    /* 30BA  mov     bp,ax */
    BP = AX;
L30BC:
    /* 30BC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L30BD:
    /* 30BD  mov     di,ax */
    DI = AX;
L30BF:
    /* 30BF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L30C0:
    /* 30C0  add     ax,word ptr ds:[OBJ_Y] */
    AX = (uint16_t)(AX + rw(pDS, 0x2888));
L30C4:
    /* 30C4  imul    di */
    imul16(DI);
L30C6:
    /* 30C6  add     bp,ax */
    BP = add16(BP, AX, 0);
L30C8:
    /* 30C8  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L30CA:
    /* 30CA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L30CB:
    /* 30CB  mov     di,ax */
    DI = AX;
L30CD:
    /* 30CD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L30CE:
    /* 30CE  add     ax,word ptr ds:[OBJ_Z] */
    AX = (uint16_t)(AX + rw(pDS, 0x288A));
L30D2:
    /* 30D2  imul    di */
    imul16(DI);
L30D4:
    /* 30D4  add     bp,ax */
    BP = add16(BP, AX, 0);
L30D6:
    /* 30D6  adc     cx,dx */
    CX = add16(CX, DX, CF);
L30D8:
    /* 30D8  js      L3090 */
    if (SF) goto L3090;
L30DA:
    /* 30DA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L30DB:
    /* 30DB  push    si */
    push16(SI);
L30DC:
    /* 30DC  add     si,ax */
    SI = (uint16_t)(SI + AX);
L30DE:
    /* 30DE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L30DF:
    /* 30DF  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L30E0:
    /* 30E0  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x30E4)) != 0) return c;
L30E4:
    /* 30E4  pop     si */
    SI = pop16();
L30E5:
    /* 30E5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L30E6:
    /* 30E6  push    si */
    push16(SI);
L30E7:
    /* 30E7  add     si,ax */
    SI = (uint16_t)(SI + AX);
L30E9:
    /* 30E9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L30EA:
    /* 30EA  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L30EB:
    /* 30EB  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x30EF)) != 0) return c;
L30EF:
    /* 30EF  pop     si */
    SI = pop16();
L30F0:
    /* 30F0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L30F1:
    /* 30F1  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L30F2:
    /* 30F2  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_30F6  (+30F6)
       Opcode 0Ch: do_sortnorm with no x term in the normal. */
L30F6: /* _do_sortnorm_x0 */
    /* 30F6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L30F7:
    /* 30F7  mov     di,ax */
    DI = AX;
L30F9:
    /* 30F9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L30FA:
    /* 30FA  add     ax,word ptr ds:[OBJ_Y] */
    AX = (uint16_t)(AX + rw(pDS, 0x2888));
L30FE:
    /* 30FE  imul    di */
    imul16(DI);
L3100:
    /* 3100  mov     bp,ax */
    BP = AX;
L3102:
    /* 3102  mov     cx,dx */
    CX = DX;
L3104:
    /* 3104  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3105:
    /* 3105  mov     di,ax */
    DI = AX;
L3107:
    /* 3107  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3108:
    /* 3108  add     ax,word ptr ds:[OBJ_Z] */
    AX = (uint16_t)(AX + rw(pDS, 0x288A));
L310C:
    /* 310C  imul    di */
    imul16(DI);
L310E:
    /* 310E  add     bp,ax */
    BP = add16(BP, AX, 0);
L3110:
    /* 3110  adc     cx,dx */
    CX = add16(CX, DX, CF);
L3112:
    /* 3112  jns     short L3117 */
    if (!SF) goto L3117;
L3114:
    /* 3114  jmp     L3090 */
    goto L3090;
L3117: /* L3117 */
    /* 3117  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3118:
    /* 3118  push    si */
    push16(SI);
L3119:
    /* 3119  add     si,ax */
    SI = (uint16_t)(SI + AX);
L311B:
    /* 311B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L311C:
    /* 311C  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L311D:
    /* 311D  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x3121)) != 0) return c;
L3121:
    /* 3121  pop     si */
    SI = pop16();
L3122:
    /* 3122  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3123:
    /* 3123  push    si */
    push16(SI);
L3124:
    /* 3124  add     si,ax */
    SI = (uint16_t)(SI + AX);
L3126:
    /* 3126  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3127:
    /* 3127  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3128:
    /* 3128  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x312C)) != 0) return c;
L312C:
    /* 312C  pop     si */
    SI = pop16();
L312D:
    /* 312D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L312E:
    /* 312E  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L312F:
    /* 312F  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3133  (+3133)
       Opcode 0Eh: do_sortnorm with no y term. */
L3133: /* _do_sortnorm_y0 */
    /* 3133  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3134:
    /* 3134  mov     di,ax */
    DI = AX;
L3136:
    /* 3136  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3137:
    /* 3137  add     ax,word ptr ds:[OBJ_X] */
    AX = (uint16_t)(AX + rw(pDS, 0x2886));
L313B:
    /* 313B  imul    di */
    imul16(DI);
L313D:
    /* 313D  mov     bp,ax */
    BP = AX;
L313F:
    /* 313F  mov     cx,dx */
    CX = DX;
L3141:
    /* 3141  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3142:
    /* 3142  mov     di,ax */
    DI = AX;
L3144:
    /* 3144  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3145:
    /* 3145  add     ax,word ptr ds:[OBJ_Z] */
    AX = (uint16_t)(AX + rw(pDS, 0x288A));
L3149:
    /* 3149  imul    di */
    imul16(DI);
L314B:
    /* 314B  add     bp,ax */
    BP = add16(BP, AX, 0);
L314D:
    /* 314D  adc     cx,dx */
    CX = add16(CX, DX, CF);
L314F:
    /* 314F  jns     short L3154 */
    if (!SF) goto L3154;
L3151:
    /* 3151  jmp     L3090 */
    goto L3090;
L3154: /* L3154 */
    /* 3154  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3155:
    /* 3155  push    si */
    push16(SI);
L3156:
    /* 3156  add     si,ax */
    SI = (uint16_t)(SI + AX);
L3158:
    /* 3158  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3159:
    /* 3159  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L315A:
    /* 315A  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x315E)) != 0) return c;
L315E:
    /* 315E  pop     si */
    SI = pop16();
L315F:
    /* 315F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3160:
    /* 3160  push    si */
    push16(SI);
L3161:
    /* 3161  add     si,ax */
    SI = (uint16_t)(SI + AX);
L3163:
    /* 3163  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3164:
    /* 3164  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3165:
    /* 3165  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x3169)) != 0) return c;
L3169:
    /* 3169  pop     si */
    SI = pop16();
L316A:
    /* 316A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L316B:
    /* 316B  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L316C:
    /* 316C  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3170  (+3170)
       Opcode 10h: do_sortnorm with no z term. */
L3170: /* _do_sortnorm_z0 */
    /* 3170  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3171:
    /* 3171  mov     di,ax */
    DI = AX;
L3173:
    /* 3173  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3174:
    /* 3174  add     ax,word ptr ds:[OBJ_X] */
    AX = (uint16_t)(AX + rw(pDS, 0x2886));
L3178:
    /* 3178  imul    di */
    imul16(DI);
L317A:
    /* 317A  mov     bp,ax */
    BP = AX;
L317C:
    /* 317C  mov     cx,dx */
    CX = DX;
L317E:
    /* 317E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L317F:
    /* 317F  mov     di,ax */
    DI = AX;
L3181:
    /* 3181  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3182:
    /* 3182  add     ax,word ptr ds:[OBJ_Y] */
    AX = (uint16_t)(AX + rw(pDS, 0x2888));
L3186:
    /* 3186  imul    di */
    imul16(DI);
L3188:
    /* 3188  add     bp,ax */
    BP = add16(BP, AX, 0);
L318A:
    /* 318A  adc     cx,dx */
    CX = add16(CX, DX, CF);
L318C:
    /* 318C  jns     short L3191 */
    if (!SF) goto L3191;
L318E:
    /* 318E  jmp     L3090 */
    goto L3090;
L3191: /* L3191 */
    /* 3191  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3192:
    /* 3192  push    si */
    push16(SI);
L3193:
    /* 3193  add     si,ax */
    SI = (uint16_t)(SI + AX);
L3195:
    /* 3195  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3196:
    /* 3196  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3197:
    /* 3197  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x319B)) != 0) return c;
L319B:
    /* 319B  pop     si */
    SI = pop16();
L319C:
    /* 319C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L319D:
    /* 319D  push    si */
    push16(SI);
L319E:
    /* 319E  add     si,ax */
    SI = (uint16_t)(SI + AX);
L31A0:
    /* 31A0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L31A1:
    /* 31A1  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L31A2:
    /* 31A2  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x31A6)) != 0) return c;
L31A6:
    /* 31A6  pop     si */
    SI = pop16();
L31A7:
    /* 31A7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L31A8:
    /* 31A8  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L31A9:
    /* 31A9  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_31AD  (+31AD)
       Opcode 18h, operands: three 32-bit world coordinates. Sets the object origin (2886h..
       288Ah) to the negated position relative to the eye. If a relative coordinate does not fit
       16 bits, sets 2882h = -1 and stops, with the axes before it already updated. Calls
       self_modify. */
L31AD: /* _do_org */
    /* 31AD  mov     word ptr ds:[2882h],0 */
    ww(pDS, 0x2882, 0x0);
L31B3:
    /* 31B3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L31B4:
    /* 31B4  sub     ax,word ptr ds:[EYE_X] */
    AX = sub16(AX, rw(pDS, 0x26BA), 0);
L31B8:
    /* 31B8  mov     bx,word ptr [si] */
    BX = rw(pDS, SI);
L31BA:
    /* 31BA  inc     si */
    SI = inc16(SI);
L31BB:
    /* 31BB  inc     si */
    SI = inc16(SI);
L31BC:
    /* 31BC  sbb     bx,word ptr ds:[EYE_X+2] */
    BX = (uint16_t)(BX - rw(pDS, 0x26BC) - CF);
L31C0:
    /* 31C0  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L31C1:
    /* 31C1  cmp     bx,dx */
    sub16(BX, DX, 0);
L31C3:
    /* 31C3  jne     short L3201 */
    if (!ZF) goto L3201;
L31C5:
    /* 31C5  neg     ax */
    AX = (uint16_t)-AX;
L31C7:
    /* 31C7  mov     word ptr ds:[OBJ_X],ax */
    ww(pDS, 0x2886, AX);
L31CA:
    /* 31CA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L31CB:
    /* 31CB  sub     ax,word ptr ds:[EYE_Y] */
    AX = sub16(AX, rw(pDS, 0x26BE), 0);
L31CF:
    /* 31CF  mov     bx,word ptr [si] */
    BX = rw(pDS, SI);
L31D1:
    /* 31D1  inc     si */
    SI = inc16(SI);
L31D2:
    /* 31D2  inc     si */
    SI = inc16(SI);
L31D3:
    /* 31D3  sbb     bx,word ptr ds:[EYE_Y+2] */
    BX = (uint16_t)(BX - rw(pDS, 0x26C0) - CF);
L31D7:
    /* 31D7  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L31D8:
    /* 31D8  cmp     bx,dx */
    sub16(BX, DX, 0);
L31DA:
    /* 31DA  jne     short L3201 */
    if (!ZF) goto L3201;
L31DC:
    /* 31DC  neg     ax */
    AX = (uint16_t)-AX;
L31DE:
    /* 31DE  mov     word ptr ds:[OBJ_Y],ax */
    ww(pDS, 0x2888, AX);
L31E1:
    /* 31E1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L31E2:
    /* 31E2  sub     ax,word ptr ds:[EYE_Z] */
    AX = sub16(AX, rw(pDS, 0x26C2), 0);
L31E6:
    /* 31E6  mov     bx,word ptr [si] */
    BX = rw(pDS, SI);
L31E8:
    /* 31E8  inc     si */
    SI = inc16(SI);
L31E9:
    /* 31E9  inc     si */
    SI = inc16(SI);
L31EA:
    /* 31EA  sbb     bx,word ptr ds:[EYE_Z+2] */
    BX = (uint16_t)(BX - rw(pDS, 0x26C4) - CF);
L31EE:
    /* 31EE  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L31EF:
    /* 31EF  cmp     bx,dx */
    sub16(BX, DX, 0);
L31F1:
    /* 31F1  jne     short L3201 */
    if (!ZF) goto L3201;
L31F3:
    /* 31F3  neg     ax */
    AX = (uint16_t)-AX;
L31F5:
    /* 31F5  mov     word ptr ds:[OBJ_Z],ax */
    ww(pDS, 0x288A, AX);
L31F8:
    /* 31F8  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x31FB)) != 0) return c;
L31FB:
    /* 31FB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L31FC:
    /* 31FC  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L31FD:
    /* 31FD  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L3201: /* L3201 */
    /* 3201  mov     word ptr ds:[2882h],0FFFFh */
    ww(pDS, 0x2882, 0xFFFF);
L3207:
    /* 3207  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3208:
    /* 3208  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3209:
    /* 3209  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_320D  (+320D)
       Opcode 84h, operands: count, then count points. A visibility test: if all the points share
       a clip-code bit (entirely outside one plane) the program ends here (jumps to do_eof's ret);
       if none has a code, calls set_accept (a no-op in UW2); otherwise continues. */
L320D: /* _do_hull */
    /* 320D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L320E:
    /* 320E  mov     cx,ax */
    CX = AX;
L3210:
    /* 3210  dec     cx */
    CX = (uint16_t)(CX - 1);
L3211:
    /* 3211  mov     bx,PNT_CODES */
    BX = 0x1626;
L3214:
    /* 3214  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3215:
    /* 3215  mov     di,ax */
    DI = AX;
L3217:
    /* 3217  mov     dl,byte ptr [bx+di] */
    DL = rb(pDS, BX + DI);
L3219:
    /* 3219  mov     bp,dx */
    BP = DX;
L321B: /* L321B */
    /* 321B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L321C:
    /* 321C  mov     di,ax */
    DI = AX;
L321E:
    /* 321E  or      dl,byte ptr [bx+di] */
    DL = (uint8_t)(DL | rb(pDS, BX + DI));
L3220:
    /* 3220  and     bp,word ptr [bx+di] */
    BP = logic16((uint16_t)(BP & rw(pDS, BX + DI)));
L3222:
    /* 3222  loop    L321B */
    if (--CX) goto L321B;
L3224:
    /* 3224  je      short L3229 */
    if (ZF) goto L3229;
L3226:
    /* 3226  jmp     _do_eof */
    goto L27D8;
L3229: /* L3229 */
    /* 3229  or      dl,dl */
    DL = logic8((uint8_t)(DL | DL));
L322B:
    /* 322B  jne     short L3232 */
    if (!ZF) goto L3232;
L322D:
    /* 322D  mov     al,0FFh */
    AL = 0xFF;
L322F:
    /* 322F  call    _seg004_5040 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5040), 0x3232)) != 0) return c;
L3232: /* L3232 */
    /* 3232  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3233:
    /* 3233  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3234:
    /* 3234  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3238  (+3238)
       flat_ variant of do_y_rel (opcode 88h when the view is level): the y offset adds straight to
       view y, halved; recomputes the clip code bits 4 and 8 only. */
L3238: /* _flat_y_rel */
    /* 3238  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3239:
    /* 3239  mov     bp,ax */
    BP = AX;
L323B:
    /* 323B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L323C:
    /* 323C  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L3240:
    /* 3240  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L3242:
    /* 3242  sar     ax,1 */
    AX = (uint16_t)((int16_t)AX >> 1);
L3244:
    /* 3244  mov     cx,ax */
    CX = AX;
L3246:
    /* 3246  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3247:
    /* 3247  xchg    bp,si */
    { uint16_t t_ = SI;
    SI = BP;
    BP = t_; }
L3249:
    /* 3249  mov     di,PNT_X */
    DI = 0x1620;
L324C:
    /* 324C  add     si,di */
    SI = (uint16_t)(SI + DI);
L324E:
    /* 324E  add     di,ax */
    DI = (uint16_t)(DI + AX);
L3250:
    /* 3250  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3251:
    /* 3251  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3252:
    /* 3252  add     ax,cx */
    AX = (uint16_t)(AX + CX);
L3254:
    /* 3254  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3255:
    /* 3255  mov     cx,ax */
    CX = AX;
L3257:
    /* 3257  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3258:
    /* 3258  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3259:
    /* 3259  mov     dx,ax */
    DX = AX;
L325B:
    /* 325B  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L325C:
    /* 325C  and     al,0F3h */
    AL = (uint8_t)(AL & 0xF3);
L325E:
    /* 325E  cmp     cx,dx */
    sub16(CX, DX, 0);
L3260:
    /* 3260  jle     short L327C */
    if (ZF || SF != OF) goto L327C;
L3262:
    /* 3262  add     cx,dx */
    CX = add16(CX, DX, 0);
L3264:
    /* 3264  jge     short L3271 */
    if (SF == OF) goto L3271;
L3266:
    /* 3266  or      al,0Ch */
    AL = logic8((uint8_t)(AL | 0xC));
L3268:
    /* 3268  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3269:
    /* 3269  mov     si,bp */
    SI = BP;
L326B:
    /* 326B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L326C:
    /* 326C  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L326D:
    /* 326D  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L3271: /* L3271 */
    /* 3271  or      al,4 */
    AL = logic8((uint8_t)(AL | 0x4));
L3273:
    /* 3273  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3274:
    /* 3274  mov     si,bp */
    SI = BP;
L3276:
    /* 3276  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3277:
    /* 3277  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3278:
    /* 3278  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L327C: /* L327C */
    /* 327C  add     cx,dx */
    CX = add16(CX, DX, 0);
L327E:
    /* 327E  jge     short L3282 */
    if (SF == OF) goto L3282;
L3280:
    /* 3280  or      al,8 */
    AL = logic8((uint8_t)(AL | 0x8));
L3282: /* L3282 */
    /* 3282  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3283:
    /* 3283  mov     si,bp */
    SI = BP;
L3285:
    /* 3285  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3286:
    /* 3286  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3287:
    /* 3287  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_328B  (+328B)
       Opcode 88h (also 1Ch), operands: source point, dy, destination point. New point = source +
       dy along the model's y axis (rotated through the matrix's y row), clip-coded. (System Shock:
       do_y_rel.) The three entries after it are FM's do_animate_p, do_animate_b and
       do_animate_h (same order, five bytes apart; UW2's are seven apart, and UW2's opcode table
       has no entry for them where UW1's has them as opcodes 6Eh, 74h and 76h): each loads BP with
       instance_pitch, instance_bank or instance_head, moves an animated angle variable ([di+2])
       towards its target ([di]) by its rate ([di+4]) times
       the frame time at ss:[9B0h], offsets the origin, and runs the sub-program [di+6] rotated by
       that angle, restoring the matrix and origin afterwards. */
L328B: /* _do_y_rel */
    /* 328B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L328C:
    /* 328C  mov     di,ax */
    DI = AX;
L328E:
    /* 328E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L328F:
    /* 328F  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L3293:
    /* 3293  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L3295:
    /* 3295  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L3299:
    /* 3299  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L329D:
    /* 329D  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L32A1:
    /* 32A1  mov     di,ax */
    DI = AX;
L32A3:
    /* 32A3  imul    word ptr ds:[MAT_YX] */
    imul16(rw(pDS, 0x1608));
L32A7:
    /* 32A7  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L32A9:
    /* 32A9  mov     ax,word ptr ds:[MAT_YY] */
    AX = rw(pDS, 0x160A);
L32AC:
    /* 32AC  imul    di */
    imul16(DI);
L32AE:
    /* 32AE  add     cx,dx */
    CX = (uint16_t)(CX + DX);
L32B0:
    /* 32B0  mov     ax,word ptr ds:[MAT_YZ] */
    AX = rw(pDS, 0x160C);
L32B3:
    /* 32B3  imul    di */
    imul16(DI);
L32B5:
    /* 32B5  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L32B7:
    /* 32B7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L32B8:
    /* 32B8  mov     di,ax */
    DI = AX;
L32BA:
    /* 32BA  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L32BE:
    /* 32BE  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L32C2:
    /* 32C2  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L32C6:
    /* 32C6  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x32C9)) != 0) return c;
L32C9:
    /* 32C9  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L32CD:
    /* 32CD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L32CE:
    /* 32CE  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L32CF:
    /* 32CF  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_32D3  (+32D3)
       do_animate_p (FM name), opcode 6Eh in UW1: see do_y_rel's comment. */
L32D3: /* _do_animate_p */
    /* 32D3  mov     bp,offset _instance_pitch */
    BP = 0x558E;
L32D6:
    /* 32D6  jmp     short L32E0 */
    goto L32E0;

    /* seg004_32D8  (+32D8)
       do_animate_b (FM name), opcode 74h in UW1: see do_y_rel's comment. */
L32D8: /* _do_animate_b */
    /* 32D8  mov     bp,offset _instance_bank */
    BP = 0x56F6;
L32DB:
    /* 32DB  jmp     short L32E0 */
    goto L32E0;

    /* seg004_32DD  (+32DD)
       do_animate_h (FM name), opcode 76h in UW1: see do_y_rel's comment. */
L32DD: /* _do_animate_h */
    /* 32DD  mov     bp,offset _instance_head */
    BP = 0x5426;
L32E0: /* L32E0 */
    /* 32E0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L32E1:
    /* 32E1  mov     di,ax */
    DI = AX;
L32E3:
    /* 32E3  mov     bx,word ptr [di+2] */
    BX = rw(pDS, DI + 0x2);
L32E6:
    /* 32E6  cmp     bx,word ptr [di] */
    sub16(BX, rw(pDS, DI), 0);
L32E8:
    /* 32E8  je      short L331C */
    if (ZF) goto L331C;
L32EA:
    /* 32EA  jg      short L3305 */
    if (!ZF && SF == OF) goto L3305;
L32EC:
    /* 32EC  mov     ax,word ptr [di+4] */
    AX = rw(pDS, DI + 0x4);
L32EF:
    /* 32EF  imul    word ptr ss:[9B0h] */
    imul16(rw(pSS, 0x9B0));
L32F4:
    /* 32F4  shl     ax,1 */
    AX = shl16(AX, 1);
L32F6:
    /* 32F6  rcl     dx,1 */
    DX = rcl16(DX, 1);
L32F8:
    /* 32F8  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L32FA:
    /* 32FA  cmp     bx,word ptr [di] */
    sub16(BX, rw(pDS, DI), 0);
L32FC:
    /* 32FC  jle     short L3300 */
    if (ZF || SF != OF) goto L3300;
L32FE:
    /* 32FE  mov     bx,word ptr [di] */
    BX = rw(pDS, DI);
L3300: /* L3300 */
    /* 3300  mov     word ptr [di+2],bx */
    ww(pDS, DI + 0x2, BX);
L3303:
    /* 3303  jmp     short L331C */
    goto L331C;
L3305: /* L3305 */
    /* 3305  mov     ax,word ptr [di+4] */
    AX = rw(pDS, DI + 0x4);
L3308:
    /* 3308  imul    word ptr ss:[9B0h] */
    imul16(rw(pSS, 0x9B0));
L330D:
    /* 330D  shl     ax,1 */
    AX = shl16(AX, 1);
L330F:
    /* 330F  rcl     dx,1 */
    DX = rcl16(DX, 1);
L3311:
    /* 3311  sub     bx,dx */
    BX = (uint16_t)(BX - DX);
L3313:
    /* 3313  cmp     bx,word ptr [di] */
    sub16(BX, rw(pDS, DI), 0);
L3315:
    /* 3315  jge     short L3319 */
    if (SF == OF) goto L3319;
L3317:
    /* 3317  mov     bx,word ptr [di] */
    BX = rw(pDS, DI);
L3319: /* L3319 */
    /* 3319  mov     word ptr [di+2],bx */
    ww(pDS, DI + 0x2, BX);
L331C: /* L331C */
    /* 331C  push    word ptr ds:[OBJ_X] */
    push16(rw(pDS, 0x2886));
L3320:
    /* 3320  push    word ptr ds:[OBJ_Y] */
    push16(rw(pDS, 0x2888));
L3324:
    /* 3324  push    word ptr ds:[OBJ_Z] */
    push16(rw(pDS, 0x288A));
L3328:
    /* 3328  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3329:
    /* 3329  sub     word ptr ds:[OBJ_X],ax */
    ww(pDS, 0x2886, (uint16_t)(rw(pDS, 0x2886) - AX));
L332D:
    /* 332D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L332E:
    /* 332E  sub     word ptr ds:[OBJ_Y],ax */
    ww(pDS, 0x2888, (uint16_t)(rw(pDS, 0x2888) - AX));
L3332:
    /* 3332  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3333:
    /* 3333  sub     word ptr ds:[OBJ_Z],ax */
    ww(pDS, 0x288A, (uint16_t)(rw(pDS, 0x288A) - AX));
L3337:
    /* 3337  push    si */
    push16(SI);
L3338:
    /* 3338  mov     si,word ptr [di+6] */
    SI = rw(pDS, DI + 0x6);
L333B:
    /* 333B  or      bx,bx */
    BX = logic16((uint16_t)(BX | BX));
L333D:
    /* 333D  je      short L33AD */
    if (ZF) goto L33AD;
L333F:
    /* 333F  push    word ptr ds:[MAT_XX] */
    push16(rw(pDS, 0x1602));
L3343:
    /* 3343  push    word ptr ds:[MAT_YX] */
    push16(rw(pDS, 0x1608));
L3347:
    /* 3347  push    word ptr ds:[MAT_ZX] */
    push16(rw(pDS, 0x160E));
L334B:
    /* 334B  push    word ptr ds:[MAT_XY] */
    push16(rw(pDS, 0x1604));
L334F:
    /* 334F  push    word ptr ds:[MAT_YY] */
    push16(rw(pDS, 0x160A));
L3353:
    /* 3353  push    word ptr ds:[MAT_ZY] */
    push16(rw(pDS, 0x1610));
L3357:
    /* 3357  push    word ptr ds:[MAT_XZ] */
    push16(rw(pDS, 0x1606));
L335B:
    /* 335B  push    word ptr ds:[MAT_YZ] */
    push16(rw(pDS, 0x160C));
L335F:
    /* 335F  push    word ptr ds:[MAT_ZZ] */
    push16(rw(pDS, 0x1612));
L3363:
    /* 3363  call    bp */
    if ((c = asm_call(ASM_JMP(0x06E7, BP), 0x3365)) != 0) return c;
L3365:
    /* 3365  push    si */
    push16(SI);
L3366:
    /* 3366  call    _check_flat */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x4E76), 0x3369)) != 0) return c;
L3369:
    /* 3369  pop     si */
    SI = pop16();
L336A:
    /* 336A  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x336D)) != 0) return c;
L336D:
    /* 336D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L336E:
    /* 336E  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L336F:
    /* 336F  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x3373)) != 0) return c;
L3373:
    /* 3373  pop     word ptr ds:[MAT_ZZ] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1612, t_); }
L3377:
    /* 3377  pop     word ptr ds:[MAT_YZ] */
    { uint16_t t_ = pop16(); ww(pDS, 0x160C, t_); }
L337B:
    /* 337B  pop     word ptr ds:[MAT_XZ] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1606, t_); }
L337F:
    /* 337F  pop     word ptr ds:[MAT_ZY] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1610, t_); }
L3383:
    /* 3383  pop     word ptr ds:[MAT_YY] */
    { uint16_t t_ = pop16(); ww(pDS, 0x160A, t_); }
L3387:
    /* 3387  pop     word ptr ds:[MAT_XY] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1604, t_); }
L338B:
    /* 338B  pop     word ptr ds:[MAT_ZX] */
    { uint16_t t_ = pop16(); ww(pDS, 0x160E, t_); }
L338F:
    /* 338F  pop     word ptr ds:[MAT_YX] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1608, t_); }
L3393:
    /* 3393  pop     word ptr ds:[MAT_XX] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1602, t_); }
L3397:
    /* 3397  pop     si */
    SI = pop16();
L3398:
    /* 3398  pop     word ptr ds:[OBJ_Z] */
    { uint16_t t_ = pop16(); ww(pDS, 0x288A, t_); }
L339C:
    /* 339C  pop     word ptr ds:[OBJ_Y] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2888, t_); }
L33A0:
    /* 33A0  pop     word ptr ds:[OBJ_X] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2886, t_); }
L33A4:
    /* 33A4  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x33A7)) != 0) return c;
L33A7:
    /* 33A7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L33A8:
    /* 33A8  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L33A9:
    /* 33A9  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L33AD: /* L33AD */
    /* 33AD  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x33B0)) != 0) return c;
L33B0:
    /* 33B0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L33B1:
    /* 33B1  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L33B2:
    /* 33B2  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x33B6)) != 0) return c;
L33B6:
    /* 33B6  pop     si */
    SI = pop16();
L33B7:
    /* 33B7  pop     word ptr ds:[OBJ_Z] */
    { uint16_t t_ = pop16(); ww(pDS, 0x288A, t_); }
L33BB:
    /* 33BB  pop     word ptr ds:[OBJ_Y] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2888, t_); }
L33BF:
    /* 33BF  pop     word ptr ds:[OBJ_X] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2886, t_); }
L33C3:
    /* 33C3  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x33C6)) != 0) return c;
L33C6:
    /* 33C6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L33C7:
    /* 33C7  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L33C8:
    /* 33C8  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_33CC  (+33CC)
       Opcode 26h, operands: destination point, source point. Copies the point (7 bytes: x, y, z,
       codes). */
L33CC: /* _do_movres */
    /* 33CC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L33CD:
    /* 33CD  mov     di,ax */
    DI = AX;
L33CF:
    /* 33CF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L33D0:
    /* 33D0  xchg    si,ax */
    { uint16_t t_ = SI;
    SI = AX;
    AX = t_; }
L33D1:
    /* 33D1  add     di,PNT_X */
    DI = (uint16_t)(DI + 0x1620);
L33D5:
    /* 33D5  add     si,PNT_X */
    SI = add16(SI, 0x1620, 0);
L33D9:
    /* 33D9  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L33DA:
    /* 33DA  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L33DB:
    /* 33DB  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L33DC:
    /* 33DC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L33DD:
    /* 33DD  mov     si,ax */
    SI = AX;
L33DF:
    /* 33DF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L33E0:
    /* 33E0  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L33E1:
    /* 33E1  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_33E5  (+33E5)
       flat_ variant of do_x_rel: with the view level only the matrix's x and z terms are used and
       the clip code is recomputed inline. */
L33E5: /* _flat_x_rel */
    /* 33E5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L33E6:
    /* 33E6  mov     di,ax */
    DI = AX;
L33E8:
    /* 33E8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L33E9:
    /* 33E9  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L33ED:
    /* 33ED  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L33EF:
    /* 33EF  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L33F3:
    /* 33F3  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L33F7:
    /* 33F7  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L33FB:
    /* 33FB  mov     di,ax */
    DI = AX;
L33FD:
    /* 33FD  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L3401:
    /* 3401  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L3403:
    /* 3403  mov     ax,di */
    AX = DI;
L3405:
    /* 3405  imul    word ptr ds:[MAT_XZ] */
    imul16(rw(pDS, 0x1606));
L3409:
    /* 3409  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L340A:
    /* 340A  mov     di,ax */
    DI = AX;
L340C:
    /* 340C  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L3410:
    /* 3410  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L3414:
    /* 3414  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L3416:
    /* 3416  add     bp,dx */
    BP = add16(BP, DX, 0);
L3418:
    /* 3418  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L341C:
    /* 341C  jg      short L3420 */
    if (!ZF && SF == OF) goto L3420;
L341E:
    /* 341E  mov     al,80h */
    AL = 0x80;
L3420: /* L3420 */
    /* 3420  cmp     bx,bp */
    sub16(BX, BP, 0);
L3422:
    /* 3422  jle     short L3426 */
    if (ZF || SF != OF) goto L3426;
L3424:
    /* 3424  inc     ax */
    AX = (uint16_t)(AX + 1);
L3425:
    /* 3425  inc     ax */
    AX = (uint16_t)(AX + 1);
L3426: /* L3426 */
    /* 3426  cmp     cx,bp */
    sub16(CX, BP, 0);
L3428:
    /* 3428  jle     short L342C */
    if (ZF || SF != OF) goto L342C;
L342A:
    /* 342A  add     al,4 */
    AL = (uint8_t)(AL + 0x4);
L342C: /* L342C */
    /* 342C  add     bx,bp */
    BX = add16(BX, BP, 0);
L342E:
    /* 342E  jge     short L3431 */
    if (SF == OF) goto L3431;
L3430:
    /* 3430  inc     ax */
    AX = (uint16_t)(AX + 1);
L3431: /* L3431 */
    /* 3431  add     cx,bp */
    CX = add16(CX, BP, 0);
L3433:
    /* 3433  jge     short L3437 */
    if (SF == OF) goto L3437;
L3435:
    /* 3435  add     al,8 */
    AL = add8(AL, 0x8, 0);
L3437: /* L3437 */
    /* 3437  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L343B:
    /* 343B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L343C:
    /* 343C  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L343D:
    /* 343D  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3441  (+3441)
       Opcodes 86h and 2Ah, operands: source point, dx, destination point. New point = source +
       dx along the model's x axis, clip-coded. (System Shock: do_x_rel.) */
L3441: /* _do_x_rel */
    /* 3441  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3442:
    /* 3442  mov     di,ax */
    DI = AX;
L3444:
    /* 3444  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3445:
    /* 3445  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L3449:
    /* 3449  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L344B:
    /* 344B  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L344F:
    /* 344F  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L3453:
    /* 3453  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L3457:
    /* 3457  mov     di,ax */
    DI = AX;
L3459:
    /* 3459  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L345D:
    /* 345D  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L345F:
    /* 345F  mov     ax,word ptr ds:[MAT_XY] */
    AX = rw(pDS, 0x1604);
L3462:
    /* 3462  imul    di */
    imul16(DI);
L3464:
    /* 3464  add     cx,dx */
    CX = (uint16_t)(CX + DX);
L3466:
    /* 3466  mov     ax,word ptr ds:[MAT_XZ] */
    AX = rw(pDS, 0x1606);
L3469:
    /* 3469  imul    di */
    imul16(DI);
L346B:
    /* 346B  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L346D:
    /* 346D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L346E:
    /* 346E  mov     di,ax */
    DI = AX;
L3470:
    /* 3470  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L3474:
    /* 3474  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L3478:
    /* 3478  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L347C:
    /* 347C  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x347F)) != 0) return c;
L347F:
    /* 347F  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L3483:
    /* 3483  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3484:
    /* 3484  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3485:
    /* 3485  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3489  (+3489)
       flat_ variant of do_z_rel. */
L3489: /* _flat_z_rel */
    /* 3489  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L348A:
    /* 348A  mov     di,ax */
    DI = AX;
L348C:
    /* 348C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L348D:
    /* 348D  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L3491:
    /* 3491  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L3493:
    /* 3493  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L3497:
    /* 3497  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L349B:
    /* 349B  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L349F:
    /* 349F  mov     di,ax */
    DI = AX;
L34A1:
    /* 34A1  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L34A5:
    /* 34A5  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L34A7:
    /* 34A7  mov     ax,word ptr ds:[MAT_ZZ] */
    AX = rw(pDS, 0x1612);
L34AA:
    /* 34AA  imul    di */
    imul16(DI);
L34AC:
    /* 34AC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L34AD:
    /* 34AD  mov     di,ax */
    DI = AX;
L34AF:
    /* 34AF  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L34B3:
    /* 34B3  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L34B7:
    /* 34B7  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L34B9:
    /* 34B9  add     bp,dx */
    BP = add16(BP, DX, 0);
L34BB:
    /* 34BB  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L34BF:
    /* 34BF  jg      short L34C3 */
    if (!ZF && SF == OF) goto L34C3;
L34C1:
    /* 34C1  mov     al,80h */
    AL = 0x80;
L34C3: /* L34C3 */
    /* 34C3  cmp     bx,bp */
    sub16(BX, BP, 0);
L34C5:
    /* 34C5  jle     short L34C9 */
    if (ZF || SF != OF) goto L34C9;
L34C7:
    /* 34C7  inc     ax */
    AX = (uint16_t)(AX + 1);
L34C8:
    /* 34C8  inc     ax */
    AX = (uint16_t)(AX + 1);
L34C9: /* L34C9 */
    /* 34C9  cmp     cx,bp */
    sub16(CX, BP, 0);
L34CB:
    /* 34CB  jle     short L34CF */
    if (ZF || SF != OF) goto L34CF;
L34CD:
    /* 34CD  add     al,4 */
    AL = (uint8_t)(AL + 0x4);
L34CF: /* L34CF */
    /* 34CF  add     bx,bp */
    BX = add16(BX, BP, 0);
L34D1:
    /* 34D1  jge     short L34D4 */
    if (SF == OF) goto L34D4;
L34D3:
    /* 34D3  inc     ax */
    AX = (uint16_t)(AX + 1);
L34D4: /* L34D4 */
    /* 34D4  add     cx,bp */
    CX = add16(CX, BP, 0);
L34D6:
    /* 34D6  jge     short L34DA */
    if (SF == OF) goto L34DA;
L34D8:
    /* 34D8  add     al,8 */
    AL = add8(AL, 0x8, 0);
L34DA: /* L34DA */
    /* 34DA  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L34DE:
    /* 34DE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L34DF:
    /* 34DF  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L34E0:
    /* 34E0  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_34E4  (+34E4)
       Opcodes 8Ah and 2Ch, operands: source point, dz, destination point. As do_x_rel along z. */
L34E4: /* _do_z_rel */
    /* 34E4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L34E5:
    /* 34E5  mov     di,ax */
    DI = AX;
L34E7:
    /* 34E7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L34E8:
    /* 34E8  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L34EC:
    /* 34EC  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L34EE:
    /* 34EE  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L34F2:
    /* 34F2  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L34F6:
    /* 34F6  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L34FA:
    /* 34FA  mov     di,ax */
    DI = AX;
L34FC:
    /* 34FC  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L3500:
    /* 3500  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L3502:
    /* 3502  mov     ax,word ptr ds:[MAT_ZY] */
    AX = rw(pDS, 0x1610);
L3505:
    /* 3505  imul    di */
    imul16(DI);
L3507:
    /* 3507  add     cx,dx */
    CX = (uint16_t)(CX + DX);
L3509:
    /* 3509  mov     ax,word ptr ds:[MAT_ZZ] */
    AX = rw(pDS, 0x1612);
L350C:
    /* 350C  imul    di */
    imul16(DI);
L350E:
    /* 350E  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L3510:
    /* 3510  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3511:
    /* 3511  mov     di,ax */
    DI = AX;
L3513:
    /* 3513  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L3517:
    /* 3517  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L351B:
    /* 351B  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L351F:
    /* 351F  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x3522)) != 0) return c;
L3522:
    /* 3522  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L3526:
    /* 3526  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3527:
    /* 3527  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3528:
    /* 3528  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_352C  (+352C)
       Opcode 92h, operands: dx, dz, source point, destination point. Offset along two axes. */
L352C: /* _do_xz_rel */
    /* 352C  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L3530:
    /* 3530  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3531:
    /* 3531  shl     ax,cl */
    AX = shl16(AX, CL);
L3533:
    /* 3533  mov     di,ax */
    DI = AX;
L3535:
    /* 3535  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L3539:
    /* 3539  mov     bx,dx */
    BX = DX;
L353B:
    /* 353B  mov     ax,word ptr ds:[MAT_XZ] */
    AX = rw(pDS, 0x1606);
L353E:
    /* 353E  imul    di */
    imul16(DI);
L3540:
    /* 3540  mov     bp,dx */
    BP = DX;
L3542:
    /* 3542  mov     ax,word ptr ds:[MAT_XY] */
    AX = rw(pDS, 0x1604);
L3545:
    /* 3545  imul    di */
    imul16(DI);
L3547:
    /* 3547  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3548:
    /* 3548  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L354A:
    /* 354A  mov     cx,dx */
    CX = DX;
L354C:
    /* 354C  mov     di,ax */
    DI = AX;
L354E:
    /* 354E  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L3552:
    /* 3552  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L3554:
    /* 3554  mov     ax,word ptr ds:[MAT_ZY] */
    AX = rw(pDS, 0x1610);
L3557:
    /* 3557  imul    di */
    imul16(DI);
L3559:
    /* 3559  add     cx,dx */
    CX = (uint16_t)(CX + DX);
L355B:
    /* 355B  mov     ax,word ptr ds:[MAT_ZZ] */
    AX = rw(pDS, 0x1612);
L355E:
    /* 355E  imul    di */
    imul16(DI);
L3560:
    /* 3560  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L3562:
    /* 3562  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3563:
    /* 3563  mov     di,ax */
    DI = AX;
L3565:
    /* 3565  add     bx,word ptr [di+PNT_X] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x1620));
L3569:
    /* 3569  add     cx,word ptr [di+PNT_Y] */
    CX = (uint16_t)(CX + rw(pDS, DI + 0x1622));
L356D:
    /* 356D  add     bp,word ptr [di+PNT_Z] */
    BP = (uint16_t)(BP + rw(pDS, DI + 0x1624));
L3571:
    /* 3571  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3572:
    /* 3572  mov     di,ax */
    DI = AX;
L3574:
    /* 3574  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L3578:
    /* 3578  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L357C:
    /* 357C  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L3580:
    /* 3580  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x3583)) != 0) return c;
L3583:
    /* 3583  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L3587:
    /* 3587  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3588:
    /* 3588  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3589:
    /* 3589  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_358D  (+358D)
       Opcode 90h, operands: dx, dy, source point, destination point. */
L358D: /* _do_xy_rel */
    /* 358D  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L3591:
    /* 3591  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3592:
    /* 3592  shl     ax,cl */
    AX = shl16(AX, CL);
L3594:
    /* 3594  mov     di,ax */
    DI = AX;
L3596:
    /* 3596  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L359A:
    /* 359A  mov     bx,dx */
    BX = DX;
L359C:
    /* 359C  mov     ax,word ptr ds:[MAT_XZ] */
    AX = rw(pDS, 0x1606);
L359F:
    /* 359F  imul    di */
    imul16(DI);
L35A1:
    /* 35A1  mov     bp,dx */
    BP = DX;
L35A3:
    /* 35A3  mov     ax,word ptr ds:[MAT_XY] */
    AX = rw(pDS, 0x1604);
L35A6:
    /* 35A6  imul    di */
    imul16(DI);
L35A8:
    /* 35A8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L35A9:
    /* 35A9  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L35AB:
    /* 35AB  mov     cx,dx */
    CX = DX;
L35AD:
    /* 35AD  mov     di,ax */
    DI = AX;
L35AF:
    /* 35AF  imul    word ptr ds:[MAT_YX] */
    imul16(rw(pDS, 0x1608));
L35B3:
    /* 35B3  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L35B5:
    /* 35B5  mov     ax,word ptr ds:[MAT_YY] */
    AX = rw(pDS, 0x160A);
L35B8:
    /* 35B8  imul    di */
    imul16(DI);
L35BA:
    /* 35BA  add     cx,dx */
    CX = (uint16_t)(CX + DX);
L35BC:
    /* 35BC  mov     ax,word ptr ds:[MAT_YZ] */
    AX = rw(pDS, 0x160C);
L35BF:
    /* 35BF  imul    di */
    imul16(DI);
L35C1:
    /* 35C1  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L35C3:
    /* 35C3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L35C4:
    /* 35C4  mov     di,ax */
    DI = AX;
L35C6:
    /* 35C6  add     bx,word ptr [di+PNT_X] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x1620));
L35CA:
    /* 35CA  add     cx,word ptr [di+PNT_Y] */
    CX = (uint16_t)(CX + rw(pDS, DI + 0x1622));
L35CE:
    /* 35CE  add     bp,word ptr [di+PNT_Z] */
    BP = (uint16_t)(BP + rw(pDS, DI + 0x1624));
L35D2:
    /* 35D2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L35D3:
    /* 35D3  mov     di,ax */
    DI = AX;
L35D5:
    /* 35D5  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L35D9:
    /* 35D9  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L35DD:
    /* 35DD  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L35E1:
    /* 35E1  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x35E4)) != 0) return c;
L35E4:
    /* 35E4  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L35E8:
    /* 35E8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L35E9:
    /* 35E9  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L35EA:
    /* 35EA  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_35EE  (+35EE)
       Opcode 94h, operands: dy, dz, source point, destination point. (Note the operand order: the
       first operand is multiplied by the z row and the second by the y row, as in the code.) */
L35EE: /* _do_yz_rel */
    /* 35EE  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L35F2:
    /* 35F2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L35F3:
    /* 35F3  shl     ax,cl */
    AX = shl16(AX, CL);
L35F5:
    /* 35F5  mov     di,ax */
    DI = AX;
L35F7:
    /* 35F7  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L35FB:
    /* 35FB  mov     bx,dx */
    BX = DX;
L35FD:
    /* 35FD  mov     ax,word ptr ds:[MAT_ZZ] */
    AX = rw(pDS, 0x1612);
L3600:
    /* 3600  imul    di */
    imul16(DI);
L3602:
    /* 3602  mov     bp,dx */
    BP = DX;
L3604:
    /* 3604  mov     ax,word ptr ds:[MAT_ZY] */
    AX = rw(pDS, 0x1610);
L3607:
    /* 3607  imul    di */
    imul16(DI);
L3609:
    /* 3609  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L360A:
    /* 360A  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L360C:
    /* 360C  mov     cx,dx */
    CX = DX;
L360E:
    /* 360E  mov     di,ax */
    DI = AX;
L3610:
    /* 3610  imul    word ptr ds:[MAT_YX] */
    imul16(rw(pDS, 0x1608));
L3614:
    /* 3614  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L3616:
    /* 3616  mov     ax,word ptr ds:[MAT_YY] */
    AX = rw(pDS, 0x160A);
L3619:
    /* 3619  imul    di */
    imul16(DI);
L361B:
    /* 361B  add     cx,dx */
    CX = (uint16_t)(CX + DX);
L361D:
    /* 361D  mov     ax,word ptr ds:[MAT_YZ] */
    AX = rw(pDS, 0x160C);
L3620:
    /* 3620  imul    di */
    imul16(DI);
L3622:
    /* 3622  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L3624:
    /* 3624  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3625:
    /* 3625  mov     di,ax */
    DI = AX;
L3627:
    /* 3627  add     bx,word ptr [di+PNT_X] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x1620));
L362B:
    /* 362B  add     cx,word ptr [di+PNT_Y] */
    CX = (uint16_t)(CX + rw(pDS, DI + 0x1622));
L362F:
    /* 362F  add     bp,word ptr [di+PNT_Z] */
    BP = (uint16_t)(BP + rw(pDS, DI + 0x1624));
L3633:
    /* 3633  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3634:
    /* 3634  mov     di,ax */
    DI = AX;
L3636:
    /* 3636  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L363A:
    /* 363A  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L363E:
    /* 363E  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L3642:
    /* 3642  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x3645)) != 0) return c;
L3645:
    /* 3645  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L3649:
    /* 3649  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L364A:
    /* 364A  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L364B:
    /* 364B  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_364F  (+364F)
       flat_ variant of do_xz_rel. */
L364F: /* _flat_xz_rel */
    /* 364F  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L3653:
    /* 3653  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3654:
    /* 3654  shl     ax,cl */
    AX = shl16(AX, CL);
L3656:
    /* 3656  mov     di,ax */
    DI = AX;
L3658:
    /* 3658  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L365C:
    /* 365C  mov     bx,dx */
    BX = DX;
L365E:
    /* 365E  mov     ax,word ptr ds:[MAT_XZ] */
    AX = rw(pDS, 0x1606);
L3661:
    /* 3661  imul    di */
    imul16(DI);
L3663:
    /* 3663  mov     bp,dx */
    BP = DX;
L3665:
    /* 3665  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3666:
    /* 3666  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L3668:
    /* 3668  mov     di,ax */
    DI = AX;
L366A:
    /* 366A  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L366E:
    /* 366E  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L3670:
    /* 3670  mov     ax,word ptr ds:[MAT_ZZ] */
    AX = rw(pDS, 0x1612);
L3673:
    /* 3673  imul    di */
    imul16(DI);
L3675:
    /* 3675  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L3677:
    /* 3677  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3678:
    /* 3678  mov     di,ax */
    DI = AX;
L367A:
    /* 367A  add     bx,word ptr [di+PNT_X] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x1620));
L367E:
    /* 367E  add     bp,word ptr [di+PNT_Z] */
    BP = (uint16_t)(BP + rw(pDS, DI + 0x1624));
L3682:
    /* 3682  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L3686:
    /* 3686  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3687:
    /* 3687  mov     di,ax */
    DI = AX;
L3689:
    /* 3689  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L368D:
    /* 368D  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L3691:
    /* 3691  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L3695:
    /* 3695  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x3698)) != 0) return c;
L3698:
    /* 3698  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L369C:
    /* 369C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L369D:
    /* 369D  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L369E:
    /* 369E  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_36A2  (+36A2)
       flat_ variant of do_xy_rel. */
L36A2: /* _flat_xy_rel */
    /* 36A2  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L36A6:
    /* 36A6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L36A7:
    /* 36A7  shl     ax,cl */
    AX = shl16(AX, CL);
L36A9:
    /* 36A9  mov     di,ax */
    DI = AX;
L36AB:
    /* 36AB  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L36AF:
    /* 36AF  mov     bx,dx */
    BX = DX;
L36B1:
    /* 36B1  mov     ax,word ptr ds:[MAT_XZ] */
    AX = rw(pDS, 0x1606);
L36B4:
    /* 36B4  imul    di */
    imul16(DI);
L36B6:
    /* 36B6  mov     bp,dx */
    BP = DX;
L36B8:
    /* 36B8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L36B9:
    /* 36B9  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L36BB:
    /* 36BB  sar     ax,1 */
    AX = (uint16_t)((int16_t)AX >> 1);
L36BD:
    /* 36BD  mov     cx,ax */
    CX = AX;
L36BF:
    /* 36BF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L36C0:
    /* 36C0  mov     di,ax */
    DI = AX;
L36C2:
    /* 36C2  add     bx,word ptr [di+PNT_X] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x1620));
L36C6:
    /* 36C6  add     cx,word ptr [di+PNT_Y] */
    CX = (uint16_t)(CX + rw(pDS, DI + 0x1622));
L36CA:
    /* 36CA  add     bp,word ptr [di+PNT_Z] */
    BP = (uint16_t)(BP + rw(pDS, DI + 0x1624));
L36CE:
    /* 36CE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L36CF:
    /* 36CF  mov     di,ax */
    DI = AX;
L36D1:
    /* 36D1  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L36D5:
    /* 36D5  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L36D9:
    /* 36D9  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L36DD:
    /* 36DD  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x36E0)) != 0) return c;
L36E0:
    /* 36E0  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L36E4:
    /* 36E4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L36E5:
    /* 36E5  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L36E6:
    /* 36E6  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_36EA  (+36EA)
       flat_ variant of do_yz_rel. */
L36EA: /* _flat_yz_rel */
    /* 36EA  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L36EE:
    /* 36EE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L36EF:
    /* 36EF  shl     ax,cl */
    AX = shl16(AX, CL);
L36F1:
    /* 36F1  mov     di,ax */
    DI = AX;
L36F3:
    /* 36F3  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L36F7:
    /* 36F7  mov     bx,dx */
    BX = DX;
L36F9:
    /* 36F9  mov     ax,word ptr ds:[MAT_ZZ] */
    AX = rw(pDS, 0x1612);
L36FC:
    /* 36FC  imul    di */
    imul16(DI);
L36FE:
    /* 36FE  mov     bp,dx */
    BP = DX;
L3700:
    /* 3700  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3701:
    /* 3701  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L3703:
    /* 3703  sar     ax,1 */
    AX = (uint16_t)((int16_t)AX >> 1);
L3705:
    /* 3705  mov     cx,ax */
    CX = AX;
L3707:
    /* 3707  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3708:
    /* 3708  mov     di,ax */
    DI = AX;
L370A:
    /* 370A  add     bx,word ptr [di+PNT_X] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x1620));
L370E:
    /* 370E  add     cx,word ptr [di+PNT_Y] */
    CX = (uint16_t)(CX + rw(pDS, DI + 0x1622));
L3712:
    /* 3712  add     bp,word ptr [di+PNT_Z] */
    BP = (uint16_t)(BP + rw(pDS, DI + 0x1624));
L3716:
    /* 3716  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3717:
    /* 3717  mov     di,ax */
    DI = AX;
L3719:
    /* 3719  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L371D:
    /* 371D  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L3721:
    /* 3721  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L3725:
    /* 3725  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x3728)) != 0) return c;
L3728:
    /* 3728  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L372C:
    /* 372C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L372D:
    /* 372D  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L372E:
    /* 372E  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3732  (+3732)
       Opcode 2Eh, operand: colour. Selects flat filling (15F6h = 438Dh, 15F8h = 4134h, seg003
       offsets) and sets the colour through seg003's _seg003_5B65 (with DS = seg048). Its
       second label, setcol_common (FM), is shared by do_setcolv, do_uwsurf and PGCACHE. */
L3732: /* _do_setcolor */
    /* 3732  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3733: /* setcol_common */
    /* 3733  mov     bx,438Dh */
    BX = 0x438D;
L3736:
    /* 3736  mov     word ptr ds:[POLY_FILL],bx */
    ww(pDS, 0x15F6, BX);
L373A:
    /* 373A  mov     bx,4134h */
    BX = 0x4134;
L373D:
    /* 373D  mov     word ptr ds:[POLY_UFILL],bx */
    ww(pDS, 0x15F8, BX);
L3741:
    /* 3741  mov     bx,seg seg048 */
    BX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L3744:
    /* 3744  mov     ds,bx */
    SET_DS(BX);
L3746:
    /* 3746  call    far ptr _seg003_5B65 */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x5B65), 0x06E7 + PORT_LOAD_SEG, 0x374B)) != 0) return c;
L374B:
    /* 374B  mov     bp,seg seg051 */
    BP = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L374E:
    /* 374E  mov     ds,bp */
    SET_DS(BP);
L3750:
    /* 3750  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3751:
    /* 3751  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3752:
    /* 3752  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3756  (+3756)
       Opcode 5Ch, operands: address, colour. Colour = [address] + colour, then setcol_common. */
L3756: /* _do_setcolv */
    /* 3756  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3757:
    /* 3757  mov     di,ax */
    DI = AX;
L3759:
    /* 3759  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L375A:
    /* 375A  add     ax,word ptr [di] */
    AX = (uint16_t)(AX + rw(pDS, DI));
L375C:
    /* 375C  jmp     setcol_common */
    goto L3733;

    /* seg004_375E  (+375E)
       UW1 only: opcode 24h. Takes two points and a step count from its operands and sets up
       per-step increments at 26ECh..26F6h (and 286Dh from the first point's clip codes); not
       traced further. */
L375E: /* _seg004_375E */
    /* 375E  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L3760:
    /* 3760  mov     di,26FAh */
    DI = 0x26FA;
L3763:
    /* 3763  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3764:
    /* 3764  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3765:
    /* 3765  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3766:
    /* 3766  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3767:
    /* 3767  mov     di,ax */
    DI = AX;
L3769:
    /* 3769  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L376D:
    /* 376D  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L3771:
    /* 3771  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L3775:
    /* 3775  mov     al,byte ptr [di+PNT_CODES] */
    AL = rb(pDS, DI + 0x1626);
L3779:
    /* 3779  mov     byte ptr ds:[286Dh],al */
    wb(pDS, 0x286D, AL);
L377C:
    /* 377C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L377D:
    /* 377D  mov     di,ax */
    DI = AX;
L377F:
    /* 377F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3780:
    /* 3780  mov     word ptr ds:[26F8h],ax */
    ww(pDS, 0x26F8, AX);
L3783:
    /* 3783  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3784:
    /* 3784  push    si */
    push16(SI);
L3785:
    /* 3785  mov     si,ax */
    SI = AX;
L3787:
    /* 3787  mov     ax,word ptr [di+PNT_X] */
    AX = rw(pDS, DI + 0x1620);
L378B:
    /* 378B  sub     ax,bx */
    AX = (uint16_t)(AX - BX);
L378D:
    /* 378D  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L378E:
    /* 378E  idiv    si */
    if (asm_idiv16(SI) && (c = asm_divfault(0x06E7, 0x378E, 2)) != 0) return c;
L3790:
    /* 3790  mov     word ptr ds:[26ECh],ax */
    ww(pDS, 0x26EC, AX);
L3793:
    /* 3793  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L3795:
    /* 3795  idiv    si */
    if (asm_idiv16(SI) && (c = asm_divfault(0x06E7, 0x3795, 2)) != 0) return c;
L3797:
    /* 3797  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L3798:
    /* 3798  mov     word ptr ds:[26EEh],ax */
    ww(pDS, 0x26EE, AX);
L379B:
    /* 379B  add     word ptr ds:[26ECh],dx */
    ww(pDS, 0x26EC, (uint16_t)(rw(pDS, 0x26EC) + DX));
L379F:
    /* 379F  mov     ax,word ptr [di+PNT_Z] */
    AX = rw(pDS, DI + 0x1624);
L37A3:
    /* 37A3  sub     ax,di */
    AX = (uint16_t)(AX - DI);
L37A5:
    /* 37A5  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L37A6:
    /* 37A6  idiv    si */
    if (asm_idiv16(SI) && (c = asm_divfault(0x06E7, 0x37A6, 2)) != 0) return c;
L37A8:
    /* 37A8  mov     word ptr ds:[26F4h],ax */
    ww(pDS, 0x26F4, AX);
L37AB:
    /* 37AB  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L37AD:
    /* 37AD  idiv    si */
    if (asm_idiv16(SI) && (c = asm_divfault(0x06E7, 0x37AD, 2)) != 0) return c;
L37AF:
    /* 37AF  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L37B0:
    /* 37B0  mov     word ptr ds:[26F6h],ax */
    ww(pDS, 0x26F6, AX);
L37B3:
    /* 37B3  add     word ptr ds:[26F4h],dx */
    ww(pDS, 0x26F4, (uint16_t)(rw(pDS, 0x26F4) + DX));
L37B7:
    /* 37B7  mov     ax,word ptr [di+PNT_Y] */
    AX = rw(pDS, DI + 0x1622);
L37BB:
    /* 37BB  sub     ax,cx */
    AX = (uint16_t)(AX - CX);
L37BD:
    /* 37BD  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L37BE:
    /* 37BE  idiv    si */
    if (asm_idiv16(SI) && (c = asm_divfault(0x06E7, 0x37BE, 2)) != 0) return c;
L37C0:
    /* 37C0  mov     word ptr ds:[26F0h],ax */
    ww(pDS, 0x26F0, AX);
L37C3:
    /* 37C3  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L37C5:
    /* 37C5  idiv    si */
    if (asm_idiv16(SI) && (c = asm_divfault(0x06E7, 0x37C5, 2)) != 0) return c;
L37C7:
    /* 37C7  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L37C8:
    /* 37C8  mov     word ptr ds:[26F2h],ax */
    ww(pDS, 0x26F2, AX);
L37CB:
    /* 37CB  add     word ptr ds:[26F0h],dx */
    ww(pDS, 0x26F0, (uint16_t)(rw(pDS, 0x26F0) + DX));
L37CF:
    /* 37CF  mov     al,byte ptr [di+PNT_CODES] */
    AL = rb(pDS, DI + 0x1626);
L37D3:
    /* 37D3  cmp     al,byte ptr ds:[286Dh] */
    sub8(AL, rb(pDS, 0x286D), 0);
L37D7:
    /* 37D7  je      short L3833 */
    if (ZF) goto L3833;
L37D9:
    /* 37D9  mov     di,word ptr ds:[26F8h] */
    DI = rw(pDS, 0x26F8);
L37DD: /* L37DD */
    /* 37DD  mov     ax,word ptr ds:[26EEh] */
    AX = rw(pDS, 0x26EE);
L37E0:
    /* 37E0  add     word ptr ds:[26FAh],ax */
    ww(pDS, 0x26FA, add16(rw(pDS, 0x26FA), AX, 0));
L37E4:
    /* 37E4  adc     bx,word ptr ds:[26ECh] */
    BX = (uint16_t)(BX + rw(pDS, 0x26EC) + CF);
L37E8:
    /* 37E8  add     word ptr ds:[26FAh],ax */
    ww(pDS, 0x26FA, add16(rw(pDS, 0x26FA), AX, 0));
L37EC:
    /* 37EC  adc     bx,0 */
    BX = (uint16_t)(BX + 0x0 + CF);
L37EF:
    /* 37EF  mov     ax,word ptr ds:[26F2h] */
    AX = rw(pDS, 0x26F2);
L37F2:
    /* 37F2  add     word ptr ds:[26FCh],ax */
    ww(pDS, 0x26FC, add16(rw(pDS, 0x26FC), AX, 0));
L37F6:
    /* 37F6  adc     cx,word ptr ds:[26F0h] */
    CX = (uint16_t)(CX + rw(pDS, 0x26F0) + CF);
L37FA:
    /* 37FA  add     word ptr ds:[26FCh],ax */
    ww(pDS, 0x26FC, add16(rw(pDS, 0x26FC), AX, 0));
L37FE:
    /* 37FE  adc     cx,0 */
    CX = (uint16_t)(CX + 0x0 + CF);
L3801:
    /* 3801  mov     ax,word ptr ds:[26F6h] */
    AX = rw(pDS, 0x26F6);
L3804:
    /* 3804  add     word ptr ds:[26FEh],ax */
    ww(pDS, 0x26FE, add16(rw(pDS, 0x26FE), AX, 0));
L3808:
    /* 3808  adc     bp,word ptr ds:[26F4h] */
    BP = (uint16_t)(BP + rw(pDS, 0x26F4) + CF);
L380C:
    /* 380C  add     word ptr ds:[26FEh],ax */
    ww(pDS, 0x26FE, add16(rw(pDS, 0x26FE), AX, 0));
L3810:
    /* 3810  adc     bp,0 */
    BP = (uint16_t)(BP + 0x0 + CF);
L3813:
    /* 3813  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x3816)) != 0) return c;
L3816:
    /* 3816  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L381A:
    /* 381A  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L381E:
    /* 381E  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L3822:
    /* 3822  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L3826:
    /* 3826  add     di,8 */
    DI = add16(DI, 0x8, 0);
L3829:
    /* 3829  dec     si */
    SI = dec16(SI);
L382A:
    /* 382A  jne     L37DD */
    if (!ZF) goto L37DD;
L382C:
    /* 382C  pop     si */
    SI = pop16();
L382D:
    /* 382D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L382E:
    /* 382E  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L382F:
    /* 382F  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L3833: /* L3833 */
    /* 3833  mov     di,word ptr ds:[26F8h] */
    DI = rw(pDS, 0x26F8);
L3837:
    /* 3837  mov     dl,al */
    DL = AL;
L3839:
    /* 3839  xchg    si,cx */
    { uint16_t t_ = CX;
    CX = SI;
    SI = t_; }
L383B: /* L383B */
    /* 383B  mov     ax,word ptr ds:[26EEh] */
    AX = rw(pDS, 0x26EE);
L383E:
    /* 383E  add     word ptr ds:[26FAh],ax */
    ww(pDS, 0x26FA, add16(rw(pDS, 0x26FA), AX, 0));
L3842:
    /* 3842  adc     bx,word ptr ds:[26ECh] */
    BX = (uint16_t)(BX + rw(pDS, 0x26EC) + CF);
L3846:
    /* 3846  add     word ptr ds:[26FAh],ax */
    ww(pDS, 0x26FA, add16(rw(pDS, 0x26FA), AX, 0));
L384A:
    /* 384A  adc     bx,0 */
    BX = (uint16_t)(BX + 0x0 + CF);
L384D:
    /* 384D  mov     ax,word ptr ds:[26F2h] */
    AX = rw(pDS, 0x26F2);
L3850:
    /* 3850  add     word ptr ds:[26FCh],ax */
    ww(pDS, 0x26FC, add16(rw(pDS, 0x26FC), AX, 0));
L3854:
    /* 3854  adc     si,word ptr ds:[26F0h] */
    SI = (uint16_t)(SI + rw(pDS, 0x26F0) + CF);
L3858:
    /* 3858  add     word ptr ds:[26FCh],ax */
    ww(pDS, 0x26FC, add16(rw(pDS, 0x26FC), AX, 0));
L385C:
    /* 385C  adc     si,0 */
    SI = (uint16_t)(SI + 0x0 + CF);
L385F:
    /* 385F  mov     ax,word ptr ds:[26F6h] */
    AX = rw(pDS, 0x26F6);
L3862:
    /* 3862  add     word ptr ds:[26FEh],ax */
    ww(pDS, 0x26FE, add16(rw(pDS, 0x26FE), AX, 0));
L3866:
    /* 3866  adc     bp,word ptr ds:[26F4h] */
    BP = (uint16_t)(BP + rw(pDS, 0x26F4) + CF);
L386A:
    /* 386A  add     word ptr ds:[26FEh],ax */
    ww(pDS, 0x26FE, add16(rw(pDS, 0x26FE), AX, 0));
L386E:
    /* 386E  adc     bp,0 */
    BP = (uint16_t)(BP + 0x0 + CF);
L3871:
    /* 3871  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L3875:
    /* 3875  mov     word ptr [di+PNT_Y],si */
    ww(pDS, DI + 0x1622, SI);
L3879:
    /* 3879  mov     word ptr [di+PNT_Z],bx */
    ww(pDS, DI + 0x1624, BX);
L387D:
    /* 387D  mov     byte ptr [di+PNT_CODES],dl */
    wb(pDS, DI + 0x1626, DL);
L3881:
    /* 3881  add     di,8 */
    DI = add16(DI, 0x8, 0);
L3884:
    /* 3884  loop    L383B */
    if (--CX) goto L383B;
L3886:
    /* 3886  pop     si */
    SI = pop16();
L3887:
    /* 3887  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3888:
    /* 3888  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3889:
    /* 3889  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_388D  (+388D)
       Opcode CAh, operand: address. do_goursurf with the colour taken from a model variable: the
       `db 0A8h` turns do_goursurf's lodsw into the operand of a `test al,` so AX is kept. */
L388D: /* _do_goursurfv */
    /* 388D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L388E:
    /* 388E  mov     bx,ax */
    BX = AX;
L3890:
    /* 3890  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L3892:
    /* 3892  db      0A8h */
    goto L3894;

    /* seg004_3893  (+3893)
       Opcode C8h, operand: colour. Selects Gouraud filling (15F6h = _seg003_6146, 15F8h =
       _seg003_6156) and stores the colour's shading-table byte (seg048:21Fh + 2 * colour) at 300h. */
L3893: /* _do_goursurf */
    /* 3893  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3894:
    /* 3894  mov     bx,ax */
    BX = AX;
L3896:
    /* 3896  push    es */
    push16(asm_es);
L3897:
    /* 3897  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L389A:
    /* 389A  mov     es,ax */
    SET_ES(AX);
L389C:
    /* 389C  shl     bx,1 */
    BX = shl16(BX, 1);
L389E:
    /* 389E  mov     al,byte ptr es:[bx+21Fh] */
    AL = rb(pES, BX + 0x21F);
L38A3:
    /* 38A3  pop     es */
    SET_ES(pop16());
L38A4:
    /* 38A4  mov     byte ptr ds:[300h],al */
    wb(pDS, 0x300, AL);
L38A7:
    /* 38A7  mov     word ptr ds:[POLY_FILL],offset _seg003_6146 */
    ww(pDS, 0x15F6, 0x6146);
L38AD:
    /* 38AD  mov     word ptr ds:[POLY_UFILL],offset _seg003_6156 */
    ww(pDS, 0x15F8, 0x6156);
L38B3:
    /* 38B3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L38B4:
    /* 38B4  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L38B5:
    /* 38B5  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_38B9  (+38B9)
       Opcode D6h (UW-specific): the standard surface. When the low byte of 2932h is zero,
       selects Gouraud filling and points seg003's handler pair at 0F27h/0F29h at 0F2Bh/0FF7h;
       otherwise sets the flat colour from seg048:0DC3h through setcol_common. */
L38B9: /* _do_uwsurf */
    /* 38B9  test    word ptr ds:[2932h],0FFh */
    logic16((uint16_t)(rw(pDS, 0x2932) & 0xFF));
L38BF:
    /* 38BF  jne     short L38EA */
    if (!ZF) goto L38EA;
L38C1:
    /* 38C1  mov     word ptr ds:[POLY_FILL],offset _seg003_6146 */
    ww(pDS, 0x15F6, 0x6146);
L38C7:
    /* 38C7  mov     word ptr ds:[POLY_UFILL],offset _seg003_6156 */
    ww(pDS, 0x15F8, 0x6156);
L38CD:
    /* 38CD  mov     bx,es */
    BX = asm_es;
L38CF:
    /* 38CF  mov     ax,seg seg003 */
    AX = (uint16_t)(0x0090 + PORT_LOAD_SEG);
L38D2:
    /* 38D2  mov     es,ax */
    SET_ES(AX);
L38D4:
    /* 38D4  mov     word ptr es:[0F27h],0F2Bh */
    ww(pES, 0xF27, 0xF2B);
L38DB:
    /* 38DB  mov     word ptr es:[0F29h],0FF7h */
    ww(pES, 0xF29, 0xFF7);
L38E2:
    /* 38E2  mov     es,bx */
    SET_ES(BX);
L38E4:
    /* 38E4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L38E5:
    /* 38E5  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L38E6:
    /* 38E6  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L38EA: /* L38EA */
    /* 38EA  mov     bx,es */
    BX = asm_es;
L38EC:
    /* 38EC  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L38EF:
    /* 38EF  mov     es,ax */
    SET_ES(AX);
L38F1:
    /* 38F1  mov     al,byte ptr es:[0DC3h] */
    AL = rb(pES, 0xDC3);
L38F5:
    /* 38F5  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L38F7:
    /* 38F7  mov     es,bx */
    SET_ES(BX);
L38F9:
    /* 38F9  jmp     setcol_common */
    goto L3733;

    /* seg004_38FC  (+38FC)
       Opcode D8h: a translucent surface. Gouraud filling with seg003's handler pair at
       0F27h/0F29h pointed at 0F56h/103Dh (the same pair smooth_over uses). */
L38FC: /* _do_transsurf */
    /* 38FC  mov     word ptr ds:[POLY_FILL],offset _seg003_6146 */
    ww(pDS, 0x15F6, 0x6146);
L3902:
    /* 3902  mov     word ptr ds:[POLY_UFILL],offset _seg003_6156 */
    ww(pDS, 0x15F8, 0x6156);
L3908:
    /* 3908  mov     bx,es */
    BX = asm_es;
L390A:
    /* 390A  mov     ax,seg seg003 */
    AX = (uint16_t)(0x0090 + PORT_LOAD_SEG);
L390D:
    /* 390D  mov     es,ax */
    SET_ES(AX);
L390F:
    /* 390F  mov     word ptr es:[0F27h],0F56h */
    ww(pES, 0xF27, 0xF56);
L3916:
    /* 3916  mov     word ptr es:[0F29h],103Dh */
    ww(pES, 0xF29, 0x103D);
L391D:
    /* 391D  mov     es,bx */
    SET_ES(BX);
L391F:
    /* 391F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3920:
    /* 3920  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3921:
    /* 3921  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3925  (+3925)
       Opcode 44h: selects flat filling (438Dh, 4134h) without setting a colour. */
L3925: /* _do_vexsurf */
    /* 3925  mov     word ptr ds:[POLY_FILL],438Dh */
    ww(pDS, 0x15F6, 0x438D);
L392B:
    /* 392B  mov     word ptr ds:[POLY_UFILL],4134h */
    ww(pDS, 0x15F8, 0x4134);
L3931:
    /* 3931  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3932:
    /* 3932  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3933:
    /* 3933  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3937  (+3937)
       Opcode 40h: selects the fill routine 2E7Ch for both slots (seg003; what it draws is not
       known). After it come FM's do_setpat, do_putpat and do_usepat, in FM's order: UW1's opcode
       table has them as opcodes 3Ch, 98h and 9Ah, where UW2 keeps the same code with no table
       entry. Roughly: do_setpat rotates a list of vectors into a table, and do_putpat and
       do_usepat place points from such a table at a rotated offset (do_putpat then runs a
       sub-program); not traced further. */
L3937: /* _do_cavesurf */
    /* 3937  mov     word ptr ds:[POLY_FILL],2E7Ch */
    ww(pDS, 0x15F6, 0x2E7C);
L393D:
    /* 393D  mov     word ptr ds:[POLY_UFILL],2E7Ch */
    ww(pDS, 0x15F8, 0x2E7C);
L3943:
    /* 3943  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3944:
    /* 3944  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3945:
    /* 3945  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3949  (+3949)
       do_setpat (FM name), opcode 3Ch in UW1: see do_cavesurf's comment. */
L3949: /* _do_setpat */
    /* 3949  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L394A:
    /* 394A  push    si */
    push16(SI);
L394B:
    /* 394B  add     si,ax */
    SI = (uint16_t)(SI + AX);
L394D:
    /* 394D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L394E:
    /* 394E  mov     byte ptr ds:[2872h],al */
    wb(pDS, 0x2872, AL);
L3951:
    /* 3951  inc     si */
    SI = (uint16_t)(SI + 1);
L3952:
    /* 3952  inc     si */
    SI = (uint16_t)(SI + 1);
L3953:
    /* 3953  mov     word ptr ds:[26F8h],si */
    ww(pDS, 0x26F8, SI);
L3957:
    /* 3957  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L3959:
    /* 3959  add     si,ax */
    SI = (uint16_t)(SI + AX);
L395B:
    /* 395B  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L395D:
    /* 395D  add     si,ax */
    SI = (uint16_t)(SI + AX);
L395F: /* L395F */
    /* 395F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3960:
    /* 3960  mov     bx,ax */
    BX = AX;
L3962:
    /* 3962  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3963:
    /* 3963  mov     cx,ax */
    CX = AX;
L3965:
    /* 3965  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3966:
    /* 3966  mov     bp,ax */
    BP = AX;
L3968:
    /* 3968  call    _mxmul */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x50D8), 0x396B)) != 0) return c;
L396B:
    /* 396B  mov     di,word ptr ds:[26F8h] */
    DI = rw(pDS, 0x26F8);
L396F:
    /* 396F  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L3971:
    /* 3971  mov     word ptr [di+4],cx */
    ww(pDS, DI + 0x4, CX);
L3974:
    /* 3974  mov     word ptr [di+2],bp */
    ww(pDS, DI + 0x2, BP);
L3977:
    /* 3977  add     word ptr ds:[26F8h],6 */
    ww(pDS, 0x26F8, add16(rw(pDS, 0x26F8), 0x6, 0));
L397C:
    /* 397C  dec     byte ptr ds:[2872h] */
    wb(pDS, 0x2872, dec8(rb(pDS, 0x2872)));
L3980:
    /* 3980  jne     L395F */
    if (!ZF) goto L395F;
L3982:
    /* 3982  pop     si */
    SI = pop16();
L3983:
    /* 3983  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3984:
    /* 3984  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3985:
    /* 3985  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3989  (+3989)
       do_putpat (FM name), opcode 98h in UW1: see do_cavesurf's comment. */
L3989: /* _do_putpat */
    /* 3989  push    word ptr ds:[OBJ_X] */
    push16(rw(pDS, 0x2886));
L398D:
    /* 398D  push    word ptr ds:[OBJ_Y] */
    push16(rw(pDS, 0x2888));
L3991:
    /* 3991  push    word ptr ds:[OBJ_Z] */
    push16(rw(pDS, 0x288A));
L3995:
    /* 3995  call    _load_xlate_rotate_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x50BE), 0x3998)) != 0) return c;
L3998:
    /* 3998  mov     word ptr ds:[15FCh],bx */
    ww(pDS, 0x15FC, BX);
L399C:
    /* 399C  mov     word ptr ds:[15FEh],cx */
    ww(pDS, 0x15FE, CX);
L39A0:
    /* 39A0  mov     word ptr ds:[1600h],bp */
    ww(pDS, 0x1600, BP);
L39A4:
    /* 39A4  mov     ax,word ptr ds:[OBJ_X] */
    AX = rw(pDS, 0x2886);
L39A7:
    /* 39A7  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L39AA:
    /* 39AA  mov     word ptr ds:[OBJ_X],ax */
    ww(pDS, 0x2886, AX);
L39AD:
    /* 39AD  mov     ax,word ptr ds:[OBJ_Y] */
    AX = rw(pDS, 0x2888);
L39B0:
    /* 39B0  sub     ax,word ptr [si-2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFE));
L39B3:
    /* 39B3  mov     word ptr ds:[OBJ_Y],ax */
    ww(pDS, 0x2888, AX);
L39B6:
    /* 39B6  mov     ax,word ptr ds:[OBJ_Z] */
    AX = rw(pDS, 0x288A);
L39B9:
    /* 39B9  sub     ax,word ptr [si-4] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFC));
L39BC:
    /* 39BC  mov     word ptr ds:[OBJ_Z],ax */
    ww(pDS, 0x288A, AX);
L39BF:
    /* 39BF  mov     bp,PNT_X */
    BP = 0x1620;
L39C2:
    /* 39C2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L39C3:
    /* 39C3  add     di,ax */
    DI = (uint16_t)(DI + AX);
L39C5:
    /* 39C5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L39C6:
    /* 39C6  push    si */
    push16(SI);
L39C7:
    /* 39C7  add     si,ax */
    SI = (uint16_t)(SI + AX);
L39C9:
    /* 39C9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L39CA:
    /* 39CA  mov     dx,ax */
    DX = AX;
L39CC:
    /* 39CC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L39CD:
    /* 39CD  sub     al,byte ptr ds:[OBJ_SHIFT] */
    AL = (uint8_t)(AL - rb(pDS, 0x2880));
L39D1:
    /* 39D1  mov     dh,al */
    DH = AL;
L39D3:
    /* 39D3  shr     dl,1 */
    DL = shr8(DL, 1);
L39D5:
    /* 39D5  jae     short L3A05 */
    if (!CF) goto L3A05;
L39D7:
    /* 39D7  mov     cl,dh */
    CL = DH;
L39D9:
    /* 39D9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L39DA:
    /* 39DA  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L39DC:
    /* 39DC  mov     bx,ax */
    BX = AX;
L39DE:
    /* 39DE  add     bx,word ptr ds:[15FCh] */
    BX = add16(BX, rw(pDS, 0x15FC), 0);
L39E2:
    /* 39E2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L39E3:
    /* 39E3  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L39E5:
    /* 39E5  mov     bp,ax */
    BP = AX;
L39E7:
    /* 39E7  add     bp,word ptr ds:[1600h] */
    BP = add16(BP, rw(pDS, 0x1600), 0);
L39EB:
    /* 39EB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L39EC:
    /* 39EC  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L39EE:
    /* 39EE  mov     cx,ax */
    CX = AX;
L39F0:
    /* 39F0  add     cx,word ptr ds:[15FEh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15FE));
L39F4:
    /* 39F4  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x39F7)) != 0) return c;
L39F7:
    /* 39F7  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L39F9:
    /* 39F9  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L39FC:
    /* 39FC  mov     word ptr [di+4],bp */
    ww(pDS, DI + 0x4, BP);
L39FF:
    /* 39FF  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L3A02:
    /* 3A02  add     di,8 */
    DI = add16(DI, 0x8, 0);
L3A05: /* L3A05 */
    /* 3A05  mov     cl,dh */
    CL = DH;
L3A07:
    /* 3A07  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3A08:
    /* 3A08  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L3A0A:
    /* 3A0A  mov     bx,ax */
    BX = AX;
L3A0C:
    /* 3A0C  add     bx,word ptr ds:[15FCh] */
    BX = add16(BX, rw(pDS, 0x15FC), 0);
L3A10:
    /* 3A10  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3A11:
    /* 3A11  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L3A13:
    /* 3A13  mov     bp,ax */
    BP = AX;
L3A15:
    /* 3A15  add     bp,word ptr ds:[1600h] */
    BP = add16(BP, rw(pDS, 0x1600), 0);
L3A19:
    /* 3A19  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3A1A:
    /* 3A1A  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L3A1C:
    /* 3A1C  mov     cx,ax */
    CX = AX;
L3A1E:
    /* 3A1E  add     cx,word ptr ds:[15FEh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15FE));
L3A22:
    /* 3A22  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x3A25)) != 0) return c;
L3A25:
    /* 3A25  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L3A27:
    /* 3A27  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L3A2A:
    /* 3A2A  mov     word ptr [di+4],bp */
    ww(pDS, DI + 0x4, BP);
L3A2D:
    /* 3A2D  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L3A30:
    /* 3A30  mov     cl,dh */
    CL = DH;
L3A32:
    /* 3A32  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3A33:
    /* 3A33  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L3A35:
    /* 3A35  mov     bx,ax */
    BX = AX;
L3A37:
    /* 3A37  add     bx,word ptr ds:[15FCh] */
    BX = add16(BX, rw(pDS, 0x15FC), 0);
L3A3B:
    /* 3A3B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3A3C:
    /* 3A3C  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L3A3E:
    /* 3A3E  mov     bp,ax */
    BP = AX;
L3A40:
    /* 3A40  add     bp,word ptr ds:[1600h] */
    BP = add16(BP, rw(pDS, 0x1600), 0);
L3A44:
    /* 3A44  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3A45:
    /* 3A45  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L3A47:
    /* 3A47  mov     cx,ax */
    CX = AX;
L3A49:
    /* 3A49  add     cx,word ptr ds:[15FEh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15FE));
L3A4D:
    /* 3A4D  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x3A50)) != 0) return c;
L3A50:
    /* 3A50  mov     word ptr [di+8],bx */
    ww(pDS, DI + 0x8, BX);
L3A53:
    /* 3A53  mov     word ptr [di+0Ah],cx */
    ww(pDS, DI + 0xA, CX);
L3A56:
    /* 3A56  mov     word ptr [di+0Ch],bp */
    ww(pDS, DI + 0xC, BP);
L3A59:
    /* 3A59  mov     byte ptr [di+0Eh],al */
    wb(pDS, DI + 0xE, AL);
L3A5C:
    /* 3A5C  add     di,10h */
    DI = add16(DI, 0x10, 0);
L3A5F:
    /* 3A5F  dec     dl */
    DL = dec8(DL);
L3A61:
    /* 3A61  jne     L3A05 */
    if (!ZF) goto L3A05;
L3A63:
    /* 3A63  pop     si */
    SI = pop16();
L3A64:
    /* 3A64  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3A65:
    /* 3A65  push    si */
    push16(SI);
L3A66:
    /* 3A66  add     si,ax */
    SI = (uint16_t)(SI + AX);
L3A68:
    /* 3A68  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3A69:
    /* 3A69  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3A6A:
    /* 3A6A  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x3A6E)) != 0) return c;
L3A6E:
    /* 3A6E  pop     si */
    SI = pop16();
L3A6F:
    /* 3A6F  pop     word ptr ds:[OBJ_Z] */
    { uint16_t t_ = pop16(); ww(pDS, 0x288A, t_); }
L3A73:
    /* 3A73  pop     word ptr ds:[OBJ_Y] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2888, t_); }
L3A77:
    /* 3A77  pop     word ptr ds:[OBJ_X] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2886, t_); }
L3A7B:
    /* 3A7B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3A7C:
    /* 3A7C  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3A7D:
    /* 3A7D  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3A81  (+3A81)
       do_usepat (FM name), opcode 9Ah in UW1: see do_cavesurf's comment. */
L3A81: /* _do_usepat */
    /* 3A81  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L3A85:
    /* 3A85  mov     bx,word ptr ds:[OBJ_X] */
    BX = rw(pDS, 0x2886);
L3A89:
    /* 3A89  neg     bx */
    BX = neg16(BX);
L3A8B:
    /* 3A8B  shl     bx,cl */
    BX = (uint16_t)(BX << (CL & 31));
L3A8D:
    /* 3A8D  mov     bp,word ptr ds:[OBJ_Z] */
    BP = rw(pDS, 0x288A);
L3A91:
    /* 3A91  neg     bp */
    BP = neg16(BP);
L3A93:
    /* 3A93  shl     bp,cl */
    BP = (uint16_t)(BP << (CL & 31));
L3A95:
    /* 3A95  mov     ax,word ptr ds:[OBJ_Y] */
    AX = rw(pDS, 0x2888);
L3A98:
    /* 3A98  neg     ax */
    AX = neg16(AX);
L3A9A:
    /* 3A9A  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L3A9C:
    /* 3A9C  mov     cx,ax */
    CX = AX;
L3A9E:
    /* 3A9E  call    _mxmul */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x50D8), 0x3AA1)) != 0) return c;
L3AA1:
    /* 3AA1  mov     word ptr ds:[15FCh],bx */
    ww(pDS, 0x15FC, BX);
L3AA5:
    /* 3AA5  mov     word ptr ds:[15FEh],cx */
    ww(pDS, 0x15FE, CX);
L3AA9:
    /* 3AA9  mov     word ptr ds:[1600h],bp */
    ww(pDS, 0x1600, BP);
L3AAD:
    /* 3AAD  mov     di,PNT_X */
    DI = 0x1620;
L3AB0:
    /* 3AB0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3AB1:
    /* 3AB1  add     di,ax */
    DI = (uint16_t)(DI + AX);
L3AB3:
    /* 3AB3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3AB4:
    /* 3AB4  push    si */
    push16(SI);
L3AB5:
    /* 3AB5  add     si,ax */
    SI = (uint16_t)(SI + AX);
L3AB7:
    /* 3AB7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3AB8:
    /* 3AB8  mov     cx,ax */
    CX = AX;
L3ABA:
    /* 3ABA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3ABB:
    /* 3ABB  sub     al,byte ptr ds:[OBJ_SHIFT] */
    AL = (uint8_t)(AL - rb(pDS, 0x2880));
L3ABF:
    /* 3ABF  mov     dl,al */
    DL = AL;
L3AC1:
    /* 3AC1  shr     cl,1 */
    CL = shr8(CL, 1);
L3AC3:
    /* 3AC3  mov     dh,cl */
    DH = CL;
L3AC5:
    /* 3AC5  jae     short L3AF5 */
    if (!CF) goto L3AF5;
L3AC7:
    /* 3AC7  mov     cl,dl */
    CL = DL;
L3AC9:
    /* 3AC9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3ACA:
    /* 3ACA  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L3ACC:
    /* 3ACC  mov     bx,ax */
    BX = AX;
L3ACE:
    /* 3ACE  add     bx,word ptr ds:[15FCh] */
    BX = add16(BX, rw(pDS, 0x15FC), 0);
L3AD2:
    /* 3AD2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3AD3:
    /* 3AD3  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L3AD5:
    /* 3AD5  mov     bp,ax */
    BP = AX;
L3AD7:
    /* 3AD7  add     bp,word ptr ds:[1600h] */
    BP = add16(BP, rw(pDS, 0x1600), 0);
L3ADB:
    /* 3ADB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3ADC:
    /* 3ADC  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L3ADE:
    /* 3ADE  mov     cx,ax */
    CX = AX;
L3AE0:
    /* 3AE0  add     cx,word ptr ds:[15FEh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15FE));
L3AE4:
    /* 3AE4  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x3AE7)) != 0) return c;
L3AE7:
    /* 3AE7  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L3AE9:
    /* 3AE9  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L3AEC:
    /* 3AEC  mov     word ptr [di+4],bp */
    ww(pDS, DI + 0x4, BP);
L3AEF:
    /* 3AEF  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L3AF2:
    /* 3AF2  add     di,8 */
    DI = add16(DI, 0x8, 0);
L3AF5: /* L3AF5 */
    /* 3AF5  mov     cl,dl */
    CL = DL;
L3AF7:
    /* 3AF7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3AF8:
    /* 3AF8  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L3AFA:
    /* 3AFA  mov     bx,ax */
    BX = AX;
L3AFC:
    /* 3AFC  add     bx,word ptr ds:[15FCh] */
    BX = add16(BX, rw(pDS, 0x15FC), 0);
L3B00:
    /* 3B00  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3B01:
    /* 3B01  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L3B03:
    /* 3B03  mov     bp,ax */
    BP = AX;
L3B05:
    /* 3B05  add     bp,word ptr ds:[1600h] */
    BP = add16(BP, rw(pDS, 0x1600), 0);
L3B09:
    /* 3B09  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3B0A:
    /* 3B0A  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L3B0C:
    /* 3B0C  mov     cx,ax */
    CX = AX;
L3B0E:
    /* 3B0E  add     cx,word ptr ds:[15FEh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15FE));
L3B12:
    /* 3B12  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x3B15)) != 0) return c;
L3B15:
    /* 3B15  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L3B17:
    /* 3B17  mov     word ptr [di+2],cx */
    ww(pDS, DI + 0x2, CX);
L3B1A:
    /* 3B1A  mov     word ptr [di+4],bp */
    ww(pDS, DI + 0x4, BP);
L3B1D:
    /* 3B1D  mov     byte ptr [di+6],al */
    wb(pDS, DI + 0x6, AL);
L3B20:
    /* 3B20  mov     cl,dl */
    CL = DL;
L3B22:
    /* 3B22  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3B23:
    /* 3B23  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L3B25:
    /* 3B25  mov     bx,ax */
    BX = AX;
L3B27:
    /* 3B27  add     bx,word ptr ds:[15FCh] */
    BX = add16(BX, rw(pDS, 0x15FC), 0);
L3B2B:
    /* 3B2B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3B2C:
    /* 3B2C  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L3B2E:
    /* 3B2E  mov     bp,ax */
    BP = AX;
L3B30:
    /* 3B30  add     bp,word ptr ds:[1600h] */
    BP = add16(BP, rw(pDS, 0x1600), 0);
L3B34:
    /* 3B34  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3B35:
    /* 3B35  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L3B37:
    /* 3B37  mov     cx,ax */
    CX = AX;
L3B39:
    /* 3B39  add     cx,word ptr ds:[15FEh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15FE));
L3B3D:
    /* 3B3D  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x3B40)) != 0) return c;
L3B40:
    /* 3B40  mov     word ptr [di+8],bx */
    ww(pDS, DI + 0x8, BX);
L3B43:
    /* 3B43  mov     word ptr [di+0Ah],cx */
    ww(pDS, DI + 0xA, CX);
L3B46:
    /* 3B46  mov     word ptr [di+0Ch],bp */
    ww(pDS, DI + 0xC, BP);
L3B49:
    /* 3B49  mov     byte ptr [di+0Eh],al */
    wb(pDS, DI + 0xE, AL);
L3B4C:
    /* 3B4C  add     di,10h */
    DI = add16(DI, 0x10, 0);
L3B4F:
    /* 3B4F  dec     dh */
    DH = dec8(DH);
L3B51:
    /* 3B51  jne     L3AF5 */
    if (!ZF) goto L3AF5;
L3B53:
    /* 3B53  pop     si */
    SI = pop16();
L3B54:
    /* 3B54  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3B55:
    /* 3B55  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3B56:
    /* 3B56  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3B5A  (+3B5A)
       Opcode 48h, operand: offset. Jumps: SI += offset. */
L3B5A: /* _do_ijmp */
    /* 3B5A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3B5B:
    /* 3B5B  add     si,ax */
    SI = add16(SI, AX, 0);
L3B5D:
    /* 3B5D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3B5E:
    /* 3B5E  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3B5F:
    /* 3B5F  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3B63  (+3B63)
       Opcode 32h, operand: address. Jumps to the program address held in the model variable. */
L3B63: /* _do_ijmp_i */
    /* 3B63  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3B64:
    /* 3B64  mov     bx,ax */
    BX = AX;
L3B66:
    /* 3B66  mov     si,word ptr [bx] */
    SI = rw(pDS, BX);
L3B68:
    /* 3B68  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3B69:
    /* 3B69  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3B6A:
    /* 3B6A  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3B6E  (+3B6E)
       Opcode 4Ah, operands: dx, dy, dz. Moves the object origin by a relative offset (subtracts
       from 2886h..288Ah) and calls self_modify. */
L3B6E: /* _do_rorg */
    /* 3B6E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3B6F:
    /* 3B6F  sub     word ptr ds:[OBJ_X],ax */
    ww(pDS, 0x2886, (uint16_t)(rw(pDS, 0x2886) - AX));
L3B73:
    /* 3B73  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3B74:
    /* 3B74  sub     word ptr ds:[OBJ_Y],ax */
    ww(pDS, 0x2888, (uint16_t)(rw(pDS, 0x2888) - AX));
L3B78:
    /* 3B78  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3B79:
    /* 3B79  sub     word ptr ds:[OBJ_Z],ax */
    ww(pDS, 0x288A, (uint16_t)(rw(pDS, 0x288A) - AX));
L3B7D:
    /* 3B7D  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x3B80)) != 0) return c;
L3B80:
    /* 3B80  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3B81:
    /* 3B81  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3B82:
    /* 3B82  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3B86  (+3B86)
       Opcode 4Ch, operands: an (x, z, y) offset, point. Stores the rotated offset (no origin, no
       clip
       code) as a point, for do_addres and do_subres. */
L3B86: /* _do_defdelta */
    /* 3B86  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L3B8A:
    /* 3B8A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3B8B:
    /* 3B8B  shl     ax,cl */
    AX = shl16(AX, CL);
L3B8D:
    /* 3B8D  mov     bx,ax */
    BX = AX;
L3B8F:
    /* 3B8F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3B90:
    /* 3B90  shl     ax,cl */
    AX = shl16(AX, CL);
L3B92:
    /* 3B92  mov     bp,ax */
    BP = AX;
L3B94:
    /* 3B94  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3B95:
    /* 3B95  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L3B97:
    /* 3B97  mov     cx,ax */
    CX = AX;
L3B99:
    /* 3B99  call    _mxmul */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x50D8), 0x3B9C)) != 0) return c;
L3B9C:
    /* 3B9C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3B9D:
    /* 3B9D  mov     di,ax */
    DI = AX;
L3B9F:
    /* 3B9F  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L3BA3:
    /* 3BA3  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L3BA7:
    /* 3BA7  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L3BAB:
    /* 3BAB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3BAC:
    /* 3BAC  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3BAD:
    /* 3BAD  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3BB1  (+3BB1)
       Opcode 8Ch, operands: point a, point b, destination. Destination = a + b, clip-coded. */
L3BB1: /* _do_addres */
    /* 3BB1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3BB2:
    /* 3BB2  mov     bp,ax */
    BP = AX;
L3BB4:
    /* 3BB4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3BB5:
    /* 3BB5  mov     di,ax */
    DI = AX;
L3BB7:
    /* 3BB7  mov     bx,word ptr ds:[bp+PNT_X] */
    BX = rw(pDS, BP + 0x1620);
L3BBC:
    /* 3BBC  add     bx,word ptr [di+PNT_X] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x1620));
L3BC0:
    /* 3BC0  mov     cx,word ptr ds:[bp+PNT_Y] */
    CX = rw(pDS, BP + 0x1622);
L3BC5:
    /* 3BC5  add     cx,word ptr [di+PNT_Y] */
    CX = (uint16_t)(CX + rw(pDS, DI + 0x1622));
L3BC9:
    /* 3BC9  mov     bp,word ptr ds:[bp+PNT_Z] */
    BP = rw(pDS, BP + 0x1624);
L3BCE:
    /* 3BCE  add     bp,word ptr [di+PNT_Z] */
    BP = (uint16_t)(BP + rw(pDS, DI + 0x1624));
L3BD2:
    /* 3BD2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3BD3:
    /* 3BD3  mov     di,ax */
    DI = AX;
L3BD5:
    /* 3BD5  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x3BD8)) != 0) return c;
L3BD8:
    /* 3BD8  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L3BDC:
    /* 3BDC  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L3BE0:
    /* 3BE0  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L3BE4:
    /* 3BE4  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L3BE8:
    /* 3BE8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3BE9:
    /* 3BE9  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3BEA:
    /* 3BEA  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3BEE  (+3BEE)
       Opcode C6h, operands: point a, point b, destination. Destination = a - b, clip-coded. */
L3BEE: /* _do_subres */
    /* 3BEE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3BEF:
    /* 3BEF  mov     bp,ax */
    BP = AX;
L3BF1:
    /* 3BF1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3BF2:
    /* 3BF2  mov     di,ax */
    DI = AX;
L3BF4:
    /* 3BF4  mov     bx,word ptr ds:[bp+PNT_X] */
    BX = rw(pDS, BP + 0x1620);
L3BF9:
    /* 3BF9  sub     bx,word ptr [di+PNT_X] */
    BX = (uint16_t)(BX - rw(pDS, DI + 0x1620));
L3BFD:
    /* 3BFD  mov     cx,word ptr ds:[bp+PNT_Y] */
    CX = rw(pDS, BP + 0x1622);
L3C02:
    /* 3C02  sub     cx,word ptr [di+PNT_Y] */
    CX = (uint16_t)(CX - rw(pDS, DI + 0x1622));
L3C06:
    /* 3C06  mov     bp,word ptr ds:[bp+PNT_Z] */
    BP = rw(pDS, BP + 0x1624);
L3C0B:
    /* 3C0B  sub     bp,word ptr [di+PNT_Z] */
    BP = (uint16_t)(BP - rw(pDS, DI + 0x1624));
L3C0F:
    /* 3C0F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3C10:
    /* 3C10  mov     di,ax */
    DI = AX;
L3C12:
    /* 3C12  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x3C15)) != 0) return c;
L3C15:
    /* 3C15  mov     word ptr [di+PNT_X],bx */
    ww(pDS, DI + 0x1620, BX);
L3C19:
    /* 3C19  mov     word ptr [di+PNT_Y],cx */
    ww(pDS, DI + 0x1622, CX);
L3C1D:
    /* 3C1D  mov     word ptr [di+PNT_Z],bp */
    ww(pDS, DI + 0x1624, BP);
L3C21:
    /* 3C21  mov     byte ptr [di+PNT_CODES],al */
    wb(pDS, DI + 0x1626, AL);
L3C25:
    /* 3C25  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3C26:
    /* 3C26  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3C27:
    /* 3C27  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3C2B  (+3C2B)
       Opcode BAh: do_ihcall with the angle taken from a model variable. */
L3C2B: /* _do_ihcall_ind */
    /* 3C2B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3C2C:
    /* 3C2C  mov     bx,ax */
    BX = AX;
L3C2E:
    /* 3C2E  mov     bx,word ptr [bx] */
    BX = rw(pDS, BX);
L3C30:
    /* 3C30  jmp     short L3C35 */
    goto L3C35;

    /* seg004_3C32  (+3C32)
       Opcode 50h, operands: angle, offset. Saves the matrix and origin, rotates the matrix by the
       heading angle (instance_head, which also rotates the origin), runs the sub-program at
       SI + offset, and restores both. The b and p variants below do the same for bank and pitch. */
L3C32: /* _do_ihcall */
    /* 3C32  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3C33:
    /* 3C33  mov     bx,ax */
    BX = AX;
L3C35: /* L3C35 */
    /* 3C35  push    word ptr ds:[MAT_XX] */
    push16(rw(pDS, 0x1602));
L3C39:
    /* 3C39  push    word ptr ds:[MAT_YX] */
    push16(rw(pDS, 0x1608));
L3C3D:
    /* 3C3D  push    word ptr ds:[MAT_ZX] */
    push16(rw(pDS, 0x160E));
L3C41:
    /* 3C41  push    word ptr ds:[MAT_XY] */
    push16(rw(pDS, 0x1604));
L3C45:
    /* 3C45  push    word ptr ds:[MAT_YY] */
    push16(rw(pDS, 0x160A));
L3C49:
    /* 3C49  push    word ptr ds:[MAT_ZY] */
    push16(rw(pDS, 0x1610));
L3C4D:
    /* 3C4D  push    word ptr ds:[MAT_XZ] */
    push16(rw(pDS, 0x1606));
L3C51:
    /* 3C51  push    word ptr ds:[MAT_YZ] */
    push16(rw(pDS, 0x160C));
L3C55:
    /* 3C55  push    word ptr ds:[MAT_ZZ] */
    push16(rw(pDS, 0x1612));
L3C59:
    /* 3C59  push    word ptr ds:[OBJ_X] */
    push16(rw(pDS, 0x2886));
L3C5D:
    /* 3C5D  push    word ptr ds:[OBJ_Y] */
    push16(rw(pDS, 0x2888));
L3C61:
    /* 3C61  push    word ptr ds:[OBJ_Z] */
    push16(rw(pDS, 0x288A));
L3C65:
    /* 3C65  call    _instance_head */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5426), 0x3C68)) != 0) return c;
L3C68:
    /* 3C68  push    si */
    push16(SI);
L3C69:
    /* 3C69  call    _check_flat */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x4E76), 0x3C6C)) != 0) return c;
L3C6C:
    /* 3C6C  pop     si */
    SI = pop16();
L3C6D:
    /* 3C6D  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x3C70)) != 0) return c;
L3C70:
    /* 3C70  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3C71:
    /* 3C71  push    si */
    push16(SI);
L3C72:
    /* 3C72  add     si,ax */
    SI = (uint16_t)(SI + AX);
L3C74:
    /* 3C74  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3C75:
    /* 3C75  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3C76:
    /* 3C76  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x3C7A)) != 0) return c;
L3C7A:
    /* 3C7A  pop     si */
    SI = pop16();
L3C7B:
    /* 3C7B  pop     word ptr ds:[OBJ_Z] */
    { uint16_t t_ = pop16(); ww(pDS, 0x288A, t_); }
L3C7F:
    /* 3C7F  pop     word ptr ds:[OBJ_Y] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2888, t_); }
L3C83:
    /* 3C83  pop     word ptr ds:[OBJ_X] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2886, t_); }
L3C87:
    /* 3C87  pop     word ptr ds:[MAT_ZZ] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1612, t_); }
L3C8B:
    /* 3C8B  pop     word ptr ds:[MAT_YZ] */
    { uint16_t t_ = pop16(); ww(pDS, 0x160C, t_); }
L3C8F:
    /* 3C8F  pop     word ptr ds:[MAT_XZ] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1606, t_); }
L3C93:
    /* 3C93  pop     word ptr ds:[MAT_ZY] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1610, t_); }
L3C97:
    /* 3C97  pop     word ptr ds:[MAT_YY] */
    { uint16_t t_ = pop16(); ww(pDS, 0x160A, t_); }
L3C9B:
    /* 3C9B  pop     word ptr ds:[MAT_XY] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1604, t_); }
L3C9F:
    /* 3C9F  pop     word ptr ds:[MAT_ZX] */
    { uint16_t t_ = pop16(); ww(pDS, 0x160E, t_); }
L3CA3:
    /* 3CA3  pop     word ptr ds:[MAT_YX] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1608, t_); }
L3CA7:
    /* 3CA7  pop     word ptr ds:[MAT_XX] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1602, t_); }
L3CAB:
    /* 3CAB  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x3CAE)) != 0) return c;
L3CAE:
    /* 3CAE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3CAF:
    /* 3CAF  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3CB0:
    /* 3CB0  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3CB4  (+3CB4)
       Opcode C4h: do_ibcall with the angle from a model variable. */
L3CB4: /* _do_ibcall_ind */
    /* 3CB4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3CB5:
    /* 3CB5  mov     bx,ax */
    BX = AX;
L3CB7:
    /* 3CB7  mov     bx,word ptr [bx] */
    BX = rw(pDS, BX);
L3CB9:
    /* 3CB9  jmp     short L3CBE */
    goto L3CBE;

    /* seg004_3CBB  (+3CBB)
       Opcode 70h: a sub-program rotated by a bank angle (instance_bank). */
L3CBB: /* _do_ibcall */
    /* 3CBB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3CBC:
    /* 3CBC  mov     bx,ax */
    BX = AX;
L3CBE: /* L3CBE */
    /* 3CBE  push    word ptr ds:[MAT_XX] */
    push16(rw(pDS, 0x1602));
L3CC2:
    /* 3CC2  push    word ptr ds:[MAT_YX] */
    push16(rw(pDS, 0x1608));
L3CC6:
    /* 3CC6  push    word ptr ds:[MAT_ZX] */
    push16(rw(pDS, 0x160E));
L3CCA:
    /* 3CCA  push    word ptr ds:[MAT_XY] */
    push16(rw(pDS, 0x1604));
L3CCE:
    /* 3CCE  push    word ptr ds:[MAT_YY] */
    push16(rw(pDS, 0x160A));
L3CD2:
    /* 3CD2  push    word ptr ds:[MAT_ZY] */
    push16(rw(pDS, 0x1610));
L3CD6:
    /* 3CD6  push    word ptr ds:[MAT_XZ] */
    push16(rw(pDS, 0x1606));
L3CDA:
    /* 3CDA  push    word ptr ds:[MAT_YZ] */
    push16(rw(pDS, 0x160C));
L3CDE:
    /* 3CDE  push    word ptr ds:[MAT_ZZ] */
    push16(rw(pDS, 0x1612));
L3CE2:
    /* 3CE2  push    word ptr ds:[OBJ_X] */
    push16(rw(pDS, 0x2886));
L3CE6:
    /* 3CE6  push    word ptr ds:[OBJ_Y] */
    push16(rw(pDS, 0x2888));
L3CEA:
    /* 3CEA  push    word ptr ds:[OBJ_Z] */
    push16(rw(pDS, 0x288A));
L3CEE:
    /* 3CEE  call    _instance_bank */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x56F6), 0x3CF1)) != 0) return c;
L3CF1:
    /* 3CF1  push    si */
    push16(SI);
L3CF2:
    /* 3CF2  call    _check_flat */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x4E76), 0x3CF5)) != 0) return c;
L3CF5:
    /* 3CF5  pop     si */
    SI = pop16();
L3CF6:
    /* 3CF6  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x3CF9)) != 0) return c;
L3CF9:
    /* 3CF9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3CFA:
    /* 3CFA  push    si */
    push16(SI);
L3CFB:
    /* 3CFB  add     si,ax */
    SI = (uint16_t)(SI + AX);
L3CFD:
    /* 3CFD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3CFE:
    /* 3CFE  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3CFF:
    /* 3CFF  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x3D03)) != 0) return c;
L3D03:
    /* 3D03  pop     si */
    SI = pop16();
L3D04:
    /* 3D04  pop     word ptr ds:[OBJ_Z] */
    { uint16_t t_ = pop16(); ww(pDS, 0x288A, t_); }
L3D08:
    /* 3D08  pop     word ptr ds:[OBJ_Y] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2888, t_); }
L3D0C:
    /* 3D0C  pop     word ptr ds:[OBJ_X] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2886, t_); }
L3D10:
    /* 3D10  pop     word ptr ds:[MAT_ZZ] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1612, t_); }
L3D14:
    /* 3D14  pop     word ptr ds:[MAT_YZ] */
    { uint16_t t_ = pop16(); ww(pDS, 0x160C, t_); }
L3D18:
    /* 3D18  pop     word ptr ds:[MAT_XZ] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1606, t_); }
L3D1C:
    /* 3D1C  pop     word ptr ds:[MAT_ZY] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1610, t_); }
L3D20:
    /* 3D20  pop     word ptr ds:[MAT_YY] */
    { uint16_t t_ = pop16(); ww(pDS, 0x160A, t_); }
L3D24:
    /* 3D24  pop     word ptr ds:[MAT_XY] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1604, t_); }
L3D28:
    /* 3D28  pop     word ptr ds:[MAT_ZX] */
    { uint16_t t_ = pop16(); ww(pDS, 0x160E, t_); }
L3D2C:
    /* 3D2C  pop     word ptr ds:[MAT_YX] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1608, t_); }
L3D30:
    /* 3D30  pop     word ptr ds:[MAT_XX] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1602, t_); }
L3D34:
    /* 3D34  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x3D37)) != 0) return c;
L3D37:
    /* 3D37  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3D38:
    /* 3D38  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3D39:
    /* 3D39  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3D3D  (+3D3D)
       Opcode C2h: do_ipcall with the angle from a model variable. */
L3D3D: /* _do_ipcall_ind */
    /* 3D3D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3D3E:
    /* 3D3E  mov     bx,ax */
    BX = AX;
L3D40:
    /* 3D40  mov     bx,word ptr [bx] */
    BX = rw(pDS, BX);
L3D42:
    /* 3D42  jmp     short L3D47 */
    goto L3D47;

    /* seg004_3D44  (+3D44)
       Opcode 72h: a sub-program rotated by a pitch angle (instance_pitch). */
L3D44: /* _do_ipcall */
    /* 3D44  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3D45:
    /* 3D45  mov     bx,ax */
    BX = AX;
L3D47: /* L3D47 */
    /* 3D47  push    word ptr ds:[MAT_XX] */
    push16(rw(pDS, 0x1602));
L3D4B:
    /* 3D4B  push    word ptr ds:[MAT_YX] */
    push16(rw(pDS, 0x1608));
L3D4F:
    /* 3D4F  push    word ptr ds:[MAT_ZX] */
    push16(rw(pDS, 0x160E));
L3D53:
    /* 3D53  push    word ptr ds:[MAT_XY] */
    push16(rw(pDS, 0x1604));
L3D57:
    /* 3D57  push    word ptr ds:[MAT_YY] */
    push16(rw(pDS, 0x160A));
L3D5B:
    /* 3D5B  push    word ptr ds:[MAT_ZY] */
    push16(rw(pDS, 0x1610));
L3D5F:
    /* 3D5F  push    word ptr ds:[MAT_XZ] */
    push16(rw(pDS, 0x1606));
L3D63:
    /* 3D63  push    word ptr ds:[MAT_YZ] */
    push16(rw(pDS, 0x160C));
L3D67:
    /* 3D67  push    word ptr ds:[MAT_ZZ] */
    push16(rw(pDS, 0x1612));
L3D6B:
    /* 3D6B  push    word ptr ds:[OBJ_X] */
    push16(rw(pDS, 0x2886));
L3D6F:
    /* 3D6F  push    word ptr ds:[OBJ_Y] */
    push16(rw(pDS, 0x2888));
L3D73:
    /* 3D73  push    word ptr ds:[OBJ_Z] */
    push16(rw(pDS, 0x288A));
L3D77:
    /* 3D77  call    _instance_pitch */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x558E), 0x3D7A)) != 0) return c;
L3D7A:
    /* 3D7A  push    si */
    push16(SI);
L3D7B:
    /* 3D7B  call    _check_flat */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x4E76), 0x3D7E)) != 0) return c;
L3D7E:
    /* 3D7E  pop     si */
    SI = pop16();
L3D7F:
    /* 3D7F  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x3D82)) != 0) return c;
L3D82:
    /* 3D82  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3D83:
    /* 3D83  push    si */
    push16(SI);
L3D84:
    /* 3D84  add     si,ax */
    SI = (uint16_t)(SI + AX);
L3D86:
    /* 3D86  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3D87:
    /* 3D87  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3D88:
    /* 3D88  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x3D8C)) != 0) return c;
L3D8C:
    /* 3D8C  pop     si */
    SI = pop16();
L3D8D:
    /* 3D8D  pop     word ptr ds:[OBJ_Z] */
    { uint16_t t_ = pop16(); ww(pDS, 0x288A, t_); }
L3D91:
    /* 3D91  pop     word ptr ds:[OBJ_Y] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2888, t_); }
L3D95:
    /* 3D95  pop     word ptr ds:[OBJ_X] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2886, t_); }
L3D99:
    /* 3D99  pop     word ptr ds:[MAT_ZZ] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1612, t_); }
L3D9D:
    /* 3D9D  pop     word ptr ds:[MAT_YZ] */
    { uint16_t t_ = pop16(); ww(pDS, 0x160C, t_); }
L3DA1:
    /* 3DA1  pop     word ptr ds:[MAT_XZ] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1606, t_); }
L3DA5:
    /* 3DA5  pop     word ptr ds:[MAT_ZY] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1610, t_); }
L3DA9:
    /* 3DA9  pop     word ptr ds:[MAT_YY] */
    { uint16_t t_ = pop16(); ww(pDS, 0x160A, t_); }
L3DAD:
    /* 3DAD  pop     word ptr ds:[MAT_XY] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1604, t_); }
L3DB1:
    /* 3DB1  pop     word ptr ds:[MAT_ZX] */
    { uint16_t t_ = pop16(); ww(pDS, 0x160E, t_); }
L3DB5:
    /* 3DB5  pop     word ptr ds:[MAT_YX] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1608, t_); }
L3DB9:
    /* 3DB9  pop     word ptr ds:[MAT_XX] */
    { uint16_t t_ = pop16(); ww(pDS, 0x1602, t_); }
L3DBD:
    /* 3DBD  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x3DC0)) != 0) return c;
L3DC0:
    /* 3DC0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3DC1:
    /* 3DC1  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3DC2:
    /* 3DC2  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3DC6  (+3DC6)
       UW1 only: opcode 52h. Takes points relative to the object origin, rotates them
       (seg004_52FF, 5374) and divides by a count from its operands; not traced further. */
L3DC6: /* _seg004_3DC6 */
    /* 3DC6  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L3DC8:
    /* 3DC8  mov     di,26ECh */
    DI = 0x26EC;
L3DCB:
    /* 3DCB  mov     cx,6 */
    CX = 0x6;
L3DCE:
    /* 3DCE  rep stosw */
    while (CX) { ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2)); CX--; }
L3DD0:
    /* 3DD0  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L3DD4:
    /* 3DD4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3DD5:
    /* 3DD5  sub     ax,word ptr ds:[OBJ_X] */
    AX = sub16(AX, rw(pDS, 0x2886), 0);
L3DD9:
    /* 3DD9  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L3DDB:
    /* 3DDB  mov     bx,ax */
    BX = AX;
L3DDD:
    /* 3DDD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3DDE:
    /* 3DDE  sub     ax,word ptr ds:[OBJ_Z] */
    AX = sub16(AX, rw(pDS, 0x288A), 0);
L3DE2:
    /* 3DE2  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L3DE4:
    /* 3DE4  mov     bp,ax */
    BP = AX;
L3DE6:
    /* 3DE6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3DE7:
    /* 3DE7  sub     ax,word ptr ds:[OBJ_Y] */
    AX = sub16(AX, rw(pDS, 0x2888), 0);
L3DEB:
    /* 3DEB  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L3DED:
    /* 3DED  mov     cx,ax */
    CX = AX;
L3DEF:
    /* 3DEF  call    _seg004_52FF */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x52FF), 0x3DF2)) != 0) return c;
L3DF2:
    /* 3DF2  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L3DF6:
    /* 3DF6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3DF7:
    /* 3DF7  sub     ax,word ptr [si-8] */
    AX = sub16(AX, rw(pDS, SI + 0xFFF8), 0);
L3DFA:
    /* 3DFA  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L3DFC:
    /* 3DFC  mov     bx,ax */
    BX = AX;
L3DFE:
    /* 3DFE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3DFF:
    /* 3DFF  sub     ax,word ptr [si-8] */
    AX = sub16(AX, rw(pDS, SI + 0xFFF8), 0);
L3E02:
    /* 3E02  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L3E04:
    /* 3E04  mov     bp,ax */
    BP = AX;
L3E06:
    /* 3E06  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3E07:
    /* 3E07  sub     ax,word ptr [si-8] */
    AX = sub16(AX, rw(pDS, SI + 0xFFF8), 0);
L3E0A:
    /* 3E0A  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L3E0C:
    /* 3E0C  mov     cx,ax */
    CX = AX;
L3E0E:
    /* 3E0E  call    _seg004_5374 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5374), 0x3E11)) != 0) return c;
L3E11:
    /* 3E11  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3E12:
    /* 3E12  mov     cx,ax */
    CX = AX;
L3E14:
    /* 3E14  mov     ax,word ptr ds:[26E2h] */
    AX = rw(pDS, 0x26E2);
L3E17:
    /* 3E17  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L3E18:
    /* 3E18  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x3E18, 2)) != 0) return c;
L3E1A:
    /* 3E1A  mov     word ptr ds:[26E2h],ax */
    ww(pDS, 0x26E2, AX);
L3E1D:
    /* 3E1D  mov     ax,word ptr ds:[26E0h] */
    AX = rw(pDS, 0x26E0);
L3E20:
    /* 3E20  sar     dx,1 */
    DX = sar16(DX, 1);
L3E22:
    /* 3E22  rcr     ax,1 */
    AX = rcr16(AX, 1);
L3E24:
    /* 3E24  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x3E24, 2)) != 0) return c;
L3E26:
    /* 3E26  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L3E27:
    /* 3E27  shl     ax,1 */
    AX = shl16(AX, 1);
L3E29:
    /* 3E29  rcl     dx,1 */
    DX = rcl16(DX, 1);
L3E2B:
    /* 3E2B  add     word ptr ds:[26E0h],ax */
    ww(pDS, 0x26E0, (uint16_t)(rw(pDS, 0x26E0) + AX));
L3E2F:
    /* 3E2F  add     word ptr ds:[26E2h],dx */
    ww(pDS, 0x26E2, (uint16_t)(rw(pDS, 0x26E2) + DX));
L3E33:
    /* 3E33  mov     ax,word ptr ds:[26E6h] */
    AX = rw(pDS, 0x26E6);
L3E36:
    /* 3E36  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L3E37:
    /* 3E37  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x3E37, 2)) != 0) return c;
L3E39:
    /* 3E39  mov     word ptr ds:[26E6h],ax */
    ww(pDS, 0x26E6, AX);
L3E3C:
    /* 3E3C  mov     ax,word ptr ds:[26E4h] */
    AX = rw(pDS, 0x26E4);
L3E3F:
    /* 3E3F  sar     dx,1 */
    DX = sar16(DX, 1);
L3E41:
    /* 3E41  rcr     ax,1 */
    AX = rcr16(AX, 1);
L3E43:
    /* 3E43  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x3E43, 2)) != 0) return c;
L3E45:
    /* 3E45  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L3E46:
    /* 3E46  shl     ax,1 */
    AX = shl16(AX, 1);
L3E48:
    /* 3E48  rcl     dx,1 */
    DX = rcl16(DX, 1);
L3E4A:
    /* 3E4A  add     word ptr ds:[26E4h],ax */
    ww(pDS, 0x26E4, (uint16_t)(rw(pDS, 0x26E4) + AX));
L3E4E:
    /* 3E4E  add     word ptr ds:[26E6h],dx */
    ww(pDS, 0x26E6, (uint16_t)(rw(pDS, 0x26E6) + DX));
L3E52:
    /* 3E52  mov     ax,word ptr ds:[26EAh] */
    AX = rw(pDS, 0x26EA);
L3E55:
    /* 3E55  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L3E56:
    /* 3E56  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x3E56, 2)) != 0) return c;
L3E58:
    /* 3E58  mov     word ptr ds:[26EAh],ax */
    ww(pDS, 0x26EA, AX);
L3E5B:
    /* 3E5B  mov     ax,word ptr ds:[26E8h] */
    AX = rw(pDS, 0x26E8);
L3E5E:
    /* 3E5E  sar     dx,1 */
    DX = sar16(DX, 1);
L3E60:
    /* 3E60  rcr     ax,1 */
    AX = rcr16(AX, 1);
L3E62:
    /* 3E62  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x3E62, 2)) != 0) return c;
L3E64:
    /* 3E64  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L3E65:
    /* 3E65  shl     ax,1 */
    AX = shl16(AX, 1);
L3E67:
    /* 3E67  rcl     dx,1 */
    DX = rcl16(DX, 1);
L3E69:
    /* 3E69  add     word ptr ds:[26E8h],ax */
    ww(pDS, 0x26E8, (uint16_t)(rw(pDS, 0x26E8) + AX));
L3E6D:
    /* 3E6D  add     word ptr ds:[26EAh],dx */
    ww(pDS, 0x26EA, (uint16_t)(rw(pDS, 0x26EA) + DX));
L3E71:
    /* 3E71  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3E72:
    /* 3E72  mov     di,ax */
    DI = AX;
L3E74:
    /* 3E74  add     di,PNT_X */
    DI = (uint16_t)(DI + 0x1620);
L3E78:
    /* 3E78  push    si */
    push16(SI);
L3E79:
    /* 3E79  mov     si,cx */
    SI = CX;
L3E7B: /* L3E7B */
    /* 3E7B  mov     ax,word ptr ds:[26D6h] */
    AX = rw(pDS, 0x26D6);
L3E7E:
    /* 3E7E  add     ax,word ptr ds:[26EEh] */
    AX = (uint16_t)(AX + rw(pDS, 0x26EE));
L3E82:
    /* 3E82  mov     bx,ax */
    BX = AX;
L3E84:
    /* 3E84  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3E85:
    /* 3E85  mov     ax,word ptr ds:[26DAh] */
    AX = rw(pDS, 0x26DA);
L3E88:
    /* 3E88  add     ax,word ptr ds:[26F2h] */
    AX = (uint16_t)(AX + rw(pDS, 0x26F2));
L3E8C:
    /* 3E8C  mov     cx,ax */
    CX = AX;
L3E8E:
    /* 3E8E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3E8F:
    /* 3E8F  mov     ax,word ptr ds:[26DEh] */
    AX = rw(pDS, 0x26DE);
L3E92:
    /* 3E92  add     ax,word ptr ds:[26F6h] */
    AX = (uint16_t)(AX + rw(pDS, 0x26F6));
L3E96:
    /* 3E96  mov     bp,ax */
    BP = AX;
L3E98:
    /* 3E98  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3E99:
    /* 3E99  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x3E9C)) != 0) return c;
L3E9C:
    /* 3E9C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3E9D:
    /* 3E9D  mov     ax,word ptr ds:[26E0h] */
    AX = rw(pDS, 0x26E0);
L3EA0:
    /* 3EA0  add     word ptr ds:[26ECh],ax */
    ww(pDS, 0x26EC, add16(rw(pDS, 0x26EC), AX, 0));
L3EA4:
    /* 3EA4  mov     ax,word ptr ds:[26E2h] */
    AX = rw(pDS, 0x26E2);
L3EA7:
    /* 3EA7  adc     word ptr ds:[26EEh],ax */
    ww(pDS, 0x26EE, (uint16_t)(rw(pDS, 0x26EE) + AX + CF));
L3EAB:
    /* 3EAB  mov     ax,word ptr ds:[26E4h] */
    AX = rw(pDS, 0x26E4);
L3EAE:
    /* 3EAE  add     word ptr ds:[26F0h],ax */
    ww(pDS, 0x26F0, add16(rw(pDS, 0x26F0), AX, 0));
L3EB2:
    /* 3EB2  mov     ax,word ptr ds:[26E6h] */
    AX = rw(pDS, 0x26E6);
L3EB5:
    /* 3EB5  adc     word ptr ds:[26F2h],ax */
    ww(pDS, 0x26F2, (uint16_t)(rw(pDS, 0x26F2) + AX + CF));
L3EB9:
    /* 3EB9  mov     ax,word ptr ds:[26E8h] */
    AX = rw(pDS, 0x26E8);
L3EBC:
    /* 3EBC  add     word ptr ds:[26F4h],ax */
    ww(pDS, 0x26F4, add16(rw(pDS, 0x26F4), AX, 0));
L3EC0:
    /* 3EC0  mov     ax,word ptr ds:[26EAh] */
    AX = rw(pDS, 0x26EA);
L3EC3:
    /* 3EC3  adc     word ptr ds:[26F6h],ax */
    ww(pDS, 0x26F6, add16(rw(pDS, 0x26F6), AX, CF));
L3EC7:
    /* 3EC7  dec     si */
    SI = dec16(SI);
L3EC8:
    /* 3EC8  jne     L3E7B */
    if (!ZF) goto L3E7B;
L3ECA:
    /* 3ECA  pop     si */
    SI = pop16();
L3ECB:
    /* 3ECB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3ECC:
    /* 3ECC  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3ECD:
    /* 3ECD  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3ED1  (+3ED1)
       Opcode 54h: pushes the object origin and the coordinate shift on the stack (the interpreter's
       own stack, SS = seg063). */
L3ED1: /* _do_pushc */
    /* 3ED1  push    word ptr ds:[OBJ_X] */
    push16(rw(pDS, 0x2886));
L3ED5:
    /* 3ED5  push    word ptr ds:[OBJ_Y] */
    push16(rw(pDS, 0x2888));
L3ED9:
    /* 3ED9  push    word ptr ds:[OBJ_Z] */
    push16(rw(pDS, 0x288A));
L3EDD:
    /* 3EDD  push    word ptr ds:[OBJ_SHIFT] */
    push16(rw(pDS, 0x2880));
L3EE1:
    /* 3EE1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3EE2:
    /* 3EE2  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3EE3:
    /* 3EE3  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3EE7  (+3EE7)
       Opcode 56h: pops what do_pushc pushed and calls self_modify. */
L3EE7: /* _do_popc */
    /* 3EE7  pop     word ptr ds:[OBJ_SHIFT] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2880, t_); }
L3EEB:
    /* 3EEB  pop     word ptr ds:[OBJ_Z] */
    { uint16_t t_ = pop16(); ww(pDS, 0x288A, t_); }
L3EEF:
    /* 3EEF  pop     word ptr ds:[OBJ_Y] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2888, t_); }
L3EF3:
    /* 3EF3  pop     word ptr ds:[OBJ_X] */
    { uint16_t t_ = pop16(); ww(pDS, 0x2886, t_); }
L3EF7:
    /* 3EF7  call    _self_modify */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5069), 0x3EFA)) != 0) return c;
L3EFA:
    /* 3EFA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3EFB:
    /* 3EFB  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3EFC:
    /* 3EFC  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3F00  (+3F00)
       Opcode 5Eh: do_jnorm with no x term. */
L3F00: /* _do_jnorm_x0 */
    /* 3F00  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F01:
    /* 3F01  mov     bx,ax */
    BX = AX;
L3F03:
    /* 3F03  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F04:
    /* 3F04  mov     di,ax */
    DI = AX;
L3F06:
    /* 3F06  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F07:
    /* 3F07  add     ax,word ptr ds:[OBJ_Y] */
    AX = (uint16_t)(AX + rw(pDS, 0x2888));
L3F0B:
    /* 3F0B  imul    di */
    imul16(DI);
L3F0D:
    /* 3F0D  mov     bp,ax */
    BP = AX;
L3F0F:
    /* 3F0F  mov     cx,dx */
    CX = DX;
L3F11:
    /* 3F11  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F12:
    /* 3F12  mov     di,ax */
    DI = AX;
L3F14:
    /* 3F14  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F15:
    /* 3F15  add     ax,word ptr ds:[OBJ_Z] */
    AX = (uint16_t)(AX + rw(pDS, 0x288A));
L3F19:
    /* 3F19  imul    di */
    imul16(DI);
L3F1B:
    /* 3F1B  add     bp,ax */
    BP = add16(BP, AX, 0);
L3F1D:
    /* 3F1D  adc     cx,dx */
    CX = add16(CX, DX, CF);
L3F1F:
    /* 3F1F  jns     short L3F23 */
    if (!SF) goto L3F23;
L3F21:
    /* 3F21  add     si,bx */
    SI = add16(SI, BX, 0);
L3F23: /* L3F23 */
    /* 3F23  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F24:
    /* 3F24  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3F25:
    /* 3F25  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3F29  (+3F29)
       Opcode 60h: do_jnorm with no y term. */
L3F29: /* _do_jnorm_y0 */
    /* 3F29  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F2A:
    /* 3F2A  mov     bx,ax */
    BX = AX;
L3F2C:
    /* 3F2C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F2D:
    /* 3F2D  mov     cx,ax */
    CX = AX;
L3F2F:
    /* 3F2F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F30:
    /* 3F30  add     ax,word ptr ds:[OBJ_X] */
    AX = (uint16_t)(AX + rw(pDS, 0x2886));
L3F34:
    /* 3F34  imul    cx */
    imul16(CX);
L3F36:
    /* 3F36  mov     cx,dx */
    CX = DX;
L3F38:
    /* 3F38  mov     bp,ax */
    BP = AX;
L3F3A:
    /* 3F3A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F3B:
    /* 3F3B  mov     di,ax */
    DI = AX;
L3F3D:
    /* 3F3D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F3E:
    /* 3F3E  add     ax,word ptr ds:[OBJ_Z] */
    AX = (uint16_t)(AX + rw(pDS, 0x288A));
L3F42:
    /* 3F42  imul    di */
    imul16(DI);
L3F44:
    /* 3F44  add     bp,ax */
    BP = add16(BP, AX, 0);
L3F46:
    /* 3F46  adc     cx,dx */
    CX = add16(CX, DX, CF);
L3F48:
    /* 3F48  jns     short L3F4C */
    if (!SF) goto L3F4C;
L3F4A:
    /* 3F4A  add     si,bx */
    SI = add16(SI, BX, 0);
L3F4C: /* L3F4C */
    /* 3F4C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F4D:
    /* 3F4D  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3F4E:
    /* 3F4E  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3F52  (+3F52)
       Opcode 62h: do_jnorm with no z term. */
L3F52: /* _do_jnorm_z0 */
    /* 3F52  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F53:
    /* 3F53  mov     bx,ax */
    BX = AX;
L3F55:
    /* 3F55  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F56:
    /* 3F56  mov     cx,ax */
    CX = AX;
L3F58:
    /* 3F58  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F59:
    /* 3F59  add     ax,word ptr ds:[OBJ_X] */
    AX = (uint16_t)(AX + rw(pDS, 0x2886));
L3F5D:
    /* 3F5D  imul    cx */
    imul16(CX);
L3F5F:
    /* 3F5F  mov     cx,dx */
    CX = DX;
L3F61:
    /* 3F61  mov     bp,ax */
    BP = AX;
L3F63:
    /* 3F63  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F64:
    /* 3F64  mov     di,ax */
    DI = AX;
L3F66:
    /* 3F66  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F67:
    /* 3F67  add     ax,word ptr ds:[OBJ_Y] */
    AX = (uint16_t)(AX + rw(pDS, 0x2888));
L3F6B:
    /* 3F6B  imul    di */
    imul16(DI);
L3F6D:
    /* 3F6D  add     bp,ax */
    BP = add16(BP, AX, 0);
L3F6F:
    /* 3F6F  adc     cx,dx */
    CX = add16(CX, DX, CF);
L3F71:
    /* 3F71  jns     short L3F75 */
    if (!SF) goto L3F75;
L3F73:
    /* 3F73  add     si,bx */
    SI = add16(SI, BX, 0);
L3F75: /* L3F75 */
    /* 3F75  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F76:
    /* 3F76  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3F77:
    /* 3F77  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3F7B  (+3F7B)
       Opcode 64h, operands: skip, nx, px. do_jnorm for a normal along x alone: only the sign of
       (px + origin x) against nx is tested. */
L3F7B: /* _do_jnorm_yz0 */
    /* 3F7B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F7C:
    /* 3F7C  mov     bx,ax */
    BX = AX;
L3F7E:
    /* 3F7E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F7F:
    /* 3F7F  mov     cx,ax */
    CX = AX;
L3F81:
    /* 3F81  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F82:
    /* 3F82  add     ax,word ptr ds:[OBJ_X] */
    AX = (uint16_t)(AX + rw(pDS, 0x2886));
L3F86:
    /* 3F86  xor     ax,cx */
    AX = logic16((uint16_t)(AX ^ CX));
L3F88:
    /* 3F88  jns     short L3F8C */
    if (!SF) goto L3F8C;
L3F8A:
    /* 3F8A  add     si,bx */
    SI = add16(SI, BX, 0);
L3F8C: /* L3F8C */
    /* 3F8C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F8D:
    /* 3F8D  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3F8E:
    /* 3F8E  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3F92  (+3F92)
       Opcode 66h: the same for a normal along y. */
L3F92: /* _do_jnorm_xz0 */
    /* 3F92  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F93:
    /* 3F93  mov     bx,ax */
    BX = AX;
L3F95:
    /* 3F95  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F96:
    /* 3F96  mov     di,ax */
    DI = AX;
L3F98:
    /* 3F98  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3F99:
    /* 3F99  add     ax,word ptr ds:[OBJ_Y] */
    AX = (uint16_t)(AX + rw(pDS, 0x2888));
L3F9D:
    /* 3F9D  xor     ax,di */
    AX = logic16((uint16_t)(AX ^ DI));
L3F9F:
    /* 3F9F  jns     short L3FA3 */
    if (!SF) goto L3FA3;
L3FA1:
    /* 3FA1  add     si,bx */
    SI = add16(SI, BX, 0);
L3FA3: /* L3FA3 */
    /* 3FA3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3FA4:
    /* 3FA4  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3FA5:
    /* 3FA5  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3FA9  (+3FA9)
       Opcode 68h: the same for a normal along z. */
L3FA9: /* _do_jnorm_xy0 */
    /* 3FA9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3FAA:
    /* 3FAA  mov     bx,ax */
    BX = AX;
L3FAC:
    /* 3FAC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3FAD:
    /* 3FAD  mov     di,ax */
    DI = AX;
L3FAF:
    /* 3FAF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3FB0:
    /* 3FB0  add     ax,word ptr ds:[OBJ_Z] */
    AX = (uint16_t)(AX + rw(pDS, 0x288A));
L3FB4:
    /* 3FB4  xor     ax,di */
    AX = logic16((uint16_t)(AX ^ DI));
L3FB6:
    /* 3FB6  jns     short L3FBA */
    if (!SF) goto L3FBA;
L3FB8:
    /* 3FB8  add     si,bx */
    SI = add16(SI, BX, 0);
L3FBA: /* L3FBA */
    /* 3FBA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3FBB:
    /* 3FBB  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3FBC:
    /* 3FBC  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3FC0  (+3FC0)
       Opcode 58h, operands: skip, then (n, p) for x, y, z. Back-face test: the same sum as
       do_sortnorm; when it is negative (the eye on the back of the plane) skips `skip` bytes,
       normally the face that follows. (System Shock: do_jnorm.) The most used opcode in UW1's
       models after polygons and x_rel (model_opcodes.tsv). */
L3FC0: /* _do_jnorm */
    /* 3FC0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3FC1:
    /* 3FC1  mov     bx,ax */
    BX = AX;
L3FC3:
    /* 3FC3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3FC4:
    /* 3FC4  mov     cx,ax */
    CX = AX;
L3FC6:
    /* 3FC6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3FC7:
    /* 3FC7  add     ax,word ptr ds:[OBJ_X] */
    AX = (uint16_t)(AX + rw(pDS, 0x2886));
L3FCB:
    /* 3FCB  imul    cx */
    imul16(CX);
L3FCD:
    /* 3FCD  mov     cx,dx */
    CX = DX;
L3FCF:
    /* 3FCF  mov     bp,ax */
    BP = AX;
L3FD1:
    /* 3FD1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3FD2:
    /* 3FD2  mov     di,ax */
    DI = AX;
L3FD4:
    /* 3FD4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3FD5:
    /* 3FD5  add     ax,word ptr ds:[OBJ_Y] */
    AX = (uint16_t)(AX + rw(pDS, 0x2888));
L3FD9:
    /* 3FD9  imul    di */
    imul16(DI);
L3FDB:
    /* 3FDB  add     bp,ax */
    BP = add16(BP, AX, 0);
L3FDD:
    /* 3FDD  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L3FDF:
    /* 3FDF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3FE0:
    /* 3FE0  mov     di,ax */
    DI = AX;
L3FE2:
    /* 3FE2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3FE3:
    /* 3FE3  add     ax,word ptr ds:[OBJ_Z] */
    AX = (uint16_t)(AX + rw(pDS, 0x288A));
L3FE7:
    /* 3FE7  imul    di */
    imul16(DI);
L3FE9:
    /* 3FE9  add     bp,ax */
    BP = add16(BP, AX, 0);
L3FEB:
    /* 3FEB  adc     cx,dx */
    CX = add16(CX, DX, CF);
L3FED:
    /* 3FED  jns     short L3FF1 */
    if (!SF) goto L3FF1;
L3FEF:
    /* 3FEF  add     si,bx */
    SI = add16(SI, BX, 0);
L3FF1: /* L3FF1 */
    /* 3FF1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3FF2:
    /* 3FF2  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3FF3:
    /* 3FF3  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_3FF7  (+3FF7)
       Recovery for do_closure's projection (stored at ss:[4D5h] in clip_needed): drops the int 0
       frame (sti; add sp,6) and falls into do_3d_clip. */
L3FF7: /* _clip_overflow */
    /* 3FF7  sti */
    ;
L3FF8:
    /* 3FF8  add     sp,6 */
    SP = (uint16_t)(SP + 0x6);

    /* seg004_3FFB  (+3FFB)
       Clips the polygon in the vertex buffer in 3D against each plane its codes OR shows it
       crossing (right, left, top, bottom), stopping as soon as the codes AND to nonzero (nothing
       left), and then projects and draws it (back to do_closure's L2F30). Restores DS and ES to
       seg051 when it gives up. */
L3FFB: /* _do_3d_clip */
    /* 3FFB  mov     word ptr ss:[4D5h],offset _overflow_handler_reg */
    ww(pSS, 0x4D5, 0x5BD1);
L4002:
    /* 4002  mov     ax,ds */
    AX = asm_ds;
L4004:
    /* 4004  mov     es,ax */
    SET_ES(AX);
L4006:
    /* 4006  test    byte ptr ds:[CLIP_OR],2 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x2));
L400B:
    /* 400B  je      short L4017 */
    if (ZF) goto L4017;
L400D:
    /* 400D  call    _clip_right */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x4274), 0x4010)) != 0) return c;
L4010:
    /* 4010  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L4015:
    /* 4015  jne     short L404D */
    if (!ZF) goto L404D;
L4017: /* L4017 */
    /* 4017  test    byte ptr ds:[CLIP_OR],1 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x1));
L401C:
    /* 401C  je      short L4028 */
    if (ZF) goto L4028;
L401E:
    /* 401E  call    _clip_left */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x4382), 0x4021)) != 0) return c;
L4021:
    /* 4021  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L4026:
    /* 4026  jne     short L404D */
    if (!ZF) goto L404D;
L4028: /* L4028 */
    /* 4028  test    byte ptr ds:[CLIP_OR],4 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x4));
L402D:
    /* 402D  je      short L4039 */
    if (ZF) goto L4039;
L402F:
    /* 402F  call    _clip_top */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x405B), 0x4032)) != 0) return c;
L4032:
    /* 4032  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L4037:
    /* 4037  jne     short L404D */
    if (!ZF) goto L404D;
L4039: /* L4039 */
    /* 4039  test    byte ptr ds:[CLIP_OR],8 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x8));
L403E:
    /* 403E  je      short L404A */
    if (ZF) goto L404A;
L4040:
    /* 4040  call    _clip_bot */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x4164), 0x4043)) != 0) return c;
L4043:
    /* 4043  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L4048:
    /* 4048  jne     short L404D */
    if (!ZF) goto L404D;
L404A: /* L404A */
    /* 404A  jmp     L2F30 */
    goto L2F30;
L404D: /* L404D */
    /* 404D  pop     si */
    SI = pop16();
L404E:
    /* 404E  mov     ax,seg seg051 */
    AX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L4051:
    /* 4051  mov     ds,ax */
    SET_DS(AX);
L4053:
    /* 4053  mov     es,ax */
    SET_ES(AX);
L4055:
    /* 4055  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L4056:
    /* 4056  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L4057:
    /* 4057  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_405B  (+405B)
       Clips the vertex buffer against the y = z plane (code bit 4): copies the first vertices past
       the end to close the loop, then walks the edges writing the surviving vertices and the
       intersections into the other buffer (0C69h or 309h), recomputing the codes OR and AND.
       The intersection uses fixed-point interpolation with rounding (sar ax,1; adc). The other
       three are the same for the other planes. */
L405B: /* _clip_top */
    /* 405B  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L4060:
    /* 4060  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L4065:
    /* 4065  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L4069:
    /* 4069  mov     di,word ptr ds:[VB_END] */
    DI = rw(pDS, 0x305);
L406D:
    /* 406D  mov     cx,8 */
    CX = 0x8;
L4070:
    /* 4070  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L4072:
    /* 4072  add     word ptr ds:[VB_END],8 */
    ww(pDS, 0x305, (uint16_t)(rw(pDS, 0x305) + 0x8));
L4077:
    /* 4077  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L407B:
    /* 407B  mov     di,VBUF1 */
    DI = 0xC69;
L407E:
    /* 407E  cmp     si,VBUF0 */
    sub16(SI, 0x309, 0);
L4082:
    /* 4082  je      short L4087 */
    if (ZF) goto L4087;
L4084:
    /* 4084  mov     di,VBUF0 */
    DI = 0x309;
L4087: /* L4087 */
    /* 4087  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L408B: /* L408B */
    /* 408B  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L408E: /* L408E */
    /* 408E  cmp     si,word ptr ds:[VB_END] */
    sub16(SI, rw(pDS, 0x305), 0);
L4092:
    /* 4092  jne     short L4097 */
    if (!ZF) goto L4097;
L4094:
    /* 4094  jmp     L415F */
    goto L415F;
L4097: /* L4097 */
    /* 4097  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L409A:
    /* 409A  test    al,4 */
    logic8((uint8_t)(AL & 0x4));
L409C:
    /* 409C  jne     short L40AD */
    if (!ZF) goto L40AD;
L409E:
    /* 409E  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L40A2:
    /* 40A2  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L40A6:
    /* 40A6  mov     cx,4 */
    CX = 0x4;
L40A9:
    /* 40A9  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L40AB:
    /* 40AB  jmp     L408E */
    goto L408E;
L40AD: /* L40AD */
    /* 40AD  test    byte ptr [si-2],4 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFE) & 0x4));
L40B1:
    /* 40B1  jne     short L4105 */
    if (!ZF) goto L4105;
L40B3:
    /* 40B3  mov     bp,word ptr [si-4] */
    BP = rw(pDS, SI + 0xFFFC);
L40B6:
    /* 40B6  sub     bp,word ptr [si-6] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0xFFFA));
L40B9:
    /* 40B9  mov     cx,bp */
    CX = BP;
L40BB:
    /* 40BB  sub     cx,word ptr [si+4] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x4));
L40BE:
    /* 40BE  add     cx,word ptr [si+2] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0x2));
L40C1:
    /* 40C1  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L40C3:
    /* 40C3  sub     ax,word ptr [si-8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF8));
L40C6:
    /* 40C6  imul    bp */
    imul16(BP);
L40C8:
    /* 40C8  shl     ax,1 */
    AX = shl16(AX, 1);
L40CA:
    /* 40CA  rcl     dx,1 */
    DX = rcl16(DX, 1);
L40CC:
    /* 40CC  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x40CC, 2)) != 0) return c;
L40CE:
    /* 40CE  sar     ax,1 */
    AX = sar16(AX, 1);
L40D0:
    /* 40D0  jae     short L40D5 */
    if (!CF) goto L40D5;
L40D2:
    /* 40D2  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L40D3:
    /* 40D3  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L40D5: /* L40D5 */
    /* 40D5  add     ax,word ptr [si-8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF8));
L40D8:
    /* 40D8  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L40D9:
    /* 40D9  mov     bx,ax */
    BX = AX;
L40DB:
    /* 40DB  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L40DE:
    /* 40DE  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L40E1:
    /* 40E1  imul    bp */
    imul16(BP);
L40E3:
    /* 40E3  shl     ax,1 */
    AX = shl16(AX, 1);
L40E5:
    /* 40E5  rcl     dx,1 */
    DX = rcl16(DX, 1);
L40E7:
    /* 40E7  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x40E7, 2)) != 0) return c;
L40E9:
    /* 40E9  sar     ax,1 */
    AX = sar16(AX, 1);
L40EB:
    /* 40EB  jae     short L40F0 */
    if (!CF) goto L40F0;
L40ED:
    /* 40ED  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L40EE:
    /* 40EE  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L40F0: /* L40F0 */
    /* 40F0  add     ax,word ptr [si-6] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFA));
L40F3:
    /* 40F3  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L40F4:
    /* 40F4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L40F5:
    /* 40F5  mov     cx,ax */
    CX = AX;
L40F7:
    /* 40F7  mov     bp,ax */
    BP = AX;
L40F9:
    /* 40F9  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x40FC)) != 0) return c;
L40FC:
    /* 40FC  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L40FD:
    /* 40FD  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L4101:
    /* 4101  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L4105: /* L4105 */
    /* 4105  test    byte ptr [si+0Eh],4 */
    logic8((uint8_t)(rb(pDS, SI + 0xE) & 0x4));
L4109:
    /* 4109  jne     L408B */
    if (!ZF) goto L408B;
L410B:
    /* 410B  mov     bp,word ptr [si+4] */
    BP = rw(pDS, SI + 0x4);
L410E:
    /* 410E  sub     bp,word ptr [si+2] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0x2));
L4111:
    /* 4111  mov     cx,bp */
    CX = BP;
L4113:
    /* 4113  add     cx,word ptr [si+0Ah] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0xA));
L4116:
    /* 4116  sub     cx,word ptr [si+0Ch] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xC));
L4119:
    /* 4119  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L411C:
    /* 411C  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L411E:
    /* 411E  imul    bp */
    imul16(BP);
L4120:
    /* 4120  shl     ax,1 */
    AX = shl16(AX, 1);
L4122:
    /* 4122  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4124:
    /* 4124  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x4124, 2)) != 0) return c;
L4126:
    /* 4126  sar     ax,1 */
    AX = sar16(AX, 1);
L4128:
    /* 4128  jae     short L412D */
    if (!CF) goto L412D;
L412A:
    /* 412A  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L412B:
    /* 412B  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L412D: /* L412D */
    /* 412D  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L412F:
    /* 412F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4130:
    /* 4130  mov     bx,ax */
    BX = AX;
L4132:
    /* 4132  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L4135:
    /* 4135  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L4138:
    /* 4138  imul    bp */
    imul16(BP);
L413A:
    /* 413A  shl     ax,1 */
    AX = shl16(AX, 1);
L413C:
    /* 413C  rcl     dx,1 */
    DX = rcl16(DX, 1);
L413E:
    /* 413E  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x413E, 2)) != 0) return c;
L4140:
    /* 4140  sar     ax,1 */
    AX = sar16(AX, 1);
L4142:
    /* 4142  jae     short L4147 */
    if (!CF) goto L4147;
L4144:
    /* 4144  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L4145:
    /* 4145  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L4147: /* L4147 */
    /* 4147  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L414A:
    /* 414A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L414B:
    /* 414B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L414C:
    /* 414C  mov     cx,ax */
    CX = AX;
L414E:
    /* 414E  mov     bp,ax */
    BP = AX;
L4150:
    /* 4150  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x4153)) != 0) return c;
L4153:
    /* 4153  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4154:
    /* 4154  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L4158:
    /* 4158  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L415C:
    /* 415C  jmp     L408B */
    goto L408B;
L415F: /* L415F */
    /* 415F  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L4163:
    /* 4163  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_4164  (+4164)
       Clips against y = -z (code bit 8). See clip_top. */
L4164: /* _clip_bot */
    /* 4164  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L4169:
    /* 4169  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L416E:
    /* 416E  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L4172:
    /* 4172  mov     di,word ptr ds:[VB_END] */
    DI = rw(pDS, 0x305);
L4176:
    /* 4176  mov     cx,8 */
    CX = 0x8;
L4179:
    /* 4179  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L417B:
    /* 417B  add     word ptr ds:[VB_END],8 */
    ww(pDS, 0x305, (uint16_t)(rw(pDS, 0x305) + 0x8));
L4180:
    /* 4180  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L4184:
    /* 4184  mov     di,VBUF1 */
    DI = 0xC69;
L4187:
    /* 4187  cmp     si,VBUF0 */
    sub16(SI, 0x309, 0);
L418B:
    /* 418B  je      short L4190 */
    if (ZF) goto L4190;
L418D:
    /* 418D  mov     di,VBUF0 */
    DI = 0x309;
L4190: /* L4190 */
    /* 4190  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L4194: /* L4194 */
    /* 4194  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L4197: /* L4197 */
    /* 4197  cmp     si,word ptr ds:[VB_END] */
    sub16(SI, rw(pDS, 0x305), 0);
L419B:
    /* 419B  jne     short L41A0 */
    if (!ZF) goto L41A0;
L419D:
    /* 419D  jmp     L426F */
    goto L426F;
L41A0: /* L41A0 */
    /* 41A0  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L41A3:
    /* 41A3  test    al,8 */
    logic8((uint8_t)(AL & 0x8));
L41A5:
    /* 41A5  jne     short L41B6 */
    if (!ZF) goto L41B6;
L41A7:
    /* 41A7  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L41AB:
    /* 41AB  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L41AF:
    /* 41AF  mov     cx,4 */
    CX = 0x4;
L41B2:
    /* 41B2  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L41B4:
    /* 41B4  jmp     L4197 */
    goto L4197;
L41B6: /* L41B6 */
    /* 41B6  test    byte ptr [si-2],8 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFE) & 0x8));
L41BA:
    /* 41BA  jne     short L4210 */
    if (!ZF) goto L4210;
L41BC:
    /* 41BC  mov     bp,word ptr [si-4] */
    BP = rw(pDS, SI + 0xFFFC);
L41BF:
    /* 41BF  add     bp,word ptr [si-6] */
    BP = (uint16_t)(BP + rw(pDS, SI + 0xFFFA));
L41C2:
    /* 41C2  mov     cx,bp */
    CX = BP;
L41C4:
    /* 41C4  sub     cx,word ptr [si+4] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x4));
L41C7:
    /* 41C7  sub     cx,word ptr [si+2] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x2));
L41CA:
    /* 41CA  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L41CC:
    /* 41CC  sub     ax,word ptr [si-8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF8));
L41CF:
    /* 41CF  imul    bp */
    imul16(BP);
L41D1:
    /* 41D1  shl     ax,1 */
    AX = shl16(AX, 1);
L41D3:
    /* 41D3  rcl     dx,1 */
    DX = rcl16(DX, 1);
L41D5:
    /* 41D5  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x41D5, 2)) != 0) return c;
L41D7:
    /* 41D7  sar     ax,1 */
    AX = sar16(AX, 1);
L41D9:
    /* 41D9  jae     short L41DE */
    if (!CF) goto L41DE;
L41DB:
    /* 41DB  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L41DC:
    /* 41DC  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L41DE: /* L41DE */
    /* 41DE  add     ax,word ptr [si-8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF8));
L41E1:
    /* 41E1  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L41E2:
    /* 41E2  mov     bx,ax */
    BX = AX;
L41E4:
    /* 41E4  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L41E7:
    /* 41E7  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L41EA:
    /* 41EA  imul    bp */
    imul16(BP);
L41EC:
    /* 41EC  shl     ax,1 */
    AX = shl16(AX, 1);
L41EE:
    /* 41EE  rcl     dx,1 */
    DX = rcl16(DX, 1);
L41F0:
    /* 41F0  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x41F0, 2)) != 0) return c;
L41F2:
    /* 41F2  sar     ax,1 */
    AX = sar16(AX, 1);
L41F4:
    /* 41F4  jae     short L41F9 */
    if (!CF) goto L41F9;
L41F6:
    /* 41F6  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L41F7:
    /* 41F7  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L41F9: /* L41F9 */
    /* 41F9  add     ax,word ptr [si-6] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFA));
L41FC:
    /* 41FC  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L41FD:
    /* 41FD  mov     cx,ax */
    CX = AX;
L41FF:
    /* 41FF  neg     ax */
    AX = (uint16_t)-AX;
L4201:
    /* 4201  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4202:
    /* 4202  mov     bp,ax */
    BP = AX;
L4204:
    /* 4204  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x4207)) != 0) return c;
L4207:
    /* 4207  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4208:
    /* 4208  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L420C:
    /* 420C  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L4210: /* L4210 */
    /* 4210  test    byte ptr [si+0Eh],8 */
    logic8((uint8_t)(rb(pDS, SI + 0xE) & 0x8));
L4214:
    /* 4214  je      short L4219 */
    if (ZF) goto L4219;
L4216:
    /* 4216  jmp     L4194 */
    goto L4194;
L4219: /* L4219 */
    /* 4219  mov     bp,word ptr [si+4] */
    BP = rw(pDS, SI + 0x4);
L421C:
    /* 421C  add     bp,word ptr [si+2] */
    BP = (uint16_t)(BP + rw(pDS, SI + 0x2));
L421F:
    /* 421F  mov     cx,bp */
    CX = BP;
L4221:
    /* 4221  sub     cx,word ptr [si+0Ah] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xA));
L4224:
    /* 4224  sub     cx,word ptr [si+0Ch] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xC));
L4227:
    /* 4227  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L422A:
    /* 422A  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L422C:
    /* 422C  imul    bp */
    imul16(BP);
L422E:
    /* 422E  shl     ax,1 */
    AX = shl16(AX, 1);
L4230:
    /* 4230  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4232:
    /* 4232  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x4232, 2)) != 0) return c;
L4234:
    /* 4234  sar     ax,1 */
    AX = sar16(AX, 1);
L4236:
    /* 4236  jae     short L423B */
    if (!CF) goto L423B;
L4238:
    /* 4238  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L4239:
    /* 4239  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L423B: /* L423B */
    /* 423B  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L423D:
    /* 423D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L423E:
    /* 423E  mov     bx,ax */
    BX = AX;
L4240:
    /* 4240  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L4243:
    /* 4243  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L4246:
    /* 4246  imul    bp */
    imul16(BP);
L4248:
    /* 4248  shl     ax,1 */
    AX = shl16(AX, 1);
L424A:
    /* 424A  rcl     dx,1 */
    DX = rcl16(DX, 1);
L424C:
    /* 424C  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x424C, 2)) != 0) return c;
L424E:
    /* 424E  sar     ax,1 */
    AX = sar16(AX, 1);
L4250:
    /* 4250  jae     short L4255 */
    if (!CF) goto L4255;
L4252:
    /* 4252  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L4253:
    /* 4253  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L4255: /* L4255 */
    /* 4255  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L4258:
    /* 4258  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4259:
    /* 4259  mov     cx,ax */
    CX = AX;
L425B:
    /* 425B  neg     ax */
    AX = (uint16_t)-AX;
L425D:
    /* 425D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L425E:
    /* 425E  mov     bp,ax */
    BP = AX;
L4260:
    /* 4260  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x4263)) != 0) return c;
L4263:
    /* 4263  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4264:
    /* 4264  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L4268:
    /* 4268  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L426C:
    /* 426C  jmp     L4194 */
    goto L4194;
L426F: /* L426F */
    /* 426F  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L4273:
    /* 4273  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_4274  (+4274)
       Clips against x = z (code bit 2). See clip_top. */
L4274: /* _clip_right */
    /* 4274  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L4279:
    /* 4279  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L427E:
    /* 427E  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L4282:
    /* 4282  mov     di,word ptr ds:[VB_END] */
    DI = rw(pDS, 0x305);
L4286:
    /* 4286  mov     cx,8 */
    CX = 0x8;
L4289:
    /* 4289  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L428B:
    /* 428B  add     word ptr ds:[VB_END],8 */
    ww(pDS, 0x305, (uint16_t)(rw(pDS, 0x305) + 0x8));
L4290:
    /* 4290  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L4294:
    /* 4294  mov     di,VBUF1 */
    DI = 0xC69;
L4297:
    /* 4297  cmp     si,VBUF0 */
    sub16(SI, 0x309, 0);
L429B:
    /* 429B  je      short L42A0 */
    if (ZF) goto L42A0;
L429D:
    /* 429D  mov     di,VBUF0 */
    DI = 0x309;
L42A0: /* L42A0 */
    /* 42A0  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L42A4: /* L42A4 */
    /* 42A4  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L42A7: /* L42A7 */
    /* 42A7  cmp     si,word ptr ds:[VB_END] */
    sub16(SI, rw(pDS, 0x305), 0);
L42AB:
    /* 42AB  jne     short L42B0 */
    if (!ZF) goto L42B0;
L42AD:
    /* 42AD  jmp     L437D */
    goto L437D;
L42B0: /* L42B0 */
    /* 42B0  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L42B3:
    /* 42B3  test    al,2 */
    logic8((uint8_t)(AL & 0x2));
L42B5:
    /* 42B5  jne     short L42C6 */
    if (!ZF) goto L42C6;
L42B7:
    /* 42B7  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L42BB:
    /* 42BB  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L42BF:
    /* 42BF  mov     cx,4 */
    CX = 0x4;
L42C2:
    /* 42C2  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L42C4:
    /* 42C4  jmp     L42A7 */
    goto L42A7;
L42C6: /* L42C6 */
    /* 42C6  test    byte ptr [si-2],2 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFE) & 0x2));
L42CA:
    /* 42CA  jne     short L431F */
    if (!ZF) goto L431F;
L42CC:
    /* 42CC  mov     bp,word ptr [si-8] */
    BP = rw(pDS, SI + 0xFFF8);
L42CF:
    /* 42CF  sub     bp,word ptr [si-4] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0xFFFC));
L42D2:
    /* 42D2  mov     cx,bp */
    CX = BP;
L42D4:
    /* 42D4  add     cx,word ptr [si+4] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0x4));
L42D7:
    /* 42D7  sub     cx,word ptr [si] */
    CX = (uint16_t)(CX - rw(pDS, SI));
L42D9:
    /* 42D9  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L42DB:
    /* 42DB  sub     ax,word ptr [si-8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF8));
L42DE:
    /* 42DE  imul    bp */
    imul16(BP);
L42E0:
    /* 42E0  shl     ax,1 */
    AX = shl16(AX, 1);
L42E2:
    /* 42E2  rcl     dx,1 */
    DX = rcl16(DX, 1);
L42E4:
    /* 42E4  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x42E4, 2)) != 0) return c;
L42E6:
    /* 42E6  sar     ax,1 */
    AX = sar16(AX, 1);
L42E8:
    /* 42E8  jae     short L42ED */
    if (!CF) goto L42ED;
L42EA:
    /* 42EA  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L42EB:
    /* 42EB  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L42ED: /* L42ED */
    /* 42ED  add     ax,word ptr [si-8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF8));
L42F0:
    /* 42F0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L42F1:
    /* 42F1  mov     bx,ax */
    BX = AX;
L42F3:
    /* 42F3  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L42F6:
    /* 42F6  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L42F9:
    /* 42F9  imul    bp */
    imul16(BP);
L42FB:
    /* 42FB  shl     ax,1 */
    AX = shl16(AX, 1);
L42FD:
    /* 42FD  rcl     dx,1 */
    DX = rcl16(DX, 1);
L42FF:
    /* 42FF  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x42FF, 2)) != 0) return c;
L4301:
    /* 4301  sar     ax,1 */
    AX = sar16(AX, 1);
L4303:
    /* 4303  jae     short L4308 */
    if (!CF) goto L4308;
L4305:
    /* 4305  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L4306:
    /* 4306  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L4308: /* L4308 */
    /* 4308  add     ax,word ptr [si-6] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFA));
L430B:
    /* 430B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L430C:
    /* 430C  mov     cx,ax */
    CX = AX;
L430E:
    /* 430E  mov     ax,bx */
    AX = BX;
L4310:
    /* 4310  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4311:
    /* 4311  mov     bp,ax */
    BP = AX;
L4313:
    /* 4313  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x4316)) != 0) return c;
L4316:
    /* 4316  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4317:
    /* 4317  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L431B:
    /* 431B  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L431F: /* L431F */
    /* 431F  test    byte ptr [si+0Eh],2 */
    logic8((uint8_t)(rb(pDS, SI + 0xE) & 0x2));
L4323:
    /* 4323  je      short L4328 */
    if (ZF) goto L4328;
L4325:
    /* 4325  jmp     L42A4 */
    goto L42A4;
L4328: /* L4328 */
    /* 4328  mov     bp,word ptr [si] */
    BP = rw(pDS, SI);
L432A:
    /* 432A  sub     bp,word ptr [si+4] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0x4));
L432D:
    /* 432D  mov     cx,bp */
    CX = BP;
L432F:
    /* 432F  add     cx,word ptr [si+0Ch] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0xC));
L4332:
    /* 4332  sub     cx,word ptr [si+8] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x8));
L4335:
    /* 4335  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L4338:
    /* 4338  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L433A:
    /* 433A  imul    bp */
    imul16(BP);
L433C:
    /* 433C  shl     ax,1 */
    AX = shl16(AX, 1);
L433E:
    /* 433E  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4340:
    /* 4340  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x4340, 2)) != 0) return c;
L4342:
    /* 4342  sar     ax,1 */
    AX = sar16(AX, 1);
L4344:
    /* 4344  jae     short L4349 */
    if (!CF) goto L4349;
L4346:
    /* 4346  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L4347:
    /* 4347  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L4349: /* L4349 */
    /* 4349  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L434B:
    /* 434B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L434C:
    /* 434C  mov     bx,ax */
    BX = AX;
L434E:
    /* 434E  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L4351:
    /* 4351  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L4354:
    /* 4354  imul    bp */
    imul16(BP);
L4356:
    /* 4356  shl     ax,1 */
    AX = shl16(AX, 1);
L4358:
    /* 4358  rcl     dx,1 */
    DX = rcl16(DX, 1);
L435A:
    /* 435A  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x435A, 2)) != 0) return c;
L435C:
    /* 435C  sar     ax,1 */
    AX = sar16(AX, 1);
L435E:
    /* 435E  jae     short L4363 */
    if (!CF) goto L4363;
L4360:
    /* 4360  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L4361:
    /* 4361  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L4363: /* L4363 */
    /* 4363  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L4366:
    /* 4366  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4367:
    /* 4367  mov     cx,ax */
    CX = AX;
L4369:
    /* 4369  mov     ax,bx */
    AX = BX;
L436B:
    /* 436B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L436C:
    /* 436C  mov     bp,ax */
    BP = AX;
L436E:
    /* 436E  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x4371)) != 0) return c;
L4371:
    /* 4371  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4372:
    /* 4372  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L4376:
    /* 4376  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L437A:
    /* 437A  jmp     L42A4 */
    goto L42A4;
L437D: /* L437D */
    /* 437D  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L4381:
    /* 4381  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_4382  (+4382)
       Clips against x = -z (code bit 1). See clip_top. */
L4382: /* _clip_left */
    /* 4382  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L4387:
    /* 4387  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L438C:
    /* 438C  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L4390:
    /* 4390  mov     di,word ptr ds:[VB_END] */
    DI = rw(pDS, 0x305);
L4394:
    /* 4394  mov     cx,8 */
    CX = 0x8;
L4397:
    /* 4397  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L4399:
    /* 4399  add     word ptr ds:[VB_END],8 */
    ww(pDS, 0x305, (uint16_t)(rw(pDS, 0x305) + 0x8));
L439E:
    /* 439E  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L43A2:
    /* 43A2  mov     di,VBUF1 */
    DI = 0xC69;
L43A5:
    /* 43A5  cmp     si,VBUF0 */
    sub16(SI, 0x309, 0);
L43A9:
    /* 43A9  je      short L43AE */
    if (ZF) goto L43AE;
L43AB:
    /* 43AB  mov     di,VBUF0 */
    DI = 0x309;
L43AE: /* L43AE */
    /* 43AE  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L43B2: /* L43B2 */
    /* 43B2  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L43B5: /* L43B5 */
    /* 43B5  cmp     si,word ptr ds:[VB_END] */
    sub16(SI, rw(pDS, 0x305), 0);
L43B9:
    /* 43B9  jne     short L43BE */
    if (!ZF) goto L43BE;
L43BB:
    /* 43BB  jmp     L448F */
    goto L448F;
L43BE: /* L43BE */
    /* 43BE  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L43C1:
    /* 43C1  test    al,1 */
    logic8((uint8_t)(AL & 0x1));
L43C3:
    /* 43C3  jne     short L43D4 */
    if (!ZF) goto L43D4;
L43C5:
    /* 43C5  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L43C9:
    /* 43C9  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L43CD:
    /* 43CD  mov     cx,4 */
    CX = 0x4;
L43D0:
    /* 43D0  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L43D2:
    /* 43D2  jmp     L43B5 */
    goto L43B5;
L43D4: /* L43D4 */
    /* 43D4  test    byte ptr [si-2],1 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFE) & 0x1));
L43D8:
    /* 43D8  jne     short L442F */
    if (!ZF) goto L442F;
L43DA:
    /* 43DA  mov     bp,word ptr [si-4] */
    BP = rw(pDS, SI + 0xFFFC);
L43DD:
    /* 43DD  add     bp,word ptr [si-8] */
    BP = (uint16_t)(BP + rw(pDS, SI + 0xFFF8));
L43E0:
    /* 43E0  mov     cx,bp */
    CX = BP;
L43E2:
    /* 43E2  sub     cx,word ptr [si+4] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x4));
L43E5:
    /* 43E5  sub     cx,word ptr [si] */
    CX = (uint16_t)(CX - rw(pDS, SI));
L43E7:
    /* 43E7  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L43E9:
    /* 43E9  sub     ax,word ptr [si-8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF8));
L43EC:
    /* 43EC  imul    bp */
    imul16(BP);
L43EE:
    /* 43EE  shl     ax,1 */
    AX = shl16(AX, 1);
L43F0:
    /* 43F0  rcl     dx,1 */
    DX = rcl16(DX, 1);
L43F2:
    /* 43F2  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x43F2, 2)) != 0) return c;
L43F4:
    /* 43F4  sar     ax,1 */
    AX = sar16(AX, 1);
L43F6:
    /* 43F6  jae     short L43FB */
    if (!CF) goto L43FB;
L43F8:
    /* 43F8  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L43F9:
    /* 43F9  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L43FB: /* L43FB */
    /* 43FB  add     ax,word ptr [si-8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF8));
L43FE:
    /* 43FE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L43FF:
    /* 43FF  mov     bx,ax */
    BX = AX;
L4401:
    /* 4401  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L4404:
    /* 4404  sub     ax,word ptr [si-6] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFA));
L4407:
    /* 4407  imul    bp */
    imul16(BP);
L4409:
    /* 4409  shl     ax,1 */
    AX = shl16(AX, 1);
L440B:
    /* 440B  rcl     dx,1 */
    DX = rcl16(DX, 1);
L440D:
    /* 440D  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x440D, 2)) != 0) return c;
L440F:
    /* 440F  sar     ax,1 */
    AX = sar16(AX, 1);
L4411:
    /* 4411  jae     short L4416 */
    if (!CF) goto L4416;
L4413:
    /* 4413  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L4414:
    /* 4414  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L4416: /* L4416 */
    /* 4416  add     ax,word ptr [si-6] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFA));
L4419:
    /* 4419  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L441A:
    /* 441A  mov     cx,ax */
    CX = AX;
L441C:
    /* 441C  mov     ax,bx */
    AX = BX;
L441E:
    /* 441E  neg     ax */
    AX = (uint16_t)-AX;
L4420:
    /* 4420  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4421:
    /* 4421  mov     bp,ax */
    BP = AX;
L4423:
    /* 4423  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x4426)) != 0) return c;
L4426:
    /* 4426  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4427:
    /* 4427  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L442B:
    /* 442B  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L442F: /* L442F */
    /* 442F  test    byte ptr [si+0Eh],1 */
    logic8((uint8_t)(rb(pDS, SI + 0xE) & 0x1));
L4433:
    /* 4433  je      short L4438 */
    if (ZF) goto L4438;
L4435:
    /* 4435  jmp     L43B2 */
    goto L43B2;
L4438: /* L4438 */
    /* 4438  mov     bp,word ptr [si+4] */
    BP = rw(pDS, SI + 0x4);
L443B:
    /* 443B  add     bp,word ptr [si] */
    BP = (uint16_t)(BP + rw(pDS, SI));
L443D:
    /* 443D  mov     cx,bp */
    CX = BP;
L443F:
    /* 443F  sub     cx,word ptr [si+8] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x8));
L4442:
    /* 4442  sub     cx,word ptr [si+0Ch] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xC));
L4445:
    /* 4445  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L4448:
    /* 4448  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L444A:
    /* 444A  imul    bp */
    imul16(BP);
L444C:
    /* 444C  shl     ax,1 */
    AX = shl16(AX, 1);
L444E:
    /* 444E  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4450:
    /* 4450  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x4450, 2)) != 0) return c;
L4452:
    /* 4452  sar     ax,1 */
    AX = sar16(AX, 1);
L4454:
    /* 4454  jae     short L4459 */
    if (!CF) goto L4459;
L4456:
    /* 4456  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L4457:
    /* 4457  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L4459: /* L4459 */
    /* 4459  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L445B:
    /* 445B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L445C:
    /* 445C  mov     bx,ax */
    BX = AX;
L445E:
    /* 445E  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L4461:
    /* 4461  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L4464:
    /* 4464  imul    bp */
    imul16(BP);
L4466:
    /* 4466  shl     ax,1 */
    AX = shl16(AX, 1);
L4468:
    /* 4468  rcl     dx,1 */
    DX = rcl16(DX, 1);
L446A:
    /* 446A  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x446A, 2)) != 0) return c;
L446C:
    /* 446C  sar     ax,1 */
    AX = sar16(AX, 1);
L446E:
    /* 446E  jae     short L4473 */
    if (!CF) goto L4473;
L4470:
    /* 4470  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L4471:
    /* 4471  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L4473: /* L4473 */
    /* 4473  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L4476:
    /* 4476  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4477:
    /* 4477  mov     cx,ax */
    CX = AX;
L4479:
    /* 4479  mov     ax,bx */
    AX = BX;
L447B:
    /* 447B  neg     ax */
    AX = (uint16_t)-AX;
L447D:
    /* 447D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L447E:
    /* 447E  mov     bp,ax */
    BP = AX;
L4480:
    /* 4480  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x4483)) != 0) return c;
L4483:
    /* 4483  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L4484:
    /* 4484  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L4488:
    /* 4488  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L448C:
    /* 448C  jmp     L43B2 */
    goto L43B2;
L448F: /* L448F */
    /* 448F  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L4493:
    /* 4493  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_4494  (+4494)
       Opcode 78h (FM has two names at this address, do_parapiped and done_clipping_here),
       operands: point, three half-extents (scaled by the coordinate shift), offset. A bounding-box
       test: rotates the three extents into vectors (15D9h..15E9h) and tests the eight corners of
       the box around the point. All eight inside the frustum: calls set_accept (a no-op in UW2).
       All outside one plane (codes AND nonzero): skips `offset` bytes, or ends the program when
       the offset is 0. Otherwise continues after the operand. */
L4494: /* _do_parapiped */
    /* 4494  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L4495:
    /* 4495  mov     di,ax */
    DI = AX;
L4497:
    /* 4497  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L449B:
    /* 449B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L449C:
    /* 449C  shl     ax,cl */
    AX = shl16(AX, CL);
L449E:
    /* 449E  mov     bx,ax */
    BX = AX;
L44A0:
    /* 44A0  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L44A4:
    /* 44A4  mov     word ptr ds:[15D9h],dx */
    ww(pDS, 0x15D9, DX);
L44A8:
    /* 44A8  mov     ax,bx */
    AX = BX;
L44AA:
    /* 44AA  imul    word ptr ds:[MAT_XY] */
    imul16(rw(pDS, 0x1604));
L44AE:
    /* 44AE  mov     word ptr ds:[15DBh],dx */
    ww(pDS, 0x15DB, DX);
L44B2:
    /* 44B2  mov     ax,bx */
    AX = BX;
L44B4:
    /* 44B4  imul    word ptr ds:[MAT_XZ] */
    imul16(rw(pDS, 0x1606));
L44B8:
    /* 44B8  mov     word ptr ds:[15DDh],dx */
    ww(pDS, 0x15DD, DX);
L44BC:
    /* 44BC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L44BD:
    /* 44BD  shl     ax,cl */
    AX = shl16(AX, CL);
L44BF:
    /* 44BF  mov     bx,ax */
    BX = AX;
L44C1:
    /* 44C1  imul    word ptr ds:[MAT_YX] */
    imul16(rw(pDS, 0x1608));
L44C5:
    /* 44C5  mov     word ptr ds:[15DFh],dx */
    ww(pDS, 0x15DF, DX);
L44C9:
    /* 44C9  mov     ax,bx */
    AX = BX;
L44CB:
    /* 44CB  imul    word ptr ds:[MAT_YY] */
    imul16(rw(pDS, 0x160A));
L44CF:
    /* 44CF  mov     word ptr ds:[15E1h],dx */
    ww(pDS, 0x15E1, DX);
L44D3:
    /* 44D3  mov     ax,bx */
    AX = BX;
L44D5:
    /* 44D5  imul    word ptr ds:[MAT_YZ] */
    imul16(rw(pDS, 0x160C));
L44D9:
    /* 44D9  mov     word ptr ds:[15E3h],dx */
    ww(pDS, 0x15E3, DX);
L44DD:
    /* 44DD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L44DE:
    /* 44DE  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L44E0:
    /* 44E0  mov     bx,ax */
    BX = AX;
L44E2:
    /* 44E2  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L44E6:
    /* 44E6  mov     word ptr ds:[15E5h],dx */
    ww(pDS, 0x15E5, DX);
L44EA:
    /* 44EA  mov     ax,bx */
    AX = BX;
L44EC:
    /* 44EC  imul    word ptr ds:[MAT_ZY] */
    imul16(rw(pDS, 0x1610));
L44F0:
    /* 44F0  mov     word ptr ds:[15E7h],dx */
    ww(pDS, 0x15E7, DX);
L44F4:
    /* 44F4  mov     ax,bx */
    AX = BX;
L44F6:
    /* 44F6  imul    word ptr ds:[MAT_ZZ] */
    imul16(rw(pDS, 0x1612));
L44FA:
    /* 44FA  mov     word ptr ds:[15E9h],dx */
    ww(pDS, 0x15E9, DX);
L44FE:
    /* 44FE  mov     al,byte ptr [di+PNT_CODES] */
    AL = rb(pDS, DI + 0x1626);
L4502:
    /* 4502  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L4504:
    /* 4504  je      short L4509 */
    if (ZF) goto L4509;
L4506:
    /* 4506  jmp     L479B */
    goto L479B;
L4509: /* L4509 */
    /* 4509  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L450D:
    /* 450D  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L4511:
    /* 4511  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L4515:
    /* 4515  add     bx,word ptr ds:[15D9h] */
    BX = (uint16_t)(BX + rw(pDS, 0x15D9));
L4519:
    /* 4519  add     cx,word ptr ds:[15DBh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15DB));
L451D:
    /* 451D  add     bp,word ptr ds:[15DDh] */
    BP = (uint16_t)(BP + rw(pDS, 0x15DD));
L4521:
    /* 4521  add     bx,word ptr ds:[15DFh] */
    BX = (uint16_t)(BX + rw(pDS, 0x15DF));
L4525:
    /* 4525  add     cx,word ptr ds:[15E1h] */
    CX = (uint16_t)(CX + rw(pDS, 0x15E1));
L4529:
    /* 4529  add     bp,word ptr ds:[15E3h] */
    BP = (uint16_t)(BP + rw(pDS, 0x15E3));
L452D:
    /* 452D  add     bx,word ptr ds:[15E5h] */
    BX = (uint16_t)(BX + rw(pDS, 0x15E5));
L4531:
    /* 4531  add     cx,word ptr ds:[15E7h] */
    CX = (uint16_t)(CX + rw(pDS, 0x15E7));
L4535:
    /* 4535  add     bp,word ptr ds:[15E9h] */
    BP = add16(BP, rw(pDS, 0x15E9), 0);
L4539:
    /* 4539  jns     short L453E */
    if (!SF) goto L453E;
L453B:
    /* 453B  jmp     L4792 */
    goto L4792;
L453E: /* L453E */
    /* 453E  cmp     bx,bp */
    sub16(BX, BP, 0);
L4540:
    /* 4540  jle     short L4545 */
    if (ZF || SF != OF) goto L4545;
L4542:
    /* 4542  jmp     L4792 */
    goto L4792;
L4545: /* L4545 */
    /* 4545  cmp     cx,bp */
    sub16(CX, BP, 0);
L4547:
    /* 4547  jle     short L454C */
    if (ZF || SF != OF) goto L454C;
L4549:
    /* 4549  jmp     L4792 */
    goto L4792;
L454C: /* L454C */
    /* 454C  neg     bp */
    BP = (uint16_t)-BP;
L454E:
    /* 454E  cmp     bx,bp */
    sub16(BX, BP, 0);
L4550:
    /* 4550  jge     short L4555 */
    if (SF == OF) goto L4555;
L4552:
    /* 4552  jmp     L4792 */
    goto L4792;
L4555: /* L4555 */
    /* 4555  cmp     cx,bp */
    sub16(CX, BP, 0);
L4557:
    /* 4557  jge     short L455C */
    if (SF == OF) goto L455C;
L4559:
    /* 4559  jmp     L4792 */
    goto L4792;
L455C: /* L455C */
    /* 455C  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L4560:
    /* 4560  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L4564:
    /* 4564  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L4568:
    /* 4568  sub     bx,word ptr ds:[15D9h] */
    BX = (uint16_t)(BX - rw(pDS, 0x15D9));
L456C:
    /* 456C  sub     cx,word ptr ds:[15DBh] */
    CX = (uint16_t)(CX - rw(pDS, 0x15DB));
L4570:
    /* 4570  sub     bp,word ptr ds:[15DDh] */
    BP = (uint16_t)(BP - rw(pDS, 0x15DD));
L4574:
    /* 4574  add     bx,word ptr ds:[15DFh] */
    BX = (uint16_t)(BX + rw(pDS, 0x15DF));
L4578:
    /* 4578  add     cx,word ptr ds:[15E1h] */
    CX = (uint16_t)(CX + rw(pDS, 0x15E1));
L457C:
    /* 457C  add     bp,word ptr ds:[15E3h] */
    BP = (uint16_t)(BP + rw(pDS, 0x15E3));
L4580:
    /* 4580  add     bx,word ptr ds:[15E5h] */
    BX = (uint16_t)(BX + rw(pDS, 0x15E5));
L4584:
    /* 4584  add     cx,word ptr ds:[15E7h] */
    CX = (uint16_t)(CX + rw(pDS, 0x15E7));
L4588:
    /* 4588  add     bp,word ptr ds:[15E9h] */
    BP = add16(BP, rw(pDS, 0x15E9), 0);
L458C:
    /* 458C  jns     short L4591 */
    if (!SF) goto L4591;
L458E:
    /* 458E  jmp     L4792 */
    goto L4792;
L4591: /* L4591 */
    /* 4591  cmp     bx,bp */
    sub16(BX, BP, 0);
L4593:
    /* 4593  jle     short L4598 */
    if (ZF || SF != OF) goto L4598;
L4595:
    /* 4595  jmp     L4792 */
    goto L4792;
L4598: /* L4598 */
    /* 4598  cmp     cx,bp */
    sub16(CX, BP, 0);
L459A:
    /* 459A  jle     short L459F */
    if (ZF || SF != OF) goto L459F;
L459C:
    /* 459C  jmp     L4792 */
    goto L4792;
L459F: /* L459F */
    /* 459F  neg     bp */
    BP = (uint16_t)-BP;
L45A1:
    /* 45A1  cmp     bx,bp */
    sub16(BX, BP, 0);
L45A3:
    /* 45A3  jge     short L45A8 */
    if (SF == OF) goto L45A8;
L45A5:
    /* 45A5  jmp     L4792 */
    goto L4792;
L45A8: /* L45A8 */
    /* 45A8  cmp     cx,bp */
    sub16(CX, BP, 0);
L45AA:
    /* 45AA  jge     short L45AF */
    if (SF == OF) goto L45AF;
L45AC:
    /* 45AC  jmp     L4792 */
    goto L4792;
L45AF: /* L45AF */
    /* 45AF  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L45B3:
    /* 45B3  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L45B7:
    /* 45B7  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L45BB:
    /* 45BB  add     bx,word ptr ds:[15D9h] */
    BX = (uint16_t)(BX + rw(pDS, 0x15D9));
L45BF:
    /* 45BF  add     cx,word ptr ds:[15DBh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15DB));
L45C3:
    /* 45C3  add     bp,word ptr ds:[15DDh] */
    BP = (uint16_t)(BP + rw(pDS, 0x15DD));
L45C7:
    /* 45C7  sub     bx,word ptr ds:[15DFh] */
    BX = (uint16_t)(BX - rw(pDS, 0x15DF));
L45CB:
    /* 45CB  sub     cx,word ptr ds:[15E1h] */
    CX = (uint16_t)(CX - rw(pDS, 0x15E1));
L45CF:
    /* 45CF  sub     bp,word ptr ds:[15E3h] */
    BP = (uint16_t)(BP - rw(pDS, 0x15E3));
L45D3:
    /* 45D3  add     bx,word ptr ds:[15E5h] */
    BX = (uint16_t)(BX + rw(pDS, 0x15E5));
L45D7:
    /* 45D7  add     cx,word ptr ds:[15E7h] */
    CX = (uint16_t)(CX + rw(pDS, 0x15E7));
L45DB:
    /* 45DB  add     bp,word ptr ds:[15E9h] */
    BP = add16(BP, rw(pDS, 0x15E9), 0);
L45DF:
    /* 45DF  jns     short L45E4 */
    if (!SF) goto L45E4;
L45E1:
    /* 45E1  jmp     L4792 */
    goto L4792;
L45E4: /* L45E4 */
    /* 45E4  cmp     bx,bp */
    sub16(BX, BP, 0);
L45E6:
    /* 45E6  jle     short L45EB */
    if (ZF || SF != OF) goto L45EB;
L45E8:
    /* 45E8  jmp     L4792 */
    goto L4792;
L45EB: /* L45EB */
    /* 45EB  cmp     cx,bp */
    sub16(CX, BP, 0);
L45ED:
    /* 45ED  jle     short L45F2 */
    if (ZF || SF != OF) goto L45F2;
L45EF:
    /* 45EF  jmp     L4792 */
    goto L4792;
L45F2: /* L45F2 */
    /* 45F2  neg     bp */
    BP = (uint16_t)-BP;
L45F4:
    /* 45F4  cmp     bx,bp */
    sub16(BX, BP, 0);
L45F6:
    /* 45F6  jge     short L45FB */
    if (SF == OF) goto L45FB;
L45F8:
    /* 45F8  jmp     L4792 */
    goto L4792;
L45FB: /* L45FB */
    /* 45FB  cmp     cx,bp */
    sub16(CX, BP, 0);
L45FD:
    /* 45FD  jge     short L4602 */
    if (SF == OF) goto L4602;
L45FF:
    /* 45FF  jmp     L4792 */
    goto L4792;
L4602: /* L4602 */
    /* 4602  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L4606:
    /* 4606  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L460A:
    /* 460A  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L460E:
    /* 460E  sub     bx,word ptr ds:[15D9h] */
    BX = (uint16_t)(BX - rw(pDS, 0x15D9));
L4612:
    /* 4612  sub     cx,word ptr ds:[15DBh] */
    CX = (uint16_t)(CX - rw(pDS, 0x15DB));
L4616:
    /* 4616  sub     bp,word ptr ds:[15DDh] */
    BP = (uint16_t)(BP - rw(pDS, 0x15DD));
L461A:
    /* 461A  sub     bx,word ptr ds:[15DFh] */
    BX = (uint16_t)(BX - rw(pDS, 0x15DF));
L461E:
    /* 461E  sub     cx,word ptr ds:[15E1h] */
    CX = (uint16_t)(CX - rw(pDS, 0x15E1));
L4622:
    /* 4622  sub     bp,word ptr ds:[15E3h] */
    BP = (uint16_t)(BP - rw(pDS, 0x15E3));
L4626:
    /* 4626  add     bx,word ptr ds:[15E5h] */
    BX = (uint16_t)(BX + rw(pDS, 0x15E5));
L462A:
    /* 462A  add     cx,word ptr ds:[15E7h] */
    CX = (uint16_t)(CX + rw(pDS, 0x15E7));
L462E:
    /* 462E  add     bp,word ptr ds:[15E9h] */
    BP = add16(BP, rw(pDS, 0x15E9), 0);
L4632:
    /* 4632  jns     short L4637 */
    if (!SF) goto L4637;
L4634:
    /* 4634  jmp     L4792 */
    goto L4792;
L4637: /* L4637 */
    /* 4637  cmp     bx,bp */
    sub16(BX, BP, 0);
L4639:
    /* 4639  jle     short L463E */
    if (ZF || SF != OF) goto L463E;
L463B:
    /* 463B  jmp     L4792 */
    goto L4792;
L463E: /* L463E */
    /* 463E  cmp     cx,bp */
    sub16(CX, BP, 0);
L4640:
    /* 4640  jle     short L4645 */
    if (ZF || SF != OF) goto L4645;
L4642:
    /* 4642  jmp     L4792 */
    goto L4792;
L4645: /* L4645 */
    /* 4645  neg     bp */
    BP = (uint16_t)-BP;
L4647:
    /* 4647  cmp     bx,bp */
    sub16(BX, BP, 0);
L4649:
    /* 4649  jge     short L464E */
    if (SF == OF) goto L464E;
L464B:
    /* 464B  jmp     L4792 */
    goto L4792;
L464E: /* L464E */
    /* 464E  cmp     cx,bp */
    sub16(CX, BP, 0);
L4650:
    /* 4650  jge     short L4655 */
    if (SF == OF) goto L4655;
L4652:
    /* 4652  jmp     L4792 */
    goto L4792;
L4655: /* L4655 */
    /* 4655  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L4659:
    /* 4659  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L465D:
    /* 465D  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L4661:
    /* 4661  add     bx,word ptr ds:[15D9h] */
    BX = (uint16_t)(BX + rw(pDS, 0x15D9));
L4665:
    /* 4665  add     cx,word ptr ds:[15DBh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15DB));
L4669:
    /* 4669  add     bp,word ptr ds:[15DDh] */
    BP = (uint16_t)(BP + rw(pDS, 0x15DD));
L466D:
    /* 466D  add     bx,word ptr ds:[15DFh] */
    BX = (uint16_t)(BX + rw(pDS, 0x15DF));
L4671:
    /* 4671  add     cx,word ptr ds:[15E1h] */
    CX = (uint16_t)(CX + rw(pDS, 0x15E1));
L4675:
    /* 4675  add     bp,word ptr ds:[15E3h] */
    BP = (uint16_t)(BP + rw(pDS, 0x15E3));
L4679:
    /* 4679  sub     bx,word ptr ds:[15E5h] */
    BX = (uint16_t)(BX - rw(pDS, 0x15E5));
L467D:
    /* 467D  sub     cx,word ptr ds:[15E7h] */
    CX = (uint16_t)(CX - rw(pDS, 0x15E7));
L4681:
    /* 4681  sub     bp,word ptr ds:[15E9h] */
    BP = sub16(BP, rw(pDS, 0x15E9), 0);
L4685:
    /* 4685  jns     short L468A */
    if (!SF) goto L468A;
L4687:
    /* 4687  jmp     L4792 */
    goto L4792;
L468A: /* L468A */
    /* 468A  cmp     bx,bp */
    sub16(BX, BP, 0);
L468C:
    /* 468C  jle     short L4691 */
    if (ZF || SF != OF) goto L4691;
L468E:
    /* 468E  jmp     L4792 */
    goto L4792;
L4691: /* L4691 */
    /* 4691  cmp     cx,bp */
    sub16(CX, BP, 0);
L4693:
    /* 4693  jle     short L4698 */
    if (ZF || SF != OF) goto L4698;
L4695:
    /* 4695  jmp     L4792 */
    goto L4792;
L4698: /* L4698 */
    /* 4698  neg     bp */
    BP = (uint16_t)-BP;
L469A:
    /* 469A  cmp     bx,bp */
    sub16(BX, BP, 0);
L469C:
    /* 469C  jge     short L46A1 */
    if (SF == OF) goto L46A1;
L469E:
    /* 469E  jmp     L4792 */
    goto L4792;
L46A1: /* L46A1 */
    /* 46A1  cmp     cx,bp */
    sub16(CX, BP, 0);
L46A3:
    /* 46A3  jge     short L46A8 */
    if (SF == OF) goto L46A8;
L46A5:
    /* 46A5  jmp     L4792 */
    goto L4792;
L46A8: /* L46A8 */
    /* 46A8  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L46AC:
    /* 46AC  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L46B0:
    /* 46B0  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L46B4:
    /* 46B4  sub     bx,word ptr ds:[15D9h] */
    BX = (uint16_t)(BX - rw(pDS, 0x15D9));
L46B8:
    /* 46B8  sub     cx,word ptr ds:[15DBh] */
    CX = (uint16_t)(CX - rw(pDS, 0x15DB));
L46BC:
    /* 46BC  sub     bp,word ptr ds:[15DDh] */
    BP = (uint16_t)(BP - rw(pDS, 0x15DD));
L46C0:
    /* 46C0  add     bx,word ptr ds:[15DFh] */
    BX = (uint16_t)(BX + rw(pDS, 0x15DF));
L46C4:
    /* 46C4  add     cx,word ptr ds:[15E1h] */
    CX = (uint16_t)(CX + rw(pDS, 0x15E1));
L46C8:
    /* 46C8  add     bp,word ptr ds:[15E3h] */
    BP = (uint16_t)(BP + rw(pDS, 0x15E3));
L46CC:
    /* 46CC  sub     bx,word ptr ds:[15E5h] */
    BX = (uint16_t)(BX - rw(pDS, 0x15E5));
L46D0:
    /* 46D0  sub     cx,word ptr ds:[15E7h] */
    CX = (uint16_t)(CX - rw(pDS, 0x15E7));
L46D4:
    /* 46D4  sub     bp,word ptr ds:[15E9h] */
    BP = sub16(BP, rw(pDS, 0x15E9), 0);
L46D8:
    /* 46D8  jns     short L46DD */
    if (!SF) goto L46DD;
L46DA:
    /* 46DA  jmp     L4792 */
    goto L4792;
L46DD: /* L46DD */
    /* 46DD  cmp     bx,bp */
    sub16(BX, BP, 0);
L46DF:
    /* 46DF  jle     short L46E4 */
    if (ZF || SF != OF) goto L46E4;
L46E1:
    /* 46E1  jmp     L4792 */
    goto L4792;
L46E4: /* L46E4 */
    /* 46E4  cmp     cx,bp */
    sub16(CX, BP, 0);
L46E6:
    /* 46E6  jle     short L46EB */
    if (ZF || SF != OF) goto L46EB;
L46E8:
    /* 46E8  jmp     L4792 */
    goto L4792;
L46EB: /* L46EB */
    /* 46EB  neg     bp */
    BP = (uint16_t)-BP;
L46ED:
    /* 46ED  cmp     bx,bp */
    sub16(BX, BP, 0);
L46EF:
    /* 46EF  jge     short L46F4 */
    if (SF == OF) goto L46F4;
L46F1:
    /* 46F1  jmp     L4792 */
    goto L4792;
L46F4: /* L46F4 */
    /* 46F4  cmp     cx,bp */
    sub16(CX, BP, 0);
L46F6:
    /* 46F6  jge     short L46FB */
    if (SF == OF) goto L46FB;
L46F8:
    /* 46F8  jmp     L4792 */
    goto L4792;
L46FB: /* L46FB */
    /* 46FB  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L46FF:
    /* 46FF  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L4703:
    /* 4703  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L4707:
    /* 4707  add     bx,word ptr ds:[15D9h] */
    BX = (uint16_t)(BX + rw(pDS, 0x15D9));
L470B:
    /* 470B  add     cx,word ptr ds:[15DBh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15DB));
L470F:
    /* 470F  add     bp,word ptr ds:[15DDh] */
    BP = (uint16_t)(BP + rw(pDS, 0x15DD));
L4713:
    /* 4713  sub     bx,word ptr ds:[15DFh] */
    BX = (uint16_t)(BX - rw(pDS, 0x15DF));
L4717:
    /* 4717  sub     cx,word ptr ds:[15E1h] */
    CX = (uint16_t)(CX - rw(pDS, 0x15E1));
L471B:
    /* 471B  sub     bp,word ptr ds:[15E3h] */
    BP = (uint16_t)(BP - rw(pDS, 0x15E3));
L471F:
    /* 471F  sub     bx,word ptr ds:[15E5h] */
    BX = (uint16_t)(BX - rw(pDS, 0x15E5));
L4723:
    /* 4723  sub     cx,word ptr ds:[15E7h] */
    CX = (uint16_t)(CX - rw(pDS, 0x15E7));
L4727:
    /* 4727  sub     bp,word ptr ds:[15E9h] */
    BP = sub16(BP, rw(pDS, 0x15E9), 0);
L472B:
    /* 472B  js      short L4792 */
    if (SF) goto L4792;
L472D:
    /* 472D  cmp     bx,bp */
    sub16(BX, BP, 0);
L472F:
    /* 472F  jg      short L4792 */
    if (!ZF && SF == OF) goto L4792;
L4731:
    /* 4731  cmp     cx,bp */
    sub16(CX, BP, 0);
L4733:
    /* 4733  jg      short L4792 */
    if (!ZF && SF == OF) goto L4792;
L4735:
    /* 4735  neg     bp */
    BP = (uint16_t)-BP;
L4737:
    /* 4737  cmp     bx,bp */
    sub16(BX, BP, 0);
L4739:
    /* 4739  jl      short L4792 */
    if (SF != OF) goto L4792;
L473B:
    /* 473B  cmp     cx,bp */
    sub16(CX, BP, 0);
L473D:
    /* 473D  jl      short L4792 */
    if (SF != OF) goto L4792;
L473F:
    /* 473F  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L4743:
    /* 4743  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L4747:
    /* 4747  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L474B:
    /* 474B  sub     bx,word ptr ds:[15D9h] */
    BX = (uint16_t)(BX - rw(pDS, 0x15D9));
L474F:
    /* 474F  sub     cx,word ptr ds:[15DBh] */
    CX = (uint16_t)(CX - rw(pDS, 0x15DB));
L4753:
    /* 4753  sub     bp,word ptr ds:[15DDh] */
    BP = (uint16_t)(BP - rw(pDS, 0x15DD));
L4757:
    /* 4757  sub     bx,word ptr ds:[15DFh] */
    BX = (uint16_t)(BX - rw(pDS, 0x15DF));
L475B:
    /* 475B  sub     cx,word ptr ds:[15E1h] */
    CX = (uint16_t)(CX - rw(pDS, 0x15E1));
L475F:
    /* 475F  sub     bp,word ptr ds:[15E3h] */
    BP = (uint16_t)(BP - rw(pDS, 0x15E3));
L4763:
    /* 4763  sub     bx,word ptr ds:[15E5h] */
    BX = (uint16_t)(BX - rw(pDS, 0x15E5));
L4767:
    /* 4767  sub     cx,word ptr ds:[15E7h] */
    CX = (uint16_t)(CX - rw(pDS, 0x15E7));
L476B:
    /* 476B  sub     bp,word ptr ds:[15E9h] */
    BP = sub16(BP, rw(pDS, 0x15E9), 0);
L476F:
    /* 476F  js      short L4792 */
    if (SF) goto L4792;
L4771:
    /* 4771  cmp     bx,bp */
    sub16(BX, BP, 0);
L4773:
    /* 4773  jg      short L4792 */
    if (!ZF && SF == OF) goto L4792;
L4775:
    /* 4775  cmp     cx,bp */
    sub16(CX, BP, 0);
L4777:
    /* 4777  jg      short L4792 */
    if (!ZF && SF == OF) goto L4792;
L4779:
    /* 4779  neg     bp */
    BP = (uint16_t)-BP;
L477B:
    /* 477B  cmp     bx,bp */
    sub16(BX, BP, 0);
L477D:
    /* 477D  jl      short L4792 */
    if (SF != OF) goto L4792;
L477F:
    /* 477F  cmp     cx,bp */
    sub16(CX, BP, 0);
L4781:
    /* 4781  jl      short L4792 */
    if (SF != OF) goto L4792;
L4783:
    /* 4783  mov     ax,0FFFFh */
    AX = 0xFFFF;
L4786:
    /* 4786  call    _seg004_5040 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5040), 0x4789)) != 0) return c;
L4789:
    /* 4789  add     si,2 */
    SI = add16(SI, 0x2, 0);
L478C:
    /* 478C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L478D:
    /* 478D  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L478E:
    /* 478E  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L4792: /* L4792 */
    /* 4792  add     si,2 */
    SI = add16(SI, 0x2, 0);
L4795:
    /* 4795  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L4796:
    /* 4796  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L4797:
    /* 4797  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L479B: /* L479B */
    /* 479B  mov     dl,al */
    DL = AL;
L479D:
    /* 479D  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L47A1:
    /* 47A1  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L47A5:
    /* 47A5  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L47A9:
    /* 47A9  add     bx,word ptr ds:[15D9h] */
    BX = (uint16_t)(BX + rw(pDS, 0x15D9));
L47AD:
    /* 47AD  add     cx,word ptr ds:[15DBh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15DB));
L47B1:
    /* 47B1  add     bp,word ptr ds:[15DDh] */
    BP = (uint16_t)(BP + rw(pDS, 0x15DD));
L47B5:
    /* 47B5  add     bx,word ptr ds:[15DFh] */
    BX = (uint16_t)(BX + rw(pDS, 0x15DF));
L47B9:
    /* 47B9  add     cx,word ptr ds:[15E1h] */
    CX = (uint16_t)(CX + rw(pDS, 0x15E1));
L47BD:
    /* 47BD  add     bp,word ptr ds:[15E3h] */
    BP = (uint16_t)(BP + rw(pDS, 0x15E3));
L47C1:
    /* 47C1  add     bx,word ptr ds:[15E5h] */
    BX = (uint16_t)(BX + rw(pDS, 0x15E5));
L47C5:
    /* 47C5  add     cx,word ptr ds:[15E7h] */
    CX = (uint16_t)(CX + rw(pDS, 0x15E7));
L47C9:
    /* 47C9  add     bp,word ptr ds:[15E9h] */
    BP = (uint16_t)(BP + rw(pDS, 0x15E9));
L47CD:
    /* 47CD  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x47D0)) != 0) return c;
L47D0:
    /* 47D0  and     dl,al */
    DL = logic8((uint8_t)(DL & AL));
L47D2:
    /* 47D2  je      L4792 */
    if (ZF) goto L4792;
L47D4:
    /* 47D4  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L47D8:
    /* 47D8  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L47DC:
    /* 47DC  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L47E0:
    /* 47E0  sub     bx,word ptr ds:[15D9h] */
    BX = (uint16_t)(BX - rw(pDS, 0x15D9));
L47E4:
    /* 47E4  sub     cx,word ptr ds:[15DBh] */
    CX = (uint16_t)(CX - rw(pDS, 0x15DB));
L47E8:
    /* 47E8  sub     bp,word ptr ds:[15DDh] */
    BP = (uint16_t)(BP - rw(pDS, 0x15DD));
L47EC:
    /* 47EC  add     bx,word ptr ds:[15DFh] */
    BX = (uint16_t)(BX + rw(pDS, 0x15DF));
L47F0:
    /* 47F0  add     cx,word ptr ds:[15E1h] */
    CX = (uint16_t)(CX + rw(pDS, 0x15E1));
L47F4:
    /* 47F4  add     bp,word ptr ds:[15E3h] */
    BP = (uint16_t)(BP + rw(pDS, 0x15E3));
L47F8:
    /* 47F8  add     bx,word ptr ds:[15E5h] */
    BX = (uint16_t)(BX + rw(pDS, 0x15E5));
L47FC:
    /* 47FC  add     cx,word ptr ds:[15E7h] */
    CX = (uint16_t)(CX + rw(pDS, 0x15E7));
L4800:
    /* 4800  add     bp,word ptr ds:[15E9h] */
    BP = (uint16_t)(BP + rw(pDS, 0x15E9));
L4804:
    /* 4804  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x4807)) != 0) return c;
L4807:
    /* 4807  and     dl,al */
    DL = logic8((uint8_t)(DL & AL));
L4809:
    /* 4809  je      L4792 */
    if (ZF) goto L4792;
L480B:
    /* 480B  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L480F:
    /* 480F  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L4813:
    /* 4813  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L4817:
    /* 4817  add     bx,word ptr ds:[15D9h] */
    BX = (uint16_t)(BX + rw(pDS, 0x15D9));
L481B:
    /* 481B  add     cx,word ptr ds:[15DBh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15DB));
L481F:
    /* 481F  add     bp,word ptr ds:[15DDh] */
    BP = (uint16_t)(BP + rw(pDS, 0x15DD));
L4823:
    /* 4823  sub     bx,word ptr ds:[15DFh] */
    BX = (uint16_t)(BX - rw(pDS, 0x15DF));
L4827:
    /* 4827  sub     cx,word ptr ds:[15E1h] */
    CX = (uint16_t)(CX - rw(pDS, 0x15E1));
L482B:
    /* 482B  sub     bp,word ptr ds:[15E3h] */
    BP = (uint16_t)(BP - rw(pDS, 0x15E3));
L482F:
    /* 482F  add     bx,word ptr ds:[15E5h] */
    BX = (uint16_t)(BX + rw(pDS, 0x15E5));
L4833:
    /* 4833  add     cx,word ptr ds:[15E7h] */
    CX = (uint16_t)(CX + rw(pDS, 0x15E7));
L4837:
    /* 4837  add     bp,word ptr ds:[15E9h] */
    BP = (uint16_t)(BP + rw(pDS, 0x15E9));
L483B:
    /* 483B  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x483E)) != 0) return c;
L483E:
    /* 483E  and     dl,al */
    DL = logic8((uint8_t)(DL & AL));
L4840:
    /* 4840  jne     short L4845 */
    if (!ZF) goto L4845;
L4842:
    /* 4842  jmp     L4792 */
    goto L4792;
L4845: /* L4845 */
    /* 4845  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L4849:
    /* 4849  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L484D:
    /* 484D  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L4851:
    /* 4851  sub     bx,word ptr ds:[15D9h] */
    BX = (uint16_t)(BX - rw(pDS, 0x15D9));
L4855:
    /* 4855  sub     cx,word ptr ds:[15DBh] */
    CX = (uint16_t)(CX - rw(pDS, 0x15DB));
L4859:
    /* 4859  sub     bp,word ptr ds:[15DDh] */
    BP = (uint16_t)(BP - rw(pDS, 0x15DD));
L485D:
    /* 485D  sub     bx,word ptr ds:[15DFh] */
    BX = (uint16_t)(BX - rw(pDS, 0x15DF));
L4861:
    /* 4861  sub     cx,word ptr ds:[15E1h] */
    CX = (uint16_t)(CX - rw(pDS, 0x15E1));
L4865:
    /* 4865  sub     bp,word ptr ds:[15E3h] */
    BP = (uint16_t)(BP - rw(pDS, 0x15E3));
L4869:
    /* 4869  add     bx,word ptr ds:[15E5h] */
    BX = (uint16_t)(BX + rw(pDS, 0x15E5));
L486D:
    /* 486D  add     cx,word ptr ds:[15E7h] */
    CX = (uint16_t)(CX + rw(pDS, 0x15E7));
L4871:
    /* 4871  add     bp,word ptr ds:[15E9h] */
    BP = (uint16_t)(BP + rw(pDS, 0x15E9));
L4875:
    /* 4875  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x4878)) != 0) return c;
L4878:
    /* 4878  and     dl,al */
    DL = logic8((uint8_t)(DL & AL));
L487A:
    /* 487A  jne     short L487F */
    if (!ZF) goto L487F;
L487C:
    /* 487C  jmp     L4792 */
    goto L4792;
L487F: /* L487F */
    /* 487F  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L4883:
    /* 4883  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L4887:
    /* 4887  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L488B:
    /* 488B  add     bx,word ptr ds:[15D9h] */
    BX = (uint16_t)(BX + rw(pDS, 0x15D9));
L488F:
    /* 488F  add     cx,word ptr ds:[15DBh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15DB));
L4893:
    /* 4893  add     bp,word ptr ds:[15DDh] */
    BP = (uint16_t)(BP + rw(pDS, 0x15DD));
L4897:
    /* 4897  add     bx,word ptr ds:[15DFh] */
    BX = (uint16_t)(BX + rw(pDS, 0x15DF));
L489B:
    /* 489B  add     cx,word ptr ds:[15E1h] */
    CX = (uint16_t)(CX + rw(pDS, 0x15E1));
L489F:
    /* 489F  add     bp,word ptr ds:[15E3h] */
    BP = (uint16_t)(BP + rw(pDS, 0x15E3));
L48A3:
    /* 48A3  sub     bx,word ptr ds:[15E5h] */
    BX = (uint16_t)(BX - rw(pDS, 0x15E5));
L48A7:
    /* 48A7  sub     cx,word ptr ds:[15E7h] */
    CX = (uint16_t)(CX - rw(pDS, 0x15E7));
L48AB:
    /* 48AB  sub     bp,word ptr ds:[15E9h] */
    BP = (uint16_t)(BP - rw(pDS, 0x15E9));
L48AF:
    /* 48AF  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x48B2)) != 0) return c;
L48B2:
    /* 48B2  and     dl,al */
    DL = logic8((uint8_t)(DL & AL));
L48B4:
    /* 48B4  jne     short L48B9 */
    if (!ZF) goto L48B9;
L48B6:
    /* 48B6  jmp     L4792 */
    goto L4792;
L48B9: /* L48B9 */
    /* 48B9  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L48BD:
    /* 48BD  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L48C1:
    /* 48C1  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L48C5:
    /* 48C5  sub     bx,word ptr ds:[15D9h] */
    BX = (uint16_t)(BX - rw(pDS, 0x15D9));
L48C9:
    /* 48C9  sub     cx,word ptr ds:[15DBh] */
    CX = (uint16_t)(CX - rw(pDS, 0x15DB));
L48CD:
    /* 48CD  sub     bp,word ptr ds:[15DDh] */
    BP = (uint16_t)(BP - rw(pDS, 0x15DD));
L48D1:
    /* 48D1  add     bx,word ptr ds:[15DFh] */
    BX = (uint16_t)(BX + rw(pDS, 0x15DF));
L48D5:
    /* 48D5  add     cx,word ptr ds:[15E1h] */
    CX = (uint16_t)(CX + rw(pDS, 0x15E1));
L48D9:
    /* 48D9  add     bp,word ptr ds:[15E3h] */
    BP = (uint16_t)(BP + rw(pDS, 0x15E3));
L48DD:
    /* 48DD  sub     bx,word ptr ds:[15E5h] */
    BX = (uint16_t)(BX - rw(pDS, 0x15E5));
L48E1:
    /* 48E1  sub     cx,word ptr ds:[15E7h] */
    CX = (uint16_t)(CX - rw(pDS, 0x15E7));
L48E5:
    /* 48E5  sub     bp,word ptr ds:[15E9h] */
    BP = (uint16_t)(BP - rw(pDS, 0x15E9));
L48E9:
    /* 48E9  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x48EC)) != 0) return c;
L48EC:
    /* 48EC  and     dl,al */
    DL = logic8((uint8_t)(DL & AL));
L48EE:
    /* 48EE  jne     short L48F3 */
    if (!ZF) goto L48F3;
L48F0:
    /* 48F0  jmp     L4792 */
    goto L4792;
L48F3: /* L48F3 */
    /* 48F3  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L48F7:
    /* 48F7  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L48FB:
    /* 48FB  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L48FF:
    /* 48FF  add     bx,word ptr ds:[15D9h] */
    BX = (uint16_t)(BX + rw(pDS, 0x15D9));
L4903:
    /* 4903  add     cx,word ptr ds:[15DBh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15DB));
L4907:
    /* 4907  add     bp,word ptr ds:[15DDh] */
    BP = (uint16_t)(BP + rw(pDS, 0x15DD));
L490B:
    /* 490B  sub     bx,word ptr ds:[15DFh] */
    BX = (uint16_t)(BX - rw(pDS, 0x15DF));
L490F:
    /* 490F  sub     cx,word ptr ds:[15E1h] */
    CX = (uint16_t)(CX - rw(pDS, 0x15E1));
L4913:
    /* 4913  sub     bp,word ptr ds:[15E3h] */
    BP = (uint16_t)(BP - rw(pDS, 0x15E3));
L4917:
    /* 4917  sub     bx,word ptr ds:[15E5h] */
    BX = (uint16_t)(BX - rw(pDS, 0x15E5));
L491B:
    /* 491B  sub     cx,word ptr ds:[15E7h] */
    CX = (uint16_t)(CX - rw(pDS, 0x15E7));
L491F:
    /* 491F  sub     bp,word ptr ds:[15E9h] */
    BP = (uint16_t)(BP - rw(pDS, 0x15E9));
L4923:
    /* 4923  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x4926)) != 0) return c;
L4926:
    /* 4926  and     dl,al */
    DL = logic8((uint8_t)(DL & AL));
L4928:
    /* 4928  jne     short L492D */
    if (!ZF) goto L492D;
L492A:
    /* 492A  jmp     L4792 */
    goto L4792;
L492D: /* L492D */
    /* 492D  mov     bx,word ptr [di+PNT_X] */
    BX = rw(pDS, DI + 0x1620);
L4931:
    /* 4931  mov     cx,word ptr [di+PNT_Y] */
    CX = rw(pDS, DI + 0x1622);
L4935:
    /* 4935  mov     bp,word ptr [di+PNT_Z] */
    BP = rw(pDS, DI + 0x1624);
L4939:
    /* 4939  sub     bx,word ptr ds:[15D9h] */
    BX = (uint16_t)(BX - rw(pDS, 0x15D9));
L493D:
    /* 493D  sub     cx,word ptr ds:[15DBh] */
    CX = (uint16_t)(CX - rw(pDS, 0x15DB));
L4941:
    /* 4941  sub     bp,word ptr ds:[15DDh] */
    BP = (uint16_t)(BP - rw(pDS, 0x15DD));
L4945:
    /* 4945  sub     bx,word ptr ds:[15DFh] */
    BX = (uint16_t)(BX - rw(pDS, 0x15DF));
L4949:
    /* 4949  sub     cx,word ptr ds:[15E1h] */
    CX = (uint16_t)(CX - rw(pDS, 0x15E1));
L494D:
    /* 494D  sub     bp,word ptr ds:[15E3h] */
    BP = (uint16_t)(BP - rw(pDS, 0x15E3));
L4951:
    /* 4951  sub     bx,word ptr ds:[15E5h] */
    BX = (uint16_t)(BX - rw(pDS, 0x15E5));
L4955:
    /* 4955  sub     cx,word ptr ds:[15E7h] */
    CX = (uint16_t)(CX - rw(pDS, 0x15E7));
L4959:
    /* 4959  sub     bp,word ptr ds:[15E9h] */
    BP = (uint16_t)(BP - rw(pDS, 0x15E9));
L495D:
    /* 495D  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x4960)) != 0) return c;
L4960:
    /* 4960  and     dl,al */
    DL = logic8((uint8_t)(DL & AL));
L4962:
    /* 4962  jne     short L4967 */
    if (!ZF) goto L4967;
L4964:
    /* 4964  jmp     L4792 */
    goto L4792;
L4967: /* L4967 */
    /* 4967  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L4968:
    /* 4968  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L496A:
    /* 496A  je      short L4974 */
    if (ZF) goto L4974;
L496C:
    /* 496C  add     si,ax */
    SI = add16(SI, AX, 0);
L496E:
    /* 496E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L496F:
    /* 496F  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L4970:
    /* 4970  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L4974: /* L4974 */
    /* 4974  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
