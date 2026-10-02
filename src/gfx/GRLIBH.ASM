; /* target: seg003_0272_375C */
; /* opts: /ml */
;
; _375C, _37BE and _381C.
;
; SEG003 split into its original modules: this is module 8 of 14, +375C..+38B4 of the segment
; (file 6BACh), a module of the second library (UWLIB.LIB).
; Procs: seg003_0272_375C, seg003_0272_37BE, seg003_0272_381C.
; Evidence for the bounds: starts after one zero byte following a jmp: TLINK's padding before
; a word-aligned module (TASM 2.0's `even` pads with 90h).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'CODE'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        public  _seg003_0272_375C
        public  _seg003_0272_37BE
        public  _seg003_0272_381C

; seg003_0272_375C  (+375C)
_seg003_0272_375C proc near
        mov     si,word ptr ds:[49ACh]
        and     word ptr [si],7FFFh
        cmp     bx,dx
        jge     L376B
        xchg    cx,ax
        xchg    bx,dx
L376B:
        dec     dx
        cmp     ax,cx
        jle     L3784
        mov     word ptr ds:[49B0h],2
        mov     di,44D2h
        mov     bp,0FFF6h
        call    _seg003_0272_37BE
        add     di,-8
        jmp     short L3796
L3784:
        mov     word ptr ds:[49B0h],0
        mov     di,44D0h
        mov     bp,0FFFAh
        call    _seg003_0272_37BE
        add     di,-4
L3796:
        mov     ax,word ptr ds:[49B4h]
        stosw
        add     di,word ptr ds:[49B0h]
        mov     word ptr ds:[49ACh],di
        or      word ptr [di],8000h
        mov     si,word ptr ds:[49B6h]
        sub     si,2
        sub     si,word ptr ds:[49B0h]
        jmp     word ptr ds:[4112h]
L37B5:
        sub     bx,bx
        sub     cx,cx
        sub     dx,dx
        jmp     short L3800
L37BD:
        retn
_seg003_0272_375C endp

; seg003_0272_37BE  (+37BE)
_seg003_0272_37BE proc near
        mov     word ptr ds:[49B4h],cx
        mov     si,bx
        neg     si
        add     si,0C7h
        shl     si,1
        add     di,si
        shl     si,1
        add     di,si
        mov     word ptr ds:[49B6h],di
        sub     bx,dx
        mov     word ptr ds:[49B2h],bx
        mov     si,bx
        jle     L37BD
        stosw
        add     di,4
        sub     cx,ax
        je      L37B5
        xchg    cx,ax
        cwd
        idiv    si
        xchg    bx,ax
        sub     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    si
        cwd
        shl     ax,1
        rcl     dx,1
        add     bx,dx
        xchg    cx,ax
        mov     dx,7FFFh
L3800:
        cmp     si,10h
        ja      L380B
        shl     si,1
        jmp     word ptr [si+498Ah]
L380B:
        call    _seg003_0272_381C
        sub     si,0Fh
        cmp     si,10h
        ja      L380B
        shl     si,1
        jmp     word ptr [si+498Ah]
_seg003_0272_37BE endp

; seg003_0272_381C  (+381C)
_seg003_0272_381C proc near
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        ret
_seg003_0272_381C endp

SEG003_TEXT ends

        end
