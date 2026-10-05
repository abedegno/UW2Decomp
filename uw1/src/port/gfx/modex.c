/* modex.c: replaces src/gfx/MODEX.ASM (seg015_1F9B, 000E..03C2 of its
   segment). Written by tools/asm2c.py from the assembly and the bytes it assembles to; do not
   edit by hand: change the tool, or its OVERRIDES, and run it again. Each instruction is the
   C below its source line; the comments before a routine are the .ASM file's. */
#include "x86/asmrt.h"

extern uint16_t *dseg_5c99_2404;      /* gfx/grcore.c */
extern unsigned char *palette;        /* gfx/grcore.c */
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

uint32_t asm_mod_MODEX(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x000E: goto L000E;
    case 0x000F: goto L000F;
    case 0x0011: goto L0011;
    case 0x0012: goto L0012;
    case 0x0013: goto L0013;
    case 0x0014: goto L0014;
    case 0x0017: goto L0017;
    case 0x0019: goto L0019;
    case 0x001A: goto L001A;
    case 0x001D: goto L001D;
    case 0x0020: goto L0020;
    case 0x0023: goto L0023;
    case 0x0025: goto L0025;
    case 0x0029: goto L0029;
    case 0x002C: goto L002C;
    case 0x002E: goto L002E;
    case 0x0031: goto L0031;
    case 0x0034: goto L0034;
    case 0x0036: goto L0036;
    case 0x0039: goto L0039;
    case 0x003C: goto L003C;
    case 0x003E: goto L003E;
    case 0x0041: goto L0041;
    case 0x0043: goto L0043;
    case 0x0046: goto L0046;
    case 0x0048: goto L0048;
    case 0x004A: goto L004A;
    case 0x004D: goto L004D;
    case 0x0050: goto L0050;
    case 0x0052: goto L0052;
    case 0x0053: goto L0053;
    case 0x0055: goto L0055;
    case 0x0057: goto L0057;
    case 0x005A: goto L005A;
    case 0x005C: goto L005C;
    case 0x005F: goto L005F;
    case 0x0062: goto L0062;
    case 0x0063: goto L0063;
    case 0x0064: goto L0064;
    case 0x0067: goto L0067;
    case 0x0068: goto L0068;
    case 0x0069: goto L0069;
    case 0x006B: goto L006B;
    case 0x006D: goto L006D;
    case 0x0070: goto L0070;
    case 0x0073: goto L0073;
    case 0x0074: goto L0074;
    case 0x0077: goto L0077;
    case 0x0079: goto L0079;
    case 0x007B: goto L007B;
    case 0x007D: goto L007D;
    case 0x007F: goto L007F;
    case 0x0081: goto L0081;
    case 0x0083: goto L0083;
    case 0x0085: goto L0085;
    case 0x0087: goto L0087;
    case 0x0088: goto L0088;
    case 0x008A: goto L008A;
    case 0x008D: goto L008D;
    case 0x008F: goto L008F;
    case 0x0090: goto L0090;
    case 0x0093: goto L0093;
    case 0x0094: goto L0094;
    case 0x0097: goto L0097;
    case 0x0098: goto L0098;
    case 0x0099: goto L0099;
    case 0x009C: goto L009C;
    case 0x009E: goto L009E;
    case 0x00A0: goto L00A0;
    case 0x00A1: goto L00A1;
    case 0x00A2: goto L00A2;
    case 0x00A4: goto L00A4;
    case 0x00A6: goto L00A6;
    case 0x00A9: goto L00A9;
    case 0x00AC: goto L00AC;
    case 0x00AD: goto L00AD;
    case 0x00B0: goto L00B0;
    case 0x00B2: goto L00B2;
    case 0x00B4: goto L00B4;
    case 0x00B6: goto L00B6;
    case 0x00B8: goto L00B8;
    case 0x00BA: goto L00BA;
    case 0x00BB: goto L00BB;
    case 0x00BD: goto L00BD;
    case 0x00BF: goto L00BF;
    case 0x00C1: goto L00C1;
    case 0x00C2: goto L00C2;
    case 0x00C4: goto L00C4;
    case 0x00C6: goto L00C6;
    case 0x00C7: goto L00C7;
    case 0x00CA: goto L00CA;
    case 0x00CC: goto L00CC;
    case 0x00CF: goto L00CF;
    case 0x00D1: goto L00D1;
    case 0x00D2: goto L00D2;
    case 0x00D5: goto L00D5;
    case 0x00D6: goto L00D6;
    case 0x00D7: goto L00D7;
    case 0x00D9: goto L00D9;
    case 0x00DC: goto L00DC;
    case 0x00DF: goto L00DF;
    case 0x00E0: goto L00E0;
    case 0x00E1: goto L00E1;
    case 0x00E2: goto L00E2;
    case 0x00E3: goto L00E3;
    case 0x00E4: goto L00E4;
    case 0x00E6: goto L00E6;
    case 0x00E7: goto L00E7;
    case 0x00E8: goto L00E8;
    case 0x00E9: goto L00E9;
    case 0x00EB: goto L00EB;
    case 0x00EC: goto L00EC;
    case 0x00ED: goto L00ED;
    case 0x00F0: goto L00F0;
    case 0x00F2: goto L00F2;
    case 0x00F4: goto L00F4;
    case 0x00F5: goto L00F5;
    case 0x00F6: goto L00F6;
    case 0x00F7: goto L00F7;
    case 0x00F8: goto L00F8;
    case 0x00F9: goto L00F9;
    case 0x00FB: goto L00FB;
    case 0x00FC: goto L00FC;
    case 0x00FF: goto L00FF;
    case 0x0102: goto L0102;
    case 0x0105: goto L0105;
    case 0x0107: goto L0107;
    case 0x0109: goto L0109;
    case 0x010B: goto L010B;
    case 0x010C: goto L010C;
    case 0x010D: goto L010D;
    case 0x010E: goto L010E;
    case 0x010F: goto L010F;
    case 0x0111: goto L0111;
    case 0x0112: goto L0112;
    case 0x0115: goto L0115;
    case 0x0118: goto L0118;
    case 0x011A: goto L011A;
    case 0x011C: goto L011C;
    case 0x011E: goto L011E;
    case 0x011F: goto L011F;
    case 0x0121: goto L0121;
    case 0x0122: goto L0122;
    case 0x0123: goto L0123;
    case 0x0124: goto L0124;
    case 0x0125: goto L0125;
    case 0x0127: goto L0127;
    case 0x0128: goto L0128;
    case 0x0129: goto L0129;
    case 0x012A: goto L012A;
    case 0x012B: goto L012B;
    case 0x012D: goto L012D;
    case 0x0130: goto L0130;
    case 0x0133: goto L0133;
    case 0x0135: goto L0135;
    case 0x0137: goto L0137;
    case 0x0139: goto L0139;
    case 0x013C: goto L013C;
    case 0x013E: goto L013E;
    case 0x0140: goto L0140;
    case 0x0142: goto L0142;
    case 0x0144: goto L0144;
    case 0x0145: goto L0145;
    case 0x0147: goto L0147;
    case 0x0148: goto L0148;
    case 0x0149: goto L0149;
    case 0x014A: goto L014A;
    case 0x014B: goto L014B;
    case 0x014D: goto L014D;
    case 0x0150: goto L0150;
    case 0x0153: goto L0153;
    case 0x0155: goto L0155;
    case 0x0157: goto L0157;
    case 0x0159: goto L0159;
    case 0x015B: goto L015B;
    case 0x015D: goto L015D;
    case 0x015F: goto L015F;
    case 0x0161: goto L0161;
    case 0x0164: goto L0164;
    case 0x0166: goto L0166;
    case 0x0168: goto L0168;
    case 0x016A: goto L016A;
    case 0x016E: goto L016E;
    case 0x0170: goto L0170;
    case 0x0171: goto L0171;
    case 0x0172: goto L0172;
    case 0x0173: goto L0173;
    case 0x0174: goto L0174;
    case 0x0175: goto L0175;
    case 0x0177: goto L0177;
    case 0x0178: goto L0178;
    case 0x0179: goto L0179;
    case 0x017A: goto L017A;
    case 0x017C: goto L017C;
    case 0x017D: goto L017D;
    case 0x0180: goto L0180;
    case 0x0183: goto L0183;
    case 0x0185: goto L0185;
    case 0x0187: goto L0187;
    case 0x0189: goto L0189;
    case 0x018C: goto L018C;
    case 0x018F: goto L018F;
    case 0x0191: goto L0191;
    case 0x0193: goto L0193;
    case 0x0195: goto L0195;
    case 0x0197: goto L0197;
    case 0x0198: goto L0198;
    case 0x019A: goto L019A;
    case 0x019C: goto L019C;
    case 0x019E: goto L019E;
    case 0x019F: goto L019F;
    case 0x01A0: goto L01A0;
    case 0x01A1: goto L01A1;
    case 0x01A2: goto L01A2;
    case 0x01A4: goto L01A4;
    case 0x01A5: goto L01A5;
    case 0x01A6: goto L01A6;
    case 0x01A7: goto L01A7;
    case 0x01AA: goto L01AA;
    case 0x01AC: goto L01AC;
    case 0x01AE: goto L01AE;
    case 0x01B0: goto L01B0;
    case 0x01B2: goto L01B2;
    case 0x01B5: goto L01B5;
    case 0x01B7: goto L01B7;
    case 0x01B9: goto L01B9;
    case 0x01BA: goto L01BA;
    case 0x01BC: goto L01BC;
    case 0x01BF: goto L01BF;
    case 0x01C2: goto L01C2;
    case 0x01C4: goto L01C4;
    case 0x01C6: goto L01C6;
    case 0x01C8: goto L01C8;
    case 0x01C9: goto L01C9;
    case 0x01CB: goto L01CB;
    case 0x01CC: goto L01CC;
    case 0x01CE: goto L01CE;
    case 0x01D0: goto L01D0;
    case 0x01D1: goto L01D1;
    case 0x01D3: goto L01D3;
    case 0x01D5: goto L01D5;
    case 0x01D7: goto L01D7;
    case 0x01D9: goto L01D9;
    case 0x01DA: goto L01DA;
    case 0x01DC: goto L01DC;
    case 0x01DD: goto L01DD;
    case 0x01DF: goto L01DF;
    case 0x01E1: goto L01E1;
    case 0x01E3: goto L01E3;
    case 0x01E5: goto L01E5;
    case 0x01E7: goto L01E7;
    case 0x01E9: goto L01E9;
    case 0x01EB: goto L01EB;
    case 0x01ED: goto L01ED;
    case 0x01EF: goto L01EF;
    case 0x01F1: goto L01F1;
    case 0x01F3: goto L01F3;
    case 0x01F5: goto L01F5;
    case 0x01F7: goto L01F7;
    case 0x01F8: goto L01F8;
    case 0x01F9: goto L01F9;
    case 0x01FA: goto L01FA;
    case 0x01FB: goto L01FB;
    case 0x01FC: goto L01FC;
    case 0x01FD: goto L01FD;
    case 0x01FF: goto L01FF;
    case 0x0200: goto L0200;
    case 0x0201: goto L0201;
    case 0x0202: goto L0202;
    case 0x0204: goto L0204;
    case 0x0207: goto L0207;
    case 0x020A: goto L020A;
    case 0x020C: goto L020C;
    case 0x020E: goto L020E;
    case 0x0210: goto L0210;
    case 0x0213: goto L0213;
    case 0x0215: goto L0215;
    case 0x0219: goto L0219;
    case 0x021B: goto L021B;
    case 0x021E: goto L021E;
    case 0x0220: goto L0220;
    case 0x0221: goto L0221;
    case 0x0222: goto L0222;
    case 0x0223: goto L0223;
    case 0x0224: goto L0224;
    case 0x0225: goto L0225;
    case 0x0226: goto L0226;
    case 0x0228: goto L0228;
    case 0x0229: goto L0229;
    case 0x022A: goto L022A;
    case 0x022D: goto L022D;
    case 0x022E: goto L022E;
    case 0x0230: goto L0230;
    case 0x0232: goto L0232;
    case 0x0235: goto L0235;
    case 0x0237: goto L0237;
    case 0x0239: goto L0239;
    case 0x023C: goto L023C;
    case 0x023D: goto L023D;
    case 0x023E: goto L023E;
    case 0x0241: goto L0241;
    case 0x0242: goto L0242;
    case 0x0246: goto L0246;
    case 0x0248: goto L0248;
    case 0x024A: goto L024A;
    case 0x024C: goto L024C;
    case 0x024E: goto L024E;
    case 0x0250: goto L0250;
    case 0x0252: goto L0252;
    case 0x0253: goto L0253;
    case 0x0255: goto L0255;
    case 0x0256: goto L0256;
    case 0x0257: goto L0257;
    case 0x0258: goto L0258;
    case 0x0259: goto L0259;
    case 0x025A: goto L025A;
    case 0x025B: goto L025B;
    case 0x025D: goto L025D;
    case 0x0260: goto L0260;
    case 0x0261: goto L0261;
    case 0x0262: goto L0262;
    case 0x0266: goto L0266;
    case 0x0269: goto L0269;
    case 0x026B: goto L026B;
    case 0x026E: goto L026E;
    case 0x0270: goto L0270;
    case 0x0273: goto L0273;
    case 0x0276: goto L0276;
    case 0x0279: goto L0279;
    case 0x027B: goto L027B;
    case 0x027E: goto L027E;
    case 0x0280: goto L0280;
    case 0x0282: goto L0282;
    case 0x0284: goto L0284;
    case 0x0286: goto L0286;
    case 0x0289: goto L0289;
    case 0x028B: goto L028B;
    case 0x028C: goto L028C;
    case 0x028F: goto L028F;
    case 0x0292: goto L0292;
    case 0x0295: goto L0295;
    case 0x0298: goto L0298;
    case 0x029A: goto L029A;
    case 0x029B: goto L029B;
    case 0x029E: goto L029E;
    case 0x02A1: goto L02A1;
    case 0x02A2: goto L02A2;
    case 0x02A3: goto L02A3;
    case 0x02A5: goto L02A5;
    case 0x02A6: goto L02A6;
    case 0x02A7: goto L02A7;
    case 0x02A8: goto L02A8;
    case 0x02AA: goto L02AA;
    case 0x02AD: goto L02AD;
    case 0x02AE: goto L02AE;
    case 0x02AF: goto L02AF;
    case 0x02B0: goto L02B0;
    case 0x02B3: goto L02B3;
    case 0x02B5: goto L02B5;
    case 0x02B6: goto L02B6;
    case 0x02B9: goto L02B9;
    case 0x02BB: goto L02BB;
    case 0x02BE: goto L02BE;
    case 0x02C1: goto L02C1;
    case 0x02C4: goto L02C4;
    case 0x02C5: goto L02C5;
    case 0x02C9: goto L02C9;
    case 0x02CB: goto L02CB;
    case 0x02CC: goto L02CC;
    case 0x02CF: goto L02CF;
    case 0x02D2: goto L02D2;
    case 0x02D4: goto L02D4;
    case 0x02D5: goto L02D5;
    case 0x02D8: goto L02D8;
    case 0x02DB: goto L02DB;
    case 0x02DE: goto L02DE;
    case 0x02E0: goto L02E0;
    case 0x02E3: goto L02E3;
    case 0x02E6: goto L02E6;
    case 0x02E8: goto L02E8;
    case 0x02EB: goto L02EB;
    case 0x02EC: goto L02EC;
    case 0x02EF: goto L02EF;
    case 0x02F2: goto L02F2;
    case 0x02F5: goto L02F5;
    case 0x02F7: goto L02F7;
    case 0x02FA: goto L02FA;
    case 0x02FD: goto L02FD;
    case 0x0300: goto L0300;
    case 0x0301: goto L0301;
    case 0x0304: goto L0304;
    case 0x0306: goto L0306;
    case 0x0309: goto L0309;
    case 0x030C: goto L030C;
    case 0x030E: goto L030E;
    case 0x0311: goto L0311;
    case 0x0314: goto L0314;
    case 0x0316: goto L0316;
    case 0x0318: goto L0318;
    case 0x031A: goto L031A;
    case 0x031D: goto L031D;
    case 0x031E: goto L031E;
    case 0x031F: goto L031F;
    case 0x0320: goto L0320;
    case 0x0321: goto L0321;
    case 0x0323: goto L0323;
    case 0x0324: goto L0324;
    case 0x0325: goto L0325;
    case 0x0326: goto L0326;
    case 0x0328: goto L0328;
    case 0x0329: goto L0329;
    case 0x032A: goto L032A;
    case 0x032D: goto L032D;
    case 0x032F: goto L032F;
    case 0x0330: goto L0330;
    case 0x0333: goto L0333;
    case 0x0335: goto L0335;
    case 0x0338: goto L0338;
    case 0x033B: goto L033B;
    case 0x033D: goto L033D;
    case 0x033E: goto L033E;
    case 0x0341: goto L0341;
    case 0x0344: goto L0344;
    case 0x0346: goto L0346;
    case 0x034A: goto L034A;
    case 0x034C: goto L034C;
    case 0x034D: goto L034D;
    case 0x0350: goto L0350;
    case 0x0353: goto L0353;
    case 0x0355: goto L0355;
    case 0x0359: goto L0359;
    case 0x035C: goto L035C;
    case 0x035F: goto L035F;
    case 0x0360: goto L0360;
    case 0x0361: goto L0361;
    case 0x0362: goto L0362;
    case 0x0364: goto L0364;
    case 0x0365: goto L0365;
    case 0x0366: goto L0366;
    case 0x0367: goto L0367;
    case 0x0369: goto L0369;
    case 0x036A: goto L036A;
    case 0x036B: goto L036B;
    case 0x036C: goto L036C;
    case 0x036F: goto L036F;
    case 0x0371: goto L0371;
    case 0x0372: goto L0372;
    case 0x0375: goto L0375;
    case 0x0378: goto L0378;
    case 0x037D: goto L037D;
    case 0x037F: goto L037F;
    case 0x0381: goto L0381;
    case 0x0383: goto L0383;
    case 0x0386: goto L0386;
    case 0x0389: goto L0389;
    case 0x038C: goto L038C;
    case 0x038F: goto L038F;
    case 0x0391: goto L0391;
    case 0x0394: goto L0394;
    case 0x0397: goto L0397;
    case 0x0399: goto L0399;
    case 0x039B: goto L039B;
    case 0x039C: goto L039C;
    case 0x039F: goto L039F;
    case 0x03A1: goto L03A1;
    case 0x03A4: goto L03A4;
    case 0x03A6: goto L03A6;
    case 0x03A8: goto L03A8;
    case 0x03AA: goto L03AA;
    case 0x03AB: goto L03AB;
    case 0x03AE: goto L03AE;
    case 0x03AF: goto L03AF;
    case 0x03B0: goto L03B0;
    case 0x03B2: goto L03B2;
    case 0x03B4: goto L03B4;
    case 0x03B5: goto L03B5;
    case 0x03B6: goto L03B6;
    case 0x03B7: goto L03B7;
    case 0x03B9: goto L03B9;
    case 0x03BA: goto L03BA;
    case 0x03BB: goto L03BB;
    case 0x03BC: goto L03BC;
    case 0x03BE: goto L03BE;
    case 0x03C0: goto L03C0;
    case 0x03C1: goto L03C1;
    default: asm_bad_entry("MODEX.ASM", entry);
    }

    /* void far grab(void far *dst, int x, int y, int w, int h)
       Copies a w by h block of the planar screen, top row at screen line 199 - y, into dst.
       dst is filled as a linear w-wide image: the routine reads one plane at a time (read map
       select), storing every fourth byte; each of its two passes walks down the rows for one
       plane and back up (std) for the next. */
