/* expand.c: replaces src/3d/EXPAND.ASM (seg004_0, 0000..0256 of its
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

uint32_t asm_mod_EXPAND(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x0020: goto L0020;
    case 0x0023: goto L0023;
    case 0x0025: goto L0025;
    case 0x0028: goto L0028;
    case 0x002A: goto L002A;
    case 0x002D: goto L002D;
    case 0x002E: goto L002E;
    case 0x0030: goto L0030;
    case 0x0031: goto L0031;
    case 0x0033: goto L0033;
    case 0x0034: goto L0034;
    case 0x0036: goto L0036;
    case 0x0038: goto L0038;
    case 0x0039: goto L0039;
    case 0x003C: goto L003C;
    case 0x003F: goto L003F;
    case 0x0041: goto L0041;
    case 0x0044: goto L0044;
    case 0x0046: goto L0046;
    case 0x0049: goto L0049;
    case 0x004A: goto L004A;
    case 0x004C: goto L004C;
    case 0x004D: goto L004D;
    case 0x004F: goto L004F;
    case 0x0051: goto L0051;
    case 0x0054: goto L0054;
    case 0x0056: goto L0056;
    case 0x0057: goto L0057;
    case 0x0059: goto L0059;
    case 0x005B: goto L005B;
    case 0x005D: goto L005D;
    case 0x005E: goto L005E;
    case 0x0060: goto L0060;
    case 0x0062: goto L0062;
    case 0x0063: goto L0063;
    case 0x0066: goto L0066;
    case 0x0069: goto L0069;
    case 0x006B: goto L006B;
    case 0x006E: goto L006E;
    case 0x0070: goto L0070;
    case 0x0072: goto L0072;
    case 0x0075: goto L0075;
    case 0x0077: goto L0077;
    case 0x0079: goto L0079;
    case 0x007B: goto L007B;
    case 0x007D: goto L007D;
    case 0x0081: goto L0081;
    case 0x0083: goto L0083;
    case 0x0084: goto L0084;
    case 0x0086: goto L0086;
    case 0x0087: goto L0087;
    case 0x0088: goto L0088;
    case 0x008A: goto L008A;
    case 0x008B: goto L008B;
    case 0x008C: goto L008C;
    case 0x008E: goto L008E;
    case 0x008F: goto L008F;
    case 0x0090: goto L0090;
    case 0x0092: goto L0092;
    case 0x0093: goto L0093;
    case 0x0094: goto L0094;
    case 0x0096: goto L0096;
    case 0x0097: goto L0097;
    case 0x0098: goto L0098;
    case 0x009A: goto L009A;
    case 0x009B: goto L009B;
    case 0x009C: goto L009C;
    case 0x009E: goto L009E;
    case 0x009F: goto L009F;
    case 0x00A0: goto L00A0;
    case 0x00A2: goto L00A2;
    case 0x00A3: goto L00A3;
    case 0x00A4: goto L00A4;
    case 0x00A6: goto L00A6;
    case 0x00A7: goto L00A7;
    case 0x00A8: goto L00A8;
    case 0x00AA: goto L00AA;
    case 0x00AB: goto L00AB;
    case 0x00AC: goto L00AC;
    case 0x00AE: goto L00AE;
    case 0x00AF: goto L00AF;
    case 0x00B0: goto L00B0;
    case 0x00B2: goto L00B2;
    case 0x00B3: goto L00B3;
    case 0x00B4: goto L00B4;
    case 0x00B6: goto L00B6;
    case 0x00B7: goto L00B7;
    case 0x00B8: goto L00B8;
    case 0x00BA: goto L00BA;
    case 0x00BB: goto L00BB;
    case 0x00BC: goto L00BC;
    case 0x00BE: goto L00BE;
    case 0x00BF: goto L00BF;
    case 0x00C0: goto L00C0;
    case 0x00C2: goto L00C2;
    case 0x00C3: goto L00C3;
    case 0x00C5: goto L00C5;
    case 0x00C8: goto L00C8;
    case 0x00CA: goto L00CA;
    case 0x00CB: goto L00CB;
    case 0x00CE: goto L00CE;
    case 0x00D0: goto L00D0;
    case 0x00D2: goto L00D2;
    case 0x00D6: goto L00D6;
    case 0x00D8: goto L00D8;
    case 0x00D9: goto L00D9;
    case 0x00DC: goto L00DC;
    case 0x00DF: goto L00DF;
    case 0x00E1: goto L00E1;
    case 0x00E4: goto L00E4;
    case 0x00E6: goto L00E6;
    case 0x00E9: goto L00E9;
    case 0x00EA: goto L00EA;
    case 0x00EC: goto L00EC;
    case 0x00ED: goto L00ED;
    case 0x00EF: goto L00EF;
    case 0x00F1: goto L00F1;
    case 0x00F2: goto L00F2;
    case 0x00F4: goto L00F4;
    case 0x00F6: goto L00F6;
    case 0x00F9: goto L00F9;
    case 0x00FA: goto L00FA;
    case 0x00FC: goto L00FC;
    case 0x00FE: goto L00FE;
    case 0x00FF: goto L00FF;
    case 0x0101: goto L0101;
    case 0x0103: goto L0103;
    case 0x0106: goto L0106;
    case 0x0108: goto L0108;
    case 0x010B: goto L010B;
    case 0x010E: goto L010E;
    case 0x0110: goto L0110;
    case 0x0113: goto L0113;
    case 0x0116: goto L0116;
    case 0x0118: goto L0118;
    case 0x011B: goto L011B;
    case 0x011C: goto L011C;
    case 0x011F: goto L011F;
    case 0x0122: goto L0122;
    case 0x0124: goto L0124;
    case 0x0127: goto L0127;
    case 0x0129: goto L0129;
    case 0x012C: goto L012C;
    case 0x012D: goto L012D;
    case 0x012F: goto L012F;
    case 0x0132: goto L0132;
    case 0x0135: goto L0135;
    case 0x0137: goto L0137;
    case 0x0138: goto L0138;
    case 0x013A: goto L013A;
    case 0x013D: goto L013D;
    case 0x013E: goto L013E;
    case 0x013F: goto L013F;
    case 0x0142: goto L0142;
    case 0x0145: goto L0145;
    case 0x0146: goto L0146;
    case 0x0149: goto L0149;
    case 0x014B: goto L014B;
    case 0x014C: goto L014C;
    case 0x014D: goto L014D;
    case 0x0150: goto L0150;
    case 0x0153: goto L0153;
    case 0x0154: goto L0154;
    case 0x0155: goto L0155;
    case 0x0158: goto L0158;
    case 0x015B: goto L015B;
    case 0x015C: goto L015C;
    case 0x015E: goto L015E;
    case 0x0161: goto L0161;
    case 0x0162: goto L0162;
    case 0x0163: goto L0163;
    case 0x0166: goto L0166;
    case 0x0169: goto L0169;
    case 0x016A: goto L016A;
    case 0x016D: goto L016D;
    case 0x016E: goto L016E;
    case 0x0170: goto L0170;
    case 0x0172: goto L0172;
    case 0x0175: goto L0175;
    case 0x0177: goto L0177;
    case 0x017A: goto L017A;
    case 0x017D: goto L017D;
    case 0x017F: goto L017F;
    case 0x0182: goto L0182;
    case 0x0185: goto L0185;
    case 0x0187: goto L0187;
    case 0x018A: goto L018A;
    case 0x018B: goto L018B;
    case 0x018C: goto L018C;
    case 0x018D: goto L018D;
    case 0x018F: goto L018F;
    case 0x0191: goto L0191;
    case 0x0194: goto L0194;
    case 0x0196: goto L0196;
    case 0x0198: goto L0198;
    case 0x019A: goto L019A;
    case 0x019B: goto L019B;
    case 0x019D: goto L019D;
    case 0x019F: goto L019F;
    case 0x01A1: goto L01A1;
    case 0x01A2: goto L01A2;
    case 0x01A4: goto L01A4;
    case 0x01A6: goto L01A6;
    case 0x01A8: goto L01A8;
    case 0x01A9: goto L01A9;
    case 0x01AB: goto L01AB;
    case 0x01AD: goto L01AD;
    case 0x01AE: goto L01AE;
    case 0x01B0: goto L01B0;
    case 0x01B1: goto L01B1;
    case 0x01B3: goto L01B3;
    case 0x01B5: goto L01B5;
    case 0x01B7: goto L01B7;
    case 0x01B8: goto L01B8;
    case 0x01BA: goto L01BA;
    case 0x01BC: goto L01BC;
    case 0x01BE: goto L01BE;
    case 0x01C1: goto L01C1;
    case 0x01C3: goto L01C3;
    case 0x01C6: goto L01C6;
    case 0x01C8: goto L01C8;
    case 0x01CA: goto L01CA;
    case 0x01CC: goto L01CC;
    case 0x01CE: goto L01CE;
    case 0x01CF: goto L01CF;
    case 0x01D1: goto L01D1;
    case 0x01D3: goto L01D3;
    case 0x01D6: goto L01D6;
    case 0x01D8: goto L01D8;
    case 0x01D9: goto L01D9;
    case 0x01DB: goto L01DB;
    case 0x01DD: goto L01DD;
    case 0x01DF: goto L01DF;
    case 0x01E0: goto L01E0;
    case 0x01E2: goto L01E2;
    case 0x01E4: goto L01E4;
    case 0x01E6: goto L01E6;
    case 0x01E9: goto L01E9;
    case 0x01EB: goto L01EB;
    case 0x01EE: goto L01EE;
    case 0x01F0: goto L01F0;
    case 0x01F2: goto L01F2;
    case 0x01F3: goto L01F3;
    case 0x01F5: goto L01F5;
    case 0x01F7: goto L01F7;
    case 0x01F9: goto L01F9;
    case 0x01FB: goto L01FB;
    case 0x01FD: goto L01FD;
    case 0x01FF: goto L01FF;
    case 0x0200: goto L0200;
    case 0x0202: goto L0202;
    case 0x0204: goto L0204;
    case 0x0207: goto L0207;
    case 0x0209: goto L0209;
    case 0x020A: goto L020A;
    case 0x020C: goto L020C;
    case 0x020D: goto L020D;
    case 0x020F: goto L020F;
    case 0x0211: goto L0211;
    case 0x0213: goto L0213;
    case 0x0219: goto L0219;
    case 0x021B: goto L021B;
    case 0x021C: goto L021C;
    case 0x021E: goto L021E;
    case 0x0220: goto L0220;
    case 0x0221: goto L0221;
    case 0x0224: goto L0224;
    case 0x0225: goto L0225;
    case 0x0227: goto L0227;
    case 0x022D: goto L022D;
    case 0x0230: goto L0230;
    case 0x0232: goto L0232;
    case 0x0233: goto L0233;
    case 0x0235: goto L0235;
    case 0x0237: goto L0237;
    case 0x0239: goto L0239;
    case 0x023C: goto L023C;
    case 0x023E: goto L023E;
    case 0x0241: goto L0241;
    case 0x0243: goto L0243;
    case 0x0245: goto L0245;
    case 0x0247: goto L0247;
    case 0x0248: goto L0248;
    case 0x024A: goto L024A;
    case 0x024C: goto L024C;
    case 0x024F: goto L024F;
    case 0x0251: goto L0251;
    case 0x0252: goto L0252;
    case 0x0254: goto L0254;
    default: asm_bad_entry("EXPAND.ASM", entry);
    }

    /* seg004_20  (+20)
       exp_8str: format 4, 8-bit uncompressed. In: AX = image paragraph, BP = offset of its size
       word, DH = lightabs row. Copies `size` bytes to cmpbuf1_start:0, each translated through
       lightabs row DH (BX from seg004_CB; xlat reads cs:BX+AL). Out: AX = cmpbuf1_start,
       ES:DI past the last pixel. The aux palette is not used: 8-bit pixels are already colours. */
