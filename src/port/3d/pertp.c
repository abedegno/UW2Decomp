/* pertp.c: replaces src/3d/PERTP.ASM (seg004_0849_4B30, 4B30..4E04 of its
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

uint32_t asm_mod_PERTP(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x4B30: goto L4B30;
    case 0x4B34: goto L4B34;
    case 0x4B38: goto L4B38;
    case 0x4B3C: goto L4B3C;
    case 0x4B40: goto L4B40;
    case 0x4B44: goto L4B44;
    case 0x4B48: goto L4B48;
    case 0x4B4C: goto L4B4C;
    case 0x4B51: goto L4B51;
    case 0x4B55: goto L4B55;
    case 0x4B5A: goto L4B5A;
    case 0x4B5E: goto L4B5E;
    case 0x4B62: goto L4B62;
    case 0x4B66: goto L4B66;
    case 0x4B6A: goto L4B6A;
    case 0x4B6E: goto L4B6E;
    case 0x4B72: goto L4B72;
    case 0x4B75: goto L4B75;
    case 0x4B78: goto L4B78;
    case 0x4B7C: goto L4B7C;
    case 0x4B7D: goto L4B7D;
    case 0x4B80: goto L4B80;
    case 0x4B82: goto L4B82;
    case 0x4B84: goto L4B84;
    case 0x4B88: goto L4B88;
    case 0x4B89: goto L4B89;
    case 0x4B8D: goto L4B8D;
    case 0x4B91: goto L4B91;
    case 0x4B96: goto L4B96;
    case 0x4B9B: goto L4B9B;
    case 0x4BA0: goto L4BA0;
    case 0x4BA4: goto L4BA4;
    case 0x4BA8: goto L4BA8;
    case 0x4BAC: goto L4BAC;
    case 0x4BB2: goto L4BB2;
    case 0x4BB6: goto L4BB6;
    case 0x4BBA: goto L4BBA;
    case 0x4BBE: goto L4BBE;
    case 0x4BC0: goto L4BC0;
    case 0x4BC3: goto L4BC3;
    case 0x4BC6: goto L4BC6;
    case 0x4BCA: goto L4BCA;
    case 0x4BCF: goto L4BCF;
    case 0x4BD2: goto L4BD2;
    case 0x4BD7: goto L4BD7;
    case 0x4BD9: goto L4BD9;
    case 0x4BDD: goto L4BDD;
    case 0x4BE1: goto L4BE1;
    case 0x4BE6: goto L4BE6;
    case 0x4BEA: goto L4BEA;
    case 0x4BEC: goto L4BEC;
    case 0x4BED: goto L4BED;
    case 0x4BEE: goto L4BEE;
    case 0x4BF2: goto L4BF2;
    case 0x4BF7: goto L4BF7;
    case 0x4BFA: goto L4BFA;
    case 0x4BFF: goto L4BFF;
    case 0x4C01: goto L4C01;
    case 0x4C05: goto L4C05;
    case 0x4C09: goto L4C09;
    case 0x4C0E: goto L4C0E;
    case 0x4C12: goto L4C12;
    case 0x4C14: goto L4C14;
    case 0x4C15: goto L4C15;
    case 0x4C16: goto L4C16;
    case 0x4C1A: goto L4C1A;
    case 0x4C1F: goto L4C1F;
    case 0x4C22: goto L4C22;
    case 0x4C27: goto L4C27;
    case 0x4C29: goto L4C29;
    case 0x4C2D: goto L4C2D;
    case 0x4C31: goto L4C31;
    case 0x4C35: goto L4C35;
    case 0x4C3A: goto L4C3A;
    case 0x4C3F: goto L4C3F;
    case 0x4C43: goto L4C43;
    case 0x4C48: goto L4C48;
    case 0x4C4B: goto L4C4B;
    case 0x4C4F: goto L4C4F;
    case 0x4C53: goto L4C53;
    case 0x4C56: goto L4C56;
    case 0x4C58: goto L4C58;
    case 0x4C5B: goto L4C5B;
    case 0x4C5F: goto L4C5F;
    case 0x4C61: goto L4C61;
    case 0x4C64: goto L4C64;
    case 0x4C66: goto L4C66;
    case 0x4C69: goto L4C69;
    case 0x4C6C: goto L4C6C;
    case 0x4C70: goto L4C70;
    case 0x4C72: goto L4C72;
    case 0x4C75: goto L4C75;
    case 0x4C78: goto L4C78;
    case 0x4C79: goto L4C79;
    case 0x4C7E: goto L4C7E;
    case 0x4C83: goto L4C83;
    case 0x4C88: goto L4C88;
    case 0x4C8C: goto L4C8C;
    case 0x4C8E: goto L4C8E;
    case 0x4C92: goto L4C92;
    case 0x4C94: goto L4C94;
    case 0x4C99: goto L4C99;
    case 0x4C9D: goto L4C9D;
    case 0x4C9F: goto L4C9F;
    case 0x4CA3: goto L4CA3;
    case 0x4CA5: goto L4CA5;
    case 0x4CAA: goto L4CAA;
    case 0x4CAD: goto L4CAD;
    case 0x4CB1: goto L4CB1;
    case 0x4CB3: goto L4CB3;
    case 0x4CB6: goto L4CB6;
    case 0x4CB9: goto L4CB9;
    case 0x4CBA: goto L4CBA;
    case 0x4CBE: goto L4CBE;
    case 0x4CC2: goto L4CC2;
    case 0x4CC6: goto L4CC6;
    case 0x4CCA: goto L4CCA;
    case 0x4CCE: goto L4CCE;
    case 0x4CD3: goto L4CD3;
    case 0x4CD7: goto L4CD7;
    case 0x4CDC: goto L4CDC;
    case 0x4CE0: goto L4CE0;
    case 0x4CE4: goto L4CE4;
    case 0x4CE8: goto L4CE8;
    case 0x4CEC: goto L4CEC;
    case 0x4CF0: goto L4CF0;
    case 0x4CF4: goto L4CF4;
    case 0x4CF7: goto L4CF7;
    case 0x4CFA: goto L4CFA;
    case 0x4CFE: goto L4CFE;
    case 0x4CFF: goto L4CFF;
    case 0x4D02: goto L4D02;
    case 0x4D04: goto L4D04;
    case 0x4D06: goto L4D06;
    case 0x4D0A: goto L4D0A;
    case 0x4D0B: goto L4D0B;
    case 0x4D0F: goto L4D0F;
    case 0x4D13: goto L4D13;
    case 0x4D18: goto L4D18;
    case 0x4D1D: goto L4D1D;
    case 0x4D22: goto L4D22;
    case 0x4D26: goto L4D26;
    case 0x4D2A: goto L4D2A;
    case 0x4D2E: goto L4D2E;
    case 0x4D34: goto L4D34;
    case 0x4D38: goto L4D38;
    case 0x4D3C: goto L4D3C;
    case 0x4D40: goto L4D40;
    case 0x4D42: goto L4D42;
    case 0x4D45: goto L4D45;
    case 0x4D48: goto L4D48;
    case 0x4D4C: goto L4D4C;
    case 0x4D51: goto L4D51;
    case 0x4D55: goto L4D55;
    case 0x4D57: goto L4D57;
    case 0x4D58: goto L4D58;
    case 0x4D59: goto L4D59;
    case 0x4D5D: goto L4D5D;
    case 0x4D62: goto L4D62;
    case 0x4D65: goto L4D65;
    case 0x4D6A: goto L4D6A;
    case 0x4D6C: goto L4D6C;
    case 0x4D70: goto L4D70;
    case 0x4D74: goto L4D74;
    case 0x4D78: goto L4D78;
    case 0x4D7D: goto L4D7D;
    case 0x4D81: goto L4D81;
    case 0x4D83: goto L4D83;
    case 0x4D84: goto L4D84;
    case 0x4D85: goto L4D85;
    case 0x4D89: goto L4D89;
    case 0x4D8E: goto L4D8E;
    case 0x4D91: goto L4D91;
    case 0x4D96: goto L4D96;
    case 0x4D98: goto L4D98;
    case 0x4D9C: goto L4D9C;
    case 0x4DA0: goto L4DA0;
    case 0x4DA4: goto L4DA4;
    case 0x4DA8: goto L4DA8;
    case 0x4DAD: goto L4DAD;
    case 0x4DB2: goto L4DB2;
    case 0x4DB5: goto L4DB5;
    case 0x4DB6: goto L4DB6;
    case 0x4DBA: goto L4DBA;
    case 0x4DBF: goto L4DBF;
    case 0x4DC2: goto L4DC2;
    case 0x4DC3: goto L4DC3;
    case 0x4DC7: goto L4DC7;
    case 0x4DCB: goto L4DCB;
    case 0x4DCF: goto L4DCF;
    case 0x4DD3: goto L4DD3;
    case 0x4DD8: goto L4DD8;
    case 0x4DDB: goto L4DDB;
    case 0x4DDE: goto L4DDE;
    case 0x4DDF: goto L4DDF;
    case 0x4DE4: goto L4DE4;
    case 0x4DE5: goto L4DE5;
    case 0x4DE7: goto L4DE7;
    case 0x4DEB: goto L4DEB;
    case 0x4DEF: goto L4DEF;
    case 0x4DF1: goto L4DF1;
    case 0x4DF5: goto L4DF5;
    case 0x4DF9: goto L4DF9;
    case 0x4DFD: goto L4DFD;
    case 0x4E00: goto L4E00;
    case 0x4E03: goto L4E03;
    default: asm_bad_entry("PERTP.ASM", entry);
    }

    /* _asm_texture_map_scanline_per_tp: a perspective-correct unshaded span, one pair of divides (u
       * w / w, v * w / w) for every pixel. log2 width is written into the two `shl ax,2`
       instructions at L4C69+2 and L4CAA+2 (self-modifying code). Nothing is drawn if the right end
       is left of the left end or any u or v end is negative. */
