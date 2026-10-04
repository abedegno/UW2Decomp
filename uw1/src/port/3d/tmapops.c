/* tmapops.c: replaces src/3d/TMAPOPS.ASM (seg004_5C30, 5C30..6944 of its
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

uint32_t asm_mod_TMAPOPS(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x5C30: goto L5C30;
    case 0x5C31: goto L5C31;
    case 0x5C33: goto L5C33;
    case 0x5C35: goto L5C35;
    case 0x5C37: goto L5C37;
    case 0x5C39: goto L5C39;
    case 0x5C3C: goto L5C3C;
    case 0x5C3F: goto L5C3F;
    case 0x5C40: goto L5C40;
    case 0x5C41: goto L5C41;
    case 0x5C45: goto L5C45;
    case 0x5C46: goto L5C46;
    case 0x5C48: goto L5C48;
    case 0x5C4A: goto L5C4A;
    case 0x5C50: goto L5C50;
    case 0x5C52: goto L5C52;
    case 0x5C58: goto L5C58;
    case 0x5C59: goto L5C59;
    case 0x5C5A: goto L5C5A;
    case 0x5C5E: goto L5C5E;
    case 0x5C64: goto L5C64;
    case 0x5C68: goto L5C68;
    case 0x5C6A: goto L5C6A;
    case 0x5C6E: goto L5C6E;
    case 0x5C72: goto L5C72;
    case 0x5C76: goto L5C76;
    case 0x5C78: goto L5C78;
    case 0x5C79: goto L5C79;
    case 0x5C7A: goto L5C7A;
    case 0x5C7E: goto L5C7E;
    case 0x5C84: goto L5C84;
    case 0x5C86: goto L5C86;
    case 0x5C8C: goto L5C8C;
    case 0x5C8D: goto L5C8D;
    case 0x5C8F: goto L5C8F;
    case 0x5C91: goto L5C91;
    case 0x5C93: goto L5C93;
    case 0x5C96: goto L5C96;
    case 0x5C99: goto L5C99;
    case 0x5C9B: goto L5C9B;
    case 0x5CA0: goto L5CA0;
    case 0x5CA5: goto L5CA5;
    case 0x5CA8: goto L5CA8;
    case 0x5CAC: goto L5CAC;
    case 0x5CAD: goto L5CAD;
    case 0x5CAF: goto L5CAF;
    case 0x5CB0: goto L5CB0;
    case 0x5CB2: goto L5CB2;
    case 0x5CB6: goto L5CB6;
    case 0x5CB7: goto L5CB7;
    case 0x5CBB: goto L5CBB;
    case 0x5CBC: goto L5CBC;
    case 0x5CC0: goto L5CC0;
    case 0x5CC1: goto L5CC1;
    case 0x5CC5: goto L5CC5;
    case 0x5CC6: goto L5CC6;
    case 0x5CCA: goto L5CCA;
    case 0x5CCE: goto L5CCE;
    case 0x5CCF: goto L5CCF;
    case 0x5CD3: goto L5CD3;
    case 0x5CD5: goto L5CD5;
    case 0x5CD7: goto L5CD7;
    case 0x5CD8: goto L5CD8;
    case 0x5CD9: goto L5CD9;
    case 0x5CDD: goto L5CDD;
    case 0x5CDF: goto L5CDF;
    case 0x5CE0: goto L5CE0;
    case 0x5CE2: goto L5CE2;
    case 0x5CE5: goto L5CE5;
    case 0x5CEB: goto L5CEB;
    case 0x5CED: goto L5CED;
    case 0x5CF3: goto L5CF3;
    case 0x5CF5: goto L5CF5;
    case 0x5CFB: goto L5CFB;
    case 0x5CFC: goto L5CFC;
    case 0x5CFE: goto L5CFE;
    case 0x5D00: goto L5D00;
    case 0x5D02: goto L5D02;
    case 0x5D05: goto L5D05;
    case 0x5D08: goto L5D08;
    case 0x5D0A: goto L5D0A;
    case 0x5D0F: goto L5D0F;
    case 0x5D14: goto L5D14;
    case 0x5D17: goto L5D17;
    case 0x5D1B: goto L5D1B;
    case 0x5D1E: goto L5D1E;
    case 0x5D20: goto L5D20;
    case 0x5D21: goto L5D21;
    case 0x5D23: goto L5D23;
    case 0x5D25: goto L5D25;
    case 0x5D27: goto L5D27;
    case 0x5D29: goto L5D29;
    case 0x5D2D: goto L5D2D;
    case 0x5D2E: goto L5D2E;
    case 0x5D32: goto L5D32;
    case 0x5D33: goto L5D33;
    case 0x5D37: goto L5D37;
    case 0x5D38: goto L5D38;
    case 0x5D3C: goto L5D3C;
    case 0x5D3D: goto L5D3D;
    case 0x5D41: goto L5D41;
    case 0x5D45: goto L5D45;
    case 0x5D47: goto L5D47;
    case 0x5D49: goto L5D49;
    case 0x5D4B: goto L5D4B;
    case 0x5D4D: goto L5D4D;
    case 0x5D50: goto L5D50;
    case 0x5D52: goto L5D52;
    case 0x5D56: goto L5D56;
    case 0x5D57: goto L5D57;
    case 0x5D59: goto L5D59;
    case 0x5D5A: goto L5D5A;
    case 0x5D5C: goto L5D5C;
    case 0x5D5E: goto L5D5E;
    case 0x5D60: goto L5D60;
    case 0x5D63: goto L5D63;
    case 0x5D65: goto L5D65;
    case 0x5D69: goto L5D69;
    case 0x5D6A: goto L5D6A;
    case 0x5D6C: goto L5D6C;
    case 0x5D6F: goto L5D6F;
    case 0x5D75: goto L5D75;
    case 0x5D77: goto L5D77;
    case 0x5D7D: goto L5D7D;
    case 0x5D7E: goto L5D7E;
    case 0x5D80: goto L5D80;
    case 0x5D82: goto L5D82;
    case 0x5D84: goto L5D84;
    case 0x5D87: goto L5D87;
    case 0x5D8A: goto L5D8A;
    case 0x5D8C: goto L5D8C;
    case 0x5D91: goto L5D91;
    case 0x5D96: goto L5D96;
    case 0x5D99: goto L5D99;
    case 0x5D9D: goto L5D9D;
    case 0x5DA0: goto L5DA0;
    case 0x5DA2: goto L5DA2;
    case 0x5DA3: goto L5DA3;
    case 0x5DA5: goto L5DA5;
    case 0x5DA7: goto L5DA7;
    case 0x5DA9: goto L5DA9;
    case 0x5DAB: goto L5DAB;
    case 0x5DAF: goto L5DAF;
    case 0x5DB0: goto L5DB0;
    case 0x5DB4: goto L5DB4;
    case 0x5DB5: goto L5DB5;
    case 0x5DB9: goto L5DB9;
    case 0x5DBA: goto L5DBA;
    case 0x5DBE: goto L5DBE;
    case 0x5DBF: goto L5DBF;
    case 0x5DC3: goto L5DC3;
    case 0x5DC7: goto L5DC7;
    case 0x5DC9: goto L5DC9;
    case 0x5DCB: goto L5DCB;
    case 0x5DCD: goto L5DCD;
    case 0x5DCF: goto L5DCF;
    case 0x5DD2: goto L5DD2;
    case 0x5DD4: goto L5DD4;
    case 0x5DD8: goto L5DD8;
    case 0x5DD9: goto L5DD9;
    case 0x5DDB: goto L5DDB;
    case 0x5DDC: goto L5DDC;
    case 0x5DDE: goto L5DDE;
    case 0x5DE0: goto L5DE0;
    case 0x5DE2: goto L5DE2;
    case 0x5DE5: goto L5DE5;
    case 0x5DE7: goto L5DE7;
    case 0x5DEB: goto L5DEB;
    case 0x5DEC: goto L5DEC;
    case 0x5DEE: goto L5DEE;
    case 0x5DF1: goto L5DF1;
    case 0x5DF7: goto L5DF7;
    case 0x5DFA: goto L5DFA;
    case 0x5DFC: goto L5DFC;
    case 0x5E02: goto L5E02;
    case 0x5E04: goto L5E04;
    case 0x5E0A: goto L5E0A;
    case 0x5E0D: goto L5E0D;
    case 0x5E11: goto L5E11;
    case 0x5E12: goto L5E12;
    case 0x5E14: goto L5E14;
    case 0x5E16: goto L5E16;
    case 0x5E18: goto L5E18;
    case 0x5E1B: goto L5E1B;
    case 0x5E1E: goto L5E1E;
    case 0x5E20: goto L5E20;
    case 0x5E25: goto L5E25;
    case 0x5E2A: goto L5E2A;
    case 0x5E2D: goto L5E2D;
    case 0x5E31: goto L5E31;
    case 0x5E32: goto L5E32;
    case 0x5E34: goto L5E34;
    case 0x5E38: goto L5E38;
    case 0x5E39: goto L5E39;
    case 0x5E3D: goto L5E3D;
    case 0x5E3E: goto L5E3E;
    case 0x5E42: goto L5E42;
    case 0x5E43: goto L5E43;
    case 0x5E47: goto L5E47;
    case 0x5E48: goto L5E48;
    case 0x5E4C: goto L5E4C;
    case 0x5E50: goto L5E50;
    case 0x5E51: goto L5E51;
    case 0x5E53: goto L5E53;
    case 0x5E55: goto L5E55;
    case 0x5E57: goto L5E57;
    case 0x5E5B: goto L5E5B;
    case 0x5E5C: goto L5E5C;
    case 0x5E5E: goto L5E5E;
    case 0x5E60: goto L5E60;
    case 0x5E62: goto L5E62;
    case 0x5E63: goto L5E63;
    case 0x5E64: goto L5E64;
    case 0x5E66: goto L5E66;
    case 0x5E68: goto L5E68;
    case 0x5E6A: goto L5E6A;
    case 0x5E6E: goto L5E6E;
    case 0x5E70: goto L5E70;
    case 0x5E71: goto L5E71;
    case 0x5E73: goto L5E73;
    case 0x5E78: goto L5E78;
    case 0x5E7A: goto L5E7A;
    case 0x5E7D: goto L5E7D;
    case 0x5E83: goto L5E83;
    case 0x5E85: goto L5E85;
    case 0x5E88: goto L5E88;
    case 0x5E8D: goto L5E8D;
    case 0x5E8F: goto L5E8F;
    case 0x5E92: goto L5E92;
    case 0x5E98: goto L5E98;
    case 0x5E9A: goto L5E9A;
    case 0x5E9D: goto L5E9D;
    case 0x5EA2: goto L5EA2;
    case 0x5EA4: goto L5EA4;
    case 0x5EA8: goto L5EA8;
    case 0x5EAB: goto L5EAB;
    case 0x5EAD: goto L5EAD;
    case 0x5EB1: goto L5EB1;
    case 0x5EB3: goto L5EB3;
    case 0x5EB7: goto L5EB7;
    case 0x5EB9: goto L5EB9;
    case 0x5EBD: goto L5EBD;
    case 0x5EBF: goto L5EBF;
    case 0x5EC3: goto L5EC3;
    case 0x5EC5: goto L5EC5;
    case 0x5EC8: goto L5EC8;
    case 0x5EC9: goto L5EC9;
    case 0x5ECC: goto L5ECC;
    case 0x5ECF: goto L5ECF;
    case 0x5ED1: goto L5ED1;
    case 0x5ED4: goto L5ED4;
    case 0x5ED7: goto L5ED7;
    case 0x5EDA: goto L5EDA;
    case 0x5EDD: goto L5EDD;
    case 0x5EDF: goto L5EDF;
    case 0x5EE2: goto L5EE2;
    case 0x5EE4: goto L5EE4;
    case 0x5EE6: goto L5EE6;
    case 0x5EE8: goto L5EE8;
    case 0x5EE9: goto L5EE9;
    case 0x5EEB: goto L5EEB;
    case 0x5EEE: goto L5EEE;
    case 0x5EF1: goto L5EF1;
    case 0x5EF3: goto L5EF3;
    case 0x5EF5: goto L5EF5;
    case 0x5EF6: goto L5EF6;
    case 0x5EF8: goto L5EF8;
    case 0x5EFB: goto L5EFB;
    case 0x5EFE: goto L5EFE;
    case 0x5F00: goto L5F00;
    case 0x5F02: goto L5F02;
    case 0x5F03: goto L5F03;
    case 0x5F05: goto L5F05;
    case 0x5F07: goto L5F07;
    case 0x5F08: goto L5F08;
    case 0x5F0A: goto L5F0A;
    case 0x5F0B: goto L5F0B;
    case 0x5F0D: goto L5F0D;
    case 0x5F0E: goto L5F0E;
    case 0x5F11: goto L5F11;
    case 0x5F12: goto L5F12;
    case 0x5F15: goto L5F15;
    case 0x5F18: goto L5F18;
    case 0x5F1A: goto L5F1A;
    case 0x5F1C: goto L5F1C;
    case 0x5F1D: goto L5F1D;
    case 0x5F1F: goto L5F1F;
    case 0x5F20: goto L5F20;
    case 0x5F23: goto L5F23;
    case 0x5F26: goto L5F26;
    case 0x5F28: goto L5F28;
    case 0x5F2A: goto L5F2A;
    case 0x5F2B: goto L5F2B;
    case 0x5F2D: goto L5F2D;
    case 0x5F2E: goto L5F2E;
    case 0x5F2F: goto L5F2F;
    case 0x5F31: goto L5F31;
    case 0x5F35: goto L5F35;
    case 0x5F39: goto L5F39;
    case 0x5F3B: goto L5F3B;
    case 0x5F3D: goto L5F3D;
    case 0x5F3E: goto L5F3E;
    case 0x5F40: goto L5F40;
    case 0x5F44: goto L5F44;
    case 0x5F48: goto L5F48;
    case 0x5F4A: goto L5F4A;
    case 0x5F4C: goto L5F4C;
    case 0x5F4D: goto L5F4D;
    case 0x5F4F: goto L5F4F;
    case 0x5F53: goto L5F53;
    case 0x5F57: goto L5F57;
    case 0x5F59: goto L5F59;
    case 0x5F5B: goto L5F5B;
    case 0x5F5C: goto L5F5C;
    case 0x5F5E: goto L5F5E;
    case 0x5F60: goto L5F60;
    case 0x5F61: goto L5F61;
    case 0x5F63: goto L5F63;
    case 0x5F64: goto L5F64;
    case 0x5F66: goto L5F66;
    case 0x5F67: goto L5F67;
    case 0x5F6A: goto L5F6A;
    case 0x5F6B: goto L5F6B;
    case 0x5F6E: goto L5F6E;
    case 0x5F72: goto L5F72;
    case 0x5F74: goto L5F74;
    case 0x5F76: goto L5F76;
    case 0x5F77: goto L5F77;
    case 0x5F79: goto L5F79;
    case 0x5F7A: goto L5F7A;
    case 0x5F7D: goto L5F7D;
    case 0x5F81: goto L5F81;
    case 0x5F83: goto L5F83;
    case 0x5F85: goto L5F85;
    case 0x5F86: goto L5F86;
    case 0x5F88: goto L5F88;
    case 0x5F89: goto L5F89;
    case 0x5F8A: goto L5F8A;
    case 0x5F8B: goto L5F8B;
    case 0x5F8E: goto L5F8E;
    case 0x5F92: goto L5F92;
    case 0x5F94: goto L5F94;
    case 0x5F96: goto L5F96;
    case 0x5F99: goto L5F99;
    case 0x5F9A: goto L5F9A;
    case 0x5F9B: goto L5F9B;
    case 0x5F9C: goto L5F9C;
    case 0x5F9D: goto L5F9D;
    case 0x5F9F: goto L5F9F;
    case 0x5FA1: goto L5FA1;
    case 0x5FA2: goto L5FA2;
    case 0x5FA3: goto L5FA3;
    case 0x5FA4: goto L5FA4;
    case 0x5FA7: goto L5FA7;
    case 0x5FA8: goto L5FA8;
    case 0x5FA9: goto L5FA9;
    case 0x5FAA: goto L5FAA;
    case 0x5FAB: goto L5FAB;
    case 0x5FAD: goto L5FAD;
    case 0x5FAF: goto L5FAF;
    case 0x5FB0: goto L5FB0;
    case 0x5FB1: goto L5FB1;
    case 0x5FB2: goto L5FB2;
    case 0x5FB5: goto L5FB5;
    case 0x5FB6: goto L5FB6;
    case 0x5FB7: goto L5FB7;
    case 0x5FB8: goto L5FB8;
    case 0x5FB9: goto L5FB9;
    case 0x5FBB: goto L5FBB;
    case 0x5FBD: goto L5FBD;
    case 0x5FBE: goto L5FBE;
    case 0x5FBF: goto L5FBF;
    case 0x5FC0: goto L5FC0;
    case 0x5FC3: goto L5FC3;
    case 0x5FC4: goto L5FC4;
    case 0x5FC5: goto L5FC5;
    case 0x5FC6: goto L5FC6;
    case 0x5FC7: goto L5FC7;
    case 0x5FC9: goto L5FC9;
    case 0x5FCB: goto L5FCB;
    case 0x5FCC: goto L5FCC;
    case 0x5FCD: goto L5FCD;
    case 0x5FCE: goto L5FCE;
    case 0x5FD2: goto L5FD2;
    case 0x5FD6: goto L5FD6;
    case 0x5FDA: goto L5FDA;
    case 0x5FDD: goto L5FDD;
    case 0x5FDE: goto L5FDE;
    case 0x5FDF: goto L5FDF;
    case 0x5FE2: goto L5FE2;
    case 0x5FE6: goto L5FE6;
    case 0x5FE8: goto L5FE8;
    case 0x5FEA: goto L5FEA;
    case 0x5FED: goto L5FED;
    case 0x5FEE: goto L5FEE;
    case 0x5FEF: goto L5FEF;
    case 0x5FF0: goto L5FF0;
    case 0x5FF1: goto L5FF1;
    case 0x5FF3: goto L5FF3;
    case 0x5FF5: goto L5FF5;
    case 0x5FF6: goto L5FF6;
    case 0x5FF7: goto L5FF7;
    case 0x5FF8: goto L5FF8;
    case 0x5FFB: goto L5FFB;
    case 0x5FFC: goto L5FFC;
    case 0x5FFD: goto L5FFD;
    case 0x5FFE: goto L5FFE;
    case 0x5FFF: goto L5FFF;
    case 0x6001: goto L6001;
    case 0x6003: goto L6003;
    case 0x6004: goto L6004;
    case 0x6005: goto L6005;
    case 0x6006: goto L6006;
    case 0x6009: goto L6009;
    case 0x600A: goto L600A;
    case 0x600B: goto L600B;
    case 0x600C: goto L600C;
    case 0x600D: goto L600D;
    case 0x600F: goto L600F;
    case 0x6011: goto L6011;
    case 0x6012: goto L6012;
    case 0x6013: goto L6013;
    case 0x6014: goto L6014;
    case 0x6017: goto L6017;
    case 0x6018: goto L6018;
    case 0x6019: goto L6019;
    case 0x601A: goto L601A;
    case 0x601B: goto L601B;
    case 0x601D: goto L601D;
    case 0x601F: goto L601F;
    case 0x6020: goto L6020;
    case 0x6021: goto L6021;
    case 0x6022: goto L6022;
    case 0x6026: goto L6026;
    case 0x602A: goto L602A;
    case 0x602E: goto L602E;
    case 0x6031: goto L6031;
    case 0x6032: goto L6032;
    case 0x6033: goto L6033;
    case 0x6036: goto L6036;
    case 0x603A: goto L603A;
    case 0x603C: goto L603C;
    case 0x603E: goto L603E;
    case 0x6041: goto L6041;
    case 0x6042: goto L6042;
    case 0x6043: goto L6043;
    case 0x6044: goto L6044;
    case 0x6045: goto L6045;
    case 0x6047: goto L6047;
    case 0x6049: goto L6049;
    case 0x604A: goto L604A;
    case 0x604B: goto L604B;
    case 0x604C: goto L604C;
    case 0x604F: goto L604F;
    case 0x6050: goto L6050;
    case 0x6051: goto L6051;
    case 0x6052: goto L6052;
    case 0x6053: goto L6053;
    case 0x6055: goto L6055;
    case 0x6057: goto L6057;
    case 0x6058: goto L6058;
    case 0x6059: goto L6059;
    case 0x605A: goto L605A;
    case 0x605D: goto L605D;
    case 0x605E: goto L605E;
    case 0x605F: goto L605F;
    case 0x6060: goto L6060;
    case 0x6061: goto L6061;
    case 0x6063: goto L6063;
    case 0x6065: goto L6065;
    case 0x6066: goto L6066;
    case 0x6067: goto L6067;
    case 0x6068: goto L6068;
    case 0x606B: goto L606B;
    case 0x606C: goto L606C;
    case 0x606D: goto L606D;
    case 0x606E: goto L606E;
    case 0x606F: goto L606F;
    case 0x6071: goto L6071;
    case 0x6073: goto L6073;
    case 0x6074: goto L6074;
    case 0x6075: goto L6075;
    case 0x6076: goto L6076;
    case 0x607A: goto L607A;
    case 0x607E: goto L607E;
    case 0x6082: goto L6082;
    case 0x6085: goto L6085;
    case 0x6086: goto L6086;
    case 0x6087: goto L6087;
    case 0x608A: goto L608A;
    case 0x608E: goto L608E;
    case 0x6090: goto L6090;
    case 0x6092: goto L6092;
    case 0x6095: goto L6095;
    case 0x6096: goto L6096;
    case 0x6097: goto L6097;
    case 0x6098: goto L6098;
    case 0x6099: goto L6099;
    case 0x609B: goto L609B;
    case 0x609D: goto L609D;
    case 0x609E: goto L609E;
    case 0x609F: goto L609F;
    case 0x60A0: goto L60A0;
    case 0x60A3: goto L60A3;
    case 0x60A4: goto L60A4;
    case 0x60A5: goto L60A5;
    case 0x60A6: goto L60A6;
    case 0x60A7: goto L60A7;
    case 0x60A9: goto L60A9;
    case 0x60AB: goto L60AB;
    case 0x60AC: goto L60AC;
    case 0x60AD: goto L60AD;
    case 0x60AE: goto L60AE;
    case 0x60B1: goto L60B1;
    case 0x60B2: goto L60B2;
    case 0x60B3: goto L60B3;
    case 0x60B4: goto L60B4;
    case 0x60B5: goto L60B5;
    case 0x60B7: goto L60B7;
    case 0x60B9: goto L60B9;
    case 0x60BA: goto L60BA;
    case 0x60BB: goto L60BB;
    case 0x60BC: goto L60BC;
    case 0x60BF: goto L60BF;
    case 0x60C0: goto L60C0;
    case 0x60C1: goto L60C1;
    case 0x60C2: goto L60C2;
    case 0x60C3: goto L60C3;
    case 0x60C5: goto L60C5;
    case 0x60C7: goto L60C7;
    case 0x60C8: goto L60C8;
    case 0x60C9: goto L60C9;
    case 0x60CA: goto L60CA;
    case 0x60CE: goto L60CE;
    case 0x60D2: goto L60D2;
    case 0x60D6: goto L60D6;
    case 0x60D9: goto L60D9;
    case 0x60DA: goto L60DA;
    case 0x60DB: goto L60DB;
    case 0x60DC: goto L60DC;
    case 0x60E0: goto L60E0;
    case 0x60E4: goto L60E4;
    case 0x60EA: goto L60EA;
    case 0x60EC: goto L60EC;
    case 0x60EF: goto L60EF;
    case 0x60F0: goto L60F0;
    case 0x60F1: goto L60F1;
    case 0x60F5: goto L60F5;
    case 0x60F6: goto L60F6;
    case 0x60FA: goto L60FA;
    case 0x60FD: goto L60FD;
    case 0x6101: goto L6101;
    case 0x6106: goto L6106;
    case 0x610B: goto L610B;
    case 0x610C: goto L610C;
    case 0x610D: goto L610D;
    case 0x610E: goto L610E;
    case 0x610F: goto L610F;
    case 0x6110: goto L6110;
    case 0x6114: goto L6114;
    case 0x6118: goto L6118;
    case 0x611B: goto L611B;
    case 0x611F: goto L611F;
    case 0x6121: goto L6121;
    case 0x6125: goto L6125;
    case 0x6126: goto L6126;
    case 0x6129: goto L6129;
    case 0x6130: goto L6130;
    case 0x6135: goto L6135;
    case 0x6137: goto L6137;
    case 0x613C: goto L613C;
    case 0x613E: goto L613E;
    case 0x6141: goto L6141;
    case 0x6146: goto L6146;
    case 0x6148: goto L6148;
    case 0x6149: goto L6149;
    case 0x614A: goto L614A;
    case 0x614D: goto L614D;
    case 0x614F: goto L614F;
    case 0x6152: goto L6152;
    case 0x6156: goto L6156;
    case 0x6158: goto L6158;
    case 0x615B: goto L615B;
    case 0x615F: goto L615F;
    case 0x6160: goto L6160;
    case 0x6164: goto L6164;
    case 0x6168: goto L6168;
    case 0x616A: goto L616A;
    case 0x616E: goto L616E;
    case 0x616F: goto L616F;
    case 0x6170: goto L6170;
    case 0x6174: goto L6174;
    case 0x6178: goto L6178;
    case 0x617A: goto L617A;
    case 0x617E: goto L617E;
    case 0x617F: goto L617F;
    case 0x6182: goto L6182;
    case 0x6183: goto L6183;
    case 0x6184: goto L6184;
    case 0x6187: goto L6187;
    case 0x6188: goto L6188;
    case 0x618C: goto L618C;
    case 0x618E: goto L618E;
    case 0x618F: goto L618F;
    case 0x6193: goto L6193;
    case 0x6197: goto L6197;
    case 0x619D: goto L619D;
    case 0x619F: goto L619F;
    case 0x61A0: goto L61A0;
    case 0x61A1: goto L61A1;
    case 0x61A5: goto L61A5;
    case 0x61A8: goto L61A8;
    case 0x61AA: goto L61AA;
    case 0x61AB: goto L61AB;
    case 0x61B0: goto L61B0;
    case 0x61B2: goto L61B2;
    case 0x61B5: goto L61B5;
    case 0x61BA: goto L61BA;
    case 0x61BC: goto L61BC;
    case 0x61C1: goto L61C1;
    case 0x61C3: goto L61C3;
    case 0x61C6: goto L61C6;
    case 0x61CB: goto L61CB;
    case 0x61CD: goto L61CD;
    case 0x61D2: goto L61D2;
    case 0x61D4: goto L61D4;
    case 0x61D7: goto L61D7;
    case 0x61DC: goto L61DC;
    case 0x61DE: goto L61DE;
    case 0x61E3: goto L61E3;
    case 0x61E5: goto L61E5;
    case 0x61E8: goto L61E8;
    case 0x61ED: goto L61ED;
    case 0x61EF: goto L61EF;
    case 0x61F4: goto L61F4;
    case 0x61F6: goto L61F6;
    case 0x61F7: goto L61F7;
    case 0x61FA: goto L61FA;
    case 0x61FB: goto L61FB;
    case 0x61FC: goto L61FC;
    case 0x6201: goto L6201;
    case 0x6203: goto L6203;
    case 0x6206: goto L6206;
    case 0x620B: goto L620B;
    case 0x620D: goto L620D;
    case 0x6212: goto L6212;
    case 0x6214: goto L6214;
    case 0x6217: goto L6217;
    case 0x621C: goto L621C;
    case 0x621E: goto L621E;
    case 0x6223: goto L6223;
    case 0x6225: goto L6225;
    case 0x6228: goto L6228;
    case 0x622D: goto L622D;
    case 0x622F: goto L622F;
    case 0x6234: goto L6234;
    case 0x6236: goto L6236;
    case 0x6239: goto L6239;
    case 0x623E: goto L623E;
    case 0x6240: goto L6240;
    case 0x6245: goto L6245;
    case 0x6247: goto L6247;
    case 0x6248: goto L6248;
    case 0x624B: goto L624B;
    case 0x6250: goto L6250;
    case 0x6255: goto L6255;
    case 0x6259: goto L6259;
    case 0x625D: goto L625D;
    case 0x6260: goto L6260;
    case 0x6262: goto L6262;
    case 0x6267: goto L6267;
    case 0x626B: goto L626B;
    case 0x626E: goto L626E;
    case 0x6272: goto L6272;
    case 0x6274: goto L6274;
    case 0x6277: goto L6277;
    case 0x627B: goto L627B;
    case 0x627E: goto L627E;
    case 0x6282: goto L6282;
    case 0x6284: goto L6284;
    case 0x6287: goto L6287;
    case 0x628A: goto L628A;
    case 0x628C: goto L628C;
    case 0x628E: goto L628E;
    case 0x6292: goto L6292;
    case 0x6296: goto L6296;
    case 0x6299: goto L6299;
    case 0x629B: goto L629B;
    case 0x629D: goto L629D;
    case 0x62A1: goto L62A1;
    case 0x62A3: goto L62A3;
    case 0x62A6: goto L62A6;
    case 0x62A9: goto L62A9;
    case 0x62AC: goto L62AC;
    case 0x62AE: goto L62AE;
    case 0x62B1: goto L62B1;
    case 0x62B4: goto L62B4;
    case 0x62B6: goto L62B6;
    case 0x62B9: goto L62B9;
    case 0x62BB: goto L62BB;
    case 0x62BD: goto L62BD;
    case 0x62BF: goto L62BF;
    case 0x62C1: goto L62C1;
    case 0x62C3: goto L62C3;
    case 0x62C5: goto L62C5;
    case 0x62C6: goto L62C6;
    case 0x62C8: goto L62C8;
    case 0x62CB: goto L62CB;
    case 0x62CC: goto L62CC;
    case 0x62CE: goto L62CE;
    case 0x62D1: goto L62D1;
    case 0x62D4: goto L62D4;
    case 0x62D6: goto L62D6;
    case 0x62D8: goto L62D8;
    case 0x62DA: goto L62DA;
    case 0x62DC: goto L62DC;
    case 0x62DE: goto L62DE;
    case 0x62E0: goto L62E0;
    case 0x62E1: goto L62E1;
    case 0x62E3: goto L62E3;
    case 0x62E6: goto L62E6;
    case 0x62E7: goto L62E7;
    case 0x62E8: goto L62E8;
    case 0x62E9: goto L62E9;
    case 0x62EA: goto L62EA;
    case 0x62EC: goto L62EC;
    case 0x62EE: goto L62EE;
    case 0x62F1: goto L62F1;
    case 0x62F2: goto L62F2;
    case 0x62F3: goto L62F3;
    case 0x62F4: goto L62F4;
    case 0x62F8: goto L62F8;
    case 0x62FC: goto L62FC;
    case 0x62FF: goto L62FF;
    case 0x6302: goto L6302;
    case 0x6304: goto L6304;
    case 0x6306: goto L6306;
    case 0x6308: goto L6308;
    case 0x630A: goto L630A;
    case 0x630C: goto L630C;
    case 0x630E: goto L630E;
    case 0x630F: goto L630F;
    case 0x6311: goto L6311;
    case 0x6314: goto L6314;
    case 0x6315: goto L6315;
    case 0x6318: goto L6318;
    case 0x631B: goto L631B;
    case 0x631D: goto L631D;
    case 0x631F: goto L631F;
    case 0x6321: goto L6321;
    case 0x6323: goto L6323;
    case 0x6325: goto L6325;
    case 0x6327: goto L6327;
    case 0x6328: goto L6328;
    case 0x632A: goto L632A;
    case 0x632D: goto L632D;
    case 0x632E: goto L632E;
    case 0x6332: goto L6332;
    case 0x6334: goto L6334;
    case 0x6337: goto L6337;
    case 0x633A: goto L633A;
    case 0x633D: goto L633D;
    case 0x633F: goto L633F;
    case 0x6342: goto L6342;
    case 0x6345: goto L6345;
    case 0x6348: goto L6348;
    case 0x634A: goto L634A;
    case 0x634C: goto L634C;
    case 0x634E: goto L634E;
    case 0x6350: goto L6350;
    case 0x6352: goto L6352;
    case 0x6354: goto L6354;
    case 0x6356: goto L6356;
    case 0x6357: goto L6357;
    case 0x6359: goto L6359;
    case 0x635B: goto L635B;
    case 0x635C: goto L635C;
    case 0x635E: goto L635E;
    case 0x6361: goto L6361;
    case 0x6364: goto L6364;
    case 0x6366: goto L6366;
    case 0x6368: goto L6368;
    case 0x636A: goto L636A;
    case 0x636C: goto L636C;
    case 0x636E: goto L636E;
    case 0x6370: goto L6370;
    case 0x6371: goto L6371;
    case 0x6373: goto L6373;
    case 0x6376: goto L6376;
    case 0x6377: goto L6377;
    case 0x6378: goto L6378;
    case 0x6379: goto L6379;
    case 0x637A: goto L637A;
    case 0x637C: goto L637C;
    case 0x637E: goto L637E;
    case 0x6381: goto L6381;
    case 0x6382: goto L6382;
    case 0x6383: goto L6383;
    case 0x6384: goto L6384;
    case 0x6388: goto L6388;
    case 0x638C: goto L638C;
    case 0x638F: goto L638F;
    case 0x6392: goto L6392;
    case 0x6394: goto L6394;
    case 0x6396: goto L6396;
    case 0x6398: goto L6398;
    case 0x639A: goto L639A;
    case 0x639C: goto L639C;
    case 0x639E: goto L639E;
    case 0x639F: goto L639F;
    case 0x63A1: goto L63A1;
    case 0x63A4: goto L63A4;
    case 0x63A5: goto L63A5;
    case 0x63A8: goto L63A8;
    case 0x63AB: goto L63AB;
    case 0x63AD: goto L63AD;
    case 0x63AF: goto L63AF;
    case 0x63B1: goto L63B1;
    case 0x63B3: goto L63B3;
    case 0x63B5: goto L63B5;
    case 0x63B7: goto L63B7;
    case 0x63B8: goto L63B8;
    case 0x63BA: goto L63BA;
    case 0x63BD: goto L63BD;
    case 0x63BE: goto L63BE;
    case 0x63C1: goto L63C1;
    case 0x63C5: goto L63C5;
    case 0x63C6: goto L63C6;
    case 0x63CB: goto L63CB;
    case 0x63D0: goto L63D0;
    case 0x63D4: goto L63D4;
    case 0x63D8: goto L63D8;
    case 0x63DB: goto L63DB;
    case 0x63DD: goto L63DD;
    case 0x63E2: goto L63E2;
    case 0x63E6: goto L63E6;
    case 0x63E9: goto L63E9;
    case 0x63ED: goto L63ED;
    case 0x63EF: goto L63EF;
    case 0x63F2: goto L63F2;
    case 0x63F6: goto L63F6;
    case 0x63F9: goto L63F9;
    case 0x63FD: goto L63FD;
    case 0x63FF: goto L63FF;
    case 0x6402: goto L6402;
    case 0x6405: goto L6405;
    case 0x6407: goto L6407;
    case 0x6409: goto L6409;
    case 0x640D: goto L640D;
    case 0x6411: goto L6411;
    case 0x6414: goto L6414;
    case 0x6416: goto L6416;
    case 0x6418: goto L6418;
    case 0x641C: goto L641C;
    case 0x641E: goto L641E;
    case 0x6421: goto L6421;
    case 0x6424: goto L6424;
    case 0x6427: goto L6427;
    case 0x6429: goto L6429;
    case 0x642C: goto L642C;
    case 0x642F: goto L642F;
    case 0x6431: goto L6431;
    case 0x6434: goto L6434;
    case 0x6436: goto L6436;
    case 0x6438: goto L6438;
    case 0x643A: goto L643A;
    case 0x643C: goto L643C;
    case 0x643E: goto L643E;
    case 0x6440: goto L6440;
    case 0x6441: goto L6441;
    case 0x6443: goto L6443;
    case 0x6446: goto L6446;
    case 0x6447: goto L6447;
    case 0x6449: goto L6449;
    case 0x644C: goto L644C;
    case 0x644F: goto L644F;
    case 0x6451: goto L6451;
    case 0x6453: goto L6453;
    case 0x6455: goto L6455;
    case 0x6457: goto L6457;
    case 0x6459: goto L6459;
    case 0x645B: goto L645B;
    case 0x645C: goto L645C;
    case 0x645E: goto L645E;
    case 0x6461: goto L6461;
    case 0x6462: goto L6462;
    case 0x6463: goto L6463;
    case 0x6464: goto L6464;
    case 0x6466: goto L6466;
    case 0x6468: goto L6468;
    case 0x6469: goto L6469;
    case 0x646B: goto L646B;
    case 0x646E: goto L646E;
    case 0x646F: goto L646F;
    case 0x6470: goto L6470;
    case 0x6471: goto L6471;
    case 0x6475: goto L6475;
    case 0x6479: goto L6479;
    case 0x647C: goto L647C;
    case 0x647F: goto L647F;
    case 0x6481: goto L6481;
    case 0x6483: goto L6483;
    case 0x6485: goto L6485;
    case 0x6487: goto L6487;
    case 0x6489: goto L6489;
    case 0x648B: goto L648B;
    case 0x648C: goto L648C;
    case 0x648E: goto L648E;
    case 0x6491: goto L6491;
    case 0x6492: goto L6492;
    case 0x6495: goto L6495;
    case 0x6498: goto L6498;
    case 0x649A: goto L649A;
    case 0x649C: goto L649C;
    case 0x649E: goto L649E;
    case 0x64A0: goto L64A0;
    case 0x64A2: goto L64A2;
    case 0x64A4: goto L64A4;
    case 0x64A5: goto L64A5;
    case 0x64A7: goto L64A7;
    case 0x64AA: goto L64AA;
    case 0x64AB: goto L64AB;
    case 0x64AF: goto L64AF;
    case 0x64B1: goto L64B1;
    case 0x64B4: goto L64B4;
    case 0x64B7: goto L64B7;
    case 0x64BA: goto L64BA;
    case 0x64BC: goto L64BC;
    case 0x64BF: goto L64BF;
    case 0x64C2: goto L64C2;
    case 0x64C5: goto L64C5;
    case 0x64C7: goto L64C7;
    case 0x64C9: goto L64C9;
    case 0x64CB: goto L64CB;
    case 0x64CD: goto L64CD;
    case 0x64CF: goto L64CF;
    case 0x64D1: goto L64D1;
    case 0x64D3: goto L64D3;
    case 0x64D4: goto L64D4;
    case 0x64D6: goto L64D6;
    case 0x64D8: goto L64D8;
    case 0x64D9: goto L64D9;
    case 0x64DB: goto L64DB;
    case 0x64DE: goto L64DE;
    case 0x64E1: goto L64E1;
    case 0x64E3: goto L64E3;
    case 0x64E5: goto L64E5;
    case 0x64E7: goto L64E7;
    case 0x64E9: goto L64E9;
    case 0x64EB: goto L64EB;
    case 0x64ED: goto L64ED;
    case 0x64EE: goto L64EE;
    case 0x64F0: goto L64F0;
    case 0x64F3: goto L64F3;
    case 0x64F4: goto L64F4;
    case 0x64F5: goto L64F5;
    case 0x64F6: goto L64F6;
    case 0x64F8: goto L64F8;
    case 0x64FA: goto L64FA;
    case 0x64FB: goto L64FB;
    case 0x64FD: goto L64FD;
    case 0x6500: goto L6500;
    case 0x6501: goto L6501;
    case 0x6502: goto L6502;
    case 0x6503: goto L6503;
    case 0x6507: goto L6507;
    case 0x650B: goto L650B;
    case 0x650E: goto L650E;
    case 0x6511: goto L6511;
    case 0x6513: goto L6513;
    case 0x6515: goto L6515;
    case 0x6517: goto L6517;
    case 0x6519: goto L6519;
    case 0x651B: goto L651B;
    case 0x651D: goto L651D;
    case 0x651E: goto L651E;
    case 0x6520: goto L6520;
    case 0x6523: goto L6523;
    case 0x6524: goto L6524;
    case 0x6527: goto L6527;
    case 0x652A: goto L652A;
    case 0x652C: goto L652C;
    case 0x652E: goto L652E;
    case 0x6530: goto L6530;
    case 0x6532: goto L6532;
    case 0x6534: goto L6534;
    case 0x6536: goto L6536;
    case 0x6537: goto L6537;
    case 0x6539: goto L6539;
    case 0x653C: goto L653C;
    case 0x653D: goto L653D;
    case 0x6540: goto L6540;
    case 0x6544: goto L6544;
    case 0x6545: goto L6545;
    case 0x654A: goto L654A;
    case 0x654F: goto L654F;
    case 0x6553: goto L6553;
    case 0x6557: goto L6557;
    case 0x655A: goto L655A;
    case 0x655C: goto L655C;
    case 0x6561: goto L6561;
    case 0x6565: goto L6565;
    case 0x6568: goto L6568;
    case 0x656C: goto L656C;
    case 0x656E: goto L656E;
    case 0x6571: goto L6571;
    case 0x6575: goto L6575;
    case 0x6578: goto L6578;
    case 0x657C: goto L657C;
    case 0x657E: goto L657E;
    case 0x6581: goto L6581;
    case 0x6584: goto L6584;
    case 0x6586: goto L6586;
    case 0x6588: goto L6588;
    case 0x658C: goto L658C;
    case 0x6590: goto L6590;
    case 0x6593: goto L6593;
    case 0x6595: goto L6595;
    case 0x6597: goto L6597;
    case 0x659B: goto L659B;
    case 0x659D: goto L659D;
    case 0x65A0: goto L65A0;
    case 0x65A3: goto L65A3;
    case 0x65A6: goto L65A6;
    case 0x65A8: goto L65A8;
    case 0x65AB: goto L65AB;
    case 0x65AD: goto L65AD;
    case 0x65AF: goto L65AF;
    case 0x65B2: goto L65B2;
    case 0x65B4: goto L65B4;
    case 0x65B6: goto L65B6;
    case 0x65B8: goto L65B8;
    case 0x65BA: goto L65BA;
    case 0x65BC: goto L65BC;
    case 0x65BE: goto L65BE;
    case 0x65BF: goto L65BF;
    case 0x65C1: goto L65C1;
    case 0x65C4: goto L65C4;
    case 0x65C5: goto L65C5;
    case 0x65C7: goto L65C7;
    case 0x65CA: goto L65CA;
    case 0x65CD: goto L65CD;
    case 0x65CF: goto L65CF;
    case 0x65D1: goto L65D1;
    case 0x65D3: goto L65D3;
    case 0x65D5: goto L65D5;
    case 0x65D7: goto L65D7;
    case 0x65D9: goto L65D9;
    case 0x65DA: goto L65DA;
    case 0x65DC: goto L65DC;
    case 0x65DF: goto L65DF;
    case 0x65E0: goto L65E0;
    case 0x65E1: goto L65E1;
    case 0x65E2: goto L65E2;
    case 0x65E4: goto L65E4;
    case 0x65E6: goto L65E6;
    case 0x65E7: goto L65E7;
    case 0x65E9: goto L65E9;
    case 0x65EC: goto L65EC;
    case 0x65ED: goto L65ED;
    case 0x65EE: goto L65EE;
    case 0x65EF: goto L65EF;
    case 0x65F3: goto L65F3;
    case 0x65F7: goto L65F7;
    case 0x65FA: goto L65FA;
    case 0x65FD: goto L65FD;
    case 0x65FF: goto L65FF;
    case 0x6601: goto L6601;
    case 0x6603: goto L6603;
    case 0x6605: goto L6605;
    case 0x6607: goto L6607;
    case 0x6609: goto L6609;
    case 0x660A: goto L660A;
    case 0x660C: goto L660C;
    case 0x660F: goto L660F;
    case 0x6610: goto L6610;
    case 0x6613: goto L6613;
    case 0x6616: goto L6616;
    case 0x6618: goto L6618;
    case 0x661A: goto L661A;
    case 0x661C: goto L661C;
    case 0x661E: goto L661E;
    case 0x6620: goto L6620;
    case 0x6622: goto L6622;
    case 0x6623: goto L6623;
    case 0x6625: goto L6625;
    case 0x6628: goto L6628;
    case 0x6629: goto L6629;
    case 0x662D: goto L662D;
    case 0x662F: goto L662F;
    case 0x6632: goto L6632;
    case 0x6634: goto L6634;
    case 0x6637: goto L6637;
    case 0x6639: goto L6639;
    case 0x663C: goto L663C;
    case 0x663F: goto L663F;
    case 0x6642: goto L6642;
    case 0x6644: goto L6644;
    case 0x6646: goto L6646;
    case 0x6648: goto L6648;
    case 0x664A: goto L664A;
    case 0x664C: goto L664C;
    case 0x664E: goto L664E;
    case 0x6650: goto L6650;
    case 0x6651: goto L6651;
    case 0x6653: goto L6653;
    case 0x6655: goto L6655;
    case 0x6656: goto L6656;
    case 0x6658: goto L6658;
    case 0x665B: goto L665B;
    case 0x665E: goto L665E;
    case 0x6660: goto L6660;
    case 0x6662: goto L6662;
    case 0x6664: goto L6664;
    case 0x6666: goto L6666;
    case 0x6668: goto L6668;
    case 0x666A: goto L666A;
    case 0x666B: goto L666B;
    case 0x666D: goto L666D;
    case 0x6670: goto L6670;
    case 0x6671: goto L6671;
    case 0x6672: goto L6672;
    case 0x6673: goto L6673;
    case 0x6675: goto L6675;
    case 0x6677: goto L6677;
    case 0x6678: goto L6678;
    case 0x667A: goto L667A;
    case 0x667D: goto L667D;
    case 0x667E: goto L667E;
    case 0x667F: goto L667F;
    case 0x6680: goto L6680;
    case 0x6684: goto L6684;
    case 0x6688: goto L6688;
    case 0x668B: goto L668B;
    case 0x668E: goto L668E;
    case 0x6690: goto L6690;
    case 0x6692: goto L6692;
    case 0x6694: goto L6694;
    case 0x6696: goto L6696;
    case 0x6698: goto L6698;
    case 0x669A: goto L669A;
    case 0x669B: goto L669B;
    case 0x669D: goto L669D;
    case 0x66A0: goto L66A0;
    case 0x66A1: goto L66A1;
    case 0x66A4: goto L66A4;
    case 0x66A7: goto L66A7;
    case 0x66A9: goto L66A9;
    case 0x66AB: goto L66AB;
    case 0x66AD: goto L66AD;
    case 0x66AF: goto L66AF;
    case 0x66B1: goto L66B1;
    case 0x66B3: goto L66B3;
    case 0x66B4: goto L66B4;
    case 0x66B6: goto L66B6;
    case 0x66B9: goto L66B9;
    case 0x66BA: goto L66BA;
    case 0x66BD: goto L66BD;
    case 0x66C1: goto L66C1;
    case 0x66C2: goto L66C2;
    case 0x66C7: goto L66C7;
    case 0x66CC: goto L66CC;
    case 0x66D0: goto L66D0;
    case 0x66D4: goto L66D4;
    case 0x66D7: goto L66D7;
    case 0x66D9: goto L66D9;
    case 0x66DE: goto L66DE;
    case 0x66E2: goto L66E2;
    case 0x66E5: goto L66E5;
    case 0x66E9: goto L66E9;
    case 0x66EB: goto L66EB;
    case 0x66EE: goto L66EE;
    case 0x66F2: goto L66F2;
    case 0x66F5: goto L66F5;
    case 0x66F9: goto L66F9;
    case 0x66FB: goto L66FB;
    case 0x66FE: goto L66FE;
    case 0x6701: goto L6701;
    case 0x6703: goto L6703;
    case 0x6705: goto L6705;
    case 0x6709: goto L6709;
    case 0x670D: goto L670D;
    case 0x6710: goto L6710;
    case 0x6712: goto L6712;
    case 0x6714: goto L6714;
    case 0x6718: goto L6718;
    case 0x671A: goto L671A;
    case 0x671D: goto L671D;
    case 0x6720: goto L6720;
    case 0x6723: goto L6723;
    case 0x6725: goto L6725;
    case 0x6728: goto L6728;
    case 0x672A: goto L672A;
    case 0x672C: goto L672C;
    case 0x672F: goto L672F;
    case 0x6731: goto L6731;
    case 0x6733: goto L6733;
    case 0x6735: goto L6735;
    case 0x6737: goto L6737;
    case 0x6739: goto L6739;
    case 0x673B: goto L673B;
    case 0x673C: goto L673C;
    case 0x673E: goto L673E;
    case 0x6741: goto L6741;
    case 0x6742: goto L6742;
    case 0x6744: goto L6744;
    case 0x6747: goto L6747;
    case 0x674A: goto L674A;
    case 0x674C: goto L674C;
    case 0x674E: goto L674E;
    case 0x6750: goto L6750;
    case 0x6752: goto L6752;
    case 0x6754: goto L6754;
    case 0x6756: goto L6756;
    case 0x6757: goto L6757;
    case 0x6759: goto L6759;
    case 0x675C: goto L675C;
    case 0x675D: goto L675D;
    case 0x675E: goto L675E;
    case 0x675F: goto L675F;
    case 0x6761: goto L6761;
    case 0x6763: goto L6763;
    case 0x6765: goto L6765;
    case 0x6766: goto L6766;
    case 0x6768: goto L6768;
    case 0x676B: goto L676B;
    case 0x676C: goto L676C;
    case 0x676D: goto L676D;
    case 0x676E: goto L676E;
    case 0x6772: goto L6772;
    case 0x6776: goto L6776;
    case 0x6779: goto L6779;
    case 0x677C: goto L677C;
    case 0x677E: goto L677E;
    case 0x6780: goto L6780;
    case 0x6782: goto L6782;
    case 0x6784: goto L6784;
    case 0x6786: goto L6786;
    case 0x6788: goto L6788;
    case 0x6789: goto L6789;
    case 0x678B: goto L678B;
    case 0x678E: goto L678E;
    case 0x678F: goto L678F;
    case 0x6792: goto L6792;
    case 0x6795: goto L6795;
    case 0x6797: goto L6797;
    case 0x6799: goto L6799;
    case 0x679B: goto L679B;
    case 0x679D: goto L679D;
    case 0x679F: goto L679F;
    case 0x67A1: goto L67A1;
    case 0x67A2: goto L67A2;
    case 0x67A4: goto L67A4;
    case 0x67A7: goto L67A7;
    case 0x67A8: goto L67A8;
    case 0x67AC: goto L67AC;
    case 0x67AE: goto L67AE;
    case 0x67B1: goto L67B1;
    case 0x67B4: goto L67B4;
    case 0x67B6: goto L67B6;
    case 0x67B8: goto L67B8;
    case 0x67BB: goto L67BB;
    case 0x67BE: goto L67BE;
    case 0x67C1: goto L67C1;
    case 0x67C3: goto L67C3;
    case 0x67C5: goto L67C5;
    case 0x67C7: goto L67C7;
    case 0x67C9: goto L67C9;
    case 0x67CB: goto L67CB;
    case 0x67CD: goto L67CD;
    case 0x67CF: goto L67CF;
    case 0x67D0: goto L67D0;
    case 0x67D2: goto L67D2;
    case 0x67D4: goto L67D4;
    case 0x67D5: goto L67D5;
    case 0x67D7: goto L67D7;
    case 0x67DA: goto L67DA;
    case 0x67DD: goto L67DD;
    case 0x67DF: goto L67DF;
    case 0x67E1: goto L67E1;
    case 0x67E3: goto L67E3;
    case 0x67E5: goto L67E5;
    case 0x67E7: goto L67E7;
    case 0x67E9: goto L67E9;
    case 0x67EA: goto L67EA;
    case 0x67EC: goto L67EC;
    case 0x67EF: goto L67EF;
    case 0x67F0: goto L67F0;
    case 0x67F1: goto L67F1;
    case 0x67F2: goto L67F2;
    case 0x67F4: goto L67F4;
    case 0x67F6: goto L67F6;
    case 0x67F8: goto L67F8;
    case 0x67F9: goto L67F9;
    case 0x67FB: goto L67FB;
    case 0x67FE: goto L67FE;
    case 0x67FF: goto L67FF;
    case 0x6800: goto L6800;
    case 0x6801: goto L6801;
    case 0x6805: goto L6805;
    case 0x6809: goto L6809;
    case 0x680C: goto L680C;
    case 0x680F: goto L680F;
    case 0x6811: goto L6811;
    case 0x6813: goto L6813;
    case 0x6815: goto L6815;
    case 0x6817: goto L6817;
    case 0x6819: goto L6819;
    case 0x681B: goto L681B;
    case 0x681C: goto L681C;
    case 0x681E: goto L681E;
    case 0x6821: goto L6821;
    case 0x6822: goto L6822;
    case 0x6825: goto L6825;
    case 0x6828: goto L6828;
    case 0x682A: goto L682A;
    case 0x682C: goto L682C;
    case 0x682E: goto L682E;
    case 0x6830: goto L6830;
    case 0x6832: goto L6832;
    case 0x6834: goto L6834;
    case 0x6835: goto L6835;
    case 0x6837: goto L6837;
    case 0x683A: goto L683A;
    case 0x683B: goto L683B;
    case 0x683E: goto L683E;
    case 0x6842: goto L6842;
    case 0x6843: goto L6843;
    case 0x6844: goto L6844;
    case 0x6846: goto L6846;
    case 0x684A: goto L684A;
    case 0x684B: goto L684B;
    case 0x6852: goto L6852;
    case 0x6859: goto L6859;
    case 0x685C: goto L685C;
    case 0x685D: goto L685D;
    case 0x685F: goto L685F;
    case 0x6863: goto L6863;
    case 0x6866: goto L6866;
    case 0x6869: goto L6869;
    case 0x686B: goto L686B;
    case 0x686F: goto L686F;
    case 0x6871: goto L6871;
    case 0x6874: goto L6874;
    case 0x6877: goto L6877;
    case 0x6879: goto L6879;
    case 0x687D: goto L687D;
    case 0x6880: goto L6880;
    case 0x6883: goto L6883;
    case 0x6886: goto L6886;
    case 0x6888: goto L6888;
    case 0x688C: goto L688C;
    case 0x6890: goto L6890;
    case 0x6893: goto L6893;
    case 0x6896: goto L6896;
    case 0x6898: goto L6898;
    case 0x689C: goto L689C;
    case 0x68A0: goto L68A0;
    case 0x68A1: goto L68A1;
    case 0x68A4: goto L68A4;
    case 0x68A6: goto L68A6;
    case 0x68A8: goto L68A8;
    case 0x68AB: goto L68AB;
    case 0x68AE: goto L68AE;
    case 0x68B1: goto L68B1;
    case 0x68B3: goto L68B3;
    case 0x68B5: goto L68B5;
    case 0x68B7: goto L68B7;
    case 0x68BB: goto L68BB;
    case 0x68BD: goto L68BD;
    case 0x68C1: goto L68C1;
    case 0x68C5: goto L68C5;
    case 0x68C7: goto L68C7;
    case 0x68CB: goto L68CB;
    case 0x68CD: goto L68CD;
    case 0x68D1: goto L68D1;
    case 0x68D5: goto L68D5;
    case 0x68D9: goto L68D9;
    case 0x68DD: goto L68DD;
    case 0x68E0: goto L68E0;
    case 0x68E2: goto L68E2;
    case 0x68E4: goto L68E4;
    case 0x68E6: goto L68E6;
    case 0x68EA: goto L68EA;
    case 0x68EC: goto L68EC;
    case 0x68F0: goto L68F0;
    case 0x68F5: goto L68F5;
    case 0x68F9: goto L68F9;
    case 0x68FB: goto L68FB;
    case 0x68FF: goto L68FF;
    case 0x6901: goto L6901;
    case 0x6905: goto L6905;
    case 0x690A: goto L690A;
    case 0x690E: goto L690E;
    case 0x6911: goto L6911;
    case 0x6912: goto L6912;
    case 0x6913: goto L6913;
    case 0x6914: goto L6914;
    case 0x691B: goto L691B;
    case 0x691D: goto L691D;
    case 0x6922: goto L6922;
    case 0x6923: goto L6923;
    case 0x6924: goto L6924;
    case 0x6925: goto L6925;
    case 0x6926: goto L6926;
    case 0x692C: goto L692C;
    case 0x6931: goto L6931;
    case 0x6933: goto L6933;
    case 0x6936: goto L6936;
    case 0x6939: goto L6939;
    case 0x693C: goto L693C;
    case 0x693E: goto L693E;
    case 0x6943: goto L6943;
    default: asm_bad_entry("TMAPOPS.ASM", entry);
    }

    /* seg004_5C30  (+5C30)
       do_set_tmctxt (opcode 0xB2): operands: a bitmap slot (low byte), then the next opcode. Points
       bmap_blk_ptr (B07C) at slot n of the table at B07E (8 bytes a slot) for the texture-mapping
       opcodes that take their texture from the context (do_ctxt_gtmap, do_ctxt_gmap,
       do_compact_map). */
