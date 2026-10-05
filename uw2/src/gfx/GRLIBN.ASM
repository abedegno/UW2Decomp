; /* target: seg003_0272_5B50 */
; /* opts: /ml */
;
; Graphics library module 14: the clipper for smooth (Gouraud) polygons, FM Towns' s_shclip, and
; its helper. It is GRLIBF.ASM's shclip for 6-byte vertices (x, y, intensity): the same
; Sutherland-Hodgman passes against the window's top and bottom and then left and right, with
; the comparison opcodes patched in place (7Ch jl or 7Fh jg) and a far jump to flush the
; prefetch queue, and the intersections interpolate the intensity as well as the position. The
; four case handlers are reached through the near offsets GRDISP.ASM stores at 5E76.
;
; In: CX = the vertex count, the vertices at 415E; DS = seg_370D. Out: CX = the count after
; clipping, the vertices back at 415E; nothing left discards the caller's return address.
; Data: 42EE the second vertex buffer; 447E..4488 the pass state; seg_370D:07D0 the pass's
; vertex counter.
;
; name: descriptive (module 14 of seg003); its job, found here, is clipping smooth polygons. The
; routine names are FM Towns'.
;
; SEG003 split into its original modules: this is module 14 of 14, +5B50..+5D6D of the segment
; (file 8FA0h), a module of the second library (UWLIB.LIB).
; Procs: seg003_0272_5B50, seg003_0272_5D47.
; Evidence for the bounds: starts after one zero byte following a jmp: TLINK's padding before
; a word-aligned module (TASM 2.0's `even` pads with 90h).
; Its relocations are entries 2574 of UW2.EXE's relocation table.

        .186

        extrn   _seg_370D_7D0:word

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:nothing, ss:DGROUP     ; DS is seg_370D, set by the caller
        public  _seg003_0272_5B50
        public  _seg003_0272_5D47

; seg003_0272_5B50  (+5B50)
; s_shclip: attach intensities (_5D47), close the list, then clip top and bottom (skipped when
; every vertex is inside them) and left and right. L5C16 is one pass; L5C9C and L5CF0 work out
; an intersection with a horizontal or vertical edge (imul/idiv, both terms halved on overflow)
; and nudge it one pixel inward unless it lies on the edge.
_seg003_0272_5B50 proc near
        call    _seg003_0272_5D47
        mov     si,415Eh
        mov     di,cx
        shl     di,1
        add     di,cx
        shl     di,1
        add     di,si
        movsw
        movsw
        movsw
        sub     si,6
        mov     bx,word ptr ds:[3DF6h]
        mov     dx,word ptr ds:[3DFAh]
        mov     si,4160h
        push    cx
L5B72:
        lodsw
        add     si,4
        cmp     ax,bx
        jg      L5B80
        cmp     ax,dx
        jl      L5B80
        loop    L5B72
L5B80:
        test    cx,cx
        pop     cx
        je      L5BBB
        mov     word ptr ds:[4482h],2
        mov     word ptr ds:[4484h],0Ah
        mov     word ptr ds:[4486h],offset L5CF0
        mov     si,415Eh
        mov     di,42EEh
        mov     al,7Ch
        mov     dx,word ptr ds:[3DF6h]
        call    L5C16
        mov     word ptr ds:[4486h],offset L5CEC
        mov     si,42EEh
        mov     di,415Eh
        mov     al,7Fh
        mov     dx,word ptr ds:[3DFAh]
        call    L5C16
L5BBB:
        mov     bx,word ptr ds:[3DF8h]
        mov     dx,word ptr ds:[3DF4h]
        mov     si,415Eh
        push    cx
L5BC7:
        lodsw
        add     si,4
        cmp     ax,bx
        jg      L5BD5
        cmp     ax,dx
        jl      L5BD5
        loop    L5BC7
L5BD5:
        test    cx,cx
        pop     cx
        je      L5C10
        mov     word ptr ds:[4482h],0
        mov     word ptr ds:[4484h],8
        mov     word ptr ds:[4486h],offset L5C9C
        mov     si,415Eh
        mov     di,42EEh
        mov     al,7Fh
        mov     dx,word ptr ds:[3DF4h]
        call    L5C16
        mov     word ptr ds:[4486h],offset L5C98
        mov     si,42EEh
        mov     di,415Eh
        mov     al,7Ch
        mov     dx,word ptr ds:[3DF8h]
        call    L5C16
L5C10:
        jcxz    L5C14
        ret
L5C13:
        pop     ax
L5C14:
        pop     ax
        ret
L5C16:
        jcxz    L5C13
        push    di
        mov     byte ptr cs:L5C37,al
        mov     byte ptr cs:L5C46,al
        mov     word ptr ds:_seg_370D_7D0,cx
        mov     word ptr ds:[447Eh],cx
        db      0EAh                     ; far jmp to the next instruction, flushing the prefetch queue after the cs: stores above (match: as bytes, since TASM would shorten it)
        dw      offset L5C2E
        dw      seg L5C2E
L5C2E:
        sub     bx,bx
        add     si,word ptr ds:[4482h]
        lodsw
        cmp     ax,dx
L5C37:
        jg      L5C3C
        mov     bx,4
L5C3C:
        add     si,4
        lodsw
        sub     si,word ptr ds:[4484h]
        cmp     ax,dx
L5C46:
        jg      L5C4B
        or      bx,2
L5C4B:
        call    word ptr [bx+5E76h]
        dec     word ptr ds:_seg_370D_7D0
        jg      L5C2E
        mov     cx,word ptr ds:[447Eh]
        pop     si
        movsw
        movsw
        movsw
        ret
        movsw
        movsw
        movsw
        sub     si,6
        inc     word ptr ds:[447Eh]
        mov     word ptr ds:[4488h],0
        jmp     word ptr ds:[4486h]
        movsw
        movsw
        movsw
        sub     si,6
        inc     word ptr ds:[447Eh]
        mov     word ptr ds:[4488h],0
        jmp     word ptr ds:[4486h]
        mov     word ptr ds:[4488h],1
        jmp     word ptr ds:[4486h]
        dec     word ptr ds:[447Eh]
        add     si,6
        ret
L5C98:
        neg     word ptr ds:[4488h]
L5C9C:
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
        jo      L5CE4
L5CB7:
        push    dx
        push    cx
        imul    dx
        idiv    cx
        add     ax,bx
        cmp     ax,word ptr ds:[3DF6h]
        jge     L5CCF
        cmp     ax,word ptr ds:[3DFAh]
        jle     L5CCF
        add     ax,word ptr ds:[4488h]
L5CCF:
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
L5CE4:
        rcr     cx,1
        sar     dx,1
        jmp     L5CB7
L5CEA   dw      0
L5CEC:
        neg     word ptr ds:[4488h]
L5CF0:
        mov     word ptr cs:L5CEA,dx
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
        jo      L5D41
L5D0F:
        push    dx
        push    cx
        imul    dx
        idiv    cx
        add     ax,bx
        cmp     ax,word ptr ds:[3DF4h]
        jle     L5D27
        cmp     ax,word ptr ds:[3DF8h]
        jge     L5D27
        add     ax,word ptr ds:[4488h]
L5D27:
        stosw
        mov     ax,word ptr cs:L5CEA
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
L5D41:
        rcr     cx,1
        sar     dx,1
        jmp     L5D0F
_seg003_0272_5B50 endp

; seg003_0272_5D47  (+5D47)
; _5D47 (probably FM Towns' pack_vertices, which comes next to s_shclip there): turn CX (x, y)
; vertices at 415E into (x, y, intensity) ones, taking the intensities in order from the word
; table at 55EE (assembled in 42EE and copied back).
_seg003_0272_5D47 proc near
        mov     si,415Eh
        mov     bx,55EEh
        mov     di,42EEh
        push    cx
L5D51:
        movsw
        movsw
        mov     ax,word ptr [bx]
        add     bx,2
        stosw
        loop    L5D51
        pop     cx
        push    cx
        mov     ax,cx
        shl     ax,1
        add     cx,ax
        mov     si,42EEh
        mov     di,415Eh
        rep movsw
        pop     cx
        ret
_seg003_0272_5D47 endp

SEG003_TEXT ends

        end
