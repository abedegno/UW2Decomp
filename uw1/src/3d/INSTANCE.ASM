; /* target: seg004_4980 */
; /* opts: /ml */
;
; The renderer's frame entry and its transforms. render_3d (called by seg019's cRender) sets up
; the projection from the clip window, builds the view matrix, and runs the render database
; through the model interpreter (INTERP.ASM). The rest is the geometry the interpreter's opcodes
; call: turning model points into view space (mini_xlate_rotate_pnt, load_xlate_rotate_pnt,
; mxmul), the frustum clip codes of a view-space point (code_pnt), the sub-object rotations by
; heading, pitch and bank (the instance_ routines), 3 x 3 matrix products (mm9x9, mm3x9_bpsi,
; mm3x9t), and the divide-overflow handlers the renderer installs on int 0.
;
; Fixed point: matrix entries are 1.15 (7FFFh is 1.0); a product of two is shifted left once and
; its high word kept. Points are 16-bit view or model coordinates.
;
; UW1 against UW2: the same routines, mostly the same code with UW1's data offsets, and these
; differences. render_3d starts at +4987 (the table's seg004_4980 is the 7-byte int 3 handler
; before it, as in UW2) and does more (see its comment). check_flat also keeps NOCLIP.ASM's
; handler set in step, and seg004_5040 swaps the opcode table's entries 78h..9Ch between the
; two sets. UW1 has more matrix and point routines (seg004_520F, 52FF, 5374, 5821, 5834, 5937,
; 5A83). seg004_51A3
; patches four places in SPHERE.ASM where UW2 patches one (modify_axis2). The overflow
; handlers save BP at L5BCD, and overflow_handler_reg counts into overflow_count after it.
;
; Self-modifying code: render_3d writes biasx and biasy into immediates in other modules
; (SMOOTH.ASM's seg004_2A4 .. 322, INTERP.ASM's modify_biasx, modify_bx2 ..., and NOCLIP.ASM's
; seg004_2173 .. 21DA), self_modify writes the eye and shift into the xlate routines
; here, and check_flat copies one of two mxmul bodies (L512B, L517E) over mxmul.
;
; name: UW2's (inferred there, from System Shock's 3D library instance.asm); the routine names
; are FM Towns', through UW2.
;
; .186, where UW2 has .386: UW1's seg004 has no 386 instruction and none of the nop padding
; that TASM's .386 mode puts after forward conditional jumps.
;
; SEG004 split into its original modules: this is module 7 of 9, +4980..+5C30 of the segment
; (file E9F0h), a module of the second library.
; match: evidence for the bounds: starts on a paragraph after 11 zero bytes following INTERP's
; last ret, and ends with 11 zero bytes before TMAPOPS.ASM.
; Its relocations are entries 2641-2661 of UW.EXE's relocation table. Entries 2639 (seg063:04D7,
; the segment word of the far pointer the int 0 handler jumps through) and 2640 (seg051:2862,
; a far pointer to seg053) come just before them, so they belong to this module's far data, as
; UW2's FD71 piece does to UW2's INSTANCE.ASM; that data is not defined here yet.
;
; The routine comments are UW2's, with UW1's addresses, except where marked UW1.

; The model interpreter's opcode table: a word per opcode, the offset of its handler
; (INTERP.ASM), in seg051, the data segment while interpreting (UW2: 24F4h).
OPCODE_TABLE    equ     2738h

        .186

        extrn   seg048:far
        extrn   seg051:far
        extrn   seg063:far
        extrn   _seg019_C10:far
        extrn   seg053:far
        extrn   _SquareRoot_seg019_A30:far
        extrn   _sincos:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public 'code'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg004_2173:near
        extrn   _seg004_2181:near
        extrn   _seg004_21CE:near
        extrn   _seg004_21DA:near
        extrn   _flat_rod:near
        extrn   _flat_x_rel:near
        extrn   _flat_xy_rel:near
        extrn   _flat_xz_rel:near
        extrn   _flat_y_rel:near
        extrn   _flat_yz_rel:near
        extrn   _seg004_2023:near
        extrn   _seg004_20CC:near
        extrn   _seg004_223B:near
        extrn   _seg004_227C:near
        extrn   _seg004_22BD:near
        extrn   _seg004_2322:near
        extrn   _seg004_2348:near
        extrn   _seg004_2383:near
        extrn   _seg004_23BD:near
        extrn   _seg004_241B:near
        extrn   _seg004_2479:near
        extrn   _seg004_24D7:near
        extrn   _seg004_2527:near
        extrn   _seg004_256C:near
        extrn   _create_matrix:near
        extrn   _do_rod:near
        extrn   _do_x_rel:near
        extrn   _do_xy_rel:near
        extrn   _do_xz_rel:near
        extrn   _do_y_rel:near
        extrn   _do_yz_rel:near
        extrn   _do_z_rel:near
        extrn   _flat_z_rel:near
        extrn   _seg004_17AB:near
        extrn   modify_axis2:near
        extrn   modify_biasx:near
        extrn   modify_biasy:near
        extrn   modify_bx2:near
        extrn   modify_by2:near
        public  _check_flat
        public  _code_pnt
        public  _instance_bank
        public  _instance_head
        public  _instance_pitch
        public  _instance_vvars_bank
        public  _instance_vvars_head
        public  _instance_vvars_pitch
        public  _load_xlate_rotate_pnt
        public  _mini_xlate_rotate_pnt
        public  _mm3x9_bpsi
        public  _seg004_5A83
        public  _mm9x9
        public  _mxmul
        public  _overflow_handler_mem
        public  _overflow_handler_reg
        public  _overflow_handler_special_bp
        public  _render_3d
        public  _scale_matrix
        public  _seg004_4980
        public  _seg004_5040
        public  _seg004_51A3
        public  _seg004_520F
        public  _seg004_52FF
        public  _seg004_5374
        public  _seg004_5821
        public  _seg004_5834
        public  _seg004_5937
        public  _mm3x9t
        public  _self_modify
        public  _set_accept
        public  overflow_count

; seg004_4980  (+4980)
; seg004_4980: an opcode handler that breaks into the debugger (int 3) and goes on to the
; next opcode; probably the table's entry for unused opcodes.
_seg004_4980 proc near
        int     3
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_4980 endp

; seg004_4987  (+4987)
; render_3d (FM name): draw the frame whose render database starts at DbEntry (161Ch). Called
; by cRender (src/sys/C3DENTRY.ASM) with SS on cRender's private stack in seg063, where it
; reads the view window: the copy of the clip window that set_the_window (VIDMODE.ASM) keeps
; at seg063:0AA0 (left, top, right, bottom). Works out scrw, scrh, biasx and biasy from it and
; patches biasx and biasy into the projection code; counts the frame (15FAh); builds the view
; matrix (create_matrix in CAMERA.ASM, then seg004_51A3, scale_matrix, check_flat).
; It then switches to a stack in seg048 (SP 5546h) with seg048:2CECh set to 0D2Dh, and calls
; seg019_C10, which only returns (SYSLIBP.ASM). Finally it restores the stack and runs the
; database: the first opcode at DbEntry is called through the opcode table, and the chain of
; handlers returns here at the end of the frame (do_eof). Out: DS = ES = SS; seg048:2CECh
; restored. (UW2's comment, with UW1's addresses.)
; UW1 only: on entry and again at the end, when any of the 255 bytes from seg048:0008 is
; nonzero, it copies seg048:658h (first set to 50h, at the end to 0C0h, when it is 0) to
; seg048:410Fh; it sets the words 2860h..286Ah and byte 286Ch first; after create_matrix it
; walks the records from 161Eh, calling seg004_17AB (SPHERE.ASM) for each up to the one at
; 15Ch; after the frame it raises int 2 when 2860h is above 68h, calls seg004_520F unless
; 286Ch is set, and calls the handler at es:[2732h + type] for each word from 2864h to 2868h
; (in seg053).
_render_3d proc far
        push    es
        push    di
        push    ax
        mov     ax,seg seg048
        mov     es,ax
        mov     ax,0
        mov     cx,0FFh
        mov     di,8
        repe scasb
        jcxz    L49AE
        mov     al,byte ptr es:[658h]
        test    al,0FFh
        jne     short L49AA
        mov     al,50h
        mov     byte ptr es:[658h],al
