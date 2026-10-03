/* grlibi_x.c: replaces src/gfx/GRLIBI.ASM (seg003_0272_38B4, 38B4..43EF of its
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

uint32_t asm_mod_GRLIBI(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x38B4: goto L38B4;
    case 0x38B8: goto L38B8;
    case 0x38BC: goto L38BC;
    case 0x38C0: goto L38C0;
    case 0x38C4: goto L38C4;
    case 0x38C8: goto L38C8;
    case 0x38CB: goto L38CB;
    case 0x38CE: goto L38CE;
    case 0x38D1: goto L38D1;
    case 0x38D3: goto L38D3;
    case 0x38D5: goto L38D5;
    case 0x38D8: goto L38D8;
    case 0x38D9: goto L38D9;
    case 0x38DA: goto L38DA;
    case 0x38DC: goto L38DC;
    case 0x38DD: goto L38DD;
    case 0x38DE: goto L38DE;
    case 0x38DF: goto L38DF;
    case 0x38E1: goto L38E1;
    case 0x38E3: goto L38E3;
    case 0x38E5: goto L38E5;
    case 0x38E8: goto L38E8;
    case 0x38EC: goto L38EC;
    case 0x38EE: goto L38EE;
    case 0x38F2: goto L38F2;
    case 0x38F4: goto L38F4;
    case 0x38F6: goto L38F6;
    case 0x38F8: goto L38F8;
    case 0x38FA: goto L38FA;
    case 0x38FC: goto L38FC;
    case 0x3900: goto L3900;
    case 0x3904: goto L3904;
    case 0x3907: goto L3907;
    case 0x390B: goto L390B;
    case 0x390F: goto L390F;
    case 0x3912: goto L3912;
    case 0x3915: goto L3915;
    case 0x3919: goto L3919;
    case 0x391C: goto L391C;
    case 0x3920: goto L3920;
    case 0x3922: goto L3922;
    case 0x3924: goto L3924;
    case 0x3927: goto L3927;
    case 0x392A: goto L392A;
    case 0x392D: goto L392D;
    case 0x3931: goto L3931;
    case 0x3932: goto L3932;
    case 0x3934: goto L3934;
    case 0x3936: goto L3936;
    case 0x3938: goto L3938;
    case 0x393A: goto L393A;
    case 0x393C: goto L393C;
    case 0x393E: goto L393E;
    case 0x393F: goto L393F;
    case 0x3940: goto L3940;
    case 0x3942: goto L3942;
    case 0x3944: goto L3944;
    case 0x3946: goto L3946;
    case 0x3947: goto L3947;
    case 0x3948: goto L3948;
    case 0x394A: goto L394A;
    case 0x394C: goto L394C;
    case 0x394D: goto L394D;
    case 0x394E: goto L394E;
    case 0x394F: goto L394F;
    case 0x3950: goto L3950;
    case 0x3954: goto L3954;
    case 0x3958: goto L3958;
    case 0x395C: goto L395C;
    case 0x395D: goto L395D;
    case 0x395E: goto L395E;
    case 0x395F: goto L395F;
    case 0x3960: goto L3960;
    case 0x3961: goto L3961;
    case 0x3962: goto L3962;
    case 0x3963: goto L3963;
    case 0x3964: goto L3964;
    case 0x3965: goto L3965;
    case 0x3969: goto L3969;
    case 0x396B: goto L396B;
    case 0x396F: goto L396F;
    case 0x3970: goto L3970;
    case 0x3971: goto L3971;
    case 0x3972: goto L3972;
    case 0x3973: goto L3973;
    case 0x3976: goto L3976;
    case 0x3977: goto L3977;
    case 0x3979: goto L3979;
    case 0x397B: goto L397B;
    case 0x397F: goto L397F;
    case 0x3982: goto L3982;
    case 0x3986: goto L3986;
    case 0x3988: goto L3988;
    case 0x3989: goto L3989;
    case 0x398C: goto L398C;
    case 0x398D: goto L398D;
    case 0x398E: goto L398E;
    case 0x398F: goto L398F;
    case 0x3990: goto L3990;
    case 0x3994: goto L3994;
    case 0x3996: goto L3996;
    case 0x3999: goto L3999;
    case 0x399A: goto L399A;
    case 0x399B: goto L399B;
    case 0x399C: goto L399C;
    case 0x399D: goto L399D;
    case 0x39A0: goto L39A0;
    case 0x39A2: goto L39A2;
    case 0x39A4: goto L39A4;
    case 0x39A8: goto L39A8;
    case 0x39AB: goto L39AB;
    case 0x39AF: goto L39AF;
    case 0x39B1: goto L39B1;
    case 0x39B2: goto L39B2;
    case 0x39B4: goto L39B4;
    case 0x39B6: goto L39B6;
    case 0x39B8: goto L39B8;
    case 0x39BA: goto L39BA;
    case 0x39BB: goto L39BB;
    case 0x39BD: goto L39BD;
    case 0x39C1: goto L39C1;
    case 0x39C3: goto L39C3;
    case 0x39C5: goto L39C5;
    case 0x39C6: goto L39C6;
    case 0x39C9: goto L39C9;
    case 0x39CB: goto L39CB;
    case 0x39CD: goto L39CD;
    case 0x39CE: goto L39CE;
    case 0x39CF: goto L39CF;
    case 0x39D1: goto L39D1;
    case 0x39D2: goto L39D2;
    case 0x39D4: goto L39D4;
    case 0x39D6: goto L39D6;
    case 0x39D8: goto L39D8;
    case 0x39DA: goto L39DA;
    case 0x39DB: goto L39DB;
    case 0x39DD: goto L39DD;
    case 0x39DF: goto L39DF;
    case 0x39E1: goto L39E1;
    case 0x39E2: goto L39E2;
    case 0x39E5: goto L39E5;
    case 0x39E8: goto L39E8;
    case 0x39EA: goto L39EA;
    case 0x39EC: goto L39EC;
    case 0x39F0: goto L39F0;
    case 0x39F3: goto L39F3;
    case 0x39F6: goto L39F6;
    case 0x39F9: goto L39F9;
    case 0x39FB: goto L39FB;
    case 0x39FD: goto L39FD;
    case 0x3A01: goto L3A01;
    case 0x3A02: goto L3A02;
    case 0x3A04: goto L3A04;
    case 0x3A06: goto L3A06;
    case 0x3A07: goto L3A07;
    case 0x3A0A: goto L3A0A;
    case 0x3A0C: goto L3A0C;
    case 0x3A0E: goto L3A0E;
    case 0x3A0F: goto L3A0F;
    case 0x3A12: goto L3A12;
    case 0x3A14: goto L3A14;
    case 0x3A16: goto L3A16;
    case 0x3A17: goto L3A17;
    case 0x3A1A: goto L3A1A;
    case 0x3A1C: goto L3A1C;
    case 0x3A1E: goto L3A1E;
    case 0x3A1F: goto L3A1F;
    case 0x3A22: goto L3A22;
    case 0x3A24: goto L3A24;
    case 0x3A26: goto L3A26;
    case 0x3A27: goto L3A27;
    case 0x3A2A: goto L3A2A;
    case 0x3A2C: goto L3A2C;
    case 0x3A2E: goto L3A2E;
    case 0x3A2F: goto L3A2F;
    case 0x3A32: goto L3A32;
    case 0x3A34: goto L3A34;
    case 0x3A36: goto L3A36;
    case 0x3A37: goto L3A37;
    case 0x3A3A: goto L3A3A;
    case 0x3A3C: goto L3A3C;
    case 0x3A3E: goto L3A3E;
    case 0x3A3F: goto L3A3F;
    case 0x3A42: goto L3A42;
    case 0x3A44: goto L3A44;
    case 0x3A46: goto L3A46;
    case 0x3A47: goto L3A47;
    case 0x3A4A: goto L3A4A;
    case 0x3A4C: goto L3A4C;
    case 0x3A4E: goto L3A4E;
    case 0x3A4F: goto L3A4F;
    case 0x3A52: goto L3A52;
    case 0x3A54: goto L3A54;
    case 0x3A56: goto L3A56;
    case 0x3A57: goto L3A57;
    case 0x3A5A: goto L3A5A;
    case 0x3A5C: goto L3A5C;
    case 0x3A5E: goto L3A5E;
    case 0x3A5F: goto L3A5F;
    case 0x3A62: goto L3A62;
    case 0x3A64: goto L3A64;
    case 0x3A66: goto L3A66;
    case 0x3A67: goto L3A67;
    case 0x3A6A: goto L3A6A;
    case 0x3A6C: goto L3A6C;
    case 0x3A6E: goto L3A6E;
    case 0x3A6F: goto L3A6F;
    case 0x3A72: goto L3A72;
    case 0x3A74: goto L3A74;
    case 0x3A76: goto L3A76;
    case 0x3A77: goto L3A77;
    case 0x3A7A: goto L3A7A;
    case 0x3A7C: goto L3A7C;
    case 0x3A7E: goto L3A7E;
    case 0x3A7F: goto L3A7F;
    case 0x3A82: goto L3A82;
    case 0x3A84: goto L3A84;
    case 0x3A86: goto L3A86;
    case 0x3A87: goto L3A87;
    case 0x3A8A: goto L3A8A;
    case 0x3A8C: goto L3A8C;
    case 0x3A8E: goto L3A8E;
    case 0x3A8F: goto L3A8F;
    case 0x3A92: goto L3A92;
    case 0x3A94: goto L3A94;
    case 0x3A96: goto L3A96;
    case 0x3A97: goto L3A97;
    case 0x3A9A: goto L3A9A;
    case 0x3A9C: goto L3A9C;
    case 0x3A9E: goto L3A9E;
    case 0x3A9F: goto L3A9F;
    case 0x3AA2: goto L3AA2;
    case 0x3AA4: goto L3AA4;
    case 0x3AA6: goto L3AA6;
    case 0x3AA7: goto L3AA7;
    case 0x3AAA: goto L3AAA;
    case 0x3AAC: goto L3AAC;
    case 0x3AAE: goto L3AAE;
    case 0x3AAF: goto L3AAF;
    case 0x3AB2: goto L3AB2;
    case 0x3AB4: goto L3AB4;
    case 0x3AB6: goto L3AB6;
    case 0x3AB7: goto L3AB7;
    case 0x3ABA: goto L3ABA;
    case 0x3ABC: goto L3ABC;
    case 0x3ABE: goto L3ABE;
    case 0x3ABF: goto L3ABF;
    case 0x3AC2: goto L3AC2;
    case 0x3AC4: goto L3AC4;
    case 0x3AC6: goto L3AC6;
    case 0x3AC7: goto L3AC7;
    case 0x3ACA: goto L3ACA;
    case 0x3ACC: goto L3ACC;
    case 0x3ACE: goto L3ACE;
    case 0x3ACF: goto L3ACF;
    case 0x3AD2: goto L3AD2;
    case 0x3AD4: goto L3AD4;
    case 0x3AD6: goto L3AD6;
    case 0x3AD7: goto L3AD7;
    case 0x3ADA: goto L3ADA;
    case 0x3ADC: goto L3ADC;
    case 0x3ADE: goto L3ADE;
    case 0x3ADF: goto L3ADF;
    case 0x3AE2: goto L3AE2;
    case 0x3AE4: goto L3AE4;
    case 0x3AE6: goto L3AE6;
    case 0x3AE7: goto L3AE7;
    case 0x3AEA: goto L3AEA;
    case 0x3AEC: goto L3AEC;
    case 0x3AEE: goto L3AEE;
    case 0x3AEF: goto L3AEF;
    case 0x3AF2: goto L3AF2;
    case 0x3AF4: goto L3AF4;
    case 0x3AF6: goto L3AF6;
    case 0x3AF7: goto L3AF7;
    case 0x3AFA: goto L3AFA;
    case 0x3AFB: goto L3AFB;
    case 0x3AFD: goto L3AFD;
    case 0x3B00: goto L3B00;
    case 0x3B03: goto L3B03;
    case 0x3B06: goto L3B06;
    case 0x3B0A: goto L3B0A;
    case 0x3B0D: goto L3B0D;
    case 0x3B10: goto L3B10;
    case 0x3B12: goto L3B12;
    case 0x3B15: goto L3B15;
    case 0x3B17: goto L3B17;
    case 0x3B1A: goto L3B1A;
    case 0x3B1D: goto L3B1D;
    case 0x3B1E: goto L3B1E;
    case 0x3B24: goto L3B24;
    case 0x3B26: goto L3B26;
    case 0x3B2C: goto L3B2C;
    case 0x3B2E: goto L3B2E;
    case 0x3B34: goto L3B34;
    case 0x3B36: goto L3B36;
    case 0x3B3C: goto L3B3C;
    case 0x3B3E: goto L3B3E;
    case 0x3B44: goto L3B44;
    case 0x3B46: goto L3B46;
    case 0x3B4A: goto L3B4A;
    case 0x3B4D: goto L3B4D;
    case 0x3B51: goto L3B51;
    case 0x3B54: goto L3B54;
    case 0x3B58: goto L3B58;
    case 0x3B5A: goto L3B5A;
    case 0x3B5E: goto L3B5E;
    case 0x3B60: goto L3B60;
    case 0x3B64: goto L3B64;
    case 0x3B65: goto L3B65;
    case 0x3B68: goto L3B68;
    case 0x3B6C: goto L3B6C;
    case 0x3B6E: goto L3B6E;
    case 0x3B72: goto L3B72;
    case 0x3B74: goto L3B74;
    case 0x3B77: goto L3B77;
    case 0x3B7B: goto L3B7B;
    case 0x3B7D: goto L3B7D;
    case 0x3B81: goto L3B81;
    case 0x3B83: goto L3B83;
    case 0x3B87: goto L3B87;
    case 0x3B8A: goto L3B8A;
    case 0x3B8E: goto L3B8E;
    case 0x3B8F: goto L3B8F;
    case 0x3B92: goto L3B92;
    case 0x3B96: goto L3B96;
    case 0x3B99: goto L3B99;
    case 0x3B9D: goto L3B9D;
    case 0x3B9F: goto L3B9F;
    case 0x3BA3: goto L3BA3;
    case 0x3BA5: goto L3BA5;
    case 0x3BA9: goto L3BA9;
    case 0x3BAA: goto L3BAA;
    case 0x3BAD: goto L3BAD;
    case 0x3BB1: goto L3BB1;
    case 0x3BB3: goto L3BB3;
    case 0x3BB7: goto L3BB7;
    case 0x3BB9: goto L3BB9;
    case 0x3BBC: goto L3BBC;
    case 0x3BC0: goto L3BC0;
    case 0x3BC2: goto L3BC2;
    case 0x3BC6: goto L3BC6;
    case 0x3BC8: goto L3BC8;
    case 0x3BCC: goto L3BCC;
    case 0x3BD0: goto L3BD0;
    case 0x3BD4: goto L3BD4;
    case 0x3BD7: goto L3BD7;
    case 0x3BDA: goto L3BDA;
    case 0x3BDD: goto L3BDD;
    case 0x3BE0: goto L3BE0;
    case 0x3BE2: goto L3BE2;
    case 0x3BE5: goto L3BE5;
    case 0x3BE8: goto L3BE8;
    case 0x3BEB: goto L3BEB;
    case 0x3BEC: goto L3BEC;
    case 0x3BED: goto L3BED;
    case 0x3BEE: goto L3BEE;
    case 0x3BF1: goto L3BF1;
    case 0x3BF4: goto L3BF4;
    case 0x3BF5: goto L3BF5;
    case 0x3BF6: goto L3BF6;
    case 0x3BF8: goto L3BF8;
    case 0x3BF9: goto L3BF9;
    case 0x3BFA: goto L3BFA;
    case 0x3BFC: goto L3BFC;
    case 0x3BFD: goto L3BFD;
    case 0x3C01: goto L3C01;
    case 0x3C05: goto L3C05;
    case 0x3C07: goto L3C07;
    case 0x3C09: goto L3C09;
    case 0x3C0C: goto L3C0C;
    case 0x3C0F: goto L3C0F;
    case 0x3C12: goto L3C12;
    case 0x3C16: goto L3C16;
    case 0x3C17: goto L3C17;
    case 0x3C19: goto L3C19;
    case 0x3C1B: goto L3C1B;
    case 0x3C1E: goto L3C1E;
    case 0x3C21: goto L3C21;
    case 0x3C24: goto L3C24;
    case 0x3C25: goto L3C25;
    case 0x3C28: goto L3C28;
    case 0x3C2B: goto L3C2B;
    case 0x3C2E: goto L3C2E;
    case 0x3C2F: goto L3C2F;
    case 0x3C32: goto L3C32;
    case 0x3C35: goto L3C35;
    case 0x3C39: goto L3C39;
    case 0x3C3F: goto L3C3F;
    case 0x3C42: goto L3C42;
    case 0x3C45: goto L3C45;
    case 0x3C47: goto L3C47;
    case 0x3C4A: goto L3C4A;
    case 0x3C4C: goto L3C4C;
    case 0x3C4F: goto L3C4F;
    case 0x3C51: goto L3C51;
    case 0x3C54: goto L3C54;
    case 0x3C57: goto L3C57;
    case 0x3C5A: goto L3C5A;
    case 0x3C5D: goto L3C5D;
    case 0x3C60: goto L3C60;
    case 0x3C61: goto L3C61;
    case 0x3C64: goto L3C64;
    case 0x3C66: goto L3C66;
    case 0x3C6A: goto L3C6A;
    case 0x3C6D: goto L3C6D;
    case 0x3C6E: goto L3C6E;
    case 0x3C70: goto L3C70;
    case 0x3C72: goto L3C72;
    case 0x3C74: goto L3C74;
    case 0x3C78: goto L3C78;
    case 0x3C7B: goto L3C7B;
    case 0x3C7E: goto L3C7E;
    case 0x3C81: goto L3C81;
    case 0x3C84: goto L3C84;
    case 0x3C88: goto L3C88;
    case 0x3C8A: goto L3C8A;
    case 0x3C8C: goto L3C8C;
    case 0x3C90: goto L3C90;
    case 0x3C92: goto L3C92;
    case 0x3C96: goto L3C96;
    case 0x3C9A: goto L3C9A;
    case 0x3C9C: goto L3C9C;
    case 0x3CA0: goto L3CA0;
    case 0x3CA2: goto L3CA2;
    case 0x3CA6: goto L3CA6;
    case 0x3CA9: goto L3CA9;
    case 0x3CAD: goto L3CAD;
    case 0x3CB1: goto L3CB1;
    case 0x3CB2: goto L3CB2;
    case 0x3CB6: goto L3CB6;
    case 0x3CBA: goto L3CBA;
    case 0x3CBD: goto L3CBD;
    case 0x3CC1: goto L3CC1;
    case 0x3CC2: goto L3CC2;
    case 0x3CC3: goto L3CC3;
    case 0x3CC5: goto L3CC5;
    case 0x3CC7: goto L3CC7;
    case 0x3CC9: goto L3CC9;
    case 0x3CCD: goto L3CCD;
    case 0x3CCE: goto L3CCE;
    case 0x3CD0: goto L3CD0;
    case 0x3CD1: goto L3CD1;
    case 0x3CD3: goto L3CD3;
    case 0x3CD4: goto L3CD4;
    case 0x3CD6: goto L3CD6;
    case 0x3CD7: goto L3CD7;
    case 0x3CD8: goto L3CD8;
    case 0x3CDA: goto L3CDA;
    case 0x3CDC: goto L3CDC;
    case 0x3CDE: goto L3CDE;
    case 0x3CE2: goto L3CE2;
    case 0x3CE3: goto L3CE3;
    case 0x3CE5: goto L3CE5;
    case 0x3CE6: goto L3CE6;
    case 0x3CE8: goto L3CE8;
    case 0x3CE9: goto L3CE9;
    case 0x3CEB: goto L3CEB;
    case 0x3CEC: goto L3CEC;
    case 0x3CED: goto L3CED;
    case 0x3CEF: goto L3CEF;
    case 0x3CF1: goto L3CF1;
    case 0x3CF3: goto L3CF3;
    case 0x3CF7: goto L3CF7;
    case 0x3CF8: goto L3CF8;
    case 0x3CFA: goto L3CFA;
    case 0x3CFB: goto L3CFB;
    case 0x3CFD: goto L3CFD;
    case 0x3CFE: goto L3CFE;
    case 0x3D00: goto L3D00;
    case 0x3D01: goto L3D01;
    case 0x3D02: goto L3D02;
    case 0x3D04: goto L3D04;
    case 0x3D06: goto L3D06;
    case 0x3D08: goto L3D08;
    case 0x3D0C: goto L3D0C;
    case 0x3D0D: goto L3D0D;
    case 0x3D0F: goto L3D0F;
    case 0x3D10: goto L3D10;
    case 0x3D12: goto L3D12;
    case 0x3D13: goto L3D13;
    case 0x3D15: goto L3D15;
    case 0x3D16: goto L3D16;
    case 0x3D17: goto L3D17;
    case 0x3D19: goto L3D19;
    case 0x3D1B: goto L3D1B;
    case 0x3D1D: goto L3D1D;
    case 0x3D21: goto L3D21;
    case 0x3D22: goto L3D22;
    case 0x3D24: goto L3D24;
    case 0x3D25: goto L3D25;
    case 0x3D27: goto L3D27;
    case 0x3D28: goto L3D28;
    case 0x3D2A: goto L3D2A;
    case 0x3D2B: goto L3D2B;
    case 0x3D2C: goto L3D2C;
    case 0x3D2E: goto L3D2E;
    case 0x3D30: goto L3D30;
    case 0x3D32: goto L3D32;
    case 0x3D36: goto L3D36;
    case 0x3D37: goto L3D37;
    case 0x3D39: goto L3D39;
    case 0x3D3A: goto L3D3A;
    case 0x3D3C: goto L3D3C;
    case 0x3D3D: goto L3D3D;
    case 0x3D3F: goto L3D3F;
    case 0x3D40: goto L3D40;
    case 0x3D41: goto L3D41;
    case 0x3D43: goto L3D43;
    case 0x3D45: goto L3D45;
    case 0x3D47: goto L3D47;
    case 0x3D4B: goto L3D4B;
    case 0x3D4C: goto L3D4C;
    case 0x3D4E: goto L3D4E;
    case 0x3D4F: goto L3D4F;
    case 0x3D51: goto L3D51;
    case 0x3D52: goto L3D52;
    case 0x3D54: goto L3D54;
    case 0x3D55: goto L3D55;
    case 0x3D56: goto L3D56;
    case 0x3D58: goto L3D58;
    case 0x3D5A: goto L3D5A;
    case 0x3D5C: goto L3D5C;
    case 0x3D60: goto L3D60;
    case 0x3D61: goto L3D61;
    case 0x3D63: goto L3D63;
    case 0x3D64: goto L3D64;
    case 0x3D66: goto L3D66;
    case 0x3D67: goto L3D67;
    case 0x3D69: goto L3D69;
    case 0x3D6A: goto L3D6A;
    case 0x3D6B: goto L3D6B;
    case 0x3D6D: goto L3D6D;
    case 0x3D6F: goto L3D6F;
    case 0x3D71: goto L3D71;
    case 0x3D75: goto L3D75;
    case 0x3D76: goto L3D76;
    case 0x3D78: goto L3D78;
    case 0x3D79: goto L3D79;
    case 0x3D7B: goto L3D7B;
    case 0x3D7C: goto L3D7C;
    case 0x3D7E: goto L3D7E;
    case 0x3D7F: goto L3D7F;
    case 0x3D80: goto L3D80;
    case 0x3D82: goto L3D82;
    case 0x3D84: goto L3D84;
    case 0x3D86: goto L3D86;
    case 0x3D8A: goto L3D8A;
    case 0x3D8B: goto L3D8B;
    case 0x3D8D: goto L3D8D;
    case 0x3D8E: goto L3D8E;
    case 0x3D90: goto L3D90;
    case 0x3D91: goto L3D91;
    case 0x3D93: goto L3D93;
    case 0x3D94: goto L3D94;
    case 0x3D95: goto L3D95;
    case 0x3D97: goto L3D97;
    case 0x3D99: goto L3D99;
    case 0x3D9B: goto L3D9B;
    case 0x3D9F: goto L3D9F;
    case 0x3DA0: goto L3DA0;
    case 0x3DA2: goto L3DA2;
    case 0x3DA3: goto L3DA3;
    case 0x3DA5: goto L3DA5;
    case 0x3DA6: goto L3DA6;
    case 0x3DA8: goto L3DA8;
    case 0x3DA9: goto L3DA9;
    case 0x3DAA: goto L3DAA;
    case 0x3DAC: goto L3DAC;
    case 0x3DAE: goto L3DAE;
    case 0x3DB0: goto L3DB0;
    case 0x3DB4: goto L3DB4;
    case 0x3DB5: goto L3DB5;
    case 0x3DB7: goto L3DB7;
    case 0x3DB8: goto L3DB8;
    case 0x3DBA: goto L3DBA;
    case 0x3DBB: goto L3DBB;
    case 0x3DBD: goto L3DBD;
    case 0x3DBE: goto L3DBE;
    case 0x3DBF: goto L3DBF;
    case 0x3DC1: goto L3DC1;
    case 0x3DC3: goto L3DC3;
    case 0x3DC5: goto L3DC5;
    case 0x3DC9: goto L3DC9;
    case 0x3DCA: goto L3DCA;
    case 0x3DCC: goto L3DCC;
    case 0x3DCD: goto L3DCD;
    case 0x3DCF: goto L3DCF;
    case 0x3DD0: goto L3DD0;
    case 0x3DD2: goto L3DD2;
    case 0x3DD3: goto L3DD3;
    case 0x3DD4: goto L3DD4;
    case 0x3DD6: goto L3DD6;
    case 0x3DD8: goto L3DD8;
    case 0x3DDA: goto L3DDA;
    case 0x3DDE: goto L3DDE;
    case 0x3DDF: goto L3DDF;
    case 0x3DE1: goto L3DE1;
    case 0x3DE2: goto L3DE2;
    case 0x3DE4: goto L3DE4;
    case 0x3DE5: goto L3DE5;
    case 0x3DE7: goto L3DE7;
    case 0x3DE8: goto L3DE8;
    case 0x3DE9: goto L3DE9;
    case 0x3DEB: goto L3DEB;
    case 0x3DED: goto L3DED;
    case 0x3DEF: goto L3DEF;
    case 0x3DF3: goto L3DF3;
    case 0x3DF4: goto L3DF4;
    case 0x3DF6: goto L3DF6;
    case 0x3DF7: goto L3DF7;
    case 0x3DF9: goto L3DF9;
    case 0x3DFA: goto L3DFA;
    case 0x3DFC: goto L3DFC;
    case 0x3DFD: goto L3DFD;
    case 0x3DFE: goto L3DFE;
    case 0x3E00: goto L3E00;
    case 0x3E02: goto L3E02;
    case 0x3E04: goto L3E04;
    case 0x3E08: goto L3E08;
    case 0x3E09: goto L3E09;
    case 0x3E0B: goto L3E0B;
    case 0x3E0C: goto L3E0C;
    case 0x3E0E: goto L3E0E;
    case 0x3E0F: goto L3E0F;
    case 0x3E11: goto L3E11;
    case 0x3E12: goto L3E12;
    case 0x3E13: goto L3E13;
    case 0x3E15: goto L3E15;
    case 0x3E17: goto L3E17;
    case 0x3E19: goto L3E19;
    case 0x3E1D: goto L3E1D;
    case 0x3E1E: goto L3E1E;
    case 0x3E20: goto L3E20;
    case 0x3E21: goto L3E21;
    case 0x3E23: goto L3E23;
    case 0x3E24: goto L3E24;
    case 0x3E26: goto L3E26;
    case 0x3E27: goto L3E27;
    case 0x3E28: goto L3E28;
    case 0x3E2A: goto L3E2A;
    case 0x3E2C: goto L3E2C;
    case 0x3E2E: goto L3E2E;
    case 0x3E32: goto L3E32;
    case 0x3E33: goto L3E33;
    case 0x3E35: goto L3E35;
    case 0x3E36: goto L3E36;
    case 0x3E38: goto L3E38;
    case 0x3E39: goto L3E39;
    case 0x3E3B: goto L3E3B;
    case 0x3E3C: goto L3E3C;
    case 0x3E3D: goto L3E3D;
    case 0x3E3F: goto L3E3F;
    case 0x3E41: goto L3E41;
    case 0x3E43: goto L3E43;
    case 0x3E47: goto L3E47;
    case 0x3E48: goto L3E48;
    case 0x3E4A: goto L3E4A;
    case 0x3E4B: goto L3E4B;
    case 0x3E4D: goto L3E4D;
    case 0x3E4E: goto L3E4E;
    case 0x3E50: goto L3E50;
    case 0x3E51: goto L3E51;
    case 0x3E52: goto L3E52;
    case 0x3E54: goto L3E54;
    case 0x3E56: goto L3E56;
    case 0x3E58: goto L3E58;
    case 0x3E5C: goto L3E5C;
    case 0x3E5D: goto L3E5D;
    case 0x3E5F: goto L3E5F;
    case 0x3E60: goto L3E60;
    case 0x3E62: goto L3E62;
    case 0x3E63: goto L3E63;
    case 0x3E65: goto L3E65;
    case 0x3E66: goto L3E66;
    case 0x3E67: goto L3E67;
    case 0x3E69: goto L3E69;
    case 0x3E6B: goto L3E6B;
    case 0x3E6D: goto L3E6D;
    case 0x3E71: goto L3E71;
    case 0x3E72: goto L3E72;
    case 0x3E74: goto L3E74;
    case 0x3E75: goto L3E75;
    case 0x3E77: goto L3E77;
    case 0x3E78: goto L3E78;
    case 0x3E7A: goto L3E7A;
    case 0x3E7B: goto L3E7B;
    case 0x3E7C: goto L3E7C;
    case 0x3E7E: goto L3E7E;
    case 0x3E80: goto L3E80;
    case 0x3E82: goto L3E82;
    case 0x3E86: goto L3E86;
    case 0x3E87: goto L3E87;
    case 0x3E89: goto L3E89;
    case 0x3E8A: goto L3E8A;
    case 0x3E8C: goto L3E8C;
    case 0x3E8D: goto L3E8D;
    case 0x3E8F: goto L3E8F;
    case 0x3E90: goto L3E90;
    case 0x3E91: goto L3E91;
    case 0x3E93: goto L3E93;
    case 0x3E95: goto L3E95;
    case 0x3E97: goto L3E97;
    case 0x3E9B: goto L3E9B;
    case 0x3E9C: goto L3E9C;
    case 0x3E9E: goto L3E9E;
    case 0x3E9F: goto L3E9F;
    case 0x3EA1: goto L3EA1;
    case 0x3EA2: goto L3EA2;
    case 0x3EA4: goto L3EA4;
    case 0x3EA5: goto L3EA5;
    case 0x3EA6: goto L3EA6;
    case 0x3EA8: goto L3EA8;
    case 0x3EAA: goto L3EAA;
    case 0x3EAC: goto L3EAC;
    case 0x3EB0: goto L3EB0;
    case 0x3EB1: goto L3EB1;
    case 0x3EB3: goto L3EB3;
    case 0x3EB4: goto L3EB4;
    case 0x3EB6: goto L3EB6;
    case 0x3EB7: goto L3EB7;
    case 0x3EB9: goto L3EB9;
    case 0x3EBA: goto L3EBA;
    case 0x3EBB: goto L3EBB;
    case 0x3EBD: goto L3EBD;
    case 0x3EBF: goto L3EBF;
    case 0x3EC1: goto L3EC1;
    case 0x3EC5: goto L3EC5;
    case 0x3EC6: goto L3EC6;
    case 0x3EC8: goto L3EC8;
    case 0x3EC9: goto L3EC9;
    case 0x3ECB: goto L3ECB;
    case 0x3ECC: goto L3ECC;
    case 0x3ECE: goto L3ECE;
    case 0x3ECF: goto L3ECF;
    case 0x3ED0: goto L3ED0;
    case 0x3ED2: goto L3ED2;
    case 0x3ED4: goto L3ED4;
    case 0x3ED6: goto L3ED6;
    case 0x3EDA: goto L3EDA;
    case 0x3EDB: goto L3EDB;
    case 0x3EDD: goto L3EDD;
    case 0x3EDE: goto L3EDE;
    case 0x3EE0: goto L3EE0;
    case 0x3EE1: goto L3EE1;
    case 0x3EE3: goto L3EE3;
    case 0x3EE4: goto L3EE4;
    case 0x3EE5: goto L3EE5;
    case 0x3EE7: goto L3EE7;
    case 0x3EE9: goto L3EE9;
    case 0x3EEB: goto L3EEB;
    case 0x3EEF: goto L3EEF;
    case 0x3EF0: goto L3EF0;
    case 0x3EF2: goto L3EF2;
    case 0x3EF3: goto L3EF3;
    case 0x3EF5: goto L3EF5;
    case 0x3EF6: goto L3EF6;
    case 0x3EF8: goto L3EF8;
    case 0x3EF9: goto L3EF9;
    case 0x3EFA: goto L3EFA;
    case 0x3EFC: goto L3EFC;
    case 0x3EFE: goto L3EFE;
    case 0x3F00: goto L3F00;
    case 0x3F04: goto L3F04;
    case 0x3F05: goto L3F05;
    case 0x3F07: goto L3F07;
    case 0x3F08: goto L3F08;
    case 0x3F0A: goto L3F0A;
    case 0x3F0B: goto L3F0B;
    case 0x3F0D: goto L3F0D;
    case 0x3F0E: goto L3F0E;
    case 0x3F0F: goto L3F0F;
    case 0x3F11: goto L3F11;
    case 0x3F13: goto L3F13;
    case 0x3F15: goto L3F15;
    case 0x3F19: goto L3F19;
    case 0x3F1A: goto L3F1A;
    case 0x3F1C: goto L3F1C;
    case 0x3F1D: goto L3F1D;
    case 0x3F1F: goto L3F1F;
    case 0x3F20: goto L3F20;
    case 0x3F22: goto L3F22;
    case 0x3F23: goto L3F23;
    case 0x3F24: goto L3F24;
    case 0x3F26: goto L3F26;
    case 0x3F28: goto L3F28;
    case 0x3F2A: goto L3F2A;
    case 0x3F2E: goto L3F2E;
    case 0x3F2F: goto L3F2F;
    case 0x3F31: goto L3F31;
    case 0x3F32: goto L3F32;
    case 0x3F34: goto L3F34;
    case 0x3F35: goto L3F35;
    case 0x3F37: goto L3F37;
    case 0x3F38: goto L3F38;
    case 0x3F39: goto L3F39;
    case 0x3F3B: goto L3F3B;
    case 0x3F3D: goto L3F3D;
    case 0x3F3F: goto L3F3F;
    case 0x3F43: goto L3F43;
    case 0x3F44: goto L3F44;
    case 0x3F46: goto L3F46;
    case 0x3F47: goto L3F47;
    case 0x3F49: goto L3F49;
    case 0x3F4A: goto L3F4A;
    case 0x3F4C: goto L3F4C;
    case 0x3F4D: goto L3F4D;
    case 0x3F4E: goto L3F4E;
    case 0x3F50: goto L3F50;
    case 0x3F52: goto L3F52;
    case 0x3F54: goto L3F54;
    case 0x3F58: goto L3F58;
    case 0x3F59: goto L3F59;
    case 0x3F5B: goto L3F5B;
    case 0x3F5C: goto L3F5C;
    case 0x3F5E: goto L3F5E;
    case 0x3F5F: goto L3F5F;
    case 0x3F61: goto L3F61;
    case 0x3F62: goto L3F62;
    case 0x3F63: goto L3F63;
    case 0x3F65: goto L3F65;
    case 0x3F67: goto L3F67;
    case 0x3F69: goto L3F69;
    case 0x3F6D: goto L3F6D;
    case 0x3F6E: goto L3F6E;
    case 0x3F70: goto L3F70;
    case 0x3F71: goto L3F71;
    case 0x3F73: goto L3F73;
    case 0x3F74: goto L3F74;
    case 0x3F76: goto L3F76;
    case 0x3F77: goto L3F77;
    case 0x3F78: goto L3F78;
    case 0x3F7A: goto L3F7A;
    case 0x3F7C: goto L3F7C;
    case 0x3F7E: goto L3F7E;
    case 0x3F82: goto L3F82;
    case 0x3F83: goto L3F83;
    case 0x3F85: goto L3F85;
    case 0x3F86: goto L3F86;
    case 0x3F88: goto L3F88;
    case 0x3F89: goto L3F89;
    case 0x3F8B: goto L3F8B;
    case 0x3F8C: goto L3F8C;
    case 0x3F8D: goto L3F8D;
    case 0x3F8F: goto L3F8F;
    case 0x3F91: goto L3F91;
    case 0x3F93: goto L3F93;
    case 0x3F97: goto L3F97;
    case 0x3F98: goto L3F98;
    case 0x3F9A: goto L3F9A;
    case 0x3F9B: goto L3F9B;
    case 0x3F9D: goto L3F9D;
    case 0x3F9E: goto L3F9E;
    case 0x3FA0: goto L3FA0;
    case 0x3FA1: goto L3FA1;
    case 0x3FA2: goto L3FA2;
    case 0x3FA4: goto L3FA4;
    case 0x3FA6: goto L3FA6;
    case 0x3FA8: goto L3FA8;
    case 0x3FAC: goto L3FAC;
    case 0x3FAD: goto L3FAD;
    case 0x3FAF: goto L3FAF;
    case 0x3FB0: goto L3FB0;
    case 0x3FB2: goto L3FB2;
    case 0x3FB3: goto L3FB3;
    case 0x3FB5: goto L3FB5;
    case 0x3FB6: goto L3FB6;
    case 0x3FB7: goto L3FB7;
    case 0x3FB9: goto L3FB9;
    case 0x3FBB: goto L3FBB;
    case 0x3FBD: goto L3FBD;
    case 0x3FC1: goto L3FC1;
    case 0x3FC2: goto L3FC2;
    case 0x3FC4: goto L3FC4;
    case 0x3FC5: goto L3FC5;
    case 0x3FC7: goto L3FC7;
    case 0x3FC8: goto L3FC8;
    case 0x3FCA: goto L3FCA;
    case 0x3FCB: goto L3FCB;
    case 0x3FCC: goto L3FCC;
    case 0x3FCE: goto L3FCE;
    case 0x3FD0: goto L3FD0;
    case 0x3FD2: goto L3FD2;
    case 0x3FD6: goto L3FD6;
    case 0x3FD7: goto L3FD7;
    case 0x3FD9: goto L3FD9;
    case 0x3FDA: goto L3FDA;
    case 0x3FDC: goto L3FDC;
    case 0x3FDD: goto L3FDD;
    case 0x3FDF: goto L3FDF;
    case 0x3FE0: goto L3FE0;
    case 0x3FE1: goto L3FE1;
    case 0x3FE3: goto L3FE3;
    case 0x3FE5: goto L3FE5;
    case 0x3FE7: goto L3FE7;
    case 0x3FEB: goto L3FEB;
    case 0x3FEC: goto L3FEC;
    case 0x3FEE: goto L3FEE;
    case 0x3FEF: goto L3FEF;
    case 0x3FF1: goto L3FF1;
    case 0x3FF2: goto L3FF2;
    case 0x3FF4: goto L3FF4;
    case 0x3FF5: goto L3FF5;
    case 0x3FF6: goto L3FF6;
    case 0x3FF8: goto L3FF8;
    case 0x3FFA: goto L3FFA;
    case 0x3FFC: goto L3FFC;
    case 0x4000: goto L4000;
    case 0x4001: goto L4001;
    case 0x4003: goto L4003;
    case 0x4004: goto L4004;
    case 0x4006: goto L4006;
    case 0x4007: goto L4007;
    case 0x4009: goto L4009;
    case 0x400A: goto L400A;
    case 0x400B: goto L400B;
    case 0x400D: goto L400D;
    case 0x400F: goto L400F;
    case 0x4011: goto L4011;
    case 0x4015: goto L4015;
    case 0x4016: goto L4016;
    case 0x4018: goto L4018;
    case 0x4019: goto L4019;
    case 0x401B: goto L401B;
    case 0x401C: goto L401C;
    case 0x401E: goto L401E;
    case 0x401F: goto L401F;
    case 0x4020: goto L4020;
    case 0x4022: goto L4022;
    case 0x4024: goto L4024;
    case 0x4026: goto L4026;
    case 0x402A: goto L402A;
    case 0x402B: goto L402B;
    case 0x402D: goto L402D;
    case 0x402E: goto L402E;
    case 0x4030: goto L4030;
    case 0x4031: goto L4031;
    case 0x4033: goto L4033;
    case 0x4034: goto L4034;
    case 0x4035: goto L4035;
    case 0x4037: goto L4037;
    case 0x4039: goto L4039;
    case 0x403B: goto L403B;
    case 0x403F: goto L403F;
    case 0x4040: goto L4040;
    case 0x4042: goto L4042;
    case 0x4043: goto L4043;
    case 0x4045: goto L4045;
    case 0x4046: goto L4046;
    case 0x4048: goto L4048;
    case 0x4049: goto L4049;
    case 0x404A: goto L404A;
    case 0x404C: goto L404C;
    case 0x404E: goto L404E;
    case 0x4050: goto L4050;
    case 0x4054: goto L4054;
    case 0x4055: goto L4055;
    case 0x4057: goto L4057;
    case 0x4058: goto L4058;
    case 0x405A: goto L405A;
    case 0x405B: goto L405B;
    case 0x405D: goto L405D;
    case 0x405E: goto L405E;
    case 0x405F: goto L405F;
    case 0x4061: goto L4061;
    case 0x4063: goto L4063;
    case 0x4065: goto L4065;
    case 0x4069: goto L4069;
    case 0x406A: goto L406A;
    case 0x406C: goto L406C;
    case 0x406D: goto L406D;
    case 0x406F: goto L406F;
    case 0x4070: goto L4070;
    case 0x4072: goto L4072;
    case 0x4073: goto L4073;
    case 0x4074: goto L4074;
    case 0x4076: goto L4076;
    case 0x4078: goto L4078;
    case 0x407A: goto L407A;
    case 0x407E: goto L407E;
    case 0x407F: goto L407F;
    case 0x4081: goto L4081;
    case 0x4082: goto L4082;
    case 0x4084: goto L4084;
    case 0x4085: goto L4085;
    case 0x4087: goto L4087;
    case 0x4088: goto L4088;
    case 0x4089: goto L4089;
    case 0x408B: goto L408B;
    case 0x408D: goto L408D;
    case 0x408F: goto L408F;
    case 0x4093: goto L4093;
    case 0x4094: goto L4094;
    case 0x4096: goto L4096;
    case 0x4097: goto L4097;
    case 0x4099: goto L4099;
    case 0x409A: goto L409A;
    case 0x409C: goto L409C;
    case 0x409D: goto L409D;
    case 0x409E: goto L409E;
    case 0x40A0: goto L40A0;
    case 0x40A2: goto L40A2;
    case 0x40A4: goto L40A4;
    case 0x40A8: goto L40A8;
    case 0x40A9: goto L40A9;
    case 0x40AB: goto L40AB;
    case 0x40AC: goto L40AC;
    case 0x40AE: goto L40AE;
    case 0x40AF: goto L40AF;
    case 0x40B1: goto L40B1;
    case 0x40B2: goto L40B2;
    case 0x40B3: goto L40B3;
    case 0x40B5: goto L40B5;
    case 0x40B7: goto L40B7;
    case 0x40B9: goto L40B9;
    case 0x40BD: goto L40BD;
    case 0x40BE: goto L40BE;
    case 0x40C0: goto L40C0;
    case 0x40C1: goto L40C1;
    case 0x40C3: goto L40C3;
    case 0x40C4: goto L40C4;
    case 0x40C6: goto L40C6;
    case 0x40C7: goto L40C7;
    case 0x40C8: goto L40C8;
    case 0x40CA: goto L40CA;
    case 0x40CC: goto L40CC;
    case 0x40CE: goto L40CE;
    case 0x40D2: goto L40D2;
    case 0x40D3: goto L40D3;
    case 0x40D5: goto L40D5;
    case 0x40D6: goto L40D6;
    case 0x40D8: goto L40D8;
    case 0x40D9: goto L40D9;
    case 0x40DB: goto L40DB;
    case 0x40DC: goto L40DC;
    case 0x40DD: goto L40DD;
    case 0x40DF: goto L40DF;
    case 0x40E1: goto L40E1;
    case 0x40E3: goto L40E3;
    case 0x40E7: goto L40E7;
    case 0x40E8: goto L40E8;
    case 0x40EA: goto L40EA;
    case 0x40EB: goto L40EB;
    case 0x40ED: goto L40ED;
    case 0x40EE: goto L40EE;
    case 0x40F0: goto L40F0;
    case 0x40F1: goto L40F1;
    case 0x40F2: goto L40F2;
    case 0x40F4: goto L40F4;
    case 0x40F6: goto L40F6;
    case 0x40F8: goto L40F8;
    case 0x40FC: goto L40FC;
    case 0x40FD: goto L40FD;
    case 0x40FF: goto L40FF;
    case 0x4100: goto L4100;
    case 0x4102: goto L4102;
    case 0x4103: goto L4103;
    case 0x4105: goto L4105;
    case 0x4106: goto L4106;
    case 0x4107: goto L4107;
    case 0x4109: goto L4109;
    case 0x410B: goto L410B;
    case 0x410D: goto L410D;
    case 0x4111: goto L4111;
    case 0x4112: goto L4112;
    case 0x4114: goto L4114;
    case 0x4115: goto L4115;
    case 0x4117: goto L4117;
    case 0x4118: goto L4118;
    case 0x411A: goto L411A;
    case 0x411B: goto L411B;
    case 0x411D: goto L411D;
    case 0x411F: goto L411F;
    case 0x4121: goto L4121;
    case 0x4124: goto L4124;
    case 0x4125: goto L4125;
    case 0x4126: goto L4126;
    case 0x4129: goto L4129;
    case 0x412B: goto L412B;
    case 0x412F: goto L412F;
    case 0x4132: goto L4132;
    case 0x4136: goto L4136;
    case 0x413A: goto L413A;
    case 0x413E: goto L413E;
    case 0x4141: goto L4141;
    case 0x4142: goto L4142;
    case 0x4145: goto L4145;
    case 0x4146: goto L4146;
    case 0x4148: goto L4148;
    case 0x414A: goto L414A;
    case 0x414C: goto L414C;
    case 0x414E: goto L414E;
    case 0x4150: goto L4150;
    case 0x4151: goto L4151;
    case 0x4152: goto L4152;
    case 0x4158: goto L4158;
    case 0x415A: goto L415A;
    case 0x415E: goto L415E;
    case 0x4163: goto L4163;
    case 0x4167: goto L4167;
    case 0x416B: goto L416B;
    case 0x416E: goto L416E;
    case 0x4172: goto L4172;
    case 0x4173: goto L4173;
    case 0x4177: goto L4177;
    case 0x4179: goto L4179;
    case 0x417B: goto L417B;
    case 0x417D: goto L417D;
    case 0x417F: goto L417F;
    case 0x4181: goto L4181;
    case 0x4182: goto L4182;
    case 0x4186: goto L4186;
    case 0x4188: goto L4188;
    case 0x418B: goto L418B;
    case 0x418E: goto L418E;
    case 0x4191: goto L4191;
    case 0x4194: goto L4194;
    case 0x4197: goto L4197;
    case 0x419A: goto L419A;
    case 0x419D: goto L419D;
    case 0x41A0: goto L41A0;
    case 0x41A3: goto L41A3;
    case 0x41A6: goto L41A6;
    case 0x41A9: goto L41A9;
    case 0x41AC: goto L41AC;
    case 0x41AF: goto L41AF;
    case 0x41B2: goto L41B2;
    case 0x41B5: goto L41B5;
    case 0x41B8: goto L41B8;
    case 0x41BB: goto L41BB;
    case 0x41BE: goto L41BE;
    case 0x41C1: goto L41C1;
    case 0x41C4: goto L41C4;
    case 0x41C7: goto L41C7;
    case 0x41CA: goto L41CA;
    case 0x41CD: goto L41CD;
    case 0x41D0: goto L41D0;
    case 0x41D3: goto L41D3;
    case 0x41D6: goto L41D6;
    case 0x41D9: goto L41D9;
    case 0x41DC: goto L41DC;
    case 0x41DF: goto L41DF;
    case 0x41E2: goto L41E2;
    case 0x41E5: goto L41E5;
    case 0x41E8: goto L41E8;
    case 0x41EB: goto L41EB;
    case 0x41EE: goto L41EE;
    case 0x41F1: goto L41F1;
    case 0x41F4: goto L41F4;
    case 0x41F7: goto L41F7;
    case 0x41FA: goto L41FA;
    case 0x41FD: goto L41FD;
    case 0x4200: goto L4200;
    case 0x4203: goto L4203;
    case 0x4206: goto L4206;
    case 0x4209: goto L4209;
    case 0x420C: goto L420C;
    case 0x420F: goto L420F;
    case 0x4212: goto L4212;
    case 0x4215: goto L4215;
    case 0x4218: goto L4218;
    case 0x421B: goto L421B;
    case 0x421E: goto L421E;
    case 0x4221: goto L4221;
    case 0x4224: goto L4224;
    case 0x4225: goto L4225;
    case 0x4227: goto L4227;
    case 0x422A: goto L422A;
    case 0x422C: goto L422C;
    case 0x422F: goto L422F;
    case 0x4232: goto L4232;
    case 0x4234: goto L4234;
    case 0x4236: goto L4236;
    case 0x4238: goto L4238;
    case 0x423A: goto L423A;
    case 0x423C: goto L423C;
    case 0x423D: goto L423D;
    case 0x4241: goto L4241;
    case 0x4243: goto L4243;
    case 0x4246: goto L4246;
    case 0x4248: goto L4248;
    case 0x424A: goto L424A;
    case 0x424B: goto L424B;
    case 0x424F: goto L424F;
    case 0x4252: goto L4252;
    case 0x4254: goto L4254;
    case 0x4255: goto L4255;
    case 0x4259: goto L4259;
    case 0x425B: goto L425B;
    case 0x425E: goto L425E;
    case 0x4260: goto L4260;
    case 0x4261: goto L4261;
    case 0x4263: goto L4263;
    case 0x4265: goto L4265;
    case 0x4269: goto L4269;
    case 0x426B: goto L426B;
    case 0x4270: goto L4270;
    case 0x4273: goto L4273;
    case 0x4275: goto L4275;
    case 0x4279: goto L4279;
    case 0x427C: goto L427C;
    case 0x427E: goto L427E;
    case 0x4280: goto L4280;
    case 0x4282: goto L4282;
    case 0x4285: goto L4285;
    case 0x4287: goto L4287;
    case 0x4289: goto L4289;
    case 0x428B: goto L428B;
    case 0x428D: goto L428D;
    case 0x428F: goto L428F;
    case 0x4291: goto L4291;
    case 0x4293: goto L4293;
    case 0x4296: goto L4296;
    case 0x4298: goto L4298;
    case 0x4299: goto L4299;
    case 0x429C: goto L429C;
    case 0x429E: goto L429E;
    case 0x42A0: goto L42A0;
    case 0x42A1: goto L42A1;
    case 0x42A3: goto L42A3;
    case 0x42A5: goto L42A5;
    case 0x42A7: goto L42A7;
    case 0x42A9: goto L42A9;
    case 0x42AC: goto L42AC;
    case 0x42AE: goto L42AE;
    case 0x42B0: goto L42B0;
    case 0x42B2: goto L42B2;
    case 0x42B4: goto L42B4;
    case 0x42B6: goto L42B6;
    case 0x42B8: goto L42B8;
    case 0x42BA: goto L42BA;
    case 0x42BE: goto L42BE;
    case 0x42C1: goto L42C1;
    case 0x42C3: goto L42C3;
    case 0x42C4: goto L42C4;
    case 0x42C6: goto L42C6;
    case 0x42CA: goto L42CA;
    case 0x42CC: goto L42CC;
    case 0x42CD: goto L42CD;
    case 0x42D1: goto L42D1;
    case 0x42D3: goto L42D3;
    case 0x42D5: goto L42D5;
    case 0x42D7: goto L42D7;
    case 0x42D9: goto L42D9;
    case 0x42DA: goto L42DA;
    case 0x42DE: goto L42DE;
    case 0x42E0: goto L42E0;
    case 0x42E2: goto L42E2;
    case 0x42E4: goto L42E4;
    case 0x42E6: goto L42E6;
    case 0x42E7: goto L42E7;
    case 0x42EB: goto L42EB;
    case 0x42ED: goto L42ED;
    case 0x42EF: goto L42EF;
    case 0x42F1: goto L42F1;
    case 0x42F3: goto L42F3;
    case 0x42F4: goto L42F4;
    case 0x42F8: goto L42F8;
    case 0x42FA: goto L42FA;
    case 0x42FC: goto L42FC;
    case 0x42FE: goto L42FE;
    case 0x4300: goto L4300;
    case 0x4301: goto L4301;
    case 0x4305: goto L4305;
    case 0x4307: goto L4307;
    case 0x4309: goto L4309;
    case 0x430B: goto L430B;
    case 0x430D: goto L430D;
    case 0x430E: goto L430E;
    case 0x4312: goto L4312;
    case 0x4314: goto L4314;
    case 0x4316: goto L4316;
    case 0x4318: goto L4318;
    case 0x431A: goto L431A;
    case 0x431B: goto L431B;
    case 0x431F: goto L431F;
    case 0x4321: goto L4321;
    case 0x4323: goto L4323;
    case 0x4325: goto L4325;
    case 0x4327: goto L4327;
    case 0x4328: goto L4328;
    case 0x432C: goto L432C;
    case 0x432E: goto L432E;
    case 0x4330: goto L4330;
    case 0x4332: goto L4332;
    case 0x4334: goto L4334;
    case 0x4335: goto L4335;
    case 0x4339: goto L4339;
    case 0x433B: goto L433B;
    case 0x433D: goto L433D;
    case 0x433F: goto L433F;
    case 0x4341: goto L4341;
    case 0x4342: goto L4342;
    case 0x4346: goto L4346;
    case 0x4348: goto L4348;
    case 0x434A: goto L434A;
    case 0x434C: goto L434C;
    case 0x434E: goto L434E;
    case 0x434F: goto L434F;
    case 0x4353: goto L4353;
    case 0x4355: goto L4355;
    case 0x4357: goto L4357;
    case 0x4359: goto L4359;
    case 0x435B: goto L435B;
    case 0x435C: goto L435C;
    case 0x4360: goto L4360;
    case 0x4362: goto L4362;
    case 0x4364: goto L4364;
    case 0x4366: goto L4366;
    case 0x4368: goto L4368;
    case 0x4369: goto L4369;
    case 0x436D: goto L436D;
    case 0x436F: goto L436F;
    case 0x4371: goto L4371;
    case 0x4373: goto L4373;
    case 0x4375: goto L4375;
    case 0x4376: goto L4376;
    case 0x437A: goto L437A;
    case 0x437C: goto L437C;
    case 0x437E: goto L437E;
    case 0x4380: goto L4380;
    case 0x4382: goto L4382;
    case 0x4383: goto L4383;
    case 0x4387: goto L4387;
    case 0x4389: goto L4389;
    case 0x438B: goto L438B;
    case 0x438D: goto L438D;
    case 0x438F: goto L438F;
    case 0x4390: goto L4390;
    case 0x4394: goto L4394;
    case 0x4396: goto L4396;
    case 0x4398: goto L4398;
    case 0x439A: goto L439A;
    case 0x439D: goto L439D;
    case 0x439F: goto L439F;
    case 0x43A0: goto L43A0;
    case 0x43A1: goto L43A1;
    case 0x43A4: goto L43A4;
    case 0x43C5: goto L43C5;
    case 0x43C9: goto L43C9;
    case 0x43CD: goto L43CD;
    case 0x43CF: goto L43CF;
    case 0x43D2: goto L43D2;
    case 0x43D4: goto L43D4;
    case 0x43D6: goto L43D6;
    case 0x43D7: goto L43D7;
    case 0x43D9: goto L43D9;
    case 0x43DB: goto L43DB;
    case 0x43DD: goto L43DD;
    case 0x43DF: goto L43DF;
    case 0x43E3: goto L43E3;
    case 0x43E5: goto L43E5;
    case 0x43E8: goto L43E8;
    case 0x43EA: goto L43EA;
    case 0x43EC: goto L43EC;
    case 0x43EE: goto L43EE;
    default: asm_bad_entry("GRLIBI.ASM", entry);
    }

    /* seg003_0272_38B4  (+38B4)
       upolygon: fill a polygon already inside the window. In: CX = the vertex count, the vertices
       (x, y words) from 415E. Finds the top vertex, steps the two chains (_395C going one way round,
       _3989 the other) to fill the row table, and jumps to the span writer. A polygon of zero height
       becomes one span from its least to its greatest x. */
