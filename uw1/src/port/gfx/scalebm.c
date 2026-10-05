/* scalebm.c: replaces src/gfx/SCALEBM.ASM (seg003_1082, 1082..2BBA of its
   segment). Written by tools/asm2c.py from the assembly and the bytes it assembles to; do not
   edit by hand: change the tool, or its OVERRIDES, and run it again. Each instruction is the
   C below its source line; the comments before a routine are the .ASM file's. */
#include "x86/asmrt.h"

void scalebm_run_generated(uint16_t ip);   /* gfx/scalebm_code.c */
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

uint32_t asm_mod_SCALEBM(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x1082: goto L1082;
    case 0x1083: goto L1083;
    case 0x1084: goto L1084;
    case 0x1087: goto L1087;
    case 0x1089: goto L1089;
    case 0x108B: goto L108B;
    case 0x1090: goto L1090;
    case 0x1095: goto L1095;
    case 0x1096: goto L1096;
    case 0x1098: goto L1098;
    case 0x109C: goto L109C;
    case 0x109D: goto L109D;
    case 0x10A1: goto L10A1;
    case 0x10A4: goto L10A4;
    case 0x10A6: goto L10A6;
    case 0x10A9: goto L10A9;
    case 0x10AE: goto L10AE;
    case 0x10B0: goto L10B0;
    case 0x10B3: goto L10B3;
    case 0x10B6: goto L10B6;
    case 0x10B7: goto L10B7;
    case 0x10B9: goto L10B9;
    case 0x10BD: goto L10BD;
    case 0x10BF: goto L10BF;
    case 0x10C1: goto L10C1;
    case 0x10C3: goto L10C3;
    case 0x10C4: goto L10C4;
    case 0x10C6: goto L10C6;
    case 0x10C7: goto L10C7;
    case 0x10C9: goto L10C9;
    case 0x10CB: goto L10CB;
    case 0x10CE: goto L10CE;
    case 0x10D1: goto L10D1;
    case 0x10D3: goto L10D3;
    case 0x10D5: goto L10D5;
    case 0x10D8: goto L10D8;
    case 0x10DA: goto L10DA;
    case 0x10DB: goto L10DB;
    case 0x10DF: goto L10DF;
    case 0x10E0: goto L10E0;
    case 0x10E2: goto L10E2;
    case 0x10E4: goto L10E4;
    case 0x10E6: goto L10E6;
    case 0x10E9: goto L10E9;
    case 0x10ED: goto L10ED;
    case 0x10F0: goto L10F0;
    case 0x10F4: goto L10F4;
    case 0x10F6: goto L10F6;
    case 0x10F8: goto L10F8;
    case 0x10FA: goto L10FA;
    case 0x10FC: goto L10FC;
    case 0x10FF: goto L10FF;
    case 0x1102: goto L1102;
    case 0x1106: goto L1106;
    case 0x110A: goto L110A;
    case 0x110C: goto L110C;
    case 0x110F: goto L110F;
    case 0x1115: goto L1115;
    case 0x1117: goto L1117;
    case 0x111A: goto L111A;
    case 0x111D: goto L111D;
    case 0x1121: goto L1121;
    case 0x1123: goto L1123;
    case 0x1125: goto L1125;
    case 0x1129: goto L1129;
    case 0x112D: goto L112D;
    case 0x1131: goto L1131;
    case 0x1134: goto L1134;
    case 0x1136: goto L1136;
    case 0x113A: goto L113A;
    case 0x113E: goto L113E;
    case 0x1142: goto L1142;
    case 0x1144: goto L1144;
    case 0x1148: goto L1148;
    case 0x114A: goto L114A;
    case 0x114C: goto L114C;
    case 0x114E: goto L114E;
    case 0x1152: goto L1152;
    case 0x1154: goto L1154;
    case 0x1158: goto L1158;
    case 0x115A: goto L115A;
    case 0x115C: goto L115C;
    case 0x115D: goto L115D;
    case 0x1161: goto L1161;
    case 0x1163: goto L1163;
    case 0x1167: goto L1167;
    case 0x1169: goto L1169;
    case 0x116B: goto L116B;
    case 0x116D: goto L116D;
    case 0x1171: goto L1171;
    case 0x1174: goto L1174;
    case 0x1178: goto L1178;
    case 0x117A: goto L117A;
    case 0x117C: goto L117C;
    case 0x117E: goto L117E;
    case 0x1180: goto L1180;
    case 0x1184: goto L1184;
    case 0x1188: goto L1188;
    case 0x118C: goto L118C;
    case 0x1190: goto L1190;
    case 0x1194: goto L1194;
    case 0x1196: goto L1196;
    case 0x1199: goto L1199;
    case 0x119C: goto L119C;
    case 0x119F: goto L119F;
    case 0x11A4: goto L11A4;
    case 0x11A8: goto L11A8;
    case 0x11AB: goto L11AB;
    case 0x11AC: goto L11AC;
    case 0x11AD: goto L11AD;
    case 0x11B1: goto L11B1;
    case 0x11B4: goto L11B4;
    case 0x11B8: goto L11B8;
    case 0x11BC: goto L11BC;
    case 0x11BE: goto L11BE;
    case 0x11C2: goto L11C2;
    case 0x11C6: goto L11C6;
    case 0x11C8: goto L11C8;
    case 0x11C9: goto L11C9;
    case 0x11CE: goto L11CE;
    case 0x11D3: goto L11D3;
    case 0x11D4: goto L11D4;
    case 0x11D5: goto L11D5;
    case 0x11D6: goto L11D6;
    case 0x11D7: goto L11D7;
    case 0x11D8: goto L11D8;
    case 0x11D9: goto L11D9;
    case 0x11DC: goto L11DC;
    case 0x11DE: goto L11DE;
    case 0x11E1: goto L11E1;
    case 0x11E4: goto L11E4;
    case 0x11E7: goto L11E7;
    case 0x11E9: goto L11E9;
    case 0x11EB: goto L11EB;
    case 0x11EF: goto L11EF;
    case 0x11F1: goto L11F1;
    case 0x11F3: goto L11F3;
    case 0x11F5: goto L11F5;
    case 0x11F9: goto L11F9;
    case 0x11FD: goto L11FD;
    case 0x11FE: goto L11FE;
    case 0x11FF: goto L11FF;
    case 0x1200: goto L1200;
    case 0x1D59: goto L1D59;
    case 0x1D5C: goto L1D5C;
    case 0x1D5D: goto L1D5D;
    case 0x1D5F: goto L1D5F;
    case 0x1D61: goto L1D61;
    case 0x1D64: goto L1D64;
    case 0x1D66: goto L1D66;
    case 0x1D67: goto L1D67;
    case 0x1D69: goto L1D69;
    case 0x1D6A: goto L1D6A;
    case 0x1D6B: goto L1D6B;
    case 0x1D6D: goto L1D6D;
    case 0x1D6E: goto L1D6E;
    case 0x1D71: goto L1D71;
    case 0x1D72: goto L1D72;
    case 0x1D74: goto L1D74;
    case 0x1D76: goto L1D76;
    case 0x1D79: goto L1D79;
    case 0x1D7B: goto L1D7B;
    case 0x1D7C: goto L1D7C;
    case 0x1D7E: goto L1D7E;
    case 0x1D7F: goto L1D7F;
    case 0x1D80: goto L1D80;
    case 0x1D82: goto L1D82;
    case 0x1D83: goto L1D83;
    case 0x1D85: goto L1D85;
    case 0x1D88: goto L1D88;
    case 0x1D8A: goto L1D8A;
    case 0x1D8D: goto L1D8D;
    case 0x1D8F: goto L1D8F;
    case 0x1D91: goto L1D91;
    case 0x1D94: goto L1D94;
    case 0x1D95: goto L1D95;
    case 0x1D96: goto L1D96;
    case 0x1D98: goto L1D98;
    case 0x1D9A: goto L1D9A;
    case 0x1D9C: goto L1D9C;
    case 0x1D9E: goto L1D9E;
    case 0x1DA0: goto L1DA0;
    case 0x1DA1: goto L1DA1;
    case 0x1DA2: goto L1DA2;
    case 0x1DA4: goto L1DA4;
    case 0x1DA6: goto L1DA6;
    case 0x1DA8: goto L1DA8;
    case 0x1DA9: goto L1DA9;
    case 0x1DAA: goto L1DAA;
    case 0x1DAC: goto L1DAC;
    case 0x1DAE: goto L1DAE;
    case 0x1DB0: goto L1DB0;
    case 0x1DB2: goto L1DB2;
    case 0x1DB4: goto L1DB4;
    case 0x1DB6: goto L1DB6;
    case 0x1DB8: goto L1DB8;
    case 0x1DBA: goto L1DBA;
    case 0x1DBB: goto L1DBB;
    case 0x1DBC: goto L1DBC;
    case 0x1DBE: goto L1DBE;
    case 0x1DBF: goto L1DBF;
    case 0x1DC0: goto L1DC0;
    case 0x1DC1: goto L1DC1;
    case 0x1DC3: goto L1DC3;
    case 0x1DC5: goto L1DC5;
    case 0x1DC7: goto L1DC7;
    case 0x1DC8: goto L1DC8;
    case 0x1DC9: goto L1DC9;
    case 0x1DCB: goto L1DCB;
    case 0x1DCD: goto L1DCD;
    case 0x1DCF: goto L1DCF;
    case 0x1DD0: goto L1DD0;
    case 0x1DD1: goto L1DD1;
    case 0x1DD3: goto L1DD3;
    case 0x1DD5: goto L1DD5;
    case 0x1DD7: goto L1DD7;
    case 0x1DD9: goto L1DD9;
    case 0x1DDB: goto L1DDB;
    case 0x1DDD: goto L1DDD;
    case 0x1DDE: goto L1DDE;
    case 0x1DDF: goto L1DDF;
    case 0x1DE3: goto L1DE3;
    case 0x1DE4: goto L1DE4;
    case 0x1DE5: goto L1DE5;
    case 0x1DE7: goto L1DE7;
    case 0x1DE9: goto L1DE9;
    case 0x1DEB: goto L1DEB;
    case 0x1DED: goto L1DED;
    case 0x1DEE: goto L1DEE;
    case 0x1DEF: goto L1DEF;
    case 0x1DF1: goto L1DF1;
    case 0x1DF3: goto L1DF3;
    case 0x1DF5: goto L1DF5;
    case 0x1DF6: goto L1DF6;
    case 0x1DF7: goto L1DF7;
    case 0x1DF9: goto L1DF9;
    case 0x1DFB: goto L1DFB;
    case 0x1DFD: goto L1DFD;
    case 0x1DFF: goto L1DFF;
    case 0x1E01: goto L1E01;
    case 0x1E03: goto L1E03;
    case 0x1E05: goto L1E05;
    case 0x1E06: goto L1E06;
    case 0x1E07: goto L1E07;
    case 0x1E09: goto L1E09;
    case 0x1E0B: goto L1E0B;
    case 0x1E0E: goto L1E0E;
    case 0x1E10: goto L1E10;
    case 0x1E13: goto L1E13;
    case 0x1E18: goto L1E18;
    case 0x1E1A: goto L1E1A;
    case 0x1E1C: goto L1E1C;
    case 0x1E1E: goto L1E1E;
    case 0x1E21: goto L1E21;
    case 0x1E23: goto L1E23;
    case 0x1E25: goto L1E25;
    case 0x1E28: goto L1E28;
    case 0x1E2A: goto L1E2A;
    case 0x1E2C: goto L1E2C;
    case 0x1E2E: goto L1E2E;
    case 0x1E31: goto L1E31;
    case 0x1E33: goto L1E33;
    case 0x1E35: goto L1E35;
    case 0x1E36: goto L1E36;
    case 0x1E38: goto L1E38;
    case 0x1E3B: goto L1E3B;
    case 0x1E3C: goto L1E3C;
    case 0x1E3E: goto L1E3E;
    case 0x1E3F: goto L1E3F;
    case 0x1E41: goto L1E41;
    case 0x1E42: goto L1E42;
    case 0x1E44: goto L1E44;
    case 0x1E45: goto L1E45;
    case 0x1E47: goto L1E47;
    case 0x1E48: goto L1E48;
    case 0x1E4B: goto L1E4B;
    case 0x1E4C: goto L1E4C;
    case 0x1E4D: goto L1E4D;
    case 0x1E51: goto L1E51;
    case 0x1E53: goto L1E53;
    case 0x1E55: goto L1E55;
    case 0x1E58: goto L1E58;
    case 0x1E5A: goto L1E5A;
    case 0x1E5C: goto L1E5C;
    case 0x1E5E: goto L1E5E;
    case 0x1E60: goto L1E60;
    case 0x1E62: goto L1E62;
    case 0x1E66: goto L1E66;
    case 0x1E68: goto L1E68;
    case 0x1E6B: goto L1E6B;
    case 0x1E6C: goto L1E6C;
    case 0x1E6F: goto L1E6F;
    case 0x1E70: goto L1E70;
    case 0x1E72: goto L1E72;
    case 0x1E74: goto L1E74;
    case 0x1E75: goto L1E75;
    case 0x1E78: goto L1E78;
    case 0x1E79: goto L1E79;
    case 0x1E7C: goto L1E7C;
    case 0x1E7D: goto L1E7D;
    case 0x1E7F: goto L1E7F;
    case 0x1E80: goto L1E80;
    case 0x1E82: goto L1E82;
    case 0x1E83: goto L1E83;
    case 0x1E85: goto L1E85;
    case 0x1E86: goto L1E86;
    case 0x1E8A: goto L1E8A;
    case 0x1E8C: goto L1E8C;
    case 0x1E8E: goto L1E8E;
    case 0x1E8F: goto L1E8F;
    case 0x1E90: goto L1E90;
    case 0x1E94: goto L1E94;
    case 0x1E97: goto L1E97;
    case 0x1E9A: goto L1E9A;
    case 0x1E9D: goto L1E9D;
    case 0x1EA3: goto L1EA3;
    case 0x1EA5: goto L1EA5;
    case 0x1EA9: goto L1EA9;
    case 0x1EAB: goto L1EAB;
    case 0x1EAD: goto L1EAD;
    case 0x1EAF: goto L1EAF;
    case 0x1EB2: goto L1EB2;
    case 0x1EB5: goto L1EB5;
    case 0x1EB7: goto L1EB7;
    case 0x1EBA: goto L1EBA;
    case 0x1EBC: goto L1EBC;
    case 0x1EBF: goto L1EBF;
    case 0x1EC1: goto L1EC1;
    case 0x1EC3: goto L1EC3;
    case 0x1EC5: goto L1EC5;
    case 0x1EC6: goto L1EC6;
    case 0x1EC8: goto L1EC8;
    case 0x1ECA: goto L1ECA;
    case 0x1ECC: goto L1ECC;
    case 0x1ECD: goto L1ECD;
    case 0x1ECF: goto L1ECF;
    case 0x1ED2: goto L1ED2;
    case 0x1ED4: goto L1ED4;
    case 0x1ED5: goto L1ED5;
    case 0x1ED7: goto L1ED7;
    case 0x1ED9: goto L1ED9;
    case 0x1EDB: goto L1EDB;
    case 0x1EDD: goto L1EDD;
    case 0x1EDF: goto L1EDF;
    case 0x1EE1: goto L1EE1;
    case 0x1EE2: goto L1EE2;
    case 0x1EE4: goto L1EE4;
    case 0x1EE6: goto L1EE6;
    case 0x1EE8: goto L1EE8;
    case 0x1EEA: goto L1EEA;
    case 0x1EEC: goto L1EEC;
    case 0x1EEE: goto L1EEE;
    case 0x1EF0: goto L1EF0;
    case 0x1EF2: goto L1EF2;
    case 0x1EF4: goto L1EF4;
    case 0x1EF5: goto L1EF5;
    case 0x1EF7: goto L1EF7;
    case 0x1EF9: goto L1EF9;
    case 0x1EFB: goto L1EFB;
    case 0x1EFC: goto L1EFC;
    case 0x1EFE: goto L1EFE;
    case 0x1F00: goto L1F00;
    case 0x1F02: goto L1F02;
    case 0x1F03: goto L1F03;
    case 0x1F05: goto L1F05;
    case 0x1F07: goto L1F07;
    case 0x1F09: goto L1F09;
    case 0x1F0B: goto L1F0B;
    case 0x1F0D: goto L1F0D;
    case 0x1F10: goto L1F10;
    case 0x1F12: goto L1F12;
    case 0x1F14: goto L1F14;
    case 0x1F15: goto L1F15;
    case 0x1F17: goto L1F17;
    case 0x1F19: goto L1F19;
    case 0x1F1B: goto L1F1B;
    case 0x1F1E: goto L1F1E;
    case 0x1F20: goto L1F20;
    case 0x1F22: goto L1F22;
    case 0x1F24: goto L1F24;
    case 0x1F25: goto L1F25;
    case 0x1F27: goto L1F27;
    case 0x1F2A: goto L1F2A;
    case 0x1F2B: goto L1F2B;
    case 0x1F2D: goto L1F2D;
    case 0x1F2E: goto L1F2E;
    case 0x1F30: goto L1F30;
    case 0x1F31: goto L1F31;
    case 0x1F33: goto L1F33;
    default: asm_bad_entry("SCALEBM.ASM", entry);
    }

    /* seg003_1082  (+1082)
       scale_nibble_bitmap (FM Towns; far): draw the sprite described at 0B0C .. 0B3F. Gives up for a
       sprite 2 pixels or less in either direction, one wider than 200h, or one wholly outside the
       window. Works out the steps, clips left, right, top and bottom, generates the row scaler into
       L1F3A (through 0B10), then for each screen row from the top down: when the source row changes,
       unpack and scale it into 0B42 by calling L1F3A; write the scaled row into the frame buffer at
       row 095E[y] + x through the span writer (0B12). Runs on the library's stack. */