L49AA:
        mov     byte ptr es:[410Fh],al
L49AE:
        pop     ax
        pop     di
        pop     es

        mov     ax,seg seg051
        mov     ds,ax
        mov     es,ax
        mov     word ptr ds:[2860h],0Eh
        mov     word ptr ds:[2864h],0
        mov     word ptr ds:[2866h],6
        mov     word ptr ds:[2868h],0
        mov     word ptr ds:[286Ah],0Ch
        mov     byte ptr ds:[286Ch],0FFh
        mov     ax,word ptr ss:[0AA4h]
        sub     ax,word ptr ss:[0AA0h]
        inc     ax
        shr     ax,1
        mov     word ptr ds:[26B2h],ax
        mov     ax,word ptr ss:[0AA2h]
        sub     ax,word ptr ss:[0AA6h]
        inc     ax
        shr     ax,1
        mov     word ptr ds:[26B0h],ax
        mov     ax,word ptr ss:[0AA4h]
        add     ax,word ptr ss:[0AA0h]
        inc     ax
        shr     ax,1
        mov     word ptr ds:[26B4h],ax
        mov     word ptr cs:[2A5h],ax
        mov     word ptr cs:[313h],ax
        mov     word ptr cs:[modify_biasx+1],ax
        mov     word ptr cs:[modify_bx2+1],ax
        mov     word ptr cs:[_seg004_21CE+1],ax
        mov     word ptr cs:[_seg004_2173+1],ax
        mov     ax,word ptr ss:[0AA2h]
        add     ax,word ptr ss:[0AA6h]
        shr     ax,1
        mov     word ptr ds:[26B6h],ax
        mov     word ptr cs:[2B0h],ax
        mov     word ptr cs:[323h],ax
        mov     word ptr cs:[modify_biasy+1],ax
        mov     word ptr cs:[modify_by2+1],ax
        mov     word ptr cs:[_seg004_21DA+1],ax
        mov     word ptr cs:[_seg004_2181+1],ax
        inc     word ptr ds:[15FAh]
        mov     byte ptr ds:[2880h],0
        call    _create_matrix
        call    _seg004_51A3
        call    _scale_matrix
        call    _check_flat
        mov     bp,word ptr ds:[161Eh]
L4A5F:
        test    word ptr [bp],0FFFFh
        je      short L4A7B
        js      short L4A83
        cmp     bp,word ptr ds:[15Ch]
        je      short L4A7B
        add     bp,6
        push    bp
        call    _seg004_17AB
        pop     bp
        add     bp,word ptr [bp-4]
        jmp     L4A5F
L4A7B:
        add     bp,word ptr [bp+2]
        add     bp,6
        jmp     L4A5F
L4A83:
        mov     ax,ss
        mov     word ptr ds:[15F2h],ax
        mov     word ptr ds:[15F0h],sp
        mov     si,seg seg048
        mov     ds,si
        mov     ax,word ptr ds:[2CECh]
        mov     word ptr cs:L4B42,ax
        mov     word ptr ds:[2CECh],0D2Dh
        mov     ax,seg seg063
        mov     es,ax
        mov     es,si
        cli
        mov     ss,si
        mov     sp,5546h
        sti
        call    far ptr _seg019_C10
        mov     bp,seg seg051
        mov     ds,bp
        mov     es,bp
        cli
        mov     ss,word ptr ds:[15F2h]
        mov     sp,word ptr ds:[15F0h]
        sti
        mov     si,word ptr ds:[161Ch]
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        mov     ax,word ptr ds:[2860h]
        cmp     ax,68h
        jle     short L4AD7
        int     2
        nop
L4AD7:
        push    ds
        push    si
        test    byte ptr ds:[286Ch],0FFh
        jne     short L4AE3
        call    _seg004_520F
L4AE3:
        mov     si,word ptr ds:[2864h]
        mov     cx,word ptr ds:[2868h]
        sub     cx,si
        shr     cx,1
        je      short L4B03
        mov     ax,seg seg053
        mov     ds,ax
L4AF6:
        lodsw
        mov     di,ax
        mov     bx,word ptr [di+4]
        call    word ptr es:[bx+2732h]
        loop    L4AF6
L4B03:
        pop     si
        pop     ds
        push    es
        push    di
        push    ax
        mov     ax,seg seg048
        mov     es,ax
        mov     ax,0
        mov     cx,0FFh
        mov     di,8
        repe scasb
        jcxz    L4B2C
        mov     al,byte ptr es:[658h]
        test    al,0FFh
        jne     short L4B28
        mov     al,0C0h
        mov     byte ptr es:[658h],al
L4B28:
        mov     byte ptr es:[410Fh],al
L4B2C:
        pop     ax
        pop     di
        pop     es
        mov     ax,seg seg048
        mov     ds,ax
        mov     ax,word ptr cs:L4B42
        mov     word ptr ds:[2CECh],ax
        mov     ax,ss
        mov     ds,ax
        mov     es,ax
        retf
_render_3d endp

L4B42   dw      0                       ; render_3d's copy of seg048:2CECh

; seg004_4B44  (+4B44)
; scale_matrix: fold the screen's shape and the zoom into the view matrix. Scales matrix entries
; by the ratio of the view's half width to half height (with 15F4, whose sign picks which axis
; is scaled; probably so x and y project to the square frustum CLIP.ASM's 45-degree planes
; assume) and by the zoom word 2726 (the depth column when it is positive, the other entries
; when negative), and works out 26CC/26CE, a unit horizontal direction on the screen, and from
; it 26D0/26D2, the axes do_scalebm (TMAPOPS.ASM) uses to place sprites facing the viewer.
; Installs _overflow_handler_reg for the divides: cRender points int 0 at seg063:05A0, which holds
; `jmp far cs:[4D5]`, and the far pointer at 4D5 (FM overflow_ptr) is into seg004, so writing
; its offset word selects the handler. Uses SquareRoot_seg019_A30 (IMATH.ASM).
_scale_matrix proc near
        mov     word ptr ds:[2728h],7FFFh
        mov     word ptr ds:[272Ah],7FFFh
        mov     ax,word ptr ds:[1608h]
        mov     word ptr ds:[1614h],ax
        mov     ax,word ptr ds:[160Ah]
        mov     word ptr ds:[1616h],ax
        mov     ax,word ptr ds:[160Ch]
        mov     word ptr ds:[1618h],ax
        mov     bx,word ptr ds:[26B0h]
        mov     cx,word ptr ds:[26B2h]
        mov     ax,word ptr ds:[15F4h]
        or      ax,ax
        js      short L4B7B
        imul    cx
        shl     ax,1
        rcl     dx,1
        xor     cx,cx
        jmp     short L4B8B
L4B7B:
        neg     ax
        imul    bx
        shl     ax,1
        rcl     dx,1
        mov     bx,dx
        mov     dx,cx
        mov     cx,ax
        xor     ax,ax
L4B8B:
        cmp     dx,bx
        jne     short L4B92
        jmp     L4C33
