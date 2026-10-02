; /* target: seg004_0849_600 */
; /* opts: /ml */
;
; smooth_closure and smooth_over.
;
; What it does in the game: Gouraud-shaded polygons for the 3D view. INTERP.ASM's do_closure
; jumps to smooth_closure when the current fill routine (ds:14A6h) is seg003's Gouraud
; routine, which do_goursurf, do_uwsurf and do_transsurf select. smooth_closure is
; do_closure with a shade per vertex: it projects the vertex buffer, writes each vertex's
; shade byte (point +7, set by do_setshade or do_uwshade) beside it, and calls seg003's
; polygon routine; its clippers (seg004_0849_7FC, 92E, A64, B98, the counterparts of
; INTERP's clip_top, clip_bot, clip_right, clip_left) interpolate the shade at each new
; vertex as well as the position. smooth_over is a separate routine that nothing in the EXE
; calls.
;
; Data: none of its own apart from L0D7F, the word the divide overflow handler saves BP in.
; Output goes to seg_370D: the projected vertices at 415Eh and their shades at 55EEh, one
; word apart.
;
; name: descriptive (map/filenames.tsv: "3D library module 3: smooth_closure, smooth_over");
; the two names are FM Towns'.
;
; SEG004 split into its original modules: this is module 3 of 14, +600..+DB0 of the segment
; (file 97C0h), a module of the second library (UWLIB.LIB).
; Procs: smooth_closure, seg004_0849_798, seg004_0849_7FC, seg004_0849_92E, seg004_0849_A64,
; seg004_0849_B98, smooth_over, seg004_0849_D83.
; match: evidence for the bounds: starts on a paragraph after 5 zero bytes following a jmp:
; TLINK's padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90,
; not zeros).
; Its relocations are entries 2319-2327 of UW2.EXE's relocation table.

; The model interpreter's opcode table: a word per opcode, the offset of its handler
; (INTERP.ASM), in seg052_519C, the data segment while interpreting.
OPCODE_TABLE    equ     24F4h

        .386

        extrn   _seg003_0272_5311:far
        extrn   seg003_0272:far
        extrn   seg052_519C:far
        extrn   seg_370D:far
        extrn   _seg003_0272_5959:far

_DATA   segment word public use16 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public use16 'CODE'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _code_pnt:near
        public  _seg004_0849_644
        public  _seg004_0849_64F
        public  _seg004_0849_6B2
        public  _seg004_0849_6C2
        public  _seg004_0849_798
        public  _seg004_0849_7FC
        public  _seg004_0849_92E
        public  _seg004_0849_A64
        public  _seg004_0849_B98
        public  _seg004_0849_D83
        public  _smooth_closure
        public  _smooth_over

; seg004_0849_600  (+600)
; Draws the Gouraud polygon in the vertex buffer (ds:1B8h..1B6h). Raises int 2 if the buffer
; has run past 0B1Ah. Codes AND nonzero: skipped. Codes OR zero: projected (the add
; immediates at seg004_0849_644 and 64F are the screen centre, patched by render_3d) with
; the shades copied to seg_370D:55EEh, then drawn with BP = ds:[14A8h]. Codes OR with bit 80h
; (behind the eye): clipped against top, bottom, left and right in turn, retrying after each.
; Otherwise projected with overflow checks (immediates at 6B2 and 6C2), drawn with BP =
; ds:[14A6h], or clipped on overflow (L079C). Continues with the next opcode.
_smooth_closure proc near
        cmp     word ptr ds:[1B6h],0B1Ah
        jle     short L060A
        int     2
L060A:
        mov     word ptr ds:[25E4h],0
        test    byte ptr ds:[14CBh],0FFh
        je      short L061A
        jmp     L0792
L061A:
        mov     di,1BAh
        mov     word ptr ds:[1B8h],di
        test    byte ptr ds:[14CAh],0FFh
        jne     short L068D
L0628:
        push    si
        mov     ax,seg seg_370D
        mov     es,ax
        mov     di,415Eh
        mov     bx,55EEh
        mov     si,word ptr ds:[1B8h]
        mov     cx,word ptr ds:[2472h]
L063C:
        lodsw
        mov     bp,word ptr [si+2]
        imul    cx
        idiv    bp
_seg004_0849_644:
        add     ax,1234h
        stosw
        lodsw
        imul    word ptr ds:[2470h]
        idiv    bp
