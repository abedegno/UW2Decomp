; /* target: seg021_22FD_A30 */
; /* opts: /ml */
;
; Integer maths: square roots, sines and cosines, headings and arctangents from tables.
;
; SEG021 split into its original modules: this is module 14 of 17, +A30..+C00 of the segment
; (file 24730h), a module of the second library (UWLIB.LIB).
; Procs: SquareRoot_seg021_22FD_A30, sincos, seg021_22FD_A38, seg021_22FD_A69,
; SquareRoot_seg021_22FD_A78, seg021_22FD_B78, seg021_22FD_BA2, MaybeTangent_seg021_22FD_BD4.
; Evidence for the bounds: starts on a paragraph after 4 zero bytes following a retf: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG021_TEXT segment para public 'CODE'
        assume  cs:SEG021_TEXT, ds:DGROUP, ss:DGROUP
        public  _MaybeTangent_seg021_22FD_BD4
        public  _seg021_22FD_A38
        public  _seg021_22FD_A69
        public  _seg021_22FD_B78
        public  _seg021_22FD_BA2
        public  _sincos
        public  _SquareRoot_seg021_22FD_A30
        public  _SquareRoot_seg021_22FD_A78

; SquareRoot_seg021_22FD_A30  (+A30)
_SquareRoot_seg021_22FD_A30 proc far
        call    _SquareRoot_seg021_22FD_A78
        retf
_SquareRoot_seg021_22FD_A30 endp

; sincos  (+A34)
_sincos proc far
        call    _seg021_22FD_A38
        retf
_sincos endp

; HeadingLookupCalc_seg021_22FD_A38  (+A38)
_seg021_22FD_A38 proc near
        mov     cx,bx
        xor     ch,ch
        mov     bl,bh
        xor     bh,bh
        shl     bx,1
        mov     bp,word ptr [bx+5B0h]
        mov     ax,word ptr [bx+5B2h]
        sub     ax,bp
        imul    cx
        mov     al,ah
        mov     ah,dl
        add     ax,bp
        push    ax
        mov     bp,word ptr [bx+630h]
        mov     ax,word ptr [bx+632h]
        sub     ax,bp
        imul    cx
        mov     bl,ah
        mov     bh,dl
        add     bx,bp
        pop     ax
        ret
_seg021_22FD_A38 endp

; LookupHeadingTable_seg021_22FD_A69  (+A69)
_seg021_22FD_A69 proc near
        mov     bl,bh
        xor     bh,bh
        shl     bx,1
        mov     ax,word ptr [bx+5B0h]
        mov     bx,word ptr [bx+630h]
        ret
_seg021_22FD_A69 endp

; SquareRoot_seg021_22FD_A78  (+A78)
_SquareRoot_seg021_22FD_A78 proc near
        or      ch,ch
        je      short L0AB9
        mov     di,4000h
        cmp     cx,di
        jb      short L0A86
        mov     di,0FFFFh
L0A86:
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        ret
L0AB9:
        or      cl,cl
        je      short L0AF3
        mov     di,400h
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        ret
L0AF3:
        or      bh,bh
        je      short L0B2F
        mov     cx,40h
        mov     ax,bx
        xor     dx,dx
        div     cx
        add     cx,ax
        rcr     cx,1
        mov     ax,bx
        xor     dx,dx
        div     cx
        add     cx,ax
        rcr     cx,1
        mov     ax,bx
        xor     dx,dx
        div     cx
        add     cx,ax
        rcr     cx,1
        mov     ax,bx
        xor     dx,dx
        div     cx
        add     cx,ax
        rcr     cx,1
        mov     ax,bx
        xor     dx,dx
        div     cx
        add     cx,ax
        rcr     cx,1
        mov     di,cx
        ret
L0B2F:
        or      bl,bl
        je      short L0B61
        mov     cx,4
        mov     ax,bx
        div     cl
        add     cl,al
        rcr     cl,1
        mov     ax,bx
        div     cl
        add     cl,al
        rcr     cl,1
        mov     ax,bx
        div     cl
        add     cl,al
        rcr     cl,1
        mov     ax,bx
        div     cl
        add     cl,al
        rcr     cl,1
        mov     ax,bx
        div     cl
        add     cl,al
        rcr     cl,1
        mov     di,cx
        ret
L0B61:
        xor     di,di
        ret
        cwd
        xor     ax,dx
        sub     ax,dx
        mov     bl,ah
        xor     bh,bh
        shl     bx,1
        mov     cx,word ptr [bx+832h]
        xor     cx,dx
        sub     cx,dx
        ret
_SquareRoot_seg021_22FD_A78 endp

; seg021_22FD_B78  (+B78)
_seg021_22FD_B78 proc near
        cwd
        xor     ax,dx
        sub     ax,dx
        push    dx
        mov     bl,ah
        xor     bh,bh
        mov     cx,ax
        xor     ch,ch
        shl     bx,1
        mov     bp,word ptr [bx+832h]
        mov     ax,word ptr [bx+834h]
        sub     ax,bp
        imul    cx
        mov     al,ah
        mov     ah,dl
        add     ax,bp
        mov     cx,ax
        pop     dx
        xor     cx,dx
        sub     cx,dx
        ret
_seg021_22FD_B78 endp

; seg021_22FD_BA2  (+BA2)
_seg021_22FD_BA2 proc near
        mov     ax,bx
        cwd
        xor     ax,dx
        sub     ax,dx
        push    dx
        mov     cx,ax
        xor     ch,ch
        mov     bl,ah
        xor     bh,bh
        shl     bx,1
        mov     bp,word ptr [bx+934h]
        mov     ax,word ptr [bx+936h]
        sub     ax,bp
        imul    cx
        mov     al,ah
        mov     ah,dl
        add     ax,bp
        mov     cx,ax
        pop     dx
        xor     cx,dx
        sub     cx,dx
        and     dx,8000h
        add     cx,dx
        ret
_seg021_22FD_BA2 endp

; MaybeTangent_seg021_22FD_BD4  (+BD4)
_MaybeTangent_seg021_22FD_BD4 proc near
        cmp     ax,5A82h
        jg      short L0BEE
        cmp     ax,0A57Eh
        jl      short L0BEE
        push    bx
        call    _seg021_22FD_B78
        pop     ax
        or      ax,ax
        jns     short L0BED
        sub     cx,8000h
        neg     cx
L0BED:
        ret
L0BEE:
        cwd
        push    dx
        call    _seg021_22FD_BA2
        pop     dx
        xor     cx,dx
        sub     cx,dx
        ret
_MaybeTangent_seg021_22FD_BD4 endp

SEG021_TEXT ends

        end
