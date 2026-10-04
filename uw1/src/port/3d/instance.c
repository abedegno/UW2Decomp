/* instance.c: replaces src/3d/INSTANCE.ASM (seg004_4980, 4980..5C25 of its
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

uint32_t asm_mod_INSTANCE(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x4980: goto L4980;
    case 0x4981: goto L4981;
    case 0x4982: goto L4982;
    case 0x4983: goto L4983;
    case 0x4987: goto L4987;
    case 0x4988: goto L4988;
    case 0x4989: goto L4989;
    case 0x498A: goto L498A;
    case 0x498D: goto L498D;
    case 0x498F: goto L498F;
    case 0x4992: goto L4992;
    case 0x4995: goto L4995;
    case 0x4998: goto L4998;
    case 0x499A: goto L499A;
    case 0x499C: goto L499C;
    case 0x49A0: goto L49A0;
    case 0x49A2: goto L49A2;
    case 0x49A4: goto L49A4;
    case 0x49A6: goto L49A6;
    case 0x49AA: goto L49AA;
    case 0x49AE: goto L49AE;
    case 0x49AF: goto L49AF;
    case 0x49B0: goto L49B0;
    case 0x49B1: goto L49B1;
    case 0x49B4: goto L49B4;
    case 0x49B6: goto L49B6;
    case 0x49B8: goto L49B8;
    case 0x49BE: goto L49BE;
    case 0x49C4: goto L49C4;
    case 0x49CA: goto L49CA;
    case 0x49D0: goto L49D0;
    case 0x49D6: goto L49D6;
    case 0x49DB: goto L49DB;
    case 0x49DF: goto L49DF;
    case 0x49E4: goto L49E4;
    case 0x49E5: goto L49E5;
    case 0x49E7: goto L49E7;
    case 0x49EA: goto L49EA;
    case 0x49EE: goto L49EE;
    case 0x49F3: goto L49F3;
    case 0x49F4: goto L49F4;
    case 0x49F6: goto L49F6;
    case 0x49F9: goto L49F9;
    case 0x49FD: goto L49FD;
    case 0x4A02: goto L4A02;
    case 0x4A03: goto L4A03;
    case 0x4A05: goto L4A05;
    case 0x4A08: goto L4A08;
    case 0x4A0C: goto L4A0C;
    case 0x4A10: goto L4A10;
    case 0x4A14: goto L4A14;
    case 0x4A18: goto L4A18;
    case 0x4A1C: goto L4A1C;
    case 0x4A20: goto L4A20;
    case 0x4A24: goto L4A24;
    case 0x4A29: goto L4A29;
    case 0x4A2B: goto L4A2B;
    case 0x4A2E: goto L4A2E;
    case 0x4A32: goto L4A32;
    case 0x4A36: goto L4A36;
    case 0x4A3A: goto L4A3A;
    case 0x4A3E: goto L4A3E;
    case 0x4A42: goto L4A42;
    case 0x4A46: goto L4A46;
    case 0x4A4A: goto L4A4A;
    case 0x4A4F: goto L4A4F;
    case 0x4A52: goto L4A52;
    case 0x4A55: goto L4A55;
    case 0x4A58: goto L4A58;
    case 0x4A5B: goto L4A5B;
    case 0x4A5F: goto L4A5F;
    case 0x4A64: goto L4A64;
    case 0x4A66: goto L4A66;
    case 0x4A68: goto L4A68;
    case 0x4A6C: goto L4A6C;
    case 0x4A6E: goto L4A6E;
    case 0x4A71: goto L4A71;
    case 0x4A72: goto L4A72;
    case 0x4A75: goto L4A75;
    case 0x4A76: goto L4A76;
    case 0x4A79: goto L4A79;
    case 0x4A7B: goto L4A7B;
    case 0x4A7E: goto L4A7E;
    case 0x4A81: goto L4A81;
    case 0x4A83: goto L4A83;
    case 0x4A85: goto L4A85;
    case 0x4A88: goto L4A88;
    case 0x4A8C: goto L4A8C;
    case 0x4A8F: goto L4A8F;
    case 0x4A91: goto L4A91;
    case 0x4A94: goto L4A94;
    case 0x4A98: goto L4A98;
    case 0x4A9E: goto L4A9E;
    case 0x4AA1: goto L4AA1;
    case 0x4AA3: goto L4AA3;
    case 0x4AA5: goto L4AA5;
    case 0x4AA6: goto L4AA6;
    case 0x4AA8: goto L4AA8;
    case 0x4AAB: goto L4AAB;
    case 0x4AAC: goto L4AAC;
    case 0x4AB1: goto L4AB1;
    case 0x4AB4: goto L4AB4;
    case 0x4AB6: goto L4AB6;
    case 0x4AB8: goto L4AB8;
    case 0x4AB9: goto L4AB9;
    case 0x4ABD: goto L4ABD;
    case 0x4AC1: goto L4AC1;
    case 0x4AC2: goto L4AC2;
    case 0x4AC6: goto L4AC6;
    case 0x4AC7: goto L4AC7;
    case 0x4AC8: goto L4AC8;
    case 0x4ACC: goto L4ACC;
    case 0x4ACF: goto L4ACF;
    case 0x4AD2: goto L4AD2;
    case 0x4AD4: goto L4AD4;
    case 0x4AD6: goto L4AD6;
    case 0x4AD7: goto L4AD7;
    case 0x4AD8: goto L4AD8;
    case 0x4AD9: goto L4AD9;
    case 0x4ADE: goto L4ADE;
    case 0x4AE0: goto L4AE0;
    case 0x4AE3: goto L4AE3;
    case 0x4AE7: goto L4AE7;
    case 0x4AEB: goto L4AEB;
    case 0x4AED: goto L4AED;
    case 0x4AEF: goto L4AEF;
    case 0x4AF1: goto L4AF1;
    case 0x4AF4: goto L4AF4;
    case 0x4AF6: goto L4AF6;
    case 0x4AF7: goto L4AF7;
    case 0x4AF9: goto L4AF9;
    case 0x4AFC: goto L4AFC;
    case 0x4B01: goto L4B01;
    case 0x4B03: goto L4B03;
    case 0x4B04: goto L4B04;
    case 0x4B05: goto L4B05;
    case 0x4B06: goto L4B06;
    case 0x4B07: goto L4B07;
    case 0x4B08: goto L4B08;
    case 0x4B0B: goto L4B0B;
    case 0x4B0D: goto L4B0D;
    case 0x4B10: goto L4B10;
    case 0x4B13: goto L4B13;
    case 0x4B16: goto L4B16;
    case 0x4B18: goto L4B18;
    case 0x4B1A: goto L4B1A;
    case 0x4B1E: goto L4B1E;
    case 0x4B20: goto L4B20;
    case 0x4B22: goto L4B22;
    case 0x4B24: goto L4B24;
    case 0x4B28: goto L4B28;
    case 0x4B2C: goto L4B2C;
    case 0x4B2D: goto L4B2D;
    case 0x4B2E: goto L4B2E;
    case 0x4B2F: goto L4B2F;
    case 0x4B32: goto L4B32;
    case 0x4B34: goto L4B34;
    case 0x4B38: goto L4B38;
    case 0x4B3B: goto L4B3B;
    case 0x4B3D: goto L4B3D;
    case 0x4B3F: goto L4B3F;
    case 0x4B41: goto L4B41;
    case 0x4B44: goto L4B44;
    case 0x4B4A: goto L4B4A;
    case 0x4B50: goto L4B50;
    case 0x4B53: goto L4B53;
    case 0x4B56: goto L4B56;
    case 0x4B59: goto L4B59;
    case 0x4B5C: goto L4B5C;
    case 0x4B5F: goto L4B5F;
    case 0x4B62: goto L4B62;
    case 0x4B66: goto L4B66;
    case 0x4B6A: goto L4B6A;
    case 0x4B6D: goto L4B6D;
    case 0x4B6F: goto L4B6F;
    case 0x4B71: goto L4B71;
    case 0x4B73: goto L4B73;
    case 0x4B75: goto L4B75;
    case 0x4B77: goto L4B77;
    case 0x4B79: goto L4B79;
    case 0x4B7B: goto L4B7B;
    case 0x4B7D: goto L4B7D;
    case 0x4B7F: goto L4B7F;
    case 0x4B81: goto L4B81;
    case 0x4B83: goto L4B83;
    case 0x4B85: goto L4B85;
    case 0x4B87: goto L4B87;
    case 0x4B89: goto L4B89;
    case 0x4B8B: goto L4B8B;
    case 0x4B8D: goto L4B8D;
    case 0x4B8F: goto L4B8F;
    case 0x4B92: goto L4B92;
    case 0x4B94: goto L4B94;
    case 0x4B96: goto L4B96;
    case 0x4B97: goto L4B97;
    case 0x4B99: goto L4B99;
    case 0x4B9B: goto L4B9B;
    case 0x4B9D: goto L4B9D;
    case 0x4B9F: goto L4B9F;
    case 0x4BA3: goto L4BA3;
    case 0x4BA6: goto L4BA6;
    case 0x4BA8: goto L4BA8;
    case 0x4BAA: goto L4BAA;
    case 0x4BAC: goto L4BAC;
    case 0x4BB0: goto L4BB0;
    case 0x4BB3: goto L4BB3;
    case 0x4BB5: goto L4BB5;
    case 0x4BB7: goto L4BB7;
    case 0x4BB9: goto L4BB9;
    case 0x4BBD: goto L4BBD;
    case 0x4BC0: goto L4BC0;
    case 0x4BC2: goto L4BC2;
    case 0x4BC4: goto L4BC4;
    case 0x4BC6: goto L4BC6;
    case 0x4BCA: goto L4BCA;
    case 0x4BCD: goto L4BCD;
    case 0x4BCF: goto L4BCF;
    case 0x4BD1: goto L4BD1;
    case 0x4BD3: goto L4BD3;
    case 0x4BD7: goto L4BD7;
    case 0x4BDA: goto L4BDA;
    case 0x4BDC: goto L4BDC;
    case 0x4BDE: goto L4BDE;
    case 0x4BE0: goto L4BE0;
    case 0x4BE4: goto L4BE4;
    case 0x4BE6: goto L4BE6;
    case 0x4BE8: goto L4BE8;
    case 0x4BEA: goto L4BEA;
    case 0x4BEC: goto L4BEC;
    case 0x4BEE: goto L4BEE;
    case 0x4BF2: goto L4BF2;
    case 0x4BF5: goto L4BF5;
    case 0x4BF7: goto L4BF7;
    case 0x4BF9: goto L4BF9;
    case 0x4BFB: goto L4BFB;
    case 0x4BFF: goto L4BFF;
    case 0x4C02: goto L4C02;
    case 0x4C04: goto L4C04;
    case 0x4C06: goto L4C06;
    case 0x4C08: goto L4C08;
    case 0x4C0C: goto L4C0C;
    case 0x4C0F: goto L4C0F;
    case 0x4C11: goto L4C11;
    case 0x4C13: goto L4C13;
    case 0x4C15: goto L4C15;
    case 0x4C19: goto L4C19;
    case 0x4C1C: goto L4C1C;
    case 0x4C1E: goto L4C1E;
    case 0x4C20: goto L4C20;
    case 0x4C22: goto L4C22;
    case 0x4C26: goto L4C26;
    case 0x4C29: goto L4C29;
    case 0x4C2B: goto L4C2B;
    case 0x4C2D: goto L4C2D;
    case 0x4C2F: goto L4C2F;
    case 0x4C33: goto L4C33;
    case 0x4C37: goto L4C37;
    case 0x4C39: goto L4C39;
    case 0x4C3B: goto L4C3B;
    case 0x4C3E: goto L4C3E;
    case 0x4C42: goto L4C42;
    case 0x4C45: goto L4C45;
    case 0x4C47: goto L4C47;
    case 0x4C49: goto L4C49;
    case 0x4C4B: goto L4C4B;
    case 0x4C4F: goto L4C4F;
    case 0x4C52: goto L4C52;
    case 0x4C54: goto L4C54;
    case 0x4C56: goto L4C56;
    case 0x4C58: goto L4C58;
    case 0x4C5C: goto L4C5C;
    case 0x4C5F: goto L4C5F;
    case 0x4C61: goto L4C61;
    case 0x4C63: goto L4C63;
    case 0x4C65: goto L4C65;
    case 0x4C69: goto L4C69;
    case 0x4C6C: goto L4C6C;
    case 0x4C6E: goto L4C6E;
    case 0x4C70: goto L4C70;
    case 0x4C72: goto L4C72;
    case 0x4C76: goto L4C76;
    case 0x4C79: goto L4C79;
    case 0x4C7B: goto L4C7B;
    case 0x4C7D: goto L4C7D;
    case 0x4C7F: goto L4C7F;
    case 0x4C83: goto L4C83;
    case 0x4C85: goto L4C85;
    case 0x4C87: goto L4C87;
    case 0x4C89: goto L4C89;
    case 0x4C8B: goto L4C8B;
    case 0x4C8E: goto L4C8E;
    case 0x4C91: goto L4C91;
    case 0x4C93: goto L4C93;
    case 0x4C95: goto L4C95;
    case 0x4C97: goto L4C97;
    case 0x4C99: goto L4C99;
    case 0x4C9B: goto L4C9B;
    case 0x4CA0: goto L4CA0;
    case 0x4CA2: goto L4CA2;
    case 0x4CA4: goto L4CA4;
    case 0x4CA7: goto L4CA7;
    case 0x4CAA: goto L4CAA;
    case 0x4CAD: goto L4CAD;
    case 0x4CAF: goto L4CAF;
    case 0x4CB1: goto L4CB1;
    case 0x4CB3: goto L4CB3;
    case 0x4CB5: goto L4CB5;
    case 0x4CB7: goto L4CB7;
    case 0x4CBC: goto L4CBC;
    case 0x4CBE: goto L4CBE;
    case 0x4CC0: goto L4CC0;
    case 0x4CC3: goto L4CC3;
    case 0x4CC6: goto L4CC6;
    case 0x4CCC: goto L4CCC;
    case 0x4CCE: goto L4CCE;
    case 0x4CD1: goto L4CD1;
    case 0x4CD3: goto L4CD3;
    case 0x4CD5: goto L4CD5;
    case 0x4CD7: goto L4CD7;
    case 0x4CDB: goto L4CDB;
    case 0x4CDE: goto L4CDE;
    case 0x4CE0: goto L4CE0;
    case 0x4CE2: goto L4CE2;
    case 0x4CE4: goto L4CE4;
    case 0x4CE8: goto L4CE8;
    case 0x4CEB: goto L4CEB;
    case 0x4CED: goto L4CED;
    case 0x4CEF: goto L4CEF;
    case 0x4CF1: goto L4CF1;
    case 0x4CF5: goto L4CF5;
    case 0x4CF8: goto L4CF8;
    case 0x4CFA: goto L4CFA;
    case 0x4CFC: goto L4CFC;
    case 0x4CFE: goto L4CFE;
    case 0x4D02: goto L4D02;
    case 0x4D05: goto L4D05;
    case 0x4D07: goto L4D07;
    case 0x4D09: goto L4D09;
    case 0x4D0B: goto L4D0B;
    case 0x4D0F: goto L4D0F;
    case 0x4D12: goto L4D12;
    case 0x4D14: goto L4D14;
    case 0x4D16: goto L4D16;
    case 0x4D18: goto L4D18;
    case 0x4D1C: goto L4D1C;
    case 0x4D1F: goto L4D1F;
    case 0x4D21: goto L4D21;
    case 0x4D23: goto L4D23;
    case 0x4D25: goto L4D25;
    case 0x4D29: goto L4D29;
    case 0x4D2B: goto L4D2B;
    case 0x4D2E: goto L4D2E;
    case 0x4D30: goto L4D30;
    case 0x4D32: goto L4D32;
    case 0x4D34: goto L4D34;
    case 0x4D36: goto L4D36;
    case 0x4D38: goto L4D38;
    case 0x4D3A: goto L4D3A;
    case 0x4D3E: goto L4D3E;
    case 0x4D40: goto L4D40;
    case 0x4D42: goto L4D42;
    case 0x4D44: goto L4D44;
    case 0x4D46: goto L4D46;
    case 0x4D4A: goto L4D4A;
    case 0x4D4C: goto L4D4C;
    case 0x4D51: goto L4D51;
    case 0x4D53: goto L4D53;
    case 0x4D55: goto L4D55;
    case 0x4D58: goto L4D58;
    case 0x4D5B: goto L4D5B;
    case 0x4D5D: goto L4D5D;
    case 0x4D5F: goto L4D5F;
    case 0x4D61: goto L4D61;
    case 0x4D63: goto L4D63;
    case 0x4D65: goto L4D65;
    case 0x4D67: goto L4D67;
    case 0x4D6B: goto L4D6B;
    case 0x4D6D: goto L4D6D;
    case 0x4D6F: goto L4D6F;
    case 0x4D71: goto L4D71;
    case 0x4D73: goto L4D73;
    case 0x4D77: goto L4D77;
    case 0x4D79: goto L4D79;
    case 0x4D7E: goto L4D7E;
    case 0x4D80: goto L4D80;
    case 0x4D82: goto L4D82;
    case 0x4D85: goto L4D85;
    case 0x4D8C: goto L4D8C;
    case 0x4D8F: goto L4D8F;
    case 0x4D93: goto L4D93;
    case 0x4D95: goto L4D95;
    case 0x4D97: goto L4D97;
    case 0x4D9A: goto L4D9A;
    case 0x4D9E: goto L4D9E;
    case 0x4DA0: goto L4DA0;
    case 0x4DA2: goto L4DA2;
    case 0x4DA4: goto L4DA4;
    case 0x4DA6: goto L4DA6;
    case 0x4DAB: goto L4DAB;
    case 0x4DAF: goto L4DAF;
    case 0x4DB1: goto L4DB1;
    case 0x4DB3: goto L4DB3;
    case 0x4DB5: goto L4DB5;
    case 0x4DB7: goto L4DB7;
    case 0x4DBA: goto L4DBA;
    case 0x4DBC: goto L4DBC;
    case 0x4DBD: goto L4DBD;
    case 0x4DC0: goto L4DC0;
    case 0x4DC4: goto L4DC4;
    case 0x4DC6: goto L4DC6;
    case 0x4DC8: goto L4DC8;
    case 0x4DCA: goto L4DCA;
    case 0x4DCC: goto L4DCC;
    case 0x4DCF: goto L4DCF;
    case 0x4DD1: goto L4DD1;
    case 0x4DD2: goto L4DD2;
    case 0x4DD5: goto L4DD5;
    case 0x4DD7: goto L4DD7;
    case 0x4DDA: goto L4DDA;
    case 0x4DDE: goto L4DDE;
    case 0x4DE0: goto L4DE0;
    case 0x4DE2: goto L4DE2;
    case 0x4DE5: goto L4DE5;
    case 0x4DE9: goto L4DE9;
    case 0x4DEB: goto L4DEB;
    case 0x4DED: goto L4DED;
    case 0x4DF2: goto L4DF2;
    case 0x4DF6: goto L4DF6;
    case 0x4DF8: goto L4DF8;
    case 0x4DFA: goto L4DFA;
    case 0x4DFC: goto L4DFC;
    case 0x4DFE: goto L4DFE;
    case 0x4E01: goto L4E01;
    case 0x4E03: goto L4E03;
    case 0x4E04: goto L4E04;
    case 0x4E06: goto L4E06;
    case 0x4E0A: goto L4E0A;
    case 0x4E0C: goto L4E0C;
    case 0x4E0E: goto L4E0E;
    case 0x4E10: goto L4E10;
    case 0x4E12: goto L4E12;
    case 0x4E15: goto L4E15;
    case 0x4E17: goto L4E17;
    case 0x4E18: goto L4E18;
    case 0x4E1E: goto L4E1E;
    case 0x4E20: goto L4E20;
    case 0x4E22: goto L4E22;
    case 0x4E24: goto L4E24;
    case 0x4E28: goto L4E28;
    case 0x4E2B: goto L4E2B;
    case 0x4E2E: goto L4E2E;
    case 0x4E32: goto L4E32;
    case 0x4E34: goto L4E34;
    case 0x4E36: goto L4E36;
    case 0x4E39: goto L4E39;
    case 0x4E3B: goto L4E3B;
    case 0x4E3F: goto L4E3F;
    case 0x4E41: goto L4E41;
    case 0x4E43: goto L4E43;
    case 0x4E45: goto L4E45;
    case 0x4E47: goto L4E47;
    case 0x4E49: goto L4E49;
    case 0x4E4C: goto L4E4C;
    case 0x4E50: goto L4E50;
    case 0x4E53: goto L4E53;
    case 0x4E57: goto L4E57;
    case 0x4E59: goto L4E59;
    case 0x4E5B: goto L4E5B;
    case 0x4E5E: goto L4E5E;
    case 0x4E60: goto L4E60;
    case 0x4E64: goto L4E64;
    case 0x4E66: goto L4E66;
    case 0x4E68: goto L4E68;
    case 0x4E6A: goto L4E6A;
    case 0x4E6C: goto L4E6C;
    case 0x4E6E: goto L4E6E;
    case 0x4E71: goto L4E71;
    case 0x4E75: goto L4E75;
    case 0x4E76: goto L4E76;
    case 0x4E7C: goto L4E7C;
    case 0x4E7E: goto L4E7E;
    case 0x4E84: goto L4E84;
    case 0x4E86: goto L4E86;
    case 0x4E8C: goto L4E8C;
    case 0x4E8E: goto L4E8E;
    case 0x4E94: goto L4E94;
    case 0x4E96: goto L4E96;
    case 0x4E9C: goto L4E9C;
    case 0x4E9E: goto L4E9E;
    case 0x4EA1: goto L4EA1;
    case 0x4EA6: goto L4EA6;
    case 0x4EA8: goto L4EA8;
    case 0x4EAB: goto L4EAB;
    case 0x4EB0: goto L4EB0;
    case 0x4EB6: goto L4EB6;
    case 0x4EBC: goto L4EBC;
    case 0x4EC2: goto L4EC2;
    case 0x4EC8: goto L4EC8;
    case 0x4ECE: goto L4ECE;
    case 0x4ED4: goto L4ED4;
    case 0x4EDA: goto L4EDA;
    case 0x4EE0: goto L4EE0;
    case 0x4EE6: goto L4EE6;
    case 0x4EEC: goto L4EEC;
    case 0x4EF2: goto L4EF2;
    case 0x4EF8: goto L4EF8;
    case 0x4EFE: goto L4EFE;
    case 0x4F04: goto L4F04;
    case 0x4F09: goto L4F09;
    case 0x4F0B: goto L4F0B;
    case 0x4F11: goto L4F11;
    case 0x4F17: goto L4F17;
    case 0x4F1D: goto L4F1D;
    case 0x4F23: goto L4F23;
    case 0x4F29: goto L4F29;
    case 0x4F2F: goto L4F2F;
    case 0x4F35: goto L4F35;
    case 0x4F37: goto L4F37;
    case 0x4F3D: goto L4F3D;
    case 0x4F43: goto L4F43;
    case 0x4F49: goto L4F49;
    case 0x4F4F: goto L4F4F;
    case 0x4F55: goto L4F55;
    case 0x4F5B: goto L4F5B;
    case 0x4F61: goto L4F61;
    case 0x4F64: goto L4F64;
    case 0x4F67: goto L4F67;
    case 0x4F68: goto L4F68;
    case 0x4F69: goto L4F69;
    case 0x4F6A: goto L4F6A;
    case 0x4F6D: goto L4F6D;
    case 0x4F70: goto L4F70;
    case 0x4F71: goto L4F71;
    case 0x4F72: goto L4F72;
    case 0x4F77: goto L4F77;
    case 0x4F79: goto L4F79;
    case 0x4F7E: goto L4F7E;
    case 0x4F84: goto L4F84;
    case 0x4F8A: goto L4F8A;
    case 0x4F90: goto L4F90;
    case 0x4F96: goto L4F96;
    case 0x4F9C: goto L4F9C;
    case 0x4FA2: goto L4FA2;
    case 0x4FA8: goto L4FA8;
    case 0x4FAE: goto L4FAE;
    case 0x4FB4: goto L4FB4;
    case 0x4FBA: goto L4FBA;
    case 0x4FC0: goto L4FC0;
    case 0x4FC6: goto L4FC6;
    case 0x4FCC: goto L4FCC;
    case 0x4FD2: goto L4FD2;
    case 0x4FD7: goto L4FD7;
    case 0x4FD9: goto L4FD9;
    case 0x4FDF: goto L4FDF;
    case 0x4FE5: goto L4FE5;
    case 0x4FEB: goto L4FEB;
    case 0x4FF1: goto L4FF1;
    case 0x4FF7: goto L4FF7;
    case 0x4FFD: goto L4FFD;
    case 0x5003: goto L5003;
    case 0x5005: goto L5005;
    case 0x500B: goto L500B;
    case 0x5011: goto L5011;
    case 0x5017: goto L5017;
    case 0x501D: goto L501D;
    case 0x5023: goto L5023;
    case 0x5029: goto L5029;
    case 0x502F: goto L502F;
    case 0x5032: goto L5032;
    case 0x5035: goto L5035;
    case 0x5036: goto L5036;
    case 0x5037: goto L5037;
    case 0x5038: goto L5038;
    case 0x503B: goto L503B;
    case 0x503E: goto L503E;
    case 0x503F: goto L503F;
    case 0x5040: goto L5040;
    case 0x5042: goto L5042;
    case 0x5046: goto L5046;
    case 0x5048: goto L5048;
    case 0x5049: goto L5049;
    case 0x504A: goto L504A;
    case 0x504B: goto L504B;
    case 0x504E: goto L504E;
    case 0x5051: goto L5051;
    case 0x5055: goto L5055;
    case 0x5057: goto L5057;
    case 0x505A: goto L505A;
    case 0x505C: goto L505C;
    case 0x505D: goto L505D;
    case 0x505E: goto L505E;
    case 0x505F: goto L505F;
    case 0x5060: goto L5060;
    case 0x5063: goto L5063;
    case 0x5065: goto L5065;
    case 0x5066: goto L5066;
    case 0x5067: goto L5067;
    case 0x5068: goto L5068;
    case 0x5069: goto L5069;
    case 0x506C: goto L506C;
    case 0x5070: goto L5070;
    case 0x5074: goto L5074;
    case 0x5077: goto L5077;
    case 0x507B: goto L507B;
    case 0x507F: goto L507F;
    case 0x5082: goto L5082;
    case 0x5086: goto L5086;
    case 0x508A: goto L508A;
    case 0x508D: goto L508D;
    case 0x5091: goto L5091;
    case 0x5095: goto L5095;
    case 0x5096: goto L5096;
    case 0x5098: goto L5098;
    case 0x5099: goto L5099;
    case 0x509A: goto L509A;
    case 0x509D: goto L509D;
    case 0x50A0: goto L50A0;
    case 0x50A2: goto L50A2;
    case 0x50A4: goto L50A4;
    case 0x50A5: goto L50A5;
    case 0x50A6: goto L50A6;
    case 0x50A9: goto L50A9;
    case 0x50AC: goto L50AC;
    case 0x50AE: goto L50AE;
    case 0x50B0: goto L50B0;
    case 0x50B1: goto L50B1;
    case 0x50B2: goto L50B2;
    case 0x50B5: goto L50B5;
    case 0x50B8: goto L50B8;
    case 0x50BA: goto L50BA;
    case 0x50BC: goto L50BC;
    case 0x50BE: goto L50BE;
    case 0x50C0: goto L50C0;
    case 0x50C1: goto L50C1;
    case 0x50C4: goto L50C4;
    case 0x50C6: goto L50C6;
    case 0x50C8: goto L50C8;
    case 0x50C9: goto L50C9;
    case 0x50CC: goto L50CC;
    case 0x50CE: goto L50CE;
    case 0x50D0: goto L50D0;
    case 0x50D1: goto L50D1;
    case 0x50D4: goto L50D4;
    case 0x50D6: goto L50D6;
    case 0x50D8: goto L50D8;
    case 0x50DA: goto L50DA;
    case 0x50DE: goto L50DE;
    case 0x50E0: goto L50E0;
    case 0x50E2: goto L50E2;
    case 0x50E6: goto L50E6;
    case 0x50E8: goto L50E8;
    case 0x50EA: goto L50EA;
    case 0x50EE: goto L50EE;
    case 0x50F0: goto L50F0;
    case 0x50F4: goto L50F4;
    case 0x50F6: goto L50F6;
    case 0x50FA: goto L50FA;
    case 0x50FC: goto L50FC;
    case 0x50FE: goto L50FE;
    case 0x5102: goto L5102;
    case 0x5104: goto L5104;
    case 0x5106: goto L5106;
    case 0x510A: goto L510A;
    case 0x510C: goto L510C;
    case 0x510E: goto L510E;
    case 0x5112: goto L5112;
    case 0x5114: goto L5114;
    case 0x5116: goto L5116;
    case 0x511A: goto L511A;
    case 0x511C: goto L511C;
    case 0x511E: goto L511E;
    case 0x5122: goto L5122;
    case 0x5124: goto L5124;
    case 0x5126: goto L5126;
    case 0x512A: goto L512A;
    case 0x512B: goto L512B;
    case 0x512D: goto L512D;
    case 0x5131: goto L5131;
    case 0x5133: goto L5133;
    case 0x5135: goto L5135;
    case 0x5139: goto L5139;
    case 0x513B: goto L513B;
    case 0x513D: goto L513D;
    case 0x5141: goto L5141;
    case 0x5143: goto L5143;
    case 0x5147: goto L5147;
    case 0x5149: goto L5149;
    case 0x514D: goto L514D;
    case 0x514F: goto L514F;
    case 0x5151: goto L5151;
    case 0x5155: goto L5155;
    case 0x5157: goto L5157;
    case 0x5159: goto L5159;
    case 0x515D: goto L515D;
    case 0x515F: goto L515F;
    case 0x5161: goto L5161;
    case 0x5165: goto L5165;
    case 0x5167: goto L5167;
    case 0x5169: goto L5169;
    case 0x516D: goto L516D;
    case 0x516F: goto L516F;
    case 0x5171: goto L5171;
    case 0x5175: goto L5175;
    case 0x5177: goto L5177;
    case 0x5179: goto L5179;
    case 0x517D: goto L517D;
    case 0x517E: goto L517E;
    case 0x5180: goto L5180;
    case 0x5184: goto L5184;
    case 0x5186: goto L5186;
    case 0x5188: goto L5188;
    case 0x518C: goto L518C;
    case 0x518E: goto L518E;
    case 0x5190: goto L5190;
    case 0x5192: goto L5192;
    case 0x5196: goto L5196;
    case 0x5198: goto L5198;
    case 0x519A: goto L519A;
    case 0x519E: goto L519E;
    case 0x51A0: goto L51A0;
    case 0x51A2: goto L51A2;
    case 0x51A3: goto L51A3;
    case 0x51A6: goto L51A6;
    case 0x51A7: goto L51A7;
    case 0x51A9: goto L51A9;
    case 0x51AB: goto L51AB;
    case 0x51AE: goto L51AE;
    case 0x51B0: goto L51B0;
    case 0x51B3: goto L51B3;
    case 0x51B5: goto L51B5;
    case 0x51B8: goto L51B8;
    case 0x51B9: goto L51B9;
    case 0x51BB: goto L51BB;
    case 0x51BD: goto L51BD;
    case 0x51BF: goto L51BF;
    case 0x51C1: goto L51C1;
    case 0x51C4: goto L51C4;
    case 0x51C5: goto L51C5;
    case 0x51C7: goto L51C7;
    case 0x51C9: goto L51C9;
    case 0x51CB: goto L51CB;
    case 0x51CD: goto L51CD;
    case 0x51D0: goto L51D0;
    case 0x51D2: goto L51D2;
    case 0x51D5: goto L51D5;
    case 0x51D7: goto L51D7;
    case 0x51D9: goto L51D9;
    case 0x51DD: goto L51DD;
    case 0x51DF: goto L51DF;
    case 0x51E1: goto L51E1;
    case 0x51E6: goto L51E6;
    case 0x51EB: goto L51EB;
    case 0x51F0: goto L51F0;
    case 0x51F5: goto L51F5;
    case 0x51F8: goto L51F8;
    case 0x51FC: goto L51FC;
    case 0x5200: goto L5200;
    case 0x5203: goto L5203;
    case 0x5206: goto L5206;
    case 0x520A: goto L520A;
    case 0x520E: goto L520E;
    case 0x520F: goto L520F;
    case 0x5214: goto L5214;
    case 0x5218: goto L5218;
    case 0x521C: goto L521C;
    case 0x521E: goto L521E;
    case 0x5221: goto L5221;
    case 0x5223: goto L5223;
    case 0x5225: goto L5225;
    case 0x5226: goto L5226;
    case 0x5227: goto L5227;
    case 0x522A: goto L522A;
    case 0x522C: goto L522C;
    case 0x522E: goto L522E;
    case 0x5230: goto L5230;
    case 0x5231: goto L5231;
    case 0x5232: goto L5232;
    case 0x5234: goto L5234;
    case 0x5236: goto L5236;
    case 0x5239: goto L5239;
    case 0x523C: goto L523C;
    case 0x5240: goto L5240;
    case 0x5242: goto L5242;
    case 0x5244: goto L5244;
    case 0x5248: goto L5248;
    case 0x524A: goto L524A;
    case 0x524C: goto L524C;
    case 0x524D: goto L524D;
    case 0x524E: goto L524E;
    case 0x5250: goto L5250;
    case 0x5252: goto L5252;
    case 0x5254: goto L5254;
    case 0x5256: goto L5256;
    case 0x5257: goto L5257;
    case 0x5258: goto L5258;
    case 0x5259: goto L5259;
    case 0x525D: goto L525D;
    case 0x5260: goto L5260;
    case 0x5262: goto L5262;
    case 0x5264: goto L5264;
    case 0x5265: goto L5265;
    case 0x5267: goto L5267;
    case 0x5269: goto L5269;
    case 0x526C: goto L526C;
    case 0x526D: goto L526D;
    case 0x526E: goto L526E;
    case 0x5271: goto L5271;
    case 0x5273: goto L5273;
    case 0x5275: goto L5275;
    case 0x5277: goto L5277;
    case 0x527A: goto L527A;
    case 0x527B: goto L527B;
    case 0x527D: goto L527D;
    case 0x527F: goto L527F;
    case 0x5281: goto L5281;
    case 0x5285: goto L5285;
    case 0x5287: goto L5287;
    case 0x528A: goto L528A;
    case 0x528D: goto L528D;
    case 0x528F: goto L528F;
    case 0x5291: goto L5291;
    case 0x5294: goto L5294;
    case 0x5296: goto L5296;
    case 0x5297: goto L5297;
    case 0x5298: goto L5298;
    case 0x5299: goto L5299;
    case 0x529E: goto L529E;
    case 0x52A0: goto L52A0;
    case 0x52A3: goto L52A3;
    case 0x52A7: goto L52A7;
    case 0x52AB: goto L52AB;
    case 0x52AE: goto L52AE;
    case 0x52B2: goto L52B2;
    case 0x52B6: goto L52B6;
    case 0x52BA: goto L52BA;
    case 0x52BE: goto L52BE;
    case 0x52BF: goto L52BF;
    case 0x52C0: goto L52C0;
    case 0x52C3: goto L52C3;
    case 0x52C5: goto L52C5;
    case 0x52C7: goto L52C7;
    case 0x52C9: goto L52C9;
    case 0x52CB: goto L52CB;
    case 0x52CF: goto L52CF;
    case 0x52D1: goto L52D1;
    case 0x52D3: goto L52D3;
    case 0x52D4: goto L52D4;
    case 0x52D6: goto L52D6;
    case 0x52D8: goto L52D8;
    case 0x52DA: goto L52DA;
    case 0x52DC: goto L52DC;
    case 0x52DE: goto L52DE;
    case 0x52E0: goto L52E0;
    case 0x52E1: goto L52E1;
    case 0x52E2: goto L52E2;
    case 0x52E6: goto L52E6;
    case 0x52E7: goto L52E7;
    case 0x52E9: goto L52E9;
    case 0x52EA: goto L52EA;
    case 0x52EB: goto L52EB;
    case 0x52EC: goto L52EC;
    case 0x52EE: goto L52EE;
    case 0x52F0: goto L52F0;
    case 0x52F2: goto L52F2;
    case 0x52F4: goto L52F4;
    case 0x52F6: goto L52F6;
    case 0x52F8: goto L52F8;
    case 0x52F9: goto L52F9;
    case 0x52FA: goto L52FA;
    case 0x52FE: goto L52FE;
    case 0x52FF: goto L52FF;
    case 0x5300: goto L5300;
    case 0x5302: goto L5302;
    case 0x5306: goto L5306;
    case 0x5308: goto L5308;
    case 0x530A: goto L530A;
    case 0x530C: goto L530C;
    case 0x5310: goto L5310;
    case 0x5312: goto L5312;
    case 0x5314: goto L5314;
    case 0x5316: goto L5316;
    case 0x531A: goto L531A;
    case 0x531C: goto L531C;
    case 0x531E: goto L531E;
    case 0x5322: goto L5322;
    case 0x5326: goto L5326;
    case 0x5328: goto L5328;
    case 0x532C: goto L532C;
    case 0x532E: goto L532E;
    case 0x5330: goto L5330;
    case 0x5332: goto L5332;
    case 0x5336: goto L5336;
    case 0x5338: goto L5338;
    case 0x533A: goto L533A;
    case 0x533C: goto L533C;
    case 0x5340: goto L5340;
    case 0x5342: goto L5342;
    case 0x5344: goto L5344;
    case 0x5348: goto L5348;
    case 0x534C: goto L534C;
    case 0x534E: goto L534E;
    case 0x5352: goto L5352;
    case 0x5354: goto L5354;
    case 0x5356: goto L5356;
    case 0x5358: goto L5358;
    case 0x535C: goto L535C;
    case 0x535E: goto L535E;
    case 0x5360: goto L5360;
    case 0x5362: goto L5362;
    case 0x5366: goto L5366;
    case 0x5368: goto L5368;
    case 0x536A: goto L536A;
    case 0x536E: goto L536E;
    case 0x5372: goto L5372;
    case 0x5373: goto L5373;
    case 0x5374: goto L5374;
    case 0x5375: goto L5375;
    case 0x5377: goto L5377;
    case 0x537B: goto L537B;
    case 0x537D: goto L537D;
    case 0x537F: goto L537F;
    case 0x5381: goto L5381;
    case 0x5385: goto L5385;
    case 0x5387: goto L5387;
    case 0x5389: goto L5389;
    case 0x538B: goto L538B;
    case 0x538F: goto L538F;
    case 0x5391: goto L5391;
    case 0x5393: goto L5393;
    case 0x5397: goto L5397;
    case 0x539B: goto L539B;
    case 0x539D: goto L539D;
    case 0x53A1: goto L53A1;
    case 0x53A3: goto L53A3;
    case 0x53A5: goto L53A5;
    case 0x53A7: goto L53A7;
    case 0x53AB: goto L53AB;
    case 0x53AD: goto L53AD;
    case 0x53AF: goto L53AF;
    case 0x53B1: goto L53B1;
    case 0x53B5: goto L53B5;
    case 0x53B7: goto L53B7;
    case 0x53B9: goto L53B9;
    case 0x53BD: goto L53BD;
    case 0x53C1: goto L53C1;
    case 0x53C3: goto L53C3;
    case 0x53C7: goto L53C7;
    case 0x53C9: goto L53C9;
    case 0x53CB: goto L53CB;
    case 0x53CD: goto L53CD;
    case 0x53D1: goto L53D1;
    case 0x53D3: goto L53D3;
    case 0x53D5: goto L53D5;
    case 0x53D7: goto L53D7;
    case 0x53DB: goto L53DB;
    case 0x53DD: goto L53DD;
    case 0x53DF: goto L53DF;
    case 0x53E3: goto L53E3;
    case 0x53E7: goto L53E7;
    case 0x53E8: goto L53E8;
    case 0x53E9: goto L53E9;
    case 0x53EC: goto L53EC;
    case 0x53F0: goto L53F0;
    case 0x53F2: goto L53F2;
    case 0x53F4: goto L53F4;
    case 0x53F7: goto L53F7;
    case 0x53FB: goto L53FB;
    case 0x53FD: goto L53FD;
    case 0x53FF: goto L53FF;
    case 0x5401: goto L5401;
    case 0x5403: goto L5403;
    case 0x5406: goto L5406;
    case 0x540A: goto L540A;
    case 0x540E: goto L540E;
    case 0x5410: goto L5410;
    case 0x5412: goto L5412;
    case 0x5415: goto L5415;
    case 0x5419: goto L5419;
    case 0x541B: goto L541B;
    case 0x541D: goto L541D;
    case 0x541F: goto L541F;
    case 0x5421: goto L5421;
    case 0x5425: goto L5425;
    case 0x5426: goto L5426;
    case 0x5428: goto L5428;
    case 0x542A: goto L542A;
    case 0x542F: goto L542F;
    case 0x5431: goto L5431;
    case 0x5433: goto L5433;
    case 0x5436: goto L5436;
    case 0x543A: goto L543A;
    case 0x543C: goto L543C;
    case 0x543F: goto L543F;
    case 0x5442: goto L5442;
    case 0x5445: goto L5445;
    case 0x5449: goto L5449;
    case 0x544B: goto L544B;
    case 0x544D: goto L544D;
    case 0x5450: goto L5450;
    case 0x5454: goto L5454;
    case 0x5456: goto L5456;
    case 0x5458: goto L5458;
    case 0x545A: goto L545A;
    case 0x545C: goto L545C;
    case 0x545E: goto L545E;
    case 0x5460: goto L5460;
    case 0x5463: goto L5463;
    case 0x5465: goto L5465;
    case 0x5468: goto L5468;
    case 0x546C: goto L546C;
    case 0x546F: goto L546F;
    case 0x5473: goto L5473;
    case 0x5475: goto L5475;
    case 0x5477: goto L5477;
    case 0x547A: goto L547A;
    case 0x547E: goto L547E;
    case 0x5480: goto L5480;
    case 0x5482: goto L5482;
    case 0x5484: goto L5484;
    case 0x5486: goto L5486;
    case 0x5488: goto L5488;
    case 0x548A: goto L548A;
    case 0x548D: goto L548D;
    case 0x548F: goto L548F;
    case 0x5492: goto L5492;
    case 0x5496: goto L5496;
    case 0x5499: goto L5499;
    case 0x549D: goto L549D;
    case 0x549F: goto L549F;
    case 0x54A1: goto L54A1;
    case 0x54A4: goto L54A4;
    case 0x54A8: goto L54A8;
    case 0x54AA: goto L54AA;
    case 0x54AC: goto L54AC;
    case 0x54AE: goto L54AE;
    case 0x54B0: goto L54B0;
    case 0x54B2: goto L54B2;
    case 0x54B4: goto L54B4;
    case 0x54B7: goto L54B7;
    case 0x54B9: goto L54B9;
    case 0x54BC: goto L54BC;
    case 0x54C0: goto L54C0;
    case 0x54C3: goto L54C3;
    case 0x54C7: goto L54C7;
    case 0x54C9: goto L54C9;
    case 0x54CB: goto L54CB;
    case 0x54CE: goto L54CE;
    case 0x54D2: goto L54D2;
    case 0x54D4: goto L54D4;
    case 0x54D6: goto L54D6;
    case 0x54D8: goto L54D8;
    case 0x54DA: goto L54DA;
    case 0x54DC: goto L54DC;
    case 0x54DE: goto L54DE;
    case 0x54E1: goto L54E1;
    case 0x54E3: goto L54E3;
    case 0x54E6: goto L54E6;
    case 0x54EA: goto L54EA;
    case 0x54ED: goto L54ED;
    case 0x54F1: goto L54F1;
    case 0x54F3: goto L54F3;
    case 0x54F5: goto L54F5;
    case 0x54F8: goto L54F8;
    case 0x54FC: goto L54FC;
    case 0x54FE: goto L54FE;
    case 0x5500: goto L5500;
    case 0x5502: goto L5502;
    case 0x5504: goto L5504;
    case 0x5506: goto L5506;
    case 0x5508: goto L5508;
    case 0x550B: goto L550B;
    case 0x550D: goto L550D;
    case 0x5510: goto L5510;
    case 0x5514: goto L5514;
    case 0x5517: goto L5517;
    case 0x551B: goto L551B;
    case 0x551D: goto L551D;
    case 0x551F: goto L551F;
    case 0x5522: goto L5522;
    case 0x5526: goto L5526;
    case 0x5528: goto L5528;
    case 0x552A: goto L552A;
    case 0x552C: goto L552C;
    case 0x552E: goto L552E;
    case 0x5530: goto L5530;
    case 0x5532: goto L5532;
    case 0x5535: goto L5535;
    case 0x5537: goto L5537;
    case 0x553A: goto L553A;
    case 0x553E: goto L553E;
    case 0x5541: goto L5541;
    case 0x5544: goto L5544;
    case 0x5547: goto L5547;
    case 0x554A: goto L554A;
    case 0x554D: goto L554D;
    case 0x5550: goto L5550;
    case 0x5551: goto L5551;
    case 0x5554: goto L5554;
    case 0x5558: goto L5558;
    case 0x555A: goto L555A;
    case 0x555C: goto L555C;
    case 0x555F: goto L555F;
    case 0x5563: goto L5563;
    case 0x5565: goto L5565;
    case 0x5567: goto L5567;
    case 0x5569: goto L5569;
    case 0x556B: goto L556B;
    case 0x556E: goto L556E;
    case 0x5572: goto L5572;
    case 0x5576: goto L5576;
    case 0x5578: goto L5578;
    case 0x557A: goto L557A;
    case 0x557D: goto L557D;
    case 0x5581: goto L5581;
    case 0x5583: goto L5583;
    case 0x5585: goto L5585;
    case 0x5587: goto L5587;
    case 0x5589: goto L5589;
    case 0x558D: goto L558D;
    case 0x558E: goto L558E;
    case 0x5590: goto L5590;
    case 0x5592: goto L5592;
    case 0x5597: goto L5597;
    case 0x5599: goto L5599;
    case 0x559B: goto L559B;
    case 0x559E: goto L559E;
    case 0x55A2: goto L55A2;
    case 0x55A4: goto L55A4;
    case 0x55A7: goto L55A7;
    case 0x55AA: goto L55AA;
    case 0x55AD: goto L55AD;
    case 0x55B1: goto L55B1;
    case 0x55B3: goto L55B3;
    case 0x55B5: goto L55B5;
    case 0x55B8: goto L55B8;
    case 0x55BC: goto L55BC;
    case 0x55BE: goto L55BE;
    case 0x55C0: goto L55C0;
    case 0x55C2: goto L55C2;
    case 0x55C4: goto L55C4;
    case 0x55C6: goto L55C6;
    case 0x55C8: goto L55C8;
    case 0x55CB: goto L55CB;
    case 0x55CD: goto L55CD;
    case 0x55D0: goto L55D0;
    case 0x55D4: goto L55D4;
    case 0x55D7: goto L55D7;
    case 0x55DB: goto L55DB;
    case 0x55DD: goto L55DD;
    case 0x55DF: goto L55DF;
    case 0x55E2: goto L55E2;
    case 0x55E6: goto L55E6;
    case 0x55E8: goto L55E8;
    case 0x55EA: goto L55EA;
    case 0x55EC: goto L55EC;
    case 0x55EE: goto L55EE;
    case 0x55F0: goto L55F0;
    case 0x55F2: goto L55F2;
    case 0x55F5: goto L55F5;
    case 0x55F7: goto L55F7;
    case 0x55FA: goto L55FA;
    case 0x55FE: goto L55FE;
    case 0x5601: goto L5601;
    case 0x5605: goto L5605;
    case 0x5607: goto L5607;
    case 0x5609: goto L5609;
    case 0x560C: goto L560C;
    case 0x5610: goto L5610;
    case 0x5612: goto L5612;
    case 0x5614: goto L5614;
    case 0x5616: goto L5616;
    case 0x5618: goto L5618;
    case 0x561A: goto L561A;
    case 0x561C: goto L561C;
    case 0x561F: goto L561F;
    case 0x5621: goto L5621;
    case 0x5624: goto L5624;
    case 0x5628: goto L5628;
    case 0x562B: goto L562B;
    case 0x562F: goto L562F;
    case 0x5631: goto L5631;
    case 0x5633: goto L5633;
    case 0x5636: goto L5636;
    case 0x563A: goto L563A;
    case 0x563C: goto L563C;
    case 0x563E: goto L563E;
    case 0x5640: goto L5640;
    case 0x5642: goto L5642;
    case 0x5644: goto L5644;
    case 0x5646: goto L5646;
    case 0x5649: goto L5649;
    case 0x564B: goto L564B;
    case 0x564E: goto L564E;
    case 0x5652: goto L5652;
    case 0x5655: goto L5655;
    case 0x5659: goto L5659;
    case 0x565B: goto L565B;
    case 0x565D: goto L565D;
    case 0x5660: goto L5660;
    case 0x5664: goto L5664;
    case 0x5666: goto L5666;
    case 0x5668: goto L5668;
    case 0x566A: goto L566A;
    case 0x566C: goto L566C;
    case 0x566E: goto L566E;
    case 0x5670: goto L5670;
    case 0x5673: goto L5673;
    case 0x5675: goto L5675;
    case 0x5678: goto L5678;
    case 0x567C: goto L567C;
    case 0x567F: goto L567F;
    case 0x5683: goto L5683;
    case 0x5685: goto L5685;
    case 0x5687: goto L5687;
    case 0x568A: goto L568A;
    case 0x568E: goto L568E;
    case 0x5690: goto L5690;
    case 0x5692: goto L5692;
    case 0x5694: goto L5694;
    case 0x5696: goto L5696;
    case 0x5698: goto L5698;
    case 0x569A: goto L569A;
    case 0x569D: goto L569D;
    case 0x569F: goto L569F;
    case 0x56A2: goto L56A2;
    case 0x56A6: goto L56A6;
    case 0x56A9: goto L56A9;
    case 0x56AC: goto L56AC;
    case 0x56AF: goto L56AF;
    case 0x56B2: goto L56B2;
    case 0x56B5: goto L56B5;
    case 0x56B8: goto L56B8;
    case 0x56B9: goto L56B9;
    case 0x56BC: goto L56BC;
    case 0x56C0: goto L56C0;
    case 0x56C2: goto L56C2;
    case 0x56C4: goto L56C4;
    case 0x56C7: goto L56C7;
    case 0x56CB: goto L56CB;
    case 0x56CD: goto L56CD;
    case 0x56CF: goto L56CF;
    case 0x56D1: goto L56D1;
    case 0x56D3: goto L56D3;
    case 0x56D6: goto L56D6;
    case 0x56DA: goto L56DA;
    case 0x56DE: goto L56DE;
    case 0x56E0: goto L56E0;
    case 0x56E2: goto L56E2;
    case 0x56E5: goto L56E5;
    case 0x56E9: goto L56E9;
    case 0x56EB: goto L56EB;
    case 0x56ED: goto L56ED;
    case 0x56EF: goto L56EF;
    case 0x56F1: goto L56F1;
    case 0x56F5: goto L56F5;
    case 0x56F6: goto L56F6;
    case 0x56F8: goto L56F8;
    case 0x56FA: goto L56FA;
    case 0x56FF: goto L56FF;
    case 0x5701: goto L5701;
    case 0x5703: goto L5703;
    case 0x5706: goto L5706;
    case 0x570A: goto L570A;
    case 0x570C: goto L570C;
    case 0x570F: goto L570F;
    case 0x5712: goto L5712;
    case 0x5715: goto L5715;
    case 0x5719: goto L5719;
    case 0x571B: goto L571B;
    case 0x571D: goto L571D;
    case 0x5720: goto L5720;
    case 0x5724: goto L5724;
    case 0x5726: goto L5726;
    case 0x5728: goto L5728;
    case 0x572A: goto L572A;
    case 0x572C: goto L572C;
    case 0x572E: goto L572E;
    case 0x5730: goto L5730;
    case 0x5733: goto L5733;
    case 0x5735: goto L5735;
    case 0x5738: goto L5738;
    case 0x573C: goto L573C;
    case 0x573F: goto L573F;
    case 0x5743: goto L5743;
    case 0x5745: goto L5745;
    case 0x5747: goto L5747;
    case 0x574A: goto L574A;
    case 0x574E: goto L574E;
    case 0x5750: goto L5750;
    case 0x5752: goto L5752;
    case 0x5754: goto L5754;
    case 0x5756: goto L5756;
    case 0x5758: goto L5758;
    case 0x575A: goto L575A;
    case 0x575D: goto L575D;
    case 0x575F: goto L575F;
    case 0x5762: goto L5762;
    case 0x5766: goto L5766;
    case 0x5769: goto L5769;
    case 0x576D: goto L576D;
    case 0x576F: goto L576F;
    case 0x5771: goto L5771;
    case 0x5774: goto L5774;
    case 0x5778: goto L5778;
    case 0x577A: goto L577A;
    case 0x577C: goto L577C;
    case 0x577E: goto L577E;
    case 0x5780: goto L5780;
    case 0x5782: goto L5782;
    case 0x5784: goto L5784;
    case 0x5787: goto L5787;
    case 0x5789: goto L5789;
    case 0x578C: goto L578C;
    case 0x5790: goto L5790;
    case 0x5793: goto L5793;
    case 0x5797: goto L5797;
    case 0x5799: goto L5799;
    case 0x579B: goto L579B;
    case 0x579E: goto L579E;
    case 0x57A2: goto L57A2;
    case 0x57A4: goto L57A4;
    case 0x57A6: goto L57A6;
    case 0x57A8: goto L57A8;
    case 0x57AA: goto L57AA;
    case 0x57AC: goto L57AC;
    case 0x57AE: goto L57AE;
    case 0x57B1: goto L57B1;
    case 0x57B3: goto L57B3;
    case 0x57B6: goto L57B6;
    case 0x57BA: goto L57BA;
    case 0x57BD: goto L57BD;
    case 0x57C1: goto L57C1;
    case 0x57C3: goto L57C3;
    case 0x57C5: goto L57C5;
    case 0x57C8: goto L57C8;
    case 0x57CC: goto L57CC;
    case 0x57CE: goto L57CE;
    case 0x57D0: goto L57D0;
    case 0x57D2: goto L57D2;
    case 0x57D4: goto L57D4;
    case 0x57D6: goto L57D6;
    case 0x57D8: goto L57D8;
    case 0x57DB: goto L57DB;
    case 0x57DD: goto L57DD;
    case 0x57E0: goto L57E0;
    case 0x57E4: goto L57E4;
    case 0x57E7: goto L57E7;
    case 0x57EB: goto L57EB;
    case 0x57ED: goto L57ED;
    case 0x57EF: goto L57EF;
    case 0x57F2: goto L57F2;
    case 0x57F6: goto L57F6;
    case 0x57F8: goto L57F8;
    case 0x57FA: goto L57FA;
    case 0x57FC: goto L57FC;
    case 0x57FE: goto L57FE;
    case 0x5800: goto L5800;
    case 0x5802: goto L5802;
    case 0x5805: goto L5805;
    case 0x5807: goto L5807;
    case 0x580A: goto L580A;
    case 0x580E: goto L580E;
    case 0x5811: goto L5811;
    case 0x5814: goto L5814;
    case 0x5817: goto L5817;
    case 0x581A: goto L581A;
    case 0x581D: goto L581D;
    case 0x5820: goto L5820;
    case 0x5821: goto L5821;
    case 0x5824: goto L5824;
    case 0x5827: goto L5827;
    case 0x582B: goto L582B;
    case 0x582F: goto L582F;
    case 0x5833: goto L5833;
    case 0x5834: goto L5834;
    case 0x5835: goto L5835;
    case 0x5838: goto L5838;
    case 0x583A: goto L583A;
    case 0x583B: goto L583B;
    case 0x583C: goto L583C;
    case 0x583E: goto L583E;
    case 0x5840: goto L5840;
    case 0x5843: goto L5843;
    case 0x5848: goto L5848;
    case 0x584D: goto L584D;
    case 0x5852: goto L5852;
    case 0x5855: goto L5855;
    case 0x5858: goto L5858;
    case 0x585D: goto L585D;
    case 0x5862: goto L5862;
    case 0x5867: goto L5867;
    case 0x586A: goto L586A;
    case 0x586D: goto L586D;
    case 0x5872: goto L5872;
    case 0x5877: goto L5877;
    case 0x587C: goto L587C;
    case 0x587D: goto L587D;
    case 0x587E: goto L587E;
    case 0x5880: goto L5880;
    case 0x5882: goto L5882;
    case 0x5884: goto L5884;
    case 0x5886: goto L5886;
    case 0x5888: goto L5888;
    case 0x588A: goto L588A;
    case 0x588C: goto L588C;
    case 0x588E: goto L588E;
    case 0x5890: goto L5890;
    case 0x5892: goto L5892;
    case 0x5894: goto L5894;
    case 0x5896: goto L5896;
    case 0x5897: goto L5897;
    case 0x5899: goto L5899;
    case 0x589B: goto L589B;
    case 0x589D: goto L589D;
    case 0x589F: goto L589F;
    case 0x58A1: goto L58A1;
    case 0x58A2: goto L58A2;
    case 0x58A4: goto L58A4;
    case 0x58A5: goto L58A5;
    case 0x58A7: goto L58A7;
    case 0x58A8: goto L58A8;
    case 0x58AA: goto L58AA;
    case 0x58AC: goto L58AC;
    case 0x58AE: goto L58AE;
    case 0x58B0: goto L58B0;
    case 0x58B2: goto L58B2;
    case 0x58B3: goto L58B3;
    case 0x58B5: goto L58B5;
    case 0x58B6: goto L58B6;
    case 0x58B8: goto L58B8;
    case 0x58B9: goto L58B9;
    case 0x58BB: goto L58BB;
    case 0x58BC: goto L58BC;
    case 0x58BE: goto L58BE;
    case 0x58BF: goto L58BF;
    case 0x58C1: goto L58C1;
    case 0x58C3: goto L58C3;
    case 0x58C5: goto L58C5;
    case 0x58C7: goto L58C7;
    case 0x58C9: goto L58C9;
    case 0x58CA: goto L58CA;
    case 0x58CC: goto L58CC;
    case 0x58CE: goto L58CE;
    case 0x58D0: goto L58D0;
    case 0x58D2: goto L58D2;
    case 0x58D4: goto L58D4;
    case 0x58D5: goto L58D5;
    case 0x58D7: goto L58D7;
    case 0x58D9: goto L58D9;
    case 0x58DB: goto L58DB;
    case 0x58DC: goto L58DC;
    case 0x58DE: goto L58DE;
    case 0x58E0: goto L58E0;
    case 0x58E2: goto L58E2;
    case 0x58E3: goto L58E3;
    case 0x58E5: goto L58E5;
    case 0x58E6: goto L58E6;
    case 0x58E8: goto L58E8;
    case 0x58EA: goto L58EA;
    case 0x58EC: goto L58EC;
    case 0x58ED: goto L58ED;
    case 0x58EF: goto L58EF;
    case 0x58F0: goto L58F0;
    case 0x58F2: goto L58F2;
    case 0x58F4: goto L58F4;
    case 0x58F6: goto L58F6;
    case 0x58F7: goto L58F7;
    case 0x58F9: goto L58F9;
    case 0x58FA: goto L58FA;
    case 0x58FD: goto L58FD;
    case 0x5902: goto L5902;
    case 0x5907: goto L5907;
    case 0x590C: goto L590C;
    case 0x590F: goto L590F;
    case 0x5912: goto L5912;
    case 0x5917: goto L5917;
    case 0x591C: goto L591C;
    case 0x5921: goto L5921;
    case 0x5924: goto L5924;
    case 0x5927: goto L5927;
    case 0x592C: goto L592C;
    case 0x5931: goto L5931;
    case 0x5936: goto L5936;
    case 0x5937: goto L5937;
    case 0x5939: goto L5939;
    case 0x593C: goto L593C;
    case 0x593E: goto L593E;
    case 0x5940: goto L5940;
    case 0x5943: goto L5943;
    case 0x5946: goto L5946;
    case 0x5948: goto L5948;
    case 0x594A: goto L594A;
    case 0x594D: goto L594D;
    case 0x5950: goto L5950;
    case 0x5952: goto L5952;
    case 0x5954: goto L5954;
    case 0x5956: goto L5956;
    case 0x5958: goto L5958;
    case 0x595A: goto L595A;
    case 0x595C: goto L595C;
    case 0x595D: goto L595D;
    case 0x5960: goto L5960;
    case 0x5962: goto L5962;
    case 0x5964: goto L5964;
    case 0x5968: goto L5968;
    case 0x596A: goto L596A;
    case 0x596D: goto L596D;
    case 0x596E: goto L596E;
    case 0x5970: goto L5970;
    case 0x5973: goto L5973;
    case 0x5975: goto L5975;
    case 0x5977: goto L5977;
    case 0x597A: goto L597A;
    case 0x597D: goto L597D;
    case 0x597F: goto L597F;
    case 0x5981: goto L5981;
    case 0x5984: goto L5984;
    case 0x5987: goto L5987;
    case 0x5989: goto L5989;
    case 0x598B: goto L598B;
    case 0x598D: goto L598D;
    case 0x598F: goto L598F;
    case 0x5991: goto L5991;
    case 0x5993: goto L5993;
    case 0x5994: goto L5994;
    case 0x5997: goto L5997;
    case 0x5999: goto L5999;
    case 0x599B: goto L599B;
    case 0x599F: goto L599F;
    case 0x59A1: goto L59A1;
    case 0x59A4: goto L59A4;
    case 0x59A6: goto L59A6;
    case 0x59A9: goto L59A9;
    case 0x59AB: goto L59AB;
    case 0x59AD: goto L59AD;
    case 0x59B0: goto L59B0;
    case 0x59B3: goto L59B3;
    case 0x59B5: goto L59B5;
    case 0x59B7: goto L59B7;
    case 0x59BA: goto L59BA;
    case 0x59BD: goto L59BD;
    case 0x59BF: goto L59BF;
    case 0x59C1: goto L59C1;
    case 0x59C3: goto L59C3;
    case 0x59C5: goto L59C5;
    case 0x59C7: goto L59C7;
    case 0x59C9: goto L59C9;
    case 0x59CA: goto L59CA;
    case 0x59CD: goto L59CD;
    case 0x59CF: goto L59CF;
    case 0x59D1: goto L59D1;
    case 0x59D5: goto L59D5;
    case 0x59D7: goto L59D7;
    case 0x59DA: goto L59DA;
    case 0x59DB: goto L59DB;
    case 0x59DC: goto L59DC;
    case 0x59DF: goto L59DF;
    case 0x59E1: goto L59E1;
    case 0x59E3: goto L59E3;
    case 0x59E5: goto L59E5;
    case 0x59E8: goto L59E8;
    case 0x59EB: goto L59EB;
    case 0x59ED: goto L59ED;
    case 0x59EF: goto L59EF;
    case 0x59F2: goto L59F2;
    case 0x59F5: goto L59F5;
    case 0x59F7: goto L59F7;
    case 0x59F9: goto L59F9;
    case 0x59FB: goto L59FB;
    case 0x59FD: goto L59FD;
    case 0x59FF: goto L59FF;
    case 0x5A01: goto L5A01;
    case 0x5A02: goto L5A02;
    case 0x5A05: goto L5A05;
    case 0x5A07: goto L5A07;
    case 0x5A09: goto L5A09;
    case 0x5A0D: goto L5A0D;
    case 0x5A0F: goto L5A0F;
    case 0x5A12: goto L5A12;
    case 0x5A13: goto L5A13;
    case 0x5A16: goto L5A16;
    case 0x5A19: goto L5A19;
    case 0x5A1B: goto L5A1B;
    case 0x5A1D: goto L5A1D;
    case 0x5A20: goto L5A20;
    case 0x5A23: goto L5A23;
    case 0x5A25: goto L5A25;
    case 0x5A27: goto L5A27;
    case 0x5A2A: goto L5A2A;
    case 0x5A2D: goto L5A2D;
    case 0x5A2F: goto L5A2F;
    case 0x5A31: goto L5A31;
    case 0x5A33: goto L5A33;
    case 0x5A35: goto L5A35;
    case 0x5A37: goto L5A37;
    case 0x5A39: goto L5A39;
    case 0x5A3A: goto L5A3A;
    case 0x5A3D: goto L5A3D;
    case 0x5A3F: goto L5A3F;
    case 0x5A41: goto L5A41;
    case 0x5A45: goto L5A45;
    case 0x5A47: goto L5A47;
    case 0x5A4A: goto L5A4A;
    case 0x5A4D: goto L5A4D;
    case 0x5A50: goto L5A50;
    case 0x5A52: goto L5A52;
    case 0x5A54: goto L5A54;
    case 0x5A57: goto L5A57;
    case 0x5A5A: goto L5A5A;
    case 0x5A5C: goto L5A5C;
    case 0x5A5E: goto L5A5E;
    case 0x5A61: goto L5A61;
    case 0x5A64: goto L5A64;
    case 0x5A66: goto L5A66;
    case 0x5A68: goto L5A68;
    case 0x5A6A: goto L5A6A;
    case 0x5A6C: goto L5A6C;
    case 0x5A6E: goto L5A6E;
    case 0x5A70: goto L5A70;
    case 0x5A71: goto L5A71;
    case 0x5A74: goto L5A74;
    case 0x5A76: goto L5A76;
    case 0x5A78: goto L5A78;
    case 0x5A7C: goto L5A7C;
    case 0x5A7E: goto L5A7E;
    case 0x5A81: goto L5A81;
    case 0x5A82: goto L5A82;
    case 0x5A83: goto L5A83;
    case 0x5A85: goto L5A85;
    case 0x5A88: goto L5A88;
    case 0x5A8A: goto L5A8A;
    case 0x5A8C: goto L5A8C;
    case 0x5A8F: goto L5A8F;
    case 0x5A92: goto L5A92;
    case 0x5A94: goto L5A94;
    case 0x5A96: goto L5A96;
    case 0x5A99: goto L5A99;
    case 0x5A9C: goto L5A9C;
    case 0x5A9E: goto L5A9E;
    case 0x5AA0: goto L5AA0;
    case 0x5AA2: goto L5AA2;
    case 0x5AA4: goto L5AA4;
    case 0x5AA6: goto L5AA6;
    case 0x5AA8: goto L5AA8;
    case 0x5AA9: goto L5AA9;
    case 0x5AAC: goto L5AAC;
    case 0x5AAE: goto L5AAE;
    case 0x5AB0: goto L5AB0;
    case 0x5AB4: goto L5AB4;
    case 0x5AB6: goto L5AB6;
    case 0x5AB9: goto L5AB9;
    case 0x5ABA: goto L5ABA;
    case 0x5ABC: goto L5ABC;
    case 0x5ABF: goto L5ABF;
    case 0x5AC1: goto L5AC1;
    case 0x5AC3: goto L5AC3;
    case 0x5AC6: goto L5AC6;
    case 0x5AC9: goto L5AC9;
    case 0x5ACB: goto L5ACB;
    case 0x5ACD: goto L5ACD;
    case 0x5AD0: goto L5AD0;
    case 0x5AD3: goto L5AD3;
    case 0x5AD5: goto L5AD5;
    case 0x5AD7: goto L5AD7;
    case 0x5AD9: goto L5AD9;
    case 0x5ADB: goto L5ADB;
    case 0x5ADD: goto L5ADD;
    case 0x5ADF: goto L5ADF;
    case 0x5AE0: goto L5AE0;
    case 0x5AE3: goto L5AE3;
    case 0x5AE5: goto L5AE5;
    case 0x5AE7: goto L5AE7;
    case 0x5AEB: goto L5AEB;
    case 0x5AED: goto L5AED;
    case 0x5AF0: goto L5AF0;
    case 0x5AF2: goto L5AF2;
    case 0x5AF5: goto L5AF5;
    case 0x5AF7: goto L5AF7;
    case 0x5AF9: goto L5AF9;
    case 0x5AFC: goto L5AFC;
    case 0x5AFF: goto L5AFF;
    case 0x5B01: goto L5B01;
    case 0x5B03: goto L5B03;
    case 0x5B06: goto L5B06;
    case 0x5B09: goto L5B09;
    case 0x5B0B: goto L5B0B;
    case 0x5B0D: goto L5B0D;
    case 0x5B0F: goto L5B0F;
    case 0x5B11: goto L5B11;
    case 0x5B13: goto L5B13;
    case 0x5B15: goto L5B15;
    case 0x5B16: goto L5B16;
    case 0x5B19: goto L5B19;
    case 0x5B1B: goto L5B1B;
    case 0x5B1D: goto L5B1D;
    case 0x5B21: goto L5B21;
    case 0x5B23: goto L5B23;
    case 0x5B26: goto L5B26;
    case 0x5B27: goto L5B27;
    case 0x5B28: goto L5B28;
    case 0x5B2A: goto L5B2A;
    case 0x5B2D: goto L5B2D;
    case 0x5B2F: goto L5B2F;
    case 0x5B31: goto L5B31;
    case 0x5B34: goto L5B34;
    case 0x5B37: goto L5B37;
    case 0x5B39: goto L5B39;
    case 0x5B3B: goto L5B3B;
    case 0x5B3E: goto L5B3E;
    case 0x5B41: goto L5B41;
    case 0x5B43: goto L5B43;
    case 0x5B45: goto L5B45;
    case 0x5B47: goto L5B47;
    case 0x5B49: goto L5B49;
    case 0x5B4B: goto L5B4B;
    case 0x5B4D: goto L5B4D;
    case 0x5B4E: goto L5B4E;
    case 0x5B51: goto L5B51;
    case 0x5B53: goto L5B53;
    case 0x5B55: goto L5B55;
    case 0x5B59: goto L5B59;
    case 0x5B5B: goto L5B5B;
    case 0x5B5E: goto L5B5E;
    case 0x5B5F: goto L5B5F;
    case 0x5B61: goto L5B61;
    case 0x5B64: goto L5B64;
    case 0x5B66: goto L5B66;
    case 0x5B68: goto L5B68;
    case 0x5B6B: goto L5B6B;
    case 0x5B6E: goto L5B6E;
    case 0x5B70: goto L5B70;
    case 0x5B72: goto L5B72;
    case 0x5B75: goto L5B75;
    case 0x5B78: goto L5B78;
    case 0x5B7A: goto L5B7A;
    case 0x5B7C: goto L5B7C;
    case 0x5B7E: goto L5B7E;
    case 0x5B80: goto L5B80;
    case 0x5B82: goto L5B82;
    case 0x5B84: goto L5B84;
    case 0x5B85: goto L5B85;
    case 0x5B88: goto L5B88;
    case 0x5B8A: goto L5B8A;
    case 0x5B8C: goto L5B8C;
    case 0x5B90: goto L5B90;
    case 0x5B92: goto L5B92;
    case 0x5B95: goto L5B95;
    case 0x5B97: goto L5B97;
    case 0x5B9A: goto L5B9A;
    case 0x5B9C: goto L5B9C;
    case 0x5B9E: goto L5B9E;
    case 0x5BA1: goto L5BA1;
    case 0x5BA4: goto L5BA4;
    case 0x5BA6: goto L5BA6;
    case 0x5BA8: goto L5BA8;
    case 0x5BAB: goto L5BAB;
    case 0x5BAE: goto L5BAE;
    case 0x5BB0: goto L5BB0;
    case 0x5BB2: goto L5BB2;
    case 0x5BB4: goto L5BB4;
    case 0x5BB6: goto L5BB6;
    case 0x5BB8: goto L5BB8;
    case 0x5BBA: goto L5BBA;
    case 0x5BBB: goto L5BBB;
    case 0x5BBE: goto L5BBE;
    case 0x5BC0: goto L5BC0;
    case 0x5BC2: goto L5BC2;
    case 0x5BC6: goto L5BC6;
    case 0x5BC8: goto L5BC8;
    case 0x5BCB: goto L5BCB;
    case 0x5BCC: goto L5BCC;
    case 0x5BD1: goto L5BD1;
    case 0x5BD6: goto L5BD6;
    case 0x5BD8: goto L5BD8;
    case 0x5BDB: goto L5BDB;
    case 0x5BDE: goto L5BDE;
    case 0x5BE1: goto L5BE1;
    case 0x5BE3: goto L5BE3;
    case 0x5BE8: goto L5BE8;
    case 0x5BED: goto L5BED;
    case 0x5BEE: goto L5BEE;
    case 0x5BF3: goto L5BF3;
    case 0x5BF5: goto L5BF5;
    case 0x5BF9: goto L5BF9;
    case 0x5BFC: goto L5BFC;
    case 0x5BFE: goto L5BFE;
    case 0x5C03: goto L5C03;
    case 0x5C04: goto L5C04;
    case 0x5C09: goto L5C09;
    case 0x5C0B: goto L5C0B;
    case 0x5C0E: goto L5C0E;
    case 0x5C11: goto L5C11;
    case 0x5C16: goto L5C16;
    case 0x5C18: goto L5C18;
    case 0x5C1A: goto L5C1A;
    case 0x5C1B: goto L5C1B;
    case 0x5C1E: goto L5C1E;
    case 0x5C20: goto L5C20;
    case 0x5C22: goto L5C22;
    case 0x5C24: goto L5C24;
    default: asm_bad_entry("INSTANCE.ASM", entry);
    }

    /* seg004_4980  (+4980)
       seg004_4980: an opcode handler that breaks into the debugger (int 3) and goes on to the
       next opcode; probably the table's entry for unused opcodes. */