L000E: /* _grab */
    /* 000E  push    bp */
    push16(BP);
L000F:
    /* 000F  mov     bp,sp */
    BP = SP;
L0011:
    /* 0011  push    ds */
    push16(asm_ds);
L0012:
    /* 0012  push    si */
    push16(SI);
L0013:
    /* 0013  push    di */
    push16(DI);
L0014:
    /* 0014  mov     dx,GC_INDEX */
    DX = 0x3CE;
L0017:
    /* 0017  mov     al,4 */
    AL = 0x4;
L0019:
    /* 0019  out     dx,al */
    asm_out8(DX, AL);
L001A:
    /* 001A  mov     ax,0C7h */
    AX = 0xC7;
L001D:
    /* 001D  sub     ax,[bp+0Ch] */
    AX = (uint16_t)(AX - rw(pSS, BP + 0xC));
L0020:
    /* 0020  mov     bx,50h */
    BX = 0x50;
L0023:
    /* 0023  mul     bx */
    mul16(BX);
L0025:
    /* 0025  les     di,_dseg_5c99_2404 */
    /* by hand: les di,_dseg_5c99_2404: GRCORE's far pointer to the screen's page offset (seg048:3838), a C object in the port's DGROUP (gfx/grcore.c) */
    { unsigned s_, o_; port_fp_split_recent(dseg_5c99_2404, &s_, &o_); DI = (uint16_t)o_; SET_ES(s_); }
