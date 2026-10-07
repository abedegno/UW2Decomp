/* grentry.c: replaces src/gfx/GRENTRY.ASM (seg003_A46, 0A46..1082 of its
   segment). Written by tools/asm2c.py from the assembly and the bytes it assembles to; do not
   edit by hand: change the tool, or its OVERRIDES, and run it again. Each instruction is the
   C below its source line; the comments before a routine are the .ASM file's. */
#include "x86/asmrt.h"

int port_hole_mark(void);                /* gfx/holeprobe.c */
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
    case 0x0A46: goto L0A46;
    case 0x0A48: goto L0A48;
    case 0x0A4A: goto L0A4A;
    case 0x0A4B: goto L0A4B;
    case 0x0A4E: goto L0A4E;
    case 0x0A50: goto L0A50;
    case 0x0A53: goto L0A53;
    case 0x0A55: goto L0A55;
    case 0x0A58: goto L0A58;
    case 0x0A5A: goto L0A5A;
    case 0x0A5B: goto L0A5B;
    case 0x0A5C: goto L0A5C;
    case 0x0A5D: goto L0A5D;
    case 0x0A5E: goto L0A5E;
    case 0x0A63: goto L0A63;
    case 0x0A68: goto L0A68;
    case 0x0A6A: goto L0A6A;
    case 0x0A6C: goto L0A6C;
    case 0x0A6E: goto L0A6E;
    case 0x0A70: goto L0A70;
    case 0x0A75: goto L0A75;
    case 0x0A77: goto L0A77;
    case 0x0A79: goto L0A79;
    case 0x0A7B: goto L0A7B;
    case 0x0A7D: goto L0A7D;
    case 0x0A7F: goto L0A7F;
    case 0x0A80: goto L0A80;
    case 0x0A82: goto L0A82;
    case 0x0A84: goto L0A84;
    case 0x0A85: goto L0A85;
    case 0x0A87: goto L0A87;
    case 0x0A88: goto L0A88;
    case 0x0A8A: goto L0A8A;
    case 0x0A8B: goto L0A8B;
    case 0x0A8C: goto L0A8C;
    case 0x0A8D: goto L0A8D;
    case 0x0A8E: goto L0A8E;
    case 0x0A8F: goto L0A8F;
    case 0x0A94: goto L0A94;
    case 0x0A99: goto L0A99;
    case 0x0A9B: goto L0A9B;
    case 0x0A9D: goto L0A9D;
    case 0x0A9F: goto L0A9F;
    case 0x0AA1: goto L0AA1;
    case 0x0AA6: goto L0AA6;
    case 0x0AA8: goto L0AA8;
    case 0x0AAA: goto L0AAA;
    case 0x0AAC: goto L0AAC;
    case 0x0AAE: goto L0AAE;
    case 0x0AB0: goto L0AB0;
    case 0x0AB1: goto L0AB1;
    case 0x0AB3: goto L0AB3;
    case 0x0AB5: goto L0AB5;
    case 0x0AB6: goto L0AB6;
    case 0x0AB8: goto L0AB8;
    case 0x0AB9: goto L0AB9;
    case 0x0ABB: goto L0ABB;
    case 0x0ABD: goto L0ABD;
    case 0x0ABE: goto L0ABE;
    case 0x0ABF: goto L0ABF;
    case 0x0AC0: goto L0AC0;
    case 0x0AC2: goto L0AC2;
    case 0x0AC3: goto L0AC3;
    case 0x0AC5: goto L0AC5;
    case 0x0AC6: goto L0AC6;
    case 0x0AC7: goto L0AC7;
    case 0x0AC8: goto L0AC8;
    case 0x0AC9: goto L0AC9;
    case 0x0ACA: goto L0ACA;
    case 0x0ACD: goto L0ACD;
    case 0x0ACF: goto L0ACF;
    case 0x0AD2: goto L0AD2;
    case 0x0AD4: goto L0AD4;
    case 0x0AD6: goto L0AD6;
    case 0x0AD8: goto L0AD8;
    case 0x0ADB: goto L0ADB;
    case 0x0ADD: goto L0ADD;
    case 0x0ADF: goto L0ADF;
    case 0x0AE4: goto L0AE4;
    case 0x0AE6: goto L0AE6;
    case 0x0AE7: goto L0AE7;
    case 0x0AE9: goto L0AE9;
    case 0x0AEA: goto L0AEA;
    case 0x0AEB: goto L0AEB;
    case 0x0AEC: goto L0AEC;
    case 0x0AED: goto L0AED;
    case 0x0AF0: goto L0AF0;
    case 0x0AF2: goto L0AF2;
    case 0x0AF5: goto L0AF5;
    case 0x0AF7: goto L0AF7;
    case 0x0AF9: goto L0AF9;
    case 0x0AFE: goto L0AFE;
    case 0x0B00: goto L0B00;
    case 0x0B01: goto L0B01;
    case 0x0B03: goto L0B03;
    case 0x0B04: goto L0B04;
    case 0x0B06: goto L0B06;
    case 0x0B07: goto L0B07;
    case 0x0B0C: goto L0B0C;
    case 0x0B0F: goto L0B0F;
    case 0x0B12: goto L0B12;
    case 0x0B14: goto L0B14;
    case 0x0B16: goto L0B16;
    case 0x0B19: goto L0B19;
    case 0x0B1A: goto L0B1A;
    case 0x0B1C: goto L0B1C;
    case 0x0B1E: goto L0B1E;
    case 0x0B21: goto L0B21;
    case 0x0B24: goto L0B24;
    case 0x0B26: goto L0B26;
    case 0x0B27: goto L0B27;
    case 0x0B2B: goto L0B2B;
    case 0x0B2D: goto L0B2D;
    case 0x0B31: goto L0B31;
    case 0x0B33: goto L0B33;
    case 0x0B39: goto L0B39;
    case 0x0B3B: goto L0B3B;
    case 0x0B3E: goto L0B3E;
    case 0x0B42: goto L0B42;
    case 0x0B44: goto L0B44;
    case 0x0B48: goto L0B48;
    case 0x0B4B: goto L0B4B;
    case 0x0B4F: goto L0B4F;
    case 0x0B51: goto L0B51;
    case 0x0B54: goto L0B54;
    case 0x0B58: goto L0B58;
    case 0x0B5C: goto L0B5C;
    case 0x0B5D: goto L0B5D;
    case 0x0B5E: goto L0B5E;
    case 0x0B5F: goto L0B5F;
    case 0x0B60: goto L0B60;
    case 0x0B61: goto L0B61;
    case 0x0B62: goto L0B62;
    case 0x0B65: goto L0B65;
    case 0x0B67: goto L0B67;
    case 0x0B69: goto L0B69;
    case 0x0B6B: goto L0B6B;
    case 0x0B70: goto L0B70;
    case 0x0B75: goto L0B75;
    case 0x0B76: goto L0B76;
    case 0x0B78: goto L0B78;
    case 0x0B7C: goto L0B7C;
    case 0x0B7D: goto L0B7D;
    case 0x0B80: goto L0B80;
    case 0x0B82: goto L0B82;
    case 0x0B86: goto L0B86;
    case 0x0B89: goto L0B89;
    case 0x0B8D: goto L0B8D;
    case 0x0B90: goto L0B90;
    case 0x0B92: goto L0B92;
    case 0x0B96: goto L0B96;
    case 0x0B98: goto L0B98;
    case 0x0B9C: goto L0B9C;
    case 0x0B9E: goto L0B9E;
    case 0x0B9F: goto L0B9F;
    case 0x0BA1: goto L0BA1;
    case 0x0BA4: goto L0BA4;
    case 0x0BA6: goto L0BA6;
    case 0x0BA9: goto L0BA9;
    case 0x0BAB: goto L0BAB;
    case 0x0BAC: goto L0BAC;
    case 0x0BAE: goto L0BAE;
    case 0x0BB1: goto L0BB1;
    case 0x0BB3: goto L0BB3;
    case 0x0BB6: goto L0BB6;
    case 0x0BB8: goto L0BB8;
    case 0x0BB9: goto L0BB9;
    case 0x0BBB: goto L0BBB;
    case 0x0BBE: goto L0BBE;
    case 0x0BC0: goto L0BC0;
    case 0x0BC3: goto L0BC3;
    case 0x0BC5: goto L0BC5;
    case 0x0BC6: goto L0BC6;
    case 0x0BC8: goto L0BC8;
    case 0x0BCA: goto L0BCA;
    case 0x0BCD: goto L0BCD;
    case 0x0BD0: goto L0BD0;
    case 0x0BD2: goto L0BD2;
    case 0x0BD3: goto L0BD3;
    case 0x0BD8: goto L0BD8;
    case 0x0BDA: goto L0BDA;
    case 0x0BDB: goto L0BDB;
    case 0x0BE0: goto L0BE0;
    case 0x0BE5: goto L0BE5;
    case 0x0BE6: goto L0BE6;
    case 0x0BE7: goto L0BE7;
    case 0x0BE8: goto L0BE8;
    case 0x0BE9: goto L0BE9;
    case 0x0BEA: goto L0BEA;
    case 0x0BEB: goto L0BEB;
    case 0x0BEC: goto L0BEC;
    case 0x0BED: goto L0BED;
    case 0x0BF0: goto L0BF0;
    case 0x0BF1: goto L0BF1;
    case 0x0BF4: goto L0BF4;
    case 0x0BF5: goto L0BF5;
    case 0x0BF8: goto L0BF8;
    case 0x0BF9: goto L0BF9;
    case 0x0BFC: goto L0BFC;
    case 0x0BFD: goto L0BFD;
    case 0x0C00: goto L0C00;
    case 0x0C01: goto L0C01;
    case 0x0C04: goto L0C04;
    case 0x0C05: goto L0C05;
    case 0x0C08: goto L0C08;
    case 0x0C09: goto L0C09;
    case 0x0C0C: goto L0C0C;
    case 0x0C0D: goto L0C0D;
    case 0x0C10: goto L0C10;
    case 0x0C11: goto L0C11;
    case 0x0C14: goto L0C14;
    case 0x0C15: goto L0C15;
    case 0x0C18: goto L0C18;
    case 0x0C19: goto L0C19;
    case 0x0C1C: goto L0C1C;
    case 0x0C1D: goto L0C1D;
    case 0x0C20: goto L0C20;
    case 0x0C21: goto L0C21;
    case 0x0C24: goto L0C24;
    case 0x0C25: goto L0C25;
    case 0x0C28: goto L0C28;
    case 0x0C29: goto L0C29;
    case 0x0C2C: goto L0C2C;
    case 0x0C2D: goto L0C2D;
    case 0x0C30: goto L0C30;
    case 0x0C31: goto L0C31;
    case 0x0C34: goto L0C34;
    case 0x0C35: goto L0C35;
    case 0x0C38: goto L0C38;
    case 0x0C39: goto L0C39;
    case 0x0C3C: goto L0C3C;
    case 0x0C3D: goto L0C3D;
    case 0x0C40: goto L0C40;
    case 0x0C41: goto L0C41;
    case 0x0C44: goto L0C44;
    case 0x0C45: goto L0C45;
    case 0x0C48: goto L0C48;
    case 0x0C49: goto L0C49;
    case 0x0C4C: goto L0C4C;
    case 0x0C4D: goto L0C4D;
    case 0x0C50: goto L0C50;
    case 0x0C51: goto L0C51;
    case 0x0C54: goto L0C54;
    case 0x0C55: goto L0C55;
    case 0x0C58: goto L0C58;
    case 0x0C59: goto L0C59;
    case 0x0C5C: goto L0C5C;
    case 0x0C5D: goto L0C5D;
    case 0x0C60: goto L0C60;
    case 0x0C61: goto L0C61;
    case 0x0C64: goto L0C64;
    case 0x0C65: goto L0C65;
    case 0x0C68: goto L0C68;
    case 0x0C69: goto L0C69;
    case 0x0C6C: goto L0C6C;
    case 0x0C6D: goto L0C6D;
    case 0x0C70: goto L0C70;
    case 0x0C71: goto L0C71;
    case 0x0C74: goto L0C74;
    case 0x0C75: goto L0C75;
    case 0x0C78: goto L0C78;
    case 0x0C79: goto L0C79;
    case 0x0C7C: goto L0C7C;
    case 0x0C7D: goto L0C7D;
    case 0x0C80: goto L0C80;
    case 0x0C81: goto L0C81;
    case 0x0C84: goto L0C84;
    case 0x0C85: goto L0C85;
    case 0x0C88: goto L0C88;
    case 0x0C89: goto L0C89;
    case 0x0C8C: goto L0C8C;
    case 0x0C8D: goto L0C8D;
    case 0x0C90: goto L0C90;
    case 0x0C91: goto L0C91;
    case 0x0C94: goto L0C94;
    case 0x0C95: goto L0C95;
    case 0x0C98: goto L0C98;
    case 0x0C99: goto L0C99;
    case 0x0C9C: goto L0C9C;
    case 0x0C9D: goto L0C9D;
    case 0x0CA0: goto L0CA0;
    case 0x0CA1: goto L0CA1;
    case 0x0CA4: goto L0CA4;
    case 0x0CA5: goto L0CA5;
    case 0x0CA8: goto L0CA8;
    case 0x0CA9: goto L0CA9;
    case 0x0CAC: goto L0CAC;
    case 0x0CAD: goto L0CAD;
    case 0x0CB0: goto L0CB0;
    case 0x0CB1: goto L0CB1;
    case 0x0CB4: goto L0CB4;
    case 0x0CB5: goto L0CB5;
    case 0x0CB8: goto L0CB8;
    case 0x0CB9: goto L0CB9;
    case 0x0CBC: goto L0CBC;
    case 0x0CBD: goto L0CBD;
    case 0x0CC0: goto L0CC0;
    case 0x0CC1: goto L0CC1;
    case 0x0CC4: goto L0CC4;
    case 0x0CC5: goto L0CC5;
    case 0x0CC8: goto L0CC8;
    case 0x0CC9: goto L0CC9;
    case 0x0CCC: goto L0CCC;
    case 0x0CCD: goto L0CCD;
    case 0x0CD0: goto L0CD0;
    case 0x0CD1: goto L0CD1;
    case 0x0CD4: goto L0CD4;
    case 0x0CD5: goto L0CD5;
    case 0x0CD8: goto L0CD8;
    case 0x0CD9: goto L0CD9;
    case 0x0CDC: goto L0CDC;
    case 0x0CDD: goto L0CDD;
    case 0x0CE0: goto L0CE0;
    case 0x0CE1: goto L0CE1;
    case 0x0CE4: goto L0CE4;
    case 0x0CE5: goto L0CE5;
    case 0x0CE8: goto L0CE8;
    case 0x0CE9: goto L0CE9;
    case 0x0CEC: goto L0CEC;
    case 0x0CED: goto L0CED;
    case 0x0CF0: goto L0CF0;
    case 0x0CF1: goto L0CF1;
    case 0x0CF4: goto L0CF4;
    case 0x0CF5: goto L0CF5;
    case 0x0CF8: goto L0CF8;
    case 0x0CF9: goto L0CF9;
    case 0x0CFC: goto L0CFC;
    case 0x0CFD: goto L0CFD;
    case 0x0D00: goto L0D00;
    case 0x0D01: goto L0D01;
    case 0x0D04: goto L0D04;
    case 0x0D05: goto L0D05;
    case 0x0D08: goto L0D08;
    case 0x0D09: goto L0D09;
    case 0x0D0C: goto L0D0C;
    case 0x0D0D: goto L0D0D;
    case 0x0D10: goto L0D10;
    case 0x0D11: goto L0D11;
    case 0x0D14: goto L0D14;
    case 0x0D15: goto L0D15;
    case 0x0D18: goto L0D18;
    case 0x0D19: goto L0D19;
    case 0x0D1C: goto L0D1C;
    case 0x0D1D: goto L0D1D;
    case 0x0D20: goto L0D20;
    case 0x0D21: goto L0D21;
    case 0x0D24: goto L0D24;
    case 0x0D25: goto L0D25;
    case 0x0D28: goto L0D28;
    case 0x0D29: goto L0D29;
    case 0x0D2C: goto L0D2C;
    case 0x0D2D: goto L0D2D;
    case 0x0D2E: goto L0D2E;
    case 0x0D32: goto L0D32;
    case 0x0D36: goto L0D36;
    case 0x0D37: goto L0D37;
    case 0x0D39: goto L0D39;
    case 0x0D3B: goto L0D3B;
    case 0x0D3D: goto L0D3D;
    case 0x0D41: goto L0D41;
    case 0x0D42: goto L0D42;
    case 0x0D44: goto L0D44;
    case 0x0D46: goto L0D46;
    case 0x0D47: goto L0D47;
    case 0x0D49: goto L0D49;
    case 0x0D4B: goto L0D4B;
    case 0x0D4E: goto L0D4E;
    case 0x0D4F: goto L0D4F;
    case 0x0D51: goto L0D51;
    case 0x0D53: goto L0D53;
    case 0x0D55: goto L0D55;
    case 0x0D56: goto L0D56;
    case 0x0D57: goto L0D57;
    case 0x0D59: goto L0D59;
    case 0x0D5A: goto L0D5A;
    case 0x0D5C: goto L0D5C;
    case 0x0D5E: goto L0D5E;
    case 0x0D60: goto L0D60;
    case 0x0D63: goto L0D63;
    case 0x0D65: goto L0D65;
    case 0x0D69: goto L0D69;
    case 0x0D6A: goto L0D6A;
    case 0x0D6B: goto L0D6B;
    case 0x0D6F: goto L0D6F;
    case 0x0D73: goto L0D73;
    case 0x0D76: goto L0D76;
    case 0x0D78: goto L0D78;
    case 0x0D7A: goto L0D7A;
    case 0x0D7D: goto L0D7D;
    case 0x0D80: goto L0D80;
    case 0x0D83: goto L0D83;
    case 0x0D86: goto L0D86;
    case 0x0D89: goto L0D89;
    case 0x0D8C: goto L0D8C;
    case 0x0D8E: goto L0D8E;
    case 0x0D93: goto L0D93;
    case 0x0D95: goto L0D95;
    case 0x0D96: goto L0D96;
    case 0x0D99: goto L0D99;
    case 0x0D9B: goto L0D9B;
    case 0x0D9D: goto L0D9D;
    case 0x0D9E: goto L0D9E;
    case 0x0D9F: goto L0D9F;
    case 0x0DA2: goto L0DA2;
    case 0x0DA4: goto L0DA4;
    case 0x0DA6: goto L0DA6;
    case 0x0DA7: goto L0DA7;
    case 0x0DA9: goto L0DA9;
    case 0x0DAC: goto L0DAC;
    case 0x0DAE: goto L0DAE;
    case 0x0DB0: goto L0DB0;
    case 0x0DB1: goto L0DB1;
    case 0x0DB2: goto L0DB2;
    case 0x0DB5: goto L0DB5;
    case 0x0DB7: goto L0DB7;
    case 0x0DB9: goto L0DB9;
    case 0x0DBA: goto L0DBA;
    case 0x0DBC: goto L0DBC;
    case 0x0DBF: goto L0DBF;
    case 0x0DC1: goto L0DC1;
    case 0x0DC3: goto L0DC3;
    case 0x0DC4: goto L0DC4;
    case 0x0DC5: goto L0DC5;
    case 0x0DC8: goto L0DC8;
    case 0x0DCA: goto L0DCA;
    case 0x0DCC: goto L0DCC;
    case 0x0DCD: goto L0DCD;
    case 0x0DCF: goto L0DCF;
    case 0x0DD2: goto L0DD2;
    case 0x0DD4: goto L0DD4;
    case 0x0DD6: goto L0DD6;
    case 0x0DD7: goto L0DD7;
    case 0x0DD8: goto L0DD8;
    case 0x0DDB: goto L0DDB;
    case 0x0DDD: goto L0DDD;
    case 0x0DDE: goto L0DDE;
    case 0x0DE0: goto L0DE0;
    case 0x0DE1: goto L0DE1;
    case 0x0DE2: goto L0DE2;
    case 0x0DE3: goto L0DE3;
    case 0x0DE4: goto L0DE4;
    case 0x0DE5: goto L0DE5;
    case 0x0DE9: goto L0DE9;
    case 0x0DED: goto L0DED;
    case 0x0DEF: goto L0DEF;
    case 0x0DF2: goto L0DF2;
    case 0x0DF4: goto L0DF4;
    case 0x0DF6: goto L0DF6;
    case 0x0DF9: goto L0DF9;
    case 0x0DFE: goto L0DFE;
    case 0x0E01: goto L0E01;
    case 0x0E04: goto L0E04;
    case 0x0E07: goto L0E07;
    case 0x0E09: goto L0E09;
    case 0x0E0C: goto L0E0C;
    case 0x0E0D: goto L0E0D;
    case 0x0E0F: goto L0E0F;
    case 0x0E10: goto L0E10;
    case 0x0E12: goto L0E12;
    case 0x0E15: goto L0E15;
    case 0x0E1A: goto L0E1A;
    case 0x0E1F: goto L0E1F;
    case 0x0E22: goto L0E22;
    case 0x0E25: goto L0E25;
    case 0x0E26: goto L0E26;
    case 0x0E27: goto L0E27;
    case 0x0E2A: goto L0E2A;
    case 0x0E2C: goto L0E2C;
    case 0x0E2E: goto L0E2E;
    case 0x0E31: goto L0E31;
    case 0x0E34: goto L0E34;
    case 0x0E36: goto L0E36;
    case 0x0E39: goto L0E39;
    case 0x0E3B: goto L0E3B;
    case 0x0E3C: goto L0E3C;
    case 0x0E3D: goto L0E3D;
    case 0x0E3F: goto L0E3F;
    case 0x0E42: goto L0E42;
    case 0x0E44: goto L0E44;
    case 0x0E45: goto L0E45;
    case 0x0E46: goto L0E46;
    case 0x0E48: goto L0E48;
    case 0x0E4B: goto L0E4B;
    case 0x0E4D: goto L0E4D;
    case 0x0E52: goto L0E52;
    case 0x0E57: goto L0E57;
    case 0x0E5A: goto L0E5A;
    case 0x0E5D: goto L0E5D;
    case 0x0E5E: goto L0E5E;
    case 0x0E5F: goto L0E5F;
    case 0x0E62: goto L0E62;
    case 0x0E64: goto L0E64;
    case 0x0E66: goto L0E66;
    case 0x0E69: goto L0E69;
    case 0x0E6C: goto L0E6C;
    case 0x0E6E: goto L0E6E;
    case 0x0E71: goto L0E71;
    case 0x0E73: goto L0E73;
    case 0x0E74: goto L0E74;
    case 0x0E75: goto L0E75;
    case 0x0E77: goto L0E77;
    case 0x0E7A: goto L0E7A;
    case 0x0E7C: goto L0E7C;
    case 0x0E7D: goto L0E7D;
    case 0x0E7E: goto L0E7E;
    case 0x0E80: goto L0E80;
    case 0x0E82: goto L0E82;
    case 0x0E87: goto L0E87;
    case 0x0E8C: goto L0E8C;
    case 0x0E8F: goto L0E8F;
    case 0x0E92: goto L0E92;
    case 0x0E93: goto L0E93;
    case 0x0E94: goto L0E94;
    case 0x0E97: goto L0E97;
    case 0x0E99: goto L0E99;
    case 0x0E9B: goto L0E9B;
    case 0x0E9E: goto L0E9E;
    case 0x0EA1: goto L0EA1;
    case 0x0EA3: goto L0EA3;
    case 0x0EA6: goto L0EA6;
    case 0x0EA8: goto L0EA8;
    case 0x0EA9: goto L0EA9;
    case 0x0EAA: goto L0EAA;
    case 0x0EAC: goto L0EAC;
    case 0x0EAF: goto L0EAF;
    case 0x0EB1: goto L0EB1;
    case 0x0EB2: goto L0EB2;
    case 0x0EB3: goto L0EB3;
    case 0x0EB5: goto L0EB5;
    case 0x0EB7: goto L0EB7;
    case 0x0EBC: goto L0EBC;
    case 0x0EC1: goto L0EC1;
    case 0x0EC4: goto L0EC4;
    case 0x0EC7: goto L0EC7;
    case 0x0EC8: goto L0EC8;
    case 0x0EC9: goto L0EC9;
    case 0x0ECC: goto L0ECC;
    case 0x0ECE: goto L0ECE;
    case 0x0ED0: goto L0ED0;
    case 0x0ED3: goto L0ED3;
    case 0x0ED6: goto L0ED6;
    case 0x0ED8: goto L0ED8;
    case 0x0EDB: goto L0EDB;
    case 0x0EDD: goto L0EDD;
    case 0x0EDE: goto L0EDE;
    case 0x0EDF: goto L0EDF;
    case 0x0EE1: goto L0EE1;
    case 0x0EE4: goto L0EE4;
    case 0x0EE6: goto L0EE6;
    case 0x0EE7: goto L0EE7;
    case 0x0EE8: goto L0EE8;
    case 0x0EEA: goto L0EEA;
    case 0x0EEC: goto L0EEC;
    case 0x0EED: goto L0EED;
    case 0x0EF0: goto L0EF0;
    case 0x0EF3: goto L0EF3;
    case 0x0EF6: goto L0EF6;
    case 0x0EF7: goto L0EF7;
    case 0x0EF8: goto L0EF8;
    case 0x0EF9: goto L0EF9;
    case 0x0EFA: goto L0EFA;
    case 0x0EFD: goto L0EFD;
    case 0x0EFF: goto L0EFF;
    case 0x0F01: goto L0F01;
    case 0x0F03: goto L0F03;
    case 0x0F05: goto L0F05;
    case 0x0F09: goto L0F09;
    case 0x0F0C: goto L0F0C;
    case 0x0F10: goto L0F10;
    case 0x0F13: goto L0F13;
    case 0x0F17: goto L0F17;
    case 0x0F1A: goto L0F1A;
    case 0x0F1C: goto L0F1C;
    case 0x0F20: goto L0F20;
    case 0x0F23: goto L0F23;
    case 0x0F24: goto L0F24;
    case 0x0F2B: goto L0F2B;
    case 0x0F2C: goto L0F2C;
    case 0x0F2D: goto L0F2D;
    case 0x0F31: goto L0F31;
    case 0x0F35: goto L0F35;
    case 0x0F37: goto L0F37;
    case 0x0F3A: goto L0F3A;
    case 0x0F3C: goto L0F3C;
    case 0x0F41: goto L0F41;
    case 0x0F44: goto L0F44;
    case 0x0F48: goto L0F48;
    case 0x0F4B: goto L0F4B;
    case 0x0F4E: goto L0F4E;
    case 0x0F4F: goto L0F4F;
    case 0x0F50: goto L0F50;
    case 0x0F54: goto L0F54;
    case 0x0F56: goto L0F56;
    case 0x0F57: goto L0F57;
    case 0x0F58: goto L0F58;
    case 0x0F5D: goto L0F5D;
    case 0x0F62: goto L0F62;
    case 0x0F65: goto L0F65;
    case 0x0F67: goto L0F67;
    case 0x0F69: goto L0F69;
    case 0x0F6B: goto L0F6B;
    case 0x0F70: goto L0F70;
    case 0x0F72: goto L0F72;
    case 0x0F73: goto L0F73;
    case 0x0F75: goto L0F75;
    case 0x0F76: goto L0F76;
    case 0x0F77: goto L0F77;
    case 0x0F78: goto L0F78;
    case 0x0F7C: goto L0F7C;
    case 0x0F7E: goto L0F7E;
    case 0x0F80: goto L0F80;
    case 0x0F82: goto L0F82;
    case 0x0F87: goto L0F87;
    case 0x0F88: goto L0F88;
    case 0x0F89: goto L0F89;
    case 0x0F8A: goto L0F8A;
    case 0x0F8B: goto L0F8B;
    case 0x0F8C: goto L0F8C;
    case 0x0F8D: goto L0F8D;
    case 0x0F8E: goto L0F8E;
    case 0x0F90: goto L0F90;
    case 0x0F92: goto L0F92;
    case 0x0F94: goto L0F94;
    case 0x0F96: goto L0F96;
    case 0x0F98: goto L0F98;
    case 0x0F99: goto L0F99;
    case 0x0F9E: goto L0F9E;
    case 0x0FA3: goto L0FA3;
    case 0x0FA5: goto L0FA5;
    case 0x0FA6: goto L0FA6;
    case 0x0FA8: goto L0FA8;
    case 0x0FAA: goto L0FAA;
    case 0x0FAF: goto L0FAF;
    case 0x0FB1: goto L0FB1;
    case 0x0FB3: goto L0FB3;
    case 0x0FB5: goto L0FB5;
    case 0x0FB7: goto L0FB7;
    case 0x0FB9: goto L0FB9;
    case 0x0FBB: goto L0FBB;
    case 0x0FBD: goto L0FBD;
    case 0x0FBF: goto L0FBF;
    case 0x0FC0: goto L0FC0;
    case 0x0FC2: goto L0FC2;
    case 0x0FC4: goto L0FC4;
    case 0x0FC6: goto L0FC6;
    case 0x0FC8: goto L0FC8;
    case 0x0FCA: goto L0FCA;
    case 0x0FCC: goto L0FCC;
    case 0x0FCE: goto L0FCE;
    case 0x0FD2: goto L0FD2;
    case 0x0FD4: goto L0FD4;
    case 0x0FD6: goto L0FD6;
    case 0x0FD8: goto L0FD8;
    case 0x0FD9: goto L0FD9;
    case 0x0FDB: goto L0FDB;
    case 0x0FDD: goto L0FDD;
    case 0x0FDF: goto L0FDF;
    case 0x0FE1: goto L0FE1;
    case 0x0FE3: goto L0FE3;
    case 0x0FE5: goto L0FE5;
    case 0x0FEA: goto L0FEA;
    case 0x0FEC: goto L0FEC;
    case 0x0FED: goto L0FED;
    case 0x0FF2: goto L0FF2;
    case 0x0FF7: goto L0FF7;
    case 0x0FF8: goto L0FF8;
    case 0x0FF9: goto L0FF9;
    case 0x0FFB: goto L0FFB;
    case 0x0FFD: goto L0FFD;
    case 0x0FFF: goto L0FFF;
    case 0x1001: goto L1001;
    case 0x1004: goto L1004;
    case 0x1007: goto L1007;
    case 0x100C: goto L100C;
    case 0x100E: goto L100E;
    case 0x1010: goto L1010;
    case 0x1013: goto L1013;
    case 0x1015: goto L1015;
    case 0x101A: goto L101A;
    case 0x101D: goto L101D;
    case 0x101F: goto L101F;
    case 0x1022: goto L1022;
    case 0x1024: goto L1024;
    case 0x1026: goto L1026;
    case 0x1027: goto L1027;
    case 0x1028: goto L1028;
    case 0x102A: goto L102A;
    case 0x102C: goto L102C;
    case 0x102F: goto L102F;
    case 0x1031: goto L1031;
    case 0x1033: goto L1033;
    case 0x1034: goto L1034;
    case 0x1035: goto L1035;
    case 0x1037: goto L1037;
    case 0x1038: goto L1038;
    case 0x1039: goto L1039;
    case 0x103A: goto L103A;
    case 0x103D: goto L103D;
    case 0x103E: goto L103E;
    case 0x1040: goto L1040;
    case 0x1042: goto L1042;
    case 0x1044: goto L1044;
    case 0x1047: goto L1047;
    case 0x104A: goto L104A;
    case 0x104F: goto L104F;
    case 0x1051: goto L1051;
    case 0x1053: goto L1053;
    case 0x1055: goto L1055;
    case 0x1056: goto L1056;
    case 0x1059: goto L1059;
    case 0x105B: goto L105B;
    case 0x105E: goto L105E;
    case 0x1060: goto L1060;
    case 0x1062: goto L1062;
    case 0x1065: goto L1065;
    case 0x1067: goto L1067;
    case 0x1069: goto L1069;
    case 0x106A: goto L106A;
    case 0x106B: goto L106B;
    case 0x106D: goto L106D;
    case 0x106F: goto L106F;
    case 0x1071: goto L1071;
    case 0x1074: goto L1074;
    case 0x1076: goto L1076;
    case 0x1078: goto L1078;
    case 0x1079: goto L1079;
    case 0x107A: goto L107A;
    case 0x107C: goto L107C;
    case 0x107D: goto L107D;
    case 0x107E: goto L107E;
    case 0x107F: goto L107F;
    default: asm_bad_entry("GRENTRY.ASM", entry);
    }

    /* seg003_A46  (+A46)
       clear_fbuf (FM Towns): fill the frame buffer with colour 0 (falls into _A48). */
