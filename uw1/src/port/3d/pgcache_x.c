/* pgcache_x.c: replaces src/3d/PGCACHE.ASM (seg004_6950, 6950..7DDC of its
   segment). Written by tools/asm2c.py from the assembly and the bytes it assembles to; do not
   edit by hand: change the tool, or its OVERRIDES, and run it again. Each instruction is the
   C below its source line; the comments before a routine are the .ASM file's. */
#include "x86/asmrt.h"

extern unsigned char Palettes[];         /* LOADGR.C */
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

uint32_t asm_mod_PGCACHE(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x696C: goto L696C;
    case 0x696D: goto L696D;
    case 0x796E: goto L796E;
    case 0x796F: goto L796F;
    case 0x7971: goto L7971;
    case 0x7972: goto L7972;
    case 0x7974: goto L7974;
    case 0x7975: goto L7975;
    case 0x7978: goto L7978;
    case 0x797A: goto L797A;
    case 0x797B: goto L797B;
    case 0x797C: goto L797C;
    case 0x797D: goto L797D;
    case 0x797E: goto L797E;
    case 0x7982: goto L7982;
    case 0x7984: goto L7984;
    case 0x7988: goto L7988;
    case 0x798A: goto L798A;
    case 0x798C: goto L798C;
    case 0x7990: goto L7990;
    case 0x7992: goto L7992;
    case 0x7995: goto L7995;
    case 0x7997: goto L7997;
    case 0x7999: goto L7999;
    case 0x799B: goto L799B;
    case 0x799D: goto L799D;
    case 0x79A1: goto L79A1;
    case 0x79A3: goto L79A3;
    case 0x79A5: goto L79A5;
    case 0x79A7: goto L79A7;
    case 0x79A8: goto L79A8;
    case 0x79A9: goto L79A9;
    case 0x79AC: goto L79AC;
    case 0x79AE: goto L79AE;
    case 0x79B1: goto L79B1;
    case 0x79B3: goto L79B3;
    case 0x79B5: goto L79B5;
    case 0x79B6: goto L79B6;
    case 0x79B9: goto L79B9;
    case 0x79BB: goto L79BB;
    case 0x79BE: goto L79BE;
    case 0x79C0: goto L79C0;
    case 0x79C2: goto L79C2;
    case 0x79C5: goto L79C5;
    case 0x79C9: goto L79C9;
    case 0x79CA: goto L79CA;
    case 0x79CB: goto L79CB;
    case 0x79CC: goto L79CC;
    case 0x79CD: goto L79CD;
    case 0x79CE: goto L79CE;
    case 0x79D2: goto L79D2;
    case 0x79D3: goto L79D3;
    case 0x79D5: goto L79D5;
    case 0x79D6: goto L79D6;
    case 0x79D7: goto L79D7;
    case 0x79D8: goto L79D8;
    case 0x79D9: goto L79D9;
    case 0x79DA: goto L79DA;
    case 0x79DC: goto L79DC;
    case 0x79DF: goto L79DF;
    case 0x79E1: goto L79E1;
    case 0x79E3: goto L79E3;
    case 0x79E5: goto L79E5;
    case 0x79E8: goto L79E8;
    case 0x79EA: goto L79EA;
    case 0x79EB: goto L79EB;
    case 0x79EE: goto L79EE;
    case 0x79F1: goto L79F1;
    case 0x79F3: goto L79F3;
    case 0x79F5: goto L79F5;
    case 0x79F7: goto L79F7;
    case 0x79FA: goto L79FA;
    case 0x79FB: goto L79FB;
    case 0x79FE: goto L79FE;
    case 0x7A00: goto L7A00;
    case 0x7A01: goto L7A01;
    case 0x7A02: goto L7A02;
    case 0x7A05: goto L7A05;
    case 0x7A06: goto L7A06;
    case 0x7A09: goto L7A09;
    case 0x7A0B: goto L7A0B;
    case 0x7A0D: goto L7A0D;
    case 0x7A0F: goto L7A0F;
    case 0x7A11: goto L7A11;
    case 0x7A14: goto L7A14;
    case 0x7A16: goto L7A16;
    case 0x7A18: goto L7A18;
    case 0x7A1A: goto L7A1A;
    case 0x7A1B: goto L7A1B;
    case 0x7A1D: goto L7A1D;
    case 0x7A1E: goto L7A1E;
    case 0x7A1F: goto L7A1F;
    case 0x7A22: goto L7A22;
    case 0x7A24: goto L7A24;
    case 0x7A27: goto L7A27;
    case 0x7A2A: goto L7A2A;
    case 0x7A2C: goto L7A2C;
    case 0x7A2E: goto L7A2E;
    case 0x7A2F: goto L7A2F;
    case 0x7A30: goto L7A30;
    case 0x7A31: goto L7A31;
    case 0x7A32: goto L7A32;
    case 0x7A33: goto L7A33;
    case 0x7A37: goto L7A37;
    case 0x7A38: goto L7A38;
    case 0x7A3B: goto L7A3B;
    case 0x7A3F: goto L7A3F;
    case 0x7A41: goto L7A41;
    case 0x7A42: goto L7A42;
    case 0x7A45: goto L7A45;
    case 0x7A47: goto L7A47;
    case 0x7A4A: goto L7A4A;
    case 0x7A4D: goto L7A4D;
    case 0x7A4F: goto L7A4F;
    case 0x7A51: goto L7A51;
    case 0x7A54: goto L7A54;
    case 0x7A57: goto L7A57;
    case 0x7A59: goto L7A59;
    case 0x7A5B: goto L7A5B;
    case 0x7A5D: goto L7A5D;
    case 0x7A60: goto L7A60;
    case 0x7A62: goto L7A62;
    case 0x7A63: goto L7A63;
    case 0x7A65: goto L7A65;
    case 0x7A67: goto L7A67;
    case 0x7A68: goto L7A68;
    case 0x7A69: goto L7A69;
    case 0x7A6B: goto L7A6B;
    case 0x7A6D: goto L7A6D;
    case 0x7A71: goto L7A71;
    case 0x7A74: goto L7A74;
    case 0x7A76: goto L7A76;
    case 0x7A78: goto L7A78;
    case 0x7A7A: goto L7A7A;
    case 0x7A7C: goto L7A7C;
    case 0x7A7E: goto L7A7E;
    case 0x7A80: goto L7A80;
    case 0x7A82: goto L7A82;
    case 0x7A85: goto L7A85;
    case 0x7A87: goto L7A87;
    case 0x7A8A: goto L7A8A;
    case 0x7A8C: goto L7A8C;
    case 0x7A8E: goto L7A8E;
    case 0x7A90: goto L7A90;
    case 0x7A93: goto L7A93;
    case 0x7A98: goto L7A98;
    case 0x7A99: goto L7A99;
    case 0x7A9C: goto L7A9C;
    case 0x7A9F: goto L7A9F;
    case 0x7AA1: goto L7AA1;
    case 0x7AA2: goto L7AA2;
    case 0x7AA5: goto L7AA5;
    case 0x7AA6: goto L7AA6;
    case 0x7AA7: goto L7AA7;
    case 0x7AA8: goto L7AA8;
    case 0x7AAB: goto L7AAB;
    case 0x7AAD: goto L7AAD;
    case 0x7AAE: goto L7AAE;
    case 0x7AAF: goto L7AAF;
    case 0x7AB1: goto L7AB1;
    case 0x7AB5: goto L7AB5;
    case 0x7AB8: goto L7AB8;
    case 0x7ABD: goto L7ABD;
    case 0x7ABE: goto L7ABE;
    case 0x7AC0: goto L7AC0;
    case 0x7AC3: goto L7AC3;
    case 0x7AC5: goto L7AC5;
    case 0x7AC7: goto L7AC7;
    case 0x7AC9: goto L7AC9;
    case 0x7ACB: goto L7ACB;
    case 0x7ACE: goto L7ACE;
    case 0x7AD0: goto L7AD0;
    case 0x7AD3: goto L7AD3;
    case 0x7AD6: goto L7AD6;
    case 0x7AD8: goto L7AD8;
    case 0x7ADA: goto L7ADA;
    case 0x7ADC: goto L7ADC;
    case 0x7ADE: goto L7ADE;
    case 0x7ADF: goto L7ADF;
    case 0x7AE1: goto L7AE1;
    case 0x7AE2: goto L7AE2;
    case 0x7AE3: goto L7AE3;
    case 0x7AE6: goto L7AE6;
    case 0x7AE8: goto L7AE8;
    case 0x7AEA: goto L7AEA;
    case 0x7AEB: goto L7AEB;
    case 0x7AED: goto L7AED;
    case 0x7AF0: goto L7AF0;
    case 0x7AF2: goto L7AF2;
    case 0x7AF4: goto L7AF4;
    case 0x7AF6: goto L7AF6;
    case 0x7AF9: goto L7AF9;
    case 0x7AFA: goto L7AFA;
    case 0x7AFB: goto L7AFB;
    case 0x7AFD: goto L7AFD;
    case 0x7B00: goto L7B00;
    case 0x7B03: goto L7B03;
    case 0x7B04: goto L7B04;
    case 0x7B05: goto L7B05;
    case 0x7B09: goto L7B09;
    case 0x7B0B: goto L7B0B;
    case 0x7B0D: goto L7B0D;
    case 0x7B0F: goto L7B0F;
    case 0x7B13: goto L7B13;
    case 0x7B15: goto L7B15;
    case 0x7B18: goto L7B18;
    case 0x7B1A: goto L7B1A;
    case 0x7B1C: goto L7B1C;
    case 0x7B1E: goto L7B1E;
    case 0x7B22: goto L7B22;
    case 0x7B24: goto L7B24;
    case 0x7B26: goto L7B26;
    case 0x7B28: goto L7B28;
    case 0x7B2A: goto L7B2A;
    case 0x7B2D: goto L7B2D;
    case 0x7B31: goto L7B31;
    case 0x7B34: goto L7B34;
    case 0x7B35: goto L7B35;
    case 0x7B36: goto L7B36;
    case 0x7B37: goto L7B37;
    case 0x7B3C: goto L7B3C;
    case 0x7B3D: goto L7B3D;
    case 0x7B3F: goto L7B3F;
    case 0x7B42: goto L7B42;
    case 0x7B44: goto L7B44;
    case 0x7B47: goto L7B47;
    case 0x7B4A: goto L7B4A;
    case 0x7B4C: goto L7B4C;
    case 0x7B4E: goto L7B4E;
    case 0x7B50: goto L7B50;
    case 0x7B52: goto L7B52;
    case 0x7B54: goto L7B54;
    case 0x7B56: goto L7B56;
    case 0x7B57: goto L7B57;
    case 0x7B59: goto L7B59;
    case 0x7B5B: goto L7B5B;
    case 0x7B5C: goto L7B5C;
    case 0x7B5F: goto L7B5F;
    case 0x7B61: goto L7B61;
    case 0x7B63: goto L7B63;
    case 0x7B67: goto L7B67;
    case 0x7B69: goto L7B69;
    case 0x7B6B: goto L7B6B;
    case 0x7B6D: goto L7B6D;
    case 0x7B6F: goto L7B6F;
    case 0x7B71: goto L7B71;
    case 0x7B72: goto L7B72;
    case 0x7B74: goto L7B74;
    case 0x7B77: goto L7B77;
    case 0x7B79: goto L7B79;
    case 0x7B7C: goto L7B7C;
    case 0x7B7E: goto L7B7E;
    case 0x7B80: goto L7B80;
    case 0x7B82: goto L7B82;
    case 0x7B84: goto L7B84;
    case 0x7B89: goto L7B89;
    case 0x7B8D: goto L7B8D;
    case 0x7B92: goto L7B92;
    case 0x7B94: goto L7B94;
    case 0x7B98: goto L7B98;
    case 0x7B9A: goto L7B9A;
    case 0x7B9E: goto L7B9E;
    case 0x7BA0: goto L7BA0;
    case 0x7BA2: goto L7BA2;
    case 0x7BA4: goto L7BA4;
    case 0x7BA7: goto L7BA7;
    case 0x7BAA: goto L7BAA;
    case 0x7BAC: goto L7BAC;
    case 0x7BAE: goto L7BAE;
    case 0x7BB1: goto L7BB1;
    case 0x7BB4: goto L7BB4;
    case 0x7BB5: goto L7BB5;
    case 0x7BB6: goto L7BB6;
    case 0x7BBA: goto L7BBA;
    case 0x7BBC: goto L7BBC;
    case 0x7BBE: goto L7BBE;
    case 0x7BC0: goto L7BC0;
    case 0x7BC4: goto L7BC4;
    case 0x7BC6: goto L7BC6;
    case 0x7BC8: goto L7BC8;
    case 0x7BCB: goto L7BCB;
    case 0x7BCE: goto L7BCE;
    case 0x7BD0: goto L7BD0;
    case 0x7BD2: goto L7BD2;
    case 0x7BD4: goto L7BD4;
    case 0x7BD8: goto L7BD8;
    case 0x7BDA: goto L7BDA;
    case 0x7BDC: goto L7BDC;
    case 0x7BDD: goto L7BDD;
    case 0x7BDF: goto L7BDF;
    case 0x7BE0: goto L7BE0;
    case 0x7BE2: goto L7BE2;
    case 0x7BE5: goto L7BE5;
    case 0x7BE7: goto L7BE7;
    case 0x7BEB: goto L7BEB;
    case 0x7BEC: goto L7BEC;
    case 0x7BF0: goto L7BF0;
    case 0x7BF4: goto L7BF4;
    case 0x7BF5: goto L7BF5;
    case 0x7BF7: goto L7BF7;
    case 0x7BFA: goto L7BFA;
    case 0x7BFD: goto L7BFD;
    case 0x7BFF: goto L7BFF;
    case 0x7C01: goto L7C01;
    case 0x7C03: goto L7C03;
    case 0x7C05: goto L7C05;
    case 0x7C06: goto L7C06;
    case 0x7C08: goto L7C08;
    case 0x7C0B: goto L7C0B;
    case 0x7C0D: goto L7C0D;
    case 0x7C0E: goto L7C0E;
    case 0x7C11: goto L7C11;
    case 0x7C13: goto L7C13;
    case 0x7C15: goto L7C15;
    case 0x7C17: goto L7C17;
    case 0x7C18: goto L7C18;
    case 0x7C1A: goto L7C1A;
    case 0x7C1C: goto L7C1C;
    case 0x7C1E: goto L7C1E;
    case 0x7C1F: goto L7C1F;
    case 0x7C22: goto L7C22;
    case 0x7C25: goto L7C25;
    case 0x7C27: goto L7C27;
    case 0x7C29: goto L7C29;
    case 0x7C2B: goto L7C2B;
    case 0x7C2C: goto L7C2C;
    case 0x7C2D: goto L7C2D;
    case 0x7C30: goto L7C30;
    case 0x7C31: goto L7C31;
    case 0x7C34: goto L7C34;
    case 0x7C37: goto L7C37;
    case 0x7C3A: goto L7C3A;
    case 0x7C3C: goto L7C3C;
    case 0x7C3F: goto L7C3F;
    case 0x7C41: goto L7C41;
    case 0x7C42: goto L7C42;
    case 0x7C44: goto L7C44;
    case 0x7C47: goto L7C47;
    case 0x7C49: goto L7C49;
    case 0x7C4B: goto L7C4B;
    case 0x7C4C: goto L7C4C;
    case 0x7C4F: goto L7C4F;
    case 0x7C51: goto L7C51;
    case 0x7C53: goto L7C53;
    case 0x7C57: goto L7C57;
    case 0x7C5C: goto L7C5C;
    case 0x7C5D: goto L7C5D;
    case 0x7C5F: goto L7C5F;
    case 0x7C62: goto L7C62;
    case 0x7C64: goto L7C64;
    case 0x7C67: goto L7C67;
    case 0x7C6A: goto L7C6A;
    case 0x7C6E: goto L7C6E;
    case 0x7C70: goto L7C70;
    case 0x7C75: goto L7C75;
    case 0x7C79: goto L7C79;
    case 0x7C7E: goto L7C7E;
    case 0x7C80: goto L7C80;
    case 0x7C85: goto L7C85;
    case 0x7C86: goto L7C86;
    case 0x7C87: goto L7C87;
    case 0x7C8B: goto L7C8B;
    case 0x7C8F: goto L7C8F;
    case 0x7C92: goto L7C92;
    case 0x7C94: goto L7C94;
    case 0x7C99: goto L7C99;
    case 0x7C9D: goto L7C9D;
    case 0x7C9F: goto L7C9F;
    case 0x7CA0: goto L7CA0;
    case 0x7CA3: goto L7CA3;
    case 0x7CA5: goto L7CA5;
    case 0x7CA7: goto L7CA7;
    case 0x7CAB: goto L7CAB;
    case 0x7CAC: goto L7CAC;
    case 0x7CAF: goto L7CAF;
    case 0x7CB1: goto L7CB1;
    case 0x7CB2: goto L7CB2;
    case 0x7CB5: goto L7CB5;
    case 0x7CB7: goto L7CB7;
    case 0x7CBA: goto L7CBA;
    case 0x7CBD: goto L7CBD;
    case 0x7CC0: goto L7CC0;
    case 0x7CC4: goto L7CC4;
    case 0x7CC6: goto L7CC6;
    case 0x7CC7: goto L7CC7;
    case 0x7CC9: goto L7CC9;
    case 0x7CCB: goto L7CCB;
    case 0x7CCC: goto L7CCC;
    case 0x7CCE: goto L7CCE;
    case 0x7CCF: goto L7CCF;
    case 0x7CD2: goto L7CD2;
    case 0x7CD4: goto L7CD4;
    case 0x7CD7: goto L7CD7;
    case 0x7CD8: goto L7CD8;
    case 0x7CD9: goto L7CD9;
    case 0x7CDD: goto L7CDD;
    case 0x7CE0: goto L7CE0;
    case 0x7CE2: goto L7CE2;
    case 0x7CE4: goto L7CE4;
    case 0x7CE6: goto L7CE6;
    case 0x7CE8: goto L7CE8;
    case 0x7CEA: goto L7CEA;
    case 0x7CEC: goto L7CEC;
    case 0x7CEF: goto L7CEF;
    case 0x7CF3: goto L7CF3;
    case 0x7CF6: goto L7CF6;
    case 0x7CF8: goto L7CF8;
    case 0x7CFA: goto L7CFA;
    case 0x7CFD: goto L7CFD;
    case 0x7CFF: goto L7CFF;
    case 0x7D01: goto L7D01;
    case 0x7D02: goto L7D02;
    case 0x7D04: goto L7D04;
    case 0x7D06: goto L7D06;
    case 0x7D08: goto L7D08;
    case 0x7D0B: goto L7D0B;
    case 0x7D0D: goto L7D0D;
    case 0x7D0F: goto L7D0F;
    case 0x7D11: goto L7D11;
    case 0x7D13: goto L7D13;
    case 0x7D15: goto L7D15;
    case 0x7D17: goto L7D17;
    case 0x7D19: goto L7D19;
    case 0x7D1B: goto L7D1B;
    case 0x7D1C: goto L7D1C;
    case 0x7D1E: goto L7D1E;
    case 0x7D20: goto L7D20;
    case 0x7D22: goto L7D22;
    case 0x7D24: goto L7D24;
    case 0x7D26: goto L7D26;
    case 0x7D28: goto L7D28;
    case 0x7D2A: goto L7D2A;
    case 0x7D2C: goto L7D2C;
    case 0x7D2E: goto L7D2E;
    case 0x7D30: goto L7D30;
    case 0x7D32: goto L7D32;
    case 0x7D34: goto L7D34;
    case 0x7D35: goto L7D35;
    case 0x7D37: goto L7D37;
    case 0x7D39: goto L7D39;
    case 0x7D3A: goto L7D3A;
    case 0x7D3C: goto L7D3C;
    case 0x7D3E: goto L7D3E;
    case 0x7D40: goto L7D40;
    case 0x7D41: goto L7D41;
    case 0x7D42: goto L7D42;
    case 0x7D43: goto L7D43;
    case 0x7D45: goto L7D45;
    case 0x7D47: goto L7D47;
    case 0x7D49: goto L7D49;
    case 0x7D4D: goto L7D4D;
    case 0x7D4F: goto L7D4F;
    case 0x7D52: goto L7D52;
    case 0x7D57: goto L7D57;
    case 0x7D58: goto L7D58;
    case 0x7D59: goto L7D59;
    case 0x7D5C: goto L7D5C;
    case 0x7D5F: goto L7D5F;
    case 0x7D61: goto L7D61;
    case 0x7D62: goto L7D62;
    case 0x7D63: goto L7D63;
    case 0x7D64: goto L7D64;
    case 0x7D65: goto L7D65;
    case 0x7D66: goto L7D66;
    case 0x7D67: goto L7D67;
    case 0x7D68: goto L7D68;
    case 0x7D69: goto L7D69;
    case 0x7D6A: goto L7D6A;
    case 0x7D6D: goto L7D6D;
    case 0x7D6F: goto L7D6F;
    case 0x7D70: goto L7D70;
    case 0x7D72: goto L7D72;
    case 0x7D76: goto L7D76;
    case 0x7D79: goto L7D79;
    case 0x7D7E: goto L7D7E;
    case 0x7D80: goto L7D80;
    case 0x7D83: goto L7D83;
    case 0x7D85: goto L7D85;
    case 0x7D86: goto L7D86;
    case 0x7D8A: goto L7D8A;
    case 0x7D8C: goto L7D8C;
    case 0x7D8D: goto L7D8D;
    case 0x7D8E: goto L7D8E;
    case 0x7D92: goto L7D92;
    case 0x7D97: goto L7D97;
    case 0x7D98: goto L7D98;
    case 0x7D99: goto L7D99;
    case 0x7D9D: goto L7D9D;
    case 0x7DA3: goto L7DA3;
    case 0x7DA5: goto L7DA5;
    case 0x7DA6: goto L7DA6;
    case 0x7DA8: goto L7DA8;
    case 0x7DA9: goto L7DA9;
    case 0x7DAB: goto L7DAB;
    case 0x7DAD: goto L7DAD;
    case 0x7DAF: goto L7DAF;
    case 0x7DB3: goto L7DB3;
    case 0x7DB5: goto L7DB5;
    case 0x7DB8: goto L7DB8;
    case 0x7DBA: goto L7DBA;
    case 0x7DBC: goto L7DBC;
    case 0x7DC0: goto L7DC0;
    case 0x7DC2: goto L7DC2;
    case 0x7DC5: goto L7DC5;
    case 0x7DC7: goto L7DC7;
    case 0x7DC9: goto L7DC9;
    case 0x7DCA: goto L7DCA;
    case 0x7DCD: goto L7DCD;
    case 0x7DCF: goto L7DCF;
    case 0x7DD3: goto L7DD3;
    case 0x7DD5: goto L7DD5;
    case 0x7DD6: goto L7DD6;
    case 0x7DD9: goto L7DD9;
    default: asm_bad_entry("PGCACHE.ASM", entry);
    }
