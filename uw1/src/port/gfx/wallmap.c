/* wallmap.c: replaces src/gfx/WALLMAP.ASM (seg003_60, 0060..03AB of its
   segment). Written by tools/asm2c.py from the assembly and the bytes it assembles to; do not
   edit by hand: change the tool, or its OVERRIDES, and run it again. Each instruction is the
   C below its source line; the comments before a routine are the .ASM file's. */
#include "x86/asmrt.h"

#include "portgame.h"
#include "sys/enhance.h"     /* ENHANCED: perspective */
void persp_walk(int wall);               /* gfx/perspmap.c */
int persp_probe(void);
void persp_probe_next(void);
extern uint8_t persp_probe_colour;
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

uint32_t asm_mod_WALLMAP(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x0060: goto L0060;
    case 0x0064: goto L0064;
    case 0x0067: goto L0067;
    case 0x006A: goto L006A;
    case 0x006D: goto L006D;
    case 0x0070: goto L0070;
    case 0x0073: goto L0073;
    case 0x0076: goto L0076;
    case 0x0078: goto L0078;
    case 0x007B: goto L007B;
    case 0x007E: goto L007E;
    case 0x0081: goto L0081;
    case 0x0084: goto L0084;
    case 0x0087: goto L0087;
    case 0x008A: goto L008A;
    case 0x008E: goto L008E;
    case 0x0090: goto L0090;
    case 0x0093: goto L0093;
    case 0x0097: goto L0097;
    case 0x009B: goto L009B;
    case 0x009D: goto L009D;
    case 0x00A0: goto L00A0;
    case 0x00A3: goto L00A3;
    case 0x00A7: goto L00A7;
    case 0x00AB: goto L00AB;
    case 0x00AD: goto L00AD;
    case 0x00AF: goto L00AF;
    case 0x00B1: goto L00B1;
    case 0x00B5: goto L00B5;
    case 0x00B6: goto L00B6;
    case 0x00B8: goto L00B8;
    case 0x00BC: goto L00BC;
    case 0x00BE: goto L00BE;
    case 0x00C0: goto L00C0;
    case 0x00C2: goto L00C2;
    case 0x00C4: goto L00C4;
    case 0x00C5: goto L00C5;
    case 0x00C7: goto L00C7;
    case 0x00C9: goto L00C9;
    case 0x00CD: goto L00CD;
    case 0x00D2: goto L00D2;
    case 0x00D5: goto L00D5;
    case 0x00D9: goto L00D9;
    case 0x00DA: goto L00DA;
    case 0x00DC: goto L00DC;
    case 0x00E0: goto L00E0;
    case 0x00E2: goto L00E2;
    case 0x00E4: goto L00E4;
    case 0x00E6: goto L00E6;
    case 0x00E8: goto L00E8;
    case 0x00E9: goto L00E9;
    case 0x00EB: goto L00EB;
    case 0x00ED: goto L00ED;
    case 0x00F1: goto L00F1;
    case 0x00F6: goto L00F6;
    case 0x00F9: goto L00F9;
    case 0x00FD: goto L00FD;
    case 0x00FE: goto L00FE;
    case 0x0100: goto L0100;
    case 0x0104: goto L0104;
    case 0x0106: goto L0106;
    case 0x0108: goto L0108;
    case 0x010A: goto L010A;
    case 0x010C: goto L010C;
    case 0x010D: goto L010D;
    case 0x010F: goto L010F;
    case 0x0111: goto L0111;
    case 0x0115: goto L0115;
    case 0x011A: goto L011A;
    case 0x0120: goto L0120;
    case 0x0121: goto L0121;
    case 0x0125: goto L0125;
    case 0x0128: goto L0128;
    case 0x012B: goto L012B;
    case 0x012E: goto L012E;
    case 0x0131: goto L0131;
    case 0x0133: goto L0133;
    case 0x0136: goto L0136;
    case 0x0139: goto L0139;
    case 0x013C: goto L013C;
    case 0x013F: goto L013F;
    case 0x0142: goto L0142;
    case 0x0145: goto L0145;
    case 0x0149: goto L0149;
    case 0x014B: goto L014B;
    case 0x014F: goto L014F;
    case 0x0153: goto L0153;
    case 0x0157: goto L0157;
    case 0x0159: goto L0159;
    case 0x015C: goto L015C;
    case 0x0160: goto L0160;
    case 0x0164: goto L0164;
    case 0x0166: goto L0166;
    case 0x0168: goto L0168;
    case 0x016A: goto L016A;
    case 0x016E: goto L016E;
    case 0x016F: goto L016F;
    case 0x0171: goto L0171;
    case 0x0175: goto L0175;
    case 0x0177: goto L0177;
    case 0x0179: goto L0179;
    case 0x017B: goto L017B;
    case 0x017D: goto L017D;
    case 0x017E: goto L017E;
    case 0x0180: goto L0180;
    case 0x0182: goto L0182;
    case 0x0186: goto L0186;
    case 0x018B: goto L018B;
    case 0x018E: goto L018E;
    case 0x0192: goto L0192;
    case 0x0193: goto L0193;
    case 0x0195: goto L0195;
    case 0x0199: goto L0199;
    case 0x019B: goto L019B;
    case 0x019D: goto L019D;
    case 0x019F: goto L019F;
    case 0x01A1: goto L01A1;
    case 0x01A2: goto L01A2;
    case 0x01A4: goto L01A4;
    case 0x01A6: goto L01A6;
    case 0x01AA: goto L01AA;
    case 0x01AF: goto L01AF;
    case 0x01B2: goto L01B2;
    case 0x01B6: goto L01B6;
    case 0x01B7: goto L01B7;
    case 0x01B9: goto L01B9;
    case 0x01BD: goto L01BD;
    case 0x01BF: goto L01BF;
    case 0x01C1: goto L01C1;
    case 0x01C3: goto L01C3;
    case 0x01C5: goto L01C5;
    case 0x01C6: goto L01C6;
    case 0x01C8: goto L01C8;
    case 0x01CA: goto L01CA;
    case 0x01CE: goto L01CE;
    case 0x01D3: goto L01D3;
    case 0x01D9: goto L01D9;
    case 0x01DA: goto L01DA;
    case 0x01DB: goto L01DB;
    case 0x01DC: goto L01DC;
    case 0x01DF: goto L01DF;
    case 0x01E1: goto L01E1;
    case 0x01E3: goto L01E3;
    case 0x01E8: goto L01E8;
    case 0x01ED: goto L01ED;
    case 0x01EE: goto L01EE;
    case 0x01F0: goto L01F0;
    case 0x01F4: goto L01F4;
    case 0x01F5: goto L01F5;
    case 0x01F8: goto L01F8;
    case 0x01FA: goto L01FA;
    case 0x01FC: goto L01FC;
    case 0x01FE: goto L01FE;
    case 0x0200: goto L0200;
    case 0x0202: goto L0202;
    case 0x0204: goto L0204;
    case 0x0207: goto L0207;
    case 0x020A: goto L020A;
    case 0x020D: goto L020D;
    case 0x0210: goto L0210;
    case 0x0211: goto L0211;
    case 0x0215: goto L0215;
    case 0x021A: goto L021A;
    case 0x021C: goto L021C;
    case 0x021D: goto L021D;
    case 0x0220: goto L0220;
    case 0x0223: goto L0223;
    case 0x0225: goto L0225;
    case 0x0228: goto L0228;
    case 0x022A: goto L022A;
    case 0x022E: goto L022E;
    case 0x0231: goto L0231;
    case 0x0234: goto L0234;
    case 0x0237: goto L0237;
    case 0x0239: goto L0239;
    case 0x023A: goto L023A;
    case 0x023B: goto L023B;
    case 0x023D: goto L023D;
    case 0x023F: goto L023F;
    case 0x0241: goto L0241;
    case 0x0243: goto L0243;
    case 0x0245: goto L0245;
    case 0x0247: goto L0247;
    case 0x0248: goto L0248;
    case 0x024B: goto L024B;
    case 0x024D: goto L024D;
    case 0x0250: goto L0250;
    case 0x0253: goto L0253;
    case 0x0255: goto L0255;
    case 0x0259: goto L0259;
    case 0x025D: goto L025D;
    case 0x0261: goto L0261;
    case 0x0265: goto L0265;
    case 0x0269: goto L0269;
    case 0x026A: goto L026A;
    case 0x026B: goto L026B;
    case 0x026E: goto L026E;
    case 0x0271: goto L0271;
    case 0x0273: goto L0273;
    case 0x0276: goto L0276;
    case 0x0279: goto L0279;
    case 0x027B: goto L027B;
    case 0x027E: goto L027E;
    case 0x0282: goto L0282;
    case 0x0286: goto L0286;
    case 0x0288: goto L0288;
    case 0x028C: goto L028C;
    case 0x028E: goto L028E;
    case 0x0292: goto L0292;
    case 0x0294: goto L0294;
    case 0x0296: goto L0296;
    case 0x0299: goto L0299;
    case 0x029D: goto L029D;
    case 0x029E: goto L029E;
    case 0x02A0: goto L02A0;
    case 0x02A4: goto L02A4;
    case 0x02A7: goto L02A7;
    case 0x02AB: goto L02AB;
    case 0x02AC: goto L02AC;
    case 0x02AE: goto L02AE;
    case 0x02B2: goto L02B2;
    case 0x02B4: goto L02B4;
    case 0x02B6: goto L02B6;
    case 0x02B8: goto L02B8;
    case 0x02BA: goto L02BA;
    case 0x02BB: goto L02BB;
    case 0x02BD: goto L02BD;
    case 0x02BF: goto L02BF;
    case 0x02C3: goto L02C3;
    case 0x02C8: goto L02C8;
    case 0x02C9: goto L02C9;
    case 0x02CB: goto L02CB;
    case 0x02CE: goto L02CE;
    case 0x02D1: goto L02D1;
    case 0x02D5: goto L02D5;
    case 0x02D7: goto L02D7;
    case 0x02DB: goto L02DB;
    case 0x02DF: goto L02DF;
    case 0x02E1: goto L02E1;
    case 0x02E5: goto L02E5;
    case 0x02E7: goto L02E7;
    case 0x02E9: goto L02E9;
    case 0x02EA: goto L02EA;
    case 0x02EE: goto L02EE;
    case 0x02F2: goto L02F2;
    case 0x02F5: goto L02F5;
    case 0x02F7: goto L02F7;
    case 0x02FC: goto L02FC;
    case 0x0300: goto L0300;
    case 0x0306: goto L0306;
    case 0x030C: goto L030C;
    case 0x0312: goto L0312;
    case 0x0318: goto L0318;
    case 0x031E: goto L031E;
    case 0x0324: goto L0324;
    case 0x032A: goto L032A;
    case 0x0330: goto L0330;
    case 0x0336: goto L0336;
    case 0x033C: goto L033C;
    case 0x0342: goto L0342;
    case 0x0348: goto L0348;
    case 0x034C: goto L034C;
    case 0x0350: goto L0350;
    case 0x0352: goto L0352;
    case 0x0356: goto L0356;
    case 0x0358: goto L0358;
    case 0x035B: goto L035B;
    case 0x035E: goto L035E;
    case 0x0360: goto L0360;
    case 0x0363: goto L0363;
    case 0x0364: goto L0364;
    case 0x0369: goto L0369;
    case 0x036E: goto L036E;
    case 0x036F: goto L036F;
    case 0x0370: goto L0370;
    case 0x0371: goto L0371;
    case 0x0372: goto L0372;
    case 0x0373: goto L0373;
    case 0x0374: goto L0374;
    case 0x0377: goto L0377;
    case 0x0379: goto L0379;
    case 0x037C: goto L037C;
    case 0x037F: goto L037F;
    case 0x0382: goto L0382;
    case 0x0384: goto L0384;
    case 0x0386: goto L0386;
    case 0x038A: goto L038A;
    case 0x038C: goto L038C;
    case 0x038E: goto L038E;
    case 0x0390: goto L0390;
    case 0x0394: goto L0394;
    case 0x0398: goto L0398;
    case 0x0399: goto L0399;
    case 0x039A: goto L039A;
    case 0x039B: goto L039B;
    case 0x039C: goto L039C;
    case 0x039F: goto L039F;
    case 0x03A3: goto L03A3;
    case 0x03A5: goto L03A5;
    default: asm_bad_entry("WALLMAP.ASM", entry);
    }

    /* seg003_60  (+60)
       _60 (get_wright in POLYFILL's terms): advance the right-hand edge to the next vertex (forwards
       through the list at 659, wrapping at 7AB): load its x and its two texture coordinates, and patch
       the per-row steps of all three (16.16, from divides by the edge's height) into the drawing
       loop. Out: ZF set when no vertices are left. L0093 is the re-entry used while drawing. */
