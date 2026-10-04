/* grlibl.c: replaces src/gfx/GRLIBL.ASM (seg003_5B6A, 5B6A..6133 of its
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

uint32_t asm_mod_GRLIBL(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x5B6A: goto L5B6A;
    case 0x5B6B: goto L5B6B;
    case 0x5B6C: goto L5B6C;
    case 0x5B6F: goto L5B6F;
    case 0x5B70: goto L5B70;
    case 0x5B74: goto L5B74;
    case 0x5B75: goto L5B75;
    case 0x5B77: goto L5B77;
    case 0x5B79: goto L5B79;
    case 0x5B7B: goto L5B7B;
    case 0x5B7F: goto L5B7F;
    case 0x5B82: goto L5B82;
    case 0x5B83: goto L5B83;
    case 0x5B85: goto L5B85;
    case 0x5B86: goto L5B86;
    case 0x5B89: goto L5B89;
    case 0x5B8A: goto L5B8A;
    case 0x5B8C: goto L5B8C;
    case 0x5B8D: goto L5B8D;
    case 0x5B8E: goto L5B8E;
    case 0x5B92: goto L5B92;
    case 0x5B94: goto L5B94;
    case 0x5B96: goto L5B96;
    case 0x5B9A: goto L5B9A;
    case 0x5B9E: goto L5B9E;
    case 0x5B9F: goto L5B9F;
    case 0x5BA1: goto L5BA1;
    case 0x5BA3: goto L5BA3;
    case 0x5BA8: goto L5BA8;
    case 0x5BAA: goto L5BAA;
    case 0x5BAF: goto L5BAF;
    case 0x5BB1: goto L5BB1;
    case 0x5BB3: goto L5BB3;
    case 0x5BB5: goto L5BB5;
    case 0x5BB6: goto L5BB6;
    case 0x5BB7: goto L5BB7;
    case 0x5BB8: goto L5BB8;
    case 0x5BBA: goto L5BBA;
    case 0x5BBC: goto L5BBC;
    case 0x5BBD: goto L5BBD;
    case 0x5BC0: goto L5BC0;
    case 0x5BC2: goto L5BC2;
    case 0x5BC4: goto L5BC4;
    case 0x5BC6: goto L5BC6;
    case 0x5BC8: goto L5BC8;
    case 0x5BCC: goto L5BCC;
    case 0x5BD0: goto L5BD0;
    case 0x5BD1: goto L5BD1;
    case 0x5BD3: goto L5BD3;
    case 0x5BD5: goto L5BD5;
    case 0x5BDA: goto L5BDA;
    case 0x5BDC: goto L5BDC;
    case 0x5BE1: goto L5BE1;
    case 0x5BE3: goto L5BE3;
    case 0x5BE5: goto L5BE5;
    case 0x5BE7: goto L5BE7;
    case 0x5BE8: goto L5BE8;
    case 0x5BE9: goto L5BE9;
    case 0x5BEA: goto L5BEA;
    case 0x5BEC: goto L5BEC;
    case 0x5BEE: goto L5BEE;
    case 0x5BEF: goto L5BEF;
    case 0x5BF2: goto L5BF2;
    case 0x5BF4: goto L5BF4;
    case 0x5BF6: goto L5BF6;
    case 0x5BF8: goto L5BF8;
    case 0x5BFA: goto L5BFA;
    case 0x5BFE: goto L5BFE;
    case 0x5C02: goto L5C02;
    case 0x5C03: goto L5C03;
    case 0x5C05: goto L5C05;
    case 0x5C07: goto L5C07;
    case 0x5C0C: goto L5C0C;
    case 0x5C0E: goto L5C0E;
    case 0x5C13: goto L5C13;
    case 0x5C15: goto L5C15;
    case 0x5C17: goto L5C17;
    case 0x5C19: goto L5C19;
    case 0x5C1A: goto L5C1A;
    case 0x5C1B: goto L5C1B;
    case 0x5C1C: goto L5C1C;
    case 0x5C1E: goto L5C1E;
    case 0x5C20: goto L5C20;
    case 0x5C21: goto L5C21;
    case 0x5C24: goto L5C24;
    case 0x5C26: goto L5C26;
    case 0x5C28: goto L5C28;
    case 0x5C2A: goto L5C2A;
    case 0x5C2C: goto L5C2C;
    case 0x5C30: goto L5C30;
    case 0x5C34: goto L5C34;
    case 0x5C35: goto L5C35;
    case 0x5C37: goto L5C37;
    case 0x5C39: goto L5C39;
    case 0x5C3E: goto L5C3E;
    case 0x5C40: goto L5C40;
    case 0x5C45: goto L5C45;
    case 0x5C47: goto L5C47;
    case 0x5C49: goto L5C49;
    case 0x5C4B: goto L5C4B;
    case 0x5C4C: goto L5C4C;
    case 0x5C4D: goto L5C4D;
    case 0x5C4E: goto L5C4E;
    case 0x5C50: goto L5C50;
    case 0x5C52: goto L5C52;
    case 0x5C53: goto L5C53;
    case 0x5C56: goto L5C56;
    case 0x5C58: goto L5C58;
    case 0x5C5A: goto L5C5A;
    case 0x5C5B: goto L5C5B;
    case 0x5C5C: goto L5C5C;
    case 0x5C5F: goto L5C5F;
    case 0x5C60: goto L5C60;
    case 0x5C61: goto L5C61;
    case 0x5C64: goto L5C64;
    case 0x5C65: goto L5C65;
    case 0x5C69: goto L5C69;
    case 0x5C6A: goto L5C6A;
    case 0x5C6C: goto L5C6C;
    case 0x5C6E: goto L5C6E;
    case 0x5C70: goto L5C70;
    case 0x5C74: goto L5C74;
    case 0x5C77: goto L5C77;
    case 0x5C78: goto L5C78;
    case 0x5C7A: goto L5C7A;
    case 0x5C7B: goto L5C7B;
    case 0x5C7E: goto L5C7E;
    case 0x5C7F: goto L5C7F;
    case 0x5C81: goto L5C81;
    case 0x5C83: goto L5C83;
    case 0x5C84: goto L5C84;
    case 0x5C85: goto L5C85;
    case 0x5C89: goto L5C89;
    case 0x5C8B: goto L5C8B;
    case 0x5C8D: goto L5C8D;
    case 0x5C91: goto L5C91;
    case 0x5C95: goto L5C95;
    case 0x5C96: goto L5C96;
    case 0x5C98: goto L5C98;
    case 0x5C9A: goto L5C9A;
    case 0x5C9F: goto L5C9F;
    case 0x5CA1: goto L5CA1;
    case 0x5CA6: goto L5CA6;
    case 0x5CA8: goto L5CA8;
    case 0x5CAA: goto L5CAA;
    case 0x5CAC: goto L5CAC;
    case 0x5CAD: goto L5CAD;
    case 0x5CAE: goto L5CAE;
    case 0x5CAF: goto L5CAF;
    case 0x5CB1: goto L5CB1;
    case 0x5CB3: goto L5CB3;
    case 0x5CB4: goto L5CB4;
    case 0x5CB7: goto L5CB7;
    case 0x5CB9: goto L5CB9;
    case 0x5CBB: goto L5CBB;
    case 0x5CBC: goto L5CBC;
    case 0x5CBD: goto L5CBD;
    case 0x5CBE: goto L5CBE;
    case 0x5CC0: goto L5CC0;
    case 0x5CC2: goto L5CC2;
    case 0x5CC4: goto L5CC4;
    case 0x5CC6: goto L5CC6;
    case 0x5CCA: goto L5CCA;
    case 0x5CCE: goto L5CCE;
    case 0x5CCF: goto L5CCF;
    case 0x5CD1: goto L5CD1;
    case 0x5CD3: goto L5CD3;
    case 0x5CD8: goto L5CD8;
    case 0x5CDA: goto L5CDA;
    case 0x5CDF: goto L5CDF;
    case 0x5CE1: goto L5CE1;
    case 0x5CE3: goto L5CE3;
    case 0x5CE5: goto L5CE5;
    case 0x5CE6: goto L5CE6;
    case 0x5CE7: goto L5CE7;
    case 0x5CE8: goto L5CE8;
    case 0x5CEA: goto L5CEA;
    case 0x5CEC: goto L5CEC;
    case 0x5CED: goto L5CED;
    case 0x5CF0: goto L5CF0;
    case 0x5CF2: goto L5CF2;
    case 0x5CF4: goto L5CF4;
    case 0x5CF5: goto L5CF5;
    case 0x5CF6: goto L5CF6;
    case 0x5CF7: goto L5CF7;
    case 0x5CF9: goto L5CF9;
    case 0x5CFB: goto L5CFB;
    case 0x5CFD: goto L5CFD;
    case 0x5CFF: goto L5CFF;
    case 0x5D03: goto L5D03;
    case 0x5D07: goto L5D07;
    case 0x5D08: goto L5D08;
    case 0x5D0A: goto L5D0A;
    case 0x5D0C: goto L5D0C;
    case 0x5D11: goto L5D11;
    case 0x5D13: goto L5D13;
    case 0x5D18: goto L5D18;
    case 0x5D1A: goto L5D1A;
    case 0x5D1C: goto L5D1C;
    case 0x5D1E: goto L5D1E;
    case 0x5D1F: goto L5D1F;
    case 0x5D20: goto L5D20;
    case 0x5D21: goto L5D21;
    case 0x5D23: goto L5D23;
    case 0x5D25: goto L5D25;
    case 0x5D26: goto L5D26;
    case 0x5D29: goto L5D29;
    case 0x5D2B: goto L5D2B;
    case 0x5D2D: goto L5D2D;
    case 0x5D2E: goto L5D2E;
    case 0x5D2F: goto L5D2F;
    case 0x5D30: goto L5D30;
    case 0x5D32: goto L5D32;
    case 0x5D34: goto L5D34;
    case 0x5D36: goto L5D36;
    case 0x5D38: goto L5D38;
    case 0x5D3C: goto L5D3C;
    case 0x5D40: goto L5D40;
    case 0x5D41: goto L5D41;
    case 0x5D43: goto L5D43;
    case 0x5D45: goto L5D45;
    case 0x5D4A: goto L5D4A;
    case 0x5D4C: goto L5D4C;
    case 0x5D51: goto L5D51;
    case 0x5D53: goto L5D53;
    case 0x5D55: goto L5D55;
    case 0x5D57: goto L5D57;
    case 0x5D58: goto L5D58;
    case 0x5D59: goto L5D59;
    case 0x5D5A: goto L5D5A;
    case 0x5D5C: goto L5D5C;
    case 0x5D5E: goto L5D5E;
    case 0x5D5F: goto L5D5F;
    case 0x5D62: goto L5D62;
    case 0x5D64: goto L5D64;
    case 0x5D66: goto L5D66;
    case 0x5D67: goto L5D67;
    case 0x5D68: goto L5D68;
    case 0x5D69: goto L5D69;
    case 0x5D6B: goto L5D6B;
    case 0x5D6D: goto L5D6D;
    case 0x5D6E: goto L5D6E;
    case 0x5D6F: goto L5D6F;
    case 0x5D72: goto L5D72;
    case 0x5D73: goto L5D73;
    case 0x5D77: goto L5D77;
    case 0x5D79: goto L5D79;
    case 0x5D7C: goto L5D7C;
    case 0x5D7E: goto L5D7E;
    case 0x5D80: goto L5D80;
    case 0x5D83: goto L5D83;
    case 0x5D87: goto L5D87;
    case 0x5D8B: goto L5D8B;
    case 0x5D8E: goto L5D8E;
    case 0x5D91: goto L5D91;
    case 0x5D94: goto L5D94;
    case 0x5D97: goto L5D97;
    case 0x5D98: goto L5D98;
    case 0x5D9A: goto L5D9A;
    case 0x5D9C: goto L5D9C;
    case 0x5D9D: goto L5D9D;
    case 0x5DA1: goto L5DA1;
    case 0x5DA3: goto L5DA3;
    case 0x5DA6: goto L5DA6;
    case 0x5DA8: goto L5DA8;
    case 0x5DA9: goto L5DA9;
    case 0x5DAD: goto L5DAD;
    case 0x5DB1: goto L5DB1;
    case 0x5DB4: goto L5DB4;
    case 0x5DB5: goto L5DB5;
    case 0x5DB7: goto L5DB7;
    case 0x5DBA: goto L5DBA;
    case 0x5DBB: goto L5DBB;
    case 0x5DBF: goto L5DBF;
    case 0x5DC2: goto L5DC2;
    case 0x5DC5: goto L5DC5;
    case 0x5DC7: goto L5DC7;
    case 0x5DC9: goto L5DC9;
    case 0x5DCC: goto L5DCC;
    case 0x5DD0: goto L5DD0;
    case 0x5DD3: goto L5DD3;
    case 0x5DD4: goto L5DD4;
    case 0x5DD6: goto L5DD6;
    case 0x5DDA: goto L5DDA;
    case 0x5DDC: goto L5DDC;
    case 0x5DDF: goto L5DDF;
    case 0x5DE3: goto L5DE3;
    case 0x5DE7: goto L5DE7;
    case 0x5DEA: goto L5DEA;
    case 0x5DEB: goto L5DEB;
    case 0x5DED: goto L5DED;
    case 0x5DF0: goto L5DF0;
    case 0x5DF1: goto L5DF1;
    case 0x5DF5: goto L5DF5;
    case 0x5DF8: goto L5DF8;
    case 0x5DFB: goto L5DFB;
    case 0x5DFD: goto L5DFD;
    case 0x5DFF: goto L5DFF;
    case 0x5E02: goto L5E02;
    case 0x5E06: goto L5E06;
    case 0x5E09: goto L5E09;
    case 0x5E0A: goto L5E0A;
    case 0x5E0C: goto L5E0C;
    case 0x5E10: goto L5E10;
    case 0x5E12: goto L5E12;
    case 0x5E16: goto L5E16;
    case 0x5E1A: goto L5E1A;
    case 0x5E1D: goto L5E1D;
    case 0x5E1E: goto L5E1E;
    case 0x5E20: goto L5E20;
    case 0x5E23: goto L5E23;
    case 0x5E24: goto L5E24;
    case 0x5E28: goto L5E28;
    case 0x5E2B: goto L5E2B;
    case 0x5E2E: goto L5E2E;
    case 0x5E30: goto L5E30;
    case 0x5E32: goto L5E32;
    case 0x5E35: goto L5E35;
    case 0x5E39: goto L5E39;
    case 0x5E3C: goto L5E3C;
    case 0x5E3D: goto L5E3D;
    case 0x5E3F: goto L5E3F;
    case 0x5E43: goto L5E43;
    case 0x5E45: goto L5E45;
    case 0x5E49: goto L5E49;
    case 0x5E4D: goto L5E4D;
    case 0x5E50: goto L5E50;
    case 0x5E51: goto L5E51;
    case 0x5E53: goto L5E53;
    case 0x5E56: goto L5E56;
    case 0x5E57: goto L5E57;
    case 0x5E5B: goto L5E5B;
    case 0x5E5E: goto L5E5E;
    case 0x5E61: goto L5E61;
    case 0x5E63: goto L5E63;
    case 0x5E65: goto L5E65;
    case 0x5E68: goto L5E68;
    case 0x5E6C: goto L5E6C;
    case 0x5E6F: goto L5E6F;
    case 0x5E70: goto L5E70;
    case 0x5E72: goto L5E72;
    case 0x5E76: goto L5E76;
    case 0x5E78: goto L5E78;
    case 0x5E79: goto L5E79;
    case 0x5E7C: goto L5E7C;
    case 0x5E7F: goto L5E7F;
    case 0x5E82: goto L5E82;
    case 0x5E83: goto L5E83;
    case 0x5E84: goto L5E84;
    case 0x5E85: goto L5E85;
    case 0x5E88: goto L5E88;
    case 0x5E8A: goto L5E8A;
    case 0x5E8B: goto L5E8B;
    case 0x5E8E: goto L5E8E;
    case 0x5E8F: goto L5E8F;
    case 0x5E93: goto L5E93;
    case 0x5E95: goto L5E95;
    case 0x5E97: goto L5E97;
    case 0x5E99: goto L5E99;
    case 0x5E9B: goto L5E9B;
    case 0x5E9F: goto L5E9F;
    case 0x5EA2: goto L5EA2;
    case 0x5EA5: goto L5EA5;
    case 0x5EA7: goto L5EA7;
    case 0x5EAA: goto L5EAA;
    case 0x5EAD: goto L5EAD;
    case 0x5EAF: goto L5EAF;
    case 0x5EB0: goto L5EB0;
    case 0x5EB3: goto L5EB3;
    case 0x5EB5: goto L5EB5;
    case 0x5EB7: goto L5EB7;
    case 0x5EBA: goto L5EBA;
    case 0x5EBC: goto L5EBC;
    case 0x5EBE: goto L5EBE;
    case 0x5EBF: goto L5EBF;
    case 0x5EC2: goto L5EC2;
    case 0x5EC4: goto L5EC4;
    case 0x5EC7: goto L5EC7;
    case 0x5ECA: goto L5ECA;
    case 0x5ECC: goto L5ECC;
    case 0x5ED0: goto L5ED0;
    case 0x5ED1: goto L5ED1;
    case 0x5ED3: goto L5ED3;
    case 0x5ED5: goto L5ED5;
    case 0x5ED6: goto L5ED6;
    case 0x5ED9: goto L5ED9;
    case 0x5EDB: goto L5EDB;
    case 0x5EDC: goto L5EDC;
    case 0x5EDD: goto L5EDD;
    case 0x5EDE: goto L5EDE;
    case 0x5EE2: goto L5EE2;
    case 0x5EE3: goto L5EE3;
    case 0x5EE5: goto L5EE5;
    case 0x5EE7: goto L5EE7;
    case 0x5EEA: goto L5EEA;
    case 0x5EEC: goto L5EEC;
    case 0x5EF0: goto L5EF0;
    case 0x5EF4: goto L5EF4;
    case 0x5EF5: goto L5EF5;
    case 0x5EF7: goto L5EF7;
    case 0x5EF8: goto L5EF8;
    case 0x5EFA: goto L5EFA;
    case 0x5EFB: goto L5EFB;
    case 0x5EFD: goto L5EFD;
    case 0x5EFF: goto L5EFF;
    case 0x5F00: goto L5F00;
    case 0x5F04: goto L5F04;
    case 0x5F05: goto L5F05;
    case 0x5F07: goto L5F07;
    case 0x5F0A: goto L5F0A;
    case 0x5F0C: goto L5F0C;
    case 0x5F0D: goto L5F0D;
    case 0x5F11: goto L5F11;
    case 0x5F15: goto L5F15;
    case 0x5F18: goto L5F18;
    case 0x5F19: goto L5F19;
    case 0x5F1B: goto L5F1B;
    case 0x5F1E: goto L5F1E;
    case 0x5F1F: goto L5F1F;
    case 0x5F23: goto L5F23;
    case 0x5F26: goto L5F26;
    case 0x5F29: goto L5F29;
    case 0x5F2B: goto L5F2B;
    case 0x5F2D: goto L5F2D;
    case 0x5F30: goto L5F30;
    case 0x5F34: goto L5F34;
    case 0x5F36: goto L5F36;
    case 0x5F38: goto L5F38;
    case 0x5F3A: goto L5F3A;
    case 0x5F3B: goto L5F3B;
    case 0x5F3C: goto L5F3C;
    case 0x5F3D: goto L5F3D;
    case 0x5F3F: goto L5F3F;
    case 0x5F40: goto L5F40;
    case 0x5F42: goto L5F42;
    case 0x5F45: goto L5F45;
    case 0x5F49: goto L5F49;
    case 0x5F4B: goto L5F4B;
    case 0x5F4E: goto L5F4E;
    case 0x5F52: goto L5F52;
    case 0x5F56: goto L5F56;
    case 0x5F59: goto L5F59;
    case 0x5F5A: goto L5F5A;
    case 0x5F5C: goto L5F5C;
    case 0x5F5F: goto L5F5F;
    case 0x5F60: goto L5F60;
    case 0x5F64: goto L5F64;
    case 0x5F67: goto L5F67;
    case 0x5F6A: goto L5F6A;
    case 0x5F6C: goto L5F6C;
    case 0x5F6E: goto L5F6E;
    case 0x5F71: goto L5F71;
    case 0x5F75: goto L5F75;
    case 0x5F77: goto L5F77;
    case 0x5F79: goto L5F79;
    case 0x5F7B: goto L5F7B;
    case 0x5F7C: goto L5F7C;
    case 0x5F7D: goto L5F7D;
    case 0x5F7E: goto L5F7E;
    case 0x5F80: goto L5F80;
    case 0x5F81: goto L5F81;
    case 0x5F83: goto L5F83;
    case 0x5F86: goto L5F86;
    case 0x5F8A: goto L5F8A;
    case 0x5F8C: goto L5F8C;
    case 0x5F90: goto L5F90;
    case 0x5F94: goto L5F94;
    case 0x5F97: goto L5F97;
    case 0x5F98: goto L5F98;
    case 0x5F9A: goto L5F9A;
    case 0x5F9D: goto L5F9D;
    case 0x5F9E: goto L5F9E;
    case 0x5FA2: goto L5FA2;
    case 0x5FA5: goto L5FA5;
    case 0x5FA8: goto L5FA8;
    case 0x5FAA: goto L5FAA;
    case 0x5FAC: goto L5FAC;
    case 0x5FAF: goto L5FAF;
    case 0x5FB3: goto L5FB3;
    case 0x5FB5: goto L5FB5;
    case 0x5FB7: goto L5FB7;
    case 0x5FB9: goto L5FB9;
    case 0x5FBA: goto L5FBA;
    case 0x5FBB: goto L5FBB;
    case 0x5FBC: goto L5FBC;
    case 0x5FBE: goto L5FBE;
    case 0x5FBF: goto L5FBF;
    case 0x5FC1: goto L5FC1;
    case 0x5FC4: goto L5FC4;
    case 0x5FC8: goto L5FC8;
    case 0x5FCA: goto L5FCA;
    case 0x5FCE: goto L5FCE;
    case 0x5FD2: goto L5FD2;
    case 0x5FD5: goto L5FD5;
    case 0x5FD6: goto L5FD6;
    case 0x5FD8: goto L5FD8;
    case 0x5FDB: goto L5FDB;
    case 0x5FDC: goto L5FDC;
    case 0x5FE0: goto L5FE0;
    case 0x5FE3: goto L5FE3;
    case 0x5FE6: goto L5FE6;
    case 0x5FE8: goto L5FE8;
    case 0x5FEA: goto L5FEA;
    case 0x5FED: goto L5FED;
    case 0x5FF1: goto L5FF1;
    case 0x5FF3: goto L5FF3;
    case 0x5FF5: goto L5FF5;
    case 0x5FF7: goto L5FF7;
    case 0x5FF8: goto L5FF8;
    case 0x5FF9: goto L5FF9;
    case 0x5FFA: goto L5FFA;
    case 0x5FFC: goto L5FFC;
    case 0x5FFD: goto L5FFD;
    case 0x5FFF: goto L5FFF;
    case 0x6002: goto L6002;
    case 0x6006: goto L6006;
    case 0x6008: goto L6008;
    case 0x6009: goto L6009;
    case 0x600C: goto L600C;
    case 0x600F: goto L600F;
    case 0x6012: goto L6012;
    case 0x6013: goto L6013;
    case 0x6014: goto L6014;
    case 0x6015: goto L6015;
    case 0x6018: goto L6018;
    case 0x601A: goto L601A;
    case 0x601B: goto L601B;
    case 0x601E: goto L601E;
    case 0x601F: goto L601F;
    case 0x6023: goto L6023;
    case 0x6025: goto L6025;
    case 0x6027: goto L6027;
    case 0x6029: goto L6029;
    case 0x602B: goto L602B;
    case 0x602F: goto L602F;
    case 0x6032: goto L6032;
    case 0x6035: goto L6035;
    case 0x6037: goto L6037;
    case 0x603A: goto L603A;
    case 0x603D: goto L603D;
    case 0x603F: goto L603F;
    case 0x6040: goto L6040;
    case 0x6043: goto L6043;
    case 0x6045: goto L6045;
    case 0x6047: goto L6047;
    case 0x604A: goto L604A;
    case 0x604C: goto L604C;
    case 0x604E: goto L604E;
    case 0x604F: goto L604F;
    case 0x6052: goto L6052;
    case 0x6054: goto L6054;
    case 0x6057: goto L6057;
    case 0x605A: goto L605A;
    case 0x605C: goto L605C;
    case 0x6060: goto L6060;
    case 0x6061: goto L6061;
    case 0x6063: goto L6063;
    case 0x6065: goto L6065;
    case 0x6066: goto L6066;
    case 0x6069: goto L6069;
    case 0x606B: goto L606B;
    case 0x606C: goto L606C;
    case 0x606D: goto L606D;
    case 0x6070: goto L6070;
    case 0x6072: goto L6072;
    case 0x6073: goto L6073;
    case 0x6076: goto L6076;
    case 0x6077: goto L6077;
    case 0x607B: goto L607B;
    case 0x607D: goto L607D;
    case 0x607E: goto L607E;
    case 0x6080: goto L6080;
    case 0x6082: goto L6082;
    case 0x6084: goto L6084;
    case 0x6088: goto L6088;
    case 0x608B: goto L608B;
    case 0x608E: goto L608E;
    case 0x6091: goto L6091;
    case 0x6094: goto L6094;
    case 0x6096: goto L6096;
    case 0x6097: goto L6097;
    case 0x6099: goto L6099;
    case 0x609C: goto L609C;
    case 0x609E: goto L609E;
    case 0x60A1: goto L60A1;
    case 0x60A2: goto L60A2;
    case 0x60A4: goto L60A4;
    case 0x60A6: goto L60A6;
    case 0x60AA: goto L60AA;
    case 0x60AC: goto L60AC;
    case 0x60B0: goto L60B0;
    case 0x60B1: goto L60B1;
    case 0x60B3: goto L60B3;
    case 0x60B5: goto L60B5;
    case 0x60B6: goto L60B6;
    case 0x60B8: goto L60B8;
    case 0x60B9: goto L60B9;
    case 0x60BC: goto L60BC;
    case 0x60BE: goto L60BE;
    case 0x60BF: goto L60BF;
    case 0x60C0: goto L60C0;
    case 0x60C3: goto L60C3;
    case 0x60C5: goto L60C5;
    case 0x60C6: goto L60C6;
    case 0x60C7: goto L60C7;
    case 0x60C8: goto L60C8;
    case 0x60C9: goto L60C9;
    case 0x60CE: goto L60CE;
    case 0x60D3: goto L60D3;
    case 0x60D5: goto L60D5;
    case 0x60D7: goto L60D7;
    case 0x60D9: goto L60D9;
    case 0x60DB: goto L60DB;
    case 0x60E0: goto L60E0;
    case 0x60E2: goto L60E2;
    case 0x60E4: goto L60E4;
    case 0x60E6: goto L60E6;
    case 0x60E8: goto L60E8;
    case 0x60EA: goto L60EA;
    case 0x60EB: goto L60EB;
    case 0x60ED: goto L60ED;
    case 0x60EF: goto L60EF;
    case 0x60F0: goto L60F0;
    case 0x60F2: goto L60F2;
    case 0x60F3: goto L60F3;
    case 0x60F5: goto L60F5;
    case 0x60F6: goto L60F6;
    case 0x60F7: goto L60F7;
    case 0x60F8: goto L60F8;
    case 0x60F9: goto L60F9;
    case 0x60FA: goto L60FA;
    case 0x60FF: goto L60FF;
    case 0x6104: goto L6104;
    case 0x6106: goto L6106;
    case 0x6108: goto L6108;
    case 0x610A: goto L610A;
    case 0x610C: goto L610C;
    case 0x6111: goto L6111;
    case 0x6113: goto L6113;
    case 0x6115: goto L6115;
    case 0x6117: goto L6117;
    case 0x6119: goto L6119;
    case 0x611B: goto L611B;
    case 0x611C: goto L611C;
    case 0x611E: goto L611E;
    case 0x6120: goto L6120;
    case 0x6121: goto L6121;
    case 0x6123: goto L6123;
    case 0x6124: goto L6124;
    case 0x6126: goto L6126;
    case 0x6128: goto L6128;
    case 0x6129: goto L6129;
    case 0x612A: goto L612A;
    case 0x612B: goto L612B;
    case 0x612D: goto L612D;
    case 0x612E: goto L612E;
    case 0x6130: goto L6130;
    case 0x6131: goto L6131;
    case 0x6132: goto L6132;
    default: asm_bad_entry("GRLIBL.ASM", entry);
    }
L5B6A: /* L5B6A */
    /* 5B6A  pop     es */
    SET_ES(pop16());
