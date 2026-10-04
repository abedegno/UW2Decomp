/* sprite.c: replaces src/gfx/SPRITE.ASM (seg000, 0000..04EA of its
   segment). Written by tools/asm2c.py from the assembly and the bytes it assembles to; do not
   edit by hand: change the tool, or its OVERRIDES, and run it again. Each instruction is the
   C below its source line; the comments before a routine are the .ASM file's. */
#include "x86/asmrt.h"

uint16_t port_dgroup_seg(void);        /* x86/entry.c */
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

uint32_t asm_mod_SPRITE(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x0002: goto L0002;
    case 0x0003: goto L0003;
    case 0x0005: goto L0005;
    case 0x0006: goto L0006;
    case 0x0007: goto L0007;
    case 0x0008: goto L0008;
    case 0x0009: goto L0009;
    case 0x000C: goto L000C;
    case 0x000D: goto L000D;
    case 0x0010: goto L0010;
    case 0x0011: goto L0011;
    case 0x0014: goto L0014;
    case 0x0018: goto L0018;
    case 0x001A: goto L001A;
    case 0x001D: goto L001D;
    case 0x0021: goto L0021;
    case 0x0023: goto L0023;
    case 0x0026: goto L0026;
    case 0x0028: goto L0028;
    case 0x002C: goto L002C;
    case 0x002F: goto L002F;
    case 0x0031: goto L0031;
    case 0x0037: goto L0037;
    case 0x0039: goto L0039;
    case 0x003C: goto L003C;
    case 0x003F: goto L003F;
    case 0x0042: goto L0042;
    case 0x0044: goto L0044;
    case 0x0046: goto L0046;
    case 0x0049: goto L0049;
    case 0x004C: goto L004C;
    case 0x0051: goto L0051;
    case 0x0053: goto L0053;
    case 0x0055: goto L0055;
    case 0x0058: goto L0058;
    case 0x005A: goto L005A;
    case 0x005C: goto L005C;
    case 0x005F: goto L005F;
    case 0x0063: goto L0063;
    case 0x0065: goto L0065;
    case 0x0068: goto L0068;
    case 0x006B: goto L006B;
    case 0x006C: goto L006C;
    case 0x006D: goto L006D;
    case 0x006E: goto L006E;
    case 0x006F: goto L006F;
    case 0x0071: goto L0071;
    case 0x0072: goto L0072;
    case 0x0073: goto L0073;
    case 0x0074: goto L0074;
    case 0x0076: goto L0076;
    case 0x0077: goto L0077;
    case 0x0078: goto L0078;
    case 0x0079: goto L0079;
    case 0x007A: goto L007A;
    case 0x007D: goto L007D;
    case 0x007E: goto L007E;
    case 0x0081: goto L0081;
    case 0x0082: goto L0082;
    case 0x0085: goto L0085;
    case 0x0088: goto L0088;
    case 0x008A: goto L008A;
    case 0x008C: goto L008C;
    case 0x008F: goto L008F;
    case 0x0094: goto L0094;
    case 0x0099: goto L0099;
    case 0x009C: goto L009C;
    case 0x009E: goto L009E;
    case 0x009F: goto L009F;
    case 0x00A0: goto L00A0;
    case 0x00A1: goto L00A1;
    case 0x00A2: goto L00A2;
    case 0x00A4: goto L00A4;
    case 0x00A5: goto L00A5;
    case 0x00A6: goto L00A6;
    case 0x00A7: goto L00A7;
    case 0x00A9: goto L00A9;
    case 0x00AA: goto L00AA;
    case 0x00AB: goto L00AB;
    case 0x00AC: goto L00AC;
    case 0x00AD: goto L00AD;
    case 0x00B0: goto L00B0;
    case 0x00B1: goto L00B1;
    case 0x00B4: goto L00B4;
    case 0x00B5: goto L00B5;
    case 0x00B8: goto L00B8;
    case 0x00BB: goto L00BB;
    case 0x00BD: goto L00BD;
    case 0x00BF: goto L00BF;
    case 0x00C2: goto L00C2;
    case 0x00C5: goto L00C5;
    case 0x00C9: goto L00C9;
    case 0x00CC: goto L00CC;
    case 0x00D0: goto L00D0;
    case 0x00D3: goto L00D3;
    case 0x00D7: goto L00D7;
    case 0x00DA: goto L00DA;
    case 0x00DE: goto L00DE;
    case 0x00E1: goto L00E1;
    case 0x00E3: goto L00E3;
    case 0x00E4: goto L00E4;
    case 0x00E5: goto L00E5;
    case 0x00E6: goto L00E6;
    case 0x00E7: goto L00E7;
    case 0x00E9: goto L00E9;
    case 0x00EA: goto L00EA;
    case 0x00EB: goto L00EB;
    case 0x00EC: goto L00EC;
    case 0x00EE: goto L00EE;
    case 0x00EF: goto L00EF;
    case 0x00F0: goto L00F0;
    case 0x00F1: goto L00F1;
    case 0x00F2: goto L00F2;
    case 0x00F5: goto L00F5;
    case 0x00F6: goto L00F6;
    case 0x00F9: goto L00F9;
    case 0x00FA: goto L00FA;
    case 0x00FD: goto L00FD;
    case 0x0100: goto L0100;
    case 0x0102: goto L0102;
    case 0x0104: goto L0104;
    case 0x0107: goto L0107;
    case 0x010A: goto L010A;
    case 0x010E: goto L010E;
    case 0x0113: goto L0113;
    case 0x0116: goto L0116;
    case 0x0118: goto L0118;
    case 0x0119: goto L0119;
    case 0x011A: goto L011A;
    case 0x011B: goto L011B;
    case 0x011C: goto L011C;
    case 0x011E: goto L011E;
    case 0x011F: goto L011F;
    case 0x0120: goto L0120;
    case 0x0121: goto L0121;
    case 0x0123: goto L0123;
    case 0x0124: goto L0124;
    case 0x0125: goto L0125;
    case 0x0126: goto L0126;
    case 0x0127: goto L0127;
    case 0x012A: goto L012A;
    case 0x012B: goto L012B;
    case 0x012E: goto L012E;
    case 0x012F: goto L012F;
    case 0x0132: goto L0132;
    case 0x0135: goto L0135;
    case 0x0137: goto L0137;
    case 0x0139: goto L0139;
    case 0x013C: goto L013C;
    case 0x013F: goto L013F;
    case 0x0143: goto L0143;
    case 0x0148: goto L0148;
    case 0x014B: goto L014B;
    case 0x014D: goto L014D;
    case 0x014E: goto L014E;
    case 0x014F: goto L014F;
    case 0x0150: goto L0150;
    case 0x0151: goto L0151;
    case 0x0153: goto L0153;
    case 0x0154: goto L0154;
    case 0x0155: goto L0155;
    case 0x0156: goto L0156;
    case 0x0158: goto L0158;
    case 0x0159: goto L0159;
    case 0x015A: goto L015A;
    case 0x015B: goto L015B;
    case 0x015C: goto L015C;
    case 0x015F: goto L015F;
    case 0x0160: goto L0160;
    case 0x0163: goto L0163;
    case 0x0164: goto L0164;
    case 0x0167: goto L0167;
    case 0x016A: goto L016A;
    case 0x016C: goto L016C;
    case 0x016E: goto L016E;
    case 0x0171: goto L0171;
    case 0x0177: goto L0177;
    case 0x0179: goto L0179;
    case 0x017E: goto L017E;
    case 0x0181: goto L0181;
    case 0x0183: goto L0183;
    case 0x0184: goto L0184;
    case 0x0185: goto L0185;
    case 0x0186: goto L0186;
    case 0x0187: goto L0187;
    case 0x0189: goto L0189;
    case 0x018A: goto L018A;
    case 0x018B: goto L018B;
    case 0x018C: goto L018C;
    case 0x018E: goto L018E;
    case 0x018F: goto L018F;
    case 0x0190: goto L0190;
    case 0x0191: goto L0191;
    case 0x0192: goto L0192;
    case 0x0195: goto L0195;
    case 0x0196: goto L0196;
    case 0x0199: goto L0199;
    case 0x019A: goto L019A;
    case 0x019D: goto L019D;
    case 0x01A0: goto L01A0;
    case 0x01A2: goto L01A2;
    case 0x01A4: goto L01A4;
    case 0x01A7: goto L01A7;
    case 0x01AA: goto L01AA;
    case 0x01AE: goto L01AE;
    case 0x01B1: goto L01B1;
    case 0x01B5: goto L01B5;
    case 0x01B8: goto L01B8;
    case 0x01BA: goto L01BA;
    case 0x01BB: goto L01BB;
    case 0x01BC: goto L01BC;
    case 0x01BD: goto L01BD;
    case 0x01BE: goto L01BE;
    case 0x01C0: goto L01C0;
    case 0x01C1: goto L01C1;
    case 0x01C2: goto L01C2;
    case 0x01C3: goto L01C3;
    case 0x01C5: goto L01C5;
    case 0x01C6: goto L01C6;
    case 0x01C7: goto L01C7;
    case 0x01C8: goto L01C8;
    case 0x01C9: goto L01C9;
    case 0x01CC: goto L01CC;
    case 0x01CD: goto L01CD;
    case 0x01D0: goto L01D0;
    case 0x01D1: goto L01D1;
    case 0x01D4: goto L01D4;
    case 0x01D7: goto L01D7;
    case 0x01D9: goto L01D9;
    case 0x01DB: goto L01DB;
    case 0x01DE: goto L01DE;
    case 0x01E1: goto L01E1;
    case 0x01E5: goto L01E5;
    case 0x01E8: goto L01E8;
    case 0x01EC: goto L01EC;
    case 0x01EF: goto L01EF;
    case 0x01F1: goto L01F1;
    case 0x01F2: goto L01F2;
    case 0x01F3: goto L01F3;
    case 0x01F4: goto L01F4;
    case 0x01F5: goto L01F5;
    case 0x01F7: goto L01F7;
    case 0x01F8: goto L01F8;
    case 0x01F9: goto L01F9;
    case 0x01FA: goto L01FA;
    case 0x01FC: goto L01FC;
    case 0x01FD: goto L01FD;
    case 0x01FE: goto L01FE;
    case 0x01FF: goto L01FF;
    case 0x0200: goto L0200;
    case 0x0203: goto L0203;
    case 0x0204: goto L0204;
    case 0x0207: goto L0207;
    case 0x0208: goto L0208;
    case 0x020B: goto L020B;
    case 0x020E: goto L020E;
    case 0x0210: goto L0210;
    case 0x0212: goto L0212;
    case 0x0215: goto L0215;
    case 0x0218: goto L0218;
    case 0x021C: goto L021C;
    case 0x021F: goto L021F;
    case 0x0221: goto L0221;
    case 0x0222: goto L0222;
    case 0x0223: goto L0223;
    case 0x0224: goto L0224;
    case 0x0225: goto L0225;
    case 0x0227: goto L0227;
    case 0x0228: goto L0228;
    case 0x0229: goto L0229;
    case 0x0230: goto L0230;
    case 0x0232: goto L0232;
    case 0x0235: goto L0235;
    case 0x0239: goto L0239;
    case 0x023C: goto L023C;
    case 0x023F: goto L023F;
    case 0x0241: goto L0241;
    case 0x0245: goto L0245;
    case 0x0247: goto L0247;
    case 0x024A: goto L024A;
    case 0x024C: goto L024C;
    case 0x024E: goto L024E;
    case 0x0253: goto L0253;
    case 0x0255: goto L0255;
    case 0x0259: goto L0259;
    case 0x025C: goto L025C;
    case 0x025F: goto L025F;
    case 0x0261: goto L0261;
    case 0x0265: goto L0265;
    case 0x0267: goto L0267;
    case 0x026A: goto L026A;
    case 0x026C: goto L026C;
    case 0x026E: goto L026E;
    case 0x0272: goto L0272;
    case 0x0276: goto L0276;
    case 0x0278: goto L0278;
    case 0x027D: goto L027D;
    case 0x027F: goto L027F;
    case 0x0281: goto L0281;
    case 0x0285: goto L0285;
    case 0x0289: goto L0289;
    case 0x028B: goto L028B;
    case 0x028E: goto L028E;
    case 0x028F: goto L028F;
    case 0x0293: goto L0293;
    case 0x0297: goto L0297;
    case 0x0299: goto L0299;
    case 0x029E: goto L029E;
    case 0x029F: goto L029F;
    case 0x02A3: goto L02A3;
    case 0x02A6: goto L02A6;
    case 0x02A9: goto L02A9;
    case 0x02AD: goto L02AD;
    case 0x02B1: goto L02B1;
    case 0x02B3: goto L02B3;
    case 0x02B7: goto L02B7;
    case 0x02B9: goto L02B9;
    case 0x02BB: goto L02BB;
    case 0x02BF: goto L02BF;
    case 0x02C2: goto L02C2;
    case 0x02C6: goto L02C6;
    case 0x02C9: goto L02C9;
    case 0x02CD: goto L02CD;
    case 0x02D1: goto L02D1;
    case 0x02D3: goto L02D3;
    case 0x02D7: goto L02D7;
    case 0x02D9: goto L02D9;
    case 0x02DB: goto L02DB;
    case 0x02DF: goto L02DF;
    case 0x02E1: goto L02E1;
    case 0x02E3: goto L02E3;
    case 0x02E7: goto L02E7;
    case 0x02EB: goto L02EB;
    case 0x02ED: goto L02ED;
    case 0x02F1: goto L02F1;
    case 0x02F3: goto L02F3;
    case 0x02F5: goto L02F5;
    case 0x02F9: goto L02F9;
    case 0x02FB: goto L02FB;
    case 0x02FD: goto L02FD;
    case 0x02FF: goto L02FF;
    case 0x0302: goto L0302;
    case 0x0306: goto L0306;
    case 0x0309: goto L0309;
    case 0x030C: goto L030C;
    case 0x030E: goto L030E;
    case 0x0312: goto L0312;
    case 0x0314: goto L0314;
    case 0x0317: goto L0317;
    case 0x0319: goto L0319;
    case 0x031B: goto L031B;
    case 0x0320: goto L0320;
    case 0x0322: goto L0322;
    case 0x0326: goto L0326;
    case 0x0328: goto L0328;
    case 0x032C: goto L032C;
    case 0x032F: goto L032F;
    case 0x0330: goto L0330;
    case 0x0331: goto L0331;
    case 0x0333: goto L0333;
    case 0x0334: goto L0334;
    case 0x0335: goto L0335;
    case 0x0336: goto L0336;
    case 0x0337: goto L0337;
    case 0x033D: goto L033D;
    case 0x033F: goto L033F;
    case 0x0342: goto L0342;
    case 0x0345: goto L0345;
    case 0x0347: goto L0347;
    case 0x0349: goto L0349;
    case 0x034E: goto L034E;
    case 0x0351: goto L0351;
    case 0x0353: goto L0353;
    case 0x0356: goto L0356;
    case 0x0359: goto L0359;
    case 0x035B: goto L035B;
    case 0x035D: goto L035D;
    case 0x035F: goto L035F;
    case 0x0362: goto L0362;
    case 0x0364: goto L0364;
    case 0x0367: goto L0367;
    case 0x036D: goto L036D;
    case 0x036F: goto L036F;
    case 0x0374: goto L0374;
    case 0x0376: goto L0376;
    case 0x0377: goto L0377;
    case 0x0378: goto L0378;
    case 0x037C: goto L037C;
    case 0x0381: goto L0381;
    case 0x0382: goto L0382;
    case 0x0383: goto L0383;
    case 0x0389: goto L0389;
    case 0x038B: goto L038B;
    case 0x0390: goto L0390;
    case 0x0394: goto L0394;
    case 0x0396: goto L0396;
    case 0x0398: goto L0398;
    case 0x0399: goto L0399;
    case 0x039A: goto L039A;
    case 0x039F: goto L039F;
    case 0x03A0: goto L03A0;
    case 0x03A1: goto L03A1;
    case 0x03A4: goto L03A4;
    case 0x03A6: goto L03A6;
    case 0x03A9: goto L03A9;
    case 0x03AD: goto L03AD;
    case 0x03AF: goto L03AF;
    case 0x03B2: goto L03B2;
    case 0x03B4: goto L03B4;
    case 0x03B6: goto L03B6;
    case 0x03B8: goto L03B8;
    case 0x03BA: goto L03BA;
    case 0x03BD: goto L03BD;
    case 0x03C0: goto L03C0;
    case 0x03C2: goto L03C2;
    case 0x03C5: goto L03C5;
    case 0x03CB: goto L03CB;
    case 0x03CD: goto L03CD;
    case 0x03D2: goto L03D2;
    case 0x03D4: goto L03D4;
    case 0x03D5: goto L03D5;
    case 0x03D6: goto L03D6;
    case 0x03D7: goto L03D7;
    case 0x03DB: goto L03DB;
    case 0x03DF: goto L03DF;
    case 0x03E1: goto L03E1;
    case 0x03E5: goto L03E5;
    case 0x03E9: goto L03E9;
    case 0x03EB: goto L03EB;
    case 0x03EF: goto L03EF;
    case 0x03F4: goto L03F4;
    case 0x03F5: goto L03F5;
    case 0x03F6: goto L03F6;
    case 0x03F7: goto L03F7;
    case 0x03FA: goto L03FA;
    case 0x03FC: goto L03FC;
    case 0x03FE: goto L03FE;
    case 0x0402: goto L0402;
    case 0x0404: goto L0404;
    case 0x0406: goto L0406;
    case 0x0409: goto L0409;
    case 0x040C: goto L040C;
    case 0x040E: goto L040E;
    case 0x0411: goto L0411;
    case 0x0417: goto L0417;
    case 0x0419: goto L0419;
    case 0x041C: goto L041C;
    case 0x041D: goto L041D;
    case 0x041E: goto L041E;
    case 0x0422: goto L0422;
    case 0x0424: goto L0424;
    case 0x0426: goto L0426;
    case 0x0428: goto L0428;
    case 0x0429: goto L0429;
    case 0x042D: goto L042D;
    case 0x0431: goto L0431;
    case 0x0432: goto L0432;
    case 0x0436: goto L0436;
    case 0x0437: goto L0437;
    case 0x043B: goto L043B;
    case 0x043F: goto L043F;
    case 0x0445: goto L0445;
    case 0x0448: goto L0448;
    case 0x044A: goto L044A;
    case 0x044C: goto L044C;
    case 0x044F: goto L044F;
    case 0x0451: goto L0451;
    case 0x0457: goto L0457;
    case 0x045C: goto L045C;
    case 0x045F: goto L045F;
    case 0x0462: goto L0462;
    case 0x0464: goto L0464;
    case 0x046A: goto L046A;
    case 0x046C: goto L046C;
    case 0x0470: goto L0470;
    case 0x0472: goto L0472;
    case 0x0476: goto L0476;
    case 0x0477: goto L0477;
    case 0x047B: goto L047B;
    case 0x047C: goto L047C;
    case 0x0480: goto L0480;
    case 0x0484: goto L0484;
    case 0x048A: goto L048A;
    case 0x048D: goto L048D;
    case 0x048F: goto L048F;
    case 0x0491: goto L0491;
    case 0x0494: goto L0494;
    case 0x0496: goto L0496;
    case 0x049C: goto L049C;
    case 0x04A1: goto L04A1;
    case 0x04A4: goto L04A4;
    case 0x04A7: goto L04A7;
    case 0x04A9: goto L04A9;
    case 0x04AF: goto L04AF;
    case 0x04B2: goto L04B2;
    case 0x04B3: goto L04B3;
    case 0x04B6: goto L04B6;
    case 0x04B7: goto L04B7;
    case 0x04B8: goto L04B8;
    case 0x04B9: goto L04B9;
    case 0x04BC: goto L04BC;
    case 0x04BE: goto L04BE;
    case 0x04C0: goto L04C0;
    case 0x04C3: goto L04C3;
    case 0x04C6: goto L04C6;
    case 0x04CA: goto L04CA;
    case 0x04CC: goto L04CC;
    case 0x04CF: goto L04CF;
    case 0x04D2: goto L04D2;
    case 0x04D4: goto L04D4;
    case 0x04D6: goto L04D6;
    case 0x04DB: goto L04DB;
    case 0x04E2: goto L04E2;
    case 0x04E3: goto L04E3;
    case 0x04E4: goto L04E4;
    case 0x04E5: goto L04E5;
    case 0x04E6: goto L04E6;
    case 0x04E8: goto L04E8;
    case 0x04E9: goto L04E9;
    default: asm_bad_entry("SPRITE.ASM", entry);
    }

    /* int create_sprite(int layer, int a, int b): the number of a free record, marked in use
       (and transparent if Transparency is set), with the block valloc(a, b) when layer is not
       0; -1 if no record or no block is free */