L696C: /* L696C */
    /* 696C  int     3 */
    asm_halt_at(0x06E7, 0x696C, "int 3, the debugger break");
L696D:
    /* 696D  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_796E  (+796E)
       do_fetchmap (opcode 0x3E): load a wall or floor texture into a bitmap slot. Operands: the
       slot, the texture number, the next opcode. Textures are 4 KB (64 x 64) and four share a 16 KB
       EMS page: page tmap_fpage + n / 4 is mapped into physical page 3 (int 67h, 44h, unless it is
       already there; an EMS error breaks with int 3) and the slot's word +4 gets the texture's
       segment, EmsBuff + 0C00h + (n & 3) * 100h.

       It then sets tmcolor (2936) from the first byte of the texture, but DOS loads DS from SI (the
       slot's address) where FM Towns reads the texture itself, so the byte it reads is not the
       texture's. Probably a DOS slip; what reads tmcolor has not been traced. */
L796E: /* _do_fetchmap */
    /* 796E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L796F:
    /* 796F  mov     dx,ax */
    DX = AX;
L7971:
    /* 7971  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7972:
    /* 7972  mov     bx,ax */
    BX = AX;
L7974:
    /* 7974  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7975:
    /* 7975  shr     ax,4 */
    AX = (uint16_t)(AX >> 4);
