/* vidmode.c: replaces src/gfx/VIDMODE.ASM (seg003_2BBA, 2BBA..3A86 of its
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

uint32_t asm_mod_VIDMODE(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x2BBA: goto L2BBA;
    case 0x2BBF: goto L2BBF;
    case 0x2BC1: goto L2BC1;
    case 0x2BC7: goto L2BC7;
    case 0x2BCA: goto L2BCA;
    case 0x2BD0: goto L2BD0;
    case 0x2BD3: goto L2BD3;
    case 0x2BD8: goto L2BD8;
    case 0x2BDA: goto L2BDA;
    case 0x2BDD: goto L2BDD;
    case 0x2BDF: goto L2BDF;
    case 0x2BE5: goto L2BE5;
    case 0x2BE8: goto L2BE8;
    case 0x2BEE: goto L2BEE;
    case 0x2BF1: goto L2BF1;
    case 0x2BF7: goto L2BF7;
    case 0x2BFA: goto L2BFA;
    case 0x2BFF: goto L2BFF;
    case 0x2C01: goto L2C01;
    case 0x2C07: goto L2C07;
    case 0x2C09: goto L2C09;
    case 0x2C0F: goto L2C0F;
    case 0x2C11: goto L2C11;
    case 0x2C16: goto L2C16;
    case 0x2C18: goto L2C18;
    case 0x2C1E: goto L2C1E;
    case 0x2C20: goto L2C20;
    case 0x2C26: goto L2C26;
    case 0x2C28: goto L2C28;
    case 0x2C2E: goto L2C2E;
    case 0x2C33: goto L2C33;
    case 0x2C36: goto L2C36;
    case 0x2C38: goto L2C38;
    case 0x2C3A: goto L2C3A;
    case 0x2C40: goto L2C40;
    case 0x2C42: goto L2C42;
    case 0x2C45: goto L2C45;
    case 0x2C47: goto L2C47;
    case 0x2C4A: goto L2C4A;
    case 0x2C4C: goto L2C4C;
    case 0x2C4F: goto L2C4F;
    case 0x2C51: goto L2C51;
    case 0x2C53: goto L2C53;
    case 0x2C59: goto L2C59;
    case 0x2C5B: goto L2C5B;
    case 0x2C61: goto L2C61;
    case 0x2C65: goto L2C65;
    case 0x2C67: goto L2C67;
    case 0x2C69: goto L2C69;
    case 0x2C6A: goto L2C6A;
    case 0x2C6C: goto L2C6C;
    case 0x2C70: goto L2C70;
    case 0x2C73: goto L2C73;
    case 0x2C75: goto L2C75;
    case 0x2C7A: goto L2C7A;
    case 0x2C7C: goto L2C7C;
    case 0x2C80: goto L2C80;
    case 0x2C81: goto L2C81;
    case 0x2C85: goto L2C85;
    case 0x2C89: goto L2C89;
    case 0x2C8D: goto L2C8D;
    case 0x2C91: goto L2C91;
    case 0x2C92: goto L2C92;
    case 0x2C94: goto L2C94;
    case 0x2C96: goto L2C96;
    case 0x2C9B: goto L2C9B;
    case 0x2C9D: goto L2C9D;
    case 0x2C9F: goto L2C9F;
    case 0x2CA1: goto L2CA1;
    case 0x2CA3: goto L2CA3;
    case 0x2CA4: goto L2CA4;
    case 0x2CA8: goto L2CA8;
    case 0x2CAA: goto L2CAA;
    case 0x2CAD: goto L2CAD;
    case 0x2CB1: goto L2CB1;
    case 0x2CB3: goto L2CB3;
    case 0x2CB5: goto L2CB5;
    case 0x2CB7: goto L2CB7;
    case 0x2CB9: goto L2CB9;
    case 0x2CBB: goto L2CBB;
    case 0x2CBF: goto L2CBF;
    case 0x2CC1: goto L2CC1;
    case 0x2CC3: goto L2CC3;
    case 0x2CC6: goto L2CC6;
    case 0x2CC7: goto L2CC7;
    case 0x2CC9: goto L2CC9;
    case 0x2CCB: goto L2CCB;
    case 0x2CCD: goto L2CCD;
    case 0x2CCF: goto L2CCF;
    case 0x2CD1: goto L2CD1;
    case 0x2CD2: goto L2CD2;
    case 0x2CD6: goto L2CD6;
    case 0x2CD8: goto L2CD8;
    case 0x2CDC: goto L2CDC;
    case 0x2CDE: goto L2CDE;
    case 0x2CE0: goto L2CE0;
    case 0x2CE2: goto L2CE2;
    case 0x2CE4: goto L2CE4;
    case 0x2CE6: goto L2CE6;
    case 0x2CE8: goto L2CE8;
    case 0x2CEC: goto L2CEC;
    case 0x2CEE: goto L2CEE;
    case 0x2CF0: goto L2CF0;
    case 0x2CF2: goto L2CF2;
    case 0x2CF4: goto L2CF4;
    case 0x2CF6: goto L2CF6;
    case 0x2CF7: goto L2CF7;
    case 0x2CF9: goto L2CF9;
    case 0x2CFB: goto L2CFB;
    case 0x2CFF: goto L2CFF;
    case 0x2D02: goto L2D02;
    case 0x2D06: goto L2D06;
    case 0x2D08: goto L2D08;
    case 0x2D09: goto L2D09;
    case 0x2D0A: goto L2D0A;
    case 0x2D0B: goto L2D0B;
    case 0x2D0D: goto L2D0D;
    case 0x2D0F: goto L2D0F;
    case 0x2D10: goto L2D10;
    case 0x2D16: goto L2D16;
    case 0x2D18: goto L2D18;
    case 0x2D1A: goto L2D1A;
    case 0x2D1B: goto L2D1B;
    case 0x2D1D: goto L2D1D;
    case 0x2D1F: goto L2D1F;
    case 0x2D21: goto L2D21;
    case 0x2D24: goto L2D24;
    case 0x2D27: goto L2D27;
    case 0x2D2A: goto L2D2A;
    case 0x2D2C: goto L2D2C;
    case 0x2D2D: goto L2D2D;
    case 0x2D2F: goto L2D2F;
    case 0x2D31: goto L2D31;
    case 0x2D34: goto L2D34;
    case 0x2D37: goto L2D37;
    case 0x2D3A: goto L2D3A;
    case 0x2D3D: goto L2D3D;
    case 0x2D40: goto L2D40;
    case 0x2D42: goto L2D42;
    case 0x2D45: goto L2D45;
    case 0x2D47: goto L2D47;
    case 0x2D4B: goto L2D4B;
    case 0x2D4F: goto L2D4F;
    case 0x2D51: goto L2D51;
    case 0x2D55: goto L2D55;
    case 0x2D58: goto L2D58;
    case 0x2D59: goto L2D59;
    case 0x2D5B: goto L2D5B;
    case 0x2D5C: goto L2D5C;
    case 0x2D5F: goto L2D5F;
    case 0x2D62: goto L2D62;
    case 0x2D65: goto L2D65;
    case 0x2D67: goto L2D67;
    case 0x2D69: goto L2D69;
    case 0x2D6C: goto L2D6C;
    case 0x2D6D: goto L2D6D;
    case 0x2D72: goto L2D72;
    case 0x2D74: goto L2D74;
    case 0x2D78: goto L2D78;
    case 0x2D7A: goto L2D7A;
    case 0x2D7C: goto L2D7C;
    case 0x2D7D: goto L2D7D;
    case 0x2D7F: goto L2D7F;
    case 0x2D81: goto L2D81;
    case 0x2D84: goto L2D84;
    case 0x2D86: goto L2D86;
    case 0x2D88: goto L2D88;
    case 0x2D8A: goto L2D8A;
    case 0x2D8C: goto L2D8C;
    case 0x2D8D: goto L2D8D;
    case 0x2D8E: goto L2D8E;
    case 0x2D90: goto L2D90;
    case 0x2D93: goto L2D93;
    case 0x2D95: goto L2D95;
    case 0x2D97: goto L2D97;
    case 0x2D99: goto L2D99;
    case 0x2D9B: goto L2D9B;
    case 0x2D9C: goto L2D9C;
    case 0x2D9D: goto L2D9D;
    case 0x2D9F: goto L2D9F;
    case 0x2DA3: goto L2DA3;
    case 0x2DA5: goto L2DA5;
    case 0x2DA7: goto L2DA7;
    case 0x2DA9: goto L2DA9;
    case 0x2DAB: goto L2DAB;
    case 0x2DAF: goto L2DAF;
    case 0x2DB1: goto L2DB1;
    case 0x2DB3: goto L2DB3;
    case 0x2DB4: goto L2DB4;
    case 0x2DB6: goto L2DB6;
    case 0x2DB8: goto L2DB8;
    case 0x2DBA: goto L2DBA;
    case 0x2DBE: goto L2DBE;
    case 0x2DC0: goto L2DC0;
    case 0x2DC2: goto L2DC2;
    case 0x2DC3: goto L2DC3;
    case 0x2DC5: goto L2DC5;
    case 0x2DC7: goto L2DC7;
    case 0x2DC9: goto L2DC9;
    case 0x2DCB: goto L2DCB;
    case 0x2DCD: goto L2DCD;
    case 0x2DCF: goto L2DCF;
    case 0x2DD1: goto L2DD1;
    case 0x2DD3: goto L2DD3;
    case 0x2DD6: goto L2DD6;
    case 0x2DD8: goto L2DD8;
    case 0x2DDB: goto L2DDB;
    case 0x2DDD: goto L2DDD;
    case 0x2DE1: goto L2DE1;
    case 0x2DE6: goto L2DE6;
    case 0x2DE8: goto L2DE8;
    case 0x2DE9: goto L2DE9;
    case 0x2DEB: goto L2DEB;
    case 0x2DEF: goto L2DEF;
    case 0x2DF2: goto L2DF2;
    case 0x2DF6: goto L2DF6;
    case 0x2DF8: goto L2DF8;
    case 0x2DF9: goto L2DF9;
    case 0x2DFE: goto L2DFE;
    case 0x2E00: goto L2E00;
    case 0x2E03: goto L2E03;
    case 0x2E06: goto L2E06;
    case 0x2E09: goto L2E09;
    case 0x2E0C: goto L2E0C;
    case 0x2E0E: goto L2E0E;
    case 0x2E11: goto L2E11;
    case 0x2E12: goto L2E12;
    case 0x2E14: goto L2E14;
    case 0x2E18: goto L2E18;
    case 0x2E1C: goto L2E1C;
    case 0x2E20: goto L2E20;
    case 0x2E23: goto L2E23;
    case 0x2E24: goto L2E24;
    case 0x2E26: goto L2E26;
    case 0x2E27: goto L2E27;
    case 0x2E2A: goto L2E2A;
    case 0x2E2D: goto L2E2D;
    case 0x2E30: goto L2E30;
    case 0x2E32: goto L2E32;
    case 0x2E34: goto L2E34;
    case 0x2E37: goto L2E37;
    case 0x2E3B: goto L2E3B;
    case 0x2E3F: goto L2E3F;
    case 0x2E41: goto L2E41;
    case 0x2E42: goto L2E42;
    case 0x2E44: goto L2E44;
    case 0x2E45: goto L2E45;
    case 0x2E48: goto L2E48;
    case 0x2E49: goto L2E49;
    case 0x2E4B: goto L2E4B;
    case 0x2E4E: goto L2E4E;
    case 0x2E51: goto L2E51;
    case 0x2E54: goto L2E54;
    case 0x2E55: goto L2E55;
    case 0x2E57: goto L2E57;
    case 0x2E5A: goto L2E5A;
    case 0x2E5B: goto L2E5B;
    case 0x2E5D: goto L2E5D;
    case 0x2E61: goto L2E61;
    case 0x2E65: goto L2E65;
    case 0x2E68: goto L2E68;
    case 0x2E6C: goto L2E6C;
    case 0x2E6D: goto L2E6D;
    case 0x2E70: goto L2E70;
    case 0x2E72: goto L2E72;
    case 0x2E74: goto L2E74;
    case 0x2E75: goto L2E75;
    case 0x2E78: goto L2E78;
    case 0x2E79: goto L2E79;
    case 0x2E7B: goto L2E7B;
    case 0x2E7C: goto L2E7C;
    case 0x2E7F: goto L2E7F;
    case 0x2E81: goto L2E81;
    case 0x2E83: goto L2E83;
    case 0x2E85: goto L2E85;
    case 0x2E88: goto L2E88;
    case 0x2E8A: goto L2E8A;
    case 0x2E8C: goto L2E8C;
    case 0x2E8F: goto L2E8F;
    case 0x2E91: goto L2E91;
    case 0x2E93: goto L2E93;
    case 0x2E99: goto L2E99;
    case 0x2E9B: goto L2E9B;
    case 0x2E9D: goto L2E9D;
    case 0x2EA0: goto L2EA0;
    case 0x2EA3: goto L2EA3;
    case 0x2EA9: goto L2EA9;
    case 0x2EAB: goto L2EAB;
    case 0x2EAE: goto L2EAE;
    case 0x2EB1: goto L2EB1;
    case 0x2EB3: goto L2EB3;
    case 0x2EB5: goto L2EB5;
    case 0x2EB7: goto L2EB7;
    case 0x2EBA: goto L2EBA;
    case 0x2EBD: goto L2EBD;
    case 0x2EC0: goto L2EC0;
    case 0x2EC2: goto L2EC2;
    case 0x2EC5: goto L2EC5;
    case 0x2EC8: goto L2EC8;
    case 0x2ECA: goto L2ECA;
    case 0x2ECC: goto L2ECC;
    case 0x2ECE: goto L2ECE;
    case 0x2ECF: goto L2ECF;
    case 0x2ED1: goto L2ED1;
    case 0x2ED3: goto L2ED3;
    case 0x2ED5: goto L2ED5;
    case 0x2ED8: goto L2ED8;
    case 0x2EDB: goto L2EDB;
    case 0x2EDE: goto L2EDE;
    case 0x2EE0: goto L2EE0;
    case 0x2EE2: goto L2EE2;
    case 0x2EE4: goto L2EE4;
    case 0x2EE6: goto L2EE6;
    case 0x2EE9: goto L2EE9;
    case 0x2EEB: goto L2EEB;
    case 0x2EEE: goto L2EEE;
    case 0x2EF0: goto L2EF0;
    case 0x2EF3: goto L2EF3;
    case 0x2EF6: goto L2EF6;
    case 0x2EF9: goto L2EF9;
    case 0x2EFB: goto L2EFB;
    case 0x2EFD: goto L2EFD;
    case 0x2EFF: goto L2EFF;
    case 0x2F00: goto L2F00;
    case 0x2F02: goto L2F02;
    case 0x2F04: goto L2F04;
    case 0x2F07: goto L2F07;
    case 0x2F0A: goto L2F0A;
    case 0x2F0D: goto L2F0D;
    case 0x2F0F: goto L2F0F;
    case 0x2F12: goto L2F12;
    case 0x2F15: goto L2F15;
    case 0x2F19: goto L2F19;
    case 0x2F1C: goto L2F1C;
    case 0x2F1E: goto L2F1E;
    case 0x2F20: goto L2F20;
    case 0x2F23: goto L2F23;
    case 0x2F26: goto L2F26;
    case 0x2F28: goto L2F28;
    case 0x2F2A: goto L2F2A;
    case 0x2F2C: goto L2F2C;
    case 0x2F2F: goto L2F2F;
    case 0x2F31: goto L2F31;
    case 0x2F34: goto L2F34;
    case 0x2F36: goto L2F36;
    case 0x2F38: goto L2F38;
    case 0x2F3A: goto L2F3A;
    case 0x2F3C: goto L2F3C;
    case 0x2F3E: goto L2F3E;
    case 0x2F40: goto L2F40;
    case 0x2F43: goto L2F43;
    case 0x2F46: goto L2F46;
    case 0x2F48: goto L2F48;
    case 0x2F4B: goto L2F4B;
    case 0x2F50: goto L2F50;
    case 0x2F53: goto L2F53;
    case 0x2F57: goto L2F57;
    case 0x2F5A: goto L2F5A;
    case 0x2F5C: goto L2F5C;
    case 0x2F5E: goto L2F5E;
    case 0x2F61: goto L2F61;
    case 0x2F63: goto L2F63;
    case 0x2F65: goto L2F65;
    case 0x2F67: goto L2F67;
    case 0x2F69: goto L2F69;
    case 0x2F6C: goto L2F6C;
    case 0x2F6F: goto L2F6F;
    case 0x2F71: goto L2F71;
    case 0x2F74: goto L2F74;
    case 0x2F79: goto L2F79;
    case 0x2F7C: goto L2F7C;
    case 0x2F7E: goto L2F7E;
    case 0x2F81: goto L2F81;
    case 0x2F84: goto L2F84;
    case 0x2F87: goto L2F87;
    case 0x2F8A: goto L2F8A;
    case 0x2F8F: goto L2F8F;
    case 0x2F92: goto L2F92;
    case 0x2F96: goto L2F96;
    case 0x2F9A: goto L2F9A;
    case 0x2F9D: goto L2F9D;
    case 0x2F9F: goto L2F9F;
    case 0x2FA1: goto L2FA1;
    case 0x2FA4: goto L2FA4;
    case 0x2FA8: goto L2FA8;
    case 0x2FAB: goto L2FAB;
    case 0x2FAD: goto L2FAD;
    case 0x2FAE: goto L2FAE;
    case 0x2FB1: goto L2FB1;
    case 0x2FB3: goto L2FB3;
    case 0x2FB6: goto L2FB6;
    case 0x2FB8: goto L2FB8;
    case 0x2FBA: goto L2FBA;
    case 0x2FBE: goto L2FBE;
    case 0x2FC0: goto L2FC0;
    case 0x2FC2: goto L2FC2;
    case 0x2FC4: goto L2FC4;
    case 0x2FC6: goto L2FC6;
    case 0x2FC8: goto L2FC8;
    case 0x2FCC: goto L2FCC;
    case 0x2FCF: goto L2FCF;
    case 0x2FD2: goto L2FD2;
    case 0x2FD6: goto L2FD6;
    case 0x2FD8: goto L2FD8;
    case 0x2FDA: goto L2FDA;
    case 0x2FDD: goto L2FDD;
    case 0x2FE0: goto L2FE0;
    case 0x2FE6: goto L2FE6;
    case 0x2FEA: goto L2FEA;
    case 0x2FED: goto L2FED;
    case 0x2FEF: goto L2FEF;
    case 0x2FF3: goto L2FF3;
    case 0x2FF5: goto L2FF5;
    case 0x2FF6: goto L2FF6;
    case 0x2FF8: goto L2FF8;
    case 0x2FFB: goto L2FFB;
    case 0x2FFD: goto L2FFD;
    case 0x2FFE: goto L2FFE;
    case 0x3000: goto L3000;
    case 0x3004: goto L3004;
    case 0x3006: goto L3006;
    case 0x3008: goto L3008;
    case 0x300C: goto L300C;
    case 0x3010: goto L3010;
    case 0x3012: goto L3012;
    case 0x3014: goto L3014;
    case 0x3016: goto L3016;
    case 0x3017: goto L3017;
    case 0x301A: goto L301A;
    case 0x301C: goto L301C;
    case 0x301E: goto L301E;
    case 0x3020: goto L3020;
    case 0x3021: goto L3021;
    case 0x3024: goto L3024;
    case 0x3026: goto L3026;
    case 0x3028: goto L3028;
    case 0x302A: goto L302A;
    case 0x302E: goto L302E;
    case 0x3030: goto L3030;
    case 0x3034: goto L3034;
    case 0x3036: goto L3036;
    case 0x3038: goto L3038;
    case 0x303B: goto L303B;
    case 0x303E: goto L303E;
    case 0x3040: goto L3040;
    case 0x3043: goto L3043;
    case 0x3047: goto L3047;
    case 0x304B: goto L304B;
    case 0x304E: goto L304E;
    case 0x3051: goto L3051;
    case 0x3054: goto L3054;
    case 0x3057: goto L3057;
    case 0x3059: goto L3059;
    case 0x305C: goto L305C;
    case 0x305F: goto L305F;
    case 0x3061: goto L3061;
    case 0x3063: goto L3063;
    case 0x3066: goto L3066;
    case 0x3069: goto L3069;
    case 0x306B: goto L306B;
    case 0x306E: goto L306E;
    case 0x3072: goto L3072;
    case 0x3074: goto L3074;
    case 0x3078: goto L3078;
    case 0x307C: goto L307C;
    case 0x307F: goto L307F;
    case 0x3082: goto L3082;
    case 0x3085: goto L3085;
    case 0x3088: goto L3088;
    case 0x308B: goto L308B;
    case 0x308E: goto L308E;
    case 0x3092: goto L3092;
    case 0x3095: goto L3095;
    case 0x3097: goto L3097;
    case 0x3099: goto L3099;
    case 0x309B: goto L309B;
    case 0x309C: goto L309C;
    case 0x309E: goto L309E;
    case 0x30A2: goto L30A2;
    case 0x30A4: goto L30A4;
    case 0x30A6: goto L30A6;
    case 0x30A7: goto L30A7;
    case 0x30A9: goto L30A9;
    case 0x30AB: goto L30AB;
    case 0x30AC: goto L30AC;
    case 0x30AD: goto L30AD;
    case 0x30B1: goto L30B1;
    case 0x30B3: goto L30B3;
    case 0x30B5: goto L30B5;
    case 0x30B7: goto L30B7;
    case 0x30B9: goto L30B9;
    case 0x30BA: goto L30BA;
    case 0x30BC: goto L30BC;
    case 0x30C0: goto L30C0;
    case 0x30C2: goto L30C2;
    case 0x30C4: goto L30C4;
    case 0x30C5: goto L30C5;
    case 0x30C7: goto L30C7;
    case 0x30C9: goto L30C9;
    case 0x30CA: goto L30CA;
    case 0x30CB: goto L30CB;
    case 0x30CF: goto L30CF;
    case 0x30D1: goto L30D1;
    case 0x30D3: goto L30D3;
    case 0x30D5: goto L30D5;
    case 0x30D7: goto L30D7;
    case 0x30DB: goto L30DB;
    case 0x30DE: goto L30DE;
    case 0x30E2: goto L30E2;
    case 0x30E5: goto L30E5;
    case 0x30E8: goto L30E8;
    case 0x30EB: goto L30EB;
    case 0x30ED: goto L30ED;
    case 0x30EE: goto L30EE;
    case 0x30F0: goto L30F0;
    case 0x30F4: goto L30F4;
    case 0x30F6: goto L30F6;
    case 0x30F8: goto L30F8;
    case 0x30F9: goto L30F9;
    case 0x30FB: goto L30FB;
    case 0x30FD: goto L30FD;
    case 0x30FE: goto L30FE;
    case 0x30FF: goto L30FF;
    case 0x3103: goto L3103;
    case 0x3105: goto L3105;
    case 0x3107: goto L3107;
    case 0x3108: goto L3108;
    case 0x310A: goto L310A;
    case 0x310E: goto L310E;
    case 0x3110: goto L3110;
    case 0x3112: goto L3112;
    case 0x3113: goto L3113;
    case 0x3115: goto L3115;
    case 0x3117: goto L3117;
    case 0x3118: goto L3118;
    case 0x3119: goto L3119;
    case 0x311D: goto L311D;
    case 0x311F: goto L311F;
    case 0x3122: goto L3122;
    case 0x3125: goto L3125;
    case 0x3129: goto L3129;
    case 0x312D: goto L312D;
    case 0x3131: goto L3131;
    case 0x3134: goto L3134;
    case 0x3138: goto L3138;
    case 0x313B: goto L313B;
    case 0x313E: goto L313E;
    case 0x3141: goto L3141;
    case 0x3143: goto L3143;
    case 0x3147: goto L3147;
    case 0x3149: goto L3149;
    case 0x314A: goto L314A;
    case 0x314C: goto L314C;
    case 0x314E: goto L314E;
    case 0x3151: goto L3151;
    case 0x3154: goto L3154;
    case 0x3157: goto L3157;
    case 0x3158: goto L3158;
    case 0x315A: goto L315A;
    case 0x315D: goto L315D;
    case 0x3160: goto L3160;
    case 0x3163: goto L3163;
    case 0x3165: goto L3165;
    case 0x3167: goto L3167;
    case 0x3169: goto L3169;
    case 0x316B: goto L316B;
    case 0x316D: goto L316D;
    case 0x316F: goto L316F;
    case 0x3171: goto L3171;
    case 0x3173: goto L3173;
    case 0x3175: goto L3175;
    case 0x3178: goto L3178;
    case 0x317A: goto L317A;
    case 0x317B: goto L317B;
    case 0x317D: goto L317D;
    case 0x3180: goto L3180;
    case 0x3182: goto L3182;
    case 0x3183: goto L3183;
    case 0x3187: goto L3187;
    case 0x318B: goto L318B;
    case 0x318E: goto L318E;
    case 0x3190: goto L3190;
    case 0x3192: goto L3192;
    case 0x3194: goto L3194;
    case 0x3196: goto L3196;
    case 0x3199: goto L3199;
    case 0x319B: goto L319B;
    case 0x319C: goto L319C;
    case 0x319E: goto L319E;
    case 0x31A0: goto L31A0;
    case 0x31A3: goto L31A3;
    case 0x31A6: goto L31A6;
    case 0x31A9: goto L31A9;
    case 0x31AC: goto L31AC;
    case 0x31AE: goto L31AE;
    case 0x31B0: goto L31B0;
    case 0x31B3: goto L31B3;
    case 0x31B5: goto L31B5;
    case 0x31B7: goto L31B7;
    case 0x31B9: goto L31B9;
    case 0x31BB: goto L31BB;
    case 0x31BD: goto L31BD;
    case 0x31BF: goto L31BF;
    case 0x31C3: goto L31C3;
    case 0x31C6: goto L31C6;
    case 0x31CA: goto L31CA;
    case 0x31CC: goto L31CC;
    case 0x31CE: goto L31CE;
    case 0x31D0: goto L31D0;
    case 0x31D2: goto L31D2;
    case 0x31D3: goto L31D3;
    case 0x31D4: goto L31D4;
    case 0x31D6: goto L31D6;
    case 0x31D9: goto L31D9;
    case 0x31DC: goto L31DC;
    case 0x31DF: goto L31DF;
    case 0x31E2: goto L31E2;
    case 0x31E4: goto L31E4;
    case 0x31E5: goto L31E5;
    case 0x31E6: goto L31E6;
    case 0x31E8: goto L31E8;
    case 0x31EB: goto L31EB;
    case 0x31EE: goto L31EE;
    case 0x31F1: goto L31F1;
    case 0x31F4: goto L31F4;
    case 0x31F6: goto L31F6;
    case 0x31F7: goto L31F7;
    case 0x31F9: goto L31F9;
    case 0x31FB: goto L31FB;
    case 0x31FF: goto L31FF;
    case 0x3200: goto L3200;
    case 0x3204: goto L3204;
    case 0x3208: goto L3208;
    case 0x320A: goto L320A;
    case 0x320D: goto L320D;
    case 0x3210: goto L3210;
    case 0x3214: goto L3214;
    case 0x3218: goto L3218;
    case 0x321C: goto L321C;
    case 0x321D: goto L321D;
    case 0x321E: goto L321E;
    case 0x3221: goto L3221;
    case 0x3223: goto L3223;
    case 0x3226: goto L3226;
    case 0x3227: goto L3227;
    case 0x322A: goto L322A;
    case 0x322D: goto L322D;
    case 0x322F: goto L322F;
    case 0x3230: goto L3230;
    case 0x3236: goto L3236;
    case 0x3239: goto L3239;
    case 0x323C: goto L323C;
    case 0x323E: goto L323E;
    case 0x323F: goto L323F;
    case 0x3240: goto L3240;
    case 0x3241: goto L3241;
    case 0x3244: goto L3244;
    case 0x3247: goto L3247;
    case 0x3249: goto L3249;
    case 0x324C: goto L324C;
    case 0x3250: goto L3250;
    case 0x3254: goto L3254;
    case 0x3256: goto L3256;
    case 0x3257: goto L3257;
    case 0x325B: goto L325B;
    case 0x325D: goto L325D;
    case 0x325F: goto L325F;
    case 0x3261: goto L3261;
    case 0x3263: goto L3263;
    case 0x3267: goto L3267;
    case 0x326A: goto L326A;
    case 0x326B: goto L326B;
    case 0x326C: goto L326C;
    case 0x326D: goto L326D;
    case 0x3270: goto L3270;
    case 0x3271: goto L3271;
    case 0x3274: goto L3274;
    case 0x3276: goto L3276;
    case 0x3279: goto L3279;
    case 0x327A: goto L327A;
    case 0x327B: goto L327B;
    case 0x327C: goto L327C;
    case 0x327D: goto L327D;
    case 0x327F: goto L327F;
    case 0x3281: goto L3281;
    case 0x3284: goto L3284;
    case 0x3286: goto L3286;
    case 0x3287: goto L3287;
    case 0x3288: goto L3288;
    case 0x328B: goto L328B;
    case 0x328C: goto L328C;
    case 0x328E: goto L328E;
    case 0x3291: goto L3291;
    case 0x3293: goto L3293;
    case 0x3294: goto L3294;
    case 0x3295: goto L3295;
    case 0x3296: goto L3296;
    case 0x3298: goto L3298;
    case 0x3299: goto L3299;
    case 0x329B: goto L329B;
    case 0x329E: goto L329E;
    case 0x32A1: goto L32A1;
    case 0x32A4: goto L32A4;
    case 0x32A7: goto L32A7;
    case 0x32AA: goto L32AA;
    case 0x32AD: goto L32AD;
    case 0x32B0: goto L32B0;
    case 0x32B3: goto L32B3;
    case 0x32B6: goto L32B6;
    case 0x32B7: goto L32B7;
    case 0x32B9: goto L32B9;
    case 0x32BB: goto L32BB;
    case 0x32BE: goto L32BE;
    case 0x32C1: goto L32C1;
    case 0x32C2: goto L32C2;
    case 0x32C6: goto L32C6;
    case 0x32C8: goto L32C8;
    case 0x32CB: goto L32CB;
    case 0x32CD: goto L32CD;
    case 0x32D0: goto L32D0;
    case 0x32D2: goto L32D2;
    case 0x32D5: goto L32D5;
    case 0x32D9: goto L32D9;
    case 0x32DC: goto L32DC;
    case 0x32DF: goto L32DF;
    case 0x32E1: goto L32E1;
    case 0x32E4: goto L32E4;
    case 0x32E7: goto L32E7;
    case 0x32EA: goto L32EA;
    case 0x32ED: goto L32ED;
    case 0x32F0: goto L32F0;
    case 0x32F3: goto L32F3;
    case 0x32F4: goto L32F4;
    case 0x32F9: goto L32F9;
    case 0x32FB: goto L32FB;
    case 0x32FC: goto L32FC;
    case 0x3302: goto L3302;
    case 0x3305: goto L3305;
    case 0x3308: goto L3308;
    case 0x330A: goto L330A;
    case 0x330D: goto L330D;
    case 0x330E: goto L330E;
    case 0x3311: goto L3311;
    case 0x3312: goto L3312;
    case 0x3314: goto L3314;
    case 0x3316: goto L3316;
    case 0x3318: goto L3318;
    case 0x331B: goto L331B;
    case 0x331C: goto L331C;
    case 0x331D: goto L331D;
    case 0x3320: goto L3320;
    case 0x3323: goto L3323;
    case 0x3324: goto L3324;
    case 0x3325: goto L3325;
    case 0x332B: goto L332B;
    case 0x332D: goto L332D;
    case 0x332F: goto L332F;
    case 0x3331: goto L3331;
    case 0x3333: goto L3333;
    case 0x3335: goto L3335;
    case 0x3336: goto L3336;
    case 0x3337: goto L3337;
    case 0x3338: goto L3338;
    case 0x3339: goto L3339;
    case 0x333A: goto L333A;
    case 0x333B: goto L333B;
    case 0x333C: goto L333C;
    case 0x333E: goto L333E;
    case 0x333F: goto L333F;
    case 0x3343: goto L3343;
    case 0x3345: goto L3345;
    case 0x3349: goto L3349;
    case 0x334B: goto L334B;
    case 0x334E: goto L334E;
    case 0x3350: goto L3350;
    case 0x3351: goto L3351;
    case 0x3353: goto L3353;
    case 0x3355: goto L3355;
    case 0x3356: goto L3356;
    case 0x3357: goto L3357;
    case 0x335A: goto L335A;
    case 0x335D: goto L335D;
    case 0x3360: goto L3360;
    case 0x3364: goto L3364;
    case 0x3365: goto L3365;
    case 0x3367: goto L3367;
    case 0x336A: goto L336A;
    case 0x336E: goto L336E;
    case 0x3370: goto L3370;
    case 0x3374: goto L3374;
    case 0x3376: goto L3376;
    case 0x3378: goto L3378;
    case 0x337A: goto L337A;
    case 0x337E: goto L337E;
    case 0x3382: goto L3382;
    case 0x3385: goto L3385;
    case 0x3386: goto L3386;
    case 0x338A: goto L338A;
    case 0x338C: goto L338C;
    case 0x3390: goto L3390;
    case 0x3394: goto L3394;
    case 0x3398: goto L3398;
    case 0x3399: goto L3399;
    case 0x339A: goto L339A;
    case 0x339B: goto L339B;
    case 0x339D: goto L339D;
    case 0x339E: goto L339E;
    case 0x33A0: goto L33A0;
    case 0x33A1: goto L33A1;
    case 0x33A3: goto L33A3;
    case 0x33A4: goto L33A4;
    case 0x33A6: goto L33A6;
    case 0x33A7: goto L33A7;
    case 0x33A9: goto L33A9;
    case 0x33AA: goto L33AA;
    case 0x33AC: goto L33AC;
    case 0x33AD: goto L33AD;
    case 0x33AF: goto L33AF;
    case 0x33B0: goto L33B0;
    case 0x33B2: goto L33B2;
    case 0x33B3: goto L33B3;
    case 0x33B5: goto L33B5;
    case 0x33B6: goto L33B6;
    case 0x33B8: goto L33B8;
    case 0x33BB: goto L33BB;
    case 0x33BD: goto L33BD;
    case 0x33BE: goto L33BE;
    case 0x33C4: goto L33C4;
    case 0x33C5: goto L33C5;
    case 0x33C8: goto L33C8;
    case 0x33CB: goto L33CB;
    case 0x33CE: goto L33CE;
    case 0x33D1: goto L33D1;
    case 0x33D4: goto L33D4;
    case 0x33D7: goto L33D7;
    case 0x33D8: goto L33D8;
    case 0x33D9: goto L33D9;
    case 0x33DC: goto L33DC;
    case 0x33DE: goto L33DE;
    case 0x33DF: goto L33DF;
    case 0x33E0: goto L33E0;
    case 0x33E2: goto L33E2;
    case 0x33E3: goto L33E3;
    case 0x33E7: goto L33E7;
    case 0x33EA: goto L33EA;
    case 0x33EC: goto L33EC;
    case 0x33ED: goto L33ED;
    case 0x33EF: goto L33EF;
    case 0x33F1: goto L33F1;
    case 0x33F3: goto L33F3;
    case 0x33F4: goto L33F4;
    case 0x33F5: goto L33F5;
    case 0x33F7: goto L33F7;
    case 0x33F9: goto L33F9;
    case 0x33FA: goto L33FA;
    case 0x33FB: goto L33FB;
    case 0x33FE: goto L33FE;
    case 0x3400: goto L3400;
    case 0x3402: goto L3402;
    case 0x3404: goto L3404;
    case 0x3407: goto L3407;
    case 0x3409: goto L3409;
    case 0x340C: goto L340C;
    case 0x340E: goto L340E;
    case 0x3411: goto L3411;
    case 0x3413: goto L3413;
    case 0x3416: goto L3416;
    case 0x3418: goto L3418;
    case 0x3419: goto L3419;
    case 0x341A: goto L341A;
    case 0x341B: goto L341B;
    case 0x341C: goto L341C;
    case 0x341F: goto L341F;
    case 0x3421: goto L3421;
    case 0x3424: goto L3424;
    case 0x3426: goto L3426;
    case 0x3427: goto L3427;
    case 0x3428: goto L3428;
    case 0x3429: goto L3429;
    case 0x342B: goto L342B;
    case 0x342D: goto L342D;
    case 0x342E: goto L342E;
    case 0x3431: goto L3431;
    case 0x3433: goto L3433;
    case 0x3434: goto L3434;
    case 0x3435: goto L3435;
    case 0x3436: goto L3436;
    case 0x3438: goto L3438;
    case 0x3439: goto L3439;
    case 0x343A: goto L343A;
    case 0x343C: goto L343C;
    case 0x343D: goto L343D;
    case 0x343E: goto L343E;
    case 0x343F: goto L343F;
    case 0x3441: goto L3441;
    case 0x3442: goto L3442;
    case 0x3445: goto L3445;
    case 0x3448: goto L3448;
    case 0x3449: goto L3449;
    case 0x344C: goto L344C;
    case 0x344F: goto L344F;
    case 0x3450: goto L3450;
    case 0x3451: goto L3451;
    case 0x3455: goto L3455;
    case 0x3457: goto L3457;
    case 0x3459: goto L3459;
    case 0x345C: goto L345C;
    case 0x345E: goto L345E;
    case 0x345F: goto L345F;
    case 0x3462: goto L3462;
    case 0x3464: goto L3464;
    case 0x3465: goto L3465;
    case 0x3466: goto L3466;
    case 0x3467: goto L3467;
    case 0x3469: goto L3469;
    case 0x346A: goto L346A;
    case 0x346B: goto L346B;
    case 0x346D: goto L346D;
    case 0x346E: goto L346E;
    case 0x346F: goto L346F;
    case 0x3470: goto L3470;
    case 0x3472: goto L3472;
    case 0x3473: goto L3473;
    case 0x3476: goto L3476;
    case 0x3478: goto L3478;
    case 0x3479: goto L3479;
    case 0x347A: goto L347A;
    case 0x347D: goto L347D;
    case 0x347F: goto L347F;
    case 0x3482: goto L3482;
    case 0x3484: goto L3484;
    case 0x3487: goto L3487;
    case 0x3489: goto L3489;
    case 0x348C: goto L348C;
    case 0x348E: goto L348E;
    case 0x3491: goto L3491;
    case 0x3493: goto L3493;
    case 0x3496: goto L3496;
    case 0x3498: goto L3498;
    case 0x349B: goto L349B;
    case 0x349F: goto L349F;
    case 0x34A0: goto L34A0;
    case 0x34A2: goto L34A2;
    case 0x34A4: goto L34A4;
    case 0x34A7: goto L34A7;
    case 0x34AB: goto L34AB;
    case 0x34AE: goto L34AE;
    case 0x34B1: goto L34B1;
    case 0x34B5: goto L34B5;
    case 0x34B6: goto L34B6;
    case 0x34B8: goto L34B8;
    case 0x34BA: goto L34BA;
    case 0x34BD: goto L34BD;
    case 0x34C1: goto L34C1;
    case 0x34C4: goto L34C4;
    case 0x34C5: goto L34C5;
    case 0x34C8: goto L34C8;
    case 0x34CC: goto L34CC;
    case 0x34CD: goto L34CD;
    case 0x34CF: goto L34CF;
    case 0x34D1: goto L34D1;
    case 0x34D4: goto L34D4;
    case 0x34D8: goto L34D8;
    case 0x34DB: goto L34DB;
    case 0x34DE: goto L34DE;
    case 0x34E2: goto L34E2;
    case 0x34E3: goto L34E3;
    case 0x34E5: goto L34E5;
    case 0x34E7: goto L34E7;
    case 0x34EA: goto L34EA;
    case 0x34EE: goto L34EE;
    case 0x34F1: goto L34F1;
    case 0x34F5: goto L34F5;
    case 0x34F7: goto L34F7;
    case 0x34FB: goto L34FB;
    case 0x34FF: goto L34FF;
    case 0x3503: goto L3503;
    case 0x3507: goto L3507;
    case 0x3509: goto L3509;
    case 0x350D: goto L350D;
    case 0x3511: goto L3511;
    case 0x3515: goto L3515;
    case 0x3516: goto L3516;
    case 0x3517: goto L3517;
    case 0x3518: goto L3518;
    case 0x3519: goto L3519;
    case 0x351A: goto L351A;
    case 0x351B: goto L351B;
    case 0x351E: goto L351E;
    case 0x3521: goto L3521;
    case 0x3522: goto L3522;
    case 0x3525: goto L3525;
    case 0x3527: goto L3527;
    case 0x3528: goto L3528;
    case 0x352C: goto L352C;
    case 0x352E: goto L352E;
    case 0x3532: goto L3532;
    case 0x3534: goto L3534;
    case 0x3535: goto L3535;
    case 0x3537: goto L3537;
    case 0x353A: goto L353A;
    case 0x353B: goto L353B;
    case 0x353C: goto L353C;
    case 0x353E: goto L353E;
    case 0x3540: goto L3540;
    case 0x3542: goto L3542;
    case 0x3543: goto L3543;
    case 0x3545: goto L3545;
    case 0x3546: goto L3546;
    case 0x3548: goto L3548;
    case 0x354C: goto L354C;
    case 0x3550: goto L3550;
    case 0x3552: goto L3552;
    case 0x3554: goto L3554;
    case 0x3556: goto L3556;
    case 0x3558: goto L3558;
    case 0x355A: goto L355A;
    case 0x355C: goto L355C;
    case 0x355E: goto L355E;
    case 0x355F: goto L355F;
    case 0x3563: goto L3563;
    case 0x3565: goto L3565;
    case 0x3568: goto L3568;
    case 0x3569: goto L3569;
    case 0x356A: goto L356A;
    case 0x356C: goto L356C;
    case 0x356E: goto L356E;
    case 0x356F: goto L356F;
    case 0x3571: goto L3571;
    case 0x3574: goto L3574;
    case 0x3575: goto L3575;
    case 0x3577: goto L3577;
    case 0x3579: goto L3579;
    case 0x357A: goto L357A;
    case 0x357C: goto L357C;
    case 0x357F: goto L357F;
    case 0x3580: goto L3580;
    case 0x3582: goto L3582;
    case 0x3585: goto L3585;
    case 0x3588: goto L3588;
    case 0x3589: goto L3589;
    case 0x358B: goto L358B;
    case 0x358C: goto L358C;
    case 0x358D: goto L358D;
    case 0x358E: goto L358E;
    case 0x358F: goto L358F;
    case 0x3592: goto L3592;
    case 0x3593: goto L3593;
    case 0x3595: goto L3595;
    case 0x3596: goto L3596;
    case 0x3597: goto L3597;
    case 0x359A: goto L359A;
    case 0x359D: goto L359D;
    case 0x35A1: goto L35A1;
    case 0x35A2: goto L35A2;
    case 0x35A6: goto L35A6;
    case 0x35A8: goto L35A8;
    case 0x35AA: goto L35AA;
    case 0x35AC: goto L35AC;
    case 0x35AE: goto L35AE;
    case 0x35B0: goto L35B0;
    case 0x35B2: goto L35B2;
    case 0x35B4: goto L35B4;
    case 0x35B6: goto L35B6;
    case 0x35B8: goto L35B8;
    case 0x35BA: goto L35BA;
    case 0x35BE: goto L35BE;
    case 0x35C0: goto L35C0;
    case 0x35C1: goto L35C1;
    case 0x35C3: goto L35C3;
    case 0x35C4: goto L35C4;
    case 0x35C5: goto L35C5;
    case 0x35C7: goto L35C7;
    case 0x35C9: goto L35C9;
    case 0x35CB: goto L35CB;
    case 0x35CC: goto L35CC;
    case 0x35CE: goto L35CE;
    case 0x35CF: goto L35CF;
    case 0x35D1: goto L35D1;
    case 0x35D5: goto L35D5;
    case 0x35D9: goto L35D9;
    case 0x35DB: goto L35DB;
    case 0x35DD: goto L35DD;
    case 0x35DF: goto L35DF;
    case 0x35E1: goto L35E1;
    case 0x35E3: goto L35E3;
    case 0x35E5: goto L35E5;
    case 0x35E7: goto L35E7;
    case 0x35E8: goto L35E8;
    case 0x35EC: goto L35EC;
    case 0x35EE: goto L35EE;
    case 0x35EF: goto L35EF;
    case 0x35F0: goto L35F0;
    case 0x35F2: goto L35F2;
    case 0x35F3: goto L35F3;
    case 0x35F5: goto L35F5;
    case 0x35F7: goto L35F7;
    case 0x35F9: goto L35F9;
    case 0x35FA: goto L35FA;
    case 0x35FC: goto L35FC;
    case 0x35FD: goto L35FD;
    case 0x35FF: goto L35FF;
    case 0x3600: goto L3600;
    case 0x3601: goto L3601;
    case 0x3604: goto L3604;
    case 0x3607: goto L3607;
    case 0x360B: goto L360B;
    case 0x360C: goto L360C;
    case 0x3610: goto L3610;
    case 0x3612: goto L3612;
    case 0x3614: goto L3614;
    case 0x3616: goto L3616;
    case 0x3618: goto L3618;
    case 0x361A: goto L361A;
    case 0x361C: goto L361C;
    case 0x361E: goto L361E;
    case 0x3620: goto L3620;
    case 0x3622: goto L3622;
    case 0x3624: goto L3624;
    case 0x3628: goto L3628;
    case 0x362A: goto L362A;
    case 0x362B: goto L362B;
    case 0x362D: goto L362D;
    case 0x362E: goto L362E;
    case 0x362F: goto L362F;
    case 0x3631: goto L3631;
    case 0x3633: goto L3633;
    case 0x3635: goto L3635;
    case 0x3636: goto L3636;
    case 0x3638: goto L3638;
    case 0x3639: goto L3639;
    case 0x363B: goto L363B;
    case 0x363F: goto L363F;
    case 0x3643: goto L3643;
    case 0x3645: goto L3645;
    case 0x3647: goto L3647;
    case 0x3649: goto L3649;
    case 0x364B: goto L364B;
    case 0x364D: goto L364D;
    case 0x364F: goto L364F;
    case 0x3651: goto L3651;
    case 0x3652: goto L3652;
    case 0x3656: goto L3656;
    case 0x3658: goto L3658;
    case 0x3659: goto L3659;
    case 0x365A: goto L365A;
    case 0x365C: goto L365C;
    case 0x365D: goto L365D;
    case 0x365F: goto L365F;
    case 0x3661: goto L3661;
    case 0x3663: goto L3663;
    case 0x3664: goto L3664;
    case 0x3666: goto L3666;
    case 0x3667: goto L3667;
    case 0x3669: goto L3669;
    case 0x366B: goto L366B;
    case 0x366E: goto L366E;
    case 0x366F: goto L366F;
    case 0x3670: goto L3670;
    case 0x3671: goto L3671;
    case 0x3674: goto L3674;
    case 0x3677: goto L3677;
    case 0x367A: goto L367A;
    case 0x367B: goto L367B;
    case 0x367E: goto L367E;
    case 0x3680: goto L3680;
    case 0x3681: goto L3681;
    case 0x3683: goto L3683;
    case 0x3687: goto L3687;
    case 0x3688: goto L3688;
    case 0x368C: goto L368C;
    case 0x368E: goto L368E;
    case 0x3690: goto L3690;
    case 0x3692: goto L3692;
    case 0x3695: goto L3695;
    case 0x3698: goto L3698;
    case 0x369B: goto L369B;
    case 0x369D: goto L369D;
    case 0x369F: goto L369F;
    case 0x36A1: goto L36A1;
    case 0x36A3: goto L36A3;
    case 0x36A5: goto L36A5;
    case 0x36A6: goto L36A6;
    case 0x36AA: goto L36AA;
    case 0x36AD: goto L36AD;
    case 0x36AF: goto L36AF;
    case 0x36B2: goto L36B2;
    case 0x36B5: goto L36B5;
    case 0x36B6: goto L36B6;
    case 0x36B7: goto L36B7;
    case 0x36B8: goto L36B8;
    case 0x36BB: goto L36BB;
    case 0x36BE: goto L36BE;
    case 0x36C1: goto L36C1;
    case 0x36C2: goto L36C2;
    case 0x36C5: goto L36C5;
    case 0x36C7: goto L36C7;
    case 0x36C8: goto L36C8;
    case 0x36CA: goto L36CA;
    case 0x36CE: goto L36CE;
    case 0x36CF: goto L36CF;
    case 0x36D3: goto L36D3;
    case 0x36D5: goto L36D5;
    case 0x36D7: goto L36D7;
    case 0x36D9: goto L36D9;
    case 0x36DC: goto L36DC;
    case 0x36DF: goto L36DF;
    case 0x36E2: goto L36E2;
    case 0x36E4: goto L36E4;
    case 0x36E6: goto L36E6;
    case 0x36E8: goto L36E8;
    case 0x36EA: goto L36EA;
    case 0x36EC: goto L36EC;
    case 0x36ED: goto L36ED;
    case 0x36F1: goto L36F1;
    case 0x36F3: goto L36F3;
    case 0x36F5: goto L36F5;
    case 0x36F8: goto L36F8;
    case 0x36FA: goto L36FA;
    case 0x36FB: goto L36FB;
    case 0x36FE: goto L36FE;
    case 0x36FF: goto L36FF;
    case 0x3702: goto L3702;
    case 0x3706: goto L3706;
    case 0x370A: goto L370A;
    case 0x370D: goto L370D;
    case 0x370F: goto L370F;
    case 0x3712: goto L3712;
    case 0x3715: goto L3715;
    case 0x3716: goto L3716;
    case 0x3717: goto L3717;
    case 0x3718: goto L3718;
    case 0x371B: goto L371B;
    case 0x371E: goto L371E;
    case 0x3721: goto L3721;
    case 0x3722: goto L3722;
    case 0x3725: goto L3725;
    case 0x3727: goto L3727;
    case 0x3728: goto L3728;
    case 0x372C: goto L372C;
    case 0x372E: goto L372E;
    case 0x3730: goto L3730;
    case 0x3732: goto L3732;
    case 0x3734: goto L3734;
    case 0x3736: goto L3736;
    case 0x3738: goto L3738;
    case 0x373A: goto L373A;
    case 0x373C: goto L373C;
    case 0x373E: goto L373E;
    case 0x3742: goto L3742;
    case 0x3746: goto L3746;
    case 0x3748: goto L3748;
    case 0x3749: goto L3749;
    case 0x374C: goto L374C;
    case 0x374D: goto L374D;
    case 0x374F: goto L374F;
    case 0x3751: goto L3751;
    case 0x3753: goto L3753;
    case 0x3756: goto L3756;
    case 0x3759: goto L3759;
    case 0x375C: goto L375C;
    case 0x3760: goto L3760;
    case 0x3764: goto L3764;
    case 0x3766: goto L3766;
    case 0x3768: goto L3768;
    case 0x376A: goto L376A;
    case 0x376C: goto L376C;
    case 0x376E: goto L376E;
    case 0x3770: goto L3770;
    case 0x3772: goto L3772;
    case 0x3776: goto L3776;
    case 0x3778: goto L3778;
    case 0x377C: goto L377C;
    case 0x377D: goto L377D;
    case 0x377F: goto L377F;
    case 0x3780: goto L3780;
    case 0x3782: goto L3782;
    case 0x3783: goto L3783;
    case 0x3786: goto L3786;
    case 0x3788: goto L3788;
    case 0x3789: goto L3789;
    case 0x378B: goto L378B;
    case 0x378D: goto L378D;
    case 0x3790: goto L3790;
    case 0x3793: goto L3793;
    case 0x3794: goto L3794;
    case 0x3795: goto L3795;
    case 0x3796: goto L3796;
    case 0x3799: goto L3799;
    case 0x379C: goto L379C;
    case 0x379F: goto L379F;
    case 0x37A0: goto L37A0;
    case 0x37A3: goto L37A3;
    case 0x37A5: goto L37A5;
    case 0x37A6: goto L37A6;
    case 0x37AA: goto L37AA;
    case 0x37AC: goto L37AC;
    case 0x37AE: goto L37AE;
    case 0x37B0: goto L37B0;
    case 0x37B2: goto L37B2;
    case 0x37B4: goto L37B4;
    case 0x37B6: goto L37B6;
    case 0x37B8: goto L37B8;
    case 0x37BA: goto L37BA;
    case 0x37BC: goto L37BC;
    case 0x37C0: goto L37C0;
    case 0x37C4: goto L37C4;
    case 0x37C6: goto L37C6;
    case 0x37C7: goto L37C7;
    case 0x37CA: goto L37CA;
    case 0x37CB: goto L37CB;
    case 0x37CD: goto L37CD;
    case 0x37CF: goto L37CF;
    case 0x37D1: goto L37D1;
    case 0x37D4: goto L37D4;
    case 0x37D7: goto L37D7;
    case 0x37DA: goto L37DA;
    case 0x37DE: goto L37DE;
    case 0x37E2: goto L37E2;
    case 0x37E4: goto L37E4;
    case 0x37E6: goto L37E6;
    case 0x37E8: goto L37E8;
    case 0x37EA: goto L37EA;
    case 0x37EC: goto L37EC;
    case 0x37EE: goto L37EE;
    case 0x37F0: goto L37F0;
    case 0x37F4: goto L37F4;
    case 0x37F6: goto L37F6;
    case 0x37FA: goto L37FA;
    case 0x37FB: goto L37FB;
    case 0x37FD: goto L37FD;
    case 0x37FE: goto L37FE;
    case 0x3800: goto L3800;
    case 0x3801: goto L3801;
    case 0x3804: goto L3804;
    case 0x3806: goto L3806;
    case 0x3807: goto L3807;
    case 0x3809: goto L3809;
    case 0x380B: goto L380B;
    case 0x380E: goto L380E;
    case 0x3811: goto L3811;
    case 0x3812: goto L3812;
    case 0x3813: goto L3813;
    case 0x3814: goto L3814;
    case 0x3817: goto L3817;
    case 0x381A: goto L381A;
    case 0x381D: goto L381D;
    case 0x381E: goto L381E;
    case 0x3821: goto L3821;
    case 0x3823: goto L3823;
    case 0x3824: goto L3824;
    case 0x3826: goto L3826;
    case 0x382A: goto L382A;
    case 0x382B: goto L382B;
    case 0x382F: goto L382F;
    case 0x3831: goto L3831;
    case 0x3833: goto L3833;
    case 0x3835: goto L3835;
    case 0x3838: goto L3838;
    case 0x383B: goto L383B;
    case 0x383E: goto L383E;
    case 0x3840: goto L3840;
    case 0x3842: goto L3842;
    case 0x3844: goto L3844;
    case 0x3846: goto L3846;
    case 0x3848: goto L3848;
    case 0x3849: goto L3849;
    case 0x384D: goto L384D;
    case 0x3850: goto L3850;
    case 0x3852: goto L3852;
    case 0x3853: goto L3853;
    case 0x3854: goto L3854;
    case 0x3857: goto L3857;
    case 0x385A: goto L385A;
    case 0x385C: goto L385C;
    case 0x385D: goto L385D;
    case 0x3860: goto L3860;
    case 0x3862: goto L3862;
    case 0x3863: goto L3863;
    case 0x3864: goto L3864;
    case 0x3866: goto L3866;
    case 0x386A: goto L386A;
    case 0x386B: goto L386B;
    case 0x386F: goto L386F;
    case 0x3872: goto L3872;
    case 0x3874: goto L3874;
    case 0x3876: goto L3876;
    case 0x3879: goto L3879;
    case 0x387C: goto L387C;
    case 0x387F: goto L387F;
    case 0x3881: goto L3881;
    case 0x3883: goto L3883;
    case 0x3885: goto L3885;
    case 0x3887: goto L3887;
    case 0x3889: goto L3889;
    case 0x388A: goto L388A;
    case 0x388F: goto L388F;
    case 0x3891: goto L3891;
    case 0x3893: goto L3893;
    case 0x3894: goto L3894;
    case 0x3896: goto L3896;
    case 0x3898: goto L3898;
    case 0x389A: goto L389A;
    case 0x389C: goto L389C;
    case 0x389D: goto L389D;
    case 0x389F: goto L389F;
    case 0x38A1: goto L38A1;
    case 0x38A3: goto L38A3;
    case 0x38A5: goto L38A5;
    case 0x38A6: goto L38A6;
    case 0x38A8: goto L38A8;
    case 0x38AA: goto L38AA;
    case 0x38AC: goto L38AC;
    case 0x38AE: goto L38AE;
    case 0x38AF: goto L38AF;
    case 0x38B1: goto L38B1;
    case 0x38B3: goto L38B3;
    case 0x38B4: goto L38B4;
    case 0x38B5: goto L38B5;
    case 0x38B7: goto L38B7;
    case 0x38B8: goto L38B8;
    case 0x38B9: goto L38B9;
    case 0x38BC: goto L38BC;
    case 0x38BF: goto L38BF;
    case 0x38C1: goto L38C1;
    case 0x38C2: goto L38C2;
    case 0x38C5: goto L38C5;
    case 0x38C7: goto L38C7;
    case 0x38C8: goto L38C8;
    case 0x38C9: goto L38C9;
    case 0x38CB: goto L38CB;
    case 0x38CF: goto L38CF;
    case 0x38D0: goto L38D0;
    case 0x38D4: goto L38D4;
    case 0x38D7: goto L38D7;
    case 0x38D9: goto L38D9;
    case 0x38DB: goto L38DB;
    case 0x38DE: goto L38DE;
    case 0x38E1: goto L38E1;
    case 0x38E4: goto L38E4;
    case 0x38E6: goto L38E6;
    case 0x38E8: goto L38E8;
    case 0x38EA: goto L38EA;
    case 0x38EC: goto L38EC;
    case 0x38EE: goto L38EE;
    case 0x38EF: goto L38EF;
    case 0x38F4: goto L38F4;
    case 0x38F6: goto L38F6;
    case 0x38F8: goto L38F8;
    case 0x38F9: goto L38F9;
    case 0x38FB: goto L38FB;
    case 0x38FD: goto L38FD;
    case 0x38FF: goto L38FF;
    case 0x3900: goto L3900;
    case 0x3901: goto L3901;
    case 0x3903: goto L3903;
    case 0x3905: goto L3905;
    case 0x3907: goto L3907;
    case 0x3908: goto L3908;
    case 0x3909: goto L3909;
    case 0x390B: goto L390B;
    case 0x390D: goto L390D;
    case 0x390F: goto L390F;
    case 0x3910: goto L3910;
    case 0x3911: goto L3911;
    case 0x3913: goto L3913;
    case 0x3915: goto L3915;
    case 0x3919: goto L3919;
    case 0x391B: goto L391B;
    case 0x391F: goto L391F;
    case 0x3921: goto L3921;
    case 0x3925: goto L3925;
    case 0x3927: goto L3927;
    case 0x392B: goto L392B;
    case 0x392D: goto L392D;
    case 0x3930: goto L3930;
    case 0x3931: goto L3931;
    case 0x3935: goto L3935;
    case 0x3937: goto L3937;
    case 0x393B: goto L393B;
    case 0x393F: goto L393F;
    case 0x3941: goto L3941;
    case 0x3943: goto L3943;
    case 0x3945: goto L3945;
    case 0x3949: goto L3949;
    case 0x394A: goto L394A;
    case 0x394D: goto L394D;
    case 0x394E: goto L394E;
    case 0x394F: goto L394F;
    case 0x3950: goto L3950;
    case 0x3952: goto L3952;
    case 0x3955: goto L3955;
    case 0x3957: goto L3957;
    case 0x3958: goto L3958;
    case 0x395A: goto L395A;
    case 0x395D: goto L395D;
    case 0x395F: goto L395F;
    case 0x3960: goto L3960;
    case 0x3963: goto L3963;
    case 0x3964: goto L3964;
    case 0x3968: goto L3968;
    case 0x396A: goto L396A;
    case 0x396E: goto L396E;
    case 0x3970: goto L3970;
    case 0x3974: goto L3974;
    case 0x3976: goto L3976;
    case 0x397A: goto L397A;
    case 0x397C: goto L397C;
    case 0x397E: goto L397E;
    case 0x3980: goto L3980;
    case 0x3983: goto L3983;
    case 0x3985: goto L3985;
    case 0x3988: goto L3988;
    case 0x3989: goto L3989;
    case 0x398A: goto L398A;
    case 0x398E: goto L398E;
    case 0x3990: goto L3990;
    case 0x3992: goto L3992;
    case 0x3994: goto L3994;
    case 0x3998: goto L3998;
    case 0x399B: goto L399B;
    case 0x399C: goto L399C;
    case 0x399E: goto L399E;
    case 0x399F: goto L399F;
    case 0x39A1: goto L39A1;
    case 0x39A2: goto L39A2;
    case 0x39A4: goto L39A4;
    case 0x39A6: goto L39A6;
    case 0x39A8: goto L39A8;
    case 0x39A9: goto L39A9;
    case 0x39AC: goto L39AC;
    case 0x39AF: goto L39AF;
    case 0x39B0: goto L39B0;
    case 0x39B4: goto L39B4;
    case 0x39B8: goto L39B8;
    case 0x39BB: goto L39BB;
    case 0x39BC: goto L39BC;
    case 0x39BE: goto L39BE;
    case 0x39C0: goto L39C0;
    case 0x39C2: goto L39C2;
    case 0x39C6: goto L39C6;
    case 0x39CA: goto L39CA;
    case 0x39CE: goto L39CE;
    case 0x39CF: goto L39CF;
    case 0x39D0: goto L39D0;
    case 0x39D4: goto L39D4;
    case 0x39D7: goto L39D7;
    case 0x39D8: goto L39D8;
    case 0x39DA: goto L39DA;
    case 0x39DC: goto L39DC;
    case 0x39DD: goto L39DD;
    case 0x39E0: goto L39E0;
    case 0x39E3: goto L39E3;
    case 0x39E4: goto L39E4;
    case 0x39E5: goto L39E5;
    case 0x39E7: goto L39E7;
    case 0x39E9: goto L39E9;
    case 0x39EB: goto L39EB;
    case 0x39EC: goto L39EC;
    case 0x39F0: goto L39F0;
    case 0x39F4: goto L39F4;
    case 0x39F7: goto L39F7;
    case 0x39F8: goto L39F8;
    case 0x39FA: goto L39FA;
    case 0x39FC: goto L39FC;
    case 0x39FE: goto L39FE;
    case 0x3A02: goto L3A02;
    case 0x3A06: goto L3A06;
    case 0x3A07: goto L3A07;
    case 0x3A0A: goto L3A0A;
    case 0x3A0B: goto L3A0B;
    case 0x3A0F: goto L3A0F;
    case 0x3A10: goto L3A10;
    case 0x3A12: goto L3A12;
    case 0x3A14: goto L3A14;
    case 0x3A15: goto L3A15;
    case 0x3A16: goto L3A16;
    case 0x3A18: goto L3A18;
    case 0x3A1A: goto L3A1A;
    case 0x3A1C: goto L3A1C;
    case 0x3A20: goto L3A20;
    case 0x3A23: goto L3A23;
    case 0x3A27: goto L3A27;
    case 0x3A2A: goto L3A2A;
    case 0x3A2E: goto L3A2E;
    case 0x3A31: goto L3A31;
    case 0x3A33: goto L3A33;
    case 0x3A37: goto L3A37;
    case 0x3A3B: goto L3A3B;
    case 0x3A3E: goto L3A3E;
    case 0x3A3F: goto L3A3F;
    case 0x3A42: goto L3A42;
    case 0x3A45: goto L3A45;
    case 0x3A48: goto L3A48;
    case 0x3A4C: goto L3A4C;
    case 0x3A4E: goto L3A4E;
    case 0x3A50: goto L3A50;
    case 0x3A54: goto L3A54;
    case 0x3A57: goto L3A57;
    case 0x3A59: goto L3A59;
    case 0x3A5D: goto L3A5D;
    case 0x3A5E: goto L3A5E;
    case 0x3A5F: goto L3A5F;
    case 0x3A61: goto L3A61;
    case 0x3A65: goto L3A65;
    case 0x3A66: goto L3A66;
    case 0x3A67: goto L3A67;
    case 0x3A68: goto L3A68;
    case 0x3A6B: goto L3A6B;
    case 0x3A6C: goto L3A6C;
    case 0x3A6F: goto L3A6F;
    case 0x3A70: goto L3A70;
    case 0x3A71: goto L3A71;
    case 0x3A72: goto L3A72;
    case 0x3A73: goto L3A73;
    case 0x3A74: goto L3A74;
    case 0x3A77: goto L3A77;
    case 0x3A78: goto L3A78;
    case 0x3A7B: goto L3A7B;
    case 0x3A7D: goto L3A7D;
    case 0x3A80: goto L3A80;
    case 0x3A81: goto L3A81;
    case 0x3A82: goto L3A82;
    case 0x3A83: goto L3A83;
    case 0x3A84: goto L3A84;
    case 0x3A85: goto L3A85;
    default: asm_bad_entry("VIDMODE.ASM", entry);
    }

    /* seg003_2BBA  (+2BBA)
       show: put a bitmap on the screen. In (from GRCORE's show): AX, BX the screen position, DI, SI
       the source offset and segment, CX, BP the size, 0DC8, 0DCA the source's x and y offset.
       Chooses the transfer by _Transparency (_5B6C opaque, _5C61 transparent, GRLIBL.ASM) and joins
       the common path at L2C7C. The labelled entries after it are the same for other sources and
       targets: _2BD3 a bitmap already in video memory (_5D72 or _5E85 by plane alignment, _5EDD
       transparent), _2BFA fbshow (into the frame buffer, GRENTRY's _A5C or _A8D), _2C11 into the
       linear buffer set by _33E3 (_60C7 or _60F8), _2C28 vcopyfb (frame buffer to screen, GRENTRY's
       _DE3) and _2C36 vcopy (screen to screen, _6015 or, for an overlapping copy to the right,
       _606D).

       L2C7C: clip the rectangle to the window when _ShowClip is set (moving the source offsets to
       match), then write one 8-byte record per row (y, left, right, source offset) into the table at
       0DCD, end it, and jump to the transfer in 0DC4. L2D59 and L2E24 build the records forwards or,
       for vcopy, in the order an overlapping copy needs.

       After the ret at +2E7B is the concave polygon fill (FM Towns concave and uconcave, probably): clip
       with concave_shclip (GRLIBF.ASM), then build the spans of a polygon that may be concave from
       an edge list (1B86 .. 1B8C) into 2D40 and run the span writer. */
