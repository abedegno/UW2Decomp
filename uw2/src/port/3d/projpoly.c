/* projpoly.c: replaces src/3d/PROJPOLY.ASM (seg004_0849_4E10, 4E10..51A2 of its
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

uint32_t asm_mod_PROJPOLY(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x4E10: goto L4E10;
    case 0x4E11: goto L4E11;
    case 0x4E12: goto L4E12;
    case 0x4E14: goto L4E14;
    case 0x4E16: goto L4E16;
    case 0x4E18: goto L4E18;
    case 0x4E1B: goto L4E1B;
    case 0x4E1F: goto L4E1F;
    case 0x4E23: goto L4E23;
    case 0x4E29: goto L4E29;
    case 0x4E2F: goto L4E2F;
    case 0x4E33: goto L4E33;
    case 0x4E37: goto L4E37;
    case 0x4E3A: goto L4E3A;
    case 0x4E3D: goto L4E3D;
    case 0x4E42: goto L4E42;
    case 0x4E46: goto L4E46;
    case 0x4E4A: goto L4E4A;
    case 0x4E4D: goto L4E4D;
    case 0x4E51: goto L4E51;
    case 0x4E55: goto L4E55;
    case 0x4E58: goto L4E58;
    case 0x4E5D: goto L4E5D;
    case 0x4E61: goto L4E61;
    case 0x4E65: goto L4E65;
    case 0x4E6A: goto L4E6A;
    case 0x4E6C: goto L4E6C;
    case 0x4E6D: goto L4E6D;
    case 0x4E6E: goto L4E6E;
    case 0x4E71: goto L4E71;
    case 0x4E75: goto L4E75;
    case 0x4E78: goto L4E78;
    case 0x4E7C: goto L4E7C;
    case 0x4E7F: goto L4E7F;
    case 0x4E81: goto L4E81;
    case 0x4E83: goto L4E83;
    case 0x4E85: goto L4E85;
    case 0x4E87: goto L4E87;
    case 0x4E88: goto L4E88;
    case 0x4E89: goto L4E89;
    case 0x4E8A: goto L4E8A;
    case 0x4E8B: goto L4E8B;
    case 0x4E8D: goto L4E8D;
    case 0x4E8E: goto L4E8E;
    case 0x4E8F: goto L4E8F;
    case 0x4E90: goto L4E90;
    case 0x4E94: goto L4E94;
    case 0x4E96: goto L4E96;
    case 0x4E97: goto L4E97;
    case 0x4E98: goto L4E98;
    case 0x4E9C: goto L4E9C;
    case 0x4E9E: goto L4E9E;
    case 0x4E9F: goto L4E9F;
    case 0x4EA0: goto L4EA0;
    case 0x4EA2: goto L4EA2;
    case 0x4EA3: goto L4EA3;
    case 0x4EA4: goto L4EA4;
    case 0x4EA9: goto L4EA9;
    case 0x4EAB: goto L4EAB;
    case 0x4EAE: goto L4EAE;
    case 0x4EB1: goto L4EB1;
    case 0x4EB4: goto L4EB4;
    case 0x4EB6: goto L4EB6;
    case 0x4EB8: goto L4EB8;
    case 0x4EB9: goto L4EB9;
    case 0x4EBA: goto L4EBA;
    case 0x4EC0: goto L4EC0;
    case 0x4EC2: goto L4EC2;
    case 0x4EC5: goto L4EC5;
    case 0x4ECB: goto L4ECB;
    case 0x4ECD: goto L4ECD;
    case 0x4ED0: goto L4ED0;
    case 0x4ED3: goto L4ED3;
    case 0x4ED6: goto L4ED6;
    case 0x4ED7: goto L4ED7;
    case 0x4ED8: goto L4ED8;
    case 0x4EDB: goto L4EDB;
    case 0x4EE1: goto L4EE1;
    case 0x4EE3: goto L4EE3;
    case 0x4EE4: goto L4EE4;
    case 0x4EE5: goto L4EE5;
    case 0x4EE8: goto L4EE8;
    case 0x4EEA: goto L4EEA;
    case 0x4EEB: goto L4EEB;
    case 0x4EEE: goto L4EEE;
    case 0x4EF1: goto L4EF1;
    case 0x4EF3: goto L4EF3;
    case 0x4EF4: goto L4EF4;
    case 0x4EF5: goto L4EF5;
    case 0x4EF6: goto L4EF6;
    case 0x4EF7: goto L4EF7;
    case 0x4EF8: goto L4EF8;
    case 0x4EF9: goto L4EF9;
    case 0x4EFB: goto L4EFB;
    case 0x4EFC: goto L4EFC;
    case 0x4EFF: goto L4EFF;
    case 0x4F00: goto L4F00;
    case 0x4F02: goto L4F02;
    case 0x4F03: goto L4F03;
    case 0x4F04: goto L4F04;
    case 0x4F05: goto L4F05;
    case 0x4F0B: goto L4F0B;
    case 0x4F0E: goto L4F0E;
    case 0x4F11: goto L4F11;
    case 0x4F15: goto L4F15;
    case 0x4F17: goto L4F17;
    case 0x4F18: goto L4F18;
    case 0x4F19: goto L4F19;
    case 0x4F1A: goto L4F1A;
    case 0x4F1D: goto L4F1D;
    case 0x4F1F: goto L4F1F;
    case 0x4F21: goto L4F21;
    case 0x4F23: goto L4F23;
    case 0x4F25: goto L4F25;
    case 0x4F27: goto L4F27;
    case 0x4F29: goto L4F29;
    case 0x4F2B: goto L4F2B;
    case 0x4F2D: goto L4F2D;
    case 0x4F2E: goto L4F2E;
    case 0x4F31: goto L4F31;
    case 0x4F33: goto L4F33;
    case 0x4F35: goto L4F35;
    case 0x4F37: goto L4F37;
    case 0x4F39: goto L4F39;
    case 0x4F3B: goto L4F3B;
    case 0x4F3D: goto L4F3D;
    case 0x4F3F: goto L4F3F;
    case 0x4F41: goto L4F41;
    case 0x4F43: goto L4F43;
    case 0x4F45: goto L4F45;
    case 0x4F47: goto L4F47;
    case 0x4F49: goto L4F49;
    case 0x4F4A: goto L4F4A;
    case 0x4F4B: goto L4F4B;
    case 0x4F4C: goto L4F4C;
    case 0x4F4F: goto L4F4F;
    case 0x4F50: goto L4F50;
    case 0x4F51: goto L4F51;
    case 0x4F52: goto L4F52;
    case 0x4F58: goto L4F58;
    case 0x4F59: goto L4F59;
    case 0x4F5C: goto L4F5C;
    case 0x4F5D: goto L4F5D;
    case 0x4F60: goto L4F60;
    case 0x4F62: goto L4F62;
    case 0x4F66: goto L4F66;
    case 0x4F6A: goto L4F6A;
    case 0x4F71: goto L4F71;
    case 0x4F72: goto L4F72;
    case 0x4F73: goto L4F73;
    case 0x4F74: goto L4F74;
    case 0x4F77: goto L4F77;
    case 0x4F79: goto L4F79;
    case 0x4F7D: goto L4F7D;
    case 0x4F81: goto L4F81;
    case 0x4F82: goto L4F82;
    case 0x4F85: goto L4F85;
    case 0x4F86: goto L4F86;
    case 0x4F89: goto L4F89;
    case 0x4F8F: goto L4F8F;
    case 0x4F90: goto L4F90;
    case 0x4F93: goto L4F93;
    case 0x4F95: goto L4F95;
    case 0x4F99: goto L4F99;
    case 0x4F9C: goto L4F9C;
    case 0x4F9F: goto L4F9F;
    case 0x4FA0: goto L4FA0;
    case 0x4FA3: goto L4FA3;
    case 0x4FA5: goto L4FA5;
    case 0x4FA7: goto L4FA7;
    case 0x4FA8: goto L4FA8;
    case 0x4FAB: goto L4FAB;
    case 0x4FB0: goto L4FB0;
    case 0x4FB3: goto L4FB3;
    case 0x4FB6: goto L4FB6;
    case 0x4FBC: goto L4FBC;
    case 0x4FBE: goto L4FBE;
    case 0x4FBF: goto L4FBF;
    case 0x4FC2: goto L4FC2;
    case 0x4FC5: goto L4FC5;
    case 0x4FC9: goto L4FC9;
    case 0x4FCA: goto L4FCA;
    case 0x4FCD: goto L4FCD;
    case 0x4FD0: goto L4FD0;
    case 0x4FD1: goto L4FD1;
    case 0x4FD2: goto L4FD2;
    case 0x4FD4: goto L4FD4;
    case 0x4FD8: goto L4FD8;
    case 0x4FDB: goto L4FDB;
    case 0x4FDC: goto L4FDC;
    case 0x4FE0: goto L4FE0;
    case 0x4FE3: goto L4FE3;
    case 0x4FE4: goto L4FE4;
    case 0x4FE7: goto L4FE7;
    case 0x4FEA: goto L4FEA;
    case 0x4FEC: goto L4FEC;
    case 0x4FED: goto L4FED;
    case 0x4FF0: goto L4FF0;
    case 0x4FF4: goto L4FF4;
    case 0x4FF6: goto L4FF6;
    case 0x4FF8: goto L4FF8;
    case 0x4FFA: goto L4FFA;
    case 0x4FFD: goto L4FFD;
    case 0x5000: goto L5000;
    case 0x5003: goto L5003;
    case 0x5004: goto L5004;
    case 0x5007: goto L5007;
    case 0x5008: goto L5008;
    case 0x500B: goto L500B;
    case 0x500C: goto L500C;
    case 0x500F: goto L500F;
    case 0x5012: goto L5012;
    case 0x5016: goto L5016;
    case 0x501A: goto L501A;
    case 0x501D: goto L501D;
    case 0x5021: goto L5021;
    case 0x5026: goto L5026;
    case 0x5029: goto L5029;
    case 0x502D: goto L502D;
    case 0x5030: goto L5030;
    case 0x5033: goto L5033;
    case 0x5037: goto L5037;
    case 0x503A: goto L503A;
    case 0x503D: goto L503D;
    case 0x503E: goto L503E;
    case 0x5040: goto L5040;
    case 0x5043: goto L5043;
    case 0x5048: goto L5048;
    case 0x504B: goto L504B;
    case 0x504F: goto L504F;
    case 0x5053: goto L5053;
    case 0x5055: goto L5055;
    case 0x5059: goto L5059;
    case 0x505A: goto L505A;
    case 0x505B: goto L505B;
    case 0x5060: goto L5060;
    case 0x5063: goto L5063;
    case 0x5066: goto L5066;
    case 0x5069: goto L5069;
    case 0x506C: goto L506C;
    case 0x506F: goto L506F;
    case 0x5070: goto L5070;
    case 0x5071: goto L5071;
    case 0x5072: goto L5072;
    case 0x5074: goto L5074;
    case 0x5077: goto L5077;
    case 0x507A: goto L507A;
    case 0x507B: goto L507B;
    case 0x507E: goto L507E;
    case 0x5081: goto L5081;
    case 0x5084: goto L5084;
    case 0x5086: goto L5086;
    case 0x5088: goto L5088;
    case 0x508A: goto L508A;
    case 0x508C: goto L508C;
    case 0x508E: goto L508E;
    case 0x5090: goto L5090;
    case 0x5093: goto L5093;
    case 0x5095: goto L5095;
    case 0x5096: goto L5096;
    case 0x5097: goto L5097;
    case 0x5099: goto L5099;
    case 0x509C: goto L509C;
    case 0x509E: goto L509E;
    case 0x509F: goto L509F;
    case 0x50A1: goto L50A1;
    case 0x50A3: goto L50A3;
    case 0x50A4: goto L50A4;
    case 0x50A5: goto L50A5;
    case 0x50A8: goto L50A8;
    case 0x50AA: goto L50AA;
    case 0x50AC: goto L50AC;
    case 0x50AE: goto L50AE;
    case 0x50B0: goto L50B0;
    case 0x50B2: goto L50B2;
    case 0x50B3: goto L50B3;
    case 0x50B6: goto L50B6;
    case 0x50B8: goto L50B8;
    case 0x50BA: goto L50BA;
    case 0x50BC: goto L50BC;
    case 0x50BE: goto L50BE;
    case 0x50C0: goto L50C0;
    case 0x50C2: goto L50C2;
    case 0x50C5: goto L50C5;
    case 0x50C7: goto L50C7;
    case 0x50C9: goto L50C9;
    case 0x50CC: goto L50CC;
    case 0x50CE: goto L50CE;
    case 0x50D0: goto L50D0;
    case 0x50D4: goto L50D4;
    case 0x50D8: goto L50D8;
    case 0x50DA: goto L50DA;
    case 0x50DB: goto L50DB;
    case 0x50DD: goto L50DD;
    case 0x50DF: goto L50DF;
    case 0x50E0: goto L50E0;
    case 0x50E2: goto L50E2;
    case 0x50E4: goto L50E4;
    case 0x50E6: goto L50E6;
    case 0x50E7: goto L50E7;
    case 0x50EA: goto L50EA;
    case 0x50EC: goto L50EC;
    case 0x50EF: goto L50EF;
    case 0x50F2: goto L50F2;
    case 0x50F5: goto L50F5;
    case 0x50F8: goto L50F8;
    case 0x50FC: goto L50FC;
    case 0x50FF: goto L50FF;
    case 0x5102: goto L5102;
    case 0x5105: goto L5105;
    case 0x5109: goto L5109;
    case 0x510C: goto L510C;
    case 0x510F: goto L510F;
    case 0x5112: goto L5112;
    case 0x5115: goto L5115;
    case 0x5119: goto L5119;
    case 0x5120: goto L5120;
    case 0x5123: goto L5123;
    case 0x5127: goto L5127;
    case 0x512C: goto L512C;
    case 0x5130: goto L5130;
    case 0x5133: goto L5133;
    case 0x5135: goto L5135;
    case 0x5138: goto L5138;
    case 0x513D: goto L513D;
    case 0x5141: goto L5141;
    case 0x5144: goto L5144;
    case 0x514A: goto L514A;
    case 0x514C: goto L514C;
    case 0x5152: goto L5152;
    case 0x5153: goto L5153;
    case 0x5155: goto L5155;
    case 0x5157: goto L5157;
    case 0x5159: goto L5159;
    case 0x515A: goto L515A;
    case 0x515C: goto L515C;
    case 0x515F: goto L515F;
    case 0x5162: goto L5162;
    case 0x5165: goto L5165;
    case 0x5167: goto L5167;
    case 0x516B: goto L516B;
    case 0x516E: goto L516E;
    case 0x5170: goto L5170;
    case 0x5175: goto L5175;
    case 0x5178: goto L5178;
    case 0x517C: goto L517C;
    case 0x517E: goto L517E;
    case 0x5181: goto L5181;
    case 0x5184: goto L5184;
    case 0x5187: goto L5187;
    case 0x518A: goto L518A;
    case 0x518D: goto L518D;
    case 0x5190: goto L5190;
    case 0x5194: goto L5194;
    case 0x519A: goto L519A;
    case 0x519C: goto L519C;
    case 0x519F: goto L519F;
    case 0x51A1: goto L51A1;
    default: asm_bad_entry("PROJPOLY.ASM", entry);
    }

    /* seg004_0849_4E10  (+4E10)
       _asm_project_polygon: perspective projection and shading of a polygon's vertices.
         in:  SI = vertex records, CX = count
         out: each record's sx = x * scrw / z + biasx and sy = y * scrh / z + biasy (16.16, sy
              clamped at 0), and l = compute_lighting_value. Registers preserved except EAX, EDX.
       The multiply is by the integer scale and the 16.16 product is widened to 32.32 before the
       divide by z, so the result stays 16.16. A z near 0 can overflow the divide; the int 0 handler
       then sets _divide_overflow_has_occurred and _asm_texture_map_1 drops the polygon. */