L7978:
    /* 7978  mov     cx,ax */
    CX = AX;
L797A:
    /* 797A  push    es */
    push16(asm_es);
L797B:
    /* 797B  push    ds */
    push16(asm_ds);
L797C:
    /* 797C  push    si */
    push16(SI);
L797D:
    /* 797D  push    dx */
    push16(DX);
L797E:
    /* 797E  mov     al,byte ptr [bx-3C88h] */
    AL = rb(pDS, BX + 0xC378);
L7982:
    /* 7982  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L7984:
    /* 7984  mov     si,word ptr [bx-3C14h] */
    SI = rw(pDS, BX + 0xC3EC);
L7988:
    /* 7988  cmp     al,0 */
    sub8(AL, 0x0, 0);
L798A:
    /* 798A  je      short L79A8 */
    if (ZF) goto L79A8;
L798C:
    /* 798C  cmp     al,byte ptr ds:[0C374h] */
    sub8(AL, rb(pDS, 0xC374), 0);
L7990:
    /* 7990  je      short L79A8 */
    if (ZF) goto L79A8;
L7992:
    /* 7992  mov     byte ptr ds:[0C374h],al */
    wb(pDS, 0xC374, AL);
L7995:
    /* 7995  xor     bh,bh */
    BH = (uint8_t)(BH ^ BH);
L7997:
    /* 7997  mov     bl,al */
    BL = AL;
L7999:
    /* 7999  mov     ah,EMS_MAP_PAGE */
    AH = 0x44;
L799B:
    /* 799B  mov     al,3 */
    AL = 0x3;
L799D:
    /* 799D  mov     dx,word ptr ds:[0C4D7h] */
    DX = rw(pDS, 0xC4D7);
L79A1:
    /* 79A1  int     67h */
    asm_int(0x67);
L79A3:
    /* 79A3  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L79A5:
    /* 79A5  je      short L79A8 */
    if (ZF) goto L79A8;
L79A7:
    /* 79A7  int     3 */
    asm_halt_at(0x06E7, 0x79A7, "int 3, the debugger break");
L79A8: /* L79A8 */
    /* 79A8  pop     dx */
    DX = pop16();
L79A9:
    /* 79A9  mov     bx,seg seg051 */
    BX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L79AC:
    /* 79AC  mov     es,bx */
    SET_ES(BX);
L79AE:
    /* 79AE  mov     bx,0B07Eh */
    BX = 0xB07E;
L79B1:
    /* 79B1  mov     ax,dx */
    AX = DX;
L79B3:
    /* 79B3  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L79B5:
    /* 79B5  inc     ax */
    AX = (uint16_t)(AX + 1);
L79B6:
    /* 79B6  shl     ax,2 */
    AX = (uint16_t)(AX << 2);
L79B9:
    /* 79B9  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L79BB:
    /* 79BB  mov     word ptr es:[bx],si */
    ww(pES, BX, SI);
L79BE:
    /* 79BE  mov     ds,si */
    SET_DS(SI);
L79C0:
    /* 79C0  xor     ah,ah */
    AH = logic8((uint8_t)(AH ^ AH));
L79C2:
    /* 79C2  mov     al,byte ptr ds:[0] */
    AL = rb(pDS, 0x0);
L79C5:
    /* 79C5  mov     word ptr es:[2936h],ax */
    ww(pES, 0x2936, AX);
L79C9:
    /* 79C9  pop     si */
    SI = pop16();
L79CA:
    /* 79CA  pop     ds */
    SET_DS(pop16());
L79CB:
    /* 79CB  pop     es */
    SET_ES(pop16());
L79CC:
    /* 79CC  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L79CD:
    /* 79CD  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L79CE:
    /* 79CE  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_79D2  (+79D2)
       do_settmobj (opcode 0xC0): use an object's picture as the texture of bitmap slot 6 (B0AE, FM
       bm_number_6), for a 3D model drawn with an object's own art. Operands: the grs_off slot, an
       unused word, the next opcode. The picture is reached through seg004_7B09; the slot gets
       the picture's width (byte 1), its byte count (word 3) - 1, the segment of the 4 KB buffer
       _seg_5DFD, and (height - 1) shifted by 8 - log2(width) as the row mask; then the picture's
       pixels (from byte 5, word 3 of them) are copied into _seg_5DFD, where the mapper reads them.
       (The `lodsw` after the dispatch jump is never reached. L7A38 is do_uwobj's branch for
       animations.) */
L79D2: /* _do_settmobj */
    /* 79D2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L79D3:
    /* 79D3  mov     cx,ax */
    CX = AX;
L79D5:
    /* 79D5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L79D6:
    /* 79D6  push    es */
    push16(asm_es);
