/* c3dentry_x.c: replaces src/sys/C3DENTRY.ASM (seg019_C20, 0C20..1071 of its
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

uint32_t asm_mod_C3DENTRY(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x0C34: goto L0C34;
    case 0x0C35: goto L0C35;
    case 0x0C37: goto L0C37;
    case 0x0C38: goto L0C38;
    case 0x0C39: goto L0C39;
    case 0x0C3A: goto L0C3A;
    case 0x0C3B: goto L0C3B;
    case 0x0C3D: goto L0C3D;
    case 0x0C42: goto L0C42;
    case 0x0C44: goto L0C44;
    case 0x0C49: goto L0C49;
    case 0x0C4C: goto L0C4C;
    case 0x0C4F: goto L0C4F;
    case 0x0C51: goto L0C51;
    case 0x0C53: goto L0C53;
    case 0x0C54: goto L0C54;
    case 0x0C56: goto L0C56;
    case 0x0C59: goto L0C59;
    case 0x0C5A: goto L0C5A;
    case 0x0C5D: goto L0C5D;
    case 0x0C5F: goto L0C5F;
    case 0x0C64: goto L0C64;
    case 0x0C67: goto L0C67;
    case 0x0C68: goto L0C68;
    case 0x0C69: goto L0C69;
    case 0x0C6E: goto L0C6E;
    case 0x0C70: goto L0C70;
    case 0x0C75: goto L0C75;
    case 0x0C77: goto L0C77;
    case 0x0C78: goto L0C78;
    case 0x0C79: goto L0C79;
    case 0x0C7A: goto L0C7A;
    case 0x0C7B: goto L0C7B;
    case 0x0C7C: goto L0C7C;
    case 0x0C7D: goto L0C7D;
    case 0x0C7E: goto L0C7E;
    case 0x0C83: goto L0C83;
    case 0x0C84: goto L0C84;
    case 0x0C85: goto L0C85;
    case 0x0C87: goto L0C87;
    case 0x0C88: goto L0C88;
    case 0x0C89: goto L0C89;
    case 0x0C8A: goto L0C8A;
    case 0x0C8B: goto L0C8B;
    case 0x0C8D: goto L0C8D;
    case 0x0C92: goto L0C92;
    case 0x0C94: goto L0C94;
    case 0x0C99: goto L0C99;
    case 0x0C9C: goto L0C9C;
    case 0x0CA1: goto L0CA1;
    case 0x0CA2: goto L0CA2;
    case 0x0CA7: goto L0CA7;
    case 0x0CA9: goto L0CA9;
    case 0x0CAE: goto L0CAE;
    case 0x0CB0: goto L0CB0;
    case 0x0CB1: goto L0CB1;
    case 0x0CB2: goto L0CB2;
    case 0x0CB3: goto L0CB3;
    case 0x0CB4: goto L0CB4;
    case 0x0CB5: goto L0CB5;
    case 0x0CB6: goto L0CB6;
    case 0x0CB7: goto L0CB7;
    case 0x0CB8: goto L0CB8;
    case 0x0CBA: goto L0CBA;
    case 0x0CBB: goto L0CBB;
    case 0x0CBC: goto L0CBC;
    case 0x0CBD: goto L0CBD;
    case 0x0CBE: goto L0CBE;
    case 0x0CC0: goto L0CC0;
    case 0x0CC5: goto L0CC5;
    case 0x0CC7: goto L0CC7;
    case 0x0CCC: goto L0CCC;
    case 0x0CCF: goto L0CCF;
    case 0x0CD4: goto L0CD4;
    case 0x0CD5: goto L0CD5;
    case 0x0CDA: goto L0CDA;
    case 0x0CDC: goto L0CDC;
    case 0x0CE1: goto L0CE1;
    case 0x0CE3: goto L0CE3;
    case 0x0CE4: goto L0CE4;
    case 0x0CE5: goto L0CE5;
    case 0x0CE6: goto L0CE6;
    case 0x0CE7: goto L0CE7;
    case 0x0CE8: goto L0CE8;
    case 0x0CE9: goto L0CE9;
    case 0x0CEA: goto L0CEA;
    case 0x0CEB: goto L0CEB;
    case 0x0CED: goto L0CED;
    case 0x0CEE: goto L0CEE;
    case 0x0CEF: goto L0CEF;
    case 0x0CF0: goto L0CF0;
    case 0x0CF1: goto L0CF1;
    case 0x0CF3: goto L0CF3;
    case 0x0CF8: goto L0CF8;
    case 0x0CFA: goto L0CFA;
    case 0x0CFF: goto L0CFF;
    case 0x0D02: goto L0D02;
    case 0x0D07: goto L0D07;
    case 0x0D08: goto L0D08;
    case 0x0D0D: goto L0D0D;
    case 0x0D0F: goto L0D0F;
    case 0x0D14: goto L0D14;
    case 0x0D16: goto L0D16;
    case 0x0D17: goto L0D17;
    case 0x0D18: goto L0D18;
    case 0x0D19: goto L0D19;
    case 0x0D1A: goto L0D1A;
    case 0x0D1B: goto L0D1B;
    case 0x0D1C: goto L0D1C;
    case 0x0D1D: goto L0D1D;
    case 0x0D1E: goto L0D1E;
    case 0x0D20: goto L0D20;
    case 0x0D21: goto L0D21;
    case 0x0D22: goto L0D22;
    case 0x0D23: goto L0D23;
    case 0x0D24: goto L0D24;
    case 0x0D26: goto L0D26;
    case 0x0D2B: goto L0D2B;
    case 0x0D2D: goto L0D2D;
    case 0x0D32: goto L0D32;
    case 0x0D35: goto L0D35;
    case 0x0D38: goto L0D38;
    case 0x0D3B: goto L0D3B;
    case 0x0D3D: goto L0D3D;
    case 0x0D3F: goto L0D3F;
    case 0x0D41: goto L0D41;
    case 0x0D43: goto L0D43;
    case 0x0D45: goto L0D45;
    case 0x0D47: goto L0D47;
    case 0x0D4A: goto L0D4A;
    case 0x0D4D: goto L0D4D;
    case 0x0D50: goto L0D50;
    case 0x0D52: goto L0D52;
    case 0x0D54: goto L0D54;
    case 0x0D55: goto L0D55;
    case 0x0D57: goto L0D57;
    case 0x0D5B: goto L0D5B;
    case 0x0D5C: goto L0D5C;
    case 0x0D5F: goto L0D5F;
    case 0x0D64: goto L0D64;
    case 0x0D65: goto L0D65;
    case 0x0D6A: goto L0D6A;
    case 0x0D6C: goto L0D6C;
    case 0x0D71: goto L0D71;
    case 0x0D73: goto L0D73;
    case 0x0D74: goto L0D74;
    case 0x0D75: goto L0D75;
    case 0x0D76: goto L0D76;
    case 0x0D77: goto L0D77;
    case 0x0D78: goto L0D78;
    case 0x0D79: goto L0D79;
    case 0x0D7A: goto L0D7A;
    case 0x0D7B: goto L0D7B;
    case 0x0D7D: goto L0D7D;
    case 0x0D7E: goto L0D7E;
    case 0x0D7F: goto L0D7F;
    case 0x0D80: goto L0D80;
    case 0x0D81: goto L0D81;
    case 0x0D83: goto L0D83;
    case 0x0D88: goto L0D88;
    case 0x0D8A: goto L0D8A;
    case 0x0D8F: goto L0D8F;
    case 0x0D92: goto L0D92;
    case 0x0D94: goto L0D94;
    case 0x0D96: goto L0D96;
    case 0x0D97: goto L0D97;
    case 0x0D99: goto L0D99;
    case 0x0D9C: goto L0D9C;
    case 0x0D9D: goto L0D9D;
    case 0x0DA1: goto L0DA1;
    case 0x0DA5: goto L0DA5;
    case 0x0DA9: goto L0DA9;
    case 0x0DAD: goto L0DAD;
    case 0x0DB0: goto L0DB0;
    case 0x0DB2: goto L0DB2;
    case 0x0DB5: goto L0DB5;
    case 0x0DB9: goto L0DB9;
    case 0x0DBD: goto L0DBD;
    case 0x0DC1: goto L0DC1;
    case 0x0DC4: goto L0DC4;
    case 0x0DC7: goto L0DC7;
    case 0x0DC9: goto L0DC9;
    case 0x0DCE: goto L0DCE;
    case 0x0DD3: goto L0DD3;
    case 0x0DD8: goto L0DD8;
    case 0x0DDD: goto L0DDD;
    case 0x0DDE: goto L0DDE;
    case 0x0DE3: goto L0DE3;
    case 0x0DE5: goto L0DE5;
    case 0x0DEA: goto L0DEA;
    case 0x0DEC: goto L0DEC;
    case 0x0DED: goto L0DED;
    case 0x0DEE: goto L0DEE;
    case 0x0DEF: goto L0DEF;
    case 0x0DF0: goto L0DF0;
    case 0x0DF1: goto L0DF1;
    case 0x0DF4: goto L0DF4;
    case 0x0DF6: goto L0DF6;
    case 0x0DFA: goto L0DFA;
    case 0x0DFD: goto L0DFD;
    case 0x0E01: goto L0E01;
    case 0x0E05: goto L0E05;
    case 0x0E09: goto L0E09;
    case 0x0E0E: goto L0E0E;
    case 0x0E13: goto L0E13;
    case 0x0E18: goto L0E18;
    case 0x0E19: goto L0E19;
    case 0x0E1A: goto L0E1A;
    case 0x0E1B: goto L0E1B;
    case 0x0E1D: goto L0E1D;
    case 0x0E1E: goto L0E1E;
    case 0x0E1F: goto L0E1F;
    case 0x0E20: goto L0E20;
    case 0x0E21: goto L0E21;
    case 0x0E23: goto L0E23;
    case 0x0E28: goto L0E28;
    case 0x0E2A: goto L0E2A;
    case 0x0E2F: goto L0E2F;
    case 0x0E32: goto L0E32;
    case 0x0E34: goto L0E34;
    case 0x0E36: goto L0E36;
    case 0x0E37: goto L0E37;
    case 0x0E39: goto L0E39;
    case 0x0E3C: goto L0E3C;
    case 0x0E3D: goto L0E3D;
    case 0x0E44: goto L0E44;
    case 0x0E48: goto L0E48;
    case 0x0E4D: goto L0E4D;
    case 0x0E4E: goto L0E4E;
    case 0x0E53: goto L0E53;
    case 0x0E55: goto L0E55;
    case 0x0E5A: goto L0E5A;
    case 0x0E5C: goto L0E5C;
    case 0x0E5D: goto L0E5D;
    case 0x0E5E: goto L0E5E;
    case 0x0E5F: goto L0E5F;
    case 0x0E60: goto L0E60;
    case 0x0E61: goto L0E61;
    case 0x0E62: goto L0E62;
    case 0x0E63: goto L0E63;
    case 0x0E64: goto L0E64;
    case 0x0E66: goto L0E66;
    case 0x0E67: goto L0E67;
    case 0x0E68: goto L0E68;
    case 0x0E69: goto L0E69;
    case 0x0E6A: goto L0E6A;
    case 0x0E6C: goto L0E6C;
    case 0x0E71: goto L0E71;
    case 0x0E73: goto L0E73;
    case 0x0E78: goto L0E78;
    case 0x0E7B: goto L0E7B;
    case 0x0E7E: goto L0E7E;
    case 0x0E80: goto L0E80;
    case 0x0E82: goto L0E82;
    case 0x0E83: goto L0E83;
    case 0x0E85: goto L0E85;
    case 0x0E88: goto L0E88;
    case 0x0E89: goto L0E89;
    case 0x0E8C: goto L0E8C;
    case 0x0E8D: goto L0E8D;
    case 0x0E92: goto L0E92;
    case 0x0E94: goto L0E94;
    case 0x0E99: goto L0E99;
    case 0x0E9B: goto L0E9B;
    case 0x0E9C: goto L0E9C;
    case 0x0E9D: goto L0E9D;
    case 0x0E9E: goto L0E9E;
    case 0x0E9F: goto L0E9F;
    case 0x0EA0: goto L0EA0;
    case 0x0EA1: goto L0EA1;
    case 0x0EA4: goto L0EA4;
    case 0x0EA6: goto L0EA6;
    case 0x0EA9: goto L0EA9;
    case 0x0EAB: goto L0EAB;
    case 0x0EAC: goto L0EAC;
    case 0x0EAD: goto L0EAD;
    case 0x0EAE: goto L0EAE;
    case 0x0EAF: goto L0EAF;
    case 0x0EB1: goto L0EB1;
    case 0x0EB2: goto L0EB2;
    case 0x0EB3: goto L0EB3;
    case 0x0EB4: goto L0EB4;
    case 0x0EB5: goto L0EB5;
    case 0x0EB7: goto L0EB7;
    case 0x0EBC: goto L0EBC;
    case 0x0EBE: goto L0EBE;
    case 0x0EC3: goto L0EC3;
    case 0x0EC6: goto L0EC6;
    case 0x0EC9: goto L0EC9;
    case 0x0ECB: goto L0ECB;
    case 0x0ECD: goto L0ECD;
    case 0x0ECE: goto L0ECE;
    case 0x0ED0: goto L0ED0;
    case 0x0ED3: goto L0ED3;
    case 0x0ED4: goto L0ED4;
    case 0x0ED5: goto L0ED5;
    case 0x0ED8: goto L0ED8;
    case 0x0ED9: goto L0ED9;
    case 0x0EDA: goto L0EDA;
    case 0x0EDF: goto L0EDF;
    case 0x0EE1: goto L0EE1;
    case 0x0EE6: goto L0EE6;
    case 0x0EE8: goto L0EE8;
    case 0x0EE9: goto L0EE9;
    case 0x0EEA: goto L0EEA;
    case 0x0EEB: goto L0EEB;
    case 0x0EEC: goto L0EEC;
    case 0x0EED: goto L0EED;
    case 0x0EEE: goto L0EEE;
    case 0x0EF1: goto L0EF1;
    case 0x0EF3: goto L0EF3;
    case 0x0EF6: goto L0EF6;
    case 0x0EF8: goto L0EF8;
    case 0x0EF9: goto L0EF9;
    case 0x0EFA: goto L0EFA;
    case 0x0EFB: goto L0EFB;
    case 0x0EFC: goto L0EFC;
    case 0x0EFE: goto L0EFE;
    case 0x0EFF: goto L0EFF;
    case 0x0F00: goto L0F00;
    case 0x0F01: goto L0F01;
    case 0x0F02: goto L0F02;
    case 0x0F04: goto L0F04;
    case 0x0F09: goto L0F09;
    case 0x0F0B: goto L0F0B;
    case 0x0F10: goto L0F10;
    case 0x0F13: goto L0F13;
    case 0x0F16: goto L0F16;
    case 0x0F19: goto L0F19;
    case 0x0F1B: goto L0F1B;
    case 0x0F1D: goto L0F1D;
    case 0x0F1E: goto L0F1E;
    case 0x0F20: goto L0F20;
    case 0x0F23: goto L0F23;
    case 0x0F24: goto L0F24;
    case 0x0F27: goto L0F27;
    case 0x0F29: goto L0F29;
    case 0x0F2A: goto L0F2A;
    case 0x0F2F: goto L0F2F;
    case 0x0F31: goto L0F31;
    case 0x0F36: goto L0F36;
    case 0x0F38: goto L0F38;
    case 0x0F39: goto L0F39;
    case 0x0F3A: goto L0F3A;
    case 0x0F3B: goto L0F3B;
    case 0x0F3C: goto L0F3C;
    case 0x0F3D: goto L0F3D;
    case 0x0F3E: goto L0F3E;
    case 0x0F3F: goto L0F3F;
    case 0x0F40: goto L0F40;
    case 0x0F42: goto L0F42;
    case 0x0F43: goto L0F43;
    case 0x0F44: goto L0F44;
    case 0x0F45: goto L0F45;
    case 0x0F46: goto L0F46;
    case 0x0F48: goto L0F48;
    case 0x0F4D: goto L0F4D;
    case 0x0F4F: goto L0F4F;
    case 0x0F54: goto L0F54;
    case 0x0F57: goto L0F57;
    case 0x0F5A: goto L0F5A;
    case 0x0F5D: goto L0F5D;
    case 0x0F5F: goto L0F5F;
    case 0x0F61: goto L0F61;
    case 0x0F62: goto L0F62;
    case 0x0F64: goto L0F64;
    case 0x0F67: goto L0F67;
    case 0x0F68: goto L0F68;
    case 0x0F6B: goto L0F6B;
    case 0x0F6D: goto L0F6D;
    case 0x0F6E: goto L0F6E;
    case 0x0F73: goto L0F73;
    case 0x0F75: goto L0F75;
    case 0x0F7A: goto L0F7A;
    case 0x0F7C: goto L0F7C;
    case 0x0F7D: goto L0F7D;
    case 0x0F7E: goto L0F7E;
    case 0x0F7F: goto L0F7F;
    case 0x0F80: goto L0F80;
    case 0x0F81: goto L0F81;
    case 0x0F82: goto L0F82;
    case 0x0F83: goto L0F83;
    case 0x0F84: goto L0F84;
    case 0x0F86: goto L0F86;
    case 0x0F87: goto L0F87;
    case 0x0F88: goto L0F88;
    case 0x0F89: goto L0F89;
    case 0x0F8A: goto L0F8A;
    case 0x0F8C: goto L0F8C;
    case 0x0F91: goto L0F91;
    case 0x0F93: goto L0F93;
    case 0x0F98: goto L0F98;
    case 0x0F9B: goto L0F9B;
    case 0x0F9E: goto L0F9E;
    case 0x0FA1: goto L0FA1;
    case 0x0FA4: goto L0FA4;
    case 0x0FA7: goto L0FA7;
    case 0x0FA9: goto L0FA9;
    case 0x0FAB: goto L0FAB;
    case 0x0FAE: goto L0FAE;
    case 0x0FB0: goto L0FB0;
    case 0x0FB2: goto L0FB2;
    case 0x0FB3: goto L0FB3;
    case 0x0FB5: goto L0FB5;
    case 0x0FB8: goto L0FB8;
    case 0x0FB9: goto L0FB9;
    case 0x0FBB: goto L0FBB;
    case 0x0FBD: goto L0FBD;
    case 0x0FC2: goto L0FC2;
    case 0x0FC3: goto L0FC3;
    case 0x0FC8: goto L0FC8;
    case 0x0FCA: goto L0FCA;
    case 0x0FCF: goto L0FCF;
    case 0x0FD1: goto L0FD1;
    case 0x0FD2: goto L0FD2;
    case 0x0FD3: goto L0FD3;
    case 0x0FD4: goto L0FD4;
    case 0x0FD5: goto L0FD5;
    case 0x0FD6: goto L0FD6;
    case 0x0FD8: goto L0FD8;
    case 0x0FDB: goto L0FDB;
    case 0x0FDC: goto L0FDC;
    case 0x0FDD: goto L0FDD;
    case 0x0FDF: goto L0FDF;
    case 0x0FE1: goto L0FE1;
    case 0x0FE4: goto L0FE4;
    case 0x0FE6: goto L0FE6;
    case 0x0FE8: goto L0FE8;
    case 0x0FEA: goto L0FEA;
    case 0x0FEC: goto L0FEC;
    case 0x0FEF: goto L0FEF;
    case 0x0FF0: goto L0FF0;
    case 0x0FF2: goto L0FF2;
    case 0x0FF3: goto L0FF3;
    case 0x0FF7: goto L0FF7;
    case 0x0FFB: goto L0FFB;
    case 0x0FFF: goto L0FFF;
    case 0x1003: goto L1003;
    case 0x1004: goto L1004;
    case 0x1006: goto L1006;
    case 0x1008: goto L1008;
    case 0x100B: goto L100B;
    case 0x100E: goto L100E;
    case 0x1010: goto L1010;
    case 0x1012: goto L1012;
    case 0x1014: goto L1014;
    case 0x1016: goto L1016;
    case 0x1017: goto L1017;
    case 0x1019: goto L1019;
    case 0x101A: goto L101A;
    case 0x101E: goto L101E;
    case 0x1022: goto L1022;
    case 0x1026: goto L1026;
    case 0x102A: goto L102A;
    default: asm_bad_entry("C3DENTRY.ASM", entry);
    }

    /* seg019_C34  (+C34)
       cZoom(zoom): store the zoom word in the render database's header (the word after the address
       at seg051:0104; UW2 0158). */
