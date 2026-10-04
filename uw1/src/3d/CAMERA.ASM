; /* target: seg004_A10 */
; /* opts: /ml */
;
; The camera: the view matrix and eye position of each frame (create_matrix and its camera
; routines), the angle-to-matrix routines, and far entries that select and steer the camera.
;
; What it does in the game: render_3d (INSTANCE.ASM) calls create_matrix once a frame. The
; current view description (seg051:104h) starts with a camera type, an even word that selects
; one of six camera routines through the table at seg051:108h: head_view (type 0, the
; player's eyes), seg004_E02 (2), seg004_CF4 (4), seg004_E87 (6, does nothing), seg004_D2D (8)
; and seg004_CAB (0Ah). Each sets the eye position (26BAh..26C4h, three 32-bit coordinates),
; the zoom (2726h) and the view matrix (1602h..1612h).
;
; UW1 against UW2: this is the first part of UW2's INTERP.ASM (the far routines at DB0,
; create_matrix, head_view, angles_2_matrix and their helpers), assembled in UW1 as a module
; of its own (see below). UW2 has only head_view; its other camera slots point at an `int 2`.
; UW1 also has the other five camera routines, the aiming routines they share (seg004_D70,
; DBA, E88, EFE, 103A), a transition in create_matrix that blends one view into the next, and
; the far entries seg004_11DD .. 1245 with their command handlers (tables at seg051:114h ..
; 150h, a word per camera type). Nothing in the EXE calls any of the far entries (no far call
; or far pointer to them, in the resident code or the overlays).
;
; name: descriptive. The routine names are FM Towns' (through UW2) where UW2 has the routine.
;
; .186, where UW2 has .386: UW1's seg004 has no 386 instruction and none of the nop padding
; that TASM's .386 mode puts after forward conditional jumps.
;
; SEG004 split into its original modules: this is module 3 of 9, +A10..+1370 of the segment
; (file AA80h), a module of the second library, as in UW2.
; match: evidence for the bounds: starts on a paragraph after 5 zero bytes following SMOOTH's
; iret, and ends with 10 zero bytes before SPHERE.ASM. UW2 has this code and INTERP's opcode
; handlers in one module; UW1's handlers start a paragraph-aligned module of their own at
; +27D0 (after 5 zero bytes), so the two were separate source files here.
; Its relocations are entries 2606-2618 of UW.EXE's relocation table.
;
; The comments on create_matrix, head_view, seg004_C81, angles_2_matrix and seg004_1153 are
; UW2's, with UW1's addresses.

; The model interpreter's opcode table: a word per opcode, the offset of its handler
; (INTERP.ASM), in seg051, the data segment while interpreting (UW2: 24F4h).
OPCODE_TABLE    equ     2738h

        .186

        extrn   seg051:far
        extrn   _SquareRoot_seg019_A30:far
        extrn   _sincos:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public 'code'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _overflow_handler_reg:near
        extrn   _mm9x9:near
        extrn   _mm3x9t:near
        public  _angles_2_matrix
        public  _create_matrix
        public  _head_view
        public  _seg004_103A
        public  _seg004_1055
        public  _seg004_1153
        public  _seg004_11DD
        public  _seg004_120A
        public  _seg004_122C
        public  _seg004_1231
        public  _seg004_1236
        public  _seg004_123B
        public  _seg004_1240
        public  _seg004_1245
        public  _seg004_124A
        public  _seg004_125F
        public  _seg004_1274
        public  _seg004_1289
        public  _seg004_129E
        public  _seg004_12A8
        public  _seg004_12B2
        public  _seg004_12BC
        public  _seg004_12C6
        public  _seg004_12E2
        public  _seg004_12FE
        public  _seg004_1332
        public  _seg004_A10
        public  _seg004_A1E
        public  _seg004_A32
        public  _seg004_C81
        public  _seg004_CAB
        public  _seg004_CF4
        public  _seg004_D2D
        public  _seg004_D70
        public  _seg004_DBA
        public  _seg004_E02
        public  _seg004_E87
        public  _seg004_E88
        public  _seg004_EFE

; seg004_A10  (+A10): three far routines nothing in the EXE calls. All three work on
; seg051 words 104h (the current view description), 106h (the last one create_matrix used)
; and 1B0h. A10 saves 104h in 1B0h; A1E restores it and clears 106h.
_seg004_A10 proc far
        push    ds
        mov     cx,seg seg051
        mov     ds,cx
        mov     ax,word ptr ds:[104h]
        mov     word ptr ds:[1B0h],ax
        pop     ds
        ret
_seg004_A10 endp

; seg004_A1E  (+A1E)
_seg004_A1E proc far
        push    ds
        mov     cx,seg seg051
        mov     ds,cx
        mov     ax,word ptr ds:[1B0h]
        mov     word ptr ds:[104h],ax
        mov     word ptr ds:[106h],0
        pop     ds
        ret
_seg004_A1E endp

; seg004_A32  (+A32)
; In: AX = a view description (stored at 104h), BX nonzero to clear 106h. Clears 19Ah and
; sets ss:[4B0h] (seg063) to -1, or to 0 when the description's words 0, 2 and 3 are
; 0, 974h and 0. Nothing in the EXE calls it; what the record and the flag mean is not known.
_seg004_A32 proc far
        push    ds
        mov     cx,seg seg051
        mov     ds,cx
        mov     word ptr ds:[104h],ax
        or      bx,bx
        je      short L0A45
        mov     word ptr ds:[106h],0