L79D7:
    /* 79D7  push    ds */
    push16(asm_ds);
L79D8:
    /* 79D8  push    si */
    push16(SI);
L79D9:
    /* 79D9  push    ax */
    push16(AX);
L79DA:
    /* 79DA  mov     ax,cx */
    AX = CX;
L79DC:
    /* 79DC  mov     bx,0B10Fh */
    BX = 0xB10F;
L79DF:
    /* 79DF  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L79E1:
    /* 79E1  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L79E3:
    /* 79E3  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L79E5:
    /* 79E5  call    _seg004_7B09 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x7B09), 0x79E8)) != 0) return c;
L79E8:
    /* 79E8  mov     bx,ax */
    BX = AX;
L79EA:
    /* 79EA  push    bx */
    push16(BX);
L79EB:
    /* 79EB  mov     di,0B0AEh */
    DI = 0xB0AE;
L79EE:
    /* 79EE  mov     ax,seg seg051 */
    AX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L79F1:
    /* 79F1  mov     es,ax */
    SET_ES(AX);
L79F3:
    /* 79F3  mov     ds,bx */
    SET_DS(BX);
L79F5:
    /* 79F5  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L79F7:
    /* 79F7  mov     al,byte ptr ds:[1] */
    AL = rb(pDS, 0x1);
L79FA:
    /* 79FA  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L79FB:
    /* 79FB  mov     ax,word ptr ds:[3] */
    AX = rw(pDS, 0x3);
L79FE:
    /* 79FE  mov     bx,ax */
    BX = AX;
L7A00:
    /* 7A00  dec     ax */
    AX = (uint16_t)(AX - 1);
L7A01:
    /* 7A01  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L7A02:
    /* 7A02  mov     ax,seg _seg_5DFD */
    AX = (uint16_t)(0x5371 + PORT_LOAD_SEG);
L7A05:
    /* 7A05  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L7A06:
    /* 7A06  mov     al,byte ptr ds:[1] */
    AL = rb(pDS, 0x1);
L7A09:
    /* 7A09  mov     cl,8 */
    CL = 0x8;
L7A0B: /* L7A0B */
    /* 7A0B  dec     cl */
    CL = (uint8_t)(CL - 1);
L7A0D:
    /* 7A0D  shl     al,1 */
    AL = shl8(AL, 1);
L7A0F:
    /* 7A0F  jae     L7A0B */
    if (!CF) goto L7A0B;
L7A11:
    /* 7A11  mov     al,byte ptr ds:[2] */
    AL = rb(pDS, 0x2);
L7A14:
    /* 7A14  dec     al */
    AL = (uint8_t)(AL - 1);
L7A16:
    /* 7A16  xor     ah,ah */
    AH = logic8((uint8_t)(AH ^ AH));
L7A18:
    /* 7A18  shl     ax,cl */
    AX = shl16(AX, CL);
L7A1A:
    /* 7A1A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L7A1B:
    /* 7A1B  mov     cx,bx */
    CX = BX;
L7A1D:
    /* 7A1D  pop     dx */
    DX = pop16();
L7A1E:
    /* 7A1E  pop     bx */
    BX = pop16();
L7A1F:
    /* 7A1F  mov     si,5 */
    SI = 0x5;
L7A22:
    /* 7A22  mov     ds,dx */
    SET_DS(DX);
L7A24:
    /* 7A24  mov     di,0 */
    DI = 0x0;
L7A27:
    /* 7A27  mov     ax,seg _seg_5DFD */
    AX = (uint16_t)(0x5371 + PORT_LOAD_SEG);
L7A2A:
    /* 7A2A  mov     es,ax */
    SET_ES(AX);
L7A2C:
    /* 7A2C  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L7A2E:
    /* 7A2E  pop     si */
    SI = pop16();
L7A2F:
    /* 7A2F  pop     ds */
    SET_DS(pop16());
L7A30:
    /* 7A30  pop     es */
    SET_ES(pop16());
L7A31:
    /* 7A31  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7A32:
    /* 7A32  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L7A33:
    /* 7A33  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));
L7A37:
    /* 7A37  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7A38: /* L7A38 */
    /* 7A38  sub     ax,1C0h */
    AX = (uint16_t)(AX - 0x1C0);
L7A3B:
    /* 7A3B  add     ax,word ptr ds:[0B0F0h] */
    AX = (uint16_t)(AX + rw(pDS, 0xB0F0));
L7A3F:
    /* 7A3F  jmp     short L7A54 */
    goto L7A54;

    /* seg004_7A41  (+7A41)
       do_uwobj (opcode 0x3A): draw an object's sprite. Operands: the object type, the shade (dh for
       the decoder), the point. Types below 1C0h take their picture slot from obj_tab (bits 0-9);
       1C0h and up are animations, slot first_anim + type - 1C0h. The picture (byte 0 the format, 1
       and 2 width and height, 3 its auxiliary palette, pixels from 4) is decoded through uncmp_tab
       with the 16-byte palette from LOADGR's _Palettes (palette numbers from 20h are folded down: (n
       - 8) * 2), the decoded bitmap's address and size go to the sprite record at B0D2 (FM
       sbm_inventory), and it is drawn by in_scalebm (TMAPOPS.ASM) with record offset 1Ch. */
L7A41: /* _do_uwobj */
    /* 7A41  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7A42:
    /* 7A42  cmp     ax,1C0h */
    sub16(AX, 0x1C0, 0);
L7A45:
    /* 7A45  jge     L7A38 */
    if (SF == OF) goto L7A38;
L7A47:
    /* 7A47  mov     bx,0B90Fh */
    BX = 0xB90F;
L7A4A:
    /* 7A4A  shl     ax,2 */
    AX = (uint16_t)(AX << 2);
L7A4D:
    /* 7A4D  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L7A4F:
    /* 7A4F  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L7A51:
    /* 7A51  and     ax,3FFh */
    AX = (uint16_t)(AX & 0x3FF);
L7A54: /* L7A54 */
    /* 7A54  mov     bx,0B10Fh */
    BX = 0xB10F;
L7A57:
    /* 7A57  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L7A59:
    /* 7A59  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L7A5B:
    /* 7A5B  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L7A5D:
    /* 7A5D  call    _seg004_7B09 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x7B09), 0x7A60)) != 0) return c;
L7A60:
    /* 7A60  mov     bx,ax */
    BX = AX;
L7A62:
    /* 7A62  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7A63:
    /* 7A63  mov     ch,al */
    CH = AL;
L7A65:
    /* 7A65  mov     dx,bx */
    DX = BX;
L7A67:
    /* 7A67  push    si */
    push16(SI);
L7A68:
    /* 7A68  push    bx */
    push16(BX);
L7A69:
    /* 7A69  mov     ds,bx */
    SET_DS(BX);
L7A6B:
    /* 7A6B  xor     bh,bh */
    BH = (uint8_t)(BH ^ BH);
L7A6D:
    /* 7A6D  mov     bl,byte ptr ds:[0] */
    BL = rb(pDS, 0x0);
L7A71:
    /* 7A71  mov     al,byte ptr ds:[3] */
    AL = rb(pDS, 0x3);
L7A74:
    /* 7A74  cmp     al,20h */
    sub8(AL, 0x20, 0);
L7A76:
    /* 7A76  jb      short L7A7C */
    if (CF) goto L7A7C;
L7A78:
    /* 7A78  sub     al,8 */
    AL = (uint8_t)(AL - 0x8);
L7A7A:
    /* 7A7A  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L7A7C: /* L7A7C */
    /* 7A7C  mov     cl,4 */
    CL = 0x4;
L7A7E:
    /* 7A7E  xor     ah,ah */
    AH = logic8((uint8_t)(AH ^ AH));
L7A80:
    /* 7A80  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L7A82:
    /* 7A82  mov     si,offset DGROUP:_Palettes */
    /* by hand: do_uwobj: _Palettes is LOADGR.C's array, a C object in the port, not a DGROUP offset (as UW2's) */
    SI = (uint16_t)port_fp_off(Palettes);
L7A85:
    /* 7A85  add     si,ax */
    SI = (uint16_t)(SI + AX);
L7A87:
    /* 7A87  mov     ax,DGROUP */
    /* by hand: do_uwobj: DGROUP's segment, here the paragraph the port's map gives _Palettes (as UW2's) */
    AX = (uint16_t)port_fp_seg(Palettes);
L7A8A:
    /* 7A8A  mov     ds,ax */
    SET_DS(AX);
L7A8C:
    /* 7A8C  mov     ax,dx */
    AX = DX;
L7A8E:
    /* 7A8E  mov     dh,ch */
    DH = CH;
L7A90:
    /* 7A90  mov     bp,4 */
    BP = 0x4;
L7A93:
    /* 7A93  call    word ptr cs:uncmp_tab[bx] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(CODE004, BX + 0x6960)), 0x7A98)) != 0) return c;
L7A98:
    /* 7A98  pop     ds */
    SET_DS(pop16());
L7A99:
    /* 7A99  mov     di,0B0D2h */
    DI = 0xB0D2;
L7A9C:
    /* 7A9C  mov     bx,seg seg051 */
    BX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L7A9F:
    /* 7A9F  mov     es,bx */
    SET_ES(BX);
L7AA1:
    /* 7AA1  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L7AA2:
    /* 7AA2  mov     si,1 */
    SI = 0x1;
L7AA5:
    /* 7AA5  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L7AA6:
    /* 7AA6  inc     di */
    DI = (uint16_t)(DI + 1);
L7AA7:
    /* 7AA7  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L7AA8:
    /* 7AA8  mov     ax,seg seg051 */
    AX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L7AAB:
    /* 7AAB  mov     ds,ax */
    SET_DS(AX);
L7AAD:
    /* 7AAD  pop     si */
    SI = pop16();