L4B92:
        jl      short L4BE6
        xchg    dx,bx
        xchg    cx,ax
        shr     dx,1
        rcr     ax,1
        idiv    bx
        mov     cx,ax
        mov     word ptr ds:[2728h],cx
        mov     ax,word ptr ds:[1602h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1602h],dx
        mov     ax,word ptr ds:[1608h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1608h],dx
        mov     ax,word ptr ds:[160Eh]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[160Eh],dx
        mov     ax,word ptr ds:[1616h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1616h],dx
        mov     ax,word ptr ds:[1618h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1618h],dx
        jmp     short L4C33
L4BE6:
        shr     dx,1
        rcr     ax,1
        idiv    bx
        mov     cx,ax
        mov     word ptr ds:[272Ah],cx
        mov     ax,word ptr ds:[1604h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1604h],dx
        mov     ax,word ptr ds:[160Ah]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[160Ah],dx
        mov     ax,word ptr ds:[1610h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1610h],dx
        mov     ax,word ptr ds:[1614h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1614h],dx
        mov     ax,word ptr ds:[1618h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1618h],dx
L4C33:
        mov     cx,word ptr ds:[2726h]
        or      cx,cx
        jns     short L4C3E
        jmp     L4CC6
L4C3E:
        mov     word ptr ds:[2730h],cx
        mov     ax,word ptr ds:[1606h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1606h],dx
        mov     ax,word ptr ds:[160Ch]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[160Ch],dx
        mov     ax,word ptr ds:[1612h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1612h],dx
        mov     ax,word ptr ds:[1616h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1616h],dx
        mov     ax,word ptr ds:[1614h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1614h],dx
        mov     ax,cx
        imul    ax
        mov     bp,ax
        mov     si,dx
        mov     ax,word ptr ds:[2728h]
        mov     word ptr ds:[272Ch],ax
        imul    ax
        add     ax,bp
        adc     dx,si
        mov     bx,dx
        xor     cx,cx
        call    far ptr _SquareRoot_seg019_A30
        mov     ax,di
        xchg    ah,al
        mov     word ptr ds:[2728h],ax
        mov     ax,word ptr ds:[272Ah]
        mov     word ptr ds:[272Eh],ax
        imul    ax
        add     ax,bp
        adc     dx,si
        mov     bx,dx
        xor     cx,cx
        call    far ptr _SquareRoot_seg019_A30
        mov     ax,di
        xchg    al,ah
        mov     word ptr ds:[272Ah],ax
        jmp     L4D85