L5C30: /* _do_set_tmctxt */
    /* 5C30  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5C31:
    /* 5C31  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L5C33:
    /* 5C33  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5C35:
    /* 5C35  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5C37:
    /* 5C37  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5C39:
    /* 5C39  add     ax,0B07Eh */
    AX = add16(AX, 0xB07E, 0);
L5C3C:
    /* 5C3C  mov     word ptr ds:[0B07Ch],ax */
    ww(pDS, 0xB07C, AX);
L5C3F:
    /* 5C3F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5C40:
    /* 5C40  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L5C41:
    /* 5C41  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_5C45  (+5C45)
       do_set_gmap_ctxt (opcode 0xD0): operand: a flag. Sets the g-map context's map routine (B00A):
       _seg003_1DA (WALLMAP.ASM; UW2 60h, floor-style) when the flag's low byte is 0, else
       _seg003_545 (POLYFILL.ASM; UW2 545h, wall-style). */
L5C45: /* _do_set_gmap_ctxt */
    /* 5C45  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5C46:
    /* 5C46  test    al,0FFh */
    logic8((uint8_t)(AL & 0xFF));
L5C48:
    /* 5C48  jne     short L5C52 */
    if (!ZF) goto L5C52;
L5C4A:
    /* 5C4A  mov     word ptr ds:[0B00Ah],offset _seg003_1DA */
    ww(pDS, 0xB00A, 0x1DA);
