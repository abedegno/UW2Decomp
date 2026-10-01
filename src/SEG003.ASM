; /* target: seg003_0272 */
; /* opts: /ml */
;
; The graphics and 3D span module: the C entry points of the graphics library
; (string_to_screen, plot_pixel, show, fbshow, vcopyfb, rectangle, ...), the routines
; behind them reached through a table of near jumps at _seg003_0272_5278, polygon span
; filling, scaled bitmap drawing with run-time generated code (L1554), and the XFER.DAT
; colour tables (_cXfer). 5D6Dh bytes at file 3450h.
;
; Hand-written assembly with .186: it uses 186 instructions (shifts by an immediate,
; outsb) and no 386 ones, and has no `nop` padding after any forward jump, so the
; forward conditional jumps need no `short`. Most routines run with DS, ES and SS
; pointing at the module's data segment (seg_370D, IDA's seg048MaybeGraphics), which
; the far entry points and the dispatcher at _seg003_0272_5311 switch to, so near data
; is left as numbers (asmgen --no-near-names), except the two symbols.tsv names below.
; Much of the code patches itself: immediates written as 1234h (or 1234) are
; placeholders stored through cs: labels (`Lxxxx equ $+n`).
;
; Ten single zero bytes pad to a word boundary
; (6E1, 3205, 3685, 375B, 38B3, 43EF, 52ED, 536F, 5933, 5B4F). TASM 2.0's EVEN pads
; with 90h in code (tested), as at 3A01 and 3CC1, so these zeros are probably the
; linker's padding between separately assembled word-aligned modules; they are kept
; as `db 0`. Two `jmp` with an E9 displacement that TASM would shorten fit the same
; picture (a jump to a label in another module).
;
; Names: symbols.tsv for every public; XFERDATA is FM Towns _cXfer (init_lighting_
; loads xfer.dat, 500h bytes, into it). nullsub_1 and nullsub_2 are IDA's names for
; called one-byte `ret` routines. Everything else keeps IDA or provisional names.

_ShowClip       equ     0DC4h           ; seg_370D:0DC4, symbols.tsv
_Transparency   equ     0DC5h           ; seg_370D:0DC5, symbols.tsv

        .model  medium
        .186

        extrn   _seg021_22FD_C00:far
        extrn   dseg062_62a6:far
        extrn   seg004_0849:far
        extrn   seg049_3EE2:far
        extrn   seg_370D:far

        .code

        public  _seg003_0272_0
        public  _seg003_0272_8
        public  _seg003_0272_10
        public  _seg003_0272_18
        public  _seg003_0272_28
        public  _seg003_0272_46
        public  _seg003_0272_EC
        public  _seg003_0272_183
        public  _seg003_0272_6E2
        public  _seg003_0272_6E4
        public  _seg003_0272_764
        public  _seg003_0272_788
        public  _seg003_0272_7A8
        public  _DRAW_RELATED_seg003_0272_7F9
        public  _seg003_0272_888
        public  _seg003_0272_9C9
        public  _seg003_0272_9FC
        public  _seg003_0272_A7F
        public  _seg003_0272_B9B
        public  _seg003_0272_C13
        public  _seg003_0272_D1E
        public  _cXfer
        public  _seg003_0272_1373
        public  _seg003_0272_21D4
        public  _seg003_0272_283D
        public  _seg003_0272_2914
        public  _seg003_0272_2945
        public  _seg003_0272_295F
        public  _seg003_0272_2977
        public  _seg003_0272_2A0D
        public  _seg003_0272_2A8F
        public  _seg003_0272_2ABE
        public  _seg003_0272_2AED
        public  _seg003_0272_2B2C
        public  _seg003_0272_2B44
        public  _seg003_0272_2B53
        public  _seg003_0272_2B62
        public  _seg003_0272_2B7A
        public  _SetVideoMode_seg003_0272_2B9B
        public  _seg003_0272_2BF9
        public  _seg003_0272_2C44
        public  _seg003_0272_2D83
        public  _seg003_0272_2F18
        public  _seg003_0272_3094
        public  _seg003_0272_30E3
        public  _seg003_0272_30FB
        public  _seg003_0272_3121
        public  _seg003_0272_3164
        public  _seg003_0272_3195
        public  _seg003_0272_31BE
        public  _nullsub_1
        public  _seg003_0272_31E7
        public  _seg003_0272_3206
        public  _seg003_0272_321B
        public  _seg003_0272_326A
        public  _seg003_0272_326D
        public  _seg003_0272_327D
        public  _seg003_0272_328F
        public  _seg003_0272_32E2
        public  _seg003_0272_331A
        public  _seg003_0272_3321
        public  _seg003_0272_3324
        public  _seg003_0272_336E
        public  _seg003_0272_3371
        public  _seg003_0272_33BE
        public  _seg003_0272_341E
        public  _seg003_0272_3423
        public  _seg003_0272_342E
        public  _seg003_0272_347F
        public  _seg003_0272_34AE
        public  _seg003_0272_34C2
        public  _seg003_0272_34C7
        public  _seg003_0272_368E
        public  _seg003_0272_375C
        public  _seg003_0272_37BE
        public  _seg003_0272_381C
        public  _seg003_0272_38B4
        public  _seg003_0272_395C
        public  _seg003_0272_3989
        public  _seg003_0272_39BB
        public  _seg003_0272_3A02
        public  _seg003_0272_3B0D
        public  _seg003_0272_3B1E
        public  _seg003_0272_3B36
        public  _seg003_0272_3B8F
        public  _seg003_0272_3BE2
        public  _seg003_0272_3BFD
        public  _seg003_0272_3C61
        public  _seg003_0272_4150
        public  _seg003_0272_4186
        public  _seg003_0272_43C5
        public  _nullsub_2
        public  _seg003_0272_43F5
        public  _setup_font
        public  _string_to_screen
        public  _string_width
        public  _init_graphics
        public  _grPageFlip
        public  _grSoftPageFlip
        public  _gr_read_pixel
        public  _plot_pixel
        public  _set_the_color
        public  _init_colors
        public  _seg003_0272_49AE
        public  _seg003_0272_4A3A
        public  _vscreen_focus
        public  _set_the_window
        public  _copy_hidden_to_visible
        public  _copy_visible_to_hidden
        public  _uhline
        public  _uvline
        public  _rectangle
        public  _urectangle
        public  _clear_window
        public  _box
        public  _show
        public  _seg003_0272_5025
        public  _fbshow
        public  _vcopyfb
        public  _vcopy
        public  _seg003_0272_5220
        public  _fbuf_setcolor
        public  _seg003_0272_44DC
        public  _seg003_0272_45BA
        public  _seg003_0272_4665
        public  _seg003_0272_4722
        public  _seg003_0272_47A0
        public  _seg003_0272_4824
        public  _seg003_0272_4866
        public  _seg003_0272_48AD
        public  _seg003_0272_496F
        public  _seg003_0272_49F4
        public  _seg003_0272_4B93
        public  _seg003_0272_4BDA
        public  _seg003_0272_4C64
        public  _seg003_0272_4E0C
        public  _seg003_0272_4E56
        public  _seg003_0272_4EB0
        public  _seg003_0272_4EFA
        public  _seg003_0272_4F39
        public  _seg003_0272_4F78
        public  _seg003_0272_50E7
        public  _seg003_0272_511C
        public  _seg003_0272_5278
        public  _seg003_0272_527B
        public  _seg003_0272_527E
        public  _seg003_0272_5287
        public  _seg003_0272_528D
        public  _seg003_0272_5293
        public  _seg003_0272_5296
        public  _seg003_0272_5299
        public  _seg003_0272_529C
        public  _seg003_0272_529F
        public  _seg003_0272_52A2
        public  _seg003_0272_52A5
        public  _seg003_0272_52A8
        public  _seg003_0272_52B1
        public  _seg003_0272_52B4
        public  _seg003_0272_52B7
        public  _seg003_0272_52BA
        public  _seg003_0272_52BD
        public  _seg003_0272_52C0
        public  _seg003_0272_52E4
        public  _seg003_0272_5311
        public  _seg003_0272_5363
        public  _seg003_0272_536B
        public  _seg003_0272_5869
        public  _seg003_0272_5B50
        public  _seg003_0272_5D47

; seg003_0272_0  (+0)
_seg003_0272_0 proc near
        mov     bp,offset L32B4
        db      0EAh                     ; jmp far ptr _seg003_0272_5311, which TASM would shorten
        dw      offset _seg003_0272_5311
        dw      seg _seg003_0272_5311
_seg003_0272_0 endp

; seg003_0272_8  (+8)
_seg003_0272_8 proc near
        mov     bp,offset _seg003_0272_375C
        db      0EAh                     ; jmp far ptr _seg003_0272_5311, which TASM would shorten
        dw      offset _seg003_0272_5311
        dw      seg _seg003_0272_5311
_seg003_0272_8 endp

; seg003_0272_10  (+10)
_seg003_0272_10 proc near
        mov     bp,offset _seg003_0272_368E
        db      0EAh                     ; jmp far ptr _seg003_0272_5311, which TASM would shorten
        dw      offset _seg003_0272_5311
        dw      seg _seg003_0272_5311
_seg003_0272_10 endp

; seg003_0272_18  (+18)
_seg003_0272_18 proc far
        mov     bp,offset _seg003_0272_46
        jmp     L52FF
L001E:
        mov     bp,offset L004F
        jmp     L52FF
        call    _seg003_0272_28
        retf
_seg003_0272_18 endp

; seg003_0272_28  (+28)
_seg003_0272_28 proc near
        mov     dx,3DAh
L002B:
        call    far ptr _seg021_22FD_C00
        neg     ax
        add     ax,word ptr ds:[652h]
        cmp     ax,4
        jge     L0045
        cmp     ax,0FFFCh
        jle     L0045
        in      al,dx
        test    al,8
        je      L002B
L0045:
        ret
_seg003_0272_28 endp

; seg003_0272_46  (+46)
_seg003_0272_46 proc near
        call    far ptr _seg021_22FD_C00
        mov     word ptr ds:[652h],ax
        ret
L004F:
        ret                             ; the null routine L001E hands the dispatcher
        retf
        db      0Fh dup (0)
        int     2                       ; the debug break the dispatcher at L52FF uses,
        db      'map bitmap error'      ; followed by its message
        dd      0
        dd      12345678h
L007A   dw      0
L007C   dw      0
L007E   dw      0
L0080   dw      0
L0082   dw      0
L0084   dw      0
L0086   dw      0
L0088   dw      0
L008A   dw      0
L008C   dw      0
L008E   dw      0
L0090   dw      0
L0092   dw      0
L0094   dw      0
L0096:
        dw      0                       ; the caller's es
L0098:
        db      54h dup (0)             ; a copy of the 54h bytes at DS:658
_seg003_0272_46 endp

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
        mov     word ptr cs:L0086,bx
        dec     word ptr ds:[7D0h]
        je      L0182
        mov     ax,word ptr ds:[7D0h]
        mov     word ptr cs:L007E,ax
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
        mov     word ptr cs:L0094,cx
        mov     ax,es
        mov     word ptr cs:L0096,ax
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
        mov     di,offset L0098
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
        mov     word ptr cs:L007E,cx
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
        mov     word ptr cs:L007A,dx
        mov     word ptr cs:L007C,bp
        mov     word ptr cs:L0080,di
        mov     word ptr ds:[7B0h],di
        mov     si,di
        mov     word ptr ds:[7B2h],si
        mov     word ptr ds:[7B4h],si
        lodsw
        mov     word ptr cs:L0082,ax
        lodsw
        mov     word ptr ds:[7CEh],ax
        mov     word ptr cs:L0084,ax
        mov     word ptr cs:L0088,1234h
        mov     word ptr cs:L008A,1234h
        mov     word ptr cs:L008E,1234h
        mov     word ptr cs:L008C,1234h
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
        mov     word ptr cs:L0088,ax
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
        mov     word ptr cs:L008A,cx
        mov     word ptr cs:L008C,bx
        jle     L0470
L03FA:
        add     si,word ptr ds:[7E6h]
        adc     bp,word ptr ds:[7E8h]
        mov     ax,word ptr ds:[7E0h]
        mov     dx,word ptr ds:[7DEh]
        mov     word ptr cs:L0090,ax
        mov     word ptr cs:L0092,dx
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
        mov     word ptr cs:L008E,ax
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
        db      0                       ; zero pad to a word boundary (see the header)
_seg003_0272_183 endp

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
L06F8:
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
L0729:
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

; seg003_0272_D1E  (+D1E)
_seg003_0272_D1E proc far
        push    es
        push    ds
        mov     ax,seg seg_370D
        mov     ds,ax
        mov     bx,ss
        mov     word ptr cs:L1550,bx
        mov     word ptr cs:L1552,sp
        cli
        mov     ss,ax
        mov     sp,word ptr ds:[5588h]
        sti
        mov     cx,word ptr ds:[0B26h]
        cmp     cx,2
        jg      L0D45
        jmp     L0E64
L0D45:
        cmp     word ptr ds:[0B24h],2
        jg      L0D4F
        jmp     L0E64
L0D4F:
        mov     ax,word ptr ds:[0B1Ch]
        dec     ax
        xor     dx,dx
        mov     word ptr ds:[0B3Eh],dx
        mov     dl,ah
        mov     ah,al
        xor     al,al
        dec     cx
        div     cx
        inc     cx
        or      ax,ax
        jne     L0D6A
        jmp     L0E64
L0D6A:
        mov     word ptr ds:[0B38h],ax
        mov     ax,cs
        mov     es,ax
        mov     ax,word ptr ds:[0B20h]
        add     ax,cx
        dec     ax
        sub     ax,word ptr ds:[3DF8h]
        dec     ax
        jle     L0D89
        sub     cx,ax
        jg      L0D85
        jmp     L0E64
L0D85:
        mov     word ptr ds:[0B26h],cx
L0D89:
        mov     ax,word ptr ds:[0B20h]
        mov     si,word ptr ds:[3DF4h]
        sub     ax,si
        jge     L0DAB
        neg     ax
        jg      L0D9B
        jmp     L0E64
L0D9B:
        mov     word ptr ds:[0B3Eh],ax
        mov     word ptr ds:[0B20h],si
        sub     word ptr ds:[0B26h],ax
        jg      L0DAB
        jmp     L0E64
L0DAB:
        cmp     word ptr ds:[0B26h],200h
        jl      L0DB6
        jmp     L0E64
L0DB6:
        mov     di,offset L1554
        call    word ptr ds:[0B0Eh]
        xor     ax,ax
        xor     dx,dx
        mov     ah,byte ptr ds:[0B1Eh]
        mov     dl,byte ptr ds:[0B1Fh]
        div     word ptr ds:[0B24h]
        mov     word ptr ds:[0B3Ah],ax
        xor     dx,dx
        mov     cx,word ptr ds:[0B24h]
        mov     bp,word ptr ds:[0B22h]
        cmp     bp,word ptr ds:[3DF6h]
        jle     L0DF4
        sub     bp,word ptr ds:[3DF6h]
        sub     cx,bp
        jle     L0E64
        mov     ax,bp
        mul     word ptr ds:[0B3Ah]
        mov     dx,ax
        mov     bp,word ptr ds:[3DF6h]
L0DF4:
        mov     bx,bp
        sub     bx,cx
        inc     bx
        cmp     bx,word ptr ds:[3DFAh]
        jge     L0E07
        sub     bx,word ptr ds:[3DFAh]
        add     cx,bx
        jle     L0E64
L0E07:
        shl     bp,1
        add     bp,95Ch
        mov     bx,0B28h
        mov     word ptr ds:[0B3Ch],cx
        mov     cl,0FFh
L0E16:
        cmp     cl,dh
        je      L0E40
        mov     al,dh
        mul     byte ptr ds:[0DC0h]
        mov     si,word ptr ds:[0B18h]
        add     si,word ptr ds:[0B3Eh]
        mov     es,word ptr ds:[0B0Ch]
        mov     ds,word ptr ds:[0B1Ah]
        add     si,ax
        mov     di,0B40h
        mov     cx,0F04h
        call    near ptr L1554
        mov     ds,word ptr ss:[0B0Ch]
L0E40:
        mov     es,word ptr ds:[958h]
        mov     di,word ptr [bp]
        dec     bp
        dec     bp
        add     di,word ptr ds:[0B20h]
        mov     si,0B40h
        mov     cx,word ptr ds:[0B26h]
        call    word ptr ds:[0B10h]
        mov     cl,dh
        add     dx,word ptr ds:[0B3Ah]
        dec     word ptr ds:[0B3Ch]
        jne     L0E16
L0E64:
        cli
        mov     ss,word ptr cs:L1550
        mov     sp,word ptr cs:L1552
        sti
        pop     ds
        pop     es
        retf
_seg003_0272_D1E endp

; XFER.DAT, five 256-byte colour translation tables. OVR152 loads the file over these
; bytes at startup (lget("xfer.dat", ..., 500h)); FM Towns calls them _cXfer.
_cXfer label byte
; table 0
        db      25h,24h,1Eh,2Fh,20h,20h,21h,22h,2Eh,2Fh,2Fh,23h,23h,1Eh,0B7h,0B7h
        db      2Fh,2Fh,0B9h,0B9h,0B9h,0BAh,0BAh,0BAh,0BBh,23h,1Eh,1Fh,1Fh,20h,20h,20h
        db      21h,21h,22h,22h,23h,23h,24h,1Eh,1Fh,1Fh,1Fh,1Fh,20h,2Fh,30h,0BAh
        db      0BAh,0BAh,0BAh,0BBh,23h,23h,0B7h,2Fh,2Fh,2Fh,30h,0B9h,0BAh,0BAh,0BBh,0BEh
        db      0B7h,7Ch,7Ch,7Dh,7Eh,7Eh,7Eh,0BBh,0BCh,1Eh,1Fh,1Fh,5Bh,5Bh,5Ch,5Fh
        db      5Fh,60h,60h,60h,60h,24h,24h,1Fh,1Fh,20h,20h,21h,22h,22h,22h,23h
        db      23h,23h,24h,1Fh,1Fh,20h,20h,7Eh,21h,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
        db      20h,20h,7Eh,7Eh,22h,22h,23h,23h,1Fh,20h,20h,20h,21h,21h,22h,0B8h
        db      0B9h,0B9h,0BAh,0BAh,0BBh,0BCh,23h,23h,1Fh,1Fh,1Fh,1Fh,20h,7Dh,7Eh,7Eh
        db      0BAh,0BAh,22h,22h,23h,23h,1Eh,1Fh,1Fh,1Fh,1Fh,1Fh,20h,21h,21h,21h
        db      22h,22h,23h,23h,0B7h,0B7h,7Bh,7Ch,7Ch,7Ch,7Dh,0B9h,7Eh,7Eh,7Eh,0BBh
        db      0BCh,0BCh,0BCh,1Fh,1Fh,1Fh,20h,20h,21h,0BAh,22h,22h,22h,22h,23h,23h
        db      23h,1Eh,1Eh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,21h,21h,21h,21h,22h,23h,23h
        db      24h,1Eh,1Fh,1Fh,1Fh,7Ch,7Ch,7Dh,7Dh,7Eh,21h,7Eh,7Eh,0BCh,23h,23h
        db      23h,23h,23h,5Ch,23h,23h,23h,5Ch,23h,23h,23h,23h,23h,23h,23h,23h
        db      25h,25h,24h,24h,0BEh,24h,7Eh,7Ch,24h,22h,7Ch,0DAh,21h,22h,0F6h,7Eh
; table 1
        db      0F4h,0F4h,0A8h,81h,82h,0F6h,90h,91h,80h,81h,82h,0B1h,0B1h,0A8h,0A9h,80h
        db      81h,81h,82h,82h,82h,83h,0AFh,84h,0B0h,0B1h,0A8h,43h,43h,44h,72h,73h
        db      90h,91h,92h,93h,34h,3Fh,48h,0A9h,43h,43h,43h,81h,81h,3Ch,3Ch,3Ch
        db      83h,3Dh,84h,84h,0B1h,0B2h,0A9h,80h,81h,81h,82h,82h,0ADh,0AEh,0B0h,0B1h
        db      0AAh,0AAh,0ABh,0ABh,0ACh,0ADh,0AEh,0B0h,0B1h,0A9h,0AAh,0AAh,44h,9Fh,0A0h,0Ch
        db      0Ch,0Ch,0Ch,0Bh,0A3h,0A3h,0F4h,0A9h,43h,44h,0CBh,0CCh,0DCh,0DEh,0DEh,0DEh
        db      0DEh,0CFh,48h,43h,44h,44h,45h,0ADh,46h,42h,43h,43h,43h,43h,44h,44h
        db      44h,45h,0ADh,0AEh,0AFh,0B0h,0B1h,0B2h,44h,44h,45h,45h,45h,46h,84h,0ABh
        db      0ABh,0ACh,0ACh,0AEh,0AFh,0B0h,0B2h,48h,43h,43h,43h,44h,44h,0ACh,0ADh,0AEh
        db      0AFh,0AFh,0B0h,0B0h,0B1h,0B1h,0A8h,0A9h,0AAh,0AAh,44h,44h,0ADh,0ADh,0ADh,0AEh
        db      0AEh,0B0h,0B1h,0B1h,0A9h,0A9h,0AAh,0ABh,0ABh,0ABh,0ABh,0ACh,0ADh,0AEh,0AEh,0AFh
        db      0B0h,0B0h,0B0h,0A9h,43h,44h,44h,45h,45h,3Dh,84h,84h,3Eh,3Eh,3Eh,0B1h
        db      0B2h,0A8h,0A9h,0A9h,0AAh,0AAh,0AAh,44h,0ACh,0ADh,0ADh,0AEh,0AEh,0B0h,0B0h,0B1h
        db      0B2h,0A9h,0A9h,0AAh,0AAh,44h,44h,0ACh,0ADh,0ADh,0AEh,0AFh,0AFh,0B0h,0B1h,0B1h
        db      0B0h,0B1h,0B1h,0B0h,0B1h,0B1h,0B1h,0B0h,0B1h,0B1h,0B1h,0B0h,0B1h,0B1h,0B1h,0B1h
        db      0F4h,0F4h,0F4h,0F4h,0B2h,0B2h,0AEh,0ABh,0F4h,0DCh,0AAh,0ABh,45h,47h,0ABh,0AEh
