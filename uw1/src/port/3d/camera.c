/* camera.c: replaces src/3d/CAMERA.ASM (seg004_A10, 0A10..1366 of its
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

uint32_t asm_mod_CAMERA(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x0A10: goto L0A10;
    case 0x0A11: goto L0A11;
    case 0x0A14: goto L0A14;
    case 0x0A16: goto L0A16;
    case 0x0A19: goto L0A19;
    case 0x0A1C: goto L0A1C;
    case 0x0A1D: goto L0A1D;
    case 0x0A1E: goto L0A1E;
    case 0x0A1F: goto L0A1F;
    case 0x0A22: goto L0A22;
    case 0x0A24: goto L0A24;
    case 0x0A27: goto L0A27;
    case 0x0A2A: goto L0A2A;
    case 0x0A30: goto L0A30;
    case 0x0A31: goto L0A31;
    case 0x0A32: goto L0A32;
    case 0x0A33: goto L0A33;
    case 0x0A36: goto L0A36;
    case 0x0A38: goto L0A38;
    case 0x0A3B: goto L0A3B;
    case 0x0A3D: goto L0A3D;
    case 0x0A3F: goto L0A3F;
    case 0x0A45: goto L0A45;
    case 0x0A4B: goto L0A4B;
    case 0x0A52: goto L0A52;
    case 0x0A54: goto L0A54;
    case 0x0A57: goto L0A57;
    case 0x0A59: goto L0A59;
    case 0x0A5C: goto L0A5C;
    case 0x0A5F: goto L0A5F;
    case 0x0A61: goto L0A61;
    case 0x0A66: goto L0A66;
    case 0x0A68: goto L0A68;
    case 0x0A6F: goto L0A6F;
    case 0x0A70: goto L0A70;
    case 0x0A71: goto L0A71;
    case 0x0A74: goto L0A74;
    case 0x0A77: goto L0A77;
    case 0x0A7B: goto L0A7B;
    case 0x0A7C: goto L0A7C;
    case 0x0A7E: goto L0A7E;
    case 0x0A82: goto L0A82;
    case 0x0A84: goto L0A84;
    case 0x0A87: goto L0A87;
    case 0x0A89: goto L0A89;
    case 0x0A8B: goto L0A8B;
    case 0x0A8E: goto L0A8E;
    case 0x0A92: goto L0A92;
    case 0x0A94: goto L0A94;
    case 0x0A97: goto L0A97;
    case 0x0A9A: goto L0A9A;
    case 0x0A9E: goto L0A9E;
    case 0x0AA0: goto L0AA0;
    case 0x0AA2: goto L0AA2;
    case 0x0AA5: goto L0AA5;
    case 0x0AAA: goto L0AAA;
    case 0x0AAC: goto L0AAC;
    case 0x0AAE: goto L0AAE;
    case 0x0AB0: goto L0AB0;
    case 0x0AB2: goto L0AB2;
    case 0x0AB4: goto L0AB4;
    case 0x0AB6: goto L0AB6;
    case 0x0AB9: goto L0AB9;
    case 0x0ABC: goto L0ABC;
    case 0x0AC0: goto L0AC0;
    case 0x0AC2: goto L0AC2;
    case 0x0AC4: goto L0AC4;
    case 0x0AC7: goto L0AC7;
    case 0x0AC9: goto L0AC9;
    case 0x0ACC: goto L0ACC;
    case 0x0ACE: goto L0ACE;
    case 0x0AD0: goto L0AD0;
    case 0x0AD2: goto L0AD2;
    case 0x0AD4: goto L0AD4;
    case 0x0AD6: goto L0AD6;
    case 0x0AD9: goto L0AD9;
    case 0x0ADD: goto L0ADD;
    case 0x0ADF: goto L0ADF;
    case 0x0AE1: goto L0AE1;
    case 0x0AE3: goto L0AE3;
    case 0x0AE5: goto L0AE5;
    case 0x0AE9: goto L0AE9;
    case 0x0AEB: goto L0AEB;
    case 0x0AED: goto L0AED;
    case 0x0AEF: goto L0AEF;
    case 0x0AF3: goto L0AF3;
    case 0x0AF6: goto L0AF6;
    case 0x0AFA: goto L0AFA;
    case 0x0AFC: goto L0AFC;
    case 0x0AFE: goto L0AFE;
    case 0x0B00: goto L0B00;
    case 0x0B04: goto L0B04;
    case 0x0B06: goto L0B06;
    case 0x0B07: goto L0B07;
    case 0x0B0B: goto L0B0B;
    case 0x0B0E: goto L0B0E;
    case 0x0B12: goto L0B12;
    case 0x0B14: goto L0B14;
    case 0x0B16: goto L0B16;
    case 0x0B18: goto L0B18;
    case 0x0B1C: goto L0B1C;
    case 0x0B1E: goto L0B1E;
    case 0x0B1F: goto L0B1F;
    case 0x0B23: goto L0B23;
    case 0x0B26: goto L0B26;
    case 0x0B2A: goto L0B2A;
    case 0x0B2C: goto L0B2C;
    case 0x0B2E: goto L0B2E;
    case 0x0B30: goto L0B30;
    case 0x0B34: goto L0B34;
    case 0x0B36: goto L0B36;
    case 0x0B37: goto L0B37;
    case 0x0B3B: goto L0B3B;
    case 0x0B3E: goto L0B3E;
    case 0x0B40: goto L0B40;
    case 0x0B42: goto L0B42;
    case 0x0B44: goto L0B44;
    case 0x0B46: goto L0B46;
    case 0x0B48: goto L0B48;
    case 0x0B4B: goto L0B4B;
    case 0x0B4F: goto L0B4F;
    case 0x0B51: goto L0B51;
    case 0x0B53: goto L0B53;
    case 0x0B55: goto L0B55;
    case 0x0B57: goto L0B57;
    case 0x0B5B: goto L0B5B;
    case 0x0B5D: goto L0B5D;
    case 0x0B5F: goto L0B5F;
    case 0x0B61: goto L0B61;
    case 0x0B65: goto L0B65;
    case 0x0B68: goto L0B68;
    case 0x0B6A: goto L0B6A;
    case 0x0B6C: goto L0B6C;
    case 0x0B6E: goto L0B6E;
    case 0x0B70: goto L0B70;
    case 0x0B72: goto L0B72;
    case 0x0B75: goto L0B75;
    case 0x0B79: goto L0B79;
    case 0x0B7B: goto L0B7B;
    case 0x0B7D: goto L0B7D;
    case 0x0B7F: goto L0B7F;
    case 0x0B81: goto L0B81;
    case 0x0B85: goto L0B85;
    case 0x0B87: goto L0B87;
    case 0x0B89: goto L0B89;
    case 0x0B8B: goto L0B8B;
    case 0x0B8F: goto L0B8F;
    case 0x0B92: goto L0B92;
    case 0x0B94: goto L0B94;
    case 0x0B96: goto L0B96;
    case 0x0B98: goto L0B98;
    case 0x0B9A: goto L0B9A;
    case 0x0B9C: goto L0B9C;
    case 0x0B9F: goto L0B9F;
    case 0x0BA3: goto L0BA3;
    case 0x0BA5: goto L0BA5;
    case 0x0BA7: goto L0BA7;
    case 0x0BA9: goto L0BA9;
    case 0x0BAB: goto L0BAB;
    case 0x0BAF: goto L0BAF;
    case 0x0BB1: goto L0BB1;
    case 0x0BB3: goto L0BB3;
    case 0x0BB5: goto L0BB5;
    case 0x0BB9: goto L0BB9;
    case 0x0BBC: goto L0BBC;
    case 0x0BBF: goto L0BBF;
    case 0x0BC2: goto L0BC2;
    case 0x0BC5: goto L0BC5;
    case 0x0BC8: goto L0BC8;
    case 0x0BCB: goto L0BCB;
    case 0x0BCE: goto L0BCE;
    case 0x0BD1: goto L0BD1;
    case 0x0BD4: goto L0BD4;
    case 0x0BD7: goto L0BD7;
    case 0x0BDA: goto L0BDA;
    case 0x0BDD: goto L0BDD;
    case 0x0BE0: goto L0BE0;
    case 0x0BE3: goto L0BE3;
    case 0x0BE5: goto L0BE5;
    case 0x0BE8: goto L0BE8;
    case 0x0BE9: goto L0BE9;
    case 0x0BEC: goto L0BEC;
    case 0x0BED: goto L0BED;
    case 0x0BF0: goto L0BF0;
    case 0x0BF1: goto L0BF1;
    case 0x0BF4: goto L0BF4;
    case 0x0BF5: goto L0BF5;
    case 0x0BF6: goto L0BF6;
    case 0x0BF7: goto L0BF7;
    case 0x0BFA: goto L0BFA;
    case 0x0BFB: goto L0BFB;
    case 0x0BFD: goto L0BFD;
    case 0x0BFE: goto L0BFE;
    case 0x0C00: goto L0C00;
    case 0x0C02: goto L0C02;
    case 0x0C09: goto L0C09;
    case 0x0C0B: goto L0C0B;
    case 0x0C0D: goto L0C0D;
    case 0x0C10: goto L0C10;
    case 0x0C13: goto L0C13;
    case 0x0C18: goto L0C18;
    case 0x0C1A: goto L0C1A;
    case 0x0C1F: goto L0C1F;
    case 0x0C20: goto L0C20;
    case 0x0C21: goto L0C21;
    case 0x0C24: goto L0C24;
    case 0x0C26: goto L0C26;
    case 0x0C29: goto L0C29;
    case 0x0C2C: goto L0C2C;
    case 0x0C2D: goto L0C2D;
    case 0x0C2E: goto L0C2E;
    case 0x0C31: goto L0C31;
    case 0x0C33: goto L0C33;
    case 0x0C35: goto L0C35;
    case 0x0C38: goto L0C38;
    case 0x0C3B: goto L0C3B;
    case 0x0C40: goto L0C40;
    case 0x0C42: goto L0C42;
    case 0x0C43: goto L0C43;
    case 0x0C46: goto L0C46;
    case 0x0C47: goto L0C47;
    case 0x0C4A: goto L0C4A;
    case 0x0C4C: goto L0C4C;
    case 0x0C4D: goto L0C4D;
    case 0x0C50: goto L0C50;
    case 0x0C53: goto L0C53;
    case 0x0C56: goto L0C56;
    case 0x0C5A: goto L0C5A;
    case 0x0C5C: goto L0C5C;
    case 0x0C5D: goto L0C5D;
    case 0x0C60: goto L0C60;
    case 0x0C63: goto L0C63;
    case 0x0C66: goto L0C66;
    case 0x0C6A: goto L0C6A;
    case 0x0C6C: goto L0C6C;
    case 0x0C6D: goto L0C6D;
    case 0x0C70: goto L0C70;
    case 0x0C73: goto L0C73;
    case 0x0C76: goto L0C76;
    case 0x0C7A: goto L0C7A;
    case 0x0C7D: goto L0C7D;
    case 0x0C80: goto L0C80;
    case 0x0C81: goto L0C81;
    case 0x0C83: goto L0C83;
    case 0x0C86: goto L0C86;
    case 0x0C88: goto L0C88;
    case 0x0C8A: goto L0C8A;
    case 0x0C8C: goto L0C8C;
    case 0x0C8E: goto L0C8E;
    case 0x0C90: goto L0C90;
    case 0x0C93: goto L0C93;
    case 0x0C95: goto L0C95;
    case 0x0C97: goto L0C97;
    case 0x0C99: goto L0C99;
    case 0x0C9C: goto L0C9C;
    case 0x0C9E: goto L0C9E;
    case 0x0CA1: goto L0CA1;
    case 0x0CA3: goto L0CA3;
    case 0x0CA5: goto L0CA5;
    case 0x0CA7: goto L0CA7;
    case 0x0CAA: goto L0CAA;
    case 0x0CAB: goto L0CAB;
    case 0x0CAC: goto L0CAC;
    case 0x0CAE: goto L0CAE;
    case 0x0CAF: goto L0CAF;
    case 0x0CB1: goto L0CB1;
    case 0x0CB4: goto L0CB4;
    case 0x0CB7: goto L0CB7;
    case 0x0CBA: goto L0CBA;
    case 0x0CBC: goto L0CBC;
    case 0x0CBD: goto L0CBD;
    case 0x0CBF: goto L0CBF;
    case 0x0CC1: goto L0CC1;
    case 0x0CC2: goto L0CC2;
    case 0x0CC4: goto L0CC4;
    case 0x0CC6: goto L0CC6;
    case 0x0CCA: goto L0CCA;
    case 0x0CCE: goto L0CCE;
    case 0x0CD3: goto L0CD3;
    case 0x0CD4: goto L0CD4;
    case 0x0CD6: goto L0CD6;
    case 0x0CD8: goto L0CD8;
    case 0x0CDA: goto L0CDA;
    case 0x0CDC: goto L0CDC;
    case 0x0CDE: goto L0CDE;
    case 0x0CE0: goto L0CE0;
    case 0x0CE2: goto L0CE2;
    case 0x0CE4: goto L0CE4;
    case 0x0CE6: goto L0CE6;
    case 0x0CE8: goto L0CE8;
    case 0x0CEA: goto L0CEA;
    case 0x0CEC: goto L0CEC;
    case 0x0CEE: goto L0CEE;
    case 0x0CF1: goto L0CF1;
    case 0x0CF4: goto L0CF4;
    case 0x0CF7: goto L0CF7;
    case 0x0CFA: goto L0CFA;
    case 0x0CFC: goto L0CFC;
    case 0x0CFD: goto L0CFD;
    case 0x0D00: goto L0D00;
    case 0x0D01: goto L0D01;
    case 0x0D03: goto L0D03;
    case 0x0D05: goto L0D05;
    case 0x0D08: goto L0D08;
    case 0x0D0B: goto L0D0B;
    case 0x0D0D: goto L0D0D;
    case 0x0D0E: goto L0D0E;
    case 0x0D0F: goto L0D0F;
    case 0x0D10: goto L0D10;
    case 0x0D13: goto L0D13;
    case 0x0D14: goto L0D14;
    case 0x0D17: goto L0D17;
    case 0x0D18: goto L0D18;
    case 0x0D1B: goto L0D1B;
    case 0x0D1E: goto L0D1E;
    case 0x0D1F: goto L0D1F;
    case 0x0D24: goto L0D24;
    case 0x0D27: goto L0D27;
    case 0x0D2A: goto L0D2A;
    case 0x0D2C: goto L0D2C;
    case 0x0D2D: goto L0D2D;
    case 0x0D2E: goto L0D2E;
    case 0x0D30: goto L0D30;
    case 0x0D31: goto L0D31;
    case 0x0D32: goto L0D32;
    case 0x0D34: goto L0D34;
    case 0x0D37: goto L0D37;
    case 0x0D3A: goto L0D3A;
    case 0x0D3D: goto L0D3D;
    case 0x0D3F: goto L0D3F;
    case 0x0D41: goto L0D41;
    case 0x0D44: goto L0D44;
    case 0x0D46: goto L0D46;
    case 0x0D48: goto L0D48;
    case 0x0D4A: goto L0D4A;
    case 0x0D4B: goto L0D4B;
    case 0x0D4C: goto L0D4C;
    case 0x0D4E: goto L0D4E;
    case 0x0D50: goto L0D50;
    case 0x0D51: goto L0D51;
    case 0x0D54: goto L0D54;
    case 0x0D57: goto L0D57;
    case 0x0D58: goto L0D58;
    case 0x0D5A: goto L0D5A;
    case 0x0D5C: goto L0D5C;
    case 0x0D5E: goto L0D5E;
    case 0x0D61: goto L0D61;
    case 0x0D67: goto L0D67;
    case 0x0D6A: goto L0D6A;
    case 0x0D6D: goto L0D6D;
    case 0x0D70: goto L0D70;
    case 0x0D74: goto L0D74;
    case 0x0D77: goto L0D77;
    case 0x0D7A: goto L0D7A;
    case 0x0D7D: goto L0D7D;
    case 0x0D80: goto L0D80;
    case 0x0D84: goto L0D84;
    case 0x0D87: goto L0D87;
    case 0x0D89: goto L0D89;
    case 0x0D8C: goto L0D8C;
    case 0x0D90: goto L0D90;
    case 0x0D92: goto L0D92;
    case 0x0D94: goto L0D94;
    case 0x0D96: goto L0D96;
    case 0x0D9A: goto L0D9A;
    case 0x0D9E: goto L0D9E;
    case 0x0DA2: goto L0DA2;
    case 0x0DA7: goto L0DA7;
    case 0x0DA9: goto L0DA9;
    case 0x0DAC: goto L0DAC;
    case 0x0DAF: goto L0DAF;
    case 0x0DB3: goto L0DB3;
    case 0x0DB5: goto L0DB5;
    case 0x0DB9: goto L0DB9;
    case 0x0DBA: goto L0DBA;
    case 0x0DBC: goto L0DBC;
    case 0x0DBF: goto L0DBF;
    case 0x0DC2: goto L0DC2;
    case 0x0DC5: goto L0DC5;
    case 0x0DC6: goto L0DC6;
    case 0x0DC8: goto L0DC8;
    case 0x0DCA: goto L0DCA;
    case 0x0DCC: goto L0DCC;
    case 0x0DCE: goto L0DCE;
    case 0x0DD0: goto L0DD0;
    case 0x0DD3: goto L0DD3;
    case 0x0DD6: goto L0DD6;
    case 0x0DD9: goto L0DD9;
    case 0x0DDC: goto L0DDC;
    case 0x0DDD: goto L0DDD;
    case 0x0DDF: goto L0DDF;
    case 0x0DE1: goto L0DE1;
    case 0x0DE3: goto L0DE3;
    case 0x0DE5: goto L0DE5;
    case 0x0DE7: goto L0DE7;
    case 0x0DEA: goto L0DEA;
    case 0x0DED: goto L0DED;
    case 0x0DF0: goto L0DF0;
    case 0x0DF3: goto L0DF3;
    case 0x0DF4: goto L0DF4;
    case 0x0DF6: goto L0DF6;
    case 0x0DF8: goto L0DF8;
    case 0x0DFA: goto L0DFA;
    case 0x0DFC: goto L0DFC;
    case 0x0DFE: goto L0DFE;
    case 0x0DFF: goto L0DFF;
    case 0x0E00: goto L0E00;
    case 0x0E01: goto L0E01;
    case 0x0E02: goto L0E02;
    case 0x0E03: goto L0E03;
    case 0x0E05: goto L0E05;
    case 0x0E06: goto L0E06;
    case 0x0E08: goto L0E08;
    case 0x0E0A: goto L0E0A;
    case 0x0E0C: goto L0E0C;
    case 0x0E0F: goto L0E0F;
    case 0x0E12: goto L0E12;
    case 0x0E14: goto L0E14;
    case 0x0E16: goto L0E16;
    case 0x0E18: goto L0E18;
    case 0x0E1B: goto L0E1B;
    case 0x0E1E: goto L0E1E;
    case 0x0E22: goto L0E22;
    case 0x0E24: goto L0E24;
    case 0x0E26: goto L0E26;
    case 0x0E28: goto L0E28;
    case 0x0E29: goto L0E29;
    case 0x0E2D: goto L0E2D;
    case 0x0E31: goto L0E31;
    case 0x0E34: goto L0E34;
    case 0x0E38: goto L0E38;
    case 0x0E3A: goto L0E3A;
    case 0x0E3C: goto L0E3C;
    case 0x0E3E: goto L0E3E;
    case 0x0E3F: goto L0E3F;
    case 0x0E43: goto L0E43;
    case 0x0E47: goto L0E47;
    case 0x0E4A: goto L0E4A;
    case 0x0E4B: goto L0E4B;
    case 0x0E4E: goto L0E4E;
    case 0x0E50: goto L0E50;
    case 0x0E52: goto L0E52;
    case 0x0E56: goto L0E56;
    case 0x0E5A: goto L0E5A;
    case 0x0E5D: goto L0E5D;
    case 0x0E61: goto L0E61;
    case 0x0E64: goto L0E64;
    case 0x0E68: goto L0E68;
    case 0x0E6B: goto L0E6B;
    case 0x0E6E: goto L0E6E;
    case 0x0E71: goto L0E71;
    case 0x0E73: goto L0E73;
    case 0x0E76: goto L0E76;
    case 0x0E77: goto L0E77;
    case 0x0E7A: goto L0E7A;
    case 0x0E7D: goto L0E7D;
    case 0x0E7F: goto L0E7F;
    case 0x0E82: goto L0E82;
    case 0x0E83: goto L0E83;
    case 0x0E86: goto L0E86;
    case 0x0E87: goto L0E87;
    case 0x0E88: goto L0E88;
    case 0x0E8B: goto L0E8B;
    case 0x0E8F: goto L0E8F;
    case 0x0E92: goto L0E92;
    case 0x0E95: goto L0E95;
    case 0x0E99: goto L0E99;
    case 0x0E9C: goto L0E9C;
    case 0x0E9F: goto L0E9F;
    case 0x0EA3: goto L0EA3;
    case 0x0EA7: goto L0EA7;
    case 0x0EAA: goto L0EAA;
    case 0x0EAE: goto L0EAE;
    case 0x0EB1: goto L0EB1;
    case 0x0EB4: goto L0EB4;
    case 0x0EB8: goto L0EB8;
    case 0x0EBC: goto L0EBC;
    case 0x0EBF: goto L0EBF;
    case 0x0EC3: goto L0EC3;
    case 0x0EC6: goto L0EC6;
    case 0x0EC9: goto L0EC9;
    case 0x0ECA: goto L0ECA;
    case 0x0ECE: goto L0ECE;
    case 0x0ED0: goto L0ED0;
    case 0x0ED3: goto L0ED3;
    case 0x0ED4: goto L0ED4;
    case 0x0ED8: goto L0ED8;
    case 0x0EDA: goto L0EDA;
    case 0x0EDD: goto L0EDD;
    case 0x0EDE: goto L0EDE;
    case 0x0EE2: goto L0EE2;
    case 0x0EE4: goto L0EE4;
    case 0x0EE8: goto L0EE8;
    case 0x0EEC: goto L0EEC;
    case 0x0EF0: goto L0EF0;
    case 0x0EF4: goto L0EF4;
    case 0x0EF8: goto L0EF8;
    case 0x0EFC: goto L0EFC;
    case 0x0EFE: goto L0EFE;
    case 0x0F05: goto L0F05;
    case 0x0F08: goto L0F08;
    case 0x0F0C: goto L0F0C;
    case 0x0F10: goto L0F10;
    case 0x0F13: goto L0F13;
    case 0x0F17: goto L0F17;
    case 0x0F19: goto L0F19;
    case 0x0F1B: goto L0F1B;
    case 0x0F1D: goto L0F1D;
    case 0x0F1F: goto L0F1F;
    case 0x0F22: goto L0F22;
    case 0x0F24: goto L0F24;
    case 0x0F27: goto L0F27;
    case 0x0F2A: goto L0F2A;
    case 0x0F2E: goto L0F2E;
    case 0x0F30: goto L0F30;
    case 0x0F32: goto L0F32;
    case 0x0F34: goto L0F34;
    case 0x0F36: goto L0F36;
    case 0x0F39: goto L0F39;
    case 0x0F3B: goto L0F3B;
    case 0x0F3E: goto L0F3E;
    case 0x0F41: goto L0F41;
    case 0x0F45: goto L0F45;
    case 0x0F47: goto L0F47;
    case 0x0F49: goto L0F49;
    case 0x0F4B: goto L0F4B;
    case 0x0F4D: goto L0F4D;
    case 0x0F50: goto L0F50;
    case 0x0F52: goto L0F52;
    case 0x0F55: goto L0F55;
    case 0x0F58: goto L0F58;
    case 0x0F5B: goto L0F5B;
    case 0x0F5F: goto L0F5F;
    case 0x0F61: goto L0F61;
    case 0x0F64: goto L0F64;
    case 0x0F66: goto L0F66;
    case 0x0F68: goto L0F68;
    case 0x0F6A: goto L0F6A;
    case 0x0F6D: goto L0F6D;
    case 0x0F6F: goto L0F6F;
    case 0x0F71: goto L0F71;
    case 0x0F73: goto L0F73;
    case 0x0F78: goto L0F78;
    case 0x0F7C: goto L0F7C;
    case 0x0F7E: goto L0F7E;
    case 0x0F81: goto L0F81;
    case 0x0F83: goto L0F83;
    case 0x0F85: goto L0F85;
    case 0x0F87: goto L0F87;
    case 0x0F8A: goto L0F8A;
    case 0x0F8C: goto L0F8C;
    case 0x0F8F: goto L0F8F;
    case 0x0F92: goto L0F92;
    case 0x0F96: goto L0F96;
    case 0x0F98: goto L0F98;
    case 0x0F9A: goto L0F9A;
    case 0x0F9C: goto L0F9C;
    case 0x0F9E: goto L0F9E;
    case 0x0FA1: goto L0FA1;
    case 0x0FA3: goto L0FA3;
    case 0x0FA6: goto L0FA6;
    case 0x0FA8: goto L0FA8;
    case 0x0FAB: goto L0FAB;
    case 0x0FAD: goto L0FAD;
    case 0x0FB0: goto L0FB0;
    case 0x0FB2: goto L0FB2;
    case 0x0FB5: goto L0FB5;
    case 0x0FB7: goto L0FB7;
    case 0x0FBA: goto L0FBA;
    case 0x0FBD: goto L0FBD;
    case 0x0FC0: goto L0FC0;
    case 0x0FC4: goto L0FC4;
    case 0x0FC6: goto L0FC6;
    case 0x0FC8: goto L0FC8;
    case 0x0FCB: goto L0FCB;
    case 0x0FCF: goto L0FCF;
    case 0x0FD1: goto L0FD1;
    case 0x0FD3: goto L0FD3;
    case 0x0FD5: goto L0FD5;
    case 0x0FD7: goto L0FD7;
    case 0x0FDB: goto L0FDB;
    case 0x0FDD: goto L0FDD;
    case 0x0FE0: goto L0FE0;
    case 0x0FE4: goto L0FE4;
    case 0x0FE7: goto L0FE7;
    case 0x0FEB: goto L0FEB;
    case 0x0FED: goto L0FED;
    case 0x0FEF: goto L0FEF;
    case 0x0FF2: goto L0FF2;
    case 0x0FF6: goto L0FF6;
    case 0x0FF8: goto L0FF8;
    case 0x0FFA: goto L0FFA;
    case 0x0FFC: goto L0FFC;
    case 0x0FFE: goto L0FFE;
    case 0x1000: goto L1000;
    case 0x1003: goto L1003;
    case 0x1005: goto L1005;
    case 0x1009: goto L1009;
    case 0x100B: goto L100B;
    case 0x100E: goto L100E;
    case 0x1012: goto L1012;
    case 0x1015: goto L1015;
    case 0x1019: goto L1019;
    case 0x101B: goto L101B;
    case 0x101D: goto L101D;
    case 0x1020: goto L1020;
    case 0x1024: goto L1024;
    case 0x1026: goto L1026;
    case 0x1028: goto L1028;
    case 0x102A: goto L102A;
    case 0x102C: goto L102C;
    case 0x1030: goto L1030;
    case 0x1032: goto L1032;
    case 0x1035: goto L1035;
    case 0x1039: goto L1039;
    case 0x103A: goto L103A;
    case 0x103C: goto L103C;
    case 0x103E: goto L103E;
    case 0x103F: goto L103F;
    case 0x1041: goto L1041;
    case 0x1043: goto L1043;
    case 0x1045: goto L1045;
    case 0x1047: goto L1047;
    case 0x1049: goto L1049;
    case 0x104B: goto L104B;
    case 0x104D: goto L104D;
    case 0x104F: goto L104F;
    case 0x1054: goto L1054;
    case 0x1055: goto L1055;
    case 0x1058: goto L1058;
    case 0x1059: goto L1059;
    case 0x105B: goto L105B;
    case 0x105D: goto L105D;
    case 0x105E: goto L105E;
    case 0x1060: goto L1060;
    case 0x1063: goto L1063;
    case 0x1065: goto L1065;
    case 0x106A: goto L106A;
    case 0x106F: goto L106F;
    case 0x1073: goto L1073;
    case 0x1076: goto L1076;
    case 0x1078: goto L1078;
    case 0x107D: goto L107D;
    case 0x1082: goto L1082;
    case 0x1086: goto L1086;
    case 0x1089: goto L1089;
    case 0x108B: goto L108B;
    case 0x1090: goto L1090;
    case 0x1095: goto L1095;
    case 0x1099: goto L1099;
    case 0x109A: goto L109A;
    case 0x109C: goto L109C;
    case 0x109E: goto L109E;
    case 0x10A1: goto L10A1;
    case 0x10A3: goto L10A3;
    case 0x10A6: goto L10A6;
    case 0x10A9: goto L10A9;
    case 0x10AD: goto L10AD;
    case 0x10AF: goto L10AF;
    case 0x10B1: goto L10B1;
    case 0x10B3: goto L10B3;
    case 0x10B6: goto L10B6;
    case 0x10BA: goto L10BA;
    case 0x10BC: goto L10BC;
    case 0x10BE: goto L10BE;
    case 0x10C0: goto L10C0;
    case 0x10C1: goto L10C1;
    case 0x10C5: goto L10C5;
    case 0x10C7: goto L10C7;
    case 0x10C9: goto L10C9;
    case 0x10CB: goto L10CB;
    case 0x10CE: goto L10CE;
    case 0x10D1: goto L10D1;
    case 0x10D5: goto L10D5;
    case 0x10D7: goto L10D7;
    case 0x10D9: goto L10D9;
    case 0x10DB: goto L10DB;
    case 0x10DE: goto L10DE;
    case 0x10E1: goto L10E1;
    case 0x10E5: goto L10E5;
    case 0x10E7: goto L10E7;
    case 0x10E9: goto L10E9;
    case 0x10EB: goto L10EB;
    case 0x10EE: goto L10EE;
    case 0x10F2: goto L10F2;
    case 0x10F4: goto L10F4;
    case 0x10F6: goto L10F6;
    case 0x10F8: goto L10F8;
    case 0x10F9: goto L10F9;
    case 0x10FD: goto L10FD;
    case 0x10FF: goto L10FF;
    case 0x1101: goto L1101;
    case 0x1103: goto L1103;
    case 0x1105: goto L1105;
    case 0x1108: goto L1108;
    case 0x110A: goto L110A;
    case 0x110E: goto L110E;
    case 0x1110: goto L1110;
    case 0x1112: goto L1112;
    case 0x1114: goto L1114;
    case 0x1117: goto L1117;
    case 0x111A: goto L111A;
    case 0x111E: goto L111E;
    case 0x1120: goto L1120;
    case 0x1122: goto L1122;
    case 0x1125: goto L1125;
    case 0x1127: goto L1127;
    case 0x112B: goto L112B;
    case 0x112D: goto L112D;
    case 0x112F: goto L112F;
    case 0x1131: goto L1131;
    case 0x1134: goto L1134;
    case 0x1137: goto L1137;
    case 0x113B: goto L113B;
    case 0x113D: goto L113D;
    case 0x113F: goto L113F;
    case 0x1141: goto L1141;
    case 0x1144: goto L1144;
    case 0x1147: goto L1147;
    case 0x114B: goto L114B;
    case 0x114D: goto L114D;
    case 0x114F: goto L114F;
    case 0x1152: goto L1152;
    case 0x1153: goto L1153;
    case 0x1155: goto L1155;
    case 0x1157: goto L1157;
    case 0x115A: goto L115A;
    case 0x115C: goto L115C;
    case 0x1161: goto L1161;
    case 0x1166: goto L1166;
    case 0x116A: goto L116A;
    case 0x116E: goto L116E;
    case 0x1170: goto L1170;
    case 0x1175: goto L1175;
    case 0x117A: goto L117A;
    case 0x117E: goto L117E;
    case 0x1180: goto L1180;
    case 0x1182: goto L1182;
    case 0x1185: goto L1185;
    case 0x1187: goto L1187;
    case 0x118A: goto L118A;
    case 0x118E: goto L118E;
    case 0x1190: goto L1190;
    case 0x1192: goto L1192;
    case 0x1194: goto L1194;
    case 0x1197: goto L1197;
    case 0x119A: goto L119A;
    case 0x119E: goto L119E;
    case 0x11A0: goto L11A0;
    case 0x11A2: goto L11A2;
    case 0x11A4: goto L11A4;
    case 0x11A7: goto L11A7;
    case 0x11AC: goto L11AC;
    case 0x11AF: goto L11AF;
    case 0x11B2: goto L11B2;
    case 0x11B5: goto L11B5;
    case 0x11B7: goto L11B7;
    case 0x11BA: goto L11BA;
    case 0x11BD: goto L11BD;
    case 0x11C0: goto L11C0;
    case 0x11C3: goto L11C3;
    case 0x11C7: goto L11C7;
    case 0x11C9: goto L11C9;
    case 0x11CB: goto L11CB;
    case 0x11CE: goto L11CE;
    case 0x11D1: goto L11D1;
    case 0x11D5: goto L11D5;
    case 0x11D7: goto L11D7;
    case 0x11D9: goto L11D9;
    case 0x11DC: goto L11DC;
    case 0x11DD: goto L11DD;
    case 0x11DE: goto L11DE;
    case 0x11DF: goto L11DF;
    case 0x11E2: goto L11E2;
    case 0x11E4: goto L11E4;
    case 0x11E6: goto L11E6;
    case 0x11E9: goto L11E9;
    case 0x11ED: goto L11ED;
    case 0x11F0: goto L11F0;
    case 0x11F3: goto L11F3;
    case 0x11F5: goto L11F5;
    case 0x11F7: goto L11F7;
    case 0x11F8: goto L11F8;
    case 0x11F9: goto L11F9;
    case 0x11FC: goto L11FC;
    case 0x11FF: goto L11FF;
    case 0x1200: goto L1200;
    case 0x1201: goto L1201;
    case 0x1203: goto L1203;
    case 0x1207: goto L1207;
    case 0x1208: goto L1208;
    case 0x1209: goto L1209;
    case 0x120A: goto L120A;
    case 0x120B: goto L120B;
    case 0x120C: goto L120C;
    case 0x120F: goto L120F;
    case 0x1211: goto L1211;
    case 0x1213: goto L1213;
    case 0x1217: goto L1217;
    case 0x121B: goto L121B;
    case 0x121D: goto L121D;
    case 0x1223: goto L1223;
    case 0x1225: goto L1225;
    case 0x1227: goto L1227;
    case 0x1229: goto L1229;
    case 0x122A: goto L122A;
    case 0x122B: goto L122B;
    case 0x122C: goto L122C;
    case 0x122F: goto L122F;
    case 0x1231: goto L1231;
    case 0x1234: goto L1234;
    case 0x1236: goto L1236;
    case 0x1239: goto L1239;
    case 0x123B: goto L123B;
    case 0x123E: goto L123E;
    case 0x1240: goto L1240;
    case 0x1243: goto L1243;
    case 0x1245: goto L1245;
    case 0x1248: goto L1248;
    case 0x124A: goto L124A;
    case 0x124D: goto L124D;
    case 0x1252: goto L1252;
    case 0x1254: goto L1254;
    case 0x1256: goto L1256;
    case 0x1259: goto L1259;
    case 0x125E: goto L125E;
    case 0x125F: goto L125F;
    case 0x1262: goto L1262;
    case 0x1267: goto L1267;
    case 0x1269: goto L1269;
    case 0x126B: goto L126B;
    case 0x126E: goto L126E;
    case 0x1273: goto L1273;
    case 0x1274: goto L1274;
    case 0x1277: goto L1277;
    case 0x127C: goto L127C;
    case 0x127E: goto L127E;
    case 0x1280: goto L1280;
    case 0x1283: goto L1283;
    case 0x1288: goto L1288;
    case 0x1289: goto L1289;
    case 0x128C: goto L128C;
    case 0x1291: goto L1291;
    case 0x1293: goto L1293;
    case 0x1295: goto L1295;
    case 0x1298: goto L1298;
    case 0x129D: goto L129D;
    case 0x129E: goto L129E;
    case 0x12A1: goto L12A1;
    case 0x12A4: goto L12A4;
    case 0x12A7: goto L12A7;
    case 0x12A8: goto L12A8;
    case 0x12AB: goto L12AB;
    case 0x12AE: goto L12AE;
    case 0x12B1: goto L12B1;
    case 0x12B2: goto L12B2;
    case 0x12B5: goto L12B5;
    case 0x12B8: goto L12B8;
    case 0x12BB: goto L12BB;
    case 0x12BC: goto L12BC;
    case 0x12BF: goto L12BF;
    case 0x12C2: goto L12C2;
    case 0x12C5: goto L12C5;
    case 0x12C6: goto L12C6;
    case 0x12CA: goto L12CA;
    case 0x12CD: goto L12CD;
    case 0x12CF: goto L12CF;
    case 0x12D1: goto L12D1;
    case 0x12D4: goto L12D4;
    case 0x12D7: goto L12D7;
    case 0x12D8: goto L12D8;
    case 0x12DB: goto L12DB;
    case 0x12DE: goto L12DE;
    case 0x12E1: goto L12E1;
    case 0x12E2: goto L12E2;
    case 0x12E6: goto L12E6;
    case 0x12E9: goto L12E9;
    case 0x12EB: goto L12EB;
    case 0x12ED: goto L12ED;
    case 0x12F0: goto L12F0;
    case 0x12F3: goto L12F3;
    case 0x12F4: goto L12F4;
    case 0x12F7: goto L12F7;
    case 0x12FA: goto L12FA;
    case 0x12FD: goto L12FD;
    case 0x12FE: goto L12FE;
    case 0x1303: goto L1303;
    case 0x1307: goto L1307;
    case 0x1309: goto L1309;
    case 0x130B: goto L130B;
    case 0x130D: goto L130D;
    case 0x130F: goto L130F;
    case 0x1311: goto L1311;
    case 0x1313: goto L1313;
    case 0x1315: goto L1315;
    case 0x1316: goto L1316;
    case 0x1318: goto L1318;
    case 0x131A: goto L131A;
    case 0x131C: goto L131C;
    case 0x131E: goto L131E;
    case 0x1320: goto L1320;
    case 0x1322: goto L1322;
    case 0x1324: goto L1324;
    case 0x1325: goto L1325;
    case 0x1327: goto L1327;
    case 0x1329: goto L1329;
    case 0x132B: goto L132B;
    case 0x132D: goto L132D;
    case 0x132F: goto L132F;
    case 0x1331: goto L1331;
    case 0x1332: goto L1332;
    case 0x1334: goto L1334;
    case 0x1336: goto L1336;
    case 0x133B: goto L133B;
    case 0x133F: goto L133F;
    case 0x1341: goto L1341;
    case 0x1343: goto L1343;
    case 0x1345: goto L1345;
    case 0x1347: goto L1347;
    case 0x1349: goto L1349;
    case 0x134B: goto L134B;
    case 0x134D: goto L134D;
    case 0x134F: goto L134F;
    case 0x1350: goto L1350;
    case 0x1352: goto L1352;
    case 0x1354: goto L1354;
    case 0x1356: goto L1356;
    case 0x1358: goto L1358;
    case 0x135A: goto L135A;
    case 0x135B: goto L135B;
    case 0x135D: goto L135D;
    case 0x135F: goto L135F;
    case 0x1361: goto L1361;
    case 0x1363: goto L1363;
    case 0x1365: goto L1365;
    default: asm_bad_entry("CAMERA.ASM", entry);
    }

    /* seg004_A10  (+A10): three far routines nothing in the EXE calls. All three work on
       seg051 words 104h (the current view description), 106h (the last one create_matrix used)
       and 1B0h. A10 saves 104h in 1B0h; A1E restores it and clears 106h. */
