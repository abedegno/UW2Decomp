/* grentry_x.c: replaces src/gfx/GRENTRY.ASM (seg003_0272_6E2, 06E2..0D1E of its
   segment). Written by tools/asm2c.py from the assembly and the bytes it assembles to; do not
   edit by hand: change the tool, or its OVERRIDES, and run it again. Each instruction is the
   C below its source line; the comments before a routine are the .ASM file's. */
#include "x86/asmrt.h"

int port_hole_mark(void);                /* gfx/holeprobe.c */
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

uint32_t asm_mod_GRENTRY(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x06E2: goto L06E2;
    case 0x06E4: goto L06E4;
    case 0x06E6: goto L06E6;
    case 0x06E7: goto L06E7;
    case 0x06EA: goto L06EA;
    case 0x06EC: goto L06EC;
    case 0x06EF: goto L06EF;
    case 0x06F1: goto L06F1;
    case 0x06F4: goto L06F4;
    case 0x06F6: goto L06F6;
    case 0x06F7: goto L06F7;
    case 0x06F8: goto L06F8;
    case 0x06F9: goto L06F9;
    case 0x06FA: goto L06FA;
    case 0x06FF: goto L06FF;
    case 0x0704: goto L0704;
    case 0x0706: goto L0706;
    case 0x0708: goto L0708;
    case 0x070A: goto L070A;
    case 0x070C: goto L070C;
    case 0x0711: goto L0711;
    case 0x0713: goto L0713;
    case 0x0715: goto L0715;
    case 0x0717: goto L0717;
    case 0x0719: goto L0719;
    case 0x071B: goto L071B;
    case 0x071C: goto L071C;
    case 0x071E: goto L071E;
    case 0x0720: goto L0720;
    case 0x0721: goto L0721;
    case 0x0723: goto L0723;
    case 0x0724: goto L0724;
    case 0x0726: goto L0726;
    case 0x0727: goto L0727;
    case 0x0728: goto L0728;
    case 0x0764: goto L0764;
    case 0x0765: goto L0765;
    case 0x0766: goto L0766;
    case 0x0769: goto L0769;
    case 0x076B: goto L076B;
    case 0x076E: goto L076E;
    case 0x0770: goto L0770;
    case 0x0772: goto L0772;
    case 0x0774: goto L0774;
    case 0x0777: goto L0777;
    case 0x0779: goto L0779;
    case 0x077B: goto L077B;
    case 0x0780: goto L0780;
    case 0x0782: goto L0782;
    case 0x0783: goto L0783;
    case 0x0785: goto L0785;
    case 0x0786: goto L0786;
    case 0x0787: goto L0787;
    case 0x0788: goto L0788;
    case 0x0789: goto L0789;
    case 0x078C: goto L078C;
    case 0x078E: goto L078E;
    case 0x0791: goto L0791;
    case 0x0793: goto L0793;
    case 0x0795: goto L0795;
    case 0x079A: goto L079A;
    case 0x079C: goto L079C;
    case 0x079D: goto L079D;
    case 0x079F: goto L079F;
    case 0x07A0: goto L07A0;
    case 0x07A2: goto L07A2;
    case 0x07A3: goto L07A3;
    case 0x09C9: goto L09C9;
    case 0x09CA: goto L09CA;
    case 0x09CE: goto L09CE;
    case 0x09D2: goto L09D2;
    case 0x09D3: goto L09D3;
    case 0x09D5: goto L09D5;
    case 0x09D7: goto L09D7;
    case 0x09D9: goto L09D9;
    case 0x09DD: goto L09DD;
    case 0x09DE: goto L09DE;
    case 0x09E0: goto L09E0;
    case 0x09E2: goto L09E2;
    case 0x09E3: goto L09E3;
    case 0x09E5: goto L09E5;
    case 0x09E7: goto L09E7;
    case 0x09EA: goto L09EA;
    case 0x09EB: goto L09EB;
    case 0x09ED: goto L09ED;
    case 0x09EF: goto L09EF;
    case 0x09F1: goto L09F1;
    case 0x09F2: goto L09F2;
    case 0x09F3: goto L09F3;
    case 0x09F5: goto L09F5;
    case 0x09F6: goto L09F6;
    case 0x09F8: goto L09F8;
    case 0x09FA: goto L09FA;
    case 0x0A7F: goto L0A7F;
    case 0x0A80: goto L0A80;
    case 0x0A81: goto L0A81;
    case 0x0A85: goto L0A85;
    case 0x0A89: goto L0A89;
    case 0x0A8B: goto L0A8B;
    case 0x0A8E: goto L0A8E;
    case 0x0A90: goto L0A90;
    case 0x0A92: goto L0A92;
    case 0x0A95: goto L0A95;
    case 0x0A9A: goto L0A9A;
    case 0x0A9D: goto L0A9D;
    case 0x0AA0: goto L0AA0;
    case 0x0AA3: goto L0AA3;
    case 0x0AA5: goto L0AA5;
    case 0x0AA8: goto L0AA8;
    case 0x0AA9: goto L0AA9;
    case 0x0AAB: goto L0AAB;
    case 0x0AAC: goto L0AAC;
    case 0x0AAE: goto L0AAE;
    case 0x0AB1: goto L0AB1;
    case 0x0AB6: goto L0AB6;
    case 0x0ABB: goto L0ABB;
    case 0x0ABE: goto L0ABE;
    case 0x0AC1: goto L0AC1;
    case 0x0AC2: goto L0AC2;
    case 0x0AC3: goto L0AC3;
    case 0x0AC6: goto L0AC6;
    case 0x0AC8: goto L0AC8;
    case 0x0ACA: goto L0ACA;
    case 0x0ACD: goto L0ACD;
    case 0x0AD0: goto L0AD0;
    case 0x0AD2: goto L0AD2;
    case 0x0AD5: goto L0AD5;
    case 0x0AD7: goto L0AD7;
    case 0x0AD8: goto L0AD8;
    case 0x0AD9: goto L0AD9;
    case 0x0ADB: goto L0ADB;
    case 0x0ADE: goto L0ADE;
    case 0x0AE0: goto L0AE0;
    case 0x0AE1: goto L0AE1;
    case 0x0AE2: goto L0AE2;
    case 0x0AE4: goto L0AE4;
    case 0x0AE7: goto L0AE7;
    case 0x0AE9: goto L0AE9;
    case 0x0AEE: goto L0AEE;
    case 0x0AF3: goto L0AF3;
    case 0x0AF6: goto L0AF6;
    case 0x0AF9: goto L0AF9;
    case 0x0AFA: goto L0AFA;
    case 0x0AFB: goto L0AFB;
    case 0x0AFE: goto L0AFE;
    case 0x0B00: goto L0B00;
    case 0x0B02: goto L0B02;
    case 0x0B05: goto L0B05;
    case 0x0B08: goto L0B08;
    case 0x0B0A: goto L0B0A;
    case 0x0B0D: goto L0B0D;
    case 0x0B0F: goto L0B0F;
    case 0x0B10: goto L0B10;
    case 0x0B11: goto L0B11;
    case 0x0B13: goto L0B13;
    case 0x0B16: goto L0B16;
    case 0x0B18: goto L0B18;
    case 0x0B19: goto L0B19;
    case 0x0B1A: goto L0B1A;
    case 0x0B1C: goto L0B1C;
    case 0x0B1E: goto L0B1E;
    case 0x0B23: goto L0B23;
    case 0x0B28: goto L0B28;
    case 0x0B2B: goto L0B2B;
    case 0x0B2E: goto L0B2E;
    case 0x0B2F: goto L0B2F;
    case 0x0B30: goto L0B30;
    case 0x0B33: goto L0B33;
    case 0x0B35: goto L0B35;
    case 0x0B37: goto L0B37;
    case 0x0B3A: goto L0B3A;
    case 0x0B3D: goto L0B3D;
    case 0x0B3F: goto L0B3F;
    case 0x0B42: goto L0B42;
    case 0x0B44: goto L0B44;
    case 0x0B45: goto L0B45;
    case 0x0B46: goto L0B46;
    case 0x0B48: goto L0B48;
    case 0x0B4B: goto L0B4B;
    case 0x0B4D: goto L0B4D;
    case 0x0B4E: goto L0B4E;
    case 0x0B4F: goto L0B4F;
    case 0x0B51: goto L0B51;
    case 0x0B53: goto L0B53;
    case 0x0B58: goto L0B58;
    case 0x0B5D: goto L0B5D;
    case 0x0B60: goto L0B60;
    case 0x0B63: goto L0B63;
    case 0x0B64: goto L0B64;
    case 0x0B65: goto L0B65;
    case 0x0B68: goto L0B68;
    case 0x0B6A: goto L0B6A;
    case 0x0B6C: goto L0B6C;
    case 0x0B6F: goto L0B6F;
    case 0x0B72: goto L0B72;
    case 0x0B74: goto L0B74;
    case 0x0B77: goto L0B77;
    case 0x0B79: goto L0B79;
    case 0x0B7A: goto L0B7A;
    case 0x0B7B: goto L0B7B;
    case 0x0B7D: goto L0B7D;
    case 0x0B80: goto L0B80;
    case 0x0B82: goto L0B82;
    case 0x0B83: goto L0B83;
    case 0x0B84: goto L0B84;
    case 0x0B86: goto L0B86;
    case 0x0B88: goto L0B88;
    case 0x0B89: goto L0B89;
    case 0x0B8C: goto L0B8C;
    case 0x0B8F: goto L0B8F;
    case 0x0B92: goto L0B92;
    case 0x0B93: goto L0B93;
    case 0x0B94: goto L0B94;
    case 0x0B95: goto L0B95;
    case 0x0B96: goto L0B96;
    case 0x0B99: goto L0B99;
    case 0x0BC0: goto L0BC0;
    case 0x0BC7: goto L0BC7;
    case 0x0BC8: goto L0BC8;
    case 0x0BC9: goto L0BC9;
    case 0x0BCC: goto L0BCC;
    case 0x0BCE: goto L0BCE;
    case 0x0BD2: goto L0BD2;
    case 0x0BD6: goto L0BD6;
    case 0x0BD8: goto L0BD8;
    case 0x0BDD: goto L0BDD;
    case 0x0BE0: goto L0BE0;
    case 0x0BE4: goto L0BE4;
    case 0x0BE7: goto L0BE7;
    case 0x0BEA: goto L0BEA;
    case 0x0BEB: goto L0BEB;
    case 0x0BEC: goto L0BEC;
    case 0x0BF0: goto L0BF0;
    case 0x0BF2: goto L0BF2;
    case 0x0BF3: goto L0BF3;
    case 0x0BF4: goto L0BF4;
    case 0x0BF9: goto L0BF9;
    case 0x0BFE: goto L0BFE;
    case 0x0C01: goto L0C01;
    case 0x0C03: goto L0C03;
    case 0x0C05: goto L0C05;
    case 0x0C07: goto L0C07;
    case 0x0C0C: goto L0C0C;
    case 0x0C0E: goto L0C0E;
    case 0x0C0F: goto L0C0F;
    case 0x0C11: goto L0C11;
    case 0x0C12: goto L0C12;
    case 0x0C13: goto L0C13;
    case 0x0C14: goto L0C14;
    case 0x0C18: goto L0C18;
    case 0x0C1A: goto L0C1A;
    case 0x0C1C: goto L0C1C;
    case 0x0C1E: goto L0C1E;
    case 0x0C23: goto L0C23;
    case 0x0C24: goto L0C24;
    case 0x0C25: goto L0C25;
    case 0x0C26: goto L0C26;
    case 0x0C27: goto L0C27;
    case 0x0C28: goto L0C28;
    case 0x0C29: goto L0C29;
    case 0x0C2A: goto L0C2A;
    case 0x0C2C: goto L0C2C;
    case 0x0C2E: goto L0C2E;
    case 0x0C30: goto L0C30;
    case 0x0C32: goto L0C32;
    case 0x0C34: goto L0C34;
    case 0x0C35: goto L0C35;
    case 0x0C3A: goto L0C3A;
    case 0x0C3F: goto L0C3F;
    case 0x0C41: goto L0C41;
    case 0x0C42: goto L0C42;
    case 0x0C44: goto L0C44;
    case 0x0C46: goto L0C46;
    case 0x0C4B: goto L0C4B;
    case 0x0C4D: goto L0C4D;
    case 0x0C4F: goto L0C4F;
    case 0x0C51: goto L0C51;
    case 0x0C53: goto L0C53;
    case 0x0C55: goto L0C55;
    case 0x0C57: goto L0C57;
    case 0x0C59: goto L0C59;
    case 0x0C5B: goto L0C5B;
    case 0x0C5C: goto L0C5C;
    case 0x0C5E: goto L0C5E;
    case 0x0C60: goto L0C60;
    case 0x0C62: goto L0C62;
    case 0x0C64: goto L0C64;
    case 0x0C66: goto L0C66;
    case 0x0C68: goto L0C68;
    case 0x0C6A: goto L0C6A;
    case 0x0C6E: goto L0C6E;
    case 0x0C70: goto L0C70;
    case 0x0C72: goto L0C72;
    case 0x0C74: goto L0C74;
    case 0x0C75: goto L0C75;
    case 0x0C77: goto L0C77;
    case 0x0C79: goto L0C79;
    case 0x0C7B: goto L0C7B;
    case 0x0C7D: goto L0C7D;
    case 0x0C7F: goto L0C7F;
    case 0x0C81: goto L0C81;
    case 0x0C86: goto L0C86;
    case 0x0C88: goto L0C88;
    case 0x0C89: goto L0C89;
    case 0x0C8E: goto L0C8E;
    case 0x0C93: goto L0C93;
    case 0x0C94: goto L0C94;
    case 0x0C95: goto L0C95;
    case 0x0C97: goto L0C97;
    case 0x0C99: goto L0C99;
    case 0x0C9B: goto L0C9B;
    case 0x0C9D: goto L0C9D;
    case 0x0CA0: goto L0CA0;
    case 0x0CA3: goto L0CA3;
    case 0x0CA8: goto L0CA8;
    case 0x0CAA: goto L0CAA;
    case 0x0CAC: goto L0CAC;
    case 0x0CAF: goto L0CAF;
    case 0x0CB1: goto L0CB1;
    case 0x0CB6: goto L0CB6;
    case 0x0CB9: goto L0CB9;
    case 0x0CBB: goto L0CBB;
    case 0x0CBE: goto L0CBE;
    case 0x0CC0: goto L0CC0;
    case 0x0CC2: goto L0CC2;
    case 0x0CC3: goto L0CC3;
    case 0x0CC4: goto L0CC4;
    case 0x0CC6: goto L0CC6;
    case 0x0CC8: goto L0CC8;
    case 0x0CCB: goto L0CCB;
    case 0x0CCD: goto L0CCD;
    case 0x0CCF: goto L0CCF;
    case 0x0CD0: goto L0CD0;
    case 0x0CD1: goto L0CD1;
    case 0x0CD3: goto L0CD3;
    case 0x0CD4: goto L0CD4;
    case 0x0CD5: goto L0CD5;
    case 0x0CD6: goto L0CD6;
    case 0x0CD9: goto L0CD9;
    case 0x0CDA: goto L0CDA;
    case 0x0CDC: goto L0CDC;
    case 0x0CDE: goto L0CDE;
    case 0x0CE0: goto L0CE0;
    case 0x0CE3: goto L0CE3;
    case 0x0CE6: goto L0CE6;
    case 0x0CEB: goto L0CEB;
    case 0x0CED: goto L0CED;
    case 0x0CEF: goto L0CEF;
    case 0x0CF1: goto L0CF1;
    case 0x0CF2: goto L0CF2;
    case 0x0CF5: goto L0CF5;
    case 0x0CF7: goto L0CF7;
    case 0x0CFA: goto L0CFA;
    case 0x0CFC: goto L0CFC;
    case 0x0CFE: goto L0CFE;
    case 0x0D01: goto L0D01;
    case 0x0D03: goto L0D03;
    case 0x0D05: goto L0D05;
    case 0x0D06: goto L0D06;
    case 0x0D07: goto L0D07;
    case 0x0D09: goto L0D09;
    case 0x0D0B: goto L0D0B;
    case 0x0D0D: goto L0D0D;
    case 0x0D10: goto L0D10;
    case 0x0D12: goto L0D12;
    case 0x0D14: goto L0D14;
    case 0x0D15: goto L0D15;
    case 0x0D16: goto L0D16;
    case 0x0D18: goto L0D18;
    case 0x0D19: goto L0D19;
    case 0x0D1A: goto L0D1A;
    case 0x0D1B: goto L0D1B;
    default: asm_bad_entry("GRENTRY.ASM", entry);
    }

    /* seg003_0272_6E2  (+6E2)
       clear_fbuf (FM Towns): fill the frame buffer with colour 0 (falls into _6E4). */