L0020: /* _exp_8str */
    /* 0020  call    _seg004_CB */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x00CB), 0x0023)) != 0) return c;
L0023:
    /* 0023  mov     si,bp */
    SI = BP;
L0025:
    /* 0025  mov     dx,seg _cmpbuf1_start */
    DX = (uint16_t)(0x4423 + PORT_LOAD_SEG);
L0028:
    /* 0028  mov     es,dx */
    SET_ES(DX);
L002A:
    /* 002A  mov     di,0 */
    DI = 0x0;
L002D:
    /* 002D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L002E:
    /* 002E  mov     cx,ax */
    CX = AX;
L0030: /* L0030 */
    /* 0030  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0031:
    /* 0031  xlat    byte ptr cs:_uncmp_pal[bx] */
    AL = rb(CODE004, BX + AL);
L0033:
    /* 0033  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0034:
    /* 0034  loop    L0030 */
    if (--CX) goto L0030;
L0036:
    /* 0036  mov     ax,dx */
    AX = DX;
L0038:
    /* 0038  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_39  (+39)
       exp_4str: format 0Ah, 4-bit uncompressed. In: as exp_8str, plus DS:SI = the 16-colour aux
       palette. Builds the 16-entry uncmp_pal (seg004_63, CX = 1 row), then for each of
       `size` source bytes writes two pixels, high nibble first, each through uncmp_pal, to
       cmpbuf1_start:0. Out: AX = cmpbuf1_start. Note the loop counts bytes, two pixels each. */