L0002: /* _create_sprite */
    /* 0002  push    bp */
    push16(BP);
L0003:
    /* 0003  mov     bp,sp */
    BP = SP;
L0005:
    /* 0005  push    ds */
    push16(asm_ds);
L0006:
    /* 0006  push    es */
    push16(asm_es);
L0007:
    /* 0007  push    si */
    push16(SI);
L0008:
    /* 0008  push    di */
    push16(DI);
L0009:
    /* 0009  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L000C:
    /* 000C  pop     ds */
    SET_DS(pop16());
L000D:
    /* 000D  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L0010:
    /* 0010  pop     es */
    SET_ES(pop16());
L0011:
    /* 0011  mov     si,offset sp_inf_tab */
    SI = 0x8;
L0014: /* L0014 */
    /* 0014  test    word ptr [si],1 */
    logic16((uint16_t)(rw(pDS, SI) & 0x1));
L0018:
    /* 0018  je      short L0028 */
    if (ZF) goto L0028;
L001A:
    /* 001A  add     si,10h */
    SI = (uint16_t)(SI + 0x10);
L001D:
    /* 001D  cmp     si,offset sp_inf_end */
    sub16(SI, 0x408, 0);
L0021:
    /* 0021  jb      L0014 */
    if (CF) goto L0014;
