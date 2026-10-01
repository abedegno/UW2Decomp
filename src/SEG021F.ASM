; /* target: seg021_22FD_2E0 */
; /* opts: /ml */
;
; The joystick (port 201h): reading and calibrating the axes and buttons, with far thunks for
; C.
;
; SEG021 split into its original modules: this is module 6 of 17, +2E0..+450 of the segment
; (file 23FE0h), a module of the second library (UWLIB.LIB).
; Procs: seg021_22FD_2E0, seg021_22FD_2E4, seg021_22FD_2E8, seg021_22FD_2EC, seg021_22FD_30A,
; seg021_22FD_369, seg021_22FD_36D, seg021_22FD_371, seg021_22FD_375, seg021_22FD_380,
; seg021_22FD_38F, seg021_22FD_3A3, seg021_22FD_3FD.
; Evidence for the bounds: starts on a paragraph after 8 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG021_TEXT segment para public 'CODE'
        assume  cs:SEG021_TEXT, ds:DGROUP, ss:DGROUP
        public  _seg021_22FD_2E0
        public  _seg021_22FD_2E4
        public  _seg021_22FD_2E8
        public  _seg021_22FD_2EC
        public  _seg021_22FD_30A
        public  _seg021_22FD_369
        public  _seg021_22FD_36D
        public  _seg021_22FD_371
        public  _seg021_22FD_375
        public  _seg021_22FD_380
        public  _seg021_22FD_38F
        public  _seg021_22FD_3A3
        public  _seg021_22FD_3FD

; seg021_22FD_2E0  (+2E0)
_seg021_22FD_2E0 proc far
        call    _seg021_22FD_30A
        retf
_seg021_22FD_2E0 endp

; seg021_22FD_2E4  (+2E4)
_seg021_22FD_2E4 proc far
        call    _seg021_22FD_3A3
        retf
_seg021_22FD_2E4 endp

; seg021_22FD_2E8  (+2E8)
_seg021_22FD_2E8 proc far
        call    _seg021_22FD_2EC
        retf
_seg021_22FD_2E8 endp

; seg021_22FD_2EC  (+2EC)
_seg021_22FD_2EC proc near
        mov     dx,201h
        in      al,dx
        not     al
        mov     ah,al
        mov     di,1B6h
        mov     cx,4
L02FA:
        cmp     word ptr [di-8],0
        je      short L0303
        cwd
        mov     word ptr [di],dx
L0303:
        shl     ax,1
        dec     di
        dec     di
        loop    L02FA
        ret
_seg021_22FD_2EC endp

; seg021_22FD_30A  (+30A)
_seg021_22FD_30A proc near
        sub     ax,ax
        mov     di,1A0h
        mov     cx,10h
        rep stosw
        mov     byte ptr ds:[1D2h],0Fh
        mov     word ptr ds:[1D0h],4
        call    _seg021_22FD_3FD
        mov     si,1B8h
        mov     di,1A8h
        mov     cx,4
L032B:
        lodsw
        cmp     ax,10h
        jb      short L0336
        cmp     ax,3E8h
        jbe     short L034B
L0336:
        sub     ax,ax
        cmp     word ptr ds:[120h],0
        je      short L0347
        sub     word ptr ds:[1D0h],cx
        rep stosw
        jmp     short L034E
L0347:
        dec     word ptr ds:[1D0h]
L034B:
        stosw
        loop    L032B
L034E:
        mov     cx,4
        mov     bx,1
        sub     ax,ax
        mov     dx,ax
        mov     di,1A8h
L035B:
        scasw
        je      short L0360
        or      dx,bx
L0360:
        shl     bx,1
        loop    L035B
        mov     byte ptr ds:[1D2h],dl
        ret
_seg021_22FD_30A endp

; seg021_22FD_369  (+369)
_seg021_22FD_369 proc far
        call    _seg021_22FD_375
        retf
_seg021_22FD_369 endp

; seg021_22FD_36D  (+36D)
_seg021_22FD_36D proc far
        call    _seg021_22FD_380
        retf
_seg021_22FD_36D endp

; seg021_22FD_371  (+371)
_seg021_22FD_371 proc far
        call    _seg021_22FD_38F
        retf
_seg021_22FD_371 endp

; seg021_22FD_375  (+375)
_seg021_22FD_375 proc near
        mov     di,1C0h
        sub     ax,ax
        mov     cx,4
        rep stosw
        ret
_seg021_22FD_375 endp

; seg021_22FD_380  (+380)
_seg021_22FD_380 proc near
        call    _seg021_22FD_3A3
        mov     si,1A0h
        mov     di,1C8h
        mov     cx,4
        rep movsw
        ret
_seg021_22FD_380 endp

; seg021_22FD_38F  (+38F)
_seg021_22FD_38F proc near
        call    _seg021_22FD_3A3
        mov     si,1A0h
        mov     di,1C0h
        mov     cx,4
L039B:
        lodsw
        sub     ax,word ptr [di+8]
        stosw
        loop    L039B
        ret
_seg021_22FD_38F endp

; seg021_22FD_3A3  (+3A3)
_seg021_22FD_3A3 proc near
        cmp     word ptr ds:[1D0h],0
        je      short L03EB
        call    _seg021_22FD_3FD
        mov     cx,4
        mov     si,1B8h
        mov     di,1A0h
        mov     bx,80h
L03B9:
        cmp     word ptr [si-10h],0
        je      short L03EC
        lodsw
        sub     ax,word ptr [di+8]
        imul    bx
        idiv    word ptr [di+8]
        sub     ax,word ptr [di+20h]
        cmp     ax,0
        jge     short L03D7
        add     ax,4
        jge     short L03DC
        jmp     short L03DE
L03D7:
        sub     ax,4
        jge     short L03DE
L03DC:
        sub     ax,ax
L03DE:
        cmp     ax,7Fh
        jg      short L03F8
        cmp     ax,0FF81h
        jl      short L03F3
L03E8:
        stosw
        loop    L03B9
L03EB:
        ret
L03EC:
        inc     si
        inc     si
        inc     di
        inc     di
        loop    L03B9
        ret
L03F3:
        mov     ax,0FF81h
        jmp     L03E8
L03F8:
        mov     ax,7Fh
        jmp     L03E8
_seg021_22FD_3A3 endp

; seg021_22FD_3FD  (+3FD)
_seg021_22FD_3FD proc near
        mov     ah,byte ptr ds:[1D2h]
        sub     cx,cx
        mov     si,cx
        mov     dx,201h
        in      al,dx
        or      al,ah
        mov     bl,al
        cli
        out     dx,al
        jmp     short L0411
L0411:
        in      al,dx
        cmp     al,bl
        loope   L0411
        jcxz    L0429
        mov     bl,al
        not     al
        and     al,ah
        loope   L0411
        xor     ah,al
        inc     si
        push    cx
        push    ax
        test    ah,ah
        jne     L0411
L0429:
        sti
        test    si,si
        je      short L0445
L042E:
        pop     dx
        pop     ax
        neg     ax
        mov     cx,4
        mov     di,1B8h
L0438:
        sar     dx,1
        jae     short L043E
        mov     word ptr [di],ax
L043E:
        inc     di
        inc     di
        loop    L0438
        dec     si
        jne     L042E
L0445:
        ret
_seg021_22FD_3FD endp

SEG021_TEXT ends

        end