L4B30: /* _asm_texture_map_scanline_per_tp */
    /* 4B30  mov     eax,dword ptr ds:[0CF74h] */
    EAX = rd(pDS, 0xCF74);
L4B34:
    /* 4B34  mov     dword ptr ds:[0CCB8h],eax */
    wd(pDS, 0xCCB8, EAX);
L4B38:
    /* 4B38  mov     eax,dword ptr ds:[0CF60h] */
    EAX = rd(pDS, 0xCF60);
L4B3C:
    /* 4B3C  mov     dword ptr ds:[0CCB0h],eax */
    wd(pDS, 0xCCB0, EAX);
L4B40:
    /* 4B40  mov     eax,dword ptr ds:[0CF64h] */
    EAX = rd(pDS, 0xCF64);
L4B44:
    /* 4B44  mov     dword ptr ds:[0CCB4h],eax */
    wd(pDS, 0xCCB4, EAX);
L4B48:
    /* 4B48  mov     eax,dword ptr ds:[0CF68h] */
    EAX = rd(pDS, 0xCF68);
L4B4C:
    /* 4B4C  sub     eax,dword ptr ds:[0CF5Ch] */
    EAX = sub32(EAX, rd(pDS, 0xCF5C), 0);
L4B51:
    /* 4B51  js      _seg004_0849_4CB9 */
    if (SF) goto L4CB9;
