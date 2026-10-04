; /* target: seg004_1FD0 */
; /* opts: /ml */
;
; A second set of handlers for the model interpreter's opcodes 78h..9Ch (the point and polygon
; opcodes: do_defres, do_strres, do_polyres, do_closure, do_multires, do_hull, the relative
; moves do_x_rel .. do_yz_rel, do_cntres, do_addres, do_lnres, do_putpat, do_usepat, do_rod,
; do_parapiped), the same code as INTERP.ASM's without the clip codes: a point is stored
; without calling code_pnt, and a polygon is projected and drawn without clipping. They are for
; things known to be wholly inside the view.
;
; How they are used: seg051 keeps two copies of the opcode table's entries for 78h..9Ch, the
; normal set at 2814h and this set at 283Ah. seg004_5040 (INSTANCE.ASM) copies one of them into
; the table (27B0h) by byte 286Eh; check_flat keeps the relative-move entries of both sets (and
; of the live table, by 286Eh) in step with the level-view variants (flat_x_rel ...; here
; seg004_2348 .. 256C and 20CC). As the code stands, seg004_5040 only ever restores the normal
; set (it clears 286Eh and then always takes the 2814h branch), and the only other direct
; write to an offset 286Eh in the EXE (in seg006) stores 0, so this set looks unused. render_3d
; (INSTANCE.ASM) still patches biasx and biasy into seg004_2173 .. 21DA here.
;
; Entry points (the set at 283Ah, in opcode order): seg004_2232 (78h, do_parapiped),
; seg004_1FD0 (7Ah, do_defres), seg004_1FE8 (7Ch, do_strres), seg004_2156 (7Eh, do_polyres),
; seg004_21A3 (80h, do_closure), seg004_21F8 (82h, do_multires), seg004_2227 (84h, do_hull),
; seg004_223B (86h, do_x_rel), seg004_227C (88h, do_y_rel), seg004_22BD (8Ah, do_z_rel),
; seg004_22FE (8Ch, do_addres), seg004_2005 (8Eh, do_cntres), seg004_241B (90h, do_xy_rel),
; seg004_23BD (92h, do_xz_rel), seg004_2479 (94h, do_yz_rel), seg004_25B1 (96h, do_lnres),
; seg004_26DC (98h, do_putpat), seg004_260E (9Ah, do_usepat), seg004_2023 (9Ch, do_rod); the
; level-view variants seg004_2322 (y), 2348 (x), 2383 (z), 24D7 (xz), 2527 (xy), 256C (yz)
; and 20CC (rod).
;
; UW2 has none of this (its set_accept is empty and FM Towns' accept_flag is never written).
;
; name: descriptive. No FM Towns names: FM Towns has no such set.
;
; .186, where UW2 has .386: UW1's seg004 has no 386 instruction and none of the nop padding
; that TASM's .386 mode puts after forward conditional jumps.
;
; SEG004 split into its original modules: this is module 5 of 9, +1FD0..+27D0 of the segment
; (file C040h), a module of the second library.
; match: evidence for the bounds: starts on a paragraph after 10 zero bytes following SPHERE's
; last handler, and ends with 5 zero bytes before INTERP.ASM.
; Its relocations are entries 2619-2623 of UW.EXE's relocation table.

; The model interpreter's opcode table: a word per opcode, the offset of its handler
; (INTERP.ASM), in seg051, the data segment while interpreting (UW2: 24F4h).
OPCODE_TABLE    equ     2738h

        .186

        extrn   seg048:far
        extrn   _seg003_5B0B:far
        extrn   _seg003_8:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public 'code'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _load_xlate_rotate_pnt:near
        extrn   _mxmul:near
        public  _seg004_1FD0
        public  _seg004_1FE8
        public  _seg004_2005
        public  _seg004_2023
        public  _seg004_20CC
        public  _seg004_2156
        public  _seg004_21A3
        public  _seg004_21F8
        public  _seg004_2227
        public  _seg004_2232
        public  _seg004_223B
        public  _seg004_227C
        public  _seg004_22BD
        public  _seg004_22FE
        public  _seg004_2322
        public  _seg004_2348
        public  _seg004_2383
        public  _seg004_23BD
        public  _seg004_241B
        public  _seg004_2479
        public  _seg004_24D7
        public  _seg004_2527
        public  _seg004_256C
        public  _seg004_25B1
        public  _seg004_260E
        public  _seg004_26DC

        public  _seg004_2173
        public  _seg004_2181
        public  _seg004_21CE
        public  _seg004_21DA
; seg004_1FD0  (+1FD0)
; Opcode 7Ah in the no-clip set: INTERP.ASM's do_defres without the clip codes.
_seg004_1FD0 proc near
        call    _load_xlate_rotate_pnt
        lodsw
        mov     di,ax
        mov     word ptr [di+1620h],bx
        mov     word ptr [di+1622h],cx
        mov     word ptr [di+1624h],bp
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_1FD0 endp

; seg004_1FE8  (+1FE8)
; Opcode 7Ch in the no-clip set: INTERP.ASM's do_strres without the clip codes.
_seg004_1FE8 proc near
        lodsw
        mov     dx,si
        mov     si,ax
        add     si,1620h
        mov     di,309h
        movsw
        movsw
        movsw
        inc     di
        inc     di
        mov     word ptr ds:[305h],di
        mov     si,dx
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_1FE8 endp

; seg004_2005  (+2005)
; Opcode 8Eh in the no-clip set: INTERP.ASM's do_cntres without the clip codes.
_seg004_2005 proc near
        lodsw
        mov     dx,si
        mov     si,ax
        add     si,1620h
        mov     di,word ptr ds:[305h]
        movsw
        movsw
        movsw
        inc     di
        inc     di
        mov     word ptr ds:[305h],di
        mov     si,dx
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_2005 endp

; seg004_2023  (+2023)
; Opcode 9Ch in the no-clip set: INTERP.ASM's do_rod without the clip codes.
_seg004_2023 proc near
        lodsw
        or      ax,ax
        jne     short L203C
        mov     di,309h
        lodsw
        push    si
        mov     si,ax
        add     si,1620h
        movsw
        movsw
        movsw
        add     di,2
        pop     si
        jmp     short L2077
L203C:
        mov     cl,byte ptr ds:[2880h]
        shl     ax,cl
        mov     cx,ax
        imul    word ptr ds:[26D2h]
        mov     ax,cx
        mov     cx,dx
        imul    word ptr ds:[26D0h]
        mov     di,309h
        lodsw
        push    si
        mov     si,ax
        add     si,1620h
        lodsw
        add     ax,dx
        stosw
        lodsw
        add     ax,cx
        stosw
        movsw
        add     di,2
        sub     si,6
        lodsw
        sub     ax,dx
        stosw
        lodsw
        sub     ax,cx
        stosw
        movsw
        add     di,2
        pop     si
L2077:
        lodsw
        or      ax,ax
        jne     short L208D
        lodsw
        push    si
        mov     si,ax
        add     si,1620h
        movsw
        movsw
        movsw
        add     di,2
        pop     si
        jmp     short L20C5
L208D:
        mov     cl,byte ptr ds:[2880h]
        shl     ax,cl
        mov     cx,ax
        imul    word ptr ds:[26D2h]
        mov     ax,cx
        mov     cx,dx
        imul    word ptr ds:[26D0h]
        lodsw
        push    si
        mov     si,ax
        add     si,1620h
        lodsw
        sub     ax,dx
        stosw
        lodsw
        sub     ax,cx
        stosw
        movsw
        add     di,2
        sub     si,6
        lodsw
        add     ax,dx
        stosw
        lodsw
        add     ax,cx
        stosw
        movsw
        add     di,2
        pop     si
L20C5:
        mov     word ptr ds:[305h],di
        jmp     _seg004_21A3
_seg004_2023 endp

; seg004_20CC  (+20CC)
; The level-view variant of the no-clip do_rod (as flat_rod in INTERP.ASM), installed by check_flat.
_seg004_20CC proc near
        lodsw
        or      ax,ax
        jne     short L20E5
        mov     di,309h
        lodsw
        push    si
        mov     si,ax
        add     si,1620h
        movsw
        movsw
        movsw
        add     di,2
        pop     si
        jmp     short L2110
L20E5:
        mov     cl,byte ptr ds:[2880h]
        shl     ax,cl
        imul    word ptr ds:[272Ch]
        mov     di,309h
        lodsw
        push    si
        mov     si,ax
        add     si,1620h
        lodsw
        add     ax,dx
        stosw
        movsw
        movsw
        add     di,2
        sub     si,6
        lodsw
        sub     ax,dx
        stosw
        movsw
        movsw
        add     di,2
        pop     si
L2110:
        lodsw
        or      ax,ax
        jne     short L2126
        lodsw
        push    si
        mov     si,ax
        add     si,1620h
        movsw
        movsw
        movsw
        add     di,2
        pop     si
        jmp     short L2150
L2126:
        mov     cl,byte ptr ds:[2880h]
        shl     ax,cl
        imul    word ptr ds:[272Ch]
        lodsw
        push    si
        mov     si,ax
        add     si,1620h
        lodsw
        sub     ax,dx
        stosw
        movsw
        movsw
        add     di,2
        sub     si,6
        lodsw
        add     ax,dx
        mov     bx,ax
        stosw
        movsw
        movsw
        add     di,2
        pop     si
L2150:
        mov     word ptr ds:[305h],di
        jmp     short _seg004_21A3
_seg004_20CC endp

; seg004_2156  (+2156)
; Opcode 7Eh in the no-clip set: INTERP.ASM's do_polyres without the clip codes.
_seg004_2156 proc near
        lodsw
        mov     cx,ax
        mov     ax,seg seg048
        mov     es,ax
        mov     di,415Ch
        push    cx
L2162:
        lodsw
        mov     bx,ax
        mov     bp,word ptr [bx+1624h]
        mov     ax,word ptr [bx+1620h]
        imul    word ptr ds:[26B2h]
        idiv    bp
_seg004_2173:                           ; biasx or biasy, patched in by render_3d
        add     ax,4D2h
        stosw
        mov     ax,word ptr [bx+1622h]
        imul    word ptr ds:[26B0h]
        idiv    bp
_seg004_2181:                           ; biasx or biasy, patched in by render_3d
        add     ax,4D2h
        stosw
        loop    L2162
        mov     word ptr ds:[2884h],1
        pop     cx
        push    si
        mov     bp,word ptr ds:[15F6h]
        call    far ptr _seg003_5B0B
        mov     ax,ds
        mov     es,ax
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_2156 endp

; seg004_21A3  (+21A3)
; Opcode 80h in the no-clip set: INTERP.ASM's do_closure without the clip codes.
_seg004_21A3 proc near
        mov     word ptr ds:[2884h],1
        push    si
        mov     si,309h
        mov     ax,seg seg048
        mov     es,ax
        mov     di,415Ch
        mov     cx,word ptr ds:[305h]
        sub     cx,si
        shr     cx,1
        shr     cx,1
        shr     cx,1
        mov     bx,word ptr ds:[26B2h]
        push    cx
L21C6:
        lodsw
        mov     bp,word ptr [si+2]
        imul    bx
        idiv    bp
_seg004_21CE:                           ; biasx or biasy, patched in by render_3d
        add     ax,4D2h
        stosw
        mov     ax,word ptr [si]
        imul    word ptr ds:[26B0h]
        idiv    bp
_seg004_21DA:                           ; biasx or biasy, patched in by render_3d
        add     ax,4D2h
        stosw
        add     si,6
        loop    L21C6
        pop     cx
        mov     bp,word ptr ds:[15F6h]
        call    far ptr _seg003_5B0B
        mov     ax,ds
        mov     es,ax
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_21A3 endp

; seg004_21F8  (+21F8)
; Opcode 82h in the no-clip set: INTERP.ASM's do_multires without the clip codes.
_seg004_21F8 proc near
        lodsw
        mov     word ptr ds:[271Ch],ax
        lodsw
        mov     di,ax
        add     di,1620h
        mov     word ptr ds:[271Eh],di
L2207:
        call    _load_xlate_rotate_pnt
        mov     di,word ptr ds:[271Eh]
        mov     word ptr [di],bx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],bp
        add     word ptr ds:[271Eh],8
        dec     word ptr ds:[271Ch]
        jne     L2207
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_21F8 endp