L0023:
    /* 0023  mov     ax,0FFFFh */
    AX = 0xFFFF;
L0026:
    /* 0026  jmp     short L006B */
    goto L006B;
L0028: /* L0028 */
    /* 0028  mov     word ptr [si],5 */
    ww(pDS, SI, 0x5);
L002C:
    /* 002C  mov     ax,seg _Transparency */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L002F:
    /* 002F  mov     es,ax */
    SET_ES(AX);
L0031:
    /* 0031  cmp     es:_Transparency,0 */
    sub8(rb(pES, 0xDC7), 0x0, 0);
L0037:
    /* 0037  je      short L003C */
    if (ZF) goto L003C;
L0039:
    /* 0039  or      word ptr [si],10h */
    ww(pDS, SI, (uint16_t)(rw(pDS, SI) | 0x10));
L003C: /* L003C */
    /* 003C  mov     ax,[bp+6] */
    AX = rw(pSS, BP + 0x6);
L003F:
    /* 003F  mov     [si+9],ax */
    ww(pDS, SI + 0x9, AX);
L0042:
    /* 0042  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L0044:
    /* 0044  je      short L005A */
    if (ZF) goto L005A;
L0046:
    /* 0046  mov     ax,[bp+8] */
    AX = rw(pSS, BP + 0x8);
L0049:
    /* 0049  mov     bx,[bp+0Ah] */
    BX = rw(pSS, BP + 0xA);
L004C:
    /* 004C  call    _seg001_5A */
    if ((c = asm_callf(ASM_JMP(0x004E, 0x005A), 0x0000 + PORT_LOAD_SEG, 0x0051)) != 0) return c;
L0051:
    /* 0051  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L0053:
    /* 0053  jne     short L005C */
    if (!ZF) goto L005C;
L0055:
    /* 0055  mov     ax,0FFFFh */
    AX = 0xFFFF;
L0058:
    /* 0058  jmp     short L006B */
    goto L006B;
L005A: /* L005A */
    /* 005A  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L005C: /* L005C */
    /* 005C  mov     [si+0Dh],ax */
    ww(pDS, SI + 0xD, AX);
L005F:
    /* 005F  mov     byte ptr [si+8],0 */
    wb(pDS, SI + 0x8, 0x0);
L0063:
    /* 0063  mov     ax,si */
    AX = SI;
L0065:
    /* 0065  sub     ax,offset sp_inf_tab */
    AX = (uint16_t)(AX - 0x8);
L0068:
    /* 0068  shr     ax,4 */
    AX = shr16(AX, 4);
L006B: /* L006B */
    /* 006B  pop     di */
    DI = pop16();
L006C:
    /* 006C  pop     si */
    SI = pop16();
L006D:
    /* 006D  pop     es */
    SET_ES(pop16());
L006E:
    /* 006E  pop     ds */
    SET_DS(pop16());
L006F:
    /* 006F  mov     sp,bp */
    SP = BP;
L0071:
    /* 0071  pop     bp */
    BP = pop16();
L0072:
    /* 0072  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* destroy_sprite(int n): mark it destroyed and not visible */
L0073: /* _destroy_sprite */
    /* 0073  push    bp */
    push16(BP);
L0074:
    /* 0074  mov     bp,sp */
    BP = SP;
L0076:
    /* 0076  push    ds */
    push16(asm_ds);
L0077:
    /* 0077  push    es */
    push16(asm_es);
L0078:
    /* 0078  push    si */
    push16(SI);
L0079:
    /* 0079  push    di */
    push16(DI);
L007A:
    /* 007A  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L007D:
    /* 007D  pop     ds */
    SET_DS(pop16());
L007E:
    /* 007E  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L0081:
    /* 0081  pop     es */
    SET_ES(pop16());
L0082:
    /* 0082  mov     ax,[bp+6] */
    AX = rw(pSS, BP + 0x6);
L0085:
    /* 0085  cmp     ax,40h */
    sub16(AX, 0x40, 0);
L0088:
    /* 0088  ja      short L009E */
    if (!CF && !ZF) goto L009E;
L008A:
    /* 008A  mov     bx,ax */
    BX = AX;
L008C:
    /* 008C  shl     bx,4 */
    BX = (uint16_t)(BX << 4);
L008F:
    /* 008F  or      word ptr sp_inf_tab[bx],8 */
    ww(pDS, BX + 0x8, (uint16_t)(rw(pDS, BX + 0x8) | 0x8));
L0094:
    /* 0094  and     word ptr sp_inf_tab[bx],0FFFDh */
    ww(pDS, BX + 0x8, (uint16_t)(rw(pDS, BX + 0x8) & 0xFFFD));
L0099:
    /* 0099  call    sp_changed */
    if ((c = asm_call(ASM_JMP(0x0000, 0x0229), 0x009C)) != 0) return c;
L009C:
    /* 009C  xor     ax,ax */
    AX = logic16((uint16_t)(AX ^ AX));
L009E: /* L009E */
    /* 009E  pop     di */
    DI = pop16();
L009F:
    /* 009F  pop     si */
    SI = pop16();
L00A0:
    /* 00A0  pop     es */
    SET_ES(pop16());
L00A1:
    /* 00A1  pop     ds */
    SET_DS(pop16());
L00A2:
    /* 00A2  mov     sp,bp */
    SP = BP;
L00A4:
    /* 00A4  pop     bp */
    BP = pop16();
L00A5:
    /* 00A5  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* change_sprite(int n, int x, int y, int w, int h) */
