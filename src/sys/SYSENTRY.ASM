; /* target: seg021_22FD_750 */
; /* opts: /ml */
;
; The far C entry points that switch to the module's private stack (_755, _791, _7CD, _809),
; key, mouse and mbuttons. Its _DATA is the table of far pointers C reaches the module's data
; through (joy_position, cJoyInit, Shift, Alt, Ctrl, key_on, Asc, MouseDx, MouseDy, MouseOn,
; Time, cPerror, cExitMessage), at the start of the second library's data.
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
; DS:204E..211B (zeros, nothing refers to them) is placed here: it is the second library's
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