L0A46: /* _seg003_A46 */
    /* 0A46  xor     ax,ax */
    /* by hand: clear_fbuf's xor ax,ax: the colour the 3D view is cleared to, 0; with the hole probe (a test, gfx/holeprobe.c) its byte, so that uncovered pixels show */
    AX = (uint16_t)(port_hole_mark() >= 0 ? port_hole_mark() * 0x101 : 0);

    /* seg003_A48  (+A48)
       fill_fbuf (far, cFillFB): AL = the colour. Fills 2667h words of the frame buffer segment from
       offset 2.

       The two labels inside are span transfers into the frame buffer for VIDMODE.ASM's fbshow: _A5C
       (fbuf_draw_ylrpp) copies linear bitmap rows (8-byte records: y, left, right, source offset;
       source segment in 55EA) into the frame buffer, and _A8D (fbuf_draw_ylrpp_x) does the same
       skipping colour 0. */
L0A48: /* _seg003_A48 */
    /* 0A48  mov     ah,al */
    AH = AL;
L0A4A:
    /* 0A4A  push    es */
    push16(asm_es);
L0A4B:
    /* 0A4B  mov     cx,seg _stdat */
    CX = (uint16_t)(0x3F4B + PORT_LOAD_SEG);
L0A4E:
    /* 0A4E  mov     es,cx */
    SET_ES(CX);
