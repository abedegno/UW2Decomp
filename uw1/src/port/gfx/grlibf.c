/* grlibf.c: replaces src/gfx/GRLIBF.ASM (seg003_3A86, 3A86..3F05 of its
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

uint32_t asm_mod_GRLIBF(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x3A86: goto L3A86;
    case 0x3A89: goto L3A89;
    case 0x3A8B: goto L3A8B;
    case 0x3A8D: goto L3A8D;
    case 0x3A91: goto L3A91;
    case 0x3A93: goto L3A93;
    case 0x3A97: goto L3A97;
    case 0x3A98: goto L3A98;
    case 0x3A99: goto L3A99;
    case 0x3A9A: goto L3A9A;
    case 0x3A9B: goto L3A9B;
    case 0x3A9F: goto L3A9F;
    case 0x3AA1: goto L3AA1;
    case 0x3AA2: goto L3AA2;
    case 0x3AA3: goto L3AA3;
    case 0x3AA6: goto L3AA6;
    case 0x3AA7: goto L3AA7;
    case 0x3AA8: goto L3AA8;
    case 0x3AAC: goto L3AAC;
    case 0x3AB2: goto L3AB2;
    case 0x3AB5: goto L3AB5;
    case 0x3AB9: goto L3AB9;
    case 0x3ABA: goto L3ABA;
    case 0x3ABE: goto L3ABE;
    case 0x3AC4: goto L3AC4;
    case 0x3AC7: goto L3AC7;
    case 0x3ACB: goto L3ACB;
    case 0x3ACC: goto L3ACC;
    case 0x3AD0: goto L3AD0;
    case 0x3AD6: goto L3AD6;
    case 0x3AD9: goto L3AD9;
    case 0x3ADD: goto L3ADD;
    case 0x3ADE: goto L3ADE;
    case 0x3ADF: goto L3ADF;
    case 0x3AE2: goto L3AE2;
    case 0x3AE3: goto L3AE3;
    case 0x3AE6: goto L3AE6;
    case 0x3AE9: goto L3AE9;
    case 0x3AEA: goto L3AEA;
    case 0x3AED: goto L3AED;
    case 0x3AEF: goto L3AEF;
    case 0x3AF3: goto L3AF3;
    case 0x3AF7: goto L3AF7;
    case 0x3AF9: goto L3AF9;
    case 0x3AFC: goto L3AFC;
    case 0x3AFD: goto L3AFD;
    case 0x3B01: goto L3B01;
    case 0x3B07: goto L3B07;
    case 0x3B0A: goto L3B0A;
    case 0x3B0E: goto L3B0E;
    case 0x3B0F: goto L3B0F;
    case 0x3B12: goto L3B12;
    case 0x3B15: goto L3B15;
    case 0x3B18: goto L3B18;
    case 0x3B19: goto L3B19;
    case 0x3B1A: goto L3B1A;
    case 0x3B1F: goto L3B1F;
    case 0x3B21: goto L3B21;
    case 0x3B23: goto L3B23;
    case 0x3B27: goto L3B27;
    case 0x3B29: goto L3B29;
    case 0x3B2D: goto L3B2D;
    case 0x3B2F: goto L3B2F;
    case 0x3B31: goto L3B31;
    case 0x3B34: goto L3B34;
    case 0x3B38: goto L3B38;
    case 0x3B3A: goto L3B3A;
    case 0x3B3E: goto L3B3E;
    case 0x3B40: goto L3B40;
    case 0x3B44: goto L3B44;
    case 0x3B46: goto L3B46;
    case 0x3B4A: goto L3B4A;
    case 0x3B4C: goto L3B4C;
    case 0x3B52: goto L3B52;
    case 0x3B54: goto L3B54;
    case 0x3B57: goto L3B57;
    case 0x3B59: goto L3B59;
    case 0x3B5A: goto L3B5A;
    case 0x3B5B: goto L3B5B;
    case 0x3B5C: goto L3B5C;
    case 0x3B5D: goto L3B5D;
    case 0x3B5E: goto L3B5E;
    case 0x3B62: goto L3B62;
    case 0x3B66: goto L3B66;
    case 0x3B68: goto L3B68;
    case 0x3B6C: goto L3B6C;
    case 0x3B6E: goto L3B6E;
    case 0x3B70: goto L3B70;
    case 0x3B72: goto L3B72;
    case 0x3B74: goto L3B74;
    case 0x3B78: goto L3B78;
    case 0x3B7A: goto L3B7A;
    case 0x3B7C: goto L3B7C;
    case 0x3B7E: goto L3B7E;
    case 0x3B80: goto L3B80;
    case 0x3B82: goto L3B82;
    case 0x3B86: goto L3B86;
    case 0x3B88: goto L3B88;
    case 0x3B8A: goto L3B8A;
    case 0x3B8C: goto L3B8C;
    case 0x3B8E: goto L3B8E;
    case 0x3B90: goto L3B90;
    case 0x3B91: goto L3B91;
    case 0x3B92: goto L3B92;
    case 0x3B93: goto L3B93;
    case 0x3B96: goto L3B96;
    case 0x3B99: goto L3B99;
    case 0x3B9A: goto L3B9A;
    case 0x3B9D: goto L3B9D;
    case 0x3BA0: goto L3BA0;
    case 0x3BA1: goto L3BA1;
    case 0x3BA4: goto L3BA4;
    case 0x3BA9: goto L3BA9;
    case 0x3BAB: goto L3BAB;
    case 0x3BAF: goto L3BAF;
    case 0x3BB3: goto L3BB3;
    case 0x3BB5: goto L3BB5;
    case 0x3BB8: goto L3BB8;
    case 0x3BBC: goto L3BBC;
    case 0x3BBE: goto L3BBE;
    case 0x3BC1: goto L3BC1;
    case 0x3BC3: goto L3BC3;
    case 0x3BC5: goto L3BC5;
    case 0x3BC7: goto L3BC7;
    case 0x3BC9: goto L3BC9;
    case 0x3BCB: goto L3BCB;
    case 0x3BCC: goto L3BCC;
    case 0x3BCE: goto L3BCE;
    case 0x3BD1: goto L3BD1;
    case 0x3BD3: goto L3BD3;
    case 0x3BD4: goto L3BD4;
    case 0x3BD6: goto L3BD6;
    case 0x3BD7: goto L3BD7;
    case 0x3BD8: goto L3BD8;
    case 0x3BD9: goto L3BD9;
    case 0x3BDB: goto L3BDB;
    case 0x3BDF: goto L3BDF;
    case 0x3BE2: goto L3BE2;
    case 0x3BE6: goto L3BE6;
    case 0x3BE7: goto L3BE7;
    case 0x3BE8: goto L3BE8;
    case 0x3BE9: goto L3BE9;
    case 0x3BEA: goto L3BEA;
    case 0x3BEB: goto L3BEB;
    case 0x3BEC: goto L3BEC;
    case 0x3BED: goto L3BED;
    case 0x3BEE: goto L3BEE;
    case 0x3BF1: goto L3BF1;
    case 0x3BF4: goto L3BF4;
    case 0x3BF5: goto L3BF5;
    case 0x3BF6: goto L3BF6;
    case 0x3BF7: goto L3BF7;
    case 0x3BF8: goto L3BF8;
    case 0x3BF9: goto L3BF9;
    case 0x3BFA: goto L3BFA;
    case 0x3BFB: goto L3BFB;
    case 0x3BFE: goto L3BFE;
    case 0x3C02: goto L3C02;
    case 0x3C06: goto L3C06;
    case 0x3C09: goto L3C09;
    case 0x3C0C: goto L3C0C;
    case 0x3C10: goto L3C10;
    case 0x3C14: goto L3C14;
    case 0x3C17: goto L3C17;
    case 0x3C1A: goto L3C1A;
    case 0x3C1E: goto L3C1E;
    case 0x3C22: goto L3C22;
    case 0x3C25: goto L3C25;
    case 0x3C28: goto L3C28;
    case 0x3C2C: goto L3C2C;
    case 0x3C30: goto L3C30;
    case 0x3C33: goto L3C33;
    case 0x3C34: goto L3C34;
    case 0x3C37: goto L3C37;
    case 0x3C3A: goto L3C3A;
    case 0x3C3C: goto L3C3C;
    case 0x3C3E: goto L3C3E;
    case 0x3C40: goto L3C40;
    case 0x3C42: goto L3C42;
    case 0x3C44: goto L3C44;
    case 0x3C46: goto L3C46;
    case 0x3C48: goto L3C48;
    case 0x3C4A: goto L3C4A;
    case 0x3C4B: goto L3C4B;
    case 0x3C4F: goto L3C4F;
    case 0x3C51: goto L3C51;
    case 0x3C53: goto L3C53;
    case 0x3C55: goto L3C55;
    case 0x3C57: goto L3C57;
    case 0x3C59: goto L3C59;
    case 0x3C5C: goto L3C5C;
    case 0x3C60: goto L3C60;
    case 0x3C62: goto L3C62;
    case 0x3C64: goto L3C64;
    case 0x3C66: goto L3C66;
    case 0x3C68: goto L3C68;
    case 0x3C6A: goto L3C6A;
    case 0x3C6D: goto L3C6D;
    case 0x3C71: goto L3C71;
    case 0x3C73: goto L3C73;
    case 0x3C75: goto L3C75;
    case 0x3C77: goto L3C77;
    case 0x3C79: goto L3C79;
    case 0x3C7B: goto L3C7B;
    case 0x3C7E: goto L3C7E;
    case 0x3C82: goto L3C82;
    case 0x3C84: goto L3C84;
    case 0x3C86: goto L3C86;
    case 0x3C88: goto L3C88;
    case 0x3C8A: goto L3C8A;
    case 0x3C8C: goto L3C8C;
    case 0x3C8F: goto L3C8F;
    case 0x3C92: goto L3C92;
    case 0x3C93: goto L3C93;
    case 0x3C94: goto L3C94;
    case 0x3C95: goto L3C95;
    case 0x3C96: goto L3C96;
    case 0x3C97: goto L3C97;
    case 0x3C98: goto L3C98;
    case 0x3C99: goto L3C99;
    case 0x3C9A: goto L3C9A;
    case 0x3C9B: goto L3C9B;
    case 0x3C9C: goto L3C9C;
    case 0x3C9D: goto L3C9D;
    case 0x3C9E: goto L3C9E;
    case 0x3CA1: goto L3CA1;
    case 0x3CA3: goto L3CA3;
    case 0x3CA6: goto L3CA6;
    case 0x3CA7: goto L3CA7;
    case 0x3CA8: goto L3CA8;
    case 0x3CA9: goto L3CA9;
    case 0x3CAA: goto L3CAA;
    case 0x3CAB: goto L3CAB;
    case 0x3CAC: goto L3CAC;
    case 0x3CAD: goto L3CAD;
    case 0x3CAE: goto L3CAE;
    case 0x3CB0: goto L3CB0;
    case 0x3CB2: goto L3CB2;
    case 0x3CB4: goto L3CB4;
    case 0x3CB6: goto L3CB6;
    case 0x3CB8: goto L3CB8;
    case 0x3CB9: goto L3CB9;
    case 0x3CBC: goto L3CBC;
    case 0x3CC0: goto L3CC0;
    case 0x3CC2: goto L3CC2;
    case 0x3CC4: goto L3CC4;
    case 0x3CC6: goto L3CC6;
    case 0x3CC8: goto L3CC8;
    case 0x3CC9: goto L3CC9;
    case 0x3CCD: goto L3CCD;
    case 0x3CD1: goto L3CD1;
    case 0x3CD4: goto L3CD4;
    case 0x3CD6: goto L3CD6;
    case 0x3CD8: goto L3CD8;
    case 0x3CDC: goto L3CDC;
    case 0x3CDE: goto L3CDE;
    case 0x3CE0: goto L3CE0;
    case 0x3CE2: goto L3CE2;
    case 0x3CE4: goto L3CE4;
    case 0x3CE5: goto L3CE5;
    case 0x3CE8: goto L3CE8;
    case 0x3CEB: goto L3CEB;
    case 0x3CED: goto L3CED;
    case 0x3CEE: goto L3CEE;
    case 0x3CEF: goto L3CEF;
    case 0x3CF1: goto L3CF1;
    case 0x3CF5: goto L3CF5;
    case 0x3CF9: goto L3CF9;
    case 0x3CFA: goto L3CFA;
    case 0x3CFE: goto L3CFE;
    case 0x3CFF: goto L3CFF;
    case 0x3D03: goto L3D03;
    case 0x3D05: goto L3D05;
    case 0x3D09: goto L3D09;
    case 0x3D0B: goto L3D0B;
    case 0x3D0D: goto L3D0D;
    case 0x3D0F: goto L3D0F;
    case 0x3D10: goto L3D10;
    case 0x3D14: goto L3D14;
    case 0x3D16: goto L3D16;
    case 0x3D18: goto L3D18;
    case 0x3D1A: goto L3D1A;
    case 0x3D1C: goto L3D1C;
    case 0x3D1E: goto L3D1E;
    case 0x3D22: goto L3D22;
    case 0x3D24: goto L3D24;
    case 0x3D26: goto L3D26;
    case 0x3D28: goto L3D28;
    case 0x3D2A: goto L3D2A;
    case 0x3D2C: goto L3D2C;
    case 0x3D2E: goto L3D2E;
    case 0x3D30: goto L3D30;
    case 0x3D32: goto L3D32;
    case 0x3D33: goto L3D33;
    case 0x3D36: goto L3D36;
    case 0x3D38: goto L3D38;
    case 0x3D39: goto L3D39;
    case 0x3D3A: goto L3D3A;
    case 0x3D3B: goto L3D3B;
    case 0x3D3C: goto L3D3C;
    case 0x3D3D: goto L3D3D;
    case 0x3D3E: goto L3D3E;
    case 0x3D42: goto L3D42;
    case 0x3D45: goto L3D45;
    case 0x3D47: goto L3D47;
    case 0x3D4A: goto L3D4A;
    case 0x3D4D: goto L3D4D;
    case 0x3D4E: goto L3D4E;
    case 0x3D4F: goto L3D4F;
    case 0x3D50: goto L3D50;
    case 0x3D51: goto L3D51;
    case 0x3D54: goto L3D54;
    case 0x3D56: goto L3D56;
    case 0x3D58: goto L3D58;
    case 0x3D5A: goto L3D5A;
    case 0x3D5C: goto L3D5C;
    case 0x3D5D: goto L3D5D;
    case 0x3D5E: goto L3D5E;
    case 0x3D61: goto L3D61;
    case 0x3D65: goto L3D65;
    case 0x3D69: goto L3D69;
    case 0x3D6C: goto L3D6C;
    case 0x3D6D: goto L3D6D;
    case 0x3D6E: goto L3D6E;
    case 0x3D6F: goto L3D6F;
    case 0x3D70: goto L3D70;
    case 0x3D72: goto L3D72;
    case 0x3D74: goto L3D74;
    case 0x3D76: goto L3D76;
    case 0x3D78: goto L3D78;
    case 0x3D7A: goto L3D7A;
    case 0x3D7C: goto L3D7C;
    case 0x3D7D: goto L3D7D;
    case 0x3D7F: goto L3D7F;
    case 0x3D85: goto L3D85;
    case 0x3D8B: goto L3D8B;
    case 0x3D91: goto L3D91;
    case 0x3D94: goto L3D94;
    case 0x3D97: goto L3D97;
    case 0x3D99: goto L3D99;
    case 0x3D9D: goto L3D9D;
    case 0x3DA0: goto L3DA0;
    case 0x3DA6: goto L3DA6;
    case 0x3DA9: goto L3DA9;
    case 0x3DAC: goto L3DAC;
    case 0x3DAE: goto L3DAE;
    case 0x3DB2: goto L3DB2;
    case 0x3DB5: goto L3DB5;
    case 0x3DB9: goto L3DB9;
    case 0x3DBD: goto L3DBD;
    case 0x3DC0: goto L3DC0;
    case 0x3DC1: goto L3DC1;
    case 0x3DC2: goto L3DC2;
    case 0x3DC3: goto L3DC3;
    case 0x3DC4: goto L3DC4;
    case 0x3DC6: goto L3DC6;
    case 0x3DC8: goto L3DC8;
    case 0x3DCA: goto L3DCA;
    case 0x3DCC: goto L3DCC;
    case 0x3DCE: goto L3DCE;
    case 0x3DD0: goto L3DD0;
    case 0x3DD1: goto L3DD1;
    case 0x3DD3: goto L3DD3;
    case 0x3DD9: goto L3DD9;
    case 0x3DDF: goto L3DDF;
    case 0x3DE5: goto L3DE5;
    case 0x3DE8: goto L3DE8;
    case 0x3DEB: goto L3DEB;
    case 0x3DED: goto L3DED;
    case 0x3DF1: goto L3DF1;
    case 0x3DF4: goto L3DF4;
    case 0x3DFA: goto L3DFA;
    case 0x3DFD: goto L3DFD;
    case 0x3E00: goto L3E00;
    case 0x3E02: goto L3E02;
    case 0x3E06: goto L3E06;
    case 0x3E09: goto L3E09;
    case 0x3E0B: goto L3E0B;
    case 0x3E0C: goto L3E0C;
    case 0x3E0D: goto L3E0D;
    case 0x3E0E: goto L3E0E;
    case 0x3E0F: goto L3E0F;
    case 0x3E11: goto L3E11;
    case 0x3E12: goto L3E12;
    case 0x3E16: goto L3E16;
    case 0x3E1A: goto L3E1A;
    case 0x3E1E: goto L3E1E;
    case 0x3E22: goto L3E22;
    case 0x3E27: goto L3E27;
    case 0x3E29: goto L3E29;
    case 0x3E2D: goto L3E2D;
    case 0x3E2E: goto L3E2E;
    case 0x3E30: goto L3E30;
    case 0x3E32: goto L3E32;
    case 0x3E35: goto L3E35;
    case 0x3E36: goto L3E36;
    case 0x3E37: goto L3E37;
    case 0x3E38: goto L3E38;
    case 0x3E3C: goto L3E3C;
    case 0x3E3E: goto L3E3E;
    case 0x3E40: goto L3E40;
    case 0x3E43: goto L3E43;
    case 0x3E47: goto L3E47;
    case 0x3E4B: goto L3E4B;
    case 0x3E4D: goto L3E4D;
    case 0x3E51: goto L3E51;
    case 0x3E52: goto L3E52;
    case 0x3E53: goto L3E53;
    case 0x3E54: goto L3E54;
    case 0x3E55: goto L3E55;
    case 0x3E56: goto L3E56;
    case 0x3E57: goto L3E57;
    case 0x3E5A: goto L3E5A;
    case 0x3E5E: goto L3E5E;
    case 0x3E64: goto L3E64;
    case 0x3E68: goto L3E68;
    case 0x3E69: goto L3E69;
    case 0x3E6A: goto L3E6A;
    case 0x3E6D: goto L3E6D;
    case 0x3E71: goto L3E71;
    case 0x3E77: goto L3E77;
    case 0x3E7B: goto L3E7B;
    case 0x3E81: goto L3E81;
    case 0x3E85: goto L3E85;
    case 0x3E89: goto L3E89;
    case 0x3E8C: goto L3E8C;
    case 0x3E8D: goto L3E8D;
    case 0x3E91: goto L3E91;
    case 0x3E92: goto L3E92;
    case 0x3E94: goto L3E94;
    case 0x3E95: goto L3E95;
    case 0x3E96: goto L3E96;
    case 0x3E97: goto L3E97;
    case 0x3E98: goto L3E98;
    case 0x3E99: goto L3E99;
    case 0x3E9A: goto L3E9A;
    case 0x3E9B: goto L3E9B;
    case 0x3E9C: goto L3E9C;
    case 0x3E9F: goto L3E9F;
    case 0x3EA1: goto L3EA1;
    case 0x3EA3: goto L3EA3;
    case 0x3EA5: goto L3EA5;
    case 0x3EA7: goto L3EA7;
    case 0x3EA9: goto L3EA9;
    case 0x3EAB: goto L3EAB;
    case 0x3EAD: goto L3EAD;
    case 0x3EAF: goto L3EAF;
    case 0x3EB3: goto L3EB3;
    case 0x3EB5: goto L3EB5;
    case 0x3EB9: goto L3EB9;
    case 0x3EBB: goto L3EBB;
    case 0x3EBF: goto L3EBF;
    case 0x3EC0: goto L3EC0;
    case 0x3EC1: goto L3EC1;
    case 0x3EC2: goto L3EC2;
    case 0x3EC4: goto L3EC4;
    case 0x3EC6: goto L3EC6;
    case 0x3EC8: goto L3EC8;
    case 0x3ECC: goto L3ECC;
    case 0x3ECD: goto L3ECD;
    case 0x3ECE: goto L3ECE;
    case 0x3ECF: goto L3ECF;
    case 0x3ED0: goto L3ED0;
    case 0x3ED1: goto L3ED1;
    case 0x3ED2: goto L3ED2;
    case 0x3ED3: goto L3ED3;
    case 0x3ED4: goto L3ED4;
    case 0x3ED7: goto L3ED7;
    case 0x3ED8: goto L3ED8;
    case 0x3EDA: goto L3EDA;
    case 0x3EDC: goto L3EDC;
    case 0x3EDE: goto L3EDE;
    case 0x3EE0: goto L3EE0;
    case 0x3EE2: goto L3EE2;
    case 0x3EE4: goto L3EE4;
    case 0x3EE6: goto L3EE6;
    case 0x3EE8: goto L3EE8;
    case 0x3EEA: goto L3EEA;
    case 0x3EEE: goto L3EEE;
    case 0x3EF0: goto L3EF0;
    case 0x3EF4: goto L3EF4;
    case 0x3EF6: goto L3EF6;
    case 0x3EFA: goto L3EFA;
    case 0x3EFB: goto L3EFB;
    case 0x3EFC: goto L3EFC;
    case 0x3EFD: goto L3EFD;
    case 0x3EFE: goto L3EFE;
    case 0x3EFF: goto L3EFF;
    case 0x3F01: goto L3F01;
    case 0x3F03: goto L3F03;
    default: asm_bad_entry("GRLIBF.ASM", entry);
    }

    /* seg003_3A86  (+3A86)
       Allocate BX bytes of video memory from the bump pointer (4108). Out: carry clear and AX = the
       block's offset, or carry set when it would pass 4106 or wrap. GRCORE's far wrapper _522E
       returns AX or 0 to C. VALLOC.ASM's pool is what is left after these. */
