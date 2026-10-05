/* pgcache_x.c: replaces src/3d/PGCACHE.ASM (seg004_0849_6D20, 6D20..8154 of its
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
    case 0x6D3C: goto L6D3C;
    case 0x6D3D: goto L6D3D;
    case 0x7D3E: goto L7D3E;
    case 0x7D3F: goto L7D3F;
    case 0x7D41: goto L7D41;
    case 0x7D42: goto L7D42;
    case 0x7D44: goto L7D44;
    case 0x7D45: goto L7D45;
    case 0x7D46: goto L7D46;
    case 0x7D47: goto L7D47;
    case 0x7D49: goto L7D49;
    case 0x7D4C: goto L7D4C;
    case 0x7D50: goto L7D50;
    case 0x7D53: goto L7D53;
    case 0x7D56: goto L7D56;
    case 0x7D59: goto L7D59;
    case 0x7D5B: goto L7D5B;
    case 0x7D5F: goto L7D5F;
    case 0x7D61: goto L7D61;
    case 0x7D64: goto L7D64;
    case 0x7D66: goto L7D66;
    case 0x7D68: goto L7D68;
    case 0x7D6A: goto L7D6A;
    case 0x7D6C: goto L7D6C;
    case 0x7D70: goto L7D70;
    case 0x7D72: goto L7D72;
    case 0x7D74: goto L7D74;
    case 0x7D76: goto L7D76;
    case 0x7D77: goto L7D77;
    case 0x7D79: goto L7D79;
    case 0x7D7C: goto L7D7C;
    case 0x7D7E: goto L7D7E;
    case 0x7D81: goto L7D81;
    case 0x7D83: goto L7D83;
    case 0x7D84: goto L7D84;
    case 0x7D87: goto L7D87;
    case 0x7D89: goto L7D89;
    case 0x7D8D: goto L7D8D;
    case 0x7D90: goto L7D90;
    case 0x7D92: goto L7D92;
    case 0x7D94: goto L7D94;
    case 0x7D97: goto L7D97;
    case 0x7D9B: goto L7D9B;
    case 0x7D9C: goto L7D9C;
    case 0x7D9D: goto L7D9D;
    case 0x7D9E: goto L7D9E;
    case 0x7D9F: goto L7D9F;
    case 0x7DA0: goto L7DA0;
    case 0x7DA4: goto L7DA4;
    case 0x7DA5: goto L7DA5;
    case 0x7DA7: goto L7DA7;
    case 0x7DA8: goto L7DA8;
    case 0x7DA9: goto L7DA9;
    case 0x7DAA: goto L7DAA;
    case 0x7DAB: goto L7DAB;
    case 0x7DAC: goto L7DAC;
    case 0x7DAE: goto L7DAE;
    case 0x7DB1: goto L7DB1;
    case 0x7DB3: goto L7DB3;
    case 0x7DB5: goto L7DB5;
    case 0x7DB7: goto L7DB7;
    case 0x7DBA: goto L7DBA;
    case 0x7DBC: goto L7DBC;
    case 0x7DBD: goto L7DBD;
    case 0x7DC0: goto L7DC0;
    case 0x7DC3: goto L7DC3;
    case 0x7DC5: goto L7DC5;
    case 0x7DC7: goto L7DC7;
    case 0x7DC9: goto L7DC9;
    case 0x7DCC: goto L7DCC;
    case 0x7DCD: goto L7DCD;
    case 0x7DD0: goto L7DD0;
    case 0x7DD2: goto L7DD2;
    case 0x7DD3: goto L7DD3;
    case 0x7DD4: goto L7DD4;
    case 0x7DD7: goto L7DD7;
    case 0x7DD8: goto L7DD8;
    case 0x7DDB: goto L7DDB;
    case 0x7DDD: goto L7DDD;
    case 0x7DDF: goto L7DDF;
    case 0x7DE1: goto L7DE1;
    case 0x7DE3: goto L7DE3;
    case 0x7DE6: goto L7DE6;
    case 0x7DE8: goto L7DE8;
    case 0x7DEA: goto L7DEA;
    case 0x7DEC: goto L7DEC;
    case 0x7DED: goto L7DED;
    case 0x7DEF: goto L7DEF;
    case 0x7DF0: goto L7DF0;
    case 0x7DF1: goto L7DF1;
    case 0x7DF4: goto L7DF4;
    case 0x7DF6: goto L7DF6;
    case 0x7DF9: goto L7DF9;
    case 0x7DFC: goto L7DFC;
    case 0x7DFE: goto L7DFE;
    case 0x7E00: goto L7E00;
    case 0x7E01: goto L7E01;
    case 0x7E02: goto L7E02;
    case 0x7E03: goto L7E03;
    case 0x7E04: goto L7E04;
    case 0x7E05: goto L7E05;
    case 0x7E09: goto L7E09;
    case 0x7E0A: goto L7E0A;
    case 0x7E0D: goto L7E0D;
    case 0x7E11: goto L7E11;
    case 0x7E13: goto L7E13;
    case 0x7E14: goto L7E14;
    case 0x7E17: goto L7E17;
    case 0x7E19: goto L7E19;
    case 0x7E1C: goto L7E1C;
    case 0x7E1F: goto L7E1F;
    case 0x7E21: goto L7E21;
    case 0x7E23: goto L7E23;
    case 0x7E26: goto L7E26;
    case 0x7E29: goto L7E29;
    case 0x7E2B: goto L7E2B;
    case 0x7E2D: goto L7E2D;
    case 0x7E2F: goto L7E2F;
    case 0x7E32: goto L7E32;
    case 0x7E34: goto L7E34;
    case 0x7E35: goto L7E35;
    case 0x7E37: goto L7E37;
    case 0x7E39: goto L7E39;
    case 0x7E3A: goto L7E3A;
    case 0x7E3B: goto L7E3B;
    case 0x7E3D: goto L7E3D;
    case 0x7E3F: goto L7E3F;
    case 0x7E43: goto L7E43;
    case 0x7E46: goto L7E46;
    case 0x7E48: goto L7E48;
    case 0x7E4A: goto L7E4A;
    case 0x7E4C: goto L7E4C;
    case 0x7E4E: goto L7E4E;
    case 0x7E50: goto L7E50;
    case 0x7E52: goto L7E52;
    case 0x7E54: goto L7E54;
    case 0x7E57: goto L7E57;
    case 0x7E59: goto L7E59;
    case 0x7E5C: goto L7E5C;
    case 0x7E5E: goto L7E5E;
    case 0x7E60: goto L7E60;
    case 0x7E62: goto L7E62;
    case 0x7E65: goto L7E65;
    case 0x7E6A: goto L7E6A;
    case 0x7E6B: goto L7E6B;
    case 0x7E6E: goto L7E6E;
    case 0x7E71: goto L7E71;
    case 0x7E73: goto L7E73;
    case 0x7E74: goto L7E74;
    case 0x7E77: goto L7E77;
    case 0x7E78: goto L7E78;
    case 0x7E79: goto L7E79;
    case 0x7E7A: goto L7E7A;
    case 0x7E7D: goto L7E7D;
    case 0x7E7F: goto L7E7F;
    case 0x7E80: goto L7E80;
    case 0x7E81: goto L7E81;
    case 0x7E83: goto L7E83;
    case 0x7E87: goto L7E87;
    case 0x7E8A: goto L7E8A;
    case 0x7E8F: goto L7E8F;
    case 0x7E90: goto L7E90;
    case 0x7E92: goto L7E92;
    case 0x7E95: goto L7E95;
    case 0x7E97: goto L7E97;
    case 0x7E99: goto L7E99;
    case 0x7E9B: goto L7E9B;
    case 0x7E9D: goto L7E9D;
    case 0x7EA0: goto L7EA0;
    case 0x7EA2: goto L7EA2;
    case 0x7EA5: goto L7EA5;
    case 0x7EA8: goto L7EA8;
    case 0x7EAA: goto L7EAA;
    case 0x7EAD: goto L7EAD;
    case 0x7EAF: goto L7EAF;
    case 0x7EB0: goto L7EB0;
    case 0x7EB2: goto L7EB2;
    case 0x7EB4: goto L7EB4;
    case 0x7EB6: goto L7EB6;
    case 0x7EB7: goto L7EB7;
    case 0x7EB9: goto L7EB9;
    case 0x7EBA: goto L7EBA;
    case 0x7EBB: goto L7EBB;
    case 0x7EBE: goto L7EBE;
    case 0x7EC0: goto L7EC0;
    case 0x7EC2: goto L7EC2;
    case 0x7EC3: goto L7EC3;
    case 0x7EC5: goto L7EC5;
    case 0x7EC8: goto L7EC8;
    case 0x7ECA: goto L7ECA;
    case 0x7ECC: goto L7ECC;
    case 0x7ECE: goto L7ECE;
    case 0x7ED1: goto L7ED1;
    case 0x7ED2: goto L7ED2;
    case 0x7ED5: goto L7ED5;
    case 0x7ED8: goto L7ED8;
    case 0x7ED9: goto L7ED9;
    case 0x7EDA: goto L7EDA;
    case 0x7EDE: goto L7EDE;
    case 0x7EE0: goto L7EE0;
    case 0x7EE2: goto L7EE2;
    case 0x7EE4: goto L7EE4;
    case 0x7EE8: goto L7EE8;
    case 0x7EEA: goto L7EEA;
    case 0x7EED: goto L7EED;
    case 0x7EEF: goto L7EEF;
    case 0x7EF1: goto L7EF1;
    case 0x7EF3: goto L7EF3;
    case 0x7EF7: goto L7EF7;
    case 0x7EF9: goto L7EF9;
    case 0x7EFB: goto L7EFB;
    case 0x7EFD: goto L7EFD;
    case 0x7EFF: goto L7EFF;
    case 0x7F02: goto L7F02;
    case 0x7F06: goto L7F06;
    case 0x7F09: goto L7F09;
    case 0x7F0A: goto L7F0A;
    case 0x7F0B: goto L7F0B;
    case 0x7F0C: goto L7F0C;
    case 0x7F11: goto L7F11;
    case 0x7F12: goto L7F12;
    case 0x7F14: goto L7F14;
    case 0x7F17: goto L7F17;
    case 0x7F19: goto L7F19;
    case 0x7F1C: goto L7F1C;
    case 0x7F1F: goto L7F1F;
    case 0x7F21: goto L7F21;
    case 0x7F23: goto L7F23;
    case 0x7F25: goto L7F25;
    case 0x7F27: goto L7F27;
    case 0x7F29: goto L7F29;
    case 0x7F2B: goto L7F2B;
    case 0x7F2C: goto L7F2C;
    case 0x7F2E: goto L7F2E;
    case 0x7F30: goto L7F30;
    case 0x7F31: goto L7F31;
    case 0x7F34: goto L7F34;
    case 0x7F36: goto L7F36;
    case 0x7F38: goto L7F38;
    case 0x7F3C: goto L7F3C;
    case 0x7F3E: goto L7F3E;
    case 0x7F40: goto L7F40;
    case 0x7F42: goto L7F42;
    case 0x7F44: goto L7F44;
    case 0x7F46: goto L7F46;
    case 0x7F47: goto L7F47;
    case 0x7F49: goto L7F49;
    case 0x7F4C: goto L7F4C;
    case 0x7F4E: goto L7F4E;
    case 0x7F51: goto L7F51;
    case 0x7F53: goto L7F53;
    case 0x7F55: goto L7F55;
    case 0x7F57: goto L7F57;
    case 0x7F59: goto L7F59;
    case 0x7F5E: goto L7F5E;
    case 0x7F62: goto L7F62;
    case 0x7F67: goto L7F67;
    case 0x7F69: goto L7F69;
    case 0x7F6D: goto L7F6D;
    case 0x7F6F: goto L7F6F;
    case 0x7F73: goto L7F73;
    case 0x7F75: goto L7F75;
    case 0x7F77: goto L7F77;
    case 0x7F79: goto L7F79;
    case 0x7F7C: goto L7F7C;
    case 0x7F7F: goto L7F7F;
    case 0x7F81: goto L7F81;
    case 0x7F84: goto L7F84;
    case 0x7F87: goto L7F87;
    case 0x7F88: goto L7F88;
    case 0x7F89: goto L7F89;
    case 0x7F8D: goto L7F8D;
    case 0x7F90: goto L7F90;
    case 0x7F92: goto L7F92;
    case 0x7F96: goto L7F96;
    case 0x7F98: goto L7F98;
    case 0x7F9A: goto L7F9A;
    case 0x7F9D: goto L7F9D;
    case 0x7FA0: goto L7FA0;
    case 0x7FA2: goto L7FA2;
    case 0x7FA4: goto L7FA4;
    case 0x7FA6: goto L7FA6;
    case 0x7FAA: goto L7FAA;
    case 0x7FAC: goto L7FAC;
    case 0x7FAE: goto L7FAE;
    case 0x7FAF: goto L7FAF;
    case 0x7FB1: goto L7FB1;
    case 0x7FB2: goto L7FB2;
    case 0x7FB4: goto L7FB4;
    case 0x7FB7: goto L7FB7;
    case 0x7FB9: goto L7FB9;
    case 0x7FBD: goto L7FBD;
    case 0x7FBE: goto L7FBE;
    case 0x7FC2: goto L7FC2;
    case 0x7FC6: goto L7FC6;
    case 0x7FC7: goto L7FC7;
    case 0x7FC9: goto L7FC9;
    case 0x7FCC: goto L7FCC;
    case 0x7FCF: goto L7FCF;
    case 0x7FD1: goto L7FD1;
    case 0x7FD3: goto L7FD3;
    case 0x7FD5: goto L7FD5;
    case 0x7FD7: goto L7FD7;
    case 0x7FD8: goto L7FD8;
    case 0x7FDA: goto L7FDA;
    case 0x7FDD: goto L7FDD;
    case 0x7FDF: goto L7FDF;
    case 0x7FE0: goto L7FE0;
    case 0x7FE1: goto L7FE1;
    case 0x7FE3: goto L7FE3;
    case 0x7FE5: goto L7FE5;
    case 0x7FE7: goto L7FE7;
    case 0x7FE8: goto L7FE8;
    case 0x7FEA: goto L7FEA;
    case 0x7FEC: goto L7FEC;
    case 0x7FEE: goto L7FEE;
    case 0x7FEF: goto L7FEF;
    case 0x7FF2: goto L7FF2;
    case 0x7FF5: goto L7FF5;
    case 0x7FF7: goto L7FF7;
    case 0x7FF9: goto L7FF9;
    case 0x7FFB: goto L7FFB;
    case 0x7FFC: goto L7FFC;
    case 0x7FFD: goto L7FFD;
    case 0x8000: goto L8000;
    case 0x8001: goto L8001;
    case 0x8004: goto L8004;
    case 0x8007: goto L8007;
    case 0x800A: goto L800A;
    case 0x800C: goto L800C;
    case 0x800F: goto L800F;
    case 0x8011: goto L8011;
    case 0x8012: goto L8012;
    case 0x8014: goto L8014;
    case 0x8017: goto L8017;
    case 0x8019: goto L8019;
    case 0x801B: goto L801B;
    case 0x801C: goto L801C;
    case 0x801F: goto L801F;
    case 0x8021: goto L8021;
    case 0x8023: goto L8023;
    case 0x8027: goto L8027;
    case 0x802C: goto L802C;
    case 0x802D: goto L802D;
    case 0x802F: goto L802F;
    case 0x8032: goto L8032;
    case 0x8034: goto L8034;
    case 0x8037: goto L8037;
    case 0x803A: goto L803A;
    case 0x803E: goto L803E;
    case 0x8040: goto L8040;
    case 0x8045: goto L8045;
    case 0x8049: goto L8049;
    case 0x804E: goto L804E;
    case 0x8050: goto L8050;
    case 0x8055: goto L8055;
    case 0x8056: goto L8056;
    case 0x8057: goto L8057;
    case 0x805B: goto L805B;
    case 0x805F: goto L805F;
    case 0x8062: goto L8062;
    case 0x8064: goto L8064;
    case 0x8069: goto L8069;
    case 0x806D: goto L806D;
    case 0x806F: goto L806F;
    case 0x8070: goto L8070;
    case 0x8073: goto L8073;
    case 0x8075: goto L8075;
    case 0x8077: goto L8077;
    case 0x807B: goto L807B;
    case 0x807C: goto L807C;
    case 0x807F: goto L807F;
    case 0x8081: goto L8081;
    case 0x8082: goto L8082;
    case 0x8085: goto L8085;
    case 0x8087: goto L8087;
    case 0x808A: goto L808A;
    case 0x808D: goto L808D;
    case 0x8090: goto L8090;
    case 0x8094: goto L8094;
    case 0x8096: goto L8096;
    case 0x8097: goto L8097;
    case 0x8099: goto L8099;
    case 0x809B: goto L809B;
    case 0x809C: goto L809C;
    case 0x809E: goto L809E;
    case 0x809F: goto L809F;
    case 0x80A1: goto L80A1;
    case 0x80A4: goto L80A4;
    case 0x80A6: goto L80A6;
    case 0x80A8: goto L80A8;
    case 0x80AA: goto L80AA;
    case 0x80AC: goto L80AC;
    case 0x80AF: goto L80AF;
    case 0x80B1: goto L80B1;
    case 0x80B4: goto L80B4;
    case 0x80B6: goto L80B6;
    case 0x80B8: goto L80B8;
    case 0x80B9: goto L80B9;
    case 0x80BA: goto L80BA;
    case 0x80BB: goto L80BB;
    case 0x80BD: goto L80BD;
    case 0x80BF: goto L80BF;
    case 0x80C1: goto L80C1;
    case 0x80C5: goto L80C5;
    case 0x80C7: goto L80C7;
    case 0x80CA: goto L80CA;
    case 0x80CF: goto L80CF;
    case 0x80D0: goto L80D0;
    case 0x80D1: goto L80D1;
    case 0x80D4: goto L80D4;
    case 0x80D7: goto L80D7;
    case 0x80D9: goto L80D9;
    case 0x80DA: goto L80DA;
    case 0x80DB: goto L80DB;
    case 0x80DC: goto L80DC;
    case 0x80DD: goto L80DD;
    case 0x80DE: goto L80DE;
    case 0x80DF: goto L80DF;
    case 0x80E0: goto L80E0;
    case 0x80E1: goto L80E1;
    case 0x80E2: goto L80E2;
    case 0x80E5: goto L80E5;
    case 0x80E7: goto L80E7;
    case 0x80E8: goto L80E8;
    case 0x80EA: goto L80EA;
    case 0x80EE: goto L80EE;
    case 0x80F1: goto L80F1;
    case 0x80F6: goto L80F6;
    case 0x80F8: goto L80F8;
    case 0x80FB: goto L80FB;
    case 0x80FD: goto L80FD;
    case 0x80FE: goto L80FE;
    case 0x8102: goto L8102;
    case 0x8104: goto L8104;
    case 0x8105: goto L8105;
    case 0x8106: goto L8106;
    case 0x810A: goto L810A;
    case 0x810F: goto L810F;
    case 0x8110: goto L8110;
    case 0x8111: goto L8111;
    case 0x8115: goto L8115;
    case 0x811B: goto L811B;
    case 0x811D: goto L811D;
    case 0x811E: goto L811E;
    case 0x8120: goto L8120;
    case 0x8121: goto L8121;
    case 0x8123: goto L8123;
    case 0x8125: goto L8125;
    case 0x8127: goto L8127;
    case 0x812B: goto L812B;
    case 0x812D: goto L812D;
    case 0x8130: goto L8130;
    case 0x8132: goto L8132;
    case 0x8134: goto L8134;
    case 0x8138: goto L8138;
    case 0x813A: goto L813A;
    case 0x813D: goto L813D;
    case 0x813F: goto L813F;
    case 0x8141: goto L8141;
    case 0x8142: goto L8142;
    case 0x8145: goto L8145;
    case 0x8147: goto L8147;
    case 0x814B: goto L814B;
    case 0x814D: goto L814D;
    case 0x814E: goto L814E;
    case 0x8151: goto L8151;
    default: asm_bad_entry("PGCACHE.ASM", entry);
    }
L6D3C: /* L6D3C */
    /* 6D3C  int     3 */
    asm_halt_at(0x065C, 0x6D3C, "int 3, the debugger break");