L00A6: /* _change_sprite */
    /* 00A6  push    bp */
    push16(BP);
L00A7:
    /* 00A7  mov     bp,sp */
    BP = SP;
L00A9:
    /* 00A9  push    ds */
    push16(asm_ds);
L00AA:
    /* 00AA  push    es */
    push16(asm_es);
L00AB:
    /* 00AB  push    si */
    push16(SI);
L00AC:
    /* 00AC  push    di */
    push16(DI);
L00AD:
    /* 00AD  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L00B0:
    /* 00B0  pop     ds */
    SET_DS(pop16());
L00B1:
    /* 00B1  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L00B4:
    /* 00B4  pop     es */
    SET_ES(pop16());
L00B5:
    /* 00B5  mov     ax,[bp+6] */
    AX = rw(pSS, BP + 0x6);
L00B8:
    /* 00B8  cmp     ax,40h */
    sub16(AX, 0x40, 0);
L00BB:
    /* 00BB  ja      short L00E3 */
    if (!CF && !ZF) goto L00E3;
L00BD:
    /* 00BD  mov     bx,ax */
    BX = AX;
L00BF:
    /* 00BF  shl     bx,4 */
    BX = (uint16_t)(BX << 4);
L00C2:
    /* 00C2  mov     dx,[bp+8] */
    DX = rw(pSS, BP + 0x8);
L00C5:
    /* 00C5  mov     word ptr sp_inf_tab[bx+2],dx */
    ww(pDS, BX + 0xA, DX);
L00C9:
    /* 00C9  mov     dx,[bp+0Ah] */
    DX = rw(pSS, BP + 0xA);
L00CC:
    /* 00CC  mov     byte ptr sp_inf_tab[bx+4],dl */
    wb(pDS, BX + 0xC, DL);
L00D0:
    /* 00D0  mov     dx,[bp+0Ch] */
    DX = rw(pSS, BP + 0xC);
L00D3:
    /* 00D3  mov     word ptr sp_inf_tab[bx+5],dx */
    ww(pDS, BX + 0xD, DX);
L00D7:
    /* 00D7  mov     dx,[bp+0Eh] */
    DX = rw(pSS, BP + 0xE);
L00DA:
    /* 00DA  mov     byte ptr sp_inf_tab[bx+7],dl */
    wb(pDS, BX + 0xF, DL);
L00DE:
    /* 00DE  call    sp_changed */
    if ((c = asm_call(ASM_JMP(0x0000, 0x0229), 0x00E1)) != 0) return c;
L00E1:
    /* 00E1  xor     ax,ax */
    AX = logic16((uint16_t)(AX ^ AX));
L00E3: /* L00E3 */
    /* 00E3  pop     di */
    DI = pop16();
L00E4:
    /* 00E4  pop     si */
    SI = pop16();
L00E5:
    /* 00E5  pop     es */
    SET_ES(pop16());
L00E6:
    /* 00E6  pop     ds */
    SET_DS(pop16());
L00E7:
    /* 00E7  mov     sp,bp */
    SP = BP;
L00E9:
    /* 00E9  pop     bp */
    BP = pop16();
L00EA:
    /* 00EA  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* draw_sprite(int n, int pic) */
L00EB: /* _draw_sprite */
    /* 00EB  push    bp */
    push16(BP);
L00EC:
    /* 00EC  mov     bp,sp */
    BP = SP;
L00EE:
    /* 00EE  push    ds */
    push16(asm_ds);
L00EF:
    /* 00EF  push    es */
    push16(asm_es);
L00F0:
    /* 00F0  push    si */
    push16(SI);
L00F1:
    /* 00F1  push    di */
    push16(DI);
L00F2:
    /* 00F2  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L00F5:
    /* 00F5  pop     ds */
    SET_DS(pop16());
L00F6:
    /* 00F6  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L00F9:
    /* 00F9  pop     es */
    SET_ES(pop16());
L00FA:
    /* 00FA  mov     ax,[bp+6] */
    AX = rw(pSS, BP + 0x6);
L00FD:
    /* 00FD  cmp     ax,40h */
    sub16(AX, 0x40, 0);
L0100:
    /* 0100  ja      short L0118 */
    if (!CF && !ZF) goto L0118;
L0102:
    /* 0102  mov     bx,ax */
    BX = AX;
L0104:
    /* 0104  shl     bx,4 */
    BX = (uint16_t)(BX << 4);
L0107:
    /* 0107  mov     dx,[bp+8] */
    DX = rw(pSS, BP + 0x8);
L010A:
    /* 010A  mov     word ptr sp_inf_tab[bx+0Bh],dx */
    ww(pDS, BX + 0x13, DX);
L010E:
    /* 010E  or      word ptr sp_inf_tab[bx],2 */
    ww(pDS, BX + 0x8, (uint16_t)(rw(pDS, BX + 0x8) | 0x2));
L0113:
    /* 0113  call    sp_changed */
    if ((c = asm_call(ASM_JMP(0x0000, 0x0229), 0x0116)) != 0) return c;
L0116:
    /* 0116  xor     ax,ax */
    AX = logic16((uint16_t)(AX ^ AX));
L0118: /* L0118 */
    /* 0118  pop     di */
    DI = pop16();
L0119:
    /* 0119  pop     si */
    SI = pop16();
L011A:
    /* 011A  pop     es */
    SET_ES(pop16());
L011B:
    /* 011B  pop     ds */
    SET_DS(pop16());
L011C:
    /* 011C  mov     sp,bp */
    SP = BP;
L011E:
    /* 011E  pop     bp */
    BP = pop16();
L011F:
    /* 011F  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* draw_mask(int n, int pic): draw_sprite, drawn transparent */
L0120: /* _draw_mask */
    /* 0120  push    bp */
    push16(BP);
L0121:
    /* 0121  mov     bp,sp */
    BP = SP;
L0123:
    /* 0123  push    ds */
    push16(asm_ds);
L0124:
    /* 0124  push    es */
    push16(asm_es);
L0125:
    /* 0125  push    si */
    push16(SI);
L0126:
    /* 0126  push    di */
    push16(DI);
L0127:
    /* 0127  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L012A:
    /* 012A  pop     ds */
    SET_DS(pop16());
L012B:
    /* 012B  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L012E:
    /* 012E  pop     es */
    SET_ES(pop16());
L012F:
    /* 012F  mov     ax,[bp+6] */
    AX = rw(pSS, BP + 0x6);
L0132:
    /* 0132  cmp     ax,40h */
    sub16(AX, 0x40, 0);
L0135:
    /* 0135  ja      short L014D */
    if (!CF && !ZF) goto L014D;
L0137:
    /* 0137  mov     bx,ax */
    BX = AX;
L0139:
    /* 0139  shl     bx,4 */
    BX = (uint16_t)(BX << 4);
L013C:
    /* 013C  mov     dx,[bp+8] */
    DX = rw(pSS, BP + 0x8);
L013F:
    /* 013F  mov     word ptr sp_inf_tab[bx+0Bh],dx */
    ww(pDS, BX + 0x13, DX);
L0143:
    /* 0143  or      word ptr sp_inf_tab[bx],12h */
    ww(pDS, BX + 0x8, (uint16_t)(rw(pDS, BX + 0x8) | 0x12));
L0148:
    /* 0148  call    sp_changed */
    if ((c = asm_call(ASM_JMP(0x0000, 0x0229), 0x014B)) != 0) return c;
L014B:
    /* 014B  xor     ax,ax */
    AX = logic16((uint16_t)(AX ^ AX));
L014D: /* L014D */
    /* 014D  pop     di */
    DI = pop16();
L014E:
    /* 014E  pop     si */
    SI = pop16();
L014F:
    /* 014F  pop     es */
    SET_ES(pop16());
L0150:
    /* 0150  pop     ds */
    SET_DS(pop16());
L0151:
    /* 0151  mov     sp,bp */
    SP = BP;
L0153:
    /* 0153  pop     bp */
    BP = pop16();
L0154:
    /* 0154  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* erase_sprite(int n): hide it if it is visible */
L0155: /* _erase_sprite */
    /* 0155  push    bp */
    push16(BP);
L0156:
    /* 0156  mov     bp,sp */
    BP = SP;
L0158:
    /* 0158  push    ds */
    push16(asm_ds);
L0159:
    /* 0159  push    es */
    push16(asm_es);
L015A:
    /* 015A  push    si */
    push16(SI);
L015B:
    /* 015B  push    di */
    push16(DI);
L015C:
    /* 015C  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L015F:
    /* 015F  pop     ds */
    SET_DS(pop16());
L0160:
    /* 0160  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L0163:
    /* 0163  pop     es */
    SET_ES(pop16());
L0164:
    /* 0164  mov     ax,[bp+6] */
    AX = rw(pSS, BP + 0x6);
L0167:
    /* 0167  cmp     ax,40h */
    sub16(AX, 0x40, 0);
L016A:
    /* 016A  ja      short L0183 */
    if (!CF && !ZF) goto L0183;
L016C:
    /* 016C  mov     bx,ax */
    BX = AX;
L016E:
    /* 016E  shl     bx,4 */
    BX = (uint16_t)(BX << 4);