L3A86: /* _seg003_3A86 */
    /* 3A86  mov     ax,word ptr ds:[4108h] */
    AX = rw(pDS, 0x4108);
L3A89:
    /* 3A89  add     bx,ax */
    BX = add16(BX, AX, 0);
L3A8B:
    /* 3A8B  jb      L3A99 */
    if (CF) goto L3A99;
L3A8D:
    /* 3A8D  cmp     bx,word ptr ds:[4106h] */
    sub16(BX, rw(pDS, 0x4106), 0);
L3A91:
    /* 3A91  jae     L3A99 */
    if (!CF) goto L3A99;
L3A93:
    /* 3A93  mov     word ptr ds:[4108h],bx */
    ww(pDS, 0x4108, BX);
L3A97:
    /* 3A97  clc */
    CF = 0;
L3A98:
    /* 3A98  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3A99: /* L3A99 */
    /* 3A99  stc */
    CF = 1;
L3A9A:
    /* 3A9A  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_3A9B  (+3A9B)
       Give video memory back: set the bump pointer to AX, unless AX is below the floor 4104 (carry
       set). GRCORE's _5274 is the C wrapper.

       The unlabelled code after the ret (and so after this proc's first return) is four small
       routines that nothing in the sources jumps to by name: a clipped line drawn with span writer
       5AC6, the same with 5ACC, an unclipped line with 5AC9, and a page-flipped clear (flip, _3AEA,
       flip back). */