L4B55:
    /* 4B55  mov     ebx,dword ptr ds:[0CF68h] */
    EBX = rd(pDS, 0xCF68);
L4B5A:
    /* 4B5A  add     ebx,41h */
    EBX = (uint32_t)(EBX + 0x41);
L4B5E:
    /* 4B5E  sar     ebx,10h */
    EBX = (uint32_t)((int32_t)EBX >> 16);
L4B62:
    /* 4B62  mov     word ptr ds:[0CCC8h],bx */
    ww(pDS, 0xCCC8, BX);
L4B66:
    /* 4B66  mov     eax,dword ptr ds:[0CF5Ch] */
    EAX = rd(pDS, 0xCF5C);
L4B6A:
    /* 4B6A  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L4B6E:
    /* 4B6E  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L4B72:
    /* 4B72  mov     word ptr ds:[0CCC6h],ax */
    ww(pDS, 0xCCC6, AX);
L4B75:
    /* 4B75  mov     word ptr ds:[0CCC4h],ax */
    ww(pDS, 0xCCC4, AX);
L4B78:
    /* 4B78  mov     di,word ptr ds:[0CF5Ah] */
    DI = rw(pDS, 0xCF5A);
L4B7C:
    /* 4B7C  push    ds */
    push16(asm_ds);
L4B7D:
    /* 4B7D  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L4B80:
    /* 4B80  mov     ds,ax */
    SET_DS(AX);
L4B82:
    /* 4B82  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L4B84:
    /* 4B84  mov     di,word ptr [di+95Ch] */
    DI = rw(pDS, DI + 0x95C);
L4B88:
    /* 4B88  pop     ds */
    SET_DS(pop16());
L4B89:
    /* 4B89  add     di,word ptr ds:[0CCC4h] */
    DI = (uint16_t)(DI + rw(pDS, 0xCCC4));
L4B8D:
    /* 4B8D  mov     eax,dword ptr ds:[0CF60h] */
    EAX = rd(pDS, 0xCF60);
L4B91:
    /* 4B91  or      eax,dword ptr ds:[0CF6Ch] */
    EAX = (uint32_t)(EAX | rd(pDS, 0xCF6C));
L4B96:
    /* 4B96  or      eax,dword ptr ds:[0CF64h] */
    EAX = (uint32_t)(EAX | rd(pDS, 0xCF64));
