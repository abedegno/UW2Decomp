; /* target: seg003_0272_375C */
; /* opts: /ml */
;
; Graphics library module 8: uline, an unclipped line (FM Towns uline). A line is drawn as a
; list of horizontal spans, one per row: the x at each row is stepped in 16.16 fixed point and
; each row's span runs from that x to the next row's, so steep and shallow lines both come out
; solid. The spans go into the row table at 44CE (as urectangle's do, GRLIBF.ASM) and the
; current span writer (DS:4112) draws them. GRMISC's far thunk _8 enters it for the 3D view's
; lines that need no clipping, and GRLIBG's cline jumps to it after clipping.
;
; In: (AX, BX) and (CX, DX), the ends. DS = ES = seg_370D.
; Data: 49AC the end mark in the row table; 49B0 which end of each record is the left (0 or 2,
; for lines leaning either way); 49B2 the row count; 49B4 the end x; 49B6 the first record;
; 498A the entry table into the unrolled stepper _381C, by the remaining count.
;
; name: descriptive (module 8 of seg003); its routine is FM Towns' uline.
;
; SEG003 split into its original modules: this is module 8 of 14, +375C..+38B4 of the segment
; (file 6BACh), a module of the second library (UWLIB.LIB).
; Procs: seg003_0272_375C, seg003_0272_37BE, seg003_0272_381C.
; Evidence for the bounds: starts after one zero byte following a jmp: TLINK's padding before
; a word-aligned module (TASM 2.0's `even` pads with 90h).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        public  _seg003_0272_375C
        public  _seg003_0272_37BE
        public  _seg003_0272_381C

; seg003_0272_375C  (+375C)
; uline: order the ends so BX is the upper (y counts up), pick which side of each record the
; stepped x goes to by the line's direction, fill the records (_37BE), close the list and jump
; to the span writer.
_seg003_0272_375C proc near
        mov     si,word ptr ds:[49ACh]
        and     word ptr [si],7FFFh
        cmp     bx,dx
        jge     L376B
        xchg    cx,ax
        xchg    bx,dx
L376B:
        dec     dx
        cmp     ax,cx
        jle     L3784
        mov     word ptr ds:[49B0h],2
        mov     di,44D2h
        mov     bp,0FFF6h
        call    _seg003_0272_37BE
        add     di,-8
        jmp     short L3796
L3784:
        mov     word ptr ds:[49B0h],0
        mov     di,44D0h
        mov     bp,0FFFAh
        call    _seg003_0272_37BE
        add     di,-4
L3796:
        mov     ax,word ptr ds:[49B4h]
        stosw
        add     di,word ptr ds:[49B0h]
        mov     word ptr ds:[49ACh],di
        or      word ptr [di],8000h
        mov     si,word ptr ds:[49B6h]
        sub     si,2
        sub     si,word ptr ds:[49B0h]
        jmp     word ptr ds:[4112h]
L37B5:
        sub     bx,bx
        sub     cx,cx
        sub     dx,dx
        jmp     short L3800
L37BD:
        retn
_seg003_0272_375C endp

; seg003_0272_37BE  (+37BE)
; _37BE: fill the span records for rows BX down to DX (DI = the first record, BP = the offset
; from one record's stepped x to the other x). The slope (CX - AX) / rows is formed as a 16.16
; value in BX:CX by two divides; a vertical line takes slope 0. Rows are filled fifteen at a
; time by _381C, then the remainder by jumping into it through 498A.
_seg003_0272_37BE proc near
        mov     word ptr ds:[49B4h],cx
        mov     si,bx
        neg     si
        add     si,0C7h
        shl     si,1
        add     di,si
        shl     si,1
        add     di,si
        mov     word ptr ds:[49B6h],di
        sub     bx,dx
        mov     word ptr ds:[49B2h],bx
        mov     si,bx
        jle     L37BD
        stosw
        add     di,4
        sub     cx,ax
        je      L37B5
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
L3800:
        cmp     si,10h
        ja      L380B
        shl     si,1
        jmp     word ptr [si+498Ah]
L380B:
        call    _seg003_0272_381C
        sub     si,0Fh
        cmp     si,10h
        ja      L380B
        shl     si,1
        jmp     word ptr [si+498Ah]
_seg003_0272_37BE endp

; seg003_0272_381C  (+381C)
; _381C: fifteen rows of the line, unrolled: add the slope to the 16.16 x (AX:DX), store it as
; this record's x and the previous record's other x.
_seg003_0272_381C proc near
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
_seg003_0272_381C endp

SEG003_TEXT ends

        end