L4E10: /* __asm_project_polygon */
    /* 4E10  push    cx */
    push16(CX);
L4E11:
    /* 4E11  push    si */
    push16(SI);
L4E12:
    /* 4E12  push    ebx */
    push32(EBX);
L4E14:
    /* 4E14  push    ebp */
    push32(EBP);
L4E16:
    /* 4E16  push    edi */
    push32(EDI);
L4E18:
    /* 4E18  mov     ax,word ptr ds:[2476h] */
    AX = rw(pDS, 0x2476);
L4E1B:
    /* 4E1B  shl     eax,10h */
    EAX = (uint32_t)(EAX << 16);
L4E1F:
    /* 4E1F  mov     dword ptr ds:[0CCE4h],eax */
    wd(pDS, 0xCCE4, EAX);
L4E23:
    /* 4E23  movsx   ebp,word ptr ds:[2472h] */
    EBP = (uint32_t)(int16_t)rw(pDS, 0x2472);
L4E29:
    /* 4E29  movsx   edi,word ptr ds:[2470h] */
    EDI = (uint32_t)(int16_t)rw(pDS, 0x2470);
L4E2F:
    /* 4E2F  mov     bx,word ptr ds:[2474h] */
    BX = rw(pDS, 0x2474);
L4E33:
    /* 4E33  shl     ebx,10h */
    EBX = (uint32_t)(EBX << 16);