L0A10: /* _seg004_A10 */
    /* 0A10  push    ds */
    push16(asm_ds);
L0A11:
    /* 0A11  mov     cx,seg seg051 */
    CX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L0A14:
    /* 0A14  mov     ds,cx */
    SET_DS(CX);
L0A16:
    /* 0A16  mov     ax,word ptr ds:[104h] */
    AX = rw(pDS, 0x104);
L0A19:
    /* 0A19  mov     word ptr ds:[1B0h],ax */
    ww(pDS, 0x1B0, AX);
L0A1C:
    /* 0A1C  pop     ds */
    SET_DS(pop16());
L0A1D:
    /* 0A1D  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg004_A1E  (+A1E) */
L0A1E: /* _seg004_A1E */
    /* 0A1E  push    ds */
    push16(asm_ds);
L0A1F:
    /* 0A1F  mov     cx,seg seg051 */
    CX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L0A22:
    /* 0A22  mov     ds,cx */
    SET_DS(CX);
L0A24:
    /* 0A24  mov     ax,word ptr ds:[1B0h] */
    AX = rw(pDS, 0x1B0);
L0A27:
    /* 0A27  mov     word ptr ds:[104h],ax */
    ww(pDS, 0x104, AX);
L0A2A:
    /* 0A2A  mov     word ptr ds:[106h],0 */
    ww(pDS, 0x106, 0x0);