L0039: /* _exp_4str */
    /* 0039  mov     cx,1 */
    CX = 0x1;
L003C:
    /* 003C  call    _seg004_63 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x0063), 0x003F)) != 0) return c;
L003F:
    /* 003F  mov     si,bp */
    SI = BP;
L0041:
    /* 0041  mov     dx,seg _cmpbuf1_start */
    DX = (uint16_t)(0x4423 + PORT_LOAD_SEG);
L0044:
    /* 0044  mov     es,dx */
    SET_ES(DX);
L0046:
    /* 0046  mov     di,0 */
    DI = 0x0;
L0049:
    /* 0049  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L004A:
    /* 004A  mov     cx,ax */
    CX = AX;
L004C: /* L004C */
    /* 004C  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L004D:
    /* 004D  mov     ah,al */
    AH = AL;
L004F:
    /* 004F  and     al,0F0h */
    AL = (uint8_t)(AL & 0xF0);
L0051:
    /* 0051  shr     al,4 */
    AL = (uint8_t)(AL >> 4);
L0054:
    /* 0054  xlat    byte ptr cs:_uncmp_pal[bx] */
    AL = rb(CODE004, BX + AL);
L0056:
    /* 0056  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0057:
    /* 0057  mov     al,ah */
    AL = AH;