L4980: /* _seg004_4980 */
    /* 4980  int     3 */
    asm_halt_at(0x06E7, 0x4980, "int 3, the debugger break");
L4981:
    /* 4981  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L4982:
    /* 4982  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L4983:
    /* 4983  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_4987  (+4987)
       render_3d (FM name): draw the frame whose render database starts at DbEntry (161Ch). Called
       by cRender (src/sys/C3DENTRY.ASM) with SS on cRender's private stack in seg063, where it
       reads the view window: the copy of the clip window that set_the_window (VIDMODE.ASM) keeps
       at seg063:0AA0 (left, top, right, bottom). Works out scrw, scrh, biasx and biasy from it and
       patches biasx and biasy into the projection code; counts the frame (15FAh); builds the view
       matrix (create_matrix in CAMERA.ASM, then seg004_51A3, scale_matrix, check_flat).
       It then switches to a stack in seg048 (SP 5546h) with seg048:2CECh set to 0D2Dh, and calls
       seg019_C10, which only returns (SYSLIBP.ASM). Finally it restores the stack and runs the
       database: the first opcode at DbEntry is called through the opcode table, and the chain of
       handlers returns here at the end of the frame (do_eof). Out: DS = ES = SS; seg048:2CECh
       restored. (UW2's comment, with UW1's addresses.)
       UW1 only: on entry and again at the end, when any of the 255 bytes from seg048:0008 is
       nonzero, it copies seg048:658h (first set to 50h, at the end to 0C0h, when it is 0) to
       seg048:410Fh; it sets the words 2860h..286Ah and byte 286Ch first; after create_matrix it
       walks the records from 161Eh, calling seg004_17AB (SPHERE.ASM) for each up to the one at
       15Ch; after the frame it raises int 2 when 2860h is above 68h, calls seg004_520F unless
       286Ch is set, and calls the handler at es:[2732h + type] for each word from 2864h to 2868h
       (in seg053). */
