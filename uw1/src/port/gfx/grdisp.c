/* grdisp.c: replaces src/gfx/GRDISP.ASM (seg003_5AE8, 5AE8..5B69 of its
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

uint32_t asm_mod_GRDISP(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x5AE8: goto L5AE8;
    case 0x5AEB: goto L5AEB;
    case 0x5AEC: goto L5AEC;
    case 0x5AED: goto L5AED;
    case 0x5AF0: goto L5AF0;
    case 0x5AF2: goto L5AF2;
    case 0x5AF4: goto L5AF4;
    case 0x5AF5: goto L5AF5;
    case 0x5AF6: goto L5AF6;
    case 0x5AF9: goto L5AF9;
    case 0x5AFA: goto L5AFA;
    case 0x5AFB: goto L5AFB;
    case 0x5AFD: goto L5AFD;
    case 0x5B00: goto L5B00;
    case 0x5B02: goto L5B02;
    case 0x5B03: goto L5B03;
    case 0x5B04: goto L5B04;
    case 0x5B06: goto L5B06;
    case 0x5B08: goto L5B08;
    case 0x5B0A: goto L5B0A;
    case 0x5B0B: goto L5B0B;
    case 0x5B0C: goto L5B0C;
    case 0x5B0D: goto L5B0D;
    case 0x5B0E: goto L5B0E;
    case 0x5B11: goto L5B11;
    case 0x5B13: goto L5B13;
    case 0x5B15: goto L5B15;
    case 0x5B16: goto L5B16;
    case 0x5B1A: goto L5B1A;
    case 0x5B1B: goto L5B1B;
    case 0x5B1F: goto L5B1F;
    case 0x5B23: goto L5B23;
    case 0x5B24: goto L5B24;
    case 0x5B26: goto L5B26;
    case 0x5B29: goto L5B29;
    case 0x5B2A: goto L5B2A;
    case 0x5B2C: goto L5B2C;
    case 0x5B30: goto L5B30;
    case 0x5B31: goto L5B31;
    case 0x5B32: goto L5B32;
    case 0x5B33: goto L5B33;
    case 0x5B34: goto L5B34;
    case 0x5B35: goto L5B35;
    case 0x5B38: goto L5B38;
    case 0x5B3A: goto L5B3A;
    case 0x5B3C: goto L5B3C;
    case 0x5B3D: goto L5B3D;
    case 0x5B41: goto L5B41;
    case 0x5B42: goto L5B42;
    case 0x5B46: goto L5B46;
    case 0x5B4A: goto L5B4A;
    case 0x5B4B: goto L5B4B;
    case 0x5B4D: goto L5B4D;
    case 0x5B50: goto L5B50;
    case 0x5B51: goto L5B51;
    case 0x5B53: goto L5B53;
    case 0x5B57: goto L5B57;
    case 0x5B58: goto L5B58;
    case 0x5B5A: goto L5B5A;
    case 0x5B5C: goto L5B5C;
    case 0x5B5D: goto L5B5D;
    case 0x5B60: goto L5B60;
    case 0x5B63: goto L5B63;
    case 0x5B65: goto L5B65;
    case 0x5B68: goto L5B68;
    default: asm_bad_entry("GRDISP.ASM", entry);
    }

    /* L5AE8: copy CX words from DS:SI to 3963:558C, then enter _5AF9 with SI = 558C. Reached only
       from _5B5D.

       _5AF9 (near, entered by jmp with a far return address on the stack, BP = routine): the
       dispatcher for callers on another stack. If SS already is seg048 the library has been
       re-entered; it then executes int 2 (a break into a debugger) and calls BP on the current
       stack. Otherwise it takes the path at L5B34 below, which ends with DS = ES = seg063
       rather than the caller's values. Used by GRMISC's _18. */
L5AE8: /* L5AE8 */
    /* 5AE8  mov     di,558Ch */
    DI = 0x558C;
L5AEB:
    /* 5AEB  push    ax */
    push16(AX);
L5AEC:
    /* 5AEC  push    es */
    push16(asm_es);
L5AED:
    /* 5AED  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L5AF0:
    /* 5AF0  mov     es,ax */
    SET_ES(AX);
L5AF2:
    /* 5AF2  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L5AF4:
    /* 5AF4  pop     es */
    SET_ES(pop16());
L5AF5:
    /* 5AF5  pop     ax */
    AX = pop16();
L5AF6:
    /* 5AF6  mov     si,558Ch */
    SI = 0x558C;
L5AF9: /* _seg003_5AF9 */
    /* 5AF9  push    ax */
    push16(AX);
L5AFA:
    /* 5AFA  push    bx */
    push16(BX);
L5AFB:
    /* 5AFB  mov     ax,ss */
    AX = asm_ss;
L5AFD:
    /* 5AFD  mov     bx,seg seg048 */
    BX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L5B00:
    /* 5B00  cmp     ax,bx */
    sub16(AX, BX, 0);
L5B02:
    /* 5B02  pop     bx */
    BX = pop16();
L5B03:
    /* 5B03  pop     ax */
    AX = pop16();
L5B04:
    /* 5B04  jne     L5B34 */
    if (!ZF) goto L5B34;
L5B06:
    /* 5B06  int     2 */
    asm_halt_at(0x0090, 0x5B06, "int 2h, the debugger break");
L5B08:
    /* 5B08  call    bp */
    if ((c = asm_call(ASM_JMP(0x0090, BP), 0x5B0A)) != 0) return c;