L38B4: /* _seg003_0272_38B4 */
    /* 38B4  mov     si,word ptr ds:[49ACh] */
    SI = rw(pDS, 0x49AC);
L38B8:
    /* 38B8  and     word ptr [si],7FFFh */
    ww(pDS, SI, (uint16_t)(rw(pDS, SI) & 0x7FFF));
L38BC:
    /* 38BC  mov     word ptr ds:[49B8h],cx */
    ww(pDS, 0x49B8, CX);
L38C0:
    /* 38C0  mov     word ptr ds:[49BAh],cx */
    ww(pDS, 0x49BA, CX);
L38C4:
    /* 38C4  mov     word ptr ds:[49BCh],cx */
    ww(pDS, 0x49BC, CX);
L38C8:
    /* 38C8  mov     di,0FC19h */
    DI = 0xFC19;
L38CB:
    /* 38CB  mov     si,4160h */
    SI = 0x4160;
L38CE:
    /* 38CE  mov     bp,415Eh */
    BP = 0x415E;
L38D1:
    /* 38D1  jmp     short L38DC */
    goto L38DC;
L38D3: /* L38D3 */
    /* 38D3  mov     bp,si */
    BP = SI;
L38D5:
    /* 38D5  sub     bp,6 */
    BP = (uint16_t)(BP - 0x6);