L4987: /* _render_3d */
    /* 4987  push    es */
    push16(asm_es);
L4988:
    /* 4988  push    di */
    push16(DI);
L4989:
    /* 4989  push    ax */
    push16(AX);
L498A:
    /* 498A  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L498D:
    /* 498D  mov     es,ax */
    SET_ES(AX);
L498F:
    /* 498F  mov     ax,0 */
    AX = 0x0;
L4992:
    /* 4992  mov     cx,0FFh */
    CX = 0xFF;
L4995:
    /* 4995  mov     di,8 */
    DI = 0x8;
L4998:
    /* 4998  repe scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (!ZF) break; }
L499A:
    /* 499A  jcxz    L49AE */
    if (!CX) goto L49AE;
L499C:
    /* 499C  mov     al,byte ptr es:[658h] */
    AL = rb(pES, 0x658);
L49A0:
    /* 49A0  test    al,0FFh */
    logic8((uint8_t)(AL & 0xFF));
L49A2:
    /* 49A2  jne     short L49AA */
    if (!ZF) goto L49AA;
L49A4:
    /* 49A4  mov     al,50h */
    AL = 0x50;
L49A6:
    /* 49A6  mov     byte ptr es:[658h],al */
    wb(pES, 0x658, AL);
L49AA: /* L49AA */
    /* 49AA  mov     byte ptr es:[410Fh],al */
    wb(pES, 0x410F, AL);
L49AE: /* L49AE */
    /* 49AE  pop     ax */
    AX = pop16();
L49AF:
    /* 49AF  pop     di */
    DI = pop16();
L49B0:
    /* 49B0  pop     es */
    SET_ES(pop16());
L49B1:
    /* 49B1  mov     ax,seg seg051 */
    AX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L49B4:
    /* 49B4  mov     ds,ax */
    SET_DS(AX);
L49B6:
    /* 49B6  mov     es,ax */
    SET_ES(AX);
L49B8:
    /* 49B8  mov     word ptr ds:[2860h],0Eh */
    ww(pDS, 0x2860, 0xE);
L49BE:
    /* 49BE  mov     word ptr ds:[2864h],0 */
    ww(pDS, 0x2864, 0x0);
L49C4:
    /* 49C4  mov     word ptr ds:[2866h],6 */
    ww(pDS, 0x2866, 0x6);
L49CA:
    /* 49CA  mov     word ptr ds:[2868h],0 */
    ww(pDS, 0x2868, 0x0);
L49D0:
    /* 49D0  mov     word ptr ds:[286Ah],0Ch */
    ww(pDS, 0x286A, 0xC);
L49D6:
    /* 49D6  mov     byte ptr ds:[286Ch],0FFh */
    wb(pDS, 0x286C, 0xFF);
L49DB:
    /* 49DB  mov     ax,word ptr ss:[0AA4h] */
    AX = rw(pSS, 0xAA4);
L49DF:
    /* 49DF  sub     ax,word ptr ss:[0AA0h] */
    AX = (uint16_t)(AX - rw(pSS, 0xAA0));
L49E4:
    /* 49E4  inc     ax */
    AX = (uint16_t)(AX + 1);
L49E5:
    /* 49E5  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L49E7:
    /* 49E7  mov     word ptr ds:[PROJ_X],ax */
    ww(pDS, 0x26B2, AX);
L49EA:
    /* 49EA  mov     ax,word ptr ss:[0AA2h] */
    AX = rw(pSS, 0xAA2);
L49EE:
    /* 49EE  sub     ax,word ptr ss:[0AA6h] */
    AX = (uint16_t)(AX - rw(pSS, 0xAA6));
L49F3:
    /* 49F3  inc     ax */
    AX = (uint16_t)(AX + 1);
L49F4:
    /* 49F4  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L49F6:
    /* 49F6  mov     word ptr ds:[PROJ_Y],ax */
    ww(pDS, 0x26B0, AX);
L49F9:
    /* 49F9  mov     ax,word ptr ss:[0AA4h] */
    AX = rw(pSS, 0xAA4);
L49FD:
    /* 49FD  add     ax,word ptr ss:[0AA0h] */
    AX = (uint16_t)(AX + rw(pSS, 0xAA0));
L4A02:
    /* 4A02  inc     ax */
    AX = (uint16_t)(AX + 1);
L4A03:
    /* 4A03  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L4A05:
    /* 4A05  mov     word ptr ds:[CENTRE_X],ax */
    ww(pDS, 0x26B4, AX);
L4A08:
    /* 4A08  mov     word ptr cs:[2A5h],ax */
    ww(CODE004, 0x2A5, AX);
L4A0C:
    /* 4A0C  mov     word ptr cs:[313h],ax */
    ww(CODE004, 0x313, AX);
L4A10:
    /* 4A10  mov     word ptr cs:[modify_biasx+1],ax */
    ww(CODE004, 0x2EF3, AX);
L4A14:
    /* 4A14  mov     word ptr cs:[modify_bx2+1],ax */
    ww(CODE004, 0x2F54, AX);
L4A18:
    /* 4A18  mov     word ptr cs:[_seg004_21CE+1],ax */
    ww(CODE004, 0x21CF, AX);
L4A1C:
    /* 4A1C  mov     word ptr cs:[_seg004_2173+1],ax */
    ww(CODE004, 0x2174, AX);
L4A20:
    /* 4A20  mov     ax,word ptr ss:[0AA2h] */
    AX = rw(pSS, 0xAA2);
L4A24:
    /* 4A24  add     ax,word ptr ss:[0AA6h] */
    AX = (uint16_t)(AX + rw(pSS, 0xAA6));
L4A29:
    /* 4A29  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L4A2B:
    /* 4A2B  mov     word ptr ds:[CENTRE_Y],ax */
    ww(pDS, 0x26B6, AX);
L4A2E:
    /* 4A2E  mov     word ptr cs:[2B0h],ax */
    ww(CODE004, 0x2B0, AX);
L4A32:
    /* 4A32  mov     word ptr cs:[323h],ax */
    ww(CODE004, 0x323, AX);
L4A36:
    /* 4A36  mov     word ptr cs:[modify_biasy+1],ax */
    ww(CODE004, 0x2EFF, AX);
L4A3A:
    /* 4A3A  mov     word ptr cs:[modify_by2+1],ax */
    ww(CODE004, 0x2F65, AX);
L4A3E:
    /* 4A3E  mov     word ptr cs:[_seg004_21DA+1],ax */
    ww(CODE004, 0x21DB, AX);
L4A42:
    /* 4A42  mov     word ptr cs:[_seg004_2181+1],ax */
    ww(CODE004, 0x2182, AX);
L4A46:
    /* 4A46  inc     word ptr ds:[15FAh] */
    ww(pDS, 0x15FA, (uint16_t)(rw(pDS, 0x15FA) + 1));
L4A4A:
    /* 4A4A  mov     byte ptr ds:[OBJ_SHIFT],0 */
    wb(pDS, 0x2880, 0x0);
L4A4F:
    /* 4A4F  call    _create_matrix */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x0A71), 0x4A52)) != 0) return c;
L4A52:
    /* 4A52  call    _seg004_51A3 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x51A3), 0x4A55)) != 0) return c;
L4A55:
    /* 4A55  call    _scale_matrix */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x4B44), 0x4A58)) != 0) return c;
L4A58:
    /* 4A58  call    _check_flat */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x4E76), 0x4A5B)) != 0) return c;
L4A5B:
    /* 4A5B  mov     bp,word ptr ds:[161Eh] */
    BP = rw(pDS, 0x161E);
L4A5F: /* L4A5F */
    /* 4A5F  test    word ptr [bp],0FFFFh */
    logic16((uint16_t)(rw(pSS, BP) & 0xFFFF));
L4A64:
    /* 4A64  je      short L4A7B */
    if (ZF) goto L4A7B;
L4A66:
    /* 4A66  js      short L4A83 */
    if (SF) goto L4A83;
L4A68:
    /* 4A68  cmp     bp,word ptr ds:[15Ch] */
    sub16(BP, rw(pDS, 0x15C), 0);
L4A6C:
    /* 4A6C  je      short L4A7B */
    if (ZF) goto L4A7B;
L4A6E:
    /* 4A6E  add     bp,6 */
    BP = (uint16_t)(BP + 0x6);
L4A71:
    /* 4A71  push    bp */
    push16(BP);
L4A72:
    /* 4A72  call    _seg004_17AB */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x17AB), 0x4A75)) != 0) return c;
L4A75:
    /* 4A75  pop     bp */
    BP = pop16();
L4A76:
    /* 4A76  add     bp,word ptr [bp-4] */
    BP = (uint16_t)(BP + rw(pSS, BP + 0xFFFC));
L4A79:
    /* 4A79  jmp     L4A5F */
    goto L4A5F;
L4A7B: /* L4A7B */
    /* 4A7B  add     bp,word ptr [bp+2] */
    BP = (uint16_t)(BP + rw(pSS, BP + 0x2));
L4A7E:
    /* 4A7E  add     bp,6 */
    BP = (uint16_t)(BP + 0x6);
L4A81:
    /* 4A81  jmp     L4A5F */
    goto L4A5F;
L4A83: /* L4A83 */
    /* 4A83  mov     ax,ss */
    AX = asm_ss;
L4A85:
    /* 4A85  mov     word ptr ds:[15F2h],ax */
    ww(pDS, 0x15F2, AX);
L4A88:
    /* 4A88  mov     word ptr ds:[15F0h],sp */
    ww(pDS, 0x15F0, SP);
L4A8C:
    /* 4A8C  mov     si,seg seg048 */
    SI = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4A8F:
    /* 4A8F  mov     ds,si */
    SET_DS(SI);
L4A91:
    /* 4A91  mov     ax,word ptr ds:[2CECh] */
    AX = rw(pDS, 0x2CEC);
L4A94:
    /* 4A94  mov     word ptr cs:L4B42,ax */
    ww(CODE004, 0x4B42, AX);
L4A98:
    /* 4A98  mov     word ptr ds:[2CECh],0D2Dh */
    ww(pDS, 0x2CEC, 0xD2D);
L4A9E:
    /* 4A9E  mov     ax,seg seg063 */
    AX = (uint16_t)(0x5624 + PORT_LOAD_SEG);
L4AA1:
    /* 4AA1  mov     es,ax */
    SET_ES(AX);
L4AA3:
    /* 4AA3  mov     es,si */
    SET_ES(SI);
L4AA5:
    /* 4AA5  cli */
    ;
L4AA6:
    /* 4AA6  mov     ss,si */
    SET_SS(SI);
L4AA8:
    /* 4AA8  mov     sp,5546h */
    SP = 0x5546;
L4AAB:
    /* 4AAB  sti */
    ;
L4AAC:
    /* 4AAC  call    far ptr _seg019_C10 */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x0C10), 0x06E7 + PORT_LOAD_SEG, 0x4AB1)) != 0) return c;
L4AB1:
    /* 4AB1  mov     bp,seg seg051 */
    BP = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L4AB4:
    /* 4AB4  mov     ds,bp */
    SET_DS(BP);
L4AB6:
    /* 4AB6  mov     es,bp */
    SET_ES(BP);
L4AB8:
    /* 4AB8  cli */
    ;
L4AB9:
    /* 4AB9  mov     ss,word ptr ds:[15F2h] */
    SET_SS(rw(pDS, 0x15F2));
L4ABD:
    /* 4ABD  mov     sp,word ptr ds:[15F0h] */
    SP = rw(pDS, 0x15F0);
L4AC1:
    /* 4AC1  sti */
    ;
L4AC2:
    /* 4AC2  mov     si,word ptr ds:[161Ch] */
    SI = rw(pDS, 0x161C);
L4AC6:
    /* 4AC6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L4AC7:
    /* 4AC7  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L4AC8:
    /* 4AC8  call    word ptr [bx+OPCODE_TABLE] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX + 0x2738)), 0x4ACC)) != 0) return c;
L4ACC:
    /* 4ACC  mov     ax,word ptr ds:[2860h] */
    AX = rw(pDS, 0x2860);
L4ACF:
    /* 4ACF  cmp     ax,68h */
    sub16(AX, 0x68, 0);
L4AD2:
    /* 4AD2  jle     short L4AD7 */
    if (ZF || SF != OF) goto L4AD7;
L4AD4:
    /* 4AD4  int     2 */
    asm_halt_at(0x06E7, 0x4AD4, "int 2h, the debugger break");
L4AD6:
    /* 4AD6  nop */
    ;
L4AD7: /* L4AD7 */
    /* 4AD7  push    ds */
    push16(asm_ds);
L4AD8:
    /* 4AD8  push    si */
    push16(SI);
L4AD9:
    /* 4AD9  test    byte ptr ds:[286Ch],0FFh */
    logic8((uint8_t)(rb(pDS, 0x286C) & 0xFF));
L4ADE:
    /* 4ADE  jne     short L4AE3 */
    if (!ZF) goto L4AE3;
L4AE0:
    /* 4AE0  call    _seg004_520F */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x520F), 0x4AE3)) != 0) return c;
L4AE3: /* L4AE3 */
    /* 4AE3  mov     si,word ptr ds:[2864h] */
    SI = rw(pDS, 0x2864);
L4AE7:
    /* 4AE7  mov     cx,word ptr ds:[2868h] */
    CX = rw(pDS, 0x2868);
L4AEB:
    /* 4AEB  sub     cx,si */
    CX = (uint16_t)(CX - SI);
L4AED:
    /* 4AED  shr     cx,1 */
    CX = shr16(CX, 1);
L4AEF:
    /* 4AEF  je      short L4B03 */
    if (ZF) goto L4B03;
L4AF1:
    /* 4AF1  mov     ax,seg seg053 */
    AX = (uint16_t)(0x5471 + PORT_LOAD_SEG);
L4AF4:
    /* 4AF4  mov     ds,ax */
    SET_DS(AX);
L4AF6: /* L4AF6 */
    /* 4AF6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L4AF7:
    /* 4AF7  mov     di,ax */
    DI = AX;
L4AF9:
    /* 4AF9  mov     bx,word ptr [di+4] */
    BX = rw(pDS, DI + 0x4);
L4AFC:
    /* 4AFC  call    word ptr es:[bx+2732h] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pES, BX + 0x2732)), 0x4B01)) != 0) return c;
L4B01:
    /* 4B01  loop    L4AF6 */
    if (--CX) goto L4AF6;
L4B03: /* L4B03 */
    /* 4B03  pop     si */
    SI = pop16();
L4B04:
    /* 4B04  pop     ds */
    SET_DS(pop16());
L4B05:
    /* 4B05  push    es */
    push16(asm_es);
L4B06:
    /* 4B06  push    di */
    push16(DI);
L4B07:
    /* 4B07  push    ax */
    push16(AX);
L4B08:
    /* 4B08  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4B0B:
    /* 4B0B  mov     es,ax */
    SET_ES(AX);
L4B0D:
    /* 4B0D  mov     ax,0 */
    AX = 0x0;
L4B10:
    /* 4B10  mov     cx,0FFh */
    CX = 0xFF;
L4B13:
    /* 4B13  mov     di,8 */
    DI = 0x8;
L4B16:
    /* 4B16  repe scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (!ZF) break; }
L4B18:
    /* 4B18  jcxz    L4B2C */
    if (!CX) goto L4B2C;
L4B1A:
    /* 4B1A  mov     al,byte ptr es:[658h] */
    AL = rb(pES, 0x658);
L4B1E:
    /* 4B1E  test    al,0FFh */
    logic8((uint8_t)(AL & 0xFF));
L4B20:
    /* 4B20  jne     short L4B28 */
    if (!ZF) goto L4B28;
L4B22:
    /* 4B22  mov     al,0C0h */
    AL = 0xC0;
L4B24:
    /* 4B24  mov     byte ptr es:[658h],al */
    wb(pES, 0x658, AL);
L4B28: /* L4B28 */
    /* 4B28  mov     byte ptr es:[410Fh],al */
    wb(pES, 0x410F, AL);
L4B2C: /* L4B2C */
    /* 4B2C  pop     ax */
    AX = pop16();
L4B2D:
    /* 4B2D  pop     di */
    DI = pop16();
L4B2E:
    /* 4B2E  pop     es */
    SET_ES(pop16());
L4B2F:
    /* 4B2F  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L4B32:
    /* 4B32  mov     ds,ax */
    SET_DS(AX);
L4B34:
    /* 4B34  mov     ax,word ptr cs:L4B42 */
    AX = rw(CODE004, 0x4B42);
L4B38:
    /* 4B38  mov     word ptr ds:[2CECh],ax */
    ww(pDS, 0x2CEC, AX);
L4B3B:
    /* 4B3B  mov     ax,ss */
    AX = asm_ss;
L4B3D:
    /* 4B3D  mov     ds,ax */
    SET_DS(AX);
L4B3F:
    /* 4B3F  mov     es,ax */
    SET_ES(AX);
L4B41:
    /* 4B41  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg004_4B44  (+4B44)
       scale_matrix: fold the screen's shape and the zoom into the view matrix. Scales matrix entries
       by the ratio of the view's half width to half height (with 15F4, whose sign picks which axis
       is scaled; probably so x and y project to the square frustum CLIP.ASM's 45-degree planes
       assume) and by the zoom word 2726 (the depth column when it is positive, the other entries
       when negative), and works out 26CC/26CE, a unit horizontal direction on the screen, and from
       it 26D0/26D2, the axes do_scalebm (TMAPOPS.ASM) uses to place sprites facing the viewer.
       Installs _overflow_handler_reg for the divides: cRender points int 0 at seg063:05A0, which holds
       `jmp far cs:[4D5]`, and the far pointer at 4D5 (FM overflow_ptr) is into seg004, so writing
       its offset word selects the handler. Uses SquareRoot_seg019_A30 (IMATH.ASM). */
L4B44: /* _scale_matrix */
    /* 4B44  mov     word ptr ds:[2728h],7FFFh */
    ww(pDS, 0x2728, 0x7FFF);
L4B4A:
    /* 4B4A  mov     word ptr ds:[272Ah],7FFFh */
    ww(pDS, 0x272A, 0x7FFF);
L4B50:
    /* 4B50  mov     ax,word ptr ds:[MAT_YX] */
    AX = rw(pDS, 0x1608);
L4B53:
    /* 4B53  mov     word ptr ds:[1614h],ax */
    ww(pDS, 0x1614, AX);
L4B56:
    /* 4B56  mov     ax,word ptr ds:[MAT_YY] */
    AX = rw(pDS, 0x160A);
L4B59:
    /* 4B59  mov     word ptr ds:[1616h],ax */
    ww(pDS, 0x1616, AX);
L4B5C:
    /* 4B5C  mov     ax,word ptr ds:[MAT_YZ] */
    AX = rw(pDS, 0x160C);
L4B5F:
    /* 4B5F  mov     word ptr ds:[1618h],ax */
    ww(pDS, 0x1618, AX);
L4B62:
    /* 4B62  mov     bx,word ptr ds:[PROJ_Y] */
    BX = rw(pDS, 0x26B0);
L4B66:
    /* 4B66  mov     cx,word ptr ds:[PROJ_X] */
    CX = rw(pDS, 0x26B2);
L4B6A:
    /* 4B6A  mov     ax,word ptr ds:[15F4h] */
    AX = rw(pDS, 0x15F4);
L4B6D:
    /* 4B6D  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L4B6F:
    /* 4B6F  js      short L4B7B */
    if (SF) goto L4B7B;
L4B71:
    /* 4B71  imul    cx */
    imul16(CX);
L4B73:
    /* 4B73  shl     ax,1 */
    AX = shl16(AX, 1);
L4B75:
    /* 4B75  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4B77:
    /* 4B77  xor     cx,cx */
    CX = (uint16_t)(CX ^ CX);
L4B79:
    /* 4B79  jmp     short L4B8B */
    goto L4B8B;
L4B7B: /* L4B7B */
    /* 4B7B  neg     ax */
    AX = (uint16_t)-AX;
L4B7D:
    /* 4B7D  imul    bx */
    imul16(BX);
L4B7F:
    /* 4B7F  shl     ax,1 */
    AX = shl16(AX, 1);
L4B81:
    /* 4B81  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4B83:
    /* 4B83  mov     bx,dx */
    BX = DX;
L4B85:
    /* 4B85  mov     dx,cx */
    DX = CX;
L4B87:
    /* 4B87  mov     cx,ax */
    CX = AX;
L4B89:
    /* 4B89  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L4B8B: /* L4B8B */
    /* 4B8B  cmp     dx,bx */
    sub16(DX, BX, 0);
L4B8D:
    /* 4B8D  jne     short L4B92 */
    if (!ZF) goto L4B92;
L4B8F:
    /* 4B8F  jmp     L4C33 */
    goto L4C33;
L4B92: /* L4B92 */
    /* 4B92  jl      short L4BE6 */
    if (SF != OF) goto L4BE6;
L4B94:
    /* 4B94  xchg    dx,bx */
    { uint16_t t_ = BX;
    BX = DX;
    DX = t_; }
L4B96:
    /* 4B96  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L4B97:
    /* 4B97  shr     dx,1 */
    DX = shr16(DX, 1);
L4B99:
    /* 4B99  rcr     ax,1 */
    AX = rcr16(AX, 1);
L4B9B:
    /* 4B9B  idiv    bx */
    if (asm_idiv16(BX) && (c = asm_divfault(0x06E7, 0x4B9B, 2)) != 0) return c;
