; /* target: seg003_6134 */
; /* opts: /ml */
;
; Graphics library module 17: the Gouraud-shaded (smooth) polygon, FM Towns' spolygon and
; supolygon. The vertices are 6 bytes each (x, y, intensity) at 415C. The polygon is clipped by
; GRLIBN.ASM's s_shclip, which interpolates the intensity along with the position, then every
; edge is stepped down its rows (16.16 for both x and intensity) into the span table at 5690 (10
; bytes a row: y, left x, right x, left intensity, right intensity; GRDISP.ASM defines it with
; the y words filled in), keeping per row the leftmost and rightmost x with their intensities.
; The table is handed to GRENTRY.ASM's _F77 (FM Towns vga_smooth_ylrii), which draws the shaded
; spans. The 3D renderer's Gouraud faces reach it through SMOOTH.ASM.
;
; Entries (labels; GRCORE's jump table and seg004 use them):
;   _6146  spolygon: CX = the vertex count (more than 99 is refused; one or two vertices
;          become a point or a line, L6134), clip with s_shclip, then fill
;   _6156  attach the intensities from 55EC to plain (x, y) vertices first (_6547), then fill
;   _6159  supolygon: fill a polygon already inside the window
; Data (seg048): 566C, 566E the lowest and highest rows; 5670..5684 the current edge's stepping
; state; 567E the edges left. Negative y is clamped to 0 before filling.
;
; name: descriptive (UW2Decomp: module 13 of seg003); its job, found here, is the smooth polygon.
; Its
; entries are labels, made rows of the target table.
;
; UW1 against UW2: UW2Decomp's src/gfx/GRLIBM.ASM (UW2's seg003_0272_5934), the same code and
; length; the data sits 2 bytes lower (415C for UW2's 415E, 5690 for 5692).
;
; SEG003 split into its original modules: this is module 17 of UW1's 18, +6134..+634F.
; Evidence for the bounds: starts after one zero byte following a ret (TLINK's padding before a
; word-aligned module), as in UW2.
; It has no relocations.

; The graphics library's data in seg048 (DS while the library runs), as GRLIBF.ASM's
; and VIDMODE.ASM's headers describe it. The names are ours.
VERTS           equ     415Ch   ; the polygon vertices; and the second buffer the clippers

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg003_3B34:near
        extrn   _seg003_3F06:near
        extrn   _seg003_6350:near
        extrn   _seg003_6547:near
        extrn   _seg003_F77:near
        public  _seg003_6146
        public  _seg003_6156
        public  _seg003_6159

L6134:
        jne     L613C
        mov     si,415Ch
        jmp     _seg003_3F06
L613C:
        mov     ax,word ptr ds:[VERTS]
        mov     bx,word ptr ds:[415Eh]
        jmp     _seg003_3B34
_seg003_6146:
        cmp     cx,63h
        ja      L6155
        cmp     cx,2
        jbe     L6134
        call    _seg003_6350
        jmp     short _seg003_6159
L6155:
        ret
_seg003_6156:
        call    _seg003_6547
_seg003_6159:
        db      0E9h                    ; match: a near jmp to the next instruction, which TASM never makes
        dw      L615C-$-2
; Fill: close the vertex list by copying the first vertex after the last, find the top and
; bottom, initialise every row record to an empty span (left 3E8h, right 0FC18h), then step each
; edge (horizontal edges are handled at L630A), and finally mark the end of the table (top bit
; of the row after the last) and call _F77.
L615C:
        mov     di,cx
        shl     di,1
        add     di,cx
        shl     di,1
        mov     si,415Ch
        add     di,si
        movsw
        movsw
        movsw
        mov     word ptr ds:[567Eh],cx
        mov     bx,0FC18h
        mov     dx,3E8h
        mov     si,415Eh
L6179:
        test    word ptr [si],0FFFFh
        jns     L6183
        mov     word ptr [si],0
L6183:
        lodsw
        add     si,4
        cmp     ax,bx
        jl      L6193
        mov     bx,ax
        mov     bp,word ptr [si-8]
        mov     di,word ptr [si-4]
L6193:
        cmp     ax,dx
        jg      L6199
        mov     dx,ax
L6199:
        loop    L6179
        mov     word ptr ds:[566Eh],bx
        mov     word ptr ds:[566Ch],dx
        mov     ax,bx
        mov     si,bx
        sub     si,word ptr ds:[566Ch]
        shl     si,1
        shl     si,1
        add     si,bx
        sub     si,word ptr ds:[566Ch]
        shl     si,1
        add     si,5690h
        mov     word ptr [si],ax
        mov     word ptr [si+2],bp
        mov     word ptr [si+4],bp
        mov     word ptr [si+6],di
        mov     word ptr [si+8],di
        lea     di,[si-8]
        mov     cx,bx
        sub     cx,dx
        inc     cx
        mov     ax,3E8h
        mov     bx,0FC18h
L61D7:
        mov     word ptr [di],ax
        mov     word ptr [di+2],bx
        sub     di,0Ah
        loop    L61D7
        mov     si,415Ch
L61E4:
        lodsw
        xchg    di,ax
        lodsw
        add     si,2
        push    si
        mov     bp,word ptr [si]
        mov     word ptr ds:[5670h],bp
        mov     dx,word ptr [si+2]
        sub     di,bp
        mov     bp,word ptr [si-2]
        mov     bx,word ptr [si+4]
        mov     word ptr ds:[5684h],bx
        sub     bp,bx
        sub     ax,dx
        jne     L6209
        jmp     L630A
L6209:
        js      L6222
        neg     bp
        neg     ax
        neg     di
        mov     cx,word ptr [si-6]
        mov     word ptr ds:[5670h],cx
        mov     cx,word ptr [si-2]
        mov     word ptr ds:[5684h],cx
        mov     dx,word ptr [si-4]
L6222:
        mov     word ptr ds:[5674h],dx
        neg     ax
        xchg    di,ax
        cwd
        idiv    di
        mov     word ptr ds:[5678h],ax
        sub     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[5676h],ax
        add     word ptr ds:[5678h],dx
        mov     ax,bp
        cwd
        idiv    di
        mov     word ptr ds:[567Ch],ax
        sub     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[567Ah],ax
        add     word ptr ds:[567Ch],dx
        mov     word ptr ds:[5672h],8000h
        mov     word ptr ds:[5682h],8000h
        mov     word ptr ds:[5680h],di
        mov     bx,word ptr ds:[5674h]
        mov     di,bx
        sub     di,word ptr ds:[566Ch]
        shl     di,1
        shl     di,1
        add     di,bx
        sub     di,word ptr ds:[566Ch]
        shl     di,1
        add     di,5690h
        dec     bx
        sub     di,0Ah
        push    cx
        mov     dx,word ptr ds:[5678h]
        mov     bp,word ptr ds:[5672h]
        mov     ax,word ptr ds:[5670h]
        mov     si,word ptr ds:[5682h]
        mov     cx,word ptr ds:[5684h]
L62A1:
        mov     word ptr [di],bx
        dec     bx
        add     bp,word ptr ds:[5676h]
        adc     ax,dx
        add     si,word ptr ds:[567Ah]
        adc     cx,word ptr ds:[567Ch]
        cmp     ax,word ptr [di+2]
        jge     L62BD
        mov     word ptr [di+2],ax
        mov     word ptr [di+6],cx
L62BD:
        cmp     ax,word ptr [di+4]
        jle     L62C8
        mov     word ptr [di+4],ax
        mov     word ptr [di+8],cx
L62C8:
        sub     di,0Ah
        dec     word ptr ds:[5680h]
        jne     L62A1
        pop     cx
L62D2:
        pop     si
        dec     word ptr ds:[567Eh]
        je      L62DC
        jmp     L61E4
L62DC:
        mov     di,word ptr ds:[566Eh]
        sub     di,word ptr ds:[566Ch]
        shl     di,1
        shl     di,1
        add     di,word ptr ds:[566Eh]
        sub     di,word ptr ds:[566Ch]
        shl     di,1
        add     di,5690h
        add     di,0Ah
        or      byte ptr [di+1],80h
        mov     si,5690h
        push    di
        call    _seg003_F77
        pop     di
        and     byte ptr [di+1],7Fh
        ret
L630A:
        mov     di,dx
        sub     di,word ptr ds:[566Ch]
        shl     di,1
        shl     di,1
        add     di,dx
        sub     di,word ptr ds:[566Ch]
        shl     di,1
        add     di,5690h
        mov     word ptr [di],dx
        mov     ax,word ptr [si-6]
        mov     cx,word ptr [si]
        mov     bx,word ptr [si-2]
        mov     dx,word ptr [si+4]
        cmp     ax,cx
        jle     L6334
        xchg    cx,ax
        xchg    bx,dx
L6334:
        cmp     ax,word ptr [di+2]
        jg      L633F
        mov     word ptr [di+2],ax
        mov     word ptr [di+6],bx
L633F:
        xchg    cx,ax
        xchg    bx,dx
        cmp     ax,word ptr [di+4]
        jl      L634D
        mov     word ptr [di+4],ax
        mov     word ptr [di+8],bx
L634D:
        jmp     L62D2

SEG003_TEXT ends

        end