L3A9B: /* _seg003_3A9B */
    /* 3A9B  cmp     ax,word ptr ds:[4104h] */
    sub16(AX, rw(pDS, 0x4104), 0);
L3A9F:
    /* 3A9F  jae     L3AA3 */
    if (!CF) goto L3AA3;
L3AA1:
    /* 3AA1  stc */
    CF = 1;
L3AA2:
    /* 3AA2  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3AA3: /* L3AA3 */
    /* 3AA3  mov     word ptr ds:[4108h],ax */
    ww(pDS, 0x4108, AX);
L3AA6:
    /* 3AA6  clc */
    CF = 0;
L3AA7:
    /* 3AA7  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3AA8:
    /* 3AA8  push    word ptr ds:[SPAN_WRITER] */
    push16(rw(pDS, 0x4110));
L3AAC:
    /* 3AAC  mov     word ptr ds:[SPAN_WRITER],offset _seg003_5AC6 */
    ww(pDS, 0x4110, 0x5AC6);
L3AB2:
    /* 3AB2  call    _seg003_3F0E */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3F0E), 0x3AB5)) != 0) return c;
L3AB5:
    /* 3AB5  pop     word ptr ds:[SPAN_WRITER] */
    { uint16_t t_ = pop16(); ww(pDS, 0x4110, t_); }
L3AB9:
    /* 3AB9  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3ABA:
    /* 3ABA  push    word ptr ds:[SPAN_WRITER] */
    push16(rw(pDS, 0x4110));
L3ABE:
    /* 3ABE  mov     word ptr ds:[SPAN_WRITER],offset _seg003_5ACC */
    ww(pDS, 0x4110, 0x5ACC);
L3AC4:
    /* 3AC4  call    _seg003_3F0E */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3F0E), 0x3AC7)) != 0) return c;
L3AC7:
    /* 3AC7  pop     word ptr ds:[SPAN_WRITER] */
    { uint16_t t_ = pop16(); ww(pDS, 0x4110, t_); }
L3ACB:
    /* 3ACB  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3ACC:
    /* 3ACC  push    word ptr ds:[SPAN_WRITER] */
    push16(rw(pDS, 0x4110));
L3AD0:
    /* 3AD0  mov     word ptr ds:[SPAN_WRITER],offset _seg003_5AC9 */
    ww(pDS, 0x4110, 0x5AC9);
L3AD6:
    /* 3AD6  call    _seg003_3FDC */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3FDC), 0x3AD9)) != 0) return c;
L3AD9:
    /* 3AD9  pop     word ptr ds:[SPAN_WRITER] */
    { uint16_t t_ = pop16(); ww(pDS, 0x4110, t_); }
L3ADD:
    /* 3ADD  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3ADE:
    /* 3ADE  push    ax */
    push16(AX);
L3ADF:
    /* 3ADF  call    _seg003_5A93 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5A93), 0x3AE2)) != 0) return c;
L3AE2:
    /* 3AE2  pop     ax */
    AX = pop16();
L3AE3:
    /* 3AE3  call    _seg003_3AEA */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3AEA), 0x3AE6)) != 0) return c;
L3AE6:
    /* 3AE6  call    _seg003_5A93 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5A93), 0x3AE9)) != 0) return c;
L3AE9:
    /* 3AE9  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_3AEA  (+3AEA)
       _3AEA: set the colour (AX, through set_the_color's vector 5AB1) and fall into _3AED. */
L3AEA: /* _seg003_3AEA */
    /* 3AEA  call    _seg003_5AB1 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5AB1), 0x3AED)) != 0) return c;

    /* seg003_3AED  (+3AED)
       _3AED: fill the whole screen, (0, 0) to (3DEA, 3DEC), with the current span writer, through
       urectangle. */
L3AED: /* _seg003_3AED */
    /* 3AED  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L3AEF:
    /* 3AEF  mov     bx,word ptr ds:[SCR_LASTY] */
    BX = rw(pDS, 0x3DEC);
L3AF3:
    /* 3AF3  mov     cx,word ptr ds:[SCR_LASTX] */
    CX = rw(pDS, 0x3DEA);
L3AF7:
    /* 3AF7  sub     dx,dx */
    DX = (uint16_t)(DX - DX);
L3AF9:
    /* 3AF9  call    _seg003_3CAE */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3CAE), 0x3AFC)) != 0) return c;
L3AFC:
    /* 3AFC  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_3AFD  (+3AFD)
       copy_visible_to_hidden: run _3AED with span writer 5AC6, a whole-screen copy from the visible
       page to the hidden one (GRCORE's copy_visible_to_hidden wrapper). */
L3AFD: /* _seg003_3AFD */
    /* 3AFD  push    word ptr ds:[SPAN_WRITER] */
    push16(rw(pDS, 0x4110));
L3B01:
    /* 3B01  mov     word ptr ds:[SPAN_WRITER],offset _seg003_5AC6 */
    ww(pDS, 0x4110, 0x5AC6);