L2BBA: /* _seg003_2BBA */
    /* 2BBA  cmp     byte ptr ds:[_Transparency],0 */
    sub8(rb(pDS, 0xDC7), 0x0, 0);
L2BBF:
    /* 2BBF  jne     L2BCA */
    if (!ZF) goto L2BCA;
L2BC1:
    /* 2BC1  mov     word ptr ds:[XFER_RTN],offset _seg003_5B6C */
    ww(pDS, 0xDC4, 0x5B6C);
L2BC7:
    /* 2BC7  jmp     L2C7C */
    goto L2C7C;
L2BCA: /* L2BCA */
    /* 2BCA  mov     word ptr ds:[XFER_RTN],offset _seg003_5C61 */
    ww(pDS, 0xDC4, 0x5C61);
L2BD0:
    /* 2BD0  jmp     L2C7C */
    goto L2C7C;
L2BD3: /* _seg003_2BD3 */
    /* 2BD3  cmp     byte ptr ds:[_Transparency],0 */
    sub8(rb(pDS, 0xDC7), 0x0, 0);
L2BD8:
    /* 2BD8  jne     L2BF1 */
    if (!ZF) goto L2BF1;
L2BDA:
    /* 2BDA  test    ax,3 */
    logic16((uint16_t)(AX & 0x3));
