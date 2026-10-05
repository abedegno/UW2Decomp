; /* target: seg004_27D0 */
; /* opts: /ml */
;
; The model interpreter's opcode handlers, do_int2 .. do_parapiped with their flat_ variants.
;
; What it does in the game: the 3D view is drawn by running a program. GRIDDB.C, DRAWOBJ.C and
; GAMESORT.C write the frame's render database, a buffer of 16-bit words, and VIEW3D.C calls
; cRender (seg019), which calls render_3d (INSTANCE.ASM), which sets up the view and starts
; this interpreter on that buffer. The 3D object models are bytecode in the same language.
;
; The language. A program is a stream of words read with SI (lodsw). Each opcode is an even
; word, the byte offset of its handler in the opcode table at DS:2738h (DS = seg051 while
; interpreting). Every handler ends with the dispatch `lodsw; xchg bx,ax; jmp word ptr
; [bx+2738h]` (threaded code: there is no central loop), and do_eof's `ret` returns from the
; nearest `call word ptr [bx+2738h]`. Operands follow the opcode inline. UW1's table has 110
; entries, 0 to 0DAh; it is followed by two copies of the entries for 78h..9Ch (2814h and
; 283Ah, see NOCLIP.ASM). Point operands are byte offsets into the point table at 1620h
; (premultiplied by 8).
;
; UW1 against UW2: this is UW2's INTERP.ASM from do_int2 on, instruction for instruction but
; for UW1's data offsets and these differences: two handlers UW2 lacks, seg004_375E (opcode
; 24h) and seg004_3DC6 (52h), which set up per-step increments at 26ECh..26F6h from their
; operands and are not traced further; and the opcode table reaches routines that UW2 keeps
; with no table entry: do_animate_p, do_animate_b, do_animate_h (6Eh, 74h, 76h) and do_setpat,
; do_putpat, do_usepat (3Ch, 98h, 9Ah). UW2's module also holds the camera code, which UW1
; has as a module of its own (CAMERA.ASM). There is no 386 code anywhere in UW1's seg004.
;
; Data (seg051): the view matrix at 1602h..1612h (1.15); the point table at 1620h, 8 bytes a
; point (x, y, z, clip codes at +6, shade at +7); the polygon vertex buffers at 309h and 0C69h,
; [307h] the start and [305h] the end of the current one; 161Ah and 161Bh the OR and the AND
; of a polygon's clip codes; 15F6h and 15F8h the seg003 polygon routines the next polygon is
; drawn with; 26B0h, 26B2h the y and x projection scales, 26B4h and 26B6h the screen centre;
; 26BAh..26C4h the eye position (three 32-bit coordinates); 2886h, 2888h, 288Ah the current
; object's origin and 2880h the coordinate shift. The polygon routine is called with the
; projected vertices at seg048:415Ch and CX = the vertex count.
;
; Divide overflow: cRender points int 0 at seg063:04D0h, a `jmp dword ptr cs:[4D5h]` into
; seg004. Before a divide that may overflow, the code stores the offset of a recovery routine at
; ss:[4D5h].
;
; name: UW2's (original there: 17 of its opcode handlers have the names of System Shock's
; interp.asm handlers); the routine names are FM Towns', through UW2.
;
; .186, where UW2 has .386: UW1's seg004 has no 386 instruction and none of the nop padding
; that TASM's .386 mode puts after forward conditional jumps.
;
; SEG004 split into its original modules: this is module 6 of 9, +27D0..+4980 of the segment
; (file C840h), a module of the second library.
; match: evidence for the bounds: starts on a paragraph after 5 zero bytes following NOCLIP's
; last handler, and ends with 11 zero bytes before INSTANCE.ASM.
; Its relocations are entries 2624-2638 of UW.EXE's relocation table.
;
; The routine comments are UW2's, with UW1's addresses, except where marked UW1.

; The model interpreter's opcode table: a word per opcode, the offset of its handler
; (INTERP.ASM), in seg051, the data segment while interpreting (UW2: 24F4h).
OPCODE_TABLE    equ     2738h

; The renderer's data in seg051 (DS while interpreting), as INTERP.ASM's header
; describes it. The names are ours, from the routines that use the data.
MAT_XX          equ     1602h   ; the view matrix, 1.15 fixed point: MAT_ab is input a's
MAT_XY          equ     1604h   ; coefficient in output b (mxmul: x' = x*MAT_XX + y*MAT_YX
MAT_XZ          equ     1606h   ; + z*MAT_ZX, and so on; z' is the depth)
MAT_YX          equ     1608h
MAT_YY          equ     160Ah
MAT_YZ          equ     160Ch
MAT_ZX          equ     160Eh
MAT_ZY          equ     1610h
MAT_ZZ          equ     1612h
POLY_FILL       equ     15F6h   ; the seg003 polygon routine the next polygon is drawn with
POLY_UFILL      equ     15F8h   ; its unclipped twin (do_setcolor: polygon, upolygon)
CLIP_OR         equ     161Ah   ; the OR of the current polygon's clip codes
CLIP_AND        equ     161Bh   ; and their AND
PNT_X           equ     1620h   ; the point table, 8 bytes a point: view x
PNT_Y           equ     1622h   ; y
PNT_Z           equ     1624h   ; z, the depth
PNT_CODES       equ     1626h   ; the clip codes (code_pnt)
PNT_SHADE       equ     1627h   ; the shade
VB_END          equ     305h    ; the end of the current polygon vertex buffer
VB_START        equ     307h    ; and its start
VBUF0           equ     309h    ; the two vertex buffers (8 bytes a vertex: x, y, z, codes),
VBUF1           equ     0C69h   ; which the clippers copy between
PROJ_Y          equ     26B0h   ; the y projection scale (half the window height)
PROJ_X          equ     26B2h   ; the x projection scale (half the window width)
CENTRE_X        equ     26B4h   ; the screen centre
CENTRE_Y        equ     26B6h
EYE_X           equ     26BAh   ; the eye position, three 32-bit coordinates
EYE_Y           equ     26BEh
EYE_Z           equ     26C2h
OBJ_SHIFT       equ     2880h   ; the current object's coordinate shift
OBJ_X           equ     2886h   ; the current object's origin, eye-relative and negated
OBJ_Y           equ     2888h
OBJ_Z           equ     288Ah

        .186

        extrn   _seg003_0:far
        extrn   _seg003_8:far
        extrn   _seg003_10:far
        extrn   seg048:far
        extrn   _seg003_5B0B:far
        extrn   _seg003_5B65:far
        extrn   seg051:far
        extrn   seg003:far
        extrn   _seg003_6146:far
        extrn   _seg003_6156:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public 'code'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _check_flat:near
        extrn   _code_pnt:near
        extrn   _instance_bank:near
        extrn   _instance_head:near
        extrn   _instance_pitch:near
        extrn   _load_xlate_rotate_pnt:near
        extrn   _mini_xlate_rotate_pnt:near
        extrn   _mxmul:near
        extrn   _overflow_handler_reg:near
        extrn   _seg004_5040:near
        extrn   _seg004_52FF:near
        extrn   _seg004_5374:near
        extrn   _self_modify:near
        extrn   _smooth_closure:near
        public  _clip_bot
        public  _clip_left
        public  _clip_overflow
        public  _clip_right
        public  _clip_top
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
        public  _do_animate_p
        public  _do_animate_b
        public  _do_animate_h
        public  _seg004_375E
        public  _do_setpat
        public  _do_putpat
        public  _do_usepat
        public  _seg004_3DC6
        public  draw_poly_buf_ptr
        public  modify_biasx
        public  modify_biasy
        public  modify_bx2
        public  modify_by2
        public  setcol_common

; seg004_27D0  (+27D0)
; Opcode for every unused table slot: `int 2`, then carries on with the next opcode. A
; program that reaches it is malformed; int 2 is NMI, so presumably a debugger catch.
_do_int2 proc near
        int     2
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_int2 endp

; seg004_27D8  (+27D8)
; Opcode 00h: end of a program or sub-program. Returns from the `call [bx+OPCODE_TABLE]` that
; started it (render_3d, do_sfcal, a sort or call opcode).
_do_eof proc near
        ret
_do_eof endp

; seg004_27D9  (+27D9)
; Opcode 08h, operand: point. Projects the point and plots one pixel through seg003
; (_seg003_0) when it is in front of the eye with no clip code (or always, when byte 286Eh
; is set).
_do_pntres proc near
        lodsw
        mov     di,ax
        add     di,PNT_X
        test    byte ptr ds:[286Eh],0FFh
        jne     short L27ED
        test    byte ptr [di+6],0FFh
        jne     short L2812
L27ED:
        mov     cx,word ptr [di+4]
        mov     ax,word ptr [di+2]
        imul    word ptr ds:[PROJ_Y]
        idiv    cx
        add     ax,word ptr ds:[CENTRE_Y]
        mov     bx,ax
        mov     ax,word ptr [di]
        imul    word ptr ds:[PROJ_X]
        idiv    cx
        add     ax,word ptr ds:[CENTRE_X]
        push    si
        call    far ptr _seg003_0
        pop     si
L2812:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_pntres endp

; seg004_2818  (+2818)
; Opcode 96h, operands: two points. Draws a line (seg003 _seg003_8, or _10 after
; clipping). Lines whose codes AND to nonzero are skipped; others are clipped in 3D here, one
; plane at a time, inline (FM has these sections as lclip_top, lclip_bot, lclip_left and
; lclip_right). The two entry points after the first dispatch (28EBh and 28EDh) are the
; divide-overflow recovery addresses stored at ss:[4D5h] while projecting each endpoint (FM
; line_overflow2): they drop the interrupt frame and clip that endpoint. The clipped endpoint
; is written to a scratch point at 15C9h or 15D1h.
_do_lnres proc near
        lodsw
        mov     di,ax
        lodsw
        mov     bx,ax
        mov     al,byte ptr [di+PNT_CODES]
        test    byte ptr [bx+PNT_CODES],al
        jne     short L287F
        push    si
        or      al,byte ptr [bx+PNT_CODES]
        jne     short L2885
        mov     bp,word ptr [di+PNT_Z]
        mov     ax,word ptr [di+PNT_X]
        imul    word ptr ds:[PROJ_X]
        idiv    bp
        add     ax,word ptr ds:[CENTRE_X]
        mov     cx,ax
        mov     ax,word ptr [di+PNT_Y]
        imul    word ptr ds:[PROJ_Y]
        idiv    bp
        add     ax,word ptr ds:[CENTRE_Y]
        mov     si,ax
        mov     di,bx
        mov     bp,word ptr [di+PNT_Z]
        mov     ax,word ptr [di+PNT_Y]
        imul    word ptr ds:[PROJ_Y]
        idiv    bp
        add     ax,word ptr ds:[CENTRE_Y]
        mov     bx,ax
        mov     ax,word ptr [di+PNT_X]
        imul    word ptr ds:[PROJ_X]
        idiv    bp
        add     ax,word ptr ds:[CENTRE_X]
        mov     dx,si
        call    far ptr _seg003_8
L287E:
        pop     si
L287F:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L2885:
        add     bx,PNT_X
        add     di,PNT_X
        or      al,al
        js      short L2909
L2891:
        mov     word ptr ss:[4D5h],28EBh
        mov     bp,word ptr [bx+4]
        mov     ax,word ptr [bx]
        imul    word ptr ds:[PROJ_X]
        idiv    bp
        add     ax,word ptr ds:[CENTRE_X]
        mov     cx,ax
        mov     ax,word ptr [bx+2]
        imul    word ptr ds:[PROJ_Y]
        idiv    bp
        add     ax,word ptr ds:[CENTRE_Y]
        mov     si,ax
        mov     word ptr ss:[4D5h],28EDh
        mov     bp,word ptr [di+4]
        mov     ax,word ptr [di+2]
        imul    word ptr ds:[PROJ_Y]
        idiv    bp
        add     ax,word ptr ds:[CENTRE_Y]
        mov     bx,ax
        mov     ax,word ptr [di]
        imul    word ptr ds:[PROJ_X]
        idiv    bp
        add     ax,word ptr ds:[CENTRE_X]
        mov     dx,si
        call    far ptr _seg003_10
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
        xchg    bx,di
        sti
        add     sp,6
        mov     al,byte ptr [di+6]
        test    al,1
        je      short L28FB
        jmp     L2A13
L28FB:
        test    al,2
        je      short L2902
        jmp     L2A8D
L2902:
        test    al,4
        jne     short L2919
        jmp     L2998
L2909:
        mov     al,byte ptr [di+6]
        or      al,al
        js      short L2915
        xchg    di,bx
        mov     al,byte ptr [di+6]
L2915:
        test    al,4
        je      short L2998
L2919:
        mov     word ptr ss:[4D5h],offset _overflow_handler_reg
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
        js      short L2998
        mov     cx,dx
        mov     ax,word ptr [di]
        sub     ax,word ptr [bx]
        imul    bp
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        adc     dx,0
        add     dx,word ptr [bx]
        mov     di,15D1h
        cmp     di,bx
        jne     short L2971
        mov     si,15C9h
L2971:
        mov     word ptr [di],dx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],cx
        jcxz    L297D
        jmp     short L2980
L297D:
        jmp     L287E
L2980:
        xor     al,al
        cmp     dx,cx
        jle     short L298A
        inc     al
        inc     al
L298A:
        neg     cx
        cmp     dx,cx
        jge     short L2992
        inc     al
L2992:
        mov     byte ptr [di+6],al
        jmp     L2891