L5B6B:
    /* 5B6B  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* _5B6C: linear bitmap rows to the planar screen, opaque. For each row, four passes, one per
       plane: set the map mask for the plane, then copy every fourth source byte. */
L5B6C: /* _seg003_5B6C */
    /* 5B6C  mov     dx,SC_DATA */
    DX = 0x3C5;
L5B6F:
    /* 5B6F  push    es */
    push16(asm_es);
L5B70:
    /* 5B70  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L5B74: /* L5B74 */
    /* 5B74  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5B75:
    /* 5B75  shl     ax,1 */
    AX = shl16(AX, 1);
L5B77:
    /* 5B77  jb      L5B6A */
    if (CF) goto L5B6A;
L5B79:
    /* 5B79  mov     bp,ax */
    BP = AX;
L5B7B:
    /* 5B7B  mov     ax,word ptr [bp+YTAB] */
    AX = rw(pSS, BP + 0x36AA);
L5B7F:
    /* 5B7F  mov     word ptr ds:[55E0h],ax */
    ww(pDS, 0x55E0, AX);
L5B82:
    /* 5B82  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5B83:
    /* 5B83  mov     bx,ax */
    BX = AX;
L5B85:
    /* 5B85  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5B86:
    /* 5B86  mov     word ptr ds:[55E4h],ax */
    ww(pDS, 0x55E4, AX);
