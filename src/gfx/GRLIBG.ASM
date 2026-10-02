; /* target: seg003_0272_3686 */
; /* opts: /ml */
;
; Graphics library module 7: cline, a line clipped to the window (FM Towns cline; _3686 is
; cline_si, which loads the two ends from four words at SI first). It handles vertical and
; horizontal lines through GRLIBF's vline and hline, clips the rest Cohen-Sutherland style
; against the window (3DF4 left, 3DF8 right, 3DFA bottom, 3DF6 top; y counts up), and draws what
; is left with uline (GRLIBH.ASM). GRMISC's far thunk _10 enters it for the 3D view's lines, and
; GRCORE's _4EB0 for C.
;
; In: (AX, BX) and (CX, DX), x and y of the two ends. DS = ES = seg_370D.
; Data: 44A2 dx, 44A4 dy; 44A6 the table of the four edge handlers (the code after the jmp
; word ptr [bx+44A6h]: left, right, bottom, top); 44C6 the other end's out code.
;
; name: descriptive (module 7 of seg003); the routine is FM Towns' cline, which with its _si
; entry is the whole module.
;
; SEG003 split into its original modules: this is module 7 of 14, +3686..+375C of the segment
; (file 6AD6h), a module of the second library (UWLIB.LIB).
; Procs: seg003_0272_368E.
; Evidence for the bounds: starts after one zero byte following a jmp: TLINK's padding before
; a word-aligned module (TASM 2.0's `even` pads with 90h). It ends with a jmp to _375C in E9
; form, which TASM would shorten to EB within a module: _375C is in the next module.
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'CODE'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg003_0272_3321:near
        extrn   _seg003_0272_347F:near
        extrn   _seg003_0272_375C:near
        public  _seg003_0272_3686
        public  _seg003_0272_368E

_seg003_0272_3686:
        lodsw
        xchg    dx,ax
        lodsw
        xchg    bx,ax
        lodsw
        xchg    cx,ax
        lodsw
        xchg    dx,ax

; seg003_0272_368E  (+368E)
; cline. The out codes are 2 left, 4 right, 8 below, 10h above. Both ends inside: jump to uline.
; Both outside on the same side: return. Otherwise move the outside end onto the edge its lowest
; set bit names (the slope from dx and dy by imul/idiv) and test again.
_seg003_0272_368E proc near
        mov     si,cx
        sub     si,ax
        jne     L3697
        jmp     _seg003_0272_3321
L3697:
        mov     word ptr ds:[44A2h],si
        mov     si,dx
        sub     si,bx
        jne     L36A4
        jmp     _seg003_0272_347F
L36A4:
        mov     word ptr ds:[44A4h],si
        mov     di,ax
        mov     si,bx
        mov     bp,dx
        mov     bh,0
        cmp     cx,word ptr ds:[3DF4h]
        jge     L36B9
        or      bh,2
L36B9:
        cmp     cx,word ptr ds:[3DF8h]
        jle     L36C2
        or      bh,4
L36C2:
        cmp     bp,word ptr ds:[3DFAh]
        jge     L36CB
        or      bh,8
L36CB:
        cmp     bp,word ptr ds:[3DF6h]
        jle     L36D4
        or      bh,10h
L36D4:
        mov     al,0
        cmp     di,word ptr ds:[3DF4h]
        jge     L36DE
        or      al,2
L36DE:
        cmp     di,word ptr ds:[3DF8h]
        jle     L36E6
        or      al,4
L36E6:
        cmp     si,word ptr ds:[3DFAh]
        jge     L36EE
        or      al,8
L36EE:
        cmp     si,word ptr ds:[3DF6h]
        jle     L36F6
        or      al,10h
L36F6:
        mov     bl,al
        or      al,bh
        jne     L3706
        mov     ax,di
        mov     bx,si
        mov     dx,bp
        jmp     _seg003_0272_375C       ; E9: the target is in another module (GRLIBH)
L3705:
        ret
L3706:
        test    bl,bh
        jne     L3705
        test    bl,bl
        jne     L3714
        xchg    di,cx
        xchg    si,bp
        xchg    bl,bh
L3714:
        mov     byte ptr ds:[44C6h],bh
        mov     bh,0
        jmp     word ptr [bx+44A6h]
        mov     ax,word ptr ds:[3DF4h]
        jmp     short L3726
        mov     ax,word ptr ds:[3DF8h]
L3726:
        mov     bx,ax
        sub     ax,di
        imul    word ptr ds:[44A4h]
        idiv    word ptr ds:[44A2h]
        mov     di,bx
        add     si,ax
        mov     bh,byte ptr ds:[44C6h]
        jmp     L36D4
        mov     ax,word ptr ds:[3DFAh]
        jmp     short L3744
        mov     ax,word ptr ds:[3DF6h]
L3744:
        mov     bx,ax
        sub     ax,si
        imul    word ptr ds:[44A2h]
        idiv    word ptr ds:[44A4h]
        add     di,ax
        mov     si,bx
        mov     bh,byte ptr ds:[44C6h]
        jmp     L36D4
_seg003_0272_368E endp

SEG003_TEXT ends

        end
