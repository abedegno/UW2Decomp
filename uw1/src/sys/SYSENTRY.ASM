; /* target: seg019_750 */
; /* opts: /ml */
;
; UW1: UW2's module (src/sys in UW2Decomp), matched to UW.EXE's seg019; the differences are
; marked UW1/UW2 where they occur. Other remarks are UW2's unless they say UW1.
;
; The far C entry points into seg019's input and start-up code: key, mouse and mbuttons, the
; joystick reads JOYSTICK.C calls (_7CD, _809), and _755 and _791, which GRFX.C's grfx_init and
; grfx_close use to run STARTUP.ASM's start-up and shut-down glue. Each saves the caller's SS:SP
; in L0750/L0752, loads DS, ES and SS with seg063 and SP with 448h (UW2 510h), calls the near
; routine and switches back (STARTUP.ASM's header describes the arrangement). Its _DATA is the
; table of far pointers through which C reaches seg019's data (joy_position, joy_buttons,
; Shift, CapsLock, Alt, Ctrl, key_on, Asc, MouseDx, MouseDy, MouseOn, Time, cPerror,
; cExitMessage), at the start of the second library's data. UW1 has no cJoyInit (UW2's third
; pointer, to FD71:01A8): the table has 17 pointers where UW2's has 18.
;
; The data segment seg063 (UW1 5624:0000; UW2's FD71, 60B9:0000), as the seg019 and seg004
; code uses it. Offsets from UW1's code and pointers unless marked UW2:
;   0010  Asc: the scan code to character table (256 bytes, +80h for shifted)
;   0110  the CPU type (CPUTYPE.ASM)     0120  a flag read by the keyboard and joystick set-up
;   0190  cPerror                         0192  the exit hook SYSINIT.ASM calls
;   01A0..01D2  the joystick (JOYPORT.ASM)
;   01E0..01E9  the keyboard state: repeat flag, modifiers (Shift 01E2, Alt 01E3, Ctrl 01E4,
;         01E5..01E7), CapsLock 01E8, layout 01E9; 01EB key_on, a down count per scan code
;   026B..02A9  KEYQUEUE.ASM's read pointer and tables
;   0300..033F  the scan code ring buffer (KBDINT.ASM)
;   0350..0355  the saved video mode (VIDSAVE.ASM)   0360, 0362 the mouse (MOUSEDRV.ASM)
;   0448  the top of the private stack all the entries use; 0448, 044A MouseDx and MouseDy
;         (mouse stores the motion there); 044C cExitMessage (UW2 0510, 0512, 0514)
;   04D0  the 3D renderer's int 0 handler (UW2 05A0, INSTANCE.ASM)
;   04E0..  the sine table; 0762 the arcsine and 0864 the arccosine tables (IMATH.ASM; UW2
;         05B0, 0832, 0934)
;   0970  cPlayer, the camera (UW2 0A40); 09C8 a pointer to the 3D view's window, which
;         cInit3d makes the clip window (UW2 0A98)
;   0A92  the save area for the private stack (C3DENTRY.ASM's _FDD, below 0A92; UW2 0C2A)
;   UW2 only, not checked in UW1: 0C30..0C36 the clip window set_the_window copies there,
;         0C38.. the screen mode and size.
;
; The 0D4h zero bytes at the start of _DATA (DS:2258..232B; UW2 0CEh, DS:204E..211B) are a
; stack: C3DENTRY.ASM's _102B runs MousQUp on it during a frame.
;
; name: descriptive (module 13 of seg019, "far C entry points (key, mouse, mbuttons)"); key,
; mouse and mbuttons are FM Towns names.
;
; seg019 split into its original modules, as UW2's seg021: module 13 of 17, +750..+A30
; of the segment (file 22CF0h), a module of the second library (UWLIB.LIB).
; Procs: seg019_755, seg019_791, seg019_7CD, seg019_809, key, mouse,
; mbuttons.
; Evidence for the bounds: starts on a paragraph after 15 zero bytes following a jmp: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; Its relocations are entries 2576-2587 of UW.EXE's relocation table.
; Its _DATA is DS:2258..2370 (relocation entries 2559-2575), defined before the code because
; those entries come before the code's.
;
; DS:2258..232B (zeros; nothing names them, but C3DENTRY.ASM's _102B runs on them as a
; stack, its SP at joy_position) is placed here: it is the second library's data before this
; module's first pointer, and no module linked earlier from the library has data in DGROUP.
; The link showed where the C library's data ends: DS:2242..2257 are the _DATA of NEARHEAP,
; SETARGV and SETENVP (6, 14 and 2 zero bytes), which TLINK takes from CM.LIB after CVTFAK,
; so the run here is 0D4h bytes, not the 0EAh this module had before the link.

        .186

        extrn   _seg019_710:far
        extrn   seg063:far

_DATA   segment word public 'DATA'
        db      0D4h dup (0)                    ; UW2 0CEh
_joy_position label dword
        dd      seg063+1A0h
_joy_buttons label dword
        dd      seg063+1B0h
_Shift label dword
        dd      seg063+1E2h
_CapsLock label dword
        dd      seg063+1E8h
_Alt label dword
        dd      seg063+1E3h
_Ctrl label dword
        dd      seg063+1E4h
_key_on label dword
        dd      seg063+1EBh
        dd      seg063+1E5h
        dd      seg063+1E6h
        dd      seg063+1E7h
_Asc label dword
        dd      seg063+10h
_MouseDx label dword
        dd      seg063+448h
_MouseDy label dword
        dd      seg063+44Ah
_MouseOn label dword
        dd      seg063+362h
_Time label dword
        dd      _seg019_710
_cPerror label dword
        dd      seg063+190h
_cExitMessage label dword
        dd      seg063+44Ch
_DATA   ends
DGROUP  group   _DATA

SEG019_TEXT segment para public 'CODE'
        assume  cs:SEG019_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _diskerr:near
        extrn   _Read_Mouse_Motion_seg019_6E7:near
        extrn   _seg019_0:near
        extrn   _seg019_17:near
        extrn   _seg019_1D0:near
        extrn   _seg019_236:near
        extrn   _seg019_2EC:near
        extrn   _seg019_3A3:near
        extrn   _seg019_4A8:near
        extrn   _seg019_598:near
        extrn   _seg019_5C8:near
        extrn   _seg019_703:near
        public  _Alt
        public  _Asc
        public  _CapsLock
        public  _cExitMessage
        public  _cPerror
        public  _Ctrl
        public  _joy_buttons
        public  _joy_position
        public  _key_on
        public  _MouseDx
        public  _MouseDy
        public  _MouseOn
        public  _Shift
        public  _Time
        public  _key
        public  _mbuttons
        public  _mouse
        public  _seg019_755
        public  _seg019_791
        public  _seg019_7CD
        public  _seg019_809

L0750:
        db      0h,0h
L0752:
        db      0h,0h
        ret

; seg019_755  (+755)
; _755: run STARTUP.ASM's _0 (start-up) on the private stack; GRFX.C's grfx_init calls it.
_seg019_755 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        call    _seg019_0
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg019_755 endp

; seg019_791  (+791)
; _791: run STARTUP.ASM's _17 (shut-down); grfx_close calls it.
_seg019_791 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        call    _seg019_17
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg019_791 endp

; seg019_7CD  (+7CD)
; _7CD: read the joystick position into joy_position (JOYPORT.ASM's _3A3); JOYSTICK.C calls it.
_seg019_7CD proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        call    _seg019_3A3
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg019_7CD endp

; seg019_809  (+809)
; _809: read the joystick buttons into joy_buttons (JOYPORT.ASM's _2EC). After its retf come
; five more entries of the same form with no labels, so nothing outside can call them:
; KBDINT.ASM's install (+845) and remove (+881), BIOSKEY.ASM's _236 (+8BD), the CPU test (+8F9)
; and CRITERR.ASM's diskerr (+935). SYSINIT.ASM calls the near routines directly.
_seg019_809 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        call    _seg019_2EC
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        call    _seg019_598
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        call    _seg019_5C8
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        call    _seg019_236
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        call    _seg019_1D0
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        call    _diskerr
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg019_809 endp

; seg019_971  (+971)
; key (FM Towns): the next key event (KEYQUEUE.ASM's _4A8): AX = 0 when there is none, else AL =
; the character and AH = the scan code.
_key proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        call    _seg019_4A8
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_key endp

; seg019_9AD  (+9AD)
; mouse (FM Towns): read the mouse motion (MOUSEDRV.ASM's _6E7) into MouseDx and MouseDy
; (seg063:0448, 044A; UW2 0510, 0512).
_mouse proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        call    _Read_Mouse_Motion_seg019_6E7
        mov     word ptr ds:[448h],ax
        mov     word ptr ds:[44Ah],bx
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_mouse endp

; Mouse_Button_Status_seg019_9F0  (+9F0)
; mbuttons (FM Towns): the mouse buttons and position (MOUSEDRV.ASM's _703).
_mbuttons proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        call    _seg019_703
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_mbuttons endp

SEG019_TEXT ends

        end
