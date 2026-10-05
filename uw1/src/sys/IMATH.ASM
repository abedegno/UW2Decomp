; /* target: seg019_A30 */
; /* opts: /ml */
;
; UW1: UW2's module (src/sys in UW2Decomp), matched to UW.EXE's seg019; the differences are
; marked UW1/UW2 where they occur. Other remarks are UW2's unless they say UW1.
;
; Integer trigonometry and square roots for the 3D code and the game, from tables in seg063 (DS =
; seg063). FM Towns has sincos, fast_sincos, lsqrt, lsqrt32, fast_asin and atan2 together,
; the same set; C3DENTRY.ASM's cSinCos, cFstSinCos, cAtan2 and cSqRt are their C entries, and
; INSTANCE.ASM calls sincos and the square root directly.
;
; Units: an angle is a word, 10000h to the full turn (4000h = 90 degrees); a sine or cosine is
; 1.15 (7FFFh = 1.0).
; Tables (seg063, UW2's seg063): 04E0 the sine at each 1/256 turn (257 words, so 0560, 64 entries
; on, is the cosine); 0762 the arcsine and 0864 the arccosine of 0 .. 1.0 in 128 steps, as
; angles. UW1's tables sit 0D0h lower than UW2's (05B0, 0630, 0832, 0934).
;
; name: descriptive (module 14 of seg019, "integer square roots, sines, arctangents").
;
; seg019 split into its original modules, as UW2's seg021: module 14 of 17, +A30..+C00
; of the segment (file 22FD0h), a module of the second library (UWLIB.LIB).
; Procs: SquareRoot_seg019_A30, sincos, seg019_A38, seg019_A69,
; SquareRoot_seg019_A78, seg019_B78, seg019_BA2, MaybeTangent_seg019_BD4.
; Evidence for the bounds: starts on a paragraph after 4 zero bytes following a retf: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG019_TEXT segment para public 'CODE'
        assume  cs:SEG019_TEXT, ds:DGROUP, ss:DGROUP
        public  _MaybeTangent_seg019_BD4
        public  _seg019_A38
        public  _seg019_A69
        public  _seg019_B78
        public  _seg019_BA2
        public  _sincos
        public  _SquareRoot_seg019_A30
        public  _SquareRoot_seg019_A78

; SquareRoot_seg019_A30  (+A30)
; _A30: far entry to _A78 (square root).
_SquareRoot_seg019_A30 proc far
        call    _SquareRoot_seg019_A78
        retf
_SquareRoot_seg019_A30 endp

; sincos  (+A34)
; sincos (FM Towns name): far entry to _A38.
_sincos proc far
        call    _seg019_A38
        retf
_sincos endp

; HeadingLookupCalc_seg019_A38  (+A38)
; _A38 (sincos): BX = an angle. Out: AX = its sine, BX = its cosine, each interpolated linearly
; between the table's two entries by the angle's low byte. Changes CX, DX, BP.
_seg019_A38 proc near
        mov     cx,bx
        xor     ch,ch
        mov     bl,bh
        xor     bh,bh
        shl     bx,1
        mov     bp,word ptr [bx+4E0h]
        mov     ax,word ptr [bx+4E2h]
        sub     ax,bp
        imul    cx
        mov     al,ah
        mov     ah,dl
        add     ax,bp
        push    ax
        mov     bp,word ptr [bx+560h]
        mov     ax,word ptr [bx+562h]
        sub     ax,bp
        imul    cx
        mov     bl,ah
        mov     bh,dl
        add     bx,bp
        pop     ax
        ret
_seg019_A38 endp

; LookupHeadingTable_seg019_A69  (+A69)
; _A69 (fast_sincos, FM Towns): as _A38 without interpolation, the entry for the angle's high
; byte.
_seg019_A69 proc near
        mov     bl,bh
        xor     bh,bh
        shl     bx,1
        mov     ax,word ptr [bx+4E0h]
        mov     bx,word ptr [bx+560h]
        ret
_seg019_A69 endp

; SquareRoot_seg019_A78  (+A78)
; _A78 (lsqrt, FM Towns): DI = the square root of the 32-bit value CX:BX, by five rounds of
; Newton's method from a starting guess chosen by the value's size (CH, CL, BH or BL the highest
; non-zero byte). Changes AX, CX, DX.
;
; The unlabelled code after its last ret is fast_asin (FM Towns), probably: CX = the arcsine of
; AX (1.15) from the table's entry for |AX|'s high byte, without interpolation, with AX's sign.
_SquareRoot_seg019_A78 proc near
        or      ch,ch
        je      short L0AB9
        mov     di,4000h
        cmp     cx,di
        jb      short L0A86
        mov     di,0FFFFh