L0059:
    /* 0059  and     al,0Fh */
    AL = logic8((uint8_t)(AL & 0xF));
L005B:
    /* 005B  xlat    byte ptr cs:_uncmp_pal[bx] */
    AL = rb(CODE004, BX + AL);
L005D:
    /* 005D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L005E:
    /* 005E  loop    L004C */
    if (--CX) goto L004C;
L0060:
    /* 0060  mov     ax,dx */
    AX = DX;
L0062:
    /* 0062  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_63  (+63)
       Builds uncmp_pal (at CS:0) for a 4-bit or 5-bit image. In: DS:SI = the aux palette, CX =
       its length in 16-byte rows (1 for 4-bit, 2 for 5-bit), AX = image paragraph, DH = the
       lightabs row or 0FFh. With DH = 0FFh the palette is copied as it is; otherwise each entry
       is passed through lightabs row DH, so the image comes out shaded for its distance. The loop
       is unrolled 16 times, one row per iteration. Out: DS = the image (from AX), BX = 0 (the
       offset of uncmp_pal, for the callers' xlat), SI past the palette, ES = SEG004_TEXT. */
L0063: /* _seg004_63 */
    /* 0063  mov     di,0 */
    DI = 0x0;
L0066:
    /* 0066  mov     bx,SEG004_TEXT */
    BX = (uint16_t)(0x06E7 + PORT_LOAD_SEG);
L0069:
    /* 0069  mov     es,bx */
    SET_ES(BX);
L006B:
    /* 006B  cmp     dh,0FFh */
    sub8(DH, 0xFF, 0);
L006E:
    /* 006E  jne     short L0079 */
    if (!ZF) goto L0079;
L0070:
    /* 0070  mov     dx,ax */
    DX = AX;
L0072:
    /* 0072  shl     cx,4 */
    CX = shl16(CX, 4);
L0075:
    /* 0075  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L0077:
    /* 0077  jmp     short L00C5 */
    goto L00C5;
L0079: /* L0079 */
    /* 0079  mov     bh,dh */
    BH = DH;
L007B:
    /* 007B  xor     bl,bl */
    BL = (uint8_t)(BL ^ BL);
L007D:
    /* 007D  add     bx,offset lightabs */
    BX = add16(BX, 0x696E, 0);
L0081:
    /* 0081  mov     dx,ax */
    DX = AX;
L0083: /* L0083 */
    /* 0083  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0084:
    /* 0084  xlat    byte ptr es:[bx] */
    AL = rb(pES, BX + AL);
L0086:
    /* 0086  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0087:
    /* 0087  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0088:
    /* 0088  xlat    byte ptr es:[bx] */
    AL = rb(pES, BX + AL);
L008A:
    /* 008A  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L008B:
    /* 008B  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L008C:
    /* 008C  xlat    byte ptr es:[bx] */
    AL = rb(pES, BX + AL);
L008E:
    /* 008E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L008F:
    /* 008F  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0090:
    /* 0090  xlat    byte ptr es:[bx] */
    AL = rb(pES, BX + AL);
L0092:
    /* 0092  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0093:
    /* 0093  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0094:
    /* 0094  xlat    byte ptr es:[bx] */
    AL = rb(pES, BX + AL);
L0096:
    /* 0096  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0097:
    /* 0097  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0098:
    /* 0098  xlat    byte ptr es:[bx] */
    AL = rb(pES, BX + AL);
L009A:
    /* 009A  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L009B:
    /* 009B  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L009C:
    /* 009C  xlat    byte ptr es:[bx] */
    AL = rb(pES, BX + AL);
L009E:
    /* 009E  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L009F:
    /* 009F  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L00A0:
    /* 00A0  xlat    byte ptr es:[bx] */
    AL = rb(pES, BX + AL);
L00A2:
    /* 00A2  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L00A3:
    /* 00A3  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L00A4:
    /* 00A4  xlat    byte ptr es:[bx] */
    AL = rb(pES, BX + AL);
L00A6:
    /* 00A6  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L00A7:
    /* 00A7  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L00A8:
    /* 00A8  xlat    byte ptr es:[bx] */
    AL = rb(pES, BX + AL);
L00AA:
    /* 00AA  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L00AB:
    /* 00AB  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L00AC:
    /* 00AC  xlat    byte ptr es:[bx] */
    AL = rb(pES, BX + AL);
L00AE:
    /* 00AE  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L00AF:
    /* 00AF  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L00B0:
    /* 00B0  xlat    byte ptr es:[bx] */
    AL = rb(pES, BX + AL);
L00B2:
    /* 00B2  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L00B3:
    /* 00B3  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L00B4:
    /* 00B4  xlat    byte ptr es:[bx] */
    AL = rb(pES, BX + AL);
L00B6:
    /* 00B6  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L00B7:
    /* 00B7  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L00B8:
    /* 00B8  xlat    byte ptr es:[bx] */
    AL = rb(pES, BX + AL);
