; /* target: seg003_4C70 */
; /* opts: /ml */
;
; Graphics library module 13: the C entry points. Each far routine here is called by the game's
; C code with C arguments, saves the caller's SS:SP in L4C70/L4C72, switches DS, ES and SS to
; seg048 with the stack at 4FA6h, loads the arguments into the registers the library routine
; wants, calls it (directly or through the table of near jumps at _5A72 .. _5AE4, GRJUMPS.ASM)
; and switches back. String entries first copy the far string (up to 84h bytes) to 3963:4FA6,
; below the private stack. Its _DATA is the graphics globals C uses: palette, Color_data_ptr,
; foreground_color, background_color, cur_font, bytefont, wtop, wbot, wright, wleft, pixel_color
; and Ytab, far pointers into seg048 (3963:0000): wleft, wright, wbot and wtop are the clip
; window at 3DF2 .. 3DF8, cur_font the font header at 4D06, bytefont the glyphs at 1C56, Ytab
; the row table at 36AA, foreground_color and background_color the text colours at 2D38 and
; 2D39.
;
; Entry point -> library routine (module; FM Towns name where the alignment gives one):
;   4C75 -> _4462 build the text masks (GRLIBI)      setup_font -> _447D (GRLIBI)
;   string_to_screen -> _43B6 .. (GRLIBI)            4D5C -> _440F shadowed string (GRLIBI)
;   string_width -> _4C45 (GRLIBI)                   4E3A -> _439E string via bitmap (GRLIBI)
;   init_graphics -> _321D (VIDMODE)                 4EE5 -> _32F4 load a palette (VIDMODE)
;   grPageFlip, grSoftPageFlip -> _3357, _335A        4FA2 -> _333F show the drawing page
;   gr_read_pixel -> _397C, 5020 -> _3964 (read)      plot_pixel -> _3915, 50A4 -> _392D
;   50E6 -> _39E5, 512D -> _39A2 vertical lines       set_the_color -> _3A16, init_colors -> _3A3F
;   51EF -> _324C set the screen mode                522E, 5274 -> _3A86, _3A9B video memory
;   set_the_window -> _3A68 (VIDMODE)                copy_hidden_to_visible, copy_visible_to_hidden
;   538C -> _3B9A, 545D -> _3BA1 vline, uvline -> _3BA4; 53D3 -> _3CFF hline, uhline -> _3D2E
;   rectangle, urectangle, clear_window, box -> _3C9E, _3CAE, _3CA3, _3BEE; 5605 -> _3BF1 ubox
;   564F -> _3CAE with the window; 56A9 -> _3F0E cline (GRLIBG); 5771 -> _3FDC uline (GRLIBH)
;   56F3 -> _438D polygon, 5732 -> _4134 upolygon (GRLIBI)
;   show, 581E, fbshow, 5915, vcopyfb, vcopy -> _2BBA, _2BD3, _2BFA, _2C11, _2C28, _2C36
;   58E0 -> _33E3 set the linear target; fbuf_setcolor -> _EFF (GRENTRY)
;
; name: descriptive (UW2Decomp: module 10 of seg003, "the C entry points"). The C names are the
; ones the C sources use, FM Towns' where the alignment gave one; the rest keep their addresses.
;
; UW1 against UW2: UW2Decomp's src/gfx/GRCORE.ASM (UW2's seg003_0272_43F0) without the two
; virtual-screen wrappers (UW2's _4A3A and vscreen_focus, 87h bytes; VIDMODE has no virtual
; screen in UW1), and without the jump table that ends it in UW2, which in UW1 is a module of
; its own (GRJUMPS.ASM). Its _DATA has 30 far pointers where UW2's has 31: UW1 has none to the
; row stride (UW2's seg048+36A0h, 36A8). Otherwise only numbers differ: the private stack and
; string buffer at 4FA6 (UW2 4FA8), and the data 2 bytes lower throughout.
;
; SEG003 split into its original modules: this is module 13 of UW1's 18, +4C70..+5A71.
; Evidence for the bounds: starts after one zero byte following a ret (TLINK's padding before a
; word-aligned module), as in UW2; it ends at the zero byte at +5A71 (see GRJUMPS.ASM).
; Its relocations are entries 2798-2853 of UW.EXE's relocation table (+4C8B..+5A4B). Its _DATA
; is DS:23B4..242B (relocation entries 2768-2797), defined before the code because those
; entries come before the code's.

; The graphics library's data in seg048 (DS while the library runs), as GRLIBF.ASM's
; and VIDMODE.ASM's headers describe it. The names are ours.
SPAN_WRITER     equ     4110h   ; the current span writer (set_the_color, fbuf_setcolor)

        .186

        extrn   seg048:far

_DATA   segment word public 'DATA'
        dd      seg048+415Ch
        dd      seg048+4FA6h
_palette label dword
        dd      seg048+5046h
        dd      seg048+8
        dd      seg048+21Eh
_Color_data_ptr label dword
        dd      seg048+434h
        dd      seg048+64Ah
        dd      seg048+2D34h
_foreground_color label dword
        dd      seg048+2D38h
_background_color label dword
        dd      seg048+2D39h
_cur_font label dword
        dd      seg048+4D06h
_bytefont label dword
        dd      seg048+1C56h
        dd      seg048+3DFEh
        dd      seg048+3DFCh
_wtop label dword
        dd      seg048+3DF4h
_wbot label dword
        dd      seg048+3DF8h
_wright label dword
        dd      seg048+3DF6h
_wleft label dword
        dd      seg048+3DF2h
        dd      seg048+3DF2h
_pixel_color label dword
        dd      seg048+410Fh
_dseg_5c99_2404 label dword
        dd      seg048+3838h
_Ytab label dword
        dd      seg048+36AAh
        dd      seg048+4114h
        dd      seg048+4108h
        dd      seg048+4106h
        dd      seg048+5044h
        dd      seg048+5044h
        dd      seg048+5044h
        dd      seg048+5044h
        dd      seg048+36A0h
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg003_5A72:near
        extrn   _seg003_5A75:near
        extrn   _seg003_5A78:near
        extrn   _seg003_5A81:near
        extrn   _seg003_5A87:near
        extrn   _seg003_5A8D:near
        extrn   _seg003_5A90:near
        extrn   _seg003_5A93:near
        extrn   _seg003_5A96:near
        extrn   _seg003_5A99:near
        extrn   _seg003_5A9C:near
        extrn   _seg003_5A9F:near
        extrn   _seg003_5AA2:near
        extrn   _seg003_5AAB:near
        extrn   _seg003_5AAE:near
        extrn   _seg003_5AB1:near
        extrn   _seg003_5AB4:near
        extrn   _seg003_5AB7:near
        extrn   _seg003_5ABA:near
        extrn   _seg003_5AC6:near
        extrn   _seg003_5ADE:near
        extrn   _seg003_2BBA:near
        extrn   _seg003_2BD3:near
        extrn   _seg003_2BFA:near
        extrn   _seg003_2C11:near
        extrn   _seg003_2C28:near
        extrn   _seg003_2C36:near
        extrn   _seg003_33E3:near
        extrn   _seg003_3A86:near
        extrn   _seg003_3A9B:near
        extrn   _seg003_3AFD:near
        extrn   _seg003_3B0F:near
        extrn   _seg003_3B9A:near
        extrn   _seg003_3BA1:near
        extrn   _seg003_3BA4:near
        extrn   _seg003_3BEE:near
        extrn   _seg003_3BF1:near
        extrn   _seg003_3C9E:near
        extrn   _seg003_3CA3:near
        extrn   _seg003_3CAE:near
        extrn   _seg003_3CFF:near
        extrn   _seg003_3D2E:near
        extrn   _seg003_3F0E:near
        extrn   _seg003_3FDC:near
        extrn   _seg003_4134:near
        extrn   _seg003_438D:near
        extrn   _seg003_4C45:near
        extrn   _seg003_EFF:near
        public  _background_color
        public  _bytefont
        public  _Color_data_ptr
        public  _cur_font
        public  _dseg_5c99_2404
        public  _foreground_color
        public  _palette
        public  _pixel_color
        public  _wbot
        public  _wleft
        public  _wright
        public  _wtop
        public  _Ytab
        public  _box
        public  _clear_window
        public  _copy_hidden_to_visible
        public  _copy_visible_to_hidden
        public  _fbshow
        public  _fbuf_setcolor
        public  _gr_read_pixel
        public  _grPageFlip
        public  _grSoftPageFlip
        public  _init_colors
        public  _init_graphics
        public  _seg003_4C74
        public  _plot_pixel
        public  _rectangle
        public  _seg003_4C75
        public  _seg003_4D5C
        public  _seg003_4E3A
        public  _seg003_4EE5
        public  _seg003_4FA2
        public  _seg003_5020
        public  _seg003_50A4
        public  _seg003_50E6
        public  _seg003_512D
        public  _seg003_51EF
        public  _seg003_522E
        public  _seg003_5274
        public  _seg003_538C
        public  _seg003_53D3
        public  _seg003_545D
        public  _seg003_5605
        public  _seg003_564F
        public  _seg003_56A9
        public  _seg003_56F3
        public  _seg003_5732
        public  _seg003_5771
        public  _seg003_581E
        public  _seg003_58E0
        public  _seg003_5915
        public  _seg003_5A19
        public  _set_the_color
        public  _set_the_window
        public  _setup_font
        public  _show
        public  _string_to_screen
        public  _string_width
        public  _uhline
        public  _urectangle
        public  _uvline
        public  _vcopy
        public  _vcopyfb

L4C70   dw      0                       ; the caller's ss and sp
L4C72   dw      0

; seg003_4C74  (+4C74)
_seg003_4C74 proc near
        ret
_seg003_4C74 endp

; seg003_4C75  (+4C75): no caller found; switches to the private stack and calls
; through the vector at _seg003_5A72
_seg003_4C75 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_5A72
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_4C75 endp

; setup_font  (+4CB1)
_setup_font proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_5A75
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_setup_font endp

; WriteTextToScreen_seg003_4CED  (+4CED)
_string_to_screen proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        les     di,dword ptr [bp+6]
        mov     si,di
        mov     cx,84h
        xor     al,al
        repne scasb
        neg     cx
        add     cx,85h
        push    ds
        mov     di,es
        mov     ds,di
        mov     di,4FA6h
        mov     ax,seg seg048
        mov     es,ax
        rep movsb
        pop     ds
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+0Eh]
        mov     si,word ptr [bp+10h]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     dx,si
        mov     si,4FA6h
        call    _seg003_5A78
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_string_to_screen endp