L5C50:
    /* 5C50  jmp     short L5C58 */
    goto L5C58;
L5C52: /* L5C52 */
    /* 5C52  mov     word ptr ds:[0B00Ah],offset _seg003_545 */
    ww(pDS, 0xB00A, 0x545);
L5C58: /* L5C58 */
    /* 5C58  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5C59:
    /* 5C59  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L5C5A:
    /* 5C5A  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_5C5E  (+5C5E)
       do_ctxt_gtmap: a g-polygon textured from the context slot (do_set_tmctxt), floor-style
       mapping. Falls into do_gwtmap's vertex loop with BP = the context's bitmap block. */
L5C5E: /* _do_ctxt_gtmap */
    /* 5C5E  mov     word ptr ds:[0B006h],offset _seg003_1DA */
    ww(pDS, 0xB006, 0x1DA);
L5C64:
    /* 5C64  mov     bp,word ptr ds:[0B07Ch] */
    BP = rw(pDS, 0xB07C);
L5C68:
    /* 5C68  jmp     short L5C9B */
    goto L5C9B;

    /* seg004_5C6A  (+5C6A)
       do_ctxt_gmap: as do_ctxt_gtmap, with the mapping style of the g-map context (B00A). */
L5C6A: /* _do_ctxt_gmap */
    /* 5C6A  mov     bp,word ptr ds:[0B00Ah] */
    BP = rw(pDS, 0xB00A);
L5C6E:
    /* 5C6E  mov     word ptr ds:[0B006h],bp */
    ww(pDS, 0xB006, BP);
L5C72:
    /* 5C72  mov     bp,word ptr ds:[0B07Ch] */
    BP = rw(pDS, 0xB07C);
L5C76:
    /* 5C76  jmp     short L5C9B */
    goto L5C9B;

    /* seg004_5C78  (+5C78)
       do_gtri: does nothing but skip to the next opcode (no operands). */
L5C78: /* _do_gtri */
    /* 5C78  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5C79:
    /* 5C79  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L5C7A:
    /* 5C7A  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_5C7E  (+5C7E)
       do_gtmap and do_gwtmap: a textured polygon whose vertices carry fractional texture
       coordinates. Operands: the bitmap slot, the vertex count n, then n triples (point offset into
       resbuf, u, v); u and v are fractions (0..FFFFh) of the texture's width and height, scaled here
       by the block's words 0 and 1 (the `mul` keeps the 8.8 result). do_gtmap maps floor-style
       (_seg003_1DA; UW2 60h), do_gwtmap wall-style (_seg003_545; UW2 545h). The tail at L60E0, shared by every handler in the module,
       records vert_ptr and either draws the polygon flat (polym set) or calls _seg004_6141 to
       texture map it. */
L5C7E: /* _do_gtmap */
    /* 5C7E  mov     word ptr ds:[0B006h],offset _seg003_1DA */
    ww(pDS, 0xB006, 0x1DA);
L5C84:
    /* 5C84  jmp     short L5C8C */
    goto L5C8C;

    /* seg004_5C86  (+5C86) */
L5C86: /* _do_gwtmap */
    /* 5C86  mov     word ptr ds:[0B006h],offset _seg003_545 */
    ww(pDS, 0xB006, 0x545);
L5C8C: /* L5C8C */
    /* 5C8C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5C8D:
    /* 5C8D  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5C8F:
    /* 5C8F  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5C91:
    /* 5C91  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5C93:
    /* 5C93  add     ax,0B07Eh */
    AX = (uint16_t)(AX + 0xB07E);
L5C96:
    /* 5C96  mov     word ptr ds:[0B07Ch],ax */
    ww(pDS, 0xB07C, AX);
L5C99:
    /* 5C99  mov     bp,ax */
    BP = AX;
L5C9B: /* L5C9B */
    /* 5C9B  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L5CA0:
    /* 5CA0  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L5CA5:
    /* 5CA5  mov     di,VBUF0 */
    DI = 0x309;
L5CA8:
    /* 5CA8  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L5CAC:
    /* 5CAC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5CAD:
    /* 5CAD  mov     cx,ax */
    CX = AX;
L5CAF: /* L5CAF */
    /* 5CAF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5CB0:
    /* 5CB0  mov     bx,ax */
    BX = AX;
L5CB2:
    /* 5CB2  mov     ax,word ptr [bx+PNT_X] */
    AX = rw(pDS, BX + 0x1620);
L5CB6:
    /* 5CB6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5CB7:
    /* 5CB7  mov     ax,word ptr [bx+PNT_Y] */
    AX = rw(pDS, BX + 0x1622);
L5CBB:
    /* 5CBB  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5CBC:
    /* 5CBC  mov     ax,word ptr [bx+PNT_Z] */
    AX = rw(pDS, BX + 0x1624);
L5CC0:
    /* 5CC0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5CC1:
    /* 5CC1  mov     al,byte ptr [bx+PNT_CODES] */
    AL = rb(pDS, BX + 0x1626);
L5CC5:
    /* 5CC5  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5CC6:
    /* 5CC6  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L5CCA:
    /* 5CCA  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L5CCE:
    /* 5CCE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5CCF:
    /* 5CCF  mul     word ptr ds:[bp] */
    mul16(rw(pDS, BP));
L5CD3:
    /* 5CD3  mov     al,ah */
    AL = AH;
L5CD5:
    /* 5CD5  mov     ah,dl */
    AH = DL;
L5CD7:
    /* 5CD7  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5CD8:
    /* 5CD8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5CD9:
    /* 5CD9  mul     word ptr ds:[bp+2] */
    mul16(rw(pDS, BP + 0x2));
L5CDD:
    /* 5CDD  mov     ax,dx */
    AX = DX;
L5CDF:
    /* 5CDF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5CE0:
    /* 5CE0  loop    L5CAF */
    if (--CX) goto L5CAF;
L5CE2:
    /* 5CE2  jmp     L60E0 */
    goto L60E0;

    /* seg004_5CE5  (+5CE5)
       do_compact_map, do_compact_tmap, do_compact_wtmap: a textured quadrilateral in a compact form.
       Operands: the bitmap slot, then four point
       numbers as bytes (each times 8 is the point's offset in resbuf). The texture coordinates are
       implied: the corners get u = width-1 or 0 and v = the v limit or 0 in a fixed order, so the
       whole texture covers the quad. GRIDDB.C's txtflr and txtwal emit these for floors and walls
       (opcodes 0xA0 and 0xA2).
       UW1: do_compact_map (opcode 0D2h) stores the constant 0B00Ah, the address of the g-map
       context's routine word, as the mapper's offset where UW2 loads the word itself, and then
       reads a slot operand like the others (UW2 takes the context's block); probably a slip,
       harmless while no program uses opcode 0D2h (the C never emits it). */
L5CE5: /* _do_compact_map */
    /* 5CE5  mov     word ptr ds:[0B006h],0B00Ah */
    ww(pDS, 0xB006, 0xB00A);
L5CEB:
    /* 5CEB  jmp     short L5CFB */
    goto L5CFB;

    /* seg004_5CED  (+5CED) */
L5CED: /* _do_compact_tmap */
    /* 5CED  mov     word ptr ds:[0B006h],offset _seg003_1DA */
    ww(pDS, 0xB006, 0x1DA);