L00BA:
    /* 00BA  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L00BB:
    /* 00BB  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L00BC:
    /* 00BC  xlat    byte ptr es:[bx] */
    AL = rb(pES, BX + AL);
L00BE:
    /* 00BE  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L00BF:
    /* 00BF  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L00C0:
    /* 00C0  xlat    byte ptr es:[bx] */
    AL = rb(pES, BX + AL);
L00C2:
    /* 00C2  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L00C3:
    /* 00C3  loop    L0083 */
    if (--CX) goto L0083;
L00C5: /* L00C5 */
    /* 00C5  mov     bx,0 */
    BX = 0x0;
L00C8:
    /* 00C8  mov     ds,dx */
    SET_DS(DX);
L00CA:
    /* 00CA  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_CB  (+CB)
       exp_8str's set-up: BX = offset lightabs + DH * 256 (the shading row), DS = AX (the image).
       The cmp's result is overwritten by the add, so DH = 0FFh is not treated specially here
       (it would index past lightabs' 16 rows); probably no caller asks for an unshaded 8-bit
       image, but that is not checked. */
L00CB: /* _seg004_CB */
    /* 00CB  cmp     dh,0FFh */
L00CE:
    /* 00CE  mov     bh,dh */
    BH = DH;
L00D0:
    /* 00D0  xor     bl,bl */
    BL = (uint8_t)(BL ^ BL);
L00D2:
    /* 00D2  add     bx,offset lightabs */
    BX = add16(BX, 0x696E, 0);
L00D6:
    /* 00D6  mov     ds,ax */
    SET_DS(AX);
L00D8:
    /* 00D8  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_D9  (+D9)
       exp_4run: format 8, 4-bit run-length. In: as exp_4str; here the size word counts 4-bit
       words (nibbles). Builds uncmp_pal, unpacks the (size + 1) / 2 bytes to one nibble per byte
       at cmpbuf1_start:0 (high nibble first, untranslated), then runs the record decoder
       seg004_18D over those `size` bytes into cmpbuf1_start:1800h, translating each pixel
       through uncmp_pal. Out: AX = cmpbuf1_start + 180h, the paragraph of the pixels. */
L00D9: /* _exp_4run */
    /* 00D9  mov     cx,1 */
    CX = 0x1;
L00DC:
    /* 00DC  call    _seg004_63 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x0063), 0x00DF)) != 0) return c;
L00DF:
    /* 00DF  mov     si,bp */
    SI = BP;
L00E1:
    /* 00E1  mov     dx,seg _cmpbuf1_start */
    DX = (uint16_t)(0x4423 + PORT_LOAD_SEG);
L00E4:
    /* 00E4  mov     es,dx */
    SET_ES(DX);
L00E6:
    /* 00E6  mov     di,0 */
    DI = 0x0;
L00E9:
    /* 00E9  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L00EA:
    /* 00EA  mov     dx,ax */
    DX = AX;
L00EC:
    /* 00EC  inc     ax */
    AX = (uint16_t)(AX + 1);
L00ED:
    /* 00ED  shr     ax,1 */
    AX = (uint16_t)(AX >> 1);
L00EF:
    /* 00EF  mov     cx,ax */
    CX = AX;
L00F1: /* L00F1 */
    /* 00F1  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L00F2:
    /* 00F2  mov     ah,al */
    AH = AL;
L00F4:
    /* 00F4  and     al,0F0h */
    AL = (uint8_t)(AL & 0xF0);
L00F6:
    /* 00F6  shr     al,4 */
    AL = (uint8_t)(AL >> 4);
L00F9:
    /* 00F9  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L00FA:
    /* 00FA  mov     al,ah */
    AL = AH;
L00FC:
    /* 00FC  and     al,0Fh */
    AL = (uint8_t)(AL & 0xF);
L00FE:
    /* 00FE  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L00FF:
    /* 00FF  loop    L00F1 */
    if (--CX) goto L00F1;
L0101:
    /* 0101  mov     cx,dx */
    CX = DX;
L0103:
    /* 0103  mov     ax,seg _cmpbuf1_start */
    AX = (uint16_t)(0x4423 + PORT_LOAD_SEG);
L0106:
    /* 0106  mov     ds,ax */
    SET_DS(AX);
L0108:
    /* 0108  mov     si,0 */
    SI = 0x0;
L010B:
    /* 010B  mov     ax,seg _cmpbuf1_start */
    AX = (uint16_t)(0x4423 + PORT_LOAD_SEG);
L010E:
    /* 010E  mov     es,ax */
    SET_ES(AX);
L0110:
    /* 0110  mov     di,1800h */
    DI = 0x1800;
L0113:
    /* 0113  call    _seg004_18D */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x018D), 0x0116)) != 0) return c;
L0116:
    /* 0116  mov     ax,es */
    AX = asm_es;
L0118:
    /* 0118  add     ax,180h */
    AX = add16(AX, 0x180, 0);