L4B9B:
    /* 4B9B  or      eax,dword ptr ds:[0CF70h] */
    EAX = logic32((uint32_t)(EAX | rd(pDS, 0xCF70)));
L4BA0:
    /* 4BA0  js      _seg004_0849_4CB9 */
    if (SF) goto L4CB9;
L4BA4:
    /* 4BA4  sub     bx,word ptr ds:[0CCC4h] */
    BX = sub16(BX, rw(pDS, 0xCCC4), 0);
L4BA8:
    /* 4BA8  je      _seg004_0849_4C8E */
    if (ZF) goto L4C8E;
L4BAC:
    /* 4BAC  mov     eax,10000h */
    EAX = 0x10000;
L4BB2:
    /* 4BB2  shl     ebx,10h */
    EBX = (uint32_t)(EBX << 16);
L4BB6:
    /* 4BB6  rol     eax,10h */
    EAX = rol32(EAX, 16);
L4BBA:
    /* 4BBA  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L4BBE:
    /* 4BBE  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4BC0:
    /* 4BC0  idiv    ebx */
    if (asm_idiv32(EBX) && (c = asm_divfault(0x065C, 0x4BC0, 3)) != 0) return c;
L4BC3:
    /* 4BC3  mov     ebx,eax */
    EBX = EAX;
L4BC6:
    /* 4BC6  mov     eax,dword ptr ds:[0CF78h] */
    EAX = rd(pDS, 0xCF78);
L4BCA:
    /* 4BCA  sub     eax,dword ptr ds:[0CF74h] */
    EAX = (uint32_t)(EAX - rd(pDS, 0xCF74));
L4BCF:
    /* 4BCF  imul    ebx */
    imul32(EBX);
L4BD2:
    /* 4BD2  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L4BD7:
    /* 4BD7  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4BD9:
    /* 4BD9  mov     dword ptr ds:[0CCA8h],eax */
    wd(pDS, 0xCCA8, EAX);
L4BDD:
    /* 4BDD  mov     eax,dword ptr ds:[0CF6Ch] */
    EAX = rd(pDS, 0xCF6C);
L4BE1:
    /* 4BE1  xor     eax,dword ptr ds:[0CF60h] */
    EAX = (uint32_t)(EAX ^ rd(pDS, 0xCF60));
L4BE6:
    /* 4BE6  shr     eax,10h */
    EAX = shr32(EAX, 16);
L4BEA:
    /* 4BEA  je      _seg004_0849_4C01 */
    if (ZF) goto L4C01;
L4BEC:
    /* 4BEC  nop */
    ;
L4BED:
    /* 4BED  nop */
    ;
L4BEE:
    /* 4BEE  mov     eax,dword ptr ds:[0CF6Ch] */
    EAX = rd(pDS, 0xCF6C);
L4BF2:
    /* 4BF2  sub     eax,dword ptr ds:[0CF60h] */
    EAX = (uint32_t)(EAX - rd(pDS, 0xCF60));
L4BF7:
    /* 4BF7  imul    ebx */
    imul32(EBX);
L4BFA:
    /* 4BFA  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L4BFF:
    /* 4BFF  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4C01: /* _seg004_0849_4C01 */
    /* 4C01  mov     dword ptr ds:[0CCA0h],eax */
    wd(pDS, 0xCCA0, EAX);
L4C05:
    /* 4C05  mov     eax,dword ptr ds:[0CF70h] */
    EAX = rd(pDS, 0xCF70);
L4C09:
    /* 4C09  xor     eax,dword ptr ds:[0CF64h] */
    EAX = (uint32_t)(EAX ^ rd(pDS, 0xCF64));
L4C0E:
    /* 4C0E  shr     eax,10h */
    EAX = shr32(EAX, 16);
L4C12:
    /* 4C12  je      L4C29 */
    if (ZF) goto L4C29;
L4C14:
    /* 4C14  nop */
    ;
L4C15:
    /* 4C15  nop */
    ;
L4C16:
    /* 4C16  mov     eax,dword ptr ds:[0CF70h] */
    EAX = rd(pDS, 0xCF70);
L4C1A:
    /* 4C1A  sub     eax,dword ptr ds:[0CF64h] */
    EAX = (uint32_t)(EAX - rd(pDS, 0xCF64));
L4C1F:
    /* 4C1F  imul    ebx */
    imul32(EBX);
L4C22:
    /* 4C22  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L4C27:
    /* 4C27  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4C29: /* L4C29 */
    /* 4C29  mov     dword ptr ds:[0CCA4h],eax */
    wd(pDS, 0xCCA4, EAX);