L0060: /* _seg003_60 */
    /* 0060  mov     bx,word ptr ds:[7B5h] */
    BX = rw(pDS, 0x7B5);
L0064:
    /* 0064  mov     ax,word ptr [bx+0Ah] */
    AX = rw(pDS, BX + 0xA);
L0067:
    /* 0067  mov     ax,word ptr [bx+0Ch] */
    AX = rw(pDS, BX + 0xC);
L006A:
    /* 006A  mov     ax,word ptr [bx+4] */
    AX = rw(pDS, BX + 0x4);
L006D:
    /* 006D  mov     word ptr ds:[7C7h],ax */
    ww(pDS, 0x7C7, AX);
L0070:
    /* 0070  mov     ax,word ptr [bx+6] */
    AX = rw(pDS, BX + 0x6);
L0073:
    /* 0073  mov     word ptr ds:[7C9h],ax */
    ww(pDS, 0x7C9, AX);
L0076:
    /* 0076  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L0078:
    /* 0078  mov     word ptr ds:[7C3h],ax */
    ww(pDS, 0x7C3, AX);
L007B:
    /* 007B  mov     ax,8000h */
    AX = 0x8000;
L007E:
    /* 007E  mov     word ptr ds:[7C5h],ax */
    ww(pDS, 0x7C5, AX);