; table 2
        db      0Bh,0Bh,4Ch,0D8h,0D9h,5Bh,5Bh,5Ch,0D7h,0D8h,0D9h,0Ch,0Ch,4Ch,9Bh,9Ch
        db      9Ch,9Dh,9Eh,9Fh,9Fh,0A0h,0A0h,0A1h,0A1h,0A2h,4Ch,4Ch,4Dh,5Ah,5Bh,5Bh
        db      5Ch,5Ch,5Ch,5Ch,60h,0Bh,0Bh,4Ch,4Ch,9Ch,9Ch,9Dh,0D8h,72h,0DAh,0DAh
        db      0DBh,0DBh,0A1h,0A1h,0A2h,0Bh,9Bh,9Ch,44h,44h,0CAh,0CBh,0CCh,0A0h,0A1h,0Ch
        db      4Dh,9Dh,9Eh,9Fh,9Fh,0A0h,0A0h,0A1h,0Ch,4Ch,4Dh,4Dh,4Eh,4Eh,4Eh,4Fh
        db      4Fh,50h,50h,0Ch,0Ch,0Ch,0Bh,4Ch,4Dh,4Dh,4Eh,4Eh,5Ch,5Ch,5Ch,5Fh
        db      0Ch,0Bh,0Bh,4Dh,9Dh,9Eh,9Fh,0A0h,0A0h,4Ch,4Dh,4Dh,4Dh,4Dh,4Dh,9Dh
        db      9Eh,9Fh,0A0h,0A0h,0A0h,0A1h,0Ch,0Ch,9Dh,9Eh,9Fh,9Fh,0A0h,0A0h,0A1h,9Dh
        db      9Eh,9Fh,0A0h,0A0h,0A1h,0A1h,0Bh,0Bh,4Ch,9Ch,9Dh,9Dh,9Eh,9Fh,9Fh,0A0h
        db      0A0h,0A1h,0A1h,0A1h,0A1h,0Ch,4Ch,4Dh,4Dh,4Dh,4Dh,4Eh,4Eh,4Eh,4Eh,4Eh
        db      4Eh,0A1h,0Ch,0Ch,4Ch,9Ch,9Ch,9Dh,9Dh,9Eh,9Fh,0A0h,0A0h,0A0h,0A0h,0A1h
        db      0A1h,0Ch,0Ch,4Ch,4Dh,9Dh,9Eh,9Fh,0A0h,0A0h,0DCh,0A1h,0A1h,0A1h,0A1h,0A2h
        db      0A2h,4Ch,4Ch,4Dh,4Dh,4Dh,4Dh,4Dh,4Eh,4Eh,4Eh,0A0h,0A0h,0A1h,0A1h,0Ch
        db      0Ch,4Ch,4Ch,4Dh,4Dh,4Dh,4Dh,9Fh,9Fh,0A0h,0A0h,0A0h,0A1h,0A1h,0Ch,0Ch
        db      0Ch,0Ch,0Ch,4Eh,0Ch,0Ch,0Ch,0Ch,0Ch,0Ch,0Ch,0Ch,0Ch,0Ch,0Ch,0Ch
        db      0Bh,0Bh,0Bh,0Bh,0Ch,0Ch,0A0h,9Dh,0Bh,0A0h,4Dh,4Eh,4Eh,0A1h,0ADh,0A0h
; table 3  (_seg003_0272_788 maps the screen through it)
        db      0DFh,0A3h,2h,10h,2Ch,0B7h,7Ch,20h,37h,10h,2Bh,0A1h,0A0h,0Dh,0Eh,0Eh
        db      0Fh,10h,0F7h,0F7h,13h,8Dh,8Eh,8Fh,90h,91h,1Ah,1Bh,1Bh,1Ch,1Dh,1Eh
        db      1Fh,1Fh,20h,21h,23h,23h,0DEh,27h,69h,28h,29h,2Ah,2Bh,2Ch,2Ch,13h
        db      14h,14h,8Fh,90h,92h,93h,36h,36h,37h,39h,80h,80h,81h,82h,46h,47h
        db      40h,40h,41h,42h,43h,44h,44h,45h,47h,49h,4Ah,4Ah,4Bh,4Ch,4Dh,4Fh
        db      0Ch,0Ch,0Ch,0Ch,0Ch,0Bh,0Bh,57h,58h,58h,59h,5Ah,5Bh,5Ch,5Ch,5Ch
        db      5Ch,0DEh,0DEh,6Bh,6Dh,64h,65h,66h,66h,0B3h,69h,0D2h,6Bh,6Ch,63h,6Dh
        db      64h,65h,0D7h,0D8h,0D9h,0DBh,0DCh,0DDh,6Dh,78h,79h,65h,7Ah,7Bh,67h,7Fh
        db      7Fh,80h,81h,82h,0F6h,91h,93h,76h,69h,0B4h,89h,8Ah,8Bh,8Ch,8Ch,8Dh
        db      8Eh,73h,73h,0DBh,0DCh,0DCh,96h,97h,97h,98h,99h,9Ah,9Ah,9Bh,9Ch,9Dh
        db      9Eh,9Fh,0A0h,0A1h,0A4h,0A4h,0A5h,0A6h,0A7h,0A8h,0A8h,0AAh,44h,44h,44h,45h
        db      45h,46h,46h,0B3h,0D2h,6Ch,6Fh,0B6h,7Ah,8Eh,8Fh,0B9h,90h,90h,91h,92h
        db      93h,0C1h,0C2h,0C3h,0C3h,0C4h,0C5h,0C6h,0C7h,0C7h,0C8h,0C9h,0CAh,0CBh,0CCh,0CDh
        db      0CEh,0D1h,0B3h,0D2h,0D3h,0D4h,0D5h,0D6h,0D7h,0D7h,0D8h,0D9h,0DAh,0DBh,0DCh,0DDh
        db      0A0h,0A1h,0A0h,9Fh,0A0h,0A1h,0A0h,0A0h,0A1h,0A1h,0A1h,0A0h,0A1h,0A1h,0A1h,0A0h
        db      0DFh,0DFh,0CFh,0DFh,0B1h,0A2h,8Eh,0F7h,95h,7Ch,0A6h,4Ch,9Bh,5Bh,0A9h,0D7h
; table 4
        db      1h,1h,0C8h,16h,90h,0BBh,0BCh,24h,82h,15h,16h,0F5h,0Bh,0D8h,0D8h,0F6h
        db      0F6h,0F6h,90h,91h,92h,93h,34h,34h,0C0h,35h,0C9h,0D9h,0D9h,73h,7Eh,7Eh
        db      23h,23h,24h,25h,25h,26h,0F8h,0D8h,0D8h,72h,67h,68h,90h,32h,32h,17h
        db      18h,18h,34h,0C0h,35h,0F8h,44h,82h,3Ch,83h,3Dh,84h,3Eh,85h,3Fh,0F4h
        db      0CAh,45h,46h,46h,47h,47h,0B2h,48h,0F4h,9Dh,9Eh,9Fh,0A0h,0A1h,0Ch,53h
        db      53h,54h,54h,55h,55h,56h,1h,9Dh,0DAh,0DBh,74h,0DEh,60h,60h,61h,61h
        db      62h,62h,0F3h,0DAh,0DBh,74h,0DCh,75h,93h,0D8h,0D9h,0D9h,0DAh,0DAh,0DAh,0DBh
        db      74h,0DCh,75h,76h,76h,95h,77h,0D0h,73h,74h,74h,92h,75h,93h,19h,45h
        db      46h,84h,3Eh,85h,3Fh,3Fh,87h,0F8h,0D8h,0D9h,73h,73h,74h,92h,92h,93h
        db      94h,95h,95h,95h,77h,77h,0C9h,9Eh,9Eh,9Fh,0A0h,0A0h,0A1h,0A1h,0A1h,0CEh
        db      0A2h,0A3h,0A3h,0F5h,44h,44h,45h,45h,46h,46h,0AFh,0B0h,0B0h,0B1h,0B1h,0B2h
        db      48h,0F4h,0F4h,0D8h,0D9h,73h,74h,74h,92h,93h,19h,0BFh,0BFh,0C0h,86h,35h
        db      87h,0C9h,0C9h,0CAh,0CAh,0CBh,0CCh,0CCh,0CDh,0CDh,0DDh,0CEh,0CEh,0CFh,0DFh,0D0h
        db      0F2h,0D8h,0D9h,0D9h,0DAh,0DBh,0DBh,0DCh,0DDh,0DDh,0CEh,76h,0DEh,0CFh,77h,0D0h
        db      0Bh,0Bh,0Bh,0Ch,0Bh,0A3h,0Bh,0Bh,0Bh,0F5h,0A3h,0Bh,0Bh,0F5h,0F5h,0Bh
        db      1h,1h,0F2h,0F3h,0F2h,1h,93h,0F6h,0F8h,0DEh,0ADh,0Ch,0A1h,0DEh,0AEh,0DDh

; seg003_0272_1373  (+1373): span routines and the scaler generators, reached through
; the pointers at DS:B0E and DS:B10
_seg003_0272_1373 proc near
        mov     bx,offset _cXfer+400h
L1376:
        lodsb
        or      al,al
        je      L1384
        mov     al,byte ptr es:[di]
        xlat    byte ptr cs:[bx]
L1380:
        stosb
        loop    L1376
        retn
L1384:
        inc     di
        loop    L1376
        retn
L1388:
        mov     bx,offset _cXfer+300h
L138B:
        lodsb
        or      al,al
        je      L1399
        mov     al,byte ptr es:[di]
        xlat    byte ptr cs:[bx]
        stosb
        loop    L138B
        retn
L1399:
        inc     di
        loop    L138B
        retn
L139D:
L139E equ $+1
        sub     al,0F9h
        mov     bx,offset _cXfer
        add     bh,al
        mov     al,byte ptr es:[di]
        xlat    byte ptr cs:[bx]
        jmp     short L13BA
        mov     bx,offset _cXfer
        lodsb
        dec     cx
        je      L13CC
        or      al,al
        je      L13C2
L13B6:
        cmp     al,0F9h
        jae     L139D
L13BA:
        stosb
        lodsb
        or      al,al
        loopne  L13B6
        jcxz    L13CE
L13C2:
        inc     di
        lodsb
        or      al,al
        loope   L13C2
        jcxz    L13CE
        jmp     L13B6
L13CC:
        or      al,al
L13CE:
        je      L13D5
        cmp     al,0F9h
        jae     L13D5
        stosb
L13D5:
        retn
        rep movsb
        retn
        lodsb
        dec     cx
        je      L13F3
        or      al,al
        je      L13E9
L13E1:
        stosb
        lodsb
        or      al,al
        loopne  L13E1
        jcxz    L13F5
L13E9:
        inc     di
        lodsb
        or      al,al
        loope   L13E9
        jcxz    L13F5
        jmp     L13E1
L13F3:
        or      al,al
L13F5:
        je      L13F8
        stosb
L13F8:
        retn
        mov     bl,byte ptr ds:[0DC1h]
        lodsb
        dec     cx
        je      L1419
        or      al,al
        je      L140F
L1405:
        mov     al,bl
        stosb
        lodsb
        or      al,al
        loopne  L1405
        jcxz    L141B
L140F:
        inc     di
        lodsb
        or      al,al
        loope   L140F
        jcxz    L141B
        jmp     L1405
L1419:
        or      al,al
L141B:
        je      L1420
        mov     al,bl
        stosb
L1420:
        retn
        xor     dx,dx
        xor     bp,bp
        mov     ax,word ptr ds:[0B1Ch]
        shr     ax,1
        mov     byte ptr ds:[0DC0h],al
        test    byte ptr ds:[0B39h],0FFh
        je      L146F
L1434:
        xor     bx,bx
        mov     bl,dh
        mov     si,0E820h
        shr     bx,1
        jb      L1442
        mov     si,0E8D2h
L1442:
        sub     bx,bp
        je      L1459
        add     bp,bx
        cmp     bl,0FFh
        jne     L1452
        mov     al,4Eh
        stosb
        jmp     short L1459
L1452:
        mov     ax,0C683h
        stosw
        mov     al,bl
        stosb
L1459:
        mov     al,0ACh
        stosb
        mov     ax,si
        stosw
        mov     al,36h
        stosb
        mov     ax,0AAD7h
        stosw
        inc     bp
        add     dx,word ptr ds:[0B38h]
        loop    L1434
        jmp     short L14A6
L146F:
        mov     bp,0FFF8h
L1472:
        xor     bx,bx
        mov     bl,dh
        cmp     bx,bp
        je      L149D
        mov     bp,bx
        test    bx,1
        je      L148C
        mov     ax,0E088h
        stosw
        mov     ax,0E820h
        stosw
        jmp     short L1497
L148C:
        mov     al,0ACh
        stosb
        mov     ax,0C488h
        stosw
        mov     ax,0E8D2h
        stosw
L1497:
        mov     al,36h
        stosb
        mov     al,0D7h
        stosb
L149D:
        mov     al,0AAh
        stosb
        add     dx,word ptr ds:[0B38h]
        loop    L1472
L14A6:
        mov     al,0C3h
        stosb
        retn
        mov     bp,word ptr ds:[0B26h]
        mov     ax,word ptr ds:[0B1Ch]
        mov     byte ptr ds:[0DC0h],al
        mov     ax,word ptr ds:[0B38h]
        test    word ptr ds:[0B3Eh],0FFFFh
        je      L14CF
        mov     bx,word ptr ds:[0B3Eh]
        imul    bx
        mov     al,ah
        mov     ah,dl
        mov     word ptr ds:[0B3Eh],ax
        mov     ax,word ptr ds:[0B38h]
L14CF:
        mov     bx,ax
        test    ah,0FFh
        jne     L152A
        cmp     ax,128h
        jle     L1511
        xor     bh,bh
        mov     al,51h
        stosb
        xor     si,si
L14E2:
        xor     dx,dx
L14E4:
        add     bh,bl
        inc     dx
        jae     L14E4
        cmp     dx,1
        jne     L14F1
        inc     si
        jmp     short L1502
L14F1:
        mov     al,0A4h
        mov     cx,si
        rep stosb
        xor     si,si
        mov     al,0ACh
        stosb
        mov     al,0AAh
        mov     cx,dx
        rep stosb
L1502:
        sub     bp,dx
        jg      L14E2
        mov     al,0A4h
        mov     cx,si
        rep stosb
        mov     al,59h
        stosb
        jmp     L14A6
L1511:
        xor     bh,bh
L1513:
        xor     dx,dx
L1515:
        inc     dx
        add     bh,bl
        jae     L1515
        mov     al,0ACh
        stosb
        mov     al,0AAh
        mov     cx,dx
        rep stosb
        sub     bp,dx
        jg      L1513
        jmp     L14A6
L152A:
        xor     dx,dx
L152C:
        mov     al,0A4h
        stosb
        add     dx,bx
        dec     dh
        je      L154A
        cmp     dh,1
        jg      L1541
        mov     al,46h
        dec     dh
        stosb
        jmp     short L154A
L1541:
        mov     ax,0C683h
        stosw
        mov     al,dh
        stosb
        xor     dh,dh
L154A:
        dec     bp
        jne     L152C
        jmp     L14A6
L1550   dw      0
L1552   dw      0
L1554   db      0C80h dup (0)           ; code the generators above write and _seg003_0272_D1E calls
_seg003_0272_1373 endp

; seg003_0272_21D4  (+21D4)
_seg003_0272_21D4 proc near
        cmp     byte ptr ds:[_Transparency],0
        jne     L21E4
        mov     word ptr ds:[0DC2h],offset L5372
        jmp     L2296
L21E4:
        mov     word ptr ds:[0DC2h],offset L5467
        jmp     L2296
L21ED:
        cmp     byte ptr ds:[_Transparency],0
        jne     L220B
        test    ax,3
        je      L2202
        mov     word ptr ds:[0DC2h],offset L5578
        jmp     L2296
L2202:
        mov     word ptr ds:[0DC2h],offset L568B
        jmp     L2296
L220B:
        mov     word ptr ds:[0DC2h],offset L56E3
        jmp     L2296
L2214:
        cmp     byte ptr ds:[_Transparency],0
        jne     L2223
        mov     word ptr ds:[0DC2h],offset L06F8
        jmp     short L2296
L2223:
        mov     word ptr ds:[0DC2h],offset L0729
        jmp     short L2296
L222B:
        cmp     byte ptr ds:[_Transparency],0
        jne     L223A
        mov     word ptr ds:[0DC2h],offset L58C7
        jmp     short L2296
L223A:
        mov     word ptr ds:[0DC2h],offset L58F8
        jmp     short L2296
L2242:
        mov     word ptr ds:[0DC2h],offset _seg003_0272_A7F
        mov     byte ptr ds:[0DCAh],0
        jmp     L2373
L2250:
        cmp     bx,di
        je      L2266
        mov     word ptr ds:[0DC2h],offset L581B
        jg      L2261
        mov     bp,offset L2373
        jmp     short L227B
L2261:
        mov     bp,offset L243F
        jmp     short L227B
L2266:
        mov     bp,offset L2373
        cmp     ax,si
        jg      L2275
        mov     word ptr ds:[0DC2h],offset L586D
        jmp     short L227B
L2275:
        mov     word ptr ds:[0DC2h],offset L581B
L227B:
        cmp     bp,offset L243F
        jne     L2284
        sub     di,dx
        inc     di
L2284:
        shl     di,1
        mov     di,word ptr [di+36ACh]
        shr     si,2
        add     di,si
        mov     byte ptr ds:[0DCAh],1
        jmp     bp
L2296:
        mov     word ptr ds:[55ECh],si
        push    di
        mov     di,word ptr ds:[141Bh]
        and     word ptr [di],7FFFh
        mov     bp,word ptr ds:[0DC6h]
        mov     di,word ptr ds:[0DC8h]
        push    cx
        sub     cx,bp
        sub     dx,di
        cmp     byte ptr ds:[_ShowClip],0
        jne     L22B9
        jmp     short L230E
L22B9:
        mov     si,ax
        add     si,cx
        dec     si
        sub     si,word ptr ds:[3DF4h]
        jge     L22C7
        jmp     L236F
L22C7:
        mov     si,word ptr ds:[3DF4h]
        sub     si,ax
        jle     L22D5
        add     ax,si
        sub     cx,si
        add     bp,si
L22D5:
        mov     si,word ptr ds:[3DF8h]
        sub     si,ax
        jge     L22E0
        jmp     L236F
L22E0:
        inc     si
        cmp     cx,si
        jle     L22E7
        mov     cx,si
L22E7:
        mov     si,bx
        sub     si,dx
        inc     si
        sub     si,word ptr ds:[3DF6h]
        jg      L236F
        mov     si,word ptr ds:[3DF6h]
        sub     si,bx
        jge     L2300
        sub     di,si
        add     dx,si
        add     bx,si
L2300:
        mov     si,bx
        sub     si,word ptr ds:[3DFAh]
        jle     L236F
        cmp     dx,si
        jle     L230E
        mov     dx,si
L230E:
        mov     si,cx
        pop     cx
        xchg    cx,dx
        neg     bx
        add     bx,0C7h
        shl     bx,3
        add     bx,0DCBh
        add     si,ax
        dec     si
        xchg    di,ax
        push    bp
        mov     bp,dx
        mul     dx
        pop     dx
        cmp     word ptr ds:[55ECh],0A000h
        je      L233B
        add     dx,ax
        pop     ax
        add     dx,ax
        mov     ax,bx
        jmp     short L2351
L233B:
        add     ax,3
        shr     dx,2
        shr     ax,2
        add     dx,ax
        pop     ax
        add     dx,ax
        mov     ax,bx
        add     bp,3
        shr     bp,2
L2351:
        mov     word ptr [bx+2],di
        mov     word ptr [bx+4],si
        mov     word ptr [bx+6],dx
        add     dx,bp
        add     bx,8
        loop    L2351
        or      word ptr [bx],8000h
        mov     word ptr ds:[141Bh],bx
        mov     si,ax
        jmp     word ptr ds:[0DC2h]
L236F:
        add     sp,4
        retn
