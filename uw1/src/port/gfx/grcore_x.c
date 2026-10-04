/* grcore_x.c: replaces src/gfx/GRCORE.ASM (seg003_4C70, 4C70..5A71 of its
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

uint32_t asm_mod_GRCORE(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x4C74: goto L4C74;
    case 0x4C75: goto L4C75;
    case 0x4C76: goto L4C76;
    case 0x4C78: goto L4C78;
    case 0x4C79: goto L4C79;
    case 0x4C7A: goto L4C7A;
    case 0x4C7B: goto L4C7B;
    case 0x4C7C: goto L4C7C;
    case 0x4C7E: goto L4C7E;
    case 0x4C83: goto L4C83;
    case 0x4C85: goto L4C85;
    case 0x4C8A: goto L4C8A;
    case 0x4C8D: goto L4C8D;
    case 0x4C8F: goto L4C8F;
    case 0x4C91: goto L4C91;
    case 0x4C92: goto L4C92;
    case 0x4C94: goto L4C94;
    case 0x4C97: goto L4C97;
    case 0x4C98: goto L4C98;
    case 0x4C9B: goto L4C9B;
    case 0x4C9C: goto L4C9C;
    case 0x4CA1: goto L4CA1;
    case 0x4CA3: goto L4CA3;
    case 0x4CA8: goto L4CA8;
    case 0x4CAA: goto L4CAA;
    case 0x4CAB: goto L4CAB;
    case 0x4CAC: goto L4CAC;
    case 0x4CAD: goto L4CAD;
    case 0x4CAE: goto L4CAE;
    case 0x4CAF: goto L4CAF;
    case 0x4CB0: goto L4CB0;
    case 0x4CB1: goto L4CB1;
    case 0x4CB2: goto L4CB2;
    case 0x4CB4: goto L4CB4;
    case 0x4CB5: goto L4CB5;
    case 0x4CB6: goto L4CB6;
    case 0x4CB7: goto L4CB7;
    case 0x4CB8: goto L4CB8;
    case 0x4CBA: goto L4CBA;
    case 0x4CBF: goto L4CBF;
    case 0x4CC1: goto L4CC1;
    case 0x4CC6: goto L4CC6;
    case 0x4CC9: goto L4CC9;
    case 0x4CCB: goto L4CCB;
    case 0x4CCD: goto L4CCD;
    case 0x4CCE: goto L4CCE;
    case 0x4CD0: goto L4CD0;
    case 0x4CD3: goto L4CD3;
    case 0x4CD4: goto L4CD4;
    case 0x4CD7: goto L4CD7;
    case 0x4CD8: goto L4CD8;
    case 0x4CDD: goto L4CDD;
    case 0x4CDF: goto L4CDF;
    case 0x4CE4: goto L4CE4;
    case 0x4CE6: goto L4CE6;
    case 0x4CE7: goto L4CE7;
    case 0x4CE8: goto L4CE8;
    case 0x4CE9: goto L4CE9;
    case 0x4CEA: goto L4CEA;
    case 0x4CEB: goto L4CEB;
    case 0x4CEC: goto L4CEC;
    case 0x4CED: goto L4CED;
    case 0x4CEE: goto L4CEE;
    case 0x4CF0: goto L4CF0;
    case 0x4CF1: goto L4CF1;
    case 0x4CF2: goto L4CF2;
    case 0x4CF3: goto L4CF3;
    case 0x4CF4: goto L4CF4;
    case 0x4CF6: goto L4CF6;
    case 0x4CFB: goto L4CFB;
    case 0x4CFD: goto L4CFD;
    case 0x4D02: goto L4D02;
    case 0x4D05: goto L4D05;
    case 0x4D07: goto L4D07;
    case 0x4D0A: goto L4D0A;
    case 0x4D0C: goto L4D0C;
    case 0x4D0E: goto L4D0E;
    case 0x4D10: goto L4D10;
    case 0x4D14: goto L4D14;
    case 0x4D15: goto L4D15;
    case 0x4D17: goto L4D17;
    case 0x4D19: goto L4D19;
    case 0x4D1C: goto L4D1C;
    case 0x4D1F: goto L4D1F;
    case 0x4D21: goto L4D21;
    case 0x4D23: goto L4D23;
    case 0x4D24: goto L4D24;
    case 0x4D27: goto L4D27;
    case 0x4D2A: goto L4D2A;
    case 0x4D2D: goto L4D2D;
    case 0x4D30: goto L4D30;
    case 0x4D33: goto L4D33;
    case 0x4D35: goto L4D35;
    case 0x4D37: goto L4D37;
    case 0x4D38: goto L4D38;
    case 0x4D3A: goto L4D3A;
    case 0x4D3D: goto L4D3D;
    case 0x4D3E: goto L4D3E;
    case 0x4D40: goto L4D40;
    case 0x4D43: goto L4D43;
    case 0x4D46: goto L4D46;
    case 0x4D47: goto L4D47;
    case 0x4D4C: goto L4D4C;
    case 0x4D4E: goto L4D4E;
    case 0x4D53: goto L4D53;
    case 0x4D55: goto L4D55;
    case 0x4D56: goto L4D56;
    case 0x4D57: goto L4D57;
    case 0x4D58: goto L4D58;
    case 0x4D59: goto L4D59;
    case 0x4D5A: goto L4D5A;
    case 0x4D5B: goto L4D5B;
    case 0x4D5C: goto L4D5C;
    case 0x4D5D: goto L4D5D;
    case 0x4D5F: goto L4D5F;
    case 0x4D60: goto L4D60;
    case 0x4D61: goto L4D61;
    case 0x4D62: goto L4D62;
    case 0x4D63: goto L4D63;
    case 0x4D65: goto L4D65;
    case 0x4D6A: goto L4D6A;
    case 0x4D6C: goto L4D6C;
    case 0x4D71: goto L4D71;
    case 0x4D74: goto L4D74;
    case 0x4D76: goto L4D76;
    case 0x4D79: goto L4D79;
    case 0x4D7B: goto L4D7B;
    case 0x4D7D: goto L4D7D;
    case 0x4D7F: goto L4D7F;
    case 0x4D83: goto L4D83;
    case 0x4D84: goto L4D84;
    case 0x4D86: goto L4D86;
    case 0x4D88: goto L4D88;
    case 0x4D8B: goto L4D8B;
    case 0x4D8E: goto L4D8E;
    case 0x4D90: goto L4D90;
    case 0x4D92: goto L4D92;
    case 0x4D93: goto L4D93;
    case 0x4D96: goto L4D96;
    case 0x4D99: goto L4D99;
    case 0x4D9C: goto L4D9C;
    case 0x4D9F: goto L4D9F;
    case 0x4DA2: goto L4DA2;
    case 0x4DA4: goto L4DA4;
    case 0x4DA6: goto L4DA6;
    case 0x4DA7: goto L4DA7;
    case 0x4DA9: goto L4DA9;
    case 0x4DAC: goto L4DAC;
    case 0x4DAD: goto L4DAD;
    case 0x4DAF: goto L4DAF;
    case 0x4DB2: goto L4DB2;
    case 0x4DB5: goto L4DB5;
    case 0x4DB6: goto L4DB6;
    case 0x4DBB: goto L4DBB;
    case 0x4DBD: goto L4DBD;
    case 0x4DC2: goto L4DC2;
    case 0x4DC4: goto L4DC4;
    case 0x4DC5: goto L4DC5;
    case 0x4DC6: goto L4DC6;
    case 0x4DC7: goto L4DC7;
    case 0x4DC8: goto L4DC8;
    case 0x4DC9: goto L4DC9;
    case 0x4DCA: goto L4DCA;
    case 0x4DCB: goto L4DCB;
    case 0x4DCC: goto L4DCC;
    case 0x4DCE: goto L4DCE;
    case 0x4DCF: goto L4DCF;
    case 0x4DD0: goto L4DD0;
    case 0x4DD1: goto L4DD1;
    case 0x4DD2: goto L4DD2;
    case 0x4DD4: goto L4DD4;
    case 0x4DD9: goto L4DD9;
    case 0x4DDB: goto L4DDB;
    case 0x4DE0: goto L4DE0;
    case 0x4DE3: goto L4DE3;
    case 0x4DE5: goto L4DE5;
    case 0x4DE8: goto L4DE8;
    case 0x4DEA: goto L4DEA;
    case 0x4DEC: goto L4DEC;
    case 0x4DEE: goto L4DEE;
    case 0x4DF2: goto L4DF2;
    case 0x4DF3: goto L4DF3;
    case 0x4DF5: goto L4DF5;
    case 0x4DF7: goto L4DF7;
    case 0x4DFA: goto L4DFA;
    case 0x4DFD: goto L4DFD;
    case 0x4DFF: goto L4DFF;
    case 0x4E01: goto L4E01;
    case 0x4E02: goto L4E02;
    case 0x4E05: goto L4E05;
    case 0x4E08: goto L4E08;
    case 0x4E0B: goto L4E0B;
    case 0x4E0E: goto L4E0E;
    case 0x4E11: goto L4E11;
    case 0x4E13: goto L4E13;
    case 0x4E15: goto L4E15;
    case 0x4E16: goto L4E16;
    case 0x4E18: goto L4E18;
    case 0x4E1B: goto L4E1B;
    case 0x4E1C: goto L4E1C;
    case 0x4E1E: goto L4E1E;
    case 0x4E21: goto L4E21;
    case 0x4E24: goto L4E24;
    case 0x4E25: goto L4E25;
    case 0x4E2A: goto L4E2A;
    case 0x4E2C: goto L4E2C;
    case 0x4E31: goto L4E31;
    case 0x4E33: goto L4E33;
    case 0x4E34: goto L4E34;
    case 0x4E35: goto L4E35;
    case 0x4E36: goto L4E36;
    case 0x4E37: goto L4E37;
    case 0x4E38: goto L4E38;
    case 0x4E39: goto L4E39;
    case 0x4E3A: goto L4E3A;
    case 0x4E3B: goto L4E3B;
    case 0x4E3D: goto L4E3D;
    case 0x4E3E: goto L4E3E;
    case 0x4E3F: goto L4E3F;
    case 0x4E40: goto L4E40;
    case 0x4E41: goto L4E41;
    case 0x4E43: goto L4E43;
    case 0x4E48: goto L4E48;
    case 0x4E4A: goto L4E4A;
    case 0x4E4F: goto L4E4F;
    case 0x4E52: goto L4E52;
    case 0x4E54: goto L4E54;
    case 0x4E57: goto L4E57;
    case 0x4E59: goto L4E59;
    case 0x4E5B: goto L4E5B;
    case 0x4E5D: goto L4E5D;
    case 0x4E61: goto L4E61;
    case 0x4E62: goto L4E62;
    case 0x4E64: goto L4E64;
    case 0x4E66: goto L4E66;
    case 0x4E69: goto L4E69;
    case 0x4E6C: goto L4E6C;
    case 0x4E6E: goto L4E6E;
    case 0x4E70: goto L4E70;
    case 0x4E71: goto L4E71;
    case 0x4E74: goto L4E74;
    case 0x4E77: goto L4E77;
    case 0x4E7A: goto L4E7A;
    case 0x4E7D: goto L4E7D;
    case 0x4E80: goto L4E80;
    case 0x4E82: goto L4E82;
    case 0x4E84: goto L4E84;
    case 0x4E85: goto L4E85;
    case 0x4E87: goto L4E87;
    case 0x4E8A: goto L4E8A;
    case 0x4E8B: goto L4E8B;
    case 0x4E8D: goto L4E8D;
    case 0x4E90: goto L4E90;
    case 0x4E93: goto L4E93;
    case 0x4E94: goto L4E94;
    case 0x4E99: goto L4E99;
    case 0x4E9B: goto L4E9B;
    case 0x4EA0: goto L4EA0;
    case 0x4EA2: goto L4EA2;
    case 0x4EA3: goto L4EA3;
    case 0x4EA4: goto L4EA4;
    case 0x4EA5: goto L4EA5;
    case 0x4EA6: goto L4EA6;
    case 0x4EA7: goto L4EA7;
    case 0x4EA8: goto L4EA8;
    case 0x4EA9: goto L4EA9;
    case 0x4EAA: goto L4EAA;
    case 0x4EAC: goto L4EAC;
    case 0x4EAD: goto L4EAD;
    case 0x4EAE: goto L4EAE;
    case 0x4EAF: goto L4EAF;
    case 0x4EB0: goto L4EB0;
    case 0x4EB2: goto L4EB2;
    case 0x4EB7: goto L4EB7;
    case 0x4EB9: goto L4EB9;
    case 0x4EBE: goto L4EBE;
    case 0x4EC1: goto L4EC1;
    case 0x4EC3: goto L4EC3;
    case 0x4EC5: goto L4EC5;
    case 0x4EC6: goto L4EC6;
    case 0x4EC8: goto L4EC8;
    case 0x4ECB: goto L4ECB;
    case 0x4ECC: goto L4ECC;
    case 0x4ECF: goto L4ECF;
    case 0x4ED0: goto L4ED0;
    case 0x4ED5: goto L4ED5;
    case 0x4ED7: goto L4ED7;
    case 0x4EDC: goto L4EDC;
    case 0x4EDE: goto L4EDE;
    case 0x4EDF: goto L4EDF;
    case 0x4EE0: goto L4EE0;
    case 0x4EE1: goto L4EE1;
    case 0x4EE2: goto L4EE2;
    case 0x4EE3: goto L4EE3;
    case 0x4EE4: goto L4EE4;
    case 0x4EE5: goto L4EE5;
    case 0x4EE6: goto L4EE6;
    case 0x4EE8: goto L4EE8;
    case 0x4EE9: goto L4EE9;
    case 0x4EEA: goto L4EEA;
    case 0x4EEB: goto L4EEB;
    case 0x4EEC: goto L4EEC;
    case 0x4EEE: goto L4EEE;
    case 0x4EF3: goto L4EF3;
    case 0x4EF5: goto L4EF5;
    case 0x4EFA: goto L4EFA;
    case 0x4EFD: goto L4EFD;
    case 0x4F00: goto L4F00;
    case 0x4F03: goto L4F03;
    case 0x4F06: goto L4F06;
    case 0x4F08: goto L4F08;
    case 0x4F0A: goto L4F0A;
    case 0x4F0B: goto L4F0B;
    case 0x4F0D: goto L4F0D;
    case 0x4F10: goto L4F10;
    case 0x4F11: goto L4F11;
    case 0x4F14: goto L4F14;
    case 0x4F15: goto L4F15;
    case 0x4F1A: goto L4F1A;
    case 0x4F1C: goto L4F1C;
    case 0x4F21: goto L4F21;
    case 0x4F23: goto L4F23;
    case 0x4F24: goto L4F24;
    case 0x4F25: goto L4F25;
    case 0x4F26: goto L4F26;
    case 0x4F27: goto L4F27;
    case 0x4F28: goto L4F28;
    case 0x4F29: goto L4F29;
    case 0x4F2A: goto L4F2A;
    case 0x4F2B: goto L4F2B;
    case 0x4F2D: goto L4F2D;
    case 0x4F2E: goto L4F2E;
    case 0x4F2F: goto L4F2F;
    case 0x4F30: goto L4F30;
    case 0x4F31: goto L4F31;
    case 0x4F33: goto L4F33;
    case 0x4F38: goto L4F38;
    case 0x4F3A: goto L4F3A;
    case 0x4F3F: goto L4F3F;
    case 0x4F42: goto L4F42;
    case 0x4F44: goto L4F44;
    case 0x4F46: goto L4F46;
    case 0x4F47: goto L4F47;
    case 0x4F49: goto L4F49;
    case 0x4F4C: goto L4F4C;
    case 0x4F4D: goto L4F4D;
    case 0x4F50: goto L4F50;
    case 0x4F51: goto L4F51;
    case 0x4F56: goto L4F56;
    case 0x4F58: goto L4F58;
    case 0x4F5D: goto L4F5D;
    case 0x4F5F: goto L4F5F;
    case 0x4F60: goto L4F60;
    case 0x4F61: goto L4F61;
    case 0x4F62: goto L4F62;
    case 0x4F63: goto L4F63;
    case 0x4F64: goto L4F64;
    case 0x4F65: goto L4F65;
    case 0x4F66: goto L4F66;
    case 0x4F67: goto L4F67;
    case 0x4F69: goto L4F69;
    case 0x4F6A: goto L4F6A;
    case 0x4F6B: goto L4F6B;
    case 0x4F6C: goto L4F6C;
    case 0x4F6D: goto L4F6D;
    case 0x4F6F: goto L4F6F;
    case 0x4F74: goto L4F74;
    case 0x4F76: goto L4F76;
    case 0x4F7B: goto L4F7B;
    case 0x4F7E: goto L4F7E;
    case 0x4F80: goto L4F80;
    case 0x4F82: goto L4F82;
    case 0x4F83: goto L4F83;
    case 0x4F85: goto L4F85;
    case 0x4F88: goto L4F88;
    case 0x4F89: goto L4F89;
    case 0x4F8C: goto L4F8C;
    case 0x4F8D: goto L4F8D;
    case 0x4F92: goto L4F92;
    case 0x4F94: goto L4F94;
    case 0x4F99: goto L4F99;
    case 0x4F9B: goto L4F9B;
    case 0x4F9C: goto L4F9C;
    case 0x4F9D: goto L4F9D;
    case 0x4F9E: goto L4F9E;
    case 0x4F9F: goto L4F9F;
    case 0x4FA0: goto L4FA0;
    case 0x4FA1: goto L4FA1;
    case 0x4FA2: goto L4FA2;
    case 0x4FA3: goto L4FA3;
    case 0x4FA5: goto L4FA5;
    case 0x4FA6: goto L4FA6;
    case 0x4FA7: goto L4FA7;
    case 0x4FA8: goto L4FA8;
    case 0x4FA9: goto L4FA9;
    case 0x4FAB: goto L4FAB;
    case 0x4FB0: goto L4FB0;
    case 0x4FB2: goto L4FB2;
    case 0x4FB7: goto L4FB7;
    case 0x4FBA: goto L4FBA;
    case 0x4FBC: goto L4FBC;
    case 0x4FBE: goto L4FBE;
    case 0x4FBF: goto L4FBF;
    case 0x4FC1: goto L4FC1;
    case 0x4FC4: goto L4FC4;
    case 0x4FC5: goto L4FC5;
    case 0x4FC8: goto L4FC8;
    case 0x4FC9: goto L4FC9;
    case 0x4FCE: goto L4FCE;
    case 0x4FD0: goto L4FD0;
    case 0x4FD5: goto L4FD5;
    case 0x4FD7: goto L4FD7;
    case 0x4FD8: goto L4FD8;
    case 0x4FD9: goto L4FD9;
    case 0x4FDA: goto L4FDA;
    case 0x4FDB: goto L4FDB;
    case 0x4FDC: goto L4FDC;
    case 0x4FDD: goto L4FDD;
    case 0x4FDE: goto L4FDE;
    case 0x4FDF: goto L4FDF;
    case 0x4FE1: goto L4FE1;
    case 0x4FE2: goto L4FE2;
    case 0x4FE3: goto L4FE3;
    case 0x4FE4: goto L4FE4;
    case 0x4FE5: goto L4FE5;
    case 0x4FE7: goto L4FE7;
    case 0x4FEC: goto L4FEC;
    case 0x4FEE: goto L4FEE;
    case 0x4FF3: goto L4FF3;
    case 0x4FF6: goto L4FF6;
    case 0x4FF9: goto L4FF9;
    case 0x4FFC: goto L4FFC;
    case 0x4FFE: goto L4FFE;
    case 0x5000: goto L5000;
    case 0x5001: goto L5001;
    case 0x5003: goto L5003;
    case 0x5006: goto L5006;
    case 0x5007: goto L5007;
    case 0x500A: goto L500A;
    case 0x500B: goto L500B;
    case 0x5010: goto L5010;
    case 0x5012: goto L5012;
    case 0x5017: goto L5017;
    case 0x5019: goto L5019;
    case 0x501A: goto L501A;
    case 0x501B: goto L501B;
    case 0x501C: goto L501C;
    case 0x501D: goto L501D;
    case 0x501E: goto L501E;
    case 0x501F: goto L501F;
    case 0x5020: goto L5020;
    case 0x5021: goto L5021;
    case 0x5023: goto L5023;
    case 0x5024: goto L5024;
    case 0x5025: goto L5025;
    case 0x5026: goto L5026;
    case 0x5027: goto L5027;
    case 0x5029: goto L5029;
    case 0x502E: goto L502E;
    case 0x5030: goto L5030;
    case 0x5035: goto L5035;
    case 0x5038: goto L5038;
    case 0x503B: goto L503B;
    case 0x503E: goto L503E;
    case 0x5040: goto L5040;
    case 0x5042: goto L5042;
    case 0x5043: goto L5043;
    case 0x5045: goto L5045;
    case 0x5048: goto L5048;
    case 0x5049: goto L5049;
    case 0x504C: goto L504C;
    case 0x504D: goto L504D;
    case 0x5052: goto L5052;
    case 0x5054: goto L5054;
    case 0x5059: goto L5059;
    case 0x505B: goto L505B;
    case 0x505C: goto L505C;
    case 0x505D: goto L505D;
    case 0x505E: goto L505E;
    case 0x505F: goto L505F;
    case 0x5060: goto L5060;
    case 0x5061: goto L5061;
    case 0x5062: goto L5062;
    case 0x5063: goto L5063;
    case 0x5065: goto L5065;
    case 0x5066: goto L5066;
    case 0x5067: goto L5067;
    case 0x5068: goto L5068;
    case 0x5069: goto L5069;
    case 0x506B: goto L506B;
    case 0x5070: goto L5070;
    case 0x5072: goto L5072;
    case 0x5077: goto L5077;
    case 0x507A: goto L507A;
    case 0x507D: goto L507D;
    case 0x5080: goto L5080;
    case 0x5082: goto L5082;
    case 0x5084: goto L5084;
    case 0x5085: goto L5085;
    case 0x5087: goto L5087;
    case 0x508A: goto L508A;
    case 0x508B: goto L508B;
    case 0x508E: goto L508E;
    case 0x508F: goto L508F;
    case 0x5094: goto L5094;
    case 0x5096: goto L5096;
    case 0x509B: goto L509B;
    case 0x509D: goto L509D;
    case 0x509E: goto L509E;
    case 0x509F: goto L509F;
    case 0x50A0: goto L50A0;
    case 0x50A1: goto L50A1;
    case 0x50A2: goto L50A2;
    case 0x50A3: goto L50A3;
    case 0x50A4: goto L50A4;
    case 0x50A5: goto L50A5;
    case 0x50A7: goto L50A7;
    case 0x50A8: goto L50A8;
    case 0x50A9: goto L50A9;
    case 0x50AA: goto L50AA;
    case 0x50AB: goto L50AB;
    case 0x50AD: goto L50AD;
    case 0x50B2: goto L50B2;
    case 0x50B4: goto L50B4;
    case 0x50B9: goto L50B9;
    case 0x50BC: goto L50BC;
    case 0x50BF: goto L50BF;
    case 0x50C2: goto L50C2;
    case 0x50C4: goto L50C4;
    case 0x50C6: goto L50C6;
    case 0x50C7: goto L50C7;
    case 0x50C9: goto L50C9;
    case 0x50CC: goto L50CC;
    case 0x50CD: goto L50CD;
    case 0x50D0: goto L50D0;
    case 0x50D1: goto L50D1;
    case 0x50D6: goto L50D6;
    case 0x50D8: goto L50D8;
    case 0x50DD: goto L50DD;
    case 0x50DF: goto L50DF;
    case 0x50E0: goto L50E0;
    case 0x50E1: goto L50E1;
    case 0x50E2: goto L50E2;
    case 0x50E3: goto L50E3;
    case 0x50E4: goto L50E4;
    case 0x50E5: goto L50E5;
    case 0x50E6: goto L50E6;
    case 0x50E7: goto L50E7;
    case 0x50E9: goto L50E9;
    case 0x50EA: goto L50EA;
    case 0x50EB: goto L50EB;
    case 0x50EC: goto L50EC;
    case 0x50ED: goto L50ED;
    case 0x50EF: goto L50EF;
    case 0x50F4: goto L50F4;
    case 0x50F6: goto L50F6;
    case 0x50FB: goto L50FB;
    case 0x50FE: goto L50FE;
    case 0x5101: goto L5101;
    case 0x5104: goto L5104;
    case 0x5107: goto L5107;
    case 0x5109: goto L5109;
    case 0x510B: goto L510B;
    case 0x510C: goto L510C;
    case 0x510E: goto L510E;
    case 0x5111: goto L5111;
    case 0x5112: goto L5112;
    case 0x5114: goto L5114;
    case 0x5117: goto L5117;
    case 0x5118: goto L5118;
    case 0x511D: goto L511D;
    case 0x511F: goto L511F;
    case 0x5124: goto L5124;
    case 0x5126: goto L5126;
    case 0x5127: goto L5127;
    case 0x5128: goto L5128;
    case 0x5129: goto L5129;
    case 0x512A: goto L512A;
    case 0x512B: goto L512B;
    case 0x512C: goto L512C;
    case 0x512D: goto L512D;
    case 0x512E: goto L512E;
    case 0x5130: goto L5130;
    case 0x5131: goto L5131;
    case 0x5132: goto L5132;
    case 0x5133: goto L5133;
    case 0x5134: goto L5134;
    case 0x5136: goto L5136;
    case 0x513B: goto L513B;
    case 0x513D: goto L513D;
    case 0x5142: goto L5142;
    case 0x5145: goto L5145;
    case 0x5148: goto L5148;
    case 0x514B: goto L514B;
    case 0x514E: goto L514E;
    case 0x5150: goto L5150;
    case 0x5152: goto L5152;
    case 0x5153: goto L5153;
    case 0x5155: goto L5155;
    case 0x5158: goto L5158;
    case 0x5159: goto L5159;
    case 0x515B: goto L515B;
    case 0x515E: goto L515E;
    case 0x515F: goto L515F;
    case 0x5164: goto L5164;
    case 0x5166: goto L5166;
    case 0x516B: goto L516B;
    case 0x516D: goto L516D;
    case 0x516E: goto L516E;
    case 0x516F: goto L516F;
    case 0x5170: goto L5170;
    case 0x5171: goto L5171;
    case 0x5172: goto L5172;
    case 0x5173: goto L5173;
    case 0x5174: goto L5174;
    case 0x5175: goto L5175;
    case 0x5177: goto L5177;
    case 0x5178: goto L5178;
    case 0x5179: goto L5179;
    case 0x517A: goto L517A;
    case 0x517B: goto L517B;
    case 0x517D: goto L517D;
    case 0x5182: goto L5182;
    case 0x5184: goto L5184;
    case 0x5189: goto L5189;
    case 0x518C: goto L518C;
    case 0x518F: goto L518F;
    case 0x5191: goto L5191;
    case 0x5193: goto L5193;
    case 0x5194: goto L5194;
    case 0x5196: goto L5196;
    case 0x5199: goto L5199;
    case 0x519A: goto L519A;
    case 0x519D: goto L519D;
    case 0x519E: goto L519E;
    case 0x51A3: goto L51A3;
    case 0x51A5: goto L51A5;
    case 0x51AA: goto L51AA;
    case 0x51AC: goto L51AC;
    case 0x51AD: goto L51AD;
    case 0x51AE: goto L51AE;
    case 0x51AF: goto L51AF;
    case 0x51B0: goto L51B0;
    case 0x51B1: goto L51B1;
    case 0x51B2: goto L51B2;
    case 0x51B3: goto L51B3;
    case 0x51B4: goto L51B4;
    case 0x51B6: goto L51B6;
    case 0x51B7: goto L51B7;
    case 0x51B8: goto L51B8;
    case 0x51B9: goto L51B9;
    case 0x51BA: goto L51BA;
    case 0x51BC: goto L51BC;
    case 0x51C1: goto L51C1;
    case 0x51C3: goto L51C3;
    case 0x51C8: goto L51C8;
    case 0x51CB: goto L51CB;
    case 0x51CD: goto L51CD;
    case 0x51CF: goto L51CF;
    case 0x51D0: goto L51D0;
    case 0x51D2: goto L51D2;
    case 0x51D5: goto L51D5;
    case 0x51D6: goto L51D6;
    case 0x51D9: goto L51D9;
    case 0x51DA: goto L51DA;
    case 0x51DF: goto L51DF;
    case 0x51E1: goto L51E1;
    case 0x51E6: goto L51E6;
    case 0x51E8: goto L51E8;
    case 0x51E9: goto L51E9;
    case 0x51EA: goto L51EA;
    case 0x51EB: goto L51EB;
    case 0x51EC: goto L51EC;
    case 0x51ED: goto L51ED;
    case 0x51EE: goto L51EE;
    case 0x51EF: goto L51EF;
    case 0x51F0: goto L51F0;
    case 0x51F2: goto L51F2;
    case 0x51F3: goto L51F3;
    case 0x51F4: goto L51F4;
    case 0x51F5: goto L51F5;
    case 0x51F6: goto L51F6;
    case 0x51F8: goto L51F8;
    case 0x51FD: goto L51FD;
    case 0x51FF: goto L51FF;
    case 0x5204: goto L5204;
    case 0x5207: goto L5207;
    case 0x520A: goto L520A;
    case 0x520C: goto L520C;
    case 0x520E: goto L520E;
    case 0x520F: goto L520F;
    case 0x5211: goto L5211;
    case 0x5214: goto L5214;
    case 0x5215: goto L5215;
    case 0x5218: goto L5218;
    case 0x5219: goto L5219;
    case 0x521E: goto L521E;
    case 0x5220: goto L5220;
    case 0x5225: goto L5225;
    case 0x5227: goto L5227;
    case 0x5228: goto L5228;
    case 0x5229: goto L5229;
    case 0x522A: goto L522A;
    case 0x522B: goto L522B;
    case 0x522C: goto L522C;
    case 0x522D: goto L522D;
    case 0x522E: goto L522E;
    case 0x522F: goto L522F;
    case 0x5231: goto L5231;
    case 0x5232: goto L5232;
    case 0x5233: goto L5233;
    case 0x5234: goto L5234;
    case 0x5235: goto L5235;
    case 0x5237: goto L5237;
    case 0x523C: goto L523C;
    case 0x523E: goto L523E;
    case 0x5243: goto L5243;
    case 0x5246: goto L5246;
    case 0x5248: goto L5248;
    case 0x524B: goto L524B;
    case 0x524D: goto L524D;
    case 0x524F: goto L524F;
    case 0x5250: goto L5250;
    case 0x5252: goto L5252;
    case 0x5255: goto L5255;
    case 0x5256: goto L5256;
    case 0x5259: goto L5259;
    case 0x525B: goto L525B;
    case 0x525E: goto L525E;
    case 0x525F: goto L525F;
    case 0x5264: goto L5264;
    case 0x5266: goto L5266;
    case 0x526B: goto L526B;
    case 0x526D: goto L526D;
    case 0x526E: goto L526E;
    case 0x526F: goto L526F;
    case 0x5270: goto L5270;
    case 0x5271: goto L5271;
    case 0x5272: goto L5272;
    case 0x5273: goto L5273;
    case 0x5274: goto L5274;
    case 0x5275: goto L5275;
    case 0x5277: goto L5277;
    case 0x5278: goto L5278;
    case 0x5279: goto L5279;
    case 0x527A: goto L527A;
    case 0x527B: goto L527B;
    case 0x527D: goto L527D;
    case 0x5282: goto L5282;
    case 0x5284: goto L5284;
    case 0x5289: goto L5289;
    case 0x528C: goto L528C;
    case 0x528E: goto L528E;
    case 0x5291: goto L5291;
    case 0x5293: goto L5293;
    case 0x5295: goto L5295;
    case 0x5296: goto L5296;
    case 0x5298: goto L5298;
    case 0x529B: goto L529B;
    case 0x529C: goto L529C;
    case 0x529F: goto L529F;
    case 0x52A1: goto L52A1;
    case 0x52A4: goto L52A4;
    case 0x52A5: goto L52A5;
    case 0x52AA: goto L52AA;
    case 0x52AC: goto L52AC;
    case 0x52B1: goto L52B1;
    case 0x52B3: goto L52B3;
    case 0x52B4: goto L52B4;
    case 0x52B5: goto L52B5;
    case 0x52B6: goto L52B6;
    case 0x52B7: goto L52B7;
    case 0x52B8: goto L52B8;
    case 0x52B9: goto L52B9;
    case 0x52BA: goto L52BA;
    case 0x52BB: goto L52BB;
    case 0x52BD: goto L52BD;
    case 0x52BE: goto L52BE;
    case 0x52BF: goto L52BF;
    case 0x52C0: goto L52C0;
    case 0x52C1: goto L52C1;
    case 0x52C3: goto L52C3;
    case 0x52C8: goto L52C8;
    case 0x52CA: goto L52CA;
    case 0x52CF: goto L52CF;
    case 0x52D2: goto L52D2;
    case 0x52D5: goto L52D5;
    case 0x52D8: goto L52D8;
    case 0x52DB: goto L52DB;
    case 0x52DE: goto L52DE;
    case 0x52E0: goto L52E0;
    case 0x52E2: goto L52E2;
    case 0x52E3: goto L52E3;
    case 0x52E5: goto L52E5;
    case 0x52E8: goto L52E8;
    case 0x52E9: goto L52E9;
    case 0x52ED: goto L52ED;
    case 0x52F0: goto L52F0;
    case 0x52F4: goto L52F4;
    case 0x52F8: goto L52F8;
    case 0x52FB: goto L52FB;
    case 0x52FE: goto L52FE;
    case 0x52FF: goto L52FF;
    case 0x5304: goto L5304;
    case 0x5306: goto L5306;
    case 0x530B: goto L530B;
    case 0x530D: goto L530D;
    case 0x530E: goto L530E;
    case 0x530F: goto L530F;
    case 0x5310: goto L5310;
    case 0x5311: goto L5311;
    case 0x5312: goto L5312;
    case 0x5313: goto L5313;
    case 0x5314: goto L5314;
    case 0x5315: goto L5315;
    case 0x5317: goto L5317;
    case 0x5318: goto L5318;
    case 0x5319: goto L5319;
    case 0x531A: goto L531A;
    case 0x531B: goto L531B;
    case 0x531D: goto L531D;
    case 0x5322: goto L5322;
    case 0x5324: goto L5324;
    case 0x5329: goto L5329;
    case 0x532C: goto L532C;
    case 0x532E: goto L532E;
    case 0x5330: goto L5330;
    case 0x5331: goto L5331;
    case 0x5333: goto L5333;
    case 0x5336: goto L5336;
    case 0x5337: goto L5337;
    case 0x533A: goto L533A;
    case 0x533B: goto L533B;
    case 0x5340: goto L5340;
    case 0x5342: goto L5342;
    case 0x5347: goto L5347;
    case 0x5349: goto L5349;
    case 0x534A: goto L534A;
    case 0x534B: goto L534B;
    case 0x534C: goto L534C;
    case 0x534D: goto L534D;
    case 0x534E: goto L534E;
    case 0x534F: goto L534F;
    case 0x5350: goto L5350;
    case 0x5351: goto L5351;
    case 0x5353: goto L5353;
    case 0x5354: goto L5354;
    case 0x5355: goto L5355;
    case 0x5356: goto L5356;
    case 0x5357: goto L5357;
    case 0x5359: goto L5359;
    case 0x535E: goto L535E;
    case 0x5360: goto L5360;
    case 0x5365: goto L5365;
    case 0x5368: goto L5368;
    case 0x536A: goto L536A;
    case 0x536C: goto L536C;
    case 0x536D: goto L536D;
    case 0x536F: goto L536F;
    case 0x5372: goto L5372;
    case 0x5373: goto L5373;
    case 0x5376: goto L5376;
    case 0x5377: goto L5377;
    case 0x537C: goto L537C;
    case 0x537E: goto L537E;
    case 0x5383: goto L5383;
    case 0x5385: goto L5385;
    case 0x5386: goto L5386;
    case 0x5387: goto L5387;
    case 0x5388: goto L5388;
    case 0x5389: goto L5389;
    case 0x538A: goto L538A;
    case 0x538B: goto L538B;
    case 0x538C: goto L538C;
    case 0x538D: goto L538D;
    case 0x538F: goto L538F;
    case 0x5390: goto L5390;
    case 0x5391: goto L5391;
    case 0x5392: goto L5392;
    case 0x5393: goto L5393;
    case 0x5395: goto L5395;
    case 0x539A: goto L539A;
    case 0x539C: goto L539C;
    case 0x53A1: goto L53A1;
    case 0x53A4: goto L53A4;
    case 0x53A7: goto L53A7;
    case 0x53AA: goto L53AA;
    case 0x53AD: goto L53AD;
    case 0x53AF: goto L53AF;
    case 0x53B1: goto L53B1;
    case 0x53B2: goto L53B2;
    case 0x53B4: goto L53B4;
    case 0x53B7: goto L53B7;
    case 0x53B8: goto L53B8;
    case 0x53BA: goto L53BA;
    case 0x53BD: goto L53BD;
    case 0x53BE: goto L53BE;
    case 0x53C3: goto L53C3;
    case 0x53C5: goto L53C5;
    case 0x53CA: goto L53CA;
    case 0x53CC: goto L53CC;
    case 0x53CD: goto L53CD;
    case 0x53CE: goto L53CE;
    case 0x53CF: goto L53CF;
    case 0x53D0: goto L53D0;
    case 0x53D1: goto L53D1;
    case 0x53D2: goto L53D2;
    case 0x53D3: goto L53D3;
    case 0x53D4: goto L53D4;
    case 0x53D6: goto L53D6;
    case 0x53D7: goto L53D7;
    case 0x53D8: goto L53D8;
    case 0x53D9: goto L53D9;
    case 0x53DA: goto L53DA;
    case 0x53DC: goto L53DC;
    case 0x53E1: goto L53E1;
    case 0x53E3: goto L53E3;
    case 0x53E8: goto L53E8;
    case 0x53EB: goto L53EB;
    case 0x53EE: goto L53EE;
    case 0x53F1: goto L53F1;
    case 0x53F4: goto L53F4;
    case 0x53F6: goto L53F6;
    case 0x53F8: goto L53F8;
    case 0x53F9: goto L53F9;
    case 0x53FB: goto L53FB;
    case 0x53FE: goto L53FE;
    case 0x53FF: goto L53FF;
    case 0x5402: goto L5402;
    case 0x5403: goto L5403;
    case 0x5408: goto L5408;
    case 0x540A: goto L540A;
    case 0x540F: goto L540F;
    case 0x5411: goto L5411;
    case 0x5412: goto L5412;
    case 0x5413: goto L5413;
    case 0x5414: goto L5414;
    case 0x5415: goto L5415;
    case 0x5416: goto L5416;
    case 0x5417: goto L5417;
    case 0x5418: goto L5418;
    case 0x5419: goto L5419;
    case 0x541B: goto L541B;
    case 0x541C: goto L541C;
    case 0x541D: goto L541D;
    case 0x541E: goto L541E;
    case 0x541F: goto L541F;
    case 0x5421: goto L5421;
    case 0x5426: goto L5426;
    case 0x5428: goto L5428;
    case 0x542D: goto L542D;
    case 0x5430: goto L5430;
    case 0x5433: goto L5433;
    case 0x5436: goto L5436;
    case 0x5439: goto L5439;
    case 0x543B: goto L543B;
    case 0x543D: goto L543D;
    case 0x543E: goto L543E;
    case 0x5440: goto L5440;
    case 0x5443: goto L5443;
    case 0x5444: goto L5444;
    case 0x5447: goto L5447;
    case 0x5448: goto L5448;
    case 0x544D: goto L544D;
    case 0x544F: goto L544F;
    case 0x5454: goto L5454;
    case 0x5456: goto L5456;
    case 0x5457: goto L5457;
    case 0x5458: goto L5458;
    case 0x5459: goto L5459;
    case 0x545A: goto L545A;
    case 0x545B: goto L545B;
    case 0x545C: goto L545C;
    case 0x545D: goto L545D;
    case 0x545E: goto L545E;
    case 0x5460: goto L5460;
    case 0x5461: goto L5461;
    case 0x5462: goto L5462;
    case 0x5463: goto L5463;
    case 0x5464: goto L5464;
    case 0x5466: goto L5466;
    case 0x546B: goto L546B;
    case 0x546D: goto L546D;
    case 0x5472: goto L5472;
    case 0x5475: goto L5475;
    case 0x5478: goto L5478;
    case 0x547B: goto L547B;
    case 0x547E: goto L547E;
    case 0x5480: goto L5480;
    case 0x5482: goto L5482;
    case 0x5483: goto L5483;
    case 0x5485: goto L5485;
    case 0x5488: goto L5488;
    case 0x5489: goto L5489;
    case 0x548B: goto L548B;
    case 0x548E: goto L548E;
    case 0x548F: goto L548F;
    case 0x5494: goto L5494;
    case 0x5496: goto L5496;
    case 0x549B: goto L549B;
    case 0x549D: goto L549D;
    case 0x549E: goto L549E;
    case 0x549F: goto L549F;
    case 0x54A0: goto L54A0;
    case 0x54A1: goto L54A1;
    case 0x54A2: goto L54A2;
    case 0x54A3: goto L54A3;
    case 0x54A4: goto L54A4;
    case 0x54A5: goto L54A5;
    case 0x54A7: goto L54A7;
    case 0x54A8: goto L54A8;
    case 0x54A9: goto L54A9;
    case 0x54AA: goto L54AA;
    case 0x54AB: goto L54AB;
    case 0x54AD: goto L54AD;
    case 0x54B2: goto L54B2;
    case 0x54B4: goto L54B4;
    case 0x54B9: goto L54B9;
    case 0x54BC: goto L54BC;
    case 0x54BF: goto L54BF;
    case 0x54C2: goto L54C2;
    case 0x54C5: goto L54C5;
    case 0x54C7: goto L54C7;
    case 0x54C9: goto L54C9;
    case 0x54CA: goto L54CA;
    case 0x54CC: goto L54CC;
    case 0x54CF: goto L54CF;
    case 0x54D0: goto L54D0;
    case 0x54D2: goto L54D2;
    case 0x54D5: goto L54D5;
    case 0x54D6: goto L54D6;
    case 0x54DB: goto L54DB;
    case 0x54DD: goto L54DD;
    case 0x54E2: goto L54E2;
    case 0x54E4: goto L54E4;
    case 0x54E5: goto L54E5;
    case 0x54E6: goto L54E6;
    case 0x54E7: goto L54E7;
    case 0x54E8: goto L54E8;
    case 0x54E9: goto L54E9;
    case 0x54EA: goto L54EA;
    case 0x54EB: goto L54EB;
    case 0x54EC: goto L54EC;
    case 0x54EE: goto L54EE;
    case 0x54EF: goto L54EF;
    case 0x54F0: goto L54F0;
    case 0x54F1: goto L54F1;
    case 0x54F2: goto L54F2;
    case 0x54F4: goto L54F4;
    case 0x54F9: goto L54F9;
    case 0x54FB: goto L54FB;
    case 0x5500: goto L5500;
    case 0x5503: goto L5503;
    case 0x5506: goto L5506;
    case 0x5509: goto L5509;
    case 0x550C: goto L550C;
    case 0x550F: goto L550F;
    case 0x5511: goto L5511;
    case 0x5513: goto L5513;
    case 0x5514: goto L5514;
    case 0x5516: goto L5516;
    case 0x5519: goto L5519;
    case 0x551A: goto L551A;
    case 0x551C: goto L551C;
    case 0x551F: goto L551F;
    case 0x5520: goto L5520;
    case 0x5525: goto L5525;
    case 0x5527: goto L5527;
    case 0x552C: goto L552C;
    case 0x552E: goto L552E;
    case 0x552F: goto L552F;
    case 0x5530: goto L5530;
    case 0x5531: goto L5531;
    case 0x5532: goto L5532;
    case 0x5533: goto L5533;
    case 0x5534: goto L5534;
    case 0x5535: goto L5535;
    case 0x5536: goto L5536;
    case 0x5538: goto L5538;
    case 0x5539: goto L5539;
    case 0x553A: goto L553A;
    case 0x553B: goto L553B;
    case 0x553C: goto L553C;
    case 0x553E: goto L553E;
    case 0x5543: goto L5543;
    case 0x5545: goto L5545;
    case 0x554A: goto L554A;
    case 0x554D: goto L554D;
    case 0x5550: goto L5550;
    case 0x5553: goto L5553;
    case 0x5556: goto L5556;
    case 0x5559: goto L5559;
    case 0x555B: goto L555B;
    case 0x555D: goto L555D;
    case 0x555E: goto L555E;
    case 0x5560: goto L5560;
    case 0x5563: goto L5563;
    case 0x5564: goto L5564;
    case 0x5566: goto L5566;
    case 0x5569: goto L5569;
    case 0x556A: goto L556A;
    case 0x556F: goto L556F;
    case 0x5571: goto L5571;
    case 0x5576: goto L5576;
    case 0x5578: goto L5578;
    case 0x5579: goto L5579;
    case 0x557A: goto L557A;
    case 0x557B: goto L557B;
    case 0x557C: goto L557C;
    case 0x557D: goto L557D;
    case 0x557E: goto L557E;
    case 0x557F: goto L557F;
    case 0x5580: goto L5580;
    case 0x5582: goto L5582;
    case 0x5583: goto L5583;
    case 0x5584: goto L5584;
    case 0x5585: goto L5585;
    case 0x5586: goto L5586;
    case 0x5588: goto L5588;
    case 0x558D: goto L558D;
    case 0x558F: goto L558F;
    case 0x5594: goto L5594;
    case 0x5597: goto L5597;
    case 0x5599: goto L5599;
    case 0x559B: goto L559B;
    case 0x559C: goto L559C;
    case 0x559E: goto L559E;
    case 0x55A1: goto L55A1;
    case 0x55A2: goto L55A2;
    case 0x55A5: goto L55A5;
    case 0x55A6: goto L55A6;
    case 0x55AB: goto L55AB;
    case 0x55AD: goto L55AD;
    case 0x55B2: goto L55B2;
    case 0x55B4: goto L55B4;
    case 0x55B5: goto L55B5;
    case 0x55B6: goto L55B6;
    case 0x55B7: goto L55B7;
    case 0x55B8: goto L55B8;
    case 0x55B9: goto L55B9;
    case 0x55BA: goto L55BA;
    case 0x55BB: goto L55BB;
    case 0x55BC: goto L55BC;
    case 0x55BE: goto L55BE;
    case 0x55BF: goto L55BF;
    case 0x55C0: goto L55C0;
    case 0x55C1: goto L55C1;
    case 0x55C2: goto L55C2;
    case 0x55C4: goto L55C4;
    case 0x55C9: goto L55C9;
    case 0x55CB: goto L55CB;
    case 0x55D0: goto L55D0;
    case 0x55D3: goto L55D3;
    case 0x55D6: goto L55D6;
    case 0x55D9: goto L55D9;
    case 0x55DC: goto L55DC;
    case 0x55DF: goto L55DF;
    case 0x55E1: goto L55E1;
    case 0x55E3: goto L55E3;
    case 0x55E4: goto L55E4;
    case 0x55E6: goto L55E6;
    case 0x55E9: goto L55E9;
    case 0x55EA: goto L55EA;
    case 0x55EC: goto L55EC;
    case 0x55EF: goto L55EF;
    case 0x55F0: goto L55F0;
    case 0x55F5: goto L55F5;
    case 0x55F7: goto L55F7;
    case 0x55FC: goto L55FC;
    case 0x55FE: goto L55FE;
    case 0x55FF: goto L55FF;
    case 0x5600: goto L5600;
    case 0x5601: goto L5601;
    case 0x5602: goto L5602;
    case 0x5603: goto L5603;
    case 0x5604: goto L5604;
    case 0x5605: goto L5605;
    case 0x5606: goto L5606;
    case 0x5608: goto L5608;
    case 0x5609: goto L5609;
    case 0x560A: goto L560A;
    case 0x560B: goto L560B;
    case 0x560C: goto L560C;
    case 0x560E: goto L560E;
    case 0x5613: goto L5613;
    case 0x5615: goto L5615;
    case 0x561A: goto L561A;
    case 0x561D: goto L561D;
    case 0x5620: goto L5620;
    case 0x5623: goto L5623;
    case 0x5626: goto L5626;
    case 0x5629: goto L5629;
    case 0x562B: goto L562B;
    case 0x562D: goto L562D;
    case 0x562E: goto L562E;
    case 0x5630: goto L5630;
    case 0x5633: goto L5633;
    case 0x5634: goto L5634;
    case 0x5636: goto L5636;
    case 0x5639: goto L5639;
    case 0x563A: goto L563A;
    case 0x563F: goto L563F;
    case 0x5641: goto L5641;
    case 0x5646: goto L5646;
    case 0x5648: goto L5648;
    case 0x5649: goto L5649;
    case 0x564A: goto L564A;
    case 0x564B: goto L564B;
    case 0x564C: goto L564C;
    case 0x564D: goto L564D;
    case 0x564E: goto L564E;
    case 0x564F: goto L564F;
    case 0x5650: goto L5650;
    case 0x5652: goto L5652;
    case 0x5655: goto L5655;
    case 0x5658: goto L5658;
    case 0x565B: goto L565B;
    case 0x565E: goto L565E;
    case 0x5660: goto L5660;
    case 0x5661: goto L5661;
    case 0x5662: goto L5662;
    case 0x5663: goto L5663;
    case 0x5664: goto L5664;
    case 0x5666: goto L5666;
    case 0x566B: goto L566B;
    case 0x566D: goto L566D;
    case 0x5672: goto L5672;
    case 0x5675: goto L5675;
    case 0x5677: goto L5677;
    case 0x5679: goto L5679;
    case 0x567A: goto L567A;
    case 0x567C: goto L567C;
    case 0x567F: goto L567F;
    case 0x5680: goto L5680;
    case 0x5682: goto L5682;
    case 0x5686: goto L5686;
    case 0x568C: goto L568C;
    case 0x568F: goto L568F;
    case 0x5693: goto L5693;
    case 0x5694: goto L5694;
    case 0x5699: goto L5699;
    case 0x569B: goto L569B;
    case 0x56A0: goto L56A0;
    case 0x56A2: goto L56A2;
    case 0x56A3: goto L56A3;
    case 0x56A4: goto L56A4;
    case 0x56A5: goto L56A5;
    case 0x56A6: goto L56A6;
    case 0x56A7: goto L56A7;
    case 0x56A8: goto L56A8;
    case 0x56A9: goto L56A9;
    case 0x56AA: goto L56AA;
    case 0x56AC: goto L56AC;
    case 0x56AD: goto L56AD;
    case 0x56AE: goto L56AE;
    case 0x56AF: goto L56AF;
    case 0x56B0: goto L56B0;
    case 0x56B2: goto L56B2;
    case 0x56B7: goto L56B7;
    case 0x56B9: goto L56B9;
    case 0x56BE: goto L56BE;
    case 0x56C1: goto L56C1;
    case 0x56C4: goto L56C4;
    case 0x56C7: goto L56C7;
    case 0x56CA: goto L56CA;
    case 0x56CD: goto L56CD;
    case 0x56CF: goto L56CF;
    case 0x56D1: goto L56D1;
    case 0x56D2: goto L56D2;
    case 0x56D4: goto L56D4;
    case 0x56D7: goto L56D7;
    case 0x56D8: goto L56D8;
    case 0x56DA: goto L56DA;
    case 0x56DD: goto L56DD;
    case 0x56DE: goto L56DE;
    case 0x56E3: goto L56E3;
    case 0x56E5: goto L56E5;
    case 0x56EA: goto L56EA;
    case 0x56EC: goto L56EC;
    case 0x56ED: goto L56ED;
    case 0x56EE: goto L56EE;
    case 0x56EF: goto L56EF;
    case 0x56F0: goto L56F0;
    case 0x56F1: goto L56F1;
    case 0x56F2: goto L56F2;
    case 0x56F3: goto L56F3;
    case 0x56F4: goto L56F4;
    case 0x56F6: goto L56F6;
    case 0x56F7: goto L56F7;
    case 0x56F8: goto L56F8;
    case 0x56F9: goto L56F9;
    case 0x56FA: goto L56FA;
    case 0x56FC: goto L56FC;
    case 0x5701: goto L5701;
    case 0x5703: goto L5703;
    case 0x5708: goto L5708;
    case 0x570B: goto L570B;
    case 0x570E: goto L570E;
    case 0x5710: goto L5710;
    case 0x5712: goto L5712;
    case 0x5713: goto L5713;
    case 0x5715: goto L5715;
    case 0x5718: goto L5718;
    case 0x5719: goto L5719;
    case 0x571C: goto L571C;
    case 0x571D: goto L571D;
    case 0x5722: goto L5722;
    case 0x5724: goto L5724;
    case 0x5729: goto L5729;
    case 0x572B: goto L572B;
    case 0x572C: goto L572C;
    case 0x572D: goto L572D;
    case 0x572E: goto L572E;
    case 0x572F: goto L572F;
    case 0x5730: goto L5730;
    case 0x5731: goto L5731;
    case 0x5732: goto L5732;
    case 0x5733: goto L5733;
    case 0x5735: goto L5735;
    case 0x5736: goto L5736;
    case 0x5737: goto L5737;
    case 0x5738: goto L5738;
    case 0x5739: goto L5739;
    case 0x573B: goto L573B;
    case 0x5740: goto L5740;
    case 0x5742: goto L5742;
    case 0x5747: goto L5747;
    case 0x574A: goto L574A;
    case 0x574D: goto L574D;
    case 0x574F: goto L574F;
    case 0x5751: goto L5751;
    case 0x5752: goto L5752;
    case 0x5754: goto L5754;
    case 0x5757: goto L5757;
    case 0x5758: goto L5758;
    case 0x575B: goto L575B;
    case 0x575C: goto L575C;
    case 0x5761: goto L5761;
    case 0x5763: goto L5763;
    case 0x5768: goto L5768;
    case 0x576A: goto L576A;
    case 0x576B: goto L576B;
    case 0x576C: goto L576C;
    case 0x576D: goto L576D;
    case 0x576E: goto L576E;
    case 0x576F: goto L576F;
    case 0x5770: goto L5770;
    case 0x5771: goto L5771;
    case 0x5772: goto L5772;
    case 0x5774: goto L5774;
    case 0x5775: goto L5775;
    case 0x5776: goto L5776;
    case 0x5777: goto L5777;
    case 0x5778: goto L5778;
    case 0x577A: goto L577A;
    case 0x577F: goto L577F;
    case 0x5781: goto L5781;
    case 0x5786: goto L5786;
    case 0x5789: goto L5789;
    case 0x578C: goto L578C;
    case 0x578F: goto L578F;
    case 0x5792: goto L5792;
    case 0x5795: goto L5795;
    case 0x5797: goto L5797;
    case 0x5799: goto L5799;
    case 0x579A: goto L579A;
    case 0x579C: goto L579C;
    case 0x579F: goto L579F;
    case 0x57A0: goto L57A0;
    case 0x57A2: goto L57A2;
    case 0x57A5: goto L57A5;
    case 0x57A6: goto L57A6;
    case 0x57AB: goto L57AB;
    case 0x57AD: goto L57AD;
    case 0x57B2: goto L57B2;
    case 0x57B4: goto L57B4;
    case 0x57B5: goto L57B5;
    case 0x57B6: goto L57B6;
    case 0x57B7: goto L57B7;
    case 0x57B8: goto L57B8;
    case 0x57B9: goto L57B9;
    case 0x57BA: goto L57BA;
    case 0x57BB: goto L57BB;
    case 0x57BC: goto L57BC;
    case 0x57BE: goto L57BE;
    case 0x57BF: goto L57BF;
    case 0x57C0: goto L57C0;
    case 0x57C1: goto L57C1;
    case 0x57C2: goto L57C2;
    case 0x57C4: goto L57C4;
    case 0x57C9: goto L57C9;
    case 0x57CB: goto L57CB;
    case 0x57D0: goto L57D0;
    case 0x57D3: goto L57D3;
    case 0x57D5: goto L57D5;
    case 0x57D8: goto L57D8;
    case 0x57DC: goto L57DC;
    case 0x57DF: goto L57DF;
    case 0x57E3: goto L57E3;
    case 0x57E6: goto L57E6;
    case 0x57E9: goto L57E9;
    case 0x57EC: goto L57EC;
    case 0x57EF: goto L57EF;
    case 0x57F2: goto L57F2;
    case 0x57F5: goto L57F5;
    case 0x57F8: goto L57F8;
    case 0x57FA: goto L57FA;
    case 0x57FC: goto L57FC;
    case 0x57FD: goto L57FD;
    case 0x57FF: goto L57FF;
    case 0x5802: goto L5802;
    case 0x5803: goto L5803;
    case 0x5805: goto L5805;
    case 0x5808: goto L5808;
    case 0x5809: goto L5809;
    case 0x580E: goto L580E;
    case 0x5810: goto L5810;
    case 0x5815: goto L5815;
    case 0x5817: goto L5817;
    case 0x5818: goto L5818;
    case 0x5819: goto L5819;
    case 0x581A: goto L581A;
    case 0x581B: goto L581B;
    case 0x581C: goto L581C;
    case 0x581D: goto L581D;
    case 0x581E: goto L581E;
    case 0x581F: goto L581F;
    case 0x5821: goto L5821;
    case 0x5822: goto L5822;
    case 0x5823: goto L5823;
    case 0x5824: goto L5824;
    case 0x5825: goto L5825;
    case 0x5827: goto L5827;
    case 0x582C: goto L582C;
    case 0x582E: goto L582E;
    case 0x5833: goto L5833;
    case 0x5836: goto L5836;
    case 0x5838: goto L5838;
    case 0x583B: goto L583B;
    case 0x583F: goto L583F;
    case 0x5842: goto L5842;
    case 0x5846: goto L5846;
    case 0x5849: goto L5849;
    case 0x584C: goto L584C;
    case 0x584F: goto L584F;
    case 0x5852: goto L5852;
    case 0x5855: goto L5855;
    case 0x5858: goto L5858;
    case 0x585B: goto L585B;
    case 0x585D: goto L585D;
    case 0x585F: goto L585F;
    case 0x5860: goto L5860;
    case 0x5862: goto L5862;
    case 0x5865: goto L5865;
    case 0x5866: goto L5866;
    case 0x5868: goto L5868;
    case 0x586B: goto L586B;
    case 0x586C: goto L586C;
    case 0x5871: goto L5871;
    case 0x5873: goto L5873;
    case 0x5878: goto L5878;
    case 0x587A: goto L587A;
    case 0x587B: goto L587B;
    case 0x587C: goto L587C;
    case 0x587D: goto L587D;
    case 0x587E: goto L587E;
    case 0x587F: goto L587F;
    case 0x5880: goto L5880;
    case 0x5881: goto L5881;
    case 0x5882: goto L5882;
    case 0x5884: goto L5884;
    case 0x5885: goto L5885;
    case 0x5886: goto L5886;
    case 0x5887: goto L5887;
    case 0x5888: goto L5888;
    case 0x588A: goto L588A;
    case 0x588F: goto L588F;
    case 0x5891: goto L5891;
    case 0x5896: goto L5896;
    case 0x5899: goto L5899;
    case 0x589B: goto L589B;
    case 0x589D: goto L589D;
    case 0x58A1: goto L58A1;
    case 0x58A5: goto L58A5;
    case 0x58A8: goto L58A8;
    case 0x58AB: goto L58AB;
    case 0x58AE: goto L58AE;
    case 0x58B1: goto L58B1;
    case 0x58B4: goto L58B4;
    case 0x58B7: goto L58B7;
    case 0x58BA: goto L58BA;
    case 0x58BC: goto L58BC;
    case 0x58BE: goto L58BE;
    case 0x58BF: goto L58BF;
    case 0x58C1: goto L58C1;
    case 0x58C4: goto L58C4;
    case 0x58C5: goto L58C5;
    case 0x58C7: goto L58C7;
    case 0x58CA: goto L58CA;
    case 0x58CB: goto L58CB;
    case 0x58D0: goto L58D0;
    case 0x58D2: goto L58D2;
    case 0x58D7: goto L58D7;
    case 0x58D9: goto L58D9;
    case 0x58DA: goto L58DA;
    case 0x58DB: goto L58DB;
    case 0x58DC: goto L58DC;
    case 0x58DD: goto L58DD;
    case 0x58DE: goto L58DE;
    case 0x58DF: goto L58DF;
    case 0x58E0: goto L58E0;
    case 0x58E1: goto L58E1;
    case 0x58E3: goto L58E3;
    case 0x58E4: goto L58E4;
    case 0x58E5: goto L58E5;
    case 0x58E6: goto L58E6;
    case 0x58E7: goto L58E7;
    case 0x58E9: goto L58E9;
    case 0x58EE: goto L58EE;
    case 0x58F0: goto L58F0;
    case 0x58F5: goto L58F5;
    case 0x58F8: goto L58F8;
    case 0x58FB: goto L58FB;
    case 0x58FE: goto L58FE;
    case 0x5901: goto L5901;
    case 0x5904: goto L5904;
    case 0x5906: goto L5906;
    case 0x5908: goto L5908;
    case 0x5909: goto L5909;
    case 0x590B: goto L590B;
    case 0x590E: goto L590E;
    case 0x590F: goto L590F;
    case 0x5911: goto L5911;
    case 0x5914: goto L5914;
    case 0x5915: goto L5915;
    case 0x5916: goto L5916;
    case 0x5918: goto L5918;
    case 0x5919: goto L5919;
    case 0x591A: goto L591A;
    case 0x591B: goto L591B;
    case 0x591C: goto L591C;
    case 0x591E: goto L591E;
    case 0x5923: goto L5923;
    case 0x5925: goto L5925;
    case 0x592A: goto L592A;
    case 0x592D: goto L592D;
    case 0x592F: goto L592F;
    case 0x5931: goto L5931;
    case 0x5935: goto L5935;
    case 0x5939: goto L5939;
    case 0x593C: goto L593C;
    case 0x593F: goto L593F;
    case 0x5942: goto L5942;
    case 0x5945: goto L5945;
    case 0x5948: goto L5948;
    case 0x594B: goto L594B;
    case 0x594E: goto L594E;
    case 0x5950: goto L5950;
    case 0x5952: goto L5952;
    case 0x5953: goto L5953;
    case 0x5955: goto L5955;
    case 0x5958: goto L5958;
    case 0x5959: goto L5959;
    case 0x595B: goto L595B;
    case 0x595E: goto L595E;
    case 0x595F: goto L595F;
    case 0x5964: goto L5964;
    case 0x5966: goto L5966;
    case 0x596B: goto L596B;
    case 0x596D: goto L596D;
    case 0x596E: goto L596E;
    case 0x596F: goto L596F;
    case 0x5970: goto L5970;
    case 0x5971: goto L5971;
    case 0x5972: goto L5972;
    case 0x5973: goto L5973;
    case 0x5974: goto L5974;
    case 0x5975: goto L5975;
    case 0x5977: goto L5977;
    case 0x5978: goto L5978;
    case 0x5979: goto L5979;
    case 0x597A: goto L597A;
    case 0x597B: goto L597B;
    case 0x597D: goto L597D;
    case 0x5982: goto L5982;
    case 0x5984: goto L5984;
    case 0x5989: goto L5989;
    case 0x598C: goto L598C;
    case 0x598F: goto L598F;
    case 0x5992: goto L5992;
    case 0x5995: goto L5995;
    case 0x5998: goto L5998;
    case 0x599B: goto L599B;
    case 0x599D: goto L599D;
    case 0x599F: goto L599F;
    case 0x59A0: goto L59A0;
    case 0x59A2: goto L59A2;
    case 0x59A5: goto L59A5;
    case 0x59A6: goto L59A6;
    case 0x59A8: goto L59A8;
    case 0x59AB: goto L59AB;
    case 0x59AC: goto L59AC;
    case 0x59B1: goto L59B1;
    case 0x59B3: goto L59B3;
    case 0x59B8: goto L59B8;
    case 0x59BA: goto L59BA;
    case 0x59BB: goto L59BB;
    case 0x59BC: goto L59BC;
    case 0x59BD: goto L59BD;
    case 0x59BE: goto L59BE;
    case 0x59BF: goto L59BF;
    case 0x59C0: goto L59C0;
    case 0x59C1: goto L59C1;
    case 0x59C2: goto L59C2;
    case 0x59C4: goto L59C4;
    case 0x59C5: goto L59C5;
    case 0x59C6: goto L59C6;
    case 0x59C7: goto L59C7;
    case 0x59C8: goto L59C8;
    case 0x59CA: goto L59CA;
    case 0x59CF: goto L59CF;
    case 0x59D1: goto L59D1;
    case 0x59D6: goto L59D6;
    case 0x59D9: goto L59D9;
    case 0x59DC: goto L59DC;
    case 0x59DF: goto L59DF;
    case 0x59E2: goto L59E2;
    case 0x59E5: goto L59E5;
    case 0x59E8: goto L59E8;
    case 0x59EB: goto L59EB;
    case 0x59ED: goto L59ED;
    case 0x59EF: goto L59EF;
    case 0x59F0: goto L59F0;
    case 0x59F2: goto L59F2;
    case 0x59F5: goto L59F5;
    case 0x59F6: goto L59F6;
    case 0x59F8: goto L59F8;
    case 0x59FB: goto L59FB;
    case 0x59FC: goto L59FC;
    case 0x5A01: goto L5A01;
    case 0x5A03: goto L5A03;
    case 0x5A08: goto L5A08;
    case 0x5A0A: goto L5A0A;
    case 0x5A0B: goto L5A0B;
    case 0x5A0C: goto L5A0C;
    case 0x5A0D: goto L5A0D;
    case 0x5A0E: goto L5A0E;
    case 0x5A0F: goto L5A0F;
    case 0x5A10: goto L5A10;
    case 0x5A11: goto L5A11;
    case 0x5A12: goto L5A12;
    case 0x5A13: goto L5A13;
    case 0x5A15: goto L5A15;
    case 0x5A17: goto L5A17;
    case 0x5A18: goto L5A18;
    case 0x5A19: goto L5A19;
    case 0x5A1B: goto L5A1B;
    case 0x5A1D: goto L5A1D;
    case 0x5A20: goto L5A20;
    case 0x5A22: goto L5A22;
    case 0x5A24: goto L5A24;
    case 0x5A26: goto L5A26;
    case 0x5A27: goto L5A27;
    case 0x5A28: goto L5A28;
    case 0x5A29: goto L5A29;
    case 0x5A2A: goto L5A2A;
    case 0x5A2D: goto L5A2D;
    case 0x5A2E: goto L5A2E;
    case 0x5A30: goto L5A30;
    case 0x5A31: goto L5A31;
    case 0x5A32: goto L5A32;
    case 0x5A33: goto L5A33;
    case 0x5A35: goto L5A35;
    case 0x5A36: goto L5A36;
    case 0x5A37: goto L5A37;
    case 0x5A38: goto L5A38;
    case 0x5A39: goto L5A39;
    case 0x5A3B: goto L5A3B;
    case 0x5A40: goto L5A40;
    case 0x5A42: goto L5A42;
    case 0x5A47: goto L5A47;
    case 0x5A4A: goto L5A4A;
    case 0x5A4D: goto L5A4D;
    case 0x5A4F: goto L5A4F;
    case 0x5A51: goto L5A51;
    case 0x5A52: goto L5A52;
    case 0x5A54: goto L5A54;
    case 0x5A57: goto L5A57;
    case 0x5A58: goto L5A58;
    case 0x5A5B: goto L5A5B;
    case 0x5A5C: goto L5A5C;
    case 0x5A61: goto L5A61;
    case 0x5A63: goto L5A63;
    case 0x5A68: goto L5A68;
    case 0x5A6A: goto L5A6A;
    case 0x5A6B: goto L5A6B;
    case 0x5A6C: goto L5A6C;
    case 0x5A6D: goto L5A6D;
    case 0x5A6E: goto L5A6E;
    case 0x5A6F: goto L5A6F;
    case 0x5A70: goto L5A70;
    default: asm_bad_entry("GRCORE.ASM", entry);
    }

    /* seg003_4C74  (+4C74) */