L7AAE:
    /* 7AAE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7AAF:
    /* 7AAF  mov     di,ax */
    DI = AX;
L7AB1:
    /* 7AB1  add     di,PNT_X */
    DI = add16(DI, 0x1620, 0);
L7AB5:
    /* 7AB5  mov     ax,1Ch */
    AX = 0x1C;
L7AB8:
    /* 7AB8  db      0EAh */
    return ASM_JMP(0x06E7, 0x684B);

    /* seg004_7ABD  (+7ABD)
       do_uwcrit (opcode 0x5A): draw a critter's animation frame. Operands: the critter type (index
       into grs_3dinf), a flag (nonzero: palette 3 in place of the critter's own; probably the
       shading used for a critter being hit or a special state, not checked), the shade, the frame
       number, the point. A type with no critter map (0FFh) is skipped. The frame picks the page: the
       first of the critter map's eight CmapFrm bytes not below it. load_cpg makes sure the page is
       in the cache and mapped; parse_cpg decodes and draws the frame. */
L7ABD: /* _do_uwcrit */
    /* 7ABD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7ABE:
    /* 7ABE  mov     cx,ax */
    CX = AX;
L7AC0:
    /* 7AC0  mov     bx,0C10Fh */
    BX = 0xC10F;
L7AC3:
    /* 7AC3  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L7AC5:
    /* 7AC5  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L7AC7:
    /* 7AC7  mov     al,byte ptr [bx] */
    AL = rb(pDS, BX);
L7AC9:
    /* 7AC9  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L7ACB:
    /* 7ACB  cmp     ax,0FFh */
    sub16(AX, 0xFF, 0);
L7ACE:
    /* 7ACE  je      short L7B00 */
    if (ZF) goto L7B00;
L7AD0:
    /* 7AD0  mov     dl,byte ptr [bx+1] */
    DL = rb(pDS, BX + 0x1);
L7AD3:
    /* 7AD3  mov     bx,0C28Fh */
    BX = 0xC28F;
L7AD6:
    /* 7AD6  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L7AD8:
    /* 7AD8  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L7ADA:
    /* 7ADA  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L7ADC:
    /* 7ADC  mov     bp,ax */
    BP = AX;
L7ADE: /* L7ADE */
    /* 7ADE  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7ADF:
    /* 7ADF  mov     dh,al */
    DH = AL;
L7AE1:
    /* 7AE1  push    dx */
    push16(DX);
L7AE2:
    /* 7AE2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7AE3:
    /* 7AE3  mov     cx,3 */
    CX = 0x3;
L7AE6: /* L7AE6 */
    /* 7AE6  cmp     al,byte ptr [bx] */
    sub8(AL, rb(pDS, BX), 0);
L7AE8:
    /* 7AE8  jb      short L7AED */
    if (CF) goto L7AED;
L7AEA:
    /* 7AEA  inc     bx */
    BX = (uint16_t)(BX + 1);
L7AEB:
    /* 7AEB  loop    L7AE6 */
    if (--CX) goto L7AE6;
L7AED: /* L7AED */
    /* 7AED  mov     bx,3 */
    BX = 0x3;
L7AF0:
    /* 7AF0  sub     bx,cx */
    BX = (uint16_t)(BX - CX);
L7AF2:
    /* 7AF2  mov     dx,bp */
    DX = BP;
L7AF4:
    /* 7AF4  mov     bp,ax */
    BP = AX;
L7AF6:
    /* 7AF6  call    _seg004_7BBA */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x7BBA), 0x7AF9)) != 0) return c;
L7AF9:
    /* 7AF9  pop     dx */
    DX = pop16();
L7AFA:
    /* 7AFA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7AFB:
    /* 7AFB  mov     bx,ax */
    BX = AX;
L7AFD:
    /* 7AFD  jmp     _parse_cpg */
    goto L7CDD;
L7B00: /* L7B00 */
    /* 7B00  add     si,8 */
    SI = add16(SI, 0x8, 0);
L7B03:
    /* 7B03  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7B04:
    /* 7B04  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L7B05:
    /* 7B05  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_7B09  (+7B09)
       seg004_7B09 (DOS only; FM Towns has flat memory): turn a grs_off word into a segment. AX
       = the word: bits 12-15 the EMS logical page, mapped into physical page 2 unless it is the one
       there (obj_inpage1; an EMS error breaks with int 3), bits 0-9 the paragraph within the 16 KB
       page. Out: AX = EmsBuff + 800h + paragraph. Changes BX, CL, DX, DI. GRSPIC.C's C code does the
       same for the 2D side. */
L7B09: /* _seg004_7B09 */
    /* 7B09  mov     di,ax */
    DI = AX;
L7B0B:
    /* 7B0B  mov     cl,0Ch */
    CL = 0xC;
L7B0D:
    /* 7B0D  shr     ax,cl */
    AX = (uint16_t)(AX >> (CL & 31));
L7B0F:
    /* 7B0F  cmp     al,byte ptr ds:[0C4D4h] */
    sub8(AL, rb(pDS, 0xC4D4), 0);
L7B13:
    /* 7B13  je      short L7B28 */
    if (ZF) goto L7B28;
L7B15:
    /* 7B15  mov     byte ptr ds:[0C4D4h],al */
    wb(pDS, 0xC4D4, AL);
L7B18:
    /* 7B18  mov     bx,ax */
    BX = AX;
L7B1A:
    /* 7B1A  mov     ah,EMS_MAP_PAGE */
    AH = 0x44;
L7B1C:
    /* 7B1C  mov     al,2 */
    AL = 0x2;
L7B1E:
    /* 7B1E  mov     dx,word ptr ds:[0C4D7h] */
    DX = rw(pDS, 0xC4D7);
L7B22:
    /* 7B22  int     67h */
    asm_int(0x67);
L7B24:
    /* 7B24  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L7B26:
    /* 7B26  jne     short L7B35 */
    if (!ZF) goto L7B35;
L7B28: /* L7B28 */
    /* 7B28  mov     ax,di */
    AX = DI;
L7B2A:
    /* 7B2A  and     ax,3FFh */
    AX = (uint16_t)(AX & 0x3FF);
L7B2D:
    /* 7B2D  add     ax,word ptr ds:[0C4D5h] */
    AX = (uint16_t)(AX + rw(pDS, 0xC4D5));
L7B31:
    /* 7B31  add     ax,800h */
    AX = add16(AX, 0x800, 0);
L7B34:
    /* 7B34  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L7B35: /* L7B35 */
    /* 7B35  int     3 */
    asm_halt_at(0x06E7, 0x7B35, "int 3, the debugger break");
L7B36:
    /* 7B36  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_7B37  (+7B37)
       seg004_7B37: far entry to the decoders for C (cFrmtoRaw), BX = format * 2, other
       registers as the decoder wants (EXPAND.ASM). */
L7B37: /* _seg004_7B37 */
    /* 7B37  call    word ptr cs:uncmp_tab[bx] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(CODE004, BX + 0x6960)), 0x7B3C)) != 0) return c;
L7B3C:
    /* 7B3C  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* update_cache  (+7B3D): FM update_cache. It saves DS in BX and then reuses BX, so it
       returns with DS = 0C2EFh; cRender reloads DS afterwards.
       update_cache: called by cRender after each frame. Ages every loaded critter page (CmapCache
       entries 1 to 0FCh not 0FFh) by one, stopping at 1, so the least recently used page has the
       lowest count. */
L7B3D: /* _update_cache */
    /* 7B3D  mov     bx,ds */
    BX = asm_ds;
L7B3F:
    /* 7B3F  mov     ax,seg _CmapCache */
    AX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L7B42:
    /* 7B42  mov     ds,ax */
    SET_DS(AX);
L7B44:
    /* 7B44  mov     di,7Fh */
    DI = 0x7F;
L7B47:
    /* 7B47  mov     bx,0C2EFh */
    BX = 0xC2EF;
L7B4A: /* L7B4A */
    /* 7B4A  mov     al,byte ptr [bx+di] */
    AL = rb(pDS, BX + DI);
L7B4C:
    /* 7B4C  cmp     al,0FFh */
    sub8(AL, 0xFF, 0);
L7B4E:
    /* 7B4E  je      short L7B56 */
    if (ZF) goto L7B56;
L7B50:
    /* 7B50  dec     al */
    AL = dec8(AL);
L7B52:
    /* 7B52  je      short L7B56 */
    if (ZF) goto L7B56;
L7B54:
    /* 7B54  mov     byte ptr [bx+di],al */
    wb(pDS, BX + DI, AL);
L7B56: /* L7B56 */
    /* 7B56  dec     di */
    DI = dec16(DI);
L7B57:
    /* 7B57  jne     L7B4A */
    if (!ZF) goto L7B4A;
L7B59:
    /* 7B59  mov     ds,bx */
    SET_DS(BX);
L7B5B:
    /* 7B5B  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg004_7B5C  (+7B5C)
       unload_cpg: entered from load_cpg when no cache slot is free, BX = the cache index wanted.
       Finds the loaded page with the lowest age, takes its slot (marks it unloaded), gives the slot
       to BX and joins load_cpg to read the file. If nothing is loaded either, it drops the critter:
       discards load_cpg's return address and the pushed shade, skips the remaining operand and
       dispatches the next opcode. */
L7B5C: /* _unload_cpg */
    /* 7B5C  mov     di,7Fh */
    DI = 0x7F;
L7B5F:
    /* 7B5F  mov     dl,0FFh */
    DL = 0xFF;
L7B61:
    /* 7B61  mov     dh,0FFh */
    DH = 0xFF;
L7B63: /* L7B63 */
    /* 7B63  mov     al,byte ptr [di-3D11h] */
    AL = rb(pDS, DI + 0xC2EF);