L2373:
        add     cx,ax
        dec     cx
        and     ax,0FFFCh
        and     cx,-4
        add     cx,4
        sub     cx,ax
        mov     si,cx
        shr     si,2
        push    si
        cmp     byte ptr ds:[_ShowClip],0
        je      L23F7
        mov     si,word ptr ds:[3DF6h]
        mov     bp,bx
        sub     bp,dx
        inc     bp
        cmp     si,bp
        jge     L239E
        jmp     L243B
L239E:
        sub     si,bx
        jge     L23B7
        add     dx,si
        add     bx,si
        push    ax
        push    dx
        mov     dx,cx
        shr     dx,2
        mov     ax,si
        neg     ax
        mul     dx
        add     di,ax
        pop     dx
        pop     ax
L23B7:
        mov     si,bx
        sub     si,word ptr ds:[3DFAh]
        jle     L243B
        cmp     dx,si
        jle     L23C5
        mov     dx,si
L23C5:
        mov     si,word ptr ds:[3DF8h]
        sub     si,ax
        jl      L243B
        inc     si
        cmp     cx,si
        jle     L23D4
        mov     cx,si
L23D4:
        mov     si,word ptr ds:[3DF4h]
        mov     bp,ax
        add     bp,cx
        dec     bp
        cmp     si,bp
        jg      L243B
        sub     si,ax
        jle     L23F7
        add     ax,si
        sub     cx,si
        mov     bp,si
        neg     bp
        and     bp,3
        add     si,bp
        sar     si,2
        add     di,si
L23F7:
        mov     bp,word ptr ds:[141Bh]
        and     word ptr [bp],7FFFh
        add     cx,ax
        dec     cx
        neg     bx
        add     bx,0C7h
        shl     bx,3
        add     bx,0DCBh
        mov     si,bx
        pop     bp
        cmp     byte ptr ds:[0DCAh],0
        je      L241E
        mov     bp,word ptr ds:[36A8h]
L241E:
        mov     word ptr [bx+2],ax
        mov     word ptr [bx+4],cx
        mov     word ptr [bx+6],di
        add     di,bp
        add     bx,8
        dec     dx
        jne     L241E
        or      word ptr [bx],8000h
        mov     word ptr ds:[141Bh],bx
        jmp     word ptr ds:[0DC2h]
L243B:
        add     sp,2
        retn
L243F:
        add     cx,ax
        dec     cx
        and     ax,0FFFCh
        and     cx,-4
        add     cx,4
        sub     cx,ax
        mov     bp,cx
        shr     bp,2
        mov     si,word ptr ds:[141Bh]
        and     word ptr [si],7FFFh
        add     cx,ax
        dec     cx
        sub     bx,dx
        inc     bx
        mov     si,0DCBh
        push    dx
        mov     bp,word ptr ds:[36A8h]
L2468:
        mov     word ptr [si],bx
        mov     word ptr [si+2],ax
        mov     word ptr [si+4],cx
        mov     word ptr [si+6],di
        inc     bx
        sub     di,bp
        add     si,8
        dec     dx
        jne     L2468
        or      word ptr [si],8000h
        mov     word ptr ds:[141Bh],si
        mov     si,0DCBh
        call    word ptr ds:[0DC2h]
        pop     dx
        mov     bx,0DCBh
        mov     ax,0C7h
L2492:
        mov     word ptr [bx],ax
        dec     ax
        add     bx,8
        dec     dx
        jne     L2492
        retn
        call    _seg003_0272_34C2
        mov     ax,cx
        shl     cx,1
        shl     cx,1
        mov     si,415Eh
        mov     di,si
        add     di,cx
        mov     cx,8
        rep movsw
        mov     cx,ax
        mov     word ptr ds:[1B86h],2D42h
        mov     ch,cl
        mov     cl,5
        mov     si,4162h
        mov     di,1422h
        mov     word ptr ds:[19EAh],0
L24C9:
        mov     ax,word ptr [si]
        mov     bx,word ptr [si+2]
        cmp     bx,word ptr [si+6]
        je      L254C
        jg      L2504
        shl     ax,cl
        mov     word ptr [di+4],ax
        mov     word ptr [di+6],bx
        mov     dx,word ptr [si+4]
        shl     dx,cl
        mov     word ptr [di+2],dx
        mov     bp,word ptr [si+6]
        mov     word ptr [di],bp
        sub     ax,dx
        je      L24F8
        cwd
        sub     bx,bp
        neg     bx
        idiv    bx
        sub     word ptr [di+2],ax
L24F8:
        mov     word ptr [di+8],ax
        cmp     bp,word ptr [si+0Ah]
        jg      L2532
        dec     word ptr [di]
        jmp     short L2532
L2504:
        shl     ax,cl
        mov     word ptr [di+2],ax
        mov     word ptr [di],bx
        mov     dx,word ptr [si+4]
        shl     dx,cl
        mov     word ptr [di+4],dx
        mov     bp,word ptr [si+6]
        mov     word ptr [di+6],bp
        sub     dx,ax
        mov     ax,dx
        je      L2527
        cwd
        sub     bx,bp
        idiv    bx
        sub     word ptr [di+2],ax
L2527:
        mov     word ptr [di+8],ax
        cmp     bp,word ptr [si+0Ah]
        jl      L2532
        inc     word ptr [di+6]
L2532:
        add     di,0Ch
        inc     word ptr ds:[19EAh]
        add     si,4
        dec     ch
        jg      L24C9
        jmp     L25C4
L2543:
        add     si,4
        dec     ch
        jne     L254F
        jmp     short L25C4
L254C:
        mov     bp,word ptr [si-2]
L254F:
        mov     dx,bx
        sub     dx,word ptr [si+0Ah]
        je      L2543
        sub     bp,bx
        neg     bp
        xor     bp,dx
        js      L2583
        shl     ax,cl
        mov     word ptr [di+2],ax
        mov     word ptr [di+4],ax
        mov     word ptr [di],bx
        mov     word ptr [di+6],bx
        mov     word ptr [di+8],0
        add     di,0Ch
        inc     word ptr ds:[19EAh]
        add     si,4
        dec     ch
        jle     L2581
        jmp     L24C9
L2581:
        jmp     short L25C4
L2583:
        or      dx,dx
        js      L25BA
        shl     ax,cl
        mov     word ptr [di+2],ax
        mov     word ptr [di+4],ax
        mov     word ptr [di],bx
        mov     word ptr [di+6],bx
        mov     word ptr [di+8],0
        mov     ax,word ptr [si+4]
        shl     ax,cl
        mov     word ptr [di+0Eh],ax
        mov     word ptr [di+10h],ax
        mov     word ptr [di+0Ch],bx
        mov     word ptr [di+12h],bx
        mov     word ptr [di+14h],0
        add     di,18h
        inc     word ptr ds:[19EAh]
        inc     word ptr ds:[19EAh]
L25BA:
        add     si,4
        dec     ch
        jle     L25C4
        jmp     L24C9
L25C4:
        mov     cx,word ptr ds:[19EAh]
        cmp     cx,2
        jge     L25CE
        retn
L25CE:
        mov     si,1422h
        xor     ax,ax
        mov     bp,0Ch
        mov     dl,0FFh
L25D8:
        mov     bx,word ptr [si]
        mov     byte ptr [bx+1B8Ah],dl
        cmp     ax,bx
        jg      L25E4
        mov     ax,bx
L25E4:
        shl     bx,1
        mov     di,si
        xchg    di,word ptr [bx+19F0h]
        mov     word ptr [si+0Ah],di
        mov     bx,word ptr [si+6]
        mov     byte ptr [bx+1B89h],dl
        add     si,bp
        loop    L25D8
        mov     word ptr ds:[1420h],ax
        mov     word ptr ds:[1B84h],ax
        mov     word ptr ds:[19ECh],0
L2606:
        mov     bx,word ptr ds:[1B84h]
        mov     di,19C2h
        mov     si,di
        mov     cx,word ptr ds:[19ECh]
        jcxz    L2620
L2615:
        lodsw
        mov     bp,ax
        cmp     bx,word ptr [bp+6]
        jl      L261E
        stosw
L261E:
        loop    L2615
L2620:
        mov     byte ptr [bx+1B8Ah],cl
        mov     si,bx
        shl     si,1
        mov     ax,word ptr [si+19F0h]
        mov     word ptr [si+19F0h],cx
        or      ax,ax
        je      L2648
L2634:
        mov     si,ax
        stosw
        mov     ax,word ptr [si+0Ah]
        or      ax,ax
        je      L2648
        mov     si,ax
        stosw
        mov     ax,word ptr [si+0Ah]
        or      ax,ax
        jne     L2634
L2648:
        mov     dx,di
        sub     dx,19C2h
        shr     dx,1
        mov     word ptr ds:[19ECh],dx
        or      dx,dx
        jne     L265B
        jmp     L282D
L265B:
        cmp     dl,2
        je      L2663
        jmp     L275E
L2663:
        mov     bx,word ptr ds:[19C2h]
        mov     bp,word ptr ds:[19C4h]
        mov     ax,word ptr [bx+2]
        add     ax,word ptr [bx+8]
        sub     ax,word ptr [bp+8]
        cmp     ax,word ptr [bp+2]
        jne     L267F
        mov     ax,word ptr [bx+4]
        cmp     ax,word ptr [bp+4]
L267F:
        jl      L2683
        xchg    bx,bp
L2683:
        mov     ax,word ptr [bx+4]
        cmp     ax,word ptr [bp+4]
        jle     L268E
        jmp     L27AE
L268E:
        mov     di,word ptr ds:[1B86h]
        mov     cl,5
        mov     word ptr ds:[19C2h],bx
        mov     word ptr ds:[19C4h],bp
        mov     dx,word ptr [bx+2]
        mov     si,word ptr [bx+8]
        mov     ax,word ptr [bp+8]
        mov     bp,word ptr [bp+2]
        sub     dx,10h
        add     bp,10h
        mov     bx,word ptr ds:[1B84h]
        mov     word ptr ds:[141Eh],ax
        cmp     si,ax
        jle     L270B
L26B9:
        mov     ax,bx
        stosw
        add     dx,si
        add     bp,word ptr ds:[141Eh]
        mov     ax,dx
        sar     ax,cl
        stosw
        mov     ax,bp
        sar     ax,cl
        stosw
        dec     bx
        test    byte ptr [bx+1B8Ah],cl
        jne     L273F
        cmp     dx,bp
        jg      L26F5
        mov     ax,bx
        stosw
        add     dx,si
        add     bp,word ptr ds:[141Eh]
        mov     ax,dx
        sar     ax,cl
        stosw
        mov     ax,bp
        sar     ax,cl
        stosw
        dec     bx
        test    byte ptr [bx+1B8Ah],cl
        jne     L273F
        cmp     dx,bp
        jle     L26B9
L26F5:
        xchg    dx,bp
        xchg    si,word ptr ds:[141Eh]
        mov     ax,word ptr ds:[19C2h]
        xchg    ax,word ptr ds:[19C4h]
        mov     word ptr ds:[19C2h],ax
        sub     dx,20h
        add     bp,20h
L270B:
        mov     ax,bx
        stosw
        add     dx,si
        add     bp,word ptr ds:[141Eh]
        mov     ax,dx
        sar     ax,cl
        stosw
        mov     ax,bp
        sar     ax,cl
        stosw
        dec     bx
        test    byte ptr [bx+1B8Ah],cl
        jne     L273F
        mov     ax,bx
        stosw
        add     dx,si
        add     bp,word ptr ds:[141Eh]
        mov     ax,dx
        sar     ax,cl
        stosw
        mov     ax,bp
        sar     ax,cl
        stosw
        dec     bx
        test    byte ptr [bx+1B8Ah],cl
        je      L270B
L273F:
        add     dx,10h
        sub     bp,10h
        mov     word ptr ds:[1B86h],di
        mov     word ptr ds:[1B84h],bx
        mov     si,word ptr ds:[19C2h]
        mov     word ptr [si+2],dx
        mov     si,word ptr ds:[19C4h]
        mov     word ptr [si+2],bp
        jmp     L2606
L275E:
        test    dl,1
        je      L27AE
        mov     di,word ptr ds:[1B86h]
        mov     ax,bx
        stosw
        mov     cl,dl
        xor     ch,ch
        mov     dx,8000h
        mov     bp,7FFFh
        mov     si,19C2h
L2777:
        lodsw
        mov     bx,ax
        mov     ax,word ptr [bx+8]
        add     ax,word ptr [bx+2]
        mov     word ptr [bx+2],ax
        cmp     dx,ax
        jg      L2789
        mov     dx,ax
L2789:
        cmp     bp,ax
        jl      L278F
        mov     bp,ax
L278F:
        loop    L2777
        mov     cl,5
        mov     ax,bp
        sub     ax,10h
        sar     ax,cl
        stosw
        mov     ax,dx
        add     ax,10h
        sar     ax,cl
        stosw
        mov     word ptr ds:[1B86h],di
        dec     word ptr ds:[1B84h]
        jmp     L2606
L27AE:
        mov     dh,dl
        dec     dh
        xor     ch,ch
L27B4:
        mov     cl,dh
        mov     si,19C2h
        xor     bl,bl
L27BB:
        lodsw
        mov     di,ax
        mov     bp,word ptr [si]
        mov     ax,word ptr [di+2]
        add     ax,word ptr [di+8]
        sub     ax,word ptr [bp+8]
        cmp     ax,word ptr [bp+2]
        jl      L27D5
        inc     bl
        mov     word ptr [si-2],bp
        mov     word ptr [si],di
L27D5:
        loop    L27BB
        or      bl,bl
        je      L27DF
        dec     dh
        jne     L27B4
L27DF:
        mov     di,word ptr ds:[1B86h]
        mov     si,19C2h
        mov     bp,word ptr ds:[1B84h]
        mov     dh,dl
        shr     dh,1
        mov     cl,5
L27F0:
        mov     ax,bp
        stosw
        lodsw
        mov     bx,ax
        mov     ax,word ptr [bx+2]
        add     ax,word ptr [bx+8]
        mov     word ptr [bx+2],ax
        sub     ax,10h
        sar     ax,cl
        stosw
        lodsw
        mov     bx,ax
        mov     ax,word ptr [bx+2]
        add     ax,word ptr [bx+8]
        mov     word ptr [bx+2],ax
        add     ax,10h
        sar     ax,cl
        stosw
        dec     dh
        jne     L27F0
        mov     word ptr ds:[1B86h],di
        dec     bp
        mov     word ptr ds:[1B84h],bp
        test    byte ptr [bp+1B8Ah],cl
        je      L27AE
        jmp     L2606
L282D:
        mov     si,2D42h
        mov     bx,word ptr ds:[1B86h]
        mov     word ptr [bx],0FFFFh
        jmp     word ptr ds:[4112h]
L283C:
        retn
_seg003_0272_21D4 endp

; seg003_0272_283D  (+283D)
_seg003_0272_283D proc near
        push    ds
        mov     ax,seg dseg062_62a6
        mov     ds,ax
        mov     ax,word ptr ds:[110h]
        pop     ds
        mov     word ptr ds:[36A2h],ax
        call    _seg003_0272_2B7A
        jae     L2850
        ret
L2850:
        mov     word ptr ds:[36A4h],0FFFFh
        call    _SetVideoMode_seg003_0272_2B9B
        mov     dx,3D4h
        mov     al,9
        out     dx,al
        inc     dx
        in      al,dx
        mov     byte ptr ds:[2D35h],al
        call    _seg003_0272_2B44
        jmp     short L286C
L2869:
        mov     word ptr ds:[3DEAh],ax
L286C:
        mov     bx,word ptr ds:[3DEAh]
        cmp     bx,word ptr ds:[2CD8h]
        jne     L2877
        ret
L2877:
        mov     word ptr ds:[2CD8h],bx
        shl     bx,1
        mov     ax,bx
        shl     bx,1
        add     bx,ax
        lea     si,[bx+2CDEh]
        mov     di,3DECh
        movsw
        movsw
        movsw
        mov     si,3DEAh
        push    es
        mov     bx,seg dseg062_62a6
        mov     es,bx
        mov     di,0C38h
        movsw
        movsw
        movsw
        pop     es
        test    ax,ax
        jne     L28AE
        mov     dx,3D4h
        mov     al,9
        out     dx,al
        inc     dx
        mov     al,byte ptr ds:[2D35h]
        out     dx,al
        jmp     short L28B9
L28AE:
        mov     dx,3D4h
        mov     al,9
        out     dx,al
        inc     dx
        in      al,dx
        and     al,0E0h
        out     dx,al
L28B9:
        sub     ax,ax
        mov     word ptr ds:[3DF4h],ax
        mov     word ptr ds:[3DFAh],ax
        mov     ax,word ptr ds:[3DECh]
        mov     word ptr ds:[3DF8h],ax
        mov     ax,word ptr ds:[3DEEh]
        mov     word ptr ds:[3DF6h],ax
        mov     si,3DF4h
        call    _seg003_0272_31E7
        mov     ax,word ptr ds:[3DECh]
        inc     ax
        shr     ax,1
        shr     ax,1
        mov     word ptr ds:[36A8h],ax
        mov     ax,word ptr ds:[3DEEh]
        inc     ax
        mul     word ptr ds:[36A8h]
        mov     bx,ax
        add     ax,0FFh
        sub     al,al
        mov     word ptr ds:[2CD4h],ax
        add     ax,bx
        mov     word ptr ds:[2CD6h],ax
        add     ax,word ptr ds:[36A8h]
        mov     word ptr ds:[4106h],ax
        mov     word ptr ds:[410Ah],ax
        sub     ax,ax
        call    _seg003_0272_2AED
        call    _seg003_0272_31BE
        call    _seg003_0272_2ABE
        call    _nullsub_2
        mov     ax,0
        call    _seg003_0272_3BE2
        ret
_seg003_0272_283D endp

; seg003_0272_2914  (+2914)
_seg003_0272_2914 proc near
        cmp     word ptr ds:[3DFEh],0
        jne     L291C
        ret
L291C:
        mov     word ptr ds:[3DFEh],0
        mov     di,3E00h
        mov     cx,183h
        rep movsw
        mov     si,3E00h
L292D:
        cli
        mov     dx,3DAh
L2931:
        in      al,dx
        and     al,8
        je      L2931
        mov     al,0
        mov     dx,3C8h
        out     dx,al
        inc     dx
        mov     cx,100h
        call    _seg003_0272_2945
        sti
        ret
_seg003_0272_2914 endp

; seg003_0272_2945  (+2945)
_seg003_0272_2945 proc near
        cmp     word ptr ds:[36A2h],0BAh
        jl      L2956
        mov     ax,cx
        shl     ax,1
        add     cx,ax
        rep outsb
        ret
L2956:
        lodsb
        out     dx,al
        lodsb
        out     dx,al
        lodsb
        out     dx,al
        loop    L2956
        ret
_seg003_0272_2945 endp

; seg003_0272_295F  (+295F)
_seg003_0272_295F proc near
        mov     bx,word ptr ds:[3DEEh]
        shl     bx,1
        mov     ax,word ptr [bx+36ACh]
        mov     bx,ax
        mov     dx,3D4h
        mov     al,0Ch
        out     dx,ax
        mov     ah,bl
        inc     al
        out     dx,ax
        ret
_seg003_0272_295F endp

; seg003_0272_2977  (+2977)
_seg003_0272_2977 proc near
        shr     ax,1
        shr     ax,1
        mov     word ptr ds:[36A8h],ax
        mov     word ptr ds:[2CDCh],bx
        mov     word ptr ds:[2CDAh],cx
        mov     bx,cx
        shr     ax,1
        mov     ah,al
        mov     dx,3D4h
        mov     al,13h
        out     dx,al
        inc     dx
        mov     al,ah
        out     dx,al
        mov     dx,3C0h
        mov     al,30h
        out     dx,al
        inc     dx
        in      al,dx
        or      al,20h
        dec     dx
        out     dx,al
        mov     cx,bx
        mov     ax,bx
        or      bx,bx
        jge     L29B6
        mov     ax,3FFh
        xor     bx,bx
        xor     cx,cx
        call    _seg003_0272_2A8F
        jmp     short L29C8
L29B6:
        mov     ax,word ptr ds:[36A8h]
        mul     bx
        mov     cx,ax
        mov     ax,word ptr ds:[3DEEh]
        sub     ax,bx
        add     ax,ax
        dec     ax
        call    _seg003_0272_2A8F
L29C8:
        mov     dx,3DAh
L29CB:
        in      al,dx
        and     al,8
        je      L29CB
L29D0:
        in      al,dx
        and     al,8
        jne     L29D0
        mov     dx,3D4h
        mov     al,0Ch
        out     dx,al
        inc     dx
        mov     al,ch
        out     dx,al
        dec     dx
        mov     al,0Dh
        out     dx,al
        inc     dx
        mov     al,cl
        out     dx,al
        mov     ax,cx
        mov     si,ax
        mov     cx,bx
        mov     di,cx
        mov     bx,word ptr ds:[3DEEh]
        or      si,si
        je      L2A09
        add     ax,word ptr ds:[36A8h]
        call    _seg003_0272_2B2C
        xor     ax,ax
        mov     bx,di
        dec     bx
        xor     cx,cx
        call    _seg003_0272_2B2C
        ret
L2A09:
        call    _seg003_0272_2B2C
        ret
