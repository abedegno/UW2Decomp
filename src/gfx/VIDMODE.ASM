; /* target: seg003_0272_21D4 */
; /* opts: /ml */
;
; _21D4 .. _31E7: video mode setting (SetVideoMode_2B9B), palette and screen routines,
; nullsub_1.
;
; SEG003 split into its original modules: this is module 5 of 14, +21D4..+3206 of the segment
; (file 5624h), a module of the second library (UWLIB.LIB).
; Procs: seg003_0272_21D4, seg003_0272_283D, seg003_0272_2914, seg003_0272_2945,
; seg003_0272_295F, seg003_0272_2977, seg003_0272_2A0D, seg003_0272_2A8F, seg003_0272_2ABE,
; seg003_0272_2AED, seg003_0272_2B2C, seg003_0272_2B44, seg003_0272_2B53, seg003_0272_2B62,
; seg003_0272_2B7A, SetVideoMode_seg003_0272_2B9B, seg003_0272_2BF9, seg003_0272_2C44,
; seg003_0272_2D83, seg003_0272_2F18, seg003_0272_3094, seg003_0272_30E3, seg003_0272_30FB,
; seg003_0272_3121, seg003_0272_3164, seg003_0272_3195, seg003_0272_31BE, nullsub_1,
; seg003_0272_31E7.
; Evidence for the bounds: the boundary before it (see SCALEBM.ASM); it ends at the zero byte
; at 3205.
; Its relocations are entries 2473-2475 of UW2.EXE's relocation table.

_ShowClip       equ     0DC4h           ; seg_370D:0DC4, symbols.tsv
_Transparency   equ     0DC5h           ; seg_370D:0DC5, symbols.tsv

        .186

        extrn   dseg062_62a6:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'CODE'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _nullsub_2:near
        extrn   _seg003_0272_34C2:near
        extrn   _seg003_0272_3BE2:near
        extrn   _seg003_0272_5372:near
        extrn   _seg003_0272_5467:near
        extrn   _seg003_0272_5578:near
        extrn   _seg003_0272_568B:near
        extrn   _seg003_0272_56E3:near
        extrn   _seg003_0272_581B:near
        extrn   _seg003_0272_586D:near
        extrn   _seg003_0272_58C7:near
        extrn   _seg003_0272_58F8:near
        extrn   _seg003_0272_6F8:near
        extrn   _seg003_0272_729:near
        extrn   _seg003_0272_A7F:near
        public  _nullsub_1
        public  _seg003_0272_21D4
        public  _seg003_0272_21ED
        public  _seg003_0272_2214
        public  _seg003_0272_222B
        public  _seg003_0272_2242
        public  _seg003_0272_2250
        public  _seg003_0272_283C
        public  _seg003_0272_283D
        public  _seg003_0272_2869
        public  _seg003_0272_286C
        public  _seg003_0272_2914
        public  _seg003_0272_292D
        public  _seg003_0272_2945
        public  _seg003_0272_295F
        public  _seg003_0272_2977
        public  _seg003_0272_2A0D
        public  _seg003_0272_2A8F
        public  _seg003_0272_2ABE
        public  _seg003_0272_2AC1
        public  _seg003_0272_2AED
        public  _seg003_0272_2B2C
        public  _seg003_0272_2B44
        public  _seg003_0272_2B53
        public  _seg003_0272_2B62
        public  _seg003_0272_2B7A
        public  _seg003_0272_2BF9
        public  _seg003_0272_2C44
        public  _seg003_0272_2C9A
        public  _seg003_0272_2D0D
        public  _seg003_0272_2D83
        public  _seg003_0272_2DF3
        public  _seg003_0272_2E3A
        public  _seg003_0272_2E79
        public  _seg003_0272_2F18
        public  _seg003_0272_2F96
        public  _seg003_0272_2FD6
        public  _seg003_0272_303B
        public  _seg003_0272_3094
        public  _seg003_0272_30AC
        public  _seg003_0272_30CF
        public  _seg003_0272_30D7
        public  _seg003_0272_30E3
        public  _seg003_0272_30FB
        public  _seg003_0272_3121
        public  _seg003_0272_3164
        public  _seg003_0272_3195
        public  _seg003_0272_31BE
        public  _seg003_0272_31E7
        public  _SetVideoMode_seg003_0272_2B9B

; seg003_0272_21D4  (+21D4)
_seg003_0272_21D4 proc near
        cmp     byte ptr ds:[_Transparency],0
        jne     L21E4
        mov     word ptr ds:[0DC2h],offset _seg003_0272_5372
        jmp     L2296
L21E4:
        mov     word ptr ds:[0DC2h],offset _seg003_0272_5467
        jmp     L2296
_seg003_0272_21ED:
        cmp     byte ptr ds:[_Transparency],0
        jne     L220B
        test    ax,3
        je      L2202
        mov     word ptr ds:[0DC2h],offset _seg003_0272_5578
        jmp     L2296
L2202:
        mov     word ptr ds:[0DC2h],offset _seg003_0272_568B
        jmp     L2296
L220B:
        mov     word ptr ds:[0DC2h],offset _seg003_0272_56E3
        jmp     L2296
_seg003_0272_2214:
        cmp     byte ptr ds:[_Transparency],0
        jne     L2223
        mov     word ptr ds:[0DC2h],offset _seg003_0272_6F8
        jmp     short L2296
L2223:
        mov     word ptr ds:[0DC2h],offset _seg003_0272_729
        jmp     short L2296
_seg003_0272_222B:
        cmp     byte ptr ds:[_Transparency],0
        jne     L223A
        mov     word ptr ds:[0DC2h],offset _seg003_0272_58C7
        jmp     short L2296
L223A:
        mov     word ptr ds:[0DC2h],offset _seg003_0272_58F8
        jmp     short L2296
_seg003_0272_2242:
        mov     word ptr ds:[0DC2h],offset _seg003_0272_A7F
        mov     byte ptr ds:[0DCAh],0
        jmp     L2373
_seg003_0272_2250:
        cmp     bx,di
        je      L2266
        mov     word ptr ds:[0DC2h],offset _seg003_0272_581B
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
        mov     word ptr ds:[0DC2h],offset _seg003_0272_586D
        jmp     short L227B
L2275:
        mov     word ptr ds:[0DC2h],offset _seg003_0272_581B
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
_seg003_0272_283C:
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
        jmp     short _seg003_0272_286C
_seg003_0272_2869:
        mov     word ptr ds:[3DEAh],ax
_seg003_0272_286C:
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
_seg003_0272_292D:
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
_seg003_0272_2AC1:
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
_seg003_0272_2C9A:
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
_seg003_0272_2D0D:
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
_seg003_0272_2DF3:
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
_seg003_0272_2E3A:
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
_seg003_0272_2E79:
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
_seg003_0272_2F96:
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
_seg003_0272_2FD6:
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
_seg003_0272_303B:
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
_seg003_0272_30AC:
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
_seg003_0272_30CF:
        mov     ch,cl
        call    _seg003_0272_30E3
        cmp     al,ch
        ret
_seg003_0272_30D7:
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
_seg003_0272_31E7 endp

SEG003_TEXT ends

        end