L011B:
    /* 011B  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_11C  (+11C)
       exp_5run: format 6, 5-bit run-length (UW-Formats: some critter frames). In and out as exp_4run, with a
       32-entry uncmp_pal (CX = 2 rows). Unpacks the bit stream, most significant bit first, five
       bytes into eight 5-bit words per iteration, (size + 7) / 8 iterations, with rotates rather
       than a bit loop; then decodes as exp_4run does. */
L011C: /* _exp_5run */
    /* 011C  mov     cx,2 */
    CX = 0x2;
L011F:
    /* 011F  call    _seg004_63 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x0063), 0x0122)) != 0) return c;
L0122:
    /* 0122  mov     si,bp */
    SI = BP;
L0124:
    /* 0124  mov     dx,seg _cmpbuf1_start */
    DX = (uint16_t)(0x4423 + PORT_LOAD_SEG);
L0127:
    /* 0127  mov     es,dx */
    SET_ES(DX);
L0129:
    /* 0129  mov     di,0 */
    DI = 0x0;
L012C:
    /* 012C  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L012D:
    /* 012D  mov     dx,ax */
    DX = AX;
L012F:
    /* 012F  add     ax,7 */
    AX = (uint16_t)(AX + 0x7);
L0132:
    /* 0132  shr     ax,3 */
    AX = (uint16_t)(AX >> 3);
L0135:
    /* 0135  mov     cx,ax */
    CX = AX;
L0137: /* L0137 */
    /* 0137  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0138:
    /* 0138  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L013A:
    /* 013A  ror     ax,3 */
    AX = ror16(AX, 3);
L013D:
    /* 013D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L013E:
    /* 013E  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L013F:
    /* 013F  shr     ah,5 */
    AH = (uint8_t)(AH >> 5);
L0142:
    /* 0142  ror     ax,6 */
    AX = ror16(AX, 6);
L0145:
    /* 0145  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0146:
    /* 0146  shr     ax,3 */
    AX = (uint16_t)(AX >> 3);
L0149:
    /* 0149  xchg    ah,al */
    { uint8_t t_ = AL;
    AL = AH;
    AH = t_; }
L014B:
    /* 014B  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L014C:
    /* 014C  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L014D:
    /* 014D  shr     ah,7 */
    AH = (uint8_t)(AH >> 7);
L0150:
    /* 0150  ror     ax,4 */
    AX = ror16(AX, 4);
L0153:
    /* 0153  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0154:
    /* 0154  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0155:
    /* 0155  shr     ah,4 */
    AH = (uint8_t)(AH >> 4);
L0158:
    /* 0158  ror     ax,7 */
    AX = ror16(AX, 7);
L015B:
    /* 015B  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L015C:
    /* 015C  xor     al,al */
    AL = (uint8_t)(AL ^ AL);
L015E:
    /* 015E  rol     ax,5 */
    AX = rol16(AX, 5);
L0161:
    /* 0161  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0162:
    /* 0162  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0163:
    /* 0163  shr     ah,6 */
    AH = (uint8_t)(AH >> 6);
L0166:
    /* 0166  ror     ax,5 */
    AX = ror16(AX, 5);
L0169:
    /* 0169  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L016A:
    /* 016A  shr     ax,0Bh */
    AX = (uint16_t)(AX >> 11);
L016D:
    /* 016D  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L016E:
    /* 016E  loop    L0137 */
    if (--CX) goto L0137;
L0170:
    /* 0170  mov     cx,dx */
    CX = DX;
L0172:
    /* 0172  mov     ax,seg _cmpbuf1_start */
    AX = (uint16_t)(0x4423 + PORT_LOAD_SEG);
L0175:
    /* 0175  mov     ds,ax */
    SET_DS(AX);
L0177:
    /* 0177  mov     si,0 */
    SI = 0x0;
L017A:
    /* 017A  mov     ax,seg _cmpbuf1_start */
    AX = (uint16_t)(0x4423 + PORT_LOAD_SEG);
L017D:
    /* 017D  mov     es,ax */
    SET_ES(AX);
L017F:
    /* 017F  mov     di,1800h */
    DI = 0x1800;
L0182:
    /* 0182  call    _seg004_18D */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x018D), 0x0185)) != 0) return c;
L0185:
    /* 0185  mov     ax,es */
    AX = asm_es;
L0187:
    /* 0187  add     ax,180h */
    AX = add16(AX, 0x180, 0);
L018A:
    /* 018A  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_18B  (+18B)
       exp_8run: format 2 has no decoder; it returns at once, leaving AX as it was. Its second
       `ret` (L018C) is the end-of-input exit of the record decoder below. */
L018B: /* _exp_8run */
    /* 018B  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L018C: /* L018C */
    /* 018C  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg004_18D  (+18D)
       The record decoder's entry. In: DS:SI = the unpacked words (one per byte), CX = how many,
       ES:DI = output, BX = 0 (uncmp_pal). Sets BP = SI + CX, the end of the input, and DX = 3
       (the step past a four-word count's last three words), then falls into seg004_194. */