L4C2D:
    /* 4C2D  mov     cx,word ptr ds:[0CCC8h] */
    CX = rw(pDS, 0xCCC8);
L4C31:
    /* 4C31  sub     cx,word ptr ds:[0CCC4h] */
    CX = (uint16_t)(CX - rw(pDS, 0xCCC4));
L4C35:
    /* 4C35  mov     ebx,dword ptr ds:[0CCB8h] */
    EBX = rd(pDS, 0xCCB8);
L4C3A:
    /* 4C3A  mov     ebp,dword ptr ds:[0CCB0h] */
    EBP = rd(pDS, 0xCCB0);
L4C3F:
    /* 4C3F  mov     word ptr ds:[0CCD0h],cx */
    ww(pDS, 0xCCD0, CX);
L4C43:
    /* 4C43  mov     ecx,dword ptr ds:[0CCB4h] */
    ECX = rd(pDS, 0xCCB4);
L4C48:
    /* 4C48  mov     al,byte ptr ds:[0CCECh] */
    AL = rb(pDS, 0xCCEC);
L4C4B:
    /* 4C4B  mov     byte ptr cs:[L4C69+2],al */
    wb(CODE004, 0x4C6B, AL);
L4C4F:
    /* 4C4F  mov     byte ptr cs:[L4CAA+2],al */
    wb(CODE004, 0x4CAC, AL);
L4C53: /* L4C53 */
    /* 4C53  mov     eax,ebp */
    EAX = EBP;
L4C56:
    /* 4C56  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4C58:
    /* 4C58  idiv    ebx */
    if (asm_idiv32(EBX) && (c = asm_divfault(0x065C, 0x4C58, 3)) != 0) return c;
L4C5B:
    /* 4C5B  and     ax,word ptr ds:[0CCE8h] */
    AX = (uint16_t)(AX & rw(pDS, 0xCCE8));
L4C5F:
    /* 4C5F  mov     si,ax */
    SI = AX;
L4C61:
    /* 4C61  mov     eax,ecx */
    EAX = ECX;
L4C64:
    /* 4C64  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4C66:
    /* 4C66  idiv    ebx */
    if (asm_idiv32(EBX) && (c = asm_divfault(0x065C, 0x4C66, 3)) != 0) return c;
L4C69: /* L4C69 */
    /* 4C69  shl     ax,2 */
    AX = (uint16_t)(AX << (rb(CODE004, 0x4C6B) & 31));
L4C6C:
    /* 4C6C  and     ax,word ptr ds:[0CCEAh] */
    AX = (uint16_t)(AX & rw(pDS, 0xCCEA));
L4C70:
    /* 4C70  add     si,ax */
    SI = (uint16_t)(SI + AX);
L4C72:
    /* 4C72  mov     al,byte ptr gs:[si] */
    AL = rb(pGS, SI);
L4C75:
    /* 4C75  mov     byte ptr es:[di],al */
    wb(pES, DI, AL);
L4C78:
    /* 4C78  inc     di */
    DI = (uint16_t)(DI + 1);
L4C79:
    /* 4C79  add     ebp,dword ptr ds:[0CCA0h] */
    EBP = (uint32_t)(EBP + rd(pDS, 0xCCA0));
L4C7E:
    /* 4C7E  add     ecx,dword ptr ds:[0CCA4h] */
    ECX = (uint32_t)(ECX + rd(pDS, 0xCCA4));
L4C83:
    /* 4C83  add     ebx,dword ptr ds:[0CCA8h] */
    EBX = (uint32_t)(EBX + rd(pDS, 0xCCA8));
L4C88:
    /* 4C88  dec     word ptr ds:[0CCD0h] */
    ww(pDS, 0xCCD0, dec16(rw(pDS, 0xCCD0)));
L4C8C:
    /* 4C8C  jne     L4C53 */
    if (!ZF) goto L4C53;
L4C8E: /* _seg004_0849_4C8E */
    /* 4C8E  mov     eax,dword ptr ds:[0CF6Ch] */
    EAX = rd(pDS, 0xCF6C);
L4C92:
    /* 4C92  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4C94:
    /* 4C94  idiv    dword ptr ds:[0CF78h] */
    if (asm_idiv32(rd(pDS, 0xCF78)) && (c = asm_divfault(0x065C, 0x4C94, 5)) != 0) return c;
L4C99:
    /* 4C99  and     ax,word ptr ds:[0CCE8h] */
    AX = (uint16_t)(AX & rw(pDS, 0xCCE8));
L4C9D:
    /* 4C9D  mov     si,ax */
    SI = AX;