; seg003_4D5C  (+4D5C)
; A shadowed string (GRLIBI's _440F): (far string, x, y, ...), the same arguments as
; string_to_screen.
_seg003_4D5C proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        les     di,dword ptr [bp+6]
        mov     si,di
        mov     cx,84h
        xor     al,al
        repne scasb
        neg     cx
        add     cx,85h
        push    ds
        mov     di,es
        mov     ds,di
        mov     di,4FA6h
        mov     ax,seg seg048
        mov     es,ax
        rep movsb
        pop     ds
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+0Eh]
        mov     si,word ptr [bp+10h]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     dx,si
        mov     si,4FA6h
        call    _seg003_5A81
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_4D5C endp

; WritingTextToScreen_seg003_4DCB  (+4DCB)
_string_width proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        les     di,dword ptr [bp+6]
        mov     si,di
        mov     cx,84h
        xor     al,al
        repne scasb
        neg     cx
        add     cx,85h
        push    ds
        mov     di,es
        mov     ds,di
        mov     di,4FA6h
        mov     ax,seg seg048
        mov     es,ax
        rep movsb
        pop     ds
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+0Eh]
        mov     si,word ptr [bp+10h]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     dx,si
        mov     si,4FA6h
        call    _seg003_4C45
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_string_width endp