L4C74: /* _seg003_4C74 */
    /* 4C74  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_4C75  (+4C75): no caller found; switches to the private stack and calls
       through the vector at _seg003_5A72 */
L4C75: /* _seg003_4C75 */
    /* 4C75  push    bp */
    push16(BP);
L4C76:
    /* 4C76  mov     bp,sp */
    BP = SP;
L4C78:
    /* 4C78  push    di */
    push16(DI);
L4C79:
    /* 4C79  push    si */
    push16(SI);
L4C7A:
    /* 4C7A  push    ds */
    push16(asm_ds);
L4C7B:
    /* 4C7B  push    es */
    push16(asm_es);
L4C7C:
    /* 4C7C  mov     dx,ss */
    DX = asm_ss;
L4C7E:
    /* 4C7E  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L4C83:
    /* 4C83  mov     dx,sp */
    DX = SP;
L4C85:
    /* 4C85  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L4C8A:
    /* 4C8A  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4C8D:
    /* 4C8D  mov     ds,dx */
    SET_DS(DX);
L4C8F:
    /* 4C8F  mov     es,dx */
    SET_ES(DX);
L4C91:
    /* 4C91  cli */
    ;
L4C92:
    /* 4C92  mov     ss,dx */
    SET_SS(DX);
L4C94:
    /* 4C94  mov     sp,4FA6h */
    SP = 0x4FA6;
