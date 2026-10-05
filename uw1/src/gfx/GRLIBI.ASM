; /* target: seg003_4134 */
; /* opts: /ml */
;
; Graphics library module 12: filled polygons and text. Found by reading the code and aligning
; with FM Towns, whose 2D library has the same pieces (upolygon, polygon; string_to_screen,
; ustring_to_screen, shadowed_string_to_screen, setup_font, vga_bitmap_to_screen, shift_left_1,
; string_width), in a different order.
;
; Polygons. polygon (_438D) clips with GRLIBF's shclip and fills with upolygon (_4134), which
; walks the left and right chains of edges from the top vertex down, writes one span per row
; into the row table at 44CC and runs the current span writer (DS:4110), as GRLIBH's uline does
; for lines.
;
; Text. A string is drawn in two steps. The rasteriser at 4D12 (statically the unlabelled
; routine after _4A06, 4AA5h) renders the whole string into a one-bit-per-pixel buffer at 2D41,
; glyph by glyph from the font, already shifted for x mod 4. Then a blitter (4D14) puts the
; buffer on the screen: _44E1 writes it straight to the planar (mode X) screen, setting the
; sequencer map mask (port 3C5h) from a table that turns each buffer byte into two four-pixel
; plane masks (4A24, built by _4462) and storing the colour, so only the set pixels are written;
; L49A6 instead passes the buffer to VIDMODE's bitmap routine _2C11.
;
; Data (the library's data segment, UW2's seg048):
;   4A0C, 4A0E  the text's x and y (y is its top row; y counts up); 4A0A the colour for _44E1
;   4CFE..4D04  the bit buffer's size and pointers; 4D02 its width in bytes
;   4D06..4D11  the font's header (gfx.h struct FontInfo, which GRFX.C's grfx_load_font
;        reads here through cur_font): 4D06 widthsize, the bytes each glyph has after its
;        rows; 4D0C height; 4D0E rowwidth; 4D10 maxwidth, copied to 2D3E; 4D16 the glyph
;        pointer of each character 0..127 (setup_font), into the glyphs at 1C56h (bytefont)
;   2D34, 2D38  the shadow and text colours; 2D3A..2D3E the font fields string_width reads
;   4C8C, 4C22  entry tables into the unrolled blit and shift loops, by width
;   3DFA  the screen segment; 36AA the row address table (Ytab); 36A6 the row stride
;
; name: descriptive (UW2Decomp: module 9 of seg003). Its job, found here, is polygon filling and
; text; the
; routine names given here are FM Towns'.
;
; UW1 against UW2: UW2Decomp's src/gfx/GRLIBI.ASM (UW2's seg003_0272_38B4), the same code and
; length. Only numbers differ: the library's data sits 2 bytes lower in UW1 (44CC for UW2's
; 44CE, 4D12 for 4D14 ...), and the font's glyphs start at 1C56h (UW2 1C54h).
;
; SEG003 split into its original modules: this is module 12 of UW1's 18, +4134..+4C6F.
; Evidence for the bounds: starts after one zero byte following a ret (TLINK's padding before a
; word-aligned module), as in UW2; it ends at the zero byte at +4C6F.
; Its relocation is entry 2767 of UW.EXE's relocation table (+49A7).

; The VGA ports used here.
SC_DATA         equ     3C5h  ; sequencer data

