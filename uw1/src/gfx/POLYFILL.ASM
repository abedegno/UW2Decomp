; /* target: seg003_422 */
; /* opts: /ml */
;
; Graphics library module 4: a texture mapper for walls, FM Towns' wmap_bitmap with its edge
; steppers get_wright (_422) and get_wleft (_4B9). Its far entry is the unlabelled code at +545,
; after _4B9's ret; seg004 stores 0545h (or 01DAh, WALLMAP.ASM's entry, the older mapper) as
; its mapper routine and compares the word with 0545h. UW2Decomp found that UW2's seg004 stores
; this mapper's entry but never calls it; in UW1 nothing shows that either way.
;
; How it works, as far as read (UW2Decomp's reading): the polygon's vertices are 14-byte
; records at 3963:0659 (copied there by the caller; x at +0, y at +2, a texture coordinate at
; +6, the depth at +0Ah); the mapper first works out, for each screen column, which texture
; column it shows (a table of bytes at 7F2, filled by stepping across the polygon's top edge with
; a perspective divide by the depth), then walks the left and right edges down the screen (_4B9
; and _422) and draws each row through that table with xlat, the texture row stepped linearly.
; The steps are patched into the instructions (the 1234h immediates at L0830 .. L0885). The
; code-segment words _3B0 .. _3CE (MAPDATA.ASM) receive a copy of its inputs.
;
; Data (seg048): 7A9, 7AB the vertex list's last and end; 7B1 .. 7CD the two edges' current
; vertex, x and texture coordinate (16.16); 7CF the row; 7D1 the vertices left; 7D4 the
; segment half of this module's far pointer (3963), which DS is reloaded from; 7D8 .. 7F0 the
; column-table stepping; 932 the polygon's right x; 95A the screen segment; 95E the row offsets.
;
; name: descriptive (UW2Decomp's, module 2 of its seg003); its routines are FM Towns'
; wmap_bitmap, get_wright and get_wleft.
;
; UW1 against UW2: UW2Decomp's src/gfx/POLYFILL.ASM (UW2's seg003_0272_EC) with two additions in
; UW1 and without its last routine, which in UW1 is a module of its own (QUADFIT.ASM). The
; additions: an int 2 at L07A6, a break into a debugger where UW2 goes straight on, and, before
; the far return, a block that sets the colour byte 410F from 3963:0658 (0A0h when that is 0)
; when the pen-mode table at seg048:0008 has any byte set. One backward jmp (L08E0) is near
; where UW2's is short, the code between having grown. The data sits 1 or 2 bytes higher (7A9
; for UW2's 7A8, 932 for 930), and the texture record pointer is ES:[0B07Ch] (UW2 [0C7FEh]).
;
; SEG003 split into its original modules: this is module 4 of UW1's 18, +422..+8E6.
; Evidence for the bounds: it starts where MAPDATA's buffer ends (+422); its two code-segment
; words end at +8E7 and one zero byte follows (see QUADFIT.ASM).
; Its relocations are entries 2727-2728 of UW.EXE's relocation table (+553, +8AA). Its FD52 piece
; (3963:07D2..0958) holds entry 2726, the far pointer at 07D2, which comes right before the
; code's; it runs to the next module's first far pointer (GRENTRY's, 0958), as in UW2.

; The graphics library's data in seg048 (DS while the library runs), as GRLIBF.ASM's
; and VIDMODE.ASM's headers describe it. The names are ours.
PEN_COLOR       equ     410Fh   ; the colour

        .186

        extrn   seg048:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

FD52    segment byte public 'FAR_DATA'
        dd      seg048
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
FD52    ends

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg003_3B0:word
        extrn   _seg003_3B2:word
        extrn   _seg003_3B4:word
        extrn   _seg003_3B6:word
        extrn   _seg003_3B8:word
        extrn   _seg003_3BA:word
        extrn   _seg003_3BC:word
        extrn   _seg003_3BE:word
        extrn   _seg003_3C0:word
        extrn   _seg003_3C2:word
        extrn   _seg003_3C4:word
        extrn   _seg003_3C6:word
        extrn   _seg003_3C8:word
        extrn   _seg003_3CA:word
        extrn   _seg003_3CC:near
        extrn   _seg003_3CE:near
        public  _seg003_4B9
        public  _seg003_422
        public  _seg003_545

; seg003_422  (+422)
; _422 (get_wright, FM Towns): advance the right-hand edge to the next vertex (forwards through
; the list, wrapping at 7AB): load its x and texture coordinate, and patch the per-row steps of
; both (16.16, from two divides by the edge's height) into the drawing loop. Out: ZF set when no
; vertices are left. L043A is the re-entry used while drawing.
_seg003_422 proc near
        mov     bx,word ptr ds:[7B5h]
        mov     ax,word ptr [bx+6]
        mov     word ptr ds:[7C9h],ax
        mov     ax,word ptr [bx]
        mov     word ptr ds:[7C3h],ax
        mov     ax,8000h
        mov     word ptr ds:[7C5h],ax
        mov     word ptr ds:[7CDh],ax
L043A:
        add     bx,0Eh
        cmp     bx,word ptr ds:[7ABh]
        jne     L0446
        mov     bx,659h
L0446:
        mov     word ptr ds:[7B5h],bx
        mov     word ptr cs:_seg003_3BC,bx
        dec     word ptr ds:[7D1h]
        je      L04B8
        mov     ax,word ptr ds:[7D1h]
        mov     word ptr cs:_seg003_3B4,ax
        mov     ax,word ptr [bx+2]
        mov     word ptr cs:L0885,ax
        mov     cx,word ptr ds:[7CFh]
        sub     cx,ax
        je      _seg003_422
        mov     ax,word ptr [bx]
        sub     ax,word ptr ds:[7C3h]
        cwd
        idiv    cx
        mov     word ptr cs:L0877,ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr cs:L0871,ax
        add     word ptr cs:L0877,dx
        mov     ax,word ptr [bx+6]
        sub     ax,word ptr ds:[7C9h]
        cwd
        idiv    cx
        mov     word ptr cs:L086B,ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr cs:L0865,ax
        add     word ptr cs:L086B,dx
        test    word ptr ds:[7D1h],0FFFFh
L04B8:
        ret
_seg003_422 endp

; seg003_4B9  (+4B9)
; _4B9 (get_wleft, FM Towns): the same for the left-hand edge, backwards through the list. After
; its ret is wmap_bitmap (+545, far): save the caller's stack and switch to the library's, copy
; the vertex list to MAPDATA's _3CE, find the extremes, build the column table, then draw row by
; row until both edges run out, and return far on the caller's stack.
_seg003_4B9 proc near
        mov     bx,word ptr ds:[7B3h]
        mov     ax,word ptr [bx+6]
        mov     word ptr ds:[7BDh],ax
        mov     ax,word ptr [bx]
        mov     word ptr ds:[7B7h],ax
        mov     ax,8000h
        mov     word ptr ds:[7B9h],ax
        mov     word ptr ds:[7C1h],ax
L04D1:
        sub     bx,0Eh
        cmp     bx,64Bh
        jne     L04DE
        mov     bx,word ptr ds:[7A9h]
L04DE:
        mov     word ptr ds:[7B3h],bx
        dec     word ptr ds:[7D1h]
        je      L04B8
        mov     ax,word ptr [bx+2]
        mov     word ptr cs:L087F,ax
        mov     cx,word ptr ds:[7CFh]
        sub     cx,ax
        je      _seg003_4B9
        mov     ax,word ptr [bx]
        sub     ax,word ptr ds:[7B7h]
        cwd
        idiv    cx
        mov     word ptr cs:L085F,ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr cs:L0859,ax
        add     word ptr cs:L085F,dx
        mov     ax,word ptr [bx+6]
        sub     ax,word ptr ds:[7BDh]
        cwd
        idiv    cx
        mov     word ptr cs:L0853,ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr cs:L084D,ax
        add     word ptr cs:L0853,dx
        test    word ptr ds:[7D1h],0FFFFh
        ret
; wmap_bitmap's far entry (+545), named because TMAPOPS stores its offset (the link needs the
; name to take this module from the library)
_seg003_545 label far
        mov     word ptr cs:_seg003_3CA,cx
        mov     ax,es
        mov     word ptr cs:_seg003_3CC,ax
        push    es
        push    ds
        mov     ax,seg seg048
        mov     ds,ax
        mov     bx,ss
        mov     word ptr cs:L08E3,bx
        mov     word ptr cs:L08E5,sp
        cli
        mov     ss,ax
        mov     sp,word ptr ds:[5586h]
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
        mov     di,offset _seg003_3CE
        mov     si,659h
        rep movsb
        mov     es,bx
        pop     cx
        pop     ax
        add     ax,659h
        mov     word ptr ds:[7ABh],ax
        sub     ax,0Eh
        mov     word ptr ds:[7A9h],ax
        inc     cx
        mov     word ptr ds:[7D1h],cx
        mov     word ptr cs:_seg003_3B4,cx
        mov     si,word ptr es:[0B07Ch]
        lods    word ptr es:[si]
        dec     ax
        mov     word ptr ds:[7ADh],ax
        add     si,2
        lods    word ptr es:[si]
        mov     word ptr ds:[7AFh],ax
        lods    word ptr es:[si]
        mov     word ptr cs:L0830,ax
        mov     dx,8001h
        mov     bp,7FFFh
        mov     si,659h
        mov     di,si
        dec     cx
L05C9:
        lodsw
        cmp     dx,ax
        jge     L05D2
        mov     bx,si
        mov     dx,ax
L05D2:
        cmp     bp,ax
        jle     L05DC
        mov     bp,ax
        mov     word ptr ds:[7D8h],si
L05DC:
        lodsw
        cmp     ax,word ptr [di+2]
        jle     L05E5
        lea     di,[si-4]
L05E5:
        add     si,0Ah
        loop    L05C9
        mov     word ptr ds:[932h],dx
        mov     word ptr cs:_seg003_3B0,dx
        mov     word ptr cs:_seg003_3B2,bp
        mov     word ptr cs:_seg003_3B6,di
        mov     word ptr ds:[7B1h],di
        mov     si,di
        mov     word ptr ds:[7B3h],si
        mov     word ptr ds:[7B5h],si
        lodsw
        mov     word ptr cs:_seg003_3B8,ax
        lodsw
        mov     word ptr ds:[7CFh],ax
        mov     word ptr cs:_seg003_3BA,ax
        mov     word ptr cs:_seg003_3BE,1234h
        mov     word ptr cs:_seg003_3C0,1234h
        mov     word ptr cs:_seg003_3C4,1234h
        mov     word ptr cs:_seg003_3C2,1234h
        mov     bp,word ptr ds:[7D8h]
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
        jle     L0656
        mov     cx,word ptr [bx+0Ah]
L0656:
        idiv    cx
        cmp     ax,2
        mov     word ptr cs:_seg003_3BE,ax
        jg      L0691
L0661:
        mov     cx,word ptr [bx-2]
        mov     di,word ptr [bp-2]
        sub     cx,di
        add     di,7F2h
        mov     ax,word ptr [bx+2]
        mov     bx,word ptr [bp+2]
        sub     ax,bx
        cwd
        or      cx,cx
        jg      L0682
        mov     byte ptr [di],bh
        mov     byte ptr [di+2],bh
        jmp     L07C6
L0682:
        idiv    cx
        xchg    bx,ax
L0685:
        mov     byte ptr [di],ah
        inc     di
        add     ax,bx
        loop    L0685
        mov     byte ptr [di],ah
        jmp     L07C6
L0691:
        mov     cx,word ptr [bx+2]
        sub     cx,word ptr [bp+2]
        je      L0661
        mov     cl,ch
        xor     ch,ch
        mov     ax,word ptr es:[26B4h]
        mov     word ptr ds:[7EEh],ax
        mov     ax,word ptr es:[26B2h]
        mov     word ptr ds:[7F0h],ax
        mov     ax,word ptr [bp-2]
        mov     word ptr ds:[7DAh],ax
        mov     ax,word ptr [bp+0Ah]
        mov     word ptr ds:[7DEh],ax
        mov     word ptr ds:[7DCh],0
        mov     ax,word ptr [bp+6]
        mov     word ptr ds:[7E2h],ax
        mov     word ptr ds:[7E0h],0
        mov     ax,word ptr [bx+6]
        sub     ax,word ptr [bp+6]
        cwd
        idiv    cx
        mov     word ptr ds:[7E6h],ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[7E4h],ax
        add     word ptr ds:[7E6h],dx
        mov     ax,word ptr [bx+0Ah]
        sub     ax,word ptr [bp+0Ah]
        cwd
        idiv    cx
        mov     word ptr ds:[7EAh],ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[7E8h],ax
        add     word ptr ds:[7EAh],dx
        mov     ax,word ptr [bp+2]
        mov     word ptr ds:[7ECh],ax
        mov     bp,word ptr ds:[7DEh]
        mov     si,word ptr ds:[7DCh]
        mov     bx,word ptr [bx+2]
        mov     bl,byte ptr ds:[7EDh]
        mov     es,word ptr ds:[7D4h]
        or      cx,cx
        mov     word ptr cs:_seg003_3C0,cx
        mov     word ptr cs:_seg003_3C2,bx
        jle     L07A6
L0730:
        add     si,word ptr ds:[7E8h]
        adc     bp,word ptr ds:[7EAh]
        mov     ax,word ptr ds:[7E2h]
        mov     dx,word ptr ds:[7E0h]
        mov     word ptr cs:_seg003_3C6,ax
        mov     word ptr cs:_seg003_3C8,dx
        add     dx,word ptr ds:[7E4h]
        adc     ax,word ptr ds:[7E6h]
        mov     word ptr ds:[7E2h],ax
        mov     word ptr ds:[7E0h],dx
        imul    word ptr ds:[7F0h]
        idiv    bp
        add     ax,word ptr ds:[7EEh]
        js      L07A2
        mov     di,word ptr ds:[7DAh]
        cmp     ax,word ptr ds:[932h]
        jle     L0781
        mov     ax,word ptr ds:[932h]
        sub     ax,di
        jl      L077F
        add     di,7F2h
        inc     ax
        mov     cx,ax
        mov     al,bl
        rep stosb
L077F:
        jmp     short L07C6
L0781:
        mov     word ptr ds:[7DAh],ax
        sub     ax,di
        jl      L07A2
        inc     ax
        mov     word ptr cs:_seg003_3C4,ax
        add     di,7F2h
        mov     cx,ax
        mov     al,bl
        rep stosb
        inc     bl
        mov     byte ptr [di-1],bl
L079C:
        cmp     bl,bh
        jl      L0730
        jmp     short L07C6
L07A2:
        inc     bl
        jne     L079C
L07A6:
        int     2                       ; UW1 only: a break into a debugger (CX not above 0, or BL
                                        ; wrapped to 0), then on as UW2 goes
        mov     ax,word ptr ds:[932h]
        mov     di,word ptr ds:[7DAh]
        sub     ax,di
        jge     L07BC
        mov     di,word ptr ds:[932h]
        mov     ax,word ptr ds:[7DAh]
        sub     ax,di
L07BC:
        add     di,7F2h
        mov     cx,ax
        mov     al,bl
        rep stosb
L07C6:
        call    _seg003_4B9
        jne     L07CE
        jmp     L0898
L07CE:
        call    _seg003_422
        jne     L07D6
        jmp     L0898
L07D6:
        mov     es,word ptr ds:[95Ah]
L07DA:
        mov     di,word ptr ds:[7CFh]
        shl     di,1
        mov     di,word ptr [di+95Eh]
        mov     si,word ptr ds:[7B7h]
        add     di,si
        mov     cx,word ptr ds:[7C3h]
        sub     cx,si
        je      L0818
        mov     bp,word ptr ds:[7BDh]
        mov     ax,word ptr ds:[7C9h]
        sub     ax,bp
        cwd
        idiv    cx
        mov     word ptr cs:L083C,ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr cs:L0838,ax
        add     word ptr cs:L083C,dx
L0818:
        inc     cx
        jg      L081E
        jmp     L08E0
L081E:
        mov     dx,bp
        add     si,7F2h
        mov     bp,word ptr ds:[7C1h]
        mov     ds,word ptr ds:[7AFh]
L082C:
        mov     bx,dx
L0830 equ $+2
        and     bx,1234h
        lods    byte ptr ss:[si]
        xlatb
        stosb
L0838 equ $+2
        add     bp,1234h
L083C equ $+2
        adc     dx,1234h
        loop    L082C
        mov     ds,word ptr ss:[7D4h]
L0845:
        dec     word ptr ds:[7CFh]
L084D equ $+4
        add     word ptr ds:[7C1h],1234h
L0853 equ $+4
        adc     word ptr ds:[7BDh],1234
L0859 equ $+4
        add     word ptr ds:[7B9h],1234h
L085F equ $+4
        adc     word ptr ds:[7B7h],1234h
L0865 equ $+4
        add     word ptr ds:[7CDh],1234h
L086B equ $+4
        adc     word ptr ds:[7C9h],1234
L0871 equ $+4
        add     word ptr ds:[7C5h],1234h
L0877 equ $+4
        adc     word ptr ds:[7C3h],1234h
        mov     cx,word ptr ds:[7CFh]
L087F equ $+2
        cmp     cx,1234h
        jle     L08D1
L0883:
L0885 equ $+2
        cmp     cx,1234h
        jle     L088C
        jmp     L07DA
L088C:
        mov     bx,word ptr ds:[7B5h]
        call    L043A
        je      L0898
        jmp     L07DA
L0898:
        cli
        mov     ss,word ptr cs:L08E3
        mov     sp,word ptr cs:L08E5
        sti
        pop     ds
        pop     es
        ; UW1 only: before returning, when any byte of the pen-mode table at seg048:0008 (0FFh
        ; bytes) is set, take the colour from 3963:0659 (0A0h when it is 0, and store that back)
        ; into the colour byte 410F.
        push    es
        push    di
        push    ax
        mov     ax,seg seg048
        mov     es,ax
        mov     ax,0
        mov     cx,0FFh
        mov     di,8
        repe scasb
        jcxz    L08CD
        mov     al,byte ptr es:[658h]
        test    al,0FFh
        jne     L08C9
        mov     al,0A0h
        mov     byte ptr es:[658h],al
L08C9:
        mov     byte ptr es:[PEN_COLOR],al
L08CD:
        pop     ax
        pop     di
        pop     es
        retf
L08D1:
        mov     bx,word ptr ds:[7B3h]
        call    L04D1
        mov     cx,word ptr ds:[7CFh]
        jne     L0883
        jmp     L0898
L08E0:
        jmp     L0845
L08E3   dw      0
L08E5   dw      0
_seg003_4B9 endp

SEG003_TEXT ends

        end