L0A45:
        mov     word ptr ds:[19Ah],0
        mov     word ptr ss:[4B0h],0FFFFh
        mov     di,ax
        cmp     word ptr [di],0
        jne     short L0A6F
        mov     ax,word ptr [di+4]
        cmp     ax,974h
        jne     short L0A6F
        test    word ptr [di+6],0FFFFh
        jne     short L0A6F
        mov     word ptr ss:[4B0h],0
L0A6F:
        pop     ds
        ret
_seg004_A32 endp

; seg004_A71  (+A71)
; Builds the frame's view matrix. Calls the camera routine its first word selects through the table at
; 108h, records 104h in 106h, and keeps a copy of the eye position
; (26BAh, 12 bytes) and of the matrix's depth column and 2726h at 19Ch. Called by render_3d.
; Sets 15Ch to -1 first; head_view may set it to a model address.
; UW1 only (no int 2 check on 104h, and this transition): when 106h is set and differs from
; 104h, 19Ah becomes 8000h; while 19Ah is nonzero it falls by ss:[9B0h] / 4 a frame, and the
; eye position, the zoom (2726h) and the view direction (1606h, 160Ch, 1612h) are blended
; from the copies saved at 19Ch.. towards the new ones by 19Ah / 8000h (the factors at
; 1A8h..1AEh), and the matrix is rebuilt from that direction (seg004_EFE).
_create_matrix proc near
        mov     ax,0FFFFh
        mov     word ptr ds:[15Ch],ax
        mov     si,word ptr ds:[104h]
L0A7B:
        lodsw
        mov     bx,ax
        mov     ax,word ptr [bx+108h]
        call    ax
        mov     ax,word ptr ds:[106h]
        or      ax,ax
        jne     short L0A8E
        jmp     L0BD4
L0A8E:
        cmp     ax,word ptr ds:[104h]
        je      short L0A9A
        mov     ax,8000h
        mov     word ptr ds:[19Ah],ax
L0A9A:
        mov     bx,word ptr ds:[19Ah]
        or      bx,bx
        jne     short L0AA5
        jmp     L0BD4
L0AA5:
        mov     dx,word ptr ss:[9B0h]
        xor     ax,ax
        sar     dx,1
        sar     dx,1
        mov     cx,bx
        sub     cx,dx
        jns     short L0ABC
        mov     word ptr ds:[19Ah],ax
        jmp     L0BD4
L0ABC:
        mov     word ptr ds:[19Ah],cx
        sar     dx,1
        div     bx
        mov     bx,7FFFh
        sub     bx,ax
        mov     ax,word ptr ds:[1AEh]
        mul     bx
        shl     ax,1
        rcl     dx,1
        mov     si,dx
        mov     bp,ax
        mov     ax,word ptr ds:[2726h]
        sub     bx,7FFFh
        neg     bx
        mul     bx
        shl     ax,1
        rcl     dx,1
        sub     bx,7FFFh
        neg     bx
        add     ax,bx
        adc     dx,si
        mov     word ptr ds:[2726h],dx
        mov     ax,word ptr ds:[26BAh]
        sub     ax,word ptr ds:[19Ch]
        imul    bx
        shl     ax,1
        rcl     dx,1
        sub     word ptr ds:[26BAh],dx
        mov     ax,dx
        cwd
        sbb     word ptr ds:[26BCh],dx
        mov     ax,word ptr ds:[26BEh]
        sub     ax,word ptr ds:[1A0h]
        imul    bx
        shl     ax,1
        rcl     dx,1
        sub     word ptr ds:[26BEh],dx
        mov     ax,dx
        cwd
        sbb     word ptr ds:[26C0h],dx
        mov     ax,word ptr ds:[26C2h]
        sub     ax,word ptr ds:[1A4h]
        imul    bx
        shl     ax,1
        rcl     dx,1
        sub     word ptr ds:[26C2h],dx
        mov     ax,dx
        cwd
        sbb     word ptr ds:[26C4h],dx
        mov     ax,word ptr ds:[1A8h]
        imul    bx
        shl     ax,1
        rcl     dx,1
        mov     si,dx
        mov     bp,ax
        mov     ax,word ptr ds:[1606h]
        sub     bx,7FFFh
        neg     bx
        imul    bx
        shl     ax,1
        rcl     dx,1
        sub     bx,7FFFh
        neg     bx
        add     ax,bx
        adc     dx,si
        mov     word ptr ds:[1606h],dx
        mov     ax,word ptr ds:[1AAh]
        imul    bx
        shl     ax,1
        rcl     dx,1
        mov     si,dx
        mov     bp,ax
        mov     ax,word ptr ds:[160Ch]
        sub     bx,7FFFh
        neg     bx
        imul    bx
        shl     ax,1
        rcl     dx,1
        sub     bx,7FFFh
        neg     bx
        add     ax,bx
        adc     dx,si
        mov     word ptr ds:[160Ch],dx
        mov     ax,word ptr ds:[1ACh]
        imul    bx
        shl     ax,1
        rcl     dx,1
        mov     si,dx
        mov     bp,ax
        mov     ax,word ptr ds:[1612h]
        sub     bx,7FFFh
        neg     bx
        imul    bx
        shl     ax,1
        rcl     dx,1
        sub     bx,7FFFh
        neg     bx
        add     ax,bx
        adc     dx,si
        mov     word ptr ds:[1612h],dx
        mov     ax,word ptr ds:[1606h]
        mov     word ptr ds:[15Eh],ax
        mov     ax,word ptr ds:[160Ch]
        mov     word ptr ds:[162h],ax
        mov     ax,word ptr ds:[1612h]
        mov     word ptr ds:[166h],ax
        call    _seg004_EFE
        mov     ax,0FFFFh
        mov     word ptr ds:[15Ch],ax