_seg003_0272_2977 endp

; seg003_0272_2A0D  (+2A0D)
_seg003_0272_2A0D proc near
        xchg    bx,ax
        inc     ax
        neg     ax
        add     ax,word ptr ds:[2CDCh]
        mov     dx,word ptr ds:[2CDAh]
        or      dx,dx
        jl      L2A1F
        add     ax,dx
L2A1F:
        mul     word ptr ds:[36A8h]
        push    bx
        shr     bx,1
        shr     bx,1
        add     ax,bx
        push    ax
        mov     dx,word ptr ds:[2CDAh]
        or      dx,dx
        jl      L2A4B
        mov     bx,word ptr ds:[3DEEh]
        mov     cx,dx
        inc     cx
        pop     ax
        add     ax,word ptr ds:[36A8h]
        push    ax
        call    _seg003_0272_2B2C
        mov     bx,word ptr ds:[2CDAh]
        xor     ax,ax
        jmp     short L2A4F
L2A4B:
        mov     bx,word ptr ds:[3DEEh]
L2A4F:
        xor     cx,cx
        call    _seg003_0272_2B2C
        mov     dx,3DAh
L2A57:
        in      al,dx
        and     al,8
        je      L2A57
L2A5C:
        in      al,dx
        and     al,8
        jne     L2A5C
        pop     bx
        mov     dx,3D4h
        mov     al,0Ch
        out     dx,al
        inc     dx
        mov     al,bh
        out     dx,al
        dec     dx
        mov     al,0Dh
        out     dx,al
        inc     dx
        mov     al,bl
        out     dx,al
        mov     dx,3DAh
L2A77:
        in      al,dx
        and     al,8
        jne     L2A77
L2A7C:
        in      al,dx
        and     al,8
        je      L2A7C
        mov     dx,3C0h
        mov     al,33h
        out     dx,al
        pop     ax
        and     ax,3
        shl     ax,1
        out     dx,al
        ret
_seg003_0272_2A0D endp

; seg003_0272_2A8F  (+2A8F)
_seg003_0272_2A8F proc near
        push    ax
        mov     dx,3D4h
        mov     al,18h
        out     dx,al
        inc     dx
        pop     ax
        out     dx,al
        dec     dx
        mov     al,7
        out     dx,al
        inc     dx
        in      al,dx
        test    ah,1
        je      L2AA8
        or      al,10h
        jmp     short L2AAA
L2AA8:
        and     al,0EFh
L2AAA:
        out     dx,al
        dec     dx
        mov     al,9
        out     dx,al
        inc     dx
        in      al,dx
        test    ah,2
        je      L2ABA
        or      al,40h
        jmp     short L2ABC
L2ABA:
        and     al,0BFh
L2ABC:
        out     dx,al
        ret
_seg003_0272_2A8F endp

; seg003_0272_2ABE  (+2ABE)
_seg003_0272_2ABE proc near
        call    _seg003_0272_295F
L2AC1:
        mov     si,36ACh
        mov     di,3840h
        mov     cx,word ptr ds:[3DEEh]
        inc     cx
        rep movsw
        mov     ax,word ptr ds:[2CD4h]
        mov     bx,word ptr ds:[3DEEh]
        shl     bx,1
        sub     ax,word ptr [bx+36ACh]
        mov     bx,ax
        shl     bx,1
        neg     bx
        add     bx,word ptr ds:[2CD4h]
        mov     word ptr ds:[410Ch],bx
        call    _seg003_0272_2AED
        ret
_seg003_0272_2ABE endp

; seg003_0272_2AED  (+2AED)
_seg003_0272_2AED proc near
        mov     di,word ptr ds:[3DEEh]
        shl     di,1
        add     di,36ACh
        mov     bx,word ptr ds:[36A8h]
        mov     cx,word ptr ds:[3DEEh]
        inc     cx
        std
L2B01:
        stosw
        add     ax,bx
        stosw
        add     ax,bx
        stosw
        add     ax,bx
        stosw
        add     ax,bx
        stosw
        add     ax,bx
        stosw
        add     ax,bx
        stosw
        add     ax,bx
        stosw
        add     ax,bx
        stosw
        add     ax,bx
        stosw
        add     ax,bx
        sub     cx,0Ah
        ja      L2B01
        cld
        mov     word ptr ds:[2D1Ch],0FFFFh
        ret
_seg003_0272_2AED endp

; seg003_0272_2B2C  (+2B2C)
_seg003_0272_2B2C proc near
        neg     cx
        add     cx,bx
        inc     cx
        shl     bx,1
        mov     dx,word ptr ds:[36A8h]
L2B37:
        mov     word ptr [bx+36ACh],ax
        add     ax,dx
        sub     bx,2
        dec     cx
        jne     L2B37
        ret
_seg003_0272_2B2C endp

; seg003_0272_2B44  (+2B44)
_seg003_0272_2B44 proc near
        mov     si,2D2Ch
        mov     di,3B64h
        call    _seg003_0272_2B53
        mov     si,2D31h
        mov     di,3CA8h
_seg003_0272_2B44 endp

; seg003_0272_2B53  (+2B53)
_seg003_0272_2B53 proc near
        mov     cx,50h
L2B56:
        push    cx
        push    si
        mov     cx,4
        rep movsb
        pop     si
        pop     cx
        loop    L2B56
        ret
_seg003_0272_2B53 endp

; seg003_0272_2B62  (+2B62)
_seg003_0272_2B62 proc near
        mov     word ptr ds:[39D0h],dx
        mov     di,39D2h
        mov     dx,cx
        dec     dx
        shl     dx,1
        shl     dx,1
        add     di,dx
        std
L2B73:
        stosw
        add     ax,bx
        loop    L2B73
        cld
        ret
_seg003_0272_2B62 endp

; seg003_0272_2B7A  (+2B7A)
_seg003_0272_2B7A proc near
        mov     ax,1A00h
        int     10h
        cmp     al,1Ah
        jne     L2B99
        cmp     bl,0Bh
        je      L2B99
        cmp     bl,0Ch
        je      L2B99
        cmp     bl,7
        je      L2B97
        cmp     bl,8
        jne     L2B99
L2B97:
        clc
        ret
L2B99:
        stc
        ret
_seg003_0272_2B7A endp

; SetVideoMode_seg003_0272_2B9B  (+2B9B)
_SetVideoMode_seg003_0272_2B9B proc near
        mov     ax,13h
        int     10h
        mov     dx,3C4h
        mov     al,4
        out     dx,al
        inc     dx
        in      al,dx
        and     al,0F7h
        or      al,4
        out     dx,al
        mov     dx,3CEh
        mov     al,5
        out     dx,al
        inc     dx
        in      al,dx
        and     al,0EFh
        out     dx,al
        dec     dx
        mov     al,6
        out     dx,al
        inc     dx
        in      al,dx
        and     al,0FDh
        out     dx,al
        mov     ax,0FF08h
        mov     dx,3CEh
        out     dx,ax
        mov     dx,3C4h
        mov     ax,0F02h
        out     dx,ax
        push    es
        mov     es,word ptr ds:[3DFCh]
        sub     di,di
        mov     ax,di
        mov     cx,8000h
        rep stosw
        pop     es
        mov     dx,3D4h
        mov     al,14h
        out     dx,al
        inc     dx
        in      al,dx
        and     al,0BFh
        out     dx,al
        dec     dx
        mov     al,17h
        out     dx,al
        inc     dx
        in      al,dx
        or      al,40h
        out     dx,al
        mov     dx,3C4h
        mov     al,2
        out     dx,al
        ret
_SetVideoMode_seg003_0272_2B9B endp

; seg003_0272_2BF9  (+2BF9)
_seg003_0272_2BF9 proc near
        mov     ax,word ptr ds:[3DF6h]
        shl     ax,1
        add     ax,36AEh
        mov     bx,ax
        mov     ax,word ptr ds:[2CD6h]
        xchg    ax,word ptr [bx]
        mov     ax,word ptr ds:[3DFAh]
        shl     ax,1
        add     ax,36AAh
        mov     bx,ax
        mov     ax,word ptr ds:[2CD6h]
        xchg    ax,word ptr [bx]
        mov     ax,3B64h
        mov     bx,word ptr ds:[3DF4h]
        dec     bx
        add     ax,bx
        mov     bp,ax
        and     bx,3
        mov     al,byte ptr [bx+2D2Dh]
        mov     byte ptr [bp],al
        mov     ax,3CA8h
        mov     bx,word ptr ds:[3DF8h]
        inc     bx
        add     ax,bx
        mov     bp,ax
        and     bx,3
        mov     al,byte ptr [bx+2D30h]
        mov     byte ptr [bp],al
        ret
_seg003_0272_2BF9 endp

; seg003_0272_2C44  (+2C44)
_seg003_0272_2C44 proc near
        mov     ax,3B64h
        mov     bx,word ptr ds:[3DF4h]
        dec     bx
        add     ax,bx
        mov     bp,ax
        and     bx,3
        mov     al,byte ptr [bx+2D2Ch]
        mov     byte ptr [bp],al
        mov     ax,3CA8h
        mov     bx,word ptr ds:[3DF8h]
        inc     bx
        add     ax,bx
        mov     bp,ax
        and     bx,3
        mov     al,byte ptr [bx+2D31h]
        mov     byte ptr [bp],al
        mov     bx,word ptr ds:[3DF6h]
        shl     bx,1
        mov     ax,word ptr [bx+36ACh]
        sub     ax,word ptr ds:[36A8h]
        mov     word ptr [bx+36AEh],ax
        mov     bx,word ptr ds:[3DFAh]
        shl     bx,1
        mov     ax,word ptr [bx+36ACh]
        add     ax,word ptr ds:[36A8h]
        mov     word ptr [bx+36AAh],ax
        ret
        ret
        ret
        ret
        ret
        ret
L2C9A:
        mov     ax,1803h
        mov     dx,3CEh
        out     dx,ax
        mov     dx,3C5h
        mov     bl,0Fh
        push    es
        mov     es,word ptr ds:[3DFCh]
        jmp     short L2CBA
L2CAD:
        add     di,word ptr [bp+36ACh]
        and     al,bh
        out     dx,al
        mov     al,bl
        test    byte ptr es:[di],al
        stosb
L2CBA:
        lodsw
        shl     ax,1
        jb      L2D01
        mov     cx,ax
        lodsw
        mov     di,ax
        lodsw
        mov     bp,ax
        mov     bh,byte ptr [bp+3CA8h]
        mov     al,byte ptr [di+3B64h]
        xchg    cx,bp
        sar     di,1
        sar     di,1
        sar     cx,1
        sar     cx,1
        sub     cx,di
        jle     L2CAD
        out     dx,al
        add     di,word ptr [bp+36ACh]
        mov     al,bl
        test    byte ptr es:[di],al
        stosb
        dec     cx
        jcxz    L2CF6
        mov     al,0Fh
        out     dx,al
        mov     al,bl
L2CF0:
        test    byte ptr es:[di],al
        stosb
        loop    L2CF0
L2CF6:
        mov     al,bh
        out     dx,al
        mov     al,bl
        test    byte ptr es:[di],al
        stosb
        jmp     L2CBA
L2D01:
        mov     ax,3
        mov     dx,3CEh
        out     dx,ax
        mov     al,8
        out     dx,al
        pop     es
        ret
L2D0D:
        push    si
        call    _seg003_0272_2D83
        pop     si
        jmp     short L2D19
L2D14:
        pop     es
        ret
        call    _nullsub_2
L2D19:
        mov     dx,3C5h
        mov     bl,byte ptr ds:[4111h]
        push    es
        mov     es,word ptr ds:[3DFCh]
        jmp     short L2D43
L2D27:
        shl     al,1
        shr     bh,1
        not     al
        not     bh
        xchg    al,bh
        add     di,cx
        neg     cx
        jmp     short L2D66
L2D37:
        jl      L2D27
        add     di,word ptr [bp+3840h]
        and     al,bh
        out     dx,al
        mov     al,bl
        stosb
L2D43:
        lodsw
        shl     ax,1
        jb      L2D14
        mov     cx,ax
        lodsw
        mov     di,ax
        lodsw
        mov     bp,ax
        mov     bh,byte ptr [bp+3CA8h]
        mov     al,byte ptr [di+3B64h]
        xchg    cx,bp
        sar     di,1
        sar     di,1
        sar     cx,1
        sar     cx,1
        sub     cx,di
        jle     L2D37
L2D66:
        out     dx,al
        add     di,word ptr [bp+3840h]
        mov     al,bl
        stosb
        dec     cx
        mov     al,0Fh
        out     dx,al
        mov     al,bl
        rep stosb
        mov     al,bh
        out     dx,al
        mov     al,bl
        stosb
        jmp     L2D43
L2D7E:
        pop     es
        ret
        call    _nullsub_2
_seg003_0272_2C44 endp

; seg003_0272_2D83  (+2D83)
_seg003_0272_2D83 proc near
        mov     dx,3C5h
        mov     bl,byte ptr ds:[4111h]
        push    es
        mov     es,word ptr ds:[3DFCh]
        jmp     short L2DAD
L2D91:
        shl     al,1
        shr     bh,1
        not     al
        not     bh
        xchg    al,bh
        add     di,cx
        neg     cx
        jmp     short L2DD0
L2DA1:
        jl      L2D91
        add     di,word ptr [bp+36ACh]
        and     al,bh
        out     dx,al
        mov     al,bl
        stosb
L2DAD:
        lodsw
        shl     ax,1
        jb      L2D7E
        mov     cx,ax
        lodsw
        mov     di,ax
        lodsw
        mov     bp,ax
        mov     bh,byte ptr [bp+3CA8h]
        mov     al,byte ptr [di+3B64h]
        xchg    cx,bp
        sar     di,1
        sar     di,1
        sar     cx,1
        sar     cx,1
        sub     cx,di
        jle     L2DA1
L2DD0:
        out     dx,al
        add     di,word ptr [bp+36ACh]
        mov     al,bl
        stosb
        dec     cx
        mov     al,0Fh
        out     dx,al
        mov     al,bl
        rep stosb
        mov     al,bh
        out     dx,al
        mov     al,bl
        stosb
        jmp     L2DAD
L2DE8:
        mov     al,0FFh
        mov     dx,3CFh
        out     dx,al
        pop     es
        ret
        call    _nullsub_2
L2DF3:
        mov     ax,0
        mov     dx,3CFh
        out     dx,al
        mov     dx,3C5h
        mov     al,0Fh
        out     dx,al
        mov     bx,si
        mov     di,word ptr ds:[4116h]
        push    es
        mov     es,word ptr ds:[3DFCh]
L2E0B:
        mov     bp,word ptr [bx]
        shl     bp,1
        jb      L2DE8
        mov     si,word ptr [bx+2]
        mov     cx,word ptr [bx+4]
        add     bx,6
        sar     si,1
        sar     si,1
        sar     cx,1
        sar     cx,1
        sub     cx,si
        inc     cx
        add     si,word ptr [bp+36ACh]
        rep movs byte ptr es:[di],byte ptr es:[si]
        jmp     L2E0B
L2E2E:
        mov     ax,0FFh
        mov     dx,3CFh
        out     dx,al
        pop     es
        ret
        call    _nullsub_2
L2E3A:
        mov     ax,0
        mov     dx,3CFh
        out     dx,al
        mov     dx,3C5h
        mov     al,0Fh
        out     dx,al
        mov     bx,si
        mov     dx,word ptr ds:[410Ch]
        push    es
        mov     es,word ptr ds:[3DFCh]
L2E52:
        mov     bp,word ptr [bx]
        shl     bp,1
        jb      L2E2E
        mov     si,word ptr [bx+2]
        mov     cx,word ptr [bx+4]
        add     bx,6
        sar     si,1
        sar     si,1
        sar     cx,1
        sar     cx,1
        sub     cx,si
        inc     cx
        add     si,word ptr [bp+36ACh]
        mov     di,si
        add     si,dx
        rep movs byte ptr es:[di],byte ptr es:[si]
        jmp     L2E52
L2E79:
        push    si
        call    _seg003_0272_2F18
        pop     si
        mov     ax,word ptr ds:[410Ch]
        add     ax,word ptr ds:[36ACh]
        sub     ax,word ptr ds:[3840h]
        mov     word ptr ds:[410Eh],ax
        jmp     short L2E9A
L2E8E:
        mov     ax,0FFh
        mov     dx,3CFh
        out     dx,al
        pop     es
        ret
        call    _nullsub_2
L2E9A:
        mov     ax,0
        mov     dx,3CFh
        out     dx,al
        mov     dx,3C5h
        mov     bx,si
        push    es
        mov     es,word ptr ds:[3DFCh]
        jmp     short L2ECC
L2EAD:
        shl     al,1
        shr     ah,1
        not     ax
        xchg    al,ah
        add     di,cx
        neg     cx
        jmp     short L2EF1
L2EBB:
        jl      L2EAD
        add     di,word ptr [si+3840h]
        mov     bp,word ptr ds:[410Eh]
        and     al,ah
        out     dx,al
        mov     al,byte ptr es:[bp+di]
        stosb
L2ECC:
        mov     si,word ptr [bx]
        shl     si,1
        jb      L2E8E
        mov     di,word ptr [bx+2]
        mov     bp,word ptr [bx+4]
        add     bx,6
        mov     al,byte ptr [di+3B64h]
        mov     ah,byte ptr [bp+3CA8h]
        sar     di,1
        sar     di,1
        mov     cx,bp
        sar     cx,1
        sar     cx,1
        sub     cx,di
        jle     L2EBB
L2EF1:
        add     di,word ptr [si+3840h]
        mov     si,di
        add     si,word ptr ds:[410Eh]
        out     dx,al
        movs    byte ptr es:[di],byte ptr es:[si]
        dec     cx
        mov     al,0Fh
        out     dx,al
        rep movs byte ptr es:[di],byte ptr es:[si]
        mov     al,ah
        out     dx,al
        movs    byte ptr es:[di],byte ptr es:[si]
        jmp     L2ECC
L2F0C:
        mov     ax,0FFh
        mov     dx,3CFh
        out     dx,al
        pop     es
        ret
        call    _nullsub_2
_seg003_0272_2D83 endp

; seg003_0272_2F18  (+2F18)
_seg003_0272_2F18 proc near
        mov     ax,0
        mov     dx,3CFh
        out     dx,al
        mov     dx,3C5h
        mov     bx,si
        push    es
        mov     es,word ptr ds:[3DFCh]
        jmp     short L2F4A
L2F2B:
        shl     al,1
        shr     ah,1
        not     ax
        xchg    al,ah
        add     di,cx
        neg     cx
        jmp     short L2F6F
L2F39:
        jl      L2F2B
        add     di,word ptr [si+36ACh]
        mov     bp,word ptr ds:[410Ch]
        and     al,ah
        out     dx,al
        mov     al,byte ptr es:[bp+di]
        stosb
L2F4A:
        mov     si,word ptr [bx]
        shl     si,1
        jb      L2F0C
        mov     di,word ptr [bx+2]
        mov     bp,word ptr [bx+4]
        add     bx,6
        mov     al,byte ptr [di+3B64h]
        mov     ah,byte ptr [bp+3CA8h]
        sar     di,1
        sar     di,1
        mov     cx,bp
        sar     cx,1
        sar     cx,1
        sub     cx,di
        jle     L2F39
L2F6F:
        add     di,word ptr [si+36ACh]
        mov     si,di
        add     si,word ptr ds:[410Ch]
        out     dx,al
        movs    byte ptr es:[di],byte ptr es:[si]
        dec     cx
        mov     al,0Fh
        out     dx,al
        rep movs byte ptr es:[di],byte ptr es:[si]
        mov     al,ah
        out     dx,al
        movs    byte ptr es:[di],byte ptr es:[si]
        jmp     L2F4A
L2F8A:
        mov     ax,0FFh
        mov     dx,3CFh
        out     dx,al
        pop     es
        ret
        call    _nullsub_2
L2F96:
        mov     ax,0
        mov     dx,3CFh
        out     dx,al
        mov     dx,3C5h
        mov     al,0Fh
        out     dx,al
        mov     bx,si
        mov     si,word ptr ds:[4116h]
        push    es
        mov     es,word ptr ds:[3DFCh]
L2FAE:
        mov     bp,word ptr [bx]
        shl     bp,1
        jb      L2F8A
        mov     di,word ptr [bx+2]
        mov     cx,word ptr [bx+4]
        add     bx,6
        sar     di,1
        sar     di,1
        sar     cx,1
        sar     cx,1
        sub     cx,di
        inc     cx
        add     di,word ptr [bp+36ACh]
        rep movs byte ptr es:[di],byte ptr es:[si]
        jmp     L2FAE
L2FD1:
        pop     es
        ret
        call    _nullsub_2
L2FD6:
        mov     dx,3C5h
        mov     al,0Fh
        out     dx,al
        mov     dx,3C4h
        mov     al,2
        out     dx,al
        inc     dx
        mov     bp,si
        mov     di,word ptr ds:[4116h]
        push    ds
        mov     ds,word ptr ds:[3DFCh]
