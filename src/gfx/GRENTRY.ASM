; /* target: seg003_0272_6E2 */
; /* opts: /ml */
;
; Graphics library module 3: the frame buffer. The 3D view is drawn into a linear (one byte per
; pixel) frame buffer in its own segment (seg049_3EE2, 370D:0958 holds its segment and 095C its
; row offsets), not into video memory; this module clears and shades it, copies it to the planar
; mode X screen, and has the span writers that draw into it, including the Gouraud spans of the
; 3D view's smooth faces. Its routines line up with FM Towns' in the same order: clear_fbuf,
; fill_fbuf, fbuf_draw_ylrpp, fbuf_draw_ylrpp_x, cDimFB, cLiteFB, setup_frame_buf, cFBtoScreen,
; fake_fb_solid_ylr, fbuf_save_vylr, fbcopy_vylrpp, fbuf_setcolor, then the smooth-span family
; (smooth_span_same, trans_span_same, vga_smooth_ylrii, do_filled_dither, do_trans_dither).
;
; Callers: seg021's C3DENTRY.ASM (cRender calls _6E2 before each frame; cFillFB _6E4; cFBtoScreen
; _7F9; cPlaceFB _7A8; the dim and light entries _764 and _788), VIDMODE.ASM's fbshow and vcopyfb
; (the transfers _6F8, _729, _A7F through 0DC2), GRCORE's fbuf_setcolor (_B9B), GRLIBM.ASM and
; seg004's SMOOTH.ASM (the smooth spans, _C13).
; Data (seg_370D): 0958 the frame buffer's segment; 095C its row offsets (setup_frame_buf);
; 0AEE the frame width the copier is patched for; 0AF0 the last row; 0B08, 0B0A the current
; smooth span's intensity and x; 4110, 4111 the colour; 4112 the span writer; 4116 a second
; colour word (a video-memory offset for the save writer); 94E, 383A the screen page offset.
; The FD51 piece defined here (0956 .. 0B0A) holds the far pointer to stdat and the row table,
; initially every row at 69D4h.
;
; name: descriptive (module 3 of seg003, "the entry points seg021's cRender calls"); the routine
; names given here are FM Towns'.
;
; SEG003 split into its original modules: this is module 3 of 14, +6E2..+D1E of the segment
; (file 3B32h), a module of the second library (UWLIB.LIB).
; Procs: seg003_0272_6E2, seg003_0272_6E4, seg003_0272_764, seg003_0272_788, seg003_0272_7A8,
; DRAW_RELATED_seg003_0272_7F9, seg003_0272_888, seg003_0272_9C9, seg003_0272_9FC,
; seg003_0272_A7F, seg003_0272_B9B, seg003_0272_C13.
; Evidence for the bounds: starts after one zero byte following a ret: TLINK's padding before
; a word-aligned module (TASM 2.0's `even` pads with 90h). It ends at +D1E: relocation entries
; 2442-2450 (+6E8..+CF3) and 2453 (+D21) are separated by the FD51 pair 2451-2452, so +D21 is
; another module, and +D1E is the only routine start between.
; Its relocations are entries 2442-2450 of UW2.EXE's relocation table.
; Its FD51 piece is 370D:0956..0B0A (relocation entries 2441), defined before the code because
; those entries come before the code's.
;
; Its FD51 piece holds relocation entry 2441, the far pointer at 956 to stdat; it runs to the
; next module's first pointer (B0A). Its bounds are not otherwise known.

; The VGA ports used here.
SC_DATA         equ     3C5h  ; sequencer data
GC_INDEX        equ     3CEh  ; graphics controller index

        .186

        extrn   _stdat:far
        extrn   seg004_0849:far
        extrn   seg049_3EE2:far
        extrn   seg_370D:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

FD51    segment byte public 'FAR_DATA'
        dd      _stdat
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h,0D4h,69h
        db      0D4h,69h,0D4h,69h,40h,01h,00h,00h,0C9h,09h,00h,00h,00h,00h,00h,00h
        db      8 dup (0)
        db      0FCh,09h,00h,00h,00h,00h,00h,00h
FD51    ends

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _cXfer:byte
        public  _DRAW_RELATED_seg003_0272_7F9
        public  _seg003_0272_6E2
        public  _seg003_0272_6E4
        public  _seg003_0272_6F8
        public  _seg003_0272_729
        public  _seg003_0272_764
        public  _seg003_0272_788
        public  _seg003_0272_7A8
        public  _seg003_0272_888
        public  _seg003_0272_9C9
        public  _seg003_0272_9FC
        public  _seg003_0272_A7F
        public  _seg003_0272_B9B
        public  _seg003_0272_C13

; seg003_0272_6E2  (+6E2)
; clear_fbuf (FM Towns): fill the frame buffer with colour 0 (falls into _6E4).
_seg003_0272_6E2 proc near
        xor     ax,ax
_seg003_0272_6E2 endp

; seg003_0272_6E4  (+6E4)
; fill_fbuf (far, cFillFB): AL = the colour. Fills 34E9h words of the frame buffer segment from
; offset 2.
;
; The two labels inside are span transfers into the frame buffer for VIDMODE.ASM's fbshow: _6F8
; (fbuf_draw_ylrpp) copies linear bitmap rows (8-byte records: y, left, right, source offset;
; source segment in 55EC) into the frame buffer, and _729 (fbuf_draw_ylrpp_x) does the same
; skipping colour 0.
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
_seg003_0272_6F8:
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
_seg003_0272_729:
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
; cDimFB (FM Towns; far): remap every frame-buffer pixel through row CL of lightabs
; (seg004:6D3E, PGCACHE.ASM), so the whole view darkens by that distance shade (CH and CL are
; swapped to index by row).
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
; cLiteFB (FM Towns; far): remap every frame-buffer pixel through the colour table at cXfer+300h
; (SCALEBM.ASM), CX times. L07A4 and L07A6 hold the caller's stack for cFBtoScreen.
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
; setup_frame_buf (FM Towns; far, cPlaceFB): BX = the frame's width, CX = its height. Builds the
; row offset table at 095C (rows of width + 2 bytes from offset 2; rows past the height all
; point at 69D4h), records the last row in 0AF0, and patches the unrolled copier _888 so that it
; copies exactly one plane's share of the width: the old ret becomes a movsb again and a ret
; (C3h) is written after the new width, for widths below 320.
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
; cFBtoScreen (FM Towns; far): copy the frame buffer to the screen at A000:[94E] + [383A] (the
; page being drawn). Switches to the library's stack, then for each row from the last (the top
; line of the screen, since y counts up) down to the window's bottom edge (3DFA), four passes,
; one per plane (map masks 8, 2, 4, 1 with source offsets 3, 1, 2, 0), each through the unrolled
; copier _888; screen rows are 80 bytes apart.
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
        mov     dx,SC_DATA
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
; _888: copy every fourth byte of a frame-buffer row to the screen (movsb; add si,3, unrolled
; for 320 pixels; setup_frame_buf patches in the ret).
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
; fake_fb_solid_ylr (FM Towns): the solid span writer for the frame buffer: fill each 6-byte
; span record (y, left, right; either order) with the colour in 4111.
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
; fbuf_save_vylr (FM Towns): copy frame-buffer spans (6-byte records, widened to whole groups of
; four pixels) to video memory at the offset in 4116, one plane at a time. Probably the save
; half of VALLOC.ASM's save_rect when the frame buffer is the target.
_seg003_0272_9FC proc near
        mov     dx,SC_DATA
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
; fbcopy_vylrpp (FM Towns): copy frame-buffer rows to the planar screen (8-byte records: y,
; left, right, destination offset), plane by plane with the edge masks from 3B64 and 3CA8;
; VIDMODE's vcopyfb uses it. Ends by setting the bit mask back to 0FFh.
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
        mov     dx,SC_DATA
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
        mov     dx,GC_INDEX
        mov     ax,0FF08h
        out     dx,ax
        pop     es
        pop     ds
        ret
        add     sp,2
        jmp     L0B8C