L1082: /* _seg003_1082 */
    /* 1082  push    es */
    push16(asm_es);
L1083:
    /* 1083  push    ds */
    push16(asm_ds);
L1084:
    /* 1084  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L1087:
    /* 1087  mov     ds,ax */
    SET_DS(AX);
L1089:
    /* 1089  mov     bx,ss */
    BX = asm_ss;
L108B:
    /* 108B  mov     word ptr cs:L1F36,bx */
    ww(CODE003, 0x1F36, BX);
L1090:
    /* 1090  mov     word ptr cs:L1F38,sp */
    ww(CODE003, 0x1F38, SP);
L1095:
    /* 1095  cli */
    ;
L1096:
    /* 1096  mov     ss,ax */
    SET_SS(AX);
L1098:
    /* 1098  mov     sp,word ptr ds:[5586h] */
    SP = rw(pDS, 0x5586);
L109C:
    /* 109C  sti */
    ;
L109D:
    /* 109D  mov     cx,word ptr ds:[0B28h] */
    CX = rw(pDS, 0xB28);
L10A1:
    /* 10A1  cmp     cx,2 */
    sub16(CX, 0x2, 0);
L10A4:
    /* 10A4  jg      L10A9 */
    if (!ZF && SF == OF) goto L10A9;
L10A6:
    /* 10A6  jmp     L11C8 */
    goto L11C8;