L2FEE:
        mov     bx,word ptr [bp]
        shl     bx,1
        jb      L2FD1
        mov     si,word ptr [bp+2]
        mov     cx,word ptr [bp+4]
        add     bp,6
        sar     si,1
        sar     si,1
        sar     cx,1
        sar     cx,1
        sub     cx,si
        inc     cx
        add     si,word ptr ss:[bx+36ACh]
        mov     bx,cx
        mov     al,1
        out     dx,al
        rep movsb
        sub     si,bx
        mov     cx,bx
        shl     al,1
        out     dx,al
        rep movsb
        sub     si,bx
        mov     cx,bx
        shl     al,1
        out     dx,al
        rep movsb
        sub     si,bx
        mov     cx,bx
        shl     al,1
        out     dx,al
        rep movsb
        jmp     L2FEE
L3032:
        pop     es
        dec     dx
        mov     al,8
        out     dx,al
        ret
        call    _nullsub_2
L303B:
        mov     dx,3C5h
        mov     al,0Fh
        out     dx,al
        mov     dx,3CEh
        mov     al,4
        out     dx,al
        inc     dx
        mov     bp,si
        mov     si,word ptr ds:[4116h]
        push    ds
        mov     ds,word ptr ds:[3DFCh]
L3053:
        mov     bx,word ptr [bp]
        shl     bx,1
        jb      L3032
        mov     di,word ptr [bp+2]
        mov     cx,word ptr [bp+4]
        add     bp,6
        sar     di,1
        sar     di,1
        sar     cx,1
        sar     cx,1
        sub     cx,di
        inc     cx
        add     di,word ptr ss:[bx+36ACh]
        mov     bx,cx
        sub     ax,ax
        out     dx,al
        rep movsb
        sub     di,bx
        mov     cx,bx
        inc     ax
        out     dx,al
        rep movsb
        sub     di,bx
        mov     cx,bx
        inc     ax
        out     dx,al
        rep movsb
        sub     di,bx
        mov     cx,bx
        inc     ax
        out     dx,al
        rep movsb
        jmp     L3053
_seg003_0272_2F18 endp

; seg003_0272_3094  (+3094)
_seg003_0272_3094 proc near
        cmp     bx,word ptr ds:[3DF6h]
        jg      L30CE
        cmp     bx,word ptr ds:[3DFAh]
        jl      L30CE
        cmp     ax,word ptr ds:[3DF8h]
        jg      L30CE
        cmp     ax,word ptr ds:[3DF4h]
        jl      L30CE
L30AC:
        mov     dx,3C5h
        push    es
        mov     es,word ptr ds:[3DFCh]
        mov     di,ax
        mov     al,byte ptr [di+3B64h]
        and     al,byte ptr [di+3CA8h]
        shr     di,1
        shr     di,1
        shl     bx,1
        add     di,word ptr [bx+36ACh]
        out     dx,al
        mov     al,byte ptr ds:[4111h]
        stosb
        pop     es
L30CE:
        ret
L30CF:
        mov     ch,cl
        call    _seg003_0272_30E3
        cmp     al,ch
        ret
L30D7:
        mov     ch,cl
        call    _seg003_0272_30FB
        cmp     al,ch
        ret
L30DF:
        mov     ax,0FFFFh
        ret
_seg003_0272_3094 endp

; seg003_0272_30E3  (+30E3)
_seg003_0272_30E3 proc near
        cmp     bx,word ptr ds:[3DF6h]
        jg      L30DF
        cmp     bx,word ptr ds:[3DFAh]
        jl      L30DF
        cmp     ax,word ptr ds:[3DF8h]
        jg      L30DF
        cmp     ax,word ptr ds:[3DF4h]
        jl      L30DF
_seg003_0272_30E3 endp

; seg003_0272_30FB  (+30FB)
_seg003_0272_30FB proc near
        mov     di,ax
        mov     ah,al
        and     ah,3
        mov     al,4
        mov     dx,3CEh
        out     dx,ax
        push    es
        mov     es,word ptr ds:[3DFCh]
        shr     di,1
        shr     di,1
        shl     bx,1
        add     di,word ptr [bx+36ACh]
        mov     ah,byte ptr es:[di]
        pop     es
        mov     al,8
        out     dx,al
        mov     al,ah
        ret
_seg003_0272_30FB endp

; seg003_0272_3121  (+3121)
_seg003_0272_3121 proc near
        mov     di,ax
        mov     cx,bx
        sub     cx,dx
        inc     cx
        mov     ax,0
        mov     dx,3CFh
        out     dx,al
        mov     al,byte ptr [di+3B64h]
        and     al,byte ptr [di+3CA8h]
        mov     dx,3C5h
        out     dx,al
        shr     di,1
        shr     di,1
        shl     bx,1
        add     di,word ptr [bx+36ACh]
        mov     bx,word ptr ds:[410Ch]
        mov     dx,word ptr ds:[36A8h]
        dec     dx
        push    es
        mov     es,word ptr ds:[3DFCh]
L3153:
        mov     al,byte ptr es:[bx+di]
        stosb
        add     di,dx
        loop    L3153
        pop     es
        mov     ax,0FFh
        mov     dx,3CFh
        out     dx,al
        ret
_seg003_0272_3121 endp

; seg003_0272_3164  (+3164)
_seg003_0272_3164 proc near
        mov     di,ax
        mov     cx,bx
        sub     cx,dx
        inc     cx
        mov     al,byte ptr [di+3B64h]
        and     al,byte ptr [di+3CA8h]
        mov     dx,3C5h
        out     dx,al
        shr     di,1
        shr     di,1
        shl     bx,1
        add     di,word ptr [bx+36ACh]
        mov     bx,word ptr ds:[36A8h]
        dec     bx
        mov     al,byte ptr ds:[4111h]
        push    es
        mov     es,word ptr ds:[3DFCh]
L318E:
        stosb
        add     di,bx
        loop    L318E
        pop     es
        ret
_seg003_0272_3164 endp

; seg003_0272_3195  (+3195)
_seg003_0272_3195 proc near
        shl     ax,1
        jb      L31BD
        mov     bx,ax
        mov     ax,word ptr [bx+21Eh]
        mov     word ptr ds:[4110h],ax
        mov     ax,word ptr [bx+434h]
        mov     word ptr ds:[4116h],ax
        db      8Bh,9Fh,8h,0h                   ; mov bx,word ptr [bx+8]: disp16, the table at seg_370D:0008 (TASM gives disp8 for a number, and a label there would need that segment)
        cmp     bx,14h
        jae     L31BD
        mov     ax,word ptr [bx+2CEEh]
        sub     ax,word ptr ds:[5046h]
        mov     word ptr ds:[4112h],ax
L31BD:
        ret
_seg003_0272_3195 endp

; seg003_0272_31BE  (+31BE)
_seg003_0272_31BE proc near
        mov     di,0FFFFh
        call    _nullsub_1
        mov     cx,21Eh
        db      81h,0E9h,8h,0h                  ; sub cx,8 with an imm16: the offset of the same table, which TASM gives as 83 for a number
        shr     cx,1
        sub     bp,bp
L31CF:
        db      8Bh,9Eh,8h,0h                   ; mov bx,word ptr [bp+8]: disp16, the table at seg_370D:0008 (TASM gives disp8 for a number, and a label there would need that segment)
        cmp     bx,14h
        jae     L31DC
        call    word ptr [bx+2D04h]
L31DC:
        inc     bp
        inc     bp
        loop    L31CF
        mov     word ptr ds:[4108h],di
        ret
_seg003_0272_31BE endp

; nullsub_1  (+31E5)
_nullsub_1 proc near
        ret
        ret
_nullsub_1 endp

; seg003_0272_31E7  (+31E7)
_seg003_0272_31E7 proc near
        call    _seg003_0272_2C44
        push    si
        mov     di,3DF4h
        movsw
        movsw
        movsw
        movsw
        pop     si
        call    _seg003_0272_2BF9
        push    es
        mov     ax,seg dseg062_62a6
        mov     es,ax
        mov     di,0C30h
        movsw
        movsw
        movsw
        movsw
        pop     es
        ret
        db      0                       ; zero pad to a word boundary (see the header)
_seg003_0272_31E7 endp

; seg003_0272_3206  (+3206)
_seg003_0272_3206 proc near
        mov     ax,word ptr ds:[410Ah]
        add     bx,ax
        jb      L3219
        cmp     bx,word ptr ds:[4108h]
        jae     L3219
        mov     word ptr ds:[410Ah],bx
        clc
        ret
L3219:
        stc
        ret
_seg003_0272_3206 endp

; seg003_0272_321B  (+321B)
_seg003_0272_321B proc near
        cmp     ax,word ptr ds:[4106h]
        jae     L3223
        stc
        ret
L3223:
        mov     word ptr ds:[410Ah],ax
        clc
        ret
        push    word ptr ds:[4112h]
        mov     word ptr ds:[4112h],offset L52CC
        call    _seg003_0272_368E
        pop     word ptr ds:[4112h]
        ret
        push    word ptr ds:[4112h]
        mov     word ptr ds:[4112h],offset L52D2
        call    _seg003_0272_368E
        pop     word ptr ds:[4112h]
        ret
        push    word ptr ds:[4112h]
        mov     word ptr ds:[4112h],offset L52CF
        call    _seg003_0272_375C
        pop     word ptr ds:[4112h]
        ret
        push    ax
        call    _seg003_0272_5299
        pop     ax
        call    _seg003_0272_326A
        call    _seg003_0272_5299
        ret
_seg003_0272_321B endp

; seg003_0272_326A  (+326A)
_seg003_0272_326A proc near
        call    _seg003_0272_52B7
_seg003_0272_326A endp

; seg003_0272_326D  (+326D)
_seg003_0272_326D proc near
        sub     ax,ax
        mov     bx,word ptr ds:[3DEEh]
        mov     cx,word ptr ds:[3DECh]
        sub     dx,dx
        call    _seg003_0272_342E
        ret
_seg003_0272_326D endp

; seg003_0272_327D  (+327D)
_seg003_0272_327D proc near
        push    word ptr ds:[4112h]
        mov     word ptr ds:[4112h],offset L52CC
        call    _seg003_0272_326D
        pop     word ptr ds:[4112h]
        ret
_seg003_0272_327D endp

; seg003_0272_328F  (+328F)
_seg003_0272_328F proc near
        call    _seg003_0272_5299
        call    _seg003_0272_327D
        call    _seg003_0272_5299
        ret
L3299:
        retn
L329A:
        cmp     word ptr ds:[5046h],0
        je      L32B1
        shl     bx,1
        cmp     ax,word ptr [bx+5046h]
        jge     L32AF
        cmp     ax,word ptr [bx+5046h]
        jge     L3299
L32AF:
        shr     bx,1
L32B1:
        jmp     _seg003_0272_52A8
L32B4:
        cmp     bx,word ptr ds:[3DF6h]
        jg      L3299
        cmp     bx,word ptr ds:[3DFAh]
        jl      L3299
        cmp     ax,word ptr ds:[3DF8h]
        jg      L3299
        cmp     ax,word ptr ds:[3DF4h]
        jl      L3299
        cmp     word ptr ds:[4112h],offset L52C9
        je      L329A
        mov     di,414Ah
        mov     si,di
        xchg    bx,ax
        stosw
        xchg    bx,ax
        stosw
        stosw
        jmp     word ptr ds:[4112h]
_seg003_0272_328F endp

; seg003_0272_32E2  (+32E2)
_seg003_0272_32E2 proc near
        cmp     ax,word ptr ds:[3DF8h]
        jg      L3311
        cmp     ax,word ptr ds:[3DF4h]
        jl      L3311
        cmp     bx,dx
        jg      L32F4
        xchg    bx,dx
L32F4:
        mov     si,word ptr ds:[3DFAh]
        cmp     bx,si
        jl      L3311
        cmp     dx,si
        jg      L3302
        mov     dx,si
L3302:
        mov     si,word ptr ds:[3DF6h]
        cmp     dx,si
        jg      L3311
        cmp     bx,si
        jl      L3310
        mov     bx,si
L3310:
        ret
L3311:
        pop     ax
        ret
        call    _seg003_0272_32E2
        call    _seg003_0272_52B1
        ret
_seg003_0272_32E2 endp

; seg003_0272_331A  (+331A)
_seg003_0272_331A proc near
        call    _seg003_0272_32E2
        call    _seg003_0272_52B4
        ret
_seg003_0272_331A endp

; seg003_0272_3321  (+3321)
_seg003_0272_3321 proc near
        call    _seg003_0272_32E2
_seg003_0272_3321 endp

; seg003_0272_3324  (+3324)
_seg003_0272_3324 proc near
        cmp     word ptr ds:[5046h],0
        jne     L3341
        mov     si,word ptr ds:[4112h]
        cmp     si,offset L52C9
        jne     L3338
        jmp     _seg003_0272_52B1
L3338:
        cmp     si,offset L52CC
        jne     L3341
        jmp     _seg003_0272_52B4
L3341:
        cmp     bx,dx
        jg      L3347
        xchg    bx,dx
L3347:
        mov     cx,bx
        sub     cx,dx
        inc     cx
        mov     dx,ax
        mov     di,2D42h
L3351:
        mov     ax,bx
        stosw
        mov     ax,dx
        stosw
        stosw
        dec     bx
        loop    L3351
        mov     word ptr [di],0FFFFh
        mov     si,2D42h
        jmp     word ptr ds:[4112h]
        lodsw
        xchg    dx,ax
        lodsw
        xchg    bx,ax
        lodsw
        xchg    cx,ax
        lodsw
        xchg    dx,ax
_seg003_0272_3324 endp

; seg003_0272_336E  (+336E)
_seg003_0272_336E proc near
        call    _seg003_0272_33BE
_seg003_0272_336E endp

; seg003_0272_3371  (+3371)
_seg003_0272_3371 proc near
        mov     di,4152h
        stosw
        xchg    bx,ax
        stosw
        xchg    cx,ax
        stosw
        xchg    dx,ax
        stosw
L337B:
        mov     ax,word ptr ds:[4152h]
        mov     bx,word ptr ds:[4154h]
        mov     cx,word ptr ds:[4156h]
        call    _seg003_0272_34AE
        mov     ax,word ptr ds:[4152h]
        mov     bx,word ptr ds:[4158h]
        mov     cx,word ptr ds:[4156h]
        call    _seg003_0272_34AE
        mov     ax,word ptr ds:[4152h]
        mov     bx,word ptr ds:[4154h]
        mov     dx,word ptr ds:[4158h]
        call    _seg003_0272_3324
        mov     ax,word ptr ds:[4156h]
        mov     bx,word ptr ds:[4154h]
        mov     dx,word ptr ds:[4158h]
        call    _seg003_0272_3324
        ret
        mov     di,4152h
        mov     cx,4
        rep movsw
        jmp     L337B
_seg003_0272_3371 endp

; seg003_0272_33BE  (+33BE)
_seg003_0272_33BE proc near
        sub     bp,bp
        cmp     bx,dx
        jg      L33C6
        xchg    bx,dx
L33C6:
        cmp     ax,cx
        jl      L33CB
        xchg    cx,ax
L33CB:
        mov     si,word ptr ds:[3DF4h]
        cmp     cx,si
        jl      L3414
        cmp     ax,si
        jge     L33DC
        mov     ax,si
        add     bp,8
L33DC:
        mov     si,word ptr ds:[3DF8h]
        cmp     ax,si
        jg      L3414
        cmp     cx,si
        jle     L33ED
        mov     cx,si
        add     bp,2
L33ED:
        mov     si,word ptr ds:[3DFAh]
        cmp     bx,si
        jl      L3414
        cmp     dx,si
        jge     L33FE
        mov     dx,si
        add     bp,1
L33FE:
        mov     si,word ptr ds:[3DF6h]
        cmp     dx,si
        jg      L3414
        cmp     bx,si
        jle     L340F
        mov     bx,si
        add     bp,4
L340F:
        cmp     bp,0
        cmc
        ret
L3414:
        pop     ax
        ret
L3416:
        lodsw
        xchg    dx,ax
        lodsw
        xchg    bx,ax
        lodsw
        xchg    cx,ax
        lodsw
        xchg    dx,ax
_seg003_0272_33BE endp

; seg003_0272_341E  (+341E)
_seg003_0272_341E proc near
        call    _seg003_0272_33BE
        jmp     short _seg003_0272_342E
_seg003_0272_341E endp

; seg003_0272_3423  (+3423)
_seg003_0272_3423 proc near
        mov     si,3DF4h
        lodsw
        xchg    dx,ax
        lodsw
        xchg    bx,ax
        lodsw
        xchg    cx,ax
        lodsw
        xchg    dx,ax
_seg003_0272_3423 endp

; seg003_0272_342E  (+342E)
_seg003_0272_342E proc near
        cmp     bx,dx
        jg      L3434
        xchg    bx,dx
L3434:
        cmp     ax,cx
        jl      L3439
        xchg    cx,ax
L3439:
        mov     word ptr ds:[415Ah],ax
        mov     word ptr ds:[415Ch],cx
        mov     ax,bx
        mov     cx,dx
        neg     cx
        add     cx,ax
        inc     cx
        mov     si,word ptr ds:[49ACh]
        and     word ptr [si],7FFFh
        mov     di,44CEh
        mov     si,bx
        neg     si
        add     si,0C7h
        shl     si,1
        add     di,si
        shl     si,1
        add     di,si
        push    di
        mov     bx,415Ah
L3468:
        add     di,2
        mov     si,bx
        movsw
        movsw
        loop    L3468
        mov     word ptr ds:[49ACh],di
        or      word ptr [di],8000h
        pop     si
        jmp     word ptr ds:[4112h]
L347E:
        retn
_seg003_0272_342E endp

; seg003_0272_347F  (+347F)
_seg003_0272_347F proc near
        cmp     bx,word ptr ds:[3DF6h]
        jg      L347E
        cmp     bx,word ptr ds:[3DFAh]
        jl      L347E
        cmp     ax,cx
        jl      L3490
        xchg    cx,ax
L3490:
        mov     dx,word ptr ds:[3DF4h]
        cmp     cx,dx
        jl      L347E
        cmp     ax,dx
        jg      L349E
        mov     ax,dx
L349E:
        mov     dx,word ptr ds:[3DF8h]
        cmp     ax,dx
        jg      L347E
        cmp     cx,dx
        jl      L34AC
        mov     cx,dx
L34AC:
        jmp     short L34B3
_seg003_0272_347F endp

; seg003_0272_34AE  (+34AE)
_seg003_0272_34AE proc near
        cmp     ax,cx
        jl      L34B3
        xchg    cx,ax
L34B3:
        mov     di,414Ah
        mov     si,di
        xchg    bx,ax
        stosw
        xchg    bx,ax
        stosw
        xchg    cx,ax
        stosw
        jmp     word ptr ds:[4112h]
_seg003_0272_34AE endp

; seg003_0272_34C2  (+34C2)
_seg003_0272_34C2 proc near
        mov     si,449Ah
        jmp     short L34CA
_seg003_0272_34C2 endp

; seg003_0272_34C7  (+34C7)
_seg003_0272_34C7 proc near
        mov     si,4492h
L34CA:
        mov     di,448Ah
        movsw
        movsw
        movsw
        movsw
        mov     si,415Eh
        mov     di,cx
        shl     di,1
        shl     di,1
        add     di,si
        movsw
        movsw
        sub     si,4
        mov     bx,word ptr ds:[3DF6h]
        mov     dx,word ptr ds:[3DFAh]
        mov     si,4160h
        push    cx
L34ED:
        lodsw
        inc     si
        inc     si
        cmp     ax,bx
        jg      L34FA
        cmp     ax,dx
        jl      L34FA
        loop    L34ED
L34FA:
        test    cx,cx
        pop     cx
        je      L3535
        mov     word ptr ds:[4482h],2
        mov     word ptr ds:[4484h],8
        mov     word ptr ds:[4486h],offset L364C
        mov     si,415Eh
        mov     di,42EEh
        mov     al,7Ch
        mov     dx,word ptr ds:[3DF6h]
        call    L358F
        mov     word ptr ds:[4486h],offset L3648
        mov     si,42EEh
        mov     di,415Eh
        mov     al,7Fh
        mov     dx,word ptr ds:[3DFAh]
        call    L358F
L3535:
        mov     bx,word ptr ds:[3DF8h]
        mov     dx,word ptr ds:[3DF4h]
        mov     si,415Eh
        push    cx
L3541:
        lodsw
        inc     si
        inc     si
        cmp     ax,bx
        jg      L354E
        cmp     ax,dx
        jl      L354E
        loop    L3541
L354E:
        test    cx,cx
        pop     cx
        je      L3589
        mov     word ptr ds:[4482h],0
        mov     word ptr ds:[4484h],6
        mov     word ptr ds:[4486h],offset L3611
        mov     si,415Eh
        mov     di,42EEh
        mov     al,7Fh
        mov     dx,word ptr ds:[3DF4h]
        call    L358F
        mov     word ptr ds:[4486h],offset L360D
        mov     si,42EEh
        mov     di,415Eh
        mov     al,7Ch
        mov     dx,word ptr ds:[3DF8h]
        call    L358F
L3589:
        jcxz    L358D
        ret
L358C:
        pop     ax
L358D:
        pop     ax
        ret