L0C34: /* _cZoom */
    /* 0C34  push    bp */
    push16(BP);
L0C35:
    /* 0C35  mov     bp,sp */
    BP = SP;
L0C37:
    /* 0C37  push    di */
    push16(DI);
L0C38:
    /* 0C38  push    si */
    push16(SI);
L0C39:
    /* 0C39  push    ds */
    push16(asm_ds);
L0C3A:
    /* 0C3A  push    es */
    push16(asm_es);
L0C3B:
    /* 0C3B  mov     dx,ss */
    DX = asm_ss;
L0C3D:
    /* 0C3D  mov     word ptr cs:L0C24,dx */
    ww(CODE019, 0xC24, DX);
L0C42:
    /* 0C42  mov     dx,sp */
    DX = SP;
L0C44:
    /* 0C44  mov     word ptr cs:L0C26,dx */
    ww(CODE019, 0xC26, DX);
L0C49:
    /* 0C49  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L0C4C:
    /* 0C4C  mov     dx,seg seg063 */
    DX = (uint16_t)(0x5624 + PORT_LOAD_SEG);
L0C4F:
    /* 0C4F  mov     ds,dx */
    SET_DS(DX);
L0C51:
    /* 0C51  mov     es,dx */
    SET_ES(DX);
L0C53:
    /* 0C53  cli */
    ;