L2998:
        mov     word ptr ss:[4D5h],offset _overflow_handler_reg
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
        mov     di,15D1h
        cmp     di,bx
        jne     short L29EC
        mov     si,15C9h
L29EC:
        mov     word ptr [di],dx
        mov     word ptr [di+2],cx
        jcxz    L29F5
        jmp     short L29F8
L29F5:
        jmp     L287E
L29F8:
        xor     al,al
        cmp     dx,cx
        jge     short L2A00
        inc     al
L2A00:
        neg     cx
        cmp     dx,cx
        jle     short L2A0A
        inc     al
        inc     al
L2A0A:
        mov     byte ptr [di+6],al
        mov     word ptr [di+4],cx
        jmp     L2891
L2A13:
        mov     word ptr ss:[4D5h],offset _overflow_handler_reg
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
        jns     short L2A8D
        mov     cx,dx
        mov     ax,word ptr [di+2]
        sub     ax,word ptr [bx+2]
        imul    bp
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        adc     dx,0
        add     dx,word ptr [bx+2]
        mov     di,15D1h
        cmp     di,bx
        jne     short L2A68
        mov     si,15C9h
L2A68:
        mov     word ptr [di],cx
        mov     word ptr [di+2],dx
        jcxz    L2A71
        jmp     short L2A74
L2A71:
        jmp     L287E
L2A74:
        xor     al,al
        cmp     dx,cx
        jge     short L2A7C
        or      al,8
L2A7C:
        neg     cx
        cmp     dx,cx
        jge     short L2A84
        or      al,4
L2A84:
        mov     byte ptr [di+6],al
        mov     word ptr [di+4],cx
        jmp     L2891
L2A8D:
        mov     word ptr ss:[4D5h],offset _overflow_handler_reg
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
        mov     di,15D1h
        cmp     di,bx
        jne     short L2ADE
        mov     si,15C9h
L2ADE:
        mov     word ptr [di],dx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],dx
        jcxz    L2AEA
        jmp     short L2AED
L2AEA:
        jmp     L287E
L2AED:
        xor     al,al
        cmp     cx,dx
        jle     short L2AF5
        mov     al,4
L2AF5:
        neg     dx
        cmp     cx,dx
        jge     short L2AFE
        mov     ax,8
L2AFE:
        mov     byte ptr [di+6],al
        jmp     L2891
_do_lnres endp

; seg004_2B04  (+2B04)
; Opcode BEh, operands: a, b. Model variables: *[b] = [a] (b holds the address to store to).
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
        jmp     word ptr [bx+OPCODE_TABLE]
_do_movei endp

; seg004_2B16  (+2B16)
; Opcode 02h, operands: address, value. Stores the constant in the model variable at address
; (GRIDDB.C's Clk(n) slots).
_do_movec proc near
        lodsw
        mov     di,ax
        movsw
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_movec endp

; seg004_2B20  (+2B20)
; Opcode 04h, operands: a, b. Model variables: [b] = [a].
_do_movem proc near
        lodsw
        mov     bx,ax
        mov     bx,word ptr [bx]
        lodsw
        xchg    bx,ax
        mov     word ptr [bx],ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_movem endp

; seg004_2B2F  (+2B2F)
; Opcode 0Ah, operands: offset, address. [address] = ss:[BP + offset] (a [bp+di] operand,
; so the stack segment), with BP as the caller left it.
_do_move_odata proc near
        lodsw
        xchg    di,ax
        mov     bx,word ptr [bp+di]
        lodsw
        xchg    bx,ax
        mov     word ptr [bx],ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_move_odata endp

; seg004_2B3D  (+2B3D)
; Opcode 82h, operands: count, first point, then count (x, z, y) coordinate triples. Each
; is offset by the object origin, shifted, rotated (load_xlate_rotate_pnt), clip-coded
; (code_pnt) and stored in consecutive points.
_do_multires proc near
        lodsw
        mov     word ptr ds:[271Ch],ax
        lodsw
        mov     di,ax
        add     di,PNT_X
        mov     word ptr ds:[271Eh],di
L2B4C:
        call    _load_xlate_rotate_pnt
        call    _code_pnt
        mov     di,word ptr ds:[271Eh]
        mov     word ptr [di],bx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],bp
        mov     byte ptr [di+6],al
        add     word ptr ds:[271Eh],8
        dec     word ptr ds:[271Ch]
        jne     L2B4C
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_multires endp

; seg004_2B72  (+2B72)
; Opcode B8h: do_multires with byte operands: a byte count, a byte point number, and byte
; coordinates (mini_xlate_rotate_pnt scales each by 32).
_do_minimulti proc near
        lodsb
        xor     ah,ah
        mov     word ptr ds:[271Ch],ax
        lodsb
        xor     ah,ah
        shl     ax,3
        mov     di,ax
        add     di,PNT_X
        mov     word ptr ds:[271Eh],di
L2B88:
        call    _mini_xlate_rotate_pnt
        call    _code_pnt
        mov     di,word ptr ds:[271Eh]
        mov     word ptr [di],bx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],bp
        mov     byte ptr [di+6],al
        add     word ptr ds:[271Eh],8
        dec     word ptr ds:[271Ch]
        jne     L2B88
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_minimulti endp

; seg004_2BAE  (+2BAE)
; Opcode CCh, operands: count, then count (point, shade byte) pairs, padded to a word. Sets
; each point's shade (+7) for Gouraud polygons.
_do_setshade proc near
        lodsw
        mov     cx,ax
        mov     di,PNT_X
L2BB4:
        lodsw
        mov     bx,ax
        lodsb
        mov     byte ptr [bx+di+7],al
        loop    L2BB4
        inc     si
        and     si,-2
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_setshade endp

