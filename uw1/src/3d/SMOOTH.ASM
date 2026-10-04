; /* target: seg004_260 */
; /* opts: /ml */
;
; smooth_closure and smooth_over.
;
; What it does in the game: Gouraud-shaded polygons for the 3D view. INTERP.ASM's do_closure
; jumps to smooth_closure when the current fill routine (ds:15F6h) is seg003's Gouraud
; routine, which do_goursurf, do_uwsurf and do_transsurf select. smooth_closure is
; do_closure with a shade per vertex: it projects the vertex buffer, writes each vertex's
; shade byte (point +7, set by do_setshade or do_uwshade) beside it, and calls seg003's
; polygon routine; its clippers (seg004_45C, 58E, 6C4, 7F8, the counterparts of
; INTERP's clip_top, clip_bot, clip_right, clip_left) interpolate the shade at each new
; vertex as well as the position. smooth_over is a separate routine, which UW1's TMAPOPS.ASM
; (seg004_6130) calls when the low byte of 2928h is nonzero; nothing in UW2's EXE calls it.
;
; Data: none of its own apart from L09E1, the word the divide overflow handler saves BP in.
; Output goes to seg048: the projected vertices at 415Ch and their shades at 55ECh, one
; word apart.
;
; UW1: UW2's SMOOTH.ASM instruction for instruction, with UW1's data offsets, and one
; difference:
; the opcode table's do_uwsetup slot points at an `int 2` after smooth_over's last `ret`,
; where UW2's points at the data word after it. The comments are UW2's, with UW1's addresses.
;
; name: UW2's (descriptive there); smooth_closure and smooth_over are FM Towns' names.
;
; .186, where UW2 has .386: UW1's seg004 has no 386 instruction and none of the nop padding
; that TASM's .386 mode puts after forward conditional jumps.
;
; SEG004 split into its original modules: this is module 2 of 9, +260..+A10 of the segment
; (file A2D0h), a module of the second library. UW2 has SPHERE.ASM before it; UW1 has its
; sphere code after INTERP's matrix module (SPHERE.ASM, +1370).
; Procs: smooth_closure, seg004_3F8, seg004_45C, seg004_58E, seg004_6C4,
; seg004_7F8, smooth_over, seg004_9E5.
; match: evidence for the bounds: starts on a paragraph after 10 zero bytes following a jmp:
; TLINK's padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90,
; not zeros).
; Its relocations are entries 2597-2605 of UW.EXE's relocation table.

; The model interpreter's opcode table: a word per opcode, the offset of its handler
; (INTERP.ASM), in seg051, the data segment while interpreting.
OPCODE_TABLE    equ     2738h

        .186

        extrn   _seg003_5B0B:far
        extrn   seg003:far
        extrn   seg051:far
        extrn   seg048:far
        extrn   _seg003_6159:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public 'code'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _code_pnt:near
        public  _seg004_2A4
        public  _seg004_2AF
        public  _seg004_312
        public  _seg004_322
        public  _seg004_3F8
        public  _seg004_45C
        public  _seg004_58E
        public  _seg004_6C4
        public  _seg004_7F8
        public  _seg004_9E5
        public  _smooth_closure
        public  _smooth_over

; seg004_260  (+260)
; Draws the Gouraud polygon in the vertex buffer (ds:307h..305h). Raises int 2 if the buffer
; has run past 0C69h. Codes AND nonzero: skipped. Codes OR zero: projected (the add
; immediates at seg004_2A4 and 2AF are the screen centre, patched by render_3d) with
; the shades copied to seg048:55ECh, then drawn with BP = ds:[15F8h]. Codes OR with bit 80h
; (behind the eye): clipped against top, bottom, left and right in turn, retrying after each.
; Otherwise projected with overflow checks (immediates at 312 and 322), drawn with BP =
; ds:[15F6h], or clipped on overflow (L03FC). Continues with the next opcode.
_smooth_closure proc near
        cmp     word ptr ds:[305h],0C69h
        jle     short L026A
        int     2