L4C97:
    /* 4C97  sti */
    ;
L4C98:
    /* 4C98  call    _seg003_5A72 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5A72), 0x4C9B)) != 0) return c;
L4C9B:
    /* 4C9B  cli */
    ;
L4C9C:
    /* 4C9C  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L4CA1:
    /* 4CA1  mov     ss,dx */
    SET_SS(DX);
L4CA3:
    /* 4CA3  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L4CA8:
    /* 4CA8  mov     sp,dx */
    SP = DX;
L4CAA:
    /* 4CAA  sti */
    ;
L4CAB:
    /* 4CAB  pop     es */
    SET_ES(pop16());
L4CAC:
    /* 4CAC  pop     ds */
    SET_DS(pop16());
L4CAD:
    /* 4CAD  pop     si */
    SI = pop16();
L4CAE:
    /* 4CAE  pop     di */
    DI = pop16();
L4CAF:
    /* 4CAF  pop     bp */
    BP = pop16();
L4CB0:
    /* 4CB0  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* setup_font  (+4CB1) */
L4CB1: /* _setup_font */
    /* 4CB1  push    bp */
    push16(BP);
L4CB2:
    /* 4CB2  mov     bp,sp */
    BP = SP;
L4CB4:
    /* 4CB4  push    di */
    push16(DI);
L4CB5:
    /* 4CB5  push    si */
    push16(SI);
L4CB6:
    /* 4CB6  push    ds */
    push16(asm_ds);
L4CB7:
    /* 4CB7  push    es */
    push16(asm_es);
L4CB8:
    /* 4CB8  mov     dx,ss */
    DX = asm_ss;
L4CBA:
    /* 4CBA  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L4CBF:
    /* 4CBF  mov     dx,sp */
    DX = SP;
L4CC1:
    /* 4CC1  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L4CC6:
    /* 4CC6  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4CC9:
    /* 4CC9  mov     ds,dx */
    SET_DS(DX);
L4CCB:
    /* 4CCB  mov     es,dx */
    SET_ES(DX);
L4CCD:
    /* 4CCD  cli */
    ;
L4CCE:
    /* 4CCE  mov     ss,dx */
    SET_SS(DX);
L4CD0:
    /* 4CD0  mov     sp,4FA6h */
    SP = 0x4FA6;
L4CD3:
    /* 4CD3  sti */
    ;
L4CD4:
    /* 4CD4  call    _seg003_5A75 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5A75), 0x4CD7)) != 0) return c;
L4CD7:
    /* 4CD7  cli */
    ;
L4CD8:
    /* 4CD8  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L4CDD:
    /* 4CDD  mov     ss,dx */
    SET_SS(DX);
L4CDF:
    /* 4CDF  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L4CE4:
    /* 4CE4  mov     sp,dx */
    SP = DX;