L38D8:
    /* 38D8  xchg    di,ax */
    { uint16_t t_ = DI;
    DI = AX;
    AX = t_; }
L38D9:
    /* 38D9  dec     cx */
    CX = dec16(CX);
L38DA:
    /* 38DA  je      L38E5 */
    if (ZF) goto L38E5;
L38DC: /* L38DC */
    /* 38DC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L38DD:
    /* 38DD  inc     si */
    SI = (uint16_t)(SI + 1);
L38DE:
    /* 38DE  inc     si */
    SI = (uint16_t)(SI + 1);
L38DF:
    /* 38DF  cmp     di,ax */
    sub16(DI, AX, 0);
L38E1:
    /* 38E1  jl      L38D3 */
    if (SF != OF) goto L38D3;
L38E3:
    /* 38E3  loop    L38DC */
    if (--CX) goto L38DC;
L38E5: /* L38E5 */
    /* 38E5  sub     si,4 */
    SI = (uint16_t)(SI - 0x4);
L38E8:
    /* 38E8  mov     word ptr ds:[49C2h],si */
    ww(pDS, 0x49C2, SI);
L38EC:
    /* 38EC  mov     si,bp */
    SI = BP;
L38EE:
    /* 38EE  mov     word ptr ds:[49C0h],si */
    ww(pDS, 0x49C0, SI);
L38F2:
    /* 38F2  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L38F4:
    /* 38F4  mov     ax,di */
    AX = DI;
L38F6:
    /* 38F6  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L38F8:
    /* 38F8  add     di,ax */
    DI = (uint16_t)(DI + AX);
L38FA:
    /* 38FA  neg     di */
    DI = (uint16_t)-DI;
L38FC:
    /* 38FC  add     di,4978h */
    DI = (uint16_t)(DI + 0x4978);
L3900:
    /* 3900  mov     word ptr ds:[49BEh],di */
    ww(pDS, 0x49BE, DI);
L3904:
    /* 3904  call    _seg003_0272_395C */
    if ((c = asm_call(ASM_JMP(0x0085, 0x395C), 0x3907)) != 0) return c;
L3907:
    /* 3907  mov     si,word ptr ds:[49C0h] */
    SI = rw(pDS, 0x49C0);
L390B:
    /* 390B  mov     di,word ptr ds:[49BEh] */
    DI = rw(pDS, 0x49BE);
L390F:
    /* 390F  call    _seg003_0272_3989 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x3989), 0x3912)) != 0) return c;
L3912:
    /* 3912  sub     di,4 */
    DI = (uint16_t)(DI - 0x4);
L3915:
    /* 3915  mov     bp,word ptr ds:[49BEh] */
    BP = rw(pDS, 0x49BE);
L3919:
    /* 3919  mov     ax,word ptr ds:[4A08h] */
    AX = rw(pDS, 0x4A08);
L391C:
    /* 391C  mov     bx,word ptr ds:[4A0Ah] */
    BX = rw(pDS, 0x4A0A);
L3920:
    /* 3920  cmp     bp,di */
    sub16(BP, DI, 0);
L3922:
    /* 3922  jne     L3944 */
    if (!ZF) goto L3944;
L3924:
    /* 3924  mov     si,415Eh */
    SI = 0x415E;
L3927:
    /* 3927  mov     bx,270Fh */
    BX = 0x270F;
L392A:
    /* 392A  mov     dx,0D8F1h */
    DX = 0xD8F1;
L392D:
    /* 392D  mov     cx,word ptr ds:[49B8h] */
    CX = rw(pDS, 0x49B8);
L3931: /* L3931 */
    /* 3931  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3932:
    /* 3932  cmp     ax,bx */
    sub16(AX, BX, 0);
L3934:
    /* 3934  jg      L3938 */
    if (!ZF && SF == OF) goto L3938;
L3936:
    /* 3936  mov     bx,ax */
    BX = AX;
L3938: /* L3938 */
    /* 3938  cmp     ax,dx */
    sub16(AX, DX, 0);
L393A:
    /* 393A  jl      L393E */
    if (SF != OF) goto L393E;
L393C:
    /* 393C  mov     dx,ax */
    DX = AX;
L393E: /* L393E */
    /* 393E  inc     si */
    SI = (uint16_t)(SI + 1);
L393F:
    /* 393F  inc     si */
    SI = (uint16_t)(SI + 1);
L3940:
    /* 3940  loop    L3931 */
    if (--CX) goto L3931;
L3942:
    /* 3942  mov     ax,dx */
    AX = DX;