; The graphics library's data in seg048 (DS while the library runs), as GRLIBF.ASM's
; and VIDMODE.ASM's headers describe it. The names are ours.
ROW_BYTES       equ     36A6h   ; bytes per screen row (width / 4)
YTAB            equ     36AAh   ; the drawing page's row addresses (Ytab; row 0 the bottom line)
WIN_LEFT        equ     3DF2h   ; the clip window (y counts up, so bottom < top)
WIN_TOP         equ     3DF4h
WIN_RIGHT       equ     3DF6h
WIN_BOTTOM      equ     3DF8h
SCREEN_SEG      equ     3DFAh   ; the screen's segment
PEN_COLOR       equ     410Fh   ; the colour
SPAN_WRITER     equ     4110h   ; the current span writer (set_the_color, fbuf_setcolor)
PEN_WORD2       equ     4114h   ; the pen's second word
VERTS           equ     415Ch   ; the polygon vertices; and the second buffer the clippers
TEXT_X          equ     4A0Ch   ; the text's x
TEXT_Y          equ     4A0Eh   ; and y (its top row)
TEXT_MASKS      equ     4A24h   ; the 256 plane-mask bytes _4462 builds for text

        .186

        extrn   seg048:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg003_2C11:near
        extrn   _seg003_3B34:near
        extrn   _seg003_3C96:near
        extrn   _seg003_3D47:near
        extrn   _seg003_3F06:near
        extrn   _seg003_5AC6:near
        extrn   _seg003_5AD2:near
        extrn   _seg003_5AD5:near
        extrn   _seg003_5AE1:near
        public  _seg003_4134
        public  _seg003_41DC
        public  _seg003_4209
        public  _seg003_423B
        public  _seg003_4282
        public  _seg003_438D
        public  _seg003_439E
        public  _seg003_43A6
        public  _seg003_43AE
        public  _seg003_43B6
        public  _seg003_43BE
        public  _seg003_440F
        public  _seg003_4448
        public  _seg003_4462
        public  _seg003_447D
        public  _seg003_44E1
        public  _seg003_49D0
        public  _seg003_4A06
        public  _seg003_4C45

; seg003_4134  (+4134)
; upolygon: fill a polygon already inside the window. In: CX = the vertex count, the vertices
; (x, y words) from 415C. Finds the top vertex, steps the two chains (_41DC going one way round,
; _4209 the other) to fill the row table, and jumps to the span writer. A polygon of zero height
; becomes one span from its least to its greatest x.
_seg003_4134 proc near
        mov     si,word ptr ds:[49AAh]
        and     word ptr [si],7FFFh
        mov     word ptr ds:[49B6h],cx
        mov     word ptr ds:[49B8h],cx
        mov     word ptr ds:[49BAh],cx
        mov     di,0FC19h
        mov     si,415Eh
        mov     bp,415Ch
        jmp     short L415C
L4153:
        mov     bp,si
        sub     bp,6
        xchg    di,ax
        dec     cx
        je      L4165
L415C:
        lodsw
        inc     si
        inc     si
        cmp     di,ax
        jl      L4153
        loop    L415C
L4165:
        sub     si,4
        mov     word ptr ds:[49C0h],si
        mov     si,bp
        mov     word ptr ds:[49BEh],si
        shl     di,1
        mov     ax,di
        shl     di,1
        add     di,ax
        neg     di
        add     di,4976h
        mov     word ptr ds:[49BCh],di
        call    _seg003_41DC
        mov     si,word ptr ds:[49BEh]
        mov     di,word ptr ds:[49BCh]
        call    _seg003_4209
        sub     di,4
        mov     bp,word ptr ds:[49BCh]
        mov     ax,word ptr ds:[4A06h]
        mov     bx,word ptr ds:[4A08h]
        cmp     bp,di
        jne     L41C4
        mov     si,415Ch
        mov     bx,270Fh
        mov     dx,0D8F1h
        mov     cx,word ptr ds:[49B6h]
L41B1:
        lodsw
        cmp     ax,bx
        jg      L41B8
        mov     bx,ax
L41B8:
        cmp     ax,dx
        jl      L41BE
        mov     dx,ax
L41BE:
        inc     si
        inc     si
        loop    L41B1
        mov     ax,dx
L41C4:
        mov     si,bp
        inc     di
        inc     di
        cmp     ax,bx
        jle     L41CD
        xchg    bx,ax
L41CD:
        stosw
        xchg    bx,ax
        stosw
        mov     word ptr ds:[49AAh],di
        or      word ptr [di],8000h
        jmp     word ptr ds:[SPAN_WRITER]
_seg003_4134 endp

; seg003_41DC  (+41DC)
; _41DC: walk the chain from the top vertex backwards through the vertex list, filling one side
; of the row records (through _423B) until an edge turns upwards.
_seg003_41DC proc near
        inc     di
        inc     di
        inc     si
        inc     si
L41E0:
        std
        lodsw
        xchg    bx,ax
        lodsw
        xchg    cx,ax
        cmp     si,415Eh
        jae     L41EF
        mov     si,word ptr ds:[49C0h]
