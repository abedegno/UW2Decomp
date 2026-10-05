/* valloc.c: replaces src/gfx/VALLOC.ASM (seg001, 000A..02D1 of its
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

uint32_t asm_mod_VALLOC(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x000C: goto L000C;
    case 0x000D: goto L000D;
    case 0x000E: goto L000E;
    case 0x000F: goto L000F;
    case 0x0010: goto L0010;
    case 0x0013: goto L0013;
    case 0x0015: goto L0015;
    case 0x0019: goto L0019;
    case 0x001E: goto L001E;
    case 0x001F: goto L001F;
    case 0x0020: goto L0020;
    case 0x0025: goto L0025;
    case 0x0028: goto L0028;
    case 0x002D: goto L002D;
    case 0x0030: goto L0030;
    case 0x0032: goto L0032;
    case 0x0036: goto L0036;
    case 0x0039: goto L0039;
    case 0x003C: goto L003C;
    case 0x003F: goto L003F;
    case 0x0042: goto L0042;
    case 0x0045: goto L0045;
    case 0x0046: goto L0046;
    case 0x0047: goto L0047;
    case 0x0048: goto L0048;
    case 0x0049: goto L0049;
    case 0x004A: goto L004A;
    case 0x004B: goto L004B;
    case 0x004D: goto L004D;
    case 0x0050: goto L0050;
    case 0x0053: goto L0053;
    case 0x005A: goto L005A;
    case 0x005B: goto L005B;
    case 0x005C: goto L005C;
    case 0x005D: goto L005D;
    case 0x005E: goto L005E;
    case 0x0061: goto L0061;
    case 0x0063: goto L0063;
    case 0x0065: goto L0065;
    case 0x0068: goto L0068;
    case 0x0069: goto L0069;
    case 0x006B: goto L006B;
    case 0x006E: goto L006E;
    case 0x0072: goto L0072;
    case 0x0074: goto L0074;
    case 0x0077: goto L0077;
    case 0x0079: goto L0079;
    case 0x007B: goto L007B;
    case 0x007D: goto L007D;
    case 0x0080: goto L0080;
    case 0x0084: goto L0084;
    case 0x0086: goto L0086;
    case 0x0088: goto L0088;
    case 0x008C: goto L008C;
    case 0x008E: goto L008E;
    case 0x0090: goto L0090;
    case 0x0092: goto L0092;
    case 0x0095: goto L0095;
    case 0x0096: goto L0096;
    case 0x0098: goto L0098;
    case 0x0099: goto L0099;
    case 0x009C: goto L009C;
    case 0x00A0: goto L00A0;
    case 0x00A2: goto L00A2;
    case 0x00A4: goto L00A4;
    case 0x00A6: goto L00A6;
    case 0x00AB: goto L00AB;
    case 0x00AE: goto L00AE;
    case 0x00B0: goto L00B0;
    case 0x00B3: goto L00B3;
    case 0x00B7: goto L00B7;
    case 0x00B9: goto L00B9;
    case 0x00BB: goto L00BB;
    case 0x00BE: goto L00BE;
    case 0x00C2: goto L00C2;
    case 0x00C4: goto L00C4;
    case 0x00C6: goto L00C6;
    case 0x00CA: goto L00CA;
    case 0x00CC: goto L00CC;
    case 0x00CE: goto L00CE;
    case 0x00D0: goto L00D0;
    case 0x00D4: goto L00D4;
    case 0x00D6: goto L00D6;
    case 0x00D8: goto L00D8;
    case 0x00DA: goto L00DA;
    case 0x00DE: goto L00DE;
    case 0x00E0: goto L00E0;
    case 0x00E3: goto L00E3;
    case 0x00E7: goto L00E7;
    case 0x00EC: goto L00EC;
    case 0x00EE: goto L00EE;
    case 0x00EF: goto L00EF;
    case 0x00F0: goto L00F0;
    case 0x00F1: goto L00F1;
    case 0x00F2: goto L00F2;
    case 0x00F8: goto L00F8;
    case 0x00FA: goto L00FA;
    case 0x00FC: goto L00FC;
    case 0x00FD: goto L00FD;
    case 0x0104: goto L0104;
    case 0x0105: goto L0105;
    case 0x0106: goto L0106;
    case 0x0108: goto L0108;
    case 0x010B: goto L010B;
    case 0x0112: goto L0112;
    case 0x0113: goto L0113;
    case 0x0114: goto L0114;
    case 0x0115: goto L0115;
    case 0x0116: goto L0116;
    case 0x0119: goto L0119;
    case 0x011B: goto L011B;
    case 0x011D: goto L011D;
    case 0x0120: goto L0120;
    case 0x0122: goto L0122;
    case 0x0124: goto L0124;
    case 0x0127: goto L0127;
    case 0x012B: goto L012B;
    case 0x012D: goto L012D;
    case 0x0130: goto L0130;
    case 0x0133: goto L0133;
    case 0x0135: goto L0135;
    case 0x0137: goto L0137;
    case 0x0139: goto L0139;
    case 0x013B: goto L013B;
    case 0x013F: goto L013F;
    case 0x0141: goto L0141;
    case 0x0144: goto L0144;
    case 0x0147: goto L0147;
    case 0x0149: goto L0149;
    case 0x014B: goto L014B;
    case 0x014D: goto L014D;
    case 0x0151: goto L0151;
    case 0x0156: goto L0156;
    case 0x0158: goto L0158;
    case 0x015B: goto L015B;
    case 0x015D: goto L015D;
    case 0x0161: goto L0161;
    case 0x0163: goto L0163;
    case 0x0167: goto L0167;
    case 0x0169: goto L0169;
    case 0x016C: goto L016C;
    case 0x016F: goto L016F;
    case 0x0171: goto L0171;
    case 0x0173: goto L0173;
    case 0x0178: goto L0178;
    case 0x017B: goto L017B;
    case 0x017E: goto L017E;
    case 0x0180: goto L0180;
    case 0x0182: goto L0182;
    case 0x0185: goto L0185;
    case 0x0188: goto L0188;
    case 0x018C: goto L018C;
    case 0x018E: goto L018E;
    case 0x0191: goto L0191;
    case 0x0194: goto L0194;
    case 0x0197: goto L0197;
    case 0x019B: goto L019B;
    case 0x019D: goto L019D;
    case 0x019F: goto L019F;
    case 0x01A2: goto L01A2;
    case 0x01A4: goto L01A4;
    case 0x01A8: goto L01A8;
    case 0x01AA: goto L01AA;
    case 0x01AD: goto L01AD;
    case 0x01AF: goto L01AF;
    case 0x01B3: goto L01B3;
    case 0x01B5: goto L01B5;
    case 0x01B9: goto L01B9;
    case 0x01BE: goto L01BE;
    case 0x01BF: goto L01BF;
    case 0x01C0: goto L01C0;
    case 0x01C1: goto L01C1;
    case 0x01C2: goto L01C2;
    case 0x01C8: goto L01C8;
    case 0x01CA: goto L01CA;
    case 0x01CC: goto L01CC;
    case 0x01CD: goto L01CD;
    case 0x01D4: goto L01D4;
    case 0x01D5: goto L01D5;
    case 0x01D6: goto L01D6;
    case 0x01D8: goto L01D8;
    case 0x01D9: goto L01D9;
    case 0x01DC: goto L01DC;
    case 0x01DF: goto L01DF;
    case 0x01E2: goto L01E2;
    case 0x01E5: goto L01E5;
    case 0x01E8: goto L01E8;
    case 0x01EF: goto L01EF;
    case 0x01F0: goto L01F0;
    case 0x01F1: goto L01F1;
    case 0x01F2: goto L01F2;
    case 0x01F5: goto L01F5;
    case 0x01F7: goto L01F7;
    case 0x01FA: goto L01FA;
    case 0x01FC: goto L01FC;
    case 0x01FF: goto L01FF;
    case 0x0201: goto L0201;
    case 0x0203: goto L0203;
    case 0x0206: goto L0206;
    case 0x020A: goto L020A;
    case 0x020C: goto L020C;
    case 0x020F: goto L020F;
    case 0x0211: goto L0211;
    case 0x0214: goto L0214;
    case 0x0217: goto L0217;
    case 0x021A: goto L021A;
    case 0x021D: goto L021D;
    case 0x021F: goto L021F;
    case 0x0220: goto L0220;
    case 0x0222: goto L0222;
    case 0x0225: goto L0225;
    case 0x0226: goto L0226;
    case 0x0227: goto L0227;
    case 0x0228: goto L0228;
    case 0x0229: goto L0229;
    case 0x022A: goto L022A;
    case 0x022F: goto L022F;
    case 0x0232: goto L0232;
    case 0x0237: goto L0237;
    case 0x023A: goto L023A;
    case 0x023F: goto L023F;
    case 0x0242: goto L0242;
    case 0x0244: goto L0244;
    case 0x0245: goto L0245;
    case 0x0246: goto L0246;
    case 0x0247: goto L0247;
    case 0x024D: goto L024D;
    case 0x024F: goto L024F;
    case 0x0250: goto L0250;
    case 0x0252: goto L0252;
    case 0x0253: goto L0253;
    case 0x025A: goto L025A;
    case 0x025B: goto L025B;
    case 0x025C: goto L025C;
    case 0x025E: goto L025E;
    case 0x0261: goto L0261;
    case 0x0268: goto L0268;
    case 0x0269: goto L0269;
    case 0x026A: goto L026A;
    case 0x026B: goto L026B;
    case 0x026E: goto L026E;
    case 0x0270: goto L0270;
    case 0x0273: goto L0273;
    case 0x0275: goto L0275;
    case 0x0278: goto L0278;
    case 0x027C: goto L027C;
    case 0x027E: goto L027E;
    case 0x0280: goto L0280;
    case 0x0283: goto L0283;
    case 0x0285: goto L0285;
    case 0x0287: goto L0287;
    case 0x028A: goto L028A;
    case 0x028C: goto L028C;
    case 0x0290: goto L0290;
    case 0x0293: goto L0293;
    case 0x0298: goto L0298;
    case 0x029B: goto L029B;
    case 0x029E: goto L029E;
    case 0x02A1: goto L02A1;
    case 0x02A3: goto L02A3;
    case 0x02A6: goto L02A6;
    case 0x02A7: goto L02A7;
    case 0x02A9: goto L02A9;
    case 0x02AC: goto L02AC;
    case 0x02AD: goto L02AD;
    case 0x02AE: goto L02AE;
    case 0x02AF: goto L02AF;
    case 0x02B0: goto L02B0;
    case 0x02B1: goto L02B1;
    case 0x02B6: goto L02B6;
    case 0x02B9: goto L02B9;
    case 0x02BB: goto L02BB;
    case 0x02BC: goto L02BC;
    case 0x02BD: goto L02BD;
    case 0x02BE: goto L02BE;
    case 0x02C4: goto L02C4;
    case 0x02C6: goto L02C6;
    case 0x02C8: goto L02C8;
    case 0x02C9: goto L02C9;
    case 0x02D0: goto L02D0;
    default: asm_bad_entry("VALLOC.ASM", entry);
    }

    /* seg001_023B_C  (+C): void seg001_023B_C(void)
       Initialises the pool: claims the graphics library's free video memory (all of it but
       one byte) and sets the pool to run from what that returns up to the library's limit,
       with one block record (the first, a sentinel for vfree's merge with the block before). */