L2BDD:
    /* 2BDD  je      L2BE8 */
    if (ZF) goto L2BE8;
L2BDF:
    /* 2BDF  mov     word ptr ds:[XFER_RTN],offset _seg003_5D72 */
    ww(pDS, 0xDC4, 0x5D72);
L2BE5:
    /* 2BE5  jmp     L2C7C */
    goto L2C7C;
L2BE8: /* L2BE8 */
    /* 2BE8  mov     word ptr ds:[XFER_RTN],offset _seg003_5E85 */
    ww(pDS, 0xDC4, 0x5E85);
L2BEE:
    /* 2BEE  jmp     L2C7C */
    goto L2C7C;
L2BF1: /* L2BF1 */
    /* 2BF1  mov     word ptr ds:[XFER_RTN],offset _seg003_5EDD */
    ww(pDS, 0xDC4, 0x5EDD);
L2BF7:
    /* 2BF7  jmp     L2C7C */
    goto L2C7C;
L2BFA: /* _seg003_2BFA */
    /* 2BFA  cmp     byte ptr ds:[_Transparency],0 */
    sub8(rb(pDS, 0xDC7), 0x0, 0);
L2BFF:
    /* 2BFF  jne     L2C09 */
    if (!ZF) goto L2C09;
L2C01:
    /* 2C01  mov     word ptr ds:[XFER_RTN],offset _seg003_A5C */
    ww(pDS, 0xDC4, 0xA5C);
L2C07:
    /* 2C07  jmp     short L2C7C */
    goto L2C7C;
L2C09: /* L2C09 */
    /* 2C09  mov     word ptr ds:[XFER_RTN],offset _seg003_A8D */
    ww(pDS, 0xDC4, 0xA8D);
L2C0F:
    /* 2C0F  jmp     short L2C7C */
    goto L2C7C;
L2C11: /* _seg003_2C11 */
    /* 2C11  cmp     byte ptr ds:[_Transparency],0 */
    sub8(rb(pDS, 0xDC7), 0x0, 0);
L2C16:
    /* 2C16  jne     L2C20 */
    if (!ZF) goto L2C20;
L2C18:
    /* 2C18  mov     word ptr ds:[XFER_RTN],offset _seg003_60C7 */
    ww(pDS, 0xDC4, 0x60C7);
L2C1E:
    /* 2C1E  jmp     short L2C7C */
    goto L2C7C;
L2C20: /* L2C20 */
    /* 2C20  mov     word ptr ds:[XFER_RTN],offset _seg003_60F8 */
    ww(pDS, 0xDC4, 0x60F8);
L2C26:
    /* 2C26  jmp     short L2C7C */
    goto L2C7C;
L2C28: /* _seg003_2C28 */
    /* 2C28  mov     word ptr ds:[XFER_RTN],offset _seg003_DE3 */
    ww(pDS, 0xDC4, 0xDE3);
L2C2E:
    /* 2C2E  mov     byte ptr ds:[0DCCh],0 */
    wb(pDS, 0xDCC, 0x0);
L2C33:
    /* 2C33  jmp     L2D59 */
    goto L2D59;
L2C36: /* _seg003_2C36 */
    /* 2C36  cmp     bx,di */
    sub16(BX, DI, 0);
L2C38:
    /* 2C38  je      L2C4C */
    if (ZF) goto L2C4C;
L2C3A:
    /* 2C3A  mov     word ptr ds:[XFER_RTN],offset _seg003_606D */
    ww(pDS, 0xDC4, 0x606D);
L2C40:
    /* 2C40  jg      L2C47 */
    if (!ZF && SF == OF) goto L2C47;
L2C42:
    /* 2C42  mov     bp,offset L2D59 */
    BP = 0x2D59;
L2C45:
    /* 2C45  jmp     short L2C61 */
    goto L2C61;
L2C47: /* L2C47 */
    /* 2C47  mov     bp,offset L2E24 */
    BP = 0x2E24;
L2C4A:
    /* 2C4A  jmp     short L2C61 */
    goto L2C61;
L2C4C: /* L2C4C */
    /* 2C4C  mov     bp,offset L2D59 */
    BP = 0x2D59;
L2C4F:
    /* 2C4F  cmp     ax,si */
    sub16(AX, SI, 0);
L2C51:
    /* 2C51  jg      L2C5B */
    if (!ZF && SF == OF) goto L2C5B;
L2C53:
    /* 2C53  mov     word ptr ds:[XFER_RTN],offset _seg003_6015 */
    ww(pDS, 0xDC4, 0x6015);
L2C59:
    /* 2C59  jmp     short L2C61 */
    goto L2C61;
L2C5B: /* L2C5B */
    /* 2C5B  mov     word ptr ds:[XFER_RTN],offset _seg003_606D */
    ww(pDS, 0xDC4, 0x606D);
L2C61: /* L2C61 */
    /* 2C61  cmp     bp,offset L2E24 */
    sub16(BP, 0x2E24, 0);
L2C65:
    /* 2C65  jne     L2C6A */
    if (!ZF) goto L2C6A;
L2C67:
    /* 2C67  sub     di,dx */
    DI = (uint16_t)(DI - DX);
L2C69:
    /* 2C69  inc     di */
    DI = (uint16_t)(DI + 1);
L2C6A: /* L2C6A */
    /* 2C6A  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L2C6C:
    /* 2C6C  mov     di,word ptr [di+YTAB] */
    DI = rw(pDS, DI + 0x36AA);
L2C70:
    /* 2C70  shr     si,2 */
    SI = (uint16_t)(SI >> 2);
L2C73:
    /* 2C73  add     di,si */
    DI = add16(DI, SI, 0);
L2C75:
    /* 2C75  mov     byte ptr ds:[0DCCh],1 */
    wb(pDS, 0xDCC, 0x1);
L2C7A:
    /* 2C7A  jmp     bp */
    return ASM_JMP(0x0090, BP);
L2C7C: /* L2C7C */
    /* 2C7C  mov     word ptr ds:[55EAh],si */
    ww(pDS, 0x55EA, SI);
L2C80:
    /* 2C80  push    di */
    push16(DI);
L2C81:
    /* 2C81  mov     di,word ptr ds:[141Dh] */
    DI = rw(pDS, 0x141D);
L2C85:
    /* 2C85  and     word ptr [di],7FFFh */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) & 0x7FFF));
L2C89:
    /* 2C89  mov     bp,word ptr ds:[0DC8h] */
    BP = rw(pDS, 0xDC8);
L2C8D:
    /* 2C8D  mov     di,word ptr ds:[0DCAh] */
    DI = rw(pDS, 0xDCA);
L2C91:
    /* 2C91  push    cx */
    push16(CX);
L2C92:
    /* 2C92  sub     cx,bp */
    CX = (uint16_t)(CX - BP);
L2C94:
    /* 2C94  sub     dx,di */
    DX = (uint16_t)(DX - DI);
L2C96:
    /* 2C96  cmp     byte ptr ds:[_ShowClip],0 */
    sub8(rb(pDS, 0xDC6), 0x0, 0);
L2C9B:
    /* 2C9B  jne     L2C9F */
    if (!ZF) goto L2C9F;
L2C9D:
    /* 2C9D  jmp     short L2CF4 */
    goto L2CF4;
L2C9F: /* L2C9F */
    /* 2C9F  mov     si,ax */
    SI = AX;
L2CA1:
    /* 2CA1  add     si,cx */
    SI = (uint16_t)(SI + CX);
L2CA3:
    /* 2CA3  dec     si */
    SI = (uint16_t)(SI - 1);
L2CA4:
    /* 2CA4  sub     si,word ptr ds:[WIN_LEFT] */
    SI = sub16(SI, rw(pDS, 0x3DF2), 0);
L2CA8:
    /* 2CA8  jge     L2CAD */
    if (SF == OF) goto L2CAD;
L2CAA:
    /* 2CAA  jmp     L2D55 */
    goto L2D55;
L2CAD: /* L2CAD */
    /* 2CAD  mov     si,word ptr ds:[WIN_LEFT] */
    SI = rw(pDS, 0x3DF2);
L2CB1:
    /* 2CB1  sub     si,ax */
    SI = sub16(SI, AX, 0);
L2CB3:
    /* 2CB3  jle     L2CBB */
    if (ZF || SF != OF) goto L2CBB;
L2CB5:
    /* 2CB5  add     ax,si */
    AX = (uint16_t)(AX + SI);
L2CB7:
    /* 2CB7  sub     cx,si */
    CX = (uint16_t)(CX - SI);
L2CB9:
    /* 2CB9  add     bp,si */
    BP = (uint16_t)(BP + SI);
L2CBB: /* L2CBB */
    /* 2CBB  mov     si,word ptr ds:[WIN_RIGHT] */
    SI = rw(pDS, 0x3DF6);
L2CBF:
    /* 2CBF  sub     si,ax */
    SI = sub16(SI, AX, 0);
L2CC1:
    /* 2CC1  jge     L2CC6 */
    if (SF == OF) goto L2CC6;
L2CC3:
    /* 2CC3  jmp     L2D55 */
    goto L2D55;
L2CC6: /* L2CC6 */
    /* 2CC6  inc     si */
    SI = (uint16_t)(SI + 1);
L2CC7:
    /* 2CC7  cmp     cx,si */
    sub16(CX, SI, 0);
L2CC9:
    /* 2CC9  jle     L2CCD */
    if (ZF || SF != OF) goto L2CCD;
L2CCB:
    /* 2CCB  mov     cx,si */
    CX = SI;
L2CCD: /* L2CCD */
    /* 2CCD  mov     si,bx */
    SI = BX;
L2CCF:
    /* 2CCF  sub     si,dx */
    SI = (uint16_t)(SI - DX);
L2CD1:
    /* 2CD1  inc     si */
    SI = (uint16_t)(SI + 1);
L2CD2:
    /* 2CD2  sub     si,word ptr ds:[WIN_TOP] */
    SI = sub16(SI, rw(pDS, 0x3DF4), 0);
L2CD6:
    /* 2CD6  jg      L2D55 */
    if (!ZF && SF == OF) goto L2D55;
L2CD8:
    /* 2CD8  mov     si,word ptr ds:[WIN_TOP] */
    SI = rw(pDS, 0x3DF4);
L2CDC:
    /* 2CDC  sub     si,bx */
    SI = sub16(SI, BX, 0);
L2CDE:
    /* 2CDE  jge     L2CE6 */
    if (SF == OF) goto L2CE6;
L2CE0:
    /* 2CE0  sub     di,si */
    DI = (uint16_t)(DI - SI);
L2CE2:
    /* 2CE2  add     dx,si */
    DX = (uint16_t)(DX + SI);
L2CE4:
    /* 2CE4  add     bx,si */
    BX = (uint16_t)(BX + SI);
L2CE6: /* L2CE6 */
    /* 2CE6  mov     si,bx */
    SI = BX;
L2CE8:
    /* 2CE8  sub     si,word ptr ds:[WIN_BOTTOM] */
    SI = sub16(SI, rw(pDS, 0x3DF8), 0);
L2CEC:
    /* 2CEC  jle     L2D55 */
    if (ZF || SF != OF) goto L2D55;
L2CEE:
    /* 2CEE  cmp     dx,si */
    sub16(DX, SI, 0);
L2CF0:
    /* 2CF0  jle     L2CF4 */
    if (ZF || SF != OF) goto L2CF4;
L2CF2:
    /* 2CF2  mov     dx,si */
    DX = SI;
L2CF4: /* L2CF4 */
    /* 2CF4  mov     si,cx */
    SI = CX;
L2CF6:
    /* 2CF6  pop     cx */
    CX = pop16();
L2CF7:
    /* 2CF7  xchg    cx,dx */
    { uint16_t t_ = DX;
    DX = CX;
    CX = t_; }
L2CF9:
    /* 2CF9  neg     bx */
    BX = (uint16_t)-BX;
L2CFB:
    /* 2CFB  add     bx,0C7h */
    BX = (uint16_t)(BX + 0xC7);
L2CFF:
    /* 2CFF  shl     bx,3 */
    BX = (uint16_t)(BX << 3);
L2D02:
    /* 2D02  add     bx,0DCDh */
    BX = (uint16_t)(BX + 0xDCD);
L2D06:
    /* 2D06  add     si,ax */
    SI = (uint16_t)(SI + AX);
L2D08:
    /* 2D08  dec     si */
    SI = (uint16_t)(SI - 1);
L2D09:
    /* 2D09  xchg    di,ax */
    { uint16_t t_ = DI;
    DI = AX;
    AX = t_; }
L2D0A:
    /* 2D0A  push    bp */
    push16(BP);
L2D0B:
    /* 2D0B  mov     bp,dx */
    BP = DX;
L2D0D:
    /* 2D0D  mul     dx */
    mul16(DX);
L2D0F:
    /* 2D0F  pop     dx */
    DX = pop16();
L2D10:
    /* 2D10  cmp     word ptr ds:[55EAh],0A000h */
    sub16(rw(pDS, 0x55EA), 0xA000, 0);
L2D16:
    /* 2D16  je      L2D21 */
    if (ZF) goto L2D21;
L2D18:
    /* 2D18  add     dx,ax */
    DX = (uint16_t)(DX + AX);
L2D1A:
    /* 2D1A  pop     ax */
    AX = pop16();
L2D1B:
    /* 2D1B  add     dx,ax */
    DX = (uint16_t)(DX + AX);
L2D1D:
    /* 2D1D  mov     ax,bx */
    AX = BX;
L2D1F:
    /* 2D1F  jmp     short L2D37 */
    goto L2D37;
L2D21: /* L2D21 */
    /* 2D21  add     ax,3 */
    AX = (uint16_t)(AX + 0x3);
L2D24:
    /* 2D24  shr     dx,2 */
    DX = (uint16_t)(DX >> 2);
L2D27:
    /* 2D27  shr     ax,2 */
    AX = (uint16_t)(AX >> 2);
L2D2A:
    /* 2D2A  add     dx,ax */
    DX = (uint16_t)(DX + AX);
L2D2C:
    /* 2D2C  pop     ax */
    AX = pop16();
L2D2D:
    /* 2D2D  add     dx,ax */
    DX = (uint16_t)(DX + AX);
L2D2F:
    /* 2D2F  mov     ax,bx */
    AX = BX;
L2D31:
    /* 2D31  add     bp,3 */
    BP = (uint16_t)(BP + 0x3);
L2D34:
    /* 2D34  shr     bp,2 */
    BP = (uint16_t)(BP >> 2);
L2D37: /* L2D37 */
    /* 2D37  mov     word ptr [bx+2],di */
    ww(pDS, BX + 0x2, DI);
L2D3A:
    /* 2D3A  mov     word ptr [bx+4],si */
    ww(pDS, BX + 0x4, SI);
L2D3D:
    /* 2D3D  mov     word ptr [bx+6],dx */
    ww(pDS, BX + 0x6, DX);
L2D40:
    /* 2D40  add     dx,bp */
    DX = (uint16_t)(DX + BP);
L2D42:
    /* 2D42  add     bx,8 */
    BX = (uint16_t)(BX + 0x8);
L2D45:
    /* 2D45  loop    L2D37 */
    if (--CX) goto L2D37;
L2D47:
    /* 2D47  or      word ptr [bx],8000h */
    ww(pDS, BX, logic16((uint16_t)(rw(pDS, BX) | 0x8000)));
L2D4B:
    /* 2D4B  mov     word ptr ds:[141Dh],bx */
    ww(pDS, 0x141D, BX);
L2D4F:
    /* 2D4F  mov     si,ax */
    SI = AX;
L2D51:
    /* 2D51  jmp     word ptr ds:[XFER_RTN] */
    return ASM_JMP(0x0090, rw(pDS, 0xDC4));
L2D55: /* L2D55 */
    /* 2D55  add     sp,4 */
    SP = add16(SP, 0x4, 0);
L2D58:
    /* 2D58  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2D59: /* L2D59 */
    /* 2D59  add     cx,ax */
    CX = (uint16_t)(CX + AX);
L2D5B:
    /* 2D5B  dec     cx */
    CX = (uint16_t)(CX - 1);
L2D5C:
    /* 2D5C  and     ax,0FFFCh */
    AX = (uint16_t)(AX & 0xFFFC);
L2D5F:
    /* 2D5F  and     cx,-4 */
    CX = (uint16_t)(CX & 0xFFFC);
L2D62:
    /* 2D62  add     cx,4 */
    CX = (uint16_t)(CX + 0x4);
L2D65:
    /* 2D65  sub     cx,ax */
    CX = (uint16_t)(CX - AX);
L2D67:
    /* 2D67  mov     si,cx */
    SI = CX;
L2D69:
    /* 2D69  shr     si,2 */
    SI = (uint16_t)(SI >> 2);
L2D6C:
    /* 2D6C  push    si */
    push16(SI);
L2D6D:
    /* 2D6D  cmp     byte ptr ds:[_ShowClip],0 */
    sub8(rb(pDS, 0xDC6), 0x0, 0);
L2D72:
    /* 2D72  je      L2DDD */
    if (ZF) goto L2DDD;
