; /* target: seg003_0272_38B4 */
; /* opts: /ml */
;
; Graphics library module 9: filled polygons and text. Found by reading the code and aligning
; with FM Towns, whose 2D library has the same pieces (upolygon, polygon; string_to_screen,
; ustring_to_screen, shadowed_string_to_screen, setup_font, vga_bitmap_to_screen, shift_left_1,
; string_width), in a different order.
;
; Polygons. polygon (_3B0D) clips with GRLIBF's shclip and fills with upolygon (_38B4), which
; walks the left and right chains of edges from the top vertex down, writes one span per row
; into the row table at 44CE and runs the current span writer (DS:4112), as GRLIBH's uline does
; for lines.
;
; Text. A string is drawn in two steps. The rasteriser at 4D14 (statically the unlabelled
; routine after _4186, 4225h) renders the whole string into a one-bit-per-pixel buffer at 2D43,
; glyph by glyph from the font, already shifted for x mod 4. Then a blitter (4D16) puts the
; buffer on the screen: _3C61 writes it straight to the planar (mode X) screen, setting the
; sequencer map mask (port 3C5h) from a table that turns each buffer byte into two four-pixel
; plane masks (4A26, built by _3BE2) and storing the colour, so only the set pixels are written;
; L4126 instead passes the buffer to VIDMODE's bitmap routine _222B.
;
; Data (seg_370D):
;   4A0E, 4A10  the text's x and y (y is its top row; y counts up); 4A0C the colour for _3C61
;   4D00..4D06  the bit buffer's size and pointers; 4D04 its width in bytes
;   4D08..4D13  the font's header (gfx.h struct FontInfo, which GRFX.C's grfx_load_font
;        reads here through cur_font): 4D08 widthsize, the bytes each glyph has after its
;        rows; 4D0E height; 4D10 rowwidth; 4D12 maxwidth, copied to 2D40; 4D18 the glyph
;        pointer of each character 0..127 (setup_font), into the glyphs at 1C54h (bytefont)
;   2D36, 2D3A  the shadow and text colours; 2D3C..2D40 the font fields string_width reads
;   4C8E, 4C24  entry tables into the unrolled blit and shift loops, by width
;   3DFC  the screen segment; 36AC the row address table (Ytab); 36A8 the row stride
;
; name: descriptive (module 9 of seg003). Its job, found here, is polygon filling and text; the
; routine names given here are FM Towns'.
;
; SEG003 split into its original modules: this is module 9 of 14, +38B4..+43F0 of the segment
; (file 6D04h), a module of the second library (UWLIB.LIB).
; Procs: seg003_0272_38B4, seg003_0272_395C, seg003_0272_3989, seg003_0272_39BB,
; seg003_0272_3A02, seg003_0272_3B0D, seg003_0272_3B1E, seg003_0272_3B36, seg003_0272_3B8F,
; seg003_0272_3BE2, seg003_0272_3BFD, seg003_0272_3C61, seg003_0272_4150, seg003_0272_4186,
; seg003_0272_43C5.
; Evidence for the bounds: starts after one zero byte following a ret: TLINK's padding before
; a word-aligned module (TASM 2.0's `even` pads with 90h).
; Its relocations are entries 2477 of UW2.EXE's relocation table.

; The VGA ports used here.
SC_DATA         equ     3C5h  ; sequencer data

        .186

        extrn   seg_370D:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg003_0272_222B:near
        extrn   _seg003_0272_32B4:near
        extrn   _seg003_0272_3416:near
        extrn   _seg003_0272_34C7:near
        extrn   _seg003_0272_3686:near
        extrn   _seg003_0272_52CC:near
        extrn   _seg003_0272_52D8:near
        extrn   _seg003_0272_52DB:near
        extrn   _seg003_0272_52E7:near
        public  _seg003_0272_38B4
        public  _seg003_0272_395C
        public  _seg003_0272_3989
        public  _seg003_0272_39BB
        public  _seg003_0272_3A02
        public  _seg003_0272_3B0D
        public  _seg003_0272_3B1E
        public  _seg003_0272_3B26
        public  _seg003_0272_3B2E
        public  _seg003_0272_3B36
        public  _seg003_0272_3B3E
        public  _seg003_0272_3B8F
        public  _seg003_0272_3BC8
        public  _seg003_0272_3BE2
        public  _seg003_0272_3BFD
        public  _seg003_0272_3C61
        public  _seg003_0272_4150
        public  _seg003_0272_4186
        public  _seg003_0272_43C5

; seg003_0272_38B4  (+38B4)
; upolygon: fill a polygon already inside the window. In: CX = the vertex count, the vertices
; (x, y words) from 415E. Finds the top vertex, steps the two chains (_395C going one way round,
; _3989 the other) to fill the row table, and jumps to the span writer. A polygon of zero height
; becomes one span from its least to its greatest x.
_seg003_0272_38B4 proc near
        mov     si,word ptr ds:[49ACh]
        and     word ptr [si],7FFFh
        mov     word ptr ds:[49B8h],cx
        mov     word ptr ds:[49BAh],cx
        mov     word ptr ds:[49BCh],cx
        mov     di,0FC19h
        mov     si,4160h
        mov     bp,415Eh
        jmp     short L38DC
L38D3:
        mov     bp,si
        sub     bp,6
        xchg    di,ax
        dec     cx
        je      L38E5
L38DC:
        lodsw
        inc     si
        inc     si
        cmp     di,ax
        jl      L38D3
        loop    L38DC
L38E5:
        sub     si,4
        mov     word ptr ds:[49C2h],si
        mov     si,bp
        mov     word ptr ds:[49C0h],si
        shl     di,1
        mov     ax,di
        shl     di,1
        add     di,ax
        neg     di
        add     di,4978h
        mov     word ptr ds:[49BEh],di
        call    _seg003_0272_395C
        mov     si,word ptr ds:[49C0h]
        mov     di,word ptr ds:[49BEh]
        call    _seg003_0272_3989
        sub     di,4
        mov     bp,word ptr ds:[49BEh]
        mov     ax,word ptr ds:[4A08h]
        mov     bx,word ptr ds:[4A0Ah]
        cmp     bp,di
        jne     L3944
        mov     si,415Eh
        mov     bx,270Fh
        mov     dx,0D8F1h
        mov     cx,word ptr ds:[49B8h]
L3931:
        lodsw
        cmp     ax,bx
        jg      L3938
        mov     bx,ax
L3938:
        cmp     ax,dx
        jl      L393E
        mov     dx,ax
L393E:
        inc     si
        inc     si
        loop    L3931
        mov     ax,dx
L3944:
        mov     si,bp
        inc     di
        inc     di
        cmp     ax,bx
        jle     L394D
        xchg    bx,ax
L394D:
        stosw
        xchg    bx,ax
        stosw
        mov     word ptr ds:[49ACh],di
        or      word ptr [di],8000h
        jmp     word ptr ds:[4112h]
_seg003_0272_38B4 endp

; seg003_0272_395C  (+395C)
; _395C: walk the chain from the top vertex backwards through the vertex list, filling one side
; of the row records (through _39BB) until an edge turns upwards.
_seg003_0272_395C proc near
        inc     di
        inc     di
        inc     si
        inc     si
L3960:
        std
        lodsw
        xchg    bx,ax
        lodsw
        xchg    cx,ax
        cmp     si,4160h
        jae     L396F
        mov     si,word ptr ds:[49C2h]
L396F:
        lodsw
        xchg    dx,ax
        lodsw
        xchg    cx,ax
        add     si,4
        cld
        cmp     bx,dx
        jl      L3988
        mov     word ptr ds:[4A08h],cx
        call    _seg003_0272_39BB
        dec     word ptr ds:[49BAh]
        jg      L3960
L3988:
        ret
_seg003_0272_395C endp

; seg003_0272_3989  (+3989)
; _3989: the same walk forwards, for the other side.
_seg003_0272_3989 proc near
        add     di,4
L398C:
        lodsw
        xchg    dx,ax
        lodsw
        xchg    bx,ax
        cmp     si,word ptr ds:[49C2h]
        jb      L3999
        mov     si,415Eh
L3999:
        lodsw
        xchg    cx,ax
        lodsw
        xchg    dx,ax
        sub     si,4
        cmp     bx,dx
        jl      L39B1
        mov     word ptr ds:[4A0Ah],cx
        call    _seg003_0272_39BB
        dec     word ptr ds:[49BCh]
        jg      L398C
L39B1:
        ret
L39B2:
        sub     bx,bx
        sub     cx,cx
        sub     dx,dx
        jmp     short L39E5
L39BA:
        retn
_seg003_0272_3989 endp

; seg003_0272_39BB  (+39BB)
; _39BB: one edge, from (AX, BX) to (CX, DX): its rows from BX down to DX get the stepped x
; (16.16 slope from two divides), 31 rows a call of the unrolled _3A02 and the remainder through
; the entry table at 49C4.
_seg003_0272_39BB proc near
        sub     bx,dx
        mov     word ptr ds:[4A06h],bx
        mov     bp,bx
        jle     L39BA
        stosw
        add     di,4
        sub     cx,ax
        je      L39B2
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
L39E5:
        cmp     bp,20h
        ja      L39F0
        shl     bp,1
        jmp     word ptr [bp+49C4h]
L39F0:
        call    _seg003_0272_3A02
        sub     bp,1Fh
        cmp     bp,20h
        ja      L39F0
        shl     bp,1
        jmp     word ptr [bp+49C4h]
        even
_seg003_0272_39BB endp

; seg003_0272_3A02  (+3A02)
; _3A02: 31 rows of an edge, unrolled: add the slope to the 16.16 x and store it in the next
; record. L3AFB, after it, is polygon's case for one or two vertices: a line through cline_si or
; a point through pixel.
_seg003_0272_3A02 proc near
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
L3AFB:
        jne     L3B03
        mov     si,415Eh
        jmp     _seg003_0272_3686
L3B03:
        mov     ax,word ptr ds:[415Eh]
        mov     bx,word ptr ds:[4160h]
        jmp     _seg003_0272_32B4
_seg003_0272_3A02 endp

; seg003_0272_3B0D  (+3B0D)
; polygon: CX = the vertex count (more than 99 is refused), vertices at 415E. Two or fewer go to
; L3AFB; otherwise shclip (GRLIBF.ASM), then upolygon. GRCORE's _4EFA is the C wrapper.
_seg003_0272_3B0D proc near
        cmp     cx,63h
        ja      L3B1D
        cmp     cx,2
        jbe     L3AFB
        call    _seg003_0272_34C7
        jmp     _seg003_0272_38B4
L3B1D:
        ret
_seg003_0272_3B0D endp

; seg003_0272_3B1E  (+3B1E)
; _3B1E, _3B26, _3B2E: string entries (see _3B36). _3B1E and _3B26 use the bitmap blitter L4126
; (_3B1E clipped, _3B26 unclipped); _3B2E is the clipped single-colour entry with the colour in
; DX. GRCORE reaches them through the jump table at 527E .. 528D.
_seg003_0272_3B1E proc near
        mov     word ptr ds:[4D16h],offset L4126
        jmp     short L3B4A
_seg003_0272_3B26:
        mov     word ptr ds:[4D16h],offset L4126
        jmp     short L3B83
_seg003_0272_3B2E:
        mov     word ptr ds:[4D16h],offset _seg003_0272_3C61
        jmp     short L3B46
_seg003_0272_3B1E endp

; seg003_0272_3B36  (+3B36)
; _3B36 (string_to_screen, FM Towns): draw the string at SI with its top-left corner at (AX, BX)
; in the colour at 2D3A, through the single-colour blitter. The clipped entries give up unless
; the whole text box (font height up from y, and x) is inside the window: text is clipped whole,
; not cut. _3B3E is the unclipped entry (ustring_to_screen).
_seg003_0272_3B36 proc near
        mov     word ptr ds:[4D16h],offset _seg003_0272_3C61
        jmp     short L3B4A
_seg003_0272_3B3E:
        mov     word ptr ds:[4D16h],offset _seg003_0272_3C61
        jmp     short L3B83
L3B46:
        mov     word ptr ds:[2D3Ah],dx
L3B4A:
        mov     word ptr ds:[4A0Eh],ax
        mov     word ptr ds:[4A10h],bx
        mov     ax,word ptr ds:[4A10h]
        cmp     ax,word ptr ds:[3DFAh]
        jl      L3B8E
        cmp     ax,word ptr ds:[3DF6h]
        jg      L3B8E
        sub     ax,word ptr ds:[4D0Eh]
        inc     ax
        mov     word ptr ds:[4A14h],ax
        cmp     ax,word ptr ds:[3DFAh]
        jl      L3B8E
        cmp     ax,word ptr ds:[3DF6h]
        jg      L3B8E
        mov     ax,word ptr ds:[4A0Eh]
        cmp     ax,word ptr ds:[3DF4h]
        jl      L3B8E
        cmp     ax,word ptr ds:[3DF8h]
        jg      L3B8E
L3B83:
        call    word ptr ds:[4D14h]
        mov     ax,word ptr ds:[2D3Ah]
        call    word ptr ds:[4D16h]
L3B8E:
        ret
_seg003_0272_3B36 endp

; seg003_0272_3B8F  (+3B8F)
; _3B8F (shadowed_string_to_screen, FM Towns): the same box check, then the string drawn one
; pixel right and one down in the shadow colour (2D36), the bit buffer shifted one pixel back
; (_4150), and the string drawn again at (x, y) in the text colour (2D3A). _3BC8 is the
; unclipped entry.
_seg003_0272_3B8F proc near
        mov     word ptr ds:[4A0Eh],ax
        mov     word ptr ds:[4A10h],bx
        mov     ax,word ptr ds:[4A10h]
        cmp     ax,word ptr ds:[3DFAh]
        jl      L3B8E
        cmp     ax,word ptr ds:[3DF6h]
        jg      L3B8E
        sub     ax,word ptr ds:[4D0Eh]
        inc     ax
        mov     word ptr ds:[4A14h],ax
        cmp     ax,word ptr ds:[3DFAh]
        jl      L3B8E
        cmp     ax,word ptr ds:[3DF6h]
        jg      L3B8E
        mov     ax,word ptr ds:[4A0Eh]
        cmp     ax,word ptr ds:[3DF4h]
        jl      L3B8E
        cmp     ax,word ptr ds:[3DF8h]
        jg      L3B8E
_seg003_0272_3BC8:
        inc     word ptr ds:[4A0Eh]
        dec     word ptr ds:[4A10h]
        call    word ptr ds:[4D14h]
        mov     ax,word ptr ds:[2D36h]
        call    _seg003_0272_3C61
        call    _seg003_0272_4150
        mov     ax,word ptr ds:[2D3Ah]
        jmp     short _seg003_0272_3C61
_seg003_0272_3B8F endp

; seg003_0272_3BE2  (+3BE2)
; _3BE2: build the 256-entry table at 4A26 from the 16 plane masks at 4A16: for each byte of a
; one-bit row, the map masks of its high and low four pixels. GRCORE's _43F5 runs it through the
; jump table at 5278.
_seg003_0272_3BE2 proc near
        mov     cx,10h
        mov     si,4A16h
        mov     di,4A26h
L3BEB:
        lodsb
        push    cx
        push    si
        mov     cx,10h
        mov     si,4A16h
L3BF4:
        stosb
        movsb
        loop    L3BF4
        pop     si
        pop     cx
        loop    L3BEB
        ret
_seg003_0272_3BE2 endp

; seg003_0272_3BFD  (+3BFD)
; setup_font: after a font has been loaded into 4D08..4D12, build the glyph pointer table at
; 4D18 for characters 0 to 127, from 1C54h in steps of (height << (bytes per row - 1)) + 4D08
; and copy the font's height and row width for string_width.
;
; After its ret are small entries that set a colour (4116) and a span writer (4112: 52CC, 52DB,
; 52D8 or 52E7) and draw a rectangle from four words at SI (GRLIBF's _3416); nothing in the
; sources jumps to them by name.
_seg003_0272_3BFD proc near
        mov     bx,word ptr ds:[4D0Eh]
        mov     cx,word ptr ds:[4D10h]
        dec     cl
        shl     bx,cl
        mov     cx,80h
        mov     ax,1C54h
        mov     di,4D18h
        add     bx,word ptr ds:[4D08h]
L3C16:
        stosw
        add     ax,bx
        loop    L3C16
        mov     ax,word ptr ds:[4D0Eh]
        mov     word ptr ds:[2D3Ch],ax
        mov     ax,word ptr ds:[4D10h]
        dec     ax
        mov     word ptr ds:[2D3Eh],ax
        mov     ax,word ptr ds:[4D12h]
        mov     word ptr ds:[2D40h],ax
        ret
        mov     bx,offset _seg003_0272_52CC
L3C32:
        mov     word ptr ds:[4116h],ax
        mov     word ptr ds:[4112h],bx
        mov     word ptr ds:[4114h],0FFFFh
        jmp     _seg003_0272_3416
        mov     bx,offset _seg003_0272_52DB
        jmp     L3C32
        mov     bx,offset _seg003_0272_52D8
        jmp     L3C32
L3C4C:
        mov     bx,offset _seg003_0272_52E7
        jmp     L3C32
        push    word ptr [si+2]
        mov     ax,word ptr [si+6]
        mov     word ptr [si+2],ax
        call    L3C4C
        pop     word ptr [si+2]
        ret
_seg003_0272_3BFD endp

; seg003_0272_3C61  (+3C61)
; _3C61: the single-colour blitter. In: AX = the colour. Works out the screen address of (4A0E,
; 4A10) from Ytab, and writes the bit buffer row by row: for each byte, the two plane masks from
; 4A26 to port 3C5h, each followed by a store of the colour. The code at the second entry (after
; the jmp) sets the colour from AH and draws a filled box behind the text first. L4126, near the
; end, is the other blitter.
_seg003_0272_3C61 proc near
        mov     word ptr ds:[4A0Ch],ax
        jmp     short L3C84
        mov     byte ptr ds:[4111h],ah
        mov     ax,word ptr ds:[4D04h]
        dec     ax
        shl     ax,1
        shl     ax,1
        shl     ax,1
        add     ax,word ptr ds:[4A0Eh]
        mov     word ptr ds:[4A12h],ax
        mov     bx,52C9h
        mov     si,4A0Eh
        call    L3C32
L3C84:
        mov     di,word ptr ds:[4A0Eh]
        shr     di,1
        shr     di,1
        mov     bp,word ptr ds:[4A10h]
        shl     bp,1
        add     di,word ptr [bp+36ACh]
        mov     bx,word ptr ds:[4D04h]
        shl     bx,1
        mov     bp,word ptr ds:[36A8h]
        sub     bp,bx
        mov     ax,word ptr [bx+4C8Eh]
        mov     word ptr ds:[4CFAh],ax
        mov     ch,byte ptr ds:[4D0Eh]
        mov     si,word ptr ds:[4D06h]
        push    es
        mov     es,word ptr ds:[3DFCh]
        mov     cl,byte ptr ds:[4A0Ch]
        mov     dx,SC_DATA
L3CBD:
        jmp     word ptr ds:[4CFAh]
        even
        lodsb
        sub     bx,bx
        mov     bl,al
        shl     bx,1
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
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
        mov     ax,word ptr [bx+4A26h]
        out     dx,al
        mov     al,cl
        stosb
        mov     al,ah
        out     dx,al
        mov     al,cl
        stosb
        add     di,bp
        dec     ch
        je      L4124
        jmp     L3CBD
L4124:
        pop     es
        ret
L4126:
        mov     ax,seg seg_370D
        mov     si,ax
        mov     di,word ptr ds:[4D06h]
        mov     ax,word ptr ds:[4A0Eh]
        mov     bx,word ptr ds:[4A10h]
        mov     cx,word ptr ds:[4D04h]
        mov     dx,word ptr ds:[4D0Eh]
        jmp     _seg003_0272_222B
L4141:
        popf
        call    _seg003_0272_4186
        pushf
        sub     bx,ax
        sub     si,ax
        cmp     bx,ax
        ja      L4141
        jmp     short L417B
_seg003_0272_3C61 endp

; seg003_0272_4150  (+4150)
; _4150 (shift_left_1, FM Towns): shift the whole bit buffer left by one pixel (through _4186
; and the entry table at 4C24) and move the text's x back by one, stepping the screen address
; back a byte when x crosses a multiple of 4.
_seg003_0272_4150 proc near
        clc
        pushf
        test    word ptr ds:[4A0Eh],3
        jne     L4163
        dec     word ptr ds:[4D06h]
        sub     word ptr ds:[4A0Eh],4
L4163:
        inc     word ptr ds:[4A10h]
        dec     word ptr ds:[4A0Eh]
        mov     ax,35h
        mov     bx,word ptr ds:[4D02h]
        inc     bx
        mov     si,word ptr ds:[4D00h]
        cmp     bx,ax
        ja      L4141
L417B:
        add     si,ax
        sub     si,bx
        shl     bx,1
        popf
        jmp     word ptr [bx+4C24h]
_seg003_0272_4150 endp

; seg003_0272_4186  (+4186)
; _4186: one bit left through 53 bytes of a buffer row, unrolled (rcl from SI down to SI-34h);
; entered part way through for shorter rows.
;
; After its ret is the rasteriser (4225h, the routine 4D14 points at): measure the string
; (string_width), size the bit buffer (4D04 bytes wide, font height rows) and clear it, then OR
; each glyph's rows into it at the running bit position, through the unrolled loop L42CA..L438D
; entered by the font height (L43A5). Characters outside 20h..7Ah are drawn as '?'.
_seg003_0272_4186 proc near
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
        call    _seg003_0272_43C5
        mov     si,di
        mov     ax,word ptr ds:[4A0Eh]
        and     ax,3
        add     dx,ax
        shr     dx,1
        shr     dx,1
        shr     dx,1
        mov     cx,dx
        inc     cx
        mov     word ptr ds:[4D04h],cx
        mov     bp,cx
        mov     ax,word ptr ds:[4D0Eh]
        mul     cx
        mov     cx,ax
        inc     cx
        mov     word ptr ds:[4D02h],cx
        mov     di,2D43h
        mov     dx,di
        inc     dx
        mov     word ptr ds:[4D06h],dx
        add     ax,dx
        mov     word ptr ds:[4D00h],ax
        sub     ax,ax
        inc     cx
        shr     cx,1
        rep stosw
        mov     bx,word ptr ds:[4D0Eh]
        shl     bx,1
        mov     ax,word ptr cs:L43A5[bx-2]
        mov     word ptr ds:[4CFAh],ax
        mov     bx,si
        mov     cx,word ptr ds:[4A0Eh]
        and     cx,3
        jmp     short L42A1
L427E:
        cmp     al,8
        jle     L4291
        cmp     ch,8
        jne     L428D
        sub     al,ch
        xor     ch,ch
        jmp     short L4291
L428D:
        mov     al,8
        mov     ch,al
L4291:
        add     cl,al
        cmp     cl,8
        jb      L429C
        inc     dx
        sub     cl,8
L429C:
        cmp     al,ch
        je      L42A1
        inc     bx
L42A1:
        sub     ax,ax
        mov     al,byte ptr [bx]
        test    al,al
        jne     L42AC
        jmp     L43A4
L42AC:
        cmp     al,20h
        jl      L42B4
        cmp     al,7Ah
        jle     L42B6
L42B4:
        mov     al,3Fh
L42B6:
        mov     si,ax
        shl     si,1
        mov     si,word ptr [si+4D18h]
        cmp     ch,8
        jne     L42C4
        inc     si
L42C4:
        mov     di,dx
        jmp     word ptr ds:[4CFAh]
L42CA:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L42D7:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L42E4:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L42F1:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L42FE:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L430B:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4318:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4325:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4332:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L433F:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L434C:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4359:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4366:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4373:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L4380:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
L438D:
        sub     ax,ax
        lodsb
        add     si,word ptr ds:[2D3Eh]
        ror     ax,cl
        or      word ptr [di],ax
        add     di,bp
        cmp     ch,8
        jne     L43A0
        dec     si
L43A0:
        lodsb
        jmp     L427E
L43A4:
        ret
; entry points into the unrolled loop above, by the count at DS:4D0E (1 to 16)
L43A5   dw      offset L438D, offset L4380, offset L4373, offset L4366
        dw      offset L4359, offset L434C, offset L433F, offset L4332
        dw      offset L4325, offset L4318, offset L430B, offset L42FE
        dw      offset L42F1, offset L42E4, offset L42D7, offset L42CA
_seg003_0272_4186 endp

; seg003_0272_43C5  (+43C5)
; string_width: in: SI = the string. Out: AX = DX = the sum of its characters' widths (the byte
; just past each glyph's rows), stopping at the terminating 0 or after 54 characters. GRCORE's
; string_width is the C wrapper.
_seg003_0272_43C5 proc near
        mov     bx,word ptr ds:[2D3Ch]
        mov     cx,word ptr ds:[2D3Eh]
        shl     bx,cl
        mov     cx,36h
        sub     dx,dx
        sub     ax,ax
L43D6:
        lodsb
        test    al,al
        je      L43EC
        mov     bp,ax
        shl     bp,1
        mov     bp,word ptr [bp+4D18h]
        add     bp,bx
        mov     al,byte ptr [bp]
        add     dx,ax
        loop    L43D6
L43EC:
        mov     ax,dx
L43EE:
        ret
_seg003_0272_43C5 endp

SEG003_TEXT ends

        end