; seg004_2BC7  (+2BC7)
; Opcode D4h (UW-specific), operands: count, the address of a model variable, then count
; (point, shade byte) pairs, padded to a word. Writes the variable's low byte to seg004:6950h
; (PGCACHE's first byte, which seg003 reads) and sets each point's shade to its byte plus the
; shade bias at 2934h, at most 0Eh.
_do_uwshade proc near
        lodsw
        mov     cx,ax
        lodsw
        mov     bx,ax
        mov     dx,word ptr [bx]
        mov     byte ptr cs:[6950h],dl
        mov     dx,word ptr ds:[2934h]
        mov     di,PNT_X
L2BDB:
        lodsw
        mov     bx,ax
        lodsb
        add     al,dl
        cmp     al,0Eh
        jbe     short L2BE7
        mov     al,0Eh
L2BE7:
        mov     byte ptr [bx+di+7],al
        loop    L2BDB
        inc     si
        and     si,-2
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
        lodsw
        mov     bx,word ptr [si+6]
        mov     byte ptr [bx+PNT_SHADE],al
_do_uwshade endp

; seg004_2BFE  (+2BFE)
; Opcode 7Ah, operands: (x, z, y), point. Transforms one coordinate triple and stores it
; with its clip code. The unlabelled entry just before it (2BF6h) is FM's do_defresg: it
; takes a shade word first and sets the point's shade from it, then falls in here; the opcode
; table has no entry for it, in UW1 as in UW2.
_do_defres proc near
        call    _load_xlate_rotate_pnt
        lodsw
        mov     di,ax
        mov     word ptr [di+PNT_X],bx
        mov     word ptr [di+PNT_Y],cx
        mov     word ptr [di+PNT_Z],bp
        call    _code_pnt
        mov     byte ptr [di+PNT_CODES],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_defres endp

; seg004_2C1D  (+2C1D)
; Opcode B6h: do_defres with byte coordinates and a byte point number (GRIDDB's SetPnt
; records).
_do_minires proc near
        call    _mini_xlate_rotate_pnt
        lodsb
        xor     ah,ah
        shl     ax,3
        mov     di,ax
        mov     word ptr [di+PNT_X],bx
        mov     word ptr [di+PNT_Y],cx
        mov     word ptr [di+PNT_Z],bp
        call    _code_pnt
        mov     byte ptr [di+PNT_CODES],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_minires endp

; seg004_2C41  (+2C41)
; Opcode 7Ch, operand: point. Starts a polygon: copies the point to the vertex buffer at
; 309h and sets the codes OR and AND from it.
_do_strres proc near
        lodsw
        mov     dx,si
        mov     si,ax
        add     si,PNT_X
        mov     di,VBUF0
        movsw
        movsw
        movsw
        lodsb
        stosw
        mov     byte ptr ds:[CLIP_AND],al
        mov     byte ptr ds:[CLIP_OR],al
        mov     word ptr ds:[VB_END],di
        mov     si,dx
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_strres endp

; seg004_2C64  (+2C64)
; Opcode 8Eh, operand: point. Appends a point to the polygon do_strres started (falls into
; the dispatch; do_closure draws it).
_do_cntres proc near
        lodsw
        mov     dx,si
        mov     si,ax
        add     si,PNT_X
        mov     di,word ptr ds:[VB_END]
        movsw
        movsw
        movsw
        lodsb
        stosw
        and     byte ptr ds:[CLIP_AND],al
        or      byte ptr ds:[CLIP_OR],al
        mov     word ptr ds:[VB_END],di
        mov     si,dx
L2C84:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_cntres endp

; seg004_2C8A  (+2C8A)
; Opcode 9Ch: a rod, a quadrilateral of fixed width between two points, facing the eye. For
; each end: operand 0 then a point (that end is a single vertex), or a half-width then a
; point (two vertices, offset sideways by the width rotated through 26D0h/26D2h, which
; scale_matrix computes). Then draws it with do_closure.
_do_rod proc near
        mov     byte ptr ds:[CLIP_AND],0FFh
        mov     byte ptr ds:[CLIP_OR],0
        lodsw
        or      ax,ax
        jne     short L2CB4
        mov     di,VBUF0
        lodsw
        push    si
        mov     si,ax
        add     si,PNT_X
        movsw
        movsw
        movsw
        lodsw
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
        stosw
        pop     si
        jmp     short L2D11
L2CB4:
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        shl     ax,cl
        mov     cx,ax
        imul    word ptr ds:[26D2h]
        mov     ax,cx
        mov     cx,dx
        imul    word ptr ds:[26D0h]
        mov     di,VBUF0
        lodsw
        push    si
        mov     si,ax
        add     si,PNT_X
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
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
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
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
        stosw
        pop     si
L2D11:
        lodsw
        or      ax,ax
        jne     short L2D2E
        lodsw
        push    si
        mov     si,ax
        add     si,PNT_X
        movsw
        movsw
        movsw
        lodsw
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
        stosw
        pop     si
        jmp     short L2D88
L2D2E:
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        shl     ax,cl
        mov     cx,ax
        imul    word ptr ds:[26D2h]
        mov     ax,cx
        mov     cx,dx
        imul    word ptr ds:[26D0h]
        lodsw
        push    si
        mov     si,ax
        add     si,PNT_X
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
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
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
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
        stosw
        pop     si
L2D88:
        mov     word ptr ds:[VB_END],di
        jmp     _do_closure
_do_rod endp

; seg004_2D8F  (+2D8F)
; do_rod for a level view (check_flat installs it in the opcode table): the sideways
; offset is along x only, using 272Ch.
_flat_rod proc near
        mov     byte ptr ds:[CLIP_AND],0FFh
        mov     byte ptr ds:[CLIP_OR],0
        lodsw
        or      ax,ax
        jne     short L2DB9
        mov     di,VBUF0
        lodsw
        push    si
        mov     si,ax
        add     si,PNT_X
        movsw
        movsw
        movsw
        lodsw
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
        stosw
        pop     si
        jmp     short L2E06
L2DB9:
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        shl     ax,cl
        imul    word ptr ds:[272Ch]
        mov     di,VBUF0
        lodsw
        push    si
        mov     si,ax
        add     si,PNT_X
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
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
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
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
        stosw
        pop     si
L2E06:
        lodsw
        or      ax,ax
        jne     short L2E23
        lodsw
        push    si
        mov     si,ax
        add     si,PNT_X
        movsw
        movsw
        movsw
        lodsw
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
        stosw
        pop     si
        jmp     short L2E6D
L2E23:
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        shl     ax,cl
        imul    word ptr ds:[272Ch]
        lodsw
        push    si
        mov     si,ax
        add     si,PNT_X
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
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
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
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
        stosw
        pop     si
L2E6D:
        mov     word ptr ds:[VB_END],di
        jmp     short _do_closure
_flat_rod endp

; seg004_2E73  (+2E73)
; Opcodes 22h and 7Eh, operands: count, then count points. Builds a polygon in the vertex
; buffer, with the codes OR in BH and AND in BL; if they AND to nonzero it is skipped,
; otherwise it falls into do_closure to be drawn. (System Shock: do_polyres.)
_do_polyres proc near
        lodsw
        mov     cx,ax
        mov     di,VBUF0
        xor     bx,bx
        not     bl
        mov     bp,PNT_X
L2E80:
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
        loop    L2E80
        je      short L2EA0
        mov     word ptr ds:[2884h],0
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L2EA0:
        mov     byte ptr ds:[CLIP_OR],bh
        mov     byte ptr ds:[CLIP_AND],bl
        mov     word ptr ds:[VB_END],di
_do_polyres endp

; seg004_2EAC  (+2EAC)
; Opcode 80h: draws the polygon in the vertex buffer. Gouraud polygons (15F6h = the seg003
; Gouraud routine) go to SMOOTH.ASM's smooth_closure. Otherwise: AND of codes nonzero, skip;
; OR zero, project each vertex (x*[26B2]/z + centre, y*[26B0]/z + centre) to seg048:415Ch
; and call seg003's polygon routine (_seg003_5B0B) with BP = the fill routine and CX =
; the vertex count; behind the eye (80h), clip in 3D first (must_clip_3d, FM); otherwise
; (clip_needed, FM) project with overflow checks and fall back to do_3d_clip if a screen
; coordinate overflows. The labels inside are FM names; the add immediates at modify_biasx,
; modify_biasy, modify_bx2 and modify_by2 are the screen centre, patched by render_3d.
_do_closure proc near
        mov     di,VBUF0
        mov     word ptr ds:[VB_START],di
        cmp     word ptr ds:[POLY_FILL],offset _seg003_6146
        jne     short draw_poly_buf_ptr
        jmp     _smooth_closure
draw_poly_buf_ptr:
        mov     word ptr ds:[2884h],0
        test    byte ptr ds:[CLIP_AND],0FFh
        je      short L2ECE
        jmp     L2C84
L2ECE:
        test    byte ptr ds:[CLIP_OR],0FFh
        jne     short clip_needed
L2ED5:
        push    si
        mov     ax,seg seg048
        mov     es,ax
        mov     di,415Ch
        mov     bx,word ptr ds:[VB_END]
        mov     si,word ptr ds:[VB_START]
        mov     cx,word ptr ds:[PROJ_X]
L2EEA:
        lodsw
        mov     bp,word ptr [si+2]
        imul    cx
        idiv    bp
modify_biasx:
        add     ax,4D2h
        stosw
        mov     ax,word ptr [si]
        imul    word ptr ds:[PROJ_Y]
        idiv    bp
modify_biasy:
        add     ax,4D2h
        stosw
        add     si,6
        cmp     si,bx
        jne     L2EEA
        sub     bx,word ptr ds:[VB_START]
        shr     bx,1
        shr     bx,1
        shr     bx,1
        mov     cx,bx
        inc     word ptr ds:[2884h]
        mov     bp,word ptr ds:[POLY_FILL]
        call    far ptr _seg003_5B0B
        mov     ax,ds
        mov     es,ax
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
clip_needed:
        js      short must_clip_3d
        push    si
L2F30:
        mov     word ptr ss:[4D5h],offset _clip_overflow
        mov     ax,seg seg048
        mov     es,ax
        mov     di,415Ch
        mov     bx,word ptr ds:[VB_END]
        mov     si,word ptr ds:[VB_START]
        mov     cx,word ptr ds:[PROJ_X]
L2F4B:
        lodsw
        mov     bp,word ptr [si+2]
        imul    cx
        idiv    bp
modify_bx2:
        add     ax,4D2h
        jno     short L2F5B
        jmp     _do_3d_clip
L2F5B:
        stosw
        mov     ax,word ptr [si]
        imul    word ptr ds:[PROJ_Y]
        idiv    bp
modify_by2:
        add     ax,4D2h
        jno     short L2F6C
        jmp     _do_3d_clip
L2F6C:
        stosw
        add     si,6
        cmp     si,bx
        jne     L2F4B
        mov     cx,bx
        sub     cx,word ptr ds:[VB_START]
        shr     cx,1
        shr     cx,1
        shr     cx,1
        inc     word ptr ds:[2884h]
        mov     bp,word ptr ds:[POLY_FILL]
        call    far ptr _seg003_5B0B
        mov     ax,ds
        mov     es,ax
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
must_clip_3d:
        mov     word ptr ss:[4D5h],offset _overflow_handler_reg
        test    byte ptr ds:[CLIP_OR],4
        je      short L2FC1
        push    si
        call    _clip_top
        pop     si
        test    byte ptr ds:[CLIP_AND],0FFh
        jne     short L3027
        test    byte ptr ds:[CLIP_OR],0FFh
        js      short L2FC1
        jne     short L2FBE
        jmp     L2ED5
L2FBE:
        jmp     clip_needed
L2FC1:
        test    byte ptr ds:[CLIP_OR],8
        je      short L2FE3
        push    si
        call    _clip_bot
        pop     si
        test    byte ptr ds:[CLIP_AND],0FFh
        jne     short L3027
        test    byte ptr ds:[CLIP_OR],0FFh
        js      short L2FE3
        jne     short L2FE0
        jmp     L2ED5
L2FE0:
        jmp     clip_needed
L2FE3:
        test    byte ptr ds:[CLIP_OR],1
        je      short L3005
        push    si
        call    _clip_left
        pop     si
        test    byte ptr ds:[CLIP_AND],0FFh
        jne     short L3027
        test    byte ptr ds:[CLIP_OR],0FFh
        js      short L3005
        jne     short L3002
        jmp     L2ED5
L3002:
        jmp     clip_needed
L3005:
        test    byte ptr ds:[CLIP_OR],2
        je      short L3027
        push    si
        call    _clip_right
        pop     si
        test    byte ptr ds:[CLIP_AND],0FFh
        jne     short L3027
        test    byte ptr ds:[CLIP_OR],0FFh
        js      short L3027
        jne     short L3024
        jmp     L2ED5
L3024:
        jmp     clip_needed
L3027:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_closure endp

; seg004_302D  (+302D)
; Opcode 1Ah, operand: address. Calls a near routine in this segment and continues.
_do_asmcal proc near
        lodsw
        call    ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_asmcal endp

; seg004_3036  (+3036)
; Opcode 12h, operand: offset. Runs the sub-program at SI + offset (it ends with do_eof),
; then continues after the operand. (System Shock: do_sfcal.)
_do_sfcal proc near
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
L3041:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_sfcal endp

; seg004_3047  (+3047)
; Opcode 16h, operands: skip, address, value. If [address] >= value, skips `skip` bytes of
; program; otherwise continues (through do_sfcal's dispatch at L3041).
_do_ifge proc near
        lodsw
        mov     cx,ax
        lodsw
        mov     bx,ax
        lodsw
        cmp     word ptr [bx],ax
        jl      L3041
        add     si,cx
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_ifge endp

; seg004_305A  (+305A)
; Opcode 14h: as do_ifge, skipping when [address] <= value.
_do_ifle proc near
        lodsw
        mov     cx,ax
        lodsw
        mov     bx,ax
        lodsw
        cmp     word ptr [bx],ax
        jg      L3041
        add     si,cx
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_ifle endp

; seg004_306D  (+306D)
; Opcode 6Ch, operands: address, value, offset a, offset b. Runs two sub-programs in an
; order chosen by a variable: a then b when [address] >= value, else b then a (the shared
; tail L3090 runs the second-listed first). The model's own back-to-front order switch.
_do_sortvar proc near
        lodsw
        xchg    bx,ax
        lodsw
        cmp     word ptr [bx],ax
        jl      short L3090
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L3090:
        lodsw
        add     ax,si
        mov     bx,ax
        lodsw
        push    si
        push    bx
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_sortvar endp

; seg004_30AE  (+30AE)
; Opcode 06h, operands: three (n, p) word pairs for x, y and z, then two sub-program offsets
; a and b. Computes the sign of the sum of n * (p + origin) over the axes (the origin at
; 2886h..288Ah is the negated eye-relative position, so this is the side of the plane the
; eye is on) and runs both sub-programs, a then b when it is non-negative, b then a
; otherwise: a BSP split, so that the half nearer the eye is drawn last (System Shock:
; do_sortnorm).
_do_sortnorm proc near
        lodsw
        mov     cx,ax
        lodsw
        add     ax,word ptr ds:[OBJ_X]
        imul    cx
        mov     cx,dx
        mov     bp,ax
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[OBJ_Y]
        imul    di
        add     bp,ax
        adc     cx,dx
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[OBJ_Z]
        imul    di
        add     bp,ax
        adc     cx,dx
        js      L3090
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_sortnorm endp

; seg004_30F6  (+30F6)
; Opcode 0Ch: do_sortnorm with no x term in the normal.
_do_sortnorm_x0 proc near
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[OBJ_Y]
        imul    di
        mov     bp,ax
        mov     cx,dx
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[OBJ_Z]
        imul    di
        add     bp,ax
        adc     cx,dx
        jns     short L3117
        jmp     L3090
L3117:
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_sortnorm_x0 endp

; seg004_3133  (+3133)
; Opcode 0Eh: do_sortnorm with no y term.
_do_sortnorm_y0 proc near
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[OBJ_X]
        imul    di
        mov     bp,ax
        mov     cx,dx
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[OBJ_Z]
        imul    di
        add     bp,ax
        adc     cx,dx
        jns     short L3154
        jmp     L3090
L3154:
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_sortnorm_y0 endp

; seg004_3170  (+3170)
; Opcode 10h: do_sortnorm with no z term.
_do_sortnorm_z0 proc near
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[OBJ_X]
        imul    di
        mov     bp,ax
        mov     cx,dx
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[OBJ_Y]
        imul    di
        add     bp,ax
        adc     cx,dx
        jns     short L3191
        jmp     L3090
L3191:
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_sortnorm_z0 endp

; seg004_31AD  (+31AD)
; Opcode 18h, operands: three 32-bit world coordinates. Sets the object origin (2886h..
; 288Ah) to the negated position relative to the eye. If a relative coordinate does not fit
; 16 bits, sets 2882h = -1 and stops, with the axes before it already updated. Calls
; self_modify.
_do_org proc near
        mov     word ptr ds:[2882h],0
        lodsw
        sub     ax,word ptr ds:[EYE_X]
        mov     bx,word ptr [si]
        inc     si
        inc     si
        sbb     bx,word ptr ds:[EYE_X+2]
        cwd
        cmp     bx,dx
        jne     short L3201
        neg     ax
        mov     word ptr ds:[OBJ_X],ax
        lodsw
        sub     ax,word ptr ds:[EYE_Y]
        mov     bx,word ptr [si]
        inc     si
        inc     si
        sbb     bx,word ptr ds:[EYE_Y+2]
        cwd
        cmp     bx,dx
        jne     short L3201
        neg     ax
        mov     word ptr ds:[OBJ_Y],ax
        lodsw
        sub     ax,word ptr ds:[EYE_Z]
        mov     bx,word ptr [si]
        inc     si
        inc     si
        sbb     bx,word ptr ds:[EYE_Z+2]
        cwd
        cmp     bx,dx
        jne     short L3201
        neg     ax
        mov     word ptr ds:[OBJ_Z],ax
        call    _self_modify
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L3201:
        mov     word ptr ds:[2882h],0FFFFh
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_org endp

; seg004_320D  (+320D)
; Opcode 84h, operands: count, then count points. A visibility test: if all the points share
; a clip-code bit (entirely outside one plane) the program ends here (jumps to do_eof's ret);
; if none has a code, calls set_accept (a no-op in UW2); otherwise continues.
_do_hull proc near
        lodsw
        mov     cx,ax
        dec     cx
        mov     bx,PNT_CODES
        lodsw
        mov     di,ax
        mov     dl,byte ptr [bx+di]
        mov     bp,dx
L321B:
        lodsw
        mov     di,ax
        or      dl,byte ptr [bx+di]
        and     bp,word ptr [bx+di]
        loop    L321B
        je      short L3229
        jmp     _do_eof
L3229:
        or      dl,dl
        jne     short L3232
        mov     al,0FFh
        call    _seg004_5040
L3232:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_hull endp

; seg004_3238  (+3238)
; flat_ variant of do_y_rel (opcode 88h when the view is level): the y offset adds straight to
; view y, halved; recomputes the clip code bits 4 and 8 only.
_flat_y_rel proc near
        lodsw
        mov     bp,ax
        lodsw
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        shl     ax,cl
        sar     ax,1
        mov     cx,ax
        lodsw
        xchg    bp,si
        mov     di,PNT_X
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
        jle     short L327C
        add     cx,dx
        jge     short L3271
        or      al,0Ch
        stosb
        mov     si,bp
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L3271:
        or      al,4
        stosb
        mov     si,bp
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L327C:
        add     cx,dx
        jge     short L3282
        or      al,8
L3282:
        stosb
        mov     si,bp
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_flat_y_rel endp

; seg004_328B  (+328B)
; Opcode 88h (also 1Ch), operands: source point, dy, destination point. New point = source +
; dy along the model's y axis (rotated through the matrix's y row), clip-coded. (System Shock:
; do_y_rel.) The three entries after it are FM's do_animate_p, do_animate_b and
; do_animate_h (same order, five bytes apart; UW2's are seven apart, and UW2's opcode table
; has no entry for them where UW1's has them as opcodes 6Eh, 74h and 76h): each loads BP with
; instance_pitch, instance_bank or instance_head, moves an animated angle variable ([di+2])
; towards its target ([di]) by its rate ([di+4]) times
; the frame time at ss:[9B0h], offsets the origin, and runs the sub-program [di+6] rotated by
; that angle, restoring the matrix and origin afterwards.
_do_y_rel proc near
        lodsw
        mov     di,ax
        lodsw
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        shl     ax,cl
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        mov     di,ax
        imul    word ptr ds:[MAT_YX]
        add     bx,dx
        mov     ax,word ptr ds:[MAT_YY]
        imul    di
        add     cx,dx
        mov     ax,word ptr ds:[MAT_YZ]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        mov     word ptr [di+PNT_X],bx
        mov     word ptr [di+PNT_Y],cx
        mov     word ptr [di+PNT_Z],bp
        call    _code_pnt
        mov     byte ptr [di+PNT_CODES],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_y_rel endp

; seg004_32D3  (+32D3)
; do_animate_p (FM name), opcode 6Eh in UW1: see do_y_rel's comment.
_do_animate_p proc near
        mov     bp,offset _instance_pitch
        jmp     short L32E0
_do_animate_p endp

; seg004_32D8  (+32D8)
; do_animate_b (FM name), opcode 74h in UW1: see do_y_rel's comment.
_do_animate_b proc near
        mov     bp,offset _instance_bank
        jmp     short L32E0
_do_animate_b endp

; seg004_32DD  (+32DD)
; do_animate_h (FM name), opcode 76h in UW1: see do_y_rel's comment.
_do_animate_h proc near
        mov     bp,offset _instance_head
L32E0:
        lodsw
        mov     di,ax
        mov     bx,word ptr [di+2]
        cmp     bx,word ptr [di]
        je      short L331C
        jg      short L3305
        mov     ax,word ptr [di+4]
        imul    word ptr ss:[9B0h]
        shl     ax,1
        rcl     dx,1
        add     bx,dx
        cmp     bx,word ptr [di]
        jle     short L3300
        mov     bx,word ptr [di]
L3300:
        mov     word ptr [di+2],bx
        jmp     short L331C
L3305:
        mov     ax,word ptr [di+4]
        imul    word ptr ss:[9B0h]
        shl     ax,1
        rcl     dx,1
        sub     bx,dx
        cmp     bx,word ptr [di]
        jge     short L3319
        mov     bx,word ptr [di]
L3319:
        mov     word ptr [di+2],bx
L331C:
        push    word ptr ds:[OBJ_X]
        push    word ptr ds:[OBJ_Y]
        push    word ptr ds:[OBJ_Z]
        lodsw
        sub     word ptr ds:[OBJ_X],ax
        lodsw
        sub     word ptr ds:[OBJ_Y],ax
        lodsw
        sub     word ptr ds:[OBJ_Z],ax
        push    si
        mov     si,word ptr [di+6]
        or      bx,bx
        je      short L33AD
        push    word ptr ds:[MAT_XX]
        push    word ptr ds:[MAT_YX]
        push    word ptr ds:[MAT_ZX]
        push    word ptr ds:[MAT_XY]
        push    word ptr ds:[MAT_YY]
        push    word ptr ds:[MAT_ZY]
        push    word ptr ds:[MAT_XZ]
        push    word ptr ds:[MAT_YZ]
        push    word ptr ds:[MAT_ZZ]
        call    bp
        push    si
        call    _check_flat
        pop     si
        call    _self_modify
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     word ptr ds:[MAT_ZZ]
        pop     word ptr ds:[MAT_YZ]
        pop     word ptr ds:[MAT_XZ]
        pop     word ptr ds:[MAT_ZY]
        pop     word ptr ds:[MAT_YY]
        pop     word ptr ds:[MAT_XY]
        pop     word ptr ds:[MAT_ZX]
        pop     word ptr ds:[MAT_YX]
        pop     word ptr ds:[MAT_XX]
        pop     si
        pop     word ptr ds:[OBJ_Z]
        pop     word ptr ds:[OBJ_Y]
        pop     word ptr ds:[OBJ_X]
        call    _self_modify
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L33AD:
        call    _self_modify
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        pop     word ptr ds:[OBJ_Z]
        pop     word ptr ds:[OBJ_Y]
        pop     word ptr ds:[OBJ_X]
        call    _self_modify
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_animate_h endp

; seg004_33CC  (+33CC)
; Opcode 26h, operands: destination point, source point. Copies the point (7 bytes: x, y, z,
; codes).
_do_movres proc near
        lodsw
        mov     di,ax
        lodsw
        xchg    si,ax
        add     di,PNT_X
        add     si,PNT_X
        movsw
        movsw
        movsw
        movsb
        mov     si,ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_movres endp

; seg004_33E5  (+33E5)
; flat_ variant of do_x_rel: with the view level only the matrix's x and z terms are used and
; the clip code is recomputed inline.
_flat_x_rel proc near
        lodsw
        mov     di,ax
        lodsw
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        shl     ax,cl
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        mov     di,ax
        imul    word ptr ds:[MAT_XX]
        add     bx,dx
        mov     ax,di
        imul    word ptr ds:[MAT_XZ]
        lodsw
        mov     di,ax
        mov     word ptr [di+PNT_X],bx
        mov     word ptr [di+PNT_Y],cx
        xor     al,al
        add     bp,dx
        mov     word ptr [di+PNT_Z],bp
        jg      short L3420
        mov     al,80h
L3420:
        cmp     bx,bp
        jle     short L3426
        inc     ax
        inc     ax
L3426:
        cmp     cx,bp
        jle     short L342C
        add     al,4
L342C:
        add     bx,bp
        jge     short L3431
        inc     ax
L3431:
        add     cx,bp
        jge     short L3437
        add     al,8
L3437:
        mov     byte ptr [di+PNT_CODES],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_flat_x_rel endp

; seg004_3441  (+3441)
; Opcodes 86h and 2Ah, operands: source point, dx, destination point. New point = source +
; dx along the model's x axis, clip-coded. (System Shock: do_x_rel.)
_do_x_rel proc near
        lodsw
        mov     di,ax
        lodsw
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        shl     ax,cl
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        mov     di,ax
        imul    word ptr ds:[MAT_XX]
        add     bx,dx
        mov     ax,word ptr ds:[MAT_XY]
        imul    di
        add     cx,dx
        mov     ax,word ptr ds:[MAT_XZ]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        mov     word ptr [di+PNT_X],bx
        mov     word ptr [di+PNT_Y],cx
        mov     word ptr [di+PNT_Z],bp
        call    _code_pnt
        mov     byte ptr [di+PNT_CODES],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_x_rel endp

; seg004_3489  (+3489)
; flat_ variant of do_z_rel.
_flat_z_rel proc near
        lodsw
        mov     di,ax
        lodsw
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        shl     ax,cl
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        mov     di,ax
        imul    word ptr ds:[MAT_ZX]
        add     bx,dx
        mov     ax,word ptr ds:[MAT_ZZ]
        imul    di
        lodsw
        mov     di,ax
        mov     word ptr [di+PNT_X],bx
        mov     word ptr [di+PNT_Y],cx
        xor     al,al
        add     bp,dx
        mov     word ptr [di+PNT_Z],bp
        jg      short L34C3
        mov     al,80h
L34C3:
        cmp     bx,bp
        jle     short L34C9
        inc     ax
        inc     ax
L34C9:
        cmp     cx,bp
        jle     short L34CF
        add     al,4
L34CF:
        add     bx,bp
        jge     short L34D4
        inc     ax
L34D4:
        add     cx,bp
        jge     short L34DA
        add     al,8
L34DA:
        mov     byte ptr [di+PNT_CODES],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_flat_z_rel endp

; seg004_34E4  (+34E4)
; Opcodes 8Ah and 2Ch, operands: source point, dz, destination point. As do_x_rel along z.
_do_z_rel proc near
        lodsw
        mov     di,ax
        lodsw
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        shl     ax,cl
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        mov     di,ax
        imul    word ptr ds:[MAT_ZX]
        add     bx,dx
        mov     ax,word ptr ds:[MAT_ZY]
        imul    di
        add     cx,dx
        mov     ax,word ptr ds:[MAT_ZZ]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        mov     word ptr [di+PNT_X],bx
        mov     word ptr [di+PNT_Y],cx
        mov     word ptr [di+PNT_Z],bp
        call    _code_pnt
        mov     byte ptr [di+PNT_CODES],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_z_rel endp

; seg004_352C  (+352C)
; Opcode 92h, operands: dx, dz, source point, destination point. Offset along two axes.
_do_xz_rel proc near
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[MAT_XX]
        mov     bx,dx
        mov     ax,word ptr ds:[MAT_XZ]
        imul    di
        mov     bp,dx
        mov     ax,word ptr ds:[MAT_XY]
        imul    di
        lodsw
        shl     ax,cl
        mov     cx,dx
        mov     di,ax
        imul    word ptr ds:[MAT_ZX]
        add     bx,dx
        mov     ax,word ptr ds:[MAT_ZY]
        imul    di
        add     cx,dx
        mov     ax,word ptr ds:[MAT_ZZ]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        add     bx,word ptr [di+PNT_X]
        add     cx,word ptr [di+PNT_Y]
        add     bp,word ptr [di+PNT_Z]
        lodsw
        mov     di,ax
        mov     word ptr [di+PNT_X],bx
        mov     word ptr [di+PNT_Y],cx
        mov     word ptr [di+PNT_Z],bp
        call    _code_pnt
        mov     byte ptr [di+PNT_CODES],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_xz_rel endp

; seg004_358D  (+358D)
; Opcode 90h, operands: dx, dy, source point, destination point.
_do_xy_rel proc near
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[MAT_XX]
        mov     bx,dx
        mov     ax,word ptr ds:[MAT_XZ]
        imul    di
        mov     bp,dx
        mov     ax,word ptr ds:[MAT_XY]
        imul    di
        lodsw
        shl     ax,cl
        mov     cx,dx
        mov     di,ax
        imul    word ptr ds:[MAT_YX]
        add     bx,dx
        mov     ax,word ptr ds:[MAT_YY]
        imul    di
        add     cx,dx
        mov     ax,word ptr ds:[MAT_YZ]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        add     bx,word ptr [di+PNT_X]
        add     cx,word ptr [di+PNT_Y]
        add     bp,word ptr [di+PNT_Z]
        lodsw
        mov     di,ax
        mov     word ptr [di+PNT_X],bx
        mov     word ptr [di+PNT_Y],cx
        mov     word ptr [di+PNT_Z],bp
        call    _code_pnt
        mov     byte ptr [di+PNT_CODES],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_xy_rel endp

; seg004_35EE  (+35EE)
; Opcode 94h, operands: dy, dz, source point, destination point. (Note the operand order: the
; first operand is multiplied by the z row and the second by the y row, as in the code.)
_do_yz_rel proc near
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[MAT_ZX]
        mov     bx,dx
        mov     ax,word ptr ds:[MAT_ZZ]
        imul    di
        mov     bp,dx
        mov     ax,word ptr ds:[MAT_ZY]
        imul    di
        lodsw
        shl     ax,cl
        mov     cx,dx
        mov     di,ax
        imul    word ptr ds:[MAT_YX]
        add     bx,dx
        mov     ax,word ptr ds:[MAT_YY]
        imul    di
        add     cx,dx
        mov     ax,word ptr ds:[MAT_YZ]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        add     bx,word ptr [di+PNT_X]
        add     cx,word ptr [di+PNT_Y]
        add     bp,word ptr [di+PNT_Z]
        lodsw
        mov     di,ax
        mov     word ptr [di+PNT_X],bx
        mov     word ptr [di+PNT_Y],cx
        mov     word ptr [di+PNT_Z],bp
        call    _code_pnt
        mov     byte ptr [di+PNT_CODES],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_yz_rel endp

; seg004_364F  (+364F)
; flat_ variant of do_xz_rel.
_flat_xz_rel proc near
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[MAT_XX]
        mov     bx,dx
        mov     ax,word ptr ds:[MAT_XZ]
        imul    di
        mov     bp,dx
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[MAT_ZX]
        add     bx,dx
        mov     ax,word ptr ds:[MAT_ZZ]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        add     bx,word ptr [di+PNT_X]
        add     bp,word ptr [di+PNT_Z]
        mov     cx,word ptr [di+PNT_Y]
        lodsw
        mov     di,ax
        mov     word ptr [di+PNT_X],bx
        mov     word ptr [di+PNT_Y],cx
        mov     word ptr [di+PNT_Z],bp
        call    _code_pnt
        mov     byte ptr [di+PNT_CODES],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_flat_xz_rel endp

; seg004_36A2  (+36A2)
; flat_ variant of do_xy_rel.
_flat_xy_rel proc near
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[MAT_XX]
        mov     bx,dx
        mov     ax,word ptr ds:[MAT_XZ]
        imul    di
        mov     bp,dx
        lodsw
        shl     ax,cl
        sar     ax,1
        mov     cx,ax
        lodsw
        mov     di,ax
        add     bx,word ptr [di+PNT_X]
        add     cx,word ptr [di+PNT_Y]
        add     bp,word ptr [di+PNT_Z]
        lodsw
        mov     di,ax
        mov     word ptr [di+PNT_X],bx
        mov     word ptr [di+PNT_Y],cx
        mov     word ptr [di+PNT_Z],bp
        call    _code_pnt
        mov     byte ptr [di+PNT_CODES],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_flat_xy_rel endp

; seg004_36EA  (+36EA)
; flat_ variant of do_yz_rel.
_flat_yz_rel proc near
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[MAT_ZX]
        mov     bx,dx
        mov     ax,word ptr ds:[MAT_ZZ]
        imul    di
        mov     bp,dx
        lodsw
        shl     ax,cl
        sar     ax,1
        mov     cx,ax
        lodsw
        mov     di,ax
        add     bx,word ptr [di+PNT_X]
        add     cx,word ptr [di+PNT_Y]
        add     bp,word ptr [di+PNT_Z]
        lodsw
        mov     di,ax
        mov     word ptr [di+PNT_X],bx
        mov     word ptr [di+PNT_Y],cx
        mov     word ptr [di+PNT_Z],bp
        call    _code_pnt
        mov     byte ptr [di+PNT_CODES],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_flat_yz_rel endp

; seg004_3732  (+3732)
; Opcode 2Eh, operand: colour. Selects flat filling (15F6h = 438Dh, 15F8h = 4134h, seg003
; offsets) and sets the colour through seg003's _seg003_5B65 (with DS = seg048). Its
; second label, setcol_common (FM), is shared by do_setcolv, do_uwsurf and PGCACHE.
_do_setcolor proc near
        lodsw
setcol_common:
        mov     bx,438Dh
        mov     word ptr ds:[POLY_FILL],bx
        mov     bx,4134h
        mov     word ptr ds:[POLY_UFILL],bx
        mov     bx,seg seg048
        mov     ds,bx
        call    far ptr _seg003_5B65
        mov     bp,seg seg051
        mov     ds,bp
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_setcolor endp

; seg004_3756  (+3756)
; Opcode 5Ch, operands: address, colour. Colour = [address] + colour, then setcol_common.
_do_setcolv proc near
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr [di]
        jmp     setcol_common
_do_setcolv endp

; seg004_375E  (+375E)
; UW1 only: opcode 24h. Takes two points and a step count from its operands and sets up
; per-step increments at 26ECh..26F6h (and 286Dh from the first point's clip codes); not
; traced further.
_seg004_375E proc near
        xor     ax,ax
        mov     di,26FAh
        stosw
        stosw
        stosw
        lodsw
        mov     di,ax
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        mov     al,byte ptr [di+PNT_CODES]
        mov     byte ptr ds:[286Dh],al
        lodsw
        mov     di,ax
        lodsw
        mov     word ptr ds:[26F8h],ax
        lodsw
        push    si
        mov     si,ax
        mov     ax,word ptr [di+PNT_X]
        sub     ax,bx
        cwd
        idiv    si
        mov     word ptr ds:[26ECh],ax
        xor     ax,ax
        idiv    si
        cwd
        mov     word ptr ds:[26EEh],ax
        add     word ptr ds:[26ECh],dx
        mov     ax,word ptr [di+PNT_Z]
        sub     ax,di
        cwd
        idiv    si
        mov     word ptr ds:[26F4h],ax
        xor     ax,ax
        idiv    si
        cwd
        mov     word ptr ds:[26F6h],ax
        add     word ptr ds:[26F4h],dx
        mov     ax,word ptr [di+PNT_Y]
        sub     ax,cx
        cwd
        idiv    si
        mov     word ptr ds:[26F0h],ax
        xor     ax,ax
        idiv    si
        cwd
        mov     word ptr ds:[26F2h],ax
        add     word ptr ds:[26F0h],dx
        mov     al,byte ptr [di+PNT_CODES]
        cmp     al,byte ptr ds:[286Dh]
        je      short L3833
        mov     di,word ptr ds:[26F8h]
L37DD:
        mov     ax,word ptr ds:[26EEh]
        add     word ptr ds:[26FAh],ax
        adc     bx,word ptr ds:[26ECh]
        add     word ptr ds:[26FAh],ax
        adc     bx,0
        mov     ax,word ptr ds:[26F2h]
        add     word ptr ds:[26FCh],ax
        adc     cx,word ptr ds:[26F0h]
        add     word ptr ds:[26FCh],ax
        adc     cx,0
        mov     ax,word ptr ds:[26F6h]
        add     word ptr ds:[26FEh],ax
        adc     bp,word ptr ds:[26F4h]
        add     word ptr ds:[26FEh],ax
        adc     bp,0
        call    _code_pnt
        mov     word ptr [di+PNT_X],bx
        mov     word ptr [di+PNT_Y],cx
        mov     word ptr [di+PNT_Z],bp
        mov     byte ptr [di+PNT_CODES],al
        add     di,8
        dec     si
        jne     L37DD
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L3833:
        mov     di,word ptr ds:[26F8h]
        mov     dl,al
        xchg    si,cx
L383B:
        mov     ax,word ptr ds:[26EEh]
        add     word ptr ds:[26FAh],ax
        adc     bx,word ptr ds:[26ECh]
        add     word ptr ds:[26FAh],ax
        adc     bx,0
        mov     ax,word ptr ds:[26F2h]
        add     word ptr ds:[26FCh],ax
        adc     si,word ptr ds:[26F0h]
        add     word ptr ds:[26FCh],ax
        adc     si,0
        mov     ax,word ptr ds:[26F6h]
        add     word ptr ds:[26FEh],ax
        adc     bp,word ptr ds:[26F4h]
        add     word ptr ds:[26FEh],ax
        adc     bp,0
        mov     word ptr [di+PNT_X],bx
        mov     word ptr [di+PNT_Y],si
        mov     word ptr [di+PNT_Z],bx
        mov     byte ptr [di+PNT_CODES],dl
        add     di,8
        loop    L383B
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_375E endp

; seg004_388D  (+388D)
; Opcode CAh, operand: address. do_goursurf with the colour taken from a model variable: the
; `db 0A8h` turns do_goursurf's lodsw into the operand of a `test al,` so AX is kept.
_do_goursurfv proc near
        lodsw
        mov     bx,ax
        mov     ax,word ptr [bx]
        db      0A8h                    ; test al,: swallows do_goursurf's lodsw, keeping AX (the original's trick)
_do_goursurfv endp

; seg004_3893  (+3893)
; Opcode C8h, operand: colour. Selects Gouraud filling (15F6h = _seg003_6146, 15F8h =
; _seg003_6156) and stores the colour's shading-table byte (seg048:21Fh + 2 * colour) at 300h.
_do_goursurf proc near
        lodsw
        mov     bx,ax
        push    es
        mov     ax,seg seg048
        mov     es,ax
        shl     bx,1
        mov     al,byte ptr es:[bx+21Fh]
        pop     es
        mov     byte ptr ds:[300h],al
        mov     word ptr ds:[POLY_FILL],offset _seg003_6146
        mov     word ptr ds:[POLY_UFILL],offset _seg003_6156
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_goursurf endp

; seg004_38B9  (+38B9)
; Opcode D6h (UW-specific): the standard surface. When the low byte of 2932h is zero,
; selects Gouraud filling and points seg003's handler pair at 0F27h/0F29h at 0F2Bh/0FF7h;
; otherwise sets the flat colour from seg048:0DC3h through setcol_common.
_do_uwsurf proc near
        test    word ptr ds:[2932h],0FFh
        jne     short L38EA
        mov     word ptr ds:[POLY_FILL],offset _seg003_6146
        mov     word ptr ds:[POLY_UFILL],offset _seg003_6156
        mov     bx,es
        mov     ax,seg seg003
        mov     es,ax
        mov     word ptr es:[0F27h],0F2Bh
        mov     word ptr es:[0F29h],0FF7h
        mov     es,bx
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L38EA:
        mov     bx,es
        mov     ax,seg seg048
        mov     es,ax
        mov     al,byte ptr es:[0DC3h]
        xor     ah,ah
        mov     es,bx
        jmp     setcol_common
_do_uwsurf endp

; seg004_38FC  (+38FC)
; Opcode D8h: a translucent surface. Gouraud filling with seg003's handler pair at
; 0F27h/0F29h pointed at 0F56h/103Dh (the same pair smooth_over uses).
_do_transsurf proc near
        mov     word ptr ds:[POLY_FILL],offset _seg003_6146
        mov     word ptr ds:[POLY_UFILL],offset _seg003_6156
        mov     bx,es
        mov     ax,seg seg003
        mov     es,ax
        mov     word ptr es:[0F27h],0F56h
        mov     word ptr es:[0F29h],103Dh
        mov     es,bx
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_transsurf endp

; seg004_3925  (+3925)
; Opcode 44h: selects flat filling (438Dh, 4134h) without setting a colour.
_do_vexsurf proc near
        mov     word ptr ds:[POLY_FILL],438Dh
        mov     word ptr ds:[POLY_UFILL],4134h
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_vexsurf endp

; seg004_3937  (+3937)
; Opcode 40h: selects the fill routine 2E7Ch for both slots (seg003; what it draws is not
; known). After it come FM's do_setpat, do_putpat and do_usepat, in FM's order: UW1's opcode
; table has them as opcodes 3Ch, 98h and 9Ah, where UW2 keeps the same code with no table
; entry. Roughly: do_setpat rotates a list of vectors into a table, and do_putpat and
; do_usepat place points from such a table at a rotated offset (do_putpat then runs a
; sub-program); not traced further.
_do_cavesurf proc near
        mov     word ptr ds:[POLY_FILL],2E7Ch
        mov     word ptr ds:[POLY_UFILL],2E7Ch
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_cavesurf endp

; seg004_3949  (+3949)
; do_setpat (FM name), opcode 3Ch in UW1: see do_cavesurf's comment.
_do_setpat proc near
        lodsw
        push    si
        add     si,ax
        lodsw
        mov     byte ptr ds:[2872h],al
        inc     si
        inc     si
        mov     word ptr ds:[26F8h],si
        shl     ax,1
        add     si,ax
        shl     ax,1
        add     si,ax
L395F:
        lodsw
        mov     bx,ax
        lodsw
        mov     cx,ax
        lodsw
        mov     bp,ax
        call    _mxmul
        mov     di,word ptr ds:[26F8h]
        mov     word ptr [di],bx
        mov     word ptr [di+4],cx
        mov     word ptr [di+2],bp
        add     word ptr ds:[26F8h],6
        dec     byte ptr ds:[2872h]
        jne     L395F
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_setpat endp

; seg004_3989  (+3989)
; do_putpat (FM name), opcode 98h in UW1: see do_cavesurf's comment.
_do_putpat proc near
        push    word ptr ds:[OBJ_X]
        push    word ptr ds:[OBJ_Y]
        push    word ptr ds:[OBJ_Z]
        call    _load_xlate_rotate_pnt
        mov     word ptr ds:[15FCh],bx
        mov     word ptr ds:[15FEh],cx
        mov     word ptr ds:[1600h],bp
        mov     ax,word ptr ds:[OBJ_X]
        sub     ax,word ptr [si-6]
        mov     word ptr ds:[OBJ_X],ax
        mov     ax,word ptr ds:[OBJ_Y]
        sub     ax,word ptr [si-2]
        mov     word ptr ds:[OBJ_Y],ax
        mov     ax,word ptr ds:[OBJ_Z]
        sub     ax,word ptr [si-4]
        mov     word ptr ds:[OBJ_Z],ax
        mov     bp,PNT_X
        lodsw
        add     di,ax
        lodsw
        push    si
        add     si,ax
        lodsw
        mov     dx,ax
        lodsw
        sub     al,byte ptr ds:[OBJ_SHIFT]
        mov     dh,al
        shr     dl,1
        jae     short L3A05
        mov     cl,dh
        lodsw
        sar     ax,cl
        mov     bx,ax
        add     bx,word ptr ds:[15FCh]
        lodsw
        sar     ax,cl
        mov     bp,ax
        add     bp,word ptr ds:[1600h]
        lodsw
        sar     ax,cl
        mov     cx,ax
        add     cx,word ptr ds:[15FEh]
        call    _code_pnt
        mov     word ptr [di],bx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],bp
        mov     byte ptr [di+6],al
        add     di,8
L3A05:
        mov     cl,dh
        lodsw
        sar     ax,cl
        mov     bx,ax
        add     bx,word ptr ds:[15FCh]
        lodsw
        sar     ax,cl
        mov     bp,ax
        add     bp,word ptr ds:[1600h]
        lodsw
        sar     ax,cl
        mov     cx,ax
        add     cx,word ptr ds:[15FEh]
        call    _code_pnt
        mov     word ptr [di],bx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],bp
        mov     byte ptr [di+6],al
        mov     cl,dh
        lodsw
        sar     ax,cl
        mov     bx,ax
        add     bx,word ptr ds:[15FCh]
        lodsw
        sar     ax,cl
        mov     bp,ax
        add     bp,word ptr ds:[1600h]
        lodsw
        sar     ax,cl
        mov     cx,ax
        add     cx,word ptr ds:[15FEh]
        call    _code_pnt
        mov     word ptr [di+8],bx
        mov     word ptr [di+0Ah],cx
        mov     word ptr [di+0Ch],bp
        mov     byte ptr [di+0Eh],al
        add     di,10h
        dec     dl
        jne     L3A05
        pop     si
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        pop     word ptr ds:[OBJ_Z]
        pop     word ptr ds:[OBJ_Y]
        pop     word ptr ds:[OBJ_X]
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_putpat endp

; seg004_3A81  (+3A81)
; do_usepat (FM name), opcode 9Ah in UW1: see do_cavesurf's comment.
_do_usepat proc near
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        mov     bx,word ptr ds:[OBJ_X]
        neg     bx
        shl     bx,cl
        mov     bp,word ptr ds:[OBJ_Z]
        neg     bp
        shl     bp,cl
        mov     ax,word ptr ds:[OBJ_Y]
        neg     ax
        shl     ax,cl
        mov     cx,ax
        call    _mxmul
        mov     word ptr ds:[15FCh],bx
        mov     word ptr ds:[15FEh],cx
        mov     word ptr ds:[1600h],bp
        mov     di,PNT_X
        lodsw
        add     di,ax
        lodsw
        push    si
        add     si,ax
        lodsw
        mov     cx,ax
        lodsw
        sub     al,byte ptr ds:[OBJ_SHIFT]
        mov     dl,al
        shr     cl,1
        mov     dh,cl
        jae     short L3AF5
        mov     cl,dl
        lodsw
        sar     ax,cl
        mov     bx,ax
        add     bx,word ptr ds:[15FCh]
        lodsw
        sar     ax,cl
        mov     bp,ax
        add     bp,word ptr ds:[1600h]
        lodsw
        sar     ax,cl
        mov     cx,ax
        add     cx,word ptr ds:[15FEh]
        call    _code_pnt
        mov     word ptr [di],bx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],bp
        mov     byte ptr [di+6],al
        add     di,8