L0171:
    /* 0171  test    word ptr sp_inf_tab[bx],2 */
    logic16((uint16_t)(rw(pDS, BX + 0x8) & 0x2));
L0177:
    /* 0177  je      short L0181 */
    if (ZF) goto L0181;
L0179:
    /* 0179  and     word ptr sp_inf_tab[bx],0FFFDh */
    ww(pDS, BX + 0x8, (uint16_t)(rw(pDS, BX + 0x8) & 0xFFFD));
L017E:
    /* 017E  call    sp_changed */
    if ((c = asm_call(ASM_JMP(0x0000, 0x0229), 0x0181)) != 0) return c;
L0181: /* L0181 */
    /* 0181  xor     ax,ax */
    AX = logic16((uint16_t)(AX ^ AX));
L0183: /* L0183 */
    /* 0183  pop     di */
    DI = pop16();
L0184:
    /* 0184  pop     si */
    SI = pop16();
L0185:
    /* 0185  pop     es */
    SET_ES(pop16());
L0186:
    /* 0186  pop     ds */
    SET_DS(pop16());
L0187:
    /* 0187  mov     sp,bp */
    SP = BP;
L0189:
    /* 0189  pop     bp */
    BP = pop16();
L018A:
    /* 018A  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* move_sprite(int n, int x, int y) */
L018B: /* _move_sprite */
    /* 018B  push    bp */
    push16(BP);
L018C:
    /* 018C  mov     bp,sp */
    BP = SP;
L018E:
    /* 018E  push    ds */
    push16(asm_ds);
L018F:
    /* 018F  push    es */
    push16(asm_es);
L0190:
    /* 0190  push    si */
    push16(SI);
L0191:
    /* 0191  push    di */
    push16(DI);
L0192:
    /* 0192  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L0195:
    /* 0195  pop     ds */
    SET_DS(pop16());
L0196:
    /* 0196  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L0199:
    /* 0199  pop     es */
    SET_ES(pop16());
L019A:
    /* 019A  mov     ax,[bp+6] */
    AX = rw(pSS, BP + 0x6);
L019D:
    /* 019D  cmp     ax,40h */
    sub16(AX, 0x40, 0);
L01A0:
    /* 01A0  ja      short L01BA */
    if (!CF && !ZF) goto L01BA;
L01A2:
    /* 01A2  mov     bx,ax */
    BX = AX;
L01A4:
    /* 01A4  shl     bx,4 */
    BX = (uint16_t)(BX << 4);
L01A7:
    /* 01A7  mov     dx,[bp+8] */
    DX = rw(pSS, BP + 0x8);
L01AA:
    /* 01AA  mov     word ptr sp_inf_tab[bx+2],dx */
    ww(pDS, BX + 0xA, DX);
L01AE:
    /* 01AE  mov     dx,[bp+0Ah] */
    DX = rw(pSS, BP + 0xA);
L01B1:
    /* 01B1  mov     byte ptr sp_inf_tab[bx+4],dl */
    wb(pDS, BX + 0xC, DL);
L01B5:
    /* 01B5  call    sp_changed */
    if ((c = asm_call(ASM_JMP(0x0000, 0x0229), 0x01B8)) != 0) return c;
L01B8:
    /* 01B8  xor     ax,ax */
    AX = logic16((uint16_t)(AX ^ AX));
L01BA: /* L01BA */
    /* 01BA  pop     di */
    DI = pop16();
L01BB:
    /* 01BB  pop     si */
    SI = pop16();
L01BC:
    /* 01BC  pop     es */
    SET_ES(pop16());
L01BD:
    /* 01BD  pop     ds */
    SET_DS(pop16());
L01BE:
    /* 01BE  mov     sp,bp */
    SP = BP;
L01C0:
    /* 01C0  pop     bp */
    BP = pop16();
L01C1:
    /* 01C1  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* resize_sprite(int n, int w, int h) */
L01C2: /* _resize_sprite */
    /* 01C2  push    bp */
    push16(BP);
L01C3:
    /* 01C3  mov     bp,sp */
    BP = SP;
L01C5:
    /* 01C5  push    ds */
    push16(asm_ds);
L01C6:
    /* 01C6  push    es */
    push16(asm_es);
L01C7:
    /* 01C7  push    si */
    push16(SI);
L01C8:
    /* 01C8  push    di */
    push16(DI);
L01C9:
    /* 01C9  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L01CC:
    /* 01CC  pop     ds */
    SET_DS(pop16());
L01CD:
    /* 01CD  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L01D0:
    /* 01D0  pop     es */
    SET_ES(pop16());
L01D1:
    /* 01D1  mov     ax,[bp+6] */
    AX = rw(pSS, BP + 0x6);
L01D4:
    /* 01D4  cmp     ax,40h */
    sub16(AX, 0x40, 0);
L01D7:
    /* 01D7  ja      short L01F1 */
    if (!CF && !ZF) goto L01F1;
L01D9:
    /* 01D9  mov     bx,ax */
    BX = AX;
L01DB:
    /* 01DB  shl     bx,4 */
    BX = (uint16_t)(BX << 4);
L01DE:
    /* 01DE  mov     dx,[bp+8] */
    DX = rw(pSS, BP + 0x8);
L01E1:
    /* 01E1  mov     word ptr sp_inf_tab[bx+5],dx */
    ww(pDS, BX + 0xD, DX);
L01E5:
    /* 01E5  mov     dx,[bp+0Ah] */
    DX = rw(pSS, BP + 0xA);
L01E8:
    /* 01E8  mov     byte ptr sp_inf_tab[bx+7],dl */
    wb(pDS, BX + 0xF, DL);
L01EC:
    /* 01EC  call    sp_changed */
    if ((c = asm_call(ASM_JMP(0x0000, 0x0229), 0x01EF)) != 0) return c;
L01EF:
    /* 01EF  xor     ax,ax */
    AX = logic16((uint16_t)(AX ^ AX));
L01F1: /* L01F1 */
    /* 01F1  pop     di */
    DI = pop16();
L01F2:
    /* 01F2  pop     si */
    SI = pop16();
L01F3:
    /* 01F3  pop     es */
    SET_ES(pop16());
L01F4:
    /* 01F4  pop     ds */
    SET_DS(pop16());
L01F5:
    /* 01F5  mov     sp,bp */
    SP = BP;
L01F7:
    /* 01F7  pop     bp */
    BP = pop16();
L01F8:
    /* 01F8  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* set_yoff(int n, int yoff) */
L01F9: /* _set_yoff */
    /* 01F9  push    bp */
    push16(BP);
L01FA:
    /* 01FA  mov     bp,sp */
    BP = SP;
L01FC:
    /* 01FC  push    ds */
    push16(asm_ds);
L01FD:
    /* 01FD  push    es */
    push16(asm_es);
L01FE:
    /* 01FE  push    si */
    push16(SI);
L01FF:
    /* 01FF  push    di */
    push16(DI);
L0200:
    /* 0200  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L0203:
    /* 0203  pop     ds */
    SET_DS(pop16());
L0204:
    /* 0204  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L0207:
    /* 0207  pop     es */
    SET_ES(pop16());
L0208:
    /* 0208  mov     ax,[bp+6] */
    AX = rw(pSS, BP + 0x6);
L020B:
    /* 020B  cmp     ax,40h */
    sub16(AX, 0x40, 0);
L020E:
    /* 020E  ja      short L0221 */
    if (!CF && !ZF) goto L0221;
L0210:
    /* 0210  mov     bx,ax */
    BX = AX;
L0212:
    /* 0212  shl     bx,4 */
    BX = (uint16_t)(BX << 4);
L0215:
    /* 0215  mov     dx,[bp+8] */
    DX = rw(pSS, BP + 0x8);
L0218:
    /* 0218  mov     byte ptr sp_inf_tab[bx+8],dl */
    wb(pDS, BX + 0x10, DL);
L021C:
    /* 021C  call    sp_changed */
    if ((c = asm_call(ASM_JMP(0x0000, 0x0229), 0x021F)) != 0) return c;
L021F:
    /* 021F  xor     ax,ax */
    AX = logic16((uint16_t)(AX ^ AX));
L0221: /* L0221 */
    /* 0221  pop     di */
    DI = pop16();
L0222:
    /* 0222  pop     si */
    SI = pop16();
L0223:
    /* 0223  pop     es */
    SET_ES(pop16());
L0224:
    /* 0224  pop     ds */
    SET_DS(pop16());
L0225:
    /* 0225  mov     sp,bp */
    SP = BP;
L0227:
    /* 0227  pop     bp */
    BP = pop16();
L0228:
    /* 0228  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* sp_changed: AX the sprite. Sets sp_dirty, adds the sprite to its layer's redraw list,
       adds it to or removes it from its layer's on-screen list as it is visible or not, then
       adds every visible sprite of a higher layer that overlaps it to that layer's redraw list.
       DS and ES address the sprite data. */
L0229: /* sp_changed */
    /* 0229  mov     cs:sp_dirty,1 */
    ww(CODE000, 0x0, 0x1);
L0230:
    /* 0230  mov     si,ax */
    SI = AX;
L0232:
    /* 0232  shl     si,4 */
    SI = (uint16_t)(SI << 4);
L0235:
    /* 0235  mov     bx,word ptr sp_inf_tab[si+9] */
    BX = rw(pDS, SI + 0x11);
