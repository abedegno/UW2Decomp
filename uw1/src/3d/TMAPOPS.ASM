; /* target: seg004_5C30 */
; /* opts: /ml */
;
; The texture-mapping opcode handlers of the model interpreter (INTERP.ASM): each collects one
; polygon's vertices with their texture coordinates and hands it to the texture mapper, or
; draws it flat when texture mapping is off. do_scalebm draws a bitmap scaled to the view (a
; sprite face), through seg003's SCALEBM.ASM.
;
; UW1 against UW2: UW2's handlers are here (do_set_tmctxt .. do_scalebm) with UW1's data
; offsets, but where UW2 hands the polygon to its 386 texture mapper in seg004 (CLIP.ASM,
; PROJPOLY.ASM and the scanline modules, none of which UW1 has), UW1 clips and projects it
; here in 16-bit code and calls the mapper through the far pointer at seg051:0B006h, whose
; offset the handlers set to seg003's wall mapper (_seg003_1DA, WALLMAP.ASM) or its polygon mapper
; (_seg003_545, POLYFILL.ASM). The original named them: TLINK takes those two modules from
; the library only for a reference by name. The UW1-only routines: seg004_6130 projects and draws the textured
; polygon (seg004_6129 is its entry with this module's overflow handler, L692C, installed),
; seg004_624B, 63C6, 6545 and 66C2 clip it against the four planes, carrying the texture
; coordinates. UW1 has no do_bcompact_map (its opcode table ends at 0DAh).
;
; Data (DS = seg051): 305h and 307h the end and start of the polygon being built (vertices of
; 12 bytes: x, y, z, clip codes, u, v), 161Ah and 161Bh the OR and AND of the clip codes,
; 1620h the points, 0B006h the mapper's far pointer, 0B07Ch the current texture's block in the
; table at 0B07Eh.
;
; name: UW2's (descriptive there). Every handler has its FM Towns name, through UW2.
;
; .186, where UW2 has .386: UW1's seg004 has no 386 instruction and none of the nop padding
; that TASM's .386 mode puts after forward conditional jumps.
;
; SEG004 split into its original modules: this is module 8 of 9, +5C30..+6950 of the segment
; (file FCA0h), a module of the second library.
; match: evidence for the bounds: starts on a paragraph after 11 zero bytes following
; INSTANCE's last iret, and ends with 12 zero bytes before PGCACHE.ASM.
; Its relocations are entries 2721-2723 of UW.EXE's relocation table. Entries 2709-2720 come
; right before them: seg051 from 0B006h, starting with the mapper's far pointer (its segment
; word at 0B008h is seg003), as UW2's FD58 piece (mapper_rtn and the texture blocks) comes
; right before UW2's TMAPOPS code. That data is this module's, defined before the code
; because those entries come before the code's: seg051 (segment table entry 58) from 4723:B006
; to the segment's end at C4D9h. Where it starts is not known: it is taken from the mapper's
; far pointer, as in UW2, and the bytes before it are INSTANCE.ASM's piece. The pointer's
; offset is 01DAh in the EXE (the wall mapper); the texture block records follow, their far
; pointers into seg_5DFD and checkerboard1, then UW2's first_anim and the page cache tables.
; name: the labels are symbols.tsv's (provisional, UW2's where it has them).
;
; The routine comments are UW2's, with UW1's addresses, except where marked UW1.

; The model interpreter's opcode table: a word per opcode, the offset of its handler
; (INTERP.ASM), in seg051, the data segment while interpreting (UW2: 24F4h).
OPCODE_TABLE    equ     2738h

        .186

        extrn   seg048:far
        extrn   _seg003_1082:far
        extrn   _seg003_1DA:far
        extrn   _seg003_545:far
        extrn   _checkerboard1:far
        extrn   _seg_5DFD:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

; seg051 from B006h to its end
FD58    segment byte public 'FAR_DATA'
        public  _first_anim, _seg051_C10F, _CmaptoPg, _PgtoCmap, _CmapFrm, _CmapCache
        public  _crit_fpage, _crit_nlpages, _crit_inpage, _tmap_inpage, _seg051_C375
        public  _seg051_C376, _seg051_C377, _seg051_C378, _seg051_C3B2, _seg051_C3EC
        public  _seg051_C460, _obj_inpage1, _EmsBuff, _seg051_C4D7
        dd      _seg003_1DA
        db      0DAh,01h
        db      72h dup (0)
        db      10h,00h
        dd      _seg_5DFD+0FFh
        db      0F0h,00h,10h,00h
        dd      _seg_5DFD+0FFh
        db      0F0h,00h,20h,00h
        dd      _seg_5DFD+3FFh
        db      0E0h,03h,20h,00h
        dd      _seg_5DFD+3FFh
        db      0E0h,03h,40h,00h
        dd      _seg_5DFD+0FFFh
        db      0C0h,0Fh,40h,00h
        dd      _seg_5DFD+0FFFh
        db      0C0h,0Fh,40h,00h
        dd      _seg_5DFD+17FFh
        dd      _checkerboard1+1FC0h
        db      10h,00h,10h,00h,08h,00h,10h,00h,04h,00h
        dd      _checkerboard1+5h
        db      10h,00h,10h,00h,08h,00h,10h,00h,04h,00h
        dd      _seg_5DFD+5h
        db      10h,00h,10h,00h,08h,00h,10h,00h,04h,00h
        dd      _seg_5DFD+5h
        db      10h,00h,10h,00h,08h,00h,10h,00h,04h,00h,05h,00h,00h,00h
_first_anim label byte
        db      00h,00h,00h,00h,00h,00h,01h,00h,01h,00h,63h,72h,69h,74h,5Ch,63h
        db      72h,3Fh,3Fh,70h,61h,67h,65h,2Eh,6Eh,3Fh,3Fh,24h,00h,0FFh
        db      1001h dup (0)
_seg051_C10F label byte
        db      80h dup (0)
_CmaptoPg label byte
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
_PgtoCmap label byte
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
_CmapFrm label byte
        db      0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h
        db      0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h
        db      0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h
        db      0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h
        db      0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h
        db      0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h,0A0h
_CmapCache label byte
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
_crit_fpage label byte
        db      00h,00h
_crit_nlpages label byte
        db      00h,00h
_crit_inpage label byte
        db      00h
_tmap_inpage label byte
        db      00h
_seg051_C375 label byte
        db      00h
_seg051_C376 label byte
        db      00h
_seg051_C377 label byte
        db      00h