; seg004_2227  (+2227)
; Opcode 84h in the no-clip set: INTERP.ASM's do_hull without the clip codes.
_seg004_2227 proc near
        lodsw
        shl     ax,1
        add     si,ax
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_2227 endp

; seg004_2232  (+2232)
; Opcode 78h in the no-clip set: INTERP.ASM's do_parapiped without the clip codes.
_seg004_2232 proc near
        add     si,0Ah
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_2232 endp

; seg004_223B  (+223B)
; Opcode 86h in the no-clip set: INTERP.ASM's do_x_rel without the clip codes.
_seg004_223B proc near
        lodsw
        mov     di,ax
        lodsw
        mov     cl,byte ptr ds:[2880h]
        shl     ax,cl
        mov     bx,word ptr [di+1620h]
        mov     cx,word ptr [di+1622h]
        mov     bp,word ptr [di+1624h]
        mov     di,ax
        imul    word ptr ds:[1602h]
        add     bx,dx
        mov     ax,word ptr ds:[1604h]
        imul    di
        add     cx,dx
        mov     ax,word ptr ds:[1606h]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        mov     word ptr [di+1620h],bx
        mov     word ptr [di+1622h],cx
        mov     word ptr [di+1624h],bp
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_223B endp

; seg004_227C  (+227C)
; Opcode 88h in the no-clip set: INTERP.ASM's do_y_rel without the clip codes.
_seg004_227C proc near
        lodsw
        mov     di,ax
        lodsw
        mov     cl,byte ptr ds:[2880h]
        shl     ax,cl
        mov     bx,word ptr [di+1620h]
        mov     cx,word ptr [di+1622h]
        mov     bp,word ptr [di+1624h]
        mov     di,ax
        imul    word ptr ds:[1608h]
        add     bx,dx
        mov     ax,word ptr ds:[160Ah]
        imul    di
        add     cx,dx
        mov     ax,word ptr ds:[160Ch]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        mov     word ptr [di+1620h],bx
        mov     word ptr [di+1622h],cx
        mov     word ptr [di+1624h],bp
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_227C endp