L41EF:
        lodsw
        xchg    dx,ax
        lodsw
        xchg    cx,ax
        add     si,4
        cld
        cmp     bx,dx
        jl      L4208
        mov     word ptr ds:[4A06h],cx
        call    _seg003_423B
        dec     word ptr ds:[49B8h]
        jg      L41E0
L4208:
        ret
_seg003_41DC endp

; seg003_4209  (+4209)
; _4209: the same walk forwards, for the other side.
_seg003_4209 proc near
        add     di,4
L420C:
        lodsw
        xchg    dx,ax
        lodsw
        xchg    bx,ax
        cmp     si,word ptr ds:[49C0h]
        jb      L4219
        mov     si,415Ch
L4219:
        lodsw
        xchg    cx,ax
        lodsw
        xchg    dx,ax
        sub     si,4
        cmp     bx,dx
        jl      L4231
        mov     word ptr ds:[4A08h],cx
        call    _seg003_423B
        dec     word ptr ds:[49BAh]
        jg      L420C
L4231:
        ret
L4232:
        sub     bx,bx
        sub     cx,cx
        sub     dx,dx
        jmp     short L4265
L423A:
        retn
_seg003_4209 endp

; seg003_423B  (+423B)
; _423B: one edge, from (AX, BX) to (CX, DX): its rows from BX down to DX get the stepped x
; (16.16 slope from two divides), 31 rows a call of the unrolled _4282 and the remainder through
; the entry table at 49C2.
_seg003_423B proc near
        sub     bx,dx
        mov     word ptr ds:[4A04h],bx
        mov     bp,bx
        jle     L423A
        stosw
        add     di,4
        sub     cx,ax
        je      L4232
        xchg    cx,ax
        cwd
        idiv    bp
        xchg    bx,ax
        sub     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    bp
        cwd
        shl     ax,1
        rcl     dx,1
        add     bx,dx
        xchg    cx,ax
        mov     dx,7FFFh
L4265:
        cmp     bp,20h
        ja      L4270
        shl     bp,1
        jmp     word ptr [bp+49C2h]
L4270:
        call    _seg003_4282
        sub     bp,1Fh
        cmp     bp,20h
        ja      L4270
        shl     bp,1
        jmp     word ptr [bp+49C2h]
        even
_seg003_423B endp

; seg003_4282  (+4282)
; _4282: 31 rows of an edge, unrolled: add the slope to the 16.16 x and store it in the next
; record. L437B, after it, is polygon's case for one or two vertices: a line through cline_si or
; a point through pixel.
_seg003_4282 proc near
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        add     dx,cx
        adc     ax,bx
        stosw
        add     di,4
        ret
L437B:
        jne     L4383
        mov     si,415Ch
        jmp     _seg003_3F06
L4383:
        mov     ax,word ptr ds:[VERTS]
        mov     bx,word ptr ds:[415Eh]
        jmp     _seg003_3B34
_seg003_4282 endp

; seg003_438D  (+438D)
; polygon: CX = the vertex count (more than 99 is refused), vertices at 415C. Two or fewer go to
; L437B; otherwise shclip (GRLIBF.ASM), then upolygon. GRCORE's _56F3 is the C wrapper.
_seg003_438D proc near
        cmp     cx,63h
        ja      L439D
        cmp     cx,2
        jbe     L437B
        call    _seg003_3D47
        jmp     _seg003_4134
L439D:
        ret
_seg003_438D endp

; seg003_439E  (+439E)
; _439E, _43A6, _43AE: string entries (see _43B6). _439E and _43A6 use the bitmap blitter L49A6
; (_439E clipped, _43A6 unclipped); _43AE is the clipped single-colour entry with the colour in
; DX. GRCORE reaches them through the jump table at 527E .. 528D.
_seg003_439E proc near
        mov     word ptr ds:[4D14h],offset L49A6
        jmp     short L43CA
_seg003_43A6:
        mov     word ptr ds:[4D14h],offset L49A6
        jmp     short L4403
_seg003_43AE:
        mov     word ptr ds:[4D14h],offset _seg003_44E1
        jmp     short L43C6
_seg003_439E endp