_seg004_0849_64F:
        add     ax,1234h
        stosw
        mov     al,byte ptr [si+3]
        mov     byte ptr es:[bx],al
        add     si,4
        add     bx,2
        cmp     si,word ptr ds:[1B6h]
        jne     L063C
        mov     bx,word ptr ds:[1B6h]
        sub     bx,word ptr ds:[1B8h]
        shr     bx,1
        shr     bx,1
        shr     bx,1
        mov     cx,bx
        inc     word ptr ds:[25E4h]
        mov     bp,word ptr ds:[14A8h]
        call    far ptr _seg003_0272_5311
        mov     ax,ds
        mov     es,ax
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L068D:
        js      short L0703
        push    si
L0690:
        mov     word ptr ss:[5A5h],offset _seg004_0849_798
        mov     ax,seg seg_370D
        mov     es,ax
        mov     di,415Eh
        mov     bx,55EEh
        mov     si,word ptr ds:[1B8h]
        mov     cx,word ptr ds:[2472h]
L06AA:
        lodsw
        mov     bp,word ptr [si+2]
        imul    cx
        idiv    bp
_seg004_0849_6B2:
        add     ax,1234h
        jno     short L06BA
        jmp     L079C
L06BA:
        stosw
        lodsw
        imul    word ptr ds:[2470h]
        idiv    bp
_seg004_0849_6C2:
        add     ax,1234h
        jno     short L06CA
        jmp     L079C
L06CA:
        stosw
        mov     al,byte ptr [si+3]
        mov     byte ptr es:[bx],al
        add     si,4
        add     bx,2
        cmp     si,word ptr ds:[1B6h]
        jne     L06AA
        mov     cx,word ptr ds:[1B6h]
        sub     cx,word ptr ds:[1B8h]
        shr     cx,1
        shr     cx,1
        shr     cx,1
        inc     word ptr ds:[25E4h]
        mov     bp,word ptr ds:[14A6h]
        call    far ptr _seg003_0272_5311
        mov     ax,ds
        mov     es,ax
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L0703:
        mov     word ptr ss:[5A5h],offset _seg004_0849_D83
        test    byte ptr ds:[14CAh],4
        je      short L072C
        push    si
        call    _seg004_0849_7FC
        pop     si
        test    byte ptr ds:[14CBh],0FFh
        jne     short L0792
        test    byte ptr ds:[14CAh],0FFh
        js      short L072C
        jne     short L0729
        jmp     L0628
L0729:
        jmp     L068D
L072C:
        test    byte ptr ds:[14CAh],8
        je      short L074E
        push    si
        call    _seg004_0849_92E
        pop     si
        test    byte ptr ds:[14CBh],0FFh
        jne     short L0792
        test    byte ptr ds:[14CAh],0FFh
        js      short L074E
        jne     short L074B
        jmp     L0628
L074B:
        jmp     L068D
L074E:
        test    byte ptr ds:[14CAh],1
        je      short L0770
        push    si
        call    _seg004_0849_B98
        pop     si
        test    byte ptr ds:[14CBh],0FFh
        jne     short L0792
        test    byte ptr ds:[14CAh],0FFh
        js      short L0770
        jne     short L076D
        jmp     L0628
L076D:
        jmp     L068D
L0770:
        test    byte ptr ds:[14CAh],2
        je      short L0792
        push    si
        call    _seg004_0849_A64
        pop     si
        test    byte ptr ds:[14CBh],0FFh
        jne     short L0792
        test    byte ptr ds:[14CAh],0FFh
        js      short L0792
        jne     short L078F
        jmp     L0628
L078F:
        jmp     L068D
L0792:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_smooth_closure endp

; seg004_0849_798  (+798)
; Recovery entry for the divide overflow during smooth_closure's checked projection (it is
; stored at ss:[5A5h]): drops the int 0 frame, then (L079C) clips against right, left, top
; and bottom as the codes require and projects again; gives up when nothing is left,
; restoring DS and ES to seg052_519C. The counterpart of INTERP's clip_overflow and
; do_3d_clip.
_seg004_0849_798 proc near
        sti
        add     sp,6
L079C:
        mov     word ptr ss:[5A5h],offset _seg004_0849_D83
        mov     ax,ds
        mov     es,ax
        test    byte ptr ds:[14CAh],2
        je      short L07B8
        call    _seg004_0849_A64
        test    byte ptr ds:[14CBh],0FFh
        jne     short L07EE