L5B0A:
    /* 5B0A  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_5B0B  (+5B0B)
       _5B0B (far, entered by GRMISC's thunks with jmp far): save DS and ES, set DS = ES = seg048,
       save SP at DS:5588, switch to the private stack [558A]:[5586] with interrupts off, call BP,
       switch back to SS = seg063 (FD72, seg019's data segment, where cRender's private stack
       is) and the saved SP, restore DS and ES, retf. AX..DX, SI and DI reach the routine unchanged
       and come back as it leaves them. */
L5B0B: /* _seg003_5B0B */
    /* 5B0B  push    ds */
    push16(asm_ds);
L5B0C:
    /* 5B0C  push    es */
    push16(asm_es);
L5B0D:
    /* 5B0D  push    ax */
    push16(AX);
L5B0E:
    /* 5B0E  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L5B11:
    /* 5B11  mov     ds,ax */
    SET_DS(AX);
L5B13:
    /* 5B13  mov     es,ax */
    SET_ES(AX);
L5B15:
    /* 5B15  pop     ax */
    AX = pop16();
L5B16:
    /* 5B16  mov     word ptr ds:[5588h],sp */
    ww(pDS, 0x5588, SP);
L5B1A:
    /* 5B1A  cli */
    ;
L5B1B:
    /* 5B1B  mov     ss,word ptr ds:[558Ah] */
    SET_SS(rw(pDS, 0x558A));
L5B1F:
    /* 5B1F  mov     sp,word ptr ds:[5586h] */
    SP = rw(pDS, 0x5586);
L5B23:
    /* 5B23  sti */
    ;
L5B24:
    /* 5B24  call    bp */
    if ((c = asm_call(ASM_JMP(0x0090, BP), 0x5B26)) != 0) return c;
L5B26:
    /* 5B26  mov     bp,seg seg063 */
    BP = (uint16_t)(0x5624 + PORT_LOAD_SEG);
L5B29:
    /* 5B29  cli */
    ;
L5B2A:
    /* 5B2A  mov     ss,bp */
    SET_SS(BP);
L5B2C:
    /* 5B2C  mov     sp,word ptr ds:[5588h] */
    SP = rw(pDS, 0x5588);
L5B30:
    /* 5B30  sti */
    ;
L5B31:
    /* 5B31  pop     es */
    SET_ES(pop16());
L5B32:
    /* 5B32  pop     ds */
    SET_DS(pop16());
L5B33:
    /* 5B33  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;
L5B34: /* L5B34 */
    /* 5B34  push    ax */
    push16(AX);
L5B35:
    /* 5B35  mov     ax,seg seg048 */
    AX = (uint16_t)(0x3963 + PORT_LOAD_SEG);
L5B38:
    /* 5B38  mov     ds,ax */
    SET_DS(AX);
L5B3A:
    /* 5B3A  mov     es,ax */
    SET_ES(AX);
L5B3C:
    /* 5B3C  pop     ax */
    AX = pop16();
L5B3D:
    /* 5B3D  mov     word ptr ds:[5588h],sp */
    ww(pDS, 0x5588, SP);
L5B41:
    /* 5B41  cli */
    ;
L5B42:
    /* 5B42  mov     ss,word ptr ds:[558Ah] */
    SET_SS(rw(pDS, 0x558A));
L5B46:
    /* 5B46  mov     sp,word ptr ds:[5586h] */
    SP = rw(pDS, 0x5586);
L5B4A:
    /* 5B4A  sti */
    ;
L5B4B:
    /* 5B4B  call    bp */
    if ((c = asm_call(ASM_JMP(0x0090, BP), 0x5B4D)) != 0) return c;
L5B4D:
    /* 5B4D  mov     bp,seg seg063 */
    BP = (uint16_t)(0x5624 + PORT_LOAD_SEG);
L5B50:
    /* 5B50  cli */
    ;
L5B51:
    /* 5B51  mov     ss,bp */
    SET_SS(BP);
L5B53:
    /* 5B53  mov     sp,word ptr ds:[5588h] */
    SP = rw(pDS, 0x5588);
L5B57:
    /* 5B57  sti */
    ;
L5B58:
    /* 5B58  mov     ds,bp */
    SET_DS(BP);
L5B5A:
    /* 5B5A  mov     es,bp */
    SET_ES(BP);
L5B5C:
    /* 5B5C  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_5B5D  (+5B5D)
       _5B5D: set the clip window from the 4 words (left, top, right, bottom) at the caller's DS:SI,
       through the dispatcher: CX = 4 words are copied to 3963:558C and the window routine (_5ADE,
       VIDMODE's _3A68) is called with SI pointing at the copy. No caller was found in the sources. */
L5B5D: /* _seg003_5B5D */
    /* 5B5D  mov     cx,4 */
    CX = 0x4;
L5B60:
    /* 5B60  mov     bp,offset _seg003_5ADE */
    BP = 0x5ADE;
L5B63:
    /* 5B63  jmp     L5AE8 */
    goto L5AE8;

    /* seg003_5B65  (+5B65)
       _5B65 (far): call _5AB1 (set_the_color's vector, VIDMODE's _3A16) with the caller's DS and
       stack, without switching; it is only correct when DS already is seg048. No caller was found
       in the sources. */
L5B65: /* _seg003_5B65 */
    /* 5B65  call    _seg003_5AB1 */
    if ((c = asm_call(ASM_JMP(0x0090, 0x5AB1), 0x5B68)) != 0) return c;
L5B68:
    /* 5B68  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;
}