L2D74:
    /* 2D74  mov     si,word ptr ds:[WIN_TOP] */
    SI = rw(pDS, 0x3DF4);
L2D78:
    /* 2D78  mov     bp,bx */
    BP = BX;
L2D7A:
    /* 2D7A  sub     bp,dx */
    BP = (uint16_t)(BP - DX);
L2D7C:
    /* 2D7C  inc     bp */
    BP = (uint16_t)(BP + 1);
L2D7D:
    /* 2D7D  cmp     si,bp */
    sub16(SI, BP, 0);
L2D7F:
    /* 2D7F  jge     L2D84 */
    if (SF == OF) goto L2D84;
L2D81:
    /* 2D81  jmp     L2E20 */
    goto L2E20;
L2D84: /* L2D84 */
    /* 2D84  sub     si,bx */
    SI = sub16(SI, BX, 0);
L2D86:
    /* 2D86  jge     L2D9D */
    if (SF == OF) goto L2D9D;
L2D88:
    /* 2D88  add     dx,si */
    DX = (uint16_t)(DX + SI);
L2D8A:
    /* 2D8A  add     bx,si */
    BX = (uint16_t)(BX + SI);
L2D8C:
    /* 2D8C  push    ax */
    push16(AX);
L2D8D:
    /* 2D8D  push    dx */
    push16(DX);
L2D8E:
    /* 2D8E  mov     dx,cx */
    DX = CX;
L2D90:
    /* 2D90  shr     dx,2 */
    DX = (uint16_t)(DX >> 2);
L2D93:
    /* 2D93  mov     ax,si */
    AX = SI;
L2D95:
    /* 2D95  neg     ax */
    AX = (uint16_t)-AX;
L2D97:
    /* 2D97  mul     dx */
    mul16(DX);
L2D99:
    /* 2D99  add     di,ax */
    DI = (uint16_t)(DI + AX);
L2D9B:
    /* 2D9B  pop     dx */
    DX = pop16();
L2D9C:
    /* 2D9C  pop     ax */
    AX = pop16();
L2D9D: /* L2D9D */
    /* 2D9D  mov     si,bx */
    SI = BX;
L2D9F:
    /* 2D9F  sub     si,word ptr ds:[WIN_BOTTOM] */
    SI = sub16(SI, rw(pDS, 0x3DF8), 0);
L2DA3:
    /* 2DA3  jle     L2E20 */
    if (ZF || SF != OF) goto L2E20;
L2DA5:
    /* 2DA5  cmp     dx,si */
    sub16(DX, SI, 0);
L2DA7:
    /* 2DA7  jle     L2DAB */
    if (ZF || SF != OF) goto L2DAB;
L2DA9:
    /* 2DA9  mov     dx,si */
    DX = SI;
L2DAB: /* L2DAB */
    /* 2DAB  mov     si,word ptr ds:[WIN_RIGHT] */
    SI = rw(pDS, 0x3DF6);
L2DAF:
    /* 2DAF  sub     si,ax */
    SI = sub16(SI, AX, 0);
L2DB1:
    /* 2DB1  jl      L2E20 */
    if (SF != OF) goto L2E20;
L2DB3:
    /* 2DB3  inc     si */
    SI = (uint16_t)(SI + 1);
L2DB4:
    /* 2DB4  cmp     cx,si */
    sub16(CX, SI, 0);
L2DB6:
    /* 2DB6  jle     L2DBA */
    if (ZF || SF != OF) goto L2DBA;
L2DB8:
    /* 2DB8  mov     cx,si */
    CX = SI;
L2DBA: /* L2DBA */
    /* 2DBA  mov     si,word ptr ds:[WIN_LEFT] */
    SI = rw(pDS, 0x3DF2);
L2DBE:
    /* 2DBE  mov     bp,ax */
    BP = AX;
L2DC0:
    /* 2DC0  add     bp,cx */
    BP = (uint16_t)(BP + CX);
L2DC2:
    /* 2DC2  dec     bp */
    BP = (uint16_t)(BP - 1);
L2DC3:
    /* 2DC3  cmp     si,bp */
    sub16(SI, BP, 0);
L2DC5:
    /* 2DC5  jg      L2E20 */
    if (!ZF && SF == OF) goto L2E20;
L2DC7:
    /* 2DC7  sub     si,ax */
    SI = sub16(SI, AX, 0);
L2DC9:
    /* 2DC9  jle     L2DDD */
    if (ZF || SF != OF) goto L2DDD;
L2DCB:
    /* 2DCB  add     ax,si */
    AX = (uint16_t)(AX + SI);
L2DCD:
    /* 2DCD  sub     cx,si */
    CX = (uint16_t)(CX - SI);
L2DCF:
    /* 2DCF  mov     bp,si */
    BP = SI;
L2DD1:
    /* 2DD1  neg     bp */
    BP = (uint16_t)-BP;
L2DD3:
    /* 2DD3  and     bp,3 */
    BP = (uint16_t)(BP & 0x3);
L2DD6:
    /* 2DD6  add     si,bp */
    SI = (uint16_t)(SI + BP);
L2DD8:
    /* 2DD8  sar     si,2 */
    SI = (uint16_t)((int16_t)SI >> 2);
L2DDB:
    /* 2DDB  add     di,si */
    DI = (uint16_t)(DI + SI);
L2DDD: /* L2DDD */
    /* 2DDD  mov     bp,word ptr ds:[141Dh] */
    BP = rw(pDS, 0x141D);
L2DE1:
    /* 2DE1  and     word ptr [bp],7FFFh */
    ww(pSS, BP, (uint16_t)(rw(pSS, BP) & 0x7FFF));
L2DE6:
    /* 2DE6  add     cx,ax */
    CX = (uint16_t)(CX + AX);
L2DE8:
    /* 2DE8  dec     cx */
    CX = (uint16_t)(CX - 1);
L2DE9:
    /* 2DE9  neg     bx */
    BX = (uint16_t)-BX;
L2DEB:
    /* 2DEB  add     bx,0C7h */
    BX = (uint16_t)(BX + 0xC7);
L2DEF:
    /* 2DEF  shl     bx,3 */
    BX = (uint16_t)(BX << 3);
L2DF2:
    /* 2DF2  add     bx,0DCDh */
    BX = (uint16_t)(BX + 0xDCD);
L2DF6:
    /* 2DF6  mov     si,bx */
    SI = BX;
L2DF8:
    /* 2DF8  pop     bp */
    BP = pop16();
L2DF9:
    /* 2DF9  cmp     byte ptr ds:[0DCCh],0 */
    sub8(rb(pDS, 0xDCC), 0x0, 0);
L2DFE:
    /* 2DFE  je      L2E03 */
    if (ZF) goto L2E03;
L2E00:
    /* 2E00  mov     bp,50h */
    BP = 0x50;
L2E03: /* L2E03 */
    /* 2E03  mov     word ptr [bx+2],ax */
    ww(pDS, BX + 0x2, AX);
L2E06:
    /* 2E06  mov     word ptr [bx+4],cx */
    ww(pDS, BX + 0x4, CX);
L2E09:
    /* 2E09  mov     word ptr [bx+6],di */
    ww(pDS, BX + 0x6, DI);
L2E0C:
    /* 2E0C  add     di,bp */
    DI = (uint16_t)(DI + BP);
L2E0E:
    /* 2E0E  add     bx,8 */
    BX = (uint16_t)(BX + 0x8);
L2E11:
    /* 2E11  dec     dx */
    DX = dec16(DX);
L2E12:
    /* 2E12  jne     L2E03 */
    if (!ZF) goto L2E03;
L2E14:
    /* 2E14  or      word ptr [bx],8000h */
    ww(pDS, BX, logic16((uint16_t)(rw(pDS, BX) | 0x8000)));
L2E18:
    /* 2E18  mov     word ptr ds:[141Dh],bx */
    ww(pDS, 0x141D, BX);
L2E1C:
    /* 2E1C  jmp     word ptr ds:[XFER_RTN] */
    return ASM_JMP(0x0090, rw(pDS, 0xDC4));
L2E20: /* L2E20 */
    /* 2E20  add     sp,2 */
    SP = add16(SP, 0x2, 0);
L2E23:
    /* 2E23  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2E24: /* L2E24 */
    /* 2E24  add     cx,ax */
    CX = (uint16_t)(CX + AX);
L2E26:
    /* 2E26  dec     cx */
    CX = (uint16_t)(CX - 1);
L2E27:
    /* 2E27  and     ax,0FFFCh */
    AX = (uint16_t)(AX & 0xFFFC);
L2E2A:
    /* 2E2A  and     cx,-4 */
    CX = (uint16_t)(CX & 0xFFFC);
L2E2D:
    /* 2E2D  add     cx,4 */
    CX = (uint16_t)(CX + 0x4);
L2E30:
    /* 2E30  sub     cx,ax */
    CX = (uint16_t)(CX - AX);
L2E32:
    /* 2E32  mov     bp,cx */
    BP = CX;
L2E34:
    /* 2E34  shr     bp,2 */
    BP = (uint16_t)(BP >> 2);
L2E37:
    /* 2E37  mov     si,word ptr ds:[141Dh] */
    SI = rw(pDS, 0x141D);
L2E3B:
    /* 2E3B  and     word ptr [si],7FFFh */
    ww(pDS, SI, (uint16_t)(rw(pDS, SI) & 0x7FFF));
L2E3F:
    /* 2E3F  add     cx,ax */
    CX = (uint16_t)(CX + AX);
L2E41:
    /* 2E41  dec     cx */
    CX = (uint16_t)(CX - 1);
L2E42:
    /* 2E42  sub     bx,dx */
    BX = (uint16_t)(BX - DX);
L2E44:
    /* 2E44  inc     bx */
    BX = (uint16_t)(BX + 1);
L2E45:
    /* 2E45  mov     si,0DCDh */
    SI = 0xDCD;
L2E48:
    /* 2E48  push    dx */
    push16(DX);
L2E49: /* L2E49 */
    /* 2E49  mov     word ptr [si],bx */
    ww(pDS, SI, BX);
L2E4B:
    /* 2E4B  mov     word ptr [si+2],ax */
    ww(pDS, SI + 0x2, AX);
L2E4E:
    /* 2E4E  mov     word ptr [si+4],cx */
    ww(pDS, SI + 0x4, CX);
L2E51:
    /* 2E51  mov     word ptr [si+6],di */
    ww(pDS, SI + 0x6, DI);
L2E54:
    /* 2E54  inc     bx */
    BX = (uint16_t)(BX + 1);
L2E55:
    /* 2E55  sub     di,bp */
    DI = (uint16_t)(DI - BP);
L2E57:
    /* 2E57  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L2E5A:
    /* 2E5A  dec     dx */
    DX = dec16(DX);
L2E5B:
    /* 2E5B  jne     L2E49 */
    if (!ZF) goto L2E49;
L2E5D:
    /* 2E5D  or      word ptr [si],8000h */
    ww(pDS, SI, (uint16_t)(rw(pDS, SI) | 0x8000));
L2E61:
    /* 2E61  mov     word ptr ds:[141Dh],si */
    ww(pDS, 0x141D, SI);
L2E65:
    /* 2E65  mov     si,0DCDh */
    SI = 0xDCD;
L2E68:
    /* 2E68  call    word ptr ds:[XFER_RTN] */
    if ((c = asm_call(ASM_JMP(0x0090, rw(pDS, 0xDC4)), 0x2E6C)) != 0) return c;
L2E6C:
    /* 2E6C  pop     dx */
    DX = pop16();
L2E6D:
    /* 2E6D  mov     bx,0DCDh */
    BX = 0xDCD;
L2E70:
    /* 2E70  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L2E72: /* L2E72 */
    /* 2E72  mov     word ptr [bx],ax */
    ww(pDS, BX, AX);
L2E74:
    /* 2E74  inc     ax */
    AX = (uint16_t)(AX + 1);
L2E75:
    /* 2E75  add     bx,8 */
    BX = add16(BX, 0x8, 0);
L2E78:
    /* 2E78  dec     dx */
    DX = dec16(DX);
L2E79:
    /* 2E79  jne     L2E72 */
    if (!ZF) goto L2E72;
L2E7B:
    /* 2E7B  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2E7C:
    /* 2E7C  call    _seg003_3D42 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3D42), 0x2E7F)) != 0) return c;
L2E7F:
    /* 2E7F  mov     ax,cx */
    AX = CX;
L2E81:
    /* 2E81  shl     cx,1 */
    CX = (uint16_t)(CX << 1);
L2E83:
    /* 2E83  shl     cx,1 */
    CX = (uint16_t)(CX << 1);
L2E85:
    /* 2E85  mov     si,415Ch */
    SI = 0x415C;
L2E88:
    /* 2E88  mov     di,si */
    DI = SI;
L2E8A:
    /* 2E8A  add     di,cx */
    DI = (uint16_t)(DI + CX);
L2E8C:
    /* 2E8C  mov     cx,8 */
    CX = 0x8;
L2E8F:
    /* 2E8F  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L2E91:
    /* 2E91  mov     cx,ax */
    CX = AX;
L2E93:
    /* 2E93  mov     word ptr ds:[1B88h],2D40h */
    ww(pDS, 0x1B88, 0x2D40);
L2E99:
    /* 2E99  mov     ch,cl */
    CH = CL;
L2E9B:
    /* 2E9B  mov     cl,5 */
    CL = 0x5;
L2E9D:
    /* 2E9D  mov     si,4160h */
    SI = 0x4160;
L2EA0:
    /* 2EA0  mov     di,1424h */
    DI = 0x1424;
L2EA3:
    /* 2EA3  mov     word ptr ds:[19ECh],0 */
    ww(pDS, 0x19EC, 0x0);
L2EA9: /* L2EA9 */
    /* 2EA9  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L2EAB:
    /* 2EAB  mov     bx,word ptr [si+2] */
    BX = rw(pDS, SI + 0x2);
L2EAE:
    /* 2EAE  cmp     bx,word ptr [si+6] */
    sub16(BX, rw(pDS, SI + 0x6), 0);
L2EB1:
    /* 2EB1  je      L2F2C */
    if (ZF) goto L2F2C;
L2EB3:
    /* 2EB3  jg      L2EE4 */
    if (!ZF && SF == OF) goto L2EE4;
L2EB5:
    /* 2EB5  shl     ax,cl */
    AX = shl16(AX, CL);
L2EB7:
    /* 2EB7  mov     word ptr [di+4],ax */
    ww(pDS, DI + 0x4, AX);
L2EBA:
    /* 2EBA  mov     word ptr [di+6],bx */
    ww(pDS, DI + 0x6, BX);
L2EBD:
    /* 2EBD  mov     dx,word ptr [si+4] */
    DX = rw(pDS, SI + 0x4);
L2EC0:
    /* 2EC0  shl     dx,cl */
    DX = (uint16_t)(DX << (CL & 31));
L2EC2:
    /* 2EC2  mov     word ptr [di+2],dx */
    ww(pDS, DI + 0x2, DX);
L2EC5:
    /* 2EC5  mov     bp,word ptr [si+6] */
    BP = rw(pDS, SI + 0x6);
L2EC8:
    /* 2EC8  mov     word ptr [di],bp */
    ww(pDS, DI, BP);
L2ECA:
    /* 2ECA  sub     ax,dx */
    AX = sub16(AX, DX, 0);
L2ECC:
    /* 2ECC  je      L2ED8 */
    if (ZF) goto L2ED8;
L2ECE:
    /* 2ECE  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L2ECF:
    /* 2ECF  sub     bx,bp */
    BX = (uint16_t)(BX - BP);
L2ED1:
    /* 2ED1  neg     bx */
    BX = (uint16_t)-BX;
L2ED3:
    /* 2ED3  idiv    bx */
    if (asm_idiv16(BX) && (c = asm_divfault(0x0090, 0x2ED3, 2)) != 0) return c;
L2ED5:
    /* 2ED5  sub     word ptr [di+2],ax */
    ww(pDS, DI + 0x2, (uint16_t)(rw(pDS, DI + 0x2) - AX));
L2ED8: /* L2ED8 */
    /* 2ED8  mov     word ptr [di+8],ax */
    ww(pDS, DI + 0x8, AX);
L2EDB:
    /* 2EDB  cmp     bp,word ptr [si+0Ah] */
    sub16(BP, rw(pDS, SI + 0xA), 0);
L2EDE:
    /* 2EDE  jg      L2F12 */
    if (!ZF && SF == OF) goto L2F12;
L2EE0:
    /* 2EE0  dec     word ptr [di] */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) - 1));
L2EE2:
    /* 2EE2  jmp     short L2F12 */
    goto L2F12;
L2EE4: /* L2EE4 */
    /* 2EE4  shl     ax,cl */
    AX = shl16(AX, CL);
L2EE6:
    /* 2EE6  mov     word ptr [di+2],ax */
    ww(pDS, DI + 0x2, AX);
L2EE9:
    /* 2EE9  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L2EEB:
    /* 2EEB  mov     dx,word ptr [si+4] */
    DX = rw(pDS, SI + 0x4);
L2EEE:
    /* 2EEE  shl     dx,cl */
    DX = (uint16_t)(DX << (CL & 31));
L2EF0:
    /* 2EF0  mov     word ptr [di+4],dx */
    ww(pDS, DI + 0x4, DX);
L2EF3:
    /* 2EF3  mov     bp,word ptr [si+6] */
    BP = rw(pDS, SI + 0x6);
L2EF6:
    /* 2EF6  mov     word ptr [di+6],bp */
    ww(pDS, DI + 0x6, BP);
L2EF9:
    /* 2EF9  sub     dx,ax */
    DX = sub16(DX, AX, 0);
L2EFB:
    /* 2EFB  mov     ax,dx */
    AX = DX;
L2EFD:
    /* 2EFD  je      L2F07 */
    if (ZF) goto L2F07;
L2EFF:
    /* 2EFF  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L2F00:
    /* 2F00  sub     bx,bp */
    BX = (uint16_t)(BX - BP);
L2F02:
    /* 2F02  idiv    bx */
    if (asm_idiv16(BX) && (c = asm_divfault(0x0090, 0x2F02, 2)) != 0) return c;
L2F04:
    /* 2F04  sub     word ptr [di+2],ax */
    ww(pDS, DI + 0x2, (uint16_t)(rw(pDS, DI + 0x2) - AX));
L2F07: /* L2F07 */
    /* 2F07  mov     word ptr [di+8],ax */
    ww(pDS, DI + 0x8, AX);
L2F0A:
    /* 2F0A  cmp     bp,word ptr [si+0Ah] */
    sub16(BP, rw(pDS, SI + 0xA), 0);
L2F0D:
    /* 2F0D  jl      L2F12 */
    if (SF != OF) goto L2F12;
L2F0F:
    /* 2F0F  inc     word ptr [di+6] */
    ww(pDS, DI + 0x6, (uint16_t)(rw(pDS, DI + 0x6) + 1));
L2F12: /* L2F12 */
    /* 2F12  add     di,0Ch */
    DI = (uint16_t)(DI + 0xC);
L2F15:
    /* 2F15  inc     word ptr ds:[19ECh] */
    ww(pDS, 0x19EC, (uint16_t)(rw(pDS, 0x19EC) + 1));
L2F19:
    /* 2F19  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L2F1C:
    /* 2F1C  dec     ch */
    CH = dec8(CH);
L2F1E:
    /* 2F1E  jg      L2EA9 */
    if (!ZF && SF == OF) goto L2EA9;
L2F20:
    /* 2F20  jmp     L2FA4 */
    goto L2FA4;
L2F23: /* L2F23 */
    /* 2F23  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L2F26:
    /* 2F26  dec     ch */
    CH = dec8(CH);
L2F28:
    /* 2F28  jne     L2F2F */
    if (!ZF) goto L2F2F;
L2F2A:
    /* 2F2A  jmp     short L2FA4 */
    goto L2FA4;
L2F2C: /* L2F2C */
    /* 2F2C  mov     bp,word ptr [si-2] */
    BP = rw(pDS, SI + 0xFFFE);
L2F2F: /* L2F2F */
    /* 2F2F  mov     dx,bx */
    DX = BX;
L2F31:
    /* 2F31  sub     dx,word ptr [si+0Ah] */
    DX = sub16(DX, rw(pDS, SI + 0xA), 0);
L2F34:
    /* 2F34  je      L2F23 */
    if (ZF) goto L2F23;
L2F36:
    /* 2F36  sub     bp,bx */
    BP = (uint16_t)(BP - BX);
L2F38:
    /* 2F38  neg     bp */
    BP = (uint16_t)-BP;
L2F3A:
    /* 2F3A  xor     bp,dx */
    BP = logic16((uint16_t)(BP ^ DX));
L2F3C:
    /* 2F3C  js      L2F63 */
    if (SF) goto L2F63;
L2F3E:
    /* 2F3E  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2F40:
    /* 2F40  mov     word ptr [di+2],ax */
    ww(pDS, DI + 0x2, AX);
L2F43:
    /* 2F43  mov     word ptr [di+4],ax */
    ww(pDS, DI + 0x4, AX);
L2F46:
    /* 2F46  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L2F48:
    /* 2F48  mov     word ptr [di+6],bx */
    ww(pDS, DI + 0x6, BX);
L2F4B:
    /* 2F4B  mov     word ptr [di+8],0 */
    ww(pDS, DI + 0x8, 0x0);
L2F50:
    /* 2F50  add     di,0Ch */
    DI = (uint16_t)(DI + 0xC);
L2F53:
    /* 2F53  inc     word ptr ds:[19ECh] */
    ww(pDS, 0x19EC, (uint16_t)(rw(pDS, 0x19EC) + 1));
L2F57:
    /* 2F57  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L2F5A:
    /* 2F5A  dec     ch */
    CH = dec8(CH);
L2F5C:
    /* 2F5C  jle     L2F61 */
    if (ZF || SF != OF) goto L2F61;
L2F5E:
    /* 2F5E  jmp     L2EA9 */
    goto L2EA9;
L2F61: /* L2F61 */
    /* 2F61  jmp     short L2FA4 */
    goto L2FA4;
L2F63: /* L2F63 */
    /* 2F63  or      dx,dx */
    DX = logic16((uint16_t)(DX | DX));
L2F65:
    /* 2F65  js      L2F9A */
    if (SF) goto L2F9A;
L2F67:
    /* 2F67  shl     ax,cl */
    AX = shl16(AX, CL);
L2F69:
    /* 2F69  mov     word ptr [di+2],ax */
    ww(pDS, DI + 0x2, AX);
L2F6C:
    /* 2F6C  mov     word ptr [di+4],ax */
    ww(pDS, DI + 0x4, AX);
L2F6F:
    /* 2F6F  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L2F71:
    /* 2F71  mov     word ptr [di+6],bx */
    ww(pDS, DI + 0x6, BX);
L2F74:
    /* 2F74  mov     word ptr [di+8],0 */
    ww(pDS, DI + 0x8, 0x0);
L2F79:
    /* 2F79  mov     ax,word ptr [si+4] */
    AX = rw(pDS, SI + 0x4);
L2F7C:
    /* 2F7C  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L2F7E:
    /* 2F7E  mov     word ptr [di+0Eh],ax */
    ww(pDS, DI + 0xE, AX);
L2F81:
    /* 2F81  mov     word ptr [di+10h],ax */
    ww(pDS, DI + 0x10, AX);
L2F84:
    /* 2F84  mov     word ptr [di+0Ch],bx */
    ww(pDS, DI + 0xC, BX);
L2F87:
    /* 2F87  mov     word ptr [di+12h],bx */
    ww(pDS, DI + 0x12, BX);
L2F8A:
    /* 2F8A  mov     word ptr [di+14h],0 */
    ww(pDS, DI + 0x14, 0x0);
L2F8F:
    /* 2F8F  add     di,18h */
    DI = (uint16_t)(DI + 0x18);
L2F92:
    /* 2F92  inc     word ptr ds:[19ECh] */
    ww(pDS, 0x19EC, (uint16_t)(rw(pDS, 0x19EC) + 1));
L2F96:
    /* 2F96  inc     word ptr ds:[19ECh] */
    ww(pDS, 0x19EC, (uint16_t)(rw(pDS, 0x19EC) + 1));
L2F9A: /* L2F9A */
    /* 2F9A  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L2F9D:
    /* 2F9D  dec     ch */
    CH = dec8(CH);
L2F9F:
    /* 2F9F  jle     L2FA4 */
    if (ZF || SF != OF) goto L2FA4;
L2FA1:
    /* 2FA1  jmp     L2EA9 */
    goto L2EA9;
L2FA4: /* L2FA4 */
    /* 2FA4  mov     cx,word ptr ds:[19ECh] */
    CX = rw(pDS, 0x19EC);
L2FA8:
    /* 2FA8  cmp     cx,2 */
    sub16(CX, 0x2, 0);
L2FAB:
    /* 2FAB  jge     L2FAE */
    if (SF == OF) goto L2FAE;
L2FAD:
    /* 2FAD  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L2FAE: /* L2FAE */
    /* 2FAE  mov     si,1424h */
    SI = 0x1424;
L2FB1:
    /* 2FB1  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L2FB3:
    /* 2FB3  mov     bp,0Ch */
    BP = 0xC;
L2FB6:
    /* 2FB6  mov     dl,0FFh */
    DL = 0xFF;
L2FB8: /* L2FB8 */
    /* 2FB8  mov     bx,word ptr [si] */
    BX = rw(pDS, SI);
L2FBA:
    /* 2FBA  mov     byte ptr [bx+1B8Ch],dl */
    wb(pDS, BX + 0x1B8C, DL);
L2FBE:
    /* 2FBE  cmp     ax,bx */
    sub16(AX, BX, 0);
L2FC0:
    /* 2FC0  jg      L2FC4 */
    if (!ZF && SF == OF) goto L2FC4;
L2FC2:
    /* 2FC2  mov     ax,bx */
    AX = BX;
L2FC4: /* L2FC4 */
    /* 2FC4  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L2FC6:
    /* 2FC6  mov     di,si */
    DI = SI;
L2FC8:
    /* 2FC8  xchg    di,word ptr [bx+19F2h] */
    { uint16_t t_ = rw(pDS, BX + 0x19F2);
    ww(pDS, BX + 0x19F2, DI);
    DI = t_; }
L2FCC:
    /* 2FCC  mov     word ptr [si+0Ah],di */
    ww(pDS, SI + 0xA, DI);
L2FCF:
    /* 2FCF  mov     bx,word ptr [si+6] */
    BX = rw(pDS, SI + 0x6);
L2FD2:
    /* 2FD2  mov     byte ptr [bx+1B8Bh],dl */
    wb(pDS, BX + 0x1B8B, DL);
L2FD6:
    /* 2FD6  add     si,bp */
    SI = (uint16_t)(SI + BP);
L2FD8:
    /* 2FD8  loop    L2FB8 */
    if (--CX) goto L2FB8;
L2FDA:
    /* 2FDA  mov     word ptr ds:[1422h],ax */
    ww(pDS, 0x1422, AX);
L2FDD:
    /* 2FDD  mov     word ptr ds:[1B86h],ax */
    ww(pDS, 0x1B86, AX);
L2FE0:
    /* 2FE0  mov     word ptr ds:[19EEh],0 */
    ww(pDS, 0x19EE, 0x0);
L2FE6: /* L2FE6 */
    /* 2FE6  mov     bx,word ptr ds:[1B86h] */
    BX = rw(pDS, 0x1B86);
L2FEA:
    /* 2FEA  mov     di,19C4h */
    DI = 0x19C4;
L2FED:
    /* 2FED  mov     si,di */
    SI = DI;
L2FEF:
    /* 2FEF  mov     cx,word ptr ds:[19EEh] */
    CX = rw(pDS, 0x19EE);
L2FF3:
    /* 2FF3  jcxz    L3000 */
    if (!CX) goto L3000;
L2FF5: /* L2FF5 */
    /* 2FF5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L2FF6:
    /* 2FF6  mov     bp,ax */
    BP = AX;
L2FF8:
    /* 2FF8  cmp     bx,word ptr [bp+6] */
    sub16(BX, rw(pSS, BP + 0x6), 0);
L2FFB:
    /* 2FFB  jl      L2FFE */
    if (SF != OF) goto L2FFE;
L2FFD:
    /* 2FFD  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L2FFE: /* L2FFE */
    /* 2FFE  loop    L2FF5 */
    if (--CX) goto L2FF5;