L4E37: /* L4E37 */
    /* 4E37  mov     eax,dword ptr [si] */
    EAX = rd(pDS, SI);
L4E3A:
    /* 4E3A  imul    ebp */
    imul32(EBP);
L4E3D:
    /* 4E3D  shld    edx,eax,10h */
    EDX = shld32(EDX, EAX, 16);
L4E42:
    /* 4E42  shl     eax,10h */
    EAX = (uint32_t)(EAX << 16);
L4E46:
    /* 4E46  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x4E46, 4)) != 0) return c;
L4E4A:
    /* 4E4A  add     eax,ebx */
    EAX = (uint32_t)(EAX + EBX);
L4E4D:
    /* 4E4D  mov     dword ptr [si+0Ch],eax */
    wd(pDS, SI + 0xC, EAX);
L4E51:
    /* 4E51  mov     eax,dword ptr [si+4] */
    EAX = rd(pDS, SI + 0x4);
L4E55:
    /* 4E55  imul    edi */
    imul32(EDI);
L4E58:
    /* 4E58  shld    edx,eax,10h */
    EDX = shld32(EDX, EAX, 16);
L4E5D:
    /* 4E5D  shl     eax,10h */
    EAX = (uint32_t)(EAX << 16);
L4E61:
    /* 4E61  idiv    dword ptr [si+8] */
    if (asm_idiv32(rd(pDS, SI + 0x8)) && (c = asm_divfault(0x065C, 0x4E61, 4)) != 0) return c;
L4E65:
    /* 4E65  add     eax,dword ptr ds:[0CCE4h] */
    EAX = add32(EAX, rd(pDS, 0xCCE4), 0);
L4E6A:
    /* 4E6A  jns     L4E71 */
    if (!SF) goto L4E71;
L4E6C:
    /* 4E6C  nop */
    ;
L4E6D:
    /* 4E6D  nop */
    ;
L4E6E:
    /* 4E6E  sub     eax,eax */
    EAX = (uint32_t)(EAX - EAX);
L4E71: /* L4E71 */
    /* 4E71  mov     dword ptr [si+10h],eax */
    wd(pDS, SI + 0x10, EAX);
L4E75:
    /* 4E75  call    _compute_lighting_value */
    if ((c = asm_call(ASM_JMP(0x065C, 0x50E0), 0x4E78)) != 0) return c;
L4E78:
    /* 4E78  mov     dword ptr [si+1Ch],eax */
    wd(pDS, SI + 0x1C, EAX);
L4E7C:
    /* 4E7C  add     si,20h */
    SI = add16(SI, 0x20, 0);
L4E7F:
    /* 4E7F  loop    L4E37 */
    if (--CX) goto L4E37;
L4E81:
    /* 4E81  pop     edi */
    EDI = pop32();
L4E83:
    /* 4E83  pop     ebp */
    EBP = pop32();
L4E85:
    /* 4E85  pop     ebx */
    EBX = pop32();
L4E87:
    /* 4E87  pop     si */
    SI = pop16();
L4E88:
    /* 4E88  pop     cx */
    CX = pop16();
L4E89:
    /* 4E89  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_4E8A  (+4E8A)
       _asm_texture_map_1: clip, project and map one polygon. Near call, arguments on the stack:
         [bp+4] texture descriptor (DS:CCF2), [bp+6] vertices, [bp+8] count, [bp+0Ah] mapping
         type (2 perspective, otherwise linear), [bp+0Ch] (0 from the only caller), [bp+0Eh] shadem,
         [bp+10h] 1
       With type 2 or [bp+0Ch] = 0 it goes on; [bp+0Ch] above 0 returns at once and below 0 executes
       int 2 (the debug break; see GRMISC.ASM). It clips unless accept_flag (DS:25D3) is set, stops
       if a divide has overflowed, projects, checks again, and calls _asm_texture_map_v when
       global_vertical_scan_flag is set and _asm_texture_map otherwise, with (descriptor, clipped
       vertices, count, type, shadem, 1). _divide_overflow_has_occurred, the byte after the code,
       lives in the code segment (CS-relative) so the int 0 handler can set it whatever DS holds. */
L4E8A: /* __asm_texture_map_1 */
    /* 4E8A  push    bp */
    push16(BP);
L4E8B:
    /* 4E8B  mov     bp,sp */
    BP = SP;
L4E8D:
    /* 4E8D  push    si */
    push16(SI);
L4E8E:
    /* 4E8E  push    di */
    push16(DI);
L4E8F:
    /* 4E8F  push    bx */
    push16(BX);
L4E90:
    /* 4E90  cmp     word ptr [bp+0Ah],2 */
    sub16(rw(pSS, BP + 0xA), 0x2, 0);
L4E94:
    /* 4E94  je      L4EA4 */
    if (ZF) goto L4EA4;
L4E96:
    /* 4E96  nop */
    ;
L4E97:
    /* 4E97  nop */
    ;
L4E98:
    /* 4E98  cmp     word ptr [bp+0Ch],0 */
    sub16(rw(pSS, BP + 0xC), 0x0, 0);
