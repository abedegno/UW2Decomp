; /* target: seg004_0849_260 */
; /* opts: /ml */
;
; sphere_check, get_dist and do_obj (the model opcode handler).
;
; SEG004 split into its original modules: this is module 2 of 14, +260..+600 of the segment
; (file 9420h), a module of the second library (UWLIB.LIB).
; Procs: sphere_check, get_dist, do_obj.
; Evidence for the bounds: starts on a paragraph after 10 zero bytes following a jmp: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .386

_DATA   segment word public use16 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public use16 'CODE'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _self_modify:near
        extrn   _set_accept:near
        public  _get_dist
        public  _sphere_check
        public  do_obj
        public  modify_axis2

; seg004_0849_260  (+260)
_sphere_check proc near
        mov     word ptr ds:[10h],cx
        shl     si,cl
        shl     bx,cl
        neg     bx
        shl     bp,cl
        neg     bp
        shl     ax,cl
        neg     ax
        mov     cx,ax
        mov     ax,bx
        imul    word ptr ds:[14B6h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[14BCh]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14C2h]
        add     di,dx
        mov     word ptr ds:[0Eh],di
        jns     short L029C
        add     di,si
        jns     short L0299
        jmp     L040D
L0299:
        jmp     L036F
L029C:
        mov     ax,bx
        imul    word ptr ds:[14B2h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[14B8h]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14BEh]
        add     di,dx
        mov     ax,si
        mul     word ptr ds:[24E8h]
        mov     ax,word ptr ds:[0Eh]
        sub     ax,di
        jns     short L02CB
        add     ax,dx
        jns     short L02C8
        jmp     L040D
L02C8:
        jmp     L0398
L02CB:
        sub     ax,dx
        jns     short L02D2
        jmp     L0398
L02D2:
        add     di,word ptr ds:[0Eh]
        jns     short L02E2
        add     di,dx
        jns     short L02DF
        jmp     L040D
L02DF:
        jmp     L03A2
L02E2:
        sub     di,dx
        jns     short L02E9
        jmp     L03A2
L02E9:
        mov     ax,bx
        imul    word ptr ds:[14B4h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[14BAh]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14C0h]
        add     di,dx
        mov     ax,si
        mul     word ptr ds:[24EAh]
        mov     ax,word ptr ds:[0Eh]
        sub     ax,di
        jns     short L0318
        add     ax,dx
        jns     short L0315
        jmp     L040D
L0315:
        jmp     L03CB
L0318:
        sub     ax,dx
        jns     short L031F
        jmp     L03CB
L031F:
        add     di,word ptr ds:[0Eh]
        jns     short L032F
        add     di,dx
        jns     short L032C
        jmp     L040D
L032C:
        jmp     L03D5
L032F:
        sub     di,dx
        jns     short L0336
        jmp     L03D5
L0336:
        mov     ax,word ptr ds:[0Eh]
        or      ax,ax
        jns     short L0341
        xor     ax,ax
        jmp     short L0368
L0341:
        mov     cx,word ptr ds:[10h]
        sar     ax,cl
        test    word ptr ds:[24E6h],0FFFFh
        jns     short L0368
        mov     cx,word ptr ds:[24E6h]
        neg     cx
        mov     dx,ax
        xor     ax,ax
        sar     dx,1
        cmp     dx,cx
        jl      short L0364
        mov     ax,7FFFh
        jmp     short L0368
L0364:
        rcr     ax,1
        div     cx
L0368:
        shr     ax,1
        clc
        mov     si,0FFFFh
        ret
L036F:
        mov     ax,bx
        imul    word ptr ds:[14B2h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[14B8h]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14BEh]
        add     di,dx
        mov     ax,si
        mul     word ptr ds:[24E8h]
        mov     ax,word ptr ds:[0Eh]
        sub     ax,di
        jns     short L0398
        add     ax,dx
        js      short L040D
