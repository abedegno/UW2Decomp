; /* target: seg003_3A86 */
; /* opts: /ml */
;
; Graphics library module 9: the clipped and unclipped point, line, box and rectangle
; primitives, the window clipper for polygons, and the video-memory bump allocator. Found by
; aligning with FM Towns, whose 2D library has, in the same order, copy_visible_to_hidden_,
; copy_hidden_to_visible_, pixel, upixel, vline, uvline, box_si, box, ubox, ubox_si, clip_rect,
; rectangle, clear_window, urectangle, hline, uhline, concave_shclip and shclip; GRCORE's C
; wrappers (rectangle, urectangle, box, uhline, uvline, clear_window ...) call the DOS routines
; that line up with those names.
;
; How the primitives draw. None of them writes video memory itself. Each turns its shape into a
; list of spans and jumps through DS:4110, the current span writer (a "ylr" routine in
; VIDMODE.ASM, reached through GRCORE's jump table at 5AC3 .. 5AD5: solid fill, copy, save and
; restore). A span record is 6 bytes, y, left x, right x; a list ends with 0FFFFh. The u- prefix
; (FM Towns) means unclipped: the caller guarantees the shape is inside the window.
;
; Registers: AX = x, BX = y for a point; a second point or extent in CX, DX. DS = ES = the library's
; data segment (3963:0000, UW2's seg048). Data:
;   3DF2 left, 3DF4 top, 3DF6 right, 3DF8 bottom: the clip window. y counts up from the bottom
;        of the screen (VIDMODE.ASM builds the row table so that row 0 is the bottom line), so
;        bottom < top
;   3DEA, 3DEC  the screen's last x and last y
;   4104, 4106, 4108  video memory: the first byte after the visible and hidden pages, the end,
;        and the bump pointer (VIDMODE.ASM sets them at mode set; VALLOC.ASM takes the rest)
;   4110  the span writer; 4148 a one-span list; 4150..4156 a box's corners; 4158, 415A a
;        rectangle's x range; 44CC the per-row span table urectangle fills (row y at
;        44CC + (0C7h - y) * 6); 49AA the end mark in it
;   5044  nonzero: a per-row x limit table (5044 + 2 * y) for a concave window
;   415C, 42EC  the two vertex buffers shclip alternates between; 447C..4488 its state
;
; name: descriptive (UW2Decomp: module 6 of seg003); its job, found here, is the basic 2D
; primitives.
; Routine names in these comments are FM Towns' where the alignment above gives one; the DOS
; labels have none.
;
; UW1 against UW2: UW2Decomp's src/gfx/GRLIBF.ASM (UW2's seg003_0272_3206), the same code and
; length; only the library's data sits 2 bytes lower in UW1 (3DF2 for UW2's 3DF4, and so on
; through 5044), and the far targets move with their modules.
;
; SEG003 split into its original modules: this is module 9 of UW1's 18, +3A86..+3F05.
; Evidence for the bounds: starts after one zero byte following a ret (TLINK's padding before a
; word-aligned module), as in UW2; it ends at the zero byte at +3F05.
; Its relocation is entry 2766 of UW.EXE's relocation table (+3E25).

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg003_3F0E:near
        extrn   _seg003_3FDC:near
        extrn   _seg003_5A93:near
        extrn   _seg003_5AA2:near
        extrn   _seg003_5AAB:near
        extrn   _seg003_5AAE:near
        extrn   _seg003_5AB1:near
        extrn   _seg003_5AC3:near
        extrn   _seg003_5AC6:near
        extrn   _seg003_5AC9:near
        extrn   _seg003_5ACC:near
        public  _seg003_3A86
        public  _seg003_3A9B
        public  _seg003_3AEA
        public  _seg003_3AED
        public  _seg003_3AFD
        public  _seg003_3B0F
        public  _seg003_3B34
        public  _seg003_3B62
        public  _seg003_3B9A
        public  _seg003_3BA1
        public  _seg003_3BA4
        public  _seg003_3BEE
        public  _seg003_3BF1
        public  _seg003_3C3E
        public  _seg003_3C96
        public  _seg003_3C9E
        public  _seg003_3CA3
        public  _seg003_3CAE
        public  _seg003_3CFF
        public  _seg003_3D2E
        public  _seg003_3D42
        public  _seg003_3D47