L4C9F:
    /* 4C9F  mov     eax,dword ptr ds:[0CF70h] */
    EAX = rd(pDS, 0xCF70);
L4CA3:
    /* 4CA3  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4CA5:
    /* 4CA5  idiv    dword ptr ds:[0CF78h] */
    if (asm_idiv32(rd(pDS, 0xCF78)) && (c = asm_divfault(0x065C, 0x4CA5, 5)) != 0) return c;
L4CAA: /* L4CAA */
    /* 4CAA  shl     ax,2 */
    AX = (uint16_t)(AX << (rb(CODE004, 0x4CAC) & 31));
L4CAD:
    /* 4CAD  and     ax,word ptr ds:[0CCEAh] */
    AX = (uint16_t)(AX & rw(pDS, 0xCCEA));
L4CB1:
    /* 4CB1  add     si,ax */
    SI = add16(SI, AX, 0);
L4CB3:
    /* 4CB3  mov     al,byte ptr gs:[si] */
    AL = rb(pGS, SI);
L4CB6:
    /* 4CB6  mov     byte ptr es:[di],al */
    wb(pES, DI, AL);
L4CB9: /* _seg004_0849_4CB9 */
    /* 4CB9  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* _asm_texture_map_scanline_lin_tp: a linear unshaded span, one pixel per loop, v kept
       multiplied by the width. A nonzero u or v step gets 14h (about 1/3300 of a texel) added; the
       shaded and vertical routines do not do that. */
L4CBA: /* _asm_texture_map_scanline_lin_tp */
    /* 4CBA  mov     eax,dword ptr ds:[0CF60h] */
    EAX = rd(pDS, 0xCF60);
L4CBE:
    /* 4CBE  mov     dword ptr ds:[0CCB0h],eax */
    wd(pDS, 0xCCB0, EAX);
L4CC2:
    /* 4CC2  mov     eax,dword ptr ds:[0CF64h] */
    EAX = rd(pDS, 0xCF64);
L4CC6:
    /* 4CC6  mov     dword ptr ds:[0CCB4h],eax */
    wd(pDS, 0xCCB4, EAX);
L4CCA:
    /* 4CCA  mov     eax,dword ptr ds:[0CF68h] */
    EAX = rd(pDS, 0xCF68);
L4CCE:
    /* 4CCE  sub     eax,dword ptr ds:[0CF5Ch] */
    EAX = sub32(EAX, rd(pDS, 0xCF5C), 0);
L4CD3:
    /* 4CD3  js      L4E03 */
    if (SF) goto L4E03;
L4CD7:
    /* 4CD7  mov     ebx,dword ptr ds:[0CF68h] */
    EBX = rd(pDS, 0xCF68);
L4CDC:
    /* 4CDC  add     ebx,41h */
    EBX = (uint32_t)(EBX + 0x41);
L4CE0:
    /* 4CE0  sar     ebx,10h */
    EBX = (uint32_t)((int32_t)EBX >> 16);
L4CE4:
    /* 4CE4  mov     word ptr ds:[0CCC8h],bx */
    ww(pDS, 0xCCC8, BX);
L4CE8:
    /* 4CE8  mov     eax,dword ptr ds:[0CF5Ch] */
    EAX = rd(pDS, 0xCF5C);
L4CEC:
    /* 4CEC  add     eax,41h */
    EAX = (uint32_t)(EAX + 0x41);
L4CF0:
    /* 4CF0  sar     eax,10h */
    EAX = (uint32_t)((int32_t)EAX >> 16);
L4CF4:
    /* 4CF4  mov     word ptr ds:[0CCC6h],ax */
    ww(pDS, 0xCCC6, AX);
L4CF7:
    /* 4CF7  mov     word ptr ds:[0CCC4h],ax */
    ww(pDS, 0xCCC4, AX);
L4CFA:
    /* 4CFA  mov     di,word ptr ds:[0CF5Ah] */
    DI = rw(pDS, 0xCF5A);
L4CFE:
    /* 4CFE  push    ds */
    push16(asm_ds);
L4CFF:
    /* 4CFF  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L4D02:
    /* 4D02  mov     ds,ax */
    SET_DS(AX);
L4D04:
    /* 4D04  shl     di,1 */
    DI = (uint16_t)(DI << 1);
L4D06:
    /* 4D06  mov     di,word ptr [di+95Ch] */
    DI = rw(pDS, DI + 0x95C);
L4D0A:
    /* 4D0A  pop     ds */
    SET_DS(pop16());
L4D0B:
    /* 4D0B  add     di,word ptr ds:[0CCC4h] */
    DI = (uint16_t)(DI + rw(pDS, 0xCCC4));