L0C54:
    /* 0C54  mov     ss,dx */
    SET_SS(DX);
L0C56:
    /* 0C56  mov     sp,448h */
    SP = 0x448;
L0C59:
    /* 0C59  sti */
    ;
L0C5A:
    /* 0C5A  mov     bx,seg seg051 */
    BX = (uint16_t)(0x4723 + PORT_LOAD_SEG);
L0C5D:
    /* 0C5D  mov     es,bx */
    SET_ES(BX);
L0C5F:
    /* 0C5F  mov     di,word ptr es:[104h] */
    DI = rw(pES, 0x104);
L0C64:
    /* 0C64  add     di,2 */
    DI = add16(DI, 0x2, 0);
L0C67:
    /* 0C67  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L0C68:
    /* 0C68  cli */
    ;
L0C69:
    /* 0C69  mov     dx,word ptr cs:L0C24 */
    DX = rw(CODE019, 0xC24);
L0C6E:
    /* 0C6E  mov     ss,dx */
    SET_SS(DX);
L0C70:
    /* 0C70  mov     dx,word ptr cs:L0C26 */
    DX = rw(CODE019, 0xC26);
L0C75:
    /* 0C75  mov     sp,dx */
    SP = DX;
L0C77:
    /* 0C77  sti */
    ;
L0C78:
    /* 0C78  pop     es */
    SET_ES(pop16());
L0C79:
    /* 0C79  pop     ds */
    SET_DS(pop16());
L0C7A:
    /* 0C7A  pop     si */
    SI = pop16();
L0C7B:
    /* 0C7B  pop     di */
    DI = pop16();
L0C7C:
    /* 0C7C  pop     bp */
    BP = pop16();
L0C7D:
    /* 0C7D  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg019_C7E  (+C7E)
       cFBtoScreen: copy the 3D frame buffer to the screen (GRENTRY.ASM's _7F9, which switches stacks
       itself). */
L0C7E: /* _cFBtoScreen */
    /* 0C7E  call    far ptr _seg003_B5D */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x0B5D), 0x1F3A + PORT_LOAD_SEG, 0x0C83)) != 0) return c;
L0C83:
    /* 0C83  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg019_C84  (+C84)
       cFillFB(colour): fill the frame buffer (GRENTRY.ASM's fill_fbuf). */
L0C84: /* _cFillFB */
    /* 0C84  push    bp */
    push16(BP);
L0C85:
    /* 0C85  mov     bp,sp */
    BP = SP;
L0C87:
    /* 0C87  push    di */
    push16(DI);
L0C88:
    /* 0C88  push    si */
    push16(SI);
L0C89:
    /* 0C89  push    ds */
    push16(asm_ds);
L0C8A:
    /* 0C8A  push    es */
    push16(asm_es);
L0C8B:
    /* 0C8B  mov     dx,ss */
    DX = asm_ss;
L0C8D:
    /* 0C8D  mov     word ptr cs:L0C24,dx */
    ww(CODE019, 0xC24, DX);
L0C92:
    /* 0C92  mov     dx,sp */
    DX = SP;