L3B07:
    /* 3B07  call    _seg003_3AED */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3AED), 0x3B0A)) != 0) return c;
L3B0A:
    /* 3B0A  pop     word ptr ds:[SPAN_WRITER] */
    { uint16_t t_ = pop16(); ww(pDS, 0x4110, t_); }
L3B0E:
    /* 3B0E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_3B0F  (+3B0F)
       copy_hidden_to_visible: flip the pages (5A93, grSoftPageFlip's vector), copy, flip back, so
       the copy runs the other way.

       _3B34, inside this proc, is pixel (FM Towns): plot the point (AX, BX) if it is inside the
       window, as a one-span list through the span writer; with the solid writer (5AC3) it goes
       straight to the pixel writer 5AA2 (upixel), first checking the concave-window table at 5044
       when there is one. GRMISC's far thunk _0 enters here for the 3D view's points. */
L3B0F: /* _seg003_3B0F */
    /* 3B0F  call    _seg003_5A93 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5A93), 0x3B12)) != 0) return c;
L3B12:
    /* 3B12  call    _seg003_3AFD */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3AFD), 0x3B15)) != 0) return c;
L3B15:
    /* 3B15  call    _seg003_5A93 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5A93), 0x3B18)) != 0) return c;
L3B18:
    /* 3B18  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3B19: /* L3B19 */
    /* 3B19  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3B1A: /* L3B1A */
    /* 3B1A  cmp     word ptr ds:[5044h],0 */
    sub16(rw(pDS, 0x5044), 0x0, 0);
L3B1F:
    /* 3B1F  je      L3B31 */
    if (ZF) goto L3B31;
L3B21:
    /* 3B21  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3B23:
    /* 3B23  cmp     ax,word ptr [bx+5044h] */
    sub16(AX, rw(pDS, BX + 0x5044), 0);
L3B27:
    /* 3B27  jge     L3B2F */
    if (SF == OF) goto L3B2F;
L3B29:
    /* 3B29  cmp     ax,word ptr [bx+5044h] */
    sub16(AX, rw(pDS, BX + 0x5044), 0);
L3B2D:
    /* 3B2D  jge     L3B19 */
    if (SF == OF) goto L3B19;
L3B2F: /* L3B2F */
    /* 3B2F  shr     bx,1 */
    BX = shr16(BX, 1);
L3B31: /* L3B31 */
    /* 3B31  jmp     _seg003_5AA2 */
    return ASM_JMP(0x0090, 0x5AA2);
L3B34: /* _seg003_3B34 */
    /* 3B34  cmp     bx,word ptr ds:[WIN_TOP] */
    sub16(BX, rw(pDS, 0x3DF4), 0);
L3B38:
    /* 3B38  jg      L3B19 */
    if (!ZF && SF == OF) goto L3B19;
L3B3A:
    /* 3B3A  cmp     bx,word ptr ds:[WIN_BOTTOM] */
    sub16(BX, rw(pDS, 0x3DF8), 0);
L3B3E:
    /* 3B3E  jl      L3B19 */
    if (SF != OF) goto L3B19;
L3B40:
    /* 3B40  cmp     ax,word ptr ds:[WIN_RIGHT] */
    sub16(AX, rw(pDS, 0x3DF6), 0);
L3B44:
    /* 3B44  jg      L3B19 */
    if (!ZF && SF == OF) goto L3B19;
L3B46:
    /* 3B46  cmp     ax,word ptr ds:[WIN_LEFT] */
    sub16(AX, rw(pDS, 0x3DF2), 0);
L3B4A:
    /* 3B4A  jl      L3B19 */
    if (SF != OF) goto L3B19;
L3B4C:
    /* 3B4C  cmp     word ptr ds:[SPAN_WRITER],offset _seg003_5AC3 */
    sub16(rw(pDS, 0x4110), 0x5AC3, 0);
L3B52:
    /* 3B52  je      L3B1A */
    if (ZF) goto L3B1A;
L3B54:
    /* 3B54  mov     di,4148h */
    DI = 0x4148;
L3B57:
    /* 3B57  mov     si,di */
    SI = DI;
L3B59:
    /* 3B59  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3B5A:
    /* 3B5A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3B5B:
    /* 3B5B  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3B5C:
    /* 3B5C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3B5D:
    /* 3B5D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3B5E:
    /* 3B5E  jmp     word ptr ds:[SPAN_WRITER] */
    return ASM_JMP(0x0090, rw(pDS, 0x4110));

    /* seg003_3B62  (+3B62)
       _3B62: clip a vertical line (x AX, y from BX to DX) to the window. Returns with BX >= DX
       inside the window, or discards its caller's return address (pop ax; ret) so the caller returns
       at once when the line is wholly outside. The code after it is a clipped vertical line through
       vector 5AAB. */
L3B62: /* _seg003_3B62 */
    /* 3B62  cmp     ax,word ptr ds:[WIN_RIGHT] */
    sub16(AX, rw(pDS, 0x3DF6), 0);
L3B66:
    /* 3B66  jg      L3B91 */
    if (!ZF && SF == OF) goto L3B91;
L3B68:
    /* 3B68  cmp     ax,word ptr ds:[WIN_LEFT] */
    sub16(AX, rw(pDS, 0x3DF2), 0);
L3B6C:
    /* 3B6C  jl      L3B91 */
    if (SF != OF) goto L3B91;
L3B6E:
    /* 3B6E  cmp     bx,dx */
    sub16(BX, DX, 0);
L3B70:
    /* 3B70  jg      L3B74 */
    if (!ZF && SF == OF) goto L3B74;
L3B72:
    /* 3B72  xchg    bx,dx */
    { uint16_t t_ = DX;
    DX = BX;
    BX = t_; }
L3B74: /* L3B74 */
    /* 3B74  mov     si,word ptr ds:[WIN_BOTTOM] */
    SI = rw(pDS, 0x3DF8);
L3B78:
    /* 3B78  cmp     bx,si */
    sub16(BX, SI, 0);
L3B7A:
    /* 3B7A  jl      L3B91 */
    if (SF != OF) goto L3B91;
L3B7C:
    /* 3B7C  cmp     dx,si */
    sub16(DX, SI, 0);
L3B7E:
    /* 3B7E  jg      L3B82 */
    if (!ZF && SF == OF) goto L3B82;
L3B80:
    /* 3B80  mov     dx,si */
    DX = SI;
L3B82: /* L3B82 */
    /* 3B82  mov     si,word ptr ds:[WIN_TOP] */
    SI = rw(pDS, 0x3DF4);
L3B86:
    /* 3B86  cmp     dx,si */
    sub16(DX, SI, 0);
L3B88:
    /* 3B88  jg      L3B91 */
    if (!ZF && SF == OF) goto L3B91;
L3B8A:
    /* 3B8A  cmp     bx,si */
    sub16(BX, SI, 0);
L3B8C:
    /* 3B8C  jl      L3B90 */
    if (SF != OF) goto L3B90;
L3B8E:
    /* 3B8E  mov     bx,si */
    BX = SI;
L3B90: /* L3B90 */
    /* 3B90  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3B91: /* L3B91 */
    /* 3B91  pop     ax */
    AX = pop16();
L3B92:
    /* 3B92  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3B93:
    /* 3B93  call    _seg003_3B62 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3B62), 0x3B96)) != 0) return c;
L3B96:
    /* 3B96  call    _seg003_5AAB */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5AAB), 0x3B99)) != 0) return c;
L3B99:
    /* 3B99  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_3B9A  (+3B9A)
       _3B9A: a clipped vertical line through the vector at 5AAE. */
L3B9A: /* _seg003_3B9A */
    /* 3B9A  call    _seg003_3B62 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3B62), 0x3B9D)) != 0) return c;
L3B9D:
    /* 3B9D  call    _seg003_5AAE */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5AAE), 0x3BA0)) != 0) return c;
L3BA0:
    /* 3BA0  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_3BA1  (+3BA1)
       vline: clip (_3B62) and fall into uvline. */
L3BA1: /* _seg003_3BA1 */
    /* 3BA1  call    _seg003_3B62 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3B62), 0x3BA4)) != 0) return c;

    /* seg003_3BA4  (+3BA4)
       uvline: a vertical line at x AX from y BX to DX. The solid and copy writers have direct
       vertical-line routines (5AAB, 5AAE); otherwise it builds one span per row at 2D40 and runs the
       span writer. The code after it loads AX, BX, CX, DX from four words at SI (ubox_si, probably)
       and falls into box. */
L3BA4: /* _seg003_3BA4 */
    /* 3BA4  cmp     word ptr ds:[5044h],0 */
    sub16(rw(pDS, 0x5044), 0x0, 0);
L3BA9:
    /* 3BA9  jne     L3BC1 */
    if (!ZF) goto L3BC1;
L3BAB:
    /* 3BAB  mov     si,word ptr ds:[SPAN_WRITER] */
    SI = rw(pDS, 0x4110);
L3BAF:
    /* 3BAF  cmp     si,offset _seg003_5AC3 */
    sub16(SI, 0x5AC3, 0);
L3BB3:
    /* 3BB3  jne     L3BB8 */
    if (!ZF) goto L3BB8;
L3BB5:
    /* 3BB5  jmp     _seg003_5AAB */
    return ASM_JMP(0x0090, 0x5AAB);
L3BB8: /* L3BB8 */
    /* 3BB8  cmp     si,offset _seg003_5AC6 */
    sub16(SI, 0x5AC6, 0);
L3BBC:
    /* 3BBC  jne     L3BC1 */
    if (!ZF) goto L3BC1;