L000C: /* _seg001_023B_C */
    /* 000C  push    ds */
    push16(asm_ds);
L000D:
    /* 000D  push    es */
    push16(asm_es);
L000E:
    /* 000E  push    si */
    push16(SI);
L000F:
    /* 000F  push    di */
    push16(DI);
L0010:
    /* 0010  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L0013:
    /* 0013  mov     es,ax */
    SET_ES(AX);
L0015:
    /* 0015  mov     ax,word ptr es:[GR_VLIMIT] */
    AX = rw(pES, 0x4106);
L0019:
    /* 0019  sub     ax,word ptr es:[GR_VFREE] */
    AX = (uint16_t)(AX - rw(pES, 0x4108));
L001E:
    /* 001E  dec     ax */
    AX = (uint16_t)(AX - 1);
L001F:
    /* 001F  push    ax */
    push16(AX);
L0020:
    /* 0020  call    far ptr _seg003_522E */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x522E), 0x004E + PORT_LOAD_SEG, 0x0025)) != 0) return c;
L0025:
    /* 0025  add     sp,2 */
    SP = (uint16_t)(SP + 0x2);
L0028:
    /* 0028  mov     bx,word ptr es:[GR_VLIMIT] */
    BX = rw(pES, 0x4106);
L002D:
    /* 002D  mov     cx,seg seg055 */
    CX = (uint16_t)(0x54D7 + PORT_LOAD_SEG);