L0A30:
    /* 0A30  pop     ds */
    SET_DS(pop16());
L0A31:
    /* 0A31  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg004_A32  (+A32)
       In: AX = a view description (stored at 104h), BX nonzero to clear 106h. Clears 19Ah and
       sets ss:[4B0h] (seg063) to -1, or to 0 when the description's words 0, 2 and 3 are
       0, 974h and 0. Nothing in the EXE calls it; what the record and the flag mean is not known. */
L0A32: /* _seg004_A32 */
    /* 0A32  push    ds */
    push16(asm_ds);
L0A33:
    /* 0A33  mov     cx,seg seg051 */
    CX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L0A36:
    /* 0A36  mov     ds,cx */
    SET_DS(CX);
L0A38:
    /* 0A38  mov     word ptr ds:[104h],ax */
    ww(pDS, 0x104, AX);
L0A3B:
    /* 0A3B  or      bx,bx */
    BX = logic16((uint16_t)(BX | BX));
L0A3D:
    /* 0A3D  je      short L0A45 */
    if (ZF) goto L0A45;
L0A3F:
    /* 0A3F  mov     word ptr ds:[106h],0 */
    ww(pDS, 0x106, 0x0);
L0A45: /* L0A45 */
    /* 0A45  mov     word ptr ds:[19Ah],0 */
    ww(pDS, 0x19A, 0x0);
L0A4B:
    /* 0A4B  mov     word ptr ss:[4B0h],0FFFFh */
    ww(pSS, 0x4B0, 0xFFFF);
L0A52:
    /* 0A52  mov     di,ax */
    DI = AX;
L0A54:
    /* 0A54  cmp     word ptr [di],0 */
    sub16(rw(pDS, DI), 0x0, 0);
L0A57:
    /* 0A57  jne     short L0A6F */
    if (!ZF) goto L0A6F;
L0A59:
    /* 0A59  mov     ax,word ptr [di+4] */
    AX = rw(pDS, DI + 0x4);
L0A5C:
    /* 0A5C  cmp     ax,974h */
    sub16(AX, 0x974, 0);
L0A5F:
    /* 0A5F  jne     short L0A6F */
    if (!ZF) goto L0A6F;
L0A61:
    /* 0A61  test    word ptr [di+6],0FFFFh */
    logic16((uint16_t)(rw(pDS, DI + 0x6) & 0xFFFF));
L0A66:
    /* 0A66  jne     short L0A6F */
    if (!ZF) goto L0A6F;
L0A68:
    /* 0A68  mov     word ptr ss:[4B0h],0 */
    ww(pSS, 0x4B0, 0x0);
L0A6F: /* L0A6F */
    /* 0A6F  pop     ds */
    SET_DS(pop16());
L0A70:
    /* 0A70  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg004_A71  (+A71)
       Builds the frame's view matrix. Calls the camera routine its first word selects through the table at
       108h, records 104h in 106h, and keeps a copy of the eye position
       (26BAh, 12 bytes) and of the matrix's depth column and 2726h at 19Ch. Called by render_3d.
       Sets 15Ch to -1 first; head_view may set it to a model address.
       UW1 only (no int 2 check on 104h, and this transition): when 106h is set and differs from
       104h, 19Ah becomes 8000h; while 19Ah is nonzero it falls by ss:[9B0h] / 4 a frame, and the
       eye position, the zoom (2726h) and the view direction (1606h, 160Ch, 1612h) are blended
       from the copies saved at 19Ch.. towards the new ones by 19Ah / 8000h (the factors at
       1A8h..1AEh), and the matrix is rebuilt from that direction (seg004_EFE). */
L0A71: /* _create_matrix */
    /* 0A71  mov     ax,0FFFFh */
    AX = 0xFFFF;
L0A74:
    /* 0A74  mov     word ptr ds:[15Ch],ax */
    ww(pDS, 0x15C, AX);
L0A77:
    /* 0A77  mov     si,word ptr ds:[104h] */
    SI = rw(pDS, 0x104);
L0A7B: /* L0A7B */
    /* 0A7B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0A7C:
    /* 0A7C  mov     bx,ax */
    BX = AX;
L0A7E:
    /* 0A7E  mov     ax,word ptr [bx+108h] */
    AX = rw(pDS, BX + 0x108);
L0A82:
    /* 0A82  call    ax */
    if ((c = asm_call(ASM_JMP(0x06E7, AX), 0x0A84)) != 0) return c;
L0A84:
    /* 0A84  mov     ax,word ptr ds:[106h] */
    AX = rw(pDS, 0x106);
L0A87:
    /* 0A87  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L0A89:
    /* 0A89  jne     short L0A8E */
    if (!ZF) goto L0A8E;
L0A8B:
    /* 0A8B  jmp     L0BD4 */
    goto L0BD4;
L0A8E: /* L0A8E */
    /* 0A8E  cmp     ax,word ptr ds:[104h] */
    sub16(AX, rw(pDS, 0x104), 0);
L0A92:
    /* 0A92  je      short L0A9A */
    if (ZF) goto L0A9A;
L0A94:
    /* 0A94  mov     ax,8000h */
    AX = 0x8000;
L0A97:
    /* 0A97  mov     word ptr ds:[19Ah],ax */
    ww(pDS, 0x19A, AX);
L0A9A: /* L0A9A */
    /* 0A9A  mov     bx,word ptr ds:[19Ah] */
    BX = rw(pDS, 0x19A);
L0A9E:
    /* 0A9E  or      bx,bx */
    BX = logic16((uint16_t)(BX | BX));
L0AA0:
    /* 0AA0  jne     short L0AA5 */
    if (!ZF) goto L0AA5;
L0AA2:
    /* 0AA2  jmp     L0BD4 */
    goto L0BD4;
L0AA5: /* L0AA5 */
    /* 0AA5  mov     dx,word ptr ss:[9B0h] */
    DX = rw(pSS, 0x9B0);
L0AAA:
    /* 0AAA  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L0AAC:
    /* 0AAC  sar     dx,1 */
    DX = (uint16_t)((int16_t)DX >> 1);
L0AAE:
    /* 0AAE  sar     dx,1 */
    DX = (uint16_t)((int16_t)DX >> 1);
L0AB0:
    /* 0AB0  mov     cx,bx */
    CX = BX;
L0AB2:
    /* 0AB2  sub     cx,dx */
    CX = sub16(CX, DX, 0);
L0AB4:
    /* 0AB4  jns     short L0ABC */
    if (!SF) goto L0ABC;
L0AB6:
    /* 0AB6  mov     word ptr ds:[19Ah],ax */
    ww(pDS, 0x19A, AX);
L0AB9:
    /* 0AB9  jmp     L0BD4 */
    goto L0BD4;
L0ABC: /* L0ABC */
    /* 0ABC  mov     word ptr ds:[19Ah],cx */
    ww(pDS, 0x19A, CX);
L0AC0:
    /* 0AC0  sar     dx,1 */
    DX = (uint16_t)((int16_t)DX >> 1);
L0AC2:
    /* 0AC2  div     bx */
    if (asm_div16(BX) && (c = asm_divfault(0x06E7, 0x0AC2, 2)) != 0) return c;
L0AC4:
    /* 0AC4  mov     bx,7FFFh */
    BX = 0x7FFF;
L0AC7:
    /* 0AC7  sub     bx,ax */
    BX = (uint16_t)(BX - AX);
L0AC9:
    /* 0AC9  mov     ax,word ptr ds:[1AEh] */
    AX = rw(pDS, 0x1AE);
L0ACC:
    /* 0ACC  mul     bx */
    mul16(BX);
L0ACE:
    /* 0ACE  shl     ax,1 */
    AX = shl16(AX, 1);
L0AD0:
    /* 0AD0  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0AD2:
    /* 0AD2  mov     si,dx */
    SI = DX;
L0AD4:
    /* 0AD4  mov     bp,ax */
    BP = AX;
L0AD6:
    /* 0AD6  mov     ax,word ptr ds:[2726h] */
    AX = rw(pDS, 0x2726);
L0AD9:
    /* 0AD9  sub     bx,7FFFh */
    BX = (uint16_t)(BX - 0x7FFF);
L0ADD:
    /* 0ADD  neg     bx */
    BX = (uint16_t)-BX;
L0ADF:
    /* 0ADF  mul     bx */
    mul16(BX);
L0AE1:
    /* 0AE1  shl     ax,1 */
    AX = shl16(AX, 1);
L0AE3:
    /* 0AE3  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0AE5:
    /* 0AE5  sub     bx,7FFFh */
    BX = (uint16_t)(BX - 0x7FFF);
L0AE9:
    /* 0AE9  neg     bx */
    BX = (uint16_t)-BX;
L0AEB:
    /* 0AEB  add     ax,bx */
    AX = add16(AX, BX, 0);
L0AED:
    /* 0AED  adc     dx,si */
    DX = (uint16_t)(DX + SI + CF);
L0AEF:
    /* 0AEF  mov     word ptr ds:[2726h],dx */
    ww(pDS, 0x2726, DX);
L0AF3:
    /* 0AF3  mov     ax,word ptr ds:[EYE_X] */
    AX = rw(pDS, 0x26BA);
L0AF6:
    /* 0AF6  sub     ax,word ptr ds:[19Ch] */
    AX = (uint16_t)(AX - rw(pDS, 0x19C));
L0AFA:
    /* 0AFA  imul    bx */
    imul16(BX);
L0AFC:
    /* 0AFC  shl     ax,1 */
    AX = shl16(AX, 1);
L0AFE:
    /* 0AFE  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0B00:
    /* 0B00  sub     word ptr ds:[EYE_X],dx */
    ww(pDS, 0x26BA, sub16(rw(pDS, 0x26BA), DX, 0));
L0B04:
    /* 0B04  mov     ax,dx */
    AX = DX;
L0B06:
    /* 0B06  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0B07:
    /* 0B07  sbb     word ptr ds:[EYE_X+2],dx */
    ww(pDS, 0x26BC, (uint16_t)(rw(pDS, 0x26BC) - DX - CF));
L0B0B:
    /* 0B0B  mov     ax,word ptr ds:[EYE_Y] */
    AX = rw(pDS, 0x26BE);
L0B0E:
    /* 0B0E  sub     ax,word ptr ds:[1A0h] */
    AX = (uint16_t)(AX - rw(pDS, 0x1A0));
L0B12:
    /* 0B12  imul    bx */
    imul16(BX);
L0B14:
    /* 0B14  shl     ax,1 */
    AX = shl16(AX, 1);
L0B16:
    /* 0B16  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0B18:
    /* 0B18  sub     word ptr ds:[EYE_Y],dx */
    ww(pDS, 0x26BE, sub16(rw(pDS, 0x26BE), DX, 0));
L0B1C:
    /* 0B1C  mov     ax,dx */
    AX = DX;
L0B1E:
    /* 0B1E  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0B1F:
    /* 0B1F  sbb     word ptr ds:[EYE_Y+2],dx */
    ww(pDS, 0x26C0, (uint16_t)(rw(pDS, 0x26C0) - DX - CF));
L0B23:
    /* 0B23  mov     ax,word ptr ds:[EYE_Z] */
    AX = rw(pDS, 0x26C2);
L0B26:
    /* 0B26  sub     ax,word ptr ds:[1A4h] */
    AX = (uint16_t)(AX - rw(pDS, 0x1A4));
L0B2A:
    /* 0B2A  imul    bx */
    imul16(BX);
L0B2C:
    /* 0B2C  shl     ax,1 */
    AX = shl16(AX, 1);
L0B2E:
    /* 0B2E  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0B30:
    /* 0B30  sub     word ptr ds:[EYE_Z],dx */
    ww(pDS, 0x26C2, sub16(rw(pDS, 0x26C2), DX, 0));
L0B34:
    /* 0B34  mov     ax,dx */
    AX = DX;
L0B36:
    /* 0B36  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0B37:
    /* 0B37  sbb     word ptr ds:[EYE_Z+2],dx */
    ww(pDS, 0x26C4, (uint16_t)(rw(pDS, 0x26C4) - DX - CF));
L0B3B:
    /* 0B3B  mov     ax,word ptr ds:[1A8h] */
    AX = rw(pDS, 0x1A8);
L0B3E:
    /* 0B3E  imul    bx */
    imul16(BX);
L0B40:
    /* 0B40  shl     ax,1 */
    AX = shl16(AX, 1);
L0B42:
    /* 0B42  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0B44:
    /* 0B44  mov     si,dx */
    SI = DX;
