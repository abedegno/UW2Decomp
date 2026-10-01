; /* target: seg004_0849 */
; /* opts: /ml */
;
; The 3D renderer: the model interpreter (an opcode table at 24F4h in the renderer's data
; segment, seg052_519C, dispatches every do_ handler with lodsw; xchg bx,ax; jmp [bx+24F4h]),
; the matrix and point transforms, polygon clipping and the overflow handlers that guard its
; divides, the 386 texture mapper, the image decoders (uncmp_tab) and the critter page cache.
; 0x8154 bytes at file 0x91C0, org 0. The module opens with data, the 32-byte uncmp_pal.
;
; Hand-written assembly, .386 in a use16 segment (32-bit registers throughout the texture
; mapper; the nop padding of forward jumps is TASM 2.0's single pass). DS is seg052_519C,
; not DGROUP, so near data stays numeric. It patches its own code a lot: the stores into
; cs: name the instruction they patch (label+1 for an immediate).
;
; Names. FM Towns names where the code corresponds:
; - every opcode handler, by its slot: the DOS table at seg052:24F4 and FM vector_table
;   list the same opcodes in the same order (do_obj, do_rod, do_x_rel, ...); the flat_
;   variants by the slots check_flat/is_flat store them in, as FM's is_flat does.
; - uncmp_tab and the exp_ decoders by table slot (FM uncmp_tab), _uncmp_pal and lightabs by
;   the code that fills and reads them (FM exp_4str+32h).
; - render_3d's callees in call order (create_matrix, scale_matrix, check_flat), and the run
;   that follows in both builds' source order: set_accept, self_modify, mini_ and
;   load_xlate_rotate_pnt, mxmul, the instance_ routines, code_pnt, mm9x9, mm3x9_bpsi, mm3x9t
;   (head_view calls mm3x9t then mm9x9 in both), the overflow handlers (in_scalebm installs
;   overflow_handler_reg and clears overflow_count, as in FM).
; - the clipper: do_3d_clip calls clip_right/left/top/bottom on the same code bits as FM;
;   clip_overflow, clip_needed, must_clip_3d, draw_poly_buf_ptr and the modify_ patch points
;   follow do_closure's layout; smooth_closure is do_closure's other branch.
; - the texture mapper: _asm_texture_map_1 calls _asm_clip_polygon, _asm_project_polygon,
;   _asm_texture_map_v and _asm_texture_map as FM's does; the clip_polygon and clip_vertex
;   routines by who calls whom; the four scanline routines by the choice that picks them.
; - the page cache (update_cache .. parse_cpg, foundfreepage) instruction for instruction.
; do_obj (+49C, an opcode handler) is FM's assembly label do_obj, without the C
; underscore: FM has it beside the C function do_obj_ (seg033's _do_obj here), so the two
; names never met, and nothing outside this module refers to the handler.
; 3007 is FM render_3d, 7F12 FM update_cache and 7F0C a thunk to uncmp_tab; they keep the
; provisional names symbols.tsv gives them, because seg021 links against those.
; The rest keep IDA or provisional names.

        .model  medium
        .386

        extrn   _CmapCache:far
        extrn   _SquareRoot_seg021_22FD_A30:far
        extrn   _cmpbuf1_start:far
        extrn   _seg003_0272_0:far
        extrn   _seg003_0272_10:far
        extrn   _seg003_0272_5311:far
        extrn   _seg003_0272_536B:far
        extrn   _seg003_0272_8:far
        extrn   _seg003_0272_D1E:far
        extrn   _seg021_22FD_102B:far
        extrn   _sincos:far
        extrn   _seg021_22FD_C10:far
        extrn   dseg062_62a6:far
        extrn   seg003_0272:far
        extrn   seg052_519C:far
        extrn   seg_370D:far
        extrn   _seg_5DFD:far

        .code

        public  _exp_8str
        public  _exp_4str
        public  _seg004_0849_63
        public  _seg004_0849_CB
        public  _exp_4run
        public  _exp_5run
        public  _exp_8run
        public  _seg004_0849_18D
        public  _seg004_0849_194
        public  _sphere_check
        public  _get_dist
        public  do_obj
        public  _smooth_closure
        public  _seg004_0849_798
        public  _seg004_0849_7FC
        public  _seg004_0849_92E
        public  _seg004_0849_A64
        public  _seg004_0849_B98
        public  _smooth_over
        public  _seg004_0849_D83
        public  _seg004_0849_DB0
        public  _seg004_0849_DBE
        public  _seg004_0849_DD2
        public  _seg004_0849_E11
        public  _create_matrix
        public  _head_view
        public  _seg004_0849_EDD
        public  _seg004_0849_F08
        public  _angles_2_matrix
        public  _seg004_0849_1006
        public  _do_int2
        public  _do_eof
        public  _do_pntres
        public  _do_lnres
        public  _do_movei
        public  _do_movec
        public  _do_movem
        public  _do_move_odata
        public  _do_multires
        public  _do_minimulti
        public  _do_setshade
        public  _do_uwshade
        public  _do_defres
        public  _do_minires
        public  _do_strres
        public  _do_cntres
        public  _do_rod
        public  _flat_rod
        public  _do_polyres
        public  _do_closure
        public  _do_asmcal
        public  _do_sfcal
        public  _do_ifge
        public  _do_ifle
        public  _do_sortvar
        public  _do_sortnorm
        public  _do_sortnorm_x0
        public  _do_sortnorm_y0
        public  _do_sortnorm_z0
        public  _do_org
        public  _do_hull
        public  _flat_y_rel
        public  _do_y_rel
        public  _do_movres
        public  _flat_x_rel
        public  _do_x_rel
        public  _flat_z_rel
        public  _do_z_rel
        public  _do_xz_rel
        public  _do_xy_rel
        public  _do_yz_rel
        public  _flat_xz_rel
        public  _flat_xy_rel
        public  _flat_yz_rel
        public  _do_setcolor
        public  _do_setcolv
        public  _do_goursurfv
        public  _do_goursurf
        public  _do_uwsurf
        public  _do_transsurf
        public  _do_vexsurf
        public  _do_cavesurf
        public  _do_ijmp
        public  _do_ijmp_i
        public  _do_rorg
        public  _do_defdelta
        public  _do_addres
        public  _do_subres
        public  _do_ihcall_ind
        public  _do_ihcall
        public  _do_ibcall_ind
        public  _do_ibcall
        public  _do_ipcall_ind
        public  _do_ipcall
        public  _do_pushc
        public  _do_popc
        public  _do_jnorm_x0
        public  _do_jnorm_y0
        public  _do_jnorm_z0
        public  _do_jnorm_yz0
        public  _do_jnorm_xz0
        public  _do_jnorm_xy0
        public  _do_jnorm
        public  _clip_overflow
        public  _do_3d_clip
        public  _clip_top
        public  _clip_bot
        public  _clip_right
        public  _clip_left
        public  _do_parapiped
        public  _seg004_0849_3000
        public  _render_3d
        public  _scale_matrix
        public  _check_flat
        public  _set_accept
        public  _self_modify
        public  _mini_xlate_rotate_pnt
        public  _load_xlate_rotate_pnt
        public  _mxmul
        public  _seg004_0849_35FF
        public  _instance_vvars_head
        public  _instance_head
        public  _instance_vvars_pitch
        public  _instance_pitch
        public  _instance_vvars_bank
        public  _instance_bank
        public  _code_pnt
        public  _mm9x9
        public  _mm3x9_bpsi
        public  _mm3x9t
        public  _overflow_handler_reg
        public  _overflow_handler_mem
        public  _overflow_handler_special_bp
        public  _do_set_tmctxt
        public  _do_set_gmap_ctxt
        public  _do_ctxt_gtmap
        public  _do_ctxt_gmap
        public  _do_gtri
        public  _do_gtmap
        public  _do_gwtmap
        public  _do_compact_map
        public  _do_compact_tmap
        public  _do_compact_wtmap
        public  _do_bcompact_map
        public  _do_bcompact_tmap
        public  _do_bcompact_wtmap
        public  _do_tmap_tri
        public  _do_wtmap
        public  _do_tmap
        public  _draw_solid_tmap
        public  _seg004_0849_3F77
        public  _do_scalebm
        public  _clip_vertex_x_neg_z
        public  _clip_vertex_y_neg_z
        public  _clip_vertex_x_z
        public  _clip_vertex_y_z
        public  __asm_clip_polygon_left
        public  __asm_clip_polygon_bottom
        public  __asm_clip_polygon_right
        public  __asm_clip_polygon_top
        public  __asm_clip_polygon
        public  __asm_project_polygon
        public  __asm_texture_map_1
        public  _divide_overflow_handler
        public  _seg004_0849_4F5C
        public  _seg004_0849_4F73
        public  _asm_texture_map_from_draw_poly
        public  _compute_lighting_value
        public  _compute_square_root_32
        public  _seg004_0849_51B0
        public  _seg004_0849_54C0
        public  _seg004_0849_5712
        public  _compute_min_max_y
        public  __asm_texture_map
        public  _seg004_0849_5CDF
        public  _seg004_0849_5D2E
        public  _seg004_0849_5D66
        public  _seg004_0849_6240
        public  _seg004_0849_6437
        public  _seg004_0849_6632
        public  __asm_texture_map_v
        public  _seg004_0849_6866
        public  _seg004_0849_68B5
        public  _seg004_0849_6952
        public  _do_fetchmap
        public  _do_settmobj
        public  _do_uwobj
        public  _do_uwcrit
        public  _seg004_0849_7EDE
        public  _seg004_0849_7F0C
        public  _update_cache
        public  _unload_cpg
        public  _load_cpg
        public  _lcpg_pghere
        public  _parse_cpg
        public  _do_setbmcol
        public  _do_mouseq
        public  _do_uwcolv

; uncmp_pal: the 16 or 32 colour palette of a 4 or 5 bit image, which the exp_ decoders
; translate through (xlat with BX = 0). seg004_0849_63 fills it, from the image header or a
; lightabs shading row. Ten (0Ah) in the shipped file is just its initial contents.
_uncmp_pal      db      0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah
                db      0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah

; seg004_0849_20  (+20)
_exp_8str proc near
        call    _seg004_0849_CB
        mov     si,bp
        mov     dx,seg _cmpbuf1_start
        mov     es,dx
        mov     di,0
        lodsw
        mov     cx,ax
L0030:
        lodsb
        xlat    byte ptr cs:_uncmp_pal[bx]
        stosb
        loop    L0030
        mov     ax,dx
        ret
_exp_8str endp

; seg004_0849_39  (+39)
_exp_4str proc near
        mov     cx,1
        call    _seg004_0849_63
        mov     si,bp
        mov     dx,seg _cmpbuf1_start
        mov     es,dx
        mov     di,0
        lodsw
        mov     cx,ax
L004C:
        lodsb
        mov     ah,al
        and     al,0F0h
        shr     al,4
        xlat    byte ptr cs:_uncmp_pal[bx]
        stosb
        mov     al,ah
        and     al,0Fh
        xlat    byte ptr cs:_uncmp_pal[bx]
        stosb
        loop    L004C
        mov     ax,dx
        ret
_exp_4str endp

; seg004_0849_63  (+63)
_seg004_0849_63 proc near
        mov     di,0
        mov     bx,@code
        mov     es,bx
        cmp     dh,0FFh
        jne     short L0079
        mov     dx,ax
        shl     cx,4
        rep movsb
        jmp     short L00C5
L0079:
        mov     bh,dh
        xor     bl,bl
        add     bx,offset lightabs
        mov     dx,ax
L0083:
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        loop    L0083
L00C5:
        mov     bx,0
        mov     ds,dx
        ret
_seg004_0849_63 endp

; seg004_0849_CB  (+CB)
_seg004_0849_CB proc near
        cmp     dh,0FFh
        mov     bh,dh
        xor     bl,bl
        add     bx,offset lightabs
        mov     ds,ax
        ret
_seg004_0849_CB endp

; seg004_0849_D9  (+D9)
_exp_4run proc near
        mov     cx,1
        call    _seg004_0849_63
        mov     si,bp
        mov     dx,seg _cmpbuf1_start
        mov     es,dx
        mov     di,0
        lodsw
        mov     dx,ax
        inc     ax
        shr     ax,1
        mov     cx,ax
L00F1:
        lodsb
        mov     ah,al
        and     al,0F0h
        shr     al,4
        stosb
        mov     al,ah
        and     al,0Fh
        stosb
        loop    L00F1
        mov     cx,dx
        mov     ax,seg _cmpbuf1_start
        mov     ds,ax
        mov     si,0
        mov     ax,seg _cmpbuf1_start
        mov     es,ax
        mov     di,5400h
        call    _seg004_0849_18D
        mov     ax,es
        add     ax,540h
        ret
_exp_4run endp

; seg004_0849_11C  (+11C)
_exp_5run proc near
        mov     cx,2
        call    _seg004_0849_63
        mov     si,bp
        mov     dx,seg _cmpbuf1_start
        mov     es,dx
        mov     di,0
        lodsw
        mov     dx,ax
        add     ax,7
        shr     ax,3
        mov     cx,ax
L0137:
        lodsb
        xor     ah,ah
        ror     ax,3
        stosb
        lodsb
        shr     ah,5
        ror     ax,6
        stosb
        shr     ax,3
        xchg    ah,al
        stosb
        lodsb
        shr     ah,7
        ror     ax,4
        stosb
        lodsb
        shr     ah,4
        ror     ax,7
        stosb
        xor     al,al
        rol     ax,5
        stosb
        lodsb
        shr     ah,6
        ror     ax,5
        stosb
        shr     ax,0Bh
        stosb
        loop    L0137
        mov     cx,dx
        mov     ax,seg _cmpbuf1_start
        mov     ds,ax
        mov     si,0
        mov     ax,seg _cmpbuf1_start
        mov     es,ax
        mov     di,5400h
        call    _seg004_0849_18D
        mov     ax,es
        add     ax,540h
        ret
_exp_5run endp

; seg004_0849_18B  (+18B)
_exp_8run proc near
        ret
L018C:
        ret
_exp_8run endp

; seg004_0849_18D  (+18D)
_seg004_0849_18D proc near
        mov     bp,cx
        add     bp,si
        mov     dx,3
_seg004_0849_18D endp

; seg004_0849_194  (+194)
_seg004_0849_194 proc near
        cmp     si,bp
        jae     L018C
        sub     ax,ax
        lodsb
        cmp     al,2
        jbe     short L01F9
        mov     cx,ax
        lodsb
        xlat    byte ptr cs:_uncmp_pal[bx]
        rep stosb
L01A6:
        sub     ax,ax
        lodsb
        mov     cx,ax
        jcxz    L01CE
L01AD:
        lodsb
        xlat    byte ptr cs:_uncmp_pal[bx]
        stosb
        loop    L01AD
        jmp     _seg004_0849_194
L01B5:
        mov     cl,4
        lodsb
        shl     ax,cl
        or      al,byte ptr [si]
        shl     ax,cl
        or      al,byte ptr [si+1]
        shl     ax,cl
        or      al,byte ptr [si+2]
        add     si,dx
        mov     cx,ax
        xor     ah,ah
        jmp     L01AD
L01CE:
        lodsb
        test    al,al
        je      L01B5
        shl     ax,4
        mov     cx,ax
        lodsb
        or      cx,ax
        jmp     L01AD
L01DD:
        mov     cl,4
        lodsb
        shl     ax,cl
        or      al,byte ptr [si]
        shl     ax,cl
        or      al,byte ptr [si+1]
        shl     ax,cl
        or      al,byte ptr [si+2]
        add     si,dx
        mov     cx,ax
        lodsb
        xlat    byte ptr cs:_uncmp_pal[bx]
        rep stosb
        jmp     L01A6
L01F9:
        je      short L0213
        test    al,al
        jne     short L0211
        lodsb
        test    al,al
        je      L01DD
        shl     ax,4
        mov     cx,ax
        lodsb
        or      cx,ax
        lodsb
        xlat    byte ptr cs:_uncmp_pal[bx]
        rep stosb
L0211:
        jmp     L01A6
L0213:
        mov     byte ptr cs:L01A6,0C3h
        sub     ax,ax
        lodsb
        mov     cx,ax
        jcxz    L0247
L0220:
        push    cx
        call    _seg004_0849_194
        pop     cx
        loop    L0220
        mov     byte ptr cs:L01A6,29h
        jmp     L01A6
L0230:
        mov     cl,4
        lodsb
        shl     ax,cl
        or      al,byte ptr [si]
        shl     ax,cl
        or      al,byte ptr [si+1]
        shl     ax,cl
        or      al,byte ptr [si+2]
        add     si,dx
        mov     cx,ax
        jmp     L0220
L0247:
        lodsb
        test    al,al
        je      L0230
        shl     ax,4
        mov     cx,ax
        lodsb
        or      cx,ax
        jmp     L0220
        db      0Ah dup (0)
_seg004_0849_194 endp

; seg004_0849_260  (+260)
_sphere_check proc near
        mov     word ptr ds:[10h],cx
        shl     si,cl
        shl     bx,cl
        neg     bx
        shl     bp,cl
        neg     bp
        shl     ax,cl
        neg     ax
        mov     cx,ax
        mov     ax,bx
        imul    word ptr ds:[14B6h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[14BCh]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14C2h]
        add     di,dx
        mov     word ptr ds:[0Eh],di
        jns     short L029C
        add     di,si
        jns     short L0299
        jmp     L040D
L0299:
        jmp     L036F
L029C:
        mov     ax,bx
        imul    word ptr ds:[14B2h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[14B8h]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14BEh]
        add     di,dx
        mov     ax,si
        mul     word ptr ds:[24E8h]
        mov     ax,word ptr ds:[0Eh]
        sub     ax,di
        jns     short L02CB
        add     ax,dx
        jns     short L02C8
        jmp     L040D
L02C8:
        jmp     L0398
L02CB:
        sub     ax,dx
        jns     short L02D2
        jmp     L0398
L02D2:
        add     di,word ptr ds:[0Eh]
        jns     short L02E2
        add     di,dx
        jns     short L02DF
        jmp     L040D
L02DF:
        jmp     L03A2
L02E2:
        sub     di,dx
        jns     short L02E9
        jmp     L03A2
L02E9:
        mov     ax,bx
        imul    word ptr ds:[14B4h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[14BAh]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14C0h]
        add     di,dx
        mov     ax,si
        mul     word ptr ds:[24EAh]
        mov     ax,word ptr ds:[0Eh]
        sub     ax,di
        jns     short L0318
        add     ax,dx
        jns     short L0315
        jmp     L040D
L0315:
        jmp     L03CB
L0318:
        sub     ax,dx
        jns     short L031F
        jmp     L03CB
L031F:
        add     di,word ptr ds:[0Eh]
        jns     short L032F
        add     di,dx
        jns     short L032C
        jmp     L040D
L032C:
        jmp     L03D5
L032F:
        sub     di,dx
        jns     short L0336
        jmp     L03D5
L0336:
        mov     ax,word ptr ds:[0Eh]
        or      ax,ax
        jns     short L0341
        xor     ax,ax
        jmp     short L0368
L0341:
        mov     cx,word ptr ds:[10h]
        sar     ax,cl
        test    word ptr ds:[24E6h],0FFFFh
        jns     short L0368
        mov     cx,word ptr ds:[24E6h]
        neg     cx
        mov     dx,ax
        xor     ax,ax
        sar     dx,1
        cmp     dx,cx
        jl      short L0364
        mov     ax,7FFFh
        jmp     short L0368
L0364:
        rcr     ax,1
        div     cx
L0368:
        shr     ax,1
        clc
        mov     si,0FFFFh
        ret
L036F:
        mov     ax,bx
        imul    word ptr ds:[14B2h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[14B8h]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14BEh]
        add     di,dx
        mov     ax,si
        mul     word ptr ds:[24E8h]
        mov     ax,word ptr ds:[0Eh]
        sub     ax,di
        jns     short L0398
        add     ax,dx
        js      short L040D
L0398:
        add     di,word ptr ds:[0Eh]
        jns     short L03A2
        add     di,dx
        js      short L040D
L03A2:
        mov     ax,bx
        imul    word ptr ds:[14B4h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[14BAh]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14C0h]
        add     di,dx
        mov     ax,si
        mul     word ptr ds:[24EAh]
        mov     ax,word ptr ds:[0Eh]
        sub     ax,di
        jns     short L03CB
        add     ax,dx
        js      short L040D
L03CB:
        add     di,word ptr ds:[0Eh]
        jns     short L03D5
        add     di,dx
        js      short L040D
L03D5:
        mov     ax,word ptr ds:[0Eh]
        or      ax,ax
        jns     short L03E0
        xor     ax,ax
        jmp     short L0407
L03E0:
        mov     cx,word ptr ds:[10h]
        sar     ax,cl
        test    word ptr ds:[24E6h],0FFFFh
        jns     short L0407
        mov     cx,word ptr ds:[24E6h]
        neg     cx
        mov     dx,ax
        xor     ax,ax
        sar     dx,1
        cmp     dx,cx
        jl      short L0403
        mov     ax,7FFFh
        jmp     short L0407
L0403:
        rcr     ax,1
        div     cx
L0407:
        shr     ax,1
        clc
        xor     si,si
        ret
L040D:
        stc
        ret
_sphere_check endp

; seg004_0849_40F  (+40F)
_get_dist proc near
        push    si
        mov     bx,word ptr ds:[16h]
        mov     ax,word ptr ds:[18h]
        cwd
        xor     bx,dx
        xor     ax,dx
        neg     dx
        add     bx,dx
        adc     ax,0
        mov     bp,ax
        mov     cx,word ptr ds:[1Eh]
        mov     ax,word ptr ds:[20h]
        cwd
        xor     cx,dx
        xor     ax,dx
        neg     dx
        add     cx,dx
        adc     ax,0
        cmp     bp,ax
        jl      short L0445
        jg      short L0442
        cmp     bx,cx
        jl      short L0445
L0442:
        xchg    bp,ax
        xchg    bx,cx
L0445:
        sar     bp,1
        rcr     bx,1
        sar     bp,1
        rcr     bx,1
        mov     di,ax
        mov     si,word ptr ds:[1Ah]
        mov     ax,word ptr ds:[1Ch]
        cwd
        xor     si,dx
        xor     ax,dx
        neg     dx
        add     cx,dx
        adc     ax,0
        cmp     ax,di
        jl      short L047A
        jg      short L046C
        cmp     si,cx
        jl      short L047A
L046C:
        add     bx,cx
        adc     bp,di
        sar     bp,1
        rcr     bx,1
        sar     bp,1
        rcr     bx,1
        jmp     short L0496
L047A:
        sar     ax,1
        rcr     si,1
        cmp     ax,bp
        jl      short L048E
        jg      short L0488
        cmp     si,bx
        jl      short L048E
L0488:
        sar     bp,1
        rcr     bx,1
        jmp     short L0492
L048E:
        sar     ax,1
        rcr     si,1
L0492:
        add     si,cx
        adc     ax,di
L0496:
        add     bx,si
        adc     bp,ax
        pop     si
        ret
_get_dist endp

; seg004_0849_49C  (+49C)
do_obj proc near
        mov     word ptr ds:[25E2h],0
        lodsw
        mov     di,ax
        add     di,si
        lodsw
        mov     word ptr ds:[24C8h],ax
        mov     cx,ax
        lodsw
        mov     bp,ax
        lodsw
        sub     ax,word ptr ds:[247Ah]
        mov     word ptr ds:[16h],ax
        xchg    bx,ax
        lodsw
        xchg    bx,ax
        sbb     bx,word ptr ds:[247Ch]
        mov     word ptr ds:[18h],bx
        jcxz    L04D9
        or      cx,cx
        jns     short L04D3
        neg     cx
L04CB:
        shl     ax,1
        rcl     bx,1
        loop    L04CB
        jmp     short L04D9
L04D3:
        sar     bx,1
        rcr     ax,1
        loop    L04D3
L04D9:
        cwd
        cmp     bx,dx
        je      short L04E1
        jmp     L05E3
L04E1:
        neg     ax
        mov     word ptr ds:[25E6h],ax
        cwd
        xor     ax,dx
        sub     ax,dx
        add     bp,ax
        jno     short L04F2
        jmp     L05E3
L04F2:
        lodsw
        mov     dx,ax
        lodsw
        sub     ax,word ptr ds:[247Eh]
        mov     word ptr ds:[1Ah],ax
        xchg    bx,ax
        lodsw
        xchg    bx,ax
        sbb     bx,word ptr ds:[2480h]
        mov     word ptr ds:[1Ch],bx
        mov     cx,word ptr ds:[24C8h]
        jcxz    L0522
        or      cx,cx
        jns     short L051C
        neg     cx
L0514:
        shl     ax,1
        rcl     bx,1
        loop    L0514
        jmp     short L0522
L051C:
        sar     bx,1
        rcr     ax,1
        loop    L051C
L0522:
        mov     cx,dx
        cwd
        cmp     bx,dx
        je      short L052C
        jmp     L05E3
L052C:
        neg     ax
        mov     word ptr ds:[25E8h],ax
        cwd
        xor     ax,dx
        sub     ax,dx
        add     ax,cx
        jno     short L053D
        jmp     L05E3
L053D:
        or      bp,ax
        lodsw
        mov     dx,ax
        lodsw
        sub     ax,word ptr ds:[2482h]
        mov     word ptr ds:[1Eh],ax
        xchg    bx,ax
        lodsw
        xchg    bx,ax
        sbb     bx,word ptr ds:[2484h]
        mov     word ptr ds:[20h],bx
        mov     cx,word ptr ds:[24C8h]
        jcxz    L056F
        or      cx,cx
        jns     short L0569
        neg     cx
L0561:
        shl     ax,1
        rcl     bx,1
        loop    L0561
        jmp     short L056F
L0569:
        sar     bx,1
        rcr     ax,1
        loop    L0569
L056F:
        mov     cx,dx
        cwd
        cmp     bx,dx
        jne     short L05E3
        neg     ax
        mov     word ptr ds:[25EAh],ax
        cwd
        xor     ax,dx
        sub     ax,dx
        add     ax,cx
        jo      short L05E3
        or      bp,ax
        lodsw
        push    si
        push    di
        xchg    si,ax
        mov     ax,si
modify_axis2:
        add     ax,word ptr ds:[25E6h]
        js      short L05F1
        mov     word ptr ds:[4],0
        mov     ax,bp
        mov     bx,22h
        or      ah,ah
        je      short L05A6
        xchg    al,ah
        xlatb
        jmp     short L05A9
L05A6:
        xlatb
        add     al,8
L05A9:
        mov     byte ptr ds:[25E0h],al
        or      bp,si
        mov     ax,bp
        mov     bx,22h
        or      ah,ah
        je      short L05BC
        xchg    al,ah
        xlatb
        jmp     short L05BF
L05BC:
        xlatb
        add     al,8
L05BF:
        mov     cl,al
        mov     bx,word ptr ds:[25E6h]
        mov     bp,word ptr ds:[25EAh]
        mov     ax,word ptr ds:[25E8h]
        call    _sphere_check
        pop     di
        jb      short L05F2
        mov     word ptr ds:[25FAh],ax
        call    _self_modify
        xchg    si,ax
        call    _set_accept
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
L05E3:
        mov     word ptr ds:[25E2h],0FFFFh
        mov     si,di
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
L05F1:
        pop     di
L05F2:
        pop     si
        mov     si,di
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
        db      5 dup (0)
do_obj endp

; seg004_0849_600  (+600)
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
L0644:
        add     ax,1234h
        stosw
        lodsw
        imul    word ptr ds:[2470h]
        idiv    bp
L064F:
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
        jmp     word ptr [bx+24F4h]
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
L06B2:
        add     ax,1234h
        jno     short L06BA
        jmp     L079C
L06BA:
        stosw
        lodsw
        imul    word ptr ds:[2470h]
        idiv    bp
L06C2:
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
        jmp     word ptr [bx+24F4h]
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
        jmp     word ptr [bx+24F4h]
_smooth_closure endp

; seg004_0849_798  (+798)
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
        jmp     word ptr [bx+24F4h]
_seg004_0849_798 endp

; seg004_0849_7FC  (+7FC)
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
        mov     bp,5959h
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

; The opcode table's do_uwsetup slot points here, at the word where the divide overflow
; handler below saves BP (FM do_uwsetup is the handler itself). The table is in the data
; segment, so this is the only trace of it in the module.
do_uwsetup:
L0D7F   dw      0
        dw      0

; seg004_0849_D83  (+D83): divide overflow handler, installed by the smooth shading code
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

        db      7 dup (0)

; seg004_0849_DB0  (+DB0): three far routines nothing in the EXE calls
_seg004_0849_DB0 proc far
        push    ds
        mov     cx,seg seg052_519C
        mov     ds,cx
        mov     ax,word ptr ds:[158h]
        mov     word ptr ds:[1A6h],ax
        pop     ds
        ret
_seg004_0849_DB0 endp

; seg004_0849_DBE  (+DBE)
_seg004_0849_DBE proc far
        push    ds
        mov     cx,seg seg052_519C
        mov     ds,cx
        mov     ax,word ptr ds:[1A6h]
        mov     word ptr ds:[158h],ax
        mov     word ptr ds:[15Ah],0
        pop     ds
        ret
_seg004_0849_DBE endp

; seg004_0849_DCC  (+DD2)
_seg004_0849_DD2 proc far
        push    ds
        mov     cx,seg seg052_519C
        mov     ds,cx
        mov     word ptr ds:[158h],ax
        or      bx,bx
        je      short L0DE5
        mov     word ptr ds:[15Ah],0
L0DE5:
        mov     word ptr ds:[190h],0
        mov     word ptr ss:[580h],0FFFFh
        mov     di,ax
        cmp     word ptr [di],0
        jne     short L0E0F
        mov     ax,word ptr [di+4]
        cmp     ax,0A44h
        jne     short L0E0F
        test    word ptr [di+6],0FFFFh
        jne     short L0E0F
        mov     word ptr ss:[580h],0
L0E0F:
        pop     ds
        ret
_seg004_0849_DD2 endp

; seg004_0849_E11  (+E11)
_seg004_0849_E11 proc near
        int     2
        jmp     short L0E51
_seg004_0849_E11 endp

; seg004_0849_E15  (+E15)
_create_matrix proc near
        mov     ax,0FFFFh
        mov     word ptr ds:[168h],ax
        mov     si,word ptr ds:[158h]
        cmp     si,130h
        je      short L0E27
        int     2
L0E27:
        lodsw
        mov     bx,ax
        mov     ax,word ptr [bx+15Ch]
        call    ax
        mov     ax,word ptr ds:[158h]
        mov     word ptr ds:[15Ah],ax
        mov     si,247Ah
        mov     di,192h
        mov     cx,6
        rep movsw
        mov     ax,word ptr ds:[14B6h]
        stosw
        mov     ax,word ptr ds:[14BCh]
        stosw
        mov     ax,word ptr ds:[14C2h]
        stosw
        mov     ax,word ptr ds:[24E6h]
        stosw
L0E51:
        ret
_create_matrix endp

; seg004_0849_E52  (+E52)
_head_view proc near
        lodsw
        mov     word ptr ds:[24E6h],ax
        lodsw
        mov     bp,ax
        lodsw
        or      ax,ax
        jne     short L0E6F
        test    word ptr ss:[0A82h],0FFFFh
        jne     short L0E6F
        mov     ax,bp
        sub     ax,4
        mov     word ptr ds:[168h],ax
L0E6F:
        test    word ptr [si+6],0FFFFh
        jne     short L0E94
        mov     word ptr [si+6],1
        push    si
        push    bp
        add     si,8
        mov     di,si
        add     di,4
        call    _seg004_0849_1006
        pop     bp
        pop     si
        mov     di,word ptr [si+1Eh]
        or      di,di
        je      short L0E94
        call    _seg004_0849_EDD
L0E94:
        add     bp,12h
        test    word ptr [bp-12h],0FFFFh
        je      short L0EA3
        push    si
        call    _angles_2_matrix
        pop     si
L0EA3:
        call    _mm3x9t
        mov     ax,bx
        cwd
        add     ax,word ptr [bp-0Ch]
        adc     dx,word ptr [bp-0Ah]
        mov     word ptr ds:[247Ah],ax
        mov     word ptr ds:[247Ch],dx
        mov     ax,cx
        cwd
        add     ax,word ptr [bp-8]
        adc     dx,word ptr [bp-6]
        mov     word ptr ds:[247Eh],ax
        mov     word ptr ds:[2480h],dx
        mov     ax,di
        cwd
        add     ax,word ptr [bp-4]
        adc     dx,word ptr [bp-2]
        mov     word ptr ds:[2482h],ax
        mov     word ptr ds:[2484h],dx
        add     si,0Ch
        call    _mm9x9
        ret
_head_view endp

; seg004_0849_EDD  (+EDD)
_seg004_0849_EDD proc near
        mov     ax,di
        imul    word ptr [si+10h]
        shl     ax,1
        rcl     dx,1
        neg     dx
        mov     word ptr [si],dx
        mov     ax,di
        imul    word ptr [si+16h]
        shl     ax,1
        rcl     dx,1
        neg     dx
        mov     word ptr [si+2],dx
        mov     ax,di
        imul    word ptr [si+1Ch]
        shl     ax,1
        rcl     dx,1
        neg     dx
        mov     word ptr [si+4],dx
        ret
lag_view:                               ; FM lag_view, a bare return after head_view's helper
        ret
_seg004_0849_EDD endp

; seg004_0849_F08  (+F08)
_seg004_0849_F08 proc far
        call    _angles_2_matrix
        retf
_seg004_0849_F08 endp

; seg004_0849_F0C  (+F0C)
_angles_2_matrix proc near
        mov     ax,ss
        mov     ds,ax
        push    bp
        mov     di,bp
        mov     bx,word ptr [di+12h]
        neg     bx
        call    far ptr _sincos
        mov     word ptr es:[18Ah],bx
        mov     word ptr es:[18Ch],ax
        mov     bx,word ptr [di+14h]
        neg     bx
        call    far ptr _sincos
        mov     word ptr es:[186h],bx
        mov     word ptr es:[188h],ax
        mov     bx,word ptr [di+16h]
        neg     bx
        call    far ptr _sincos
        mov     word ptr es:[182h],bx
        mov     word ptr es:[184h],ax
        pop     bp
        mov     ax,es
        mov     ds,ax
        mov     ax,word ptr ds:[18Ch]
        neg     ax
        mov     word ptr [bp+0Ah],ax
        mov     ax,word ptr ds:[186h]
        imul    word ptr ds:[182h]
        shl     ax,1
        rcl     dx,1
        mov     bx,dx
        mov     ax,word ptr ds:[188h]
        imul    word ptr ds:[184h]
        shl     ax,1
        rcl     dx,1
        mov     di,dx
        xchg    dx,ax
        imul    word ptr ds:[18Ch]
        shl     ax,1
        rcl     dx,1
        add     dx,bx
        mov     word ptr [bp],dx
        mov     ax,word ptr ds:[188h]
        imul    word ptr ds:[18Ah]
        shl     ax,1
        rcl     dx,1
        neg     dx
        mov     word ptr [bp+6],dx
        mov     ax,word ptr ds:[186h]
        imul    word ptr ds:[184h]
        shl     ax,1
        rcl     dx,1
        mov     cx,dx
        mov     ax,word ptr ds:[188h]
        imul    word ptr ds:[182h]
        shl     ax,1
        rcl     dx,1
        mov     si,dx
        xchg    dx,ax
        imul    word ptr ds:[18Ch]
        shl     ax,1
        rcl     dx,1
        sub     dx,cx
        neg     dx
        mov     word ptr [bp+0Ch],dx
        mov     ax,cx
        imul    word ptr ds:[18Ch]
        shl     ax,1
        rcl     dx,1
        sub     si,dx
        mov     word ptr [bp+2],si
        mov     ax,word ptr ds:[186h]
        imul    word ptr ds:[18Ah]
        shl     ax,1
        rcl     dx,1
        mov     word ptr [bp+8],dx
        mov     ax,bx
        imul    word ptr ds:[18Ch]
        shl     ax,1
        rcl     dx,1
        add     dx,di
        mov     word ptr [bp+0Eh],dx
        mov     ax,word ptr ds:[184h]
        imul    word ptr ds:[18Ah]
        shl     ax,1
        rcl     dx,1
        neg     dx
        mov     word ptr [bp+4],dx
        mov     ax,word ptr ds:[182h]
        imul    word ptr ds:[18Ah]
        shl     ax,1
        rcl     dx,1
        mov     word ptr [bp+10h],dx
        ret
_angles_2_matrix endp

; seg004_0849_1006  (+1006)
_seg004_0849_1006 proc near
        mov     ax,ss
        mov     ds,ax
        mov     bx,word ptr es:[si]
        neg     bx
        call    far ptr _sincos
        mov     word ptr es:[18Ah],bx
        mov     word ptr es:[18Ch],ax
        mov     bx,word ptr es:[si+2]
        neg     bx
        call    far ptr _sincos
        mov     word ptr es:[182h],bx
        mov     word ptr es:[184h],ax
        mov     ax,es
        mov     ds,ax
        mov     ax,word ptr ds:[182h]
        mov     word ptr [di],ax
        mov     ax,word ptr ds:[184h]
        imul    word ptr ds:[18Ch]
        shl     ax,1
        rcl     dx,1
        neg     dx
        mov     word ptr [di+2],dx
        mov     ax,word ptr ds:[184h]
        imul    word ptr ds:[18Ah]
        shl     ax,1
        rcl     dx,1
        neg     dx
        mov     word ptr [di+4],dx
        mov     word ptr [di+6],0
        mov     ax,word ptr ds:[18Ah]
        mov     word ptr [di+8],ax
        mov     ax,word ptr ds:[18Ch]
        neg     ax
        mov     word ptr [di+0Ah],ax
        mov     ax,word ptr ds:[184h]
        mov     word ptr [di+0Ch],ax
        mov     ax,word ptr ds:[182h]
        imul    word ptr ds:[18Ch]
        shl     ax,1
        rcl     dx,1
        mov     word ptr [di+0Eh],dx
        mov     ax,word ptr ds:[182h]
        imul    word ptr ds:[18Ah]
        shl     ax,1
        rcl     dx,1
        mov     word ptr [di+10h],dx
        ret
_seg004_0849_1006 endp

; seg004_0849_1090  (+1090)
_do_int2 proc near
        int     2
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_int2 endp

; seg004_0849_1098  (+1098)
_do_eof proc near
        ret
_do_eof endp

; seg004_0849_1099  (+1099)
_do_pntres proc near
        lodsw
        mov     di,ax
        add     di,14D0h
        test    byte ptr ds:[25D3h],0FFh
        jne     short L10AD
        test    byte ptr [di+6],0FFh
        jne     short L10D2
L10AD:
        mov     cx,word ptr [di+4]
        mov     ax,word ptr [di+2]
        imul    word ptr ds:[2470h]
        idiv    cx
        add     ax,word ptr ds:[2476h]
        mov     bx,ax
        mov     ax,word ptr [di]
        imul    word ptr ds:[2472h]
        idiv    cx
        add     ax,word ptr ds:[2474h]
        push    si
        call    far ptr _seg003_0272_0
        pop     si
L10D2:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_pntres endp

; seg004_0849_10D8  (+10D8)
_do_lnres proc near
        lodsw
        mov     di,ax
        lodsw
        mov     bx,ax
        mov     al,byte ptr [di+14D6h]
        test    byte ptr [bx+14D6h],al
        jne     short L113F
        push    si
        or      al,byte ptr [bx+14D6h]
        jne     short L1145
        mov     bp,word ptr [di+14D4h]
        mov     ax,word ptr [di+14D0h]
        imul    word ptr ds:[2472h]
        idiv    bp
        add     ax,word ptr ds:[2474h]
        mov     cx,ax
        mov     ax,word ptr [di+14D2h]
        imul    word ptr ds:[2470h]
        idiv    bp
        add     ax,word ptr ds:[2476h]
        mov     si,ax
        mov     di,bx
        mov     bp,word ptr [di+14D4h]
        mov     ax,word ptr [di+14D2h]
        imul    word ptr ds:[2470h]
        idiv    bp
        add     ax,word ptr ds:[2476h]
        mov     bx,ax
        mov     ax,word ptr [di+14D0h]
        imul    word ptr ds:[2472h]
        idiv    bp
        add     ax,word ptr ds:[2474h]
        mov     dx,si
        call    far ptr _seg003_0272_8
L113E:
        pop     si
L113F:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
L1145:
        add     bx,14D0h
        add     di,14D0h
        or      al,al
        js      short L11C9
L1151:
        mov     word ptr ss:[5A5h],11ABh
        mov     bp,word ptr [bx+4]
        mov     ax,word ptr [bx]
        imul    word ptr ds:[2472h]
        idiv    bp
        add     ax,word ptr ds:[2474h]
        mov     cx,ax
        mov     ax,word ptr [bx+2]
        imul    word ptr ds:[2470h]
        idiv    bp
        add     ax,word ptr ds:[2476h]
        mov     si,ax
        mov     word ptr ss:[5A5h],11ADh
        mov     bp,word ptr [di+4]
        mov     ax,word ptr [di+2]
        imul    word ptr ds:[2470h]
        idiv    bp
        add     ax,word ptr ds:[2476h]
        mov     bx,ax
        mov     ax,word ptr [di]
        imul    word ptr ds:[2472h]
        idiv    bp
        add     ax,word ptr ds:[2474h]
        mov     dx,si
        call    far ptr _seg003_0272_10
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
        xchg    bx,di
        sti
        add     sp,6
        mov     al,byte ptr [di+6]
        test    al,1
        je      short L11BB
        jmp     L12D3
L11BB:
        test    al,2
        je      short L11C2
        jmp     L134D
L11C2:
        test    al,4
        jne     short L11D9
        jmp     L1258
L11C9:
        mov     al,byte ptr [di+6]
        or      al,al
        js      short L11D5
        xchg    di,bx
        mov     al,byte ptr [di+6]
L11D5:
        test    al,4
        je      short L1258
L11D9:
        mov     word ptr ss:[5A5h],offset _overflow_handler_reg
        mov     dx,word ptr [bx+4]
        sub     dx,word ptr [bx+2]
        mov     cx,word ptr [di+2]
        sub     cx,word ptr [bx+2]
        sub     cx,word ptr [di+4]
        add     cx,word ptr [bx+4]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        mov     bp,ax
        mov     ax,word ptr [di+2]
        sub     ax,word ptr [bx+2]
        imul    bp
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        adc     dx,0
        add     dx,word ptr [bx+2]
        or      dx,dx
        js      short L1258
        mov     cx,dx
        mov     ax,word ptr [di]
        sub     ax,word ptr [bx]
        imul    bp
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        adc     dx,0
        add     dx,word ptr [bx]
        mov     di,1482h
        cmp     di,bx
        jne     short L1231
        mov     si,147Ah
L1231:
        mov     word ptr [di],dx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],cx
        jcxz    L123D
        jmp     short L1240
L123D:
        jmp     L113E
L1240:
        xor     al,al
        cmp     dx,cx
        jle     short L124A
        inc     al
        inc     al
L124A:
        neg     cx
        cmp     dx,cx
        jge     short L1252
        inc     al
L1252:
        mov     byte ptr [di+6],al
        jmp     L1151
L1258:
        mov     word ptr ss:[5A5h],offset _overflow_handler_reg
        mov     dx,word ptr [bx+4]
        add     dx,word ptr [bx+2]
        mov     cx,word ptr [bx+2]
        sub     cx,word ptr [di+2]
        sub     cx,word ptr [di+4]
        add     cx,word ptr [bx+4]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        mov     bp,ax
        mov     ax,word ptr [di+2]
        sub     ax,word ptr [bx+2]
        imul    bp
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        adc     dx,0
        add     dx,word ptr [bx+2]
        mov     cx,dx
        mov     ax,word ptr [di]
        sub     ax,word ptr [bx]
        imul    bp
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        adc     dx,0
        add     dx,word ptr [bx]
        mov     di,1482h
        cmp     di,bx
        jne     short L12AC
        mov     si,147Ah
L12AC:
        mov     word ptr [di],dx
        mov     word ptr [di+2],cx
        jcxz    L12B5
        jmp     short L12B8
L12B5:
        jmp     L113E
L12B8:
        xor     al,al
        cmp     dx,cx
        jge     short L12C0
        inc     al
L12C0:
        neg     cx
        cmp     dx,cx
        jle     short L12CA
        inc     al
        inc     al
L12CA:
        mov     byte ptr [di+6],al
        mov     word ptr [di+4],cx
        jmp     L1151
L12D3:
        mov     word ptr ss:[5A5h],offset _overflow_handler_reg
        mov     dx,word ptr [bx+4]
        add     dx,word ptr [bx]
        mov     cx,word ptr [bx]
        sub     cx,word ptr [di]
        sub     cx,word ptr [di+4]
        add     cx,word ptr [bx+4]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        mov     bp,ax
        mov     ax,word ptr [di]
        sub     ax,word ptr [bx]
        imul    bp
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        adc     dx,0
        add     dx,word ptr [bx]
        or      dx,dx
        jns     short L134D
        mov     cx,dx
        mov     ax,word ptr [di+2]
        sub     ax,word ptr [bx+2]
        imul    bp
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        adc     dx,0
        add     dx,word ptr [bx+2]
        mov     di,1482h
        cmp     di,bx
        jne     short L1328
        mov     si,147Ah
L1328:
        mov     word ptr [di],cx
        mov     word ptr [di+2],dx
        jcxz    L1331
        jmp     short L1334
L1331:
        jmp     L113E
L1334:
        xor     al,al
        cmp     dx,cx
        jge     short L133C
        or      al,8
L133C:
        neg     cx
        cmp     dx,cx
        jge     short L1344
        or      al,4
L1344:
        mov     byte ptr [di+6],al
        mov     word ptr [di+4],cx
        jmp     L1151
L134D:
        mov     word ptr ss:[5A5h],offset _overflow_handler_reg
        mov     dx,word ptr [bx]
        sub     dx,word ptr [bx+4]
        mov     cx,word ptr [di+4]
        sub     cx,word ptr [bx+4]
        add     cx,word ptr [bx]
        sub     cx,word ptr [di]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        mov     bp,ax
        mov     ax,word ptr [di+2]
        sub     ax,word ptr [bx+2]
        imul    bp
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        adc     dx,0
        add     dx,word ptr [bx+2]
        mov     cx,dx
        mov     ax,word ptr [di]
        sub     ax,word ptr [bx]
        imul    bp
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        adc     dx,0
        add     dx,word ptr [bx]
        mov     di,1482h
        cmp     di,bx
        jne     short L139E
        mov     si,147Ah
L139E:
        mov     word ptr [di],dx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],dx
        jcxz    L13AA
        jmp     short L13AD