L3BBE:
    /* 3BBE  jmp     _seg003_5AAE */
    return ASM_JMP(0x0090, 0x5AAE);
L3BC1: /* L3BC1 */
    /* 3BC1  cmp     bx,dx */
    sub16(BX, DX, 0);
L3BC3:
    /* 3BC3  jg      L3BC7 */
    if (!ZF && SF == OF) goto L3BC7;
L3BC5:
    /* 3BC5  xchg    bx,dx */
    { uint16_t t_ = DX;
    DX = BX;
    BX = t_; }
L3BC7: /* L3BC7 */
    /* 3BC7  mov     cx,bx */
    CX = BX;
L3BC9:
    /* 3BC9  sub     cx,dx */
    CX = sub16(CX, DX, 0);
L3BCB:
    /* 3BCB  inc     cx */
    CX = inc16(CX);
L3BCC:
    /* 3BCC  mov     dx,ax */
    DX = AX;
L3BCE:
    /* 3BCE  mov     di,2D40h */
    DI = 0x2D40;
L3BD1: /* L3BD1 */
    /* 3BD1  mov     ax,bx */
    AX = BX;
L3BD3:
    /* 3BD3  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3BD4:
    /* 3BD4  mov     ax,dx */
    AX = DX;
L3BD6:
    /* 3BD6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3BD7:
    /* 3BD7  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3BD8:
    /* 3BD8  dec     bx */
    BX = dec16(BX);
L3BD9:
    /* 3BD9  loop    L3BD1 */
    if (--CX) goto L3BD1;
L3BDB:
    /* 3BDB  mov     word ptr [di],0FFFFh */
    ww(pDS, DI, 0xFFFF);
L3BDF:
    /* 3BDF  mov     si,2D40h */
    SI = 0x2D40;
L3BE2:
    /* 3BE2  jmp     word ptr ds:[SPAN_WRITER] */
    return ASM_JMP(0x0090, rw(pDS, 0x4110));
L3BE6:
    /* 3BE6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3BE7:
    /* 3BE7  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L3BE8:
    /* 3BE8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3BE9:
    /* 3BE9  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3BEA:
    /* 3BEA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3BEB:
    /* 3BEB  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3BEC:
    /* 3BEC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3BED:
    /* 3BED  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }

    /* seg003_3BEE  (+3BEE)
       box: clip the rectangle (clip_rect) and fall into ubox. */
L3BEE: /* _seg003_3BEE */
    /* 3BEE  call    _seg003_3C3E */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3C3E), 0x3BF1)) != 0) return c;

    /* seg003_3BF1  (+3BF1)
       ubox: an outline rectangle with corners (AX, BX) and (CX, DX): two horizontal lines (uhline)
       and two vertical ones (uvline). The code after it is the same with the corners read from SI. */
L3BF1: /* _seg003_3BF1 */
    /* 3BF1  mov     di,4150h */
    DI = 0x4150;
L3BF4:
    /* 3BF4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3BF5:
    /* 3BF5  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3BF6:
    /* 3BF6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3BF7:
    /* 3BF7  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3BF8:
    /* 3BF8  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3BF9:
    /* 3BF9  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L3BFA:
    /* 3BFA  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3BFB: /* L3BFB */
    /* 3BFB  mov     ax,word ptr ds:[4150h] */
    AX = rw(pDS, 0x4150);
L3BFE:
    /* 3BFE  mov     bx,word ptr ds:[4152h] */
    BX = rw(pDS, 0x4152);
L3C02:
    /* 3C02  mov     cx,word ptr ds:[4154h] */
    CX = rw(pDS, 0x4154);
L3C06:
    /* 3C06  call    _seg003_3D2E */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3D2E), 0x3C09)) != 0) return c;
L3C09:
    /* 3C09  mov     ax,word ptr ds:[4150h] */
    AX = rw(pDS, 0x4150);
L3C0C:
    /* 3C0C  mov     bx,word ptr ds:[4156h] */
    BX = rw(pDS, 0x4156);
L3C10:
    /* 3C10  mov     cx,word ptr ds:[4154h] */
    CX = rw(pDS, 0x4154);
L3C14:
    /* 3C14  call    _seg003_3D2E */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3D2E), 0x3C17)) != 0) return c;
L3C17:
    /* 3C17  mov     ax,word ptr ds:[4150h] */
    AX = rw(pDS, 0x4150);
L3C1A:
    /* 3C1A  mov     bx,word ptr ds:[4152h] */
    BX = rw(pDS, 0x4152);
L3C1E:
    /* 3C1E  mov     dx,word ptr ds:[4156h] */
    DX = rw(pDS, 0x4156);
L3C22:
    /* 3C22  call    _seg003_3BA4 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3BA4), 0x3C25)) != 0) return c;
L3C25:
    /* 3C25  mov     ax,word ptr ds:[4154h] */
    AX = rw(pDS, 0x4154);
L3C28:
    /* 3C28  mov     bx,word ptr ds:[4152h] */
    BX = rw(pDS, 0x4152);
L3C2C:
    /* 3C2C  mov     dx,word ptr ds:[4156h] */
    DX = rw(pDS, 0x4156);
L3C30:
    /* 3C30  call    _seg003_3BA4 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3BA4), 0x3C33)) != 0) return c;
L3C33:
    /* 3C33  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3C34:
    /* 3C34  mov     di,4150h */
    DI = 0x4150;
L3C37:
    /* 3C37  mov     cx,4 */
    CX = 0x4;
L3C3A:
    /* 3C3A  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L3C3C:
    /* 3C3C  jmp     L3BFB */
    goto L3BFB;

    /* seg003_3C3E  (+3C3E)
       clip_rect: order and clip the rectangle (AX, BX)-(CX, DX) to the window. Out: AX <= CX, DX <=
       BX (BX the upper row), inside the window; BP = the edges that were cut (8 left, 2 right, 1
       bottom, 4 top); carry set when nothing was cut. A rectangle wholly outside discards the
       caller's return address. _3C96 loads the four words at SI and falls into it. */
L3C3E: /* _seg003_3C3E */
    /* 3C3E  sub     bp,bp */
    BP = (uint16_t)(BP - BP);
L3C40:
    /* 3C40  cmp     bx,dx */
    sub16(BX, DX, 0);
L3C42:
    /* 3C42  jg      L3C46 */
    if (!ZF && SF == OF) goto L3C46;
L3C44:
    /* 3C44  xchg    bx,dx */
    { uint16_t t_ = DX;
    DX = BX;
    BX = t_; }
L3C46: /* L3C46 */
    /* 3C46  cmp     ax,cx */
    sub16(AX, CX, 0);
L3C48:
    /* 3C48  jl      L3C4B */
    if (SF != OF) goto L3C4B;
L3C4A:
    /* 3C4A  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3C4B: /* L3C4B */
    /* 3C4B  mov     si,word ptr ds:[WIN_LEFT] */
    SI = rw(pDS, 0x3DF2);
L3C4F:
    /* 3C4F  cmp     cx,si */
    sub16(CX, SI, 0);
L3C51:
    /* 3C51  jl      L3C94 */
    if (SF != OF) goto L3C94;
L3C53:
    /* 3C53  cmp     ax,si */
    sub16(AX, SI, 0);
L3C55:
    /* 3C55  jge     L3C5C */
    if (SF == OF) goto L3C5C;
L3C57:
    /* 3C57  mov     ax,si */
    AX = SI;
L3C59:
    /* 3C59  add     bp,8 */
    BP = (uint16_t)(BP + 0x8);
L3C5C: /* L3C5C */
    /* 3C5C  mov     si,word ptr ds:[WIN_RIGHT] */
    SI = rw(pDS, 0x3DF6);
L3C60:
    /* 3C60  cmp     ax,si */
    sub16(AX, SI, 0);
L3C62:
    /* 3C62  jg      L3C94 */
    if (!ZF && SF == OF) goto L3C94;
L3C64:
    /* 3C64  cmp     cx,si */
    sub16(CX, SI, 0);
L3C66:
    /* 3C66  jle     L3C6D */
    if (ZF || SF != OF) goto L3C6D;
L3C68:
    /* 3C68  mov     cx,si */
    CX = SI;
L3C6A:
    /* 3C6A  add     bp,2 */
    BP = (uint16_t)(BP + 0x2);
L3C6D: /* L3C6D */
    /* 3C6D  mov     si,word ptr ds:[WIN_BOTTOM] */
    SI = rw(pDS, 0x3DF8);
L3C71:
    /* 3C71  cmp     bx,si */
    sub16(BX, SI, 0);
L3C73:
    /* 3C73  jl      L3C94 */
    if (SF != OF) goto L3C94;
L3C75:
    /* 3C75  cmp     dx,si */
    sub16(DX, SI, 0);
L3C77:
    /* 3C77  jge     L3C7E */
    if (SF == OF) goto L3C7E;
L3C79:
    /* 3C79  mov     dx,si */
    DX = SI;
L3C7B:
    /* 3C7B  add     bp,1 */
    BP = (uint16_t)(BP + 0x1);
L3C7E: /* L3C7E */
    /* 3C7E  mov     si,word ptr ds:[WIN_TOP] */
    SI = rw(pDS, 0x3DF4);
L3C82:
    /* 3C82  cmp     dx,si */
    sub16(DX, SI, 0);
L3C84:
    /* 3C84  jg      L3C94 */
    if (!ZF && SF == OF) goto L3C94;
L3C86:
    /* 3C86  cmp     bx,si */
    sub16(BX, SI, 0);
L3C88:
    /* 3C88  jle     L3C8F */
    if (ZF || SF != OF) goto L3C8F;
L3C8A:
    /* 3C8A  mov     bx,si */
    BX = SI;