L5B89:
    /* 5B89  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5B8A:
    /* 5B8A  mov     cx,ax */
    CX = AX;
L5B8C:
    /* 5B8C  push    si */
    push16(SI);
L5B8D:
    /* 5B8D  push    ds */
    push16(asm_ds);
L5B8E:
    /* 5B8E  mov     ds,word ptr ds:[55EAh] */
    SET_DS(rw(pDS, 0x55EA));
L5B92:
    /* 5B92  mov     bp,bx */
    BP = BX;
L5B94:
    /* 5B94  mov     di,bp */
    DI = BP;
L5B96:
    /* 5B96  mov     al,byte ptr [bp+LMASKS] */
    AL = rb(pSS, BP + 0x3B62);
L5B9A:
    /* 5B9A  and     al,byte ptr [bp+RMASKS] */
    AL = (uint8_t)(AL & rb(pSS, BP + 0x3CA6));
L5B9E:
    /* 5B9E  out     dx,al */
    asm_out8(DX, AL);
L5B9F:
    /* 5B9F  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L5BA1:
    /* 5BA1  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L5BA3:
    /* 5BA3  add     di,word ptr ss:[55E0h] */
    DI = (uint16_t)(DI + rw(pSS, 0x55E0));
L5BA8:
    /* 5BA8  neg     bp */
    BP = (uint16_t)-BP;
L5BAA:
    /* 5BAA  add     bp,word ptr ss:[55E4h] */
    BP = (uint16_t)(BP + rw(pSS, 0x55E4));
L5BAF:
    /* 5BAF  sar     bp,1 */
    BP = (uint16_t)((int16_t)BP >> 1);
L5BB1:
    /* 5BB1  sar     bp,1 */
    BP = (uint16_t)((int16_t)BP >> 1);
L5BB3:
    /* 5BB3  mov     si,cx */
    SI = CX;
L5BB5:
    /* 5BB5  inc     bx */
    BX = (uint16_t)(BX + 1);
L5BB6:
    /* 5BB6  inc     cx */
    CX = (uint16_t)(CX + 1);
L5BB7:
    /* 5BB7  inc     bp */
    BP = inc16(BP);
L5BB8:
    /* 5BB8  je      L5BC4 */
    if (ZF) goto L5BC4;
L5BBA:
    /* 5BBA  xchg    bp,cx */
    { uint16_t t_ = CX;
    CX = BP;
    BP = t_; }
L5BBC: /* L5BBC */
    /* 5BBC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L5BBD:
    /* 5BBD  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L5BC0:
    /* 5BC0  loop    L5BBC */
    if (--CX) goto L5BBC;
L5BC2:
    /* 5BC2  xchg    bp,cx */
    { uint16_t t_ = CX;
    CX = BP;
    BP = t_; }
L5BC4: /* L5BC4 */
    /* 5BC4  mov     bp,bx */
    BP = BX;
L5BC6:
    /* 5BC6  mov     di,bp */
    DI = BP;
L5BC8:
    /* 5BC8  mov     al,byte ptr [bp+LMASKS] */
    AL = rb(pSS, BP + 0x3B62);
L5BCC:
    /* 5BCC  and     al,byte ptr [bp+RMASKS] */
    AL = (uint8_t)(AL & rb(pSS, BP + 0x3CA6));
L5BD0:
    /* 5BD0  out     dx,al */
    asm_out8(DX, AL);
L5BD1:
    /* 5BD1  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L5BD3:
    /* 5BD3  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L5BD5:
    /* 5BD5  add     di,word ptr ss:[55E0h] */
    DI = (uint16_t)(DI + rw(pSS, 0x55E0));
L5BDA:
    /* 5BDA  neg     bp */
    BP = (uint16_t)-BP;
L5BDC:
    /* 5BDC  add     bp,word ptr ss:[55E4h] */
    BP = (uint16_t)(BP + rw(pSS, 0x55E4));
L5BE1:
    /* 5BE1  sar     bp,1 */
    BP = (uint16_t)((int16_t)BP >> 1);
L5BE3:
    /* 5BE3  sar     bp,1 */
    BP = (uint16_t)((int16_t)BP >> 1);
L5BE5:
    /* 5BE5  mov     si,cx */
    SI = CX;
L5BE7:
    /* 5BE7  inc     bx */
    BX = (uint16_t)(BX + 1);
L5BE8:
    /* 5BE8  inc     cx */
    CX = (uint16_t)(CX + 1);
L5BE9:
    /* 5BE9  inc     bp */
    BP = inc16(BP);
L5BEA:
    /* 5BEA  je      L5BF6 */
    if (ZF) goto L5BF6;
L5BEC:
    /* 5BEC  xchg    bp,cx */
    { uint16_t t_ = CX;
    CX = BP;
    BP = t_; }
L5BEE: /* L5BEE */
    /* 5BEE  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L5BEF:
    /* 5BEF  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L5BF2:
    /* 5BF2  loop    L5BEE */
    if (--CX) goto L5BEE;
