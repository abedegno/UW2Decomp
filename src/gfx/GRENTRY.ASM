; /* target: seg003_0272_6E2 */
; /* opts: /ml */
;
; The graphics entry points _6E2 .. _C13 (DRAW_RELATED_7F9 among them) that seg021's cRender
; and friends call.
;
; SEG003 split into its original modules: this is module 3 of 14, +6E2..+D1E of the segment
; (file 3B32h), a module of the second library (UWLIB.LIB).
; Procs: seg003_0272_6E2, seg003_0272_6E4, seg003_0272_764, seg003_0272_788, seg003_0272_7A8,
; DRAW_RELATED_seg003_0272_7F9, seg003_0272_888, seg003_0272_9C9, seg003_0272_9FC,
; seg003_0272_A7F, seg003_0272_B9B, seg003_0272_C13.
; Evidence for the bounds: starts after one zero byte following a ret: TLINK's padding before
; a word-aligned module (TASM 2.0's `even` pads with 90h). It ends at +D1E: relocation entries
; 2442-2450 (+6E8..+CF3) and 2453 (+D21) are separated by the FD51 pair 2451-2452, so +D21 is
; another module, and +D1E is the only routine start between.
; Its relocations are entries 2442-2450 of UW2.EXE's relocation table.
; Its FD51 piece is 370D:0956..0B0A (relocation entries 2441), defined before the code because
; those entries come before the code's.
;
; Its FD51 piece holds relocation entry 2441, the far pointer at 956 to stdat; it runs to the
; next module's first pointer (B0A). Its bounds are not otherwise known.

        .186

        extrn   _stdat:far
        extrn   seg004_0849:far
        extrn   seg049_3EE2:far
        extrn   seg_370D:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

FD51    segment byte public 'FAR_DATA'
        dd      _stdat
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,40h,01h,00h,00h,0C9h,09h,00h,00h,00h,00h,00h,00h
        db      8 dup (0)
        db      0FCh,09h,00h,00h,00h,00h,00h,00h
FD51    ends

SEG003_TEXT segment word public 'CODE'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _cXfer:byte
        public  _DRAW_RELATED_seg003_0272_7F9
        public  _seg003_0272_6E2
        public  _seg003_0272_6E4
        public  _seg003_0272_6F8
        public  _seg003_0272_729
        public  _seg003_0272_764
        public  _seg003_0272_788
        public  _seg003_0272_7A8
        public  _seg003_0272_888
        public  _seg003_0272_9C9
        public  _seg003_0272_9FC
        public  _seg003_0272_A7F
        public  _seg003_0272_B9B
        public  _seg003_0272_C13

; seg003_0272_6E2  (+6E2)
_seg003_0272_6E2 proc near
        xor     ax,ax
_seg003_0272_6E2 endp

; seg003_0272_6E4  (+6E4)
_seg003_0272_6E4 proc far
        mov     ah,al
        push    es
        mov     cx,seg seg049_3EE2
        mov     es,cx
        mov     cx,34E9h
        xor     di,di
        add     di,2
        rep stosw
        pop     es
        retf
_seg003_0272_6F8:
        push    es
        push    ds
        mov     es,word ptr ss:[958h]
        mov     ds,word ptr ss:[55ECh]
L0704:
        lods    word ptr ss:[si]
        shl     ax,1
        jb      L0726
        mov     bx,ax
        mov     di,word ptr ss:[bx+95Ch]
        lods    word ptr ss:[si]
        add     di,ax
        mov     cx,ax
        lods    word ptr ss:[si]
        sub     ax,cx
        inc     ax
        mov     cx,ax
        lods    word ptr ss:[si]
        xchg    si,ax
        rep movsb
        xchg    si,ax
        jmp     L0704
L0726:
        pop     ds
        pop     es
        retn
_seg003_0272_729:
        push    es
        push    ds
        mov     es,word ptr ss:[958h]
        mov     ds,word ptr ss:[55ECh]
L0735:
        lods    word ptr ss:[si]
        shl     ax,1
        jb      L0761
        mov     bx,ax
        mov     di,word ptr ss:[bx+95Ch]
        lods    word ptr ss:[si]
        add     di,ax
        mov     cx,ax
        lods    word ptr ss:[si]
        sub     ax,cx
        inc     ax
        mov     cx,ax
        lods    word ptr ss:[si]
        push    si
        mov     si,ax
L0754:
        lodsb
        or      al,al
        je      L075B
        stosb
        dec     di
L075B:
        inc     di
        loop    L0754
        pop     si
        jmp     L0735
L0761:
        pop     ds
        pop     es
        retn
_seg003_0272_6E4 endp

; seg003_0272_764  (+764)
_seg003_0272_764 proc far
        push    ds
        push    es
        mov     bx,seg seg049_3EE2
        mov     ds,bx
        mov     bx,seg seg004_0849
        mov     es,bx
        xchg    ch,cl
        mov     si,cx
        mov     di,6AA6h
        xor     bx,bx
L0779:
        mov     bl,byte ptr [di]
        mov     bl,byte ptr es:[bx+si+6D3Eh]
        mov     byte ptr [di],bl
        dec     di
        jne     L0779
        pop     es
        pop     ds
        retf
_seg003_0272_764 endp

; seg003_0272_788  (+788)
_seg003_0272_788 proc far
        push    ds
        mov     bx,seg seg049_3EE2
        mov     ds,bx
L078E:
        mov     si,69D2h
        xor     bx,bx
L0793:
        mov     bl,byte ptr [si]
        mov     bl,byte ptr cs:_cXfer[bx+300h]
        mov     byte ptr [si],bl
        dec     si
        jne     L0793
        dec     cx
        jne     L078E
        pop     ds
        retf
L07A4   dw      0
L07A6   dw      0
_seg003_0272_788 endp

; seg003_0272_7A8  (+7A8)
_seg003_0272_7A8 proc far
        add     bx,2
        mov     di,95Ch
        mov     dx,cx
        xor     ax,ax
        add     ax,2
L07B5:
        stosw
        add     ax,bx
        loop    L07B5
        mov     ax,69D4h
        mov     cx,0C8h
        sub     cx,dx
        dec     dx
        mov     word ptr ds:[0AF0h],dx
        rep stosw
        cmp     bx,word ptr ds:[0AEEh]
        je      L07F8
        cmp     word ptr ds:[0AEEh],140h
        jge     L07E4
        mov     di,offset _seg003_0272_888
        mov     dx,word ptr ds:[0AEEh]
        add     di,dx
        mov     byte ptr cs:[di],0A4h   ; movsb again where the old ret was
L07E4:
        sub     bx,2
        cmp     bx,140h
        jge     L07F8
        mov     di,offset _seg003_0272_888
        mov     byte ptr cs:[bx+di],0C3h ; ret after the new width
        mov     word ptr ds:[0AEEh],bx
L07F8:
        retf
_seg003_0272_7A8 endp

; DRAW_RELATED_seg003_0272_7F9  (+7F9)
_DRAW_RELATED_seg003_0272_7F9 proc far
        push    bp
        push    es
        push    ds
        push    si
        push    di
        mov     ax,seg seg_370D
        mov     ds,ax
        mov     es,ax
        mov     bx,ss
        mov     word ptr cs:L07A4,bx
        mov     word ptr cs:L07A6,sp
        cli
        mov     ss,ax
        mov     sp,word ptr ds:[5588h]
        sti
        mov     ax,0A000h
        mov     es,ax
        mov     bp,word ptr ds:[0AF0h]
        mov     dx,3C5h
        mov     bx,word ptr ds:[94Eh]
        mov     ax,word ptr ds:[383Ah]
        add     bx,ax
        mov     ds,word ptr ds:[958h]
L0832:
        shl     bp,1
        mov     cx,word ptr [bp+95Ch]
        mov     al,8
        out     dx,al
        mov     di,bx
        mov     si,3
        add     si,cx
        call    _seg003_0272_888
        mov     al,2
        out     dx,al
        mov     di,bx
        mov     si,1
        add     si,cx
        call    _seg003_0272_888
        mov     al,4
        out     dx,al
        mov     di,bx
        mov     si,2
        add     si,cx
        call    _seg003_0272_888
        mov     al,1
        out     dx,al
        mov     di,bx
        mov     si,cx
        call    _seg003_0272_888
        add     bx,50h
        shr     bp,1
        dec     bp
        cmp     bp,word ptr ss:[3DFAh]
        jg      L0832
        cli
        mov     ss,word ptr cs:L07A4
        mov     sp,word ptr cs:L07A6
        sti
        pop     di
        pop     si
        pop     ds
        pop     es
        pop     bp
        retf
_DRAW_RELATED_seg003_0272_7F9 endp

; seg003_0272_888  (+888)
_seg003_0272_888 proc near
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        movsb
        add     si,3
        ret
_seg003_0272_888 endp

; seg003_0272_9C9  (+9C9)
_seg003_0272_9C9 proc near
        push    es
        mov     bx,word ptr ds:[3DF4h]
        mov     es,word ptr ds:[958h]
L09D2:
        lodsw
        shl     ax,1
        jb      L09F1
        mov     di,ax
        mov     di,word ptr [di+95Ch]
        lodsw
        mov     cx,ax
        add     di,ax
        lodsw
        sub     cx,ax
        neg     cx
        mov     al,byte ptr ds:[4111h]
        inc     cx
        js      L09F3
        rep stosb
        jmp     L09D2
L09F1:
        pop     es
        ret
L09F3:
        add     di,cx
        dec     cx
        neg     cx
        rep stosb
        jmp     L09D2
_seg003_0272_9C9 endp

; seg003_0272_9FC  (+9FC)
_seg003_0272_9FC proc near
        mov     dx,3C5h
        mov     bp,si
        mov     di,word ptr ds:[4116h]
        push    ds
        push    es
        mov     es,word ptr ds:[3DFCh]
        mov     ds,word ptr ds:[958h]
L0A0F:
        mov     bx,word ptr [bp]
        shl     bx,1
        jb      L0A7C
        mov     si,word ptr [bp+2]
        and     si,-4
        mov     cx,word ptr [bp+4]
        and     cx,-4
        add     cx,4
        add     bp,6
        sub     cx,si
        add     si,word ptr ss:[bx+95Ch]
        mov     bx,cx
        push    bp
        shr     cx,2
        mov     bp,di
        mov     al,1
        out     dx,al
L0A3A:
        movsb
        add     si,3
        loop    L0A3A
        sub     si,bx
        inc     si
        mov     cx,bx
        shr     cx,2
        mov     di,bp
        shl     al,1
        out     dx,al
L0A4D:
        movsb
        add     si,3
        loop    L0A4D
        sub     si,bx
        inc     si
        mov     cx,bx
        shr     cx,2
        mov     di,bp
        shl     al,1
        out     dx,al
L0A60:
        movsb
        add     si,3
        loop    L0A60
        sub     si,bx
        inc     si
        mov     cx,bx
        shr     cx,2
        mov     di,bp
        shl     al,1
        out     dx,al
L0A73:
        movsb
        add     si,3
        loop    L0A73
        pop     bp
        jmp     L0A0F
L0A7C:
        pop     es
        pop     ds
        ret
_seg003_0272_9FC endp

; seg003_0272_A7F  (+A7F)
_seg003_0272_A7F proc near
        push    ds
        push    es
        mov     es,word ptr ds:[3DFCh]
        mov     ds,word ptr ds:[958h]
        mov     bp,si
L0A8B:
        mov     bx,word ptr [bp]
        shl     bx,1
        jae     L0A95
        jmp     L0B8C
L0A95:
        mov     si,word ptr ss:[bx+95Ch]
        mov     bx,word ptr [bp+2]
        mov     cx,word ptr [bp+4]
        mov     di,word ptr [bp+6]
        add     si,bx
        add     bp,8
        push    bp
        sub     cx,bx
        inc     cx
        mov     bp,cx
        mov     dx,3C5h
        mov     al,byte ptr ss:[bx+3B64h]
        and     al,byte ptr ss:[bx+3CA8h]
        add     cx,3
        shr     cx,2
        out     dx,al
L0AC2:
        movsb
        add     si,3
        loop    L0AC2
        mov     cx,bp
        add     cx,3
        shr     cx,2
        sub     di,cx
        shl     cx,2
        sub     si,cx
        inc     si
        inc     bx
        mov     cx,bx
        and     cx,3
        jne     L0AE1
        inc     di
L0AE1:
        dec     bp
        jne     L0AE7
        jmp     L0B88
L0AE7:
        mov     cx,bp
        mov     al,byte ptr ss:[bx+3B64h]
        and     al,byte ptr ss:[bx+3CA8h]
        add     cx,3
        shr     cx,2
        out     dx,al
L0AFA:
        movsb
        add     si,3
        loop    L0AFA
        mov     cx,bp
        add     cx,3
        shr     cx,2
        sub     di,cx
        shl     cx,2
        sub     si,cx
        inc     si
        inc     bx
        mov     cx,bx
        and     cx,3
        jne     L0B19
        inc     di
L0B19:
        dec     bp
        je      L0B88
        mov     cx,bp
        mov     al,byte ptr ss:[bx+3B64h]
        and     al,byte ptr ss:[bx+3CA8h]
        add     cx,3
        shr     cx,2
        out     dx,al
L0B2F:
        movsb
        add     si,3
        loop    L0B2F
        mov     cx,bp
        add     cx,3
        shr     cx,2
        sub     di,cx
        shl     cx,2
        sub     si,cx
        inc     si
        inc     bx
        mov     cx,bx
        and     cx,3
        jne     L0B4E
        inc     di
L0B4E:
        dec     bp
        je      L0B88
        mov     cx,bp
        mov     al,byte ptr ss:[bx+3B64h]
        and     al,byte ptr ss:[bx+3CA8h]
        add     cx,3
        shr     cx,2
        out     dx,al
L0B64:
        movsb
        add     si,3
        loop    L0B64
        mov     cx,bp
        add     cx,3
        shr     cx,2
        sub     di,cx
        shl     cx,2
        sub     si,cx
        inc     si
        inc     bx
        mov     cx,bx
        and     cx,3
        jne     L0B83
        inc     di
L0B83:
        dec     bp
        je      L0B88
        mov     cx,bp
L0B88:
        pop     bp
        jmp     L0A8B
L0B8C:
        mov     dx,3CEh
        mov     ax,0FF08h
        out     dx,ax
        pop     es
        pop     ds
        ret
        add     sp,2
        jmp     L0B8C
_seg003_0272_A7F endp

; seg003_0272_B9B  (+B9B)
_seg003_0272_B9B proc near
        shl     ax,1
        jb      L0BBF
        mov     bx,ax
        mov     ax,word ptr [bx+21Eh]
        mov     word ptr ds:[4110h],ax
        mov     ax,word ptr [bx+434h]
        mov     word ptr ds:[4116h],ax
        db      8Bh,9Fh,8h,0h                   ; mov bx,word ptr [bx+8]: disp16, the table at seg_370D:0008 (TASM gives disp8 for a number, and a label there would need that segment)
        cmp     bx,14h
        jae     L0BBF
        mov     ax,word ptr [bx+0AF2h]
        mov     word ptr ds:[4112h],ax
L0BBF:
        ret
L0BC0:
        ret
L0BC1   dw      0
L0BC3   dw      offset L0BC7            ; span routines _seg003_0272_C13 jumps through
L0BC5   dw      offset L0C93
L0BC7:
        push    si
        push    es
        mov     ax,seg seg004_0849
        mov     es,ax
        mov     al,byte ptr es:[6D20h]
        mov     ah,byte ptr ds:[0B08h]
        mov     bx,ax
        mov     al,byte ptr es:[bx+6D3Eh]
        mov     byte ptr ds:[4111h],al
        or      byte ptr [si-3],80h
        sub     si,0Ah
        call    _seg003_0272_9C9
        pop     es
        pop     si
        and     byte ptr [si-3],7Fh
        jmp     short _seg003_0272_C13
        push    ds
        push    es
        mov     dh,byte ptr ss:[0B08h]
        mov     ds,word ptr ss:[958h]
        mov     ax,seg seg004_0849
        mov     es,ax
L0C03:
        mov     dl,byte ptr [di]
        mov     bx,dx
        mov     bh,byte ptr es:[bx+6D3Eh]
        mov     byte ptr [di],bh
        inc     di
        loop    L0C03
        pop     es
        pop     ds
_seg003_0272_B9B endp

; seg003_0272_C13  (+C13)
_seg003_0272_C13 proc near
        lodsw
        mov     word ptr cs:L0BC1,ax
        shl     ax,1
        jb      L0BC0
        mov     di,ax
        mov     di,word ptr ss:[di+95Ch]
        lodsw
        xchg    dx,ax
        lodsw
        xchg    cx,ax
        lodsw
        xchg    bx,ax
        lodsw
        mov     bp,cx
        sub     cx,dx
        jge     L0C35
        neg     cx
        xchg    bp,dx
        xchg    bx,ax
L0C35:
        mov     word ptr ss:[0B08h],bx
        mov     word ptr ss:[0B0Ah],dx
        add     di,dx
        inc     cx
        sub     ax,bx
        jne     L0C4B
        jmp     word ptr cs:L0BC3
L0C4B:
        or      ax,ax
        jns     L0C6A
        or      cx,cx
        jns     L0C59
        neg     ax
        neg     cx
        jmp     short L0C74
L0C59:
        neg     ax
L0C5B:
        cwd
        idiv    cx
        mov     bh,al
        sub     ax,ax
        div     cx
        mov     bl,ah
        neg     bx
        jmp     short L0C7F
L0C6A:
        test    cx,0FFFFh
        jns     L0C74
        neg     cx
        jmp     L0C5B
L0C74:
        cwd
        idiv    cx
        mov     bh,al
        sub     ax,ax
        div     cx
        mov     bl,ah
L0C7F:
        mov     bp,bx
        mov     bh,byte ptr ss:[0B08h]
        mov     bl,80h
        push    ds
        mov     ds,word ptr ss:[958h]
        jmp     word ptr cs:L0BC5
L0C93:
        push    si
        push    es
        mov     ax,cx
        mov     cx,bx
        mov     dx,bx
        shl     bp,1
        sub     dx,40h
        add     cx,40h
        shr     word ptr cs:L0BC1,1
        jae     L0CAC
        xchg    cx,dx
L0CAC:
        mov     bx,seg seg004_0849
        mov     es,bx
        mov     bl,byte ptr es:[6D20h]
        mov     si,6D3Eh
L0CB9:
        mov     bh,dh
        mov     bh,byte ptr es:[bx+si]
        mov     byte ptr [di],bh
        add     dx,bp
        inc     di
        dec     ax
        je      L0CD3
        mov     bh,ch
        mov     bh,byte ptr es:[bx+si]
        mov     byte ptr [di],bh
        add     cx,bp
        inc     di
        dec     ax
        jne     L0CB9
L0CD3:
        pop     es
        pop     si
        pop     ds
        jmp     _seg003_0272_C13
        push    si
        mov     ax,cx
        mov     cx,bx
        shl     bp,1
        sub     bx,40h
        add     cx,40h
        shr     word ptr cs:L0BC1,1
        jae     L0CEF
        xchg    cx,bx
L0CEF:
        mov     dx,bx
        push    es
        mov     bx,seg seg004_0849
        mov     es,bx
        mov     si,6D3Eh
L0CFA:
        mov     bl,byte ptr [di]
        mov     bh,dh
        mov     bh,byte ptr es:[bx+si]
        mov     byte ptr [di],bh
        add     dx,bp
        inc     di
        dec     ax
        je      L0D18
        mov     bl,byte ptr [di]
        mov     bh,ch
        mov     bh,byte ptr es:[bx+si]
        mov     byte ptr [di],bh
        add     cx,bp
        inc     di
        dec     ax
        jne     L0CFA
L0D18:
        pop     es
        pop     si
        pop     ds
        jmp     _seg003_0272_C13
_seg003_0272_C13 endp

SEG003_TEXT ends

        end