L0BD4:
        mov     ax,word ptr ds:[104h]
        mov     word ptr ds:[106h],ax
        mov     si,26BAh
        mov     di,19Ch
        mov     cx,6
        rep movsw
        mov     ax,word ptr ds:[1606h]
        stosw
        mov     ax,word ptr ds:[160Ch]
        stosw
        mov     ax,word ptr ds:[1612h]
        stosw
        mov     ax,word ptr ds:[2726h]
        stosw
L0BF5:
        ret
_create_matrix endp

; seg004_BF6  (+BF6)
; The camera routine (FM head_view). In: SI = the view record after its selector word, ES =
; DS = seg051. Reads the zoom word (2726h) and the address BP of the eye record (and
; sets 15Ch when the third word is 0 and ss:[9B2h] is clear). On first use (the record's
; word +6 zero, then set to 1) it builds a two-angle matrix in the record
; (seg004_1153) and, when its word +1Eh is nonzero, seg004_C81's offset. It builds
; the eye's matrix with angles_2_matrix when the eye record's first word is set, rotates the
; offset (mm3x9t) and adds the eye record's 32-bit coordinates to give the eye position at
; 26BAh..26C4h, and composes the final matrix at 14B2h (mm9x9). What each record field means
; beyond that has not been traced.
_head_view proc near
        lodsw
        mov     word ptr ds:[2726h],ax
        lodsw
        mov     bp,ax
        lodsw
        or      ax,ax
        jne     short L0C13
        test    word ptr ss:[9B2h],0FFFFh
        jne     short L0C13
        mov     ax,bp
        sub     ax,4
        mov     word ptr ds:[15Ch],ax
L0C13:
        test    word ptr [si+6],0FFFFh
        jne     short L0C38
        mov     word ptr [si+6],1
        push    si
        push    bp
        add     si,8
        mov     di,si
        add     di,4
        call    _seg004_1153
        pop     bp
        pop     si
        mov     di,word ptr [si+1Eh]
        or      di,di
        je      short L0C38
        call    _seg004_C81
L0C38:
        add     bp,12h
        test    word ptr [bp-12h],0FFFFh
        je      short L0C47
        push    si
        call    _angles_2_matrix
        pop     si
L0C47:
        call    _mm3x9t
        mov     ax,bx
        cwd
        add     ax,word ptr [bp-0Ch]
        adc     dx,word ptr [bp-0Ah]
        mov     word ptr ds:[26BAh],ax
        mov     word ptr ds:[26BCh],dx
        mov     ax,cx
        cwd
        add     ax,word ptr [bp-8]
        adc     dx,word ptr [bp-6]
        mov     word ptr ds:[26BEh],ax
        mov     word ptr ds:[26C0h],dx
        mov     ax,di
        cwd
        add     ax,word ptr [bp-4]
        adc     dx,word ptr [bp-2]
        mov     word ptr ds:[26C2h],ax
        mov     word ptr ds:[26C4h],dx
        add     si,0Ch
        call    _mm9x9
        ret
_head_view endp

; seg004_C81  (+C81)
; In: DI = a distance, SI = a matrix record. Writes -(DI * column) for the matrix column at
; SI+10h, +16h, +1Ch into SI+0..+4, a vector DI units back along that axis; probably the
; camera's offset behind the eye (inferred from head_view's use when the record's +1Eh word
; is set; not checked in play).
_seg004_C81 proc near
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
_seg004_C81 endp

; seg004_CAB  (+CAB)
; Camera type 0Ah. In: SI = the view description after its type word: BP the record looked
; at, a count, then the positions. The eye is the position nearest to BP (seg004_D70), copied
; to 26BAh. The words after that position: 0 continues as camera type 4 (seg004_CF4, from its
; zoom word); otherwise the next word is the zoom (2726h), or when it is 0 the zoom is the
; word after it divided by the distance (seg019's square root of seg004_D70's squared
; distance). Then it looks at BP (seg004_E88).
_seg004_CAB proc near
        lodsw
        mov     bp,ax
        lodsw
        mov     cx,ax
        call    _seg004_D70
        mov     di,26BAh
        mov     cx,6
        rep movsw
        lodsw
        or      ax,ax
        je      short L0CFC
        lodsw
        or      ax,ax
        jne     short L0CEE
        mov     bx,word ptr ds:[182h]
        mov     cx,word ptr ds:[184h]
L0CCE:
        call    far ptr _SquareRoot_seg019_A30
        lodsw
        mov     dx,ax
        xor     ax,ax
        cmp     di,dx
        jle     short L0CE4
        sar     dx,1
        rcr     ax,1
        idiv    di
        jmp     short L0CEE
L0CE4:
        xchg    dx,di
        sar     dx,1
        rcr     ax,1
        idiv    di
        neg     ax
L0CEE:
        mov     word ptr ds:[2726h],ax
        jmp     _seg004_E88
_seg004_CAB endp

