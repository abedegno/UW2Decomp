; /* target: seg003_0272_3206 */
; /* opts: /ml */
;
; Graphics library module 6: the clipped and unclipped point, line, box and rectangle
; primitives, the window clipper for polygons, and the video-memory bump allocator. Found by
; aligning with FM Towns, whose 2D library has, in the same order, copy_visible_to_hidden_,
; copy_hidden_to_visible_, pixel, upixel, vline, uvline, box_si, box, ubox, ubox_si, clip_rect,
; rectangle, clear_window, urectangle, hline, uhline, concave_shclip and shclip; GRCORE's C
; wrappers (rectangle, urectangle, box, uhline, uvline, clear_window ...) call the DOS routines
; that line up with those names.
;
; How the primitives draw. None of them writes video memory itself. Each turns its shape into a
; list of spans and jumps through DS:4112, the current span writer (a "ylr" routine in
; VIDMODE.ASM, reached through GRCORE's jump table at 52C9 .. 52DB: solid fill, copy, save and
; restore). A span record is 6 bytes, y, left x, right x; a list ends with 0FFFFh. The u- prefix
; (FM Towns) means unclipped: the caller guarantees the shape is inside the window.
;
; Registers: AX = x, BX = y for a point; a second point or extent in CX, DX. DS = ES = seg_370D.
; Data (seg_370D):
;   3DF4 left, 3DF6 top, 3DF8 right, 3DFA bottom: the clip window. y counts up from the bottom
;        of the screen (VIDMODE.ASM builds the row table so that row 0 is the bottom line), so
;        bottom < top
;   3DEC, 3DEE  the screen's last x and last y
;   4106, 4108, 410A  video memory: the first byte after the visible and hidden pages, the end,
;        and the bump pointer (VIDMODE.ASM sets them at mode set; VALLOC.ASM takes the rest)
;   4112  the span writer; 414A a one-span list; 4152..4158 a box's corners; 415A, 415C a
;        rectangle's x range; 44CE the per-row span table urectangle fills (row y at
;        44CE + (0C7h - y) * 6); 49AC the end mark in it
;   5046  nonzero: a per-row x limit table (5046 + 2 * y) for a concave window
;   415E, 42EE  the two vertex buffers shclip alternates between; 447E..448A its state
;
; name: descriptive (module 6 of seg003); its job, found here, is the basic 2D primitives.
; Routine names in these comments are FM Towns' where the alignment above gives one; the DOS
; labels have none.
;
; SEG003 split into its original modules: this is module 6 of 14, +3206..+3686 of the segment
; (file 6656h), a module of the second library (UWLIB.LIB).
; Procs: seg003_0272_3206, seg003_0272_321B, seg003_0272_326A, seg003_0272_326D,
; seg003_0272_327D, seg003_0272_328F, seg003_0272_32E2, seg003_0272_331A, seg003_0272_3321,
; seg003_0272_3324, seg003_0272_336E, seg003_0272_3371, seg003_0272_33BE, seg003_0272_341E,
; seg003_0272_3423, seg003_0272_342E, seg003_0272_347F, seg003_0272_34AE, seg003_0272_34C2,
; seg003_0272_34C7.
; Evidence for the bounds: starts after one zero byte following a ret: TLINK's padding before
; a word-aligned module (TASM 2.0's `even` pads with 90h).
; Its relocations are entries 2476 of UW2.EXE's relocation table.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg003_0272_368E:near
        extrn   _seg003_0272_375C:near
        extrn   _seg003_0272_5299:near
        extrn   _seg003_0272_52A8:near
        extrn   _seg003_0272_52B1:near
        extrn   _seg003_0272_52B4:near
        extrn   _seg003_0272_52B7:near
        extrn   _seg003_0272_52C9:near
        extrn   _seg003_0272_52CC:near
        extrn   _seg003_0272_52CF:near
        extrn   _seg003_0272_52D2:near
        public  _seg003_0272_3206
        public  _seg003_0272_321B
        public  _seg003_0272_326A
        public  _seg003_0272_326D
        public  _seg003_0272_327D
        public  _seg003_0272_328F
        public  _seg003_0272_32B4
        public  _seg003_0272_32E2
        public  _seg003_0272_331A
        public  _seg003_0272_3321
        public  _seg003_0272_3324
        public  _seg003_0272_336E
        public  _seg003_0272_3371
        public  _seg003_0272_33BE
        public  _seg003_0272_3416
        public  _seg003_0272_341E
        public  _seg003_0272_3423
        public  _seg003_0272_342E
        public  _seg003_0272_347F
        public  _seg003_0272_34AE
        public  _seg003_0272_34C2
        public  _seg003_0272_34C7

; seg003_0272_3206  (+3206)
; Allocate BX bytes of video memory from the bump pointer (410A). Out: carry clear and AX = the
; block's offset, or carry set when it would pass 4108 or wrap. GRCORE's far wrapper _49AE
; returns AX or 0 to C. VALLOC.ASM's pool is what is left after these.
_seg003_0272_3206 proc near
        mov     ax,word ptr ds:[410Ah]
        add     bx,ax
        jb      L3219
        cmp     bx,word ptr ds:[4108h]
        jae     L3219
        mov     word ptr ds:[410Ah],bx
        clc
        ret
L3219:
        stc
        ret
_seg003_0272_3206 endp

; seg003_0272_321B  (+321B)
; Give video memory back: set the bump pointer to AX, unless AX is below the floor 4106 (carry
; set). GRCORE's _49F4 is the C wrapper.
;
; The unlabelled code after the ret (and so after this proc's first return) is four small
; routines that nothing in the sources jumps to by name: a clipped line drawn with span writer
; 52CC, the same with 52D2, an unclipped line with 52CF, and a page-flipped clear (flip, _326A,
; flip back).
_seg003_0272_321B proc near
        cmp     ax,word ptr ds:[4106h]
        jae     L3223
        stc
        ret
L3223:
        mov     word ptr ds:[410Ah],ax
        clc
        ret
        push    word ptr ds:[4112h]
        mov     word ptr ds:[4112h],offset _seg003_0272_52CC
        call    _seg003_0272_368E
        pop     word ptr ds:[4112h]
        ret
        push    word ptr ds:[4112h]
        mov     word ptr ds:[4112h],offset _seg003_0272_52D2
        call    _seg003_0272_368E
        pop     word ptr ds:[4112h]
        ret
        push    word ptr ds:[4112h]
        mov     word ptr ds:[4112h],offset _seg003_0272_52CF
        call    _seg003_0272_375C
        pop     word ptr ds:[4112h]
        ret
        push    ax
        call    _seg003_0272_5299
        pop     ax
        call    _seg003_0272_326A
        call    _seg003_0272_5299
        ret
_seg003_0272_321B endp

; seg003_0272_326A  (+326A)
; _326A: set the colour (AX, through set_the_color's vector 52B7) and fall into _326D.
_seg003_0272_326A proc near
        call    _seg003_0272_52B7
_seg003_0272_326A endp

; seg003_0272_326D  (+326D)
; _326D: fill the whole screen, (0, 0) to (3DEC, 3DEE), with the current span writer, through
; urectangle.
_seg003_0272_326D proc near
        sub     ax,ax
        mov     bx,word ptr ds:[3DEEh]
        mov     cx,word ptr ds:[3DECh]
        sub     dx,dx
        call    _seg003_0272_342E
        ret
_seg003_0272_326D endp

; seg003_0272_327D  (+327D)
; copy_visible_to_hidden: run _326D with span writer 52CC, a whole-screen copy from the visible
; page to the hidden one (GRCORE's copy_visible_to_hidden wrapper).
_seg003_0272_327D proc near
        push    word ptr ds:[4112h]
        mov     word ptr ds:[4112h],offset _seg003_0272_52CC
        call    _seg003_0272_326D
        pop     word ptr ds:[4112h]
        ret
_seg003_0272_327D endp

; seg003_0272_328F  (+328F)
; copy_hidden_to_visible: flip the pages (5299, grSoftPageFlip's vector), copy, flip back, so
; the copy runs the other way.
;
; _32B4, inside this proc, is pixel (FM Towns): plot the point (AX, BX) if it is inside the
; window, as a one-span list through the span writer; with the solid writer (52C9) it goes
; straight to the pixel writer 52A8 (upixel), first checking the concave-window table at 5046
; when there is one. GRMISC's far thunk _0 enters here for the 3D view's points.
_seg003_0272_328F proc near
        call    _seg003_0272_5299
        call    _seg003_0272_327D
        call    _seg003_0272_5299
        ret
L3299:
        retn
L329A:
        cmp     word ptr ds:[5046h],0
        je      L32B1
        shl     bx,1
        cmp     ax,word ptr [bx+5046h]
        jge     L32AF
        cmp     ax,word ptr [bx+5046h]
        jge     L3299
L32AF:
        shr     bx,1
L32B1:
        jmp     _seg003_0272_52A8
_seg003_0272_32B4:
        cmp     bx,word ptr ds:[3DF6h]
        jg      L3299
        cmp     bx,word ptr ds:[3DFAh]
        jl      L3299
        cmp     ax,word ptr ds:[3DF8h]
        jg      L3299
        cmp     ax,word ptr ds:[3DF4h]
        jl      L3299
        cmp     word ptr ds:[4112h],offset _seg003_0272_52C9
        je      L329A
        mov     di,414Ah
        mov     si,di
        xchg    bx,ax
        stosw
        xchg    bx,ax
        stosw
        stosw
        jmp     word ptr ds:[4112h]
_seg003_0272_328F endp

; seg003_0272_32E2  (+32E2)
; _32E2: clip a vertical line (x AX, y from BX to DX) to the window. Returns with BX >= DX
; inside the window, or discards its caller's return address (pop ax; ret) so the caller returns
; at once when the line is wholly outside. The code after it is a clipped vertical line through
; vector 52B1.
_seg003_0272_32E2 proc near
        cmp     ax,word ptr ds:[3DF8h]
        jg      L3311
        cmp     ax,word ptr ds:[3DF4h]
        jl      L3311
        cmp     bx,dx
        jg      L32F4
        xchg    bx,dx
L32F4:
        mov     si,word ptr ds:[3DFAh]
        cmp     bx,si
        jl      L3311
        cmp     dx,si
        jg      L3302
        mov     dx,si
L3302:
        mov     si,word ptr ds:[3DF6h]
        cmp     dx,si
        jg      L3311
        cmp     bx,si
        jl      L3310
        mov     bx,si
L3310:
        ret
L3311:
        pop     ax
        ret
        call    _seg003_0272_32E2
        call    _seg003_0272_52B1
        ret
_seg003_0272_32E2 endp

; seg003_0272_331A  (+331A)
; _331A: a clipped vertical line through the vector at 52B4.
_seg003_0272_331A proc near
        call    _seg003_0272_32E2
        call    _seg003_0272_52B4
        ret
_seg003_0272_331A endp

; seg003_0272_3321  (+3321)
; vline: clip (_32E2) and fall into uvline.
_seg003_0272_3321 proc near
        call    _seg003_0272_32E2
_seg003_0272_3321 endp

; seg003_0272_3324  (+3324)
; uvline: a vertical line at x AX from y BX to DX. The solid and copy writers have direct
; vertical-line routines (52B1, 52B4); otherwise it builds one span per row at 2D42 and runs the
; span writer. The code after it loads AX, BX, CX, DX from four words at SI (ubox_si, probably)
; and falls into box.
_seg003_0272_3324 proc near
        cmp     word ptr ds:[5046h],0
        jne     L3341
        mov     si,word ptr ds:[4112h]
        cmp     si,offset _seg003_0272_52C9
        jne     L3338
        jmp     _seg003_0272_52B1
L3338:
        cmp     si,offset _seg003_0272_52CC
        jne     L3341
        jmp     _seg003_0272_52B4
L3341:
        cmp     bx,dx
        jg      L3347
        xchg    bx,dx
L3347:
        mov     cx,bx
        sub     cx,dx
        inc     cx
        mov     dx,ax
        mov     di,2D42h
L3351:
        mov     ax,bx
        stosw
        mov     ax,dx
        stosw
        stosw
        dec     bx
        loop    L3351
        mov     word ptr [di],0FFFFh
        mov     si,2D42h
        jmp     word ptr ds:[4112h]
        lodsw
        xchg    dx,ax
        lodsw
        xchg    bx,ax
        lodsw
        xchg    cx,ax
        lodsw
        xchg    dx,ax
_seg003_0272_3324 endp

; seg003_0272_336E  (+336E)
; box: clip the rectangle (clip_rect) and fall into ubox.
_seg003_0272_336E proc near
        call    _seg003_0272_33BE
_seg003_0272_336E endp

; seg003_0272_3371  (+3371)
; ubox: an outline rectangle with corners (AX, BX) and (CX, DX): two horizontal lines (uhline)
; and two vertical ones (uvline). The code after it is the same with the corners read from SI.
_seg003_0272_3371 proc near
        mov     di,4152h
        stosw
        xchg    bx,ax
        stosw
        xchg    cx,ax
        stosw
        xchg    dx,ax
        stosw
L337B:
        mov     ax,word ptr ds:[4152h]
        mov     bx,word ptr ds:[4154h]
        mov     cx,word ptr ds:[4156h]
        call    _seg003_0272_34AE
        mov     ax,word ptr ds:[4152h]
        mov     bx,word ptr ds:[4158h]
        mov     cx,word ptr ds:[4156h]
        call    _seg003_0272_34AE
        mov     ax,word ptr ds:[4152h]
        mov     bx,word ptr ds:[4154h]
        mov     dx,word ptr ds:[4158h]
        call    _seg003_0272_3324
        mov     ax,word ptr ds:[4156h]
        mov     bx,word ptr ds:[4154h]
        mov     dx,word ptr ds:[4158h]
        call    _seg003_0272_3324
        ret
        mov     di,4152h
        mov     cx,4
        rep movsw
        jmp     L337B
_seg003_0272_3371 endp

; seg003_0272_33BE  (+33BE)
; clip_rect: order and clip the rectangle (AX, BX)-(CX, DX) to the window. Out: AX <= CX, DX <=
; BX (BX the upper row), inside the window; BP = the edges that were cut (8 left, 2 right, 1
; bottom, 4 top); carry set when nothing was cut. A rectangle wholly outside discards the
; caller's return address. _3416 loads the four words at SI and falls into it.
_seg003_0272_33BE proc near
        sub     bp,bp
        cmp     bx,dx
        jg      L33C6
        xchg    bx,dx
L33C6:
        cmp     ax,cx
        jl      L33CB
        xchg    cx,ax
L33CB:
        mov     si,word ptr ds:[3DF4h]
        cmp     cx,si
        jl      L3414
        cmp     ax,si
        jge     L33DC
        mov     ax,si
        add     bp,8
L33DC:
        mov     si,word ptr ds:[3DF8h]
        cmp     ax,si
        jg      L3414
        cmp     cx,si
        jle     L33ED
        mov     cx,si
        add     bp,2
L33ED:
        mov     si,word ptr ds:[3DFAh]
        cmp     bx,si
        jl      L3414
        cmp     dx,si
        jge     L33FE
        mov     dx,si
        add     bp,1
L33FE:
        mov     si,word ptr ds:[3DF6h]
        cmp     dx,si
        jg      L3414
        cmp     bx,si
        jle     L340F
        mov     bx,si
        add     bp,4
L340F:
        cmp     bp,0
        cmc
        ret
L3414:
        pop     ax
        ret
_seg003_0272_3416:
        lodsw
        xchg    dx,ax
        lodsw
        xchg    bx,ax
        lodsw
        xchg    cx,ax
        lodsw
        xchg    dx,ax
_seg003_0272_33BE endp

; seg003_0272_341E  (+341E)
; rectangle: clip_rect, then urectangle.
_seg003_0272_341E proc near
        call    _seg003_0272_33BE
        jmp     short _seg003_0272_342E
_seg003_0272_341E endp

; seg003_0272_3423  (+3423)
; clear_window: urectangle over the whole clip window.
_seg003_0272_3423 proc near
        mov     si,3DF4h
        lodsw
        xchg    dx,ax
        lodsw
        xchg    bx,ax
        lodsw
        xchg    cx,ax
        lodsw
        xchg    dx,ax
_seg003_0272_3423 endp

; seg003_0272_342E  (+342E)
; urectangle: a filled rectangle (AX, BX)-(CX, DX), corners in either order. Writes one span per
; row into the row table at 44CE (the x range copied into each), moves the end mark, and runs
; the span writer on it.
_seg003_0272_342E proc near
        cmp     bx,dx
        jg      L3434
        xchg    bx,dx
L3434:
        cmp     ax,cx
        jl      L3439
        xchg    cx,ax
L3439:
        mov     word ptr ds:[415Ah],ax
        mov     word ptr ds:[415Ch],cx
        mov     ax,bx
        mov     cx,dx
        neg     cx
        add     cx,ax
        inc     cx
        mov     si,word ptr ds:[49ACh]
        and     word ptr [si],7FFFh
        mov     di,44CEh
        mov     si,bx
        neg     si
        add     si,0C7h
        shl     si,1
        add     di,si
        shl     si,1
        add     di,si
        push    di
        mov     bx,415Ah
L3468:
        add     di,2
        mov     si,bx
        movsw
        movsw
        loop    L3468
        mov     word ptr ds:[49ACh],di
        or      word ptr [di],8000h
        pop     si
        jmp     word ptr ds:[4112h]
L347E:
        retn
_seg003_0272_342E endp

; seg003_0272_347F  (+347F)
; hline: clip a horizontal line (y BX, x from AX to CX) to the window and draw it as one span;
; returns at once when it is outside.
_seg003_0272_347F proc near
        cmp     bx,word ptr ds:[3DF6h]
        jg      L347E
        cmp     bx,word ptr ds:[3DFAh]
        jl      L347E
        cmp     ax,cx
        jl      L3490
        xchg    cx,ax
L3490:
        mov     dx,word ptr ds:[3DF4h]
        cmp     cx,dx
        jl      L347E
        cmp     ax,dx
        jg      L349E
        mov     ax,dx
L349E:
        mov     dx,word ptr ds:[3DF8h]
        cmp     ax,dx
        jg      L347E
        cmp     cx,dx
        jl      L34AC
        mov     cx,dx
L34AC:
        jmp     short L34B3
_seg003_0272_347F endp

; seg003_0272_34AE  (+34AE)
; uhline: one span (y BX, x AX to CX, either order) through the span writer.
_seg003_0272_34AE proc near
        cmp     ax,cx
        jl      L34B3
        xchg    cx,ax
L34B3:
        mov     di,414Ah
        mov     si,di
        xchg    bx,ax
        stosw
        xchg    bx,ax
        stosw
        xchg    cx,ax
        stosw
        jmp     word ptr ds:[4112h]
_seg003_0272_34AE endp

; seg003_0272_34C2  (+34C2)
; concave_shclip (FM Towns, 7 bytes before shclip there as here): shclip with the handler set at
; 449A in place of 4492.
_seg003_0272_34C2 proc near
        mov     si,449Ah
        jmp     short L34CA
_seg003_0272_34C2 endp

; seg003_0272_34C7  (+34C7)
; shclip: clip a polygon to the window, Sutherland-Hodgman, one edge at a time. In: CX = the
; vertex count, the vertices (x, y words) at 415E. A pass is skipped when every vertex is
; already inside on that axis. Each pass (L358F) walks the edges, classifies both ends by
; patching the comparison at L35B0 and L35BE (7Ch jl or 7Fh jg, followed by a far jump to flush
; the prefetch queue), and calls one of four handlers through 448A by the in/out case: keep, add
; the intersection, or drop. The intersection (L3611, L364C) is computed with imul/idiv, halving
; both terms on overflow, and nudged one pixel inward unless it lies on the window edge. Out: CX
; = the new count, the vertices back at 415E; when nothing is left it discards the caller's
; return address.
_seg003_0272_34C7 proc near
        mov     si,4492h
L34CA:
        mov     di,448Ah
        movsw
        movsw
        movsw
        movsw
        mov     si,415Eh
        mov     di,cx
        shl     di,1
        shl     di,1
        add     di,si
        movsw
        movsw
        sub     si,4
        mov     bx,word ptr ds:[3DF6h]
        mov     dx,word ptr ds:[3DFAh]
        mov     si,4160h
        push    cx
L34ED:
        lodsw
        inc     si
        inc     si
        cmp     ax,bx
        jg      L34FA
        cmp     ax,dx
        jl      L34FA
        loop    L34ED
L34FA:
        test    cx,cx
        pop     cx
        je      L3535
        mov     word ptr ds:[4482h],2
        mov     word ptr ds:[4484h],8
        mov     word ptr ds:[4486h],offset L364C
        mov     si,415Eh
        mov     di,42EEh
        mov     al,7Ch
        mov     dx,word ptr ds:[3DF6h]
        call    L358F
        mov     word ptr ds:[4486h],offset L3648
        mov     si,42EEh
        mov     di,415Eh
        mov     al,7Fh
        mov     dx,word ptr ds:[3DFAh]
        call    L358F
L3535:
        mov     bx,word ptr ds:[3DF8h]
        mov     dx,word ptr ds:[3DF4h]
        mov     si,415Eh
        push    cx
L3541:
        lodsw
        inc     si
        inc     si
        cmp     ax,bx
        jg      L354E
        cmp     ax,dx
        jl      L354E
        loop    L3541
L354E:
        test    cx,cx
        pop     cx
        je      L3589
        mov     word ptr ds:[4482h],0
        mov     word ptr ds:[4484h],6
        mov     word ptr ds:[4486h],offset L3611
        mov     si,415Eh
        mov     di,42EEh
        mov     al,7Fh
        mov     dx,word ptr ds:[3DF4h]
        call    L358F
        mov     word ptr ds:[4486h],offset L360D
        mov     si,42EEh
        mov     di,415Eh
        mov     al,7Ch
        mov     dx,word ptr ds:[3DF8h]
        call    L358F
L3589:
        jcxz    L358D
        ret
L358C:
        pop     ax
L358D:
        pop     ax
        ret
L358F:
        jcxz    L358C
        push    di
        mov     byte ptr cs:L35B0,al
        mov     byte ptr cs:L35BE,al
        mov     word ptr ds:[4480h],cx
        mov     word ptr ds:[447Eh],cx
        db      0EAh                     ; far jmp to the next instruction, flushing the prefetch queue after the cs: stores above (match: as bytes, since TASM would shorten it)
        dw      offset L35A7
        dw      seg L35A7
L35A7:
        sub     bx,bx
        add     si,word ptr ds:[4482h]
        lodsw
        cmp     ax,dx
L35B0:                                  ; its opcode is patched above
        jg      L35B5
        mov     bx,4
L35B5:
        inc     si
        inc     si
        lodsw
        sub     si,word ptr ds:[4484h]
        cmp     ax,dx
L35BE:
        jg      L35C3
        or      bx,2
L35C3:
        call    word ptr [bx+448Ah]
        dec     word ptr ds:[4480h]
        jg      L35A7
        mov     cx,word ptr ds:[447Eh]
        pop     si
        movsw
        movsw
        ret
        movsw
        movsw
        sub     si,4
        inc     word ptr ds:[447Eh]
        mov     word ptr ds:[4488h],0
        jmp     word ptr ds:[4486h]
        movsw
        movsw
        sub     si,4
        inc     word ptr ds:[447Eh]
        mov     word ptr ds:[4488h],0
        jmp     word ptr ds:[4486h]
        mov     word ptr ds:[4488h],1
        jmp     word ptr ds:[4486h]
        dec     word ptr ds:[447Eh]
        add     si,4
        ret
L360D:
        neg     word ptr ds:[4488h]
L3611:
        push    dx
        mov     ax,dx
        stosw
        lodsw
        xchg    cx,ax
        lodsw
        xchg    bx,ax
        lodsw
        xchg    bp,ax
        lodsw
        sub     si,4
        sub     ax,bx
        sub     dx,cx
        neg     cx
        add     cx,bp
        jo      L3642
L3629:
        imul    dx
        idiv    cx
        add     ax,bx
        cmp     ax,word ptr ds:[3DF6h]
        jge     L363F
        cmp     ax,word ptr ds:[3DFAh]
        jle     L363F
        add     ax,word ptr ds:[4488h]
L363F:
        stosw
        pop     dx
        ret
L3642:
        rcr     cx,1
        sar     dx,1
        jmp     L3629
L3648:
        neg     word ptr ds:[4488h]
L364C:
        push    dx
        lodsw
        xchg    cx,ax
        lodsw
        xchg    bx,ax
        lodsw
        xchg    bp,ax
        lodsw
        sub     si,4
        xchg    bp,ax
        xchg    bx,cx
        sub     ax,bx
        sub     dx,cx
        neg     cx
        add     cx,bp
        jo      L367F
L3664:
        imul    dx
        idiv    cx
        add     ax,bx
        cmp     ax,word ptr ds:[3DF4h]
        jle     L367A
        cmp     ax,word ptr ds:[3DF8h]
        jge     L367A
        add     ax,word ptr ds:[4488h]
L367A:
        stosw
        pop     ax
        stosw
        xchg    dx,ax
        ret
L367F:
        rcr     cx,1
        sar     dx,1
        jmp     L3664
_seg003_0272_34C7 endp

SEG003_TEXT ends

        end
