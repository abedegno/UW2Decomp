/* grmisc.c: replaces src/gfx/GRMISC.ASM (seg003_0272_0, 0000..00EC of its
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

uint32_t asm_mod_GRMISC(uint16_t entry)
{
    uint32_t c;
    (void)c;
    switch (entry) {
    case 0x0000: goto L0000;
    case 0x0003: goto L0003;
    case 0x0008: goto L0008;
    case 0x000B: goto L000B;
    case 0x0010: goto L0010;
    case 0x0013: goto L0013;
    case 0x0018: goto L0018;
    case 0x001B: goto L001B;
    case 0x001E: goto L001E;
    case 0x0021: goto L0021;
    case 0x0024: goto L0024;
    case 0x0027: goto L0027;
    case 0x0028: goto L0028;
    case 0x002B: goto L002B;
    case 0x0030: goto L0030;
    case 0x0032: goto L0032;
    case 0x0036: goto L0036;
    case 0x0039: goto L0039;
    case 0x003B: goto L003B;
    case 0x003E: goto L003E;
    case 0x0040: goto L0040;
    case 0x0041: goto L0041;
    case 0x0043: goto L0043;
    case 0x0045: goto L0045;
    case 0x0046: goto L0046;
    case 0x004B: goto L004B;
    case 0x004E: goto L004E;
    case 0x004F: goto L004F;
    case 0x0050: goto L0050;
    case 0x0060: goto L0060;
    default: asm_bad_entry("GRMISC.ASM", entry);
    }
L0000: /* _seg003_0272_0 */
    /* 0000  mov     bp,offset _seg003_0272_32B4 */
    BP = 0x32B4;
L0003:
    /* 0003  db      0EAh */
    return ASM_JMP(0x0085, 0x5311);

    /* seg003_0272_8  (+8)
       _8: draw a line from (AX,BX) to (CX,DX) without clipping (GRLIBH's _375C, FM Towns uline),
       through the dispatcher. Used by do_lnres when both projected ends are inside the view. */
L0008: /* _seg003_0272_8 */
    /* 0008  mov     bp,offset _seg003_0272_375C */
    BP = 0x375C;
L000B:
    /* 000B  db      0EAh */
    return ASM_JMP(0x0085, 0x5311);

    /* seg003_0272_10  (+10)
       _10: draw a line from (AX,BX) to (CX,DX) clipped to the window (GRLIBG's _368E, FM Towns
       cline), through the dispatcher. Used by do_lnres when an end may lie outside the view. */
L0010: /* _seg003_0272_10 */
    /* 0010  mov     bp,offset _seg003_0272_368E */
    BP = 0x368E;
L0013:
    /* 0013  db      0EAh */
    return ASM_JMP(0x0085, 0x5311);

    /* seg003_0272_18  (+18)
       _18 (far): runs _46 (stamp the tick count) through GRDISP's _52FF. seg021's cRender calls it
       after render_3d, so DS:652 holds the time the last 3D frame finished drawing.

       L001E does the same with the null routine L004F; no reference to it was found. The far routine
       at +24 (call _28, retf) has no label, so nothing in the sources jumps to it directly; no
       reference to its address was found either. */
L0018: /* _seg003_0272_18 */
    /* 0018  mov     bp,offset _seg003_0272_46 */
    BP = 0x46;
L001B:
    /* 001B  jmp     _seg003_0272_52FF */
    return ASM_JMP(0x0085, 0x52FF);
L001E: /* L001E */
    /* 001E  mov     bp,offset L004F */
    BP = 0x4F;
L0021:
    /* 0021  jmp     _seg003_0272_52FF */
    return ASM_JMP(0x0085, 0x52FF);
L0024:
    /* 0024  call    _seg003_0272_28 */
    if ((c = asm_call(ASM_JMP(0x0085, 0x0028), 0x0027)) != 0) return c;
L0027:
    /* 0027  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;

    /* seg003_0272_28  (+28)
       _28: wait for the start of the next vertical retrace (input status port 3DAh, bit 3), but
       return at once when the tick counter (seg021's _C00, TICKREAD) is 4 or more ticks away from
       the value _46 stored at DS:652. In: DS = seg_370D. Out: AX, DX changed. It keeps a frame that
       is already late from waiting a further retrace. */
L0028: /* _seg003_0272_28 */
    /* 0028  mov     dx,INPUT_STATUS */
    DX = 0x3DA;
L002B: /* L002B */
    /* 002B  call    far ptr _seg021_22FD_C00 */
    if ((c = asm_callf(ASM_JMP(0x2110, 0x0C00), 0x0085 + PORT_LOAD_SEG, 0x0030)) != 0) return c;
L0030:
    /* 0030  neg     ax */
    AX = (uint16_t)-AX;
L0032:
    /* 0032  add     ax,word ptr ds:[652h] */
    AX = (uint16_t)(AX + rw(pDS, 0x652));
L0036:
    /* 0036  cmp     ax,4 */
    sub16(AX, 0x4, 0);
L0039:
    /* 0039  jge     L0045 */
    if (SF == OF) goto L0045;
L003B:
    /* 003B  cmp     ax,0FFFCh */
    sub16(AX, 0xFFFC, 0);
L003E:
    /* 003E  jle     L0045 */
    if (ZF || SF != OF) goto L0045;
L0040:
    /* 0040  in      al,dx */
    AL = asm_in8(DX);
L0041:
    /* 0041  test    al,8 */
    logic8((uint8_t)(AL & 0x8));
L0043:
    /* 0043  je      L002B */
    if (ZF) goto L002B;
L0045: /* L0045 */
    /* 0045  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;

    /* seg003_0272_46  (+46)
       _46: store the tick count (seg021's _C00) at DS:652. Run by _18 after each 3D frame.

       The bytes after it, to the end of the module, are data in the code segment:
       - L004F: a near ret (the null routine L001E passes the dispatcher) and a far retf.
       - _60: int 2, then 'map bitmap error' and two dwords (0 and 12345678h): the default
         bitmap-mapper routine. int 2 is the NMI vector, used here as a break into a debugger.
       - _7A .. _94: words POLYFILL's wall mapper (FM Towns wmap_bitmap) stores with cs: overrides as
         it sets up a polygon (bounds, vertex count, vertex pointers, step values); nothing reads them.
         With _96 (the caller's ES) and _98 (a copy of the 54h-byte vertex list at 370D:0658, six
         14-byte points) they are probably a record of the last polygon for inspecting in a debugger
         after the 'map bitmap error' break. */
L0046: /* _seg003_0272_46 */
    /* 0046  call    far ptr _seg021_22FD_C00 */
    if ((c = asm_callf(ASM_JMP(0x2110, 0x0C00), 0x0085 + PORT_LOAD_SEG, 0x004B)) != 0) return c;
L004B:
    /* 004B  mov     word ptr ds:[652h],ax */
    ww(pDS, 0x652, AX);
L004E:
    /* 004E  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L004F: /* L004F */
    /* 004F  ret */
    SP = (uint16_t)(SP + 2); return ASM_RET;
L0050:
    /* 0050  retf */
    SP = (uint16_t)(SP + 4); return ASM_RETF;
L0060: /* _seg003_0272_60 */
    /* 0060  int     2 */
    asm_halt_at(0x0085, 0x0060, "int 2h, the debugger break");
    asm_halt_at(0x0085, 0x0062, "ran off the code into data");
}