L0081:
    /* 0081  mov     word ptr ds:[7CBh],ax */
    ww(pDS, 0x7CB, AX);
L0084:
    /* 0084  mov     word ptr ds:[7CDh],ax */
    ww(pDS, 0x7CD, AX);
L0087:
    /* 0087  add     bx,0Eh */
    BX = (uint16_t)(BX + 0xE);
L008A:
    /* 008A  cmp     bx,word ptr ds:[7ABh] */
    sub16(BX, rw(pDS, 0x7AB), 0);
L008E:
    /* 008E  jne     short L0093 */
    if (!ZF) goto L0093;
L0090:
    /* 0090  mov     bx,659h */
    BX = 0x659;
L0093: /* L0093 */
    /* 0093  mov     word ptr ds:[7B5h],bx */
    ww(pDS, 0x7B5, BX);
L0097:
    /* 0097  dec     word ptr ds:[7D1h] */
    ww(pDS, 0x7D1, dec16(rw(pDS, 0x7D1)));
L009B:
    /* 009B  jne     short L00A0 */
    if (!ZF) goto L00A0;
L009D:
    /* 009D  jmp     L0120 */
    goto L0120;
L00A0: /* L00A0 */
    /* 00A0  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L00A3:
    /* 00A3  mov     word ptr cs:L0354,ax */
    ww(CODE003, 0x354, AX);
L00A7:
    /* 00A7  mov     cx,word ptr ds:[7CFh] */
    CX = rw(pDS, 0x7CF);
L00AB:
    /* 00AB  sub     cx,ax */
    CX = sub16(CX, AX, 0);
L00AD:
    /* 00AD  je      _seg003_60 */
    if (ZF) goto L0060;
L00AF:
    /* 00AF  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L00B1:
    /* 00B1  sub     ax,word ptr ds:[7C3h] */
    AX = (uint16_t)(AX - rw(pDS, 0x7C3));
L00B5:
    /* 00B5  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L00B6:
    /* 00B6  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x00B6, 2)) != 0) return c;
L00B8:
    /* 00B8  mov     word ptr cs:L0346,ax */
    ww(CODE003, 0x346, AX);
L00BC:
    /* 00BC  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L00BE:
    /* 00BE  sar     dx,1 */
    DX = sar16(DX, 1);
L00C0:
    /* 00C0  rcr     ax,1 */
    AX = rcr16(AX, 1);
L00C2:
    /* 00C2  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x00C2, 2)) != 0) return c;
L00C4:
    /* 00C4  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L00C5:
    /* 00C5  shl     ax,1 */
    AX = shl16(AX, 1);
L00C7:
    /* 00C7  rcl     dx,1 */
    DX = rcl16(DX, 1);
L00C9:
    /* 00C9  mov     word ptr cs:L0340,ax */
    ww(CODE003, 0x340, AX);
L00CD:
    /* 00CD  add     word ptr cs:L0346,dx */
    ww(CODE003, 0x346, (uint16_t)(rw(CODE003, 0x346) + DX));