; seg004_CF4  (+CF4)
; Camera type 4: a fixed eye. Copies the eye position (12 bytes at SI) to 26BAh and the zoom
; word to 2726h. When the next word is nonzero the nine matrix words that follow are the view
; matrix (copied to 1602h); otherwise the view looks along the vector that follows
; (seg004_EFE), the flag word is set to -1 and the matrix is stored after it, so that later
; frames copy it.
_seg004_CF4 proc near
        mov     di,26BAh
        mov     cx,6
        rep movsw
L0CFC:
        lodsw
        mov     word ptr ds:[2726h],ax
        lodsw
        or      ax,ax
        je      short L0D0E
        mov     di,1602h
        mov     cx,9
        rep movsw
        ret
L0D0E:
        push    si
        lodsw
        mov     word ptr ds:[15Eh],ax
        lodsw
        mov     word ptr ds:[162h],ax
        lodsw
        mov     word ptr ds:[166h],ax
        call    _seg004_EFE
        pop     di
        mov     word ptr [di-2],0FFFFh
        mov     si,1602h
        mov     cx,9
        rep movsw
        ret
_seg004_CF4 endp

; seg004_D2D  (+D2D)
; Camera type 8. In: SI = the description after its type word: BP the record looked at, the
; offset of the eye record in SS (seg063), the zoom. The eye position is copied from the eye
; record (15Ch is set to it - 0Ah), the zoom is the word, or when it is 0 worked out from the
; distance to BP as seg004_CAB does (7ED2h when seg004_DBA finds it out of range), and it
; looks at BP (seg004_E88).
_seg004_D2D proc near
        lodsw
        mov     bp,ax
        lodsw
        push    si
        mov     si,ax
        sub     ax,0Ah
        mov     word ptr ds:[15Ch],ax
        mov     di,26BAh
        mov     ax,ss
        mov     ds,ax
        mov     cx,6
        rep movsw
        mov     ax,es
        mov     ds,ax
        pop     si
        lodsw
        or      ax,ax
        jne     short L0D6A
        push    si
        mov     si,26BAh
        call    _seg004_DBA
        pop     si
        jb      short L0D61
        mov     cx,bx
        mov     bx,di
        jmp     L0CCE
L0D61:
        mov     word ptr ds:[2726h],7ED2h
        jmp     _seg004_E88
L0D6A:
        mov     word ptr ds:[2726h],ax
        jmp     _seg004_E88
_seg004_D2D endp

; seg004_D70  (+D70)
; Finds, among CX positions from SI (12 bytes each, and records 12h or 24h bytes long: 24h when
; the word at +0Ch is zero), the one nearest to the point at BP. Out: SI = that position, 180h
; the same, 182h/184h its squared distance (from seg004_DBA; 7FFFFFFFh when none is in range).
_seg004_D70 proc near
        mov     word ptr ds:[186h],cx
        mov     ax,0FFFFh
        mov     word ptr ds:[182h],ax
        mov     ax,7FFFh
        mov     word ptr ds:[184h],ax
        mov     word ptr ds:[180h],si
L0D84:
        call    _seg004_DBA
        jb      short L0DA2
        mov     ax,word ptr ds:[182h]
        mov     dx,word ptr ds:[184h]
        sub     ax,di
        sbb     dx,bx
        js      short L0DA2
        mov     word ptr ds:[182h],di
        mov     word ptr ds:[184h],bx
        mov     word ptr ds:[180h],si
L0DA2:
        test    word ptr [si+0Ch],0FFFFh
        jne     short L0DAC
        add     si,12h
L0DAC:
        add     si,12h
        dec     word ptr ds:[186h]
        jne     L0D84
        mov     si,word ptr ds:[180h]
        ret
_seg004_D70 endp

; seg004_DBA  (+DBA)
; The squared distance between the points at SI and BP (three 32-bit coordinates each).
; Out: CF clear and BX:DI = the sum of the squares when every difference fits in 16 bits,
; else CF set.
_seg004_DBA proc near
        mov     ax,word ptr [si]
        sub     ax,word ptr [bp]
        mov     cx,word ptr [si+2]
        sbb     cx,word ptr [bp+2]
        cwd
        cmp     dx,cx
        jne     short L0E00
        imul    ax
        mov     di,ax
        mov     bx,dx
        mov     ax,word ptr [si+4]
        sub     ax,word ptr [bp+4]
        mov     cx,word ptr [si+6]
        sbb     cx,word ptr [bp+6]
        cwd
        cmp     dx,cx
        jne     short L0E00
        imul    ax
        add     di,ax
        adc     bx,dx
        mov     ax,word ptr [si+8]
        sub     ax,word ptr [bp+8]
        mov     cx,word ptr [si+0Ah]
        sbb     cx,word ptr [bp+0Ah]
        cwd
        cmp     dx,cx
        jne     short L0E00
        imul    ax
        add     di,ax
        adc     bx,dx
        clc
        ret
L0E00:
        stc
        ret
_seg004_DBA endp