; seg004_22BD  (+22BD)
; Opcode 8Ah in the no-clip set: INTERP.ASM's do_z_rel without the clip codes.
_seg004_22BD proc near
        lodsw
        mov     di,ax
        lodsw
        mov     cl,byte ptr ds:[2880h]
        shl     ax,cl
        mov     bx,word ptr [di+1620h]
        mov     cx,word ptr [di+1622h]
        mov     bp,word ptr [di+1624h]
        mov     di,ax
        imul    word ptr ds:[160Eh]
        add     bx,dx
        mov     ax,word ptr ds:[1610h]
        imul    di
        add     cx,dx
        mov     ax,word ptr ds:[1612h]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        mov     word ptr [di+1620h],bx
        mov     word ptr [di+1622h],cx
        mov     word ptr [di+1624h],bp
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_22BD endp

; seg004_22FE  (+22FE)
; Opcode 8Ch in the no-clip set: INTERP.ASM's do_addres without the clip codes.
_seg004_22FE proc near
        lodsw
        mov     bp,ax
        lodsw
        mov     di,ax
        xchg    bp,si
        lea     si,[si+1620h]
        lea     di,[di+1620h]
        lodsw
        add     bx,word ptr [di]
        stosw
        lodsw
        add     bx,word ptr [di]
        stosw
        lodsw
        add     bx,word ptr [di]
        stosw
        mov     si,bp
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_22FE endp

