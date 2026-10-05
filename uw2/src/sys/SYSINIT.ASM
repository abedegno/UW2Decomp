; /* target: seg021_22FD_240 */
; /* opts: /ml */
;
; init, its exit routine and errorexit (FM Towns names): installs the handlers, hooks the exit
; chain, and on exit restores everything and prints the error message.
;
; SEG021 split into its original modules: this is module 5 of 17, +240..+2E0 of the segment
; (file 23F40h), a module of the second library (UWLIB.LIB).
; Procs: seg021_22FD_240, init, seg021_22FD_2B6, seg021_22FD_2BD.
; Evidence for the bounds: starts on a paragraph after 4 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; Its relocations are entries 2276-2277 of UW2.EXE's relocation table.
; Runs with DS on seg021's data segment (see STARTUP.ASM and SYSENTRY.ASM).
; name: descriptive (system module 5); init and errorexit are the FM Towns names.

; The DOS (int 21h) functions used here.
DOS_PRINT_STRING equ     09h

        .186

        extrn   dseg062_62a6:far
        extrn   seg052_519C:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG021_TEXT segment para public 'CODE'
        assume  cs:SEG021_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg021_22FD_17:near
        extrn   _seg021_22FD_1D0:near
        extrn   _seg021_22FD_30A:near
        extrn   _seg021_22FD_598:near
        extrn   _seg021_22FD_5C8:near
        extrn   _seg021_22FD_6C8:near
        extrn   _seg021_22FD_730:near
        extrn   _setvect24:near
        public  _init
        public  _seg021_22FD_240
        public  _seg021_22FD_2B6
        public  _seg021_22FD_2BD

; seg021_22FD_240  (+240)
; Far entry to errorexit: shuts down with SI as the message to print at exit. Nothing in
; UW2 calls it.
_seg021_22FD_240 proc far
        call    errorexit
        retf
_seg021_22FD_240 endp

; seg021_22FD_244  (+244)
; init: brings the system layer up. In order: the mouse (_6C8), the CPU type (_1D0), the
; 3D data's segment (_2B6), the int 24h handler (_setvect24), the Tandy check (_2BD), the
; joystick (_30A), the int 9 keyboard handler (_598) and the video mode save (_730). Then
; it hooks program termination: the far pointer at DS:0 points at the PSP's terminate
; address (PSP:000A, see STARTUP.ASM); init copies the address there to DS:18Ch and
; writes its own exit routine, L028C, in its place, so DOS runs L028C whenever the
; program ends, however it ends.
; If the flag word at DS:370h is nonzero, start-up has failed: the message is the one
; already at DS:190h, or "No graphics adapter" (DS:560h), and errorexit shuts down. The
; only code that sets DS:370h is _730's `dec`, which a `clc; jae` always skips, so the
; graphics adapter check is disabled and this path is never taken.
; errorexit: SI is the message for the exit routine (DS:190h, cPerror), then shutdown
; (_17, STARTUP.ASM).
_init proc near
        call    _seg021_22FD_6C8
        call    _seg021_22FD_1D0
        call    _seg021_22FD_2B6
        call    _setvect24
        call    _seg021_22FD_2BD
        call    _seg021_22FD_30A
        call    _seg021_22FD_598
        call    _seg021_22FD_730
        push    ds
        lds     si,dword ptr ds:[0]
        mov     di,18Ch
        movsw
        movsw
        pop     ds
        push    es
        les     di,dword ptr ds:[0]
        mov     ax,offset L028C                 ; the exit routine below
        stosw
        mov     ax,cs
        stosw
        pop     es
        mov     cx,word ptr ds:[370h]
        jcxz    L028B
        cmp     word ptr ds:[190h],0
        jne     short errorexit
        mov     si,560h
errorexit:
        mov     word ptr ds:[190h],si
        jmp     _seg021_22FD_17
L028B:
        ret

; Reached through the far pointer init stored at DS:0 (the exit chain).
; The exit routine, entered from DOS at termination: loads seg021's data segment into DS,
; ES and SS with SP = 122h (a small stack below DS:122h), calls the hook at DS:192h (its
; initial value, 02BCh, is the `ret` at the end of _2B6, so it does nothing unless
; something stores a routine there; nothing in UW2 does), removes the int 9 handler (_5C8),
; prints the '$'-terminated message at DS:[190h] if it is not 0 (pfatal and the divide-by-
; zero handler put cExitMessage there), and jumps on to the original terminate address.
L028C:
        mov     ax,seg dseg062_62a6
        mov     es,ax
        mov     ds,ax
        cli
        mov     ss,ax
        mov     ax,122h
        mov     sp,ax
        sti
        call    word ptr ds:[192h]
        call    _seg021_22FD_5C8
        cmp     word ptr ds:[190h],0
        je      short L02B2
        mov     dx,word ptr ds:[190h]
        mov     ah,DOS_PRINT_STRING
        int     21h
L02B2:
        jmp     dword ptr ds:[18Ch]
_init endp

; seg021_22FD_2B6  (+2B6)
; Stores the segment of the 3D renderer's data (seg052_519C) at DS:18Ah. Its final ret,
; at +2BC, doubles as the default exit hook (see above).
_seg021_22FD_2B6 proc near
        mov     ax,seg seg052_519C
        mov     word ptr ds:[18Ah],ax
        ret
_seg021_22FD_2B6 endp

; seg021_22FD_2BD  (+2BD)
; Tandy check: the far pointer at DS:186h is F000:FFFE, the BIOS model byte. If that is
; FFh (IBM PC) and the byte at F000:C000 is 21h, the machine is taken to be a Tandy 1000
; (presumably: that is the usual Tandy test) and DS:120h is set to FFFFh. KEYQUEUE.ASM
; then translates the Tandy keyboard's scan codes and JOYPORT.ASM treats the joystick
; differently.
_seg021_22FD_2BD proc near
        push    ds
        lds     si,dword ptr ds:[186h]
        lodsb
        cmp     al,0FFh
        jne     short L02D6
        mov     si,0C000h
        lodsb
        cmp     al,21h
        jne     short L02D6
        mov     word ptr ss:[120h],0FFFFh
L02D6:
        pop     ds
        ret
_seg021_22FD_2BD endp

SEG021_TEXT ends

        end