L4E9C:
    /* 4E9C  jl      L4EF9 */
    if (SF != OF) goto L4EF9;
L4E9E:
    /* 4E9E  nop */
    ;
L4E9F:
    /* 4E9F  nop */
    ;
L4EA0:
    /* 4EA0  jne     L4EF4 */
    if (!ZF) goto L4EF4;
L4EA2: /* L4EA4 */
    /* 4EA2  nop */
    ;
L4EA3:
    /* 4EA3  nop */
    ;
L4EA4:
    /* 4EA4  test    byte ptr ds:[25D3h],0FFh */
    logic8((uint8_t)(rb(pDS, 0x25D3) & 0xFF));
L4EA9:
    /* 4EA9  jne     short L4EBA */
    if (!ZF) goto L4EBA;
L4EAB:
    /* 4EAB  mov     si,word ptr [bp+6] */
    SI = rw(pSS, BP + 0x6);
L4EAE:
    /* 4EAE  mov     cx,word ptr [bp+8] */
    CX = rw(pSS, BP + 0x8);
L4EB1:
    /* 4EB1  call    __asm_clip_polygon */
    if ((c = asm_call(ASM_JMP(0x065C, 0x4686), 0x4EB4)) != 0) return c;
L4EB4:
    /* 4EB4  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L4EB6:
    /* 4EB6  je      L4EF4 */
    if (ZF) goto L4EF4;
L4EB8: /* L4EBA */
    /* 4EB8  nop */
    ;
L4EB9:
    /* 4EB9  nop */
    ;
L4EBA:
    /* 4EBA  test    byte ptr cs:_divide_overflow_has_occurred,0FFh */
    logic8((uint8_t)(rb(CODE004, 0x4EFE) & 0xFF));
L4EC0:
    /* 4EC0  jne     short L4EF1 */
    if (!ZF) goto L4EF1;
L4EC2:
    /* 4EC2  call    __asm_project_polygon */
    if ((c = asm_call(ASM_JMP(0x065C, 0x4E10), 0x4EC5)) != 0) return c;
L4EC5:
    /* 4EC5  test    byte ptr cs:_divide_overflow_has_occurred,0FFh */
    logic8((uint8_t)(rb(CODE004, 0x4EFE) & 0xFF));
L4ECB:
    /* 4ECB  jne     short L4EF1 */
    if (!ZF) goto L4EF1;
L4ECD:
    /* 4ECD  push    word ptr [bp+10h] */
    push16(rw(pSS, BP + 0x10));
L4ED0:
    /* 4ED0  push    word ptr [bp+0Eh] */
    push16(rw(pSS, BP + 0xE));
L4ED3:
    /* 4ED3  push    word ptr [bp+0Ah] */
    push16(rw(pSS, BP + 0xA));
L4ED6:
    /* 4ED6  push    cx */
    push16(CX);
L4ED7:
    /* 4ED7  push    si */
    push16(SI);
L4ED8:
    /* 4ED8  push    word ptr [bp+4] */
    push16(rw(pSS, BP + 0x4));
L4EDB:
    /* 4EDB  test    word ptr ds:[0CCEEh],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0xCCEE) & 0xFFFF));
L4EE1:
    /* 4EE1  je      L4EEB */
    if (ZF) goto L4EEB;
L4EE3:
    /* 4EE3  nop */
    ;
L4EE4:
    /* 4EE4  nop */
    ;
L4EE5:
    /* 4EE5  call    __asm_texture_map_v */
    if ((c = asm_call(ASM_JMP(0x065C, 0x6720), 0x4EE8)) != 0) return c;
L4EE8:
    /* 4EE8  jmp     L4EEE */
    goto L4EEE;
L4EEA: /* L4EEB */
    /* 4EEA  nop */
    ;
L4EEB:
    /* 4EEB  call    __asm_texture_map */
    if ((c = asm_call(ASM_JMP(0x065C, 0x5A54), 0x4EEE)) != 0) return c;
L4EEE: /* L4EEE */
    /* 4EEE  add     sp,0Ch */
    SP = add16(SP, 0xC, 0);
L4EF1: /* L4EF1 */
    /* 4EF1  jmp     L4EF4 */
    goto L4EF4;
L4EF3: /* L4EF4 */
    /* 4EF3  nop */
    ;
L4EF4:
    /* 4EF4  pop     bx */
    BX = pop16();
L4EF5:
    /* 4EF5  pop     di */
    DI = pop16();
L4EF6:
    /* 4EF6  pop     si */
    SI = pop16();
L4EF7:
    /* 4EF7  pop     bp */
    BP = pop16();
L4EF8:
    /* 4EF8  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L4EF9: /* L4EF9 */
    /* 4EF9  int     2 */
    asm_halt_at(0x065C, 0x4EF9, "int 2h, the debugger break");
L4EFB:
    /* 4EFB  nop */
    ;
L4EFC:
    /* 4EFC  jmp     L4EF4 */
    goto L4EF4;

    /* seg004_0849_4EFF  (+4EFF)
       divide_overflow_handler: the int 0 handler while a polygon is mapped. cRender (C3DENTRY.ASM)
       points int 0 at dseg062_62a6:05A0, which passes control to the seg004 offset held at 05A5 (FM
       overflow_ptr; see INSTANCE.ASM); _seg004_0849_4F5C puts this handler's offset there. It sets
       _divide_overflow_has_occurred and resumes after the faulting divide with EAX = 30000000h. To
       find the next instruction it skips a 66h operand-size prefix and any segment prefixes (36h,
       2Eh, 3Eh, 26h), then the opcode and its ModRM byte: ModRM 3Eh (idiv with a direct 16-bit
       address) skips two more bytes, mod 01 one more (an 8-bit displacement), mod 00 and mod 11
       none. Mod 10 is also given one more byte, though in 16-bit addressing it carries a 16-bit
       displacement; no divide in these modules uses that form. Any other ModRM high nibble executes
       int 2. Only the div and idiv forms (reg field 6 or 7) can reach the tests that succeed. */
L4EFF: /* _divide_overflow_handler */
    /* 4EFF  push    bp */
    push16(BP);
L4F00:
    /* 4F00  mov     bp,sp */
    BP = SP;
L4F02:
    /* 4F02  push    ax */
    push16(AX);
L4F03:
    /* 4F03  push    bx */
    push16(BX);
L4F04:
    /* 4F04  push    es */
    push16(asm_es);
L4F05:
    /* 4F05  mov     byte ptr cs:_divide_overflow_has_occurred,1 */
    wb(CODE004, 0x4EFE, 0x1);
L4F0B:
    /* 4F0B  mov     bx,word ptr [bp+2] */
    BX = rw(pSS, BP + 0x2);
L4F0E:
    /* 4F0E  mov     es,word ptr [bp+4] */
    SET_ES(rw(pSS, BP + 0x4));
L4F11:
    /* 4F11  cmp     byte ptr es:[bx],66h */
    sub8(rb(pES, BX), 0x66, 0);
L4F15:
    /* 4F15  jne     short L4F18 */
    if (!ZF) goto L4F18;
L4F17:
    /* 4F17  inc     bx */
    BX = (uint16_t)(BX + 1);
L4F18: /* L4F18 */
    /* 4F18  dec     bx */
    BX = (uint16_t)(BX - 1);
L4F19: /* L4F19 */
    /* 4F19  inc     bx */
    BX = (uint16_t)(BX + 1);