L0239:
    /* 0239  shl     bx,6 */
    BX = (uint16_t)(BX << 6);
L023C:
    /* 023C  mov     di,offset sp_inf_end */
    DI = 0x408;
L023F:
    /* 023F  add     di,bx */
    DI = (uint16_t)(DI + BX);
L0241:
    /* 0241  mov     cx,word ptr sp_inf_end[bx] */
    CX = rw(pDS, BX + 0x408);
L0245:
    /* 0245  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L0247:
    /* 0247  add     di,2 */
    DI = add16(DI, 0x2, 0);
L024A:
    /* 024A  repne   scasw */
    while (CX) { sub16(AX, rw(pES, DI), 0); DI = (uint16_t)(DI + STEP(2)); CX--; if (ZF) break; }
L024C:
    /* 024C  je      short L0255 */
    if (ZF) goto L0255;
L024E:
    /* 024E  add     word ptr sp_inf_end[bx],2 */
    ww(pDS, BX + 0x408, (uint16_t)(rw(pDS, BX + 0x408) + 0x2));
L0253:
    /* 0253  mov     [di],ax */
    ww(pDS, DI, AX);
L0255: /* L0255 */
    /* 0255  mov     bx,word ptr sp_inf_tab[si+9] */
    BX = rw(pDS, SI + 0x11);
L0259:
    /* 0259  shl     bx,6 */
    BX = (uint16_t)(BX << 6);
L025C:
    /* 025C  mov     di,offset sp_map_tab */
    DI = 0x508;
L025F:
    /* 025F  add     di,bx */
    DI = (uint16_t)(DI + BX);
L0261:
    /* 0261  mov     cx,word ptr sp_map_tab[bx] */
    CX = rw(pDS, BX + 0x508);
L0265:
    /* 0265  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L0267:
    /* 0267  add     di,2 */
    DI = add16(DI, 0x2, 0);
L026A:
    /* 026A  repne   scasw */
    while (CX) { sub16(AX, rw(pES, DI), 0); DI = (uint16_t)(DI + STEP(2)); CX--; if (ZF) break; }
L026C:
    /* 026C  je      short L0281 */
    if (ZF) goto L0281;
L026E:
    /* 026E  mov     cx,word ptr sp_inf_tab[si] */
    CX = rw(pDS, SI + 0x8);
L0272:
    /* 0272  test    cx,2 */
    logic16((uint16_t)(CX & 0x2));
L0276:
    /* 0276  je      short L029F */
    if (ZF) goto L029F;
L0278:
    /* 0278  add     word ptr sp_map_tab[bx],2 */
    ww(pDS, BX + 0x508, (uint16_t)(rw(pDS, BX + 0x508) + 0x2));
L027D:
    /* 027D  mov     [di],ax */
    ww(pDS, DI, AX);
L027F:
    /* 027F  jmp     short L029F */
    goto L029F;
L0281: /* L0281 */
    /* 0281  mov     cx,word ptr sp_inf_tab[si] */
    CX = rw(pDS, SI + 0x8);
L0285:
    /* 0285  test    cx,2 */
    logic16((uint16_t)(CX & 0x2));
L0289:
    /* 0289  jne     short L029F */
    if (!ZF) goto L029F;
L028B:
    /* 028B  sub     di,2 */
    DI = (uint16_t)(DI - 0x2);
L028E:
    /* 028E  push    si */
    push16(SI);
L028F:
    /* 028F  mov     si,word ptr sp_map_tab[bx] */
    SI = rw(pDS, BX + 0x508);
L0293:
    /* 0293  mov     cx,word ptr sp_map_tab[bx+si] */
    CX = rw(pDS, BX + SI + 0x508);
L0297:
    /* 0297  mov     [di],cx */
    ww(pDS, DI, CX);
L0299:
    /* 0299  sub     word ptr sp_map_tab[bx],2 */
    ww(pDS, BX + 0x508, (uint16_t)(rw(pDS, BX + 0x508) - 0x2));
L029E:
    /* 029E  pop     si */
    SI = pop16();
L029F: /* L029F */
    /* 029F  mov     bx,word ptr sp_inf_tab[si+9] */
    BX = rw(pDS, SI + 0x11);
L02A3:
    /* 02A3  shl     bx,6 */
    BX = (uint16_t)(BX << 6);
L02A6: /* L02A6 */
    /* 02A6  add     bx,40h */
    BX = (uint16_t)(BX + 0x40);
L02A9:
    /* 02A9  mov     word ptr sp_list,bx */
    ww(pDS, 0x608, BX);
L02AD:
    /* 02AD  cmp     bx,100h */
    sub16(BX, 0x100, 0);
L02B1:
    /* 02B1  jae     short L032F */
    if (!CF) goto L032F;
L02B3:
    /* 02B3  mov     cx,word ptr sp_map_tab[bx] */
    CX = rw(pDS, BX + 0x508);
L02B7:
    /* 02B7  shr     cx,1 */
    CX = shr16(CX, 1);
L02B9:
    /* 02B9  je      L02A6 */
    if (ZF) goto L02A6;
L02BB:
    /* 02BB  mov     word ptr sp_left,cx */
    ww(pDS, 0x60A, CX);
L02BF: /* L02BF */
    /* 02BF  add     bx,2 */
    BX = (uint16_t)(BX + 0x2);
L02C2:
    /* 02C2  mov     di,word ptr sp_map_tab[bx] */
    DI = rw(pDS, BX + 0x508);
L02C6:
    /* 02C6  shl     di,4 */
    DI = (uint16_t)(DI << 4);
L02C9:
    /* 02C9  mov     ax,word ptr sp_inf_tab[di+2] */
    AX = rw(pDS, DI + 0xA);
L02CD:
    /* 02CD  mov     dx,word ptr sp_inf_tab[si+2] */
    DX = rw(pDS, SI + 0xA);
L02D1:
    /* 02D1  mov     cx,dx */
    CX = DX;
L02D3:
    /* 02D3  add     cx,word ptr sp_inf_tab[si+5] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0xD));
L02D7:
    /* 02D7  cmp     ax,cx */
    sub16(AX, CX, 0);
L02D9:
    /* 02D9  jg      short L0322 */
    if (!ZF && SF == OF) goto L0322;
L02DB:
    /* 02DB  add     ax,word ptr sp_inf_tab[di+5] */
    AX = (uint16_t)(AX + rw(pDS, DI + 0xD));
L02DF:
    /* 02DF  cmp     ax,dx */
    sub16(AX, DX, 0);
L02E1:
    /* 02E1  jl      short L0322 */
    if (SF != OF) goto L0322;
L02E3:
    /* 02E3  mov     al,byte ptr sp_inf_tab[di+4] */
    AL = rb(pDS, DI + 0xC);
L02E7:
    /* 02E7  mov     dl,byte ptr sp_inf_tab[si+4] */
    DL = rb(pDS, SI + 0xC);
L02EB:
    /* 02EB  mov     cl,dl */
    CL = DL;
L02ED:
    /* 02ED  sub     cl,byte ptr sp_inf_tab[si+7] */
    CL = (uint8_t)(CL - rb(pDS, SI + 0xF));
L02F1:
    /* 02F1  cmp     cl,al */
    sub8(CL, AL, 0);
L02F3:
    /* 02F3  jg      short L0322 */
    if (!ZF && SF == OF) goto L0322;
L02F5:
    /* 02F5  sub     al,byte ptr sp_inf_tab[di+7] */
    AL = (uint8_t)(AL - rb(pDS, DI + 0xF));
L02F9:
    /* 02F9  cmp     dl,al */
    sub8(DL, AL, 0);
L02FB:
    /* 02FB  jl      short L0322 */
    if (SF != OF) goto L0322;
L02FD:
    /* 02FD  mov     ax,di */
    AX = DI;
L02FF:
    /* 02FF  shr     ax,4 */
    AX = (uint16_t)(AX >> 4);
L0302:
    /* 0302  mov     bx,word ptr sp_inf_tab[di+9] */
    BX = rw(pDS, DI + 0x11);
L0306:
    /* 0306  shl     bx,6 */
    BX = (uint16_t)(BX << 6);
L0309:
    /* 0309  mov     di,offset sp_inf_end */
    DI = 0x408;
L030C:
    /* 030C  add     di,bx */
    DI = (uint16_t)(DI + BX);
L030E:
    /* 030E  mov     cx,word ptr sp_inf_end[bx] */
    CX = rw(pDS, BX + 0x408);
L0312:
    /* 0312  shr     cx,1 */
    CX = (uint16_t)(CX >> 1);
L0314:
    /* 0314  add     di,2 */
    DI = add16(DI, 0x2, 0);
L0317:
    /* 0317  repne   scasw */
    while (CX) { sub16(AX, rw(pES, DI), 0); DI = (uint16_t)(DI + STEP(2)); CX--; if (ZF) break; }
L0319:
    /* 0319  je      short L0322 */
    if (ZF) goto L0322;
L031B:
    /* 031B  add     word ptr sp_inf_end[bx],2 */
    ww(pDS, BX + 0x408, (uint16_t)(rw(pDS, BX + 0x408) + 0x2));
L0320:
    /* 0320  mov     [di],ax */
    ww(pDS, DI, AX);