L0A86:
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        ret
L0AB9:
        or      cl,cl
        je      short L0AF3
        mov     di,400h
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        mov     dx,cx
        mov     ax,bx
        div     di
        add     di,ax
        rcr     di,1
        ret
L0AF3:
        or      bh,bh
        je      short L0B2F
        mov     cx,40h
        mov     ax,bx
        xor     dx,dx
        div     cx
        add     cx,ax
        rcr     cx,1
        mov     ax,bx
        xor     dx,dx
        div     cx
        add     cx,ax
        rcr     cx,1
        mov     ax,bx
        xor     dx,dx
        div     cx
        add     cx,ax
        rcr     cx,1
        mov     ax,bx
        xor     dx,dx
        div     cx
        add     cx,ax
        rcr     cx,1
        mov     ax,bx
        xor     dx,dx
        div     cx
        add     cx,ax
        rcr     cx,1
        mov     di,cx
        ret
L0B2F:
        or      bl,bl
        je      short L0B61
        mov     cx,4
        mov     ax,bx
        div     cl
        add     cl,al
        rcr     cl,1
        mov     ax,bx
        div     cl
        add     cl,al
        rcr     cl,1
        mov     ax,bx
        div     cl
        add     cl,al
        rcr     cl,1
        mov     ax,bx
        div     cl
        add     cl,al
        rcr     cl,1
        mov     ax,bx
        div     cl
        add     cl,al
        rcr     cl,1
        mov     di,cx
        ret
L0B61:
        xor     di,di
        ret
        cwd
        xor     ax,dx
        sub     ax,dx
        mov     bl,ah
        xor     bh,bh
        shl     bx,1
        mov     cx,word ptr [bx+762h]
        xor     cx,dx
        sub     cx,dx
        ret
_SquareRoot_seg019_A78 endp

; seg019_B78  (+B78)
; _B78: CX = the arcsine of AX (1.15), interpolated, with AX's sign.
_seg019_B78 proc near
        cwd
        xor     ax,dx
        sub     ax,dx
        push    dx
        mov     bl,ah
        xor     bh,bh
        mov     cx,ax
        xor     ch,ch
        shl     bx,1
        mov     bp,word ptr [bx+762h]
        mov     ax,word ptr [bx+764h]
        sub     ax,bp
        imul    cx
        mov     al,ah
        mov     ah,dl
        add     ax,bp
        mov     cx,ax
        pop     dx
        xor     cx,dx
        sub     cx,dx
        ret
_seg019_B78 endp

; seg019_BA2  (+BA2)
; _BA2: CX = the arccosine of BX (1.15), interpolated; for a negative BX the result is moved
; into the other half of the circle (by adding 8000h to the negated value).
_seg019_BA2 proc near
        mov     ax,bx
        cwd
        xor     ax,dx
        sub     ax,dx
        push    dx
        mov     cx,ax
        xor     ch,ch
        mov     bl,ah
        xor     bh,bh
        shl     bx,1
        mov     bp,word ptr [bx+864h]
        mov     ax,word ptr [bx+866h]
        sub     ax,bp
        imul    cx
        mov     al,ah
        mov     ah,dl
        add     ax,bp
        mov     cx,ax
        pop     dx
        xor     cx,dx
        sub     cx,dx
        and     dx,8000h
        add     cx,dx
        ret
_seg019_BA2 endp

; MaybeTangent_seg019_BD4  (+BD4)
; _BD4 (atan2, FM Towns, through cAtan2): CX = the angle of the unit vector (sine AX, cosine
; BX). Where the sine is small (|AX| up to sin 45 degrees, 5A82h) it uses the arcsine of AX,
; reflected when BX is negative; otherwise the arccosine of BX with AX's sign. Both are well
; conditioned in their range.
_MaybeTangent_seg019_BD4 proc near
        cmp     ax,5A82h
        jg      short L0BEE
        cmp     ax,0A57Eh
        jl      short L0BEE
        push    bx
        call    _seg019_B78
        pop     ax
        or      ax,ax
        jns     short L0BED
        sub     cx,8000h
        neg     cx
L0BED:
        ret
L0BEE:
        cwd
        push    dx
        call    _seg019_BA2
        pop     dx
        xor     cx,dx
        sub     cx,dx
        ret
_MaybeTangent_seg019_BD4 endp

SEG019_TEXT ends

        end
