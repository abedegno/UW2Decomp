; /* target: seg019_C20 */
; /* opts: /ml */
;
; UW1: UW2's module (src/sys in UW2Decomp), matched to UW.EXE's seg019; the differences are
; marked UW1/UW2 where they occur. Other remarks are UW2's unless they say UW1.
; UW1's far calls (UW2's in brackets): seg003 B5D (_7F9), A48 (_6E4), AC8 (_764), AEC (_788),
; B0C (_7A8), A46 (_6E2), 18 (_18), 5B5D (_5363); seg004 4987 render_3d, 7B3D update_cache,
; 7B37 (seg004 _7F0C). The comments below give UW2's module offsets.
;
; The far C entry points into the 3D code and the integer maths: cZoom, cFBtoScreen, cFillFB,
; cDimFB and cLiteFB (the two Callback_ procs), cPlaceFB, cRender, cInit3d, cFstSinCos, cSinCos,
; cAtan2, cSqRt and cFrmtoRaw (FM Towns names, which FM Towns has together). VIEW3D.C and the
; rest of the C call them. Each switches to the private stack in seg063 (SP 448h, UW2 510h;
; the caller's SS:SP saved in the words that open the module), loads the arguments into
; registers and calls seg003, seg004 or this segment's own maths (IMATH.ASM). Its _DATA is the
; far pointers C uses to reach the 3D data: cPlayer (seg063:0970, the camera), cDbase (the render
; database, seg051:2938), cEntryStrt, cLightTabs (lightabs, PGCACHE.ASM), cDbbase (DbEntry,
; seg051:161C), cPixXferStuff and their neighbours (UW2's offsets beside each pointer below;
; UW1's seg051, seg048 and seg052 are UW2's seg052_519C, seg_370D and seg_5DFD).
;
; cRender is the frame: see its comment. _102B lets the renderer call the C routine MousQUp in
; the middle of a frame (seg004's do_mouseq opcode): because MousQUp may itself come back into
; seg019 through an entry point, which would reset SP to 448h and overwrite the frame's stack,
; _102B first copies the used part of the private stack away (_FDD) and runs MousQUp on a small
; DGROUP stack, then copies it back (_1004).
;
; name: descriptive (module 17 of seg019, "far C entry points into the 3D code (cRender, cInit3d
; ...)"). The c- names are FM Towns'.
;
; seg019 split into its original modules, as UW2's seg021: module 17 of 17, +C20..+1071
; of the segment (file 231C0h), a module of the second library (UWLIB.LIB).
; Procs: cZoom, cFBtoScreen, cFillFB, seg019_CB7,
; seg019_CEA, cPlaceFB, cRender, cInit3d, cFstSinCos, cSinCos, cAtan2, cSqRt,
; cFrmtoRaw, seg019_FDD, seg019_1004, seg019_102B.
; Evidence for the bounds: starts on a paragraph after 15 zero bytes following a retf: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros). The 15 zero bytes after its last routine (to 1080h, which the segment table counts
; as seg019's) are padding before a later empty paragraph-aligned contribution to the segment
; (see PGCACHE.ASM).
; Its relocations are entries 2676-2699 of UW.EXE's relocation table.
; Its _DATA is DS:2370..23A4 (relocation entries 2667-2675; UW2 DS:2164..2194), defined before the code because
; those entries come before the code's.

; The DOS (int 21h) functions used here.
DOS_SET_VECTOR  equ     25h

        .186

        extrn   _seg003_B5D:far
        extrn   _MousQUp:far
        extrn   _render_3d:far
        extrn   _seg003_18:far
        extrn   _seg003_5B5D:far
        extrn   _seg003_A46:far
        extrn   _seg003_A48:far
        extrn   _seg003_AC8:far
        extrn   _seg003_AEC:far
        extrn   _seg003_B0C:far
        extrn   _seg004_7B37:far
        extrn   _seg_5DFD:far
        extrn   _update_cache:far
        extrn   seg063:far
        extrn   lightabs:far
        extrn   seg051:far
        extrn   seg048:far

_DATA   segment word public 'DATA'
        ; UW1 only: an unrelocated zero doubleword that nothing references (DS:2370), between
        ; SYSENTRY.ASM's cExitMessage and _seg_5DFD's pointer; UW2 has none. Which module owned
        ; it is not known (no relocation, no reference); it is placed here.
        dd      0
        dd      _seg_5DFD
_cPlayer label dword
        dd      seg063+970h                     ; UW2 0A40h
_cDbase label dword
        dd      seg051+2938h                    ; UW2 26A4h
_cEntryStrt label dword
        dd      seg051+73F2h                    ; UW2 8B7Ch
        dd      seg051+0B07Ch                   ; UW2 0C7FEh
_cLightTabs label dword
        dd      lightabs
        dd      seg048+0B2Ah                    ; UW2 seg_370D+0B20h
_cDbbase label dword
        dd      seg051+161Ch                    ; UW2 14CCh
_cPixXferStuff label dword
        dd      seg048+0B12h                    ; UW2 seg_370D+0B08h
        db      0Ch dup (0)
        extrn   _joy_position:dword             ; SYSENTRY's, DS:232C: the top of its private stack
_DATA   ends
DGROUP  group   _DATA

SEG019_TEXT segment para public 'CODE'
        assume  cs:SEG019_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _MaybeTangent_seg019_BD4:near
        extrn   _seg019_710:near
        extrn   _seg019_712:near
        extrn   _seg019_A38:near
        extrn   _seg019_A69:near
        extrn   _SquareRoot_seg019_A78:near
        public  _cDbase
        public  _cDbbase
        public  _cEntryStrt
        public  _cLightTabs
        public  _cPixXferStuff
        public  _cPlayer
        public  _seg019_CEA
        public  _seg019_CB7
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
        public  _seg019_1004
        public  _seg019_102B
        public  _seg019_FDD

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

; seg019_C34  (+C34)
; cZoom(zoom): store the zoom word in the render database's header (the word after the address
; at seg051:0104; UW2 0158).
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
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        mov     bx,seg seg051
        mov     es,bx
        mov     di,word ptr es:[104h]                 ; UW2 158h
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

; seg019_C7E  (+C7E)
; cFBtoScreen: copy the 3D frame buffer to the screen (GRENTRY.ASM's _7F9, which switches stacks
; itself).
_cFBtoScreen proc far
        call    far ptr _seg003_B5D
        retf
_cFBtoScreen endp

; seg019_C84  (+C84)
; cFillFB(colour): fill the frame buffer (GRENTRY.ASM's fill_fbuf).
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
        call    far ptr _seg003_A48
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

; seg019_CB7  (+CB7)
; cDimFB(shade) (FM Towns; the label is an old guess): darken the frame buffer through a
; lightabs row (GRENTRY.ASM's _764). name: cDimFB is FM Towns' name for the routine at this
; place; the proc keeps the label it was matched under.
_seg019_CB7 proc far
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
        call    far ptr _seg003_AC8
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
_seg019_CB7 endp

; seg019_CEA  (+CEA)
; cLiteFB(count) (FM Towns): remap the frame buffer through cXfer table 3, count times
; (GRENTRY.ASM's _788). name: cLiteFB is FM Towns' name; the proc keeps the label it was matched
; under.
_seg019_CEA proc far
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
        call    far ptr _seg003_AEC
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
_seg019_CEA endp

; seg019_D1D  (+D1D)
; cPlaceFB(x, y, width, height): put the 3D view's frame at (x, y) on the screen, y counting up:
; the screen offset (0C7h - y) * 80 + x / 4 goes to seg048:0950 (UW2 seg_370D:094E), then GRENTRY.ASM's
; setup_frame_buf(width, height).
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
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,word ptr ds:[5586h]                ; UW2 5588h
        sti
        mov     word ptr ds:[950h],ax             ; UW2 94Eh
        call    far ptr _seg003_B0C
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

; seg019_D7A  (+D7A)
; cRender: draw one 3D frame. Switches to the private stack, notes the game clock, points int 0
; at seg063:04D0 (UW2 05A0; the renderer's overflow handler, INSTANCE.ASM), then calls GRENTRY's clear_fbuf,
; INSTANCE's render_3d, GRMISC's _18 and PGCACHE's update_cache; restores the stack and the int
; 0 vector. Returns DX:AX = the clock ticks the frame took.
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
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        mov     ax,word ptr cs:_seg019_710
        mov     word ptr cs:L0C28,ax
        mov     ax,word ptr cs:_seg019_712
        mov     word ptr cs:L0C2A,ax
        mov     bx,0
        mov     es,bx
        mov     ax,word ptr es:[bx]
        mov     word ptr cs:L0C2C,ax
        mov     ax,word ptr es:[bx+2]
        mov     word ptr cs:L0C2E,ax
        mov     dx,4D0h                         ; UW2 5A0h
        mov     ax,DOS_SET_VECTOR shl 8
        int     21h
        call    far ptr _seg003_A46
        call    far ptr _render_3d
        call    far ptr _seg003_18
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
        mov     ax,word ptr cs:_seg019_710
        sub     ax,word ptr cs:L0C28
        mov     dx,word ptr cs:_seg019_712
        sub     dx,word ptr cs:L0C2A
        pop     bp
        retf
_cRender endp

; seg019_E1A  (+E1A)
; cInit3d: zero the game clock's low word and set the clip window from the record seg063:09C8
; (UW2 0A98) points at (GRDISP.ASM's _5363).
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
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        mov     word ptr cs:_seg019_710,0
        mov     si,word ptr ds:[9C8h]                 ; UW2 0A98h
        call    far ptr _seg003_5B5D
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

; GetVectorForDirection_seg019_E63  (+E63)
; cFstSinCos(angle, &sin, &cos) (FM Towns; the label is an old guess): the table sine and cosine
; (IMATH.ASM's _A69).
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
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        call    _seg019_A69
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

; SomethingProjectileHeading_seg019_EAE  (+EAE)
; cSinCos(angle, &sin, &cos): the interpolated sine and cosine (IMATH.ASM's _A38).
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
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        push    bp
        call    _seg019_A38
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

; MaybeGetTangent_seg019_EFB  (+EFB)
; cAtan2(sin, cos): the angle of a unit vector (IMATH.ASM's _BD4).
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
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        call    _MaybeTangent_seg019_BD4
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

; GetSquareRoot_seg019_F3F  (+F3F)
; cSqRt(long): the square root of a 32-bit value (IMATH.ASM's _A78).
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
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        call    _SquareRoot_seg019_A78
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
; cFrmtoRaw(a, b, source offset, source segment, format): decode a picture in one of the .GR/.CR
; formats to one byte per pixel through PGCACHE.ASM's far stub to the decoders (EXPAND.ASM). The
; first two arguments go to BP and AX, where the decoders take their destination and palette
; arguments (EXPAND.ASM has the details); DH is set to 0FFh.
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
        mov     dx,seg seg063
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,448h
        sti
        mov     ds,cx
        mov     dh,0FFh
        call    far ptr _seg004_7B37
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

; seg019_FDD  (+FDD)
; _FDD: copy the used part of the private stack (SP up to 448h) to just below seg063:0A92 (UW2
; 510h, 0C2A), and
; keep the caller's saved SS:SP (L0C24) in L0C20, so a nested entry cannot overwrite either.
_seg019_FDD proc near
        mov     ax,ss
        mov     ds,ax
        mov     si,448h
        mov     cx,si
        sub     cx,sp
        shr     cx,1
        mov     es,ax
        mov     di,0A92h                        ; UW2 0C2Ah
        std
        rep movsw
        cld
        mov     ax,word ptr cs:L0C24
        mov     word ptr cs:L0C20,ax
        mov     ax,word ptr cs:L0C26
        mov     word ptr cs:L0C22,ax
        ret
_seg019_FDD endp

; seg019_1004  (+1004)
; _1004: copy the private stack back from the save area and restore L0C24 from L0C20.
_seg019_1004 proc near
        mov     ax,ss
        mov     ds,ax
        mov     si,0A92h                        ; UW2 0C2Ah
        mov     di,448h
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
_seg019_1004 endp

; seg019_102B  (+102B)
; _102B: called by seg004's do_mouseq (PGCACHE.ASM) during a frame. Saves the private stack
; (_FDD), switches to DS = ES = DGROUP and the small stack below joy_position in DGROUP, calls
; MousQUp(1), switches back and restores the private stack (_1004).
_seg019_102B proc far
        push    si
        push    ds
        push    es
        call    _seg019_FDD
        mov     ax,DGROUP
        mov     ds,ax
        mov     es,ax
        mov     ax,ss
        mov     word ptr cs:L0C30,ax
        mov     word ptr cs:L0C32,sp
        mov     ax,offset DGROUP:_joy_position  ; the 0D4h bytes of SYSENTRY's _DATA below it are the stack
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
        call    _seg019_1004
        pop     es
        pop     ds
        pop     si
        retf
_seg019_102B endp

SEG019_TEXT ends

        end