L0B46:
    /* 0B46  mov     bp,ax */
    BP = AX;
L0B48:
    /* 0B48  mov     ax,word ptr ds:[MAT_XZ] */
    AX = rw(pDS, 0x1606);
L0B4B:
    /* 0B4B  sub     bx,7FFFh */
    BX = (uint16_t)(BX - 0x7FFF);
L0B4F:
    /* 0B4F  neg     bx */
    BX = (uint16_t)-BX;
L0B51:
    /* 0B51  imul    bx */
    imul16(BX);
L0B53:
    /* 0B53  shl     ax,1 */
    AX = shl16(AX, 1);
L0B55:
    /* 0B55  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0B57:
    /* 0B57  sub     bx,7FFFh */
    BX = (uint16_t)(BX - 0x7FFF);
L0B5B:
    /* 0B5B  neg     bx */
    BX = (uint16_t)-BX;
L0B5D:
    /* 0B5D  add     ax,bx */
    AX = add16(AX, BX, 0);
L0B5F:
    /* 0B5F  adc     dx,si */
    DX = (uint16_t)(DX + SI + CF);
L0B61:
    /* 0B61  mov     word ptr ds:[MAT_XZ],dx */
    ww(pDS, 0x1606, DX);
L0B65:
    /* 0B65  mov     ax,word ptr ds:[1AAh] */
    AX = rw(pDS, 0x1AA);
L0B68:
    /* 0B68  imul    bx */
    imul16(BX);
L0B6A:
    /* 0B6A  shl     ax,1 */
    AX = shl16(AX, 1);
L0B6C:
    /* 0B6C  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0B6E:
    /* 0B6E  mov     si,dx */
    SI = DX;
L0B70:
    /* 0B70  mov     bp,ax */
    BP = AX;
L0B72:
    /* 0B72  mov     ax,word ptr ds:[MAT_YZ] */
    AX = rw(pDS, 0x160C);
L0B75:
    /* 0B75  sub     bx,7FFFh */
    BX = (uint16_t)(BX - 0x7FFF);
L0B79:
    /* 0B79  neg     bx */
    BX = (uint16_t)-BX;
L0B7B:
    /* 0B7B  imul    bx */
    imul16(BX);
L0B7D:
    /* 0B7D  shl     ax,1 */
    AX = shl16(AX, 1);
L0B7F:
    /* 0B7F  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0B81:
    /* 0B81  sub     bx,7FFFh */
    BX = (uint16_t)(BX - 0x7FFF);
L0B85:
    /* 0B85  neg     bx */
    BX = (uint16_t)-BX;
L0B87:
    /* 0B87  add     ax,bx */
    AX = add16(AX, BX, 0);
L0B89:
    /* 0B89  adc     dx,si */
    DX = (uint16_t)(DX + SI + CF);
L0B8B:
    /* 0B8B  mov     word ptr ds:[MAT_YZ],dx */
    ww(pDS, 0x160C, DX);
L0B8F:
    /* 0B8F  mov     ax,word ptr ds:[1ACh] */
    AX = rw(pDS, 0x1AC);
L0B92:
    /* 0B92  imul    bx */
    imul16(BX);
L0B94:
    /* 0B94  shl     ax,1 */
    AX = shl16(AX, 1);
L0B96:
    /* 0B96  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0B98:
    /* 0B98  mov     si,dx */
    SI = DX;
L0B9A:
    /* 0B9A  mov     bp,ax */
    BP = AX;
L0B9C:
    /* 0B9C  mov     ax,word ptr ds:[MAT_ZZ] */
    AX = rw(pDS, 0x1612);
L0B9F:
    /* 0B9F  sub     bx,7FFFh */
    BX = (uint16_t)(BX - 0x7FFF);
L0BA3:
    /* 0BA3  neg     bx */
    BX = (uint16_t)-BX;
L0BA5:
    /* 0BA5  imul    bx */
    imul16(BX);
L0BA7:
    /* 0BA7  shl     ax,1 */
    AX = shl16(AX, 1);
L0BA9:
    /* 0BA9  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0BAB:
    /* 0BAB  sub     bx,7FFFh */
    BX = (uint16_t)(BX - 0x7FFF);
L0BAF:
    /* 0BAF  neg     bx */
    BX = (uint16_t)-BX;
L0BB1:
    /* 0BB1  add     ax,bx */
    AX = add16(AX, BX, 0);
L0BB3:
    /* 0BB3  adc     dx,si */
    DX = (uint16_t)(DX + SI + CF);
L0BB5:
    /* 0BB5  mov     word ptr ds:[MAT_ZZ],dx */
    ww(pDS, 0x1612, DX);
L0BB9:
    /* 0BB9  mov     ax,word ptr ds:[MAT_XZ] */
    AX = rw(pDS, 0x1606);
L0BBC:
    /* 0BBC  mov     word ptr ds:[15Eh],ax */
    ww(pDS, 0x15E, AX);
L0BBF:
    /* 0BBF  mov     ax,word ptr ds:[MAT_YZ] */
    AX = rw(pDS, 0x160C);
L0BC2:
    /* 0BC2  mov     word ptr ds:[162h],ax */
    ww(pDS, 0x162, AX);
L0BC5:
    /* 0BC5  mov     ax,word ptr ds:[MAT_ZZ] */
    AX = rw(pDS, 0x1612);
L0BC8:
    /* 0BC8  mov     word ptr ds:[166h],ax */
    ww(pDS, 0x166, AX);
L0BCB:
    /* 0BCB  call    _seg004_EFE */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x0EFE), 0x0BCE)) != 0) return c;
L0BCE:
    /* 0BCE  mov     ax,0FFFFh */
    AX = 0xFFFF;
L0BD1:
    /* 0BD1  mov     word ptr ds:[15Ch],ax */
    ww(pDS, 0x15C, AX);
L0BD4: /* L0BD4 */
    /* 0BD4  mov     ax,word ptr ds:[104h] */
    AX = rw(pDS, 0x104);
L0BD7:
    /* 0BD7  mov     word ptr ds:[106h],ax */
    ww(pDS, 0x106, AX);
L0BDA:
    /* 0BDA  mov     si,EYE_X */
    SI = 0x26BA;
L0BDD:
    /* 0BDD  mov     di,19Ch */
    DI = 0x19C;
L0BE0:
    /* 0BE0  mov     cx,6 */
    CX = 0x6;
L0BE3:
    /* 0BE3  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0BE5:
    /* 0BE5  mov     ax,word ptr ds:[MAT_XZ] */
    AX = rw(pDS, 0x1606);
L0BE8:
    /* 0BE8  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0BE9:
    /* 0BE9  mov     ax,word ptr ds:[MAT_YZ] */
    AX = rw(pDS, 0x160C);
L0BEC:
    /* 0BEC  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0BED:
    /* 0BED  mov     ax,word ptr ds:[MAT_ZZ] */
    AX = rw(pDS, 0x1612);
L0BF0:
    /* 0BF0  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0BF1:
    /* 0BF1  mov     ax,word ptr ds:[2726h] */
    AX = rw(pDS, 0x2726);
L0BF4:
    /* 0BF4  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0BF5: /* L0BF5 */
    /* 0BF5  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_BF6  (+BF6)
       The camera routine (FM head_view). In: SI = the view record after its selector word, ES =
       DS = seg051. Reads the zoom word (2726h) and the address BP of the eye record (and
       sets 15Ch when the third word is 0 and ss:[9B2h] is clear). On first use (the record's
       word +6 zero, then set to 1) it builds a two-angle matrix in the record
       (seg004_1153) and, when its word +1Eh is nonzero, seg004_C81's offset. It builds
       the eye's matrix with angles_2_matrix when the eye record's first word is set, rotates the
       offset (mm3x9t) and adds the eye record's 32-bit coordinates to give the eye position at
       26BAh..26C4h, and composes the final matrix at 14B2h (mm9x9). What each record field means
       beyond that has not been traced. */
L0BF6: /* _head_view */
    /* 0BF6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0BF7:
    /* 0BF7  mov     word ptr ds:[2726h],ax */
    ww(pDS, 0x2726, AX);
L0BFA:
    /* 0BFA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0BFB:
    /* 0BFB  mov     bp,ax */
    BP = AX;
L0BFD:
    /* 0BFD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0BFE:
    /* 0BFE  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L0C00:
    /* 0C00  jne     short L0C13 */
    if (!ZF) goto L0C13;
L0C02:
    /* 0C02  test    word ptr ss:[9B2h],0FFFFh */
    logic16((uint16_t)(rw(pSS, 0x9B2) & 0xFFFF));
L0C09:
    /* 0C09  jne     short L0C13 */
    if (!ZF) goto L0C13;
L0C0B:
    /* 0C0B  mov     ax,bp */
    AX = BP;
L0C0D:
    /* 0C0D  sub     ax,4 */
    AX = (uint16_t)(AX - 0x4);
L0C10:
    /* 0C10  mov     word ptr ds:[15Ch],ax */
    ww(pDS, 0x15C, AX);
L0C13: /* L0C13 */
    /* 0C13  test    word ptr [si+6],0FFFFh */
    logic16((uint16_t)(rw(pDS, SI + 0x6) & 0xFFFF));
L0C18:
    /* 0C18  jne     short L0C38 */
    if (!ZF) goto L0C38;
L0C1A:
    /* 0C1A  mov     word ptr [si+6],1 */
    ww(pDS, SI + 0x6, 0x1);
L0C1F:
    /* 0C1F  push    si */
    push16(SI);
L0C20:
    /* 0C20  push    bp */
    push16(BP);
L0C21:
    /* 0C21  add     si,8 */
    SI = (uint16_t)(SI + 0x8);
L0C24:
    /* 0C24  mov     di,si */
    DI = SI;
L0C26:
    /* 0C26  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L0C29:
    /* 0C29  call    _seg004_1153 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x1153), 0x0C2C)) != 0) return c;
L0C2C:
    /* 0C2C  pop     bp */
    BP = pop16();
L0C2D:
    /* 0C2D  pop     si */
    SI = pop16();
L0C2E:
    /* 0C2E  mov     di,word ptr [si+1Eh] */
    DI = rw(pDS, SI + 0x1E);
L0C31:
    /* 0C31  or      di,di */
    DI = logic16((uint16_t)(DI | DI));
L0C33:
    /* 0C33  je      short L0C38 */
    if (ZF) goto L0C38;
L0C35:
    /* 0C35  call    _seg004_C81 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x0C81), 0x0C38)) != 0) return c;
L0C38: /* L0C38 */
    /* 0C38  add     bp,12h */
    BP = (uint16_t)(BP + 0x12);
L0C3B:
    /* 0C3B  test    word ptr [bp-12h],0FFFFh */
    logic16((uint16_t)(rw(pSS, BP + 0xFFEE) & 0xFFFF));
L0C40:
    /* 0C40  je      short L0C47 */
    if (ZF) goto L0C47;
L0C42:
    /* 0C42  push    si */
    push16(SI);
L0C43:
    /* 0C43  call    _angles_2_matrix */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x1059), 0x0C46)) != 0) return c;
L0C46:
    /* 0C46  pop     si */
    SI = pop16();
L0C47: /* L0C47 */
    /* 0C47  call    _mm3x9t */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x5B28), 0x0C4A)) != 0) return c;
L0C4A:
    /* 0C4A  mov     ax,bx */
    AX = BX;
L0C4C:
    /* 0C4C  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0C4D:
    /* 0C4D  add     ax,word ptr [bp-0Ch] */
    AX = add16(AX, rw(pSS, BP + 0xFFF4), 0);
L0C50:
    /* 0C50  adc     dx,word ptr [bp-0Ah] */
    DX = (uint16_t)(DX + rw(pSS, BP + 0xFFF6) + CF);
L0C53:
    /* 0C53  mov     word ptr ds:[EYE_X],ax */
    ww(pDS, 0x26BA, AX);
L0C56:
    /* 0C56  mov     word ptr ds:[EYE_X+2],dx */
    ww(pDS, 0x26BC, DX);
L0C5A:
    /* 0C5A  mov     ax,cx */
    AX = CX;
L0C5C:
    /* 0C5C  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0C5D:
    /* 0C5D  add     ax,word ptr [bp-8] */
    AX = add16(AX, rw(pSS, BP + 0xFFF8), 0);
L0C60:
    /* 0C60  adc     dx,word ptr [bp-6] */
    DX = (uint16_t)(DX + rw(pSS, BP + 0xFFFA) + CF);
L0C63:
    /* 0C63  mov     word ptr ds:[EYE_Y],ax */
    ww(pDS, 0x26BE, AX);
L0C66:
    /* 0C66  mov     word ptr ds:[EYE_Y+2],dx */
    ww(pDS, 0x26C0, DX);
L0C6A:
    /* 0C6A  mov     ax,di */
    AX = DI;
L0C6C:
    /* 0C6C  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0C6D:
    /* 0C6D  add     ax,word ptr [bp-4] */
    AX = add16(AX, rw(pSS, BP + 0xFFFC), 0);
L0C70:
    /* 0C70  adc     dx,word ptr [bp-2] */
    DX = (uint16_t)(DX + rw(pSS, BP + 0xFFFE) + CF);
L0C73:
    /* 0C73  mov     word ptr ds:[EYE_Z],ax */
    ww(pDS, 0x26C2, AX);
L0C76:
    /* 0C76  mov     word ptr ds:[EYE_Z+2],dx */
    ww(pDS, 0x26C4, DX);
L0C7A:
    /* 0C7A  add     si,0Ch */
    SI = (uint16_t)(SI + 0xC);
L0C7D:
    /* 0C7D  call    _mm9x9 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x58FA), 0x0C80)) != 0) return c;
L0C80:
    /* 0C80  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_C81  (+C81)
       In: DI = a distance, SI = a matrix record. Writes -(DI * column) for the matrix column at
       SI+10h, +16h, +1Ch into SI+0..+4, a vector DI units back along that axis; probably the
       camera's offset behind the eye (inferred from head_view's use when the record's +1Eh word
       is set; not checked in play). */
L0C81: /* _seg004_C81 */
    /* 0C81  mov     ax,di */
    AX = DI;
L0C83:
    /* 0C83  imul    word ptr [si+10h] */
    imul16(rw(pDS, SI + 0x10));
L0C86:
    /* 0C86  shl     ax,1 */
    AX = shl16(AX, 1);
L0C88:
    /* 0C88  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0C8A:
    /* 0C8A  neg     dx */
    DX = (uint16_t)-DX;
L0C8C:
    /* 0C8C  mov     word ptr [si],dx */
    ww(pDS, SI, DX);
L0C8E:
    /* 0C8E  mov     ax,di */
    AX = DI;
L0C90:
    /* 0C90  imul    word ptr [si+16h] */
    imul16(rw(pDS, SI + 0x16));
L0C93:
    /* 0C93  shl     ax,1 */
    AX = shl16(AX, 1);
L0C95:
    /* 0C95  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0C97:
    /* 0C97  neg     dx */
    DX = (uint16_t)-DX;
L0C99:
    /* 0C99  mov     word ptr [si+2],dx */
    ww(pDS, SI + 0x2, DX);
L0C9C:
    /* 0C9C  mov     ax,di */
    AX = DI;
L0C9E:
    /* 0C9E  imul    word ptr [si+1Ch] */
    imul16(rw(pDS, SI + 0x1C));
L0CA1:
    /* 0CA1  shl     ax,1 */
    AX = shl16(AX, 1);
L0CA3:
    /* 0CA3  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0CA5:
    /* 0CA5  neg     dx */
    DX = neg16(DX);
L0CA7:
    /* 0CA7  mov     word ptr [si+4],dx */
    ww(pDS, SI + 0x4, DX);
L0CAA:
    /* 0CAA  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_CAB  (+CAB)
       Camera type 0Ah. In: SI = the view description after its type word: BP the record looked
       at, a count, then the positions. The eye is the position nearest to BP (seg004_D70), copied
       to 26BAh. The words after that position: 0 continues as camera type 4 (seg004_CF4, from its
       zoom word); otherwise the next word is the zoom (2726h), or when it is 0 the zoom is the
       word after it divided by the distance (seg019's square root of seg004_D70's squared
       distance). Then it looks at BP (seg004_E88). */
L0CAB: /* _seg004_CAB */
    /* 0CAB  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0CAC:
    /* 0CAC  mov     bp,ax */
    BP = AX;
L0CAE:
    /* 0CAE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0CAF:
    /* 0CAF  mov     cx,ax */
    CX = AX;
L0CB1:
    /* 0CB1  call    _seg004_D70 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x0D70), 0x0CB4)) != 0) return c;
L0CB4:
    /* 0CB4  mov     di,EYE_X */
    DI = 0x26BA;
L0CB7:
    /* 0CB7  mov     cx,6 */
    CX = 0x6;
L0CBA:
    /* 0CBA  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0CBC:
    /* 0CBC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0CBD:
    /* 0CBD  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L0CBF:
    /* 0CBF  je      short L0CFC */
    if (ZF) goto L0CFC;
L0CC1:
    /* 0CC1  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0CC2:
    /* 0CC2  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L0CC4:
    /* 0CC4  jne     short L0CEE */
    if (!ZF) goto L0CEE;
L0CC6:
    /* 0CC6  mov     bx,word ptr ds:[182h] */
    BX = rw(pDS, 0x182);
L0CCA:
    /* 0CCA  mov     cx,word ptr ds:[184h] */
    CX = rw(pDS, 0x184);
L0CCE: /* L0CCE */
    /* 0CCE  call    far ptr _SquareRoot_seg019_A30 */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x0A30), 0x06E7 + PORT_LOAD_SEG, 0x0CD3)) != 0) return c;
L0CD3:
    /* 0CD3  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0CD4:
    /* 0CD4  mov     dx,ax */
    DX = AX;
L0CD6:
    /* 0CD6  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L0CD8:
    /* 0CD8  cmp     di,dx */
    sub16(DI, DX, 0);