L3000: /* L3000 */
    /* 3000  mov     byte ptr [bx+1B8Ch],cl */
    wb(pDS, BX + 0x1B8C, CL);
L3004:
    /* 3004  mov     si,bx */
    SI = BX;
L3006:
    /* 3006  shl     si,1 */
    SI = (uint16_t)(SI << 1);
L3008:
    /* 3008  mov     ax,word ptr [si+19F2h] */
    AX = rw(pDS, SI + 0x19F2);
L300C:
    /* 300C  mov     word ptr [si+19F2h],cx */
    ww(pDS, SI + 0x19F2, CX);
L3010:
    /* 3010  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L3012:
    /* 3012  je      L3028 */
    if (ZF) goto L3028;
L3014: /* L3014 */
    /* 3014  mov     si,ax */
    SI = AX;
L3016:
    /* 3016  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3017:
    /* 3017  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L301A:
    /* 301A  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L301C:
    /* 301C  je      L3028 */
    if (ZF) goto L3028;
L301E:
    /* 301E  mov     si,ax */
    SI = AX;
L3020:
    /* 3020  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3021:
    /* 3021  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L3024:
    /* 3024  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L3026:
    /* 3026  jne     L3014 */
    if (!ZF) goto L3014;
L3028: /* L3028 */
    /* 3028  mov     dx,di */
    DX = DI;
L302A:
    /* 302A  sub     dx,19C4h */
    DX = (uint16_t)(DX - 0x19C4);
L302E:
    /* 302E  shr     dx,1 */
    DX = (uint16_t)(DX >> 1);
L3030:
    /* 3030  mov     word ptr ds:[19EEh],dx */
    ww(pDS, 0x19EE, DX);
L3034:
    /* 3034  or      dx,dx */
    DX = logic16((uint16_t)(DX | DX));
L3036:
    /* 3036  jne     L303B */
    if (!ZF) goto L303B;
L3038:
    /* 3038  jmp     L320D */
    goto L320D;
L303B: /* L303B */
    /* 303B  cmp     dl,2 */
    sub8(DL, 0x2, 0);
L303E:
    /* 303E  je      L3043 */
    if (ZF) goto L3043;
L3040:
    /* 3040  jmp     L313E */
    goto L313E;
L3043: /* L3043 */
    /* 3043  mov     bx,word ptr ds:[19C4h] */
    BX = rw(pDS, 0x19C4);
L3047:
    /* 3047  mov     bp,word ptr ds:[19C6h] */
    BP = rw(pDS, 0x19C6);
L304B:
    /* 304B  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L304E:
    /* 304E  add     ax,word ptr [bx+8] */
    AX = (uint16_t)(AX + rw(pDS, BX + 0x8));
L3051:
    /* 3051  sub     ax,word ptr [bp+8] */
    AX = (uint16_t)(AX - rw(pSS, BP + 0x8));
L3054:
    /* 3054  cmp     ax,word ptr [bp+2] */
    sub16(AX, rw(pSS, BP + 0x2), 0);
L3057:
    /* 3057  jne     L305F */
    if (!ZF) goto L305F;
L3059:
    /* 3059  mov     ax,word ptr [bx+4] */
    AX = rw(pDS, BX + 0x4);
L305C:
    /* 305C  cmp     ax,word ptr [bp+4] */
    sub16(AX, rw(pSS, BP + 0x4), 0);
L305F: /* L305F */
    /* 305F  jl      L3063 */
    if (SF != OF) goto L3063;
L3061:
    /* 3061  xchg    bx,bp */
    { uint16_t t_ = BP;
    BP = BX;
    BX = t_; }
L3063: /* L3063 */
    /* 3063  mov     ax,word ptr [bx+4] */
    AX = rw(pDS, BX + 0x4);
L3066:
    /* 3066  cmp     ax,word ptr [bp+4] */
    sub16(AX, rw(pSS, BP + 0x4), 0);
L3069:
    /* 3069  jle     L306E */
    if (ZF || SF != OF) goto L306E;
L306B:
    /* 306B  jmp     L318E */
    goto L318E;
L306E: /* L306E */
    /* 306E  mov     di,word ptr ds:[1B88h] */
    DI = rw(pDS, 0x1B88);
L3072:
    /* 3072  mov     cl,5 */
    CL = 0x5;
L3074:
    /* 3074  mov     word ptr ds:[19C4h],bx */
    ww(pDS, 0x19C4, BX);
L3078:
    /* 3078  mov     word ptr ds:[19C6h],bp */
    ww(pDS, 0x19C6, BP);
L307C:
    /* 307C  mov     dx,word ptr [bx+2] */
    DX = rw(pDS, BX + 0x2);
L307F:
    /* 307F  mov     si,word ptr [bx+8] */
    SI = rw(pDS, BX + 0x8);
L3082:
    /* 3082  mov     ax,word ptr [bp+8] */
    AX = rw(pSS, BP + 0x8);
L3085:
    /* 3085  mov     bp,word ptr [bp+2] */
    BP = rw(pSS, BP + 0x2);
L3088:
    /* 3088  sub     dx,10h */
    DX = (uint16_t)(DX - 0x10);
L308B:
    /* 308B  add     bp,10h */
    BP = (uint16_t)(BP + 0x10);
L308E:
    /* 308E  mov     bx,word ptr ds:[1B86h] */
    BX = rw(pDS, 0x1B86);
L3092:
    /* 3092  mov     word ptr ds:[1420h],ax */
    ww(pDS, 0x1420, AX);
L3095:
    /* 3095  cmp     si,ax */
    sub16(SI, AX, 0);
L3097:
    /* 3097  jle     L30EB */
    if (ZF || SF != OF) goto L30EB;
L3099: /* L3099 */
    /* 3099  mov     ax,bx */
    AX = BX;
L309B:
    /* 309B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L309C:
    /* 309C  add     dx,si */
    DX = (uint16_t)(DX + SI);
L309E:
    /* 309E  add     bp,word ptr ds:[1420h] */
    BP = add16(BP, rw(pDS, 0x1420), 0);
L30A2:
    /* 30A2  mov     ax,dx */
    AX = DX;
L30A4:
    /* 30A4  sar     ax,cl */
    AX = sar16(AX, CL);
L30A6:
    /* 30A6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L30A7:
    /* 30A7  mov     ax,bp */
    AX = BP;
L30A9:
    /* 30A9  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L30AB:
    /* 30AB  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L30AC:
    /* 30AC  dec     bx */
    BX = (uint16_t)(BX - 1);
L30AD:
    /* 30AD  test    byte ptr [bx+1B8Ch],cl */
    logic8((uint8_t)(rb(pDS, BX + 0x1B8C) & CL));
L30B1:
    /* 30B1  jne     L311F */
    if (!ZF) goto L311F;
L30B3:
    /* 30B3  cmp     dx,bp */
    sub16(DX, BP, 0);
L30B5:
    /* 30B5  jg      L30D5 */
    if (!ZF && SF == OF) goto L30D5;
L30B7:
    /* 30B7  mov     ax,bx */
    AX = BX;
L30B9:
    /* 30B9  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L30BA:
    /* 30BA  add     dx,si */
    DX = (uint16_t)(DX + SI);
L30BC:
    /* 30BC  add     bp,word ptr ds:[1420h] */
    BP = add16(BP, rw(pDS, 0x1420), 0);
L30C0:
    /* 30C0  mov     ax,dx */
    AX = DX;
L30C2:
    /* 30C2  sar     ax,cl */
    AX = sar16(AX, CL);
L30C4:
    /* 30C4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L30C5:
    /* 30C5  mov     ax,bp */
    AX = BP;
L30C7:
    /* 30C7  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L30C9:
    /* 30C9  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L30CA:
    /* 30CA  dec     bx */
    BX = (uint16_t)(BX - 1);
L30CB:
    /* 30CB  test    byte ptr [bx+1B8Ch],cl */
    logic8((uint8_t)(rb(pDS, BX + 0x1B8C) & CL));
L30CF:
    /* 30CF  jne     L311F */
    if (!ZF) goto L311F;
L30D1:
    /* 30D1  cmp     dx,bp */
    sub16(DX, BP, 0);
L30D3:
    /* 30D3  jle     L3099 */
    if (ZF || SF != OF) goto L3099;
L30D5: /* L30D5 */
    /* 30D5  xchg    dx,bp */
    { uint16_t t_ = BP;
    BP = DX;
    DX = t_; }
L30D7:
    /* 30D7  xchg    si,word ptr ds:[1420h] */
    { uint16_t t_ = rw(pDS, 0x1420);
    ww(pDS, 0x1420, SI);
    SI = t_; }
L30DB:
    /* 30DB  mov     ax,word ptr ds:[19C4h] */
    AX = rw(pDS, 0x19C4);
L30DE:
    /* 30DE  xchg    ax,word ptr ds:[19C6h] */
    { uint16_t t_ = rw(pDS, 0x19C6);
    ww(pDS, 0x19C6, AX);
    AX = t_; }
L30E2:
    /* 30E2  mov     word ptr ds:[19C4h],ax */
    ww(pDS, 0x19C4, AX);
L30E5:
    /* 30E5  sub     dx,20h */
    DX = (uint16_t)(DX - 0x20);
L30E8:
    /* 30E8  add     bp,20h */
    BP = (uint16_t)(BP + 0x20);
L30EB: /* L30EB */
    /* 30EB  mov     ax,bx */
    AX = BX;
L30ED:
    /* 30ED  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L30EE:
    /* 30EE  add     dx,si */
    DX = (uint16_t)(DX + SI);
L30F0:
    /* 30F0  add     bp,word ptr ds:[1420h] */
    BP = add16(BP, rw(pDS, 0x1420), 0);
L30F4:
    /* 30F4  mov     ax,dx */
    AX = DX;
L30F6:
    /* 30F6  sar     ax,cl */
    AX = sar16(AX, CL);
L30F8:
    /* 30F8  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L30F9:
    /* 30F9  mov     ax,bp */
    AX = BP;
L30FB:
    /* 30FB  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L30FD:
    /* 30FD  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L30FE:
    /* 30FE  dec     bx */
    BX = (uint16_t)(BX - 1);
L30FF:
    /* 30FF  test    byte ptr [bx+1B8Ch],cl */
    logic8((uint8_t)(rb(pDS, BX + 0x1B8C) & CL));
L3103:
    /* 3103  jne     L311F */
    if (!ZF) goto L311F;
L3105:
    /* 3105  mov     ax,bx */
    AX = BX;
L3107:
    /* 3107  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3108:
    /* 3108  add     dx,si */
    DX = (uint16_t)(DX + SI);
L310A:
    /* 310A  add     bp,word ptr ds:[1420h] */
    BP = add16(BP, rw(pDS, 0x1420), 0);
L310E:
    /* 310E  mov     ax,dx */
    AX = DX;
L3110:
    /* 3110  sar     ax,cl */
    AX = sar16(AX, CL);
L3112:
    /* 3112  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3113:
    /* 3113  mov     ax,bp */
    AX = BP;
L3115:
    /* 3115  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L3117:
    /* 3117  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3118:
    /* 3118  dec     bx */
    BX = (uint16_t)(BX - 1);
L3119:
    /* 3119  test    byte ptr [bx+1B8Ch],cl */
    logic8((uint8_t)(rb(pDS, BX + 0x1B8C) & CL));
L311D:
    /* 311D  je      L30EB */
    if (ZF) goto L30EB;
L311F: /* L311F */
    /* 311F  add     dx,10h */
    DX = (uint16_t)(DX + 0x10);
L3122:
    /* 3122  sub     bp,10h */
    BP = (uint16_t)(BP - 0x10);
L3125:
    /* 3125  mov     word ptr ds:[1B88h],di */
    ww(pDS, 0x1B88, DI);
L3129:
    /* 3129  mov     word ptr ds:[1B86h],bx */
    ww(pDS, 0x1B86, BX);
L312D:
    /* 312D  mov     si,word ptr ds:[19C4h] */
    SI = rw(pDS, 0x19C4);
L3131:
    /* 3131  mov     word ptr [si+2],dx */
    ww(pDS, SI + 0x2, DX);
L3134:
    /* 3134  mov     si,word ptr ds:[19C6h] */
    SI = rw(pDS, 0x19C6);
L3138:
    /* 3138  mov     word ptr [si+2],bp */
    ww(pDS, SI + 0x2, BP);
L313B:
    /* 313B  jmp     L2FE6 */
    goto L2FE6;
L313E: /* L313E */
    /* 313E  test    dl,1 */
    logic8((uint8_t)(DL & 0x1));
L3141:
    /* 3141  je      L318E */
    if (ZF) goto L318E;
L3143:
    /* 3143  mov     di,word ptr ds:[1B88h] */
    DI = rw(pDS, 0x1B88);
L3147:
    /* 3147  mov     ax,bx */
    AX = BX;
L3149:
    /* 3149  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L314A:
    /* 314A  mov     cl,dl */
    CL = DL;
L314C:
    /* 314C  xor     ch,ch */
    CH = (uint8_t)(CH ^ CH);
L314E:
    /* 314E  mov     dx,8000h */
    DX = 0x8000;
L3151:
    /* 3151  mov     bp,7FFFh */
    BP = 0x7FFF;
L3154:
    /* 3154  mov     si,19C4h */
    SI = 0x19C4;
L3157: /* L3157 */
    /* 3157  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3158:
    /* 3158  mov     bx,ax */
    BX = AX;
L315A:
    /* 315A  mov     ax,word ptr [bx+8] */
    AX = rw(pDS, BX + 0x8);
L315D:
    /* 315D  add     ax,word ptr [bx+2] */
    AX = (uint16_t)(AX + rw(pDS, BX + 0x2));
L3160:
    /* 3160  mov     word ptr [bx+2],ax */
    ww(pDS, BX + 0x2, AX);
L3163:
    /* 3163  cmp     dx,ax */
    sub16(DX, AX, 0);
L3165:
    /* 3165  jg      L3169 */
    if (!ZF && SF == OF) goto L3169;
L3167:
    /* 3167  mov     dx,ax */
    DX = AX;
L3169: /* L3169 */
    /* 3169  cmp     bp,ax */
    sub16(BP, AX, 0);
L316B:
    /* 316B  jl      L316F */
    if (SF != OF) goto L316F;
L316D:
    /* 316D  mov     bp,ax */
    BP = AX;
L316F: /* L316F */
    /* 316F  loop    L3157 */
    if (--CX) goto L3157;
L3171:
    /* 3171  mov     cl,5 */
    CL = 0x5;
L3173:
    /* 3173  mov     ax,bp */
    AX = BP;
L3175:
    /* 3175  sub     ax,10h */
    AX = sub16(AX, 0x10, 0);
L3178:
    /* 3178  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L317A:
    /* 317A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L317B:
    /* 317B  mov     ax,dx */
    AX = DX;
L317D:
    /* 317D  add     ax,10h */
    AX = add16(AX, 0x10, 0);
L3180:
    /* 3180  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L3182:
    /* 3182  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3183:
    /* 3183  mov     word ptr ds:[1B88h],di */
    ww(pDS, 0x1B88, DI);
L3187:
    /* 3187  dec     word ptr ds:[1B86h] */
    ww(pDS, 0x1B86, (uint16_t)(rw(pDS, 0x1B86) - 1));
L318B:
    /* 318B  jmp     L2FE6 */
    goto L2FE6;
L318E: /* L318E */
    /* 318E  mov     dh,dl */
    DH = DL;
L3190:
    /* 3190  dec     dh */
    DH = (uint8_t)(DH - 1);
L3192:
    /* 3192  xor     ch,ch */
    CH = (uint8_t)(CH ^ CH);
L3194: /* L3194 */
    /* 3194  mov     cl,dh */
    CL = DH;
L3196:
    /* 3196  mov     si,19C4h */
    SI = 0x19C4;
L3199:
    /* 3199  xor     bl,bl */
    BL = (uint8_t)(BL ^ BL);
L319B: /* L319B */
    /* 319B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L319C:
    /* 319C  mov     di,ax */
    DI = AX;
L319E:
    /* 319E  mov     bp,word ptr [si] */
    BP = rw(pDS, SI);
L31A0:
    /* 31A0  mov     ax,word ptr [di+2] */
    AX = rw(pDS, DI + 0x2);
L31A3:
    /* 31A3  add     ax,word ptr [di+8] */
    AX = (uint16_t)(AX + rw(pDS, DI + 0x8));
L31A6:
    /* 31A6  sub     ax,word ptr [bp+8] */
    AX = (uint16_t)(AX - rw(pSS, BP + 0x8));
L31A9:
    /* 31A9  cmp     ax,word ptr [bp+2] */
    sub16(AX, rw(pSS, BP + 0x2), 0);
L31AC:
    /* 31AC  jl      L31B5 */
    if (SF != OF) goto L31B5;
L31AE:
    /* 31AE  inc     bl */
    BL = (uint8_t)(BL + 1);
L31B0:
    /* 31B0  mov     word ptr [si-2],bp */
    ww(pDS, SI + 0xFFFE, BP);
L31B3:
    /* 31B3  mov     word ptr [si],di */
    ww(pDS, SI, DI);
L31B5: /* L31B5 */
    /* 31B5  loop    L319B */
    if (--CX) goto L319B;
L31B7:
    /* 31B7  or      bl,bl */
    BL = logic8((uint8_t)(BL | BL));
L31B9:
    /* 31B9  je      L31BF */
    if (ZF) goto L31BF;
L31BB:
    /* 31BB  dec     dh */
    DH = dec8(DH);
L31BD:
    /* 31BD  jne     L3194 */
    if (!ZF) goto L3194;
L31BF: /* L31BF */
    /* 31BF  mov     di,word ptr ds:[1B88h] */
    DI = rw(pDS, 0x1B88);
L31C3:
    /* 31C3  mov     si,19C4h */
    SI = 0x19C4;
L31C6:
    /* 31C6  mov     bp,word ptr ds:[1B86h] */
    BP = rw(pDS, 0x1B86);
L31CA:
    /* 31CA  mov     dh,dl */
    DH = DL;
L31CC:
    /* 31CC  shr     dh,1 */
    DH = (uint8_t)(DH >> 1);
L31CE:
    /* 31CE  mov     cl,5 */
    CL = 0x5;
L31D0: /* L31D0 */
    /* 31D0  mov     ax,bp */
    AX = BP;
L31D2:
    /* 31D2  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L31D3:
    /* 31D3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L31D4:
    /* 31D4  mov     bx,ax */
    BX = AX;
L31D6:
    /* 31D6  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L31D9:
    /* 31D9  add     ax,word ptr [bx+8] */
    AX = (uint16_t)(AX + rw(pDS, BX + 0x8));
L31DC:
    /* 31DC  mov     word ptr [bx+2],ax */
    ww(pDS, BX + 0x2, AX);
L31DF:
    /* 31DF  sub     ax,10h */
    AX = sub16(AX, 0x10, 0);
L31E2:
    /* 31E2  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L31E4:
    /* 31E4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L31E5:
    /* 31E5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L31E6:
    /* 31E6  mov     bx,ax */
    BX = AX;
L31E8:
    /* 31E8  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L31EB:
    /* 31EB  add     ax,word ptr [bx+8] */
    AX = (uint16_t)(AX + rw(pDS, BX + 0x8));
L31EE:
    /* 31EE  mov     word ptr [bx+2],ax */
    ww(pDS, BX + 0x2, AX);
L31F1:
    /* 31F1  add     ax,10h */
    AX = add16(AX, 0x10, 0);
L31F4:
    /* 31F4  sar     ax,cl */
    AX = (uint16_t)((int16_t)AX >> ((CL & 31) >= 16 ? 15 : (CL & 31)));
L31F6:
    /* 31F6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L31F7:
    /* 31F7  dec     dh */
    DH = dec8(DH);
L31F9:
    /* 31F9  jne     L31D0 */
    if (!ZF) goto L31D0;
L31FB:
    /* 31FB  mov     word ptr ds:[1B88h],di */
    ww(pDS, 0x1B88, DI);
L31FF:
    /* 31FF  dec     bp */
    BP = (uint16_t)(BP - 1);
L3200:
    /* 3200  mov     word ptr ds:[1B86h],bp */
    ww(pDS, 0x1B86, BP);
L3204:
    /* 3204  test    byte ptr [bp+1B8Ch],cl */
    logic8((uint8_t)(rb(pSS, BP + 0x1B8C) & CL));
L3208:
    /* 3208  je      L318E */
    if (ZF) goto L318E;
L320A:
    /* 320A  jmp     L2FE6 */
    goto L2FE6;
L320D: /* L320D */
    /* 320D  mov     si,2D40h */
    SI = 0x2D40;
L3210:
    /* 3210  mov     bx,word ptr ds:[1B88h] */
    BX = rw(pDS, 0x1B88);
L3214:
    /* 3214  mov     word ptr [bx],0FFFFh */
    ww(pDS, BX, 0xFFFF);
L3218:
    /* 3218  jmp     word ptr ds:[SPAN_WRITER] */
    return ASM_JMP(0x0090, rw(pDS, 0x4110));
L321C: /* _seg003_321C */
    /* 321C  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_321D  (+321D)
       init_graphics: needs a VGA (_33FB, else returns with carry set). Records the CPU type from
       FD72:0110, sets mode X (SetVideoMode), saves the CRTC maximum scan line, builds the edge mask
       tables, and falls into the mode set. _3249 sets the mode number AX first; _324C applies 3DE8:
       copies the mode's size to 3DEA and to FD72:0AA8, sets the scan doubling (the saved value for
       mode 0, 1 line per row otherwise), sets the window to the whole screen, works out the row
       length and the two pages and the video memory left after them (4104, 4108), builds Ytab,
       initialises the pens, flips to page 0, and builds the text masks (GRLIBI's _4462). */
L321D: /* _seg003_321D */
    /* 321D  push    ds */
    push16(asm_ds);
L321E:
    /* 321E  mov     ax,seg seg063 */
    AX = (uint16_t)(0x5624 + PORT_LOAD_SEG);
L3221:
    /* 3221  mov     ds,ax */
    SET_DS(AX);
L3223:
    /* 3223  mov     ax,word ptr ds:[110h] */
    AX = rw(pDS, 0x110);
L3226:
    /* 3226  pop     ds */
    SET_DS(pop16());
L3227:
    /* 3227  mov     word ptr ds:[36A0h],ax */
    ww(pDS, 0x36A0, AX);
L322A:
    /* 322A  call    _seg003_33FB */
    if ((c = asm_call(ASM_JMP(0x0090, 0x33FB), 0x322D)) != 0) return c;
L322D:
    /* 322D  jae     L3230 */
    if (!CF) goto L3230;
L322F:
    /* 322F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3230: /* L3230 */
    /* 3230  mov     word ptr ds:[36A2h],0FFFFh */
    ww(pDS, 0x36A2, 0xFFFF);
L3236:
    /* 3236  call    _seg003_341C */
    if ((c = asm_call(ASM_JMP(0x0090, 0x341C), 0x3239)) != 0) return c;
L3239:
    /* 3239  mov     dx,CRTC_INDEX */
    DX = 0x3D4;
L323C:
    /* 323C  mov     al,9 */
    AL = 0x9;
L323E:
    /* 323E  out     dx,al */
    asm_out8(DX, AL);
L323F:
    /* 323F  inc     dx */
    DX = (uint16_t)(DX + 1);
L3240:
    /* 3240  in      al,dx */
    AL = asm_in8(DX);
L3241:
    /* 3241  mov     byte ptr ds:[2D33h],al */
    wb(pDS, 0x2D33, AL);
L3244:
    /* 3244  call    _seg003_33C5 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x33C5), 0x3247)) != 0) return c;
L3247:
    /* 3247  jmp     short _seg003_324C */
    goto L324C;
L3249: /* _seg003_3249 */
    /* 3249  mov     word ptr ds:[3DE8h],ax */
    ww(pDS, 0x3DE8, AX);
L324C: /* _seg003_324C */
    /* 324C  mov     bx,word ptr ds:[3DE8h] */
    BX = rw(pDS, 0x3DE8);
L3250:
    /* 3250  cmp     bx,word ptr ds:[2CDAh] */
    sub16(BX, rw(pDS, 0x2CDA), 0);
L3254:
    /* 3254  jne     L3257 */
    if (!ZF) goto L3257;
L3256:
    /* 3256  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3257: /* L3257 */
    /* 3257  mov     word ptr ds:[2CDAh],bx */
    ww(pDS, 0x2CDA, BX);
L325B:
    /* 325B  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L325D:
    /* 325D  mov     ax,bx */
    AX = BX;
L325F:
    /* 325F  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3261:
    /* 3261  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L3263:
    /* 3263  lea     si,[bx+2CDCh] */
    SI = (uint16_t)(BX + 0x2CDC);
L3267:
    /* 3267  mov     di,3DEAh */
    DI = 0x3DEA;
L326A:
    /* 326A  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L326B:
    /* 326B  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L326C:
    /* 326C  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L326D:
    /* 326D  mov     si,3DE8h */
    SI = 0x3DE8;
L3270:
    /* 3270  push    es */
    push16(asm_es);
L3271:
    /* 3271  mov     bx,seg seg063 */
    BX = (uint16_t)(0x5624 + PORT_LOAD_SEG);
L3274:
    /* 3274  mov     es,bx */
    SET_ES(BX);
L3276:
    /* 3276  mov     di,0AA8h */
    DI = 0xAA8;
L3279:
    /* 3279  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L327A:
    /* 327A  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L327B:
    /* 327B  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L327C:
    /* 327C  pop     es */
    SET_ES(pop16());
L327D:
    /* 327D  test    ax,ax */
    logic16((uint16_t)(AX & AX));
L327F:
    /* 327F  jne     L328E */
    if (!ZF) goto L328E;
L3281:
    /* 3281  mov     dx,CRTC_INDEX */
    DX = 0x3D4;
L3284:
    /* 3284  mov     al,9 */
    AL = 0x9;
L3286:
    /* 3286  out     dx,al */
    asm_out8(DX, AL);
L3287:
    /* 3287  inc     dx */
    DX = (uint16_t)(DX + 1);
L3288:
    /* 3288  mov     al,byte ptr ds:[2D33h] */
    AL = rb(pDS, 0x2D33);
L328B:
    /* 328B  out     dx,al */
    asm_out8(DX, AL);
L328C:
    /* 328C  jmp     short L3299 */
    goto L3299;
L328E: /* L328E */
    /* 328E  mov     dx,CRTC_INDEX */
    DX = 0x3D4;
L3291:
    /* 3291  mov     al,9 */
    AL = 0x9;
L3293:
    /* 3293  out     dx,al */
    asm_out8(DX, AL);
L3294:
    /* 3294  inc     dx */
    DX = (uint16_t)(DX + 1);
L3295:
    /* 3295  in      al,dx */
    AL = asm_in8(DX);
L3296:
    /* 3296  and     al,0E0h */
    AL = (uint8_t)(AL & 0xE0);
L3298:
    /* 3298  out     dx,al */
    asm_out8(DX, AL);
L3299: /* L3299 */
    /* 3299  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L329B:
    /* 329B  mov     word ptr ds:[WIN_LEFT],ax */
    ww(pDS, 0x3DF2, AX);
L329E:
    /* 329E  mov     word ptr ds:[WIN_BOTTOM],ax */
    ww(pDS, 0x3DF8, AX);
L32A1:
    /* 32A1  mov     ax,word ptr ds:[SCR_LASTX] */
    AX = rw(pDS, 0x3DEA);
L32A4:
    /* 32A4  mov     word ptr ds:[WIN_RIGHT],ax */
    ww(pDS, 0x3DF6, AX);
L32A7:
    /* 32A7  mov     ax,word ptr ds:[SCR_LASTY] */
    AX = rw(pDS, 0x3DEC);
L32AA:
    /* 32AA  mov     word ptr ds:[WIN_TOP],ax */
    ww(pDS, 0x3DF4, AX);
L32AD:
    /* 32AD  mov     si,3DF2h */
    SI = 0x3DF2;
L32B0:
    /* 32B0  call    _seg003_3A68 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3A68), 0x32B3)) != 0) return c;
L32B3:
    /* 32B3  mov     ax,word ptr ds:[SCR_LASTX] */
    AX = rw(pDS, 0x3DEA);
L32B6:
    /* 32B6  inc     ax */
    AX = (uint16_t)(AX + 1);
L32B7:
    /* 32B7  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L32B9:
    /* 32B9  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L32BB:
    /* 32BB  mov     word ptr ds:[ROW_BYTES],ax */
    ww(pDS, 0x36A6, AX);