L0029:
    /* 0029  add     ax,es:[di] */
    AX = (uint16_t)(AX + rw(pES, DI));
L002C:
    /* 002C  mov     si,ax */
    SI = AX;
L002E:
    /* 002E  mov     ax,[bp+0Eh] */
    AX = rw(pSS, BP + 0xE);
L0031:
    /* 0031  mov     dx,[bp+10h] */
    DX = rw(pSS, BP + 0x10);
L0034:
    /* 0034  mov     dh,al */
    DH = AL;
L0036:
    /* 0036  add     ax,3 */
    AX = (uint16_t)(AX + 0x3);
L0039:
    /* 0039  shr     ax,2 */
    AX = (uint16_t)(AX >> 2);
L003C:
    /* 003C  sub     bx,ax */
    BX = (uint16_t)(BX - AX);
L003E:
    /* 003E  mov     ax,[bp+0Ah] */
    AX = rw(pSS, BP + 0xA);
L0041:
    /* 0041  mov     cx,ax */
    CX = AX;
L0043:
    /* 0043  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0046:
    /* 0046  add     si,cx */
    SI = (uint16_t)(SI + CX);
L0048:
    /* 0048  and     al,3 */
    AL = (uint8_t)(AL & 0x3);
L004A:
    /* 004A  les     di,[bp+6] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0x6)), s_ = rw(pSS, (uint16_t)(BP + 0x6 + 2));
    DI = o_;
    SET_ES(s_); }
L004D:
    /* 004D  mov     cx,0A000h */
    CX = 0xA000;
L0050:
    /* 0050  mov     ds,cx */
    SET_DS(CX);
L0052:
    /* 0052  push    bp */
    push16(BP);
L0053:
    /* 0053  mov     bp,bx */
    BP = BX;
L0055:
    /* 0055  mov     bl,dh */
    BL = DH;
L0057:
    /* 0057  and     bx,3 */
    BX = logic16((uint16_t)(BX & 0x3));
L005A:
    /* 005A  jz      grab_planes */
    if (ZF) goto L005F;
L005C:
    /* 005C  sub     bx,4 */
    BX = (uint16_t)(BX - 0x4);
L005F: /* grab_planes */
    /* 005F  mov     cx,2 */
    CX = 0x2;
L0062: /* grab_pass */
    /* 0062  push    cx */
    push16(CX);
L0063:
    /* 0063  push    dx */
    push16(DX);
L0064:
    /* 0064  mov     dx,GC_DATA */
    DX = 0x3CF;
L0067:
    /* 0067  out     dx,al */
    asm_out8(DX, AL);
L0068:
    /* 0068  pop     dx */
    DX = pop16();
L0069:
    /* 0069  mov     ah,dl */
    AH = DL;
L006B: /* grab_down */
    /* 006B  mov     cl,dh */
    CL = DH;
L006D:
    /* 006D  add     cl,3 */
    CL = (uint8_t)(CL + 0x3);
L0070:
    /* 0070  shr     cl,2 */
    CL = (uint8_t)(CL >> 2);
L0073: /* grab_down_row */
    /* 0073  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0074:
    /* 0074  add     di,3 */
    DI = (uint16_t)(DI + 0x3);
L0077:
    /* 0077  loop    grab_down_row */
    if (--CX) goto L0073;
L0079:
    /* 0079  add     si,bp */
    SI = (uint16_t)(SI + BP);
L007B:
    /* 007B  add     di,bx */
    DI = (uint16_t)(DI + BX);
L007D:
    /* 007D  dec     ah */
    AH = dec8(AH);