L07B8:
        test    byte ptr ds:[14CAh],1
        je      short L07C9
        call    _seg004_0849_B98
        test    byte ptr ds:[14CBh],0FFh
        jne     short L07EE
L07C9:
        test    byte ptr ds:[14CAh],4
        je      short L07DA
        call    _seg004_0849_7FC
        test    byte ptr ds:[14CBh],0FFh
        jne     short L07EE
L07DA:
        test    byte ptr ds:[14CAh],8
        je      short L07EB
        call    _seg004_0849_92E
        test    byte ptr ds:[14CBh],0FFh
        jne     short L07EE
L07EB:
        jmp     L0690
L07EE:
        pop     si
        mov     ax,seg seg052_519C
        mov     ds,ax
        mov     es,ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_0849_798 endp

; seg004_0849_7FC  (+7FC)
; Clips the Gouraud polygon against y = z (code bit 4), like INTERP's clip_top, also
; interpolating each new vertex's shade (+7).
_seg004_0849_7FC proc near
        mov     byte ptr ds:[14CAh],0
        mov     byte ptr ds:[14CBh],0FFh
        mov     si,word ptr ds:[1B8h]
        mov     di,word ptr ds:[1B6h]
        mov     cx,8
        rep movsw
        add     word ptr ds:[1B6h],8
        mov     si,word ptr ds:[1B8h]
        mov     di,0B1Ah
        cmp     si,1BAh
        je      short L0828
        mov     di,1BAh
L0828:
        mov     word ptr ds:[1B8h],di
L082C:
        add     si,8
L082F:
        cmp     si,word ptr ds:[1B6h]
        jne     short L0838
        jmp     L0929
L0838:
        mov     al,byte ptr [si+6]
        test    al,4
        jne     short L084E
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        mov     cx,4
        rep movsw
        jmp     L082F
L084E:
        test    byte ptr [si-2],4
        jne     short L08B9
        mov     bp,word ptr [si-4]
        sub     bp,word ptr [si-6]
        mov     cx,bp
        sub     cx,word ptr [si+4]
        add     cx,word ptr [si+2]
        mov     al,byte ptr [si+7]
        sub     al,byte ptr [si+0Fh]
        cbw
        imul    bp
        idiv    cx
        add     al,byte ptr [si+0Fh]
        mov     byte ptr es:[di+7],al
        mov     ax,word ptr [si]
        sub     ax,word ptr [si-8]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L0888
        cwd
        adc     ax,dx
L0888:
        add     ax,word ptr [si-8]
        stosw
        mov     bx,ax
        mov     ax,word ptr [si+2]
        sub     ax,word ptr [si-6]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L08A3
        cwd
        adc     ax,dx
L08A3:
        add     ax,word ptr [si-6]
        stosw
        stosw
        mov     cx,ax
        mov     bp,ax
        call    _code_pnt
        stosb
        inc     di
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
L08B9:
        test    byte ptr [si+0Eh],4
        je      short L08C2
        jmp     L082C
L08C2:
        mov     bp,word ptr [si+4]
        sub     bp,word ptr [si+2]
        mov     cx,bp
        add     cx,word ptr [si+0Ah]
        sub     cx,word ptr [si+0Ch]
        mov     al,byte ptr [si+7]
        sub     al,byte ptr [si+0Fh]
        cbw
        imul    bp
        idiv    cx
        add     al,byte ptr [si+0Fh]
        mov     byte ptr es:[di+7],al
        mov     ax,word ptr [si+8]
        sub     ax,word ptr [si]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L08F6
        cwd
        adc     ax,dx
L08F6:
        add     ax,word ptr [si]
        stosw
        mov     bx,ax
        mov     ax,word ptr [si+0Ah]
        sub     ax,word ptr [si+2]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L0910
        cwd
        adc     ax,dx
L0910:
        add     ax,word ptr [si+2]
        stosw
        stosw
        mov     cx,ax
        mov     bp,ax
        call    _code_pnt
        stosb
        inc     di
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        jmp     L082C
L0929:
        mov     word ptr ds:[1B6h],di
        ret
_seg004_0849_7FC endp

; seg004_0849_92E  (+92E)
; Gouraud clip against y = -z (code bit 8), like clip_bot.
_seg004_0849_92E proc near
        mov     byte ptr ds:[14CAh],0
        mov     byte ptr ds:[14CBh],0FFh
        mov     si,word ptr ds:[1B8h]
        mov     di,word ptr ds:[1B6h]
        mov     cx,8
        rep movsw
        add     word ptr ds:[1B6h],8
        mov     si,word ptr ds:[1B8h]
        mov     di,0B1Ah
        cmp     si,1BAh
        je      short L095A
        mov     di,1BAh