L4CE6:
    /* 4CE6  sti */
    ;
L4CE7:
    /* 4CE7  pop     es */
    SET_ES(pop16());
L4CE8:
    /* 4CE8  pop     ds */
    SET_DS(pop16());
L4CE9:
    /* 4CE9  pop     si */
    SI = pop16();
L4CEA:
    /* 4CEA  pop     di */
    DI = pop16();
L4CEB:
    /* 4CEB  pop     bp */
    BP = pop16();
L4CEC:
    /* 4CEC  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* WriteTextToScreen_seg003_4CED  (+4CED) */
L4CED: /* _string_to_screen */
    /* 4CED  push    bp */
    push16(BP);
L4CEE:
    /* 4CEE  mov     bp,sp */
    BP = SP;
L4CF0:
    /* 4CF0  push    di */
    push16(DI);
L4CF1:
    /* 4CF1  push    si */
    push16(SI);
L4CF2:
    /* 4CF2  push    ds */
    push16(asm_ds);
L4CF3:
    /* 4CF3  push    es */
    push16(asm_es);
L4CF4:
    /* 4CF4  mov     dx,ss */
    DX = asm_ss;
L4CF6:
    /* 4CF6  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L4CFB:
    /* 4CFB  mov     dx,sp */
    DX = SP;
L4CFD:
    /* 4CFD  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L4D02:
    /* 4D02  les     di,dword ptr [bp+6] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0x6)), s_ = rw(pSS, (uint16_t)(BP + 0x6 + 2));
    DI = o_;
    SET_ES(s_); }
L4D05:
    /* 4D05  mov     si,di */
    SI = DI;
L4D07:
    /* 4D07  mov     cx,84h */
    CX = 0x84;
L4D0A:
    /* 4D0A  xor     al,al */
    AL = logic8((uint8_t)(AL ^ AL));
L4D0C:
    /* 4D0C  repne scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (ZF) break; }
L4D0E:
    /* 4D0E  neg     cx */
    CX = (uint16_t)-CX;
L4D10:
    /* 4D10  add     cx,85h */
    CX = (uint16_t)(CX + 0x85);
L4D14:
    /* 4D14  push    ds */
    push16(asm_ds);
L4D15:
    /* 4D15  mov     di,es */
    DI = asm_es;
L4D17:
    /* 4D17  mov     ds,di */
    SET_DS(DI);
L4D19:
    /* 4D19  mov     di,4FA6h */
    DI = 0x4FA6;
L4D1C:
    /* 4D1C  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4D1F:
    /* 4D1F  mov     es,ax */
    SET_ES(AX);
L4D21:
    /* 4D21  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L4D23:
    /* 4D23  pop     ds */
    SET_DS(pop16());
L4D24:
    /* 4D24  mov     ax,word ptr [bp+0Ah] */
    AX = rw(pSS, BP + 0xA);
L4D27:
    /* 4D27  mov     bx,word ptr [bp+0Ch] */
    BX = rw(pSS, BP + 0xC);
L4D2A:
    /* 4D2A  mov     cx,word ptr [bp+0Eh] */
    CX = rw(pSS, BP + 0xE);
L4D2D:
    /* 4D2D  mov     si,word ptr [bp+10h] */
    SI = rw(pSS, BP + 0x10);
L4D30:
    /* 4D30  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4D33:
    /* 4D33  mov     ds,dx */
    SET_DS(DX);
L4D35:
    /* 4D35  mov     es,dx */
    SET_ES(DX);
L4D37:
    /* 4D37  cli */
    ;
L4D38:
    /* 4D38  mov     ss,dx */
    SET_SS(DX);
L4D3A:
    /* 4D3A  mov     sp,4FA6h */
    SP = 0x4FA6;
L4D3D:
    /* 4D3D  sti */
    ;
L4D3E:
    /* 4D3E  mov     dx,si */
    DX = SI;
L4D40:
    /* 4D40  mov     si,4FA6h */
    SI = 0x4FA6;
L4D43:
    /* 4D43  call    _seg003_5A78 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5A78), 0x4D46)) != 0) return c;
L4D46:
    /* 4D46  cli */
    ;
L4D47:
    /* 4D47  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L4D4C:
    /* 4D4C  mov     ss,dx */
    SET_SS(DX);
L4D4E:
    /* 4D4E  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L4D53:
    /* 4D53  mov     sp,dx */
    SP = DX;
L4D55:
    /* 4D55  sti */
    ;
L4D56:
    /* 4D56  pop     es */
    SET_ES(pop16());
L4D57:
    /* 4D57  pop     ds */
    SET_DS(pop16());
L4D58:
    /* 4D58  pop     si */
    SI = pop16();
L4D59:
    /* 4D59  pop     di */
    DI = pop16();
L4D5A:
    /* 4D5A  pop     bp */
    BP = pop16();
L4D5B:
    /* 4D5B  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_4D5C  (+4D5C)
       A shadowed string (GRLIBI's _440F): (far string, x, y, ...), the same arguments as
       string_to_screen. */
L4D5C: /* _seg003_4D5C */
    /* 4D5C  push    bp */
    push16(BP);
L4D5D:
    /* 4D5D  mov     bp,sp */
    BP = SP;
L4D5F:
    /* 4D5F  push    di */
    push16(DI);
L4D60:
    /* 4D60  push    si */
    push16(SI);
L4D61:
    /* 4D61  push    ds */
    push16(asm_ds);
L4D62:
    /* 4D62  push    es */
    push16(asm_es);
L4D63:
    /* 4D63  mov     dx,ss */
    DX = asm_ss;
L4D65:
    /* 4D65  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L4D6A:
    /* 4D6A  mov     dx,sp */
    DX = SP;
L4D6C:
    /* 4D6C  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L4D71:
    /* 4D71  les     di,dword ptr [bp+6] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0x6)), s_ = rw(pSS, (uint16_t)(BP + 0x6 + 2));
    DI = o_;
    SET_ES(s_); }
L4D74:
    /* 4D74  mov     si,di */
    SI = DI;
L4D76:
    /* 4D76  mov     cx,84h */
    CX = 0x84;
L4D79:
    /* 4D79  xor     al,al */
    AL = logic8((uint8_t)(AL ^ AL));
L4D7B:
    /* 4D7B  repne scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (ZF) break; }
L4D7D:
    /* 4D7D  neg     cx */
    CX = (uint16_t)-CX;
L4D7F:
    /* 4D7F  add     cx,85h */
    CX = (uint16_t)(CX + 0x85);
L4D83:
    /* 4D83  push    ds */
    push16(asm_ds);
L4D84:
    /* 4D84  mov     di,es */
    DI = asm_es;
L4D86:
    /* 4D86  mov     ds,di */
    SET_DS(DI);
L4D88:
    /* 4D88  mov     di,4FA6h */
    DI = 0x4FA6;
L4D8B:
    /* 4D8B  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4D8E:
    /* 4D8E  mov     es,ax */
    SET_ES(AX);
L4D90:
    /* 4D90  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L4D92:
    /* 4D92  pop     ds */
    SET_DS(pop16());
L4D93:
    /* 4D93  mov     ax,word ptr [bp+0Ah] */
    AX = rw(pSS, BP + 0xA);
L4D96:
    /* 4D96  mov     bx,word ptr [bp+0Ch] */
    BX = rw(pSS, BP + 0xC);
L4D99:
    /* 4D99  mov     cx,word ptr [bp+0Eh] */
    CX = rw(pSS, BP + 0xE);
L4D9C:
    /* 4D9C  mov     si,word ptr [bp+10h] */
    SI = rw(pSS, BP + 0x10);
L4D9F:
    /* 4D9F  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4DA2:
    /* 4DA2  mov     ds,dx */
    SET_DS(DX);
L4DA4:
    /* 4DA4  mov     es,dx */
    SET_ES(DX);
L4DA6:
    /* 4DA6  cli */
    ;
L4DA7:
    /* 4DA7  mov     ss,dx */
    SET_SS(DX);
L4DA9:
    /* 4DA9  mov     sp,4FA6h */
    SP = 0x4FA6;
L4DAC:
    /* 4DAC  sti */
    ;
L4DAD:
    /* 4DAD  mov     dx,si */
    DX = SI;
L4DAF:
    /* 4DAF  mov     si,4FA6h */
    SI = 0x4FA6;
L4DB2:
    /* 4DB2  call    _seg003_5A81 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5A81), 0x4DB5)) != 0) return c;
L4DB5:
    /* 4DB5  cli */
    ;
L4DB6:
    /* 4DB6  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L4DBB:
    /* 4DBB  mov     ss,dx */
    SET_SS(DX);
L4DBD:
    /* 4DBD  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L4DC2:
    /* 4DC2  mov     sp,dx */
    SP = DX;
L4DC4:
    /* 4DC4  sti */
    ;
L4DC5:
    /* 4DC5  pop     es */
    SET_ES(pop16());
L4DC6:
    /* 4DC6  pop     ds */
    SET_DS(pop16());
L4DC7:
    /* 4DC7  pop     si */
    SI = pop16();
L4DC8:
    /* 4DC8  pop     di */
    DI = pop16();
L4DC9:
    /* 4DC9  pop     bp */
    BP = pop16();
L4DCA:
    /* 4DCA  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* WritingTextToScreen_seg003_4DCB  (+4DCB) */
L4DCB: /* _string_width */
    /* 4DCB  push    bp */
    push16(BP);
L4DCC:
    /* 4DCC  mov     bp,sp */
    BP = SP;
L4DCE:
    /* 4DCE  push    di */
    push16(DI);
L4DCF:
    /* 4DCF  push    si */
    push16(SI);
L4DD0:
    /* 4DD0  push    ds */
    push16(asm_ds);
L4DD1:
    /* 4DD1  push    es */
    push16(asm_es);
L4DD2:
    /* 4DD2  mov     dx,ss */
    DX = asm_ss;
L4DD4:
    /* 4DD4  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L4DD9:
    /* 4DD9  mov     dx,sp */
    DX = SP;
L4DDB:
    /* 4DDB  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L4DE0:
    /* 4DE0  les     di,dword ptr [bp+6] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0x6)), s_ = rw(pSS, (uint16_t)(BP + 0x6 + 2));
    DI = o_;
    SET_ES(s_); }
L4DE3:
    /* 4DE3  mov     si,di */
    SI = DI;
L4DE5:
    /* 4DE5  mov     cx,84h */
    CX = 0x84;
L4DE8:
    /* 4DE8  xor     al,al */
    AL = logic8((uint8_t)(AL ^ AL));
L4DEA:
    /* 4DEA  repne scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (ZF) break; }
L4DEC:
    /* 4DEC  neg     cx */
    CX = (uint16_t)-CX;
L4DEE:
    /* 4DEE  add     cx,85h */
    CX = (uint16_t)(CX + 0x85);
L4DF2:
    /* 4DF2  push    ds */
    push16(asm_ds);
L4DF3:
    /* 4DF3  mov     di,es */
    DI = asm_es;
L4DF5:
    /* 4DF5  mov     ds,di */
    SET_DS(DI);
L4DF7:
    /* 4DF7  mov     di,4FA6h */
    DI = 0x4FA6;
L4DFA:
    /* 4DFA  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4DFD:
    /* 4DFD  mov     es,ax */
    SET_ES(AX);
L4DFF:
    /* 4DFF  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L4E01:
    /* 4E01  pop     ds */
    SET_DS(pop16());
L4E02:
    /* 4E02  mov     ax,word ptr [bp+0Ah] */
    AX = rw(pSS, BP + 0xA);
L4E05:
    /* 4E05  mov     bx,word ptr [bp+0Ch] */
    BX = rw(pSS, BP + 0xC);
L4E08:
    /* 4E08  mov     cx,word ptr [bp+0Eh] */
    CX = rw(pSS, BP + 0xE);
L4E0B:
    /* 4E0B  mov     si,word ptr [bp+10h] */
    SI = rw(pSS, BP + 0x10);
L4E0E:
    /* 4E0E  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4E11:
    /* 4E11  mov     ds,dx */
    SET_DS(DX);
L4E13:
    /* 4E13  mov     es,dx */
    SET_ES(DX);
L4E15:
    /* 4E15  cli */
    ;
L4E16:
    /* 4E16  mov     ss,dx */
    SET_SS(DX);
L4E18:
    /* 4E18  mov     sp,4FA6h */
    SP = 0x4FA6;
L4E1B:
    /* 4E1B  sti */
    ;
L4E1C:
    /* 4E1C  mov     dx,si */
    DX = SI;
L4E1E:
    /* 4E1E  mov     si,4FA6h */
    SI = 0x4FA6;
L4E21:
    /* 4E21  call    _seg003_4C45 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x4C45), 0x4E24)) != 0) return c;
L4E24:
    /* 4E24  cli */
    ;
L4E25:
    /* 4E25  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L4E2A:
    /* 4E2A  mov     ss,dx */
    SET_SS(DX);
L4E2C:
    /* 4E2C  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L4E31:
    /* 4E31  mov     sp,dx */
    SP = DX;
L4E33:
    /* 4E33  sti */
    ;
L4E34:
    /* 4E34  pop     es */
    SET_ES(pop16());
L4E35:
    /* 4E35  pop     ds */
    SET_DS(pop16());
L4E36:
    /* 4E36  pop     si */
    SI = pop16();
L4E37:
    /* 4E37  pop     di */
    DI = pop16();
L4E38:
    /* 4E38  pop     bp */
    BP = pop16();
L4E39:
    /* 4E39  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_4E3A  (+4E3A)
       A string drawn through the bitmap blitter (GRLIBI's _439E), with string_to_screen's arguments. */
L4E3A: /* _seg003_4E3A */
    /* 4E3A  push    bp */
    push16(BP);
L4E3B:
    /* 4E3B  mov     bp,sp */
    BP = SP;
L4E3D:
    /* 4E3D  push    di */
    push16(DI);
L4E3E:
    /* 4E3E  push    si */
    push16(SI);
L4E3F:
    /* 4E3F  push    ds */
    push16(asm_ds);
L4E40:
    /* 4E40  push    es */
    push16(asm_es);
L4E41:
    /* 4E41  mov     dx,ss */
    DX = asm_ss;
L4E43:
    /* 4E43  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L4E48:
    /* 4E48  mov     dx,sp */
    DX = SP;
L4E4A:
    /* 4E4A  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L4E4F:
    /* 4E4F  les     di,dword ptr [bp+6] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0x6)), s_ = rw(pSS, (uint16_t)(BP + 0x6 + 2));
    DI = o_;
    SET_ES(s_); }
L4E52:
    /* 4E52  mov     si,di */
    SI = DI;
L4E54:
    /* 4E54  mov     cx,84h */
    CX = 0x84;
L4E57:
    /* 4E57  xor     al,al */
    AL = logic8((uint8_t)(AL ^ AL));
L4E59:
    /* 4E59  repne scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (ZF) break; }
L4E5B:
    /* 4E5B  neg     cx */
    CX = (uint16_t)-CX;
L4E5D:
    /* 4E5D  add     cx,85h */
    CX = (uint16_t)(CX + 0x85);
L4E61:
    /* 4E61  push    ds */
    push16(asm_ds);
L4E62:
    /* 4E62  mov     di,es */
    DI = asm_es;
L4E64:
    /* 4E64  mov     ds,di */
    SET_DS(DI);
L4E66:
    /* 4E66  mov     di,4FA6h */
    DI = 0x4FA6;
L4E69:
    /* 4E69  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4E6C:
    /* 4E6C  mov     es,ax */
    SET_ES(AX);
L4E6E:
    /* 4E6E  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L4E70:
    /* 4E70  pop     ds */
    SET_DS(pop16());
L4E71:
    /* 4E71  mov     ax,word ptr [bp+0Ah] */
    AX = rw(pSS, BP + 0xA);
L4E74:
    /* 4E74  mov     bx,word ptr [bp+0Ch] */
    BX = rw(pSS, BP + 0xC);
L4E77:
    /* 4E77  mov     cx,word ptr [bp+0Eh] */
    CX = rw(pSS, BP + 0xE);
L4E7A:
    /* 4E7A  mov     si,word ptr [bp+10h] */
    SI = rw(pSS, BP + 0x10);
L4E7D:
    /* 4E7D  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4E80:
    /* 4E80  mov     ds,dx */
    SET_DS(DX);
L4E82:
    /* 4E82  mov     es,dx */
    SET_ES(DX);
L4E84:
    /* 4E84  cli */
    ;
L4E85:
    /* 4E85  mov     ss,dx */
    SET_SS(DX);
L4E87:
    /* 4E87  mov     sp,4FA6h */
    SP = 0x4FA6;
L4E8A:
    /* 4E8A  sti */
    ;
L4E8B:
    /* 4E8B  mov     dx,si */
    DX = SI;
L4E8D:
    /* 4E8D  mov     si,4FA6h */
    SI = 0x4FA6;
L4E90:
    /* 4E90  call    _seg003_5A87 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5A87), 0x4E93)) != 0) return c;
L4E93:
    /* 4E93  cli */
    ;
L4E94:
    /* 4E94  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L4E99:
    /* 4E99  mov     ss,dx */
    SET_SS(DX);
L4E9B:
    /* 4E9B  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L4EA0:
    /* 4EA0  mov     sp,dx */
    SP = DX;
L4EA2:
    /* 4EA2  sti */
    ;
L4EA3:
    /* 4EA3  pop     es */
    SET_ES(pop16());
L4EA4:
    /* 4EA4  pop     ds */
    SET_DS(pop16());
L4EA5:
    /* 4EA5  pop     si */
    SI = pop16();
L4EA6:
    /* 4EA6  pop     di */
    DI = pop16();
L4EA7:
    /* 4EA7  pop     bp */
    BP = pop16();
L4EA8:
    /* 4EA8  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* init_graphics  (+4EA9) */
L4EA9: /* _init_graphics */
    /* 4EA9  push    bp */
    push16(BP);
L4EAA:
    /* 4EAA  mov     bp,sp */
    BP = SP;
L4EAC:
    /* 4EAC  push    di */
    push16(DI);
L4EAD:
    /* 4EAD  push    si */
    push16(SI);
L4EAE:
    /* 4EAE  push    ds */
    push16(asm_ds);
L4EAF:
    /* 4EAF  push    es */
    push16(asm_es);
L4EB0:
    /* 4EB0  mov     dx,ss */
    DX = asm_ss;
L4EB2:
    /* 4EB2  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L4EB7:
    /* 4EB7  mov     dx,sp */
    DX = SP;
L4EB9:
    /* 4EB9  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L4EBE:
    /* 4EBE  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4EC1:
    /* 4EC1  mov     ds,dx */
    SET_DS(DX);
L4EC3:
    /* 4EC3  mov     es,dx */
    SET_ES(DX);
L4EC5:
    /* 4EC5  cli */
    ;
L4EC6:
    /* 4EC6  mov     ss,dx */
    SET_SS(DX);
L4EC8:
    /* 4EC8  mov     sp,4FA6h */
    SP = 0x4FA6;
L4ECB:
    /* 4ECB  sti */
    ;
L4ECC:
    /* 4ECC  call    _seg003_5AB7 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5AB7), 0x4ECF)) != 0) return c;
L4ECF:
    /* 4ECF  cli */
    ;
L4ED0:
    /* 4ED0  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L4ED5:
    /* 4ED5  mov     ss,dx */
    SET_SS(DX);
L4ED7:
    /* 4ED7  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L4EDC:
    /* 4EDC  mov     sp,dx */
    SP = DX;
L4EDE:
    /* 4EDE  sti */
    ;
L4EDF:
    /* 4EDF  pop     es */
    SET_ES(pop16());
L4EE0:
    /* 4EE0  pop     ds */
    SET_DS(pop16());
L4EE1:
    /* 4EE1  pop     si */
    SI = pop16();
L4EE2:
    /* 4EE2  pop     di */
    DI = pop16();
L4EE3:
    /* 4EE3  pop     bp */
    BP = pop16();
L4EE4:
    /* 4EE4  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_4EE5  (+4EE5)
       Load a palette (VIDMODE's _32F4): SI = the palette's offset in seg048, AX and BX the other
       two arguments (passed through; _32F4 does not read them). */
L4EE5: /* _seg003_4EE5 */
    /* 4EE5  push    bp */
    push16(BP);
L4EE6:
    /* 4EE6  mov     bp,sp */
    BP = SP;
L4EE8:
    /* 4EE8  push    di */
    push16(DI);
L4EE9:
    /* 4EE9  push    si */
    push16(SI);
L4EEA:
    /* 4EEA  push    ds */
    push16(asm_ds);
L4EEB:
    /* 4EEB  push    es */
    push16(asm_es);
L4EEC:
    /* 4EEC  mov     dx,ss */
    DX = asm_ss;
L4EEE:
    /* 4EEE  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L4EF3:
    /* 4EF3  mov     dx,sp */
    DX = SP;
L4EF5:
    /* 4EF5  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L4EFA:
    /* 4EFA  mov     si,word ptr [bp+6] */
    SI = rw(pSS, BP + 0x6);
L4EFD:
    /* 4EFD  mov     ax,word ptr [bp+0Ah] */
    AX = rw(pSS, BP + 0xA);
L4F00:
    /* 4F00  mov     bx,word ptr [bp+0Ch] */
    BX = rw(pSS, BP + 0xC);
L4F03:
    /* 4F03  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4F06:
    /* 4F06  mov     ds,dx */
    SET_DS(DX);
L4F08:
    /* 4F08  mov     es,dx */
    SET_ES(DX);
L4F0A:
    /* 4F0A  cli */
    ;
L4F0B:
    /* 4F0B  mov     ss,dx */
    SET_SS(DX);
L4F0D:
    /* 4F0D  mov     sp,4FA6h */
    SP = 0x4FA6;
L4F10:
    /* 4F10  sti */
    ;
L4F11:
    /* 4F11  call    _seg003_5A8D */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5A8D), 0x4F14)) != 0) return c;
L4F14:
    /* 4F14  cli */
    ;
L4F15:
    /* 4F15  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L4F1A:
    /* 4F1A  mov     ss,dx */
    SET_SS(DX);
L4F1C:
    /* 4F1C  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L4F21:
    /* 4F21  mov     sp,dx */
    SP = DX;
L4F23:
    /* 4F23  sti */
    ;
L4F24:
    /* 4F24  pop     es */
    SET_ES(pop16());
L4F25:
    /* 4F25  pop     ds */
    SET_DS(pop16());
L4F26:
    /* 4F26  pop     si */
    SI = pop16();
L4F27:
    /* 4F27  pop     di */
    DI = pop16();
L4F28:
    /* 4F28  pop     bp */
    BP = pop16();
L4F29:
    /* 4F29  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* MaybeApplyPalette_seg003_4F2A  (+4F2A) */
L4F2A: /* _grPageFlip */
    /* 4F2A  push    bp */
    push16(BP);
L4F2B:
    /* 4F2B  mov     bp,sp */
    BP = SP;
L4F2D:
    /* 4F2D  push    di */
    push16(DI);
L4F2E:
    /* 4F2E  push    si */
    push16(SI);
L4F2F:
    /* 4F2F  push    ds */
    push16(asm_ds);
L4F30:
    /* 4F30  push    es */
    push16(asm_es);
L4F31:
    /* 4F31  mov     dx,ss */
    DX = asm_ss;
L4F33:
    /* 4F33  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L4F38:
    /* 4F38  mov     dx,sp */
    DX = SP;
L4F3A:
    /* 4F3A  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L4F3F:
    /* 4F3F  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4F42:
    /* 4F42  mov     ds,dx */
    SET_DS(DX);
L4F44:
    /* 4F44  mov     es,dx */
    SET_ES(DX);
L4F46:
    /* 4F46  cli */
    ;
L4F47:
    /* 4F47  mov     ss,dx */
    SET_SS(DX);
L4F49:
    /* 4F49  mov     sp,4FA6h */
    SP = 0x4FA6;
L4F4C:
    /* 4F4C  sti */
    ;
L4F4D:
    /* 4F4D  call    _seg003_5A90 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5A90), 0x4F50)) != 0) return c;
L4F50:
    /* 4F50  cli */
    ;
L4F51:
    /* 4F51  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L4F56:
    /* 4F56  mov     ss,dx */
    SET_SS(DX);