L4F1A:
    /* 4F1A  mov     al,byte ptr es:[bx] */
    AL = rb(pES, BX);
L4F1D:
    /* 4F1D  cmp     al,36h */
    sub8(AL, 0x36, 0);
L4F1F:
    /* 4F1F  je      L4F19 */
    if (ZF) goto L4F19;
L4F21:
    /* 4F21  cmp     al,2Eh */
    sub8(AL, 0x2E, 0);
L4F23:
    /* 4F23  je      L4F19 */
    if (ZF) goto L4F19;
L4F25:
    /* 4F25  cmp     al,3Eh */
    sub8(AL, 0x3E, 0);
L4F27:
    /* 4F27  je      L4F19 */
    if (ZF) goto L4F19;
L4F29:
    /* 4F29  cmp     al,26h */
    sub8(AL, 0x26, 0);
L4F2B:
    /* 4F2B  je      L4F19 */
    if (ZF) goto L4F19;
L4F2D:
    /* 4F2D  inc     bx */
    BX = (uint16_t)(BX + 1);
L4F2E:
    /* 4F2E  mov     al,byte ptr es:[bx] */
    AL = rb(pES, BX);
L4F31:
    /* 4F31  cmp     al,3Eh */
    sub8(AL, 0x3E, 0);
L4F33:
    /* 4F33  je      short L4F49 */
    if (ZF) goto L4F49;
L4F35:
    /* 4F35  and     al,0F0h */
    AL = (uint8_t)(AL & 0xF0);
L4F37:
    /* 4F37  cmp     al,30h */
    sub8(AL, 0x30, 0);
L4F39:
    /* 4F39  je      short L4F4B */
    if (ZF) goto L4F4B;
L4F3B:
    /* 4F3B  cmp     al,70h */
    sub8(AL, 0x70, 0);
L4F3D:
    /* 4F3D  je      short L4F4A */
    if (ZF) goto L4F4A;
L4F3F:
    /* 4F3F  cmp     al,0B0h */
    sub8(AL, 0xB0, 0);
L4F41:
    /* 4F41  je      short L4F4A */
    if (ZF) goto L4F4A;
L4F43:
    /* 4F43  cmp     al,0F0h */
    sub8(AL, 0xF0, 0);
L4F45:
    /* 4F45  je      short L4F4B */
    if (ZF) goto L4F4B;
L4F47:
    /* 4F47  int     2 */
    asm_halt_at(0x065C, 0x4F47, "int 2h, the debugger break");
L4F49: /* L4F49 */
    /* 4F49  inc     bx */
    BX = inc16(BX);
L4F4A: /* L4F4A */
    /* 4F4A  inc     bx */
    BX = inc16(BX);
L4F4B: /* L4F4B */
    /* 4F4B  inc     bx */
    BX = inc16(BX);
L4F4C:
    /* 4F4C  mov     word ptr [bp+2],bx */
    ww(pSS, BP + 0x2, BX);
L4F4F:
    /* 4F4F  pop     es */
    SET_ES(pop16());
L4F50:
    /* 4F50  pop     bx */
    BX = pop16();
L4F51:
    /* 4F51  pop     ax */
    AX = pop16();
L4F52:
    /* 4F52  mov     eax,30000000h */
    EAX = 0x30000000;
L4F58:
    /* 4F58  pop     bp */
    BP = pop16();
L4F59:
    /* 4F59  iret */
    SP = (uint16_t)(SP + 4); asm_set_flags(pop16()); return ASM_IRET;

    /* seg004_0849_4F5C  (+4F5C)
       Installs divide_overflow_handler: saves the offset at dseg062_62a6:05A5 in
       old_divide_overflow_handler and puts the handler's offset there. Called at the start of
       asm_texture_map_from_draw_poly. */
L4F5C: /* _seg004_0849_4F5C */
    /* 4F5C  push    es */
    push16(asm_es);
L4F5D:
    /* 4F5D  mov     ax,seg dseg062_62a6 */
    AX = (uint16_t)(0x60B9 + PORT_LOAD_SEG);
L4F60:
    /* 4F60  mov     es,ax */
    SET_ES(AX);
L4F62:
    /* 4F62  mov     ax,word ptr es:[5A5h] */
    AX = rw(pES, 0x5A5);
L4F66:
    /* 4F66  mov     word ptr cs:old_divide_overflow_handler,ax */
    ww(CODE004, 0x4F5A, AX);
L4F6A:
    /* 4F6A  mov     word ptr es:[5A5h],offset _divide_overflow_handler */
    ww(pES, 0x5A5, 0x4EFF);
L4F71:
    /* 4F71  pop     es */
    SET_ES(pop16());
L4F72:
    /* 4F72  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_4F73  (+4F73)
       Restores the int 0 target offset _seg004_0849_4F5C saved. L4F83, the word after it, is the
       polygon's vertex count (FM _rv2+20Ch). */
L4F73: /* _seg004_0849_4F73 */
    /* 4F73  push    es */
    push16(asm_es);
L4F74:
    /* 4F74  mov     ax,seg dseg062_62a6 */
    AX = (uint16_t)(0x60B9 + PORT_LOAD_SEG);
L4F77:
    /* 4F77  mov     es,ax */
    SET_ES(AX);
L4F79:
    /* 4F79  mov     ax,word ptr cs:old_divide_overflow_handler */
    AX = rw(CODE004, 0x4F5A);
L4F7D:
    /* 4F7D  mov     word ptr es:[5A5h],ax */
    ww(pES, 0x5A5, AX);
L4F81:
    /* 4F81  pop     es */
    SET_ES(pop16());
L4F82:
    /* 4F82  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_4F85  (+4F85)
       asm_texture_map_from_draw_poly: map one polygon the model interpreter built (called by
       TMAPOPS's _seg004_0849_3F77).
         in:  BX = texture descriptor, 8 bytes: width (a power of 2), width * height - 1, segment,
              (height - 1) * width
              SI = polygon buffer of 12-byte vertices: x, y, z (integer view coordinates), the clip
              code word, u (8.8 texel column), v (texel offset, row * width)
              CX = vertex count; more than 8 executes int 2 and keeps 8
         out: nothing; all registers preserved (pusha/popa)
       It fills the descriptor and size variables (CCE8 .. CCF8, CEFC) and writes the vertex records
       at DS:CCFC last to first, so their order is reversed. x, y and z become 16.16 with no
       fraction; u and v become the texel column and row (u's integer part, v / width) plus half a
       texel. L5072 classifies the polygon; the result goes to global_vertical_scan_flag and
       global_floor_or_ceiling_flag, and the polygon to _asm_texture_map_1 with shadem (DS:2688) as
       the shading flag. Its own divide overflow handler is installed for the duration. */
L4F85: /* _asm_texture_map_from_draw_poly */
    /* 4F85  pusha */
    { uint16_t t_ = SP; push16(AX); push16(CX); push16(DX); push16(BX); push16(t_); push16(BP); push16(SI); push16(DI); }
L4F86:
    /* 4F86  call    _seg004_0849_4F5C */
    if ((c = asm_call(ASM_JMP(0x065C, 0x4F5C), 0x4F89)) != 0) return c;
L4F89:
    /* 4F89  mov     byte ptr cs:_divide_overflow_has_occurred,0 */
    wb(CODE004, 0x4EFE, 0x0);
L4F8F:
    /* 4F8F  push    es */
    push16(asm_es);
L4F90:
    /* 4F90  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L4F93:
    /* 4F93  mov     es,ax */
    SET_ES(AX);
