; /* target: seg003_0272_EC */
; /* opts: /ml */
;
; Polygon span filling (_EC, _183) with its edge tables.
;
; SEG003 split into its original modules: this is module 2 of 14, +EC..+6E2 of the segment
; (file 353Ch), a module of the second library (UWLIB.LIB).
; Procs: seg003_0272_EC, seg003_0272_183.
; Evidence for the bounds: the boundary at +EC (see GRMISC.ASM); it ends at the zero byte at
; 6E1.
; Its relocations are entries 2440 of UW2.EXE's relocation table.
; Its FD51 piece is 370D:07D0..0956 (relocation entries 2439), defined before the code because
; those entries come before the code's.
;
; Its FD51 piece (segment table entry 51, the graphics data segment seg_370D) holds the
; relocation entry 2439, which comes right before this module's code entry (2440). Where the
; piece starts and ends is not known: it is taken from that far pointer (7D0) to the next
; module's; the bytes before 7D0 come from the EXE (tools/extract.py). The code refers to
; 7A8..958 of the segment.

        .186

        extrn   seg_370D:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

FD51    segment byte public 'FAR_DATA'
_seg_370D_7D0 label word
        dd      seg_370D-8h
        db      1Ch dup (0)
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h,55h
        db      1Eh dup (0)
        db      60h,0A8h,00h,00h,00h,00h,00h,00h
FD51    ends

SEG003_TEXT segment word public 'CODE'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg003_0272_7A:word
        extrn   _seg003_0272_7C:word
        extrn   _seg003_0272_7E:word
        extrn   _seg003_0272_80:word
        extrn   _seg003_0272_82:word
        extrn   _seg003_0272_84:word
        extrn   _seg003_0272_86:word
        extrn   _seg003_0272_88:word
        extrn   _seg003_0272_8A:word
        extrn   _seg003_0272_8C:word
        extrn   _seg003_0272_8E:word
        extrn   _seg003_0272_90:word
        extrn   _seg003_0272_92:word
        extrn   _seg003_0272_94:word
        extrn   _seg003_0272_96:near
        extrn   _seg003_0272_98:near
        public  _seg_370D_7D0
        public  _seg003_0272_183
        public  _seg003_0272_EC

; seg003_0272_EC  (+EC)
_seg003_0272_EC proc near
        mov     bx,word ptr ds:[7B4h]
        mov     ax,word ptr [bx+6]
        mov     word ptr ds:[7C8h],ax
        mov     ax,word ptr [bx]
        mov     word ptr ds:[7C2h],ax
        mov     ax,8000h
        mov     word ptr ds:[7C4h],ax
        mov     word ptr ds:[7CCh],ax
L0104:
        add     bx,0Eh
        cmp     bx,word ptr ds:[7AAh]
        jne     L0110
        mov     bx,658h
L0110:
        mov     word ptr ds:[7B4h],bx
        mov     word ptr cs:_seg003_0272_86,bx
        dec     word ptr ds:[7D0h]
        je      L0182
        mov     ax,word ptr ds:[7D0h]
        mov     word ptr cs:_seg003_0272_7E,ax
        mov     ax,word ptr [bx+2]
        mov     word ptr cs:L054D,ax
        mov     cx,word ptr ds:[7CEh]
        sub     cx,ax
        je      _seg003_0272_EC
        mov     ax,word ptr [bx]
        sub     ax,word ptr ds:[7C2h]
        cwd
        idiv    cx
        mov     word ptr cs:L053F,ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr cs:L0539,ax
        add     word ptr cs:L053F,dx
        mov     ax,word ptr [bx+6]
        sub     ax,word ptr ds:[7C8h]
        cwd
        idiv    cx
        mov     word ptr cs:L0533,ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr cs:L052D,ax
        add     word ptr cs:L0533,dx
        test    word ptr ds:[7D0h],0FFFFh
L0182:
        ret
_seg003_0272_EC endp