L0C94:
    /* 0C94  mov     word ptr cs:L0C26,dx */
    ww(CODE019, 0xC26, DX);
L0C99:
    /* 0C99  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L0C9C:
    /* 0C9C  call    far ptr _seg003_A48 */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x0A48), 0x1F3A + PORT_LOAD_SEG, 0x0CA1)) != 0) return c;
L0CA1:
    /* 0CA1  cli */
    ;
L0CA2:
    /* 0CA2  mov     dx,word ptr cs:L0C24 */
    DX = rw(CODE019, 0xC24);
L0CA7:
    /* 0CA7  mov     ss,dx */
    SET_SS(DX);
L0CA9:
    /* 0CA9  mov     dx,word ptr cs:L0C26 */
    DX = rw(CODE019, 0xC26);
L0CAE:
    /* 0CAE  mov     sp,dx */
    SP = DX;
L0CB0:
    /* 0CB0  sti */
    ;
L0CB1:
    /* 0CB1  pop     es */
    SET_ES(pop16());
L0CB2:
    /* 0CB2  pop     ds */
    SET_DS(pop16());
L0CB3:
    /* 0CB3  pop     si */
    SI = pop16();
L0CB4:
    /* 0CB4  pop     di */
    DI = pop16();
L0CB5:
    /* 0CB5  pop     bp */
    BP = pop16();
L0CB6:
    /* 0CB6  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg019_CB7  (+CB7)
       cDimFB(shade) (FM Towns; the label is an old guess): darken the frame buffer through a
       lightabs row (GRENTRY.ASM's _764). name: cDimFB is FM Towns' name for the routine at this
       place; the proc keeps the label it was matched under. */
L0CB7: /* _seg019_CB7 */
    /* 0CB7  push    bp */
    push16(BP);
L0CB8:
    /* 0CB8  mov     bp,sp */
    BP = SP;
L0CBA:
    /* 0CBA  push    di */
    push16(DI);
L0CBB:
    /* 0CBB  push    si */
    push16(SI);
L0CBC:
    /* 0CBC  push    ds */
    push16(asm_ds);
L0CBD:
    /* 0CBD  push    es */
    push16(asm_es);
L0CBE:
    /* 0CBE  mov     dx,ss */
    DX = asm_ss;
L0CC0:
    /* 0CC0  mov     word ptr cs:L0C24,dx */
    ww(CODE019, 0xC24, DX);
L0CC5:
    /* 0CC5  mov     dx,sp */
    DX = SP;
L0CC7:
    /* 0CC7  mov     word ptr cs:L0C26,dx */
    ww(CODE019, 0xC26, DX);
L0CCC:
    /* 0CCC  mov     cx,word ptr [bp+6] */
    CX = rw(pSS, BP + 0x6);
L0CCF:
    /* 0CCF  call    far ptr _seg003_AC8 */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x0AC8), 0x1F3A + PORT_LOAD_SEG, 0x0CD4)) != 0) return c;
L0CD4:
    /* 0CD4  cli */
    ;
L0CD5:
    /* 0CD5  mov     dx,word ptr cs:L0C24 */
    DX = rw(CODE019, 0xC24);
L0CDA:
    /* 0CDA  mov     ss,dx */
    SET_SS(DX);
L0CDC:
    /* 0CDC  mov     dx,word ptr cs:L0C26 */
    DX = rw(CODE019, 0xC26);
L0CE1:
    /* 0CE1  mov     sp,dx */
    SP = DX;
L0CE3:
    /* 0CE3  sti */
    ;
L0CE4:
    /* 0CE4  pop     es */
    SET_ES(pop16());
L0CE5:
    /* 0CE5  pop     ds */
    SET_DS(pop16());
L0CE6:
    /* 0CE6  pop     si */
    SI = pop16();
L0CE7:
    /* 0CE7  pop     di */
    DI = pop16();
L0CE8:
    /* 0CE8  pop     bp */
    BP = pop16();
L0CE9:
    /* 0CE9  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg019_CEA  (+CEA)
       cLiteFB(count) (FM Towns): remap the frame buffer through cXfer table 3, count times
       (GRENTRY.ASM's _788). name: cLiteFB is FM Towns' name; the proc keeps the label it was matched
       under. */
L0CEA: /* _seg019_CEA */
    /* 0CEA  push    bp */
    push16(BP);
L0CEB:
    /* 0CEB  mov     bp,sp */
    BP = SP;
L0CED:
    /* 0CED  push    di */
    push16(DI);
L0CEE:
    /* 0CEE  push    si */
    push16(SI);
L0CEF:
    /* 0CEF  push    ds */
    push16(asm_ds);
L0CF0:
    /* 0CF0  push    es */
    push16(asm_es);
L0CF1:
    /* 0CF1  mov     dx,ss */
    DX = asm_ss;
L0CF3:
    /* 0CF3  mov     word ptr cs:L0C24,dx */
    ww(CODE019, 0xC24, DX);
L0CF8:
    /* 0CF8  mov     dx,sp */
    DX = SP;
L0CFA:
    /* 0CFA  mov     word ptr cs:L0C26,dx */
    ww(CODE019, 0xC26, DX);
L0CFF:
    /* 0CFF  mov     cx,word ptr [bp+6] */
    CX = rw(pSS, BP + 0x6);
L0D02:
    /* 0D02  call    far ptr _seg003_AEC */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x0AEC), 0x1F3A + PORT_LOAD_SEG, 0x0D07)) != 0) return c;
L0D07:
    /* 0D07  cli */
    ;
L0D08:
    /* 0D08  mov     dx,word ptr cs:L0C24 */
    DX = rw(CODE019, 0xC24);
L0D0D:
    /* 0D0D  mov     ss,dx */
    SET_SS(DX);
L0D0F:
    /* 0D0F  mov     dx,word ptr cs:L0C26 */
    DX = rw(CODE019, 0xC26);
L0D14:
    /* 0D14  mov     sp,dx */
    SP = DX;
L0D16:
    /* 0D16  sti */
    ;
L0D17:
    /* 0D17  pop     es */
    SET_ES(pop16());
L0D18:
    /* 0D18  pop     ds */
    SET_DS(pop16());
L0D19:
    /* 0D19  pop     si */
    SI = pop16();
L0D1A:
    /* 0D1A  pop     di */
    DI = pop16();
L0D1B:
    /* 0D1B  pop     bp */
    BP = pop16();
L0D1C:
    /* 0D1C  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg019_D1D  (+D1D)
       cPlaceFB(x, y, width, height): put the 3D view's frame at (x, y) on the screen, y counting up:
       the screen offset (0C7h - y) * 80 + x / 4 goes to seg048:0950 (UW2 seg_370D:094E), then GRENTRY.ASM's
       setup_frame_buf(width, height). */
L0D1D: /* _cPlaceFB */
    /* 0D1D  push    bp */
    push16(BP);
L0D1E:
    /* 0D1E  mov     bp,sp */
    BP = SP;
L0D20:
    /* 0D20  push    di */
    push16(DI);
L0D21:
    /* 0D21  push    si */
    push16(SI);
L0D22:
    /* 0D22  push    ds */
    push16(asm_ds);
L0D23:
    /* 0D23  push    es */
    push16(asm_es);
L0D24:
    /* 0D24  mov     dx,ss */
    DX = asm_ss;
L0D26:
    /* 0D26  mov     word ptr cs:L0C24,dx */
    ww(CODE019, 0xC24, DX);
L0D2B:
    /* 0D2B  mov     dx,sp */
    DX = SP;
L0D2D:
    /* 0D2D  mov     word ptr cs:L0C26,dx */
    ww(CODE019, 0xC26, DX);
L0D32:
    /* 0D32  mov     bx,word ptr [bp+6] */
    BX = rw(pSS, BP + 0x6);
L0D35:
    /* 0D35  mov     cx,word ptr [bp+8] */
    CX = rw(pSS, BP + 0x8);
L0D38:
    /* 0D38  mov     ax,0C7h */
    AX = 0xC7;
L0D3B:
    /* 0D3B  sub     ax,cx */
    AX = (uint16_t)(AX - CX);
L0D3D:
    /* 0D3D  mov     cl,50h */
    CL = 0x50;
L0D3F:
    /* 0D3F  mul     cl */
    mul8(CL);
L0D41:
    /* 0D41  shr     bx,1 */
    BX = (uint16_t)(BX >> 1);