L06E2: /* _seg003_0272_6E2 */
    /* 06E2  xor     ax,ax */
    /* by hand: clear_fbuf's xor ax,ax: the colour the 3D view is cleared to, 0; with the hole probe (a test, gfx/holeprobe.c) its byte, so that uncovered pixels show */
    AX = (uint16_t)(port_hole_mark() >= 0 ? port_hole_mark() * 0x101 : 0);

    /* seg003_0272_6E4  (+6E4)
       fill_fbuf (far, cFillFB): AL = the colour. Fills 34E9h words of the frame buffer segment from
       offset 2.

       The two labels inside are span transfers into the frame buffer for VIDMODE.ASM's fbshow: _6F8
       (fbuf_draw_ylrpp) copies linear bitmap rows (8-byte records: y, left, right, source offset;
       source segment in 55EC) into the frame buffer, and _729 (fbuf_draw_ylrpp_x) does the same
       skipping colour 0. */
L06E4: /* _seg003_0272_6E4 */
    /* 06E4  mov     ah,al */
    AH = AL;
L06E6:
    /* 06E6  push    es */
    push16(asm_es);
L06E7:
    /* 06E7  mov     cx,seg seg049_3EE2 */
    CX = (uint16_t)(0x3CF5 + PORT_LOAD_SEG);