L0A50:
    /* 0A50  mov     cx,2667h */
    CX = 0x2667;
L0A53:
    /* 0A53  xor     di,di */
    DI = (uint16_t)(DI ^ DI);
L0A55:
    /* 0A55  add     di,2 */
    DI = add16(DI, 0x2, 0);
L0A58:
    /* 0A58  rep stosw */
    while (CX) { ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0A5A:
    /* 0A5A  pop     es */
    SET_ES(pop16());
L0A5B:
    /* 0A5B  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;
L0A5C: /* _seg003_A5C */
    /* 0A5C  push    es */
    push16(asm_es);
L0A5D:
    /* 0A5D  push    ds */
    push16(asm_ds);
L0A5E:
    /* 0A5E  mov     es,word ptr ss:[95Ah] */
    SET_ES(rw(pSS, 0x95A));
L0A63:
    /* 0A63  mov     ds,word ptr ss:[55EAh] */
    SET_DS(rw(pSS, 0x55EA));
L0A68: /* L0A68 */
    /* 0A68  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0A6A:
    /* 0A6A  shl     ax,1 */
    AX = shl16(AX, 1);
L0A6C:
    /* 0A6C  jb      L0A8A */
    if (CF) goto L0A8A;
L0A6E:
    /* 0A6E  mov     bx,ax */
    BX = AX;
L0A70:
    /* 0A70  mov     di,word ptr ss:[bx+95Eh] */
    DI = rw(pSS, BX + 0x95E);
L0A75:
    /* 0A75  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0A77:
    /* 0A77  add     di,ax */
    DI = (uint16_t)(DI + AX);
L0A79:
    /* 0A79  mov     cx,ax */
    CX = AX;
L0A7B:
    /* 0A7B  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0A7D:
    /* 0A7D  sub     ax,cx */
    AX = (uint16_t)(AX - CX);
L0A7F:
    /* 0A7F  inc     ax */
    AX = (uint16_t)(AX + 1);
L0A80:
    /* 0A80  mov     cx,ax */
    CX = AX;
L0A82:
    /* 0A82  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0A84:
    /* 0A84  xchg    si,ax */
    { uint16_t t_ = SI;
    SI = AX;
    AX = t_; }
L0A85:
    /* 0A85  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L0A87:
    /* 0A87  xchg    si,ax */
    { uint16_t t_ = SI;
    SI = AX;
    AX = t_; }
L0A88:
    /* 0A88  jmp     L0A68 */
    goto L0A68;
L0A8A: /* L0A8A */
    /* 0A8A  pop     ds */
    SET_DS(pop16());
L0A8B:
    /* 0A8B  pop     es */
    SET_ES(pop16());
L0A8C:
    /* 0A8C  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0A8D: /* _seg003_A8D */
    /* 0A8D  push    es */
    push16(asm_es);
L0A8E:
    /* 0A8E  push    ds */
    push16(asm_ds);
L0A8F:
    /* 0A8F  mov     es,word ptr ss:[95Ah] */
    SET_ES(rw(pSS, 0x95A));
L0A94:
    /* 0A94  mov     ds,word ptr ss:[55EAh] */
    SET_DS(rw(pSS, 0x55EA));
L0A99: /* L0A99 */
    /* 0A99  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0A9B:
    /* 0A9B  shl     ax,1 */
    AX = shl16(AX, 1);
L0A9D:
    /* 0A9D  jb      L0AC5 */
    if (CF) goto L0AC5;
L0A9F:
    /* 0A9F  mov     bx,ax */
    BX = AX;
L0AA1:
    /* 0AA1  mov     di,word ptr ss:[bx+95Eh] */
    DI = rw(pSS, BX + 0x95E);
L0AA6:
    /* 0AA6  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0AA8:
    /* 0AA8  add     di,ax */
    DI = (uint16_t)(DI + AX);
L0AAA:
    /* 0AAA  mov     cx,ax */
    CX = AX;
L0AAC:
    /* 0AAC  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0AAE:
    /* 0AAE  sub     ax,cx */
    AX = (uint16_t)(AX - CX);
L0AB0:
    /* 0AB0  inc     ax */
    AX = (uint16_t)(AX + 1);
L0AB1:
    /* 0AB1  mov     cx,ax */
    CX = AX;
L0AB3:
    /* 0AB3  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0AB5:
    /* 0AB5  push    si */
    push16(SI);
L0AB6:
    /* 0AB6  mov     si,ax */
    SI = AX;
L0AB8: /* L0AB8 */
    /* 0AB8  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0AB9:
    /* 0AB9  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L0ABB:
    /* 0ABB  je      L0ABF */
    if (ZF) goto L0ABF;
L0ABD:
    /* 0ABD  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0ABE:
    /* 0ABE  dec     di */
    DI = (uint16_t)(DI - 1);
L0ABF: /* L0ABF */
    /* 0ABF  inc     di */
    DI = (uint16_t)(DI + 1);
L0AC0:
    /* 0AC0  loop    L0AB8 */
    if (--CX) goto L0AB8;
L0AC2:
    /* 0AC2  pop     si */
    SI = pop16();
L0AC3:
    /* 0AC3  jmp     L0A99 */
    goto L0A99;
L0AC5: /* L0AC5 */
    /* 0AC5  pop     ds */
    SET_DS(pop16());
L0AC6:
    /* 0AC6  pop     es */
    SET_ES(pop16());
L0AC7:
    /* 0AC7  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_AC8  (+AC8)
       cDimFB (FM Towns; far): remap every frame-buffer pixel through row CL of lightabs
       (seg004:696E, PGCACHE.ASM), so the whole view darkens by that distance shade (CH and CL are
       swapped to index by row). */
L0AC8: /* _seg003_AC8 */
    /* 0AC8  push    ds */
    push16(asm_ds);
L0AC9:
    /* 0AC9  push    es */
    push16(asm_es);
L0ACA:
    /* 0ACA  mov     bx,seg _stdat */
    BX = (uint16_t)(0x3F4B + PORT_LOAD_SEG);
L0ACD:
    /* 0ACD  mov     ds,bx */
    SET_DS(BX);
L0ACF:
    /* 0ACF  mov     bx,seg seg004 */
    BX = (uint16_t)(0x06E7 + PORT_LOAD_SEG);
L0AD2:
    /* 0AD2  mov     es,bx */
    SET_ES(BX);
L0AD4:
    /* 0AD4  xchg    ch,cl */
    { uint8_t t_ = CL;
    CL = CH;
    CH = t_; }
L0AD6:
    /* 0AD6  mov     si,cx */
    SI = CX;
L0AD8:
    /* 0AD8  mov     di,4D7Eh */
    DI = 0x4D7E;
L0ADB:
    /* 0ADB  xor     bx,bx */
    BX = logic16((uint16_t)(BX ^ BX));
L0ADD: /* L0ADD */
    /* 0ADD  mov     bl,byte ptr [di] */
    BL = rb(pDS, DI);
L0ADF:
    /* 0ADF  mov     bl,byte ptr es:[bx+si+696Eh] */
    BL = rb(pES, BX + SI + 0x696E);
L0AE4:
    /* 0AE4  mov     byte ptr [di],bl */
    wb(pDS, DI, BL);
L0AE6:
    /* 0AE6  dec     di */
    DI = dec16(DI);
L0AE7:
    /* 0AE7  jne     L0ADD */
    if (!ZF) goto L0ADD;
L0AE9:
    /* 0AE9  pop     es */
    SET_ES(pop16());
L0AEA:
    /* 0AEA  pop     ds */
    SET_DS(pop16());
L0AEB:
    /* 0AEB  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_AEC  (+AEC)
       cLiteFB (FM Towns; far): remap every frame-buffer pixel through the colour table at cXfer+100h
       (SCALEBM.ASM), CX times. L0B08 and L0B0A hold the caller's stack for cFBtoScreen. */
L0AEC: /* _seg003_AEC */
    /* 0AEC  push    ds */
    push16(asm_ds);
L0AED:
    /* 0AED  mov     bx,seg _stdat */
    BX = (uint16_t)(0x3F4B + PORT_LOAD_SEG);
L0AF0:
    /* 0AF0  mov     ds,bx */
    SET_DS(BX);
L0AF2: /* L0AF2 */
    /* 0AF2  mov     si,4CCEh */
    SI = 0x4CCE;
L0AF5:
    /* 0AF5  xor     bx,bx */
    BX = logic16((uint16_t)(BX ^ BX));
L0AF7: /* L0AF7 */
    /* 0AF7  mov     bl,byte ptr [si] */
    BL = rb(pDS, SI);
L0AF9:
    /* 0AF9  mov     bl,byte ptr cs:_cXfer[bx+100h] */
    BL = rb(CODE003, BX + 0x1301);
L0AFE:
    /* 0AFE  mov     byte ptr [si],bl */
    wb(pDS, SI, BL);
L0B00:
    /* 0B00  dec     si */
    SI = dec16(SI);
L0B01:
    /* 0B01  jne     L0AF7 */
    if (!ZF) goto L0AF7;
L0B03:
    /* 0B03  dec     cx */
    CX = dec16(CX);
L0B04:
    /* 0B04  jne     L0AF2 */
    if (!ZF) goto L0AF2;
L0B06:
    /* 0B06  pop     ds */
    SET_DS(pop16());
L0B07:
    /* 0B07  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_B0C  (+B0C)
       setup_frame_buf (FM Towns; far, cPlaceFB): BX = the frame's width, CX = its height. Builds the
       row offset table at 095E (rows of width + 2 bytes from offset 2; rows past the height all
       point at 4CD0h), records the last row in 0AF2, and patches the unrolled copier _BEC so that it
       copies exactly one plane's share of the width: the old ret becomes a movsb again and a ret
       (C3h) is written after the new width, for widths below 320. */
L0B0C: /* _seg003_B0C */
    /* 0B0C  add     bx,2 */
    BX = (uint16_t)(BX + 0x2);
L0B0F:
    /* 0B0F  mov     di,95Eh */
    DI = 0x95E;
L0B12:
    /* 0B12  mov     dx,cx */
    DX = CX;
L0B14:
    /* 0B14  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L0B16:
    /* 0B16  add     ax,2 */
    AX = (uint16_t)(AX + 0x2);
L0B19: /* L0B19 */
    /* 0B19  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0B1A:
    /* 0B1A  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L0B1C:
    /* 0B1C  loop    L0B19 */
    if (--CX) goto L0B19;
L0B1E:
    /* 0B1E  mov     ax,4CD0h */
    AX = 0x4CD0;
L0B21:
    /* 0B21  mov     cx,0C8h */
    CX = 0xC8;
L0B24:
    /* 0B24  sub     cx,dx */
    CX = (uint16_t)(CX - DX);
L0B26:
    /* 0B26  dec     dx */
    DX = (uint16_t)(DX - 1);
L0B27:
    /* 0B27  mov     word ptr ds:[0AF2h],dx */
    ww(pDS, 0xAF2, DX);
L0B2B:
    /* 0B2B  rep stosw */
    while (CX) { ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0B2D:
    /* 0B2D  cmp     bx,word ptr ds:[0AF0h] */
    sub16(BX, rw(pDS, 0xAF0), 0);
L0B31:
    /* 0B31  je      L0B5C */
    if (ZF) goto L0B5C;
L0B33:
    /* 0B33  cmp     word ptr ds:[0AF0h],140h */
    sub16(rw(pDS, 0xAF0), 0x140, 0);
L0B39:
    /* 0B39  jge     L0B48 */
    if (SF == OF) goto L0B48;
L0B3B:
    /* 0B3B  mov     di,offset _seg003_BEC */
    DI = 0xBEC;
L0B3E:
    /* 0B3E  mov     dx,word ptr ds:[0AF0h] */
    DX = rw(pDS, 0xAF0);
L0B42:
    /* 0B42  add     di,dx */
    DI = (uint16_t)(DI + DX);
L0B44:
    /* 0B44  mov     byte ptr cs:[di],0A4h */
    wb(CODE003, DI, 0xA4);
L0B48: /* L0B48 */
    /* 0B48  sub     bx,2 */
    BX = (uint16_t)(BX - 0x2);
L0B4B:
    /* 0B4B  cmp     bx,140h */
    sub16(BX, 0x140, 0);
L0B4F:
    /* 0B4F  jge     L0B5C */
    if (SF == OF) goto L0B5C;
L0B51:
    /* 0B51  mov     di,offset _seg003_BEC */
    DI = 0xBEC;
L0B54:
    /* 0B54  mov     byte ptr cs:[bx+di],0C3h */
    wb(CODE003, BX + DI, 0xC3);
L0B58:
    /* 0B58  mov     word ptr ds:[0AF0h],bx */
    ww(pDS, 0xAF0, BX);
L0B5C: /* L0B5C */
    /* 0B5C  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* DRAW_RELATED_seg003_B5D  (+B5D)
       cFBtoScreen (FM Towns; far): copy the frame buffer to the screen at A000:[950] + [3838] (the
       page being drawn). Switches to the library's stack, then for each row from the last (the top
       line of the screen, since y counts up) down to the window's bottom edge (3DF8), four passes,
       one per plane (map masks 8, 2, 4, 1 with source offsets 3, 1, 2, 0), each through the unrolled
       copier _BEC; screen rows are 80 bytes apart. */
L0B5D: /* _seg003_B5D */
    /* 0B5D  push    bp */
    push16(BP);
L0B5E:
    /* 0B5E  push    es */
    push16(asm_es);
L0B5F:
    /* 0B5F  push    ds */
    push16(asm_ds);
L0B60:
    /* 0B60  push    si */
    push16(SI);
L0B61:
    /* 0B61  push    di */
    push16(DI);
L0B62:
    /* 0B62  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L0B65:
    /* 0B65  mov     ds,ax */
    SET_DS(AX);
L0B67:
    /* 0B67  mov     es,ax */
    SET_ES(AX);
L0B69:
    /* 0B69  mov     bx,ss */
    BX = asm_ss;
L0B6B:
    /* 0B6B  mov     word ptr cs:L0B08,bx */
    ww(CODE003, 0xB08, BX);
L0B70:
    /* 0B70  mov     word ptr cs:L0B0A,sp */
    ww(CODE003, 0xB0A, SP);
L0B75:
    /* 0B75  cli */
    ;
L0B76:
    /* 0B76  mov     ss,ax */
    SET_SS(AX);
L0B78:
    /* 0B78  mov     sp,word ptr ds:[5586h] */
    SP = rw(pDS, 0x5586);
L0B7C:
    /* 0B7C  sti */
    ;
L0B7D:
    /* 0B7D  mov     ax,0A000h */
    AX = 0xA000;
L0B80:
    /* 0B80  mov     es,ax */
    SET_ES(AX);
L0B82:
    /* 0B82  mov     bp,word ptr ds:[0AF2h] */
    BP = rw(pDS, 0xAF2);
L0B86:
    /* 0B86  mov     dx,SC_DATA */
    DX = 0x3C5;
L0B89:
    /* 0B89  mov     bx,word ptr ds:[950h] */
    BX = rw(pDS, 0x950);
L0B8D:
    /* 0B8D  mov     ax,word ptr ds:[3838h] */
    AX = rw(pDS, 0x3838);
L0B90:
    /* 0B90  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L0B92:
    /* 0B92  mov     ds,word ptr ds:[95Ah] */
    SET_DS(rw(pDS, 0x95A));
L0B96: /* L0B96 */
    /* 0B96  shl     bp,1 */
    BP = (uint16_t)(BP << 1);
L0B98:
    /* 0B98  mov     cx,word ptr [bp+95Eh] */
    CX = rw(pSS, BP + 0x95E);
L0B9C:
    /* 0B9C  mov     al,8 */
    AL = 0x8;
L0B9E:
    /* 0B9E  out     dx,al */
    asm_out8(DX, AL);
L0B9F:
    /* 0B9F  mov     di,bx */
    DI = BX;
L0BA1:
    /* 0BA1  mov     si,3 */
    SI = 0x3;
L0BA4:
    /* 0BA4  add     si,cx */
    SI = (uint16_t)(SI + CX);
L0BA6:
    /* 0BA6  call    _seg003_BEC */
    if ((c = asm_call(ASM_JMP(0x0090, 0x0BEC), 0x0BA9)) != 0) return c;
L0BA9:
    /* 0BA9  mov     al,2 */
    AL = 0x2;
L0BAB:
    /* 0BAB  out     dx,al */
    asm_out8(DX, AL);
L0BAC:
    /* 0BAC  mov     di,bx */
    DI = BX;
L0BAE:
    /* 0BAE  mov     si,1 */
    SI = 0x1;
L0BB1:
    /* 0BB1  add     si,cx */
    SI = (uint16_t)(SI + CX);
L0BB3:
    /* 0BB3  call    _seg003_BEC */
    if ((c = asm_call(ASM_JMP(0x0090, 0x0BEC), 0x0BB6)) != 0) return c;
L0BB6:
    /* 0BB6  mov     al,4 */
    AL = 0x4;
L0BB8:
    /* 0BB8  out     dx,al */
    asm_out8(DX, AL);
L0BB9:
    /* 0BB9  mov     di,bx */
    DI = BX;
L0BBB:
    /* 0BBB  mov     si,2 */
    SI = 0x2;
L0BBE:
    /* 0BBE  add     si,cx */
    SI = (uint16_t)(SI + CX);
L0BC0:
    /* 0BC0  call    _seg003_BEC */
    if ((c = asm_call(ASM_JMP(0x0090, 0x0BEC), 0x0BC3)) != 0) return c;
L0BC3:
    /* 0BC3  mov     al,1 */
    AL = 0x1;
L0BC5:
    /* 0BC5  out     dx,al */
    asm_out8(DX, AL);
L0BC6:
    /* 0BC6  mov     di,bx */
    DI = BX;
L0BC8:
    /* 0BC8  mov     si,cx */
    SI = CX;
L0BCA:
    /* 0BCA  call    _seg003_BEC */
    if ((c = asm_call(ASM_JMP(0x0090, 0x0BEC), 0x0BCD)) != 0) return c;
L0BCD:
    /* 0BCD  add     bx,50h */
    BX = (uint16_t)(BX + 0x50);
L0BD0:
    /* 0BD0  shr     bp,1 */
    BP = (uint16_t)(BP >> 1);
L0BD2:
    /* 0BD2  dec     bp */
    BP = (uint16_t)(BP - 1);
L0BD3:
    /* 0BD3  cmp     bp,word ptr ss:[3DF8h] */
    sub16(BP, rw(pSS, 0x3DF8), 0);
L0BD8:
    /* 0BD8  jg      L0B96 */
    if (!ZF && SF == OF) goto L0B96;
L0BDA:
    /* 0BDA  cli */
    ;
L0BDB:
    /* 0BDB  mov     ss,word ptr cs:L0B08 */
    SET_SS(rw(CODE003, 0xB08));
L0BE0:
    /* 0BE0  mov     sp,word ptr cs:L0B0A */
    SP = rw(CODE003, 0xB0A);
L0BE5:
    /* 0BE5  sti */
    ;
L0BE6:
    /* 0BE6  pop     di */
    DI = pop16();
L0BE7:
    /* 0BE7  pop     si */
    SI = pop16();
L0BE8:
    /* 0BE8  pop     ds */
    SET_DS(pop16());
L0BE9:
    /* 0BE9  pop     es */
    SET_ES(pop16());
L0BEA:
    /* 0BEA  pop     bp */
    BP = pop16();
L0BEB:
    /* 0BEB  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_BEC  (+BEC)
       _BEC: copy every fourth byte of a frame-buffer row to the screen (movsb; add si,3, unrolled
       for 320 pixels; setup_frame_buf patches in the ret). */
L0BEC: /* _seg003_BEC */
    /* 0BEC  movsb */
    /* by hand: _BEC, the unrolled copier (movsb; add si,3 for 320 pixels): setup_frame_buf (_B0C) writes a ret (C3h) at _BEC + the frame's width and puts back the movsb (A4h) the last one replaced, so the copier runs from the code block until its ret (UW2's cPlaceFB set-up does the same) */
    { uint16_t p_ = 0x0BEC;
      for (;;) {
        uint8_t op_ = CODE003[p_];
        if (op_ == 0xC3) { SP = (uint16_t)(SP + 2); return ASM_RET; }
        if (op_ == 0xA4) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); p_++; }
        else if (op_ == 0x83 && CODE003[p_ + 1] == 0xC6) { SI = (uint16_t)(SI + (int8_t)CODE003[p_ + 2]); p_ += 3; }
        else port_halt("GRENTRY _BEC: an instruction the copier does not have");
      } }