L0D43:
    /* 0D43  shr     bx,1 */
    BX = (uint16_t)(BX >> 1);
L0D45:
    /* 0D45  add     ax,bx */
    AX = (uint16_t)(AX + BX);
L0D47:
    /* 0D47  mov     bx,word ptr [bp+0Ah] */
    BX = rw(pSS, BP + 0xA);
L0D4A:
    /* 0D4A  mov     cx,word ptr [bp+0Ch] */
    CX = rw(pSS, BP + 0xC);
L0D4D:
    /* 0D4D  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L0D50:
    /* 0D50  mov     ds,dx */
    SET_DS(DX);
L0D52:
    /* 0D52  mov     es,dx */
    SET_ES(DX);
L0D54:
    /* 0D54  cli */
    ;
L0D55:
    /* 0D55  mov     ss,dx */
    SET_SS(DX);
L0D57:
    /* 0D57  mov     sp,word ptr ds:[5586h] */
    SP = rw(pDS, 0x5586);
L0D5B:
    /* 0D5B  sti */
    ;
L0D5C:
    /* 0D5C  mov     word ptr ds:[950h],ax */
    ww(pDS, 0x950, AX);
L0D5F:
    /* 0D5F  call    far ptr _seg003_B0C */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x0B0C), 0x1F3A + PORT_LOAD_SEG, 0x0D64)) != 0) return c;
L0D64:
    /* 0D64  cli */
    ;
L0D65:
    /* 0D65  mov     dx,word ptr cs:L0C24 */
    DX = rw(CODE019, 0xC24);
L0D6A:
    /* 0D6A  mov     ss,dx */
    SET_SS(DX);
L0D6C:
    /* 0D6C  mov     dx,word ptr cs:L0C26 */
    DX = rw(CODE019, 0xC26);
L0D71:
    /* 0D71  mov     sp,dx */
    SP = DX;
L0D73:
    /* 0D73  sti */
    ;
L0D74:
    /* 0D74  pop     es */
    SET_ES(pop16());
L0D75:
    /* 0D75  pop     ds */
    SET_DS(pop16());
L0D76:
    /* 0D76  pop     si */
    SI = pop16();
L0D77:
    /* 0D77  pop     di */
    DI = pop16();
L0D78:
    /* 0D78  pop     bp */
    BP = pop16();
L0D79:
    /* 0D79  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg019_D7A  (+D7A)
       cRender: draw one 3D frame. Switches to the private stack, notes the game clock, points int 0
       at seg063:04D0 (UW2 05A0; the renderer's overflow handler, INSTANCE.ASM), then calls GRENTRY's clear_fbuf,
       INSTANCE's render_3d, GRMISC's _18 and PGCACHE's update_cache; restores the stack and the int
       0 vector. Returns DX:AX = the clock ticks the frame took. */
L0D7A: /* _cRender */
    /* 0D7A  push    bp */
    push16(BP);
L0D7B:
    /* 0D7B  mov     bp,sp */
    BP = SP;
L0D7D:
    /* 0D7D  push    di */
    push16(DI);
L0D7E:
    /* 0D7E  push    si */
    push16(SI);
L0D7F:
    /* 0D7F  push    ds */
    push16(asm_ds);
L0D80:
    /* 0D80  push    es */
    push16(asm_es);
L0D81:
    /* 0D81  mov     dx,ss */
    DX = asm_ss;
L0D83:
    /* 0D83  mov     word ptr cs:L0C24,dx */
    ww(CODE019, 0xC24, DX);
L0D88:
    /* 0D88  mov     dx,sp */
    DX = SP;
L0D8A:
    /* 0D8A  mov     word ptr cs:L0C26,dx */
    ww(CODE019, 0xC26, DX);
L0D8F:
    /* 0D8F  mov     dx,seg seg063 */
    DX = (uint16_t)(0x5624 + PORT_LOAD_SEG);
L0D92:
    /* 0D92  mov     ds,dx */
    SET_DS(DX);
L0D94:
    /* 0D94  mov     es,dx */
    SET_ES(DX);
L0D96:
    /* 0D96  cli */
    ;
L0D97:
    /* 0D97  mov     ss,dx */
    SET_SS(DX);
L0D99:
    /* 0D99  mov     sp,448h */
    SP = 0x448;
L0D9C:
    /* 0D9C  sti */
    ;
L0D9D:
    /* 0D9D  mov     ax,word ptr cs:_seg019_710 */
    AX = rw(CODE019, 0x710);
L0DA1:
    /* 0DA1  mov     word ptr cs:L0C28,ax */
    ww(CODE019, 0xC28, AX);
L0DA5:
    /* 0DA5  mov     ax,word ptr cs:_seg019_712 */
    AX = rw(CODE019, 0x712);
L0DA9:
    /* 0DA9  mov     word ptr cs:L0C2A,ax */
    ww(CODE019, 0xC2A, AX);
L0DAD:
    /* 0DAD  mov     bx,0 */
    BX = 0x0;
L0DB0:
    /* 0DB0  mov     es,bx */
    SET_ES(BX);
L0DB2:
    /* 0DB2  mov     ax,word ptr es:[bx] */
    AX = rw(pES, BX);
L0DB5:
    /* 0DB5  mov     word ptr cs:L0C2C,ax */
    ww(CODE019, 0xC2C, AX);
L0DB9:
    /* 0DB9  mov     ax,word ptr es:[bx+2] */
    AX = rw(pES, BX + 0x2);
L0DBD:
    /* 0DBD  mov     word ptr cs:L0C2E,ax */
    ww(CODE019, 0xC2E, AX);
L0DC1:
    /* 0DC1  mov     dx,4D0h */
    DX = 0x4D0;
L0DC4:
    /* 0DC4  mov     ax,DOS_SET_VECTOR shl 8 */
    AX = 0x2500;
L0DC7:
    /* 0DC7  int     21h */
    asm_int(0x21);
L0DC9:
    /* 0DC9  call    far ptr _seg003_A46 */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x0A46), 0x1F3A + PORT_LOAD_SEG, 0x0DCE)) != 0) return c;
L0DCE:
    /* 0DCE  call    far ptr _render_3d */
    if ((c = asm_callf(ASM_JMP(0x06E7, 0x4987), 0x1F3A + PORT_LOAD_SEG, 0x0DD3)) != 0) return c;
L0DD3:
    /* 0DD3  call    far ptr _seg003_18 */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x0018), 0x1F3A + PORT_LOAD_SEG, 0x0DD8)) != 0) return c;
L0DD8:
    /* 0DD8  call    far ptr _update_cache */
    if ((c = asm_callf(ASM_JMP(0x06E7, 0x7B3D), 0x1F3A + PORT_LOAD_SEG, 0x0DDD)) != 0) return c;
L0DDD:
    /* 0DDD  cli */
    ;
L0DDE:
    /* 0DDE  mov     dx,word ptr cs:L0C24 */
    DX = rw(CODE019, 0xC24);
L0DE3:
    /* 0DE3  mov     ss,dx */
    SET_SS(DX);
L0DE5:
    /* 0DE5  mov     dx,word ptr cs:L0C26 */
    DX = rw(CODE019, 0xC26);
L0DEA:
    /* 0DEA  mov     sp,dx */
    SP = DX;
L0DEC:
    /* 0DEC  sti */
    ;
L0DED:
    /* 0DED  pop     es */
    SET_ES(pop16());
L0DEE:
    /* 0DEE  pop     ds */
    SET_DS(pop16());
L0DEF:
    /* 0DEF  pop     si */
    SI = pop16();
L0DF0:
    /* 0DF0  pop     di */
    DI = pop16();
L0DF1:
    /* 0DF1  mov     bx,0 */
    BX = 0x0;
L0DF4:
    /* 0DF4  mov     es,bx */
    SET_ES(BX);
L0DF6:
    /* 0DF6  mov     ax,word ptr cs:L0C2C */
    AX = rw(CODE019, 0xC2C);
L0DFA:
    /* 0DFA  mov     word ptr es:[bx],ax */
    ww(pES, BX, AX);
L0DFD:
    /* 0DFD  mov     ax,word ptr cs:L0C2E */
    AX = rw(CODE019, 0xC2E);
L0E01:
    /* 0E01  mov     word ptr es:[bx+2],ax */
    ww(pES, BX + 0x2, AX);
L0E05:
    /* 0E05  mov     ax,word ptr cs:_seg019_710 */
    AX = rw(CODE019, 0x710);