L06EA:
    /* 06EA  mov     es,cx */
    SET_ES(CX);
L06EC:
    /* 06EC  mov     cx,34E9h */
    CX = 0x34E9;
L06EF:
    /* 06EF  xor     di,di */
    DI = (uint16_t)(DI ^ DI);
L06F1:
    /* 06F1  add     di,2 */
    DI = add16(DI, 0x2, 0);
L06F4:
    /* 06F4  rep stosw */
    while (CX) { ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2)); CX--; }
L06F6:
    /* 06F6  pop     es */
    SET_ES(pop16());
L06F7:
    /* 06F7  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;
L06F8: /* _seg003_0272_6F8 */
    /* 06F8  push    es */
    push16(asm_es);
L06F9:
    /* 06F9  push    ds */
    push16(asm_ds);
L06FA:
    /* 06FA  mov     es,word ptr ss:[958h] */
    SET_ES(rw(pSS, 0x958));
L06FF:
    /* 06FF  mov     ds,word ptr ss:[55ECh] */
    SET_DS(rw(pSS, 0x55EC));
L0704: /* L0704 */
    /* 0704  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0706:
    /* 0706  shl     ax,1 */
    AX = shl16(AX, 1);
L0708:
    /* 0708  jb      L0726 */
    if (CF) goto L0726;
L070A:
    /* 070A  mov     bx,ax */
    BX = AX;
L070C:
    /* 070C  mov     di,word ptr ss:[bx+95Ch] */
    DI = rw(pSS, BX + 0x95C);
L0711:
    /* 0711  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0713:
    /* 0713  add     di,ax */
    DI = (uint16_t)(DI + AX);
L0715:
    /* 0715  mov     cx,ax */
    CX = AX;
L0717:
    /* 0717  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0719:
    /* 0719  sub     ax,cx */
    AX = (uint16_t)(AX - CX);
L071B:
    /* 071B  inc     ax */
    AX = (uint16_t)(AX + 1);
L071C:
    /* 071C  mov     cx,ax */
    CX = AX;
L071E:
    /* 071E  lods    word ptr ss:[si] */
    AX = rw(pSS, SI); SI = (uint16_t)(SI + STEP(2));
L0720:
    /* 0720  xchg    si,ax */
    { uint16_t t_ = SI;
    SI = AX;
    AX = t_; }
L0721:
    /* 0721  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L0723:
    /* 0723  xchg    si,ax */
    { uint16_t t_ = SI;
    SI = AX;
    AX = t_; }
L0724:
    /* 0724  jmp     L0704 */
    goto L0704;
L0726: /* L0726 */
    /* 0726  pop     ds */
    SET_DS(pop16());
L0727:
    /* 0727  pop     es */
    SET_ES(pop16());
L0728:
    /* 0728  retn */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_764  (+764)
       cDimFB (FM Towns; far): remap every frame-buffer pixel through row CL of lightabs
       (seg004:6D3E, PGCACHE.ASM), so the whole view darkens by that distance shade (CH and CL are
       swapped to index by row). */