L007F:
    /* 007F  jnz     grab_down */
    if (!ZF) goto L006B;
L0081:
    /* 0081  inc     al */
    AL = (uint8_t)(AL + 1);
L0083:
    /* 0083  and     al,3 */
    AL = logic8((uint8_t)(AL & 0x3));
L0085:
    /* 0085  jnz     grab_down_next */
    if (!ZF) goto L0088;
L0087:
    /* 0087  inc     si */
    SI = (uint16_t)(SI + 1);
L0088: /* grab_down_next */
    /* 0088  dec     dh */
    DH = (uint8_t)(DH - 1);
L008A:
    /* 008A  test    dh,3 */
    logic8((uint8_t)(DH & 0x3));
L008D:
    /* 008D  jnz     grab_down_plane */
    if (!ZF) goto L0093;
L008F:
    /* 008F  inc     bp */
    BP = (uint16_t)(BP + 1);
L0090:
    /* 0090  add     bx,4 */
    BX = (uint16_t)(BX + 0x4);
L0093: /* grab_down_plane */
    /* 0093  push    dx */
    push16(DX);
L0094:
    /* 0094  mov     dx,GC_DATA */
    DX = 0x3CF;
L0097:
    /* 0097  out     dx,al */
    asm_out8(DX, AL);
L0098:
    /* 0098  pop     dx */
    DX = pop16();
L0099:
    /* 0099  sub     di,3 */
    DI = (uint16_t)(DI - 0x3);
L009C:
    /* 009C  sub     di,bx */
    DI = (uint16_t)(DI - BX);
L009E:
    /* 009E  sub     si,bp */
    SI = (uint16_t)(SI - BP);
L00A0:
    /* 00A0  dec     si */
    SI = (uint16_t)(SI - 1);
L00A1:
    /* 00A1  std */
    DF = 1;
L00A2:
    /* 00A2  mov     ah,dl */
    AH = DL;
L00A4: /* grab_up */
    /* 00A4  mov     cl,dh */
    CL = DH;
L00A6:
    /* 00A6  add     cl,3 */
    CL = (uint8_t)(CL + 0x3);
L00A9:
    /* 00A9  shr     cl,2 */
    CL = (uint8_t)(CL >> 2);
L00AC: /* grab_up_row */
    /* 00AC  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L00AD:
    /* 00AD  sub     di,3 */
    DI = (uint16_t)(DI - 0x3);
L00B0:
    /* 00B0  loop    grab_up_row */
    if (--CX) goto L00AC;
L00B2:
    /* 00B2  sub     si,bp */
    SI = (uint16_t)(SI - BP);
L00B4:
    /* 00B4  sub     di,bx */
    DI = (uint16_t)(DI - BX);
L00B6:
    /* 00B6  dec     ah */
    AH = dec8(AH);
L00B8:
    /* 00B8  jnz     grab_up */
    if (!ZF) goto L00A4;
L00BA:
    /* 00BA  cld */
    DF = 0;
L00BB:
    /* 00BB  inc     al */
    AL = (uint8_t)(AL + 1);
L00BD:
    /* 00BD  and     al,3 */
    AL = logic8((uint8_t)(AL & 0x3));
L00BF:
    /* 00BF  jnz     grab_up_next */
    if (!ZF) goto L00C2;
L00C1:
    /* 00C1  inc     si */
    SI = (uint16_t)(SI + 1);
L00C2: /* grab_up_next */
    /* 00C2  dec     dh */
    DH = (uint8_t)(DH - 1);
L00C4:
    /* 00C4  add     si,bp */
    SI = (uint16_t)(SI + BP);
L00C6:
    /* 00C6  inc     si */
    SI = (uint16_t)(SI + 1);
L00C7:
    /* 00C7  add     di,5 */
    DI = (uint16_t)(DI + 0x5);
L00CA:
    /* 00CA  add     di,bx */
    DI = (uint16_t)(DI + BX);
L00CC:
    /* 00CC  test    dh,3 */
    logic8((uint8_t)(DH & 0x3));
L00CF:
    /* 00CF  jnz     grab_up_plane */
    if (!ZF) goto L00D5;
L00D1:
    /* 00D1  inc     bp */
    BP = (uint16_t)(BP + 1);
L00D2:
    /* 00D2  add     bx,4 */
    BX = add16(BX, 0x4, 0);
L00D5: /* grab_up_plane */
    /* 00D5  pop     cx */
    CX = pop16();
L00D6:
    /* 00D6  dec     cx */
    CX = dec16(CX);
L00D7:
    /* 00D7  jnz     grab_pass */
    if (!ZF) goto L0062;
L00D9:
    /* 00D9  mov     dx,GC_INDEX */
    DX = 0x3CE;
L00DC:
    /* 00DC  mov     ax,0FF08h */
    AX = 0xFF08;
L00DF:
    /* 00DF  out     dx,ax */
    asm_out16(DX, AX);
L00E0:
    /* 00E0  pop     bp */
    BP = pop16();
L00E1:
    /* 00E1  pop     di */
    DI = pop16();
L00E2:
    /* 00E2  pop     si */
    SI = pop16();
L00E3:
    /* 00E3  pop     ds */
    SET_DS(pop16());
L00E4:
    /* 00E4  mov     sp,bp */
    SP = BP;
L00E6:
    /* 00E6  pop     bp */
    BP = pop16();
L00E7:
    /* 00E7  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* void far seg015_1F9B_E8(char far *s)    DOS print, '$' terminated */
L00E8: /* _seg015_1F9B_E8 */
    /* 00E8  push    bp */
    push16(BP);
L00E9:
    /* 00E9  mov     bp,sp */
    BP = SP;
L00EB:
    /* 00EB  push    dx */
    push16(DX);
L00EC:
    /* 00EC  push    ds */
    push16(asm_ds);
L00ED:
    /* 00ED  lds     dx,[bp+6] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0x6)), s_ = rw(pSS, (uint16_t)(BP + 0x6 + 2));
    DX = o_;
    SET_DS(s_); }
L00F0:
    /* 00F0  mov     ah,DOS_PRINT_STRING */
    AH = 0x9;
L00F2:
    /* 00F2  int     21h */
    asm_int(0x21);
L00F4:
    /* 00F4  pop     ds */
    SET_DS(pop16());
L00F5:
    /* 00F5  pop     dx */
    DX = pop16();
L00F6:
    /* 00F6  pop     bp */
    BP = pop16();
L00F7:
    /* 00F7  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* void far mem_set(void far *dst, char value, unsigned count)
       UW1 stores count / 2 words and nothing more; UW2 fixed the odd byte. */
L00F8: /* _mem_set */
    /* 00F8  push    bp */
    push16(BP);
L00F9:
    /* 00F9  mov     bp,sp */
    BP = SP;
L00FB:
    /* 00FB  push    di */
    push16(DI);
L00FC:
    /* 00FC  les     di,[bp+6] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0x6)), s_ = rw(pSS, (uint16_t)(BP + 0x6 + 2));
    DI = o_;
    SET_ES(s_); }
L00FF:
    /* 00FF  mov     cx,[bp+0Ch] */
    CX = rw(pSS, BP + 0xC);
L0102:
    /* 0102  mov     ax,[bp+0Ah] */
    AX = rw(pSS, BP + 0xA);
L0105:
    /* 0105  mov     ah,al */
    AH = AL;
L0107:
    /* 0107  shr     cx,1 */
    CX = shr16(CX, 1);
L0109:
    /* 0109  rep     stosw */
    while (CX) { ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2)); CX--; }
L010B:
    /* 010B  pop     di */
    DI = pop16();
L010C:
    /* 010C  pop     bp */
    BP = pop16();
L010D:
    /* 010D  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* int far str_len(char far *s) */
L010E: /* _str_len */
    /* 010E  push    bp */
    push16(BP);
L010F:
    /* 010F  mov     bp,sp */
    BP = SP;
L0111:
    /* 0111  push    di */
    push16(DI);
L0112:
    /* 0112  les     di,[bp+6] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0x6)), s_ = rw(pSS, (uint16_t)(BP + 0x6 + 2));
    DI = o_;
    SET_ES(s_); }
L0115:
    /* 0115  mov     cx,0FFFFh */
    CX = 0xFFFF;