L026A:
        mov     word ptr ds:[2884h],0
        test    byte ptr ds:[161Bh],0FFh
        je      short L027A
        jmp     L03F2
L027A:
        mov     di,309h
        mov     word ptr ds:[307h],di
        test    byte ptr ds:[161Ah],0FFh
        jne     short L02ED
L0288:
        push    si
        mov     ax,seg seg048
        mov     es,ax
        mov     di,415Ch
        mov     bx,55ECh
        mov     si,word ptr ds:[307h]
        mov     cx,word ptr ds:[26B2h]
L029C:
        lodsw
        mov     bp,word ptr [si+2]
        imul    cx
        idiv    bp
_seg004_2A4:
        add     ax,1234h
        stosw
        lodsw
        imul    word ptr ds:[26B0h]
        idiv    bp
_seg004_2AF:
        add     ax,1234h
        stosw
        mov     al,byte ptr [si+3]
        mov     byte ptr es:[bx],al
        add     si,4
        add     bx,2
        cmp     si,word ptr ds:[305h]
        jne     L029C
        mov     bx,word ptr ds:[305h]
        sub     bx,word ptr ds:[307h]
        shr     bx,1
        shr     bx,1
        shr     bx,1
        mov     cx,bx
        inc     word ptr ds:[2884h]
        mov     bp,word ptr ds:[15F8h]
        call    far ptr _seg003_5B0B
        mov     ax,ds
        mov     es,ax
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L02ED:
        js      short L0363
        push    si
L02F0:
        mov     word ptr ss:[4D5h],offset _seg004_3F8
        mov     ax,seg seg048
        mov     es,ax
        mov     di,415Ch
        mov     bx,55ECh
        mov     si,word ptr ds:[307h]
        mov     cx,word ptr ds:[26B2h]
L030A:
        lodsw
        mov     bp,word ptr [si+2]
        imul    cx
        idiv    bp
_seg004_312:
        add     ax,1234h
        jno     short L031A
        jmp     L03FC
L031A:
        stosw
        lodsw
        imul    word ptr ds:[26B0h]
        idiv    bp
_seg004_322:
        add     ax,1234h
        jno     short L032A
        jmp     L03FC
L032A:
        stosw
        mov     al,byte ptr [si+3]
        mov     byte ptr es:[bx],al
        add     si,4
        add     bx,2
        cmp     si,word ptr ds:[305h]
        jne     L030A
        mov     cx,word ptr ds:[305h]
        sub     cx,word ptr ds:[307h]
        shr     cx,1
        shr     cx,1
        shr     cx,1
        inc     word ptr ds:[2884h]
        mov     bp,word ptr ds:[15F6h]
        call    far ptr _seg003_5B0B
        mov     ax,ds
        mov     es,ax
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L0363:
        mov     word ptr ss:[4D5h],offset _seg004_9E5
        test    byte ptr ds:[161Ah],4
        je      short L038C
        push    si
        call    _seg004_45C
        pop     si
        test    byte ptr ds:[161Bh],0FFh
        jne     short L03F2
        test    byte ptr ds:[161Ah],0FFh
        js      short L038C
        jne     short L0389
        jmp     L0288
L0389:
        jmp     L02ED
L038C:
        test    byte ptr ds:[161Ah],8
        je      short L03AE
        push    si
        call    _seg004_58E
        pop     si
        test    byte ptr ds:[161Bh],0FFh
        jne     short L03F2
        test    byte ptr ds:[161Ah],0FFh
        js      short L03AE
        jne     short L03AB
        jmp     L0288
L03AB:
        jmp     L02ED
L03AE:
        test    byte ptr ds:[161Ah],1
        je      short L03D0
        push    si
        call    _seg004_7F8
        pop     si
        test    byte ptr ds:[161Bh],0FFh
        jne     short L03F2
        test    byte ptr ds:[161Ah],0FFh
        js      short L03D0
        jne     short L03CD
        jmp     L0288
L03CD:
        jmp     L02ED