L0764: /* _seg003_0272_764 */
    /* 0764  push    ds */
    push16(asm_ds);
L0765:
    /* 0765  push    es */
    push16(asm_es);
L0766:
    /* 0766  mov     bx,seg seg049_3EE2 */
    BX = (uint16_t)(0x3CF5 + PORT_LOAD_SEG);
L0769:
    /* 0769  mov     ds,bx */
    SET_DS(BX);
L076B:
    /* 076B  mov     bx,seg seg004_0849 */
    BX = (uint16_t)(0x065C + PORT_LOAD_SEG);
L076E:
    /* 076E  mov     es,bx */
    SET_ES(BX);
L0770:
    /* 0770  xchg    ch,cl */
    { uint8_t t_ = CL;
    CL = CH;
    CH = t_; }
L0772:
    /* 0772  mov     si,cx */
    SI = CX;
L0774:
    /* 0774  mov     di,6AA6h */
    DI = 0x6AA6;
L0777:
    /* 0777  xor     bx,bx */
    BX = logic16((uint16_t)(BX ^ BX));
L0779: /* L0779 */
    /* 0779  mov     bl,byte ptr [di] */
    BL = rb(pDS, DI);
L077B:
    /* 077B  mov     bl,byte ptr es:[bx+si+6D3Eh] */
    BL = rb(pES, BX + SI + 0x6D3E);
L0780:
    /* 0780  mov     byte ptr [di],bl */
    wb(pDS, DI, BL);
L0782:
    /* 0782  dec     di */
    DI = dec16(DI);
L0783:
    /* 0783  jne     L0779 */
    if (!ZF) goto L0779;
L0785:
    /* 0785  pop     es */
    SET_ES(pop16());
L0786:
    /* 0786  pop     ds */
    SET_DS(pop16());
L0787:
    /* 0787  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_0272_788  (+788)
       cLiteFB (FM Towns; far): remap every frame-buffer pixel through the colour table at cXfer+300h
       (SCALEBM.ASM), CX times. L07A4 and L07A6 hold the caller's stack for cFBtoScreen. */
L0788: /* _seg003_0272_788 */
    /* 0788  push    ds */
    push16(asm_ds);
L0789:
    /* 0789  mov     bx,seg seg049_3EE2 */
    BX = (uint16_t)(0x3CF5 + PORT_LOAD_SEG);
L078C:
    /* 078C  mov     ds,bx */
    SET_DS(BX);
L078E: /* L078E */
    /* 078E  mov     si,69D2h */
    SI = 0x69D2;
L0791:
    /* 0791  xor     bx,bx */
    BX = logic16((uint16_t)(BX ^ BX));
L0793: /* L0793 */
    /* 0793  mov     bl,byte ptr [si] */
    BL = rb(pDS, SI);
L0795:
    /* 0795  mov     bl,byte ptr cs:_cXfer[bx+300h] */
    BL = rb(CODE003, BX + 0x1173);
L079A:
    /* 079A  mov     byte ptr [si],bl */
    wb(pDS, SI, BL);
L079C:
    /* 079C  dec     si */
    SI = dec16(SI);
L079D:
    /* 079D  jne     L0793 */
    if (!ZF) goto L0793;
L079F:
    /* 079F  dec     cx */
    CX = dec16(CX);
L07A0:
    /* 07A0  jne     L078E */
    if (!ZF) goto L078E;
L07A2:
    /* 07A2  pop     ds */
    SET_DS(pop16());
L07A3:
    /* 07A3  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_0272_9C9  (+9C9)
       fake_fb_solid_ylr (FM Towns): the solid span writer for the frame buffer: fill each 6-byte
       span record (y, left, right; either order) with the colour in 4111. */
L09C9: /* _seg003_0272_9C9 */
    /* 09C9  push    es */
    push16(asm_es);
L09CA:
    /* 09CA  mov     bx,word ptr ds:[3DF4h] */
    BX = rw(pDS, 0x3DF4);
L09CE:
    /* 09CE  mov     es,word ptr ds:[958h] */
    SET_ES(rw(pDS, 0x958));
L09D2: /* L09D2 */
    /* 09D2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L09D3:
    /* 09D3  shl     ax,1 */
    AX = shl16(AX, 1);
L09D5:
    /* 09D5  jb      L09F1 */
    if (CF) goto L09F1;
L09D7:
    /* 09D7  mov     di,ax */
    DI = AX;
L09D9:
    /* 09D9  mov     di,word ptr [di+95Ch] */
    DI = rw(pDS, DI + 0x95C);
L09DD:
    /* 09DD  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L09DE:
    /* 09DE  mov     cx,ax */
    CX = AX;
L09E0:
    /* 09E0  add     di,ax */
    DI = (uint16_t)(DI + AX);
L09E2:
    /* 09E2  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L09E3:
    /* 09E3  sub     cx,ax */
    CX = (uint16_t)(CX - AX);
L09E5:
    /* 09E5  neg     cx */
    CX = (uint16_t)-CX;
L09E7:
    /* 09E7  mov     al,byte ptr ds:[4111h] */
    AL = rb(pDS, 0x4111);
L09EA:
    /* 09EA  inc     cx */
    CX = inc16(CX);
L09EB:
    /* 09EB  js      L09F3 */
    if (SF) goto L09F3;
L09ED:
    /* 09ED  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L09EF:
    /* 09EF  jmp     L09D2 */
    goto L09D2;
L09F1: /* L09F1 */
    /* 09F1  pop     es */
    SET_ES(pop16());
L09F2:
    /* 09F2  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L09F3: /* L09F3 */
    /* 09F3  add     di,cx */
    DI = (uint16_t)(DI + CX);
L09F5:
    /* 09F5  dec     cx */
    CX = (uint16_t)(CX - 1);
L09F6:
    /* 09F6  neg     cx */
    CX = (uint16_t)-CX;
L09F8:
    /* 09F8  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L09FA:
    /* 09FA  jmp     L09D2 */
    goto L09D2;

    /* seg003_0272_A7F  (+A7F)
       fbcopy_vylrpp (FM Towns): copy frame-buffer rows to the planar screen (8-byte records: y,
       left, right, destination offset), plane by plane with the edge masks from 3B64 and 3CA8;
       VIDMODE's vcopyfb uses it. Ends by setting the bit mask back to 0FFh. */
L0A7F: /* _seg003_0272_A7F */
    /* 0A7F  push    ds */
    push16(asm_ds);
L0A80:
    /* 0A80  push    es */
    push16(asm_es);
L0A81:
    /* 0A81  mov     es,word ptr ds:[3DFCh] */
    SET_ES(rw(pDS, 0x3DFC));
L0A85:
    /* 0A85  mov     ds,word ptr ds:[958h] */
    SET_DS(rw(pDS, 0x958));
L0A89:
    /* 0A89  mov     bp,si */
    BP = SI;
L0A8B: /* L0A8B */
    /* 0A8B  mov     bx,word ptr [bp] */
    BX = rw(pSS, BP);
L0A8E:
    /* 0A8E  shl     bx,1 */
    BX = shl16(BX, 1);