L00D2:
    /* 00D2  mov     ax,word ptr [bx+4] */
    AX = rw(pDS, BX + 0x4);
L00D5:
    /* 00D5  sub     ax,word ptr ds:[7C7h] */
    AX = (uint16_t)(AX - rw(pDS, 0x7C7));
L00D9:
    /* 00D9  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L00DA:
    /* 00DA  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x00DA, 2)) != 0) return c;
L00DC:
    /* 00DC  mov     word ptr cs:L032E,ax */
    ww(CODE003, 0x32E, AX);
L00E0:
    /* 00E0  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L00E2:
    /* 00E2  sar     dx,1 */
    DX = sar16(DX, 1);
L00E4:
    /* 00E4  rcr     ax,1 */
    AX = rcr16(AX, 1);
L00E6:
    /* 00E6  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x00E6, 2)) != 0) return c;
L00E8:
    /* 00E8  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L00E9:
    /* 00E9  shl     ax,1 */
    AX = shl16(AX, 1);
L00EB:
    /* 00EB  rcl     dx,1 */
    DX = rcl16(DX, 1);
L00ED:
    /* 00ED  mov     word ptr cs:L0328,ax */
    ww(CODE003, 0x328, AX);
L00F1:
    /* 00F1  add     word ptr cs:L032E,dx */
    ww(CODE003, 0x32E, (uint16_t)(rw(CODE003, 0x32E) + DX));
L00F6:
    /* 00F6  mov     ax,word ptr [bx+6] */
    AX = rw(pDS, BX + 0x6);
L00F9:
    /* 00F9  sub     ax,word ptr ds:[7C9h] */
    AX = (uint16_t)(AX - rw(pDS, 0x7C9));
L00FD:
    /* 00FD  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L00FE:
    /* 00FE  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x00FE, 2)) != 0) return c;
L0100:
    /* 0100  mov     word ptr cs:L033A,ax */
    ww(CODE003, 0x33A, AX);
L0104:
    /* 0104  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L0106:
    /* 0106  sar     dx,1 */
    DX = sar16(DX, 1);
L0108:
    /* 0108  rcr     ax,1 */
    AX = rcr16(AX, 1);
L010A:
    /* 010A  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x010A, 2)) != 0) return c;
L010C:
    /* 010C  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L010D:
    /* 010D  shl     ax,1 */
    AX = shl16(AX, 1);
L010F:
    /* 010F  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0111:
    /* 0111  mov     word ptr cs:L0334,ax */
    ww(CODE003, 0x334, AX);
L0115:
    /* 0115  add     word ptr cs:L033A,dx */
    ww(CODE003, 0x33A, (uint16_t)(rw(CODE003, 0x33A) + DX));
L011A:
    /* 011A  test    word ptr ds:[7D1h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x7D1) & 0xFFFF));
L0120: /* L0120 */
    /* 0120  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_121  (+121)
       _121 (get_wleft): the same for the left-hand edge, backwards through the list. After its ret is
       the mapper's far entry (+1DA): save the caller's stack and switch to the library's, take the
       vertex count in CX and the texture record from ES:[0B07Ch], find the extremes, then draw row
       by row until both edges run out (each row stepping the two texture coordinates across it,
       the texture's row through the AND mask patched at L02E3 and the column in DH), set the colour
       from 3963:0658 when the pen-mode table is in use (as POLYFILL does in UW1, here with 0B0h for
       0A0h), and return far on the caller's stack. */
L0121: /* _seg003_121 */
    /* 0121  mov     bx,word ptr ds:[7B3h] */
    BX = rw(pDS, 0x7B3);
L0125:
    /* 0125  mov     ax,word ptr [bx+4] */
    AX = rw(pDS, BX + 0x4);
L0128:
    /* 0128  mov     word ptr ds:[7BBh],ax */
    ww(pDS, 0x7BB, AX);
L012B:
    /* 012B  mov     ax,word ptr [bx+6] */
    AX = rw(pDS, BX + 0x6);
L012E:
    /* 012E  mov     word ptr ds:[7BDh],ax */
    ww(pDS, 0x7BD, AX);
L0131:
    /* 0131  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L0133:
    /* 0133  mov     word ptr ds:[7B7h],ax */
    ww(pDS, 0x7B7, AX);
L0136:
    /* 0136  mov     ax,8000h */
    AX = 0x8000;
L0139:
    /* 0139  mov     word ptr ds:[7B9h],ax */
    ww(pDS, 0x7B9, AX);
L013C:
    /* 013C  mov     word ptr ds:[7BFh],ax */
    ww(pDS, 0x7BF, AX);
L013F:
    /* 013F  mov     word ptr ds:[7C1h],ax */
    ww(pDS, 0x7C1, AX);
L0142:
    /* 0142  sub     bx,0Eh */
    BX = (uint16_t)(BX - 0xE);
L0145:
    /* 0145  cmp     bx,64Bh */
    sub16(BX, 0x64B, 0);
L0149:
    /* 0149  jne     short L014F */
    if (!ZF) goto L014F;
L014B:
    /* 014B  mov     bx,word ptr ds:[7A9h] */
    BX = rw(pDS, 0x7A9);
L014F: /* L014F */
    /* 014F  mov     word ptr ds:[7B3h],bx */
    ww(pDS, 0x7B3, BX);
L0153:
    /* 0153  dec     word ptr ds:[7D1h] */
    ww(pDS, 0x7D1, dec16(rw(pDS, 0x7D1)));
L0157:
    /* 0157  je      L0120 */
    if (ZF) goto L0120;
L0159:
    /* 0159  mov     ax,word ptr [bx+2] */
    AX = rw(pDS, BX + 0x2);
L015C:
    /* 015C  mov     word ptr cs:L034E,ax */
    ww(CODE003, 0x34E, AX);