L095A:
        mov     word ptr ds:[1B8h],di
L095E:
        add     si,8
L0961:
        cmp     si,word ptr ds:[1B6h]
        jne     short L096A
        jmp     L0A5F
L096A:
        mov     al,byte ptr [si+6]
        test    al,8
        jne     short L0980
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        mov     cx,4
        rep movsw
        jmp     L0961
L0980:
        test    byte ptr [si-2],8
        jne     short L09ED
        mov     bp,word ptr [si-4]
        add     bp,word ptr [si-6]
        mov     cx,bp
        sub     cx,word ptr [si+4]
        sub     cx,word ptr [si+2]
        mov     al,byte ptr [si+7]
        sub     al,byte ptr [si+0Fh]
        cbw
        imul    bp
        idiv    cx
        add     al,byte ptr [si+0Fh]
        mov     byte ptr es:[di+7],al
        mov     ax,word ptr [si]
        sub     ax,word ptr [si-8]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L09BA
        cwd
        adc     ax,dx
L09BA:
        add     ax,word ptr [si-8]
        stosw
        mov     bx,ax
        mov     ax,word ptr [si+2]
        sub     ax,word ptr [si-6]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L09D5
        cwd
        adc     ax,dx
L09D5:
        add     ax,word ptr [si-6]
        stosw
        mov     cx,ax
        neg     ax
        stosw
        mov     bp,ax
        call    _code_pnt
        stosb
        inc     di
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
L09ED:
        test    byte ptr [si+0Eh],8
        je      short L09F6
        jmp     L095E
L09F6:
        mov     bp,word ptr [si+4]
        add     bp,word ptr [si+2]
        mov     cx,bp
        sub     cx,word ptr [si+0Ah]
        sub     cx,word ptr [si+0Ch]
        mov     al,byte ptr [si+7]
        sub     al,byte ptr [si+0Fh]
        cbw
        imul    bp
        idiv    cx
        add     al,byte ptr [si+0Fh]
        mov     byte ptr es:[di+7],al
        mov     ax,word ptr [si+8]
        sub     ax,word ptr [si]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L0A2A
        cwd
        adc     ax,dx
L0A2A:
        add     ax,word ptr [si]
        stosw
        mov     bx,ax
        mov     ax,word ptr [si+0Ah]
        sub     ax,word ptr [si+2]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L0A44
        cwd
        adc     ax,dx
L0A44:
        add     ax,word ptr [si+2]
        stosw
        mov     cx,ax
        neg     ax
        stosw
        mov     bp,ax
        call    _code_pnt
        stosb
        inc     di
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        jmp     L095E
L0A5F:
        mov     word ptr ds:[1B6h],di
        ret
_seg004_0849_92E endp

; seg004_0849_A64  (+A64)
; Gouraud clip against x = z (code bit 2), like clip_right.
_seg004_0849_A64 proc near
        mov     byte ptr ds:[14CAh],0
        mov     byte ptr ds:[14CBh],0FFh
        mov     si,word ptr ds:[1B8h]
        mov     di,word ptr ds:[1B6h]
        mov     cx,8
        rep movsw
        add     word ptr ds:[1B6h],8
        mov     si,word ptr ds:[1B8h]
        mov     di,0B1Ah
        cmp     si,1BAh
        je      short L0A90
        mov     di,1BAh
L0A90:
        mov     word ptr ds:[1B8h],di
L0A94:
        add     si,8
L0A97:
        cmp     si,word ptr ds:[1B6h]
        jne     short L0AA0
        jmp     L0B93
L0AA0:
        mov     al,byte ptr [si+6]
        test    al,2
        jne     short L0AB6
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        mov     cx,4
        rep movsw
        jmp     L0A97
L0AB6:
        test    byte ptr [si-2],2
        jne     short L0B22
        mov     bp,word ptr [si-8]
        sub     bp,word ptr [si-4]
        mov     cx,bp
        add     cx,word ptr [si+4]
        sub     cx,word ptr [si]
        mov     al,byte ptr [si+7]
        sub     al,byte ptr [si+0Fh]
        cbw
        imul    bp
        idiv    cx
        add     al,byte ptr [si+0Fh]
        mov     byte ptr es:[di+7],al
        mov     ax,word ptr [si]
        sub     ax,word ptr [si-8]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L0AEF
        cwd
        adc     ax,dx