L0A90:
    /* 0A90  jae     L0A95 */
    if (!CF) goto L0A95;
L0A92:
    /* 0A92  jmp     L0B8C */
    goto L0B8C;
L0A95: /* L0A95 */
    /* 0A95  mov     si,word ptr ss:[bx+95Ch] */
    SI = rw(pSS, BX + 0x95C);
L0A9A:
    /* 0A9A  mov     bx,word ptr [bp+2] */
    BX = rw(pSS, BP + 0x2);
L0A9D:
    /* 0A9D  mov     cx,word ptr [bp+4] */
    CX = rw(pSS, BP + 0x4);
L0AA0:
    /* 0AA0  mov     di,word ptr [bp+6] */
    DI = rw(pSS, BP + 0x6);
L0AA3:
    /* 0AA3  add     si,bx */
    SI = (uint16_t)(SI + BX);
L0AA5:
    /* 0AA5  add     bp,8 */
    BP = (uint16_t)(BP + 0x8);
L0AA8:
    /* 0AA8  push    bp */
    push16(BP);
L0AA9:
    /* 0AA9  sub     cx,bx */
    CX = (uint16_t)(CX - BX);
L0AAB:
    /* 0AAB  inc     cx */
    CX = (uint16_t)(CX + 1);
L0AAC:
    /* 0AAC  mov     bp,cx */
    BP = CX;
L0AAE:
    /* 0AAE  mov     dx,SC_DATA */
    DX = 0x3C5;
L0AB1:
    /* 0AB1  mov     al,byte ptr ss:[bx+3B64h] */
    AL = rb(pSS, BX + 0x3B64);
L0AB6:
    /* 0AB6  and     al,byte ptr ss:[bx+3CA8h] */
    AL = (uint8_t)(AL & rb(pSS, BX + 0x3CA8));
L0ABB:
    /* 0ABB  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0ABE:
    /* 0ABE  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0AC1:
    /* 0AC1  out     dx,al */
    asm_out8(DX, AL);
L0AC2: /* L0AC2 */
    /* 0AC2  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0AC3:
    /* 0AC3  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0AC6:
    /* 0AC6  loop    L0AC2 */
    if (--CX) goto L0AC2;
L0AC8:
    /* 0AC8  mov     cx,bp */
    CX = BP;
L0ACA:
    /* 0ACA  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0ACD:
    /* 0ACD  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0AD0:
    /* 0AD0  sub     di,cx */
    DI = (uint16_t)(DI - CX);
L0AD2:
    /* 0AD2  shl     cx,2 */
    CX = (uint16_t)(CX << 2);
L0AD5:
    /* 0AD5  sub     si,cx */
    SI = (uint16_t)(SI - CX);
L0AD7:
    /* 0AD7  inc     si */
    SI = (uint16_t)(SI + 1);
L0AD8:
    /* 0AD8  inc     bx */
    BX = (uint16_t)(BX + 1);
L0AD9:
    /* 0AD9  mov     cx,bx */
    CX = BX;
L0ADB:
    /* 0ADB  and     cx,3 */
    CX = logic16((uint16_t)(CX & 0x3));
L0ADE:
    /* 0ADE  jne     L0AE1 */
    if (!ZF) goto L0AE1;
L0AE0:
    /* 0AE0  inc     di */
    DI = (uint16_t)(DI + 1);
L0AE1: /* L0AE1 */
    /* 0AE1  dec     bp */
    BP = dec16(BP);
L0AE2:
    /* 0AE2  jne     L0AE7 */
    if (!ZF) goto L0AE7;
L0AE4:
    /* 0AE4  jmp     L0B88 */
    goto L0B88;
L0AE7: /* L0AE7 */
    /* 0AE7  mov     cx,bp */
    CX = BP;
L0AE9:
    /* 0AE9  mov     al,byte ptr ss:[bx+3B64h] */
    AL = rb(pSS, BX + 0x3B64);
L0AEE:
    /* 0AEE  and     al,byte ptr ss:[bx+3CA8h] */
    AL = (uint8_t)(AL & rb(pSS, BX + 0x3CA8));
L0AF3:
    /* 0AF3  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0AF6:
    /* 0AF6  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0AF9:
    /* 0AF9  out     dx,al */
    asm_out8(DX, AL);
L0AFA: /* L0AFA */
    /* 0AFA  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0AFB:
    /* 0AFB  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0AFE:
    /* 0AFE  loop    L0AFA */
    if (--CX) goto L0AFA;
L0B00:
    /* 0B00  mov     cx,bp */
    CX = BP;
L0B02:
    /* 0B02  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0B05:
    /* 0B05  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0B08:
    /* 0B08  sub     di,cx */
    DI = (uint16_t)(DI - CX);
L0B0A:
    /* 0B0A  shl     cx,2 */
    CX = (uint16_t)(CX << 2);
L0B0D:
    /* 0B0D  sub     si,cx */
    SI = (uint16_t)(SI - CX);
L0B0F:
    /* 0B0F  inc     si */
    SI = (uint16_t)(SI + 1);
L0B10:
    /* 0B10  inc     bx */
    BX = (uint16_t)(BX + 1);
L0B11:
    /* 0B11  mov     cx,bx */
    CX = BX;
L0B13:
    /* 0B13  and     cx,3 */
    CX = logic16((uint16_t)(CX & 0x3));
L0B16:
    /* 0B16  jne     L0B19 */
    if (!ZF) goto L0B19;
L0B18:
    /* 0B18  inc     di */
    DI = (uint16_t)(DI + 1);
L0B19: /* L0B19 */
    /* 0B19  dec     bp */
    BP = dec16(BP);
L0B1A:
    /* 0B1A  je      L0B88 */
    if (ZF) goto L0B88;
L0B1C:
    /* 0B1C  mov     cx,bp */
    CX = BP;
L0B1E:
    /* 0B1E  mov     al,byte ptr ss:[bx+3B64h] */
    AL = rb(pSS, BX + 0x3B64);
L0B23:
    /* 0B23  and     al,byte ptr ss:[bx+3CA8h] */
    AL = (uint8_t)(AL & rb(pSS, BX + 0x3CA8));
L0B28:
    /* 0B28  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0B2B:
    /* 0B2B  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0B2E:
    /* 0B2E  out     dx,al */
    asm_out8(DX, AL);
L0B2F: /* L0B2F */
    /* 0B2F  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0B30:
    /* 0B30  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0B33:
    /* 0B33  loop    L0B2F */
    if (--CX) goto L0B2F;
L0B35:
    /* 0B35  mov     cx,bp */
    CX = BP;
L0B37:
    /* 0B37  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0B3A:
    /* 0B3A  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0B3D:
    /* 0B3D  sub     di,cx */
    DI = (uint16_t)(DI - CX);
L0B3F:
    /* 0B3F  shl     cx,2 */
    CX = (uint16_t)(CX << 2);
L0B42:
    /* 0B42  sub     si,cx */
    SI = (uint16_t)(SI - CX);
L0B44:
    /* 0B44  inc     si */
    SI = (uint16_t)(SI + 1);
L0B45:
    /* 0B45  inc     bx */
    BX = (uint16_t)(BX + 1);