_seg003_0272_A7F endp

; seg003_0272_B9B  (+B9B)
; fbuf_setcolor (FM Towns): AX = a pen number (negative: no change). Sets the colour bytes 4110
; and 4116 from the tables at 21E and 434, and, when the pen's mode (the byte table at
; seg_370D:0008) is below 14h, the span writer 4112 from the table at 0AF2; so a pen chooses
; both a colour and a way of writing it (solid, copy, save, restore ...).
;
; After its ret: L0BC1 (the current span's y, whose low bit picks the dither phase) and the two
; smooth-span vectors L0BC3 and L0BC5, which seg004 sets (INTERP.ASM's opcode D6h to 0BC7h and
; 0C93h, solid; opcode D8h and SMOOTH.ASM to 0BF2h and 0CD9h, translucent). L0BC7
; (smooth_span_same, solid, both ends the same shade) draws the span in one colour,
; lightabs[shade][base colour] with the base colour at seg004:6D20 (the byte PGCACHE.ASM keeps
; for seg003), through fake_fb_solid_ylr. The code at 0BF2 (trans_span_same) remaps the pixels
; already in the span through one lightabs row.
_seg003_0272_B9B proc near
        shl     ax,1
        jb      L0BBF
        mov     bx,ax
        mov     ax,word ptr [bx+21Eh]
        mov     word ptr ds:[4110h],ax
        mov     ax,word ptr [bx+434h]
        mov     word ptr ds:[4116h],ax
        db      8Bh,9Fh,8h,0h                   ; match: mov bx,word ptr [bx+8]: disp16, the table at seg_370D:0008 (TASM gives disp8 for a number, and a label there would need that segment)
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
; vga_smooth_ylrii (FM Towns): draw a table of Gouraud spans into the frame buffer. In: SI =
; 10-byte records (y, left x, right x, left intensity, right intensity; GRLIBM.ASM's table at
; 5692), ended by a y with its top bit set. For each, the intensity step per pixel is formed as
; 8.8 and the span drawn through L0BC5, or through L0BC3 when both ends have the same intensity.
;
; L0C93 (do_filled_dither, solid) writes lightabs[intensity][base colour]; the code after L0CD3
; (do_trans_dither, translucent) writes lightabs[intensity][the pixel already there]. Both
; dither: two intensity accumulators 40h apart alternate pixel by pixel, and which starts
; depends on the row's parity, so a shade between two lightabs rows comes out as a checkerboard
; of both.
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

SEG003_TEXT ends

        end