; seg003_0272_183  (+183)
_seg003_0272_183 proc near
        mov     bx,word ptr ds:[7B2h]
        mov     ax,word ptr [bx+6]
        mov     word ptr ds:[7BCh],ax
        mov     ax,word ptr [bx]
        mov     word ptr ds:[7B6h],ax
        mov     ax,8000h
        mov     word ptr ds:[7B8h],ax
        mov     word ptr ds:[7C0h],ax
L019B:
        sub     bx,0Eh
        cmp     bx,64Ah
        jne     L01A8
        mov     bx,word ptr ds:[7A8h]
L01A8:
        mov     word ptr ds:[7B2h],bx
        dec     word ptr ds:[7D0h]
        je      L0182
        mov     ax,word ptr [bx+2]
        mov     word ptr cs:L0547,ax
        mov     cx,word ptr ds:[7CEh]
        sub     cx,ax
        je      _seg003_0272_183
        mov     ax,word ptr [bx]
        sub     ax,word ptr ds:[7B6h]
        cwd
        idiv    cx
        mov     word ptr cs:L0527,ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr cs:L0521,ax
        add     word ptr cs:L0527,dx
        mov     ax,word ptr [bx+6]
        sub     ax,word ptr ds:[7BCh]
        cwd
        idiv    cx
        mov     word ptr cs:L051B,ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr cs:L0515,ax
        add     word ptr cs:L051B,dx
        test    word ptr ds:[7D0h],0FFFFh
        ret
        mov     word ptr cs:_seg003_0272_94,cx
        mov     ax,es
        mov     word ptr cs:_seg003_0272_96,ax
        push    es
        push    ds
        mov     ax,seg seg_370D
        mov     ds,ax
        mov     bx,ss
        mov     word ptr cs:L0580,bx
        mov     word ptr cs:L0582,sp
        cli
        mov     ss,ax
        mov     sp,word ptr ds:[5588h]
        sti
        mov     ax,cx
        shl     ax,1
        shl     ax,1
        shl     ax,1
        sub     ax,cx
        shl     ax,1
        push    ax
        push    cx
        mov     bx,es
        mov     cx,54h
        mov     ax,cs
        mov     es,ax
        mov     di,offset _seg003_0272_98
        mov     si,658h
        rep movsb
        mov     es,bx
        pop     cx
        pop     ax
        add     ax,658h
        mov     word ptr ds:[7AAh],ax
        sub     ax,0Eh
        mov     word ptr ds:[7A8h],ax
        inc     cx
        mov     word ptr ds:[7D0h],cx
        mov     word ptr cs:_seg003_0272_7E,cx
        mov     si,word ptr es:[0C7FEh]
        lods    word ptr es:[si]
        dec     ax
        mov     word ptr ds:[7ACh],ax
        add     si,2
        lods    word ptr es:[si]
        mov     word ptr ds:[7AEh],ax
        lods    word ptr es:[si]
        mov     word ptr cs:L04F8,ax
        mov     dx,8001h
        mov     bp,7FFFh
        mov     si,658h
        mov     di,si
        dec     cx
L0293:
        lodsw
        cmp     dx,ax
        jge     L029C
        mov     bx,si
        mov     dx,ax
L029C:
        cmp     bp,ax
        jle     L02A6
        mov     bp,ax
        mov     word ptr ds:[7D6h],si
L02A6:
        lodsw
        cmp     ax,word ptr [di+2]
        jle     L02AF
        lea     di,[si-4]
L02AF:
        add     si,0Ah
        loop    L0293
        mov     word ptr ds:[930h],dx
        mov     word ptr cs:_seg003_0272_7A,dx
        mov     word ptr cs:_seg003_0272_7C,bp
        mov     word ptr cs:_seg003_0272_80,di
        mov     word ptr ds:[7B0h],di
        mov     si,di
        mov     word ptr ds:[7B2h],si
        mov     word ptr ds:[7B4h],si
        lodsw
        mov     word ptr cs:_seg003_0272_82,ax
        lodsw
        mov     word ptr ds:[7CEh],ax
        mov     word ptr cs:_seg003_0272_84,ax
        mov     word ptr cs:_seg003_0272_88,1234h
        mov     word ptr cs:_seg003_0272_8A,1234h
        mov     word ptr cs:_seg003_0272_8E,1234h
        mov     word ptr cs:_seg003_0272_8C,1234h
        mov     bp,word ptr ds:[7D6h]
        mov     ax,word ptr [bx+0Ah]
        mov     cx,word ptr [bp+0Ah]
        sub     ax,cx
        cwd
        xor     ax,dx
        sub     ax,dx
        cwd
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        rcl     dx,1
        cmp     cx,word ptr [bx+0Ah]
        jle     L0320
        mov     cx,word ptr [bx+0Ah]