L4CC6:
        mov     word ptr ds:[2730h],7FFFh
        neg     cx
        mov     ax,word ptr ds:[1602h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1602h],dx
        mov     ax,word ptr ds:[1608h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1608h],dx
        mov     ax,word ptr ds:[160Eh]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[160Eh],dx
        mov     ax,word ptr ds:[1604h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1604h],dx
        mov     ax,word ptr ds:[160Ah]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[160Ah],dx
        mov     ax,word ptr ds:[1610h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1610h],dx
        mov     ax,word ptr ds:[1618h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1618h],dx
        mov     si,cx
        mov     ax,word ptr ds:[2728h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     bp,dx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[272Ch],dx
        mov     dx,bp
        mov     ax,dx
        imul    ax
        mov     bx,dx
        add     bx,3FFFh
        xor     cx,cx
        call    far ptr _SquareRoot_seg019_A30
        mov     ax,di
        xchg    ah,al
        mov     word ptr ds:[2728h],ax
        mov     ax,word ptr ds:[272Ah]
        imul    si
        shl     ax,1
        rcl     dx,1
        mov     bp,dx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[272Eh],dx
        mov     dx,bp
        mov     ax,dx
        imul    ax
        mov     bx,dx
        add     bx,3FFFh
        xor     cx,cx
        call    far ptr _SquareRoot_seg019_A30
        mov     ax,di
        xchg    al,ah
        mov     word ptr ds:[272Ah],ax
L4D85:
        mov     word ptr ss:[4D5h],offset _overflow_handler_reg
        mov     ax,word ptr ds:[1606h]
        imul    word ptr ds:[1606h]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[1612h]
        imul    word ptr ds:[1612h]
        add     bx,ax
        adc     cx,dx
        or      ch,ch
        je      short L4DD7
        call    far ptr _SquareRoot_seg019_A30
        mov     dx,word ptr ds:[1606h]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L4DBD
        inc     ax
L4DBD:
        mov     word ptr ds:[26CCh],ax
        mov     dx,word ptr ds:[1612h]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L4DD2
        inc     ax
L4DD2:
        mov     word ptr ds:[26CEh],ax
        jmp     short L4E2B
L4DD7:
        mov     ax,word ptr ds:[1604h]
        imul    word ptr ds:[1604h]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[1610h]
        imul    word ptr ds:[1610h]
        add     bx,ax
        adc     cx,dx
        call    far ptr _SquareRoot_seg019_A30
        mov     dx,word ptr ds:[1604h]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L4E04
        inc     ax
L4E04:
        mov     bx,ax
        mov     dx,word ptr ds:[1610h]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L4E18
        inc     ax
L4E18:
        test    word ptr ds:[160Ch],0FFFFh
        js      short L4E24
        neg     bx
        neg     ax
L4E24:
        mov     word ptr ds:[26CCh],bx
        mov     word ptr ds:[26CEh],ax
L4E2B:
        mov     ax,word ptr ds:[26CEh]
        imul    word ptr ds:[1602h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[26CCh]
        neg     ax
        imul    word ptr ds:[160Eh]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        adc     dx,0
        mov     word ptr ds:[26D0h],dx
        mov     ax,word ptr ds:[26CEh]
        imul    word ptr ds:[1604h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[26CCh]
        neg     ax
        imul    word ptr ds:[1610h]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        adc     dx,0
        mov     word ptr ds:[26D2h],dx
        ret
_scale_matrix endp

; seg004_4E76  (+4E76)
; check_flat: when the view is level (the matrix's middle row and column are 0 except the middle
; entry, at least 7FFBh), install the flat variants of the relative-move opcode handlers
; (flat_x_rel ... flat_rod, INTERP.ASM) in the opcode table and copy the short mxmul body
; (L517E) over mxmul; otherwise install the general handlers and the full body (L512B). 25D4
; records which set is in, so the copy is done only on a change. A level view is probably the
; common case: the view pitches only while the player looks up or down.
_check_flat proc near
        test    word ptr ds:[1608h],0FFFFh
        jne     short L4EA1
        test    word ptr ds:[1604h],0FFFFh
        jne     short L4EA1
        test    word ptr ds:[1610h],0FFFFh
        jne     short L4EA1
        test    word ptr ds:[160Ch],0FFFFh
        jne     short L4EA1
        cmp     word ptr ds:[160Ah],7FFBh
        jl      short L4EA1
        jmp     L4F72
L4EA1:
        test    byte ptr ds:[286Fh],0FFh
        jne     short L4EAB
        jmp     L4F71
L4EAB:
        mov     byte ptr ds:[286Fh],0
        mov     word ptr ds:[2824h],offset _do_y_rel
        mov     word ptr ds:[2822h],offset _do_x_rel
        mov     word ptr ds:[2826h],offset _do_z_rel
        mov     word ptr ds:[2848h],offset _seg004_223B
        mov     word ptr ds:[284Ah],offset _seg004_227C
        mov     word ptr ds:[284Ch],offset _seg004_22BD
        mov     word ptr ds:[282Ch],offset _do_xy_rel
        mov     word ptr ds:[282Eh],offset _do_xz_rel
        mov     word ptr ds:[2830h],offset _do_yz_rel
        mov     word ptr ds:[2852h],offset _seg004_241B
        mov     word ptr ds:[2856h],offset _seg004_2479
        mov     word ptr ds:[2854h],offset _seg004_23BD
        mov     word ptr ds:[2838h],offset _do_rod
        mov     word ptr ds:[285Eh],offset _seg004_2023
L4F04:
        test    byte ptr ds:[286Eh],0FFh
        je      short L4F37
        mov     word ptr ds:[27BEh],offset _seg004_223B
        mov     word ptr ds:[27C0h],offset _seg004_227C
        mov     word ptr ds:[27C2h],offset _seg004_22BD
        mov     word ptr ds:[27C8h],offset _seg004_241B
        mov     word ptr ds:[27CCh],offset _seg004_2479
        mov     word ptr ds:[27CAh],offset _seg004_23BD
        mov     word ptr ds:[27D4h],offset _seg004_2023
        jmp     short L4F61
L4F37:
        mov     word ptr ds:[27BEh],offset _do_x_rel
        mov     word ptr ds:[27C0h],offset _do_y_rel
        mov     word ptr ds:[27C2h],offset _do_z_rel
        mov     word ptr ds:[27C8h],offset _do_xy_rel
        mov     word ptr ds:[27CCh],offset _do_yz_rel
        mov     word ptr ds:[27CAh],offset _do_xz_rel
        mov     word ptr ds:[27D4h],offset _do_rod
L4F61:
        mov     si,offset L512B
        mov     di,offset _mxmul
        push    es
        push    cs
        pop     es
        mov     cx,54h
        rep movs byte ptr es:[di],byte ptr cs:[si]
        pop     es
L4F71:
        ret
is_flat:
L4F72:
        test    byte ptr ds:[286Fh],0FFh
        jne     L4F71
        mov     byte ptr ds:[286Fh],0FFh
        mov     word ptr ds:[2824h],offset _flat_y_rel
        mov     word ptr ds:[2822h],offset _flat_x_rel
        mov     word ptr ds:[2826h],offset _flat_z_rel
        mov     word ptr ds:[2848h],offset _seg004_2348
        mov     word ptr ds:[284Ah],offset _seg004_2322
        mov     word ptr ds:[284Ch],offset _seg004_2383
        mov     word ptr ds:[282Ch],offset _flat_xy_rel
        mov     word ptr ds:[282Eh],offset _flat_xz_rel
        mov     word ptr ds:[2830h],offset _flat_yz_rel
        mov     word ptr ds:[2852h],offset _seg004_2527
        mov     word ptr ds:[2856h],offset _seg004_256C
        mov     word ptr ds:[2854h],offset _seg004_24D7
        mov     word ptr ds:[2838h],offset _flat_rod
        mov     word ptr ds:[285Eh],offset _seg004_20CC
        test    byte ptr ds:[286Eh],0FFh
        je      short L5005
        mov     word ptr ds:[27BEh],offset _seg004_2348
        mov     word ptr ds:[27C0h],offset _seg004_2322
        mov     word ptr ds:[27C2h],offset _seg004_2383
        mov     word ptr ds:[27C8h],offset _seg004_2527
        mov     word ptr ds:[27CCh],offset _seg004_256C
        mov     word ptr ds:[27CAh],offset _seg004_24D7
        mov     word ptr ds:[27D4h],offset _seg004_20CC
        jmp     short L502F
L5005:
        mov     word ptr ds:[27BEh],offset _flat_x_rel
        mov     word ptr ds:[27C0h],offset _flat_y_rel
        mov     word ptr ds:[27C2h],offset _flat_z_rel
        mov     word ptr ds:[27C8h],offset _flat_xy_rel
        mov     word ptr ds:[27CCh],offset _flat_yz_rel
        mov     word ptr ds:[27CAh],offset _flat_xz_rel
        mov     word ptr ds:[27D4h],offset _flat_rod
L502F:
        mov     si,offset L517E
        mov     di,offset _mxmul
        push    es
        push    cs
        pop     es
        mov     cx,26h
        rep movs byte ptr es:[di],byte ptr cs:[si]
        pop     es
        ret
_check_flat endp

; seg004_5040  (+5040)
; UW1 only: when byte 286Eh is set, clears it and copies the normal handlers for opcodes
; 78h..9Ch (seg051:2814h) into the opcode table (27B0h). The branch that would copy
; NOCLIP.ASM's set (283Ah) is never taken: the xor that clears 286Eh leaves ZF set.
_seg004_5040 proc near
        xor     ax,ax
        xor     al,byte ptr ds:[286Eh]
        je      short L505F
        push    si
        push    di
        push    cx
        mov     di,27B0h
        mov     cx,13h
        xor     byte ptr ds:[286Eh],al
        je      short L5060
        mov     si,283Ah
        rep movsw
        pop     cx
        pop     di
        pop     si
L505F:
        ret
L5060:
        mov     si,2814h
        rep movsw
        pop     cx
        pop     di
        pop     si
_seg004_5040 endp

; seg004_5068  (+5068)
; set_accept: empty here and in FM Towns (accept_flag, which it would set, is never written; see
; CLIP.ASM).
_set_accept proc near
        ret
_set_accept endp

; seg004_5069  (+5069)
; self_modify: patch the eye position in the current object's frame (2886, 2888, 288A) and the
; point shift (2880) into the immediates of mini_xlate_rotate_pnt and load_xlate_rotate_pnt, so
; that turning a model point into view space needs no memory reads for them.
_self_modify proc near
        mov     ax,word ptr ds:[2886h]
        mov     word ptr cs:[modify_mex+1],ax
        mov     word ptr cs:[L509D+1],ax
        mov     ax,word ptr ds:[2888h]
        mov     word ptr cs:[modify_mey+1],ax
        mov     word ptr cs:[L50B5+1],ax
        mov     ax,word ptr ds:[288Ah]
        mov     word ptr cs:[modify_mez+1],ax
        mov     word ptr cs:[L50A9+1],ax
        mov     al,byte ptr ds:[2880h]
        mov     byte ptr cs:[modify_mes+1],al
        mov     byte ptr cs:[_mini_xlate_rotate_pnt+1],al
        ret
_self_modify endp

; seg004_5096  (+5096)
; mini_xlate_rotate_pnt: in: SI = three signed bytes, a model point in the order x, z, y, each
; times 32. Subtracts the patched eye, shifts by the patched shift and falls into mxmul. Out:
; BX, CX, BP = the view-space point (see mxmul); SI advanced by 3. load_xlate_rotate_pnt below
; is the same for three words.
_mini_xlate_rotate_pnt proc near
        mov     cl,5                    ; the shift and the three origins are patched by self_modify
        lodsb
        cbw
        shl     ax,5
L509D:
        sub     ax,1234h
        shl     ax,cl
        mov     bx,ax
        lodsb
        cbw
        shl     ax,5
L50A9:
        sub     ax,1234h
        shl     ax,cl
        mov     bp,ax
        lodsb
        cbw
        shl     ax,5
L50B5:
        sub     ax,1234h
        shl     ax,cl
        mov     cx,ax
        jmp     short _mxmul
_mini_xlate_rotate_pnt endp

; seg004_50BE  (+50BE)
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

; seg004_50D8  (+50D8)
; mxmul: rotate a point by the view matrix. In: BX, CX, BP = the point's x, y (vertical) and z.
; Out: BX, CX, BP = view x, y and depth z, each the high word of its 1.15 products (so at half
; scale); DI, DX changed. The general body (copied from L512B) does all nine products; the flat
; body (L517E, for a level view) skips the products with the zero entries and takes y as it is,
; halved.
_mxmul proc near
        mov     ax,bx
        imul    word ptr ds:[1602h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[1608h]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[160Eh]
        add     di,dx
        mov     word ptr ds:[2708h],di
        mov     ax,bx
        imul    word ptr ds:[1604h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[160Ah]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[1610h]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[1612h]
        mov     bp,dx
        mov     ax,bx
        imul    word ptr ds:[1606h]
        add     bp,dx
        mov     ax,cx
        imul    word ptr ds:[160Ch]
        add     bp,dx
        mov     cx,di
        mov     bx,word ptr ds:[2708h]
        ret
; L512B and L517E: the two bodies check_flat and is_flat copy over the one above
L512B:
        mov     ax,bx
        imul    word ptr ds:[1602h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[1608h]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[160Eh]
        add     di,dx
        mov     word ptr ds:[2708h],di
        mov     ax,bx
        imul    word ptr ds:[1604h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[160Ah]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[1610h]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[1612h]
        mov     bp,dx
        mov     ax,bx
        imul    word ptr ds:[1606h]
        add     bp,dx
        mov     ax,cx
        imul    word ptr ds:[160Ch]
        add     bp,dx
        mov     cx,di
        mov     bx,word ptr ds:[2708h]
        ret
L517E:
        mov     ax,bx
        imul    word ptr ds:[1602h]
        mov     di,dx
        mov     ax,bp
        imul    word ptr ds:[160Eh]
        add     di,dx
        mov     ax,bx
        mov     bx,di
        imul    word ptr ds:[1606h]
        mov     ax,bp
        mov     bp,dx
        imul    word ptr ds:[1612h]
        add     bp,dx
        sar     cx,1
        ret
_mxmul endp

; seg004_51A3  (+51A3)
; seg004_51A3: choose the axis for SPHERE.ASM's first cull (modify_axis2): y when the view
; is pitched by more than 45 degrees (|160C| > 5A82h, sin 45 in 1.15), else whichever of x and z
; the view looks along more. Patches the instruction to add or subtract (by the sign) the eye's
; coordinate on that axis (2886 + offset).
_seg004_51A3 proc near
        mov     ax,word ptr ds:[160Ch]
        cwd
        xor     ax,dx
        sub     ax,dx
        cmp     ax,5A82h
        jle     short L51B5
        mov     ax,2
        jmp     short L51D7
L51B5:
        mov     ax,word ptr ds:[1606h]
        cwd
        xor     ax,dx
        sub     ax,dx
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[1612h]
        cwd
        xor     ax,dx
        sub     ax,dx
        cmp     bx,ax
        jg      short L51D2
        mov     ax,4
        jmp     short L51D7
L51D2:
        mov     ax,0
        mov     dx,cx
L51D7:
        mov     bl,2Bh
        test    dx,0FFFFh
        jns     short L51E1
        mov     bl,3
L51E1:
        mov     byte ptr cs:[1876h],bl
        mov     byte ptr cs:[1681h],bl
        mov     byte ptr cs:[1CD4h],bl
        mov     byte ptr cs:[1B6Ch],bl
        add     ax,1C8h
        mov     word ptr cs:[1878h],ax
        mov     word ptr cs:[1683h],ax
        sub     ax,1C8h
        add     ax,2886h
        mov     word ptr cs:[modify_axis2+2],ax
        mov     word ptr cs:[1B6Eh],ax
        ret
_seg004_51A3 endp

; seg004_520F  (+520F)
; UW1 only: sorts the words from 2864h to 2868h (offsets of records in seg053) by the records'
; words +2 and +0 (an insertion sort), and sets byte 286Ch. render_3d calls it after the
; frame unless 286Ch is already set.
_seg004_520F proc near
        mov     byte ptr ds:[286Ch],0FFh
        mov     cx,word ptr ds:[2868h]
        mov     si,word ptr ds:[2864h]
        mov     dx,si
        add     si,2
        cmp     cx,si
        jle     short L5258
        push    ds
        push    es
        mov     ax,seg seg053
        mov     ds,ax
        mov     es,ax
L522E:
        mov     di,si
        lodsw
        xchg    bx,ax
L5232:
        cmp     di,dx
        jle     short L5250
        mov     bp,word ptr [di-2]
        mov     ax,word ptr [bx+2]
        cmp     ax,word ptr ds:[bp+2]
        jl      short L5250
        mov     ax,word ptr [bx]
        cmp     ax,word ptr ds:[bp]
        jle     short L5250
        mov     word ptr [di],bp
        dec     di
        dec     di
        jmp     L5232
L5250:
        mov     word ptr [di],bx
        cmp     cx,si
        jne     L522E
        pop     es
        pop     ds
L5258:
        ret
        mov     cx,word ptr ds:[286Ah]
        mov     si,0Ch
        sub     cx,si
        shr     cx,1
        dec     cx
        jle     short L5298
        mov     dx,si
        add     si,2
        push    ds
        push    es
        mov     ax,seg seg053
        mov     ds,ax
        mov     es,ax
L5275:
        mov     di,si
        sub     di,2
        lodsw
        mov     bx,ax
        mov     ax,word ptr [bx]
L527F:
        mov     bp,word ptr [di]
        cmp     ax,word ptr ds:[bp]
        jl      short L5291
        mov     word ptr [di+2],bp
        sub     di,2
        cmp     di,dx
        jne     L527F
L5291:
        mov     word ptr [di+2],bx
        loop    L5275
        pop     es
        pop     ds
L5298:
        ret
        test    byte ptr ds:[286Ch],0FFh
        jne     short L52A3
        call    _seg004_520F
L52A3:
        mov     di,word ptr ds:[2866h]
        mov     si,word ptr ds:[2864h]
        mov     bp,0Ch
        mov     dx,word ptr ds:[2868h]
        mov     cx,word ptr ds:[286Ah]
        mov     word ptr ds:[2866h],si
        mov     word ptr ds:[2864h],di
        push    ds
        push    es
        mov     ax,seg seg053
        mov     es,ax
        mov     ds,ax
L52C7:
        mov     bx,word ptr [si]
        mov     ax,word ptr [bx]
        mov     bx,word ptr ds:[bp]
        cmp     ax,word ptr [bx]
        jl      short L52E7
        movsw
        cmp     si,dx
        jne     L52C7
        mov     si,bp
        sub     cx,si
        shr     cx,1
        rep movsw
        pop     es
        pop     ds
        mov     word ptr ds:[2868h],di
        ret
L52E7:
        mov     ax,bx
        stosw
        inc     bp
        inc     bp
        cmp     bp,cx
        jne     L52C7
        mov     cx,dx
        sub     cx,si
        shr     cx,1
        rep movsw
        pop     es
        pop     ds
        mov     word ptr ds:[2868h],di
        ret
_seg004_520F endp

; seg004_52FF  (+52FF)
; UW1 only: the vector BX, CX, BP times the view matrix, in 32 bits, at 26D4h..26DEh (for
; INTERP.ASM's seg004_3DC6).
_seg004_52FF proc near
        push    si
        mov     ax,bx
        imul    word ptr ds:[1602h]
        mov     si,ax
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[1608h]
        add     si,ax
        adc     di,dx
        mov     ax,bp
        imul    word ptr ds:[160Eh]
        add     si,ax
        adc     di,dx
        mov     word ptr ds:[26D4h],si
        mov     word ptr ds:[26D6h],di
        mov     ax,bx
        imul    word ptr ds:[1604h]
        mov     si,ax
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[160Ah]
        add     si,ax
        adc     di,dx
        mov     ax,bp
        imul    word ptr ds:[1610h]
        add     si,ax
        adc     di,dx
        mov     word ptr ds:[26D8h],si
        mov     word ptr ds:[26DAh],di
        mov     ax,bx
        imul    word ptr ds:[1606h]
        mov     si,ax
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[160Ch]
        add     si,ax
        adc     di,dx
        mov     ax,bp
        imul    word ptr ds:[1612h]
        add     si,ax
        adc     di,dx
        mov     word ptr ds:[26DCh],si
        mov     word ptr ds:[26DEh],di
        pop     si
        ret
_seg004_52FF endp

; seg004_5374  (+5374)
; UW1 only: as seg004_52FF, the results at 26E0h..26EAh.
_seg004_5374 proc near
        push    si
        mov     ax,bx
        imul    word ptr ds:[1602h]
        mov     si,ax
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[1608h]
        add     si,ax
        adc     di,dx
        mov     ax,bp
        imul    word ptr ds:[160Eh]
        add     si,ax
        adc     di,dx
        mov     word ptr ds:[26E0h],si
        mov     word ptr ds:[26E2h],di
        mov     ax,bx
        imul    word ptr ds:[1604h]
        mov     si,ax
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[160Ah]
        add     si,ax
        adc     di,dx
        mov     ax,bp
        imul    word ptr ds:[1610h]
        add     si,ax
        adc     di,dx
        mov     word ptr ds:[26E4h],si
        mov     word ptr ds:[26E6h],di
        mov     ax,bx
        imul    word ptr ds:[1606h]
        mov     si,ax
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[160Ch]
        add     si,ax
        adc     di,dx
        mov     ax,bp
        imul    word ptr ds:[1612h]
        add     si,ax
        adc     di,dx
        mov     word ptr ds:[26E8h],si
        mov     word ptr ds:[26EAh],di
        pop     si
        ret
_seg004_5374 endp

; seg004_53E9  (+53E9)
; instance_vvars_head: rotate the eye position in the object's frame (2886 x, 288A z) by the
; instance heading in 270A..270E, so later points can be taken relative to the rotated object.
_instance_vvars_head proc near
        mov     ax,word ptr ds:[2886h]
        imul    word ptr ds:[270Ah]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[288Ah]
        imul    word ptr ds:[270Eh]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        mov     ax,word ptr ds:[2886h]
        mov     word ptr ds:[2886h],cx
        imul    word ptr ds:[270Ch]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[288Ah]
        imul    word ptr ds:[270Ah]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[288Ah],dx
        ret
_instance_vvars_head endp

; seg004_5426  (+5426)
; instance_head: enter a sub-object turned by a heading. In: AX = the angle. Gets sin and cos
; (sincos, IMATH.ASM, called with DS = SS), rotates the eye (instance_vvars_head) and multiplies
; the view matrix by the rotation about the vertical axis, saturating each entry to 1.15. The
; interpreter saves and restores the matrix around the sub-object (INTERP.ASM, opcode 50h and
; the do_ihcall family).
_instance_head proc near
        mov     ax,ss
        mov     ds,ax
        call    far ptr _sincos
        mov     dx,es
        mov     ds,dx
        mov     word ptr ds:[270Ch],ax
        mov     word ptr ds:[270Ah],bx
        neg     ax
        mov     word ptr ds:[270Eh],ax
        call    _instance_vvars_head
        mov     ax,word ptr ds:[270Ah]
        imul    word ptr ds:[1602h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[270Eh]
        imul    word ptr ds:[160Eh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L5468
        jg      short L5465
        mov     cx,8000h
        jmp     short L5468
L5465:
        mov     cx,7FFFh
L5468:
        mov     word ptr ds:[26D4h],cx
        mov     ax,word ptr ds:[270Ah]
        imul    word ptr ds:[1604h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[270Eh]
        imul    word ptr ds:[1610h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L5492
        jg      short L548F
        mov     cx,8000h
        jmp     short L5492
L548F:
        mov     cx,7FFFh
L5492:
        mov     word ptr ds:[26D6h],cx
        mov     ax,word ptr ds:[270Ah]
        imul    word ptr ds:[1606h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[270Eh]
        imul    word ptr ds:[1612h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L54BC
        jg      short L54B9
        mov     cx,8000h
        jmp     short L54BC
L54B9:
        mov     cx,7FFFh
L54BC:
        mov     word ptr ds:[26D8h],cx
        mov     ax,word ptr ds:[270Ch]
        imul    word ptr ds:[1602h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[270Ah]
        imul    word ptr ds:[160Eh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L54E6
        jg      short L54E3
        mov     cx,8000h
        jmp     short L54E6
L54E3:
        mov     cx,7FFFh
L54E6:
        mov     word ptr ds:[160Eh],cx
        mov     ax,word ptr ds:[270Ch]
        imul    word ptr ds:[1604h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[270Ah]
        imul    word ptr ds:[1610h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L5510
        jg      short L550D
        mov     cx,8000h
        jmp     short L5510
L550D:
        mov     cx,7FFFh
L5510:
        mov     word ptr ds:[1610h],cx
        mov     ax,word ptr ds:[270Ch]
        imul    word ptr ds:[1606h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[270Ah]
        imul    word ptr ds:[1612h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L553A
        jg      short L5537
        mov     cx,8000h
        jmp     short L553A
L5537:
        mov     cx,7FFFh
L553A:
        mov     word ptr ds:[1612h],cx
        mov     ax,word ptr ds:[26D4h]
        mov     word ptr ds:[1602h],ax
        mov     ax,word ptr ds:[26D6h]
        mov     word ptr ds:[1604h],ax
        mov     ax,word ptr ds:[26D8h]
        mov     word ptr ds:[1606h],ax
        ret
_instance_head endp

; seg004_5551  (+5551)
; instance_vvars_pitch and instance_pitch, instance_vvars_bank and instance_bank: as
; instance_vvars_head and instance_head for rotations about the other two axes (pitch and bank).
_instance_vvars_pitch proc near
        mov     ax,word ptr ds:[2888h]
        imul    word ptr ds:[2716h]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[288Ah]
        imul    word ptr ds:[271Ah]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        mov     ax,word ptr ds:[2888h]
        mov     word ptr ds:[2888h],dx
        imul    word ptr ds:[2718h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[288Ah]
        imul    word ptr ds:[2716h]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[288Ah],dx
        ret
_instance_vvars_pitch endp

; seg004_558E  (+558E)
_instance_pitch proc near
        mov     ax,ss
        mov     ds,ax
        call    far ptr _sincos
        mov     dx,es
        mov     ds,dx
        mov     word ptr ds:[2718h],ax
        mov     word ptr ds:[2716h],bx
        neg     ax
        mov     word ptr ds:[271Ah],ax
        call    _instance_vvars_pitch
        mov     ax,word ptr ds:[2716h]
        imul    word ptr ds:[1608h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[271Ah]
        imul    word ptr ds:[160Eh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L55D0
        jg      short L55CD
        mov     cx,8000h
        jmp     short L55D0
L55CD:
        mov     cx,7FFFh
L55D0:
        mov     word ptr ds:[26D4h],cx
        mov     ax,word ptr ds:[2716h]
        imul    word ptr ds:[160Ah]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[271Ah]
        imul    word ptr ds:[1610h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L55FA
        jg      short L55F7
        mov     cx,8000h
        jmp     short L55FA
L55F7:
        mov     cx,7FFFh
L55FA:
        mov     word ptr ds:[26D6h],cx
        mov     ax,word ptr ds:[2716h]
        imul    word ptr ds:[160Ch]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[271Ah]
        imul    word ptr ds:[1612h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L5624
        jg      short L5621
        mov     cx,8000h
        jmp     short L5624
L5621:
        mov     cx,7FFFh
L5624:
        mov     word ptr ds:[26D8h],cx
        mov     ax,word ptr ds:[2718h]
        imul    word ptr ds:[1608h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2716h]
        imul    word ptr ds:[160Eh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L564E
        jg      short L564B
        mov     cx,8000h
        jmp     short L564E
L564B:
        mov     cx,7FFFh
L564E:
        mov     word ptr ds:[160Eh],cx
        mov     ax,word ptr ds:[2718h]
        imul    word ptr ds:[160Ah]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2716h]
        imul    word ptr ds:[1610h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L5678
        jg      short L5675
        mov     cx,8000h
        jmp     short L5678
L5675:
        mov     cx,7FFFh
L5678:
        mov     word ptr ds:[1610h],cx
        mov     ax,word ptr ds:[2718h]
        imul    word ptr ds:[160Ch]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2716h]
        imul    word ptr ds:[1612h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L56A2
        jg      short L569F
        mov     cx,8000h
        jmp     short L56A2
L569F:
        mov     cx,7FFFh
L56A2:
        mov     word ptr ds:[1612h],cx
        mov     ax,word ptr ds:[26D4h]
        mov     word ptr ds:[1608h],ax
        mov     ax,word ptr ds:[26D6h]
        mov     word ptr ds:[160Ah],ax
        mov     ax,word ptr ds:[26D8h]
        mov     word ptr ds:[160Ch],ax
        ret
_instance_pitch endp

; seg004_56B9  (+56B9)
_instance_vvars_bank proc near
        mov     ax,word ptr ds:[2888h]
        imul    word ptr ds:[2710h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2886h]
        imul    word ptr ds:[2714h]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        mov     ax,word ptr ds:[2888h]
        mov     word ptr ds:[2888h],dx
        imul    word ptr ds:[2712h]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[2886h]
        imul    word ptr ds:[2710h]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        mov     word ptr ds:[2886h],cx
        ret
_instance_vvars_bank endp

; seg004_56F6  (+56F6)
_instance_bank proc near
        mov     ax,ss
        mov     ds,ax
        call    far ptr _sincos
        mov     dx,es
        mov     ds,dx
        mov     word ptr ds:[2712h],ax
        mov     word ptr ds:[2710h],bx
        neg     ax
        mov     word ptr ds:[2714h],ax
        call    _instance_vvars_bank
        mov     ax,word ptr ds:[2710h]
        imul    word ptr ds:[1602h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2712h]
        imul    word ptr ds:[1608h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L5738
        jg      short L5735
        mov     cx,8000h
        jmp     short L5738
L5735:
        mov     cx,7FFFh
L5738:
        mov     word ptr ds:[26D4h],cx
        mov     ax,word ptr ds:[2710h]
        imul    word ptr ds:[1604h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2712h]
        imul    word ptr ds:[160Ah]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L5762
        jg      short L575F
        mov     cx,8000h
        jmp     short L5762
L575F:
        mov     cx,7FFFh
L5762:
        mov     word ptr ds:[26D6h],cx
        mov     ax,word ptr ds:[2710h]
        imul    word ptr ds:[1606h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2712h]
        imul    word ptr ds:[160Ch]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L578C
        jg      short L5789
        mov     cx,8000h
        jmp     short L578C
L5789:
        mov     cx,7FFFh
L578C:
        mov     word ptr ds:[26D8h],cx
        mov     ax,word ptr ds:[2714h]
        imul    word ptr ds:[1602h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2710h]
        imul    word ptr ds:[1608h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L57B6
        jg      short L57B3
        mov     cx,8000h
        jmp     short L57B6
L57B3:
        mov     cx,7FFFh
L57B6:
        mov     word ptr ds:[1608h],cx
        mov     ax,word ptr ds:[2714h]
        imul    word ptr ds:[1604h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2710h]
        imul    word ptr ds:[160Ah]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L57E0
        jg      short L57DD
        mov     cx,8000h
        jmp     short L57E0
L57DD:
        mov     cx,7FFFh
L57E0:
        mov     word ptr ds:[160Ah],cx
        mov     ax,word ptr ds:[2714h]
        imul    word ptr ds:[1606h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2710h]
        imul    word ptr ds:[160Ch]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L580A
        jg      short L5807
        mov     cx,8000h
        jmp     short L580A
L5807:
        mov     cx,7FFFh
L580A:
        mov     word ptr ds:[160Ch],cx
        mov     ax,word ptr ds:[26D4h]
        mov     word ptr ds:[1602h],ax
        mov     ax,word ptr ds:[26D6h]
        mov     word ptr ds:[1604h],ax
        mov     ax,word ptr ds:[26D8h]
        mov     word ptr ds:[1606h],ax
        ret
_instance_bank endp

; seg004_5821  (+5821)
; UW1 only: the object origin (2886h, 2888h, 288Ah) times the matrix at BP (seg004_5937),
; stored back.
_seg004_5821 proc near
        mov     si,2886h
        call    _seg004_5937
        mov     word ptr ds:[2886h],bx
        mov     word ptr ds:[2888h],cx
        mov     word ptr ds:[288Ah],di
        ret
_seg004_5821 endp

; seg004_5834  (+5834)
; UW1 only: seg004_5821, then the view matrix (1602h..1612h) from the matrix at SI (in SS)
; and the one at BP, a column at a time (seg004_5A83). Called by SPHERE.ASM's seg004_17AB.
_seg004_5834 proc near
        push    si
        call    _seg004_5821
        mov     si,bp
        pop     bp
        push    ds
        mov     ax,ss
        mov     ds,ax
        call    _seg004_5A83
        mov     word ptr es:[1602h],bx
        mov     word ptr es:[1604h],cx
        mov     word ptr es:[1606h],di
        add     si,2
        call    _seg004_5A83
        mov     word ptr es:[1608h],bx
        mov     word ptr es:[160Ah],cx
        mov     word ptr es:[160Ch],di
        add     si,2
        call    _seg004_5A83
        mov     word ptr es:[160Eh],bx
        mov     word ptr es:[1610h],cx
        mov     word ptr es:[1612h],di
        pop     ds
        ret
_seg004_5834 endp

; seg004_587E  (+587E)
; code_pnt: the clip codes of a view-space point. In: BX = x, CX = y, BP = z. Out: AL = 0 inside
; the frustum, else bits 1 (x < -z), 2 (x > z), 4 (y > z), 8 (y < -z), and 80h when z <= 0
; (behind the eye, with the side bits set from the signs). The four planes are CLIP.ASM's.
; Changes AX only.
_code_pnt proc near
        mov     ax,bp
        neg     ax
        jge     short L58BF
        cmp     bx,ax
        jl      short L5897
        cmp     bx,bp
        jg      short L58A8
        cmp     cx,bp
        jg      short L58B9
        cmp     cx,ax
        jl      short L58BC
        xor     al,al
        ret
L5897:
        cmp     cx,bp
        jg      short L58A2
        cmp     cx,ax
        jl      short L58A5
        mov     al,1
        ret
L58A2:
        mov     al,5
        ret
L58A5:
        mov     al,9
        ret
L58A8:
        cmp     cx,bp
        jg      short L58B3
        cmp     cx,ax
        jl      short L58B6
        mov     al,2
        ret
L58B3:
        mov     al,6
        ret
L58B6:
        mov     al,0Ah
        ret
L58B9:
        mov     al,4
        ret
L58BC:
        mov     al,8
        ret
L58BF:
        cmp     bx,ax
        jl      short L58CA
        cmp     cx,bp
        jg      short L58F0
        mov     al,8Ah
        ret
L58CA:
        cmp     bx,bp
        jg      short L58D5
        cmp     cx,bp
        jg      short L58E6
        mov     al,89h
        ret
L58D5:
        cmp     cx,bp
        jg      short L58DC
        mov     al,8Bh
        ret
L58DC:
        cmp     cx,ax
        jl      short L58E3
        mov     al,87h
        ret
L58E3:
        mov     al,8Fh
        ret
L58E6:
        cmp     cx,ax
        jl      short L58ED
        mov     al,85h
        ret
L58ED:
        mov     al,8Dh
        ret
L58F0:
        cmp     cx,ax
        jl      short L58F7
        mov     al,86h
        ret
L58F7:
        mov     al,8Eh
        ret
_code_pnt endp

; seg004_58FA  (+58FA)
; mm9x9: the view matrix (written through ES) = the 3 x 3 matrix at BP times the one at SI, row
; by row through mm3x9_bpsi. BP is advanced by 18 bytes.
_mm9x9 proc near
        call    _mm3x9_bpsi
        mov     word ptr es:[1602h],bx
        mov     word ptr es:[1604h],cx
        mov     word ptr es:[1606h],di
        add     bp,6
        call    _mm3x9_bpsi
        mov     word ptr es:[1608h],bx
        mov     word ptr es:[160Ah],cx
        mov     word ptr es:[160Ch],di
        add     bp,6
        call    _mm3x9_bpsi
        mov     word ptr es:[160Eh],bx
        mov     word ptr es:[1610h],cx
        mov     word ptr es:[1612h],di
        ret
_mm9x9 endp

; seg004_5937  (+5937)
; UW1 only: as mm3x9_bpsi with SI and BP the other way round: the row at SI times the columns
; of the matrix at BP (words +0, +6, +0Ch ...), saturated the same way.
_seg004_5937 proc near
        mov     ax,word ptr [si]
        imul    word ptr [bp]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr [si+2]
        imul    word ptr [bp+6]
        add     bx,ax
        adc     cx,dx
        mov     ax,word ptr [si+4]
        imul    word ptr [bp+0Ch]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        jno     short L5964
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5964:
        cmp     cx,8000h
        jne     short L596D
        mov     cx,8001h
L596D:
        push    cx
        mov     ax,word ptr [si]
        imul    word ptr [bp+2]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr [si+2]
        imul    word ptr [bp+8]
        add     bx,ax
        adc     cx,dx
        mov     ax,word ptr [si+4]
        imul    word ptr [bp+0Eh]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        jno     short L599B
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L599B:
        cmp     cx,8000h
        jne     short L59A4
        mov     cx,8001h
L59A4:
        mov     ax,word ptr [si]
        imul    word ptr [bp+4]
        mov     bx,ax
        mov     di,dx
        mov     ax,word ptr [si+2]
        imul    word ptr [bp+0Ah]
        add     bx,ax
        adc     di,dx
        mov     ax,word ptr [si+4]
        imul    word ptr [bp+10h]
        add     bx,ax
        adc     di,dx
        shl     bx,1
        rcl     di,1
        jno     short L59D1
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L59D1:
        cmp     cx,8000h
        jne     short L59DA
        mov     cx,8001h
L59DA:
        pop     bx
        ret
_seg004_5937 endp

; seg004_59DC  (+59DC)
; mm3x9_bpsi: a row times a matrix: BP = a row of three 1.15 words, SI = a 3 x 3 matrix by rows.
; Out: BX, CX, DI = the row's three products, saturated to +-8001h on overflow (8000h becomes
; 8001h, so negation is safe). The overflow fix-up of the third product reads and writes CX, not
; DI, so an overflow there overwrites the second result and leaves the third wrapped; FM Towns'
; mm3x9_bpsi saturates DI. Probably a slip in the DOS source.
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
        jno     short L5A09
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5A09:
        cmp     cx,8000h
        jne     short L5A12
        mov     cx,8001h
L5A12:
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
        jno     short L5A41
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5A41:
        cmp     cx,8000h
        jne     short L5A4A
        mov     cx,8001h
L5A4A:
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
        jno     short L5A78
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5A78:
        cmp     di,8000h
        jne     short L5A81
        mov     di,8001h
L5A81:
        pop     bx
        ret
_mm3x9_bpsi endp

; seg004_5A83  (+5A83)
; UW1 only: as mm3x9t, but the three products take the column at SI (words +0, +6, +0Ch)
; against the columns of BP's matrix; with the same slip in the third product's overflow
; fix-up. Called three times by seg004_5834.
_seg004_5A83 proc near
        mov     ax,word ptr [si]
        imul    word ptr [bp]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr [si+6]
        imul    word ptr [bp+6]
        add     bx,ax
        adc     cx,dx
        mov     ax,word ptr [si+0Ch]
        imul    word ptr [bp+0Ch]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        jno     short L5AB0
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5AB0:
        cmp     cx,8000h
        jne     short L5AB9
        mov     cx,8001h
L5AB9:
        push    cx
        mov     ax,word ptr [si]
        imul    word ptr [bp+2]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr [si+6]
        imul    word ptr [bp+8]
        add     bx,ax
        adc     cx,dx
        mov     ax,word ptr [si+0Ch]
        imul    word ptr [bp+0Eh]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        jno     short L5AE7
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5AE7:
        cmp     cx,8000h
        jne     short L5AF0
        mov     cx,8001h
L5AF0:
        mov     ax,word ptr [si]
        imul    word ptr [bp+4]
        mov     bx,ax
        mov     di,dx
        mov     ax,word ptr [si+6]
        imul    word ptr [bp+0Ah]
        add     bx,ax
        adc     di,dx
        mov     ax,word ptr [si+0Ch]
        imul    word ptr [bp+10h]
        add     bx,ax
        adc     di,dx
        shl     bx,1
        rcl     di,1
        jno     short L5B1D
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5B1D:
        cmp     di,8000h
        jne     short L5B26
        mov     di,8001h
L5B26:
        pop     bx
        ret
_seg004_5A83 endp

; seg004_5B28  (+5B28)
; mm3x9t: as mm3x9_bpsi with the matrix at BP taken transposed (a row at SI times the columns of
; BP's matrix), with the same slip in the third product's overflow fix-up. Called by head_view
; (CAMERA.ASM).
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
        jno     short L5B55
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5B55:
        cmp     cx,8000h
        jne     short L5B5E
        mov     cx,8001h
L5B5E:
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
        jno     short L5B8C
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5B8C:
        cmp     cx,8000h
        jne     short L5B95
        mov     cx,8001h
L5B95:
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
        jno     short L5BC2
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5BC2:
        cmp     di,8000h
        jne     short L5BCB
        mov     di,8001h
L5BCB:
        pop     bx
        ret
_mm3x9t endp

; The word the divide overflow handlers save BP in, and overflow_count (FM's name), which
; overflow_handler_reg counts in and do_scalebm (TMAPOPS.ASM) clears and tests.
L5BCD           dw      0
overflow_count  dw      0

; seg004_5BD1  (+5BD1)
; overflow_handler_reg: the int 0 handler for a 2-byte `idiv reg`: skips the instruction and
; returns AX = +-7FFFh by the sign of DX (DX the sign extension), and counts the overflow
; (do_scalebm abandons a sprite that overflowed).
_overflow_handler_reg proc far
        mov     word ptr cs:L5BCD,bp
        mov     bp,sp
        inc     word ptr [bp]
        inc     word ptr [bp]
        mov     ax,7FFFh
        xor     dx,dx
        mov     bp,word ptr cs:L5BCD
        inc     word ptr cs:overflow_count
        iret
_overflow_handler_reg endp

; seg004_5BEE  (+5BEE)
; overflow_handler_mem: for a 4-byte divide by a memory operand: skips it, AX = 7FFFh, DX = 0.
_overflow_handler_mem proc far
        mov     word ptr cs:L5BCD,bp
        mov     bp,sp
        add     word ptr [bp],4
        mov     ax,7FFFh
        xor     dx,dx
        mov     bp,word ptr cs:L5BCD
        iret
_overflow_handler_mem endp

; seg004_5C04  (+5C04)
; overflow_handler_special_bp: for a 2-byte divide by BP: skips it and returns +-7FFFh by the
; sign of DX xor BP (the quotient's sign), DX = 0.
_overflow_handler_special_bp proc far
        mov     word ptr cs:L5BCD,bp
        mov     bp,sp
        inc     word ptr [bp]
        inc     word ptr [bp]
        mov     bp,word ptr cs:L5BCD
        mov     ax,dx
        xor     ax,bp
        cwd
        mov     ax,7FFFh
        xor     ax,dx
        sub     ax,dx
        xor     dx,dx
        iret
_overflow_handler_special_bp endp

SEG004_TEXT ends

        end
