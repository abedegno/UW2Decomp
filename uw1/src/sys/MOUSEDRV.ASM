; /* target: seg019_6C0 */
; /* opts: /ml */
;
; UW1: UW2's module (src/sys in UW2Decomp), matched to UW.EXE's seg019; the differences are
; marked UW1/UW2 where they occur. Other remarks are UW2's unless they say UW1.
;
; The mouse through the DOS mouse driver (int 33h): reset, relative motion scaled for the game,
; position and buttons. FM Towns has initmouse, mouse, mbuttons and resetmouse in this place;
; SYSENTRY.ASM's far C entries (mouse, mbuttons) and the start-up code call these. Data in seg063:
; 0360 the motion divisor (200 with a mouse, 0 without), 0362 MouseOn (the C side's _MouseOn
; points here).
;
; name: descriptive (module 10 of seg019, "the int 33h mouse driver interface").
;
; seg019 split into its original modules, as UW2's seg021: module 10 of 17, +6C0..+710
; of the segment (file 22C60h), a module of the second library (UWLIB.LIB).
; Procs: seg019_6C0, seg019_6C4, seg019_6C8, Read_Mouse_Motion_seg019_6E7,
; seg019_6F8, seg019_703.
; Evidence for the bounds: starts on a paragraph after 9 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

; The mouse driver (int 33h) functions used here.
MOUSE_STATUS    equ     03h
MOUSE_MOTION    equ     0Bh

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG019_TEXT segment para public 'CODE'
        assume  cs:SEG019_TEXT, ds:DGROUP, ss:DGROUP
        public  _Read_Mouse_Motion_seg019_6E7
        public  _seg019_6C0
        public  _seg019_6C4
        public  _seg019_6C8
        public  _seg019_6F8
        public  _seg019_703

; seg019_6C0  (+6C0)
; _6C0: far entry to _6E7 (the motion).
_seg019_6C0 proc far
        call    _Read_Mouse_Motion_seg019_6E7
        retf
_seg019_6C0 endp

; seg019_6C4  (+6C4)
; _6C4: far entry to _703 (position and buttons).
_seg019_6C4 proc far
        call    _seg019_703
        retf
_seg019_6C4 endp

; seg019_6C8  (+6C8)
; _6C8 (initmouse, probably): if the int 33h vector (0000:00CC) is not null, reset the driver
; (function 0). A driver that answers sets both 0360 and 0362 to 200; otherwise both are 0 (and
; the game runs without a mouse).
_seg019_6C8 proc near
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
_seg019_6C8 endp

; Read_Mouse_Motion_seg019_6E7  (+6E7)
; _6E7: the motion since the last call (function 0Bh, mickeys), each scaled by 100 / 200 (_6F8),
; so half a mickey per unit. Out: AX = x, BX = y.
_Read_Mouse_Motion_seg019_6E7 proc near
        mov     ax,MOUSE_MOTION
        int     33h
        push    dx
        xchg    cx,ax
        call    _seg019_6F8
        pop     ax
        push    bx
        call    _seg019_6F8
        pop     ax
        ret
_Read_Mouse_Motion_seg019_6E7 endp

; Mouse_Sensitivity_Maybe_seg019_6F8  (+6F8)
; _6F8: BX = AX * 100 / [0360]. The divisor is the 200 _6C8 stored; nothing else found writes
; it, so the old name's "sensitivity" is a guess.
_seg019_6F8 proc near
        mov     bx,64h
        imul    bx
        idiv    word ptr ds:[360h]
        xchg    bx,ax
        ret
_seg019_6F8 endp

; Mouse_Position_And_Button_status_seg019_703  (+703)
; _703: the position and buttons (function 3). Out: AX = the left and right buttons (bits 0, 1),
; CX, DX = x, y as the driver gives them.
_seg019_703 proc near
        mov     ax,MOUSE_STATUS
        int     33h
        xchg    bx,ax
        and     ax,3
        ret
_seg019_703 endp

SEG019_TEXT ends

        end