_seg051_C378 label byte
        db      3Ah dup (0)
_seg051_C3B2 label byte
        db      3Ah dup (0)
_seg051_C3EC label byte
        db      74h dup (0)
_seg051_C460 label byte
        db      74h dup (0)
_obj_inpage1 label byte
        db      00h
_EmsBuff label byte
        db      00h,00h
_seg051_C4D7 label byte
        db      00h,00h
FD58    ends

SEG004_TEXT segment para public 'code'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _code_pnt:near
        extrn   _overflow_handler_reg:near
        extrn   _smooth_over:near
        extrn   draw_poly_buf_ptr:near
        extrn   overflow_count:word
        public  _do_bcompact_tmap
        public  _do_bcompact_wtmap
        public  _do_compact_map
        public  _do_compact_tmap
        public  _do_compact_wtmap
        public  _do_ctxt_gmap
        public  _do_ctxt_gtmap
        public  _do_gtmap
        public  _do_gtri
        public  _do_gwtmap
        public  _do_scalebm
        public  _do_set_gmap_ctxt
        public  _do_set_tmctxt
        public  _do_tmap
        public  _do_tmap_tri
        public  _do_wtmap
        public  _draw_solid_tmap
        public  _seg004_6129
        public  _seg004_6130
        public  _seg004_624B
        public  _seg004_63C6
        public  _seg004_6545
        public  _seg004_66C2
        public  in_scalebm

; seg004_5C30  (+5C30)
; do_set_tmctxt (opcode 0xB2): operands: a bitmap slot (low byte), then the next opcode. Points
; bmap_blk_ptr (B07C) at slot n of the table at B07E (8 bytes a slot) for the texture-mapping
; opcodes that take their texture from the context (do_ctxt_gtmap, do_ctxt_gmap,
; do_compact_map).
_do_set_tmctxt proc near
        lodsw
        xor     ah,ah
        shl     ax,1
        shl     ax,1
        shl     ax,1
        add     ax,0B07Eh
        mov     word ptr ds:[0B07Ch],ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_set_tmctxt endp

; seg004_5C45  (+5C45)
; do_set_gmap_ctxt (opcode 0xD0): operand: a flag. Sets the g-map context's map routine (B00A):
; 60h (floor-style) when the flag's low byte is 0, else 545h (wall).
_do_set_gmap_ctxt proc near
        lodsw
        test    al,0FFh
        jne     short L5C52
        mov     word ptr ds:[0B00Ah],offset _seg003_1DA
        jmp     short L5C58
L5C52:
        mov     word ptr ds:[0B00Ah],offset _seg003_545
L5C58:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_set_gmap_ctxt endp

; seg004_5C5E  (+5C5E)
; do_ctxt_gtmap: a g-polygon textured from the context slot (do_set_tmctxt), floor-style
; mapping. Falls into do_gwtmap's vertex loop with BP = the context's bitmap block.
_do_ctxt_gtmap proc near
        mov     word ptr ds:[0B006h],offset _seg003_1DA
        mov     bp,word ptr ds:[0B07Ch]
        jmp     short L5C9B
_do_ctxt_gtmap endp

; seg004_5C6A  (+5C6A)
; do_ctxt_gmap: as do_ctxt_gtmap, with the mapping style of the g-map context (B00A).
_do_ctxt_gmap proc near
        mov     bp,word ptr ds:[0B00Ah]
        mov     word ptr ds:[0B006h],bp
        mov     bp,word ptr ds:[0B07Ch]
        jmp     short L5C9B
_do_ctxt_gmap endp

; seg004_5C78  (+5C78)
; do_gtri: does nothing but skip to the next opcode (no operands).
_do_gtri proc near
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_gtri endp

; seg004_5C7E  (+5C7E)
; do_gtmap and do_gwtmap: a textured polygon whose vertices carry fractional texture
; coordinates. Operands: the bitmap slot, the vertex count n, then n triples (point offset into
; resbuf, u, v); u and v are fractions (0..FFFFh) of the texture's width and height, scaled here
; by the block's words 0 and 1 (the `mul` keeps the 8.8 result). do_gtmap maps floor-style
; (60h), do_gwtmap wall-style (545h). The tail at L60E0, shared by every handler in the module,
; records vert_ptr and either draws the polygon flat (polym set) or calls _seg004_6141 to
; texture map it.
_do_gtmap proc near
        mov     word ptr ds:[0B006h],offset _seg003_1DA
        jmp     short L5C8C
_do_gtmap endp

; seg004_5C86  (+5C86)
_do_gwtmap proc near
        mov     word ptr ds:[0B006h],offset _seg003_545
L5C8C:
        lodsw
        shl     ax,1
        shl     ax,1
        shl     ax,1
        add     ax,0B07Eh
        mov     word ptr ds:[0B07Ch],ax
        mov     bp,ax
L5C9B:
        mov     byte ptr ds:[161Ah],0
        mov     byte ptr ds:[161Bh],0FFh
        mov     di,309h
        mov     word ptr ds:[307h],di
        lodsw
        mov     cx,ax
L5CAF:
        lodsw
        mov     bx,ax
        mov     ax,word ptr [bx+1620h]
        stosw
        mov     ax,word ptr [bx+1622h]
        stosw
        mov     ax,word ptr [bx+1624h]
        stosw
        mov     al,byte ptr [bx+1626h]
        stosw
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        lodsw
        mul     word ptr ds:[bp]
        mov     al,ah
        mov     ah,dl
        stosw
        lodsw
        mul     word ptr ds:[bp+2]
        mov     ax,dx
        stosw
        loop    L5CAF
        jmp     L60E0
_do_gwtmap endp