L5BF4:
    /* 5BF4  xchg    bp,cx */
    { uint16_t t_ = CX;
    CX = BP;
    BP = t_; }
L5BF6: /* L5BF6 */
    /* 5BF6  mov     bp,bx */
    BP = BX;
L5BF8:
    /* 5BF8  mov     di,bp */
    DI = BP;
L5BFA:
    /* 5BFA  mov     al,byte ptr [bp+LMASKS] */
    AL = rb(pSS, BP + 0x3B62);
L5BFE:
    /* 5BFE  and     al,byte ptr [bp+RMASKS] */
    AL = (uint8_t)(AL & rb(pSS, BP + 0x3CA6));
L5C02:
    /* 5C02  out     dx,al */
    asm_out8(DX, AL);
L5C03:
    /* 5C03  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L5C05:
    /* 5C05  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L5C07:
    /* 5C07  add     di,word ptr ss:[55E0h] */
    DI = (uint16_t)(DI + rw(pSS, 0x55E0));
L5C0C:
    /* 5C0C  neg     bp */
    BP = (uint16_t)-BP;
L5C0E:
    /* 5C0E  add     bp,word ptr ss:[55E4h] */
    BP = (uint16_t)(BP + rw(pSS, 0x55E4));
L5C13:
    /* 5C13  sar     bp,1 */
    BP = (uint16_t)((int16_t)BP >> 1);
L5C15:
    /* 5C15  sar     bp,1 */
    BP = (uint16_t)((int16_t)BP >> 1);
L5C17:
    /* 5C17  mov     si,cx */
    SI = CX;
L5C19:
    /* 5C19  inc     bx */
    BX = (uint16_t)(BX + 1);
L5C1A:
    /* 5C1A  inc     cx */
    CX = (uint16_t)(CX + 1);
L5C1B:
    /* 5C1B  inc     bp */
    BP = inc16(BP);
L5C1C:
    /* 5C1C  je      L5C28 */
    if (ZF) goto L5C28;
L5C1E:
    /* 5C1E  xchg    bp,cx */
    { uint16_t t_ = CX;
    CX = BP;
    BP = t_; }
L5C20: /* L5C20 */
    /* 5C20  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L5C21:
    /* 5C21  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L5C24:
    /* 5C24  loop    L5C20 */
    if (--CX) goto L5C20;
L5C26:
    /* 5C26  xchg    bp,cx */
    { uint16_t t_ = CX;
    CX = BP;
    BP = t_; }
L5C28: /* L5C28 */
    /* 5C28  mov     bp,bx */
    BP = BX;
L5C2A:
    /* 5C2A  mov     di,bp */
    DI = BP;
L5C2C:
    /* 5C2C  mov     al,byte ptr [bp+LMASKS] */
    AL = rb(pSS, BP + 0x3B62);
L5C30:
    /* 5C30  and     al,byte ptr [bp+RMASKS] */
    AL = (uint8_t)(AL & rb(pSS, BP + 0x3CA6));
L5C34:
    /* 5C34  out     dx,al */
    asm_out8(DX, AL);
L5C35:
    /* 5C35  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L5C37:
    /* 5C37  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L5C39:
    /* 5C39  add     di,word ptr ss:[55E0h] */
    DI = (uint16_t)(DI + rw(pSS, 0x55E0));
L5C3E:
    /* 5C3E  neg     bp */
    BP = (uint16_t)-BP;
L5C40:
    /* 5C40  add     bp,word ptr ss:[55E4h] */
    BP = (uint16_t)(BP + rw(pSS, 0x55E4));
L5C45:
    /* 5C45  sar     bp,1 */
    BP = (uint16_t)((int16_t)BP >> 1);
L5C47:
    /* 5C47  sar     bp,1 */
    BP = (uint16_t)((int16_t)BP >> 1);
L5C49:
    /* 5C49  mov     si,cx */
    SI = CX;
L5C4B:
    /* 5C4B  inc     bx */
    BX = (uint16_t)(BX + 1);
L5C4C:
    /* 5C4C  inc     cx */
    CX = (uint16_t)(CX + 1);
L5C4D:
    /* 5C4D  inc     bp */
    BP = inc16(BP);
L5C4E:
    /* 5C4E  je      L5C5A */
    if (ZF) goto L5C5A;
L5C50:
    /* 5C50  xchg    bp,cx */
    { uint16_t t_ = CX;
    CX = BP;
    BP = t_; }
L5C52: /* L5C52 */
    /* 5C52  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L5C53:
    /* 5C53  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L5C56:
    /* 5C56  loop    L5C52 */
    if (--CX) goto L5C52;
L5C58:
    /* 5C58  xchg    bp,cx */
    { uint16_t t_ = CX;
    CX = BP;
    BP = t_; }
L5C5A: /* L5C5A */
    /* 5C5A  pop     ds */
    SET_DS(pop16());
L5C5B:
    /* 5C5B  pop     si */
    SI = pop16();
L5C5C:
    /* 5C5C  jmp     L5B74 */
    goto L5B74;
L5C5F: /* L5C5F */
    /* 5C5F  pop     es */
    SET_ES(pop16());
L5C60:
    /* 5C60  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* _5C61: as _5B6C, skipping source bytes of colour 0 (transparent). */
L5C61: /* _seg003_5C61 */
    /* 5C61  mov     dx,SC_DATA */
    DX = 0x3C5;
L5C64:
    /* 5C64  push    es */
    push16(asm_es);
L5C65:
    /* 5C65  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L5C69: /* L5C69 */
    /* 5C69  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5C6A:
    /* 5C6A  shl     ax,1 */
    AX = shl16(AX, 1);
L5C6C:
    /* 5C6C  jb      L5C5F */
    if (CF) goto L5C5F;
L5C6E:
    /* 5C6E  mov     bp,ax */
    BP = AX;
L5C70:
    /* 5C70  mov     ax,word ptr [bp+YTAB] */
    AX = rw(pSS, BP + 0x36AA);
L5C74:
    /* 5C74  mov     word ptr ds:[55E0h],ax */
    ww(pDS, 0x55E0, AX);
L5C77:
    /* 5C77  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5C78:
    /* 5C78  mov     bx,ax */
    BX = AX;
L5C7A:
    /* 5C7A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5C7B:
    /* 5C7B  mov     word ptr ds:[55E4h],ax */
    ww(pDS, 0x55E4, AX);
L5C7E:
    /* 5C7E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5C7F:
    /* 5C7F  mov     cx,ax */
    CX = AX;
L5C81:
    /* 5C81  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L5C83:
    /* 5C83  push    si */
    push16(SI);
L5C84:
    /* 5C84  push    ds */
    push16(asm_ds);
L5C85:
    /* 5C85  mov     ds,word ptr ds:[55EAh] */
    SET_DS(rw(pDS, 0x55EA));
L5C89:
    /* 5C89  mov     bp,bx */
    BP = BX;
L5C8B:
    /* 5C8B  mov     di,bp */
    DI = BP;
L5C8D:
    /* 5C8D  mov     al,byte ptr [bp+LMASKS] */
    AL = rb(pSS, BP + 0x3B62);
L5C91:
    /* 5C91  and     al,byte ptr [bp+RMASKS] */
    AL = (uint8_t)(AL & rb(pSS, BP + 0x3CA6));
L5C95:
    /* 5C95  out     dx,al */
    asm_out8(DX, AL);
L5C96:
    /* 5C96  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L5C98:
    /* 5C98  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L5C9A:
    /* 5C9A  add     di,word ptr ss:[55E0h] */
    DI = (uint16_t)(DI + rw(pSS, 0x55E0));
L5C9F:
    /* 5C9F  neg     bp */
    BP = (uint16_t)-BP;
L5CA1:
    /* 5CA1  add     bp,word ptr ss:[55E4h] */
    BP = (uint16_t)(BP + rw(pSS, 0x55E4));
L5CA6:
    /* 5CA6  sar     bp,1 */
    BP = (uint16_t)((int16_t)BP >> 1);
L5CA8:
    /* 5CA8  sar     bp,1 */
    BP = (uint16_t)((int16_t)BP >> 1);
L5CAA:
    /* 5CAA  mov     si,cx */
    SI = CX;
L5CAC:
    /* 5CAC  inc     bx */
    BX = (uint16_t)(BX + 1);
L5CAD:
    /* 5CAD  inc     cx */
    CX = (uint16_t)(CX + 1);
L5CAE:
    /* 5CAE  inc     bp */
    BP = inc16(BP);
L5CAF:
    /* 5CAF  je      L5CC2 */
    if (ZF) goto L5CC2;
L5CB1:
    /* 5CB1  xchg    bp,cx */
    { uint16_t t_ = CX;
    CX = BP;
    BP = t_; }
L5CB3: /* L5CB3 */
    /* 5CB3  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L5CB4:
    /* 5CB4  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L5CB7:
    /* 5CB7  cmp     al,ah */
    sub8(AL, AH, 0);
L5CB9:
    /* 5CB9  je      L5CBD */
    if (ZF) goto L5CBD;
L5CBB:
    /* 5CBB  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L5CBC:
    /* 5CBC  dec     di */
    DI = (uint16_t)(DI - 1);
L5CBD: /* L5CBD */
    /* 5CBD  inc     di */
    DI = (uint16_t)(DI + 1);
L5CBE:
    /* 5CBE  loop    L5CB3 */
    if (--CX) goto L5CB3;
L5CC0:
    /* 5CC0  xchg    bp,cx */
    { uint16_t t_ = CX;
    CX = BP;
    BP = t_; }
L5CC2: /* L5CC2 */
    /* 5CC2  mov     bp,bx */
    BP = BX;
L5CC4:
    /* 5CC4  mov     di,bp */
    DI = BP;
L5CC6:
    /* 5CC6  mov     al,byte ptr [bp+LMASKS] */
    AL = rb(pSS, BP + 0x3B62);
L5CCA:
    /* 5CCA  and     al,byte ptr [bp+RMASKS] */
    AL = (uint8_t)(AL & rb(pSS, BP + 0x3CA6));
L5CCE:
    /* 5CCE  out     dx,al */
    asm_out8(DX, AL);
L5CCF:
    /* 5CCF  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L5CD1:
    /* 5CD1  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L5CD3:
    /* 5CD3  add     di,word ptr ss:[55E0h] */
    DI = (uint16_t)(DI + rw(pSS, 0x55E0));
L5CD8:
    /* 5CD8  neg     bp */
    BP = (uint16_t)-BP;
L5CDA:
    /* 5CDA  add     bp,word ptr ss:[55E4h] */
    BP = (uint16_t)(BP + rw(pSS, 0x55E4));
L5CDF:
    /* 5CDF  sar     bp,1 */
    BP = (uint16_t)((int16_t)BP >> 1);
L5CE1:
    /* 5CE1  sar     bp,1 */
    BP = (uint16_t)((int16_t)BP >> 1);
L5CE3:
    /* 5CE3  mov     si,cx */
    SI = CX;
L5CE5:
    /* 5CE5  inc     bx */
    BX = (uint16_t)(BX + 1);
L5CE6:
    /* 5CE6  inc     cx */
    CX = (uint16_t)(CX + 1);
L5CE7:
    /* 5CE7  inc     bp */
    BP = inc16(BP);
L5CE8:
    /* 5CE8  je      L5CFB */
    if (ZF) goto L5CFB;
L5CEA:
    /* 5CEA  xchg    bp,cx */
    { uint16_t t_ = CX;
    CX = BP;
    BP = t_; }
L5CEC: /* L5CEC */
    /* 5CEC  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L5CED:
    /* 5CED  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L5CF0:
    /* 5CF0  cmp     al,ah */
    sub8(AL, AH, 0);
L5CF2:
    /* 5CF2  je      L5CF6 */
    if (ZF) goto L5CF6;
