; /* target: seg021_22FD */
; /* opts: /ml */
;
; The system module: startup and shutdown (init, the exit routine it hooks in, CPU
; detection, the int 24h critical-error handler, palette save and restore, setting the DOS
; clock from the real-time clock), the keyboard (an int 9 handler with its own buffer),
; the joystick (port 201h), the mouse, and the far C entry points into the 3D code
; (cZoom, cFillFB, cPlaceFB, cRender, cInit3d, cFstSinCos, cSinCos, cAtan2, cSqRt,
; cFrmtoRaw), which switch to a private stack in the module's data segment.
;
; Hand-written assembly with .186. Most routines run with DS pointing at the module's
; own data segment (paragraph 60B9, IDA's dseg062_62a6), not DGROUP, so their near data
; is left as numbers: verify.py can only check DGROUP references, and naming them as
; DGROUP symbols would be wrong. That segment is named by the provisional extern
; dseg062_62a6, used only for its segment value; seg052_519C and seg_370D likewise.
; Small far thunks (`call x; retf`) give C entry points to near routines.
;
; Names: symbols.tsv for everything C calls (key, mouse, mbuttons and the c* routines are
; FM Towns names); from the FM Towns system module, setvect24 (hooks int 24h with INT24),
; diskerr (FM _diskerr: returns and clears the code INT24 left), INT24, init (the same
; sequence as FM init, down to the novideo/perror/noGraph test that falls into errorexit)
; and errorexit. cFrmtoRaw follows cSqRt in both builds and does the same thing (dh = 0FFh,
; then a decompressor chosen by the byte argument); symbols.tsv still has it as the
; provisional _seg021_22FD_F83, now _cFrmtoRaw. The rest keep IDA or provisional names.

        .model  medium
        .186

        extrn   _DRAW_RELATED_seg003_0272_7F9:far
        extrn   _MousQUp:far
        extrn   _seg003_0272_18:far
        extrn   _seg003_0272_5363:far
        extrn   _seg003_0272_6E2:far
        extrn   _seg003_0272_6E4:far
        extrn   _seg003_0272_764:far
        extrn   _seg003_0272_788:far
        extrn   _seg003_0272_7A8:far
        extrn   _seg004_0849_3007:far
        extrn   _seg004_0849_7F0C:far
        extrn   _seg004_0849_7F12:far
        extrn   dseg062_62a6:far
        extrn   seg052_519C:far
        extrn   seg_370D:far

        .code

        public  _seg021_22FD_0
        public  _seg021_22FD_17
        public  _seg021_22FD_65
        public  _seg021_22FD_A1
        public  _seg021_22FD_E9
        public  _seg021_22FD_112
        public  _seg021_22FD_11B
        public  _setvect24
        public  _seg021_22FD_174
        public  _diskerr
        public  _seg021_22FD_1D0
        public  _seg021_22FD_220
        public  _seg021_22FD_224
        public  _seg021_22FD_228
        public  _seg021_22FD_236
        public  _seg021_22FD_240
        public  _init
        public  _seg021_22FD_2B6
        public  _seg021_22FD_2BD
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
        public  _seg021_22FD_450
        public  _seg021_22FD_454
        public  _seg021_22FD_46B
        public  _seg021_22FD_4A8
        public  _seg021_22FD_51A
        public  _seg021_22FD_552
        public  _seg021_22FD_562
        public  _seg021_22FD_577
        public  _seg021_22FD_598
        public  _seg021_22FD_5C8
        public  _seg021_22FD_5E9
        public  _seg021_22FD_680
        public  _seg021_22FD_692
        public  _seg021_22FD_6A0
        public  _seg021_22FD_6A6
        public  _seg021_22FD_6C0
        public  _seg021_22FD_6C4
        public  _seg021_22FD_6C8
        public  _Read_Mouse_Motion_seg021_6E7
        public  _seg021_22FD_6F8
        public  _seg021_22FD_703
        public  _seg021_22FD_730
        public  _seg021_22FD_73E
        public  _seg021_22FD_755
        public  _seg021_22FD_791
        public  _seg021_22FD_7CD
        public  _seg021_22FD_809
        public  _key
        public  _mouse
        public  _mbuttons
        public  _SquareRoot_seg021_22FD_A30
        public  _seg021_22FD_A34
        public  _seg021_22FD_A38
        public  _seg021_22FD_A69
        public  _SquareRoot_seg021_22FD_A78
        public  _seg021_22FD_B78
        public  _seg021_22FD_BA2
        public  _MaybeTangent_seg021_22FD_BD4
        public  _seg021_22FD_C00
        public  _seg021_22FD_C10
        public  _cZoom
        public  _cFBtoScreen
        public  _cFillFB
        public  _CallbackFunctionSleepRelated_seg
        public  _Callback_seg021_22FD_CEA
        public  _cPlaceFB
        public  _cRender
        public  _cInit3d
        public  _cFstSinCos
        public  _cSinCos
        public  _cAtan2
        public  _cSqRt
        public  _cFrmtoRaw
        public  _seg021_22FD_FDD
        public  _seg021_22FD_1004
        public  _seg021_22FD_102B

; seg021_22FD_0  (+0)
_seg021_22FD_0 proc near
        push    es
        mov     ax,@data
        mov     es,ax
        mov     bx,92h
        mov     ax,word ptr es:[bx]
        mov     word ptr ds:[2],ax
        pop     es
        call    _seg021_22FD_65
        call    _init
        ret
_seg021_22FD_0 endp

; seg021_22FD_17  (+17)
_seg021_22FD_17 proc near
        call    _seg021_22FD_73E
        call    _seg021_22FD_174
        call    _seg021_22FD_A1
        call    _seg021_22FD_E9
        ret
L0024:
        db      0h
L0025:
        db      0h
L0026:
        db      2Eh dup (0)
L0054:
        db      11h dup (0)
_seg021_22FD_17 endp

; seg021_22FD_65  (+65)
_seg021_22FD_65 proc near
        push    si
        push    di
        push    es
        mov     ax,1009h
        mov     dx,@code
        mov     es,dx
        mov     dx,54h
        int     10h
        mov     di,0
        mov     si,0
L007B:
        mov     ax,1015h
        mov     bl,byte ptr cs:L0054[si]
        int     10h
        mov     byte ptr cs:L0024[di],dh
        mov     byte ptr cs:L0025[di],ch
        mov     byte ptr cs:L0026[di],cl
        inc     si
        add     di,3
        cmp     di,30h
        jne     L007B
        pop     es
        pop     di
        pop     si
        ret
_seg021_22FD_65 endp

; MaybePaletteCycling_seg021_22FD_A1  (+A1)
_seg021_22FD_A1 proc near
        push    si
        push    di
        mov     ax,1002h
        mov     dx,@code
        push    es
        mov     es,dx
        mov     dx,54h
        mov     bx,0
        mov     cx,10h
        int     10h
        mov     cx,10h
        mov     si,0
        mov     di,0
        mov     bx,0
L00C3:
        mov     ax,1010h
        mov     bl,byte ptr cs:L0054[si]
        mov     dh,byte ptr cs:L0024[di]
        mov     ch,byte ptr cs:L0025[di]
        mov     cl,byte ptr cs:L0026[di]
        int     10h
        add     di,3
        inc     si
        cmp     si,10h
        jne     L00C3
        pop     es
        pop     di
        pop     si
        ret
_seg021_22FD_A1 endp

; seg021_22FD_E9  (+E9)
_seg021_22FD_E9 proc near
        mov     ah,4
        int     1Ah
        push    cx
        push    dx
        mov     ah,2
        int     1Ah
        call    _seg021_22FD_112
        xor     dl,dl
        mov     ah,2Dh
        int     21h
        pop     dx
        pop     cx
        call    _seg021_22FD_112
        xor     al,al
        xchg    ah,al
        mov     ch,64h
        mul     ch
        xor     ch,ch
        add     cx,ax
        mov     ah,2Bh
        int     21h
        ret
_seg021_22FD_E9 endp

; seg021_22FD_112  (+112)
_seg021_22FD_112 proc near
        push    cx
        mov     ax,dx
        call    _seg021_22FD_11B
        mov     dx,ax
        pop     ax
_seg021_22FD_112 endp

; seg021_22FD_11B  (+11B)
_seg021_22FD_11B proc near
        xor     bx,bx
        mov     cx,4
        xchg    ch,ah
        xchg    ch,al
        ror     ax,cl
        shr     ah,cl
        xchg    bh,ah
        mov     cl,0Ah
        mul     cl
        add     al,bh
        xchg    ch,al
        xor     ah,ah
        mov     cl,4
        ror     ax,cl
        shr     ah,cl
        xchg    bl,ah
        mov     cl,0Ah
        mul     cl
        add     al,bl
        mov     ah,ch
        mov     cl,al
        ret
        db      9 dup (0)
_seg021_22FD_11B endp

; seg021_22FD_150  (+150)
_setvect24 proc near
        push    bp
        push    ds
        push    es
        mov     ax,3524h
        int     21h
        mov     word ptr cs:L0170,es
        mov     word ptr cs:L0172,bx
        push    cs
        pop     ds
        mov     dx,offset INT24
        mov     ax,2524h
        int     21h
        pop     es
        pop     ds
        pop     bp
        ret
L0170:
        db      0h,0h
L0172:
        db      0h,0h
_setvect24 endp

; seg021_22FD_174  (+174)
_seg021_22FD_174 proc near
        push    bp
        push    ds
        mov     ax,word ptr cs:L0170
        push    ax
        pop     ds
        mov     dx,word ptr cs:L0172
        mov     ax,2524h
        int     21h
        pop     ds
        pop     bp
        ret
_seg021_22FD_174 endp

; seg021_22FD_189  (+189)
_diskerr proc near
        mov     al,byte ptr cs:L0195
        xor     ah,ah
        mov     byte ptr cs:L0195,ah
        ret
L0195:
        db      0h                              ; the error code _diskerr hands back

; The DOS critical-error handler (int 24h): fail the call and remember why.
INT24:
        cmp     byte ptr cs:L0195,0
        jne     short L01C0
        and     ah,80h
        cmp     ah,0
        je      short L01B8
        push    ds
        push    bp
        pop     ds
        mov     ax,word ptr [si+4]
        pop     ds
        and     ax,8000h
        cmp     ax,0
        jne     short L01B8
        mov     di,0Dh
L01B8:
        mov     ax,di
        add     al,14h
        mov     byte ptr cs:L0195,al
L01C0:
        mov     al,0
        iret
        db      0Dh dup (0)
_diskerr endp

; seg021_22FD_1D0  (+1D0)
_seg021_22FD_1D0 proc near
        pushf
        xor     ax,ax
        push    ax
        popf
        pushf
        pop     ax
        and     ax,0F000h
        cmp     ax,0F000h
        je      short L01F5
        mov     ax,7000h
        push    ax
        popf
        pushf
        pop     ax
        and     ax,7000h
        je      short L01F0
        mov     ax,386h
        jmp     short L0216
L01F0:
        mov     ax,286h
        jmp     short L0216
L01F5:
        push    cx
        mov     ax,0FFFFh
        mov     cl,21h
        shl     ax,cl
        jne     short L0212
        pop     cx
        xor     al,al
        mov     al,40h
        mul     al
        je      short L020D
        mov     ax,86h
        jmp     short L0216
L020D:
        mov     ax,30h
        jmp     short L0216
L0212:
        mov     ax,186h
        pop     cx
L0216:
        popf
        mov     word ptr ds:[110h],ax
        ret
        db      0h,0h,0h,0h,0h
_seg021_22FD_1D0 endp

; seg021_22FD_220  (+220)
_seg021_22FD_220 proc far
        call    L022E
        retf
_seg021_22FD_220 endp

; seg021_22FD_224  (+224)
_seg021_22FD_224 proc far
        call    _seg021_22FD_236
        retf
_seg021_22FD_224 endp

; seg021_22FD_228  (+228)
_seg021_22FD_228 proc near
        mov     ah,1
        int     16h
        je      short L0233
L022E:
        mov     ah,0
        int     16h
        ret
L0233:
        sub     ax,ax
        ret
_seg021_22FD_228 endp

; seg021_22FD_236  (+236)
_seg021_22FD_236 proc near
        call    _seg021_22FD_228
        jne     _seg021_22FD_236
        ret
        db      0h,0h,0h,0h
_seg021_22FD_236 endp

; seg021_22FD_240  (+240)
_seg021_22FD_240 proc far
        call    errorexit
        retf
_seg021_22FD_240 endp

; seg021_22FD_244  (+244)
_init proc near
        call    _seg021_22FD_6C8
        call    _seg021_22FD_1D0
        call    _seg021_22FD_2B6
        call    _setvect24
        call    _seg021_22FD_2BD
        call    _seg021_22FD_30A
        call    _seg021_22FD_598
        call    _seg021_22FD_730
        push    ds
        lds     si,dword ptr ds:[0]
        mov     di,18Ch
        movsw
        movsw
        pop     ds
        push    es
        les     di,dword ptr ds:[0]
        mov     ax,offset L028C                 ; the exit routine below
        stosw
        mov     ax,cs
        stosw
        pop     es
        mov     cx,word ptr ds:[370h]
        jcxz    L028B
        cmp     word ptr ds:[190h],0
        jne     short errorexit
        mov     si,560h
errorexit:
        mov     word ptr ds:[190h],si
        jmp     _seg021_22FD_17
L028B:
        ret

; Reached through the far pointer init stored at DS:0 (the exit chain).
L028C:
        mov     ax,seg dseg062_62a6
        mov     es,ax
        mov     ds,ax
        cli
        mov     ss,ax
        mov     ax,122h
        mov     sp,ax
        sti
        call    word ptr ds:[192h]
        call    _seg021_22FD_5C8
        cmp     word ptr ds:[190h],0
        je      short L02B2
        mov     dx,word ptr ds:[190h]
        mov     ah,9
        int     21h
L02B2:
        jmp     dword ptr ds:[18Ch]
_init endp

; seg021_22FD_2B6  (+2B6)
_seg021_22FD_2B6 proc near
        mov     ax,seg seg052_519C
        mov     word ptr ds:[18Ah],ax
        ret
_seg021_22FD_2B6 endp

; seg021_22FD_2BD  (+2BD)
_seg021_22FD_2BD proc near
        push    ds
        lds     si,dword ptr ds:[186h]
        lodsb
        cmp     al,0FFh
        jne     short L02D6
        mov     si,0C000h
        lodsb
        cmp     al,21h
        jne     short L02D6
        mov     word ptr ss:[120h],0FFFFh
L02D6:
        pop     ds
        ret
        db      8 dup (0)
_seg021_22FD_2BD endp

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
        db      0Ah dup (0)
_seg021_22FD_3FD endp

; seg021_22FD_450  (+450)
_seg021_22FD_450 proc far
        call    _seg021_22FD_4A8
        retf
_seg021_22FD_450 endp

; seg021_22FD_454  (+454)
_seg021_22FD_454 proc near
        mov     ah,al
        and     ah,80h
        and     al,7Fh
        mov     di,26Eh
        mov     cx,0Dh
        repne scasb
        jne     short L0468
        mov     al,byte ptr [di+0Ch]
L0468:
        or      al,ah
        ret
_seg021_22FD_454 endp

; seg021_22FD_46B  (+46B)
_seg021_22FD_46B proc near
        cmp     word ptr ds:[1E9h],0
        jne     short L048C
        push    ds
        sub     ax,ax
        mov     ds,ax
        and     byte ptr ds:[496h],10h
        pop     ds
        jne     short L0494
        cmp     word ptr ds:[120h],0
        jne     short L048D
        mov     word ptr ds:[1E9h],0
L048C:
        ret
L048D:
        mov     word ptr ds:[1E9h],4
        ret
L0494:
        mov     word ptr ds:[1E9h],2
        ret
L049B:
        sub     ax,ax
        ret
L049E:
        call    _seg021_22FD_577
        call    _seg021_22FD_577
        mov     al,0E0h
        jmp     short L04DA
_seg021_22FD_46B endp

; seg021_22FD_4A8  (+4A8)
_seg021_22FD_4A8 proc near
        call    _seg021_22FD_562
        jb      L049B
        cmp     al,0E1h
        je      L049E
        cmp     al,0E0h
        jne     short L04DA
        call    _seg021_22FD_577
        mov     byte ptr ds:[26Bh],al
        and     al,7Fh
        cmp     al,2Ah
        je      _seg021_22FD_4A8
        cmp     al,36h
        je      _seg021_22FD_4A8
        mov     di,288h
        mov     cx,10h
        repne scasb
        jne     _seg021_22FD_4A8
        sub     di,229h
        mov     al,byte ptr ds:[26Bh]
        and     al,80h
        or      ax,di
L04DA:
        cmp     word ptr ds:[1E9h],4
        jne     short L04E4
        call    _seg021_22FD_454
L04E4:
        sub     ah,ah
        mov     bx,ax
        and     bl,7Fh
        test    al,80h
        jne     short L053B
        cmp     word ptr ds:[1E0h],0
        jne     short L04FD
        cmp     byte ptr [bx+1EBh],0
        jne     _seg021_22FD_4A8
L04FD:
        inc     byte ptr [bx+1EBh]
        cmp     byte ptr ds:[1E2h],0
        je      short L050B
        or      bl,80h
L050B:
        ; mov ah,[bx+10h] with a 16-bit displacement: the source named a relocatable
        ; label (offset 10h in the module's data segment), which always takes 16 bits.
        ; A plain number gives 8A 67 10, and a forward equate 8A 67 10 90.
        db      8Ah,0A7h,10h,0h
        push    ax
        call    _seg021_22FD_51A
        pop     ax
        sub     bx,bx
        xchg    al,ah
        inc     bx
        ret
_seg021_22FD_4A8 endp

; seg021_22FD_51A  (+51A)
_seg021_22FD_51A proc near
        mov     cx,6
        mov     bx,word ptr ds:[1E9h]
        mov     di,1E2h
L0524:
        mov     si,word ptr [bx+299h]
        mov     al,byte ptr [si+1EBh]
        mov     si,word ptr [bx+2A1h]
        or      al,byte ptr [si+1EBh]
        stosb
        add     bx,10h
        loop    L0524
        ret
L053B:
        mov     byte ptr [bx+1EBh],0
        call    _seg021_22FD_51A
        jmp     _seg021_22FD_4A8
L0546:
        call    _seg021_22FD_552
        je      short L058B
L054B:
        call    _seg021_22FD_552
        je      short L058B
        jmp     short _seg021_22FD_577
_seg021_22FD_51A endp

; seg021_22FD_552  (+552)
_seg021_22FD_552 proc near
        inc     si
        cmp     si,340h
        jne     short L055C
        mov     si,300h
L055C:
        cmp     si,word ptr cs:L0590
        ret
_seg021_22FD_552 endp

; seg021_22FD_562  (+562)
_seg021_22FD_562 proc near
        mov     si,word ptr ds:[26Ch]
        cmp     si,word ptr cs:L0590
        je      short L058B
        mov     al,byte ptr [si]
        cmp     al,0E0h
        je      L054B
        cmp     al,0E1h
        je      L0546
_seg021_22FD_562 endp

; seg021_22FD_577  (+577)
_seg021_22FD_577 proc near
        mov     si,word ptr ds:[26Ch]
        lodsb
        cmp     si,340h
        jne     short L0585
        mov     si,300h
L0585:
        mov     word ptr ds:[26Ch],si
        clc
        ret
L058B:
        stc
        ret
        db      0h,0h,0h
L0590:
        db      0h,3h
        dw      seg dseg062_62a6
L0594:
        db      0h,0h
L0596:
        db      0h,0h
_seg021_22FD_577 endp

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
        lds     di,dword ptr cs:L0590
        in      al,60h
        mov     byte ptr [di],al
        inc     di
        cmp     di,340h
        jne     short L0645
        mov     di,300h
L0645:
        mov     word ptr cs:L0590,di
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
        db      0Bh dup (0)
_seg021_22FD_5E9 endp

; seg021_22FD_680  (+680)
_seg021_22FD_680 proc near
        mov     ah,0Fh
        int     10h
        mov     byte ptr ds:[350h],al
        push    ds
        lds     si,dword ptr ds:[351h]
        lodsb
        pop     ds
        mov     byte ptr ds:[355h],al
        ret
_seg021_22FD_680 endp

; seg021_22FD_692  (+692)
_seg021_22FD_692 proc near
        mov     al,byte ptr ds:[355h]
        call    _seg021_22FD_6A6
        mov     ah,0
        mov     al,byte ptr ds:[350h]
        int     10h
        ret
_seg021_22FD_692 endp

; seg021_22FD_6A0  (+6A0)
_seg021_22FD_6A0 proc near
        mov     al,20h
        jmp     short _seg021_22FD_6A6
        mov     al,30h
_seg021_22FD_6A0 endp

; seg021_22FD_6A6  (+6A6)
_seg021_22FD_6A6 proc near
        mov     ah,byte ptr ds:[355h]
        and     ah,0CFh
        or      al,ah
        push    es
        les     di,dword ptr ds:[351h]
        stosb
        pop     es
        ret
        db      9 dup (0)
_seg021_22FD_6A6 endp

; seg021_22FD_6C0  (+6C0)
_seg021_22FD_6C0 proc far
        call    _Read_Mouse_Motion_seg021_6E7
        retf
_seg021_22FD_6C0 endp

; seg021_22FD_6C4  (+6C4)
_seg021_22FD_6C4 proc far
        call    _seg021_22FD_703
        retf
_seg021_22FD_6C4 endp

; seg021_22FD_6C8  (+6C8)
_seg021_22FD_6C8 proc near
        push    es
        sub     ax,ax
        mov     es,ax
        mov     di,0CCh
        scasw
        jne     short L06D6
        scasw
        je      short L06DF
L06D6:
        int     33h
        test    ax,ax
        je      short L06DF
        mov     ax,0C8h
L06DF:
        mov     word ptr ds:[360h],ax
        mov     word ptr ds:[362h],ax
        pop     es
        ret
_seg021_22FD_6C8 endp

; Read_Mouse_Motion_seg021_6E7  (+6E7)
_Read_Mouse_Motion_seg021_6E7 proc near
        mov     ax,0Bh
        int     33h
        push    dx
        xchg    cx,ax
        call    _seg021_22FD_6F8
        pop     ax
        push    bx
        call    _seg021_22FD_6F8
        pop     ax
        ret
_Read_Mouse_Motion_seg021_6E7 endp

; Mouse_Sensitivity_Maybe_seg021_6F8  (+6F8)
_seg021_22FD_6F8 proc near
        mov     bx,64h
        imul    bx
        idiv    word ptr ds:[360h]
        xchg    bx,ax
        ret
_seg021_22FD_6F8 endp

; Mouse_Position_And_Button_status_seg021_703  (+703)
_seg021_22FD_703 proc near
        mov     ax,3
        int     33h
        xchg    bx,ax
        and     ax,3
        ret
        db      0h,0h,0h
L0710:
        db      0h,0h
L0712:
        db      0h,0h
        add     word ptr cs:L0710,1
        adc     word ptr cs:L0712,0
        retf
        db      0Fh dup (0)
_seg021_22FD_703 endp

; seg021_22FD_730  (+730)
_seg021_22FD_730 proc near
        call    _seg021_22FD_680
        call    _seg021_22FD_6A0
        clc
        jae     short L073D
        dec     word ptr ds:[370h]
L073D:
        ret
_seg021_22FD_730 endp

; seg021_22FD_73E  (+73E)
_seg021_22FD_73E proc near
        jmp     _seg021_22FD_692
        db      0Fh dup (0)
L0750:
        db      0h,0h
L0752:
        db      0h,0h
        ret
_seg021_22FD_73E endp

; seg021_22FD_755  (+755)
_seg021_22FD_755 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_0
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg021_22FD_755 endp

; seg021_22FD_791  (+791)
_seg021_22FD_791 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_17
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg021_22FD_791 endp

; seg021_22FD_7CD  (+7CD)
_seg021_22FD_7CD proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_3A3
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg021_22FD_7CD endp

; seg021_22FD_809  (+809)
_seg021_22FD_809 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_2EC
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_598
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_5C8
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_236
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_1D0
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _diskerr
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg021_22FD_809 endp

; seg021_22FD_971  (+971)
_key proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_4A8
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_key endp

; seg021_22FD_9AD  (+9AD)
_mouse proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _Read_Mouse_Motion_seg021_6E7
        mov     word ptr ds:[510h],ax
        mov     word ptr ds:[512h],bx
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_mouse endp

; Mouse_Button_Status_seg021_9F0  (+9F0)
_mbuttons proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0750,dx
        mov     dx,sp
        mov     word ptr cs:L0752,dx
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_703
        cli
        mov     dx,word ptr cs:L0750
        mov     ss,dx
        mov     dx,word ptr cs:L0752
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
        db      0h,0h,0h,0h
_mbuttons endp

; SquareRoot_seg021_22FD_A30  (+A30)
_SquareRoot_seg021_22FD_A30 proc far
        call    _SquareRoot_seg021_22FD_A78
        retf
_SquareRoot_seg021_22FD_A30 endp

; seg021_22FD_A34  (+A34)
_seg021_22FD_A34 proc far
        call    _seg021_22FD_A38
        retf
_seg021_22FD_A34 endp

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
        db      0h,0h,0h,0h,0h,0h,0h
_MaybeTangent_seg021_22FD_BD4 endp

; seg021_22FD_C00  (+C00)
_seg021_22FD_C00 proc far
        mov     ax,word ptr cs:L0710
        retf
        db      0Bh dup (0)
_seg021_22FD_C00 endp

; seg021_22FD_C10  (+C10)
_seg021_22FD_C10 proc far
        retf
        db      0Fh dup (0)
L0C20:
        db      0h,0h
L0C22:
        db      0h,0h
L0C24:
        db      0h,0h
L0C26:
        db      0h,0h
L0C28:
        db      0h,0h
L0C2A:
        db      0h,0h
L0C2C:
        db      0h,0h
L0C2E:
        db      0h,0h
L0C30:
        db      0h,0h
L0C32:
        db      0h,0h
_seg021_22FD_C10 endp

; seg021_22FD_C34  (+C34)
_cZoom proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0C24,dx
        mov     dx,sp
        mov     word ptr cs:L0C26,dx
        mov     ax,word ptr [bp+6]
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        mov     bx,seg seg052_519C
        mov     es,bx
        mov     di,word ptr es:[158h]
        add     di,2
        stosw
        cli
        mov     dx,word ptr cs:L0C24
        mov     ss,dx
        mov     dx,word ptr cs:L0C26
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_cZoom endp

; seg021_22FD_C7E  (+C7E)
_cFBtoScreen proc far
        call    far ptr _DRAW_RELATED_seg003_0272_7F9
        retf
_cFBtoScreen endp

; seg021_22FD_C84  (+C84)
_cFillFB proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0C24,dx
        mov     dx,sp
        mov     word ptr cs:L0C26,dx
        mov     ax,word ptr [bp+6]
        call    far ptr _seg003_0272_6E4
        cli
        mov     dx,word ptr cs:L0C24
        mov     ss,dx
        mov     dx,word ptr cs:L0C26
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_cFillFB endp

; CallbackFunctionSleepRelated_seg021_22FD_CB7  (+CB7)
_CallbackFunctionSleepRelated_seg proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0C24,dx
        mov     dx,sp
        mov     word ptr cs:L0C26,dx
        mov     cx,word ptr [bp+6]
        call    far ptr _seg003_0272_764
        cli
        mov     dx,word ptr cs:L0C24
        mov     ss,dx
        mov     dx,word ptr cs:L0C26
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_CallbackFunctionSleepRelated_seg endp

; Callback_seg021_22FD_CEA  (+CEA)
_Callback_seg021_22FD_CEA proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0C24,dx
        mov     dx,sp
        mov     word ptr cs:L0C26,dx
        mov     cx,word ptr [bp+6]
        call    far ptr _seg003_0272_788
        cli
        mov     dx,word ptr cs:L0C24
        mov     ss,dx
        mov     dx,word ptr cs:L0C26
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_Callback_seg021_22FD_CEA endp

; seg021_22FD_D1D  (+D1D)
_cPlaceFB proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0C24,dx
        mov     dx,sp
        mov     word ptr cs:L0C26,dx
        mov     bx,word ptr [bp+6]
        mov     cx,word ptr [bp+8]
        mov     ax,0C7h
        sub     ax,cx
        mov     cl,50h
        mul     cl
        shr     bx,1
        shr     bx,1
        add     ax,bx
        mov     bx,word ptr [bp+0Ah]
        mov     cx,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,word ptr ds:[5588h]
        sti
        mov     word ptr ds:[94Eh],ax
        call    far ptr _seg003_0272_7A8
        cli
        mov     dx,word ptr cs:L0C24
        mov     ss,dx
        mov     dx,word ptr cs:L0C26
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_cPlaceFB endp

; seg021_22FD_D7A  (+D7A)
_cRender proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0C24,dx
        mov     dx,sp
        mov     word ptr cs:L0C26,dx
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        mov     ax,word ptr cs:L0710
        mov     word ptr cs:L0C28,ax
        mov     ax,word ptr cs:L0712
        mov     word ptr cs:L0C2A,ax
        mov     bx,0
        mov     es,bx
        mov     ax,word ptr es:[bx]
        mov     word ptr cs:L0C2C,ax
        mov     ax,word ptr es:[bx+2]
        mov     word ptr cs:L0C2E,ax
        mov     dx,5A0h
        mov     ax,2500h
        int     21h
        call    far ptr _seg003_0272_6E2
        call    far ptr _seg004_0849_3007
        call    far ptr _seg003_0272_18
        call    far ptr _seg004_0849_7F12
        cli
        mov     dx,word ptr cs:L0C24
        mov     ss,dx
        mov     dx,word ptr cs:L0C26
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        mov     bx,0
        mov     es,bx
        mov     ax,word ptr cs:L0C2C
        mov     word ptr es:[bx],ax
        mov     ax,word ptr cs:L0C2E
        mov     word ptr es:[bx+2],ax
        mov     ax,word ptr cs:L0710
        sub     ax,word ptr cs:L0C28
        mov     dx,word ptr cs:L0712
        sub     dx,word ptr cs:L0C2A
        pop     bp
        retf
_cRender endp

; seg021_22FD_E1A  (+E1A)
_cInit3d proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0C24,dx
        mov     dx,sp
        mov     word ptr cs:L0C26,dx
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        mov     word ptr cs:L0710,0
        mov     si,word ptr ds:[0A98h]
        call    far ptr _seg003_0272_5363
        cli
        mov     dx,word ptr cs:L0C24
        mov     ss,dx
        mov     dx,word ptr cs:L0C26
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_cInit3d endp

; GetVectorForDirection_seg021_22FD_E63  (+E63)
_cFstSinCos proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0C24,dx
        mov     dx,sp
        mov     word ptr cs:L0C26,dx
        mov     bx,word ptr [bp+6]
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _seg021_22FD_A69
        cli
        mov     dx,word ptr cs:L0C24
        mov     ss,dx
        mov     dx,word ptr cs:L0C26
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        push    di
        mov     di,word ptr [bp+8]
        mov     word ptr [di],ax
        mov     di,word ptr [bp+0Ah]
        mov     word ptr [di],bx
        pop     di
        pop     bp
        retf
_cFstSinCos endp

; SomethingProjectileHeading_seg021_22FD_EAE  (+EAE)
_cSinCos proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0C24,dx
        mov     dx,sp
        mov     word ptr cs:L0C26,dx
        mov     bx,word ptr [bp+6]
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        push    bp
        call    _seg021_22FD_A38
        pop     bp
        cli
        mov     dx,word ptr cs:L0C24
        mov     ss,dx
        mov     dx,word ptr cs:L0C26
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        push    di
        mov     di,word ptr [bp+8]
        mov     word ptr [di],ax
        mov     di,word ptr [bp+0Ah]
        mov     word ptr [di],bx
        pop     di
        pop     bp
        retf
_cSinCos endp

; MaybeGetTangent_seg021_22FD_EFB  (+EFB)
_cAtan2 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0C24,dx
        mov     dx,sp
        mov     word ptr cs:L0C26,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _MaybeTangent_seg021_22FD_BD4
        mov     ax,cx
        cli
        mov     dx,word ptr cs:L0C24
        mov     ss,dx
        mov     dx,word ptr cs:L0C26
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_cAtan2 endp

; GetSquareRoot_seg021_22FD_F3F  (+F3F)
_cSqRt proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0C24,dx
        mov     dx,sp
        mov     word ptr cs:L0C26,dx
        mov     bx,word ptr [bp+6]
        mov     cx,word ptr [bp+8]
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        call    _SquareRoot_seg021_22FD_A78
        mov     ax,di
        cli
        mov     dx,word ptr cs:L0C24
        mov     ss,dx
        mov     dx,word ptr cs:L0C26
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_cSqRt endp

; cFrmtoRaw  (+F83)
_cFrmtoRaw proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L0C24,dx
        mov     dx,sp
        mov     word ptr cs:L0C26,dx
        mov     dx,word ptr [bp+6]
        mov     ax,word ptr [bp+8]
        mov     si,word ptr [bp+0Ah]
        mov     cx,word ptr [bp+0Ch]
        mov     bl,byte ptr [bp+0Eh]
        xor     bh,bh
        mov     bp,dx
        mov     dx,seg dseg062_62a6
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,510h
        sti
        mov     ds,cx
        mov     dh,0FFh
        call    far ptr _seg004_0849_7F0C
        cli
        mov     dx,word ptr cs:L0C24
        mov     ss,dx
        mov     dx,word ptr cs:L0C26
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        mov     dx,ax
        mov     ax,0
        pop     bp
        retf
_cFrmtoRaw endp

; seg021_22FD_FDD  (+FDD)
_seg021_22FD_FDD proc near
        mov     ax,ss
        mov     ds,ax
        mov     si,510h
        mov     cx,si
        sub     cx,sp
        shr     cx,1
        mov     es,ax
        mov     di,0C2Ah
        std
        rep movsw
        cld
        mov     ax,word ptr cs:L0C24
        mov     word ptr cs:L0C20,ax
        mov     ax,word ptr cs:L0C26
        mov     word ptr cs:L0C22,ax
        ret
_seg021_22FD_FDD endp

; seg021_22FD_1004  (+1004)
_seg021_22FD_1004 proc near
        mov     ax,ss
        mov     ds,ax
        mov     si,0C2Ah
        mov     di,510h
        mov     cx,di
        sub     cx,sp
        shr     cx,1
        mov     es,ax
        std
        rep movsw
        cld
        mov     ax,word ptr cs:L0C20
        mov     word ptr cs:L0C24,ax
        mov     ax,word ptr cs:L0C22
        mov     word ptr cs:L0C26,ax
        ret
_seg021_22FD_1004 endp

; seg021_22FD_102B  (+102B)
_seg021_22FD_102B proc far
        push    si
        push    ds
        push    es
        call    _seg021_22FD_FDD
        mov     ax,@data
        mov     ds,ax
        mov     es,ax
        mov     ax,ss
        mov     word ptr cs:L0C30,ax
        mov     word ptr cs:L0C32,sp
        mov     ax,211Ch
        cli
        mov     sp,ax
        mov     ax,@data
        mov     ss,ax
        sti
        mov     bp,sp
        push    bp
        mov     bp,sp
        mov     ax,1
        push    ax
        call    far ptr _MousQUp
        pop     bp
        cli
        mov     ss,word ptr cs:L0C30
        mov     sp,word ptr cs:L0C32
        sti
        call    _seg021_22FD_1004
        pop     es
        pop     ds
        pop     si
        retf
_seg021_22FD_102B endp

        end