L0B46:
    /* 0B46  mov     cx,bx */
    CX = BX;
L0B48:
    /* 0B48  and     cx,3 */
    CX = logic16((uint16_t)(CX & 0x3));
L0B4B:
    /* 0B4B  jne     L0B4E */
    if (!ZF) goto L0B4E;
L0B4D:
    /* 0B4D  inc     di */
    DI = (uint16_t)(DI + 1);
L0B4E: /* L0B4E */
    /* 0B4E  dec     bp */
    BP = dec16(BP);
L0B4F:
    /* 0B4F  je      L0B88 */
    if (ZF) goto L0B88;
L0B51:
    /* 0B51  mov     cx,bp */
    CX = BP;
L0B53:
    /* 0B53  mov     al,byte ptr ss:[bx+3B64h] */
    AL = rb(pSS, BX + 0x3B64);
L0B58:
    /* 0B58  and     al,byte ptr ss:[bx+3CA8h] */
    AL = (uint8_t)(AL & rb(pSS, BX + 0x3CA8));
L0B5D:
    /* 0B5D  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0B60:
    /* 0B60  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0B63:
    /* 0B63  out     dx,al */
    asm_out8(DX, AL);
L0B64: /* L0B64 */
    /* 0B64  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0B65:
    /* 0B65  add     si,3 */
    SI = (uint16_t)(SI + 0x3);
L0B68:
    /* 0B68  loop    L0B64 */
    if (--CX) goto L0B64;
L0B6A:
    /* 0B6A  mov     cx,bp */
    CX = BP;
L0B6C:
    /* 0B6C  add     cx,3 */
    CX = (uint16_t)(CX + 0x3);
L0B6F:
    /* 0B6F  shr     cx,2 */
    CX = (uint16_t)(CX >> 2);
L0B72:
    /* 0B72  sub     di,cx */
    DI = (uint16_t)(DI - CX);
L0B74:
    /* 0B74  shl     cx,2 */
    CX = (uint16_t)(CX << 2);
L0B77:
    /* 0B77  sub     si,cx */
    SI = (uint16_t)(SI - CX);
L0B79:
    /* 0B79  inc     si */
    SI = (uint16_t)(SI + 1);
L0B7A:
    /* 0B7A  inc     bx */
    BX = (uint16_t)(BX + 1);
L0B7B:
    /* 0B7B  mov     cx,bx */
    CX = BX;
L0B7D:
    /* 0B7D  and     cx,3 */
    CX = logic16((uint16_t)(CX & 0x3));
L0B80:
    /* 0B80  jne     L0B83 */
    if (!ZF) goto L0B83;
L0B82:
    /* 0B82  inc     di */
    DI = (uint16_t)(DI + 1);
L0B83: /* L0B83 */
    /* 0B83  dec     bp */
    BP = dec16(BP);
L0B84:
    /* 0B84  je      L0B88 */
    if (ZF) goto L0B88;
L0B86:
    /* 0B86  mov     cx,bp */
    CX = BP;
L0B88: /* L0B88 */
    /* 0B88  pop     bp */
    BP = pop16();
L0B89:
    /* 0B89  jmp     L0A8B */
    goto L0A8B;
L0B8C: /* L0B8C */
    /* 0B8C  mov     dx,GC_INDEX */
    DX = 0x3CE;
L0B8F:
    /* 0B8F  mov     ax,0FF08h */
    AX = 0xFF08;
L0B92:
    /* 0B92  out     dx,ax */
    asm_out16(DX, AX);
L0B93:
    /* 0B93  pop     es */
    SET_ES(pop16());
L0B94:
    /* 0B94  pop     ds */
    SET_DS(pop16());
L0B95:
    /* 0B95  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0B96:
    /* 0B96  add     sp,2 */
    SP = add16(SP, 0x2, 0);
L0B99:
    /* 0B99  jmp     L0B8C */
    goto L0B8C;
L0BC0: /* L0BC0 */
    /* 0BC0  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0BC7: /* L0BC7 */
    /* 0BC7  push    si */
    push16(SI);
L0BC8:
    /* 0BC8  push    es */
    push16(asm_es);
L0BC9:
    /* 0BC9  mov     ax,seg seg004_0849 */
    AX = (uint16_t)(0x065C + PORT_LOAD_SEG);
L0BCC:
    /* 0BCC  mov     es,ax */
    SET_ES(AX);
L0BCE:
    /* 0BCE  mov     al,byte ptr es:[6D20h] */
    AL = rb(pES, 0x6D20);
L0BD2:
    /* 0BD2  mov     ah,byte ptr ds:[0B08h] */
    AH = rb(pDS, 0xB08);
L0BD6:
    /* 0BD6  mov     bx,ax */
    BX = AX;
L0BD8:
    /* 0BD8  mov     al,byte ptr es:[bx+6D3Eh] */
    AL = rb(pES, BX + 0x6D3E);
L0BDD:
    /* 0BDD  mov     byte ptr ds:[4111h],al */
    wb(pDS, 0x4111, AL);
L0BE0:
    /* 0BE0  or      byte ptr [si-3],80h */
    wb(pDS, SI + 0xFFFD, (uint8_t)(rb(pDS, SI + 0xFFFD) | 0x80));
L0BE4:
    /* 0BE4  sub     si,0Ah */
    SI = (uint16_t)(SI - 0xA);
L0BE7:
    /* 0BE7  call    _seg003_0272_9C9 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x09C9), 0x0BEA)) != 0) return c;
L0BEA:
    /* 0BEA  pop     es */
    SET_ES(pop16());
L0BEB:
    /* 0BEB  pop     si */
    SI = pop16();
L0BEC:
    /* 0BEC  and     byte ptr [si-3],7Fh */
    wb(pDS, SI + 0xFFFD, (uint8_t)(rb(pDS, SI + 0xFFFD) & 0x7F));
L0BF0:
    /* 0BF0  jmp     short _seg003_0272_C13 */
    goto L0C13;
L0BF2:
    /* 0BF2  push    ds */
    push16(asm_ds);
L0BF3:
    /* 0BF3  push    es */
    push16(asm_es);
L0BF4:
    /* 0BF4  mov     dh,byte ptr ss:[0B08h] */
    DH = rb(pSS, 0xB08);
L0BF9:
    /* 0BF9  mov     ds,word ptr ss:[958h] */
    SET_DS(rw(pSS, 0x958));
L0BFE:
    /* 0BFE  mov     ax,seg seg004_0849 */
    AX = (uint16_t)(0x065C + PORT_LOAD_SEG);
L0C01:
    /* 0C01  mov     es,ax */
    SET_ES(AX);
L0C03: /* L0C03 */
    /* 0C03  mov     dl,byte ptr [di] */
    DL = rb(pDS, DI);
L0C05:
    /* 0C05  mov     bx,dx */
    BX = DX;
L0C07:
    /* 0C07  mov     bh,byte ptr es:[bx+6D3Eh] */
    BH = rb(pES, BX + 0x6D3E);
L0C0C:
    /* 0C0C  mov     byte ptr [di],bh */
    wb(pDS, DI, BH);