; seg003_43B6  (+43B6)
; _43B6 (string_to_screen, FM Towns): draw the string at SI with its top-left corner at (AX, BX)
; in the colour at 2D38, through the single-colour blitter. The clipped entries give up unless
; the whole text box (font height up from y, and x) is inside the window: text is clipped whole,
; not cut. _43BE is the unclipped entry (ustring_to_screen).
_seg003_43B6 proc near
        mov     word ptr ds:[4D14h],offset _seg003_44E1
        jmp     short L43CA
_seg003_43BE:
        mov     word ptr ds:[4D14h],offset _seg003_44E1
        jmp     short L4403
L43C6:
        mov     word ptr ds:[2D38h],dx
L43CA:
        mov     word ptr ds:[TEXT_X],ax
        mov     word ptr ds:[TEXT_Y],bx
        mov     ax,word ptr ds:[TEXT_Y]
        cmp     ax,word ptr ds:[WIN_BOTTOM]
        jl      L440E
        cmp     ax,word ptr ds:[WIN_TOP]
        jg      L440E
        sub     ax,word ptr ds:[4D0Ch]
        inc     ax
        mov     word ptr ds:[4A12h],ax
        cmp     ax,word ptr ds:[WIN_BOTTOM]
        jl      L440E
        cmp     ax,word ptr ds:[WIN_TOP]
        jg      L440E
        mov     ax,word ptr ds:[TEXT_X]
        cmp     ax,word ptr ds:[WIN_LEFT]
        jl      L440E
        cmp     ax,word ptr ds:[WIN_RIGHT]
        jg      L440E
L4403:
        call    word ptr ds:[4D12h]
        mov     ax,word ptr ds:[2D38h]
        call    word ptr ds:[4D14h]
L440E:
        ret
_seg003_43B6 endp

; seg003_440F  (+440F)
; _440F (shadowed_string_to_screen, FM Towns): the same box check, then the string drawn one
; pixel right and one down in the shadow colour (2D34), the bit buffer shifted one pixel back
; (_49D0), and the string drawn again at (x, y) in the text colour (2D38). _4448 is the
; unclipped entry.
_seg003_440F proc near
        mov     word ptr ds:[TEXT_X],ax
        mov     word ptr ds:[TEXT_Y],bx
        mov     ax,word ptr ds:[TEXT_Y]
        cmp     ax,word ptr ds:[WIN_BOTTOM]
        jl      L440E
        cmp     ax,word ptr ds:[WIN_TOP]
        jg      L440E
        sub     ax,word ptr ds:[4D0Ch]
        inc     ax
        mov     word ptr ds:[4A12h],ax
        cmp     ax,word ptr ds:[WIN_BOTTOM]
        jl      L440E
        cmp     ax,word ptr ds:[WIN_TOP]
        jg      L440E
        mov     ax,word ptr ds:[TEXT_X]
        cmp     ax,word ptr ds:[WIN_LEFT]
        jl      L440E
        cmp     ax,word ptr ds:[WIN_RIGHT]
        jg      L440E
_seg003_4448:
        inc     word ptr ds:[TEXT_X]
        dec     word ptr ds:[TEXT_Y]
        call    word ptr ds:[4D12h]
        mov     ax,word ptr ds:[2D34h]
        call    _seg003_44E1
        call    _seg003_49D0
        mov     ax,word ptr ds:[2D38h]
        jmp     short _seg003_44E1
_seg003_440F endp

; seg003_4462  (+4462)
; _4462: build the 256-entry table at 4A24 from the 16 plane masks at 4A14: for each byte of a
; one-bit row, the map masks of its high and low four pixels. GRCORE's _4C75 runs it through the
; jump table at 5278.
_seg003_4462 proc near
        mov     cx,10h
        mov     si,4A14h
        mov     di,4A24h
L446B:
        lodsb
        push    cx
        push    si
        mov     cx,10h
        mov     si,4A14h
L4474:
        stosb
        movsb
        loop    L4474
        pop     si
        pop     cx
        loop    L446B
        ret
_seg003_4462 endp