L3944: /* L3944 */
    /* 3944  mov     si,bp */
    SI = BP;
L3946:
    /* 3946  inc     di */
    DI = (uint16_t)(DI + 1);
L3947:
    /* 3947  inc     di */
    DI = (uint16_t)(DI + 1);
L3948:
    /* 3948  cmp     ax,bx */
    sub16(AX, BX, 0);
L394A:
    /* 394A  jle     L394D */
    if (ZF || SF != OF) goto L394D;
L394C:
    /* 394C  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L394D: /* L394D */
    /* 394D  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L394E:
    /* 394E  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L394F:
    /* 394F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3950:
    /* 3950  mov     word ptr ds:[49ACh],di */
    ww(pDS, 0x49AC, DI);
L3954:
    /* 3954  or      word ptr [di],8000h */
    ww(pDS, DI, logic16((uint16_t)(rw(pDS, DI) | 0x8000)));
L3958:
    /* 3958  jmp     word ptr ds:[4112h] */
    return ASM_JMP(0x0085, rw(pDS, 0x4112));

    /* seg003_0272_395C  (+395C)
       _395C: walk the chain from the top vertex backwards through the vertex list, filling one side
       of the row records (through _39BB) until an edge turns upwards. */
L395C: /* _seg003_0272_395C */
    /* 395C  inc     di */
    DI = (uint16_t)(DI + 1);
L395D:
    /* 395D  inc     di */
    DI = (uint16_t)(DI + 1);
L395E:
    /* 395E  inc     si */
    SI = (uint16_t)(SI + 1);
L395F:
    /* 395F  inc     si */
    SI = (uint16_t)(SI + 1);
L3960: /* L3960 */
    /* 3960  std */
    DF = 1;
L3961:
    /* 3961  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3962:
    /* 3962  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3963:
    /* 3963  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3964:
    /* 3964  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3965:
    /* 3965  cmp     si,4160h */
    sub16(SI, 0x4160, 0);
L3969:
    /* 3969  jae     L396F */
    if (!CF) goto L396F;
L396B:
    /* 396B  mov     si,word ptr ds:[49C2h] */
    SI = rw(pDS, 0x49C2);
L396F: /* L396F */
    /* 396F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3970:
    /* 3970  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L3971:
    /* 3971  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L3972:
    /* 3972  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L3973:
    /* 3973  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L3976:
    /* 3976  cld */
    DF = 0;
L3977:
    /* 3977  cmp     bx,dx */
    sub16(BX, DX, 0);
L3979:
    /* 3979  jl      L3988 */
    if (SF != OF) goto L3988;
L397B:
    /* 397B  mov     word ptr ds:[4A08h],cx */
    ww(pDS, 0x4A08, CX);
L397F:
    /* 397F  call    _seg003_0272_39BB */
    if ((c = asm_call(ASM_JMP(0x0085, 0x39BB), 0x3982)) != 0) return c;
L3982:
    /* 3982  dec     word ptr ds:[49BAh] */
    ww(pDS, 0x49BA, dec16(rw(pDS, 0x49BA)));
L3986:
    /* 3986  jg      L3960 */
    if (!ZF && SF == OF) goto L3960;
L3988: /* L3988 */
    /* 3988  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_3989  (+3989)
       _3989: the same walk forwards, for the other side. */
L3989: /* _seg003_0272_3989 */
    /* 3989  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L398C: /* L398C */
    /* 398C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L398D:
    /* 398D  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L398E:
    /* 398E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L398F:
    /* 398F  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L3990:
    /* 3990  cmp     si,word ptr ds:[49C2h] */
    sub16(SI, rw(pDS, 0x49C2), 0);
L3994:
    /* 3994  jb      L3999 */
    if (CF) goto L3999;
L3996:
    /* 3996  mov     si,415Eh */
    SI = 0x415E;
L3999: /* L3999 */
    /* 3999  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L399A:
    /* 399A  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L399B:
    /* 399B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L399C:
    /* 399C  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L399D:
    /* 399D  sub     si,4 */
    SI = (uint16_t)(SI - 0x4);
L39A0:
    /* 39A0  cmp     bx,dx */
    sub16(BX, DX, 0);
L39A2:
    /* 39A2  jl      L39B1 */
    if (SF != OF) goto L39B1;
L39A4:
    /* 39A4  mov     word ptr ds:[4A0Ah],cx */
    ww(pDS, 0x4A0A, CX);
L39A8:
    /* 39A8  call    _seg003_0272_39BB */
    if ((c = asm_call(ASM_JMP(0x0085, 0x39BB), 0x39AB)) != 0) return c;
L39AB:
    /* 39AB  dec     word ptr ds:[49BCh] */
    ww(pDS, 0x49BC, dec16(rw(pDS, 0x49BC)));
L39AF:
    /* 39AF  jg      L398C */
    if (!ZF && SF == OF) goto L398C;
L39B1: /* L39B1 */
    /* 39B1  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L39B2: /* L39B2 */
    /* 39B2  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L39B4:
    /* 39B4  sub     cx,cx */
    CX = (uint16_t)(CX - CX);
L39B6:
    /* 39B6  sub     dx,dx */
    DX = (uint16_t)(DX - DX);
L39B8:
    /* 39B8  jmp     short L39E5 */
    goto L39E5;
L39BA: /* L39BA */
    /* 39BA  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_39BB  (+39BB)
       _39BB: one edge, from (AX, BX) to (CX, DX): its rows from BX down to DX get the stepped x
       (16.16 slope from two divides), 31 rows a call of the unrolled _3A02 and the remainder through
       the entry table at 49C4. */
L39BB: /* _seg003_0272_39BB */
    /* 39BB  sub     bx,dx */
    BX = sub16(BX, DX, 0);
L39BD:
    /* 39BD  mov     word ptr ds:[4A06h],bx */
    ww(pDS, 0x4A06, BX);
L39C1:
    /* 39C1  mov     bp,bx */
    BP = BX;
L39C3:
    /* 39C3  jle     L39BA */
    if (ZF || SF != OF) goto L39BA;
L39C5:
    /* 39C5  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L39C6:
    /* 39C6  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L39C9:
    /* 39C9  sub     cx,ax */
    CX = sub16(CX, AX, 0);
L39CB:
    /* 39CB  je      L39B2 */
    if (ZF) goto L39B2;
L39CD:
    /* 39CD  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L39CE:
    /* 39CE  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L39CF:
    /* 39CF  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x0085, 0x39CF, 2)) != 0) return c;
L39D1:
    /* 39D1  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L39D2:
    /* 39D2  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L39D4:
    /* 39D4  sar     dx,1 */
    DX = sar16(DX, 1);
L39D6:
    /* 39D6  rcr     ax,1 */
    AX = rcr16(AX, 1);
L39D8:
    /* 39D8  idiv    bp */
    if (asm_idiv16(BP) && (c = asm_divfault(0x0085, 0x39D8, 2)) != 0) return c;
L39DA:
    /* 39DA  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L39DB:
    /* 39DB  shl     ax,1 */
    AX = shl16(AX, 1);
L39DD:
    /* 39DD  rcl     dx,1 */
    DX = rcl16(DX, 1);
L39DF:
    /* 39DF  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L39E1:
    /* 39E1  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L39E2:
    /* 39E2  mov     dx,7FFFh */
    DX = 0x7FFF;
L39E5: /* L39E5 */
    /* 39E5  cmp     bp,20h */
    sub16(BP, 0x20, 0);
L39E8:
    /* 39E8  ja      L39F0 */
    if (!CF && !ZF) goto L39F0;
L39EA:
    /* 39EA  shl     bp,1 */
    BP = shl16(BP, 1);
L39EC:
    /* 39EC  jmp     word ptr [bp+49C4h] */
    return ASM_JMP(0x0085, rw(pSS, BP + 0x49C4));
L39F0: /* L39F0 */
    /* 39F0  call    _seg003_0272_3A02 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x3A02), 0x39F3)) != 0) return c;
L39F3:
    /* 39F3  sub     bp,1Fh */
    BP = (uint16_t)(BP - 0x1F);
L39F6:
    /* 39F6  cmp     bp,20h */
    sub16(BP, 0x20, 0);
L39F9:
    /* 39F9  ja      L39F0 */
    if (!CF && !ZF) goto L39F0;
L39FB:
    /* 39FB  shl     bp,1 */
    BP = shl16(BP, 1);
L39FD:
    /* 39FD  jmp     word ptr [bp+49C4h] */
    return ASM_JMP(0x0085, rw(pSS, BP + 0x49C4));
L3A01:
    /* 3A01  even */
    ;

    /* seg003_0272_3A02  (+3A02)
       _3A02: 31 rows of an edge, unrolled: add the slope to the 16.16 x and store it in the next
       record. L3AFB, after it, is polygon's case for one or two vertices: a line through cline_si or
       a point through pixel. */
L3A02: /* _seg003_0272_3A02 */
    /* 3A02  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A04:
    /* 3A04  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A06:
    /* 3A06  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A07:
    /* 3A07  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A0A:
    /* 3A0A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A0C:
    /* 3A0C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A0E:
    /* 3A0E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A0F:
    /* 3A0F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A12:
    /* 3A12  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A14:
    /* 3A14  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A16:
    /* 3A16  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A17:
    /* 3A17  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A1A:
    /* 3A1A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A1C:
    /* 3A1C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A1E:
    /* 3A1E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A1F:
    /* 3A1F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A22:
    /* 3A22  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A24:
    /* 3A24  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A26:
    /* 3A26  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A27:
    /* 3A27  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A2A:
    /* 3A2A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A2C:
    /* 3A2C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A2E:
    /* 3A2E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A2F:
    /* 3A2F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A32:
    /* 3A32  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A34:
    /* 3A34  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A36:
    /* 3A36  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A37:
    /* 3A37  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A3A:
    /* 3A3A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A3C:
    /* 3A3C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A3E:
    /* 3A3E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A3F:
    /* 3A3F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A42:
    /* 3A42  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A44:
    /* 3A44  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A46:
    /* 3A46  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A47:
    /* 3A47  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A4A:
    /* 3A4A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A4C:
    /* 3A4C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A4E:
    /* 3A4E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A4F:
    /* 3A4F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A52:
    /* 3A52  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A54:
    /* 3A54  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A56:
    /* 3A56  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A57:
    /* 3A57  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A5A:
    /* 3A5A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A5C:
    /* 3A5C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A5E:
    /* 3A5E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A5F:
    /* 3A5F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A62:
    /* 3A62  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A64:
    /* 3A64  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A66:
    /* 3A66  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A67:
    /* 3A67  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A6A:
    /* 3A6A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A6C:
    /* 3A6C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A6E:
    /* 3A6E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A6F:
    /* 3A6F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A72:
    /* 3A72  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A74:
    /* 3A74  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A76:
    /* 3A76  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A77:
    /* 3A77  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A7A:
    /* 3A7A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A7C:
    /* 3A7C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A7E:
    /* 3A7E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A7F:
    /* 3A7F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A82:
    /* 3A82  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A84:
    /* 3A84  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A86:
    /* 3A86  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A87:
    /* 3A87  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A8A:
    /* 3A8A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A8C:
    /* 3A8C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A8E:
    /* 3A8E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A8F:
    /* 3A8F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A92:
    /* 3A92  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A94:
    /* 3A94  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A96:
    /* 3A96  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A97:
    /* 3A97  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3A9A:
    /* 3A9A  add     dx,cx */
    DX = add16(DX, CX, 0);
L3A9C:
    /* 3A9C  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3A9E:
    /* 3A9E  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3A9F:
    /* 3A9F  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3AA2:
    /* 3AA2  add     dx,cx */
    DX = add16(DX, CX, 0);
L3AA4:
    /* 3AA4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3AA6:
    /* 3AA6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3AA7:
    /* 3AA7  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3AAA:
    /* 3AAA  add     dx,cx */
    DX = add16(DX, CX, 0);
L3AAC:
    /* 3AAC  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3AAE:
    /* 3AAE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3AAF:
    /* 3AAF  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3AB2:
    /* 3AB2  add     dx,cx */
    DX = add16(DX, CX, 0);
L3AB4:
    /* 3AB4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3AB6:
    /* 3AB6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3AB7:
    /* 3AB7  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3ABA:
    /* 3ABA  add     dx,cx */
    DX = add16(DX, CX, 0);
L3ABC:
    /* 3ABC  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3ABE:
    /* 3ABE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3ABF:
    /* 3ABF  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3AC2:
    /* 3AC2  add     dx,cx */
    DX = add16(DX, CX, 0);
L3AC4:
    /* 3AC4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3AC6:
    /* 3AC6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3AC7:
    /* 3AC7  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3ACA:
    /* 3ACA  add     dx,cx */
    DX = add16(DX, CX, 0);
L3ACC:
    /* 3ACC  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3ACE:
    /* 3ACE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3ACF:
    /* 3ACF  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3AD2:
    /* 3AD2  add     dx,cx */
    DX = add16(DX, CX, 0);
L3AD4:
    /* 3AD4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3AD6:
    /* 3AD6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3AD7:
    /* 3AD7  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3ADA:
    /* 3ADA  add     dx,cx */
    DX = add16(DX, CX, 0);
L3ADC:
    /* 3ADC  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3ADE:
    /* 3ADE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3ADF:
    /* 3ADF  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3AE2:
    /* 3AE2  add     dx,cx */
    DX = add16(DX, CX, 0);
L3AE4:
    /* 3AE4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3AE6:
    /* 3AE6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3AE7:
    /* 3AE7  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3AEA:
    /* 3AEA  add     dx,cx */
    DX = add16(DX, CX, 0);
L3AEC:
    /* 3AEC  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3AEE:
    /* 3AEE  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3AEF:
    /* 3AEF  add     di,4 */
    DI = (uint16_t)(DI + 0x4);
L3AF2:
    /* 3AF2  add     dx,cx */
    DX = add16(DX, CX, 0);
L3AF4:
    /* 3AF4  adc     ax,bx */
    AX = (uint16_t)(AX + BX + CF);
L3AF6:
    /* 3AF6  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3AF7:
    /* 3AF7  add     di,4 */
    DI = add16(DI, 0x4, 0);
L3AFA:
    /* 3AFA  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3AFB: /* L3AFB */
    /* 3AFB  jne     L3B03 */
    if (!ZF) goto L3B03;
L3AFD:
    /* 3AFD  mov     si,415Eh */
    SI = 0x415E;
L3B00:
    /* 3B00  jmp     _seg003_0272_3686 */
    return ASM_JMP(0x0085, 0x3686);
L3B03: /* L3B03 */
    /* 3B03  mov     ax,word ptr ds:[415Eh] */
    AX = rw(pDS, 0x415E);
L3B06:
    /* 3B06  mov     bx,word ptr ds:[4160h] */
    BX = rw(pDS, 0x4160);
L3B0A:
    /* 3B0A  jmp     _seg003_0272_32B4 */
    return ASM_JMP(0x0085, 0x32B4);

    /* seg003_0272_3B0D  (+3B0D)
       polygon: CX = the vertex count (more than 99 is refused), vertices at 415E. Two or fewer go to
       L3AFB; otherwise shclip (GRLIBF.ASM), then upolygon. GRCORE's _4EFA is the C wrapper. */
L3B0D: /* _seg003_0272_3B0D */
    /* 3B0D  cmp     cx,63h */
    sub16(CX, 0x63, 0);
L3B10:
    /* 3B10  ja      L3B1D */
    if (!CF && !ZF) goto L3B1D;
L3B12:
    /* 3B12  cmp     cx,2 */
    sub16(CX, 0x2, 0);
L3B15:
    /* 3B15  jbe     L3AFB */
    if (CF || ZF) goto L3AFB;
L3B17:
    /* 3B17  call    _seg003_0272_34C7 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x34C7), 0x3B1A)) != 0) return c;
L3B1A:
    /* 3B1A  jmp     _seg003_0272_38B4 */
    goto L38B4;
L3B1D: /* L3B1D */
    /* 3B1D  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_3B1E  (+3B1E)
       _3B1E, _3B26, _3B2E: string entries (see _3B36). _3B1E and _3B26 use the bitmap blitter L4126
       (_3B1E clipped, _3B26 unclipped); _3B2E is the clipped single-colour entry with the colour in
       DX. GRCORE reaches them through the jump table at 527E .. 528D. */
L3B1E: /* _seg003_0272_3B1E */
    /* 3B1E  mov     word ptr ds:[4D16h],offset L4126 */
    ww(pDS, 0x4D16, 0x4126);
L3B24:
    /* 3B24  jmp     short L3B4A */
    goto L3B4A;
L3B26: /* _seg003_0272_3B26 */
    /* 3B26  mov     word ptr ds:[4D16h],offset L4126 */
    ww(pDS, 0x4D16, 0x4126);
L3B2C:
    /* 3B2C  jmp     short L3B83 */
    goto L3B83;
L3B2E: /* _seg003_0272_3B2E */
    /* 3B2E  mov     word ptr ds:[4D16h],offset _seg003_0272_3C61 */
    ww(pDS, 0x4D16, 0x3C61);
L3B34:
    /* 3B34  jmp     short L3B46 */
    goto L3B46;

    /* seg003_0272_3B36  (+3B36)
       _3B36 (string_to_screen, FM Towns): draw the string at SI with its top-left corner at (AX, BX)
       in the colour at 2D3A, through the single-colour blitter. The clipped entries give up unless
       the whole text box (font height up from y, and x) is inside the window: text is clipped whole,
       not cut. _3B3E is the unclipped entry (ustring_to_screen). */
L3B36: /* _seg003_0272_3B36 */
    /* 3B36  mov     word ptr ds:[4D16h],offset _seg003_0272_3C61 */
    ww(pDS, 0x4D16, 0x3C61);
L3B3C:
    /* 3B3C  jmp     short L3B4A */
    goto L3B4A;
L3B3E: /* _seg003_0272_3B3E */
    /* 3B3E  mov     word ptr ds:[4D16h],offset _seg003_0272_3C61 */
    ww(pDS, 0x4D16, 0x3C61);
L3B44:
    /* 3B44  jmp     short L3B83 */
    goto L3B83;
L3B46: /* L3B46 */
    /* 3B46  mov     word ptr ds:[2D3Ah],dx */
    ww(pDS, 0x2D3A, DX);
L3B4A: /* L3B4A */
    /* 3B4A  mov     word ptr ds:[4A0Eh],ax */
    ww(pDS, 0x4A0E, AX);
L3B4D:
    /* 3B4D  mov     word ptr ds:[4A10h],bx */
    ww(pDS, 0x4A10, BX);
L3B51:
    /* 3B51  mov     ax,word ptr ds:[4A10h] */
    AX = rw(pDS, 0x4A10);
L3B54:
    /* 3B54  cmp     ax,word ptr ds:[3DFAh] */
    sub16(AX, rw(pDS, 0x3DFA), 0);
L3B58:
    /* 3B58  jl      L3B8E */
    if (SF != OF) goto L3B8E;
L3B5A:
    /* 3B5A  cmp     ax,word ptr ds:[3DF6h] */
    sub16(AX, rw(pDS, 0x3DF6), 0);
L3B5E:
    /* 3B5E  jg      L3B8E */
    if (!ZF && SF == OF) goto L3B8E;
L3B60:
    /* 3B60  sub     ax,word ptr ds:[4D0Eh] */
    AX = (uint16_t)(AX - rw(pDS, 0x4D0E));
L3B64:
    /* 3B64  inc     ax */
    AX = (uint16_t)(AX + 1);
L3B65:
    /* 3B65  mov     word ptr ds:[4A14h],ax */
    ww(pDS, 0x4A14, AX);
L3B68:
    /* 3B68  cmp     ax,word ptr ds:[3DFAh] */
    sub16(AX, rw(pDS, 0x3DFA), 0);
L3B6C:
    /* 3B6C  jl      L3B8E */
    if (SF != OF) goto L3B8E;
L3B6E:
    /* 3B6E  cmp     ax,word ptr ds:[3DF6h] */
    sub16(AX, rw(pDS, 0x3DF6), 0);
L3B72:
    /* 3B72  jg      L3B8E */
    if (!ZF && SF == OF) goto L3B8E;
L3B74:
    /* 3B74  mov     ax,word ptr ds:[4A0Eh] */
    AX = rw(pDS, 0x4A0E);
L3B77:
    /* 3B77  cmp     ax,word ptr ds:[3DF4h] */
    sub16(AX, rw(pDS, 0x3DF4), 0);
L3B7B:
    /* 3B7B  jl      L3B8E */
    if (SF != OF) goto L3B8E;
L3B7D:
    /* 3B7D  cmp     ax,word ptr ds:[3DF8h] */
    sub16(AX, rw(pDS, 0x3DF8), 0);
L3B81:
    /* 3B81  jg      L3B8E */
    if (!ZF && SF == OF) goto L3B8E;
L3B83: /* L3B83 */
    /* 3B83  call    word ptr ds:[4D14h] */
    if ((c = asm_call(ASM_JMP(0x0085, rw(pDS, 0x4D14)), 0x3B87)) != 0) return c;