L13AA:
        jmp     L113E
L13AD:
        xor     al,al
        cmp     cx,dx
        jle     short L13B5
        mov     al,4
L13B5:
        neg     dx
        cmp     cx,dx
        jge     short L13BE
        mov     ax,8
L13BE:
        mov     byte ptr [di+6],al
        jmp     L1151
_do_lnres endp

; seg004_0849_13C4  (+13C4)
_do_movei proc near
        lodsw
        mov     di,ax
        lodsw
        mov     bx,ax
        mov     bx,word ptr [bx]
        mov     ax,word ptr [di]
        mov     word ptr [bx],ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_movei endp

; seg004_0849_13D6  (+13D6)
_do_movec proc near
        lodsw
        mov     di,ax
        movsw
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_movec endp

; seg004_0849_13E0  (+13E0)
_do_movem proc near
        lodsw
        mov     bx,ax
        mov     bx,word ptr [bx]
        lodsw
        xchg    bx,ax
        mov     word ptr [bx],ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_movem endp

; seg004_0849_13EF  (+13EF)
_do_move_odata proc near
        lodsw
        xchg    di,ax
        mov     bx,word ptr [bp+di]
        lodsw
        xchg    bx,ax
        mov     word ptr [bx],ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_move_odata endp

; seg004_0849_13FD  (+13FD)
_do_multires proc near
        lodsw
        mov     word ptr ds:[24DCh],ax
        lodsw
        mov     di,ax
        add     di,14D0h
        mov     word ptr ds:[24DEh],di
L140C:
        call    _load_xlate_rotate_pnt
        call    _code_pnt
        mov     di,word ptr ds:[24DEh]
        mov     word ptr [di],bx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],bp
        mov     byte ptr [di+6],al
        add     word ptr ds:[24DEh],8
        dec     word ptr ds:[24DCh]
        jne     L140C
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_multires endp

; seg004_0849_1432  (+1432)
_do_minimulti proc near
        lodsb
        xor     ah,ah
        mov     word ptr ds:[24DCh],ax
        lodsb
        xor     ah,ah
        shl     ax,3
        mov     di,ax
        add     di,14D0h
        mov     word ptr ds:[24DEh],di
L1448:
        call    _mini_xlate_rotate_pnt
        call    _code_pnt
        mov     di,word ptr ds:[24DEh]
        mov     word ptr [di],bx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],bp
        mov     byte ptr [di+6],al
        add     word ptr ds:[24DEh],8
        dec     word ptr ds:[24DCh]
        jne     L1448
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_minimulti endp

; seg004_0849_146E  (+146E)
_do_setshade proc near
        lodsw
        mov     cx,ax
        mov     di,14D0h
L1474:
        lodsw
        mov     bx,ax
        lodsb
        mov     byte ptr [bx+di+7],al
        loop    L1474
        inc     si
        and     si,-2
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_setshade endp

; seg004_0849_1487  (+1487)
_do_uwshade proc near
        lodsw
        mov     cx,ax
        lodsw
        mov     bx,ax
        mov     dx,word ptr [bx]
        mov     byte ptr cs:L6D20,dl
        mov     dx,word ptr ds:[2694h]
        mov     di,14D0h
L149B:
        lodsw
        mov     bx,ax
        lodsb
        add     al,dl
        cmp     al,0Eh
        jbe     short L14A7
        mov     al,0Eh
L14A7:
        mov     byte ptr [bx+di+7],al
        loop    L149B
        inc     si
        and     si,-2
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
        lodsw
        mov     bx,word ptr [si+6]
        mov     byte ptr [bx+14D7h],al
_do_uwshade endp