L0030:
    /* 0030  mov     ds,cx */
    SET_DS(CX);
L0032:
    /* 0032  mov     word ptr ds:[POOL_END],bx */
    ww(pDS, 0xE, BX);
L0036:
    /* 0036  mov     word ptr ds:[POOL_START],ax */
    ww(pDS, 0xC, AX);
L0039:
    /* 0039  mov     word ptr ds:[POOL_TOP],ax */
    ww(pDS, 0x10, AX);
L003C:
    /* 003C  mov     ax,RECS */
    AX = 0x14;
L003F:
    /* 003F  add     ax,0Dh */
    AX = add16(AX, 0xD, 0);
L0042:
    /* 0042  mov     word ptr ds:[REC_END],ax */
    ww(pDS, 0x12, AX);
L0045:
    /* 0045  pop     di */
    DI = pop16();
L0046:
    /* 0046  pop     si */
    SI = pop16();
L0047:
    /* 0047  pop     es */
    SET_ES(pop16());
L0048:
    /* 0048  pop     ds */
    SET_DS(pop16());
L0049:
    /* 0049  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg001_4A  (+4A): int valloc(int w, int h)
       Register entry seg001_5A: AX = w, BX = h. Returns AX = the block's video offset,
       or 0 when the pool is full or a split finds the record table full (its end at 104Ch;
       only the split path checks it, after it has already moved the records up). Size is
       (w / 4 + 1) * h.
       First fit: a free record of exactly that size is reused; a larger free record is split,
       the records after it moved up to make room for the remainder; failing both, a new block
       comes from the bump pointer. */
L004A: /* _valloc */
    /* 004A  push    bp */
    push16(BP);
L004B:
    /* 004B  mov     bp,sp */
    BP = SP;
L004D:
    /* 004D  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L0050:
    /* 0050  mov     bx,word ptr [bp+8] */
    BX = rw(pSS, BP + 0x8);
L0053:
    /* 0053  mov     word ptr cs:c_entry,1 */
    ww(CODE001, 0xA, 0x1);
L005A: /* _seg001_5A */
    /* 005A  push    ds */
    push16(asm_ds);
L005B:
    /* 005B  push    es */
    push16(asm_es);
L005C:
    /* 005C  push    si */
    push16(SI);
L005D:
    /* 005D  push    di */
    push16(DI);
L005E:
    /* 005E  mov     cx,seg seg055 */
    CX = (uint16_t)(0x54D7 + PORT_LOAD_SEG);
L0061:
    /* 0061  mov     ds,cx */
    SET_DS(CX);
L0063:
    /* 0063  mov     es,cx */
    SET_ES(CX);