L6D3D:
    /* 6D3D  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_7D3E  (+7D3E)
       do_fetchmap (opcode 0x3E): load a wall or floor texture into a bitmap slot. Operands: the
       slot, the texture number, the next opcode. Textures are 4 KB (64 x 64) and four share a 16 KB
       EMS page: page tmap_fpage + n / 4 is mapped into physical page 3 (int 67h, 44h, unless it is
       already there; an EMS error breaks with int 3) and the slot's word +4 gets the texture's
       segment, EmsBuff + 0C00h + (n & 3) * 100h.

       It then sets tmcolor (2696) from the first byte of the texture, but DOS loads DS from SI (the
       slot's address) where FM Towns reads the texture itself, so the byte it reads is not the
       texture's. Probably a DOS slip; what reads tmcolor has not been traced. */
L7D3E: /* _do_fetchmap */
    /* 7D3E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7D3F:
    /* 7D3F  mov     di,ax */
    DI = AX;
L7D41:
    /* 7D41  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7D42:
    /* 7D42  mov     bx,ax */
    BX = AX;
L7D44:
    /* 7D44  push    es */
    push16(asm_es);
L7D45:
    /* 7D45  push    ds */
    push16(asm_ds);
L7D46:
    /* 7D46  push    si */
    push16(SI);