L0118:
    /* 0118  xor     al,al */
    AL = logic8((uint8_t)(AL ^ AL));
L011A:
    /* 011A  repne   scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (ZF) break; }
L011C:
    /* 011C  not     cx */
    CX = (uint16_t)~CX;
L011E:
    /* 011E  dec     cx */
    CX = dec16(CX);
L011F:
    /* 011F  mov     ax,cx */
    AX = CX;
L0121:
    /* 0121  pop     di */
    DI = pop16();
L0122:
    /* 0122  pop     bp */
    BP = pop16();
L0123:
    /* 0123  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* char far * far str_ncopy(char far *dst, char far *src, int n)
       Copies src with its terminator, but at most n bytes; shares str_copy's copy loop. */
L0124: /* _str_ncopy */
    /* 0124  push    bp */
    push16(BP);
L0125:
    /* 0125  mov     bp,sp */
    BP = SP;
L0127:
    /* 0127  push    ds */
    push16(asm_ds);
L0128:
    /* 0128  push    es */
    push16(asm_es);
L0129:
    /* 0129  push    si */
    push16(SI);
L012A:
    /* 012A  push    di */
    push16(DI);
L012B:
    /* 012B  xor     al,al */
    AL = logic8((uint8_t)(AL ^ AL));
L012D:
    /* 012D  mov     cx,0FFFFh */
    CX = 0xFFFF;
L0130:
    /* 0130  les     di,[bp+0Ah] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0xA)), s_ = rw(pSS, (uint16_t)(BP + 0xA + 2));
    DI = o_;
    SET_ES(s_); }
L0133:
    /* 0133  mov     si,di */
    SI = DI;
L0135:
    /* 0135  repne   scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (ZF) break; }
L0137:
    /* 0137  not     cx */
    CX = (uint16_t)~CX;
L0139:
    /* 0139  mov     dx,[bp+0Eh] */
    DX = rw(pSS, BP + 0xE);
L013C:
    /* 013C  cmp     cx,dx */
    sub16(CX, DX, 0);
L013E:
    /* 013E  jle     short str_copy_count */
    if (ZF || SF != OF) goto L0159;
L0140:
    /* 0140  mov     cx,dx */
    CX = DX;
L0142:
    /* 0142  jmp     short str_copy_count */
    goto L0159;

    /* char far * far str_copy(char far *dst, char far *src) */
L0144: /* _str_copy */
    /* 0144  push    bp */
    push16(BP);
L0145:
    /* 0145  mov     bp,sp */
    BP = SP;
L0147:
    /* 0147  push    ds */
    push16(asm_ds);
L0148:
    /* 0148  push    es */
    push16(asm_es);
L0149:
    /* 0149  push    si */
    push16(SI);
L014A:
    /* 014A  push    di */
    push16(DI);
L014B:
    /* 014B  xor     al,al */
    AL = logic8((uint8_t)(AL ^ AL));
L014D:
    /* 014D  mov     cx,0FFFFh */
    CX = 0xFFFF;
L0150:
    /* 0150  les     di,[bp+0Ah] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0xA)), s_ = rw(pSS, (uint16_t)(BP + 0xA + 2));
    DI = o_;
    SET_ES(s_); }
L0153:
    /* 0153  mov     si,di */
    SI = DI;
L0155:
    /* 0155  repne   scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (ZF) break; }
L0157:
    /* 0157  not     cx */
    CX = (uint16_t)~CX;
L0159: /* str_copy_count */
    /* 0159  mov     bx,cx */
    BX = CX;
L015B:
    /* 015B  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L015D:
    /* 015D  mov     ax,es */
    AX = asm_es;
L015F:
    /* 015F  mov     ds,ax */
    SET_DS(AX);
L0161:
    /* 0161  les     di,[bp+6] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0x6)), s_ = rw(pSS, (uint16_t)(BP + 0x6 + 2));
    DI = o_;
    SET_ES(s_); }
L0164:
    /* 0164  mov     dx,es */
    DX = asm_es;
L0166:
    /* 0166  mov     ax,di */
    AX = DI;
L0168:
    /* 0168  rep     movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L016A:
    /* 016A  test    bx,1 */
    logic16((uint16_t)(BX & 0x1));
L016E:
    /* 016E  jz      str_copy_done */
    if (ZF) goto L0171;
L0170:
    /* 0170  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0171: /* str_copy_done */
    /* 0171  pop     di */
    DI = pop16();
L0172:
    /* 0172  pop     si */
    SI = pop16();
L0173:
    /* 0173  pop     es */
    SET_ES(pop16());
L0174:
    /* 0174  pop     ds */
    SET_DS(pop16());
L0175:
    /* 0175  mov     sp,bp */
    SP = BP;
L0177:
    /* 0177  pop     bp */
    BP = pop16();
L0178:
    /* 0178  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* char far * far FindStringDelimiter(char far *s, int c)    strchr: the first c in s, or 0 */
L0179: /* _FindStringDelimiter */
    /* 0179  push    bp */
    push16(BP);
L017A:
    /* 017A  mov     bp,sp */
    BP = SP;
L017C:
    /* 017C  push    di */
    push16(DI);
L017D:
    /* 017D  les     di,[bp+6] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0x6)), s_ = rw(pSS, (uint16_t)(BP + 0x6 + 2));
    DI = o_;
    SET_ES(s_); }
L0180:
    /* 0180  mov     cx,0FFFFh */
    CX = 0xFFFF;
L0183:
    /* 0183  xor     ax,ax */
    AX = logic16((uint16_t)(AX ^ AX));
L0185:
    /* 0185  repne   scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (ZF) break; }
L0187:
    /* 0187  not     cx */
    CX = (uint16_t)~CX;
L0189:
    /* 0189  mov     ax,[bp+0Ah] */
    AX = rw(pSS, BP + 0xA);
L018C:
    /* 018C  les     di,[bp+6] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0x6)), s_ = rw(pSS, (uint16_t)(BP + 0x6 + 2));
    DI = o_;
    SET_ES(s_); }
L018F:
    /* 018F  repne   scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (ZF) break; }
L0191:
    /* 0191  jnz     short find_none */
    if (!ZF) goto L019A;
L0193:
    /* 0193  mov     dx,es */
    DX = asm_es;
L0195:
    /* 0195  mov     ax,di */
    AX = DI;
L0197:
    /* 0197  dec     ax */
    AX = dec16(AX);
L0198:
    /* 0198  jmp     short find_done */
    goto L019E;
L019A: /* find_none */
    /* 019A  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L019C:
    /* 019C  xor     ax,ax */
    AX = logic16((uint16_t)(AX ^ AX));
L019E: /* find_done */
    /* 019E  pop     di */
    DI = pop16();
L019F:
    /* 019F  pop     bp */
    BP = pop16();
L01A0:
    /* 01A0  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* char far * far str_str(char far *s, char far *find)    strstr: the first find in s, or 0 */
L01A1: /* _str_str */
    /* 01A1  push    bp */
    push16(BP);
L01A2:
    /* 01A2  mov     bp,sp */
    BP = SP;
L01A4:
    /* 01A4  push    ds */
    push16(asm_ds);
L01A5:
    /* 01A5  push    si */
    push16(SI);
L01A6:
    /* 01A6  push    di */
    push16(DI);
L01A7:
    /* 01A7  les     di,[bp+0Ah] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0xA)), s_ = rw(pSS, (uint16_t)(BP + 0xA + 2));
    DI = o_;
    SET_ES(s_); }
L01AA:
    /* 01AA  mov     ax,es */
    AX = asm_es;
L01AC:
    /* 01AC  mov     ds,ax */
    SET_DS(AX);
L01AE:
    /* 01AE  mov     si,di */
    SI = DI;
L01B0:
    /* 01B0  xor     al,al */
    AL = logic8((uint8_t)(AL ^ AL));
L01B2:
    /* 01B2  mov     cx,0FFFFh */
    CX = 0xFFFF;
L01B5:
    /* 01B5  repne   scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (ZF) break; }
L01B7:
    /* 01B7  not     cx */
    CX = (uint16_t)~CX;
L01B9:
    /* 01B9  dec     cx */
    CX = dec16(CX);
L01BA:
    /* 01BA  mov     dx,cx */
    DX = CX;