L0065:
    /* 0065  shr     ax,2 */
    AX = (uint16_t)(AX >> 2);
L0068:
    /* 0068  inc     ax */
    AX = (uint16_t)(AX + 1);
L0069:
    /* 0069  mul     bx */
    mul16(BX);
L006B:
    /* 006B  mov     bx,RECS */
    BX = 0x14;
L006E: /* L006E */
    /* 006E  cmp     byte ptr [bx+4],0 */
    sub8(rb(pDS, BX + 0x4), 0x0, 0);
L0072:
    /* 0072  je      short L007D */
    if (ZF) goto L007D;
L0074:
    /* 0074  mov     dx,word ptr [bx+2] */
    DX = rw(pDS, BX + 0x2);
L0077:
    /* 0077  cmp     ax,dx */
    sub16(AX, DX, 0);
L0079:
    /* 0079  je      short L00BE */
    if (ZF) goto L00BE;
L007B:
    /* 007B  jb      short L0088 */
    if (CF) goto L0088;
L007D: /* L007D */
    /* 007D  add     bx,0Dh */
    BX = (uint16_t)(BX + 0xD);
L0080:
    /* 0080  cmp     bx,word ptr ds:[REC_END] */
    sub16(BX, rw(pDS, 0x12), 0);
L0084:
    /* 0084  jb      L006E */
    if (CF) goto L006E;
L0086:
    /* 0086  jmp     short L00C6 */
    goto L00C6;
L0088: /* L0088 */
    /* 0088  mov     cx,word ptr ds:[REC_END] */
    CX = rw(pDS, 0x12);
L008C:
    /* 008C  mov     si,cx */
    SI = CX;
L008E:
    /* 008E  sub     cx,bx */
    CX = (uint16_t)(CX - BX);
L0090:
    /* 0090  mov     di,si */
    DI = SI;
L0092:
    /* 0092  add     di,0Dh */
    DI = (uint16_t)(DI + 0xD);
L0095:
    /* 0095  std */
    DF = 1;
L0096:
    /* 0096  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L0098:
    /* 0098  cld */
    DF = 0;
L0099:
    /* 0099  mov     cx,RECS_END */
    CX = 0x1054;
L009C:
    /* 009C  cmp     word ptr ds:[REC_END],cx */
    sub16(rw(pDS, 0x12), CX, 0);
L00A0:
    /* 00A0  jb      short L00A6 */
    if (CF) goto L00A6;
L00A2:
    /* 00A2  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L00A4:
    /* 00A4  jmp     short L00EE */
    goto L00EE;
L00A6: /* L00A6 */
    /* 00A6  add     word ptr ds:[REC_END],0Dh */
    ww(pDS, 0x12, (uint16_t)(rw(pDS, 0x12) + 0xD));
L00AB:
    /* 00AB  mov     word ptr [bx+2],ax */
    ww(pDS, BX + 0x2, AX);
L00AE:
    /* 00AE  sub     dx,ax */
    DX = (uint16_t)(DX - AX);
L00B0:
    /* 00B0  mov     word ptr [bx+0Fh],dx */
    ww(pDS, BX + 0xF, DX);
L00B3:
    /* 00B3  mov     byte ptr [bx+11h],1 */
    wb(pDS, BX + 0x11, 0x1);
L00B7:
    /* 00B7  mov     si,word ptr [bx] */
    SI = rw(pDS, BX);
L00B9:
    /* 00B9  add     ax,si */
    AX = (uint16_t)(AX + SI);
L00BB:
    /* 00BB  mov     word ptr [bx+0Dh],ax */
    ww(pDS, BX + 0xD, AX);
L00BE: /* L00BE */
    /* 00BE  mov     byte ptr [bx+4],0 */
    wb(pDS, BX + 0x4, 0x0);
L00C2:
    /* 00C2  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L00C4:
    /* 00C4  jmp     short L00EE */
    goto L00EE;
L00C6: /* L00C6 */
    /* 00C6  mov     dx,word ptr ds:[POOL_TOP] */
    DX = rw(pDS, 0x10);
L00CA:
    /* 00CA  mov     cx,dx */
    CX = DX;
L00CC:
    /* 00CC  add     cx,ax */
    CX = add16(CX, AX, 0);
L00CE:
    /* 00CE  jb      short L00D6 */
    if (CF) goto L00D6;
L00D0:
    /* 00D0  cmp     cx,word ptr ds:[POOL_END] */
    sub16(CX, rw(pDS, 0xE), 0);
L00D4:
    /* 00D4  jbe     short L00DA */
    if (CF || ZF) goto L00DA;
L00D6: /* L00D6 */
    /* 00D6  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L00D8:
    /* 00D8  jmp     short L00EE */
    goto L00EE;
L00DA: /* L00DA */
    /* 00DA  mov     word ptr ds:[POOL_TOP],cx */
    ww(pDS, 0x10, CX);
L00DE:
    /* 00DE  mov     word ptr [bx],dx */
    ww(pDS, BX, DX);
L00E0:
    /* 00E0  mov     word ptr [bx+2],ax */
    ww(pDS, BX + 0x2, AX);
L00E3:
    /* 00E3  mov     byte ptr [bx+4],0 */
    wb(pDS, BX + 0x4, 0x0);