L4B9D:
    /* 4B9D  mov     cx,ax */
    CX = AX;
L4B9F:
    /* 4B9F  mov     word ptr ds:[2728h],cx */
    ww(pDS, 0x2728, CX);
L4BA3:
    /* 4BA3  mov     ax,word ptr ds:[MAT_XX] */
    AX = rw(pDS, 0x1602);
L4BA6:
    /* 4BA6  imul    cx */
    imul16(CX);
L4BA8:
    /* 4BA8  shl     ax,1 */
    AX = shl16(AX, 1);
L4BAA:
    /* 4BAA  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4BAC:
    /* 4BAC  mov     word ptr ds:[MAT_XX],dx */
    ww(pDS, 0x1602, DX);
L4BB0:
    /* 4BB0  mov     ax,word ptr ds:[MAT_YX] */
    AX = rw(pDS, 0x1608);
L4BB3:
    /* 4BB3  imul    cx */
    imul16(CX);
L4BB5:
    /* 4BB5  shl     ax,1 */
    AX = shl16(AX, 1);
L4BB7:
    /* 4BB7  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4BB9:
    /* 4BB9  mov     word ptr ds:[MAT_YX],dx */
    ww(pDS, 0x1608, DX);
L4BBD:
    /* 4BBD  mov     ax,word ptr ds:[MAT_ZX] */
    AX = rw(pDS, 0x160E);
L4BC0:
    /* 4BC0  imul    cx */
    imul16(CX);
L4BC2:
    /* 4BC2  shl     ax,1 */
    AX = shl16(AX, 1);
L4BC4:
    /* 4BC4  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4BC6:
    /* 4BC6  mov     word ptr ds:[MAT_ZX],dx */
    ww(pDS, 0x160E, DX);
L4BCA:
    /* 4BCA  mov     ax,word ptr ds:[1616h] */
    AX = rw(pDS, 0x1616);
L4BCD:
    /* 4BCD  imul    cx */
    imul16(CX);
L4BCF:
    /* 4BCF  shl     ax,1 */
    AX = shl16(AX, 1);
L4BD1:
    /* 4BD1  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4BD3:
    /* 4BD3  mov     word ptr ds:[1616h],dx */
    ww(pDS, 0x1616, DX);
L4BD7:
    /* 4BD7  mov     ax,word ptr ds:[1618h] */
    AX = rw(pDS, 0x1618);
L4BDA:
    /* 4BDA  imul    cx */
    imul16(CX);
L4BDC:
    /* 4BDC  shl     ax,1 */
    AX = shl16(AX, 1);
L4BDE:
    /* 4BDE  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4BE0:
    /* 4BE0  mov     word ptr ds:[1618h],dx */
    ww(pDS, 0x1618, DX);
L4BE4:
    /* 4BE4  jmp     short L4C33 */
    goto L4C33;
L4BE6: /* L4BE6 */
    /* 4BE6  shr     dx,1 */
    DX = shr16(DX, 1);
L4BE8:
    /* 4BE8  rcr     ax,1 */
    AX = rcr16(AX, 1);
L4BEA:
    /* 4BEA  idiv    bx */
    if (asm_idiv16(BX) && (c = asm_divfault(0x06E7, 0x4BEA, 2)) != 0) return c;
L4BEC:
    /* 4BEC  mov     cx,ax */
    CX = AX;
L4BEE:
    /* 4BEE  mov     word ptr ds:[272Ah],cx */
    ww(pDS, 0x272A, CX);
L4BF2:
    /* 4BF2  mov     ax,word ptr ds:[MAT_XY] */
    AX = rw(pDS, 0x1604);
L4BF5:
    /* 4BF5  imul    cx */
    imul16(CX);
L4BF7:
    /* 4BF7  shl     ax,1 */
    AX = shl16(AX, 1);
L4BF9:
    /* 4BF9  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4BFB:
    /* 4BFB  mov     word ptr ds:[MAT_XY],dx */
    ww(pDS, 0x1604, DX);
L4BFF:
    /* 4BFF  mov     ax,word ptr ds:[MAT_YY] */
    AX = rw(pDS, 0x160A);
L4C02:
    /* 4C02  imul    cx */
    imul16(CX);
L4C04:
    /* 4C04  shl     ax,1 */
    AX = shl16(AX, 1);
L4C06:
    /* 4C06  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4C08:
    /* 4C08  mov     word ptr ds:[MAT_YY],dx */
    ww(pDS, 0x160A, DX);
L4C0C:
    /* 4C0C  mov     ax,word ptr ds:[MAT_ZY] */
    AX = rw(pDS, 0x1610);
L4C0F:
    /* 4C0F  imul    cx */
    imul16(CX);
L4C11:
    /* 4C11  shl     ax,1 */
    AX = shl16(AX, 1);
L4C13:
    /* 4C13  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4C15:
    /* 4C15  mov     word ptr ds:[MAT_ZY],dx */
    ww(pDS, 0x1610, DX);
L4C19:
    /* 4C19  mov     ax,word ptr ds:[1614h] */
    AX = rw(pDS, 0x1614);
L4C1C:
    /* 4C1C  imul    cx */
    imul16(CX);
L4C1E:
    /* 4C1E  shl     ax,1 */
    AX = shl16(AX, 1);
L4C20:
    /* 4C20  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4C22:
    /* 4C22  mov     word ptr ds:[1614h],dx */
    ww(pDS, 0x1614, DX);
L4C26:
    /* 4C26  mov     ax,word ptr ds:[1618h] */
    AX = rw(pDS, 0x1618);
L4C29:
    /* 4C29  imul    cx */
    imul16(CX);
L4C2B:
    /* 4C2B  shl     ax,1 */
    AX = shl16(AX, 1);
L4C2D:
    /* 4C2D  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4C2F:
    /* 4C2F  mov     word ptr ds:[1618h],dx */
    ww(pDS, 0x1618, DX);
L4C33: /* L4C33 */
    /* 4C33  mov     cx,word ptr ds:[2726h] */
    CX = rw(pDS, 0x2726);
L4C37:
    /* 4C37  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L4C39:
    /* 4C39  jns     short L4C3E */
    if (!SF) goto L4C3E;
L4C3B:
    /* 4C3B  jmp     L4CC6 */
    goto L4CC6;
L4C3E: /* L4C3E */
    /* 4C3E  mov     word ptr ds:[2730h],cx */
    ww(pDS, 0x2730, CX);
L4C42:
    /* 4C42  mov     ax,word ptr ds:[MAT_XZ] */
    AX = rw(pDS, 0x1606);
L4C45:
    /* 4C45  imul    cx */
    imul16(CX);
L4C47:
    /* 4C47  shl     ax,1 */
    AX = shl16(AX, 1);
L4C49:
    /* 4C49  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4C4B:
    /* 4C4B  mov     word ptr ds:[MAT_XZ],dx */
    ww(pDS, 0x1606, DX);
L4C4F:
    /* 4C4F  mov     ax,word ptr ds:[MAT_YZ] */
    AX = rw(pDS, 0x160C);
L4C52:
    /* 4C52  imul    cx */
    imul16(CX);
L4C54:
    /* 4C54  shl     ax,1 */
    AX = shl16(AX, 1);
L4C56:
    /* 4C56  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4C58:
    /* 4C58  mov     word ptr ds:[MAT_YZ],dx */
    ww(pDS, 0x160C, DX);
L4C5C:
    /* 4C5C  mov     ax,word ptr ds:[MAT_ZZ] */
    AX = rw(pDS, 0x1612);
L4C5F:
    /* 4C5F  imul    cx */
    imul16(CX);
L4C61:
    /* 4C61  shl     ax,1 */
    AX = shl16(AX, 1);
L4C63:
    /* 4C63  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4C65:
    /* 4C65  mov     word ptr ds:[MAT_ZZ],dx */
    ww(pDS, 0x1612, DX);
L4C69:
    /* 4C69  mov     ax,word ptr ds:[1616h] */
    AX = rw(pDS, 0x1616);
L4C6C:
    /* 4C6C  imul    cx */
    imul16(CX);
L4C6E:
    /* 4C6E  shl     ax,1 */
    AX = shl16(AX, 1);
L4C70:
    /* 4C70  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4C72:
    /* 4C72  mov     word ptr ds:[1616h],dx */
    ww(pDS, 0x1616, DX);
L4C76:
    /* 4C76  mov     ax,word ptr ds:[1614h] */
    AX = rw(pDS, 0x1614);
L4C79:
    /* 4C79  imul    cx */
    imul16(CX);
L4C7B:
    /* 4C7B  shl     ax,1 */
    AX = shl16(AX, 1);
L4C7D:
    /* 4C7D  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4C7F:
    /* 4C7F  mov     word ptr ds:[1614h],dx */
    ww(pDS, 0x1614, DX);
L4C83:
    /* 4C83  mov     ax,cx */
    AX = CX;
L4C85:
    /* 4C85  imul    ax */
    imul16(AX);
L4C87:
    /* 4C87  mov     bp,ax */
    BP = AX;
L4C89:
    /* 4C89  mov     si,dx */
    SI = DX;
L4C8B:
    /* 4C8B  mov     ax,word ptr ds:[2728h] */
    AX = rw(pDS, 0x2728);
L4C8E:
    /* 4C8E  mov     word ptr ds:[272Ch],ax */
    ww(pDS, 0x272C, AX);
L4C91:
    /* 4C91  imul    ax */
    imul16(AX);
L4C93:
    /* 4C93  add     ax,bp */
    AX = add16(AX, BP, 0);
L4C95:
    /* 4C95  adc     dx,si */
    DX = (uint16_t)(DX + SI + CF);
L4C97:
    /* 4C97  mov     bx,dx */
    BX = DX;
L4C99:
    /* 4C99  xor     cx,cx */
    CX = (uint16_t)(CX ^ CX);
L4C9B:
    /* 4C9B  call    far ptr _SquareRoot_seg019_A30 */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x0A30), 0x06E7 + PORT_LOAD_SEG, 0x4CA0)) != 0) return c;
L4CA0:
    /* 4CA0  mov     ax,di */
    AX = DI;
L4CA2:
    /* 4CA2  xchg    ah,al */
    { uint8_t t_ = AL;
    AL = AH;
    AH = t_; }
L4CA4:
    /* 4CA4  mov     word ptr ds:[2728h],ax */
    ww(pDS, 0x2728, AX);
L4CA7:
    /* 4CA7  mov     ax,word ptr ds:[272Ah] */
    AX = rw(pDS, 0x272A);
L4CAA:
    /* 4CAA  mov     word ptr ds:[272Eh],ax */
    ww(pDS, 0x272E, AX);
L4CAD:
    /* 4CAD  imul    ax */
    imul16(AX);
L4CAF:
    /* 4CAF  add     ax,bp */
    AX = add16(AX, BP, 0);
L4CB1:
    /* 4CB1  adc     dx,si */
    DX = (uint16_t)(DX + SI + CF);
L4CB3:
    /* 4CB3  mov     bx,dx */
    BX = DX;
L4CB5:
    /* 4CB5  xor     cx,cx */
    CX = (uint16_t)(CX ^ CX);
L4CB7:
    /* 4CB7  call    far ptr _SquareRoot_seg019_A30 */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x0A30), 0x06E7 + PORT_LOAD_SEG, 0x4CBC)) != 0) return c;
L4CBC:
    /* 4CBC  mov     ax,di */
    AX = DI;
L4CBE:
    /* 4CBE  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L4CC0:
    /* 4CC0  mov     word ptr ds:[272Ah],ax */
    ww(pDS, 0x272A, AX);
L4CC3:
    /* 4CC3  jmp     L4D85 */
    goto L4D85;
L4CC6: /* L4CC6 */
    /* 4CC6  mov     word ptr ds:[2730h],7FFFh */
    ww(pDS, 0x2730, 0x7FFF);
L4CCC:
    /* 4CCC  neg     cx */
    CX = (uint16_t)-CX;
L4CCE:
    /* 4CCE  mov     ax,word ptr ds:[MAT_XX] */
    AX = rw(pDS, 0x1602);
L4CD1:
    /* 4CD1  imul    cx */
    imul16(CX);
L4CD3:
    /* 4CD3  shl     ax,1 */
    AX = shl16(AX, 1);
L4CD5:
    /* 4CD5  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4CD7:
    /* 4CD7  mov     word ptr ds:[MAT_XX],dx */
    ww(pDS, 0x1602, DX);
L4CDB:
    /* 4CDB  mov     ax,word ptr ds:[MAT_YX] */
    AX = rw(pDS, 0x1608);
L4CDE:
    /* 4CDE  imul    cx */
    imul16(CX);
L4CE0:
    /* 4CE0  shl     ax,1 */
    AX = shl16(AX, 1);
L4CE2:
    /* 4CE2  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4CE4:
    /* 4CE4  mov     word ptr ds:[MAT_YX],dx */
    ww(pDS, 0x1608, DX);
L4CE8:
    /* 4CE8  mov     ax,word ptr ds:[MAT_ZX] */
    AX = rw(pDS, 0x160E);
L4CEB:
    /* 4CEB  imul    cx */
    imul16(CX);
L4CED:
    /* 4CED  shl     ax,1 */
    AX = shl16(AX, 1);
L4CEF:
    /* 4CEF  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4CF1:
    /* 4CF1  mov     word ptr ds:[MAT_ZX],dx */
    ww(pDS, 0x160E, DX);
L4CF5:
    /* 4CF5  mov     ax,word ptr ds:[MAT_XY] */
    AX = rw(pDS, 0x1604);
L4CF8:
    /* 4CF8  imul    cx */
    imul16(CX);
L4CFA:
    /* 4CFA  shl     ax,1 */
    AX = shl16(AX, 1);
L4CFC:
    /* 4CFC  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4CFE:
    /* 4CFE  mov     word ptr ds:[MAT_XY],dx */
    ww(pDS, 0x1604, DX);
L4D02:
    /* 4D02  mov     ax,word ptr ds:[MAT_YY] */
    AX = rw(pDS, 0x160A);
L4D05:
    /* 4D05  imul    cx */
    imul16(CX);
L4D07:
    /* 4D07  shl     ax,1 */
    AX = shl16(AX, 1);
L4D09:
    /* 4D09  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4D0B:
    /* 4D0B  mov     word ptr ds:[MAT_YY],dx */
    ww(pDS, 0x160A, DX);
L4D0F:
    /* 4D0F  mov     ax,word ptr ds:[MAT_ZY] */
    AX = rw(pDS, 0x1610);
L4D12:
    /* 4D12  imul    cx */
    imul16(CX);
L4D14:
    /* 4D14  shl     ax,1 */
    AX = shl16(AX, 1);
L4D16:
    /* 4D16  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4D18:
    /* 4D18  mov     word ptr ds:[MAT_ZY],dx */
    ww(pDS, 0x1610, DX);
L4D1C:
    /* 4D1C  mov     ax,word ptr ds:[1618h] */
    AX = rw(pDS, 0x1618);
L4D1F:
    /* 4D1F  imul    cx */
    imul16(CX);
L4D21:
    /* 4D21  shl     ax,1 */
    AX = shl16(AX, 1);
L4D23:
    /* 4D23  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4D25:
    /* 4D25  mov     word ptr ds:[1618h],dx */
    ww(pDS, 0x1618, DX);
L4D29:
    /* 4D29  mov     si,cx */
    SI = CX;
L4D2B:
    /* 4D2B  mov     ax,word ptr ds:[2728h] */
    AX = rw(pDS, 0x2728);
L4D2E:
    /* 4D2E  imul    cx */
    imul16(CX);
L4D30:
    /* 4D30  shl     ax,1 */
    AX = shl16(AX, 1);
L4D32:
    /* 4D32  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4D34:
    /* 4D34  mov     bp,dx */
    BP = DX;
L4D36:
    /* 4D36  shl     ax,1 */
    AX = shl16(AX, 1);
L4D38:
    /* 4D38  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4D3A:
    /* 4D3A  mov     word ptr ds:[272Ch],dx */
    ww(pDS, 0x272C, DX);
L4D3E:
    /* 4D3E  mov     dx,bp */
    DX = BP;
L4D40:
    /* 4D40  mov     ax,dx */
    AX = DX;
L4D42:
    /* 4D42  imul    ax */
    imul16(AX);
L4D44:
    /* 4D44  mov     bx,dx */
    BX = DX;
L4D46:
    /* 4D46  add     bx,3FFFh */
    BX = (uint16_t)(BX + 0x3FFF);
L4D4A:
    /* 4D4A  xor     cx,cx */
    CX = (uint16_t)(CX ^ CX);
L4D4C:
    /* 4D4C  call    far ptr _SquareRoot_seg019_A30 */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x0A30), 0x06E7 + PORT_LOAD_SEG, 0x4D51)) != 0) return c;
L4D51:
    /* 4D51  mov     ax,di */
    AX = DI;
L4D53:
    /* 4D53  xchg    ah,al */
    { uint8_t t_ = AL;
    AL = AH;
    AH = t_; }
L4D55:
    /* 4D55  mov     word ptr ds:[2728h],ax */
    ww(pDS, 0x2728, AX);
L4D58:
    /* 4D58  mov     ax,word ptr ds:[272Ah] */
    AX = rw(pDS, 0x272A);
L4D5B:
    /* 4D5B  imul    si */
    imul16(SI);
L4D5D:
    /* 4D5D  shl     ax,1 */
    AX = shl16(AX, 1);
L4D5F:
    /* 4D5F  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4D61:
    /* 4D61  mov     bp,dx */
    BP = DX;
L4D63:
    /* 4D63  shl     ax,1 */
    AX = shl16(AX, 1);
L4D65:
    /* 4D65  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4D67:
    /* 4D67  mov     word ptr ds:[272Eh],dx */
    ww(pDS, 0x272E, DX);
L4D6B:
    /* 4D6B  mov     dx,bp */
    DX = BP;
L4D6D:
    /* 4D6D  mov     ax,dx */
    AX = DX;
L4D6F:
    /* 4D6F  imul    ax */
    imul16(AX);
L4D71:
    /* 4D71  mov     bx,dx */
    BX = DX;
L4D73:
    /* 4D73  add     bx,3FFFh */
    BX = (uint16_t)(BX + 0x3FFF);
L4D77:
    /* 4D77  xor     cx,cx */
    CX = (uint16_t)(CX ^ CX);
L4D79:
    /* 4D79  call    far ptr _SquareRoot_seg019_A30 */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x0A30), 0x06E7 + PORT_LOAD_SEG, 0x4D7E)) != 0) return c;
L4D7E:
    /* 4D7E  mov     ax,di */
    AX = DI;
L4D80:
    /* 4D80  xchg    al,ah */
    { uint8_t t_ = AH;
    AH = AL;
    AL = t_; }
L4D82:
    /* 4D82  mov     word ptr ds:[272Ah],ax */
    ww(pDS, 0x272A, AX);
L4D85: /* L4D85 */
    /* 4D85  mov     word ptr ss:[4D5h],offset _overflow_handler_reg */
    ww(pSS, 0x4D5, 0x5BD1);
L4D8C:
    /* 4D8C  mov     ax,word ptr ds:[MAT_XZ] */
    AX = rw(pDS, 0x1606);
L4D8F:
    /* 4D8F  imul    word ptr ds:[MAT_XZ] */
    imul16(rw(pDS, 0x1606));
L4D93:
    /* 4D93  mov     cx,dx */
    CX = DX;
L4D95:
    /* 4D95  mov     bx,ax */
    BX = AX;
L4D97:
    /* 4D97  mov     ax,word ptr ds:[MAT_ZZ] */
    AX = rw(pDS, 0x1612);
L4D9A:
    /* 4D9A  imul    word ptr ds:[MAT_ZZ] */
    imul16(rw(pDS, 0x1612));
L4D9E:
    /* 4D9E  add     bx,ax */
    BX = add16(BX, AX, 0);
L4DA0:
    /* 4DA0  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L4DA2:
    /* 4DA2  or      ch,ch */
    CH = logic8((uint8_t)(CH | CH));
L4DA4:
    /* 4DA4  je      short L4DD7 */
    if (ZF) goto L4DD7;
L4DA6:
    /* 4DA6  call    far ptr _SquareRoot_seg019_A30 */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x0A30), 0x06E7 + PORT_LOAD_SEG, 0x4DAB)) != 0) return c;
L4DAB:
    /* 4DAB  mov     dx,word ptr ds:[MAT_XZ] */
    DX = rw(pDS, 0x1606);
L4DAF:
    /* 4DAF  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L4DB1:
    /* 4DB1  sar     dx,1 */
    DX = sar16(DX, 1);
L4DB3:
    /* 4DB3  rcr     ax,1 */
    AX = rcr16(AX, 1);
L4DB5:
    /* 4DB5  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x06E7, 0x4DB5, 2)) != 0) return c;
L4DB7:
    /* 4DB7  cmp     ax,8000h */
    sub16(AX, 0x8000, 0);
L4DBA:
    /* 4DBA  jne     short L4DBD */
    if (!ZF) goto L4DBD;
L4DBC:
    /* 4DBC  inc     ax */
    AX = (uint16_t)(AX + 1);
L4DBD: /* L4DBD */
    /* 4DBD  mov     word ptr ds:[26CCh],ax */
    ww(pDS, 0x26CC, AX);
L4DC0:
    /* 4DC0  mov     dx,word ptr ds:[MAT_ZZ] */
    DX = rw(pDS, 0x1612);
L4DC4:
    /* 4DC4  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L4DC6:
    /* 4DC6  sar     dx,1 */
    DX = sar16(DX, 1);
L4DC8:
    /* 4DC8  rcr     ax,1 */
    AX = rcr16(AX, 1);
L4DCA:
    /* 4DCA  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x06E7, 0x4DCA, 2)) != 0) return c;
L4DCC:
    /* 4DCC  cmp     ax,8000h */
    sub16(AX, 0x8000, 0);
L4DCF:
    /* 4DCF  jne     short L4DD2 */
    if (!ZF) goto L4DD2;
L4DD1:
    /* 4DD1  inc     ax */
    AX = (uint16_t)(AX + 1);
L4DD2: /* L4DD2 */
    /* 4DD2  mov     word ptr ds:[26CEh],ax */
    ww(pDS, 0x26CE, AX);
L4DD5:
    /* 4DD5  jmp     short L4E2B */
    goto L4E2B;
L4DD7: /* L4DD7 */
    /* 4DD7  mov     ax,word ptr ds:[MAT_XY] */
    AX = rw(pDS, 0x1604);
L4DDA:
    /* 4DDA  imul    word ptr ds:[MAT_XY] */
    imul16(rw(pDS, 0x1604));
L4DDE:
    /* 4DDE  mov     cx,dx */
    CX = DX;
L4DE0:
    /* 4DE0  mov     bx,ax */
    BX = AX;
L4DE2:
    /* 4DE2  mov     ax,word ptr ds:[MAT_ZY] */
    AX = rw(pDS, 0x1610);
L4DE5:
    /* 4DE5  imul    word ptr ds:[MAT_ZY] */
    imul16(rw(pDS, 0x1610));
L4DE9:
    /* 4DE9  add     bx,ax */
    BX = add16(BX, AX, 0);
L4DEB:
    /* 4DEB  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L4DED:
    /* 4DED  call    far ptr _SquareRoot_seg019_A30 */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x0A30), 0x06E7 + PORT_LOAD_SEG, 0x4DF2)) != 0) return c;
L4DF2:
    /* 4DF2  mov     dx,word ptr ds:[MAT_XY] */
    DX = rw(pDS, 0x1604);
L4DF6:
    /* 4DF6  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L4DF8:
    /* 4DF8  sar     dx,1 */
    DX = sar16(DX, 1);
L4DFA:
    /* 4DFA  rcr     ax,1 */
    AX = rcr16(AX, 1);
L4DFC:
    /* 4DFC  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x06E7, 0x4DFC, 2)) != 0) return c;
L4DFE:
    /* 4DFE  cmp     ax,8000h */
    sub16(AX, 0x8000, 0);
L4E01:
    /* 4E01  jne     short L4E04 */
    if (!ZF) goto L4E04;
L4E03:
    /* 4E03  inc     ax */
    AX = (uint16_t)(AX + 1);
L4E04: /* L4E04 */
    /* 4E04  mov     bx,ax */
    BX = AX;
L4E06:
    /* 4E06  mov     dx,word ptr ds:[MAT_ZY] */
    DX = rw(pDS, 0x1610);
L4E0A:
    /* 4E0A  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L4E0C:
    /* 4E0C  sar     dx,1 */
    DX = sar16(DX, 1);
L4E0E:
    /* 4E0E  rcr     ax,1 */
    AX = rcr16(AX, 1);
L4E10:
    /* 4E10  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x06E7, 0x4E10, 2)) != 0) return c;
L4E12:
    /* 4E12  cmp     ax,8000h */
    sub16(AX, 0x8000, 0);
L4E15:
    /* 4E15  jne     short L4E18 */
    if (!ZF) goto L4E18;
L4E17:
    /* 4E17  inc     ax */
    AX = (uint16_t)(AX + 1);
L4E18: /* L4E18 */
    /* 4E18  test    word ptr ds:[MAT_YZ],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x160C) & 0xFFFF));
L4E1E:
    /* 4E1E  js      short L4E24 */
    if (SF) goto L4E24;
L4E20:
    /* 4E20  neg     bx */
    BX = (uint16_t)-BX;
L4E22:
    /* 4E22  neg     ax */
    AX = (uint16_t)-AX;
L4E24: /* L4E24 */
    /* 4E24  mov     word ptr ds:[26CCh],bx */
    ww(pDS, 0x26CC, BX);
L4E28:
    /* 4E28  mov     word ptr ds:[26CEh],ax */
    ww(pDS, 0x26CE, AX);
L4E2B: /* L4E2B */
    /* 4E2B  mov     ax,word ptr ds:[26CEh] */
    AX = rw(pDS, 0x26CE);
L4E2E:
    /* 4E2E  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L4E32:
    /* 4E32  mov     bx,ax */
    BX = AX;
L4E34:
    /* 4E34  mov     cx,dx */
    CX = DX;
L4E36:
    /* 4E36  mov     ax,word ptr ds:[26CCh] */
    AX = rw(pDS, 0x26CC);
L4E39:
    /* 4E39  neg     ax */
    AX = (uint16_t)-AX;
L4E3B:
    /* 4E3B  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L4E3F:
    /* 4E3F  add     ax,bx */
    AX = add16(AX, BX, 0);
L4E41:
    /* 4E41  adc     dx,cx */
    DX = (uint16_t)(DX + CX + CF);
L4E43:
    /* 4E43  shl     ax,1 */
    AX = shl16(AX, 1);
L4E45:
    /* 4E45  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4E47:
    /* 4E47  shl     ax,1 */
    AX = shl16(AX, 1);
L4E49:
    /* 4E49  adc     dx,0 */
    DX = (uint16_t)(DX + 0x0 + CF);
L4E4C:
    /* 4E4C  mov     word ptr ds:[26D0h],dx */
    ww(pDS, 0x26D0, DX);
L4E50:
    /* 4E50  mov     ax,word ptr ds:[26CEh] */
    AX = rw(pDS, 0x26CE);
L4E53:
    /* 4E53  imul    word ptr ds:[MAT_XY] */
    imul16(rw(pDS, 0x1604));
L4E57:
    /* 4E57  mov     bx,ax */
    BX = AX;
L4E59:
    /* 4E59  mov     cx,dx */
    CX = DX;
L4E5B:
    /* 4E5B  mov     ax,word ptr ds:[26CCh] */
    AX = rw(pDS, 0x26CC);
L4E5E:
    /* 4E5E  neg     ax */
    AX = (uint16_t)-AX;
L4E60:
    /* 4E60  imul    word ptr ds:[MAT_ZY] */
    imul16(rw(pDS, 0x1610));
L4E64:
    /* 4E64  add     ax,bx */
    AX = add16(AX, BX, 0);
L4E66:
    /* 4E66  adc     dx,cx */
    DX = (uint16_t)(DX + CX + CF);
L4E68:
    /* 4E68  shl     ax,1 */
    AX = shl16(AX, 1);
L4E6A:
    /* 4E6A  rcl     dx,1 */
    DX = rcl16(DX, 1);
L4E6C:
    /* 4E6C  shl     ax,1 */
    AX = shl16(AX, 1);
L4E6E:
    /* 4E6E  adc     dx,0 */
    DX = add16(DX, 0x0, CF);
L4E71:
    /* 4E71  mov     word ptr ds:[26D2h],dx */
    ww(pDS, 0x26D2, DX);
L4E75:
    /* 4E75  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_4E76  (+4E76)
       check_flat: when the view is level (the matrix's middle row and column are 0 except the middle
       entry, at least 7FFBh), install the flat variants of the relative-move opcode handlers
       (flat_x_rel ... flat_rod, INTERP.ASM) in the opcode table and copy the short mxmul body
       (L517E) over mxmul; otherwise install the general handlers and the full body (L512B). 25D4
       records which set is in, so the copy is done only on a change. A level view is probably the
       common case: the view pitches only while the player looks up or down. */
L4E76: /* _check_flat */
    /* 4E76  test    word ptr ds:[MAT_YX],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x1608) & 0xFFFF));
L4E7C:
    /* 4E7C  jne     short L4EA1 */
    if (!ZF) goto L4EA1;
L4E7E:
    /* 4E7E  test    word ptr ds:[MAT_XY],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x1604) & 0xFFFF));
L4E84:
    /* 4E84  jne     short L4EA1 */
    if (!ZF) goto L4EA1;
L4E86:
    /* 4E86  test    word ptr ds:[MAT_ZY],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x1610) & 0xFFFF));
L4E8C:
    /* 4E8C  jne     short L4EA1 */
    if (!ZF) goto L4EA1;
L4E8E:
    /* 4E8E  test    word ptr ds:[MAT_YZ],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x160C) & 0xFFFF));
L4E94:
    /* 4E94  jne     short L4EA1 */
    if (!ZF) goto L4EA1;
L4E96:
    /* 4E96  cmp     word ptr ds:[MAT_YY],7FFBh */
    sub16(rw(pDS, 0x160A), 0x7FFB, 0);
L4E9C:
    /* 4E9C  jl      short L4EA1 */
    if (SF != OF) goto L4EA1;
L4E9E:
    /* 4E9E  jmp     L4F72 */
    goto L4F72;
L4EA1: /* L4EA1 */
    /* 4EA1  test    byte ptr ds:[286Fh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x286F) & 0xFF));
L4EA6:
    /* 4EA6  jne     short L4EAB */
    if (!ZF) goto L4EAB;
L4EA8:
    /* 4EA8  jmp     L4F71 */
    goto L4F71;
L4EAB: /* L4EAB */
    /* 4EAB  mov     byte ptr ds:[286Fh],0 */
    wb(pDS, 0x286F, 0x0);
L4EB0:
    /* 4EB0  mov     word ptr ds:[2824h],offset _do_y_rel */
    ww(pDS, 0x2824, 0x328B);
L4EB6:
    /* 4EB6  mov     word ptr ds:[2822h],offset _do_x_rel */
    ww(pDS, 0x2822, 0x3441);
L4EBC:
    /* 4EBC  mov     word ptr ds:[2826h],offset _do_z_rel */
    ww(pDS, 0x2826, 0x34E4);
L4EC2:
    /* 4EC2  mov     word ptr ds:[2848h],offset _seg004_223B */
    ww(pDS, 0x2848, 0x223B);
L4EC8:
    /* 4EC8  mov     word ptr ds:[284Ah],offset _seg004_227C */
    ww(pDS, 0x284A, 0x227C);
L4ECE:
    /* 4ECE  mov     word ptr ds:[284Ch],offset _seg004_22BD */
    ww(pDS, 0x284C, 0x22BD);
L4ED4:
    /* 4ED4  mov     word ptr ds:[282Ch],offset _do_xy_rel */
    ww(pDS, 0x282C, 0x358D);
L4EDA:
    /* 4EDA  mov     word ptr ds:[282Eh],offset _do_xz_rel */
    ww(pDS, 0x282E, 0x352C);
L4EE0:
    /* 4EE0  mov     word ptr ds:[2830h],offset _do_yz_rel */
    ww(pDS, 0x2830, 0x35EE);
L4EE6:
    /* 4EE6  mov     word ptr ds:[2852h],offset _seg004_241B */
    ww(pDS, 0x2852, 0x241B);