L4F95:
    /* 4F95  mov     ax,word ptr es:[0AEEh] */
    AX = rw(pES, 0xAEE);
L4F99:
    /* 4F99  add     ax,2 */
    AX = (uint16_t)(AX + 0x2);
L4F9C:
    /* 4F9C  mov     word ptr ds:[0CEFCh],ax */
    ww(pDS, 0xCEFC, AX);
L4F9F:
    /* 4F9F  pop     es */
    SET_ES(pop16());
L4FA0:
    /* 4FA0  cmp     cx,8 */
    sub16(CX, 0x8, 0);
L4FA3:
    /* 4FA3  jbe     short L4FAB */
    if (CF || ZF) goto L4FAB;
L4FA5:
    /* 4FA5  int     2 */
    asm_halt_at(0x065C, 0x4FA5, "int 2h, the debugger break");
L4FA7:
    /* 4FA7  nop */
    ;
L4FA8:
    /* 4FA8  mov     cx,8 */
    CX = 0x8;
L4FAB: /* L4FAB */
    /* 4FAB  mov     word ptr cs:L4F83,cx */
    ww(CODE004, 0x4F83, CX);
L4FB0:
    /* 4FB0  mov     ax,word ptr [bx+4] */
    AX = rw(pDS, BX + 0x4);
L4FB3:
    /* 4FB3  mov     word ptr ds:[0CCF8h],ax */
    ww(pDS, 0xCCF8, AX);
L4FB6:
    /* 4FB6  mov     word ptr ds:[0CCF6h],0 */
    ww(pDS, 0xCCF6, 0x0);
L4FBC:
    /* 4FBC  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L4FBE:
    /* 4FBE  push    cx */
    push16(CX);
L4FBF:
    /* 4FBF  bsf     cx,ax */
    { uint16_t t_ = AX; if (t_) { CX = (uint16_t)__builtin_ctz(t_); ZF = 0; } else ZF = 1; }
L4FC2:
    /* 4FC2  mov     word ptr ds:[0CCF2h],ax */
    ww(pDS, 0xCCF2, AX);
L4FC5:
    /* 4FC5  mov     word ptr ds:[0CCECh],cx */
    ww(pDS, 0xCCEC, CX);
L4FC9:
    /* 4FC9  dec     ax */
    AX = (uint16_t)(AX - 1);
L4FCA:
    /* 4FCA  mov     word ptr ds:[0CCE8h],ax */
    ww(pDS, 0xCCE8, AX);
L4FCD:
    /* 4FCD  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L4FD0:
    /* 4FD0  inc     ax */
    AX = (uint16_t)(AX + 1);
L4FD1:
    /* 4FD1  push    ax */
    push16(AX);
L4FD2:
    /* 4FD2  sub     dx,dx */
    DX = (uint16_t)(DX - DX);
L4FD4:
    /* 4FD4  div     word ptr ds:[0CCF2h] */
    if (asm_div16(rw(pDS, 0xCCF2)) && (c = asm_divfault(0x065C, 0x4FD4, 4)) != 0) return c;
L4FD8:
    /* 4FD8  mov     word ptr ds:[0CCF4h],ax */
    ww(pDS, 0xCCF4, AX);
L4FDB:
    /* 4FDB  pop     ax */
    AX = pop16();
L4FDC:
    /* 4FDC  sub     ax,word ptr ds:[0CCF2h] */
    AX = (uint16_t)(AX - rw(pDS, 0xCCF2));
L4FE0:
    /* 4FE0  mov     word ptr ds:[0CCEAh],ax */
    ww(pDS, 0xCCEA, AX);
L4FE3:
    /* 4FE3  pop     cx */
    CX = pop16();
L4FE4:
    /* 4FE4  mov     ax,word ptr [bx+6] */
    AX = rw(pDS, BX + 0x6);
L4FE7:
    /* 4FE7  mov     word ptr ds:[0CCEAh],ax */
    ww(pDS, 0xCCEA, AX);
L4FEA:
    /* 4FEA  mov     di,cx */
    DI = CX;
L4FEC:
    /* 4FEC  dec     di */
    DI = (uint16_t)(DI - 1);
L4FED:
    /* 4FED  imul    di,20h */
    DI = imul16x(DI, 0x20);
L4FF0:
    /* 4FF0  add     di,0CCFCh */
    DI = (uint16_t)(DI + 0xCCFC);
L4FF4:
    /* 4FF4  mov     dx,cx */
    DX = CX;
L4FF6: /* L4FF6 */
    /* 4FF6  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4FF8:
    /* 4FF8  mov     word ptr [di],ax */
    ww(pDS, DI, AX);
L4FFA:
    /* 4FFA  mov     word ptr [di+4],ax */
    ww(pDS, DI + 0x4, AX);
L4FFD:
    /* 4FFD  mov     word ptr [di+8],ax */
    ww(pDS, DI + 0x8, AX);
L5000:
    /* 5000  mov     word ptr [di+1Ch],ax */
    ww(pDS, DI + 0x1C, AX);
L5003:
    /* 5003  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5004:
    /* 5004  mov     word ptr [di+2],ax */
    ww(pDS, DI + 0x2, AX);
L5007:
    /* 5007  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L5008:
    /* 5008  mov     word ptr [di+6],ax */
    ww(pDS, DI + 0x6, AX);
L500B:
    /* 500B  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L500C:
    /* 500C  mov     word ptr [di+0Ah],ax */
    ww(pDS, DI + 0xA, AX);
L500F:
    /* 500F  add     si,2 */
    SI = (uint16_t)(SI + 0x2);
L5012:
    /* 5012  movzx   eax,word ptr [si] */
    EAX = (uint32_t)rw(pDS, SI);
L5016:
    /* 5016  shl     eax,8 */
    EAX = (uint32_t)(EAX << 8);
L501A:
    /* 501A  mov     ax,8000h */
    AX = 0x8000;
L501D:
    /* 501D  mov     dword ptr [di+14h],eax */
    wd(pDS, DI + 0x14, EAX);
L5021:
    /* 5021  movzx   eax,word ptr [si+2] */
    EAX = (uint32_t)rw(pDS, SI + 0x2);
L5026:
    /* 5026  mov     cx,10h */
    CX = 0x10;
L5029:
    /* 5029  sub     cx,word ptr ds:[0CCECh] */
    CX = sub16(CX, rw(pDS, 0xCCEC), 0);
L502D:
    /* 502D  shl     eax,cl */
    EAX = (uint32_t)(EAX << (CL & 31));
L5030:
    /* 5030  mov     ax,8000h */
    AX = 0x8000;
L5033:
    /* 5033  mov     dword ptr [di+18h],eax */
    wd(pDS, DI + 0x18, EAX);
L5037:
    /* 5037  add     si,4 */
    SI = (uint16_t)(SI + 0x4);
L503A:
    /* 503A  sub     di,20h */
    DI = (uint16_t)(DI - 0x20);
L503D:
    /* 503D  dec     dx */
    DX = dec16(DX);
L503E:
    /* 503E  jne     L4FF6 */
    if (!ZF) goto L4FF6;
L5040:
    /* 5040  mov     si,0CCFCh */
    SI = 0xCCFC;
L5043:
    /* 5043  mov     cx,word ptr cs:L4F83 */
    CX = rw(CODE004, 0x4F83);
L5048:
    /* 5048  call    L5072 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x5072), 0x504B)) != 0) return c;
L504B:
    /* 504B  mov     word ptr ds:[0CCEEh],bx */
    ww(pDS, 0xCCEE, BX);