L0160:
    /* 0160  mov     cx,word ptr ds:[7CFh] */
    CX = rw(pDS, 0x7CF);
L0164:
    /* 0164  sub     cx,ax */
    CX = sub16(CX, AX, 0);
L0166:
    /* 0166  je      _seg003_121 */
    if (ZF) goto L0121;
L0168:
    /* 0168  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L016A:
    /* 016A  sub     ax,word ptr ds:[7B7h] */
    AX = (uint16_t)(AX - rw(pDS, 0x7B7));
L016E:
    /* 016E  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L016F:
    /* 016F  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x016F, 2)) != 0) return c;
L0171:
    /* 0171  mov     word ptr cs:L0322,ax */
    ww(CODE003, 0x322, AX);
L0175:
    /* 0175  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L0177:
    /* 0177  sar     dx,1 */
    DX = sar16(DX, 1);
L0179:
    /* 0179  rcr     ax,1 */
    AX = rcr16(AX, 1);
L017B:
    /* 017B  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x017B, 2)) != 0) return c;
L017D:
    /* 017D  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L017E:
    /* 017E  shl     ax,1 */
    AX = shl16(AX, 1);
L0180:
    /* 0180  rcl     dx,1 */
    DX = rcl16(DX, 1);
L0182:
    /* 0182  mov     word ptr cs:L031C,ax */
    ww(CODE003, 0x31C, AX);
L0186:
    /* 0186  add     word ptr cs:L0322,dx */
    ww(CODE003, 0x322, (uint16_t)(rw(CODE003, 0x322) + DX));
L018B:
    /* 018B  mov     ax,word ptr [bx+4] */
    AX = rw(pDS, BX + 0x4);
L018E:
    /* 018E  sub     ax,word ptr ds:[7BBh] */
    AX = (uint16_t)(AX - rw(pDS, 0x7BB));
L0192:
    /* 0192  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0193:
    /* 0193  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x0193, 2)) != 0) return c;
L0195:
    /* 0195  mov     word ptr cs:L030A,ax */
    ww(CODE003, 0x30A, AX);
L0199:
    /* 0199  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L019B:
    /* 019B  sar     dx,1 */
    DX = sar16(DX, 1);
L019D:
    /* 019D  rcr     ax,1 */
    AX = rcr16(AX, 1);
L019F:
    /* 019F  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x019F, 2)) != 0) return c;
L01A1:
    /* 01A1  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L01A2:
    /* 01A2  shl     ax,1 */
    AX = shl16(AX, 1);
L01A4:
    /* 01A4  rcl     dx,1 */
    DX = rcl16(DX, 1);
L01A6:
    /* 01A6  mov     word ptr cs:L0304,ax */
    ww(CODE003, 0x304, AX);
L01AA:
    /* 01AA  add     word ptr cs:L030A,dx */
    ww(CODE003, 0x30A, (uint16_t)(rw(CODE003, 0x30A) + DX));
L01AF:
    /* 01AF  mov     ax,word ptr [bx+6] */
    AX = rw(pDS, BX + 0x6);
L01B2:
    /* 01B2  sub     ax,word ptr ds:[7BDh] */
    AX = (uint16_t)(AX - rw(pDS, 0x7BD));
L01B6:
    /* 01B6  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L01B7:
    /* 01B7  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x01B7, 2)) != 0) return c;
L01B9:
    /* 01B9  mov     word ptr cs:L0316,ax */
    ww(CODE003, 0x316, AX);
L01BD:
    /* 01BD  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L01BF:
    /* 01BF  sar     dx,1 */
    DX = sar16(DX, 1);
L01C1:
    /* 01C1  rcr     ax,1 */
    AX = rcr16(AX, 1);
L01C3:
    /* 01C3  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x01C3, 2)) != 0) return c;
L01C5:
    /* 01C5  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L01C6:
    /* 01C6  shl     ax,1 */
    AX = shl16(AX, 1);
L01C8:
    /* 01C8  rcl     dx,1 */
    DX = rcl16(DX, 1);
L01CA:
    /* 01CA  mov     word ptr cs:L0310,ax */
    ww(CODE003, 0x310, AX);
L01CE:
    /* 01CE  add     word ptr cs:L0316,dx */
    ww(CODE003, 0x316, (uint16_t)(rw(CODE003, 0x316) + DX));
L01D3:
    /* 01D3  test    word ptr ds:[7D1h],0FFFFh */
    logic16((uint16_t)(rw(pDS, 0x7D1) & 0xFFFF));
L01D9:
    /* 01D9  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* the mapper's far entry (+1DA), named because TMAPOPS stores its offset (the link needs the
       name to take this module from the library) */
L01DA: /* _seg003_1DA */
    /* 01DA  push    es */
    push16(asm_es);
L01DB:
    /* 01DB  push    ds */
    push16(asm_ds);
L01DC:
    /* 01DC  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L01DF:
    /* 01DF  mov     ds,ax */
    SET_DS(AX);
L01E1:
    /* 01E1  mov     bx,ss */
    BX = asm_ss;
L01E3:
    /* 01E3  mov     word ptr cs:L03A7,bx */
    ww(CODE003, 0x3A7, BX);
L01E8:
    /* 01E8  mov     word ptr cs:L03A9,sp */
    ww(CODE003, 0x3A9, SP);
L01ED:
    /* 01ED  cli */
    ;
L01EE:
    /* 01EE  mov     ss,ax */
    SET_SS(AX);
L01F0:
    /* 01F0  mov     sp,word ptr ds:[5586h] */
    SP = rw(pDS, 0x5586);
L01F4:
    /* 01F4  sti */
    ;