L0AEF:
        add     ax,word ptr [si-8]
        stosw
        mov     bx,ax
        mov     ax,word ptr [si+2]
        sub     ax,word ptr [si-6]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L0B0A
        cwd
        adc     ax,dx
L0B0A:
        add     ax,word ptr [si-6]
        stosw
        mov     cx,ax
        mov     ax,bx
        stosw
        mov     bp,ax
        call    _code_pnt
        stosb
        inc     di
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
L0B22:
        test    byte ptr [si+0Eh],2
        je      short L0B2B
        jmp     L0A94
L0B2B:
        mov     bp,word ptr [si]
        sub     bp,word ptr [si+4]
        mov     cx,bp
        add     cx,word ptr [si+0Ch]
        sub     cx,word ptr [si+8]
        mov     al,byte ptr [si+7]
        sub     al,byte ptr [si+0Fh]
        cbw
        imul    bp
        idiv    cx
        add     al,byte ptr [si+0Fh]
        mov     byte ptr es:[di+7],al
        mov     ax,word ptr [si+8]
        sub     ax,word ptr [si]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L0B5E
        cwd
        adc     ax,dx
L0B5E:
        add     ax,word ptr [si]
        stosw
        mov     bx,ax
        mov     ax,word ptr [si+0Ah]
        sub     ax,word ptr [si+2]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L0B78
        cwd
        adc     ax,dx
L0B78:
        add     ax,word ptr [si+2]
        stosw
        mov     cx,ax
        mov     ax,bx
        stosw
        mov     bp,ax
        call    _code_pnt
        stosb
        inc     di
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        jmp     L0A94
L0B93:
        mov     word ptr ds:[1B6h],di
        ret
_seg004_0849_A64 endp

; seg004_0849_B98  (+B98)
; Gouraud clip against x = -z (code bit 1), like clip_left.
_seg004_0849_B98 proc near
        mov     byte ptr ds:[14CAh],0
        mov     byte ptr ds:[14CBh],0FFh
        mov     si,word ptr ds:[1B8h]
        mov     di,word ptr ds:[1B6h]
        mov     cx,8
        rep movsw
        add     word ptr ds:[1B6h],8
        mov     si,word ptr ds:[1B8h]
        mov     di,0B1Ah
        cmp     si,1BAh
        je      short L0BC4
        mov     di,1BAh
L0BC4:
        mov     word ptr ds:[1B8h],di
L0BC8:
        add     si,8
L0BCB:
        cmp     si,word ptr ds:[1B6h]
        jne     short L0BD4
        jmp     L0CCB
L0BD4:
        mov     al,byte ptr [si+6]
        test    al,1
        jne     short L0BEA
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        mov     cx,4
        rep movsw
        jmp     L0BCB
L0BEA:
        test    byte ptr [si-2],1
        jne     short L0C58
        mov     bp,word ptr [si-4]
        add     bp,word ptr [si-8]
        mov     cx,bp
        sub     cx,word ptr [si+4]
        sub     cx,word ptr [si]
        mov     al,byte ptr [si+7]
        sub     al,byte ptr [si+0Fh]
        cbw
        imul    bp
        idiv    cx
        add     al,byte ptr [si+0Fh]
        mov     byte ptr es:[di+7],al
        mov     ax,word ptr [si]
        sub     ax,word ptr [si-8]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L0C23
        cwd
        adc     ax,dx
L0C23:
        add     ax,word ptr [si-8]
        stosw
        mov     bx,ax
        mov     ax,word ptr [si+2]
        sub     ax,word ptr [si-6]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L0C3E
        cwd
        adc     ax,dx
L0C3E:
        add     ax,word ptr [si-6]
        stosw
        mov     cx,ax
        mov     ax,bx
        neg     ax
        stosw
        mov     bp,ax
        call    _code_pnt
        stosb
        inc     di
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
L0C58:
        test    byte ptr [si+0Eh],1
        je      short L0C61
        jmp     L0BC8
L0C61:
        mov     bp,word ptr [si+4]
        add     bp,word ptr [si]
        mov     cx,bp
        sub     cx,word ptr [si+8]
        sub     cx,word ptr [si+0Ch]
        mov     al,byte ptr [si+7]
        sub     al,byte ptr [si+0Fh]
        cbw
        imul    bp
        idiv    cx
        add     al,byte ptr [si+0Fh]
        mov     byte ptr es:[di+7],al
        mov     ax,word ptr [si+8]
        sub     ax,word ptr [si]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L0C94
        cwd
        adc     ax,dx