L4EEC:
    /* 4EEC  mov     word ptr ds:[2856h],offset _seg004_2479 */
    ww(pDS, 0x2856, 0x2479);
L4EF2:
    /* 4EF2  mov     word ptr ds:[2854h],offset _seg004_23BD */
    ww(pDS, 0x2854, 0x23BD);
L4EF8:
    /* 4EF8  mov     word ptr ds:[2838h],offset _do_rod */
    ww(pDS, 0x2838, 0x2C8A);
L4EFE:
    /* 4EFE  mov     word ptr ds:[285Eh],offset _seg004_2023 */
    ww(pDS, 0x285E, 0x2023);
L4F04: /* L4F04 */
    /* 4F04  test    byte ptr ds:[286Eh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x286E) & 0xFF));
L4F09:
    /* 4F09  je      short L4F37 */
    if (ZF) goto L4F37;
L4F0B:
    /* 4F0B  mov     word ptr ds:[27BEh],offset _seg004_223B */
    ww(pDS, 0x27BE, 0x223B);
L4F11:
    /* 4F11  mov     word ptr ds:[27C0h],offset _seg004_227C */
    ww(pDS, 0x27C0, 0x227C);
L4F17:
    /* 4F17  mov     word ptr ds:[27C2h],offset _seg004_22BD */
    ww(pDS, 0x27C2, 0x22BD);
L4F1D:
    /* 4F1D  mov     word ptr ds:[27C8h],offset _seg004_241B */
    ww(pDS, 0x27C8, 0x241B);
L4F23:
    /* 4F23  mov     word ptr ds:[27CCh],offset _seg004_2479 */
    ww(pDS, 0x27CC, 0x2479);
L4F29:
    /* 4F29  mov     word ptr ds:[27CAh],offset _seg004_23BD */
    ww(pDS, 0x27CA, 0x23BD);
L4F2F:
    /* 4F2F  mov     word ptr ds:[27D4h],offset _seg004_2023 */
    ww(pDS, 0x27D4, 0x2023);
L4F35:
    /* 4F35  jmp     short L4F61 */
    goto L4F61;
L4F37: /* L4F37 */
    /* 4F37  mov     word ptr ds:[27BEh],offset _do_x_rel */
    ww(pDS, 0x27BE, 0x3441);
L4F3D:
    /* 4F3D  mov     word ptr ds:[27C0h],offset _do_y_rel */
    ww(pDS, 0x27C0, 0x328B);
L4F43:
    /* 4F43  mov     word ptr ds:[27C2h],offset _do_z_rel */
    ww(pDS, 0x27C2, 0x34E4);
L4F49:
    /* 4F49  mov     word ptr ds:[27C8h],offset _do_xy_rel */
    ww(pDS, 0x27C8, 0x358D);
L4F4F:
    /* 4F4F  mov     word ptr ds:[27CCh],offset _do_yz_rel */
    ww(pDS, 0x27CC, 0x35EE);
L4F55:
    /* 4F55  mov     word ptr ds:[27CAh],offset _do_xz_rel */
    ww(pDS, 0x27CA, 0x352C);
L4F5B:
    /* 4F5B  mov     word ptr ds:[27D4h],offset _do_rod */
    ww(pDS, 0x27D4, 0x2C8A);
L4F61: /* L4F61 */
    /* 4F61  mov     si,offset L512B */
    SI = 0x512B;
L4F64:
    /* 4F64  mov     di,offset _mxmul */
    DI = 0x50D8;
L4F67:
    /* 4F67  push    es */
    push16(asm_es);
L4F68:
    /* 4F68  push    cs */
    push16((uint16_t)(0x06E7 + PORT_LOAD_SEG));
L4F69:
    /* 4F69  pop     es */
    SET_ES(pop16());
L4F6A:
    /* 4F6A  mov     cx,54h */
    CX = 0x54;
L4F6D:
    /* 4F6D  rep movs byte ptr es:[di],byte ptr cs:[si] */
    while (CX) { wb(pES, DI, rb(CODE004, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L4F70:
    /* 4F70  pop     es */
    SET_ES(pop16());
L4F71: /* L4F71 */
    /* 4F71  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L4F72: /* is_flat, L4F72 */
    /* 4F72  test    byte ptr ds:[286Fh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x286F) & 0xFF));
L4F77:
    /* 4F77  jne     L4F71 */
    if (!ZF) goto L4F71;
L4F79:
    /* 4F79  mov     byte ptr ds:[286Fh],0FFh */
    wb(pDS, 0x286F, 0xFF);
L4F7E:
    /* 4F7E  mov     word ptr ds:[2824h],offset _flat_y_rel */
    ww(pDS, 0x2824, 0x3238);
L4F84:
    /* 4F84  mov     word ptr ds:[2822h],offset _flat_x_rel */
    ww(pDS, 0x2822, 0x33E5);
L4F8A:
    /* 4F8A  mov     word ptr ds:[2826h],offset _flat_z_rel */
    ww(pDS, 0x2826, 0x3489);
L4F90:
    /* 4F90  mov     word ptr ds:[2848h],offset _seg004_2348 */
    ww(pDS, 0x2848, 0x2348);
L4F96:
    /* 4F96  mov     word ptr ds:[284Ah],offset _seg004_2322 */
    ww(pDS, 0x284A, 0x2322);
L4F9C:
    /* 4F9C  mov     word ptr ds:[284Ch],offset _seg004_2383 */
    ww(pDS, 0x284C, 0x2383);
L4FA2:
    /* 4FA2  mov     word ptr ds:[282Ch],offset _flat_xy_rel */
    ww(pDS, 0x282C, 0x36A2);
L4FA8:
    /* 4FA8  mov     word ptr ds:[282Eh],offset _flat_xz_rel */
    ww(pDS, 0x282E, 0x364F);
L4FAE:
    /* 4FAE  mov     word ptr ds:[2830h],offset _flat_yz_rel */
    ww(pDS, 0x2830, 0x36EA);
L4FB4:
    /* 4FB4  mov     word ptr ds:[2852h],offset _seg004_2527 */
    ww(pDS, 0x2852, 0x2527);
L4FBA:
    /* 4FBA  mov     word ptr ds:[2856h],offset _seg004_256C */
    ww(pDS, 0x2856, 0x256C);
L4FC0:
    /* 4FC0  mov     word ptr ds:[2854h],offset _seg004_24D7 */
    ww(pDS, 0x2854, 0x24D7);
L4FC6:
    /* 4FC6  mov     word ptr ds:[2838h],offset _flat_rod */
    ww(pDS, 0x2838, 0x2D8F);
L4FCC:
    /* 4FCC  mov     word ptr ds:[285Eh],offset _seg004_20CC */
    ww(pDS, 0x285E, 0x20CC);
L4FD2:
    /* 4FD2  test    byte ptr ds:[286Eh],0FFh */
    logic8((uint8_t)(rb(pDS, 0x286E) & 0xFF));
L4FD7:
    /* 4FD7  je      short L5005 */
    if (ZF) goto L5005;
L4FD9:
    /* 4FD9  mov     word ptr ds:[27BEh],offset _seg004_2348 */
    ww(pDS, 0x27BE, 0x2348);
L4FDF:
    /* 4FDF  mov     word ptr ds:[27C0h],offset _seg004_2322 */
    ww(pDS, 0x27C0, 0x2322);
L4FE5:
    /* 4FE5  mov     word ptr ds:[27C2h],offset _seg004_2383 */
    ww(pDS, 0x27C2, 0x2383);
L4FEB:
    /* 4FEB  mov     word ptr ds:[27C8h],offset _seg004_2527 */
    ww(pDS, 0x27C8, 0x2527);
L4FF1:
    /* 4FF1  mov     word ptr ds:[27CCh],offset _seg004_256C */
    ww(pDS, 0x27CC, 0x256C);
L4FF7:
    /* 4FF7  mov     word ptr ds:[27CAh],offset _seg004_24D7 */
    ww(pDS, 0x27CA, 0x24D7);
L4FFD:
    /* 4FFD  mov     word ptr ds:[27D4h],offset _seg004_20CC */
    ww(pDS, 0x27D4, 0x20CC);
L5003:
    /* 5003  jmp     short L502F */
    goto L502F;
L5005: /* L5005 */
    /* 5005  mov     word ptr ds:[27BEh],offset _flat_x_rel */
    ww(pDS, 0x27BE, 0x33E5);
L500B:
    /* 500B  mov     word ptr ds:[27C0h],offset _flat_y_rel */
    ww(pDS, 0x27C0, 0x3238);
L5011:
    /* 5011  mov     word ptr ds:[27C2h],offset _flat_z_rel */
    ww(pDS, 0x27C2, 0x3489);
L5017:
    /* 5017  mov     word ptr ds:[27C8h],offset _flat_xy_rel */
    ww(pDS, 0x27C8, 0x36A2);
L501D:
    /* 501D  mov     word ptr ds:[27CCh],offset _flat_yz_rel */
    ww(pDS, 0x27CC, 0x36EA);
L5023:
    /* 5023  mov     word ptr ds:[27CAh],offset _flat_xz_rel */
    ww(pDS, 0x27CA, 0x364F);
L5029:
    /* 5029  mov     word ptr ds:[27D4h],offset _flat_rod */
    ww(pDS, 0x27D4, 0x2D8F);
L502F: /* L502F */
    /* 502F  mov     si,offset L517E */
    SI = 0x517E;
L5032:
    /* 5032  mov     di,offset _mxmul */
    DI = 0x50D8;
L5035:
    /* 5035  push    es */
    push16(asm_es);
L5036:
    /* 5036  push    cs */
    push16((uint16_t)(0x06E7 + PORT_LOAD_SEG));
L5037:
    /* 5037  pop     es */
    SET_ES(pop16());
L5038:
    /* 5038  mov     cx,26h */
    CX = 0x26;
L503B:
    /* 503B  rep movs byte ptr es:[di],byte ptr cs:[si] */
    while (CX) { wb(pES, DI, rb(CODE004, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L503E:
    /* 503E  pop     es */
    SET_ES(pop16());
L503F:
    /* 503F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_5040  (+5040)
       UW1 only: when byte 286Eh is set, clears it and copies the normal handlers for opcodes
       78h..9Ch (seg051:2814h) into the opcode table (27B0h). The branch that would copy
       NOCLIP.ASM's set (283Ah) is never taken: the xor that clears 286Eh leaves ZF set. */
L5040: /* _seg004_5040 */
    /* 5040  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L5042:
    /* 5042  xor     al,byte ptr ds:[286Eh] */
    AL = logic8((uint8_t)(AL ^ rb(pDS, 0x286E)));
L5046:
    /* 5046  je      short L505F */
    if (ZF) goto L505F;
L5048:
    /* 5048  push    si */
    push16(SI);
L5049:
    /* 5049  push    di */
    push16(DI);
L504A:
    /* 504A  push    cx */
    push16(CX);
L504B:
    /* 504B  mov     di,27B0h */
    DI = 0x27B0;
L504E:
    /* 504E  mov     cx,13h */
    CX = 0x13;
L5051:
    /* 5051  xor     byte ptr ds:[286Eh],al */
    wb(pDS, 0x286E, logic8((uint8_t)(rb(pDS, 0x286E) ^ AL)));
L5055:
    /* 5055  je      short L5060 */
    if (ZF) goto L5060;
L5057:
    /* 5057  mov     si,283Ah */
    SI = 0x283A;
L505A:
    /* 505A  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L505C:
    /* 505C  pop     cx */
    CX = pop16();
L505D:
    /* 505D  pop     di */
    DI = pop16();
L505E:
    /* 505E  pop     si */
    SI = pop16();
L505F: /* L505F */
    /* 505F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L5060: /* L5060 */
    /* 5060  mov     si,2814h */
    SI = 0x2814;
L5063:
    /* 5063  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L5065:
    /* 5065  pop     cx */
    CX = pop16();
L5066:
    /* 5066  pop     di */
    DI = pop16();
L5067:
    /* 5067  pop     si */
    SI = pop16();

    /* seg004_5068  (+5068)
       set_accept: empty here and in FM Towns (accept_flag, which it would set, is never written; see
       CLIP.ASM). */
L5068: /* _set_accept */
    /* 5068  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_5069  (+5069)
       self_modify: patch the eye position in the current object's frame (2886, 2888, 288A) and the
       point shift (2880) into the immediates of mini_xlate_rotate_pnt and load_xlate_rotate_pnt, so
       that turning a model point into view space needs no memory reads for them. */
L5069: /* _self_modify */
    /* 5069  mov     ax,word ptr ds:[OBJ_X] */
    AX = rw(pDS, 0x2886);
L506C:
    /* 506C  mov     word ptr cs:[modify_mex+1],ax */
    ww(CODE004, 0x50C2, AX);
L5070:
    /* 5070  mov     word ptr cs:[L509D+1],ax */
    ww(CODE004, 0x509E, AX);
L5074:
    /* 5074  mov     ax,word ptr ds:[OBJ_Y] */
    AX = rw(pDS, 0x2888);
L5077:
    /* 5077  mov     word ptr cs:[modify_mey+1],ax */
    ww(CODE004, 0x50D2, AX);
L507B:
    /* 507B  mov     word ptr cs:[L50B5+1],ax */
    ww(CODE004, 0x50B6, AX);
L507F:
    /* 507F  mov     ax,word ptr ds:[OBJ_Z] */
    AX = rw(pDS, 0x288A);
L5082:
    /* 5082  mov     word ptr cs:[modify_mez+1],ax */
    ww(CODE004, 0x50CA, AX);
L5086:
    /* 5086  mov     word ptr cs:[L50A9+1],ax */
    ww(CODE004, 0x50AA, AX);
L508A:
    /* 508A  mov     al,byte ptr ds:[OBJ_SHIFT] */
    AL = rb(pDS, 0x2880);
L508D:
    /* 508D  mov     byte ptr cs:[modify_mes+1],al */
    wb(CODE004, 0x50BF, AL);
L5091:
    /* 5091  mov     byte ptr cs:[_mini_xlate_rotate_pnt+1],al */
    wb(CODE004, 0x5097, AL);
L5095:
    /* 5095  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_5096  (+5096)
       mini_xlate_rotate_pnt: in: SI = three signed bytes, a model point in the order x, z, y, each
       times 32. Subtracts the patched eye, shifts by the patched shift and falls into mxmul. Out:
       BX, CX, BP = the view-space point (see mxmul); SI advanced by 3. load_xlate_rotate_pnt below
       is the same for three words. */
L5096: /* _mini_xlate_rotate_pnt */
    /* 5096  mov     cl,5 */
    CL = rb(CODE004, 0x5097);
L5098:
    /* 5098  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L5099:
    /* 5099  cbw */
    AX = (uint16_t)(int8_t)AL;
L509A:
    /* 509A  shl     ax,5 */
    AX = (uint16_t)(AX << 5);
L509D: /* L509D */
    /* 509D  sub     ax,1234h */
    AX = sub16(AX, rw(CODE004, 0x509E), 0);
L50A0:
    /* 50A0  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L50A2:
    /* 50A2  mov     bx,ax */
    BX = AX;
L50A4:
    /* 50A4  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L50A5:
    /* 50A5  cbw */
    AX = (uint16_t)(int8_t)AL;
L50A6:
    /* 50A6  shl     ax,5 */
    AX = (uint16_t)(AX << 5);
L50A9: /* L50A9 */
    /* 50A9  sub     ax,1234h */
    AX = sub16(AX, rw(CODE004, 0x50AA), 0);
L50AC:
    /* 50AC  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L50AE:
    /* 50AE  mov     bp,ax */
    BP = AX;
L50B0:
    /* 50B0  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L50B1:
    /* 50B1  cbw */
    AX = (uint16_t)(int8_t)AL;
L50B2:
    /* 50B2  shl     ax,5 */
    AX = (uint16_t)(AX << 5);
L50B5: /* L50B5 */
    /* 50B5  sub     ax,1234h */
    AX = sub16(AX, rw(CODE004, 0x50B6), 0);
L50B8:
    /* 50B8  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L50BA:
    /* 50BA  mov     cx,ax */
    CX = AX;
L50BC:
    /* 50BC  jmp     short _mxmul */
    goto L50D8;

    /* seg004_50BE  (+50BE) */
L50BE: /* _load_xlate_rotate_pnt, modify_mes */
    /* 50BE  mov     cl,5 */
    CL = rb(CODE004, 0x50BF);
L50C0:
    /* 50C0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L50C1: /* modify_mex */
    /* 50C1  sub     ax,1234h */
    AX = sub16(AX, rw(CODE004, 0x50C2), 0);
L50C4:
    /* 50C4  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L50C6:
    /* 50C6  mov     bx,ax */
    BX = AX;
L50C8:
    /* 50C8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L50C9: /* modify_mez */
    /* 50C9  sub     ax,1234h */
    AX = sub16(AX, rw(CODE004, 0x50CA), 0);
L50CC:
    /* 50CC  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L50CE:
    /* 50CE  mov     bp,ax */
    BP = AX;
L50D0:
    /* 50D0  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L50D1: /* modify_mey */
    /* 50D1  sub     ax,1234h */
    AX = sub16(AX, rw(CODE004, 0x50D2), 0);
L50D4:
    /* 50D4  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L50D6:
    /* 50D6  mov     cx,ax */
    CX = AX;

    /* seg004_50D8  (+50D8)
       mxmul: rotate a point by the view matrix. In: BX, CX, BP = the point's x, y (vertical) and z.
       Out: BX, CX, BP = view x, y and depth z, each the high word of its 1.15 products (so at half
       scale); DI, DX changed. The general body (copied from L512B) does all nine products; the flat
       body (L517E, for a level view) skips the products with the zero entries and takes y as it is,
       halved. */
L50D8: /* _mxmul */
    /* 50D8  mov     ax,bx */
    AX = BX;
L50DA:
    /* 50DA  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L50DE:
    /* 50DE  mov     di,dx */
    DI = DX;
L50E0:
    /* 50E0  mov     ax,cx */
    AX = CX;
L50E2:
    /* 50E2  imul    word ptr ds:[MAT_YX] */
    imul16(rw(pDS, 0x1608));
L50E6:
    /* 50E6  add     di,dx */
    DI = (uint16_t)(DI + DX);
L50E8:
    /* 50E8  mov     ax,bp */
    AX = BP;
L50EA:
    /* 50EA  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L50EE:
    /* 50EE  add     di,dx */
    DI = (uint16_t)(DI + DX);
L50F0:
    /* 50F0  mov     word ptr ds:[2708h],di */
    ww(pDS, 0x2708, DI);
L50F4:
    /* 50F4  mov     ax,bx */
    AX = BX;
L50F6:
    /* 50F6  imul    word ptr ds:[MAT_XY] */
    imul16(rw(pDS, 0x1604));
L50FA:
    /* 50FA  mov     di,dx */
    DI = DX;
L50FC:
    /* 50FC  mov     ax,cx */
    AX = CX;
L50FE:
    /* 50FE  imul    word ptr ds:[MAT_YY] */
    imul16(rw(pDS, 0x160A));
L5102:
    /* 5102  add     di,dx */
    DI = (uint16_t)(DI + DX);
L5104:
    /* 5104  mov     ax,bp */
    AX = BP;
L5106:
    /* 5106  imul    word ptr ds:[MAT_ZY] */
    imul16(rw(pDS, 0x1610));
L510A:
    /* 510A  add     di,dx */
    DI = (uint16_t)(DI + DX);
L510C:
    /* 510C  mov     ax,bp */
    AX = BP;
L510E:
    /* 510E  imul    word ptr ds:[MAT_ZZ] */
    imul16(rw(pDS, 0x1612));
L5112:
    /* 5112  mov     bp,dx */
    BP = DX;
L5114:
    /* 5114  mov     ax,bx */
    AX = BX;
L5116:
    /* 5116  imul    word ptr ds:[MAT_XZ] */
    imul16(rw(pDS, 0x1606));
L511A:
    /* 511A  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L511C:
    /* 511C  mov     ax,cx */
    AX = CX;
L511E:
    /* 511E  imul    word ptr ds:[MAT_YZ] */
    imul16(rw(pDS, 0x160C));
L5122:
    /* 5122  add     bp,dx */
    BP = add16(BP, DX, 0);
L5124:
    /* 5124  mov     cx,di */
    CX = DI;
L5126:
    /* 5126  mov     bx,word ptr ds:[2708h] */
    BX = rw(pDS, 0x2708);
L512A:
    /* 512A  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* L512B and L517E: the two bodies check_flat and is_flat copy over the one above */
L512B: /* L512B */
    /* 512B  mov     ax,bx */
    AX = BX;
L512D:
    /* 512D  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L5131:
    /* 5131  mov     di,dx */
    DI = DX;
L5133:
    /* 5133  mov     ax,cx */
    AX = CX;
L5135:
    /* 5135  imul    word ptr ds:[MAT_YX] */
    imul16(rw(pDS, 0x1608));
L5139:
    /* 5139  add     di,dx */
    DI = (uint16_t)(DI + DX);
L513B:
    /* 513B  mov     ax,bp */
    AX = BP;
L513D:
    /* 513D  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L5141:
    /* 5141  add     di,dx */
    DI = (uint16_t)(DI + DX);
L5143:
    /* 5143  mov     word ptr ds:[2708h],di */
    ww(pDS, 0x2708, DI);
L5147:
    /* 5147  mov     ax,bx */
    AX = BX;
L5149:
    /* 5149  imul    word ptr ds:[MAT_XY] */
    imul16(rw(pDS, 0x1604));
L514D:
    /* 514D  mov     di,dx */
    DI = DX;
L514F:
    /* 514F  mov     ax,cx */
    AX = CX;
L5151:
    /* 5151  imul    word ptr ds:[MAT_YY] */
    imul16(rw(pDS, 0x160A));
L5155:
    /* 5155  add     di,dx */
    DI = (uint16_t)(DI + DX);
L5157:
    /* 5157  mov     ax,bp */
    AX = BP;
L5159:
    /* 5159  imul    word ptr ds:[MAT_ZY] */
    imul16(rw(pDS, 0x1610));
L515D:
    /* 515D  add     di,dx */
    DI = (uint16_t)(DI + DX);
L515F:
    /* 515F  mov     ax,bp */
    AX = BP;
L5161:
    /* 5161  imul    word ptr ds:[MAT_ZZ] */
    imul16(rw(pDS, 0x1612));
L5165:
    /* 5165  mov     bp,dx */
    BP = DX;
L5167:
    /* 5167  mov     ax,bx */
    AX = BX;
L5169:
    /* 5169  imul    word ptr ds:[MAT_XZ] */
    imul16(rw(pDS, 0x1606));
L516D:
    /* 516D  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L516F:
    /* 516F  mov     ax,cx */
    AX = CX;
L5171:
    /* 5171  imul    word ptr ds:[MAT_YZ] */
    imul16(rw(pDS, 0x160C));
L5175:
    /* 5175  add     bp,dx */
    BP = add16(BP, DX, 0);
L5177:
    /* 5177  mov     cx,di */
    CX = DI;
L5179:
    /* 5179  mov     bx,word ptr ds:[2708h] */
    BX = rw(pDS, 0x2708);
L517D:
    /* 517D  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L517E: /* L517E */
    /* 517E  mov     ax,bx */
    AX = BX;
L5180:
    /* 5180  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L5184:
    /* 5184  mov     di,dx */
    DI = DX;
L5186:
    /* 5186  mov     ax,bp */
    AX = BP;
L5188:
    /* 5188  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L518C:
    /* 518C  add     di,dx */
    DI = (uint16_t)(DI + DX);
L518E:
    /* 518E  mov     ax,bx */
    AX = BX;
L5190:
    /* 5190  mov     bx,di */
    BX = DI;
L5192:
    /* 5192  imul    word ptr ds:[MAT_XZ] */
    imul16(rw(pDS, 0x1606));
L5196:
    /* 5196  mov     ax,bp */
    AX = BP;
L5198:
    /* 5198  mov     bp,dx */
    BP = DX;
L519A:
    /* 519A  imul    word ptr ds:[MAT_ZZ] */
    imul16(rw(pDS, 0x1612));
L519E:
    /* 519E  add     bp,dx */
    BP = (uint16_t)(BP + DX);
L51A0:
    /* 51A0  sar     cx,1 */
    CX = sar16(CX, 1);
L51A2:
    /* 51A2  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_51A3  (+51A3)
       seg004_51A3: choose the axis for SPHERE.ASM's first cull (modify_axis2): y when the view
       is pitched by more than 45 degrees (|160C| > 5A82h, sin 45 in 1.15), else whichever of x and z
       the view looks along more. Patches the instruction to add or subtract (by the sign) the eye's
       coordinate on that axis (2886 + offset). */
L51A3: /* _seg004_51A3 */
    /* 51A3  mov     ax,word ptr ds:[MAT_YZ] */
    AX = rw(pDS, 0x160C);
L51A6:
    /* 51A6  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L51A7:
    /* 51A7  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L51A9:
    /* 51A9  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L51AB:
    /* 51AB  cmp     ax,5A82h */
    sub16(AX, 0x5A82, 0);
L51AE:
    /* 51AE  jle     short L51B5 */
    if (ZF || SF != OF) goto L51B5;
L51B0:
    /* 51B0  mov     ax,2 */
    AX = 0x2;
L51B3:
    /* 51B3  jmp     short L51D7 */
    goto L51D7;
L51B5: /* L51B5 */
    /* 51B5  mov     ax,word ptr ds:[MAT_XZ] */
    AX = rw(pDS, 0x1606);
L51B8:
    /* 51B8  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L51B9:
    /* 51B9  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L51BB:
    /* 51BB  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L51BD:
    /* 51BD  mov     bx,ax */
    BX = AX;
L51BF:
    /* 51BF  mov     cx,dx */
    CX = DX;
L51C1:
    /* 51C1  mov     ax,word ptr ds:[MAT_ZZ] */
    AX = rw(pDS, 0x1612);
L51C4:
    /* 51C4  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L51C5:
    /* 51C5  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L51C7:
    /* 51C7  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L51C9:
    /* 51C9  cmp     bx,ax */
    sub16(BX, AX, 0);
L51CB:
    /* 51CB  jg      short L51D2 */
    if (!ZF && SF == OF) goto L51D2;
L51CD:
    /* 51CD  mov     ax,4 */
    AX = 0x4;
L51D0:
    /* 51D0  jmp     short L51D7 */
    goto L51D7;
L51D2: /* L51D2 */
    /* 51D2  mov     ax,0 */
    AX = 0x0;
L51D5:
    /* 51D5  mov     dx,cx */
    DX = CX;
L51D7: /* L51D7 */
    /* 51D7  mov     bl,2Bh */
    BL = 0x2B;
L51D9:
    /* 51D9  test    dx,0FFFFh */
    logic16((uint16_t)(DX & 0xFFFF));
L51DD:
    /* 51DD  jns     short L51E1 */
    if (!SF) goto L51E1;
L51DF:
    /* 51DF  mov     bl,3 */
    BL = 0x3;
L51E1: /* L51E1 */
    /* 51E1  mov     byte ptr cs:[1876h],bl */
    wb(CODE004, 0x1876, BL);
L51E6:
    /* 51E6  mov     byte ptr cs:[1681h],bl */
    wb(CODE004, 0x1681, BL);
L51EB:
    /* 51EB  mov     byte ptr cs:[1CD4h],bl */
    wb(CODE004, 0x1CD4, BL);
L51F0:
    /* 51F0  mov     byte ptr cs:[1B6Ch],bl */
    wb(CODE004, 0x1B6C, BL);
L51F5:
    /* 51F5  add     ax,1C8h */
    AX = (uint16_t)(AX + 0x1C8);
L51F8:
    /* 51F8  mov     word ptr cs:[1878h],ax */
    ww(CODE004, 0x1878, AX);
L51FC:
    /* 51FC  mov     word ptr cs:[1683h],ax */
    ww(CODE004, 0x1683, AX);
L5200:
    /* 5200  sub     ax,1C8h */
    AX = (uint16_t)(AX - 0x1C8);
L5203:
    /* 5203  add     ax,OBJ_X */
    AX = add16(AX, 0x2886, 0);
L5206:
    /* 5206  mov     word ptr cs:[modify_axis2+2],ax */
    ww(CODE004, 0x1CD6, AX);
L520A:
    /* 520A  mov     word ptr cs:[1B6Eh],ax */
    ww(CODE004, 0x1B6E, AX);
L520E:
    /* 520E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_520F  (+520F)
       UW1 only: sorts the words from 2864h to 2868h (offsets of records in seg053) by the records'
       words +2 and +0 (an insertion sort), and sets byte 286Ch. render_3d calls it after the
       frame unless 286Ch is already set. */
L520F: /* _seg004_520F */
    /* 520F  mov     byte ptr ds:[286Ch],0FFh */
    wb(pDS, 0x286C, 0xFF);
L5214:
    /* 5214  mov     cx,word ptr ds:[2868h] */
    CX = rw(pDS, 0x2868);
L5218:
    /* 5218  mov     si,word ptr ds:[2864h] */
    SI = rw(pDS, 0x2864);
L521C:
    /* 521C  mov     dx,si */
    DX = SI;
L521E:
    /* 521E  add     si,2 */
    SI = (uint16_t)(SI + 0x2);
L5221:
    /* 5221  cmp     cx,si */
    sub16(CX, SI, 0);
L5223:
    /* 5223  jle     short L5258 */
    if (ZF || SF != OF) goto L5258;
L5225:
    /* 5225  push    ds */
    push16(asm_ds);
L5226:
    /* 5226  push    es */
    push16(asm_es);
L5227:
    /* 5227  mov     ax,seg seg053 */
    AX = (uint16_t)(0x5471 + PORT_LOAD_SEG);
L522A:
    /* 522A  mov     ds,ax */
    SET_DS(AX);
L522C:
    /* 522C  mov     es,ax */
    SET_ES(AX);
L522E: /* L522E */
    /* 522E  mov     di,si */
    DI = SI;
L5230:
    /* 5230  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5231:
    /* 5231  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L5232: /* L5232 */
    /* 5232  cmp     di,dx */
    sub16(DI, DX, 0);
L5234:
    /* 5234  jle     short L5250 */
    if (ZF || SF != OF) goto L5250;
L5236:
    /* 5236  mov     bp,word ptr [di-2] */
    BP = rw(pDS, DI + 0xFFFE);
L5239:
    /* 5239  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L523C:
    /* 523C  cmp     ax,word ptr ds:[bp+2] */
    sub16(AX, rw(pDS, BP + 0x2), 0);
L5240:
    /* 5240  jl      short L5250 */
    if (SF != OF) goto L5250;
L5242:
    /* 5242  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L5244:
    /* 5244  cmp     ax,word ptr ds:[bp] */
    sub16(AX, rw(pDS, BP), 0);
L5248:
    /* 5248  jle     short L5250 */
    if (ZF || SF != OF) goto L5250;
L524A:
    /* 524A  mov     word ptr [di],bp */
    ww(pDS, DI, BP);
L524C:
    /* 524C  dec     di */
    DI = (uint16_t)(DI - 1);
L524D:
    /* 524D  dec     di */
    DI = (uint16_t)(DI - 1);
L524E:
    /* 524E  jmp     L5232 */
    goto L5232;
L5250: /* L5250 */
    /* 5250  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L5252:
    /* 5252  cmp     cx,si */
    sub16(CX, SI, 0);
L5254:
    /* 5254  jne     L522E */
    if (!ZF) goto L522E;
L5256:
    /* 5256  pop     es */
    SET_ES(pop16());
L5257:
    /* 5257  pop     ds */
    SET_DS(pop16());
L5258: /* L5258 */
    /* 5258  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L5259:
    /* 5259  mov     cx,word ptr ds:[286Ah] */
    CX = rw(pDS, 0x286A);
L525D:
    /* 525D  mov     si,0Ch */
    SI = 0xC;
L5260:
    /* 5260  sub     cx,si */
    CX = (uint16_t)(CX - SI);
L5262:
    /* 5262  shr     cx,1 */
    CX = shr16(CX, 1);
L5264:
    /* 5264  dec     cx */
    CX = dec16(CX);
L5265:
    /* 5265  jle     short L5298 */
    if (ZF || SF != OF) goto L5298;
L5267:
    /* 5267  mov     dx,si */
    DX = SI;
L5269:
    /* 5269  add     si,2 */
    SI = (uint16_t)(SI + 0x2);
L526C:
    /* 526C  push    ds */
    push16(asm_ds);
L526D:
    /* 526D  push    es */
    push16(asm_es);
L526E:
    /* 526E  mov     ax,seg seg053 */
    AX = (uint16_t)(0x5471 + PORT_LOAD_SEG);
L5271:
    /* 5271  mov     ds,ax */
    SET_DS(AX);
L5273:
    /* 5273  mov     es,ax */
    SET_ES(AX);
L5275: /* L5275 */
    /* 5275  mov     di,si */
    DI = SI;
L5277:
    /* 5277  sub     di,2 */
    DI = (uint16_t)(DI - 0x2);
L527A:
    /* 527A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L527B:
    /* 527B  mov     bx,ax */
    BX = AX;
L527D:
    /* 527D  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L527F: /* L527F */
    /* 527F  mov     bp,word ptr [di] */
    BP = rw(pDS, DI);
L5281:
    /* 5281  cmp     ax,word ptr ds:[bp] */
    sub16(AX, rw(pDS, BP), 0);
L5285:
    /* 5285  jl      short L5291 */
    if (SF != OF) goto L5291;
L5287:
    /* 5287  mov     word ptr [di+2],bp */
    ww(pDS, DI + 0x2, BP);
L528A:
    /* 528A  sub     di,2 */
    DI = (uint16_t)(DI - 0x2);
L528D:
    /* 528D  cmp     di,dx */
    sub16(DI, DX, 0);
L528F:
    /* 528F  jne     L527F */
    if (!ZF) goto L527F;
L5291: /* L5291 */
    /* 5291  mov     word ptr [di+2],bx */
    ww(pDS, DI + 0x2, BX);
L5294:
    /* 5294  loop    L5275 */
    if (--CX) goto L5275;
L5296:
    /* 5296  pop     es */
    SET_ES(pop16());
L5297:
    /* 5297  pop     ds */
    SET_DS(pop16());
L5298: /* L5298 */
    /* 5298  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L5299:
    /* 5299  test    byte ptr ds:[286Ch],0FFh */
    logic8((uint8_t)(rb(pDS, 0x286C) & 0xFF));
L529E:
    /* 529E  jne     short L52A3 */
    if (!ZF) goto L52A3;
L52A0:
    /* 52A0  call    _seg004_520F */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x520F), 0x52A3)) != 0) return c;
L52A3: /* L52A3 */
    /* 52A3  mov     di,word ptr ds:[2866h] */
    DI = rw(pDS, 0x2866);
L52A7:
    /* 52A7  mov     si,word ptr ds:[2864h] */
    SI = rw(pDS, 0x2864);
L52AB:
    /* 52AB  mov     bp,0Ch */
    BP = 0xC;
L52AE:
    /* 52AE  mov     dx,word ptr ds:[2868h] */
    DX = rw(pDS, 0x2868);
L52B2:
    /* 52B2  mov     cx,word ptr ds:[286Ah] */
    CX = rw(pDS, 0x286A);
L52B6:
    /* 52B6  mov     word ptr ds:[2866h],si */
    ww(pDS, 0x2866, SI);
L52BA:
    /* 52BA  mov     word ptr ds:[2864h],di */
    ww(pDS, 0x2864, DI);
L52BE:
    /* 52BE  push    ds */
    push16(asm_ds);
L52BF:
    /* 52BF  push    es */
    push16(asm_es);
L52C0:
    /* 52C0  mov     ax,seg seg053 */
    AX = (uint16_t)(0x5471 + PORT_LOAD_SEG);
L52C3:
    /* 52C3  mov     es,ax */
    SET_ES(AX);
L52C5:
    /* 52C5  mov     ds,ax */
    SET_DS(AX);
L52C7: /* L52C7 */
    /* 52C7  mov     bx,word ptr [si] */
    BX = rw(pDS, SI);
L52C9:
    /* 52C9  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L52CB:
    /* 52CB  mov     bx,word ptr ds:[bp] */
    BX = rw(pDS, BP);
L52CF:
    /* 52CF  cmp     ax,word ptr [bx] */
    sub16(AX, rw(pDS, BX), 0);
L52D1:
    /* 52D1  jl      short L52E7 */
    if (SF != OF) goto L52E7;
L52D3:
    /* 52D3  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L52D4:
    /* 52D4  cmp     si,dx */
    sub16(SI, DX, 0);
L52D6:
    /* 52D6  jne     L52C7 */
    if (!ZF) goto L52C7;
L52D8:
    /* 52D8  mov     si,bp */
    SI = BP;
L52DA:
    /* 52DA  sub     cx,si */
    CX = (uint16_t)(CX - SI);
L52DC:
    /* 52DC  shr     cx,1 */
    CX = shr16(CX, 1);
L52DE:
    /* 52DE  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L52E0:
    /* 52E0  pop     es */
    SET_ES(pop16());
L52E1:
    /* 52E1  pop     ds */
    SET_DS(pop16());
L52E2:
    /* 52E2  mov     word ptr ds:[2868h],di */
    ww(pDS, 0x2868, DI);
L52E6:
    /* 52E6  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L52E7: /* L52E7 */
    /* 52E7  mov     ax,bx */
    AX = BX;
L52E9:
    /* 52E9  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L52EA:
    /* 52EA  inc     bp */
    BP = (uint16_t)(BP + 1);
L52EB:
    /* 52EB  inc     bp */
    BP = (uint16_t)(BP + 1);
L52EC:
    /* 52EC  cmp     bp,cx */
    sub16(BP, CX, 0);
L52EE:
    /* 52EE  jne     L52C7 */
    if (!ZF) goto L52C7;
L52F0:
    /* 52F0  mov     cx,dx */
    CX = DX;
L52F2:
    /* 52F2  sub     cx,si */
    CX = (uint16_t)(CX - SI);
L52F4:
    /* 52F4  shr     cx,1 */
    CX = shr16(CX, 1);
L52F6:
    /* 52F6  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L52F8:
    /* 52F8  pop     es */
    SET_ES(pop16());
L52F9:
    /* 52F9  pop     ds */
    SET_DS(pop16());
L52FA:
    /* 52FA  mov     word ptr ds:[2868h],di */
    ww(pDS, 0x2868, DI);
L52FE:
    /* 52FE  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_52FF  (+52FF)
       UW1 only: the vector BX, CX, BP times the view matrix, in 32 bits, at 26D4h..26DEh (for
       INTERP.ASM's seg004_3DC6). */
L52FF: /* _seg004_52FF */
    /* 52FF  push    si */
    push16(SI);
L5300:
    /* 5300  mov     ax,bx */
    AX = BX;
L5302:
    /* 5302  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L5306:
    /* 5306  mov     si,ax */
    SI = AX;
L5308:
    /* 5308  mov     di,dx */
    DI = DX;
L530A:
    /* 530A  mov     ax,cx */
    AX = CX;
L530C:
    /* 530C  imul    word ptr ds:[MAT_YX] */
    imul16(rw(pDS, 0x1608));
L5310:
    /* 5310  add     si,ax */
    SI = add16(SI, AX, 0);
L5312:
    /* 5312  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L5314:
    /* 5314  mov     ax,bp */
    AX = BP;
L5316:
    /* 5316  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L531A:
    /* 531A  add     si,ax */
    SI = add16(SI, AX, 0);
L531C:
    /* 531C  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L531E:
    /* 531E  mov     word ptr ds:[26D4h],si */
    ww(pDS, 0x26D4, SI);
L5322:
    /* 5322  mov     word ptr ds:[26D6h],di */
    ww(pDS, 0x26D6, DI);
L5326:
    /* 5326  mov     ax,bx */
    AX = BX;
L5328:
    /* 5328  imul    word ptr ds:[MAT_XY] */
    imul16(rw(pDS, 0x1604));
L532C:
    /* 532C  mov     si,ax */
    SI = AX;
L532E:
    /* 532E  mov     di,dx */
    DI = DX;
L5330:
    /* 5330  mov     ax,cx */
    AX = CX;
L5332:
    /* 5332  imul    word ptr ds:[MAT_YY] */
    imul16(rw(pDS, 0x160A));
L5336:
    /* 5336  add     si,ax */
    SI = add16(SI, AX, 0);
L5338:
    /* 5338  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L533A:
    /* 533A  mov     ax,bp */
    AX = BP;
L533C:
    /* 533C  imul    word ptr ds:[MAT_ZY] */
    imul16(rw(pDS, 0x1610));
L5340:
    /* 5340  add     si,ax */
    SI = add16(SI, AX, 0);
L5342:
    /* 5342  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L5344:
    /* 5344  mov     word ptr ds:[26D8h],si */
    ww(pDS, 0x26D8, SI);
L5348:
    /* 5348  mov     word ptr ds:[26DAh],di */
    ww(pDS, 0x26DA, DI);
L534C:
    /* 534C  mov     ax,bx */
    AX = BX;
L534E:
    /* 534E  imul    word ptr ds:[MAT_XZ] */
    imul16(rw(pDS, 0x1606));
L5352:
    /* 5352  mov     si,ax */
    SI = AX;
L5354:
    /* 5354  mov     di,dx */
    DI = DX;
L5356:
    /* 5356  mov     ax,cx */
    AX = CX;
L5358:
    /* 5358  imul    word ptr ds:[MAT_YZ] */
    imul16(rw(pDS, 0x160C));
L535C:
    /* 535C  add     si,ax */
    SI = add16(SI, AX, 0);
L535E:
    /* 535E  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L5360:
    /* 5360  mov     ax,bp */
    AX = BP;
L5362:
    /* 5362  imul    word ptr ds:[MAT_ZZ] */
    imul16(rw(pDS, 0x1612));
L5366:
    /* 5366  add     si,ax */
    SI = add16(SI, AX, 0);
L5368:
    /* 5368  adc     di,dx */
    DI = add16(DI, DX, CF);
L536A:
    /* 536A  mov     word ptr ds:[26DCh],si */
    ww(pDS, 0x26DC, SI);
L536E:
    /* 536E  mov     word ptr ds:[26DEh],di */
    ww(pDS, 0x26DE, DI);
L5372:
    /* 5372  pop     si */
    SI = pop16();
L5373:
    /* 5373  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_5374  (+5374)
       UW1 only: as seg004_52FF, the results at 26E0h..26EAh. */
L5374: /* _seg004_5374 */
    /* 5374  push    si */
    push16(SI);
L5375:
    /* 5375  mov     ax,bx */
    AX = BX;
L5377:
    /* 5377  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L537B:
    /* 537B  mov     si,ax */
    SI = AX;
L537D:
    /* 537D  mov     di,dx */
    DI = DX;
L537F:
    /* 537F  mov     ax,cx */
    AX = CX;
L5381:
    /* 5381  imul    word ptr ds:[MAT_YX] */
    imul16(rw(pDS, 0x1608));
L5385:
    /* 5385  add     si,ax */
    SI = add16(SI, AX, 0);
L5387:
    /* 5387  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L5389:
    /* 5389  mov     ax,bp */
    AX = BP;
L538B:
    /* 538B  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L538F:
    /* 538F  add     si,ax */
    SI = add16(SI, AX, 0);
L5391:
    /* 5391  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L5393:
    /* 5393  mov     word ptr ds:[26E0h],si */
    ww(pDS, 0x26E0, SI);
L5397:
    /* 5397  mov     word ptr ds:[26E2h],di */
    ww(pDS, 0x26E2, DI);
L539B:
    /* 539B  mov     ax,bx */
    AX = BX;
L539D:
    /* 539D  imul    word ptr ds:[MAT_XY] */
    imul16(rw(pDS, 0x1604));
L53A1:
    /* 53A1  mov     si,ax */
    SI = AX;
L53A3:
    /* 53A3  mov     di,dx */
    DI = DX;
L53A5:
    /* 53A5  mov     ax,cx */
    AX = CX;
L53A7:
    /* 53A7  imul    word ptr ds:[MAT_YY] */
    imul16(rw(pDS, 0x160A));
L53AB:
    /* 53AB  add     si,ax */
    SI = add16(SI, AX, 0);
L53AD:
    /* 53AD  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L53AF:
    /* 53AF  mov     ax,bp */
    AX = BP;
L53B1:
    /* 53B1  imul    word ptr ds:[MAT_ZY] */
    imul16(rw(pDS, 0x1610));
L53B5:
    /* 53B5  add     si,ax */
    SI = add16(SI, AX, 0);
L53B7:
    /* 53B7  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L53B9:
    /* 53B9  mov     word ptr ds:[26E4h],si */
    ww(pDS, 0x26E4, SI);
L53BD:
    /* 53BD  mov     word ptr ds:[26E6h],di */
    ww(pDS, 0x26E6, DI);
L53C1:
    /* 53C1  mov     ax,bx */
    AX = BX;
L53C3:
    /* 53C3  imul    word ptr ds:[MAT_XZ] */
    imul16(rw(pDS, 0x1606));
L53C7:
    /* 53C7  mov     si,ax */
    SI = AX;
L53C9:
    /* 53C9  mov     di,dx */
    DI = DX;
L53CB:
    /* 53CB  mov     ax,cx */
    AX = CX;
L53CD:
    /* 53CD  imul    word ptr ds:[MAT_YZ] */
    imul16(rw(pDS, 0x160C));
L53D1:
    /* 53D1  add     si,ax */
    SI = add16(SI, AX, 0);
L53D3:
    /* 53D3  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L53D5:
    /* 53D5  mov     ax,bp */
    AX = BP;
L53D7:
    /* 53D7  imul    word ptr ds:[MAT_ZZ] */
    imul16(rw(pDS, 0x1612));
L53DB:
    /* 53DB  add     si,ax */
    SI = add16(SI, AX, 0);
L53DD:
    /* 53DD  adc     di,dx */
    DI = add16(DI, DX, CF);
L53DF:
    /* 53DF  mov     word ptr ds:[26E8h],si */
    ww(pDS, 0x26E8, SI);
L53E3:
    /* 53E3  mov     word ptr ds:[26EAh],di */
    ww(pDS, 0x26EA, DI);
L53E7:
    /* 53E7  pop     si */
    SI = pop16();
L53E8:
    /* 53E8  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_53E9  (+53E9)
       instance_vvars_head: rotate the eye position in the object's frame (2886 x, 288A z) by the
       instance heading in 270A..270E, so later points can be taken relative to the rotated object. */
L53E9: /* _instance_vvars_head */
    /* 53E9  mov     ax,word ptr ds:[OBJ_X] */
    AX = rw(pDS, 0x2886);
L53EC:
    /* 53EC  imul    word ptr ds:[270Ah] */
    imul16(rw(pDS, 0x270A));
L53F0:
    /* 53F0  mov     cx,dx */
    CX = DX;
L53F2:
    /* 53F2  mov     bx,ax */
    BX = AX;
L53F4:
    /* 53F4  mov     ax,word ptr ds:[OBJ_Z] */
    AX = rw(pDS, 0x288A);
L53F7:
    /* 53F7  imul    word ptr ds:[270Eh] */
    imul16(rw(pDS, 0x270E));
L53FB:
    /* 53FB  add     bx,ax */
    BX = add16(BX, AX, 0);
L53FD:
    /* 53FD  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L53FF:
    /* 53FF  shl     bx,1 */
    BX = shl16(BX, 1);
L5401:
    /* 5401  rcl     cx,1 */
    CX = rcl16(CX, 1);
L5403:
    /* 5403  mov     ax,word ptr ds:[OBJ_X] */
    AX = rw(pDS, 0x2886);
L5406:
    /* 5406  mov     word ptr ds:[OBJ_X],cx */
    ww(pDS, 0x2886, CX);
L540A:
    /* 540A  imul    word ptr ds:[270Ch] */
    imul16(rw(pDS, 0x270C));
L540E:
    /* 540E  mov     bx,ax */
    BX = AX;
L5410:
    /* 5410  mov     cx,dx */
    CX = DX;
L5412:
    /* 5412  mov     ax,word ptr ds:[OBJ_Z] */
    AX = rw(pDS, 0x288A);
L5415:
    /* 5415  imul    word ptr ds:[270Ah] */
    imul16(rw(pDS, 0x270A));
L5419:
    /* 5419  add     ax,bx */
    AX = add16(AX, BX, 0);
L541B:
    /* 541B  adc     dx,cx */
    DX = (uint16_t)(DX + CX + CF);
L541D:
    /* 541D  shl     ax,1 */
    AX = shl16(AX, 1);
L541F:
    /* 541F  rcl     dx,1 */
    DX = rcl16(DX, 1);
L5421:
    /* 5421  mov     word ptr ds:[OBJ_Z],dx */
    ww(pDS, 0x288A, DX);
L5425:
    /* 5425  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_5426  (+5426)
       instance_head: enter a sub-object turned by a heading. In: AX = the angle. Gets sin and cos
       (sincos, IMATH.ASM, called with DS = SS), rotates the eye (instance_vvars_head) and multiplies
       the view matrix by the rotation about the vertical axis, saturating each entry to 1.15. The
       interpreter saves and restores the matrix around the sub-object (INTERP.ASM, opcode 50h and
       the do_ihcall family). */
L5426: /* _instance_head */
    /* 5426  mov     ax,ss */
    AX = asm_ss;
L5428:
    /* 5428  mov     ds,ax */
    SET_DS(AX);
L542A:
    /* 542A  call    far ptr _sincos */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x0A34), 0x06E7 + PORT_LOAD_SEG, 0x542F)) != 0) return c;