; seg004_E02  (+E02)
; Camera type 2. In: SI = the description after its type word: BP the record looked at, the
; offset of the eye record in SS. Takes the eye position from the eye record and looks at BP
; (seg004_E88), then moves the eye back by 5DCh along the view's x and z and up or down by
; 64h by the sign of 160Ch. Then it makes the description at 0C8h current (104h; its type
; word is 4) and writes after the type word the eye, the word 1, the view matrix and 7ED2h,
; and sets the zoom to 7ED2h. seg004_CF4 reads a type 4 description as eye, zoom, flag and
; matrix, which is one word off this layout; not checked in play.
_seg004_E02 proc near
        lodsw
        mov     bp,ax
        lodsw
        mov     si,ax
        mov     ax,ss
        mov     ds,ax
        mov     di,26BAh
        mov     cx,6
        rep movsw
        mov     ax,es
        mov     ds,ax
        call    _seg004_E88
        mov     ax,5DCh
        imul    word ptr ds:[1606h]
        add     ax,ax
        adc     dx,dx
        mov     ax,dx
        cwd
        sub     word ptr ds:[26BAh],ax
        sbb     word ptr ds:[26BCh],dx
        mov     ax,5DCh
        imul    word ptr ds:[1612h]
        add     ax,ax
        adc     dx,dx
        mov     ax,dx
        cwd
        sub     word ptr ds:[26C2h],ax
        sbb     word ptr ds:[26C4h],dx
        mov     ax,word ptr ds:[160Ch]
        cwd
        mov     ax,64h
        xor     ax,dx
        sub     ax,dx
        add     ax,word ptr ds:[26BEh]
        adc     dx,word ptr ds:[26C0h]
        mov     word ptr ds:[26BEh],ax
        mov     word ptr ds:[26C0h],dx
        mov     di,0C8h
        mov     word ptr ds:[104h],di
        add     di,2
        mov     si,26BAh
        mov     cx,6
        rep movsw
        mov     ax,1
        stosw
        mov     si,1602h
        mov     cx,9
        rep movsw
        mov     ax,7ED2h
        stosw
        mov     word ptr ds:[2726h],ax
        ret
_seg004_E02 endp

; seg004_E87  (+E87)
; Camera type 6: does nothing.
_seg004_E87 proc near
        ret
_seg004_E87 endp

; seg004_E88  (+E88)
; Aims the view at the point BP points at (three 32-bit coordinates): the difference from the
; eye (26BAh..) is halved until each part fits in 16 bits, stored at 15Eh, 162h, 166h, and
; the matrix is built from it (seg004_EFE).
_seg004_E88 proc near
        mov     ax,word ptr [bp]
        sub     ax,word ptr ds:[26BAh]
        mov     word ptr ds:[15Eh],ax
        mov     ax,word ptr [bp+2]
        sbb     ax,word ptr ds:[26BCh]
        mov     word ptr ds:[160h],ax
        mov     bx,word ptr [bp+4]
        sub     bx,word ptr ds:[26BEh]
        mov     word ptr ds:[162h],bx
        mov     ax,word ptr [bp+6]
        sbb     ax,word ptr ds:[26C0h]
        mov     word ptr ds:[164h],ax
        mov     cx,word ptr [bp+8]
        sub     cx,word ptr ds:[26C2h]
        mov     word ptr ds:[166h],cx
        mov     ax,word ptr [bp+0Ah]
        sbb     ax,word ptr ds:[26C4h]
        mov     word ptr ds:[168h],ax
L0EC6:
        mov     ax,word ptr ds:[15Eh]
        cwd
        cmp     dx,word ptr ds:[160h]
        jne     short L0EE4
        mov     ax,word ptr ds:[162h]
        cwd
        cmp     dx,word ptr ds:[164h]
        jne     short L0EE4
        mov     ax,word ptr ds:[166h]
        cwd
        cmp     dx,word ptr ds:[168h]
        je      short _seg004_EFE
L0EE4:
        sar     word ptr ds:[160h],1
        rcr     word ptr ds:[15Eh],1
        sar     word ptr ds:[164h],1
        rcr     word ptr ds:[162h],1
        sar     word ptr ds:[168h],1
        rcr     word ptr ds:[166h],1
        jmp     L0EC6
_seg004_E88 endp

; seg004_EFE  (+EFE)
; Builds the view matrix (1602h..1612h, 1.15) that looks along the vector at 15Eh, 162h, 166h
; (x, y, z): the third row is the vector divided by its length (seg004_103A), the first
; the horizontal unit vector at right angles to it (from x and z; or from y alone when x and z
; are 0), the second their cross product. 8000h is changed to 8001h throughout. Installs
; overflow_handler_reg for the divides.
_seg004_EFE proc near
        mov     word ptr ss:[4D5h],offset _overflow_handler_reg
        mov     ax,word ptr ds:[15Eh]
        mov     bx,word ptr ds:[162h]
        mov     cx,word ptr ds:[166h]
        call    _seg004_103A
        mov     dx,word ptr ds:[15Eh]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L0F27
        mov     ax,8001h
L0F27:
        mov     word ptr ds:[1606h],ax
        mov     dx,word ptr ds:[162h]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L0F3E
        mov     ax,8001h
L0F3E:
        mov     word ptr ds:[160Ch],ax
        mov     dx,word ptr ds:[166h]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L0F55
        mov     ax,8001h
L0F55:
        mov     word ptr ds:[1612h],ax
        mov     ax,word ptr ds:[15Eh]
        or      ax,word ptr ds:[166h]
        je      short L0FAD
        mov     ax,word ptr ds:[15Eh]
        imul    ax
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[166h]
        imul    ax
        add     bx,ax
        adc     cx,dx
        call    far ptr _SquareRoot_seg019_A30
        mov     dx,word ptr ds:[166h]
        xor     ax,ax
        mov     word ptr ds:[1608h],ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L0F8F
        mov     ax,8001h