L0BED:
    /* 0BED  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0BF0:
    /* 0BF0  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0BF1:
    /* 0BF1  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0BF4:
    /* 0BF4  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0BF5:
    /* 0BF5  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0BF8:
    /* 0BF8  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0BF9:
    /* 0BF9  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0BFC:
    /* 0BFC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0BFD:
    /* 0BFD  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C00:
    /* 0C00  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C01:
    /* 0C01  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C04:
    /* 0C04  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C05:
    /* 0C05  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C08:
    /* 0C08  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C09:
    /* 0C09  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C0C:
    /* 0C0C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C0D:
    /* 0C0D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C10:
    /* 0C10  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C11:
    /* 0C11  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C14:
    /* 0C14  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C15:
    /* 0C15  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C18:
    /* 0C18  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C19:
    /* 0C19  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C1C:
    /* 0C1C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C1D:
    /* 0C1D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C20:
    /* 0C20  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C21:
    /* 0C21  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C24:
    /* 0C24  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C25:
    /* 0C25  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C28:
    /* 0C28  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C29:
    /* 0C29  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C2C:
    /* 0C2C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C2D:
    /* 0C2D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C30:
    /* 0C30  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C31:
    /* 0C31  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C34:
    /* 0C34  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C35:
    /* 0C35  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C38:
    /* 0C38  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C39:
    /* 0C39  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C3C:
    /* 0C3C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C3D:
    /* 0C3D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C40:
    /* 0C40  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C41:
    /* 0C41  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C44:
    /* 0C44  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C45:
    /* 0C45  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C48:
    /* 0C48  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C49:
    /* 0C49  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C4C:
    /* 0C4C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C4D:
    /* 0C4D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C50:
    /* 0C50  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C51:
    /* 0C51  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C54:
    /* 0C54  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C55:
    /* 0C55  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C58:
    /* 0C58  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C59:
    /* 0C59  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C5C:
    /* 0C5C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C5D:
    /* 0C5D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C60:
    /* 0C60  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C61:
    /* 0C61  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C64:
    /* 0C64  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C65:
    /* 0C65  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C68:
    /* 0C68  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C69:
    /* 0C69  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C6C:
    /* 0C6C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C6D:
    /* 0C6D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C70:
    /* 0C70  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C71:
    /* 0C71  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C74:
    /* 0C74  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C75:
    /* 0C75  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C78:
    /* 0C78  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C79:
    /* 0C79  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C7C:
    /* 0C7C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C7D:
    /* 0C7D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C80:
    /* 0C80  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C81:
    /* 0C81  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C84:
    /* 0C84  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C85:
    /* 0C85  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C88:
    /* 0C88  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C89:
    /* 0C89  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C8C:
    /* 0C8C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C8D:
    /* 0C8D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C90:
    /* 0C90  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C91:
    /* 0C91  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C94:
    /* 0C94  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C95:
    /* 0C95  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C98:
    /* 0C98  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C99:
    /* 0C99  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0C9C:
    /* 0C9C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0C9D:
    /* 0C9D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CA0:
    /* 0CA0  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CA1:
    /* 0CA1  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CA4:
    /* 0CA4  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CA5:
    /* 0CA5  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CA8:
    /* 0CA8  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CA9:
    /* 0CA9  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CAC:
    /* 0CAC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CAD:
    /* 0CAD  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CB0:
    /* 0CB0  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CB1:
    /* 0CB1  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CB4:
    /* 0CB4  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CB5:
    /* 0CB5  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CB8:
    /* 0CB8  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CB9:
    /* 0CB9  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CBC:
    /* 0CBC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CBD:
    /* 0CBD  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CC0:
    /* 0CC0  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CC1:
    /* 0CC1  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CC4:
    /* 0CC4  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CC5:
    /* 0CC5  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CC8:
    /* 0CC8  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CC9:
    /* 0CC9  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CCC:
    /* 0CCC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CCD:
    /* 0CCD  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CD0:
    /* 0CD0  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CD1:
    /* 0CD1  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CD4:
    /* 0CD4  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CD5:
    /* 0CD5  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CD8:
    /* 0CD8  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CD9:
    /* 0CD9  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CDC:
    /* 0CDC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CDD:
    /* 0CDD  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CE0:
    /* 0CE0  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CE1:
    /* 0CE1  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CE4:
    /* 0CE4  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CE5:
    /* 0CE5  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CE8:
    /* 0CE8  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CE9:
    /* 0CE9  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CEC:
    /* 0CEC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CED:
    /* 0CED  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CF0:
    /* 0CF0  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CF1:
    /* 0CF1  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CF4:
    /* 0CF4  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CF5:
    /* 0CF5  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CF8:
    /* 0CF8  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CF9:
    /* 0CF9  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0CFC:
    /* 0CFC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0CFD:
    /* 0CFD  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0D00:
    /* 0D00  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0D01:
    /* 0D01  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0D04:
    /* 0D04  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0D05:
    /* 0D05  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0D08:
    /* 0D08  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0D09:
    /* 0D09  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0D0C:
    /* 0D0C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0D0D:
    /* 0D0D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0D10:
    /* 0D10  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0D11:
    /* 0D11  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0D14:
    /* 0D14  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0D15:
    /* 0D15  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0D18:
    /* 0D18  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0D19:
    /* 0D19  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0D1C:
    /* 0D1C  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0D1D:
    /* 0D1D  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0D20:
    /* 0D20  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0D21:
    /* 0D21  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0D24:
    /* 0D24  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0D25:
    /* 0D25  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0D28:
    /* 0D28  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0D29:
    /* 0D29  add     si,3 */
    SI = add16(SI, 0x3, 0);