L0C94:
        add     ax,word ptr [si]
        stosw
        mov     bx,ax
        mov     ax,word ptr [si+0Ah]
        sub     ax,word ptr [si+2]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L0CAE
        cwd
        adc     ax,dx
L0CAE:
        add     ax,word ptr [si+2]
        stosw
        mov     cx,ax
        mov     ax,bx
        neg     ax
        stosw
        mov     bp,ax
        call    _code_pnt
        stosb
        inc     di
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        jmp     L0BC8
L0CCB:
        mov     word ptr ds:[1B6h],di
        ret
_seg004_0849_B98 endp

; smooth_over  (+CD0): FM smooth_over, the same loop (copy the projected x and y, then
; an intensity from the vertex normal's length)
; In: CX = the vertex count. For each 14-byte record at seg_370D:658h it copies the screen x
; and y to 415Eh, takes the three words at +8 (each >> 5), and computes the length of that
; vector by four Newton steps from 64; the shade is the low bytes of the length and [64Ch]
; multiplied, >> 6, plus [650h] (at least 0) plus [64Eh], at most 15. Then it points seg003's handler pair at 0BC3h/0BC5h
; at 0BF2h/0CD9h (as do_transsurf does) and calls the polygon routine with BP =
; _seg003_0272_5959 and CX = ds:[0C7FCh] of seg052_519C. Nothing in the EXE calls it.
_smooth_over proc near
        mov     ax,seg seg_370D
        mov     es,ax
        mov     ds,ax
        mov     di,415Eh
        mov     si,658h
        mov     bp,cx
L0CDF:
        movsw
        movsw
        add     si,4
        xor     cx,cx
        lodsw
        sar     ax,5
        mov     dx,ax
        imul    dx
        add     cx,ax
        lodsw
        sar     ax,5
        mov     dx,ax
        imul    dx
        add     cx,ax
        lodsw
        sar     ax,5
        mov     dx,ax
        imul    dx
        add     cx,ax
        mov     bx,cx
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
        mov     ax,cx
        mov     cx,word ptr ds:[64Ch]
        mul     cl
        shr     ax,6
        xor     ah,ah
        add     ax,word ptr ds:[650h]
        jns     short L0D46
        xor     ax,ax
L0D46:
        add     ax,word ptr ds:[64Eh]
        cmp     al,0Fh
        jbe     short L0D50
        mov     al,0Fh
L0D50:
        stosw
        dec     bp
        jne     L0CDF
        mov     bp,offset _seg003_0272_5959
        mov     bx,es
        mov     ax,seg seg003_0272
        mov     es,ax
        mov     word ptr es:[0BC3h],0BF2h
        mov     word ptr es:[0BC5h],0CD9h
        mov     es,bx
        mov     bx,seg seg052_519C
        mov     es,bx
        mov     ds,bx
        mov     cx,word ptr ds:[0C7FCh]
        call    far ptr _seg003_0272_5311
        ret
_smooth_over endp

; name: the opcode table's do_uwsetup slot (0DAh) points here, at the word where the divide
; overflow handler below saves BP (FM do_uwsetup is the handler itself). The table is in the
; data segment, so this is the only trace of it in the module. An opcode 0DAh would execute
; this data, so UW2's programs presumably never use it.
do_uwsetup:
L0D7F   dw      0
        dw      0

; seg004_0849_D83  (+D83): divide overflow handler, installed by the smooth shading code
; (stored at ss:[5A5h] while clipping). Skips the 2-byte idiv, then retries the divide with
; the dividend halved (sar dx; rcr ax), or returns 7FFFh when the halved high word equals CX
; (a partial overflow check).
_seg004_0849_D83 proc far
        mov     word ptr cs:L0D7F,bp
        mov     bp,sp
        add     word ptr [bp],2
        sar     dx,1
        rcr     ax,1
        cmp     dx,cx
        je      short L0D9E
        idiv    cx
        mov     bp,word ptr cs:L0D7F
        iret
L0D9E:
        mov     ax,7FFFh
        xor     dx,dx
        mov     bp,word ptr cs:L0D7F
        iret
_seg004_0849_D83 endp

SEG004_TEXT ends

        end