; seg003_4E3A  (+4E3A)
; A string drawn through the bitmap blitter (GRLIBI's _439E), with string_to_screen's arguments.
_seg003_4E3A proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        les     di,dword ptr [bp+6]
        mov     si,di
        mov     cx,84h
        xor     al,al
        repne scasb
        neg     cx
        add     cx,85h
        push    ds
        mov     di,es
        mov     ds,di
        mov     di,4FA6h
        mov     ax,seg seg048
        mov     es,ax
        rep movsb
        pop     ds
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+0Eh]
        mov     si,word ptr [bp+10h]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     dx,si
        mov     si,4FA6h
        call    _seg003_5A87
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_4E3A endp

; init_graphics  (+4EA9)
_init_graphics proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_5AB7
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_init_graphics endp

; seg003_4EE5  (+4EE5)
; Load a palette (VIDMODE's _32F4): SI = the palette's offset in seg048, AX and BX the other
; two arguments (passed through; _32F4 does not read them).
_seg003_4EE5 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     si,word ptr [bp+6]
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_5A8D
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_4EE5 endp

; MaybeApplyPalette_seg003_4F2A  (+4F2A)
_grPageFlip proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_5A90
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_grPageFlip endp

; GraphicsCall_seg003_4F66  (+4F66)
_grSoftPageFlip proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_5A93
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_grSoftPageFlip endp

; seg003_4FA2  (+4FA2)
; Show the drawing page (VIDMODE's _333F), without switching pages.
_seg003_4FA2 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_5A96
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_4FA2 endp

; seg003_4FDE  (+4FDE)
_gr_read_pixel proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_5A99
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_gr_read_pixel endp

; seg003_5020  (+5020)
; Read the pixel at (x, y), or -1 outside the window (VIDMODE's _3964).
_seg003_5020 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_5A9C
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_5020 endp

; seg003_5062  (+5062)
_plot_pixel proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_5A9F
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_plot_pixel endp

; seg003_50A4  (+50A4)
; Plot (x, y) without clipping (VIDMODE's _392D through the jump table).
_seg003_50A4 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_5AA2
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_50A4 endp

; seg003_50E6  (+50E6)
; A solid vertical line (x, y1, y2) without clipping (VIDMODE's _39E5).
_seg003_50E6 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        xchg    cx,dx
        call    _seg003_5AAB
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_50E6 endp

; seg003_512D  (+512D)
; A vertical line (x, y1, y2) copied from the other page (VIDMODE's _39A2).
_seg003_512D proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        xchg    cx,dx
        call    _seg003_5AAE
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_512D endp

; seg003_5174  (+5174)
_set_the_color proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_5AB1
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_set_the_color endp

; init_colors  (+51B3)
_init_colors proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_5AB4
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_init_colors endp

; seg003_51EF  (+51EF)
; Set the screen mode (AX = the mode number, VIDMODE's _324C).
_seg003_51EF proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_5ABA
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_51EF endp

; seg003_522E  (+522E)
; Allocate n bytes of video memory (GRLIBF's _3A86). Returns the offset, or 0 when there is no
; room.
_seg003_522E proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     bx,word ptr [bp+6]
        mov     ax,bx
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_3A86
        jae     L525E
        mov     ax,0
L525E:
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_522E endp

; seg003_5274  (+5274)
; Give video memory back down to an offset (GRLIBF's _3A9B). Returns the offset, or 0 if it was
; below the floor.
_seg003_5274 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     bx,word ptr [bp+6]
        mov     ax,bx
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_3A9B
        jae     L52A4
        mov     ax,0
L52A4:
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_5274 endp

; DrawBitmapScreen_seg003_52BA  (+52BA)
_set_the_window proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     di,word ptr [bp+0Ch]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     word ptr ds:[503Eh],bx
        mov     word ptr ds:[503Ch],ax
        mov     word ptr ds:[5042h],di
        mov     word ptr ds:[5040h],cx
        mov     si,503Ch
        call    near ptr _seg003_5ADE
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_set_the_window endp

; seg003_5314  (+5314)
_copy_hidden_to_visible proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_3B0F
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_copy_hidden_to_visible endp

; GraphicsCall_seg003_5350  (+5350)
_copy_visible_to_hidden proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_3AFD
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_copy_visible_to_hidden endp

; seg003_538C  (+538C)
; A clipped vertical line through the vector at 5AAE (GRLIBF's _3B9A).
_seg003_538C proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        xchg    cx,dx
        call    _seg003_3B9A
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_538C endp

; seg003_53D3  (+53D3)
; hline: a clipped horizontal line (x1, y, x2; GRLIBF's _3CFF).
_seg003_53D3 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_3CFF
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_53D3 endp

; seg003_5418  (+5418)
_uhline proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_3D2E
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_uhline endp

; seg003_545D  (+545D)
; vline: a clipped vertical line (x, y1, y2; GRLIBF's _3BA1).
_seg003_545D proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        xchg    cx,dx
        call    _seg003_3BA1
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_545D endp

; seg003_54A4  (+54A4)
_uvline proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        xchg    cx,dx
        call    _seg003_3BA4
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_uvline endp

; seg003_54EB  (+54EB)
_rectangle proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     dx,si
        call    _seg003_3C9E
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_rectangle endp

; seg003_5535  (+5535)
_urectangle proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     dx,si
        call    _seg003_3CAE
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_urectangle endp

; clear_window  (+557F)
_clear_window proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_3CA3
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_clear_window endp

; box  (+55BB)
_box proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     dx,si
        call    _seg003_3BEE
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_box endp

; seg003_5605  (+5605)
; ubox: an unclipped outline rectangle (GRLIBF's _3BF1).
_seg003_5605 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     dx,si
        call    _seg003_3BF1
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_5605 endp

; seg003_564F  (+564F)
; Fill the whole window (GRLIBF's urectangle on the window's corners).
_seg003_564F proc far
        push    bp
        mov     bp,sp
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,word ptr [bp+0Ch]
        mov     bp,dx
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     dx,bp
        push    word ptr ds:[SPAN_WRITER]
        mov     word ptr ds:[SPAN_WRITER],offset _seg003_5AC6
        call    _seg003_3CAE
        pop     word ptr ds:[SPAN_WRITER]
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_564F endp

; seg003_56A9  (+56A9)
; cline: a clipped line (x1, y1, x2, y2; GRLIBG's _3F0E).
_seg003_56A9 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     dx,si
        call    _seg003_3F0E
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_56A9 endp

; seg003_56F3  (+56F3)
; polygon: CX = the vertex count, the vertices already at 415E (GRLIBI's _438D).
_seg003_56F3 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     cx,word ptr [bp+6]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_438D
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_56F3 endp

; seg003_5732  (+5732)
; upolygon: as polygon, without clipping (GRLIBI's _4134).
_seg003_5732 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     cx,word ptr [bp+6]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_4134
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_5732 endp

; seg003_5771  (+5771)
; uline: an unclipped line (GRLIBH's _3FDC).
_seg003_5771 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     dx,si
        call    _seg003_3FDC
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_5771 endp

; DrawArtToScreen_seg003_57BB  (+57BB)
_show proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,seg seg048
        mov     es,ax
        mov     ax,word ptr [bp+12h]
        mov     word ptr es:[0DC8h],ax
        mov     ax,word ptr [bp+14h]
        mov     word ptr es:[0DCAh],ax
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     di,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+10h]
        mov     bp,word ptr [bp+0Eh]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     dx,bp
        call    _seg003_2BBA
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_show endp

; DRAW_RELATED_seg003_581E  (+581E)
_seg003_581E proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,seg seg048
        mov     es,ax
        mov     ax,word ptr [bp+10h]
        mov     word ptr es:[0DC8h],ax
        mov     ax,word ptr [bp+12h]
        mov     word ptr es:[0DCAh],ax
        mov     di,word ptr [bp+6]
        mov     si,0A000h
        mov     ax,word ptr [bp+8]
        mov     bx,word ptr [bp+0Ah]
        mov     cx,word ptr [bp+0Ch]
        mov     bp,word ptr [bp+0Eh]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     dx,bp
        call    _seg003_2BD3
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_581E endp

; seg003_5881  (+5881)
_fbshow proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,seg seg048
        mov     es,ax
        xor     ax,ax
        mov     word ptr es:[0DC8h],ax
        mov     word ptr es:[0DCAh],ax
        mov     di,word ptr [bp+6]
        mov     si,word ptr [bp+8]
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+0Eh]
        mov     bp,word ptr [bp+10h]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     dx,bp
        call    _seg003_2BFA
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_fbshow endp

; seg003_58E0  (+58E0)
; Set the linear buffer bitmaps are drawn into by _5915 (VIDMODE's _33E3).
_seg003_58E0 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     di,word ptr [bp+8]
        mov     bx,word ptr [bp+0Ah]
        mov     cx,word ptr [bp+0Ch]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     dx,di
        call    _seg003_33E3
        retf
_seg003_58E0 endp

; seg003_5915  (+5915)
; Draw a bitmap into that linear buffer (VIDMODE's _2C11), with fbshow's arguments.
_seg003_5915 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,seg seg048
        mov     es,ax
        xor     ax,ax
        mov     word ptr es:[0DC8h],ax
        mov     word ptr es:[0DCAh],ax
        mov     di,word ptr [bp+6]
        mov     si,word ptr [bp+8]
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+0Eh]
        mov     bp,word ptr [bp+10h]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     dx,bp
        call    _seg003_2C11
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_5915 endp

; seg003_5974  (+5974)
_vcopyfb proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     di,word ptr [bp+0Eh]
        mov     bp,word ptr [bp+0Ch]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     dx,bp
        call    _seg003_2C28
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_vcopyfb endp

; vcopy  (+59C1)
_vcopy proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Eh]
        mov     di,word ptr [bp+10h]
        mov     bp,word ptr [bp+0Ch]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        mov     dx,bp
        call    _seg003_2C36
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
L5A11:
        lodsb
        stosb
        or      al,al
        jne     L5A11
        dec     di
        retf
_vcopy endp

; seg003_5A19  (+5A19)
; _5A19: CX = the length of the string at SI (a strlen). The far code after its ret compares two
; strings with it (repe cmpsb); L5A11, in vcopy's proc, is a far strcpy. No callers of these
; were found.
_seg003_5A19 proc near
        mov     di,si
        xor     al,al
        mov     cx,0FFFFh
        repne scasb
        mov     cx,di
        sub     cx,si
        dec     cx
        ret
        push    si
        push    di
        call    _seg003_5A19
        pop     di
        repe cmpsb
        pop     si
        retf
_seg003_5A19 endp

; seg003_5A32  (+5A32)
_fbuf_setcolor proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L4C70,dx
        mov     dx,sp
        mov     word ptr cs:L4C72,dx
        mov     ax,word ptr [bp+6]
        mov     dx,seg seg048
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA6h
        sti
        call    _seg003_EFF
        cli
        mov     dx,word ptr cs:L4C70
        mov     ss,dx
        mov     dx,word ptr cs:L4C72
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_fbuf_setcolor endp

SEG003_TEXT ends

        end