L0D2C:
    /* 0D2C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_D2D  (+D2D)
       fake_fb_solid_ylr (FM Towns): the solid span writer for the frame buffer: fill each 6-byte
       span record (y, left, right; either order) with the colour in 410F. */
L0D2D: /* _seg003_D2D */
    /* 0D2D  push    es */
    push16(asm_es);
L0D2E:
    /* 0D2E  mov     bx,word ptr ds:[WIN_LEFT] */
    BX = rw(pDS, 0x3DF2);
L0D32:
    /* 0D32  mov     es,word ptr ds:[95Ah] */
    SET_ES(rw(pDS, 0x95A));
L0D36: /* L0D36 */
    /* 0D36  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0D37:
    /* 0D37  shl     ax,1 */
    AX = shl16(AX, 1);
L0D39:
    /* 0D39  jb      L0D55 */
    if (CF) goto L0D55;
L0D3B:
    /* 0D3B  mov     di,ax */
    DI = AX;
L0D3D:
    /* 0D3D  mov     di,word ptr [di+95Eh] */
    DI = rw(pDS, DI + 0x95E);
L0D41:
    /* 0D41  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0D42:
    /* 0D42  mov     cx,ax */
    CX = AX;
L0D44:
    /* 0D44  add     di,ax */
    DI = (uint16_t)(DI + AX);
L0D46:
    /* 0D46  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0D47:
    /* 0D47  sub     cx,ax */
    CX = (uint16_t)(CX - AX);