L10A9: /* L10A9 */
    /* 10A9  cmp     word ptr ds:[0B26h],2 */
    sub16(rw(pDS, 0xB26), 0x2, 0);
L10AE:
    /* 10AE  jg      L10B3 */
    if (!ZF && SF == OF) goto L10B3;
L10B0:
    /* 10B0  jmp     L11C8 */
    goto L11C8;
L10B3: /* L10B3 */
    /* 10B3  mov     ax,word ptr ds:[0B1Eh] */
    AX = rw(pDS, 0xB1E);
L10B6:
    /* 10B6  dec     ax */
    AX = (uint16_t)(AX - 1);
L10B7:
    /* 10B7  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L10B9:
    /* 10B9  mov     word ptr ds:[0B40h],dx */
    ww(pDS, 0xB40, DX);
L10BD:
    /* 10BD  mov     dl,ah */
    DL = AH;
L10BF:
    /* 10BF  mov     ah,al */
    AH = AL;
L10C1:
    /* 10C1  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L10C3:
    /* 10C3  dec     cx */
    CX = (uint16_t)(CX - 1);
L10C4:
    /* 10C4  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x0090, 0x10C4, 2)) != 0) return c;
L10C6:
    /* 10C6  inc     cx */
    CX = (uint16_t)(CX + 1);
L10C7:
    /* 10C7  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L10C9:
    /* 10C9  jne     L10CE */
    if (!ZF) goto L10CE;
L10CB:
    /* 10CB  jmp     L11C8 */
    goto L11C8;