L0320:
        idiv    cx
        cmp     ax,2
        mov     word ptr cs:_seg003_0272_88,ax
        jg      L035B
L032B:
        mov     cx,word ptr [bx-2]
        mov     di,word ptr [bp-2]
        sub     cx,di
        add     di,7F0h
        mov     ax,word ptr [bx+2]
        mov     bx,word ptr [bp+2]
        sub     ax,bx
        cwd
        or      cx,cx
        jg      L034C
        mov     byte ptr [di],bh
        mov     byte ptr [di+2],bh
        jmp     L048E
L034C:
        idiv    cx
        xchg    bx,ax
L034F:
        mov     byte ptr [di],ah
        inc     di
        add     ax,bx
        loop    L034F
        mov     byte ptr [di],ah
        jmp     L048E
L035B:
        mov     cx,word ptr [bx+2]
        sub     cx,word ptr [bp+2]
        je      L032B
        mov     cl,ch
        xor     ch,ch
        mov     ax,word ptr es:[2474h]
        mov     word ptr ds:[7ECh],ax
        mov     ax,word ptr es:[2472h]
        mov     word ptr ds:[7EEh],ax
        mov     ax,word ptr [bp-2]
        mov     word ptr ds:[7D8h],ax
        mov     ax,word ptr [bp+0Ah]
        mov     word ptr ds:[7DCh],ax
        mov     word ptr ds:[7DAh],0
        mov     ax,word ptr [bp+6]
        mov     word ptr ds:[7E0h],ax
        mov     word ptr ds:[7DEh],0
        mov     ax,word ptr [bx+6]
        sub     ax,word ptr [bp+6]
        cwd
        idiv    cx
        mov     word ptr ds:[7E4h],ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[7E2h],ax
        add     word ptr ds:[7E4h],dx
        mov     ax,word ptr [bx+0Ah]
        sub     ax,word ptr [bp+0Ah]
        cwd
        idiv    cx
        mov     word ptr ds:[7E8h],ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[7E6h],ax
        add     word ptr ds:[7E8h],dx
        mov     ax,word ptr [bp+2]
        mov     word ptr ds:[7EAh],ax
        mov     bp,word ptr ds:[7DCh]
        mov     si,word ptr ds:[7DAh]
        mov     bx,word ptr [bx+2]
        mov     bl,byte ptr ds:[7EBh]
        mov     es,word ptr ds:[7D2h]
        or      cx,cx
        mov     word ptr cs:_seg003_0272_8A,cx
        mov     word ptr cs:_seg003_0272_8C,bx
        jle     L0470
L03FA:
        add     si,word ptr ds:[7E6h]
        adc     bp,word ptr ds:[7E8h]
        mov     ax,word ptr ds:[7E0h]
        mov     dx,word ptr ds:[7DEh]
        mov     word ptr cs:_seg003_0272_90,ax
        mov     word ptr cs:_seg003_0272_92,dx
        add     dx,word ptr ds:[7E2h]
        adc     ax,word ptr ds:[7E4h]
        mov     word ptr ds:[7E0h],ax
        mov     word ptr ds:[7DEh],dx
        imul    word ptr ds:[7EEh]
        idiv    bp
        add     ax,word ptr ds:[7ECh]
        js      L046C
        mov     di,word ptr ds:[7D8h]
        cmp     ax,word ptr ds:[930h]
        jle     L044B
        mov     ax,word ptr ds:[930h]
        sub     ax,di
        jl      L0449
        add     di,7F0h
        inc     ax
        mov     cx,ax
        mov     al,bl
        rep stosb
L0449:
        jmp     short L048E