L3B87:
    /* 3B87  mov     ax,word ptr ds:[2D3Ah] */
    AX = rw(pDS, 0x2D3A);
L3B8A:
    /* 3B8A  call    word ptr ds:[4D16h] */
    if ((c = asm_call(ASM_JMP(0x0085, rw(pDS, 0x4D16)), 0x3B8E)) != 0) return c;
L3B8E: /* L3B8E */
    /* 3B8E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_3B8F  (+3B8F)
       _3B8F (shadowed_string_to_screen, FM Towns): the same box check, then the string drawn one
       pixel right and one down in the shadow colour (2D36), the bit buffer shifted one pixel back
       (_4150), and the string drawn again at (x, y) in the text colour (2D3A). _3BC8 is the
       unclipped entry. */
L3B8F: /* _seg003_0272_3B8F */
    /* 3B8F  mov     word ptr ds:[4A0Eh],ax */
    ww(pDS, 0x4A0E, AX);
L3B92:
    /* 3B92  mov     word ptr ds:[4A10h],bx */
    ww(pDS, 0x4A10, BX);
L3B96:
    /* 3B96  mov     ax,word ptr ds:[4A10h] */
    AX = rw(pDS, 0x4A10);
L3B99:
    /* 3B99  cmp     ax,word ptr ds:[3DFAh] */
    sub16(AX, rw(pDS, 0x3DFA), 0);
L3B9D:
    /* 3B9D  jl      L3B8E */
    if (SF != OF) goto L3B8E;
L3B9F:
    /* 3B9F  cmp     ax,word ptr ds:[3DF6h] */
    sub16(AX, rw(pDS, 0x3DF6), 0);
L3BA3:
    /* 3BA3  jg      L3B8E */
    if (!ZF && SF == OF) goto L3B8E;
L3BA5:
    /* 3BA5  sub     ax,word ptr ds:[4D0Eh] */
    AX = (uint16_t)(AX - rw(pDS, 0x4D0E));
L3BA9:
    /* 3BA9  inc     ax */
    AX = (uint16_t)(AX + 1);
L3BAA:
    /* 3BAA  mov     word ptr ds:[4A14h],ax */
    ww(pDS, 0x4A14, AX);
L3BAD:
    /* 3BAD  cmp     ax,word ptr ds:[3DFAh] */
    sub16(AX, rw(pDS, 0x3DFA), 0);
L3BB1:
    /* 3BB1  jl      L3B8E */
    if (SF != OF) goto L3B8E;
L3BB3:
    /* 3BB3  cmp     ax,word ptr ds:[3DF6h] */
    sub16(AX, rw(pDS, 0x3DF6), 0);
L3BB7:
    /* 3BB7  jg      L3B8E */
    if (!ZF && SF == OF) goto L3B8E;
L3BB9:
    /* 3BB9  mov     ax,word ptr ds:[4A0Eh] */
    AX = rw(pDS, 0x4A0E);
L3BBC:
    /* 3BBC  cmp     ax,word ptr ds:[3DF4h] */
    sub16(AX, rw(pDS, 0x3DF4), 0);
L3BC0:
    /* 3BC0  jl      L3B8E */
    if (SF != OF) goto L3B8E;
L3BC2:
    /* 3BC2  cmp     ax,word ptr ds:[3DF8h] */
    sub16(AX, rw(pDS, 0x3DF8), 0);
L3BC6:
    /* 3BC6  jg      L3B8E */
    if (!ZF && SF == OF) goto L3B8E;
L3BC8: /* _seg003_0272_3BC8 */
    /* 3BC8  inc     word ptr ds:[4A0Eh] */
    ww(pDS, 0x4A0E, (uint16_t)(rw(pDS, 0x4A0E) + 1));
L3BCC:
    /* 3BCC  dec     word ptr ds:[4A10h] */
    ww(pDS, 0x4A10, (uint16_t)(rw(pDS, 0x4A10) - 1));
L3BD0:
    /* 3BD0  call    word ptr ds:[4D14h] */
    if ((c = asm_call(ASM_JMP(0x0085, rw(pDS, 0x4D14)), 0x3BD4)) != 0) return c;
L3BD4:
    /* 3BD4  mov     ax,word ptr ds:[2D36h] */
    AX = rw(pDS, 0x2D36);
L3BD7:
    /* 3BD7  call    _seg003_0272_3C61 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x3C61), 0x3BDA)) != 0) return c;
L3BDA:
    /* 3BDA  call    _seg003_0272_4150 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x4150), 0x3BDD)) != 0) return c;
L3BDD:
    /* 3BDD  mov     ax,word ptr ds:[2D3Ah] */
    AX = rw(pDS, 0x2D3A);
L3BE0:
    /* 3BE0  jmp     short _seg003_0272_3C61 */
    goto L3C61;

    /* seg003_0272_3BE2  (+3BE2)
       _3BE2: build the 256-entry table at 4A26 from the 16 plane masks at 4A16: for each byte of a
       one-bit row, the map masks of its high and low four pixels. GRCORE's _43F5 runs it through the
       jump table at 5278. */
L3BE2: /* _seg003_0272_3BE2 */
    /* 3BE2  mov     cx,10h */
    CX = 0x10;
L3BE5:
    /* 3BE5  mov     si,4A16h */
    SI = 0x4A16;
L3BE8:
    /* 3BE8  mov     di,4A26h */
    DI = 0x4A26;
L3BEB: /* L3BEB */
    /* 3BEB  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3BEC:
    /* 3BEC  push    cx */
    push16(CX);
L3BED:
    /* 3BED  push    si */
    push16(SI);
L3BEE:
    /* 3BEE  mov     cx,10h */
    CX = 0x10;
L3BF1:
    /* 3BF1  mov     si,4A16h */
    SI = 0x4A16;
L3BF4: /* L3BF4 */
    /* 3BF4  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3BF5:
    /* 3BF5  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L3BF6:
    /* 3BF6  loop    L3BF4 */
    if (--CX) goto L3BF4;
L3BF8:
    /* 3BF8  pop     si */
    SI = pop16();
L3BF9:
    /* 3BF9  pop     cx */
    CX = pop16();
L3BFA:
    /* 3BFA  loop    L3BEB */
    if (--CX) goto L3BEB;
L3BFC:
    /* 3BFC  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_3BFD  (+3BFD)
       setup_font: after a font has been loaded into 4D08..4D12, build the glyph pointer table at
       4D18 for characters 0 to 127, from 1C54h in steps of (height << (bytes per row - 1)) + 4D08
       and copy the font's height and row width for string_width.

       After its ret are small entries that set a colour (4116) and a span writer (4112: 52CC, 52DB,
       52D8 or 52E7) and draw a rectangle from four words at SI (GRLIBF's _3416); nothing in the
       sources jumps to them by name. */
L3BFD: /* _seg003_0272_3BFD */
    /* 3BFD  mov     bx,word ptr ds:[4D0Eh] */
    BX = rw(pDS, 0x4D0E);
L3C01:
    /* 3C01  mov     cx,word ptr ds:[4D10h] */
    CX = rw(pDS, 0x4D10);
L3C05:
    /* 3C05  dec     cl */
    CL = dec8(CL);
L3C07:
    /* 3C07  shl     bx,cl */
    BX = (uint16_t)(BX << (CL & 31));
L3C09:
    /* 3C09  mov     cx,80h */
    CX = 0x80;
L3C0C:
    /* 3C0C  mov     ax,1C54h */
    AX = 0x1C54;
L3C0F:
    /* 3C0F  mov     di,4D18h */
    DI = 0x4D18;
L3C12:
    /* 3C12  add     bx,word ptr ds:[4D08h] */
    BX = (uint16_t)(BX + rw(pDS, 0x4D08));
L3C16: /* L3C16 */
    /* 3C16  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L3C17:
    /* 3C17  add     ax,bx */
    AX = add16(AX, BX, 0);
L3C19:
    /* 3C19  loop    L3C16 */
    if (--CX) goto L3C16;
L3C1B:
    /* 3C1B  mov     ax,word ptr ds:[4D0Eh] */
    AX = rw(pDS, 0x4D0E);
L3C1E:
    /* 3C1E  mov     word ptr ds:[2D3Ch],ax */
    ww(pDS, 0x2D3C, AX);
L3C21:
    /* 3C21  mov     ax,word ptr ds:[4D10h] */
    AX = rw(pDS, 0x4D10);
L3C24:
    /* 3C24  dec     ax */
    AX = dec16(AX);
L3C25:
    /* 3C25  mov     word ptr ds:[2D3Eh],ax */
    ww(pDS, 0x2D3E, AX);
L3C28:
    /* 3C28  mov     ax,word ptr ds:[4D12h] */
    AX = rw(pDS, 0x4D12);
L3C2B:
    /* 3C2B  mov     word ptr ds:[2D40h],ax */
    ww(pDS, 0x2D40, AX);
L3C2E:
    /* 3C2E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L3C2F:
    /* 3C2F  mov     bx,offset _seg003_0272_52CC */
    BX = 0x52CC;
L3C32: /* L3C32 */
    /* 3C32  mov     word ptr ds:[4116h],ax */
    ww(pDS, 0x4116, AX);
L3C35:
    /* 3C35  mov     word ptr ds:[4112h],bx */
    ww(pDS, 0x4112, BX);
L3C39:
    /* 3C39  mov     word ptr ds:[4114h],0FFFFh */
    ww(pDS, 0x4114, 0xFFFF);
L3C3F:
    /* 3C3F  jmp     _seg003_0272_3416 */
    return ASM_JMP(0x0085, 0x3416);
L3C42:
    /* 3C42  mov     bx,offset _seg003_0272_52DB */
    BX = 0x52DB;
L3C45:
    /* 3C45  jmp     L3C32 */
    goto L3C32;
L3C47:
    /* 3C47  mov     bx,offset _seg003_0272_52D8 */
    BX = 0x52D8;
L3C4A:
    /* 3C4A  jmp     L3C32 */
    goto L3C32;
L3C4C: /* L3C4C */
    /* 3C4C  mov     bx,offset _seg003_0272_52E7 */
    BX = 0x52E7;
L3C4F:
    /* 3C4F  jmp     L3C32 */
    goto L3C32;
L3C51:
    /* 3C51  push    word ptr [si+2] */
    push16(rw(pDS, SI + 0x2));
L3C54:
    /* 3C54  mov     ax,word ptr [si+6] */
    AX = rw(pDS, SI + 0x6);
L3C57:
    /* 3C57  mov     word ptr [si+2],ax */
    ww(pDS, SI + 0x2, AX);
L3C5A:
    /* 3C5A  call    L3C4C */
    if ((c = asm_call(ASM_JMP(0x0085, 0x3C4C), 0x3C5D)) != 0) return c;
L3C5D:
    /* 3C5D  pop     word ptr [si+2] */
    { uint16_t t_ = pop16(); ww(pDS, SI + 0x2, t_); }
L3C60:
    /* 3C60  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_3C61  (+3C61)
       _3C61: the single-colour blitter. In: AX = the colour. Works out the screen address of (4A0E,
       4A10) from Ytab, and writes the bit buffer row by row: for each byte, the two plane masks from
       4A26 to port 3C5h, each followed by a store of the colour. The code at the second entry (after
       the jmp) sets the colour from AH and draws a filled box behind the text first. L4126, near the
       end, is the other blitter. */
L3C61: /* _seg003_0272_3C61 */
    /* 3C61  mov     word ptr ds:[4A0Ch],ax */
    ww(pDS, 0x4A0C, AX);
L3C64:
    /* 3C64  jmp     short L3C84 */
    goto L3C84;
L3C66:
    /* 3C66  mov     byte ptr ds:[4111h],ah */
    wb(pDS, 0x4111, AH);
L3C6A:
    /* 3C6A  mov     ax,word ptr ds:[4D04h] */
    AX = rw(pDS, 0x4D04);
L3C6D:
    /* 3C6D  dec     ax */
    AX = (uint16_t)(AX - 1);
L3C6E:
    /* 3C6E  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L3C70:
    /* 3C70  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L3C72:
    /* 3C72  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L3C74:
    /* 3C74  add     ax,word ptr ds:[4A0Eh] */
    AX = (uint16_t)(AX + rw(pDS, 0x4A0E));
L3C78:
    /* 3C78  mov     word ptr ds:[4A12h],ax */
    ww(pDS, 0x4A12, AX);
L3C7B:
    /* 3C7B  mov     bx,52C9h */
    BX = 0x52C9;
L3C7E:
    /* 3C7E  mov     si,4A0Eh */
    SI = 0x4A0E;
L3C81:
    /* 3C81  call    L3C32 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x3C32), 0x3C84)) != 0) return c;
L3C84: /* L3C84 */
    /* 3C84  mov     di,word ptr ds:[4A0Eh] */
    DI = rw(pDS, 0x4A0E);
L3C88:
    /* 3C88  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L3C8A:
    /* 3C8A  shr     di,1 */
    DI = (uint16_t)(DI >> 1);
L3C8C:
    /* 3C8C  mov     bp,word ptr ds:[4A10h] */
    BP = rw(pDS, 0x4A10);
L3C90:
    /* 3C90  shl     bp,1 */
    BP = (uint16_t)(BP << 1);
L3C92:
    /* 3C92  add     di,word ptr [bp+36ACh] */
    DI = (uint16_t)(DI + rw(pSS, BP + 0x36AC));
L3C96:
    /* 3C96  mov     bx,word ptr ds:[4D04h] */
    BX = rw(pDS, 0x4D04);
L3C9A:
    /* 3C9A  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3C9C:
    /* 3C9C  mov     bp,word ptr ds:[36A8h] */
    BP = rw(pDS, 0x36A8);
L3CA0:
    /* 3CA0  sub     bp,bx */
    BP = sub16(BP, BX, 0);
L3CA2:
    /* 3CA2  mov     ax,word ptr [bx+4C8Eh] */
    AX = rw(pDS, BX + 0x4C8E);
L3CA6:
    /* 3CA6  mov     word ptr ds:[4CFAh],ax */
    ww(pDS, 0x4CFA, AX);
L3CA9:
    /* 3CA9  mov     ch,byte ptr ds:[4D0Eh] */
    CH = rb(pDS, 0x4D0E);
L3CAD:
    /* 3CAD  mov     si,word ptr ds:[4D06h] */
    SI = rw(pDS, 0x4D06);
L3CB1:
    /* 3CB1  push    es */
    push16(asm_es);
L3CB2:
    /* 3CB2  mov     es,word ptr ds:[3DFCh] */
    SET_ES(rw(pDS, 0x3DFC));
L3CB6:
    /* 3CB6  mov     cl,byte ptr ds:[4A0Ch] */
    CL = rb(pDS, 0x4A0C);
L3CBA:
    /* 3CBA  mov     dx,SC_DATA */
    DX = 0x3C5;
L3CBD: /* L3CBD */
    /* 3CBD  jmp     word ptr ds:[4CFAh] */
    return ASM_JMP(0x0085, rw(pDS, 0x4CFA));
L3CC1:
    /* 3CC1  even */
    ;
L3CC2:
    /* 3CC2  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3CC3:
    /* 3CC3  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3CC5:
    /* 3CC5  mov     bl,al */
    BL = AL;
L3CC7:
    /* 3CC7  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3CC9:
    /* 3CC9  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3CCD:
    /* 3CCD  out     dx,al */
    asm_out8(DX, AL);
L3CCE:
    /* 3CCE  mov     al,cl */
    AL = CL;
L3CD0:
    /* 3CD0  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3CD1:
    /* 3CD1  mov     al,ah */
    AL = AH;
L3CD3:
    /* 3CD3  out     dx,al */
    asm_out8(DX, AL);
L3CD4:
    /* 3CD4  mov     al,cl */
    AL = CL;
L3CD6:
    /* 3CD6  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3CD7:
    /* 3CD7  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3CD8:
    /* 3CD8  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3CDA:
    /* 3CDA  mov     bl,al */
    BL = AL;
L3CDC:
    /* 3CDC  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3CDE:
    /* 3CDE  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3CE2:
    /* 3CE2  out     dx,al */
    asm_out8(DX, AL);
L3CE3:
    /* 3CE3  mov     al,cl */
    AL = CL;
L3CE5:
    /* 3CE5  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3CE6:
    /* 3CE6  mov     al,ah */
    AL = AH;
L3CE8:
    /* 3CE8  out     dx,al */
    asm_out8(DX, AL);
L3CE9:
    /* 3CE9  mov     al,cl */
    AL = CL;
L3CEB:
    /* 3CEB  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3CEC:
    /* 3CEC  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3CED:
    /* 3CED  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3CEF:
    /* 3CEF  mov     bl,al */
    BL = AL;
L3CF1:
    /* 3CF1  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3CF3:
    /* 3CF3  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3CF7:
    /* 3CF7  out     dx,al */
    asm_out8(DX, AL);
L3CF8:
    /* 3CF8  mov     al,cl */
    AL = CL;
L3CFA:
    /* 3CFA  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3CFB:
    /* 3CFB  mov     al,ah */
    AL = AH;
L3CFD:
    /* 3CFD  out     dx,al */
    asm_out8(DX, AL);
L3CFE:
    /* 3CFE  mov     al,cl */
    AL = CL;
L3D00:
    /* 3D00  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3D01:
    /* 3D01  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3D02:
    /* 3D02  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3D04:
    /* 3D04  mov     bl,al */
    BL = AL;
L3D06:
    /* 3D06  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3D08:
    /* 3D08  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3D0C:
    /* 3D0C  out     dx,al */
    asm_out8(DX, AL);
L3D0D:
    /* 3D0D  mov     al,cl */
    AL = CL;
L3D0F:
    /* 3D0F  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3D10:
    /* 3D10  mov     al,ah */
    AL = AH;
L3D12:
    /* 3D12  out     dx,al */
    asm_out8(DX, AL);
L3D13:
    /* 3D13  mov     al,cl */
    AL = CL;
L3D15:
    /* 3D15  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3D16:
    /* 3D16  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3D17:
    /* 3D17  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3D19:
    /* 3D19  mov     bl,al */
    BL = AL;
L3D1B:
    /* 3D1B  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3D1D:
    /* 3D1D  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3D21:
    /* 3D21  out     dx,al */
    asm_out8(DX, AL);
L3D22:
    /* 3D22  mov     al,cl */
    AL = CL;
L3D24:
    /* 3D24  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3D25:
    /* 3D25  mov     al,ah */
    AL = AH;
L3D27:
    /* 3D27  out     dx,al */
    asm_out8(DX, AL);
L3D28:
    /* 3D28  mov     al,cl */
    AL = CL;
L3D2A:
    /* 3D2A  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3D2B:
    /* 3D2B  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3D2C:
    /* 3D2C  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3D2E:
    /* 3D2E  mov     bl,al */
    BL = AL;
L3D30:
    /* 3D30  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3D32:
    /* 3D32  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3D36:
    /* 3D36  out     dx,al */
    asm_out8(DX, AL);
L3D37:
    /* 3D37  mov     al,cl */
    AL = CL;
L3D39:
    /* 3D39  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3D3A:
    /* 3D3A  mov     al,ah */
    AL = AH;
L3D3C:
    /* 3D3C  out     dx,al */
    asm_out8(DX, AL);
L3D3D:
    /* 3D3D  mov     al,cl */
    AL = CL;
L3D3F:
    /* 3D3F  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3D40:
    /* 3D40  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3D41:
    /* 3D41  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3D43:
    /* 3D43  mov     bl,al */
    BL = AL;
L3D45:
    /* 3D45  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3D47:
    /* 3D47  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3D4B:
    /* 3D4B  out     dx,al */
    asm_out8(DX, AL);
L3D4C:
    /* 3D4C  mov     al,cl */
    AL = CL;
L3D4E:
    /* 3D4E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3D4F:
    /* 3D4F  mov     al,ah */
    AL = AH;
L3D51:
    /* 3D51  out     dx,al */
    asm_out8(DX, AL);
L3D52:
    /* 3D52  mov     al,cl */
    AL = CL;
L3D54:
    /* 3D54  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3D55:
    /* 3D55  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3D56:
    /* 3D56  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3D58:
    /* 3D58  mov     bl,al */
    BL = AL;
L3D5A:
    /* 3D5A  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3D5C:
    /* 3D5C  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3D60:
    /* 3D60  out     dx,al */
    asm_out8(DX, AL);
L3D61:
    /* 3D61  mov     al,cl */
    AL = CL;
L3D63:
    /* 3D63  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3D64:
    /* 3D64  mov     al,ah */
    AL = AH;
L3D66:
    /* 3D66  out     dx,al */
    asm_out8(DX, AL);
L3D67:
    /* 3D67  mov     al,cl */
    AL = CL;
L3D69:
    /* 3D69  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3D6A:
    /* 3D6A  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3D6B:
    /* 3D6B  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3D6D:
    /* 3D6D  mov     bl,al */
    BL = AL;
L3D6F:
    /* 3D6F  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3D71:
    /* 3D71  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3D75:
    /* 3D75  out     dx,al */
    asm_out8(DX, AL);