L01F5:
    /* 01F5  mov     ax,0Eh */
    /* by hand: gfx_texture_poly_affine's body, after the stack switch: --enhance perspective walks the face perspective-correctly in gfx/perspmap.c (uwpatch's) and leaves by the original's exit */
    persp_probe_next();  /* the coverage test's face colour */
    if (ENHANCED(ENH_PERSPECTIVE)) { persp_walk(0); goto L0363; }  /* gfx/perspmap.c, uwpatch's */
    AX = 0xE;
L01F8:
    /* 01F8  mov     ax,cx */
    AX = CX;
L01FA:
    /* 01FA  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L01FC:
    /* 01FC  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L01FE:
    /* 01FE  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L0200:
    /* 0200  sub     ax,cx */
    AX = (uint16_t)(AX - CX);
L0202:
    /* 0202  shl     ax,1 */
    AX = (uint16_t)(AX << 1);
L0204:
    /* 0204  add     ax,659h */
    AX = (uint16_t)(AX + 0x659);
L0207:
    /* 0207  mov     word ptr ds:[7ABh],ax */
    ww(pDS, 0x7AB, AX);
L020A:
    /* 020A  sub     ax,0Eh */
    AX = (uint16_t)(AX - 0xE);
L020D:
    /* 020D  mov     word ptr ds:[7A9h],ax */
    ww(pDS, 0x7A9, AX);
L0210:
    /* 0210  inc     cx */
    CX = (uint16_t)(CX + 1);
L0211:
    /* 0211  mov     word ptr ds:[7D1h],cx */
    ww(pDS, 0x7D1, CX);
L0215:
    /* 0215  mov     si,word ptr es:[0B07Ch] */
    SI = rw(pES, 0xB07C);
L021A:
    /* 021A  lods    word ptr es:[si] */
    AX = rw(pES, SI); SI = (uint16_t)(SI + STEP(2));
L021C:
    /* 021C  dec     ax */
    AX = (uint16_t)(AX - 1);
L021D:
    /* 021D  mov     word ptr ds:[7ADh],ax */
    ww(pDS, 0x7AD, AX);
L0220:
    /* 0220  add     si,2 */
    SI = (uint16_t)(SI + 0x2);
L0223:
    /* 0223  lods    word ptr es:[si] */
    AX = rw(pES, SI); SI = (uint16_t)(SI + STEP(2));
L0225:
    /* 0225  mov     word ptr ds:[7AFh],ax */
    ww(pDS, 0x7AF, AX);
L0228:
    /* 0228  lods    word ptr es:[si] */
    AX = rw(pES, SI); SI = (uint16_t)(SI + STEP(2));
L022A:
    /* 022A  mov     word ptr cs:L02E3,ax */
    ww(CODE003, 0x2E3, AX);
L022E:
    /* 022E  mov     bx,0D8F0h */
    BX = 0xD8F0;
L0231:
    /* 0231  mov     bp,2710h */
    BP = 0x2710;
L0234:
    /* 0234  mov     si,659h */
    SI = 0x659;
L0237:
    /* 0237  mov     di,si */
    DI = SI;
L0239:
    /* 0239  dec     cx */
    CX = (uint16_t)(CX - 1);
L023A: /* L023A */
    /* 023A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L023B:
    /* 023B  cmp     bx,ax */
    sub16(BX, AX, 0);
L023D:
    /* 023D  jge     short L0241 */
    if (SF == OF) goto L0241;
L023F:
    /* 023F  mov     bx,ax */
    BX = AX;
L0241: /* L0241 */
    /* 0241  cmp     bp,ax */
    sub16(BP, AX, 0);
L0243:
    /* 0243  jle     short L0247 */
    if (ZF || SF != OF) goto L0247;
L0245:
    /* 0245  mov     bp,ax */
    BP = AX;
L0247: /* L0247 */
    /* 0247  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0248:
    /* 0248  cmp     ax,word ptr [di+2] */
    sub16(AX, rw(pDS, DI + 0x2), 0);
L024B:
    /* 024B  jle     short L0250 */
    if (ZF || SF != OF) goto L0250;
L024D:
    /* 024D  lea     di,[si-4] */
    DI = (uint16_t)(SI + 0xFFFC);
L0250: /* L0250 */
    /* 0250  add     si,0Ah */
    SI = (uint16_t)(SI + 0xA);
L0253:
    /* 0253  loop    L023A */
    if (--CX) goto L023A;
L0255:
    /* 0255  mov     word ptr ds:[7B1h],di */
    ww(pDS, 0x7B1, DI);
L0259:
    /* 0259  mov     es,word ptr ds:[95Ah] */
    SET_ES(rw(pDS, 0x95A));
L025D:
    /* 025D  mov     si,word ptr ds:[7B1h] */
    SI = rw(pDS, 0x7B1);
L0261:
    /* 0261  mov     word ptr ds:[7B3h],si */
    ww(pDS, 0x7B3, SI);
L0265:
    /* 0265  mov     word ptr ds:[7B5h],si */
    ww(pDS, 0x7B5, SI);
L0269:
    /* 0269  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L026A:
    /* 026A  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L026B:
    /* 026B  mov     word ptr ds:[7CFh],ax */
    ww(pDS, 0x7CF, AX);
L026E:
    /* 026E  call    _seg003_121 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x0121), 0x0271)) != 0) return c;
L0271:
    /* 0271  jne     short L0276 */
    if (!ZF) goto L0276;
L0273:
    /* 0273  jmp     L0363 */
    goto L0363;
L0276: /* L0276 */
    /* 0276  call    _seg003_60 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x0060), 0x0279)) != 0) return c;
L0279:
    /* 0279  jne     short L027E */
    if (!ZF) goto L027E;
L027B:
    /* 027B  jmp     L0363 */
    goto L0363;
L027E: /* L027E */
    /* 027E  mov     si,word ptr ds:[7B7h] */
    SI = rw(pDS, 0x7B7);