L044B:
        mov     word ptr ds:[7D8h],ax
        sub     ax,di
        jl      L046C
        inc     ax
        mov     word ptr cs:_seg003_0272_8E,ax
        add     di,7F0h
        mov     cx,ax
        mov     al,bl
        rep stosb
        inc     bl
        mov     byte ptr [di-1],bl
L0466:
        cmp     bl,bh
        jl      L03FA
        jmp     short L048E
L046C:
        inc     bl
        jne     L0466
L0470:
        mov     ax,word ptr ds:[930h]
        mov     di,word ptr ds:[7D8h]
        sub     ax,di
        jge     L0484
        mov     di,word ptr ds:[930h]
        mov     ax,word ptr ds:[7D8h]
        sub     ax,di
L0484:
        add     di,7F0h
        mov     cx,ax
        mov     al,bl
        rep stosb
L048E:
        call    _seg003_0272_183
        jne     L0496
        jmp     L0560
L0496:
        call    _seg003_0272_EC
        jne     L049E
        jmp     L0560
L049E:
        mov     es,word ptr ds:[958h]
L04A2:
        mov     di,word ptr ds:[7CEh]
        shl     di,1
        mov     di,word ptr [di+95Ch]
        mov     si,word ptr ds:[7B6h]
        add     di,si
        mov     cx,word ptr ds:[7C2h]
        sub     cx,si
        je      L04E0
        mov     bp,word ptr ds:[7BCh]
        mov     ax,word ptr ds:[7C8h]
        sub     ax,bp
        cwd
        idiv    cx
        mov     word ptr cs:L0504,ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr cs:L0500,ax
        add     word ptr cs:L0504,dx
L04E0:
        inc     cx
        jg      L04E6
        jmp     L057E
L04E6:
        mov     dx,bp
        add     si,7F0h
        mov     bp,word ptr ds:[7C0h]
        mov     ds,word ptr ds:[7AEh]
L04F4:
        mov     bx,dx
L04F8 equ $+2
        and     bx,1234h
        lods    byte ptr ss:[si]
        xlatb
        stosb
L0500 equ $+2
        add     bp,1234h
L0504 equ $+2
        adc     dx,1234h
        loop    L04F4
        mov     ds,word ptr ss:[7D2h]
L050D:
        dec     word ptr ds:[7CEh]
L0515 equ $+4
        add     word ptr ds:[7C0h],1234h
L051B equ $+4
        adc     word ptr ds:[7BCh],1234
L0521 equ $+4
        add     word ptr ds:[7B8h],1234h
L0527 equ $+4
        adc     word ptr ds:[7B6h],1234h
L052D equ $+4
        add     word ptr ds:[7CCh],1234h
L0533 equ $+4
        adc     word ptr ds:[7C8h],1234
L0539 equ $+4
        add     word ptr ds:[7C4h],1234h
L053F equ $+4
        adc     word ptr ds:[7C2h],1234h
        mov     cx,word ptr ds:[7CEh]
L0547 equ $+2
        cmp     cx,1234h
        jle     L056F
L054B:
L054D equ $+2
        cmp     cx,1234h
        jle     L0554
        jmp     L04A2
L0554:
        mov     bx,word ptr ds:[7B4h]
        call    L0104
        je      L0560
        jmp     L04A2
L0560:
        cli
        mov     ss,word ptr cs:L0580
        mov     sp,word ptr cs:L0582
        sti
        pop     ds
        pop     es
        retf
L056F:
        mov     bx,word ptr ds:[7B2h]
        call    L019B
        mov     cx,word ptr ds:[7CEh]
        jne     L054B
        jmp     L0560
L057E:
        jmp     L050D
L0580   dw      0
L0582   dw      0
        mov     ax,word ptr ds:[93Ch]
        sub     word ptr ds:[93Eh],ax
        sub     word ptr ds:[940h],ax
        mov     ax,word ptr ds:[932h]
        sub     word ptr ds:[934h],ax
        sub     word ptr ds:[936h],ax
        mov     ax,word ptr ds:[936h]
        sub     ax,word ptr ds:[934h]
        imul    word ptr ds:[936h]
        or      ax,ax
        jne     L05AC
        jmp     L06C9