L0322: /* L0322 */
    /* 0322  dec     word ptr sp_left */
    ww(pDS, 0x60A, dec16(rw(pDS, 0x60A)));
L0326:
    /* 0326  jne     L02BF */
    if (!ZF) goto L02BF;
L0328:
    /* 0328  mov     bx,word ptr sp_list */
    BX = rw(pDS, 0x608);
L032C:
    /* 032C  jmp     L02A6 */
    goto L02A6;
L032F: /* L032F */
    /* 032F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* update_sprites(void): if anything changed, with the mouse hidden: from the top layer
       down, restore what the sprites to be redrawn had covered (and free the blocks of freed
       sprites); then from the bottom layer up save what each visible one will cover and draw
       it (mask_to_screen, with byte +8 as its last argument, when that byte is set, else
       pic_to_screen); empty the redraw lists and clear sp_dirty. */
L0330: /* _update_sprites */
    /* 0330  push    bp */
    push16(BP);
L0331:
    /* 0331  mov     bp,sp */
    BP = SP;
L0333:
    /* 0333  push    ds */
    push16(asm_ds);
L0334:
    /* 0334  push    es */
    push16(asm_es);
L0335:
    /* 0335  push    si */
    push16(SI);
L0336:
    /* 0336  push    di */
    push16(DI);
L0337:
    /* 0337  cmp     cs:sp_dirty,0 */
    sub16(rw(CODE000, 0x0), 0x0, 0);
L033D:
    /* 033D  jne     short L0342 */
    if (!ZF) goto L0342;
L033F:
    /* 033F  jmp     L04DB */
    goto L04DB;
L0342: /* L0342 */
    /* 0342  mov     ax,DGROUP */
    /* by hand: mov ax,DGROUP: the C stack's segment, the port's stand-in for DGROUP (x86/entry.c) */
    AX = port_dgroup_seg();
L0345:
    /* 0345  mov     ds,ax */
    SET_DS(AX);
L0347:
    /* 0347  mov     es,ax */
    SET_ES(AX);
L0349:
    /* 0349  call    _mouse_hide */
    if ((c = asm_callf(ASM_JMP(0x1ADC, 0x00D7), 0x0000 + PORT_LOAD_SEG, 0x034E)) != 0) return c;
L034E:
    /* 034E  mov     ax,seg sp_inf_tab */
    AX = (uint16_t)(0x5477 + PORT_LOAD_SEG);
L0351:
    /* 0351  mov     ds,ax */
    SET_DS(AX);
L0353:
    /* 0353  mov     bx,offset sp_map_tab */
    BX = 0x508;
L0356: /* L0356 */
    /* 0356  sub     bx,40h */
    BX = (uint16_t)(BX - 0x40);
L0359: /* L0359 */
    /* 0359  mov     cx,[bx] */
    CX = rw(pDS, BX);
L035B:
    /* 035B  shr     cx,1 */
    CX = shr16(CX, 1);
L035D:
    /* 035D  je      short L03A6 */
    if (ZF) goto L03A6;
L035F:
    /* 035F  mov     si,2 */
    SI = 0x2;
L0362: /* L0362 */
    /* 0362  mov     di,[bx+si] */
    DI = rw(pDS, BX + SI);
L0364:
    /* 0364  shl     di,4 */
    DI = (uint16_t)(DI << 4);
L0367:
    /* 0367  test    word ptr sp_inf_tab[di],4 */
    logic16((uint16_t)(rw(pDS, DI + 0x8) & 0x4));
L036D:
    /* 036D  je      short L0376 */
    if (ZF) goto L0376;
L036F:
    /* 036F  and     word ptr sp_inf_tab[di],0FFFBh */
    ww(pDS, DI + 0x8, (uint16_t)(rw(pDS, DI + 0x8) & 0xFFFB));
L0374:
    /* 0374  jmp     short L0383 */
    goto L0383;
L0376: /* L0376 */
    /* 0376  push    bx */
    push16(BX);
L0377:
    /* 0377  push    cx */
    push16(CX);
L0378:
    /* 0378  mov     ax,word ptr sp_inf_tab[di+0Dh] */
    AX = rw(pDS, DI + 0x15);
L037C:
    /* 037C  call    restore_rect */
    if ((c = asm_callf(ASM_JMP(0x004E, 0x0268), 0x0000 + PORT_LOAD_SEG, 0x0381)) != 0) return c;
L0381:
    /* 0381  pop     cx */
    CX = pop16();
L0382:
    /* 0382  pop     bx */
    BX = pop16();
L0383: /* L0383 */
    /* 0383  test    word ptr sp_inf_tab[di],8 */
    logic16((uint16_t)(rw(pDS, DI + 0x8) & 0x8));
L0389:
    /* 0389  je      short L03A1 */
    if (ZF) goto L03A1;
L038B:
    /* 038B  and     word ptr sp_inf_tab[di],0FFFEh */
    ww(pDS, DI + 0x8, (uint16_t)(rw(pDS, DI + 0x8) & 0xFFFE));
L0390:
    /* 0390  mov     ax,word ptr sp_inf_tab[di+0Dh] */
    AX = rw(pDS, DI + 0x15);
L0394:
    /* 0394  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L0396:
    /* 0396  je      short L03A1 */
    if (ZF) goto L03A1;
L0398:
    /* 0398  push    bx */
    push16(BX);
L0399:
    /* 0399  push    cx */
    push16(CX);
L039A:
    /* 039A  call    far ptr _vfree+0Dh */
    if ((c = asm_callf(ASM_JMP(0x004E, 0x0112), 0x0000 + PORT_LOAD_SEG, 0x039F)) != 0) return c;
L039F:
    /* 039F  pop     cx */
    CX = pop16();
L03A0:
    /* 03A0  pop     bx */
    BX = pop16();
L03A1: /* L03A1 */
    /* 03A1  add     si,2 */
    SI = (uint16_t)(SI + 0x2);
L03A4:
    /* 03A4  loop    L0362 */
    if (--CX) goto L0362;
L03A6: /* L03A6 */
    /* 03A6  sub     bx,40h */
    BX = (uint16_t)(BX - 0x40);
L03A9:
    /* 03A9  cmp     bx,offset sp_inf_end */
    sub16(BX, 0x408, 0);
L03AD:
    /* 03AD  ja      L0359 */
    if (!CF && !ZF) goto L0359;
L03AF:
    /* 03AF  mov     bx,offset sp_inf_end */
    BX = 0x408;
L03B2:
    /* 03B2  jmp     short L03FC */
    goto L03FC;
L03B4: /* L03B4 */
    /* 03B4  mov     cx,[bx] */
    CX = rw(pDS, BX);
L03B6:
    /* 03B6  shr     cx,1 */
    CX = shr16(CX, 1);
L03B8:
    /* 03B8  jne     short L03BD */
    if (!ZF) goto L03BD;
L03BA:
    /* 03BA  jmp     L04C3 */
    goto L04C3;
L03BD: /* L03BD */
    /* 03BD  mov     si,2 */
    SI = 0x2;
L03C0: /* L03C0 */
    /* 03C0  mov     di,[bx+si] */
    DI = rw(pDS, BX + SI);
L03C2:
    /* 03C2  shl     di,4 */
    DI = (uint16_t)(DI << 4);
L03C5:
    /* 03C5  test    word ptr sp_inf_tab[di],2 */
    logic16((uint16_t)(rw(pDS, DI + 0x8) & 0x2));
L03CB:
    /* 03CB  jne     short L03D4 */
    if (!ZF) goto L03D4;
L03CD:
    /* 03CD  or      word ptr sp_inf_tab[di],4 */
    ww(pDS, DI + 0x8, (uint16_t)(rw(pDS, DI + 0x8) | 0x4));
L03D2:
    /* 03D2  jmp     short L03F7 */
    goto L03F7;
L03D4: /* L03D4 */
    /* 03D4  push    bx */
    push16(BX);
L03D5:
    /* 03D5  push    cx */
    push16(CX);
L03D6:
    /* 03D6  push    si */
    push16(SI);
L03D7:
    /* 03D7  mov     si,word ptr sp_inf_tab[di+0Dh] */
    SI = rw(pDS, DI + 0x15);
L03DB:
    /* 03DB  mov     ax,word ptr sp_inf_tab[di+2] */
    AX = rw(pDS, DI + 0xA);
L03DF:
    /* 03DF  xor     bh,bh */
    BH = (uint8_t)(BH ^ BH);
L03E1:
    /* 03E1  mov     bl,byte ptr sp_inf_tab[di+4] */
    BL = rb(pDS, DI + 0xC);
L03E5:
    /* 03E5  mov     cx,word ptr sp_inf_tab[di+5] */
    CX = rw(pDS, DI + 0xD);
L03E9:
    /* 03E9  xor     dh,dh */
    DH = (uint8_t)(DH ^ DH);
L03EB:
    /* 03EB  mov     dl,byte ptr sp_inf_tab[di+7] */
    DL = rb(pDS, DI + 0xF);
L03EF:
    /* 03EF  call    save_rect */
    if ((c = asm_callf(ASM_JMP(0x004E, 0x01EF), 0x0000 + PORT_LOAD_SEG, 0x03F4)) != 0) return c;
L03F4:
    /* 03F4  pop     si */
    SI = pop16();