L0F8F:
        mov     word ptr ds:[1602h],ax
        mov     dx,word ptr ds:[15Eh]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L0FA6
        mov     ax,8001h
L0FA6:
        neg     ax
        mov     word ptr ds:[160Eh],ax
        jmp     short L0FBD
L0FAD:
        mov     ax,word ptr ds:[160Ch]
        neg     ax
        mov     word ptr ds:[1602h],ax
        xor     ax,ax
        mov     word ptr ds:[1608h],ax
        mov     word ptr ds:[160Eh],ax
L0FBD:
        mov     ax,word ptr ds:[1608h]
        imul    word ptr ds:[1612h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[160Eh]
        imul    word ptr ds:[160Ch]
        sub     ax,bx
        sbb     dx,cx
        shl     ax,1
        rcl     dx,1
        cmp     dx,8000h
        jne     short L0FE0
        mov     dx,8001h
L0FE0:
        mov     word ptr ds:[1604h],dx
        mov     ax,word ptr ds:[1606h]
        imul    word ptr ds:[160Eh]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[1602h]
        imul    word ptr ds:[1612h]
        sub     ax,bx
        sbb     dx,cx
        shl     ax,1
        rcl     dx,1
        jno     short L1005
        mov     dx,7FFFh
        jmp     short L100E
L1005:
        cmp     dx,8000h
        jne     short L100E
        mov     dx,8001h
L100E:
        mov     word ptr ds:[160Ah],dx
        mov     ax,word ptr ds:[1602h]
        imul    word ptr ds:[160Ch]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[1608h]
        imul    word ptr ds:[1606h]
        sub     ax,bx
        sbb     dx,cx
        shl     ax,1
        rcl     dx,1
        cmp     dx,8000h
        jne     short L1035
        mov     dx,8001h
L1035:
        mov     word ptr ds:[1610h],dx
        ret
_seg004_EFE endp

; seg004_103A  (+103A)
; In: AX, BX, CX = a vector. Out: the length, from seg019's square root of the sum of the
; squares.
_seg004_103A proc near
        imul    ax
        mov     bp,dx
        xchg    bx,ax
        imul    ax
        add     bx,ax
        adc     bp,dx
        mov     ax,cx
        imul    ax
        add     bx,ax
        adc     dx,bp
        mov     cx,dx
        call    far ptr _SquareRoot_seg019_A30
        ret
_seg004_103A endp

; seg004_1055  (+1055)
; Far wrapper for angles_2_matrix; nothing in the EXE calls it.
_seg004_1055 proc far
        call    _angles_2_matrix
        retf
_seg004_1055 endp

; seg004_1059  (+1059)
; In: BP (and DI) = a record whose words +12h, +14h and +16h are three angles, ES = the 3D
; data segment. Calls seg019's sincos for each negated angle (with DS = SS, as sincos needs),
; keeping the sines and cosines at es:18Ch..196h, and writes the 3x3 rotation matrix they make
; to BP+0..+10h in 1.15 fixed point. Which angle is heading, pitch and bank has not been
; checked.
_angles_2_matrix proc near
        mov     ax,ss
        mov     ds,ax
        push    bp
        mov     di,bp
        mov     bx,word ptr [di+12h]
        neg     bx
        call    far ptr _sincos
        mov     word ptr es:[194h],bx
        mov     word ptr es:[196h],ax
        mov     bx,word ptr [di+14h]
        neg     bx
        call    far ptr _sincos
        mov     word ptr es:[190h],bx
        mov     word ptr es:[192h],ax
        mov     bx,word ptr [di+16h]
        neg     bx
        call    far ptr _sincos
        mov     word ptr es:[18Ch],bx
        mov     word ptr es:[18Eh],ax
        pop     bp
        mov     ax,es
        mov     ds,ax
        mov     ax,word ptr ds:[196h]
        neg     ax
        mov     word ptr [bp+0Ah],ax
        mov     ax,word ptr ds:[190h]
        imul    word ptr ds:[18Ch]
        shl     ax,1
        rcl     dx,1
        mov     bx,dx
        mov     ax,word ptr ds:[192h]
        imul    word ptr ds:[18Eh]
        shl     ax,1
        rcl     dx,1
        mov     di,dx
        xchg    dx,ax
        imul    word ptr ds:[196h]
        shl     ax,1
        rcl     dx,1
        add     dx,bx
        mov     word ptr [bp],dx
        mov     ax,word ptr ds:[192h]
        imul    word ptr ds:[194h]
        shl     ax,1
        rcl     dx,1
        neg     dx
        mov     word ptr [bp+6],dx
        mov     ax,word ptr ds:[190h]
        imul    word ptr ds:[18Eh]
        shl     ax,1
        rcl     dx,1
        mov     cx,dx
        mov     ax,word ptr ds:[192h]
        imul    word ptr ds:[18Ch]
        shl     ax,1
        rcl     dx,1
        mov     si,dx
        xchg    dx,ax
        imul    word ptr ds:[196h]
        shl     ax,1
        rcl     dx,1
        sub     dx,cx
        neg     dx
        mov     word ptr [bp+0Ch],dx
        mov     ax,cx
        imul    word ptr ds:[196h]
        shl     ax,1
        rcl     dx,1
        sub     si,dx
        mov     word ptr [bp+2],si
        mov     ax,word ptr ds:[190h]
        imul    word ptr ds:[194h]
        shl     ax,1
        rcl     dx,1
        mov     word ptr [bp+8],dx
        mov     ax,bx
        imul    word ptr ds:[196h]
        shl     ax,1
        rcl     dx,1
        add     dx,di
        mov     word ptr [bp+0Eh],dx
        mov     ax,word ptr ds:[18Eh]
        imul    word ptr ds:[194h]
        shl     ax,1
        rcl     dx,1
        neg     dx
        mov     word ptr [bp+4],dx
        mov     ax,word ptr ds:[18Ch]
        imul    word ptr ds:[194h]
        shl     ax,1
        rcl     dx,1
        mov     word ptr [bp+10h],dx
        ret
_angles_2_matrix endp

; seg004_1153  (+1153)
; Two-angle version of angles_2_matrix: angles at es:[si] and es:[si+2], matrix written to
; DI+0..+10h with element +6 zero (no roll term). Called by head_view for the eye's own
; rotation.
_seg004_1153 proc near
        mov     ax,ss
        mov     ds,ax
        mov     bx,word ptr es:[si]
        neg     bx
        call    far ptr _sincos
        mov     word ptr es:[194h],bx
        mov     word ptr es:[196h],ax
        mov     bx,word ptr es:[si+2]
        neg     bx
        call    far ptr _sincos
        mov     word ptr es:[18Ch],bx
        mov     word ptr es:[18Eh],ax
        mov     ax,es
        mov     ds,ax
        mov     ax,word ptr ds:[18Ch]
        mov     word ptr [di],ax
        mov     ax,word ptr ds:[18Eh]
        imul    word ptr ds:[196h]
        shl     ax,1
        rcl     dx,1
        neg     dx
        mov     word ptr [di+2],dx
        mov     ax,word ptr ds:[18Eh]
        imul    word ptr ds:[194h]
        shl     ax,1
        rcl     dx,1
        neg     dx
        mov     word ptr [di+4],dx
        mov     word ptr [di+6],0
        mov     ax,word ptr ds:[194h]
        mov     word ptr [di+8],ax
        mov     ax,word ptr ds:[196h]
        neg     ax
        mov     word ptr [di+0Ah],ax
        mov     ax,word ptr ds:[18Eh]
        mov     word ptr [di+0Ch],ax
        mov     ax,word ptr ds:[18Ch]
        imul    word ptr ds:[196h]
        shl     ax,1
        rcl     dx,1
        mov     word ptr [di+0Eh],dx
        mov     ax,word ptr ds:[18Ch]
        imul    word ptr ds:[194h]
        shl     ax,1
        rcl     dx,1
        mov     word ptr [di+10h],dx
        ret
_seg004_1153 endp

; seg004_11DD  (+11DD)
; Far, called by nothing in the EXE. Makes the type 0Ah description at 0ECh current (104h),
; sets its first position's x and z (32-bit) from the record at ss:97Ah (seg063, where
; cPlayer, the camera record, is 0970h) and clears 106h.
_seg004_11DD proc far
        push    ds
        push    es
        mov     ax,seg seg051
        mov     ds,ax
        mov     es,ax
        mov     di,0ECh
        mov     word ptr ds:[104h],di
        add     di,6
        mov     si,97Ah
        mov     ax,ss
        mov     ds,ax
        movsw
        movsw
        add     si,4
        add     di,4
        movsw
        movsw
        xor     ax,ax
        mov     word ptr es:[106h],ax
        pop     es
        pop     ds
        retf