L01BC:
    /* 01BC  les     di,[bp+6] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0x6)), s_ = rw(pSS, (uint16_t)(BP + 0x6 + 2));
    DI = o_;
    SET_ES(s_); }
L01BF:
    /* 01BF  mov     cx,0FFFFh */
    CX = 0xFFFF;
L01C2:
    /* 01C2  repne   scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (ZF) break; }
L01C4:
    /* 01C4  not     cx */
    CX = (uint16_t)~CX;
L01C6:
    /* 01C6  sub     di,cx */
    DI = sub16(DI, CX, 0);
L01C8:
    /* 01C8  dec     cx */
    CX = dec16(CX);
L01C9:
    /* 01C9  mov     bx,si */
    BX = SI;
L01CB:
    /* 01CB  dec     di */
    DI = dec16(DI);
L01CC:
    /* 01CC  mov     bp,di */
    BP = DI;
L01CE: /* str_str_next */
    /* 01CE  mov     si,bx */
    SI = BX;
L01D0:
    /* 01D0  inc     bp */
    BP = inc16(BP);
L01D1:
    /* 01D1  mov     di,bp */
    DI = BP;
L01D3:
    /* 01D3  mov     al,[si] */
    AL = rb(pDS, SI);
L01D5:
    /* 01D5  repne   scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (ZF) break; }
L01D7:
    /* 01D7  jnz     short str_str_none */
    if (!ZF) goto L01F3;
L01D9:
    /* 01D9  dec     di */
    DI = (uint16_t)(DI - 1);
L01DA:
    /* 01DA  mov     bp,di */
    BP = DI;
L01DC:
    /* 01DC  inc     cx */
    CX = (uint16_t)(CX + 1);
L01DD:
    /* 01DD  cmp     cx,dx */
    sub16(CX, DX, 0);
L01DF:
    /* 01DF  jb      short str_str_none */
    if (CF) goto L01F3;
L01E1:
    /* 01E1  mov     ax,cx */
    AX = CX;
L01E3:
    /* 01E3  mov     cx,dx */
    CX = DX;
L01E5:
    /* 01E5  repe    cmpsb */
    while (CX) { sub8(rb(pDS, SI), rb(pES, DI), 0); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; if (!ZF) break; }
L01E7:
    /* 01E7  mov     cx,ax */
    CX = AX;
L01E9:
    /* 01E9  jnz     str_str_next */
    if (!ZF) goto L01CE;
L01EB:
    /* 01EB  sub     di,dx */
    DI = sub16(DI, DX, 0);
L01ED:
    /* 01ED  mov     dx,es */
    DX = asm_es;
L01EF:
    /* 01EF  mov     ax,di */
    AX = DI;
L01F1:
    /* 01F1  jmp     short str_str_done */
    goto L01F7;
L01F3: /* str_str_none */
    /* 01F3  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L01F5:
    /* 01F5  xor     ax,ax */
    AX = logic16((uint16_t)(AX ^ AX));
L01F7: /* str_str_done */
    /* 01F7  pop     di */
    DI = pop16();
L01F8:
    /* 01F8  pop     si */
    SI = pop16();
L01F9:
    /* 01F9  pop     ds */
    SET_DS(pop16());
L01FA:
    /* 01FA  pop     bp */
    BP = pop16();
L01FB:
    /* 01FB  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* int far str_cmp(char far *a, char far *b) */
L01FC: /* _str_cmp */
    /* 01FC  push    bp */
    push16(BP);
L01FD:
    /* 01FD  mov     bp,sp */
    BP = SP;
L01FF:
    /* 01FF  push    ds */
    push16(asm_ds);
L0200:
    /* 0200  push    si */
    push16(SI);
L0201:
    /* 0201  push    di */
    push16(DI);
L0202:
    /* 0202  xor     ax,ax */
    AX = logic16((uint16_t)(AX ^ AX));
L0204:
    /* 0204  les     di,[bp+6] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0x6)), s_ = rw(pSS, (uint16_t)(BP + 0x6 + 2));
    DI = o_;
    SET_ES(s_); }
L0207:
    /* 0207  mov     cx,0FFFFh */
    CX = 0xFFFF;
L020A:
    /* 020A  repne   scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (ZF) break; }
L020C:
    /* 020C  not     cx */
    CX = (uint16_t)~CX;
L020E:
    /* 020E  sub     di,cx */
    DI = sub16(DI, CX, 0);
L0210:
    /* 0210  lds     si,[bp+0Ah] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0xA)), s_ = rw(pSS, (uint16_t)(BP + 0xA + 2));
    SI = o_;
    SET_DS(s_); }
L0213:
    /* 0213  repe    cmpsb */
    while (CX) { sub8(rb(pDS, SI), rb(pES, DI), 0); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; if (!ZF) break; }
L0215:
    /* 0215  mov     al,es:[di-1] */
    AL = rb(pES, DI + 0xFFFF);
L0219:
    /* 0219  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L021B:
    /* 021B  mov     dl,[si-1] */
    DL = rb(pDS, SI + 0xFFFF);
L021E:
    /* 021E  sub     ax,dx */
    AX = sub16(AX, DX, 0);
L0220:
    /* 0220  pop     di */
    DI = pop16();
L0221:
    /* 0221  pop     si */
    SI = pop16();
L0222:
    /* 0222  pop     ds */
    SET_DS(pop16());
L0223:
    /* 0223  pop     bp */
    BP = pop16();
L0224:
    /* 0224  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* void far local_do_palette(int count, unsigned char first)
       Waits for vertical retrace and loads count DAC entries from palette, starting at first. */
L0225: /* _local_do_palette */
    /* 0225  push    bp */
    push16(BP);
L0226:
    /* 0226  mov     bp,sp */
    BP = SP;
L0228:
    /* 0228  push    si */
    push16(SI);
L0229:
    /* 0229  push    di */
    push16(DI);
L022A:
    /* 022A  mov     dx,INPUT_STATUS */
    DX = 0x3DA;
L022D: /* palette_wait */
    /* 022D  in      al,dx */
    AL = asm_in8(DX);
L022E:
    /* 022E  and     al,8 */
    AL = logic8((uint8_t)(AL & 0x8));
L0230:
    /* 0230  jz      palette_wait */
    if (ZF) goto L022D;
L0232:
    /* 0232  mov     al,[bp+8] */
    AL = rb(pSS, BP + 0x8);
L0235:
    /* 0235  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L0237:
    /* 0237  mov     di,ax */
    DI = AX;
L0239:
    /* 0239  mov     dx,DAC_WRITE_INDEX */
    DX = 0x3C8;
L023C:
    /* 023C  out     dx,al */
    asm_out8(DX, AL);
L023D:
    /* 023D  inc     dx */
    DX = (uint16_t)(DX + 1);
L023E:
    /* 023E  mov     cx,[bp+6] */
    CX = rw(pSS, BP + 0x6);
L0241:
    /* 0241  push    ds */
    push16(asm_ds);
L0242:
    /* 0242  lds     si,_palette */
    /* by hand: lds si,_palette: GRCORE's far pointer to the RGB triples (seg048:5046), a C object (gfx/grcore.c) */
    { unsigned s_, o_; port_fp_split_recent(palette, &s_, &o_); SI = (uint16_t)o_; SET_DS(s_); }
L0246:
    /* 0246  add     si,di */
    SI = (uint16_t)(SI + DI);
L0248:
    /* 0248  add     si,di */
    SI = (uint16_t)(SI + DI);
L024A:
    /* 024A  add     si,di */
    SI = (uint16_t)(SI + DI);
L024C:
    /* 024C  mov     ax,cx */
    AX = CX;
L024E:
    /* 024E  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L0250:
    /* 0250  add     cx,ax */
    CX = add16(CX, AX, 0);
L0252: /* palette_out */
    /* 0252  outsb */
    asm_out8(DX, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1));
L0253:
    /* 0253  loop    palette_out */
    if (--CX) goto L0252;
L0255:
    /* 0255  pop     ds */
    SET_DS(pop16());
L0256:
    /* 0256  pop     di */
    DI = pop16();
L0257:
    /* 0257  pop     si */
    SI = pop16();
L0258:
    /* 0258  pop     bp */
    BP = pop16();
L0259:
    /* 0259  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* void far seg015_1F9B_25A(int x, int y, int colour)    (UW2 gr_pixel)
          screen line 199 - y */
