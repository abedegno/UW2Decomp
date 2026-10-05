; /* target: seg003_3FDC */
; /* opts: /ml */
;
; Graphics library module 11: uline, an unclipped line (FM Towns uline). A line is drawn as a
; list of horizontal spans, one per row: the x at each row is stepped in 16.16 fixed point and
; each row's span runs from that x to the next row's, so steep and shallow lines both come out
; solid. The spans go into the row table at 44CC (as urectangle's do, GRLIBF.ASM) and the
; current span writer (DS:4110) draws them. GRMISC's far thunk _8 enters it for the 3D view's
; lines that need no clipping, and GRLIBG's cline jumps to it after clipping.
;
; In: (AX, BX) and (CX, DX), the ends. DS = ES = the library's data segment.
; Data: 49AA the end mark in the row table; 49AE which end of each record is the left (0 or 2,
; for lines leaning either way); 49B0 the row count; 49B2 the end x; 49B4 the first record;
; 4988 the entry table into the unrolled stepper _409C, by the remaining count.
;
; name: descriptive (UW2Decomp: module 8 of seg003); its routine is FM Towns' uline.
;
; UW1 against UW2: UW2Decomp's src/gfx/GRLIBH.ASM (UW2's seg003_0272_375C), the same code and
; length; the library's data sits 2 bytes lower in UW1 (44CC for UW2's 44CE, 49AA for
; 49AC).
;
; SEG003 split into its original modules: this is module 11 of UW1's 18, +3FDC..+4133.
; Evidence for the bounds: starts after one zero byte following a jmp (TLINK's padding before a
; word-aligned module), as in UW2.
; It has no relocations.

; The graphics library's data in seg048 (DS while the library runs), as GRLIBF.ASM's
; and VIDMODE.ASM's headers describe it. The names are ours.
SPAN_WRITER     equ     4110h   ; the current span writer (set_the_color, fbuf_setcolor)

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        public  _seg003_3FDC
        public  _seg003_403E
        public  _seg003_409C

; seg003_3FDC  (+3FDC)
; uline: order the ends so BX is the upper (y counts up), pick which side of each record the
; stepped x goes to by the line's direction, fill the records (_403E), close the list and jump
; to the span writer.
_seg003_3FDC proc near
        mov     si,word ptr ds:[49AAh]
        and     word ptr [si],7FFFh
        cmp     bx,dx
        jge     L3FEB
        xchg    cx,ax
        xchg    bx,dx
L3FEB:
        dec     dx
        cmp     ax,cx
        jle     L4004
        mov     word ptr ds:[49AEh],2
        mov     di,44D0h
        mov     bp,0FFF6h
        call    _seg003_403E
        add     di,-8
        jmp     short L4016
L4004:
        mov     word ptr ds:[49AEh],0
        mov     di,44CEh
        mov     bp,0FFFAh
        call    _seg003_403E
        add     di,-4
L4016:
        mov     ax,word ptr ds:[49B2h]
        stosw
        add     di,word ptr ds:[49AEh]
        mov     word ptr ds:[49AAh],di
        or      word ptr [di],8000h
        mov     si,word ptr ds:[49B4h]
        sub     si,2
        sub     si,word ptr ds:[49AEh]
        jmp     word ptr ds:[SPAN_WRITER]
L4035:
        sub     bx,bx
        sub     cx,cx
        sub     dx,dx
        jmp     short L4080
L403D:
        retn
_seg003_3FDC endp

; seg003_403E  (+403E)
; _403E: fill the span records for rows BX down to DX (DI = the first record, BP = the offset
; from one record's stepped x to the other x). The slope (CX - AX) / rows is formed as a 16.16
; value in BX:CX by two divides; a vertical line takes slope 0. Rows are filled fifteen at a
; time by _409C, then the remainder by jumping into it through 4988.
_seg003_403E proc near
        mov     word ptr ds:[49B2h],cx
        mov     si,bx
        neg     si
        add     si,0C7h
        shl     si,1
        add     di,si
        shl     si,1
        add     di,si
        mov     word ptr ds:[49B4h],di
        sub     bx,dx
        mov     word ptr ds:[49B0h],bx
        mov     si,bx
        jle     L403D
        stosw
        add     di,4
        sub     cx,ax
        je      L4035
        xchg    cx,ax
        cwd
        idiv    si
        xchg    bx,ax
        sub     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    si
        cwd
        shl     ax,1
        rcl     dx,1
        add     bx,dx
        xchg    cx,ax
        mov     dx,7FFFh
L4080:
        cmp     si,10h
        ja      L408B
        shl     si,1
        jmp     word ptr [si+4988h]
L408B:
        call    _seg003_409C
        sub     si,0Fh
        cmp     si,10h
        ja      L408B
        shl     si,1
        jmp     word ptr [si+4988h]
_seg003_403E endp

; seg003_409C  (+409C)
; _409C: fifteen rows of the line, unrolled: add the slope to the 16.16 x (AX:DX), store it as
; this record's x and the previous record's other x.
_seg003_409C proc near
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        mov     word ptr [bp+di],ax
        add     di,4
        ret
_seg003_409C endp

SEG003_TEXT ends

        end