L3AF5:
        mov     cl,dl
        lodsw
        sar     ax,cl
        mov     bx,ax
        add     bx,word ptr ds:[15FCh]
        lodsw
        sar     ax,cl
        mov     bp,ax
        add     bp,word ptr ds:[1600h]
        lodsw
        sar     ax,cl
        mov     cx,ax
        add     cx,word ptr ds:[15FEh]
        call    _code_pnt
        mov     word ptr [di],bx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],bp
        mov     byte ptr [di+6],al
        mov     cl,dl
        lodsw
        sar     ax,cl
        mov     bx,ax
        add     bx,word ptr ds:[15FCh]
        lodsw
        sar     ax,cl
        mov     bp,ax
        add     bp,word ptr ds:[1600h]
        lodsw
        sar     ax,cl
        mov     cx,ax
        add     cx,word ptr ds:[15FEh]
        call    _code_pnt
        mov     word ptr [di+8],bx
        mov     word ptr [di+0Ah],cx
        mov     word ptr [di+0Ch],bp
        mov     byte ptr [di+0Eh],al
        add     di,10h
        dec     dh
        jne     L3AF5
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_usepat endp

; seg004_3B5A  (+3B5A)
; Opcode 48h, operand: offset. Jumps: SI += offset.
_do_ijmp proc near
        lodsw
        add     si,ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_ijmp endp