L0E09:
    /* 0E09  sub     ax,word ptr cs:L0C28 */
    AX = (uint16_t)(AX - rw(CODE019, 0xC28));
L0E0E:
    /* 0E0E  mov     dx,word ptr cs:_seg019_712 */
    DX = rw(CODE019, 0x712);
L0E13:
    /* 0E13  sub     dx,word ptr cs:L0C2A */
    DX = sub16(DX, rw(CODE019, 0xC2A), 0);
L0E18:
    /* 0E18  pop     bp */
    BP = pop16();
L0E19:
    /* 0E19  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg019_E1A  (+E1A)
       cInit3d: zero the game clock's low word and set the clip window from the record seg063:09C8
       (UW2 0A98) points at (GRDISP.ASM's _5363). */
L0E1A: /* _cInit3d */
    /* 0E1A  push    bp */
    push16(BP);
L0E1B:
    /* 0E1B  mov     bp,sp */
    BP = SP;
L0E1D:
    /* 0E1D  push    di */
    push16(DI);
L0E1E:
    /* 0E1E  push    si */
    push16(SI);
L0E1F:
    /* 0E1F  push    ds */
    push16(asm_ds);
L0E20:
    /* 0E20  push    es */
    push16(asm_es);
L0E21:
    /* 0E21  mov     dx,ss */
    DX = asm_ss;
L0E23:
    /* 0E23  mov     word ptr cs:L0C24,dx */
    ww(CODE019, 0xC24, DX);
L0E28:
    /* 0E28  mov     dx,sp */
    DX = SP;
L0E2A:
    /* 0E2A  mov     word ptr cs:L0C26,dx */
    ww(CODE019, 0xC26, DX);
L0E2F:
    /* 0E2F  mov     dx,seg seg063 */
    DX = (uint16_t)(0x5624 + PORT_LOAD_SEG);
L0E32:
    /* 0E32  mov     ds,dx */
    SET_DS(DX);
L0E34:
    /* 0E34  mov     es,dx */
    SET_ES(DX);
L0E36:
    /* 0E36  cli */
    ;
L0E37:
    /* 0E37  mov     ss,dx */
    SET_SS(DX);
L0E39:
    /* 0E39  mov     sp,448h */
    SP = 0x448;
L0E3C:
    /* 0E3C  sti */
    ;
L0E3D:
    /* 0E3D  mov     word ptr cs:_seg019_710,0 */
    ww(CODE019, 0x710, 0x0);
L0E44:
    /* 0E44  mov     si,word ptr ds:[9C8h] */
    SI = rw(pDS, 0x9C8);
L0E48:
    /* 0E48  call    far ptr _seg003_5B5D */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x5B5D), 0x1F3A + PORT_LOAD_SEG, 0x0E4D)) != 0) return c;
L0E4D:
    /* 0E4D  cli */
    ;
L0E4E:
    /* 0E4E  mov     dx,word ptr cs:L0C24 */
    DX = rw(CODE019, 0xC24);
L0E53:
    /* 0E53  mov     ss,dx */
    SET_SS(DX);
L0E55:
    /* 0E55  mov     dx,word ptr cs:L0C26 */
    DX = rw(CODE019, 0xC26);
L0E5A:
    /* 0E5A  mov     sp,dx */
    SP = DX;
L0E5C:
    /* 0E5C  sti */
    ;
L0E5D:
    /* 0E5D  pop     es */
    SET_ES(pop16());
L0E5E:
    /* 0E5E  pop     ds */
    SET_DS(pop16());
L0E5F:
    /* 0E5F  pop     si */
    SI = pop16();
L0E60:
    /* 0E60  pop     di */
    DI = pop16();
L0E61:
    /* 0E61  pop     bp */
    BP = pop16();
L0E62:
    /* 0E62  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* GetVectorForDirection_seg019_E63  (+E63)
       cFstSinCos(angle, &sin, &cos) (FM Towns; the label is an old guess): the table sine and cosine
       (IMATH.ASM's _A69). */
L0E63: /* _cFstSinCos */
    /* 0E63  push    bp */
    push16(BP);
L0E64:
    /* 0E64  mov     bp,sp */
    BP = SP;
L0E66:
    /* 0E66  push    di */
    push16(DI);
L0E67:
    /* 0E67  push    si */
    push16(SI);
L0E68:
    /* 0E68  push    ds */
    push16(asm_ds);
L0E69:
    /* 0E69  push    es */
    push16(asm_es);
L0E6A:
    /* 0E6A  mov     dx,ss */
    DX = asm_ss;
L0E6C:
    /* 0E6C  mov     word ptr cs:L0C24,dx */
    ww(CODE019, 0xC24, DX);
L0E71:
    /* 0E71  mov     dx,sp */
    DX = SP;
L0E73:
    /* 0E73  mov     word ptr cs:L0C26,dx */
    ww(CODE019, 0xC26, DX);
L0E78:
    /* 0E78  mov     bx,word ptr [bp+6] */
    BX = rw(pSS, BP + 0x6);
L0E7B:
    /* 0E7B  mov     dx,seg seg063 */
    DX = (uint16_t)(0x5624 + PORT_LOAD_SEG);
L0E7E:
    /* 0E7E  mov     ds,dx */
    SET_DS(DX);
L0E80:
    /* 0E80  mov     es,dx */
    SET_ES(DX);
L0E82:
    /* 0E82  cli */
    ;
L0E83:
    /* 0E83  mov     ss,dx */
    SET_SS(DX);
L0E85:
    /* 0E85  mov     sp,448h */
    SP = 0x448;
L0E88:
    /* 0E88  sti */
    ;
L0E89:
    /* 0E89  call    _seg019_A69 */
    if ((c = asm_call(ASM_JMP(0x1F3A, 0x0A69), 0x0E8C)) != 0) return c;
L0E8C:
    /* 0E8C  cli */
    ;
L0E8D:
    /* 0E8D  mov     dx,word ptr cs:L0C24 */
    DX = rw(CODE019, 0xC24);
L0E92:
    /* 0E92  mov     ss,dx */
    SET_SS(DX);
L0E94:
    /* 0E94  mov     dx,word ptr cs:L0C26 */
    DX = rw(CODE019, 0xC26);
L0E99:
    /* 0E99  mov     sp,dx */
    SP = DX;
L0E9B:
    /* 0E9B  sti */
    ;
L0E9C:
    /* 0E9C  pop     es */
    SET_ES(pop16());
L0E9D:
    /* 0E9D  pop     ds */
    SET_DS(pop16());
L0E9E:
    /* 0E9E  pop     si */
    SI = pop16();
L0E9F:
    /* 0E9F  pop     di */
    DI = pop16();
L0EA0:
    /* 0EA0  push    di */
    push16(DI);
L0EA1:
    /* 0EA1  mov     di,word ptr [bp+8] */
    DI = rw(pSS, BP + 0x8);
L0EA4:
    /* 0EA4  mov     word ptr [di],ax */
    ww(pDS, DI, AX);
L0EA6:
    /* 0EA6  mov     di,word ptr [bp+0Ah] */
    DI = rw(pSS, BP + 0xA);
L0EA9:
    /* 0EA9  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L0EAB:
    /* 0EAB  pop     di */
    DI = pop16();
L0EAC:
    /* 0EAC  pop     bp */
    BP = pop16();
L0EAD:
    /* 0EAD  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* SomethingProjectileHeading_seg019_EAE  (+EAE)
       cSinCos(angle, &sin, &cos): the interpolated sine and cosine (IMATH.ASM's _A38). */
L0EAE: /* _cSinCos */
    /* 0EAE  push    bp */
    push16(BP);
L0EAF:
    /* 0EAF  mov     bp,sp */
    BP = SP;
L0EB1:
    /* 0EB1  push    di */
    push16(DI);
L0EB2:
    /* 0EB2  push    si */
    push16(SI);
L0EB3:
    /* 0EB3  push    ds */
    push16(asm_ds);
L0EB4:
    /* 0EB4  push    es */
    push16(asm_es);
L0EB5:
    /* 0EB5  mov     dx,ss */
    DX = asm_ss;
L0EB7:
    /* 0EB7  mov     word ptr cs:L0C24,dx */
    ww(CODE019, 0xC24, DX);
L0EBC:
    /* 0EBC  mov     dx,sp */
    DX = SP;
L0EBE:
    /* 0EBE  mov     word ptr cs:L0C26,dx */
    ww(CODE019, 0xC26, DX);