L4D0F:
    /* 4D0F  mov     eax,dword ptr ds:[0CF60h] */
    EAX = rd(pDS, 0xCF60);
L4D13:
    /* 4D13  or      eax,dword ptr ds:[0CF6Ch] */
    EAX = (uint32_t)(EAX | rd(pDS, 0xCF6C));
L4D18:
    /* 4D18  or      eax,dword ptr ds:[0CF64h] */
    EAX = (uint32_t)(EAX | rd(pDS, 0xCF64));
L4D1D:
    /* 4D1D  or      eax,dword ptr ds:[0CF70h] */
    EAX = logic32((uint32_t)(EAX | rd(pDS, 0xCF70)));
L4D22:
    /* 4D22  js      L4E03 */
    if (SF) goto L4E03;
L4D26:
    /* 4D26  sub     bx,word ptr ds:[0CCC4h] */
    BX = sub16(BX, rw(pDS, 0xCCC4), 0);
L4D2A:
    /* 4D2A  je      L4DE7 */
    if (ZF) goto L4DE7;
L4D2E:
    /* 4D2E  mov     eax,10000h */
    EAX = 0x10000;
L4D34:
    /* 4D34  shl     ebx,10h */
    EBX = (uint32_t)(EBX << 16);
L4D38:
    /* 4D38  rol     eax,10h */
    EAX = rol32(EAX, 16);
L4D3C:
    /* 4D3C  movsx   edx,ax */
    EDX = (uint32_t)(int16_t)AX;
L4D40:
    /* 4D40  sub     ax,ax */
    AX = (uint16_t)(AX - AX);
L4D42:
    /* 4D42  idiv    ebx */
    if (asm_idiv32(EBX) && (c = asm_divfault(0x065C, 0x4D42, 3)) != 0) return c;
L4D45:
    /* 4D45  mov     ebx,eax */
    EBX = EAX;
L4D48:
    /* 4D48  mov     eax,dword ptr ds:[0CF6Ch] */
    EAX = rd(pDS, 0xCF6C);
L4D4C:
    /* 4D4C  xor     eax,dword ptr ds:[0CF60h] */
    EAX = (uint32_t)(EAX ^ rd(pDS, 0xCF60));
L4D51:
    /* 4D51  shr     eax,10h */
    EAX = shr32(EAX, 16);
L4D55:
    /* 4D55  je      L4D70 */
    if (ZF) goto L4D70;
L4D57:
    /* 4D57  nop */
    ;
L4D58:
    /* 4D58  nop */
    ;
L4D59:
    /* 4D59  mov     eax,dword ptr ds:[0CF6Ch] */
    EAX = rd(pDS, 0xCF6C);
L4D5D:
    /* 4D5D  sub     eax,dword ptr ds:[0CF60h] */
    EAX = (uint32_t)(EAX - rd(pDS, 0xCF60));
L4D62:
    /* 4D62  imul    ebx */
    imul32(EBX);
L4D65:
    /* 4D65  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L4D6A:
    /* 4D6A  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4D6C:
    /* 4D6C  add     eax,14h */
    EAX = (uint32_t)(EAX + 0x14);
L4D70: /* L4D70 */
    /* 4D70  mov     dword ptr ds:[0CCA0h],eax */
    wd(pDS, 0xCCA0, EAX);
L4D74:
    /* 4D74  mov     eax,dword ptr ds:[0CF70h] */
    EAX = rd(pDS, 0xCF70);
L4D78:
    /* 4D78  xor     eax,dword ptr ds:[0CF64h] */
    EAX = (uint32_t)(EAX ^ rd(pDS, 0xCF64));
L4D7D:
    /* 4D7D  shr     eax,10h */
    EAX = shr32(EAX, 16);
L4D81:
    /* 4D81  je      L4D9C */
    if (ZF) goto L4D9C;
L4D83:
    /* 4D83  nop */
    ;
L4D84:
    /* 4D84  nop */
    ;
L4D85:
    /* 4D85  mov     eax,dword ptr ds:[0CF70h] */
    EAX = rd(pDS, 0xCF70);
L4D89:
    /* 4D89  sub     eax,dword ptr ds:[0CF64h] */
    EAX = (uint32_t)(EAX - rd(pDS, 0xCF64));
L4D8E:
    /* 4D8E  imul    ebx */
    imul32(EBX);
L4D91:
    /* 4D91  shrd    eax,edx,10h */
    EAX = shrd32(EAX, EDX, 16);
L4D96:
    /* 4D96  cdq */
    EDX = (int32_t)EAX < 0 ? 0xFFFFFFFFu : 0;
