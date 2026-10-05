/* lpfdelta.c: replaces src/gfx/LPFDELTA.ASM (seg002, 000A..009C of its
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

uint32_t asm_mod_LPFDELTA(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x000A: goto L000A;
    case 0x000B: goto L000B;
    case 0x000D: goto L000D;
    case 0x000E: goto L000E;
    case 0x000F: goto L000F;
    case 0x0010: goto L0010;
    case 0x0013: goto L0013;
    case 0x0016: goto L0016;
    case 0x0019: goto L0019;
    case 0x001A: goto L001A;
    case 0x001D: goto L001D;
    case 0x001F: goto L001F;
    case 0x0021: goto L0021;
    case 0x0024: goto L0024;
    case 0x0026: goto L0026;
    case 0x0028: goto L0028;
    case 0x002A: goto L002A;
    case 0x002B: goto L002B;
    case 0x002D: goto L002D;
    case 0x002F: goto L002F;
    case 0x0031: goto L0031;
    case 0x0033: goto L0033;
    case 0x0035: goto L0035;
    case 0x0036: goto L0036;
    case 0x0038: goto L0038;
    case 0x003A: goto L003A;
    case 0x003C: goto L003C;
    case 0x003E: goto L003E;
    case 0x003F: goto L003F;
    case 0x0040: goto L0040;
    case 0x0042: goto L0042;
    case 0x0044: goto L0044;
    case 0x0045: goto L0045;
    case 0x0047: goto L0047;
    case 0x0049: goto L0049;
    case 0x004B: goto L004B;
    case 0x004D: goto L004D;
    case 0x004E: goto L004E;
    case 0x0050: goto L0050;
    case 0x0052: goto L0052;
    case 0x0054: goto L0054;
    case 0x0056: goto L0056;
    case 0x0058: goto L0058;
    case 0x005A: goto L005A;
    case 0x005D: goto L005D;
    case 0x0060: goto L0060;
    case 0x0062: goto L0062;
    case 0x0066: goto L0066;
    case 0x0068: goto L0068;
    case 0x0069: goto L0069;
    case 0x006A: goto L006A;
    case 0x006C: goto L006C;
    case 0x006E: goto L006E;
    case 0x0070: goto L0070;
    case 0x0072: goto L0072;
    case 0x0074: goto L0074;
    case 0x0076: goto L0076;
    case 0x0077: goto L0077;
    case 0x0079: goto L0079;
    case 0x007C: goto L007C;
    case 0x007D: goto L007D;
    case 0x007F: goto L007F;
    case 0x0083: goto L0083;
    case 0x0085: goto L0085;
    case 0x0086: goto L0086;
    case 0x0087: goto L0087;
    case 0x0089: goto L0089;
    case 0x008B: goto L008B;
    case 0x008D: goto L008D;
    case 0x008F: goto L008F;
    case 0x0091: goto L0091;
    case 0x0092: goto L0092;
    case 0x0094: goto L0094;
    case 0x0095: goto L0095;
    case 0x0097: goto L0097;
    case 0x0098: goto L0098;
    case 0x0099: goto L0099;
    case 0x009A: goto L009A;
    case 0x009B: goto L009B;
    default: asm_bad_entry("LPFDELTA.ASM", entry);
    }
L000A: /* _seg002_A */
    /* 000A  push    bp */
    push16(BP);
L000B:
    /* 000B  mov     bp,sp */
    BP = SP;
L000D:
    /* 000D  push    si */
    push16(SI);
L000E:
    /* 000E  push    di */
    push16(DI);
L000F:
    /* 000F  push    es */
    push16(asm_es);
L0010:
    /* 0010  mov     si,word ptr [bp+6] */
    SI = rw(pSS, BP + 0x6);
L0013:
    /* 0013  mov     di,word ptr [bp+0Ah] */
    DI = rw(pSS, BP + 0xA);
L0016:
    /* 0016  mov     es,word ptr [bp+0Ch] */
    SET_ES(rw(pSS, BP + 0xC));
L0019:
    /* 0019  push    ds */
    push16(asm_ds);
L001A:
    /* 001A  mov     ds,word ptr [bp+8] */
    SET_DS(rw(pSS, BP + 0x8));
L001D:
    /* 001D  sub     ch,ch */
    CH = (uint8_t)(CH - CH);
L001F:
    /* 001F  jmp     short L0020 */
    goto L0028;
L0021: /* L0019 */
    /* 0021  sub     cl,80h */
    CL = sub8(CL, 0x80, 0);
L0024:
    /* 0024  je      short L0045 */
    if (ZF) goto L004D;
L0026:
    /* 0026  add     di,cx */
    DI = (uint16_t)(DI + CX);
L0028: /* L0020 */
    /* 0028  mov     cl,byte ptr [si] */
    CL = rb(pDS, SI);
L002A:
    /* 002A  inc     si */
    SI = (uint16_t)(SI + 1);
L002B:
    /* 002B  jcxz    L0034 */
    if (!CX) goto L003C;
L002D:
    /* 002D  or      cl,cl */
    CL = logic8((uint8_t)(CL | CL));
L002F:
    /* 002F  jl      L0019 */
    if (SF != OF) goto L0021;
L0031: /* L0029 */
    /* 0031  rep movsb */
    while (CX) { wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1)); CX--; }