L4F58:
    /* 4F58  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L4F5D:
    /* 4F5D  mov     sp,dx */
    SP = DX;
L4F5F:
    /* 4F5F  sti */
    ;
L4F60:
    /* 4F60  pop     es */
    SET_ES(pop16());
L4F61:
    /* 4F61  pop     ds */
    SET_DS(pop16());
L4F62:
    /* 4F62  pop     si */
    SI = pop16();
L4F63:
    /* 4F63  pop     di */
    DI = pop16();
L4F64:
    /* 4F64  pop     bp */
    BP = pop16();
L4F65:
    /* 4F65  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* GraphicsCall_seg003_4F66  (+4F66) */
L4F66: /* _grSoftPageFlip */
    /* 4F66  push    bp */
    push16(BP);
L4F67:
    /* 4F67  mov     bp,sp */
    BP = SP;
L4F69:
    /* 4F69  push    di */
    push16(DI);
L4F6A:
    /* 4F6A  push    si */
    push16(SI);
L4F6B:
    /* 4F6B  push    ds */
    push16(asm_ds);
L4F6C:
    /* 4F6C  push    es */
    push16(asm_es);
L4F6D:
    /* 4F6D  mov     dx,ss */
    DX = asm_ss;
L4F6F:
    /* 4F6F  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L4F74:
    /* 4F74  mov     dx,sp */
    DX = SP;
L4F76:
    /* 4F76  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L4F7B:
    /* 4F7B  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4F7E:
    /* 4F7E  mov     ds,dx */
    SET_DS(DX);
L4F80:
    /* 4F80  mov     es,dx */
    SET_ES(DX);
L4F82:
    /* 4F82  cli */
    ;
L4F83:
    /* 4F83  mov     ss,dx */
    SET_SS(DX);
L4F85:
    /* 4F85  mov     sp,4FA6h */
    SP = 0x4FA6;
L4F88:
    /* 4F88  sti */
    ;
L4F89:
    /* 4F89  call    _seg003_5A93 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5A93), 0x4F8C)) != 0) return c;
L4F8C:
    /* 4F8C  cli */
    ;
L4F8D:
    /* 4F8D  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L4F92:
    /* 4F92  mov     ss,dx */
    SET_SS(DX);
L4F94:
    /* 4F94  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L4F99:
    /* 4F99  mov     sp,dx */
    SP = DX;
L4F9B:
    /* 4F9B  sti */
    ;
L4F9C:
    /* 4F9C  pop     es */
    SET_ES(pop16());
L4F9D:
    /* 4F9D  pop     ds */
    SET_DS(pop16());
L4F9E:
    /* 4F9E  pop     si */
    SI = pop16();
L4F9F:
    /* 4F9F  pop     di */
    DI = pop16();
L4FA0:
    /* 4FA0  pop     bp */
    BP = pop16();
L4FA1:
    /* 4FA1  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_4FA2  (+4FA2)
       Show the drawing page (VIDMODE's _333F), without switching pages. */
L4FA2: /* _seg003_4FA2 */
    /* 4FA2  push    bp */
    push16(BP);
L4FA3:
    /* 4FA3  mov     bp,sp */
    BP = SP;
L4FA5:
    /* 4FA5  push    di */
    push16(DI);
L4FA6:
    /* 4FA6  push    si */
    push16(SI);
L4FA7:
    /* 4FA7  push    ds */
    push16(asm_ds);
L4FA8:
    /* 4FA8  push    es */
    push16(asm_es);
L4FA9:
    /* 4FA9  mov     dx,ss */
    DX = asm_ss;
L4FAB:
    /* 4FAB  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L4FB0:
    /* 4FB0  mov     dx,sp */
    DX = SP;
L4FB2:
    /* 4FB2  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L4FB7:
    /* 4FB7  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4FBA:
    /* 4FBA  mov     ds,dx */
    SET_DS(DX);
L4FBC:
    /* 4FBC  mov     es,dx */
    SET_ES(DX);
L4FBE:
    /* 4FBE  cli */
    ;
L4FBF:
    /* 4FBF  mov     ss,dx */
    SET_SS(DX);
L4FC1:
    /* 4FC1  mov     sp,4FA6h */
    SP = 0x4FA6;
L4FC4:
    /* 4FC4  sti */
    ;
L4FC5:
    /* 4FC5  call    _seg003_5A96 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5A96), 0x4FC8)) != 0) return c;
L4FC8:
    /* 4FC8  cli */
    ;
L4FC9:
    /* 4FC9  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L4FCE:
    /* 4FCE  mov     ss,dx */
    SET_SS(DX);
L4FD0:
    /* 4FD0  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L4FD5:
    /* 4FD5  mov     sp,dx */
    SP = DX;
L4FD7:
    /* 4FD7  sti */
    ;
L4FD8:
    /* 4FD8  pop     es */
    SET_ES(pop16());
L4FD9:
    /* 4FD9  pop     ds */
    SET_DS(pop16());
L4FDA:
    /* 4FDA  pop     si */
    SI = pop16();
L4FDB:
    /* 4FDB  pop     di */
    DI = pop16();
L4FDC:
    /* 4FDC  pop     bp */
    BP = pop16();
L4FDD:
    /* 4FDD  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_4FDE  (+4FDE) */
L4FDE: /* _gr_read_pixel */
    /* 4FDE  push    bp */
    push16(BP);
L4FDF:
    /* 4FDF  mov     bp,sp */
    BP = SP;
L4FE1:
    /* 4FE1  push    di */
    push16(DI);
L4FE2:
    /* 4FE2  push    si */
    push16(SI);
L4FE3:
    /* 4FE3  push    ds */
    push16(asm_ds);
L4FE4:
    /* 4FE4  push    es */
    push16(asm_es);
L4FE5:
    /* 4FE5  mov     dx,ss */
    DX = asm_ss;
L4FE7:
    /* 4FE7  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L4FEC:
    /* 4FEC  mov     dx,sp */
    DX = SP;
L4FEE:
    /* 4FEE  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L4FF3:
    /* 4FF3  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L4FF6:
    /* 4FF6  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L4FF9:
    /* 4FF9  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4FFC:
    /* 4FFC  mov     ds,dx */
    SET_DS(DX);
L4FFE:
    /* 4FFE  mov     es,dx */
    SET_ES(DX);
L5000:
    /* 5000  cli */
    ;
L5001:
    /* 5001  mov     ss,dx */
    SET_SS(DX);
L5003:
    /* 5003  mov     sp,4FA6h */
    SP = 0x4FA6;
L5006:
    /* 5006  sti */
    ;
L5007:
    /* 5007  call    _seg003_5A99 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5A99), 0x500A)) != 0) return c;
L500A:
    /* 500A  cli */
    ;
L500B:
    /* 500B  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L5010:
    /* 5010  mov     ss,dx */
    SET_SS(DX);
L5012:
    /* 5012  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L5017:
    /* 5017  mov     sp,dx */
    SP = DX;
L5019:
    /* 5019  sti */
    ;
L501A:
    /* 501A  pop     es */
    SET_ES(pop16());
L501B:
    /* 501B  pop     ds */
    SET_DS(pop16());
L501C:
    /* 501C  pop     si */
    SI = pop16();
L501D:
    /* 501D  pop     di */
    DI = pop16();
L501E:
    /* 501E  pop     bp */
    BP = pop16();
L501F:
    /* 501F  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_5020  (+5020)
       Read the pixel at (x, y), or -1 outside the window (VIDMODE's _3964). */
L5020: /* _seg003_5020 */
    /* 5020  push    bp */
    push16(BP);
L5021:
    /* 5021  mov     bp,sp */
    BP = SP;
L5023:
    /* 5023  push    di */
    push16(DI);
L5024:
    /* 5024  push    si */
    push16(SI);
L5025:
    /* 5025  push    ds */
    push16(asm_ds);
L5026:
    /* 5026  push    es */
    push16(asm_es);
L5027:
    /* 5027  mov     dx,ss */
    DX = asm_ss;
L5029:
    /* 5029  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L502E:
    /* 502E  mov     dx,sp */
    DX = SP;
L5030:
    /* 5030  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5035:
    /* 5035  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L5038:
    /* 5038  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L503B:
    /* 503B  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L503E:
    /* 503E  mov     ds,dx */
    SET_DS(DX);
L5040:
    /* 5040  mov     es,dx */
    SET_ES(DX);
L5042:
    /* 5042  cli */
    ;
L5043:
    /* 5043  mov     ss,dx */
    SET_SS(DX);
L5045:
    /* 5045  mov     sp,4FA6h */
    SP = 0x4FA6;
L5048:
    /* 5048  sti */
    ;
L5049:
    /* 5049  call    _seg003_5A9C */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5A9C), 0x504C)) != 0) return c;
L504C:
    /* 504C  cli */
    ;
L504D:
    /* 504D  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L5052:
    /* 5052  mov     ss,dx */
    SET_SS(DX);
L5054:
    /* 5054  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L5059:
    /* 5059  mov     sp,dx */
    SP = DX;
L505B:
    /* 505B  sti */
    ;
L505C:
    /* 505C  pop     es */
    SET_ES(pop16());
L505D:
    /* 505D  pop     ds */
    SET_DS(pop16());
L505E:
    /* 505E  pop     si */
    SI = pop16();
L505F:
    /* 505F  pop     di */
    DI = pop16();
L5060:
    /* 5060  pop     bp */
    BP = pop16();
L5061:
    /* 5061  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_5062  (+5062) */
L5062: /* _plot_pixel */
    /* 5062  push    bp */
    push16(BP);
L5063:
    /* 5063  mov     bp,sp */
    BP = SP;
L5065:
    /* 5065  push    di */
    push16(DI);
L5066:
    /* 5066  push    si */
    push16(SI);
L5067:
    /* 5067  push    ds */
    push16(asm_ds);
L5068:
    /* 5068  push    es */
    push16(asm_es);
L5069:
    /* 5069  mov     dx,ss */
    DX = asm_ss;
L506B:
    /* 506B  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L5070:
    /* 5070  mov     dx,sp */
    DX = SP;
L5072:
    /* 5072  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5077:
    /* 5077  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L507A:
    /* 507A  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L507D:
    /* 507D  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L5080:
    /* 5080  mov     ds,dx */
    SET_DS(DX);
L5082:
    /* 5082  mov     es,dx */
    SET_ES(DX);
L5084:
    /* 5084  cli */
    ;
L5085:
    /* 5085  mov     ss,dx */
    SET_SS(DX);
L5087:
    /* 5087  mov     sp,4FA6h */
    SP = 0x4FA6;
L508A:
    /* 508A  sti */
    ;
L508B:
    /* 508B  call    _seg003_5A9F */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5A9F), 0x508E)) != 0) return c;
L508E:
    /* 508E  cli */
    ;
L508F:
    /* 508F  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L5094:
    /* 5094  mov     ss,dx */
    SET_SS(DX);
L5096:
    /* 5096  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L509B:
    /* 509B  mov     sp,dx */
    SP = DX;
L509D:
    /* 509D  sti */
    ;
L509E:
    /* 509E  pop     es */
    SET_ES(pop16());
L509F:
    /* 509F  pop     ds */
    SET_DS(pop16());
L50A0:
    /* 50A0  pop     si */
    SI = pop16();
L50A1:
    /* 50A1  pop     di */
    DI = pop16();
L50A2:
    /* 50A2  pop     bp */
    BP = pop16();
L50A3:
    /* 50A3  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_50A4  (+50A4)
       Plot (x, y) without clipping (VIDMODE's _392D through the jump table). */
L50A4: /* _seg003_50A4 */
    /* 50A4  push    bp */
    push16(BP);
L50A5:
    /* 50A5  mov     bp,sp */
    BP = SP;
L50A7:
    /* 50A7  push    di */
    push16(DI);
L50A8:
    /* 50A8  push    si */
    push16(SI);
L50A9:
    /* 50A9  push    ds */
    push16(asm_ds);
L50AA:
    /* 50AA  push    es */
    push16(asm_es);
L50AB:
    /* 50AB  mov     dx,ss */
    DX = asm_ss;
L50AD:
    /* 50AD  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L50B2:
    /* 50B2  mov     dx,sp */
    DX = SP;
L50B4:
    /* 50B4  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L50B9:
    /* 50B9  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L50BC:
    /* 50BC  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L50BF:
    /* 50BF  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L50C2:
    /* 50C2  mov     ds,dx */
    SET_DS(DX);
L50C4:
    /* 50C4  mov     es,dx */
    SET_ES(DX);
L50C6:
    /* 50C6  cli */
    ;
L50C7:
    /* 50C7  mov     ss,dx */
    SET_SS(DX);
L50C9:
    /* 50C9  mov     sp,4FA6h */
    SP = 0x4FA6;
L50CC:
    /* 50CC  sti */
    ;
L50CD:
    /* 50CD  call    _seg003_5AA2 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5AA2), 0x50D0)) != 0) return c;
L50D0:
    /* 50D0  cli */
    ;
L50D1:
    /* 50D1  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L50D6:
    /* 50D6  mov     ss,dx */
    SET_SS(DX);
L50D8:
    /* 50D8  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L50DD:
    /* 50DD  mov     sp,dx */
    SP = DX;
L50DF:
    /* 50DF  sti */
    ;
L50E0:
    /* 50E0  pop     es */
    SET_ES(pop16());
L50E1:
    /* 50E1  pop     ds */
    SET_DS(pop16());
L50E2:
    /* 50E2  pop     si */
    SI = pop16();
L50E3:
    /* 50E3  pop     di */
    DI = pop16();
L50E4:
    /* 50E4  pop     bp */
    BP = pop16();
L50E5:
    /* 50E5  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_50E6  (+50E6)
       A solid vertical line (x, y1, y2) without clipping (VIDMODE's _39E5). */
L50E6: /* _seg003_50E6 */
    /* 50E6  push    bp */
    push16(BP);
L50E7:
    /* 50E7  mov     bp,sp */
    BP = SP;
L50E9:
    /* 50E9  push    di */
    push16(DI);
L50EA:
    /* 50EA  push    si */
    push16(SI);
L50EB:
    /* 50EB  push    ds */
    push16(asm_ds);
L50EC:
    /* 50EC  push    es */
    push16(asm_es);
L50ED:
    /* 50ED  mov     dx,ss */
    DX = asm_ss;
L50EF:
    /* 50EF  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L50F4:
    /* 50F4  mov     dx,sp */
    DX = SP;
L50F6:
    /* 50F6  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L50FB:
    /* 50FB  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L50FE:
    /* 50FE  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L5101:
    /* 5101  mov     cx,word ptr [bp+0Ah] */
    CX = rw(pSS, BP + 0xA);
L5104:
    /* 5104  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L5107:
    /* 5107  mov     ds,dx */
    SET_DS(DX);
L5109:
    /* 5109  mov     es,dx */
    SET_ES(DX);
L510B:
    /* 510B  cli */
    ;
L510C:
    /* 510C  mov     ss,dx */
    SET_SS(DX);
L510E:
    /* 510E  mov     sp,4FA6h */
    SP = 0x4FA6;
L5111:
    /* 5111  sti */
    ;
L5112:
    /* 5112  xchg    cx,dx */
    { uint16_t t_ = DX;
    DX = CX;
    CX = t_; }
L5114:
    /* 5114  call    _seg003_5AAB */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5AAB), 0x5117)) != 0) return c;
L5117:
    /* 5117  cli */
    ;
L5118:
    /* 5118  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L511D:
    /* 511D  mov     ss,dx */
    SET_SS(DX);
L511F:
    /* 511F  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L5124:
    /* 5124  mov     sp,dx */
    SP = DX;
L5126:
    /* 5126  sti */
    ;
L5127:
    /* 5127  pop     es */
    SET_ES(pop16());
L5128:
    /* 5128  pop     ds */
    SET_DS(pop16());
L5129:
    /* 5129  pop     si */
    SI = pop16();
L512A:
    /* 512A  pop     di */
    DI = pop16();
L512B:
    /* 512B  pop     bp */
    BP = pop16();
L512C:
    /* 512C  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_512D  (+512D)
       A vertical line (x, y1, y2) copied from the other page (VIDMODE's _39A2). */
L512D: /* _seg003_512D */
    /* 512D  push    bp */
    push16(BP);
L512E:
    /* 512E  mov     bp,sp */
    BP = SP;
L5130:
    /* 5130  push    di */
    push16(DI);
L5131:
    /* 5131  push    si */
    push16(SI);
L5132:
    /* 5132  push    ds */
    push16(asm_ds);
L5133:
    /* 5133  push    es */
    push16(asm_es);
L5134:
    /* 5134  mov     dx,ss */
    DX = asm_ss;
L5136:
    /* 5136  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L513B:
    /* 513B  mov     dx,sp */
    DX = SP;
L513D:
    /* 513D  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5142:
    /* 5142  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L5145:
    /* 5145  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L5148:
    /* 5148  mov     cx,word ptr [bp+0Ah] */
    CX = rw(pSS, BP + 0xA);
L514B:
    /* 514B  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L514E:
    /* 514E  mov     ds,dx */
    SET_DS(DX);
L5150:
    /* 5150  mov     es,dx */
    SET_ES(DX);
L5152:
    /* 5152  cli */
    ;
L5153:
    /* 5153  mov     ss,dx */
    SET_SS(DX);
L5155:
    /* 5155  mov     sp,4FA6h */
    SP = 0x4FA6;
L5158:
    /* 5158  sti */
    ;
L5159:
    /* 5159  xchg    cx,dx */
    { uint16_t t_ = DX;
    DX = CX;
    CX = t_; }
L515B:
    /* 515B  call    _seg003_5AAE */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5AAE), 0x515E)) != 0) return c;
L515E:
    /* 515E  cli */
    ;
L515F:
    /* 515F  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L5164:
    /* 5164  mov     ss,dx */
    SET_SS(DX);
L5166:
    /* 5166  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L516B:
    /* 516B  mov     sp,dx */
    SP = DX;
L516D:
    /* 516D  sti */
    ;
L516E:
    /* 516E  pop     es */
    SET_ES(pop16());
L516F:
    /* 516F  pop     ds */
    SET_DS(pop16());
L5170:
    /* 5170  pop     si */
    SI = pop16();
L5171:
    /* 5171  pop     di */
    DI = pop16();
L5172:
    /* 5172  pop     bp */
    BP = pop16();
L5173:
    /* 5173  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_5174  (+5174) */
L5174: /* _set_the_color */
    /* 5174  push    bp */
    push16(BP);
L5175:
    /* 5175  mov     bp,sp */
    BP = SP;
L5177:
    /* 5177  push    di */
    push16(DI);
L5178:
    /* 5178  push    si */
    push16(SI);
L5179:
    /* 5179  push    ds */
    push16(asm_ds);
L517A:
    /* 517A  push    es */
    push16(asm_es);
L517B:
    /* 517B  mov     dx,ss */
    DX = asm_ss;
L517D:
    /* 517D  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L5182:
    /* 5182  mov     dx,sp */
    DX = SP;
L5184:
    /* 5184  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5189:
    /* 5189  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L518C:
    /* 518C  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L518F:
    /* 518F  mov     ds,dx */
    SET_DS(DX);
L5191:
    /* 5191  mov     es,dx */
    SET_ES(DX);
L5193:
    /* 5193  cli */
    ;
L5194:
    /* 5194  mov     ss,dx */
    SET_SS(DX);
L5196:
    /* 5196  mov     sp,4FA6h */
    SP = 0x4FA6;
L5199:
    /* 5199  sti */
    ;
L519A:
    /* 519A  call    _seg003_5AB1 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5AB1), 0x519D)) != 0) return c;
L519D:
    /* 519D  cli */
    ;
L519E:
    /* 519E  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L51A3:
    /* 51A3  mov     ss,dx */
    SET_SS(DX);
L51A5:
    /* 51A5  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L51AA:
    /* 51AA  mov     sp,dx */
    SP = DX;
L51AC:
    /* 51AC  sti */
    ;
L51AD:
    /* 51AD  pop     es */
    SET_ES(pop16());
L51AE:
    /* 51AE  pop     ds */
    SET_DS(pop16());
L51AF:
    /* 51AF  pop     si */
    SI = pop16();
L51B0:
    /* 51B0  pop     di */
    DI = pop16();
L51B1:
    /* 51B1  pop     bp */
    BP = pop16();
L51B2:
    /* 51B2  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* init_colors  (+51B3) */
L51B3: /* _init_colors */
    /* 51B3  push    bp */
    push16(BP);
L51B4:
    /* 51B4  mov     bp,sp */
    BP = SP;
L51B6:
    /* 51B6  push    di */
    push16(DI);
L51B7:
    /* 51B7  push    si */
    push16(SI);
L51B8:
    /* 51B8  push    ds */
    push16(asm_ds);
L51B9:
    /* 51B9  push    es */
    push16(asm_es);
L51BA:
    /* 51BA  mov     dx,ss */
    DX = asm_ss;
L51BC:
    /* 51BC  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L51C1:
    /* 51C1  mov     dx,sp */
    DX = SP;
L51C3:
    /* 51C3  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L51C8:
    /* 51C8  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L51CB:
    /* 51CB  mov     ds,dx */
    SET_DS(DX);
L51CD:
    /* 51CD  mov     es,dx */
    SET_ES(DX);
L51CF:
    /* 51CF  cli */
    ;
L51D0:
    /* 51D0  mov     ss,dx */
    SET_SS(DX);
L51D2:
    /* 51D2  mov     sp,4FA6h */
    SP = 0x4FA6;
L51D5:
    /* 51D5  sti */
    ;
L51D6:
    /* 51D6  call    _seg003_5AB4 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5AB4), 0x51D9)) != 0) return c;
L51D9:
    /* 51D9  cli */
    ;
L51DA:
    /* 51DA  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L51DF:
    /* 51DF  mov     ss,dx */
    SET_SS(DX);
L51E1:
    /* 51E1  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L51E6:
    /* 51E6  mov     sp,dx */
    SP = DX;
L51E8:
    /* 51E8  sti */
    ;
L51E9:
    /* 51E9  pop     es */
    SET_ES(pop16());
L51EA:
    /* 51EA  pop     ds */
    SET_DS(pop16());
L51EB:
    /* 51EB  pop     si */
    SI = pop16();
L51EC:
    /* 51EC  pop     di */
    DI = pop16();
L51ED:
    /* 51ED  pop     bp */
    BP = pop16();
L51EE:
    /* 51EE  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_51EF  (+51EF)
       Set the screen mode (AX = the mode number, VIDMODE's _324C). */
L51EF: /* _seg003_51EF */
    /* 51EF  push    bp */
    push16(BP);
L51F0:
    /* 51F0  mov     bp,sp */
    BP = SP;
L51F2:
    /* 51F2  push    di */
    push16(DI);
L51F3:
    /* 51F3  push    si */
    push16(SI);
L51F4:
    /* 51F4  push    ds */
    push16(asm_ds);
L51F5:
    /* 51F5  push    es */
    push16(asm_es);
L51F6:
    /* 51F6  mov     dx,ss */
    DX = asm_ss;
L51F8:
    /* 51F8  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L51FD:
    /* 51FD  mov     dx,sp */
    DX = SP;
L51FF:
    /* 51FF  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5204:
    /* 5204  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L5207:
    /* 5207  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L520A:
    /* 520A  mov     ds,dx */
    SET_DS(DX);
L520C:
    /* 520C  mov     es,dx */
    SET_ES(DX);
L520E:
    /* 520E  cli */
    ;
L520F:
    /* 520F  mov     ss,dx */
    SET_SS(DX);
L5211:
    /* 5211  mov     sp,4FA6h */
    SP = 0x4FA6;
L5214:
    /* 5214  sti */
    ;
L5215:
    /* 5215  call    _seg003_5ABA */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5ABA), 0x5218)) != 0) return c;
L5218:
    /* 5218  cli */
    ;
L5219:
    /* 5219  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L521E:
    /* 521E  mov     ss,dx */
    SET_SS(DX);
L5220:
    /* 5220  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L5225:
    /* 5225  mov     sp,dx */
    SP = DX;
L5227:
    /* 5227  sti */
    ;
L5228:
    /* 5228  pop     es */
    SET_ES(pop16());
L5229:
    /* 5229  pop     ds */
    SET_DS(pop16());