; seg004_0849_14BE  (+14BE)
_do_defres proc near
        call    _load_xlate_rotate_pnt
        lodsw
        mov     di,ax
        mov     word ptr [di+14D0h],bx
        mov     word ptr [di+14D2h],cx
        mov     word ptr [di+14D4h],bp
        call    _code_pnt
        mov     byte ptr [di+14D6h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_defres endp

; seg004_0849_14DD  (+14DD)
_do_minires proc near
        call    _mini_xlate_rotate_pnt
        lodsb
        xor     ah,ah
        shl     ax,3
        mov     di,ax
        mov     word ptr [di+14D0h],bx
        mov     word ptr [di+14D2h],cx
        mov     word ptr [di+14D4h],bp
        call    _code_pnt
        mov     byte ptr [di+14D6h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_minires endp

; seg004_0849_1501  (+1501)
_do_strres proc near
        lodsw
        mov     dx,si
        mov     si,ax
        add     si,14D0h
        mov     di,1BAh
        movsw
        movsw
        movsw
        lodsb
        stosw
        mov     byte ptr ds:[14CBh],al
        mov     byte ptr ds:[14CAh],al
        mov     word ptr ds:[1B6h],di
        mov     si,dx
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_strres endp

; seg004_0849_1524  (+1524)
_do_cntres proc near
        lodsw
        mov     dx,si
        mov     si,ax
        add     si,14D0h
        mov     di,word ptr ds:[1B6h]
        movsw
        movsw
        movsw
        lodsb
        stosw
        and     byte ptr ds:[14CBh],al
        or      byte ptr ds:[14CAh],al
        mov     word ptr ds:[1B6h],di
        mov     si,dx
L1544:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_cntres endp

; seg004_0849_154A  (+154A)
_do_rod proc near
        mov     byte ptr ds:[14CBh],0FFh
        mov     byte ptr ds:[14CAh],0
        lodsw
        or      ax,ax
        jne     short L1574
        mov     di,1BAh
        lodsw
        push    si
        mov     si,ax
        add     si,14D0h
        movsw
        movsw
        movsw
        lodsw
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        stosw
        pop     si
        jmp     short L15D1
L1574:
        mov     cl,byte ptr ds:[25E0h]
        shl     ax,cl
        mov     cx,ax
        imul    word ptr ds:[2492h]
        mov     ax,cx
        mov     cx,dx
        imul    word ptr ds:[2490h]
        mov     di,1BAh
        lodsw
        push    si
        mov     si,ax
        add     si,14D0h
        lodsw
        add     ax,dx
        mov     bx,ax
        stosw
        lodsw
        add     ax,cx
        push    cx
        mov     cx,ax
        stosw
        lodsw
        mov     bp,ax
        stosw
        call    _code_pnt
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        stosw
        pop     cx
        sub     si,6
        lodsw
        sub     ax,dx
        mov     bx,ax
        stosw
        lodsw
        sub     ax,cx
        mov     cx,ax
        stosw
        lodsw
        mov     bp,ax
        stosw
        call    _code_pnt
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        stosw
        pop     si
L15D1:
        lodsw
        or      ax,ax
        jne     short L15EE
        lodsw
        push    si
        mov     si,ax
        add     si,14D0h
        movsw
        movsw
        movsw
        lodsw
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        stosw
        pop     si
        jmp     short L1648
L15EE:
        mov     cl,byte ptr ds:[25E0h]
        shl     ax,cl
        mov     cx,ax
        imul    word ptr ds:[2492h]
        mov     ax,cx
        mov     cx,dx
        imul    word ptr ds:[2490h]
        lodsw
        push    si
        mov     si,ax
        add     si,14D0h
        lodsw
        sub     ax,dx
        mov     bx,ax
        stosw
        lodsw
        sub     ax,cx
        push    cx
        mov     cx,ax
        stosw
        lodsw
        mov     bp,ax
        stosw
        call    _code_pnt
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        stosw
        pop     cx
        sub     si,6
        lodsw
        add     ax,dx
        mov     bx,ax
        stosw
        lodsw
        add     ax,cx
        mov     cx,ax
        stosw
        lodsw
        mov     bp,ax
        stosw
        call    _code_pnt
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        stosw
        pop     si
L1648:
        mov     word ptr ds:[1B6h],di
        jmp     _do_closure
_do_rod endp

; seg004_0849_164F  (+164F)
_flat_rod proc near
        mov     byte ptr ds:[14CBh],0FFh
        mov     byte ptr ds:[14CAh],0
        lodsw
        or      ax,ax
        jne     short L1679
        mov     di,1BAh
        lodsw
        push    si
        mov     si,ax
        add     si,14D0h
        movsw
        movsw
        movsw
        lodsw
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        stosw
        pop     si
        jmp     short L16C6
L1679:
        mov     cl,byte ptr ds:[25E0h]
        shl     ax,cl
        imul    word ptr ds:[24ECh]
        mov     di,1BAh
        lodsw
        push    si
        mov     si,ax
        add     si,14D0h
        lodsw
        add     ax,dx
        mov     bx,ax
        stosw
        lodsw
        mov     cx,ax
        stosw
        lodsw
        mov     bp,ax
        stosw
        call    _code_pnt
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        stosw
        sub     si,6
        lodsw
        sub     ax,dx
        mov     bx,ax
        stosw
        lodsw
        mov     cx,ax
        stosw
        lodsw
        mov     bp,ax
        stosw
        call    _code_pnt
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        stosw
        pop     si
L16C6:
        lodsw
        or      ax,ax
        jne     short L16E3
        lodsw
        push    si
        mov     si,ax
        add     si,14D0h
        movsw
        movsw
        movsw
        lodsw
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        stosw
        pop     si
        jmp     short L172D
L16E3:
        mov     cl,byte ptr ds:[25E0h]
        shl     ax,cl
        imul    word ptr ds:[24ECh]
        lodsw
        push    si
        mov     si,ax
        add     si,14D0h
        lodsw
        sub     ax,dx
        mov     bx,ax
        stosw
        lodsw
        mov     cx,ax
        stosw
        lodsw
        mov     bp,ax
        stosw
        call    _code_pnt
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        stosw
        sub     si,6
        lodsw
        add     ax,dx
        mov     bx,ax
        stosw
        lodsw
        mov     cx,ax
        stosw
        lodsw
        mov     bp,ax
        stosw
        call    _code_pnt
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        stosw
        pop     si
L172D:
        mov     word ptr ds:[1B6h],di
        jmp     short _do_closure
_flat_rod endp

; seg004_0849_1733  (+1733)
_do_polyres proc near
        lodsw
        mov     cx,ax
        mov     di,1BAh
        xor     bx,bx
        not     bl
        mov     bp,14D0h
L1740:
        lodsw
        xchg    si,ax
        add     si,bp
        movsw
        movsw
        movsw
        mov     dl,byte ptr [si]
        or      bh,dl
        and     bl,dl
        movsw
        mov     si,ax
        loop    L1740
        je      short L1760
        mov     word ptr ds:[25E4h],0
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
L1760:
        mov     byte ptr ds:[14CAh],bh
        mov     byte ptr ds:[14CBh],bl
        mov     word ptr ds:[1B6h],di
_do_polyres endp

; seg004_0849_176C  (+176C)
_do_closure proc near
        mov     di,1BAh
        mov     word ptr ds:[1B8h],di
        cmp     word ptr ds:[14A6h],5946h
        jne     short draw_poly_buf_ptr
        jmp     _smooth_closure
draw_poly_buf_ptr:
        mov     word ptr ds:[25E4h],0
        test    byte ptr ds:[14CBh],0FFh
        je      short L178E
        jmp     L1544
L178E:
        test    byte ptr ds:[14CAh],0FFh
        jne     short clip_needed
L1795:
        push    si
        mov     ax,seg seg_370D
        mov     es,ax
        mov     di,415Eh
        mov     bx,word ptr ds:[1B6h]
        mov     si,word ptr ds:[1B8h]
        mov     cx,word ptr ds:[2472h]
L17AA:
        lodsw
        mov     bp,word ptr [si+2]
        imul    cx
        idiv    bp
modify_biasx:
        add     ax,4D2h
        stosw
        mov     ax,word ptr [si]
        imul    word ptr ds:[2470h]
        idiv    bp
modify_biasy:
        add     ax,4D2h
        stosw
        add     si,6
        cmp     si,bx
        jne     L17AA
        sub     bx,word ptr ds:[1B8h]
        shr     bx,1
        shr     bx,1
        shr     bx,1
        mov     cx,bx
        inc     word ptr ds:[25E4h]
        mov     bp,word ptr ds:[14A6h]
        call    far ptr _seg003_0272_5311
        mov     ax,ds
        mov     es,ax
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
clip_needed:
        js      short must_clip_3d
        push    si
L17F0:
        mov     word ptr ss:[5A5h],offset _clip_overflow
        mov     ax,seg seg_370D
        mov     es,ax
        mov     di,415Eh
        mov     bx,word ptr ds:[1B6h]
        mov     si,word ptr ds:[1B8h]
        mov     cx,word ptr ds:[2472h]
L180B:
        lodsw
        mov     bp,word ptr [si+2]
        imul    cx
        idiv    bp
modify_bx2:
        add     ax,4D2h
        jno     short L181B
        jmp     _do_3d_clip
L181B:
        stosw
        mov     ax,word ptr [si]
        imul    word ptr ds:[2470h]
        idiv    bp
modify_by2:
        add     ax,4D2h
        jno     short L182C
        jmp     _do_3d_clip
L182C:
        stosw
        add     si,6
        cmp     si,bx
        jne     L180B
        mov     cx,bx
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
        jmp     word ptr [bx+24F4h]
must_clip_3d:
        mov     word ptr ss:[5A5h],offset _overflow_handler_reg
        test    byte ptr ds:[14CAh],4
        je      short L1881
        push    si
        call    _clip_top
        pop     si
        test    byte ptr ds:[14CBh],0FFh
        jne     short L18E7
        test    byte ptr ds:[14CAh],0FFh
        js      short L1881
        jne     short L187E
        jmp     L1795
L187E:
        jmp     clip_needed
L1881:
        test    byte ptr ds:[14CAh],8
        je      short L18A3
        push    si
        call    _clip_bot
        pop     si
        test    byte ptr ds:[14CBh],0FFh
        jne     short L18E7
        test    byte ptr ds:[14CAh],0FFh
        js      short L18A3
        jne     short L18A0
        jmp     L1795
L18A0:
        jmp     clip_needed
L18A3:
        test    byte ptr ds:[14CAh],1
        je      short L18C5
        push    si
        call    _clip_left
        pop     si
        test    byte ptr ds:[14CBh],0FFh
        jne     short L18E7
        test    byte ptr ds:[14CAh],0FFh
        js      short L18C5
        jne     short L18C2
        jmp     L1795
L18C2:
        jmp     clip_needed
L18C5:
        test    byte ptr ds:[14CAh],2
        je      short L18E7
        push    si
        call    _clip_right
        pop     si
        test    byte ptr ds:[14CBh],0FFh
        jne     short L18E7
        test    byte ptr ds:[14CAh],0FFh
        js      short L18E7
        jne     short L18E4
        jmp     L1795
L18E4:
        jmp     clip_needed
L18E7:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_closure endp

; seg004_0849_18ED  (+18ED)
_do_asmcal proc near
        lodsw
        call    ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_asmcal endp

; seg004_0849_18F6  (+18F6)
_do_sfcal proc near
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     si
L1901:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_sfcal endp

; seg004_0849_1907  (+1907)
_do_ifge proc near
        lodsw
        mov     cx,ax
        lodsw
        mov     bx,ax
        lodsw
        cmp     word ptr [bx],ax
        jl      L1901
        add     si,cx
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_ifge endp

; seg004_0849_191A  (+191A)
_do_ifle proc near
        lodsw
        mov     cx,ax
        lodsw
        mov     bx,ax
        lodsw
        cmp     word ptr [bx],ax
        jg      L1901
        add     si,cx
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_ifle endp

; seg004_0849_192D  (+192D)
_do_sortvar proc near
        lodsw
        xchg    bx,ax
        lodsw
        cmp     word ptr [bx],ax
        jl      short L1950
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     si
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
L1950:
        lodsw
        add     ax,si
        mov     bx,ax
        lodsw
        push    si
        push    bx
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     si
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_sortvar endp

; seg004_0849_196E  (+196E)
_do_sortnorm proc near
        lodsw
        mov     cx,ax
        lodsw
        add     ax,word ptr ds:[25E6h]
        imul    cx
        mov     cx,dx
        mov     bp,ax
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[25E8h]
        imul    di
        add     bp,ax
        adc     cx,dx
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[25EAh]
        imul    di
        add     bp,ax
        adc     cx,dx
        js      L1950
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     si
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_sortnorm endp

; seg004_0849_19B6  (+19B6)
_do_sortnorm_x0 proc near
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[25E8h]
        imul    di
        mov     bp,ax
        mov     cx,dx
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[25EAh]
        imul    di
        add     bp,ax
        adc     cx,dx
        jns     short L19D7
        jmp     L1950
L19D7:
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     si
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_sortnorm_x0 endp

; seg004_0849_19F3  (+19F3)
_do_sortnorm_y0 proc near
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[25E6h]
        imul    di
        mov     bp,ax
        mov     cx,dx
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[25EAh]
        imul    di
        add     bp,ax
        adc     cx,dx
        jns     short L1A14
        jmp     L1950
L1A14:
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     si
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_sortnorm_y0 endp

; seg004_0849_1A30  (+1A30)
_do_sortnorm_z0 proc near
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[25E6h]
        imul    di
        mov     bp,ax
        mov     cx,dx
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[25E8h]
        imul    di
        add     bp,ax
        adc     cx,dx
        jns     short L1A51
        jmp     L1950
L1A51:
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     si
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_sortnorm_z0 endp

; seg004_0849_1A6D  (+1A6D)
_do_org proc near
        mov     word ptr ds:[25E2h],0
        lodsw
        sub     ax,word ptr ds:[247Ah]
        mov     bx,word ptr [si]
        inc     si
        inc     si
        sbb     bx,word ptr ds:[247Ch]
        cwd
        cmp     bx,dx
        jne     short L1AC1
        neg     ax
        mov     word ptr ds:[25E6h],ax
        lodsw
        sub     ax,word ptr ds:[247Eh]
        mov     bx,word ptr [si]
        inc     si
        inc     si
        sbb     bx,word ptr ds:[2480h]
        cwd
        cmp     bx,dx
        jne     short L1AC1
        neg     ax
        mov     word ptr ds:[25E8h],ax
        lodsw
        sub     ax,word ptr ds:[2482h]
        mov     bx,word ptr [si]
        inc     si
        inc     si
        sbb     bx,word ptr ds:[2484h]
        cwd
        cmp     bx,dx
        jne     short L1AC1
        neg     ax
        mov     word ptr ds:[25EAh],ax
        call    _self_modify
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
L1AC1:
        mov     word ptr ds:[25E2h],0FFFFh
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_org endp

; seg004_0849_1ACD  (+1ACD)
_do_hull proc near
        lodsw
        mov     cx,ax
        dec     cx
        mov     bx,14D6h
        lodsw
        mov     di,ax
        mov     dl,byte ptr [bx+di]
        mov     bp,dx
L1ADB:
        lodsw
        mov     di,ax
        or      dl,byte ptr [bx+di]
        and     bp,word ptr [bx+di]
        loop    L1ADB
        je      short L1AE9
        jmp     _do_eof
L1AE9:
        or      dl,dl
        jne     short L1AF2
        mov     al,0FFh
        call    _set_accept
L1AF2:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_hull endp

; seg004_0849_1AF8  (+1AF8)
_flat_y_rel proc near
        lodsw
        mov     bp,ax
        lodsw
        mov     cl,byte ptr ds:[25E0h]
        shl     ax,cl
        sar     ax,1
        mov     cx,ax
        lodsw
        xchg    bp,si
        mov     di,14D0h
        add     si,di
        add     di,ax
        movsw
        lodsw
        add     ax,cx
        stosw
        mov     cx,ax
        lodsw
        stosw
        mov     dx,ax
        lodsb
        and     al,0F3h
        cmp     cx,dx
        jle     short L1B3C
        add     cx,dx
        jge     short L1B31
        or      al,0Ch
        stosb
        mov     si,bp
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
L1B31:
        or      al,4
        stosb
        mov     si,bp
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
L1B3C:
        add     cx,dx
        jge     short L1B42
        or      al,8
L1B42:
        stosb
        mov     si,bp
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_flat_y_rel endp

; seg004_0849_1B4B  (+1B4B)
_do_y_rel proc near
        lodsw
        mov     di,ax
        lodsw
        mov     cl,byte ptr ds:[25E0h]
        shl     ax,cl
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        mov     di,ax
        imul    word ptr ds:[14B8h]
        add     bx,dx
        mov     ax,word ptr ds:[14BAh]
        imul    di
        add     cx,dx
        mov     ax,word ptr ds:[14BCh]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        mov     word ptr [di+14D0h],bx
        mov     word ptr [di+14D2h],cx
        mov     word ptr [di+14D4h],bp
        call    _code_pnt
        mov     byte ptr [di+14D6h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
        mov     bp,offset _instance_pitch
        jmp     short L1BA0
        mov     bp,offset _instance_bank
        jmp     short L1BA0
        mov     bp,offset _instance_head
L1BA0:
        lodsw
        mov     di,ax
        mov     bx,word ptr [di+2]
        cmp     bx,word ptr [di]
        je      short L1BDC
        jg      short L1BC5
        mov     ax,word ptr [di+4]
        imul    word ptr ss:[0A80h]
        shl     ax,1
        rcl     dx,1
        add     bx,dx
        cmp     bx,word ptr [di]
        jle     short L1BC0
        mov     bx,word ptr [di]
L1BC0:
        mov     word ptr [di+2],bx
        jmp     short L1BDC
L1BC5:
        mov     ax,word ptr [di+4]
        imul    word ptr ss:[0A80h]
        shl     ax,1
        rcl     dx,1
        sub     bx,dx
        cmp     bx,word ptr [di]
        jge     short L1BD9
        mov     bx,word ptr [di]
L1BD9:
        mov     word ptr [di+2],bx
L1BDC:
        push    word ptr ds:[25E6h]
        push    word ptr ds:[25E8h]
        push    word ptr ds:[25EAh]
        lodsw
        sub     word ptr ds:[25E6h],ax
        lodsw
        sub     word ptr ds:[25E8h],ax
        lodsw
        sub     word ptr ds:[25EAh],ax
        push    si
        mov     si,word ptr [di+6]
        or      bx,bx
        je      short L1C6D
        push    word ptr ds:[14B2h]
        push    word ptr ds:[14B8h]
        push    word ptr ds:[14BEh]
        push    word ptr ds:[14B4h]
        push    word ptr ds:[14BAh]
        push    word ptr ds:[14C0h]
        push    word ptr ds:[14B6h]
        push    word ptr ds:[14BCh]
        push    word ptr ds:[14C2h]
        call    bp
        push    si
        call    _check_flat
        pop     si
        call    _self_modify
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     word ptr ds:[14C2h]
        pop     word ptr ds:[14BCh]
        pop     word ptr ds:[14B6h]
        pop     word ptr ds:[14C0h]
        pop     word ptr ds:[14BAh]
        pop     word ptr ds:[14B4h]
        pop     word ptr ds:[14BEh]
        pop     word ptr ds:[14B8h]
        pop     word ptr ds:[14B2h]
        pop     si
        pop     word ptr ds:[25EAh]
        pop     word ptr ds:[25E8h]
        pop     word ptr ds:[25E6h]
        call    _self_modify
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
L1C6D:
        call    _self_modify
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     si
        pop     word ptr ds:[25EAh]
        pop     word ptr ds:[25E8h]
        pop     word ptr ds:[25E6h]
        call    _self_modify
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_y_rel endp

; seg004_0849_1C8C  (+1C8C)
_do_movres proc near
        lodsw
        mov     di,ax
        lodsw
        xchg    si,ax
        add     di,14D0h
        add     si,14D0h
        movsw
        movsw
        movsw
        movsb
        mov     si,ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_movres endp

; seg004_0849_1CA5  (+1CA5)
_flat_x_rel proc near
        lodsw
        mov     di,ax
        lodsw
        mov     cl,byte ptr ds:[25E0h]
        shl     ax,cl
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        mov     di,ax
        imul    word ptr ds:[14B2h]
        add     bx,dx
        mov     ax,di
        imul    word ptr ds:[14B6h]
        lodsw
        mov     di,ax
        mov     word ptr [di+14D0h],bx
        mov     word ptr [di+14D2h],cx
        xor     al,al
        add     bp,dx
        mov     word ptr [di+14D4h],bp
        jg      short L1CE0
        mov     al,80h
L1CE0:
        cmp     bx,bp
        jle     short L1CE6
        inc     ax
        inc     ax
L1CE6:
        cmp     cx,bp
        jle     short L1CEC
        add     al,4
L1CEC:
        add     bx,bp
        jge     short L1CF1
        inc     ax
L1CF1:
        add     cx,bp
        jge     short L1CF7
        add     al,8
L1CF7:
        mov     byte ptr [di+14D6h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_flat_x_rel endp

; seg004_0849_1D01  (+1D01)
_do_x_rel proc near
        lodsw
        mov     di,ax
        lodsw
        mov     cl,byte ptr ds:[25E0h]
        shl     ax,cl
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        mov     di,ax
        imul    word ptr ds:[14B2h]
        add     bx,dx
        mov     ax,word ptr ds:[14B4h]
        imul    di
        add     cx,dx
        mov     ax,word ptr ds:[14B6h]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        mov     word ptr [di+14D0h],bx
        mov     word ptr [di+14D2h],cx
        mov     word ptr [di+14D4h],bp
        call    _code_pnt
        mov     byte ptr [di+14D6h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_x_rel endp

; seg004_0849_1D49  (+1D49)
_flat_z_rel proc near
        lodsw
        mov     di,ax
        lodsw
        mov     cl,byte ptr ds:[25E0h]
        shl     ax,cl
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        mov     di,ax
        imul    word ptr ds:[14BEh]
        add     bx,dx
        mov     ax,word ptr ds:[14C2h]
        imul    di
        lodsw
        mov     di,ax
        mov     word ptr [di+14D0h],bx
        mov     word ptr [di+14D2h],cx
        xor     al,al
        add     bp,dx
        mov     word ptr [di+14D4h],bp
        jg      short L1D83
        mov     al,80h
L1D83:
        cmp     bx,bp
        jle     short L1D89
        inc     ax
        inc     ax
L1D89:
        cmp     cx,bp
        jle     short L1D8F
        add     al,4
L1D8F:
        add     bx,bp
        jge     short L1D94
        inc     ax
L1D94:
        add     cx,bp
        jge     short L1D9A
        add     al,8
L1D9A:
        mov     byte ptr [di+14D6h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_flat_z_rel endp

; seg004_0849_1DA4  (+1DA4)
_do_z_rel proc near
        lodsw
        mov     di,ax
        lodsw
        mov     cl,byte ptr ds:[25E0h]
        shl     ax,cl
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        mov     di,ax
        imul    word ptr ds:[14BEh]
        add     bx,dx
        mov     ax,word ptr ds:[14C0h]
        imul    di
        add     cx,dx
        mov     ax,word ptr ds:[14C2h]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        mov     word ptr [di+14D0h],bx
        mov     word ptr [di+14D2h],cx
        mov     word ptr [di+14D4h],bp
        call    _code_pnt
        mov     byte ptr [di+14D6h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_z_rel endp

; seg004_0849_1DEC  (+1DEC)
_do_xz_rel proc near
        mov     cl,byte ptr ds:[25E0h]
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[14B2h]
        mov     bx,dx
        mov     ax,word ptr ds:[14B6h]
        imul    di
        mov     bp,dx
        mov     ax,word ptr ds:[14B4h]
        imul    di
        lodsw
        shl     ax,cl
        mov     cx,dx
        mov     di,ax
        imul    word ptr ds:[14BEh]
        add     bx,dx
        mov     ax,word ptr ds:[14C0h]
        imul    di
        add     cx,dx
        mov     ax,word ptr ds:[14C2h]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        add     bx,word ptr [di+14D0h]
        add     cx,word ptr [di+14D2h]
        add     bp,word ptr [di+14D4h]
        lodsw
        mov     di,ax
        mov     word ptr [di+14D0h],bx
        mov     word ptr [di+14D2h],cx
        mov     word ptr [di+14D4h],bp
        call    _code_pnt
        mov     byte ptr [di+14D6h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_xz_rel endp

; seg004_0849_1E4D  (+1E4D)
_do_xy_rel proc near
        mov     cl,byte ptr ds:[25E0h]
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[14B2h]
        mov     bx,dx
        mov     ax,word ptr ds:[14B6h]
        imul    di
        mov     bp,dx
        mov     ax,word ptr ds:[14B4h]
        imul    di
        lodsw
        shl     ax,cl
        mov     cx,dx
        mov     di,ax
        imul    word ptr ds:[14B8h]
        add     bx,dx
        mov     ax,word ptr ds:[14BAh]
        imul    di
        add     cx,dx
        mov     ax,word ptr ds:[14BCh]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        add     bx,word ptr [di+14D0h]
        add     cx,word ptr [di+14D2h]
        add     bp,word ptr [di+14D4h]
        lodsw
        mov     di,ax
        mov     word ptr [di+14D0h],bx
        mov     word ptr [di+14D2h],cx
        mov     word ptr [di+14D4h],bp
        call    _code_pnt
        mov     byte ptr [di+14D6h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_xy_rel endp

; seg004_0849_1EAE  (+1EAE)
_do_yz_rel proc near
        mov     cl,byte ptr ds:[25E0h]
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[14BEh]
        mov     bx,dx
        mov     ax,word ptr ds:[14C2h]
        imul    di
        mov     bp,dx
        mov     ax,word ptr ds:[14C0h]
        imul    di
        lodsw
        shl     ax,cl
        mov     cx,dx
        mov     di,ax
        imul    word ptr ds:[14B8h]
        add     bx,dx
        mov     ax,word ptr ds:[14BAh]
        imul    di
        add     cx,dx
        mov     ax,word ptr ds:[14BCh]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        add     bx,word ptr [di+14D0h]
        add     cx,word ptr [di+14D2h]
        add     bp,word ptr [di+14D4h]
        lodsw
        mov     di,ax
        mov     word ptr [di+14D0h],bx
        mov     word ptr [di+14D2h],cx
        mov     word ptr [di+14D4h],bp
        call    _code_pnt
        mov     byte ptr [di+14D6h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_yz_rel endp

; seg004_0849_1F0F  (+1F0F)
_flat_xz_rel proc near
        mov     cl,byte ptr ds:[25E0h]
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[14B2h]
        mov     bx,dx
        mov     ax,word ptr ds:[14B6h]
        imul    di
        mov     bp,dx
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[14BEh]
        add     bx,dx
        mov     ax,word ptr ds:[14C2h]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        add     bx,word ptr [di+14D0h]
        add     bp,word ptr [di+14D4h]
        mov     cx,word ptr [di+14D2h]
        lodsw
        mov     di,ax
        mov     word ptr [di+14D0h],bx
        mov     word ptr [di+14D2h],cx
        mov     word ptr [di+14D4h],bp
        call    _code_pnt
        mov     byte ptr [di+14D6h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_flat_xz_rel endp

; seg004_0849_1F62  (+1F62)
_flat_xy_rel proc near
        mov     cl,byte ptr ds:[25E0h]
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[14B2h]
        mov     bx,dx
        mov     ax,word ptr ds:[14B6h]
        imul    di
        mov     bp,dx
        lodsw
        shl     ax,cl
        sar     ax,1
        mov     cx,ax
        lodsw
        mov     di,ax
        add     bx,word ptr [di+14D0h]
        add     cx,word ptr [di+14D2h]
        add     bp,word ptr [di+14D4h]
        lodsw
        mov     di,ax
        mov     word ptr [di+14D0h],bx
        mov     word ptr [di+14D2h],cx
        mov     word ptr [di+14D4h],bp
        call    _code_pnt
        mov     byte ptr [di+14D6h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_flat_xy_rel endp

; seg004_0849_1FAA  (+1FAA)
_flat_yz_rel proc near
        mov     cl,byte ptr ds:[25E0h]
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[14BEh]
        mov     bx,dx
        mov     ax,word ptr ds:[14C2h]
        imul    di
        mov     bp,dx
        lodsw
        shl     ax,cl
        sar     ax,1
        mov     cx,ax
        lodsw
        mov     di,ax
        add     bx,word ptr [di+14D0h]
        add     cx,word ptr [di+14D2h]
        add     bp,word ptr [di+14D4h]
        lodsw
        mov     di,ax
        mov     word ptr [di+14D0h],bx
        mov     word ptr [di+14D2h],cx
        mov     word ptr [di+14D4h],bp
        call    _code_pnt
        mov     byte ptr [di+14D6h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_flat_yz_rel endp

; seg004_0849_1FF2  (+1FF2)
_do_setcolor proc near
        lodsw
setcol_common:
        mov     bx,3B0Dh
        mov     word ptr ds:[14A6h],bx
        mov     bx,38B4h
        mov     word ptr ds:[14A8h],bx
        mov     bx,seg seg_370D
        mov     ds,bx
        call    far ptr _seg003_0272_536B
        mov     bp,seg seg052_519C
        mov     ds,bp
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_setcolor endp

; seg004_0849_2016  (+2016)
_do_setcolv proc near
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr [di]
        jmp     setcol_common
_do_setcolv endp

; seg004_0849_201E  (+201E)
_do_goursurfv proc near
        lodsw
        mov     bx,ax
        mov     ax,word ptr [bx]
        db      0A8h                    ; test al,: swallows do_goursurf's lodsw, keeping AX
_do_goursurfv endp

; seg004_0849_2024  (+2024)
_do_goursurf proc near
        lodsw
        mov     bx,ax
        push    es
        mov     ax,seg seg_370D
        mov     es,ax
        shl     bx,1
        mov     al,byte ptr es:[bx+21Fh]
        pop     es
        mov     byte ptr ds:[1B0h],al
        mov     word ptr ds:[14A6h],5946h
        mov     word ptr ds:[14A8h],5956h
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_goursurf endp

; seg004_0849_204A  (+204A)
_do_uwsurf proc near
        test    word ptr ds:[2692h],0FFh
        jne     short L207B
        mov     word ptr ds:[14A6h],5946h
        mov     word ptr ds:[14A8h],5956h
        mov     bx,es
        mov     ax,seg seg003_0272
        mov     es,ax
        mov     word ptr es:[0BC3h],0BC7h
        mov     word ptr es:[0BC5h],0C93h
        mov     es,bx
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
L207B:
        mov     bx,es
        mov     ax,seg seg_370D
        mov     es,ax
        mov     al,byte ptr es:[0DC1h]
        xor     ah,ah
        mov     es,bx
        jmp     setcol_common
_do_uwsurf endp

; seg004_0849_208D  (+208D)
_do_transsurf proc near
        mov     word ptr ds:[14A6h],5946h
        mov     word ptr ds:[14A8h],5956h
        mov     bx,es
        mov     ax,seg seg003_0272
        mov     es,ax
        mov     word ptr es:[0BC3h],0BF2h
        mov     word ptr es:[0BC5h],0CD9h
        mov     es,bx
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_transsurf endp

; seg004_0849_20B6  (+20B6)
_do_vexsurf proc near
        mov     word ptr ds:[14A6h],3B0Dh
        mov     word ptr ds:[14A8h],38B4h
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_vexsurf endp

; seg004_0849_20C8  (+20C8)
_do_cavesurf proc near
        mov     word ptr ds:[14A6h],249Ch
        mov     word ptr ds:[14A8h],249Ch
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
        lodsw
        push    si
        add     si,ax
        lodsw
        mov     byte ptr ds:[25D6h],al
        inc     si
        inc     si
        mov     word ptr ds:[24B8h],si
        shl     ax,1
        add     si,ax
        shl     ax,1
        add     si,ax
L20F0:
        lodsw
        mov     bx,ax
        lodsw
        mov     cx,ax
        lodsw
        mov     bp,ax
        call    _mxmul
        mov     di,word ptr ds:[24B8h]
        mov     word ptr [di],bx
        mov     word ptr [di+4],cx
        mov     word ptr [di+2],bp
        add     word ptr ds:[24B8h],6
        dec     byte ptr ds:[25D6h]
        jne     L20F0
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
        push    word ptr ds:[25E6h]
        push    word ptr ds:[25E8h]
        push    word ptr ds:[25EAh]
        call    _load_xlate_rotate_pnt
        mov     word ptr ds:[14ACh],bx
        mov     word ptr ds:[14AEh],cx
        mov     word ptr ds:[14B0h],bp
        mov     ax,word ptr ds:[25E6h]
        sub     ax,word ptr [si-6]
        mov     word ptr ds:[25E6h],ax
        mov     ax,word ptr ds:[25E8h]
        sub     ax,word ptr [si-2]
        mov     word ptr ds:[25E8h],ax
        mov     ax,word ptr ds:[25EAh]
        sub     ax,word ptr [si-4]
        mov     word ptr ds:[25EAh],ax
        mov     bp,14D0h
        lodsw
        add     di,ax
        lodsw
        push    si
        add     si,ax
        lodsw
        mov     dx,ax
        lodsw
        sub     al,byte ptr ds:[25E0h]
        mov     dh,al
        shr     dl,1
        jae     short L2196
        mov     cl,dh
        lodsw
        sar     ax,cl
        mov     bx,ax
        add     bx,word ptr ds:[14ACh]
        lodsw
        sar     ax,cl
        mov     bp,ax
        add     bp,word ptr ds:[14B0h]
        lodsw
        sar     ax,cl
        mov     cx,ax
        add     cx,word ptr ds:[14AEh]
        call    _code_pnt
        mov     word ptr [di],bx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],bp
        mov     byte ptr [di+6],al
        add     di,8
L2196:
        mov     cl,dh
        lodsw
        sar     ax,cl
        mov     bx,ax
        add     bx,word ptr ds:[14ACh]
        lodsw
        sar     ax,cl
        mov     bp,ax
        add     bp,word ptr ds:[14B0h]
        lodsw
        sar     ax,cl
        mov     cx,ax
        add     cx,word ptr ds:[14AEh]
        call    _code_pnt
        mov     word ptr [di],bx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],bp
        mov     byte ptr [di+6],al
        mov     cl,dh
        lodsw
        sar     ax,cl
        mov     bx,ax
        add     bx,word ptr ds:[14ACh]
        lodsw
        sar     ax,cl
        mov     bp,ax
        add     bp,word ptr ds:[14B0h]
        lodsw
        sar     ax,cl
        mov     cx,ax
        add     cx,word ptr ds:[14AEh]
        call    _code_pnt
        mov     word ptr [di+8],bx
        mov     word ptr [di+0Ah],cx
        mov     word ptr [di+0Ch],bp
        mov     byte ptr [di+0Eh],al
        add     di,10h
        dec     dl
        jne     L2196
        pop     si
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     si
        pop     word ptr ds:[25EAh]
        pop     word ptr ds:[25E8h]
        pop     word ptr ds:[25E6h]
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
        mov     cl,byte ptr ds:[25E0h]
        mov     bx,word ptr ds:[25E6h]
        neg     bx
        shl     bx,cl
        mov     bp,word ptr ds:[25EAh]
        neg     bp
        shl     bp,cl
        mov     ax,word ptr ds:[25E8h]
        neg     ax
        shl     ax,cl
        mov     cx,ax
        call    _mxmul
        mov     word ptr ds:[14ACh],bx
        mov     word ptr ds:[14AEh],cx
        mov     word ptr ds:[14B0h],bp
        mov     di,14D0h
        lodsw
        add     di,ax
        lodsw
        push    si
        add     si,ax
        lodsw
        mov     cx,ax
        lodsw
        sub     al,byte ptr ds:[25E0h]
        mov     dl,al
        shr     cl,1
        mov     dh,cl
        jae     short L2286
        mov     cl,dl
        lodsw
        sar     ax,cl
        mov     bx,ax
        add     bx,word ptr ds:[14ACh]
        lodsw
        sar     ax,cl
        mov     bp,ax
        add     bp,word ptr ds:[14B0h]
        lodsw
        sar     ax,cl
        mov     cx,ax
        add     cx,word ptr ds:[14AEh]
        call    _code_pnt
        mov     word ptr [di],bx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],bp
        mov     byte ptr [di+6],al
        add     di,8
L2286:
        mov     cl,dl
        lodsw
        sar     ax,cl
        mov     bx,ax
        add     bx,word ptr ds:[14ACh]
        lodsw
        sar     ax,cl
        mov     bp,ax
        add     bp,word ptr ds:[14B0h]
        lodsw
        sar     ax,cl
        mov     cx,ax
        add     cx,word ptr ds:[14AEh]
        call    _code_pnt
        mov     word ptr [di],bx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],bp
        mov     byte ptr [di+6],al
        mov     cl,dl
        lodsw
        sar     ax,cl
        mov     bx,ax
        add     bx,word ptr ds:[14ACh]
        lodsw
        sar     ax,cl
        mov     bp,ax
        add     bp,word ptr ds:[14B0h]
        lodsw
        sar     ax,cl
        mov     cx,ax
        add     cx,word ptr ds:[14AEh]
        call    _code_pnt
        mov     word ptr [di+8],bx
        mov     word ptr [di+0Ah],cx
        mov     word ptr [di+0Ch],bp
        mov     byte ptr [di+0Eh],al
        add     di,10h
        dec     dh
        jne     L2286
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_cavesurf endp

; seg004_0849_22EB  (+22EB)
_do_ijmp proc near
        lodsw
        add     si,ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_ijmp endp

; seg004_0849_22F4  (+22F4)
_do_ijmp_i proc near
        lodsw
        mov     bx,ax
        mov     si,word ptr [bx]
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_ijmp_i endp

; seg004_0849_22FF  (+22FF)
_do_rorg proc near
        lodsw
        sub     word ptr ds:[25E6h],ax
        lodsw
        sub     word ptr ds:[25E8h],ax
        lodsw
        sub     word ptr ds:[25EAh],ax
        call    _self_modify
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_rorg endp

; seg004_0849_2317  (+2317)
_do_defdelta proc near
        mov     cl,byte ptr ds:[25E0h]
        lodsw
        shl     ax,cl
        mov     bx,ax
        lodsw
        shl     ax,cl
        mov     bp,ax
        lodsw
        shl     ax,cl
        mov     cx,ax
        call    _mxmul
        lodsw
        mov     di,ax
        mov     word ptr [di+14D0h],bx
        mov     word ptr [di+14D2h],cx
        mov     word ptr [di+14D4h],bp
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_defdelta endp

; seg004_0849_2342  (+2342)
_do_addres proc near
        lodsw
        mov     bp,ax
        lodsw
        mov     di,ax
        mov     bx,word ptr ds:[bp+14D0h]
        add     bx,word ptr [di+14D0h]
        mov     cx,word ptr ds:[bp+14D2h]
        add     cx,word ptr [di+14D2h]
        mov     bp,word ptr ds:[bp+14D4h]
        add     bp,word ptr [di+14D4h]
        lodsw
        mov     di,ax
        call    _code_pnt
        mov     word ptr [di+14D0h],bx
        mov     word ptr [di+14D2h],cx
        mov     word ptr [di+14D4h],bp
        mov     byte ptr [di+14D6h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_addres endp

; seg004_0849_237F  (+237F)
_do_subres proc near
        lodsw
        mov     bp,ax
        lodsw
        mov     di,ax
        mov     bx,word ptr ds:[bp+14D0h]
        sub     bx,word ptr [di+14D0h]
        mov     cx,word ptr ds:[bp+14D2h]
        sub     cx,word ptr [di+14D2h]
        mov     bp,word ptr ds:[bp+14D4h]
        sub     bp,word ptr [di+14D4h]
        lodsw
        mov     di,ax
        call    _code_pnt
        mov     word ptr [di+14D0h],bx
        mov     word ptr [di+14D2h],cx
        mov     word ptr [di+14D4h],bp
        mov     byte ptr [di+14D6h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_subres endp

; seg004_0849_23BC  (+23BC)
_do_ihcall_ind proc near
        lodsw
        mov     bx,ax
        mov     bx,word ptr [bx]
        jmp     short L23C6
_do_ihcall_ind endp

; seg004_0849_23C3  (+23C3)
_do_ihcall proc near
        lodsw
        mov     bx,ax
L23C6:
        push    word ptr ds:[14B2h]
        push    word ptr ds:[14B8h]
        push    word ptr ds:[14BEh]
        push    word ptr ds:[14B4h]
        push    word ptr ds:[14BAh]
        push    word ptr ds:[14C0h]
        push    word ptr ds:[14B6h]
        push    word ptr ds:[14BCh]
        push    word ptr ds:[14C2h]
        push    word ptr ds:[25E6h]
        push    word ptr ds:[25E8h]
        push    word ptr ds:[25EAh]
        call    _instance_head
        push    si
        call    _check_flat
        pop     si
        call    _self_modify
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     si
        pop     word ptr ds:[25EAh]
        pop     word ptr ds:[25E8h]
        pop     word ptr ds:[25E6h]
        pop     word ptr ds:[14C2h]
        pop     word ptr ds:[14BCh]
        pop     word ptr ds:[14B6h]
        pop     word ptr ds:[14C0h]
        pop     word ptr ds:[14BAh]
        pop     word ptr ds:[14B4h]
        pop     word ptr ds:[14BEh]
        pop     word ptr ds:[14B8h]
        pop     word ptr ds:[14B2h]
        call    _self_modify
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_ihcall endp

; seg004_0849_2445  (+2445)
_do_ibcall_ind proc near
        lodsw
        mov     bx,ax
        mov     bx,word ptr [bx]
        jmp     short L244F
_do_ibcall_ind endp

; seg004_0849_244C  (+244C)
_do_ibcall proc near
        lodsw
        mov     bx,ax
L244F:
        push    word ptr ds:[14B2h]
        push    word ptr ds:[14B8h]
        push    word ptr ds:[14BEh]
        push    word ptr ds:[14B4h]
        push    word ptr ds:[14BAh]
        push    word ptr ds:[14C0h]
        push    word ptr ds:[14B6h]
        push    word ptr ds:[14BCh]
        push    word ptr ds:[14C2h]
        push    word ptr ds:[25E6h]
        push    word ptr ds:[25E8h]
        push    word ptr ds:[25EAh]
        call    _instance_bank
        push    si
        call    _check_flat
        pop     si
        call    _self_modify
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     si
        pop     word ptr ds:[25EAh]
        pop     word ptr ds:[25E8h]
        pop     word ptr ds:[25E6h]
        pop     word ptr ds:[14C2h]
        pop     word ptr ds:[14BCh]
        pop     word ptr ds:[14B6h]
        pop     word ptr ds:[14C0h]
        pop     word ptr ds:[14BAh]
        pop     word ptr ds:[14B4h]
        pop     word ptr ds:[14BEh]
        pop     word ptr ds:[14B8h]
        pop     word ptr ds:[14B2h]
        call    _self_modify
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_ibcall endp

; seg004_0849_24CE  (+24CE)
_do_ipcall_ind proc near
        lodsw
        mov     bx,ax
        mov     bx,word ptr [bx]
        jmp     short L24D8
_do_ipcall_ind endp

; seg004_0849_24D5  (+24D5)
_do_ipcall proc near
        lodsw
        mov     bx,ax
L24D8:
        push    word ptr ds:[14B2h]
        push    word ptr ds:[14B8h]
        push    word ptr ds:[14BEh]
        push    word ptr ds:[14B4h]
        push    word ptr ds:[14BAh]
        push    word ptr ds:[14C0h]
        push    word ptr ds:[14B6h]
        push    word ptr ds:[14BCh]
        push    word ptr ds:[14C2h]
        push    word ptr ds:[25E6h]
        push    word ptr ds:[25E8h]
        push    word ptr ds:[25EAh]
        call    _instance_pitch
        push    si
        call    _check_flat
        pop     si
        call    _self_modify
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        pop     si
        pop     word ptr ds:[25EAh]
        pop     word ptr ds:[25E8h]
        pop     word ptr ds:[25E6h]
        pop     word ptr ds:[14C2h]
        pop     word ptr ds:[14BCh]
        pop     word ptr ds:[14B6h]
        pop     word ptr ds:[14C0h]
        pop     word ptr ds:[14BAh]
        pop     word ptr ds:[14B4h]
        pop     word ptr ds:[14BEh]
        pop     word ptr ds:[14B8h]
        pop     word ptr ds:[14B2h]
        call    _self_modify
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_ipcall endp

; seg004_0849_2557  (+2557)
_do_pushc proc near
        push    word ptr ds:[25E6h]
        push    word ptr ds:[25E8h]
        push    word ptr ds:[25EAh]
        push    word ptr ds:[25E0h]
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_pushc endp

; seg004_0849_256D  (+256D)
_do_popc proc near
        pop     word ptr ds:[25E0h]
        pop     word ptr ds:[25EAh]
        pop     word ptr ds:[25E8h]
        pop     word ptr ds:[25E6h]
        call    _self_modify
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_popc endp

; seg004_0849_2586  (+2586)
_do_jnorm_x0 proc near
        lodsw
        mov     bx,ax
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[25E8h]
        imul    di
        mov     bp,ax
        mov     cx,dx
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[25EAh]
        imul    di
        add     bp,ax
        adc     cx,dx
        jns     short L25A9
        add     si,bx
L25A9:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_jnorm_x0 endp

; seg004_0849_25AF  (+25AF)
_do_jnorm_y0 proc near
        lodsw
        mov     bx,ax
        lodsw
        mov     cx,ax
        lodsw
        add     ax,word ptr ds:[25E6h]
        imul    cx
        mov     cx,dx
        mov     bp,ax
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[25EAh]
        imul    di
        add     bp,ax
        adc     cx,dx
        jns     short L25D2
        add     si,bx
L25D2:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_jnorm_y0 endp

; seg004_0849_25D8  (+25D8)
_do_jnorm_z0 proc near
        lodsw
        mov     bx,ax
        lodsw
        mov     cx,ax
        lodsw
        add     ax,word ptr ds:[25E6h]
        imul    cx
        mov     cx,dx
        mov     bp,ax
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[25E8h]
        imul    di
        add     bp,ax
        adc     cx,dx
        jns     short L25FB
        add     si,bx
L25FB:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_jnorm_z0 endp

; seg004_0849_2601  (+2601)
_do_jnorm_yz0 proc near
        lodsw
        mov     bx,ax
        lodsw
        mov     cx,ax
        lodsw
        add     ax,word ptr ds:[25E6h]
        xor     ax,cx
        jns     short L2612
        add     si,bx
L2612:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_jnorm_yz0 endp

; seg004_0849_2618  (+2618)
_do_jnorm_xz0 proc near
        lodsw
        mov     bx,ax
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[25E8h]
        xor     ax,di
        jns     short L2629
        add     si,bx
L2629:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_jnorm_xz0 endp

; seg004_0849_262F  (+262F)
_do_jnorm_xy0 proc near
        lodsw
        mov     bx,ax
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[25EAh]
        xor     ax,di
        jns     short L2640
        add     si,bx
L2640:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_jnorm_xy0 endp

; seg004_0849_2646  (+2646)
_do_jnorm proc near
        lodsw
        mov     bx,ax
        lodsw
        mov     cx,ax
        lodsw
        add     ax,word ptr ds:[25E6h]
        imul    cx
        mov     cx,dx
        mov     bp,ax
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[25E8h]
        imul    di
        add     bp,ax
        adc     cx,dx
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[25EAh]
        imul    di
        add     bp,ax
        adc     cx,dx
        jns     short L2677
        add     si,bx
L2677:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_jnorm endp

; seg004_0849_267D  (+267D)
_clip_overflow proc near
        sti
        add     sp,6
_clip_overflow endp

; seg004_0849_2681  (+2681)
_do_3d_clip proc near
        mov     word ptr ss:[5A5h],offset _overflow_handler_reg
        mov     ax,ds
        mov     es,ax
        test    byte ptr ds:[14CAh],2
        je      short L269D
        call    _clip_right
        test    byte ptr ds:[14CBh],0FFh
        jne     short L26D3
L269D:
        test    byte ptr ds:[14CAh],1
        je      short L26AE
        call    _clip_left
        test    byte ptr ds:[14CBh],0FFh
        jne     short L26D3
L26AE:
        test    byte ptr ds:[14CAh],4
        je      short L26BF
        call    _clip_top
        test    byte ptr ds:[14CBh],0FFh
        jne     short L26D3
L26BF:
        test    byte ptr ds:[14CAh],8
        je      short L26D0
        call    _clip_bot
        test    byte ptr ds:[14CBh],0FFh
        jne     short L26D3
L26D0:
        jmp     L17F0
L26D3:
        pop     si
        mov     ax,seg seg052_519C
        mov     ds,ax
        mov     es,ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_3d_clip endp

; seg004_0849_26E1  (+26E1)
_clip_top proc near
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
        je      short L270D
        mov     di,1BAh
L270D:
        mov     word ptr ds:[1B8h],di
L2711:
        add     si,8
L2714:
        cmp     si,word ptr ds:[1B6h]
        jne     short L271D
        jmp     L27E5
L271D:
        mov     al,byte ptr [si+6]
        test    al,4
        jne     short L2733
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        mov     cx,4
        rep movsw
        jmp     L2714
L2733:
        test    byte ptr [si-2],4
        jne     short L278B
        mov     bp,word ptr [si-4]
        sub     bp,word ptr [si-6]
        mov     cx,bp
        sub     cx,word ptr [si+4]
        add     cx,word ptr [si+2]
        mov     ax,word ptr [si]
        sub     ax,word ptr [si-8]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L275B
        cwd
        adc     ax,dx
L275B:
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
        jae     short L2776
        cwd
        adc     ax,dx
L2776:
        add     ax,word ptr [si-6]
        stosw
        stosw
        mov     cx,ax
        mov     bp,ax
        call    _code_pnt
        stosw
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
L278B:
        test    byte ptr [si+0Eh],4
        jne     L2711
        mov     bp,word ptr [si+4]
        sub     bp,word ptr [si+2]
        mov     cx,bp
        add     cx,word ptr [si+0Ah]
        sub     cx,word ptr [si+0Ch]
        mov     ax,word ptr [si+8]
        sub     ax,word ptr [si]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L27B3
        cwd
        adc     ax,dx
L27B3:
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
        jae     short L27CD
        cwd
        adc     ax,dx
L27CD:
        add     ax,word ptr [si+2]
        stosw
        stosw
        mov     cx,ax
        mov     bp,ax
        call    _code_pnt
        stosw
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        jmp     L2711
L27E5:
        mov     word ptr ds:[1B6h],di
        ret
_clip_top endp

; seg004_0849_27EA  (+27EA)
_clip_bot proc near
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
        je      short L2816
        mov     di,1BAh
L2816:
        mov     word ptr ds:[1B8h],di
L281A:
        add     si,8
L281D:
        cmp     si,word ptr ds:[1B6h]
        jne     short L2826
        jmp     L28F5
L2826:
        mov     al,byte ptr [si+6]
        test    al,8
        jne     short L283C
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        mov     cx,4
        rep movsw
        jmp     L281D
L283C:
        test    byte ptr [si-2],8
        jne     short L2896
        mov     bp,word ptr [si-4]
        add     bp,word ptr [si-6]
        mov     cx,bp
        sub     cx,word ptr [si+4]
        sub     cx,word ptr [si+2]
        mov     ax,word ptr [si]
        sub     ax,word ptr [si-8]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L2864
        cwd
        adc     ax,dx
L2864:
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
        jae     short L287F
        cwd
        adc     ax,dx
L287F:
        add     ax,word ptr [si-6]
        stosw
        mov     cx,ax
        neg     ax
        stosw
        mov     bp,ax
        call    _code_pnt
        stosw
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
L2896:
        test    byte ptr [si+0Eh],8
        je      short L289F
        jmp     L281A
L289F:
        mov     bp,word ptr [si+4]
        add     bp,word ptr [si+2]
        mov     cx,bp
        sub     cx,word ptr [si+0Ah]
        sub     cx,word ptr [si+0Ch]
        mov     ax,word ptr [si+8]
        sub     ax,word ptr [si]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L28C1
        cwd
        adc     ax,dx
L28C1:
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
        jae     short L28DB
        cwd
        adc     ax,dx
L28DB:
        add     ax,word ptr [si+2]
        stosw
        mov     cx,ax
        neg     ax
        stosw
        mov     bp,ax
        call    _code_pnt
        stosw
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        jmp     L281A
L28F5:
        mov     word ptr ds:[1B6h],di
        ret
_clip_bot endp

; seg004_0849_28FA  (+28FA)
_clip_right proc near
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
        je      short L2926
        mov     di,1BAh
L2926:
        mov     word ptr ds:[1B8h],di
L292A:
        add     si,8
L292D:
        cmp     si,word ptr ds:[1B6h]
        jne     short L2936
        jmp     L2A03
L2936:
        mov     al,byte ptr [si+6]
        test    al,2
        jne     short L294C
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        mov     cx,4
        rep movsw
        jmp     L292D
L294C:
        test    byte ptr [si-2],2
        jne     short L29A5
        mov     bp,word ptr [si-8]
        sub     bp,word ptr [si-4]
        mov     cx,bp
        add     cx,word ptr [si+4]
        sub     cx,word ptr [si]
        mov     ax,word ptr [si]
        sub     ax,word ptr [si-8]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L2973
        cwd
        adc     ax,dx
L2973:
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
        jae     short L298E
        cwd
        adc     ax,dx
L298E:
        add     ax,word ptr [si-6]
        stosw
        mov     cx,ax
        mov     ax,bx
        stosw
        mov     bp,ax
        call    _code_pnt
        stosw
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
L29A5:
        test    byte ptr [si+0Eh],2
        je      short L29AE
        jmp     L292A
L29AE:
        mov     bp,word ptr [si]
        sub     bp,word ptr [si+4]
        mov     cx,bp
        add     cx,word ptr [si+0Ch]
        sub     cx,word ptr [si+8]
        mov     ax,word ptr [si+8]
        sub     ax,word ptr [si]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L29CF
        cwd
        adc     ax,dx
L29CF:
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
        jae     short L29E9
        cwd
        adc     ax,dx
L29E9:
        add     ax,word ptr [si+2]
        stosw
        mov     cx,ax
        mov     ax,bx
        stosw
        mov     bp,ax
        call    _code_pnt
        stosw
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        jmp     L292A
L2A03:
        mov     word ptr ds:[1B6h],di
        ret
_clip_right endp

; seg004_0849_2A08  (+2A08)
_clip_left proc near
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
        je      short L2A34
        mov     di,1BAh
L2A34:
        mov     word ptr ds:[1B8h],di
L2A38:
        add     si,8
L2A3B:
        cmp     si,word ptr ds:[1B6h]
        jne     short L2A44
        jmp     L2B15
L2A44:
        mov     al,byte ptr [si+6]
        test    al,1
        jne     short L2A5A
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        mov     cx,4
        rep movsw
        jmp     L2A3B
L2A5A:
        test    byte ptr [si-2],1
        jne     short L2AB5
        mov     bp,word ptr [si-4]
        add     bp,word ptr [si-8]
        mov     cx,bp
        sub     cx,word ptr [si+4]
        sub     cx,word ptr [si]
        mov     ax,word ptr [si]
        sub     ax,word ptr [si-8]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L2A81
        cwd
        adc     ax,dx
L2A81:
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
        jae     short L2A9C
        cwd
        adc     ax,dx
L2A9C:
        add     ax,word ptr [si-6]
        stosw
        mov     cx,ax
        mov     ax,bx
        neg     ax
        stosw
        mov     bp,ax
        call    _code_pnt
        stosw
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
L2AB5:
        test    byte ptr [si+0Eh],1
        je      short L2ABE
        jmp     L2A38
L2ABE:
        mov     bp,word ptr [si+4]
        add     bp,word ptr [si]
        mov     cx,bp
        sub     cx,word ptr [si+8]
        sub     cx,word ptr [si+0Ch]
        mov     ax,word ptr [si+8]
        sub     ax,word ptr [si]
        imul    bp
        shl     ax,1
        rcl     dx,1
        idiv    cx
        sar     ax,1
        jae     short L2ADF
        cwd
        adc     ax,dx
L2ADF:
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
        jae     short L2AF9
        cwd
        adc     ax,dx
L2AF9:
        add     ax,word ptr [si+2]
        stosw
        mov     cx,ax
        mov     ax,bx
        neg     ax
        stosw
        mov     bp,ax
        call    _code_pnt
        stosw
        or      byte ptr ds:[14CAh],al
        and     byte ptr ds:[14CBh],al
        jmp     L2A38
L2B15:
        mov     word ptr ds:[1B6h],di
        ret
_clip_left endp

; seg004_0849_2B1A  (+2B1A)
_do_parapiped proc near
        lodsw
        mov     di,ax
        mov     cl,byte ptr ds:[25E0h]
        lodsw
        shl     ax,cl
        mov     bx,ax
        imul    word ptr ds:[14B2h]
        mov     word ptr ds:[148Ah],dx
        mov     ax,bx
        imul    word ptr ds:[14B4h]
        mov     word ptr ds:[148Ch],dx
        mov     ax,bx
        imul    word ptr ds:[14B6h]
        mov     word ptr ds:[148Eh],dx
        lodsw
        shl     ax,cl
        mov     bx,ax
        imul    word ptr ds:[14B8h]
        mov     word ptr ds:[1490h],dx
        mov     ax,bx
        imul    word ptr ds:[14BAh]
        mov     word ptr ds:[1492h],dx
        mov     ax,bx
        imul    word ptr ds:[14BCh]
        mov     word ptr ds:[1494h],dx
        lodsw
        shl     ax,cl
        mov     bx,ax
        imul    word ptr ds:[14BEh]
        mov     word ptr ds:[1496h],dx
        mov     ax,bx
        imul    word ptr ds:[14C0h]
        mov     word ptr ds:[1498h],dx
        mov     ax,bx
        imul    word ptr ds:[14C2h]
        mov     word ptr ds:[149Ah],dx
        mov     al,byte ptr [di+14D6h]
        or      al,al
        je      short L2B8F
        jmp     L2E21
L2B8F:
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        add     bx,word ptr ds:[148Ah]
        add     cx,word ptr ds:[148Ch]
        add     bp,word ptr ds:[148Eh]
        add     bx,word ptr ds:[1490h]
        add     cx,word ptr ds:[1492h]
        add     bp,word ptr ds:[1494h]
        add     bx,word ptr ds:[1496h]
        add     cx,word ptr ds:[1498h]
        add     bp,word ptr ds:[149Ah]
        jns     short L2BC4
        jmp     L2E18
L2BC4:
        cmp     bx,bp
        jle     short L2BCB
        jmp     L2E18
L2BCB:
        cmp     cx,bp
        jle     short L2BD2
        jmp     L2E18
L2BD2:
        neg     bp
        cmp     bx,bp
        jge     short L2BDB
        jmp     L2E18
L2BDB:
        cmp     cx,bp
        jge     short L2BE2
        jmp     L2E18
L2BE2:
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        sub     bx,word ptr ds:[148Ah]
        sub     cx,word ptr ds:[148Ch]
        sub     bp,word ptr ds:[148Eh]
        add     bx,word ptr ds:[1490h]
        add     cx,word ptr ds:[1492h]
        add     bp,word ptr ds:[1494h]
        add     bx,word ptr ds:[1496h]
        add     cx,word ptr ds:[1498h]
        add     bp,word ptr ds:[149Ah]
        jns     short L2C17
        jmp     L2E18
L2C17:
        cmp     bx,bp
        jle     short L2C1E
        jmp     L2E18
L2C1E:
        cmp     cx,bp
        jle     short L2C25
        jmp     L2E18
L2C25:
        neg     bp
        cmp     bx,bp
        jge     short L2C2E
        jmp     L2E18
L2C2E:
        cmp     cx,bp
        jge     short L2C35
        jmp     L2E18
L2C35:
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        add     bx,word ptr ds:[148Ah]
        add     cx,word ptr ds:[148Ch]
        add     bp,word ptr ds:[148Eh]
        sub     bx,word ptr ds:[1490h]
        sub     cx,word ptr ds:[1492h]
        sub     bp,word ptr ds:[1494h]
        add     bx,word ptr ds:[1496h]
        add     cx,word ptr ds:[1498h]
        add     bp,word ptr ds:[149Ah]
        jns     short L2C6A
        jmp     L2E18
L2C6A:
        cmp     bx,bp
        jle     short L2C71
        jmp     L2E18
L2C71:
        cmp     cx,bp
        jle     short L2C78
        jmp     L2E18
L2C78:
        neg     bp
        cmp     bx,bp
        jge     short L2C81
        jmp     L2E18
L2C81:
        cmp     cx,bp
        jge     short L2C88
        jmp     L2E18
L2C88:
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        sub     bx,word ptr ds:[148Ah]
        sub     cx,word ptr ds:[148Ch]
        sub     bp,word ptr ds:[148Eh]
        sub     bx,word ptr ds:[1490h]
        sub     cx,word ptr ds:[1492h]
        sub     bp,word ptr ds:[1494h]
        add     bx,word ptr ds:[1496h]
        add     cx,word ptr ds:[1498h]
        add     bp,word ptr ds:[149Ah]
        jns     short L2CBD
        jmp     L2E18
L2CBD:
        cmp     bx,bp
        jle     short L2CC4
        jmp     L2E18
L2CC4:
        cmp     cx,bp
        jle     short L2CCB
        jmp     L2E18
L2CCB:
        neg     bp
        cmp     bx,bp
        jge     short L2CD4
        jmp     L2E18
L2CD4:
        cmp     cx,bp
        jge     short L2CDB
        jmp     L2E18
L2CDB:
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        add     bx,word ptr ds:[148Ah]
        add     cx,word ptr ds:[148Ch]
        add     bp,word ptr ds:[148Eh]
        add     bx,word ptr ds:[1490h]
        add     cx,word ptr ds:[1492h]
        add     bp,word ptr ds:[1494h]
        sub     bx,word ptr ds:[1496h]
        sub     cx,word ptr ds:[1498h]
        sub     bp,word ptr ds:[149Ah]
        jns     short L2D10
        jmp     L2E18
L2D10:
        cmp     bx,bp
        jle     short L2D17
        jmp     L2E18
L2D17:
        cmp     cx,bp
        jle     short L2D1E
        jmp     L2E18
L2D1E:
        neg     bp
        cmp     bx,bp
        jge     short L2D27
        jmp     L2E18
L2D27:
        cmp     cx,bp
        jge     short L2D2E
        jmp     L2E18
L2D2E:
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        sub     bx,word ptr ds:[148Ah]
        sub     cx,word ptr ds:[148Ch]
        sub     bp,word ptr ds:[148Eh]
        add     bx,word ptr ds:[1490h]
        add     cx,word ptr ds:[1492h]
        add     bp,word ptr ds:[1494h]
        sub     bx,word ptr ds:[1496h]
        sub     cx,word ptr ds:[1498h]
        sub     bp,word ptr ds:[149Ah]
        jns     short L2D63
        jmp     L2E18
L2D63:
        cmp     bx,bp
        jle     short L2D6A
        jmp     L2E18
L2D6A:
        cmp     cx,bp
        jle     short L2D71
        jmp     L2E18
L2D71:
        neg     bp
        cmp     bx,bp
        jge     short L2D7A
        jmp     L2E18
L2D7A:
        cmp     cx,bp
        jge     short L2D81
        jmp     L2E18
L2D81:
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        add     bx,word ptr ds:[148Ah]
        add     cx,word ptr ds:[148Ch]
        add     bp,word ptr ds:[148Eh]
        sub     bx,word ptr ds:[1490h]
        sub     cx,word ptr ds:[1492h]
        sub     bp,word ptr ds:[1494h]
        sub     bx,word ptr ds:[1496h]
        sub     cx,word ptr ds:[1498h]
        sub     bp,word ptr ds:[149Ah]
        js      short L2E18
        cmp     bx,bp
        jg      short L2E18
        cmp     cx,bp
        jg      short L2E18
        neg     bp
        cmp     bx,bp
        jl      short L2E18
        cmp     cx,bp
        jl      short L2E18
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        sub     bx,word ptr ds:[148Ah]
        sub     cx,word ptr ds:[148Ch]
        sub     bp,word ptr ds:[148Eh]
        sub     bx,word ptr ds:[1490h]
        sub     cx,word ptr ds:[1492h]
        sub     bp,word ptr ds:[1494h]
        sub     bx,word ptr ds:[1496h]
        sub     cx,word ptr ds:[1498h]
        sub     bp,word ptr ds:[149Ah]
        js      short L2E18
        cmp     bx,bp
        jg      short L2E18
        cmp     cx,bp
        jg      short L2E18
        neg     bp
        cmp     bx,bp
        jl      short L2E18
        cmp     cx,bp
        jl      short L2E18
        mov     ax,0FFFFh
        call    _set_accept
        add     si,2
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
L2E18:
        add     si,2
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
L2E21:
        mov     dl,al
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        add     bx,word ptr ds:[148Ah]
        add     cx,word ptr ds:[148Ch]
        add     bp,word ptr ds:[148Eh]
        add     bx,word ptr ds:[1490h]
        add     cx,word ptr ds:[1492h]
        add     bp,word ptr ds:[1494h]
        add     bx,word ptr ds:[1496h]
        add     cx,word ptr ds:[1498h]
        add     bp,word ptr ds:[149Ah]
        call    _code_pnt
        and     dl,al
        je      L2E18
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        sub     bx,word ptr ds:[148Ah]
        sub     cx,word ptr ds:[148Ch]
        sub     bp,word ptr ds:[148Eh]
        add     bx,word ptr ds:[1490h]
        add     cx,word ptr ds:[1492h]
        add     bp,word ptr ds:[1494h]
        add     bx,word ptr ds:[1496h]
        add     cx,word ptr ds:[1498h]
        add     bp,word ptr ds:[149Ah]
        call    _code_pnt
        and     dl,al
        je      L2E18
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        add     bx,word ptr ds:[148Ah]
        add     cx,word ptr ds:[148Ch]
        add     bp,word ptr ds:[148Eh]
        sub     bx,word ptr ds:[1490h]
        sub     cx,word ptr ds:[1492h]
        sub     bp,word ptr ds:[1494h]
        add     bx,word ptr ds:[1496h]
        add     cx,word ptr ds:[1498h]
        add     bp,word ptr ds:[149Ah]
        call    _code_pnt
        and     dl,al
        jne     short L2ECB
        jmp     L2E18
L2ECB:
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        sub     bx,word ptr ds:[148Ah]
        sub     cx,word ptr ds:[148Ch]
        sub     bp,word ptr ds:[148Eh]
        sub     bx,word ptr ds:[1490h]
        sub     cx,word ptr ds:[1492h]
        sub     bp,word ptr ds:[1494h]
        add     bx,word ptr ds:[1496h]
        add     cx,word ptr ds:[1498h]
        add     bp,word ptr ds:[149Ah]
        call    _code_pnt
        and     dl,al
        jne     short L2F05
        jmp     L2E18
L2F05:
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        add     bx,word ptr ds:[148Ah]
        add     cx,word ptr ds:[148Ch]
        add     bp,word ptr ds:[148Eh]
        add     bx,word ptr ds:[1490h]
        add     cx,word ptr ds:[1492h]
        add     bp,word ptr ds:[1494h]
        sub     bx,word ptr ds:[1496h]
        sub     cx,word ptr ds:[1498h]
        sub     bp,word ptr ds:[149Ah]
        call    _code_pnt
        and     dl,al
        jne     short L2F3F
        jmp     L2E18
L2F3F:
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        sub     bx,word ptr ds:[148Ah]
        sub     cx,word ptr ds:[148Ch]
        sub     bp,word ptr ds:[148Eh]
        add     bx,word ptr ds:[1490h]
        add     cx,word ptr ds:[1492h]
        add     bp,word ptr ds:[1494h]
        sub     bx,word ptr ds:[1496h]
        sub     cx,word ptr ds:[1498h]
        sub     bp,word ptr ds:[149Ah]
        call    _code_pnt
        and     dl,al
        jne     short L2F79
        jmp     L2E18
L2F79:
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        add     bx,word ptr ds:[148Ah]
        add     cx,word ptr ds:[148Ch]
        add     bp,word ptr ds:[148Eh]
        sub     bx,word ptr ds:[1490h]
        sub     cx,word ptr ds:[1492h]
        sub     bp,word ptr ds:[1494h]
        sub     bx,word ptr ds:[1496h]
        sub     cx,word ptr ds:[1498h]
        sub     bp,word ptr ds:[149Ah]
        call    _code_pnt
        and     dl,al
        jne     short L2FB3
        jmp     L2E18
L2FB3:
        mov     bx,word ptr [di+14D0h]
        mov     cx,word ptr [di+14D2h]
        mov     bp,word ptr [di+14D4h]
        sub     bx,word ptr ds:[148Ah]
        sub     cx,word ptr ds:[148Ch]
        sub     bp,word ptr ds:[148Eh]
        sub     bx,word ptr ds:[1490h]
        sub     cx,word ptr ds:[1492h]
        sub     bp,word ptr ds:[1494h]
        sub     bx,word ptr ds:[1496h]
        sub     cx,word ptr ds:[1498h]
        sub     bp,word ptr ds:[149Ah]
        call    _code_pnt
        and     dl,al
        jne     short L2FED
        jmp     L2E18
L2FED:
        lodsw
        or      ax,ax
        je      short L2FFA
        add     si,ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
L2FFA:
        ret
        db      5 dup (0)
_do_parapiped endp

; seg004_0849_3000  (+3000)
_seg004_0849_3000 proc near
        int     3
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_seg004_0849_3000 endp

; render_3d  (+3007): FM render_3d
_render_3d proc far
        mov     ax,seg seg052_519C
        mov     ds,ax
        mov     es,ax
        mov     ax,word ptr ss:[0C34h]
        sub     ax,word ptr ss:[0C30h]
        inc     ax
        shr     ax,1
        mov     word ptr ds:[2472h],ax
        mov     ax,word ptr ss:[0C32h]
        sub     ax,word ptr ss:[0C36h]
        inc     ax
        shr     ax,1
        mov     word ptr ds:[2470h],ax
        mov     ax,word ptr ss:[0C34h]
        add     ax,word ptr ss:[0C30h]
        inc     ax
        shr     ax,1
        mov     word ptr ds:[2474h],ax
        mov     word ptr cs:[L0644+1],ax
        mov     word ptr cs:[L06B2+1],ax
        mov     word ptr cs:[modify_biasx+1],ax
        mov     word ptr cs:[modify_bx2+1],ax
        mov     ax,word ptr ss:[0C32h]
        add     ax,word ptr ss:[0C36h]
        shr     ax,1
        mov     word ptr ds:[2476h],ax
        mov     word ptr cs:[L064F+1],ax
        mov     word ptr cs:[L06C2+1],ax
        mov     word ptr cs:[modify_biasy+1],ax
        mov     word ptr cs:[modify_by2+1],ax
        inc     word ptr ds:[14AAh]
        mov     byte ptr ds:[25E0h],0
        call    _create_matrix
        call    _seg004_0849_35FF
        call    _scale_matrix
        call    _check_flat
        mov     ax,ss
        mov     word ptr ds:[14A2h],ax
        mov     word ptr ds:[14A0h],sp
        mov     si,seg seg_370D
        mov     ds,si
        mov     ax,word ptr ds:[2CEEh]
        mov     word ptr cs:L30DA,ax
        mov     word ptr ds:[2CEEh],9C9h
        mov     ax,seg dseg062_62a6
        mov     es,ax
        mov     es,si
        cli
        mov     ss,si
        mov     sp,5548h
        sti
        call    far ptr _seg021_22FD_C10
        mov     bp,seg seg052_519C
        mov     ds,bp
        mov     es,bp
        cli
        mov     ss,word ptr ds:[14A2h]
        mov     sp,word ptr ds:[14A0h]
        sti
        mov     si,word ptr ds:[14CCh]
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        mov     ax,seg seg_370D
        mov     ds,ax
        mov     ax,word ptr cs:L30DA
        mov     word ptr ds:[2CEEh],ax
        mov     ax,ss
        mov     ds,ax
        mov     es,ax
        retf
L30DA   dw      0                       ; the seg_370D word at 2CEEh, saved over the render
_render_3d endp

; seg004_0849_30DC  (+30DC)
_scale_matrix proc near
        mov     word ptr ds:[24E8h],7FFFh
        mov     word ptr ds:[24EAh],7FFFh
        mov     ax,word ptr ds:[14B8h]
        mov     word ptr ds:[14C4h],ax
        mov     ax,word ptr ds:[14BAh]
        mov     word ptr ds:[14C6h],ax
        mov     ax,word ptr ds:[14BCh]
        mov     word ptr ds:[14C8h],ax
        mov     bx,word ptr ds:[2470h]
        mov     cx,word ptr ds:[2472h]
        mov     ax,word ptr ds:[14A4h]
        or      ax,ax
        js      short L3113
        imul    cx
        shl     ax,1
        rcl     dx,1
        xor     cx,cx
        jmp     short L3123
L3113:
        neg     ax
        imul    bx
        shl     ax,1
        rcl     dx,1
        mov     bx,dx
        mov     dx,cx
        mov     cx,ax
        xor     ax,ax
L3123:
        cmp     dx,bx
        jne     short L312A
        jmp     L31CB
L312A:
        jl      short L317E
        xchg    dx,bx
        xchg    cx,ax
        shr     dx,1
        rcr     ax,1
        idiv    bx
        mov     cx,ax
        mov     word ptr ds:[24E8h],cx
        mov     ax,word ptr ds:[14B2h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14B2h],dx
        mov     ax,word ptr ds:[14B8h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14B8h],dx
        mov     ax,word ptr ds:[14BEh]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14BEh],dx
        mov     ax,word ptr ds:[14C6h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C6h],dx
        mov     ax,word ptr ds:[14C8h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C8h],dx
        jmp     short L31CB
L317E:
        shr     dx,1
        rcr     ax,1
        idiv    bx
        mov     cx,ax
        mov     word ptr ds:[24EAh],cx
        mov     ax,word ptr ds:[14B4h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14B4h],dx
        mov     ax,word ptr ds:[14BAh]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14BAh],dx
        mov     ax,word ptr ds:[14C0h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C0h],dx
        mov     ax,word ptr ds:[14C4h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C4h],dx
        mov     ax,word ptr ds:[14C8h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C8h],dx
L31CB:
        mov     cx,word ptr ds:[24E6h]
        or      cx,cx
        jns     short L31D6
        jmp     L325E
L31D6:
        mov     word ptr ds:[24F0h],cx
        mov     ax,word ptr ds:[14B6h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14B6h],dx
        mov     ax,word ptr ds:[14BCh]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14BCh],dx
        mov     ax,word ptr ds:[14C2h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C2h],dx
        mov     ax,word ptr ds:[14C6h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C6h],dx
        mov     ax,word ptr ds:[14C4h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C4h],dx
        mov     ax,cx
        imul    ax
        mov     bp,ax
        mov     si,dx
        mov     ax,word ptr ds:[24E8h]
        mov     word ptr ds:[24ECh],ax
        imul    ax
        add     ax,bp
        adc     dx,si
        mov     bx,dx
        xor     cx,cx
        call    far ptr _SquareRoot_seg021_22FD_A30
        mov     ax,di
        xchg    ah,al
        mov     word ptr ds:[24E8h],ax
        mov     ax,word ptr ds:[24EAh]
        mov     word ptr ds:[24EEh],ax
        imul    ax
        add     ax,bp
        adc     dx,si
        mov     bx,dx
        xor     cx,cx
        call    far ptr _SquareRoot_seg021_22FD_A30
        mov     ax,di
        xchg    al,ah
        mov     word ptr ds:[24EAh],ax
        jmp     L331D
L325E:
        mov     word ptr ds:[24F0h],7FFFh
        neg     cx
        mov     ax,word ptr ds:[14B2h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14B2h],dx
        mov     ax,word ptr ds:[14B8h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14B8h],dx
        mov     ax,word ptr ds:[14BEh]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14BEh],dx
        mov     ax,word ptr ds:[14B4h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14B4h],dx
        mov     ax,word ptr ds:[14BAh]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14BAh],dx
        mov     ax,word ptr ds:[14C0h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C0h],dx
        mov     ax,word ptr ds:[14C8h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C8h],dx
        mov     si,cx
        mov     ax,word ptr ds:[24E8h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     bp,dx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[24ECh],dx
        mov     dx,bp
        mov     ax,dx
        imul    ax
        mov     bx,dx
        add     bx,3FFFh
        xor     cx,cx
        call    far ptr _SquareRoot_seg021_22FD_A30
        mov     ax,di
        xchg    ah,al
        mov     word ptr ds:[24E8h],ax
        mov     ax,word ptr ds:[24EAh]
        imul    si
        shl     ax,1
        rcl     dx,1
        mov     bp,dx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[24EEh],dx
        mov     dx,bp
        mov     ax,dx
        imul    ax
        mov     bx,dx
        add     bx,3FFFh
        xor     cx,cx
        call    far ptr _SquareRoot_seg021_22FD_A30
        mov     ax,di
        xchg    al,ah
        mov     word ptr ds:[24EAh],ax
L331D:
        mov     word ptr ss:[5A5h],offset _overflow_handler_reg
        mov     ax,word ptr ds:[14B6h]
        imul    word ptr ds:[14B6h]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[14C2h]
        imul    word ptr ds:[14C2h]
        add     bx,ax
        adc     cx,dx
        or      ch,ch
        je      short L336F
        call    far ptr _SquareRoot_seg021_22FD_A30
        mov     dx,word ptr ds:[14B6h]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L3355
        inc     ax
L3355:
        mov     word ptr ds:[248Ch],ax
        mov     dx,word ptr ds:[14C2h]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L336A
        inc     ax
L336A:
        mov     word ptr ds:[248Eh],ax
        jmp     short L33C3
L336F:
        mov     ax,word ptr ds:[14B4h]
        imul    word ptr ds:[14B4h]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[14C0h]
        imul    word ptr ds:[14C0h]
        add     bx,ax
        adc     cx,dx
        call    far ptr _SquareRoot_seg021_22FD_A30
        mov     dx,word ptr ds:[14B4h]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L339C
        inc     ax
L339C:
        mov     bx,ax
        mov     dx,word ptr ds:[14C0h]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L33B0
        inc     ax
L33B0:
        test    word ptr ds:[14BCh],0FFFFh
        js      short L33BC
        neg     bx
        neg     ax
L33BC:
        mov     word ptr ds:[248Ch],bx
        mov     word ptr ds:[248Eh],ax
L33C3:
        mov     ax,word ptr ds:[248Eh]
        imul    word ptr ds:[14B2h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[248Ch]
        neg     ax
        imul    word ptr ds:[14BEh]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        adc     dx,0
        mov     word ptr ds:[2490h],dx
        mov     ax,word ptr ds:[248Eh]
        imul    word ptr ds:[14B4h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[248Ch]
        neg     ax
        imul    word ptr ds:[14C0h]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        adc     dx,0
        mov     word ptr ds:[2492h],dx
        ret
_scale_matrix endp

; seg004_0849_340E  (+340E)
_check_flat proc near
        test    word ptr ds:[14B8h],0FFFFh
        jne     short L3436
        test    word ptr ds:[14B4h],0FFFFh
        jne     short L3436
        test    word ptr ds:[14C0h],0FFFFh
        jne     short L3436
        test    word ptr ds:[14BCh],0FFFFh
        jne     short L3436
        cmp     word ptr ds:[14BAh],7FFBh
        jge     short is_flat
L3436:
        test    byte ptr ds:[25D4h],0FFh
        je      short L347C
        mov     byte ptr ds:[25D4h],0
        mov     word ptr ds:[257Ah],offset _do_x_rel
        mov     word ptr ds:[257Ch],offset _do_y_rel
        mov     word ptr ds:[257Eh],offset _do_z_rel
        mov     word ptr ds:[2584h],offset _do_xy_rel
        mov     word ptr ds:[2588h],offset _do_yz_rel
        mov     word ptr ds:[2586h],offset _do_xz_rel
        mov     word ptr ds:[2590h],offset _do_rod
        mov     si,offset L3587
        mov     di,offset _mxmul
        push    es
        push    cs
        pop     es
        mov     cx,54h
        rep movs byte ptr es:[di],byte ptr cs:[si]
        pop     es
L347C:
        ret
is_flat:
        test    byte ptr ds:[25D4h],0FFh
        jne     L347C
        mov     byte ptr ds:[25D4h],0FFh
        mov     word ptr ds:[257Ah],offset _flat_x_rel
        mov     word ptr ds:[257Ch],offset _flat_y_rel
        mov     word ptr ds:[257Eh],offset _flat_z_rel
        mov     word ptr ds:[2584h],offset _flat_xy_rel
        mov     word ptr ds:[2588h],offset _flat_yz_rel
        mov     word ptr ds:[2586h],offset _flat_xz_rel
        mov     word ptr ds:[2590h],offset _flat_rod
        mov     si,offset L35DA
        mov     di,offset _mxmul
        push    es
        push    cs
        pop     es
        mov     cx,26h
        rep movs byte ptr es:[di],byte ptr cs:[si]
        pop     es
        ret
_check_flat endp

; seg004_0849_34C4  (+34C4)
_set_accept proc near
        ret
_set_accept endp

; seg004_0849_34C5  (+34C5)
_self_modify proc near
        mov     ax,word ptr ds:[25E6h]
        mov     word ptr cs:[modify_mex+1],ax
        mov     word ptr cs:[L34F9+1],ax
        mov     ax,word ptr ds:[25E8h]
        mov     word ptr cs:[modify_mey+1],ax
        mov     word ptr cs:[L3511+1],ax
        mov     ax,word ptr ds:[25EAh]
        mov     word ptr cs:[modify_mez+1],ax
        mov     word ptr cs:[L3505+1],ax
        mov     al,byte ptr ds:[25E0h]
        mov     byte ptr cs:[modify_mes+1],al
        mov     byte ptr cs:[_mini_xlate_rotate_pnt+1],al
        ret
_self_modify endp

; seg004_0849_34F2  (+34F2)
_mini_xlate_rotate_pnt proc near
        mov     cl,5                    ; the shift and the three origins are patched by self_modify
        lodsb
        cbw
        shl     ax,5
L34F9:
        sub     ax,1234h
        shl     ax,cl
        mov     bx,ax
        lodsb
        cbw
        shl     ax,5
L3505:
        sub     ax,1234h
        shl     ax,cl
        mov     bp,ax
        lodsb
        cbw
        shl     ax,5
L3511:
        sub     ax,1234h
        shl     ax,cl
        mov     cx,ax
        jmp     short _mxmul
_mini_xlate_rotate_pnt endp

; seg004_0849_351A  (+351A)
_load_xlate_rotate_pnt proc near
modify_mes:                             ; FM names for the patched shift and origins
        mov     cl,5
        lodsw
modify_mex:
        sub     ax,1234h
        shl     ax,cl
        mov     bx,ax
        lodsw
modify_mez:
        sub     ax,1234h
        shl     ax,cl
        mov     bp,ax
        lodsw
modify_mey:
        sub     ax,1234h
        shl     ax,cl
        mov     cx,ax
_load_xlate_rotate_pnt endp

; seg004_0849_3534  (+3534)
_mxmul proc near
        mov     ax,bx
        imul    word ptr ds:[14B2h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[14B8h]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14BEh]
        add     di,dx
        mov     word ptr ds:[24C8h],di
        mov     ax,bx
        imul    word ptr ds:[14B4h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[14BAh]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14C0h]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14C2h]
        mov     bp,dx
        mov     ax,bx
        imul    word ptr ds:[14B6h]
        add     bp,dx
        mov     ax,cx
        imul    word ptr ds:[14BCh]
        add     bp,dx
        mov     cx,di
        mov     bx,word ptr ds:[24C8h]
        ret
; L3587 and L35DA: the two bodies check_flat and is_flat copy over the one above
L3587:
        mov     ax,bx
        imul    word ptr ds:[14B2h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[14B8h]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14BEh]
        add     di,dx
        mov     word ptr ds:[24C8h],di
        mov     ax,bx
        imul    word ptr ds:[14B4h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[14BAh]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14C0h]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14C2h]
        mov     bp,dx
        mov     ax,bx
        imul    word ptr ds:[14B6h]
        add     bp,dx
        mov     ax,cx
        imul    word ptr ds:[14BCh]
        add     bp,dx
        mov     cx,di
        mov     bx,word ptr ds:[24C8h]
        ret
L35DA:
        mov     ax,bx
        imul    word ptr ds:[14B2h]
        mov     di,dx
        mov     ax,bp
        imul    word ptr ds:[14BEh]
        add     di,dx
        mov     ax,bx
        mov     bx,di
        imul    word ptr ds:[14B6h]
        mov     ax,bp
        mov     bp,dx
        imul    word ptr ds:[14C2h]
        add     bp,dx
        sar     cx,1
        ret
_mxmul endp

; seg004_0849_35FF  (+35FF)
_seg004_0849_35FF proc near
        mov     ax,word ptr ds:[14BCh]
        cwd
        xor     ax,dx
        sub     ax,dx
        cmp     ax,5A82h
        jle     short L3611
        mov     ax,2
        jmp     short L3633
L3611:
        mov     ax,word ptr ds:[14B6h]
        cwd
        xor     ax,dx
        sub     ax,dx
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[14C2h]
        cwd
        xor     ax,dx
        sub     ax,dx
        cmp     bx,ax
        jg      short L362E
        mov     ax,4
        jmp     short L3633
L362E:
        mov     ax,0
        mov     dx,cx
L3633:
        mov     bl,2Bh
        test    dx,0FFFFh
        jns     short L363D
        mov     bl,3
L363D:
        mov     byte ptr cs:modify_axis2,bl
        add     ax,8
        sub     ax,8
        add     ax,25E6h
        mov     word ptr cs:[modify_axis2+2],ax
        ret
_seg004_0849_35FF endp

; seg004_0849_3650  (+3650)
_instance_vvars_head proc near
        mov     ax,word ptr ds:[25E6h]
        imul    word ptr ds:[24CAh]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[25EAh]
        imul    word ptr ds:[24CEh]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        mov     ax,word ptr ds:[25E6h]
        mov     word ptr ds:[25E6h],cx
        imul    word ptr ds:[24CCh]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[25EAh]
        imul    word ptr ds:[24CAh]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[25EAh],dx
        ret
_instance_vvars_head endp

; seg004_0849_368D  (+368D)
_instance_head proc near
        mov     ax,ss
        mov     ds,ax
        call    far ptr _sincos
        mov     dx,es
        mov     ds,dx
        mov     word ptr ds:[24CCh],ax
        mov     word ptr ds:[24CAh],bx
        neg     ax
        mov     word ptr ds:[24CEh],ax
        call    _instance_vvars_head
        mov     ax,word ptr ds:[24CAh]
        imul    word ptr ds:[14B2h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24CEh]
        imul    word ptr ds:[14BEh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L36CF
        jg      short L36CC
        mov     cx,8000h
        jmp     short L36CF
L36CC:
        mov     cx,7FFFh
L36CF:
        mov     word ptr ds:[2494h],cx
        mov     ax,word ptr ds:[24CAh]
        imul    word ptr ds:[14B4h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24CEh]
        imul    word ptr ds:[14C0h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L36F9
        jg      short L36F6
        mov     cx,8000h
        jmp     short L36F9
L36F6:
        mov     cx,7FFFh
L36F9:
        mov     word ptr ds:[2496h],cx
        mov     ax,word ptr ds:[24CAh]
        imul    word ptr ds:[14B6h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24CEh]
        imul    word ptr ds:[14C2h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L3723
        jg      short L3720
        mov     cx,8000h
        jmp     short L3723
L3720:
        mov     cx,7FFFh
L3723:
        mov     word ptr ds:[2498h],cx
        mov     ax,word ptr ds:[24CCh]
        imul    word ptr ds:[14B2h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24CAh]
        imul    word ptr ds:[14BEh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L374D
        jg      short L374A
        mov     cx,8000h
        jmp     short L374D
L374A:
        mov     cx,7FFFh
L374D:
        mov     word ptr ds:[14BEh],cx
        mov     ax,word ptr ds:[24CCh]
        imul    word ptr ds:[14B4h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24CAh]
        imul    word ptr ds:[14C0h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L3777
        jg      short L3774
        mov     cx,8000h
        jmp     short L3777
L3774:
        mov     cx,7FFFh
L3777:
        mov     word ptr ds:[14C0h],cx
        mov     ax,word ptr ds:[24CCh]
        imul    word ptr ds:[14B6h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24CAh]
        imul    word ptr ds:[14C2h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L37A1
        jg      short L379E
        mov     cx,8000h
        jmp     short L37A1
L379E:
        mov     cx,7FFFh
L37A1:
        mov     word ptr ds:[14C2h],cx
        mov     ax,word ptr ds:[2494h]
        mov     word ptr ds:[14B2h],ax
        mov     ax,word ptr ds:[2496h]
        mov     word ptr ds:[14B4h],ax
        mov     ax,word ptr ds:[2498h]
        mov     word ptr ds:[14B6h],ax
        ret
_instance_head endp

; seg004_0849_37B8  (+37B8)
_instance_vvars_pitch proc near
        mov     ax,word ptr ds:[25E8h]
        imul    word ptr ds:[24D6h]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[25EAh]
        imul    word ptr ds:[24DAh]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        mov     ax,word ptr ds:[25E8h]
        mov     word ptr ds:[25E8h],dx
        imul    word ptr ds:[24D8h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[25EAh]
        imul    word ptr ds:[24D6h]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[25EAh],dx
        ret
_instance_vvars_pitch endp

; seg004_0849_37F5  (+37F5)
_instance_pitch proc near
        mov     ax,ss
        mov     ds,ax
        call    far ptr _sincos
        mov     dx,es
        mov     ds,dx
        mov     word ptr ds:[24D8h],ax
        mov     word ptr ds:[24D6h],bx
        neg     ax
        mov     word ptr ds:[24DAh],ax
        call    _instance_vvars_pitch
        mov     ax,word ptr ds:[24D6h]
        imul    word ptr ds:[14B8h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24DAh]
        imul    word ptr ds:[14BEh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L3837
        jg      short L3834
        mov     cx,8000h
        jmp     short L3837
L3834:
        mov     cx,7FFFh
L3837:
        mov     word ptr ds:[2494h],cx
        mov     ax,word ptr ds:[24D6h]
        imul    word ptr ds:[14BAh]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24DAh]
        imul    word ptr ds:[14C0h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L3861
        jg      short L385E
        mov     cx,8000h
        jmp     short L3861
L385E:
        mov     cx,7FFFh
L3861:
        mov     word ptr ds:[2496h],cx
        mov     ax,word ptr ds:[24D6h]
        imul    word ptr ds:[14BCh]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24DAh]
        imul    word ptr ds:[14C2h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L388B
        jg      short L3888
        mov     cx,8000h
        jmp     short L388B
L3888:
        mov     cx,7FFFh
L388B:
        mov     word ptr ds:[2498h],cx
        mov     ax,word ptr ds:[24D8h]
        imul    word ptr ds:[14B8h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24D6h]
        imul    word ptr ds:[14BEh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L38B5
        jg      short L38B2
        mov     cx,8000h
        jmp     short L38B5
L38B2:
        mov     cx,7FFFh
L38B5:
        mov     word ptr ds:[14BEh],cx
        mov     ax,word ptr ds:[24D8h]
        imul    word ptr ds:[14BAh]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24D6h]
        imul    word ptr ds:[14C0h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L38DF
        jg      short L38DC
        mov     cx,8000h
        jmp     short L38DF
L38DC:
        mov     cx,7FFFh
L38DF:
        mov     word ptr ds:[14C0h],cx
        mov     ax,word ptr ds:[24D8h]
        imul    word ptr ds:[14BCh]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24D6h]
        imul    word ptr ds:[14C2h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L3909
        jg      short L3906
        mov     cx,8000h
        jmp     short L3909
L3906:
        mov     cx,7FFFh
L3909:
        mov     word ptr ds:[14C2h],cx
        mov     ax,word ptr ds:[2494h]
        mov     word ptr ds:[14B8h],ax
        mov     ax,word ptr ds:[2496h]
        mov     word ptr ds:[14BAh],ax
        mov     ax,word ptr ds:[2498h]
        mov     word ptr ds:[14BCh],ax
        ret
_instance_pitch endp

; seg004_0849_3920  (+3920)
_instance_vvars_bank proc near
        mov     ax,word ptr ds:[25E8h]
        imul    word ptr ds:[24D0h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[25E6h]
        imul    word ptr ds:[24D4h]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        mov     ax,word ptr ds:[25E8h]
        mov     word ptr ds:[25E8h],dx
        imul    word ptr ds:[24D2h]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[25E6h]
        imul    word ptr ds:[24D0h]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        mov     word ptr ds:[25E6h],cx
        ret
_instance_vvars_bank endp

; seg004_0849_395D  (+395D)
_instance_bank proc near
        mov     ax,ss
        mov     ds,ax
        call    far ptr _sincos
        mov     dx,es
        mov     ds,dx
        mov     word ptr ds:[24D2h],ax
        mov     word ptr ds:[24D0h],bx
        neg     ax
        mov     word ptr ds:[24D4h],ax
        call    _instance_vvars_bank
        mov     ax,word ptr ds:[24D0h]
        imul    word ptr ds:[14B2h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24D2h]
        imul    word ptr ds:[14B8h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L399F
        jg      short L399C
        mov     cx,8000h
        jmp     short L399F
L399C:
        mov     cx,7FFFh
L399F:
        mov     word ptr ds:[2494h],cx
        mov     ax,word ptr ds:[24D0h]
        imul    word ptr ds:[14B4h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24D2h]
        imul    word ptr ds:[14BAh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L39C9
        jg      short L39C6
        mov     cx,8000h
        jmp     short L39C9
L39C6:
        mov     cx,7FFFh
L39C9:
        mov     word ptr ds:[2496h],cx
        mov     ax,word ptr ds:[24D0h]
        imul    word ptr ds:[14B6h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24D2h]
        imul    word ptr ds:[14BCh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L39F3
        jg      short L39F0
        mov     cx,8000h
        jmp     short L39F3
L39F0:
        mov     cx,7FFFh
L39F3:
        mov     word ptr ds:[2498h],cx
        mov     ax,word ptr ds:[24D4h]
        imul    word ptr ds:[14B2h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24D0h]
        imul    word ptr ds:[14B8h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L3A1D
        jg      short L3A1A
        mov     cx,8000h
        jmp     short L3A1D
L3A1A:
        mov     cx,7FFFh
L3A1D:
        mov     word ptr ds:[14B8h],cx
        mov     ax,word ptr ds:[24D4h]
        imul    word ptr ds:[14B4h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24D0h]
        imul    word ptr ds:[14BAh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L3A47
        jg      short L3A44
        mov     cx,8000h
        jmp     short L3A47
L3A44:
        mov     cx,7FFFh
L3A47:
        mov     word ptr ds:[14BAh],cx
        mov     ax,word ptr ds:[24D4h]
        imul    word ptr ds:[14B6h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24D0h]
        imul    word ptr ds:[14BCh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L3A71
        jg      short L3A6E
        mov     cx,8000h
        jmp     short L3A71
L3A6E:
        mov     cx,7FFFh
L3A71:
        mov     word ptr ds:[14BCh],cx
        mov     ax,word ptr ds:[2494h]
        mov     word ptr ds:[14B2h],ax
        mov     ax,word ptr ds:[2496h]
        mov     word ptr ds:[14B4h],ax
        mov     ax,word ptr ds:[2498h]
        mov     word ptr ds:[14B6h],ax
        ret
_instance_bank endp

; seg004_0849_3A88  (+3A88)
_code_pnt proc near
        mov     ax,bp
        neg     ax
        jge     short L3AC9
        cmp     bx,ax
        jl      short L3AA1
        cmp     bx,bp
        jg      short L3AB2
        cmp     cx,bp
        jg      short L3AC3
        cmp     cx,ax
        jl      short L3AC6
        xor     al,al
        ret
L3AA1:
        cmp     cx,bp
        jg      short L3AAC
        cmp     cx,ax
        jl      short L3AAF
        mov     al,1
        ret
L3AAC:
        mov     al,5
        ret
L3AAF:
        mov     al,9
        ret
L3AB2:
        cmp     cx,bp
        jg      short L3ABD
        cmp     cx,ax
        jl      short L3AC0
        mov     al,2
        ret
L3ABD:
        mov     al,6
        ret
L3AC0:
        mov     al,0Ah
        ret
L3AC3:
        mov     al,4
        ret
L3AC6:
        mov     al,8
        ret
L3AC9:
        cmp     bx,ax
        jl      short L3AD4
        cmp     cx,bp
        jg      short L3AFA
        mov     al,8Ah
        ret
L3AD4:
        cmp     bx,bp
        jg      short L3ADF
        cmp     cx,bp
        jg      short L3AF0
        mov     al,89h
        ret
L3ADF:
        cmp     cx,bp
        jg      short L3AE6
        mov     al,8Bh
        ret
L3AE6:
        cmp     cx,ax
        jl      short L3AED
        mov     al,87h
        ret
L3AED:
        mov     al,8Fh
        ret
L3AF0:
        cmp     cx,ax
        jl      short L3AF7
        mov     al,85h
        ret
L3AF7:
        mov     al,8Dh
        ret
L3AFA:
        cmp     cx,ax
        jl      short L3B01
        mov     al,86h
        ret
L3B01:
        mov     al,8Eh
        ret
_code_pnt endp

; seg004_0849_3B04  (+3B04)
_mm9x9 proc near
        call    _mm3x9_bpsi
        mov     word ptr es:[14B2h],bx
        mov     word ptr es:[14B4h],cx
        mov     word ptr es:[14B6h],di
        add     bp,6
        call    _mm3x9_bpsi
        mov     word ptr es:[14B8h],bx
        mov     word ptr es:[14BAh],cx
        mov     word ptr es:[14BCh],di
        add     bp,6
        call    _mm3x9_bpsi
        mov     word ptr es:[14BEh],bx
        mov     word ptr es:[14C0h],cx
        mov     word ptr es:[14C2h],di
        ret
_mm9x9 endp

; seg004_0849_3B41  (+3B41)
_mm3x9_bpsi proc near
        mov     ax,word ptr [bp]
        imul    word ptr [si]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr [bp+2]
        imul    word ptr [si+6]
        add     bx,ax
        adc     cx,dx
        mov     ax,word ptr [bp+4]
        imul    word ptr [si+0Ch]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        jno     short L3B6E
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L3B6E:
        cmp     cx,8000h
        jne     short L3B77
        mov     cx,8001h
L3B77:
        push    cx
        mov     ax,word ptr [bp]
        imul    word ptr [si+2]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr [bp+2]
        imul    word ptr [si+8]
        add     bx,ax
        adc     cx,dx
        mov     ax,word ptr [bp+4]
        imul    word ptr [si+0Eh]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        jno     short L3BA6
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L3BA6:
        cmp     cx,8000h
        jne     short L3BAF
        mov     cx,8001h
L3BAF:
        mov     ax,word ptr [bp]
        imul    word ptr [si+4]
        mov     bx,ax
        mov     di,dx
        mov     ax,word ptr [bp+2]
        imul    word ptr [si+0Ah]
        add     bx,ax
        adc     di,dx
        mov     ax,word ptr [bp+4]
        imul    word ptr [si+10h]
        add     bx,ax
        adc     di,dx
        shl     bx,1
        rcl     di,1
        jno     short L3BDD
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L3BDD:
        cmp     di,8000h
        jne     short L3BE6
        mov     di,8001h
L3BE6:
        pop     bx
        ret
_mm3x9_bpsi endp

; seg004_0849_3BE8  (+3BE8)
_mm3x9t proc near
        mov     ax,word ptr [si]
        imul    word ptr [bp]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr [si+2]
        imul    word ptr [bp+2]
        add     bx,ax
        adc     cx,dx
        mov     ax,word ptr [si+4]
        imul    word ptr [bp+4]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        jno     short L3C15
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L3C15:
        cmp     cx,8000h
        jne     short L3C1E
        mov     cx,8001h
L3C1E:
        push    cx
        mov     ax,word ptr [si]
        imul    word ptr [bp+6]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr [si+2]
        imul    word ptr [bp+8]
        add     bx,ax
        adc     cx,dx
        mov     ax,word ptr [si+4]
        imul    word ptr [bp+0Ah]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        jno     short L3C4C
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L3C4C:
        cmp     cx,8000h
        jne     short L3C55
        mov     cx,8001h
L3C55:
        mov     ax,word ptr [si]
        imul    word ptr [bp+0Ch]
        mov     bx,ax
        mov     di,dx
        mov     ax,word ptr [si+2]
        imul    word ptr [bp+0Eh]
        add     bx,ax
        adc     di,dx
        mov     ax,word ptr [si+4]
        imul    word ptr [bp+10h]
        add     bx,ax
        adc     di,dx
        shl     bx,1
        rcl     di,1
        jno     short L3C82
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L3C82:
        cmp     di,8000h
        jne     short L3C8B
        mov     di,8001h
L3C8B:
        pop     bx
        ret
_mm3x9t endp

; The divide overflow handlers. ss:[5A5h] holds the one int 0 dispatches to (FM overflow_ptr);
; each skips the faulting instruction and returns a saturated result.
L3C8D           dw      0               ; bp, saved while a handler runs
overflow_count  dw      0               ; FM overflow_count, cleared by in_scalebm

; seg004_0849_3C91  (+3C91)
_overflow_handler_reg proc far
        mov     word ptr cs:L3C8D,bp
        mov     bp,sp
        inc     word ptr [bp]
        inc     word ptr [bp]
        mov     ax,7FFFh
        test    dx,0FFFFh
        jns     short L3CA9
        neg     ax
L3CA9:
        cwd
        mov     bp,word ptr cs:L3C8D
        inc     word ptr cs:overflow_count
        iret
_overflow_handler_reg endp

; seg004_0849_3CB5  (+3CB5)
_overflow_handler_mem proc far
        mov     word ptr cs:L3C8D,bp
        mov     bp,sp
        add     word ptr [bp],4
        mov     ax,7FFFh
        xor     dx,dx
        mov     bp,word ptr cs:L3C8D
        iret
_overflow_handler_mem endp

; seg004_0849_3CCB  (+3CCB)
_overflow_handler_special_bp proc far
        mov     word ptr cs:L3C8D,bp
        mov     bp,sp
        inc     word ptr [bp]
        inc     word ptr [bp]
        mov     bp,word ptr cs:L3C8D
        mov     ax,dx
        xor     ax,bp
        cwd
        mov     ax,7FFFh
        xor     ax,dx
        sub     ax,dx
        xor     dx,dx
        iret
_overflow_handler_special_bp endp

        db      4 dup (0)

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
        db      0Eh dup (0)
_do_scalebm endp

; seg004_0849_40B0  (+40B0)
_clip_vertex_x_neg_z proc near
        push    cx
        mov     ecx,dword ptr [bx]
        sub     ecx,dword ptr [si]
        add     ecx,dword ptr [bx+8]
        sub     ecx,dword ptr [si+8]
        je      L4152
        mov     eax,dword ptr [si+8]
        neg     eax
        sub     eax,dword ptr [si]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    ecx
        mov     ecx,eax
        mov     eax,dword ptr [bx]
        sub     eax,dword ptr [si]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si]
        mov     dword ptr [di],eax
        mov     eax,dword ptr [bx+4]
        sub     eax,dword ptr [si+4]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+4]
        mov     dword ptr [di+4],eax
        mov     eax,dword ptr [bx+14h]
        sub     eax,dword ptr [si+14h]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+14h]
        mov     dword ptr [di+14h],eax
        mov     eax,dword ptr [bx+18h]
        sub     eax,dword ptr [si+18h]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+18h]
        mov     dword ptr [di+18h],eax
        mov     eax,dword ptr [di]
        neg     eax
        mov     dword ptr [di+8],eax
        jne     short L4150
        jmp     L46E5
L4150:
        jmp     short L4164
L4152:
        jmp     L46E5
        push    cx
        push    es
        push    si
        push    di
        push    ds
        pop     es
        mov     cx,20h
        rep movsb
        pop     di
        pop     si
        pop     es
        pop     cx
L4164:
        pop     cx
        ret
_clip_vertex_x_neg_z endp

; seg004_0849_4166  (+4166)
_clip_vertex_y_neg_z proc near
        push    cx
        mov     ecx,dword ptr [bx+4]
        sub     ecx,dword ptr [si+4]
        add     ecx,dword ptr [bx+8]
        sub     ecx,dword ptr [si+8]
        je      L420C
        mov     eax,dword ptr [si+8]
        neg     eax
        sub     eax,dword ptr [si+4]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    ecx
        mov     ecx,eax
        mov     eax,dword ptr [bx]
        sub     eax,dword ptr [si]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si]
        mov     dword ptr [di],eax
        mov     eax,dword ptr [bx+4]
        sub     eax,dword ptr [si+4]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+4]
        mov     dword ptr [di+4],eax
        mov     eax,dword ptr [bx+14h]
        sub     eax,dword ptr [si+14h]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+14h]
        mov     dword ptr [di+14h],eax
        mov     eax,dword ptr [bx+18h]
        sub     eax,dword ptr [si+18h]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+18h]
        mov     dword ptr [di+18h],eax
        mov     eax,dword ptr [di+4]
        neg     eax
        mov     dword ptr [di+8],eax
        jne     short L420A
        jmp     L46E5
L420A:
        jmp     short L421E
L420C:
        jmp     L46E5
        push    cx
        push    es
        push    si
        push    di
        push    ds
        pop     es
        mov     cx,20h
        rep movsb
        pop     di
        pop     si
        pop     es
        pop     cx
L421E:
        pop     cx
        ret
_clip_vertex_y_neg_z endp

; seg004_0849_4220  (+4220)
_clip_vertex_x_z proc near
        push    cx
        mov     ecx,dword ptr [bx]
        sub     ecx,dword ptr [si]
        sub     ecx,dword ptr [bx+8]
        add     ecx,dword ptr [si+8]
        je      L42BF
        mov     eax,dword ptr [si+8]
        sub     eax,dword ptr [si]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    ecx
        mov     ecx,eax
        mov     eax,dword ptr [bx]
        sub     eax,dword ptr [si]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si]
        mov     dword ptr [di],eax
        mov     eax,dword ptr [bx+4]
        sub     eax,dword ptr [si+4]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+4]
        mov     dword ptr [di+4],eax
        mov     eax,dword ptr [bx+14h]
        sub     eax,dword ptr [si+14h]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+14h]
        mov     dword ptr [di+14h],eax
        mov     eax,dword ptr [bx+18h]
        sub     eax,dword ptr [si+18h]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+18h]
        mov     dword ptr [di+18h],eax
        mov     eax,dword ptr [di]
        or      eax,eax
        mov     dword ptr [di+8],eax
        jne     short L42BD
        jmp     L46E5
L42BD:
        jmp     short L42D1
L42BF:
        jmp     L46E5
        push    cx
        push    es
        push    si
        push    di
        push    ds
        pop     es
        mov     cx,20h
        rep movsb
        pop     di
        pop     si
        pop     es
        pop     cx
L42D1:
        pop     cx
        ret
_clip_vertex_x_z endp

; seg004_0849_42D3  (+42D3)
_clip_vertex_y_z proc near
        push    cx
        mov     ecx,dword ptr [bx+4]
        sub     ecx,dword ptr [si+4]
        sub     ecx,dword ptr [bx+8]
        add     ecx,dword ptr [si+8]
        je      L4376
        mov     eax,dword ptr [si+8]
        sub     eax,dword ptr [si+4]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    ecx
        mov     ecx,eax
        mov     eax,dword ptr [bx]
        sub     eax,dword ptr [si]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si]
        mov     dword ptr [di],eax
        mov     eax,dword ptr [bx+4]
        sub     eax,dword ptr [si+4]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+4]
        mov     dword ptr [di+4],eax
        mov     eax,dword ptr [bx+14h]
        sub     eax,dword ptr [si+14h]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+14h]
        mov     dword ptr [di+14h],eax
        mov     eax,dword ptr [bx+18h]
        sub     eax,dword ptr [si+18h]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+18h]
        mov     dword ptr [di+18h],eax
        mov     eax,dword ptr [di+4]
        or      eax,eax
        mov     dword ptr [di+8],eax
        jne     short L4374
        jmp     L46E5
L4374:
        jmp     short L4388
L4376:
        jmp     L46E5
        push    cx
        push    es
        push    si
        push    di
        push    ds
        pop     es
        mov     cx,20h
        rep movsb
        pop     di
        pop     si
        pop     es
        pop     cx
L4388:
        pop     cx
        ret
_clip_vertex_y_z endp

; seg004_0849_438A  (+438A)
__asm_clip_polygon_left proc near
        mov     word ptr ds:[0C88Ah],0
        mov     word ptr ds:[0C888h],si
        mov     byte ptr ds:[0C88Ch],0FFh
L4399:
        mov     eax,dword ptr [si]
        neg     eax
        cmp     eax,dword ptr [si+8]
        jge     L43D9
        test    byte ptr ds:[0C88Ch],0FFh
        jne     short L43BB
        lea     bx,[si-20h]
        call    _clip_vertex_x_neg_z
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L43BB:
        mov     byte ptr ds:[0C88Ch],1
        push    cx
        push    es
        push    si
        push    di
        push    ds
        pop     es
        mov     cx,20h
        rep movsb
        pop     di
        pop     si
        pop     es
        pop     cx
        add     di,20h
        inc     word ptr ds:[0C88Ah]
        jmp     L43F6
L43D9:
        cmp     byte ptr ds:[0C88Ch],1
        jne     short L43F1
        lea     bx,[si-20h]
        push    si
        xchg    bx,si
        call    _clip_vertex_x_neg_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L43F1:
        mov     byte ptr ds:[0C88Ch],0
L43F6:
        add     si,20h
        loop    L4399
        test    byte ptr ds:[0C88Ch],0FFh
        jne     short L4428
        mov     bx,word ptr ds:[0C888h]
        mov     eax,dword ptr [bx]
        neg     eax
        cmp     eax,dword ptr [bx+8]
        jge     L4447
        push    si
        sub     si,20h
        xchg    bx,si
        call    _clip_vertex_x_neg_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
        jmp     L4447
L4428:
        mov     bx,word ptr ds:[0C888h]
        mov     eax,dword ptr [bx]
        neg     eax
        cmp     eax,dword ptr [bx+8]
        jl      short L4447
        push    si
        sub     si,20h
        call    _clip_vertex_x_neg_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L4447:
        mov     cx,word ptr ds:[0C88Ah]
        ret
__asm_clip_polygon_left endp

; seg004_0849_444C  (+444C)
__asm_clip_polygon_bottom proc near
        mov     word ptr ds:[0C88Ah],0
        mov     word ptr ds:[0C888h],si
        mov     byte ptr ds:[0C88Ch],0FFh
L445B:
        mov     eax,dword ptr [si+4]
        neg     eax
        cmp     eax,dword ptr [si+8]
        jge     L449C
        test    byte ptr ds:[0C88Ch],0FFh
        jne     short L447E
        lea     bx,[si-20h]
        call    _clip_vertex_y_neg_z
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L447E:
        mov     byte ptr ds:[0C88Ch],1
        push    cx
        push    es
        push    si
        push    di
        push    ds
        pop     es
        mov     cx,20h
        rep movsb
        pop     di
        pop     si
        pop     es
        pop     cx
        add     di,20h
        inc     word ptr ds:[0C88Ah]
        jmp     L44B9
L449C:
        cmp     byte ptr ds:[0C88Ch],1
        jne     short L44B4
        lea     bx,[si-20h]
        push    si
        xchg    bx,si
        call    _clip_vertex_y_neg_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L44B4:
        mov     byte ptr ds:[0C88Ch],0
L44B9:
        add     si,20h
        loop    L445B
        test    byte ptr ds:[0C88Ch],0FFh
        jne     short L44EC
        mov     bx,word ptr ds:[0C888h]
        mov     eax,dword ptr [bx+4]
        neg     eax
        cmp     eax,dword ptr [bx+8]
        jge     L450C
        push    si
        sub     si,20h
        xchg    bx,si
        call    _clip_vertex_y_neg_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
        jmp     L450C
L44EC:
        mov     bx,word ptr ds:[0C888h]
        mov     eax,dword ptr [bx+4]
        neg     eax
        cmp     eax,dword ptr [bx+8]
        jl      short L450C
        push    si
        sub     si,20h
        call    _clip_vertex_y_neg_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L450C:
        mov     cx,word ptr ds:[0C88Ah]
        ret
__asm_clip_polygon_bottom endp

; seg004_0849_4511  (+4511)
__asm_clip_polygon_right proc near
        mov     word ptr ds:[0C88Ah],0
        mov     word ptr ds:[0C888h],si
        mov     byte ptr ds:[0C88Ch],0FFh
L4520:
        mov     eax,dword ptr [si]
        cmp     eax,dword ptr [si+8]
        jge     L455D
        test    byte ptr ds:[0C88Ch],0FFh
        jne     short L453F
        lea     bx,[si-20h]
        call    _clip_vertex_x_z
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L453F:
        mov     byte ptr ds:[0C88Ch],1
        push    cx
        push    es
        push    si
        push    di
        push    ds
        pop     es
        mov     cx,20h
        rep movsb
        pop     di
        pop     si
        pop     es
        pop     cx
        add     di,20h
        inc     word ptr ds:[0C88Ah]
        jmp     L457A
L455D:
        cmp     byte ptr ds:[0C88Ch],1
        jne     short L4575
        lea     bx,[si-20h]
        push    si
        xchg    bx,si
        call    _clip_vertex_x_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L4575:
        mov     byte ptr ds:[0C88Ch],0
L457A:
        add     si,20h
        loop    L4520
        test    byte ptr ds:[0C88Ch],0FFh
        jne     short L45A9
        mov     bx,word ptr ds:[0C888h]
        mov     eax,dword ptr [bx]
        cmp     eax,dword ptr [bx+8]
        jge     L45C5
        push    si
        sub     si,20h
        xchg    bx,si
        call    _clip_vertex_x_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
        jmp     L45C5
L45A9:
        mov     bx,word ptr ds:[0C888h]
        mov     eax,dword ptr [bx]
        cmp     eax,dword ptr [bx+8]
        jl      short L45C5
        push    si
        sub     si,20h
        call    _clip_vertex_x_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L45C5:
        mov     cx,word ptr ds:[0C88Ah]
        ret
__asm_clip_polygon_right endp

; seg004_0849_45CA  (+45CA)
__asm_clip_polygon_top proc near
        mov     word ptr ds:[0C88Ah],0
        mov     word ptr ds:[0C888h],si
        mov     byte ptr ds:[0C88Ch],0FFh
L45D9:
        mov     eax,dword ptr [si+4]
        cmp     eax,dword ptr [si+8]
        jge     L4617
        test    byte ptr ds:[0C88Ch],0FFh
        jne     short L45F9
        lea     bx,[si-20h]
        call    _clip_vertex_y_z
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L45F9:
        mov     byte ptr ds:[0C88Ch],1
        push    cx
        push    es
        push    si
        push    di
        push    ds
        pop     es
        mov     cx,20h
        rep movsb
        pop     di
        pop     si
        pop     es
        pop     cx
        add     di,20h
        inc     word ptr ds:[0C88Ah]
        jmp     L4634
L4617:
        cmp     byte ptr ds:[0C88Ch],1
        jne     short L462F
        lea     bx,[si-20h]
        push    si
        xchg    bx,si
        call    _clip_vertex_y_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L462F:
        mov     byte ptr ds:[0C88Ch],0
L4634:
        add     si,20h
        loop    L45D9
        test    byte ptr ds:[0C88Ch],0FFh
        jne     short L4664
        mov     bx,word ptr ds:[0C888h]
        mov     eax,dword ptr [bx+4]
        cmp     eax,dword ptr [bx+8]
        jge     L4681
        push    si
        sub     si,20h
        xchg    bx,si
        call    _clip_vertex_y_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
        jmp     L4681
L4664:
        mov     bx,word ptr ds:[0C888h]
        mov     eax,dword ptr [bx+4]
        cmp     eax,dword ptr [bx+8]
        jl      short L4681
        push    si
        sub     si,20h
        call    _clip_vertex_y_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L4681:
        mov     cx,word ptr ds:[0C88Ah]
        ret
__asm_clip_polygon_top endp

; seg004_0849_4686  (+4686)
__asm_clip_polygon proc near
        mov     word ptr ds:[0C890h],cx
        mov     word ptr ds:[0C888h],si
        push    di
        push    si
        push    es
        push    ds
        pop     es
        mov     di,0C892h
        imul    cx,20h
        rep movsb
        pop     es
        mov     cx,word ptr ds:[0C890h]
        mov     si,0C892h
L46A3:
        add     si,20h
        loop    L46A3
        mov     byte ptr ds:[0C882h],0
        mov     word ptr ds:[0C880h],sp
        mov     si,0C892h
        mov     di,0CA92h
        mov     cx,word ptr ds:[0C890h]
        call    __asm_clip_polygon_left
        jcxz    L46DF
        mov     si,0CA92h
        mov     di,0C892h
        call    __asm_clip_polygon_right
        jcxz    L46DF
        mov     si,0C892h
        mov     di,0CA92h
        call    __asm_clip_polygon_top
        jcxz    L46DF
        mov     si,0CA92h
        mov     di,0C892h
        call    __asm_clip_polygon_bottom
L46DF:
        pop     si
        pop     di
        mov     si,0C892h
        ret
L46E5:
        mov     sp,word ptr ds:[0C880h]
        pop     si
        pop     di
        sub     cx,cx
        ret
        db      2 dup (0)
_asm_texture_map_scanline_per_tlp:
        mov     eax,dword ptr ds:[0CF74h]
        mov     dword ptr ds:[0CCB8h],eax
        mov     eax,dword ptr ds:[0CF60h]
        mov     dword ptr ds:[0CCB0h],eax
        mov     eax,dword ptr ds:[0CF64h]
        mov     dword ptr ds:[0CCB4h],eax
        mov     eax,dword ptr ds:[0CF7Ch]
        mov     dword ptr ds:[0CCBCh],eax
        mov     ebx,dword ptr ds:[0CF68h]
        add     ebx,41h
        sar     ebx,10h
        mov     word ptr ds:[0CCC8h],bx
        mov     eax,dword ptr ds:[0CF5Ch]
        add     eax,41h
        sar     eax,10h
        mov     word ptr ds:[0CCC6h],ax
        mov     word ptr ds:[0CCC4h],ax
        mov     di,word ptr ds:[0CF5Ah]
        push    ds
        mov     ax,seg seg_370D
        mov     ds,ax
        shl     di,1
        mov     di,word ptr [di+95Ch]
        pop     ds
        add     di,word ptr ds:[0CCC4h]
        mov     eax,dword ptr ds:[0CF60h]
        or      eax,dword ptr ds:[0CF6Ch]
        or      eax,dword ptr ds:[0CF64h]
        or      eax,dword ptr ds:[0CF70h]
        js      L4924
        sub     bx,word ptr ds:[0CCC4h]
        je      L48ED
        js      L4924
        mov     eax,10000h
        shl     ebx,10h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    ebx
        mov     ebx,eax
        mov     eax,dword ptr ds:[0CF78h]
        sub     eax,dword ptr ds:[0CF74h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CCA8h],eax
        mov     eax,dword ptr ds:[0CF6Ch]
        xor     eax,dword ptr ds:[0CF60h]
        shr     eax,10h
        je      L47C0
        mov     eax,dword ptr ds:[0CF6Ch]
        sub     eax,dword ptr ds:[0CF60h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
L47C0:
        mov     dword ptr ds:[0CCA0h],eax
        mov     eax,dword ptr ds:[0CF70h]
        xor     eax,dword ptr ds:[0CF64h]
        shr     eax,10h
        je      L47E8
        mov     eax,dword ptr ds:[0CF70h]
        sub     eax,dword ptr ds:[0CF64h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
L47E8:
        mov     dword ptr ds:[0CCA4h],eax
        mov     eax,dword ptr ds:[0CF80h]
        sub     eax,dword ptr ds:[0CF7Ch]
        imul    ebx
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CCACh],eax
        mov     cx,word ptr ds:[0CCC8h]
        sub     cx,word ptr ds:[0CCC4h]
        mov     word ptr ds:[0CCCCh],cx
        mov     esi,dword ptr ds:[0CCB8h]
        mov     ebp,dword ptr ds:[0CCB0h]
        mov     ecx,dword ptr ds:[0CCB4h]
        mov     eax,dword ptr ds:[0CCACh]
        shl     eax,8
        mov     dword ptr cs:[L48AC+5],eax
        mov     dword ptr cs:[L48CF+5],eax
        mov     eax,dword ptr ds:[0CCBCh]
        mov     dword ptr ds:[0CCC0h],eax
        mov     ax,word ptr ds:[0CF5Eh]
        xor     ax,word ptr ds:[0CF5Ah]
        shr     ax,1
        jb      short L484E
        add     dword ptr ds:[0CCC0h],8000h
        jmp     short L4857
L484E:
        add     dword ptr ds:[0CCBCh],8000h
L4857:
        shl     dword ptr ds:[0CCBCh],8
        shl     dword ptr ds:[0CCC0h],8
        mov     al,byte ptr ds:[0CCECh]
        mov     byte ptr cs:[L488F+2],al
        shl     dword ptr ds:[0CCA0h],1
        shl     dword ptr ds:[0CCA4h],1
        shl     dword ptr ds:[0CCA8h],1
L4879:
        mov     eax,ebp
        cdq
        idiv    esi
        mov     bx,ax
        mov     eax,ecx
        cdq
        idiv    esi
        and     bx,word ptr ds:[0CCE8h]
L488F:
        shl     ax,2
        and     ax,word ptr ds:[0CCEAh]
        add     bx,ax
        mov     word ptr ds:[0CCCEh],bx
        mov     bl,byte ptr gs:[bx]
        mov     bh,byte ptr ds:[0CCBFh]
        mov     al,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],al
        inc     di
L48AC:
        add     dword ptr ds:[0CCBCh],12345678h
        dec     word ptr ds:[0CCCCh]
        je      short L48ED
        mov     bx,word ptr ds:[0CCCEh]
        mov     bl,byte ptr gs:[bx]
        mov     bh,byte ptr ds:[0CCC3h]
        mov     al,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],al
        inc     di
L48CF:
        add     dword ptr ds:[0CCC0h],12345678h
        add     ebp,dword ptr ds:[0CCA0h]
        add     ecx,dword ptr ds:[0CCA4h]
        add     esi,dword ptr ds:[0CCA8h]
        dec     word ptr ds:[0CCCCh]
        jne     L4879
L48ED:
        mov     eax,dword ptr ds:[0CF6Ch]
        cdq
        idiv    dword ptr ds:[0CF78h]
        mov     si,ax
        mov     eax,dword ptr ds:[0CF70h]
        cdq
        idiv    dword ptr ds:[0CF78h]
        and     si,word ptr ds:[0CCE8h]
        mov     cx,word ptr ds:[0CCECh]
        shl     ax,cl
        and     ax,word ptr ds:[0CCEAh]
        add     si,ax
        mov     bl,byte ptr gs:[si]
        mov     bh,byte ptr ds:[0CF82h]
        mov     al,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],al
L4924:
        ret
_asm_texture_map_scanline_lin_tlp:
        mov     eax,dword ptr ds:[0CF60h]
        mov     dword ptr ds:[0CCB0h],eax
        mov     eax,dword ptr ds:[0CF64h]
        mov     dword ptr ds:[0CCB4h],eax
        mov     eax,dword ptr ds:[0CF7Ch]
        mov     dword ptr ds:[0CCBCh],eax
        mov     ebx,dword ptr ds:[0CF68h]
        add     ebx,41h
        sar     ebx,10h
        mov     word ptr ds:[0CCC8h],bx
        mov     eax,dword ptr ds:[0CF5Ch]
        add     eax,41h
        sar     eax,10h
        mov     word ptr ds:[0CCC6h],ax
        mov     word ptr ds:[0CCC4h],ax
        mov     di,word ptr ds:[0CF5Ah]
        push    ds
        mov     ax,seg seg_370D
        mov     ds,ax
        shl     di,1
        mov     di,word ptr [di+95Ch]
        pop     ds
        add     di,word ptr ds:[0CCC4h]
        mov     eax,dword ptr ds:[0CF60h]
        or      eax,dword ptr ds:[0CF6Ch]
        or      eax,dword ptr ds:[0CF64h]
        or      eax,dword ptr ds:[0CF70h]
        js      L4B2B
        sub     bx,word ptr ds:[0CCC4h]
        je      L4B05
        js      L4B05
        mov     eax,10000h
        shl     ebx,10h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    ebx
        mov     ebx,eax
        mov     eax,dword ptr ds:[0CF6Ch]
        xor     eax,dword ptr ds:[0CF60h]
        shr     eax,10h
        je      L49D6
        mov     eax,dword ptr ds:[0CF6Ch]
        sub     eax,dword ptr ds:[0CF60h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
L49D6:
        mov     dword ptr ds:[0CCA0h],eax
        mov     eax,dword ptr ds:[0CF70h]
        xor     eax,dword ptr ds:[0CF64h]
        shr     eax,10h
        je      L49FE
        mov     eax,dword ptr ds:[0CF70h]
        sub     eax,dword ptr ds:[0CF64h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
L49FE:
        mov     dword ptr ds:[0CCA4h],eax
        mov     eax,dword ptr ds:[0CF80h]
        sub     eax,dword ptr ds:[0CF7Ch]
        imul    ebx
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CCACh],eax
        mov     cx,word ptr ds:[0CCC8h]
        sub     cx,word ptr ds:[0CCC4h]
        mov     esi,dword ptr ds:[0CCA0h]
        mov     ebp,dword ptr ds:[0CCA4h]
        mov     edx,dword ptr ds:[0CCACh]
        push    cx
        mov     cx,word ptr ds:[0CCECh]
        shl     dword ptr ds:[0CCB4h],cl
        shl     ebp,cl
        pop     cx
        shl     dword ptr ds:[0CCBCh],8
        shl     edx,8
        mov     eax,dword ptr ds:[0CCBCh]
        mov     dword ptr ds:[0CCC0h],eax
        mov     ax,word ptr ds:[0CF5Eh]
        xor     ax,word ptr ds:[0CF5Ah]
        shr     ax,1
        jb      short L4A66
        add     dword ptr ds:[0CCC0h],800000h
        jmp     short L4A6F
L4A66:
        add     dword ptr ds:[0CCBCh],800000h
L4A6F:
        shr     cx,1
        pushf
        je      short L4AD7
L4A74:
        mov     ax,word ptr ds:[0CCB6h]
        mov     bx,word ptr ds:[0CCB2h]
        and     ax,word ptr ds:[0CCEAh]
        and     bx,word ptr ds:[0CCE8h]
        add     bx,ax
        mov     bl,byte ptr gs:[bx]
        mov     bh,byte ptr ds:[0CCBFh]
        mov     al,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],al
        inc     di
        add     dword ptr ds:[0CCB0h],esi
        add     dword ptr ds:[0CCB4h],ebp
        add     dword ptr ds:[0CCBCh],edx
        mov     ax,word ptr ds:[0CCB6h]
        mov     bx,word ptr ds:[0CCB2h]
        and     ax,word ptr ds:[0CCEAh]
        and     bx,word ptr ds:[0CCE8h]
        add     bx,ax
        mov     bl,byte ptr gs:[bx]
        mov     bh,byte ptr ds:[0CCC3h]
        mov     al,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],al
        inc     di
        add     dword ptr ds:[0CCB0h],esi
        add     dword ptr ds:[0CCB4h],ebp
        add     dword ptr ds:[0CCC0h],edx
        dec     cx
        jne     L4A74
L4AD7:
        popf
        jae     short L4B05
        mov     ax,word ptr ds:[0CCB6h]
        mov     bx,word ptr ds:[0CCB2h]
        and     ax,word ptr ds:[0CCEAh]
        and     bx,word ptr ds:[0CCE8h]
        add     bx,ax
        mov     bl,byte ptr gs:[bx]
        mov     bh,byte ptr ds:[0CCBFh]
        mov     al,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],al
        inc     di
        add     dword ptr ds:[0CCB0h],esi
        add     dword ptr ds:[0CCB4h],ebp
L4B05:
        mov     si,word ptr ds:[0CF6Eh]
        mov     ax,word ptr ds:[0CF72h]
        mov     cx,word ptr ds:[0CCECh]
        shl     ax,cl
        and     ax,word ptr ds:[0CCEAh]
        and     si,word ptr ds:[0CCE8h]
        add     si,ax
        mov     bl,byte ptr gs:[si]
        mov     bh,byte ptr ds:[0CF82h]
        mov     al,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],al
L4B2B:
        ret
        db      4 dup (0)
_asm_texture_map_scanline_per_tp:
        mov     eax,dword ptr ds:[0CF74h]
        mov     dword ptr ds:[0CCB8h],eax
        mov     eax,dword ptr ds:[0CF60h]
        mov     dword ptr ds:[0CCB0h],eax
        mov     eax,dword ptr ds:[0CF64h]
        mov     dword ptr ds:[0CCB4h],eax
        mov     eax,dword ptr ds:[0CF68h]
        sub     eax,dword ptr ds:[0CF5Ch]
        js      L4CB9
        mov     ebx,dword ptr ds:[0CF68h]
        add     ebx,41h
        sar     ebx,10h
        mov     word ptr ds:[0CCC8h],bx
        mov     eax,dword ptr ds:[0CF5Ch]
        add     eax,41h
        sar     eax,10h
        mov     word ptr ds:[0CCC6h],ax
        mov     word ptr ds:[0CCC4h],ax
        mov     di,word ptr ds:[0CF5Ah]
        push    ds
        mov     ax,seg seg_370D
        mov     ds,ax
        shl     di,1
        mov     di,word ptr [di+95Ch]
        pop     ds
        add     di,word ptr ds:[0CCC4h]
        mov     eax,dword ptr ds:[0CF60h]
        or      eax,dword ptr ds:[0CF6Ch]
        or      eax,dword ptr ds:[0CF64h]
        or      eax,dword ptr ds:[0CF70h]
        js      L4CB9
        sub     bx,word ptr ds:[0CCC4h]
        je      L4C8E
        mov     eax,10000h
        shl     ebx,10h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    ebx
        mov     ebx,eax
        mov     eax,dword ptr ds:[0CF78h]
        sub     eax,dword ptr ds:[0CF74h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CCA8h],eax
        mov     eax,dword ptr ds:[0CF6Ch]
        xor     eax,dword ptr ds:[0CF60h]
        shr     eax,10h
        je      L4C01
        mov     eax,dword ptr ds:[0CF6Ch]
        sub     eax,dword ptr ds:[0CF60h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
L4C01:
        mov     dword ptr ds:[0CCA0h],eax
        mov     eax,dword ptr ds:[0CF70h]
        xor     eax,dword ptr ds:[0CF64h]
        shr     eax,10h
        je      L4C29
        mov     eax,dword ptr ds:[0CF70h]
        sub     eax,dword ptr ds:[0CF64h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
L4C29:
        mov     dword ptr ds:[0CCA4h],eax
        mov     cx,word ptr ds:[0CCC8h]
        sub     cx,word ptr ds:[0CCC4h]
        mov     ebx,dword ptr ds:[0CCB8h]
        mov     ebp,dword ptr ds:[0CCB0h]
        mov     word ptr ds:[0CCD0h],cx
        mov     ecx,dword ptr ds:[0CCB4h]
        mov     al,byte ptr ds:[0CCECh]
        mov     byte ptr cs:[L4C69+2],al
        mov     byte ptr cs:[L4CAA+2],al
L4C53:
        mov     eax,ebp
        cdq
        idiv    ebx
        and     ax,word ptr ds:[0CCE8h]
        mov     si,ax
        mov     eax,ecx
        cdq
        idiv    ebx
L4C69:
        shl     ax,2
        and     ax,word ptr ds:[0CCEAh]
        add     si,ax
        mov     al,byte ptr gs:[si]
        mov     byte ptr es:[di],al
        inc     di
        add     ebp,dword ptr ds:[0CCA0h]
        add     ecx,dword ptr ds:[0CCA4h]
        add     ebx,dword ptr ds:[0CCA8h]
        dec     word ptr ds:[0CCD0h]
        jne     L4C53
L4C8E:
        mov     eax,dword ptr ds:[0CF6Ch]
        cdq
        idiv    dword ptr ds:[0CF78h]
        and     ax,word ptr ds:[0CCE8h]
        mov     si,ax
        mov     eax,dword ptr ds:[0CF70h]
        cdq
        idiv    dword ptr ds:[0CF78h]
L4CAA:
        shl     ax,2
        and     ax,word ptr ds:[0CCEAh]
        add     si,ax
        mov     al,byte ptr gs:[si]
        mov     byte ptr es:[di],al
L4CB9:
        ret
_asm_texture_map_scanline_lin_tp:
        mov     eax,dword ptr ds:[0CF60h]
        mov     dword ptr ds:[0CCB0h],eax
        mov     eax,dword ptr ds:[0CF64h]
        mov     dword ptr ds:[0CCB4h],eax
        mov     eax,dword ptr ds:[0CF68h]
        sub     eax,dword ptr ds:[0CF5Ch]
        js      L4E03
        mov     ebx,dword ptr ds:[0CF68h]
        add     ebx,41h
        sar     ebx,10h
        mov     word ptr ds:[0CCC8h],bx
        mov     eax,dword ptr ds:[0CF5Ch]
        add     eax,41h
        sar     eax,10h
        mov     word ptr ds:[0CCC6h],ax
        mov     word ptr ds:[0CCC4h],ax
        mov     di,word ptr ds:[0CF5Ah]
        push    ds
        mov     ax,seg seg_370D
        mov     ds,ax
        shl     di,1
        mov     di,word ptr [di+95Ch]
        pop     ds
        add     di,word ptr ds:[0CCC4h]
        mov     eax,dword ptr ds:[0CF60h]
        or      eax,dword ptr ds:[0CF6Ch]
        or      eax,dword ptr ds:[0CF64h]
        or      eax,dword ptr ds:[0CF70h]
        js      L4E03
        sub     bx,word ptr ds:[0CCC4h]
        je      L4DE7
        mov     eax,10000h
        shl     ebx,10h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    ebx
        mov     ebx,eax
        mov     eax,dword ptr ds:[0CF6Ch]
        xor     eax,dword ptr ds:[0CF60h]
        shr     eax,10h
        je      L4D70
        mov     eax,dword ptr ds:[0CF6Ch]
        sub     eax,dword ptr ds:[0CF60h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
        add     eax,14h
L4D70:
        mov     dword ptr ds:[0CCA0h],eax
        mov     eax,dword ptr ds:[0CF70h]
        xor     eax,dword ptr ds:[0CF64h]
        shr     eax,10h
        je      L4D9C
        mov     eax,dword ptr ds:[0CF70h]
        sub     eax,dword ptr ds:[0CF64h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
        add     eax,14h
L4D9C:
        mov     dword ptr ds:[0CCA4h],eax
        mov     cx,word ptr ds:[0CCC8h]
        sub     cx,word ptr ds:[0CCC4h]
        mov     ebx,dword ptr ds:[0CCA0h]
        mov     ebp,dword ptr ds:[0CCA4h]
        mov     edx,ebx
        push    cx
        mov     cx,word ptr ds:[0CCECh]
        shl     dword ptr ds:[0CCB4h],cl
        shl     ebp,cl
        pop     cx
L4DC3:
        mov     si,word ptr ds:[0CCB6h]
        and     si,word ptr ds:[0CCEAh]
        mov     bx,word ptr ds:[0CCB2h]
        and     bx,word ptr ds:[0CCE8h]
        add     dword ptr ds:[0CCB0h],edx
        mov     al,byte ptr gs:[bx+si]
        mov     byte ptr es:[di],al
        inc     di
        add     dword ptr ds:[0CCB4h],ebp
        dec     cx
        jne     L4DC3
L4DE7:
        mov     si,word ptr ds:[0CF72h]
        mov     cx,word ptr ds:[0CCECh]
        shl     si,cl
        and     si,word ptr ds:[0CCEAh]
        mov     bx,word ptr ds:[0CF6Eh]
        and     bx,word ptr ds:[0CCE8h]
        mov     al,byte ptr gs:[bx+si]
        mov     byte ptr es:[di],al
L4E03:
        ret
        db      0Ch dup (0)
__asm_clip_polygon endp

; seg004_0849_4E10  (+4E10)
__asm_project_polygon proc near
        push    cx
        push    si
        push    ebx
        push    ebp
        push    edi
        mov     ax,word ptr ds:[2476h]
        shl     eax,10h
        mov     dword ptr ds:[0CCE4h],eax
        movsx   ebp,word ptr ds:[2472h]
        movsx   edi,word ptr ds:[2470h]
        mov     bx,word ptr ds:[2474h]
        shl     ebx,10h
L4E37:
        mov     eax,dword ptr [si]
        imul    ebp
        shld    edx,eax,10h
        shl     eax,10h
        idiv    dword ptr [si+8]
        add     eax,ebx
        mov     dword ptr [si+0Ch],eax
        mov     eax,dword ptr [si+4]
        imul    edi
        shld    edx,eax,10h
        shl     eax,10h
        idiv    dword ptr [si+8]
        add     eax,dword ptr ds:[0CCE4h]
        jns     L4E71
        sub     eax,eax
L4E71:
        mov     dword ptr [si+10h],eax
        call    _compute_lighting_value
        mov     dword ptr [si+1Ch],eax
        add     si,20h
        loop    L4E37
        pop     edi
        pop     ebp
        pop     ebx
        pop     si
        pop     cx
        ret
__asm_project_polygon endp

; seg004_0849_4E8A  (+4E8A)
__asm_texture_map_1 proc near
        push    bp
        mov     bp,sp
        push    si
        push    di
        push    bx
        cmp     word ptr [bp+0Ah],2
        je      L4EA4
        cmp     word ptr [bp+0Ch],0
        jl      L4EF9
        jne     L4EF4
L4EA4:
        test    byte ptr ds:[25D3h],0FFh
        jne     short L4EBA
        mov     si,word ptr [bp+6]
        mov     cx,word ptr [bp+8]
        call    __asm_clip_polygon
        or      cx,cx
        je      L4EF4
L4EBA:
        test    byte ptr cs:_divide_overflow_has_occurred,0FFh
        jne     short L4EF1
        call    __asm_project_polygon
        test    byte ptr cs:_divide_overflow_has_occurred,0FFh
        jne     short L4EF1
        push    word ptr [bp+10h]
        push    word ptr [bp+0Eh]
        push    word ptr [bp+0Ah]
        push    cx
        push    si
        push    word ptr [bp+4]
        test    word ptr ds:[0CCEEh],0FFFFh
        je      L4EEB
        call    __asm_texture_map_v
        jmp     L4EEE
L4EEB:
        call    __asm_texture_map
L4EEE:
        add     sp,0Ch
L4EF1:
        jmp     L4EF4
L4EF4:
        pop     bx
        pop     di
        pop     si
        pop     bp
        ret
L4EF9:
        int     2
        nop
        jmp     L4EF4
_divide_overflow_has_occurred db 0     ; FM _divide_overflow_has_occurred
__asm_texture_map_1 endp

; seg004_0849_4EFF  (+4EFF)
_divide_overflow_handler proc far
        push    bp
        mov     bp,sp
        push    ax
        push    bx
        push    es
        mov     byte ptr cs:_divide_overflow_has_occurred,1
        mov     bx,word ptr [bp+2]
        mov     es,word ptr [bp+4]
        cmp     byte ptr es:[bx],66h
        jne     short L4F18
        inc     bx
L4F18:
        dec     bx
L4F19:
        inc     bx
        mov     al,byte ptr es:[bx]
        cmp     al,36h
        je      L4F19
        cmp     al,2Eh
        je      L4F19
        cmp     al,3Eh
        je      L4F19
        cmp     al,26h
        je      L4F19
        inc     bx
        mov     al,byte ptr es:[bx]
        cmp     al,3Eh
        je      short L4F49
        and     al,0F0h
        cmp     al,30h
        je      short L4F4B
        cmp     al,70h
        je      short L4F4A
        cmp     al,0B0h
        je      short L4F4A
        cmp     al,0F0h
        je      short L4F4B
        int     2
L4F49:
        inc     bx
L4F4A:
        inc     bx
L4F4B:
        inc     bx
        mov     word ptr [bp+2],bx
        pop     es
        pop     bx
        pop     ax
        mov     eax,30000000h
        pop     bp
        iret
old_divide_overflow_handler dw 0       ; FM old_divide_overflow_handler: the handler it replaced
_divide_overflow_handler endp

; seg004_0849_4F5C  (+4F5C)
_seg004_0849_4F5C proc near
        push    es
        mov     ax,seg dseg062_62a6
        mov     es,ax
        mov     ax,word ptr es:[5A5h]
        mov     word ptr cs:old_divide_overflow_handler,ax
        mov     word ptr es:[5A5h],offset _divide_overflow_handler
        pop     es
        ret
_seg004_0849_4F5C endp

; seg004_0849_4F73  (+4F73)
_seg004_0849_4F73 proc near
        push    es
        mov     ax,seg dseg062_62a6
        mov     es,ax
        mov     ax,word ptr cs:old_divide_overflow_handler
        mov     word ptr es:[5A5h],ax
        pop     es
        ret
L4F83   dw      0
_seg004_0849_4F73 endp

; seg004_0849_4F85  (+4F85)
_asm_texture_map_from_draw_poly proc near
        pusha
        call    _seg004_0849_4F5C
        mov     byte ptr cs:_divide_overflow_has_occurred,0
        push    es
        mov     ax,seg seg_370D
        mov     es,ax
        mov     ax,word ptr es:[0AEEh]
        add     ax,2
        mov     word ptr ds:[0CEFCh],ax
        pop     es
        cmp     cx,8
        jbe     short L4FAB
        int     2
        nop
        mov     cx,8
L4FAB:
        mov     word ptr cs:L4F83,cx
        mov     ax,word ptr [bx+4]
        mov     word ptr ds:[0CCF8h],ax
        mov     word ptr ds:[0CCF6h],0
        mov     ax,word ptr [bx]
        push    cx
        bsf     cx,ax
        mov     word ptr ds:[0CCF2h],ax
        mov     word ptr ds:[0CCECh],cx
        dec     ax
        mov     word ptr ds:[0CCE8h],ax
        mov     ax,word ptr [bx+2]
        inc     ax
        push    ax
        sub     dx,dx
        div     word ptr ds:[0CCF2h]
        mov     word ptr ds:[0CCF4h],ax
        pop     ax
        sub     ax,word ptr ds:[0CCF2h]
        mov     word ptr ds:[0CCEAh],ax
        pop     cx
        mov     ax,word ptr [bx+6]
        mov     word ptr ds:[0CCEAh],ax
        mov     di,cx
        dec     di
        imul    di,20h
        add     di,0CCFCh
        mov     dx,cx
L4FF6:
        sub     ax,ax
        mov     word ptr [di],ax
        mov     word ptr [di+4],ax
        mov     word ptr [di+8],ax
        mov     word ptr [di+1Ch],ax
        lodsw
        mov     word ptr [di+2],ax
        lodsw
        mov     word ptr [di+6],ax
        lodsw
        mov     word ptr [di+0Ah],ax
        add     si,2
        movzx   eax,word ptr [si]
        shl     eax,8
        mov     ax,8000h
        mov     dword ptr [di+14h],eax
        movzx   eax,word ptr [si+2]
        mov     cx,10h
        sub     cx,word ptr ds:[0CCECh]
        shl     eax,cl
        mov     ax,8000h
        mov     dword ptr [di+18h],eax
        add     si,4
        sub     di,20h
        dec     dx
        jne     L4FF6
        mov     si,0CCFCh
        mov     cx,word ptr cs:L4F83
        call    L5072
        mov     word ptr ds:[0CCEEh],bx
        mov     word ptr ds:[0CCF0h],bp
        push    1
        push    word ptr ds:[2688h]
        push    cx
        push    ax
        push    word ptr cs:L4F83
        push    0CCFCh
        push    0CCF2h
        call    __asm_texture_map_1
        add     sp,0Eh
        call    _seg004_0849_4F73
        popa
        ret
L5071:
        ret
L5072:
        jcxz    L5071
        mov     bx,4000h
        mov     dx,0C000h
        push    di
        mov     di,word ptr [si+6]
        mov     bp,1
L5081:
        mov     ax,word ptr [si+0Ah]
        cmp     ax,bx
        jg      short L508A
        mov     bx,ax
L508A:
        cmp     ax,dx
        jl      short L5090
        mov     dx,ax
L5090:
        cmp     di,word ptr [si+6]
        je      L5099
        sub     bp,bp
L5099:
        add     si,20h
        loop    L5081
        pop     di
        or      bp,bp
        je      L50AE
        mov     ax,2
        sub     cx,cx
        sub     bx,bx
        jmp     short L50DF
L50AE:
        or      bx,bx
        js      short L50C9
        inc     bx
        shl     bx,2
        shr     bx,1
        cmp     dx,bx
        jg      short L50C2
        sub     ax,ax
        sub     cx,cx
        jmp     short L50CE
L50C2:
        mov     ax,2
        sub     cx,cx
        jmp     short L50CE
L50C9:
        mov     ax,2
        sub     cx,cx
L50CE:
        sub     bx,bx
        mov     dx,word ptr ds:[0C800h]
        cmp     word ptr ds:[0C788h],dx
        jne     short L50DF
        inc     bx
        sub     ax,ax
        sub     cx,cx
L50DF:
        ret
_asm_texture_map_from_draw_poly endp

; seg004_0849_50E0  (+50E0)
_compute_lighting_value proc near
        push    ecx
        push    ebx
        push    esi
        push    es
        mov     ax,seg seg_370D
        mov     es,ax
        mov     eax,dword ptr [si]
        imul    eax
        mov     ecx,eax
        mov     ebx,edx
        mov     eax,dword ptr [si+4]
        imul    eax
        add     ecx,eax
        adc     ebx,edx
        mov     eax,dword ptr [si+8]
        imul    eax
        add     eax,ecx
        adc     edx,ebx
        call    _compute_square_root_32
        shr     eax,5
        movzx   ecx,word ptr es:[64Ch]
        mul     ecx
        shr     eax,6
        mov     cx,word ptr es:[650h]
        shl     ecx,10h
        add     eax,ecx
        jns     short L5138
        xor     eax,eax
L5138:
        mov     cx,word ptr es:[64Eh]
        shl     ecx,10h
        add     eax,ecx
        cmp     eax,0F0000h
        jbe     short L5152
        mov     eax,0F0000h
L5152:
        pop     es
        pop     esi
        pop     ebx
        pop     ecx
        ret
_compute_lighting_value endp

; seg004_0849_515A  (+515A)
_compute_square_root_32 proc near
        push    edx
        mov     esi,eax
        mov     ebx,edx
        mov     ecx,edx
        inc     ecx
        shl     ecx,0Ch
        cmp     ecx,edx
        ja      short L517E
        lea     ecx,ds:[edx+1]
        sub     eax,eax
        sub     eax,1
        jcxz    L519F
L517E:
        mov     eax,esi
        mov     edx,ebx
        div     ecx
        add     ecx,eax
        rcr     ecx,1
        sub     eax,ecx
        add     eax,40h
        cmp     eax,80h
        ja      L517E
        mov     eax,ecx
L519F:
        pop     edx
        ret
        db      0Eh dup (0)
_compute_square_root_32 endp

; seg004_0849_51B0  (+51B0)
_seg004_0849_51B0 proc near
        mov     eax,dword ptr ds:[0CFE8h]
        mov     dword ptr ds:[0CF10h],eax
        mov     eax,dword ptr ds:[0CFF0h]
        mov     dword ptr ds:[0CF14h],eax
        mov     eax,dword ptr ds:[0CFF8h]
        mov     dword ptr ds:[0CF1Ch],eax
        mov     ebx,dword ptr ds:[0CFE4h]
        add     ebx,41h
        sar     ebx,10h
        mov     word ptr ds:[0CF26h],bx
        mov     eax,dword ptr ds:[0CFE0h]
        add     eax,41h
        sar     eax,10h
        mov     word ptr ds:[0CF24h],ax
        mov     word ptr ds:[0CF5Ah],ax
        mov     di,word ptr ds:[0CFE2h]
        push    ds
        mov     ax,seg seg_370D
        mov     ds,ax
        shl     di,1
        mov     di,word ptr [di+95Ch]
        pop     ds
        add     di,word ptr ds:[0CCC4h]
        mov     eax,dword ptr ds:[0CFE8h]
        or      eax,dword ptr ds:[0CFECh]
        or      eax,dword ptr ds:[0CFF0h]
        or      eax,dword ptr ds:[0CFF4h]
        js      L53B7
        mov     ax,word ptr ds:[0CF12h]
        and     ax,word ptr ds:[0CCE8h]
        mov     word ptr ds:[0CF2Ch],ax
        sub     bx,word ptr ds:[0CF5Ah]
        je      L5396
        js      L5396
        mov     eax,10000h
        shl     ebx,10h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    ebx
        mov     ebx,eax
        mov     eax,dword ptr ds:[0CFECh]
        xor     eax,dword ptr ds:[0CFE8h]
        shr     eax,10h
        je      L526B
        mov     eax,dword ptr ds:[0CFECh]
        sub     eax,dword ptr ds:[0CFE8h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
L526B:
        mov     dword ptr ds:[0CF00h],eax
        mov     eax,dword ptr ds:[0CFF4h]
        xor     eax,dword ptr ds:[0CFF0h]
        shr     eax,10h
        je      L5293
        mov     eax,dword ptr ds:[0CFF4h]
        sub     eax,dword ptr ds:[0CFF0h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
L5293:
        mov     dword ptr ds:[0CF04h],eax
        mov     eax,dword ptr ds:[0CFFCh]
        sub     eax,dword ptr ds:[0CFF8h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF0Ch],eax
        mov     cx,word ptr ds:[0CF26h]
        sub     cx,word ptr ds:[0CF5Ah]
        mov     esi,dword ptr ds:[0CF00h]
        mov     ebp,dword ptr ds:[0CF04h]
        mov     edx,dword ptr ds:[0CF0Ch]
        push    cx
        mov     cx,word ptr ds:[0CCECh]
        shl     dword ptr ds:[0CF14h],cl
        shl     ebp,cl
        pop     cx
        shl     dword ptr ds:[0CF1Ch],8
        shl     edx,8
        mov     eax,dword ptr ds:[0CF1Ch]
        mov     dword ptr ds:[0CF20h],eax
        mov     ax,word ptr ds:[0CFE2h]
        xor     ax,word ptr ds:[0CCC4h]
        shr     ax,1
        jb      L52FD
        add     dword ptr ds:[0CF20h],800000h
        jmp     short L5306
L52FD:
        add     dword ptr ds:[0CF1Ch],800000h
L5306:
        test    word ptr ds:[2688h],0FFFFh
        je      L544B
        mov     ax,word ptr ds:[0CFFAh]
        cmp     ax,word ptr ds:[0CFFEh]
        je      L53B8
        mov     ax,word ptr ds:[0CCEAh]
        shr     cx,1
        pushf
        je      short L5374
L5323:
        mov     bx,word ptr ds:[0CF16h]
        and     bx,ax
        add     bx,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[bx]
        mov     bh,byte ptr ds:[0CF1Fh]
        mov     bl,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],bl
        add     di,word ptr ds:[0CEFCh]
        add     dword ptr ds:[0CF14h],ebp
        add     dword ptr ds:[0CF1Ch],edx
        mov     bx,word ptr ds:[0CF16h]
        and     bx,ax
        add     bx,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[bx]
        mov     bh,byte ptr ds:[0CF23h]
        mov     bl,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],bl
        add     di,word ptr ds:[0CEFCh]
        add     dword ptr ds:[0CF14h],ebp
        add     dword ptr ds:[0CF20h],edx
        dec     cx
        jne     L5323
L5374:
        popf
        jae     L5396
        mov     bx,word ptr ds:[0CF16h]
        and     bx,ax
        add     bx,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[bx]
        mov     bh,byte ptr ds:[0CF1Fh]
        mov     bl,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],bl
        add     di,word ptr ds:[0CEFCh]
L5396:
        mov     si,word ptr ds:[0CFF6h]
        mov     cx,word ptr ds:[0CCECh]
        shl     si,cl
        and     si,word ptr ds:[0CCEAh]
        add     si,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[si]
        mov     bh,byte ptr ds:[0CFFEh]
        mov     al,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],al
L53B7:
        ret
L53B8:
        mov     si,word ptr ds:[0CEFCh]
        mov     dl,byte ptr ds:[0CF1Fh]
        mov     dh,byte ptr ds:[0CF23h]
        mov     ax,word ptr ds:[0CCEAh]
        shr     cx,1
        pushf
        je      short L540B
L53CC:
        mov     bx,word ptr ds:[0CF16h]
        and     bx,ax
        add     bx,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[bx]
        mov     bh,dl
        mov     bl,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],bl
        add     di,si
        add     dword ptr ds:[0CF14h],ebp
        mov     bx,word ptr ds:[0CF16h]
        and     bx,ax
        add     bx,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[bx]
        mov     bh,dh
        mov     bl,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],bl
        add     di,si
        add     dword ptr ds:[0CF14h],ebp
        dec     cx
        jne     L53CC
L540B:
        popf
        jae     L5429
        mov     bx,word ptr ds:[0CF16h]
        and     bx,ax
        add     bx,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[bx]
        mov     bh,dl
        mov     bl,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],bl
        add     di,si
L5429:
        mov     si,word ptr ds:[0CFF6h]
        mov     cx,word ptr ds:[0CCECh]
        shl     si,cl
        and     si,word ptr ds:[0CCEAh]
        add     si,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[si]
        mov     bh,byte ptr ds:[0CFFEh]
        mov     al,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],al
        ret