L025A: /* _seg015_1F9B_25A */
    /* 025A  push    bp */
    push16(BP);
L025B:
    /* 025B  mov     bp,sp */
    BP = SP;
L025D:
    /* 025D  sub     sp,0Eh */
    SP = (uint16_t)(SP - 0xE);
L0260:
    /* 0260  push    si */
    push16(SI);
L0261:
    /* 0261  push    di */
    push16(DI);
L0262:
    /* 0262  les     bx,_dseg_5c99_2404 */
    /* by hand: les bx,_dseg_5c99_2404, as at 0025 */
    { unsigned s_, o_; port_fp_split_recent(dseg_5c99_2404, &s_, &o_); BX = (uint16_t)o_; SET_ES(s_); }
L0266:
    /* 0266  mov     ax,es:[bx] */
    AX = rw(pES, BX);
L0269:
    /* 0269  mov     bx,ax */
    BX = AX;
L026B:
    /* 026B  mov     ax,0A000h */
    AX = 0xA000;
L026E:
    /* 026E  mov     es,ax */
    SET_ES(AX);
L0270:
    /* 0270  mov     ax,50h */
    AX = 0x50;
L0273:
    /* 0273  mov     dx,0C7h */
    DX = 0xC7;
L0276:
    /* 0276  sub     dx,[bp+8] */
    DX = sub16(DX, rw(pSS, BP + 0x8), 0);
L0279:
    /* 0279  imul    dx */
    imul16(DX);
L027B:
    /* 027B  mov     dx,[bp+6] */
    DX = rw(pSS, BP + 0x6);
L027E:
    /* 027E  mov     cl,2 */
    CL = 0x2;
L0280:
    /* 0280  shr     dx,cl */
    DX = (uint16_t)(DX >> (CL & 31));
L0282:
    /* 0282  add     dx,ax */
    DX = (uint16_t)(DX + AX);
L0284:
    /* 0284  add     bx,dx */
    BX = (uint16_t)(BX + DX);
L0286:
    /* 0286  mov     dx,SC_INDEX */
    DX = 0x3C4;
L0289:
    /* 0289  mov     al,2 */
    AL = 0x2;
L028B:
    /* 028B  out     dx,al */
    asm_out8(DX, AL);
L028C:
    /* 028C  mov     dx,SC_DATA */
    DX = 0x3C5;
L028F:
    /* 028F  mov     cx,[bp+6] */
    CX = rw(pSS, BP + 0x6);
L0292:
    /* 0292  and     cx,3 */
    CX = logic16((uint16_t)(CX & 0x3));
L0295:
    /* 0295  mov     ax,1 */
    AX = 0x1;
L0298:
    /* 0298  shl     ax,cl */
    AX = shl16(AX, CL);
L029A:
    /* 029A  out     dx,al */
    asm_out8(DX, AL);
L029B:
    /* 029B  mov     ax,[bp+0Ah] */
    AX = rw(pSS, BP + 0xA);
L029E:
    /* 029E  mov     es:[bx],al */
    wb(pES, BX, AL);
L02A1:
    /* 02A1  pop     di */
    DI = pop16();
L02A2:
    /* 02A2  pop     si */
    SI = pop16();
L02A3:
    /* 02A3  mov     sp,bp */
    SP = BP;
L02A5:
    /* 02A5  pop     bp */
    BP = pop16();
L02A6:
    /* 02A6  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* int far seg015_1F9B_2A7(void far *src, int dst, int w, int h)
       Writes a header byte at dst holding 4, w and h in planes 0, 1 and 2, then the w by h
       image after it, a plane at a time; returns dst. */
L02A7: /* _seg015_1F9B_2A7 */
    /* 02A7  push    bp */
    push16(BP);
L02A8:
    /* 02A8  mov     bp,sp */
    BP = SP;
L02AA:
    /* 02AA  sub     sp,2 */
    SP = (uint16_t)(SP - 0x2);
L02AD:
    /* 02AD  push    ds */
    push16(asm_ds);
L02AE:
    /* 02AE  push    si */
    push16(SI);
L02AF:
    /* 02AF  push    di */
    push16(DI);
L02B0:
    /* 02B0  mov     dx,SC_INDEX */
    DX = 0x3C4;
L02B3:
    /* 02B3  mov     al,2 */
    AL = 0x2;
L02B5:
    /* 02B5  out     dx,al */
    asm_out8(DX, AL);
L02B6:
    /* 02B6  mov     ax,0A000h */
    AX = 0xA000;
L02B9:
    /* 02B9  mov     es,ax */
    SET_ES(AX);
L02BB:
    /* 02BB  mov     ax,1001h */
    AX = 0x1001;
L02BE:
    /* 02BE  mov     dx,SC_DATA */
    DX = 0x3C5;
L02C1:
    /* 02C1  mov     di,[bp+0Ah] */
    DI = rw(pSS, BP + 0xA);
L02C4:
    /* 02C4  out     dx,al */
    asm_out8(DX, AL);
L02C5:
    /* 02C5  mov     byte ptr es:[di],4 */
    wb(pES, DI, 0x4);
L02C9:
    /* 02C9  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L02CB:
    /* 02CB  out     dx,al */
    asm_out8(DX, AL);
L02CC:
    /* 02CC  mov     cx,[bp+0Ch] */
    CX = rw(pSS, BP + 0xC);
L02CF:
    /* 02CF  mov     es:[di],cl */
    wb(pES, DI, CL);
L02D2:
    /* 02D2  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L02D4:
    /* 02D4  out     dx,al */
    asm_out8(DX, AL);
L02D5:
    /* 02D5  mov     cx,[bp+0Eh] */
    CX = rw(pSS, BP + 0xE);
L02D8:
    /* 02D8  mov     es:[di],cl */
    wb(pES, DI, CL);
L02DB:
    /* 02DB  inc     word ptr [bp+0Ah] */
    ww(pSS, BP + 0xA, (uint16_t)(rw(pSS, BP + 0xA) + 1));
L02DE:
    /* 02DE  mov     dx,si */
    DX = SI;
L02E0:
    /* 02E0  lds     bx,[bp+6] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0x6)), s_ = rw(pSS, (uint16_t)(BP + 0x6 + 2));
    BX = o_;
    SET_DS(s_); }
L02E3:
    /* 02E3  mov     ax,1001h */
    AX = 0x1001;
L02E6:
    /* 02E6  xor     dx,dx */
    DX = (uint16_t)(DX ^ DX);
L02E8: /* draw_plane */
    /* 02E8  mov     dx,SC_DATA */
    DX = 0x3C5;
L02EB:
    /* 02EB  out     dx,al */
    asm_out8(DX, AL);
L02EC:
    /* 02EC  mov     dx,[bp+0Eh] */
    DX = rw(pSS, BP + 0xE);
L02EF:
    /* 02EF  mov     [bp-2],dx */
    ww(pSS, BP + 0xFFFE, DX);
L02F2:
    /* 02F2  mov     di,[bp+0Ah] */
    DI = rw(pSS, BP + 0xA);
L02F5: /* draw_row */
    /* 02F5  mov     si,bx */
    SI = BX;
L02F7:
    /* 02F7  mov     cx,[bp+0Ch] */
    CX = rw(pSS, BP + 0xC);
L02FA:
    /* 02FA  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L02FD:
    /* 02FD  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0300: /* draw_byte */
    /* 0300  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0301:
    /* 0301  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0304:
    /* 0304  loop    draw_byte */
    if (--CX) goto L0300;
L0306:
    /* 0306  add     bx,[bp+0Ch] */
    BX = (uint16_t)(BX + rw(pSS, BP + 0xC));
L0309:
    /* 0309  dec     word ptr [bp-2] */
    ww(pSS, BP + 0xFFFE, dec16(rw(pSS, BP + 0xFFFE)));
L030C:
    /* 030C  jnz     draw_row */
    if (!ZF) goto L02F5;
L030E:
    /* 030E  inc     word ptr [bp+6] */
    ww(pSS, BP + 0x6, (uint16_t)(rw(pSS, BP + 0x6) + 1));
L0311:
    /* 0311  mov     bx,[bp+6] */
    BX = rw(pSS, BP + 0x6);
L0314:
    /* 0314  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L0316:
    /* 0316  cmp     al,ah */
    sub8(AL, AH, 0);