L00E7:
    /* 00E7  add     word ptr ds:[REC_END],0Dh */
    ww(pDS, 0x12, (uint16_t)(rw(pDS, 0x12) + 0xD));
L00EC:
    /* 00EC  mov     ax,word ptr [bx] */
    AX = rw(pDS, BX);
L00EE: /* L00EE */
    /* 00EE  pop     di */
    DI = pop16();
L00EF:
    /* 00EF  pop     si */
    SI = pop16();
L00F0:
    /* 00F0  pop     es */
    SET_ES(pop16());
L00F1:
    /* 00F1  pop     ds */
    SET_DS(pop16());
L00F2:
    /* 00F2  cmp     word ptr cs:c_entry,1 */
    sub16(rw(CODE001, 0xA), 0x1, 0);
L00F8:
    /* 00F8  jne     short L00FD */
    if (!ZF) goto L00FD;
L00FA:
    /* 00FA  mov     sp,bp */
    SP = BP;
L00FC:
    /* 00FC  pop     bp */
    BP = pop16();
L00FD: /* L00FD */
    /* 00FD  mov     word ptr cs:c_entry,0 */
    ww(CODE001, 0xA, 0x0);
L0104:
    /* 0104  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg001_105  (+105): int vfree(int block)
       Register entry L0112 (_vfree+0Dh, which SPRITE.ASM calls): AX = the block's offset.
       Returns 1 if no record has that offset, else 0. Merges the block with a free record
       just before or after it (removing the merged records), else marks it free; a block
       that ends at the bump pointer is returned to it. */
L0105: /* _vfree */
    /* 0105  push    bp */
    push16(BP);
L0106:
    /* 0106  mov     bp,sp */
    BP = SP;
L0108:
    /* 0108  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L010B:
    /* 010B  mov     word ptr cs:c_entry,1 */
    ww(CODE001, 0xA, 0x1);
L0112: /* L0112 */
    /* 0112  push    ds */
    push16(asm_ds);
L0113:
    /* 0113  push    es */
    push16(asm_es);
L0114:
    /* 0114  push    si */
    push16(SI);
L0115:
    /* 0115  push    di */
    push16(DI);
L0116:
    /* 0116  mov     bx,seg seg055 */
    BX = (uint16_t)(0x54D7 + PORT_LOAD_SEG);
L0119:
    /* 0119  mov     ds,bx */
    SET_DS(BX);
L011B:
    /* 011B  mov     es,bx */
    SET_ES(BX);
L011D:
    /* 011D  mov     bx,RECS */
    BX = 0x14;
L0120: /* L0120 */
    /* 0120  cmp     word ptr [bx],ax */
    sub16(rw(pDS, BX), AX, 0);
L0122:
    /* 0122  je      short L0133 */
    if (ZF) goto L0133;
L0124:
    /* 0124  add     bx,0Dh */
    BX = (uint16_t)(BX + 0xD);
L0127:
    /* 0127  cmp     bx,word ptr ds:[REC_END] */
    sub16(BX, rw(pDS, 0x12), 0);
L012B:
    /* 012B  jne     L0120 */
    if (!ZF) goto L0120;
L012D:
    /* 012D  mov     ax,1 */
    AX = 0x1;
L0130:
    /* 0130  jmp     L01BE */
    goto L01BE;
L0133: /* L0133 */
    /* 0133  mov     si,bx */
    SI = BX;
L0135:
    /* 0135  mov     di,bx */
    DI = BX;
L0137:
    /* 0137  xor     bx,bx */
    BX = (uint16_t)(BX ^ BX);
L0139:
    /* 0139  xor     cx,cx */
    CX = (uint16_t)(CX ^ CX);
L013B:
    /* 013B  cmp     byte ptr [di-9],0 */
    sub8(rb(pDS, DI + 0xFFF7), 0x0, 0);
L013F:
    /* 013F  je      short L015D */
    if (ZF) goto L015D;
L0141:
    /* 0141  mov     dx,word ptr [di-0Dh] */
    DX = rw(pDS, DI + 0xFFF3);
L0144:
    /* 0144  mov     bx,word ptr [di-0Bh] */
    BX = rw(pDS, DI + 0xFFF5);
L0147:
    /* 0147  add     dx,bx */
    DX = (uint16_t)(DX + BX);
L0149:
    /* 0149  cmp     ax,dx */
    sub16(AX, DX, 0);
L014B:
    /* 014B  jne     short L015D */
    if (!ZF) goto L015D;
L014D:
    /* 014D  mov     cx,word ptr ds:[REC_END] */
    CX = rw(pDS, 0x12);
L0151:
    /* 0151  sub     word ptr ds:[REC_END],0Dh */
    ww(pDS, 0x12, (uint16_t)(rw(pDS, 0x12) - 0xD));
L0156:
    /* 0156  sub     cx,di */
    CX = (uint16_t)(CX - DI);
L0158:
    /* 0158  add     si,0Dh */
    SI = (uint16_t)(SI + 0xD);
L015B:
    /* 015B  jmp     short L0163 */
    goto L0163;
L015D: /* L015D */
    /* 015D  cmp     byte ptr [di+11h],0 */
    sub8(rb(pDS, DI + 0x11), 0x0, 0);