L0CDA:
    /* 0CDA  jle     short L0CE4 */
    if (ZF || SF != OF) goto L0CE4;
L0CDC:
    /* 0CDC  sar     dx,1 */
    DX = sar16(DX, 1);
L0CDE:
    /* 0CDE  rcr     ax,1 */
    AX = rcr16(AX, 1);
L0CE0:
    /* 0CE0  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x06E7, 0x0CE0, 2)) != 0) return c;
L0CE2:
    /* 0CE2  jmp     short L0CEE */
    goto L0CEE;
L0CE4: /* L0CE4 */
    /* 0CE4  xchg    dx,di */
    { uint16_t t_ = DI;
    DI = DX;
    DX = t_; }
L0CE6:
    /* 0CE6  sar     dx,1 */
    DX = sar16(DX, 1);
L0CE8:
    /* 0CE8  rcr     ax,1 */
    AX = rcr16(AX, 1);
L0CEA:
    /* 0CEA  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x06E7, 0x0CEA, 2)) != 0) return c;
L0CEC:
    /* 0CEC  neg     ax */
    AX = (uint16_t)-AX;
L0CEE: /* L0CEE */
    /* 0CEE  mov     word ptr ds:[2726h],ax */
    ww(pDS, 0x2726, AX);
L0CF1:
    /* 0CF1  jmp     _seg004_E88 */
    goto L0E88;

    /* seg004_CF4  (+CF4)
       Camera type 4: a fixed eye. Copies the eye position (12 bytes at SI) to 26BAh and the zoom
       word to 2726h. When the next word is nonzero the nine matrix words that follow are the view
       matrix (copied to 1602h); otherwise the view looks along the vector that follows
       (seg004_EFE), the flag word is set to -1 and the matrix is stored after it, so that later
       frames copy it. */
L0CF4: /* _seg004_CF4 */
    /* 0CF4  mov     di,EYE_X */
    DI = 0x26BA;
L0CF7:
    /* 0CF7  mov     cx,6 */
    CX = 0x6;
L0CFA:
    /* 0CFA  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0CFC: /* L0CFC */
    /* 0CFC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0CFD:
    /* 0CFD  mov     word ptr ds:[2726h],ax */
    ww(pDS, 0x2726, AX);
L0D00:
    /* 0D00  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0D01:
    /* 0D01  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L0D03:
    /* 0D03  je      short L0D0E */
    if (ZF) goto L0D0E;
L0D05:
    /* 0D05  mov     di,MAT_XX */
    DI = 0x1602;
L0D08:
    /* 0D08  mov     cx,9 */
    CX = 0x9;
L0D0B:
    /* 0D0B  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0D0D:
    /* 0D0D  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0D0E: /* L0D0E */
    /* 0D0E  push    si */
    push16(SI);
L0D0F:
    /* 0D0F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0D10:
    /* 0D10  mov     word ptr ds:[15Eh],ax */
    ww(pDS, 0x15E, AX);
L0D13:
    /* 0D13  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0D14:
    /* 0D14  mov     word ptr ds:[162h],ax */
    ww(pDS, 0x162, AX);
L0D17:
    /* 0D17  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0D18:
    /* 0D18  mov     word ptr ds:[166h],ax */
    ww(pDS, 0x166, AX);
L0D1B:
    /* 0D1B  call    _seg004_EFE */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x0EFE), 0x0D1E)) != 0) return c;
L0D1E:
    /* 0D1E  pop     di */
    DI = pop16();
L0D1F:
    /* 0D1F  mov     word ptr [di-2],0FFFFh */
    ww(pDS, DI + 0xFFFE, 0xFFFF);
L0D24:
    /* 0D24  mov     si,MAT_XX */
    SI = 0x1602;
L0D27:
    /* 0D27  mov     cx,9 */
    CX = 0x9;
L0D2A:
    /* 0D2A  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0D2C:
    /* 0D2C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_D2D  (+D2D)
       Camera type 8. In: SI = the description after its type word: BP the record looked at, the
       offset of the eye record in SS (seg063), the zoom. The eye position is copied from the eye
       record (15Ch is set to it - 0Ah), the zoom is the word, or when it is 0 worked out from the
       distance to BP as seg004_CAB does (7ED2h when seg004_DBA finds it out of range), and it
       looks at BP (seg004_E88). */
L0D2D: /* _seg004_D2D */
    /* 0D2D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0D2E:
    /* 0D2E  mov     bp,ax */
    BP = AX;
L0D30:
    /* 0D30  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0D31:
    /* 0D31  push    si */
    push16(SI);
L0D32:
    /* 0D32  mov     si,ax */
    SI = AX;
L0D34:
    /* 0D34  sub     ax,0Ah */
    AX = (uint16_t)(AX - 0xA);
L0D37:
    /* 0D37  mov     word ptr ds:[15Ch],ax */
    ww(pDS, 0x15C, AX);
L0D3A:
    /* 0D3A  mov     di,EYE_X */
    DI = 0x26BA;
L0D3D:
    /* 0D3D  mov     ax,ss */
    AX = asm_ss;
L0D3F:
    /* 0D3F  mov     ds,ax */
    SET_DS(AX);
L0D41:
    /* 0D41  mov     cx,6 */
    CX = 0x6;
L0D44:
    /* 0D44  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0D46:
    /* 0D46  mov     ax,es */
    AX = asm_es;
L0D48:
    /* 0D48  mov     ds,ax */
    SET_DS(AX);
L0D4A:
    /* 0D4A  pop     si */
    SI = pop16();
L0D4B:
    /* 0D4B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0D4C:
    /* 0D4C  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L0D4E:
    /* 0D4E  jne     short L0D6A */
    if (!ZF) goto L0D6A;
L0D50:
    /* 0D50  push    si */
    push16(SI);
L0D51:
    /* 0D51  mov     si,EYE_X */
    SI = 0x26BA;
L0D54:
    /* 0D54  call    _seg004_DBA */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x0DBA), 0x0D57)) != 0) return c;
L0D57:
    /* 0D57  pop     si */
    SI = pop16();
L0D58:
    /* 0D58  jb      short L0D61 */
    if (CF) goto L0D61;
L0D5A:
    /* 0D5A  mov     cx,bx */
    CX = BX;
L0D5C:
    /* 0D5C  mov     bx,di */
    BX = DI;
L0D5E:
    /* 0D5E  jmp     L0CCE */
    goto L0CCE;
L0D61: /* L0D61 */
    /* 0D61  mov     word ptr ds:[2726h],7ED2h */
    ww(pDS, 0x2726, 0x7ED2);
L0D67:
    /* 0D67  jmp     _seg004_E88 */
    goto L0E88;
L0D6A: /* L0D6A */
    /* 0D6A  mov     word ptr ds:[2726h],ax */
    ww(pDS, 0x2726, AX);
L0D6D:
    /* 0D6D  jmp     _seg004_E88 */
    goto L0E88;

    /* seg004_D70  (+D70)
       Finds, among CX positions from SI (12 bytes each, and records 12h or 24h bytes long: 24h when
       the word at +0Ch is zero), the one nearest to the point at BP. Out: SI = that position, 180h
       the same, 182h/184h its squared distance (from seg004_DBA; 7FFFFFFFh when none is in range). */
L0D70: /* _seg004_D70 */
    /* 0D70  mov     word ptr ds:[186h],cx */
    ww(pDS, 0x186, CX);
L0D74:
    /* 0D74  mov     ax,0FFFFh */
    AX = 0xFFFF;
L0D77:
    /* 0D77  mov     word ptr ds:[182h],ax */
    ww(pDS, 0x182, AX);
L0D7A:
    /* 0D7A  mov     ax,7FFFh */
    AX = 0x7FFF;
L0D7D:
    /* 0D7D  mov     word ptr ds:[184h],ax */
    ww(pDS, 0x184, AX);
L0D80:
    /* 0D80  mov     word ptr ds:[180h],si */
    ww(pDS, 0x180, SI);
L0D84: /* L0D84 */
    /* 0D84  call    _seg004_DBA */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x0DBA), 0x0D87)) != 0) return c;
L0D87:
    /* 0D87  jb      short L0DA2 */
    if (CF) goto L0DA2;
L0D89:
    /* 0D89  mov     ax,word ptr ds:[182h] */
    AX = rw(pDS, 0x182);
L0D8C:
    /* 0D8C  mov     dx,word ptr ds:[184h] */
    DX = rw(pDS, 0x184);
L0D90:
    /* 0D90  sub     ax,di */
    AX = sub16(AX, DI, 0);
L0D92:
    /* 0D92  sbb     dx,bx */
    DX = sub16(DX, BX, CF);
L0D94:
    /* 0D94  js      short L0DA2 */
    if (SF) goto L0DA2;
L0D96:
    /* 0D96  mov     word ptr ds:[182h],di */
    ww(pDS, 0x182, DI);
L0D9A:
    /* 0D9A  mov     word ptr ds:[184h],bx */
    ww(pDS, 0x184, BX);
L0D9E:
    /* 0D9E  mov     word ptr ds:[180h],si */
    ww(pDS, 0x180, SI);
L0DA2: /* L0DA2 */
    /* 0DA2  test    word ptr [si+0Ch],0FFFFh */
    logic16((uint16_t)(rw(pDS, SI + 0xC) & 0xFFFF));
L0DA7:
    /* 0DA7  jne     short L0DAC */
    if (!ZF) goto L0DAC;
L0DA9:
    /* 0DA9  add     si,12h */
    SI = (uint16_t)(SI + 0x12);
L0DAC: /* L0DAC */
    /* 0DAC  add     si,12h */
    SI = add16(SI, 0x12, 0);
L0DAF:
    /* 0DAF  dec     word ptr ds:[186h] */
    ww(pDS, 0x186, dec16(rw(pDS, 0x186)));
L0DB3:
    /* 0DB3  jne     L0D84 */
    if (!ZF) goto L0D84;
L0DB5:
    /* 0DB5  mov     si,word ptr ds:[180h] */
    SI = rw(pDS, 0x180);
L0DB9:
    /* 0DB9  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_DBA  (+DBA)
       The squared distance between the points at SI and BP (three 32-bit coordinates each).
       Out: CF clear and BX:DI = the sum of the squares when every difference fits in 16 bits,
       else CF set. */
L0DBA: /* _seg004_DBA */
    /* 0DBA  mov     ax,word ptr [si] */
    AX = rw(pDS, SI);
L0DBC:
    /* 0DBC  sub     ax,word ptr [bp] */
    AX = sub16(AX, rw(pSS, BP), 0);
L0DBF:
    /* 0DBF  mov     cx,word ptr [si+2] */
    CX = rw(pDS, SI + 0x2);
L0DC2:
    /* 0DC2  sbb     cx,word ptr [bp+2] */
    CX = (uint16_t)(CX - rw(pSS, BP + 0x2) - CF);
L0DC5:
    /* 0DC5  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0DC6:
    /* 0DC6  cmp     dx,cx */
    sub16(DX, CX, 0);
L0DC8:
    /* 0DC8  jne     short L0E00 */
    if (!ZF) goto L0E00;
L0DCA:
    /* 0DCA  imul    ax */
    imul16(AX);
L0DCC:
    /* 0DCC  mov     di,ax */
    DI = AX;
L0DCE:
    /* 0DCE  mov     bx,dx */
    BX = DX;
L0DD0:
    /* 0DD0  mov     ax,word ptr [si+4] */
    AX = rw(pDS, SI + 0x4);
L0DD3:
    /* 0DD3  sub     ax,word ptr [bp+4] */
    AX = sub16(AX, rw(pSS, BP + 0x4), 0);
L0DD6:
    /* 0DD6  mov     cx,word ptr [si+6] */
    CX = rw(pDS, SI + 0x6);
L0DD9:
    /* 0DD9  sbb     cx,word ptr [bp+6] */
    CX = (uint16_t)(CX - rw(pSS, BP + 0x6) - CF);
L0DDC:
    /* 0DDC  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0DDD:
    /* 0DDD  cmp     dx,cx */
    sub16(DX, CX, 0);
L0DDF:
    /* 0DDF  jne     short L0E00 */
    if (!ZF) goto L0E00;
L0DE1:
    /* 0DE1  imul    ax */
    imul16(AX);
L0DE3:
    /* 0DE3  add     di,ax */
    DI = add16(DI, AX, 0);
L0DE5:
    /* 0DE5  adc     bx,dx */
    BX = (uint16_t)(BX + DX + CF);
L0DE7:
    /* 0DE7  mov     ax,word ptr [si+8] */
    AX = rw(pDS, SI + 0x8);
L0DEA:
    /* 0DEA  sub     ax,word ptr [bp+8] */
    AX = sub16(AX, rw(pSS, BP + 0x8), 0);
L0DED:
    /* 0DED  mov     cx,word ptr [si+0Ah] */
    CX = rw(pDS, SI + 0xA);
L0DF0:
    /* 0DF0  sbb     cx,word ptr [bp+0Ah] */
    CX = (uint16_t)(CX - rw(pSS, BP + 0xA) - CF);
L0DF3:
    /* 0DF3  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0DF4:
    /* 0DF4  cmp     dx,cx */
    sub16(DX, CX, 0);
L0DF6:
    /* 0DF6  jne     short L0E00 */
    if (!ZF) goto L0E00;
L0DF8:
    /* 0DF8  imul    ax */
    imul16(AX);
L0DFA:
    /* 0DFA  add     di,ax */
    DI = add16(DI, AX, 0);
L0DFC:
    /* 0DFC  adc     bx,dx */
    BX = add16(BX, DX, CF);
L0DFE:
    /* 0DFE  clc */
    CF = 0;
L0DFF:
    /* 0DFF  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0E00: /* L0E00 */
    /* 0E00  stc */
    CF = 1;
L0E01:
    /* 0E01  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_E02  (+E02)
       Camera type 2. In: SI = the description after its type word: BP the record looked at, the
       offset of the eye record in SS. Takes the eye position from the eye record and looks at BP
       (seg004_E88), then moves the eye back by 5DCh along the view's x and z and up or down by
       64h by the sign of 160Ch. Then it makes the description at 0C8h current (104h; its type
       word is 4) and writes after the type word the eye, the word 1, the view matrix and 7ED2h,
       and sets the zoom to 7ED2h. seg004_CF4 reads a type 4 description as eye, zoom, flag and
       matrix, which is one word off this layout; not checked in play. */
L0E02: /* _seg004_E02 */
    /* 0E02  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0E03:
    /* 0E03  mov     bp,ax */
    BP = AX;
L0E05:
    /* 0E05  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0E06:
    /* 0E06  mov     si,ax */
    SI = AX;
L0E08:
    /* 0E08  mov     ax,ss */
    AX = asm_ss;
L0E0A:
    /* 0E0A  mov     ds,ax */
    SET_DS(AX);
L0E0C:
    /* 0E0C  mov     di,EYE_X */
    DI = 0x26BA;
L0E0F:
    /* 0E0F  mov     cx,6 */
    CX = 0x6;
L0E12:
    /* 0E12  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0E14:
    /* 0E14  mov     ax,es */
    AX = asm_es;
L0E16:
    /* 0E16  mov     ds,ax */
    SET_DS(AX);
L0E18:
    /* 0E18  call    _seg004_E88 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x0E88), 0x0E1B)) != 0) return c;
L0E1B:
    /* 0E1B  mov     ax,5DCh */
    AX = 0x5DC;
L0E1E:
    /* 0E1E  imul    word ptr ds:[MAT_XZ] */
    imul16(rw(pDS, 0x1606));
L0E22:
    /* 0E22  add     ax,ax */
    AX = add16(AX, AX, 0);
L0E24:
    /* 0E24  adc     dx,dx */
    DX = (uint16_t)(DX + DX + CF);
L0E26:
    /* 0E26  mov     ax,dx */
    AX = DX;
L0E28:
    /* 0E28  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0E29:
    /* 0E29  sub     word ptr ds:[EYE_X],ax */
    ww(pDS, 0x26BA, sub16(rw(pDS, 0x26BA), AX, 0));
L0E2D:
    /* 0E2D  sbb     word ptr ds:[EYE_X+2],dx */
    ww(pDS, 0x26BC, (uint16_t)(rw(pDS, 0x26BC) - DX - CF));
L0E31:
    /* 0E31  mov     ax,5DCh */
    AX = 0x5DC;
L0E34:
    /* 0E34  imul    word ptr ds:[MAT_ZZ] */
    imul16(rw(pDS, 0x1612));
L0E38:
    /* 0E38  add     ax,ax */
    AX = add16(AX, AX, 0);
L0E3A:
    /* 0E3A  adc     dx,dx */
    DX = (uint16_t)(DX + DX + CF);
L0E3C:
    /* 0E3C  mov     ax,dx */
    AX = DX;
L0E3E:
    /* 0E3E  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0E3F:
    /* 0E3F  sub     word ptr ds:[EYE_Z],ax */
    ww(pDS, 0x26C2, sub16(rw(pDS, 0x26C2), AX, 0));
L0E43:
    /* 0E43  sbb     word ptr ds:[EYE_Z+2],dx */
    ww(pDS, 0x26C4, (uint16_t)(rw(pDS, 0x26C4) - DX - CF));
L0E47:
    /* 0E47  mov     ax,word ptr ds:[MAT_YZ] */
    AX = rw(pDS, 0x160C);
L0E4A:
    /* 0E4A  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0E4B:
    /* 0E4B  mov     ax,64h */
    AX = 0x64;
L0E4E:
    /* 0E4E  xor     ax,dx */
    AX = (uint16_t)(AX ^ DX);
L0E50:
    /* 0E50  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L0E52:
    /* 0E52  add     ax,word ptr ds:[EYE_Y] */
    AX = add16(AX, rw(pDS, 0x26BE), 0);
L0E56:
    /* 0E56  adc     dx,word ptr ds:[EYE_Y+2] */
    DX = (uint16_t)(DX + rw(pDS, 0x26C0) + CF);