L7D47:
    /* 7D47  mov     ax,bx */
    AX = BX;
L7D49:
    /* 7D49  shr     ax,2 */
    AX = (uint16_t)(AX >> 2);
L7D4C:
    /* 7D4C  add     al,byte ptr ds:[0E4CFh] */
    AL = (uint8_t)(AL + rb(pDS, 0xE4CF));
L7D50:
    /* 7D50  and     bx,3 */
    BX = (uint16_t)(BX & 0x3);
L7D53:
    /* 7D53  add     bx,0Ch */
    BX = (uint16_t)(BX + 0xC);
L7D56:
    /* 7D56  shl     bx,8 */
    BX = (uint16_t)(BX << 8);
L7D59:
    /* 7D59  mov     si,bx */
    SI = BX;
L7D5B:
    /* 7D5B  cmp     al,byte ptr ds:[0E4CEh] */
    sub8(AL, rb(pDS, 0xE4CE), 0);
L7D5F:
    /* 7D5F  je      short L7D77 */
    if (ZF) goto L7D77;
L7D61:
    /* 7D61  mov     byte ptr ds:[0E4CEh],al */
    wb(pDS, 0xE4CE, AL);
L7D64:
    /* 7D64  xor     bh,bh */
    BH = (uint8_t)(BH ^ BH);
L7D66:
    /* 7D66  mov     bl,al */
    BL = AL;
L7D68:
    /* 7D68  mov     ah,44h */
    AH = 0x44;
L7D6A:
    /* 7D6A  mov     al,3 */
    AL = 0x3;
L7D6C:
    /* 7D6C  mov     dx,word ptr ds:[0E4D4h] */
    DX = rw(pDS, 0xE4D4);
L7D70:
    /* 7D70  int     67h */
    asm_int(0x67);
L7D72:
    /* 7D72  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L7D74:
    /* 7D74  je      short L7D77 */
    if (ZF) goto L7D77;
L7D76:
    /* 7D76  int     3 */
    asm_halt_at(0x065C, 0x7D76, "int 3, the debugger break");
L7D77: /* L7D77 */
    /* 7D77  mov     ax,si */
    AX = SI;
L7D79:
    /* 7D79  mov     bx,seg seg052_519C */
    BX = (uint16_t)(0x4FAF + PORT_LOAD_SEG);
L7D7C:
    /* 7D7C  mov     es,bx */
    SET_ES(BX);
L7D7E:
    /* 7D7E  mov     si,0C802h */
    SI = 0xC802;
L7D81:
    /* 7D81  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L7D83:
    /* 7D83  inc     di */
    DI = (uint16_t)(DI + 1);
L7D84:
    /* 7D84  shl     di,2 */
    DI = (uint16_t)(DI << 2);
L7D87:
    /* 7D87  add     si,di */
    SI = (uint16_t)(SI + DI);
L7D89:
    /* 7D89  add     ax,word ptr ds:[0E4D2h] */
    AX = (uint16_t)(AX + rw(pDS, 0xE4D2));
L7D8D:
    /* 7D8D  mov     word ptr es:[si],ax */
    ww(pES, SI, AX);
L7D90:
    /* 7D90  mov     ds,si */
    SET_DS(SI);
L7D92:
    /* 7D92  xor     ah,ah */
    AH = logic8((uint8_t)(AH ^ AH));
L7D94:
    /* 7D94  mov     al,byte ptr ds:[0] */
    /* by hand: do_fetchmap's tmcolor: DS is the slot's address taken as a segment (a DOS slip, FINDINGS.md), so DOS reads upper memory here; nothing reads tmcolor, so the port stores 0 instead of reading a paragraph it has no memory for */
    AL = 0;
L7D97:
    /* 7D97  mov     word ptr es:[2696h],ax */
    ww(pES, 0x2696, AX);
L7D9B:
    /* 7D9B  pop     si */
    SI = pop16();
L7D9C:
    /* 7D9C  pop     ds */
    SET_DS(pop16());
L7D9D:
    /* 7D9D  pop     es */
    SET_ES(pop16());
L7D9E:
    /* 7D9E  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7D9F:
    /* 7D9F  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L7DA0:
    /* 7DA0  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_7DA4  (+7DA4)
       do_settmobj (opcode 0xC0): use an object's picture as the texture of bitmap slot 6 (C832, FM
       bm_number_6), for a 3D model drawn with an object's own art. Operands: the grs_off slot, an
       unused word, the next opcode. The picture is reached through seg004_0849_7EDE; the slot gets
       the picture's width (byte 1), its byte count (word 3) - 1, the segment of the 4 KB buffer
       _seg_5DFD, and (height - 1) shifted by 8 - log2(width) as the row mask; then the picture's
       pixels (from byte 5, word 3 of them) are copied into _seg_5DFD, where the mapper reads them.
       (The `lodsw` after the dispatch jump is never reached. L7E0A is do_uwobj's branch for
       animations.) */
L7DA4: /* _do_settmobj */
    /* 7DA4  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7DA5:
    /* 7DA5  mov     cx,ax */
    CX = AX;
L7DA7:
    /* 7DA7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7DA8:
    /* 7DA8  push    es */
    push16(asm_es);