L0398:
        add     di,word ptr ds:[0Eh]
        jns     short L03A2
        add     di,dx
        js      short L040D
L03A2:
        mov     ax,bx
        imul    word ptr ds:[14B4h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[14BAh]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14C0h]
        add     di,dx
        mov     ax,si
        mul     word ptr ds:[24EAh]
        mov     ax,word ptr ds:[0Eh]
        sub     ax,di
        jns     short L03CB
        add     ax,dx
        js      short L040D
L03CB:
        add     di,word ptr ds:[0Eh]
        jns     short L03D5
        add     di,dx
        js      short L040D
L03D5:
        mov     ax,word ptr ds:[0Eh]
        or      ax,ax
        jns     short L03E0
        xor     ax,ax
        jmp     short L0407
L03E0:
        mov     cx,word ptr ds:[10h]
        sar     ax,cl
        test    word ptr ds:[24E6h],0FFFFh
        jns     short L0407
        mov     cx,word ptr ds:[24E6h]
        neg     cx
        mov     dx,ax
        xor     ax,ax
        sar     dx,1
        cmp     dx,cx
        jl      short L0403
        mov     ax,7FFFh
        jmp     short L0407
L0403:
        rcr     ax,1
        div     cx
L0407:
        shr     ax,1
        clc
        xor     si,si
        ret
L040D:
        stc
        ret
_sphere_check endp

; seg004_0849_40F  (+40F)
_get_dist proc near
        push    si
        mov     bx,word ptr ds:[16h]
        mov     ax,word ptr ds:[18h]
        cwd
        xor     bx,dx
        xor     ax,dx
        neg     dx
        add     bx,dx
        adc     ax,0
        mov     bp,ax
        mov     cx,word ptr ds:[1Eh]
        mov     ax,word ptr ds:[20h]
        cwd
        xor     cx,dx
        xor     ax,dx
        neg     dx
        add     cx,dx
        adc     ax,0
        cmp     bp,ax
        jl      short L0445
        jg      short L0442
        cmp     bx,cx
        jl      short L0445
L0442:
        xchg    bp,ax
        xchg    bx,cx
L0445:
        sar     bp,1
        rcr     bx,1
        sar     bp,1
        rcr     bx,1
        mov     di,ax
        mov     si,word ptr ds:[1Ah]
        mov     ax,word ptr ds:[1Ch]
        cwd
        xor     si,dx
        xor     ax,dx
        neg     dx
        add     cx,dx
        adc     ax,0
        cmp     ax,di
        jl      short L047A
        jg      short L046C
        cmp     si,cx
        jl      short L047A
L046C:
        add     bx,cx
        adc     bp,di
        sar     bp,1
        rcr     bx,1
        sar     bp,1
        rcr     bx,1
        jmp     short L0496
L047A:
        sar     ax,1
        rcr     si,1
        cmp     ax,bp
        jl      short L048E
        jg      short L0488
        cmp     si,bx
        jl      short L048E
L0488:
        sar     bp,1
        rcr     bx,1
        jmp     short L0492
L048E:
        sar     ax,1
        rcr     si,1
L0492:
        add     si,cx
        adc     ax,di
L0496:
        add     bx,si
        adc     bp,ax
        pop     si
        ret
_get_dist endp

; seg004_0849_49C  (+49C)
do_obj proc near
        mov     word ptr ds:[25E2h],0
        lodsw
        mov     di,ax
        add     di,si
        lodsw
        mov     word ptr ds:[24C8h],ax
        mov     cx,ax
        lodsw
        mov     bp,ax
        lodsw
        sub     ax,word ptr ds:[247Ah]
        mov     word ptr ds:[16h],ax
        xchg    bx,ax
        lodsw
        xchg    bx,ax
        sbb     bx,word ptr ds:[247Ch]
        mov     word ptr ds:[18h],bx
        jcxz    L04D9
        or      cx,cx
        jns     short L04D3
        neg     cx