L0EC3:
    /* 0EC3  mov     bx,word ptr [bp+6] */
    BX = rw(pSS, BP + 0x6);
L0EC6:
    /* 0EC6  mov     dx,seg seg063 */
    DX = (uint16_t)(0x5624 + PORT_LOAD_SEG);
L0EC9:
    /* 0EC9  mov     ds,dx */
    SET_DS(DX);
L0ECB:
    /* 0ECB  mov     es,dx */
    SET_ES(DX);
L0ECD:
    /* 0ECD  cli */
    ;
L0ECE:
    /* 0ECE  mov     ss,dx */
    SET_SS(DX);
L0ED0:
    /* 0ED0  mov     sp,448h */
    SP = 0x448;
L0ED3:
    /* 0ED3  sti */
    ;
L0ED4:
    /* 0ED4  push    bp */
    push16(BP);
L0ED5:
    /* 0ED5  call    _seg019_A38 */
    if ((c = asm_call(ASM_JMP(0x1F3A, 0x0A38), 0x0ED8)) != 0) return c;
L0ED8:
    /* 0ED8  pop     bp */
    BP = pop16();
L0ED9:
    /* 0ED9  cli */
    ;
L0EDA:
    /* 0EDA  mov     dx,word ptr cs:L0C24 */
    DX = rw(CODE019, 0xC24);
L0EDF:
    /* 0EDF  mov     ss,dx */
    SET_SS(DX);
L0EE1:
    /* 0EE1  mov     dx,word ptr cs:L0C26 */
    DX = rw(CODE019, 0xC26);
L0EE6:
    /* 0EE6  mov     sp,dx */
    SP = DX;
L0EE8:
    /* 0EE8  sti */
    ;
L0EE9:
    /* 0EE9  pop     es */
    SET_ES(pop16());
L0EEA:
    /* 0EEA  pop     ds */
    SET_DS(pop16());
L0EEB:
    /* 0EEB  pop     si */
    SI = pop16();
L0EEC:
    /* 0EEC  pop     di */
    DI = pop16();
L0EED:
    /* 0EED  push    di */
    push16(DI);
L0EEE:
    /* 0EEE  mov     di,word ptr [bp+8] */
    DI = rw(pSS, BP + 0x8);
L0EF1:
    /* 0EF1  mov     word ptr [di],ax */
    ww(pDS, DI, AX);
L0EF3:
    /* 0EF3  mov     di,word ptr [bp+0Ah] */
    DI = rw(pSS, BP + 0xA);
L0EF6:
    /* 0EF6  mov     word ptr [di],bx */
    ww(pDS, DI, BX);
L0EF8:
    /* 0EF8  pop     di */
    DI = pop16();
L0EF9:
    /* 0EF9  pop     bp */
    BP = pop16();
L0EFA:
    /* 0EFA  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* MaybeGetTangent_seg019_EFB  (+EFB)
       cAtan2(sin, cos): the angle of a unit vector (IMATH.ASM's _BD4). */
L0EFB: /* _cAtan2 */
    /* 0EFB  push    bp */
    push16(BP);
L0EFC:
    /* 0EFC  mov     bp,sp */
    BP = SP;
L0EFE:
    /* 0EFE  push    di */
    push16(DI);
L0EFF:
    /* 0EFF  push    si */
    push16(SI);
L0F00:
    /* 0F00  push    ds */
    push16(asm_ds);
L0F01:
    /* 0F01  push    es */
    push16(asm_es);
L0F02:
    /* 0F02  mov     dx,ss */
    DX = asm_ss;
L0F04:
    /* 0F04  mov     word ptr cs:L0C24,dx */
    ww(CODE019, 0xC24, DX);
L0F09:
    /* 0F09  mov     dx,sp */
    DX = SP;
L0F0B:
    /* 0F0B  mov     word ptr cs:L0C26,dx */
    ww(CODE019, 0xC26, DX);
L0F10:
    /* 0F10  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L0F13:
    /* 0F13  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L0F16:
    /* 0F16  mov     dx,seg seg063 */
    DX = (uint16_t)(0x5624 + PORT_LOAD_SEG);
L0F19:
    /* 0F19  mov     ds,dx */
    SET_DS(DX);
L0F1B:
    /* 0F1B  mov     es,dx */
    SET_ES(DX);
L0F1D:
    /* 0F1D  cli */
    ;
L0F1E:
    /* 0F1E  mov     ss,dx */
    SET_SS(DX);
L0F20:
    /* 0F20  mov     sp,448h */
    SP = 0x448;
L0F23:
    /* 0F23  sti */
    ;
L0F24:
    /* 0F24  call    _MaybeTangent_seg019_BD4 */
    if ((c = asm_call(ASM_JMP(0x1F3A, 0x0BD4), 0x0F27)) != 0) return c;
L0F27:
    /* 0F27  mov     ax,cx */
    AX = CX;
L0F29:
    /* 0F29  cli */
    ;
L0F2A:
    /* 0F2A  mov     dx,word ptr cs:L0C24 */
    DX = rw(CODE019, 0xC24);
L0F2F:
    /* 0F2F  mov     ss,dx */
    SET_SS(DX);
L0F31:
    /* 0F31  mov     dx,word ptr cs:L0C26 */
    DX = rw(CODE019, 0xC26);
L0F36:
    /* 0F36  mov     sp,dx */
    SP = DX;
L0F38:
    /* 0F38  sti */
    ;
L0F39:
    /* 0F39  pop     es */
    SET_ES(pop16());
L0F3A:
    /* 0F3A  pop     ds */
    SET_DS(pop16());
L0F3B:
    /* 0F3B  pop     si */
    SI = pop16();
L0F3C:
    /* 0F3C  pop     di */
    DI = pop16();
L0F3D:
    /* 0F3D  pop     bp */
    BP = pop16();
L0F3E:
    /* 0F3E  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* GetSquareRoot_seg019_F3F  (+F3F)
       cSqRt(long): the square root of a 32-bit value (IMATH.ASM's _A78). */
L0F3F: /* _cSqRt */
    /* 0F3F  push    bp */
    push16(BP);
L0F40:
    /* 0F40  mov     bp,sp */
    BP = SP;
L0F42:
    /* 0F42  push    di */
    push16(DI);
L0F43:
    /* 0F43  push    si */
    push16(SI);
L0F44:
    /* 0F44  push    ds */
    push16(asm_ds);
L0F45:
    /* 0F45  push    es */
    push16(asm_es);
L0F46:
    /* 0F46  mov     dx,ss */
    DX = asm_ss;
L0F48:
    /* 0F48  mov     word ptr cs:L0C24,dx */
    ww(CODE019, 0xC24, DX);
L0F4D:
    /* 0F4D  mov     dx,sp */
    DX = SP;
L0F4F:
    /* 0F4F  mov     word ptr cs:L0C26,dx */
    ww(CODE019, 0xC26, DX);
L0F54:
    /* 0F54  mov     bx,word ptr [bp+6] */
    BX = rw(pSS, BP + 0x6);
L0F57:
    /* 0F57  mov     cx,word ptr [bp+8] */
    CX = rw(pSS, BP + 0x8);
L0F5A:
    /* 0F5A  mov     dx,seg seg063 */
    DX = (uint16_t)(0x5624 + PORT_LOAD_SEG);
L0F5D:
    /* 0F5D  mov     ds,dx */
    SET_DS(DX);
L0F5F:
    /* 0F5F  mov     es,dx */
    SET_ES(DX);
L0F61:
    /* 0F61  cli */
    ;
L0F62:
    /* 0F62  mov     ss,dx */
    SET_SS(DX);
L0F64:
    /* 0F64  mov     sp,448h */
    SP = 0x448;
L0F67:
    /* 0F67  sti */
    ;
L0F68:
    /* 0F68  call    _SquareRoot_seg019_A78 */
    if ((c = asm_call(ASM_JMP(0x1F3A, 0x0A78), 0x0F6B)) != 0) return c;
L0F6B:
    /* 0F6B  mov     ax,di */
    AX = DI;
L0F6D:
    /* 0F6D  cli */
    ;
L0F6E:
    /* 0F6E  mov     dx,word ptr cs:L0C24 */
    DX = rw(CODE019, 0xC24);
L0F73:
    /* 0F73  mov     ss,dx */
    SET_SS(DX);
