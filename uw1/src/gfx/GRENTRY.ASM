; /* target: seg003_A46 */
; /* opts: /ml */
;
; Graphics library module 6: the frame buffer. The 3D view is drawn into a linear (one byte per
; pixel) frame buffer (in UW1 the segment of stdat, 3F4B, whose far pointer the FD52 piece
; holds; 3963:095A holds its segment and 095E its row offsets), not into video memory; this
; module clears and shades it, copies it to the planar
; mode X screen, and has the span writers that draw into it, including the Gouraud spans of the
; 3D view's smooth faces. Its routines line up with FM Towns' in the same order: clear_fbuf,
; fill_fbuf, fbuf_draw_ylrpp, fbuf_draw_ylrpp_x, cDimFB, cLiteFB, setup_frame_buf, cFBtoScreen,
; fake_fb_solid_ylr, fbuf_save_vylr, fbcopy_vylrpp, fbuf_setcolor, then the smooth-span family
; (smooth_span_same, trans_span_same, vga_smooth_ylrii, do_filled_dither, do_trans_dither).
;
; Callers: seg019's C3DENTRY.ASM (cRender calls _A46 before each frame; cFillFB _A48; cFBtoScreen
; _B5D; cPlaceFB _B0C; the dim and light entries _AC8 and _AEC), VIDMODE.ASM's fbshow and vcopyfb
; (the transfers _A5C, _A8D, _DE3 through 0DC2), GRCORE's fbuf_setcolor (_EFF), GRLIBM.ASM and
; seg004's SMOOTH.ASM (the smooth spans, _F77).
; Data (seg048): 095A the frame buffer's segment; 095E its row offsets (setup_frame_buf);
; 0AF0 the frame width the copier is patched for; 0AF2 the last row; 0B0A, 0B0C the current
; smooth span's intensity and x; 410E, 410F the colour; 4110 the span writer; 4114 a second
; colour word (a video-memory offset for the save writer); 950, 3838 the screen page offset.
; The FD52 piece defined here (0958 .. 0B0C) holds the far pointer to stdat and the row table,
; initially every row at 4CD0h.
;
; name: descriptive (UW2Decomp: module 3 of seg003, "the entry points seg019's cRender
; calls"); the routine names given here are FM Towns'.
;
; UW1 against UW2: UW2Decomp's src/gfx/GRENTRY.ASM (UW2's seg003_0272_6E2), the same length and
; the same instructions but for one block and the numbers. The frame buffer is smaller (2667h
; words, UW2 34E9h; rows past the height point at 4CD0h, UW2 69D4h) and lives in stdat's
; segment, so _A48, _AC8 and _AEC load seg _stdat (UW2 a segment of its own). The library's data
; sits 2 bytes higher here (095A for UW2's 0958) and 2 lower from 3838 on. cLiteFB uses the
; second cXfer table (+100h, UW2 the fourth). smooth_span_same (L0F2B) reads its base colour
; from seg004 through CS (0090:CEC0, seg004's 6950) before loading ES, where UW2 reads
; seg004:6D20 through ES.
;
; SEG003 split into its original modules: this is module 6 of UW1's 18, +A46..+1081.
; Evidence for the bounds: starts after one zero byte following a ret (TLINK's padding before a
; word-aligned module), as in UW2. It ends at +1082: its relocations (entries 2730-2738,
; +A4C..+1057) and the next ones in this segment (2741-2742, +1085, +11DA) are separated by the
; FD52 pair 2739-2740, so +1085 is another module, and +1082 is the only routine start between.
; Its FD52 piece (3963:0958..0B0C) holds relocation entry 2729, the far pointer to stdat, which
; comes right before the code's; it runs to the next module's first far pointer (0B0C).

; The VGA ports used here.
SC_DATA         equ     3C5h  ; sequencer data
GC_INDEX        equ     3CEh  ; graphics controller index

; The graphics library's data in seg048 (DS while the library runs), as GRLIBF.ASM's
; and VIDMODE.ASM's headers describe it. The names are ours.
WIN_LEFT        equ     3DF2h   ; the clip window (y counts up, so bottom < top)
SCREEN_SEG      equ     3DFAh   ; the screen's segment
PEN_COLOR       equ     410Fh   ; the colour
SPAN_WRITER     equ     4110h   ; the current span writer (set_the_color, fbuf_setcolor)
PEN_WORD2       equ     4114h   ; the pen's second word

        .186

        extrn   _stdat:far
        extrn   seg004:far
        extrn   seg048:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

FD52    segment byte public 'FAR_DATA'
        dd      _stdat
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch,0D0h,4Ch
        db      0D0h,4Ch,0D0h,4Ch,40h,01h,00h,00h,2Dh,0Dh   ; 0D2Dh, 0D60h: _seg003_D2D and _seg003_D60
        db      0Eh dup (0)
        db      60h,0Dh,00h,00h,00h,00h,00h,00h
FD52    ends

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _cXfer:byte
        public  _seg003_B5D
        public  _seg003_A46
        public  _seg003_A48
        public  _seg003_A5C
        public  _seg003_A8D
        public  _seg003_AC8
        public  _seg003_AEC
        public  _seg003_B0C
        public  _seg003_BEC
        public  _seg003_D2D
        public  _seg003_D60
        public  _seg003_DE3
        public  _seg003_EFF
        public  _seg003_F77

; seg003_A46  (+A46)
; clear_fbuf (FM Towns): fill the frame buffer with colour 0 (falls into _A48).
_seg003_A46 proc near
        xor     ax,ax
_seg003_A46 endp

; seg003_A48  (+A48)
; fill_fbuf (far, cFillFB): AL = the colour. Fills 2667h words of the frame buffer segment from
; offset 2.
;
; The two labels inside are span transfers into the frame buffer for VIDMODE.ASM's fbshow: _A5C
; (fbuf_draw_ylrpp) copies linear bitmap rows (8-byte records: y, left, right, source offset;
; source segment in 55EA) into the frame buffer, and _A8D (fbuf_draw_ylrpp_x) does the same
; skipping colour 0.
_seg003_A48 proc far
        mov     ah,al
        push    es
        mov     cx,seg _stdat
        mov     es,cx
        mov     cx,2667h
        xor     di,di
        add     di,2
        rep stosw
        pop     es
        retf
_seg003_A5C:
        push    es
        push    ds
        mov     es,word ptr ss:[95Ah]
        mov     ds,word ptr ss:[55EAh]
L0A68:
        lods    word ptr ss:[si]
        shl     ax,1
        jb      L0A8A
        mov     bx,ax
        mov     di,word ptr ss:[bx+95Eh]
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
        jmp     L0A68
L0A8A:
        pop     ds
        pop     es
        retn
_seg003_A8D:
        push    es
        push    ds
        mov     es,word ptr ss:[95Ah]
        mov     ds,word ptr ss:[55EAh]
L0A99:
        lods    word ptr ss:[si]
        shl     ax,1
        jb      L0AC5
        mov     bx,ax
        mov     di,word ptr ss:[bx+95Eh]
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
L0AB8:
        lodsb
        or      al,al
        je      L0ABF
        stosb
        dec     di
L0ABF:
        inc     di
        loop    L0AB8
        pop     si
        jmp     L0A99
L0AC5:
        pop     ds
        pop     es
        retn
_seg003_A48 endp

; seg003_AC8  (+AC8)
; cDimFB (FM Towns; far): remap every frame-buffer pixel through row CL of lightabs
; (seg004:696E, PGCACHE.ASM), so the whole view darkens by that distance shade (CH and CL are
; swapped to index by row).
_seg003_AC8 proc far
        push    ds
        push    es
        mov     bx,seg _stdat
        mov     ds,bx
        mov     bx,seg seg004
        mov     es,bx
        xchg    ch,cl
        mov     si,cx
        mov     di,4D7Eh
        xor     bx,bx
L0ADD:
        mov     bl,byte ptr [di]
        mov     bl,byte ptr es:[bx+si+696Eh]
        mov     byte ptr [di],bl
        dec     di
        jne     L0ADD
        pop     es
        pop     ds
        retf
_seg003_AC8 endp

; seg003_AEC  (+AEC)
; cLiteFB (FM Towns; far): remap every frame-buffer pixel through the colour table at cXfer+100h
; (SCALEBM.ASM), CX times. L0B08 and L0B0A hold the caller's stack for cFBtoScreen.
_seg003_AEC proc far
        push    ds
        mov     bx,seg _stdat
        mov     ds,bx
L0AF2:
        mov     si,4CCEh
        xor     bx,bx
L0AF7:
        mov     bl,byte ptr [si]
        mov     bl,byte ptr cs:_cXfer[bx+100h]  ; UW1: the second table (UW2 the fourth, +300h)
        mov     byte ptr [si],bl
        dec     si
        jne     L0AF7
        dec     cx
        jne     L0AF2
        pop     ds
        retf
L0B08   dw      0
L0B0A   dw      0
_seg003_AEC endp

; seg003_B0C  (+B0C)
; setup_frame_buf (FM Towns; far, cPlaceFB): BX = the frame's width, CX = its height. Builds the
; row offset table at 095E (rows of width + 2 bytes from offset 2; rows past the height all
; point at 4CD0h), records the last row in 0AF2, and patches the unrolled copier _BEC so that it
; copies exactly one plane's share of the width: the old ret becomes a movsb again and a ret
; (C3h) is written after the new width, for widths below 320.
_seg003_B0C proc far
        add     bx,2
        mov     di,95Eh
        mov     dx,cx
        xor     ax,ax
        add     ax,2
L0B19:
        stosw
        add     ax,bx
        loop    L0B19
        mov     ax,4CD0h
        mov     cx,0C8h
        sub     cx,dx
        dec     dx
        mov     word ptr ds:[0AF2h],dx
        rep stosw
        cmp     bx,word ptr ds:[0AF0h]
        je      L0B5C
        cmp     word ptr ds:[0AF0h],140h
        jge     L0B48
        mov     di,offset _seg003_BEC
        mov     dx,word ptr ds:[0AF0h]
        add     di,dx
        mov     byte ptr cs:[di],0A4h   ; movsb again where the old ret was
L0B48:
        sub     bx,2
        cmp     bx,140h
        jge     L0B5C
        mov     di,offset _seg003_BEC
        mov     byte ptr cs:[bx+di],0C3h ; ret after the new width
        mov     word ptr ds:[0AF0h],bx
L0B5C:
        retf
_seg003_B0C endp

; DRAW_RELATED_seg003_B5D  (+B5D)
; cFBtoScreen (FM Towns; far): copy the frame buffer to the screen at A000:[950] + [3838] (the
; page being drawn). Switches to the library's stack, then for each row from the last (the top
; line of the screen, since y counts up) down to the window's bottom edge (3DF8), four passes,
; one per plane (map masks 8, 2, 4, 1 with source offsets 3, 1, 2, 0), each through the unrolled
; copier _BEC; screen rows are 80 bytes apart.
_seg003_B5D proc far
        push    bp
        push    es
        push    ds
        push    si
        push    di
        mov     ax,seg seg048
        mov     ds,ax
        mov     es,ax
        mov     bx,ss
        mov     word ptr cs:L0B08,bx
        mov     word ptr cs:L0B0A,sp
        cli
        mov     ss,ax
        mov     sp,word ptr ds:[5586h]
        sti
        mov     ax,0A000h
        mov     es,ax
        mov     bp,word ptr ds:[0AF2h]
        mov     dx,SC_DATA
        mov     bx,word ptr ds:[950h]
        mov     ax,word ptr ds:[3838h]
        add     bx,ax
        mov     ds,word ptr ds:[95Ah]
L0B96:
        shl     bp,1
        mov     cx,word ptr [bp+95Eh]
        mov     al,8
        out     dx,al
        mov     di,bx
        mov     si,3
        add     si,cx
        call    _seg003_BEC
        mov     al,2
        out     dx,al
        mov     di,bx
        mov     si,1
        add     si,cx
        call    _seg003_BEC
        mov     al,4
        out     dx,al
        mov     di,bx
        mov     si,2
        add     si,cx
        call    _seg003_BEC
        mov     al,1
        out     dx,al
        mov     di,bx
        mov     si,cx
        call    _seg003_BEC
        add     bx,50h
        shr     bp,1
        dec     bp
        cmp     bp,word ptr ss:[3DF8h]
        jg      L0B96
        cli
        mov     ss,word ptr cs:L0B08
        mov     sp,word ptr cs:L0B0A
        sti
        pop     di
        pop     si
        pop     ds
        pop     es
        pop     bp
        retf
_seg003_B5D endp

; seg003_BEC  (+BEC)
; _BEC: copy every fourth byte of a frame-buffer row to the screen (movsb; add si,3, unrolled
; for 320 pixels; setup_frame_buf patches in the ret).
_seg003_BEC proc near
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
_seg003_BEC endp

; seg003_D2D  (+D2D)
; fake_fb_solid_ylr (FM Towns): the solid span writer for the frame buffer: fill each 6-byte
; span record (y, left, right; either order) with the colour in 410F.
_seg003_D2D proc near
        push    es
        mov     bx,word ptr ds:[WIN_LEFT]
        mov     es,word ptr ds:[95Ah]
L0D36:
        lodsw
        shl     ax,1
        jb      L0D55
        mov     di,ax
        mov     di,word ptr [di+95Eh]
        lodsw
        mov     cx,ax
        add     di,ax
        lodsw
        sub     cx,ax
        neg     cx
        mov     al,byte ptr ds:[PEN_COLOR]
        inc     cx
        js      L0D57
        rep stosb
        jmp     L0D36
L0D55:
        pop     es
        ret
L0D57:
        add     di,cx
        dec     cx
        neg     cx
        rep stosb
        jmp     L0D36
_seg003_D2D endp

; seg003_D60  (+D60)
; fbuf_save_vylr (FM Towns): copy frame-buffer spans (6-byte records, widened to whole groups of
; four pixels) to video memory at the offset in 4114, one plane at a time. Probably the save
; half of VALLOC.ASM's save_rect when the frame buffer is the target.
_seg003_D60 proc near
        mov     dx,SC_DATA
        mov     bp,si
        mov     di,word ptr ds:[PEN_WORD2]
        push    ds
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
        mov     ds,word ptr ds:[95Ah]
L0D73:
        mov     bx,word ptr [bp]
        shl     bx,1
        jb      L0DE0
        mov     si,word ptr [bp+2]
        and     si,-4
        mov     cx,word ptr [bp+4]
        and     cx,-4
        add     cx,4
        add     bp,6
        sub     cx,si
        add     si,word ptr ss:[bx+95Eh]
        mov     bx,cx
        push    bp
        shr     cx,2
        mov     bp,di
        mov     al,1
        out     dx,al
L0D9E:
        movsb
        add     si,3
        loop    L0D9E
        sub     si,bx
        inc     si
        mov     cx,bx
        shr     cx,2
        mov     di,bp
        shl     al,1
        out     dx,al
L0DB1:
        movsb
        add     si,3
        loop    L0DB1
        sub     si,bx
        inc     si
        mov     cx,bx
        shr     cx,2
        mov     di,bp
        shl     al,1
        out     dx,al
L0DC4:
        movsb
        add     si,3
        loop    L0DC4
        sub     si,bx
        inc     si
        mov     cx,bx
        shr     cx,2
        mov     di,bp
        shl     al,1
        out     dx,al
L0DD7:
        movsb
        add     si,3
        loop    L0DD7
        pop     bp
        jmp     L0D73
L0DE0:
        pop     es
        pop     ds
        ret
_seg003_D60 endp

; seg003_DE3  (+DE3)
; fbcopy_vylrpp (FM Towns): copy frame-buffer rows to the planar screen (8-byte records: y,
; left, right, destination offset), plane by plane with the edge masks from 3B62 and 3CA6;
; VIDMODE's vcopyfb uses it. Ends by setting the bit mask back to 0FFh.
_seg003_DE3 proc near
        push    ds
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
        mov     ds,word ptr ds:[95Ah]
        mov     bp,si
L0DEF:
        mov     bx,word ptr [bp]
        shl     bx,1
        jae     L0DF9
        jmp     L0EF0
L0DF9:
        mov     si,word ptr ss:[bx+95Eh]
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
        mov     al,byte ptr ss:[bx+3B62h]
        and     al,byte ptr ss:[bx+3CA6h]
        add     cx,3
        shr     cx,2
        out     dx,al
L0E26:
        movsb
        add     si,3
        loop    L0E26
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
        jne     L0E45
        inc     di
L0E45:
        dec     bp
        jne     L0E4B
        jmp     L0EEC
L0E4B:
        mov     cx,bp
        mov     al,byte ptr ss:[bx+3B62h]
        and     al,byte ptr ss:[bx+3CA6h]
        add     cx,3
        shr     cx,2
        out     dx,al
L0E5E:
        movsb
        add     si,3
        loop    L0E5E
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
        jne     L0E7D
        inc     di
L0E7D:
        dec     bp
        je      L0EEC
        mov     cx,bp
        mov     al,byte ptr ss:[bx+3B62h]
        and     al,byte ptr ss:[bx+3CA6h]
        add     cx,3
        shr     cx,2
        out     dx,al
L0E93:
        movsb
        add     si,3
        loop    L0E93
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
        jne     L0EB2
        inc     di
L0EB2:
        dec     bp
        je      L0EEC
        mov     cx,bp
        mov     al,byte ptr ss:[bx+3B62h]
        and     al,byte ptr ss:[bx+3CA6h]
        add     cx,3
        shr     cx,2
        out     dx,al
L0EC8:
        movsb
        add     si,3
        loop    L0EC8
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
        jne     L0EE7
        inc     di
L0EE7:
        dec     bp
        je      L0EEC
        mov     cx,bp
L0EEC:
        pop     bp
        jmp     L0DEF
L0EF0:
        mov     dx,GC_INDEX
        mov     ax,0FF08h
        out     dx,ax
        pop     es
        pop     ds
        ret
        add     sp,2
        jmp     L0EF0
_seg003_DE3 endp

; seg003_EFF  (+EFF)
; fbuf_setcolor (FM Towns): AX = a pen number (negative: no change). Sets the colour bytes 410E
; and 4114 from the tables at 21E and 434, and, when the pen's mode (the byte table at
; seg048:0008) is below 14h, the span writer 4110 from the table at 0AF4; so a pen chooses
; both a colour and a way of writing it (solid, copy, save, restore ...).
;
; After its ret: L0F25 (the current span's y, whose low bit picks the dither phase) and the two
; smooth-span vectors L0F27 and L0F29, which seg004 sets (INTERP.ASM's opcode D6h to 0BC7h and
; 0C93h, solid; opcode D8h and SMOOTH.ASM to 0BF2h and 0CD9h, translucent). L0F2B
; (smooth_span_same, solid, both ends the same shade) draws the span in one colour,
; lightabs[shade][base colour] with the base colour at seg004:6950 (the byte PGCACHE.ASM keeps
; for seg003; UW2 6D20), through fake_fb_solid_ylr. The code at 0BF2 (trans_span_same) remaps the
; pixels
; already in the span through one lightabs row.
_seg003_EFF proc near
        shl     ax,1
        jb      L0F23
        mov     bx,ax
        mov     ax,word ptr [bx+21Eh]
        mov     word ptr ds:[410Eh],ax
        mov     ax,word ptr [bx+434h]
        mov     word ptr ds:[PEN_WORD2],ax
        db      8Bh,9Fh,8h,0h                   ; match: mov bx,word ptr [bx+8]: disp16, the table at seg048:0008 (TASM gives disp8 for a number, and a label there would need that segment)
        cmp     bx,14h
        jae     L0F23
        mov     ax,word ptr [bx+0AF4h]
        mov     word ptr ds:[SPAN_WRITER],ax
L0F23:
        ret
L0F24:
        ret
L0F25   dw      0
L0F27   dw      offset L0F2B            ; span routines _seg003_F77 jumps through
L0F29   dw      offset L0FF7
L0F2B:
        push    si
        push    es
        mov     al,byte ptr cs:[0CEC0h] ; UW1: the base colour through CS, 0090:CEC0 being seg004's
                                        ; 06E7:6950 (UW2 loads ES first and reads seg004:6D20)
        mov     ah,byte ptr ds:[0B0Ah]
        mov     bx,ax
        mov     ax,seg seg004
        mov     es,ax
        mov     al,byte ptr es:[bx+696Eh]
        mov     byte ptr ds:[PEN_COLOR],al
        or      byte ptr [si-3],80h
        sub     si,0Ah
        call    _seg003_D2D
        pop     es
        pop     si
        and     byte ptr [si-3],7Fh
        jmp     short _seg003_F77
        push    ds
        push    es
        mov     dh,byte ptr ss:[0B0Ah]
        mov     ds,word ptr ss:[95Ah]
        mov     ax,seg seg004
        mov     es,ax
L0F67:
        mov     dl,byte ptr [di]
        mov     bx,dx
        mov     bh,byte ptr es:[bx+696Eh]
        mov     byte ptr [di],bh
        inc     di
        loop    L0F67
        pop     es
        pop     ds
_seg003_EFF endp

; seg003_F77  (+F77)
; vga_smooth_ylrii (FM Towns): draw a table of Gouraud spans into the frame buffer. In: SI =
; 10-byte records (y, left x, right x, left intensity, right intensity; GRLIBM.ASM's table at
; 5692), ended by a y with its top bit set. For each, the intensity step per pixel is formed as
; 8.8 and the span drawn through L0F29, or through L0F27 when both ends have the same intensity.
;
; L0FF7 (do_filled_dither, solid) writes lightabs[intensity][base colour]; the code after L1037
; (do_trans_dither, translucent) writes lightabs[intensity][the pixel already there]. Both
; dither: two intensity accumulators 40h apart alternate pixel by pixel, and which starts
; depends on the row's parity, so a shade between two lightabs rows comes out as a checkerboard
; of both.
_seg003_F77 proc near
        lodsw
        mov     word ptr cs:L0F25,ax
        shl     ax,1
        jb      L0F24
        mov     di,ax
        mov     di,word ptr ss:[di+95Eh]
        lodsw
        xchg    dx,ax
        lodsw
        xchg    cx,ax
        lodsw
        xchg    bx,ax
        lodsw
        mov     bp,cx
        sub     cx,dx
        jge     L0F99
        neg     cx
        xchg    bp,dx
        xchg    bx,ax
L0F99:
        mov     word ptr ss:[0B0Ah],bx
        mov     word ptr ss:[0B0Ch],dx
        add     di,dx
        inc     cx
        sub     ax,bx
        jne     L0FAF
        jmp     word ptr cs:L0F27
L0FAF:
        or      ax,ax
        jns     L0FCE
        or      cx,cx
        jns     L0FBD
        neg     ax
        neg     cx
        jmp     short L0FD8
L0FBD:
        neg     ax
L0FBF:
        cwd
        idiv    cx
        mov     bh,al
        sub     ax,ax
        div     cx
        mov     bl,ah
        neg     bx
        jmp     short L0FE3
L0FCE:
        test    cx,0FFFFh
        jns     L0FD8
        neg     cx
        jmp     L0FBF
L0FD8:
        cwd
        idiv    cx
        mov     bh,al
        sub     ax,ax
        div     cx
        mov     bl,ah
L0FE3:
        mov     bp,bx
        mov     bh,byte ptr ss:[0B0Ah]
        mov     bl,80h
        push    ds
        mov     ds,word ptr ss:[95Ah]
        jmp     word ptr cs:L0F29
L0FF7:
        push    si
        push    es
        mov     ax,cx
        mov     cx,bx
        mov     dx,bx
        shl     bp,1
        sub     dx,40h
        add     cx,40h
        shr     word ptr cs:L0F25,1
        jae     L1010
        xchg    cx,dx
L1010:
        mov     bx,seg seg004
        mov     es,bx
        mov     bl,byte ptr es:[6950h]
        mov     si,696Eh
L101D:
        mov     bh,dh
        mov     bh,byte ptr es:[bx+si]
        mov     byte ptr [di],bh
        add     dx,bp
        inc     di
        dec     ax
        je      L1037
        mov     bh,ch
        mov     bh,byte ptr es:[bx+si]
        mov     byte ptr [di],bh
        add     cx,bp
        inc     di
        dec     ax
        jne     L101D
L1037:
        pop     es
        pop     si
        pop     ds
        jmp     _seg003_F77
        push    si
        mov     ax,cx
        mov     cx,bx
        shl     bp,1
        sub     bx,40h
        add     cx,40h
        shr     word ptr cs:L0F25,1
        jae     L1053
        xchg    cx,bx
L1053:
        mov     dx,bx
        push    es
        mov     bx,seg seg004
        mov     es,bx
        mov     si,696Eh
L105E:
        mov     bl,byte ptr [di]
        mov     bh,dh
        mov     bh,byte ptr es:[bx+si]
        mov     byte ptr [di],bh
        add     dx,bp
        inc     di
        dec     ax
        je      L107C
        mov     bl,byte ptr [di]
        mov     bh,ch
        mov     bh,byte ptr es:[bx+si]
        mov     byte ptr [di],bh
        add     cx,bp
        inc     di
        dec     ax
        jne     L105E
L107C:
        pop     es
        pop     si
        pop     ds
        jmp     _seg003_F77
_seg003_F77 endp

SEG003_TEXT ends

        end