; seg004_3B63  (+3B63)
; Opcode 32h, operand: address. Jumps to the program address held in the model variable.
_do_ijmp_i proc near
        lodsw
        mov     bx,ax
        mov     si,word ptr [bx]
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_ijmp_i endp

; seg004_3B6E  (+3B6E)
; Opcode 4Ah, operands: dx, dy, dz. Moves the object origin by a relative offset (subtracts
; from 2886h..288Ah) and calls self_modify.
_do_rorg proc near
        lodsw
        sub     word ptr ds:[OBJ_X],ax
        lodsw
        sub     word ptr ds:[OBJ_Y],ax
        lodsw
        sub     word ptr ds:[OBJ_Z],ax
        call    _self_modify
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_rorg endp

; seg004_3B86  (+3B86)
; Opcode 4Ch, operands: an (x, z, y) offset, point. Stores the rotated offset (no origin, no
; clip
; code) as a point, for do_addres and do_subres.
_do_defdelta proc near
        mov     cl,byte ptr ds:[OBJ_SHIFT]
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
        mov     word ptr [di+PNT_X],bx
        mov     word ptr [di+PNT_Y],cx
        mov     word ptr [di+PNT_Z],bp
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_defdelta endp

; seg004_3BB1  (+3BB1)
; Opcode 8Ch, operands: point a, point b, destination. Destination = a + b, clip-coded.
_do_addres proc near
        lodsw
        mov     bp,ax
        lodsw
        mov     di,ax
        mov     bx,word ptr ds:[bp+PNT_X]
        add     bx,word ptr [di+PNT_X]
        mov     cx,word ptr ds:[bp+PNT_Y]
        add     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr ds:[bp+PNT_Z]
        add     bp,word ptr [di+PNT_Z]
        lodsw
        mov     di,ax
        call    _code_pnt
        mov     word ptr [di+PNT_X],bx
        mov     word ptr [di+PNT_Y],cx
        mov     word ptr [di+PNT_Z],bp
        mov     byte ptr [di+PNT_CODES],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_addres endp