L0282:
    /* 0282  mov     di,word ptr ds:[7CFh] */
    DI = rw(pDS, 0x7CF);
L0286:
    /* 0286  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L0288:
    /* 0288  mov     di,word ptr [di+95Eh] */
    DI = rw(pDS, DI + 0x95E);
L028C:
    /* 028C  add     di,si */
    DI = (uint16_t)(DI + SI);
L028E:
    /* 028E  mov     cx,word ptr ds:[7C3h] */
    CX = rw(pDS, 0x7C3);
L0292:
    /* 0292  sub     cx,si */
    CX = sub16(CX, SI, 0);
L0294:
    /* 0294  je      short L02C8 */
    if (ZF) goto L02C8;
L0296:
    /* 0296  mov     ax,word ptr ds:[7C7h] */
    AX = rw(pDS, 0x7C7);
L0299:
    /* 0299  sub     ax,word ptr ds:[7BBh] */
    AX = (uint16_t)(AX - rw(pDS, 0x7BB));
L029D:
    /* 029D  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L029E:
    /* 029E  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x029E, 2)) != 0) return c;
L02A0:
    /* 02A0  mov     word ptr cs:L02EC,ax */
    ww(CODE003, 0x2EC, AX);
L02A4:
    /* 02A4  mov     ax,word ptr ds:[7C9h] */
    AX = rw(pDS, 0x7C9);
L02A7:
    /* 02A7  sub     ax,word ptr ds:[7BDh] */
    AX = (uint16_t)(AX - rw(pDS, 0x7BD));
L02AB:
    /* 02AB  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L02AC:
    /* 02AC  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x02AC, 2)) != 0) return c;
L02AE:
    /* 02AE  mov     word ptr cs:L02F3,ax */
    ww(CODE003, 0x2F3, AX);
L02B2:
    /* 02B2  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L02B4:
    /* 02B4  sar     dx,1 */
    DX = sar16(DX, 1);
L02B6:
    /* 02B6  rcr     ax,1 */
    AX = rcr16(AX, 1);
L02B8:
    /* 02B8  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0090, 0x02B8, 2)) != 0) return c;
L02BA:
    /* 02BA  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L02BB:
    /* 02BB  shl     ax,1 */
    AX = shl16(AX, 1);
L02BD:
    /* 02BD  rcl     dx,1 */
    DX = rcl16(DX, 1);
L02BF:
    /* 02BF  mov     word ptr cs:L02F0,ax */
    ww(CODE003, 0x2F0, AX);
L02C3:
    /* 02C3  add     word ptr cs:L02F3,dx */
    ww(CODE003, 0x2F3, add16(rw(CODE003, 0x2F3), DX, 0));
L02C8: /* L02C8 */
    /* 02C8  inc     cx */
    CX = inc16(CX);
L02C9:
    /* 02C9  jg      short L02CE */
    if (!ZF && SF == OF) goto L02CE;
L02CB:
    /* 02CB  jmp     L03A5 */
    goto L03A5;
L02CE: /* L02CE */
    /* 02CE  mov     ax,word ptr ds:[7BDh] */
    AX = rw(pDS, 0x7BD);
L02D1:
    /* 02D1  mov     dx,word ptr ds:[7BBh] */
    DX = rw(pDS, 0x7BB);
L02D5:
    /* 02D5  xor     bx,bx */
    BX = (uint16_t)(BX ^ BX);
L02D7:
    /* 02D7  mov     bp,word ptr ds:[7C1h] */
    BP = rw(pDS, 0x7C1);
L02DB:
    /* 02DB  mov     ds,word ptr ds:[7AFh] */
    SET_DS(rw(pDS, 0x7AF));
L02DF: /* L02DF */
    /* 02DF  mov     si,ax */
    SI = AX;
L02E1:
    /* 02E1  and     si,1234h */
    SI = (uint16_t)(SI & rw(CODE003, 0x02E3));
L02E5:
    /* 02E5  mov     bl,dh */
    BL = DH;
L02E7:
    /* 02E7  add     si,bx */
    SI = (uint16_t)(SI + BX);