L0C0E:
    /* 0C0E  inc     di */
    DI = (uint16_t)(DI + 1);
L0C0F:
    /* 0C0F  loop    L0C03 */
    if (--CX) goto L0C03;
L0C11:
    /* 0C11  pop     es */
    SET_ES(pop16());
L0C12:
    /* 0C12  pop     ds */
    SET_DS(pop16());

    /* seg003_0272_C13  (+C13)
       vga_smooth_ylrii (FM Towns): draw a table of Gouraud spans into the frame buffer. In: SI =
       10-byte records (y, left x, right x, left intensity, right intensity; GRLIBM.ASM's table at
       5692), ended by a y with its top bit set. For each, the intensity step per pixel is formed as
       8.8 and the span drawn through L0BC5, or through L0BC3 when both ends have the same intensity.

       L0C93 (do_filled_dither, solid) writes lightabs[intensity][base colour]; the code after L0CD3
       (do_trans_dither, translucent) writes lightabs[intensity][the pixel already there]. Both
       dither: two intensity accumulators 40h apart alternate pixel by pixel, and which starts
       depends on the row's parity, so a shade between two lightabs rows comes out as a checkerboard
       of both. */
L0C13: /* _seg003_0272_C13 */
    /* 0C13  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0C14:
    /* 0C14  mov     word ptr cs:L0BC1,ax */
    ww(CODE003, 0xBC1, AX);
L0C18:
    /* 0C18  shl     ax,1 */
    AX = shl16(AX, 1);
L0C1A:
    /* 0C1A  jb      L0BC0 */
    if (CF) goto L0BC0;
L0C1C:
    /* 0C1C  mov     di,ax */
    DI = AX;
L0C1E:
    /* 0C1E  mov     di,word ptr ss:[di+95Ch] */
    DI = rw(pSS, DI + 0x95C);
L0C23:
    /* 0C23  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0C24:
    /* 0C24  xchg    dx,ax */
    { uint16_t t_ = DX;
    DX = AX;
    AX = t_; }
L0C25:
    /* 0C25  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0C26:
    /* 0C26  xchg    cx,ax */
    { uint16_t t_ = CX;
    CX = AX;
    AX = t_; }
L0C27:
    /* 0C27  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0C28:
    /* 0C28  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L0C29:
    /* 0C29  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L0C2A:
    /* 0C2A  mov     bp,cx */
    BP = CX;
L0C2C:
    /* 0C2C  sub     cx,dx */
    CX = sub16(CX, DX, 0);
L0C2E:
    /* 0C2E  jge     L0C35 */
    if (SF == OF) goto L0C35;
L0C30:
    /* 0C30  neg     cx */
    CX = (uint16_t)-CX;
L0C32:
    /* 0C32  xchg    bp,dx */
    { uint16_t t_ = DX;
    DX = BP;
    BP = t_; }
L0C34:
    /* 0C34  xchg    bx,ax */
    { uint16_t t_ = BX;
    BX = AX;
    AX = t_; }
L0C35: /* L0C35 */
    /* 0C35  mov     word ptr ss:[0B08h],bx */
    ww(pSS, 0xB08, BX);
L0C3A:
    /* 0C3A  mov     word ptr ss:[0B0Ah],dx */
    ww(pSS, 0xB0A, DX);
L0C3F:
    /* 0C3F  add     di,dx */
    DI = (uint16_t)(DI + DX);
L0C41:
    /* 0C41  inc     cx */
    CX = (uint16_t)(CX + 1);
L0C42:
    /* 0C42  sub     ax,bx */
    AX = sub16(AX, BX, 0);
L0C44:
    /* 0C44  jne     L0C4B */
    if (!ZF) goto L0C4B;
L0C46:
    /* 0C46  jmp     word ptr cs:L0BC3 */
    return ASM_JMP(0x0085, rw(CODE003, 0xBC3));
L0C4B: /* L0C4B */
    /* 0C4B  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L0C4D:
    /* 0C4D  jns     L0C6A */
    if (!SF) goto L0C6A;
L0C4F:
    /* 0C4F  or      cx,cx */
    CX = logic16((uint16_t)(CX | CX));
L0C51:
    /* 0C51  jns     L0C59 */
    if (!SF) goto L0C59;
L0C53:
    /* 0C53  neg     ax */
    AX = (uint16_t)-AX;
L0C55:
    /* 0C55  neg     cx */
    CX = (uint16_t)-CX;
L0C57:
    /* 0C57  jmp     short L0C74 */
    goto L0C74;
L0C59: /* L0C59 */
    /* 0C59  neg     ax */
    AX = (uint16_t)-AX;
L0C5B: /* L0C5B */
    /* 0C5B  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0C5C:
    /* 0C5C  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0085, 0x0C5C, 2)) != 0) return c;
L0C5E:
    /* 0C5E  mov     bh,al */
    BH = AL;
L0C60:
    /* 0C60  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L0C62:
    /* 0C62  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x0085, 0x0C62, 2)) != 0) return c;
L0C64:
    /* 0C64  mov     bl,ah */
    BL = AH;
L0C66:
    /* 0C66  neg     bx */
    BX = neg16(BX);
L0C68:
    /* 0C68  jmp     short L0C7F */
    goto L0C7F;
L0C6A: /* L0C6A */
    /* 0C6A  test    cx,0FFFFh */
    logic16((uint16_t)(CX & 0xFFFF));
L0C6E:
    /* 0C6E  jns     L0C74 */
    if (!SF) goto L0C74;
L0C70:
    /* 0C70  neg     cx */
    CX = (uint16_t)-CX;
L0C72:
    /* 0C72  jmp     L0C5B */
    goto L0C5B;
L0C74: /* L0C74 */
    /* 0C74  cwd */
    DX = (int16_t)AX < 0 ? 0xFFFF : 0;
L0C75:
    /* 0C75  idiv    cx */
    if (asm_idiv16(CX) && (c = asm_divfault(0x0085, 0x0C75, 2)) != 0) return c;
L0C77:
    /* 0C77  mov     bh,al */
    BH = AL;
L0C79:
    /* 0C79  sub     ax,ax */
    AX = sub16(AX, AX, 0);
L0C7B:
    /* 0C7B  div     cx */
    if (asm_div16(CX) && (c = asm_divfault(0x0085, 0x0C7B, 2)) != 0) return c;
L0C7D:
    /* 0C7D  mov     bl,ah */
    BL = AH;
L0C7F: /* L0C7F */
    /* 0C7F  mov     bp,bx */
    BP = BX;
L0C81:
    /* 0C81  mov     bh,byte ptr ss:[0B08h] */
    BH = rb(pSS, 0xB08);
L0C86:
    /* 0C86  mov     bl,80h */
    BL = 0x80;
L0C88:
    /* 0C88  push    ds */
    push16(asm_ds);
L0C89:
    /* 0C89  mov     ds,word ptr ss:[958h] */
    SET_DS(rw(pSS, 0x958));
L0C8E:
    /* 0C8E  jmp     word ptr cs:L0BC5 */
    return ASM_JMP(0x0085, rw(CODE003, 0xBC5));