L0E5A:
    /* 0E5A  mov     word ptr ds:[EYE_Y],ax */
    ww(pDS, 0x26BE, AX);
L0E5D:
    /* 0E5D  mov     word ptr ds:[EYE_Y+2],dx */
    ww(pDS, 0x26C0, DX);
L0E61:
    /* 0E61  mov     di,0C8h */
    DI = 0xC8;
L0E64:
    /* 0E64  mov     word ptr ds:[104h],di */
    ww(pDS, 0x104, DI);
L0E68:
    /* 0E68  add     di,2 */
    DI = add16(DI, 0x2, 0);
L0E6B:
    /* 0E6B  mov     si,EYE_X */
    SI = 0x26BA;
L0E6E:
    /* 0E6E  mov     cx,6 */
    CX = 0x6;
L0E71:
    /* 0E71  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0E73:
    /* 0E73  mov     ax,1 */
    AX = 0x1;
L0E76:
    /* 0E76  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0E77:
    /* 0E77  mov     si,MAT_XX */
    SI = 0x1602;
L0E7A:
    /* 0E7A  mov     cx,9 */
    CX = 0x9;
L0E7D:
    /* 0E7D  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0E7F:
    /* 0E7F  mov     ax,7ED2h */
    AX = 0x7ED2;
L0E82:
    /* 0E82  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0E83:
    /* 0E83  mov     word ptr ds:[2726h],ax */
    ww(pDS, 0x2726, AX);
L0E86:
    /* 0E86  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_E87  (+E87)
       Camera type 6: does nothing. */
L0E87: /* _seg004_E87 */
    /* 0E87  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_E88  (+E88)
       Aims the view at the point BP points at (three 32-bit coordinates): the difference from the
       eye (26BAh..) is halved until each part fits in 16 bits, stored at 15Eh, 162h, 166h, and
       the matrix is built from it (seg004_EFE). */
L0E88: /* _seg004_E88 */
    /* 0E88  mov     ax,word ptr [bp] */
    AX = rw(pSS, BP);
L0E8B:
    /* 0E8B  sub     ax,word ptr ds:[EYE_X] */
    AX = sub16(AX, rw(pDS, 0x26BA), 0);
L0E8F:
    /* 0E8F  mov     word ptr ds:[15Eh],ax */
    ww(pDS, 0x15E, AX);
L0E92:
    /* 0E92  mov     ax,word ptr [bp+2] */
    AX = rw(pSS, BP + 0x2);
L0E95:
    /* 0E95  sbb     ax,word ptr ds:[EYE_X+2] */
    AX = (uint16_t)(AX - rw(pDS, 0x26BC) - CF);
L0E99:
    /* 0E99  mov     word ptr ds:[160h],ax */
    ww(pDS, 0x160, AX);
L0E9C:
    /* 0E9C  mov     bx,word ptr [bp+4] */
    BX = rw(pSS, BP + 0x4);
L0E9F:
    /* 0E9F  sub     bx,word ptr ds:[EYE_Y] */
    BX = sub16(BX, rw(pDS, 0x26BE), 0);
L0EA3:
    /* 0EA3  mov     word ptr ds:[162h],bx */
    ww(pDS, 0x162, BX);
L0EA7:
    /* 0EA7  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L0EAA:
    /* 0EAA  sbb     ax,word ptr ds:[EYE_Y+2] */
    AX = (uint16_t)(AX - rw(pDS, 0x26C0) - CF);
L0EAE:
    /* 0EAE  mov     word ptr ds:[164h],ax */
    ww(pDS, 0x164, AX);
L0EB1:
    /* 0EB1  mov     cx,word ptr [bp+8] */
    CX = rw(pSS, BP + 0x8);
L0EB4:
    /* 0EB4  sub     cx,word ptr ds:[EYE_Z] */
    CX = sub16(CX, rw(pDS, 0x26C2), 0);
L0EB8:
    /* 0EB8  mov     word ptr ds:[166h],cx */
    ww(pDS, 0x166, CX);
L0EBC:
    /* 0EBC  mov     ax,word ptr [bp+0Ah] */
    AX = rw(pSS, BP + 0xA);
L0EBF:
    /* 0EBF  sbb     ax,word ptr ds:[EYE_Z+2] */
    AX = (uint16_t)(AX - rw(pDS, 0x26C4) - CF);
L0EC3:
    /* 0EC3  mov     word ptr ds:[168h],ax */
    ww(pDS, 0x168, AX);
L0EC6: /* L0EC6 */
    /* 0EC6  mov     ax,word ptr ds:[15Eh] */
    AX = rw(pDS, 0x15E);
L0EC9:
    /* 0EC9  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0ECA:
    /* 0ECA  cmp     dx,word ptr ds:[160h] */
    sub16(DX, rw(pDS, 0x160), 0);
L0ECE:
    /* 0ECE  jne     short L0EE4 */
    if (!ZF) goto L0EE4;
L0ED0:
    /* 0ED0  mov     ax,word ptr ds:[162h] */
    AX = rw(pDS, 0x162);
L0ED3:
    /* 0ED3  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0ED4:
    /* 0ED4  cmp     dx,word ptr ds:[164h] */
    sub16(DX, rw(pDS, 0x164), 0);
L0ED8:
    /* 0ED8  jne     short L0EE4 */
    if (!ZF) goto L0EE4;
L0EDA:
    /* 0EDA  mov     ax,word ptr ds:[166h] */
    AX = rw(pDS, 0x166);
L0EDD:
    /* 0EDD  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0EDE:
    /* 0EDE  cmp     dx,word ptr ds:[168h] */
    sub16(DX, rw(pDS, 0x168), 0);
L0EE2:
    /* 0EE2  je      short _seg004_EFE */
    if (ZF) goto L0EFE;
L0EE4: /* L0EE4 */
    /* 0EE4  sar     word ptr ds:[160h],1 */
    ww(pDS, 0x160, sar16(rw(pDS, 0x160), 1));
L0EE8:
    /* 0EE8  rcr     word ptr ds:[15Eh],1 */
    ww(pDS, 0x15E, rcr16(rw(pDS, 0x15E), 1));
L0EEC:
    /* 0EEC  sar     word ptr ds:[164h],1 */
    ww(pDS, 0x164, sar16(rw(pDS, 0x164), 1));
L0EF0:
    /* 0EF0  rcr     word ptr ds:[162h],1 */
    ww(pDS, 0x162, rcr16(rw(pDS, 0x162), 1));
L0EF4:
    /* 0EF4  sar     word ptr ds:[168h],1 */
    ww(pDS, 0x168, sar16(rw(pDS, 0x168), 1));
L0EF8:
    /* 0EF8  rcr     word ptr ds:[166h],1 */
    ww(pDS, 0x166, rcr16(rw(pDS, 0x166), 1));
L0EFC:
    /* 0EFC  jmp     L0EC6 */
    goto L0EC6;

    /* seg004_EFE  (+EFE)
       Builds the view matrix (1602h..1612h, 1.15) that looks along the vector at 15Eh, 162h, 166h
       (x, y, z): the third row is the vector divided by its length (seg004_103A), the first
       the horizontal unit vector at right angles to it (from x and z; or from y alone when x and z
       are 0), the second their cross product. 8000h is changed to 8001h throughout. Installs
       overflow_handler_reg for the divides. */
L0EFE: /* _seg004_EFE */
    /* 0EFE  mov     word ptr ss:[4D5h],offset _overflow_handler_reg */
    ww(pSS, 0x4D5, 0x5BD1);
L0F05:
    /* 0F05  mov     ax,word ptr ds:[15Eh] */
    AX = rw(pDS, 0x15E);
L0F08:
    /* 0F08  mov     bx,word ptr ds:[162h] */
    BX = rw(pDS, 0x162);
L0F0C:
    /* 0F0C  mov     cx,word ptr ds:[166h] */
    CX = rw(pDS, 0x166);
L0F10:
    /* 0F10  call    _seg004_103A */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x103A), 0x0F13)) != 0) return c;
L0F13:
    /* 0F13  mov     dx,word ptr ds:[15Eh] */
    DX = rw(pDS, 0x15E);
L0F17:
    /* 0F17  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L0F19:
    /* 0F19  sar     dx,1 */
    DX = sar16(DX, 1);
L0F1B:
    /* 0F1B  rcr     ax,1 */
    AX = rcr16(AX, 1);
L0F1D:
    /* 0F1D  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x06E7, 0x0F1D, 2)) != 0) return c;
L0F1F:
    /* 0F1F  cmp     ax,8000h */
    sub16(AX, 0x8000, 0);
L0F22:
    /* 0F22  jne     short L0F27 */
    if (!ZF) goto L0F27;
L0F24:
    /* 0F24  mov     ax,8001h */
    AX = 0x8001;
L0F27: /* L0F27 */
    /* 0F27  mov     word ptr ds:[MAT_XZ],ax */
    ww(pDS, 0x1606, AX);
L0F2A:
    /* 0F2A  mov     dx,word ptr ds:[162h] */
    DX = rw(pDS, 0x162);
L0F2E:
    /* 0F2E  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L0F30:
    /* 0F30  sar     dx,1 */
    DX = sar16(DX, 1);
L0F32:
    /* 0F32  rcr     ax,1 */
    AX = rcr16(AX, 1);
L0F34:
    /* 0F34  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x06E7, 0x0F34, 2)) != 0) return c;
L0F36:
    /* 0F36  cmp     ax,8000h */
    sub16(AX, 0x8000, 0);
L0F39:
    /* 0F39  jne     short L0F3E */
    if (!ZF) goto L0F3E;
L0F3B:
    /* 0F3B  mov     ax,8001h */
    AX = 0x8001;
L0F3E: /* L0F3E */
    /* 0F3E  mov     word ptr ds:[MAT_YZ],ax */
    ww(pDS, 0x160C, AX);
L0F41:
    /* 0F41  mov     dx,word ptr ds:[166h] */
    DX = rw(pDS, 0x166);
L0F45:
    /* 0F45  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L0F47:
    /* 0F47  sar     dx,1 */
    DX = sar16(DX, 1);
L0F49:
    /* 0F49  rcr     ax,1 */
    AX = rcr16(AX, 1);
L0F4B:
    /* 0F4B  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x06E7, 0x0F4B, 2)) != 0) return c;
L0F4D:
    /* 0F4D  cmp     ax,8000h */
    sub16(AX, 0x8000, 0);
L0F50:
    /* 0F50  jne     short L0F55 */
    if (!ZF) goto L0F55;
L0F52:
    /* 0F52  mov     ax,8001h */
    AX = 0x8001;
L0F55: /* L0F55 */
    /* 0F55  mov     word ptr ds:[MAT_ZZ],ax */
    ww(pDS, 0x1612, AX);
L0F58:
    /* 0F58  mov     ax,word ptr ds:[15Eh] */
    AX = rw(pDS, 0x15E);
L0F5B:
    /* 0F5B  or      ax,word ptr ds:[166h] */
    AX = logic16((uint16_t)(AX | rw(pDS, 0x166)));
L0F5F:
    /* 0F5F  je      short L0FAD */
    if (ZF) goto L0FAD;
L0F61:
    /* 0F61  mov     ax,word ptr ds:[15Eh] */
    AX = rw(pDS, 0x15E);
L0F64:
    /* 0F64  imul    ax */
    imul16(AX);
L0F66:
    /* 0F66  mov     cx,dx */
    CX = DX;
L0F68:
    /* 0F68  mov     bx,ax */
    BX = AX;
L0F6A:
    /* 0F6A  mov     ax,word ptr ds:[166h] */
    AX = rw(pDS, 0x166);
L0F6D:
    /* 0F6D  imul    ax */
    imul16(AX);
L0F6F:
    /* 0F6F  add     bx,ax */
    BX = add16(BX, AX, 0);
L0F71:
    /* 0F71  adc     cx,dx */
    CX = (uint16_t)(CX + DX + CF);
L0F73:
    /* 0F73  call    far ptr _SquareRoot_seg019_A30 */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x0A30), 0x06E7 + PORT_LOAD_SEG, 0x0F78)) != 0) return c;
L0F78:
    /* 0F78  mov     dx,word ptr ds:[166h] */
    DX = rw(pDS, 0x166);
L0F7C:
    /* 0F7C  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L0F7E:
    /* 0F7E  mov     word ptr ds:[MAT_YX],ax */
    ww(pDS, 0x1608, AX);
L0F81:
    /* 0F81  sar     dx,1 */
    DX = sar16(DX, 1);
L0F83:
    /* 0F83  rcr     ax,1 */
    AX = rcr16(AX, 1);
L0F85:
    /* 0F85  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x06E7, 0x0F85, 2)) != 0) return c;
L0F87:
    /* 0F87  cmp     ax,8000h */
    sub16(AX, 0x8000, 0);
L0F8A:
    /* 0F8A  jne     short L0F8F */
    if (!ZF) goto L0F8F;
L0F8C:
    /* 0F8C  mov     ax,8001h */
    AX = 0x8001;
L0F8F: /* L0F8F */
    /* 0F8F  mov     word ptr ds:[MAT_XX],ax */
    ww(pDS, 0x1602, AX);
L0F92:
    /* 0F92  mov     dx,word ptr ds:[15Eh] */
    DX = rw(pDS, 0x15E);
L0F96:
    /* 0F96  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L0F98:
    /* 0F98  sar     dx,1 */
    DX = sar16(DX, 1);
L0F9A:
    /* 0F9A  rcr     ax,1 */
    AX = rcr16(AX, 1);
L0F9C:
    /* 0F9C  idiv    di */
    if (asm_idiv16(DI) && (c = asm_divfault(0x06E7, 0x0F9C, 2)) != 0) return c;
L0F9E:
    /* 0F9E  cmp     ax,8000h */
    sub16(AX, 0x8000, 0);
L0FA1:
    /* 0FA1  jne     short L0FA6 */
    if (!ZF) goto L0FA6;
L0FA3:
    /* 0FA3  mov     ax,8001h */
    AX = 0x8001;
L0FA6: /* L0FA6 */
    /* 0FA6  neg     ax */
    AX = (uint16_t)-AX;
L0FA8:
    /* 0FA8  mov     word ptr ds:[MAT_ZX],ax */
    ww(pDS, 0x160E, AX);
L0FAB:
    /* 0FAB  jmp     short L0FBD */
    goto L0FBD;
L0FAD: /* L0FAD */
    /* 0FAD  mov     ax,word ptr ds:[MAT_YZ] */
    AX = rw(pDS, 0x160C);
L0FB0:
    /* 0FB0  neg     ax */
    AX = (uint16_t)-AX;
L0FB2:
    /* 0FB2  mov     word ptr ds:[MAT_XX],ax */
    ww(pDS, 0x1602, AX);
L0FB5:
    /* 0FB5  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L0FB7:
    /* 0FB7  mov     word ptr ds:[MAT_YX],ax */
    ww(pDS, 0x1608, AX);
L0FBA:
    /* 0FBA  mov     word ptr ds:[MAT_ZX],ax */
    ww(pDS, 0x160E, AX);
L0FBD: /* L0FBD */
    /* 0FBD  mov     ax,word ptr ds:[MAT_YX] */
    AX = rw(pDS, 0x1608);
L0FC0:
    /* 0FC0  imul    word ptr ds:[MAT_ZZ] */
    imul16(rw(pDS, 0x1612));
L0FC4:
    /* 0FC4  mov     bx,ax */
    BX = AX;
L0FC6:
    /* 0FC6  mov     cx,dx */
    CX = DX;
L0FC8:
    /* 0FC8  mov     ax,word ptr ds:[MAT_ZX] */
    AX = rw(pDS, 0x160E);
L0FCB:
    /* 0FCB  imul    word ptr ds:[MAT_YZ] */
    imul16(rw(pDS, 0x160C));
L0FCF:
    /* 0FCF  sub     ax,bx */
    AX = sub16(AX, BX, 0);
L0FD1:
    /* 0FD1  sbb     dx,cx */
    DX = (uint16_t)(DX - CX - CF);
L0FD3:
    /* 0FD3  shl     ax,1 */
    AX = shl16(AX, 1);
L0FD5:
    /* 0FD5  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0FD7:
    /* 0FD7  cmp     dx,8000h */
    sub16(DX, 0x8000, 0);
L0FDB:
    /* 0FDB  jne     short L0FE0 */
    if (!ZF) goto L0FE0;
L0FDD:
    /* 0FDD  mov     dx,8001h */
    DX = 0x8001;
L0FE0: /* L0FE0 */
    /* 0FE0  mov     word ptr ds:[MAT_XY],dx */
    ww(pDS, 0x1604, DX);
L0FE4:
    /* 0FE4  mov     ax,word ptr ds:[MAT_XZ] */
    AX = rw(pDS, 0x1606);
L0FE7:
    /* 0FE7  imul    word ptr ds:[MAT_ZX] */
    imul16(rw(pDS, 0x160E));
L0FEB:
    /* 0FEB  mov     bx,ax */
    BX = AX;
L0FED:
    /* 0FED  mov     cx,dx */
    CX = DX;
L0FEF:
    /* 0FEF  mov     ax,word ptr ds:[MAT_XX] */
    AX = rw(pDS, 0x1602);
L0FF2:
    /* 0FF2  imul    word ptr ds:[MAT_ZZ] */
    imul16(rw(pDS, 0x1612));
L0FF6:
    /* 0FF6  sub     ax,bx */
    AX = sub16(AX, BX, 0);
L0FF8:
    /* 0FF8  sbb     dx,cx */
    DX = (uint16_t)(DX - CX - CF);
L0FFA:
    /* 0FFA  shl     ax,1 */
    AX = shl16(AX, 1);
L0FFC:
    /* 0FFC  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0FFE:
    /* 0FFE  jno     short L1005 */
    if (!OF) goto L1005;
L1000:
    /* 1000  mov     dx,7FFFh */
    DX = 0x7FFF;
L1003:
    /* 1003  jmp     short L100E */
    goto L100E;