L3D76:
    /* 3D76  mov     al,cl */
    AL = CL;
L3D78:
    /* 3D78  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3D79:
    /* 3D79  mov     al,ah */
    AL = AH;
L3D7B:
    /* 3D7B  out     dx,al */
    asm_out8(DX, AL);
L3D7C:
    /* 3D7C  mov     al,cl */
    AL = CL;
L3D7E:
    /* 3D7E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3D7F:
    /* 3D7F  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3D80:
    /* 3D80  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3D82:
    /* 3D82  mov     bl,al */
    BL = AL;
L3D84:
    /* 3D84  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3D86:
    /* 3D86  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3D8A:
    /* 3D8A  out     dx,al */
    asm_out8(DX, AL);
L3D8B:
    /* 3D8B  mov     al,cl */
    AL = CL;
L3D8D:
    /* 3D8D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3D8E:
    /* 3D8E  mov     al,ah */
    AL = AH;
L3D90:
    /* 3D90  out     dx,al */
    asm_out8(DX, AL);
L3D91:
    /* 3D91  mov     al,cl */
    AL = CL;
L3D93:
    /* 3D93  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3D94:
    /* 3D94  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3D95:
    /* 3D95  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3D97:
    /* 3D97  mov     bl,al */
    BL = AL;
L3D99:
    /* 3D99  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3D9B:
    /* 3D9B  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3D9F:
    /* 3D9F  out     dx,al */
    asm_out8(DX, AL);
L3DA0:
    /* 3DA0  mov     al,cl */
    AL = CL;
L3DA2:
    /* 3DA2  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3DA3:
    /* 3DA3  mov     al,ah */
    AL = AH;
L3DA5:
    /* 3DA5  out     dx,al */
    asm_out8(DX, AL);
L3DA6:
    /* 3DA6  mov     al,cl */
    AL = CL;
L3DA8:
    /* 3DA8  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3DA9:
    /* 3DA9  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3DAA:
    /* 3DAA  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3DAC:
    /* 3DAC  mov     bl,al */
    BL = AL;
L3DAE:
    /* 3DAE  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3DB0:
    /* 3DB0  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3DB4:
    /* 3DB4  out     dx,al */
    asm_out8(DX, AL);
L3DB5:
    /* 3DB5  mov     al,cl */
    AL = CL;
L3DB7:
    /* 3DB7  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3DB8:
    /* 3DB8  mov     al,ah */
    AL = AH;
L3DBA:
    /* 3DBA  out     dx,al */
    asm_out8(DX, AL);
L3DBB:
    /* 3DBB  mov     al,cl */
    AL = CL;
L3DBD:
    /* 3DBD  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3DBE:
    /* 3DBE  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3DBF:
    /* 3DBF  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3DC1:
    /* 3DC1  mov     bl,al */
    BL = AL;
L3DC3:
    /* 3DC3  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3DC5:
    /* 3DC5  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3DC9:
    /* 3DC9  out     dx,al */
    asm_out8(DX, AL);
L3DCA:
    /* 3DCA  mov     al,cl */
    AL = CL;
L3DCC:
    /* 3DCC  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3DCD:
    /* 3DCD  mov     al,ah */
    AL = AH;
L3DCF:
    /* 3DCF  out     dx,al */
    asm_out8(DX, AL);
L3DD0:
    /* 3DD0  mov     al,cl */
    AL = CL;
L3DD2:
    /* 3DD2  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3DD3:
    /* 3DD3  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3DD4:
    /* 3DD4  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3DD6:
    /* 3DD6  mov     bl,al */
    BL = AL;
L3DD8:
    /* 3DD8  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3DDA:
    /* 3DDA  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3DDE:
    /* 3DDE  out     dx,al */
    asm_out8(DX, AL);
L3DDF:
    /* 3DDF  mov     al,cl */
    AL = CL;
L3DE1:
    /* 3DE1  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3DE2:
    /* 3DE2  mov     al,ah */
    AL = AH;
L3DE4:
    /* 3DE4  out     dx,al */
    asm_out8(DX, AL);
L3DE5:
    /* 3DE5  mov     al,cl */
    AL = CL;
L3DE7:
    /* 3DE7  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3DE8:
    /* 3DE8  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3DE9:
    /* 3DE9  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3DEB:
    /* 3DEB  mov     bl,al */
    BL = AL;
L3DED:
    /* 3DED  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3DEF:
    /* 3DEF  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3DF3:
    /* 3DF3  out     dx,al */
    asm_out8(DX, AL);
L3DF4:
    /* 3DF4  mov     al,cl */
    AL = CL;
L3DF6:
    /* 3DF6  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3DF7:
    /* 3DF7  mov     al,ah */
    AL = AH;
L3DF9:
    /* 3DF9  out     dx,al */
    asm_out8(DX, AL);
L3DFA:
    /* 3DFA  mov     al,cl */
    AL = CL;
L3DFC:
    /* 3DFC  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3DFD:
    /* 3DFD  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3DFE:
    /* 3DFE  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3E00:
    /* 3E00  mov     bl,al */
    BL = AL;
L3E02:
    /* 3E02  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3E04:
    /* 3E04  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3E08:
    /* 3E08  out     dx,al */
    asm_out8(DX, AL);
L3E09:
    /* 3E09  mov     al,cl */
    AL = CL;
L3E0B:
    /* 3E0B  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3E0C:
    /* 3E0C  mov     al,ah */
    AL = AH;
L3E0E:
    /* 3E0E  out     dx,al */
    asm_out8(DX, AL);
L3E0F:
    /* 3E0F  mov     al,cl */
    AL = CL;
L3E11:
    /* 3E11  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3E12:
    /* 3E12  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3E13:
    /* 3E13  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3E15:
    /* 3E15  mov     bl,al */
    BL = AL;
L3E17:
    /* 3E17  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3E19:
    /* 3E19  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3E1D:
    /* 3E1D  out     dx,al */
    asm_out8(DX, AL);
L3E1E:
    /* 3E1E  mov     al,cl */
    AL = CL;
L3E20:
    /* 3E20  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3E21:
    /* 3E21  mov     al,ah */
    AL = AH;
L3E23:
    /* 3E23  out     dx,al */
    asm_out8(DX, AL);
L3E24:
    /* 3E24  mov     al,cl */
    AL = CL;
L3E26:
    /* 3E26  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3E27:
    /* 3E27  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3E28:
    /* 3E28  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3E2A:
    /* 3E2A  mov     bl,al */
    BL = AL;
L3E2C:
    /* 3E2C  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3E2E:
    /* 3E2E  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3E32:
    /* 3E32  out     dx,al */
    asm_out8(DX, AL);
L3E33:
    /* 3E33  mov     al,cl */
    AL = CL;
L3E35:
    /* 3E35  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3E36:
    /* 3E36  mov     al,ah */
    AL = AH;
L3E38:
    /* 3E38  out     dx,al */
    asm_out8(DX, AL);
L3E39:
    /* 3E39  mov     al,cl */
    AL = CL;
L3E3B:
    /* 3E3B  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3E3C:
    /* 3E3C  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3E3D:
    /* 3E3D  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3E3F:
    /* 3E3F  mov     bl,al */
    BL = AL;
L3E41:
    /* 3E41  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3E43:
    /* 3E43  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3E47:
    /* 3E47  out     dx,al */
    asm_out8(DX, AL);
L3E48:
    /* 3E48  mov     al,cl */
    AL = CL;
L3E4A:
    /* 3E4A  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3E4B:
    /* 3E4B  mov     al,ah */
    AL = AH;
L3E4D:
    /* 3E4D  out     dx,al */
    asm_out8(DX, AL);
L3E4E:
    /* 3E4E  mov     al,cl */
    AL = CL;
L3E50:
    /* 3E50  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3E51:
    /* 3E51  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3E52:
    /* 3E52  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3E54:
    /* 3E54  mov     bl,al */
    BL = AL;
L3E56:
    /* 3E56  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3E58:
    /* 3E58  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3E5C:
    /* 3E5C  out     dx,al */
    asm_out8(DX, AL);
L3E5D:
    /* 3E5D  mov     al,cl */
    AL = CL;
L3E5F:
    /* 3E5F  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3E60:
    /* 3E60  mov     al,ah */
    AL = AH;
L3E62:
    /* 3E62  out     dx,al */
    asm_out8(DX, AL);
L3E63:
    /* 3E63  mov     al,cl */
    AL = CL;
L3E65:
    /* 3E65  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3E66:
    /* 3E66  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3E67:
    /* 3E67  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3E69:
    /* 3E69  mov     bl,al */
    BL = AL;
L3E6B:
    /* 3E6B  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3E6D:
    /* 3E6D  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3E71:
    /* 3E71  out     dx,al */
    asm_out8(DX, AL);
L3E72:
    /* 3E72  mov     al,cl */
    AL = CL;
L3E74:
    /* 3E74  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3E75:
    /* 3E75  mov     al,ah */
    AL = AH;
L3E77:
    /* 3E77  out     dx,al */
    asm_out8(DX, AL);
L3E78:
    /* 3E78  mov     al,cl */
    AL = CL;
L3E7A:
    /* 3E7A  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3E7B:
    /* 3E7B  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3E7C:
    /* 3E7C  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3E7E:
    /* 3E7E  mov     bl,al */
    BL = AL;
L3E80:
    /* 3E80  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3E82:
    /* 3E82  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3E86:
    /* 3E86  out     dx,al */
    asm_out8(DX, AL);
L3E87:
    /* 3E87  mov     al,cl */
    AL = CL;
L3E89:
    /* 3E89  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3E8A:
    /* 3E8A  mov     al,ah */
    AL = AH;
L3E8C:
    /* 3E8C  out     dx,al */
    asm_out8(DX, AL);
L3E8D:
    /* 3E8D  mov     al,cl */
    AL = CL;
L3E8F:
    /* 3E8F  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3E90:
    /* 3E90  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3E91:
    /* 3E91  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3E93:
    /* 3E93  mov     bl,al */
    BL = AL;
L3E95:
    /* 3E95  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3E97:
    /* 3E97  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3E9B:
    /* 3E9B  out     dx,al */
    asm_out8(DX, AL);
L3E9C:
    /* 3E9C  mov     al,cl */
    AL = CL;
L3E9E:
    /* 3E9E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3E9F:
    /* 3E9F  mov     al,ah */
    AL = AH;
L3EA1:
    /* 3EA1  out     dx,al */
    asm_out8(DX, AL);
L3EA2:
    /* 3EA2  mov     al,cl */
    AL = CL;
L3EA4:
    /* 3EA4  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3EA5:
    /* 3EA5  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3EA6:
    /* 3EA6  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3EA8:
    /* 3EA8  mov     bl,al */
    BL = AL;
L3EAA:
    /* 3EAA  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3EAC:
    /* 3EAC  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3EB0:
    /* 3EB0  out     dx,al */
    asm_out8(DX, AL);
L3EB1:
    /* 3EB1  mov     al,cl */
    AL = CL;
L3EB3:
    /* 3EB3  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3EB4:
    /* 3EB4  mov     al,ah */
    AL = AH;
L3EB6:
    /* 3EB6  out     dx,al */
    asm_out8(DX, AL);
L3EB7:
    /* 3EB7  mov     al,cl */
    AL = CL;
L3EB9:
    /* 3EB9  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3EBA:
    /* 3EBA  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3EBB:
    /* 3EBB  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3EBD:
    /* 3EBD  mov     bl,al */
    BL = AL;
L3EBF:
    /* 3EBF  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3EC1:
    /* 3EC1  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3EC5:
    /* 3EC5  out     dx,al */
    asm_out8(DX, AL);
L3EC6:
    /* 3EC6  mov     al,cl */
    AL = CL;
L3EC8:
    /* 3EC8  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3EC9:
    /* 3EC9  mov     al,ah */
    AL = AH;
L3ECB:
    /* 3ECB  out     dx,al */
    asm_out8(DX, AL);
L3ECC:
    /* 3ECC  mov     al,cl */
    AL = CL;
L3ECE:
    /* 3ECE  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3ECF:
    /* 3ECF  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3ED0:
    /* 3ED0  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3ED2:
    /* 3ED2  mov     bl,al */
    BL = AL;
L3ED4:
    /* 3ED4  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3ED6:
    /* 3ED6  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3EDA:
    /* 3EDA  out     dx,al */
    asm_out8(DX, AL);
L3EDB:
    /* 3EDB  mov     al,cl */
    AL = CL;
L3EDD:
    /* 3EDD  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3EDE:
    /* 3EDE  mov     al,ah */
    AL = AH;
L3EE0:
    /* 3EE0  out     dx,al */
    asm_out8(DX, AL);
L3EE1:
    /* 3EE1  mov     al,cl */
    AL = CL;
L3EE3:
    /* 3EE3  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3EE4:
    /* 3EE4  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3EE5:
    /* 3EE5  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3EE7:
    /* 3EE7  mov     bl,al */
    BL = AL;
L3EE9:
    /* 3EE9  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3EEB:
    /* 3EEB  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3EEF:
    /* 3EEF  out     dx,al */
    asm_out8(DX, AL);
L3EF0:
    /* 3EF0  mov     al,cl */
    AL = CL;
L3EF2:
    /* 3EF2  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3EF3:
    /* 3EF3  mov     al,ah */
    AL = AH;
L3EF5:
    /* 3EF5  out     dx,al */
    asm_out8(DX, AL);
L3EF6:
    /* 3EF6  mov     al,cl */
    AL = CL;
L3EF8:
    /* 3EF8  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3EF9:
    /* 3EF9  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3EFA:
    /* 3EFA  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3EFC:
    /* 3EFC  mov     bl,al */
    BL = AL;
L3EFE:
    /* 3EFE  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3F00:
    /* 3F00  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3F04:
    /* 3F04  out     dx,al */
    asm_out8(DX, AL);
L3F05:
    /* 3F05  mov     al,cl */
    AL = CL;
L3F07:
    /* 3F07  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3F08:
    /* 3F08  mov     al,ah */
    AL = AH;
L3F0A:
    /* 3F0A  out     dx,al */
    asm_out8(DX, AL);
L3F0B:
    /* 3F0B  mov     al,cl */
    AL = CL;
L3F0D:
    /* 3F0D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3F0E:
    /* 3F0E  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3F0F:
    /* 3F0F  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3F11:
    /* 3F11  mov     bl,al */
    BL = AL;
L3F13:
    /* 3F13  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3F15:
    /* 3F15  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3F19:
    /* 3F19  out     dx,al */
    asm_out8(DX, AL);
L3F1A:
    /* 3F1A  mov     al,cl */
    AL = CL;
L3F1C:
    /* 3F1C  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3F1D:
    /* 3F1D  mov     al,ah */
    AL = AH;
L3F1F:
    /* 3F1F  out     dx,al */
    asm_out8(DX, AL);
L3F20:
    /* 3F20  mov     al,cl */
    AL = CL;
L3F22:
    /* 3F22  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3F23:
    /* 3F23  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3F24:
    /* 3F24  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3F26:
    /* 3F26  mov     bl,al */
    BL = AL;
L3F28:
    /* 3F28  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3F2A:
    /* 3F2A  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3F2E:
    /* 3F2E  out     dx,al */
    asm_out8(DX, AL);
L3F2F:
    /* 3F2F  mov     al,cl */
    AL = CL;
L3F31:
    /* 3F31  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3F32:
    /* 3F32  mov     al,ah */
    AL = AH;
L3F34:
    /* 3F34  out     dx,al */
    asm_out8(DX, AL);
L3F35:
    /* 3F35  mov     al,cl */
    AL = CL;
L3F37:
    /* 3F37  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3F38:
    /* 3F38  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3F39:
    /* 3F39  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3F3B:
    /* 3F3B  mov     bl,al */
    BL = AL;
L3F3D:
    /* 3F3D  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3F3F:
    /* 3F3F  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3F43:
    /* 3F43  out     dx,al */
    asm_out8(DX, AL);
L3F44:
    /* 3F44  mov     al,cl */
    AL = CL;
L3F46:
    /* 3F46  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3F47:
    /* 3F47  mov     al,ah */
    AL = AH;
L3F49:
    /* 3F49  out     dx,al */
    asm_out8(DX, AL);
L3F4A:
    /* 3F4A  mov     al,cl */
    AL = CL;
L3F4C:
    /* 3F4C  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3F4D:
    /* 3F4D  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3F4E:
    /* 3F4E  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3F50:
    /* 3F50  mov     bl,al */
    BL = AL;
L3F52:
    /* 3F52  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3F54:
    /* 3F54  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3F58:
    /* 3F58  out     dx,al */
    asm_out8(DX, AL);
L3F59:
    /* 3F59  mov     al,cl */
    AL = CL;
L3F5B:
    /* 3F5B  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3F5C:
    /* 3F5C  mov     al,ah */
    AL = AH;
L3F5E:
    /* 3F5E  out     dx,al */
    asm_out8(DX, AL);
L3F5F:
    /* 3F5F  mov     al,cl */
    AL = CL;
L3F61:
    /* 3F61  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3F62:
    /* 3F62  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3F63:
    /* 3F63  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3F65:
    /* 3F65  mov     bl,al */
    BL = AL;
L3F67:
    /* 3F67  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3F69:
    /* 3F69  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3F6D:
    /* 3F6D  out     dx,al */
    asm_out8(DX, AL);
L3F6E:
    /* 3F6E  mov     al,cl */
    AL = CL;
L3F70:
    /* 3F70  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3F71:
    /* 3F71  mov     al,ah */
    AL = AH;
L3F73:
    /* 3F73  out     dx,al */
    asm_out8(DX, AL);
L3F74:
    /* 3F74  mov     al,cl */
    AL = CL;
L3F76:
    /* 3F76  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3F77:
    /* 3F77  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3F78:
    /* 3F78  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3F7A:
    /* 3F7A  mov     bl,al */
    BL = AL;
L3F7C:
    /* 3F7C  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3F7E:
    /* 3F7E  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3F82:
    /* 3F82  out     dx,al */
    asm_out8(DX, AL);
L3F83:
    /* 3F83  mov     al,cl */
    AL = CL;
L3F85:
    /* 3F85  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3F86:
    /* 3F86  mov     al,ah */
    AL = AH;
L3F88:
    /* 3F88  out     dx,al */
    asm_out8(DX, AL);
L3F89:
    /* 3F89  mov     al,cl */
    AL = CL;
L3F8B:
    /* 3F8B  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3F8C:
    /* 3F8C  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3F8D:
    /* 3F8D  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3F8F:
    /* 3F8F  mov     bl,al */
    BL = AL;
L3F91:
    /* 3F91  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3F93:
    /* 3F93  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3F97:
    /* 3F97  out     dx,al */
    asm_out8(DX, AL);
L3F98:
    /* 3F98  mov     al,cl */
    AL = CL;
L3F9A:
    /* 3F9A  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3F9B:
    /* 3F9B  mov     al,ah */
    AL = AH;
L3F9D:
    /* 3F9D  out     dx,al */
    asm_out8(DX, AL);
L3F9E:
    /* 3F9E  mov     al,cl */
    AL = CL;
L3FA0:
    /* 3FA0  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3FA1:
    /* 3FA1  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3FA2:
    /* 3FA2  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3FA4:
    /* 3FA4  mov     bl,al */
    BL = AL;
L3FA6:
    /* 3FA6  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3FA8:
    /* 3FA8  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3FAC:
    /* 3FAC  out     dx,al */
    asm_out8(DX, AL);
L3FAD:
    /* 3FAD  mov     al,cl */
    AL = CL;
L3FAF:
    /* 3FAF  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3FB0:
    /* 3FB0  mov     al,ah */
    AL = AH;
L3FB2:
    /* 3FB2  out     dx,al */
    asm_out8(DX, AL);
L3FB3:
    /* 3FB3  mov     al,cl */
    AL = CL;
L3FB5:
    /* 3FB5  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3FB6:
    /* 3FB6  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3FB7:
    /* 3FB7  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3FB9:
    /* 3FB9  mov     bl,al */
    BL = AL;
L3FBB:
    /* 3FBB  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3FBD:
    /* 3FBD  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3FC1:
    /* 3FC1  out     dx,al */
    asm_out8(DX, AL);
L3FC2:
    /* 3FC2  mov     al,cl */
    AL = CL;
L3FC4:
    /* 3FC4  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3FC5:
    /* 3FC5  mov     al,ah */
    AL = AH;
L3FC7:
    /* 3FC7  out     dx,al */
    asm_out8(DX, AL);
L3FC8:
    /* 3FC8  mov     al,cl */
    AL = CL;
L3FCA:
    /* 3FCA  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3FCB:
    /* 3FCB  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3FCC:
    /* 3FCC  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3FCE:
    /* 3FCE  mov     bl,al */
    BL = AL;
L3FD0:
    /* 3FD0  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3FD2:
    /* 3FD2  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3FD6:
    /* 3FD6  out     dx,al */
    asm_out8(DX, AL);
L3FD7:
    /* 3FD7  mov     al,cl */
    AL = CL;