L504F:
    /* 504F  mov     word ptr ds:[0CCF0h],bp */
    ww(pDS, 0xCCF0, BP);
L5053:
    /* 5053  push    1 */
    push16(0x1);
L5055:
    /* 5055  push    word ptr ds:[2688h] */
    push16(rw(pDS, 0x2688));
L5059:
    /* 5059  push    cx */
    push16(CX);
L505A:
    /* 505A  push    ax */
    push16(AX);
L505B:
    /* 505B  push    word ptr cs:L4F83 */
    push16(rw(CODE004, 0x4F83));
L5060:
    /* 5060  push    0CCFCh */
    push16(0xCCFC);
L5063:
    /* 5063  push    0CCF2h */
    push16(0xCCF2);
L5066:
    /* 5066  call    __asm_texture_map_1 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x4E8A), 0x5069)) != 0) return c;
L5069:
    /* 5069  add     sp,0Eh */
    SP = (uint16_t)(SP + 0xE);
L506C:
    /* 506C  call    _seg004_0849_4F73 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x4F73), 0x506F)) != 0) return c;
L506F:
    /* 506F  popa */
    DI = pop16(); SI = pop16(); BP = pop16(); SP = (uint16_t)(SP + 2); BX = pop16(); DX = pop16(); CX = pop16(); AX = pop16();
L5070:
    /* 5070  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* L5072, the classifier (L5071 is its early return). in: SI = records, CX = count. out: AX =
       mapping type, CX = 0, BX = vertical, BP = floor or ceiling.

       If every vertex has the same integer y, the polygon is parallel to the view's x-z plane, a
       floor or ceiling while the view is level (BP = 1): perspective (AX = 2), horizontal spans.
       Otherwise it is perspective when the nearest z is negative or the farthest z is more than
       twice (nearest + 1), and linear (AX = 0) when the depth varies less than that. Then, if
       mapper_rtn (C788) equals wmap_bitmap_offset (C800, initially 20Fh), the polygon came from one
       of the wall opcodes (do_wtmap, do_gwtmap, do_compact_wtmap ...), and it is drawn in vertical
       columns (BX = 1) with linear mapping: down a column of an upright wall the depth does not
       change while the view is not pitched. That reading of "w" as wall rests on FM Towns' names
       wmap_bitmap and start_wmapbm; probable. */
L5071: /* L5071 */
    /* 5071  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L5072: /* L5072 */
    /* 5072  jcxz    L5071 */
    if (!CX) goto L5071;
L5074:
    /* 5074  mov     bx,4000h */
    BX = 0x4000;
L5077:
    /* 5077  mov     dx,0C000h */
    DX = 0xC000;
L507A:
    /* 507A  push    di */
    push16(DI);
L507B:
    /* 507B  mov     di,word ptr [si+6] */
    DI = rw(pDS, SI + 0x6);
L507E:
    /* 507E  mov     bp,1 */
    BP = 0x1;
L5081: /* L5081 */
    /* 5081  mov     ax,word ptr [si+0Ah] */
    AX = rw(pDS, SI + 0xA);
L5084:
    /* 5084  cmp     ax,bx */
    sub16(AX, BX, 0);
L5086:
    /* 5086  jg      short L508A */
    if (!ZF && SF == OF) goto L508A;
L5088:
    /* 5088  mov     bx,ax */
    BX = AX;
L508A: /* L508A */
    /* 508A  cmp     ax,dx */
    sub16(AX, DX, 0);
L508C:
    /* 508C  jl      short L5090 */
    if (SF != OF) goto L5090;
L508E:
    /* 508E  mov     dx,ax */
    DX = AX;
L5090: /* L5090 */
    /* 5090  cmp     di,word ptr [si+6] */
    sub16(DI, rw(pDS, SI + 0x6), 0);
L5093:
    /* 5093  je      L5099 */
    if (ZF) goto L5099;
L5095:
    /* 5095  nop */
    ;
L5096:
    /* 5096  nop */
    ;
L5097:
    /* 5097  sub     bp,bp */
    BP = (uint16_t)(BP - BP);
L5099: /* L5099 */
    /* 5099  add     si,20h */
    SI = (uint16_t)(SI + 0x20);
L509C:
    /* 509C  loop    L5081 */
    if (--CX) goto L5081;
L509E:
    /* 509E  pop     di */
    DI = pop16();
L509F:
    /* 509F  or      bp,bp */
    BP = logic16((uint16_t)(BP | BP));
L50A1:
    /* 50A1  je      L50AE */
    if (ZF) goto L50AE;
L50A3:
    /* 50A3  nop */
    ;
L50A4:
    /* 50A4  nop */
    ;
L50A5:
    /* 50A5  mov     ax,2 */
    AX = 0x2;
L50A8:
    /* 50A8  sub     cx,cx */
    CX = (uint16_t)(CX - CX);
L50AA:
    /* 50AA  sub     bx,bx */
    BX = sub16(BX, BX, 0);
L50AC:
    /* 50AC  jmp     short L50DF */
    goto L50DF;
L50AE: /* L50AE */
    /* 50AE  or      bx,bx */
    BX = logic16((uint16_t)(BX | BX));
L50B0:
    /* 50B0  js      short L50C9 */
    if (SF) goto L50C9;
L50B2:
    /* 50B2  inc     bx */
    BX = (uint16_t)(BX + 1);
L50B3:
    /* 50B3  shl     bx,2 */
    BX = (uint16_t)(BX << 2);
L50B6:
    /* 50B6  shr     bx,1 */
    BX = (uint16_t)(BX >> 1);
L50B8:
    /* 50B8  cmp     dx,bx */
    sub16(DX, BX, 0);
L50BA:
    /* 50BA  jg      short L50C2 */
    if (!ZF && SF == OF) goto L50C2;
L50BC:
    /* 50BC  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L50BE:
    /* 50BE  sub     cx,cx */
    CX = (uint16_t)(CX - CX);
L50C0:
    /* 50C0  jmp     short L50CE */
    goto L50CE;
L50C2: /* L50C2 */
    /* 50C2  mov     ax,2 */
    AX = 0x2;
L50C5:
    /* 50C5  sub     cx,cx */
    CX = (uint16_t)(CX - CX);
L50C7:
    /* 50C7  jmp     short L50CE */
    goto L50CE;
L50C9: /* L50C9 */
    /* 50C9  mov     ax,2 */
    AX = 0x2;
L50CC:
    /* 50CC  sub     cx,cx */
    CX = (uint16_t)(CX - CX);
L50CE: /* L50CE */
    /* 50CE  sub     bx,bx */
    BX = (uint16_t)(BX - BX);
L50D0:
    /* 50D0  mov     dx,word ptr ds:[0C800h] */
    DX = rw(pDS, 0xC800);
L50D4:
    /* 50D4  cmp     word ptr ds:[0C788h],dx */
    sub16(rw(pDS, 0xC788), DX, 0);
L50D8:
    /* 50D8  jne     short L50DF */
    if (!ZF) goto L50DF;
L50DA:
    /* 50DA  inc     bx */
    BX = (uint16_t)(BX + 1);
L50DB:
    /* 50DB  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L50DD:
    /* 50DD  sub     cx,cx */
    CX = sub16(CX, CX, 0);