L5CF3:
    /* 5CF3  jmp     short L5CFB */
    goto L5CFB;

    /* seg004_5CF5  (+5CF5) */
L5CF5: /* _do_compact_wtmap */
    /* 5CF5  mov     word ptr ds:[0B006h],offset _seg003_545 */
    ww(pDS, 0xB006, 0x545);
L5CFB: /* L5CFB */
    /* 5CFB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5CFC:
    /* 5CFC  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5CFE:
    /* 5CFE  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5D00:
    /* 5D00  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5D02:
    /* 5D02  add     ax,0B07Eh */
    AX = (uint16_t)(AX + 0xB07E);
L5D05:
    /* 5D05  mov     word ptr ds:[0B07Ch],ax */
    ww(pDS, 0xB07C, AX);
L5D08:
    /* 5D08  mov     bp,ax */
    BP = AX;
L5D0A: /* L5D0A */
    /* 5D0A  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L5D0F:
    /* 5D0F  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L5D14:
    /* 5D14  mov     di,VBUF0 */
    DI = 0x309;
L5D17:
    /* 5D17  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L5D1B:
    /* 5D1B  mov     cx,4 */
    CX = 0x4;
L5D1E: /* L5D1E */
    /* 5D1E  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L5D20:
    /* 5D20  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L5D21:
    /* 5D21  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5D23:
    /* 5D23  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5D25:
    /* 5D25  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5D27:
    /* 5D27  mov     bx,ax */
    BX = AX;
L5D29:
    /* 5D29  mov     ax,word ptr [bx+PNT_X] */
    AX = rw(pDS, BX + 0x1620);
L5D2D:
    /* 5D2D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5D2E:
    /* 5D2E  mov     ax,word ptr [bx+PNT_Y] */
    AX = rw(pDS, BX + 0x1622);
L5D32:
    /* 5D32  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5D33:
    /* 5D33  mov     ax,word ptr [bx+PNT_Z] */
    AX = rw(pDS, BX + 0x1624);
L5D37:
    /* 5D37  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5D38:
    /* 5D38  mov     al,byte ptr [bx+PNT_CODES] */
    AL = rb(pDS, BX + 0x1626);
L5D3C:
    /* 5D3C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5D3D:
    /* 5D3D  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L5D41:
    /* 5D41  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L5D45:
    /* 5D45  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L5D47:
    /* 5D47  mov     bl,cl */
    BL = CL;
L5D49:
    /* 5D49  dec     bl */
    BL = (uint8_t)(BL - 1);
L5D4B:
    /* 5D4B  dec     bl */
    BL = (uint8_t)(BL - 1);
L5D4D:
    /* 5D4D  test    bl,2 */
    logic8((uint8_t)(BL & 0x2));
L5D50:
    /* 5D50  jne     short L5D59 */
    if (!ZF) goto L5D59;
L5D52:
    /* 5D52  mov     ax,word ptr ds:[bp] */
    AX = rw(pDS, BP);
L5D56:
    /* 5D56  dec     ax */
    AX = (uint16_t)(AX - 1);
L5D57:
    /* 5D57  xchg    ah,al */
    { uint8_t t_ = AL;
    AL = AH;
    AH = t_; }
L5D59: /* L5D59 */
    /* 5D59  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5D5A:
    /* 5D5A  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L5D5C:
    /* 5D5C  mov     bl,cl */
    BL = CL;
L5D5E:
    /* 5D5E  dec     bl */
    BL = (uint8_t)(BL - 1);
L5D60:
    /* 5D60  test    bl,2 */
    logic8((uint8_t)(BL & 0x2));
L5D63:
    /* 5D63  jne     short L5D69 */
    if (!ZF) goto L5D69;
L5D65:
    /* 5D65  mov     ax,word ptr ds:[bp+2] */
    AX = rw(pDS, BP + 0x2);
L5D69: /* L5D69 */
    /* 5D69  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5D6A:
    /* 5D6A  loop    L5D1E */
    if (--CX) goto L5D1E;
L5D6C:
    /* 5D6C  jmp     L5E73 */
    goto L5E73;

    /* seg004_5D6F  (+5D6F)
       do_bcompact_map, do_bcompact_tmap, do_bcompact_wtmap: as the do_compact_ handlers with the u
       corners swapped (the `je` where do_compact has `jne`), so the texture is mirrored left to
       right; probably for faces seen from behind (b for back). do_bcompact_map sets floor-style
       mapping and jumps to L5D8A with AX unset: AX there is the BX the previous handler left (the
       dispatch's xchg), where FM Towns' do_bcompact_map loads bmap_blk_ptr first. So in DOS it
       would map with a stray block pointer; probably the C never emits it. */
L5D6F: /* _do_bcompact_tmap */
    /* 5D6F  mov     word ptr ds:[0B006h],offset _seg003_1DA */
    ww(pDS, 0xB006, 0x1DA);
L5D75:
    /* 5D75  jmp     short L5D7D */
    goto L5D7D;

    /* seg004_5D77  (+5D77) */
L5D77: /* _do_bcompact_wtmap */
    /* 5D77  mov     word ptr ds:[0B006h],offset _seg003_545 */
    ww(pDS, 0xB006, 0x545);
L5D7D: /* L5D7D */
    /* 5D7D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5D7E:
    /* 5D7E  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5D80:
    /* 5D80  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5D82:
    /* 5D82  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5D84:
    /* 5D84  add     ax,0B07Eh */
    AX = (uint16_t)(AX + 0xB07E);
L5D87:
    /* 5D87  mov     word ptr ds:[0B07Ch],ax */
    ww(pDS, 0xB07C, AX);
L5D8A: /* L5D8A */
    /* 5D8A  mov     bp,ax */
    BP = AX;
L5D8C:
    /* 5D8C  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L5D91:
    /* 5D91  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L5D96:
    /* 5D96  mov     di,VBUF0 */
    DI = 0x309;
L5D99:
    /* 5D99  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L5D9D:
    /* 5D9D  mov     cx,4 */
    CX = 0x4;
L5DA0: /* L5DA0 */
    /* 5DA0  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L5DA2:
    /* 5DA2  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L5DA3:
    /* 5DA3  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5DA5:
    /* 5DA5  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5DA7:
    /* 5DA7  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5DA9:
    /* 5DA9  mov     bx,ax */
    BX = AX;
L5DAB:
    /* 5DAB  mov     ax,word ptr [bx+PNT_X] */
    AX = rw(pDS, BX + 0x1620);
L5DAF:
    /* 5DAF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5DB0:
    /* 5DB0  mov     ax,word ptr [bx+PNT_Y] */
    AX = rw(pDS, BX + 0x1622);
L5DB4:
    /* 5DB4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5DB5:
    /* 5DB5  mov     ax,word ptr [bx+PNT_Z] */
    AX = rw(pDS, BX + 0x1624);
L5DB9:
    /* 5DB9  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5DBA:
    /* 5DBA  mov     al,byte ptr [bx+PNT_CODES] */
    AL = rb(pDS, BX + 0x1626);
L5DBE:
    /* 5DBE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5DBF:
    /* 5DBF  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L5DC3:
    /* 5DC3  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L5DC7:
    /* 5DC7  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L5DC9:
    /* 5DC9  mov     bl,cl */
    BL = CL;
L5DCB:
    /* 5DCB  dec     bl */
    BL = (uint8_t)(BL - 1);
L5DCD:
    /* 5DCD  dec     bl */
    BL = (uint8_t)(BL - 1);
L5DCF:
    /* 5DCF  test    bl,2 */
    logic8((uint8_t)(BL & 0x2));
L5DD2:
    /* 5DD2  je      short L5DDB */
    if (ZF) goto L5DDB;
L5DD4:
    /* 5DD4  mov     ax,word ptr ds:[bp] */
    AX = rw(pDS, BP);
L5DD8:
    /* 5DD8  dec     ax */
    AX = (uint16_t)(AX - 1);
L5DD9:
    /* 5DD9  xchg    ah,al */
    { uint8_t t_ = AL;
    AL = AH;
    AH = t_; }
L5DDB: /* L5DDB */
    /* 5DDB  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5DDC:
    /* 5DDC  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L5DDE:
    /* 5DDE  mov     bl,cl */
    BL = CL;
L5DE0:
    /* 5DE0  dec     bl */
    BL = (uint8_t)(BL - 1);
L5DE2:
    /* 5DE2  test    bl,2 */
    logic8((uint8_t)(BL & 0x2));
L5DE5:
    /* 5DE5  jne     short L5DEB */
    if (!ZF) goto L5DEB;
L5DE7:
    /* 5DE7  mov     ax,word ptr ds:[bp+2] */
    AX = rw(pDS, BP + 0x2);
L5DEB: /* L5DEB */
    /* 5DEB  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5DEC:
    /* 5DEC  loop    L5DA0 */
    if (--CX) goto L5DA0;
L5DEE:
    /* 5DEE  jmp     L60E0 */
    goto L60E0;

    /* seg004_5DF1  (+5DF1)
       do_tmap_tri, do_wtmap, do_tmap: a textured triangle (do_tmap_tri) or quadrilateral,
       floor-style (do_tmap, opcode 0x36) or wall-style (do_wtmap). Operands: the bitmap slot, then
       per vertex a point offset into resbuf and a corner word: bit 0 set gives u = width-1, a value
       of 2 or more gives v = the v limit, so each vertex sits at one corner of the texture. */
L5DF1: /* _do_tmap_tri */
    /* 5DF1  mov     word ptr ds:[0B006h],offset _seg003_1DA */
    ww(pDS, 0xB006, 0x1DA);
L5DF7:
    /* 5DF7  mov     cx,3 */
    CX = 0x3;
L5DFA:
    /* 5DFA  jmp     short L5E0D */
    goto L5E0D;

    /* seg004_5DFC  (+5DFC) */
L5DFC: /* _do_wtmap */
    /* 5DFC  mov     word ptr ds:[0B006h],offset _seg003_545 */
    ww(pDS, 0xB006, 0x545);
L5E02:
    /* 5E02  jmp     short L5E0A */
    goto L5E0A;

    /* seg004_5E04  (+5E04) */
L5E04: /* _do_tmap */
    /* 5E04  mov     word ptr ds:[0B006h],offset _seg003_1DA */
    ww(pDS, 0xB006, 0x1DA);
L5E0A: /* L5E0A */
    /* 5E0A  mov     cx,4 */
    CX = 0x4;
L5E0D: /* L5E0D */
    /* 5E0D  mov     word ptr ds:[0B00Ch],cx */
    ww(pDS, 0xB00C, CX);
L5E11:
    /* 5E11  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5E12:
    /* 5E12  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5E14:
    /* 5E14  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5E16:
    /* 5E16  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L5E18:
    /* 5E18  add     ax,0B07Eh */
    AX = (uint16_t)(AX + 0xB07E);
L5E1B:
    /* 5E1B  mov     word ptr ds:[0B07Ch],ax */
    ww(pDS, 0xB07C, AX);
L5E1E:
    /* 5E1E  mov     bp,ax */
    BP = AX;
L5E20:
    /* 5E20  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L5E25:
    /* 5E25  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L5E2A:
    /* 5E2A  mov     di,VBUF0 */
    DI = 0x309;
L5E2D:
    /* 5E2D  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L5E31: /* L5E31 */
    /* 5E31  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5E32:
    /* 5E32  mov     bx,ax */
    BX = AX;
L5E34:
    /* 5E34  mov     ax,word ptr [bx+PNT_X] */
    AX = rw(pDS, BX + 0x1620);
L5E38:
    /* 5E38  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5E39:
    /* 5E39  mov     ax,word ptr [bx+PNT_Y] */
    AX = rw(pDS, BX + 0x1622);
L5E3D:
    /* 5E3D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5E3E:
    /* 5E3E  mov     ax,word ptr [bx+PNT_Z] */
    AX = rw(pDS, BX + 0x1624);
L5E42:
    /* 5E42  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5E43:
    /* 5E43  mov     al,byte ptr [bx+PNT_CODES] */
    AL = rb(pDS, BX + 0x1626);
L5E47:
    /* 5E47  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5E48:
    /* 5E48  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L5E4C:
    /* 5E4C  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L5E50:
    /* 5E50  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5E51:
    /* 5E51  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L5E53:
    /* 5E53  test    al,1 */
    logic8((uint8_t)(AL & 0x1));
L5E55:
    /* 5E55  je      short L5E60 */
    if (ZF) goto L5E60;
L5E57:
    /* 5E57  mov     dx,word ptr ds:[bp] */
    DX = rw(pDS, BP);
L5E5B:
    /* 5E5B  dec     dx */
    DX = (uint16_t)(DX - 1);
L5E5C:
    /* 5E5C  xchg    dh,dl */
    { uint8_t t_ = DL;
    DL = DH;
    DH = t_; }
L5E5E:
    /* 5E5E  mov     dl,0FFh */
    DL = 0xFF;
L5E60: /* L5E60 */
    /* 5E60  mov     word ptr [di],dx */
    ww(pDS, DI, DX);
L5E62:
    /* 5E62  inc     di */
    DI = (uint16_t)(DI + 1);
L5E63:
    /* 5E63  inc     di */
    DI = (uint16_t)(DI + 1);
L5E64:
    /* 5E64  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L5E66:
    /* 5E66  cmp     al,2 */
    sub8(AL, 0x2, 0);
L5E68:
    /* 5E68  jl      short L5E6E */
    if (SF != OF) goto L5E6E;
L5E6A:
    /* 5E6A  mov     dx,word ptr ds:[bp+2] */
    DX = rw(pDS, BP + 0x2);
L5E6E: /* L5E6E */
    /* 5E6E  mov     ax,dx */
    AX = DX;
L5E70:
    /* 5E70  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5E71:
    /* 5E71  loop    L5E31 */
    if (--CX) goto L5E31;
L5E73: /* L5E73 */
    /* 5E73  cmp     word ptr ds:[0B00Ch],4 */
    sub16(rw(pDS, 0xB00C), 0x4, 0);
L5E78:
    /* 5E78  je      short L5E7D */
    if (ZF) goto L5E7D;
L5E7A:
    /* 5E7A  jmp     L60E0 */
    goto L60E0;
L5E7D: /* L5E7D */
    /* 5E7D  cmp     word ptr ds:[0B006h],offset _seg003_545 */
    sub16(rw(pDS, 0xB006), 0x545, 0);
L5E83:
    /* 5E83  je      short L5E88 */
    if (ZF) goto L5E88;
L5E85:
    /* 5E85  jmp     L60E0 */
    goto L60E0;
L5E88: /* L5E88 */
    /* 5E88  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L5E8D:
    /* 5E8D  je      short L5E92 */
    if (ZF) goto L5E92;
L5E8F:
    /* 5E8F  jmp     L60EF */
    goto L60EF;
L5E92: /* L5E92 */
    /* 5E92  test    word ptr ds:[2930h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x2930) & 0xFFFF));
L5E98:
    /* 5E98  je      short L5E9D */
    if (ZF) goto L5E9D;
L5E9A:
    /* 5E9A  jmp     L60E0 */
    goto L60E0;
L5E9D: /* L5E9D */
    /* 5E9D  test    byte ptr ds:[CLIP_OR],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0xFF));
L5EA2:
    /* 5EA2  jne     short L5EC8 */
    if (!ZF) goto L5EC8;
L5EA4:
    /* 5EA4  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L5EA8:
    /* 5EA8  mov     ax,1 */
    AX = 0x1;
L5EAB:
    /* 5EAB  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L5EAD:
    /* 5EAD  cmp     ax,word ptr ds:[30Dh] */
    sub16(AX, rw(pDS, 0x30D), 0);
L5EB1:
    /* 5EB1  jg      short L5EC8 */
    if (!ZF && SF == OF) goto L5EC8;
L5EB3:
    /* 5EB3  cmp     ax,word ptr ds:[319h] */
    sub16(AX, rw(pDS, 0x319), 0);
L5EB7:
    /* 5EB7  jg      short L5EC8 */
    if (!ZF && SF == OF) goto L5EC8;
L5EB9:
    /* 5EB9  cmp     ax,word ptr ds:[325h] */
    sub16(AX, rw(pDS, 0x325), 0);
L5EBD:
    /* 5EBD  jg      short L5EC8 */
    if (!ZF && SF == OF) goto L5EC8;
L5EBF:
    /* 5EBF  cmp     ax,word ptr ds:[331h] */
    sub16(AX, rw(pDS, 0x331), 0);
L5EC3:
    /* 5EC3  jg      short L5EC8 */
    if (!ZF && SF == OF) goto L5EC8;
L5EC5:
    /* 5EC5  jmp     L60E0 */
    goto L60E0;
L5EC8: /* L5EC8 */
    /* 5EC8  push    si */
    push16(SI);
L5EC9:
    /* 5EC9  mov     si,VBUF0 */
    SI = 0x309;
L5ECC:
    /* 5ECC  mov     cx,6 */
    CX = 0x6;
L5ECF:
    /* 5ECF  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L5ED1:
    /* 5ED1  mov     di,0B00Eh */
    DI = 0xB00E;
L5ED4:
    /* 5ED4  mov     si,VBUF0 */
    SI = 0x309;
L5ED7:
    /* 5ED7  mov     dx,4 */
    DX = 0x4;
L5EDA: /* L5EDA */
    /* 5EDA  mov     cx,6 */
    CX = 0x6;
L5EDD:
    /* 5EDD  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L5EDF:
    /* 5EDF  mov     bx,word ptr [si-0Ch] */
    BX = rw(pDS, SI + 0xFFF4);
L5EE2:
    /* 5EE2  add     bx,word ptr [si] */
    BX = add16(BX, rw(pDS, SI), 0);
L5EE4:
    /* 5EE4  jno     short L5EE9 */
    if (!OF) goto L5EE9;
L5EE6:
    /* 5EE6  rcr     bx,1 */
    BX = rcr16(BX, 1);
L5EE8:
    /* 5EE8  test    ax,0FBD1h */
L5EEB:
    /* 5EEB  mov     cx,word ptr [si-0Ah] */
    CX = rw(pDS, SI + 0xFFF6);
L5EEE:
    /* 5EEE  add     cx,word ptr [si+2] */
    CX = add16(CX, rw(pDS, SI + 0x2), 0);
L5EF1:
    /* 5EF1  jno     short L5EF6 */
    if (!OF) goto L5EF6;
L5EF3:
    /* 5EF3  rcr     cx,1 */
    CX = rcr16(CX, 1);
L5EF5:
    /* 5EF5  test    ax,0F9D1h */
L5EF8:
    /* 5EF8  mov     bp,word ptr [si-8] */
    BP = rw(pDS, SI + 0xFFF8);
L5EFB:
    /* 5EFB  add     bp,word ptr [si+4] */
    BP = add16(BP, rw(pDS, SI + 0x4), 0);
L5EFE:
    /* 5EFE  jno     short L5F03 */
    if (!OF) goto L5F03;
L5F00:
    /* 5F00  rcr     bp,1 */
    BP = rcr16(BP, 1);
L5F02:
    /* 5F02  test    ax,0FDD1h */
L5F05:
    /* 5F05  mov     ax,bx */
    AX = BX;
L5F07:
    /* 5F07  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5F08:
    /* 5F08  mov     ax,cx */
    AX = CX;
L5F0A:
    /* 5F0A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5F0B:
    /* 5F0B  mov     ax,bp */
    AX = BP;
L5F0D:
    /* 5F0D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5F0E:
    /* 5F0E  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x5F11)) != 0) return c;
L5F11:
    /* 5F11  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5F12:
    /* 5F12  mov     ax,word ptr [si-4] */
    AX = rw(pDS, SI + 0xFFFC);
L5F15:
    /* 5F15  add     ax,word ptr [si+8] */
    AX = add16(AX, rw(pDS, SI + 0x8), 0);
L5F18:
    /* 5F18  jno     short L5F1D */
    if (!OF) goto L5F1D;
L5F1A:
    /* 5F1A  rcr     ax,1 */
    AX = rcr16(AX, 1);
L5F1C:
    /* 5F1C  test    ax,0F8D1h */
L5F1F:
    /* 5F1F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5F20:
    /* 5F20  mov     ax,word ptr [si-2] */
    AX = rw(pDS, SI + 0xFFFE);
L5F23:
    /* 5F23  add     ax,word ptr [si+0Ah] */
    AX = add16(AX, rw(pDS, SI + 0xA), 0);
L5F26:
    /* 5F26  jno     short L5F2B */
    if (!OF) goto L5F2B;
L5F28:
    /* 5F28  rcr     ax,1 */
    AX = rcr16(AX, 1);
L5F2A:
    /* 5F2A  test    ax,0F8D1h */
L5F2D:
    /* 5F2D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5F2E:
    /* 5F2E  dec     dx */
    DX = dec16(DX);
L5F2F:
    /* 5F2F  jne     L5EDA */
    if (!ZF) goto L5EDA;
L5F31:
    /* 5F31  mov     bx,word ptr ds:[0B01Ah] */
    BX = rw(pDS, 0xB01A);
L5F35:
    /* 5F35  add     bx,word ptr ds:[0B04Ah] */
    BX = add16(BX, rw(pDS, 0xB04A), 0);
L5F39:
    /* 5F39  jno     short L5F3E */
    if (!OF) goto L5F3E;
L5F3B:
    /* 5F3B  rcr     bx,1 */
    BX = rcr16(BX, 1);
L5F3D:
    /* 5F3D  test    ax,0FBD1h */
L5F40:
    /* 5F40  mov     cx,word ptr ds:[0B01Ch] */
    CX = rw(pDS, 0xB01C);
L5F44:
    /* 5F44  add     cx,word ptr ds:[0B04Ch] */
    CX = add16(CX, rw(pDS, 0xB04C), 0);
L5F48:
    /* 5F48  jno     short L5F4D */
    if (!OF) goto L5F4D;
L5F4A:
    /* 5F4A  rcr     cx,1 */
    CX = rcr16(CX, 1);
L5F4C:
    /* 5F4C  test    ax,0F9D1h */
L5F4F:
    /* 5F4F  mov     bp,word ptr ds:[0B01Eh] */
    BP = rw(pDS, 0xB01E);