L02E9:
    /* 02E9  movsb */
    /* by hand: movsb, the affine mapper's texel: with UW1PORT_TEXTURE_PROBE (a test), the face's colour (gfx/perspmap.c) */
    wb(pES, DI, persp_probe() ? persp_probe_colour : rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L02EA:
    /* 02EA  add     dx,1234h */
    DX = (uint16_t)(DX + rw(CODE003, 0x02EC));
L02EE:
    /* 02EE  add     bp,1234h */
    BP = add16(BP, rw(CODE003, 0x02F0), 0);
L02F2:
    /* 02F2  adc     ax,1234h */
    AX = (uint16_t)(AX + rw(CODE003, 0x02F3) + CF);
L02F5:
    /* 02F5  loop    L02DF */
    if (--CX) goto L02DF;
L02F7:
    /* 02F7  mov     ds,word ptr ss:[7D4h] */
    SET_DS(rw(pSS, 0x7D4));
L02FC:
    /* 02FC  dec     word ptr ds:[7CFh] */
    ww(pDS, 0x7CF, (uint16_t)(rw(pDS, 0x7CF) - 1));
L0300:
    /* 0300  add     word ptr ds:[7BFh],1234h */
    ww(pDS, 0x7BF, add16(rw(pDS, 0x7BF), rw(CODE003, 0x0304), 0));
L0306:
    /* 0306  adc     word ptr ds:[7BBh],1234h */
    ww(pDS, 0x7BB, (uint16_t)(rw(pDS, 0x7BB) + rw(CODE003, 0x030A) + CF));
L030C:
    /* 030C  add     word ptr ds:[7C1h],1234h */
    ww(pDS, 0x7C1, add16(rw(pDS, 0x7C1), rw(CODE003, 0x0310), 0));
L0312:
    /* 0312  adc     word ptr ds:[7BDh],1234 */
    ww(pDS, 0x7BD, (uint16_t)(rw(pDS, 0x7BD) + rw(CODE003, 0x0316) + CF));
L0318:
    /* 0318  add     word ptr ds:[7B9h],1234h */
    ww(pDS, 0x7B9, add16(rw(pDS, 0x7B9), rw(CODE003, 0x031C), 0));
L031E:
    /* 031E  adc     word ptr ds:[7B7h],1234h */
    ww(pDS, 0x7B7, (uint16_t)(rw(pDS, 0x7B7) + rw(CODE003, 0x0322) + CF));
L0324:
    /* 0324  add     word ptr ds:[7CBh],1234h */
    ww(pDS, 0x7CB, add16(rw(pDS, 0x7CB), rw(CODE003, 0x0328), 0));
L032A:
    /* 032A  adc     word ptr ds:[7C7h],1234h */
    ww(pDS, 0x7C7, (uint16_t)(rw(pDS, 0x7C7) + rw(CODE003, 0x032E) + CF));
L0330:
    /* 0330  add     word ptr ds:[7CDh],1234h */
    ww(pDS, 0x7CD, add16(rw(pDS, 0x7CD), rw(CODE003, 0x0334), 0));
L0336:
    /* 0336  adc     word ptr ds:[7C9h],1234 */
    ww(pDS, 0x7C9, (uint16_t)(rw(pDS, 0x7C9) + rw(CODE003, 0x033A) + CF));
L033C:
    /* 033C  add     word ptr ds:[7C5h],1234h */
    ww(pDS, 0x7C5, add16(rw(pDS, 0x7C5), rw(CODE003, 0x0340), 0));
L0342:
    /* 0342  adc     word ptr ds:[7C3h],1234h */
    ww(pDS, 0x7C3, (uint16_t)(rw(pDS, 0x7C3) + rw(CODE003, 0x0346) + CF));
L0348:
    /* 0348  mov     cx,word ptr ds:[7CFh] */
    CX = rw(pDS, 0x7CF);
L034C:
    /* 034C  cmp     cx,1234h */
    sub16(CX, rw(CODE003, 0x034E), 0);
L0350:
    /* 0350  jle     short L039C */
    if (ZF || SF != OF) goto L039C;
L0352: /* L0352 */
    /* 0352  cmp     cx,1234h */
    sub16(CX, rw(CODE003, 0x0354), 0);
L0356:
    /* 0356  jle     short L035B */
    if (ZF || SF != OF) goto L035B;
L0358:
    /* 0358  jmp     L027E */
    goto L027E;
L035B: /* L035B */
    /* 035B  call    _seg003_60 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x0060), 0x035E)) != 0) return c;
L035E:
    /* 035E  je      short L0363 */
    if (ZF) goto L0363;
L0360:
    /* 0360  jmp     L027E */
    goto L027E;
L0363: /* L0363 */
    /* 0363  cli */
    ;
L0364:
    /* 0364  mov     ss,word ptr cs:L03A7 */
    SET_SS(rw(CODE003, 0x3A7));
L0369:
    /* 0369  mov     sp,word ptr cs:L03A9 */
    SP = rw(CODE003, 0x3A9);
L036E:
    /* 036E  sti */
    ;
L036F:
    /* 036F  pop     ds */
    SET_DS(pop16());
L0370:
    /* 0370  pop     es */
    SET_ES(pop16());
L0371:
    /* 0371  push    es */
    push16(asm_es);
L0372:
    /* 0372  push    di */
    push16(DI);
L0373:
    /* 0373  push    ax */
    push16(AX);
L0374:
    /* 0374  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L0377:
    /* 0377  mov     es,ax */
    SET_ES(AX);
L0379:
    /* 0379  mov     ax,0 */
    AX = 0x0;
L037C:
    /* 037C  mov     cx,0FFh */
    CX = 0xFF;
L037F:
    /* 037F  mov     di,8 */
    DI = 0x8;
L0382:
    /* 0382  repe scasb */
    while (CX) { sub8(AL, rb(pES, DI), 0); DI = (uint16_t)(DI + STEP(1)); CX--; if (!ZF) break; }
L0384:
    /* 0384  jcxz    L0398 */
    if (!CX) goto L0398;
L0386:
    /* 0386  mov     al,byte ptr es:[658h] */
    AL = rb(pES, 0x658);
L038A:
    /* 038A  test    al,0FFh */
    logic8((uint8_t)(AL & 0xFF));
L038C:
    /* 038C  jne     short L0394 */
    if (!ZF) goto L0394;
L038E:
    /* 038E  mov     al,0B0h */
    AL = 0xB0;
L0390:
    /* 0390  mov     byte ptr es:[658h],al */
    wb(pES, 0x658, AL);
L0394: /* L0394 */
    /* 0394  mov     byte ptr es:[PEN_COLOR],al */
    wb(pES, 0x410F, AL);
L0398: /* L0398 */
    /* 0398  pop     ax */
    AX = pop16();
L0399:
    /* 0399  pop     di */
    DI = pop16();
L039A:
    /* 039A  pop     es */
    SET_ES(pop16());
L039B:
    /* 039B  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;
L039C: /* L039C */
    /* 039C  call    _seg003_121 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x0121), 0x039F)) != 0) return c;
L039F:
    /* 039F  mov     cx,word ptr ds:[7CFh] */
    CX = rw(pDS, 0x7CF);
L03A3:
    /* 03A3  jne     L0352 */
    if (!ZF) goto L0352;
L03A5: /* L03A5 */
    /* 03A5  jmp     L0363 */
    goto L0363;
}