; seg004_5CE5  (+5CE5)
; do_compact_map, do_compact_tmap, do_compact_wtmap: a textured quadrilateral in a compact form.
; Operands: the bitmap slot (not for do_compact_map, which uses the context), then four point
; numbers as bytes (each times 8 is the point's offset in resbuf). The texture coordinates are
; implied: the corners get u = width-1 or 0 and v = the v limit or 0 in a fixed order, so the
; whole texture covers the quad. GRIDDB.C's txtflr and txtwal emit these for floors and walls
; (opcodes 0xA0 and 0xA2).
_do_compact_map proc near
        mov     word ptr ds:[0B006h],0B00Ah
        jmp     short L5CFB
_do_compact_map endp

; seg004_5CED  (+5CED)
_do_compact_tmap proc near
        mov     word ptr ds:[0B006h],offset _seg003_1DA
        jmp     short L5CFB
_do_compact_tmap endp

; seg004_5CF5  (+5CF5)
_do_compact_wtmap proc near
        mov     word ptr ds:[0B006h],offset _seg003_545
L5CFB:
        lodsw
        shl     ax,1
        shl     ax,1
        shl     ax,1
        add     ax,0B07Eh
        mov     word ptr ds:[0B07Ch],ax
        mov     bp,ax
L5D0A:
        mov     byte ptr ds:[161Ah],0
        mov     byte ptr ds:[161Bh],0FFh
        mov     di,309h
        mov     word ptr ds:[307h],di
        mov     cx,4
L5D1E:
        xor     ah,ah
        lodsb
        shl     ax,1
        shl     ax,1
        shl     ax,1
        mov     bx,ax
        mov     ax,word ptr [bx+1620h]
        stosw
        mov     ax,word ptr [bx+1622h]
        stosw
        mov     ax,word ptr [bx+1624h]
        stosw
        mov     al,byte ptr [bx+1626h]
        stosw
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        xor     ax,ax
        mov     bl,cl
        dec     bl
        dec     bl
        test    bl,2
        jne     short L5D59
        mov     ax,word ptr ds:[bp]
        dec     ax
        xchg    ah,al
L5D59:
        stosw
        xor     ax,ax
        mov     bl,cl
        dec     bl
        test    bl,2
        jne     short L5D69
        mov     ax,word ptr ds:[bp+2]
L5D69:
        stosw
        loop    L5D1E
        jmp     L5E73
_do_compact_wtmap endp

; seg004_5D6F  (+5D6F)
; do_bcompact_map, do_bcompact_tmap, do_bcompact_wtmap: as the do_compact_ handlers with the u
; corners swapped (the `je` where do_compact has `jne`), so the texture is mirrored left to
; right; probably for faces seen from behind (b for back). do_bcompact_map sets floor-style
; mapping and jumps to L5D8A with AX unset: AX there is the BX the previous handler left (the
; dispatch's xchg), where FM Towns' do_bcompact_map loads bmap_blk_ptr first. So in DOS it
; would map with a stray block pointer; probably the C never emits it.
_do_bcompact_tmap proc near
        mov     word ptr ds:[0B006h],offset _seg003_1DA
        jmp     short L5D7D
_do_bcompact_tmap endp

; seg004_5D77  (+5D77)
_do_bcompact_wtmap proc near
        mov     word ptr ds:[0B006h],offset _seg003_545
L5D7D:
        lodsw
        shl     ax,1
        shl     ax,1
        shl     ax,1
        add     ax,0B07Eh
        mov     word ptr ds:[0B07Ch],ax
L5D8A:
        mov     bp,ax
        mov     byte ptr ds:[161Ah],0
        mov     byte ptr ds:[161Bh],0FFh
        mov     di,309h
        mov     word ptr ds:[307h],di
        mov     cx,4
L5DA0:
        xor     ah,ah
        lodsb
        shl     ax,1
        shl     ax,1
        shl     ax,1
        mov     bx,ax
        mov     ax,word ptr [bx+1620h]
        stosw
        mov     ax,word ptr [bx+1622h]
        stosw
        mov     ax,word ptr [bx+1624h]
        stosw
        mov     al,byte ptr [bx+1626h]
        stosw
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        xor     ax,ax
        mov     bl,cl
        dec     bl
        dec     bl
        test    bl,2
        je      short L5DDB
        mov     ax,word ptr ds:[bp]
        dec     ax
        xchg    ah,al
L5DDB:
        stosw
        xor     ax,ax
        mov     bl,cl
        dec     bl
        test    bl,2
        jne     short L5DEB
        mov     ax,word ptr ds:[bp+2]
L5DEB:
        stosw
        loop    L5DA0
        jmp     L60E0
_do_bcompact_wtmap endp

; seg004_5DF1  (+5DF1)
; do_tmap_tri, do_wtmap, do_tmap: a textured triangle (do_tmap_tri) or quadrilateral,
; floor-style (do_tmap, opcode 0x36) or wall-style (do_wtmap). Operands: the bitmap slot, then
; per vertex a point offset into resbuf and a corner word: bit 0 set gives u = width-1, a value
; of 2 or more gives v = the v limit, so each vertex sits at one corner of the texture.
_do_tmap_tri proc near
        mov     word ptr ds:[0B006h],offset _seg003_1DA
        mov     cx,3
        jmp     short L5E0D
_do_tmap_tri endp

; seg004_5DFC  (+5DFC)
_do_wtmap proc near
        mov     word ptr ds:[0B006h],offset _seg003_545
        jmp     short L5E0A
_do_wtmap endp

; seg004_5E04  (+5E04)
_do_tmap proc near
        mov     word ptr ds:[0B006h],offset _seg003_1DA
L5E0A:
        mov     cx,4
L5E0D:
        mov     word ptr ds:[0B00Ch],cx
        lodsw
        shl     ax,1
        shl     ax,1
        shl     ax,1
        add     ax,0B07Eh
        mov     word ptr ds:[0B07Ch],ax
        mov     bp,ax
        mov     byte ptr ds:[161Ah],0
        mov     byte ptr ds:[161Bh],0FFh
        mov     di,309h
        mov     word ptr ds:[307h],di
L5E31:
        lodsw
        mov     bx,ax
        mov     ax,word ptr [bx+1620h]
        stosw
        mov     ax,word ptr [bx+1622h]
        stosw
        mov     ax,word ptr [bx+1624h]
        stosw
        mov     al,byte ptr [bx+1626h]
        stosw
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        lodsw
        xor     dx,dx
        test    al,1
        je      short L5E60
        mov     dx,word ptr ds:[bp]
        dec     dx
        xchg    dh,dl
        mov     dl,0FFh
L5E60:
        mov     word ptr [di],dx
        inc     di
        inc     di
        xor     dx,dx
        cmp     al,2
        jl      short L5E6E
        mov     dx,word ptr ds:[bp+2]
L5E6E:
        mov     ax,dx
        stosw
        loop    L5E31
L5E73:
        cmp     word ptr ds:[0B00Ch],4
        je      short L5E7D
        jmp     L60E0
L5E7D:
        cmp     word ptr ds:[0B006h],offset _seg003_545
        je      short L5E88
        jmp     L60E0
L5E88:
        test    byte ptr ds:[161Bh],0FFh
        je      short L5E92
        jmp     L60EF
L5E92:
        test    word ptr ds:[2930h],0FFFFh
        je      short L5E9D
        jmp     L60E0
L5E9D:
        test    byte ptr ds:[161Ah],0FFh
        jne     short L5EC8
        mov     cl,byte ptr ds:[2880h]
        mov     ax,1
        shl     ax,cl
        cmp     ax,word ptr ds:[30Dh]
        jg      short L5EC8
        cmp     ax,word ptr ds:[319h]
        jg      short L5EC8
        cmp     ax,word ptr ds:[325h]
        jg      short L5EC8
        cmp     ax,word ptr ds:[331h]
        jg      short L5EC8
        jmp     L60E0
L5EC8:
        push    si
        mov     si,309h
        mov     cx,6
        rep movsw
        mov     di,0B00Eh
        mov     si,309h
        mov     dx,4
L5EDA:
        mov     cx,6
        rep movsw
        mov     bx,word ptr [si-0Ch]
        add     bx,word ptr [si]
        jno     short L5EE9
        rcr     bx,1
L5EE9 equ $+1
        test    ax,0FBD1h
        mov     cx,word ptr [si-0Ah]
        add     cx,word ptr [si+2]
        jno     short L5EF6
        rcr     cx,1
L5EF6 equ $+1
        test    ax,0F9D1h
        mov     bp,word ptr [si-8]
        add     bp,word ptr [si+4]
        jno     short L5F03
        rcr     bp,1
L5F03 equ $+1
        test    ax,0FDD1h
        mov     ax,bx
        stosw
        mov     ax,cx
        stosw
        mov     ax,bp
        stosw
        call    _code_pnt
        stosw
        mov     ax,word ptr [si-4]
        add     ax,word ptr [si+8]
        jno     short L5F1D
        rcr     ax,1
L5F1D equ $+1
        test    ax,0F8D1h
        stosw
        mov     ax,word ptr [si-2]
        add     ax,word ptr [si+0Ah]
        jno     short L5F2B
        rcr     ax,1
L5F2B equ $+1
        test    ax,0F8D1h
        stosw
        dec     dx
        jne     L5EDA
        mov     bx,word ptr ds:[0B01Ah]
        add     bx,word ptr ds:[0B04Ah]
        jno     short L5F3E
        rcr     bx,1
L5F3E equ $+1
        test    ax,0FBD1h
        mov     cx,word ptr ds:[0B01Ch]
        add     cx,word ptr ds:[0B04Ch]
        jno     short L5F4D
        rcr     cx,1
L5F4D equ $+1
        test    ax,0F9D1h
        mov     bp,word ptr ds:[0B01Eh]
        add     bp,word ptr ds:[0B04Eh]
        jno     short L5F5C
        rcr     bp,1
L5F5C equ $+1
        test    ax,0FDD1h
        mov     ax,bx
        stosw
        mov     ax,cx
        stosw
        mov     ax,bp
        stosw
        call    _code_pnt
        stosw
        mov     ax,word ptr ds:[0B022h]
        add     ax,word ptr ds:[0B052h]
        jno     short L5F77
        rcr     ax,1
L5F77 equ $+1
        test    ax,0F8D1h
        stosw
        mov     ax,word ptr ds:[0B024h]
        add     ax,word ptr ds:[0B054h]
        jno     short L5F86
        rcr     ax,1
L5F86 equ $+1
        test    ax,0F8D1h
        stosw
        pop     si
        push    si
        mov     di,309h
        mov     word ptr ds:[307h],di
        mov     bl,0
        mov     bh,0FFh
        mov     si,0B00Eh
        movsw
        movsw
        movsw
        lodsw
        or      bl,al
        and     bh,al
        stosw
        movsw
        movsw
        mov     si,0B01Ah
        movsw
        movsw
        movsw
        lodsw
        or      bl,al
        and     bh,al
        stosw
        movsw
        movsw
        mov     si,0B06Eh
        movsw
        movsw
        movsw
        lodsw
        or      bl,al
        and     bh,al
        stosw
        movsw
        movsw
        mov     si,0B062h
        movsw
        movsw
        movsw
        lodsw
        or      bl,al
        and     bh,al
        stosw
        movsw
        movsw
        mov     word ptr ds:[305h],di
        mov     byte ptr ds:[161Bh],bh
        mov     byte ptr ds:[161Ah],bl
        call    _seg004_6129
        pop     si
        push    si
        mov     di,309h
        mov     word ptr ds:[307h],di
        mov     bl,0
        mov     bh,0FFh
        mov     si,0B01Ah
        movsw
        movsw
        movsw
        lodsw
        or      bl,al
        and     bh,al
        stosw
        movsw
        movsw
        mov     si,0B026h
        movsw
        movsw
        movsw
        lodsw
        or      bl,al
        and     bh,al
        stosw
        movsw
        movsw
        mov     si,0B032h
        movsw
        movsw
        movsw
        lodsw
        or      bl,al
        and     bh,al
        stosw
        movsw
        movsw
        mov     si,0B06Eh
        movsw
        movsw
        movsw
        lodsw
        or      bl,al
        and     bh,al
        stosw
        movsw
        movsw
        mov     word ptr ds:[305h],di
        mov     byte ptr ds:[161Bh],bh
        mov     byte ptr ds:[161Ah],bl
        call    _seg004_6129
        pop     si
        push    si
        mov     di,309h
        mov     word ptr ds:[307h],di
        mov     bl,0
        mov     bh,0FFh
        mov     si,0B062h
        movsw
        movsw
        movsw
        lodsw
        or      bl,al
        and     bh,al
        stosw
        movsw
        movsw
        mov     si,0B06Eh
        movsw
        movsw
        movsw
        lodsw
        or      bl,al
        and     bh,al
        stosw
        movsw
        movsw
        mov     si,0B04Ah
        movsw
        movsw
        movsw
        lodsw
        or      bl,al
        and     bh,al
        stosw
        movsw
        movsw
        mov     si,0B056h
        movsw
        movsw
        movsw
        lodsw
        or      bl,al
        and     bh,al
        stosw
        movsw
        movsw
        mov     word ptr ds:[305h],di
        mov     byte ptr ds:[161Bh],bh
        mov     byte ptr ds:[161Ah],bl
        call    _seg004_6129
        pop     si
        push    si
        mov     di,309h
        mov     word ptr ds:[307h],di
        mov     bl,0
        mov     bh,0FFh
        mov     si,0B06Eh
        movsw
        movsw
        movsw
        lodsw
        or      bl,al
        and     bh,al
        stosw
        movsw
        movsw
        mov     si,0B032h
        movsw
        movsw
        movsw
        lodsw
        or      bl,al
        and     bh,al
        stosw
        movsw
        movsw
        mov     si,0B03Eh
        movsw
        movsw
        movsw
        lodsw
        or      bl,al
        and     bh,al
        stosw
        movsw
        movsw
        mov     si,0B04Ah
        movsw
        movsw
        movsw
        lodsw
        or      bl,al
        and     bh,al
        stosw
        movsw
        movsw
        mov     word ptr ds:[305h],di
        mov     byte ptr ds:[161Bh],bh
        mov     byte ptr ds:[161Ah],bl
        call    _seg004_6129
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L60E0:
        mov     word ptr ds:[305h],di
        test    word ptr ds:[2930h],0FFFFh
        jne     short _draw_solid_tmap
        call    _seg004_6129
L60EF:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_tmap endp

; seg004_60F5  (+60F5)
; draw_solid_tmap: draw the polygon just collected as a flat polygon instead (polym nonzero,
; texture mapping turned off). Compacts vbuf's 12-byte vertices to the 8-byte form the flat
; polygon code reads, dropping u and v, recomputes codes_or and codes_and, and jumps to
; INTERP.ASM's draw_poly_buf_ptr, which ends the opcode.
_draw_solid_tmap proc near
        push    si
        mov     si,word ptr ds:[307h]
        mov     di,309h
        mov     word ptr ds:[307h],di
        mov     byte ptr ds:[161Ah],0
        mov     byte ptr ds:[161Bh],0FFh
L610B:
        movsw
        movsw
        movsw
        lodsw
        stosw
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        add     si,4
        cmp     si,word ptr ds:[305h]
        jne     L610B
        mov     word ptr ds:[305h],di
        pop     si
        jmp     draw_poly_buf_ptr
_draw_solid_tmap endp

; seg004_6129  (+6129)
; UW1 only: seg004_6130's entry that first installs this module's divide overflow handler
; (L692C).
_seg004_6129 proc near
        mov     word ptr ss:[4D5h],offset L692C
_seg004_6129 endp

; seg004_6130  (+6130)
; UW1 only: draws the textured polygon collected at 307h..305h. When no vertex needs
; clipping (or byte 286Eh is set) it projects each vertex into the 14-byte records at
; seg048:659h, calls the mapper through the far pointer at 0B006h with the count at 0B07Ah,
; and then smooth_over (SMOOTH.ASM) when the low byte of 2928h is set. When every vertex is
; off one edge it draws nothing; otherwise it clips the polygon against the planes its codes
; name (seg004_624B for code 4, 63C6 for 8, 66C2 for 1, 6545 for 2) and tries again.
_seg004_6130 proc near
        test    byte ptr ds:[286Eh],0FFh
        jne     short L6148
        test    byte ptr ds:[161Bh],0FFh
        je      short L6141
        jmp     L61FB

; seg004_6141  (+6141)
; seg004_6141 (FM Towns has it unnamed, at draw_solid_tmap+53h): texture map the polygon in
; vbuf unless codes_and says it is wholly off one edge. CX = the vertex count ((vert_ptr -
; buf_ptr) / 12; a remainder breaks with int 2), BX = bmap_blk_ptr, then
; asm_texture_map_from_draw_poly (PROJPOLY.ASM). Keeps every register (pusha/popa).
L6141:
        test    byte ptr ds:[161Ah],0FFh
        jne     short L61AA
L6148:
        push    si
        push    es
        mov     ax,seg seg048
        mov     es,ax
        mov     di,659h
        mov     si,word ptr ds:[307h]
        xor     cx,cx
L6158:
        mov     bp,word ptr [si+4]
        mov     word ptr es:[di+0Ch],bp
        lodsw
        mov     word ptr es:[di+8],ax
        imul    word ptr ds:[26B2h]
        idiv    bp
        add     ax,word ptr ds:[26B4h]
        stosw
        lodsw
        mov     word ptr es:[di+8],ax
        imul    word ptr ds:[26B0h]
        idiv    bp
        add     ax,word ptr ds:[26B6h]
        stosw
        add     si,4
        movsw
        movsw
        add     di,6
        inc     cx
        cmp     si,word ptr ds:[305h]
        jne     L6158
        pop     es
        mov     word ptr ds:[0B07Ah],cx
        call    dword ptr ds:[0B006h]
        test    word ptr ds:[2928h],0FFh
        jne     short L61A1
L619F:
        pop     si
        ret
L61A1:
        mov     cx,word ptr ds:[0B07Ah]
        call    _smooth_over
        jmp     L619F
L61AA:
        push    si
        test    byte ptr ds:[161Ah],4
        je      short L61BC
        call    _seg004_624B
        test    byte ptr ds:[161Bh],0FFh
        jne     short L61FA
L61BC:
        test    byte ptr ds:[161Ah],8
        je      short L61CD
        call    _seg004_63C6
        test    byte ptr ds:[161Bh],0FFh
        jne     short L61FA
L61CD:
        test    byte ptr ds:[161Ah],1
        je      short L61DE
        call    _seg004_66C2
        test    byte ptr ds:[161Bh],0FFh
        jne     short L61FA
L61DE:
        test    byte ptr ds:[161Ah],2
        je      short L61E8
        call    _seg004_6545
L61E8:
        test    byte ptr ds:[161Bh],0FFh
        jne     short L61FA
        test    byte ptr ds:[161Ah],0FFh
        jne     short L61FC
        pop     si
        jmp     L6148
L61FA:
        pop     si
L61FB:
        ret
L61FC:
        test    byte ptr ds:[161Ah],4
        je      short L620D
        call    _seg004_624B
        test    byte ptr ds:[161Bh],0FFh
        jne     L61FA
L620D:
        test    byte ptr ds:[161Ah],8
        je      short L621E
        call    _seg004_63C6
        test    byte ptr ds:[161Bh],0FFh
        jne     L61FA
L621E:
        test    byte ptr ds:[161Ah],1
        je      short L622F
        call    _seg004_66C2
        test    byte ptr ds:[161Bh],0FFh
        jne     L61FA
L622F:
        test    byte ptr ds:[161Ah],2
        je      short L6239
        call    _seg004_6545
L6239:
        test    byte ptr ds:[161Bh],0FFh
        jne     L61FA
        test    byte ptr ds:[161Ah],0FFh
        jne     L61FA
        pop     si
        jmp     L6148
_seg004_6130 endp

; seg004_624B  (+624B)
; UW1 only: clips the textured polygon against the plane of clip code 4, interpolating u and
; v as well; the counterpart of INTERP.ASM's clip_top.
_seg004_624B proc near
        mov     byte ptr ds:[161Ah],0
        mov     byte ptr ds:[161Bh],0FFh
        mov     si,word ptr ds:[307h]
        mov     di,word ptr ds:[305h]
        mov     cx,0Ch
        rep movsw
        add     word ptr ds:[305h],0Ch
        mov     si,word ptr ds:[307h]
        mov     di,0C69h
        cmp     si,309h
        je      short L6277
        mov     di,309h
L6277:
        mov     word ptr ds:[307h],di
L627B:
        add     si,0Ch
L627E:
        cmp     si,word ptr ds:[305h]
        jne     short L6287
        jmp     L63C1
L6287:
        mov     al,byte ptr [si+6]
        test    al,4
        jne     short L629D
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        mov     cx,6
        rep movsw
        jmp     L627E
L629D:
        test    byte ptr [si-6],4
        je      short L62A6
        jmp     L632E
L62A6:
        mov     bp,word ptr [si-8]
        sub     bp,word ptr [si-0Ah]
        mov     cx,bp
        sub     cx,word ptr [si+4]
        add     cx,word ptr [si+2]
        mov     ax,word ptr [si]
        sub     ax,word ptr [si-0Ch]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L62C8
        cwd
        adc     ax,dx
L62C8:
        add     ax,word ptr [si-0Ch]
        stosw
        mov     bx,ax
        mov     ax,word ptr [si+2]
        sub     ax,word ptr [si-0Ah]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L62E3
        cwd
        adc     ax,dx
L62E3:
        add     ax,word ptr [si-0Ah]
        stosw
        stosw
        push    cx
        push    bp
        mov     cx,ax
        mov     bp,ax
        call    _code_pnt
        pop     bp
        pop     cx
        stosw
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        mov     ax,word ptr [si+8]
        sub     ax,word ptr [si-4]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L6311
        cwd
        adc     ax,dx
L6311:
        add     ax,word ptr [si-4]
        stosw
        mov     ax,word ptr [si+0Ah]
        sub     ax,word ptr [si-2]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L632A
        cwd
        adc     ax,dx
L632A:
        add     ax,word ptr [si-2]
        stosw
L632E:
        test    byte ptr [si+12h],4
        je      short L6337
        jmp     L627B
L6337:
        mov     bp,word ptr [si+4]
        sub     bp,word ptr [si+2]
        mov     cx,bp
        add     cx,word ptr [si+0Eh]
        sub     cx,word ptr [si+10h]
        mov     ax,word ptr [si+0Ch]
        sub     ax,word ptr [si]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L6359
        cwd
        adc     ax,dx
L6359:
        add     ax,word ptr [si]
        stosw
        mov     bx,ax
        mov     ax,word ptr [si+0Eh]
        sub     ax,word ptr [si+2]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L6373
        cwd
        adc     ax,dx
L6373:
        add     ax,word ptr [si+2]
        stosw
        stosw
        push    cx
        push    bp
        mov     cx,ax
        mov     bp,ax
        call    _code_pnt
        pop     bp
        pop     cx
        stosw
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        mov     ax,word ptr [si+14h]
        sub     ax,word ptr [si+8]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L63A1
        cwd
        adc     ax,dx
L63A1:
        add     ax,word ptr [si+8]
        stosw
        mov     ax,word ptr [si+16h]
        sub     ax,word ptr [si+0Ah]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L63BA
        cwd
        adc     ax,dx
L63BA:
        add     ax,word ptr [si+0Ah]
        stosw
        jmp     L627B
L63C1:
        mov     word ptr ds:[305h],di
        ret
_seg004_624B endp

; seg004_63C6  (+63C6)
; UW1 only: as seg004_624B for clip code 8 (INTERP.ASM's clip_bot).
_seg004_63C6 proc near
        mov     byte ptr ds:[161Ah],0
        mov     byte ptr ds:[161Bh],0FFh
        mov     si,word ptr ds:[307h]
        mov     di,word ptr ds:[305h]
        mov     cx,0Ch
        rep movsw
        add     word ptr ds:[305h],0Ch
        mov     si,word ptr ds:[307h]
        mov     di,0C69h
        cmp     si,309h
        je      short L63F2
        mov     di,309h
L63F2:
        mov     word ptr ds:[307h],di
L63F6:
        add     si,0Ch
L63F9:
        cmp     si,word ptr ds:[305h]
        jne     short L6402
        jmp     L6540
L6402:
        mov     al,byte ptr [si+6]
        test    al,8
        jne     short L6418
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        mov     cx,6
        rep movsw
        jmp     L63F9
L6418:
        test    byte ptr [si-6],8
        je      short L6421
        jmp     L64AB
L6421:
        mov     bp,word ptr [si-8]
        add     bp,word ptr [si-0Ah]
        mov     cx,bp
        sub     cx,word ptr [si+4]
        sub     cx,word ptr [si+2]
        mov     ax,word ptr [si]
        sub     ax,word ptr [si-0Ch]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L6443
        cwd
        adc     ax,dx
L6443:
        add     ax,word ptr [si-0Ch]
        stosw
        mov     bx,ax
        mov     ax,word ptr [si+2]
        sub     ax,word ptr [si-0Ah]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L645E
        cwd
        adc     ax,dx
L645E:
        add     ax,word ptr [si-0Ah]
        stosw
        push    bp
        push    cx
        mov     cx,ax
        neg     ax
        stosw
        mov     bp,ax
        call    _code_pnt
        stosw
        pop     cx
        pop     bp
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        mov     ax,word ptr [si+8]
        sub     ax,word ptr [si-4]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L648E
        cwd
        adc     ax,dx
L648E:
        add     ax,word ptr [si-4]
        stosw
        mov     ax,word ptr [si+0Ah]
        sub     ax,word ptr [si-2]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L64A7
        cwd
        adc     ax,dx
L64A7:
        add     ax,word ptr [si-2]
        stosw
L64AB:
        test    byte ptr [si+12h],8
        je      short L64B4
        jmp     L63F6
L64B4:
        mov     bp,word ptr [si+4]
        add     bp,word ptr [si+2]
        mov     cx,bp
        sub     cx,word ptr [si+0Eh]
        sub     cx,word ptr [si+10h]
        mov     ax,word ptr [si+0Ch]
        sub     ax,word ptr [si]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L64D6
        cwd
        adc     ax,dx
L64D6:
        add     ax,word ptr [si]
        stosw
        mov     bx,ax
        mov     ax,word ptr [si+0Eh]
        sub     ax,word ptr [si+2]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L64F0
        cwd
        adc     ax,dx
L64F0:
        add     ax,word ptr [si+2]
        stosw
        push    bp
        push    cx
        mov     cx,ax
        neg     ax
        stosw
        mov     bp,ax
        call    _code_pnt
        pop     cx
        pop     bp
        stosw
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        mov     ax,word ptr [si+14h]
        sub     ax,word ptr [si+8]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L6520
        cwd
        adc     ax,dx
L6520:
        add     ax,word ptr [si+8]
        stosw
        mov     ax,word ptr [si+16h]
        sub     ax,word ptr [si+0Ah]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L6539
        cwd
        adc     ax,dx
L6539:
        add     ax,word ptr [si+0Ah]
        stosw
        jmp     L63F6
L6540:
        mov     word ptr ds:[305h],di
        ret
_seg004_63C6 endp

; seg004_6545  (+6545)
; UW1 only: as seg004_624B for clip code 2 (INTERP.ASM's clip_right).
_seg004_6545 proc near
        mov     byte ptr ds:[161Ah],0
        mov     byte ptr ds:[161Bh],0FFh
        mov     si,word ptr ds:[307h]
        mov     di,word ptr ds:[305h]
        mov     cx,0Ch
        rep movsw
        add     word ptr ds:[305h],0Ch
        mov     si,word ptr ds:[307h]
        mov     di,0C69h
        cmp     si,309h
        je      short L6571
        mov     di,309h
L6571:
        mov     word ptr ds:[307h],di
L6575:
        add     si,0Ch
L6578:
        cmp     si,word ptr ds:[305h]
        jne     short L6581
        jmp     L66BD
L6581:
        mov     al,byte ptr [si+6]
        test    al,2
        jne     short L6597
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        mov     cx,6
        rep movsw
        jmp     L6578
L6597:
        test    byte ptr [si-6],2
        je      short L65A0
        jmp     L6629
L65A0:
        mov     bp,word ptr [si-0Ch]
        sub     bp,word ptr [si-8]
        mov     cx,bp
        add     cx,word ptr [si+4]
        sub     cx,word ptr [si]
        mov     ax,word ptr [si]
        sub     ax,word ptr [si-0Ch]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L65C1
        cwd
        adc     ax,dx
L65C1:
        add     ax,word ptr [si-0Ch]
        stosw
        mov     bx,ax
        mov     ax,word ptr [si+2]
        sub     ax,word ptr [si-0Ah]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L65DC
        cwd
        adc     ax,dx
L65DC:
        add     ax,word ptr [si-0Ah]
        stosw
        push    cx
        push    bp
        mov     cx,ax
        mov     ax,bx
        stosw
        mov     bp,ax
        call    _code_pnt
        pop     bp
        pop     cx
        stosw
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        mov     ax,word ptr [si+8]
        sub     ax,word ptr [si-4]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L660C
        cwd
        adc     ax,dx
L660C:
        add     ax,word ptr [si-4]
        stosw
        mov     ax,word ptr [si+0Ah]
        sub     ax,word ptr [si-2]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L6625
        cwd
        adc     ax,dx
L6625:
        add     ax,word ptr [si-2]
        stosw
L6629:
        test    byte ptr [si+12h],2
        je      short L6632
        jmp     L6575
L6632:
        mov     bp,word ptr [si]
        sub     bp,word ptr [si+4]
        mov     cx,bp
        add     cx,word ptr [si+10h]
        sub     cx,word ptr [si+0Ch]
        mov     ax,word ptr [si+0Ch]
        sub     ax,word ptr [si]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L6653
        cwd
        adc     ax,dx
L6653:
        add     ax,word ptr [si]
        stosw
        mov     bx,ax
        mov     ax,word ptr [si+0Eh]
        sub     ax,word ptr [si+2]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L666D
        cwd
        adc     ax,dx
L666D:
        add     ax,word ptr [si+2]
        stosw
        push    cx
        push    bp
        mov     cx,ax
        mov     ax,bx
        stosw
        mov     bp,ax
        call    _code_pnt
        pop     bp
        pop     cx
        stosw
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        mov     ax,word ptr [si+14h]
        sub     ax,word ptr [si+8]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L669D
        cwd
        adc     ax,dx
L669D:
        add     ax,word ptr [si+8]
        stosw
        mov     ax,word ptr [si+16h]
        sub     ax,word ptr [si+0Ah]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L66B6
        cwd
        adc     ax,dx
L66B6:
        add     ax,word ptr [si+0Ah]
        stosw
        jmp     L6575
L66BD:
        mov     word ptr ds:[305h],di
        ret
_seg004_6545 endp

; seg004_66C2  (+66C2)
; UW1 only: as seg004_624B for clip code 1 (INTERP.ASM's clip_left).
_seg004_66C2 proc near
        mov     byte ptr ds:[161Ah],0
        mov     byte ptr ds:[161Bh],0FFh
        mov     si,word ptr ds:[307h]
        mov     di,word ptr ds:[305h]
        mov     cx,0Ch
        rep movsw
        add     word ptr ds:[305h],0Ch
        mov     si,word ptr ds:[307h]
        mov     di,0C69h
        cmp     si,309h
        je      short L66EE
        mov     di,309h
L66EE:
        mov     word ptr ds:[307h],di
L66F2:
        add     si,0Ch
L66F5:
        cmp     si,word ptr ds:[305h]
        jne     short L66FE
        jmp     L683E
L66FE:
        mov     al,byte ptr [si+6]
        test    al,1
        jne     short L6714
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        mov     cx,6
        rep movsw
        jmp     L66F5
L6714:
        test    byte ptr [si-6],1
        je      short L671D
        jmp     L67A8
L671D:
        mov     bp,word ptr [si-8]
        add     bp,word ptr [si-0Ch]
        mov     cx,bp
        sub     cx,word ptr [si+4]
        sub     cx,word ptr [si]
        mov     ax,word ptr [si]
        sub     ax,word ptr [si-0Ch]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L673E
        cwd
        adc     ax,dx
L673E:
        add     ax,word ptr [si-0Ch]
        stosw
        mov     bx,ax
        mov     ax,word ptr [si+2]
        sub     ax,word ptr [si-0Ah]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L6759
        cwd
        adc     ax,dx
L6759:
        add     ax,word ptr [si-0Ah]
        stosw
        push    cx
        push    bp
        mov     cx,ax
        mov     ax,bx
        neg     ax
        stosw
        mov     bp,ax
        call    _code_pnt
        pop     bp
        pop     cx
        stosw
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        mov     ax,word ptr [si+8]
        sub     ax,word ptr [si-4]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L678B
        cwd
        adc     ax,dx
L678B:
        add     ax,word ptr [si-4]
        stosw
        mov     ax,word ptr [si+0Ah]
        sub     ax,word ptr [si-2]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L67A4
        cwd
        adc     ax,dx
L67A4:
        add     ax,word ptr [si-2]
        stosw
L67A8:
        test    byte ptr [si+12h],1
        je      short L67B1
        jmp     L66F2
L67B1:
        mov     bp,word ptr [si+4]
        add     bp,word ptr [si]
        mov     cx,bp
        sub     cx,word ptr [si+0Ch]
        sub     cx,word ptr [si+10h]
        mov     ax,word ptr [si+0Ch]
        sub     ax,word ptr [si]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L67D2
        cwd
        adc     ax,dx
L67D2:
        add     ax,word ptr [si]
        stosw
        mov     bx,ax
        mov     ax,word ptr [si+0Eh]
        sub     ax,word ptr [si+2]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L67EC
        cwd
        adc     ax,dx
L67EC:
        add     ax,word ptr [si+2]
        stosw
        push    cx
        push    bp
        mov     cx,ax
        mov     ax,bx
        neg     ax
        stosw
        mov     bp,ax
        call    _code_pnt
        pop     bp
        pop     cx
        stosw
        or      byte ptr ds:[161Ah],al
        and     byte ptr ds:[161Bh],al
        mov     ax,word ptr [si+14h]
        sub     ax,word ptr [si+8]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L681E
        cwd
        adc     ax,dx
L681E:
        add     ax,word ptr [si+8]
        stosw
        mov     ax,word ptr [si+16h]
        sub     ax,word ptr [si+0Ah]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L6837
        cwd
        adc     ax,dx
L6837:
        add     ax,word ptr [si+0Ah]
        stosw
        jmp     L66F2
L683E:
        mov     word ptr ds:[305h],di
L6842:
        ret
_seg004_66C2 endp

; seg004_6843  (+6843)
; do_scalebm: draw a bitmap scaled to its distance, standing upright at a point (a sprite).
; Operands: the point's offset in resbuf, the bitmap record (an offset from B0B6: do_uwobj
; passes 1Ch, the record at B0D2, and do_uwcrit 0Eh, the record at B0C4, which they fill), and the next opcode. in_scalebm is a second entry with DI =
; the point and AX = the slot, used by PGCACHE.ASM's do_uwobj and do_uwcrit.
;
; It offsets the point by the bitmap's hot spot (the record's words at +0Ah and +0Ch times bytes
; +6 and +8, shifted by 3 - the zoom at DS:2880, times the view scales at 26D0 and 160A),
; projects that corner and the opposite one (_code_pnt, INSTANCE.ASM, gives the clip codes; a
; point behind the eye or off the wrong edge abandons the sprite), stores the screen corner and
; size in seg048:0B1A..0B27 and calls seg003's _seg003_1082 (SCALEBM.ASM) to draw it. A
; divide overflow while projecting is caught by _overflow_handler_reg (INTERP.ASM), installed
; through SS:4D5, and also abandons the sprite.
_do_scalebm proc far
        lodsw
        mov     di,ax
        add     di,1620h
        lodsw
in_scalebm:
        mov     word ptr ss:[4D5h],offset _overflow_handler_reg
        mov     word ptr cs:overflow_count,0
        add     ax,0B0B6h
        push    si
        mov     si,ax
        mov     cl,byte ptr ds:[2880h]
        mov     ax,word ptr [si+0Ah]
        imul    byte ptr [si+6]
        shl     ax,cl
        imul    word ptr ds:[26D0h]
        sub     word ptr [di],dx
        mov     ax,word ptr [si+0Ch]
        imul    byte ptr [si+8]
        shl     ax,cl
        imul    word ptr ds:[160Ah]
        add     word ptr [di+2],dx
        mov     ax,word ptr [si+2]
        imul    byte ptr [si+0Ah]
        shl     ax,cl
        imul    word ptr ds:[26D0h]
        mov     word ptr ds:[15FCh],dx
        mov     ax,word ptr [si+4]
        imul    byte ptr [si+0Ch]
        shl     ax,cl
        imul    word ptr ds:[160Ah]
        mov     word ptr ds:[15FEh],dx
        push    es
        mov     ax,seg seg048
        mov     es,ax
        mov     bx,word ptr [di]
        mov     cx,word ptr [di+2]
        mov     bp,word ptr [di+4]
        call    _code_pnt
        test    al,82h
        jne     short L6922
        mov     ax,bx
        imul    word ptr ds:[26B2h]
        idiv    bp
        add     ax,word ptr ds:[26B4h]
        mov     word ptr es:[0B22h],ax
        mov     ax,cx
        imul    word ptr ds:[26B0h]
        idiv    bp
        add     ax,word ptr ds:[26B6h]
        mov     word ptr es:[0B24h],ax
        add     bx,word ptr ds:[15FCh]
        add     cx,word ptr ds:[15FEh]
        call    _code_pnt
        test    al,89h
        jne     short L6922
        mov     ax,bx
        imul    word ptr ds:[26B2h]
        idiv    bp
        add     ax,word ptr ds:[26B4h]
        sub     ax,word ptr es:[0B22h]
        mov     word ptr es:[0B28h],ax
        mov     ax,cx
        imul    word ptr ds:[26B0h]
        idiv    bp
        add     ax,word ptr ds:[26B6h]
        sub     ax,word ptr es:[0B24h]
        mov     word ptr es:[0B26h],ax
        mov     di,0B1Ch
        movsw
        movsw
        movsw
        test    word ptr cs:overflow_count,0FFFFh
        jne     short L6922
        call    far ptr _seg003_1082
L6922:
        pop     es
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L692A   dw      0                       ; where the overflow handler below saves BP
L692C:
        mov     word ptr cs:L692A,bp
        mov     bp,sp
        inc     word ptr [bp]
        inc     word ptr [bp]
        mov     ax,7FFFh
        xor     dx,dx
        mov     bp,word ptr cs:L692A
        iret
_do_scalebm endp

SEG004_TEXT ends

        end