L1005: /* L1005 */
    /* 1005  cmp     dx,8000h */
    sub16(DX, 0x8000, 0);
L1009:
    /* 1009  jne     short L100E */
    if (!ZF) goto L100E;
L100B:
    /* 100B  mov     dx,8001h */
    DX = 0x8001;
L100E: /* L100E */
    /* 100E  mov     word ptr ds:[MAT_YY],dx */
    ww(pDS, 0x160A, DX);
L1012:
    /* 1012  mov     ax,word ptr ds:[MAT_XX] */
    AX = rw(pDS, 0x1602);
L1015:
    /* 1015  imul    word ptr ds:[MAT_YZ] */
    imul16(rw(pDS, 0x160C));
L1019:
    /* 1019  mov     bx,ax */
    BX = AX;
L101B:
    /* 101B  mov     cx,dx */
    CX = DX;
L101D:
    /* 101D  mov     ax,word ptr ds:[MAT_YX] */
    AX = rw(pDS, 0x1608);
L1020:
    /* 1020  imul    word ptr ds:[MAT_XZ] */
    imul16(rw(pDS, 0x1606));
L1024:
    /* 1024  sub     ax,bx */
    AX = sub16(AX, BX, 0);
L1026:
    /* 1026  sbb     dx,cx */
    DX = (uint16_t)(DX - CX - CF);
L1028:
    /* 1028  shl     ax,1 */
    AX = shl16(AX, 1);
L102A:
    /* 102A  rcl     dx,1 */
    DX = rcl16(DX, 1);
L102C:
    /* 102C  cmp     dx,8000h */
    sub16(DX, 0x8000, 0);
L1030:
    /* 1030  jne     short L1035 */
    if (!ZF) goto L1035;
L1032:
    /* 1032  mov     dx,8001h */
    DX = 0x8001;
L1035: /* L1035 */
    /* 1035  mov     word ptr ds:[MAT_ZY],dx */
    ww(pDS, 0x1610, DX);
L1039:
    /* 1039  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_103A  (+103A)
       In: AX, BX, CX = a vector. Out: the length, from seg019's square root of the sum of the
       squares. */
L103A: /* _seg004_103A */
    /* 103A  imul    ax */
    imul16(AX);
L103C:
    /* 103C  mov     bp,dx */
    BP = DX;
L103E:
    /* 103E  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L103F:
    /* 103F  imul    ax */
    imul16(AX);
L1041:
    /* 1041  add     bx,ax */
    BX = add16(BX, AX, 0);
L1043:
    /* 1043  adc     bp,dx */
    BP = (uint16_t)(BP + DX + CF);
L1045:
    /* 1045  mov     ax,cx */
    AX = CX;
L1047:
    /* 1047  imul    ax */
    imul16(AX);
L1049:
    /* 1049  add     bx,ax */
    BX = add16(BX, AX, 0);
L104B:
    /* 104B  adc     dx,bp */
    DX = (uint16_t)(DX + BP + CF);
L104D:
    /* 104D  mov     cx,dx */
    CX = DX;
L104F:
    /* 104F  call    far ptr _SquareRoot_seg019_A30 */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x0A30), 0x06E7 + PORT_LOAD_SEG, 0x1054)) != 0) return c;
L1054:
    /* 1054  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_1055  (+1055)
       Far wrapper for angles_2_matrix; nothing in the EXE calls it. */
L1055: /* _seg004_1055 */
    /* 1055  call    _angles_2_matrix */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x1059), 0x1058)) != 0) return c;
L1058:
    /* 1058  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg004_1059  (+1059)
       In: BP (and DI) = a record whose words +12h, +14h and +16h are three angles, ES = the 3D
       data segment. Calls seg019's sincos for each negated angle (with DS = SS, as sincos needs),
       keeping the sines and cosines at es:18Ch..196h, and writes the 3x3 rotation matrix they make
       to BP+0..+10h in 1.15 fixed point. Which angle is heading, pitch and bank has not been
       checked. */
L1059: /* _angles_2_matrix */
    /* 1059  mov     ax,ss */
    AX = asm_ss;
L105B:
    /* 105B  mov     ds,ax */
    SET_DS(AX);
L105D:
    /* 105D  push    bp */
    push16(BP);
L105E:
    /* 105E  mov     di,bp */
    DI = BP;
L1060:
    /* 1060  mov     bx,word ptr [di+12h] */
    BX = rw(pDS, DI + 0x12);
L1063:
    /* 1063  neg     bx */
    BX = (uint16_t)-BX;
L1065:
    /* 1065  call    far ptr _sincos */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x0A34), 0x06E7 + PORT_LOAD_SEG, 0x106A)) != 0) return c;
L106A:
    /* 106A  mov     word ptr es:[194h],bx */
    ww(pES, 0x194, BX);
L106F:
    /* 106F  mov     word ptr es:[196h],ax */
    ww(pES, 0x196, AX);
L1073:
    /* 1073  mov     bx,word ptr [di+14h] */
    BX = rw(pDS, DI + 0x14);
L1076:
    /* 1076  neg     bx */
    BX = (uint16_t)-BX;
L1078:
    /* 1078  call    far ptr _sincos */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x0A34), 0x06E7 + PORT_LOAD_SEG, 0x107D)) != 0) return c;
L107D:
    /* 107D  mov     word ptr es:[190h],bx */
    ww(pES, 0x190, BX);
L1082:
    /* 1082  mov     word ptr es:[192h],ax */
    ww(pES, 0x192, AX);
L1086:
    /* 1086  mov     bx,word ptr [di+16h] */
    BX = rw(pDS, DI + 0x16);
L1089:
    /* 1089  neg     bx */
    BX = (uint16_t)-BX;
L108B:
    /* 108B  call    far ptr _sincos */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x0A34), 0x06E7 + PORT_LOAD_SEG, 0x1090)) != 0) return c;
L1090:
    /* 1090  mov     word ptr es:[18Ch],bx */
    ww(pES, 0x18C, BX);
L1095:
    /* 1095  mov     word ptr es:[18Eh],ax */
    ww(pES, 0x18E, AX);
L1099:
    /* 1099  pop     bp */
    BP = pop16();
L109A:
    /* 109A  mov     ax,es */
    AX = asm_es;
L109C:
    /* 109C  mov     ds,ax */
    SET_DS(AX);
L109E:
    /* 109E  mov     ax,word ptr ds:[196h] */
    AX = rw(pDS, 0x196);
L10A1:
    /* 10A1  neg     ax */
    AX = (uint16_t)-AX;
L10A3:
    /* 10A3  mov     word ptr [bp+0Ah],ax */
    ww(pSS, BP + 0xA, AX);
L10A6:
    /* 10A6  mov     ax,word ptr ds:[190h] */
    AX = rw(pDS, 0x190);
L10A9:
    /* 10A9  imul    word ptr ds:[18Ch] */
    imul16(rw(pDS, 0x18C));
L10AD:
    /* 10AD  shl     ax,1 */
    AX = shl16(AX, 1);
L10AF:
    /* 10AF  rcl     dx,1 */
    DX = rcl16(DX, 1);
L10B1:
    /* 10B1  mov     bx,dx */
    BX = DX;
L10B3:
    /* 10B3  mov     ax,word ptr ds:[192h] */
    AX = rw(pDS, 0x192);
L10B6:
    /* 10B6  imul    word ptr ds:[18Eh] */
    imul16(rw(pDS, 0x18E));
L10BA:
    /* 10BA  shl     ax,1 */
    AX = shl16(AX, 1);
L10BC:
    /* 10BC  rcl     dx,1 */
    DX = rcl16(DX, 1);
L10BE:
    /* 10BE  mov     di,dx */
    DI = DX;
L10C0:
    /* 10C0  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L10C1:
    /* 10C1  imul    word ptr ds:[196h] */
    imul16(rw(pDS, 0x196));
L10C5:
    /* 10C5  shl     ax,1 */
    AX = shl16(AX, 1);
L10C7:
    /* 10C7  rcl     dx,1 */
    DX = rcl16(DX, 1);
L10C9:
    /* 10C9  add     dx,bx */
    DX = (uint16_t)(DX + BX);
L10CB:
    /* 10CB  mov     word ptr [bp],dx */
    ww(pSS, BP, DX);
L10CE:
    /* 10CE  mov     ax,word ptr ds:[192h] */
    AX = rw(pDS, 0x192);
L10D1:
    /* 10D1  imul    word ptr ds:[194h] */
    imul16(rw(pDS, 0x194));
L10D5:
    /* 10D5  shl     ax,1 */
    AX = shl16(AX, 1);
L10D7:
    /* 10D7  rcl     dx,1 */
    DX = rcl16(DX, 1);
L10D9:
    /* 10D9  neg     dx */
    DX = (uint16_t)-DX;
L10DB:
    /* 10DB  mov     word ptr [bp+6],dx */
    ww(pSS, BP + 0x6, DX);
L10DE:
    /* 10DE  mov     ax,word ptr ds:[190h] */
    AX = rw(pDS, 0x190);
L10E1:
    /* 10E1  imul    word ptr ds:[18Eh] */
    imul16(rw(pDS, 0x18E));
L10E5:
    /* 10E5  shl     ax,1 */
    AX = shl16(AX, 1);
L10E7:
    /* 10E7  rcl     dx,1 */
    DX = rcl16(DX, 1);
L10E9:
    /* 10E9  mov     cx,dx */
    CX = DX;
L10EB:
    /* 10EB  mov     ax,word ptr ds:[192h] */
    AX = rw(pDS, 0x192);
L10EE:
    /* 10EE  imul    word ptr ds:[18Ch] */
    imul16(rw(pDS, 0x18C));
L10F2:
    /* 10F2  shl     ax,1 */
    AX = shl16(AX, 1);
L10F4:
    /* 10F4  rcl     dx,1 */
    DX = rcl16(DX, 1);
L10F6:
    /* 10F6  mov     si,dx */
    SI = DX;
L10F8:
    /* 10F8  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L10F9:
    /* 10F9  imul    word ptr ds:[196h] */
    imul16(rw(pDS, 0x196));
L10FD:
    /* 10FD  shl     ax,1 */
    AX = shl16(AX, 1);
L10FF:
    /* 10FF  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1101:
    /* 1101  sub     dx,cx */
    DX = (uint16_t)(DX - CX);
L1103:
    /* 1103  neg     dx */
    DX = (uint16_t)-DX;
L1105:
    /* 1105  mov     word ptr [bp+0Ch],dx */
    ww(pSS, BP + 0xC, DX);
L1108:
    /* 1108  mov     ax,cx */
    AX = CX;
L110A:
    /* 110A  imul    word ptr ds:[196h] */
    imul16(rw(pDS, 0x196));
L110E:
    /* 110E  shl     ax,1 */
    AX = shl16(AX, 1);
L1110:
    /* 1110  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1112:
    /* 1112  sub     si,dx */
    SI = (uint16_t)(SI - DX);
L1114:
    /* 1114  mov     word ptr [bp+2],si */
    ww(pSS, BP + 0x2, SI);
L1117:
    /* 1117  mov     ax,word ptr ds:[190h] */
    AX = rw(pDS, 0x190);
L111A:
    /* 111A  imul    word ptr ds:[194h] */
    imul16(rw(pDS, 0x194));
L111E:
    /* 111E  shl     ax,1 */
    AX = shl16(AX, 1);
L1120:
    /* 1120  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1122:
    /* 1122  mov     word ptr [bp+8],dx */
    ww(pSS, BP + 0x8, DX);
L1125:
    /* 1125  mov     ax,bx */
    AX = BX;
L1127:
    /* 1127  imul    word ptr ds:[196h] */
    imul16(rw(pDS, 0x196));
L112B:
    /* 112B  shl     ax,1 */
    AX = shl16(AX, 1);
L112D:
    /* 112D  rcl     dx,1 */
    DX = rcl16(DX, 1);
L112F:
    /* 112F  add     dx,di */
    DX = (uint16_t)(DX + DI);
L1131:
    /* 1131  mov     word ptr [bp+0Eh],dx */
    ww(pSS, BP + 0xE, DX);
L1134:
    /* 1134  mov     ax,word ptr ds:[18Eh] */
    AX = rw(pDS, 0x18E);
L1137:
    /* 1137  imul    word ptr ds:[194h] */
    imul16(rw(pDS, 0x194));
L113B:
    /* 113B  shl     ax,1 */
    AX = shl16(AX, 1);
L113D:
    /* 113D  rcl     dx,1 */
    DX = rcl16(DX, 1);
L113F:
    /* 113F  neg     dx */
    DX = (uint16_t)-DX;
L1141:
    /* 1141  mov     word ptr [bp+4],dx */
    ww(pSS, BP + 0x4, DX);
L1144:
    /* 1144  mov     ax,word ptr ds:[18Ch] */
    AX = rw(pDS, 0x18C);
L1147:
    /* 1147  imul    word ptr ds:[194h] */
    imul16(rw(pDS, 0x194));
L114B:
    /* 114B  shl     ax,1 */
    AX = shl16(AX, 1);
L114D:
    /* 114D  rcl     dx,1 */
    DX = rcl16(DX, 1);
L114F:
    /* 114F  mov     word ptr [bp+10h],dx */
    ww(pSS, BP + 0x10, DX);
L1152:
    /* 1152  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_1153  (+1153)
       Two-angle version of angles_2_matrix: angles at es:[si] and es:[si+2], matrix written to
       DI+0..+10h with element +6 zero (no roll term). Called by head_view for the eye's own
       rotation. */
L1153: /* _seg004_1153 */
    /* 1153  mov     ax,ss */
    AX = asm_ss;
L1155:
    /* 1155  mov     ds,ax */
    SET_DS(AX);
L1157:
    /* 1157  mov     bx,word ptr es:[si] */
    BX = rw(pES, SI);
L115A:
    /* 115A  neg     bx */
    BX = (uint16_t)-BX;
L115C:
    /* 115C  call    far ptr _sincos */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x0A34), 0x06E7 + PORT_LOAD_SEG, 0x1161)) != 0) return c;
L1161:
    /* 1161  mov     word ptr es:[194h],bx */
    ww(pES, 0x194, BX);
L1166:
    /* 1166  mov     word ptr es:[196h],ax */
    ww(pES, 0x196, AX);
L116A:
    /* 116A  mov     bx,word ptr es:[si+2] */
    BX = rw(pES, SI + 0x2);
L116E:
    /* 116E  neg     bx */
    BX = (uint16_t)-BX;
L1170:
    /* 1170  call    far ptr _sincos */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x0A34), 0x06E7 + PORT_LOAD_SEG, 0x1175)) != 0) return c;
L1175:
    /* 1175  mov     word ptr es:[18Ch],bx */
    ww(pES, 0x18C, BX);
L117A:
    /* 117A  mov     word ptr es:[18Eh],ax */
    ww(pES, 0x18E, AX);
L117E:
    /* 117E  mov     ax,es */
    AX = asm_es;
L1180:
    /* 1180  mov     ds,ax */
    SET_DS(AX);
L1182:
    /* 1182  mov     ax,word ptr ds:[18Ch] */
    AX = rw(pDS, 0x18C);
L1185:
    /* 1185  mov     word ptr [di],ax */
    ww(pDS, DI, AX);
L1187:
    /* 1187  mov     ax,word ptr ds:[18Eh] */
    AX = rw(pDS, 0x18E);
L118A:
    /* 118A  imul    word ptr ds:[196h] */
    imul16(rw(pDS, 0x196));
L118E:
    /* 118E  shl     ax,1 */
    AX = shl16(AX, 1);
L1190:
    /* 1190  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1192:
    /* 1192  neg     dx */
    DX = (uint16_t)-DX;
L1194:
    /* 1194  mov     word ptr [di+2],dx */
    ww(pDS, DI + 0x2, DX);
L1197:
    /* 1197  mov     ax,word ptr ds:[18Eh] */
    AX = rw(pDS, 0x18E);
L119A:
    /* 119A  imul    word ptr ds:[194h] */
    imul16(rw(pDS, 0x194));
L119E:
    /* 119E  shl     ax,1 */
    AX = shl16(AX, 1);
L11A0:
    /* 11A0  rcl     dx,1 */
    DX = rcl16(DX, 1);
L11A2:
    /* 11A2  neg     dx */
    DX = (uint16_t)-DX;
L11A4:
    /* 11A4  mov     word ptr [di+4],dx */
    ww(pDS, DI + 0x4, DX);
L11A7:
    /* 11A7  mov     word ptr [di+6],0 */
    ww(pDS, DI + 0x6, 0x0);
L11AC:
    /* 11AC  mov     ax,word ptr ds:[194h] */
    AX = rw(pDS, 0x194);
L11AF:
    /* 11AF  mov     word ptr [di+8],ax */
    ww(pDS, DI + 0x8, AX);
L11B2:
    /* 11B2  mov     ax,word ptr ds:[196h] */
    AX = rw(pDS, 0x196);
L11B5:
    /* 11B5  neg     ax */
    AX = (uint16_t)-AX;
L11B7:
    /* 11B7  mov     word ptr [di+0Ah],ax */
    ww(pDS, DI + 0xA, AX);
L11BA:
    /* 11BA  mov     ax,word ptr ds:[18Eh] */
    AX = rw(pDS, 0x18E);
L11BD:
    /* 11BD  mov     word ptr [di+0Ch],ax */
    ww(pDS, DI + 0xC, AX);
L11C0:
    /* 11C0  mov     ax,word ptr ds:[18Ch] */
    AX = rw(pDS, 0x18C);
L11C3:
    /* 11C3  imul    word ptr ds:[196h] */
    imul16(rw(pDS, 0x196));
L11C7:
    /* 11C7  shl     ax,1 */
    AX = shl16(AX, 1);
L11C9:
    /* 11C9  rcl     dx,1 */
    DX = rcl16(DX, 1);
L11CB:
    /* 11CB  mov     word ptr [di+0Eh],dx */
    ww(pDS, DI + 0xE, DX);
L11CE:
    /* 11CE  mov     ax,word ptr ds:[18Ch] */
    AX = rw(pDS, 0x18C);
