; /* target: seg003_0272_5370 */
; /* opts: /ml */
;
; Graphics library module 12: the bitmap span transfers. VIDMODE.ASM's bitmap routines (show,
; fbshow, vcopy and the rest) clip a bitmap's rectangle to the window, write one 8-byte record
; per row (y, left x, right x, source offset; the list ends with a y whose top bit is set) and
; call the transfer they chose through seg_370D:0DC2. The transfers here move those rows to or
; from the mode X screen or the linear frame buffer:
;
;   _5372  a linear (one byte per pixel) bitmap to the planar screen, opaque          (show)
;   _5467  the same, transparent: colour 0 is skipped                                 (show)
;   _5578  video memory to the screen when source and destination planes differ:
;          each plane read separately (read map select, 3CEh index 4), opaque
;   _568B  video memory to the screen, plane-aligned: a latched copy, four pixels a byte
;   _56E3  video memory to the screen, plane by plane, transparent
;   _581B  screen to screen (vcopy), latched, left to right; _586D the same right to left,
;          for overlapping areas
;   _58C7  a linear bitmap to the linear frame buffer (segment 39D0, row table 39D2), opaque
;   _58F8  the same, transparent
;   _5869  set the graphics controller's bit mask back to 0FFh (its tail ends the copies)
; Planar writes go one plane at a time: the sequencer map mask (port 3C5h) is set from the
; tables at 3B64 and 3CA8 by x, and the source is stepped by 4. A latched copy writes the bit
; mask (3CFh, index 8) as 0 so every byte comes from the latches.
;
; In: SI = the first row record; DS = seg_370D, the source segment in 55EC (for a linear source).
; Data: 3DFC the screen segment; 36AC the screen's row addresses; 55E2, 55E6, 55E8 the current
; row's address, right x and pixel count.
;
; name: descriptive (module 12 of seg003); its job, found here, is the bitmap transfers. Only
; _5869 has a proc of its own in the target table; the entries are labels VIDMODE.ASM names. FM
; Towns' counterparts (its show and fbshow family) are not in its symbol table by these entry
; points.
;
; SEG003 split into its original modules: this is module 12 of 14, +5370..+5934 of the segment
; (file 87C0h), a module of the second library (UWLIB.LIB).
; Procs: seg003_0272_5869.
; Evidence for the bounds: starts after one zero byte following a retf: TLINK's padding before
; a word-aligned module (TASM 2.0's `even` pads with 90h).
; It has no relocations.

; The VGA ports used here.
SC_DATA         equ     3C5h  ; sequencer data
GC_INDEX        equ     3CEh  ; graphics controller index
GC_DATA         equ     3CFh  ; graphics controller data

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        public  _seg003_0272_5372
        public  _seg003_0272_5467
        public  _seg003_0272_5578
        public  _seg003_0272_568B
        public  _seg003_0272_56E3
        public  _seg003_0272_581B
        public  _seg003_0272_5869
        public  _seg003_0272_586D
        public  _seg003_0272_58C7
        public  _seg003_0272_58F8

L5370:
        pop     es
        retn
; _5372: linear bitmap rows to the planar screen, opaque. For each row, four passes, one per
; plane: set the map mask for the plane, then copy every fourth source byte.
_seg003_0272_5372:
        mov     dx,SC_DATA
        push    es
        mov     es,word ptr ds:[3DFCh]
L537A:
        lodsw
        shl     ax,1
        jb      L5370
        mov     bp,ax
        mov     ax,word ptr [bp+36ACh]
        mov     word ptr ds:[55E2h],ax
        lodsw
        mov     bx,ax
        lodsw
        mov     word ptr ds:[55E6h],ax
        lodsw
        mov     cx,ax
        push    si
        push    ds
        mov     ds,word ptr ds:[55ECh]
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+3B64h]
        and     al,byte ptr [bp+3CA8h]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E2h]
        neg     bp
        add     bp,word ptr ss:[55E6h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L53CA
        xchg    bp,cx
L53C2:
        movsb
        add     si,3
        loop    L53C2
        xchg    bp,cx
L53CA:
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+3B64h]
        and     al,byte ptr [bp+3CA8h]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E2h]
        neg     bp
        add     bp,word ptr ss:[55E6h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L53FC
        xchg    bp,cx
L53F4:
        movsb
        add     si,3
        loop    L53F4
        xchg    bp,cx
L53FC:
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+3B64h]
        and     al,byte ptr [bp+3CA8h]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E2h]
        neg     bp
        add     bp,word ptr ss:[55E6h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L542E
        xchg    bp,cx
L5426:
        movsb
        add     si,3
        loop    L5426
        xchg    bp,cx
L542E:
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+3B64h]
        and     al,byte ptr [bp+3CA8h]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E2h]
        neg     bp
        add     bp,word ptr ss:[55E6h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L5460
        xchg    bp,cx
L5458:
        movsb
        add     si,3
        loop    L5458
        xchg    bp,cx
L5460:
        pop     ds
        pop     si
        jmp     L537A
L5465:
        pop     es
        retn
; _5467: as _5372, skipping source bytes of colour 0 (transparent).
_seg003_0272_5467:
        mov     dx,SC_DATA
        push    es
        mov     es,word ptr ds:[3DFCh]
L546F:
        lodsw
        shl     ax,1
        jb      L5465
        mov     bp,ax
        mov     ax,word ptr [bp+36ACh]
        mov     word ptr ds:[55E2h],ax
        lodsw
        mov     bx,ax
        lodsw
        mov     word ptr ds:[55E6h],ax
        lodsw
        mov     cx,ax
        xor     ah,ah
        push    si
        push    ds
        mov     ds,word ptr ds:[55ECh]
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+3B64h]
        and     al,byte ptr [bp+3CA8h]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E2h]
        neg     bp
        add     bp,word ptr ss:[55E6h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L54C8
        xchg    bp,cx
L54B9:
        lodsb
        add     si,3
        cmp     al,ah
        je      L54C3
        stosb
        dec     di
L54C3:
        inc     di
        loop    L54B9
        xchg    bp,cx
L54C8:
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+3B64h]
        and     al,byte ptr [bp+3CA8h]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E2h]
        neg     bp
        add     bp,word ptr ss:[55E6h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L5501
        xchg    bp,cx
L54F2:
        lodsb
        add     si,3
        cmp     al,ah
        je      L54FC
        stosb
        dec     di
L54FC:
        inc     di
        loop    L54F2
        xchg    bp,cx
L5501:
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+3B64h]
        and     al,byte ptr [bp+3CA8h]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E2h]
        neg     bp
        add     bp,word ptr ss:[55E6h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L553A
        xchg    bp,cx
L552B:
        lodsb
        add     si,3
        cmp     al,ah
        je      L5535
        stosb
        dec     di
L5535:
        inc     di
        loop    L552B
        xchg    bp,cx
L553A:
        mov     bp,bx
        mov     di,bp
        mov     al,byte ptr [bp+3B64h]
        and     al,byte ptr [bp+3CA8h]
        out     dx,al
        sar     di,1
        sar     di,1
        add     di,word ptr ss:[55E2h]
        neg     bp
        add     bp,word ptr ss:[55E6h]
        sar     bp,1
        sar     bp,1
        mov     si,cx
        inc     bx
        inc     cx
        inc     bp
        je      L5573
        xchg    bp,cx
L5564:
        lodsb
        add     si,3
        cmp     al,ah
        je      L556E
        stosb
        dec     di
L556E:
        inc     di
        loop    L5564
        xchg    bp,cx
L5573:
        pop     ds
        pop     si
        jmp     L546F
; _5578: rows from video memory (ES:offset) to the screen when the planes do not line up: for
; each plane, select it for reading (3CEh index 4) and writing, and copy its bytes. Ends by
; setting the bit mask back to 0FFh.
_seg003_0272_5578:
        push    es
        mov     es,word ptr ds:[3DFCh]
        mov     bp,si
L557F:
        mov     bx,word ptr [bp]
        shl     bx,1
        jae     L5589
        jmp     L5682
L5589:
        mov     di,word ptr [bx+36ACh]
        mov     word ptr ds:[55E2h],di
        mov     bx,word ptr [bp+2]
        mov     di,word ptr [bp+4]
        mov     ax,word ptr [bp+6]
        add     bp,8
        push    bp
        mov     bp,ax
        sub     di,bx
        inc     di
        mov     word ptr ds:[55E8h],di
        xor     ah,ah
        mov     dx,GC_INDEX
        mov     al,4
        out     dx,al
        mov     al,byte ptr [bx+3B64h]
        and     al,byte ptr [bx+3CA8h]
        mov     dx,SC_DATA
        out     dx,al
        mov     al,ah
        mov     dx,GC_DATA
        out     dx,al
        mov     cx,word ptr ds:[55E8h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E2h]
        rep movs byte ptr es:[di],byte ptr es:[si]
        inc     bx
        inc     ah
        dec     word ptr ds:[55E8h]
        jne     L55E5
        jmp     L567E
L55E5:
        mov     al,byte ptr [bx+3B64h]
        and     al,byte ptr [bx+3CA8h]
        mov     dx,SC_DATA
        out     dx,al
        mov     al,ah
        mov     dx,GC_DATA
        out     dx,al
        mov     cx,word ptr ds:[55E8h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E2h]
        rep movs byte ptr es:[di],byte ptr es:[si]
        inc     bx
        inc     ah
        dec     word ptr ds:[55E8h]
        je      L567E
        mov     al,byte ptr [bx+3B64h]
        and     al,byte ptr [bx+3CA8h]
        mov     dx,SC_DATA
        out     dx,al
        mov     al,ah
        mov     dx,GC_DATA
        out     dx,al
        mov     cx,word ptr ds:[55E8h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E2h]
        rep movs byte ptr es:[di],byte ptr es:[si]
        inc     bx
        inc     ah
        dec     word ptr ds:[55E8h]
        je      L567E
        mov     al,byte ptr [bx+3B64h]
        and     al,byte ptr [bx+3CA8h]
        mov     dx,SC_DATA
        out     dx,al
        mov     al,ah
        mov     dx,GC_DATA
        out     dx,al
        mov     cx,word ptr ds:[55E8h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E2h]
        rep movs byte ptr es:[di],byte ptr es:[si]
        inc     bx
        inc     ah
        dec     word ptr ds:[55E8h]
        je      L567E
L567E:
        pop     bp
        jmp     L557F
L5682:
        mov     dx,GC_INDEX
        mov     ax,0FF08h
        out     dx,ax
        pop     es
        retn
; _568B: rows from video memory to the screen, plane-aligned: bit mask 0, so the bytes come from
; the latches; whole groups of four pixels with all planes enabled, then the last partial group
; with the right-edge plane mask.
_seg003_0272_568B:
        mov     dx,GC_DATA
        xor     al,al
        out     dx,al
        mov     dx,SC_DATA
        push    es
        mov     es,word ptr ds:[3DFCh]
        mov     bx,si
L569B:
        mov     bp,word ptr [bx]
        shl     bp,1
        jb      L56DB
        mov     di,word ptr [bp+36ACh]
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
        je      L56D2
        mov     al,0Fh
        out     dx,al
        rep movs byte ptr es:[di],byte ptr es:[si]
        mov     cx,bp
        and     cx,3
        sub     cx,3
        je      L569B
L56D2:
        mov     al,byte ptr [bp+3CA8h]
        out     dx,al
        movs    byte ptr es:[di],byte ptr es:[si]
        jmp     L569B
L56DB:
        pop     es
        mov     dx,GC_DATA
        mov     al,0FFh
        out     dx,al
        retn
; _56E3: as _5578, skipping colour 0.
_seg003_0272_56E3:
        push    es
        mov     es,word ptr ds:[3DFCh]
L56E8:
        lodsw
        shl     ax,1
        jae     L56F0
        jmp     L5812
L56F0:
        mov     bx,ax
        mov     di,word ptr [bx+36ACh]
        mov     word ptr ds:[55E2h],di
        lodsw
        mov     bx,ax
        lodsw
        mov     di,ax
        lodsw
        mov     bp,ax
        sub     di,bx
        inc     di
        mov     word ptr ds:[55E8h],di
        push    si
        xor     ah,ah
        mov     dx,GC_INDEX
        mov     al,4
        out     dx,al
        mov     al,byte ptr [bx+3B64h]
        and     al,byte ptr [bx+3CA8h]
        mov     dx,SC_DATA
        out     dx,al
        mov     al,ah
        mov     dx,GC_DATA
        out     dx,al
        mov     cx,word ptr ds:[55E8h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E2h]
L573A:
        lods    byte ptr es:[si]
        or      al,al
        je      L5742
        stosb
        dec     di
L5742:
        inc     di
        loop    L573A
        inc     bx
        inc     ah
        and     ah,3
        dec     word ptr ds:[55E8h]
        jne     L5754
        jmp     L580E
L5754:
        mov     al,byte ptr [bx+3B64h]
        and     al,byte ptr [bx+3CA8h]
        mov     dx,SC_DATA
        out     dx,al
        mov     al,ah
        mov     dx,GC_DATA
        out     dx,al
        mov     cx,word ptr ds:[55E8h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E2h]
L577B:
        lods    byte ptr es:[si]
        or      al,al
        je      L5783
        stosb
        dec     di
L5783:
        inc     di
        loop    L577B
        inc     bx
        inc     ah
        and     ah,3
        dec     word ptr ds:[55E8h]
        je      L580E
        mov     al,byte ptr [bx+3B64h]
        and     al,byte ptr [bx+3CA8h]
        mov     dx,SC_DATA
        out     dx,al
        mov     al,ah
        mov     dx,GC_DATA
        out     dx,al
        mov     cx,word ptr ds:[55E8h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E2h]
L57B9:
        lods    byte ptr es:[si]
        or      al,al
        je      L57C1
        stosb
        dec     di
L57C1:
        inc     di
        loop    L57B9
        inc     bx
        inc     ah
        and     ah,3
        dec     word ptr ds:[55E8h]
        je      L580E
        mov     al,byte ptr [bx+3B64h]
        and     al,byte ptr [bx+3CA8h]
        mov     dx,SC_DATA
        out     dx,al
        mov     al,ah
        mov     dx,GC_DATA
        out     dx,al
        mov     cx,word ptr ds:[55E8h]
        add     cx,3
        shr     cx,2
        mov     si,bp
        mov     di,bx
        shr     di,2
        add     di,word ptr ds:[55E2h]
L57F7:
        lods    byte ptr es:[si]
        or      al,al
        je      L57FF
        stosb
        dec     di
L57FF:
        inc     di
        loop    L57F7
        inc     bx
        inc     ah
        and     ah,3
        dec     word ptr ds:[55E8h]
        je      L580E
L580E:
        pop     si
        jmp     L56E8
L5812:
        mov     dx,GC_INDEX
        mov     ax,0FF08h
        out     dx,ax
        pop     es
        retn
; _581B: screen to screen through the latches, left to right (vcopy); the record's source offset
; is the destination here and the row address the source.
_seg003_0272_581B:
        mov     dx,GC_DATA
        xor     al,al
        out     dx,al
        mov     dx,SC_DATA
        push    es
        mov     es,word ptr ds:[3DFCh]
        mov     bx,si
L582B:
        mov     bp,word ptr [bx]
        shl     bp,1
        jb      L5865
        mov     si,word ptr [bp+36ACh]
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
        je      L585C
        mov     al,0Fh
        out     dx,al
        rep movs byte ptr es:[di],byte ptr es:[si]
        mov     cx,bp
        je      L582B
L585C:
        mov     al,byte ptr [bp+3CA8h]
        out     dx,al
        movs    byte ptr es:[di],byte ptr es:[si]
        jmp     L582B
L5865:
        pop     es
        mov     dx,GC_DATA

; seg003_0272_5869  (+5869)
; _5869: the bit mask back to 0FFh (DX = 3CFh), the tail of _581B. _586D is _581B right to left
; (std), for a copy onto an overlapping area further right.
_seg003_0272_5869 proc near
        mov     al,0FFh
        out     dx,al
        ret
_seg003_0272_586D:
        mov     dx,GC_DATA
        xor     al,al
        out     dx,al
        mov     dx,SC_DATA
        push    es
        mov     es,word ptr ds:[3DFCh]
        mov     bx,si
        std
L587E:
        mov     bp,word ptr [bx]
        shl     bp,1
        jb      L58BE
        mov     si,word ptr [bp+36ACh]
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
        je      L58B5
        mov     al,byte ptr [bp+3CA8h]
        out     dx,al
        movs    byte ptr es:[di],byte ptr es:[si]
        jcxz    L587E
L58B5:
        inc     cx
        mov     al,0Fh
        out     dx,al
        rep movs byte ptr es:[di],byte ptr es:[si]
        jmp     L587E
L58BE:
        cld
        pop     es
        mov     dx,GC_DATA
        mov     al,0FFh
        out     dx,al
        ret
; _58C7: linear bitmap rows to the linear frame buffer (segment 39D0, row offsets 39D2), opaque,
; with rep movsb.
_seg003_0272_58C7:
        push    es
        push    ds
        mov     es,word ptr ss:[39D0h]
        mov     ds,word ptr ss:[55ECh]
L58D3:
        lods    word ptr ss:[si]
        shl     ax,1
        jb      L58F5
        mov     bx,ax
        mov     di,word ptr ss:[bx+39D2h]
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
        jmp     L58D3
L58F5:
        pop     ds
        pop     es
        ret
; _58F8: as _58C7, skipping colour 0.
_seg003_0272_58F8:
        push    es
        push    ds
        mov     es,word ptr ss:[39D0h]
        mov     ds,word ptr ss:[55ECh]
L5904:
        lods    word ptr ss:[si]
        shl     ax,1
        jb      L5930
        mov     bx,ax
        mov     di,word ptr ss:[bx+39D2h]
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
L5923:
        lodsb
        or      al,al
        je      L592A
        stosb
        dec     di
L592A:
        inc     di
        loop    L5923
        pop     si
        jmp     L5904
L5930:
        pop     ds
        pop     es
        ret
_seg003_0272_5869 endp

SEG003_TEXT ends

        end
