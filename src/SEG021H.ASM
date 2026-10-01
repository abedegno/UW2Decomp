; /* target: seg021_22FD_590 */
; /* opts: /ml */
;
; Installing and removing the int 9 keyboard handler (_598, _5C8) and setting the keyboard
; LEDs (_5E9). It opens with its data: the far pointer to the key buffer (_590) and the saved
; int 9 vector.
;
; SEG021 split into its original modules: this is module 8 of 17, +590..+680 of the segment
; (file 24290h), a module of the second library (UWLIB.LIB).
; Procs: seg021_22FD_598, seg021_22FD_5C8, seg021_22FD_5E9.
; Evidence for the bounds: starts on a paragraph after 3 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; Its relocations are entries 2278-2279 of UW2.EXE's relocation table.

        .186

        extrn   dseg062_62a6:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG021_TEXT segment para public 'CODE'
        assume  cs:SEG021_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg021_22FD_46B:near
        public  _seg021_22FD_590
        public  _seg021_22FD_598
        public  _seg021_22FD_5C8
        public  _seg021_22FD_5E9

_seg021_22FD_590:
        db      0h,3h
        dw      seg dseg062_62a6
L0594:
        db      0h,0h
L0596:
        db      0h,0h

; seg021_22FD_598  (+598)
_seg021_22FD_598 proc near
        call    _seg021_22FD_46B
        push    es
        mov     ax,3509h
        int     21h
        mov     word ptr cs:L0596,es
        mov     word ptr cs:L0594,bx
        push    ds
        push    cs
        pop     ds
        mov     dx,62Eh
        mov     ax,2509h
        int     21h
        mov     ax,seg dseg062_62a6
        mov     ds,ax
        mov     ah,byte ptr ds:[1E8h]
        shl     ah,2
        call    _seg021_22FD_5E9
        pop     ds
        pop     es
        ret
_seg021_22FD_598 endp

; seg021_22FD_5C8  (+5C8)
_seg021_22FD_5C8 proc near
        push    ds
        push    es
        lds     dx,dword ptr cs:L0594
        mov     ax,2509h
        int     21h
        xor     ax,ax
        mov     es,ax
        mov     ah,byte ptr es:[417h]
        and     ah,70h
        shr     ah,4
        call    _seg021_22FD_5E9
        pop     es
        pop     ds
        ret
_seg021_22FD_5C8 endp

; seg021_22FD_5E9  (+5E9)
_seg021_22FD_5E9 proc near
        cli
        push    dx
        mov     dx,3
L05EE:
        in      al,64h
        test    al,2
        jne     L05EE
        mov     al,0EDh
        out     60h,al
L05F8:
        in      al,64h
        test    al,1
        je      L05F8
        in      al,60h
        cmp     al,0FAh
        je      short L0609
        dec     dx
        jne     L05EE
        je      short L062B
L0609:
        mov     dx,3
L060C:
        in      al,64h
        test    al,2
        jne     L060C
        mov     al,ah
        out     60h,al
L0616:
        in      al,64h
        test    al,1
        je      L0616
        in      al,60h
        cmp     al,0FAh
        je      short L0627
        dec     dx
        jne     L060C
        je      short L062B
L0627:
        clc
L0628:
        pop     dx
        sti
        ret
L062B:
        stc
        jmp     L0628
        sti
        push    ds
        push    ax
        push    di
        lds     di,dword ptr cs:_seg021_22FD_590
        in      al,60h
        mov     byte ptr [di],al
        inc     di
        cmp     di,340h
        jne     short L0645
        mov     di,300h
L0645:
        mov     word ptr cs:_seg021_22FD_590,di
        cmp     al,3Ah
        jne     short L065D
        xor     byte ptr ds:[1E8h],1
        mov     ah,byte ptr ds:[1E8h]
        shl     ah,2
        call    _seg021_22FD_5E9
L065D:
        in      al,61h
        jmp     short L0661
L0661:
        mov     ah,al
        or      al,80h
        out     61h,al
        jmp     short L0669
L0669:
        mov     al,ah
        out     61h,al
        mov     al,20h
        out     20h,al
        pop     di
        pop     ax
        pop     ds
        iret
_seg021_22FD_5E9 endp

SEG021_TEXT ends

        end