L7DA9:
    /* 7DA9  push    ds */
    push16(asm_ds);
L7DAA:
    /* 7DAA  push    si */
    push16(SI);
L7DAB:
    /* 7DAB  push    ax */
    push16(AX);
L7DAC:
    /* 7DAC  mov     ax,cx */
    AX = CX;
L7DAE:
    /* 7DAE  mov     bx,0D049h */
    BX = 0xD049;
L7DB1:
    /* 7DB1  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L7DB3:
    /* 7DB3  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L7DB5:
    /* 7DB5  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L7DB7:
    /* 7DB7  call    _seg004_0849_7EDE */
    if ((c = asm_call(ASM_JMP(0x065C, 0x7EDE), 0x7DBA)) != 0) return c;
L7DBA:
    /* 7DBA  mov     bx,ax */
    BX = AX;
L7DBC:
    /* 7DBC  push    bx */
    push16(BX);
L7DBD:
    /* 7DBD  mov     di,0C832h */
    DI = 0xC832;
L7DC0:
    /* 7DC0  mov     ax,seg seg052_519C */
    AX = (uint16_t)(0x4FAF + PORT_LOAD_SEG);
L7DC3:
    /* 7DC3  mov     es,ax */
    SET_ES(AX);
L7DC5:
    /* 7DC5  mov     ds,bx */
    SET_DS(BX);
L7DC7:
    /* 7DC7  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L7DC9:
    /* 7DC9  mov     al,byte ptr ds:[1] */
    AL = rb(pDS, 0x1);
L7DCC:
    /* 7DCC  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L7DCD:
    /* 7DCD  mov     ax,word ptr ds:[3] */
    AX = rw(pDS, 0x3);
L7DD0:
    /* 7DD0  mov     bx,ax */
    BX = AX;
L7DD2:
    /* 7DD2  dec     ax */
    AX = (uint16_t)(AX - 1);
L7DD3:
    /* 7DD3  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L7DD4:
    /* 7DD4  mov     ax,seg _seg_5DFD */
    AX = (uint16_t)(0x5DFD + PORT_LOAD_SEG);
L7DD7:
    /* 7DD7  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L7DD8:
    /* 7DD8  mov     al,byte ptr ds:[1] */
    AL = rb(pDS, 0x1);
L7DDB:
    /* 7DDB  mov     cl,8 */
    CL = 0x8;
L7DDD: /* L7DDD */
    /* 7DDD  dec     cl */
    CL = (uint8_t)(CL - 1);
L7DDF:
    /* 7DDF  shl     al,1 */
    AL = shl8(AL, 1);
L7DE1:
    /* 7DE1  jae     L7DDD */
    if (!CF) goto L7DDD;
L7DE3:
    /* 7DE3  mov     al,byte ptr ds:[2] */
    AL = rb(pDS, 0x2);
L7DE6:
    /* 7DE6  dec     al */
    AL = (uint8_t)(AL - 1);
L7DE8:
    /* 7DE8  xor     ah,ah */
    AH = logic8((uint8_t)(AH ^ AH));
L7DEA:
    /* 7DEA  shl     ax,cl */
    AX = shl16(AX, CL);
L7DEC:
    /* 7DEC  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L7DED:
    /* 7DED  mov     cx,bx */
    CX = BX;
L7DEF:
    /* 7DEF  pop     dx */
    DX = pop16();
L7DF0:
    /* 7DF0  pop     bx */
    BX = pop16();
L7DF1:
    /* 7DF1  mov     si,5 */
    SI = 0x5;
L7DF4:
    /* 7DF4  mov     ds,dx */
    SET_DS(DX);
L7DF6:
    /* 7DF6  mov     di,0 */
    DI = 0x0;
L7DF9:
    /* 7DF9  mov     ax,seg _seg_5DFD */
    AX = (uint16_t)(0x5DFD + PORT_LOAD_SEG);
L7DFC:
    /* 7DFC  mov     es,ax */
    SET_ES(AX);
L7DFE:
    /* 7DFE  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L7E00:
    /* 7E00  pop     si */
    SI = pop16();
L7E01:
    /* 7E01  pop     ds */
    SET_DS(pop16());
L7E02:
    /* 7E02  pop     es */
    SET_ES(pop16());
L7E03:
    /* 7E03  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7E04:
    /* 7E04  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L7E05:
    /* 7E05  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));
L7E09:
    /* 7E09  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7E0A: /* L7E0A */
    /* 7E0A  sub     ax,1C0h */
    AX = (uint16_t)(AX - 0x1C0);
L7E0D:
    /* 7E0D  add     ax,word ptr ds:[0D030h] */
    AX = (uint16_t)(AX + rw(pDS, 0xD030));
L7E11:
    /* 7E11  jmp     short L7E26 */
    goto L7E26;

    /* seg004_0849_7E13  (+7E13)
       do_uwobj (opcode 0x3A): draw an object's sprite. Operands: the object type, the shade (dh for
       the decoder), the point. Types below 1C0h take their picture slot from obj_tab (bits 0-9);
       1C0h and up are animations, slot first_anim + type - 1C0h. The picture (byte 0 the format, 1
       and 2 width and height, 3 its auxiliary palette, pixels from 4) is decoded through uncmp_tab
       with the 16-byte palette from LOADGR's _Palettes (palette numbers from 20h are folded down: (n
       - 8) * 2), the decoded bitmap's address and size go to the sprite record at C856 (FM
       sbm_inventory), and it is drawn by in_scalebm (TMAPOPS.ASM) with record offset 1Ch. */
L7E13: /* _do_uwobj */
    /* 7E13  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7E14:
    /* 7E14  cmp     ax,1C0h */
    sub16(AX, 0x1C0, 0);
L7E17:
    /* 7E17  jge     L7E0A */
    if (SF == OF) goto L7E0A;
L7E19:
    /* 7E19  mov     bx,0D849h */
    BX = 0xD849;
L7E1C:
    /* 7E1C  shl     ax,2 */
    AX = (uint16_t)(AX << 2);
L7E1F:
    /* 7E1F  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L7E21:
    /* 7E21  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L7E23:
    /* 7E23  and     ax,3FFh */
    AX = (uint16_t)(AX & 0x3FF);
L7E26: /* L7E26 */
    /* 7E26  mov     bx,0D049h */
    BX = 0xD049;
L7E29:
    /* 7E29  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L7E2B:
    /* 7E2B  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L7E2D:
    /* 7E2D  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L7E2F:
    /* 7E2F  call    _seg004_0849_7EDE */
    if ((c = asm_call(ASM_JMP(0x065C, 0x7EDE), 0x7E32)) != 0) return c;
L7E32:
    /* 7E32  mov     bx,ax */
    BX = AX;
L7E34:
    /* 7E34  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7E35:
    /* 7E35  mov     ch,al */
    CH = AL;
L7E37:
    /* 7E37  mov     dx,bx */
    DX = BX;
L7E39:
    /* 7E39  push    si */
    push16(SI);
L7E3A:
    /* 7E3A  push    bx */
    push16(BX);
L7E3B:
    /* 7E3B  mov     ds,bx */
    SET_DS(BX);
L7E3D:
    /* 7E3D  xor     bh,bh */
    BH = (uint8_t)(BH ^ BH);
L7E3F:
    /* 7E3F  mov     bl,byte ptr ds:[0] */
    BL = rb(pDS, 0x0);
L7E43:
    /* 7E43  mov     al,byte ptr ds:[3] */
    AL = rb(pDS, 0x3);
L7E46:
    /* 7E46  cmp     al,20h */
    sub8(AL, 0x20, 0);
L7E48:
    /* 7E48  jb      short L7E4E */
    if (CF) goto L7E4E;
L7E4A:
    /* 7E4A  sub     al,8 */
    AL = (uint8_t)(AL - 0x8);
L7E4C:
    /* 7E4C  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L7E4E: /* L7E4E */
    /* 7E4E  mov     cl,4 */
    CL = 0x4;
L7E50:
    /* 7E50  xor     ah,ah */
    AH = logic8((uint8_t)(AH ^ AH));
L7E52:
    /* 7E52  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L7E54:
    /* 7E54  mov     si,offset DGROUP:_Palettes */
    /* by hand: do_uwobj: _Palettes is LOADGR.C's array, a C object in the port, not a DGROUP offset */
    SI = (uint16_t)port_fp_off(Palettes);
L7E57:
    /* 7E57  add     si,ax */
    SI = (uint16_t)(SI + AX);
L7E59:
    /* 7E59  mov     ax,DGROUP */
    /* by hand: do_uwobj: DGROUP's segment, here the paragraph the port's map gives _Palettes */
    AX = (uint16_t)port_fp_seg(Palettes);
L7E5C:
    /* 7E5C  mov     ds,ax */
    SET_DS(AX);
L7E5E:
    /* 7E5E  mov     ax,dx */
    AX = DX;
L7E60:
    /* 7E60  mov     dh,ch */
    DH = CH;