; seg004_3BEE  (+3BEE)
; Opcode C6h, operands: point a, point b, destination. Destination = a - b, clip-coded.
_do_subres proc near
        lodsw
        mov     bp,ax
        lodsw
        mov     di,ax
        mov     bx,word ptr ds:[bp+PNT_X]
        sub     bx,word ptr [di+PNT_X]
        mov     cx,word ptr ds:[bp+PNT_Y]
        sub     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr ds:[bp+PNT_Z]
        sub     bp,word ptr [di+PNT_Z]
        lodsw
        mov     di,ax
        call    _code_pnt
        mov     word ptr [di+PNT_X],bx
        mov     word ptr [di+PNT_Y],cx
        mov     word ptr [di+PNT_Z],bp
        mov     byte ptr [di+PNT_CODES],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_subres endp

; seg004_3C2B  (+3C2B)
; Opcode BAh: do_ihcall with the angle taken from a model variable.
_do_ihcall_ind proc near
        lodsw
        mov     bx,ax
        mov     bx,word ptr [bx]
        jmp     short L3C35
_do_ihcall_ind endp

; seg004_3C32  (+3C32)
; Opcode 50h, operands: angle, offset. Saves the matrix and origin, rotates the matrix by the
; heading angle (instance_head, which also rotates the origin), runs the sub-program at
; SI + offset, and restores both. The b and p variants below do the same for bank and pitch.
_do_ihcall proc near
        lodsw
        mov     bx,ax
L3C35:
        push    word ptr ds:[MAT_XX]
        push    word ptr ds:[MAT_YX]
        push    word ptr ds:[MAT_ZX]
        push    word ptr ds:[MAT_XY]
        push    word ptr ds:[MAT_YY]
        push    word ptr ds:[MAT_ZY]
        push    word ptr ds:[MAT_XZ]
        push    word ptr ds:[MAT_YZ]
        push    word ptr ds:[MAT_ZZ]
        push    word ptr ds:[OBJ_X]
        push    word ptr ds:[OBJ_Y]
        push    word ptr ds:[OBJ_Z]
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
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        pop     word ptr ds:[OBJ_Z]
        pop     word ptr ds:[OBJ_Y]
        pop     word ptr ds:[OBJ_X]
        pop     word ptr ds:[MAT_ZZ]
        pop     word ptr ds:[MAT_YZ]
        pop     word ptr ds:[MAT_XZ]
        pop     word ptr ds:[MAT_ZY]
        pop     word ptr ds:[MAT_YY]
        pop     word ptr ds:[MAT_XY]
        pop     word ptr ds:[MAT_ZX]
        pop     word ptr ds:[MAT_YX]
        pop     word ptr ds:[MAT_XX]
        call    _self_modify
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_ihcall endp

; seg004_3CB4  (+3CB4)
; Opcode C4h: do_ibcall with the angle from a model variable.
_do_ibcall_ind proc near
        lodsw
        mov     bx,ax
        mov     bx,word ptr [bx]
        jmp     short L3CBE
_do_ibcall_ind endp

; seg004_3CBB  (+3CBB)
; Opcode 70h: a sub-program rotated by a bank angle (instance_bank).
_do_ibcall proc near
        lodsw
        mov     bx,ax
L3CBE:
        push    word ptr ds:[MAT_XX]
        push    word ptr ds:[MAT_YX]
        push    word ptr ds:[MAT_ZX]
        push    word ptr ds:[MAT_XY]
        push    word ptr ds:[MAT_YY]
        push    word ptr ds:[MAT_ZY]
        push    word ptr ds:[MAT_XZ]
        push    word ptr ds:[MAT_YZ]
        push    word ptr ds:[MAT_ZZ]
        push    word ptr ds:[OBJ_X]
        push    word ptr ds:[OBJ_Y]
        push    word ptr ds:[OBJ_Z]
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
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        pop     word ptr ds:[OBJ_Z]
        pop     word ptr ds:[OBJ_Y]
        pop     word ptr ds:[OBJ_X]
        pop     word ptr ds:[MAT_ZZ]
        pop     word ptr ds:[MAT_YZ]
        pop     word ptr ds:[MAT_XZ]
        pop     word ptr ds:[MAT_ZY]
        pop     word ptr ds:[MAT_YY]
        pop     word ptr ds:[MAT_XY]
        pop     word ptr ds:[MAT_ZX]
        pop     word ptr ds:[MAT_YX]
        pop     word ptr ds:[MAT_XX]
        call    _self_modify
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_ibcall endp

; seg004_3D3D  (+3D3D)
; Opcode C2h: do_ipcall with the angle from a model variable.
_do_ipcall_ind proc near
        lodsw
        mov     bx,ax
        mov     bx,word ptr [bx]
        jmp     short L3D47
_do_ipcall_ind endp

; seg004_3D44  (+3D44)
; Opcode 72h: a sub-program rotated by a pitch angle (instance_pitch).
_do_ipcall proc near
        lodsw
        mov     bx,ax
L3D47:
        push    word ptr ds:[MAT_XX]
        push    word ptr ds:[MAT_YX]
        push    word ptr ds:[MAT_ZX]
        push    word ptr ds:[MAT_XY]
        push    word ptr ds:[MAT_YY]
        push    word ptr ds:[MAT_ZY]
        push    word ptr ds:[MAT_XZ]
        push    word ptr ds:[MAT_YZ]
        push    word ptr ds:[MAT_ZZ]
        push    word ptr ds:[OBJ_X]
        push    word ptr ds:[OBJ_Y]
        push    word ptr ds:[OBJ_Z]
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
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        pop     word ptr ds:[OBJ_Z]
        pop     word ptr ds:[OBJ_Y]
        pop     word ptr ds:[OBJ_X]
        pop     word ptr ds:[MAT_ZZ]
        pop     word ptr ds:[MAT_YZ]
        pop     word ptr ds:[MAT_XZ]
        pop     word ptr ds:[MAT_ZY]
        pop     word ptr ds:[MAT_YY]
        pop     word ptr ds:[MAT_XY]
        pop     word ptr ds:[MAT_ZX]
        pop     word ptr ds:[MAT_YX]
        pop     word ptr ds:[MAT_XX]
        call    _self_modify
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_ipcall endp

; seg004_3DC6  (+3DC6)
; UW1 only: opcode 52h. Takes points relative to the object origin, rotates them
; (seg004_52FF, 5374) and divides by a count from its operands; not traced further.
_seg004_3DC6 proc near
        xor     ax,ax
        mov     di,26ECh
        mov     cx,6
        rep stosw
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        lodsw
        sub     ax,word ptr ds:[OBJ_X]
        shl     ax,cl
        mov     bx,ax
        lodsw
        sub     ax,word ptr ds:[OBJ_Z]
        shl     ax,cl
        mov     bp,ax
        lodsw
        sub     ax,word ptr ds:[OBJ_Y]
        shl     ax,cl
        mov     cx,ax
        call    _seg004_52FF
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        lodsw
        sub     ax,word ptr [si-8]
        shl     ax,cl
        mov     bx,ax
        lodsw
        sub     ax,word ptr [si-8]
        shl     ax,cl
        mov     bp,ax
        lodsw
        sub     ax,word ptr [si-8]
        shl     ax,cl
        mov     cx,ax
        call    _seg004_5374
        lodsw
        mov     cx,ax
        mov     ax,word ptr ds:[26E2h]
        cwd
        idiv    cx
        mov     word ptr ds:[26E2h],ax
        mov     ax,word ptr ds:[26E0h]
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        add     word ptr ds:[26E0h],ax
        add     word ptr ds:[26E2h],dx
        mov     ax,word ptr ds:[26E6h]
        cwd
        idiv    cx
        mov     word ptr ds:[26E6h],ax
        mov     ax,word ptr ds:[26E4h]
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        add     word ptr ds:[26E4h],ax
        add     word ptr ds:[26E6h],dx
        mov     ax,word ptr ds:[26EAh]
        cwd
        idiv    cx
        mov     word ptr ds:[26EAh],ax
        mov     ax,word ptr ds:[26E8h]
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        add     word ptr ds:[26E8h],ax
        add     word ptr ds:[26EAh],dx
        lodsw
        mov     di,ax
        add     di,PNT_X
        push    si
        mov     si,cx
L3E7B:
        mov     ax,word ptr ds:[26D6h]
        add     ax,word ptr ds:[26EEh]
        mov     bx,ax
        stosw
        mov     ax,word ptr ds:[26DAh]
        add     ax,word ptr ds:[26F2h]
        mov     cx,ax
        stosw
        mov     ax,word ptr ds:[26DEh]
        add     ax,word ptr ds:[26F6h]
        mov     bp,ax
        stosw
        call    _code_pnt
        stosw
        mov     ax,word ptr ds:[26E0h]
        add     word ptr ds:[26ECh],ax
        mov     ax,word ptr ds:[26E2h]
        adc     word ptr ds:[26EEh],ax
        mov     ax,word ptr ds:[26E4h]
        add     word ptr ds:[26F0h],ax
        mov     ax,word ptr ds:[26E6h]
        adc     word ptr ds:[26F2h],ax
        mov     ax,word ptr ds:[26E8h]
        add     word ptr ds:[26F4h],ax
        mov     ax,word ptr ds:[26EAh]
        adc     word ptr ds:[26F6h],ax
        dec     si
        jne     L3E7B
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_3DC6 endp

; seg004_3ED1  (+3ED1)
; Opcode 54h: pushes the object origin and the coordinate shift on the stack (the interpreter's
; own stack, SS = seg063).
_do_pushc proc near
        push    word ptr ds:[OBJ_X]
        push    word ptr ds:[OBJ_Y]
        push    word ptr ds:[OBJ_Z]
        push    word ptr ds:[OBJ_SHIFT]
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_pushc endp