L04CB:
        shl     ax,1
        rcl     bx,1
        loop    L04CB
        jmp     short L04D9
L04D3:
        sar     bx,1
        rcr     ax,1
        loop    L04D3
L04D9:
        cwd
        cmp     bx,dx
        je      short L04E1
        jmp     L05E3
L04E1:
        neg     ax
        mov     word ptr ds:[25E6h],ax
        cwd
        xor     ax,dx
        sub     ax,dx
        add     bp,ax
        jno     short L04F2
        jmp     L05E3
L04F2:
        lodsw
        mov     dx,ax
        lodsw
        sub     ax,word ptr ds:[247Eh]
        mov     word ptr ds:[1Ah],ax
        xchg    bx,ax
        lodsw
        xchg    bx,ax
        sbb     bx,word ptr ds:[2480h]
        mov     word ptr ds:[1Ch],bx
        mov     cx,word ptr ds:[24C8h]
        jcxz    L0522
        or      cx,cx
        jns     short L051C
        neg     cx
L0514:
        shl     ax,1
        rcl     bx,1
        loop    L0514
        jmp     short L0522
L051C:
        sar     bx,1
        rcr     ax,1
        loop    L051C
L0522:
        mov     cx,dx
        cwd
        cmp     bx,dx
        je      short L052C
        jmp     L05E3
L052C:
        neg     ax
        mov     word ptr ds:[25E8h],ax
        cwd
        xor     ax,dx
        sub     ax,dx
        add     ax,cx
        jno     short L053D
        jmp     L05E3
L053D:
        or      bp,ax
        lodsw
        mov     dx,ax
        lodsw
        sub     ax,word ptr ds:[2482h]
        mov     word ptr ds:[1Eh],ax
        xchg    bx,ax
        lodsw
        xchg    bx,ax
        sbb     bx,word ptr ds:[2484h]
        mov     word ptr ds:[20h],bx
        mov     cx,word ptr ds:[24C8h]
        jcxz    L056F
        or      cx,cx
        jns     short L0569
        neg     cx
L0561:
        shl     ax,1
        rcl     bx,1
        loop    L0561
        jmp     short L056F
L0569:
        sar     bx,1
        rcr     ax,1
        loop    L0569
L056F:
        mov     cx,dx
        cwd
        cmp     bx,dx
        jne     short L05E3
        neg     ax
        mov     word ptr ds:[25EAh],ax
        cwd
        xor     ax,dx
        sub     ax,dx
        add     ax,cx
        jo      short L05E3
        or      bp,ax
        lodsw
        push    si
        push    di
        xchg    si,ax
        mov     ax,si
modify_axis2:
        add     ax,word ptr ds:[25E6h]
        js      short L05F1
        mov     word ptr ds:[4],0
        mov     ax,bp
        mov     bx,22h
        or      ah,ah
        je      short L05A6
        xchg    al,ah
        xlatb
        jmp     short L05A9
L05A6:
        xlatb
        add     al,8
L05A9:
        mov     byte ptr ds:[25E0h],al
        or      bp,si
        mov     ax,bp
        mov     bx,22h
        or      ah,ah
        je      short L05BC
        xchg    al,ah
        xlatb
        jmp     short L05BF
L05BC:
        xlatb
        add     al,8
L05BF:
        mov     cl,al
        mov     bx,word ptr ds:[25E6h]
        mov     bp,word ptr ds:[25EAh]
        mov     ax,word ptr ds:[25E8h]
        call    _sphere_check
        pop     di
        jb      short L05F2
        mov     word ptr ds:[25FAh],ax
        call    _self_modify
        xchg    si,ax
        call    _set_accept
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
L05E3:
        mov     word ptr ds:[25E2h],0FFFFh
        mov     si,di
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
L05F1:
        pop     di
L05F2:
        pop     si
        mov     si,di
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
do_obj endp

SEG004_TEXT ends

        end
