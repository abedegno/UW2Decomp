; /* target: seg003_3F06 */
; /* opts: /ml */
;
; Graphics library module 10: cline, a line clipped to the window (FM Towns cline; _3F06 is
; cline_si, which loads the two ends from four words at SI first). It handles vertical and
; horizontal lines through GRLIBF's vline and hline, clips the rest Cohen-Sutherland style
; against the window (3DF2 left, 3DF6 right, 3DF8 bottom, 3DF4 top; y counts up), and draws what
; is left with uline (GRLIBH.ASM). GRMISC's far thunk _10 enters it for the 3D view's lines, and
; GRCORE's _56A9 for C.
;
; In: (AX, BX) and (CX, DX), x and y of the two ends. DS = ES = the library's data segment.
; Data: 44A0 dx, 44A2 dy; 44A4 the table of the four edge handlers (the code after the jmp
; word ptr [bx+44A4h]: left, right, bottom, top); 44C4 the other end's out code.
;
; name: descriptive (UW2Decomp: module 7 of seg003); the routine is FM Towns' cline, which with
; its _si
; entry is the whole module.
;
; UW1 against UW2: UW2Decomp's src/gfx/GRLIBG.ASM (UW2's seg003_0272_3686), the same code and
; length; the library's data sits 2 bytes lower in UW1 (3DF2 for UW2's 3DF4, 44A0
; for 44A2). It ends with a jmp to _3FDC in E9 form, which TASM would shorten to EB within a
; module: _3FDC is in the next module.
;
; SEG003 split into its original modules: this is module 10 of UW1's 18, +3F06..+3FDB.
; Evidence for the bounds: starts after one zero byte following a jmp (TLINK's padding before a
; word-aligned module), as in UW2.
; It has no relocations.

; The graphics library's data in seg048 (DS while the library runs), as GRLIBF.ASM's
; and VIDMODE.ASM's headers describe it. The names are ours.
WIN_LEFT        equ     3DF2h   ; the clip window (y counts up, so bottom < top)
WIN_TOP         equ     3DF4h
WIN_RIGHT       equ     3DF6h
WIN_BOTTOM      equ     3DF8h

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg003_3BA1:near
        extrn   _seg003_3CFF:near
        extrn   _seg003_3FDC:near
        public  _seg003_3F06
        public  _seg003_3F0E

_seg003_3F06:
        lodsw
        xchg    dx,ax
        lodsw
        xchg    bx,ax
        lodsw
        xchg    cx,ax
        lodsw
        xchg    dx,ax

; seg003_3F0E  (+3F0E)
; cline. The out codes are 2 left, 4 right, 8 below, 10h above. Both ends inside: jump to uline.
; Both outside on the same side: return. Otherwise move the outside end onto the edge its lowest
; set bit names (the slope from dx and dy by imul/idiv) and test again.
_seg003_3F0E proc near
        mov     si,cx
        sub     si,ax
        jne     L3F17
        jmp     _seg003_3BA1
L3F17:
        mov     word ptr ds:[44A0h],si
        mov     si,dx
        sub     si,bx
        jne     L3F24
        jmp     _seg003_3CFF
L3F24:
        mov     word ptr ds:[44A2h],si
        mov     di,ax
        mov     si,bx
        mov     bp,dx
        mov     bh,0
        cmp     cx,word ptr ds:[WIN_LEFT]
        jge     L3F39
        or      bh,2
L3F39:
        cmp     cx,word ptr ds:[WIN_RIGHT]
        jle     L3F42
        or      bh,4
L3F42:
        cmp     bp,word ptr ds:[WIN_BOTTOM]
        jge     L3F4B
        or      bh,8
L3F4B:
        cmp     bp,word ptr ds:[WIN_TOP]
        jle     L3F54
        or      bh,10h
L3F54:
        mov     al,0
        cmp     di,word ptr ds:[WIN_LEFT]
        jge     L3F5E
        or      al,2
L3F5E:
        cmp     di,word ptr ds:[WIN_RIGHT]
        jle     L3F66
        or      al,4
L3F66:
        cmp     si,word ptr ds:[WIN_BOTTOM]
        jge     L3F6E
        or      al,8
L3F6E:
        cmp     si,word ptr ds:[WIN_TOP]
        jle     L3F76
        or      al,10h
L3F76:
        mov     bl,al
        or      al,bh
        jne     L3F86
        mov     ax,di
        mov     bx,si
        mov     dx,bp
        jmp     _seg003_3FDC       ; E9: the target is in another module (GRLIBH)
L3F85:
        ret
L3F86:
        test    bl,bh
        jne     L3F85
        test    bl,bl
        jne     L3F94
        xchg    di,cx
        xchg    si,bp
        xchg    bl,bh
L3F94:
        mov     byte ptr ds:[44C4h],bh
        mov     bh,0
        jmp     word ptr [bx+44A4h]
        mov     ax,word ptr ds:[WIN_LEFT]
        jmp     short L3FA6
        mov     ax,word ptr ds:[WIN_RIGHT]
L3FA6:
        mov     bx,ax
        sub     ax,di
        imul    word ptr ds:[44A2h]
        idiv    word ptr ds:[44A0h]
        mov     di,bx
        add     si,ax
        mov     bh,byte ptr ds:[44C4h]
        jmp     L3F54
        mov     ax,word ptr ds:[WIN_BOTTOM]
        jmp     short L3FC4
        mov     ax,word ptr ds:[WIN_TOP]
L3FC4:
        mov     bx,ax
        sub     ax,si
        imul    word ptr ds:[44A0h]
        idiv    word ptr ds:[44A2h]
        add     di,ax
        mov     si,bx
        mov     bh,byte ptr ds:[44C4h]
        jmp     L3F54
_seg003_3F0E endp

SEG003_TEXT ends

        end
