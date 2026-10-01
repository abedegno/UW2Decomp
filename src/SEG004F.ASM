; /* target: seg004_0849_3CF0 */
; /* opts: /ml */
;
; The texture-mapping opcode handlers: do_set_tmctxt .. do_tmap, draw_solid_tmap and
; do_scalebm (FM Towns names).
;
; SEG004 split into its original modules: this is module 6 of 14, +3CF0..+40B0 of the segment
; (file CEB0h), a module of the second library (UWLIB.LIB).
; Procs: do_set_tmctxt, do_set_gmap_ctxt, do_ctxt_gtmap, do_ctxt_gmap, do_gtri, do_gtmap,
; do_gwtmap, do_compact_map, do_compact_tmap, do_compact_wtmap, do_bcompact_map,
; do_bcompact_tmap, do_bcompact_wtmap, do_tmap_tri, do_wtmap, do_tmap, draw_solid_tmap,
; seg004_0849_3F77, do_scalebm.
; Evidence for the bounds: starts on a paragraph after 4 zero bytes following a iret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; Its relocations are entries 2426-2427 of UW2.EXE's relocation table.
; Its FD58 piece is 4FAF:C788..E4D6 (relocation entries 2414-2425), defined before the code
; because those entries come before the code's.
;
; The FD58 piece (segment table entry 58, the 3D data segment seg052_519C) holds the segment's
; twelve relocations, entries 2414-2425, which come right before this module's code entries.
; Where the piece starts is not known: it is taken from the first of them, the far pointer at
; C788 (FM Towns mapper_rtn, pointing at seg003's debug break), and it runs to the end of the
; segment; the bytes before it come from the EXE (tools/extract.py).

        .386

        extrn   _checkerboard1:far
        extrn   _seg003_0272_60:far
        extrn   _seg003_0272_D1E:far
        extrn   _seg_5DFD:far
        extrn   seg_370D:far

_DATA   segment word public use16 'DATA'
_DATA   ends
DGROUP  group   _DATA

FD58    segment byte public use16 'FAR_DATA'
        dd      _seg003_0272_60
        db      60h
        db      73h dup (0)
        db      0Fh,02h,10h,00h
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
        db      10h,00h,10h,00h,08h,00h,10h,00h,20h,00h
        dd      _checkerboard1+28h
        db      10h,00h,10h,00h,08h,00h,10h,00h,18h,00h
        dd      _seg_5DFD+18h
        db      10h,00h,10h,00h,08h,00h,10h,00h,20h,00h
        dd      _seg_5DFD+28h
        db      10h,00h,10h,00h,08h,00h,10h,00h,20h,00h,28h,00h,00h,00h,00h,00h
        db      472h dup (0)
        db      3Fh,00h,0C0h,0Fh,06h,00h,00h,00h,00h,00h,40h,00h,40h,00h,00h,00h
        db      204h dup (0)
        db      40h,01h
        db      32h dup (0)
        db      01h,05h
        db      0FEh dup (0)
_first_anim label byte
        db      00h,00h,00h,00h,00h,00h,01h,00h,01h,00h,63h,72h,69h,74h,5Ch,63h
        db      72h,3Fh,3Fh,2Eh,3Fh,3Fh,00h,0FFh
        db      1001h dup (0)
_grs_3dinf label byte
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
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
_CmapFrm label byte
        db      0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh
        db      0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh
        db      0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh
        db      0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh
        db      0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh
        db      0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh
        db      0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh
        db      0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh
        db      0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh
        db      0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh
        db      0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh
        db      0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh
        db      0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh
        db      0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh
        db      0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh
        db      0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh,0FDh
_CmapCache label byte
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
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
_tmap_fpage label byte
        db      00h
_scrgr_fpage label byte
        db      00h
_obj_inpage1 label byte
        db      00h
_EmsBuff label byte
        db      00h,00h
_seg052_519C_E4D4 label byte
        db      00h,00h
FD58    ends

SEG004_TEXT segment para public use16 'CODE'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _asm_texture_map_from_draw_poly:near
        extrn   _code_pnt:near
        extrn   _overflow_handler_reg:far
        extrn   draw_poly_buf_ptr:near
        extrn   overflow_count:word
        public  _CmapCache
        public  _CmapFrm
        public  _CmaptoPg
        public  _crit_fpage
        public  _crit_inpage
        public  _crit_nlpages
        public  _EmsBuff
        public  _first_anim
        public  _grs_3dinf
        public  _obj_inpage1
        public  _PgtoCmap
        public  _scrgr_fpage
        public  _seg052_519C_E4D4
        public  _tmap_fpage
        public  _tmap_inpage
        public  _do_bcompact_map
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
        public  _seg004_0849_3F77
        public  in_scalebm

; seg004_0849_3CF0  (+3CF0)
_do_set_tmctxt proc near
        lodsw
        xor     ah,ah
        shl     ax,1
        shl     ax,1
        shl     ax,1
        add     ax,0C802h
        mov     word ptr ds:[0C7FEh],ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_set_tmctxt endp

; seg004_0849_3D05  (+3D05)
_do_set_gmap_ctxt proc near
        lodsw
        test    al,0FFh
        jne     short L3D12
        mov     word ptr ds:[0C78Ch],60h
        jmp     short L3D18
L3D12:
        mov     word ptr ds:[0C78Ch],20Fh
L3D18:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_set_gmap_ctxt endp

; seg004_0849_3D1E  (+3D1E)
_do_ctxt_gtmap proc near
        mov     word ptr ds:[0C788h],60h
        mov     bp,word ptr ds:[0C7FEh]
        jmp     short L3D58
_do_ctxt_gtmap endp

; seg004_0849_3D2A  (+3D2A)
_do_ctxt_gmap proc near
        mov     bp,word ptr ds:[0C78Ch]
        mov     word ptr ds:[0C788h],bp
        mov     bp,word ptr ds:[0C7FEh]
        jmp     short L3D58
_do_ctxt_gmap endp

; seg004_0849_3D38  (+3D38)
_do_gtri proc near
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_gtri endp

; seg004_0849_3D3E  (+3D3E)
_do_gtmap proc near
        mov     word ptr ds:[0C788h],60h
        jmp     short L3D4C
_do_gtmap endp

; seg004_0849_3D46  (+3D46)
_do_gwtmap proc near
        mov     word ptr ds:[0C788h],20Fh
L3D4C:
        lodsw
        shl     ax,3
        add     ax,0C802h
        mov     word ptr ds:[0C7FEh],ax
        mov     bp,ax
L3D58:
        mov     byte ptr ds:[14CAh],0
        mov     byte ptr ds:[14CBh],0FFh
        mov     di,1BAh
        mov     word ptr ds:[1B8h],di
        lodsw
        mov     cx,ax
L3D6C:
        lodsw
        mov     bx,ax
        mov     ax,word ptr [bx+14D0h]
        stosw
        mov     ax,word ptr [bx+14D2h]
        stosw
        mov     ax,word ptr [bx+14D4h]
        stosw
        mov     al,byte ptr [bx+14D6h]
        stosw
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        lodsw
        mul     word ptr ds:[bp]
        mov     al,ah
        mov     ah,dl
        stosw
        lodsw
        mul     word ptr ds:[bp+2]
        mov     ax,dx
        stosw
        loop    L3D6C
        jmp     L3F2E
_do_gwtmap endp

; seg004_0849_3DA2  (+3DA2)
_do_compact_map proc near
        mov     bp,word ptr ds:[0C78Ch]
        mov     word ptr ds:[0C788h],bp
        mov     bp,word ptr ds:[0C7FEh]
        jmp     short L3DCA
_do_compact_map endp

; seg004_0849_3DB0  (+3DB0)
_do_compact_tmap proc near
        mov     word ptr ds:[0C788h],60h
        jmp     short L3DBE
_do_compact_tmap endp

; seg004_0849_3DB8  (+3DB8)
_do_compact_wtmap proc near
        mov     word ptr ds:[0C788h],20Fh
L3DBE:
        lodsw
        shl     ax,3
        add     ax,0C802h
        mov     word ptr ds:[0C7FEh],ax
        mov     bp,ax
L3DCA:
        mov     byte ptr ds:[14CAh],0
        mov     byte ptr ds:[14CBh],0FFh
        mov     di,1BAh
        mov     word ptr ds:[1B8h],di
        mov     cx,4
L3DDE:
        xor     ah,ah
        lodsb
        shl     ax,3
        mov     bx,ax
        mov     ax,word ptr [bx+14D0h]
        stosw
        mov     ax,word ptr [bx+14D2h]
        stosw
        mov     ax,word ptr [bx+14D4h]
        stosw
        mov     al,byte ptr [bx+14D6h]
        stosw
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        xor     ax,ax
        mov     bl,cl
        dec     bl
        dec     bl
        test    bl,2
        jne     short L3E16
        mov     ax,word ptr ds:[bp]
        dec     ax
        xchg    ah,al
L3E16:
        stosw
        xor     ax,ax
        mov     bl,cl
        dec     bl
        test    bl,2
        jne     short L3E26
        mov     ax,word ptr ds:[bp+2]
L3E26:
        stosw
        loop    L3DDE
        jmp     L3F2E
_do_compact_wtmap endp

; seg004_0849_3E2C  (+3E2C)
_do_bcompact_map proc near
        mov     word ptr ds:[0C788h],60h
        jmp     short L3E4C
_do_bcompact_map endp

; seg004_0849_3E34  (+3E34)
_do_bcompact_tmap proc near
        mov     word ptr ds:[0C788h],60h
        jmp     short L3E42
_do_bcompact_tmap endp

; seg004_0849_3E3C  (+3E3C)
_do_bcompact_wtmap proc near
        mov     word ptr ds:[0C788h],20Fh
L3E42:
        lodsw
        shl     ax,3
        add     ax,0C802h
        mov     word ptr ds:[0C7FEh],ax
L3E4C:
        mov     bp,ax
        mov     byte ptr ds:[14CAh],0
        mov     byte ptr ds:[14CBh],0FFh
        mov     di,1BAh
        mov     word ptr ds:[1B8h],di
        mov     cx,4
L3E62:
        xor     ah,ah
        lodsb
        shl     ax,3
        mov     bx,ax
        mov     ax,word ptr [bx+14D0h]
        stosw
        mov     ax,word ptr [bx+14D2h]
        stosw
        mov     ax,word ptr [bx+14D4h]
        stosw
        mov     al,byte ptr [bx+14D6h]
        stosw
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        xor     ax,ax
        mov     bl,cl
        dec     bl
        dec     bl
        test    bl,2
        je      short L3E9A
        mov     ax,word ptr ds:[bp]
        dec     ax
        xchg    ah,al
L3E9A:
        stosw
        xor     ax,ax
        mov     bl,cl
        dec     bl
        test    bl,2
        jne     short L3EAA
        mov     ax,word ptr ds:[bp+2]
L3EAA:
        stosw
        loop    L3E62
        jmp     short L3F2E
_do_bcompact_wtmap endp

; seg004_0849_3EAF  (+3EAF)
_do_tmap_tri proc near
        mov     word ptr ds:[0C788h],60h
        mov     cx,3
        jmp     short L3ECB
_do_tmap_tri endp

; seg004_0849_3EBA  (+3EBA)
_do_wtmap proc near
        mov     word ptr ds:[0C788h],20Fh
        jmp     short L3EC8
_do_wtmap endp

; seg004_0849_3EC2  (+3EC2)
_do_tmap proc near
        mov     word ptr ds:[0C788h],60h
L3EC8:
        mov     cx,4
L3ECB:
        mov     word ptr ds:[0C78Eh],cx
        lodsw
        shl     ax,3
        add     ax,0C802h
        mov     word ptr ds:[0C7FEh],ax
        mov     bp,ax
        mov     byte ptr ds:[14CAh],0
        mov     byte ptr ds:[14CBh],0FFh
        mov     di,1BAh
        mov     word ptr ds:[1B8h],di
L3EEC:
        lodsw
        mov     bx,ax
        mov     ax,word ptr [bx+14D0h]
        stosw
        mov     ax,word ptr [bx+14D2h]
        stosw
        mov     ax,word ptr [bx+14D4h]
        stosw
        mov     al,byte ptr [bx+14D6h]
        stosw
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        lodsw
        xor     dx,dx
        test    al,1
        je      short L3F1B
        mov     dx,word ptr ds:[bp]
        dec     dx
        xchg    dh,dl
        mov     dl,0FFh
L3F1B:
        mov     word ptr [di],dx
        inc     di
        inc     di
        xor     dx,dx
        cmp     al,2
        jl      short L3F29
        mov     dx,word ptr ds:[bp+2]
L3F29:
        mov     ax,dx
        stosw
        loop    L3EEC
L3F2E:
        mov     word ptr ds:[1B6h],di
        test    word ptr ds:[2690h],0FFFFh
        jne     short _draw_solid_tmap
        call    _seg004_0849_3F77
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_tmap endp

; seg004_0849_3F43  (+3F43)
_draw_solid_tmap proc near
        push    si
        mov     si,word ptr ds:[1B8h]
        mov     di,1BAh
        mov     word ptr ds:[1B8h],di
        mov     byte ptr ds:[14CAh],0
        mov     byte ptr ds:[14CBh],0FFh
L3F59:
        movsw
        movsw
        movsw
        lodsw
        stosw
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        add     si,4
        cmp     si,word ptr ds:[1B6h]
        jne     L3F59
        mov     word ptr ds:[1B6h],di
        pop     si
        jmp     draw_poly_buf_ptr
_draw_solid_tmap endp

; seg004_0849_3F77  (+3F77)
_seg004_0849_3F77 proc near
        test    byte ptr ds:[14CBh],0FFh
        jne     short L3F9E
        pusha
        mov     si,word ptr ds:[1B8h]
        mov     ax,word ptr ds:[1B6h]
        sub     ax,si
        mov     cx,0Ch
        div     cl
        mov     cl,al
        or      ah,ah
        je      short L3F96
        int     2
        nop
L3F96:
        mov     bx,word ptr ds:[0C7FEh]
        call    _asm_texture_map_from_draw_poly
        popa
L3F9E:
        ret
_seg004_0849_3F77 endp

; seg004_0849_3F9F  (+3F9F)
_do_scalebm proc near
        lodsw
        mov     di,ax
        add     di,14D0h
        lodsw
in_scalebm:
        mov     word ptr ss:[5A5h],offset _overflow_handler_reg
        mov     word ptr cs:overflow_count,0
        add     ax,0C83Ah
        push    si
        mov     si,ax
        mov     cl,3
        sub     cl,byte ptr ds:[25E0h]
        mov     ax,word ptr [si+0Ah]
        imul    byte ptr [si+6]
        shr     ax,cl
        imul    word ptr ds:[2490h]
        sub     word ptr [di],dx
        mov     ax,word ptr [si+0Ch]
        imul    byte ptr [si+8]
        shr     ax,cl
        imul    word ptr ds:[14BAh]
        add     word ptr [di+2],dx
        mov     ax,word ptr [si+2]
        imul    byte ptr [si+0Ah]
        shr     ax,cl
        imul    word ptr ds:[2490h]
        mov     word ptr ds:[14ACh],dx
        mov     ax,word ptr [si+4]
        imul    byte ptr [si+0Ch]
        shr     ax,cl
        imul    word ptr ds:[14BAh]
        mov     word ptr ds:[14AEh],dx
        push    es
        mov     ax,seg seg_370D
        mov     es,ax
        mov     bx,word ptr [di]
        mov     cx,word ptr [di+2]
        mov     bp,word ptr [di+4]
        call    _code_pnt
        test    al,82h
        jne     short L4080
        mov     ax,bx
        imul    word ptr ds:[2472h]
        idiv    bp
        add     ax,word ptr ds:[2474h]
        mov     word ptr es:[0B20h],ax
        mov     ax,cx
        imul    word ptr ds:[2470h]
        idiv    bp
        add     ax,word ptr ds:[2476h]
        mov     word ptr es:[0B22h],ax
        add     bx,word ptr ds:[14ACh]
        add     cx,word ptr ds:[14AEh]
        call    _code_pnt
        test    al,89h
        jne     short L4080
        mov     ax,bx
        imul    word ptr ds:[2472h]
        idiv    bp
        add     ax,word ptr ds:[2474h]
        sub     ax,word ptr es:[0B20h]
        mov     word ptr es:[0B26h],ax
        mov     ax,cx
        imul    word ptr ds:[2470h]
        idiv    bp
        add     ax,word ptr ds:[2476h]
        sub     ax,word ptr es:[0B22h]
        mov     word ptr es:[0B24h],ax
        mov     di,0B1Ah
        movsw
        movsw
        movsw
        test    word ptr cs:overflow_count,0FFFFh
        jne     short L4080
        call    far ptr _seg003_0272_D1E
L4080:
        pop     es
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
; An overflow handler like overflow_handler_mem that nothing installs.
L4088   dw      0
L408A:
        mov     word ptr cs:L4088,bp
        mov     bp,sp
        inc     word ptr [bp]
        inc     word ptr [bp]
        mov     ax,7FFFh
        xor     dx,dx
        mov     bp,word ptr cs:L4088
        iret
_do_scalebm endp

SEG004_TEXT ends

        end