L05AC:
        mov     cx,ax
        mov     ax,word ptr ds:[940h]
        xor     dx,dx
        div     cx
        mov     word ptr ds:[948h],ax
        xor     ax,ax
        div     cx
        mov     word ptr ds:[946h],ax
        mov     ax,word ptr ds:[936h]
        sub     ax,word ptr ds:[934h]
        imul    word ptr ds:[934h]
        or      ax,ax
        jne     L05D1
        jmp     L06C9
L05D1:
        mov     cx,ax
        mov     ax,word ptr ds:[93Eh]
        xor     dx,dx
        div     cx
        mov     word ptr ds:[94Ch],ax
        xor     ax,ax
        div     cx
        mov     word ptr ds:[94Ah],ax
        mov     ax,word ptr ds:[936h]
        mul     word ptr ds:[94Ah]
        mov     bp,dx
        mov     si,ax
        mov     ax,word ptr ds:[936h]
        mul     word ptr ds:[94Ch]
        add     bp,ax
        mov     ax,word ptr ds:[934h]
        mul     word ptr ds:[946h]
        sub     si,ax
        sbb     bp,dx
        mov     ax,word ptr ds:[934h]
        imul    word ptr ds:[948h]
        sub     bp,ax
        mov     bx,word ptr ds:[948h]
        mov     dx,word ptr ds:[946h]
        sub     dx,word ptr ds:[94Ah]
        sbb     bx,word ptr ds:[94Ch]
        add     si,dx
        adc     bp,bx
        shl     dx,1
        rcl     bx,1
        mov     ax,word ptr ds:[93Ch]
        ret
        mov     ax,word ptr ds:[940h]
        sub     word ptr ds:[942h],ax
        sub     word ptr ds:[944h],ax
        mov     ax,word ptr ds:[936h]
        sub     word ptr ds:[938h],ax
        sub     word ptr ds:[93Ah],ax
        mov     ax,word ptr ds:[93Ah]
        sub     ax,word ptr ds:[938h]
        imul    word ptr ds:[93Ah]
        or      ax,ax
        jne     L0650
        jmp     L06D5
L0650:
        mov     cx,ax
        mov     ax,word ptr ds:[944h]
        xor     dx,dx
        div     cx
        mov     word ptr ds:[948h],ax
        xor     ax,ax
        div     cx
        mov     word ptr ds:[946h],ax
        mov     ax,word ptr ds:[93Ah]
        sub     ax,word ptr ds:[938h]
        imul    word ptr ds:[938h]
        or      ax,ax
        je      L06D5
        mov     cx,ax
        mov     ax,word ptr ds:[942h]
        xor     dx,dx
        div     cx
        mov     word ptr ds:[94Ch],ax
        xor     ax,ax
        div     cx
        mov     word ptr ds:[94Ah],ax
        mov     ax,word ptr ds:[93Ah]
        mul     word ptr ds:[94Ah]
        mov     bp,dx
        mov     si,ax
        mov     ax,word ptr ds:[93Ah]
        mul     word ptr ds:[94Ch]
        add     bp,ax
        mov     ax,word ptr ds:[938h]
        mul     word ptr ds:[946h]
        sub     si,ax
        sbb     bp,dx
        mov     ax,word ptr ds:[938h]
        imul    word ptr ds:[948h]
        sub     bp,ax
        mov     bx,word ptr ds:[948h]
        mov     dx,word ptr ds:[946h]
        sub     dx,word ptr ds:[94Ah]
        sbb     bx,word ptr ds:[94Ch]
        add     si,dx
        adc     bp,bx
        shl     dx,1
        rcl     bx,1
        mov     ax,word ptr ds:[940h]
        ret
L06C9:
        xor     bp,bp
        xor     bx,bx
        xor     dx,dx
        xor     si,si
        mov     ax,word ptr ds:[93Ch]
        ret
L06D5:
        xor     bp,bp
        xor     bx,bx
        xor     dx,dx
        xor     si,si
        mov     ax,word ptr ds:[940h]
        ret
_seg003_0272_183 endp

SEG003_TEXT ends

        end
