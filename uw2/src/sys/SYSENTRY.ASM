; /* target: seg021_22FD_750 */
; /* opts: /ml */
;
; The far C entry points into seg021's input and start-up code: key, mouse and mbuttons, the
; joystick reads JOYSTICK.C calls (_7CD, _809), and _755 and _791, which GRFX.C's grfx_init and
; grfx_close use to run STARTUP.ASM's start-up and shut-down glue. Each saves the caller's SS:SP
; in L0750/L0752, loads DS, ES and SS with dseg062_62a6 (FD71) and SP with 510h, calls the near
; routine and switches back (STARTUP.ASM's header describes the arrangement). Its _DATA is the
; table of far pointers through which C reaches seg021's data (joy_position, joy_buttons,
; cJoyInit, Shift, CapsLock, Alt, Ctrl, key_on, Asc, MouseDx, MouseDy, MouseOn, Time, cPerror,
; cExitMessage), at the start of the second library's data.
;
; The data segment FD71 (dseg062_62a6, 60B9:0000, C40h bytes), as the seg021 and seg004 code
; uses it:
;   0010  Asc: the scan code to character table (256 bytes, +80h for shifted)
;   0110  the CPU type (CPUTYPE.ASM)     0120  a flag read by the keyboard and joystick set-up
;   0190  cPerror                         0192  the exit hook SYSINIT.ASM calls
;   01A0..01D2  the joystick (JOYPORT.ASM)
;   01E0..01E9  the keyboard state: repeat flag, modifiers (Shift 01E2, Alt 01E3, Ctrl 01E4,
;         01E5..01E7), CapsLock 01E8, layout 01E9; 01EB key_on, a down count per scan code
;   026B..02A9  KEYQUEUE.ASM's read pointer and tables
;   0300..033F  the scan code ring buffer (KBDINT.ASM)
;   0350..0355  the saved video mode (VIDSAVE.ASM)   0360, 0362 the mouse (MOUSEDRV.ASM)
;   0510  the top of the private stack all the entries use; 0510, 0512 MouseDx and MouseDy
;         (mouse stores the motion there); 0514 cExitMessage
;   05A0  the 3D renderer's int 0 handler, jmp far cs:[05A5] (INSTANCE.ASM)
;   05B0..  the sine table; 0832 the arcsine and 0934 the arccosine tables (IMATH.ASM)
;   0A40  cPlayer, the camera (view3d.h struct Camera); 0A98 a pointer to the 3D view's window, which cInit3d makes the clip window
;   0C2A  the save area for the private stack (C3DENTRY.ASM's _FDD, below 0C2A)
;   0C30..0C36  the clip window set_the_window copies here; 0C38.. the screen mode and size
;
; The 0CEh zero bytes at the start of _DATA (DS:204E..211B) are a stack: C3DENTRY.ASM's _102B
; runs MousQUp on it during a frame.
;
; name: descriptive (module 13 of seg021, "far C entry points (key, mouse, mbuttons)"); key,
; mouse and mbuttons are FM Towns names.
;
; SEG021 split into its original modules: this is module 13 of 17, +750..+A30 of the segment
; (file 24450h), a module of the second library (UWLIB.LIB).
; Procs: seg021_22FD_755, seg021_22FD_791, seg021_22FD_7CD, seg021_22FD_809, key, mouse,
; mbuttons.
; Evidence for the bounds: starts on a paragraph after 15 zero bytes following a jmp: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; Its relocations are entries 2298-2309 of UW2.EXE's relocation table.
; Its _DATA is DS:204E..2164 (relocation entries 2280-2297), defined before the code because
; those entries come before the code's.
;
; DS:204E..211B (zeros; nothing names them, but C3DENTRY.ASM's _102B runs on them as a
; stack, its SP at joy_position) is placed here: it is the second library's
; data before this module's first pointer, and no module linked earlier from the library has
; data in DGROUP; which module it belonged to is not known.

        .186

        extrn   _seg021_22FD_710:far
        extrn   dseg062_62a6:far

_DATA   segment word public 'DATA'
        db      0CEh dup (0)
_joy_position label dword
        dd      dseg062_62a6+1A0h
_joy_buttons label dword
        dd      dseg062_62a6+1B0h
_cJoyInit label dword
        dd      dseg062_62a6+1A8h
_Shift label dword
        dd      dseg062_62a6+1E2h
_CapsLock label dword
        dd      dseg062_62a6+1E8h
_Alt label dword
        dd      dseg062_62a6+1E3h
_Ctrl label dword
        dd      dseg062_62a6+1E4h
_key_on label dword
        dd      dseg062_62a6+1EBh
        dd      dseg062_62a6+1E5h
        dd      dseg062_62a6+1E6h
        dd      dseg062_62a6+1E7h
_Asc label dword
        dd      dseg062_62a6+10h
_MouseDx label dword
        dd      dseg062_62a6+510h
_MouseDy label dword
        dd      dseg062_62a6+512h
_MouseOn label dword
        dd      dseg062_62a6+362h
_Time label dword
        dd      _seg021_22FD_710
_cPerror label dword
        dd      dseg062_62a6+190h
_cExitMessage label dword
        dd      dseg062_62a6+514h
_DATA   ends
DGROUP  group   _DATA

SEG021_TEXT segment para public 'CODE'
        assume  cs:SEG021_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _diskerr:near
        extrn   _Read_Mouse_Motion_seg021_6E7:near
        extrn   _seg021_22FD_0:near
        extrn   _seg021_22FD_17:near
        extrn   _seg021_22FD_1D0:near
        extrn   _seg021_22FD_236:near
        extrn   _seg021_22FD_2EC:near
        extrn   _seg021_22FD_3A3:near
        extrn   _seg021_22FD_4A8:near
        extrn   _seg021_22FD_598:near
        extrn   _seg021_22FD_5C8:near
        extrn   _seg021_22FD_703:near
        public  _Alt
        public  _Asc
        public  _CapsLock
        public  _cExitMessage
        public  _cJoyInit
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
        public  _seg021_22FD_755
        public  _seg021_22FD_791
        public  _seg021_22FD_7CD
        public  _seg021_22FD_809

L0750:
        db      0h,0h
L0752:
        db      0h,0h
        ret

; seg021_22FD_755  (+755)
; _755: run STARTUP.ASM's _0 (start-up) on the private stack; GRFX.C's grfx_init calls it.
_seg021_22FD_755 proc far
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
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_0
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
_seg021_22FD_755 endp

; seg021_22FD_791  (+791)
; _791: run STARTUP.ASM's _17 (shut-down); grfx_close calls it.
_seg021_22FD_791 proc far
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
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_17
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
_seg021_22FD_791 endp

; seg021_22FD_7CD  (+7CD)
; _7CD: read the joystick position into joy_position (JOYPORT.ASM's _3A3); JOYSTICK.C calls it.
_seg021_22FD_7CD proc far
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
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_3A3
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
_seg021_22FD_7CD endp

; seg021_22FD_809  (+809)
; _809: read the joystick buttons into joy_buttons (JOYPORT.ASM's _2EC). After its retf come
; five more entries of the same form with no labels, so nothing outside can call them:
; KBDINT.ASM's install (+845) and remove (+881), BIOSKEY.ASM's _236 (+8BD), the CPU test (+8F9)
; and CRITERR.ASM's diskerr (+935). SYSINIT.ASM calls the near routines directly.
_seg021_22FD_809 proc far
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
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_2EC
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
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_598
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
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_5C8
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
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_236
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
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_1D0
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
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
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
_seg021_22FD_809 endp

; seg021_22FD_971  (+971)
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
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_4A8
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

; seg021_22FD_9AD  (+9AD)
; mouse (FM Towns): read the mouse motion (MOUSEDRV.ASM's _6E7) into MouseDx and MouseDy
; (FD71:0510, 0512).
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
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _Read_Mouse_Motion_seg021_6E7
        mov     word ptr ds:[510h],ax
        mov     word ptr ds:[512h],bx
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

; Mouse_Button_Status_seg021_9F0  (+9F0)
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
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_703
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

SEG021_TEXT ends

        end