L542F:
    /* 542F  mov     dx,es */
    DX = asm_es;
L5431:
    /* 5431  mov     ds,dx */
    SET_DS(DX);
L5433:
    /* 5433  mov     word ptr ds:[270Ch],ax */
    ww(pDS, 0x270C, AX);
L5436:
    /* 5436  mov     word ptr ds:[270Ah],bx */
    ww(pDS, 0x270A, BX);
L543A:
    /* 543A  neg     ax */
    AX = (uint16_t)-AX;
L543C:
    /* 543C  mov     word ptr ds:[270Eh],ax */
    ww(pDS, 0x270E, AX);
L543F:
    /* 543F  call    _instance_vvars_head */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x53E9), 0x5442)) != 0) return c;
L5442:
    /* 5442  mov     ax,word ptr ds:[270Ah] */
    AX = rw(pDS, 0x270A);
L5445:
    /* 5445  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L5449:
    /* 5449  mov     bx,ax */
    BX = AX;
L544B:
    /* 544B  mov     cx,dx */
    CX = DX;
L544D:
    /* 544D  mov     ax,word ptr ds:[270Eh] */
    AX = rw(pDS, 0x270E);
L5450:
    /* 5450  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L5454:
    /* 5454  add     bx,ax */
    BX = add16(BX, AX, 0);
L5456:
    /* 5456  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5458:
    /* 5458  add     bx,bx */
    BX = add16(BX, BX, 0);
L545A:
    /* 545A  adc     cx,cx */
    CX = add16(CX, CX, CF);
L545C:
    /* 545C  jno     short L5468 */
    if (!OF) goto L5468;
L545E:
    /* 545E  jg      short L5465 */
    if (!ZF && SF == OF) goto L5465;
L5460:
    /* 5460  mov     cx,8000h */
    CX = 0x8000;
L5463:
    /* 5463  jmp     short L5468 */
    goto L5468;
L5465: /* L5465 */
    /* 5465  mov     cx,7FFFh */
    CX = 0x7FFF;
L5468: /* L5468 */
    /* 5468  mov     word ptr ds:[26D4h],cx */
    ww(pDS, 0x26D4, CX);
L546C:
    /* 546C  mov     ax,word ptr ds:[270Ah] */
    AX = rw(pDS, 0x270A);
L546F:
    /* 546F  imul    word ptr ds:[MAT_XY] */
    imul16(rw(pDS, 0x1604));
L5473:
    /* 5473  mov     bx,ax */
    BX = AX;
L5475:
    /* 5475  mov     cx,dx */
    CX = DX;
L5477:
    /* 5477  mov     ax,word ptr ds:[270Eh] */
    AX = rw(pDS, 0x270E);
L547A:
    /* 547A  imul    word ptr ds:[MAT_ZY] */
    imul16(rw(pDS, 0x1610));
L547E:
    /* 547E  add     bx,ax */
    BX = add16(BX, AX, 0);
L5480:
    /* 5480  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5482:
    /* 5482  add     bx,bx */
    BX = add16(BX, BX, 0);
L5484:
    /* 5484  adc     cx,cx */
    CX = add16(CX, CX, CF);
L5486:
    /* 5486  jno     short L5492 */
    if (!OF) goto L5492;
L5488:
    /* 5488  jg      short L548F */
    if (!ZF && SF == OF) goto L548F;
L548A:
    /* 548A  mov     cx,8000h */
    CX = 0x8000;
L548D:
    /* 548D  jmp     short L5492 */
    goto L5492;
L548F: /* L548F */
    /* 548F  mov     cx,7FFFh */
    CX = 0x7FFF;
L5492: /* L5492 */
    /* 5492  mov     word ptr ds:[26D6h],cx */
    ww(pDS, 0x26D6, CX);
L5496:
    /* 5496  mov     ax,word ptr ds:[270Ah] */
    AX = rw(pDS, 0x270A);
L5499:
    /* 5499  imul    word ptr ds:[MAT_XZ] */
    imul16(rw(pDS, 0x1606));
L549D:
    /* 549D  mov     bx,ax */
    BX = AX;
L549F:
    /* 549F  mov     cx,dx */
    CX = DX;
L54A1:
    /* 54A1  mov     ax,word ptr ds:[270Eh] */
    AX = rw(pDS, 0x270E);
L54A4:
    /* 54A4  imul    word ptr ds:[MAT_ZZ] */
    imul16(rw(pDS, 0x1612));
L54A8:
    /* 54A8  add     bx,ax */
    BX = add16(BX, AX, 0);
L54AA:
    /* 54AA  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L54AC:
    /* 54AC  add     bx,bx */
    BX = add16(BX, BX, 0);
L54AE:
    /* 54AE  adc     cx,cx */
    CX = add16(CX, CX, CF);
L54B0:
    /* 54B0  jno     short L54BC */
    if (!OF) goto L54BC;
L54B2:
    /* 54B2  jg      short L54B9 */
    if (!ZF && SF == OF) goto L54B9;
L54B4:
    /* 54B4  mov     cx,8000h */
    CX = 0x8000;
L54B7:
    /* 54B7  jmp     short L54BC */
    goto L54BC;
L54B9: /* L54B9 */
    /* 54B9  mov     cx,7FFFh */
    CX = 0x7FFF;
L54BC: /* L54BC */
    /* 54BC  mov     word ptr ds:[26D8h],cx */
    ww(pDS, 0x26D8, CX);
L54C0:
    /* 54C0  mov     ax,word ptr ds:[270Ch] */
    AX = rw(pDS, 0x270C);
L54C3:
    /* 54C3  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L54C7:
    /* 54C7  mov     bx,ax */
    BX = AX;
L54C9:
    /* 54C9  mov     cx,dx */
    CX = DX;
L54CB:
    /* 54CB  mov     ax,word ptr ds:[270Ah] */
    AX = rw(pDS, 0x270A);
L54CE:
    /* 54CE  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L54D2:
    /* 54D2  add     bx,ax */
    BX = add16(BX, AX, 0);
L54D4:
    /* 54D4  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L54D6:
    /* 54D6  add     bx,bx */
    BX = add16(BX, BX, 0);
L54D8:
    /* 54D8  adc     cx,cx */
    CX = add16(CX, CX, CF);
L54DA:
    /* 54DA  jno     short L54E6 */
    if (!OF) goto L54E6;
L54DC:
    /* 54DC  jg      short L54E3 */
    if (!ZF && SF == OF) goto L54E3;
L54DE:
    /* 54DE  mov     cx,8000h */
    CX = 0x8000;
L54E1:
    /* 54E1  jmp     short L54E6 */
    goto L54E6;
L54E3: /* L54E3 */
    /* 54E3  mov     cx,7FFFh */
    CX = 0x7FFF;
L54E6: /* L54E6 */
    /* 54E6  mov     word ptr ds:[MAT_ZX],cx */
    ww(pDS, 0x160E, CX);
L54EA:
    /* 54EA  mov     ax,word ptr ds:[270Ch] */
    AX = rw(pDS, 0x270C);
L54ED:
    /* 54ED  imul    word ptr ds:[MAT_XY] */
    imul16(rw(pDS, 0x1604));
L54F1:
    /* 54F1  mov     bx,ax */
    BX = AX;
L54F3:
    /* 54F3  mov     cx,dx */
    CX = DX;
L54F5:
    /* 54F5  mov     ax,word ptr ds:[270Ah] */
    AX = rw(pDS, 0x270A);
L54F8:
    /* 54F8  imul    word ptr ds:[MAT_ZY] */
    imul16(rw(pDS, 0x1610));
L54FC:
    /* 54FC  add     bx,ax */
    BX = add16(BX, AX, 0);
L54FE:
    /* 54FE  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5500:
    /* 5500  add     bx,bx */
    BX = add16(BX, BX, 0);
L5502:
    /* 5502  adc     cx,cx */
    CX = add16(CX, CX, CF);
L5504:
    /* 5504  jno     short L5510 */
    if (!OF) goto L5510;
L5506:
    /* 5506  jg      short L550D */
    if (!ZF && SF == OF) goto L550D;
L5508:
    /* 5508  mov     cx,8000h */
    CX = 0x8000;
L550B:
    /* 550B  jmp     short L5510 */
    goto L5510;
L550D: /* L550D */
    /* 550D  mov     cx,7FFFh */
    CX = 0x7FFF;
L5510: /* L5510 */
    /* 5510  mov     word ptr ds:[MAT_ZY],cx */
    ww(pDS, 0x1610, CX);
L5514:
    /* 5514  mov     ax,word ptr ds:[270Ch] */
    AX = rw(pDS, 0x270C);
L5517:
    /* 5517  imul    word ptr ds:[MAT_XZ] */
    imul16(rw(pDS, 0x1606));
L551B:
    /* 551B  mov     bx,ax */
    BX = AX;
L551D:
    /* 551D  mov     cx,dx */
    CX = DX;
L551F:
    /* 551F  mov     ax,word ptr ds:[270Ah] */
    AX = rw(pDS, 0x270A);
L5522:
    /* 5522  imul    word ptr ds:[MAT_ZZ] */
    imul16(rw(pDS, 0x1612));
L5526:
    /* 5526  add     bx,ax */
    BX = add16(BX, AX, 0);
L5528:
    /* 5528  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L552A:
    /* 552A  add     bx,bx */
    BX = add16(BX, BX, 0);
L552C:
    /* 552C  adc     cx,cx */
    CX = add16(CX, CX, CF);
L552E:
    /* 552E  jno     short L553A */
    if (!OF) goto L553A;
L5530:
    /* 5530  jg      short L5537 */
    if (!ZF && SF == OF) goto L5537;
L5532:
    /* 5532  mov     cx,8000h */
    CX = 0x8000;
L5535:
    /* 5535  jmp     short L553A */
    goto L553A;
L5537: /* L5537 */
    /* 5537  mov     cx,7FFFh */
    CX = 0x7FFF;
L553A: /* L553A */
    /* 553A  mov     word ptr ds:[MAT_ZZ],cx */
    ww(pDS, 0x1612, CX);
L553E:
    /* 553E  mov     ax,word ptr ds:[26D4h] */
    AX = rw(pDS, 0x26D4);
L5541:
    /* 5541  mov     word ptr ds:[MAT_XX],ax */
    ww(pDS, 0x1602, AX);
L5544:
    /* 5544  mov     ax,word ptr ds:[26D6h] */
    AX = rw(pDS, 0x26D6);
L5547:
    /* 5547  mov     word ptr ds:[MAT_XY],ax */
    ww(pDS, 0x1604, AX);
L554A:
    /* 554A  mov     ax,word ptr ds:[26D8h] */
    AX = rw(pDS, 0x26D8);
L554D:
    /* 554D  mov     word ptr ds:[MAT_XZ],ax */
    ww(pDS, 0x1606, AX);
L5550:
    /* 5550  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_5551  (+5551)
       instance_vvars_pitch and instance_pitch, instance_vvars_bank and instance_bank: as
       instance_vvars_head and instance_head for rotations about the other two axes (pitch and bank). */
L5551: /* _instance_vvars_pitch */
    /* 5551  mov     ax,word ptr ds:[OBJ_Y] */
    AX = rw(pDS, 0x2888);
L5554:
    /* 5554  imul    word ptr ds:[2716h] */
    imul16(rw(pDS, 0x2716));
L5558:
    /* 5558  mov     cx,dx */
    CX = DX;
L555A:
    /* 555A  mov     bx,ax */
    BX = AX;
L555C:
    /* 555C  mov     ax,word ptr ds:[OBJ_Z] */
    AX = rw(pDS, 0x288A);
L555F:
    /* 555F  imul    word ptr ds:[271Ah] */
    imul16(rw(pDS, 0x271A));
L5563:
    /* 5563  add     ax,bx */
    AX = add16(AX, BX, 0);
L5565:
    /* 5565  adc     dx,cx */
    DX = (uint16_t)(DX + CX + CF);
L5567:
    /* 5567  shl     ax,1 */
    AX = shl16(AX, 1);
L5569:
    /* 5569  rcl     dx,1 */
    DX = rcl16(DX, 1);
L556B:
    /* 556B  mov     ax,word ptr ds:[OBJ_Y] */
    AX = rw(pDS, 0x2888);
L556E:
    /* 556E  mov     word ptr ds:[OBJ_Y],dx */
    ww(pDS, 0x2888, DX);
L5572:
    /* 5572  imul    word ptr ds:[2718h] */
    imul16(rw(pDS, 0x2718));
L5576:
    /* 5576  mov     bx,ax */
    BX = AX;
L5578:
    /* 5578  mov     cx,dx */
    CX = DX;
L557A:
    /* 557A  mov     ax,word ptr ds:[OBJ_Z] */
    AX = rw(pDS, 0x288A);
L557D:
    /* 557D  imul    word ptr ds:[2716h] */
    imul16(rw(pDS, 0x2716));
L5581:
    /* 5581  add     ax,bx */
    AX = add16(AX, BX, 0);
L5583:
    /* 5583  adc     dx,cx */
    DX = (uint16_t)(DX + CX + CF);
L5585:
    /* 5585  shl     ax,1 */
    AX = shl16(AX, 1);
L5587:
    /* 5587  rcl     dx,1 */
    DX = rcl16(DX, 1);
L5589:
    /* 5589  mov     word ptr ds:[OBJ_Z],dx */
    ww(pDS, 0x288A, DX);
L558D:
    /* 558D  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_558E  (+558E) */
L558E: /* _instance_pitch */
    /* 558E  mov     ax,ss */
    AX = asm_ss;
L5590:
    /* 5590  mov     ds,ax */
    SET_DS(AX);
L5592:
    /* 5592  call    far ptr _sincos */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x0A34), 0x06E7 + PORT_LOAD_SEG, 0x5597)) != 0) return c;
L5597:
    /* 5597  mov     dx,es */
    DX = asm_es;
L5599:
    /* 5599  mov     ds,dx */
    SET_DS(DX);
L559B:
    /* 559B  mov     word ptr ds:[2718h],ax */
    ww(pDS, 0x2718, AX);
L559E:
    /* 559E  mov     word ptr ds:[2716h],bx */
    ww(pDS, 0x2716, BX);
L55A2:
    /* 55A2  neg     ax */
    AX = (uint16_t)-AX;
L55A4:
    /* 55A4  mov     word ptr ds:[271Ah],ax */
    ww(pDS, 0x271A, AX);
L55A7:
    /* 55A7  call    _instance_vvars_pitch */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5551), 0x55AA)) != 0) return c;
L55AA:
    /* 55AA  mov     ax,word ptr ds:[2716h] */
    AX = rw(pDS, 0x2716);
L55AD:
    /* 55AD  imul    word ptr ds:[MAT_YX] */
    imul16(rw(pDS, 0x1608));
L55B1:
    /* 55B1  mov     bx,ax */
    BX = AX;
L55B3:
    /* 55B3  mov     cx,dx */
    CX = DX;
L55B5:
    /* 55B5  mov     ax,word ptr ds:[271Ah] */
    AX = rw(pDS, 0x271A);
L55B8:
    /* 55B8  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L55BC:
    /* 55BC  add     bx,ax */
    BX = add16(BX, AX, 0);