L10CE: /* L10CE */
    /* 10CE  mov     word ptr ds:[0B3Ah],ax */
    ww(pDS, 0xB3A, AX);
L10D1:
    /* 10D1  mov     ax,cs */
    AX = (uint16_t)(0x0090 + PORT_LOAD_SEG);
L10D3:
    /* 10D3  mov     es,ax */
    SET_ES(AX);
L10D5:
    /* 10D5  mov     ax,word ptr ds:[0B22h] */
    AX = rw(pDS, 0xB22);
L10D8:
    /* 10D8  add     ax,cx */
    AX = (uint16_t)(AX + CX);
L10DA:
    /* 10DA  dec     ax */
    AX = (uint16_t)(AX - 1);
L10DB:
    /* 10DB  sub     ax,word ptr ds:[WIN_RIGHT] */
    AX = (uint16_t)(AX - rw(pDS, 0x3DF6));
L10DF:
    /* 10DF  dec     ax */
    AX = dec16(AX);
L10E0:
    /* 10E0  jle     L10ED */
    if (ZF || SF != OF) goto L10ED;
L10E2:
    /* 10E2  sub     cx,ax */
    CX = sub16(CX, AX, 0);
L10E4:
    /* 10E4  jg      L10E9 */
    if (!ZF && SF == OF) goto L10E9;
L10E6:
    /* 10E6  jmp     L11C8 */
    goto L11C8;
L10E9: /* L10E9 */
    /* 10E9  mov     word ptr ds:[0B28h],cx */
    ww(pDS, 0xB28, CX);
L10ED: /* L10ED */
    /* 10ED  mov     ax,word ptr ds:[0B22h] */
    AX = rw(pDS, 0xB22);
L10F0:
    /* 10F0  mov     si,word ptr ds:[WIN_LEFT] */
    SI = rw(pDS, 0x3DF2);
L10F4:
    /* 10F4  sub     ax,si */
    AX = sub16(AX, SI, 0);
L10F6:
    /* 10F6  jge     L110F */
    if (SF == OF) goto L110F;
L10F8:
    /* 10F8  neg     ax */
    AX = neg16(AX);
L10FA:
    /* 10FA  jg      L10FF */
    if (!ZF && SF == OF) goto L10FF;
L10FC:
    /* 10FC  jmp     L11C8 */
    goto L11C8;
L10FF: /* L10FF */
    /* 10FF  mov     word ptr ds:[0B40h],ax */
    ww(pDS, 0xB40, AX);
L1102:
    /* 1102  mov     word ptr ds:[0B22h],si */
    ww(pDS, 0xB22, SI);
L1106:
    /* 1106  sub     word ptr ds:[0B28h],ax */
    ww(pDS, 0xB28, sub16(rw(pDS, 0xB28), AX, 0));
L110A:
    /* 110A  jg      L110F */
    if (!ZF && SF == OF) goto L110F;
L110C:
    /* 110C  jmp     L11C8 */
    goto L11C8;
L110F: /* L110F */
    /* 110F  cmp     word ptr ds:[0B28h],200h */
    sub16(rw(pDS, 0xB28), 0x200, 0);
L1115:
    /* 1115  jl      L111A */
    if (SF != OF) goto L111A;
L1117:
    /* 1117  jmp     L11C8 */
    goto L11C8;
L111A: /* L111A */
    /* 111A  mov     di,offset L1F3A */
    DI = 0x1F3A;
L111D:
    /* 111D  call    word ptr ds:[0B10h] */
    if ((c = asm_call(ASM_JMP(0x0090, rw(pDS, 0xB10)), 0x1121)) != 0) return c;
L1121:
    /* 1121  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L1123:
    /* 1123  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L1125:
    /* 1125  mov     ah,byte ptr ds:[0B20h] */
    AH = rb(pDS, 0xB20);
L1129:
    /* 1129  mov     dl,byte ptr ds:[0B21h] */
    DL = rb(pDS, 0xB21);
L112D:
    /* 112D  div     word ptr ds:[0B26h] */
    if (asm_div16(rw(pDS, 0xB26)) && (c = asm_divfault(0x0090, 0x112D, 4)) != 0) return c;
L1131:
    /* 1131  mov     word ptr ds:[0B3Ch],ax */
    ww(pDS, 0xB3C, AX);
L1134:
    /* 1134  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L1136:
    /* 1136  mov     cx,word ptr ds:[0B26h] */
    CX = rw(pDS, 0xB26);
L113A:
    /* 113A  mov     bp,word ptr ds:[0B24h] */
    BP = rw(pDS, 0xB24);
L113E:
    /* 113E  cmp     bp,word ptr ds:[WIN_TOP] */
    sub16(BP, rw(pDS, 0x3DF4), 0);
L1142:
    /* 1142  jle     L1158 */
    if (ZF || SF != OF) goto L1158;
L1144:
    /* 1144  sub     bp,word ptr ds:[WIN_TOP] */
    BP = (uint16_t)(BP - rw(pDS, 0x3DF4));
L1148:
    /* 1148  sub     cx,bp */
    CX = sub16(CX, BP, 0);
L114A:
    /* 114A  jle     L11C8 */
    if (ZF || SF != OF) goto L11C8;
L114C:
    /* 114C  mov     ax,bp */
    AX = BP;
L114E:
    /* 114E  mul     word ptr ds:[0B3Ch] */
    mul16(rw(pDS, 0xB3C));
L1152:
    /* 1152  mov     dx,ax */
    DX = AX;
L1154:
    /* 1154  mov     bp,word ptr ds:[WIN_TOP] */
    BP = rw(pDS, 0x3DF4);
L1158: /* L1158 */
    /* 1158  mov     bx,bp */
    BX = BP;
L115A:
    /* 115A  sub     bx,cx */
    BX = (uint16_t)(BX - CX);
L115C:
    /* 115C  inc     bx */
    BX = (uint16_t)(BX + 1);
L115D:
    /* 115D  cmp     bx,word ptr ds:[WIN_BOTTOM] */
    sub16(BX, rw(pDS, 0x3DF8), 0);
L1161:
    /* 1161  jge     L116B */
    if (SF == OF) goto L116B;
L1163:
    /* 1163  sub     bx,word ptr ds:[WIN_BOTTOM] */
    BX = (uint16_t)(BX - rw(pDS, 0x3DF8));
L1167:
    /* 1167  add     cx,bx */
    CX = add16(CX, BX, 0);
L1169:
    /* 1169  jle     L11C8 */
    if (ZF || SF != OF) goto L11C8;
L116B: /* L116B */
    /* 116B  shl     bp,1 */
    BP = (uint16_t)(BP << 1);
L116D:
    /* 116D  add     bp,95Eh */
    BP = (uint16_t)(BP + 0x95E);
L1171:
    /* 1171  mov     bx,0B2Ah */
    BX = 0xB2A;
L1174:
    /* 1174  mov     word ptr ds:[0B3Eh],cx */
    ww(pDS, 0xB3E, CX);
L1178:
    /* 1178  mov     cl,0FFh */
    CL = 0xFF;