; seg003_3A86  (+3A86)
; Allocate BX bytes of video memory from the bump pointer (4108). Out: carry clear and AX = the
; block's offset, or carry set when it would pass 4106 or wrap. GRCORE's far wrapper _522E
; returns AX or 0 to C. VALLOC.ASM's pool is what is left after these.
_seg003_3A86 proc near
        mov     ax,word ptr ds:[4108h]
        add     bx,ax
        jb      L3A99
        cmp     bx,word ptr ds:[4106h]
        jae     L3A99
        mov     word ptr ds:[4108h],bx
        clc
        ret
L3A99:
        stc
        ret
_seg003_3A86 endp

; seg003_3A9B  (+3A9B)
; Give video memory back: set the bump pointer to AX, unless AX is below the floor 4104 (carry
; set). GRCORE's _5274 is the C wrapper.
;
; The unlabelled code after the ret (and so after this proc's first return) is four small
; routines that nothing in the sources jumps to by name: a clipped line drawn with span writer
; 5AC6, the same with 5ACC, an unclipped line with 5AC9, and a page-flipped clear (flip, _3AEA,
; flip back).
_seg003_3A9B proc near
        cmp     ax,word ptr ds:[4104h]
        jae     L3AA3
        stc
        ret
L3AA3:
        mov     word ptr ds:[4108h],ax
        clc
        ret
        push    word ptr ds:[4110h]
        mov     word ptr ds:[4110h],offset _seg003_5AC6
        call    _seg003_3F0E
        pop     word ptr ds:[4110h]
        ret
        push    word ptr ds:[4110h]
        mov     word ptr ds:[4110h],offset _seg003_5ACC
        call    _seg003_3F0E
        pop     word ptr ds:[4110h]
        ret
        push    word ptr ds:[4110h]
        mov     word ptr ds:[4110h],offset _seg003_5AC9
        call    _seg003_3FDC
        pop     word ptr ds:[4110h]
        ret
        push    ax
        call    _seg003_5A93
        pop     ax
        call    _seg003_3AEA
        call    _seg003_5A93
        ret
_seg003_3A9B endp

; seg003_3AEA  (+3AEA)
; _3AEA: set the colour (AX, through set_the_color's vector 5AB1) and fall into _3AED.
_seg003_3AEA proc near
        call    _seg003_5AB1
_seg003_3AEA endp

; seg003_3AED  (+3AED)
; _3AED: fill the whole screen, (0, 0) to (3DEA, 3DEC), with the current span writer, through
; urectangle.
_seg003_3AED proc near
        sub     ax,ax
        mov     bx,word ptr ds:[3DECh]
        mov     cx,word ptr ds:[3DEAh]
        sub     dx,dx
        call    _seg003_3CAE
        ret
_seg003_3AED endp

; seg003_3AFD  (+3AFD)
; copy_visible_to_hidden: run _3AED with span writer 5AC6, a whole-screen copy from the visible
; page to the hidden one (GRCORE's copy_visible_to_hidden wrapper).
_seg003_3AFD proc near
        push    word ptr ds:[4110h]
        mov     word ptr ds:[4110h],offset _seg003_5AC6
        call    _seg003_3AED
        pop     word ptr ds:[4110h]
        ret
_seg003_3AFD endp

; seg003_3B0F  (+3B0F)
; copy_hidden_to_visible: flip the pages (5A93, grSoftPageFlip's vector), copy, flip back, so
; the copy runs the other way.
;
; _3B34, inside this proc, is pixel (FM Towns): plot the point (AX, BX) if it is inside the
; window, as a one-span list through the span writer; with the solid writer (5AC3) it goes
; straight to the pixel writer 5AA2 (upixel), first checking the concave-window table at 5044
; when there is one. GRMISC's far thunk _0 enters here for the 3D view's points.
_seg003_3B0F proc near
        call    _seg003_5A93
        call    _seg003_3AFD
        call    _seg003_5A93
        ret
L3B19:
        retn
L3B1A:
        cmp     word ptr ds:[5044h],0
        je      L3B31
        shl     bx,1
        cmp     ax,word ptr [bx+5044h]
        jge     L3B2F
        cmp     ax,word ptr [bx+5044h]
        jge     L3B19
L3B2F:
        shr     bx,1
L3B31:
        jmp     _seg003_5AA2
_seg003_3B34:
        cmp     bx,word ptr ds:[3DF4h]
        jg      L3B19
        cmp     bx,word ptr ds:[3DF8h]
        jl      L3B19
        cmp     ax,word ptr ds:[3DF6h]
        jg      L3B19
        cmp     ax,word ptr ds:[3DF2h]
        jl      L3B19
        cmp     word ptr ds:[4110h],offset _seg003_5AC3
        je      L3B1A
        mov     di,4148h
        mov     si,di
        xchg    bx,ax
        stosw
        xchg    bx,ax
        stosw
        stosw
        jmp     word ptr ds:[4110h]
_seg003_3B0F endp

; seg003_3B62  (+3B62)
; _3B62: clip a vertical line (x AX, y from BX to DX) to the window. Returns with BX >= DX
; inside the window, or discards its caller's return address (pop ax; ret) so the caller returns
; at once when the line is wholly outside. The code after it is a clipped vertical line through
; vector 5AAB.
_seg003_3B62 proc near
        cmp     ax,word ptr ds:[3DF6h]
        jg      L3B91
        cmp     ax,word ptr ds:[3DF2h]
        jl      L3B91
        cmp     bx,dx
        jg      L3B74
        xchg    bx,dx
L3B74:
        mov     si,word ptr ds:[3DF8h]
        cmp     bx,si
        jl      L3B91
        cmp     dx,si
        jg      L3B82
        mov     dx,si
L3B82:
        mov     si,word ptr ds:[3DF4h]
        cmp     dx,si
        jg      L3B91
        cmp     bx,si
        jl      L3B90
        mov     bx,si
L3B90:
        ret
L3B91:
        pop     ax
        ret
        call    _seg003_3B62
        call    _seg003_5AAB
        ret
_seg003_3B62 endp

; seg003_3B9A  (+3B9A)
; _3B9A: a clipped vertical line through the vector at 5AAE.
_seg003_3B9A proc near
        call    _seg003_3B62
        call    _seg003_5AAE
        ret
_seg003_3B9A endp

; seg003_3BA1  (+3BA1)
; vline: clip (_3B62) and fall into uvline.
_seg003_3BA1 proc near
        call    _seg003_3B62
_seg003_3BA1 endp

; seg003_3BA4  (+3BA4)
; uvline: a vertical line at x AX from y BX to DX. The solid and copy writers have direct
; vertical-line routines (5AAB, 5AAE); otherwise it builds one span per row at 2D40 and runs the
; span writer. The code after it loads AX, BX, CX, DX from four words at SI (ubox_si, probably)
; and falls into box.
_seg003_3BA4 proc near
        cmp     word ptr ds:[5044h],0
        jne     L3BC1
        mov     si,word ptr ds:[4110h]
        cmp     si,offset _seg003_5AC3
        jne     L3BB8
        jmp     _seg003_5AAB
L3BB8:
        cmp     si,offset _seg003_5AC6
        jne     L3BC1
        jmp     _seg003_5AAE
L3BC1:
        cmp     bx,dx
        jg      L3BC7
        xchg    bx,dx
L3BC7:
        mov     cx,bx
        sub     cx,dx
        inc     cx
        mov     dx,ax
        mov     di,2D40h
L3BD1:
        mov     ax,bx
        stosw
        mov     ax,dx
        stosw
        stosw
        dec     bx
        loop    L3BD1
        mov     word ptr [di],0FFFFh
        mov     si,2D40h
        jmp     word ptr ds:[4110h]
        lodsw
        xchg    dx,ax
        lodsw
        xchg    bx,ax
        lodsw
        xchg    cx,ax
        lodsw
        xchg    dx,ax
_seg003_3BA4 endp

; seg003_3BEE  (+3BEE)
; box: clip the rectangle (clip_rect) and fall into ubox.
_seg003_3BEE proc near
        call    _seg003_3C3E
_seg003_3BEE endp

; seg003_3BF1  (+3BF1)
; ubox: an outline rectangle with corners (AX, BX) and (CX, DX): two horizontal lines (uhline)
; and two vertical ones (uvline). The code after it is the same with the corners read from SI.
_seg003_3BF1 proc near
        mov     di,4150h
        stosw
        xchg    bx,ax
        stosw
        xchg    cx,ax
        stosw
        xchg    dx,ax
        stosw
L3BFB:
        mov     ax,word ptr ds:[4150h]
        mov     bx,word ptr ds:[4152h]
        mov     cx,word ptr ds:[4154h]
        call    _seg003_3D2E
        mov     ax,word ptr ds:[4150h]
        mov     bx,word ptr ds:[4156h]
        mov     cx,word ptr ds:[4154h]
        call    _seg003_3D2E
        mov     ax,word ptr ds:[4150h]
        mov     bx,word ptr ds:[4152h]
        mov     dx,word ptr ds:[4156h]
        call    _seg003_3BA4
        mov     ax,word ptr ds:[4154h]
        mov     bx,word ptr ds:[4152h]
        mov     dx,word ptr ds:[4156h]
        call    _seg003_3BA4
        ret
        mov     di,4150h
        mov     cx,4
        rep movsw
        jmp     L3BFB
_seg003_3BF1 endp

; seg003_3C3E  (+3C3E)
; clip_rect: order and clip the rectangle (AX, BX)-(CX, DX) to the window. Out: AX <= CX, DX <=
; BX (BX the upper row), inside the window; BP = the edges that were cut (8 left, 2 right, 1
; bottom, 4 top); carry set when nothing was cut. A rectangle wholly outside discards the
; caller's return address. _3C96 loads the four words at SI and falls into it.
_seg003_3C3E proc near
        sub     bp,bp
        cmp     bx,dx
        jg      L3C46
        xchg    bx,dx
L3C46:
        cmp     ax,cx
        jl      L3C4B
        xchg    cx,ax
L3C4B:
        mov     si,word ptr ds:[3DF2h]
        cmp     cx,si
        jl      L3C94
        cmp     ax,si
        jge     L3C5C
        mov     ax,si
        add     bp,8
L3C5C:
        mov     si,word ptr ds:[3DF6h]
        cmp     ax,si
        jg      L3C94
        cmp     cx,si
        jle     L3C6D
        mov     cx,si
        add     bp,2
L3C6D:
        mov     si,word ptr ds:[3DF8h]
        cmp     bx,si
        jl      L3C94
        cmp     dx,si
        jge     L3C7E
        mov     dx,si
        add     bp,1
L3C7E:
        mov     si,word ptr ds:[3DF4h]
        cmp     dx,si
        jg      L3C94
        cmp     bx,si
        jle     L3C8F
        mov     bx,si
        add     bp,4
L3C8F:
        cmp     bp,0
        cmc
        ret
L3C94:
        pop     ax
        ret
_seg003_3C96:
        lodsw
        xchg    dx,ax
        lodsw
        xchg    bx,ax
        lodsw
        xchg    cx,ax
        lodsw
        xchg    dx,ax
_seg003_3C3E endp

; seg003_3C9E  (+3C9E)
; rectangle: clip_rect, then urectangle.
_seg003_3C9E proc near
        call    _seg003_3C3E
        jmp     short _seg003_3CAE
_seg003_3C9E endp

; seg003_3CA3  (+3CA3)
; clear_window: urectangle over the whole clip window.
_seg003_3CA3 proc near
        mov     si,3DF2h
        lodsw
        xchg    dx,ax
        lodsw
        xchg    bx,ax
        lodsw
        xchg    cx,ax
        lodsw
        xchg    dx,ax
_seg003_3CA3 endp

; seg003_3CAE  (+3CAE)
; urectangle: a filled rectangle (AX, BX)-(CX, DX), corners in either order. Writes one span per
; row into the row table at 44CC (the x range copied into each), moves the end mark, and runs
; the span writer on it.
_seg003_3CAE proc near
        cmp     bx,dx
        jg      L3CB4
        xchg    bx,dx
L3CB4:
        cmp     ax,cx
        jl      L3CB9
        xchg    cx,ax
L3CB9:
        mov     word ptr ds:[4158h],ax
        mov     word ptr ds:[415Ah],cx
        mov     ax,bx
        mov     cx,dx
        neg     cx
        add     cx,ax
        inc     cx
        mov     si,word ptr ds:[49AAh]
        and     word ptr [si],7FFFh
        mov     di,44CCh
        mov     si,bx
        neg     si
        add     si,0C7h
        shl     si,1
        add     di,si
        shl     si,1
        add     di,si
        push    di
        mov     bx,4158h
L3CE8:
        add     di,2
        mov     si,bx
        movsw
        movsw
        loop    L3CE8
        mov     word ptr ds:[49AAh],di
        or      word ptr [di],8000h
        pop     si
        jmp     word ptr ds:[4110h]
L3CFE:
        retn
_seg003_3CAE endp

; seg003_3CFF  (+3CFF)
; hline: clip a horizontal line (y BX, x from AX to CX) to the window and draw it as one span;
; returns at once when it is outside.
_seg003_3CFF proc near
        cmp     bx,word ptr ds:[3DF4h]
        jg      L3CFE
        cmp     bx,word ptr ds:[3DF8h]
        jl      L3CFE
        cmp     ax,cx
        jl      L3D10
        xchg    cx,ax
L3D10:
        mov     dx,word ptr ds:[3DF2h]
        cmp     cx,dx
        jl      L3CFE
        cmp     ax,dx
        jg      L3D1E
        mov     ax,dx
L3D1E:
        mov     dx,word ptr ds:[3DF6h]
        cmp     ax,dx
        jg      L3CFE
        cmp     cx,dx
        jl      L3D2C
        mov     cx,dx
L3D2C:
        jmp     short L3D33
_seg003_3CFF endp

; seg003_3D2E  (+3D2E)
; uhline: one span (y BX, x AX to CX, either order) through the span writer.
_seg003_3D2E proc near
        cmp     ax,cx
        jl      L3D33
        xchg    cx,ax
L3D33:
        mov     di,4148h
        mov     si,di
        xchg    bx,ax
        stosw
        xchg    bx,ax
        stosw
        xchg    cx,ax
        stosw
        jmp     word ptr ds:[4110h]
_seg003_3D2E endp

; seg003_3D42  (+3D42)
; concave_shclip (FM Towns, 7 bytes before shclip there as here): shclip with the handler set at
; 4498 in place of 4490.
_seg003_3D42 proc near
        mov     si,4498h
        jmp     short L3D4A
_seg003_3D42 endp

; seg003_3D47  (+3D47)
; shclip: clip a polygon to the window, Sutherland-Hodgman, one edge at a time. In: CX = the
; vertex count, the vertices (x, y words) at 415C. A pass is skipped when every vertex is
; already inside on that axis. Each pass (L3E0F) walks the edges, classifies both ends by
; patching the comparison at L3E30 and L3E3E (7Ch jl or 7Fh jg, followed by a far jump to flush
; the prefetch queue), and calls one of four handlers through 4488 by the in/out case: keep, add
; the intersection, or drop. The intersection (L3E91, L3ECC) is computed with imul/idiv, halving
; both terms on overflow, and nudged one pixel inward unless it lies on the window edge. Out: CX
; = the new count, the vertices back at 415C; when nothing is left it discards the caller's
; return address.
_seg003_3D47 proc near
        mov     si,4490h
L3D4A:
        mov     di,4488h
        movsw
        movsw
        movsw
        movsw
        mov     si,415Ch
        mov     di,cx
        shl     di,1
        shl     di,1
        add     di,si
        movsw
        movsw
        sub     si,4
        mov     bx,word ptr ds:[3DF4h]
        mov     dx,word ptr ds:[3DF8h]
        mov     si,415Eh
        push    cx
L3D6D:
        lodsw
        inc     si
        inc     si
        cmp     ax,bx
        jg      L3D7A
        cmp     ax,dx
        jl      L3D7A
        loop    L3D6D
L3D7A:
        test    cx,cx
        pop     cx
        je      L3DB5
        mov     word ptr ds:[4480h],2
        mov     word ptr ds:[4482h],8
        mov     word ptr ds:[4484h],offset L3ECC
        mov     si,415Ch
        mov     di,42ECh
        mov     al,7Ch
        mov     dx,word ptr ds:[3DF4h]
        call    L3E0F
        mov     word ptr ds:[4484h],offset L3EC8
        mov     si,42ECh
        mov     di,415Ch
        mov     al,7Fh
        mov     dx,word ptr ds:[3DF8h]
        call    L3E0F
L3DB5:
        mov     bx,word ptr ds:[3DF6h]
        mov     dx,word ptr ds:[3DF2h]
        mov     si,415Ch
        push    cx
L3DC1:
        lodsw
        inc     si
        inc     si
        cmp     ax,bx
        jg      L3DCE
        cmp     ax,dx
        jl      L3DCE
        loop    L3DC1
L3DCE:
        test    cx,cx
        pop     cx
        je      L3E09
        mov     word ptr ds:[4480h],0
        mov     word ptr ds:[4482h],6
        mov     word ptr ds:[4484h],offset L3E91
        mov     si,415Ch
        mov     di,42ECh
        mov     al,7Fh
        mov     dx,word ptr ds:[3DF2h]
        call    L3E0F
        mov     word ptr ds:[4484h],offset L3E8D
        mov     si,42ECh
        mov     di,415Ch
        mov     al,7Ch
        mov     dx,word ptr ds:[3DF6h]
        call    L3E0F
L3E09:
        jcxz    L3E0D
        ret
L3E0C:
        pop     ax
L3E0D:
        pop     ax
        ret
L3E0F:
        jcxz    L3E0C
        push    di
        mov     byte ptr cs:L3E30,al
        mov     byte ptr cs:L3E3E,al
        mov     word ptr ds:[447Eh],cx
        mov     word ptr ds:[447Ch],cx
        db      0EAh                     ; far jmp to the next instruction, flushing the prefetch queue after the cs: stores above (match: as bytes, since TASM would shorten it)
        dw      offset L3E27
        dw      seg L3E27
L3E27:
        sub     bx,bx
        add     si,word ptr ds:[4480h]
        lodsw
        cmp     ax,dx
L3E30:                                  ; its opcode is patched above
        jg      L3E35
        mov     bx,4
L3E35:
        inc     si
        inc     si
        lodsw
        sub     si,word ptr ds:[4482h]
        cmp     ax,dx
L3E3E:
        jg      L3E43
        or      bx,2
L3E43:
        call    word ptr [bx+4488h]
        dec     word ptr ds:[447Eh]
        jg      L3E27
        mov     cx,word ptr ds:[447Ch]
        pop     si
        movsw
        movsw
        ret
        movsw
        movsw
        sub     si,4
        inc     word ptr ds:[447Ch]
        mov     word ptr ds:[4486h],0
        jmp     word ptr ds:[4484h]
        movsw
        movsw
        sub     si,4
        inc     word ptr ds:[447Ch]
        mov     word ptr ds:[4486h],0
        jmp     word ptr ds:[4484h]
        mov     word ptr ds:[4486h],1
        jmp     word ptr ds:[4484h]
        dec     word ptr ds:[447Ch]
        add     si,4
        ret
L3E8D:
        neg     word ptr ds:[4486h]
L3E91:
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
        jo      L3EC2
L3EA9:
        imul    dx
        idiv    cx
        add     ax,bx
        cmp     ax,word ptr ds:[3DF4h]
        jge     L3EBF
        cmp     ax,word ptr ds:[3DF8h]
        jle     L3EBF
        add     ax,word ptr ds:[4486h]
L3EBF:
        stosw
        pop     dx
        ret
L3EC2:
        rcr     cx,1
        sar     dx,1
        jmp     L3EA9
L3EC8:
        neg     word ptr ds:[4486h]
L3ECC:
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
        jo      L3EFF
L3EE4:
        imul    dx
        idiv    cx
        add     ax,bx
        cmp     ax,word ptr ds:[3DF2h]
        jle     L3EFA
        cmp     ax,word ptr ds:[3DF6h]
        jge     L3EFA
        add     ax,word ptr ds:[4486h]
L3EFA:
        stosw
        pop     ax
        stosw
        xchg    dx,ax
        ret
L3EFF:
        rcr     cx,1
        sar     dx,1
        jmp     L3EE4
_seg003_3D47 endp

SEG003_TEXT ends

        end