L7E62:
    /* 7E62  mov     bp,4 */
    BP = 0x4;
L7E65:
    /* 7E65  call    word ptr cs:uncmp_tab[bx] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(CODE004, BX + 0x6D30)), 0x7E6A)) != 0) return c;
L7E6A:
    /* 7E6A  pop     ds */
    SET_DS(pop16());
L7E6B:
    /* 7E6B  mov     di,0C856h */
    DI = 0xC856;
L7E6E:
    /* 7E6E  mov     bx,seg seg052_519C */
    BX = (uint16_t)(0x4FAF + PORT_LOAD_SEG);
L7E71:
    /* 7E71  mov     es,bx */
    SET_ES(BX);
L7E73:
    /* 7E73  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L7E74:
    /* 7E74  mov     si,1 */
    SI = 0x1;
L7E77:
    /* 7E77  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L7E78:
    /* 7E78  inc     di */
    DI = (uint16_t)(DI + 1);
L7E79:
    /* 7E79  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L7E7A:
    /* 7E7A  mov     ax,seg seg052_519C */
    AX = (uint16_t)(0x4FAF + PORT_LOAD_SEG);
L7E7D:
    /* 7E7D  mov     ds,ax */
    SET_DS(AX);
L7E7F:
    /* 7E7F  pop     si */
    SI = pop16();
L7E80:
    /* 7E80  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7E81:
    /* 7E81  mov     di,ax */
    DI = AX;
L7E83:
    /* 7E83  add     di,14D0h */
    DI = add16(DI, 0x14D0, 0);
L7E87:
    /* 7E87  mov     ax,1Ch */
    AX = 0x1C;
L7E8A:
    /* 7E8A  db      0EAh */
    return ASM_JMP(0x065C, 0x3FA7);

    /* seg004_0849_7E8F  (+7E8F)
       do_uwcrit (opcode 0x5A): draw a critter's animation frame. Operands: the critter type (index
       into grs_3dinf), a flag (nonzero: palette 3 in place of the critter's own; probably the
       shading used for a critter being hit or a special state, not checked), the shade, the frame
       number, the point. A type with no critter map (0FFh) is skipped. The frame picks the page: the
       first of the critter map's eight CmapFrm bytes not below it. load_cpg makes sure the page is
       in the cache and mapped; parse_cpg decodes and draws the frame. */
L7E8F: /* _do_uwcrit */
    /* 7E8F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7E90:
    /* 7E90  mov     cx,ax */
    CX = AX;
L7E92:
    /* 7E92  mov     bx,0E049h */
    BX = 0xE049;
L7E95:
    /* 7E95  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L7E97:
    /* 7E97  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L7E99:
    /* 7E99  mov     al,byte ptr [bx] */
    AL = rb(pDS, BX);
L7E9B:
    /* 7E9B  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L7E9D:
    /* 7E9D  cmp     ax,0FFh */
    sub16(AX, 0xFF, 0);
L7EA0:
    /* 7EA0  je      short L7ED5 */
    if (ZF) goto L7ED5;
L7EA2:
    /* 7EA2  mov     dl,byte ptr [bx+1] */
    DL = rb(pDS, BX + 0x1);
L7EA5:
    /* 7EA5  mov     bx,0E2C9h */
    BX = 0xE2C9;
L7EA8:
    /* 7EA8  mov     bp,ax */
    BP = AX;
L7EAA:
    /* 7EAA  shl     ax,3 */
    AX = (uint16_t)(AX << 3);
L7EAD:
    /* 7EAD  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L7EAF:
    /* 7EAF  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7EB0:
    /* 7EB0  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L7EB2:
    /* 7EB2  je      short L7EB6 */
    if (ZF) goto L7EB6;
L7EB4:
    /* 7EB4  mov     dl,3 */
    DL = 0x3;
L7EB6: /* L7EB6 */
    /* 7EB6  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7EB7:
    /* 7EB7  mov     dh,al */
    DH = AL;
L7EB9:
    /* 7EB9  push    dx */
    push16(DX);
L7EBA:
    /* 7EBA  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7EBB:
    /* 7EBB  mov     cx,7 */
    CX = 0x7;
L7EBE: /* L7EBE */
    /* 7EBE  cmp     al,byte ptr [bx] */
    sub8(AL, rb(pDS, BX), 0);
L7EC0:
    /* 7EC0  jbe     short L7EC5 */
    if (CF || ZF) goto L7EC5;
L7EC2:
    /* 7EC2  inc     bx */
    BX = (uint16_t)(BX + 1);
L7EC3:
    /* 7EC3  loop    L7EBE */
    if (--CX) goto L7EBE;
L7EC5: /* L7EC5 */
    /* 7EC5  mov     bx,7 */
    BX = 0x7;
L7EC8:
    /* 7EC8  sub     bx,cx */
    BX = (uint16_t)(BX - CX);
L7ECA:
    /* 7ECA  mov     dx,bp */
    DX = BP;
L7ECC:
    /* 7ECC  mov     bp,ax */
    BP = AX;
L7ECE:
    /* 7ECE  call    _load_cpg */
    if ((c = asm_call(ASM_JMP(0x065C, 0x7F8D), 0x7ED1)) != 0) return c;
L7ED1:
    /* 7ED1  pop     dx */
    DX = pop16();
L7ED2:
    /* 7ED2  jmp     _parse_cpg */
    goto L809F;
L7ED5: /* L7ED5 */
    /* 7ED5  add     si,8 */
    SI = add16(SI, 0x8, 0);
L7ED8:
    /* 7ED8  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7ED9:
    /* 7ED9  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L7EDA:
    /* 7EDA  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_7EDE  (+7EDE)
       seg004_0849_7EDE (DOS only; FM Towns has flat memory): turn a grs_off word into a segment. AX
       = the word: bits 12-15 the EMS logical page, mapped into physical page 2 unless it is the one
       there (obj_inpage1; an EMS error breaks with int 3), bits 0-9 the paragraph within the 16 KB
       page. Out: AX = EmsBuff + 800h + paragraph. Changes BX, CL, DX, DI. GRSPIC.C's C code does the
       same for the 2D side. */
L7EDE: /* _seg004_0849_7EDE */
    /* 7EDE  mov     di,ax */
    DI = AX;
L7EE0:
    /* 7EE0  mov     cl,0Ch */
    CL = 0xC;
L7EE2:
    /* 7EE2  shr     ax,cl */
    AX = (uint16_t)(AX >> (CL & 31));
L7EE4:
    /* 7EE4  cmp     al,byte ptr ds:[0E4D1h] */
    sub8(AL, rb(pDS, 0xE4D1), 0);
L7EE8:
    /* 7EE8  je      short L7EFD */
    if (ZF) goto L7EFD;
L7EEA:
    /* 7EEA  mov     byte ptr ds:[0E4D1h],al */
    wb(pDS, 0xE4D1, AL);
L7EED:
    /* 7EED  mov     bx,ax */
    BX = AX;
L7EEF:
    /* 7EEF  mov     ah,44h */
    AH = 0x44;
L7EF1:
    /* 7EF1  mov     al,2 */
    AL = 0x2;
L7EF3:
    /* 7EF3  mov     dx,word ptr ds:[0E4D4h] */
    DX = rw(pDS, 0xE4D4);
L7EF7:
    /* 7EF7  int     67h */
    asm_int(0x67);
L7EF9:
    /* 7EF9  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L7EFB:
    /* 7EFB  jne     short L7F0A */
    if (!ZF) goto L7F0A;
L7EFD: /* L7EFD */
    /* 7EFD  mov     ax,di */
    AX = DI;
L7EFF:
    /* 7EFF  and     ax,3FFh */
    AX = (uint16_t)(AX & 0x3FF);
L7F02:
    /* 7F02  add     ax,word ptr ds:[0E4D2h] */
    AX = (uint16_t)(AX + rw(pDS, 0xE4D2));
L7F06:
    /* 7F06  add     ax,800h */
    AX = add16(AX, 0x800, 0);
L7F09:
    /* 7F09  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L7F0A: /* L7F0A */
    /* 7F0A  int     3 */
    asm_halt_at(0x065C, 0x7F0A, "int 3, the debugger break");
L7F0B:
    /* 7F0B  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_7F0C  (+7F0C)
       seg004_0849_7F0C: far entry to the decoders for C (cFrmtoRaw), BX = format * 2, other
       registers as the decoder wants (EXPAND.ASM). */
L7F0C: /* _seg004_0849_7F0C */
    /* 7F0C  call    word ptr cs:uncmp_tab[bx] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(CODE004, BX + 0x6D30)), 0x7F11)) != 0) return c;
L7F11:
    /* 7F11  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* update_cache  (+7F12): FM update_cache. It saves DS in BX and then reuses BX, so it
       returns with DS = 0E3C9h; cRender reloads DS afterwards.
       update_cache: called by cRender after each frame. Ages every loaded critter page (CmapCache
       entries 1 to 0FCh not 0FFh) by one, stopping at 1, so the least recently used page has the
       lowest count. */