L117A: /* L117A */
    /* 117A  cmp     cl,dh */
    sub8(CL, DH, 0);
L117C:
    /* 117C  je      L11A4 */
    if (ZF) goto L11A4;
L117E:
    /* 117E  mov     al,dh */
    AL = DH;
L1180:
    /* 1180  mul     byte ptr ds:[0DC2h] */
    mul8(rb(pDS, 0xDC2));
L1184:
    /* 1184  mov     si,word ptr ds:[0B1Ah] */
    SI = rw(pDS, 0xB1A);
L1188:
    /* 1188  add     si,word ptr ds:[0B40h] */
    SI = (uint16_t)(SI + rw(pDS, 0xB40));
L118C:
    /* 118C  mov     es,word ptr ds:[0B0Eh] */
    SET_ES(rw(pDS, 0xB0E));
L1190:
    /* 1190  mov     ds,word ptr ds:[0B1Ch] */
    SET_DS(rw(pDS, 0xB1C));
L1194:
    /* 1194  add     si,ax */
    SI = (uint16_t)(SI + AX);
L1196:
    /* 1196  mov     di,0B42h */
    DI = 0xB42;
L1199:
    /* 1199  mov     cx,0F04h */
    CX = 0xF04;
L119C:
    /* 119C  call    near ptr L1F3A */
    /* by hand: call near ptr L1F3A: the scaler the generators wrote is interpreted (gfx/scalebm_code.c), as UW2's */
    scalebm_run_generated(0x1F3A);
L119F:
    /* 119F  mov     ds,word ptr ss:[0B0Eh] */
    SET_DS(rw(pSS, 0xB0E));
L11A4: /* L11A4 */
    /* 11A4  mov     es,word ptr ds:[95Ah] */
    SET_ES(rw(pDS, 0x95A));
L11A8:
    /* 11A8  mov     di,word ptr [bp] */
    DI = rw(pSS, BP);
L11AB:
    /* 11AB  dec     bp */
    BP = (uint16_t)(BP - 1);
L11AC:
    /* 11AC  dec     bp */
    BP = (uint16_t)(BP - 1);
L11AD:
    /* 11AD  add     di,word ptr ds:[0B22h] */
    DI = (uint16_t)(DI + rw(pDS, 0xB22));
L11B1:
    /* 11B1  mov     si,0B42h */
    SI = 0xB42;
L11B4:
    /* 11B4  mov     cx,word ptr ds:[0B28h] */
    CX = rw(pDS, 0xB28);
L11B8:
    /* 11B8  call    word ptr ds:[0B12h] */
    if ((c = asm_call(ASM_JMP(0x0090, rw(pDS, 0xB12)), 0x11BC)) != 0) return c;
L11BC:
    /* 11BC  mov     cl,dh */
    CL = DH;
L11BE:
    /* 11BE  add     dx,word ptr ds:[0B3Ch] */
    DX = add16(DX, rw(pDS, 0xB3C), 0);
L11C2:
    /* 11C2  dec     word ptr ds:[0B3Eh] */
    ww(pDS, 0xB3E, dec16(rw(pDS, 0xB3E)));
L11C6:
    /* 11C6  jne     L117A */
    if (!ZF) goto L117A;
L11C8: /* L11C8 */
    /* 11C8  cli */
    ;
L11C9:
    /* 11C9  mov     ss,word ptr cs:L1F36 */
    SET_SS(rw(CODE003, 0x1F36));
L11CE:
    /* 11CE  mov     sp,word ptr cs:L1F38 */
    SP = rw(CODE003, 0x1F38);
L11D3:
    /* 11D3  sti */
    ;
L11D4:
    /* 11D4  pop     ds */
    SET_DS(pop16());
L11D5:
    /* 11D5  pop     es */
    SET_ES(pop16());
L11D6:
    /* 11D6  push    es */
    push16(asm_es);
L11D7:
    /* 11D7  push    di */
    push16(DI);
L11D8:
    /* 11D8  push    ax */
    push16(AX);
L11D9:
    /* 11D9  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L11DC:
    /* 11DC  mov     es,ax */
    SET_ES(AX);
L11DE:
    /* 11DE  mov     ax,0 */
    AX = 0x0;
L11E1:
    /* 11E1  mov     cx,0FFh */
    CX = 0xFF;
L11E4:
    /* 11E4  mov     di,8 */
    DI = 0x8;
L11E7:
    /* 11E7  repe scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (!ZF) break; }
L11E9:
    /* 11E9  jcxz    L11FD */
    if (!CX) goto L11FD;
L11EB:
    /* 11EB  mov     al,byte ptr es:[658h] */
    AL = rb(pES, 0x658);
L11EF:
    /* 11EF  test    al,0FFh */
    logic8((uint8_t)(AL & 0xFF));
L11F1:
    /* 11F1  jne     L11F9 */
    if (!ZF) goto L11F9;
L11F3:
    /* 11F3  mov     al,60h */
    AL = 0x60;
L11F5:
    /* 11F5  mov     byte ptr es:[658h],al */
    wb(pES, 0x658, AL);
L11F9: /* L11F9 */
    /* 11F9  mov     byte ptr es:[PEN_COLOR],al */
    wb(pES, 0x410F, AL);
L11FD: /* L11FD */
    /* 11FD  pop     ax */
    AX = pop16();
L11FE:
    /* 11FE  pop     di */
    DI = pop16();
L11FF:
    /* 11FF  pop     es */
    SET_ES(pop16());
L1200:
    /* 1200  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_1D59  (+1D59): span routines and the scaler generators, reached through
       The span writers (CX pixels from SI at 0B42 to ES:DI in the frame buffer): _1D59 remaps the
       screen pixel under each non-zero source pixel through cXfer table 0 (a translucent sprite),
       L1D6E the same with table 1 (UW2 tables 4 and 3); the unlabelled writer after L1D83 writes
       non-zero pixels, those
       from 0FBh through the cXfer table they name; then a plain copy (rep movsb), a transparent copy
       (skip 0), and a writer that puts bmcolor (0DC3) wherever the source is non-zero, for pick
       frames.

       The generators (write code to ES:DI = L1F3A, ending it with a ret): the first (after L1E06)
       unpacks four-bit pixels, choosing the nibble by the parity of the source position and
       translating through a 16-entry palette with xlat ss:; the one after L1E8C scales a byte row,
       emitting movsb for pixels kept once, lodsb then several stosb for pixels repeated when
       enlarging (step below 1.0), or skipping source bytes with inc si / add si,n when shrinking
       (step above 1.0). */
L1D59: /* _seg003_1D59 */
    /* 1D59  mov     bx,offset _cXfer */
    BX = 0x1201;
L1D5C: /* L1D5C */
    /* 1D5C  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1D5D:
    /* 1D5D  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L1D5F:
    /* 1D5F  je      L1D6A */
    if (ZF) goto L1D6A;
L1D61:
    /* 1D61  mov     al,byte ptr es:[di] */
    AL = rb(pES, DI);