L0161:
    /* 0161  je      short L01A4 */
    if (ZF) goto L01A4;
L0163: /* L0163 */
    /* 0163  cmp     byte ptr [di+11h],0 */
    sub8(rb(pDS, DI + 0x11), 0x0, 0);
L0167:
    /* 0167  je      short L0191 */
    if (ZF) goto L0191;
L0169:
    /* 0169  add     ax,word ptr [di+2] */
    AX = (uint16_t)(AX + rw(pDS, DI + 0x2));
L016C:
    /* 016C  mov     dx,word ptr [di+0Dh] */
    DX = rw(pDS, DI + 0xD);
L016F:
    /* 016F  cmp     ax,dx */
    sub16(AX, DX, 0);
L0171:
    /* 0171  jne     short L0191 */
    if (!ZF) goto L0191;
L0173:
    /* 0173  sub     word ptr ds:[REC_END],0Dh */
    ww(pDS, 0x12, (uint16_t)(rw(pDS, 0x12) - 0xD));
L0178:
    /* 0178  add     bx,word ptr [di+0Fh] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0xF));
L017B:
    /* 017B  add     si,0Dh */
    SI = (uint16_t)(SI + 0xD);
L017E:
    /* 017E  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L0180:
    /* 0180  jne     short L0191 */
    if (!ZF) goto L0191;
L0182:
    /* 0182  mov     bx,word ptr [di+2] */
    BX = rw(pDS, DI + 0x2);
L0185:
    /* 0185  add     si,0Dh */
    SI = (uint16_t)(SI + 0xD);
L0188:
    /* 0188  mov     cx,word ptr ds:[REC_END] */
    CX = rw(pDS, 0x12);
L018C:
    /* 018C  sub     cx,di */
    CX = (uint16_t)(CX - DI);
L018E:
    /* 018E  add     di,0Dh */
    DI = (uint16_t)(DI + 0xD);
L0191: /* L0191 */
    /* 0191  add     bx,word ptr [di+2] */
    BX = (uint16_t)(BX + rw(pDS, DI + 0x2));
L0194:
    /* 0194  mov     word ptr [di-0Bh],bx */
    ww(pDS, DI + 0xFFF5, BX);
L0197:
    /* 0197  mov     byte ptr [di-9],1 */
    wb(pDS, DI + 0xFFF7, 0x1);
L019B:
    /* 019B  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L019D:
    /* 019D  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L019F:
    /* 019F  sub     di,0Dh */
    DI = (uint16_t)(DI - 0xD);
L01A2:
    /* 01A2  jmp     short L01A8 */
    goto L01A8;
L01A4: /* L01A4 */
    /* 01A4  mov     byte ptr [di+4],1 */
    wb(pDS, DI + 0x4, 0x1);
L01A8: /* L01A8 */
    /* 01A8  mov     bx,word ptr [di] */
    BX = rw(pDS, DI);
L01AA:
    /* 01AA  mov     cx,word ptr [di+2] */
    CX = rw(pDS, DI + 0x2);
L01AD:
    /* 01AD  add     bx,cx */
    BX = (uint16_t)(BX + CX);
L01AF:
    /* 01AF  cmp     bx,word ptr ds:[POOL_TOP] */
    sub16(BX, rw(pDS, 0x10), 0);
L01B3:
    /* 01B3  jne     short L01BE */
    if (!ZF) goto L01BE;
L01B5:
    /* 01B5  sub     word ptr ds:[POOL_TOP],cx */
    ww(pDS, 0x10, (uint16_t)(rw(pDS, 0x10) - CX));
L01B9:
    /* 01B9  sub     word ptr ds:[REC_END],0Dh */
    ww(pDS, 0x12, (uint16_t)(rw(pDS, 0x12) - 0xD));
L01BE: /* L01BE */
    /* 01BE  pop     di */
    DI = pop16();
L01BF:
    /* 01BF  pop     si */
    SI = pop16();
L01C0:
    /* 01C0  pop     es */
    SET_ES(pop16());
L01C1:
    /* 01C1  pop     ds */
    SET_DS(pop16());
L01C2:
    /* 01C2  cmp     word ptr cs:c_entry,1 */
    sub16(rw(CODE001, 0xA), 0x1, 0);
L01C8:
    /* 01C8  jne     short L01CD */
    if (!ZF) goto L01CD;
L01CA:
    /* 01CA  mov     sp,bp */
    SP = BP;
L01CC:
    /* 01CC  pop     bp */
    BP = pop16();
L01CD: /* L01CD */
    /* 01CD  mov     word ptr cs:c_entry,0 */
    ww(CODE001, 0xA, 0x0);
L01D4:
    /* 01D4  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg001_1D5  (+1D5): int save_rect(int block, int x, int y, int w, int h)
       Register entry save_rect: SI block, AX x, BX y (the top row, bottom-up), CX w, DX h.
       Records the rectangle in the block's record and copies the screen's x, y .. x+w-1,
       y-h+1 into the block (pen 109h). Returns 0, or 1 if the block is unknown. */
L01D5: /* _save_rect */
    /* 01D5  push    bp */
    push16(BP);