L55BE:
    /* 55BE  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L55C0:
    /* 55C0  add     bx,bx */
    BX = add16(BX, BX, 0);
L55C2:
    /* 55C2  adc     cx,cx */
    CX = add16(CX, CX, CF);
L55C4:
    /* 55C4  jno     short L55D0 */
    if (!OF) goto L55D0;
L55C6:
    /* 55C6  jg      short L55CD */
    if (!ZF && SF == OF) goto L55CD;
L55C8:
    /* 55C8  mov     cx,8000h */
    CX = 0x8000;
L55CB:
    /* 55CB  jmp     short L55D0 */
    goto L55D0;
L55CD: /* L55CD */
    /* 55CD  mov     cx,7FFFh */
    CX = 0x7FFF;
L55D0: /* L55D0 */
    /* 55D0  mov     word ptr ds:[26D4h],cx */
    ww(pDS, 0x26D4, CX);
L55D4:
    /* 55D4  mov     ax,word ptr ds:[2716h] */
    AX = rw(pDS, 0x2716);
L55D7:
    /* 55D7  imul    word ptr ds:[MAT_YY] */
    imul16(rw(pDS, 0x160A));
L55DB:
    /* 55DB  mov     bx,ax */
    BX = AX;
L55DD:
    /* 55DD  mov     cx,dx */
    CX = DX;
L55DF:
    /* 55DF  mov     ax,word ptr ds:[271Ah] */
    AX = rw(pDS, 0x271A);
L55E2:
    /* 55E2  imul    word ptr ds:[MAT_ZY] */
    imul16(rw(pDS, 0x1610));
L55E6:
    /* 55E6  add     bx,ax */
    BX = add16(BX, AX, 0);
L55E8:
    /* 55E8  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L55EA:
    /* 55EA  add     bx,bx */
    BX = add16(BX, BX, 0);
L55EC:
    /* 55EC  adc     cx,cx */
    CX = add16(CX, CX, CF);
L55EE:
    /* 55EE  jno     short L55FA */
    if (!OF) goto L55FA;
L55F0:
    /* 55F0  jg      short L55F7 */
    if (!ZF && SF == OF) goto L55F7;
L55F2:
    /* 55F2  mov     cx,8000h */
    CX = 0x8000;
L55F5:
    /* 55F5  jmp     short L55FA */
    goto L55FA;
L55F7: /* L55F7 */
    /* 55F7  mov     cx,7FFFh */
    CX = 0x7FFF;
L55FA: /* L55FA */
    /* 55FA  mov     word ptr ds:[26D6h],cx */
    ww(pDS, 0x26D6, CX);
L55FE:
    /* 55FE  mov     ax,word ptr ds:[2716h] */
    AX = rw(pDS, 0x2716);
L5601:
    /* 5601  imul    word ptr ds:[MAT_YZ] */
    imul16(rw(pDS, 0x160C));
L5605:
    /* 5605  mov     bx,ax */
    BX = AX;
L5607:
    /* 5607  mov     cx,dx */
    CX = DX;
L5609:
    /* 5609  mov     ax,word ptr ds:[271Ah] */
    AX = rw(pDS, 0x271A);
L560C:
    /* 560C  imul    word ptr ds:[MAT_ZZ] */
    imul16(rw(pDS, 0x1612));
L5610:
    /* 5610  add     bx,ax */
    BX = add16(BX, AX, 0);
L5612:
    /* 5612  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5614:
    /* 5614  add     bx,bx */
    BX = add16(BX, BX, 0);
L5616:
    /* 5616  adc     cx,cx */
    CX = add16(CX, CX, CF);
L5618:
    /* 5618  jno     short L5624 */
    if (!OF) goto L5624;
L561A:
    /* 561A  jg      short L5621 */
    if (!ZF && SF == OF) goto L5621;
L561C:
    /* 561C  mov     cx,8000h */
    CX = 0x8000;
L561F:
    /* 561F  jmp     short L5624 */
    goto L5624;
L5621: /* L5621 */
    /* 5621  mov     cx,7FFFh */
    CX = 0x7FFF;
L5624: /* L5624 */
    /* 5624  mov     word ptr ds:[26D8h],cx */
    ww(pDS, 0x26D8, CX);
L5628:
    /* 5628  mov     ax,word ptr ds:[2718h] */
    AX = rw(pDS, 0x2718);
L562B:
    /* 562B  imul    word ptr ds:[MAT_YX] */
    imul16(rw(pDS, 0x1608));
L562F:
    /* 562F  mov     bx,ax */
    BX = AX;
L5631:
    /* 5631  mov     cx,dx */
    CX = DX;
L5633:
    /* 5633  mov     ax,word ptr ds:[2716h] */
    AX = rw(pDS, 0x2716);
L5636:
    /* 5636  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L563A:
    /* 563A  add     bx,ax */
    BX = add16(BX, AX, 0);
L563C:
    /* 563C  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L563E:
    /* 563E  add     bx,bx */
    BX = add16(BX, BX, 0);
L5640:
    /* 5640  adc     cx,cx */
    CX = add16(CX, CX, CF);
L5642:
    /* 5642  jno     short L564E */
    if (!OF) goto L564E;
L5644:
    /* 5644  jg      short L564B */
    if (!ZF && SF == OF) goto L564B;
L5646:
    /* 5646  mov     cx,8000h */
    CX = 0x8000;
L5649:
    /* 5649  jmp     short L564E */
    goto L564E;
L564B: /* L564B */
    /* 564B  mov     cx,7FFFh */
    CX = 0x7FFF;
L564E: /* L564E */
    /* 564E  mov     word ptr ds:[MAT_ZX],cx */
    ww(pDS, 0x160E, CX);
L5652:
    /* 5652  mov     ax,word ptr ds:[2718h] */
    AX = rw(pDS, 0x2718);
L5655:
    /* 5655  imul    word ptr ds:[MAT_YY] */
    imul16(rw(pDS, 0x160A));
L5659:
    /* 5659  mov     bx,ax */
    BX = AX;
L565B:
    /* 565B  mov     cx,dx */
    CX = DX;
L565D:
    /* 565D  mov     ax,word ptr ds:[2716h] */
    AX = rw(pDS, 0x2716);
L5660:
    /* 5660  imul    word ptr ds:[MAT_ZY] */
    imul16(rw(pDS, 0x1610));
L5664:
    /* 5664  add     bx,ax */
    BX = add16(BX, AX, 0);
L5666:
    /* 5666  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5668:
    /* 5668  add     bx,bx */
    BX = add16(BX, BX, 0);
L566A:
    /* 566A  adc     cx,cx */
    CX = add16(CX, CX, CF);
L566C:
    /* 566C  jno     short L5678 */
    if (!OF) goto L5678;
L566E:
    /* 566E  jg      short L5675 */
    if (!ZF && SF == OF) goto L5675;
L5670:
    /* 5670  mov     cx,8000h */
    CX = 0x8000;
L5673:
    /* 5673  jmp     short L5678 */
    goto L5678;
L5675: /* L5675 */
    /* 5675  mov     cx,7FFFh */
    CX = 0x7FFF;
L5678: /* L5678 */
    /* 5678  mov     word ptr ds:[MAT_ZY],cx */
    ww(pDS, 0x1610, CX);
L567C:
    /* 567C  mov     ax,word ptr ds:[2718h] */
    AX = rw(pDS, 0x2718);
L567F:
    /* 567F  imul    word ptr ds:[MAT_YZ] */
    imul16(rw(pDS, 0x160C));
L5683:
    /* 5683  mov     bx,ax */
    BX = AX;
L5685:
    /* 5685  mov     cx,dx */
    CX = DX;
L5687:
    /* 5687  mov     ax,word ptr ds:[2716h] */
    AX = rw(pDS, 0x2716);
L568A:
    /* 568A  imul    word ptr ds:[MAT_ZZ] */
    imul16(rw(pDS, 0x1612));
L568E:
    /* 568E  add     bx,ax */
    BX = add16(BX, AX, 0);
L5690:
    /* 5690  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5692:
    /* 5692  add     bx,bx */
    BX = add16(BX, BX, 0);
L5694:
    /* 5694  adc     cx,cx */
    CX = add16(CX, CX, CF);
L5696:
    /* 5696  jno     short L56A2 */
    if (!OF) goto L56A2;
L5698:
    /* 5698  jg      short L569F */
    if (!ZF && SF == OF) goto L569F;
L569A:
    /* 569A  mov     cx,8000h */
    CX = 0x8000;
L569D:
    /* 569D  jmp     short L56A2 */
    goto L56A2;
L569F: /* L569F */
    /* 569F  mov     cx,7FFFh */
    CX = 0x7FFF;
L56A2: /* L56A2 */
    /* 56A2  mov     word ptr ds:[MAT_ZZ],cx */
    ww(pDS, 0x1612, CX);
L56A6:
    /* 56A6  mov     ax,word ptr ds:[26D4h] */
    AX = rw(pDS, 0x26D4);
L56A9:
    /* 56A9  mov     word ptr ds:[MAT_YX],ax */
    ww(pDS, 0x1608, AX);
L56AC:
    /* 56AC  mov     ax,word ptr ds:[26D6h] */
    AX = rw(pDS, 0x26D6);
L56AF:
    /* 56AF  mov     word ptr ds:[MAT_YY],ax */
    ww(pDS, 0x160A, AX);
L56B2:
    /* 56B2  mov     ax,word ptr ds:[26D8h] */
    AX = rw(pDS, 0x26D8);
L56B5:
    /* 56B5  mov     word ptr ds:[MAT_YZ],ax */
    ww(pDS, 0x160C, AX);
L56B8:
    /* 56B8  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_56B9  (+56B9) */
L56B9: /* _instance_vvars_bank */
    /* 56B9  mov     ax,word ptr ds:[OBJ_Y] */
    AX = rw(pDS, 0x2888);
L56BC:
    /* 56BC  imul    word ptr ds:[2710h] */
    imul16(rw(pDS, 0x2710));
L56C0:
    /* 56C0  mov     bx,ax */
    BX = AX;
L56C2:
    /* 56C2  mov     cx,dx */
    CX = DX;
L56C4:
    /* 56C4  mov     ax,word ptr ds:[OBJ_X] */
    AX = rw(pDS, 0x2886);
L56C7:
    /* 56C7  imul    word ptr ds:[2714h] */
    imul16(rw(pDS, 0x2714));
L56CB:
    /* 56CB  add     ax,bx */
    AX = add16(AX, BX, 0);
L56CD:
    /* 56CD  adc     dx,cx */
    DX = (uint16_t)(DX + CX + CF);
L56CF:
    /* 56CF  shl     ax,1 */
    AX = shl16(AX, 1);
L56D1:
    /* 56D1  rcl     dx,1 */
    DX = rcl16(DX, 1);
L56D3:
    /* 56D3  mov     ax,word ptr ds:[OBJ_Y] */
    AX = rw(pDS, 0x2888);
L56D6:
    /* 56D6  mov     word ptr ds:[OBJ_Y],dx */
    ww(pDS, 0x2888, DX);
L56DA:
    /* 56DA  imul    word ptr ds:[2712h] */
    imul16(rw(pDS, 0x2712));
L56DE:
    /* 56DE  mov     cx,dx */
    CX = DX;
L56E0:
    /* 56E0  mov     bx,ax */
    BX = AX;
L56E2:
    /* 56E2  mov     ax,word ptr ds:[OBJ_X] */
    AX = rw(pDS, 0x2886);
L56E5:
    /* 56E5  imul    word ptr ds:[2710h] */
    imul16(rw(pDS, 0x2710));
L56E9:
    /* 56E9  add     bx,ax */
    BX = add16(BX, AX, 0);
L56EB:
    /* 56EB  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L56ED:
    /* 56ED  shl     bx,1 */
    BX = shl16(BX, 1);
L56EF:
    /* 56EF  rcl     cx,1 */
    CX = rcl16(CX, 1);
L56F1:
    /* 56F1  mov     word ptr ds:[OBJ_X],cx */
    ww(pDS, 0x2886, CX);
L56F5:
    /* 56F5  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_56F6  (+56F6) */
L56F6: /* _instance_bank */
    /* 56F6  mov     ax,ss */
    AX = asm_ss;
L56F8:
    /* 56F8  mov     ds,ax */
    SET_DS(AX);
L56FA:
    /* 56FA  call    far ptr _sincos */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x0A34), 0x06E7 + PORT_LOAD_SEG, 0x56FF)) != 0) return c;
L56FF:
    /* 56FF  mov     dx,es */
    DX = asm_es;
L5701:
    /* 5701  mov     ds,dx */
    SET_DS(DX);
L5703:
    /* 5703  mov     word ptr ds:[2712h],ax */
    ww(pDS, 0x2712, AX);
L5706:
    /* 5706  mov     word ptr ds:[2710h],bx */
    ww(pDS, 0x2710, BX);
L570A:
    /* 570A  neg     ax */
    AX = (uint16_t)-AX;
L570C:
    /* 570C  mov     word ptr ds:[2714h],ax */
    ww(pDS, 0x2714, AX);
L570F:
    /* 570F  call    _instance_vvars_bank */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x56B9), 0x5712)) != 0) return c;
L5712:
    /* 5712  mov     ax,word ptr ds:[2710h] */
    AX = rw(pDS, 0x2710);
L5715:
    /* 5715  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L5719:
    /* 5719  mov     bx,ax */
    BX = AX;
L571B:
    /* 571B  mov     cx,dx */
    CX = DX;
L571D:
    /* 571D  mov     ax,word ptr ds:[2712h] */
    AX = rw(pDS, 0x2712);
L5720:
    /* 5720  imul    word ptr ds:[MAT_YX] */
    imul16(rw(pDS, 0x1608));
L5724:
    /* 5724  add     bx,ax */
    BX = add16(BX, AX, 0);
L5726:
    /* 5726  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5728:
    /* 5728  add     bx,bx */
    BX = add16(BX, BX, 0);
L572A:
    /* 572A  adc     cx,cx */
    CX = add16(CX, CX, CF);
L572C:
    /* 572C  jno     short L5738 */
    if (!OF) goto L5738;
L572E:
    /* 572E  jg      short L5735 */
    if (!ZF && SF == OF) goto L5735;
L5730:
    /* 5730  mov     cx,8000h */
    CX = 0x8000;
L5733:
    /* 5733  jmp     short L5738 */
    goto L5738;
L5735: /* L5735 */
    /* 5735  mov     cx,7FFFh */
    CX = 0x7FFF;
L5738: /* L5738 */
    /* 5738  mov     word ptr ds:[26D4h],cx */
    ww(pDS, 0x26D4, CX);
L573C:
    /* 573C  mov     ax,word ptr ds:[2710h] */
    AX = rw(pDS, 0x2710);
L573F:
    /* 573F  imul    word ptr ds:[MAT_XY] */
    imul16(rw(pDS, 0x1604));
L5743:
    /* 5743  mov     bx,ax */
    BX = AX;
L5745:
    /* 5745  mov     cx,dx */
    CX = DX;
L5747:
    /* 5747  mov     ax,word ptr ds:[2712h] */
    AX = rw(pDS, 0x2712);
L574A:
    /* 574A  imul    word ptr ds:[MAT_YY] */
    imul16(rw(pDS, 0x160A));
L574E:
    /* 574E  add     bx,ax */
    BX = add16(BX, AX, 0);
L5750:
    /* 5750  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5752:
    /* 5752  add     bx,bx */
    BX = add16(BX, BX, 0);
L5754:
    /* 5754  adc     cx,cx */
    CX = add16(CX, CX, CF);
L5756:
    /* 5756  jno     short L5762 */
    if (!OF) goto L5762;
L5758:
    /* 5758  jg      short L575F */
    if (!ZF && SF == OF) goto L575F;
L575A:
    /* 575A  mov     cx,8000h */
    CX = 0x8000;
L575D:
    /* 575D  jmp     short L5762 */
    goto L5762;
L575F: /* L575F */
    /* 575F  mov     cx,7FFFh */
    CX = 0x7FFF;
L5762: /* L5762 */
    /* 5762  mov     word ptr ds:[26D6h],cx */
    ww(pDS, 0x26D6, CX);
L5766:
    /* 5766  mov     ax,word ptr ds:[2710h] */
    AX = rw(pDS, 0x2710);
L5769:
    /* 5769  imul    word ptr ds:[MAT_XZ] */
    imul16(rw(pDS, 0x1606));
L576D:
    /* 576D  mov     bx,ax */
    BX = AX;
L576F:
    /* 576F  mov     cx,dx */
    CX = DX;
L5771:
    /* 5771  mov     ax,word ptr ds:[2712h] */
    AX = rw(pDS, 0x2712);
L5774:
    /* 5774  imul    word ptr ds:[MAT_YZ] */
    imul16(rw(pDS, 0x160C));
L5778:
    /* 5778  add     bx,ax */
    BX = add16(BX, AX, 0);
L577A:
    /* 577A  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L577C:
    /* 577C  add     bx,bx */
    BX = add16(BX, BX, 0);
L577E:
    /* 577E  adc     cx,cx */
    CX = add16(CX, CX, CF);
L5780:
    /* 5780  jno     short L578C */
    if (!OF) goto L578C;
L5782:
    /* 5782  jg      short L5789 */
    if (!ZF && SF == OF) goto L5789;
L5784:
    /* 5784  mov     cx,8000h */
    CX = 0x8000;
L5787:
    /* 5787  jmp     short L578C */
    goto L578C;
L5789: /* L5789 */
    /* 5789  mov     cx,7FFFh */
    CX = 0x7FFF;
L578C: /* L578C */
    /* 578C  mov     word ptr ds:[26D8h],cx */
    ww(pDS, 0x26D8, CX);
L5790:
    /* 5790  mov     ax,word ptr ds:[2714h] */
    AX = rw(pDS, 0x2714);
L5793:
    /* 5793  imul    word ptr ds:[MAT_XX] */
    imul16(rw(pDS, 0x1602));
L5797:
    /* 5797  mov     bx,ax */
    BX = AX;
L5799:
    /* 5799  mov     cx,dx */
    CX = DX;
L579B:
    /* 579B  mov     ax,word ptr ds:[2710h] */
    AX = rw(pDS, 0x2710);
L579E:
    /* 579E  imul    word ptr ds:[MAT_YX] */
    imul16(rw(pDS, 0x1608));
L57A2:
    /* 57A2  add     bx,ax */
    BX = add16(BX, AX, 0);
L57A4:
    /* 57A4  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L57A6:
    /* 57A6  add     bx,bx */
    BX = add16(BX, BX, 0);
L57A8:
    /* 57A8  adc     cx,cx */
    CX = add16(CX, CX, CF);
L57AA:
    /* 57AA  jno     short L57B6 */
    if (!OF) goto L57B6;
L57AC:
    /* 57AC  jg      short L57B3 */
    if (!ZF && SF == OF) goto L57B3;
L57AE:
    /* 57AE  mov     cx,8000h */
    CX = 0x8000;
L57B1:
    /* 57B1  jmp     short L57B6 */
    goto L57B6;
L57B3: /* L57B3 */
    /* 57B3  mov     cx,7FFFh */
    CX = 0x7FFF;
L57B6: /* L57B6 */
    /* 57B6  mov     word ptr ds:[MAT_YX],cx */
    ww(pDS, 0x1608, CX);
L57BA:
    /* 57BA  mov     ax,word ptr ds:[2714h] */
    AX = rw(pDS, 0x2714);
L57BD:
    /* 57BD  imul    word ptr ds:[MAT_XY] */
    imul16(rw(pDS, 0x1604));
L57C1:
    /* 57C1  mov     bx,ax */
    BX = AX;
L57C3:
    /* 57C3  mov     cx,dx */
    CX = DX;
L57C5:
    /* 57C5  mov     ax,word ptr ds:[2710h] */
    AX = rw(pDS, 0x2710);
L57C8:
    /* 57C8  imul    word ptr ds:[MAT_YY] */
    imul16(rw(pDS, 0x160A));
L57CC:
    /* 57CC  add     bx,ax */
    BX = add16(BX, AX, 0);
L57CE:
    /* 57CE  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L57D0:
    /* 57D0  add     bx,bx */
    BX = add16(BX, BX, 0);
L57D2:
    /* 57D2  adc     cx,cx */
    CX = add16(CX, CX, CF);
L57D4:
    /* 57D4  jno     short L57E0 */
    if (!OF) goto L57E0;
L57D6:
    /* 57D6  jg      short L57DD */
    if (!ZF && SF == OF) goto L57DD;
L57D8:
    /* 57D8  mov     cx,8000h */
    CX = 0x8000;
L57DB:
    /* 57DB  jmp     short L57E0 */
    goto L57E0;
L57DD: /* L57DD */
    /* 57DD  mov     cx,7FFFh */
    CX = 0x7FFF;
L57E0: /* L57E0 */
    /* 57E0  mov     word ptr ds:[MAT_YY],cx */
    ww(pDS, 0x160A, CX);
L57E4:
    /* 57E4  mov     ax,word ptr ds:[2714h] */
    AX = rw(pDS, 0x2714);
L57E7:
    /* 57E7  imul    word ptr ds:[MAT_XZ] */
    imul16(rw(pDS, 0x1606));
L57EB:
    /* 57EB  mov     bx,ax */
    BX = AX;
L57ED:
    /* 57ED  mov     cx,dx */
    CX = DX;
L57EF:
    /* 57EF  mov     ax,word ptr ds:[2710h] */
    AX = rw(pDS, 0x2710);
L57F2:
    /* 57F2  imul    word ptr ds:[MAT_YZ] */
    imul16(rw(pDS, 0x160C));
L57F6:
    /* 57F6  add     bx,ax */
    BX = add16(BX, AX, 0);
L57F8:
    /* 57F8  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L57FA:
    /* 57FA  add     bx,bx */
    BX = add16(BX, BX, 0);
L57FC:
    /* 57FC  adc     cx,cx */
    CX = add16(CX, CX, CF);
L57FE:
    /* 57FE  jno     short L580A */
    if (!OF) goto L580A;
L5800:
    /* 5800  jg      short L5807 */
    if (!ZF && SF == OF) goto L5807;
L5802:
    /* 5802  mov     cx,8000h */
    CX = 0x8000;
L5805:
    /* 5805  jmp     short L580A */
    goto L580A;
L5807: /* L5807 */
    /* 5807  mov     cx,7FFFh */
    CX = 0x7FFF;
L580A: /* L580A */
    /* 580A  mov     word ptr ds:[MAT_YZ],cx */
    ww(pDS, 0x160C, CX);
L580E:
    /* 580E  mov     ax,word ptr ds:[26D4h] */
    AX = rw(pDS, 0x26D4);
L5811:
    /* 5811  mov     word ptr ds:[MAT_XX],ax */
    ww(pDS, 0x1602, AX);
L5814:
    /* 5814  mov     ax,word ptr ds:[26D6h] */
    AX = rw(pDS, 0x26D6);
L5817:
    /* 5817  mov     word ptr ds:[MAT_XY],ax */
    ww(pDS, 0x1604, AX);
L581A:
    /* 581A  mov     ax,word ptr ds:[26D8h] */
    AX = rw(pDS, 0x26D8);
L581D:
    /* 581D  mov     word ptr ds:[MAT_XZ],ax */
    ww(pDS, 0x1606, AX);
L5820:
    /* 5820  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_5821  (+5821)
       UW1 only: the object origin (2886h, 2888h, 288Ah) times the matrix at BP (seg004_5937),
       stored back. */
L5821: /* _seg004_5821 */
    /* 5821  mov     si,OBJ_X */
    SI = 0x2886;
L5824:
    /* 5824  call    _seg004_5937 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5937), 0x5827)) != 0) return c;
L5827:
    /* 5827  mov     word ptr ds:[OBJ_X],bx */
    ww(pDS, 0x2886, BX);
L582B:
    /* 582B  mov     word ptr ds:[OBJ_Y],cx */
    ww(pDS, 0x2888, CX);
L582F:
    /* 582F  mov     word ptr ds:[OBJ_Z],di */
    ww(pDS, 0x288A, DI);
L5833:
    /* 5833  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_5834  (+5834)
       UW1 only: seg004_5821, then the view matrix (1602h..1612h) from the matrix at SI (in SS)
       and the one at BP, a column at a time (seg004_5A83). Called by SPHERE.ASM's seg004_17AB. */
L5834: /* _seg004_5834 */
    /* 5834  push    si */
    push16(SI);
L5835:
    /* 5835  call    _seg004_5821 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5821), 0x5838)) != 0) return c;
L5838:
    /* 5838  mov     si,bp */
    SI = BP;
L583A:
    /* 583A  pop     bp */
    BP = pop16();
L583B:
    /* 583B  push    ds */
    push16(asm_ds);
L583C:
    /* 583C  mov     ax,ss */
    AX = asm_ss;
L583E:
    /* 583E  mov     ds,ax */
    SET_DS(AX);
L5840:
    /* 5840  call    _seg004_5A83 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5A83), 0x5843)) != 0) return c;
L5843:
    /* 5843  mov     word ptr es:[MAT_XX],bx */
    ww(pES, 0x1602, BX);
L5848:
    /* 5848  mov     word ptr es:[MAT_XY],cx */
    ww(pES, 0x1604, CX);
L584D:
    /* 584D  mov     word ptr es:[MAT_XZ],di */
    ww(pES, 0x1606, DI);
L5852:
    /* 5852  add     si,2 */
    SI = (uint16_t)(SI + 0x2);
L5855:
    /* 5855  call    _seg004_5A83 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5A83), 0x5858)) != 0) return c;
L5858:
    /* 5858  mov     word ptr es:[MAT_YX],bx */
    ww(pES, 0x1608, BX);
L585D:
    /* 585D  mov     word ptr es:[MAT_YY],cx */
    ww(pES, 0x160A, CX);
L5862:
    /* 5862  mov     word ptr es:[MAT_YZ],di */
    ww(pES, 0x160C, DI);
L5867:
    /* 5867  add     si,2 */
    SI = (uint16_t)(SI + 0x2);
L586A:
    /* 586A  call    _seg004_5A83 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5A83), 0x586D)) != 0) return c;
L586D:
    /* 586D  mov     word ptr es:[MAT_ZX],bx */
    ww(pES, 0x160E, BX);
L5872:
    /* 5872  mov     word ptr es:[MAT_ZY],cx */
    ww(pES, 0x1610, CX);
L5877:
    /* 5877  mov     word ptr es:[MAT_ZZ],di */
    ww(pES, 0x1612, DI);
L587C:
    /* 587C  pop     ds */
    SET_DS(pop16());
L587D:
    /* 587D  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_587E  (+587E)
       code_pnt: the clip codes of a view-space point. In: BX = x, CX = y, BP = z. Out: AL = 0 inside
       the frustum, else bits 1 (x < -z), 2 (x > z), 4 (y > z), 8 (y < -z), and 80h when z <= 0
       (behind the eye, with the side bits set from the signs). The four planes are CLIP.ASM's.
       Changes AX only. */
L587E: /* _code_pnt */
    /* 587E  mov     ax,bp */
    AX = BP;
L5880:
    /* 5880  neg     ax */
    AX = neg16(AX);
L5882:
    /* 5882  jge     short L58BF */
    if (SF == OF) goto L58BF;
L5884:
    /* 5884  cmp     bx,ax */
    sub16(BX, AX, 0);
L5886:
    /* 5886  jl      short L5897 */
    if (SF != OF) goto L5897;
L5888:
    /* 5888  cmp     bx,bp */
    sub16(BX, BP, 0);
L588A:
    /* 588A  jg      short L58A8 */
    if (!ZF && SF == OF) goto L58A8;
L588C:
    /* 588C  cmp     cx,bp */
    sub16(CX, BP, 0);
L588E:
    /* 588E  jg      short L58B9 */
    if (!ZF && SF == OF) goto L58B9;
L5890:
    /* 5890  cmp     cx,ax */
    sub16(CX, AX, 0);
L5892:
    /* 5892  jl      short L58BC */
    if (SF != OF) goto L58BC;
L5894:
    /* 5894  xor     al,al */
    AL = logic8((uint8_t)(AL ^ AL));
L5896:
    /* 5896  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L5897: /* L5897 */
    /* 5897  cmp     cx,bp */
    sub16(CX, BP, 0);
L5899:
    /* 5899  jg      short L58A2 */
    if (!ZF && SF == OF) goto L58A2;
L589B:
    /* 589B  cmp     cx,ax */
    sub16(CX, AX, 0);
L589D:
    /* 589D  jl      short L58A5 */
    if (SF != OF) goto L58A5;
L589F:
    /* 589F  mov     al,1 */
    AL = 0x1;
L58A1:
    /* 58A1  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L58A2: /* L58A2 */
    /* 58A2  mov     al,5 */
    AL = 0x5;
L58A4:
    /* 58A4  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L58A5: /* L58A5 */
    /* 58A5  mov     al,9 */
    AL = 0x9;
L58A7:
    /* 58A7  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L58A8: /* L58A8 */
    /* 58A8  cmp     cx,bp */
    sub16(CX, BP, 0);
L58AA:
    /* 58AA  jg      short L58B3 */
    if (!ZF && SF == OF) goto L58B3;
L58AC:
    /* 58AC  cmp     cx,ax */
    sub16(CX, AX, 0);
L58AE:
    /* 58AE  jl      short L58B6 */
    if (SF != OF) goto L58B6;
L58B0:
    /* 58B0  mov     al,2 */
    AL = 0x2;
L58B2:
    /* 58B2  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L58B3: /* L58B3 */
    /* 58B3  mov     al,6 */
    AL = 0x6;
L58B5:
    /* 58B5  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L58B6: /* L58B6 */
    /* 58B6  mov     al,0Ah */
    AL = 0xA;
L58B8:
    /* 58B8  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L58B9: /* L58B9 */
    /* 58B9  mov     al,4 */
    AL = 0x4;
L58BB:
    /* 58BB  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L58BC: /* L58BC */
    /* 58BC  mov     al,8 */
    AL = 0x8;
L58BE:
    /* 58BE  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L58BF: /* L58BF */
    /* 58BF  cmp     bx,ax */
    sub16(BX, AX, 0);
L58C1:
    /* 58C1  jl      short L58CA */
    if (SF != OF) goto L58CA;
L58C3:
    /* 58C3  cmp     cx,bp */
    sub16(CX, BP, 0);
L58C5:
    /* 58C5  jg      short L58F0 */
    if (!ZF && SF == OF) goto L58F0;
L58C7:
    /* 58C7  mov     al,8Ah */
    AL = 0x8A;
L58C9:
    /* 58C9  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L58CA: /* L58CA */
    /* 58CA  cmp     bx,bp */
    sub16(BX, BP, 0);
L58CC:
    /* 58CC  jg      short L58D5 */
    if (!ZF && SF == OF) goto L58D5;
L58CE:
    /* 58CE  cmp     cx,bp */
    sub16(CX, BP, 0);
L58D0:
    /* 58D0  jg      short L58E6 */
    if (!ZF && SF == OF) goto L58E6;
L58D2:
    /* 58D2  mov     al,89h */
    AL = 0x89;
L58D4:
    /* 58D4  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L58D5: /* L58D5 */
    /* 58D5  cmp     cx,bp */
    sub16(CX, BP, 0);
L58D7:
    /* 58D7  jg      short L58DC */
    if (!ZF && SF == OF) goto L58DC;
L58D9:
    /* 58D9  mov     al,8Bh */
    AL = 0x8B;
L58DB:
    /* 58DB  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L58DC: /* L58DC */
    /* 58DC  cmp     cx,ax */
    sub16(CX, AX, 0);
L58DE:
    /* 58DE  jl      short L58E3 */
    if (SF != OF) goto L58E3;
L58E0:
    /* 58E0  mov     al,87h */
    AL = 0x87;
L58E2:
    /* 58E2  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L58E3: /* L58E3 */
    /* 58E3  mov     al,8Fh */
    AL = 0x8F;
L58E5:
    /* 58E5  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L58E6: /* L58E6 */
    /* 58E6  cmp     cx,ax */
    sub16(CX, AX, 0);
L58E8:
    /* 58E8  jl      short L58ED */
    if (SF != OF) goto L58ED;
L58EA:
    /* 58EA  mov     al,85h */
    AL = 0x85;
L58EC:
    /* 58EC  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L58ED: /* L58ED */
    /* 58ED  mov     al,8Dh */
    AL = 0x8D;
L58EF:
    /* 58EF  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L58F0: /* L58F0 */
    /* 58F0  cmp     cx,ax */
    sub16(CX, AX, 0);