L3C8C:
    /* 3C8C  add     bp,4 */
    BP = (uint16_t)(BP + 0x4);
L3C8F: /* L3C8F */
    /* 3C8F  cmp     bp,0 */
    sub16(BP, 0x0, 0);
L3C92:
    /* 3C92  cmc */
    CF = !CF;
L3C93:
    /* 3C93  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3C94: /* L3C94 */
    /* 3C94  pop     ax */
    AX = pop16();
L3C95:
    /* 3C95  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3C96: /* _seg003_3C96 */
    /* 3C96  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3C97:
    /* 3C97  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L3C98:
    /* 3C98  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3C99:
    /* 3C99  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3C9A:
    /* 3C9A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3C9B:
    /* 3C9B  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3C9C:
    /* 3C9C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3C9D:
    /* 3C9D  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }

    /* seg003_3C9E  (+3C9E)
       rectangle: clip_rect, then urectangle. */
L3C9E: /* _seg003_3C9E */
    /* 3C9E  call    _seg003_3C3E */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3C3E), 0x3CA1)) != 0) return c;
L3CA1:
    /* 3CA1  jmp     short _seg003_3CAE */
    goto L3CAE;

    /* seg003_3CA3  (+3CA3)
       clear_window: urectangle over the whole clip window. */
L3CA3: /* _seg003_3CA3 */
    /* 3CA3  mov     si,3DF2h */
    SI = 0x3DF2;
L3CA6:
    /* 3CA6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3CA7:
    /* 3CA7  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L3CA8:
    /* 3CA8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3CA9:
    /* 3CA9  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3CAA:
    /* 3CAA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3CAB:
    /* 3CAB  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3CAC:
    /* 3CAC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3CAD:
    /* 3CAD  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }

    /* seg003_3CAE  (+3CAE)
       urectangle: a filled rectangle (AX, BX)-(CX, DX), corners in either order. Writes one span per
       row into the row table at 44CC (the x range copied into each), moves the end mark, and runs
       the span writer on it. */
L3CAE: /* _seg003_3CAE */
    /* 3CAE  cmp     bx,dx */
    sub16(BX, DX, 0);
L3CB0:
    /* 3CB0  jg      L3CB4 */
    if (!ZF && SF == OF) goto L3CB4;
L3CB2:
    /* 3CB2  xchg    bx,dx */
    { uint16_t t_ = DX;
    DX = BX;
    BX = t_; }
L3CB4: /* L3CB4 */
    /* 3CB4  cmp     ax,cx */
    sub16(AX, CX, 0);
L3CB6:
    /* 3CB6  jl      L3CB9 */
    if (SF != OF) goto L3CB9;
L3CB8:
    /* 3CB8  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3CB9: /* L3CB9 */
    /* 3CB9  mov     word ptr ds:[4158h],ax */
    ww(pDS, 0x4158, AX);
L3CBC:
    /* 3CBC  mov     word ptr ds:[415Ah],cx */
    ww(pDS, 0x415A, CX);
L3CC0:
    /* 3CC0  mov     ax,bx */
    AX = BX;
L3CC2:
    /* 3CC2  mov     cx,dx */
    CX = DX;
L3CC4:
    /* 3CC4  neg     cx */
    CX = (uint16_t)-CX;
L3CC6:
    /* 3CC6  add     cx,ax */
    CX = (uint16_t)(CX + AX);
L3CC8:
    /* 3CC8  inc     cx */
    CX = (uint16_t)(CX + 1);
L3CC9:
    /* 3CC9  mov     si,word ptr ds:[49AAh] */
    SI = rw(pDS, 0x49AA);
L3CCD:
    /* 3CCD  and     word ptr [si],7FFFh */
    ww(pDS, SI, (uint16_t)(rw(pDS, SI) & 0x7FFF));
L3CD1:
    /* 3CD1  mov     di,44CCh */
    DI = 0x44CC;
L3CD4:
    /* 3CD4  mov     si,bx */
    SI = BX;
L3CD6:
    /* 3CD6  neg     si */
    SI = (uint16_t)-SI;
L3CD8:
    /* 3CD8  add     si,0C7h */
    SI = (uint16_t)(SI + 0xC7);
L3CDC:
    /* 3CDC  shl     si,1 */
    SI = (uint16_t)(SI << 1);
L3CDE:
    /* 3CDE  add     di,si */
    DI = (uint16_t)(DI + SI);
L3CE0:
    /* 3CE0  shl     si,1 */
    SI = (uint16_t)(SI << 1);
L3CE2:
    /* 3CE2  add     di,si */
    DI = (uint16_t)(DI + SI);
L3CE4:
    /* 3CE4  push    di */
    push16(DI);
L3CE5:
    /* 3CE5  mov     bx,4158h */
    BX = 0x4158;
L3CE8: /* L3CE8 */
    /* 3CE8  add     di,2 */
    DI = (uint16_t)(DI + 0x2);
L3CEB:
    /* 3CEB  mov     si,bx */
    SI = BX;
L3CED:
    /* 3CED  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3CEE:
    /* 3CEE  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3CEF:
    /* 3CEF  loop    L3CE8 */
    if (--CX) goto L3CE8;
L3CF1:
    /* 3CF1  mov     word ptr ds:[49AAh],di */
    ww(pDS, 0x49AA, DI);
L3CF5:
    /* 3CF5  or      word ptr [di],8000h */
    ww(pDS, DI, logic16((uint16_t)(rw(pDS, DI) | 0x8000)));
L3CF9:
    /* 3CF9  pop     si */
    SI = pop16();
L3CFA:
    /* 3CFA  jmp     word ptr ds:[SPAN_WRITER] */
    return ASM_JMP(0x0090, rw(pDS, 0x4110));
L3CFE: /* L3CFE */
    /* 3CFE  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_3CFF  (+3CFF)
       hline: clip a horizontal line (y BX, x from AX to CX) to the window and draw it as one span;
       returns at once when it is outside. */
L3CFF: /* _seg003_3CFF */
    /* 3CFF  cmp     bx,word ptr ds:[WIN_TOP] */
    sub16(BX, rw(pDS, 0x3DF4), 0);
L3D03:
    /* 3D03  jg      L3CFE */
    if (!ZF && SF == OF) goto L3CFE;
L3D05:
    /* 3D05  cmp     bx,word ptr ds:[WIN_BOTTOM] */
    sub16(BX, rw(pDS, 0x3DF8), 0);
L3D09:
    /* 3D09  jl      L3CFE */
    if (SF != OF) goto L3CFE;
L3D0B:
    /* 3D0B  cmp     ax,cx */
    sub16(AX, CX, 0);
L3D0D:
    /* 3D0D  jl      L3D10 */
    if (SF != OF) goto L3D10;
L3D0F:
    /* 3D0F  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3D10: /* L3D10 */
    /* 3D10  mov     dx,word ptr ds:[WIN_LEFT] */
    DX = rw(pDS, 0x3DF2);
L3D14:
    /* 3D14  cmp     cx,dx */
    sub16(CX, DX, 0);
L3D16:
    /* 3D16  jl      L3CFE */
    if (SF != OF) goto L3CFE;
L3D18:
    /* 3D18  cmp     ax,dx */
    sub16(AX, DX, 0);
L3D1A:
    /* 3D1A  jg      L3D1E */
    if (!ZF && SF == OF) goto L3D1E;
L3D1C:
    /* 3D1C  mov     ax,dx */
    AX = DX;
L3D1E: /* L3D1E */
    /* 3D1E  mov     dx,word ptr ds:[WIN_RIGHT] */
    DX = rw(pDS, 0x3DF6);
L3D22:
    /* 3D22  cmp     ax,dx */
    sub16(AX, DX, 0);
L3D24:
    /* 3D24  jg      L3CFE */
    if (!ZF && SF == OF) goto L3CFE;
L3D26:
    /* 3D26  cmp     cx,dx */
    sub16(CX, DX, 0);
L3D28:
    /* 3D28  jl      L3D2C */
    if (SF != OF) goto L3D2C;
L3D2A:
    /* 3D2A  mov     cx,dx */
    CX = DX;
L3D2C: /* L3D2C */
    /* 3D2C  jmp     short L3D33 */
    goto L3D33;

    /* seg003_3D2E  (+3D2E)
       uhline: one span (y BX, x AX to CX, either order) through the span writer. */
L3D2E: /* _seg003_3D2E */
    /* 3D2E  cmp     ax,cx */
    sub16(AX, CX, 0);
L3D30:
    /* 3D30  jl      L3D33 */
    if (SF != OF) goto L3D33;
L3D32:
    /* 3D32  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3D33: /* L3D33 */
    /* 3D33  mov     di,4148h */
    DI = 0x4148;
L3D36:
    /* 3D36  mov     si,di */
    SI = DI;
L3D38:
    /* 3D38  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3D39:
    /* 3D39  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3D3A:
    /* 3D3A  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3D3B:
    /* 3D3B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3D3C:
    /* 3D3C  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3D3D:
    /* 3D3D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3D3E:
    /* 3D3E  jmp     word ptr ds:[SPAN_WRITER] */
    return ASM_JMP(0x0090, rw(pDS, 0x4110));

    /* seg003_3D42  (+3D42)
       concave_shclip (FM Towns, 7 bytes before shclip there as here): shclip with the handler set at
       4498 in place of 4490. */
L3D42: /* _seg003_3D42 */
    /* 3D42  mov     si,4498h */
    SI = 0x4498;