L32BE:
    /* 32BE  mov     ax,word ptr ds:[SCR_LASTY] */
    AX = rw(pDS, 0x3DEC);
L32C1:
    /* 32C1  inc     ax */
    AX = (uint16_t)(AX + 1);
L32C2:
    /* 32C2  mul     word ptr ds:[ROW_BYTES] */
    mul16(rw(pDS, 0x36A6));
L32C6:
    /* 32C6  mov     bx,ax */
    BX = AX;
L32C8:
    /* 32C8  add     ax,0FFh */
    AX = (uint16_t)(AX + 0xFF);
L32CB:
    /* 32CB  sub     al,al */
    AL = (uint8_t)(AL - AL);
L32CD:
    /* 32CD  mov     word ptr ds:[2CD6h],ax */
    ww(pDS, 0x2CD6, AX);
L32D0:
    /* 32D0  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L32D2:
    /* 32D2  mov     word ptr ds:[2CD8h],ax */
    ww(pDS, 0x2CD8, AX);
L32D5:
    /* 32D5  add     ax,word ptr ds:[ROW_BYTES] */
    AX = (uint16_t)(AX + rw(pDS, 0x36A6));
L32D9:
    /* 32D9  mov     word ptr ds:[4104h],ax */
    ww(pDS, 0x4104, AX);
L32DC:
    /* 32DC  mov     word ptr ds:[4108h],ax */
    ww(pDS, 0x4108, AX);
L32DF:
    /* 32DF  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L32E1:
    /* 32E1  call    _seg003_3386 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3386), 0x32E4)) != 0) return c;
L32E4:
    /* 32E4  call    _seg003_3A3F */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3A3F), 0x32E7)) != 0) return c;
L32E7:
    /* 32E7  call    _seg003_3357 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3357), 0x32EA)) != 0) return c;
L32EA:
    /* 32EA  call    _seg003_4C74 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x4C74), 0x32ED)) != 0) return c;
L32ED:
    /* 32ED  mov     ax,0 */
    AX = 0x0;
L32F0:
    /* 32F0  call    _seg003_4462 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x4462), 0x32F3)) != 0) return c;
L32F3:
    /* 32F3  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_32F4  (+32F4)
       _32F4: when 3DFC is set, clear it, copy a palette (183h words) from SI to 3DFE and load it.
       _330D: wait for vertical retrace and write 256 colours from SI to the DAC (_3325). */
L32F4: /* _seg003_32F4 */
    /* 32F4  cmp     word ptr ds:[3DFCh],0 */
    sub16(rw(pDS, 0x3DFC), 0x0, 0);
L32F9:
    /* 32F9  jne     L32FC */
    if (!ZF) goto L32FC;
L32FB:
    /* 32FB  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L32FC: /* L32FC */
    /* 32FC  mov     word ptr ds:[3DFCh],0 */
    ww(pDS, 0x3DFC, 0x0);
L3302:
    /* 3302  mov     di,3DFEh */
    DI = 0x3DFE;
L3305:
    /* 3305  mov     cx,183h */
    CX = 0x183;
L3308:
    /* 3308  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L330A:
    /* 330A  mov     si,3DFEh */
    SI = 0x3DFE;
L330D: /* _seg003_330D */
    /* 330D  cli */
    ;
L330E:
    /* 330E  mov     dx,INPUT_STATUS */
    DX = 0x3DA;
L3311: /* L3311 */
    /* 3311  in      al,dx */
    AL = asm_in8(DX);
L3312:
    /* 3312  and     al,8 */
    AL = logic8((uint8_t)(AL & 0x8));
L3314:
    /* 3314  je      L3311 */
    if (ZF) goto L3311;
L3316:
    /* 3316  mov     al,0 */
    AL = 0x0;
L3318:
    /* 3318  mov     dx,DAC_WRITE_INDEX */
    DX = 0x3C8;
L331B:
    /* 331B  out     dx,al */
    asm_out8(DX, AL);
L331C:
    /* 331C  inc     dx */
    DX = (uint16_t)(DX + 1);
L331D:
    /* 331D  mov     cx,100h */
    CX = 0x100;
L3320:
    /* 3320  call    _seg003_3325 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3325), 0x3323)) != 0) return c;
L3323:
    /* 3323  sti */
    ;
L3324:
    /* 3324  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_3325  (+3325)
       _3325: write CX colours from SI to the DAC data port (DX = 3C9h), with rep outsb on a 186 or
       later (36A0 at least 0BAh), else with single out instructions, since the 8086 has no outsb. */
L3325: /* _seg003_3325 */
    /* 3325  cmp     word ptr ds:[36A0h],0BAh */
    sub16(rw(pDS, 0x36A0), 0xBA, 0);
L332B:
    /* 332B  jl      L3336 */
    if (SF != OF) goto L3336;
L332D:
    /* 332D  mov     ax,cx */
    AX = CX;
L332F:
    /* 332F  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L3331:
    /* 3331  add     cx,ax */
    CX = add16(CX, AX, 0);
L3333:
    /* 3333  rep outsb */
    while (CX) { asm_out8(DX, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); CX--; }
L3335:
    /* 3335  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3336: /* L3336 */
    /* 3336  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3337:
    /* 3337  out     dx,al */
    asm_out8(DX, AL);
L3338:
    /* 3338  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3339:
    /* 3339  out     dx,al */
    asm_out8(DX, AL);
L333A:
    /* 333A  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L333B:
    /* 333B  out     dx,al */
    asm_out8(DX, AL);
L333C:
    /* 333C  loop    L3336 */
    if (--CX) goto L3336;
L333E:
    /* 333E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_333F  (+333F)
       _333F: set the CRTC display start (registers 0Ch, 0Dh) to the drawing page, Ytab of the
       highest row. */
L333F: /* _seg003_333F */
    /* 333F  mov     bx,word ptr ds:[SCR_LASTY] */
    BX = rw(pDS, 0x3DEC);
L3343:
    /* 3343  shl     bx,1 */
    BX = shl16(BX, 1);
L3345:
    /* 3345  mov     ax,word ptr [bx+YTAB] */
    AX = rw(pDS, BX + 0x36AA);
L3349:
    /* 3349  mov     bx,ax */
    BX = AX;
L334B:
    /* 334B  mov     dx,CRTC_INDEX */
    DX = 0x3D4;
L334E:
    /* 334E  mov     al,0Ch */
    AL = 0xC;
L3350:
    /* 3350  out     dx,ax */
    asm_out16(DX, AX);
L3351:
    /* 3351  mov     ah,bl */
    AH = BL;
L3353:
    /* 3353  inc     al */
    AL = inc8(AL);
L3355:
    /* 3355  out     dx,ax */
    asm_out16(DX, AX);
L3356:
    /* 3356  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_3357  (+3357)
       grPageFlip: display the drawing page (_333F), then fall into grSoftPageFlip (_335A): keep the
       current row table as the displayed page's (383E), work out the other page's start and the
       offset between them (410A), and rebuild Ytab for it, so drawing goes to the page not on
       screen. */
L3357: /* _seg003_3357 */
    /* 3357  call    _seg003_333F */
    if ((c = asm_call(ASM_JMP(0x0090, 0x333F), 0x335A)) != 0) return c;
L335A: /* _seg003_335A */
    /* 335A  mov     si,36AAh */
    SI = 0x36AA;
L335D:
    /* 335D  mov     di,383Eh */
    DI = 0x383E;
L3360:
    /* 3360  mov     cx,word ptr ds:[SCR_LASTY] */
    CX = rw(pDS, 0x3DEC);
L3364:
    /* 3364  inc     cx */
    CX = (uint16_t)(CX + 1);
L3365:
    /* 3365  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L3367:
    /* 3367  mov     ax,word ptr ds:[2CD6h] */
    AX = rw(pDS, 0x2CD6);
L336A:
    /* 336A  mov     bx,word ptr ds:[SCR_LASTY] */
    BX = rw(pDS, 0x3DEC);
L336E:
    /* 336E  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3370:
    /* 3370  sub     ax,word ptr [bx+YTAB] */
    AX = (uint16_t)(AX - rw(pDS, BX + 0x36AA));
L3374:
    /* 3374  mov     bx,ax */
    BX = AX;
L3376:
    /* 3376  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3378:
    /* 3378  neg     bx */
    BX = (uint16_t)-BX;
L337A:
    /* 337A  add     bx,word ptr ds:[2CD6h] */
    BX = (uint16_t)(BX + rw(pDS, 0x2CD6));
L337E:
    /* 337E  mov     word ptr ds:[PAGE_DIFF],bx */
    ww(pDS, 0x410A, BX);
L3382:
    /* 3382  call    _seg003_3386 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3386), 0x3385)) != 0) return c;
L3385:
    /* 3385  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_3386  (+3386)
       _3386: build Ytab from AX: the highest row (3DEC) gets AX and each lower row 36A6 more (ten
       rows a pass). Marks 2D1A as -1. */
L3386: /* _seg003_3386 */
    /* 3386  mov     di,word ptr ds:[SCR_LASTY] */
    DI = rw(pDS, 0x3DEC);
L338A:
    /* 338A  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L338C:
    /* 338C  add     di,36AAh */
    DI = (uint16_t)(DI + 0x36AA);
L3390:
    /* 3390  mov     bx,word ptr ds:[ROW_BYTES] */
    BX = rw(pDS, 0x36A6);
L3394:
    /* 3394  mov     cx,word ptr ds:[SCR_LASTY] */
    CX = rw(pDS, 0x3DEC);
L3398:
    /* 3398  inc     cx */
    CX = (uint16_t)(CX + 1);
L3399:
    /* 3399  std */
    DF = 1;
L339A: /* L339A */
    /* 339A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L339B:
    /* 339B  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L339D:
    /* 339D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L339E:
    /* 339E  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L33A0:
    /* 33A0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L33A1:
    /* 33A1  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L33A3:
    /* 33A3  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L33A4:
    /* 33A4  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L33A6:
    /* 33A6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L33A7:
    /* 33A7  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L33A9:
    /* 33A9  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L33AA:
    /* 33AA  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L33AC:
    /* 33AC  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L33AD:
    /* 33AD  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L33AF:
    /* 33AF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L33B0:
    /* 33B0  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L33B2:
    /* 33B2  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L33B3:
    /* 33B3  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L33B5:
    /* 33B5  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L33B6:
    /* 33B6  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L33B8:
    /* 33B8  sub     cx,0Ah */
    CX = sub16(CX, 0xA, 0);
L33BB:
    /* 33BB  ja      L339A */
    if (!CF && !ZF) goto L339A;
L33BD:
    /* 33BD  cld */
    DF = 0;
L33BE:
    /* 33BE  mov     word ptr ds:[2D1Ah],0FFFFh */
    ww(pDS, 0x2D1A, 0xFFFF);
L33C4:
    /* 33C4  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_33C5  (+33C5)
       _33C5: build the edge plane mask tables: 3B62 from the four bytes at 2D2A and 3CA6 from those
       at 2D2F, each repeated 80 times (_33D4), so 3B62[x] and 3CA6[x] are the planes at and right
       of, and at and left of, pixel x within its group of four. */
L33C5: /* _seg003_33C5 */
    /* 33C5  mov     si,2D2Ah */
    SI = 0x2D2A;
L33C8:
    /* 33C8  mov     di,3B62h */
    DI = 0x3B62;
L33CB:
    /* 33CB  call    _seg003_33D4 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x33D4), 0x33CE)) != 0) return c;
L33CE:
    /* 33CE  mov     si,2D2Fh */
    SI = 0x2D2F;
L33D1:
    /* 33D1  mov     di,3CA6h */
    DI = 0x3CA6;

    /* seg003_33D4  (+33D4) */
L33D4: /* _seg003_33D4 */
    /* 33D4  mov     cx,50h */
    CX = 0x50;
L33D7: /* L33D7 */
    /* 33D7  push    cx */
    push16(CX);
L33D8:
    /* 33D8  push    si */
    push16(SI);
L33D9:
    /* 33D9  mov     cx,4 */
    CX = 0x4;
L33DC:
    /* 33DC  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L33DE:
    /* 33DE  pop     si */
    SI = pop16();
L33DF:
    /* 33DF  pop     cx */
    CX = pop16();
L33E0:
    /* 33E0  loop    L33D7 */
    if (--CX) goto L33D7;
L33E2:
    /* 33E2  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_33E3  (+33E3)
       _33E3: set the linear buffer _2C11 draws into (GRCORE's _58E0): DX its segment (39CE), and a
       row table at 39D0 for CX rows of stride BX starting at AX, highest row first. */
L33E3: /* _seg003_33E3 */
    /* 33E3  mov     word ptr ds:[39CEh],dx */
    ww(pDS, 0x39CE, DX);
L33E7:
    /* 33E7  mov     di,39D0h */
    DI = 0x39D0;
L33EA:
    /* 33EA  mov     dx,cx */
    DX = CX;
L33EC:
    /* 33EC  dec     dx */
    DX = (uint16_t)(DX - 1);
L33ED:
    /* 33ED  shl     dx,1 */
    DX = (uint16_t)(DX << 1);
L33EF:
    /* 33EF  shl     dx,1 */
    DX = (uint16_t)(DX << 1);
L33F1:
    /* 33F1  add     di,dx */
    DI = (uint16_t)(DI + DX);
L33F3:
    /* 33F3  std */
    DF = 1;
L33F4: /* L33F4 */
    /* 33F4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L33F5:
    /* 33F5  add     ax,bx */
    AX = add16(AX, BX, 0);
L33F7:
    /* 33F7  loop    L33F4 */
    if (--CX) goto L33F4;
L33F9:
    /* 33F9  cld */
    DF = 0;
L33FA:
    /* 33FA  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_33FB  (+33FB)
       _33FB: is there a VGA? int 10h, AX = 1A00h (read display combination): carry clear for an
       active VGA (codes 7 or 8), set otherwise. */
L33FB: /* _seg003_33FB */
    /* 33FB  mov     ax,VID_DISPLAY_CODE shl 8 */
    AX = 0x1A00;
L33FE:
    /* 33FE  int     10h */
    asm_int(0x10);
L3400:
    /* 3400  cmp     al,1Ah */
    sub8(AL, 0x1A, 0);
L3402:
    /* 3402  jne     L341A */
    if (!ZF) goto L341A;
L3404:
    /* 3404  cmp     bl,0Bh */
    sub8(BL, 0xB, 0);
L3407:
    /* 3407  je      L341A */
    if (ZF) goto L341A;
L3409:
    /* 3409  cmp     bl,0Ch */
    sub8(BL, 0xC, 0);
L340C:
    /* 340C  je      L341A */
    if (ZF) goto L341A;
L340E:
    /* 340E  cmp     bl,7 */
    sub8(BL, 0x7, 0);
L3411:
    /* 3411  je      L3418 */
    if (ZF) goto L3418;
L3413:
    /* 3413  cmp     bl,8 */
    sub8(BL, 0x8, 0);
L3416:
    /* 3416  jne     L341A */
    if (!ZF) goto L341A;
L3418: /* L3418 */
    /* 3418  clc */
    CF = 0;
L3419:
    /* 3419  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L341A: /* L341A */
    /* 341A  stc */
    CF = 1;
L341B:
    /* 341B  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_341C  (+341C)
       SetVideoMode: mode 13h through the BIOS, then unchained (mode X): chain-4 off in the
       sequencer's memory mode, odd/even off in the graphics controller, bit mask and map mask all
       on, the whole 256 KB cleared, and the CRTC switched from doubleword to byte addressing. Leaves
       the sequencer index at the map mask. */
L341C: /* _seg003_341C */
    /* 341C  mov     ax,(VID_SET_MODE shl 8) + 13h */
    AX = 0x13;
L341F:
    /* 341F  int     10h */
    asm_int(0x10);
L3421:
    /* 3421  mov     dx,SC_INDEX */
    DX = 0x3C4;
L3424:
    /* 3424  mov     al,4 */
    AL = 0x4;
L3426:
    /* 3426  out     dx,al */
    asm_out8(DX, AL);
L3427:
    /* 3427  inc     dx */
    DX = (uint16_t)(DX + 1);
L3428:
    /* 3428  in      al,dx */
    AL = asm_in8(DX);
L3429:
    /* 3429  and     al,0F7h */
    AL = (uint8_t)(AL & 0xF7);
L342B:
    /* 342B  or      al,4 */
    AL = (uint8_t)(AL | 0x4);
L342D:
    /* 342D  out     dx,al */
    asm_out8(DX, AL);
L342E:
    /* 342E  mov     dx,GC_INDEX */
    DX = 0x3CE;
L3431:
    /* 3431  mov     al,5 */
    AL = 0x5;
L3433:
    /* 3433  out     dx,al */
    asm_out8(DX, AL);
L3434:
    /* 3434  inc     dx */
    DX = (uint16_t)(DX + 1);
L3435:
    /* 3435  in      al,dx */
    AL = asm_in8(DX);
L3436:
    /* 3436  and     al,0EFh */
    AL = (uint8_t)(AL & 0xEF);
L3438:
    /* 3438  out     dx,al */
    asm_out8(DX, AL);
L3439:
    /* 3439  dec     dx */
    DX = (uint16_t)(DX - 1);
L343A:
    /* 343A  mov     al,6 */
    AL = 0x6;
L343C:
    /* 343C  out     dx,al */
    asm_out8(DX, AL);
L343D:
    /* 343D  inc     dx */
    DX = (uint16_t)(DX + 1);
L343E:
    /* 343E  in      al,dx */
    AL = asm_in8(DX);
L343F:
    /* 343F  and     al,0FDh */
    AL = (uint8_t)(AL & 0xFD);
L3441:
    /* 3441  out     dx,al */
    asm_out8(DX, AL);
L3442:
    /* 3442  mov     ax,0FF08h */
    AX = 0xFF08;
L3445:
    /* 3445  mov     dx,GC_INDEX */
    DX = 0x3CE;
L3448:
    /* 3448  out     dx,ax */
    asm_out16(DX, AX);
L3449:
    /* 3449  mov     dx,SC_INDEX */
    DX = 0x3C4;
L344C:
    /* 344C  mov     ax,0F02h */
    AX = 0xF02;
L344F:
    /* 344F  out     dx,ax */
    asm_out16(DX, AX);
L3450:
    /* 3450  push    es */
    push16(asm_es);
L3451:
    /* 3451  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L3455:
    /* 3455  sub     di,di */
    DI = (uint16_t)(DI - DI);
L3457:
    /* 3457  mov     ax,di */
    AX = DI;
L3459:
    /* 3459  mov     cx,8000h */
    CX = 0x8000;
L345C:
    /* 345C  rep stosw */
    while (CX) { ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2)); CX--; }
L345E:
    /* 345E  pop     es */
    SET_ES(pop16());
L345F:
    /* 345F  mov     dx,CRTC_INDEX */
    DX = 0x3D4;
L3462:
    /* 3462  mov     al,14h */
    AL = 0x14;
L3464:
    /* 3464  out     dx,al */
    asm_out8(DX, AL);
L3465:
    /* 3465  inc     dx */
    DX = (uint16_t)(DX + 1);
L3466:
    /* 3466  in      al,dx */
    AL = asm_in8(DX);
L3467:
    /* 3467  and     al,0BFh */
    AL = (uint8_t)(AL & 0xBF);
L3469:
    /* 3469  out     dx,al */
    asm_out8(DX, AL);
L346A:
    /* 346A  dec     dx */
    DX = (uint16_t)(DX - 1);
L346B:
    /* 346B  mov     al,17h */
    AL = 0x17;
L346D:
    /* 346D  out     dx,al */
    asm_out8(DX, AL);
L346E:
    /* 346E  inc     dx */
    DX = (uint16_t)(DX + 1);
L346F:
    /* 346F  in      al,dx */
    AL = asm_in8(DX);
L3470:
    /* 3470  or      al,40h */
    AL = logic8((uint8_t)(AL | 0x40));
L3472:
    /* 3472  out     dx,al */
    asm_out8(DX, AL);
L3473:
    /* 3473  mov     dx,SC_INDEX */
    DX = 0x3C4;
L3476:
    /* 3476  mov     al,2 */
    AL = 0x2;
L3478:
    /* 3478  out     dx,al */
    asm_out8(DX, AL);
L3479:
    /* 3479  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_347A  (+347A)
       _347A: guard the new window's edges for the unclipped writers: the Ytab entries just above and
       below the window point at the spare area 2CD8, and the edge mask entries just left and right
       of it get partial masks (2D2B, 2D2E). _34C5 undoes this for the old window. Both are called by
       set_the_window.

       The span writers follow (in 2C44's proc). Each takes SI = a list of 6-byte spans (y, left x,
       right x) ended by a y with its top bit set, and writes the left and right partial groups with
       the edge masks and the whole groups between with all four planes:
         _351B  XOR colour 0Fh into the spans (graphics controller function XOR, latches loaded
                first), probably for a highlight or cursor
         _358E  solid fill on the drawing page (_3604), then the same on the other page (383E) */
L347A: /* _seg003_347A */
    /* 347A  mov     ax,word ptr ds:[WIN_TOP] */
    AX = rw(pDS, 0x3DF4);
L347D:
    /* 347D  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L347F:
    /* 347F  add     ax,36ACh */
    AX = (uint16_t)(AX + 0x36AC);
L3482:
    /* 3482  mov     bx,ax */
    BX = AX;
L3484:
    /* 3484  mov     ax,word ptr ds:[2CD8h] */
    AX = rw(pDS, 0x2CD8);
L3487:
    /* 3487  xchg    ax,word ptr [bx] */
    { uint16_t t_ = rw(pDS, BX);
    ww(pDS, BX, AX);
    AX = t_; }
L3489:
    /* 3489  mov     ax,word ptr ds:[WIN_BOTTOM] */
    AX = rw(pDS, 0x3DF8);
L348C:
    /* 348C  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L348E:
    /* 348E  add     ax,36A8h */
    AX = (uint16_t)(AX + 0x36A8);
L3491:
    /* 3491  mov     bx,ax */
    BX = AX;
L3493:
    /* 3493  mov     ax,word ptr ds:[2CD8h] */
    AX = rw(pDS, 0x2CD8);
L3496:
    /* 3496  xchg    ax,word ptr [bx] */
    { uint16_t t_ = rw(pDS, BX);
    ww(pDS, BX, AX);
    AX = t_; }
L3498:
    /* 3498  mov     ax,3B62h */
    AX = 0x3B62;
L349B:
    /* 349B  mov     bx,word ptr ds:[WIN_LEFT] */
    BX = rw(pDS, 0x3DF2);
L349F:
    /* 349F  dec     bx */
    BX = (uint16_t)(BX - 1);
L34A0:
    /* 34A0  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L34A2:
    /* 34A2  mov     bp,ax */
    BP = AX;
L34A4:
    /* 34A4  and     bx,3 */
    BX = (uint16_t)(BX & 0x3);
L34A7:
    /* 34A7  mov     al,byte ptr [bx+2D2Bh] */
    AL = rb(pDS, BX + 0x2D2B);
L34AB:
    /* 34AB  mov     byte ptr [bp],al */
    wb(pSS, BP, AL);
L34AE:
    /* 34AE  mov     ax,3CA6h */
    AX = 0x3CA6;
L34B1:
    /* 34B1  mov     bx,word ptr ds:[WIN_RIGHT] */
    BX = rw(pDS, 0x3DF6);
L34B5:
    /* 34B5  inc     bx */
    BX = (uint16_t)(BX + 1);
L34B6:
    /* 34B6  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L34B8:
    /* 34B8  mov     bp,ax */
    BP = AX;
L34BA:
    /* 34BA  and     bx,3 */
    BX = logic16((uint16_t)(BX & 0x3));
L34BD:
    /* 34BD  mov     al,byte ptr [bx+2D2Eh] */
    AL = rb(pDS, BX + 0x2D2E);
L34C1:
    /* 34C1  mov     byte ptr [bp],al */
    wb(pSS, BP, AL);
L34C4:
    /* 34C4  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_34C5  (+34C5) */
L34C5: /* _seg003_34C5 */
    /* 34C5  mov     ax,3B62h */
    AX = 0x3B62;
L34C8:
    /* 34C8  mov     bx,word ptr ds:[WIN_LEFT] */
    BX = rw(pDS, 0x3DF2);
L34CC:
    /* 34CC  dec     bx */
    BX = (uint16_t)(BX - 1);
L34CD:
    /* 34CD  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L34CF:
    /* 34CF  mov     bp,ax */
    BP = AX;
L34D1:
    /* 34D1  and     bx,3 */
    BX = (uint16_t)(BX & 0x3);
L34D4:
    /* 34D4  mov     al,byte ptr [bx+2D2Ah] */
    AL = rb(pDS, BX + 0x2D2A);
L34D8:
    /* 34D8  mov     byte ptr [bp],al */
    wb(pSS, BP, AL);
L34DB:
    /* 34DB  mov     ax,3CA6h */
    AX = 0x3CA6;
L34DE:
    /* 34DE  mov     bx,word ptr ds:[WIN_RIGHT] */
    BX = rw(pDS, 0x3DF6);
L34E2:
    /* 34E2  inc     bx */
    BX = (uint16_t)(BX + 1);
L34E3:
    /* 34E3  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L34E5:
    /* 34E5  mov     bp,ax */
    BP = AX;
L34E7:
    /* 34E7  and     bx,3 */
    BX = (uint16_t)(BX & 0x3);
L34EA:
    /* 34EA  mov     al,byte ptr [bx+2D2Fh] */
    AL = rb(pDS, BX + 0x2D2F);
L34EE:
    /* 34EE  mov     byte ptr [bp],al */
    wb(pSS, BP, AL);
L34F1:
    /* 34F1  mov     bx,word ptr ds:[WIN_TOP] */
    BX = rw(pDS, 0x3DF4);
L34F5:
    /* 34F5  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L34F7:
    /* 34F7  mov     ax,word ptr [bx+YTAB] */
    AX = rw(pDS, BX + 0x36AA);
L34FB:
    /* 34FB  sub     ax,word ptr ds:[ROW_BYTES] */
    AX = (uint16_t)(AX - rw(pDS, 0x36A6));
L34FF:
    /* 34FF  mov     word ptr [bx+36ACh],ax */
    ww(pDS, BX + 0x36AC, AX);
L3503:
    /* 3503  mov     bx,word ptr ds:[WIN_BOTTOM] */
    BX = rw(pDS, 0x3DF8);
L3507:
    /* 3507  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3509:
    /* 3509  mov     ax,word ptr [bx+YTAB] */
    AX = rw(pDS, BX + 0x36AA);
L350D:
    /* 350D  add     ax,word ptr ds:[ROW_BYTES] */
    AX = add16(AX, rw(pDS, 0x36A6), 0);
L3511:
    /* 3511  mov     word ptr [bx+36A8h],ax */
    ww(pDS, BX + 0x36A8, AX);
L3515:
    /* 3515  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3516:
    /* 3516  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3517:
    /* 3517  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3518:
    /* 3518  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3519:
    /* 3519  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L351A:
    /* 351A  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L351B: /* _seg003_351B */
    /* 351B  mov     ax,1803h */
    AX = 0x1803;
L351E:
    /* 351E  mov     dx,GC_INDEX */
    DX = 0x3CE;
L3521:
    /* 3521  out     dx,ax */
    asm_out16(DX, AX);
L3522:
    /* 3522  mov     dx,SC_DATA */
    DX = 0x3C5;
L3525:
    /* 3525  mov     bl,0Fh */
    BL = 0xF;
L3527:
    /* 3527  push    es */
    push16(asm_es);
L3528:
    /* 3528  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L352C:
    /* 352C  jmp     short L353B */
    goto L353B;
L352E: /* L352E */
    /* 352E  add     di,word ptr [bp+YTAB] */
    DI = (uint16_t)(DI + rw(pSS, BP + 0x36AA));
L3532:
    /* 3532  and     al,bh */
    AL = (uint8_t)(AL & BH);
L3534:
    /* 3534  out     dx,al */
    asm_out8(DX, AL);
L3535:
    /* 3535  mov     al,bl */
    AL = BL;
L3537:
    /* 3537  test    byte ptr es:[di],al */
L353A:
    /* 353A  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L353B: /* L353B */
    /* 353B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L353C:
    /* 353C  shl     ax,1 */
    AX = shl16(AX, 1);
