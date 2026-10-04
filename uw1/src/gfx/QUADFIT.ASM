; /* target: seg003_8E8 */
; /* opts: /ml */
;
; Graphics library module 5: the coefficients of a quadratic through three values. It takes the
; values at 934 .. 938 and 93E .. 942 (seg048) relative to the first of each, and returns a start
; and two steps in BP:SI and BX:DX, with working values up to 94E; probably an alternative
; stepping for the wall mapper's texture coordinate (UW2Decomp's reading). No caller was found.
;
; UW2Decomp keeps this routine at the end of src/gfx/POLYFILL.ASM ("the routine after L0582 (no
; label, no caller found) works out the coefficients of a quadratic"), UW2's seg003_0272_584
; .. 6E0. UW1 has the same code, with the data 2 bytes higher (934 for UW2's 932).
;
; name: descriptive (what it computes); no FM Towns counterpart was looked for.
;
; SEG003 split into its original modules: this is module 5 of UW1's 18, +8E8..+A44.
; Evidence for the bounds: POLYFILL's two code-segment words end at +8E7 (odd), and one zero
; byte follows before this code at +8E8. TASM pads with 90h (`even`), never with zero; a zero
; byte there is TLINK's padding before a word-aligned module, so in UW1 the routine is a module
; of its own (in UW2 the words end on an even byte and the routine follows at once, so nothing
; shows a boundary there). It ends at the zero byte at +A45, GRENTRY starting at +A46.
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        public  _seg003_8E8

; seg003_8E8  (+8E8)
_seg003_8E8 proc near
        mov     ax,word ptr ds:[93Eh]
        sub     word ptr ds:[940h],ax
        sub     word ptr ds:[942h],ax
        mov     ax,word ptr ds:[934h]
        sub     word ptr ds:[936h],ax
        sub     word ptr ds:[938h],ax
        mov     ax,word ptr ds:[938h]
        sub     ax,word ptr ds:[936h]
        imul    word ptr ds:[938h]
        or      ax,ax
        jne     L0910
        jmp     L0A2D
L0910:
        mov     cx,ax
        mov     ax,word ptr ds:[942h]
        xor     dx,dx
        div     cx
        mov     word ptr ds:[94Ah],ax
        xor     ax,ax
        div     cx
        mov     word ptr ds:[948h],ax
        mov     ax,word ptr ds:[938h]
        sub     ax,word ptr ds:[936h]
        imul    word ptr ds:[936h]
        or      ax,ax
        jne     L0935
        jmp     L0A2D
L0935:
        mov     cx,ax
        mov     ax,word ptr ds:[940h]
        xor     dx,dx
        div     cx
        mov     word ptr ds:[94Eh],ax
        xor     ax,ax
        div     cx
        mov     word ptr ds:[94Ch],ax
        mov     ax,word ptr ds:[938h]
        mul     word ptr ds:[94Ch]
        mov     bp,dx
        mov     si,ax
        mov     ax,word ptr ds:[938h]
        mul     word ptr ds:[94Eh]
        add     bp,ax
        mov     ax,word ptr ds:[936h]
        mul     word ptr ds:[948h]
        sub     si,ax
        sbb     bp,dx
        mov     ax,word ptr ds:[936h]
        imul    word ptr ds:[94Ah]
        sub     bp,ax
        mov     bx,word ptr ds:[94Ah]
        mov     dx,word ptr ds:[948h]
        sub     dx,word ptr ds:[94Ch]
        sbb     bx,word ptr ds:[94Eh]
        add     si,dx
        adc     bp,bx
        shl     dx,1
        rcl     bx,1
        mov     ax,word ptr ds:[93Eh]
        ret
        mov     ax,word ptr ds:[942h]
        sub     word ptr ds:[944h],ax
        sub     word ptr ds:[946h],ax
        mov     ax,word ptr ds:[938h]
        sub     word ptr ds:[93Ah],ax
        sub     word ptr ds:[93Ch],ax
        mov     ax,word ptr ds:[93Ch]
        sub     ax,word ptr ds:[93Ah]
        imul    word ptr ds:[93Ch]
        or      ax,ax
        jne     L09B4
        jmp     L0A39
L09B4:
        mov     cx,ax
        mov     ax,word ptr ds:[946h]
        xor     dx,dx
        div     cx
        mov     word ptr ds:[94Ah],ax
        xor     ax,ax
        div     cx
        mov     word ptr ds:[948h],ax
        mov     ax,word ptr ds:[93Ch]
        sub     ax,word ptr ds:[93Ah]
        imul    word ptr ds:[93Ah]
        or      ax,ax
        je      L0A39
        mov     cx,ax
        mov     ax,word ptr ds:[944h]
        xor     dx,dx
        div     cx
        mov     word ptr ds:[94Eh],ax
        xor     ax,ax
        div     cx
        mov     word ptr ds:[94Ch],ax
        mov     ax,word ptr ds:[93Ch]
        mul     word ptr ds:[94Ch]
        mov     bp,dx
        mov     si,ax
        mov     ax,word ptr ds:[93Ch]
        mul     word ptr ds:[94Eh]
        add     bp,ax
        mov     ax,word ptr ds:[93Ah]
        mul     word ptr ds:[948h]
        sub     si,ax
        sbb     bp,dx
        mov     ax,word ptr ds:[93Ah]
        imul    word ptr ds:[94Ah]
        sub     bp,ax
        mov     bx,word ptr ds:[94Ah]
        mov     dx,word ptr ds:[948h]
        sub     dx,word ptr ds:[94Ch]
        sbb     bx,word ptr ds:[94Eh]
        add     si,dx
        adc     bp,bx
        shl     dx,1
        rcl     bx,1
        mov     ax,word ptr ds:[942h]
        ret
L0A2D:
        xor     bp,bp
        xor     bx,bx
        xor     dx,dx
        xor     si,si
        mov     ax,word ptr ds:[93Eh]
        ret
L0A39:
        xor     bp,bp
        xor     bx,bx
        xor     dx,dx
        xor     si,si
        mov     ax,word ptr ds:[942h]
        ret
_seg003_8E8 endp

SEG003_TEXT ends

        end