L5F53:
    /* 5F53  add     bp,word ptr ds:[0B04Eh] */
    BP = add16(BP, rw(pDS, 0xB04E), 0);
L5F57:
    /* 5F57  jno     short L5F5C */
    if (!OF) goto L5F5C;
L5F59:
    /* 5F59  rcr     bp,1 */
    BP = rcr16(BP, 1);
L5F5B:
    /* 5F5B  test    ax,0FDD1h */
L5F5E:
    /* 5F5E  mov     ax,bx */
    AX = BX;
L5F60:
    /* 5F60  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5F61:
    /* 5F61  mov     ax,cx */
    AX = CX;
L5F63:
    /* 5F63  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5F64:
    /* 5F64  mov     ax,bp */
    AX = BP;
L5F66:
    /* 5F66  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5F67:
    /* 5F67  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x5F6A)) != 0) return c;
L5F6A:
    /* 5F6A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5F6B:
    /* 5F6B  mov     ax,word ptr ds:[0B022h] */
    AX = rw(pDS, 0xB022);
L5F6E:
    /* 5F6E  add     ax,word ptr ds:[0B052h] */
    AX = add16(AX, rw(pDS, 0xB052), 0);
L5F72:
    /* 5F72  jno     short L5F77 */
    if (!OF) goto L5F77;
L5F74:
    /* 5F74  rcr     ax,1 */
    AX = rcr16(AX, 1);
L5F76:
    /* 5F76  test    ax,0F8D1h */
L5F79:
    /* 5F79  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5F7A:
    /* 5F7A  mov     ax,word ptr ds:[0B024h] */
    AX = rw(pDS, 0xB024);
L5F7D:
    /* 5F7D  add     ax,word ptr ds:[0B054h] */
    AX = add16(AX, rw(pDS, 0xB054), 0);
L5F81:
    /* 5F81  jno     short L5F86 */
    if (!OF) goto L5F86;
L5F83:
    /* 5F83  rcr     ax,1 */
    AX = rcr16(AX, 1);
L5F85:
    /* 5F85  test    ax,0F8D1h */
L5F88:
    /* 5F88  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5F89:
    /* 5F89  pop     si */
    SI = pop16();
L5F8A:
    /* 5F8A  push    si */
    push16(SI);
L5F8B:
    /* 5F8B  mov     di,VBUF0 */
    DI = 0x309;
L5F8E:
    /* 5F8E  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L5F92:
    /* 5F92  mov     bl,0 */
    BL = 0x0;
L5F94:
    /* 5F94  mov     bh,0FFh */
    BH = 0xFF;
L5F96:
    /* 5F96  mov     si,0B00Eh */
    SI = 0xB00E;
L5F99:
    /* 5F99  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5F9A:
    /* 5F9A  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5F9B:
    /* 5F9B  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5F9C:
    /* 5F9C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5F9D:
    /* 5F9D  or      bl,al */
    BL = (uint8_t)(BL | AL);
L5F9F:
    /* 5F9F  and     bh,al */
    BH = (uint8_t)(BH & AL);
L5FA1:
    /* 5FA1  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5FA2:
    /* 5FA2  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FA3:
    /* 5FA3  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FA4:
    /* 5FA4  mov     si,0B01Ah */
    SI = 0xB01A;
L5FA7:
    /* 5FA7  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FA8:
    /* 5FA8  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FA9:
    /* 5FA9  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FAA:
    /* 5FAA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5FAB:
    /* 5FAB  or      bl,al */
    BL = (uint8_t)(BL | AL);
L5FAD:
    /* 5FAD  and     bh,al */
    BH = (uint8_t)(BH & AL);
L5FAF:
    /* 5FAF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5FB0:
    /* 5FB0  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FB1:
    /* 5FB1  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FB2:
    /* 5FB2  mov     si,0B06Eh */
    SI = 0xB06E;
L5FB5:
    /* 5FB5  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FB6:
    /* 5FB6  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FB7:
    /* 5FB7  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FB8:
    /* 5FB8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5FB9:
    /* 5FB9  or      bl,al */
    BL = (uint8_t)(BL | AL);
L5FBB:
    /* 5FBB  and     bh,al */
    BH = (uint8_t)(BH & AL);
L5FBD:
    /* 5FBD  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5FBE:
    /* 5FBE  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FBF:
    /* 5FBF  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FC0:
    /* 5FC0  mov     si,0B062h */
    SI = 0xB062;
L5FC3:
    /* 5FC3  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FC4:
    /* 5FC4  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FC5:
    /* 5FC5  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FC6:
    /* 5FC6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5FC7:
    /* 5FC7  or      bl,al */
    BL = (uint8_t)(BL | AL);
L5FC9:
    /* 5FC9  and     bh,al */
    BH = (uint8_t)(BH & AL);
L5FCB:
    /* 5FCB  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5FCC:
    /* 5FCC  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FCD:
    /* 5FCD  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FCE:
    /* 5FCE  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L5FD2:
    /* 5FD2  mov     byte ptr ds:[CLIP_AND],bh */
    wb(pDS, 0x161B, BH);
L5FD6:
    /* 5FD6  mov     byte ptr ds:[CLIP_OR],bl */
    wb(pDS, 0x161A, BL);
L5FDA:
    /* 5FDA  call    _seg004_6129 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x6129), 0x5FDD)) != 0) return c;
L5FDD:
    /* 5FDD  pop     si */
    SI = pop16();
L5FDE:
    /* 5FDE  push    si */
    push16(SI);
L5FDF:
    /* 5FDF  mov     di,VBUF0 */
    DI = 0x309;
L5FE2:
    /* 5FE2  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L5FE6:
    /* 5FE6  mov     bl,0 */
    BL = 0x0;
L5FE8:
    /* 5FE8  mov     bh,0FFh */
    BH = 0xFF;
L5FEA:
    /* 5FEA  mov     si,0B01Ah */
    SI = 0xB01A;
L5FED:
    /* 5FED  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FEE:
    /* 5FEE  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FEF:
    /* 5FEF  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FF0:
    /* 5FF0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5FF1:
    /* 5FF1  or      bl,al */
    BL = (uint8_t)(BL | AL);
L5FF3:
    /* 5FF3  and     bh,al */
    BH = (uint8_t)(BH & AL);
L5FF5:
    /* 5FF5  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L5FF6:
    /* 5FF6  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FF7:
    /* 5FF7  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FF8:
    /* 5FF8  mov     si,0B026h */
    SI = 0xB026;
L5FFB:
    /* 5FFB  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FFC:
    /* 5FFC  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FFD:
    /* 5FFD  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L5FFE:
    /* 5FFE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5FFF:
    /* 5FFF  or      bl,al */
    BL = (uint8_t)(BL | AL);
L6001:
    /* 6001  and     bh,al */
    BH = (uint8_t)(BH & AL);
L6003:
    /* 6003  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6004:
    /* 6004  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6005:
    /* 6005  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6006:
    /* 6006  mov     si,0B032h */
    SI = 0xB032;
L6009:
    /* 6009  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L600A:
    /* 600A  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L600B:
    /* 600B  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L600C:
    /* 600C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L600D:
    /* 600D  or      bl,al */
    BL = (uint8_t)(BL | AL);
L600F:
    /* 600F  and     bh,al */
    BH = (uint8_t)(BH & AL);
L6011:
    /* 6011  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6012:
    /* 6012  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6013:
    /* 6013  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6014:
    /* 6014  mov     si,0B06Eh */
    SI = 0xB06E;
L6017:
    /* 6017  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6018:
    /* 6018  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6019:
    /* 6019  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L601A:
    /* 601A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L601B:
    /* 601B  or      bl,al */
    BL = (uint8_t)(BL | AL);
L601D:
    /* 601D  and     bh,al */
    BH = (uint8_t)(BH & AL);
L601F:
    /* 601F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6020:
    /* 6020  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6021:
    /* 6021  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6022:
    /* 6022  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L6026:
    /* 6026  mov     byte ptr ds:[CLIP_AND],bh */
    wb(pDS, 0x161B, BH);
L602A:
    /* 602A  mov     byte ptr ds:[CLIP_OR],bl */
    wb(pDS, 0x161A, BL);
L602E:
    /* 602E  call    _seg004_6129 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x6129), 0x6031)) != 0) return c;
L6031:
    /* 6031  pop     si */
    SI = pop16();
L6032:
    /* 6032  push    si */
    push16(SI);
L6033:
    /* 6033  mov     di,VBUF0 */
    DI = 0x309;
L6036:
    /* 6036  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L603A:
    /* 603A  mov     bl,0 */
    BL = 0x0;
L603C:
    /* 603C  mov     bh,0FFh */
    BH = 0xFF;
L603E:
    /* 603E  mov     si,0B062h */
    SI = 0xB062;
L6041:
    /* 6041  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6042:
    /* 6042  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6043:
    /* 6043  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6044:
    /* 6044  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L6045:
    /* 6045  or      bl,al */
    BL = (uint8_t)(BL | AL);
L6047:
    /* 6047  and     bh,al */
    BH = (uint8_t)(BH & AL);
L6049:
    /* 6049  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L604A:
    /* 604A  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L604B:
    /* 604B  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L604C:
    /* 604C  mov     si,0B06Eh */
    SI = 0xB06E;
L604F:
    /* 604F  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6050:
    /* 6050  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6051:
    /* 6051  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6052:
    /* 6052  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L6053:
    /* 6053  or      bl,al */
    BL = (uint8_t)(BL | AL);
L6055:
    /* 6055  and     bh,al */
    BH = (uint8_t)(BH & AL);
L6057:
    /* 6057  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6058:
    /* 6058  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6059:
    /* 6059  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L605A:
    /* 605A  mov     si,0B04Ah */
    SI = 0xB04A;
L605D:
    /* 605D  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L605E:
    /* 605E  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L605F:
    /* 605F  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6060:
    /* 6060  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L6061:
    /* 6061  or      bl,al */
    BL = (uint8_t)(BL | AL);
L6063:
    /* 6063  and     bh,al */
    BH = (uint8_t)(BH & AL);
L6065:
    /* 6065  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6066:
    /* 6066  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6067:
    /* 6067  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6068:
    /* 6068  mov     si,0B056h */
    SI = 0xB056;
L606B:
    /* 606B  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L606C:
    /* 606C  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L606D:
    /* 606D  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L606E:
    /* 606E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L606F:
    /* 606F  or      bl,al */
    BL = (uint8_t)(BL | AL);
L6071:
    /* 6071  and     bh,al */
    BH = (uint8_t)(BH & AL);
L6073:
    /* 6073  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6074:
    /* 6074  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6075:
    /* 6075  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6076:
    /* 6076  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L607A:
    /* 607A  mov     byte ptr ds:[CLIP_AND],bh */
    wb(pDS, 0x161B, BH);
L607E:
    /* 607E  mov     byte ptr ds:[CLIP_OR],bl */
    wb(pDS, 0x161A, BL);
L6082:
    /* 6082  call    _seg004_6129 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x6129), 0x6085)) != 0) return c;
L6085:
    /* 6085  pop     si */
    SI = pop16();
L6086:
    /* 6086  push    si */
    push16(SI);
L6087:
    /* 6087  mov     di,VBUF0 */
    DI = 0x309;
L608A:
    /* 608A  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L608E:
    /* 608E  mov     bl,0 */
    BL = 0x0;
L6090:
    /* 6090  mov     bh,0FFh */
    BH = 0xFF;
L6092:
    /* 6092  mov     si,0B06Eh */
    SI = 0xB06E;
L6095:
    /* 6095  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6096:
    /* 6096  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6097:
    /* 6097  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6098:
    /* 6098  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L6099:
    /* 6099  or      bl,al */
    BL = (uint8_t)(BL | AL);
L609B:
    /* 609B  and     bh,al */
    BH = (uint8_t)(BH & AL);
L609D:
    /* 609D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L609E:
    /* 609E  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L609F:
    /* 609F  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L60A0:
    /* 60A0  mov     si,0B032h */
    SI = 0xB032;
L60A3:
    /* 60A3  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L60A4:
    /* 60A4  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L60A5:
    /* 60A5  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L60A6:
    /* 60A6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L60A7:
    /* 60A7  or      bl,al */
    BL = (uint8_t)(BL | AL);
L60A9:
    /* 60A9  and     bh,al */
    BH = (uint8_t)(BH & AL);
L60AB:
    /* 60AB  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L60AC:
    /* 60AC  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L60AD:
    /* 60AD  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L60AE:
    /* 60AE  mov     si,0B03Eh */
    SI = 0xB03E;
L60B1:
    /* 60B1  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L60B2:
    /* 60B2  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L60B3:
    /* 60B3  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L60B4:
    /* 60B4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L60B5:
    /* 60B5  or      bl,al */
    BL = (uint8_t)(BL | AL);
L60B7:
    /* 60B7  and     bh,al */
    BH = (uint8_t)(BH & AL);
L60B9:
    /* 60B9  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L60BA:
    /* 60BA  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L60BB:
    /* 60BB  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L60BC:
    /* 60BC  mov     si,0B04Ah */
    SI = 0xB04A;
L60BF:
    /* 60BF  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L60C0:
    /* 60C0  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L60C1:
    /* 60C1  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L60C2:
    /* 60C2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L60C3:
    /* 60C3  or      bl,al */
    BL = (uint8_t)(BL | AL);
L60C5:
    /* 60C5  and     bh,al */
    BH = (uint8_t)(BH & AL);
L60C7:
    /* 60C7  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L60C8:
    /* 60C8  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L60C9:
    /* 60C9  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L60CA:
    /* 60CA  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L60CE:
    /* 60CE  mov     byte ptr ds:[CLIP_AND],bh */
    wb(pDS, 0x161B, BH);
L60D2:
    /* 60D2  mov     byte ptr ds:[CLIP_OR],bl */
    wb(pDS, 0x161A, BL);
L60D6:
    /* 60D6  call    _seg004_6129 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x6129), 0x60D9)) != 0) return c;
L60D9:
    /* 60D9  pop     si */
    SI = pop16();
L60DA:
    /* 60DA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L60DB:
    /* 60DB  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L60DC:
    /* 60DC  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L60E0: /* L60E0 */
    /* 60E0  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L60E4:
    /* 60E4  test    word ptr ds:[2930h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x2930) & 0xFFFF));
L60EA:
    /* 60EA  jne     short _draw_solid_tmap */
    if (!ZF) goto L60F5;
L60EC:
    /* 60EC  call    _seg004_6129 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x6129), 0x60EF)) != 0) return c;
L60EF: /* L60EF */
    /* 60EF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L60F0:
    /* 60F0  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L60F1:
    /* 60F1  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_60F5  (+60F5)
       draw_solid_tmap: draw the polygon just collected as a flat polygon instead (polym nonzero,
       texture mapping turned off). Compacts vbuf's 12-byte vertices to the 8-byte form the flat
       polygon code reads, dropping u and v, recomputes codes_or and codes_and, and jumps to
       INTERP.ASM's draw_poly_buf_ptr, which ends the opcode. */
L60F5: /* _draw_solid_tmap */
    /* 60F5  push    si */
    push16(SI);
L60F6:
    /* 60F6  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L60FA:
    /* 60FA  mov     di,VBUF0 */
    DI = 0x309;
L60FD:
    /* 60FD  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L6101:
    /* 6101  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L6106:
    /* 6106  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L610B: /* L610B */
    /* 610B  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L610C:
    /* 610C  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L610D:
    /* 610D  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L610E:
    /* 610E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L610F:
    /* 610F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6110:
    /* 6110  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L6114:
    /* 6114  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L6118:
    /* 6118  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L611B:
    /* 611B  cmp     si,word ptr ds:[VB_END] */
    sub16(SI, rw(pDS, 0x305), 0);
L611F:
    /* 611F  jne     L610B */
    if (!ZF) goto L610B;
L6121:
    /* 6121  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L6125:
    /* 6125  pop     si */
    SI = pop16();
L6126:
    /* 6126  jmp     draw_poly_buf_ptr */
    return ASM_JMP(0x06E7, 0x2EBE);

    /* seg004_6129  (+6129)
       UW1 only: seg004_6130's entry that first installs this module's divide overflow handler
       (L692C). */
L6129: /* _seg004_6129 */
    /* 6129  mov     word ptr ss:[4D5h],offset L692C */
    ww(pSS, 0x4D5, 0x692C);

    /* seg004_6130  (+6130)
       UW1 only: draws the textured polygon collected at 307h..305h. When no vertex needs
       clipping (or byte 286Eh is set) it projects each vertex into the 14-byte records at
       seg048:659h, calls the mapper through the far pointer at 0B006h with the count at 0B07Ah,
       and then smooth_over (SMOOTH.ASM) when the low byte of 2928h is set. When every vertex is
       off one edge it draws nothing; otherwise it clips the polygon against the planes its codes
       name (seg004_624B for code 4, 63C6 for 8, 66C2 for 1, 6545 for 2) and tries again. */
L6130: /* _seg004_6130 */
    /* 6130  test    byte ptr ds:[286Eh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x286E) & 0xFF));
L6135:
    /* 6135  jne     short L6148 */
    if (!ZF) goto L6148;
L6137:
    /* 6137  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L613C:
    /* 613C  je      short L6141 */
    if (ZF) goto L6141;
L613E:
    /* 613E  jmp     L61FB */
    goto L61FB;

    /* seg004_6141  (+6141)
       seg004_6141 (FM Towns has it unnamed, at draw_solid_tmap+53h): texture map the polygon in
       vbuf unless codes_and says it is wholly off one edge. CX = the vertex count ((vert_ptr -
       buf_ptr) / 12; a remainder breaks with int 2), BX = bmap_blk_ptr, then
       asm_texture_map_from_draw_poly (PROJPOLY.ASM). Keeps every register (pusha/popa). */
L6141: /* L6141 */
    /* 6141  test    byte ptr ds:[CLIP_OR],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0xFF));
L6146:
    /* 6146  jne     short L61AA */
    if (!ZF) goto L61AA;
L6148: /* L6148 */
    /* 6148  push    si */
    push16(SI);
L6149:
    /* 6149  push    es */
    push16(asm_es);
L614A:
    /* 614A  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L614D:
    /* 614D  mov     es,ax */
    SET_ES(AX);
L614F:
    /* 614F  mov     di,659h */
    DI = 0x659;
L6152:
    /* 6152  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L6156:
    /* 6156  xor     cx,cx */
    CX = (uint16_t)(CX ^ CX);
L6158: /* L6158 */
    /* 6158  mov     bp,word ptr [si+4] */
    BP = rw(pDS, SI + 0x4);
L615B:
    /* 615B  mov     word ptr es:[di+0Ch],bp */
    ww(pES, DI + 0xC, BP);
L615F:
    /* 615F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L6160:
    /* 6160  mov     word ptr es:[di+8],ax */
    ww(pES, DI + 0x8, AX);
L6164:
    /* 6164  imul    word ptr ds:[PROJ_X] */
    imul16(rw(pDS, 0x26B2));
L6168:
    /* 6168  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x6168, 2)) != 0) return c;
L616A:
    /* 616A  add     ax,word ptr ds:[CENTRE_X] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B4));
L616E:
    /* 616E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L616F:
    /* 616F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L6170:
    /* 6170  mov     word ptr es:[di+8],ax */
    ww(pES, DI + 0x8, AX);
L6174:
    /* 6174  imul    word ptr ds:[PROJ_Y] */
    imul16(rw(pDS, 0x26B0));
L6178:
    /* 6178  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x6178, 2)) != 0) return c;
L617A:
    /* 617A  add     ax,word ptr ds:[CENTRE_Y] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B6));
L617E:
    /* 617E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L617F:
    /* 617F  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L6182:
    /* 6182  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6183:
    /* 6183  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6184:
    /* 6184  add     di,6 */
    DI = (uint16_t)(DI + 0x6);
L6187:
    /* 6187  inc     cx */
    CX = (uint16_t)(CX + 1);
L6188:
    /* 6188  cmp     si,word ptr ds:[VB_END] */
    sub16(SI, rw(pDS, 0x305), 0);
L618C:
    /* 618C  jne     L6158 */
    if (!ZF) goto L6158;
L618E:
    /* 618E  pop     es */
    SET_ES(pop16());
L618F:
    /* 618F  mov     word ptr ds:[0B07Ah],cx */
    ww(pDS, 0xB07A, CX);
L6193:
    /* 6193  call    dword ptr ds:[0B006h] */
    { uint16_t o_ = rw(pDS, (uint16_t)(0xB006)), s_ = rw(pDS, (uint16_t)(0xB006 + 2));
      if ((c = asm_callf(ASM_JMP((uint16_t)(s_ - PORT_LOAD_SEG), o_), 0x06E7 + PORT_LOAD_SEG, 0x6197)) != 0) return c; }
L6197:
    /* 6197  test    word ptr ds:[2928h],0FFh */
    logic16((uint16_t)(rw(pDS, 0x2928) & 0xFF));
L619D:
    /* 619D  jne     short L61A1 */
    if (!ZF) goto L61A1;
L619F: /* L619F */
    /* 619F  pop     si */
    SI = pop16();
L61A0:
    /* 61A0  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L61A1: /* L61A1 */
    /* 61A1  mov     cx,word ptr ds:[0B07Ah] */
    CX = rw(pDS, 0xB07A);
L61A5:
    /* 61A5  call    _smooth_over */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x0930), 0x61A8)) != 0) return c;
L61A8:
    /* 61A8  jmp     L619F */
    goto L619F;
L61AA: /* L61AA */
    /* 61AA  push    si */
    push16(SI);
L61AB:
    /* 61AB  test    byte ptr ds:[CLIP_OR],4 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x4));
L61B0:
    /* 61B0  je      short L61BC */
    if (ZF) goto L61BC;
L61B2:
    /* 61B2  call    _seg004_624B */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x624B), 0x61B5)) != 0) return c;
L61B5:
    /* 61B5  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L61BA:
    /* 61BA  jne     short L61FA */
    if (!ZF) goto L61FA;