L018D: /* _seg004_18D */
    /* 018D  mov     bp,cx */
    BP = CX;
L018F:
    /* 018F  add     bp,si */
    BP = (uint16_t)(BP + SI);
L0191:
    /* 0191  mov     dx,3 */
    DX = 0x3;

    /* seg004_194  (+194)
       The run-length record decoder: one repeat record then one run record per pass, looping
       until SI reaches BP (exit through L018C). The first word is the repeat count:
         3 up:   repeat the next word that many times;
         0:      an extended count (L01F9 path): if the next word is nonzero, count = it << 4 |
                 the word after; if it is zero, the four words after form a 16-bit count
                 (UW-Formats describes this as 0 0 0 w3 w4 w5; the code takes the third zero
                 word as the count's top nibble, which is the same when it is zero);
         1:      no repeat record this time;
         2:      (L0213) a count of repeat records follows; each is decoded by a recursive call,
                 and to make the recursive call stop after its repeat record the routine
                 patches its own code: L01A6's first byte becomes C3h (ret) for the duration and
                 is restored to 29h (the first byte of `sub ax,ax`) afterwards.
       Then the run record at L01A6: a count (extended the same way, a zero count reaching L01CE)
       and that many words copied, each through uncmp_pal. In/out registers as seg004_18D;
       DI advances over the output. */
L0194: /* _seg004_194 */
    /* 0194  cmp     si,bp */
    sub16(SI, BP, 0);
L0196:
    /* 0196  jae     L018C */
    if (!CF) goto L018C;
L0198:
    /* 0198  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L019A:
    /* 019A  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L019B:
    /* 019B  cmp     al,2 */
    sub8(AL, 0x2, 0);
L019D:
    /* 019D  jbe     short L01F9 */
    if (CF || ZF) goto L01F9;
L019F:
    /* 019F  mov     cx,ax */
    CX = AX;
L01A1:
    /* 01A1  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L01A2:
    /* 01A2  xlat    byte ptr cs:_uncmp_pal[bx] */
    AL = rb(CODE004, BX + AL);
L01A4:
    /* 01A4  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L01A6: /* L01A6 */
    /* 01A6  sub     ax,ax */
    /* by hand: L01A6: the record decoder writes a ret over the sub here (C3h) and puts it back (29h), as UW2's (UW2Decomp's tools/asm2c.py) */
    if (CODE004[0x01A6] == 0xC3) { SP = (uint16_t)(SP + 2); return ASM_RET; }
    AX = sub16(AX, AX, 0);
L01A8:
    /* 01A8  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L01A9:
    /* 01A9  mov     cx,ax */
    CX = AX;
L01AB:
    /* 01AB  jcxz    L01CE */
    if (!CX) goto L01CE;
L01AD: /* L01AD */
    /* 01AD  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L01AE:
    /* 01AE  xlat    byte ptr cs:_uncmp_pal[bx] */
    AL = rb(CODE004, BX + AL);
L01B0:
    /* 01B0  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L01B1:
    /* 01B1  loop    L01AD */
    if (--CX) goto L01AD;
L01B3:
    /* 01B3  jmp     _seg004_194 */
    goto L0194;
L01B5: /* L01B5 */
    /* 01B5  mov     cl,4 */
    CL = 0x4;
L01B7:
    /* 01B7  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L01B8:
    /* 01B8  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L01BA:
    /* 01BA  or      al,byte ptr [si] */
    AL = logic8((uint8_t)(AL | rb(pDS, SI)));
L01BC:
    /* 01BC  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L01BE:
    /* 01BE  or      al,byte ptr [si+1] */
    AL = logic8((uint8_t)(AL | rb(pDS, SI + 0x1)));
L01C1:
    /* 01C1  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L01C3:
    /* 01C3  or      al,byte ptr [si+2] */
    AL = (uint8_t)(AL | rb(pDS, SI + 0x2));
L01C6:
    /* 01C6  add     si,dx */
    SI = (uint16_t)(SI + DX);
L01C8:
    /* 01C8  mov     cx,ax */
    CX = AX;
L01CA:
    /* 01CA  xor     ah,ah */
    AH = (uint8_t)(AH ^ AH);
L01CC:
    /* 01CC  jmp     L01AD */
    goto L01AD;
L01CE: /* L01CE */
    /* 01CE  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L01CF:
    /* 01CF  test    al,al */
    logic8((uint8_t)(AL & AL));
L01D1:
    /* 01D1  je      L01B5 */
    if (ZF) goto L01B5;
L01D3:
    /* 01D3  shl     ax,4 */
    AX = (uint16_t)(AX << 4);
L01D6:
    /* 01D6  mov     cx,ax */
    CX = AX;
L01D8:
    /* 01D8  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L01D9:
    /* 01D9  or      cx,ax */
    CX = (uint16_t)(CX | AX);
L01DB:
    /* 01DB  jmp     L01AD */
    goto L01AD;