_seg004_11DD endp

; seg004_120A  (+120A)
; Far: a camera command. When the current description is settled (104h = 106h, no
; transition, 19Ah = 0), calls its type's handler from the table at BX (a word per type, the
; type word being the index), with SI = the description.
_seg004_120A proc far
        push    ds
        push    es
        mov     ax,seg seg051
        mov     ds,ax
        mov     es,ax
        mov     si,word ptr ds:[104h]
        cmp     si,word ptr ds:[106h]
        jne     short L1229
        test    word ptr ds:[19Ah],0FFFFh
        jne     short L1229
        add     bx,word ptr [si]
        call    word ptr [bx]
L1229:
        pop     es
        pop     ds
        retf
_seg004_120A endp

; seg004_122C  (+122C)
; Far entries for the six camera commands (handler tables at 114h, 120h, 12Ch, 138h, 144h,
; 150h); nothing in the EXE calls them. Commands 0 to 3 turn head_view's camera, 4 and 5 zoom
; scale the zoom word (types 0, 4 and 0Ah); the other types ignore them (a bare ret, BF5h).
_seg004_122C proc near
        mov     bx,114h
        jmp     _seg004_120A
_seg004_122C endp

; seg004_1231  (+1231)
_seg004_1231 proc near
        mov     bx,120h
        jmp     _seg004_120A
_seg004_1231 endp

; seg004_1236  (+1236)
_seg004_1236 proc near
        mov     bx,12Ch
        jmp     _seg004_120A
_seg004_1236 endp

; seg004_123B  (+123B)
_seg004_123B proc near
        mov     bx,138h
        jmp     _seg004_120A
_seg004_123B endp

; seg004_1240  (+1240)
_seg004_1240 proc near
        mov     bx,144h
        jmp     _seg004_120A
_seg004_1240 endp

; seg004_1245  (+1245)
_seg004_1245 proc near
        mov     bx,150h
        jmp     _seg004_120A
