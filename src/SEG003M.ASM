; /* target: seg003_0272_5934 */
; /* opts: /ml */
;
; Code from 5934 (no routine start in the target table).
;
; SEG003 split into its original modules: this is module 13 of 14, +5934..+5B50 of the segment
; (file 8D84h), a module of the second library (UWLIB.LIB).
; Procs: none (data).
; Evidence for the bounds: starts after one zero byte following a ret: TLINK's padding before
; a word-aligned module (TASM 2.0's `even` pads with 90h).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'CODE'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg003_0272_32B4:near
        extrn   _seg003_0272_3686:near
        extrn   _seg003_0272_5B50:near
        extrn   _seg003_0272_5D47:near
        extrn   _seg003_0272_C13:near
        public  _seg003_0272_5946
        public  _seg003_0272_5956
        public  _seg003_0272_5959

L5934:
        jne     L593C
        mov     si,415Eh
        jmp     _seg003_0272_3686
L593C:
        mov     ax,word ptr ds:[415Eh]
        mov     bx,word ptr ds:[4160h]
        jmp     _seg003_0272_32B4
_seg003_0272_5946:
        cmp     cx,63h
        ja      L5955
        cmp     cx,2
        jbe     L5934
        call    _seg003_0272_5B50
        jmp     short _seg003_0272_5959
L5955:
        ret
_seg003_0272_5956:
        call    _seg003_0272_5D47
_seg003_0272_5959:
        db      0E9h                    ; jmp to the next instruction in the near form, which TASM never makes
        dw      L595C-$-2
L595C:
        mov     di,cx
        shl     di,1
        add     di,cx
        shl     di,1
        mov     si,415Eh
        add     di,si
        movsw
        movsw
        movsw
        mov     word ptr ds:[5680h],cx
        mov     bx,0FC18h
        mov     dx,3E8h
        mov     si,4160h
L5979:
        test    word ptr [si],0FFFFh
        jns     L5983
        mov     word ptr [si],0
L5983:
        lodsw
        add     si,4
        cmp     ax,bx
        jl      L5993
        mov     bx,ax
        mov     bp,word ptr [si-8]
        mov     di,word ptr [si-4]
L5993:
        cmp     ax,dx
        jg      L5999
        mov     dx,ax
L5999:
        loop    L5979
        mov     word ptr ds:[5670h],bx
        mov     word ptr ds:[566Eh],dx
        mov     ax,bx
        mov     si,bx
        sub     si,word ptr ds:[566Eh]
        shl     si,1
        shl     si,1
        add     si,bx
        sub     si,word ptr ds:[566Eh]
        shl     si,1
        add     si,5692h
        mov     word ptr [si],ax
        mov     word ptr [si+2],bp
        mov     word ptr [si+4],bp
        mov     word ptr [si+6],di
        mov     word ptr [si+8],di
        lea     di,[si-8]
        mov     cx,bx
        sub     cx,dx
        inc     cx
        mov     ax,3E8h
        mov     bx,0FC18h
L59D7:
        mov     word ptr [di],ax
        mov     word ptr [di+2],bx
        sub     di,0Ah
        loop    L59D7
        mov     si,415Eh
L59E4:
        lodsw
        xchg    di,ax
        lodsw
        add     si,2
        push    si
        mov     bp,word ptr [si]
        mov     word ptr ds:[5672h],bp
        mov     dx,word ptr [si+2]
        sub     di,bp
        mov     bp,word ptr [si-2]
        mov     bx,word ptr [si+4]
        mov     word ptr ds:[5686h],bx
        sub     bp,bx
        sub     ax,dx
        jne     L5A09
        jmp     L5B0A
L5A09:
        js      L5A22
        neg     bp
        neg     ax
        neg     di
        mov     cx,word ptr [si-6]
        mov     word ptr ds:[5672h],cx
        mov     cx,word ptr [si-2]
        mov     word ptr ds:[5686h],cx
        mov     dx,word ptr [si-4]
L5A22:
        mov     word ptr ds:[5676h],dx
        neg     ax
        xchg    di,ax
        cwd
        idiv    di
        mov     word ptr ds:[567Ah],ax
        sub     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[5678h],ax
        add     word ptr ds:[567Ah],dx
        mov     ax,bp
        cwd
        idiv    di
        mov     word ptr ds:[567Eh],ax
        sub     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[567Ch],ax
        add     word ptr ds:[567Eh],dx
        mov     word ptr ds:[5674h],8000h
        mov     word ptr ds:[5684h],8000h
        mov     word ptr ds:[5682h],di
        mov     bx,word ptr ds:[5676h]
        mov     di,bx
        sub     di,word ptr ds:[566Eh]
        shl     di,1
        shl     di,1
        add     di,bx
        sub     di,word ptr ds:[566Eh]
        shl     di,1
        add     di,5692h
        dec     bx
        sub     di,0Ah
        push    cx
        mov     dx,word ptr ds:[567Ah]
        mov     bp,word ptr ds:[5674h]
        mov     ax,word ptr ds:[5672h]
        mov     si,word ptr ds:[5684h]
        mov     cx,word ptr ds:[5686h]
L5AA1:
        mov     word ptr [di],bx
        dec     bx
        add     bp,word ptr ds:[5678h]
        adc     ax,dx
        add     si,word ptr ds:[567Ch]
        adc     cx,word ptr ds:[567Eh]
        cmp     ax,word ptr [di+2]
        jge     L5ABD
        mov     word ptr [di+2],ax
        mov     word ptr [di+6],cx
L5ABD:
        cmp     ax,word ptr [di+4]
        jle     L5AC8
        mov     word ptr [di+4],ax
        mov     word ptr [di+8],cx
L5AC8:
        sub     di,0Ah
        dec     word ptr ds:[5682h]
        jne     L5AA1
        pop     cx
L5AD2:
        pop     si
        dec     word ptr ds:[5680h]
        je      L5ADC
        jmp     L59E4
L5ADC:
        mov     di,word ptr ds:[5670h]
        sub     di,word ptr ds:[566Eh]
        shl     di,1
        shl     di,1
        add     di,word ptr ds:[5670h]
        sub     di,word ptr ds:[566Eh]
        shl     di,1
        add     di,5692h
        add     di,0Ah
        or      byte ptr [di+1],80h
        mov     si,5692h
        push    di
        call    _seg003_0272_C13
        pop     di
        and     byte ptr [di+1],7Fh
        ret
L5B0A:
        mov     di,dx
        sub     di,word ptr ds:[566Eh]
        shl     di,1
        shl     di,1
        add     di,dx
        sub     di,word ptr ds:[566Eh]
        shl     di,1
        add     di,5692h
        mov     word ptr [di],dx
        mov     ax,word ptr [si-6]
        mov     cx,word ptr [si]
        mov     bx,word ptr [si-2]
        mov     dx,word ptr [si+4]
        cmp     ax,cx
        jle     L5B34
        xchg    cx,ax
        xchg    bx,dx
L5B34:
        cmp     ax,word ptr [di+2]
        jg      L5B3F
        mov     word ptr [di+2],ax
        mov     word ptr [di+6],bx
L5B3F:
        xchg    cx,ax
        xchg    bx,dx
        cmp     ax,word ptr [di+4]
        jl      L5B4D
        mov     word ptr [di+4],ax
        mov     word ptr [di+8],bx
L5B4D:
        jmp     L5AD2

SEG003_TEXT ends

        end