L61BC: /* L61BC */
    /* 61BC  test    byte ptr ds:[CLIP_OR],8 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x8));
L61C1:
    /* 61C1  je      short L61CD */
    if (ZF) goto L61CD;
L61C3:
    /* 61C3  call    _seg004_63C6 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x63C6), 0x61C6)) != 0) return c;
L61C6:
    /* 61C6  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L61CB:
    /* 61CB  jne     short L61FA */
    if (!ZF) goto L61FA;
L61CD: /* L61CD */
    /* 61CD  test    byte ptr ds:[CLIP_OR],1 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x1));
L61D2:
    /* 61D2  je      short L61DE */
    if (ZF) goto L61DE;
L61D4:
    /* 61D4  call    _seg004_66C2 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x66C2), 0x61D7)) != 0) return c;
L61D7:
    /* 61D7  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L61DC:
    /* 61DC  jne     short L61FA */
    if (!ZF) goto L61FA;
L61DE: /* L61DE */
    /* 61DE  test    byte ptr ds:[CLIP_OR],2 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x2));
L61E3:
    /* 61E3  je      short L61E8 */
    if (ZF) goto L61E8;
L61E5:
    /* 61E5  call    _seg004_6545 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x6545), 0x61E8)) != 0) return c;
L61E8: /* L61E8 */
    /* 61E8  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L61ED:
    /* 61ED  jne     short L61FA */
    if (!ZF) goto L61FA;
L61EF:
    /* 61EF  test    byte ptr ds:[CLIP_OR],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0xFF));
L61F4:
    /* 61F4  jne     short L61FC */
    if (!ZF) goto L61FC;
L61F6:
    /* 61F6  pop     si */
    SI = pop16();
L61F7:
    /* 61F7  jmp     L6148 */
    goto L6148;
L61FA: /* L61FA */
    /* 61FA  pop     si */
    SI = pop16();
L61FB: /* L61FB */
    /* 61FB  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L61FC: /* L61FC */
    /* 61FC  test    byte ptr ds:[CLIP_OR],4 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x4));
L6201:
    /* 6201  je      short L620D */
    if (ZF) goto L620D;
L6203:
    /* 6203  call    _seg004_624B */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x624B), 0x6206)) != 0) return c;
L6206:
    /* 6206  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L620B:
    /* 620B  jne     L61FA */
    if (!ZF) goto L61FA;
L620D: /* L620D */
    /* 620D  test    byte ptr ds:[CLIP_OR],8 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x8));
L6212:
    /* 6212  je      short L621E */
    if (ZF) goto L621E;
L6214:
    /* 6214  call    _seg004_63C6 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x63C6), 0x6217)) != 0) return c;
L6217:
    /* 6217  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L621C:
    /* 621C  jne     L61FA */
    if (!ZF) goto L61FA;
L621E: /* L621E */
    /* 621E  test    byte ptr ds:[CLIP_OR],1 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x1));
L6223:
    /* 6223  je      short L622F */
    if (ZF) goto L622F;
L6225:
    /* 6225  call    _seg004_66C2 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x66C2), 0x6228)) != 0) return c;
L6228:
    /* 6228  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L622D:
    /* 622D  jne     L61FA */
    if (!ZF) goto L61FA;
L622F: /* L622F */
    /* 622F  test    byte ptr ds:[CLIP_OR],2 */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0x2));
L6234:
    /* 6234  je      short L6239 */
    if (ZF) goto L6239;
L6236:
    /* 6236  call    _seg004_6545 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x6545), 0x6239)) != 0) return c;
L6239: /* L6239 */
    /* 6239  test    byte ptr ds:[CLIP_AND],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161B) & 0xFF));
L623E:
    /* 623E  jne     L61FA */
    if (!ZF) goto L61FA;
L6240:
    /* 6240  test    byte ptr ds:[CLIP_OR],0FFh */
    logic8((uint8_t)(rb(pDS, 0x161A) & 0xFF));
L6245:
    /* 6245  jne     L61FA */
    if (!ZF) goto L61FA;
L6247:
    /* 6247  pop     si */
    SI = pop16();
L6248:
    /* 6248  jmp     L6148 */
    goto L6148;

    /* seg004_624B  (+624B)
       UW1 only: clips the textured polygon against the plane of clip code 4, interpolating u and
       v as well; the counterpart of INTERP.ASM's clip_top. */
L624B: /* _seg004_624B */
    /* 624B  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L6250:
    /* 6250  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L6255:
    /* 6255  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L6259:
    /* 6259  mov     di,word ptr ds:[VB_END] */
    DI = rw(pDS, 0x305);
L625D:
    /* 625D  mov     cx,0Ch */
    CX = 0xC;
L6260:
    /* 6260  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L6262:
    /* 6262  add     word ptr ds:[VB_END],0Ch */
    ww(pDS, 0x305, (uint16_t)(rw(pDS, 0x305) + 0xC));
L6267:
    /* 6267  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L626B:
    /* 626B  mov     di,VBUF1 */
    DI = 0xC69;
L626E:
    /* 626E  cmp     si,VBUF0 */
    sub16(SI, 0x309, 0);
L6272:
    /* 6272  je      short L6277 */
    if (ZF) goto L6277;
L6274:
    /* 6274  mov     di,VBUF0 */
    DI = 0x309;
L6277: /* L6277 */
    /* 6277  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L627B: /* L627B */
    /* 627B  add     si,0Ch */
    SI = (uint16_t)(SI + 0xC);
L627E: /* L627E */
    /* 627E  cmp     si,word ptr ds:[VB_END] */
    sub16(SI, rw(pDS, 0x305), 0);
L6282:
    /* 6282  jne     short L6287 */
    if (!ZF) goto L6287;
L6284:
    /* 6284  jmp     L63C1 */
    goto L63C1;
L6287: /* L6287 */
    /* 6287  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L628A:
    /* 628A  test    al,4 */
    logic8((uint8_t)(AL & 0x4));
L628C:
    /* 628C  jne     short L629D */
    if (!ZF) goto L629D;
L628E:
    /* 628E  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L6292:
    /* 6292  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L6296:
    /* 6296  mov     cx,6 */
    CX = 0x6;
L6299:
    /* 6299  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L629B:
    /* 629B  jmp     L627E */
    goto L627E;
L629D: /* L629D */
    /* 629D  test    byte ptr [si-6],4 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFA) & 0x4));
L62A1:
    /* 62A1  je      short L62A6 */
    if (ZF) goto L62A6;
L62A3:
    /* 62A3  jmp     L632E */
    goto L632E;
L62A6: /* L62A6 */
    /* 62A6  mov     bp,word ptr [si-8] */
    BP = rw(pDS, SI + 0xFFF8);
L62A9:
    /* 62A9  sub     bp,word ptr [si-0Ah] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0xFFF6));
L62AC:
    /* 62AC  mov     cx,bp */
    CX = BP;
L62AE:
    /* 62AE  sub     cx,word ptr [si+4] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x4));
L62B1:
    /* 62B1  add     cx,word ptr [si+2] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0x2));
L62B4:
    /* 62B4  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L62B6:
    /* 62B6  sub     ax,word ptr [si-0Ch] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF4));
L62B9:
    /* 62B9  imul    bp */
    imul16(BP);
L62BB:
    /* 62BB  shl     ax,1 */
    AX = shl16(AX, 1);
L62BD:
    /* 62BD  rcl     dx,1 */
    DX = rcl16(DX, 1);
L62BF:
    /* 62BF  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x62BF, 2)) != 0) return c;
L62C1:
    /* 62C1  sar     ax,1 */
    AX = sar16(AX, 1);
L62C3:
    /* 62C3  jae     short L62C8 */
    if (!CF) goto L62C8;
L62C5:
    /* 62C5  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L62C6:
    /* 62C6  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L62C8: /* L62C8 */
    /* 62C8  add     ax,word ptr [si-0Ch] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF4));
L62CB:
    /* 62CB  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L62CC:
    /* 62CC  mov     bx,ax */
    BX = AX;
L62CE:
    /* 62CE  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L62D1:
    /* 62D1  sub     ax,word ptr [si-0Ah] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF6));
L62D4:
    /* 62D4  imul    bp */
    imul16(BP);
L62D6:
    /* 62D6  shl     ax,1 */
    AX = shl16(AX, 1);
L62D8:
    /* 62D8  rcl     dx,1 */
    DX = rcl16(DX, 1);
L62DA:
    /* 62DA  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x62DA, 2)) != 0) return c;
L62DC:
    /* 62DC  sar     ax,1 */
    AX = sar16(AX, 1);
L62DE:
    /* 62DE  jae     short L62E3 */
    if (!CF) goto L62E3;
L62E0:
    /* 62E0  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L62E1:
    /* 62E1  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L62E3: /* L62E3 */
    /* 62E3  add     ax,word ptr [si-0Ah] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF6));
L62E6:
    /* 62E6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L62E7:
    /* 62E7  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L62E8:
    /* 62E8  push    cx */
    push16(CX);
L62E9:
    /* 62E9  push    bp */
    push16(BP);
L62EA:
    /* 62EA  mov     cx,ax */
    CX = AX;
L62EC:
    /* 62EC  mov     bp,ax */
    BP = AX;
L62EE:
    /* 62EE  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x62F1)) != 0) return c;
L62F1:
    /* 62F1  pop     bp */
    BP = pop16();
L62F2:
    /* 62F2  pop     cx */
    CX = pop16();
L62F3:
    /* 62F3  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L62F4:
    /* 62F4  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L62F8:
    /* 62F8  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L62FC:
    /* 62FC  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L62FF:
    /* 62FF  sub     ax,word ptr [si-4] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFC));
L6302:
    /* 6302  imul    bp */
    imul16(BP);
L6304:
    /* 6304  shl     ax,1 */
    AX = shl16(AX, 1);
L6306:
    /* 6306  rcl     dx,1 */
    DX = rcl16(DX, 1);
L6308:
    /* 6308  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x6308, 2)) != 0) return c;
L630A:
    /* 630A  sar     ax,1 */
    AX = sar16(AX, 1);
L630C:
    /* 630C  jae     short L6311 */
    if (!CF) goto L6311;
L630E:
    /* 630E  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L630F:
    /* 630F  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L6311: /* L6311 */
    /* 6311  add     ax,word ptr [si-4] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFC));
L6314:
    /* 6314  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6315:
    /* 6315  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L6318:
    /* 6318  sub     ax,word ptr [si-2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFE));
L631B:
    /* 631B  imul    bp */
    imul16(BP);
L631D:
    /* 631D  shl     ax,1 */
    AX = shl16(AX, 1);
L631F:
    /* 631F  rcl     dx,1 */
    DX = rcl16(DX, 1);
L6321:
    /* 6321  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x6321, 2)) != 0) return c;
L6323:
    /* 6323  sar     ax,1 */
    AX = sar16(AX, 1);
L6325:
    /* 6325  jae     short L632A */
    if (!CF) goto L632A;
L6327:
    /* 6327  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L6328:
    /* 6328  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L632A: /* L632A */
    /* 632A  add     ax,word ptr [si-2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFE));
L632D:
    /* 632D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L632E: /* L632E */
    /* 632E  test    byte ptr [si+12h],4 */
    logic8((uint8_t)(rb(pDS, SI + 0x12) & 0x4));
L6332:
    /* 6332  je      short L6337 */
    if (ZF) goto L6337;
L6334:
    /* 6334  jmp     L627B */
    goto L627B;
L6337: /* L6337 */
    /* 6337  mov     bp,word ptr [si+4] */
    BP = rw(pDS, SI + 0x4);
L633A:
    /* 633A  sub     bp,word ptr [si+2] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0x2));
L633D:
    /* 633D  mov     cx,bp */
    CX = BP;
L633F:
    /* 633F  add     cx,word ptr [si+0Eh] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0xE));
L6342:
    /* 6342  sub     cx,word ptr [si+10h] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x10));
L6345:
    /* 6345  mov     ax,word ptr [si+0Ch] */
    AX = rw(pDS, SI + 0xC);
L6348:
    /* 6348  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L634A:
    /* 634A  imul    bp */
    imul16(BP);
L634C:
    /* 634C  shl     ax,1 */
    AX = shl16(AX, 1);
L634E:
    /* 634E  rcl     dx,1 */
    DX = rcl16(DX, 1);
L6350:
    /* 6350  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x6350, 2)) != 0) return c;
L6352:
    /* 6352  sar     ax,1 */
    AX = sar16(AX, 1);
L6354:
    /* 6354  jae     short L6359 */
    if (!CF) goto L6359;
L6356:
    /* 6356  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L6357:
    /* 6357  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L6359: /* L6359 */
    /* 6359  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L635B:
    /* 635B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L635C:
    /* 635C  mov     bx,ax */
    BX = AX;
L635E:
    /* 635E  mov     ax,word ptr [si+0Eh] */
    AX = rw(pDS, SI + 0xE);
L6361:
    /* 6361  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L6364:
    /* 6364  imul    bp */
    imul16(BP);
L6366:
    /* 6366  shl     ax,1 */
    AX = shl16(AX, 1);
L6368:
    /* 6368  rcl     dx,1 */
    DX = rcl16(DX, 1);
L636A:
    /* 636A  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x636A, 2)) != 0) return c;
L636C:
    /* 636C  sar     ax,1 */
    AX = sar16(AX, 1);
L636E:
    /* 636E  jae     short L6373 */
    if (!CF) goto L6373;
L6370:
    /* 6370  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L6371:
    /* 6371  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L6373: /* L6373 */
    /* 6373  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L6376:
    /* 6376  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6377:
    /* 6377  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6378:
    /* 6378  push    cx */
    push16(CX);
L6379:
    /* 6379  push    bp */
    push16(BP);
L637A:
    /* 637A  mov     cx,ax */
    CX = AX;
L637C:
    /* 637C  mov     bp,ax */
    BP = AX;
L637E:
    /* 637E  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x6381)) != 0) return c;
L6381:
    /* 6381  pop     bp */
    BP = pop16();
L6382:
    /* 6382  pop     cx */
    CX = pop16();
L6383:
    /* 6383  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6384:
    /* 6384  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L6388:
    /* 6388  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L638C:
    /* 638C  mov     ax,word ptr [si+14h] */
    AX = rw(pDS, SI + 0x14);
L638F:
    /* 638F  sub     ax,word ptr [si+8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x8));
L6392:
    /* 6392  imul    bp */
    imul16(BP);
L6394:
    /* 6394  shl     ax,1 */
    AX = shl16(AX, 1);
L6396:
    /* 6396  rcl     dx,1 */
    DX = rcl16(DX, 1);
L6398:
    /* 6398  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x6398, 2)) != 0) return c;
L639A:
    /* 639A  sar     ax,1 */
    AX = sar16(AX, 1);
L639C:
    /* 639C  jae     short L63A1 */
    if (!CF) goto L63A1;
L639E:
    /* 639E  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L639F:
    /* 639F  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L63A1: /* L63A1 */
    /* 63A1  add     ax,word ptr [si+8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x8));
L63A4:
    /* 63A4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L63A5:
    /* 63A5  mov     ax,word ptr [si+16h] */
    AX = rw(pDS, SI + 0x16);
L63A8:
    /* 63A8  sub     ax,word ptr [si+0Ah] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xA));
L63AB:
    /* 63AB  imul    bp */
    imul16(BP);
L63AD:
    /* 63AD  shl     ax,1 */
    AX = shl16(AX, 1);
L63AF:
    /* 63AF  rcl     dx,1 */
    DX = rcl16(DX, 1);
L63B1:
    /* 63B1  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x63B1, 2)) != 0) return c;
L63B3:
    /* 63B3  sar     ax,1 */
    AX = sar16(AX, 1);
L63B5:
    /* 63B5  jae     short L63BA */
    if (!CF) goto L63BA;
L63B7:
    /* 63B7  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L63B8:
    /* 63B8  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L63BA: /* L63BA */
    /* 63BA  add     ax,word ptr [si+0Ah] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xA));
L63BD:
    /* 63BD  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L63BE:
    /* 63BE  jmp     L627B */
    goto L627B;
L63C1: /* L63C1 */
    /* 63C1  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L63C5:
    /* 63C5  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_63C6  (+63C6)
       UW1 only: as seg004_624B for clip code 8 (INTERP.ASM's clip_bot). */
L63C6: /* _seg004_63C6 */
    /* 63C6  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L63CB:
    /* 63CB  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L63D0:
    /* 63D0  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L63D4:
    /* 63D4  mov     di,word ptr ds:[VB_END] */
    DI = rw(pDS, 0x305);
L63D8:
    /* 63D8  mov     cx,0Ch */
    CX = 0xC;
L63DB:
    /* 63DB  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L63DD:
    /* 63DD  add     word ptr ds:[VB_END],0Ch */
    ww(pDS, 0x305, (uint16_t)(rw(pDS, 0x305) + 0xC));
L63E2:
    /* 63E2  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L63E6:
    /* 63E6  mov     di,VBUF1 */
    DI = 0xC69;
L63E9:
    /* 63E9  cmp     si,VBUF0 */
    sub16(SI, 0x309, 0);
L63ED:
    /* 63ED  je      short L63F2 */
    if (ZF) goto L63F2;
L63EF:
    /* 63EF  mov     di,VBUF0 */
    DI = 0x309;
L63F2: /* L63F2 */
    /* 63F2  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L63F6: /* L63F6 */
    /* 63F6  add     si,0Ch */
    SI = (uint16_t)(SI + 0xC);
L63F9: /* L63F9 */
    /* 63F9  cmp     si,word ptr ds:[VB_END] */
    sub16(SI, rw(pDS, 0x305), 0);
L63FD:
    /* 63FD  jne     short L6402 */
    if (!ZF) goto L6402;
L63FF:
    /* 63FF  jmp     L6540 */
    goto L6540;
L6402: /* L6402 */
    /* 6402  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L6405:
    /* 6405  test    al,8 */
    logic8((uint8_t)(AL & 0x8));
L6407:
    /* 6407  jne     short L6418 */
    if (!ZF) goto L6418;
L6409:
    /* 6409  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L640D:
    /* 640D  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L6411:
    /* 6411  mov     cx,6 */
    CX = 0x6;
L6414:
    /* 6414  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L6416:
    /* 6416  jmp     L63F9 */
    goto L63F9;
L6418: /* L6418 */
    /* 6418  test    byte ptr [si-6],8 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFA) & 0x8));
L641C:
    /* 641C  je      short L6421 */
    if (ZF) goto L6421;
L641E:
    /* 641E  jmp     L64AB */
    goto L64AB;
L6421: /* L6421 */
    /* 6421  mov     bp,word ptr [si-8] */
    BP = rw(pDS, SI + 0xFFF8);
L6424:
    /* 6424  add     bp,word ptr [si-0Ah] */
    BP = (uint16_t)(BP + rw(pDS, SI + 0xFFF6));
L6427:
    /* 6427  mov     cx,bp */
    CX = BP;
L6429:
    /* 6429  sub     cx,word ptr [si+4] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x4));
L642C:
    /* 642C  sub     cx,word ptr [si+2] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x2));
L642F:
    /* 642F  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L6431:
    /* 6431  sub     ax,word ptr [si-0Ch] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF4));
L6434:
    /* 6434  imul    bp */
    imul16(BP);
L6436:
    /* 6436  shl     ax,1 */
    AX = shl16(AX, 1);
L6438:
    /* 6438  rcl     dx,1 */
    DX = rcl16(DX, 1);
L643A:
    /* 643A  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x643A, 2)) != 0) return c;
L643C:
    /* 643C  sar     ax,1 */
    AX = sar16(AX, 1);
L643E:
    /* 643E  jae     short L6443 */
    if (!CF) goto L6443;
L6440:
    /* 6440  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L6441:
    /* 6441  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L6443: /* L6443 */
    /* 6443  add     ax,word ptr [si-0Ch] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF4));
L6446:
    /* 6446  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6447:
    /* 6447  mov     bx,ax */
    BX = AX;
L6449:
    /* 6449  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L644C:
    /* 644C  sub     ax,word ptr [si-0Ah] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF6));
L644F:
    /* 644F  imul    bp */
    imul16(BP);
L6451:
    /* 6451  shl     ax,1 */
    AX = shl16(AX, 1);
L6453:
    /* 6453  rcl     dx,1 */
    DX = rcl16(DX, 1);
L6455:
    /* 6455  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x6455, 2)) != 0) return c;
L6457:
    /* 6457  sar     ax,1 */
    AX = sar16(AX, 1);
L6459:
    /* 6459  jae     short L645E */
    if (!CF) goto L645E;
L645B:
    /* 645B  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L645C:
    /* 645C  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L645E: /* L645E */
    /* 645E  add     ax,word ptr [si-0Ah] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF6));
L6461:
    /* 6461  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6462:
    /* 6462  push    bp */
    push16(BP);
L6463:
    /* 6463  push    cx */
    push16(CX);
L6464:
    /* 6464  mov     cx,ax */
    CX = AX;
L6466:
    /* 6466  neg     ax */
    AX = (uint16_t)-AX;
L6468:
    /* 6468  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6469:
    /* 6469  mov     bp,ax */
    BP = AX;
L646B:
    /* 646B  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x646E)) != 0) return c;
L646E:
    /* 646E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L646F:
    /* 646F  pop     cx */
    CX = pop16();
L6470:
    /* 6470  pop     bp */
    BP = pop16();
L6471:
    /* 6471  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L6475:
    /* 6475  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L6479:
    /* 6479  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L647C:
    /* 647C  sub     ax,word ptr [si-4] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFC));
L647F:
    /* 647F  imul    bp */
    imul16(BP);
L6481:
    /* 6481  shl     ax,1 */
    AX = shl16(AX, 1);
L6483:
    /* 6483  rcl     dx,1 */
    DX = rcl16(DX, 1);
L6485:
    /* 6485  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x6485, 2)) != 0) return c;
L6487:
    /* 6487  sar     ax,1 */
    AX = sar16(AX, 1);
L6489:
    /* 6489  jae     short L648E */
    if (!CF) goto L648E;
