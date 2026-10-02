; /* target: seg021_22FD_6C0 */
; /* opts: /ml */
;
; The mouse driver interface (int 33h): reset, motion (scaled by the sensitivity), position
; and buttons, with far thunks for C.
;
; SEG021 split into its original modules: this is module 10 of 17, +6C0..+710 of the segment
; (file 243C0h), a module of the second library (UWLIB.LIB).
; Procs: seg021_22FD_6C0, seg021_22FD_6C4, seg021_22FD_6C8, Read_Mouse_Motion_seg021_6E7,
; seg021_22FD_6F8, seg021_22FD_703.
; Evidence for the bounds: starts on a paragraph after 9 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG021_TEXT segment para public 'CODE'
        assume  cs:SEG021_TEXT, ds:DGROUP, ss:DGROUP
        public  _Read_Mouse_Motion_seg021_6E7
        public  _seg021_22FD_6C0
        public  _seg021_22FD_6C4
        public  _seg021_22FD_6C8
        public  _seg021_22FD_6F8
        public  _seg021_22FD_703

; seg021_22FD_6C0  (+6C0)
_seg021_22FD_6C0 proc far
        call    _Read_Mouse_Motion_seg021_6E7
        retf
_seg021_22FD_6C0 endp

; seg021_22FD_6C4  (+6C4)
_seg021_22FD_6C4 proc far
        call    _seg021_22FD_703
        retf
_seg021_22FD_6C4 endp

; seg021_22FD_6C8  (+6C8)
_seg021_22FD_6C8 proc near
        push    es
        sub     ax,ax
        mov     es,ax
        mov     di,0CCh
        scasw
        jne     short L06D6
        scasw
        je      short L06DF
L06D6:
        int     33h
        test    ax,ax
        je      short L06DF
        mov     ax,0C8h
L06DF:
        mov     word ptr ds:[360h],ax
        mov     word ptr ds:[362h],ax
        pop     es
        ret
_seg021_22FD_6C8 endp

; Read_Mouse_Motion_seg021_6E7  (+6E7)
_Read_Mouse_Motion_seg021_6E7 proc near
        mov     ax,0Bh
        int     33h
        push    dx
        xchg    cx,ax
        call    _seg021_22FD_6F8
        pop     ax
        push    bx
        call    _seg021_22FD_6F8
        pop     ax
        ret
_Read_Mouse_Motion_seg021_6E7 endp

; Mouse_Sensitivity_Maybe_seg021_6F8  (+6F8)
_seg021_22FD_6F8 proc near
        mov     bx,64h
        imul    bx
        idiv    word ptr ds:[360h]
        xchg    bx,ax
        ret
_seg021_22FD_6F8 endp

; Mouse_Position_And_Button_status_seg021_703  (+703)
_seg021_22FD_703 proc near
        mov     ax,3
        int     33h
        xchg    bx,ax
        and     ax,3
        ret
_seg021_22FD_703 endp

SEG021_TEXT ends

        end
