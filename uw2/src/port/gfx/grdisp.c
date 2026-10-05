/* grdisp.c: replaces src/gfx/GRDISP.ASM (seg003_0272_52EE, 52EE..536F of its
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
    case 0x52EE: goto L52EE;
    case 0x52F1: goto L52F1;
    case 0x52F2: goto L52F2;
    case 0x52F3: goto L52F3;
    case 0x52F6: goto L52F6;
    case 0x52F8: goto L52F8;
    case 0x52FA: goto L52FA;
    case 0x52FB: goto L52FB;
    case 0x52FC: goto L52FC;
    case 0x52FF: goto L52FF;
    case 0x5300: goto L5300;
    case 0x5301: goto L5301;
    case 0x5303: goto L5303;
    case 0x5306: goto L5306;
    case 0x5308: goto L5308;
    case 0x5309: goto L5309;
    case 0x530A: goto L530A;
    case 0x530C: goto L530C;
    case 0x530E: goto L530E;
    case 0x5310: goto L5310;
    case 0x5311: goto L5311;
    case 0x5312: goto L5312;
    case 0x5313: goto L5313;
    case 0x5314: goto L5314;
    case 0x5317: goto L5317;
    case 0x5319: goto L5319;
    case 0x531B: goto L531B;
    case 0x531C: goto L531C;
    case 0x5320: goto L5320;
    case 0x5321: goto L5321;
    case 0x5325: goto L5325;
    case 0x5329: goto L5329;
    case 0x532A: goto L532A;
    case 0x532C: goto L532C;
    case 0x532F: goto L532F;
    case 0x5330: goto L5330;
    case 0x5332: goto L5332;
    case 0x5336: goto L5336;
    case 0x5337: goto L5337;
    case 0x5338: goto L5338;
    case 0x5339: goto L5339;
    case 0x533A: goto L533A;
    case 0x533B: goto L533B;
    case 0x533E: goto L533E;
    case 0x5340: goto L5340;
    case 0x5342: goto L5342;
    case 0x5343: goto L5343;
    case 0x5347: goto L5347;
    case 0x5348: goto L5348;
    case 0x534C: goto L534C;
    case 0x5350: goto L5350;
    case 0x5351: goto L5351;
    case 0x5353: goto L5353;
    case 0x5356: goto L5356;
    case 0x5357: goto L5357;
    case 0x5359: goto L5359;
    case 0x535D: goto L535D;
    case 0x535E: goto L535E;
    case 0x5360: goto L5360;
    case 0x5362: goto L5362;
    case 0x536B: goto L536B;
    case 0x536E: goto L536E;
    default: asm_bad_entry("GRDISP.ASM", entry);
    }

    /* L52EE: copy CX words from DS:SI to 370D:558E, then enter _52FF with SI = 558E. Reached only
       from _5363.

       _52FF (near, entered by jmp with a far return address on the stack, BP = routine): the
       dispatcher for callers on another stack. If SS already is seg_370D the library has been
       re-entered; it then executes int 2 (a break into a debugger) and calls BP on the current
       stack. Otherwise it takes the path at L533A below, which ends with DS = ES = dseg062_62a6
       rather than the caller's values. Used by GRMISC's _18. */
L52EE: /* L52EE */
    /* 52EE  mov     di,558Eh */
    DI = 0x558E;
L52F1:
    /* 52F1  push    ax */
    push16(AX);
L52F2:
    /* 52F2  push    es */
    push16(asm_es);
L52F3:
    /* 52F3  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L52F6:
    /* 52F6  mov     es,ax */
    SET_ES(AX);
L52F8:
    /* 52F8  rep movsw */
    while (CX) { ww(pES, DI, rw(pDS, SI)); SI = (uint16_t)(SI + STEP(2)); DI = (uint16_t)(DI + STEP(2)); CX--; }
L52FA:
    /* 52FA  pop     es */
    SET_ES(pop16());
L52FB:
    /* 52FB  pop     ax */
    AX = pop16();
L52FC:
    /* 52FC  mov     si,558Eh */
    SI = 0x558E;
L52FF: /* _seg003_0272_52FF */
    /* 52FF  push    ax */
    push16(AX);
L5300:
    /* 5300  push    bx */
    push16(BX);
L5301:
    /* 5301  mov     ax,ss */
    AX = asm_ss;
L5303:
    /* 5303  mov     bx,seg seg_370D */
    BX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L5306:
    /* 5306  cmp     ax,bx */
    sub16(AX, BX, 0);
L5308:
    /* 5308  pop     bx */
    BX = pop16();
L5309:
    /* 5309  pop     ax */
    AX = pop16();
L530A:
    /* 530A  jne     L533A */
    if (!ZF) goto L533A;