; seg004_3EE7  (+3EE7)
; Opcode 56h: pops what do_pushc pushed and calls self_modify.
_do_popc proc near
        pop     word ptr ds:[OBJ_SHIFT]
        pop     word ptr ds:[OBJ_Z]
        pop     word ptr ds:[OBJ_Y]
        pop     word ptr ds:[OBJ_X]
        call    _self_modify
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_popc endp

; seg004_3F00  (+3F00)
; Opcode 5Eh: do_jnorm with no x term.
_do_jnorm_x0 proc near
        lodsw
        mov     bx,ax
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[OBJ_Y]
        imul    di
        mov     bp,ax
        mov     cx,dx
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[OBJ_Z]
        imul    di
        add     bp,ax
        adc     cx,dx
        jns     short L3F23
        add     si,bx
L3F23:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_jnorm_x0 endp

; seg004_3F29  (+3F29)
; Opcode 60h: do_jnorm with no y term.
_do_jnorm_y0 proc near
        lodsw
        mov     bx,ax
        lodsw
        mov     cx,ax
        lodsw
        add     ax,word ptr ds:[OBJ_X]
        imul    cx
        mov     cx,dx
        mov     bp,ax
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[OBJ_Z]
        imul    di
        add     bp,ax
        adc     cx,dx
        jns     short L3F4C
        add     si,bx
L3F4C:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_jnorm_y0 endp

; seg004_3F52  (+3F52)
; Opcode 62h: do_jnorm with no z term.
_do_jnorm_z0 proc near
        lodsw
        mov     bx,ax
        lodsw
        mov     cx,ax
        lodsw
        add     ax,word ptr ds:[OBJ_X]
        imul    cx
        mov     cx,dx
        mov     bp,ax
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[OBJ_Y]
        imul    di
        add     bp,ax
        adc     cx,dx
        jns     short L3F75
        add     si,bx
L3F75:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_jnorm_z0 endp

; seg004_3F7B  (+3F7B)
; Opcode 64h, operands: skip, nx, px. do_jnorm for a normal along x alone: only the sign of
; (px + origin x) against nx is tested.
_do_jnorm_yz0 proc near
        lodsw
        mov     bx,ax
        lodsw
        mov     cx,ax
        lodsw
        add     ax,word ptr ds:[OBJ_X]
        xor     ax,cx
        jns     short L3F8C
        add     si,bx
L3F8C:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_jnorm_yz0 endp

; seg004_3F92  (+3F92)
; Opcode 66h: the same for a normal along y.
_do_jnorm_xz0 proc near
        lodsw
        mov     bx,ax
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[OBJ_Y]
        xor     ax,di
        jns     short L3FA3
        add     si,bx
L3FA3:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_jnorm_xz0 endp

; seg004_3FA9  (+3FA9)
; Opcode 68h: the same for a normal along z.
_do_jnorm_xy0 proc near
        lodsw
        mov     bx,ax
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[OBJ_Z]
        xor     ax,di
        jns     short L3FBA
        add     si,bx
L3FBA:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_jnorm_xy0 endp

; seg004_3FC0  (+3FC0)
; Opcode 58h, operands: skip, then (n, p) for x, y, z. Back-face test: the same sum as
; do_sortnorm; when it is negative (the eye on the back of the plane) skips `skip` bytes,
; normally the face that follows. (System Shock: do_jnorm.) The most used opcode in UW1's
; models after polygons and x_rel (model_opcodes.tsv).
_do_jnorm proc near
        lodsw
        mov     bx,ax
        lodsw
        mov     cx,ax
        lodsw
        add     ax,word ptr ds:[OBJ_X]
        imul    cx
        mov     cx,dx
        mov     bp,ax
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[OBJ_Y]
        imul    di
        add     bp,ax
        adc     cx,dx
        lodsw
        mov     di,ax
        lodsw
        add     ax,word ptr ds:[OBJ_Z]
        imul    di
        add     bp,ax
        adc     cx,dx
        jns     short L3FF1
        add     si,bx
L3FF1:
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_jnorm endp

; seg004_3FF7  (+3FF7)
; Recovery for do_closure's projection (stored at ss:[4D5h] in clip_needed): drops the int 0
; frame (sti; add sp,6) and falls into do_3d_clip.
_clip_overflow proc near
        sti
        add     sp,6
_clip_overflow endp