L1D64:
    /* 1D64  xlat    byte ptr cs:[bx] */
    AL = rb(CODE003, BX + AL);
L1D66: /* L1D66 */
    /* 1D66  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1D67:
    /* 1D67  loop    L1D5C */
    if (--CX) goto L1D5C;
L1D69:
    /* 1D69  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L1D6A: /* L1D6A */
    /* 1D6A  inc     di */
    DI = inc16(DI);
L1D6B:
    /* 1D6B  loop    L1D5C */
    if (--CX) goto L1D5C;
L1D6D:
    /* 1D6D  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L1D6E: /* L1D6E */
    /* 1D6E  mov     bx,offset _cXfer+100h */
    BX = 0x1301;
L1D71: /* L1D71 */
    /* 1D71  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1D72:
    /* 1D72  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L1D74:
    /* 1D74  je      L1D7F */
    if (ZF) goto L1D7F;
L1D76:
    /* 1D76  mov     al,byte ptr es:[di] */
    AL = rb(pES, DI);
L1D79:
    /* 1D79  xlat    byte ptr cs:[bx] */
    AL = rb(CODE003, BX + AL);
L1D7B:
    /* 1D7B  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1D7C:
    /* 1D7C  loop    L1D71 */
    if (--CX) goto L1D71;
L1D7E:
    /* 1D7E  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L1D7F: /* L1D7F */
    /* 1D7F  inc     di */
    DI = inc16(DI);
L1D80:
    /* 1D80  loop    L1D71 */
    if (--CX) goto L1D71;
L1D82:
    /* 1D82  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L1D83: /* L1D83 */
    /* 1D83  sub     al,0FBh */
    AL = (uint8_t)(AL - 0xFB);
L1D85:
    /* 1D85  mov     bx,offset _cXfer */
    BX = 0x1201;
L1D88:
    /* 1D88  add     bh,al */
    BH = (uint8_t)(BH + AL);
L1D8A:
    /* 1D8A  mov     al,byte ptr es:[di] */
    AL = rb(pES, DI);
L1D8D:
    /* 1D8D  xlat    byte ptr cs:[bx] */
    AL = rb(CODE003, BX + AL);
L1D8F:
    /* 1D8F  jmp     short L1DA0 */
    goto L1DA0;
L1D91:
    /* 1D91  mov     bx,offset _cXfer */
    BX = 0x1201;
L1D94:
    /* 1D94  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1D95:
    /* 1D95  dec     cx */
    CX = dec16(CX);
L1D96:
    /* 1D96  je      L1DB2 */
    if (ZF) goto L1DB2;
L1D98:
    /* 1D98  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L1D9A:
    /* 1D9A  je      L1DA8 */
    if (ZF) goto L1DA8;
L1D9C: /* L1D9C */
    /* 1D9C  cmp     al,0FBh */
    sub8(AL, 0xFB, 0);
L1D9E:
    /* 1D9E  jae     L1D83 */
    if (!CF) goto L1D83;
L1DA0: /* L1DA0 */
    /* 1DA0  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1DA1:
    /* 1DA1  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1DA2:
    /* 1DA2  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L1DA4:
    /* 1DA4  loopne  L1D9C */
    if (--CX && !ZF) goto L1D9C;
L1DA6:
    /* 1DA6  jcxz    L1DB4 */
    if (!CX) goto L1DB4;
L1DA8: /* L1DA8 */
    /* 1DA8  inc     di */
    DI = (uint16_t)(DI + 1);
L1DA9:
    /* 1DA9  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1DAA:
    /* 1DAA  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L1DAC:
    /* 1DAC  loope   L1DA8 */
    if (--CX && ZF) goto L1DA8;
L1DAE:
    /* 1DAE  jcxz    L1DB4 */
    if (!CX) goto L1DB4;
L1DB0:
    /* 1DB0  jmp     L1D9C */
    goto L1D9C;
L1DB2: /* L1DB2 */
    /* 1DB2  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L1DB4: /* L1DB4 */
    /* 1DB4  je      L1DBB */
    if (ZF) goto L1DBB;
L1DB6:
    /* 1DB6  cmp     al,0FBh */
    sub8(AL, 0xFB, 0);
L1DB8:
    /* 1DB8  jae     L1DBB */
    if (!CF) goto L1DBB;
L1DBA:
    /* 1DBA  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1DBB: /* L1DBB */
    /* 1DBB  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L1DBC:
    /* 1DBC  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L1DBE:
    /* 1DBE  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L1DBF:
    /* 1DBF  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1DC0:
    /* 1DC0  dec     cx */
    CX = dec16(CX);
L1DC1:
    /* 1DC1  je      L1DD9 */
    if (ZF) goto L1DD9;
L1DC3:
    /* 1DC3  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L1DC5:
    /* 1DC5  je      L1DCF */
    if (ZF) goto L1DCF;
L1DC7: /* L1DC7 */
    /* 1DC7  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1DC8:
    /* 1DC8  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1DC9:
    /* 1DC9  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L1DCB:
    /* 1DCB  loopne  L1DC7 */
    if (--CX && !ZF) goto L1DC7;
L1DCD:
    /* 1DCD  jcxz    L1DDB */
    if (!CX) goto L1DDB;
L1DCF: /* L1DCF */
    /* 1DCF  inc     di */
    DI = (uint16_t)(DI + 1);
L1DD0:
    /* 1DD0  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1DD1:
    /* 1DD1  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L1DD3:
    /* 1DD3  loope   L1DCF */
    if (--CX && ZF) goto L1DCF;
L1DD5:
    /* 1DD5  jcxz    L1DDB */
    if (!CX) goto L1DDB;
L1DD7:
    /* 1DD7  jmp     L1DC7 */
    goto L1DC7;
L1DD9: /* L1DD9 */
    /* 1DD9  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L1DDB: /* L1DDB */
    /* 1DDB  je      L1DDE */
    if (ZF) goto L1DDE;
L1DDD:
    /* 1DDD  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1DDE: /* L1DDE */
    /* 1DDE  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L1DDF:
    /* 1DDF  mov     bl,byte ptr ds:[0DC3h] */
    BL = rb(pDS, 0xDC3);
L1DE3:
    /* 1DE3  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1DE4:
    /* 1DE4  dec     cx */
    CX = dec16(CX);
L1DE5:
    /* 1DE5  je      L1DFF */
    if (ZF) goto L1DFF;
L1DE7:
    /* 1DE7  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L1DE9:
    /* 1DE9  je      L1DF5 */
    if (ZF) goto L1DF5;
L1DEB: /* L1DEB */
    /* 1DEB  mov     al,bl */
    AL = BL;
L1DED:
    /* 1DED  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1DEE:
    /* 1DEE  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1DEF:
    /* 1DEF  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L1DF1:
    /* 1DF1  loopne  L1DEB */
    if (--CX && !ZF) goto L1DEB;
L1DF3:
    /* 1DF3  jcxz    L1E01 */
    if (!CX) goto L1E01;
L1DF5: /* L1DF5 */
    /* 1DF5  inc     di */
    DI = (uint16_t)(DI + 1);
