; /* target: seg004_1370 */
; /* opts: /ml */
;
; sphere_check, get_dist, do_obj (the model opcode handler) and UW1's other object headers.
;
; What it does in the game: do_obj is opcode 38h of the model interpreter (INTERP.ASM), the
; header that starts each placed thing in the render database. It places the thing (sets the
; object origin and the coordinate shift) and culls it, skipping its whole program when it is
; behind the eye, out of 16-bit range or outside the view, using sphere_check's bounding-sphere
; test. get_dist is a distance estimate; in UW1 seg004_15AC and seg004_17AB call it (nothing
; in UW2's EXE does).
;
; UW1 against UW2: sphere_check, get_dist and do_obj are UW2's code, with UW1's data offsets.
; UW1 adds more handlers of the same kind, in opcode slots that are do_int2 in UW2 (and in FM
; Towns): seg004_15AC (opcode 1Eh), seg004_1A8D (6Ah), seg004_1D43 (20h: a point given in
; 32-bit world coordinates, taken relative to the eye and scaled down when it does not fit in
; 16 bits), seg004_1E84 (28h) and seg004_1E79 (30h, the same with its program address read
; through a pointer), and seg004_17AB, which render_3d (INSTANCE.ASM) calls for each record
; before the database is run. Like do_obj they call self_modify and seg004_5040 (INSTANCE.ASM,
; which puts the clipping handlers back, see NOCLIP.ASM). They are not traced further.
;
; Data: none of its own. It works in seg051 (the interpreter's DS): the matrix at 1602h, the
; eye position at 26BAh, the object origin at 2886h..288Ah, the shift at 2880h, the frustum
; factors 2726h..272Ah from scale_matrix, scratch words at 1C0h..1E0h, and the bit-length table
; at 1E2h (entry v = 7 - the bit length of v).
;
; name: UW2's (descriptive there); the routine names are FM Towns', through UW2. do_obj keeps
; no underscore, as in UW2: DRAWOBJ.C has a C function do_obj (_do_obj).
;
; .186, where UW2 has .386: UW1's seg004 has no 386 instruction and none of the nop padding
; that TASM's .386 mode puts after forward conditional jumps.
;
; SEG004 split into its original modules: this is module 4 of 9, +1370..+1FD0 of the segment
; (file B3E0h), a module of the second library. UW2 has it second, right after EXPAND.ASM.
; match: evidence for the bounds: starts on a paragraph after 10 zero bytes following CAMERA's
; last ret, and ends with 10 zero bytes before NOCLIP.ASM.
; It has no relocations.

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
PNT_X           equ     1620h   ; the point table, 8 bytes a point: view x
PNT_Y           equ     1622h   ; y
PNT_Z           equ     1624h   ; z, the depth
PNT_CODES       equ     1626h   ; the clip codes (code_pnt)
EYE_X           equ     26BAh   ; the eye position, three 32-bit coordinates
EYE_Y           equ     26BEh
EYE_Z           equ     26C2h
OBJ_SHIFT       equ     2880h   ; the current object's coordinate shift
OBJ_X           equ     2886h   ; the current object's origin, eye-relative and negated
OBJ_Y           equ     2888h
OBJ_Z           equ     288Ah

        .186

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
        extrn   _instance_vvars_bank:near
        extrn   _instance_vvars_head:near
        extrn   _instance_vvars_pitch:near
        extrn   _mxmul:near
        extrn   _seg004_5040:near
        extrn   _seg004_5821:near
        extrn   _seg004_5834:near
        extrn   _self_modify:near
        public  _get_dist
        public  _seg004_15AC
        public  _seg004_17AB
        public  _seg004_1A8D
        public  _seg004_1D43
        public  _seg004_1E79
        public  _seg004_1E84
        public  _sphere_check
        public  do_obj
        public  modify_axis2

; seg004_1370  (+1370)
; Bounding-sphere test against the view frustum. In: BX, AX, BP = the object origin words
; (2886h, 2888h, 288Ah: negated eye-relative x, y, z), SI = radius, CL = shift. Shifts all
; four by CL and negates the position, takes its depth along the matrix's z column (kept at
; ds:0Eh, the shift at ds:10h), and tests it against the near plane and the four side planes
; (the x and y columns, with the radius scaled by 2728h and 272Ah). Out: CF set when the
; sphere is wholly outside one plane. Otherwise CF clear, SI = -1 when the sphere is wholly
; inside (the first path) or 0 when it crosses a plane (the second, from L147F), and AX =
; the depth shifted back by the original shift (scaled by 8000h / -24E6h when 2726h is
; negative, 7FFFh if that overflows) and halved; 0 when the centre is behind the eye. Called
; only by do_obj.
_sphere_check proc near
        mov     word ptr ds:[1D0h],cx
        shl     si,cl
        shl     bx,cl
        neg     bx
        shl     bp,cl
        neg     bp
        shl     ax,cl
        neg     ax
        mov     cx,ax
        mov     ax,bx
        imul    word ptr ds:[MAT_XZ]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[MAT_YZ]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[MAT_ZZ]
        add     di,dx
        mov     word ptr ds:[1CEh],di
        jns     short L13AC
        add     di,si
        jns     short L13A9
        jmp     L151D
L13A9:
        jmp     L147F
L13AC:
        mov     ax,bx
        imul    word ptr ds:[MAT_XX]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[MAT_YX]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[MAT_ZX]
        add     di,dx
        mov     ax,si
        mul     word ptr ds:[2728h]
        mov     ax,word ptr ds:[1CEh]
        sub     ax,di
        jns     short L13DB
        add     ax,dx
        jns     short L13D8
        jmp     L151D
L13D8:
        jmp     L14A8
L13DB:
        sub     ax,dx
        jns     short L13E2
        jmp     L14A8
L13E2:
        add     di,word ptr ds:[1CEh]
        jns     short L13F2
        add     di,dx
        jns     short L13EF
        jmp     L151D
L13EF:
        jmp     L14B2
L13F2:
        sub     di,dx
        jns     short L13F9
        jmp     L14B2
L13F9:
        mov     ax,bx
        imul    word ptr ds:[MAT_XY]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[MAT_YY]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[MAT_ZY]
        add     di,dx
        mov     ax,si
        mul     word ptr ds:[272Ah]
        mov     ax,word ptr ds:[1CEh]
        sub     ax,di
        jns     short L1428
        add     ax,dx
        jns     short L1425
        jmp     L151D
L1425:
        jmp     L14DB
L1428:
        sub     ax,dx
        jns     short L142F
        jmp     L14DB
L142F:
        add     di,word ptr ds:[1CEh]
        jns     short L143F
        add     di,dx
        jns     short L143C
        jmp     L151D
L143C:
        jmp     L14E5
L143F:
        sub     di,dx
        jns     short L1446
        jmp     L14E5
L1446:
        mov     ax,word ptr ds:[1CEh]
        or      ax,ax
        jns     short L1451
        xor     ax,ax
        jmp     short L1478
L1451:
        mov     cx,word ptr ds:[1D0h]
        sar     ax,cl
        test    word ptr ds:[2726h],0FFFFh
        jns     short L1478
        mov     cx,word ptr ds:[2726h]
        neg     cx
        mov     dx,ax
        xor     ax,ax
        sar     dx,1
        cmp     dx,cx
        jl      short L1474
        mov     ax,7FFFh
        jmp     short L1478
L1474:
        rcr     ax,1
        div     cx
L1478:
        shr     ax,1
        clc
        mov     si,0FFFFh
        ret
L147F:
        mov     ax,bx
        imul    word ptr ds:[MAT_XX]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[MAT_YX]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[MAT_ZX]
        add     di,dx
        mov     ax,si
        mul     word ptr ds:[2728h]
        mov     ax,word ptr ds:[1CEh]
        sub     ax,di
        jns     short L14A8
        add     ax,dx
        js      short L151D
L14A8:
        add     di,word ptr ds:[1CEh]
        jns     short L14B2
        add     di,dx
        js      short L151D
L14B2:
        mov     ax,bx
        imul    word ptr ds:[MAT_XY]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[MAT_YY]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[MAT_ZY]
        add     di,dx
        mov     ax,si
        mul     word ptr ds:[272Ah]
        mov     ax,word ptr ds:[1CEh]
        sub     ax,di
        jns     short L14DB
        add     ax,dx
        js      short L151D
L14DB:
        add     di,word ptr ds:[1CEh]
        jns     short L14E5
        add     di,dx
        js      short L151D
L14E5:
        mov     ax,word ptr ds:[1CEh]
        or      ax,ax
        jns     short L14F0
        xor     ax,ax
        jmp     short L1517
L14F0:
        mov     cx,word ptr ds:[1D0h]
        sar     ax,cl
        test    word ptr ds:[2726h],0FFFFh
        jns     short L1517
        mov     cx,word ptr ds:[2726h]
        neg     cx
        mov     dx,ax
        xor     ax,ax
        sar     dx,1
        cmp     dx,cx
        jl      short L1513
        mov     ax,7FFFh
        jmp     short L1517
L1513:
        rcr     ax,1
        div     cx
L1517:
        shr     ax,1
        clc
        xor     si,si
        ret
L151D:
        stc
        ret
_sphere_check endp

; seg004_151F  (+151F)
; Distance estimate from the three 32-bit eye-relative coordinates do_obj leaves at ds:16h,
; 1Ah and 1Eh: takes absolute values and combines the largest with fractions of the others
; (halvings and quarterings, an octagonal approximation). Out: BP:BX. In the y term the
; absolute value's correction is added to CX, not SI (`add cx,dx` after `xor si,dx`), which
; looks like a slip. UW2 never calls it; UW1's seg004_15AC and seg004_17AB do, so there the
; slip is live (not checked in play).
_get_dist proc near
        push    si
        mov     bx,word ptr ds:[1D6h]
        mov     ax,word ptr ds:[1D8h]
        cwd
        xor     bx,dx
        xor     ax,dx
        neg     dx
        add     bx,dx
        adc     ax,0
        mov     bp,ax
        mov     cx,word ptr ds:[1DEh]
        mov     ax,word ptr ds:[1E0h]
        cwd
        xor     cx,dx
        xor     ax,dx
        neg     dx
        add     cx,dx
        adc     ax,0
        cmp     bp,ax
        jl      short L1555
        jg      short L1552
        cmp     bx,cx
        jl      short L1555
L1552:
        xchg    bp,ax
        xchg    bx,cx
L1555:
        sar     bp,1
        rcr     bx,1
        sar     bp,1
        rcr     bx,1
        mov     di,ax
        mov     si,word ptr ds:[1DAh]
        mov     ax,word ptr ds:[1DCh]
        cwd
        xor     si,dx
        xor     ax,dx
        neg     dx
        add     cx,dx
        adc     ax,0
        cmp     ax,di
        jl      short L158A
        jg      short L157C
        cmp     si,cx
        jl      short L158A
L157C:
        add     bx,cx
        adc     bp,di
        sar     bp,1
        rcr     bx,1
        sar     bp,1
        rcr     bx,1
        jmp     short L15A6
L158A:
        sar     ax,1
        rcr     si,1
        cmp     ax,bp
        jl      short L159E
        jg      short L1598
        cmp     si,bx
        jl      short L159E
L1598:
        sar     bp,1
        rcr     bx,1
        jmp     short L15A2
L159E:
        sar     ax,1
        rcr     si,1
L15A2:
        add     si,cx
        adc     ax,di
L15A6:
        add     bx,si
        adc     bp,ax
        pop     si
        ret
_get_dist endp

; seg004_15AC  (+15AC)
; UW1 only: opcode 1Eh, an object header that calls get_dist, self_modify and seg004_5040
; (see the module header); not traced further.
_seg004_15AC proc near
        lea     ax,[si+0Eh]
        push    ax
        lodsw
        mov     di,ax
        add     di,si
        mov     es,word ptr [di-8]
        mov     dx,word ptr ds:[EYE_X]
        lodsw
        sub     dx,ax
        mov     word ptr ds:[1D6h],dx
        mov     bx,word ptr ds:[EYE_X+2]
        lodsw
        sbb     bx,ax
        mov     word ptr ds:[1D8h],bx
        xchg    dx,ax
        mov     cx,es
        jcxz    L15E7
        or      cx,cx
        jns     short L15E1
        neg     cx
L15D9:
        shl     ax,1
        rcl     bx,1
        loop    L15D9
        jmp     short L15E7
L15E1:
        sar     bx,1
        rcr     ax,1
        loop    L15E1
L15E7:
        cwd
        cmp     bx,dx
        je      short L15EF
        jmp     L16FE
L15EF:
        mov     word ptr ds:[1C8h],ax
        cwd
        xor     ax,dx
        sub     ax,dx
        mov     bp,ax
        mov     dx,word ptr ds:[EYE_Y]
        lodsw
        sub     dx,ax
        mov     word ptr ds:[1DAh],dx
        mov     bx,word ptr ds:[EYE_Y+2]
        lodsw
        sbb     bx,ax
        mov     word ptr ds:[1DCh],bx
        xchg    dx,ax
        mov     cx,es
        jcxz    L1628
        or      cx,cx
        jns     short L1622
        neg     cx
        shl     ax,1
        rcl     bx,1
        loop    L15D9
        jmp     short L1628
L1622:
        sar     bx,1
        rcr     ax,1
        loop    L1622
L1628:
        cwd
        cmp     bx,dx
        je      short L1630
        jmp     L16FE
L1630:
        mov     word ptr ds:[1CAh],ax
        cwd
        xor     ax,dx
        sub     ax,dx
        or      bp,ax
        mov     dx,word ptr ds:[EYE_Z]
        lodsw
        sub     dx,ax
        mov     word ptr ds:[1DEh],dx
        mov     bx,word ptr ds:[EYE_Z+2]
        lodsw
        sbb     bx,ax
        mov     word ptr ds:[1E0h],bx
        xchg    dx,ax
        mov     cx,es
        jcxz    L1669
        or      cx,cx
        jns     short L1663
        neg     cx
L165B:
        shl     ax,1
        rcl     bx,1
        loop    L165B
        jmp     short L1669
L1663:
        sar     bx,1
        rcr     ax,1
        loop    L1663
L1669:
        cwd
        cmp     bx,dx
        je      short L1671
        jmp     L16FE
L1671:
        mov     word ptr ds:[1CCh],ax
        cwd
        xor     ax,dx
        sub     ax,dx
        or      bp,ax
        push    di
        mov     si,word ptr [di-0Ah]
        mov     ax,si
        add     ax,word ptr ds:[1C8h]
        jns     short L168A
        jmp     L170F
L168A:
        or      bp,si
        mov     ax,bp
        mov     bx,1E2h
        or      ah,ah
        je      short L169A
        xchg    al,ah
        xlatb
        jmp     short L169D
L169A:
        xlatb
        add     al,8
L169D:
        mov     cl,al
        mov     bx,word ptr ds:[1C8h]
        mov     bp,word ptr ds:[1CCh]
        mov     ax,word ptr ds:[1CAh]
        call    _sphere_check
        jb      short L170F
        mov     word ptr ds:[1D2h],ax
        call    _get_dist
        les     di,dword ptr ds:[2860h]
        mov     dx,di
        xchg    bx,ax
        stosw
        xchg    bp,ax
        stosw
        mov     ax,0
        stosw
        pop     ax
        stosw
        mov     ax,word ptr ds:[1C8h]
        stosw
        mov     ax,word ptr ds:[1CAh]
        stosw
        mov     ax,word ptr ds:[1CCh]
        stosw
        mov     ax,word ptr ds:[1D2h]
        stosw
        xchg    si,ax
        stosw
        mov     si,1D6h
        mov     cx,6
        rep movsw
        mov     word ptr ds:[286Ch],0
        mov     word ptr ds:[2860h],di
        mov     di,word ptr ds:[2868h]
        xchg    dx,ax
        stosw
        mov     word ptr ds:[2868h],di
        mov     ax,ds
        mov     es,ax
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L16FE:
        mov     word ptr ds:[2882h],0FFFFh
        mov     ax,ds
        mov     es,ax
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L170F:
        mov     ax,ds
        mov     es,ax
        pop     ax
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
        push    ds
        push    si
        push    cx
        mov     si,di
        add     si,6
        lodsw
        xchg    dx,ax
        lodsw
        xchg    bx,ax
        lodsw
        xchg    cx,ax
        lodsw
        xchg    bp,ax
        lodsw
        mov     word ptr es:[289Ah],ax
        lodsw
        push    cx
        mov     di,1D6h
        mov     cx,6
        rep movsw
        pop     cx
        xchg    di,ax
        mov     ax,es
        mov     ds,ax
        mov     word ptr ds:[2882h],0
        mov     word ptr ds:[1C4h],1
        mov     si,dx
        mov     word ptr ds:[OBJ_X],bx
        mov     ax,bx
        cwd
        xor     bx,dx
        sub     bx,dx
        add     bx,word ptr [si-6]
        mov     word ptr ds:[OBJ_Y],cx
        xchg    cx,ax
        cwd
        xor     ax,dx
        sub     ax,dx
        add     ax,word ptr [si-4]
        or      bx,ax
        mov     word ptr ds:[OBJ_Z],bp
        xchg    bp,ax
        cwd
        xor     ax,dx
        sub     ax,dx
        add     ax,word ptr [si-2]
        or      bx,ax
        mov     ax,bx
        mov     bx,1E2h
        or      ah,ah
        je      short L1788
        xchg    al,ah
        xlatb
        jmp     short L178B
L1788:
        xlatb
        add     al,8
L178B:
        mov     byte ptr ds:[OBJ_SHIFT],al
        call    _self_modify
        mov     ax,di
        call    _seg004_5040
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     cx
        pop     si
        pop     ds
        ret
        mov     word ptr es:[2882h],0FFFFh
        pop     cx
        pop     si
        pop     ds
        ret
_seg004_15AC endp

; seg004_17AB  (+17AB)
; UW1 only: called by render_3d for each record from 161Eh before the database runs (BP = the
; record + 6); takes the record's position relative to the eye (26BAh..) and calls get_dist;
; not traced further.
_seg004_17AB proc near
        mov     word ptr ds:[1C0h],bp
        add     bp,2
        mov     di,word ptr [bp]
        mov     es,word ptr [di-8]
        mov     ax,word ptr ds:[EYE_X]
        sub     ax,word ptr [bp+2]
        mov     word ptr ds:[1D6h],ax
        mov     bx,word ptr ds:[EYE_X+2]
        sbb     bx,word ptr [bp+4]
        mov     word ptr ds:[1D8h],bx
        mov     cx,es
        jcxz    L17E4
        or      cx,cx
        jns     short L17DE
        neg     cx
L17D6:
        shl     ax,1
        rcl     bx,1
        loop    L17D6
        jmp     short L17E4
L17DE:
        sar     bx,1
        rcr     ax,1
        loop    L17DE
L17E4:
        cwd
        cmp     bx,dx
        je      short L17EC
        jmp     L18EC
L17EC:
        mov     word ptr ds:[1C8h],ax
        xor     ax,dx
        sub     ax,dx
        mov     si,ax
        mov     ax,word ptr ds:[EYE_Y]
        sub     ax,word ptr [bp+6]
        mov     word ptr ds:[1DAh],ax
        mov     bx,word ptr ds:[EYE_Y+2]
        sbb     bx,word ptr [bp+8]
        mov     word ptr ds:[1DCh],bx
        mov     cx,es
        jcxz    L1821
        or      cx,cx
        jns     short L181B
        neg     cx
L1813:
        shl     ax,1
        rcl     bx,1
        loop    L1813
        jmp     short L1821
L181B:
        sar     bx,1
        rcr     ax,1
        loop    L181B
L1821:
        cwd
        cmp     bx,dx
        je      short L1829
        jmp     L18EC
L1829:
        mov     word ptr ds:[1CAh],ax
        xor     ax,dx
        sub     ax,dx
        or      si,ax
        mov     ax,word ptr ds:[EYE_Z]
        sbb     ax,word ptr [bp+0Ah]
        mov     word ptr ds:[1DEh],ax
        mov     bx,word ptr ds:[EYE_Z+2]
        sbb     bx,word ptr [bp+0Ch]
        mov     word ptr ds:[1E0h],bx
        mov     cx,es
        jcxz    L185E
        or      cx,cx
        jns     short L1858
        neg     cx
L1850:
        shl     ax,1
        rcl     bx,1
        loop    L1850
        jmp     short L185E
L1858:
        sar     bx,1
        rcr     ax,1
        loop    L1858
L185E:
        cwd
        cmp     bx,dx
        je      short L1866
        jmp     L18EC
L1866:
        mov     word ptr ds:[1CCh],ax
        xor     ax,dx
        sub     ax,dx
        or      si,ax
        mov     bp,si
        mov     si,word ptr [di-0Ah]
        mov     ax,si
        add     ax,word ptr ds:[1C8h]
        js      short L18F7
        or      bp,si
        mov     ax,bp
        mov     bx,1E2h
        or      ah,ah
        je      short L188C
        xchg    al,ah
        xlatb
        jmp     short L188F
L188C:
        xlatb
        add     al,8
L188F:
        mov     cl,al
        mov     bx,word ptr ds:[1C8h]
        mov     bp,word ptr ds:[1CCh]
        mov     ax,word ptr ds:[1CAh]
        call    _sphere_check
        jb      short L18F7
        mov     word ptr ds:[1D2h],ax
        call    _get_dist
        les     di,dword ptr ds:[2860h]
        mov     dx,di
        xchg    bx,ax
        stosw
        xchg    bp,ax
        stosw
        mov     ax,2
        stosw
        mov     ax,word ptr ds:[1C0h]
        stosw
        mov     ax,word ptr ds:[1C8h]
        stosw
        mov     ax,word ptr ds:[1CAh]
        stosw
        mov     ax,word ptr ds:[1CCh]
        stosw
        mov     ax,word ptr ds:[1D2h]
        stosw
        xchg    si,ax
        stosw
        mov     si,1D6h
        mov     cx,6
        rep movsw
        mov     word ptr ds:[286Ch],0
        mov     word ptr ds:[2860h],di
        mov     di,word ptr ds:[2868h]
        xchg    dx,ax
        stosw
        mov     word ptr ds:[2868h],di
        mov     ax,ds
        mov     es,ax
        ret
L18EC:
        mov     word ptr ds:[2882h],0FFFFh
        mov     ax,ds
        mov     es,ax
        ret
L18F7:
        mov     ax,ds
        mov     es,ax
        ret
        push    ds
        push    si
        push    di
        push    cx
        push    word ptr ds:[OBJ_Z]
        push    word ptr ds:[OBJ_Y]
        push    word ptr ds:[OBJ_X]
        mov     si,di
        add     si,6
        lodsw
        mov     bp,ax
        lodsw
        mov     word ptr es:[OBJ_X],ax
        lodsw
        mov     word ptr es:[OBJ_Y],ax
        lodsw
        mov     word ptr es:[OBJ_Z],ax
        lodsw
        mov     word ptr es:[289Ah],ax
        lodsw
        mov     di,1D6h
        mov     cx,6
        rep movsw
        mov     bx,es
        mov     ds,bx
        mov     word ptr ds:[2882h],0
        mov     word ptr ds:[1C4h],2
        call    _seg004_5040
        push    word ptr ds:[MAT_ZZ]
        push    word ptr ds:[MAT_ZY]
        push    word ptr ds:[MAT_ZX]
        push    word ptr ds:[MAT_YZ]
        push    word ptr ds:[MAT_YY]
        push    word ptr ds:[MAT_YX]
        push    word ptr ds:[MAT_XZ]
        push    word ptr ds:[MAT_XY]
        push    word ptr ds:[MAT_XX]
        mov     word ptr ds:[1C6h],bp
        mov     ax,word ptr [bp]
        mov     word ptr ds:[1C0h],ax
        add     bp,2
        lea     ax,[bp+2Eh]
        mov     word ptr ds:[1C2h],ax
        test    word ptr [bp-4],0FFFFh
        jne     short L1990
        mov     si,sp
        push    word ptr [bp]
        add     bp,0Eh
        call    _seg004_5834
        pop     si
        jmp     short L19B5
L1990:
        mov     si,word ptr [bp]
        mov     bx,word ptr [bp+24h]
        or      bx,bx
        je      short L199F
        push    bp
        call    _instance_head
        pop     bp
L199F:
        mov     bx,word ptr [bp+20h]
        or      bx,bx
        je      short L19AB
        push    bp
        call    _instance_pitch
        pop     bp
L19AB:
        mov     bx,word ptr [bp+22h]
        or      bx,bx
        je      short L19B5
        call    _instance_bank
L19B5:
        mov     ax,word ptr ds:[OBJ_X]
        cwd
        xor     ax,dx
        sub     ax,dx
        mov     bp,word ptr [si-6]
        add     bp,ax
        jno     short L19C7
        jmp     L1A52
L19C7:
        mov     ax,word ptr ds:[OBJ_Y]
        cwd
        xor     ax,dx
        sub     ax,dx
        add     ax,word ptr [si-4]
        jo      short L1A52
        or      bp,ax
        mov     ax,word ptr ds:[OBJ_Z]
        cwd
        xor     ax,dx
        sub     ax,dx
        add     ax,word ptr [si-2]
        jo      short L1A52
        or      ax,bp
        xchg    bx,ax
        mov     ax,bx
        mov     bx,1E2h
        or      ah,ah
        je      short L19F4
        xchg    al,ah
        xlatb
        jmp     short L19F7
L19F4:
        xlatb
        add     al,8
L19F7:
        mov     byte ptr ds:[OBJ_SHIFT],al
        push    si
        call    _check_flat
        call    _self_modify
        mov     si,word ptr ds:[1C0h]
        or      si,si
        je      short L1A13
        mov     bp,word ptr ds:[1C2h]
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
L1A13:
        pop     si
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     word ptr ds:[MAT_XX]
        pop     word ptr ds:[MAT_XY]
        pop     word ptr ds:[MAT_XZ]
        pop     word ptr ds:[MAT_YX]
        pop     word ptr ds:[MAT_YY]
        pop     word ptr ds:[MAT_YZ]
        pop     word ptr ds:[MAT_ZX]
        pop     word ptr ds:[MAT_ZY]
        pop     word ptr ds:[MAT_ZZ]
        pop     word ptr ds:[OBJ_X]
        pop     word ptr ds:[OBJ_Y]
        pop     word ptr ds:[OBJ_Z]
        call    _self_modify
        pop     cx
        pop     di
        pop     si
        pop     ds
        ret
L1A52:
        mov     word ptr ds:[2882h],0FFFFh
        pop     word ptr ds:[MAT_XX]
        pop     word ptr ds:[MAT_XY]
        pop     word ptr ds:[MAT_XZ]
        pop     word ptr ds:[MAT_YX]
        pop     word ptr ds:[MAT_YY]
        pop     word ptr ds:[MAT_YZ]
        pop     word ptr ds:[MAT_ZX]
        pop     word ptr ds:[MAT_ZY]
        pop     word ptr ds:[MAT_ZZ]
        pop     word ptr ds:[OBJ_X]
        pop     word ptr ds:[OBJ_Y]
        pop     word ptr ds:[OBJ_Z]
        pop     cx
        pop     di
        pop     si
        pop     ds
        ret
_seg004_17AB endp

; seg004_1A8D  (+1A8D)
; UW1 only: opcode 6Ah, an object header like do_obj; not traced further.
_seg004_1A8D proc near
        mov     word ptr ds:[2882h],0
        lodsw
        add     ax,si
        mov     di,ax
        mov     ax,word ptr ds:[EYE_X]
        sub     ax,word ptr [si]
        mov     word ptr ds:[1D6h],ax
        mov     bx,word ptr ds:[EYE_X+2]
        sbb     bx,word ptr [si+2]
        mov     word ptr ds:[1D8h],bx
        mov     cx,word ptr [di-8]
        jcxz    L1AC4
        or      cx,cx
        jns     short L1ABE
        neg     cx
L1AB6:
        shl     ax,1
        rcl     bx,1
        loop    L1AB6
        jmp     short L1AC4
L1ABE:
        sar     bx,1
        rcr     ax,1
        loop    L1ABE
L1AC4:
        cwd
        cmp     bx,dx
        je      short L1ACC
        jmp     L1BCD
L1ACC:
        mov     word ptr ds:[OBJ_X],ax
        xor     ax,dx
        sub     ax,dx
        mov     bp,word ptr [di-6]
        add     bp,ax
        jno     short L1ADD
        jmp     L1BCD
L1ADD:
        mov     ax,word ptr ds:[EYE_Y]
        sub     ax,word ptr [si+4]
        mov     word ptr ds:[1DAh],ax
        mov     bx,word ptr ds:[EYE_Y+2]
        sbb     bx,word ptr [si+6]
        mov     word ptr ds:[1DCh],bx
        mov     cx,word ptr [di-8]
        jcxz    L1B0A
        or      cx,cx
        jns     short L1B04
        neg     cx
L1AFC:
        shl     ax,1
        rcl     bx,1
        loop    L1AFC
        jmp     short L1B0A
L1B04:
        sar     bx,1
        rcr     ax,1
        loop    L1B04
L1B0A:
        cwd
        cmp     bx,dx
        je      short L1B12
        jmp     L1BCD
L1B12:
        mov     word ptr ds:[OBJ_Y],ax
        xor     ax,dx
        sub     ax,dx
        add     ax,word ptr [di-4]
        jno     short L1B21
        jmp     L1BCD
L1B21:
        or      bp,ax
        mov     ax,word ptr ds:[EYE_Z]
        sub     ax,word ptr [si+8]
        mov     word ptr ds:[1DEh],ax
        mov     bx,word ptr ds:[EYE_Z+2]
        sbb     bx,word ptr [si+0Ah]
        mov     word ptr ds:[1E0h],bx
        mov     cx,word ptr [di-8]
        jcxz    L1B50
        or      cx,cx
        jns     short L1B4A
        neg     cx
L1B42:
        shl     ax,1
        rcl     bx,1
        loop    L1B42
        jmp     short L1B50
L1B4A:
        sar     bx,1
        rcr     ax,1
        loop    L1B4A
L1B50:
        cwd
        cmp     bx,dx
        jne     short L1BCD
        mov     word ptr ds:[OBJ_Z],ax
        xor     bx,ax
        sub     bx,dx
        add     bx,word ptr [di-2]
        jo      short L1BCD
        or      bx,bp
        add     si,0Ch
        push    si
        mov     si,word ptr [di-0Ah]
        mov     ax,si
        add     ax,word ptr ds:[OBJ_X]
        js      short L1BDD
        mov     word ptr ds:[1C4h],1
        push    bx
        mov     ax,bx
        mov     bx,1E2h
        or      ah,ah
        je      short L1B87
        xchg    al,ah
        xlatb
        jmp     short L1B8A
L1B87:
        xlatb
        add     al,8
L1B8A:
        pop     bx
        mov     byte ptr ds:[OBJ_SHIFT],al
        or      bx,si
        mov     ax,bx
        mov     bx,1E2h
        or      ah,ah
        je      short L1B9E
        xchg    al,ah
        xlatb
        jmp     short L1BA1
L1B9E:
        xlatb
        add     al,8
L1BA1:
        mov     cl,al
        push    di
        mov     bx,word ptr ds:[OBJ_X]
        mov     bp,word ptr ds:[OBJ_Z]
        mov     ax,word ptr ds:[OBJ_Y]
        call    _sphere_check
        jb      short L1BDC
        mov     word ptr ds:[289Ah],ax
        call    _self_modify
        mov     ax,si
        call    _seg004_5040
        pop     si
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L1BCD:
        mov     word ptr ds:[2882h],0FFFFh
        add     si,0Ch
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L1BDC:
        pop     si
L1BDD:
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_1A8D endp

; seg004_1BE4  (+1BE4)
; Opcode 38h, the object header. Operands: skip (the offset from after the operand to the
; end of the object's program), shift, then three (extent, 32-bit coordinate) pairs for x,
; y and z, then the radius, then the object's program. Clears 2882h; computes each
; coordinate relative to the eye (26BAh..26C4h) and shifts it by `shift` (right when
; positive, left when negative); if any relative coordinate does not fit in 16 bits the
; object is skipped with 2882h = -1 (L1D2B). Stores the negated values as the object origin
; (2886h..288Ah). The extents are added to the coordinates' magnitudes and ORed together,
; and the bit-length table at ds:22h turns that into 2880h, 15 minus its bit length: the
; left shift the point loaders apply, so a small or near object keeps the most precision.
; A first cull at modify_axis2 (FM name; the add or sub and the axis address are patched by
; seg004_51A3 to the axis the view looks most along) skips the object if its radius
; plus that coordinate is negative, behind the eye. Then sphere_check; if it passes, AX
; goes to 289Ah, self_modify loads the origin into the loaders, set_accept gets sphere_check's
; SI, and the program after the radius is run. Every rejection dispatches the opcode after
; the object (SI = the skip target).
do_obj proc near
        mov     word ptr ds:[2882h],0
        lodsw
        mov     di,ax
        add     di,si
        lodsw
        mov     word ptr ds:[2708h],ax
        mov     cx,ax
        lodsw
        mov     bp,ax
        lodsw
        sub     ax,word ptr ds:[EYE_X]
        mov     word ptr ds:[1D6h],ax
        xchg    bx,ax
        lodsw
        xchg    bx,ax
        sbb     bx,word ptr ds:[EYE_X+2]
        mov     word ptr ds:[1D8h],bx
        jcxz    L1C21
        or      cx,cx
        jns     short L1C1B
        neg     cx
L1C13:
        shl     ax,1
        rcl     bx,1
        loop    L1C13
        jmp     short L1C21
L1C1B:
        sar     bx,1
        rcr     ax,1
        loop    L1C1B
L1C21:
        cwd
        cmp     bx,dx
        je      short L1C29
        jmp     L1D2B
L1C29:
        neg     ax
        mov     word ptr ds:[OBJ_X],ax
        cwd
        xor     ax,dx
        sub     ax,dx
        add     bp,ax
        jno     short L1C3A
        jmp     L1D2B
L1C3A:
        lodsw
        mov     dx,ax
        lodsw
        sub     ax,word ptr ds:[EYE_Y]
        mov     word ptr ds:[1DAh],ax
        xchg    bx,ax
        lodsw
        xchg    bx,ax
        sbb     bx,word ptr ds:[EYE_Y+2]
        mov     word ptr ds:[1DCh],bx
        mov     cx,word ptr ds:[2708h]
        jcxz    L1C6A
        or      cx,cx
        jns     short L1C64
        neg     cx
L1C5C:
        shl     ax,1
        rcl     bx,1
        loop    L1C5C
        jmp     short L1C6A
L1C64:
        sar     bx,1
        rcr     ax,1
        loop    L1C64
L1C6A:
        mov     cx,dx
        cwd
        cmp     bx,dx
        je      short L1C74
        jmp     L1D2B
L1C74:
        neg     ax
        mov     word ptr ds:[OBJ_Y],ax
        cwd
        xor     ax,dx
        sub     ax,dx
        add     ax,cx
        jno     short L1C85
        jmp     L1D2B
L1C85:
        or      bp,ax
        lodsw
        mov     dx,ax
        lodsw
        sub     ax,word ptr ds:[EYE_Z]
        mov     word ptr ds:[1DEh],ax
        xchg    bx,ax
        lodsw
        xchg    bx,ax
        sbb     bx,word ptr ds:[EYE_Z+2]
        mov     word ptr ds:[1E0h],bx
        mov     cx,word ptr ds:[2708h]
        jcxz    L1CB7
        or      cx,cx
        jns     short L1CB1
        neg     cx
L1CA9:
        shl     ax,1
        rcl     bx,1
        loop    L1CA9
        jmp     short L1CB7
L1CB1:
        sar     bx,1
        rcr     ax,1
        loop    L1CB1
L1CB7:
        mov     cx,dx
        cwd
        cmp     bx,dx
        jne     short L1D2B
        neg     ax
        mov     word ptr ds:[OBJ_Z],ax
        cwd
        xor     ax,dx
        sub     ax,dx
        add     ax,cx
        jo      short L1D2B
        or      bp,ax
        lodsw
        push    si
        push    di
        xchg    si,ax
        mov     ax,si
modify_axis2:
        add     ax,word ptr ds:[OBJ_X]
        js      short L1D39
        mov     word ptr ds:[1C4h],0
        mov     ax,bp
        mov     bx,1E2h
        or      ah,ah
        je      short L1CEE
        xchg    al,ah
        xlatb
        jmp     short L1CF1
L1CEE:
        xlatb
        add     al,8
L1CF1:
        mov     byte ptr ds:[OBJ_SHIFT],al
        or      bp,si
        mov     ax,bp
        mov     bx,1E2h
        or      ah,ah
        je      short L1D04
        xchg    al,ah
        xlatb
        jmp     short L1D07
L1D04:
        xlatb
        add     al,8
L1D07:
        mov     cl,al
        mov     bx,word ptr ds:[OBJ_X]
        mov     bp,word ptr ds:[OBJ_Z]
        mov     ax,word ptr ds:[OBJ_Y]
        call    _sphere_check
        pop     di
        jb      short L1D3A
        mov     word ptr ds:[289Ah],ax
        call    _self_modify
        xchg    si,ax
        call    _seg004_5040
        pop     si
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L1D2B:
        mov     word ptr ds:[2882h],0FFFFh
        mov     si,di
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L1D39:
        pop     di
L1D3A:
        pop     si
        mov     si,di
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
do_obj endp

; seg004_1D43  (+1D43)
; UW1 only: opcode 20h, a point given in 32-bit world coordinates: taken relative to the eye
; (26BAh..), shifted to fit 16 bits (by the bit-length table at 1E2h), transformed (mxmul) and
; stored with its clip code (code_pnt); when a coordinate does not fit, all three are scaled
; down first (from 1DDBh).
_seg004_1D43 proc near
        mov     di,si
        lodsw
        sub     ax,word ptr ds:[EYE_X]
        xchg    bx,ax
        lodsw
        xchg    bx,ax
        sbb     bx,word ptr ds:[EYE_X+2]
        cwd
        cmp     bx,dx
        je      short L1D59
        jmp     L1DDB
L1D59:
        mov     word ptr ds:[OBJ_X],ax
        xor     ax,dx
        sub     ax,dx
        mov     bp,ax
        lodsw
        sub     ax,word ptr ds:[EYE_Y]
        xchg    bx,ax
        lodsw
        xchg    bx,ax
        sbb     bx,word ptr ds:[EYE_Y+2]
        cwd
        cmp     bx,dx
        jne     short L1DDB
        mov     word ptr ds:[OBJ_Y],ax
        xor     ax,dx
        sub     ax,dx
        or      bp,ax
        lodsw
        sub     ax,word ptr ds:[EYE_Z]
        xchg    bx,ax
        lodsw
        xchg    bx,ax
        sbb     bx,word ptr ds:[EYE_Z+2]
        cwd
        cmp     bx,dx
        jne     short L1DDB
        mov     word ptr ds:[OBJ_Z],ax
        xor     ax,dx
        sub     ax,dx
        or      bp,ax
        mov     ax,bp
        mov     bx,1E2h
        or      ah,ah
        je      short L1DA4
        xchg    al,ah
        xlatb
        jmp     short L1DA7
L1DA4:
        xlatb
        add     al,8
L1DA7:
        mov     cl,al
        mov     bx,word ptr ds:[OBJ_X]
        shl     bx,cl
        mov     bp,word ptr ds:[OBJ_Z]
        shl     bp,cl
        mov     ax,word ptr ds:[OBJ_Y]
        shl     ax,cl
        mov     cx,ax
L1DBC:
        call    _mxmul
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
L1DDB:
        mov     si,di
        lodsw
        sub     ax,word ptr ds:[EYE_X]
        lodsw
        sbb     ax,word ptr ds:[EYE_X+2]
        cwd
        xor     ax,dx
        mov     bp,ax
        lodsw
        sub     ax,word ptr ds:[EYE_Y]
        lodsw
        sbb     ax,word ptr ds:[EYE_Y+2]
        cwd
        xor     ax,dx
        or      bp,ax
        lodsw
        sub     ax,word ptr ds:[EYE_Z]
        lodsw
        sbb     ax,word ptr ds:[EYE_Z+2]
        cwd
        xor     ax,dx
        or      bp,ax
        mov     ax,bp
        mov     bx,1E2h
        or      ah,ah
        je      short L1E18
        xchg    al,ah
        xlatb
        jmp     short L1E1B
L1E18:
        xlatb
        add     al,8
L1E1B:
        sub     al,10h
        neg     al
        xor     ah,ah
        mov     bp,ax
        mov     si,di
        lodsw
        sub     ax,word ptr ds:[EYE_X]
        xchg    bx,ax
        lodsw
        sbb     ax,word ptr ds:[EYE_X+2]
        mov     cx,bp
L1E32:
        sar     ax,1
        rcr     bx,1
        loop    L1E32
        mov     word ptr ds:[OBJ_X],bx
        lodsw
        sub     ax,word ptr ds:[EYE_Y]
        xchg    bx,ax
        lodsw
        sbb     ax,word ptr ds:[EYE_Y+2]
        mov     cx,bp
L1E49:
        sar     ax,1
        rcr     bx,1
        loop    L1E49
        mov     word ptr ds:[OBJ_Y],bx
        lodsw
        sub     ax,word ptr ds:[EYE_Z]
        xchg    bx,ax
        lodsw
        sbb     ax,word ptr ds:[EYE_Z+2]
        mov     cx,bp
L1E60:
        sar     ax,1
        rcr     bx,1
        loop    L1E60
        mov     word ptr ds:[OBJ_Z],bx
        mov     bx,word ptr ds:[OBJ_X]
        mov     cx,word ptr ds:[OBJ_Y]
        mov     bp,word ptr ds:[OBJ_Z]
        jmp     L1DBC
_seg004_1D43 endp

; seg004_1E79  (+1E79)
; UW1 only: opcode 30h, as opcode 28h (seg004_1E84) with its program address read through a
; pointer.
_seg004_1E79 proc near
        lodsw
        mov     word ptr ds:[1D4h],si
        mov     bx,ax
        mov     si,word ptr [bx]
        jmp     short L1E8B
_seg004_1E79 endp

; seg004_1E84  (+1E84)
; UW1 only: opcode 28h: runs a sub-program at a scaled offset from the current origin (saved
; at 1C8h..1CCh and restored after), with a bounding test like do_obj's; not traced further.
_seg004_1E84 proc near
        lodsw
        mov     word ptr ds:[1D4h],si
        mov     si,ax
L1E8B:
        mov     ax,word ptr ds:[OBJ_X]
        mov     word ptr ds:[1C8h],ax
        mov     ax,word ptr ds:[OBJ_Y]
        mov     word ptr ds:[1CAh],ax
        mov     ax,word ptr ds:[OBJ_Z]
        mov     word ptr ds:[1CCh],ax
        mov     bp,word ptr [si-8]
        mov     ax,word ptr ds:[1D6h]
        mov     bx,word ptr ds:[1D8h]
        mov     cx,bp
        jcxz    L1EBF
        or      cx,cx
        jns     short L1EB9
        neg     cx
L1EB1:
        shl     ax,1
        rcl     bx,1
        loop    L1EB1
        jmp     short L1EBF
L1EB9:
        sar     ax,1
        rcr     bx,1
        loop    L1EB9
L1EBF:
        cwd
        cmp     bx,dx
        je      short L1EC7
        jmp     L1FAA
L1EC7:
        mov     word ptr ds:[OBJ_X],ax
        mov     ax,word ptr ds:[1DAh]
        mov     bx,word ptr ds:[1DCh]
        mov     cx,bp
        jcxz    L1EE9
        or      cx,cx
        jns     short L1EE3
        neg     cx
L1EDB:
        shl     ax,1
        rcl     bx,1
        loop    L1EDB
        jmp     short L1EE9
L1EE3:
        sar     ax,1
        rcr     bx,1
        loop    L1EE3
L1EE9:
        cwd
        cmp     bx,dx
        je      short L1EF1
        jmp     L1FAA
L1EF1:
        mov     word ptr ds:[OBJ_Y],ax
        mov     ax,word ptr ds:[1DEh]
        mov     bx,word ptr ds:[1E0h]
        mov     cx,bp
        jcxz    L1F13
        or      cx,cx
        jns     short L1F0D
        neg     cx
L1F05:
        shl     ax,1
        rcl     bx,1
        loop    L1F05
        jmp     short L1F13
L1F0D:
        sar     ax,1
        rcr     bx,1
        loop    L1F0D
L1F13:
        cwd
        cmp     bx,dx
        je      short L1F1B
        jmp     L1FAA
L1F1B:
        mov     word ptr ds:[OBJ_Z],ax
        cmp     word ptr ds:[1C4h],2
        jne     short L1F5F
        push    si
        mov     bp,word ptr ds:[1C6h]
        add     bp,2
        test    word ptr [bp-4],0FFFFh
        jne     short L1F3C
        add     bp,0Eh
        call    _seg004_5821
        jmp     short L1F5E
L1F3C:
        mov     bx,word ptr [bp+24h]
        or      bx,bx
        je      short L1F48
        push    bp
        call    _instance_vvars_head
        pop     bp
L1F48:
        mov     bx,word ptr [bp+20h]
        or      bx,bx
        je      short L1F54
        push    bp
        call    _instance_vvars_pitch
        pop     bp
L1F54:
        mov     bx,word ptr [bp+22h]
        or      bx,bx
        je      short L1F5E
        call    _instance_vvars_bank
L1F5E:
        pop     si
L1F5F:
        mov     ax,word ptr ds:[OBJ_X]
        cwd
        xor     ax,dx
        sub     ax,dx
        mov     bp,word ptr [si-6]
        add     bp,ax
        jo      short L1FAA
        mov     ax,word ptr ds:[OBJ_Y]
        cwd
        xor     ax,dx
        sub     ax,dx
        add     ax,word ptr [si-4]
        jo      short L1FAA
        or      bp,ax
        mov     ax,word ptr ds:[OBJ_Z]
        cwd
        xor     ax,dx
        sub     ax,dx
        add     ax,word ptr [si-2]
        jo      short L1FAA
        or      ax,bp
        xchg    bx,ax
        mov     ax,bx
        mov     bx,1E2h
        or      ah,ah
        je      short L1F9B
        xchg    al,ah
        xlatb
        jmp     short L1F9E
L1F9B:
        xlatb
        add     al,8
L1F9E:
        mov     byte ptr ds:[OBJ_SHIFT],al
        call    _self_modify
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
L1FAA:
        mov     ax,word ptr ds:[1C8h]
        mov     word ptr ds:[OBJ_X],ax
        mov     ax,word ptr ds:[1CAh]
        mov     word ptr ds:[OBJ_Y],ax
        mov     ax,word ptr ds:[1CCh]
        mov     word ptr ds:[OBJ_Z],ax
        mov     si,word ptr ds:[1D4h]
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_1E84 endp

SEG004_TEXT ends

        end