_seg004_1245 endp

; seg004_124A  (+124A)
; Command 0 for type 0: lowers word +12h of the description (head_view's second angle) by
; 7D0h * ss:[9B0h] / 8000h and clears +0Eh, so head_view rebuilds its matrix. 125F raises
; it; 1274 raises +10h (the first angle) and 1289 lowers it.
_seg004_124A proc near
        mov     ax,7D0h
        imul    word ptr ss:[9B0h]
        shl     ax,1
        rcl     dx,1
        sub     word ptr [si+12h],dx
        mov     word ptr [si+0Eh],0
        ret
_seg004_124A endp

; seg004_125F  (+125F)
_seg004_125F proc near
        mov     ax,7D0h
        imul    word ptr ss:[9B0h]
        shl     ax,1
        rcl     dx,1
        add     word ptr [si+12h],dx
        mov     word ptr [si+0Eh],0
        ret
_seg004_125F endp

; seg004_1274  (+1274)
_seg004_1274 proc near
        mov     ax,7D0h
        imul    word ptr ss:[9B0h]
        shl     ax,1
        rcl     dx,1
        add     word ptr [si+10h],dx
        mov     word ptr [si+0Eh],0
        ret
_seg004_1274 endp

; seg004_1289  (+1289)
_seg004_1289 proc near
        mov     ax,7D0h
        imul    word ptr ss:[9B0h]
        shl     ax,1
        rcl     dx,1
        sub     word ptr [si+10h],dx
        mov     word ptr [si+0Eh],0
        ret
_seg004_1289 endp

; seg004_129E  (+129E)
; Commands 4 and 5 for type 0: word +2 (the zoom) scaled by seg004_12FE or seg004_1332.
_seg004_129E proc near
        mov     ax,word ptr [si+2]
        call    _seg004_12FE
        mov     word ptr [si+2],ax
        ret
_seg004_129E endp

; seg004_12A8  (+12A8)
_seg004_12A8 proc near
        mov     ax,word ptr [si+2]
        call    _seg004_1332
        mov     word ptr [si+2],ax
        ret
_seg004_12A8 endp

; seg004_12B2  (+12B2)
; Commands 4 and 5 for type 4: word +0Eh (the zoom) scaled likewise.
_seg004_12B2 proc near
        mov     ax,word ptr [si+0Eh]
        call    _seg004_12FE
        mov     word ptr [si+0Eh],ax
        ret
_seg004_12B2 endp

; seg004_12BC  (+12BC)
_seg004_12BC proc near
        mov     ax,word ptr [si+0Eh]
        call    _seg004_1332
        mov     word ptr [si+0Eh],ax
        ret
_seg004_12BC endp

; seg004_12C6  (+12C6)
; Commands 4 and 5 for type 0Ah: word +0Eh of the chosen position (180h), or +10h when +0Eh
; is 0, scaled likewise.
_seg004_12C6 proc near
        mov     si,word ptr ds:[180h]
        mov     ax,word ptr [si+0Eh]
        or      ax,ax
        je      short L12D8
        call    _seg004_12FE
        mov     word ptr [si+0Eh],ax
        ret
L12D8:
        mov     ax,word ptr [si+10h]
        call    _seg004_12FE
        mov     word ptr [si+10h],ax
        ret
_seg004_12C6 endp

; seg004_12E2  (+12E2)
_seg004_12E2 proc near
        mov     si,word ptr ds:[180h]
        mov     ax,word ptr [si+0Eh]
        or      ax,ax
        je      short L12F4
        call    _seg004_1332
        mov     word ptr [si+0Eh],ax
        ret
L12F4:
        mov     ax,word ptr [si+10h]
        call    _seg004_1332
        mov     word ptr [si+10h],ax
        ret
_seg004_12E2 endp

; seg004_12FE  (+12FE)
; AX scaled by the factor 7FFFh - ss:[9B0h] (1.15): multiplied when AX is positive, divided
; by it when AX is negative. seg004_1332 does the opposite.
_seg004_12FE proc near
        mov     bx,word ptr ss:[9B0h]
        sub     bx,7FFFh
        neg     bx
        or      ax,ax
        js      short L1316
        imul    bx
        shl     ax,1
        rcl     dx,1
        mov     ax,dx
        ret
L1316:
        mov     dx,ax
        add     ax,bx
        js      short L1325
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    bx
        ret
L1325:
        xchg    dx,bx
        neg     bx
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    bx
        ret
_seg004_12FE endp

; seg004_1332  (+1332)
_seg004_1332 proc near
        mov     dx,ax
        xor     ax,ax
        mov     bx,word ptr ss:[9B0h]
        sub     bx,7FFFh
        neg     bx
        or      dx,dx
        js      short L135B
        cmp     dx,bx
        jg      short L1350
        sar     dx,1
        rcr     ax,1
        idiv    bx
        ret
L1350:
        xchg    dx,bx
        sar     dx,1
        rcr     ax,1
        idiv    bx
        neg     ax
        ret
L135B:
        mov     ax,dx
        imul    bx
        shl     ax,1
        rcl     dx,1
        mov     ax,dx
        ret

; seg004_1366  (+1366)
; Opcode for every unused table slot: `int 2`, then carries on with the next opcode. A
; program that reaches it is malformed; int 2 is NMI, so presumably a debugger catch.
_seg004_1332 endp

SEG004_TEXT ends

        end
