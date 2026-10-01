; /* target: seg021_22FD_1D0 */
; /* opts: /ml */
;
; CPU detection (_1D0): 8086, 80186, 80286 or 80386, stored at offset 110h of the module data
; segment.
;
; SEG021 split into its original modules: this is module 3 of 17, +1D0..+220 of the segment
; (file 23ED0h), a module of the second library (UWLIB.LIB).
; Procs: seg021_22FD_1D0.
; Evidence for the bounds: starts on a paragraph after 13 zero bytes following a iret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG021_TEXT segment para public 'CODE'
        assume  cs:SEG021_TEXT, ds:DGROUP, ss:DGROUP
        public  _seg021_22FD_1D0

; seg021_22FD_1D0  (+1D0)
_seg021_22FD_1D0 proc near
        pushf
        xor     ax,ax
        push    ax
        popf
        pushf
        pop     ax
        and     ax,0F000h
        cmp     ax,0F000h
        je      short L01F5
        mov     ax,7000h
        push    ax
        popf
        pushf
        pop     ax
        and     ax,7000h
        je      short L01F0
        mov     ax,386h
        jmp     short L0216
L01F0:
        mov     ax,286h
        jmp     short L0216
L01F5:
        push    cx
        mov     ax,0FFFFh
        mov     cl,21h
        shl     ax,cl
        jne     short L0212
        pop     cx
        xor     al,al
        mov     al,40h
        mul     al
        je      short L020D
        mov     ax,86h
        jmp     short L0216
L020D:
        mov     ax,30h
        jmp     short L0216
L0212:
        mov     ax,186h
        pop     cx
L0216:
        popf
        mov     word ptr ds:[110h],ax
        ret
_seg021_22FD_1D0 endp

SEG021_TEXT ends

        end