L03D0:
        test    byte ptr ds:[161Ah],2
        je      short L03F2
        push    si
        call    _seg004_6C4
        pop     si
        test    byte ptr ds:[161Bh],0FFh
        jne     short L03F2
        test    byte ptr ds:[161Ah],0FFh
        js      short L03F2
        jne     short L03EF
        jmp     L0288
L03EF:
        jmp     L02ED
L03F2:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_smooth_closure endp

; seg004_3F8  (+3F8)
; Recovery entry for the divide overflow during smooth_closure's checked projection (it is
; stored at ss:[4D5h]): drops the int 0 frame, then (L03FC) clips against right, left, top
; and bottom as the codes require and projects again; gives up when nothing is left,
; restoring DS and ES to seg051. The counterpart of INTERP's clip_overflow and
; do_3d_clip.
_seg004_3F8 proc near
        sti
        add     sp,6
L03FC:
        mov     word ptr ss:[4D5h],offset _seg004_9E5
        mov     ax,ds
        mov     es,ax
        test    byte ptr ds:[161Ah],2
        je      short L0418
        call    _seg004_6C4
        test    byte ptr ds:[161Bh],0FFh
        jne     short L044E
L0418:
        test    byte ptr ds:[161Ah],1
        je      short L0429
        call    _seg004_7F8
        test    byte ptr ds:[161Bh],0FFh
        jne     short L044E
L0429:
        test    byte ptr ds:[161Ah],4
        je      short L043A
        call    _seg004_45C
        test    byte ptr ds:[161Bh],0FFh
        jne     short L044E
L043A:
        test    byte ptr ds:[161Ah],8
        je      short L044B
        call    _seg004_58E
        test    byte ptr ds:[161Bh],0FFh
        jne     short L044E
L044B:
        jmp     L02F0
L044E:
        pop     si
        mov     ax,seg seg051
        mov     ds,ax
        mov     es,ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_3F8 endp

; seg004_45C  (+45C)
; Clips the Gouraud polygon against y = z (code bit 4), like INTERP's clip_top, also
; interpolating each new vertex's shade (+7).
_seg004_45C proc near
        mov     byte ptr ds:[161Ah],0
        mov     byte ptr ds:[161Bh],0FFh
        mov     si,word ptr ds:[307h]
        mov     di,word ptr ds:[305h]
        mov     cx,8
        rep movsw
        add     word ptr ds:[305h],8
        mov     si,word ptr ds:[307h]
        mov     di,0C69h
        cmp     si,309h
        je      short L0488
        mov     di,309h
L0488:
        mov     word ptr ds:[307h],di
L048C:
        add     si,8
L048F:
        cmp     si,word ptr ds:[305h]
        jne     short L0498
        jmp     L0589
L0498:
        mov     al,byte ptr [si+6]
        test    al,4
        jne     short L04AE
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        mov     cx,4
        rep movsw
        jmp     L048F
L04AE:
        test    byte ptr [si-2],4
        jne     short L0519
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
        jae     short L04E8
        cwd
        adc     ax,dx
L04E8:
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
        jae     short L0503
        cwd
        adc     ax,dx
L0503:
        add     ax,word ptr [si-6]
        stosw
        stosw
        mov     cx,ax
        mov     bp,ax
        call    _code_pnt
        stosb
        inc     di
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
L0519:
        test    byte ptr [si+0Eh],4
        je      short L0522
        jmp     L048C
L0522:
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
        jae     short L0556
        cwd
        adc     ax,dx
L0556:
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
        jae     short L0570
        cwd
        adc     ax,dx
L0570:
        add     ax,word ptr [si+2]
        stosw
        stosw
        mov     cx,ax
        mov     bp,ax
        call    _code_pnt
        stosb
        inc     di
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        jmp     L048C
L0589:
        mov     word ptr ds:[305h],di
        ret
_seg004_45C endp