L01DD: /* L01DD */
    /* 01DD  mov     cl,4 */
    CL = 0x4;
L01DF:
    /* 01DF  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L01E0:
    /* 01E0  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L01E2:
    /* 01E2  or      al,byte ptr [si] */
    AL = logic8((uint8_t)(AL | rb(pDS, SI)));
L01E4:
    /* 01E4  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L01E6:
    /* 01E6  or      al,byte ptr [si+1] */
    AL = logic8((uint8_t)(AL | rb(pDS, SI + 0x1)));
L01E9:
    /* 01E9  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L01EB:
    /* 01EB  or      al,byte ptr [si+2] */
    AL = (uint8_t)(AL | rb(pDS, SI + 0x2));
L01EE:
    /* 01EE  add     si,dx */
    SI = (uint16_t)(SI + DX);
L01F0:
    /* 01F0  mov     cx,ax */
    CX = AX;
L01F2:
    /* 01F2  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L01F3:
    /* 01F3  xlat    byte ptr cs:_uncmp_pal[bx] */
    AL = rb(CODE004, BX + AL);
L01F5:
    /* 01F5  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L01F7:
    /* 01F7  jmp     L01A6 */
    goto L01A6;
L01F9: /* L01F9 */
    /* 01F9  je      short L0213 */
    if (ZF) goto L0213;
L01FB:
    /* 01FB  test    al,al */
    logic8((uint8_t)(AL & AL));
L01FD:
    /* 01FD  jne     short L0211 */
    if (!ZF) goto L0211;
L01FF:
    /* 01FF  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0200:
    /* 0200  test    al,al */
    logic8((uint8_t)(AL & AL));
L0202:
    /* 0202  je      L01DD */
    if (ZF) goto L01DD;
L0204:
    /* 0204  shl     ax,4 */
    AX = (uint16_t)(AX << 4);
L0207:
    /* 0207  mov     cx,ax */
    CX = AX;
L0209:
    /* 0209  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L020A:
    /* 020A  or      cx,ax */
    CX = (uint16_t)(CX | AX);
L020C:
    /* 020C  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L020D:
    /* 020D  xlat    byte ptr cs:_uncmp_pal[bx] */
    AL = rb(CODE004, BX + AL);
L020F:
    /* 020F  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L0211: /* L0211 */
    /* 0211  jmp     L01A6 */
    goto L01A6;
L0213: /* L0213 */
    /* 0213  mov     byte ptr cs:L01A6,0C3h */
    wb(CODE004, 0x1A6, 0xC3);
L0219:
    /* 0219  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L021B:
    /* 021B  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L021C:
    /* 021C  mov     cx,ax */
    CX = AX;
L021E:
    /* 021E  jcxz    L0247 */
    if (!CX) goto L0247;
L0220: /* L0220 */
    /* 0220  push    cx */
    push16(CX);
L0221:
    /* 0221  call    _seg004_194 */
    if ((c = asm_call(ASM_JMP(0x06E7, 0x0194), 0x0224)) != 0) return c;
L0224:
    /* 0224  pop     cx */
    CX = pop16();
L0225:
    /* 0225  loop    L0220 */
    if (--CX) goto L0220;
L0227:
    /* 0227  mov     byte ptr cs:L01A6,29h */
    wb(CODE004, 0x1A6, 0x29);
L022D:
    /* 022D  jmp     L01A6 */
    goto L01A6;
L0230: /* L0230 */
    /* 0230  mov     cl,4 */
    CL = 0x4;
L0232:
    /* 0232  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0233:
    /* 0233  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L0235:
    /* 0235  or      al,byte ptr [si] */
    AL = logic8((uint8_t)(AL | rb(pDS, SI)));
L0237:
    /* 0237  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L0239:
    /* 0239  or      al,byte ptr [si+1] */
    AL = logic8((uint8_t)(AL | rb(pDS, SI + 0x1)));
L023C:
    /* 023C  shl     ax,cl */
    AX = (uint16_t)(AX << (CL & 31));
L023E:
    /* 023E  or      al,byte ptr [si+2] */
    AL = (uint8_t)(AL | rb(pDS, SI + 0x2));
L0241:
    /* 0241  add     si,dx */
    SI = (uint16_t)(SI + DX);
L0243:
    /* 0243  mov     cx,ax */
    CX = AX;
L0245:
    /* 0245  jmp     L0220 */
    goto L0220;
L0247: /* L0247 */
    /* 0247  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0248:
    /* 0248  test    al,al */
    logic8((uint8_t)(AL & AL));
L024A:
    /* 024A  je      L0230 */
    if (ZF) goto L0230;
L024C:
    /* 024C  shl     ax,4 */
    AX = (uint16_t)(AX << 4);
L024F:
    /* 024F  mov     cx,ax */
    CX = AX;
L0251:
    /* 0251  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0252:
    /* 0252  or      cx,ax */
    CX = (uint16_t)(CX | AX);
L0254:
    /* 0254  jmp     L0220 */
    goto L0220;
}