L7F12: /* _update_cache */
    /* 7F12  mov     bx,ds */
    BX = asm_ds;
L7F14:
    /* 7F14  mov     ax,seg _CmapCache */
    AX = (uint16_t)(0x4FAF + PORT_LOAD_SEG);
L7F17:
    /* 7F17  mov     ds,ax */
    SET_DS(AX);
L7F19:
    /* 7F19  mov     di,0FCh */
    DI = 0xFC;
L7F1C:
    /* 7F1C  mov     bx,0E3C9h */
    BX = 0xE3C9;
L7F1F: /* L7F1F */
    /* 7F1F  mov     al,byte ptr [bx+di] */
    AL = rb(pDS, BX + DI);
L7F21:
    /* 7F21  cmp     al,0FFh */
    sub8(AL, 0xFF, 0);
L7F23:
    /* 7F23  je      short L7F2B */
    if (ZF) goto L7F2B;
L7F25:
    /* 7F25  dec     al */
    AL = dec8(AL);
L7F27:
    /* 7F27  je      short L7F2B */
    if (ZF) goto L7F2B;
L7F29:
    /* 7F29  mov     byte ptr [bx+di],al */
    wb(pDS, BX + DI, AL);
L7F2B: /* L7F2B */
    /* 7F2B  dec     di */
    DI = dec16(DI);
L7F2C:
    /* 7F2C  jne     L7F1F */
    if (!ZF) goto L7F1F;
L7F2E:
    /* 7F2E  mov     ds,bx */
    SET_DS(BX);
L7F30:
    /* 7F30  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg004_0849_7F31  (+7F31)
       unload_cpg: entered from load_cpg when no cache slot is free, BX = the cache index wanted.
       Finds the loaded page with the lowest age, takes its slot (marks it unloaded), gives the slot
       to BX and joins load_cpg to read the file. If nothing is loaded either, it drops the critter:
       discards load_cpg's return address and the pushed shade, skips the remaining operand and
       dispatches the next opcode. */
L7F31: /* _unload_cpg */
    /* 7F31  mov     di,0FFh */
    DI = 0xFF;
L7F34:
    /* 7F34  mov     dl,0FFh */
    DL = 0xFF;
L7F36:
    /* 7F36  mov     dh,0FFh */
    DH = 0xFF;
L7F38: /* L7F38 */
    /* 7F38  mov     al,byte ptr [di-1C37h] */
    AL = rb(pDS, DI + 0xE3C9);
L7F3C:
    /* 7F3C  cmp     al,dl */
    sub8(AL, DL, 0);
L7F3E:
    /* 7F3E  jae     short L7F46 */
    if (!CF) goto L7F46;
L7F40:
    /* 7F40  mov     dl,al */
    DL = AL;
L7F42:
    /* 7F42  mov     ax,di */
    AX = DI;
L7F44:
    /* 7F44  mov     dh,al */
    DH = AL;
L7F46: /* L7F46 */
    /* 7F46  dec     di */
    DI = dec16(DI);
L7F47:
    /* 7F47  jne     L7F38 */
    if (!ZF) goto L7F38;
L7F49:
    /* 7F49  cmp     dh,0FFh */
    sub8(DH, 0xFF, 0);
L7F4C:
    /* 7F4C  je      short L7F81 */
    if (ZF) goto L7F81;
L7F4E:
    /* 7F4E  mov     di,0E1C9h */
    DI = 0xE1C9;
L7F51:
    /* 7F51  mov     ax,bx */
    AX = BX;
L7F53:
    /* 7F53  xchg    dh,dl */
    { uint8_t t_ = DL;
    DL = DH;
    DH = t_; }
L7F55:
    /* 7F55  xor     dh,dh */
    DH = (uint8_t)(DH ^ DH);
L7F57:
    /* 7F57  mov     bx,dx */
    BX = DX;
L7F59:
    /* 7F59  mov     byte ptr [bx-1C37h],0FFh */
    wb(pDS, BX + 0xE3C9, 0xFF);
L7F5E:
    /* 7F5E  mov     cl,byte ptr [bx-1F37h] */
    CL = rb(pDS, BX + 0xE0C9);
L7F62:
    /* 7F62  mov     byte ptr [bx-1F37h],0FFh */
    wb(pDS, BX + 0xE0C9, 0xFF);
L7F67:
    /* 7F67  mov     bl,cl */
    BL = CL;
L7F69:
    /* 7F69  mov     byte ptr [bx-1E37h],al */
    wb(pDS, BX + 0xE1C9, AL);
L7F6D:
    /* 7F6D  mov     bx,ax */
    BX = AX;
L7F6F:
    /* 7F6F  mov     byte ptr [bx-1F37h],cl */
    wb(pDS, BX + 0xE0C9, CL);
L7F73:
    /* 7F73  xor     ch,ch */
    CH = (uint8_t)(CH ^ CH);
L7F75:
    /* 7F75  mov     bx,ax */
    BX = AX;
L7F77:
    /* 7F77  mov     dx,ax */
    DX = AX;
L7F79:
    /* 7F79  and     ax,7 */
    AX = (uint16_t)(AX & 0x7);
L7F7C:
    /* 7F7C  shr     dx,3 */
    DX = (uint16_t)(DX >> 3);
L7F7F:
    /* 7F7F  jmp     short L7FC2 */
    goto L7FC2;
L7F81: /* L7F81 */
    /* 7F81  add     sp,4 */
    SP = (uint16_t)(SP + 0x4);
L7F84:
    /* 7F84  add     si,4 */
    SI = add16(SI, 0x4, 0);
L7F87:
    /* 7F87  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L7F88:
    /* 7F88  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L7F89:
    /* 7F89  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_7F8D  (+7F8D)
       load_cpg: make critter page (DX = critter map, BX = page 0-7) present and mapped. If CmaptoPg
       has a slot for it, lcpg_pghere maps it. Otherwise it takes a free slot (PgtoCmap 0FEh, or
       unload_cpg's), builds the file name crit\crMM.0P from the map number in two octal digits and
       the page, opens it, maps the slot and reads up to 7FFFh bytes into the page frame, and sets
       its age to 10h. Out: the slot mapped into physical pages 0 and 1. If the file does not open or
       read, it breaks with int 2, frees the slot and skips the critter (dispatching the next
       opcode). */
L7F8D: /* _load_cpg */
    /* 7F8D  shl     dx,3 */
    DX = (uint16_t)(DX << 3);
L7F90:
    /* 7F90  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L7F92:
    /* 7F92  mov     al,byte ptr [bx-1F37h] */
    AL = rb(pDS, BX + 0xE0C9);
L7F96:
    /* 7F96  cmp     al,0FFh */
    sub8(AL, 0xFF, 0);
L7F98:
    /* 7F98  je      short L7F9D */
    if (ZF) goto L7F9D;
L7F9A:
    /* 7F9A  jmp     _lcpg_pghere */
    goto L805B;
L7F9D: /* L7F9D */
    /* 7F9D  mov     di,0E1C9h */
    DI = 0xE1C9;
L7FA0:
    /* 7FA0  mov     al,0FEh */
    AL = 0xFE;
L7FA2:
    /* 7FA2  mov     cx,ds */
    CX = asm_ds;
L7FA4:
    /* 7FA4  mov     es,cx */
    SET_ES(CX);
L7FA6:
    /* 7FA6  mov     cx,word ptr ds:[0E4CBh] */
    CX = rw(pDS, 0xE4CB);
L7FAA:
    /* 7FAA  repne scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (ZF) break; }
L7FAC:
    /* 7FAC  jne     _unload_cpg */
    if (!ZF) goto L7F31;
L7FAE: /* foundfreepage */
    /* 7FAE  dec     di */
    DI = (uint16_t)(DI - 1);
L7FAF:
    /* 7FAF  mov     ax,bx */
    AX = BX;
L7FB1:
    /* 7FB1  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L7FB2:
    /* 7FB2  sub     ax,dx */
    AX = (uint16_t)(AX - DX);
L7FB4:
    /* 7FB4  shr     dx,3 */
    DX = (uint16_t)(DX >> 3);
L7FB7:
    /* 7FB7  neg     cx */
    CX = (uint16_t)-CX;
L7FB9:
    /* 7FB9  add     cx,word ptr ds:[0E4CBh] */
    CX = (uint16_t)(CX + rw(pDS, 0xE4CB));
L7FBD:
    /* 7FBD  dec     cx */
    CX = (uint16_t)(CX - 1);
L7FBE:
    /* 7FBE  mov     byte ptr [bx-1F37h],cl */
    wb(pDS, BX + 0xE0C9, CL);
L7FC2: /* L7FC2 */
    /* 7FC2  mov     word ptr ds:[0D047h],bx */
    ww(pDS, 0xD047, BX);
L7FC6:
    /* 7FC6  push    cx */
    push16(CX);
L7FC7:
    /* 7FC7  mov     ch,al */
    CH = AL;