L0D49:
    /* 0D49  neg     cx */
    CX = (uint16_t)-CX;
L0D4B:
    /* 0D4B  mov     al,byte ptr ds:[PEN_COLOR] */
    AL = rb(pDS, 0x410F);
L0D4E:
    /* 0D4E  inc     cx */
    CX = inc16(CX);
L0D4F:
    /* 0D4F  js      L0D57 */
    if (SF) goto L0D57;
L0D51:
    /* 0D51  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L0D53:
    /* 0D53  jmp     L0D36 */
    goto L0D36;
L0D55: /* L0D55 */
    /* 0D55  pop     es */
    SET_ES(pop16());
L0D56:
    /* 0D56  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0D57: /* L0D57 */
    /* 0D57  add     di,cx */
    DI = (uint16_t)(DI + CX);
L0D59:
    /* 0D59  dec     cx */
    CX = (uint16_t)(CX - 1);
L0D5A:
    /* 0D5A  neg     cx */
    CX = (uint16_t)-CX;
L0D5C:
    /* 0D5C  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L0D5E:
    /* 0D5E  jmp     L0D36 */
    goto L0D36;

    /* seg003_D60  (+D60)
       fbuf_save_vylr (FM Towns): copy frame-buffer spans (6-byte records, widened to whole groups of
       four pixels) to video memory at the offset in 4114, one plane at a time. Probably the save
       half of VALLOC.ASM's save_rect when the frame buffer is the target. */
L0D60: /* _seg003_D60 */
    /* 0D60  mov     dx,SC_DATA */
    DX = 0x3C5;
L0D63:
    /* 0D63  mov     bp,si */
    BP = SI;
L0D65:
    /* 0D65  mov     di,word ptr ds:[PEN_WORD2] */
    DI = rw(pDS, 0x4114);
L0D69:
    /* 0D69  push    ds */
    push16(asm_ds);
L0D6A:
    /* 0D6A  push    es */
    push16(asm_es);
L0D6B:
    /* 0D6B  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L0D6F:
    /* 0D6F  mov     ds,word ptr ds:[95Ah] */
    SET_DS(rw(pDS, 0x95A));
L0D73: /* L0D73 */
    /* 0D73  mov     bx,word ptr [bp] */
    BX = rw(pSS, BP);
L0D76:
    /* 0D76  shl     bx,1 */
    BX = shl16(BX, 1);
L0D78:
    /* 0D78  jb      L0DE0 */
    if (CF) goto L0DE0;
L0D7A:
    /* 0D7A  mov     si,word ptr [bp+2] */
    SI = rw(pSS, BP + 0x2);
L0D7D:
    /* 0D7D  and     si,-4 */
    SI = (uint16_t)(SI & 0xFFFC);
L0D80:
    /* 0D80  mov     cx,word ptr [bp+4] */
    CX = rw(pSS, BP + 0x4);
L0D83:
    /* 0D83  and     cx,-4 */
    CX = (uint16_t)(CX & 0xFFFC);
L0D86:
    /* 0D86  add     cx,4 */
    CX = (uint16_t)(CX + 0x4);
L0D89:
    /* 0D89  add     bp,6 */
    BP = (uint16_t)(BP + 0x6);
L0D8C:
    /* 0D8C  sub     cx,si */
    CX = (uint16_t)(CX - SI);
L0D8E:
    /* 0D8E  add     si,word ptr ss:[bx+95Eh] */
    SI = (uint16_t)(SI + rw(pSS, BX + 0x95E));
L0D93:
    /* 0D93  mov     bx,cx */
    BX = CX;
L0D95:
    /* 0D95  push    bp */
    push16(BP);
L0D96:
    /* 0D96  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0D99:
    /* 0D99  mov     bp,di */
    BP = DI;
L0D9B:
    /* 0D9B  mov     al,1 */
    AL = 0x1;
L0D9D:
    /* 0D9D  out     dx,al */
    asm_out8(DX, AL);
L0D9E: /* L0D9E */
    /* 0D9E  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0D9F:
    /* 0D9F  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0DA2:
    /* 0DA2  loop    L0D9E */
    if (--CX) goto L0D9E;
L0DA4:
    /* 0DA4  sub     si,bx */
    SI = (uint16_t)(SI - BX);
L0DA6:
    /* 0DA6  inc     si */
    SI = (uint16_t)(SI + 1);
L0DA7:
    /* 0DA7  mov     cx,bx */
    CX = BX;
L0DA9:
    /* 0DA9  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0DAC:
    /* 0DAC  mov     di,bp */
    DI = BP;
L0DAE:
    /* 0DAE  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L0DB0:
    /* 0DB0  out     dx,al */
    asm_out8(DX, AL);
L0DB1: /* L0DB1 */
    /* 0DB1  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0DB2:
    /* 0DB2  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0DB5:
    /* 0DB5  loop    L0DB1 */
    if (--CX) goto L0DB1;
L0DB7:
    /* 0DB7  sub     si,bx */
    SI = (uint16_t)(SI - BX);
L0DB9:
    /* 0DB9  inc     si */
    SI = (uint16_t)(SI + 1);
L0DBA:
    /* 0DBA  mov     cx,bx */
    CX = BX;
L0DBC:
    /* 0DBC  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0DBF:
    /* 0DBF  mov     di,bp */
    DI = BP;
L0DC1:
    /* 0DC1  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L0DC3:
    /* 0DC3  out     dx,al */
    asm_out8(DX, AL);
L0DC4: /* L0DC4 */
    /* 0DC4  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0DC5:
    /* 0DC5  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0DC8:
    /* 0DC8  loop    L0DC4 */
    if (--CX) goto L0DC4;
L0DCA:
    /* 0DCA  sub     si,bx */
    SI = (uint16_t)(SI - BX);
L0DCC:
    /* 0DCC  inc     si */
    SI = (uint16_t)(SI + 1);
L0DCD:
    /* 0DCD  mov     cx,bx */
    CX = BX;
L0DCF:
    /* 0DCF  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0DD2:
    /* 0DD2  mov     di,bp */
    DI = BP;
L0DD4:
    /* 0DD4  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L0DD6:
    /* 0DD6  out     dx,al */
    asm_out8(DX, AL);
L0DD7: /* L0DD7 */
    /* 0DD7  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0DD8:
    /* 0DD8  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0DDB:
    /* 0DDB  loop    L0DD7 */
    if (--CX) goto L0DD7;
L0DDD:
    /* 0DDD  pop     bp */
    BP = pop16();
L0DDE:
    /* 0DDE  jmp     L0D73 */
    goto L0D73;
L0DE0: /* L0DE0 */
    /* 0DE0  pop     es */
    SET_ES(pop16());
L0DE1:
    /* 0DE1  pop     ds */
    SET_DS(pop16());
L0DE2:
    /* 0DE2  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_DE3  (+DE3)
       fbcopy_vylrpp (FM Towns): copy frame-buffer rows to the planar screen (8-byte records: y,
       left, right, destination offset), plane by plane with the edge masks from 3B62 and 3CA6;
       VIDMODE's vcopyfb uses it. Ends by setting the bit mask back to 0FFh. */
L0DE3: /* _seg003_DE3 */
    /* 0DE3  push    ds */
    push16(asm_ds);
L0DE4:
    /* 0DE4  push    es */
    push16(asm_es);
L0DE5:
    /* 0DE5  mov     es,word ptr ds:[SCREEN_SEG] */
    SET_ES(rw(pDS, 0x3DFA));
L0DE9:
    /* 0DE9  mov     ds,word ptr ds:[95Ah] */
    SET_DS(rw(pDS, 0x95A));
L0DED:
    /* 0DED  mov     bp,si */
    BP = SI;
L0DEF: /* L0DEF */
    /* 0DEF  mov     bx,word ptr [bp] */
    BX = rw(pSS, BP);
L0DF2:
    /* 0DF2  shl     bx,1 */
    BX = shl16(BX, 1);
L0DF4:
    /* 0DF4  jae     L0DF9 */
    if (!CF) goto L0DF9;
L0DF6:
    /* 0DF6  jmp     L0EF0 */
    goto L0EF0;
L0DF9: /* L0DF9 */
    /* 0DF9  mov     si,word ptr ss:[bx+95Eh] */
    SI = rw(pSS, BX + 0x95E);
L0DFE:
    /* 0DFE  mov     bx,word ptr [bp+2] */
    BX = rw(pSS, BP + 0x2);
L0E01:
    /* 0E01  mov     cx,word ptr [bp+4] */
    CX = rw(pSS, BP + 0x4);
L0E04:
    /* 0E04  mov     di,word ptr [bp+6] */
    DI = rw(pSS, BP + 0x6);
L0E07:
    /* 0E07  add     si,bx */
    SI = (uint16_t)(SI + BX);
L0E09:
    /* 0E09  add     bp,8 */
    BP = (uint16_t)(BP + 0x8);
L0E0C:
    /* 0E0C  push    bp */
    push16(BP);
L0E0D:
    /* 0E0D  sub     cx,bx */
    CX = (uint16_t)(CX - BX);
L0E0F:
    /* 0E0F  inc     cx */
    CX = (uint16_t)(CX + 1);
L0E10:
    /* 0E10  mov     bp,cx */
    BP = CX;
L0E12:
    /* 0E12  mov     dx,SC_DATA */
    DX = 0x3C5;
L0E15:
    /* 0E15  mov     al,byte ptr ss:[bx+3B62h] */
    AL = rb(pSS, BX + 0x3B62);
L0E1A:
    /* 0E1A  and     al,byte ptr ss:[bx+3CA6h] */
    AL = (uint8_t)(AL & rb(pSS, BX + 0x3CA6));
L0E1F:
    /* 0E1F  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0E22:
    /* 0E22  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0E25:
    /* 0E25  out     dx,al */
    asm_out8(DX, AL);
L0E26: /* L0E26 */
    /* 0E26  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0E27:
    /* 0E27  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0E2A:
    /* 0E2A  loop    L0E26 */
    if (--CX) goto L0E26;
L0E2C:
    /* 0E2C  mov     cx,bp */
    CX = BP;
L0E2E:
    /* 0E2E  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0E31:
    /* 0E31  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0E34:
    /* 0E34  sub     di,cx */
    DI = (uint16_t)(DI - CX);
L0E36:
    /* 0E36  shl     cx,2 */
    CX = (uint16_t)(CX << 2);
L0E39:
    /* 0E39  sub     si,cx */
    SI = (uint16_t)(SI - CX);
L0E3B:
    /* 0E3B  inc     si */
    SI = (uint16_t)(SI + 1);
L0E3C:
    /* 0E3C  inc     bx */
    BX = (uint16_t)(BX + 1);
L0E3D:
    /* 0E3D  mov     cx,bx */
    CX = BX;
L0E3F:
    /* 0E3F  and     cx,3 */
    CX = logic16((uint16_t)(CX & 0x3));
L0E42:
    /* 0E42  jne     L0E45 */
    if (!ZF) goto L0E45;
L0E44:
    /* 0E44  inc     di */
    DI = (uint16_t)(DI + 1);
L0E45: /* L0E45 */
    /* 0E45  dec     bp */
    BP = dec16(BP);
L0E46:
    /* 0E46  jne     L0E4B */
    if (!ZF) goto L0E4B;
L0E48:
    /* 0E48  jmp     L0EEC */
    goto L0EEC;
L0E4B: /* L0E4B */
    /* 0E4B  mov     cx,bp */
    CX = BP;
L0E4D:
    /* 0E4D  mov     al,byte ptr ss:[bx+3B62h] */
    AL = rb(pSS, BX + 0x3B62);
L0E52:
    /* 0E52  and     al,byte ptr ss:[bx+3CA6h] */
    AL = (uint8_t)(AL & rb(pSS, BX + 0x3CA6));
L0E57:
    /* 0E57  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0E5A:
    /* 0E5A  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0E5D:
    /* 0E5D  out     dx,al */
    asm_out8(DX, AL);
L0E5E: /* L0E5E */
    /* 0E5E  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0E5F:
    /* 0E5F  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0E62:
    /* 0E62  loop    L0E5E */
    if (--CX) goto L0E5E;
L0E64:
    /* 0E64  mov     cx,bp */
    CX = BP;
L0E66:
    /* 0E66  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0E69:
    /* 0E69  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0E6C:
    /* 0E6C  sub     di,cx */
    DI = (uint16_t)(DI - CX);
L0E6E:
    /* 0E6E  shl     cx,2 */
    CX = (uint16_t)(CX << 2);