L7B67:
    /* 7B67  cmp     al,dl */
    sub8(AL, DL, 0);
L7B69:
    /* 7B69  jae     short L7B71 */
    if (!CF) goto L7B71;
L7B6B:
    /* 7B6B  mov     dl,al */
    DL = AL;
L7B6D:
    /* 7B6D  mov     ax,di */
    AX = DI;
L7B6F:
    /* 7B6F  mov     dh,al */
    DH = AL;
L7B71: /* L7B71 */
    /* 7B71  dec     di */
    DI = dec16(DI);
L7B72:
    /* 7B72  jne     L7B63 */
    if (!ZF) goto L7B63;
L7B74:
    /* 7B74  cmp     dh,0FFh */
    sub8(DH, 0xFF, 0);
L7B77:
    /* 7B77  je      short L7BAC */
    if (ZF) goto L7BAC;
L7B79:
    /* 7B79  mov     di,0C20Fh */
    DI = 0xC20F;
L7B7C:
    /* 7B7C  mov     ax,bx */
    AX = BX;
L7B7E:
    /* 7B7E  xchg    dh,dl */
    { uint8_t t_ = DL;
    DL = DH;
    DH = t_; }
L7B80:
    /* 7B80  xor     dh,dh */
    DH = (uint8_t)(DH ^ DH);
L7B82:
    /* 7B82  mov     bx,dx */
    BX = DX;
L7B84:
    /* 7B84  mov     byte ptr [bx-3D11h],0FFh */
    wb(pDS, BX + 0xC2EF, 0xFF);
L7B89:
    /* 7B89  mov     cl,byte ptr [bx-3E71h] */
    CL = rb(pDS, BX + 0xC18F);
L7B8D:
    /* 7B8D  mov     byte ptr [bx-3E71h],0FFh */
    wb(pDS, BX + 0xC18F, 0xFF);
L7B92:
    /* 7B92  mov     bl,cl */
    BL = CL;
L7B94:
    /* 7B94  mov     byte ptr [bx-3DF1h],al */
    wb(pDS, BX + 0xC20F, AL);
L7B98:
    /* 7B98  mov     bx,ax */
    BX = AX;
L7B9A:
    /* 7B9A  mov     byte ptr [bx-3E71h],cl */
    wb(pDS, BX + 0xC18F, CL);
L7B9E:
    /* 7B9E  xor     ch,ch */
    CH = (uint8_t)(CH ^ CH);
L7BA0:
    /* 7BA0  mov     bx,ax */
    BX = AX;
L7BA2:
    /* 7BA2  mov     dx,ax */
    DX = AX;
L7BA4:
    /* 7BA4  and     ax,3 */
    AX = (uint16_t)(AX & 0x3);
L7BA7:
    /* 7BA7  shr     dx,2 */
    DX = (uint16_t)(DX >> 2);
L7BAA:
    /* 7BAA  jmp     short L7BF0 */
    goto L7BF0;
L7BAC: /* L7BAC */
    /* 7BAC  int     2 */
    asm_halt_at(0x06E7, 0x7BAC, "int 2h, the debugger break");
L7BAE: /* L7BAE */
    /* 7BAE  add     sp,4 */
    SP = (uint16_t)(SP + 0x4);
L7BB1:
    /* 7BB1  add     si,4 */
    SI = add16(SI, 0x4, 0);
L7BB4:
    /* 7BB4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7BB5:
    /* 7BB5  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L7BB6:
    /* 7BB6  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_7BBA  (+7BBA)
       UW1 only: load_cpg's entry that doubles DX first (do_uwcrit calls it). */
L7BBA: /* _seg004_7BBA */
    /* 7BBA  shl     dx,1 */
    DX = (uint16_t)(DX << 1);

    /* seg004_7BBC  (+7BBC)
       load_cpg: make critter page (DX = critter map, BX = page 0-7) present and mapped. If CmaptoPg
       has a slot for it, lcpg_pghere maps it. Otherwise it takes a free slot (PgtoCmap 0FEh, or
       unload_cpg's), builds the file name crit\crMM.0P from the map number in two octal digits and
       the page, opens it, maps the slot and reads up to 7FFFh bytes into the page frame, and sets
       its age to 10h. Out: the slot mapped into physical pages 0 and 1. If the file does not open or
       read, it breaks with int 2, frees the slot and skips the critter (dispatching the next
       opcode). */
L7BBC: /* _load_cpg */
    /* 7BBC  shl     dx,1 */
    DX = (uint16_t)(DX << 1);
L7BBE:
    /* 7BBE  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L7BC0:
    /* 7BC0  mov     al,byte ptr [bx-3E71h] */
    AL = rb(pDS, BX + 0xC18F);
L7BC4:
    /* 7BC4  cmp     al,0FFh */
    sub8(AL, 0xFF, 0);
L7BC6:
    /* 7BC6  je      short L7BCB */
    if (ZF) goto L7BCB;
L7BC8:
    /* 7BC8  jmp     _lcpg_pghere */
    goto L7C8B;
L7BCB: /* L7BCB */
    /* 7BCB  mov     di,0C20Fh */
    DI = 0xC20F;
L7BCE:
    /* 7BCE  mov     al,0FEh */
    AL = 0xFE;
L7BD0:
    /* 7BD0  mov     cx,ds */
    CX = asm_ds;
L7BD2:
    /* 7BD2  mov     es,cx */
    SET_ES(CX);
L7BD4:
    /* 7BD4  mov     cx,word ptr ds:[0C371h] */
    CX = rw(pDS, 0xC371);
L7BD8:
    /* 7BD8  repne scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (ZF) break; }
L7BDA:
    /* 7BDA  jne     _unload_cpg */
    if (!ZF) goto L7B5C;
L7BDC: /* foundfreepage */
    /* 7BDC  dec     di */
    DI = (uint16_t)(DI - 1);
L7BDD:
    /* 7BDD  mov     ax,bx */
    AX = BX;
L7BDF:
    /* 7BDF  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L7BE0:
    /* 7BE0  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L7BE2:
    /* 7BE2  shr     dx,2 */
    DX = (uint16_t)(DX >> 2);
L7BE5:
    /* 7BE5  neg     cx */
    CX = (uint16_t)-CX;
L7BE7:
    /* 7BE7  add     cx,word ptr ds:[0C371h] */
    CX = (uint16_t)(CX + rw(pDS, 0xC371));
L7BEB:
    /* 7BEB  dec     cx */
    CX = (uint16_t)(CX - 1);
L7BEC:
    /* 7BEC  mov     byte ptr [bx-3E71h],cl */
    wb(pDS, BX + 0xC18F, CL);
L7BF0: /* L7BF0 */
    /* 7BF0  mov     word ptr ds:[0B10Dh],bx */
    ww(pDS, 0xB10D, BX);
L7BF4:
    /* 7BF4  push    cx */
    push16(CX);
L7BF5:
    /* 7BF5  mov     ch,al */
    CH = AL;
L7BF7:
    /* 7BF7  mov     di,0B0FAh */
    DI = 0xB0FA;
L7BFA:
    /* 7BFA  add     di,7 */
    DI = add16(DI, 0x7, 0);
L7BFD:
    /* 7BFD  mov     ax,dx */
    AX = DX;
L7BFF:
    /* 7BFF  mov     cl,3 */
    CL = 0x3;
L7C01:
    /* 7C01  shr     ax,cl */
    AX = (uint16_t)(AX >> (CL & 31));
L7C03:
    /* 7C03  add     al,30h */
    AL = (uint8_t)(AL + 0x30);
L7C05:
    /* 7C05  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L7C06:
    /* 7C06  mov     ax,dx */
    AX = DX;
L7C08:
    /* 7C08  and     ax,7 */
    AX = (uint16_t)(AX & 0x7);
L7C0B:
    /* 7C0B  add     al,30h */
    AL = (uint8_t)(AL + 0x30);
L7C0D:
    /* 7C0D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L7C0E:
    /* 7C0E  add     di,6 */
    DI = add16(DI, 0x6, 0);
L7C11:
    /* 7C11  mov     al,ch */
    AL = CH;
L7C13:
    /* 7C13  shr     al,cl */
    AL = (uint8_t)(AL >> (CL & 31));
L7C15:
    /* 7C15  add     al,30h */
    AL = (uint8_t)(AL + 0x30);
L7C17:
    /* 7C17  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L7C18:
    /* 7C18  mov     al,ch */
    AL = CH;
L7C1A:
    /* 7C1A  and     al,7 */
    AL = (uint8_t)(AL & 0x7);
L7C1C:
    /* 7C1C  add     al,30h */
    AL = (uint8_t)(AL + 0x30);
L7C1E:
    /* 7C1E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L7C1F:
    /* 7C1F  mov     dx,0B0FAh */
    DX = 0xB0FA;
L7C22:
    /* 7C22  mov     ax,DOS_OPEN shl 8 */
    AX = 0x3D00;
L7C25:
    /* 7C25  int     21h */
    asm_int(0x21);
L7C27:
    /* 7C27  jb      short L7C5D */
    if (CF) goto L7C5D;
L7C29:
    /* 7C29  mov     dx,ax */
    DX = AX;
L7C2B:
    /* 7C2B  pop     ax */
    AX = pop16();
L7C2C:
    /* 7C2C  push    dx */
    push16(DX);
L7C2D:
    /* 7C2D  call    _lcpg_pghere */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x7C8B), 0x7C30)) != 0) return c;
L7C30:
    /* 7C30  pop     bx */
    BX = pop16();
L7C31:
    /* 7C31  mov     cx,7FFFh */
    CX = 0x7FFF;
L7C34:
    /* 7C34  mov     dx,0 */
    DX = 0x0;
L7C37:
    /* 7C37  mov     ax,word ptr ds:[0C4D5h] */
    AX = rw(pDS, 0xC4D5);
