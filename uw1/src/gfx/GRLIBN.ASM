; /* target: seg003_6350 */
; /* opts: /ml */
;
; Graphics library module 18: the clipper for smooth (Gouraud) polygons, FM Towns' s_shclip, and
; its helper. It is GRLIBF.ASM's shclip for 6-byte vertices (x, y, intensity): the same
; Sutherland-Hodgman passes against the window's top and bottom and then left and right, with
; the comparison opcodes patched in place (7Ch jl or 7Fh jg) and a far jump to flush the
; prefetch queue, and the intersections interpolate the intensity as well as the position. The
; four case handlers are reached through the near offsets GRDISP.ASM stores at 5E74.
;
; In: CX = the vertex count, the vertices at 415C; DS = seg048. Out: CX = the count after
; clipping, the vertices back at 415C; nothing left discards the caller's return address.
; Data: 42EC the second vertex buffer; 447C..4486 the pass state; seg048:07D1 the pass's
; vertex counter.
;
; name: descriptive (UW2Decomp: module 14 of seg003); its job, found here, is clipping smooth
; polygons. The
; routine names are FM Towns'.
;
; UW1 against UW2: UW2Decomp's src/gfx/GRLIBN.ASM (UW2's seg003_0272_5B50), the same code and
; length; the data sits 2 bytes lower (415C for UW2's 415E), but the pass's vertex counter is
; the word at 07D1, odd and one byte higher than UW2's 07D0 (POLYFILL's far piece starts there
; in UW2, so UW2Decomp names it; nothing in UW1 does).
;
; SEG003 split into its original modules: this is module 18 of UW1's 18, +6350..+656C. It ends
; the segment (segment table entry 3 gives seg003 656Dh bytes; seg004 starts at the next
; paragraph, 6570h).
; Evidence for the bounds: starts after one zero byte following a jmp (TLINK's padding before a
; word-aligned module), as in UW2.
; Its relocation is entry 2861 of UW.EXE's relocation table (+642C).

        .186


_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:nothing, ss:DGROUP     ; DS is seg048, set by the caller
        public  _seg003_6350
        public  _seg003_6547

; seg003_6350  (+6350)
; s_shclip: attach intensities (_6547), close the list, then clip top and bottom (skipped when
; every vertex is inside them) and left and right. L6416 is one pass; L649C and L64F0 work out
; an intersection with a horizontal or vertical edge (imul/idiv, both terms halved on overflow)
; and nudge it one pixel inward unless it lies on the edge.
_seg003_6350 proc near
        call    _seg003_6547
        mov     si,415Ch
        mov     di,cx
        shl     di,1
        add     di,cx
        shl     di,1
        add     di,si
        movsw
        movsw
        movsw
        sub     si,6
        mov     bx,word ptr ds:[3DF4h]
        mov     dx,word ptr ds:[3DF8h]
        mov     si,415Eh
        push    cx
L6372:
        lodsw
        add     si,4
        cmp     ax,bx
        jg      L6380
        cmp     ax,dx
        jl      L6380
        loop    L6372
L6380:
        test    cx,cx
        pop     cx
        je      L63BB
        mov     word ptr ds:[4480h],2
        mov     word ptr ds:[4482h],0Ah
        mov     word ptr ds:[4484h],offset L64F0
        mov     si,415Ch
        mov     di,42ECh
        mov     al,7Ch
        mov     dx,word ptr ds:[3DF4h]
        call    L6416
        mov     word ptr ds:[4484h],offset L64EC
        mov     si,42ECh
        mov     di,415Ch
        mov     al,7Fh
        mov     dx,word ptr ds:[3DF8h]
        call    L6416
L63BB:
        mov     bx,word ptr ds:[3DF6h]
        mov     dx,word ptr ds:[3DF2h]
        mov     si,415Ch
        push    cx
L63C7:
        lodsw
        add     si,4
        cmp     ax,bx
        jg      L63D5
        cmp     ax,dx
        jl      L63D5
        loop    L63C7
L63D5:
        test    cx,cx
        pop     cx
        je      L6410
        mov     word ptr ds:[4480h],0
        mov     word ptr ds:[4482h],8
        mov     word ptr ds:[4484h],offset L649C
        mov     si,415Ch
        mov     di,42ECh
        mov     al,7Fh
        mov     dx,word ptr ds:[3DF2h]
        call    L6416
        mov     word ptr ds:[4484h],offset L6498
        mov     si,42ECh
        mov     di,415Ch
        mov     al,7Ch
        mov     dx,word ptr ds:[3DF6h]
        call    L6416
L6410:
        jcxz    L6414
        ret