L5CF4:
    /* 5CF4  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L5CF5:
    /* 5CF5  dec     di */
    DI = (uint16_t)(DI - 1);
L5CF6: /* L5CF6 */
    /* 5CF6  inc     di */
    DI = (uint16_t)(DI + 1);
L5CF7:
    /* 5CF7  loop    L5CEC */
    if (--CX) goto L5CEC;
L5CF9:
    /* 5CF9  xchg    bp,cx */
    { uint16_t t_ = CX;
    CX = BP;
    BP = t_; }
L5CFB: /* L5CFB */
    /* 5CFB  mov     bp,bx */
    BP = BX;
L5CFD:
    /* 5CFD  mov     di,bp */
    DI = BP;
L5CFF:
    /* 5CFF  mov     al,byte ptr [bp+LMASKS] */
    AL = rb(pSS, BP + 0x3B62);
L5D03:
    /* 5D03  and     al,byte ptr [bp+RMASKS] */
    AL = (uint8_t)(AL & rb(pSS, BP + 0x3CA6));
L5D07:
    /* 5D07  out     dx,al */
    asm_out8(DX, AL);
L5D08:
    /* 5D08  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L5D0A:
    /* 5D0A  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L5D0C:
    /* 5D0C  add     di,word ptr ss:[55E0h] */
    DI = (uint16_t)(DI + rw(pSS, 0x55E0));
L5D11:
    /* 5D11  neg     bp */
    BP = (uint16_t)-BP;
L5D13:
    /* 5D13  add     bp,word ptr ss:[55E4h] */
    BP = (uint16_t)(BP + rw(pSS, 0x55E4));
L5D18:
    /* 5D18  sar     bp,1 */
    BP = (uint16_t)((int16_t)BP >> 1);
L5D1A:
    /* 5D1A  sar     bp,1 */
    BP = (uint16_t)((int16_t)BP >> 1);
L5D1C:
    /* 5D1C  mov     si,cx */
    SI = CX;
L5D1E:
    /* 5D1E  inc     bx */
    BX = (uint16_t)(BX + 1);
L5D1F:
    /* 5D1F  inc     cx */
    CX = (uint16_t)(CX + 1);
L5D20:
    /* 5D20  inc     bp */
    BP = inc16(BP);
L5D21:
    /* 5D21  je      L5D34 */
    if (ZF) goto L5D34;
L5D23:
    /* 5D23  xchg    bp,cx */
    { uint16_t t_ = CX;
    CX = BP;
    BP = t_; }
L5D25: /* L5D25 */
    /* 5D25  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L5D26:
    /* 5D26  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L5D29:
    /* 5D29  cmp     al,ah */
    sub8(AL, AH, 0);
L5D2B:
    /* 5D2B  je      L5D2F */
    if (ZF) goto L5D2F;
L5D2D:
    /* 5D2D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L5D2E:
    /* 5D2E  dec     di */
    DI = (uint16_t)(DI - 1);
L5D2F: /* L5D2F */
    /* 5D2F  inc     di */
    DI = (uint16_t)(DI + 1);
L5D30:
    /* 5D30  loop    L5D25 */
    if (--CX) goto L5D25;
L5D32:
    /* 5D32  xchg    bp,cx */
    { uint16_t t_ = CX;
    CX = BP;
    BP = t_; }
L5D34: /* L5D34 */
    /* 5D34  mov     bp,bx */
    BP = BX;
L5D36:
    /* 5D36  mov     di,bp */
    DI = BP;
L5D38:
    /* 5D38  mov     al,byte ptr [bp+LMASKS] */
    AL = rb(pSS, BP + 0x3B62);
L5D3C:
    /* 5D3C  and     al,byte ptr [bp+RMASKS] */
    AL = (uint8_t)(AL & rb(pSS, BP + 0x3CA6));
L5D40:
    /* 5D40  out     dx,al */
    asm_out8(DX, AL);
L5D41:
    /* 5D41  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L5D43:
    /* 5D43  sar     di,1 */
    DI = (uint16_t)((int16_t)DI >> 1);
L5D45:
    /* 5D45  add     di,word ptr ss:[55E0h] */
    DI = (uint16_t)(DI + rw(pSS, 0x55E0));
L5D4A:
    /* 5D4A  neg     bp */
    BP = (uint16_t)-BP;
L5D4C:
    /* 5D4C  add     bp,word ptr ss:[55E4h] */
    BP = (uint16_t)(BP + rw(pSS, 0x55E4));
L5D51:
    /* 5D51  sar     bp,1 */
    BP = (uint16_t)((int16_t)BP >> 1);
L5D53:
    /* 5D53  sar     bp,1 */
    BP = (uint16_t)((int16_t)BP >> 1);
L5D55:
    /* 5D55  mov     si,cx */
    SI = CX;
L5D57:
    /* 5D57  inc     bx */
    BX = (uint16_t)(BX + 1);
L5D58:
    /* 5D58  inc     cx */
    CX = (uint16_t)(CX + 1);
L5D59:
    /* 5D59  inc     bp */
    BP = inc16(BP);
L5D5A:
    /* 5D5A  je      L5D6D */
    if (ZF) goto L5D6D;
L5D5C:
    /* 5D5C  xchg    bp,cx */
    { uint16_t t_ = CX;
    CX = BP;
    BP = t_; }
L5D5E: /* L5D5E */
    /* 5D5E  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L5D5F:
    /* 5D5F  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L5D62:
    /* 5D62  cmp     al,ah */
    sub8(AL, AH, 0);
L5D64:
    /* 5D64  je      L5D68 */
    if (ZF) goto L5D68;
L5D66:
    /* 5D66  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L5D67:
    /* 5D67  dec     di */
    DI = (uint16_t)(DI - 1);
L5D68: /* L5D68 */
    /* 5D68  inc     di */
    DI = (uint16_t)(DI + 1);
L5D69:
    /* 5D69  loop    L5D5E */
    if (--CX) goto L5D5E;
L5D6B:
    /* 5D6B  xchg    bp,cx */
    { uint16_t t_ = CX;
    CX = BP;
    BP = t_; }
L5D6D: /* L5D6D */
    /* 5D6D  pop     ds */
    SET_DS(pop16());
L5D6E:
    /* 5D6E  pop     si */
    SI = pop16();
L5D6F:
    /* 5D6F  jmp     L5C69 */
    goto L5C69;

    /* _5D72: rows from video memory (ES:offset) to the screen when the planes do not line up: for
       each plane, select it for reading (3CEh index 4) and writing, and copy its bytes. Ends by
       setting the bit mask back to 0FFh. */
L5D72: /* _seg003_5D72 */
    /* 5D72  push    es */
    push16(asm_es);
L5D73:
    /* 5D73  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L5D77:
    /* 5D77  mov     bp,si */
    BP = SI;
L5D79: /* L5D79 */
    /* 5D79  mov     bx,word ptr [bp] */
    BX = rw(pSS, BP);
L5D7C:
    /* 5D7C  shl     bx,1 */
    BX = shl16(BX, 1);
L5D7E:
    /* 5D7E  jae     L5D83 */
    if (!CF) goto L5D83;
L5D80:
    /* 5D80  jmp     L5E7C */
    goto L5E7C;
L5D83: /* L5D83 */
    /* 5D83  mov     di,word ptr [bx+YTAB] */
    DI = rw(pDS, BX + 0x36AA);
L5D87:
    /* 5D87  mov     word ptr ds:[55E0h],di */
    ww(pDS, 0x55E0, DI);
L5D8B:
    /* 5D8B  mov     bx,word ptr [bp+2] */
    BX = rw(pSS, BP + 0x2);
L5D8E:
    /* 5D8E  mov     di,word ptr [bp+4] */
    DI = rw(pSS, BP + 0x4);
L5D91:
    /* 5D91  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L5D94:
    /* 5D94  add     bp,8 */
    BP = (uint16_t)(BP + 0x8);
L5D97:
    /* 5D97  push    bp */
    push16(BP);
L5D98:
    /* 5D98  mov     bp,ax */
    BP = AX;
L5D9A:
    /* 5D9A  sub     di,bx */
    DI = (uint16_t)(DI - BX);
L5D9C:
    /* 5D9C  inc     di */
    DI = (uint16_t)(DI + 1);
L5D9D:
    /* 5D9D  mov     word ptr ds:[55E6h],di */
    ww(pDS, 0x55E6, DI);
L5DA1:
    /* 5DA1  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L5DA3:
    /* 5DA3  mov     dx,GC_INDEX */
    DX = 0x3CE;
L5DA6:
    /* 5DA6  mov     al,4 */
    AL = 0x4;
L5DA8:
    /* 5DA8  out     dx,al */
    asm_out8(DX, AL);
L5DA9:
    /* 5DA9  mov     al,byte ptr [bx+LMASKS] */
    AL = rb(pDS, BX + 0x3B62);
L5DAD:
    /* 5DAD  and     al,byte ptr [bx+RMASKS] */
    AL = (uint8_t)(AL & rb(pDS, BX + 0x3CA6));
L5DB1:
    /* 5DB1  mov     dx,SC_DATA */
    DX = 0x3C5;
L5DB4:
    /* 5DB4  out     dx,al */
    asm_out8(DX, AL);
L5DB5:
    /* 5DB5  mov     al,ah */
    AL = AH;
L5DB7:
    /* 5DB7  mov     dx,GC_DATA */
    DX = 0x3CF;
L5DBA:
    /* 5DBA  out     dx,al */
    asm_out8(DX, AL);
L5DBB:
    /* 5DBB  mov     cx,word ptr ds:[55E6h] */
    CX = rw(pDS, 0x55E6);
L5DBF:
    /* 5DBF  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L5DC2:
    /* 5DC2  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L5DC5:
    /* 5DC5  mov     si,bp */
    SI = BP;
L5DC7:
    /* 5DC7  mov     di,bx */
    DI = BX;
L5DC9:
    /* 5DC9  shr     di,2 */
    DI = (uint16_t)(DI >> 2);
L5DCC:
    /* 5DCC  add     di,word ptr ds:[55E0h] */
    DI = (uint16_t)(DI + rw(pDS, 0x55E0));