L530C:
    /* 530C  int     2 */
    asm_halt_at(0x0085, 0x530C, "int 2h, the debugger break");
L530E:
    /* 530E  call    bp */
    if ((c = asm_call(ASM_JMP(0x0085, BP), 0x5310)) != 0) return c;
L5310:
    /* 5310  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_0272_5311  (+5311)
       _5311 (far, entered by GRMISC's thunks with jmp far): save DS and ES, set DS = ES = seg_370D,
       save SP at DS:558A, switch to the private stack [558C]:[5588] with interrupts off, call BP,
       switch back to SS = dseg062_62a6 (FD71, seg021's data segment, where cRender's private stack
       is) and the saved SP, restore DS and ES, retf. AX..DX, SI and DI reach the routine unchanged
       and come back as it leaves them. */
L5311: /* _seg003_0272_5311 */
    /* 5311  push    ds */
    push16(asm_ds);
L5312:
    /* 5312  push    es */
    push16(asm_es);
L5313:
    /* 5313  push    ax */
    push16(AX);
L5314:
    /* 5314  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L5317:
    /* 5317  mov     ds,ax */
    SET_DS(AX);
L5319:
    /* 5319  mov     es,ax */
    SET_ES(AX);
L531B:
    /* 531B  pop     ax */
    AX = pop16();
L531C:
    /* 531C  mov     word ptr ds:[558Ah],sp */
    ww(pDS, 0x558A, SP);
L5320:
    /* 5320  cli */
    ;
L5321:
    /* 5321  mov     ss,word ptr ds:[558Ch] */
    SET_SS(rw(pDS, 0x558C));
L5325:
    /* 5325  mov     sp,word ptr ds:[5588h] */
    SP = rw(pDS, 0x5588);
L5329:
    /* 5329  sti */
    ;
L532A:
    /* 532A  call    bp */
    if ((c = asm_call(ASM_JMP(0x0085, BP), 0x532C)) != 0) return c;
L532C:
    /* 532C  mov     bp,seg dseg062_62a6 */
    BP = (uint16_t)(0x60B9 + PORT_LOAD_SEG);
L532F:
    /* 532F  cli */
    ;
L5330:
    /* 5330  mov     ss,bp */
    SET_SS(BP);
L5332:
    /* 5332  mov     sp,word ptr ds:[558Ah] */
    SP = rw(pDS, 0x558A);
L5336:
    /* 5336  sti */
    ;
L5337:
    /* 5337  pop     es */
    SET_ES(pop16());
L5338:
    /* 5338  pop     ds */
    SET_DS(pop16());
L5339:
    /* 5339  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;
L533A: /* L533A */
    /* 533A  push    ax */
    push16(AX);
L533B:
    /* 533B  mov     ax,seg seg_370D */
    AX = (uint16_t)(0x370D + PORT_LOAD_SEG);
L533E:
    /* 533E  mov     ds,ax */
    SET_DS(AX);
L5340:
    /* 5340  mov     es,ax */
    SET_ES(AX);
L5342:
    /* 5342  pop     ax */
    AX = pop16();
L5343:
    /* 5343  mov     word ptr ds:[558Ah],sp */
    ww(pDS, 0x558A, SP);
L5347:
    /* 5347  cli */
    ;
L5348:
    /* 5348  mov     ss,word ptr ds:[558Ch] */
    SET_SS(rw(pDS, 0x558C));
L534C:
    /* 534C  mov     sp,word ptr ds:[5588h] */
    SP = rw(pDS, 0x5588);
L5350:
    /* 5350  sti */
    ;
L5351:
    /* 5351  call    bp */
    if ((c = asm_call(ASM_JMP(0x0085, BP), 0x5353)) != 0) return c;
L5353:
    /* 5353  mov     bp,seg dseg062_62a6 */
    BP = (uint16_t)(0x60B9 + PORT_LOAD_SEG);
L5356:
    /* 5356  cli */
    ;
L5357:
    /* 5357  mov     ss,bp */
    SET_SS(BP);
L5359:
    /* 5359  mov     sp,word ptr ds:[558Ah] */
    SP = rw(pDS, 0x558A);
L535D:
    /* 535D  sti */
    ;
L535E:
    /* 535E  mov     ds,bp */
    SET_DS(BP);
L5360:
    /* 5360  mov     es,bp */
    SET_ES(BP);
L5362:
    /* 5362  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_0272_536B  (+536B)
       _536B (far): call _52B7 (set_the_color's vector, VIDMODE's _3195) with the caller's DS and
       stack, without switching; it is only correct when DS already is seg_370D. No caller was found
       in the sources. */
L536B: /* _seg003_0272_536B */
    /* 536B  call    _seg003_0272_52B7 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x52B7), 0x536E)) != 0) return c;
L536E:
    /* 536E  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;
}