; seg004_58E  (+58E)
; Gouraud clip against y = -z (code bit 8), like clip_bot.
_seg004_58E proc near
        mov     byte ptr ds:[161Ah],0
        mov     byte ptr ds:[161Bh],0FFh
        mov     si,word ptr ds:[307h]
        mov     di,word ptr ds:[305h]
        mov     cx,8
        rep movsw
        add     word ptr ds:[305h],8
        mov     si,word ptr ds:[307h]
        mov     di,0C69h
        cmp     si,309h
        je      short L05BA
        mov     di,309h
L05BA:
        mov     word ptr ds:[307h],di
L05BE:
        add     si,8
L05C1:
        cmp     si,word ptr ds:[305h]
        jne     short L05CA
        jmp     L06BF
L05CA:
        mov     al,byte ptr [si+6]
        test    al,8
        jne     short L05E0
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        mov     cx,4
        rep movsw
        jmp     L05C1
L05E0:
        test    byte ptr [si-2],8
        jne     short L064D
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
        jae     short L061A
        cwd
        adc     ax,dx
L061A:
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
        jae     short L0635
        cwd
        adc     ax,dx
L0635:
        add     ax,word ptr [si-6]
        stosw
        mov     cx,ax
        neg     ax
        stosw
        mov     bp,ax
        call    _code_pnt
        stosb
        inc     di
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
L064D:
        test    byte ptr [si+0Eh],8
        je      short L0656
        jmp     L05BE
L0656:
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
        jae     short L068A
        cwd
        adc     ax,dx
L068A:
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
        jae     short L06A4
        cwd
        adc     ax,dx
L06A4:
        add     ax,word ptr [si+2]
        stosw
        mov     cx,ax
        neg     ax
        stosw
        mov     bp,ax
        call    _code_pnt
        stosb
        inc     di
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        jmp     L05BE
L06BF:
        mov     word ptr ds:[305h],di
        ret
_seg004_58E endp

; seg004_6C4  (+6C4)
; Gouraud clip against x = z (code bit 2), like clip_right.
_seg004_6C4 proc near
        mov     byte ptr ds:[161Ah],0
        mov     byte ptr ds:[161Bh],0FFh
        mov     si,word ptr ds:[307h]
        mov     di,word ptr ds:[305h]
        mov     cx,8
        rep movsw
        add     word ptr ds:[305h],8
        mov     si,word ptr ds:[307h]
        mov     di,0C69h
        cmp     si,309h
        je      short L06F0
        mov     di,309h
L06F0:
        mov     word ptr ds:[307h],di
L06F4:
        add     si,8
L06F7:
        cmp     si,word ptr ds:[305h]
        jne     short L0700
        jmp     L07F3
L0700:
        mov     al,byte ptr [si+6]
        test    al,2
        jne     short L0716
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        mov     cx,4
        rep movsw
        jmp     L06F7
L0716:
        test    byte ptr [si-2],2
        jne     short L0782
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
        jae     short L074F
        cwd
        adc     ax,dx
L074F:
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
        jae     short L076A
        cwd
        adc     ax,dx
L076A:
        add     ax,word ptr [si-6]
        stosw
        mov     cx,ax
        mov     ax,bx
        stosw
        mov     bp,ax
        call    _code_pnt
        stosb
        inc     di
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
L0782:
        test    byte ptr [si+0Eh],2
        je      short L078B
        jmp     L06F4
L078B:
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
        jae     short L07BE
        cwd
        adc     ax,dx
L07BE:
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
        jae     short L07D8
        cwd
        adc     ax,dx
L07D8:
        add     ax,word ptr [si+2]
        stosw
        mov     cx,ax
        mov     ax,bx
        stosw
        mov     bp,ax
        call    _code_pnt
        stosb
        inc     di
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        jmp     L06F4
L07F3:
        mov     word ptr ds:[305h],di
        ret
_seg004_6C4 endp

; seg004_7F8  (+7F8)
; Gouraud clip against x = -z (code bit 1), like clip_left.
_seg004_7F8 proc near
        mov     byte ptr ds:[161Ah],0
        mov     byte ptr ds:[161Bh],0FFh
        mov     si,word ptr ds:[307h]
        mov     di,word ptr ds:[305h]
        mov     cx,8
        rep movsw
        add     word ptr ds:[305h],8
        mov     si,word ptr ds:[307h]
        mov     di,0C69h
        cmp     si,309h
        je      short L0824
        mov     di,309h