L544B:
        mov     si,word ptr ds:[0CEFCh]
        mov     ax,word ptr ds:[0CCEAh]
        shr     cx,1
        pushf
        je      short L5488
L5457:
        mov     bx,word ptr ds:[0CF16h]
        and     bx,ax
        add     bx,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[bx]
        mov     byte ptr es:[di],bl
        add     di,si
        add     dword ptr ds:[0CF14h],ebp
        mov     bx,word ptr ds:[0CF16h]
        and     bx,ax
        add     bx,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[bx]
        mov     byte ptr es:[di],bl
        add     di,si
        add     dword ptr ds:[0CF14h],ebp
        dec     cx
        jne     L5457
L5488:
        popf
        jae     L549F
        mov     bx,word ptr ds:[0CF16h]
        and     bx,ax
        add     bx,word ptr ds:[0CF2Ch]
        mov     al,byte ptr gs:[bx]
        mov     byte ptr es:[di],al
        add     di,si
L549F:
        mov     si,word ptr ds:[0CFF6h]
        mov     cx,word ptr ds:[0CCECh]
        shl     si,cl
        and     si,word ptr ds:[0CCEAh]
        add     si,word ptr ds:[0CF2Ch]
        mov     al,byte ptr gs:[si]
        mov     byte ptr es:[di],al
        ret
        db      8 dup (0)