L3D45:
    /* 3D45  jmp     short L3D4A */
    goto L3D4A;

    /* seg003_3D47  (+3D47)
       shclip: clip a polygon to the window, Sutherland-Hodgman, one edge at a time. In: CX = the
       vertex count, the vertices (x, y words) at 415C. A pass is skipped when every vertex is
       already inside on that axis. Each pass (L3E0F) walks the edges, classifies both ends by
       patching the comparison at L3E30 and L3E3E (7Ch jl or 7Fh jg, followed by a far jump to flush
       the prefetch queue), and calls one of four handlers through 4488 by the in/out case: keep, add
       the intersection, or drop. The intersection (L3E91, L3ECC) is computed with imul/idiv, halving
       both terms on overflow, and nudged one pixel inward unless it lies on the window edge. Out: CX
       = the new count, the vertices back at 415C; when nothing is left it discards the caller's
       return address. */
L3D47: /* _seg003_3D47 */
    /* 3D47  mov     si,4490h */
    SI = 0x4490;
L3D4A: /* L3D4A */
    /* 3D4A  mov     di,4488h */
    DI = 0x4488;
L3D4D:
    /* 3D4D  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3D4E:
    /* 3D4E  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3D4F:
    /* 3D4F  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3D50:
    /* 3D50  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3D51:
    /* 3D51  mov     si,415Ch */
    SI = 0x415C;
L3D54:
    /* 3D54  mov     di,cx */
    DI = CX;
L3D56:
    /* 3D56  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L3D58:
    /* 3D58  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L3D5A:
    /* 3D5A  add     di,si */
    DI = (uint16_t)(DI + SI);
L3D5C:
    /* 3D5C  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3D5D:
    /* 3D5D  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3D5E:
    /* 3D5E  sub     si,4 */
    SI = (uint16_t)(SI - 0x4);
L3D61:
    /* 3D61  mov     bx,word ptr ds:[WIN_TOP] */
    BX = rw(pDS, 0x3DF4);
L3D65:
    /* 3D65  mov     dx,word ptr ds:[WIN_BOTTOM] */
    DX = rw(pDS, 0x3DF8);
L3D69:
    /* 3D69  mov     si,415Eh */
    SI = 0x415E;
L3D6C:
    /* 3D6C  push    cx */
    push16(CX);
L3D6D: /* L3D6D */
    /* 3D6D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3D6E:
    /* 3D6E  inc     si */
    SI = (uint16_t)(SI + 1);
L3D6F:
    /* 3D6F  inc     si */
    SI = (uint16_t)(SI + 1);
L3D70:
    /* 3D70  cmp     ax,bx */
    sub16(AX, BX, 0);
L3D72:
    /* 3D72  jg      L3D7A */
    if (!ZF && SF == OF) goto L3D7A;
L3D74:
    /* 3D74  cmp     ax,dx */
    sub16(AX, DX, 0);
L3D76:
    /* 3D76  jl      L3D7A */
    if (SF != OF) goto L3D7A;
L3D78:
    /* 3D78  loop    L3D6D */
    if (--CX) goto L3D6D;
L3D7A: /* L3D7A */
    /* 3D7A  test    cx,cx */
    logic16((uint16_t)(CX & CX));
L3D7C:
    /* 3D7C  pop     cx */
    CX = pop16();
L3D7D:
    /* 3D7D  je      L3DB5 */
    if (ZF) goto L3DB5;
L3D7F:
    /* 3D7F  mov     word ptr ds:[4480h],2 */
    ww(pDS, 0x4480, 0x2);
L3D85:
    /* 3D85  mov     word ptr ds:[4482h],8 */
    ww(pDS, 0x4482, 0x8);
L3D8B:
    /* 3D8B  mov     word ptr ds:[4484h],offset L3ECC */
    ww(pDS, 0x4484, 0x3ECC);
L3D91:
    /* 3D91  mov     si,415Ch */
    SI = 0x415C;
L3D94:
    /* 3D94  mov     di,42ECh */
    DI = 0x42EC;
L3D97:
    /* 3D97  mov     al,7Ch */
    AL = 0x7C;
L3D99:
    /* 3D99  mov     dx,word ptr ds:[WIN_TOP] */
    DX = rw(pDS, 0x3DF4);
L3D9D:
    /* 3D9D  call    L3E0F */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3E0F), 0x3DA0)) != 0) return c;
L3DA0:
    /* 3DA0  mov     word ptr ds:[4484h],offset L3EC8 */
    ww(pDS, 0x4484, 0x3EC8);
L3DA6:
    /* 3DA6  mov     si,42ECh */
    SI = 0x42EC;
L3DA9:
    /* 3DA9  mov     di,415Ch */
    DI = 0x415C;
L3DAC:
    /* 3DAC  mov     al,7Fh */
    AL = 0x7F;
L3DAE:
    /* 3DAE  mov     dx,word ptr ds:[WIN_BOTTOM] */
    DX = rw(pDS, 0x3DF8);
L3DB2:
    /* 3DB2  call    L3E0F */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3E0F), 0x3DB5)) != 0) return c;
L3DB5: /* L3DB5 */
    /* 3DB5  mov     bx,word ptr ds:[WIN_RIGHT] */
    BX = rw(pDS, 0x3DF6);
L3DB9:
    /* 3DB9  mov     dx,word ptr ds:[WIN_LEFT] */
    DX = rw(pDS, 0x3DF2);
L3DBD:
    /* 3DBD  mov     si,415Ch */
    SI = 0x415C;
L3DC0:
    /* 3DC0  push    cx */
    push16(CX);
L3DC1: /* L3DC1 */
    /* 3DC1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3DC2:
    /* 3DC2  inc     si */
    SI = (uint16_t)(SI + 1);
L3DC3:
    /* 3DC3  inc     si */
    SI = (uint16_t)(SI + 1);
L3DC4:
    /* 3DC4  cmp     ax,bx */
    sub16(AX, BX, 0);
L3DC6:
    /* 3DC6  jg      L3DCE */
    if (!ZF && SF == OF) goto L3DCE;
L3DC8:
    /* 3DC8  cmp     ax,dx */
    sub16(AX, DX, 0);
L3DCA:
    /* 3DCA  jl      L3DCE */
    if (SF != OF) goto L3DCE;
L3DCC:
    /* 3DCC  loop    L3DC1 */
    if (--CX) goto L3DC1;
L3DCE: /* L3DCE */
    /* 3DCE  test    cx,cx */
    logic16((uint16_t)(CX & CX));
L3DD0:
    /* 3DD0  pop     cx */
    CX = pop16();
L3DD1:
    /* 3DD1  je      L3E09 */
    if (ZF) goto L3E09;
L3DD3:
    /* 3DD3  mov     word ptr ds:[4480h],0 */
    ww(pDS, 0x4480, 0x0);
L3DD9:
    /* 3DD9  mov     word ptr ds:[4482h],6 */
    ww(pDS, 0x4482, 0x6);
L3DDF:
    /* 3DDF  mov     word ptr ds:[4484h],offset L3E91 */
    ww(pDS, 0x4484, 0x3E91);
L3DE5:
    /* 3DE5  mov     si,415Ch */
    SI = 0x415C;
L3DE8:
    /* 3DE8  mov     di,42ECh */
    DI = 0x42EC;
L3DEB:
    /* 3DEB  mov     al,7Fh */
    AL = 0x7F;
L3DED:
    /* 3DED  mov     dx,word ptr ds:[WIN_LEFT] */
    DX = rw(pDS, 0x3DF2);
L3DF1:
    /* 3DF1  call    L3E0F */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3E0F), 0x3DF4)) != 0) return c;
L3DF4:
    /* 3DF4  mov     word ptr ds:[4484h],offset L3E8D */
    ww(pDS, 0x4484, 0x3E8D);
L3DFA:
    /* 3DFA  mov     si,42ECh */
    SI = 0x42EC;
L3DFD:
    /* 3DFD  mov     di,415Ch */
    DI = 0x415C;
L3E00:
    /* 3E00  mov     al,7Ch */
    AL = 0x7C;
L3E02:
    /* 3E02  mov     dx,word ptr ds:[WIN_RIGHT] */
    DX = rw(pDS, 0x3DF6);
L3E06:
    /* 3E06  call    L3E0F */
    if ((c = asm_call(ASM_JMP(0x0090, 0x3E0F), 0x3E09)) != 0) return c;
L3E09: /* L3E09 */
    /* 3E09  jcxz    L3E0D */
    if (!CX) goto L3E0D;
L3E0B:
    /* 3E0B  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3E0C: /* L3E0C */
    /* 3E0C  pop     ax */
    AX = pop16();
L3E0D: /* L3E0D */
    /* 3E0D  pop     ax */
    AX = pop16();
L3E0E:
    /* 3E0E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3E0F: /* L3E0F */
    /* 3E0F  jcxz    L3E0C */
    if (!CX) goto L3E0C;
L3E11:
    /* 3E11  push    di */
    push16(DI);
L3E12:
    /* 3E12  mov     byte ptr cs:L3E30,al */
    wb(CODE003, 0x3E30, AL);
L3E16:
    /* 3E16  mov     byte ptr cs:L3E3E,al */
    wb(CODE003, 0x3E3E, AL);
L3E1A:
    /* 3E1A  mov     word ptr ds:[447Eh],cx */
    ww(pDS, 0x447E, CX);
L3E1E:
    /* 3E1E  mov     word ptr ds:[447Ch],cx */
    ww(pDS, 0x447C, CX);
L3E22:
    /* 3E22  db      0EAh */
    return ASM_JMP(0x0090, 0x3E27);
L3E27: /* L3E27 */
    /* 3E27  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3E29:
    /* 3E29  add     si,word ptr ds:[4480h] */
    SI = (uint16_t)(SI + rw(pDS, 0x4480));
L3E2D:
    /* 3E2D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3E2E:
    /* 3E2E  cmp     ax,dx */
    sub16(AX, DX, 0);