L50DF: /* L50DF */
    /* 50DF  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_50E0  (+50E0)
       compute_lighting_value: a vertex's shade from its distance to the eye.
         in:  SI = vertex record (view space x, y, z)
         out: EAX = shade, 16.16, between 0 and 15.0; EDX changed, ECX, EBX, ESI kept
       d = sqrt(x*x + y*y + z*z) (16.16); shade = (d >> 5) * _smooth_div >> 6, plus
       _smooth_lowpass << 16, clamped at 0 if negative, plus _smooth_base << 16, capped at 0F0000h.
       The three constants are words in seg_370D (064C, 064E, 0650; FM Towns names) that set_light
       (LIGHTING.C) reads from SHADES.DAT for the current light level; VIEW3D.C and GAMESORT.C work
       out the same formula in C. The shade selects a row of lightabs (PGCACHE.ASM), which is loaded
       from LIGHT.DAT; in the copy built into the EXE row 0 leaves a colour alone and row 15 maps
       every colour to 0, so a larger shade is darker. */
L50E0: /* _compute_lighting_value */
    /* 50E0  push    ecx */
    push32(ECX);
L50E2:
    /* 50E2  push    ebx */
    push32(EBX);
L50E4:
    /* 50E4  push    esi */
    push32(ESI);
L50E6:
    /* 50E6  push    es */
    push16(asm_es);
L50E7:
    /* 50E7  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L50EA:
    /* 50EA  mov     es,ax */
    SET_ES(AX);
L50EC:
    /* 50EC  mov     eax,dword ptr [si] */
    EAX = rd(pDS, SI);
L50EF:
    /* 50EF  imul    eax */
    imul32(EAX);
L50F2:
    /* 50F2  mov     ecx,eax */
    ECX = EAX;
L50F5:
    /* 50F5  mov     ebx,edx */
    EBX = EDX;
L50F8:
    /* 50F8  mov     eax,dword ptr [si+4] */
    EAX = rd(pDS, SI + 0x4);
L50FC:
    /* 50FC  imul    eax */
    imul32(EAX);
L50FF:
    /* 50FF  add     ecx,eax */
    ECX = add32(ECX, EAX, 0);
L5102:
    /* 5102  adc     ebx,edx */
    EBX = (uint32_t)(EBX + EDX + CF);
L5105:
    /* 5105  mov     eax,dword ptr [si+8] */
    EAX = rd(pDS, SI + 0x8);
L5109:
    /* 5109  imul    eax */
    imul32(EAX);
L510C:
    /* 510C  add     eax,ecx */
    EAX = add32(EAX, ECX, 0);
L510F:
    /* 510F  adc     edx,ebx */
    EDX = (uint32_t)(EDX + EBX + CF);
L5112:
    /* 5112  call    _compute_square_root_32 */
    if ((c = asm_call(ASM_JMP(0x065C, 0x515A), 0x5115)) != 0) return c;
L5115:
    /* 5115  shr     eax,5 */
    EAX = (uint32_t)(EAX >> 5);
L5119:
    /* 5119  movzx   ecx,word ptr es:[64Ch] */
    ECX = (uint32_t)rw(pES, 0x64C);
L5120:
    /* 5120  mul     ecx */
    mul32(ECX);
L5123:
    /* 5123  shr     eax,6 */
    EAX = (uint32_t)(EAX >> 6);
L5127:
    /* 5127  mov     cx,word ptr es:[650h] */
    CX = rw(pES, 0x650);
L512C:
    /* 512C  shl     ecx,10h */
    ECX = (uint32_t)(ECX << 16);
L5130:
    /* 5130  add     eax,ecx */
    EAX = add32(EAX, ECX, 0);
L5133:
    /* 5133  jns     short L5138 */
    if (!SF) goto L5138;
L5135:
    /* 5135  xor     eax,eax */
    EAX = (uint32_t)(EAX ^ EAX);
L5138: /* L5138 */
    /* 5138  mov     cx,word ptr es:[64Eh] */
    CX = rw(pES, 0x64E);
L513D:
    /* 513D  shl     ecx,10h */
    ECX = (uint32_t)(ECX << 16);
L5141:
    /* 5141  add     eax,ecx */
    EAX = (uint32_t)(EAX + ECX);
L5144:
    /* 5144  cmp     eax,0F0000h */
    sub32(EAX, 0xF0000, 0);
L514A:
    /* 514A  jbe     short L5152 */
    if (CF || ZF) goto L5152;
L514C:
    /* 514C  mov     eax,0F0000h */
    EAX = 0xF0000;
L5152: /* L5152 */
    /* 5152  pop     es */
    SET_ES(pop16());
L5153:
    /* 5153  pop     esi */
    ESI = pop32();
L5155:
    /* 5155  pop     ebx */
    EBX = pop32();
L5157:
    /* 5157  pop     ecx */
    ECX = pop32();
L5159:
    /* 5159  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_0849_515A  (+515A)
       compute_square_root_32: the square root of a 64-bit number.
         in:  EDX:EAX
         out: EAX = root; ESI, EBX and ECX changed, EDX kept
       Newton's method: the first estimate is (EDX + 1) << 12, and it iterates estimate = (estimate +
       n / estimate) / 2 until an iteration moves the estimate by less than about 40h. If that first
       estimate overflows it starts from EDX + 1 instead, and returns 0FFFFFFFFh at once when the low
       word of EDX + 1 is 0 (jcxz tests CX only). */
L515A: /* _compute_square_root_32 */
    /* 515A  push    edx */
    push32(EDX);
L515C:
    /* 515C  mov     esi,eax */
    ESI = EAX;
L515F:
    /* 515F  mov     ebx,edx */
    EBX = EDX;
L5162:
    /* 5162  mov     ecx,edx */
    ECX = EDX;
L5165:
    /* 5165  inc     ecx */
    ECX = (uint32_t)(ECX + 1);
L5167:
    /* 5167  shl     ecx,0Ch */
    ECX = (uint32_t)(ECX << 12);
L516B:
    /* 516B  cmp     ecx,edx */
    sub32(ECX, EDX, 0);
L516E:
    /* 516E  ja      short L517E */
    if (!CF && !ZF) goto L517E;
L5170:
    /* 5170  lea     ecx,ds:[edx+1] */
    ECX = (uint32_t)(EDX + 0x1);
L5175:
    /* 5175  sub     eax,eax */
    EAX = (uint32_t)(EAX - EAX);
L5178:
    /* 5178  sub     eax,1 */
    EAX = sub32(EAX, 0x1, 0);
L517C:
    /* 517C  jcxz    L519F */
    if (!CX) goto L519F;
L517E: /* L517E */
    /* 517E  mov     eax,esi */
    EAX = ESI;
L5181:
    /* 5181  mov     edx,ebx */
    EDX = EBX;
L5184:
    /* 5184  div     ecx */
    if (asm_div32(ECX) && (c = asm_divfault(0x065C, 0x5184, 3)) != 0) return c;
L5187:
    /* 5187  add     ecx,eax */
    ECX = add32(ECX, EAX, 0);
L518A:
    /* 518A  rcr     ecx,1 */
    ECX = rcr32(ECX, 1);
L518D:
    /* 518D  sub     eax,ecx */
    EAX = (uint32_t)(EAX - ECX);
L5190:
    /* 5190  add     eax,40h */
    EAX = (uint32_t)(EAX + 0x40);
L5194:
    /* 5194  cmp     eax,80h */
    sub32(EAX, 0x80, 0);
L519A:
    /* 519A  ja      L517E */
    if (!CF && !ZF) goto L517E;
L519C:
    /* 519C  mov     eax,ecx */
    EAX = ECX;
L519F: /* L519F */
    /* 519F  pop     edx */
    EDX = pop32();
L51A1:
    /* 51A1  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
