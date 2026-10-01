; /* target: seg004_0849_DB0 */
; /* opts: /ml */
;
; The model interpreter: the far routines at DB0, create_matrix, head_view, angles_2_matrix,
; and the opcode handlers do_int2 .. do_movres with their flat_ variants.
;
; SEG004 split into its original modules: this is module 4 of 14, +DB0..+3000 of the segment
; (file 9F70h), a module of the second library (UWLIB.LIB).
; Procs: seg004_0849_DB0, seg004_0849_DBE, seg004_0849_DD2, seg004_0849_E11, create_matrix,
; head_view, seg004_0849_EDD, seg004_0849_F08, angles_2_matrix, seg004_0849_1006, do_int2,
; do_eof, do_pntres, do_lnres, do_movei, do_movec, do_movem, do_move_odata, do_multires,
; do_minimulti, do_setshade, do_uwshade, do_defres, do_minires, do_strres, do_cntres, do_rod,
; flat_rod, do_polyres, do_closure, do_asmcal, do_sfcal, do_ifge, do_ifle, do_sortvar,
; do_sortnorm, do_sortnorm_x0, do_sortnorm_y0, do_sortnorm_z0, do_org, do_hull, flat_y_rel,
; do_y_rel, do_movres, flat_x_rel, do_x_rel, flat_z_rel, do_z_rel, do_xz_rel, do_xy_rel,
; do_yz_rel, flat_xz_rel, flat_xy_rel, flat_yz_rel, do_setcolor, do_setcolv, do_goursurfv,
; do_goursurf, do_uwsurf, do_transsurf, do_vexsurf, do_cavesurf, do_ijmp, do_ijmp_i, do_rorg,
; do_defdelta, do_addres, do_subres, do_ihcall_ind, do_ihcall, do_ibcall_ind, do_ibcall,
; do_ipcall_ind, do_ipcall, do_pushc, do_popc, do_jnorm_x0, do_jnorm_y0, do_jnorm_z0,
; do_jnorm_yz0, do_jnorm_xz0, do_jnorm_xy0, do_jnorm, clip_overflow, do_3d_clip, clip_top,
; clip_bot, clip_right, clip_left, do_parapiped.
; Evidence for the bounds: starts on a paragraph after 7 zero bytes following a iret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; Its relocations are entries 2328-2350 of UW2.EXE's relocation table.

        .386

        extrn   _seg003_0272_0:far
        extrn   _seg003_0272_10:far
        extrn   _seg003_0272_5311:far
        extrn   _seg003_0272_536B:far
        extrn   _seg003_0272_8:far
        extrn   _sincos:far
        extrn   seg003_0272:far
        extrn   seg052_519C:far
        extrn   seg_370D:far
        extrn   _seg003_0272_5946:far
        extrn   _seg003_0272_5956:far

_DATA   segment word public use16 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public use16 'CODE'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _check_flat:near
        extrn   _code_pnt:near
        extrn   _instance_bank:near
        extrn   _instance_head:near
        extrn   _instance_pitch:near
        extrn   _load_xlate_rotate_pnt:near
        extrn   _mini_xlate_rotate_pnt:near
        extrn   _mm3x9t:near
        extrn   _mm9x9:near
        extrn   _mxmul:near
        extrn   _overflow_handler_reg:far
        extrn   _seg004_0849_6D20:byte
        extrn   _self_modify:near
        extrn   _set_accept:near
        extrn   _smooth_closure:near
        public  _angles_2_matrix
        public  _clip_bot
        public  _clip_left
        public  _clip_overflow
        public  _clip_right
        public  _clip_top
        public  _create_matrix
        public  _do_3d_clip
        public  _do_addres
        public  _do_asmcal
        public  _do_cavesurf
        public  _do_closure
        public  _do_cntres
        public  _do_defdelta
        public  _do_defres
        public  _do_eof
        public  _do_goursurf
        public  _do_goursurfv
        public  _do_hull
        public  _do_ibcall
        public  _do_ibcall_ind
        public  _do_ifge
        public  _do_ifle
        public  _do_ihcall
        public  _do_ihcall_ind
        public  _do_ijmp
        public  _do_ijmp_i
        public  _do_int2
        public  _do_ipcall
        public  _do_ipcall_ind
        public  _do_jnorm
        public  _do_jnorm_x0
        public  _do_jnorm_xy0
        public  _do_jnorm_xz0
        public  _do_jnorm_y0
        public  _do_jnorm_yz0
        public  _do_jnorm_z0
        public  _do_lnres
        public  _do_minimulti
        public  _do_minires
        public  _do_move_odata
        public  _do_movec
        public  _do_movei
        public  _do_movem
        public  _do_movres
        public  _do_multires
        public  _do_org
        public  _do_parapiped
        public  _do_pntres
        public  _do_polyres
        public  _do_popc
        public  _do_pushc
        public  _do_rod
        public  _do_rorg
        public  _do_setcolor
        public  _do_setcolv
        public  _do_setshade
        public  _do_sfcal
        public  _do_sortnorm
        public  _do_sortnorm_x0
        public  _do_sortnorm_y0
        public  _do_sortnorm_z0
        public  _do_sortvar
        public  _do_strres
        public  _do_subres
        public  _do_transsurf
        public  _do_uwshade
        public  _do_uwsurf
        public  _do_vexsurf
        public  _do_x_rel
        public  _do_xy_rel
        public  _do_xz_rel
        public  _do_y_rel
        public  _do_yz_rel
        public  _do_z_rel
        public  _flat_rod
        public  _flat_x_rel
        public  _flat_xy_rel
        public  _flat_xz_rel
        public  _flat_y_rel
        public  _flat_yz_rel
        public  _flat_z_rel
        public  _head_view
        public  _seg004_0849_1006
        public  _seg004_0849_DB0
        public  _seg004_0849_DBE
        public  _seg004_0849_DD2
        public  _seg004_0849_E11
        public  _seg004_0849_EDD
        public  _seg004_0849_F08
        public  draw_poly_buf_ptr
        public  modify_biasx
        public  modify_biasy
        public  modify_bx2
        public  modify_by2
        public  setcol_common

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
        mov     byte ptr cs:_seg004_0849_6D20,dl
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
        cmp     word ptr ds:[14A6h],offset _seg003_0272_5946
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
        mov     word ptr ds:[14A6h],offset _seg003_0272_5946
        mov     word ptr ds:[14A8h],offset _seg003_0272_5956
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_goursurf endp

; seg004_0849_204A  (+204A)
_do_uwsurf proc near
        test    word ptr ds:[2692h],0FFh
        jne     short L207B
        mov     word ptr ds:[14A6h],offset _seg003_0272_5946
        mov     word ptr ds:[14A8h],offset _seg003_0272_5956
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
        mov     word ptr ds:[14A6h],offset _seg003_0272_5946
        mov     word ptr ds:[14A8h],offset _seg003_0272_5956
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
_do_parapiped endp

SEG004_TEXT ends

        end