L3E30: /* L3E30 */
    /* 3E30  jg      L3E35 */
    if (asm_jcc(CODE003[0x3E30])) goto L3E35;
L3E32:
    /* 3E32  mov     bx,4 */
    BX = 0x4;
L3E35: /* L3E35 */
    /* 3E35  inc     si */
    SI = (uint16_t)(SI + 1);
L3E36:
    /* 3E36  inc     si */
    SI = (uint16_t)(SI + 1);
L3E37:
    /* 3E37  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3E38:
    /* 3E38  sub     si,word ptr ds:[4482h] */
    SI = (uint16_t)(SI - rw(pDS, 0x4482));
L3E3C:
    /* 3E3C  cmp     ax,dx */
    sub16(AX, DX, 0);
L3E3E: /* L3E3E */
    /* 3E3E  jg      L3E43 */
    if (asm_jcc(CODE003[0x3E3E])) goto L3E43;
L3E40:
    /* 3E40  or      bx,2 */
    BX = (uint16_t)(BX | 0x2);
L3E43: /* L3E43 */
    /* 3E43  call    word ptr [bx+4488h] */
    if ((c = asm_call(ASM_JMP(0x0090, rw(pDS, BX + 0x4488)), 0x3E47)) != 0) return c;
L3E47:
    /* 3E47  dec     word ptr ds:[447Eh] */
    ww(pDS, 0x447E, dec16(rw(pDS, 0x447E)));
L3E4B:
    /* 3E4B  jg      L3E27 */
    if (!ZF && SF == OF) goto L3E27;
L3E4D:
    /* 3E4D  mov     cx,word ptr ds:[447Ch] */
    CX = rw(pDS, 0x447C);
L3E51:
    /* 3E51  pop     si */
    SI = pop16();
L3E52:
    /* 3E52  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3E53:
    /* 3E53  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3E54:
    /* 3E54  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3E55:
    /* 3E55  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3E56:
    /* 3E56  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3E57:
    /* 3E57  sub     si,4 */
    SI = sub16(SI, 0x4, 0);
L3E5A:
    /* 3E5A  inc     word ptr ds:[447Ch] */
    ww(pDS, 0x447C, inc16(rw(pDS, 0x447C)));
L3E5E:
    /* 3E5E  mov     word ptr ds:[4486h],0 */
    ww(pDS, 0x4486, 0x0);
L3E64:
    /* 3E64  jmp     word ptr ds:[4484h] */
    return ASM_JMP(0x0090, rw(pDS, 0x4484));
L3E68:
    /* 3E68  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3E69:
    /* 3E69  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L3E6A:
    /* 3E6A  sub     si,4 */
    SI = sub16(SI, 0x4, 0);
L3E6D:
    /* 3E6D  inc     word ptr ds:[447Ch] */
    ww(pDS, 0x447C, inc16(rw(pDS, 0x447C)));
L3E71:
    /* 3E71  mov     word ptr ds:[4486h],0 */
    ww(pDS, 0x4486, 0x0);
L3E77:
    /* 3E77  jmp     word ptr ds:[4484h] */
    return ASM_JMP(0x0090, rw(pDS, 0x4484));
L3E7B:
    /* 3E7B  mov     word ptr ds:[4486h],1 */
    ww(pDS, 0x4486, 0x1);
L3E81:
    /* 3E81  jmp     word ptr ds:[4484h] */
    return ASM_JMP(0x0090, rw(pDS, 0x4484));
L3E85:
    /* 3E85  dec     word ptr ds:[447Ch] */
    ww(pDS, 0x447C, (uint16_t)(rw(pDS, 0x447C) - 1));
L3E89:
    /* 3E89  add     si,4 */
    SI = add16(SI, 0x4, 0);
L3E8C:
    /* 3E8C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3E8D: /* L3E8D */
    /* 3E8D  neg     word ptr ds:[4486h] */
    ww(pDS, 0x4486, (uint16_t)-rw(pDS, 0x4486));
L3E91: /* L3E91 */
    /* 3E91  push    dx */
    push16(DX);
L3E92:
    /* 3E92  mov     ax,dx */
    AX = DX;
L3E94:
    /* 3E94  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3E95:
    /* 3E95  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3E96:
    /* 3E96  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3E97:
    /* 3E97  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3E98:
    /* 3E98  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3E99:
    /* 3E99  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3E9A:
    /* 3E9A  xchg    bp,ax */
    { uint16_t t_ = BP;
    BP = AX;
    AX = t_; }
L3E9B:
    /* 3E9B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3E9C:
    /* 3E9C  sub     si,4 */
    SI = (uint16_t)(SI - 0x4);
L3E9F:
    /* 3E9F  sub     ax,bx */
    AX = (uint16_t)(AX - BX);
L3EA1:
    /* 3EA1  sub     dx,cx */
    DX = (uint16_t)(DX - CX);
L3EA3:
    /* 3EA3  neg     cx */
    CX = (uint16_t)-CX;
L3EA5:
    /* 3EA5  add     cx,bp */
    CX = add16(CX, BP, 0);
L3EA7:
    /* 3EA7  jo      L3EC2 */
    if (OF) goto L3EC2;
L3EA9: /* L3EA9 */
    /* 3EA9  imul    dx */
    imul16(DX);
L3EAB:
    /* 3EAB  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x3EAB, 2)) != 0) return c;
L3EAD:
    /* 3EAD  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L3EAF:
    /* 3EAF  cmp     ax,word ptr ds:[WIN_TOP] */
    sub16(AX, rw(pDS, 0x3DF4), 0);
L3EB3:
    /* 3EB3  jge     L3EBF */
    if (SF == OF) goto L3EBF;
L3EB5:
    /* 3EB5  cmp     ax,word ptr ds:[WIN_BOTTOM] */
    sub16(AX, rw(pDS, 0x3DF8), 0);
L3EB9:
    /* 3EB9  jle     L3EBF */
    if (ZF || SF != OF) goto L3EBF;
L3EBB:
    /* 3EBB  add     ax,word ptr ds:[4486h] */
    AX = add16(AX, rw(pDS, 0x4486), 0);
L3EBF: /* L3EBF */
    /* 3EBF  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3EC0:
    /* 3EC0  pop     dx */
    DX = pop16();
L3EC1:
    /* 3EC1  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3EC2: /* L3EC2 */
    /* 3EC2  rcr     cx,1 */
    CX = rcr16(CX, 1);
L3EC4:
    /* 3EC4  sar     dx,1 */
    DX = (uint16_t)((int16_t)DX >> 1);
L3EC6:
    /* 3EC6  jmp     L3EA9 */
    goto L3EA9;
L3EC8: /* L3EC8 */
    /* 3EC8  neg     word ptr ds:[4486h] */
    ww(pDS, 0x4486, (uint16_t)-rw(pDS, 0x4486));
L3ECC: /* L3ECC */
    /* 3ECC  push    dx */
    push16(DX);
L3ECD:
    /* 3ECD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3ECE:
    /* 3ECE  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3ECF:
    /* 3ECF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3ED0:
    /* 3ED0  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3ED1:
    /* 3ED1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3ED2:
    /* 3ED2  xchg    bp,ax */
    { uint16_t t_ = BP;
    BP = AX;
    AX = t_; }
L3ED3:
    /* 3ED3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3ED4:
    /* 3ED4  sub     si,4 */
    SI = (uint16_t)(SI - 0x4);
L3ED7:
    /* 3ED7  xchg    bp,ax */
    { uint16_t t_ = BP;
    BP = AX;
    AX = t_; }
L3ED8:
    /* 3ED8  xchg    bx,cx */
    { uint16_t t_ = CX;
    CX = BX;
    BX = t_; }
L3EDA:
    /* 3EDA  sub     ax,bx */
    AX = (uint16_t)(AX - BX);
L3EDC:
    /* 3EDC  sub     dx,cx */
    DX = (uint16_t)(DX - CX);
L3EDE:
    /* 3EDE  neg     cx */
    CX = (uint16_t)-CX;
L3EE0:
    /* 3EE0  add     cx,bp */
    CX = add16(CX, BP, 0);
L3EE2:
    /* 3EE2  jo      L3EFF */
    if (OF) goto L3EFF;
L3EE4: /* L3EE4 */
    /* 3EE4  imul    dx */
    imul16(DX);
L3EE6:
    /* 3EE6  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x3EE6, 2)) != 0) return c;
L3EE8:
    /* 3EE8  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L3EEA:
    /* 3EEA  cmp     ax,word ptr ds:[WIN_LEFT] */
    sub16(AX, rw(pDS, 0x3DF2), 0);
L3EEE:
    /* 3EEE  jle     L3EFA */
    if (ZF || SF != OF) goto L3EFA;
L3EF0:
    /* 3EF0  cmp     ax,word ptr ds:[WIN_RIGHT] */
    sub16(AX, rw(pDS, 0x3DF6), 0);
L3EF4:
    /* 3EF4  jge     L3EFA */
    if (SF == OF) goto L3EFA;
L3EF6:
    /* 3EF6  add     ax,word ptr ds:[4486h] */
    AX = add16(AX, rw(pDS, 0x4486), 0);
L3EFA: /* L3EFA */
    /* 3EFA  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3EFB:
    /* 3EFB  pop     ax */
    AX = pop16();
L3EFC:
    /* 3EFC  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3EFD:
    /* 3EFD  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L3EFE:
    /* 3EFE  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3EFF: /* L3EFF */
    /* 3EFF  rcr     cx,1 */
    CX = rcr16(CX, 1);
L3F01:
    /* 3F01  sar     dx,1 */
    DX = (uint16_t)((int16_t)DX >> 1);
L3F03:
    /* 3F03  jmp     L3EE4 */
    goto L3EE4;
}