L1DF6:
    /* 1DF6  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L1DF7:
    /* 1DF7  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L1DF9:
    /* 1DF9  loope   L1DF5 */
    if (--CX && ZF) goto L1DF5;
L1DFB:
    /* 1DFB  jcxz    L1E01 */
    if (!CX) goto L1E01;
L1DFD:
    /* 1DFD  jmp     L1DEB */
    goto L1DEB;
L1DFF: /* L1DFF */
    /* 1DFF  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L1E01: /* L1E01 */
    /* 1E01  je      L1E06 */
    if (ZF) goto L1E06;
L1E03:
    /* 1E03  mov     al,bl */
    AL = BL;
L1E05:
    /* 1E05  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1E06: /* L1E06 */
    /* 1E06  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L1E07:
    /* 1E07  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L1E09:
    /* 1E09  xor     bp,bp */
    BP = (uint16_t)(BP ^ BP);
L1E0B:
    /* 1E0B  mov     ax,word ptr ds:[0B1Eh] */
    AX = rw(pDS, 0xB1E);
L1E0E:
    /* 1E0E  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L1E10:
    /* 1E10  mov     byte ptr ds:[0DC2h],al */
    wb(pDS, 0xDC2, AL);
L1E13:
    /* 1E13  test    byte ptr ds:[0B3Bh],0FFh */
    logic8((uint8_t)(rb(pDS, 0xB3B) & 0xFF));
L1E18:
    /* 1E18  je      L1E55 */
    if (ZF) goto L1E55;
L1E1A: /* L1E1A */
    /* 1E1A  xor     bx,bx */
    BX = (uint16_t)(BX ^ BX);
L1E1C:
    /* 1E1C  mov     bl,dh */
    BL = DH;
L1E1E:
    /* 1E1E  mov     si,0E820h */
    SI = 0xE820;
L1E21:
    /* 1E21  shr     bx,1 */
    BX = shr16(BX, 1);
L1E23:
    /* 1E23  jb      L1E28 */
    if (CF) goto L1E28;
L1E25:
    /* 1E25  mov     si,0E8D2h */
    SI = 0xE8D2;
L1E28: /* L1E28 */
    /* 1E28  sub     bx,bp */
    BX = sub16(BX, BP, 0);
L1E2A:
    /* 1E2A  je      L1E3F */
    if (ZF) goto L1E3F;
L1E2C:
    /* 1E2C  add     bp,bx */
    BP = (uint16_t)(BP + BX);
L1E2E:
    /* 1E2E  cmp     bl,0FFh */
    sub8(BL, 0xFF, 0);
L1E31:
    /* 1E31  jne     L1E38 */
    if (!ZF) goto L1E38;
L1E33:
    /* 1E33  mov     al,4Eh */
    AL = 0x4E;
L1E35:
    /* 1E35  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1E36:
    /* 1E36  jmp     short L1E3F */
    goto L1E3F;
L1E38: /* L1E38 */
    /* 1E38  mov     ax,0C683h */
    AX = 0xC683;
L1E3B:
    /* 1E3B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1E3C:
    /* 1E3C  mov     al,bl */
    AL = BL;
L1E3E:
    /* 1E3E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1E3F: /* L1E3F */
    /* 1E3F  mov     al,0ACh */
    AL = 0xAC;
L1E41:
    /* 1E41  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1E42:
    /* 1E42  mov     ax,si */
    AX = SI;
L1E44:
    /* 1E44  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1E45:
    /* 1E45  mov     al,36h */
    AL = 0x36;
L1E47:
    /* 1E47  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1E48:
    /* 1E48  mov     ax,0AAD7h */
    AX = 0xAAD7;
L1E4B:
    /* 1E4B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1E4C:
    /* 1E4C  inc     bp */
    BP = (uint16_t)(BP + 1);
L1E4D:
    /* 1E4D  add     dx,word ptr ds:[0B3Ah] */
    DX = add16(DX, rw(pDS, 0xB3A), 0);
L1E51:
    /* 1E51  loop    L1E1A */
    if (--CX) goto L1E1A;
L1E53:
    /* 1E53  jmp     short L1E8C */
    goto L1E8C;
L1E55: /* L1E55 */
    /* 1E55  mov     bp,0FFF8h */
    BP = 0xFFF8;
L1E58: /* L1E58 */
    /* 1E58  xor     bx,bx */
    BX = (uint16_t)(BX ^ BX);
L1E5A:
    /* 1E5A  mov     bl,dh */
    BL = DH;
L1E5C:
    /* 1E5C  cmp     bx,bp */
    sub16(BX, BP, 0);
L1E5E:
    /* 1E5E  je      L1E83 */
    if (ZF) goto L1E83;
L1E60:
    /* 1E60  mov     bp,bx */
    BP = BX;
L1E62:
    /* 1E62  test    bx,1 */
    logic16((uint16_t)(BX & 0x1));
L1E66:
    /* 1E66  je      L1E72 */
    if (ZF) goto L1E72;
L1E68:
    /* 1E68  mov     ax,0E088h */
    AX = 0xE088;
L1E6B:
    /* 1E6B  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1E6C:
    /* 1E6C  mov     ax,0E820h */
    AX = 0xE820;
L1E6F:
    /* 1E6F  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1E70:
    /* 1E70  jmp     short L1E7D */
    goto L1E7D;
L1E72: /* L1E72 */
    /* 1E72  mov     al,0ACh */
    AL = 0xAC;
L1E74:
    /* 1E74  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1E75:
    /* 1E75  mov     ax,0C488h */
    AX = 0xC488;
L1E78:
    /* 1E78  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1E79:
    /* 1E79  mov     ax,0E8D2h */
    AX = 0xE8D2;
L1E7C:
    /* 1E7C  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1E7D: /* L1E7D */
    /* 1E7D  mov     al,36h */
    AL = 0x36;
L1E7F:
    /* 1E7F  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1E80:
    /* 1E80  mov     al,0D7h */
    AL = 0xD7;
L1E82:
    /* 1E82  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1E83: /* L1E83 */
    /* 1E83  mov     al,0AAh */
    AL = 0xAA;
L1E85:
    /* 1E85  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1E86:
    /* 1E86  add     dx,word ptr ds:[0B3Ah] */
    DX = add16(DX, rw(pDS, 0xB3A), 0);
L1E8A:
    /* 1E8A  loop    L1E58 */
    if (--CX) goto L1E58;
L1E8C: /* L1E8C */
    /* 1E8C  mov     al,0C3h */
    AL = 0xC3;
L1E8E:
    /* 1E8E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1E8F:
    /* 1E8F  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L1E90:
    /* 1E90  mov     bp,word ptr ds:[0B28h] */
    BP = rw(pDS, 0xB28);
L1E94:
    /* 1E94  mov     ax,word ptr ds:[0B1Eh] */
    AX = rw(pDS, 0xB1E);
L1E97:
    /* 1E97  mov     byte ptr ds:[0DC2h],al */
    wb(pDS, 0xDC2, AL);