; seg004_2322  (+2322)
; The level-view variant of the no-clip do_y_rel (as flat_y_rel in INTERP.ASM), installed by check_flat.
_seg004_2322 proc near
        lodsw
        mov     bp,ax
        lodsw
        mov     cl,byte ptr ds:[2880h]
        shl     ax,cl
        sar     ax,1
        mov     cx,ax
        lodsw
        xchg    bp,si
        mov     di,1620h
        add     si,di
        add     di,ax
        movsw
        lodsw
        add     ax,cx
        stosw
        movsw
        mov     si,bp
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_2322 endp

; seg004_2348  (+2348)
; The level-view variant of the no-clip do_x_rel (as flat_x_rel in INTERP.ASM), installed by check_flat.
_seg004_2348 proc near
        lodsw
        mov     di,ax
        lodsw
        mov     cl,byte ptr ds:[2880h]
        shl     ax,cl
        mov     bx,word ptr [di+1620h]
        mov     cx,word ptr [di+1622h]
        mov     bp,word ptr [di+1624h]
        mov     di,ax
        imul    word ptr ds:[1602h]
        add     bx,dx
        mov     ax,di
        imul    word ptr ds:[1606h]
        add     bp,dx
        lodsw
        mov     di,ax
        mov     word ptr [di+1620h],bx
        mov     word ptr [di+1622h],cx
        mov     word ptr [di+1624h],bp
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_2348 endp