L7FC9:
    /* 7FC9  mov     di,0D03Ah */
    DI = 0xD03A;
L7FCC:
    /* 7FCC  add     di,7 */
    DI = add16(DI, 0x7, 0);
L7FCF:
    /* 7FCF  mov     ax,dx */
    AX = DX;
L7FD1:
    /* 7FD1  mov     cl,3 */
    CL = 0x3;
L7FD3:
    /* 7FD3  shr     ax,cl */
    AX = (uint16_t)(AX >> (CL & 31));
L7FD5:
    /* 7FD5  add     al,30h */
    AL = (uint8_t)(AL + 0x30);
L7FD7:
    /* 7FD7  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L7FD8:
    /* 7FD8  mov     ax,dx */
    AX = DX;
L7FDA:
    /* 7FDA  and     ax,7 */
    AX = (uint16_t)(AX & 0x7);
L7FDD:
    /* 7FDD  add     al,30h */
    AL = add8(AL, 0x30, 0);
L7FDF:
    /* 7FDF  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L7FE0:
    /* 7FE0  inc     di */
    DI = inc16(DI);
L7FE1:
    /* 7FE1  mov     al,ch */
    AL = CH;
L7FE3:
    /* 7FE3  shr     al,cl */
    AL = (uint8_t)(AL >> (CL & 31));
L7FE5:
    /* 7FE5  add     al,30h */
    AL = (uint8_t)(AL + 0x30);
L7FE7:
    /* 7FE7  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L7FE8:
    /* 7FE8  mov     al,ch */
    AL = CH;
L7FEA:
    /* 7FEA  and     al,7 */
    AL = (uint8_t)(AL & 0x7);
L7FEC:
    /* 7FEC  add     al,30h */
    AL = (uint8_t)(AL + 0x30);
L7FEE:
    /* 7FEE  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L7FEF:
    /* 7FEF  mov     dx,0D03Ah */
    DX = 0xD03A;
L7FF2:
    /* 7FF2  mov     ax,DOS_OPEN shl 8 */
    AX = 0x3D00;
L7FF5:
    /* 7FF5  int     21h */
    asm_int(0x21);
L7FF7:
    /* 7FF7  jb      short L802D */
    if (CF) goto L802D;
L7FF9:
    /* 7FF9  mov     dx,ax */
    DX = AX;
L7FFB:
    /* 7FFB  pop     ax */
    AX = pop16();
L7FFC:
    /* 7FFC  push    dx */
    push16(DX);
L7FFD:
    /* 7FFD  call    _lcpg_pghere */
    if ((c = asm_call(ASM_JMP(0x065C, 0x805B), 0x8000)) != 0) return c;
L8000:
    /* 8000  pop     bx */
    BX = pop16();
L8001:
    /* 8001  mov     cx,7FFFh */
    CX = 0x7FFF;
L8004:
    /* 8004  mov     dx,0 */
    DX = 0x0;
L8007:
    /* 8007  mov     ax,word ptr ds:[0E4D2h] */
    AX = rw(pDS, 0xE4D2);
L800A:
    /* 800A  mov     ds,ax */
    SET_DS(AX);
L800C:
    /* 800C  mov     ax,DOS_READ shl 8 */
    AX = 0x3F00;
L800F:
    /* 800F  int     21h */
    asm_int(0x21);
L8011:
    /* 8011  lahf */
    AH = (uint8_t)(SF << 7 | ZF << 6 | 2 | CF);
L8012:
    /* 8012  mov     cl,ah */
    CL = AH;
L8014:
    /* 8014  mov     ax,DOS_CLOSE shl 8 */
    AX = 0x3E00;
L8017:
    /* 8017  int     21h */
    asm_int(0x21);
L8019:
    /* 8019  mov     ah,cl */
    AH = CL;
L801B:
    /* 801B  sahf */
    CF = AH & 1; ZF = AH >> 6 & 1; SF = AH >> 7 & 1;
L801C:
    /* 801C  mov     ax,seg seg052_519C */
    AX = (uint16_t)(0x4FAF + PORT_LOAD_SEG);
L801F:
    /* 801F  mov     ds,ax */
    SET_DS(AX);
L8021:
    /* 8021  jb      short L8032 */
    if (CF) goto L8032;
L8023:
    /* 8023  mov     bx,word ptr ds:[0D047h] */
    BX = rw(pDS, 0xD047);
L8027:
    /* 8027  mov     byte ptr [bx-1C37h],10h */
    wb(pDS, BX + 0xE3C9, 0x10);
L802C:
    /* 802C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L802D: /* L802D */
    /* 802D  int     2 */
    asm_halt_at(0x065C, 0x802D, "int 2h, the debugger break");
L802F:
    /* 802F  add     sp,2 */
    SP = (uint16_t)(SP + 0x2);
L8032: /* L8032 */
    /* 8032  int     2 */
    asm_halt_at(0x065C, 0x8032, "int 2h, the debugger break");
L8034:
    /* 8034  add     si,2 */
    SI = (uint16_t)(SI + 0x2);
L8037:
    /* 8037  add     sp,4 */
    SP = (uint16_t)(SP + 0x4);
L803A:
    /* 803A  mov     bx,word ptr ds:[0D047h] */
    BX = rw(pDS, 0xD047);
L803E:
    /* 803E  xor     ch,ch */
    CH = logic8((uint8_t)(CH ^ CH));
L8040:
    /* 8040  mov     byte ptr [bx-1C37h],0FFh */
    wb(pDS, BX + 0xE3C9, 0xFF);
L8045:
    /* 8045  mov     cl,byte ptr [bx-1F37h] */
    CL = rb(pDS, BX + 0xE0C9);
L8049:
    /* 8049  mov     byte ptr [bx-1F37h],0FFh */
    wb(pDS, BX + 0xE0C9, 0xFF);
L804E:
    /* 804E  mov     bl,cl */
    BL = CL;
L8050:
    /* 8050  mov     byte ptr [bx-1E37h],0FEh */
    wb(pDS, BX + 0xE1C9, 0xFE);
L8055:
    /* 8055  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L8056:
    /* 8056  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L8057:
    /* 8057  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_805B  (+805B)
       lcpg_pghere: AL = cache slot, BX = cache index. Adds 2 to the page's age (up to 0F0h) and,
       unless the slot is already mapped (crit_inpage), maps its two EMS pages (crit_fpage + slot * 2
       and the next) into physical pages 0 and 1 with int 67h function 50h. An EMS error breaks with
       int 2. */
L805B: /* _lcpg_pghere */
    /* 805B  mov     dl,byte ptr [bx-1C37h] */
    DL = rb(pDS, BX + 0xE3C9);
L805F:
    /* 805F  cmp     dl,0F0h */
    sub8(DL, 0xF0, 0);
L8062:
    /* 8062  jae     short L8069 */
    if (!CF) goto L8069;
L8064:
    /* 8064  add     byte ptr [bx-1C37h],2 */
    wb(pDS, BX + 0xE3C9, (uint8_t)(rb(pDS, BX + 0xE3C9) + 0x2));
L8069: /* L8069 */
    /* 8069  cmp     al,byte ptr ds:[0E4CDh] */
    sub8(AL, rb(pDS, 0xE4CD), 0);
L806D:
    /* 806D  jne     short lcpg_swap */
    if (!ZF) goto L8070;
L806F:
    /* 806F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L8070: /* lcpg_swap */
    /* 8070  mov     byte ptr ds:[0E4CDh],al */
    wb(pDS, 0xE4CD, AL);
L8073:
    /* 8073  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L8075:
    /* 8075  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L8077:
    /* 8077  add     ax,word ptr ds:[0E4C9h] */
    AX = (uint16_t)(AX + rw(pDS, 0xE4C9));
L807B:
    /* 807B  push    si */
    push16(SI);
L807C:
    /* 807C  mov     si,0D032h */
    SI = 0xD032;
L807F:
    /* 807F  mov     word ptr [si],ax */
    ww(pDS, SI, AX);
L8081:
    /* 8081  inc     ax */
    AX = (uint16_t)(AX + 1);
L8082:
    /* 8082  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L8085:
    /* 8085  mov     word ptr [si],ax */
    ww(pDS, SI, AX);
L8087:
    /* 8087  sub     si,4 */
    SI = (uint16_t)(SI - 0x4);
L808A:
    /* 808A  mov     cx,2 */
    CX = 0x2;
L808D:
    /* 808D  mov     ax,EMS_MAP_PAGES shl 8 */
    AX = 0x5000;
L8090:
    /* 8090  mov     dx,word ptr ds:[0E4D4h] */
    DX = rw(pDS, 0xE4D4);
L8094:
    /* 8094  int     67h */
    asm_int(0x67);
L8096:
    /* 8096  pop     si */
    SI = pop16();
L8097:
    /* 8097  or      ah,ah */
    AH = logic8((uint8_t)(AH | AH));
L8099:
    /* 8099  jne     short L809C */
    if (!ZF) goto L809C;