L58F2:
    /* 58F2  jl      short L58F7 */
    if (SF != OF) goto L58F7;
L58F4:
    /* 58F4  mov     al,86h */
    AL = 0x86;
L58F6:
    /* 58F6  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L58F7: /* L58F7 */
    /* 58F7  mov     al,8Eh */
    AL = 0x8E;
L58F9:
    /* 58F9  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_58FA  (+58FA)
       mm9x9: the view matrix (written through ES) = the 3 x 3 matrix at BP times the one at SI, row
       by row through mm3x9_bpsi. BP is advanced by 18 bytes. */
L58FA: /* _mm9x9 */
    /* 58FA  call    _mm3x9_bpsi */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x59DC), 0x58FD)) != 0) return c;
L58FD:
    /* 58FD  mov     word ptr es:[MAT_XX],bx */
    ww(pES, 0x1602, BX);
L5902:
    /* 5902  mov     word ptr es:[MAT_XY],cx */
    ww(pES, 0x1604, CX);
L5907:
    /* 5907  mov     word ptr es:[MAT_XZ],di */
    ww(pES, 0x1606, DI);
L590C:
    /* 590C  add     bp,6 */
    BP = (uint16_t)(BP + 0x6);
L590F:
    /* 590F  call    _mm3x9_bpsi */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x59DC), 0x5912)) != 0) return c;
L5912:
    /* 5912  mov     word ptr es:[MAT_YX],bx */
    ww(pES, 0x1608, BX);
L5917:
    /* 5917  mov     word ptr es:[MAT_YY],cx */
    ww(pES, 0x160A, CX);
L591C:
    /* 591C  mov     word ptr es:[MAT_YZ],di */
    ww(pES, 0x160C, DI);
L5921:
    /* 5921  add     bp,6 */
    BP = (uint16_t)(BP + 0x6);
L5924:
    /* 5924  call    _mm3x9_bpsi */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x59DC), 0x5927)) != 0) return c;
L5927:
    /* 5927  mov     word ptr es:[MAT_ZX],bx */
    ww(pES, 0x160E, BX);
L592C:
    /* 592C  mov     word ptr es:[MAT_ZY],cx */
    ww(pES, 0x1610, CX);
L5931:
    /* 5931  mov     word ptr es:[MAT_ZZ],di */
    ww(pES, 0x1612, DI);
L5936:
    /* 5936  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_5937  (+5937)
       UW1 only: as mm3x9_bpsi with SI and BP the other way round: the row at SI times the columns
       of the matrix at BP (words +0, +6, +0Ch ...), saturated the same way. */
L5937: /* _seg004_5937 */
    /* 5937  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L5939:
    /* 5939  imul    word ptr [bp] */
    imul16(rw(pSS, BP));
L593C:
    /* 593C  mov     bx,ax */
    BX = AX;
L593E:
    /* 593E  mov     cx,dx */
    CX = DX;
L5940:
    /* 5940  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L5943:
    /* 5943  imul    word ptr [bp+6] */
    imul16(rw(pSS, BP + 0x6));
L5946:
    /* 5946  add     bx,ax */
    BX = add16(BX, AX, 0);
L5948:
    /* 5948  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L594A:
    /* 594A  mov     ax,word ptr [si+4] */
    AX = rw(pDS, SI + 0x4);
L594D:
    /* 594D  imul    word ptr [bp+0Ch] */
    imul16(rw(pSS, BP + 0xC));
L5950:
    /* 5950  add     bx,ax */
    BX = add16(BX, AX, 0);
L5952:
    /* 5952  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5954:
    /* 5954  shl     bx,1 */
    BX = shl16(BX, 1);
L5956:
    /* 5956  rcl     cx,1 */
    CX = rcl16(CX, 1);
L5958:
    /* 5958  jno     short L5964 */
    if (!OF) goto L5964;
L595A:
    /* 595A  mov     ax,cx */
    AX = CX;
L595C:
    /* 595C  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L595D:
    /* 595D  mov     cx,8001h */
    CX = 0x8001;
L5960:
    /* 5960  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L5962:
    /* 5962  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L5964: /* L5964 */
    /* 5964  cmp     cx,8000h */
    sub16(CX, 0x8000, 0);
L5968:
    /* 5968  jne     short L596D */
    if (!ZF) goto L596D;
L596A:
    /* 596A  mov     cx,8001h */
    CX = 0x8001;
L596D: /* L596D */
    /* 596D  push    cx */
    push16(CX);
L596E:
    /* 596E  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L5970:
    /* 5970  imul    word ptr [bp+2] */
    imul16(rw(pSS, BP + 0x2));
L5973:
    /* 5973  mov     bx,ax */
    BX = AX;
L5975:
    /* 5975  mov     cx,dx */
    CX = DX;
L5977:
    /* 5977  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L597A:
    /* 597A  imul    word ptr [bp+8] */
    imul16(rw(pSS, BP + 0x8));
L597D:
    /* 597D  add     bx,ax */
    BX = add16(BX, AX, 0);
L597F:
    /* 597F  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5981:
    /* 5981  mov     ax,word ptr [si+4] */
    AX = rw(pDS, SI + 0x4);
L5984:
    /* 5984  imul    word ptr [bp+0Eh] */
    imul16(rw(pSS, BP + 0xE));
L5987:
    /* 5987  add     bx,ax */
    BX = add16(BX, AX, 0);
L5989:
    /* 5989  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L598B:
    /* 598B  shl     bx,1 */
    BX = shl16(BX, 1);
L598D:
    /* 598D  rcl     cx,1 */
    CX = rcl16(CX, 1);
L598F:
    /* 598F  jno     short L599B */
    if (!OF) goto L599B;
L5991:
    /* 5991  mov     ax,cx */
    AX = CX;
L5993:
    /* 5993  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L5994:
    /* 5994  mov     cx,8001h */
    CX = 0x8001;
L5997:
    /* 5997  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L5999:
    /* 5999  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L599B: /* L599B */
    /* 599B  cmp     cx,8000h */
    sub16(CX, 0x8000, 0);
L599F:
    /* 599F  jne     short L59A4 */
    if (!ZF) goto L59A4;
L59A1:
    /* 59A1  mov     cx,8001h */
    CX = 0x8001;
L59A4: /* L59A4 */
    /* 59A4  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L59A6:
    /* 59A6  imul    word ptr [bp+4] */
    imul16(rw(pSS, BP + 0x4));
L59A9:
    /* 59A9  mov     bx,ax */
    BX = AX;
L59AB:
    /* 59AB  mov     di,dx */
    DI = DX;
L59AD:
    /* 59AD  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L59B0:
    /* 59B0  imul    word ptr [bp+0Ah] */
    imul16(rw(pSS, BP + 0xA));
L59B3:
    /* 59B3  add     bx,ax */
    BX = add16(BX, AX, 0);
L59B5:
    /* 59B5  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L59B7:
    /* 59B7  mov     ax,word ptr [si+4] */
    AX = rw(pDS, SI + 0x4);
L59BA:
    /* 59BA  imul    word ptr [bp+10h] */
    imul16(rw(pSS, BP + 0x10));
L59BD:
    /* 59BD  add     bx,ax */
    BX = add16(BX, AX, 0);
L59BF:
    /* 59BF  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L59C1:
    /* 59C1  shl     bx,1 */
    BX = shl16(BX, 1);
L59C3:
    /* 59C3  rcl     di,1 */
    DI = rcl16(DI, 1);
L59C5:
    /* 59C5  jno     short L59D1 */
    if (!OF) goto L59D1;
L59C7:
    /* 59C7  mov     ax,cx */
    AX = CX;
L59C9:
    /* 59C9  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L59CA:
    /* 59CA  mov     cx,8001h */
    CX = 0x8001;
L59CD:
    /* 59CD  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L59CF:
    /* 59CF  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L59D1: /* L59D1 */
    /* 59D1  cmp     cx,8000h */
    sub16(CX, 0x8000, 0);
L59D5:
    /* 59D5  jne     short L59DA */
    if (!ZF) goto L59DA;
L59D7:
    /* 59D7  mov     cx,8001h */
    CX = 0x8001;
L59DA: /* L59DA */
    /* 59DA  pop     bx */
    BX = pop16();
L59DB:
    /* 59DB  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_59DC  (+59DC)
       mm3x9_bpsi: a row times a matrix: BP = a row of three 1.15 words, SI = a 3 x 3 matrix by rows.
       Out: BX, CX, DI = the row's three products, saturated to +-8001h on overflow (8000h becomes
       8001h, so negation is safe). The overflow fix-up of the third product reads and writes CX, not
       DI, so an overflow there overwrites the second result and leaves the third wrapped; FM Towns'
       mm3x9_bpsi saturates DI. Probably a slip in the DOS source. */
L59DC: /* _mm3x9_bpsi */
    /* 59DC  mov     ax,word ptr [bp] */
    AX = rw(pSS, BP);
L59DF:
    /* 59DF  imul    word ptr [si] */
    imul16(rw(pDS, SI));
L59E1:
    /* 59E1  mov     bx,ax */
    BX = AX;
L59E3:
    /* 59E3  mov     cx,dx */
    CX = DX;
L59E5:
    /* 59E5  mov     ax,word ptr [bp+2] */
    AX = rw(pSS, BP + 0x2);
L59E8:
    /* 59E8  imul    word ptr [si+6] */
    imul16(rw(pDS, SI + 0x6));
L59EB:
    /* 59EB  add     bx,ax */
    BX = add16(BX, AX, 0);
L59ED:
    /* 59ED  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L59EF:
    /* 59EF  mov     ax,word ptr [bp+4] */
    AX = rw(pSS, BP + 0x4);
L59F2:
    /* 59F2  imul    word ptr [si+0Ch] */
    imul16(rw(pDS, SI + 0xC));
L59F5:
    /* 59F5  add     bx,ax */
    BX = add16(BX, AX, 0);
L59F7:
    /* 59F7  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L59F9:
    /* 59F9  shl     bx,1 */
    BX = shl16(BX, 1);
L59FB:
    /* 59FB  rcl     cx,1 */
    CX = rcl16(CX, 1);
L59FD:
    /* 59FD  jno     short L5A09 */
    if (!OF) goto L5A09;
L59FF:
    /* 59FF  mov     ax,cx */
    AX = CX;
L5A01:
    /* 5A01  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L5A02:
    /* 5A02  mov     cx,8001h */
    CX = 0x8001;
L5A05:
    /* 5A05  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L5A07:
    /* 5A07  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L5A09: /* L5A09 */
    /* 5A09  cmp     cx,8000h */
    sub16(CX, 0x8000, 0);
L5A0D:
    /* 5A0D  jne     short L5A12 */
    if (!ZF) goto L5A12;
L5A0F:
    /* 5A0F  mov     cx,8001h */
    CX = 0x8001;
L5A12: /* L5A12 */
    /* 5A12  push    cx */
    push16(CX);
L5A13:
    /* 5A13  mov     ax,word ptr [bp] */
    AX = rw(pSS, BP);
L5A16:
    /* 5A16  imul    word ptr [si+2] */
    imul16(rw(pDS, SI + 0x2));
L5A19:
    /* 5A19  mov     bx,ax */
    BX = AX;
L5A1B:
    /* 5A1B  mov     cx,dx */
    CX = DX;
L5A1D:
    /* 5A1D  mov     ax,word ptr [bp+2] */
    AX = rw(pSS, BP + 0x2);
L5A20:
    /* 5A20  imul    word ptr [si+8] */
    imul16(rw(pDS, SI + 0x8));
L5A23:
    /* 5A23  add     bx,ax */
    BX = add16(BX, AX, 0);
L5A25:
    /* 5A25  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5A27:
    /* 5A27  mov     ax,word ptr [bp+4] */
    AX = rw(pSS, BP + 0x4);
L5A2A:
    /* 5A2A  imul    word ptr [si+0Eh] */
    imul16(rw(pDS, SI + 0xE));
L5A2D:
    /* 5A2D  add     bx,ax */
    BX = add16(BX, AX, 0);
L5A2F:
    /* 5A2F  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5A31:
    /* 5A31  shl     bx,1 */
    BX = shl16(BX, 1);
L5A33:
    /* 5A33  rcl     cx,1 */
    CX = rcl16(CX, 1);
L5A35:
    /* 5A35  jno     short L5A41 */
    if (!OF) goto L5A41;
L5A37:
    /* 5A37  mov     ax,cx */
    AX = CX;
L5A39:
    /* 5A39  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L5A3A:
    /* 5A3A  mov     cx,8001h */
    CX = 0x8001;
L5A3D:
    /* 5A3D  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L5A3F:
    /* 5A3F  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L5A41: /* L5A41 */
    /* 5A41  cmp     cx,8000h */
    sub16(CX, 0x8000, 0);
L5A45:
    /* 5A45  jne     short L5A4A */
    if (!ZF) goto L5A4A;
L5A47:
    /* 5A47  mov     cx,8001h */
    CX = 0x8001;
L5A4A: /* L5A4A */
    /* 5A4A  mov     ax,word ptr [bp] */
    AX = rw(pSS, BP);
L5A4D:
    /* 5A4D  imul    word ptr [si+4] */
    imul16(rw(pDS, SI + 0x4));
L5A50:
    /* 5A50  mov     bx,ax */
    BX = AX;
L5A52:
    /* 5A52  mov     di,dx */
    DI = DX;
L5A54:
    /* 5A54  mov     ax,word ptr [bp+2] */
    AX = rw(pSS, BP + 0x2);
L5A57:
    /* 5A57  imul    word ptr [si+0Ah] */
    imul16(rw(pDS, SI + 0xA));
L5A5A:
    /* 5A5A  add     bx,ax */
    BX = add16(BX, AX, 0);
L5A5C:
    /* 5A5C  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L5A5E:
    /* 5A5E  mov     ax,word ptr [bp+4] */
    AX = rw(pSS, BP + 0x4);
L5A61:
    /* 5A61  imul    word ptr [si+10h] */
    imul16(rw(pDS, SI + 0x10));
L5A64:
    /* 5A64  add     bx,ax */
    BX = add16(BX, AX, 0);
L5A66:
    /* 5A66  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L5A68:
    /* 5A68  shl     bx,1 */
    BX = shl16(BX, 1);
L5A6A:
    /* 5A6A  rcl     di,1 */
    DI = rcl16(DI, 1);
L5A6C:
    /* 5A6C  jno     short L5A78 */
    if (!OF) goto L5A78;
L5A6E:
    /* 5A6E  mov     ax,cx */
    AX = CX;
L5A70:
    /* 5A70  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L5A71:
    /* 5A71  mov     cx,8001h */
    CX = 0x8001;
L5A74:
    /* 5A74  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L5A76:
    /* 5A76  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L5A78: /* L5A78 */
    /* 5A78  cmp     di,8000h */
    sub16(DI, 0x8000, 0);
L5A7C:
    /* 5A7C  jne     short L5A81 */
    if (!ZF) goto L5A81;
L5A7E:
    /* 5A7E  mov     di,8001h */
    DI = 0x8001;
L5A81: /* L5A81 */
    /* 5A81  pop     bx */
    BX = pop16();
L5A82:
    /* 5A82  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_5A83  (+5A83)
       UW1 only: as mm3x9t, but the three products take the column at SI (words +0, +6, +0Ch)
       against the columns of BP's matrix; with the same slip in the third product's overflow
       fix-up. Called three times by seg004_5834. */
L5A83: /* _seg004_5A83 */
    /* 5A83  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L5A85:
    /* 5A85  imul    word ptr [bp] */
    imul16(rw(pSS, BP));
L5A88:
    /* 5A88  mov     bx,ax */
    BX = AX;
L5A8A:
    /* 5A8A  mov     cx,dx */
    CX = DX;
L5A8C:
    /* 5A8C  mov     ax,word ptr [si+6] */
    AX = rw(pDS, SI + 0x6);
L5A8F:
    /* 5A8F  imul    word ptr [bp+6] */
    imul16(rw(pSS, BP + 0x6));
L5A92:
    /* 5A92  add     bx,ax */
    BX = add16(BX, AX, 0);
L5A94:
    /* 5A94  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5A96:
    /* 5A96  mov     ax,word ptr [si+0Ch] */
    AX = rw(pDS, SI + 0xC);
L5A99:
    /* 5A99  imul    word ptr [bp+0Ch] */
    imul16(rw(pSS, BP + 0xC));
L5A9C:
    /* 5A9C  add     bx,ax */
    BX = add16(BX, AX, 0);
L5A9E:
    /* 5A9E  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5AA0:
    /* 5AA0  shl     bx,1 */
    BX = shl16(BX, 1);
L5AA2:
    /* 5AA2  rcl     cx,1 */
    CX = rcl16(CX, 1);
L5AA4:
    /* 5AA4  jno     short L5AB0 */
    if (!OF) goto L5AB0;
L5AA6:
    /* 5AA6  mov     ax,cx */
    AX = CX;
L5AA8:
    /* 5AA8  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L5AA9:
    /* 5AA9  mov     cx,8001h */
    CX = 0x8001;
L5AAC:
    /* 5AAC  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L5AAE:
    /* 5AAE  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L5AB0: /* L5AB0 */
    /* 5AB0  cmp     cx,8000h */
    sub16(CX, 0x8000, 0);
L5AB4:
    /* 5AB4  jne     short L5AB9 */
    if (!ZF) goto L5AB9;
L5AB6:
    /* 5AB6  mov     cx,8001h */
    CX = 0x8001;
L5AB9: /* L5AB9 */
    /* 5AB9  push    cx */
    push16(CX);
L5ABA:
    /* 5ABA  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L5ABC:
    /* 5ABC  imul    word ptr [bp+2] */
    imul16(rw(pSS, BP + 0x2));
L5ABF:
    /* 5ABF  mov     bx,ax */
    BX = AX;
L5AC1:
    /* 5AC1  mov     cx,dx */
    CX = DX;
L5AC3:
    /* 5AC3  mov     ax,word ptr [si+6] */
    AX = rw(pDS, SI + 0x6);
L5AC6:
    /* 5AC6  imul    word ptr [bp+8] */
    imul16(rw(pSS, BP + 0x8));
L5AC9:
    /* 5AC9  add     bx,ax */
    BX = add16(BX, AX, 0);
L5ACB:
    /* 5ACB  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5ACD:
    /* 5ACD  mov     ax,word ptr [si+0Ch] */
    AX = rw(pDS, SI + 0xC);
L5AD0:
    /* 5AD0  imul    word ptr [bp+0Eh] */
    imul16(rw(pSS, BP + 0xE));
L5AD3:
    /* 5AD3  add     bx,ax */
    BX = add16(BX, AX, 0);
L5AD5:
    /* 5AD5  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5AD7:
    /* 5AD7  shl     bx,1 */
    BX = shl16(BX, 1);
L5AD9:
    /* 5AD9  rcl     cx,1 */
    CX = rcl16(CX, 1);
L5ADB:
    /* 5ADB  jno     short L5AE7 */
    if (!OF) goto L5AE7;
L5ADD:
    /* 5ADD  mov     ax,cx */
    AX = CX;
L5ADF:
    /* 5ADF  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L5AE0:
    /* 5AE0  mov     cx,8001h */
    CX = 0x8001;
L5AE3:
    /* 5AE3  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L5AE5:
    /* 5AE5  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L5AE7: /* L5AE7 */
    /* 5AE7  cmp     cx,8000h */
    sub16(CX, 0x8000, 0);
L5AEB:
    /* 5AEB  jne     short L5AF0 */
    if (!ZF) goto L5AF0;
L5AED:
    /* 5AED  mov     cx,8001h */
    CX = 0x8001;
L5AF0: /* L5AF0 */
    /* 5AF0  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L5AF2:
    /* 5AF2  imul    word ptr [bp+4] */
    imul16(rw(pSS, BP + 0x4));
L5AF5:
    /* 5AF5  mov     bx,ax */
    BX = AX;
L5AF7:
    /* 5AF7  mov     di,dx */
    DI = DX;
L5AF9:
    /* 5AF9  mov     ax,word ptr [si+6] */
    AX = rw(pDS, SI + 0x6);
L5AFC:
    /* 5AFC  imul    word ptr [bp+0Ah] */
    imul16(rw(pSS, BP + 0xA));
L5AFF:
    /* 5AFF  add     bx,ax */
    BX = add16(BX, AX, 0);
L5B01:
    /* 5B01  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L5B03:
    /* 5B03  mov     ax,word ptr [si+0Ch] */
    AX = rw(pDS, SI + 0xC);
L5B06:
    /* 5B06  imul    word ptr [bp+10h] */
    imul16(rw(pSS, BP + 0x10));
L5B09:
    /* 5B09  add     bx,ax */
    BX = add16(BX, AX, 0);
L5B0B:
    /* 5B0B  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L5B0D:
    /* 5B0D  shl     bx,1 */
    BX = shl16(BX, 1);
L5B0F:
    /* 5B0F  rcl     di,1 */
    DI = rcl16(DI, 1);
L5B11:
    /* 5B11  jno     short L5B1D */
    if (!OF) goto L5B1D;
L5B13:
    /* 5B13  mov     ax,cx */
    AX = CX;
L5B15:
    /* 5B15  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L5B16:
    /* 5B16  mov     cx,8001h */
    CX = 0x8001;
L5B19:
    /* 5B19  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L5B1B:
    /* 5B1B  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L5B1D: /* L5B1D */
    /* 5B1D  cmp     di,8000h */
    sub16(DI, 0x8000, 0);
L5B21:
    /* 5B21  jne     short L5B26 */
    if (!ZF) goto L5B26;
L5B23:
    /* 5B23  mov     di,8001h */
    DI = 0x8001;
L5B26: /* L5B26 */
    /* 5B26  pop     bx */
    BX = pop16();
L5B27:
    /* 5B27  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_5B28  (+5B28)
       mm3x9t: as mm3x9_bpsi with the matrix at BP taken transposed (a row at SI times the columns of
       BP's matrix), with the same slip in the third product's overflow fix-up. Called by head_view
       (CAMERA.ASM). */
L5B28: /* _mm3x9t */
    /* 5B28  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L5B2A:
    /* 5B2A  imul    word ptr [bp] */
    imul16(rw(pSS, BP));
L5B2D:
    /* 5B2D  mov     bx,ax */
    BX = AX;
L5B2F:
    /* 5B2F  mov     cx,dx */
    CX = DX;
L5B31:
    /* 5B31  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L5B34:
    /* 5B34  imul    word ptr [bp+2] */
    imul16(rw(pSS, BP + 0x2));
L5B37:
    /* 5B37  add     bx,ax */
    BX = add16(BX, AX, 0);
L5B39:
    /* 5B39  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5B3B:
    /* 5B3B  mov     ax,word ptr [si+4] */
    AX = rw(pDS, SI + 0x4);
L5B3E:
    /* 5B3E  imul    word ptr [bp+4] */
    imul16(rw(pSS, BP + 0x4));
L5B41:
    /* 5B41  add     bx,ax */
    BX = add16(BX, AX, 0);
L5B43:
    /* 5B43  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5B45:
    /* 5B45  shl     bx,1 */
    BX = shl16(BX, 1);
L5B47:
    /* 5B47  rcl     cx,1 */
    CX = rcl16(CX, 1);
L5B49:
    /* 5B49  jno     short L5B55 */
    if (!OF) goto L5B55;
L5B4B:
    /* 5B4B  mov     ax,cx */
    AX = CX;
L5B4D:
    /* 5B4D  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L5B4E:
    /* 5B4E  mov     cx,8001h */
    CX = 0x8001;
L5B51:
    /* 5B51  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L5B53:
    /* 5B53  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L5B55: /* L5B55 */
    /* 5B55  cmp     cx,8000h */
    sub16(CX, 0x8000, 0);
L5B59:
    /* 5B59  jne     short L5B5E */
    if (!ZF) goto L5B5E;
L5B5B:
    /* 5B5B  mov     cx,8001h */
    CX = 0x8001;
L5B5E: /* L5B5E */
    /* 5B5E  push    cx */
    push16(CX);
L5B5F:
    /* 5B5F  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L5B61:
    /* 5B61  imul    word ptr [bp+6] */
    imul16(rw(pSS, BP + 0x6));
L5B64:
    /* 5B64  mov     bx,ax */
    BX = AX;
L5B66:
    /* 5B66  mov     cx,dx */
    CX = DX;
L5B68:
    /* 5B68  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L5B6B:
    /* 5B6B  imul    word ptr [bp+8] */
    imul16(rw(pSS, BP + 0x8));
L5B6E:
    /* 5B6E  add     bx,ax */
    BX = add16(BX, AX, 0);
L5B70:
    /* 5B70  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5B72:
    /* 5B72  mov     ax,word ptr [si+4] */
    AX = rw(pDS, SI + 0x4);
L5B75:
    /* 5B75  imul    word ptr [bp+0Ah] */
    imul16(rw(pSS, BP + 0xA));
L5B78:
    /* 5B78  add     bx,ax */
    BX = add16(BX, AX, 0);
L5B7A:
    /* 5B7A  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L5B7C:
    /* 5B7C  shl     bx,1 */
    BX = shl16(BX, 1);
L5B7E:
    /* 5B7E  rcl     cx,1 */
    CX = rcl16(CX, 1);
L5B80:
    /* 5B80  jno     short L5B8C */
    if (!OF) goto L5B8C;
L5B82:
    /* 5B82  mov     ax,cx */
    AX = CX;
L5B84:
    /* 5B84  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L5B85:
    /* 5B85  mov     cx,8001h */
    CX = 0x8001;
L5B88:
    /* 5B88  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L5B8A:
    /* 5B8A  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L5B8C: /* L5B8C */
    /* 5B8C  cmp     cx,8000h */
    sub16(CX, 0x8000, 0);
L5B90:
    /* 5B90  jne     short L5B95 */
    if (!ZF) goto L5B95;
L5B92:
    /* 5B92  mov     cx,8001h */
    CX = 0x8001;
L5B95: /* L5B95 */
    /* 5B95  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L5B97:
    /* 5B97  imul    word ptr [bp+0Ch] */
    imul16(rw(pSS, BP + 0xC));
L5B9A:
    /* 5B9A  mov     bx,ax */
    BX = AX;
L5B9C:
    /* 5B9C  mov     di,dx */
    DI = DX;
L5B9E:
    /* 5B9E  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L5BA1:
    /* 5BA1  imul    word ptr [bp+0Eh] */
    imul16(rw(pSS, BP + 0xE));
L5BA4:
    /* 5BA4  add     bx,ax */
    BX = add16(BX, AX, 0);
L5BA6:
    /* 5BA6  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L5BA8:
    /* 5BA8  mov     ax,word ptr [si+4] */
    AX = rw(pDS, SI + 0x4);
L5BAB:
    /* 5BAB  imul    word ptr [bp+10h] */
    imul16(rw(pSS, BP + 0x10));
L5BAE:
    /* 5BAE  add     bx,ax */
    BX = add16(BX, AX, 0);
L5BB0:
    /* 5BB0  adc     di,dx */
    DI = (uint16_t)(DI + DX + CF);
L5BB2:
    /* 5BB2  shl     bx,1 */
    BX = shl16(BX, 1);
L5BB4:
    /* 5BB4  rcl     di,1 */
    DI = rcl16(DI, 1);
L5BB6:
    /* 5BB6  jno     short L5BC2 */
    if (!OF) goto L5BC2;
L5BB8:
    /* 5BB8  mov     ax,cx */
    AX = CX;
L5BBA:
    /* 5BBA  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L5BBB:
    /* 5BBB  mov     cx,8001h */
    CX = 0x8001;
L5BBE:
    /* 5BBE  xor     cx,dx */
    CX = (uint16_t)(CX ^ DX);
L5BC0:
    /* 5BC0  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L5BC2: /* L5BC2 */
    /* 5BC2  cmp     di,8000h */
    sub16(DI, 0x8000, 0);
L5BC6:
    /* 5BC6  jne     short L5BCB */
    if (!ZF) goto L5BCB;
L5BC8:
    /* 5BC8  mov     di,8001h */
    DI = 0x8001;
L5BCB: /* L5BCB */
    /* 5BCB  pop     bx */
    BX = pop16();
L5BCC:
    /* 5BCC  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_5BD1  (+5BD1)
       overflow_handler_reg: the int 0 handler for a 2-byte `idiv reg`: skips the instruction and
       returns AX = +-7FFFh by the sign of DX (DX the sign extension), and counts the overflow
       (do_scalebm abandons a sprite that overflowed). */
L5BD1: /* _overflow_handler_reg */
    /* 5BD1  mov     word ptr cs:L5BCD,bp */
    ww(CODE004, 0x5BCD, BP);
L5BD6:
    /* 5BD6  mov     bp,sp */
    BP = SP;
L5BD8:
    /* 5BD8  inc     word ptr [bp] */
    ww(pSS, BP, (uint16_t)(rw(pSS, BP) + 1));
L5BDB:
    /* 5BDB  inc     word ptr [bp] */
    ww(pSS, BP, (uint16_t)(rw(pSS, BP) + 1));
L5BDE:
    /* 5BDE  mov     ax,7FFFh */
    AX = 0x7FFF;
L5BE1:
    /* 5BE1  xor     dx,dx */
    DX = logic16((uint16_t)(DX ^ DX));
L5BE3:
    /* 5BE3  mov     bp,word ptr cs:L5BCD */
    BP = rw(CODE004, 0x5BCD);
L5BE8:
    /* 5BE8  inc     word ptr cs:overflow_count */
    ww(CODE004, 0x5BCF, inc16(rw(CODE004, 0x5BCF)));
L5BED:
    /* 5BED  iret */
    SP = (uint16_t)(SP + 4); asm_set_flags(pop16()); return ASM_IRET;

    /* seg004_5BEE  (+5BEE)
       overflow_handler_mem: for a 4-byte divide by a memory operand: skips it, AX = 7FFFh, DX = 0. */
L5BEE: /* _overflow_handler_mem */
    /* 5BEE  mov     word ptr cs:L5BCD,bp */
    ww(CODE004, 0x5BCD, BP);
L5BF3:
    /* 5BF3  mov     bp,sp */
    BP = SP;
L5BF5:
    /* 5BF5  add     word ptr [bp],4 */
    ww(pSS, BP, (uint16_t)(rw(pSS, BP) + 0x4));
L5BF9:
    /* 5BF9  mov     ax,7FFFh */
    AX = 0x7FFF;
L5BFC:
    /* 5BFC  xor     dx,dx */
    DX = logic16((uint16_t)(DX ^ DX));
L5BFE:
    /* 5BFE  mov     bp,word ptr cs:L5BCD */
    BP = rw(CODE004, 0x5BCD);
L5C03:
    /* 5C03  iret */
    SP = (uint16_t)(SP + 4); asm_set_flags(pop16()); return ASM_IRET;

    /* seg004_5C04  (+5C04)
       overflow_handler_special_bp: for a 2-byte divide by BP: skips it and returns +-7FFFh by the
       sign of DX xor BP (the quotient's sign), DX = 0. */
L5C04: /* _overflow_handler_special_bp */
    /* 5C04  mov     word ptr cs:L5BCD,bp */
    ww(CODE004, 0x5BCD, BP);
L5C09:
    /* 5C09  mov     bp,sp */
    BP = SP;
L5C0B:
    /* 5C0B  inc     word ptr [bp] */
    ww(pSS, BP, (uint16_t)(rw(pSS, BP) + 1));
L5C0E:
    /* 5C0E  inc     word ptr [bp] */
    ww(pSS, BP, (uint16_t)(rw(pSS, BP) + 1));
L5C11:
    /* 5C11  mov     bp,word ptr cs:L5BCD */
    BP = rw(CODE004, 0x5BCD);
L5C16:
    /* 5C16  mov     ax,dx */
    AX = DX;
L5C18:
    /* 5C18  xor     ax,bp */
    AX = (uint16_t)(AX ^ BP);
L5C1A:
    /* 5C1A  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L5C1B:
    /* 5C1B  mov     ax,7FFFh */
    AX = 0x7FFF;
L5C1E:
    /* 5C1E  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L5C20:
    /* 5C20  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L5C22:
    /* 5C22  xor     dx,dx */
    DX = logic16((uint16_t)(DX ^ DX));
L5C24:
    /* 5C24  iret */
    SP = (uint16_t)(SP + 4); asm_set_flags(pop16()); return ASM_IRET;
}