; seg004_2383  (+2383)
; The level-view variant of the no-clip do_z_rel (as flat_z_rel in INTERP.ASM), installed by check_flat.
_seg004_2383 proc near
        lodsw
        mov     di,ax
        lodsw
        mov     cl,byte ptr ds:[2880h]
        shl     ax,cl
        mov     bx,word ptr [di+1620h]
        mov     cx,word ptr [di+1622h]
        mov     bp,word ptr [di+1624h]
        mov     di,ax
        imul    word ptr ds:[160Eh]
        add     bx,dx
        mov     ax,word ptr ds:[1612h]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        mov     word ptr [di+1620h],bx
        mov     word ptr [di+1622h],cx
        mov     word ptr [di+1624h],bp
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_2383 endp

; seg004_23BD  (+23BD)
; Opcode 92h in the no-clip set: INTERP.ASM's do_xz_rel without the clip codes.
_seg004_23BD proc near
        mov     cl,byte ptr ds:[2880h]
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[1602h]
        mov     bx,dx
        mov     ax,word ptr ds:[1606h]
        imul    di
        mov     bp,dx
        mov     ax,word ptr ds:[1604h]
        imul    di
        lodsw
        shl     ax,cl
        mov     cx,dx
        mov     di,ax
        imul    word ptr ds:[160Eh]
        add     bx,dx
        mov     ax,word ptr ds:[1610h]
        imul    di
        add     cx,dx
        mov     ax,word ptr ds:[1612h]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        add     bx,word ptr [di+1620h]
        add     cx,word ptr [di+1622h]
        add     bp,word ptr [di+1624h]
        lodsw
        mov     di,ax
        mov     word ptr [di+1620h],bx
        mov     word ptr [di+1622h],cx
        mov     word ptr [di+1624h],bp
        mov     byte ptr [di+1626h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_23BD endp

; seg004_241B  (+241B)
; Opcode 90h in the no-clip set: INTERP.ASM's do_xy_rel without the clip codes.
_seg004_241B proc near
        mov     cl,byte ptr ds:[2880h]
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[1602h]
        mov     bx,dx
        mov     ax,word ptr ds:[1606h]
        imul    di
        mov     bp,dx
        mov     ax,word ptr ds:[1604h]
        imul    di
        lodsw
        shl     ax,cl
        mov     cx,dx
        mov     di,ax
        imul    word ptr ds:[1608h]
        add     bx,dx
        mov     ax,word ptr ds:[160Ah]
        imul    di
        add     cx,dx
        mov     ax,word ptr ds:[160Ch]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        add     bx,word ptr [di+1620h]
        add     cx,word ptr [di+1622h]
        add     bp,word ptr [di+1624h]
        lodsw
        mov     di,ax
        mov     word ptr [di+1620h],bx
        mov     word ptr [di+1622h],cx
        mov     word ptr [di+1624h],bp
        mov     byte ptr [di+1626h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_241B endp

; seg004_2479  (+2479)
; Opcode 94h in the no-clip set: INTERP.ASM's do_yz_rel without the clip codes.
_seg004_2479 proc near
        mov     cl,byte ptr ds:[2880h]
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[160Eh]
        mov     bx,dx
        mov     ax,word ptr ds:[1612h]
        imul    di
        mov     bp,dx
        mov     ax,word ptr ds:[1610h]
        imul    di
        lodsw
        shl     ax,cl
        mov     cx,dx
        mov     di,ax
        imul    word ptr ds:[1608h]
        add     bx,dx
        mov     ax,word ptr ds:[160Ah]
        imul    di
        add     cx,dx
        mov     ax,word ptr ds:[160Ch]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        add     bx,word ptr [di+1620h]
        add     cx,word ptr [di+1622h]
        add     bp,word ptr [di+1624h]
        lodsw
        mov     di,ax
        mov     word ptr [di+1620h],bx
        mov     word ptr [di+1622h],cx
        mov     word ptr [di+1624h],bp
        mov     byte ptr [di+1626h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_2479 endp

; seg004_24D7  (+24D7)
; The level-view variant of the no-clip do_xz_rel (as flat_xz_rel in INTERP.ASM), installed by check_flat.
_seg004_24D7 proc near
        mov     cl,byte ptr ds:[2880h]
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[1602h]
        mov     bx,dx
        mov     ax,word ptr ds:[1606h]
        imul    di
        mov     bp,dx
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[160Eh]
        add     bx,dx
        mov     ax,word ptr ds:[1612h]
        imul    di
        add     bp,dx
        lodsw
        mov     di,ax
        add     bx,word ptr [di+1620h]
        add     bp,word ptr [di+1624h]
        mov     cx,word ptr [di+1622h]
        lodsw
        mov     di,ax
        mov     word ptr [di+1620h],bx
        mov     word ptr [di+1622h],cx
        mov     word ptr [di+1624h],bp
        mov     byte ptr [di+1626h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_24D7 endp

; seg004_2527  (+2527)
; The level-view variant of the no-clip do_xy_rel (as flat_xy_rel in INTERP.ASM), installed by check_flat.
_seg004_2527 proc near
        mov     cl,byte ptr ds:[2880h]
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[1602h]
        mov     bx,dx
        mov     ax,word ptr ds:[1606h]
        imul    di
        mov     bp,dx
        lodsw
        shl     ax,cl
        sar     ax,1
        mov     cx,ax
        lodsw
        mov     di,ax
        add     bx,word ptr [di+1620h]
        add     cx,word ptr [di+1622h]
        add     bp,word ptr [di+1624h]
        lodsw
        mov     di,ax
        mov     word ptr [di+1620h],bx
        mov     word ptr [di+1622h],cx
        mov     word ptr [di+1624h],bp
        mov     byte ptr [di+1626h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_2527 endp

; seg004_256C  (+256C)
; The level-view variant of the no-clip do_yz_rel (as flat_yz_rel in INTERP.ASM), installed by check_flat.
_seg004_256C proc near
        mov     cl,byte ptr ds:[2880h]
        lodsw
        shl     ax,cl
        mov     di,ax
        imul    word ptr ds:[160Eh]
        mov     bx,dx
        mov     ax,word ptr ds:[1612h]
        imul    di
        mov     bp,dx
        lodsw
        shl     ax,cl
        sar     ax,1
        mov     cx,ax
        lodsw
        mov     di,ax
        add     bx,word ptr [di+1620h]
        add     cx,word ptr [di+1622h]
        add     bp,word ptr [di+1624h]
        lodsw
        mov     di,ax
        mov     word ptr [di+1620h],bx
        mov     word ptr [di+1622h],cx
        mov     word ptr [di+1624h],bp
        mov     byte ptr [di+1626h],al
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_256C endp

; seg004_25B1  (+25B1)
; Opcode 96h in the no-clip set: INTERP.ASM's do_lnres without the clip codes.
_seg004_25B1 proc near
        lodsw
        mov     di,ax
        lodsw
        mov     bx,ax
        push    si
        mov     bp,word ptr [di+1624h]
        mov     ax,word ptr [di+1620h]
        imul    word ptr ds:[26B2h]
        idiv    bp
        add     ax,word ptr ds:[26B4h]
        mov     cx,ax
        mov     ax,word ptr [di+1622h]
        imul    word ptr ds:[26B0h]
        idiv    bp
        add     ax,word ptr ds:[26B6h]
        mov     si,ax
        mov     di,bx
        mov     bp,word ptr [di+1624h]
        mov     ax,word ptr [di+1622h]
        imul    word ptr ds:[26B0h]
        idiv    bp
        add     ax,word ptr ds:[26B6h]
        mov     bx,ax
        mov     ax,word ptr [di+1620h]
        imul    word ptr ds:[26B2h]
        idiv    bp
        add     ax,word ptr ds:[26B4h]
        mov     dx,si
        call    far ptr _seg003_8
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_25B1 endp

; seg004_260E  (+260E)
; Opcode 9Ah in the no-clip set: INTERP.ASM's do_usepat without the clip codes.
_seg004_260E proc near
        mov     cl,byte ptr ds:[2880h]
        mov     bx,word ptr ds:[2886h]
        neg     bx
        shl     bx,cl
        mov     bp,word ptr ds:[288Ah]
        neg     bp
        shl     bp,cl
        mov     ax,word ptr ds:[2888h]
        neg     ax
        shl     ax,cl
        mov     cx,ax
        call    _mxmul
        mov     word ptr ds:[15FCh],bx
        mov     word ptr ds:[15FEh],cx
        mov     word ptr ds:[1600h],bp
        mov     di,1620h
        lodsw
        add     di,ax
        lodsw
        push    si
        add     si,ax
        lodsw
        mov     dx,ax
        lodsw
        sub     al,byte ptr ds:[2880h]
        mov     dh,al
        shr     dl,1
        jae     short L267D
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
        mov     word ptr [di],bx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],bp
        mov     byte ptr [di+6],al
        add     di,8
L267D:
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
        mov     word ptr [di+8],bx
        mov     word ptr [di+0Ah],cx
        mov     word ptr [di+0Ch],bp
        mov     byte ptr [di+0Eh],al
        add     di,10h
        dec     dl
        jne     L267D
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_260E endp

; seg004_26DC  (+26DC)
; Opcode 98h in the no-clip set: INTERP.ASM's do_putpat without the clip codes.
_seg004_26DC proc near
        push    word ptr ds:[2886h]
        push    word ptr ds:[2888h]
        push    word ptr ds:[288Ah]
        call    _load_xlate_rotate_pnt
        mov     word ptr ds:[15FCh],bx
        mov     word ptr ds:[15FEh],cx
        mov     word ptr ds:[1600h],bp
        mov     ax,word ptr ds:[2886h]
        sub     ax,word ptr [si-6]
        mov     word ptr ds:[2886h],ax
        mov     ax,word ptr ds:[2888h]
        sub     ax,word ptr [si-2]
        mov     word ptr ds:[2888h],ax
        mov     ax,word ptr ds:[288Ah]
        sub     ax,word ptr [si-4]
        mov     word ptr ds:[288Ah],ax
        mov     bp,1620h
        lodsw
        add     di,ax
        lodsw
        push    si
        add     si,ax
        lodsw
        mov     dx,ax
        lodsw
        sub     al,byte ptr ds:[2880h]
        mov     dh,al
        shr     dl,1
        jae     short L2755
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
        mov     word ptr [di],bx
        mov     word ptr [di+2],cx
        mov     word ptr [di+4],bp
        mov     byte ptr [di+6],al
        add     di,8
L2755:
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
        mov     word ptr [di+8],bx
        mov     word ptr [di+0Ah],cx
        mov     word ptr [di+0Ch],bp
        mov     byte ptr [di+0Eh],al
        add     di,10h
        dec     dl
        jne     L2755
        pop     si
        lodsw
        push    si
        add     si,ax
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        pop     word ptr ds:[288Ah]
        pop     word ptr ds:[2888h]
        pop     word ptr ds:[2886h]
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_26DC endp

SEG004_TEXT ends

        end