L522A:
    /* 522A  pop     si */
    SI = pop16();
L522B:
    /* 522B  pop     di */
    DI = pop16();
L522C:
    /* 522C  pop     bp */
    BP = pop16();
L522D:
    /* 522D  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_522E  (+522E)
       Allocate n bytes of video memory (GRLIBF's _3A86). Returns the offset, or 0 when there is no
       room. */
L522E: /* _seg003_522E */
    /* 522E  push    bp */
    push16(BP);
L522F:
    /* 522F  mov     bp,sp */
    BP = SP;
L5231:
    /* 5231  push    di */
    push16(DI);
L5232:
    /* 5232  push    si */
    push16(SI);
L5233:
    /* 5233  push    ds */
    push16(asm_ds);
L5234:
    /* 5234  push    es */
    push16(asm_es);
L5235:
    /* 5235  mov     dx,ss */
    DX = asm_ss;
L5237:
    /* 5237  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L523C:
    /* 523C  mov     dx,sp */
    DX = SP;
L523E:
    /* 523E  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5243:
    /* 5243  mov     bx,word ptr [bp+6] */
    BX = rw(pSS, BP + 0x6);
L5246:
    /* 5246  mov     ax,bx */
    AX = BX;
L5248:
    /* 5248  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L524B:
    /* 524B  mov     ds,dx */
    SET_DS(DX);
L524D:
    /* 524D  mov     es,dx */
    SET_ES(DX);
L524F:
    /* 524F  cli */
    ;
L5250:
    /* 5250  mov     ss,dx */
    SET_SS(DX);
L5252:
    /* 5252  mov     sp,4FA6h */
    SP = 0x4FA6;
L5255:
    /* 5255  sti */
    ;
L5256:
    /* 5256  call    _seg003_3A86 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3A86), 0x5259)) != 0) return c;
L5259:
    /* 5259  jae     L525E */
    if (!CF) goto L525E;
L525B:
    /* 525B  mov     ax,0 */
    AX = 0x0;
L525E: /* L525E */
    /* 525E  cli */
    ;
L525F:
    /* 525F  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L5264:
    /* 5264  mov     ss,dx */
    SET_SS(DX);
L5266:
    /* 5266  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L526B:
    /* 526B  mov     sp,dx */
    SP = DX;
L526D:
    /* 526D  sti */
    ;
L526E:
    /* 526E  pop     es */
    SET_ES(pop16());
L526F:
    /* 526F  pop     ds */
    SET_DS(pop16());
L5270:
    /* 5270  pop     si */
    SI = pop16();
L5271:
    /* 5271  pop     di */
    DI = pop16();
L5272:
    /* 5272  pop     bp */
    BP = pop16();
L5273:
    /* 5273  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_5274  (+5274)
       Give video memory back down to an offset (GRLIBF's _3A9B). Returns the offset, or 0 if it was
       below the floor. */
L5274: /* _seg003_5274 */
    /* 5274  push    bp */
    push16(BP);
L5275:
    /* 5275  mov     bp,sp */
    BP = SP;
L5277:
    /* 5277  push    di */
    push16(DI);
L5278:
    /* 5278  push    si */
    push16(SI);
L5279:
    /* 5279  push    ds */
    push16(asm_ds);
L527A:
    /* 527A  push    es */
    push16(asm_es);
L527B:
    /* 527B  mov     dx,ss */
    DX = asm_ss;
L527D:
    /* 527D  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L5282:
    /* 5282  mov     dx,sp */
    DX = SP;
L5284:
    /* 5284  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5289:
    /* 5289  mov     bx,word ptr [bp+6] */
    BX = rw(pSS, BP + 0x6);
L528C:
    /* 528C  mov     ax,bx */
    AX = BX;
L528E:
    /* 528E  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L5291:
    /* 5291  mov     ds,dx */
    SET_DS(DX);
L5293:
    /* 5293  mov     es,dx */
    SET_ES(DX);
L5295:
    /* 5295  cli */
    ;
L5296:
    /* 5296  mov     ss,dx */
    SET_SS(DX);
L5298:
    /* 5298  mov     sp,4FA6h */
    SP = 0x4FA6;
L529B:
    /* 529B  sti */
    ;
L529C:
    /* 529C  call    _seg003_3A9B */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3A9B), 0x529F)) != 0) return c;
L529F:
    /* 529F  jae     L52A4 */
    if (!CF) goto L52A4;
L52A1:
    /* 52A1  mov     ax,0 */
    AX = 0x0;
L52A4: /* L52A4 */
    /* 52A4  cli */
    ;
L52A5:
    /* 52A5  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L52AA:
    /* 52AA  mov     ss,dx */
    SET_SS(DX);
L52AC:
    /* 52AC  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L52B1:
    /* 52B1  mov     sp,dx */
    SP = DX;
L52B3:
    /* 52B3  sti */
    ;
L52B4:
    /* 52B4  pop     es */
    SET_ES(pop16());
L52B5:
    /* 52B5  pop     ds */
    SET_DS(pop16());
L52B6:
    /* 52B6  pop     si */
    SI = pop16();
L52B7:
    /* 52B7  pop     di */
    DI = pop16();
L52B8:
    /* 52B8  pop     bp */
    BP = pop16();
L52B9:
    /* 52B9  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* DrawBitmapScreen_seg003_52BA  (+52BA) */
L52BA: /* _set_the_window */
    /* 52BA  push    bp */
    push16(BP);
L52BB:
    /* 52BB  mov     bp,sp */
    BP = SP;
L52BD:
    /* 52BD  push    di */
    push16(DI);
L52BE:
    /* 52BE  push    si */
    push16(SI);
L52BF:
    /* 52BF  push    ds */
    push16(asm_ds);
L52C0:
    /* 52C0  push    es */
    push16(asm_es);
L52C1:
    /* 52C1  mov     dx,ss */
    DX = asm_ss;
L52C3:
    /* 52C3  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L52C8:
    /* 52C8  mov     dx,sp */
    DX = SP;
L52CA:
    /* 52CA  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L52CF:
    /* 52CF  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L52D2:
    /* 52D2  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L52D5:
    /* 52D5  mov     cx,word ptr [bp+0Ah] */
    CX = rw(pSS, BP + 0xA);
L52D8:
    /* 52D8  mov     di,word ptr [bp+0Ch] */
    DI = rw(pSS, BP + 0xC);
L52DB:
    /* 52DB  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L52DE:
    /* 52DE  mov     ds,dx */
    SET_DS(DX);
L52E0:
    /* 52E0  mov     es,dx */
    SET_ES(DX);
L52E2:
    /* 52E2  cli */
    ;
L52E3:
    /* 52E3  mov     ss,dx */
    SET_SS(DX);
L52E5:
    /* 52E5  mov     sp,4FA6h */
    SP = 0x4FA6;
L52E8:
    /* 52E8  sti */
    ;
L52E9:
    /* 52E9  mov     word ptr ds:[503Eh],bx */
    ww(pDS, 0x503E, BX);
L52ED:
    /* 52ED  mov     word ptr ds:[503Ch],ax */
    ww(pDS, 0x503C, AX);
L52F0:
    /* 52F0  mov     word ptr ds:[5042h],di */
    ww(pDS, 0x5042, DI);
L52F4:
    /* 52F4  mov     word ptr ds:[5040h],cx */
    ww(pDS, 0x5040, CX);
L52F8:
    /* 52F8  mov     si,503Ch */
    SI = 0x503C;
L52FB:
    /* 52FB  call    near ptr _seg003_5ADE */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5ADE), 0x52FE)) != 0) return c;
L52FE:
    /* 52FE  cli */
    ;
L52FF:
    /* 52FF  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L5304:
    /* 5304  mov     ss,dx */
    SET_SS(DX);
L5306:
    /* 5306  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L530B:
    /* 530B  mov     sp,dx */
    SP = DX;
L530D:
    /* 530D  sti */
    ;
L530E:
    /* 530E  pop     es */
    SET_ES(pop16());
L530F:
    /* 530F  pop     ds */
    SET_DS(pop16());
L5310:
    /* 5310  pop     si */
    SI = pop16();
L5311:
    /* 5311  pop     di */
    DI = pop16();
L5312:
    /* 5312  pop     bp */
    BP = pop16();
L5313:
    /* 5313  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_5314  (+5314) */
L5314: /* _copy_hidden_to_visible */
    /* 5314  push    bp */
    push16(BP);
L5315:
    /* 5315  mov     bp,sp */
    BP = SP;
L5317:
    /* 5317  push    di */
    push16(DI);
L5318:
    /* 5318  push    si */
    push16(SI);
L5319:
    /* 5319  push    ds */
    push16(asm_ds);
L531A:
    /* 531A  push    es */
    push16(asm_es);
L531B:
    /* 531B  mov     dx,ss */
    DX = asm_ss;
L531D:
    /* 531D  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L5322:
    /* 5322  mov     dx,sp */
    DX = SP;
L5324:
    /* 5324  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5329:
    /* 5329  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L532C:
    /* 532C  mov     ds,dx */
    SET_DS(DX);
L532E:
    /* 532E  mov     es,dx */
    SET_ES(DX);
L5330:
    /* 5330  cli */
    ;
L5331:
    /* 5331  mov     ss,dx */
    SET_SS(DX);
L5333:
    /* 5333  mov     sp,4FA6h */
    SP = 0x4FA6;
L5336:
    /* 5336  sti */
    ;
L5337:
    /* 5337  call    _seg003_3B0F */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3B0F), 0x533A)) != 0) return c;
L533A:
    /* 533A  cli */
    ;
L533B:
    /* 533B  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L5340:
    /* 5340  mov     ss,dx */
    SET_SS(DX);
L5342:
    /* 5342  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L5347:
    /* 5347  mov     sp,dx */
    SP = DX;
L5349:
    /* 5349  sti */
    ;
L534A:
    /* 534A  pop     es */
    SET_ES(pop16());
L534B:
    /* 534B  pop     ds */
    SET_DS(pop16());
L534C:
    /* 534C  pop     si */
    SI = pop16();
L534D:
    /* 534D  pop     di */
    DI = pop16();
L534E:
    /* 534E  pop     bp */
    BP = pop16();
L534F:
    /* 534F  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* GraphicsCall_seg003_5350  (+5350) */
L5350: /* _copy_visible_to_hidden */
    /* 5350  push    bp */
    push16(BP);
L5351:
    /* 5351  mov     bp,sp */
    BP = SP;
L5353:
    /* 5353  push    di */
    push16(DI);
L5354:
    /* 5354  push    si */
    push16(SI);
L5355:
    /* 5355  push    ds */
    push16(asm_ds);
L5356:
    /* 5356  push    es */
    push16(asm_es);
L5357:
    /* 5357  mov     dx,ss */
    DX = asm_ss;
L5359:
    /* 5359  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L535E:
    /* 535E  mov     dx,sp */
    DX = SP;
L5360:
    /* 5360  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5365:
    /* 5365  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L5368:
    /* 5368  mov     ds,dx */
    SET_DS(DX);
L536A:
    /* 536A  mov     es,dx */
    SET_ES(DX);
L536C:
    /* 536C  cli */
    ;
L536D:
    /* 536D  mov     ss,dx */
    SET_SS(DX);
L536F:
    /* 536F  mov     sp,4FA6h */
    SP = 0x4FA6;
L5372:
    /* 5372  sti */
    ;
L5373:
    /* 5373  call    _seg003_3AFD */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3AFD), 0x5376)) != 0) return c;
L5376:
    /* 5376  cli */
    ;
L5377:
    /* 5377  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L537C:
    /* 537C  mov     ss,dx */
    SET_SS(DX);
L537E:
    /* 537E  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L5383:
    /* 5383  mov     sp,dx */
    SP = DX;
L5385:
    /* 5385  sti */
    ;
L5386:
    /* 5386  pop     es */
    SET_ES(pop16());
L5387:
    /* 5387  pop     ds */
    SET_DS(pop16());
L5388:
    /* 5388  pop     si */
    SI = pop16();
L5389:
    /* 5389  pop     di */
    DI = pop16();
L538A:
    /* 538A  pop     bp */
    BP = pop16();
L538B:
    /* 538B  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_538C  (+538C)
       A clipped vertical line through the vector at 5AAE (GRLIBF's _3B9A). */
L538C: /* _seg003_538C */
    /* 538C  push    bp */
    push16(BP);
L538D:
    /* 538D  mov     bp,sp */
    BP = SP;
L538F:
    /* 538F  push    di */
    push16(DI);
L5390:
    /* 5390  push    si */
    push16(SI);
L5391:
    /* 5391  push    ds */
    push16(asm_ds);
L5392:
    /* 5392  push    es */
    push16(asm_es);
L5393:
    /* 5393  mov     dx,ss */
    DX = asm_ss;
L5395:
    /* 5395  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L539A:
    /* 539A  mov     dx,sp */
    DX = SP;
L539C:
    /* 539C  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L53A1:
    /* 53A1  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L53A4:
    /* 53A4  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L53A7:
    /* 53A7  mov     cx,word ptr [bp+0Ah] */
    CX = rw(pSS, BP + 0xA);
L53AA:
    /* 53AA  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L53AD:
    /* 53AD  mov     ds,dx */
    SET_DS(DX);
L53AF:
    /* 53AF  mov     es,dx */
    SET_ES(DX);
L53B1:
    /* 53B1  cli */
    ;
L53B2:
    /* 53B2  mov     ss,dx */
    SET_SS(DX);
L53B4:
    /* 53B4  mov     sp,4FA6h */
    SP = 0x4FA6;
L53B7:
    /* 53B7  sti */
    ;
L53B8:
    /* 53B8  xchg    cx,dx */
    { uint16_t t_ = DX;
    DX = CX;
    CX = t_; }
L53BA:
    /* 53BA  call    _seg003_3B9A */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3B9A), 0x53BD)) != 0) return c;
L53BD:
    /* 53BD  cli */
    ;
L53BE:
    /* 53BE  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L53C3:
    /* 53C3  mov     ss,dx */
    SET_SS(DX);
L53C5:
    /* 53C5  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L53CA:
    /* 53CA  mov     sp,dx */
    SP = DX;
L53CC:
    /* 53CC  sti */
    ;
L53CD:
    /* 53CD  pop     es */
    SET_ES(pop16());
L53CE:
    /* 53CE  pop     ds */
    SET_DS(pop16());
L53CF:
    /* 53CF  pop     si */
    SI = pop16();
L53D0:
    /* 53D0  pop     di */
    DI = pop16();
L53D1:
    /* 53D1  pop     bp */
    BP = pop16();
L53D2:
    /* 53D2  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_53D3  (+53D3)
       hline: a clipped horizontal line (x1, y, x2; GRLIBF's _3CFF). */
L53D3: /* _seg003_53D3 */
    /* 53D3  push    bp */
    push16(BP);
L53D4:
    /* 53D4  mov     bp,sp */
    BP = SP;
L53D6:
    /* 53D6  push    di */
    push16(DI);
L53D7:
    /* 53D7  push    si */
    push16(SI);
L53D8:
    /* 53D8  push    ds */
    push16(asm_ds);
L53D9:
    /* 53D9  push    es */
    push16(asm_es);
L53DA:
    /* 53DA  mov     dx,ss */
    DX = asm_ss;
L53DC:
    /* 53DC  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L53E1:
    /* 53E1  mov     dx,sp */
    DX = SP;
L53E3:
    /* 53E3  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L53E8:
    /* 53E8  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L53EB:
    /* 53EB  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L53EE:
    /* 53EE  mov     cx,word ptr [bp+0Ah] */
    CX = rw(pSS, BP + 0xA);
L53F1:
    /* 53F1  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L53F4:
    /* 53F4  mov     ds,dx */
    SET_DS(DX);
L53F6:
    /* 53F6  mov     es,dx */
    SET_ES(DX);
L53F8:
    /* 53F8  cli */
    ;
L53F9:
    /* 53F9  mov     ss,dx */
    SET_SS(DX);
L53FB:
    /* 53FB  mov     sp,4FA6h */
    SP = 0x4FA6;
L53FE:
    /* 53FE  sti */
    ;
L53FF:
    /* 53FF  call    _seg003_3CFF */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3CFF), 0x5402)) != 0) return c;
L5402:
    /* 5402  cli */
    ;
L5403:
    /* 5403  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L5408:
    /* 5408  mov     ss,dx */
    SET_SS(DX);
L540A:
    /* 540A  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L540F:
    /* 540F  mov     sp,dx */
    SP = DX;
L5411:
    /* 5411  sti */
    ;
L5412:
    /* 5412  pop     es */
    SET_ES(pop16());
L5413:
    /* 5413  pop     ds */
    SET_DS(pop16());
L5414:
    /* 5414  pop     si */
    SI = pop16();
L5415:
    /* 5415  pop     di */
    DI = pop16();
L5416:
    /* 5416  pop     bp */
    BP = pop16();
L5417:
    /* 5417  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_5418  (+5418) */
L5418: /* _uhline */
    /* 5418  push    bp */
    push16(BP);
L5419:
    /* 5419  mov     bp,sp */
    BP = SP;
L541B:
    /* 541B  push    di */
    push16(DI);
L541C:
    /* 541C  push    si */
    push16(SI);
L541D:
    /* 541D  push    ds */
    push16(asm_ds);
L541E:
    /* 541E  push    es */
    push16(asm_es);
L541F:
    /* 541F  mov     dx,ss */
    DX = asm_ss;
L5421:
    /* 5421  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L5426:
    /* 5426  mov     dx,sp */
    DX = SP;
L5428:
    /* 5428  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L542D:
    /* 542D  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L5430:
    /* 5430  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L5433:
    /* 5433  mov     cx,word ptr [bp+0Ah] */
    CX = rw(pSS, BP + 0xA);
L5436:
    /* 5436  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L5439:
    /* 5439  mov     ds,dx */
    SET_DS(DX);
L543B:
    /* 543B  mov     es,dx */
    SET_ES(DX);
L543D:
    /* 543D  cli */
    ;
L543E:
    /* 543E  mov     ss,dx */
    SET_SS(DX);
L5440:
    /* 5440  mov     sp,4FA6h */
    SP = 0x4FA6;
L5443:
    /* 5443  sti */
    ;
L5444:
    /* 5444  call    _seg003_3D2E */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3D2E), 0x5447)) != 0) return c;
L5447:
    /* 5447  cli */
    ;
L5448:
    /* 5448  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L544D:
    /* 544D  mov     ss,dx */
    SET_SS(DX);
L544F:
    /* 544F  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L5454:
    /* 5454  mov     sp,dx */
    SP = DX;
L5456:
    /* 5456  sti */
    ;
L5457:
    /* 5457  pop     es */
    SET_ES(pop16());
L5458:
    /* 5458  pop     ds */
    SET_DS(pop16());
L5459:
    /* 5459  pop     si */
    SI = pop16();
L545A:
    /* 545A  pop     di */
    DI = pop16();
L545B:
    /* 545B  pop     bp */
    BP = pop16();
L545C:
    /* 545C  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_545D  (+545D)
       vline: a clipped vertical line (x, y1, y2; GRLIBF's _3BA1). */
L545D: /* _seg003_545D */
    /* 545D  push    bp */
    push16(BP);
L545E:
    /* 545E  mov     bp,sp */
    BP = SP;
L5460:
    /* 5460  push    di */
    push16(DI);
L5461:
    /* 5461  push    si */
    push16(SI);
L5462:
    /* 5462  push    ds */
    push16(asm_ds);
L5463:
    /* 5463  push    es */
    push16(asm_es);
L5464:
    /* 5464  mov     dx,ss */
    DX = asm_ss;
L5466:
    /* 5466  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L546B:
    /* 546B  mov     dx,sp */
    DX = SP;
L546D:
    /* 546D  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5472:
    /* 5472  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L5475:
    /* 5475  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L5478:
    /* 5478  mov     cx,word ptr [bp+0Ah] */
    CX = rw(pSS, BP + 0xA);
L547B:
    /* 547B  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L547E:
    /* 547E  mov     ds,dx */
    SET_DS(DX);
L5480:
    /* 5480  mov     es,dx */
    SET_ES(DX);
L5482:
    /* 5482  cli */
    ;
L5483:
    /* 5483  mov     ss,dx */
    SET_SS(DX);
L5485:
    /* 5485  mov     sp,4FA6h */
    SP = 0x4FA6;
L5488:
    /* 5488  sti */
    ;
L5489:
    /* 5489  xchg    cx,dx */
    { uint16_t t_ = DX;
    DX = CX;
    CX = t_; }
L548B:
    /* 548B  call    _seg003_3BA1 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3BA1), 0x548E)) != 0) return c;
L548E:
    /* 548E  cli */
    ;
L548F:
    /* 548F  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L5494:
    /* 5494  mov     ss,dx */
    SET_SS(DX);
L5496:
    /* 5496  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L549B:
    /* 549B  mov     sp,dx */
    SP = DX;
L549D:
    /* 549D  sti */
    ;
L549E:
    /* 549E  pop     es */
    SET_ES(pop16());
L549F:
    /* 549F  pop     ds */
    SET_DS(pop16());
L54A0:
    /* 54A0  pop     si */
    SI = pop16();
L54A1:
    /* 54A1  pop     di */
    DI = pop16();
L54A2:
    /* 54A2  pop     bp */
    BP = pop16();
L54A3:
    /* 54A3  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_54A4  (+54A4) */
L54A4: /* _uvline */
    /* 54A4  push    bp */
    push16(BP);
L54A5:
    /* 54A5  mov     bp,sp */
    BP = SP;
L54A7:
    /* 54A7  push    di */
    push16(DI);
L54A8:
    /* 54A8  push    si */
    push16(SI);
L54A9:
    /* 54A9  push    ds */
    push16(asm_ds);
L54AA:
    /* 54AA  push    es */
    push16(asm_es);
L54AB:
    /* 54AB  mov     dx,ss */
    DX = asm_ss;
L54AD:
    /* 54AD  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L54B2:
    /* 54B2  mov     dx,sp */
    DX = SP;
L54B4:
    /* 54B4  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L54B9:
    /* 54B9  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L54BC:
    /* 54BC  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L54BF:
    /* 54BF  mov     cx,word ptr [bp+0Ah] */
    CX = rw(pSS, BP + 0xA);
L54C2:
    /* 54C2  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L54C5:
    /* 54C5  mov     ds,dx */
    SET_DS(DX);
L54C7:
    /* 54C7  mov     es,dx */
    SET_ES(DX);
L54C9:
    /* 54C9  cli */
    ;
L54CA:
    /* 54CA  mov     ss,dx */
    SET_SS(DX);
L54CC:
    /* 54CC  mov     sp,4FA6h */
    SP = 0x4FA6;
L54CF:
    /* 54CF  sti */
    ;
L54D0:
    /* 54D0  xchg    cx,dx */
    { uint16_t t_ = DX;
    DX = CX;
    CX = t_; }
L54D2:
    /* 54D2  call    _seg003_3BA4 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3BA4), 0x54D5)) != 0) return c;
L54D5:
    /* 54D5  cli */
    ;
L54D6:
    /* 54D6  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L54DB:
    /* 54DB  mov     ss,dx */
    SET_SS(DX);
L54DD:
    /* 54DD  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L54E2:
    /* 54E2  mov     sp,dx */
    SP = DX;
L54E4:
    /* 54E4  sti */
    ;
L54E5:
    /* 54E5  pop     es */
    SET_ES(pop16());
L54E6:
    /* 54E6  pop     ds */
    SET_DS(pop16());
L54E7:
    /* 54E7  pop     si */
    SI = pop16();
L54E8:
    /* 54E8  pop     di */
    DI = pop16();
L54E9:
    /* 54E9  pop     bp */
    BP = pop16();
L54EA:
    /* 54EA  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_54EB  (+54EB) */
L54EB: /* _rectangle */
    /* 54EB  push    bp */
    push16(BP);
L54EC:
    /* 54EC  mov     bp,sp */
    BP = SP;
L54EE:
    /* 54EE  push    di */
    push16(DI);
L54EF:
    /* 54EF  push    si */
    push16(SI);
L54F0:
    /* 54F0  push    ds */
    push16(asm_ds);
L54F1:
    /* 54F1  push    es */
    push16(asm_es);