L358F:
        jcxz    L358C
        push    di
        mov     byte ptr cs:L35B0,al
        mov     byte ptr cs:L35BE,al
        mov     word ptr ds:[4480h],cx
        mov     word ptr ds:[447Eh],cx
        db      0EAh                     ; far jmp to the next instruction, flushing the prefetch queue after the cs: stores above; TASM would shorten it
        dw      offset L35A7
        dw      seg L35A7
L35A7:
        sub     bx,bx
        add     si,word ptr ds:[4482h]
        lodsw
        cmp     ax,dx
L35B0:                                  ; its opcode is patched above
        jg      L35B5
        mov     bx,4
L35B5:
        inc     si
        inc     si
        lodsw
        sub     si,word ptr ds:[4484h]
        cmp     ax,dx
L35BE:
        jg      L35C3
        or      bx,2
L35C3:
        call    word ptr [bx+448Ah]
        dec     word ptr ds:[4480h]
        jg      L35A7
        mov     cx,word ptr ds:[447Eh]
        pop     si
        movsw
        movsw
        ret
        movsw
        movsw
        sub     si,4
        inc     word ptr ds:[447Eh]
        mov     word ptr ds:[4488h],0
        jmp     word ptr ds:[4486h]
        movsw
        movsw
        sub     si,4
        inc     word ptr ds:[447Eh]
        mov     word ptr ds:[4488h],0
        jmp     word ptr ds:[4486h]
        mov     word ptr ds:[4488h],1
        jmp     word ptr ds:[4486h]
        dec     word ptr ds:[447Eh]
        add     si,4
        ret
L360D:
        neg     word ptr ds:[4488h]
L3611:
        push    dx
        mov     ax,dx
        stosw
        lodsw
        xchg    cx,ax
        lodsw
        xchg    bx,ax
        lodsw
        xchg    bp,ax
        lodsw
        sub     si,4
        sub     ax,bx
        sub     dx,cx
        neg     cx
        add     cx,bp
        jo      L3642
L3629:
        imul    dx
        idiv    cx
        add     ax,bx
        cmp     ax,word ptr ds:[3DF6h]
        jge     L363F
        cmp     ax,word ptr ds:[3DFAh]
        jle     L363F
        add     ax,word ptr ds:[4488h]
L363F:
        stosw
        pop     dx
        ret
L3642:
        rcr     cx,1
        sar     dx,1
        jmp     L3629
L3648:
        neg     word ptr ds:[4488h]
L364C:
        push    dx
        lodsw
        xchg    cx,ax
        lodsw
        xchg    bx,ax
        lodsw
        xchg    bp,ax
        lodsw
        sub     si,4
        xchg    bp,ax
        xchg    bx,cx
        sub     ax,bx
        sub     dx,cx
        neg     cx
        add     cx,bp
        jo      L367F
L3664:
        imul    dx
        idiv    cx
        add     ax,bx
        cmp     ax,word ptr ds:[3DF4h]
        jle     L367A
        cmp     ax,word ptr ds:[3DF8h]
        jge     L367A
        add     ax,word ptr ds:[4488h]
L367A:
        stosw
        pop     ax
        stosw
        xchg    dx,ax
        ret
L367F:
        rcr     cx,1
        sar     dx,1
        jmp     L3664
        db      0                       ; zero pad to a word boundary (see the header)
L3686:
        lodsw
        xchg    dx,ax
        lodsw
        xchg    bx,ax
        lodsw
        xchg    cx,ax
        lodsw
        xchg    dx,ax
_seg003_0272_34C7 endp

; seg003_0272_368E  (+368E)
_seg003_0272_368E proc near
        mov     si,cx
        sub     si,ax
        jne     L3697
        jmp     _seg003_0272_3321
L3697:
        mov     word ptr ds:[44A2h],si
        mov     si,dx
        sub     si,bx
        jne     L36A4
        jmp     _seg003_0272_347F
L36A4:
        mov     word ptr ds:[44A4h],si
        mov     di,ax
        mov     si,bx
        mov     bp,dx
        mov     bh,0
        cmp     cx,word ptr ds:[3DF4h]
        jge     L36B9
        or      bh,2
L36B9:
        cmp     cx,word ptr ds:[3DF8h]
        jle     L36C2
        or      bh,4
L36C2:
        cmp     bp,word ptr ds:[3DFAh]
        jge     L36CB
        or      bh,8
L36CB:
        cmp     bp,word ptr ds:[3DF6h]
        jle     L36D4
        or      bh,10h
L36D4:
        mov     al,0
        cmp     di,word ptr ds:[3DF4h]
        jge     L36DE
        or      al,2
L36DE:
        cmp     di,word ptr ds:[3DF8h]
        jle     L36E6
        or      al,4
L36E6:
        cmp     si,word ptr ds:[3DFAh]
        jge     L36EE
        or      al,8
L36EE:
        cmp     si,word ptr ds:[3DF6h]
        jle     L36F6
        or      al,10h
L36F6:
        mov     bl,al
        or      al,bh
        jne     L3706
        mov     ax,di
        mov     bx,si
        mov     dx,bp
        db      0E9h                    ; jmp _seg003_0272_375C: TASM would write EB xx 90
        dw      _seg003_0272_375C-$-2
L3705:
        ret
L3706:
        test    bl,bh
        jne     L3705
        test    bl,bl
        jne     L3714
        xchg    di,cx
        xchg    si,bp
        xchg    bl,bh
L3714:
        mov     byte ptr ds:[44C6h],bh
        mov     bh,0
        jmp     word ptr [bx+44A6h]
        mov     ax,word ptr ds:[3DF4h]
        jmp     short L3726
        mov     ax,word ptr ds:[3DF8h]
L3726:
        mov     bx,ax
        sub     ax,di
        imul    word ptr ds:[44A4h]
        idiv    word ptr ds:[44A2h]
        mov     di,bx
        add     si,ax
        mov     bh,byte ptr ds:[44C6h]
        jmp     L36D4
        mov     ax,word ptr ds:[3DFAh]
        jmp     short L3744
        mov     ax,word ptr ds:[3DF6h]
L3744:
        mov     bx,ax
        sub     ax,si
        imul    word ptr ds:[44A2h]
        idiv    word ptr ds:[44A4h]
        add     di,ax
        mov     si,bx
        mov     bh,byte ptr ds:[44C6h]
        jmp     L36D4
        db      0                       ; zero pad to a word boundary (see the header)
_seg003_0272_368E endp

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
        db      0                       ; zero pad to a word boundary (see the header)
_seg003_0272_381C endp

; seg003_0272_38B4  (+38B4)
_seg003_0272_38B4 proc near
        mov     si,word ptr ds:[49ACh]
        and     word ptr [si],7FFFh
        mov     word ptr ds:[49B8h],cx
        mov     word ptr ds:[49BAh],cx
        mov     word ptr ds:[49BCh],cx
        mov     di,0FC19h
        mov     si,4160h
        mov     bp,415Eh
        jmp     short L38DC
L38D3:
        mov     bp,si
        sub     bp,6
        xchg    di,ax
        dec     cx
        je      L38E5
L38DC:
        lodsw
        inc     si
        inc     si
        cmp     di,ax
        jl      L38D3
        loop    L38DC
L38E5:
        sub     si,4
        mov     word ptr ds:[49C2h],si
        mov     si,bp
        mov     word ptr ds:[49C0h],si
        shl     di,1
        mov     ax,di
        shl     di,1
        add     di,ax
        neg     di
        add     di,4978h
        mov     word ptr ds:[49BEh],di
        call    _seg003_0272_395C
        mov     si,word ptr ds:[49C0h]
        mov     di,word ptr ds:[49BEh]
        call    _seg003_0272_3989
        sub     di,4
        mov     bp,word ptr ds:[49BEh]
        mov     ax,word ptr ds:[4A08h]
        mov     bx,word ptr ds:[4A0Ah]
        cmp     bp,di
        jne     L3944
        mov     si,415Eh
        mov     bx,270Fh
        mov     dx,0D8F1h
        mov     cx,word ptr ds:[49B8h]
L3931:
        lodsw
        cmp     ax,bx
        jg      L3938
        mov     bx,ax
L3938:
        cmp     ax,dx
        jl      L393E
        mov     dx,ax
L393E:
        inc     si
        inc     si
        loop    L3931
        mov     ax,dx
L3944:
        mov     si,bp
        inc     di
        inc     di
        cmp     ax,bx
        jle     L394D
        xchg    bx,ax
L394D:
        stosw
        xchg    bx,ax
        stosw
        mov     word ptr ds:[49ACh],di
        or      word ptr [di],8000h
        jmp     word ptr ds:[4112h]
_seg003_0272_38B4 endp

; seg003_0272_395C  (+395C)
_seg003_0272_395C proc near
        inc     di
        inc     di
        inc     si
        inc     si
L3960:
        std
        lodsw
        xchg    bx,ax
        lodsw
        xchg    cx,ax
        cmp     si,4160h
        jae     L396F
        mov     si,word ptr ds:[49C2h]
L396F:
        lodsw
        xchg    dx,ax
        lodsw
        xchg    cx,ax
        add     si,4
        cld
        cmp     bx,dx
        jl      L3988
        mov     word ptr ds:[4A08h],cx
        call    _seg003_0272_39BB
        dec     word ptr ds:[49BAh]
        jg      L3960
L3988:
        ret
_seg003_0272_395C endp

; seg003_0272_3989  (+3989)
_seg003_0272_3989 proc near
        add     di,4
L398C:
        lodsw
        xchg    dx,ax
        lodsw
        xchg    bx,ax
        cmp     si,word ptr ds:[49C2h]
        jb      L3999
        mov     si,415Eh
L3999:
        lodsw
        xchg    cx,ax
        lodsw
        xchg    dx,ax
        sub     si,4
        cmp     bx,dx
        jl      L39B1
        mov     word ptr ds:[4A0Ah],cx
        call    _seg003_0272_39BB
        dec     word ptr ds:[49BCh]
        jg      L398C
L39B1:
        ret
L39B2:
        sub     bx,bx
        sub     cx,cx
        sub     dx,dx
        jmp     short L39E5
L39BA:
        retn
_seg003_0272_3989 endp

; seg003_0272_39BB  (+39BB)
_seg003_0272_39BB proc near
        sub     bx,dx
        mov     word ptr ds:[4A06h],bx
        mov     bp,bx
        jle     L39BA
        stosw
        add     di,4
        sub     cx,ax
        je      L39B2
        xchg    cx,ax
        cwd
        idiv    bp
        xchg    bx,ax
        sub     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    bp
        cwd
        shl     ax,1
        rcl     dx,1
        add     bx,dx
        xchg    cx,ax
        mov     dx,7FFFh
L39E5:
        cmp     bp,20h
        ja      L39F0
        shl     bp,1
        jmp     word ptr [bp+49C4h]
L39F0:
        call    _seg003_0272_3A02
        sub     bp,1Fh
        cmp     bp,20h
        ja      L39F0
        shl     bp,1
        jmp     word ptr [bp+49C4h]
        even
_seg003_0272_39BB endp

; seg003_0272_3A02  (+3A02)
_seg003_0272_3A02 proc near
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        ret
L3AFB:
        jne     L3B03
        mov     si,415Eh
        jmp     L3686
L3B03:
        mov     ax,word ptr ds:[415Eh]
        mov     bx,word ptr ds:[4160h]
        jmp     L32B4
_seg003_0272_3A02 endp

; seg003_0272_3B0D  (+3B0D)
_seg003_0272_3B0D proc near
        cmp     cx,63h
        ja      L3B1D
        cmp     cx,2
        jbe     L3AFB
        call    _seg003_0272_34C7
        jmp     _seg003_0272_38B4
L3B1D:
        ret
_seg003_0272_3B0D endp

; seg003_0272_3B1E  (+3B1E)
_seg003_0272_3B1E proc near
        mov     word ptr ds:[4D16h],offset L4126
        jmp     short L3B4A
L3B26:
        mov     word ptr ds:[4D16h],offset L4126
        jmp     short L3B83
L3B2E:
        mov     word ptr ds:[4D16h],offset _seg003_0272_3C61
        jmp     short L3B46
_seg003_0272_3B1E endp

; seg003_0272_3B36  (+3B36)
_seg003_0272_3B36 proc near
        mov     word ptr ds:[4D16h],offset _seg003_0272_3C61
        jmp     short L3B4A
L3B3E:
        mov     word ptr ds:[4D16h],offset _seg003_0272_3C61
        jmp     short L3B83
L3B46:
        mov     word ptr ds:[2D3Ah],dx
L3B4A:
        mov     word ptr ds:[4A0Eh],ax
        mov     word ptr ds:[4A10h],bx
        mov     ax,word ptr ds:[4A10h]
        cmp     ax,word ptr ds:[3DFAh]
        jl      L3B8E
        cmp     ax,word ptr ds:[3DF6h]
        jg      L3B8E
        sub     ax,word ptr ds:[4D0Eh]
        inc     ax
        mov     word ptr ds:[4A14h],ax
        cmp     ax,word ptr ds:[3DFAh]
        jl      L3B8E
        cmp     ax,word ptr ds:[3DF6h]
        jg      L3B8E
        mov     ax,word ptr ds:[4A0Eh]
        cmp     ax,word ptr ds:[3DF4h]
        jl      L3B8E
        cmp     ax,word ptr ds:[3DF8h]
        jg      L3B8E
L3B83:
        call    word ptr ds:[4D14h]
        mov     ax,word ptr ds:[2D3Ah]
        call    word ptr ds:[4D16h]
L3B8E:
        ret
_seg003_0272_3B36 endp

; seg003_0272_3B8F  (+3B8F)
_seg003_0272_3B8F proc near
        mov     word ptr ds:[4A0Eh],ax
        mov     word ptr ds:[4A10h],bx
        mov     ax,word ptr ds:[4A10h]
        cmp     ax,word ptr ds:[3DFAh]
        jl      L3B8E
        cmp     ax,word ptr ds:[3DF6h]
        jg      L3B8E
        sub     ax,word ptr ds:[4D0Eh]
        inc     ax
        mov     word ptr ds:[4A14h],ax
        cmp     ax,word ptr ds:[3DFAh]
        jl      L3B8E
        cmp     ax,word ptr ds:[3DF6h]
        jg      L3B8E
        mov     ax,word ptr ds:[4A0Eh]
        cmp     ax,word ptr ds:[3DF4h]
        jl      L3B8E
        cmp     ax,word ptr ds:[3DF8h]
        jg      L3B8E
L3BC8:
        inc     word ptr ds:[4A0Eh]
        dec     word ptr ds:[4A10h]
        call    word ptr ds:[4D14h]
        mov     ax,word ptr ds:[2D36h]
        call    _seg003_0272_3C61
        call    _seg003_0272_4150
        mov     ax,word ptr ds:[2D3Ah]
        jmp     short _seg003_0272_3C61
_seg003_0272_3B8F endp

; seg003_0272_3BE2  (+3BE2)
_seg003_0272_3BE2 proc near
        mov     cx,10h
        mov     si,4A16h
        mov     di,4A26h
L3BEB:
        lodsb
        push    cx
        push    si
        mov     cx,10h
        mov     si,4A16h
L3BF4:
        stosb
        movsb
        loop    L3BF4
        pop     si
        pop     cx
        loop    L3BEB
        ret
_seg003_0272_3BE2 endp

; seg003_0272_3BFD  (+3BFD)
_seg003_0272_3BFD proc near
        mov     bx,word ptr ds:[4D0Eh]
        mov     cx,word ptr ds:[4D10h]
        dec     cl
        shl     bx,cl
        mov     cx,80h
        mov     ax,1C54h
        mov     di,4D18h
        add     bx,word ptr ds:[4D08h]
L3C16:
        stosw
        add     ax,bx
        loop    L3C16
        mov     ax,word ptr ds:[4D0Eh]
        mov     word ptr ds:[2D3Ch],ax
        mov     ax,word ptr ds:[4D10h]
        dec     ax
        mov     word ptr ds:[2D3Eh],ax
        mov     ax,word ptr ds:[4D12h]
        mov     word ptr ds:[2D40h],ax
        ret
        mov     bx,offset L52CC
L3C32:
        mov     word ptr ds:[4116h],ax
        mov     word ptr ds:[4112h],bx
        mov     word ptr ds:[4114h],0FFFFh
        jmp     L3416
        mov     bx,offset L52DB
        jmp     L3C32
        mov     bx,offset L52D8
        jmp     L3C32
L3C4C:
        mov     bx,offset L52E7
        jmp     L3C32
        push    word ptr [si+2]
        mov     ax,word ptr [si+6]
        mov     word ptr [si+2],ax
        call    L3C4C
        pop     word ptr [si+2]
        ret
_seg003_0272_3BFD endp

; seg003_0272_3C61  (+3C61)
_seg003_0272_3C61 proc near
        mov     word ptr ds:[4A0Ch],ax
        jmp     short L3C84
        mov     byte ptr ds:[4111h],ah
        mov     ax,word ptr ds:[4D04h]
        dec     ax
        shl     ax,1
        shl     ax,1
        shl     ax,1
        add     ax,word ptr ds:[4A0Eh]
        mov     word ptr ds:[4A12h],ax
        mov     bx,52C9h
        mov     si,4A0Eh
        call    L3C32
L3C84:
        mov     di,word ptr ds:[4A0Eh]
        shr     di,1
        shr     di,1
        mov     bp,word ptr ds:[4A10h]
        shl     bp,1
        add     di,word ptr [bp+36ACh]
        mov     bx,word ptr ds:[4D04h]
        shl     bx,1
        mov     bp,word ptr ds:[36A8h]
        sub     bp,bx
        mov     ax,word ptr [bx+4C8Eh]
        mov     word ptr ds:[4CFAh],ax
        mov     ch,byte ptr ds:[4D0Eh]
        mov     si,word ptr ds:[4D06h]
        push    es
        mov     es,word ptr ds:[3DFCh]
        mov     cl,byte ptr ds:[4A0Ch]
        mov     dx,3C5h
L3CBD:
        jmp     word ptr ds:[4CFAh]
        even
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        add     di,bp
        dec     ch
        je      L4124
        jmp     L3CBD
L4124:
        pop     es
        ret
L4126:
        mov     ax,seg seg_370D
        mov     si,ax
        mov     di,word ptr ds:[4D06h]
        mov     ax,word ptr ds:[4A0Eh]
        mov     bx,word ptr ds:[4A10h]
        mov     cx,word ptr ds:[4D04h]
        mov     dx,word ptr ds:[4D0Eh]
        jmp     L222B
L4141:
        popf
        call    _seg003_0272_4186
        pushf
        sub     bx,ax
        sub     si,ax
        cmp     bx,ax
        ja      L4141
        jmp     short L417B
_seg003_0272_3C61 endp

; seg003_0272_4150  (+4150)
_seg003_0272_4150 proc near
        clc
        pushf
        test    word ptr ds:[4A0Eh],3
        jne     L4163
        dec     word ptr ds:[4D06h]
        sub     word ptr ds:[4A0Eh],4
L4163:
        inc     word ptr ds:[4A10h]
        dec     word ptr ds:[4A0Eh]
        mov     ax,35h
        mov     bx,word ptr ds:[4D02h]
        inc     bx
        mov     si,word ptr ds:[4D00h]
        cmp     bx,ax
        ja      L4141
L417B:
        add     si,ax
        sub     si,bx
        shl     bx,1
        popf
        jmp     word ptr [bx+4C24h]
_seg003_0272_4150 endp

; seg003_0272_4186  (+4186)
_seg003_0272_4186 proc near
        rcl     byte ptr [si],1
        rcl     byte ptr [si-1],1
        rcl     byte ptr [si-2],1
        rcl     byte ptr [si-3],1
        rcl     byte ptr [si-4],1
        rcl     byte ptr [si-5],1
        rcl     byte ptr [si-6],1
        rcl     byte ptr [si-7],1
        rcl     byte ptr [si-8],1
        rcl     byte ptr [si-9],1
        rcl     byte ptr [si-0Ah],1
        rcl     byte ptr [si-0Bh],1
        rcl     byte ptr [si-0Ch],1
        rcl     byte ptr [si-0Dh],1
        rcl     byte ptr [si-0Eh],1
        rcl     byte ptr [si-0Fh],1
        rcl     byte ptr [si-10h],1
        rcl     byte ptr [si-11h],1
        rcl     byte ptr [si-12h],1
        rcl     byte ptr [si-13h],1
        rcl     byte ptr [si-14h],1
        rcl     byte ptr [si-15h],1
        rcl     byte ptr [si-16h],1
        rcl     byte ptr [si-17h],1
        rcl     byte ptr [si-18h],1
        rcl     byte ptr [si-19h],1
        rcl     byte ptr [si-1Ah],1
        rcl     byte ptr [si-1Bh],1
        rcl     byte ptr [si-1Ch],1
        rcl     byte ptr [si-1Dh],1
        rcl     byte ptr [si-1Eh],1
        rcl     byte ptr [si-1Fh],1
        rcl     byte ptr [si-20h],1
        rcl     byte ptr [si-21h],1
        rcl     byte ptr [si-22h],1
        rcl     byte ptr [si-23h],1
        rcl     byte ptr [si-24h],1
        rcl     byte ptr [si-25h],1
        rcl     byte ptr [si-26h],1
        rcl     byte ptr [si-27h],1
        rcl     byte ptr [si-28h],1
        rcl     byte ptr [si-29h],1
        rcl     byte ptr [si-2Ah],1
        rcl     byte ptr [si-2Bh],1
        rcl     byte ptr [si-2Ch],1
        rcl     byte ptr [si-2Dh],1
        rcl     byte ptr [si-2Eh],1
        rcl     byte ptr [si-2Fh],1
        rcl     byte ptr [si-30h],1
        rcl     byte ptr [si-31h],1
        rcl     byte ptr [si-32h],1
        rcl     byte ptr [si-33h],1
        rcl     byte ptr [si-34h],1
        ret
        mov     di,si
        call    _seg003_0272_43C5
        mov     si,di
        mov     ax,word ptr ds:[4A0Eh]
        and     ax,3
        add     dx,ax
        shr     dx,1
        shr     dx,1
        shr     dx,1
        mov     cx,dx
        inc     cx
        mov     word ptr ds:[4D04h],cx
        mov     bp,cx
        mov     ax,word ptr ds:[4D0Eh]
        mul     cx
        mov     cx,ax
        inc     cx
        mov     word ptr ds:[4D02h],cx
        mov     di,2D43h
        mov     dx,di
        inc     dx
        mov     word ptr ds:[4D06h],dx
        add     ax,dx
        mov     word ptr ds:[4D00h],ax
        sub     ax,ax
        inc     cx
        shr     cx,1
        rep stosw
        mov     bx,word ptr ds:[4D0Eh]
        shl     bx,1
        mov     ax,word ptr cs:L43A5[bx-2]
        mov     word ptr ds:[4CFAh],ax
        mov     bx,si
        mov     cx,word ptr ds:[4A0Eh]
        and     cx,3
        jmp     short L42A1