; seg004_3FFB  (+3FFB)
; Clips the polygon in the vertex buffer in 3D against each plane its codes OR shows it
; crossing (right, left, top, bottom), stopping as soon as the codes AND to nonzero (nothing
; left), and then projects and draws it (back to do_closure's L2F30). Restores DS and ES to
; seg051 when it gives up.
_do_3d_clip proc near
        mov     word ptr ss:[4D5h],offset _overflow_handler_reg
        mov     ax,ds
        mov     es,ax
        test    byte ptr ds:[CLIP_OR],2
        je      short L4017
        call    _clip_right
        test    byte ptr ds:[CLIP_AND],0FFh
        jne     short L404D
L4017:
        test    byte ptr ds:[CLIP_OR],1
        je      short L4028
        call    _clip_left
        test    byte ptr ds:[CLIP_AND],0FFh
        jne     short L404D
L4028:
        test    byte ptr ds:[CLIP_OR],4
        je      short L4039
        call    _clip_top
        test    byte ptr ds:[CLIP_AND],0FFh
        jne     short L404D
L4039:
        test    byte ptr ds:[CLIP_OR],8
        je      short L404A
        call    _clip_bot
        test    byte ptr ds:[CLIP_AND],0FFh
        jne     short L404D
L404A:
        jmp     L2F30
L404D:
        pop     si
        mov     ax,seg seg051
        mov     ds,ax
        mov     es,ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_3d_clip endp

; seg004_405B  (+405B)
; Clips the vertex buffer against the y = z plane (code bit 4): copies the first vertices past
; the end to close the loop, then walks the edges writing the surviving vertices and the
; intersections into the other buffer (0C69h or 309h), recomputing the codes OR and AND.
; The intersection uses fixed-point interpolation with rounding (sar ax,1; adc). The other
; three are the same for the other planes.
_clip_top proc near
        mov     byte ptr ds:[CLIP_OR],0
        mov     byte ptr ds:[CLIP_AND],0FFh
        mov     si,word ptr ds:[VB_START]
        mov     di,word ptr ds:[VB_END]
        mov     cx,8
        rep movsw
        add     word ptr ds:[VB_END],8
        mov     si,word ptr ds:[VB_START]
        mov     di,VBUF1
        cmp     si,VBUF0
        je      short L4087
        mov     di,VBUF0
L4087:
        mov     word ptr ds:[VB_START],di
L408B:
        add     si,8
L408E:
        cmp     si,word ptr ds:[VB_END]
        jne     short L4097
        jmp     L415F
L4097:
        mov     al,byte ptr [si+6]
        test    al,4
        jne     short L40AD
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
        mov     cx,4
        rep movsw
        jmp     L408E
L40AD:
        test    byte ptr [si-2],4
        jne     short L4105
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
        jae     short L40D5
        cwd
        adc     ax,dx
L40D5:
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
        jae     short L40F0
        cwd
        adc     ax,dx
L40F0:
        add     ax,word ptr [si-6]
        stosw
        stosw
        mov     cx,ax
        mov     bp,ax
        call    _code_pnt
        stosw
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
L4105:
        test    byte ptr [si+0Eh],4
        jne     L408B
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
        jae     short L412D
        cwd
        adc     ax,dx
L412D:
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
        jae     short L4147
        cwd
        adc     ax,dx
L4147:
        add     ax,word ptr [si+2]
        stosw
        stosw
        mov     cx,ax
        mov     bp,ax
        call    _code_pnt
        stosw
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
        jmp     L408B
L415F:
        mov     word ptr ds:[VB_END],di
        ret
_clip_top endp

; seg004_4164  (+4164)
; Clips against y = -z (code bit 8). See clip_top.
_clip_bot proc near
        mov     byte ptr ds:[CLIP_OR],0
        mov     byte ptr ds:[CLIP_AND],0FFh
        mov     si,word ptr ds:[VB_START]
        mov     di,word ptr ds:[VB_END]
        mov     cx,8
        rep movsw
        add     word ptr ds:[VB_END],8
        mov     si,word ptr ds:[VB_START]
        mov     di,VBUF1
        cmp     si,VBUF0
        je      short L4190
        mov     di,VBUF0
L4190:
        mov     word ptr ds:[VB_START],di
L4194:
        add     si,8
L4197:
        cmp     si,word ptr ds:[VB_END]
        jne     short L41A0
        jmp     L426F
L41A0:
        mov     al,byte ptr [si+6]
        test    al,8
        jne     short L41B6
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
        mov     cx,4
        rep movsw
        jmp     L4197
L41B6:
        test    byte ptr [si-2],8
        jne     short L4210
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
        jae     short L41DE
        cwd
        adc     ax,dx
L41DE:
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
        jae     short L41F9
        cwd
        adc     ax,dx
L41F9:
        add     ax,word ptr [si-6]
        stosw
        mov     cx,ax
        neg     ax
        stosw
        mov     bp,ax
        call    _code_pnt
        stosw
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
L4210:
        test    byte ptr [si+0Eh],8
        je      short L4219
        jmp     L4194
L4219:
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
        jae     short L423B
        cwd
        adc     ax,dx
L423B:
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
        jae     short L4255
        cwd
        adc     ax,dx
L4255:
        add     ax,word ptr [si+2]
        stosw
        mov     cx,ax
        neg     ax
        stosw
        mov     bp,ax
        call    _code_pnt
        stosw
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
        jmp     L4194
L426F:
        mov     word ptr ds:[VB_END],di
        ret
_clip_bot endp

; seg004_4274  (+4274)
; Clips against x = z (code bit 2). See clip_top.
_clip_right proc near
        mov     byte ptr ds:[CLIP_OR],0
        mov     byte ptr ds:[CLIP_AND],0FFh
        mov     si,word ptr ds:[VB_START]
        mov     di,word ptr ds:[VB_END]
        mov     cx,8
        rep movsw
        add     word ptr ds:[VB_END],8
        mov     si,word ptr ds:[VB_START]
        mov     di,VBUF1
        cmp     si,VBUF0
        je      short L42A0
        mov     di,VBUF0
L42A0:
        mov     word ptr ds:[VB_START],di
L42A4:
        add     si,8
L42A7:
        cmp     si,word ptr ds:[VB_END]
        jne     short L42B0
        jmp     L437D
L42B0:
        mov     al,byte ptr [si+6]
        test    al,2
        jne     short L42C6
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
        mov     cx,4
        rep movsw
        jmp     L42A7
L42C6:
        test    byte ptr [si-2],2
        jne     short L431F
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
        jae     short L42ED
        cwd
        adc     ax,dx
L42ED:
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
        jae     short L4308
        cwd
        adc     ax,dx
L4308:
        add     ax,word ptr [si-6]
        stosw
        mov     cx,ax
        mov     ax,bx
        stosw
        mov     bp,ax
        call    _code_pnt
        stosw
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
L431F:
        test    byte ptr [si+0Eh],2
        je      short L4328
        jmp     L42A4
L4328:
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
        jae     short L4349
        cwd
        adc     ax,dx
L4349:
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
        jae     short L4363
        cwd
        adc     ax,dx
L4363:
        add     ax,word ptr [si+2]
        stosw
        mov     cx,ax
        mov     ax,bx
        stosw
        mov     bp,ax
        call    _code_pnt
        stosw
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
        jmp     L42A4
L437D:
        mov     word ptr ds:[VB_END],di
        ret
_clip_right endp

; seg004_4382  (+4382)
; Clips against x = -z (code bit 1). See clip_top.
_clip_left proc near
        mov     byte ptr ds:[CLIP_OR],0
        mov     byte ptr ds:[CLIP_AND],0FFh
        mov     si,word ptr ds:[VB_START]
        mov     di,word ptr ds:[VB_END]
        mov     cx,8
        rep movsw
        add     word ptr ds:[VB_END],8
        mov     si,word ptr ds:[VB_START]
        mov     di,VBUF1
        cmp     si,VBUF0
        je      short L43AE
        mov     di,VBUF0
L43AE:
        mov     word ptr ds:[VB_START],di
L43B2:
        add     si,8
L43B5:
        cmp     si,word ptr ds:[VB_END]
        jne     short L43BE
        jmp     L448F
L43BE:
        mov     al,byte ptr [si+6]
        test    al,1
        jne     short L43D4
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
        mov     cx,4
        rep movsw
        jmp     L43B5
L43D4:
        test    byte ptr [si-2],1
        jne     short L442F
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
        jae     short L43FB
        cwd
        adc     ax,dx
L43FB:
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
        jae     short L4416
        cwd
        adc     ax,dx
L4416:
        add     ax,word ptr [si-6]
        stosw
        mov     cx,ax
        mov     ax,bx
        neg     ax
        stosw
        mov     bp,ax
        call    _code_pnt
        stosw
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
L442F:
        test    byte ptr [si+0Eh],1
        je      short L4438
        jmp     L43B2
L4438:
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
        jae     short L4459
        cwd
        adc     ax,dx
L4459:
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
        jae     short L4473
        cwd
        adc     ax,dx
L4473:
        add     ax,word ptr [si+2]
        stosw
        mov     cx,ax
        mov     ax,bx
        neg     ax
        stosw
        mov     bp,ax
        call    _code_pnt
        stosw
        or      byte ptr ds:[CLIP_OR],al
        and     byte ptr ds:[CLIP_AND],al
        jmp     L43B2
L448F:
        mov     word ptr ds:[VB_END],di
        ret
_clip_left endp

; seg004_4494  (+4494)
; Opcode 78h (FM has two names at this address, do_parapiped and done_clipping_here),
; operands: point, three half-extents (scaled by the coordinate shift), offset. A bounding-box
; test: rotates the three extents into vectors (15D9h..15E9h) and tests the eight corners of
; the box around the point. All eight inside the frustum: calls set_accept (a no-op in UW2).
; All outside one plane (codes AND nonzero): skips `offset` bytes, or ends the program when
; the offset is 0. Otherwise continues after the operand.
_do_parapiped proc near
        lodsw
        mov     di,ax
        mov     cl,byte ptr ds:[OBJ_SHIFT]
        lodsw
        shl     ax,cl
        mov     bx,ax
        imul    word ptr ds:[MAT_XX]
        mov     word ptr ds:[15D9h],dx
        mov     ax,bx
        imul    word ptr ds:[MAT_XY]
        mov     word ptr ds:[15DBh],dx
        mov     ax,bx
        imul    word ptr ds:[MAT_XZ]
        mov     word ptr ds:[15DDh],dx
        lodsw
        shl     ax,cl
        mov     bx,ax
        imul    word ptr ds:[MAT_YX]
        mov     word ptr ds:[15DFh],dx
        mov     ax,bx
        imul    word ptr ds:[MAT_YY]
        mov     word ptr ds:[15E1h],dx
        mov     ax,bx
        imul    word ptr ds:[MAT_YZ]
        mov     word ptr ds:[15E3h],dx
        lodsw
        shl     ax,cl
        mov     bx,ax
        imul    word ptr ds:[MAT_ZX]
        mov     word ptr ds:[15E5h],dx
        mov     ax,bx
        imul    word ptr ds:[MAT_ZY]
        mov     word ptr ds:[15E7h],dx
        mov     ax,bx
        imul    word ptr ds:[MAT_ZZ]
        mov     word ptr ds:[15E9h],dx
        mov     al,byte ptr [di+PNT_CODES]
        or      al,al
        je      short L4509
        jmp     L479B
L4509:
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        add     bx,word ptr ds:[15D9h]
        add     cx,word ptr ds:[15DBh]
        add     bp,word ptr ds:[15DDh]
        add     bx,word ptr ds:[15DFh]
        add     cx,word ptr ds:[15E1h]
        add     bp,word ptr ds:[15E3h]
        add     bx,word ptr ds:[15E5h]
        add     cx,word ptr ds:[15E7h]
        add     bp,word ptr ds:[15E9h]
        jns     short L453E
        jmp     L4792
L453E:
        cmp     bx,bp
        jle     short L4545
        jmp     L4792
L4545:
        cmp     cx,bp
        jle     short L454C
        jmp     L4792
L454C:
        neg     bp
        cmp     bx,bp
        jge     short L4555
        jmp     L4792
L4555:
        cmp     cx,bp
        jge     short L455C
        jmp     L4792
L455C:
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        sub     bx,word ptr ds:[15D9h]
        sub     cx,word ptr ds:[15DBh]
        sub     bp,word ptr ds:[15DDh]
        add     bx,word ptr ds:[15DFh]
        add     cx,word ptr ds:[15E1h]
        add     bp,word ptr ds:[15E3h]
        add     bx,word ptr ds:[15E5h]
        add     cx,word ptr ds:[15E7h]
        add     bp,word ptr ds:[15E9h]
        jns     short L4591
        jmp     L4792
L4591:
        cmp     bx,bp
        jle     short L4598
        jmp     L4792
L4598:
        cmp     cx,bp
        jle     short L459F
        jmp     L4792
L459F:
        neg     bp
        cmp     bx,bp
        jge     short L45A8
        jmp     L4792
L45A8:
        cmp     cx,bp
        jge     short L45AF
        jmp     L4792
L45AF:
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        add     bx,word ptr ds:[15D9h]
        add     cx,word ptr ds:[15DBh]
        add     bp,word ptr ds:[15DDh]
        sub     bx,word ptr ds:[15DFh]
        sub     cx,word ptr ds:[15E1h]
        sub     bp,word ptr ds:[15E3h]
        add     bx,word ptr ds:[15E5h]
        add     cx,word ptr ds:[15E7h]
        add     bp,word ptr ds:[15E9h]
        jns     short L45E4
        jmp     L4792
L45E4:
        cmp     bx,bp
        jle     short L45EB
        jmp     L4792
L45EB:
        cmp     cx,bp
        jle     short L45F2
        jmp     L4792
L45F2:
        neg     bp
        cmp     bx,bp
        jge     short L45FB
        jmp     L4792
L45FB:
        cmp     cx,bp
        jge     short L4602
        jmp     L4792
L4602:
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        sub     bx,word ptr ds:[15D9h]
        sub     cx,word ptr ds:[15DBh]
        sub     bp,word ptr ds:[15DDh]
        sub     bx,word ptr ds:[15DFh]
        sub     cx,word ptr ds:[15E1h]
        sub     bp,word ptr ds:[15E3h]
        add     bx,word ptr ds:[15E5h]
        add     cx,word ptr ds:[15E7h]
        add     bp,word ptr ds:[15E9h]
        jns     short L4637
        jmp     L4792
L4637:
        cmp     bx,bp
        jle     short L463E
        jmp     L4792
L463E:
        cmp     cx,bp
        jle     short L4645
        jmp     L4792
L4645:
        neg     bp
        cmp     bx,bp
        jge     short L464E
        jmp     L4792
L464E:
        cmp     cx,bp
        jge     short L4655
        jmp     L4792
L4655:
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        add     bx,word ptr ds:[15D9h]
        add     cx,word ptr ds:[15DBh]
        add     bp,word ptr ds:[15DDh]
        add     bx,word ptr ds:[15DFh]
        add     cx,word ptr ds:[15E1h]
        add     bp,word ptr ds:[15E3h]
        sub     bx,word ptr ds:[15E5h]
        sub     cx,word ptr ds:[15E7h]
        sub     bp,word ptr ds:[15E9h]
        jns     short L468A
        jmp     L4792
L468A:
        cmp     bx,bp
        jle     short L4691
        jmp     L4792
L4691:
        cmp     cx,bp
        jle     short L4698
        jmp     L4792
L4698:
        neg     bp
        cmp     bx,bp
        jge     short L46A1
        jmp     L4792
L46A1:
        cmp     cx,bp
        jge     short L46A8
        jmp     L4792
L46A8:
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        sub     bx,word ptr ds:[15D9h]
        sub     cx,word ptr ds:[15DBh]
        sub     bp,word ptr ds:[15DDh]
        add     bx,word ptr ds:[15DFh]
        add     cx,word ptr ds:[15E1h]
        add     bp,word ptr ds:[15E3h]
        sub     bx,word ptr ds:[15E5h]
        sub     cx,word ptr ds:[15E7h]
        sub     bp,word ptr ds:[15E9h]
        jns     short L46DD
        jmp     L4792
L46DD:
        cmp     bx,bp
        jle     short L46E4
        jmp     L4792
L46E4:
        cmp     cx,bp
        jle     short L46EB
        jmp     L4792
L46EB:
        neg     bp
        cmp     bx,bp
        jge     short L46F4
        jmp     L4792
L46F4:
        cmp     cx,bp
        jge     short L46FB
        jmp     L4792
L46FB:
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        add     bx,word ptr ds:[15D9h]
        add     cx,word ptr ds:[15DBh]
        add     bp,word ptr ds:[15DDh]
        sub     bx,word ptr ds:[15DFh]
        sub     cx,word ptr ds:[15E1h]
        sub     bp,word ptr ds:[15E3h]
        sub     bx,word ptr ds:[15E5h]
        sub     cx,word ptr ds:[15E7h]
        sub     bp,word ptr ds:[15E9h]
        js      short L4792
        cmp     bx,bp
        jg      short L4792
        cmp     cx,bp
        jg      short L4792
        neg     bp
        cmp     bx,bp
        jl      short L4792
        cmp     cx,bp
        jl      short L4792
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        sub     bx,word ptr ds:[15D9h]
        sub     cx,word ptr ds:[15DBh]
        sub     bp,word ptr ds:[15DDh]
        sub     bx,word ptr ds:[15DFh]
        sub     cx,word ptr ds:[15E1h]
        sub     bp,word ptr ds:[15E3h]
        sub     bx,word ptr ds:[15E5h]
        sub     cx,word ptr ds:[15E7h]
        sub     bp,word ptr ds:[15E9h]
        js      short L4792
        cmp     bx,bp
        jg      short L4792
        cmp     cx,bp
        jg      short L4792
        neg     bp
        cmp     bx,bp
        jl      short L4792
        cmp     cx,bp
        jl      short L4792
        mov     ax,0FFFFh
        call    _seg004_5040
        add     si,2
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L4792:
        add     si,2
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L479B:
        mov     dl,al
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        add     bx,word ptr ds:[15D9h]
        add     cx,word ptr ds:[15DBh]
        add     bp,word ptr ds:[15DDh]
        add     bx,word ptr ds:[15DFh]
        add     cx,word ptr ds:[15E1h]
        add     bp,word ptr ds:[15E3h]
        add     bx,word ptr ds:[15E5h]
        add     cx,word ptr ds:[15E7h]
        add     bp,word ptr ds:[15E9h]
        call    _code_pnt
        and     dl,al
        je      L4792
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        sub     bx,word ptr ds:[15D9h]
        sub     cx,word ptr ds:[15DBh]
        sub     bp,word ptr ds:[15DDh]
        add     bx,word ptr ds:[15DFh]
        add     cx,word ptr ds:[15E1h]
        add     bp,word ptr ds:[15E3h]
        add     bx,word ptr ds:[15E5h]
        add     cx,word ptr ds:[15E7h]
        add     bp,word ptr ds:[15E9h]
        call    _code_pnt
        and     dl,al
        je      L4792
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        add     bx,word ptr ds:[15D9h]
        add     cx,word ptr ds:[15DBh]
        add     bp,word ptr ds:[15DDh]
        sub     bx,word ptr ds:[15DFh]
        sub     cx,word ptr ds:[15E1h]
        sub     bp,word ptr ds:[15E3h]
        add     bx,word ptr ds:[15E5h]
        add     cx,word ptr ds:[15E7h]
        add     bp,word ptr ds:[15E9h]
        call    _code_pnt
        and     dl,al
        jne     short L4845
        jmp     L4792
L4845:
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        sub     bx,word ptr ds:[15D9h]
        sub     cx,word ptr ds:[15DBh]
        sub     bp,word ptr ds:[15DDh]
        sub     bx,word ptr ds:[15DFh]
        sub     cx,word ptr ds:[15E1h]
        sub     bp,word ptr ds:[15E3h]
        add     bx,word ptr ds:[15E5h]
        add     cx,word ptr ds:[15E7h]
        add     bp,word ptr ds:[15E9h]
        call    _code_pnt
        and     dl,al
        jne     short L487F
        jmp     L4792
L487F:
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        add     bx,word ptr ds:[15D9h]
        add     cx,word ptr ds:[15DBh]
        add     bp,word ptr ds:[15DDh]
        add     bx,word ptr ds:[15DFh]
        add     cx,word ptr ds:[15E1h]
        add     bp,word ptr ds:[15E3h]
        sub     bx,word ptr ds:[15E5h]
        sub     cx,word ptr ds:[15E7h]
        sub     bp,word ptr ds:[15E9h]
        call    _code_pnt
        and     dl,al
        jne     short L48B9
        jmp     L4792
L48B9:
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        sub     bx,word ptr ds:[15D9h]
        sub     cx,word ptr ds:[15DBh]
        sub     bp,word ptr ds:[15DDh]
        add     bx,word ptr ds:[15DFh]
        add     cx,word ptr ds:[15E1h]
        add     bp,word ptr ds:[15E3h]
        sub     bx,word ptr ds:[15E5h]
        sub     cx,word ptr ds:[15E7h]
        sub     bp,word ptr ds:[15E9h]
        call    _code_pnt
        and     dl,al
        jne     short L48F3
        jmp     L4792
L48F3:
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        add     bx,word ptr ds:[15D9h]
        add     cx,word ptr ds:[15DBh]
        add     bp,word ptr ds:[15DDh]
        sub     bx,word ptr ds:[15DFh]
        sub     cx,word ptr ds:[15E1h]
        sub     bp,word ptr ds:[15E3h]
        sub     bx,word ptr ds:[15E5h]
        sub     cx,word ptr ds:[15E7h]
        sub     bp,word ptr ds:[15E9h]
        call    _code_pnt
        and     dl,al
        jne     short L492D
        jmp     L4792
L492D:
        mov     bx,word ptr [di+PNT_X]
        mov     cx,word ptr [di+PNT_Y]
        mov     bp,word ptr [di+PNT_Z]
        sub     bx,word ptr ds:[15D9h]
        sub     cx,word ptr ds:[15DBh]
        sub     bp,word ptr ds:[15DDh]
        sub     bx,word ptr ds:[15DFh]
        sub     cx,word ptr ds:[15E1h]
        sub     bp,word ptr ds:[15E3h]
        sub     bx,word ptr ds:[15E5h]
        sub     cx,word ptr ds:[15E7h]
        sub     bp,word ptr ds:[15E9h]
        call    _code_pnt
        and     dl,al
        jne     short L4967
        jmp     L4792
L4967:
        lodsw
        or      ax,ax
        je      short L4974
        add     si,ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L4974:
        ret
_do_parapiped endp

SEG004_TEXT ends

        end