L648B:
    /* 648B  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L648C:
    /* 648C  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L648E: /* L648E */
    /* 648E  add     ax,word ptr [si-4] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFC));
L6491:
    /* 6491  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6492:
    /* 6492  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L6495:
    /* 6495  sub     ax,word ptr [si-2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFE));
L6498:
    /* 6498  imul    bp */
    imul16(BP);
L649A:
    /* 649A  shl     ax,1 */
    AX = shl16(AX, 1);
L649C:
    /* 649C  rcl     dx,1 */
    DX = rcl16(DX, 1);
L649E:
    /* 649E  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x649E, 2)) != 0) return c;
L64A0:
    /* 64A0  sar     ax,1 */
    AX = sar16(AX, 1);
L64A2:
    /* 64A2  jae     short L64A7 */
    if (!CF) goto L64A7;
L64A4:
    /* 64A4  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L64A5:
    /* 64A5  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L64A7: /* L64A7 */
    /* 64A7  add     ax,word ptr [si-2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFE));
L64AA:
    /* 64AA  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L64AB: /* L64AB */
    /* 64AB  test    byte ptr [si+12h],8 */
    logic8((uint8_t)(rb(pDS, SI + 0x12) & 0x8));
L64AF:
    /* 64AF  je      short L64B4 */
    if (ZF) goto L64B4;
L64B1:
    /* 64B1  jmp     L63F6 */
    goto L63F6;
L64B4: /* L64B4 */
    /* 64B4  mov     bp,word ptr [si+4] */
    BP = rw(pDS, SI + 0x4);
L64B7:
    /* 64B7  add     bp,word ptr [si+2] */
    BP = (uint16_t)(BP + rw(pDS, SI + 0x2));
L64BA:
    /* 64BA  mov     cx,bp */
    CX = BP;
L64BC:
    /* 64BC  sub     cx,word ptr [si+0Eh] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xE));
L64BF:
    /* 64BF  sub     cx,word ptr [si+10h] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x10));
L64C2:
    /* 64C2  mov     ax,word ptr [si+0Ch] */
    AX = rw(pDS, SI + 0xC);
L64C5:
    /* 64C5  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L64C7:
    /* 64C7  imul    bp */
    imul16(BP);
L64C9:
    /* 64C9  shl     ax,1 */
    AX = shl16(AX, 1);
L64CB:
    /* 64CB  rcl     dx,1 */
    DX = rcl16(DX, 1);
L64CD:
    /* 64CD  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x64CD, 2)) != 0) return c;
L64CF:
    /* 64CF  sar     ax,1 */
    AX = sar16(AX, 1);
L64D1:
    /* 64D1  jae     short L64D6 */
    if (!CF) goto L64D6;
L64D3:
    /* 64D3  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L64D4:
    /* 64D4  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L64D6: /* L64D6 */
    /* 64D6  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L64D8:
    /* 64D8  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L64D9:
    /* 64D9  mov     bx,ax */
    BX = AX;
L64DB:
    /* 64DB  mov     ax,word ptr [si+0Eh] */
    AX = rw(pDS, SI + 0xE);
L64DE:
    /* 64DE  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L64E1:
    /* 64E1  imul    bp */
    imul16(BP);
L64E3:
    /* 64E3  shl     ax,1 */
    AX = shl16(AX, 1);
L64E5:
    /* 64E5  rcl     dx,1 */
    DX = rcl16(DX, 1);
L64E7:
    /* 64E7  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x64E7, 2)) != 0) return c;
L64E9:
    /* 64E9  sar     ax,1 */
    AX = sar16(AX, 1);
L64EB:
    /* 64EB  jae     short L64F0 */
    if (!CF) goto L64F0;
L64ED:
    /* 64ED  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L64EE:
    /* 64EE  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L64F0: /* L64F0 */
    /* 64F0  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L64F3:
    /* 64F3  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L64F4:
    /* 64F4  push    bp */
    push16(BP);
L64F5:
    /* 64F5  push    cx */
    push16(CX);
L64F6:
    /* 64F6  mov     cx,ax */
    CX = AX;
L64F8:
    /* 64F8  neg     ax */
    AX = (uint16_t)-AX;
L64FA:
    /* 64FA  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L64FB:
    /* 64FB  mov     bp,ax */
    BP = AX;
L64FD:
    /* 64FD  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x6500)) != 0) return c;
L6500:
    /* 6500  pop     cx */
    CX = pop16();
L6501:
    /* 6501  pop     bp */
    BP = pop16();
L6502:
    /* 6502  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6503:
    /* 6503  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L6507:
    /* 6507  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L650B:
    /* 650B  mov     ax,word ptr [si+14h] */
    AX = rw(pDS, SI + 0x14);
L650E:
    /* 650E  sub     ax,word ptr [si+8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x8));
L6511:
    /* 6511  imul    bp */
    imul16(BP);
L6513:
    /* 6513  shl     ax,1 */
    AX = shl16(AX, 1);
L6515:
    /* 6515  rcl     dx,1 */
    DX = rcl16(DX, 1);
L6517:
    /* 6517  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x6517, 2)) != 0) return c;
L6519:
    /* 6519  sar     ax,1 */
    AX = sar16(AX, 1);
L651B:
    /* 651B  jae     short L6520 */
    if (!CF) goto L6520;
L651D:
    /* 651D  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L651E:
    /* 651E  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L6520: /* L6520 */
    /* 6520  add     ax,word ptr [si+8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x8));
L6523:
    /* 6523  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6524:
    /* 6524  mov     ax,word ptr [si+16h] */
    AX = rw(pDS, SI + 0x16);
L6527:
    /* 6527  sub     ax,word ptr [si+0Ah] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xA));
L652A:
    /* 652A  imul    bp */
    imul16(BP);
L652C:
    /* 652C  shl     ax,1 */
    AX = shl16(AX, 1);
L652E:
    /* 652E  rcl     dx,1 */
    DX = rcl16(DX, 1);
L6530:
    /* 6530  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x6530, 2)) != 0) return c;
L6532:
    /* 6532  sar     ax,1 */
    AX = sar16(AX, 1);
L6534:
    /* 6534  jae     short L6539 */
    if (!CF) goto L6539;
L6536:
    /* 6536  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L6537:
    /* 6537  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L6539: /* L6539 */
    /* 6539  add     ax,word ptr [si+0Ah] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xA));
L653C:
    /* 653C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L653D:
    /* 653D  jmp     L63F6 */
    goto L63F6;
L6540: /* L6540 */
    /* 6540  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L6544:
    /* 6544  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_6545  (+6545)
       UW1 only: as seg004_624B for clip code 2 (INTERP.ASM's clip_right). */
L6545: /* _seg004_6545 */
    /* 6545  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L654A:
    /* 654A  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L654F:
    /* 654F  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L6553:
    /* 6553  mov     di,word ptr ds:[VB_END] */
    DI = rw(pDS, 0x305);
L6557:
    /* 6557  mov     cx,0Ch */
    CX = 0xC;
L655A:
    /* 655A  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L655C:
    /* 655C  add     word ptr ds:[VB_END],0Ch */
    ww(pDS, 0x305, (uint16_t)(rw(pDS, 0x305) + 0xC));
L6561:
    /* 6561  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L6565:
    /* 6565  mov     di,VBUF1 */
    DI = 0xC69;
L6568:
    /* 6568  cmp     si,VBUF0 */
    sub16(SI, 0x309, 0);
L656C:
    /* 656C  je      short L6571 */
    if (ZF) goto L6571;
L656E:
    /* 656E  mov     di,VBUF0 */
    DI = 0x309;
L6571: /* L6571 */
    /* 6571  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L6575: /* L6575 */
    /* 6575  add     si,0Ch */
    SI = (uint16_t)(SI + 0xC);
L6578: /* L6578 */
    /* 6578  cmp     si,word ptr ds:[VB_END] */
    sub16(SI, rw(pDS, 0x305), 0);
L657C:
    /* 657C  jne     short L6581 */
    if (!ZF) goto L6581;
L657E:
    /* 657E  jmp     L66BD */
    goto L66BD;
L6581: /* L6581 */
    /* 6581  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L6584:
    /* 6584  test    al,2 */
    logic8((uint8_t)(AL & 0x2));
L6586:
    /* 6586  jne     short L6597 */
    if (!ZF) goto L6597;
L6588:
    /* 6588  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L658C:
    /* 658C  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L6590:
    /* 6590  mov     cx,6 */
    CX = 0x6;
L6593:
    /* 6593  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L6595:
    /* 6595  jmp     L6578 */
    goto L6578;
L6597: /* L6597 */
    /* 6597  test    byte ptr [si-6],2 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFA) & 0x2));
L659B:
    /* 659B  je      short L65A0 */
    if (ZF) goto L65A0;
L659D:
    /* 659D  jmp     L6629 */
    goto L6629;
L65A0: /* L65A0 */
    /* 65A0  mov     bp,word ptr [si-0Ch] */
    BP = rw(pDS, SI + 0xFFF4);
L65A3:
    /* 65A3  sub     bp,word ptr [si-8] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0xFFF8));
L65A6:
    /* 65A6  mov     cx,bp */
    CX = BP;
L65A8:
    /* 65A8  add     cx,word ptr [si+4] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0x4));
L65AB:
    /* 65AB  sub     cx,word ptr [si] */
    CX = (uint16_t)(CX - rw(pDS, SI));
L65AD:
    /* 65AD  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L65AF:
    /* 65AF  sub     ax,word ptr [si-0Ch] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF4));
L65B2:
    /* 65B2  imul    bp */
    imul16(BP);
L65B4:
    /* 65B4  shl     ax,1 */
    AX = shl16(AX, 1);
L65B6:
    /* 65B6  rcl     dx,1 */
    DX = rcl16(DX, 1);
L65B8:
    /* 65B8  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x65B8, 2)) != 0) return c;
L65BA:
    /* 65BA  sar     ax,1 */
    AX = sar16(AX, 1);
L65BC:
    /* 65BC  jae     short L65C1 */
    if (!CF) goto L65C1;
L65BE:
    /* 65BE  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L65BF:
    /* 65BF  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L65C1: /* L65C1 */
    /* 65C1  add     ax,word ptr [si-0Ch] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF4));
L65C4:
    /* 65C4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L65C5:
    /* 65C5  mov     bx,ax */
    BX = AX;
L65C7:
    /* 65C7  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L65CA:
    /* 65CA  sub     ax,word ptr [si-0Ah] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF6));
L65CD:
    /* 65CD  imul    bp */
    imul16(BP);
L65CF:
    /* 65CF  shl     ax,1 */
    AX = shl16(AX, 1);
L65D1:
    /* 65D1  rcl     dx,1 */
    DX = rcl16(DX, 1);
L65D3:
    /* 65D3  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x65D3, 2)) != 0) return c;
L65D5:
    /* 65D5  sar     ax,1 */
    AX = sar16(AX, 1);
L65D7:
    /* 65D7  jae     short L65DC */
    if (!CF) goto L65DC;
L65D9:
    /* 65D9  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L65DA:
    /* 65DA  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L65DC: /* L65DC */
    /* 65DC  add     ax,word ptr [si-0Ah] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF6));
L65DF:
    /* 65DF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L65E0:
    /* 65E0  push    cx */
    push16(CX);
L65E1:
    /* 65E1  push    bp */
    push16(BP);
L65E2:
    /* 65E2  mov     cx,ax */
    CX = AX;
L65E4:
    /* 65E4  mov     ax,bx */
    AX = BX;
L65E6:
    /* 65E6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L65E7:
    /* 65E7  mov     bp,ax */
    BP = AX;
L65E9:
    /* 65E9  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x65EC)) != 0) return c;
L65EC:
    /* 65EC  pop     bp */
    BP = pop16();
L65ED:
    /* 65ED  pop     cx */
    CX = pop16();
L65EE:
    /* 65EE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L65EF:
    /* 65EF  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L65F3:
    /* 65F3  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L65F7:
    /* 65F7  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L65FA:
    /* 65FA  sub     ax,word ptr [si-4] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFC));
L65FD:
    /* 65FD  imul    bp */
    imul16(BP);
L65FF:
    /* 65FF  shl     ax,1 */
    AX = shl16(AX, 1);
L6601:
    /* 6601  rcl     dx,1 */
    DX = rcl16(DX, 1);
L6603:
    /* 6603  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x6603, 2)) != 0) return c;
L6605:
    /* 6605  sar     ax,1 */
    AX = sar16(AX, 1);
L6607:
    /* 6607  jae     short L660C */
    if (!CF) goto L660C;
L6609:
    /* 6609  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L660A:
    /* 660A  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L660C: /* L660C */
    /* 660C  add     ax,word ptr [si-4] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFC));
L660F:
    /* 660F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6610:
    /* 6610  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L6613:
    /* 6613  sub     ax,word ptr [si-2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFE));
L6616:
    /* 6616  imul    bp */
    imul16(BP);
L6618:
    /* 6618  shl     ax,1 */
    AX = shl16(AX, 1);
L661A:
    /* 661A  rcl     dx,1 */
    DX = rcl16(DX, 1);
L661C:
    /* 661C  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x661C, 2)) != 0) return c;
L661E:
    /* 661E  sar     ax,1 */
    AX = sar16(AX, 1);
L6620:
    /* 6620  jae     short L6625 */
    if (!CF) goto L6625;
L6622:
    /* 6622  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L6623:
    /* 6623  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L6625: /* L6625 */
    /* 6625  add     ax,word ptr [si-2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFE));
L6628:
    /* 6628  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6629: /* L6629 */
    /* 6629  test    byte ptr [si+12h],2 */
    logic8((uint8_t)(rb(pDS, SI + 0x12) & 0x2));
L662D:
    /* 662D  je      short L6632 */
    if (ZF) goto L6632;
L662F:
    /* 662F  jmp     L6575 */
    goto L6575;
L6632: /* L6632 */
    /* 6632  mov     bp,word ptr [si] */
    BP = rw(pDS, SI);
L6634:
    /* 6634  sub     bp,word ptr [si+4] */
    BP = (uint16_t)(BP - rw(pDS, SI + 0x4));
L6637:
    /* 6637  mov     cx,bp */
    CX = BP;
L6639:
    /* 6639  add     cx,word ptr [si+10h] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0x10));
L663C:
    /* 663C  sub     cx,word ptr [si+0Ch] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xC));
L663F:
    /* 663F  mov     ax,word ptr [si+0Ch] */
    AX = rw(pDS, SI + 0xC);
L6642:
    /* 6642  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L6644:
    /* 6644  imul    bp */
    imul16(BP);
L6646:
    /* 6646  shl     ax,1 */
    AX = shl16(AX, 1);
L6648:
    /* 6648  rcl     dx,1 */
    DX = rcl16(DX, 1);
L664A:
    /* 664A  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x664A, 2)) != 0) return c;
L664C:
    /* 664C  sar     ax,1 */
    AX = sar16(AX, 1);
L664E:
    /* 664E  jae     short L6653 */
    if (!CF) goto L6653;
L6650:
    /* 6650  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L6651:
    /* 6651  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L6653: /* L6653 */
    /* 6653  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L6655:
    /* 6655  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6656:
    /* 6656  mov     bx,ax */
    BX = AX;
L6658:
    /* 6658  mov     ax,word ptr [si+0Eh] */
    AX = rw(pDS, SI + 0xE);
L665B:
    /* 665B  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L665E:
    /* 665E  imul    bp */
    imul16(BP);
L6660:
    /* 6660  shl     ax,1 */
    AX = shl16(AX, 1);
L6662:
    /* 6662  rcl     dx,1 */
    DX = rcl16(DX, 1);
L6664:
    /* 6664  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x6664, 2)) != 0) return c;
L6666:
    /* 6666  sar     ax,1 */
    AX = sar16(AX, 1);
L6668:
    /* 6668  jae     short L666D */
    if (!CF) goto L666D;
L666A:
    /* 666A  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L666B:
    /* 666B  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L666D: /* L666D */
    /* 666D  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L6670:
    /* 6670  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6671:
    /* 6671  push    cx */
    push16(CX);
L6672:
    /* 6672  push    bp */
    push16(BP);
L6673:
    /* 6673  mov     cx,ax */
    CX = AX;
L6675:
    /* 6675  mov     ax,bx */
    AX = BX;
L6677:
    /* 6677  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6678:
    /* 6678  mov     bp,ax */
    BP = AX;
L667A:
    /* 667A  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x667D)) != 0) return c;
L667D:
    /* 667D  pop     bp */
    BP = pop16();
L667E:
    /* 667E  pop     cx */
    CX = pop16();
L667F:
    /* 667F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6680:
    /* 6680  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L6684:
    /* 6684  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L6688:
    /* 6688  mov     ax,word ptr [si+14h] */
    AX = rw(pDS, SI + 0x14);
L668B:
    /* 668B  sub     ax,word ptr [si+8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x8));
L668E:
    /* 668E  imul    bp */
    imul16(BP);
L6690:
    /* 6690  shl     ax,1 */
    AX = shl16(AX, 1);
L6692:
    /* 6692  rcl     dx,1 */
    DX = rcl16(DX, 1);
L6694:
    /* 6694  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x6694, 2)) != 0) return c;
L6696:
    /* 6696  sar     ax,1 */
    AX = sar16(AX, 1);
L6698:
    /* 6698  jae     short L669D */
    if (!CF) goto L669D;
L669A:
    /* 669A  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L669B:
    /* 669B  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L669D: /* L669D */
    /* 669D  add     ax,word ptr [si+8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x8));
L66A0:
    /* 66A0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L66A1:
    /* 66A1  mov     ax,word ptr [si+16h] */
    AX = rw(pDS, SI + 0x16);
L66A4:
    /* 66A4  sub     ax,word ptr [si+0Ah] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xA));
L66A7:
    /* 66A7  imul    bp */
    imul16(BP);
L66A9:
    /* 66A9  shl     ax,1 */
    AX = shl16(AX, 1);
L66AB:
    /* 66AB  rcl     dx,1 */
    DX = rcl16(DX, 1);
L66AD:
    /* 66AD  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x66AD, 2)) != 0) return c;
L66AF:
    /* 66AF  sar     ax,1 */
    AX = sar16(AX, 1);
L66B1:
    /* 66B1  jae     short L66B6 */
    if (!CF) goto L66B6;
L66B3:
    /* 66B3  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L66B4:
    /* 66B4  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L66B6: /* L66B6 */
    /* 66B6  add     ax,word ptr [si+0Ah] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xA));
L66B9:
    /* 66B9  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L66BA:
    /* 66BA  jmp     L6575 */
    goto L6575;
L66BD: /* L66BD */
    /* 66BD  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L66C1:
    /* 66C1  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_66C2  (+66C2)
       UW1 only: as seg004_624B for clip code 1 (INTERP.ASM's clip_left). */
L66C2: /* _seg004_66C2 */
    /* 66C2  mov     byte ptr ds:[CLIP_OR],0 */
    wb(pDS, 0x161A, 0x0);
L66C7:
    /* 66C7  mov     byte ptr ds:[CLIP_AND],0FFh */
    wb(pDS, 0x161B, 0xFF);
L66CC:
    /* 66CC  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L66D0:
    /* 66D0  mov     di,word ptr ds:[VB_END] */
    DI = rw(pDS, 0x305);
L66D4:
    /* 66D4  mov     cx,0Ch */
    CX = 0xC;
L66D7:
    /* 66D7  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L66D9:
    /* 66D9  add     word ptr ds:[VB_END],0Ch */
    ww(pDS, 0x305, (uint16_t)(rw(pDS, 0x305) + 0xC));
L66DE:
    /* 66DE  mov     si,word ptr ds:[VB_START] */
    SI = rw(pDS, 0x307);
L66E2:
    /* 66E2  mov     di,VBUF1 */
    DI = 0xC69;
L66E5:
    /* 66E5  cmp     si,VBUF0 */
    sub16(SI, 0x309, 0);
L66E9:
    /* 66E9  je      short L66EE */
    if (ZF) goto L66EE;
L66EB:
    /* 66EB  mov     di,VBUF0 */
    DI = 0x309;
L66EE: /* L66EE */
    /* 66EE  mov     word ptr ds:[VB_START],di */
    ww(pDS, 0x307, DI);
L66F2: /* L66F2 */
    /* 66F2  add     si,0Ch */
    SI = (uint16_t)(SI + 0xC);
L66F5: /* L66F5 */
    /* 66F5  cmp     si,word ptr ds:[VB_END] */
    sub16(SI, rw(pDS, 0x305), 0);
L66F9:
    /* 66F9  jne     short L66FE */
    if (!ZF) goto L66FE;
L66FB:
    /* 66FB  jmp     L683E */
    goto L683E;
L66FE: /* L66FE */
    /* 66FE  mov     al,byte ptr [si+6] */
    AL = rb(pDS, SI + 0x6);
L6701:
    /* 6701  test    al,1 */
    logic8((uint8_t)(AL & 0x1));
L6703:
    /* 6703  jne     short L6714 */
    if (!ZF) goto L6714;
L6705:
    /* 6705  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L6709:
    /* 6709  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L670D:
    /* 670D  mov     cx,6 */
    CX = 0x6;
L6710:
    /* 6710  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L6712:
    /* 6712  jmp     L66F5 */
    goto L66F5;
L6714: /* L6714 */
    /* 6714  test    byte ptr [si-6],1 */
    logic8((uint8_t)(rb(pDS, SI + 0xFFFA) & 0x1));
L6718:
    /* 6718  je      short L671D */
    if (ZF) goto L671D;
L671A:
    /* 671A  jmp     L67A8 */
    goto L67A8;
L671D: /* L671D */
    /* 671D  mov     bp,word ptr [si-8] */
    BP = rw(pDS, SI + 0xFFF8);
L6720:
    /* 6720  add     bp,word ptr [si-0Ch] */
    BP = (uint16_t)(BP + rw(pDS, SI + 0xFFF4));
L6723:
    /* 6723  mov     cx,bp */
    CX = BP;