L427E:
        cmp     al,8
        jle     L4291
        cmp     ch,8
        jne     L428D
        sub     al,ch
        xor     ch,ch
        jmp     short L4291
L428D:
        mov     al,8
        mov     ch,al
L4291:
        add     cl,al
        cmp     cl,8
        jb      L429C
        inc     dx
        sub     cl,8
L429C:
        cmp     al,ch
        je      L42A1
        inc     bx
L42A1:
        sub     ax,ax
        mov     al,byte ptr [bx]
        test    al,al
        jne     L42AC
        jmp     L43A4
L42AC:
        cmp     al,20h
        jl      L42B4
        cmp     al,7Ah
        jle     L42B6
L42B4:
        mov     al,3Fh
L42B6:
        mov     si,ax
        shl     si,1
        mov     si,word ptr [si+4D18h]
        cmp     ch,8
        jne     L42C4
        inc     si
L42C4:
        mov     di,dx
        jmp     word ptr ds:[4CFAh]
L42CA:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L42D7:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L42E4:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L42F1:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L42FE:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L430B:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4318:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4325:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4332:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L433F:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L434C:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4359:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4366:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4373:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4380:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L438D:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
        cmp     ch,8
        jne     L43A0
        dec     si
L43A0:
        lodsb
        jmp     L427E
L43A4:
        ret
; entry points into the unrolled loop above, by the count at DS:4D0E (1 to 16)
L43A5   dw      offset L438D, offset L4380, offset L4373, offset L4366
        dw      offset L4359, offset L434C, offset L433F, offset L4332
        dw      offset L4325, offset L4318, offset L430B, offset L42FE
        dw      offset L42F1, offset L42E4, offset L42D7, offset L42CA
_seg003_0272_4186 endp

; seg003_0272_43C5  (+43C5)
_seg003_0272_43C5 proc near
        mov     bx,word ptr ds:[2D3Ch]
        mov     cx,word ptr ds:[2D3Eh]
        shl     bx,cl
        mov     cx,36h
        sub     dx,dx
        sub     ax,ax
L43D6:
        lodsb
        test    al,al
        je      L43EC
        mov     bp,ax
        shl     bp,1
        mov     bp,word ptr [bp+4D18h]
        add     bp,bx
        mov     al,byte ptr [bp]
        add     dx,ax
        loop    L43D6
L43EC:
        mov     ax,dx
L43EE:
        ret
        db      0                       ; zero pad to a word boundary (see the header)
_seg003_0272_43C5 endp

L43F0   dw      0                       ; the caller's ss and sp
L43F2   dw      0

; nullsub_2  (+43F4)
_nullsub_2 proc near
        ret
_nullsub_2 endp

; seg003_0272_43F5  (+43F5): no caller found; switches to the private stack and calls
; through the vector at _seg003_0272_5278
_seg003_0272_43F5 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_5278
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_43F5 endp

; setup_font  (+4431)
_setup_font proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_527B
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_setup_font endp

; WriteTextToScreen_seg003_0272_446D  (+446D)
_string_to_screen proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        les     di,dword ptr [bp+6]
        mov     si,di
        mov     cx,84h
        xor     al,al
        repne scasb
        neg     cx
        add     cx,85h
        push    ds
        mov     di,es
        mov     ds,di
        mov     di,4FA8h
        mov     ax,seg seg_370D
        mov     es,ax
        rep movsb
        pop     ds
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+0Eh]
        mov     si,word ptr [bp+10h]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        mov     si,4FA8h
        call    _seg003_0272_527E
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_string_to_screen endp

; seg003_0272_44DC  (+44DC)
_seg003_0272_44DC proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        les     di,dword ptr [bp+6]
        mov     si,di
        mov     cx,84h
        xor     al,al
        repne scasb
        neg     cx
        add     cx,85h
        push    ds
        mov     di,es
        mov     ds,di
        mov     di,4FA8h
        mov     ax,seg seg_370D
        mov     es,ax
        rep movsb
        pop     ds
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+0Eh]
        mov     si,word ptr [bp+10h]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        mov     si,4FA8h
        call    _seg003_0272_5287
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_44DC endp

; WritingTextToScreen_seg003_0272_454B  (+454B)
_string_width proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        les     di,dword ptr [bp+6]
        mov     si,di
        mov     cx,84h
        xor     al,al
        repne scasb
        neg     cx
        add     cx,85h
        push    ds
        mov     di,es
        mov     ds,di
        mov     di,4FA8h
        mov     ax,seg seg_370D
        mov     es,ax
        rep movsb
        pop     ds
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+0Eh]
        mov     si,word ptr [bp+10h]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        mov     si,4FA8h
        call    _seg003_0272_43C5
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_string_width endp

; seg003_0272_45BA  (+45BA)
_seg003_0272_45BA proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        les     di,dword ptr [bp+6]
        mov     si,di
        mov     cx,84h
        xor     al,al
        repne scasb
        neg     cx
        add     cx,85h
        push    ds
        mov     di,es
        mov     ds,di
        mov     di,4FA8h
        mov     ax,seg seg_370D
        mov     es,ax
        rep movsb
        pop     ds
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+0Eh]
        mov     si,word ptr [bp+10h]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        mov     si,4FA8h
        call    _seg003_0272_528D
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_45BA endp

; init_graphics  (+4629)
_init_graphics proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_52BD
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_init_graphics endp

; seg003_0272_4665  (+4665)
_seg003_0272_4665 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     si,word ptr [bp+6]
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_5293
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4665 endp

; MaybeApplyPalette_seg003_0272_46AA  (+46AA)
_grPageFlip proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_5296
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_grPageFlip endp

; GraphicsCall_seg003_0272_46E6  (+46E6)
_grSoftPageFlip proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_5299
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_grSoftPageFlip endp

; seg003_0272_4722  (+4722)
_seg003_0272_4722 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_529C
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4722 endp

; seg003_0272_475E  (+475E)
_gr_read_pixel proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_529F
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_gr_read_pixel endp

; seg003_0272_47A0  (+47A0)
_seg003_0272_47A0 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_52A2
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_47A0 endp

; seg003_0272_47E2  (+47E2)
_plot_pixel proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_52A5
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_plot_pixel endp

; seg003_0272_4824  (+4824)
_seg003_0272_4824 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_52A8
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4824 endp

; seg003_0272_4866  (+4866)
_seg003_0272_4866 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        xchg    cx,dx
        call    _seg003_0272_52B1
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4866 endp

; seg003_0272_48AD  (+48AD)
_seg003_0272_48AD proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        xchg    cx,dx
        call    _seg003_0272_52B4
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_48AD endp

; seg003_0272_48F4  (+48F4)
_set_the_color proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_52B7
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_set_the_color endp

; init_colors  (+4933)
_init_colors proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_52BA
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_init_colors endp

; seg003_0272_496F  (+496F)
_seg003_0272_496F proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_52C0
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_496F endp

; seg003_0272_49AE  (+49AE)
_seg003_0272_49AE proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     bx,word ptr [bp+6]
        mov     ax,bx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_3206
        jae     L49DE
        mov     ax,0
L49DE:
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_49AE endp

; seg003_0272_49F4  (+49F4)
_seg003_0272_49F4 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     bx,word ptr [bp+6]
        mov     ax,bx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_321B
        jae     L4A24
        mov     ax,0
L4A24:
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_49F4 endp

; seg003_0272_4A3A  (+4A3A)
_seg003_0272_4A3A proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_2977
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4A3A endp

; seg003_0272_4A7F  (+4A7F)
_vscreen_focus proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_2A0D
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_vscreen_focus endp

; DrawBitmapScreen_seg003_0272_4AC1  (+4AC1)
_set_the_window proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     di,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     word ptr ds:[5040h],bx
        mov     word ptr ds:[503Eh],ax
        mov     word ptr ds:[5044h],di
        mov     word ptr ds:[5042h],cx
        mov     si,503Eh
        call    near ptr _seg003_0272_52E4
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_set_the_window endp

; seg003_0272_4B1B  (+4B1B)
_copy_hidden_to_visible proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_328F
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_copy_hidden_to_visible endp

; GraphicsCall_seg003_0272_4B57  (+4B57)
_copy_visible_to_hidden proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_327D
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_copy_visible_to_hidden endp

; seg003_0272_4B93  (+4B93)
_seg003_0272_4B93 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        xchg    cx,dx
        call    _seg003_0272_331A
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4B93 endp

; seg003_0272_4BDA  (+4BDA)
_seg003_0272_4BDA proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_347F
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4BDA endp

; seg003_0272_4C1F  (+4C1F)
_uhline proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_34AE
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_uhline endp

; seg003_0272_4C64  (+4C64)
_seg003_0272_4C64 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        xchg    cx,dx
        call    _seg003_0272_3321
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4C64 endp

; seg003_0272_4CAB  (+4CAB)
_uvline proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        xchg    cx,dx
        call    _seg003_0272_3324
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_uvline endp

; seg003_0272_4CF2  (+4CF2)
_rectangle proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        call    _seg003_0272_341E
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_rectangle endp

; seg003_0272_4D3C  (+4D3C)
_urectangle proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        call    _seg003_0272_342E
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_urectangle endp

; clear_window  (+4D86)
_clear_window proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_3423
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_clear_window endp

; box  (+4DC2)
_box proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        call    _seg003_0272_336E
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_box endp

; seg003_0272_4E0C  (+4E0C)
_seg003_0272_4E0C proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        call    _seg003_0272_3371
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4E0C endp

; seg003_0272_4E56  (+4E56)
_seg003_0272_4E56 proc far
        push    bp
        mov     bp,sp
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,word ptr [bp+0Ch]
        mov     bp,dx
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,bp
        push    word ptr ds:[4112h]
        mov     word ptr ds:[4112h],offset L52CC
        call    _seg003_0272_342E
        pop     word ptr ds:[4112h]
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4E56 endp

; seg003_0272_4EB0  (+4EB0)
_seg003_0272_4EB0 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        call    _seg003_0272_368E
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4EB0 endp

; seg003_0272_4EFA  (+4EFA)
_seg003_0272_4EFA proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     cx,word ptr [bp+6]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_3B0D
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4EFA endp

; seg003_0272_4F39  (+4F39)
_seg003_0272_4F39 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     cx,word ptr [bp+6]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_38B4
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4F39 endp

; seg003_0272_4F78  (+4F78)
_seg003_0272_4F78 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        call    _seg003_0272_375C
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4F78 endp

; DrawArtToScreen_seg003_0272_4FC2  (+4FC2)
_show proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,seg seg_370D
        mov     es,ax
        mov     ax,word ptr [bp+12h]
        mov     word ptr es:[0DC6h],ax
        mov     ax,word ptr [bp+14h]
        mov     word ptr es:[0DC8h],ax
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     di,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+10h]
        mov     bp,word ptr [bp+0Eh]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,bp
        call    _seg003_0272_21D4
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_show endp

; DRAW_RELATED_seg003_0272_5025  (+5025)
_seg003_0272_5025 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,seg seg_370D
        mov     es,ax
        mov     ax,word ptr [bp+10h]
        mov     word ptr es:[0DC6h],ax
        mov     ax,word ptr [bp+12h]
        mov     word ptr es:[0DC8h],ax
        mov     di,word ptr [bp+6]
        mov     si,0A000h
        mov     ax,word ptr [bp+8]
        mov     bx,word ptr [bp+0Ah]
        mov     cx,word ptr [bp+0Ch]
        mov     bp,word ptr [bp+0Eh]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,bp
        call    L21ED
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_5025 endp

; seg003_0272_5088  (+5088)
_fbshow proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,seg seg_370D
        mov     es,ax
        xor     ax,ax
        mov     word ptr es:[0DC6h],ax
        mov     word ptr es:[0DC8h],ax
        mov     di,word ptr [bp+6]
        mov     si,word ptr [bp+8]
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+0Eh]
        mov     bp,word ptr [bp+10h]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,bp
        call    L2214
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_fbshow endp

; seg003_0272_50E7  (+50E7)
_seg003_0272_50E7 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     di,word ptr [bp+8]
        mov     bx,word ptr [bp+0Ah]
        mov     cx,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,di
        call    _seg003_0272_2B62
        retf
_seg003_0272_50E7 endp

; seg003_0272_511C  (+511C)
_seg003_0272_511C proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,seg seg_370D
        mov     es,ax
        xor     ax,ax
        mov     word ptr es:[0DC6h],ax
        mov     word ptr es:[0DC8h],ax
        mov     di,word ptr [bp+6]
        mov     si,word ptr [bp+8]
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+0Eh]
        mov     bp,word ptr [bp+10h]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,bp
        call    L222B
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_511C endp

; seg003_0272_517B  (+517B)
_vcopyfb proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     di,word ptr [bp+0Eh]
        mov     bp,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,bp
        call    L2242
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_vcopyfb endp

; vcopy  (+51C8)
_vcopy proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Eh]
        mov     di,word ptr [bp+10h]
        mov     bp,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,bp
        call    L2250
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
L5218:
        lodsb
        stosb
        or      al,al
        jne     L5218
        dec     di
        retf
_vcopy endp

; seg003_0272_5220  (+5220)
_seg003_0272_5220 proc near
        mov     di,si
        xor     al,al
        mov     cx,0FFFFh
        repne scasb
        mov     cx,di
        sub     cx,si
        dec     cx
        ret
        push    si
        push    di
        call    _seg003_0272_5220
        pop     di
        repe cmpsb
        pop     si
        retf
_seg003_0272_5220 endp

; seg003_0272_5239  (+5239)
_fbuf_setcolor proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_B9B
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_fbuf_setcolor endp

; seg003_0272_5278  (+5278)
_seg003_0272_5278 proc near
        jmp     _seg003_0272_3BE2
_seg003_0272_5278 endp

; seg003_0272_527B  (+527B)
_seg003_0272_527B proc near
        jmp     _seg003_0272_3BFD
_seg003_0272_527B endp

; seg003_0272_527E  (+527E)
_seg003_0272_527E proc near
        jmp     _seg003_0272_3B36
        jmp     L3B3E
        jmp     L3B2E
_seg003_0272_527E endp

; seg003_0272_5287  (+5287)
_seg003_0272_5287 proc near
        jmp     _seg003_0272_3B8F
        jmp     L3BC8
_seg003_0272_5287 endp

; seg003_0272_528D  (+528D)
_seg003_0272_528D proc near
        jmp     _seg003_0272_3B1E
        jmp     L3B26
_seg003_0272_528D endp

; seg003_0272_5293  (+5293)
_seg003_0272_5293 proc near
        jmp     _seg003_0272_2914
_seg003_0272_5293 endp

; seg003_0272_5296  (+5296)
_seg003_0272_5296 proc near
        jmp     _seg003_0272_2ABE
_seg003_0272_5296 endp

; seg003_0272_5299  (+5299)
_seg003_0272_5299 proc near
        jmp     L2AC1
_seg003_0272_5299 endp

; seg003_0272_529C  (+529C)
_seg003_0272_529C proc near
        jmp     _seg003_0272_295F
_seg003_0272_529C endp

; seg003_0272_529F  (+529F)
_seg003_0272_529F proc near
        jmp     _seg003_0272_30FB
_seg003_0272_529F endp

; seg003_0272_52A2  (+52A2)
_seg003_0272_52A2 proc near
        jmp     _seg003_0272_30E3
_seg003_0272_52A2 endp

; seg003_0272_52A5  (+52A5)
_seg003_0272_52A5 proc near
        jmp     _seg003_0272_3094
_seg003_0272_52A5 endp

; seg003_0272_52A8  (+52A8)
_seg003_0272_52A8 proc near
        jmp     L30AC
        jmp     L30D7
        jmp     L30CF
_seg003_0272_52A8 endp

; seg003_0272_52B1  (+52B1)
_seg003_0272_52B1 proc near
        jmp     _seg003_0272_3164
_seg003_0272_52B1 endp

; seg003_0272_52B4  (+52B4)
_seg003_0272_52B4 proc near
        jmp     _seg003_0272_3121
_seg003_0272_52B4 endp

; seg003_0272_52B7  (+52B7)
_seg003_0272_52B7 proc near
        jmp     _seg003_0272_3195
_seg003_0272_52B7 endp

; seg003_0272_52BA  (+52BA)
_seg003_0272_52BA proc near
        jmp     _seg003_0272_31BE
_seg003_0272_52BA endp

; seg003_0272_52BD  (+52BD)
_seg003_0272_52BD proc near
        jmp     _seg003_0272_283D
_seg003_0272_52BD endp

; seg003_0272_52C0  (+52C0)
_seg003_0272_52C0 proc near
        jmp     L286C
        jmp     L2869
        jmp     L292D
L52C9:
        jmp     _seg003_0272_2D83
L52CC:
        jmp     _seg003_0272_2F18
L52CF:
        jmp     L2D0D
L52D2:
        jmp     L2E79
        jmp     L2E3A
L52D8:
        jmp     L303B
L52DB:
        jmp     L2FD6
        jmp     L2F96
        jmp     L2DF3
_seg003_0272_52C0 endp

; seg003_0272_52E4  (+52E4)
_seg003_0272_52E4 proc far
        jmp     _seg003_0272_31E7
L52E7:
        jmp     L2C9A
        jmp     L283C
        db      0                       ; zero pad to a word boundary (see the header)
L52EE:
        mov     di,558Eh
        push    ax
        push    es
        mov     ax,seg seg_370D
        mov     es,ax
        rep movsw
        pop     es
        pop     ax
        mov     si,558Eh
L52FF:
        push    ax
        push    bx
        mov     ax,ss
        mov     bx,seg seg_370D
        cmp     ax,bx
        pop     bx
        pop     ax
        jne     L533A
        int     2
        call    bp
        retf
_seg003_0272_52E4 endp

; seg003_0272_5311  (+5311)
_seg003_0272_5311 proc far
        push    ds
        push    es
        push    ax
        mov     ax,seg seg_370D
        mov     ds,ax
        mov     es,ax
        pop     ax
        mov     word ptr ds:[558Ah],sp
        cli
        mov     ss,word ptr ds:[558Ch]
        mov     sp,word ptr ds:[5588h]
        sti
        call    bp
        mov     bp,seg dseg062_62a6
        cli
        mov     ss,bp
        mov     sp,word ptr ds:[558Ah]
        sti
        pop     es
        pop     ds
        retf
L533A:
        push    ax
        mov     ax,seg seg_370D
        mov     ds,ax
        mov     es,ax
        pop     ax
        mov     word ptr ds:[558Ah],sp
        cli
        mov     ss,word ptr ds:[558Ch]
        mov     sp,word ptr ds:[5588h]
        sti
        call    bp
        mov     bp,seg dseg062_62a6
        cli
        mov     ss,bp
        mov     sp,word ptr ds:[558Ah]
        sti
        mov     ds,bp
        mov     es,bp
        retf
_seg003_0272_5311 endp

; seg003_0272_5363  (+5363)
_seg003_0272_5363 proc near
        mov     cx,4
        mov     bp,offset _seg003_0272_52E4
        jmp     L52EE
_seg003_0272_5363 endp

; seg003_0272_536B  (+536B)
_seg003_0272_536B proc far
        call    _seg003_0272_52B7
        retf
        db      0                       ; zero pad to a word boundary (see the header)
L5370:
        pop     es
        retn
L5372:
        mov     dx,3C5h
        push    es
        mov     es,word ptr ds:[3DFCh]