L3FD9:
    /* 3FD9  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3FDA:
    /* 3FDA  mov     al,ah */
    AL = AH;
L3FDC:
    /* 3FDC  out     dx,al */
    asm_out8(DX, AL);
L3FDD:
    /* 3FDD  mov     al,cl */
    AL = CL;
L3FDF:
    /* 3FDF  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3FE0:
    /* 3FE0  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3FE1:
    /* 3FE1  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3FE3:
    /* 3FE3  mov     bl,al */
    BL = AL;
L3FE5:
    /* 3FE5  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3FE7:
    /* 3FE7  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L3FEB:
    /* 3FEB  out     dx,al */
    asm_out8(DX, AL);
L3FEC:
    /* 3FEC  mov     al,cl */
    AL = CL;
L3FEE:
    /* 3FEE  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3FEF:
    /* 3FEF  mov     al,ah */
    AL = AH;
L3FF1:
    /* 3FF1  out     dx,al */
    asm_out8(DX, AL);
L3FF2:
    /* 3FF2  mov     al,cl */
    AL = CL;
L3FF4:
    /* 3FF4  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L3FF5:
    /* 3FF5  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L3FF6:
    /* 3FF6  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L3FF8:
    /* 3FF8  mov     bl,al */
    BL = AL;
L3FFA:
    /* 3FFA  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L3FFC:
    /* 3FFC  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L4000:
    /* 4000  out     dx,al */
    asm_out8(DX, AL);
L4001:
    /* 4001  mov     al,cl */
    AL = CL;
L4003:
    /* 4003  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4004:
    /* 4004  mov     al,ah */
    AL = AH;
L4006:
    /* 4006  out     dx,al */
    asm_out8(DX, AL);
L4007:
    /* 4007  mov     al,cl */
    AL = CL;
L4009:
    /* 4009  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L400A:
    /* 400A  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L400B:
    /* 400B  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L400D:
    /* 400D  mov     bl,al */
    BL = AL;
L400F:
    /* 400F  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4011:
    /* 4011  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L4015:
    /* 4015  out     dx,al */
    asm_out8(DX, AL);
L4016:
    /* 4016  mov     al,cl */
    AL = CL;
L4018:
    /* 4018  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4019:
    /* 4019  mov     al,ah */
    AL = AH;
L401B:
    /* 401B  out     dx,al */
    asm_out8(DX, AL);
L401C:
    /* 401C  mov     al,cl */
    AL = CL;
L401E:
    /* 401E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L401F:
    /* 401F  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4020:
    /* 4020  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4022:
    /* 4022  mov     bl,al */
    BL = AL;
L4024:
    /* 4024  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4026:
    /* 4026  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L402A:
    /* 402A  out     dx,al */
    asm_out8(DX, AL);
L402B:
    /* 402B  mov     al,cl */
    AL = CL;
L402D:
    /* 402D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L402E:
    /* 402E  mov     al,ah */
    AL = AH;
L4030:
    /* 4030  out     dx,al */
    asm_out8(DX, AL);
L4031:
    /* 4031  mov     al,cl */
    AL = CL;
L4033:
    /* 4033  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4034:
    /* 4034  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4035:
    /* 4035  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4037:
    /* 4037  mov     bl,al */
    BL = AL;
L4039:
    /* 4039  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L403B:
    /* 403B  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L403F:
    /* 403F  out     dx,al */
    asm_out8(DX, AL);
L4040:
    /* 4040  mov     al,cl */
    AL = CL;
L4042:
    /* 4042  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4043:
    /* 4043  mov     al,ah */
    AL = AH;
L4045:
    /* 4045  out     dx,al */
    asm_out8(DX, AL);
L4046:
    /* 4046  mov     al,cl */
    AL = CL;
L4048:
    /* 4048  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4049:
    /* 4049  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L404A:
    /* 404A  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L404C:
    /* 404C  mov     bl,al */
    BL = AL;
L404E:
    /* 404E  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4050:
    /* 4050  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L4054:
    /* 4054  out     dx,al */
    asm_out8(DX, AL);
L4055:
    /* 4055  mov     al,cl */
    AL = CL;
L4057:
    /* 4057  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4058:
    /* 4058  mov     al,ah */
    AL = AH;
L405A:
    /* 405A  out     dx,al */
    asm_out8(DX, AL);
L405B:
    /* 405B  mov     al,cl */
    AL = CL;
L405D:
    /* 405D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L405E:
    /* 405E  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L405F:
    /* 405F  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4061:
    /* 4061  mov     bl,al */
    BL = AL;
L4063:
    /* 4063  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4065:
    /* 4065  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L4069:
    /* 4069  out     dx,al */
    asm_out8(DX, AL);
L406A:
    /* 406A  mov     al,cl */
    AL = CL;
L406C:
    /* 406C  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L406D:
    /* 406D  mov     al,ah */
    AL = AH;
L406F:
    /* 406F  out     dx,al */
    asm_out8(DX, AL);
L4070:
    /* 4070  mov     al,cl */
    AL = CL;
L4072:
    /* 4072  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4073:
    /* 4073  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4074:
    /* 4074  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4076:
    /* 4076  mov     bl,al */
    BL = AL;
L4078:
    /* 4078  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L407A:
    /* 407A  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L407E:
    /* 407E  out     dx,al */
    asm_out8(DX, AL);
L407F:
    /* 407F  mov     al,cl */
    AL = CL;
L4081:
    /* 4081  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4082:
    /* 4082  mov     al,ah */
    AL = AH;
L4084:
    /* 4084  out     dx,al */
    asm_out8(DX, AL);
L4085:
    /* 4085  mov     al,cl */
    AL = CL;
L4087:
    /* 4087  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4088:
    /* 4088  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4089:
    /* 4089  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L408B:
    /* 408B  mov     bl,al */
    BL = AL;
L408D:
    /* 408D  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L408F:
    /* 408F  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L4093:
    /* 4093  out     dx,al */
    asm_out8(DX, AL);
L4094:
    /* 4094  mov     al,cl */
    AL = CL;
L4096:
    /* 4096  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4097:
    /* 4097  mov     al,ah */
    AL = AH;
L4099:
    /* 4099  out     dx,al */
    asm_out8(DX, AL);
L409A:
    /* 409A  mov     al,cl */
    AL = CL;
L409C:
    /* 409C  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L409D:
    /* 409D  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L409E:
    /* 409E  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L40A0:
    /* 40A0  mov     bl,al */
    BL = AL;
L40A2:
    /* 40A2  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L40A4:
    /* 40A4  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L40A8:
    /* 40A8  out     dx,al */
    asm_out8(DX, AL);
L40A9:
    /* 40A9  mov     al,cl */
    AL = CL;
L40AB:
    /* 40AB  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L40AC:
    /* 40AC  mov     al,ah */
    AL = AH;
L40AE:
    /* 40AE  out     dx,al */
    asm_out8(DX, AL);
L40AF:
    /* 40AF  mov     al,cl */
    AL = CL;
L40B1:
    /* 40B1  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L40B2:
    /* 40B2  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L40B3:
    /* 40B3  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L40B5:
    /* 40B5  mov     bl,al */
    BL = AL;
L40B7:
    /* 40B7  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L40B9:
    /* 40B9  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L40BD:
    /* 40BD  out     dx,al */
    asm_out8(DX, AL);
L40BE:
    /* 40BE  mov     al,cl */
    AL = CL;
L40C0:
    /* 40C0  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L40C1:
    /* 40C1  mov     al,ah */
    AL = AH;
L40C3:
    /* 40C3  out     dx,al */
    asm_out8(DX, AL);
L40C4:
    /* 40C4  mov     al,cl */
    AL = CL;
L40C6:
    /* 40C6  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L40C7:
    /* 40C7  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L40C8:
    /* 40C8  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L40CA:
    /* 40CA  mov     bl,al */
    BL = AL;
L40CC:
    /* 40CC  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L40CE:
    /* 40CE  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L40D2:
    /* 40D2  out     dx,al */
    asm_out8(DX, AL);
L40D3:
    /* 40D3  mov     al,cl */
    AL = CL;
L40D5:
    /* 40D5  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L40D6:
    /* 40D6  mov     al,ah */
    AL = AH;
L40D8:
    /* 40D8  out     dx,al */
    asm_out8(DX, AL);
L40D9:
    /* 40D9  mov     al,cl */
    AL = CL;
L40DB:
    /* 40DB  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L40DC:
    /* 40DC  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L40DD:
    /* 40DD  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L40DF:
    /* 40DF  mov     bl,al */
    BL = AL;
L40E1:
    /* 40E1  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L40E3:
    /* 40E3  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L40E7:
    /* 40E7  out     dx,al */
    asm_out8(DX, AL);
L40E8:
    /* 40E8  mov     al,cl */
    AL = CL;
L40EA:
    /* 40EA  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L40EB:
    /* 40EB  mov     al,ah */
    AL = AH;
L40ED:
    /* 40ED  out     dx,al */
    asm_out8(DX, AL);
L40EE:
    /* 40EE  mov     al,cl */
    AL = CL;
L40F0:
    /* 40F0  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L40F1:
    /* 40F1  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L40F2:
    /* 40F2  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L40F4:
    /* 40F4  mov     bl,al */
    BL = AL;
L40F6:
    /* 40F6  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L40F8:
    /* 40F8  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L40FC:
    /* 40FC  out     dx,al */
    asm_out8(DX, AL);
L40FD:
    /* 40FD  mov     al,cl */
    AL = CL;
L40FF:
    /* 40FF  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4100:
    /* 4100  mov     al,ah */
    AL = AH;
L4102:
    /* 4102  out     dx,al */
    asm_out8(DX, AL);
L4103:
    /* 4103  mov     al,cl */
    AL = CL;
L4105:
    /* 4105  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4106:
    /* 4106  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4107:
    /* 4107  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L4109:
    /* 4109  mov     bl,al */
    BL = AL;
L410B:
    /* 410B  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L410D:
    /* 410D  mov     ax,word ptr [bx+4A26h] */
    AX = rw(pDS, BX + 0x4A26);
L4111:
    /* 4111  out     dx,al */
    asm_out8(DX, AL);
L4112:
    /* 4112  mov     al,cl */
    AL = CL;
L4114:
    /* 4114  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L4115:
    /* 4115  mov     al,ah */
    AL = AH;
L4117:
    /* 4117  out     dx,al */
    asm_out8(DX, AL);
L4118:
    /* 4118  mov     al,cl */
    AL = CL;
L411A:
    /* 411A  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L411B:
    /* 411B  add     di,bp */
    DI = add16(DI, BP, 0);
L411D:
    /* 411D  dec     ch */
    CH = dec8(CH);
L411F:
    /* 411F  je      L4124 */
    if (ZF) goto L4124;
L4121:
    /* 4121  jmp     L3CBD */
    goto L3CBD;
L4124: /* L4124 */
    /* 4124  pop     es */
    SET_ES(pop16());
L4125:
    /* 4125  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L4126: /* L4126 */
    /* 4126  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L4129:
    /* 4129  mov     si,ax */
    SI = AX;
L412B:
    /* 412B  mov     di,word ptr ds:[4D06h] */
    DI = rw(pDS, 0x4D06);
L412F:
    /* 412F  mov     ax,word ptr ds:[4A0Eh] */
    AX = rw(pDS, 0x4A0E);
L4132:
    /* 4132  mov     bx,word ptr ds:[4A10h] */
    BX = rw(pDS, 0x4A10);
L4136:
    /* 4136  mov     cx,word ptr ds:[4D04h] */
    CX = rw(pDS, 0x4D04);
L413A:
    /* 413A  mov     dx,word ptr ds:[4D0Eh] */
    DX = rw(pDS, 0x4D0E);
L413E:
    /* 413E  jmp     _seg003_0272_222B */
    return ASM_JMP(0x0085, 0x222B);
L4141: /* L4141 */
    /* 4141  popf */
    asm_set_flags(pop16());
L4142:
    /* 4142  call    _seg003_0272_4186 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x4186), 0x4145)) != 0) return c;
L4145:
    /* 4145  pushf */
    push16(asm_flags());
L4146:
    /* 4146  sub     bx,ax */
    BX = (uint16_t)(BX - AX);
L4148:
    /* 4148  sub     si,ax */
    SI = (uint16_t)(SI - AX);
L414A:
    /* 414A  cmp     bx,ax */
    sub16(BX, AX, 0);
L414C:
    /* 414C  ja      L4141 */
    if (!CF && !ZF) goto L4141;
L414E:
    /* 414E  jmp     short L417B */
    goto L417B;

    /* seg003_0272_4150  (+4150)
       _4150 (shift_left_1, FM Towns): shift the whole bit buffer left by one pixel (through _4186
       and the entry table at 4C24) and move the text's x back by one, stepping the screen address
       back a byte when x crosses a multiple of 4. */
L4150: /* _seg003_0272_4150 */
    /* 4150  clc */
    CF = 0;
L4151:
    /* 4151  pushf */
    push16(asm_flags());
L4152:
    /* 4152  test    word ptr ds:[4A0Eh],3 */
    logic16((uint16_t)(rw(pDS, 0x4A0E) & 0x3));
L4158:
    /* 4158  jne     L4163 */
    if (!ZF) goto L4163;
L415A:
    /* 415A  dec     word ptr ds:[4D06h] */
    ww(pDS, 0x4D06, (uint16_t)(rw(pDS, 0x4D06) - 1));
L415E:
    /* 415E  sub     word ptr ds:[4A0Eh],4 */
    ww(pDS, 0x4A0E, (uint16_t)(rw(pDS, 0x4A0E) - 0x4));
L4163: /* L4163 */
    /* 4163  inc     word ptr ds:[4A10h] */
    ww(pDS, 0x4A10, (uint16_t)(rw(pDS, 0x4A10) + 1));
L4167:
    /* 4167  dec     word ptr ds:[4A0Eh] */
    ww(pDS, 0x4A0E, (uint16_t)(rw(pDS, 0x4A0E) - 1));
L416B:
    /* 416B  mov     ax,35h */
    AX = 0x35;
L416E:
    /* 416E  mov     bx,word ptr ds:[4D02h] */
    BX = rw(pDS, 0x4D02);
L4172:
    /* 4172  inc     bx */
    BX = (uint16_t)(BX + 1);
L4173:
    /* 4173  mov     si,word ptr ds:[4D00h] */
    SI = rw(pDS, 0x4D00);
L4177:
    /* 4177  cmp     bx,ax */
    sub16(BX, AX, 0);
L4179:
    /* 4179  ja      L4141 */
    if (!CF && !ZF) goto L4141;
L417B: /* L417B */
    /* 417B  add     si,ax */
    SI = (uint16_t)(SI + AX);
L417D:
    /* 417D  sub     si,bx */
    SI = (uint16_t)(SI - BX);
L417F:
    /* 417F  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L4181:
    /* 4181  popf */
    asm_set_flags(pop16());
L4182:
    /* 4182  jmp     word ptr [bx+4C24h] */
    return ASM_JMP(0x0085, rw(pDS, BX + 0x4C24));

    /* seg003_0272_4186  (+4186)
       _4186: one bit left through 53 bytes of a buffer row, unrolled (rcl from SI down to SI-34h);
       entered part way through for shorter rows.

       After its ret is the rasteriser (4225h, the routine 4D14 points at): measure the string
       (string_width), size the bit buffer (4D04 bytes wide, font height rows) and clear it, then OR
       each glyph's rows into it at the running bit position, through the unrolled loop L42CA..L438D
       entered by the font height (L43A5). Characters outside 20h..7Ah are drawn as '?'. */
L4186: /* _seg003_0272_4186 */
    /* 4186  rcl     byte ptr [si],1 */
    wb(pDS, SI, rcl8(rb(pDS, SI), 1));
L4188:
    /* 4188  rcl     byte ptr [si-1],1 */
    wb(pDS, SI + 0xFFFF, rcl8(rb(pDS, SI + 0xFFFF), 1));
L418B:
    /* 418B  rcl     byte ptr [si-2],1 */
    wb(pDS, SI + 0xFFFE, rcl8(rb(pDS, SI + 0xFFFE), 1));
L418E:
    /* 418E  rcl     byte ptr [si-3],1 */
    wb(pDS, SI + 0xFFFD, rcl8(rb(pDS, SI + 0xFFFD), 1));
L4191:
    /* 4191  rcl     byte ptr [si-4],1 */
    wb(pDS, SI + 0xFFFC, rcl8(rb(pDS, SI + 0xFFFC), 1));
L4194:
    /* 4194  rcl     byte ptr [si-5],1 */
    wb(pDS, SI + 0xFFFB, rcl8(rb(pDS, SI + 0xFFFB), 1));
L4197:
    /* 4197  rcl     byte ptr [si-6],1 */
    wb(pDS, SI + 0xFFFA, rcl8(rb(pDS, SI + 0xFFFA), 1));
L419A:
    /* 419A  rcl     byte ptr [si-7],1 */
    wb(pDS, SI + 0xFFF9, rcl8(rb(pDS, SI + 0xFFF9), 1));
L419D:
    /* 419D  rcl     byte ptr [si-8],1 */
    wb(pDS, SI + 0xFFF8, rcl8(rb(pDS, SI + 0xFFF8), 1));
L41A0:
    /* 41A0  rcl     byte ptr [si-9],1 */
    wb(pDS, SI + 0xFFF7, rcl8(rb(pDS, SI + 0xFFF7), 1));
L41A3:
    /* 41A3  rcl     byte ptr [si-0Ah],1 */
    wb(pDS, SI + 0xFFF6, rcl8(rb(pDS, SI + 0xFFF6), 1));
L41A6:
    /* 41A6  rcl     byte ptr [si-0Bh],1 */
    wb(pDS, SI + 0xFFF5, rcl8(rb(pDS, SI + 0xFFF5), 1));
L41A9:
    /* 41A9  rcl     byte ptr [si-0Ch],1 */
    wb(pDS, SI + 0xFFF4, rcl8(rb(pDS, SI + 0xFFF4), 1));
L41AC:
    /* 41AC  rcl     byte ptr [si-0Dh],1 */
    wb(pDS, SI + 0xFFF3, rcl8(rb(pDS, SI + 0xFFF3), 1));
L41AF:
    /* 41AF  rcl     byte ptr [si-0Eh],1 */
    wb(pDS, SI + 0xFFF2, rcl8(rb(pDS, SI + 0xFFF2), 1));
L41B2:
    /* 41B2  rcl     byte ptr [si-0Fh],1 */
    wb(pDS, SI + 0xFFF1, rcl8(rb(pDS, SI + 0xFFF1), 1));
L41B5:
    /* 41B5  rcl     byte ptr [si-10h],1 */
    wb(pDS, SI + 0xFFF0, rcl8(rb(pDS, SI + 0xFFF0), 1));
L41B8:
    /* 41B8  rcl     byte ptr [si-11h],1 */
    wb(pDS, SI + 0xFFEF, rcl8(rb(pDS, SI + 0xFFEF), 1));
L41BB:
    /* 41BB  rcl     byte ptr [si-12h],1 */
    wb(pDS, SI + 0xFFEE, rcl8(rb(pDS, SI + 0xFFEE), 1));
L41BE:
    /* 41BE  rcl     byte ptr [si-13h],1 */
    wb(pDS, SI + 0xFFED, rcl8(rb(pDS, SI + 0xFFED), 1));
L41C1:
    /* 41C1  rcl     byte ptr [si-14h],1 */
    wb(pDS, SI + 0xFFEC, rcl8(rb(pDS, SI + 0xFFEC), 1));
L41C4:
    /* 41C4  rcl     byte ptr [si-15h],1 */
    wb(pDS, SI + 0xFFEB, rcl8(rb(pDS, SI + 0xFFEB), 1));
L41C7:
    /* 41C7  rcl     byte ptr [si-16h],1 */
    wb(pDS, SI + 0xFFEA, rcl8(rb(pDS, SI + 0xFFEA), 1));
L41CA:
    /* 41CA  rcl     byte ptr [si-17h],1 */
    wb(pDS, SI + 0xFFE9, rcl8(rb(pDS, SI + 0xFFE9), 1));
L41CD:
    /* 41CD  rcl     byte ptr [si-18h],1 */
    wb(pDS, SI + 0xFFE8, rcl8(rb(pDS, SI + 0xFFE8), 1));
L41D0:
    /* 41D0  rcl     byte ptr [si-19h],1 */
    wb(pDS, SI + 0xFFE7, rcl8(rb(pDS, SI + 0xFFE7), 1));
L41D3:
    /* 41D3  rcl     byte ptr [si-1Ah],1 */
    wb(pDS, SI + 0xFFE6, rcl8(rb(pDS, SI + 0xFFE6), 1));
L41D6:
    /* 41D6  rcl     byte ptr [si-1Bh],1 */
    wb(pDS, SI + 0xFFE5, rcl8(rb(pDS, SI + 0xFFE5), 1));
L41D9:
    /* 41D9  rcl     byte ptr [si-1Ch],1 */
    wb(pDS, SI + 0xFFE4, rcl8(rb(pDS, SI + 0xFFE4), 1));
