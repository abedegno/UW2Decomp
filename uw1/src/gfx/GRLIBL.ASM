; /* target: seg003_5B6A */
; /* opts: /ml */
;
; Graphics library module 16: the bitmap span transfers. VIDMODE.ASM's bitmap routines (show,
; fbshow, vcopy and the rest) clip a bitmap's rectangle to the window, write one 8-byte record
; per row (y, left x, right x, source offset; the list ends with a y whose top bit is set) and
; call the transfer they chose through seg048:0DC2. The transfers here move those rows to or
; from the mode X screen or the linear frame buffer:
;
;   _5B6C  a linear (one byte per pixel) bitmap to the planar screen, opaque          (show)
;   _5C61  the same, transparent: colour 0 is skipped                                 (show)
;   _5D72  video memory to the screen when source and destination planes differ:
;          each plane read separately (read map select, 3CEh index 4), opaque
;   _5E85  video memory to the screen, plane-aligned: a latched copy, four pixels a byte
;   _5EDD  video memory to the screen, plane by plane, transparent
;   _6015  screen to screen (vcopy), latched, left to right; _606D the same right to left,
;          for overlapping areas
;   _60C7  a linear bitmap to the linear frame buffer (segment 39CE, row table 39D0), opaque
;   _60F8  the same, transparent
;   _6069  set the graphics controller's bit mask back to 0FFh (its tail ends the copies)
; Planar writes go one plane at a time: the sequencer map mask (port 3C5h) is set from the
; tables at 3B62 and 3CA6 by x, and the source is stepped by 4. A latched copy writes the bit
; mask (3CFh, index 8) as 0 so every byte comes from the latches.
;
; In: SI = the first row record; DS = seg048, the source segment in 55EA (for a linear source).
; Data: 3DFA the screen segment; 36AA the screen's row addresses; 55E0, 55E4, 55E6 the current
; row's address, right x and pixel count.
;
; name: descriptive (UW2Decomp: module 12 of seg003); its job, found here, is the bitmap
; transfers. Its
; entries are labels VIDMODE.ASM names, each a row of the target table. FM
; Towns' counterparts (its show and fbshow family) are not in its symbol table by these entry
; points.
;
; UW1 against UW2: UW2Decomp's src/gfx/GRLIBL.ASM (UW2's seg003_0272_5370) with one change, the
; add cx,3 / sub cx,3 pair in _6015 (6 bytes more, 5C9h in all); the data sits 2 bytes lower
; (36AA for UW2's 36AC, 55EA for 55EC).
;
; SEG003 split into its original modules: this is module 16 of UW1's 18, +5B6A..+6133.
; Evidence for the bounds: starts straight after GRDISP's zero padding byte at +5B69, as in UW2;
; it ends at the zero byte at +6133.
; It has no relocations.

; The VGA ports used here.
SC_DATA         equ     3C5h  ; sequencer data
GC_INDEX        equ     3CEh  ; graphics controller index
GC_DATA         equ     3CFh  ; graphics controller data

; The graphics library's data in seg048 (DS while the library runs), as GRLIBF.ASM's
; and VIDMODE.ASM's headers describe it. The names are ours.
YTAB            equ     36AAh   ; the drawing page's row addresses (Ytab; row 0 the bottom line)
LMASKS          equ     3B62h   ; the left-edge plane masks, by x
RMASKS          equ     3CA6h   ; the right-edge plane masks, by x
SCREEN_SEG      equ     3DFAh   ; the screen's segment

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        public  _seg003_5B6C
        public  _seg003_5C61
        public  _seg003_5D72
        public  _seg003_5E85
        public  _seg003_5EDD
        public  _seg003_6015
        public  _seg003_6069
        public  _seg003_606D
        public  _seg003_60C7
        public  _seg003_60F8

L5B6A:
        pop     es
        retn
; _5B6C: linear bitmap rows to the planar screen, opaque. For each row, four passes, one per
; plane: set the map mask for the plane, then copy every fourth source byte.
_seg003_5B6C:
        mov     dx,SC_DATA
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
L5B74:
        lodsw
        shl     ax,1
        jb      L5B6A
        mov     bp,ax
        mov     ax,word ptr [bp+YTAB]
        mov     word ptr ds:[55E0h],ax
        lodsw
        mov     bx,ax
        lodsw
        mov     word ptr ds:[55E4h],ax
        lodsw
        mov     cx,ax
        push    si
        push    ds
        mov     ds,word ptr ds:[55EAh]
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+LMASKS]
        and     al,byte ptr [bp+RMASKS]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E0h]
        neg     bp
        add     bp,word ptr ss:[55E4h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L5BC4
        xchg    bp,cx
L5BBC:
        movsb
        add     si,3
        loop    L5BBC
        xchg    bp,cx
L5BC4:
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+LMASKS]
        and     al,byte ptr [bp+RMASKS]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E0h]
        neg     bp
        add     bp,word ptr ss:[55E4h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L5BF6
        xchg    bp,cx
L5BEE:
        movsb
        add     si,3
        loop    L5BEE
        xchg    bp,cx
L5BF6:
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+LMASKS]
        and     al,byte ptr [bp+RMASKS]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E0h]
        neg     bp
        add     bp,word ptr ss:[55E4h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L5C28
        xchg    bp,cx
L5C20:
        movsb
        add     si,3
        loop    L5C20
        xchg    bp,cx
L5C28:
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+LMASKS]
        and     al,byte ptr [bp+RMASKS]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E0h]
        neg     bp
        add     bp,word ptr ss:[55E4h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L5C5A
        xchg    bp,cx
L5C52:
        movsb
        add     si,3
        loop    L5C52
        xchg    bp,cx
L5C5A:
        pop     ds
        pop     si
        jmp     L5B74
L5C5F:
        pop     es
        retn
; _5C61: as _5B6C, skipping source bytes of colour 0 (transparent).
_seg003_5C61:
        mov     dx,SC_DATA
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
L5C69:
        lodsw
        shl     ax,1
        jb      L5C5F
        mov     bp,ax
        mov     ax,word ptr [bp+YTAB]
        mov     word ptr ds:[55E0h],ax
        lodsw
        mov     bx,ax
        lodsw
        mov     word ptr ds:[55E4h],ax
        lodsw
        mov     cx,ax
        xor     ah,ah
        push    si
        push    ds
        mov     ds,word ptr ds:[55EAh]
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+LMASKS]
        and     al,byte ptr [bp+RMASKS]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E0h]
        neg     bp
        add     bp,word ptr ss:[55E4h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L5CC2
        xchg    bp,cx
L5CB3:
        lodsb
        add     si,3
        cmp     al,ah
        je      L5CBD
        stosb
        dec     di
L5CBD:
        inc     di
        loop    L5CB3
        xchg    bp,cx
L5CC2:
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+LMASKS]
        and     al,byte ptr [bp+RMASKS]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E0h]
        neg     bp
        add     bp,word ptr ss:[55E4h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L5CFB
        xchg    bp,cx
L5CEC:
        lodsb
        add     si,3
        cmp     al,ah
        je      L5CF6
        stosb
        dec     di
L5CF6:
        inc     di
        loop    L5CEC
        xchg    bp,cx
L5CFB:
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+LMASKS]
        and     al,byte ptr [bp+RMASKS]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E0h]
        neg     bp
        add     bp,word ptr ss:[55E4h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L5D34
        xchg    bp,cx
L5D25:
        lodsb
        add     si,3
        cmp     al,ah
        je      L5D2F
        stosb
        dec     di
L5D2F:
        inc     di
        loop    L5D25
        xchg    bp,cx
L5D34:
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+LMASKS]
        and     al,byte ptr [bp+RMASKS]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E0h]
        neg     bp
        add     bp,word ptr ss:[55E4h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L5D6D
        xchg    bp,cx
L5D5E:
        lodsb
        add     si,3
        cmp     al,ah
        je      L5D68
        stosb
        dec     di
L5D68:
        inc     di
        loop    L5D5E
        xchg    bp,cx
L5D6D:
        pop     ds
        pop     si
        jmp     L5C69
; _5D72: rows from video memory (ES:offset) to the screen when the planes do not line up: for
; each plane, select it for reading (3CEh index 4) and writing, and copy its bytes. Ends by
; setting the bit mask back to 0FFh.
_seg003_5D72:
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
        mov     bp,si
L5D79:
        mov     bx,word ptr [bp]
        shl     bx,1
        jae     L5D83
        jmp     L5E7C
L5D83:
        mov     di,word ptr [bx+YTAB]
        mov     word ptr ds:[55E0h],di
        mov     bx,word ptr [bp+2]
        mov     di,word ptr [bp+4]
        mov     ax,word ptr [bp+6]
        add     bp,8
        push    bp
        mov     bp,ax
        sub     di,bx
        inc     di
        mov     word ptr ds:[55E6h],di
        xor     ah,ah
        mov     dx,GC_INDEX
        mov     al,4
        out     dx,al
        mov     al,byte ptr [bx+LMASKS]
        and     al,byte ptr [bx+RMASKS]
        mov     dx,SC_DATA
        out     dx,al
        mov     al,ah
        mov     dx,GC_DATA
        out     dx,al
        mov     cx,word ptr ds:[55E6h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E0h]
        rep movs byte ptr es:[di],byte ptr es:[si]
        inc     bx
        inc     ah
        dec     word ptr ds:[55E6h]
        jne     L5DDF
        jmp     L5E78
L5DDF:
        mov     al,byte ptr [bx+LMASKS]
        and     al,byte ptr [bx+RMASKS]
        mov     dx,SC_DATA
        out     dx,al
        mov     al,ah
        mov     dx,GC_DATA
        out     dx,al
        mov     cx,word ptr ds:[55E6h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E0h]
        rep movs byte ptr es:[di],byte ptr es:[si]
        inc     bx
        inc     ah
        dec     word ptr ds:[55E6h]
        je      L5E78
        mov     al,byte ptr [bx+LMASKS]
        and     al,byte ptr [bx+RMASKS]
        mov     dx,SC_DATA
        out     dx,al
        mov     al,ah
        mov     dx,GC_DATA
        out     dx,al
        mov     cx,word ptr ds:[55E6h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E0h]
        rep movs byte ptr es:[di],byte ptr es:[si]
        inc     bx
        inc     ah
        dec     word ptr ds:[55E6h]
        je      L5E78
        mov     al,byte ptr [bx+LMASKS]
        and     al,byte ptr [bx+RMASKS]
        mov     dx,SC_DATA
        out     dx,al
        mov     al,ah
        mov     dx,GC_DATA
        out     dx,al
        mov     cx,word ptr ds:[55E6h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E0h]
        rep movs byte ptr es:[di],byte ptr es:[si]
        inc     bx
        inc     ah
        dec     word ptr ds:[55E6h]
        je      L5E78
L5E78:
        pop     bp
        jmp     L5D79
L5E7C:
        mov     dx,GC_INDEX
        mov     ax,0FF08h
        out     dx,ax
        pop     es
        retn
; _5E85: rows from video memory to the screen, plane-aligned: bit mask 0, so the bytes come from
; the latches; whole groups of four pixels with all planes enabled, then the last partial group
; with the right-edge plane mask.
_seg003_5E85:
        mov     dx,GC_DATA
        xor     al,al
        out     dx,al
        mov     dx,SC_DATA
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
        mov     bx,si
L5E95:
        mov     bp,word ptr [bx]
        shl     bp,1
        jb      L5ED5
        mov     di,word ptr [bp+YTAB]
        mov     ax,word ptr [bx+2]
        mov     cx,word ptr [bx+4]
        mov     bp,cx
        mov     si,word ptr [bx+6]
        add     bx,8
        sub     cx,ax
        inc     cx
        shr     ax,2
        add     di,ax
        mov     ax,cx
        shr     cx,2
        je      L5ECC
        mov     al,0Fh
        out     dx,al
        rep movs byte ptr es:[di],byte ptr es:[si]
        mov     cx,bp
        and     cx,3
        sub     cx,3
        je      L5E95
L5ECC:
        mov     al,byte ptr [bp+RMASKS]
        out     dx,al
        movs    byte ptr es:[di],byte ptr es:[si]
        jmp     L5E95
L5ED5:
        pop     es
        mov     dx,GC_DATA
        mov     al,0FFh
        out     dx,al
        retn
; _5EDD: as _5D72, skipping colour 0.
_seg003_5EDD:
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
L5EE2:
        lodsw
        shl     ax,1
        jae     L5EEA
        jmp     L600C
L5EEA:
        mov     bx,ax
        mov     di,word ptr [bx+YTAB]
        mov     word ptr ds:[55E0h],di
        lodsw
        mov     bx,ax
        lodsw
        mov     di,ax
        lodsw
        mov     bp,ax
        sub     di,bx
        inc     di
        mov     word ptr ds:[55E6h],di
        push    si
        xor     ah,ah
        mov     dx,GC_INDEX
        mov     al,4
        out     dx,al
        mov     al,byte ptr [bx+LMASKS]
        and     al,byte ptr [bx+RMASKS]
        mov     dx,SC_DATA
        out     dx,al
        mov     al,ah
        mov     dx,GC_DATA
        out     dx,al
        mov     cx,word ptr ds:[55E6h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E0h]
L5F34:
        lods    byte ptr es:[si]
        or      al,al
        je      L5F3C
        stosb
        dec     di
L5F3C:
        inc     di
        loop    L5F34
        inc     bx
        inc     ah
        and     ah,3
        dec     word ptr ds:[55E6h]
        jne     L5F4E
        jmp     L6008
L5F4E:
        mov     al,byte ptr [bx+LMASKS]
        and     al,byte ptr [bx+RMASKS]
        mov     dx,SC_DATA
        out     dx,al
        mov     al,ah
        mov     dx,GC_DATA
        out     dx,al
        mov     cx,word ptr ds:[55E6h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E0h]
L5F75:
        lods    byte ptr es:[si]
        or      al,al
        je      L5F7D
        stosb
        dec     di
L5F7D:
        inc     di
        loop    L5F75
        inc     bx
        inc     ah
        and     ah,3
        dec     word ptr ds:[55E6h]
        je      L6008
        mov     al,byte ptr [bx+LMASKS]
        and     al,byte ptr [bx+RMASKS]
        mov     dx,SC_DATA
        out     dx,al
        mov     al,ah
        mov     dx,GC_DATA
        out     dx,al
        mov     cx,word ptr ds:[55E6h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E0h]
L5FB3:
        lods    byte ptr es:[si]
        or      al,al
        je      L5FBB
        stosb
        dec     di
L5FBB:
        inc     di
        loop    L5FB3
        inc     bx
        inc     ah
        and     ah,3
        dec     word ptr ds:[55E6h]
        je      L6008
        mov     al,byte ptr [bx+LMASKS]
        and     al,byte ptr [bx+RMASKS]
        mov     dx,SC_DATA
        out     dx,al
        mov     al,ah
        mov     dx,GC_DATA
        out     dx,al
        mov     cx,word ptr ds:[55E6h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E0h]
L5FF1:
        lods    byte ptr es:[si]
        or      al,al
        je      L5FF9
        stosb
        dec     di
L5FF9:
        inc     di
        loop    L5FF1
        inc     bx
        inc     ah
        and     ah,3
        dec     word ptr ds:[55E6h]
        je      L6008
L6008:
        pop     si
        jmp     L5EE2
L600C:
        mov     dx,GC_INDEX
        mov     ax,0FF08h
        out     dx,ax
        pop     es
        retn
; _6015: screen to screen through the latches, left to right (vcopy); the record's source offset
; is the destination here and the row address the source.
_seg003_6015:
        mov     dx,GC_DATA
        xor     al,al
        out     dx,al
        mov     dx,SC_DATA
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
        mov     bx,si
L6025:
        mov     bp,word ptr [bx]
        shl     bp,1
        jb      L6065
        mov     si,word ptr [bp+YTAB]
        mov     ax,word ptr [bx+2]
        mov     cx,word ptr [bx+4]
        mov     bp,cx
        mov     di,word ptr [bx+6]
        add     bx,8
        sub     cx,ax
        inc     cx
        shr     ax,2
        add     si,ax
        mov     ax,cx
        shr     cx,2
        je      L605C
        mov     al,0Fh
        out     dx,al
        rep movs byte ptr es:[di],byte ptr es:[si]
        mov     cx,bp
        add     cx,3                    ; UW1 only: add and take back 3, so the je below tests
        sub     cx,3                    ; CX (= BP) for 0; UW2 has neither and jumps on the flags of shr
        je      L6025
L605C:
        mov     al,byte ptr [bp+RMASKS]
        out     dx,al
        movs    byte ptr es:[di],byte ptr es:[si]
        jmp     L6025
L6065:
        pop     es
        mov     dx,GC_DATA

; seg003_6069  (+6069)
; _6069: the bit mask back to 0FFh (DX = 3CFh), the tail of _6015. _606D is _6015 right to left
; (std), for a copy onto an overlapping area further right.
_seg003_6069 proc near
        mov     al,0FFh
        out     dx,al
        ret
_seg003_606D:
        mov     dx,GC_DATA
        xor     al,al
        out     dx,al
        mov     dx,SC_DATA
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
        mov     bx,si
        std
L607E:
        mov     bp,word ptr [bx]
        shl     bp,1
        jb      L60BE
        mov     si,word ptr [bp+YTAB]
        mov     ax,word ptr [bx+2]
        mov     cx,word ptr [bx+4]
        mov     di,word ptr [bx+6]
        add     bx,8
        sub     cx,ax
        inc     cx
        mov     bp,cx
        shr     ax,2
        add     si,ax
        shr     cx,2
        dec     cx
        add     si,cx
        add     di,cx
        test    bp,3
        je      L60B5
        mov     al,byte ptr [bp+RMASKS]
        out     dx,al
        movs    byte ptr es:[di],byte ptr es:[si]
        jcxz    L607E
L60B5:
        inc     cx
        mov     al,0Fh
        out     dx,al
        rep movs byte ptr es:[di],byte ptr es:[si]
        jmp     L607E
L60BE:
        cld
        pop     es
        mov     dx,GC_DATA
        mov     al,0FFh
        out     dx,al
        ret
; _60C7: linear bitmap rows to the linear frame buffer (segment 39CE, row offsets 39D0), opaque,
; with rep movsb.
_seg003_60C7:
        push    es
        push    ds
        mov     es,word ptr ss:[39CEh]
        mov     ds,word ptr ss:[55EAh]
L60D3:
        lods    word ptr ss:[si]
        shl     ax,1
        jb      L60F5
        mov     bx,ax
        mov     di,word ptr ss:[bx+39D0h]
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
        jmp     L60D3
L60F5:
        pop     ds
        pop     es
        ret
; _60F8: as _60C7, skipping colour 0.
_seg003_60F8:
        push    es
        push    ds
        mov     es,word ptr ss:[39CEh]
        mov     ds,word ptr ss:[55EAh]
L6104:
        lods    word ptr ss:[si]
        shl     ax,1
        jb      L6130
        mov     bx,ax
        mov     di,word ptr ss:[bx+39D0h]
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
L6123:
        lodsb
        or      al,al
        je      L612A
        stosb
        dec     di
L612A:
        inc     di
        loop    L6123
        pop     si
        jmp     L6104
L6130:
        pop     ds
        pop     es
        ret
_seg003_6069 endp

SEG003_TEXT ends

        end