L54F2:
    /* 54F2  mov     dx,ss */
    DX = asm_ss;
L54F4:
    /* 54F4  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L54F9:
    /* 54F9  mov     dx,sp */
    DX = SP;
L54FB:
    /* 54FB  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5500:
    /* 5500  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L5503:
    /* 5503  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L5506:
    /* 5506  mov     cx,word ptr [bp+0Ah] */
    CX = rw(pSS, BP + 0xA);
L5509:
    /* 5509  mov     si,word ptr [bp+0Ch] */
    SI = rw(pSS, BP + 0xC);
L550C:
    /* 550C  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L550F:
    /* 550F  mov     ds,dx */
    SET_DS(DX);
L5511:
    /* 5511  mov     es,dx */
    SET_ES(DX);
L5513:
    /* 5513  cli */
    ;
L5514:
    /* 5514  mov     ss,dx */
    SET_SS(DX);
L5516:
    /* 5516  mov     sp,4FA6h */
    SP = 0x4FA6;
L5519:
    /* 5519  sti */
    ;
L551A:
    /* 551A  mov     dx,si */
    DX = SI;
L551C:
    /* 551C  call    _seg003_3C9E */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3C9E), 0x551F)) != 0) return c;
L551F:
    /* 551F  cli */
    ;
L5520:
    /* 5520  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L5525:
    /* 5525  mov     ss,dx */
    SET_SS(DX);
L5527:
    /* 5527  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L552C:
    /* 552C  mov     sp,dx */
    SP = DX;
L552E:
    /* 552E  sti */
    ;
L552F:
    /* 552F  pop     es */
    SET_ES(pop16());
L5530:
    /* 5530  pop     ds */
    SET_DS(pop16());
L5531:
    /* 5531  pop     si */
    SI = pop16();
L5532:
    /* 5532  pop     di */
    DI = pop16();
L5533:
    /* 5533  pop     bp */
    BP = pop16();
L5534:
    /* 5534  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_5535  (+5535) */
L5535: /* _urectangle */
    /* 5535  push    bp */
    push16(BP);
L5536:
    /* 5536  mov     bp,sp */
    BP = SP;
L5538:
    /* 5538  push    di */
    push16(DI);
L5539:
    /* 5539  push    si */
    push16(SI);
L553A:
    /* 553A  push    ds */
    push16(asm_ds);
L553B:
    /* 553B  push    es */
    push16(asm_es);
L553C:
    /* 553C  mov     dx,ss */
    DX = asm_ss;
L553E:
    /* 553E  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L5543:
    /* 5543  mov     dx,sp */
    DX = SP;
L5545:
    /* 5545  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L554A:
    /* 554A  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L554D:
    /* 554D  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L5550:
    /* 5550  mov     cx,word ptr [bp+0Ah] */
    CX = rw(pSS, BP + 0xA);
L5553:
    /* 5553  mov     si,word ptr [bp+0Ch] */
    SI = rw(pSS, BP + 0xC);
L5556:
    /* 5556  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L5559:
    /* 5559  mov     ds,dx */
    SET_DS(DX);
L555B:
    /* 555B  mov     es,dx */
    SET_ES(DX);
L555D:
    /* 555D  cli */
    ;
L555E:
    /* 555E  mov     ss,dx */
    SET_SS(DX);
L5560:
    /* 5560  mov     sp,4FA6h */
    SP = 0x4FA6;
L5563:
    /* 5563  sti */
    ;
L5564:
    /* 5564  mov     dx,si */
    DX = SI;
L5566:
    /* 5566  call    _seg003_3CAE */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3CAE), 0x5569)) != 0) return c;
L5569:
    /* 5569  cli */
    ;
L556A:
    /* 556A  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L556F:
    /* 556F  mov     ss,dx */
    SET_SS(DX);
L5571:
    /* 5571  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L5576:
    /* 5576  mov     sp,dx */
    SP = DX;
L5578:
    /* 5578  sti */
    ;
L5579:
    /* 5579  pop     es */
    SET_ES(pop16());
L557A:
    /* 557A  pop     ds */
    SET_DS(pop16());
L557B:
    /* 557B  pop     si */
    SI = pop16();
L557C:
    /* 557C  pop     di */
    DI = pop16();
L557D:
    /* 557D  pop     bp */
    BP = pop16();
L557E:
    /* 557E  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* clear_window  (+557F) */
L557F: /* _clear_window */
    /* 557F  push    bp */
    push16(BP);
L5580:
    /* 5580  mov     bp,sp */
    BP = SP;
L5582:
    /* 5582  push    di */
    push16(DI);
L5583:
    /* 5583  push    si */
    push16(SI);
L5584:
    /* 5584  push    ds */
    push16(asm_ds);
L5585:
    /* 5585  push    es */
    push16(asm_es);
L5586:
    /* 5586  mov     dx,ss */
    DX = asm_ss;
L5588:
    /* 5588  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L558D:
    /* 558D  mov     dx,sp */
    DX = SP;
L558F:
    /* 558F  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5594:
    /* 5594  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L5597:
    /* 5597  mov     ds,dx */
    SET_DS(DX);
L5599:
    /* 5599  mov     es,dx */
    SET_ES(DX);
L559B:
    /* 559B  cli */
    ;
L559C:
    /* 559C  mov     ss,dx */
    SET_SS(DX);
L559E:
    /* 559E  mov     sp,4FA6h */
    SP = 0x4FA6;
L55A1:
    /* 55A1  sti */
    ;
L55A2:
    /* 55A2  call    _seg003_3CA3 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3CA3), 0x55A5)) != 0) return c;
L55A5:
    /* 55A5  cli */
    ;
L55A6:
    /* 55A6  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L55AB:
    /* 55AB  mov     ss,dx */
    SET_SS(DX);
L55AD:
    /* 55AD  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L55B2:
    /* 55B2  mov     sp,dx */
    SP = DX;
L55B4:
    /* 55B4  sti */
    ;
L55B5:
    /* 55B5  pop     es */
    SET_ES(pop16());
L55B6:
    /* 55B6  pop     ds */
    SET_DS(pop16());
L55B7:
    /* 55B7  pop     si */
    SI = pop16();
L55B8:
    /* 55B8  pop     di */
    DI = pop16();
L55B9:
    /* 55B9  pop     bp */
    BP = pop16();
L55BA:
    /* 55BA  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* box  (+55BB) */
L55BB: /* _box */
    /* 55BB  push    bp */
    push16(BP);
L55BC:
    /* 55BC  mov     bp,sp */
    BP = SP;
L55BE:
    /* 55BE  push    di */
    push16(DI);
L55BF:
    /* 55BF  push    si */
    push16(SI);
L55C0:
    /* 55C0  push    ds */
    push16(asm_ds);
L55C1:
    /* 55C1  push    es */
    push16(asm_es);
L55C2:
    /* 55C2  mov     dx,ss */
    DX = asm_ss;
L55C4:
    /* 55C4  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L55C9:
    /* 55C9  mov     dx,sp */
    DX = SP;
L55CB:
    /* 55CB  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L55D0:
    /* 55D0  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L55D3:
    /* 55D3  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L55D6:
    /* 55D6  mov     cx,word ptr [bp+0Ah] */
    CX = rw(pSS, BP + 0xA);
L55D9:
    /* 55D9  mov     si,word ptr [bp+0Ch] */
    SI = rw(pSS, BP + 0xC);
L55DC:
    /* 55DC  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L55DF:
    /* 55DF  mov     ds,dx */
    SET_DS(DX);
L55E1:
    /* 55E1  mov     es,dx */
    SET_ES(DX);
L55E3:
    /* 55E3  cli */
    ;
L55E4:
    /* 55E4  mov     ss,dx */
    SET_SS(DX);
L55E6:
    /* 55E6  mov     sp,4FA6h */
    SP = 0x4FA6;
L55E9:
    /* 55E9  sti */
    ;
L55EA:
    /* 55EA  mov     dx,si */
    DX = SI;
L55EC:
    /* 55EC  call    _seg003_3BEE */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3BEE), 0x55EF)) != 0) return c;
L55EF:
    /* 55EF  cli */
    ;
L55F0:
    /* 55F0  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L55F5:
    /* 55F5  mov     ss,dx */
    SET_SS(DX);
L55F7:
    /* 55F7  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L55FC:
    /* 55FC  mov     sp,dx */
    SP = DX;
L55FE:
    /* 55FE  sti */
    ;
L55FF:
    /* 55FF  pop     es */
    SET_ES(pop16());
L5600:
    /* 5600  pop     ds */
    SET_DS(pop16());
L5601:
    /* 5601  pop     si */
    SI = pop16();
L5602:
    /* 5602  pop     di */
    DI = pop16();
L5603:
    /* 5603  pop     bp */
    BP = pop16();
L5604:
    /* 5604  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_5605  (+5605)
       ubox: an unclipped outline rectangle (GRLIBF's _3BF1). */
L5605: /* _seg003_5605 */
    /* 5605  push    bp */
    push16(BP);
L5606:
    /* 5606  mov     bp,sp */
    BP = SP;
L5608:
    /* 5608  push    di */
    push16(DI);
L5609:
    /* 5609  push    si */
    push16(SI);
L560A:
    /* 560A  push    ds */
    push16(asm_ds);
L560B:
    /* 560B  push    es */
    push16(asm_es);
L560C:
    /* 560C  mov     dx,ss */
    DX = asm_ss;
L560E:
    /* 560E  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L5613:
    /* 5613  mov     dx,sp */
    DX = SP;
L5615:
    /* 5615  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L561A:
    /* 561A  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L561D:
    /* 561D  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L5620:
    /* 5620  mov     cx,word ptr [bp+0Ah] */
    CX = rw(pSS, BP + 0xA);
L5623:
    /* 5623  mov     si,word ptr [bp+0Ch] */
    SI = rw(pSS, BP + 0xC);
L5626:
    /* 5626  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L5629:
    /* 5629  mov     ds,dx */
    SET_DS(DX);
L562B:
    /* 562B  mov     es,dx */
    SET_ES(DX);
L562D:
    /* 562D  cli */
    ;
L562E:
    /* 562E  mov     ss,dx */
    SET_SS(DX);
L5630:
    /* 5630  mov     sp,4FA6h */
    SP = 0x4FA6;
L5633:
    /* 5633  sti */
    ;
L5634:
    /* 5634  mov     dx,si */
    DX = SI;
L5636:
    /* 5636  call    _seg003_3BF1 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3BF1), 0x5639)) != 0) return c;
L5639:
    /* 5639  cli */
    ;
L563A:
    /* 563A  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L563F:
    /* 563F  mov     ss,dx */
    SET_SS(DX);
L5641:
    /* 5641  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L5646:
    /* 5646  mov     sp,dx */
    SP = DX;
L5648:
    /* 5648  sti */
    ;
L5649:
    /* 5649  pop     es */
    SET_ES(pop16());
L564A:
    /* 564A  pop     ds */
    SET_DS(pop16());
L564B:
    /* 564B  pop     si */
    SI = pop16();
L564C:
    /* 564C  pop     di */
    DI = pop16();
L564D:
    /* 564D  pop     bp */
    BP = pop16();
L564E:
    /* 564E  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_564F  (+564F)
       Fill the whole window (GRLIBF's urectangle on the window's corners). */
L564F: /* _seg003_564F */
    /* 564F  push    bp */
    push16(BP);
L5650:
    /* 5650  mov     bp,sp */
    BP = SP;
L5652:
    /* 5652  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L5655:
    /* 5655  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L5658:
    /* 5658  mov     cx,word ptr [bp+0Ah] */
    CX = rw(pSS, BP + 0xA);
L565B:
    /* 565B  mov     dx,word ptr [bp+0Ch] */
    DX = rw(pSS, BP + 0xC);
L565E:
    /* 565E  mov     bp,dx */
    BP = DX;
L5660:
    /* 5660  push    di */
    push16(DI);
L5661:
    /* 5661  push    si */
    push16(SI);
L5662:
    /* 5662  push    ds */
    push16(asm_ds);
L5663:
    /* 5663  push    es */
    push16(asm_es);
L5664:
    /* 5664  mov     dx,ss */
    DX = asm_ss;
L5666:
    /* 5666  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L566B:
    /* 566B  mov     dx,sp */
    DX = SP;
L566D:
    /* 566D  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5672:
    /* 5672  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L5675:
    /* 5675  mov     ds,dx */
    SET_DS(DX);
L5677:
    /* 5677  mov     es,dx */
    SET_ES(DX);
L5679:
    /* 5679  cli */
    ;
L567A:
    /* 567A  mov     ss,dx */
    SET_SS(DX);
L567C:
    /* 567C  mov     sp,4FA6h */
    SP = 0x4FA6;
L567F:
    /* 567F  sti */
    ;
L5680:
    /* 5680  mov     dx,bp */
    DX = BP;
L5682:
    /* 5682  push    word ptr ds:[SPAN_WRITER] */
    push16(rw(pDS, 0x4110));
L5686:
    /* 5686  mov     word ptr ds:[SPAN_WRITER],offset _seg003_5AC6 */
    ww(pDS, 0x4110, 0x5AC6);
L568C:
    /* 568C  call    _seg003_3CAE */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3CAE), 0x568F)) != 0) return c;
L568F:
    /* 568F  pop     word ptr ds:[SPAN_WRITER] */
    { uint16_t t_ = pop16(); ww(pDS, 0x4110, t_); }
L5693:
    /* 5693  cli */
    ;
L5694:
    /* 5694  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L5699:
    /* 5699  mov     ss,dx */
    SET_SS(DX);
L569B:
    /* 569B  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L56A0:
    /* 56A0  mov     sp,dx */
    SP = DX;
L56A2:
    /* 56A2  sti */
    ;
L56A3:
    /* 56A3  pop     es */
    SET_ES(pop16());
L56A4:
    /* 56A4  pop     ds */
    SET_DS(pop16());
L56A5:
    /* 56A5  pop     si */
    SI = pop16();
L56A6:
    /* 56A6  pop     di */
    DI = pop16();
L56A7:
    /* 56A7  pop     bp */
    BP = pop16();
L56A8:
    /* 56A8  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_56A9  (+56A9)
       cline: a clipped line (x1, y1, x2, y2; GRLIBG's _3F0E). */
L56A9: /* _seg003_56A9 */
    /* 56A9  push    bp */
    push16(BP);
L56AA:
    /* 56AA  mov     bp,sp */
    BP = SP;
L56AC:
    /* 56AC  push    di */
    push16(DI);
L56AD:
    /* 56AD  push    si */
    push16(SI);
L56AE:
    /* 56AE  push    ds */
    push16(asm_ds);
L56AF:
    /* 56AF  push    es */
    push16(asm_es);
L56B0:
    /* 56B0  mov     dx,ss */
    DX = asm_ss;
L56B2:
    /* 56B2  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L56B7:
    /* 56B7  mov     dx,sp */
    DX = SP;
L56B9:
    /* 56B9  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L56BE:
    /* 56BE  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L56C1:
    /* 56C1  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L56C4:
    /* 56C4  mov     cx,word ptr [bp+0Ah] */
    CX = rw(pSS, BP + 0xA);
L56C7:
    /* 56C7  mov     si,word ptr [bp+0Ch] */
    SI = rw(pSS, BP + 0xC);
L56CA:
    /* 56CA  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L56CD:
    /* 56CD  mov     ds,dx */
    SET_DS(DX);
L56CF:
    /* 56CF  mov     es,dx */
    SET_ES(DX);
L56D1:
    /* 56D1  cli */
    ;
L56D2:
    /* 56D2  mov     ss,dx */
    SET_SS(DX);
L56D4:
    /* 56D4  mov     sp,4FA6h */
    SP = 0x4FA6;
L56D7:
    /* 56D7  sti */
    ;
L56D8:
    /* 56D8  mov     dx,si */
    DX = SI;
L56DA:
    /* 56DA  call    _seg003_3F0E */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3F0E), 0x56DD)) != 0) return c;
L56DD:
    /* 56DD  cli */
    ;
L56DE:
    /* 56DE  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L56E3:
    /* 56E3  mov     ss,dx */
    SET_SS(DX);
L56E5:
    /* 56E5  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L56EA:
    /* 56EA  mov     sp,dx */
    SP = DX;
L56EC:
    /* 56EC  sti */
    ;
L56ED:
    /* 56ED  pop     es */
    SET_ES(pop16());
L56EE:
    /* 56EE  pop     ds */
    SET_DS(pop16());
L56EF:
    /* 56EF  pop     si */
    SI = pop16();
L56F0:
    /* 56F0  pop     di */
    DI = pop16();
L56F1:
    /* 56F1  pop     bp */
    BP = pop16();
L56F2:
    /* 56F2  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_56F3  (+56F3)
       polygon: CX = the vertex count, the vertices already at 415E (GRLIBI's _438D). */
L56F3: /* _seg003_56F3 */
    /* 56F3  push    bp */
    push16(BP);
L56F4:
    /* 56F4  mov     bp,sp */
    BP = SP;
L56F6:
    /* 56F6  push    di */
    push16(DI);
L56F7:
    /* 56F7  push    si */
    push16(SI);
L56F8:
    /* 56F8  push    ds */
    push16(asm_ds);
L56F9:
    /* 56F9  push    es */
    push16(asm_es);
L56FA:
    /* 56FA  mov     dx,ss */
    DX = asm_ss;
L56FC:
    /* 56FC  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L5701:
    /* 5701  mov     dx,sp */
    DX = SP;
L5703:
    /* 5703  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5708:
    /* 5708  mov     cx,word ptr [bp+6] */
    CX = rw(pSS, BP + 0x6);
L570B:
    /* 570B  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L570E:
    /* 570E  mov     ds,dx */
    SET_DS(DX);
L5710:
    /* 5710  mov     es,dx */
    SET_ES(DX);
L5712:
    /* 5712  cli */
    ;
L5713:
    /* 5713  mov     ss,dx */
    SET_SS(DX);
L5715:
    /* 5715  mov     sp,4FA6h */
    SP = 0x4FA6;
L5718:
    /* 5718  sti */
    ;
L5719:
    /* 5719  call    _seg003_438D */
    if ((c = asm_call(ASM_JMP(0x0090, 0x438D), 0x571C)) != 0) return c;
L571C:
    /* 571C  cli */
    ;
L571D:
    /* 571D  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L5722:
    /* 5722  mov     ss,dx */
    SET_SS(DX);
L5724:
    /* 5724  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L5729:
    /* 5729  mov     sp,dx */
    SP = DX;
L572B:
    /* 572B  sti */
    ;
L572C:
    /* 572C  pop     es */
    SET_ES(pop16());
L572D:
    /* 572D  pop     ds */
    SET_DS(pop16());
L572E:
    /* 572E  pop     si */
    SI = pop16();
L572F:
    /* 572F  pop     di */
    DI = pop16();
L5730:
    /* 5730  pop     bp */
    BP = pop16();
L5731:
    /* 5731  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_5732  (+5732)
       upolygon: as polygon, without clipping (GRLIBI's _4134). */
L5732: /* _seg003_5732 */
    /* 5732  push    bp */
    push16(BP);
L5733:
    /* 5733  mov     bp,sp */
    BP = SP;
L5735:
    /* 5735  push    di */
    push16(DI);
L5736:
    /* 5736  push    si */
    push16(SI);
L5737:
    /* 5737  push    ds */
    push16(asm_ds);
L5738:
    /* 5738  push    es */
    push16(asm_es);
L5739:
    /* 5739  mov     dx,ss */
    DX = asm_ss;
L573B:
    /* 573B  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L5740:
    /* 5740  mov     dx,sp */
    DX = SP;
L5742:
    /* 5742  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5747:
    /* 5747  mov     cx,word ptr [bp+6] */
    CX = rw(pSS, BP + 0x6);
L574A:
    /* 574A  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L574D:
    /* 574D  mov     ds,dx */
    SET_DS(DX);
L574F:
    /* 574F  mov     es,dx */
    SET_ES(DX);
L5751:
    /* 5751  cli */
    ;
L5752:
    /* 5752  mov     ss,dx */
    SET_SS(DX);
L5754:
    /* 5754  mov     sp,4FA6h */
    SP = 0x4FA6;
L5757:
    /* 5757  sti */
    ;
L5758:
    /* 5758  call    _seg003_4134 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x4134), 0x575B)) != 0) return c;
L575B:
    /* 575B  cli */
    ;
L575C:
    /* 575C  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L5761:
    /* 5761  mov     ss,dx */
    SET_SS(DX);
L5763:
    /* 5763  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L5768:
    /* 5768  mov     sp,dx */
    SP = DX;
L576A:
    /* 576A  sti */
    ;
L576B:
    /* 576B  pop     es */
    SET_ES(pop16());
L576C:
    /* 576C  pop     ds */
    SET_DS(pop16());
L576D:
    /* 576D  pop     si */
    SI = pop16();
L576E:
    /* 576E  pop     di */
    DI = pop16();
L576F:
    /* 576F  pop     bp */
    BP = pop16();
L5770:
    /* 5770  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_5771  (+5771)
       uline: an unclipped line (GRLIBH's _3FDC). */
L5771: /* _seg003_5771 */
    /* 5771  push    bp */
    push16(BP);
L5772:
    /* 5772  mov     bp,sp */
    BP = SP;
L5774:
    /* 5774  push    di */
    push16(DI);
L5775:
    /* 5775  push    si */
    push16(SI);
L5776:
    /* 5776  push    ds */
    push16(asm_ds);
L5777:
    /* 5777  push    es */
    push16(asm_es);
L5778:
    /* 5778  mov     dx,ss */
    DX = asm_ss;
L577A:
    /* 577A  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L577F:
    /* 577F  mov     dx,sp */
    DX = SP;
L5781:
    /* 5781  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5786:
    /* 5786  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L5789:
    /* 5789  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L578C:
    /* 578C  mov     cx,word ptr [bp+0Ah] */
    CX = rw(pSS, BP + 0xA);
L578F:
    /* 578F  mov     si,word ptr [bp+0Ch] */
    SI = rw(pSS, BP + 0xC);
L5792:
    /* 5792  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L5795:
    /* 5795  mov     ds,dx */
    SET_DS(DX);
L5797:
    /* 5797  mov     es,dx */
    SET_ES(DX);
L5799:
    /* 5799  cli */
    ;
L579A:
    /* 579A  mov     ss,dx */
    SET_SS(DX);
L579C:
    /* 579C  mov     sp,4FA6h */
    SP = 0x4FA6;
L579F:
    /* 579F  sti */
    ;
L57A0:
    /* 57A0  mov     dx,si */
    DX = SI;
L57A2:
    /* 57A2  call    _seg003_3FDC */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3FDC), 0x57A5)) != 0) return c;
L57A5:
    /* 57A5  cli */
    ;
L57A6:
    /* 57A6  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L57AB:
    /* 57AB  mov     ss,dx */
    SET_SS(DX);
L57AD:
    /* 57AD  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L57B2:
    /* 57B2  mov     sp,dx */
    SP = DX;
L57B4:
    /* 57B4  sti */
    ;
L57B5:
    /* 57B5  pop     es */
    SET_ES(pop16());
L57B6:
    /* 57B6  pop     ds */
    SET_DS(pop16());
L57B7:
    /* 57B7  pop     si */
    SI = pop16();
L57B8:
    /* 57B8  pop     di */
    DI = pop16();
L57B9:
    /* 57B9  pop     bp */
    BP = pop16();
L57BA:
    /* 57BA  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* DrawArtToScreen_seg003_57BB  (+57BB) */
L57BB: /* _show */
    /* 57BB  push    bp */
    push16(BP);
L57BC:
    /* 57BC  mov     bp,sp */
    BP = SP;
L57BE:
    /* 57BE  push    di */
    push16(DI);
L57BF:
    /* 57BF  push    si */
    push16(SI);
L57C0:
    /* 57C0  push    ds */
    push16(asm_ds);
L57C1:
    /* 57C1  push    es */
    push16(asm_es);
L57C2:
    /* 57C2  mov     dx,ss */
    DX = asm_ss;
L57C4:
    /* 57C4  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L57C9:
    /* 57C9  mov     dx,sp */
    DX = SP;
L57CB:
    /* 57CB  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L57D0:
    /* 57D0  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L57D3:
    /* 57D3  mov     es,ax */
    SET_ES(AX);