L537A:
        lodsw
        shl     ax,1
        jb      L5370
        mov     bp,ax
        mov     ax,word ptr [bp+36ACh]
        mov     word ptr ds:[55E2h],ax
        lodsw
        mov     bx,ax
        lodsw
        mov     word ptr ds:[55E6h],ax
        lodsw
        mov     cx,ax
        push    si
        push    ds
        mov     ds,word ptr ds:[55ECh]
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+3B64h]
        and     al,byte ptr [bp+3CA8h]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E2h]
        neg     bp
        add     bp,word ptr ss:[55E6h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L53CA
        xchg    bp,cx
L53C2:
        movsb
        add     si,3
        loop    L53C2
        xchg    bp,cx
L53CA:
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+3B64h]
        and     al,byte ptr [bp+3CA8h]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E2h]
        neg     bp
        add     bp,word ptr ss:[55E6h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L53FC
        xchg    bp,cx
L53F4:
        movsb
        add     si,3
        loop    L53F4
        xchg    bp,cx
L53FC:
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+3B64h]
        and     al,byte ptr [bp+3CA8h]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E2h]
        neg     bp
        add     bp,word ptr ss:[55E6h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L542E
        xchg    bp,cx
L5426:
        movsb
        add     si,3
        loop    L5426
        xchg    bp,cx
L542E:
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+3B64h]
        and     al,byte ptr [bp+3CA8h]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E2h]
        neg     bp
        add     bp,word ptr ss:[55E6h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L5460
        xchg    bp,cx
L5458:
        movsb
        add     si,3
        loop    L5458
        xchg    bp,cx
L5460:
        pop     ds
        pop     si
        jmp     L537A
L5465:
        pop     es
        retn
L5467:
        mov     dx,3C5h
        push    es
        mov     es,word ptr ds:[3DFCh]
L546F:
        lodsw
        shl     ax,1
        jb      L5465
        mov     bp,ax
        mov     ax,word ptr [bp+36ACh]
        mov     word ptr ds:[55E2h],ax
        lodsw
        mov     bx,ax
        lodsw
        mov     word ptr ds:[55E6h],ax
        lodsw
        mov     cx,ax
        xor     ah,ah
        push    si
        push    ds
        mov     ds,word ptr ds:[55ECh]
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+3B64h]
        and     al,byte ptr [bp+3CA8h]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E2h]
        neg     bp
        add     bp,word ptr ss:[55E6h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L54C8
        xchg    bp,cx
L54B9:
        lodsb
        add     si,3
        cmp     al,ah
        je      L54C3
        stosb
        dec     di
L54C3:
        inc     di
        loop    L54B9
        xchg    bp,cx
L54C8:
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+3B64h]
        and     al,byte ptr [bp+3CA8h]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E2h]
        neg     bp
        add     bp,word ptr ss:[55E6h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L5501
        xchg    bp,cx
L54F2:
        lodsb
        add     si,3
        cmp     al,ah
        je      L54FC
        stosb
        dec     di
L54FC:
        inc     di
        loop    L54F2
        xchg    bp,cx
L5501:
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+3B64h]
        and     al,byte ptr [bp+3CA8h]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E2h]
        neg     bp
        add     bp,word ptr ss:[55E6h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L553A
        xchg    bp,cx
L552B:
        lodsb
        add     si,3
        cmp     al,ah
        je      L5535
        stosb
        dec     di
L5535:
        inc     di
        loop    L552B
        xchg    bp,cx
L553A:
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+3B64h]
        and     al,byte ptr [bp+3CA8h]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E2h]
        neg     bp
        add     bp,word ptr ss:[55E6h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L5573
        xchg    bp,cx
L5564:
        lodsb
        add     si,3
        cmp     al,ah
        je      L556E
        stosb
        dec     di
L556E:
        inc     di
        loop    L5564
        xchg    bp,cx
L5573:
        pop     ds
        pop     si
        jmp     L546F
L5578:
        push    es
        mov     es,word ptr ds:[3DFCh]
        mov     bp,si
L557F:
        mov     bx,word ptr [bp]
        shl     bx,1
        jae     L5589
        jmp     L5682
L5589:
        mov     di,word ptr [bx+36ACh]
        mov     word ptr ds:[55E2h],di
        mov     bx,word ptr [bp+2]
        mov     di,word ptr [bp+4]
        mov     ax,word ptr [bp+6]
        add     bp,8
        push    bp
        mov     bp,ax
        sub     di,bx
        inc     di
        mov     word ptr ds:[55E8h],di
        xor     ah,ah
        mov     dx,3CEh
        mov     al,4
        out     dx,al
        mov     al,byte ptr [bx+3B64h]
        and     al,byte ptr [bx+3CA8h]
        mov     dx,3C5h
        out     dx,al
        mov     al,ah
        mov     dx,3CFh
        out     dx,al
        mov     cx,word ptr ds:[55E8h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E2h]
        rep movs byte ptr es:[di],byte ptr es:[si]
        inc     bx
        inc     ah
        dec     word ptr ds:[55E8h]
        jne     L55E5
        jmp     L567E
L55E5:
        mov     al,byte ptr [bx+3B64h]
        and     al,byte ptr [bx+3CA8h]
        mov     dx,3C5h
        out     dx,al
        mov     al,ah
        mov     dx,3CFh
        out     dx,al
        mov     cx,word ptr ds:[55E8h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E2h]
        rep movs byte ptr es:[di],byte ptr es:[si]
        inc     bx
        inc     ah
        dec     word ptr ds:[55E8h]
        je      L567E
        mov     al,byte ptr [bx+3B64h]
        and     al,byte ptr [bx+3CA8h]
        mov     dx,3C5h
        out     dx,al
        mov     al,ah
        mov     dx,3CFh
        out     dx,al
        mov     cx,word ptr ds:[55E8h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E2h]
        rep movs byte ptr es:[di],byte ptr es:[si]
        inc     bx
        inc     ah
        dec     word ptr ds:[55E8h]
        je      L567E
        mov     al,byte ptr [bx+3B64h]
        and     al,byte ptr [bx+3CA8h]
        mov     dx,3C5h
        out     dx,al
        mov     al,ah
        mov     dx,3CFh
        out     dx,al
        mov     cx,word ptr ds:[55E8h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E2h]
        rep movs byte ptr es:[di],byte ptr es:[si]
        inc     bx
        inc     ah
        dec     word ptr ds:[55E8h]
        je      L567E
L567E:
        pop     bp
        jmp     L557F
L5682:
        mov     dx,3CEh
        mov     ax,0FF08h
        out     dx,ax
        pop     es
        retn
L568B:
        mov     dx,3CFh
        xor     al,al
        out     dx,al
        mov     dx,3C5h
        push    es
        mov     es,word ptr ds:[3DFCh]
        mov     bx,si
L569B:
        mov     bp,word ptr [bx]
        shl     bp,1
        jb      L56DB
        mov     di,word ptr [bp+36ACh]
        mov     ax,word ptr [bx+2]
        mov     cx,word ptr [bx+4]
        mov     bp,cx
        mov     si,word ptr [bx+6]
        add     bx,8
        sub     cx,ax
        inc     cx
        shr     ax,2
        add     di,ax
        mov     ax,cx
        shr     cx,2
        je      L56D2
        mov     al,0Fh
        out     dx,al
        rep movs byte ptr es:[di],byte ptr es:[si]
        mov     cx,bp
        and     cx,3
        sub     cx,3
        je      L569B
L56D2:
        mov     al,byte ptr [bp+3CA8h]
        out     dx,al
        movs    byte ptr es:[di],byte ptr es:[si]
        jmp     L569B
L56DB:
        pop     es
        mov     dx,3CFh
        mov     al,0FFh
        out     dx,al
        retn
L56E3:
        push    es
        mov     es,word ptr ds:[3DFCh]
L56E8:
        lodsw
        shl     ax,1
        jae     L56F0
        jmp     L5812
L56F0:
        mov     bx,ax
        mov     di,word ptr [bx+36ACh]
        mov     word ptr ds:[55E2h],di
        lodsw
        mov     bx,ax
        lodsw
        mov     di,ax
        lodsw
        mov     bp,ax
        sub     di,bx
        inc     di
        mov     word ptr ds:[55E8h],di
        push    si
        xor     ah,ah
        mov     dx,3CEh
        mov     al,4
        out     dx,al
        mov     al,byte ptr [bx+3B64h]
        and     al,byte ptr [bx+3CA8h]
        mov     dx,3C5h
        out     dx,al
        mov     al,ah
        mov     dx,3CFh
        out     dx,al
        mov     cx,word ptr ds:[55E8h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E2h]
L573A:
        lods    byte ptr es:[si]
        or      al,al
        je      L5742
        stosb
        dec     di
L5742:
        inc     di
        loop    L573A
        inc     bx
        inc     ah
        and     ah,3
        dec     word ptr ds:[55E8h]
        jne     L5754
        jmp     L580E
L5754:
        mov     al,byte ptr [bx+3B64h]
        and     al,byte ptr [bx+3CA8h]
        mov     dx,3C5h
        out     dx,al
        mov     al,ah
        mov     dx,3CFh
        out     dx,al
        mov     cx,word ptr ds:[55E8h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E2h]
L577B:
        lods    byte ptr es:[si]
        or      al,al
        je      L5783
        stosb
        dec     di
L5783:
        inc     di
        loop    L577B
        inc     bx
        inc     ah
        and     ah,3
        dec     word ptr ds:[55E8h]
        je      L580E
        mov     al,byte ptr [bx+3B64h]
        and     al,byte ptr [bx+3CA8h]
        mov     dx,3C5h
        out     dx,al
        mov     al,ah
        mov     dx,3CFh
        out     dx,al
        mov     cx,word ptr ds:[55E8h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E2h]
L57B9:
        lods    byte ptr es:[si]
        or      al,al
        je      L57C1
        stosb
        dec     di
L57C1:
        inc     di
        loop    L57B9
        inc     bx
        inc     ah
        and     ah,3
        dec     word ptr ds:[55E8h]
        je      L580E
        mov     al,byte ptr [bx+3B64h]
        and     al,byte ptr [bx+3CA8h]
        mov     dx,3C5h
        out     dx,al
        mov     al,ah
        mov     dx,3CFh
        out     dx,al
        mov     cx,word ptr ds:[55E8h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E2h]
L57F7:
        lods    byte ptr es:[si]
        or      al,al
        je      L57FF
        stosb
        dec     di
L57FF:
        inc     di
        loop    L57F7
        inc     bx
        inc     ah
        and     ah,3
        dec     word ptr ds:[55E8h]
        je      L580E
L580E:
        pop     si
        jmp     L56E8
L5812:
        mov     dx,3CEh
        mov     ax,0FF08h
        out     dx,ax
        pop     es
        retn
L581B:
        mov     dx,3CFh
        xor     al,al
        out     dx,al
        mov     dx,3C5h
        push    es
        mov     es,word ptr ds:[3DFCh]
        mov     bx,si
L582B:
        mov     bp,word ptr [bx]
        shl     bp,1
        jb      L5865
        mov     si,word ptr [bp+36ACh]
        mov     ax,word ptr [bx+2]
        mov     cx,word ptr [bx+4]
        mov     bp,cx
        mov     di,word ptr [bx+6]
        add     bx,8
        sub     cx,ax
        inc     cx
        shr     ax,2
        add     si,ax
        mov     ax,cx
        shr     cx,2
        je      L585C
        mov     al,0Fh
        out     dx,al
        rep movs byte ptr es:[di],byte ptr es:[si]
        mov     cx,bp
        je      L582B
L585C:
        mov     al,byte ptr [bp+3CA8h]
        out     dx,al
        movs    byte ptr es:[di],byte ptr es:[si]
        jmp     L582B
L5865:
        pop     es
        mov     dx,3CFh
_seg003_0272_536B endp

; seg003_0272_5869  (+5869)
_seg003_0272_5869 proc near
        mov     al,0FFh
        out     dx,al
        ret
L586D:
        mov     dx,3CFh
        xor     al,al
        out     dx,al
        mov     dx,3C5h
        push    es
        mov     es,word ptr ds:[3DFCh]
        mov     bx,si
        std
L587E:
        mov     bp,word ptr [bx]
        shl     bp,1
        jb      L58BE
        mov     si,word ptr [bp+36ACh]
        mov     ax,word ptr [bx+2]
        mov     cx,word ptr [bx+4]
        mov     di,word ptr [bx+6]
        add     bx,8
        sub     cx,ax
        inc     cx
        mov     bp,cx
        shr     ax,2
        add     si,ax
        shr     cx,2
        dec     cx
        add     si,cx
        add     di,cx
        test    bp,3
        je      L58B5
        mov     al,byte ptr [bp+3CA8h]
        out     dx,al
        movs    byte ptr es:[di],byte ptr es:[si]
        jcxz    L587E
L58B5:
        inc     cx
        mov     al,0Fh
        out     dx,al
        rep movs byte ptr es:[di],byte ptr es:[si]
        jmp     L587E
L58BE:
        cld
        pop     es
        mov     dx,3CFh
        mov     al,0FFh
        out     dx,al
        ret
L58C7:
        push    es
        push    ds
        mov     es,word ptr ss:[39D0h]
        mov     ds,word ptr ss:[55ECh]
L58D3:
        lods    word ptr ss:[si]
        shl     ax,1
        jb      L58F5
        mov     bx,ax
        mov     di,word ptr ss:[bx+39D2h]
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
        jmp     L58D3
L58F5:
        pop     ds
        pop     es
        ret
L58F8:
        push    es
        push    ds
        mov     es,word ptr ss:[39D0h]
        mov     ds,word ptr ss:[55ECh]
L5904:
        lods    word ptr ss:[si]
        shl     ax,1
        jb      L5930
        mov     bx,ax
        mov     di,word ptr ss:[bx+39D2h]
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
L5923:
        lodsb
        or      al,al
        je      L592A
        stosb
        dec     di
L592A:
        inc     di
        loop    L5923
        pop     si
        jmp     L5904
L5930:
        pop     ds
        pop     es
        ret
        db      0                       ; zero pad to a word boundary (see the header)
L5934:
        jne     L593C
        mov     si,415Eh
        jmp     L3686
L593C:
        mov     ax,word ptr ds:[415Eh]
        mov     bx,word ptr ds:[4160h]
        jmp     L32B4
        cmp     cx,63h
        ja      L5955
        cmp     cx,2
        jbe     L5934
        call    _seg003_0272_5B50
        jmp     short L5959
L5955:
        ret
        call    _seg003_0272_5D47
L5959:
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
        db      0                       ; zero pad to a word boundary (see the header)
_seg003_0272_5869 endp

; seg003_0272_5B50  (+5B50)
_seg003_0272_5B50 proc near
        call    _seg003_0272_5D47
        mov     si,415Eh
        mov     di,cx
        shl     di,1
        add     di,cx
        shl     di,1
        add     di,si
        movsw
        movsw
        movsw
        sub     si,6
        mov     bx,word ptr ds:[3DF6h]
        mov     dx,word ptr ds:[3DFAh]
        mov     si,4160h
        push    cx
L5B72:
        lodsw
        add     si,4
        cmp     ax,bx
        jg      L5B80
        cmp     ax,dx
        jl      L5B80
        loop    L5B72
L5B80:
        test    cx,cx
        pop     cx
        je      L5BBB
        mov     word ptr ds:[4482h],2
        mov     word ptr ds:[4484h],0Ah
        mov     word ptr ds:[4486h],offset L5CF0
        mov     si,415Eh
        mov     di,42EEh
        mov     al,7Ch
        mov     dx,word ptr ds:[3DF6h]
        call    L5C16
        mov     word ptr ds:[4486h],offset L5CEC
        mov     si,42EEh
        mov     di,415Eh
        mov     al,7Fh
        mov     dx,word ptr ds:[3DFAh]
        call    L5C16
L5BBB:
        mov     bx,word ptr ds:[3DF8h]
        mov     dx,word ptr ds:[3DF4h]
        mov     si,415Eh
        push    cx
L5BC7:
        lodsw
        add     si,4
        cmp     ax,bx
        jg      L5BD5
        cmp     ax,dx
        jl      L5BD5
        loop    L5BC7
L5BD5:
        test    cx,cx
        pop     cx
        je      L5C10
        mov     word ptr ds:[4482h],0
        mov     word ptr ds:[4484h],8
        mov     word ptr ds:[4486h],offset L5C9C
        mov     si,415Eh
        mov     di,42EEh
        mov     al,7Fh
        mov     dx,word ptr ds:[3DF4h]
        call    L5C16
        mov     word ptr ds:[4486h],offset L5C98
        mov     si,42EEh
        mov     di,415Eh
        mov     al,7Ch
        mov     dx,word ptr ds:[3DF8h]
        call    L5C16
L5C10:
        jcxz    L5C14
        ret
L5C13:
        pop     ax
L5C14:
        pop     ax
        ret
L5C16:
        jcxz    L5C13
        push    di
        mov     byte ptr cs:L5C37,al
        mov     byte ptr cs:L5C46,al
        mov     word ptr ds:[7D0h],cx
        mov     word ptr ds:[447Eh],cx
        db      0EAh                     ; far jmp to the next instruction, flushing the prefetch queue after the cs: stores above; TASM would shorten it
        dw      offset L5C2E
        dw      seg L5C2E
L5C2E:
        sub     bx,bx
        add     si,word ptr ds:[4482h]
        lodsw
        cmp     ax,dx
L5C37:
        jg      L5C3C
        mov     bx,4
L5C3C:
        add     si,4
        lodsw
        sub     si,word ptr ds:[4484h]
        cmp     ax,dx
L5C46:
        jg      L5C4B
        or      bx,2
L5C4B:
        call    word ptr [bx+5E76h]
        dec     word ptr ds:[7D0h]
        jg      L5C2E
        mov     cx,word ptr ds:[447Eh]
        pop     si
        movsw
        movsw
        movsw
        ret
        movsw
        movsw
        movsw
        sub     si,6
        inc     word ptr ds:[447Eh]
        mov     word ptr ds:[4488h],0
        jmp     word ptr ds:[4486h]
        movsw
        movsw
        movsw
        sub     si,6
        inc     word ptr ds:[447Eh]
        mov     word ptr ds:[4488h],0
        jmp     word ptr ds:[4486h]
        mov     word ptr ds:[4488h],1
        jmp     word ptr ds:[4486h]
        dec     word ptr ds:[447Eh]
        add     si,6
        ret
L5C98:
        neg     word ptr ds:[4488h]
L5C9C:
        push    dx
        mov     ax,dx
        stosw
        lodsw
        xchg    cx,ax
        lodsw
        xchg    bx,ax
        add     si,2
        lodsw
        xchg    bp,ax
        lodsw
        sub     si,4
        sub     ax,bx
        sub     dx,cx
        neg     cx
        add     cx,bp
        jo      L5CE4
L5CB7:
        push    dx
        push    cx
        imul    dx
        idiv    cx
        add     ax,bx
        cmp     ax,word ptr ds:[3DF6h]
        jge     L5CCF
        cmp     ax,word ptr ds:[3DFAh]
        jle     L5CCF
        add     ax,word ptr ds:[4488h]
L5CCF:
        stosw
        pop     cx
        pop     dx
        push    ax
        mov     ax,word ptr [si+4]
        sub     ax,word ptr [si-2]
        imul    dx
        idiv    cx
        add     ax,word ptr [si-2]
        stosw
        pop     ax
        pop     dx
        ret
L5CE4:
        rcr     cx,1
        sar     dx,1
        jmp     L5CB7
L5CEA   dw      0
L5CEC:
        neg     word ptr ds:[4488h]
L5CF0:
        mov     word ptr cs:L5CEA,dx
        lodsw
        xchg    cx,ax
        lodsw
        xchg    bx,ax
        add     si,2
        lodsw
        xchg    bp,ax
        lodsw
        sub     si,4
        xchg    bp,ax
        xchg    bx,cx
        sub     ax,bx
        sub     dx,cx
        neg     cx
        add     cx,bp
        jo      L5D41
L5D0F:
        push    dx
        push    cx
        imul    dx
        idiv    cx
        add     ax,bx
        cmp     ax,word ptr ds:[3DF4h]
        jle     L5D27
        cmp     ax,word ptr ds:[3DF8h]
        jge     L5D27
        add     ax,word ptr ds:[4488h]
L5D27:
        stosw
        mov     ax,word ptr cs:L5CEA
        stosw
        pop     cx
        pop     dx
        push    ax
        mov     ax,word ptr [si+4]
        sub     ax,word ptr [si-2]
        imul    dx
        idiv    cx
        add     ax,word ptr [si-2]
        stosw
        pop     ax
        xchg    dx,ax
        ret
L5D41:
        rcr     cx,1
        sar     dx,1
        jmp     L5D0F
_seg003_0272_5B50 endp

; seg003_0272_5D47  (+5D47)
_seg003_0272_5D47 proc near
        mov     si,415Eh
        mov     bx,55EEh
        mov     di,42EEh
        push    cx
L5D51:
        movsw
        movsw
        mov     ax,word ptr [bx]
        add     bx,2
        stosw
        loop    L5D51
        pop     cx
        push    cx
        mov     ax,cx
        shl     ax,1
        add     cx,ax
        mov     si,42EEh
        mov     di,415Eh
        rep movsw
        pop     cx
        ret
_seg003_0272_5D47 endp

        end