L11D1:
    /* 11D1  imul    word ptr ds:[194h] */
    imul16(rw(pDS, 0x194));
L11D5:
    /* 11D5  shl     ax,1 */
    AX = shl16(AX, 1);
L11D7:
    /* 11D7  rcl     dx,1 */
    DX = rcl16(DX, 1);
L11D9:
    /* 11D9  mov     word ptr [di+10h],dx */
    ww(pDS, DI + 0x10, DX);
L11DC:
    /* 11DC  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_11DD  (+11DD)
       Far, called by nothing in the EXE. Makes the type 0Ah description at 0ECh current (104h),
       sets its first position's x and z (32-bit) from the record at ss:97Ah (seg063, where
       cPlayer, the camera record, is 0970h) and clears 106h. */
L11DD: /* _seg004_11DD */
    /* 11DD  push    ds */
    push16(asm_ds);
L11DE:
    /* 11DE  push    es */
    push16(asm_es);
L11DF:
    /* 11DF  mov     ax,seg seg051 */
    AX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L11E2:
    /* 11E2  mov     ds,ax */
    SET_DS(AX);
L11E4:
    /* 11E4  mov     es,ax */
    SET_ES(AX);
L11E6:
    /* 11E6  mov     di,0ECh */
    DI = 0xEC;
L11E9:
    /* 11E9  mov     word ptr ds:[104h],di */
    ww(pDS, 0x104, DI);
L11ED:
    /* 11ED  add     di,6 */
    DI = (uint16_t)(DI + 0x6);
L11F0:
    /* 11F0  mov     si,97Ah */
    SI = 0x97A;
L11F3:
    /* 11F3  mov     ax,ss */
    AX = asm_ss;
L11F5:
    /* 11F5  mov     ds,ax */
    SET_DS(AX);
L11F7:
    /* 11F7  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L11F8:
    /* 11F8  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L11F9:
    /* 11F9  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L11FC:
    /* 11FC  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L11FF:
    /* 11FF  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1200:
    /* 1200  movsw */
    ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2));
L1201:
    /* 1201  xor     ax,ax */
    AX = logic16((uint16_t)(AX ^ AX));
L1203:
    /* 1203  mov     word ptr es:[106h],ax */
    ww(pES, 0x106, AX);
L1207:
    /* 1207  pop     es */
    SET_ES(pop16());
L1208:
    /* 1208  pop     ds */
    SET_DS(pop16());
L1209:
    /* 1209  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg004_120A  (+120A)
       Far: a camera command. When the current description is settled (104h = 106h, no
       transition, 19Ah = 0), calls its type's handler from the table at BX (a word per type, the
       type word being the index), with SI = the description. */
L120A: /* _seg004_120A */
    /* 120A  push    ds */
    push16(asm_ds);
L120B:
    /* 120B  push    es */
    push16(asm_es);
L120C:
    /* 120C  mov     ax,seg seg051 */
    AX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L120F:
    /* 120F  mov     ds,ax */
    SET_DS(AX);
L1211:
    /* 1211  mov     es,ax */
    SET_ES(AX);
L1213:
    /* 1213  mov     si,word ptr ds:[104h] */
    SI = rw(pDS, 0x104);
L1217:
    /* 1217  cmp     si,word ptr ds:[106h] */
    sub16(SI, rw(pDS, 0x106), 0);
L121B:
    /* 121B  jne     short L1229 */
    if (!ZF) goto L1229;
L121D:
    /* 121D  test    word ptr ds:[19Ah],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x19A) & 0xFFFF));
L1223:
    /* 1223  jne     short L1229 */
    if (!ZF) goto L1229;
L1225:
    /* 1225  add     bx,word ptr [si] */
    BX = (uint16_t)(BX + rw(pDS, SI));
L1227:
    /* 1227  call    word ptr [bx] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(pDS, BX)), 0x1229)) != 0) return c;
L1229: /* L1229 */
    /* 1229  pop     es */
    SET_ES(pop16());
L122A:
    /* 122A  pop     ds */
    SET_DS(pop16());
L122B:
    /* 122B  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg004_122C  (+122C)
       Far entries for the six camera commands (handler tables at 114h, 120h, 12Ch, 138h, 144h,
       150h); nothing in the EXE calls them. Commands 0 to 3 turn head_view's camera, 4 and 5 zoom
       scale the zoom word (types 0, 4 and 0Ah); the other types ignore them (a bare ret, BF5h). */
L122C: /* _seg004_122C */
    /* 122C  mov     bx,114h */
    BX = 0x114;
L122F:
    /* 122F  jmp     _seg004_120A */
    goto L120A;

    /* seg004_1231  (+1231) */
L1231: /* _seg004_1231 */
    /* 1231  mov     bx,120h */
    BX = 0x120;
L1234:
    /* 1234  jmp     _seg004_120A */
    goto L120A;

    /* seg004_1236  (+1236) */
L1236: /* _seg004_1236 */
    /* 1236  mov     bx,12Ch */
    BX = 0x12C;
L1239:
    /* 1239  jmp     _seg004_120A */
    goto L120A;

    /* seg004_123B  (+123B) */
L123B: /* _seg004_123B */
    /* 123B  mov     bx,138h */
    BX = 0x138;
L123E:
    /* 123E  jmp     _seg004_120A */
    goto L120A;

    /* seg004_1240  (+1240) */
L1240: /* _seg004_1240 */
    /* 1240  mov     bx,144h */
    BX = 0x144;
L1243:
    /* 1243  jmp     _seg004_120A */
    goto L120A;

    /* seg004_1245  (+1245) */
L1245: /* _seg004_1245 */
    /* 1245  mov     bx,150h */
    BX = 0x150;
L1248:
    /* 1248  jmp     _seg004_120A */
    goto L120A;

    /* seg004_124A  (+124A)
       Command 0 for type 0: lowers word +12h of the description (head_view's second angle) by
       7D0h * ss:[9B0h] / 8000h and clears +0Eh, so head_view rebuilds its matrix. 125F raises
       it; 1274 raises +10h (the first angle) and 1289 lowers it. */
L124A: /* _seg004_124A */
    /* 124A  mov     ax,7D0h */
    AX = 0x7D0;
L124D:
    /* 124D  imul    word ptr ss:[9B0h] */
    imul16(rw(pSS, 0x9B0));
L1252:
    /* 1252  shl     ax,1 */
    AX = shl16(AX, 1);
L1254:
    /* 1254  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1256:
    /* 1256  sub     word ptr [si+12h],dx */
    ww(pDS, SI + 0x12, sub16(rw(pDS, SI + 0x12), DX, 0));
L1259:
    /* 1259  mov     word ptr [si+0Eh],0 */
    ww(pDS, SI + 0xE, 0x0);
L125E:
    /* 125E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_125F  (+125F) */
L125F: /* _seg004_125F */
    /* 125F  mov     ax,7D0h */
    AX = 0x7D0;
L1262:
    /* 1262  imul    word ptr ss:[9B0h] */
    imul16(rw(pSS, 0x9B0));
L1267:
    /* 1267  shl     ax,1 */
    AX = shl16(AX, 1);
L1269:
    /* 1269  rcl     dx,1 */
    DX = rcl16(DX, 1);
L126B:
    /* 126B  add     word ptr [si+12h],dx */
    ww(pDS, SI + 0x12, add16(rw(pDS, SI + 0x12), DX, 0));
L126E:
    /* 126E  mov     word ptr [si+0Eh],0 */
    ww(pDS, SI + 0xE, 0x0);
L1273:
    /* 1273  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_1274  (+1274) */
L1274: /* _seg004_1274 */
    /* 1274  mov     ax,7D0h */
    AX = 0x7D0;
L1277:
    /* 1277  imul    word ptr ss:[9B0h] */
    imul16(rw(pSS, 0x9B0));
L127C:
    /* 127C  shl     ax,1 */
    AX = shl16(AX, 1);
L127E:
    /* 127E  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1280:
    /* 1280  add     word ptr [si+10h],dx */
    ww(pDS, SI + 0x10, add16(rw(pDS, SI + 0x10), DX, 0));
L1283:
    /* 1283  mov     word ptr [si+0Eh],0 */
    ww(pDS, SI + 0xE, 0x0);
L1288:
    /* 1288  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_1289  (+1289) */
L1289: /* _seg004_1289 */
    /* 1289  mov     ax,7D0h */
    AX = 0x7D0;
L128C:
    /* 128C  imul    word ptr ss:[9B0h] */
    imul16(rw(pSS, 0x9B0));
L1291:
    /* 1291  shl     ax,1 */
    AX = shl16(AX, 1);
L1293:
    /* 1293  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1295:
    /* 1295  sub     word ptr [si+10h],dx */
    ww(pDS, SI + 0x10, sub16(rw(pDS, SI + 0x10), DX, 0));
L1298:
    /* 1298  mov     word ptr [si+0Eh],0 */
    ww(pDS, SI + 0xE, 0x0);
L129D:
    /* 129D  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_129E  (+129E)
       Commands 4 and 5 for type 0: word +2 (the zoom) scaled by seg004_12FE or seg004_1332. */
L129E: /* _seg004_129E */
    /* 129E  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L12A1:
    /* 12A1  call    _seg004_12FE */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x12FE), 0x12A4)) != 0) return c;
L12A4:
    /* 12A4  mov     word ptr [si+2],ax */
    ww(pDS, SI + 0x2, AX);
L12A7:
    /* 12A7  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_12A8  (+12A8) */
L12A8: /* _seg004_12A8 */
    /* 12A8  mov     ax,word ptr [si+2] */
    AX = rw(pDS, SI + 0x2);
L12AB:
    /* 12AB  call    _seg004_1332 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x1332), 0x12AE)) != 0) return c;
L12AE:
    /* 12AE  mov     word ptr [si+2],ax */
    ww(pDS, SI + 0x2, AX);
L12B1:
    /* 12B1  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_12B2  (+12B2)
       Commands 4 and 5 for type 4: word +0Eh (the zoom) scaled likewise. */
L12B2: /* _seg004_12B2 */
    /* 12B2  mov     ax,word ptr [si+0Eh] */
    AX = rw(pDS, SI + 0xE);
L12B5:
    /* 12B5  call    _seg004_12FE */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x12FE), 0x12B8)) != 0) return c;
L12B8:
    /* 12B8  mov     word ptr [si+0Eh],ax */
    ww(pDS, SI + 0xE, AX);
L12BB:
    /* 12BB  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_12BC  (+12BC) */
L12BC: /* _seg004_12BC */
    /* 12BC  mov     ax,word ptr [si+0Eh] */
    AX = rw(pDS, SI + 0xE);
L12BF:
    /* 12BF  call    _seg004_1332 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x1332), 0x12C2)) != 0) return c;
L12C2:
    /* 12C2  mov     word ptr [si+0Eh],ax */
    ww(pDS, SI + 0xE, AX);
L12C5:
    /* 12C5  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_12C6  (+12C6)
       Commands 4 and 5 for type 0Ah: word +0Eh of the chosen position (180h), or +10h when +0Eh
       is 0, scaled likewise. */
L12C6: /* _seg004_12C6 */
    /* 12C6  mov     si,word ptr ds:[180h] */
    SI = rw(pDS, 0x180);
L12CA:
    /* 12CA  mov     ax,word ptr [si+0Eh] */
    AX = rw(pDS, SI + 0xE);
L12CD:
    /* 12CD  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L12CF:
    /* 12CF  je      short L12D8 */
    if (ZF) goto L12D8;
L12D1:
    /* 12D1  call    _seg004_12FE */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x12FE), 0x12D4)) != 0) return c;
L12D4:
    /* 12D4  mov     word ptr [si+0Eh],ax */
    ww(pDS, SI + 0xE, AX);
L12D7:
    /* 12D7  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L12D8: /* L12D8 */
    /* 12D8  mov     ax,word ptr [si+10h] */
    AX = rw(pDS, SI + 0x10);
L12DB:
    /* 12DB  call    _seg004_12FE */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x12FE), 0x12DE)) != 0) return c;
L12DE:
    /* 12DE  mov     word ptr [si+10h],ax */
    ww(pDS, SI + 0x10, AX);
L12E1:
    /* 12E1  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_12E2  (+12E2) */
L12E2: /* _seg004_12E2 */
    /* 12E2  mov     si,word ptr ds:[180h] */
    SI = rw(pDS, 0x180);
L12E6:
    /* 12E6  mov     ax,word ptr [si+0Eh] */
    AX = rw(pDS, SI + 0xE);
L12E9:
    /* 12E9  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L12EB:
    /* 12EB  je      short L12F4 */
    if (ZF) goto L12F4;
L12ED:
    /* 12ED  call    _seg004_1332 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x1332), 0x12F0)) != 0) return c;
L12F0:
    /* 12F0  mov     word ptr [si+0Eh],ax */
    ww(pDS, SI + 0xE, AX);
L12F3:
    /* 12F3  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L12F4: /* L12F4 */
    /* 12F4  mov     ax,word ptr [si+10h] */
    AX = rw(pDS, SI + 0x10);
L12F7:
    /* 12F7  call    _seg004_1332 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x1332), 0x12FA)) != 0) return c;
L12FA:
    /* 12FA  mov     word ptr [si+10h],ax */
    ww(pDS, SI + 0x10, AX);
L12FD:
    /* 12FD  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_12FE  (+12FE)
       AX scaled by the factor 7FFFh - ss:[9B0h] (1.15): multiplied when AX is positive, divided
       by it when AX is negative. seg004_1332 does the opposite. */
L12FE: /* _seg004_12FE */
    /* 12FE  mov     bx,word ptr ss:[9B0h] */
    BX = rw(pSS, 0x9B0);
L1303:
    /* 1303  sub     bx,7FFFh */
    BX = (uint16_t)(BX - 0x7FFF);
L1307:
    /* 1307  neg     bx */
    BX = (uint16_t)-BX;
L1309:
    /* 1309  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L130B:
    /* 130B  js      short L1316 */
    if (SF) goto L1316;
L130D:
    /* 130D  imul    bx */
    imul16(BX);
L130F:
    /* 130F  shl     ax,1 */
    AX = shl16(AX, 1);
L1311:
    /* 1311  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1313:
    /* 1313  mov     ax,dx */
    AX = DX;
L1315:
    /* 1315  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L1316: /* L1316 */
    /* 1316  mov     dx,ax */
    DX = AX;
L1318:
    /* 1318  add     ax,bx */
    AX = add16(AX, BX, 0);
L131A:
    /* 131A  js      short L1325 */
    if (SF) goto L1325;
L131C:
    /* 131C  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L131E:
    /* 131E  sar     dx,1 */
    DX = sar16(DX, 1);
L1320:
    /* 1320  rcr     ax,1 */
    AX = rcr16(AX, 1);
L1322:
    /* 1322  idiv    bx */
    if (asm_idiv16(BX) && (c = asm_divfault(0x06E7, 0x1322, 2)) != 0) return c;
L1324:
    /* 1324  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L1325: /* L1325 */
    /* 1325  xchg    dx,bx */
    { uint16_t t_ = BX;
    BX = DX;
    DX = t_; }
L1327:
    /* 1327  neg     bx */
    BX = (uint16_t)-BX;
L1329:
    /* 1329  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L132B:
    /* 132B  sar     dx,1 */
    DX = sar16(DX, 1);
L132D:
    /* 132D  rcr     ax,1 */
    AX = rcr16(AX, 1);
L132F:
    /* 132F  idiv    bx */
    if (asm_idiv16(BX) && (c = asm_divfault(0x06E7, 0x132F, 2)) != 0) return c;
L1331:
    /* 1331  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_1332  (+1332) */
L1332: /* _seg004_1332 */
    /* 1332  mov     dx,ax */
    DX = AX;
L1334:
    /* 1334  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L1336:
    /* 1336  mov     bx,word ptr ss:[9B0h] */
    BX = rw(pSS, 0x9B0);
L133B:
    /* 133B  sub     bx,7FFFh */
    BX = (uint16_t)(BX - 0x7FFF);
L133F:
    /* 133F  neg     bx */
    BX = (uint16_t)-BX;
L1341:
    /* 1341  or      dx,dx */
    DX = logic16((uint16_t)(DX | DX));
L1343:
    /* 1343  js      short L135B */
    if (SF) goto L135B;
L1345:
    /* 1345  cmp     dx,bx */
    sub16(DX, BX, 0);
L1347:
    /* 1347  jg      short L1350 */
    if (!ZF && SF == OF) goto L1350;
L1349:
    /* 1349  sar     dx,1 */
    DX = sar16(DX, 1);
L134B:
    /* 134B  rcr     ax,1 */
    AX = rcr16(AX, 1);
L134D:
    /* 134D  idiv    bx */
    if (asm_idiv16(BX) && (c = asm_divfault(0x06E7, 0x134D, 2)) != 0) return c;
L134F:
    /* 134F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L1350: /* L1350 */
    /* 1350  xchg    dx,bx */
    { uint16_t t_ = BX;
    BX = DX;
    DX = t_; }
L1352:
    /* 1352  sar     dx,1 */
    DX = sar16(DX, 1);
L1354:
    /* 1354  rcr     ax,1 */
    AX = rcr16(AX, 1);
L1356:
    /* 1356  idiv    bx */
    if (asm_idiv16(BX) && (c = asm_divfault(0x06E7, 0x1356, 2)) != 0) return c;
L1358:
    /* 1358  neg     ax */
    AX = neg16(AX);
L135A:
    /* 135A  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L135B: /* L135B */
    /* 135B  mov     ax,dx */
    AX = DX;
L135D:
    /* 135D  imul    bx */
    imul16(BX);
L135F:
    /* 135F  shl     ax,1 */
    AX = shl16(AX, 1);
L1361:
    /* 1361  rcl     dx,1 */
    DX = rcl16(DX, 1);
L1363:
    /* 1363  mov     ax,dx */
    AX = DX;
L1365:
    /* 1365  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