L4D98:
    /* 4D98  add     eax,14h */
    EAX = (uint32_t)(EAX + 0x14);
L4D9C: /* L4D9C */
    /* 4D9C  mov     dword ptr ds:[0CCA4h],eax */
    wd(pDS, 0xCCA4, EAX);
L4DA0:
    /* 4DA0  mov     cx,word ptr ds:[0CCC8h] */
    CX = rw(pDS, 0xCCC8);
L4DA4:
    /* 4DA4  sub     cx,word ptr ds:[0CCC4h] */
    CX = sub16(CX, rw(pDS, 0xCCC4), 0);
L4DA8:
    /* 4DA8  mov     ebx,dword ptr ds:[0CCA0h] */
    EBX = rd(pDS, 0xCCA0);
L4DAD:
    /* 4DAD  mov     ebp,dword ptr ds:[0CCA4h] */
    EBP = rd(pDS, 0xCCA4);
L4DB2:
    /* 4DB2  mov     edx,ebx */
    EDX = EBX;
L4DB5:
    /* 4DB5  push    cx */
    push16(CX);
L4DB6:
    /* 4DB6  mov     cx,word ptr ds:[0CCECh] */
    CX = rw(pDS, 0xCCEC);
L4DBA:
    /* 4DBA  shl     dword ptr ds:[0CCB4h],cl */
    wd(pDS, 0xCCB4, shl32(rd(pDS, 0xCCB4), CL));
L4DBF:
    /* 4DBF  shl     ebp,cl */
    EBP = (uint32_t)(EBP << (CL & 31));
L4DC2:
    /* 4DC2  pop     cx */
    CX = pop16();
L4DC3: /* L4DC3 */
    /* 4DC3  mov     si,word ptr ds:[0CCB6h] */
    SI = rw(pDS, 0xCCB6);
L4DC7:
    /* 4DC7  and     si,word ptr ds:[0CCEAh] */
    SI = (uint16_t)(SI & rw(pDS, 0xCCEA));
L4DCB:
    /* 4DCB  mov     bx,word ptr ds:[0CCB2h] */
    BX = rw(pDS, 0xCCB2);
L4DCF:
    /* 4DCF  and     bx,word ptr ds:[0CCE8h] */
    BX = (uint16_t)(BX & rw(pDS, 0xCCE8));
L4DD3:
    /* 4DD3  add     dword ptr ds:[0CCB0h],edx */
    wd(pDS, 0xCCB0, (uint32_t)(rd(pDS, 0xCCB0) + EDX));
L4DD8:
    /* 4DD8  mov     al,byte ptr gs:[bx+si] */
    AL = rb(pGS, BX + SI);
L4DDB:
    /* 4DDB  mov     byte ptr es:[di],al */
    wb(pES, DI, AL);
L4DDE:
    /* 4DDE  inc     di */
    DI = (uint16_t)(DI + 1);
L4DDF:
    /* 4DDF  add     dword ptr ds:[0CCB4h],ebp */
    wd(pDS, 0xCCB4, add32(rd(pDS, 0xCCB4), EBP, 0));
L4DE4:
    /* 4DE4  dec     cx */
    CX = dec16(CX);
L4DE5:
    /* 4DE5  jne     L4DC3 */
    if (!ZF) goto L4DC3;
L4DE7: /* L4DE7 */
    /* 4DE7  mov     si,word ptr ds:[0CF72h] */
    SI = rw(pDS, 0xCF72);
L4DEB:
    /* 4DEB  mov     cx,word ptr ds:[0CCECh] */
    CX = rw(pDS, 0xCCEC);
L4DEF:
    /* 4DEF  shl     si,cl */
    SI = (uint16_t)(SI << (CL & 31));
L4DF1:
    /* 4DF1  and     si,word ptr ds:[0CCEAh] */
    SI = (uint16_t)(SI & rw(pDS, 0xCCEA));
L4DF5:
    /* 4DF5  mov     bx,word ptr ds:[0CF6Eh] */
    BX = rw(pDS, 0xCF6E);
L4DF9:
    /* 4DF9  and     bx,word ptr ds:[0CCE8h] */
    BX = logic16((uint16_t)(BX & rw(pDS, 0xCCE8)));
L4DFD:
    /* 4DFD  mov     al,byte ptr gs:[bx+si] */
    AL = rb(pGS, BX + SI);
L4E00:
    /* 4E00  mov     byte ptr es:[di],al */
    wb(pES, DI, AL);
L4E03: /* L4E03 */
    /* 4E03  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
}