L7C3A:
    /* 7C3A  mov     ds,ax */
    SET_DS(AX);
L7C3C:
    /* 7C3C  mov     ax,DOS_READ shl 8 */
    AX = 0x3F00;
L7C3F:
    /* 7C3F  int     21h */
    asm_int(0x21);
L7C41:
    /* 7C41  lahf */
    AH = (uint8_t)(SF << 7 | ZF << 6 | 2 | CF);
L7C42:
    /* 7C42  mov     cl,ah */
    CL = AH;
L7C44:
    /* 7C44  mov     ax,DOS_CLOSE shl 8 */
    AX = 0x3E00;
L7C47:
    /* 7C47  int     21h */
    asm_int(0x21);
L7C49:
    /* 7C49  mov     ah,cl */
    AH = CL;
L7C4B:
    /* 7C4B  sahf */
    CF = AH & 1; ZF = AH >> 6 & 1; SF = AH >> 7 & 1;
L7C4C:
    /* 7C4C  mov     ax,seg seg051 */
    AX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L7C4F:
    /* 7C4F  mov     ds,ax */
    SET_DS(AX);
L7C51:
    /* 7C51  jb      short L7C62 */
    if (CF) goto L7C62;
L7C53:
    /* 7C53  mov     bx,word ptr ds:[0B10Dh] */
    BX = rw(pDS, 0xB10D);
L7C57:
    /* 7C57  mov     byte ptr [bx-3D11h],10h */
    wb(pDS, BX + 0xC2EF, 0x10);
L7C5C:
    /* 7C5C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L7C5D: /* L7C5D */
    /* 7C5D  int     2 */
    asm_halt_at(0x06E7, 0x7C5D, "int 2h, the debugger break");
L7C5F:
    /* 7C5F  add     sp,2 */
    SP = (uint16_t)(SP + 0x2);
L7C62: /* L7C62 */
    /* 7C62  int     2 */
    asm_halt_at(0x06E7, 0x7C62, "int 2h, the debugger break");
L7C64:
    /* 7C64  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L7C67:
    /* 7C67  add     sp,4 */
    SP = (uint16_t)(SP + 0x4);
L7C6A:
    /* 7C6A  mov     bx,word ptr ds:[0B10Dh] */
    BX = rw(pDS, 0xB10D);
L7C6E:
    /* 7C6E  xor     ch,ch */
    CH = logic8((uint8_t)(CH ^ CH));
L7C70:
    /* 7C70  mov     byte ptr [bx-3D11h],0FFh */
    wb(pDS, BX + 0xC2EF, 0xFF);
L7C75:
    /* 7C75  mov     cl,byte ptr [bx-3E71h] */
    CL = rb(pDS, BX + 0xC18F);
L7C79:
    /* 7C79  mov     byte ptr [bx-3E71h],0FFh */
    wb(pDS, BX + 0xC18F, 0xFF);
L7C7E:
    /* 7C7E  mov     bl,cl */
    BL = CL;
L7C80:
    /* 7C80  mov     byte ptr [bx-3DF1h],0FEh */
    wb(pDS, BX + 0xC20F, 0xFE);
L7C85:
    /* 7C85  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7C86:
    /* 7C86  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L7C87:
    /* 7C87  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_7C8B  (+7C8B)
       lcpg_pghere: AL = cache slot, BX = cache index. Adds 2 to the page's age (up to 0F0h) and,
       unless the slot is already mapped (crit_inpage), maps its two EMS pages (crit_fpage + slot * 2
       and the next) into physical pages 0 and 1 with int 67h function 50h. An EMS error breaks with
       int 2. */
L7C8B: /* _lcpg_pghere */
    /* 7C8B  mov     dl,byte ptr [bx-3D11h] */
    DL = rb(pDS, BX + 0xC2EF);
L7C8F:
    /* 7C8F  cmp     dl,0F0h */
    sub8(DL, 0xF0, 0);
L7C92:
    /* 7C92  jae     short L7C99 */
    if (!CF) goto L7C99;
L7C94:
    /* 7C94  add     byte ptr [bx-3D11h],2 */
    wb(pDS, BX + 0xC2EF, (uint8_t)(rb(pDS, BX + 0xC2EF) + 0x2));
L7C99: /* L7C99 */
    /* 7C99  cmp     al,byte ptr ds:[0C373h] */
    sub8(AL, rb(pDS, 0xC373), 0);
L7C9D:
    /* 7C9D  jne     short lcpg_swap */
    if (!ZF) goto L7CA0;
L7C9F:
    /* 7C9F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L7CA0: /* lcpg_swap */
    /* 7CA0  mov     byte ptr ds:[0C373h],al */
    wb(pDS, 0xC373, AL);
L7CA3:
    /* 7CA3  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L7CA5:
    /* 7CA5  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L7CA7:
    /* 7CA7  add     ax,word ptr ds:[0C36Fh] */
    AX = (uint16_t)(AX + rw(pDS, 0xC36F));
L7CAB:
    /* 7CAB  push    si */
    push16(SI);
L7CAC:
    /* 7CAC  mov     si,0B0F2h */
    SI = 0xB0F2;
L7CAF:
    /* 7CAF  mov     word ptr [si],ax */
    ww(pDS, SI, AX);
L7CB1:
    /* 7CB1  inc     ax */
    AX = (uint16_t)(AX + 1);
L7CB2:
    /* 7CB2  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L7CB5:
    /* 7CB5  mov     word ptr [si],ax */
    ww(pDS, SI, AX);
L7CB7:
    /* 7CB7  sub     si,4 */
    SI = (uint16_t)(SI - 0x4);
L7CBA:
    /* 7CBA  mov     cx,2 */
    CX = 0x2;
L7CBD:
    /* 7CBD  mov     ax,EMS_MAP_PAGES shl 8 */
    AX = 0x5000;
L7CC0:
    /* 7CC0  mov     dx,word ptr ds:[0C4D7h] */
    DX = rw(pDS, 0xC4D7);
L7CC4:
    /* 7CC4  int     67h */
    asm_int(0x67);
L7CC6:
    /* 7CC6  pop     si */
    SI = pop16();
L7CC7:
    /* 7CC7  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L7CC9:
    /* 7CC9  jne     short L7CCC */
    if (!ZF) goto L7CCC;
L7CCB:
    /* 7CCB  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L7CCC: /* L7CCC */
    /* 7CCC  int     2 */
    asm_halt_at(0x06E7, 0x7CCC, "int 2h, the debugger break");
L7CCE:
    /* 7CCE  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L7CCF: /* L7CCF */
    /* 7CCF  mov     ax,seg seg051 */
    AX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L7CD2:
    /* 7CD2  mov     ds,ax */
    SET_DS(AX);
L7CD4:
    /* 7CD4  add     si,2 */
    SI = add16(SI, 0x2, 0);
L7CD7:
    /* 7CD7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7CD8:
    /* 7CD8  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L7CD9:
    /* 7CD9  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_7CDD  (+7CDD)
       parse_cpg (FM name): decode and draw a frame from the mapped critter page, as UW2's does,
       but UW1's page layout differs: the frame is looked up through the page's tables, and a frame
       that is not there (0FFh) or a palette number not below the page's count skips the opcode
       (L7CCF, before this routine). In: BP = frame number, DL = the palette, DH = the shade. */
L7CDD: /* _parse_cpg */
    /* 7CDD  mov     ax,word ptr ds:[0C4D5h] */
    AX = rw(pDS, 0xC4D5);
L7CE0:
    /* 7CE0  mov     ds,ax */
    SET_DS(AX);
L7CE2:
    /* 7CE2  xor     di,di */
    DI = (uint16_t)(DI ^ DI);
L7CE4:
    /* 7CE4  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L7CE6:
    /* 7CE6  xor     ch,ch */
    CH = (uint8_t)(CH ^ CH);
L7CE8:
    /* 7CE8  mov     al,byte ptr [di] */
    AL = rb(pDS, DI);
L7CEA:
    /* 7CEA  sub     bp,ax */
    BP = (uint16_t)(BP - AX);
L7CEC:
    /* 7CEC  mov     al,byte ptr [di+1] */
    AL = rb(pDS, DI + 0x1);
L7CEF:
    /* 7CEF  mov     cl,byte ptr ds:[bp+di+2] */
    CL = rb(pDS, BP + DI + 0x2);
L7CF3:
    /* 7CF3  cmp     cl,0FFh */
    sub8(CL, 0xFF, 0);
L7CF6:
    /* 7CF6  je      L7CCF */
    if (ZF) goto L7CCF;
L7CF8:
    /* 7CF8  mov     bp,cx */
    BP = CX;
L7CFA:
    /* 7CFA  add     di,2 */
    DI = (uint16_t)(DI + 0x2);
L7CFD:
    /* 7CFD  add     di,ax */
    DI = add16(DI, AX, 0);
L7CFF:
    /* 7CFF  mov     al,byte ptr [di] */
    AL = rb(pDS, DI);
L7D01:
    /* 7D01  inc     di */
    DI = inc16(DI);
L7D02:
    /* 7D02  mov     cl,3 */
    CL = 0x3;
L7D04:
    /* 7D04  shl     bp,cl */
    BP = (uint16_t)(BP << (CL & 31));
L7D06:
    /* 7D06  add     bp,bx */
    BP = (uint16_t)(BP + BX);
L7D08:
    /* 7D08  mov     bl,byte ptr ds:[bp+di] */
    BL = rb(pDS, BP + DI);
L7D0B:
    /* 7D0B  mov     bh,0FFh */
    BH = 0xFF;
L7D0D:
    /* 7D0D  cmp     bl,bh */
    sub8(BL, BH, 0);
L7D0F:
    /* 7D0F  jne     short L7D13 */
    if (!ZF) goto L7D13;