L0824:
        mov     word ptr ds:[307h],di
L0828:
        add     si,8
L082B:
        cmp     si,word ptr ds:[305h]
        jne     short L0834
        jmp     L092B
L0834:
        mov     al,byte ptr [si+6]
        test    al,1
        jne     short L084A
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        mov     cx,4
        rep movsw
        jmp     L082B
L084A:
        test    byte ptr [si-2],1
        jne     short L08B8
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
        jae     short L0883
        cwd
        adc     ax,dx
L0883:
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
        jae     short L089E
        cwd
        adc     ax,dx
L089E:
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
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
L08B8:
        test    byte ptr [si+0Eh],1
        je      short L08C1
        jmp     L0828
L08C1:
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
        jae     short L08F4
        cwd
        adc     ax,dx
L08F4:
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
        jae     short L090E
        cwd
        adc     ax,dx
L090E:
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
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        jmp     L0828
L092B:
        mov     word ptr ds:[305h],di
        ret
_seg004_7F8 endp

; smooth_over  (+930): FM smooth_over, the same loop (copy the projected x and y, then
; an intensity from the vertex normal's length)
; In: CX = the vertex count. For each 14-byte record at seg048:659h it copies the screen x
; and y to 415Ch, takes the three words at +8 (each >> 5), and computes the length of that
; vector by four Newton steps from 64; the shade is the low bytes of the length and [64Ch]
; multiplied, >> 6, plus [650h] (at least 0) plus [64Eh], at most 15. Then it points
; seg003's handler pair at 0F27h/0F29h at 0F56h/103Dh (as do_transsurf does) and calls the
; polygon routine with BP = _seg003_6159 and CX = ds:[0B07Ah] of seg051. Called by
; TMAPOPS.ASM's seg004_6130 (UW1 only).
_smooth_over proc near
        mov     ax,seg seg048
        mov     es,ax
        mov     ds,ax
        mov     di,415Ch
        mov     si,659h
        mov     bp,cx
L093F:
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
        jns     short L09A6
        xor     ax,ax
L09A6:
        add     ax,word ptr ds:[64Eh]
        cmp     al,0Fh
        jbe     short L09B0
        mov     al,0Fh
L09B0:
        stosw
        dec     bp
        jne     L093F
        mov     bp,offset _seg003_6159
        mov     bx,es
        mov     ax,seg seg003
        mov     es,ax
        mov     word ptr es:[0F27h],0F56h
        mov     word ptr es:[0F29h],103Dh
        mov     es,bx
        mov     bx,seg seg051
        mov     es,bx
        mov     ds,bx
        mov     cx,word ptr ds:[0B07Ah]
        call    far ptr _seg003_5B0B
        ret
; UW1: the opcode table's do_uwsetup slot (0DAh) points here, at an int 2 (UW2's points at
; the data word below), so an opcode 0DAh traps.
do_uwsetup:
        int     2
_smooth_over endp

; The word where the divide overflow handler below saves BP.
L09E1   dw      0
        dw      0

; seg004_9E5  (+9E5): divide overflow handler, installed by the smooth shading code
; (stored at ss:[4D5h] while clipping). Skips the 2-byte idiv, then retries the divide with
; the dividend halved (sar dx; rcr ax), or returns 7FFFh when the halved high word equals CX
; (a partial overflow check).
_seg004_9E5 proc far
        mov     word ptr cs:L09E1,bp
        mov     bp,sp
        add     word ptr [bp],2
        sar     dx,1
        rcr     ax,1
        cmp     dx,cx
        je      short L0A00
        idiv    cx
        mov     bp,word ptr cs:L09E1
        iret
L0A00:
        mov     ax,7FFFh
        xor     dx,dx
        mov     bp,word ptr cs:L09E1
        iret
_seg004_9E5 endp

SEG004_TEXT ends

        end