L0033:
    /* 0033  mov     cl,byte ptr [si] */
    CL = rb(pDS, SI);
L0035:
    /* 0035  inc     si */
    SI = (uint16_t)(SI + 1);
L0036:
    /* 0036  or      cl,cl */
    CL = logic8((uint8_t)(CL | CL));
L0038:
    /* 0038  jl      L0019 */
    if (SF != OF) goto L0021;
L003A:
    /* 003A  jg      L0029 */
    if (!ZF && SF == OF) goto L0031;
L003C: /* L0034 */
    /* 003C  mov     cl,byte ptr [si] */
    CL = rb(pDS, SI);
L003E:
    /* 003E  inc     si */
    SI = (uint16_t)(SI + 1);
L003F:
    /* 003F  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L0040:
    /* 0040  rep stosb */
    while (CX) { wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1)); CX--; }
L0042:
    /* 0042  mov     cl,byte ptr [si] */
    CL = rb(pDS, SI);
L0044:
    /* 0044  inc     si */
    SI = (uint16_t)(SI + 1);
L0045:
    /* 0045  jcxz    L0034 */
    if (!CX) goto L003C;
L0047:
    /* 0047  or      cl,cl */
    CL = logic8((uint8_t)(CL | CL));
L0049:
    /* 0049  jl      L0019 */
    if (SF != OF) goto L0021;
L004B:
    /* 004B  jmp     L0029 */
    goto L0031;
L004D: /* L0045 */
    /* 004D  lodsw */
    AX = rw(pDS, SI); SI = (uint16_t)(SI + STEP(2));
L004E:
    /* 004E  or      ax,ax */
    AX = logic16((uint16_t)(AX | AX));
L0050:
    /* 0050  jle     short L004E */
    if (ZF || SF != OF) goto L0056;
L0052:
    /* 0052  add     di,ax */
    DI = (uint16_t)(DI + AX);
L0054:
    /* 0054  jmp     L0020 */
    goto L0028;
L0056: /* L004E */
    /* 0056  je      short L008C */
    if (ZF) goto L0094;
L0058:
    /* 0058  mov     cx,ax */
    CX = AX;
L005A:
    /* 005A  sub     ch,80h */
    CH = (uint8_t)(CH - 0x80);
L005D:
    /* 005D  cmp     ch,40h */
    sub8(CH, 0x40, 0);
L0060:
    /* 0060  jge     short L0071 */
    if (SF == OF) goto L0079;
L0062:
    /* 0062  test    si,1 */
    logic16((uint16_t)(SI & 0x1));
L0066:
    /* 0066  je      short L0062 */
    if (ZF) goto L006A;
L0068:
    /* 0068  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0069:
    /* 0069  dec     cx */
    CX = (uint16_t)(CX - 1);
L006A: /* L0062 */
    /* 006A  shr     cx,1 */
    CX = shr16(CX, 1);
L006C:
    /* 006C  jb      short L006C */
    if (CF) goto L0074;
L006E:
    /* 006E  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0070: /* L0068 */
    /* 0070  sub     ch,ch */
    CH = (uint8_t)(CH - CH);
L0072:
    /* 0072  jmp     L0020 */
    goto L0028;
L0074: /* L006C */
    /* 0074  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0076:
    /* 0076  movsb */
    wb(pES, DI, rb(pDS, SI)); SI = (uint16_t)(SI + STEP(1)); DI = (uint16_t)(DI + STEP(1));
L0077:
    /* 0077  jmp     L0068 */
    goto L0070;
L0079: /* L0071 */
    /* 0079  sub     ch,40h */
    CH = (uint8_t)(CH - 0x40);
L007C:
    /* 007C  lodsb */
    AL = rb(pDS, SI); SI = (uint16_t)(SI + STEP(1));
L007D:
    /* 007D  mov     ah,al */
    AH = AL;
L007F:
    /* 007F  test    di,1 */
    logic16((uint16_t)(DI & 0x1));
L0083:
    /* 0083  je      short L007F */
    if (ZF) goto L0087;
L0085:
    /* 0085  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0086:
    /* 0086  dec     cx */
    CX = (uint16_t)(CX - 1);
L0087: /* L007F */
    /* 0087  shr     cx,1 */
    CX = shr16(CX, 1);
L0089:
    /* 0089  jb      short L0087 */
    if (CF) goto L008F;
L008B:
    /* 008B  rep stosw */
    while (CX) { ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2)); CX--; }
L008D:
    /* 008D  jmp     L0068 */
    goto L0070;
L008F: /* L0087 */
    /* 008F  rep stosw */
    while (CX) { ww(pES, DI, AX); DI = (uint16_t)(DI + STEP(2)); CX--; }
L0091:
    /* 0091  stosb */
    wb(pES, DI, AL); DI = (uint16_t)(DI + STEP(1));
L0092:
    /* 0092  jmp     L0068 */
    goto L0070;
L0094: /* L008C */
    /* 0094  pop     ds */
    SET_DS(pop16());
L0095:
    /* 0095  mov     ax,di */
    AX = DI;
L0097:
    /* 0097  pop     es */
    SET_ES(pop16());
L0098:
    /* 0098  pop     di */
    DI = pop16();
L0099:
    /* 0099  pop     si */
    SI = pop16();
L009A:
    /* 009A  pop     bp */
    BP = pop16();
L009B:
    /* 009B  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;
}
