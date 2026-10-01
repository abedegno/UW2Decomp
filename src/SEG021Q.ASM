; /* target: seg021_22FD_C20 */
; /* opts: /ml */
;
; The far C entry points into the 3D code (cZoom, cFBtoScreen, cFillFB, cPlaceFB, cRender,
; cInit3d, cFstSinCos, cSinCos, cAtan2, cSqRt, cFrmtoRaw), which switch to the private stack
; whose words open the module. Its _DATA is the far pointers cPlayer, cDbase, cEntryStrt,
; cLightTabs, cDbbase, cPixXferStuff and their neighbours.
;
; SEG021 split into its original modules: this is module 17 of 17, +C20..+1071 of the segment
; (file 24920h), a module of the second library (UWLIB.LIB).
; Procs: cZoom, cFBtoScreen, cFillFB, CallbackFunctionSleepRelated_seg,
; Callback_seg021_22FD_CEA, cPlaceFB, cRender, cInit3d, cFstSinCos, cSinCos, cAtan2, cSqRt,
; cFrmtoRaw, seg021_22FD_FDD, seg021_22FD_1004, seg021_22FD_102B.
; Evidence for the bounds: starts on a paragraph after 15 zero bytes following a retf: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros). The 15 zero bytes after its last routine (to 1080h, which the segment table counts
; as seg021's) are padding before a later empty paragraph-aligned contribution to the segment
; (see SEG004N.ASM).
; Its relocations are entries 2381-2404 of UW2.EXE's relocation table.
; Its _DATA is DS:2164..2194 (relocation entries 2372-2380), defined before the code because
; those entries come before the code's.

        .186

        extrn   _DRAW_RELATED_seg003_0272_7F9:far
        extrn   _MousQUp:far
        extrn   _render_3d:far
        extrn   _seg003_0272_18:far
        extrn   _seg003_0272_5363:far
        extrn   _seg003_0272_6E2:far
        extrn   _seg003_0272_6E4:far
        extrn   _seg003_0272_764:far
        extrn   _seg003_0272_788:far
        extrn   _seg003_0272_7A8:far
        extrn   _seg004_0849_7F0C:far
        extrn   _seg_5DFD:far
        extrn   _update_cache:far
        extrn   dseg062_62a6:far
        extrn   lightabs:far
        extrn   seg052_519C:far
        extrn   seg_370D:far

_DATA   segment word public 'DATA'
        dd      _seg_5DFD
_cPlayer label dword
        dd      dseg062_62a6+0A40h
_cDbase label dword
        dd      seg052_519C+26A4h
_cEntryStrt label dword
        dd      seg052_519C+8B7Ch
        dd      seg052_519C+0C7FEh
_cLightTabs label dword
        dd      lightabs
        dd      seg_370D+0B20h
_cDbbase label dword
        dd      seg052_519C+14CCh
_cPixXferStuff label dword
        dd      seg_370D+0B08h
        db      0Ch dup (0)
        extrn   _joy_position:dword             ; SEG021M's, DS:211C: the top of its private stack
_DATA   ends
DGROUP  group   _DATA

SEG021_TEXT segment para public 'CODE'
        assume  cs:SEG021_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _MaybeTangent_seg021_22FD_BD4:near
        extrn   _seg021_22FD_710:near
        extrn   _seg021_22FD_712:near
        extrn   _seg021_22FD_A38:near
        extrn   _seg021_22FD_A69:near
        extrn   _SquareRoot_seg021_22FD_A78:near
        public  _cDbase
        public  _cDbbase
        public  _cEntryStrt
        public  _cLightTabs
        public  _cPixXferStuff
        public  _cPlayer
        public  _Callback_seg021_22FD_CEA
        public  _CallbackFunctionSleepRelated_seg
        public  _cAtan2
        public  _cFBtoScreen
        public  _cFillFB
        public  _cFrmtoRaw
        public  _cFstSinCos
        public  _cInit3d
        public  _cPlaceFB
        public  _cRender
        public  _cSinCos
        public  _cSqRt
        public  _cZoom
        public  _seg021_22FD_1004
        public  _seg021_22FD_102B
        public  _seg021_22FD_FDD

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
        mov     ax,word ptr cs:_seg021_22FD_710
        mov     word ptr cs:L0C28,ax
        mov     ax,word ptr cs:_seg021_22FD_712
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
        call    far ptr _render_3d
        call    far ptr _seg003_0272_18
        call    far ptr _update_cache
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
        mov     ax,word ptr cs:_seg021_22FD_710
        sub     ax,word ptr cs:L0C28
        mov     dx,word ptr cs:_seg021_22FD_712
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
        mov     word ptr cs:_seg021_22FD_710,0
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
        mov     ax,DGROUP
        mov     ds,ax
        mov     es,ax
        mov     ax,ss
        mov     word ptr cs:L0C30,ax
        mov     word ptr cs:L0C32,sp
        mov     ax,offset DGROUP:_joy_position  ; the 0CEh bytes of SEG021M's _DATA below it are the stack
        cli
        mov     sp,ax
        mov     ax,DGROUP
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

SEG021_TEXT ends

        end