_seg004_0849_51B0 endp

; seg004_0849_54C0  (+54C0)
_seg004_0849_54C0 proc near
        mov     ax,word ptr ds:[0CF52h]
        dec     ax
        jns     short L54CA
        add     ax,word ptr [bp+8]
        nop
L54CA:
        imul    ax,20h
        mov     bx,ax
        add     ax,word ptr [bp+6]
        nop
        mov     word ptr ds:[0CF4Ch],ax
        sub     bx,20h
        jns     short L54E3
        mov     ax,word ptr [bp+8]
        imul    ax,20h
        add     bx,ax
L54E3:
        add     bx,word ptr [bp+6]
        nop
        mov     word ptr ds:[0CF4Eh],bx
        mov     bx,word ptr ds:[0CF4Ch]
        mov     eax,dword ptr [bx+0Ch]
        mov     dword ptr ds:[0CF5Ch],eax
        cmp     word ptr [bp+0Ah],1
        nop
        ja      short L5509
        mov     dword ptr ds:[0CF74h],10000h
        jmp     short L5535
L5509:
        mov     eax,100h
        cmp     dword ptr [bx+8],eax
        ja      short L551D
        mov     word ptr ds:[0CF44h],1
        jmp     short L5531
L551D:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
L5531:
        mov     dword ptr ds:[0CF74h],eax