L6413:
        pop     ax
L6414:
        pop     ax
        ret
L6416:
        jcxz    L6413
        push    di
        mov     byte ptr cs:L6437,al
        mov     byte ptr cs:L6446,al
        mov     word ptr ds:[7D1h],cx
        mov     word ptr ds:[447Ch],cx
        db      0EAh                     ; far jmp to the next instruction, flushing the prefetch queue after the cs: stores above (match: as bytes, since TASM would shorten it)
        dw      offset L642E
        dw      seg L642E
L642E:
        sub     bx,bx
        add     si,word ptr ds:[4480h]
        lodsw
        cmp     ax,dx
L6437:
        jg      L643C
        mov     bx,4
L643C:
        add     si,4
        lodsw
        sub     si,word ptr ds:[4482h]
        cmp     ax,dx
L6446:
        jg      L644B
        or      bx,2
L644B:
        call    word ptr [bx+5E74h]
        dec     word ptr ds:[7D1h]
        jg      L642E
        mov     cx,word ptr ds:[447Ch]
        pop     si
        movsw
        movsw
        movsw
        ret
        movsw
        movsw
        movsw
        sub     si,6
        inc     word ptr ds:[447Ch]
        mov     word ptr ds:[4486h],0
        jmp     word ptr ds:[4484h]
        movsw
        movsw
        movsw
        sub     si,6
        inc     word ptr ds:[447Ch]
        mov     word ptr ds:[4486h],0
        jmp     word ptr ds:[4484h]
        mov     word ptr ds:[4486h],1
        jmp     word ptr ds:[4484h]
        dec     word ptr ds:[447Ch]
        add     si,6
        ret
L6498:
        neg     word ptr ds:[4486h]
L649C:
        push    dx
        mov     ax,dx
        stosw
        lodsw
        xchg    cx,ax
        lodsw
        xchg    bx,ax
        add     si,2
        lodsw
        xchg    bp,ax
        lodsw
        sub     si,4
        sub     ax,bx
        sub     dx,cx
        neg     cx
        add     cx,bp
        jo      L64E4
L64B7:
        push    dx
        push    cx
        imul    dx
        idiv    cx
        add     ax,bx
        cmp     ax,word ptr ds:[3DF4h]
        jge     L64CF
        cmp     ax,word ptr ds:[3DF8h]
        jle     L64CF
        add     ax,word ptr ds:[4486h]
L64CF:
        stosw
        pop     cx
        pop     dx
        push    ax
        mov     ax,word ptr [si+4]
        sub     ax,word ptr [si-2]
        imul    dx
        idiv    cx
        add     ax,word ptr [si-2]
        stosw
        pop     ax
        pop     dx
        ret
L64E4:
        rcr     cx,1
        sar     dx,1
        jmp     L64B7
L64EA   dw      0
L64EC:
        neg     word ptr ds:[4486h]
L64F0:
        mov     word ptr cs:L64EA,dx
        lodsw
        xchg    cx,ax
        lodsw
        xchg    bx,ax
        add     si,2
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
        jo      L6541
L650F:
        push    dx
        push    cx
        imul    dx
        idiv    cx
        add     ax,bx
        cmp     ax,word ptr ds:[3DF2h]
        jle     L6527
        cmp     ax,word ptr ds:[3DF6h]
        jge     L6527
        add     ax,word ptr ds:[4486h]
L6527:
        stosw
        mov     ax,word ptr cs:L64EA
        stosw
        pop     cx
        pop     dx
        push    ax
        mov     ax,word ptr [si+4]
        sub     ax,word ptr [si-2]
        imul    dx
        idiv    cx
        add     ax,word ptr [si-2]
        stosw
        pop     ax
        xchg    dx,ax
        ret
L6541:
        rcr     cx,1
        sar     dx,1
        jmp     L650F
_seg003_6350 endp

; seg003_6547  (+6547)
; _6547 (probably FM Towns' pack_vertices, which comes next to s_shclip there): turn CX (x, y)
; vertices at 415C into (x, y, intensity) ones, taking the intensities in order from the word
; table at 55EC (assembled in 42EC and copied back).
_seg003_6547 proc near
        mov     si,415Ch
        mov     bx,55ECh
        mov     di,42ECh
        push    cx
L6551:
        movsw
        movsw
        mov     ax,word ptr [bx]
        add     bx,2
        stosw
        loop    L6551
        pop     cx
        push    cx
        mov     ax,cx
        shl     ax,1
        add     cx,ax
        mov     si,42ECh
        mov     di,415Ch
        rep movsw
        pop     cx
        ret
_seg003_6547 endp

SEG003_TEXT ends

        end