L353E:
    /* 353E  jb      L3582 */
    if (CF) goto L3582;
L3540:
    /* 3540  mov     cx,ax */
    CX = AX;
L3542:
    /* 3542  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3543:
    /* 3543  mov     di,ax */
    DI = AX;
L3545:
    /* 3545  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3546:
    /* 3546  mov     bp,ax */
    BP = AX;
L3548:
    /* 3548  mov     bh,byte ptr [bp+RMASKS] */
    BH = rb(pSS, BP + 0x3CA6);
L354C:
    /* 354C  mov     al,byte ptr [di+LMASKS] */
    AL = rb(pDS, DI + 0x3B62);
L3550:
    /* 3550  xchg    cx,bp */
    { uint16_t t_ = BP;
    BP = CX;
    CX = t_; }
L3552:
    /* 3552  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L3554:
    /* 3554  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L3556:
    /* 3556  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L3558:
    /* 3558  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L355A:
    /* 355A  sub     cx,di */
    CX = sub16(CX, DI, 0);
L355C:
    /* 355C  jle     L352E */
    if (ZF || SF != OF) goto L352E;
L355E:
    /* 355E  out     dx,al */
    asm_out8(DX, AL);
L355F:
    /* 355F  add     di,word ptr [bp+YTAB] */
    DI = (uint16_t)(DI + rw(pSS, BP + 0x36AA));
L3563:
    /* 3563  mov     al,bl */
    AL = BL;
L3565:
    /* 3565  test    byte ptr es:[di],al */
L3568:
    /* 3568  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3569:
    /* 3569  dec     cx */
    CX = (uint16_t)(CX - 1);
L356A:
    /* 356A  jcxz    L3577 */
    if (!CX) goto L3577;
L356C:
    /* 356C  mov     al,0Fh */
    AL = 0xF;
L356E:
    /* 356E  out     dx,al */
    asm_out8(DX, AL);
L356F:
    /* 356F  mov     al,bl */
    AL = BL;
L3571: /* L3571 */
    /* 3571  test    byte ptr es:[di],al */
L3574:
    /* 3574  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3575:
    /* 3575  loop    L3571 */
    if (--CX) goto L3571;
L3577: /* L3577 */
    /* 3577  mov     al,bh */
    AL = BH;
L3579:
    /* 3579  out     dx,al */
    asm_out8(DX, AL);
L357A:
    /* 357A  mov     al,bl */
    AL = BL;
L357C:
    /* 357C  test    byte ptr es:[di],al */
L357F:
    /* 357F  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3580:
    /* 3580  jmp     L353B */
    goto L353B;
L3582: /* L3582 */
    /* 3582  mov     ax,3 */
    AX = 0x3;
L3585:
    /* 3585  mov     dx,GC_INDEX */
    DX = 0x3CE;
L3588:
    /* 3588  out     dx,ax */
    asm_out16(DX, AX);
L3589:
    /* 3589  mov     al,8 */
    AL = 0x8;
L358B:
    /* 358B  out     dx,al */
    asm_out8(DX, AL);
L358C:
    /* 358C  pop     es */
    SET_ES(pop16());
L358D:
    /* 358D  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L358E: /* _seg003_358E */
    /* 358E  push    si */
    push16(SI);
L358F:
    /* 358F  call    _seg003_3604 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3604), 0x3592)) != 0) return c;
L3592:
    /* 3592  pop     si */
    SI = pop16();
L3593:
    /* 3593  jmp     short L359A */
    goto L359A;
L3595: /* L3595 */
    /* 3595  pop     es */
    SET_ES(pop16());
L3596:
    /* 3596  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3597:
    /* 3597  call    _seg003_4C74 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x4C74), 0x359A)) != 0) return c;
L359A: /* L359A */
    /* 359A  mov     dx,SC_DATA */
    DX = 0x3C5;
L359D:
    /* 359D  mov     bl,byte ptr ds:[PEN_COLOR] */
    BL = rb(pDS, 0x410F);
L35A1:
    /* 35A1  push    es */
    push16(asm_es);
L35A2:
    /* 35A2  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L35A6:
    /* 35A6  jmp     short L35C4 */
    goto L35C4;
L35A8: /* L35A8 */
    /* 35A8  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L35AA:
    /* 35AA  shr     bh,1 */
    BH = (uint8_t)(BH >> 1);
L35AC:
    /* 35AC  not     al */
    AL = (uint8_t)~AL;
L35AE:
    /* 35AE  not     bh */
    BH = (uint8_t)~BH;
L35B0:
    /* 35B0  xchg    al,bh */
    { uint8_t t_ = BH;
    BH = AL;
    AL = t_; }
L35B2:
    /* 35B2  add     di,cx */
    DI = (uint16_t)(DI + CX);
L35B4:
    /* 35B4  neg     cx */
    CX = (uint16_t)-CX;
L35B6:
    /* 35B6  jmp     short L35E7 */
    goto L35E7;
L35B8: /* L35B8 */
    /* 35B8  jl      L35A8 */
    if (SF != OF) goto L35A8;
L35BA:
    /* 35BA  add     di,word ptr [bp+YTAB2] */
    DI = (uint16_t)(DI + rw(pSS, BP + 0x383E));
L35BE:
    /* 35BE  and     al,bh */
    AL = (uint8_t)(AL & BH);
L35C0:
    /* 35C0  out     dx,al */
    asm_out8(DX, AL);
L35C1:
    /* 35C1  mov     al,bl */
    AL = BL;
L35C3:
    /* 35C3  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L35C4: /* L35C4 */
    /* 35C4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L35C5:
    /* 35C5  shl     ax,1 */
    AX = shl16(AX, 1);
L35C7:
    /* 35C7  jb      L3595 */
    if (CF) goto L3595;
L35C9:
    /* 35C9  mov     cx,ax */
    CX = AX;
L35CB:
    /* 35CB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L35CC:
    /* 35CC  mov     di,ax */
    DI = AX;
L35CE:
    /* 35CE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L35CF:
    /* 35CF  mov     bp,ax */
    BP = AX;
L35D1:
    /* 35D1  mov     bh,byte ptr [bp+RMASKS] */
    BH = rb(pSS, BP + 0x3CA6);
L35D5:
    /* 35D5  mov     al,byte ptr [di+LMASKS] */
    AL = rb(pDS, DI + 0x3B62);
L35D9:
    /* 35D9  xchg    cx,bp */
    { uint16_t t_ = BP;
    BP = CX;
    CX = t_; }
L35DB:
    /* 35DB  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L35DD:
    /* 35DD  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L35DF:
    /* 35DF  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L35E1:
    /* 35E1  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L35E3:
    /* 35E3  sub     cx,di */
    CX = sub16(CX, DI, 0);
L35E5:
    /* 35E5  jle     L35B8 */
    if (ZF || SF != OF) goto L35B8;
L35E7: /* L35E7 */
    /* 35E7  out     dx,al */
    asm_out8(DX, AL);
L35E8:
    /* 35E8  add     di,word ptr [bp+YTAB2] */
    DI = (uint16_t)(DI + rw(pSS, BP + 0x383E));
L35EC:
    /* 35EC  mov     al,bl */
    AL = BL;
L35EE:
    /* 35EE  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L35EF:
    /* 35EF  dec     cx */
    CX = (uint16_t)(CX - 1);
L35F0:
    /* 35F0  mov     al,0Fh */
    AL = 0xF;
L35F2:
    /* 35F2  out     dx,al */
    asm_out8(DX, AL);
L35F3:
    /* 35F3  mov     al,bl */
    AL = BL;
L35F5:
    /* 35F5  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L35F7:
    /* 35F7  mov     al,bh */
    AL = BH;
L35F9:
    /* 35F9  out     dx,al */
    asm_out8(DX, AL);
L35FA:
    /* 35FA  mov     al,bl */
    AL = BL;
L35FC:
    /* 35FC  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L35FD:
    /* 35FD  jmp     L35C4 */
    goto L35C4;
L35FF: /* L35FF */
    /* 35FF  pop     es */
    SET_ES(pop16());
L3600:
    /* 3600  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3601:
    /* 3601  call    _seg003_4C74 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x4C74), 0x3604)) != 0) return c;

    /* seg003_3604  (+3604)
       _3604: fill the spans with the colour in 410F (FM solid_ylr). The labels after it:
         _3674  save: copy the spans' whole groups to video memory at 4114 through the latches
         _36BB  copy the spans' whole groups from the other page (410A) through the latches
         _36FA  copy from the other page (_3799), then the same onto the displayed page (410C) */
L3604: /* _seg003_3604 */
    /* 3604  mov     dx,SC_DATA */
    DX = 0x3C5;
L3607:
    /* 3607  mov     bl,byte ptr ds:[PEN_COLOR] */
    BL = rb(pDS, 0x410F);
L360B:
    /* 360B  push    es */
    push16(asm_es);
L360C:
    /* 360C  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L3610:
    /* 3610  jmp     short L362E */
    goto L362E;
L3612: /* L3612 */
    /* 3612  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L3614:
    /* 3614  shr     bh,1 */
    BH = (uint8_t)(BH >> 1);
L3616:
    /* 3616  not     al */
    AL = (uint8_t)~AL;
L3618:
    /* 3618  not     bh */
    BH = (uint8_t)~BH;
L361A:
    /* 361A  xchg    al,bh */
    { uint8_t t_ = BH;
    BH = AL;
    AL = t_; }
L361C:
    /* 361C  add     di,cx */
    DI = (uint16_t)(DI + CX);
L361E:
    /* 361E  neg     cx */
    CX = (uint16_t)-CX;
L3620:
    /* 3620  jmp     short L3651 */
    goto L3651;
L3622: /* L3622 */
    /* 3622  jl      L3612 */
    if (SF != OF) goto L3612;
L3624:
    /* 3624  add     di,word ptr [bp+YTAB] */
    DI = (uint16_t)(DI + rw(pSS, BP + 0x36AA));
L3628:
    /* 3628  and     al,bh */
    AL = (uint8_t)(AL & BH);
L362A:
    /* 362A  out     dx,al */
    asm_out8(DX, AL);
L362B:
    /* 362B  mov     al,bl */
    AL = BL;
L362D:
    /* 362D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L362E: /* L362E */
    /* 362E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L362F:
    /* 362F  shl     ax,1 */
    AX = shl16(AX, 1);
L3631:
    /* 3631  jb      L35FF */
    if (CF) goto L35FF;
L3633:
    /* 3633  mov     cx,ax */
    CX = AX;
L3635:
    /* 3635  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3636:
    /* 3636  mov     di,ax */
    DI = AX;
L3638:
    /* 3638  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3639:
    /* 3639  mov     bp,ax */
    BP = AX;
L363B:
    /* 363B  mov     bh,byte ptr [bp+RMASKS] */
    BH = rb(pSS, BP + 0x3CA6);
L363F:
    /* 363F  mov     al,byte ptr [di+LMASKS] */
    AL = rb(pDS, DI + 0x3B62);
L3643:
    /* 3643  xchg    cx,bp */
    { uint16_t t_ = BP;
    BP = CX;
    CX = t_; }
L3645:
    /* 3645  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L3647:
    /* 3647  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L3649:
    /* 3649  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L364B:
    /* 364B  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L364D:
    /* 364D  sub     cx,di */
    CX = sub16(CX, DI, 0);
L364F:
    /* 364F  jle     L3622 */
    if (ZF || SF != OF) goto L3622;
L3651: /* L3651 */
    /* 3651  out     dx,al */
    asm_out8(DX, AL);
L3652:
    /* 3652  add     di,word ptr [bp+YTAB] */
    DI = (uint16_t)(DI + rw(pSS, BP + 0x36AA));
L3656:
    /* 3656  mov     al,bl */
    AL = BL;
L3658:
    /* 3658  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3659:
    /* 3659  dec     cx */
    CX = (uint16_t)(CX - 1);
L365A:
    /* 365A  mov     al,0Fh */
    AL = 0xF;
L365C:
    /* 365C  out     dx,al */
    asm_out8(DX, AL);
L365D:
    /* 365D  mov     al,bl */
    AL = BL;
L365F:
    /* 365F  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L3661:
    /* 3661  mov     al,bh */
    AL = BH;
L3663:
    /* 3663  out     dx,al */
    asm_out8(DX, AL);
L3664:
    /* 3664  mov     al,bl */
    AL = BL;
L3666:
    /* 3666  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3667:
    /* 3667  jmp     L362E */
    goto L362E;
L3669: /* L3669 */
    /* 3669  mov     al,0FFh */
    AL = 0xFF;
L366B:
    /* 366B  mov     dx,GC_DATA */
    DX = 0x3CF;
L366E:
    /* 366E  out     dx,al */
    asm_out8(DX, AL);
L366F:
    /* 366F  pop     es */
    SET_ES(pop16());
L3670:
    /* 3670  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3671:
    /* 3671  call    _seg003_4C74 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x4C74), 0x3674)) != 0) return c;
L3674: /* _seg003_3674 */
    /* 3674  mov     ax,0 */
    AX = 0x0;
L3677:
    /* 3677  mov     dx,GC_DATA */
    DX = 0x3CF;
L367A:
    /* 367A  out     dx,al */
    asm_out8(DX, AL);
L367B:
    /* 367B  mov     dx,SC_DATA */
    DX = 0x3C5;
L367E:
    /* 367E  mov     al,0Fh */
    AL = 0xF;
L3680:
    /* 3680  out     dx,al */
    asm_out8(DX, AL);
L3681:
    /* 3681  mov     bx,si */
    BX = SI;
L3683:
    /* 3683  mov     di,word ptr ds:[PEN_WORD2] */
    DI = rw(pDS, 0x4114);
L3687:
    /* 3687  push    es */
    push16(asm_es);
L3688:
    /* 3688  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L368C: /* L368C */
    /* 368C  mov     bp,word ptr [bx] */
    BP = rw(pDS, BX);
L368E:
    /* 368E  shl     bp,1 */
    BP = shl16(BP, 1);
L3690:
    /* 3690  jb      L3669 */
    if (CF) goto L3669;
L3692:
    /* 3692  mov     si,word ptr [bx+2] */
    SI = rw(pDS, BX + 0x2);
L3695:
    /* 3695  mov     cx,word ptr [bx+4] */
    CX = rw(pDS, BX + 0x4);
L3698:
    /* 3698  add     bx,6 */
    BX = (uint16_t)(BX + 0x6);
L369B:
    /* 369B  sar     si,1 */
    SI = (uint16_t)((int16_t)SI >> 1);
L369D:
    /* 369D  sar     si,1 */
    SI = (uint16_t)((int16_t)SI >> 1);
L369F:
    /* 369F  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L36A1:
    /* 36A1  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L36A3:
    /* 36A3  sub     cx,si */
    CX = (uint16_t)(CX - SI);
L36A5:
    /* 36A5  inc     cx */
    CX = (uint16_t)(CX + 1);
L36A6:
    /* 36A6  add     si,word ptr [bp+YTAB] */
    SI = (uint16_t)(SI + rw(pSS, BP + 0x36AA));