L0E71:
    /* 0E71  sub     si,cx */
    SI = (uint16_t)(SI - CX);
L0E73:
    /* 0E73  inc     si */
    SI = (uint16_t)(SI + 1);
L0E74:
    /* 0E74  inc     bx */
    BX = (uint16_t)(BX + 1);
L0E75:
    /* 0E75  mov     cx,bx */
    CX = BX;
L0E77:
    /* 0E77  and     cx,3 */
    CX = logic16((uint16_t)(CX & 0x3));
L0E7A:
    /* 0E7A  jne     L0E7D */
    if (!ZF) goto L0E7D;
L0E7C:
    /* 0E7C  inc     di */
    DI = (uint16_t)(DI + 1);
L0E7D: /* L0E7D */
    /* 0E7D  dec     bp */
    BP = dec16(BP);
L0E7E:
    /* 0E7E  je      L0EEC */
    if (ZF) goto L0EEC;
L0E80:
    /* 0E80  mov     cx,bp */
    CX = BP;
L0E82:
    /* 0E82  mov     al,byte ptr ss:[bx+3B62h] */
    AL = rb(pSS, BX + 0x3B62);
L0E87:
    /* 0E87  and     al,byte ptr ss:[bx+3CA6h] */
    AL = (uint8_t)(AL & rb(pSS, BX + 0x3CA6));
L0E8C:
    /* 0E8C  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0E8F:
    /* 0E8F  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0E92:
    /* 0E92  out     dx,al */
    asm_out8(DX, AL);
L0E93: /* L0E93 */
    /* 0E93  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0E94:
    /* 0E94  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0E97:
    /* 0E97  loop    L0E93 */
    if (--CX) goto L0E93;
L0E99:
    /* 0E99  mov     cx,bp */
    CX = BP;
L0E9B:
    /* 0E9B  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0E9E:
    /* 0E9E  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0EA1:
    /* 0EA1  sub     di,cx */
    DI = (uint16_t)(DI - CX);
L0EA3:
    /* 0EA3  shl     cx,2 */
    CX = (uint16_t)(CX << 2);
L0EA6:
    /* 0EA6  sub     si,cx */
    SI = (uint16_t)(SI - CX);
L0EA8:
    /* 0EA8  inc     si */
    SI = (uint16_t)(SI + 1);
L0EA9:
    /* 0EA9  inc     bx */
    BX = (uint16_t)(BX + 1);
L0EAA:
    /* 0EAA  mov     cx,bx */
    CX = BX;
L0EAC:
    /* 0EAC  and     cx,3 */
    CX = logic16((uint16_t)(CX & 0x3));
L0EAF:
    /* 0EAF  jne     L0EB2 */
    if (!ZF) goto L0EB2;
L0EB1:
    /* 0EB1  inc     di */
    DI = (uint16_t)(DI + 1);
L0EB2: /* L0EB2 */
    /* 0EB2  dec     bp */
    BP = dec16(BP);
L0EB3:
    /* 0EB3  je      L0EEC */
    if (ZF) goto L0EEC;
L0EB5:
    /* 0EB5  mov     cx,bp */
    CX = BP;
L0EB7:
    /* 0EB7  mov     al,byte ptr ss:[bx+3B62h] */
    AL = rb(pSS, BX + 0x3B62);
L0EBC:
    /* 0EBC  and     al,byte ptr ss:[bx+3CA6h] */
    AL = (uint8_t)(AL & rb(pSS, BX + 0x3CA6));
L0EC1:
    /* 0EC1  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0EC4:
    /* 0EC4  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0EC7:
    /* 0EC7  out     dx,al */
    asm_out8(DX, AL);
L0EC8: /* L0EC8 */
    /* 0EC8  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0EC9:
    /* 0EC9  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0ECC:
    /* 0ECC  loop    L0EC8 */
    if (--CX) goto L0EC8;
L0ECE:
    /* 0ECE  mov     cx,bp */
    CX = BP;
L0ED0:
    /* 0ED0  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0ED3:
    /* 0ED3  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0ED6:
    /* 0ED6  sub     di,cx */
    DI = (uint16_t)(DI - CX);
L0ED8:
    /* 0ED8  shl     cx,2 */
    CX = (uint16_t)(CX << 2);
L0EDB:
    /* 0EDB  sub     si,cx */
    SI = (uint16_t)(SI - CX);
L0EDD:
    /* 0EDD  inc     si */
    SI = (uint16_t)(SI + 1);
L0EDE:
    /* 0EDE  inc     bx */
    BX = (uint16_t)(BX + 1);
L0EDF:
    /* 0EDF  mov     cx,bx */
    CX = BX;
L0EE1:
    /* 0EE1  and     cx,3 */
    CX = logic16((uint16_t)(CX & 0x3));
L0EE4:
    /* 0EE4  jne     L0EE7 */
    if (!ZF) goto L0EE7;
L0EE6:
    /* 0EE6  inc     di */
    DI = (uint16_t)(DI + 1);
L0EE7: /* L0EE7 */
    /* 0EE7  dec     bp */
    BP = dec16(BP);
L0EE8:
    /* 0EE8  je      L0EEC */
    if (ZF) goto L0EEC;
L0EEA:
    /* 0EEA  mov     cx,bp */
    CX = BP;
L0EEC: /* L0EEC */
    /* 0EEC  pop     bp */
    BP = pop16();
L0EED:
    /* 0EED  jmp     L0DEF */
    goto L0DEF;
L0EF0: /* L0EF0 */
    /* 0EF0  mov     dx,GC_INDEX */
    DX = 0x3CE;
L0EF3:
    /* 0EF3  mov     ax,0FF08h */
    AX = 0xFF08;
L0EF6:
    /* 0EF6  out     dx,ax */
    asm_out16(DX, AX);
L0EF7:
    /* 0EF7  pop     es */
    SET_ES(pop16());
L0EF8:
    /* 0EF8  pop     ds */
    SET_DS(pop16());
L0EF9:
    /* 0EF9  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0EFA:
    /* 0EFA  add     sp,2 */
    SP = add16(SP, 0x2, 0);
L0EFD:
    /* 0EFD  jmp     L0EF0 */
    goto L0EF0;

    /* seg003_EFF  (+EFF)
       fbuf_setcolor (FM Towns): AX = a pen number (negative: no change). Sets the colour bytes 410E
       and 4114 from the tables at 21E and 434, and, when the pen's mode (the byte table at
       seg048:0008) is below 14h, the span writer 4110 from the table at 0AF4; so a pen chooses
       both a colour and a way of writing it (solid, copy, save, restore ...).

       After its ret: L0F25 (the current span's y, whose low bit picks the dither phase) and the two
       smooth-span vectors L0F27 and L0F29, which seg004 sets (INTERP.ASM's opcode D6h to 0BC7h and
       0C93h, solid; opcode D8h and SMOOTH.ASM to 0BF2h and 0CD9h, translucent). L0F2B
       (smooth_span_same, solid, both ends the same shade) draws the span in one colour,
       lightabs[shade][base colour] with the base colour at seg004:6950 (the byte PGCACHE.ASM keeps
       for seg003; UW2 6D20), through fake_fb_solid_ylr. The code at 0BF2 (trans_span_same) remaps the
       pixels
       already in the span through one lightabs row. */
L0EFF: /* _seg003_EFF */
    /* 0EFF  shl     ax,1 */
    AX = shl16(AX, 1);
L0F01:
    /* 0F01  jb      L0F23 */
    if (CF) goto L0F23;
L0F03:
    /* 0F03  mov     bx,ax */
    BX = AX;
L0F05:
    /* 0F05  mov     ax,word ptr [bx+21Eh] */
    AX = rw(pDS, BX + 0x21E);
L0F09:
    /* 0F09  mov     word ptr ds:[410Eh],ax */
    ww(pDS, 0x410E, AX);
L0F0C:
    /* 0F0C  mov     ax,word ptr [bx+434h] */
    AX = rw(pDS, BX + 0x434);
L0F10:
    /* 0F10  mov     word ptr ds:[PEN_WORD2],ax */
    ww(pDS, 0x4114, AX);
L0F13:
    /* 0F13  db      8Bh,9Fh,8h,0h */
    BX = rw(pDS, BX + 0x8);
L0F17:
    /* 0F17  cmp     bx,14h */
    sub16(BX, 0x14, 0);
L0F1A:
    /* 0F1A  jae     L0F23 */
    if (!CF) goto L0F23;
L0F1C:
    /* 0F1C  mov     ax,word ptr [bx+0AF4h] */
    AX = rw(pDS, BX + 0xAF4);
L0F20:
    /* 0F20  mov     word ptr ds:[SPAN_WRITER],ax */
    ww(pDS, 0x4110, AX);
L0F23: /* L0F23 */
    /* 0F23  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0F24: /* L0F24 */
    /* 0F24  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0F2B: /* L0F2B */
    /* 0F2B  push    si */
    push16(SI);
L0F2C:
    /* 0F2C  push    es */
    push16(asm_es);
L0F2D:
    /* 0F2D  mov     al,byte ptr cs:[0CEC0h] */
    AL = rb(CODE003, 0xCEC0);
L0F31:
    /* 0F31  mov     ah,byte ptr ds:[0B0Ah] */
    AH = rb(pDS, 0xB0A);
L0F35:
    /* 0F35  mov     bx,ax */
    BX = AX;
L0F37:
    /* 0F37  mov     ax,seg seg004 */
    AX = (uint16_t)(0x06E7 + PORT_LOAD_SEG);
L0F3A:
    /* 0F3A  mov     es,ax */
    SET_ES(AX);
L0F3C:
    /* 0F3C  mov     al,byte ptr es:[bx+696Eh] */
    AL = rb(pES, BX + 0x696E);
L0F41:
    /* 0F41  mov     byte ptr ds:[PEN_COLOR],al */
    wb(pDS, 0x410F, AL);
L0F44:
    /* 0F44  or      byte ptr [si-3],80h */
    wb(pDS, SI + 0xFFFD, (uint8_t)(rb(pDS, SI + 0xFFFD) | 0x80));
L0F48:
    /* 0F48  sub     si,0Ah */
    SI = (uint16_t)(SI - 0xA);
L0F4B:
    /* 0F4B  call    _seg003_D2D */
    if ((c = asm_call(ASM_JMP(0x0090, 0x0D2D), 0x0F4E)) != 0) return c;
L0F4E:
    /* 0F4E  pop     es */
    SET_ES(pop16());
L0F4F:
    /* 0F4F  pop     si */
    SI = pop16();
L0F50:
    /* 0F50  and     byte ptr [si-3],7Fh */
    wb(pDS, SI + 0xFFFD, (uint8_t)(rb(pDS, SI + 0xFFFD) & 0x7F));
L0F54:
    /* 0F54  jmp     short _seg003_F77 */
    goto L0F77;
L0F56:
    /* 0F56  push    ds */
    push16(asm_ds);
L0F57:
    /* 0F57  push    es */
    push16(asm_es);
L0F58:
    /* 0F58  mov     dh,byte ptr ss:[0B0Ah] */
    DH = rb(pSS, 0xB0A);
L0F5D:
    /* 0F5D  mov     ds,word ptr ss:[95Ah] */
    SET_DS(rw(pSS, 0x95A));
L0F62:
    /* 0F62  mov     ax,seg seg004 */
    AX = (uint16_t)(0x06E7 + PORT_LOAD_SEG);
L0F65:
    /* 0F65  mov     es,ax */
    SET_ES(AX);
L0F67: /* L0F67 */
    /* 0F67  mov     dl,byte ptr [di] */
    DL = rb(pDS, DI);
L0F69:
    /* 0F69  mov     bx,dx */
    BX = DX;
L0F6B:
    /* 0F6B  mov     bh,byte ptr es:[bx+696Eh] */
    BH = rb(pES, BX + 0x696E);
L0F70:
    /* 0F70  mov     byte ptr [di],bh */
    wb(pDS, DI, BH);
L0F72:
    /* 0F72  inc     di */
    DI = (uint16_t)(DI + 1);
L0F73:
    /* 0F73  loop    L0F67 */
    if (--CX) goto L0F67;
L0F75:
    /* 0F75  pop     es */
    SET_ES(pop16());