; seg003_447D  (+447D)
; setup_font: after a font has been loaded into 4D06..4D10, build the glyph pointer table at
; 4D16 for characters 0 to 127, from 1C56h in steps of (height << (bytes per row - 1)) + 4D06
; and copy the font's height and row width for string_width.
;
; After its ret are small entries that set a colour (4114) and a span writer (4110: 52CC, 52DB,
; 52D8 or 52E7) and draw a rectangle from four words at SI (GRLIBF's _3C96); nothing in the
; sources jumps to them by name.
_seg003_447D proc near
        mov     bx,word ptr ds:[4D0Ch]
        mov     cx,word ptr ds:[4D0Eh]
        dec     cl
        shl     bx,cl
        mov     cx,80h
        mov     ax,1C56h
        mov     di,4D16h
        add     bx,word ptr ds:[4D06h]
L4496:
        stosw
        add     ax,bx
        loop    L4496
        mov     ax,word ptr ds:[4D0Ch]
        mov     word ptr ds:[2D3Ah],ax
        mov     ax,word ptr ds:[4D0Eh]
        dec     ax
        mov     word ptr ds:[2D3Ch],ax
        mov     ax,word ptr ds:[4D10h]
        mov     word ptr ds:[2D3Eh],ax
        ret
        mov     bx,offset _seg003_5AC6
L44B2:
        mov     word ptr ds:[PEN_WORD2],ax
        mov     word ptr ds:[SPAN_WRITER],bx
        mov     word ptr ds:[4112h],0FFFFh
        jmp     _seg003_3C96
        mov     bx,offset _seg003_5AD5
        jmp     L44B2
        mov     bx,offset _seg003_5AD2
        jmp     L44B2
L44CC:
        mov     bx,offset _seg003_5AE1
        jmp     L44B2
        push    word ptr [si+2]
        mov     ax,word ptr [si+6]
        mov     word ptr [si+2],ax
        call    L44CC
        pop     word ptr [si+2]
        ret
_seg003_447D endp

; seg003_44E1  (+44E1)
; _44E1: the single-colour blitter. In: AX = the colour. Works out the screen address of (4A0C,
; 4A0E) from Ytab, and writes the bit buffer row by row: for each byte, the two plane masks from
; 4A24 to port 3C5h, each followed by a store of the colour. The code at the second entry (after
; the jmp) sets the colour from AH and draws a filled box behind the text first. L49A6, near the
; end, is the other blitter.
_seg003_44E1 proc near
        mov     word ptr ds:[4A0Ah],ax
        jmp     short L4504
        mov     byte ptr ds:[PEN_COLOR],ah
        mov     ax,word ptr ds:[4D02h]
        dec     ax
        shl     ax,1
        shl     ax,1
        shl     ax,1
        add     ax,word ptr ds:[TEXT_X]
        mov     word ptr ds:[4A10h],ax
        mov     bx,5AC3h
        mov     si,4A0Ch
        call    L44B2
L4504:
        mov     di,word ptr ds:[TEXT_X]
        shr     di,1
        shr     di,1
        mov     bp,word ptr ds:[TEXT_Y]
        shl     bp,1
        add     di,word ptr [bp+YTAB]
        mov     bx,word ptr ds:[4D02h]
        shl     bx,1
        mov     bp,word ptr ds:[ROW_BYTES]
        sub     bp,bx
        mov     ax,word ptr [bx+4C8Ch]
        mov     word ptr ds:[4CF8h],ax
        mov     ch,byte ptr ds:[4D0Ch]
        mov     si,word ptr ds:[4D04h]
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
        mov     cl,byte ptr ds:[4A0Ah]
        mov     dx,SC_DATA
L453D:
        jmp     word ptr ds:[4CF8h]
        even
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+TEXT_MASKS]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        add     di,bp
        dec     ch
        je      L49A4
        jmp     L453D
L49A4:
        pop     es
        ret
L49A6:
        mov     ax,seg seg048
        mov     si,ax
        mov     di,word ptr ds:[4D04h]
        mov     ax,word ptr ds:[TEXT_X]
        mov     bx,word ptr ds:[TEXT_Y]
        mov     cx,word ptr ds:[4D02h]
        mov     dx,word ptr ds:[4D0Ch]
        jmp     _seg003_2C11