L57D5:
    /* 57D5  mov     ax,word ptr [bp+12h] */
    AX = rw(pSS, BP + 0x12);
L57D8:
    /* 57D8  mov     word ptr es:[0DC8h],ax */
    ww(pES, 0xDC8, AX);
L57DC:
    /* 57DC  mov     ax,word ptr [bp+14h] */
    AX = rw(pSS, BP + 0x14);
L57DF:
    /* 57DF  mov     word ptr es:[0DCAh],ax */
    ww(pES, 0xDCA, AX);
L57E3:
    /* 57E3  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L57E6:
    /* 57E6  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L57E9:
    /* 57E9  mov     di,word ptr [bp+0Ah] */
    DI = rw(pSS, BP + 0xA);
L57EC:
    /* 57EC  mov     si,word ptr [bp+0Ch] */
    SI = rw(pSS, BP + 0xC);
L57EF:
    /* 57EF  mov     cx,word ptr [bp+10h] */
    CX = rw(pSS, BP + 0x10);
L57F2:
    /* 57F2  mov     bp,word ptr [bp+0Eh] */
    BP = rw(pSS, BP + 0xE);
L57F5:
    /* 57F5  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L57F8:
    /* 57F8  mov     ds,dx */
    SET_DS(DX);
L57FA:
    /* 57FA  mov     es,dx */
    SET_ES(DX);
L57FC:
    /* 57FC  cli */
    ;
L57FD:
    /* 57FD  mov     ss,dx */
    SET_SS(DX);
L57FF:
    /* 57FF  mov     sp,4FA6h */
    SP = 0x4FA6;
L5802:
    /* 5802  sti */
    ;
L5803:
    /* 5803  mov     dx,bp */
    DX = BP;
L5805:
    /* 5805  call    _seg003_2BBA */
    if ((c = asm_call(ASM_JMP(0x0090, 0x2BBA), 0x5808)) != 0) return c;
L5808:
    /* 5808  cli */
    ;
L5809:
    /* 5809  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L580E:
    /* 580E  mov     ss,dx */
    SET_SS(DX);
L5810:
    /* 5810  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L5815:
    /* 5815  mov     sp,dx */
    SP = DX;
L5817:
    /* 5817  sti */
    ;
L5818:
    /* 5818  pop     es */
    SET_ES(pop16());
L5819:
    /* 5819  pop     ds */
    SET_DS(pop16());
L581A:
    /* 581A  pop     si */
    SI = pop16();
L581B:
    /* 581B  pop     di */
    DI = pop16();
L581C:
    /* 581C  pop     bp */
    BP = pop16();
L581D:
    /* 581D  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* DRAW_RELATED_seg003_581E  (+581E) */
L581E: /* _seg003_581E */
    /* 581E  push    bp */
    push16(BP);
L581F:
    /* 581F  mov     bp,sp */
    BP = SP;
L5821:
    /* 5821  push    di */
    push16(DI);
L5822:
    /* 5822  push    si */
    push16(SI);
L5823:
    /* 5823  push    ds */
    push16(asm_ds);
L5824:
    /* 5824  push    es */
    push16(asm_es);
L5825:
    /* 5825  mov     dx,ss */
    DX = asm_ss;
L5827:
    /* 5827  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L582C:
    /* 582C  mov     dx,sp */
    DX = SP;
L582E:
    /* 582E  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5833:
    /* 5833  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L5836:
    /* 5836  mov     es,ax */
    SET_ES(AX);
L5838:
    /* 5838  mov     ax,word ptr [bp+10h] */
    AX = rw(pSS, BP + 0x10);
L583B:
    /* 583B  mov     word ptr es:[0DC8h],ax */
    ww(pES, 0xDC8, AX);
L583F:
    /* 583F  mov     ax,word ptr [bp+12h] */
    AX = rw(pSS, BP + 0x12);
L5842:
    /* 5842  mov     word ptr es:[0DCAh],ax */
    ww(pES, 0xDCA, AX);
L5846:
    /* 5846  mov     di,word ptr [bp+6] */
    DI = rw(pSS, BP + 0x6);
L5849:
    /* 5849  mov     si,0A000h */
    SI = 0xA000;
L584C:
    /* 584C  mov     ax,word ptr [bp+8] */
    AX = rw(pSS, BP + 0x8);
L584F:
    /* 584F  mov     bx,word ptr [bp+0Ah] */
    BX = rw(pSS, BP + 0xA);
L5852:
    /* 5852  mov     cx,word ptr [bp+0Ch] */
    CX = rw(pSS, BP + 0xC);
L5855:
    /* 5855  mov     bp,word ptr [bp+0Eh] */
    BP = rw(pSS, BP + 0xE);
L5858:
    /* 5858  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L585B:
    /* 585B  mov     ds,dx */
    SET_DS(DX);
L585D:
    /* 585D  mov     es,dx */
    SET_ES(DX);
L585F:
    /* 585F  cli */
    ;
L5860:
    /* 5860  mov     ss,dx */
    SET_SS(DX);
L5862:
    /* 5862  mov     sp,4FA6h */
    SP = 0x4FA6;
L5865:
    /* 5865  sti */
    ;
L5866:
    /* 5866  mov     dx,bp */
    DX = BP;
L5868:
    /* 5868  call    _seg003_2BD3 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x2BD3), 0x586B)) != 0) return c;
L586B:
    /* 586B  cli */
    ;
L586C:
    /* 586C  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L5871:
    /* 5871  mov     ss,dx */
    SET_SS(DX);
L5873:
    /* 5873  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L5878:
    /* 5878  mov     sp,dx */
    SP = DX;
L587A:
    /* 587A  sti */
    ;
L587B:
    /* 587B  pop     es */
    SET_ES(pop16());
L587C:
    /* 587C  pop     ds */
    SET_DS(pop16());
L587D:
    /* 587D  pop     si */
    SI = pop16();
L587E:
    /* 587E  pop     di */
    DI = pop16();
L587F:
    /* 587F  pop     bp */
    BP = pop16();
L5880:
    /* 5880  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_5881  (+5881) */
L5881: /* _fbshow */
    /* 5881  push    bp */
    push16(BP);
L5882:
    /* 5882  mov     bp,sp */
    BP = SP;
L5884:
    /* 5884  push    di */
    push16(DI);
L5885:
    /* 5885  push    si */
    push16(SI);
L5886:
    /* 5886  push    ds */
    push16(asm_ds);
L5887:
    /* 5887  push    es */
    push16(asm_es);
L5888:
    /* 5888  mov     dx,ss */
    DX = asm_ss;
L588A:
    /* 588A  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L588F:
    /* 588F  mov     dx,sp */
    DX = SP;
L5891:
    /* 5891  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5896:
    /* 5896  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L5899:
    /* 5899  mov     es,ax */
    SET_ES(AX);
L589B:
    /* 589B  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L589D:
    /* 589D  mov     word ptr es:[0DC8h],ax */
    ww(pES, 0xDC8, AX);
L58A1:
    /* 58A1  mov     word ptr es:[0DCAh],ax */
    ww(pES, 0xDCA, AX);
L58A5:
    /* 58A5  mov     di,word ptr [bp+6] */
    DI = rw(pSS, BP + 0x6);
L58A8:
    /* 58A8  mov     si,word ptr [bp+8] */
    SI = rw(pSS, BP + 0x8);
L58AB:
    /* 58AB  mov     ax,word ptr [bp+0Ah] */
    AX = rw(pSS, BP + 0xA);
L58AE:
    /* 58AE  mov     bx,word ptr [bp+0Ch] */
    BX = rw(pSS, BP + 0xC);
L58B1:
    /* 58B1  mov     cx,word ptr [bp+0Eh] */
    CX = rw(pSS, BP + 0xE);
L58B4:
    /* 58B4  mov     bp,word ptr [bp+10h] */
    BP = rw(pSS, BP + 0x10);
L58B7:
    /* 58B7  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L58BA:
    /* 58BA  mov     ds,dx */
    SET_DS(DX);
L58BC:
    /* 58BC  mov     es,dx */
    SET_ES(DX);
L58BE:
    /* 58BE  cli */
    ;
L58BF:
    /* 58BF  mov     ss,dx */
    SET_SS(DX);
L58C1:
    /* 58C1  mov     sp,4FA6h */
    SP = 0x4FA6;
L58C4:
    /* 58C4  sti */
    ;
L58C5:
    /* 58C5  mov     dx,bp */
    DX = BP;
L58C7:
    /* 58C7  call    _seg003_2BFA */
    if ((c = asm_call(ASM_JMP(0x0090, 0x2BFA), 0x58CA)) != 0) return c;
L58CA:
    /* 58CA  cli */
    ;
L58CB:
    /* 58CB  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L58D0:
    /* 58D0  mov     ss,dx */
    SET_SS(DX);
L58D2:
    /* 58D2  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L58D7:
    /* 58D7  mov     sp,dx */
    SP = DX;
L58D9:
    /* 58D9  sti */
    ;
L58DA:
    /* 58DA  pop     es */
    SET_ES(pop16());
L58DB:
    /* 58DB  pop     ds */
    SET_DS(pop16());
L58DC:
    /* 58DC  pop     si */
    SI = pop16();
L58DD:
    /* 58DD  pop     di */
    DI = pop16();
L58DE:
    /* 58DE  pop     bp */
    BP = pop16();
L58DF:
    /* 58DF  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_58E0  (+58E0)
       Set the linear buffer bitmaps are drawn into by _5915 (VIDMODE's _33E3). */
L58E0: /* _seg003_58E0 */
    /* 58E0  push    bp */
    push16(BP);
L58E1:
    /* 58E1  mov     bp,sp */
    BP = SP;
L58E3:
    /* 58E3  push    di */
    push16(DI);
L58E4:
    /* 58E4  push    si */
    push16(SI);
L58E5:
    /* 58E5  push    ds */
    push16(asm_ds);
L58E6:
    /* 58E6  push    es */
    push16(asm_es);
L58E7:
    /* 58E7  mov     dx,ss */
    DX = asm_ss;
L58E9:
    /* 58E9  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L58EE:
    /* 58EE  mov     dx,sp */
    DX = SP;
L58F0:
    /* 58F0  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L58F5:
    /* 58F5  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L58F8:
    /* 58F8  mov     di,word ptr [bp+8] */
    DI = rw(pSS, BP + 0x8);
L58FB:
    /* 58FB  mov     bx,word ptr [bp+0Ah] */
    BX = rw(pSS, BP + 0xA);
L58FE:
    /* 58FE  mov     cx,word ptr [bp+0Ch] */
    CX = rw(pSS, BP + 0xC);
L5901:
    /* 5901  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L5904:
    /* 5904  mov     ds,dx */
    SET_DS(DX);
L5906:
    /* 5906  mov     es,dx */
    SET_ES(DX);
L5908:
    /* 5908  cli */
    ;
L5909:
    /* 5909  mov     ss,dx */
    SET_SS(DX);
L590B:
    /* 590B  mov     sp,4FA6h */
    SP = 0x4FA6;
L590E:
    /* 590E  sti */
    ;
L590F:
    /* 590F  mov     dx,di */
    DX = DI;
L5911:
    /* 5911  call    _seg003_33E3 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x33E3), 0x5914)) != 0) return c;
L5914:
    /* 5914  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_5915  (+5915)
       Draw a bitmap into that linear buffer (VIDMODE's _2C11), with fbshow's arguments. */
L5915: /* _seg003_5915 */
    /* 5915  push    bp */
    push16(BP);
L5916:
    /* 5916  mov     bp,sp */
    BP = SP;
L5918:
    /* 5918  push    di */
    push16(DI);
L5919:
    /* 5919  push    si */
    push16(SI);
L591A:
    /* 591A  push    ds */
    push16(asm_ds);
L591B:
    /* 591B  push    es */
    push16(asm_es);
L591C:
    /* 591C  mov     dx,ss */
    DX = asm_ss;
L591E:
    /* 591E  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L5923:
    /* 5923  mov     dx,sp */
    DX = SP;
L5925:
    /* 5925  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L592A:
    /* 592A  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L592D:
    /* 592D  mov     es,ax */
    SET_ES(AX);
L592F:
    /* 592F  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L5931:
    /* 5931  mov     word ptr es:[0DC8h],ax */
    ww(pES, 0xDC8, AX);
L5935:
    /* 5935  mov     word ptr es:[0DCAh],ax */
    ww(pES, 0xDCA, AX);
L5939:
    /* 5939  mov     di,word ptr [bp+6] */
    DI = rw(pSS, BP + 0x6);
L593C:
    /* 593C  mov     si,word ptr [bp+8] */
    SI = rw(pSS, BP + 0x8);
L593F:
    /* 593F  mov     ax,word ptr [bp+0Ah] */
    AX = rw(pSS, BP + 0xA);
L5942:
    /* 5942  mov     bx,word ptr [bp+0Ch] */
    BX = rw(pSS, BP + 0xC);
L5945:
    /* 5945  mov     cx,word ptr [bp+0Eh] */
    CX = rw(pSS, BP + 0xE);
L5948:
    /* 5948  mov     bp,word ptr [bp+10h] */
    BP = rw(pSS, BP + 0x10);
L594B:
    /* 594B  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L594E:
    /* 594E  mov     ds,dx */
    SET_DS(DX);
L5950:
    /* 5950  mov     es,dx */
    SET_ES(DX);
L5952:
    /* 5952  cli */
    ;
L5953:
    /* 5953  mov     ss,dx */
    SET_SS(DX);
L5955:
    /* 5955  mov     sp,4FA6h */
    SP = 0x4FA6;
L5958:
    /* 5958  sti */
    ;
L5959:
    /* 5959  mov     dx,bp */
    DX = BP;
L595B:
    /* 595B  call    _seg003_2C11 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x2C11), 0x595E)) != 0) return c;
L595E:
    /* 595E  cli */
    ;
L595F:
    /* 595F  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L5964:
    /* 5964  mov     ss,dx */
    SET_SS(DX);
L5966:
    /* 5966  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L596B:
    /* 596B  mov     sp,dx */
    SP = DX;
L596D:
    /* 596D  sti */
    ;
L596E:
    /* 596E  pop     es */
    SET_ES(pop16());
L596F:
    /* 596F  pop     ds */
    SET_DS(pop16());
L5970:
    /* 5970  pop     si */
    SI = pop16();
L5971:
    /* 5971  pop     di */
    DI = pop16();
L5972:
    /* 5972  pop     bp */
    BP = pop16();
L5973:
    /* 5973  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_5974  (+5974) */
L5974: /* _vcopyfb */
    /* 5974  push    bp */
    push16(BP);
L5975:
    /* 5975  mov     bp,sp */
    BP = SP;
L5977:
    /* 5977  push    di */
    push16(DI);
L5978:
    /* 5978  push    si */
    push16(SI);
L5979:
    /* 5979  push    ds */
    push16(asm_ds);
L597A:
    /* 597A  push    es */
    push16(asm_es);
L597B:
    /* 597B  mov     dx,ss */
    DX = asm_ss;
L597D:
    /* 597D  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L5982:
    /* 5982  mov     dx,sp */
    DX = SP;
L5984:
    /* 5984  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5989:
    /* 5989  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L598C:
    /* 598C  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L598F:
    /* 598F  mov     cx,word ptr [bp+0Ah] */
    CX = rw(pSS, BP + 0xA);
L5992:
    /* 5992  mov     di,word ptr [bp+0Eh] */
    DI = rw(pSS, BP + 0xE);
L5995:
    /* 5995  mov     bp,word ptr [bp+0Ch] */
    BP = rw(pSS, BP + 0xC);
L5998:
    /* 5998  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L599B:
    /* 599B  mov     ds,dx */
    SET_DS(DX);
L599D:
    /* 599D  mov     es,dx */
    SET_ES(DX);
L599F:
    /* 599F  cli */
    ;
L59A0:
    /* 59A0  mov     ss,dx */
    SET_SS(DX);
L59A2:
    /* 59A2  mov     sp,4FA6h */
    SP = 0x4FA6;
L59A5:
    /* 59A5  sti */
    ;
L59A6:
    /* 59A6  mov     dx,bp */
    DX = BP;
L59A8:
    /* 59A8  call    _seg003_2C28 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x2C28), 0x59AB)) != 0) return c;
L59AB:
    /* 59AB  cli */
    ;
L59AC:
    /* 59AC  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L59B1:
    /* 59B1  mov     ss,dx */
    SET_SS(DX);
L59B3:
    /* 59B3  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L59B8:
    /* 59B8  mov     sp,dx */
    SP = DX;
L59BA:
    /* 59BA  sti */
    ;
L59BB:
    /* 59BB  pop     es */
    SET_ES(pop16());
L59BC:
    /* 59BC  pop     ds */
    SET_DS(pop16());
L59BD:
    /* 59BD  pop     si */
    SI = pop16();
L59BE:
    /* 59BE  pop     di */
    DI = pop16();
L59BF:
    /* 59BF  pop     bp */
    BP = pop16();
L59C0:
    /* 59C0  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* vcopy  (+59C1) */
L59C1: /* _vcopy */
    /* 59C1  push    bp */
    push16(BP);
L59C2:
    /* 59C2  mov     bp,sp */
    BP = SP;
L59C4:
    /* 59C4  push    di */
    push16(DI);
L59C5:
    /* 59C5  push    si */
    push16(SI);
L59C6:
    /* 59C6  push    ds */
    push16(asm_ds);
L59C7:
    /* 59C7  push    es */
    push16(asm_es);
L59C8:
    /* 59C8  mov     dx,ss */
    DX = asm_ss;
L59CA:
    /* 59CA  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L59CF:
    /* 59CF  mov     dx,sp */
    DX = SP;
L59D1:
    /* 59D1  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L59D6:
    /* 59D6  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L59D9:
    /* 59D9  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L59DC:
    /* 59DC  mov     cx,word ptr [bp+0Ah] */
    CX = rw(pSS, BP + 0xA);
L59DF:
    /* 59DF  mov     si,word ptr [bp+0Eh] */
    SI = rw(pSS, BP + 0xE);
L59E2:
    /* 59E2  mov     di,word ptr [bp+10h] */
    DI = rw(pSS, BP + 0x10);
L59E5:
    /* 59E5  mov     bp,word ptr [bp+0Ch] */
    BP = rw(pSS, BP + 0xC);
L59E8:
    /* 59E8  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L59EB:
    /* 59EB  mov     ds,dx */
    SET_DS(DX);
L59ED:
    /* 59ED  mov     es,dx */
    SET_ES(DX);
L59EF:
    /* 59EF  cli */
    ;
L59F0:
    /* 59F0  mov     ss,dx */
    SET_SS(DX);
L59F2:
    /* 59F2  mov     sp,4FA6h */
    SP = 0x4FA6;
L59F5:
    /* 59F5  sti */
    ;
L59F6:
    /* 59F6  mov     dx,bp */
    DX = BP;
L59F8:
    /* 59F8  call    _seg003_2C36 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x2C36), 0x59FB)) != 0) return c;
L59FB:
    /* 59FB  cli */
    ;
L59FC:
    /* 59FC  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L5A01:
    /* 5A01  mov     ss,dx */
    SET_SS(DX);
L5A03:
    /* 5A03  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L5A08:
    /* 5A08  mov     sp,dx */
    SP = DX;
L5A0A:
    /* 5A0A  sti */
    ;
L5A0B:
    /* 5A0B  pop     es */
    SET_ES(pop16());
L5A0C:
    /* 5A0C  pop     ds */
    SET_DS(pop16());
L5A0D:
    /* 5A0D  pop     si */
    SI = pop16();
L5A0E:
    /* 5A0E  pop     di */
    DI = pop16();
L5A0F:
    /* 5A0F  pop     bp */
    BP = pop16();
L5A10:
    /* 5A10  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;
L5A11: /* L5A11 */
    /* 5A11  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L5A12:
    /* 5A12  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L5A13:
    /* 5A13  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L5A15:
    /* 5A15  jne     L5A11 */
    if (!ZF) goto L5A11;
L5A17:
    /* 5A17  dec     di */
    DI = dec16(DI);
L5A18:
    /* 5A18  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_5A19  (+5A19)
       _5A19: CX = the length of the string at SI (a strlen). The far code after its ret compares two
       strings with it (repe cmpsb); L5A11, in vcopy's proc, is a far strcpy. No callers of these
       were found. */
L5A19: /* _seg003_5A19 */
    /* 5A19  mov     di,si */
    DI = SI;
L5A1B:
    /* 5A1B  xor     al,al */
    AL = logic8((uint8_t)(AL ^ AL));
L5A1D:
    /* 5A1D  mov     cx,0FFFFh */
    CX = 0xFFFF;
L5A20:
    /* 5A20  repne scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (ZF) break; }
L5A22:
    /* 5A22  mov     cx,di */
    CX = DI;
L5A24:
    /* 5A24  sub     cx,si */
    CX = sub16(CX, SI, 0);
L5A26:
    /* 5A26  dec     cx */
    CX = dec16(CX);
L5A27:
    /* 5A27  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L5A28:
    /* 5A28  push    si */
    push16(SI);
L5A29:
    /* 5A29  push    di */
    push16(DI);
L5A2A:
    /* 5A2A  call    _seg003_5A19 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5A19), 0x5A2D)) != 0) return c;
L5A2D:
    /* 5A2D  pop     di */
    DI = pop16();
L5A2E:
    /* 5A2E  repe cmpsb */
    while (CX) { sub8(rb(pDS, SI), rb(pES, DI), 0); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; if (!ZF) break; }
L5A30:
    /* 5A30  pop     si */
    SI = pop16();
L5A31:
    /* 5A31  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_5A32  (+5A32) */
L5A32: /* _fbuf_setcolor */
    /* 5A32  push    bp */
    push16(BP);
L5A33:
    /* 5A33  mov     bp,sp */
    BP = SP;
L5A35:
    /* 5A35  push    di */
    push16(DI);
L5A36:
    /* 5A36  push    si */
    push16(SI);
L5A37:
    /* 5A37  push    ds */
    push16(asm_ds);
L5A38:
    /* 5A38  push    es */
    push16(asm_es);
L5A39:
    /* 5A39  mov     dx,ss */
    DX = asm_ss;
L5A3B:
    /* 5A3B  mov     word ptr cs:L4C70,dx */
    ww(CODE003, 0x4C70, DX);
L5A40:
    /* 5A40  mov     dx,sp */
    DX = SP;
L5A42:
    /* 5A42  mov     word ptr cs:L4C72,dx */
    ww(CODE003, 0x4C72, DX);
L5A47:
    /* 5A47  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L5A4A:
    /* 5A4A  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L5A4D:
    /* 5A4D  mov     ds,dx */
    SET_DS(DX);
L5A4F:
    /* 5A4F  mov     es,dx */
    SET_ES(DX);
L5A51:
    /* 5A51  cli */
    ;
L5A52:
    /* 5A52  mov     ss,dx */
    SET_SS(DX);
L5A54:
    /* 5A54  mov     sp,4FA6h */
    SP = 0x4FA6;
L5A57:
    /* 5A57  sti */
    ;
L5A58:
    /* 5A58  call    _seg003_EFF */
    if ((c = asm_call(ASM_JMP(0x0090, 0x0EFF), 0x5A5B)) != 0) return c;
L5A5B:
    /* 5A5B  cli */
    ;
L5A5C:
    /* 5A5C  mov     dx,word ptr cs:L4C70 */
    DX = rw(CODE003, 0x4C70);
L5A61:
    /* 5A61  mov     ss,dx */
    SET_SS(DX);
L5A63:
    /* 5A63  mov     dx,word ptr cs:L4C72 */
    DX = rw(CODE003, 0x4C72);
L5A68:
    /* 5A68  mov     sp,dx */
    SP = DX;
L5A6A:
    /* 5A6A  sti */
    ;
L5A6B:
    /* 5A6B  pop     es */
    SET_ES(pop16());
L5A6C:
    /* 5A6C  pop     ds */
    SET_DS(pop16());
L5A6D:
    /* 5A6D  pop     si */
    SI = pop16();
L5A6E:
    /* 5A6E  pop     di */
    DI = pop16();
L5A6F:
    /* 5A6F  pop     bp */
    BP = pop16();
L5A70:
    /* 5A70  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;
}