L5535:
        mov     eax,dword ptr [bx+1Ch]
        mov     dword ptr ds:[0CF7Ch],eax
        mov     eax,dword ptr ds:[0CF74h]
        imul    dword ptr [bx+14h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF60h],eax
        mov     eax,dword ptr [bx+18h]
        imul    dword ptr ds:[0CF74h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF64h],eax
        mov     si,word ptr ds:[0CF4Eh]
        mov     eax,dword ptr [si+10h]
        add     eax,41h
        sar     eax,10h
        mov     ecx,dword ptr [bx+10h]
        add     ecx,41h
        sar     ecx,10h
        sub     eax,ecx
        shl     eax,10h
        mov     dword ptr ds:[0CF48h],eax
        jne     short L55A7
        sub     eax,eax
        mov     dword ptr ds:[0CF84h],eax
        mov     dword ptr ds:[0CF88h],eax
        mov     dword ptr ds:[0CF8Ch],eax
        mov     dword ptr ds:[0CF9Ch],eax
        mov     dword ptr ds:[0CFA4h],eax
        jmp     L5711
L55A7:
        mov     eax,dword ptr [si+1Ch]
        sub     eax,dword ptr [bx+1Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CFA4h],eax
        mov     eax,dword ptr [si+0Ch]
        sub     eax,dword ptr [bx+0Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF84h],eax
        cmp     word ptr [bp+0Ah],1
        nop
        ja      short L5626
        mov     eax,dword ptr [si+14h]
        sub     eax,dword ptr [bx+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF88h],eax
        mov     eax,dword ptr [si+18h]
        sub     eax,dword ptr [bx+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF8Ch],eax
        mov     dword ptr ds:[0CF9Ch],0
        jmp     L5711