L0C93: /* L0C93 */
    /* 0C93  push    si */
    push16(SI);
L0C94:
    /* 0C94  push    es */
    push16(asm_es);
L0C95:
    /* 0C95  mov     ax,cx */
    AX = CX;
L0C97:
    /* 0C97  mov     cx,bx */
    CX = BX;
L0C99:
    /* 0C99  mov     dx,bx */
    DX = BX;
L0C9B:
    /* 0C9B  shl     bp,1 */
    BP = (uint16_t)(BP << 1);
L0C9D:
    /* 0C9D  sub     dx,40h */
    DX = (uint16_t)(DX - 0x40);
L0CA0:
    /* 0CA0  add     cx,40h */
    CX = (uint16_t)(CX + 0x40);
L0CA3:
    /* 0CA3  shr     word ptr cs:L0BC1,1 */
    ww(CODE003, 0xBC1, shr16(rw(CODE003, 0xBC1), 1));
L0CA8:
    /* 0CA8  jae     L0CAC */
    if (!CF) goto L0CAC;
L0CAA:
    /* 0CAA  xchg    cx,dx */
    { uint16_t t_ = DX;
    DX = CX;
    CX = t_; }
L0CAC: /* L0CAC */
    /* 0CAC  mov     bx,seg seg004_0849 */
    BX = (uint16_t)(0x065C + PORT_LOAD_SEG);
L0CAF:
    /* 0CAF  mov     es,bx */
    SET_ES(BX);
L0CB1:
    /* 0CB1  mov     bl,byte ptr es:[6D20h] */
    BL = rb(pES, 0x6D20);
L0CB6:
    /* 0CB6  mov     si,6D3Eh */
    SI = 0x6D3E;
L0CB9: /* L0CB9 */
    /* 0CB9  mov     bh,dh */
    BH = DH;
L0CBB:
    /* 0CBB  mov     bh,byte ptr es:[bx+si] */
    BH = rb(pES, BX + SI);
L0CBE:
    /* 0CBE  mov     byte ptr [di],bh */
    wb(pDS, DI, BH);
L0CC0:
    /* 0CC0  add     dx,bp */
    DX = (uint16_t)(DX + BP);
L0CC2:
    /* 0CC2  inc     di */
    DI = (uint16_t)(DI + 1);
L0CC3:
    /* 0CC3  dec     ax */
    AX = dec16(AX);
L0CC4:
    /* 0CC4  je      L0CD3 */
    if (ZF) goto L0CD3;
L0CC6:
    /* 0CC6  mov     bh,ch */
    BH = CH;
L0CC8:
    /* 0CC8  mov     bh,byte ptr es:[bx+si] */
    BH = rb(pES, BX + SI);
L0CCB:
    /* 0CCB  mov     byte ptr [di],bh */
    wb(pDS, DI, BH);
L0CCD:
    /* 0CCD  add     cx,bp */
    CX = (uint16_t)(CX + BP);
L0CCF:
    /* 0CCF  inc     di */
    DI = (uint16_t)(DI + 1);
L0CD0:
    /* 0CD0  dec     ax */
    AX = dec16(AX);
L0CD1:
    /* 0CD1  jne     L0CB9 */
    if (!ZF) goto L0CB9;
L0CD3: /* L0CD3 */
    /* 0CD3  pop     es */
    SET_ES(pop16());
L0CD4:
    /* 0CD4  pop     si */
    SI = pop16();
L0CD5:
    /* 0CD5  pop     ds */
    SET_DS(pop16());
L0CD6:
    /* 0CD6  jmp     _seg003_0272_C13 */
    goto L0C13;
L0CD9:
    /* 0CD9  push    si */
    push16(SI);
L0CDA:
    /* 0CDA  mov     ax,cx */
    AX = CX;
L0CDC:
    /* 0CDC  mov     cx,bx */
    CX = BX;
L0CDE:
    /* 0CDE  shl     bp,1 */
    BP = (uint16_t)(BP << 1);
L0CE0:
    /* 0CE0  sub     bx,40h */
    BX = (uint16_t)(BX - 0x40);
L0CE3:
    /* 0CE3  add     cx,40h */
    CX = (uint16_t)(CX + 0x40);
L0CE6:
    /* 0CE6  shr     word ptr cs:L0BC1,1 */
    ww(CODE003, 0xBC1, shr16(rw(CODE003, 0xBC1), 1));
L0CEB:
    /* 0CEB  jae     L0CEF */
    if (!CF) goto L0CEF;
L0CED:
    /* 0CED  xchg    cx,bx */
    { uint16_t t_ = BX;
    BX = CX;
    CX = t_; }
L0CEF: /* L0CEF */
    /* 0CEF  mov     dx,bx */
    DX = BX;
L0CF1:
    /* 0CF1  push    es */
    push16(asm_es);
L0CF2:
    /* 0CF2  mov     bx,seg seg004_0849 */
    BX = (uint16_t)(0x065C + PORT_LOAD_SEG);
L0CF5:
    /* 0CF5  mov     es,bx */
    SET_ES(BX);
L0CF7:
    /* 0CF7  mov     si,6D3Eh */
    SI = 0x6D3E;
L0CFA: /* L0CFA */
    /* 0CFA  mov     bl,byte ptr [di] */
    BL = rb(pDS, DI);
L0CFC:
    /* 0CFC  mov     bh,dh */
    BH = DH;
L0CFE:
    /* 0CFE  mov     bh,byte ptr es:[bx+si] */
    BH = rb(pES, BX + SI);
L0D01:
    /* 0D01  mov     byte ptr [di],bh */
    wb(pDS, DI, BH);
L0D03:
    /* 0D03  add     dx,bp */
    DX = (uint16_t)(DX + BP);
L0D05:
    /* 0D05  inc     di */
    DI = (uint16_t)(DI + 1);
L0D06:
    /* 0D06  dec     ax */
    AX = dec16(AX);
L0D07:
    /* 0D07  je      L0D18 */
    if (ZF) goto L0D18;
L0D09:
    /* 0D09  mov     bl,byte ptr [di] */
    BL = rb(pDS, DI);
L0D0B:
    /* 0D0B  mov     bh,ch */
    BH = CH;
L0D0D:
    /* 0D0D  mov     bh,byte ptr es:[bx+si] */
    BH = rb(pES, BX + SI);
L0D10:
    /* 0D10  mov     byte ptr [di],bh */
    wb(pDS, DI, BH);
L0D12:
    /* 0D12  add     cx,bp */
    CX = (uint16_t)(CX + BP);
L0D14:
    /* 0D14  inc     di */
    DI = (uint16_t)(DI + 1);
L0D15:
    /* 0D15  dec     ax */
    AX = dec16(AX);
L0D16:
    /* 0D16  jne     L0CFA */
    if (!ZF) goto L0CFA;
L0D18: /* L0D18 */
    /* 0D18  pop     es */
    SET_ES(pop16());
L0D19:
    /* 0D19  pop     si */
    SI = pop16();
L0D1A:
    /* 0D1A  pop     ds */
    SET_DS(pop16());
L0D1B:
    /* 0D1B  jmp     _seg003_0272_C13 */
    goto L0C13;
}