L49C1:
        popf
        call    _seg003_4A06
        pushf
        sub     bx,ax
        sub     si,ax
        cmp     bx,ax
        ja      L49C1
        jmp     short L49FB
_seg003_44E1 endp

; seg003_49D0  (+49D0)
; _49D0 (shift_left_1, FM Towns): shift the whole bit buffer left by one pixel (through _4A06
; and the entry table at 4C22) and move the text's x back by one, stepping the screen address
; back a byte when x crosses a multiple of 4.
_seg003_49D0 proc near
        clc
        pushf
        test    word ptr ds:[TEXT_X],3
        jne     L49E3
        dec     word ptr ds:[4D04h]
        sub     word ptr ds:[TEXT_X],4
L49E3:
        inc     word ptr ds:[TEXT_Y]
        dec     word ptr ds:[TEXT_X]
        mov     ax,35h
        mov     bx,word ptr ds:[4D00h]
        inc     bx
        mov     si,word ptr ds:[4CFEh]
        cmp     bx,ax
        ja      L49C1
L49FB:
        add     si,ax
        sub     si,bx
        shl     bx,1
        popf
        jmp     word ptr [bx+4C22h]
_seg003_49D0 endp

; seg003_4A06  (+4A06)
; _4A06: one bit left through 53 bytes of a buffer row, unrolled (rcl from SI down to SI-34h);
; entered part way through for shorter rows.
;
; After its ret is the rasteriser (4225h, the routine 4D12 points at): measure the string
; (string_width), size the bit buffer (4D02 bytes wide, font height rows) and clear it, then OR
; each glyph's rows into it at the running bit position, through the unrolled loop L4B4A..L4C0D
; entered by the font height (L4C25). Characters outside 20h..7Ah are drawn as '?'.
_seg003_4A06 proc near
        rcl     byte ptr [si],1
        rcl     byte ptr [si-1],1
        rcl     byte ptr [si-2],1
        rcl     byte ptr [si-3],1
        rcl     byte ptr [si-4],1
        rcl     byte ptr [si-5],1
        rcl     byte ptr [si-6],1
        rcl     byte ptr [si-7],1
        rcl     byte ptr [si-8],1
        rcl     byte ptr [si-9],1
        rcl     byte ptr [si-0Ah],1
        rcl     byte ptr [si-0Bh],1
        rcl     byte ptr [si-0Ch],1
        rcl     byte ptr [si-0Dh],1
        rcl     byte ptr [si-0Eh],1
        rcl     byte ptr [si-0Fh],1
        rcl     byte ptr [si-10h],1
        rcl     byte ptr [si-11h],1
        rcl     byte ptr [si-12h],1
        rcl     byte ptr [si-13h],1
        rcl     byte ptr [si-14h],1
        rcl     byte ptr [si-15h],1
        rcl     byte ptr [si-16h],1
        rcl     byte ptr [si-17h],1
        rcl     byte ptr [si-18h],1
        rcl     byte ptr [si-19h],1
        rcl     byte ptr [si-1Ah],1
        rcl     byte ptr [si-1Bh],1
        rcl     byte ptr [si-1Ch],1
        rcl     byte ptr [si-1Dh],1
        rcl     byte ptr [si-1Eh],1
        rcl     byte ptr [si-1Fh],1
        rcl     byte ptr [si-20h],1
        rcl     byte ptr [si-21h],1
        rcl     byte ptr [si-22h],1
        rcl     byte ptr [si-23h],1
        rcl     byte ptr [si-24h],1
        rcl     byte ptr [si-25h],1
        rcl     byte ptr [si-26h],1
        rcl     byte ptr [si-27h],1
        rcl     byte ptr [si-28h],1
        rcl     byte ptr [si-29h],1
        rcl     byte ptr [si-2Ah],1
        rcl     byte ptr [si-2Bh],1
        rcl     byte ptr [si-2Ch],1
        rcl     byte ptr [si-2Dh],1
        rcl     byte ptr [si-2Eh],1
        rcl     byte ptr [si-2Fh],1
        rcl     byte ptr [si-30h],1
        rcl     byte ptr [si-31h],1
        rcl     byte ptr [si-32h],1
        rcl     byte ptr [si-33h],1
        rcl     byte ptr [si-34h],1
        ret
        mov     di,si
        call    _seg003_4C45
        mov     si,di
        mov     ax,word ptr ds:[TEXT_X]
        and     ax,3
        add     dx,ax
        shr     dx,1
        shr     dx,1
        shr     dx,1
        mov     cx,dx
        inc     cx
        mov     word ptr ds:[4D02h],cx
        mov     bp,cx
        mov     ax,word ptr ds:[4D0Ch]
        mul     cx
        mov     cx,ax
        inc     cx
        mov     word ptr ds:[4D00h],cx
        mov     di,2D41h
        mov     dx,di
        inc     dx
        mov     word ptr ds:[4D04h],dx
        add     ax,dx
        mov     word ptr ds:[4CFEh],ax
        sub     ax,ax
        inc     cx
        shr     cx,1
        rep stosw
        mov     bx,word ptr ds:[4D0Ch]
        shl     bx,1
        mov     ax,word ptr cs:L4C25[bx-2]
        mov     word ptr ds:[4CF8h],ax
        mov     bx,si
        mov     cx,word ptr ds:[TEXT_X]
        and     cx,3
        jmp     short L4B21