L7D11:
    /* 7D11  mov     bl,0 */
    BL = 0x0;
L7D13: /* L7D13 */
    /* 7D13  xor     bh,bh */
    BH = logic8((uint8_t)(BH ^ BH));
L7D15:
    /* 7D15  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L7D17:
    /* 7D17  add     di,ax */
    DI = (uint16_t)(DI + AX);
L7D19:
    /* 7D19  mov     al,byte ptr [di] */
    AL = rb(pDS, DI);
L7D1B:
    /* 7D1B  inc     di */
    DI = (uint16_t)(DI + 1);
L7D1C:
    /* 7D1C  cmp     al,dl */
    sub8(AL, DL, 0);
L7D1E:
    /* 7D1E  jbe     L7CCF */
    if (CF || ZF) goto L7CCF;
L7D20:
    /* 7D20  inc     cl */
    CL = (uint8_t)(CL + 1);
L7D22:
    /* 7D22  inc     cl */
    CL = (uint8_t)(CL + 1);
L7D24:
    /* 7D24  xor     ah,ah */
    AH = logic8((uint8_t)(AH ^ AH));
L7D26:
    /* 7D26  xchg    dh,ah */
    { uint8_t t_ = AH;
    AH = DH;
    DH = t_; }
L7D28:
    /* 7D28  mov     bp,dx */
    BP = DX;
L7D2A:
    /* 7D2A  xchg    dh,ah */
    { uint8_t t_ = AH;
    AH = DH;
    DH = t_; }
L7D2C:
    /* 7D2C  shl     bp,cl */
    BP = (uint16_t)(BP << (CL & 31));
L7D2E:
    /* 7D2E  add     bp,di */
    BP = add16(BP, DI, 0);
L7D30:
    /* 7D30  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L7D32:
    /* 7D32  add     di,ax */
    DI = (uint16_t)(DI + AX);
L7D34:
    /* 7D34  inc     di */
    DI = (uint16_t)(DI + 1);
L7D35:
    /* 7D35  mov     al,byte ptr [di] */
    AL = rb(pDS, DI);
L7D37:
    /* 7D37  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L7D39:
    /* 7D39  inc     di */
    DI = (uint16_t)(DI + 1);
L7D3A:
    /* 7D3A  mov     cx,word ptr [bx+di] */
    CX = rw(pDS, BX + DI);
L7D3C:
    /* 7D3C  mov     bl,al */
    BL = AL;
L7D3E:
    /* 7D3E  xor     bh,bh */
    BH = (uint8_t)(BH ^ BH);
L7D40:
    /* 7D40  push    si */
    push16(SI);
L7D41:
    /* 7D41  push    ds */
    push16(asm_ds);
L7D42:
    /* 7D42  push    cx */
    push16(CX);
L7D43:
    /* 7D43  mov     si,bp */
    SI = BP;
L7D45:
    /* 7D45  mov     ax,ds */
    AX = asm_ds;
L7D47:
    /* 7D47  mov     bp,cx */
    BP = CX;
L7D49:
    /* 7D49  mov     bl,byte ptr ds:[bp+4] */
    BL = rb(pDS, BP + 0x4);
L7D4D:
    /* 7D4D  xor     bh,bh */
    BH = (uint8_t)(BH ^ BH);
L7D4F:
    /* 7D4F  add     bp,5 */
    BP = (uint16_t)(BP + 0x5);
L7D52:
    /* 7D52  call    word ptr cs:uncmp_tab[bx] */
    if ((c = asm_call(ASM_JMP(0x06E7, rw(CODE004, BX + 0x6960)), 0x7D57)) != 0) return c;
L7D57:
    /* 7D57  pop     si */
    SI = pop16();
L7D58:
    /* 7D58  pop     ds */
    SET_DS(pop16());
L7D59:
    /* 7D59  mov     di,0B0C4h */
    DI = 0xB0C4;
L7D5C:
    /* 7D5C  mov     bx,seg seg051 */
    BX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L7D5F:
    /* 7D5F  mov     es,bx */
    SET_ES(BX);
L7D61:
    /* 7D61  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L7D62:
    /* 7D62  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L7D63:
    /* 7D63  inc     di */
    DI = (uint16_t)(DI + 1);
L7D64:
    /* 7D64  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L7D65:
    /* 7D65  inc     di */
    DI = (uint16_t)(DI + 1);
L7D66:
    /* 7D66  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L7D67:
    /* 7D67  inc     di */
    DI = (uint16_t)(DI + 1);
L7D68:
    /* 7D68  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L7D69:
    /* 7D69  pop     si */
    SI = pop16();
L7D6A:
    /* 7D6A  mov     ax,seg seg051 */
    AX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L7D6D:
    /* 7D6D  mov     ds,ax */
    SET_DS(AX);
L7D6F:
    /* 7D6F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7D70:
    /* 7D70  mov     di,ax */
    DI = AX;
L7D72:
    /* 7D72  add     di,PNT_X */
    DI = add16(DI, 0x1620, 0);
L7D76:
    /* 7D76  mov     ax,0Eh */
    AX = 0xE;
L7D79:
    /* 7D79  db      0EAh */
    return ASM_JMP(0x06E7, 0x684B);

    /* seg004_7D7E  (+7D7E)
       do_setbmcol (opcode 0xAE): set bmcolor (seg048:DC3), the colour sprites are drawn in during
       a pick frame. Operand: the colour. */
L7D7E: /* _do_setbmcol */
    /* 7D7E  mov     bx,es */
    BX = asm_es;
L7D80:
    /* 7D80  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L7D83:
    /* 7D83  mov     es,ax */
    SET_ES(AX);
L7D85:
    /* 7D85  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7D86:
    /* 7D86  mov     byte ptr es:[0DC3h],al */
    wb(pES, 0xDC3, AL);
L7D8A:
    /* 7D8A  mov     es,bx */
    SET_ES(BX);
L7D8C:
    /* 7D8C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7D8D:
    /* 7D8D  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L7D8E:
    /* 7D8E  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_7D92  (+7D92)
       do_mouseq (opcode 0xB0): service the mouse during a frame. Calls seg019_102B
       (C3DENTRY.ASM), which calls MousQUp; FM Towns calls MousQUp(1) directly. GRIDDB.C emits it at
       the end of each grid row, so the cursor keeps moving while a frame renders. */
L7D92: /* _do_mouseq */
    /* 7D92  call    far ptr _seg019_102B */
    if ((c = asm_callf(ASM_JMP(0x1F3A, 0x102B), 0x06E7 + PORT_LOAD_SEG, 0x7D97)) != 0) return c;
L7D97:
    /* 7D97  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7D98:
    /* 7D98  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L7D99:
    /* 7D99  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x06E7, rw(pDS, BX + 0x2738));

    /* seg004_7D9D  (+7D9D)
       do_uwcolv: set the colour for the next flat polygon from a colour word and the distance shade.
       Operands: the address (in DS) of the colour, a shade word whose low byte is added to cdist
       (2934), capped at 15, to pick the lightabs row. In a pick frame (pickup set) the colour is
       bmcolor instead. Ends in INTERP.ASM's setcol_common. */
L7D9D: /* _do_uwcolv */
    /* 7D9D  test    word ptr ds:[2932h],0FFh */
    logic16((uint16_t)(rw(pDS, 0x2932) & 0xFF));
L7DA3:
    /* 7DA3  jne     short L7DC9 */
    if (!ZF) goto L7DC9;
L7DA5:
    /* 7DA5  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7DA6:
    /* 7DA6  mov     di,ax */
    DI = AX;
L7DA8:
    /* 7DA8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7DA9:
    /* 7DA9  mov     bx,ax */
    BX = AX;
L7DAB:
    /* 7DAB  mov     ax,word ptr [di] */
    AX = rw(pDS, DI);
L7DAD:
    /* 7DAD  xchg    bh,bl */
    { uint8_t t_ = BL;
    BL = BH;
    BH = t_; }
L7DAF:
    /* 7DAF  mov     cx,word ptr ds:[2934h] */
    CX = rw(pDS, 0x2934);
L7DB3:
    /* 7DB3  add     bh,cl */
    BH = (uint8_t)(BH + CL);
L7DB5:
    /* 7DB5  cmp     bh,0Fh */
    sub8(BH, 0xF, 0);
L7DB8:
    /* 7DB8  jbe     short L7DBC */
    if (CF || ZF) goto L7DBC;
L7DBA:
    /* 7DBA  mov     bh,0Fh */
    BH = 0xF;
L7DBC: /* L7DBC */
    /* 7DBC  add     bx,696Eh */
    BX = (uint16_t)(BX + 0x696E);
L7DC0:
    /* 7DC0  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L7DC2:
    /* 7DC2  mov     al,byte ptr cs:[bx] */
    AL = rb(CODE004, BX);
L7DC5:
    /* 7DC5  xor     ah,ah */
    AH = logic8((uint8_t)(AH ^ AH));
L7DC7:
    /* 7DC7  jmp     short L7DD9 */
    goto L7DD9;
L7DC9: /* L7DC9 */
    /* 7DC9  push    es */
    push16(asm_es);
L7DCA:
    /* 7DCA  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L7DCD:
    /* 7DCD  mov     es,ax */
    SET_ES(AX);
L7DCF:
    /* 7DCF  mov     al,byte ptr es:[0DC3h] */
    AL = rb(pES, 0xDC3);
L7DD3:
    /* 7DD3  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L7DD5:
    /* 7DD5  pop     es */
    SET_ES(pop16());
L7DD6:
    /* 7DD6  add     si,4 */
    SI = add16(SI, 0x4, 0);
L7DD9: /* L7DD9 */
    /* 7DD9  jmp     setcol_common */
    return ASM_JMP(0x06E7, 0x3733);
}