L809B:
    /* 809B  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L809C: /* L809C */
    /* 809C  int     2 */
    asm_halt_at(0x065C, 0x809C, "int 2h, the debugger break");
L809E:
    /* 809E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_809F  (+809F)
       parse_cpg: decode and draw a frame from the mapped critter page. In: BP = frame number, DL =
       the palette, DH = the shade. The page holds 32-byte palettes from offset 0
       (palette n at n * 32) and a word per frame at 80h giving the frame's offset; a frame is width,
       height, hot spot x and y (bytes 0-3), the format (byte 4) and the pixels. The decoded bitmap
       goes to the sprite record at C848 (FM sbm_critter) and in_scalebm draws it at the operand
       point with record offset 0Eh. */
L809F: /* _parse_cpg */
    /* 809F  mov     bx,bp */
    BX = BP;
L80A1:
    /* 80A1  mov     ax,word ptr ds:[0E4D2h] */
    AX = rw(pDS, 0xE4D2);
L80A4:
    /* 80A4  mov     ds,ax */
    SET_DS(AX);
L80A6:
    /* 80A6  xor     ah,ah */
    AH = logic8((uint8_t)(AH ^ AH));
L80A8:
    /* 80A8  mov     al,dl */
    AL = DL;
L80AA:
    /* 80AA  mov     bp,ax */
    BP = AX;
L80AC:
    /* 80AC  mov     cx,5 */
    CX = 0x5;
L80AF:
    /* 80AF  shl     bp,cl */
    BP = (uint16_t)(BP << (CL & 31));
L80B1:
    /* 80B1  mov     di,80h */
    DI = 0x80;
L80B4:
    /* 80B4  shl     bx,1 */
    BX = (uint16_t)(BX << 1);
L80B6:
    /* 80B6  mov     cx,word ptr [bx+di] */
    CX = rw(pDS, BX + DI);
L80B8:
    /* 80B8  push    si */
    push16(SI);
L80B9:
    /* 80B9  push    ds */
    push16(asm_ds);
L80BA:
    /* 80BA  push    cx */
    push16(CX);
L80BB:
    /* 80BB  mov     si,bp */
    SI = BP;
L80BD:
    /* 80BD  mov     ax,ds */
    AX = asm_ds;
L80BF:
    /* 80BF  mov     bp,cx */
    BP = CX;
L80C1:
    /* 80C1  mov     bl,byte ptr ds:[bp+4] */
    BL = rb(pDS, BP + 0x4);
L80C5:
    /* 80C5  xor     bh,bh */
    BH = (uint8_t)(BH ^ BH);
L80C7:
    /* 80C7  add     bp,5 */
    BP = (uint16_t)(BP + 0x5);
L80CA:
    /* 80CA  call    word ptr cs:uncmp_tab[bx] */
    if ((c = asm_call(ASM_JMP(0x065C, rw(CODE004, BX + 0x6D30)), 0x80CF)) != 0) return c;
L80CF:
    /* 80CF  pop     si */
    SI = pop16();
L80D0:
    /* 80D0  pop     ds */
    SET_DS(pop16());
L80D1:
    /* 80D1  mov     di,0C848h */
    DI = 0xC848;
L80D4:
    /* 80D4  mov     bx,seg seg052_519C */
    BX = (uint16_t)(0x4FAF + PORT_LOAD_SEG);
L80D7:
    /* 80D7  mov     es,bx */
    SET_ES(BX);
L80D9:
    /* 80D9  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L80DA:
    /* 80DA  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L80DB:
    /* 80DB  inc     di */
    DI = (uint16_t)(DI + 1);
L80DC:
    /* 80DC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L80DD:
    /* 80DD  inc     di */
    DI = (uint16_t)(DI + 1);
L80DE:
    /* 80DE  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L80DF:
    /* 80DF  inc     di */
    DI = (uint16_t)(DI + 1);
L80E0:
    /* 80E0  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L80E1:
    /* 80E1  pop     si */
    SI = pop16();
L80E2:
    /* 80E2  mov     ax,seg seg052_519C */
    AX = (uint16_t)(0x4FAF + PORT_LOAD_SEG);
L80E5:
    /* 80E5  mov     ds,ax */
    SET_DS(AX);
L80E7:
    /* 80E7  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L80E8:
    /* 80E8  mov     di,ax */
    DI = AX;
L80EA:
    /* 80EA  add     di,14D0h */
    DI = add16(DI, 0x14D0, 0);
L80EE:
    /* 80EE  mov     ax,0Eh */
    AX = 0xE;
L80F1:
    /* 80F1  db      0EAh */
    return ASM_JMP(0x065C, 0x3FA7);

    /* seg004_0849_80F6  (+80F6)
       do_setbmcol (opcode 0xAE): set bmcolor (seg_370D:0DC1), the colour sprites are drawn in during
       a pick frame. Operand: the colour. */
L80F6: /* _do_setbmcol */
    /* 80F6  mov     bx,es */
    BX = asm_es;
L80F8:
    /* 80F8  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L80FB:
    /* 80FB  mov     es,ax */
    SET_ES(AX);
L80FD:
    /* 80FD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L80FE:
    /* 80FE  mov     byte ptr es:[0DC1h],al */
    wb(pES, 0xDC1, AL);
L8102:
    /* 8102  mov     es,bx */
    SET_ES(BX);
L8104:
    /* 8104  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L8105:
    /* 8105  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L8106:
    /* 8106  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_810A  (+810A)
       do_mouseq (opcode 0xB0): service the mouse during a frame. Calls seg021_22FD_102B
       (C3DENTRY.ASM), which calls MousQUp; FM Towns calls MousQUp(1) directly. GRIDDB.C emits it at
       the end of each grid row, so the cursor keeps moving while a frame renders. */
L810A: /* _do_mouseq */
    /* 810A  call    far ptr _seg021_22FD_102B */
    if ((c = asm_callf(ASM_JMP(0x2110, 0x102B), 0x065C + PORT_LOAD_SEG, 0x810F)) != 0) return c;
L810F:
    /* 810F  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L8110:
    /* 8110  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L8111:
    /* 8111  jmp     word ptr [bx+OPCODE_TABLE] */
    return ASM_JMP(0x065C, rw(pDS, BX + 0x24F4));

    /* seg004_0849_8115  (+8115)
       do_uwcolv: set the colour for the next flat polygon from a colour word and the distance shade.
       Operands: the address (in DS) of the colour, a shade word whose low byte is added to cdist
       (2694), capped at 15, to pick the lightabs row. In a pick frame (pickup set) the colour is
       bmcolor instead. Ends in INTERP.ASM's setcol_common. */
L8115: /* _do_uwcolv */
    /* 8115  test    word ptr ds:[2692h],0FFh */
    logic16((uint16_t)(rw(pDS, 0x2692) & 0xFF));
L811B:
    /* 811B  jne     short L8141 */
    if (!ZF) goto L8141;
L811D:
    /* 811D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L811E:
    /* 811E  mov     di,ax */
    DI = AX;
L8120:
    /* 8120  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L8121:
    /* 8121  mov     bx,ax */
    BX = AX;
L8123:
    /* 8123  mov     ax,word ptr [di] */
    AX = rw(pDS, DI);
L8125:
    /* 8125  xchg    bh,bl */
    { uint8_t t_ = BL;
    BL = BH;
    BH = t_; }
L8127:
    /* 8127  mov     cx,word ptr ds:[2694h] */
    CX = rw(pDS, 0x2694);
L812B:
    /* 812B  add     bh,cl */
    BH = (uint8_t)(BH + CL);
L812D:
    /* 812D  cmp     bh,0Fh */
    sub8(BH, 0xF, 0);
L8130:
    /* 8130  jbe     short L8134 */
    if (CF || ZF) goto L8134;
L8132:
    /* 8132  mov     bh,0Fh */
    BH = 0xF;
L8134: /* L8134 */
    /* 8134  add     bx,offset lightabs */
    BX = (uint16_t)(BX + 0x6D3E);
L8138:
    /* 8138  add     bx,ax */
    BX = (uint16_t)(BX + AX);
L813A:
    /* 813A  mov     al,byte ptr cs:[bx] */
    AL = rb(CODE004, BX);
L813D:
    /* 813D  xor     ah,ah */
    AH = logic8((uint8_t)(AH ^ AH));
L813F:
    /* 813F  jmp     short L8151 */
    goto L8151;
L8141: /* L8141 */
    /* 8141  push    es */
    push16(asm_es);
L8142:
    /* 8142  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L8145:
    /* 8145  mov     es,ax */
    SET_ES(AX);
L8147:
    /* 8147  mov     al,byte ptr es:[0DC1h] */
    AL = rb(pES, 0xDC1);
L814B:
    /* 814B  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L814D:
    /* 814D  pop     es */
    SET_ES(pop16());
L814E:
    /* 814E  add     si,4 */
    SI = add16(SI, 0x4, 0);
L8151: /* L8151 */
    /* 8151  jmp     setcol_common */
    return ASM_JMP(0x065C, 0x1FF3);
}