L41DC:
    /* 41DC  rcl     byte ptr [si-1Dh],1 */
    wb(pDS, SI + 0xFFE3, rcl8(rb(pDS, SI + 0xFFE3), 1));
L41DF:
    /* 41DF  rcl     byte ptr [si-1Eh],1 */
    wb(pDS, SI + 0xFFE2, rcl8(rb(pDS, SI + 0xFFE2), 1));
L41E2:
    /* 41E2  rcl     byte ptr [si-1Fh],1 */
    wb(pDS, SI + 0xFFE1, rcl8(rb(pDS, SI + 0xFFE1), 1));
L41E5:
    /* 41E5  rcl     byte ptr [si-20h],1 */
    wb(pDS, SI + 0xFFE0, rcl8(rb(pDS, SI + 0xFFE0), 1));
L41E8:
    /* 41E8  rcl     byte ptr [si-21h],1 */
    wb(pDS, SI + 0xFFDF, rcl8(rb(pDS, SI + 0xFFDF), 1));
L41EB:
    /* 41EB  rcl     byte ptr [si-22h],1 */
    wb(pDS, SI + 0xFFDE, rcl8(rb(pDS, SI + 0xFFDE), 1));
L41EE:
    /* 41EE  rcl     byte ptr [si-23h],1 */
    wb(pDS, SI + 0xFFDD, rcl8(rb(pDS, SI + 0xFFDD), 1));
L41F1:
    /* 41F1  rcl     byte ptr [si-24h],1 */
    wb(pDS, SI + 0xFFDC, rcl8(rb(pDS, SI + 0xFFDC), 1));
L41F4:
    /* 41F4  rcl     byte ptr [si-25h],1 */
    wb(pDS, SI + 0xFFDB, rcl8(rb(pDS, SI + 0xFFDB), 1));
L41F7:
    /* 41F7  rcl     byte ptr [si-26h],1 */
    wb(pDS, SI + 0xFFDA, rcl8(rb(pDS, SI + 0xFFDA), 1));
L41FA:
    /* 41FA  rcl     byte ptr [si-27h],1 */
    wb(pDS, SI + 0xFFD9, rcl8(rb(pDS, SI + 0xFFD9), 1));
L41FD:
    /* 41FD  rcl     byte ptr [si-28h],1 */
    wb(pDS, SI + 0xFFD8, rcl8(rb(pDS, SI + 0xFFD8), 1));
L4200:
    /* 4200  rcl     byte ptr [si-29h],1 */
    wb(pDS, SI + 0xFFD7, rcl8(rb(pDS, SI + 0xFFD7), 1));
L4203:
    /* 4203  rcl     byte ptr [si-2Ah],1 */
    wb(pDS, SI + 0xFFD6, rcl8(rb(pDS, SI + 0xFFD6), 1));
L4206:
    /* 4206  rcl     byte ptr [si-2Bh],1 */
    wb(pDS, SI + 0xFFD5, rcl8(rb(pDS, SI + 0xFFD5), 1));
L4209:
    /* 4209  rcl     byte ptr [si-2Ch],1 */
    wb(pDS, SI + 0xFFD4, rcl8(rb(pDS, SI + 0xFFD4), 1));
L420C:
    /* 420C  rcl     byte ptr [si-2Dh],1 */
    wb(pDS, SI + 0xFFD3, rcl8(rb(pDS, SI + 0xFFD3), 1));
L420F:
    /* 420F  rcl     byte ptr [si-2Eh],1 */
    wb(pDS, SI + 0xFFD2, rcl8(rb(pDS, SI + 0xFFD2), 1));
L4212:
    /* 4212  rcl     byte ptr [si-2Fh],1 */
    wb(pDS, SI + 0xFFD1, rcl8(rb(pDS, SI + 0xFFD1), 1));
L4215:
    /* 4215  rcl     byte ptr [si-30h],1 */
    wb(pDS, SI + 0xFFD0, rcl8(rb(pDS, SI + 0xFFD0), 1));
L4218:
    /* 4218  rcl     byte ptr [si-31h],1 */
    wb(pDS, SI + 0xFFCF, rcl8(rb(pDS, SI + 0xFFCF), 1));
L421B:
    /* 421B  rcl     byte ptr [si-32h],1 */
    wb(pDS, SI + 0xFFCE, rcl8(rb(pDS, SI + 0xFFCE), 1));
L421E:
    /* 421E  rcl     byte ptr [si-33h],1 */
    wb(pDS, SI + 0xFFCD, rcl8(rb(pDS, SI + 0xFFCD), 1));
L4221:
    /* 4221  rcl     byte ptr [si-34h],1 */
    wb(pDS, SI + 0xFFCC, rcl8(rb(pDS, SI + 0xFFCC), 1));
L4224:
    /* 4224  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L4225:
    /* 4225  mov     di,si */
    DI = SI;
L4227:
    /* 4227  call    _seg003_0272_43C5 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x43C5), 0x422A)) != 0) return c;
L422A:
    /* 422A  mov     si,di */
    SI = DI;
L422C:
    /* 422C  mov     ax,word ptr ds:[4A0Eh] */
    AX = rw(pDS, 0x4A0E);
L422F:
    /* 422F  and     ax,3 */
    AX = (uint16_t)(AX & 0x3);
L4232:
    /* 4232  add     dx,ax */
    DX = (uint16_t)(DX + AX);
L4234:
    /* 4234  shr     dx,1 */
    DX = (uint16_t)(DX >> 1);
L4236:
    /* 4236  shr     dx,1 */
    DX = (uint16_t)(DX >> 1);
L4238:
    /* 4238  shr     dx,1 */
    DX = (uint16_t)(DX >> 1);
L423A:
    /* 423A  mov     cx,dx */
    CX = DX;
L423C:
    /* 423C  inc     cx */
    CX = (uint16_t)(CX + 1);
L423D:
    /* 423D  mov     word ptr ds:[4D04h],cx */
    ww(pDS, 0x4D04, CX);
L4241:
    /* 4241  mov     bp,cx */
    BP = CX;
L4243:
    /* 4243  mov     ax,word ptr ds:[4D0Eh] */
    AX = rw(pDS, 0x4D0E);
L4246:
    /* 4246  mul     cx */
    mul16(CX);
L4248:
    /* 4248  mov     cx,ax */
    CX = AX;
L424A:
    /* 424A  inc     cx */
    CX = (uint16_t)(CX + 1);
L424B:
    /* 424B  mov     word ptr ds:[4D02h],cx */
    ww(pDS, 0x4D02, CX);
L424F:
    /* 424F  mov     di,2D43h */
    DI = 0x2D43;
L4252:
    /* 4252  mov     dx,di */
    DX = DI;
L4254:
    /* 4254  inc     dx */
    DX = (uint16_t)(DX + 1);
L4255:
    /* 4255  mov     word ptr ds:[4D06h],dx */
    ww(pDS, 0x4D06, DX);
L4259:
    /* 4259  add     ax,dx */
    AX = (uint16_t)(AX + DX);
L425B:
    /* 425B  mov     word ptr ds:[4D00h],ax */
    ww(pDS, 0x4D00, AX);
L425E:
    /* 425E  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4260:
    /* 4260  inc     cx */
    CX = (uint16_t)(CX + 1);
L4261:
    /* 4261  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L4263:
    /* 4263  rep stosw */
    while (CX) { ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2)); CX--; }
L4265:
    /* 4265  mov     bx,word ptr ds:[4D0Eh] */
    BX = rw(pDS, 0x4D0E);
L4269:
    /* 4269  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L426B:
    /* 426B  mov     ax,word ptr cs:L43A5[bx-2] */
    AX = rw(CODE003, BX + 0x43A3);
L4270:
    /* 4270  mov     word ptr ds:[4CFAh],ax */
    ww(pDS, 0x4CFA, AX);
L4273:
    /* 4273  mov     bx,si */
    BX = SI;
L4275:
    /* 4275  mov     cx,word ptr ds:[4A0Eh] */
    CX = rw(pDS, 0x4A0E);
L4279:
    /* 4279  and     cx,3 */
    CX = (uint16_t)(CX & 0x3);
L427C:
    /* 427C  jmp     short L42A1 */
    goto L42A1;
L427E: /* L427E */
    /* 427E  cmp     al,8 */
    sub8(AL, 0x8, 0);
L4280:
    /* 4280  jle     L4291 */
    if (ZF || SF != OF) goto L4291;
L4282:
    /* 4282  cmp     ch,8 */
    sub8(CH, 0x8, 0);
L4285:
    /* 4285  jne     L428D */
    if (!ZF) goto L428D;
L4287:
    /* 4287  sub     al,ch */
    AL = (uint8_t)(AL - CH);
L4289:
    /* 4289  xor     ch,ch */
    CH = (uint8_t)(CH ^ CH);
L428B:
    /* 428B  jmp     short L4291 */
    goto L4291;
L428D: /* L428D */
    /* 428D  mov     al,8 */
    AL = 0x8;
L428F:
    /* 428F  mov     ch,al */
    CH = AL;
L4291: /* L4291 */
    /* 4291  add     cl,al */
    CL = (uint8_t)(CL + AL);
L4293:
    /* 4293  cmp     cl,8 */
    sub8(CL, 0x8, 0);
L4296:
    /* 4296  jb      L429C */
    if (CF) goto L429C;
L4298:
    /* 4298  inc     dx */
    DX = (uint16_t)(DX + 1);
L4299:
    /* 4299  sub     cl,8 */
    CL = (uint8_t)(CL - 0x8);
L429C: /* L429C */
    /* 429C  cmp     al,ch */
    sub8(AL, CH, 0);
L429E:
    /* 429E  je      L42A1 */
    if (ZF) goto L42A1;
L42A0:
    /* 42A0  inc     bx */
    BX = (uint16_t)(BX + 1);
L42A1: /* L42A1 */
    /* 42A1  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L42A3:
    /* 42A3  mov     al,byte ptr [bx] */
    AL = rb(pDS, BX);
L42A5:
    /* 42A5  test    al,al */
    logic8((uint8_t)(AL & AL));
L42A7:
    /* 42A7  jne     L42AC */
    if (!ZF) goto L42AC;
L42A9:
    /* 42A9  jmp     L43A4 */
    goto L43A4;
L42AC: /* L42AC */
    /* 42AC  cmp     al,20h */
    sub8(AL, 0x20, 0);
L42AE:
    /* 42AE  jl      L42B4 */
    if (SF != OF) goto L42B4;
L42B0:
    /* 42B0  cmp     al,7Ah */
    sub8(AL, 0x7A, 0);
L42B2:
    /* 42B2  jle     L42B6 */
    if (ZF || SF != OF) goto L42B6;
L42B4: /* L42B4 */
    /* 42B4  mov     al,3Fh */
    AL = 0x3F;
L42B6: /* L42B6 */
    /* 42B6  mov     si,ax */
    SI = AX;
L42B8:
    /* 42B8  shl     si,1 */
    SI = (uint16_t)(SI << 1);
L42BA:
    /* 42BA  mov     si,word ptr [si+4D18h] */
    SI = rw(pDS, SI + 0x4D18);
L42BE:
    /* 42BE  cmp     ch,8 */
    sub8(CH, 0x8, 0);
L42C1:
    /* 42C1  jne     L42C4 */
    if (!ZF) goto L42C4;
L42C3:
    /* 42C3  inc     si */
    SI = inc16(SI);
L42C4: /* L42C4 */
    /* 42C4  mov     di,dx */
    DI = DX;
L42C6:
    /* 42C6  jmp     word ptr ds:[4CFAh] */
    return ASM_JMP(0x0085, rw(pDS, 0x4CFA));
L42CA: /* L42CA */
    /* 42CA  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L42CC:
    /* 42CC  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L42CD:
    /* 42CD  add     si,word ptr ds:[2D3Eh] */
    SI = add16(SI, rw(pDS, 0x2D3E), 0);
L42D1:
    /* 42D1  ror     ax,cl */
    AX = ror16(AX, CL);
L42D3:
    /* 42D3  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L42D5:
    /* 42D5  add     di,bp */
    DI = (uint16_t)(DI + BP);
L42D7: /* L42D7 */
    /* 42D7  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L42D9:
    /* 42D9  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L42DA:
    /* 42DA  add     si,word ptr ds:[2D3Eh] */
    SI = add16(SI, rw(pDS, 0x2D3E), 0);
L42DE:
    /* 42DE  ror     ax,cl */
    AX = ror16(AX, CL);
L42E0:
    /* 42E0  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L42E2:
    /* 42E2  add     di,bp */
    DI = (uint16_t)(DI + BP);
L42E4: /* L42E4 */
    /* 42E4  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L42E6:
    /* 42E6  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L42E7:
    /* 42E7  add     si,word ptr ds:[2D3Eh] */
    SI = add16(SI, rw(pDS, 0x2D3E), 0);
L42EB:
    /* 42EB  ror     ax,cl */
    AX = ror16(AX, CL);
L42ED:
    /* 42ED  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L42EF:
    /* 42EF  add     di,bp */
    DI = (uint16_t)(DI + BP);
L42F1: /* L42F1 */
    /* 42F1  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L42F3:
    /* 42F3  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L42F4:
    /* 42F4  add     si,word ptr ds:[2D3Eh] */
    SI = add16(SI, rw(pDS, 0x2D3E), 0);
L42F8:
    /* 42F8  ror     ax,cl */
    AX = ror16(AX, CL);
L42FA:
    /* 42FA  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L42FC:
    /* 42FC  add     di,bp */
    DI = (uint16_t)(DI + BP);
L42FE: /* L42FE */
    /* 42FE  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4300:
    /* 4300  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4301:
    /* 4301  add     si,word ptr ds:[2D3Eh] */
    SI = add16(SI, rw(pDS, 0x2D3E), 0);
L4305:
    /* 4305  ror     ax,cl */
    AX = ror16(AX, CL);
L4307:
    /* 4307  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4309:
    /* 4309  add     di,bp */
    DI = (uint16_t)(DI + BP);
L430B: /* L430B */
    /* 430B  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L430D:
    /* 430D  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L430E:
    /* 430E  add     si,word ptr ds:[2D3Eh] */
    SI = add16(SI, rw(pDS, 0x2D3E), 0);
L4312:
    /* 4312  ror     ax,cl */
    AX = ror16(AX, CL);
L4314:
    /* 4314  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4316:
    /* 4316  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4318: /* L4318 */
    /* 4318  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L431A:
    /* 431A  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L431B:
    /* 431B  add     si,word ptr ds:[2D3Eh] */
    SI = add16(SI, rw(pDS, 0x2D3E), 0);
L431F:
    /* 431F  ror     ax,cl */
    AX = ror16(AX, CL);
L4321:
    /* 4321  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4323:
    /* 4323  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4325: /* L4325 */
    /* 4325  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4327:
    /* 4327  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4328:
    /* 4328  add     si,word ptr ds:[2D3Eh] */
    SI = add16(SI, rw(pDS, 0x2D3E), 0);
L432C:
    /* 432C  ror     ax,cl */
    AX = ror16(AX, CL);
L432E:
    /* 432E  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4330:
    /* 4330  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4332: /* L4332 */
    /* 4332  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4334:
    /* 4334  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4335:
    /* 4335  add     si,word ptr ds:[2D3Eh] */
    SI = add16(SI, rw(pDS, 0x2D3E), 0);
L4339:
    /* 4339  ror     ax,cl */
    AX = ror16(AX, CL);
L433B:
    /* 433B  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L433D:
    /* 433D  add     di,bp */
    DI = (uint16_t)(DI + BP);
L433F: /* L433F */
    /* 433F  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4341:
    /* 4341  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4342:
    /* 4342  add     si,word ptr ds:[2D3Eh] */
    SI = add16(SI, rw(pDS, 0x2D3E), 0);
L4346:
    /* 4346  ror     ax,cl */
    AX = ror16(AX, CL);
L4348:
    /* 4348  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L434A:
    /* 434A  add     di,bp */
    DI = (uint16_t)(DI + BP);
L434C: /* L434C */
    /* 434C  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L434E:
    /* 434E  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L434F:
    /* 434F  add     si,word ptr ds:[2D3Eh] */
    SI = add16(SI, rw(pDS, 0x2D3E), 0);
L4353:
    /* 4353  ror     ax,cl */
    AX = ror16(AX, CL);
L4355:
    /* 4355  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4357:
    /* 4357  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4359: /* L4359 */
    /* 4359  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L435B:
    /* 435B  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L435C:
    /* 435C  add     si,word ptr ds:[2D3Eh] */
    SI = add16(SI, rw(pDS, 0x2D3E), 0);
L4360:
    /* 4360  ror     ax,cl */
    AX = ror16(AX, CL);
L4362:
    /* 4362  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4364:
    /* 4364  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4366: /* L4366 */
    /* 4366  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4368:
    /* 4368  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4369:
    /* 4369  add     si,word ptr ds:[2D3Eh] */
    SI = add16(SI, rw(pDS, 0x2D3E), 0);
L436D:
    /* 436D  ror     ax,cl */
    AX = ror16(AX, CL);
L436F:
    /* 436F  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4371:
    /* 4371  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4373: /* L4373 */
    /* 4373  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4375:
    /* 4375  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4376:
    /* 4376  add     si,word ptr ds:[2D3Eh] */
    SI = add16(SI, rw(pDS, 0x2D3E), 0);
L437A:
    /* 437A  ror     ax,cl */
    AX = ror16(AX, CL);
L437C:
    /* 437C  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L437E:
    /* 437E  add     di,bp */
    DI = (uint16_t)(DI + BP);
L4380: /* L4380 */
    /* 4380  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4382:
    /* 4382  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4383:
    /* 4383  add     si,word ptr ds:[2D3Eh] */
    SI = add16(SI, rw(pDS, 0x2D3E), 0);
L4387:
    /* 4387  ror     ax,cl */
    AX = ror16(AX, CL);
L4389:
    /* 4389  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L438B:
    /* 438B  add     di,bp */
    DI = (uint16_t)(DI + BP);
L438D: /* L438D */
    /* 438D  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L438F:
    /* 438F  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L4390:
    /* 4390  add     si,word ptr ds:[2D3Eh] */
    SI = add16(SI, rw(pDS, 0x2D3E), 0);
L4394:
    /* 4394  ror     ax,cl */
    AX = ror16(AX, CL);
L4396:
    /* 4396  or      word ptr [di],ax */
    ww(pDS, DI, (uint16_t)(rw(pDS, DI) | AX));
L4398:
    /* 4398  add     di,bp */
    DI = (uint16_t)(DI + BP);
L439A:
    /* 439A  cmp     ch,8 */
    sub8(CH, 0x8, 0);
L439D:
    /* 439D  jne     L43A0 */
    if (!ZF) goto L43A0;
L439F:
    /* 439F  dec     si */
    SI = (uint16_t)(SI - 1);
L43A0: /* L43A0 */
    /* 43A0  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L43A1:
    /* 43A1  jmp     L427E */
    goto L427E;
L43A4: /* L43A4 */
    /* 43A4  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_43C5  (+43C5)
       string_width: in: SI = the string. Out: AX = DX = the sum of its characters' widths (the byte
       just past each glyph's rows), stopping at the terminating 0 or after 54 characters. GRCORE's
       string_width is the C wrapper. */
L43C5: /* _seg003_0272_43C5 */
    /* 43C5  mov     bx,word ptr ds:[2D3Ch] */
    BX = rw(pDS, 0x2D3C);
L43C9:
    /* 43C9  mov     cx,word ptr ds:[2D3Eh] */
    CX = rw(pDS, 0x2D3E);
L43CD:
    /* 43CD  shl     bx,cl */
    BX = (uint16_t)(BX << (CL & 31));
L43CF:
    /* 43CF  mov     cx,36h */
    CX = 0x36;
L43D2:
    /* 43D2  sub     dx,dx */
    DX = (uint16_t)(DX - DX);
L43D4:
    /* 43D4  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L43D6: /* L43D6 */
    /* 43D6  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L43D7:
    /* 43D7  test    al,al */
    logic8((uint8_t)(AL & AL));
L43D9:
    /* 43D9  je      L43EC */
    if (ZF) goto L43EC;
L43DB:
    /* 43DB  mov     bp,ax */
    BP = AX;
L43DD:
    /* 43DD  shl     bp,1 */
    BP = (uint16_t)(BP << 1);
L43DF:
    /* 43DF  mov     bp,word ptr [bp+4D18h] */
    BP = rw(pSS, BP + 0x4D18);
L43E3:
    /* 43E3  add     bp,bx */
    BP = (uint16_t)(BP + BX);
L43E5:
    /* 43E5  mov     al,byte ptr [bp] */
    AL = rb(pSS, BP);
L43E8:
    /* 43E8  add     dx,ax */
    DX = add16(DX, AX, 0);
L43EA:
    /* 43EA  loop    L43D6 */
    if (--CX) goto L43D6;
L43EC: /* L43EC */
    /* 43EC  mov     ax,dx */
    AX = DX;
L43EE: /* L43EE */
    /* 43EE  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