L0F76:
    /* 0F76  pop     ds */
    SET_DS(pop16());

    /* seg003_F77  (+F77)
       vga_smooth_ylrii (FM Towns): draw a table of Gouraud spans into the frame buffer. In: SI =
       10-byte records (y, left x, right x, left intensity, right intensity; GRLIBM.ASM's table at
       5692), ended by a y with its top bit set. For each, the intensity step per pixel is formed as
       8.8 and the span drawn through L0F29, or through L0F27 when both ends have the same intensity.

       L0FF7 (do_filled_dither, solid) writes lightabs[intensity][base colour]; the code after L1037
       (do_trans_dither, translucent) writes lightabs[intensity][the pixel already there]. Both
       dither: two intensity accumulators 40h apart alternate pixel by pixel, and which starts
       depends on the row's parity, so a shade between two lightabs rows comes out as a checkerboard
       of both. */
L0F77: /* _seg003_F77 */
    /* 0F77  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0F78:
    /* 0F78  mov     word ptr cs:L0F25,ax */
    ww(CODE003, 0xF25, AX);
L0F7C:
    /* 0F7C  shl     ax,1 */
    AX = shl16(AX, 1);
L0F7E:
    /* 0F7E  jb      L0F24 */
    if (CF) goto L0F24;
L0F80:
    /* 0F80  mov     di,ax */
    DI = AX;
L0F82:
    /* 0F82  mov     di,word ptr ss:[di+95Eh] */
    DI = rw(pSS, DI + 0x95E);
L0F87:
    /* 0F87  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0F88:
    /* 0F88  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L0F89:
    /* 0F89  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0F8A:
    /* 0F8A  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L0F8B:
    /* 0F8B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0F8C:
    /* 0F8C  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L0F8D:
    /* 0F8D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0F8E:
    /* 0F8E  mov     bp,cx */
    BP = CX;
L0F90:
    /* 0F90  sub     cx,dx */
    CX = sub16(CX, DX, 0);
L0F92:
    /* 0F92  jge     L0F99 */
    if (SF == OF) goto L0F99;
L0F94:
    /* 0F94  neg     cx */
    CX = (uint16_t)-CX;
L0F96:
    /* 0F96  xchg    bp,dx */
    { uint16_t t_ = DX;
    DX = BP;
    BP = t_; }
L0F98:
    /* 0F98  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L0F99: /* L0F99 */
    /* 0F99  mov     word ptr ss:[0B0Ah],bx */
    ww(pSS, 0xB0A, BX);
L0F9E:
    /* 0F9E  mov     word ptr ss:[0B0Ch],dx */
    ww(pSS, 0xB0C, DX);
L0FA3:
    /* 0FA3  add     di,dx */
    DI = (uint16_t)(DI + DX);
L0FA5:
    /* 0FA5  inc     cx */
    CX = (uint16_t)(CX + 1);
L0FA6:
    /* 0FA6  sub     ax,bx */
    AX = sub16(AX, BX, 0);
L0FA8:
    /* 0FA8  jne     L0FAF */
    if (!ZF) goto L0FAF;
L0FAA:
    /* 0FAA  jmp     word ptr cs:L0F27 */
    return ASM_JMP(0x0090, rw(CODE003, 0xF27));
L0FAF: /* L0FAF */
    /* 0FAF  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L0FB1:
    /* 0FB1  jns     L0FCE */
    if (!SF) goto L0FCE;
L0FB3:
    /* 0FB3  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L0FB5:
    /* 0FB5  jns     L0FBD */
    if (!SF) goto L0FBD;
L0FB7:
    /* 0FB7  neg     ax */
    AX = (uint16_t)-AX;
L0FB9:
    /* 0FB9  neg     cx */
    CX = (uint16_t)-CX;
L0FBB:
    /* 0FBB  jmp     short L0FD8 */
    goto L0FD8;
L0FBD: /* L0FBD */
    /* 0FBD  neg     ax */
    AX = (uint16_t)-AX;
L0FBF: /* L0FBF */
    /* 0FBF  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0FC0:
    /* 0FC0  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x0FC0, 2)) != 0) return c;
L0FC2:
    /* 0FC2  mov     bh,al */
    BH = AL;
L0FC4:
    /* 0FC4  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L0FC6:
    /* 0FC6  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x0090, 0x0FC6, 2)) != 0) return c;
L0FC8:
    /* 0FC8  mov     bl,ah */
    BL = AH;
L0FCA:
    /* 0FCA  neg     bx */
    BX = neg16(BX);
L0FCC:
    /* 0FCC  jmp     short L0FE3 */
    goto L0FE3;
L0FCE: /* L0FCE */
    /* 0FCE  test    cx,0FFFFh */
    logic16((uint16_t)(CX & 0xFFFF));
L0FD2:
    /* 0FD2  jns     L0FD8 */
    if (!SF) goto L0FD8;
L0FD4:
    /* 0FD4  neg     cx */
    CX = (uint16_t)-CX;
L0FD6:
    /* 0FD6  jmp     L0FBF */
    goto L0FBF;
L0FD8: /* L0FD8 */
    /* 0FD8  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0FD9:
    /* 0FD9  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x0FD9, 2)) != 0) return c;
L0FDB:
    /* 0FDB  mov     bh,al */
    BH = AL;
L0FDD:
    /* 0FDD  sub     ax,ax */
    AX = sub16(AX, AX, 0);
L0FDF:
    /* 0FDF  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x0090, 0x0FDF, 2)) != 0) return c;
L0FE1:
    /* 0FE1  mov     bl,ah */
    BL = AH;
L0FE3: /* L0FE3 */
    /* 0FE3  mov     bp,bx */
    BP = BX;
L0FE5:
    /* 0FE5  mov     bh,byte ptr ss:[0B0Ah] */
    BH = rb(pSS, 0xB0A);
L0FEA:
    /* 0FEA  mov     bl,80h */
    BL = 0x80;
L0FEC:
    /* 0FEC  push    ds */
    push16(asm_ds);
L0FED:
    /* 0FED  mov     ds,word ptr ss:[95Ah] */
    SET_DS(rw(pSS, 0x95A));
L0FF2:
    /* 0FF2  jmp     word ptr cs:L0F29 */
    return ASM_JMP(0x0090, rw(CODE003, 0xF29));
L0FF7: /* L0FF7 */
    /* 0FF7  push    si */
    push16(SI);
L0FF8:
    /* 0FF8  push    es */
    push16(asm_es);
L0FF9:
    /* 0FF9  mov     ax,cx */
    AX = CX;
L0FFB:
    /* 0FFB  mov     cx,bx */
    CX = BX;
L0FFD:
    /* 0FFD  mov     dx,bx */
    DX = BX;
L0FFF:
    /* 0FFF  shl     bp,1 */
    BP = (uint16_t)(BP << 1);
L1001:
    /* 1001  sub     dx,40h */
    DX = (uint16_t)(DX - 0x40);
L1004:
    /* 1004  add     cx,40h */
    CX = (uint16_t)(CX + 0x40);
L1007:
    /* 1007  shr     word ptr cs:L0F25,1 */
    ww(CODE003, 0xF25, shr16(rw(CODE003, 0xF25), 1));
L100C:
    /* 100C  jae     L1010 */
    if (!CF) goto L1010;
L100E:
    /* 100E  xchg    cx,dx */
    { uint16_t t_ = DX;
    DX = CX;
    CX = t_; }
L1010: /* L1010 */
    /* 1010  mov     bx,seg seg004 */
    BX = (uint16_t)(0x06E7 + PORT_LOAD_SEG);
L1013:
    /* 1013  mov     es,bx */
    SET_ES(BX);
L1015:
    /* 1015  mov     bl,byte ptr es:[6950h] */
    BL = rb(pES, 0x6950);
L101A:
    /* 101A  mov     si,696Eh */
    SI = 0x696E;
L101D: /* L101D */
    /* 101D  mov     bh,dh */
    BH = DH;
L101F:
    /* 101F  mov     bh,byte ptr es:[bx+si] */
    BH = rb(pES, BX + SI);
L1022:
    /* 1022  mov     byte ptr [di],bh */
    wb(pDS, DI, BH);
L1024:
    /* 1024  add     dx,bp */
    DX = (uint16_t)(DX + BP);
L1026:
    /* 1026  inc     di */
    DI = (uint16_t)(DI + 1);
L1027:
    /* 1027  dec     ax */
    AX = dec16(AX);
L1028:
    /* 1028  je      L1037 */
    if (ZF) goto L1037;
L102A:
    /* 102A  mov     bh,ch */
    BH = CH;
L102C:
    /* 102C  mov     bh,byte ptr es:[bx+si] */
    BH = rb(pES, BX + SI);
L102F:
    /* 102F  mov     byte ptr [di],bh */
    wb(pDS, DI, BH);
L1031:
    /* 1031  add     cx,bp */
    CX = (uint16_t)(CX + BP);
L1033:
    /* 1033  inc     di */
    DI = (uint16_t)(DI + 1);
L1034:
    /* 1034  dec     ax */
    AX = dec16(AX);
L1035:
    /* 1035  jne     L101D */
    if (!ZF) goto L101D;
L1037: /* L1037 */
    /* 1037  pop     es */
    SET_ES(pop16());
L1038:
    /* 1038  pop     si */
    SI = pop16();
L1039:
    /* 1039  pop     ds */
    SET_DS(pop16());
L103A:
    /* 103A  jmp     _seg003_F77 */
    goto L0F77;
L103D:
    /* 103D  push    si */
    push16(SI);
L103E:
    /* 103E  mov     ax,cx */
    AX = CX;
L1040:
    /* 1040  mov     cx,bx */
    CX = BX;
L1042:
    /* 1042  shl     bp,1 */
    BP = (uint16_t)(BP << 1);
L1044:
    /* 1044  sub     bx,40h */
    BX = (uint16_t)(BX - 0x40);
L1047:
    /* 1047  add     cx,40h */
    CX = (uint16_t)(CX + 0x40);
L104A:
    /* 104A  shr     word ptr cs:L0F25,1 */
    ww(CODE003, 0xF25, shr16(rw(CODE003, 0xF25), 1));
L104F:
    /* 104F  jae     L1053 */
    if (!CF) goto L1053;
L1051:
    /* 1051  xchg    cx,bx */
    { uint16_t t_ = BX;
    BX = CX;
    CX = t_; }
L1053: /* L1053 */
    /* 1053  mov     dx,bx */
    DX = BX;
L1055:
    /* 1055  push    es */
    push16(asm_es);
L1056:
    /* 1056  mov     bx,seg seg004 */
    BX = (uint16_t)(0x06E7 + PORT_LOAD_SEG);
L1059:
    /* 1059  mov     es,bx */
    SET_ES(BX);
L105B:
    /* 105B  mov     si,696Eh */
    SI = 0x696E;
L105E: /* L105E */
    /* 105E  mov     bl,byte ptr [di] */
    BL = rb(pDS, DI);
L1060:
    /* 1060  mov     bh,dh */
    BH = DH;
L1062:
    /* 1062  mov     bh,byte ptr es:[bx+si] */
    BH = rb(pES, BX + SI);
L1065:
    /* 1065  mov     byte ptr [di],bh */
    wb(pDS, DI, BH);
L1067:
    /* 1067  add     dx,bp */
    DX = (uint16_t)(DX + BP);
L1069:
    /* 1069  inc     di */
    DI = (uint16_t)(DI + 1);
L106A:
    /* 106A  dec     ax */
    AX = dec16(AX);
L106B:
    /* 106B  je      L107C */
    if (ZF) goto L107C;
L106D:
    /* 106D  mov     bl,byte ptr [di] */
    BL = rb(pDS, DI);
L106F:
    /* 106F  mov     bh,ch */
    BH = CH;
L1071:
    /* 1071  mov     bh,byte ptr es:[bx+si] */
    BH = rb(pES, BX + SI);
L1074:
    /* 1074  mov     byte ptr [di],bh */
    wb(pDS, DI, BH);
L1076:
    /* 1076  add     cx,bp */
    CX = (uint16_t)(CX + BP);
L1078:
    /* 1078  inc     di */
    DI = (uint16_t)(DI + 1);
L1079:
    /* 1079  dec     ax */
    AX = dec16(AX);
L107A:
    /* 107A  jne     L105E */
    if (!ZF) goto L105E;
L107C: /* L107C */
    /* 107C  pop     es */
    SET_ES(pop16());
L107D:
    /* 107D  pop     si */
    SI = pop16();
L107E:
    /* 107E  pop     ds */
    SET_DS(pop16());
L107F:
    /* 107F  jmp     _seg003_F77 */
    goto L0F77;
}