L0F75:
    /* 0F75  mov     dx,word ptr cs:L0C26 */
    DX = rw(CODE019, 0xC26);
L0F7A:
    /* 0F7A  mov     sp,dx */
    SP = DX;
L0F7C:
    /* 0F7C  sti */
    ;
L0F7D:
    /* 0F7D  pop     es */
    SET_ES(pop16());
L0F7E:
    /* 0F7E  pop     ds */
    SET_DS(pop16());
L0F7F:
    /* 0F7F  pop     si */
    SI = pop16();
L0F80:
    /* 0F80  pop     di */
    DI = pop16();
L0F81:
    /* 0F81  pop     bp */
    BP = pop16();
L0F82:
    /* 0F82  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* cFrmtoRaw  (+F83)
       cFrmtoRaw(a, b, source offset, source segment, format): decode a picture in one of the .GR/.CR
       formats to one byte per pixel through PGCACHE.ASM's far stub to the decoders (EXPAND.ASM). The
       first two arguments go to BP and AX, where the decoders take their destination and palette
       arguments (EXPAND.ASM has the details); DH is set to 0FFh. */
L0F83: /* _cFrmtoRaw */
    /* 0F83  push    bp */
    push16(BP);
L0F84:
    /* 0F84  mov     bp,sp */
    BP = SP;
L0F86:
    /* 0F86  push    di */
    push16(DI);
L0F87:
    /* 0F87  push    si */
    push16(SI);
L0F88:
    /* 0F88  push    ds */
    push16(asm_ds);
L0F89:
    /* 0F89  push    es */
    push16(asm_es);
L0F8A:
    /* 0F8A  mov     dx,ss */
    DX = asm_ss;
L0F8C:
    /* 0F8C  mov     word ptr cs:L0C24,dx */
    ww(CODE019, 0xC24, DX);
L0F91:
    /* 0F91  mov     dx,sp */
    DX = SP;
L0F93:
    /* 0F93  mov     word ptr cs:L0C26,dx */
    ww(CODE019, 0xC26, DX);
L0F98:
    /* 0F98  mov     dx,word ptr [bp+6] */
    DX = rw(pSS, BP + 0x6);
L0F9B:
    /* 0F9B  mov     ax,word ptr [bp+8] */
    AX = rw(pSS, BP + 0x8);
L0F9E:
    /* 0F9E  mov     si,word ptr [bp+0Ah] */
    SI = rw(pSS, BP + 0xA);
L0FA1:
    /* 0FA1  mov     cx,word ptr [bp+0Ch] */
    CX = rw(pSS, BP + 0xC);
L0FA4:
    /* 0FA4  mov     bl,byte ptr [bp+0Eh] */
    BL = rb(pSS, BP + 0xE);
L0FA7:
    /* 0FA7  xor     bh,bh */
    BH = (uint8_t)(BH ^ BH);
L0FA9:
    /* 0FA9  mov     bp,dx */
    BP = DX;
L0FAB:
    /* 0FAB  mov     dx,seg seg063 */
    DX = (uint16_t)(0x5624 + PORT_LOAD_SEG);
L0FAE:
    /* 0FAE  mov     ds,dx */
    SET_DS(DX);
L0FB0:
    /* 0FB0  mov     es,dx */
    SET_ES(DX);
L0FB2:
    /* 0FB2  cli */
    ;
L0FB3:
    /* 0FB3  mov     ss,dx */
    SET_SS(DX);
L0FB5:
    /* 0FB5  mov     sp,448h */
    SP = 0x448;
L0FB8:
    /* 0FB8  sti */
    ;
L0FB9:
    /* 0FB9  mov     ds,cx */
    SET_DS(CX);
L0FBB:
    /* 0FBB  mov     dh,0FFh */
    DH = 0xFF;
L0FBD:
    /* 0FBD  call    far ptr _seg004_7B37 */
    if ((c = asm_callf(ASM_JMP(0x06E7, 0x7B37), 0x1F3A + PORT_LOAD_SEG, 0x0FC2)) != 0) return c;
L0FC2:
    /* 0FC2  cli */
    ;
L0FC3:
    /* 0FC3  mov     dx,word ptr cs:L0C24 */
    DX = rw(CODE019, 0xC24);
L0FC8:
    /* 0FC8  mov     ss,dx */
    SET_SS(DX);
L0FCA:
    /* 0FCA  mov     dx,word ptr cs:L0C26 */
    DX = rw(CODE019, 0xC26);
L0FCF:
    /* 0FCF  mov     sp,dx */
    SP = DX;
L0FD1:
    /* 0FD1  sti */
    ;
L0FD2:
    /* 0FD2  pop     es */
    SET_ES(pop16());
L0FD3:
    /* 0FD3  pop     ds */
    SET_DS(pop16());
L0FD4:
    /* 0FD4  pop     si */
    SI = pop16();
L0FD5:
    /* 0FD5  pop     di */
    DI = pop16();
L0FD6:
    /* 0FD6  mov     dx,ax */
    DX = AX;
L0FD8:
    /* 0FD8  mov     ax,0 */
    AX = 0x0;
L0FDB:
    /* 0FDB  pop     bp */
    BP = pop16();
L0FDC:
    /* 0FDC  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg019_FDD  (+FDD)
       _FDD: copy the used part of the private stack (SP up to 448h) to just below seg063:0A92 (UW2
       510h, 0C2A), and
       keep the caller's saved SS:SP (L0C24) in L0C20, so a nested entry cannot overwrite either. */
L0FDD: /* _seg019_FDD */
    /* 0FDD  mov     ax,ss */
    AX = asm_ss;
L0FDF:
    /* 0FDF  mov     ds,ax */
    SET_DS(AX);
L0FE1:
    /* 0FE1  mov     si,448h */
    SI = 0x448;
L0FE4:
    /* 0FE4  mov     cx,si */
    CX = SI;
L0FE6:
    /* 0FE6  sub     cx,sp */
    CX = (uint16_t)(CX - SP);
L0FE8:
    /* 0FE8  shr     cx,1 */
    CX = shr16(CX, 1);
L0FEA:
    /* 0FEA  mov     es,ax */
    SET_ES(AX);
L0FEC:
    /* 0FEC  mov     di,0A92h */
    DI = 0xA92;
L0FEF:
    /* 0FEF  std */
    DF = 1;
L0FF0:
    /* 0FF0  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0FF2:
    /* 0FF2  cld */
    DF = 0;
L0FF3:
    /* 0FF3  mov     ax,word ptr cs:L0C24 */
    AX = rw(CODE019, 0xC24);
L0FF7:
    /* 0FF7  mov     word ptr cs:L0C20,ax */
    ww(CODE019, 0xC20, AX);
L0FFB:
    /* 0FFB  mov     ax,word ptr cs:L0C26 */
    AX = rw(CODE019, 0xC26);
L0FFF:
    /* 0FFF  mov     word ptr cs:L0C22,ax */
    ww(CODE019, 0xC22, AX);
L1003:
    /* 1003  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg019_1004  (+1004)
       _1004: copy the private stack back from the save area and restore L0C24 from L0C20. */
L1004: /* _seg019_1004 */
    /* 1004  mov     ax,ss */
    AX = asm_ss;
L1006:
    /* 1006  mov     ds,ax */
    SET_DS(AX);
L1008:
    /* 1008  mov     si,0A92h */
    SI = 0xA92;
L100B:
    /* 100B  mov     di,448h */
    DI = 0x448;
L100E:
    /* 100E  mov     cx,di */
    CX = DI;
L1010:
    /* 1010  sub     cx,sp */
    CX = (uint16_t)(CX - SP);
L1012:
    /* 1012  shr     cx,1 */
    CX = shr16(CX, 1);
L1014:
    /* 1014  mov     es,ax */
    SET_ES(AX);
L1016:
    /* 1016  std */
    DF = 1;
L1017:
    /* 1017  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L1019:
    /* 1019  cld */
    DF = 0;
L101A:
    /* 101A  mov     ax,word ptr cs:L0C20 */
    AX = rw(CODE019, 0xC20);
L101E:
    /* 101E  mov     word ptr cs:L0C24,ax */
    ww(CODE019, 0xC24, AX);
L1022:
    /* 1022  mov     ax,word ptr cs:L0C22 */
    AX = rw(CODE019, 0xC22);
L1026:
    /* 1026  mov     word ptr cs:L0C26,ax */
    ww(CODE019, 0xC26, AX);
L102A:
    /* 102A  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