L1E9A:
    /* 1E9A  mov     ax,word ptr ds:[0B3Ah] */
    AX = rw(pDS, 0xB3A);
L1E9D:
    /* 1E9D  test    word ptr ds:[0B40h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0xB40) & 0xFFFF));
L1EA3:
    /* 1EA3  je      L1EB5 */
    if (ZF) goto L1EB5;
L1EA5:
    /* 1EA5  mov     bx,word ptr ds:[0B40h] */
    BX = rw(pDS, 0xB40);
L1EA9:
    /* 1EA9  imul    bx */
    imul16(BX);
L1EAB:
    /* 1EAB  mov     al,ah */
    AL = AH;
L1EAD:
    /* 1EAD  mov     ah,dl */
    AH = DL;
L1EAF:
    /* 1EAF  mov     word ptr ds:[0B40h],ax */
    ww(pDS, 0xB40, AX);
L1EB2:
    /* 1EB2  mov     ax,word ptr ds:[0B3Ah] */
    AX = rw(pDS, 0xB3A);
L1EB5: /* L1EB5 */
    /* 1EB5  mov     bx,ax */
    BX = AX;
L1EB7:
    /* 1EB7  test    ah,0FFh */
    logic8((uint8_t)(AH & 0xFF));
L1EBA:
    /* 1EBA  jne     L1F10 */
    if (!ZF) goto L1F10;
L1EBC:
    /* 1EBC  cmp     ax,128h */
    sub16(AX, 0x128, 0);
L1EBF:
    /* 1EBF  jle     L1EF7 */
    if (ZF || SF != OF) goto L1EF7;
L1EC1:
    /* 1EC1  xor     bh,bh */
    BH = (uint8_t)(BH ^ BH);
L1EC3:
    /* 1EC3  mov     al,51h */
    AL = 0x51;
L1EC5:
    /* 1EC5  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1EC6:
    /* 1EC6  xor     si,si */
    SI = (uint16_t)(SI ^ SI);
L1EC8: /* L1EC8 */
    /* 1EC8  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L1ECA: /* L1ECA */
    /* 1ECA  add     bh,bl */
    BH = add8(BH, BL, 0);
L1ECC:
    /* 1ECC  inc     dx */
    DX = inc16(DX);
L1ECD:
    /* 1ECD  jae     L1ECA */
    if (!CF) goto L1ECA;
L1ECF:
    /* 1ECF  cmp     dx,1 */
    sub16(DX, 0x1, 0);
L1ED2:
    /* 1ED2  jne     L1ED7 */
    if (!ZF) goto L1ED7;
L1ED4:
    /* 1ED4  inc     si */
    SI = (uint16_t)(SI + 1);
L1ED5:
    /* 1ED5  jmp     short L1EE8 */
    goto L1EE8;
L1ED7: /* L1ED7 */
    /* 1ED7  mov     al,0A4h */
    AL = 0xA4;
L1ED9:
    /* 1ED9  mov     cx,si */
    CX = SI;
L1EDB:
    /* 1EDB  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L1EDD:
    /* 1EDD  xor     si,si */
    SI = (uint16_t)(SI ^ SI);
L1EDF:
    /* 1EDF  mov     al,0ACh */
    AL = 0xAC;
L1EE1:
    /* 1EE1  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1EE2:
    /* 1EE2  mov     al,0AAh */
    AL = 0xAA;
L1EE4:
    /* 1EE4  mov     cx,dx */
    CX = DX;
L1EE6:
    /* 1EE6  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L1EE8: /* L1EE8 */
    /* 1EE8  sub     bp,dx */
    BP = sub16(BP, DX, 0);
L1EEA:
    /* 1EEA  jg      L1EC8 */
    if (!ZF && SF == OF) goto L1EC8;
L1EEC:
    /* 1EEC  mov     al,0A4h */
    AL = 0xA4;
L1EEE:
    /* 1EEE  mov     cx,si */
    CX = SI;
L1EF0:
    /* 1EF0  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L1EF2:
    /* 1EF2  mov     al,59h */
    AL = 0x59;
L1EF4:
    /* 1EF4  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1EF5:
    /* 1EF5  jmp     L1E8C */
    goto L1E8C;
L1EF7: /* L1EF7 */
    /* 1EF7  xor     bh,bh */
    BH = (uint8_t)(BH ^ BH);
L1EF9: /* L1EF9 */
    /* 1EF9  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L1EFB: /* L1EFB */
    /* 1EFB  inc     dx */
    DX = (uint16_t)(DX + 1);
L1EFC:
    /* 1EFC  add     bh,bl */
    BH = add8(BH, BL, 0);
L1EFE:
    /* 1EFE  jae     L1EFB */
    if (!CF) goto L1EFB;
L1F00:
    /* 1F00  mov     al,0ACh */
    AL = 0xAC;
L1F02:
    /* 1F02  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1F03:
    /* 1F03  mov     al,0AAh */
    AL = 0xAA;
L1F05:
    /* 1F05  mov     cx,dx */
    CX = DX;
L1F07:
    /* 1F07  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L1F09:
    /* 1F09  sub     bp,dx */
    BP = sub16(BP, DX, 0);
L1F0B:
    /* 1F0B  jg      L1EF9 */
    if (!ZF && SF == OF) goto L1EF9;
L1F0D:
    /* 1F0D  jmp     L1E8C */
    goto L1E8C;
L1F10: /* L1F10 */
    /* 1F10  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L1F12: /* L1F12 */
    /* 1F12  mov     al,0A4h */
    AL = 0xA4;
L1F14:
    /* 1F14  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1F15:
    /* 1F15  add     dx,bx */
    DX = add16(DX, BX, 0);
L1F17:
    /* 1F17  dec     dh */
    DH = dec8(DH);
L1F19:
    /* 1F19  je      L1F30 */
    if (ZF) goto L1F30;
L1F1B:
    /* 1F1B  cmp     dh,1 */
    sub8(DH, 0x1, 0);
L1F1E:
    /* 1F1E  jg      L1F27 */
    if (!ZF && SF == OF) goto L1F27;
L1F20:
    /* 1F20  mov     al,46h */
    AL = 0x46;
L1F22:
    /* 1F22  dec     dh */
    DH = dec8(DH);
L1F24:
    /* 1F24  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1F25:
    /* 1F25  jmp     short L1F30 */
    goto L1F30;
L1F27: /* L1F27 */
    /* 1F27  mov     ax,0C683h */
    AX = 0xC683;
L1F2A:
    /* 1F2A  stosw */
    ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2));
L1F2B:
    /* 1F2B  mov     al,dh */
    AL = DH;
L1F2D:
    /* 1F2D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L1F2E:
    /* 1F2E  xor     dh,dh */
    DH = logic8((uint8_t)(DH ^ DH));
L1F30: /* L1F30 */
    /* 1F30  dec     bp */
    BP = dec16(BP);
L1F31:
    /* 1F31  jne     L1F12 */
    if (!ZF) goto L1F12;
L1F33:
    /* 1F33  jmp     L1E8C */
    goto L1E8C;
}