L01D6:
    /* 01D6  mov     bp,sp */
    BP = SP;
L01D8:
    /* 01D8  push    si */
    push16(SI);
L01D9:
    /* 01D9  mov     si,word ptr [bp+6] */
    SI = rw(pSS, BP + 0x6);
L01DC:
    /* 01DC  mov     ax,word ptr [bp+8] */
    AX = rw(pSS, BP + 0x8);
L01DF:
    /* 01DF  mov     bx,word ptr [bp+0Ah] */
    BX = rw(pSS, BP + 0xA);
L01E2:
    /* 01E2  mov     cx,word ptr [bp+0Ch] */
    CX = rw(pSS, BP + 0xC);
L01E5:
    /* 01E5  mov     dx,word ptr [bp+0Eh] */
    DX = rw(pSS, BP + 0xE);
L01E8:
    /* 01E8  mov     word ptr cs:c_entry,1 */
    ww(CODE001, 0xA, 0x1);
L01EF: /* save_rect */
    /* 01EF  push    ds */
    push16(asm_ds);
L01F0:
    /* 01F0  push    es */
    push16(asm_es);
L01F1:
    /* 01F1  push    di */
    push16(DI);
L01F2:
    /* 01F2  mov     di,seg seg055 */
    DI = (uint16_t)(0x54D7 + PORT_LOAD_SEG);
L01F5:
    /* 01F5  mov     ds,di */
    SET_DS(DI);
L01F7:
    /* 01F7  mov     di,seg seg048 */
    DI = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L01FA:
    /* 01FA  mov     es,di */
    SET_ES(DI);
L01FC:
    /* 01FC  mov     di,RECS */
    DI = 0x14;
L01FF: /* L01FF */
    /* 01FF  cmp     word ptr [di],si */
    sub16(rw(pDS, DI), SI, 0);
L0201:
    /* 0201  je      short L0211 */
    if (ZF) goto L0211;
L0203:
    /* 0203  add     di,0Dh */
    DI = (uint16_t)(DI + 0xD);
L0206:
    /* 0206  cmp     di,word ptr ds:[REC_END] */
    sub16(DI, rw(pDS, 0x12), 0);
L020A:
    /* 020A  jb      L01FF */
    if (CF) goto L01FF;
L020C:
    /* 020C  mov     ax,1 */
    AX = 0x1;
L020F:
    /* 020F  jmp     short L0244 */
    goto L0244;
L0211: /* L0211 */
    /* 0211  mov     word ptr [di+5],ax */
    ww(pDS, DI + 0x5, AX);
L0214:
    /* 0214  mov     word ptr [di+7],bx */
    ww(pDS, DI + 0x7, BX);
L0217:
    /* 0217  mov     word ptr [di+9],cx */
    ww(pDS, DI + 0x9, CX);
L021A:
    /* 021A  mov     word ptr [di+0Bh],dx */
    ww(pDS, DI + 0xB, DX);
L021D:
    /* 021D  add     cx,ax */
    CX = (uint16_t)(CX + AX);
L021F:
    /* 021F  dec     cx */
    CX = (uint16_t)(CX - 1);
L0220:
    /* 0220  mov     dx,bx */
    DX = BX;
L0222:
    /* 0222  sub     dx,word ptr [di+0Bh] */
    DX = (uint16_t)(DX - rw(pDS, DI + 0xB));
L0225:
    /* 0225  inc     dx */
    DX = (uint16_t)(DX + 1);
L0226:
    /* 0226  push    dx */
    push16(DX);
L0227:
    /* 0227  push    cx */
    push16(CX);
L0228:
    /* 0228  push    bx */
    push16(BX);
L0229:
    /* 0229  push    ax */
    push16(AX);
L022A:
    /* 022A  mov     word ptr es:[646h],si */
    ww(pES, 0x646, SI);
L022F:
    /* 022F  push    109h */
    push16(0x109);
L0232:
    /* 0232  call    far ptr _set_the_color */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x5174), 0x004E + PORT_LOAD_SEG, 0x0237)) != 0) return c;
L0237:
    /* 0237  add     sp,2 */
    SP = (uint16_t)(SP + 0x2);
L023A:
    /* 023A  call    far ptr _rectangle */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x54EB), 0x004E + PORT_LOAD_SEG, 0x023F)) != 0) return c;
L023F:
    /* 023F  add     sp,8 */
    SP = (uint16_t)(SP + 0x8);
L0242:
    /* 0242  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L0244: /* L0244 */
    /* 0244  pop     di */
    DI = pop16();
L0245:
    /* 0245  pop     es */
    SET_ES(pop16());
L0246:
    /* 0246  pop     ds */
    SET_DS(pop16());
L0247:
    /* 0247  cmp     word ptr cs:c_entry,1 */
    sub16(rw(CODE001, 0xA), 0x1, 0);
L024D:
    /* 024D  jne     short L0253 */
    if (!ZF) goto L0253;
L024F:
    /* 024F  pop     si */
    SI = pop16();
L0250:
    /* 0250  mov     sp,bp */
    SP = BP;
L0252:
    /* 0252  pop     bp */
    BP = pop16();
L0253: /* L0253 */
    /* 0253  mov     word ptr cs:c_entry,0 */
    ww(CODE001, 0xA, 0x0);