L5DD0:
    /* 5DD0  rep movs byte ptr es:[di],byte ptr es:[si] */
    while (CX) { wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L5DD3:
    /* 5DD3  inc     bx */
    BX = (uint16_t)(BX + 1);
L5DD4:
    /* 5DD4  inc     ah */
    AH = (uint8_t)(AH + 1);
L5DD6:
    /* 5DD6  dec     word ptr ds:[55E6h] */
    ww(pDS, 0x55E6, dec16(rw(pDS, 0x55E6)));
L5DDA:
    /* 5DDA  jne     L5DDF */
    if (!ZF) goto L5DDF;
L5DDC:
    /* 5DDC  jmp     L5E78 */
    goto L5E78;
L5DDF: /* L5DDF */
    /* 5DDF  mov     al,byte ptr [bx+LMASKS] */
    AL = rb(pDS, BX + 0x3B62);
L5DE3:
    /* 5DE3  and     al,byte ptr [bx+RMASKS] */
    AL = (uint8_t)(AL & rb(pDS, BX + 0x3CA6));
L5DE7:
    /* 5DE7  mov     dx,SC_DATA */
    DX = 0x3C5;
L5DEA:
    /* 5DEA  out     dx,al */
    asm_out8(DX, AL);
L5DEB:
    /* 5DEB  mov     al,ah */
    AL = AH;
L5DED:
    /* 5DED  mov     dx,GC_DATA */
    DX = 0x3CF;
L5DF0:
    /* 5DF0  out     dx,al */
    asm_out8(DX, AL);
L5DF1:
    /* 5DF1  mov     cx,word ptr ds:[55E6h] */
    CX = rw(pDS, 0x55E6);
L5DF5:
    /* 5DF5  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L5DF8:
    /* 5DF8  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L5DFB:
    /* 5DFB  mov     si,bp */
    SI = BP;
L5DFD:
    /* 5DFD  mov     di,bx */
    DI = BX;
L5DFF:
    /* 5DFF  shr     di,2 */
    DI = (uint16_t)(DI >> 2);
L5E02:
    /* 5E02  add     di,word ptr ds:[55E0h] */
    DI = (uint16_t)(DI + rw(pDS, 0x55E0));
L5E06:
    /* 5E06  rep movs byte ptr es:[di],byte ptr es:[si] */
    while (CX) { wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L5E09:
    /* 5E09  inc     bx */
    BX = (uint16_t)(BX + 1);
L5E0A:
    /* 5E0A  inc     ah */
    AH = (uint8_t)(AH + 1);
L5E0C:
    /* 5E0C  dec     word ptr ds:[55E6h] */
    ww(pDS, 0x55E6, dec16(rw(pDS, 0x55E6)));
L5E10:
    /* 5E10  je      L5E78 */
    if (ZF) goto L5E78;
L5E12:
    /* 5E12  mov     al,byte ptr [bx+LMASKS] */
    AL = rb(pDS, BX + 0x3B62);
L5E16:
    /* 5E16  and     al,byte ptr [bx+RMASKS] */
    AL = (uint8_t)(AL & rb(pDS, BX + 0x3CA6));
L5E1A:
    /* 5E1A  mov     dx,SC_DATA */
    DX = 0x3C5;
L5E1D:
    /* 5E1D  out     dx,al */
    asm_out8(DX, AL);
L5E1E:
    /* 5E1E  mov     al,ah */
    AL = AH;
L5E20:
    /* 5E20  mov     dx,GC_DATA */
    DX = 0x3CF;
L5E23:
    /* 5E23  out     dx,al */
    asm_out8(DX, AL);
L5E24:
    /* 5E24  mov     cx,word ptr ds:[55E6h] */
    CX = rw(pDS, 0x55E6);
L5E28:
    /* 5E28  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L5E2B:
    /* 5E2B  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L5E2E:
    /* 5E2E  mov     si,bp */
    SI = BP;
L5E30:
    /* 5E30  mov     di,bx */
    DI = BX;
L5E32:
    /* 5E32  shr     di,2 */
    DI = (uint16_t)(DI >> 2);
L5E35:
    /* 5E35  add     di,word ptr ds:[55E0h] */
    DI = (uint16_t)(DI + rw(pDS, 0x55E0));
L5E39:
    /* 5E39  rep movs byte ptr es:[di],byte ptr es:[si] */
    while (CX) { wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L5E3C:
    /* 5E3C  inc     bx */
    BX = (uint16_t)(BX + 1);
L5E3D:
    /* 5E3D  inc     ah */
    AH = (uint8_t)(AH + 1);
L5E3F:
    /* 5E3F  dec     word ptr ds:[55E6h] */
    ww(pDS, 0x55E6, dec16(rw(pDS, 0x55E6)));
L5E43:
    /* 5E43  je      L5E78 */
    if (ZF) goto L5E78;
L5E45:
    /* 5E45  mov     al,byte ptr [bx+LMASKS] */
    AL = rb(pDS, BX + 0x3B62);
L5E49:
    /* 5E49  and     al,byte ptr [bx+RMASKS] */
    AL = (uint8_t)(AL & rb(pDS, BX + 0x3CA6));
L5E4D:
    /* 5E4D  mov     dx,SC_DATA */
    DX = 0x3C5;
L5E50:
    /* 5E50  out     dx,al */
    asm_out8(DX, AL);
L5E51:
    /* 5E51  mov     al,ah */
    AL = AH;
L5E53:
    /* 5E53  mov     dx,GC_DATA */
    DX = 0x3CF;
L5E56:
    /* 5E56  out     dx,al */
    asm_out8(DX, AL);
L5E57:
    /* 5E57  mov     cx,word ptr ds:[55E6h] */
    CX = rw(pDS, 0x55E6);
L5E5B:
    /* 5E5B  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L5E5E:
    /* 5E5E  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L5E61:
    /* 5E61  mov     si,bp */
    SI = BP;
L5E63:
    /* 5E63  mov     di,bx */
    DI = BX;
L5E65:
    /* 5E65  shr     di,2 */
    DI = (uint16_t)(DI >> 2);
L5E68:
    /* 5E68  add     di,word ptr ds:[55E0h] */
    DI = (uint16_t)(DI + rw(pDS, 0x55E0));
L5E6C:
    /* 5E6C  rep movs byte ptr es:[di],byte ptr es:[si] */
    while (CX) { wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L5E6F:
    /* 5E6F  inc     bx */
    BX = (uint16_t)(BX + 1);
L5E70:
    /* 5E70  inc     ah */
    AH = (uint8_t)(AH + 1);
L5E72:
    /* 5E72  dec     word ptr ds:[55E6h] */
    ww(pDS, 0x55E6, dec16(rw(pDS, 0x55E6)));
L5E76:
    /* 5E76  je      L5E78 */
    if (ZF) goto L5E78;
L5E78: /* L5E78 */
    /* 5E78  pop     bp */
    BP = pop16();
L5E79:
    /* 5E79  jmp     L5D79 */
    goto L5D79;
L5E7C: /* L5E7C */
    /* 5E7C  mov     dx,GC_INDEX */
    DX = 0x3CE;
L5E7F:
    /* 5E7F  mov     ax,0FF08h */
    AX = 0xFF08;
L5E82:
    /* 5E82  out     dx,ax */
    asm_out16(DX, AX);
L5E83:
    /* 5E83  pop     es */
    SET_ES(pop16());
L5E84:
    /* 5E84  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* _5E85: rows from video memory to the screen, plane-aligned: bit mask 0, so the bytes come from
       the latches; whole groups of four pixels with all planes enabled, then the last partial group
       with the right-edge plane mask. */
L5E85: /* _seg003_5E85 */
    /* 5E85  mov     dx,GC_DATA */
    DX = 0x3CF;
L5E88:
    /* 5E88  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L5E8A:
    /* 5E8A  out     dx,al */
    asm_out8(DX, AL);
L5E8B:
    /* 5E8B  mov     dx,SC_DATA */
    DX = 0x3C5;
L5E8E:
    /* 5E8E  push    es */
    push16(asm_es);
L5E8F:
    /* 5E8F  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L5E93:
    /* 5E93  mov     bx,si */
    BX = SI;
L5E95: /* L5E95 */
    /* 5E95  mov     bp,word ptr [bx] */
    BP = rw(pDS, BX);
L5E97:
    /* 5E97  shl     bp,1 */
    BP = shl16(BP, 1);
L5E99:
    /* 5E99  jb      L5ED5 */
    if (CF) goto L5ED5;
L5E9B:
    /* 5E9B  mov     di,word ptr [bp+YTAB] */
    DI = rw(pSS, BP + 0x36AA);
L5E9F:
    /* 5E9F  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L5EA2:
    /* 5EA2  mov     cx,word ptr [bx+4] */
    CX = rw(pDS, BX + 0x4);
L5EA5:
    /* 5EA5  mov     bp,cx */
    BP = CX;
L5EA7:
    /* 5EA7  mov     si,word ptr [bx+6] */
    SI = rw(pDS, BX + 0x6);
L5EAA:
    /* 5EAA  add     bx,8 */
    BX = (uint16_t)(BX + 0x8);
L5EAD:
    /* 5EAD  sub     cx,ax */
    CX = (uint16_t)(CX - AX);
L5EAF:
    /* 5EAF  inc     cx */
    CX = (uint16_t)(CX + 1);
L5EB0:
    /* 5EB0  shr     ax,2 */
    AX = (uint16_t)(AX >> 2);
L5EB3:
    /* 5EB3  add     di,ax */
    DI = (uint16_t)(DI + AX);
L5EB5:
    /* 5EB5  mov     ax,cx */
    AX = CX;
L5EB7:
    /* 5EB7  shr     cx,2 */
    CX = shr16(CX, 2);
L5EBA:
    /* 5EBA  je      L5ECC */
    if (ZF) goto L5ECC;
L5EBC:
    /* 5EBC  mov     al,0Fh */
    AL = 0xF;
L5EBE:
    /* 5EBE  out     dx,al */
    asm_out8(DX, AL);
L5EBF:
    /* 5EBF  rep movs byte ptr es:[di],byte ptr es:[si] */
    while (CX) { wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L5EC2:
    /* 5EC2  mov     cx,bp */
    CX = BP;
L5EC4:
    /* 5EC4  and     cx,3 */
    CX = (uint16_t)(CX & 0x3);
L5EC7:
    /* 5EC7  sub     cx,3 */
    CX = sub16(CX, 0x3, 0);
L5ECA:
    /* 5ECA  je      L5E95 */
    if (ZF) goto L5E95;
L5ECC: /* L5ECC */
    /* 5ECC  mov     al,byte ptr [bp+RMASKS] */
    AL = rb(pSS, BP + 0x3CA6);
L5ED0:
    /* 5ED0  out     dx,al */
    asm_out8(DX, AL);
L5ED1:
    /* 5ED1  movs    byte ptr es:[di],byte ptr es:[si] */
    wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L5ED3:
    /* 5ED3  jmp     L5E95 */
    goto L5E95;
L5ED5: /* L5ED5 */
    /* 5ED5  pop     es */
    SET_ES(pop16());
L5ED6:
    /* 5ED6  mov     dx,GC_DATA */
    DX = 0x3CF;
L5ED9:
    /* 5ED9  mov     al,0FFh */
    AL = 0xFF;
L5EDB:
    /* 5EDB  out     dx,al */
    asm_out8(DX, AL);
L5EDC:
    /* 5EDC  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* _5EDD: as _5D72, skipping colour 0. */
L5EDD: /* _seg003_5EDD */
    /* 5EDD  push    es */
    push16(asm_es);
L5EDE:
    /* 5EDE  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L5EE2: /* L5EE2 */
    /* 5EE2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5EE3:
    /* 5EE3  shl     ax,1 */
    AX = shl16(AX, 1);
L5EE5:
    /* 5EE5  jae     L5EEA */
    if (!CF) goto L5EEA;
L5EE7:
    /* 5EE7  jmp     L600C */
    goto L600C;
L5EEA: /* L5EEA */
    /* 5EEA  mov     bx,ax */
    BX = AX;
L5EEC:
    /* 5EEC  mov     di,word ptr [bx+YTAB] */
    DI = rw(pDS, BX + 0x36AA);
L5EF0:
    /* 5EF0  mov     word ptr ds:[55E0h],di */
    ww(pDS, 0x55E0, DI);
L5EF4:
    /* 5EF4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5EF5:
    /* 5EF5  mov     bx,ax */
    BX = AX;
L5EF7:
    /* 5EF7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5EF8:
    /* 5EF8  mov     di,ax */
    DI = AX;
L5EFA:
    /* 5EFA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5EFB:
    /* 5EFB  mov     bp,ax */
    BP = AX;
L5EFD:
    /* 5EFD  sub     di,bx */
    DI = (uint16_t)(DI - BX);
L5EFF:
    /* 5EFF  inc     di */
    DI = (uint16_t)(DI + 1);
L5F00:
    /* 5F00  mov     word ptr ds:[55E6h],di */
    ww(pDS, 0x55E6, DI);
L5F04:
    /* 5F04  push    si */
    push16(SI);
L5F05:
    /* 5F05  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L5F07:
    /* 5F07  mov     dx,GC_INDEX */
    DX = 0x3CE;
L5F0A:
    /* 5F0A  mov     al,4 */
    AL = 0x4;
L5F0C:
    /* 5F0C  out     dx,al */
    asm_out8(DX, AL);
L5F0D:
    /* 5F0D  mov     al,byte ptr [bx+LMASKS] */
    AL = rb(pDS, BX + 0x3B62);
L5F11:
    /* 5F11  and     al,byte ptr [bx+RMASKS] */
    AL = (uint8_t)(AL & rb(pDS, BX + 0x3CA6));
L5F15:
    /* 5F15  mov     dx,SC_DATA */
    DX = 0x3C5;
L5F18:
    /* 5F18  out     dx,al */
    asm_out8(DX, AL);
L5F19:
    /* 5F19  mov     al,ah */
    AL = AH;
L5F1B:
    /* 5F1B  mov     dx,GC_DATA */
    DX = 0x3CF;
L5F1E:
    /* 5F1E  out     dx,al */
    asm_out8(DX, AL);
L5F1F:
    /* 5F1F  mov     cx,word ptr ds:[55E6h] */
    CX = rw(pDS, 0x55E6);
L5F23:
    /* 5F23  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L5F26:
    /* 5F26  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L5F29:
    /* 5F29  mov     si,bp */
    SI = BP;
L5F2B:
    /* 5F2B  mov     di,bx */
    DI = BX;
L5F2D:
    /* 5F2D  shr     di,2 */
    DI = (uint16_t)(DI >> 2);
L5F30:
    /* 5F30  add     di,word ptr ds:[55E0h] */
    DI = (uint16_t)(DI + rw(pDS, 0x55E0));
L5F34: /* L5F34 */
    /* 5F34  lods    byte ptr es:[si] */
    AL = rb(pES, SI); SI = (uint16_t)(SI + STEP(1));
L5F36:
    /* 5F36  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L5F38:
    /* 5F38  je      L5F3C */
    if (ZF) goto L5F3C;
L5F3A:
    /* 5F3A  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L5F3B:
    /* 5F3B  dec     di */
    DI = (uint16_t)(DI - 1);
L5F3C: /* L5F3C */
    /* 5F3C  inc     di */
    DI = (uint16_t)(DI + 1);
L5F3D:
    /* 5F3D  loop    L5F34 */
    if (--CX) goto L5F34;
L5F3F:
    /* 5F3F  inc     bx */
    BX = (uint16_t)(BX + 1);
L5F40:
    /* 5F40  inc     ah */
    AH = (uint8_t)(AH + 1);
L5F42:
    /* 5F42  and     ah,3 */
    AH = (uint8_t)(AH & 0x3);
L5F45:
    /* 5F45  dec     word ptr ds:[55E6h] */
    ww(pDS, 0x55E6, dec16(rw(pDS, 0x55E6)));
L5F49:
    /* 5F49  jne     L5F4E */
    if (!ZF) goto L5F4E;
L5F4B:
    /* 5F4B  jmp     L6008 */
    goto L6008;
L5F4E: /* L5F4E */
    /* 5F4E  mov     al,byte ptr [bx+LMASKS] */
    AL = rb(pDS, BX + 0x3B62);
L5F52:
    /* 5F52  and     al,byte ptr [bx+RMASKS] */
    AL = (uint8_t)(AL & rb(pDS, BX + 0x3CA6));
L5F56:
    /* 5F56  mov     dx,SC_DATA */
    DX = 0x3C5;
L5F59:
    /* 5F59  out     dx,al */
    asm_out8(DX, AL);
L5F5A:
    /* 5F5A  mov     al,ah */
    AL = AH;
L5F5C:
    /* 5F5C  mov     dx,GC_DATA */
    DX = 0x3CF;
L5F5F:
    /* 5F5F  out     dx,al */
    asm_out8(DX, AL);
L5F60:
    /* 5F60  mov     cx,word ptr ds:[55E6h] */
    CX = rw(pDS, 0x55E6);
L5F64:
    /* 5F64  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L5F67:
    /* 5F67  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L5F6A:
    /* 5F6A  mov     si,bp */
    SI = BP;
L5F6C:
    /* 5F6C  mov     di,bx */
    DI = BX;
L5F6E:
    /* 5F6E  shr     di,2 */
    DI = (uint16_t)(DI >> 2);
L5F71:
    /* 5F71  add     di,word ptr ds:[55E0h] */
    DI = (uint16_t)(DI + rw(pDS, 0x55E0));
L5F75: /* L5F75 */
    /* 5F75  lods    byte ptr es:[si] */
    AL = rb(pES, SI); SI = (uint16_t)(SI + STEP(1));
L5F77:
    /* 5F77  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L5F79:
    /* 5F79  je      L5F7D */
    if (ZF) goto L5F7D;
L5F7B:
    /* 5F7B  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L5F7C:
    /* 5F7C  dec     di */
    DI = (uint16_t)(DI - 1);
L5F7D: /* L5F7D */
    /* 5F7D  inc     di */
    DI = (uint16_t)(DI + 1);
L5F7E:
    /* 5F7E  loop    L5F75 */
    if (--CX) goto L5F75;
L5F80:
    /* 5F80  inc     bx */
    BX = (uint16_t)(BX + 1);
L5F81:
    /* 5F81  inc     ah */
    AH = (uint8_t)(AH + 1);
L5F83:
    /* 5F83  and     ah,3 */
    AH = (uint8_t)(AH & 0x3);
L5F86:
    /* 5F86  dec     word ptr ds:[55E6h] */
    ww(pDS, 0x55E6, dec16(rw(pDS, 0x55E6)));
L5F8A:
    /* 5F8A  je      L6008 */
    if (ZF) goto L6008;
L5F8C:
    /* 5F8C  mov     al,byte ptr [bx+LMASKS] */
    AL = rb(pDS, BX + 0x3B62);
L5F90:
    /* 5F90  and     al,byte ptr [bx+RMASKS] */
    AL = (uint8_t)(AL & rb(pDS, BX + 0x3CA6));
L5F94:
    /* 5F94  mov     dx,SC_DATA */
    DX = 0x3C5;
L5F97:
    /* 5F97  out     dx,al */
    asm_out8(DX, AL);
L5F98:
    /* 5F98  mov     al,ah */
    AL = AH;
L5F9A:
    /* 5F9A  mov     dx,GC_DATA */
    DX = 0x3CF;
L5F9D:
    /* 5F9D  out     dx,al */
    asm_out8(DX, AL);
L5F9E:
    /* 5F9E  mov     cx,word ptr ds:[55E6h] */
    CX = rw(pDS, 0x55E6);
L5FA2:
    /* 5FA2  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L5FA5:
    /* 5FA5  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L5FA8:
    /* 5FA8  mov     si,bp */
    SI = BP;
L5FAA:
    /* 5FAA  mov     di,bx */
    DI = BX;
L5FAC:
    /* 5FAC  shr     di,2 */
    DI = (uint16_t)(DI >> 2);
L5FAF:
    /* 5FAF  add     di,word ptr ds:[55E0h] */
    DI = (uint16_t)(DI + rw(pDS, 0x55E0));
L5FB3: /* L5FB3 */
    /* 5FB3  lods    byte ptr es:[si] */
    AL = rb(pES, SI); SI = (uint16_t)(SI + STEP(1));
L5FB5:
    /* 5FB5  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L5FB7:
    /* 5FB7  je      L5FBB */
    if (ZF) goto L5FBB;
L5FB9:
    /* 5FB9  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L5FBA:
    /* 5FBA  dec     di */
    DI = (uint16_t)(DI - 1);
L5FBB: /* L5FBB */
    /* 5FBB  inc     di */
    DI = (uint16_t)(DI + 1);
L5FBC:
    /* 5FBC  loop    L5FB3 */
    if (--CX) goto L5FB3;
L5FBE:
    /* 5FBE  inc     bx */
    BX = (uint16_t)(BX + 1);
L5FBF:
    /* 5FBF  inc     ah */
    AH = (uint8_t)(AH + 1);
L5FC1:
    /* 5FC1  and     ah,3 */
    AH = (uint8_t)(AH & 0x3);
L5FC4:
    /* 5FC4  dec     word ptr ds:[55E6h] */
    ww(pDS, 0x55E6, dec16(rw(pDS, 0x55E6)));
L5FC8:
    /* 5FC8  je      L6008 */
    if (ZF) goto L6008;
L5FCA:
    /* 5FCA  mov     al,byte ptr [bx+LMASKS] */
    AL = rb(pDS, BX + 0x3B62);
L5FCE:
    /* 5FCE  and     al,byte ptr [bx+RMASKS] */
    AL = (uint8_t)(AL & rb(pDS, BX + 0x3CA6));
L5FD2:
    /* 5FD2  mov     dx,SC_DATA */
    DX = 0x3C5;
L5FD5:
    /* 5FD5  out     dx,al */
    asm_out8(DX, AL);
L5FD6:
    /* 5FD6  mov     al,ah */
    AL = AH;
L5FD8:
    /* 5FD8  mov     dx,GC_DATA */
    DX = 0x3CF;
L5FDB:
    /* 5FDB  out     dx,al */
    asm_out8(DX, AL);
L5FDC:
    /* 5FDC  mov     cx,word ptr ds:[55E6h] */
    CX = rw(pDS, 0x55E6);
L5FE0:
    /* 5FE0  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L5FE3:
    /* 5FE3  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L5FE6:
    /* 5FE6  mov     si,bp */
    SI = BP;
L5FE8:
    /* 5FE8  mov     di,bx */
    DI = BX;
L5FEA:
    /* 5FEA  shr     di,2 */
    DI = (uint16_t)(DI >> 2);
L5FED:
    /* 5FED  add     di,word ptr ds:[55E0h] */
    DI = (uint16_t)(DI + rw(pDS, 0x55E0));
L5FF1: /* L5FF1 */
    /* 5FF1  lods    byte ptr es:[si] */
    AL = rb(pES, SI); SI = (uint16_t)(SI + STEP(1));
L5FF3:
    /* 5FF3  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L5FF5:
    /* 5FF5  je      L5FF9 */
    if (ZF) goto L5FF9;
L5FF7:
    /* 5FF7  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L5FF8:
    /* 5FF8  dec     di */
    DI = (uint16_t)(DI - 1);
L5FF9: /* L5FF9 */
    /* 5FF9  inc     di */
    DI = (uint16_t)(DI + 1);
L5FFA:
    /* 5FFA  loop    L5FF1 */
    if (--CX) goto L5FF1;
L5FFC:
    /* 5FFC  inc     bx */
    BX = (uint16_t)(BX + 1);
L5FFD:
    /* 5FFD  inc     ah */
    AH = (uint8_t)(AH + 1);
L5FFF:
    /* 5FFF  and     ah,3 */
    AH = (uint8_t)(AH & 0x3);
L6002:
    /* 6002  dec     word ptr ds:[55E6h] */
    ww(pDS, 0x55E6, dec16(rw(pDS, 0x55E6)));
L6006:
    /* 6006  je      L6008 */
    if (ZF) goto L6008;
L6008: /* L6008 */
    /* 6008  pop     si */
    SI = pop16();
L6009:
    /* 6009  jmp     L5EE2 */
    goto L5EE2;
L600C: /* L600C */
    /* 600C  mov     dx,GC_INDEX */
    DX = 0x3CE;
L600F:
    /* 600F  mov     ax,0FF08h */
    AX = 0xFF08;
L6012:
    /* 6012  out     dx,ax */
    asm_out16(DX, AX);
L6013:
    /* 6013  pop     es */
    SET_ES(pop16());
L6014:
    /* 6014  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* _6015: screen to screen through the latches, left to right (vcopy); the record's source offset
       is the destination here and the row address the source. */
L6015: /* _seg003_6015 */
    /* 6015  mov     dx,GC_DATA */
    DX = 0x3CF;
L6018:
    /* 6018  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L601A:
    /* 601A  out     dx,al */
    asm_out8(DX, AL);
L601B:
    /* 601B  mov     dx,SC_DATA */
    DX = 0x3C5;
L601E:
    /* 601E  push    es */
    push16(asm_es);
L601F:
    /* 601F  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L6023:
    /* 6023  mov     bx,si */
    BX = SI;
L6025: /* L6025 */
    /* 6025  mov     bp,word ptr [bx] */
    BP = rw(pDS, BX);
L6027:
    /* 6027  shl     bp,1 */
    BP = shl16(BP, 1);
L6029:
    /* 6029  jb      L6065 */
    if (CF) goto L6065;
L602B:
    /* 602B  mov     si,word ptr [bp+YTAB] */
    SI = rw(pSS, BP + 0x36AA);
L602F:
    /* 602F  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L6032:
    /* 6032  mov     cx,word ptr [bx+4] */
    CX = rw(pDS, BX + 0x4);
L6035:
    /* 6035  mov     bp,cx */
    BP = CX;
L6037:
    /* 6037  mov     di,word ptr [bx+6] */
    DI = rw(pDS, BX + 0x6);
L603A:
    /* 603A  add     bx,8 */
    BX = (uint16_t)(BX + 0x8);
L603D:
    /* 603D  sub     cx,ax */
    CX = (uint16_t)(CX - AX);
L603F:
    /* 603F  inc     cx */
    CX = (uint16_t)(CX + 1);
L6040:
    /* 6040  shr     ax,2 */
    AX = (uint16_t)(AX >> 2);
L6043:
    /* 6043  add     si,ax */
    SI = (uint16_t)(SI + AX);
L6045:
    /* 6045  mov     ax,cx */
    AX = CX;
L6047:
    /* 6047  shr     cx,2 */
    CX = shr16(CX, 2);
L604A:
    /* 604A  je      L605C */
    if (ZF) goto L605C;
L604C:
    /* 604C  mov     al,0Fh */
    AL = 0xF;
L604E:
    /* 604E  out     dx,al */
    asm_out8(DX, AL);
L604F:
    /* 604F  rep movs byte ptr es:[di],byte ptr es:[si] */
    while (CX) { wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L6052:
    /* 6052  mov     cx,bp */
    CX = BP;
L6054:
    /* 6054  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L6057:
    /* 6057  sub     cx,3 */
    CX = sub16(CX, 0x3, 0);
L605A:
    /* 605A  je      L6025 */
    if (ZF) goto L6025;
L605C: /* L605C */
    /* 605C  mov     al,byte ptr [bp+RMASKS] */
    AL = rb(pSS, BP + 0x3CA6);
L6060:
    /* 6060  out     dx,al */
    asm_out8(DX, AL);
L6061:
    /* 6061  movs    byte ptr es:[di],byte ptr es:[si] */
    wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L6063:
    /* 6063  jmp     L6025 */
    goto L6025;
L6065: /* L6065 */
    /* 6065  pop     es */
    SET_ES(pop16());
L6066:
    /* 6066  mov     dx,GC_DATA */
    DX = 0x3CF;

    /* seg003_6069  (+6069)
       _6069: the bit mask back to 0FFh (DX = 3CFh), the tail of _6015. _606D is _6015 right to left
       (std), for a copy onto an overlapping area further right. */
L6069: /* _seg003_6069 */
    /* 6069  mov     al,0FFh */
    AL = 0xFF;
L606B:
    /* 606B  out     dx,al */
    asm_out8(DX, AL);
L606C:
    /* 606C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L606D: /* _seg003_606D */
    /* 606D  mov     dx,GC_DATA */
    DX = 0x3CF;
L6070:
    /* 6070  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L6072:
    /* 6072  out     dx,al */
    asm_out8(DX, AL);
L6073:
    /* 6073  mov     dx,SC_DATA */
    DX = 0x3C5;
L6076:
    /* 6076  push    es */
    push16(asm_es);
L6077:
    /* 6077  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L607B:
    /* 607B  mov     bx,si */
    BX = SI;
L607D:
    /* 607D  std */
    DF = 1;
L607E: /* L607E */
    /* 607E  mov     bp,word ptr [bx] */
    BP = rw(pDS, BX);
L6080:
    /* 6080  shl     bp,1 */
    BP = shl16(BP, 1);
L6082:
    /* 6082  jb      L60BE */
    if (CF) goto L60BE;
L6084:
    /* 6084  mov     si,word ptr [bp+YTAB] */
    SI = rw(pSS, BP + 0x36AA);
L6088:
    /* 6088  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L608B:
    /* 608B  mov     cx,word ptr [bx+4] */
    CX = rw(pDS, BX + 0x4);
L608E:
    /* 608E  mov     di,word ptr [bx+6] */
    DI = rw(pDS, BX + 0x6);
L6091:
    /* 6091  add     bx,8 */
    BX = (uint16_t)(BX + 0x8);
L6094:
    /* 6094  sub     cx,ax */
    CX = (uint16_t)(CX - AX);
L6096:
    /* 6096  inc     cx */
    CX = (uint16_t)(CX + 1);
L6097:
    /* 6097  mov     bp,cx */
    BP = CX;
L6099:
    /* 6099  shr     ax,2 */
    AX = (uint16_t)(AX >> 2);
L609C:
    /* 609C  add     si,ax */
    SI = (uint16_t)(SI + AX);
L609E:
    /* 609E  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L60A1:
    /* 60A1  dec     cx */
    CX = (uint16_t)(CX - 1);
L60A2:
    /* 60A2  add     si,cx */
    SI = (uint16_t)(SI + CX);
L60A4:
    /* 60A4  add     di,cx */
    DI = (uint16_t)(DI + CX);
L60A6:
    /* 60A6  test    bp,3 */
    logic16((uint16_t)(BP & 0x3));
L60AA:
    /* 60AA  je      L60B5 */
    if (ZF) goto L60B5;
L60AC:
    /* 60AC  mov     al,byte ptr [bp+RMASKS] */
    AL = rb(pSS, BP + 0x3CA6);
L60B0:
    /* 60B0  out     dx,al */
    asm_out8(DX, AL);
L60B1:
    /* 60B1  movs    byte ptr es:[di],byte ptr es:[si] */
    wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L60B3:
    /* 60B3  jcxz    L607E */
    if (!CX) goto L607E;
L60B5: /* L60B5 */
    /* 60B5  inc     cx */
    CX = (uint16_t)(CX + 1);
L60B6:
    /* 60B6  mov     al,0Fh */
    AL = 0xF;
L60B8:
    /* 60B8  out     dx,al */
    asm_out8(DX, AL);
L60B9:
    /* 60B9  rep movs byte ptr es:[di],byte ptr es:[si] */
    while (CX) { wb(pES, DI, rb(pES, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L60BC:
    /* 60BC  jmp     L607E */
    goto L607E;
L60BE: /* L60BE */
    /* 60BE  cld */
    DF = 0;
L60BF:
    /* 60BF  pop     es */
    SET_ES(pop16());
L60C0:
    /* 60C0  mov     dx,GC_DATA */
    DX = 0x3CF;
L60C3:
    /* 60C3  mov     al,0FFh */
    AL = 0xFF;
L60C5:
    /* 60C5  out     dx,al */
    asm_out8(DX, AL);
L60C6:
    /* 60C6  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* _60C7: linear bitmap rows to the linear frame buffer (segment 39CE, row offsets 39D0), opaque,
       with rep movsb. */
L60C7: /* _seg003_60C7 */
    /* 60C7  push    es */
    push16(asm_es);
L60C8:
    /* 60C8  push    ds */
    push16(asm_ds);
L60C9:
    /* 60C9  mov     es,word ptr ss:[39CEh] */
    SET_ES(rw(pSS, 0x39CE));
L60CE:
    /* 60CE  mov     ds,word ptr ss:[55EAh] */
    SET_DS(rw(pSS, 0x55EA));
L60D3: /* L60D3 */
    /* 60D3  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L60D5:
    /* 60D5  shl     ax,1 */
    AX = shl16(AX, 1);
L60D7:
    /* 60D7  jb      L60F5 */
    if (CF) goto L60F5;
L60D9:
    /* 60D9  mov     bx,ax */
    BX = AX;
L60DB:
    /* 60DB  mov     di,word ptr ss:[bx+39D0h] */
    DI = rw(pSS, BX + 0x39D0);
L60E0:
    /* 60E0  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L60E2:
    /* 60E2  add     di,ax */
    DI = (uint16_t)(DI + AX);
L60E4:
    /* 60E4  mov     cx,ax */
    CX = AX;
L60E6:
    /* 60E6  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L60E8:
    /* 60E8  sub     ax,cx */
    AX = (uint16_t)(AX - CX);
L60EA:
    /* 60EA  inc     ax */
    AX = (uint16_t)(AX + 1);
L60EB:
    /* 60EB  mov     cx,ax */
    CX = AX;
L60ED:
    /* 60ED  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L60EF:
    /* 60EF  xchg    si,ax */
    { uint16_t t_ = SI;
    SI = AX;
    AX = t_; }
L60F0:
    /* 60F0  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L60F2:
    /* 60F2  xchg    si,ax */
    { uint16_t t_ = SI;
    SI = AX;
    AX = t_; }
L60F3:
    /* 60F3  jmp     L60D3 */
    goto L60D3;
L60F5: /* L60F5 */
    /* 60F5  pop     ds */
    SET_DS(pop16());
L60F6:
    /* 60F6  pop     es */
    SET_ES(pop16());
L60F7:
    /* 60F7  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* _60F8: as _60C7, skipping colour 0. */
L60F8: /* _seg003_60F8 */
    /* 60F8  push    es */
    push16(asm_es);
L60F9:
    /* 60F9  push    ds */
    push16(asm_ds);
L60FA:
    /* 60FA  mov     es,word ptr ss:[39CEh] */
    SET_ES(rw(pSS, 0x39CE));
L60FF:
    /* 60FF  mov     ds,word ptr ss:[55EAh] */
    SET_DS(rw(pSS, 0x55EA));
L6104: /* L6104 */
    /* 6104  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L6106:
    /* 6106  shl     ax,1 */
    AX = shl16(AX, 1);
L6108:
    /* 6108  jb      L6130 */
    if (CF) goto L6130;
L610A:
    /* 610A  mov     bx,ax */
    BX = AX;
L610C:
    /* 610C  mov     di,word ptr ss:[bx+39D0h] */
    DI = rw(pSS, BX + 0x39D0);
L6111:
    /* 6111  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L6113:
    /* 6113  add     di,ax */
    DI = (uint16_t)(DI + AX);
L6115:
    /* 6115  mov     cx,ax */
    CX = AX;
L6117:
    /* 6117  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L6119:
    /* 6119  sub     ax,cx */
    AX = (uint16_t)(AX - CX);
L611B:
    /* 611B  inc     ax */
    AX = (uint16_t)(AX + 1);
L611C:
    /* 611C  mov     cx,ax */
    CX = AX;
L611E:
    /* 611E  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L6120:
    /* 6120  push    si */
    push16(SI);
L6121:
    /* 6121  mov     si,ax */
    SI = AX;
L6123: /* L6123 */
    /* 6123  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L6124:
    /* 6124  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L6126:
    /* 6126  je      L612A */
    if (ZF) goto L612A;
L6128:
    /* 6128  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L6129:
    /* 6129  dec     di */
    DI = (uint16_t)(DI - 1);
L612A: /* L612A */
    /* 612A  inc     di */
    DI = (uint16_t)(DI + 1);
L612B:
    /* 612B  loop    L6123 */
    if (--CX) goto L6123;
L612D:
    /* 612D  pop     si */
    SI = pop16();
L612E:
    /* 612E  jmp     L6104 */
    goto L6104;
L6130: /* L6130 */
    /* 6130  pop     ds */
    SET_DS(pop16());
L6131:
    /* 6131  pop     es */
    SET_ES(pop16());
L6132:
    /* 6132  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