L5626:
        mov     eax,dword ptr [bx+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
        mov     ecx,eax
        mov     eax,dword ptr [si+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF88h],eax
        mov     eax,dword ptr [bx+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
        mov     ecx,eax
        mov     eax,dword ptr [si+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF8Ch],eax
        mov     eax,100h
        cmp     dword ptr [bx+8],eax
        ja      short L56BC
        mov     word ptr ds:[0CF44h],1
        jmp     short L56D0
L56BC:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
L56D0:
        mov     ecx,eax
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L56E7
        mov     word ptr ds:[0CF44h],1
        jmp     short L56FB
L56E7:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L56FB:
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF9Ch],eax
L5711:
        ret
_seg004_0849_54C0 endp

; seg004_0849_5712  (+5712)
_seg004_0849_5712 proc near
        mov     ax,word ptr ds:[0CF54h]
        inc     ax
        cmp     ax,word ptr [bp+8]
        nop
        jb      short L5720
        sub     ax,word ptr [bp+8]
        nop
L5720:
        mov     bx,ax
        imul    ax,20h
        add     ax,word ptr [bp+6]
        nop
        mov     word ptr ds:[0CF4Ch],ax
        mov     ax,bx
        inc     ax
        cmp     ax,word ptr [bp+8]
        nop
        jb      short L5739
        sub     ax,word ptr [bp+8]
        nop
L5739:
        imul    ax,20h
        add     ax,word ptr [bp+6]
        nop
        mov     word ptr ds:[0CF4Eh],ax
        mov     bx,word ptr ds:[0CF4Ch]
        mov     eax,dword ptr [bx+0Ch]
        mov     dword ptr ds:[0CF68h],eax
        cmp     word ptr [bp+0Ah],1
        nop
        ja      short L5761
        mov     dword ptr ds:[0CF78h],10000h
        jmp     short L578D
L5761:
        mov     eax,100h
        cmp     dword ptr [bx+8],eax
        ja      short L5775
        mov     word ptr ds:[0CF44h],1
        jmp     short L5789
L5775:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
L5789:
        mov     dword ptr ds:[0CF78h],eax
L578D:
        mov     eax,dword ptr [bx+1Ch]
        mov     dword ptr ds:[0CF80h],eax
        mov     eax,dword ptr ds:[0CF78h]
        imul    dword ptr [bx+14h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF6Ch],eax
        mov     eax,dword ptr [bx+18h]
        imul    dword ptr ds:[0CF78h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF70h],eax
        mov     si,word ptr ds:[0CF4Eh]
        mov     eax,dword ptr [si+10h]
        add     eax,41h
        sar     eax,10h
        mov     ecx,dword ptr [bx+10h]
        add     ecx,41h
        sar     ecx,10h
        sub     eax,ecx
        shl     eax,10h
        mov     dword ptr ds:[0CF48h],eax
        jne     short L57FF
        sub     eax,eax
        mov     dword ptr ds:[0CF90h],eax
        mov     dword ptr ds:[0CF94h],eax
        mov     dword ptr ds:[0CF98h],eax
        mov     dword ptr ds:[0CFA0h],eax
        mov     dword ptr ds:[0CFA8h],eax
        jmp     L5965
L57FF:
        mov     eax,dword ptr [si+1Ch]
        sub     eax,dword ptr [bx+1Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CFA8h],eax
        mov     eax,dword ptr [si+0Ch]
        sub     eax,dword ptr [bx+0Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF90h],eax
        cmp     word ptr [bp+0Ah],1
        nop
        ja      short L587C
        mov     eax,dword ptr [si+14h]
        sub     eax,dword ptr [bx+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF94h],eax
        mov     eax,dword ptr [si+18h]
        sub     eax,dword ptr [bx+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF98h],eax
        sub     eax,eax
        mov     dword ptr ds:[0CFA0h],eax
        jmp     L5965
L587C:
        mov     eax,dword ptr [bx+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
        mov     ecx,eax
        mov     eax,dword ptr [si+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF94h],eax
        mov     eax,dword ptr [bx+18h]
        cdq
        shld    edx,eax,17h
        shl     eax,17h
        idiv    dword ptr [bx+8]
        mov     ecx,eax
        mov     eax,dword ptr [si+18h]
        cdq
        shld    edx,eax,17h
        shl     eax,17h
        idiv    dword ptr [si+8]
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF98h],eax
        mov     eax,100h
        cmp     dword ptr [bx+8],eax
        ja      short L5910
        mov     word ptr ds:[0CF44h],1
        jmp     short L5924
L5910:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
L5924:
        mov     ecx,eax
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L593B
        mov     word ptr ds:[0CF44h],1
        jmp     short L594F
L593B:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L594F:
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CFA0h],eax
L5965:
        ret
_seg004_0849_5712 endp

; seg004_0849_5966  (+5966)
_compute_min_max_y proc near
        mov     dword ptr ds:[0CFCCh],7FFFFFFFh
        mov     dword ptr ds:[0CFD0h],80000001h
        mov     bx,word ptr [bp+6]
        nop
        sub     dx,dx
L597E:
        mov     eax,dword ptr [bx+10h]
        cmp     eax,dword ptr ds:[0CFCCh]
        jge     short L5991
        mov     dword ptr ds:[0CFCCh],eax
        mov     word ptr ds:[0CF52h],dx
L5991:
        cmp     eax,dword ptr ds:[0CFD0h]
        jle     short L599C
        mov     dword ptr ds:[0CFD0h],eax
L599C:
        inc     dx
        add     bx,20h
        cmp     dx,word ptr [bp+8]
        nop
        jb      L597E
        mov     ax,word ptr ds:[0CF52h]
        mov     word ptr ds:[0CF54h],ax
        mov     bx,20h
        imul    bx,word ptr ds:[0CF52h]
        add     bx,word ptr [bp+6]
        nop
        mov     edx,dword ptr [bx+10h]
        add     edx,41h
        sar     edx,10h
        mov     bx,word ptr ds:[0CF52h]
        inc     bx
        cmp     bx,word ptr [bp+8]
        nop
        jb      short L59D1
        sub     bx,bx
L59D1:
        mov     cx,bx
        imul    bx,20h
        add     bx,word ptr [bp+6]
        nop
        mov     eax,dword ptr [bx+10h]
        add     eax,41h
        sar     eax,10h
        cmp     ax,dx
        jne     short L59EE
        mov     word ptr ds:[0CF54h],cx
L59EE:
        mov     bx,word ptr ds:[0CF52h]
        dec     bx
        jns     short L59F9
        add     bx,word ptr [bp+8]
        nop
L59F9:
        imul    bx,20h
        add     bx,word ptr [bp+6]
        nop
        mov     eax,dword ptr [bx+10h]
        add     eax,41h
        sar     eax,10h
        cmp     ax,dx
        jne     short L5A23
        mov     ax,word ptr ds:[0CF52h]
        mov     word ptr ds:[0CF54h],ax
        dec     word ptr ds:[0CF52h]
        jns     short L5A23
        mov     ax,word ptr [bp+8]
        add     word ptr ds:[0CF52h],ax
L5A23:
        mov     bx,word ptr ds:[2470h]
        shl     bx,1
        mov     eax,dword ptr ds:[0CFCCh]
        shr     eax,10h
        or      ax,ax
        jns     short L5A37
        sub     ax,ax
L5A37:
        cmp     ax,bx
        jb      short L5A3E
        lea     ax,[bx-1]
L5A3E:
        mov     word ptr ds:[0CF56h],ax
        mov     eax,dword ptr ds:[0CFD0h]
        shr     eax,10h
        cmp     ax,bx
        jb      short L5A50
        lea     ax,[bx-1]
L5A50:
        mov     word ptr ds:[0CF58h],ax
        ret
_compute_min_max_y endp

; seg004_0849_5A54  (+5A54)
__asm_texture_map proc near
        push    bp
        mov     bp,sp
        push    si
        push    di
        push    es
        mov     ax,seg seg_370D
        mov     es,ax
        mov     es,word ptr es:[958h]
        push    gs
        call    _seg004_0849_5D2E
        mov     bx,word ptr [bp+4]
        mov     ax,word ptr [bx+4]
        mov     word ptr ds:[0CF3Ch],ax
        mov     gs,word ptr [bx+6]
        mov     ax,word ptr [bx]
        mov     word ptr ds:[0CF3Eh],ax
        mov     word ptr ds:[0CF36h],ax
        mov     word ptr ds:[0CF34h],0
        mov     ax,word ptr [bx+2]
        mov     word ptr ds:[0CF40h],ax
        mov     word ptr ds:[0CF3Ah],ax
        mov     word ptr ds:[0CF38h],0
        mov     al,byte ptr [bx+8]
        mov     byte ptr ds:[0CF46h],al
        call    _compute_min_max_y
        call    _seg004_0849_5D66
        mov     ax,word ptr ds:[0CF56h]
        mov     word ptr ds:[0CF5Ah],ax
        mov     eax,dword ptr ds:[0CF60h]
        mov     dword ptr ds:[0CFACh],eax
        mov     eax,dword ptr ds:[0CF64h]
        mov     dword ptr ds:[0CFB4h],eax
        mov     eax,dword ptr ds:[0CF6Ch]
        mov     dword ptr ds:[0CFB0h],eax
        mov     eax,dword ptr ds:[0CF70h]
        mov     dword ptr ds:[0CFB8h],eax
L5AC4:
        mov     si,word ptr ds:[0CF52h]
        dec     si
        jns     short L5ACE
        add     si,word ptr [bp+8]
L5ACE:
        imul    si,20h
        mov     bx,word ptr [bp+6]
        mov     eax,dword ptr [bx+si+10h]
        add     eax,41h
        sar     eax,10h
        cmp     ax,word ptr ds:[0CF5Ah]
        jg      short L5B06
        call    _seg004_0849_54C0
        dec     word ptr ds:[0CF52h]
        jns     short L5AF6
        mov     ax,word ptr [bp+8]
        add     word ptr ds:[0CF52h],ax
L5AF6:
        mov     eax,dword ptr ds:[0CF60h]
        mov     dword ptr ds:[0CFACh],eax
        mov     eax,dword ptr ds:[0CF64h]
        mov     dword ptr ds:[0CFB4h],eax
L5B06:
        mov     bx,word ptr ds:[0CF54h]
        inc     bx
        cmp     bx,word ptr [bp+8]
        jb      short L5B12
        sub     bx,bx
L5B12:
        imul    bx,20h
        add     bx,word ptr [bp+6]
        mov     eax,dword ptr [bx+10h]
        add     eax,41h
        sar     eax,10h
        cmp     ax,word ptr ds:[0CF5Ah]
        jg      short L5B4F
        call    _seg004_0849_5712
        inc     word ptr ds:[0CF54h]
        mov     ax,word ptr ds:[0CF54h]
        cmp     ax,word ptr [bp+8]
        jb      short L5B3F
        mov     word ptr ds:[0CF54h],0
L5B3F:
        mov     eax,dword ptr ds:[0CF6Ch]
        mov     dword ptr ds:[0CFB0h],eax
        mov     eax,dword ptr ds:[0CF70h]
        mov     dword ptr ds:[0CFB8h],eax
L5B4F:
        mov     al,byte ptr ds:[0CF31h]
        mov     byte ptr ds:[0CF30h],al
        test    word ptr ds:[0CCF0h],0FFFFh
        je      short L5BBE
        mov     byte ptr ds:[0CF30h],5
        mov     eax,dword ptr ds:[0CFB0h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF78h]
        mov     dword ptr ds:[0CF6Ch],eax
        mov     eax,dword ptr ds:[0CFACh]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF74h]
        mov     dword ptr ds:[0CF60h],eax
        mov     eax,dword ptr ds:[0CFB8h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF78h]
        mov     dword ptr ds:[0CF70h],eax
        mov     eax,dword ptr ds:[0CFB4h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF74h]
        mov     dword ptr ds:[0CF64h],eax
L5BBE:
        cmp     word ptr ds:[0CF44h],1
        je      L5CD2
        call    _seg004_0849_5CDF
        push    bp
        call    word ptr ds:[0CF42h]
        pop     bp
        mov     eax,dword ptr ds:[0CF84h]
        add     dword ptr ds:[0CF5Ch],eax
        mov     eax,dword ptr ds:[0CF9Ch]
        add     dword ptr ds:[0CF74h],eax
        mov     eax,dword ptr ds:[0CFA4h]
        add     dword ptr ds:[0CF7Ch],eax
        mov     eax,dword ptr ds:[0CF90h]
        add     dword ptr ds:[0CF68h],eax
        mov     eax,dword ptr ds:[0CFA0h]
        add     dword ptr ds:[0CF78h],eax
        mov     eax,dword ptr ds:[0CFA8h]
        add     dword ptr ds:[0CF80h],eax
        test    word ptr ds:[0CCF0h],0FFFFh
        jne     short L5C34
        mov     eax,dword ptr ds:[0CF88h]
        add     dword ptr ds:[0CF60h],eax
        mov     eax,dword ptr ds:[0CF8Ch]
        add     dword ptr ds:[0CF64h],eax
        mov     eax,dword ptr ds:[0CF94h]
        add     dword ptr ds:[0CF6Ch],eax
        mov     eax,dword ptr ds:[0CF98h]
        add     dword ptr ds:[0CF70h],eax
        jmp     short L5C58
L5C34:
        mov     eax,dword ptr ds:[0CF88h]
        add     dword ptr ds:[0CFACh],eax
        mov     eax,dword ptr ds:[0CF8Ch]
        add     dword ptr ds:[0CFB4h],eax
        mov     eax,dword ptr ds:[0CF94h]
        add     dword ptr ds:[0CFB0h],eax
        mov     eax,dword ptr ds:[0CF98h]
        add     dword ptr ds:[0CFB8h],eax
L5C58:
        inc     word ptr ds:[0CF5Ah]
        mov     ax,word ptr ds:[0CF5Ah]
        cmp     ax,word ptr ds:[0CF58h]
        jl      L5AC4
        test    word ptr ds:[0CCF0h],0FFFFh
        je      short L5CCB
        mov     eax,dword ptr ds:[0CFB0h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF78h]
        mov     dword ptr ds:[0CF6Ch],eax
        mov     eax,dword ptr ds:[0CFACh]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF74h]
        mov     dword ptr ds:[0CF60h],eax
        mov     eax,dword ptr ds:[0CFB8h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF78h]
        mov     dword ptr ds:[0CF70h],eax
        mov     eax,dword ptr ds:[0CFB4h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF74h]
        mov     dword ptr ds:[0CF64h],eax
L5CCB:
        call    _seg004_0849_5CDF
        call    word ptr ds:[0CF42h]
L5CD2:
        mov     word ptr ds:[0CF44h],0
        pop     gs
        pop     es
        pop     di
        pop     si
        pop     bp
        ret
__asm_texture_map endp

; seg004_0849_5CDF  (+5CDF)
_seg004_0849_5CDF proc near
        test    word ptr ds:[0CF5Eh],0FFFFh
        jns     short L5CF0
        mov     dword ptr ds:[0CF5Ch],0
L5CF0:
        test    word ptr ds:[0CF6Ah],0FFFFh
        jns     short L5D01
        mov     dword ptr ds:[0CF68h],0
L5D01:
        mov     ax,word ptr ds:[2472h]
        shl     ax,1
        cmp     word ptr ds:[0CF5Eh],ax
        jb      short L5D13
        mov     word ptr ds:[0CF5Eh],ax
        dec     word ptr ds:[0CF5Eh]
L5D13:
        cmp     word ptr ds:[0CF6Ah],ax
        jb      short L5D1D
        dec     ax
        mov     word ptr ds:[0CF6Ah],ax
L5D1D:
        mov     ax,word ptr ds:[2470h]
        shl     ax,1
        cmp     word ptr ds:[0CF5Ah],ax
        jae     short L5D29
        ret
L5D29:
        dec     ax
        mov     word ptr ds:[0CF5Ah],ax
        ret
_seg004_0849_5CDF endp

; seg004_0849_5D2E  (+5D2E)
_seg004_0849_5D2E proc near
        test    word ptr ds:[0CCF0h],0FFFFh
        jne     L5D3E
        cmp     word ptr [bp+0Ah],1
        ja      short L5D51
L5D3E:
        test    word ptr [bp+0Ch],0FFFFh
        je      short L5D4A
        mov     ax,offset _asm_texture_map_scanline_lin_tlp
        jmp     short L5D4F
L5D4A:
        mov     ax,offset _asm_texture_map_scanline_lin_tp
        jmp     short L5D4F
L5D4F:
        jmp     short L5D62
L5D51:
        test    word ptr [bp+0Ch],0FFFFh
        je      short L5D5D
        mov     ax,offset _asm_texture_map_scanline_per_tlp
        jmp     short L5D62
L5D5D:
        mov     ax,offset _asm_texture_map_scanline_per_tp
        jmp     short L5D62
L5D62:
        mov     word ptr ds:[0CF42h],ax
        ret
_seg004_0849_5D2E endp

; seg004_0849_5D66  (+5D66)
_seg004_0849_5D66 proc near
        mov     ax,20h
        imul    word ptr ds:[0CF52h]
        add     ax,word ptr [bp+6]
        mov     word ptr ds:[0CFC4h],ax
        mov     ax,word ptr ds:[0CF52h]
        dec     ax
        jns     short L5D7C
        add     ax,word ptr [bp+8]
L5D7C:
        imul    ax,20h
        add     ax,word ptr [bp+6]
        mov     word ptr ds:[0CFC6h],ax
        mov     ax,20h
        imul    word ptr ds:[0CF54h]
        add     ax,word ptr [bp+6]
        mov     word ptr ds:[0CFC8h],ax
        mov     ax,word ptr ds:[0CF54h]
        inc     ax
        cmp     ax,word ptr [bp+8]
        jb      short L5D9D
        sub     ax,ax
L5D9D:
        imul    ax,20h
        add     ax,word ptr [bp+6]
        mov     word ptr ds:[0CFCAh],ax
        mov     si,word ptr ds:[0CFC6h]
        mov     eax,dword ptr [si+10h]
        add     eax,41h
        sar     eax,10h
        mov     si,word ptr ds:[0CFC4h]
        mov     ecx,dword ptr [si+10h]
        add     ecx,41h
        sar     ecx,10h
        sub     ax,cx
        jne     short L5DCB
        inc     ax
L5DCB:
        shl     eax,10h
        mov     dword ptr ds:[0CFBCh],eax
        mov     si,word ptr ds:[0CFCAh]
        mov     eax,dword ptr [si+10h]
        add     eax,41h
        sar     eax,10h
        mov     si,word ptr ds:[0CFC8h]
        mov     ecx,dword ptr [si+10h]
        add     ecx,41h
        sar     ecx,10h
        sub     ax,cx
        jne     short L5DF8
        inc     ax
L5DF8:
        shl     eax,10h
        mov     dword ptr ds:[0CFC0h],eax
        mov     si,word ptr ds:[0CFC4h]
        mov     ecx,dword ptr [si+0Ch]
        mov     dword ptr ds:[0CF5Ch],ecx
        mov     si,word ptr ds:[0CFC6h]
        mov     eax,dword ptr [si+0Ch]
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CF84h],eax
        mov     si,word ptr ds:[0CFC8h]
        mov     ecx,dword ptr [si+0Ch]
        mov     dword ptr ds:[0CF68h],ecx
        mov     si,word ptr ds:[0CFCAh]
        mov     eax,dword ptr [si+0Ch]
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CF90h],eax
        mov     si,word ptr ds:[0CFC4h]
        mov     di,word ptr ds:[0CFC6h]
        cmp     word ptr [bp+0Ah],1
        ja      L5F65
        mov     eax,dword ptr [si+1Ch]
        mov     dword ptr ds:[0CF7Ch],eax
        mov     dword ptr ds:[0CF74h],10000h
        mov     eax,dword ptr [si+14h]
        mov     dword ptr ds:[0CF60h],eax
        mov     eax,dword ptr [si+18h]
        mov     dword ptr ds:[0CF64h],eax
        sub     eax,eax
        mov     dword ptr ds:[0CF9Ch],eax
        mov     eax,dword ptr [di+14h]
        sub     eax,dword ptr [si+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CF88h],eax
        mov     eax,dword ptr [di+18h]
        sub     eax,dword ptr [si+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CF8Ch],eax
        mov     eax,dword ptr [di+1Ch]
        sub     eax,dword ptr [si+1Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CFA4h],eax
        mov     dword ptr ds:[0CF78h],10000h
        mov     bx,word ptr ds:[0CFC8h]
        mov     eax,dword ptr [bx+1Ch]
        mov     dword ptr ds:[0CF80h],eax
        mov     eax,dword ptr [bx+14h]
        mov     dword ptr ds:[0CF6Ch],eax
        mov     eax,dword ptr [bx+18h]
        mov     dword ptr ds:[0CF70h],eax
        sub     eax,eax
        mov     dword ptr ds:[0CFA0h],eax
        push    si
        mov     si,word ptr ds:[0CFCAh]
        mov     eax,dword ptr [si+14h]
        sub     eax,dword ptr [bx+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CF94h],eax
        mov     eax,dword ptr [si+18h]
        sub     eax,dword ptr [bx+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CF98h],eax
        mov     eax,dword ptr [si+1Ch]
        sub     eax,dword ptr [bx+1Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CFA8h],eax
        pop     si
        jmp     L6227
L5F65:
        mov     eax,dword ptr [si+1Ch]
        mov     dword ptr ds:[0CF7Ch],eax
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L5F81
        mov     word ptr ds:[0CF44h],1
        jmp     short L5F95
L5F81:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L5F95:
        mov     dword ptr ds:[0CF74h],eax
        imul    dword ptr [si+14h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF60h],eax
        mov     eax,dword ptr [si+18h]
        imul    dword ptr ds:[0CF74h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF64h],eax
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L5FD0
        mov     word ptr ds:[0CF44h],1
        jmp     short L5FE4
L5FD0:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L5FE4:
        mov     ecx,eax
        mov     eax,100h
        cmp     dword ptr [di+8],eax
        ja      short L5FFB
        mov     word ptr ds:[0CF44h],1
        jmp     short L600F