L03F5:
    /* 03F5  pop     cx */
    CX = pop16();
L03F6:
    /* 03F6  pop     bx */
    BX = pop16();
L03F7: /* L03F7 */
    /* 03F7  add     si,2 */
    SI = (uint16_t)(SI + 0x2);
L03FA:
    /* 03FA  loop    L03C0 */
    if (--CX) goto L03C0;
L03FC: /* L03FC */
    /* 03FC  mov     cx,[bx] */
    CX = rw(pDS, BX);
L03FE:
    /* 03FE  mov     word ptr [bx],0 */
    ww(pDS, BX, 0x0);
L0402:
    /* 0402  shr     cx,1 */
    CX = shr16(CX, 1);
L0404:
    /* 0404  jne     short L0409 */
    if (!ZF) goto L0409;
L0406:
    /* 0406  jmp     L04C3 */
    goto L04C3;
L0409: /* L0409 */
    /* 0409  mov     si,2 */
    SI = 0x2;
L040C: /* L040C */
    /* 040C  mov     di,[bx+si] */
    DI = rw(pDS, BX + SI);
L040E:
    /* 040E  shl     di,4 */
    DI = (uint16_t)(DI << 4);
L0411:
    /* 0411  test    word ptr sp_inf_tab[di],2 */
    logic16((uint16_t)(rw(pDS, DI + 0x8) & 0x2));
L0417:
    /* 0417  jne     short L041C */
    if (!ZF) goto L041C;
L0419:
    /* 0419  jmp     L04B9 */
    goto L04B9;
L041C: /* L041C */
    /* 041C  push    bx */
    push16(BX);
L041D:
    /* 041D  push    cx */
    push16(CX);
L041E:
    /* 041E  mov     al,byte ptr sp_inf_tab[di+8] */
    AL = rb(pDS, DI + 0x10);
L0422:
    /* 0422  or      al,al */
    AL = logic8((uint8_t)(AL | AL));
L0424:
    /* 0424  je      short L046C */
    if (ZF) goto L046C;
L0426:
    /* 0426  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L0428:
    /* 0428  push    ax */
    push16(AX);
L0429:
    /* 0429  push    word ptr sp_inf_tab[di+5] */
    push16(rw(pDS, DI + 0xD));
L042D:
    /* 042D  mov     al,byte ptr sp_inf_tab[di+7] */
    AL = rb(pDS, DI + 0xF);
L0431:
    /* 0431  push    ax */
    push16(AX);
L0432:
    /* 0432  mov     al,byte ptr sp_inf_tab[di+4] */
    AL = rb(pDS, DI + 0xC);
L0436:
    /* 0436  push    ax */
    push16(AX);
L0437:
    /* 0437  push    word ptr sp_inf_tab[di+2] */
    push16(rw(pDS, DI + 0xA));
L043B:
    /* 043B  push    word ptr sp_inf_tab[di+0Bh] */
    push16(rw(pDS, DI + 0x13));
L043F:
    /* 043F  test    word ptr sp_inf_tab[di],10h */
    logic16((uint16_t)(rw(pDS, DI + 0x8) & 0x10));
L0445:
    /* 0445  mov     ax,DGROUP */
    /* by hand: mov ax,DGROUP, as at 0342 */
    AX = port_dgroup_seg();
L0448:
    /* 0448  mov     ds,ax */
    SET_DS(AX);
L044A:
    /* 044A  je      short L0457 */
    if (ZF) goto L0457;
L044C:
    /* 044C  mov     ax,seg _Transparency */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L044F:
    /* 044F  mov     es,ax */
    SET_ES(AX);
L0451:
    /* 0451  mov     es:_Transparency,1 */
    wb(pES, 0xDC7, 0x1);
L0457: /* L0457 */
    /* 0457  call    _mask_to_screen */
    if ((c = asm_callf(ASM_JMP(0x1A31, 0x034D), 0x0000 + PORT_LOAD_SEG, 0x045C)) != 0) return c;
L045C:
    /* 045C  add     sp,0Ch */
    SP = (uint16_t)(SP + 0xC);
L045F:
    /* 045F  mov     ax,seg _Transparency */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L0462:
    /* 0462  mov     es,ax */
    SET_ES(AX);
L0464:
    /* 0464  mov     es:_Transparency,0 */
    wb(pES, 0xDC7, 0x0);
L046A:
    /* 046A  jmp     short L04AF */
    goto L04AF;
L046C: /* L046C */
    /* 046C  push    word ptr sp_inf_tab[di+5] */
    push16(rw(pDS, DI + 0xD));
L0470:
    /* 0470  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L0472:
    /* 0472  mov     al,byte ptr sp_inf_tab[di+7] */
    AL = rb(pDS, DI + 0xF);
L0476:
    /* 0476  push    ax */
    push16(AX);
L0477:
    /* 0477  mov     al,byte ptr sp_inf_tab[di+4] */
    AL = rb(pDS, DI + 0xC);
L047B:
    /* 047B  push    ax */
    push16(AX);
L047C:
    /* 047C  push    word ptr sp_inf_tab[di+2] */
    push16(rw(pDS, DI + 0xA));
L0480:
    /* 0480  push    word ptr sp_inf_tab[di+0Bh] */
    push16(rw(pDS, DI + 0x13));
L0484:
    /* 0484  test    word ptr sp_inf_tab[di],10h */
    logic16((uint16_t)(rw(pDS, DI + 0x8) & 0x10));
L048A:
    /* 048A  mov     ax,DGROUP */
    /* by hand: mov ax,DGROUP, as at 0342 */
    AX = port_dgroup_seg();
L048D:
    /* 048D  mov     ds,ax */
    SET_DS(AX);
L048F:
    /* 048F  je      short L049C */
    if (ZF) goto L049C;
L0491:
    /* 0491  mov     ax,seg _Transparency */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L0494:
    /* 0494  mov     es,ax */
    SET_ES(AX);
L0496:
    /* 0496  mov     es:_Transparency,1 */
    wb(pES, 0xDC7, 0x1);
L049C: /* L049C */
    /* 049C  call    _pic_to_screen */
    if ((c = asm_callf(ASM_JMP(0x1A31, 0x0265), 0x0000 + PORT_LOAD_SEG, 0x04A1)) != 0) return c;
L04A1:
    /* 04A1  add     sp,0Ah */
    SP = (uint16_t)(SP + 0xA);
L04A4:
    /* 04A4  mov     ax,seg _Transparency */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L04A7:
    /* 04A7  mov     es,ax */
    SET_ES(AX);
L04A9:
    /* 04A9  mov     es:_Transparency,0 */
    wb(pES, 0xDC7, 0x0);
L04AF: /* L04AF */
    /* 04AF  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L04B2:
    /* 04B2  pop     ds */
    SET_DS(pop16());
L04B3:
    /* 04B3  push    seg sp_inf_tab */
    push16((uint16_t)(0x5477 + PORT_LOAD_SEG));
L04B6:
    /* 04B6  pop     es */
    SET_ES(pop16());
L04B7:
    /* 04B7  pop     cx */
    CX = pop16();
L04B8:
    /* 04B8  pop     bx */
    BX = pop16();
L04B9: /* L04B9 */
    /* 04B9  add     si,2 */
    SI = (uint16_t)(SI + 0x2);
L04BC:
    /* 04BC  loop    L04C0 */
    if (--CX) goto L04C0;
L04BE:
    /* 04BE  jmp     short L04C3 */
    goto L04C3;
L04C0: /* L04C0 */
    /* 04C0  jmp     L040C */
    goto L040C;
L04C3: /* L04C3 */
    /* 04C3  add     bx,40h */
    BX = (uint16_t)(BX + 0x40);
L04C6:
    /* 04C6  cmp     bx,offset sp_map_tab */
    sub16(BX, 0x508, 0);
L04CA:
    /* 04CA  jae     short L04CF */
    if (!CF) goto L04CF;
L04CC:
    /* 04CC  jmp     L03B4 */
    goto L03B4;
L04CF: /* L04CF */
    /* 04CF  mov     ax,DGROUP */
    /* by hand: mov ax,DGROUP, as at 0342 */
    AX = port_dgroup_seg();
L04D2:
    /* 04D2  mov     ds,ax */
    SET_DS(AX);
L04D4:
    /* 04D4  mov     es,ax */
    SET_ES(AX);
L04D6:
    /* 04D6  call    _mouse_show */
    if ((c = asm_callf(ASM_JMP(0x1ADC, 0x00C1), 0x0000 + PORT_LOAD_SEG, 0x04DB)) != 0) return c;
L04DB: /* L04DB */
    /* 04DB  mov     cs:sp_dirty,0 */
    ww(CODE000, 0x0, 0x0);
L04E2:
    /* 04E2  pop     di */
    DI = pop16();
L04E3:
    /* 04E3  pop     si */
    SI = pop16();
L04E4:
    /* 04E4  pop     es */
    SET_ES(pop16());
L04E5:
    /* 04E5  pop     ds */
    SET_DS(pop16());
L04E6:
    /* 04E6  mov     sp,bp */
    SP = BP;
L04E8:
    /* 04E8  pop     bp */
    BP = pop16();
L04E9:
    /* 04E9  ret */
    SP = (uint16_t)(SP + 4); return ASM_RETF;
}