L6725:
    /* 6725  sub     cx,word ptr [si+4] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x4));
L6728:
    /* 6728  sub     cx,word ptr [si] */
    CX = (uint16_t)(CX - rw(pDS, SI));
L672A:
    /* 672A  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L672C:
    /* 672C  sub     ax,word ptr [si-0Ch] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF4));
L672F:
    /* 672F  imul    bp */
    imul16(BP);
L6731:
    /* 6731  shl     ax,1 */
    AX = shl16(AX, 1);
L6733:
    /* 6733  rcl     dx,1 */
    DX = rcl16(DX, 1);
L6735:
    /* 6735  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x6735, 2)) != 0) return c;
L6737:
    /* 6737  sar     ax,1 */
    AX = sar16(AX, 1);
L6739:
    /* 6739  jae     short L673E */
    if (!CF) goto L673E;
L673B:
    /* 673B  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L673C:
    /* 673C  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L673E: /* L673E */
    /* 673E  add     ax,word ptr [si-0Ch] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF4));
L6741:
    /* 6741  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6742:
    /* 6742  mov     bx,ax */
    BX = AX;
L6744:
    /* 6744  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L6747:
    /* 6747  sub     ax,word ptr [si-0Ah] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFF6));
L674A:
    /* 674A  imul    bp */
    imul16(BP);
L674C:
    /* 674C  shl     ax,1 */
    AX = shl16(AX, 1);
L674E:
    /* 674E  rcl     dx,1 */
    DX = rcl16(DX, 1);
L6750:
    /* 6750  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x6750, 2)) != 0) return c;
L6752:
    /* 6752  sar     ax,1 */
    AX = sar16(AX, 1);
L6754:
    /* 6754  jae     short L6759 */
    if (!CF) goto L6759;
L6756:
    /* 6756  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L6757:
    /* 6757  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L6759: /* L6759 */
    /* 6759  add     ax,word ptr [si-0Ah] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFF6));
L675C:
    /* 675C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L675D:
    /* 675D  push    cx */
    push16(CX);
L675E:
    /* 675E  push    bp */
    push16(BP);
L675F:
    /* 675F  mov     cx,ax */
    CX = AX;
L6761:
    /* 6761  mov     ax,bx */
    AX = BX;
L6763:
    /* 6763  neg     ax */
    AX = (uint16_t)-AX;
L6765:
    /* 6765  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6766:
    /* 6766  mov     bp,ax */
    BP = AX;
L6768:
    /* 6768  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x676B)) != 0) return c;
L676B:
    /* 676B  pop     bp */
    BP = pop16();
L676C:
    /* 676C  pop     cx */
    CX = pop16();
L676D:
    /* 676D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L676E:
    /* 676E  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L6772:
    /* 6772  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L6776:
    /* 6776  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L6779:
    /* 6779  sub     ax,word ptr [si-4] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFC));
L677C:
    /* 677C  imul    bp */
    imul16(BP);
L677E:
    /* 677E  shl     ax,1 */
    AX = shl16(AX, 1);
L6780:
    /* 6780  rcl     dx,1 */
    DX = rcl16(DX, 1);
L6782:
    /* 6782  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x6782, 2)) != 0) return c;
L6784:
    /* 6784  sar     ax,1 */
    AX = sar16(AX, 1);
L6786:
    /* 6786  jae     short L678B */
    if (!CF) goto L678B;
L6788:
    /* 6788  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L6789:
    /* 6789  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L678B: /* L678B */
    /* 678B  add     ax,word ptr [si-4] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFC));
L678E:
    /* 678E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L678F:
    /* 678F  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L6792:
    /* 6792  sub     ax,word ptr [si-2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xFFFE));
L6795:
    /* 6795  imul    bp */
    imul16(BP);
L6797:
    /* 6797  shl     ax,1 */
    AX = shl16(AX, 1);
L6799:
    /* 6799  rcl     dx,1 */
    DX = rcl16(DX, 1);
L679B:
    /* 679B  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x679B, 2)) != 0) return c;
L679D:
    /* 679D  sar     ax,1 */
    AX = sar16(AX, 1);
L679F:
    /* 679F  jae     short L67A4 */
    if (!CF) goto L67A4;
L67A1:
    /* 67A1  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L67A2:
    /* 67A2  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L67A4: /* L67A4 */
    /* 67A4  add     ax,word ptr [si-2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xFFFE));
L67A7:
    /* 67A7  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L67A8: /* L67A8 */
    /* 67A8  test    byte ptr [si+12h],1 */
    logic8((uint8_t)(rb(pDS, SI + 0x12) & 0x1));
L67AC:
    /* 67AC  je      short L67B1 */
    if (ZF) goto L67B1;
L67AE:
    /* 67AE  jmp     L66F2 */
    goto L66F2;
L67B1: /* L67B1 */
    /* 67B1  mov     bp,word ptr [si+4] */
    BP = rw(pDS, SI + 0x4);
L67B4:
    /* 67B4  add     bp,word ptr [si] */
    BP = (uint16_t)(BP + rw(pDS, SI));
L67B6:
    /* 67B6  mov     cx,bp */
    CX = BP;
L67B8:
    /* 67B8  sub     cx,word ptr [si+0Ch] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0xC));
L67BB:
    /* 67BB  sub     cx,word ptr [si+10h] */
    CX = (uint16_t)(CX - rw(pDS, SI + 0x10));
L67BE:
    /* 67BE  mov     ax,word ptr [si+0Ch] */
    AX = rw(pDS, SI + 0xC);
L67C1:
    /* 67C1  sub     ax,word ptr [si] */
    AX = (uint16_t)(AX - rw(pDS, SI));
L67C3:
    /* 67C3  imul    bp */
    imul16(BP);
L67C5:
    /* 67C5  shl     ax,1 */
    AX = shl16(AX, 1);
L67C7:
    /* 67C7  rcl     dx,1 */
    DX = rcl16(DX, 1);
L67C9:
    /* 67C9  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x67C9, 2)) != 0) return c;
L67CB:
    /* 67CB  sar     ax,1 */
    AX = sar16(AX, 1);
L67CD:
    /* 67CD  jae     short L67D2 */
    if (!CF) goto L67D2;
L67CF:
    /* 67CF  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L67D0:
    /* 67D0  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L67D2: /* L67D2 */
    /* 67D2  add     ax,word ptr [si] */
    AX = (uint16_t)(AX + rw(pDS, SI));
L67D4:
    /* 67D4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L67D5:
    /* 67D5  mov     bx,ax */
    BX = AX;
L67D7:
    /* 67D7  mov     ax,word ptr [si+0Eh] */
    AX = rw(pDS, SI + 0xE);
L67DA:
    /* 67DA  sub     ax,word ptr [si+2] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x2));
L67DD:
    /* 67DD  imul    bp */
    imul16(BP);
L67DF:
    /* 67DF  shl     ax,1 */
    AX = shl16(AX, 1);
L67E1:
    /* 67E1  rcl     dx,1 */
    DX = rcl16(DX, 1);
L67E3:
    /* 67E3  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x67E3, 2)) != 0) return c;
L67E5:
    /* 67E5  sar     ax,1 */
    AX = sar16(AX, 1);
L67E7:
    /* 67E7  jae     short L67EC */
    if (!CF) goto L67EC;
L67E9:
    /* 67E9  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L67EA:
    /* 67EA  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L67EC: /* L67EC */
    /* 67EC  add     ax,word ptr [si+2] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x2));
L67EF:
    /* 67EF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L67F0:
    /* 67F0  push    cx */
    push16(CX);
L67F1:
    /* 67F1  push    bp */
    push16(BP);
L67F2:
    /* 67F2  mov     cx,ax */
    CX = AX;
L67F4:
    /* 67F4  mov     ax,bx */
    AX = BX;
L67F6:
    /* 67F6  neg     ax */
    AX = (uint16_t)-AX;
L67F8:
    /* 67F8  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L67F9:
    /* 67F9  mov     bp,ax */
    BP = AX;
L67FB:
    /* 67FB  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x67FE)) != 0) return c;
L67FE:
    /* 67FE  pop     bp */
    BP = pop16();
L67FF:
    /* 67FF  pop     cx */
    CX = pop16();
L6800:
    /* 6800  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6801:
    /* 6801  or      byte ptr ds:[CLIP_OR],al */
    wb(pDS, 0x161A, (uint8_t)(rb(pDS, 0x161A) | AL));
L6805:
    /* 6805  and     byte ptr ds:[CLIP_AND],al */
    wb(pDS, 0x161B, (uint8_t)(rb(pDS, 0x161B) & AL));
L6809:
    /* 6809  mov     ax,word ptr [si+14h] */
    AX = rw(pDS, SI + 0x14);
L680C:
    /* 680C  sub     ax,word ptr [si+8] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0x8));
L680F:
    /* 680F  imul    bp */
    imul16(BP);
L6811:
    /* 6811  shl     ax,1 */
    AX = shl16(AX, 1);
L6813:
    /* 6813  rcl     dx,1 */
    DX = rcl16(DX, 1);
L6815:
    /* 6815  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x6815, 2)) != 0) return c;
L6817:
    /* 6817  sar     ax,1 */
    AX = sar16(AX, 1);
L6819:
    /* 6819  jae     short L681E */
    if (!CF) goto L681E;
L681B:
    /* 681B  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L681C:
    /* 681C  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L681E: /* L681E */
    /* 681E  add     ax,word ptr [si+8] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0x8));
L6821:
    /* 6821  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L6822:
    /* 6822  mov     ax,word ptr [si+16h] */
    AX = rw(pDS, SI + 0x16);
L6825:
    /* 6825  sub     ax,word ptr [si+0Ah] */
    AX = (uint16_t)(AX - rw(pDS, SI + 0xA));
L6828:
    /* 6828  imul    bp */
    imul16(BP);
L682A:
    /* 682A  shl     ax,1 */
    AX = shl16(AX, 1);
L682C:
    /* 682C  rcl     dx,1 */
    DX = rcl16(DX, 1);
L682E:
    /* 682E  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x06E7, 0x682E, 2)) != 0) return c;
L6830:
    /* 6830  sar     ax,1 */
    AX = sar16(AX, 1);
L6832:
    /* 6832  jae     short L6837 */
    if (!CF) goto L6837;
L6834:
    /* 6834  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L6835:
    /* 6835  adc     ax,dx */
    AX = (uint16_t)(AX + DX + CF);
L6837: /* L6837 */
    /* 6837  add     ax,word ptr [si+0Ah] */
    AX = (uint16_t)(AX + rw(pDS, SI + 0xA));
L683A:
    /* 683A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L683B:
    /* 683B  jmp     L66F2 */
    goto L66F2;
L683E: /* L683E */
    /* 683E  mov     word ptr ds:[VB_END],di */
    ww(pDS, 0x305, DI);
L6842: /* L6842 */
    /* 6842  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_6843  (+6843)
       do_scalebm: draw a bitmap scaled to its distance, standing upright at a point (a sprite).
       Operands: the point's offset in resbuf, the bitmap record (an offset from B0B6: do_uwobj
       passes 1Ch, the record at B0D2, and do_uwcrit 0Eh, the record at B0C4, which they fill), and the next opcode. in_scalebm is a second entry with DI =
       the point and AX = the slot, used by PGCACHE.ASM's do_uwobj and do_uwcrit.

       It offsets the point by the bitmap's hot spot (the record's words at +0Ah and +0Ch times bytes
       +6 and +8, shifted by 3 - the zoom at DS:2880, times the view scales at 26D0 and 160A),
       projects that corner and the opposite one (_code_pnt, INSTANCE.ASM, gives the clip codes; a
       point behind the eye or off the wrong edge abandons the sprite), stores the screen corner and
       size in seg048:0B1A..0B27 and calls seg003's _seg003_1082 (SCALEBM.ASM) to draw it. A
       divide overflow while projecting is caught by _overflow_handler_reg (INTERP.ASM), installed
       through SS:4D5, and also abandons the sprite. */
L6843: /* _do_scalebm */
    /* 6843  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L6844:
    /* 6844  mov     di,ax */
    DI = AX;
L6846:
    /* 6846  add     di,PNT_X */
    DI = (uint16_t)(DI + 0x1620);
L684A:
    /* 684A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L684B: /* in_scalebm */
    /* 684B  mov     word ptr ss:[4D5h],offset _overflow_handler_reg */
    ww(pSS, 0x4D5, 0x5BD1);
L6852:
    /* 6852  mov     word ptr cs:overflow_count,0 */
    ww(CODE004, 0x5BCF, 0x0);
L6859:
    /* 6859  add     ax,0B0B6h */
    AX = add16(AX, 0xB0B6, 0);
L685C:
    /* 685C  push    si */
    push16(SI);
L685D:
    /* 685D  mov     si,ax */
    SI = AX;
L685F:
    /* 685F  mov     cl,byte ptr ds:[OBJ_SHIFT] */
    CL = rb(pDS, 0x2880);
L6863:
    /* 6863  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L6866:
    /* 6866  imul    byte ptr [si+6] */
    imul8(rb(pDS, SI + 0x6));
L6869:
    /* 6869  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L686B:
    /* 686B  imul    word ptr ds:[26D0h] */
    imul16(rw(pDS, 0x26D0));
L686F:
    /* 686F  sub     word ptr [di],dx */
    ww(pDS, DI, sub16(rw(pDS, DI), DX, 0));
L6871:
    /* 6871  mov     ax,word ptr [si+0Ch] */
    AX = rw(pDS, SI + 0xC);
L6874:
    /* 6874  imul    byte ptr [si+8] */
    imul8(rb(pDS, SI + 0x8));
L6877:
    /* 6877  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L6879:
    /* 6879  imul    word ptr ds:[MAT_YY] */
    imul16(rw(pDS, 0x160A));
L687D:
    /* 687D  add     word ptr [di+2],dx */
    ww(pDS, DI + 0x2, add16(rw(pDS, DI + 0x2), DX, 0));
L6880:
    /* 6880  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L6883:
    /* 6883  imul    byte ptr [si+0Ah] */
    imul8(rb(pDS, SI + 0xA));
L6886:
    /* 6886  shl     ax,cl */
    AX = shl16(AX, CL);
L6888:
    /* 6888  imul    word ptr ds:[26D0h] */
    imul16(rw(pDS, 0x26D0));
L688C:
    /* 688C  mov     word ptr ds:[15FCh],dx */
    ww(pDS, 0x15FC, DX);
L6890:
    /* 6890  mov     ax,word ptr [si+4] */
    AX = rw(pDS, SI + 0x4);
L6893:
    /* 6893  imul    byte ptr [si+0Ch] */
    imul8(rb(pDS, SI + 0xC));
L6896:
    /* 6896  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L6898:
    /* 6898  imul    word ptr ds:[MAT_YY] */
    imul16(rw(pDS, 0x160A));
L689C:
    /* 689C  mov     word ptr ds:[15FEh],dx */
    ww(pDS, 0x15FE, DX);
L68A0:
    /* 68A0  push    es */
    push16(asm_es);
L68A1:
    /* 68A1  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L68A4:
    /* 68A4  mov     es,ax */
    SET_ES(AX);
L68A6:
    /* 68A6  mov     bx,word ptr [di] */
    BX = rw(pDS, DI);
L68A8:
    /* 68A8  mov     cx,word ptr [di+2] */
    CX = rw(pDS, DI + 0x2);
L68AB:
    /* 68AB  mov     bp,word ptr [di+4] */
    BP = rw(pDS, DI + 0x4);
L68AE:
    /* 68AE  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x68B1)) != 0) return c;
L68B1:
    /* 68B1  test    al,82h */
    logic8((uint8_t)(AL & 0x82));
L68B3:
    /* 68B3  jne     short L6922 */
    if (!ZF) goto L6922;
L68B5:
    /* 68B5  mov     ax,bx */
    AX = BX;
L68B7:
    /* 68B7  imul    word ptr ds:[PROJ_X] */
    imul16(rw(pDS, 0x26B2));
L68BB:
    /* 68BB  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x68BB, 2)) != 0) return c;
L68BD:
    /* 68BD  add     ax,word ptr ds:[CENTRE_X] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B4));
L68C1:
    /* 68C1  mov     word ptr es:[0B22h],ax */
    ww(pES, 0xB22, AX);
L68C5:
    /* 68C5  mov     ax,cx */
    AX = CX;
L68C7:
    /* 68C7  imul    word ptr ds:[PROJ_Y] */
    imul16(rw(pDS, 0x26B0));
L68CB:
    /* 68CB  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x68CB, 2)) != 0) return c;
L68CD:
    /* 68CD  add     ax,word ptr ds:[CENTRE_Y] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B6));
L68D1:
    /* 68D1  mov     word ptr es:[0B24h],ax */
    ww(pES, 0xB24, AX);
L68D5:
    /* 68D5  add     bx,word ptr ds:[15FCh] */
    BX = (uint16_t)(BX + rw(pDS, 0x15FC));
L68D9:
    /* 68D9  add     cx,word ptr ds:[15FEh] */
    CX = (uint16_t)(CX + rw(pDS, 0x15FE));
L68DD:
    /* 68DD  call    _code_pnt */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x587E), 0x68E0)) != 0) return c;
L68E0:
    /* 68E0  test    al,89h */
    logic8((uint8_t)(AL & 0x89));
L68E2:
    /* 68E2  jne     short L6922 */
    if (!ZF) goto L6922;
L68E4:
    /* 68E4  mov     ax,bx */
    AX = BX;
L68E6:
    /* 68E6  imul    word ptr ds:[PROJ_X] */
    imul16(rw(pDS, 0x26B2));
L68EA:
    /* 68EA  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x68EA, 2)) != 0) return c;
L68EC:
    /* 68EC  add     ax,word ptr ds:[CENTRE_X] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B4));
L68F0:
    /* 68F0  sub     ax,word ptr es:[0B22h] */
    AX = (uint16_t)(AX - rw(pES, 0xB22));
L68F5:
    /* 68F5  mov     word ptr es:[0B28h],ax */
    ww(pES, 0xB28, AX);
L68F9:
    /* 68F9  mov     ax,cx */
    AX = CX;
L68FB:
    /* 68FB  imul    word ptr ds:[PROJ_Y] */
    imul16(rw(pDS, 0x26B0));
L68FF:
    /* 68FF  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x06E7, 0x68FF, 2)) != 0) return c;
L6901:
    /* 6901  add     ax,word ptr ds:[CENTRE_Y] */
    AX = (uint16_t)(AX + rw(pDS, 0x26B6));
L6905:
    /* 6905  sub     ax,word ptr es:[0B24h] */
    AX = (uint16_t)(AX - rw(pES, 0xB24));
L690A:
    /* 690A  mov     word ptr es:[0B26h],ax */
    ww(pES, 0xB26, AX);
L690E:
    /* 690E  mov     di,0B1Ch */
    DI = 0xB1C;
L6911:
    /* 6911  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6912:
    /* 6912  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6913:
    /* 6913  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L6914:
    /* 6914  test    word ptr cs:overflow_count,0FFFFh */
    logic16((uint16_t)(rw(CODE004, 0x5BCF) & 0xFFFF));
L691B:
    /* 691B  jne     short L6922 */
    if (!ZF) goto L6922;
L691D:
    /* 691D  call    far ptr _seg003_1082 */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x1082), 0x06E7 + PORT_LOAD_SEG, 0x6922)) != 0) return c;
L6922: /* L6922 */
    /* 6922  pop     es */
    SET_ES(pop16());
L6923:
    /* 6923  pop     si */
    SI = pop16();
L6924:
    /* 6924  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L6925:
    /* 6925  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L6926:
    /* 6926  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L692C: /* L692C */
    /* 692C  mov     word ptr cs:L692A,bp */
    ww(CODE004, 0x692A, BP);
L6931:
    /* 6931  mov     bp,sp */
    BP = SP;
L6933:
    /* 6933  inc     word ptr [bp] */
    ww(pSS, BP, (uint16_t)(rw(pSS, BP) + 1));
L6936:
    /* 6936  inc     word ptr [bp] */
    ww(pSS, BP, (uint16_t)(rw(pSS, BP) + 1));
L6939:
    /* 6939  mov     ax,7FFFh */
    AX = 0x7FFF;
L693C:
    /* 693C  xor     dx,dx */
    DX = logic16((uint16_t)(DX ^ DX));
L693E:
    /* 693E  mov     bp,word ptr cs:L692A */
    BP = rw(CODE004, 0x692A);
L6943:
    /* 6943  iret */
    SP = (uint16_t)(SP + 4); asm_set_flags(pop16()); return ASM_IRET;
L5EE9: /* L5EE9 */
    /* 5EE9  sar bx,1 */
    BX = (uint16_t)((int16_t)BX >> 1);
    goto L5EEB;
L5EF6: /* L5EF6 */
    /* 5EF6  sar cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
    goto L5EF8;
L5F03: /* L5F03 */
    /* 5F03  sar bp,1 */
    BP = (uint16_t)((int16_t)BP >> 1);
    goto L5F05;
L5F1D: /* L5F1D */
    /* 5F1D  sar ax,1 */
    AX = (uint16_t)((int16_t)AX >> 1);
    goto L5F1F;
L5F2B: /* L5F2B */
    /* 5F2B  sar ax,1 */
    AX = (uint16_t)((int16_t)AX >> 1);
    goto L5F2D;
L5F3E: /* L5F3E */
    /* 5F3E  sar bx,1 */
    BX = (uint16_t)((int16_t)BX >> 1);
    goto L5F40;
L5F4D: /* L5F4D */
    /* 5F4D  sar cx,1 */
    CX = (uint16_t)((int16_t)CX >> 1);
    goto L5F4F;
L5F5C: /* L5F5C */
    /* 5F5C  sar bp,1 */
    BP = (uint16_t)((int16_t)BP >> 1);
    goto L5F5E;
L5F77: /* L5F77 */
    /* 5F77  sar ax,1 */
    AX = (uint16_t)((int16_t)AX >> 1);
    goto L5F79;
L5F86: /* L5F86 */
    /* 5F86  sar ax,1 */
    AX = (uint16_t)((int16_t)AX >> 1);
    goto L5F88;
}