L5FFB:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
L600F:
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CF9Ch],eax
        mov     eax,dword ptr [di+1Ch]
        sub     eax,dword ptr [si+1Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CFA4h],eax
        mov     eax,dword ptr [si+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        mov     ecx,eax
        mov     eax,dword ptr [di+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CF88h],eax
        mov     eax,dword ptr [si+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        mov     ecx,eax
        mov     eax,dword ptr [di+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CF8Ch],eax
        mov     si,word ptr ds:[0CFC8h]
        mov     eax,dword ptr [si+1Ch]
        mov     dword ptr ds:[0CF80h],eax
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L60E2
        mov     word ptr ds:[0CF44h],1
        jmp     short L60F6
L60E2:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L60F6:
        mov     dword ptr ds:[0CF78h],eax
        imul    dword ptr [si+14h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF6Ch],eax
        mov     eax,dword ptr [si+18h]
        imul    dword ptr ds:[0CF78h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF70h],eax
        mov     di,word ptr ds:[0CFCAh]
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L6135
        mov     word ptr ds:[0CF44h],1
        jmp     short L6149
L6135:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L6149:
        mov     ecx,eax
        mov     eax,100h
        cmp     dword ptr [di+8],eax
        ja      short L6160
        mov     word ptr ds:[0CF44h],1
        jmp     short L6174
L6160:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
L6174:
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CFA0h],eax
        mov     eax,dword ptr [di+1Ch]
        sub     eax,dword ptr [si+1Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CFA8h],eax
        mov     eax,dword ptr [si+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        mov     ecx,eax
        mov     eax,dword ptr [di+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CF94h],eax
        mov     eax,dword ptr [si+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        mov     ecx,eax
        mov     eax,dword ptr [di+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CF98h],eax
L6227:
        ret
        neg     ax
        je      short L622F
        cwde
        ret
L622F:
        mov     eax,10000h
        ret
        db      0Ah dup (0)
_seg004_0849_5D66 endp

; seg004_0849_6240  (+6240)
_seg004_0849_6240 proc near
        mov     ax,word ptr ds:[0CF52h]
        dec     ax
        jns     short L624A
        add     ax,word ptr [bp+8]
        nop
L624A:
        imul    ax,20h
        mov     bx,ax
        add     ax,word ptr [bp+6]
        nop
        mov     word ptr ds:[0D01Ch],ax
        sub     bx,20h
        jns     short L6263
        mov     ax,word ptr [bp+8]
        imul    ax,20h
        add     bx,ax
L6263:
        add     bx,word ptr [bp+6]
        nop
        mov     word ptr ds:[0D01Eh],bx
        mov     bx,word ptr ds:[0D01Ch]
        mov     eax,dword ptr [bx+10h]
        mov     dword ptr ds:[0D010h],eax
        mov     eax,100h
        cmp     dword ptr [bx+8],eax
        ja      short L628B
        mov     word ptr ds:[0CF44h],1
        jmp     short L629F
L628B:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
L629F:
        mov     dword ptr ds:[0CF74h],eax
        mov     eax,dword ptr [bx+1Ch]
        mov     dword ptr ds:[0CF7Ch],eax
        mov     eax,dword ptr ds:[0CF74h]
        imul    dword ptr [bx+14h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF60h],eax
        mov     eax,dword ptr [bx+18h]
        imul    dword ptr ds:[0CF74h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF64h],eax
        mov     si,word ptr ds:[0D01Eh]
        mov     eax,dword ptr [si+0Ch]
        add     eax,41h
        sar     eax,10h
        mov     ecx,dword ptr [bx+0Ch]
        add     ecx,41h
        sar     ecx,10h
        sub     eax,ecx
        shl     eax,10h
        mov     dword ptr ds:[0D018h],eax
        jne     short L6315
        sub     eax,eax
        mov     dword ptr ds:[0CF84h],eax
        mov     dword ptr ds:[0CF88h],eax
        mov     dword ptr ds:[0CF8Ch],eax
        mov     dword ptr ds:[0CF9Ch],eax
        mov     dword ptr ds:[0CFA4h],eax
        jmp     L6436
L6315:
        mov     eax,dword ptr [si+1Ch]
        sub     eax,dword ptr [bx+1Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0D018h]
        mov     dword ptr ds:[0CFA4h],eax
        mov     eax,dword ptr [si+10h]
        sub     eax,dword ptr [bx+10h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0D018h]
        mov     dword ptr ds:[0CF84h],eax
        mov     eax,dword ptr [bx+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
        mov     ecx,eax
        mov     eax,dword ptr [si+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0D018h]
        mov     dword ptr ds:[0CF88h],eax
        mov     eax,dword ptr [bx+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
        mov     ecx,eax
        mov     eax,dword ptr [si+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0D018h]
        mov     dword ptr ds:[0CF8Ch],eax
        mov     eax,100h
        cmp     dword ptr [bx+8],eax
        ja      short L63E1
        mov     word ptr ds:[0CF44h],1
        jmp     short L63F5
L63E1:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
L63F5:
        mov     ecx,eax
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L640C
        mov     word ptr ds:[0CF44h],1
        jmp     short L6420
L640C:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L6420:
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0D018h]
        mov     dword ptr ds:[0CF9Ch],eax
L6436:
        ret
_seg004_0849_6240 endp

; seg004_0849_6437  (+6437)
_seg004_0849_6437 proc near
        mov     ax,word ptr ds:[0CF54h]
        inc     ax
        cmp     ax,word ptr [bp+8]
        nop
        jb      short L6445
        sub     ax,word ptr [bp+8]
        nop
L6445:
        mov     bx,ax
        imul    ax,20h
        add     ax,word ptr [bp+6]
        nop
        mov     word ptr ds:[0D01Ch],ax
        mov     ax,bx
        inc     ax
        cmp     ax,word ptr [bp+8]
        nop
        jb      short L645E
        sub     ax,word ptr [bp+8]
        nop
L645E:
        imul    ax,20h
        add     ax,word ptr [bp+6]
        nop
        mov     word ptr ds:[0D01Eh],ax
        mov     bx,word ptr ds:[0D01Ch]
        mov     eax,dword ptr [bx+10h]
        mov     dword ptr ds:[0D014h],eax
        mov     eax,100h
        cmp     dword ptr [bx+8],eax
        ja      short L6488
        mov     word ptr ds:[0CF44h],1
        jmp     short L649C
L6488:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
L649C:
        mov     dword ptr ds:[0CF78h],eax
        mov     eax,dword ptr [bx+1Ch]
        mov     dword ptr ds:[0CF80h],eax
        mov     eax,dword ptr ds:[0CF78h]
        imul    dword ptr [bx+14h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF6Ch],eax
        mov     eax,dword ptr [bx+18h]
        imul    dword ptr ds:[0CF78h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF70h],eax
        mov     si,word ptr ds:[0D01Eh]
        mov     eax,dword ptr [si+0Ch]
        add     eax,41h
        sar     eax,10h
        mov     ecx,dword ptr [bx+0Ch]
        add     ecx,41h
        sar     ecx,10h
        sub     eax,ecx
        shl     eax,10h
        mov     dword ptr ds:[0D018h],eax
        jne     short L6512
        sub     eax,eax
        mov     dword ptr ds:[0CF90h],eax
        mov     dword ptr ds:[0CF94h],eax
        mov     dword ptr ds:[0CF98h],eax
        mov     dword ptr ds:[0CFA0h],eax
        mov     dword ptr ds:[0CFA8h],eax
        jmp     L6631
L6512:
        mov     eax,dword ptr [si+1Ch]
        sub     eax,dword ptr [bx+1Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0D018h]
        mov     dword ptr ds:[0CFA8h],eax
        mov     eax,dword ptr [si+10h]
        sub     eax,dword ptr [bx+10h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0D018h]
        mov     dword ptr ds:[0CF90h],eax
        mov     eax,dword ptr [bx+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
        mov     ecx,eax
        mov     eax,dword ptr [si+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0D018h]
        mov     dword ptr ds:[0CF94h],eax
        mov     eax,dword ptr [bx+18h]
        cdq
        shld    edx,eax,17h
        shl     eax,17h
        idiv    dword ptr [bx+8]
        mov     ecx,eax
        mov     eax,dword ptr [si+18h]
        cdq
        shld    edx,eax,17h
        shl     eax,17h
        idiv    dword ptr [si+8]
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0D018h]
        mov     dword ptr ds:[0CF98h],eax
        mov     eax,100h
        cmp     dword ptr [bx+8],eax
        ja      short L65DC
        mov     word ptr ds:[0CF44h],1
        jmp     short L65F0
L65DC:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
L65F0:
        mov     ecx,eax
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L6607
        mov     word ptr ds:[0CF44h],1
        jmp     short L661B
L6607:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L661B:
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0D018h]
        mov     dword ptr ds:[0CFA0h],eax
L6631:
        ret
_seg004_0849_6437 endp

; seg004_0849_6632  (+6632)
_seg004_0849_6632 proc near
        mov     dword ptr ds:[0D008h],7FFFFFFFh
        mov     dword ptr ds:[0D00Ch],80000001h
        mov     bx,word ptr [bp+6]
        nop
        sub     dx,dx
L664A:
        mov     eax,dword ptr [bx+0Ch]
        cmp     eax,dword ptr ds:[0D008h]
        jge     short L665D
        mov     dword ptr ds:[0D008h],eax
        mov     word ptr ds:[0CF52h],dx
L665D:
        cmp     eax,dword ptr ds:[0D00Ch]
        jle     short L6668
        mov     dword ptr ds:[0D00Ch],eax
L6668:
        inc     dx
        add     bx,20h
        cmp     dx,word ptr [bp+8]
        nop
        jb      L664A
        mov     ax,word ptr ds:[0CF52h]
        mov     word ptr ds:[0CF54h],ax
        mov     bx,20h
        imul    bx,word ptr ds:[0CF52h]
        add     bx,word ptr [bp+6]
        nop
        mov     edx,dword ptr [bx+0Ch]
        add     edx,41h
        sar     edx,10h
        mov     bx,word ptr ds:[0CF52h]
        inc     bx
        cmp     bx,word ptr [bp+8]
        nop
        jb      short L669D
        sub     bx,bx
L669D:
        mov     cx,bx
        imul    bx,20h
        add     bx,word ptr [bp+6]
        nop
        mov     eax,dword ptr [bx+0Ch]
        add     eax,41h
        sar     eax,10h
        cmp     ax,dx
        jne     short L66BA
        mov     word ptr ds:[0CF54h],cx
L66BA:
        mov     bx,word ptr ds:[0CF52h]
        dec     bx
        jns     short L66C5
        add     bx,word ptr [bp+8]
        nop
L66C5:
        imul    bx,20h
        add     bx,word ptr [bp+6]
        nop
        mov     eax,dword ptr [bx+0Ch]
        add     eax,41h
        sar     eax,10h
        cmp     ax,dx
        jne     short L66EF
        mov     ax,word ptr ds:[0CF52h]
        mov     word ptr ds:[0CF54h],ax
        dec     word ptr ds:[0CF52h]
        jns     short L66EF
        mov     ax,word ptr [bp+8]
        add     word ptr ds:[0CF52h],ax
L66EF:
        mov     bx,word ptr ds:[2472h]
        shl     bx,1
        mov     eax,dword ptr ds:[0D008h]
        shr     eax,10h
        or      ax,ax
        jns     short L6703
        sub     ax,ax
L6703:
        cmp     ax,bx
        jl      short L670A
        lea     ax,[bx-1]
L670A:
        mov     word ptr ds:[0D022h],ax
        mov     eax,dword ptr ds:[0D00Ch]
        shr     eax,10h
        cmp     ax,bx
        jl      short L671C
        lea     ax,[bx-1]
L671C:
        mov     word ptr ds:[0D024h],ax
        ret
_seg004_0849_6632 endp

; seg004_0849_6720  (+6720)
__asm_texture_map_v proc near
        push    bp
        mov     bp,sp
        push    si
        push    di
        push    es
        mov     ax,seg seg_370D
        mov     es,ax
        mov     es,word ptr es:[958h]
        push    gs
        mov     bx,word ptr [bp+4]
        mov     ax,word ptr [bx+4]
        or      ax,ax
        je      L6741
        int     2
        nop
L6741:
        mov     word ptr ds:[0CF3Ch],ax
        mov     gs,word ptr [bx+6]
        mov     ax,word ptr [bx]
        mov     word ptr ds:[0CF3Eh],ax
        mov     word ptr ds:[0CF36h],ax
        mov     word ptr ds:[0CF34h],0
        mov     ax,word ptr [bx+2]
        mov     word ptr ds:[0CF40h],ax
        mov     word ptr ds:[0CF3Ah],ax
        mov     word ptr ds:[0CF38h],0
        mov     al,byte ptr [bx+8]
        mov     byte ptr ds:[0CF46h],al
        call    _seg004_0849_6632
        call    _seg004_0849_6952
        mov     ax,word ptr ds:[0D022h]
        mov     word ptr ds:[0CCC4h],ax
L6776:
        mov     si,word ptr ds:[0CF52h]
        dec     si
        jns     short L6780
        add     si,word ptr [bp+8]
L6780:
        imul    si,20h
        mov     bx,word ptr [bp+6]
        mov     eax,dword ptr [bx+si+0Ch]
        add     eax,41h
        sar     eax,10h
        cmp     ax,word ptr ds:[0CCC4h]
        jg      short L67A8
        call    _seg004_0849_6240
        dec     word ptr ds:[0CF52h]
        jns     short L67A8
        mov     ax,word ptr [bp+8]
        add     word ptr ds:[0CF52h],ax
L67A8:
        mov     bx,word ptr ds:[0CF54h]
        inc     bx
        cmp     bx,word ptr [bp+8]
        jb      short L67B4
        sub     bx,bx
L67B4:
        imul    bx,20h
        add     bx,word ptr [bp+6]
        mov     eax,dword ptr [bx+0Ch]
        add     eax,41h
        sar     eax,10h
        cmp     ax,word ptr ds:[0CCC4h]
        jg      short L67E1
        call    _seg004_0849_6437
        inc     word ptr ds:[0CF54h]
        mov     ax,word ptr ds:[0CF54h]
        cmp     ax,word ptr [bp+8]
        jb      short L67E1
        mov     word ptr ds:[0CF54h],0
L67E1:
        cmp     word ptr ds:[0CF44h],1
        je      L6859
        call    _seg004_0849_68B5
        mov     eax,dword ptr ds:[0CF84h]
        add     dword ptr ds:[0D010h],eax
        mov     eax,dword ptr ds:[0CF88h]
        add     dword ptr ds:[0CF60h],eax
        mov     eax,dword ptr ds:[0CF8Ch]
        add     dword ptr ds:[0CF64h],eax
        mov     eax,dword ptr ds:[0CF9Ch]
        add     dword ptr ds:[0CF74h],eax
        mov     eax,dword ptr ds:[0CFA4h]
        add     dword ptr ds:[0CF7Ch],eax
        mov     eax,dword ptr ds:[0CF90h]
        add     dword ptr ds:[0D014h],eax
        mov     eax,dword ptr ds:[0CF94h]
        add     dword ptr ds:[0CF6Ch],eax
        mov     eax,dword ptr ds:[0CF98h]
        add     dword ptr ds:[0CF70h],eax
        mov     eax,dword ptr ds:[0CFA0h]
        add     dword ptr ds:[0CF78h],eax
        mov     eax,dword ptr ds:[0CFA8h]
        add     dword ptr ds:[0CF80h],eax
        inc     word ptr ds:[0CCC4h]
        mov     ax,word ptr ds:[0CCC4h]
        cmp     ax,word ptr ds:[0D024h]
        jl      L6776
        call    _seg004_0849_68B5
L6859:
        mov     word ptr ds:[0CF44h],0
        pop     gs
        pop     es
        pop     di
        pop     si
        pop     bp
        ret
__asm_texture_map_v endp

; seg004_0849_6866  (+6866)
_seg004_0849_6866 proc near
        test    word ptr ds:[0CFE6h],0FFFFh
        jns     short L6877
        mov     dword ptr ds:[0CFE4h],0
L6877:
        test    word ptr ds:[0CFE2h],0FFFFh
        jns     short L6888
        mov     dword ptr ds:[0CFE0h],0
L6888:
        mov     ax,word ptr ds:[2470h]
        shl     ax,1
        cmp     word ptr ds:[0CFE6h],ax
        jb      short L689A
        mov     word ptr ds:[0CFE6h],ax
        dec     word ptr ds:[0CFE6h]
L689A:
        cmp     word ptr ds:[0CFE2h],ax
        jb      short L68A4
        dec     ax
        mov     word ptr ds:[0CFE2h],ax
L68A4:
        mov     ax,word ptr ds:[2472h]
        shl     ax,1
        cmp     word ptr ds:[0CCC4h],ax
        jae     short L68B0
        ret
L68B0:
        dec     ax
        mov     word ptr ds:[0CCC4h],ax
        ret
_seg004_0849_6866 endp

; seg004_0849_68B5  (+68B5)
_seg004_0849_68B5 proc near
        mov     eax,dword ptr ds:[0D010h]
        mov     dword ptr ds:[0CFE4h],eax
        mov     eax,dword ptr ds:[0D014h]
        mov     dword ptr ds:[0CFE0h],eax
        dec     word ptr ds:[0CFE2h]
        inc     word ptr ds:[0CFE6h]
        call    _seg004_0849_6866
        mov     eax,dword ptr ds:[0CF6Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF78h]
        mov     dword ptr ds:[0CFE8h],eax
        mov     eax,dword ptr ds:[0CF60h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF74h]
        mov     dword ptr ds:[0CFECh],eax
        mov     eax,dword ptr ds:[0CF70h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF78h]
        mov     dword ptr ds:[0CFF0h],eax
        mov     eax,dword ptr ds:[0CF64h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF74h]
        mov     dword ptr ds:[0CFF4h],eax
        mov     eax,dword ptr ds:[0CF80h]
        mov     dword ptr ds:[0CFF8h],eax
        mov     eax,dword ptr ds:[0CF7Ch]
        mov     dword ptr ds:[0CFFCh],eax
        mov     eax,dword ptr ds:[0CF78h]
        mov     dword ptr ds:[0D000h],eax
        mov     eax,dword ptr ds:[0CF74h]
        mov     dword ptr ds:[0D004h],eax
        push    bp
        call    _seg004_0849_51B0
        pop     bp
        ret
_seg004_0849_68B5 endp

; seg004_0849_6952  (+6952)
_seg004_0849_6952 proc near
        mov     ax,20h
        imul    word ptr ds:[0CF52h]
        add     ax,word ptr [bp+6]
        mov     word ptr ds:[0CFC4h],ax
        mov     ax,word ptr ds:[0CF52h]
        dec     ax
        jns     short L6968
        add     ax,word ptr [bp+8]
L6968:
        imul    ax,20h
        add     ax,word ptr [bp+6]
        mov     word ptr ds:[0CFC6h],ax
        mov     ax,20h
        imul    word ptr ds:[0CF54h]
        add     ax,word ptr [bp+6]
        mov     word ptr ds:[0CFC8h],ax
        mov     ax,word ptr ds:[0CF54h]
        inc     ax
        cmp     ax,word ptr [bp+8]
        jb      short L6989
        sub     ax,ax
L6989:
        imul    ax,20h
        add     ax,word ptr [bp+6]
        mov     word ptr ds:[0CFCAh],ax
        mov     si,word ptr ds:[0CFC6h]
        mov     eax,dword ptr [si+0Ch]
        add     eax,41h
        sar     eax,10h
        mov     si,word ptr ds:[0CFC4h]
        mov     ecx,dword ptr [si+0Ch]
        add     ecx,41h
        sar     ecx,10h
        sub     ax,cx
        jne     short L69B7
        inc     ax
L69B7:
        shl     eax,10h
        mov     dword ptr ds:[0CFBCh],eax
        mov     si,word ptr ds:[0CFCAh]
        mov     eax,dword ptr [si+0Ch]
        add     eax,41h
        sar     eax,10h
        mov     si,word ptr ds:[0CFC8h]
        mov     ecx,dword ptr [si+0Ch]
        add     ecx,41h
        sar     ecx,10h
        sub     ax,cx
        jne     short L69E4
        inc     ax
L69E4:
        shl     eax,10h
        mov     dword ptr ds:[0CFC0h],eax
        mov     si,word ptr ds:[0CFC4h]
        mov     ecx,dword ptr [si+10h]
        mov     dword ptr ds:[0D010h],ecx
        mov     si,word ptr ds:[0CFC6h]
        mov     eax,dword ptr [si+10h]
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CF84h],eax
        mov     si,word ptr ds:[0CFC8h]
        mov     ecx,dword ptr [si+10h]
        mov     dword ptr ds:[0D014h],ecx
        mov     si,word ptr ds:[0CFCAh]
        mov     eax,dword ptr [si+10h]
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CF90h],eax
        mov     si,word ptr ds:[0CFC4h]
        mov     di,word ptr ds:[0CFC6h]
        mov     eax,dword ptr [si+1Ch]
        mov     dword ptr ds:[0CF7Ch],eax
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L6A66
        mov     word ptr ds:[0CF44h],1
        jmp     short L6A7A
L6A66:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L6A7A:
        mov     dword ptr ds:[0CF74h],eax
        imul    dword ptr [si+14h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF60h],eax
        mov     eax,dword ptr [si+18h]
        imul    dword ptr ds:[0CF74h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF64h],eax
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L6AB5
        mov     word ptr ds:[0CF44h],1
        jmp     short L6AC9
L6AB5:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L6AC9:
        mov     ecx,eax
        mov     eax,100h
        cmp     dword ptr [di+8],eax
        ja      short L6AE0
        mov     word ptr ds:[0CF44h],1
        jmp     short L6AF4
L6AE0:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
L6AF4:
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CF9Ch],eax
        mov     eax,dword ptr [di+1Ch]
        sub     eax,dword ptr [si+1Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CFA4h],eax
        mov     eax,dword ptr [si+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        mov     ecx,eax
        mov     eax,dword ptr [di+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CF88h],eax
        mov     eax,dword ptr [si+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        mov     ecx,eax
        mov     eax,dword ptr [di+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CF8Ch],eax
        mov     si,word ptr ds:[0CFC8h]
        mov     eax,dword ptr [si+1Ch]
        mov     dword ptr ds:[0CF80h],eax
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L6BC7
        mov     word ptr ds:[0CF44h],1
        jmp     short L6BDB
L6BC7:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L6BDB:
        mov     dword ptr ds:[0CF78h],eax
        imul    dword ptr [si+14h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF6Ch],eax
        mov     eax,dword ptr [si+18h]
        imul    dword ptr ds:[0CF78h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF70h],eax
        mov     di,word ptr ds:[0CFCAh]
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L6C1A
        mov     word ptr ds:[0CF44h],1
        jmp     short L6C2E
L6C1A:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L6C2E:
        mov     ecx,eax
        mov     eax,100h
        cmp     dword ptr [di+8],eax
        ja      short L6C45
        mov     word ptr ds:[0CF44h],1
        jmp     short L6C59
L6C45:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
L6C59:
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CFA0h],eax
        mov     eax,dword ptr [di+1Ch]
        sub     eax,dword ptr [si+1Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CFA8h],eax
        mov     eax,dword ptr [si+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        mov     ecx,eax
        mov     eax,dword ptr [di+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CF94h],eax
        mov     eax,dword ptr [si+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        mov     ecx,eax
        mov     eax,dword ptr [di+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CF98h],eax
        ret
        neg     ax
        je      short L6D14
        cwde
        ret
L6D14:
        mov     eax,10000h
        ret
        db      5 dup (0)
L6D20   db      10h dup (0)             ; seg003 reads its first byte (es:6D20h)

; uncmp_tab: FM uncmp_tab, the decoder for each image format byte (cFrmtoRaw, do_uwobj and
; parse_cpg call through it with BX = format * 2)
uncmp_tab       dw      offset L6D3C, offset _exp_8run, offset _exp_8str
                dw      offset _exp_5run, offset _exp_4run, offset _exp_4str
L6D3C:                                  ; format 0: not an image
        int     3
        ret

; lightabs: FM lightabs, 16 shading rows of 256 colours each, row = distance
; shade 0 (no change)
lightabs        db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,18h,19h,1Ah,1Bh,1Ch,1Dh,1Eh,1Fh
                db      20h,21h,22h,23h,24h,25h,26h,27h,28h,29h,2Ah,2Bh,2Ch,2Dh,2Eh,2Fh
                db      30h,31h,32h,33h,34h,35h,36h,37h,38h,39h,3Ah,3Bh,3Ch,3Dh,3Eh,3Fh
                db      40h,41h,42h,43h,44h,45h,46h,47h,48h,49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh
                db      50h,51h,52h,53h,54h,55h,56h,57h,58h,59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh
                db      60h,61h,62h,63h,64h,65h,66h,67h,68h,69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh
                db      70h,71h,72h,73h,74h,75h,76h,77h,78h,79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh
                db      80h,81h,82h,83h,84h,85h,86h,87h,88h,89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh
                db      90h,91h,92h,93h,94h,95h,96h,97h,98h,99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh
                db      0A0h,0A1h,0A2h,0A3h,0A4h,0A5h,0A6h,0A7h,0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh
                db      0B0h,0B1h,0B2h,0B3h,0B4h,0B5h,0B6h,0B7h,0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh
                db      0C0h,0C1h,0C2h,0C3h,0C4h,0C5h,0C6h,0C7h,0C8h,0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh
                db      0D0h,0D1h,0D2h,0D3h,0D4h,0D5h,0D6h,0D7h,0D8h,0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh
                db      0E0h,0E1h,0E2h,0E3h,0E4h,0E5h,0E6h,0E7h,0E8h,0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 1
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,19h,1Ah,1Bh,1Ch,1Dh,1Eh,1Fh,1Fh
                db      21h,22h,23h,24h,25h,26h,27h,27h,29h,2Ah,2Bh,2Ch,2Dh,2Eh,2Fh,2Fh
                db      30h,31h,32h,33h,34h,35h,36h,37h,38h,39h,3Ah,3Bh,3Ch,3Dh,3Eh,3Fh
                db      41h,42h,43h,44h,45h,46h,47h,48h,49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh
                db      51h,52h,53h,54h,55h,56h,57h,58h,59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh
                db      61h,62h,63h,64h,65h,66h,67h,68h,69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh
                db      71h,72h,73h,74h,75h,76h,77h,78h,79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh
                db      81h,82h,83h,84h,85h,86h,87h,88h,89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh
                db      91h,92h,93h,94h,95h,96h,97h,98h,99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh
                db      0A1h,0A2h,0A3h,0A4h,0A5h,0A6h,0A7h,0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh
                db      0B1h,0B2h,0B3h,0B4h,0B5h,0B6h,0B7h,0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh
                db      0C1h,0C2h,0C3h,0C4h,0C5h,0C6h,0C7h,0C8h,0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh
                db      0D1h,0D2h,0D3h,0D4h,0D5h,0D6h,0D7h,0D8h,0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh
                db      0E1h,0E2h,0E3h,0E4h,0E5h,0E6h,0E7h,0E8h,0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 2
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Ah,1Bh,1Ch,1Dh,1Eh,1Fh,1Fh,1Fh
                db      22h,23h,24h,25h,26h,27h,27h,27h,2Ah,2Bh,2Ch,2Dh,2Eh,2Fh,2Fh,2Fh
                db      34h,35h,36h,37h,38h,39h,3Ah,3Bh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      42h,43h,44h,45h,46h,47h,48h,49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh
                db      52h,53h,54h,55h,56h,57h,58h,59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh
                db      62h,63h,64h,65h,66h,67h,68h,69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh
                db      72h,73h,74h,75h,76h,77h,78h,79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh
                db      82h,83h,84h,85h,86h,87h,88h,89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh
                db      92h,93h,94h,95h,96h,97h,98h,99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh
                db      0A2h,0A3h,0A4h,0A5h,0A6h,0A7h,0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh
                db      0B2h,0B3h,0B4h,0B5h,0B6h,0B7h,0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh
                db      0C2h,0C3h,0C4h,0C5h,0C6h,0C7h,0C8h,0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh
                db      0D2h,0D3h,0D4h,0D5h,0D6h,0D7h,0D8h,0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh
                db      0E2h,0E3h,0E4h,0E5h,0E6h,0E7h,0E8h,0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 3
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Bh,1Ch,1Dh,1Eh,1Fh,1Fh,1Fh,1Fh
                db      23h,24h,25h,26h,27h,27h,27h,27h,2Bh,2Ch,2Dh,2Eh,2Fh,2Fh,2Fh,2Fh
                db      34h,35h,36h,37h,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      43h,44h,45h,46h,47h,48h,49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh
                db      53h,54h,55h,56h,57h,58h,59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh
                db      63h,64h,65h,66h,67h,68h,69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh
                db      73h,74h,75h,76h,77h,78h,79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh
                db      83h,84h,85h,86h,87h,88h,89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh
                db      93h,94h,95h,96h,97h,98h,99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh
                db      0A3h,0A4h,0A5h,0A6h,0A7h,0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh
                db      0B3h,0B4h,0B5h,0B6h,0B7h,0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh
                db      0C3h,0C4h,0C5h,0C6h,0C7h,0C8h,0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh
                db      0D3h,0D4h,0D5h,0D6h,0D7h,0D8h,0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh
                db      0E3h,0E4h,0E5h,0E6h,0E7h,0E8h,0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 4
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Ch,1Dh,1Eh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      24h,25h,26h,27h,27h,27h,27h,27h,2Ch,2Dh,2Eh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      44h,45h,46h,47h,48h,49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      54h,55h,56h,57h,58h,59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      64h,65h,66h,67h,68h,69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      74h,75h,76h,77h,78h,79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      84h,85h,86h,87h,88h,89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      94h,95h,96h,97h,98h,99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0A4h,0A5h,0A6h,0A7h,0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0B4h,0B5h,0B6h,0B7h,0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0C4h,0C5h,0C6h,0C7h,0C8h,0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0D4h,0D5h,0D6h,0D7h,0D8h,0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0E4h,0E5h,0E6h,0E7h,0E8h,0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 5
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Dh,1Eh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      25h,26h,27h,27h,27h,27h,27h,27h,2Dh,2Eh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      45h,46h,47h,48h,49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      55h,56h,57h,58h,59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      65h,66h,67h,68h,69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      75h,76h,77h,78h,79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      85h,86h,87h,88h,89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      95h,96h,97h,98h,99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0A5h,0A6h,0A7h,0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0B5h,0B6h,0B7h,0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0C5h,0C6h,0C7h,0C8h,0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0D5h,0D6h,0D7h,0D8h,0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0E5h,0E6h,0E7h,0E8h,0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 6
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Eh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      26h,27h,27h,27h,27h,27h,27h,27h,2Eh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      46h,47h,48h,49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      56h,57h,58h,59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      66h,67h,68h,69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      76h,77h,78h,79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      86h,87h,88h,89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      96h,97h,98h,99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0A6h,0A7h,0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0B6h,0B7h,0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0C6h,0C7h,0C8h,0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0D6h,0D7h,0D8h,0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0E6h,0E7h,0E8h,0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 7
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      27h,27h,27h,27h,27h,27h,27h,27h,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      47h,48h,49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      57h,58h,59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      67h,68h,69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      77h,78h,79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      87h,88h,89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      97h,98h,99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0A7h,0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0B7h,0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0C7h,0C8h,0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0D7h,0D8h,0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0E7h,0E8h,0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 8
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      27h,27h,27h,27h,27h,27h,27h,27h,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      48h,49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      58h,59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      68h,69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      78h,79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      88h,89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      98h,99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0C8h,0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0D8h,0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0E8h,0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 9
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      27h,27h,27h,27h,27h,27h,27h,27h,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 10
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      27h,27h,27h,27h,27h,27h,27h,27h,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 11
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      27h,27h,27h,27h,27h,27h,27h,27h,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 12
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      27h,27h,27h,27h,27h,27h,27h,27h,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 13
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      27h,27h,27h,27h,27h,27h,27h,27h,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 14
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      27h,27h,27h,27h,27h,27h,27h,27h,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 15
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
_seg004_0849_6952 endp

; seg004_0849_7D3E  (+7D3E)
_do_fetchmap proc near
        lodsw
        mov     di,ax
        lodsw
        mov     bx,ax
        push    es
        push    ds
        push    si
        mov     ax,bx
        shr     ax,2
        add     al,byte ptr ds:[0E4CFh]
        and     bx,3
        add     bx,0Ch
        shl     bx,8
        mov     si,bx
        cmp     al,byte ptr ds:[0E4CEh]
        je      short L7D77
        mov     byte ptr ds:[0E4CEh],al         ; map that EMS page into physical page 3
        xor     bh,bh
        mov     bl,al
        mov     ah,44h
        mov     al,3
        mov     dx,word ptr ds:[0E4D4h]
        int     67h
        or      ah,ah
        je      short L7D77
        int     3
L7D77:
        mov     ax,si
        mov     bx,seg seg052_519C
        mov     es,bx
        mov     si,0C802h
        shl     di,1
        inc     di
        shl     di,2
        add     si,di
        add     ax,word ptr ds:[0E4D2h]
        mov     word ptr es:[si],ax
        mov     ds,si
        xor     ah,ah
        mov     al,byte ptr ds:[0]
        mov     word ptr es:[2696h],ax
        pop     si
        pop     ds
        pop     es
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_fetchmap endp

; seg004_0849_7DA4  (+7DA4)
_do_settmobj proc near
        lodsw
        mov     cx,ax
        lodsw
        push    es
        push    ds
        push    si
        push    ax
        mov     ax,cx
        mov     bx,0D049h
        shl     ax,1
        add     bx,ax
        mov     ax,word ptr [bx]
        call    _seg004_0849_7EDE
        mov     bx,ax
        push    bx
        mov     di,0C832h
        mov     ax,seg seg052_519C
        mov     es,ax
        mov     ds,bx
        xor     ah,ah
        mov     al,byte ptr ds:[1]
        stosw
        mov     ax,word ptr ds:[3]
        mov     bx,ax
        dec     ax
        stosw
        mov     ax,seg _seg_5DFD
        stosw
        mov     al,byte ptr ds:[1]
        mov     cl,8
L7DDD:
        dec     cl
        shl     al,1
        jae     L7DDD
        mov     al,byte ptr ds:[2]
        dec     al
        xor     ah,ah
        shl     ax,cl
        stosw
        mov     cx,bx
        pop     dx
        pop     bx
        mov     si,5
        mov     ds,dx
        mov     di,0
        mov     ax,seg _seg_5DFD
        mov     es,ax
        rep movsb
        pop     si
        pop     ds
        pop     es
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
        lodsw
L7E0A:
        sub     ax,1C0h
        add     ax,word ptr ds:[0D030h]
        jmp     short L7E26
_do_settmobj endp

; seg004_0849_7E13  (+7E13)
_do_uwobj proc near
        lodsw
        cmp     ax,1C0h
        jge     L7E0A
        mov     bx,0D849h
        shl     ax,2
        add     bx,ax
        mov     ax,word ptr [bx]
        and     ax,3FFh
L7E26:
        mov     bx,0D049h
        shl     ax,1
        add     bx,ax
        mov     ax,word ptr [bx]
        call    _seg004_0849_7EDE
        mov     bx,ax
        lodsw
        mov     ch,al
        mov     dx,bx
        push    si
        push    bx
        mov     ds,bx
        xor     bh,bh
        mov     bl,byte ptr ds:[0]
        mov     al,byte ptr ds:[3]
        cmp     al,20h
        jb      short L7E4E
        sub     al,8
        shl     al,1
L7E4E:
        mov     cl,4
        xor     ah,ah
        shl     ax,cl
        mov     si,6742h
        add     si,ax
        mov     ax,@data
        mov     ds,ax
        mov     ax,dx
        mov     dh,ch
        mov     bp,4
        call    word ptr cs:uncmp_tab[bx]
        pop     ds
        mov     di,0C856h
        mov     bx,seg seg052_519C
        mov     es,bx
        stosw
        mov     si,1
        movsb
        inc     di
        movsb
        mov     ax,seg seg052_519C
        mov     ds,ax
        pop     si
        lodsw
        mov     di,ax
        add     di,14D0h
        mov     ax,1Ch
        db      0EAh                     ; jmp far ptr in_scalebm: TASM would shorten it
        dw      offset in_scalebm
        dw      seg in_scalebm
_do_uwobj endp

; seg004_0849_7E8F  (+7E8F)
_do_uwcrit proc near
        lodsw
        mov     cx,ax
        mov     bx,0E049h
        add     bx,ax
        add     bx,ax
        mov     al,byte ptr [bx]
        xor     ah,ah
        cmp     ax,0FFh
        je      short L7ED5
        mov     dl,byte ptr [bx+1]
        mov     bx,0E2C9h
        mov     bp,ax
        shl     ax,3
        add     bx,ax
        lodsw
        or      ax,ax
        je      short L7EB6
        mov     dl,3
L7EB6:
        lodsw
        mov     dh,al
        push    dx
        lodsw
        mov     cx,7
L7EBE:
        cmp     al,byte ptr [bx]
        jbe     short L7EC5
        inc     bx
        loop    L7EBE
L7EC5:
        mov     bx,7
        sub     bx,cx
        mov     dx,bp
        mov     bp,ax
        call    _load_cpg
        pop     dx
        jmp     _parse_cpg
L7ED5:
        add     si,8
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_uwcrit endp

; seg004_0849_7EDE  (+7EDE)
_seg004_0849_7EDE proc near
        mov     di,ax
        mov     cl,0Ch
        shr     ax,cl
        cmp     al,byte ptr ds:[0E4D1h]
        je      short L7EFD
        mov     byte ptr ds:[0E4D1h],al
        mov     bx,ax
        mov     ah,44h
        mov     al,2
        mov     dx,word ptr ds:[0E4D4h]
        int     67h
        or      ah,ah
        jne     short L7F0A
L7EFD:
        mov     ax,di
        and     ax,3FFh
        add     ax,word ptr ds:[0E4D2h]
        add     ax,800h
        ret
L7F0A:
        int     3
        ret
_seg004_0849_7EDE endp

; seg004_0849_7F0C  (+7F0C)
_seg004_0849_7F0C proc far
        call    word ptr cs:uncmp_tab[bx]
        retf
_seg004_0849_7F0C endp

; update_cache  (+7F12): FM update_cache. It saves DS in BX and then reuses BX, so it
; returns with DS = 0E3C9h; cRender reloads DS afterwards.
_update_cache proc far
        mov     bx,ds
        mov     ax,seg _CmapCache
        mov     ds,ax
        mov     di,0FCh
        mov     bx,0E3C9h
L7F1F:
        mov     al,byte ptr [bx+di]
        cmp     al,0FFh
        je      short L7F2B
        dec     al
        je      short L7F2B
        mov     byte ptr [bx+di],al
L7F2B:
        dec     di
        jne     L7F1F
        mov     ds,bx
        retf
_update_cache endp

; seg004_0849_7F31  (+7F31)
_unload_cpg proc near
        mov     di,0FFh
        mov     dl,0FFh
        mov     dh,0FFh
L7F38:
        mov     al,byte ptr [di-1C37h]
        cmp     al,dl
        jae     short L7F46
        mov     dl,al
        mov     ax,di
        mov     dh,al
L7F46:
        dec     di
        jne     L7F38
        cmp     dh,0FFh
        je      short L7F81
        mov     di,0E1C9h
        mov     ax,bx
        xchg    dh,dl
        xor     dh,dh
        mov     bx,dx
        mov     byte ptr [bx-1C37h],0FFh
        mov     cl,byte ptr [bx-1F37h]
        mov     byte ptr [bx-1F37h],0FFh
        mov     bl,cl
        mov     byte ptr [bx-1E37h],al
        mov     bx,ax
        mov     byte ptr [bx-1F37h],cl
        xor     ch,ch
        mov     bx,ax
        mov     dx,ax
        and     ax,7
        shr     dx,3
        jmp     short L7FC2
L7F81:
        add     sp,4
        add     si,4
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_unload_cpg endp

; seg004_0849_7F8D  (+7F8D)
_load_cpg proc near
        shl     dx,3
        add     bx,dx
        mov     al,byte ptr [bx-1F37h]
        cmp     al,0FFh
        je      short L7F9D
        jmp     _lcpg_pghere
L7F9D:
        mov     di,0E1C9h
        mov     al,0FEh
        mov     cx,ds
        mov     es,cx
        mov     cx,word ptr ds:[0E4CBh]
        repne scasb
        jne     _unload_cpg
foundfreepage:                          ; FM foundfreepage
        dec     di
        mov     ax,bx
        stosb
        sub     ax,dx
        shr     dx,3
        neg     cx
        add     cx,word ptr ds:[0E4CBh]
        dec     cx
        mov     byte ptr [bx-1F37h],cl
L7FC2:
        mov     word ptr ds:[0D047h],bx
        push    cx
        mov     ch,al
        mov     di,0D03Ah
        add     di,7
        mov     ax,dx
        mov     cl,3
        shr     ax,cl
        add     al,30h
        stosb
        mov     ax,dx
        and     ax,7
        add     al,30h
        stosb
        inc     di
        mov     al,ch
        shr     al,cl
        add     al,30h
        stosb
        mov     al,ch
        and     al,7
        add     al,30h
        stosb
        mov     dx,0D03Ah
        mov     ax,3D00h
        int     21h
        jb      short L802D
        mov     dx,ax
        pop     ax
        push    dx
        call    _lcpg_pghere
        pop     bx
        mov     cx,7FFFh
        mov     dx,0
        mov     ax,word ptr ds:[0E4D2h]
        mov     ds,ax
        mov     ax,3F00h
        int     21h
        lahf
        mov     cl,ah
        mov     ax,3E00h
        int     21h
        mov     ah,cl
        sahf
        mov     ax,seg seg052_519C
        mov     ds,ax
        jb      short L8032
        mov     bx,word ptr ds:[0D047h]
        mov     byte ptr [bx-1C37h],10h
        ret
L802D:
        int     2
        add     sp,2
L8032:
        int     2
        add     si,2
        add     sp,4
        mov     bx,word ptr ds:[0D047h]
        xor     ch,ch
        mov     byte ptr [bx-1C37h],0FFh
        mov     cl,byte ptr [bx-1F37h]
        mov     byte ptr [bx-1F37h],0FFh
        mov     bl,cl
        mov     byte ptr [bx-1E37h],0FEh
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_load_cpg endp

; seg004_0849_805B  (+805B)
_lcpg_pghere proc near
        mov     dl,byte ptr [bx-1C37h]
        cmp     dl,0F0h
        jae     short L8069
        add     byte ptr [bx-1C37h],2
L8069:
        cmp     al,byte ptr ds:[0E4CDh]
        jne     short lcpg_swap
        ret
lcpg_swap:
        mov     byte ptr ds:[0E4CDh],al
        xor     ah,ah
        shl     ax,1
        add     ax,word ptr ds:[0E4C9h]
        push    si
        mov     si,0D032h
        mov     word ptr [si],ax
        inc     ax
        add     si,4
        mov     word ptr [si],ax
        sub     si,4
        mov     cx,2
        mov     ax,5000h
        mov     dx,word ptr ds:[0E4D4h]
        int     67h
        pop     si
        or      ah,ah
        jne     short L809C
        ret
L809C:
        int     2
        ret
_lcpg_pghere endp

; seg004_0849_809F  (+809F)
_parse_cpg proc near
        mov     bx,bp
        mov     ax,word ptr ds:[0E4D2h]
        mov     ds,ax
        xor     ah,ah
        mov     al,dl
        mov     bp,ax
        mov     cx,5
        shl     bp,cl
        mov     di,80h
        shl     bx,1
        mov     cx,word ptr [bx+di]
        push    si
        push    ds
        push    cx
        mov     si,bp
        mov     ax,ds
        mov     bp,cx
        mov     bl,byte ptr ds:[bp+4]
        xor     bh,bh
        add     bp,5
        call    word ptr cs:uncmp_tab[bx]
        pop     si
        pop     ds
        mov     di,0C848h
        mov     bx,seg seg052_519C
        mov     es,bx
        stosw
        movsb
        inc     di
        movsb
        inc     di
        movsb
        inc     di
        movsb
        pop     si
        mov     ax,seg seg052_519C
        mov     ds,ax
        lodsw
        mov     di,ax
        add     di,14D0h
        mov     ax,0Eh
        db      0EAh                     ; jmp far ptr in_scalebm: TASM would shorten it
        dw      offset in_scalebm
        dw      seg in_scalebm
_parse_cpg endp

; seg004_0849_80F6  (+80F6)
_do_setbmcol proc near
        mov     bx,es
        mov     ax,seg seg_370D
        mov     es,ax
        lodsw
        mov     byte ptr es:[0DC1h],al
        mov     es,bx
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_setbmcol endp

; seg004_0849_810A  (+810A)
_do_mouseq proc near
        call    far ptr _seg021_22FD_102B
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_mouseq endp

; seg004_0849_8115  (+8115)
_do_uwcolv proc near
        test    word ptr ds:[2692h],0FFh
        jne     short L8141
        lodsw
        mov     di,ax
        lodsw
        mov     bx,ax
        mov     ax,word ptr [di]
        xchg    bh,bl
        mov     cx,word ptr ds:[2694h]
        add     bh,cl
        cmp     bh,0Fh
        jbe     short L8134
        mov     bh,0Fh
L8134:
        add     bx,offset lightabs
        add     bx,ax
        mov     al,byte ptr cs:[bx]
        xor     ah,ah
        jmp     short L8151
L8141:
        push    es
        mov     ax,seg seg_370D
        mov     es,ax
        mov     al,byte ptr es:[0DC1h]
        xor     ah,ah
        pop     es
        add     si,4
L8151:
        jmp     setcol_common
_do_uwcolv endp

        end