L025A:
    /* 025A  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* MaybeRedrawSlot_seg001_25B  (+25B): int restore_rect(int block)
       Register entry restore_rect: AX block. Copies the block back to the rectangle recorded
       by save_rect (pen 10Ah). Returns 0, or 1 if the block is unknown. */
L025B: /* _restore_rect */
    /* 025B  push    bp */
    push16(BP);
L025C:
    /* 025C  mov     bp,sp */
    BP = SP;
L025E:
    /* 025E  mov     ax,word ptr [bp+6] */
    AX = rw(pSS, BP + 0x6);
L0261:
    /* 0261  mov     word ptr cs:c_entry,1 */
    ww(CODE001, 0xA, 0x1);
L0268: /* restore_rect */
    /* 0268  push    ds */
    push16(asm_ds);
L0269:
    /* 0269  push    si */
    push16(SI);
L026A:
    /* 026A  push    di */
    push16(DI);
L026B:
    /* 026B  mov     dx,seg seg055 */
    DX = (uint16_t)(0x54D7 + PORT_LOAD_SEG);
L026E:
    /* 026E  mov     ds,dx */
    SET_DS(DX);
L0270:
    /* 0270  mov     dx,seg seg048 */
    DX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L0273:
    /* 0273  mov     es,dx */
    SET_ES(DX);
L0275:
    /* 0275  mov     si,RECS */
    SI = 0x14;
L0278:
    /* 0278  mov     cx,word ptr ds:[REC_END] */
    CX = rw(pDS, 0x12);
L027C: /* L027C */
    /* 027C  cmp     word ptr [si],ax */
    sub16(rw(pDS, SI), AX, 0);
L027E:
    /* 027E  je      short L028C */
    if (ZF) goto L028C;
L0280:
    /* 0280  add     si,0Dh */
    SI = (uint16_t)(SI + 0xD);
L0283:
    /* 0283  cmp     si,cx */
    sub16(SI, CX, 0);
L0285:
    /* 0285  jb      L027C */
    if (CF) goto L027C;
L0287:
    /* 0287  mov     ax,1 */
    AX = 0x1;
L028A:
    /* 028A  jmp     short L02BB */
    goto L02BB;
L028C: /* L028C */
    /* 028C  mov     word ptr es:[648h],ax */
    ww(pES, 0x648, AX);
L0290:
    /* 0290  push    10Ah */
    push16(0x10A);
L0293:
    /* 0293  call    far ptr _set_the_color */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x5174), 0x004E + PORT_LOAD_SEG, 0x0298)) != 0) return c;
L0298:
    /* 0298  add     sp,2 */
    SP = (uint16_t)(SP + 0x2);
L029B:
    /* 029B  mov     ax,word ptr [si+5] */
    AX = rw(pDS, SI + 0x5);
L029E:
    /* 029E  mov     bx,word ptr [si+7] */
    BX = rw(pDS, SI + 0x7);
L02A1:
    /* 02A1  mov     cx,ax */
    CX = AX;
L02A3:
    /* 02A3  add     cx,word ptr [si+9] */
    CX = (uint16_t)(CX + rw(pDS, SI + 0x9));
L02A6:
    /* 02A6  dec     cx */
    CX = (uint16_t)(CX - 1);
L02A7:
    /* 02A7  mov     dx,bx */
    DX = BX;
L02A9:
    /* 02A9  sub     dx,word ptr [si+0Bh] */
    DX = (uint16_t)(DX - rw(pDS, SI + 0xB));
L02AC:
    /* 02AC  inc     dx */
    DX = (uint16_t)(DX + 1);
L02AD:
    /* 02AD  push    dx */
    push16(DX);
L02AE:
    /* 02AE  push    cx */
    push16(CX);
L02AF:
    /* 02AF  push    bx */
    push16(BX);
L02B0:
    /* 02B0  push    ax */
    push16(AX);
L02B1:
    /* 02B1  call    far ptr _rectangle */
    if ((c = asm_callf(ASM_JMP(0x0090, 0x54EB), 0x004E + PORT_LOAD_SEG, 0x02B6)) != 0) return c;
L02B6:
    /* 02B6  add     sp,8 */
    SP = (uint16_t)(SP + 0x8);
L02B9:
    /* 02B9  xor     ax,ax */
    AX = (uint16_t)(AX ^ AX);
L02BB: /* L02BB */
    /* 02BB  pop     di */
    DI = pop16();
L02BC:
    /* 02BC  pop     si */
    SI = pop16();
L02BD:
    /* 02BD  pop     ds */
    SET_DS(pop16());
L02BE:
    /* 02BE  cmp     word ptr cs:c_entry,1 */
    sub16(rw(CODE001, 0xA), 0x1, 0);
L02C4:
    /* 02C4  jne     short L02C9 */
    if (!ZF) goto L02C9;
L02C6:
    /* 02C6  mov     sp,bp */
    SP = BP;
L02C8:
    /* 02C8  pop     bp */
    BP = pop16();
L02C9: /* L02C9 */
    /* 02C9  mov     word ptr cs:c_entry,0 */
    ww(CODE001, 0xA, 0x0);
L02D0:
    /* 02D0  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;
}