L36AA:
    /* 36AA  rep movs byte ptr es:[di],byte ptr es:[si] */
    while (CX) { wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L36AD:
    /* 36AD  jmp     L368C */
    goto L368C;
L36AF: /* L36AF */
    /* 36AF  mov     ax,0FFh */
    AX = 0xFF;
L36B2:
    /* 36B2  mov     dx,GC_DATA */
    DX = 0x3CF;
L36B5:
    /* 36B5  out     dx,al */
    asm_out8(DX, AL);
L36B6:
    /* 36B6  pop     es */
    SET_ES(pop16());
L36B7:
    /* 36B7  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L36B8:
    /* 36B8  call    _seg003_4C74 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x4C74), 0x36BB)) != 0) return c;
L36BB: /* _seg003_36BB */
    /* 36BB  mov     ax,0 */
    AX = 0x0;
L36BE:
    /* 36BE  mov     dx,GC_DATA */
    DX = 0x3CF;
L36C1:
    /* 36C1  out     dx,al */
    asm_out8(DX, AL);
L36C2:
    /* 36C2  mov     dx,SC_DATA */
    DX = 0x3C5;
L36C5:
    /* 36C5  mov     al,0Fh */
    AL = 0xF;
L36C7:
    /* 36C7  out     dx,al */
    asm_out8(DX, AL);
L36C8:
    /* 36C8  mov     bx,si */
    BX = SI;
L36CA:
    /* 36CA  mov     dx,word ptr ds:[PAGE_DIFF] */
    DX = rw(pDS, 0x410A);
L36CE:
    /* 36CE  push    es */
    push16(asm_es);
L36CF:
    /* 36CF  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L36D3: /* L36D3 */
    /* 36D3  mov     bp,word ptr [bx] */
    BP = rw(pDS, BX);
L36D5:
    /* 36D5  shl     bp,1 */
    BP = shl16(BP, 1);
L36D7:
    /* 36D7  jb      L36AF */
    if (CF) goto L36AF;
L36D9:
    /* 36D9  mov     si,word ptr [bx+2] */
    SI = rw(pDS, BX + 0x2);
L36DC:
    /* 36DC  mov     cx,word ptr [bx+4] */
    CX = rw(pDS, BX + 0x4);
L36DF:
    /* 36DF  add     bx,6 */
    BX = (uint16_t)(BX + 0x6);
L36E2:
    /* 36E2  sar     si,1 */
    SI = (uint16_t)((int16_t)SI >> 1);
L36E4:
    /* 36E4  sar     si,1 */
    SI = (uint16_t)((int16_t)SI >> 1);
L36E6:
    /* 36E6  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L36E8:
    /* 36E8  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L36EA:
    /* 36EA  sub     cx,si */
    CX = (uint16_t)(CX - SI);
L36EC:
    /* 36EC  inc     cx */
    CX = (uint16_t)(CX + 1);
L36ED:
    /* 36ED  add     si,word ptr [bp+YTAB] */
    SI = (uint16_t)(SI + rw(pSS, BP + 0x36AA));
L36F1:
    /* 36F1  mov     di,si */
    DI = SI;
L36F3:
    /* 36F3  add     si,dx */
    SI = (uint16_t)(SI + DX);
L36F5:
    /* 36F5  rep movs byte ptr es:[di],byte ptr es:[si] */
    while (CX) { wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L36F8:
    /* 36F8  jmp     L36D3 */
    goto L36D3;
L36FA: /* _seg003_36FA */
    /* 36FA  push    si */
    push16(SI);
L36FB:
    /* 36FB  call    _seg003_3799 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3799), 0x36FE)) != 0) return c;
L36FE:
    /* 36FE  pop     si */
    SI = pop16();
L36FF:
    /* 36FF  mov     ax,word ptr ds:[PAGE_DIFF] */
    AX = rw(pDS, 0x410A);
L3702:
    /* 3702  add     ax,word ptr ds:[YTAB] */
    AX = (uint16_t)(AX + rw(pDS, 0x36AA));
L3706:
    /* 3706  sub     ax,word ptr ds:[YTAB2] */
    AX = (uint16_t)(AX - rw(pDS, 0x383E));
L370A:
    /* 370A  mov     word ptr ds:[410Ch],ax */
    ww(pDS, 0x410C, AX);
L370D:
    /* 370D  jmp     short L371B */
    goto L371B;
L370F: /* L370F */
    /* 370F  mov     ax,0FFh */
    AX = 0xFF;
L3712:
    /* 3712  mov     dx,GC_DATA */
    DX = 0x3CF;
L3715:
    /* 3715  out     dx,al */
    asm_out8(DX, AL);
L3716:
    /* 3716  pop     es */
    SET_ES(pop16());
L3717:
    /* 3717  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3718:
    /* 3718  call    _seg003_4C74 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x4C74), 0x371B)) != 0) return c;
L371B: /* L371B */
    /* 371B  mov     ax,0 */
    AX = 0x0;
L371E:
    /* 371E  mov     dx,GC_DATA */
    DX = 0x3CF;
L3721:
    /* 3721  out     dx,al */
    asm_out8(DX, AL);
L3722:
    /* 3722  mov     dx,SC_DATA */
    DX = 0x3C5;
L3725:
    /* 3725  mov     bx,si */
    BX = SI;
L3727:
    /* 3727  push    es */
    push16(asm_es);
L3728:
    /* 3728  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L372C:
    /* 372C  jmp     short L374D */
    goto L374D;
L372E: /* L372E */
    /* 372E  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L3730:
    /* 3730  shr     ah,1 */
    AH = (uint8_t)(AH >> 1);
L3732:
    /* 3732  not     ax */
    AX = (uint16_t)~AX;
L3734:
    /* 3734  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L3736:
    /* 3736  add     di,cx */
    DI = (uint16_t)(DI + CX);
L3738:
    /* 3738  neg     cx */
    CX = (uint16_t)-CX;
L373A:
    /* 373A  jmp     short L3772 */
    goto L3772;
L373C: /* L373C */
    /* 373C  jl      L372E */
    if (SF != OF) goto L372E;
L373E:
    /* 373E  add     di,word ptr [si+YTAB2] */
    DI = (uint16_t)(DI + rw(pDS, SI + 0x383E));
L3742:
    /* 3742  mov     bp,word ptr ds:[410Ch] */
    BP = rw(pDS, 0x410C);
L3746:
    /* 3746  and     al,ah */
    AL = (uint8_t)(AL & AH);
L3748:
    /* 3748  out     dx,al */
    asm_out8(DX, AL);
L3749:
    /* 3749  mov     al,byte ptr es:[bp+di] */
    AL = rb(pES, BP + DI);
L374C:
    /* 374C  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L374D: /* L374D */
    /* 374D  mov     si,word ptr [bx] */
    SI = rw(pDS, BX);
L374F:
    /* 374F  shl     si,1 */
    SI = shl16(SI, 1);
L3751:
    /* 3751  jb      L370F */
    if (CF) goto L370F;
L3753:
    /* 3753  mov     di,word ptr [bx+2] */
    DI = rw(pDS, BX + 0x2);
L3756:
    /* 3756  mov     bp,word ptr [bx+4] */
    BP = rw(pDS, BX + 0x4);
L3759:
    /* 3759  add     bx,6 */
    BX = (uint16_t)(BX + 0x6);
L375C:
    /* 375C  mov     al,byte ptr [di+LMASKS] */
    AL = rb(pDS, DI + 0x3B62);
L3760:
    /* 3760  mov     ah,byte ptr [bp+RMASKS] */
    AH = rb(pSS, BP + 0x3CA6);
L3764:
    /* 3764  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L3766:
    /* 3766  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L3768:
    /* 3768  mov     cx,bp */
    CX = BP;
L376A:
    /* 376A  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L376C:
    /* 376C  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L376E:
    /* 376E  sub     cx,di */
    CX = sub16(CX, DI, 0);
L3770:
    /* 3770  jle     L373C */
    if (ZF || SF != OF) goto L373C;
L3772: /* L3772 */
    /* 3772  add     di,word ptr [si+YTAB2] */
    DI = (uint16_t)(DI + rw(pDS, SI + 0x383E));
L3776:
    /* 3776  mov     si,di */
    SI = DI;
L3778:
    /* 3778  add     si,word ptr ds:[410Ch] */
    SI = (uint16_t)(SI + rw(pDS, 0x410C));
L377C:
    /* 377C  out     dx,al */
    asm_out8(DX, AL);
L377D:
    /* 377D  movs    byte ptr es:[di],byte ptr es:[si] */
    wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L377F:
    /* 377F  dec     cx */
    CX = (uint16_t)(CX - 1);
L3780:
    /* 3780  mov     al,0Fh */
    AL = 0xF;
L3782:
    /* 3782  out     dx,al */
    asm_out8(DX, AL);
L3783:
    /* 3783  rep movs byte ptr es:[di],byte ptr es:[si] */
    while (CX) { wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L3786:
    /* 3786  mov     al,ah */
    AL = AH;
L3788:
    /* 3788  out     dx,al */
    asm_out8(DX, AL);
L3789:
    /* 3789  movs    byte ptr es:[di],byte ptr es:[si] */
    wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L378B:
    /* 378B  jmp     L374D */
    goto L374D;
L378D: /* L378D */
    /* 378D  mov     ax,0FFh */
    AX = 0xFF;
L3790:
    /* 3790  mov     dx,GC_DATA */
    DX = 0x3CF;
L3793:
    /* 3793  out     dx,al */
    asm_out8(DX, AL);
L3794:
    /* 3794  pop     es */
    SET_ES(pop16());
L3795:
    /* 3795  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3796:
    /* 3796  call    _seg003_4C74 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x4C74), 0x3799)) != 0) return c;

    /* seg003_3799  (+3799)
       _3799: copy the spans from the other page (410A away) into the drawing page through the
       latches, edge masks included (FM copy_ylr). The labels after it:
         _3817  restore: copy the spans' whole groups back from video memory at 4114
         _3857, _38BC  plane-by-plane copies between the spans and the area at 4114, one plane at a
                time (map mask or read map select); probably the save and restore of a pen whose
                area is not in video memory, not traced */
L3799: /* _seg003_3799 */
    /* 3799  mov     ax,0 */
    AX = 0x0;
L379C:
    /* 379C  mov     dx,GC_DATA */
    DX = 0x3CF;
L379F:
    /* 379F  out     dx,al */
    asm_out8(DX, AL);
L37A0:
    /* 37A0  mov     dx,SC_DATA */
    DX = 0x3C5;
L37A3:
    /* 37A3  mov     bx,si */
    BX = SI;
L37A5:
    /* 37A5  push    es */
    push16(asm_es);
L37A6:
    /* 37A6  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L37AA:
    /* 37AA  jmp     short L37CB */
    goto L37CB;
L37AC: /* L37AC */
    /* 37AC  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L37AE:
    /* 37AE  shr     ah,1 */
    AH = (uint8_t)(AH >> 1);
L37B0:
    /* 37B0  not     ax */
    AX = (uint16_t)~AX;
L37B2:
    /* 37B2  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L37B4:
    /* 37B4  add     di,cx */
    DI = (uint16_t)(DI + CX);
L37B6:
    /* 37B6  neg     cx */
    CX = (uint16_t)-CX;
L37B8:
    /* 37B8  jmp     short L37F0 */
    goto L37F0;
L37BA: /* L37BA */
    /* 37BA  jl      L37AC */
    if (SF != OF) goto L37AC;
L37BC:
    /* 37BC  add     di,word ptr [si+YTAB] */
    DI = (uint16_t)(DI + rw(pDS, SI + 0x36AA));
L37C0:
    /* 37C0  mov     bp,word ptr ds:[PAGE_DIFF] */
    BP = rw(pDS, 0x410A);
L37C4:
    /* 37C4  and     al,ah */
    AL = (uint8_t)(AL & AH);
L37C6:
    /* 37C6  out     dx,al */
    asm_out8(DX, AL);
L37C7:
    /* 37C7  mov     al,byte ptr es:[bp+di] */
    AL = rb(pES, BP + DI);
L37CA:
    /* 37CA  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L37CB: /* L37CB */
    /* 37CB  mov     si,word ptr [bx] */
    SI = rw(pDS, BX);
L37CD:
    /* 37CD  shl     si,1 */
    SI = shl16(SI, 1);
L37CF:
    /* 37CF  jb      L378D */
    if (CF) goto L378D;
L37D1:
    /* 37D1  mov     di,word ptr [bx+2] */
    DI = rw(pDS, BX + 0x2);
L37D4:
    /* 37D4  mov     bp,word ptr [bx+4] */
    BP = rw(pDS, BX + 0x4);
L37D7:
    /* 37D7  add     bx,6 */
    BX = (uint16_t)(BX + 0x6);
L37DA:
    /* 37DA  mov     al,byte ptr [di+LMASKS] */
    AL = rb(pDS, DI + 0x3B62);
L37DE:
    /* 37DE  mov     ah,byte ptr [bp+RMASKS] */
    AH = rb(pSS, BP + 0x3CA6);
L37E2:
    /* 37E2  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L37E4:
    /* 37E4  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L37E6:
    /* 37E6  mov     cx,bp */
    CX = BP;
L37E8:
    /* 37E8  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L37EA:
    /* 37EA  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L37EC:
    /* 37EC  sub     cx,di */
    CX = sub16(CX, DI, 0);
L37EE:
    /* 37EE  jle     L37BA */
    if (ZF || SF != OF) goto L37BA;
L37F0: /* L37F0 */
    /* 37F0  add     di,word ptr [si+YTAB] */
    DI = (uint16_t)(DI + rw(pDS, SI + 0x36AA));
L37F4:
    /* 37F4  mov     si,di */
    SI = DI;
L37F6:
    /* 37F6  add     si,word ptr ds:[PAGE_DIFF] */
    SI = (uint16_t)(SI + rw(pDS, 0x410A));
L37FA:
    /* 37FA  out     dx,al */
    asm_out8(DX, AL);
L37FB:
    /* 37FB  movs    byte ptr es:[di],byte ptr es:[si] */
    wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L37FD:
    /* 37FD  dec     cx */
    CX = (uint16_t)(CX - 1);
L37FE:
    /* 37FE  mov     al,0Fh */
    AL = 0xF;
L3800:
    /* 3800  out     dx,al */
    asm_out8(DX, AL);
L3801:
    /* 3801  rep movs byte ptr es:[di],byte ptr es:[si] */
    while (CX) { wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L3804:
    /* 3804  mov     al,ah */
    AL = AH;
L3806:
    /* 3806  out     dx,al */
    asm_out8(DX, AL);
L3807:
    /* 3807  movs    byte ptr es:[di],byte ptr es:[si] */
    wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L3809:
    /* 3809  jmp     L37CB */
    goto L37CB;
L380B: /* L380B */
    /* 380B  mov     ax,0FFh */
    AX = 0xFF;
L380E:
    /* 380E  mov     dx,GC_DATA */
    DX = 0x3CF;
L3811:
    /* 3811  out     dx,al */
    asm_out8(DX, AL);
L3812:
    /* 3812  pop     es */
    SET_ES(pop16());
L3813:
    /* 3813  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3814:
    /* 3814  call    _seg003_4C74 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x4C74), 0x3817)) != 0) return c;
L3817: /* _seg003_3817 */
    /* 3817  mov     ax,0 */
    AX = 0x0;
L381A:
    /* 381A  mov     dx,GC_DATA */
    DX = 0x3CF;
L381D:
    /* 381D  out     dx,al */
    asm_out8(DX, AL);
L381E:
    /* 381E  mov     dx,SC_DATA */
    DX = 0x3C5;
L3821:
    /* 3821  mov     al,0Fh */
    AL = 0xF;
L3823:
    /* 3823  out     dx,al */
    asm_out8(DX, AL);
L3824:
    /* 3824  mov     bx,si */
    BX = SI;
L3826:
    /* 3826  mov     si,word ptr ds:[PEN_WORD2] */
    SI = rw(pDS, 0x4114);
L382A:
    /* 382A  push    es */
    push16(asm_es);
L382B:
    /* 382B  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L382F: /* L382F */
    /* 382F  mov     bp,word ptr [bx] */
    BP = rw(pDS, BX);
L3831:
    /* 3831  shl     bp,1 */
    BP = shl16(BP, 1);
L3833:
    /* 3833  jb      L380B */
    if (CF) goto L380B;
L3835:
    /* 3835  mov     di,word ptr [bx+2] */
    DI = rw(pDS, BX + 0x2);
L3838:
    /* 3838  mov     cx,word ptr [bx+4] */
    CX = rw(pDS, BX + 0x4);
L383B:
    /* 383B  add     bx,6 */
    BX = (uint16_t)(BX + 0x6);
L383E:
    /* 383E  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L3840:
    /* 3840  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L3842:
    /* 3842  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L3844:
    /* 3844  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L3846:
    /* 3846  sub     cx,di */
    CX = (uint16_t)(CX - DI);
L3848:
    /* 3848  inc     cx */
    CX = (uint16_t)(CX + 1);
L3849:
    /* 3849  add     di,word ptr [bp+YTAB] */
    DI = (uint16_t)(DI + rw(pSS, BP + 0x36AA));
L384D:
    /* 384D  rep movs byte ptr es:[di],byte ptr es:[si] */
    while (CX) { wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L3850:
    /* 3850  jmp     L382F */
    goto L382F;
L3852: /* L3852 */
    /* 3852  pop     es */
    SET_ES(pop16());
L3853:
    /* 3853  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3854:
    /* 3854  call    _seg003_4C74 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x4C74), 0x3857)) != 0) return c;
L3857: /* _seg003_3857 */
    /* 3857  mov     dx,SC_DATA */
    DX = 0x3C5;
L385A:
    /* 385A  mov     al,0Fh */
    AL = 0xF;
L385C:
    /* 385C  out     dx,al */
    asm_out8(DX, AL);
L385D:
    /* 385D  mov     dx,SC_INDEX */
    DX = 0x3C4;
L3860:
    /* 3860  mov     al,2 */
    AL = 0x2;
L3862:
    /* 3862  out     dx,al */
    asm_out8(DX, AL);
L3863:
    /* 3863  inc     dx */
    DX = (uint16_t)(DX + 1);
L3864:
    /* 3864  mov     bp,si */
    BP = SI;
L3866:
    /* 3866  mov     di,word ptr ds:[PEN_WORD2] */
    DI = rw(pDS, 0x4114);
L386A:
    /* 386A  push    ds */
    push16(asm_ds);
L386B:
    /* 386B  mov     ds,word ptr ds:[SCREEN_SEG] */
    SET_DS(rw(pDS, 0x3DFA));
L386F: /* L386F */
    /* 386F  mov     bx,word ptr [bp] */
    BX = rw(pSS, BP);
L3872:
    /* 3872  shl     bx,1 */
    BX = shl16(BX, 1);
L3874:
    /* 3874  jb      L3852 */
    if (CF) goto L3852;
L3876:
    /* 3876  mov     si,word ptr [bp+2] */
    SI = rw(pSS, BP + 0x2);
L3879:
    /* 3879  mov     cx,word ptr [bp+4] */
    CX = rw(pSS, BP + 0x4);
L387C:
    /* 387C  add     bp,6 */
    BP = (uint16_t)(BP + 0x6);
L387F:
    /* 387F  sar     si,1 */
    SI = (uint16_t)((int16_t)SI >> 1);
L3881:
    /* 3881  sar     si,1 */
    SI = (uint16_t)((int16_t)SI >> 1);
L3883:
    /* 3883  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L3885:
    /* 3885  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L3887:
    /* 3887  sub     cx,si */
    CX = (uint16_t)(CX - SI);
L3889:
    /* 3889  inc     cx */
    CX = (uint16_t)(CX + 1);
L388A:
    /* 388A  add     si,word ptr ss:[bx+36AAh] */
    SI = (uint16_t)(SI + rw(pSS, BX + 0x36AA));
L388F:
    /* 388F  mov     bx,cx */
    BX = CX;
L3891:
    /* 3891  mov     al,1 */
    AL = 0x1;
L3893:
    /* 3893  out     dx,al */
    asm_out8(DX, AL);
L3894:
    /* 3894  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L3896:
    /* 3896  sub     si,bx */
    SI = (uint16_t)(SI - BX);
L3898:
    /* 3898  mov     cx,bx */
    CX = BX;
L389A:
    /* 389A  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L389C:
    /* 389C  out     dx,al */
    asm_out8(DX, AL);
L389D:
    /* 389D  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L389F:
    /* 389F  sub     si,bx */
    SI = (uint16_t)(SI - BX);
L38A1:
    /* 38A1  mov     cx,bx */
    CX = BX;
L38A3:
    /* 38A3  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L38A5:
    /* 38A5  out     dx,al */
    asm_out8(DX, AL);
L38A6:
    /* 38A6  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L38A8:
    /* 38A8  sub     si,bx */
    SI = (uint16_t)(SI - BX);
L38AA:
    /* 38AA  mov     cx,bx */
    CX = BX;
L38AC:
    /* 38AC  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L38AE:
    /* 38AE  out     dx,al */
    asm_out8(DX, AL);
L38AF:
    /* 38AF  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L38B1:
    /* 38B1  jmp     L386F */
    goto L386F;
L38B3: /* L38B3 */
    /* 38B3  pop     es */
    SET_ES(pop16());
L38B4:
    /* 38B4  dec     dx */
    DX = dec16(DX);
L38B5:
    /* 38B5  mov     al,8 */
    AL = 0x8;
L38B7:
    /* 38B7  out     dx,al */
    asm_out8(DX, AL);
L38B8:
    /* 38B8  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L38B9:
    /* 38B9  call    _seg003_4C74 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x4C74), 0x38BC)) != 0) return c;
L38BC: /* _seg003_38BC */
    /* 38BC  mov     dx,SC_DATA */
    DX = 0x3C5;
L38BF:
    /* 38BF  mov     al,0Fh */
    AL = 0xF;
L38C1:
    /* 38C1  out     dx,al */
    asm_out8(DX, AL);
L38C2:
    /* 38C2  mov     dx,GC_INDEX */
    DX = 0x3CE;
L38C5:
    /* 38C5  mov     al,4 */
    AL = 0x4;
L38C7:
    /* 38C7  out     dx,al */
    asm_out8(DX, AL);
L38C8:
    /* 38C8  inc     dx */
    DX = (uint16_t)(DX + 1);
L38C9:
    /* 38C9  mov     bp,si */
    BP = SI;
L38CB:
    /* 38CB  mov     si,word ptr ds:[PEN_WORD2] */
    SI = rw(pDS, 0x4114);
L38CF:
    /* 38CF  push    ds */
    push16(asm_ds);
L38D0:
    /* 38D0  mov     ds,word ptr ds:[SCREEN_SEG] */
    SET_DS(rw(pDS, 0x3DFA));
L38D4: /* L38D4 */
    /* 38D4  mov     bx,word ptr [bp] */
    BX = rw(pSS, BP);
L38D7:
    /* 38D7  shl     bx,1 */
    BX = shl16(BX, 1);
L38D9:
    /* 38D9  jb      L38B3 */
    if (CF) goto L38B3;
L38DB:
    /* 38DB  mov     di,word ptr [bp+2] */
    DI = rw(pSS, BP + 0x2);
L38DE:
    /* 38DE  mov     cx,word ptr [bp+4] */
    CX = rw(pSS, BP + 0x4);
L38E1:
    /* 38E1  add     bp,6 */
    BP = (uint16_t)(BP + 0x6);
L38E4:
    /* 38E4  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L38E6:
    /* 38E6  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L38E8:
    /* 38E8  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L38EA:
    /* 38EA  sar     cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
L38EC:
    /* 38EC  sub     cx,di */
    CX = (uint16_t)(CX - DI);
L38EE:
    /* 38EE  inc     cx */
    CX = (uint16_t)(CX + 1);
L38EF:
    /* 38EF  add     di,word ptr ss:[bx+36AAh] */
    DI = (uint16_t)(DI + rw(pSS, BX + 0x36AA));
L38F4:
    /* 38F4  mov     bx,cx */
    BX = CX;
L38F6:
    /* 38F6  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L38F8:
    /* 38F8  out     dx,al */
    asm_out8(DX, AL);
L38F9:
    /* 38F9  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L38FB:
    /* 38FB  sub     di,bx */
    DI = (uint16_t)(DI - BX);
L38FD:
    /* 38FD  mov     cx,bx */
    CX = BX;
L38FF:
    /* 38FF  inc     ax */
    AX = (uint16_t)(AX + 1);
L3900:
    /* 3900  out     dx,al */
    asm_out8(DX, AL);
L3901:
    /* 3901  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L3903:
    /* 3903  sub     di,bx */
    DI = (uint16_t)(DI - BX);
L3905:
    /* 3905  mov     cx,bx */
    CX = BX;
L3907:
    /* 3907  inc     ax */
    AX = (uint16_t)(AX + 1);
L3908:
    /* 3908  out     dx,al */
    asm_out8(DX, AL);
L3909:
    /* 3909  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L390B:
    /* 390B  sub     di,bx */
    DI = (uint16_t)(DI - BX);
L390D:
    /* 390D  mov     cx,bx */
    CX = BX;
L390F:
    /* 390F  inc     ax */
    AX = (uint16_t)(AX + 1);
L3910:
    /* 3910  out     dx,al */
    asm_out8(DX, AL);
L3911:
    /* 3911  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L3913:
    /* 3913  jmp     L38D4 */
    goto L38D4;

    /* seg003_3915  (+3915)
       _3915 (plot): plot (AX, BX) in the colour 410F if it is inside the window. _392D is the
       unclipped entry: map mask from the edge tables, address from Ytab. _3950 and _3958 read a
       pixel (clipped or not) and compare it with CL, leaving the flags. */
L3915: /* _seg003_3915 */
    /* 3915  cmp     bx,word ptr ds:[WIN_TOP] */
    sub16(BX, rw(pDS, 0x3DF4), 0);
L3919:
    /* 3919  jg      L394F */
    if (!ZF && SF == OF) goto L394F;
L391B:
    /* 391B  cmp     bx,word ptr ds:[WIN_BOTTOM] */
    sub16(BX, rw(pDS, 0x3DF8), 0);
L391F:
    /* 391F  jl      L394F */
    if (SF != OF) goto L394F;
L3921:
    /* 3921  cmp     ax,word ptr ds:[WIN_RIGHT] */
    sub16(AX, rw(pDS, 0x3DF6), 0);
L3925:
    /* 3925  jg      L394F */
    if (!ZF && SF == OF) goto L394F;
L3927:
    /* 3927  cmp     ax,word ptr ds:[WIN_LEFT] */
    sub16(AX, rw(pDS, 0x3DF2), 0);
L392B:
    /* 392B  jl      L394F */
    if (SF != OF) goto L394F;
L392D: /* _seg003_392D */
    /* 392D  mov     dx,SC_DATA */
    DX = 0x3C5;
L3930:
    /* 3930  push    es */
    push16(asm_es);
L3931:
    /* 3931  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L3935:
    /* 3935  mov     di,ax */
    DI = AX;
L3937:
    /* 3937  mov     al,byte ptr [di+LMASKS] */
    AL = rb(pDS, DI + 0x3B62);
L393B:
    /* 393B  and     al,byte ptr [di+RMASKS] */
    AL = (uint8_t)(AL & rb(pDS, DI + 0x3CA6));
L393F:
    /* 393F  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L3941:
    /* 3941  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L3943:
    /* 3943  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3945:
    /* 3945  add     di,word ptr [bx+YTAB] */
    DI = add16(DI, rw(pDS, BX + 0x36AA), 0);
L3949:
    /* 3949  out     dx,al */
    asm_out8(DX, AL);
L394A:
    /* 394A  mov     al,byte ptr ds:[PEN_COLOR] */
    AL = rb(pDS, 0x410F);
L394D:
    /* 394D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L394E:
    /* 394E  pop     es */
    SET_ES(pop16());
L394F: /* L394F */
    /* 394F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3950: /* _seg003_3950 */
    /* 3950  mov     ch,cl */
    CH = CL;
L3952:
    /* 3952  call    _seg003_3964 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3964), 0x3955)) != 0) return c;
L3955:
    /* 3955  cmp     al,ch */
    sub8(AL, CH, 0);
L3957:
    /* 3957  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3958: /* _seg003_3958 */
    /* 3958  mov     ch,cl */
    CH = CL;
L395A:
    /* 395A  call    _seg003_397C */
    if ((c = asm_call(ASM_JMP(0x0090, 0x397C), 0x395D)) != 0) return c;
L395D:
    /* 395D  cmp     al,ch */
    sub8(AL, CH, 0);
L395F:
    /* 395F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3960: /* L3960 */
    /* 3960  mov     ax,0FFFFh */
    AX = 0xFFFF;
L3963:
    /* 3963  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_3964  (+3964)
       _3964: read the pixel at (AX, BX), or AX = -1 outside the window; falls into _397C. */
L3964: /* _seg003_3964 */
    /* 3964  cmp     bx,word ptr ds:[WIN_TOP] */
    sub16(BX, rw(pDS, 0x3DF4), 0);
L3968:
    /* 3968  jg      L3960 */
    if (!ZF && SF == OF) goto L3960;
L396A:
    /* 396A  cmp     bx,word ptr ds:[WIN_BOTTOM] */
    sub16(BX, rw(pDS, 0x3DF8), 0);
L396E:
    /* 396E  jl      L3960 */
    if (SF != OF) goto L3960;
L3970:
    /* 3970  cmp     ax,word ptr ds:[WIN_RIGHT] */
    sub16(AX, rw(pDS, 0x3DF6), 0);
L3974:
    /* 3974  jg      L3960 */
    if (!ZF && SF == OF) goto L3960;
L3976:
    /* 3976  cmp     ax,word ptr ds:[WIN_LEFT] */
    sub16(AX, rw(pDS, 0x3DF2), 0);
L397A:
    /* 397A  jl      L3960 */
    if (SF != OF) goto L3960;

    /* seg003_397C  (+397C)
       _397C: read the pixel at (AX, BX): select its plane for reading (graphics controller register
       4) and read the byte. Out: AL = the colour. Leaves the graphics controller index at 8. */
L397C: /* _seg003_397C */
    /* 397C  mov     di,ax */
    DI = AX;
L397E:
    /* 397E  mov     ah,al */
    AH = AL;
L3980:
    /* 3980  and     ah,3 */
    AH = (uint8_t)(AH & 0x3);
L3983:
    /* 3983  mov     al,4 */
    AL = 0x4;
L3985:
    /* 3985  mov     dx,GC_INDEX */
    DX = 0x3CE;
L3988:
    /* 3988  out     dx,ax */
    asm_out16(DX, AX);
L3989:
    /* 3989  push    es */
    push16(asm_es);
L398A:
    /* 398A  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L398E:
    /* 398E  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L3990:
    /* 3990  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L3992:
    /* 3992  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3994:
    /* 3994  add     di,word ptr [bx+YTAB] */
    DI = add16(DI, rw(pDS, BX + 0x36AA), 0);
L3998:
    /* 3998  mov     ah,byte ptr es:[di] */
    AH = rb(pES, DI);
L399B:
    /* 399B  pop     es */
    SET_ES(pop16());
L399C:
    /* 399C  mov     al,8 */
    AL = 0x8;
L399E:
    /* 399E  out     dx,al */
    asm_out8(DX, AL);
L399F:
    /* 399F  mov     al,ah */
    AL = AH;
L39A1:
    /* 39A1  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_39A2  (+39A2)
       _39A2: copy a vertical line (x AX, rows BX down to DX) from the other page (410A) through the
       latches (FM copy_uvline). */
L39A2: /* _seg003_39A2 */
    /* 39A2  mov     di,ax */
    DI = AX;
L39A4:
    /* 39A4  mov     cx,bx */
    CX = BX;
L39A6:
    /* 39A6  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L39A8:
    /* 39A8  inc     cx */
    CX = (uint16_t)(CX + 1);
L39A9:
    /* 39A9  mov     ax,0 */
    AX = 0x0;
L39AC:
    /* 39AC  mov     dx,GC_DATA */
    DX = 0x3CF;
L39AF:
    /* 39AF  out     dx,al */
    asm_out8(DX, AL);
L39B0:
    /* 39B0  mov     al,byte ptr [di+LMASKS] */
    AL = rb(pDS, DI + 0x3B62);
L39B4:
    /* 39B4  and     al,byte ptr [di+RMASKS] */
    AL = (uint8_t)(AL & rb(pDS, DI + 0x3CA6));
L39B8:
    /* 39B8  mov     dx,SC_DATA */
    DX = 0x3C5;
L39BB:
    /* 39BB  out     dx,al */
    asm_out8(DX, AL);
L39BC:
    /* 39BC  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L39BE:
    /* 39BE  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L39C0:
    /* 39C0  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L39C2:
    /* 39C2  add     di,word ptr [bx+YTAB] */
    DI = (uint16_t)(DI + rw(pDS, BX + 0x36AA));
L39C6:
    /* 39C6  mov     bx,word ptr ds:[PAGE_DIFF] */
    BX = rw(pDS, 0x410A);
L39CA:
    /* 39CA  mov     dx,word ptr ds:[ROW_BYTES] */
    DX = rw(pDS, 0x36A6);
L39CE:
    /* 39CE  dec     dx */
    DX = (uint16_t)(DX - 1);
L39CF:
    /* 39CF  push    es */
    push16(asm_es);
L39D0:
    /* 39D0  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L39D4: /* L39D4 */
    /* 39D4  mov     al,byte ptr es:[bx+di] */
    AL = rb(pES, BX + DI);
L39D7:
    /* 39D7  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L39D8:
    /* 39D8  add     di,dx */
    DI = add16(DI, DX, 0);
L39DA:
    /* 39DA  loop    L39D4 */
    if (--CX) goto L39D4;
L39DC:
    /* 39DC  pop     es */
    SET_ES(pop16());
L39DD:
    /* 39DD  mov     ax,0FFh */
    AX = 0xFF;
L39E0:
    /* 39E0  mov     dx,GC_DATA */
    DX = 0x3CF;
L39E3:
    /* 39E3  out     dx,al */
    asm_out8(DX, AL);
L39E4:
    /* 39E4  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_39E5  (+39E5)
       _39E5: a solid vertical line (x AX, rows BX down to DX) in the colour 410F (FM solid_uvline). */
L39E5: /* _seg003_39E5 */
    /* 39E5  mov     di,ax */
    DI = AX;
L39E7:
    /* 39E7  mov     cx,bx */
    CX = BX;
L39E9:
    /* 39E9  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L39EB:
    /* 39EB  inc     cx */
    CX = (uint16_t)(CX + 1);
L39EC:
    /* 39EC  mov     al,byte ptr [di+LMASKS] */
    AL = rb(pDS, DI + 0x3B62);
L39F0:
    /* 39F0  and     al,byte ptr [di+RMASKS] */
    AL = (uint8_t)(AL & rb(pDS, DI + 0x3CA6));
L39F4:
    /* 39F4  mov     dx,SC_DATA */
    DX = 0x3C5;
L39F7:
    /* 39F7  out     dx,al */
    asm_out8(DX, AL);
L39F8:
    /* 39F8  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L39FA:
    /* 39FA  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L39FC:
    /* 39FC  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L39FE:
    /* 39FE  add     di,word ptr [bx+YTAB] */
    DI = (uint16_t)(DI + rw(pDS, BX + 0x36AA));
L3A02:
    /* 3A02  mov     bx,word ptr ds:[ROW_BYTES] */
    BX = rw(pDS, 0x36A6);
L3A06:
    /* 3A06  dec     bx */
    BX = (uint16_t)(BX - 1);
L3A07:
    /* 3A07  mov     al,byte ptr ds:[PEN_COLOR] */
    AL = rb(pDS, 0x410F);
L3A0A:
    /* 3A0A  push    es */
    push16(asm_es);
L3A0B:
    /* 3A0B  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L3A0F: /* L3A0F */
    /* 3A0F  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3A10:
    /* 3A10  add     di,bx */
    DI = add16(DI, BX, 0);
L3A12:
    /* 3A12  loop    L3A0F */
    if (--CX) goto L3A0F;
L3A14:
    /* 3A14  pop     es */
    SET_ES(pop16());
L3A15:
    /* 3A15  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_3A16  (+3A16)
       set_the_color: AX = a pen number (negative: no change). As GRENTRY's fbuf_setcolor, for the
       screen: colour bytes 410E and 4114 from the tables at 21E and 434, and the span writer from
       the table at 2CEC by the pen's mode, less 5044 (the concave-window variants). */
L3A16: /* _seg003_3A16 */
    /* 3A16  shl     ax,1 */
    AX = shl16(AX, 1);
L3A18:
    /* 3A18  jb      L3A3E */
    if (CF) goto L3A3E;
L3A1A:
    /* 3A1A  mov     bx,ax */
    BX = AX;
L3A1C:
    /* 3A1C  mov     ax,word ptr [bx+21Eh] */
    AX = rw(pDS, BX + 0x21E);
L3A20:
    /* 3A20  mov     word ptr ds:[410Eh],ax */
    ww(pDS, 0x410E, AX);
L3A23:
    /* 3A23  mov     ax,word ptr [bx+434h] */
    AX = rw(pDS, BX + 0x434);
L3A27:
    /* 3A27  mov     word ptr ds:[PEN_WORD2],ax */
    ww(pDS, 0x4114, AX);
L3A2A:
    /* 3A2A  db      8Bh,9Fh,8h,0h */
    BX = rw(pDS, BX + 0x8);
L3A2E:
    /* 3A2E  cmp     bx,14h */
    sub16(BX, 0x14, 0);
L3A31:
    /* 3A31  jae     L3A3E */
    if (!CF) goto L3A3E;
L3A33:
    /* 3A33  mov     ax,word ptr [bx+2CECh] */
    AX = rw(pDS, BX + 0x2CEC);
L3A37:
    /* 3A37  sub     ax,word ptr ds:[5044h] */
    AX = sub16(AX, rw(pDS, 0x5044), 0);
L3A3B:
    /* 3A3B  mov     word ptr ds:[SPAN_WRITER],ax */
    ww(pDS, 0x4110, AX);
L3A3E: /* L3A3E */
    /* 3A3E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_3A3F  (+3A3F)
       init_colors: run each pen's set-up routine (the table at 2D02, by the pen's mode) for every
       pen, then store DI (lowered by any pen that took video memory) as the video memory limit 4106. */
L3A3F: /* _seg003_3A3F */
    /* 3A3F  mov     di,0FFFFh */
    DI = 0xFFFF;
L3A42:
    /* 3A42  call    _seg003_3A66 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3A66), 0x3A45)) != 0) return c;
L3A45:
    /* 3A45  mov     cx,21Eh */
    CX = 0x21E;
L3A48:
    /* 3A48  db      81h,0E9h,8h,0h */
    CX = (uint16_t)(CX - 0x8);
L3A4C:
    /* 3A4C  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L3A4E:
    /* 3A4E  sub     bp,bp */
    BP = (uint16_t)(BP - BP);
L3A50: /* L3A50 */
    /* 3A50  db      8Bh,9Eh,8h,0h */
    BX = rw(pSS, BP + 0x8);
L3A54:
    /* 3A54  cmp     bx,14h */
    sub16(BX, 0x14, 0);
L3A57:
    /* 3A57  jae     L3A5D */
    if (!CF) goto L3A5D;
L3A59:
    /* 3A59  call    word ptr [bx+2D02h] */
    if ((c = asm_call(ASM_JMP(0x0090, rw(pDS, BX + 0x2D02)), 0x3A5D)) != 0) return c;
L3A5D: /* L3A5D */
    /* 3A5D  inc     bp */
    BP = inc16(BP);
L3A5E:
    /* 3A5E  inc     bp */
    BP = inc16(BP);
L3A5F:
    /* 3A5F  loop    L3A50 */
    if (--CX) goto L3A50;
L3A61:
    /* 3A61  mov     word ptr ds:[4106h],di */
    ww(pDS, 0x4106, DI);
L3A65:
    /* 3A65  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_3A66  (+3A66) */
L3A66: /* _seg003_3A66 */
    /* 3A66  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3A67:
    /* 3A67  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_3A68  (+3A68)
       set_the_window (far through 5ADE): SI = four words, left, top, right, bottom (y counting up).
       Removes the old window's edge guards (_34C5), copies the window to 3DF2, guards the new one
       (_347A), and copies it to FD72:0AA0, where the 3D renderer's render_3d reads it
       (INSTANCE.ASM). */
L3A68: /* _seg003_3A68 */
    /* 3A68  call    _seg003_34C5 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x34C5), 0x3A6B)) != 0) return c;
L3A6B:
    /* 3A6B  push    si */
    push16(SI);
L3A6C:
    /* 3A6C  mov     di,3DF2h */
    DI = 0x3DF2;
L3A6F:
    /* 3A6F  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3A70:
    /* 3A70  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3A71:
    /* 3A71  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3A72:
    /* 3A72  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3A73:
    /* 3A73  pop     si */
    SI = pop16();
L3A74:
    /* 3A74  call    _seg003_347A */
    if ((c = asm_call(ASM_JMP(0x0090, 0x347A), 0x3A77)) != 0) return c;
L3A77:
    /* 3A77  push    es */
    push16(asm_es);
L3A78:
    /* 3A78  mov     ax,seg seg063 */
    AX = (uint16_t)(0x5624 + PORT_LOAD_SEG);
L3A7B:
    /* 3A7B  mov     es,ax */
    SET_ES(AX);
L3A7D:
    /* 3A7D  mov     di,0AA0h */
    DI = 0xAA0;
L3A80:
    /* 3A80  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3A81:
    /* 3A81  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3A82:
    /* 3A82  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3A83:
    /* 3A83  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3A84:
    /* 3A84  pop     es */
    SET_ES(pop16());
L3A85:
    /* 3A85  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