L4AFE:
        cmp     al,8
        jle     L4B11
        cmp     ch,8
        jne     L4B0D
        sub     al,ch
        xor     ch,ch
        jmp     short L4B11
L4B0D:
        mov     al,8
        mov     ch,al
L4B11:
        add     cl,al
        cmp     cl,8
        jb      L4B1C
        inc     dx
        sub     cl,8
L4B1C:
        cmp     al,ch
        je      L4B21
        inc     bx
L4B21:
        sub     ax,ax
        mov     al,byte ptr [bx]
        test    al,al
        jne     L4B2C
        jmp     L4C24
L4B2C:
        cmp     al,20h
        jl      L4B34
        cmp     al,7Ah
        jle     L4B36
L4B34:
        mov     al,3Fh
L4B36:
        mov     si,ax
        shl     si,1
        mov     si,word ptr [si+4D16h]
        cmp     ch,8
        jne     L4B44
        inc     si
L4B44:
        mov     di,dx
        jmp     word ptr ds:[4CF8h]
L4B4A:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Ch]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4B57:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Ch]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4B64:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Ch]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4B71:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Ch]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4B7E:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Ch]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4B8B:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Ch]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4B98:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Ch]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4BA5:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Ch]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4BB2:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Ch]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4BBF:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Ch]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4BCC:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Ch]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4BD9:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Ch]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4BE6:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Ch]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4BF3:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Ch]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4C00:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Ch]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4C0D:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Ch]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
        cmp     ch,8
        jne     L4C20
        dec     si
L4C20:
        lodsb
        jmp     L4AFE
L4C24:
        ret
; entry points into the unrolled loop above, by the count at DS:4D0C (1 to 16)
L4C25   dw      offset L4C0D, offset L4C00, offset L4BF3, offset L4BE6
        dw      offset L4BD9, offset L4BCC, offset L4BBF, offset L4BB2
        dw      offset L4BA5, offset L4B98, offset L4B8B, offset L4B7E
        dw      offset L4B71, offset L4B64, offset L4B57, offset L4B4A
_seg003_4A06 endp

; seg003_4C45  (+4C45)
; string_width: in: SI = the string. Out: AX = DX = the sum of its characters' widths (the byte
; just past each glyph's rows), stopping at the terminating 0 or after 54 characters. GRCORE's
; string_width is the C wrapper.
_seg003_4C45 proc near
        mov     bx,word ptr ds:[2D3Ah]
        mov     cx,word ptr ds:[2D3Ch]
        shl     bx,cl
        mov     cx,36h
        sub     dx,dx
        sub     ax,ax
L4C56:
        lodsb
        test    al,al
        je      L4C6C
        mov     bp,ax
        shl     bp,1
        mov     bp,word ptr [bp+4D16h]
        add     bp,bx
        mov     al,byte ptr [bp]
        add     dx,ax
        loop    L4C56
L4C6C:
        mov     ax,dx
L4C6E:
        ret
_seg003_4C45 endp

SEG003_TEXT ends

        end