L0318:
    /* 0318  jb      draw_plane */
    if (CF) goto L02E8;
L031A:
    /* 031A  mov     ax,[bp+0Ah] */
    AX = rw(pSS, BP + 0xA);
L031D:
    /* 031D  dec     ax */
    AX = dec16(AX);
L031E:
    /* 031E  pop     di */
    DI = pop16();
L031F:
    /* 031F  pop     si */
    SI = pop16();
L0320:
    /* 0320  pop     ds */
    SET_DS(pop16());
L0321:
    /* 0321  mov     sp,bp */
    SP = BP;
L0323:
    /* 0323  pop     bp */
    BP = pop16();
L0324:
    /* 0324  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* void far seg015_1F9B_325(unsigned offset, int far *width, int far *height)
       Reads planes 1 and 2 of the byte at offset (a header written by 2A2) into two ints. */
L0325: /* _seg015_1F9B_325 */
    /* 0325  push    bp */
    push16(BP);
L0326:
    /* 0326  mov     bp,sp */
    BP = SP;
L0328:
    /* 0328  push    ds */
    push16(asm_ds);
L0329:
    /* 0329  push    si */
    push16(SI);
L032A:
    /* 032A  mov     dx,GC_INDEX */
    DX = 0x3CE;
L032D:
    /* 032D  mov     al,4 */
    AL = 0x4;
L032F:
    /* 032F  out     dx,al */
    asm_out8(DX, AL);
L0330:
    /* 0330  mov     bx,0A000h */
    BX = 0xA000;
L0333:
    /* 0333  mov     es,bx */
    SET_ES(BX);
L0335:
    /* 0335  mov     bx,[bp+6] */
    BX = rw(pSS, BP + 0x6);
L0338:
    /* 0338  mov     dx,GC_DATA */
    DX = 0x3CF;
L033B:
    /* 033B  mov     al,1 */
    AL = 0x1;
L033D:
    /* 033D  out     dx,al */
    asm_out8(DX, AL);
L033E:
    /* 033E  mov     cl,es:[bx] */
    CL = rb(pES, BX);
L0341:
    /* 0341  lds     si,[bp+8] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0x8)), s_ = rw(pSS, (uint16_t)(BP + 0x8 + 2));
    SI = o_;
    SET_DS(s_); }
L0344:
    /* 0344  mov     [si],cl */
    wb(pDS, SI, CL);
L0346:
    /* 0346  mov     byte ptr [si+1],0 */
    wb(pDS, SI + 0x1, 0x0);
L034A:
    /* 034A  inc     al */
    AL = inc8(AL);
L034C:
    /* 034C  out     dx,al */
    asm_out8(DX, AL);
L034D:
    /* 034D  lds     si,[bp+0Ch] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0xC)), s_ = rw(pSS, (uint16_t)(BP + 0xC + 2));
    SI = o_;
    SET_DS(s_); }
L0350:
    /* 0350  mov     cl,es:[bx] */
    CL = rb(pES, BX);
L0353:
    /* 0353  mov     [si],cl */
    wb(pDS, SI, CL);
L0355:
    /* 0355  mov     byte ptr [si+1],0 */
    wb(pDS, SI + 0x1, 0x0);
L0359:
    /* 0359  mov     dx,GC_INDEX */
    DX = 0x3CE;
L035C:
    /* 035C  mov     ax,0FF08h */
    AX = 0xFF08;
L035F:
    /* 035F  out     dx,ax */
    asm_out16(DX, AX);
L0360:
    /* 0360  pop     si */
    SI = pop16();
L0361:
    /* 0361  pop     ds */
    SET_DS(pop16());
L0362:
    /* 0362  mov     sp,bp */
    SP = BP;
L0364:
    /* 0364  pop     bp */
    BP = pop16();
L0365:
    /* 0365  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* unsigned far seg015_1F9B_366(void far *src, int size)
       Allocates video memory for size bytes and copies src into it a plane at a time;
       returns the video offset, or 0. The map mask is set only before planes 1 to 3, so the
       first quarter goes to whichever planes the mask last enabled. */
L0366: /* _seg015_1F9B_366 */
    /* 0366  push    bp */
    push16(BP);
L0367:
    /* 0367  mov     bp,sp */
    BP = SP;
L0369:
    /* 0369  push    ds */
    push16(asm_ds);
L036A:
    /* 036A  push    si */
    push16(SI);
L036B:
    /* 036B  push    di */
    push16(DI);
L036C:
    /* 036C  mov     dx,SC_INDEX */
    DX = 0x3C4;
L036F:
    /* 036F  mov     al,2 */
    AL = 0x2;
L0371:
    /* 0371  out     dx,al */
    asm_out8(DX, AL);
L0372:
    /* 0372  mov     ax,[bp+0Ah] */
    AX = rw(pSS, BP + 0xA);
L0375:
    /* 0375  mov     bx,1 */
    BX = 0x1;
L0378:
    /* 0378  call    _seg001_5A */
    if ((c = asm_callf(ASM_JMP(0x004E, 0x005A), 0x1DAE + PORT_LOAD_SEG, 0x037D)) != 0) return c;
L037D:
    /* 037D  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L037F:
    /* 037F  jz      short store_done */
    if (ZF) goto L03B4;
L0381:
    /* 0381  mov     bx,ax */
    BX = AX;
L0383:
    /* 0383  mov     dx,[bp+0Ah] */
    DX = rw(pSS, BP + 0xA);
L0386:
    /* 0386  add     dx,3 */
    DX = (uint16_t)(DX + 0x3);
L0389:
    /* 0389  shr     dx,2 */
    DX = (uint16_t)(DX >> 2);
L038C:
    /* 038C  mov     ax,0A000h */
    AX = 0xA000;
L038F:
    /* 038F  mov     es,ax */
    SET_ES(AX);
L0391:
    /* 0391  mov     ax,1001h */
    AX = 0x1001;
L0394: /* store_plane */
    /* 0394  lds     si,[bp+6] */
    { uint16_t o_ = rw(pSS, (uint16_t)(BP + 0x6)), s_ = rw(pSS, (uint16_t)(BP + 0x6 + 2));
    SI = o_;
    SET_DS(s_); }
L0397:
    /* 0397  mov     di,bx */
    DI = BX;
L0399:
    /* 0399  mov     cx,dx */
    CX = DX;
L039B: /* store_byte */
    /* 039B  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L039C:
    /* 039C  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L039F:
    /* 039F  loop    store_byte */
    if (--CX) goto L039B;
L03A1:
    /* 03A1  inc     word ptr [bp+6] */
    ww(pSS, BP + 0x6, (uint16_t)(rw(pSS, BP + 0x6) + 1));
L03A4:
    /* 03A4  shl     al,1 */
    AL = (uint8_t)(AL << 1);
L03A6:
    /* 03A6  cmp     al,ah */
    sub8(AL, AH, 0);
L03A8:
    /* 03A8  jnb     short store_result */
    if (!CF) goto L03B2;
L03AA:
    /* 03AA  push    dx */
    push16(DX);
L03AB:
    /* 03AB  mov     dx,SC_DATA */
    DX = 0x3C5;
L03AE:
    /* 03AE  out     dx,al */
    asm_out8(DX, AL);
L03AF:
    /* 03AF  pop     dx */
    DX = pop16();
L03B0:
    /* 03B0  jmp     store_plane */
    goto L0394;
L03B2: /* store_result */
    /* 03B2  mov     ax,bx */
    AX = BX;
L03B4: /* store_done */
    /* 03B4  pop     di */
    DI = pop16();
L03B5:
    /* 03B5  pop     si */
    SI = pop16();
L03B6:
    /* 03B6  pop     ds */
    SET_DS(pop16());
L03B7:
    /* 03B7  mov     sp,bp */
    SP = BP;
L03B9:
    /* 03B9  pop     bp */
    BP = pop16();
L03BA:
    /* 03BA  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* An empty routine at the end of the segment (+3BBh), as in UW2. */
L03BB: /* seg015_1F9B_3BB */
    /* 03BB  push    bp */
    push16(BP);
L03BC:
    /* 03BC  mov     bp,sp */
    BP = SP;
L03BE:
    /* 03BE  mov     sp,bp */
    SP = BP;
L03C0:
    /* 03C0  pop     bp */
    BP = pop16();
L03C1:
    /* 03C1  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;
}
