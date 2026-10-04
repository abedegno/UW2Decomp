; /* target: seg003_2BBA */
; /* opts: /ml */
;
; Graphics library module 8: the VGA itself. It sets up the game's screen mode (mode X: the
; BIOS's 320 x 200 mode 13h unchained to four planes, so there is room for two pages and spare
; video memory), keeps the row address table (Ytab) through which every routine in the library
; addresses the screen, flips pages, loads the palette, and holds the span writers
; the primitives call through DS:4110 (solid, copy, save, restore, xor, on one page or both),
; the pixel and vertical-line writers, set_the_color, init_colors and set_the_window. Its first
; proc also holds the bitmap routines (show, fbshow, vcopyfb, vcopy and two more), which clip a
; bitmap's rectangle, write one transfer record per row and call the transfer chosen in 0DC4
; (GRLIBL.ASM, GRENTRY.ASM), and a concave polygon fill.
;
; The y axis points up. _3386 builds Ytab from the highest row down: the highest row gets the
; page's start address and each lower row the next 80 bytes, and the display start is set to
; that address (_333F), so row 0 is the bottom line of the screen. Every routine in seg003, and
; the 3D renderer's frame buffer, uses this orientation.
;
; Data (seg048): 36A0 the CPU type CPUTYPE.ASM found (FD72:0110: 86h, 30h, 186h, 286h or
; 386h; from 186h up _3325 writes the palette with rep outsb, which the 8086 lacks); 36A6 the
; bytes per row (width / 4); 36AA Ytab,
; the drawing page's row addresses; 383E the other page's; 2CD6, 2CD8 the second page's start
; and a spare area after it; 2CDA the current mode; 2CDC the mode table (6 bytes a mode,
; copied to 3DEA: last x, last y, and a third word; mode 0 is 320 x 200, mode 1 320 x 400);
; 2D33 the CRTC's original maximum scan line;
; 3B62, 3CA6 the left-edge and right-edge plane masks by x; 3DE8..3DF8 the screen size and the
; clip window; 3DFA the screen segment; 3DFE the palette being loaded; 410A the offset from
; the drawing page to the other one; 410E, 410F the colour; 4110 the span writer; 4114 the
; pen's second word (a video memory offset for save and restore).
; _ShowClip (0DC6) nonzero clips bitmaps to the window; _Transparency (0DC7) nonzero skips
; colour 0 in them.
;
; GRCORE.ASM's jump table maps C entry points to these (5AC3 .. 5AE1 are the span writers):
;   5AB7 _321D init_graphics   5ABA _324C, 5ABD _3249 set mode   5AC0 _330D palette to the DAC
;   5A8D _32F4 load a palette  5A90 _3357 grPageFlip  5A93 _335A grSoftPageFlip
;   5A96 _333F show the drawing page  5A99 _397C, 5A9C _3964 read a pixel  5A9F _3915 plot
;   5AA2 _392D unclipped plot, then _3958 and _3950 read and compare
;   5AAB _39E5, 5AAE _39A2 vertical lines  5AB1 _3A16 set_the_color  5AB4 _3A3F init_colors
;   5AC3 _3604 solid  5AC6 _3799 copy from the other page  5AC9 _358E solid on both pages
;   5ACC _36FA copy onto both pages  5ACF _36BB copy whole groups from the other page
;   5AD2 _38BC, 5AD5 _3857 plane-by-plane copies with the buffer at 4114  5AD8 _3817 restore
;   5ADB _3674 save  5ADE _3A68 set_the_window  5AE1 _351B xor  5AE4 _321C nothing
;
; name: descriptive (UW2Decomp: module 5 of seg003, "video mode, palette and screen routines").
; FM Towns'
; library has the same jobs under names such as init_graphics_, vga_config, vga_focus,
; solid_ylr, save_vylr, copy_ylr, restore_vylr, solid_pixel, read_pixel_, solid_uvline,
; copy_uvline, set_the_color_, set_the_window, concave, show, fbshow, vcopyfb and vcopy, but for
; its own hardware, so the alignment here is by job, not by code.
;
; UW1 against UW2: UW2Decomp's src/gfx/VIDMODE.ASM (UW2's seg003_0272_21D4) without UW2's
; virtual screen: there is no UW2 _2977 (set up a split, scrolling screen), vscreen_focus, _2A8F
; (the CRTC line compare) or _2B2C (Ytab for the split), 15Fh bytes, and GRCORE has no
; wrappers for them. The concave fill steps rows by a constant 80 bytes (UW2 by the stride at
; 36A8) and numbers its records from 0 up (UW2 from 0C7h down). Otherwise only numbers differ:
; the library's data below 2CD0 sits 2 bytes higher than UW2's (0DC4 for 0DC2), from there on
; 2 lower (3DF2 for 3DF4), and seg019's copy of the window is at FD72:0AA0 (UW2 FD72:0C30).
;
; SEG003 split into its original modules: this is module 8 of UW1's 18, +2BBA..+3A85.
; Evidence for the bounds: it follows the zero bytes that end SCALEBM's code buffer (see
; SCALEBM.ASM); it ends at the ret at +3A85, GRLIBF starting at the next word with no padding.
; Its relocations are entries 2763-2765 of UW.EXE's relocation table (+321F, +3272, +3A79).

; The BIOS video (int 10h) functions used here.
VID_SET_MODE    equ     00h
VID_DISPLAY_CODE equ     1Ah
; The VGA ports used here.
ATTR_INDEX      equ     3C0h  ; attribute controller
SC_INDEX        equ     3C4h  ; sequencer index
SC_DATA         equ     3C5h  ; sequencer data
DAC_WRITE_INDEX equ     3C8h  ; DAC write index
GC_INDEX        equ     3CEh  ; graphics controller index
GC_DATA         equ     3CFh  ; graphics controller data
CRTC_INDEX      equ     3D4h  ; CRT controller index
INPUT_STATUS    equ     3DAh  ; input status 1 (vertical retrace)

_ShowClip       equ     0DC6h           ; seg048:0DC8, symbols.tsv (UW2 0DC6)
_Transparency   equ     0DC7h           ; seg048:0DC7, symbols.tsv (UW2 0DC7)

; The graphics library's data in seg048 (DS while the library runs), as GRLIBF.ASM's
; and VIDMODE.ASM's headers describe it. The names are ours.
XFER_RTN        equ     0DC4h   ; the bitmap row transfer routine (GRLIBL.ASM, GRENTRY.ASM)
ROW_BYTES       equ     36A6h   ; bytes per screen row (width / 4)
YTAB            equ     36AAh   ; the drawing page's row addresses (Ytab; row 0 the bottom line)
YTAB2           equ     383Eh   ; the other page's
LMASKS          equ     3B62h   ; the left-edge plane masks, by x
RMASKS          equ     3CA6h   ; the right-edge plane masks, by x
SCR_LASTX       equ     3DEAh   ; the screen's last x
SCR_LASTY       equ     3DECh   ; and last y
WIN_LEFT        equ     3DF2h   ; the clip window (y counts up, so bottom < top)
WIN_TOP         equ     3DF4h
WIN_RIGHT       equ     3DF6h
WIN_BOTTOM      equ     3DF8h
SCREEN_SEG      equ     3DFAh   ; the screen's segment
PAGE_DIFF       equ     410Ah   ; the offset from the drawing page to the other one
PEN_COLOR       equ     410Fh   ; the colour
SPAN_WRITER     equ     4110h   ; the current span writer (set_the_color, fbuf_setcolor)
PEN_WORD2       equ     4114h   ; the pen's second word

        .186

        extrn   seg063:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg003_4C74:near
        extrn   _seg003_3D42:near
        extrn   _seg003_4462:near
        extrn   _seg003_5B6C:near
        extrn   _seg003_5C61:near
        extrn   _seg003_5D72:near
        extrn   _seg003_5E85:near
        extrn   _seg003_5EDD:near
        extrn   _seg003_6015:near
        extrn   _seg003_606D:near
        extrn   _seg003_60C7:near
        extrn   _seg003_60F8:near
        extrn   _seg003_A5C:near
        extrn   _seg003_A8D:near
        extrn   _seg003_DE3:near
        public  _seg003_3A66
        public  _seg003_2BBA
        public  _seg003_2BD3
        public  _seg003_2BFA
        public  _seg003_2C11
        public  _seg003_2C28
        public  _seg003_2C36
        public  _seg003_321C
        public  _seg003_321D
        public  _seg003_3249
        public  _seg003_324C
        public  _seg003_32F4
        public  _seg003_330D
        public  _seg003_3325
        public  _seg003_333F
        public  _seg003_3357
        public  _seg003_335A
        public  _seg003_3386
        public  _seg003_33C5
        public  _seg003_33D4
        public  _seg003_33E3
        public  _seg003_33FB
        public  _seg003_347A
        public  _seg003_34C5
        public  _seg003_351B
        public  _seg003_358E
        public  _seg003_3604
        public  _seg003_3674
        public  _seg003_36BB
        public  _seg003_36FA
        public  _seg003_3799
        public  _seg003_3817
        public  _seg003_3857
        public  _seg003_38BC
        public  _seg003_3915
        public  _seg003_392D
        public  _seg003_3950
        public  _seg003_3958
        public  _seg003_3964
        public  _seg003_397C
        public  _seg003_39A2
        public  _seg003_39E5
        public  _seg003_3A16
        public  _seg003_3A3F
        public  _seg003_3A68
        public  _seg003_341C

; seg003_2BBA  (+2BBA)
; show: put a bitmap on the screen. In (from GRCORE's show): AX, BX the screen position, DI, SI
; the source offset and segment, CX, BP the size, 0DC8, 0DCA the source's x and y offset.
; Chooses the transfer by _Transparency (_5B6C opaque, _5C61 transparent, GRLIBL.ASM) and joins
; the common path at L2C7C. The labelled entries after it are the same for other sources and
; targets: _2BD3 a bitmap already in video memory (_5D72 or _5E85 by plane alignment, _5EDD
; transparent), _2BFA fbshow (into the frame buffer, GRENTRY's _A5C or _A8D), _2C11 into the
; linear buffer set by _33E3 (_60C7 or _60F8), _2C28 vcopyfb (frame buffer to screen, GRENTRY's
; _DE3) and _2C36 vcopy (screen to screen, _6015 or, for an overlapping copy to the right,
; _606D).
;
; L2C7C: clip the rectangle to the window when _ShowClip is set (moving the source offsets to
; match), then write one 8-byte record per row (y, left, right, source offset) into the table at
; 0DCD, end it, and jump to the transfer in 0DC4. L2D59 and L2E24 build the records forwards or,
; for vcopy, in the order an overlapping copy needs.
;
; After the ret at +2E7B is the concave polygon fill (FM Towns concave and uconcave, probably): clip
; with concave_shclip (GRLIBF.ASM), then build the spans of a polygon that may be concave from
; an edge list (1B86 .. 1B8C) into 2D40 and run the span writer.
_seg003_2BBA proc near
        cmp     byte ptr ds:[_Transparency],0
        jne     L2BCA
        mov     word ptr ds:[XFER_RTN],offset _seg003_5B6C
        jmp     L2C7C
L2BCA:
        mov     word ptr ds:[XFER_RTN],offset _seg003_5C61
        jmp     L2C7C
_seg003_2BD3:
        cmp     byte ptr ds:[_Transparency],0
        jne     L2BF1
        test    ax,3
        je      L2BE8
        mov     word ptr ds:[XFER_RTN],offset _seg003_5D72
        jmp     L2C7C
L2BE8:
        mov     word ptr ds:[XFER_RTN],offset _seg003_5E85
        jmp     L2C7C
L2BF1:
        mov     word ptr ds:[XFER_RTN],offset _seg003_5EDD
        jmp     L2C7C
_seg003_2BFA:
        cmp     byte ptr ds:[_Transparency],0
        jne     L2C09
        mov     word ptr ds:[XFER_RTN],offset _seg003_A5C
        jmp     short L2C7C
L2C09:
        mov     word ptr ds:[XFER_RTN],offset _seg003_A8D
        jmp     short L2C7C
_seg003_2C11:
        cmp     byte ptr ds:[_Transparency],0
        jne     L2C20
        mov     word ptr ds:[XFER_RTN],offset _seg003_60C7
        jmp     short L2C7C
L2C20:
        mov     word ptr ds:[XFER_RTN],offset _seg003_60F8
        jmp     short L2C7C
_seg003_2C28:
        mov     word ptr ds:[XFER_RTN],offset _seg003_DE3
        mov     byte ptr ds:[0DCCh],0
        jmp     L2D59
_seg003_2C36:
        cmp     bx,di
        je      L2C4C
        mov     word ptr ds:[XFER_RTN],offset _seg003_606D
        jg      L2C47
        mov     bp,offset L2D59
        jmp     short L2C61
L2C47:
        mov     bp,offset L2E24
        jmp     short L2C61
L2C4C:
        mov     bp,offset L2D59
        cmp     ax,si
        jg      L2C5B
        mov     word ptr ds:[XFER_RTN],offset _seg003_6015
        jmp     short L2C61
L2C5B:
        mov     word ptr ds:[XFER_RTN],offset _seg003_606D
L2C61:
        cmp     bp,offset L2E24
        jne     L2C6A
        sub     di,dx
        inc     di
L2C6A:
        shl     di,1
        mov     di,word ptr [di+YTAB]
        shr     si,2
        add     di,si
        mov     byte ptr ds:[0DCCh],1
        jmp     bp
L2C7C:
        mov     word ptr ds:[55EAh],si
        push    di
        mov     di,word ptr ds:[141Dh]
        and     word ptr [di],7FFFh
        mov     bp,word ptr ds:[0DC8h]
        mov     di,word ptr ds:[0DCAh]
        push    cx
        sub     cx,bp
        sub     dx,di
        cmp     byte ptr ds:[_ShowClip],0
        jne     L2C9F
        jmp     short L2CF4
L2C9F:
        mov     si,ax
        add     si,cx
        dec     si
        sub     si,word ptr ds:[WIN_LEFT]
        jge     L2CAD
        jmp     L2D55
L2CAD:
        mov     si,word ptr ds:[WIN_LEFT]
        sub     si,ax
        jle     L2CBB
        add     ax,si
        sub     cx,si
        add     bp,si
L2CBB:
        mov     si,word ptr ds:[WIN_RIGHT]
        sub     si,ax
        jge     L2CC6
        jmp     L2D55
L2CC6:
        inc     si
        cmp     cx,si
        jle     L2CCD
        mov     cx,si
L2CCD:
        mov     si,bx
        sub     si,dx
        inc     si
        sub     si,word ptr ds:[WIN_TOP]
        jg      L2D55
        mov     si,word ptr ds:[WIN_TOP]
        sub     si,bx
        jge     L2CE6
        sub     di,si
        add     dx,si
        add     bx,si
L2CE6:
        mov     si,bx
        sub     si,word ptr ds:[WIN_BOTTOM]
        jle     L2D55
        cmp     dx,si
        jle     L2CF4
        mov     dx,si
L2CF4:
        mov     si,cx
        pop     cx
        xchg    cx,dx
        neg     bx
        add     bx,0C7h
        shl     bx,3
        add     bx,0DCDh
        add     si,ax
        dec     si
        xchg    di,ax
        push    bp
        mov     bp,dx
        mul     dx
        pop     dx
        cmp     word ptr ds:[55EAh],0A000h
        je      L2D21
        add     dx,ax
        pop     ax
        add     dx,ax
        mov     ax,bx
        jmp     short L2D37
L2D21:
        add     ax,3
        shr     dx,2
        shr     ax,2
        add     dx,ax
        pop     ax
        add     dx,ax
        mov     ax,bx
        add     bp,3
        shr     bp,2
L2D37:
        mov     word ptr [bx+2],di
        mov     word ptr [bx+4],si
        mov     word ptr [bx+6],dx
        add     dx,bp
        add     bx,8
        loop    L2D37
        or      word ptr [bx],8000h
        mov     word ptr ds:[141Dh],bx
        mov     si,ax
        jmp     word ptr ds:[XFER_RTN]
L2D55:
        add     sp,4
        retn
L2D59:
        add     cx,ax
        dec     cx
        and     ax,0FFFCh
        and     cx,-4
        add     cx,4
        sub     cx,ax
        mov     si,cx
        shr     si,2
        push    si
        cmp     byte ptr ds:[_ShowClip],0
        je      L2DDD
        mov     si,word ptr ds:[WIN_TOP]
        mov     bp,bx
        sub     bp,dx
        inc     bp
        cmp     si,bp
        jge     L2D84
        jmp     L2E20
L2D84:
        sub     si,bx
        jge     L2D9D
        add     dx,si
        add     bx,si
        push    ax
        push    dx
        mov     dx,cx
        shr     dx,2
        mov     ax,si
        neg     ax
        mul     dx
        add     di,ax
        pop     dx
        pop     ax
L2D9D:
        mov     si,bx
        sub     si,word ptr ds:[WIN_BOTTOM]
        jle     L2E20
        cmp     dx,si
        jle     L2DAB
        mov     dx,si
L2DAB:
        mov     si,word ptr ds:[WIN_RIGHT]
        sub     si,ax
        jl      L2E20
        inc     si
        cmp     cx,si
        jle     L2DBA
        mov     cx,si
L2DBA:
        mov     si,word ptr ds:[WIN_LEFT]
        mov     bp,ax
        add     bp,cx
        dec     bp
        cmp     si,bp
        jg      L2E20
        sub     si,ax
        jle     L2DDD
        add     ax,si
        sub     cx,si
        mov     bp,si
        neg     bp
        and     bp,3
        add     si,bp
        sar     si,2
        add     di,si
L2DDD:
        mov     bp,word ptr ds:[141Dh]
        and     word ptr [bp],7FFFh
        add     cx,ax
        dec     cx
        neg     bx
        add     bx,0C7h
        shl     bx,3
        add     bx,0DCDh
        mov     si,bx
        pop     bp
        cmp     byte ptr ds:[0DCCh],0
        je      L2E03
        mov     bp,50h                  ; UW1: 80 bytes a row, a constant (UW2 reads the stride 36A6)
L2E03:
        mov     word ptr [bx+2],ax
        mov     word ptr [bx+4],cx
        mov     word ptr [bx+6],di
        add     di,bp
        add     bx,8
        dec     dx
        jne     L2E03
        or      word ptr [bx],8000h
        mov     word ptr ds:[141Dh],bx
        jmp     word ptr ds:[XFER_RTN]
L2E20:
        add     sp,2
        retn
L2E24:
        add     cx,ax
        dec     cx
        and     ax,0FFFCh
        and     cx,-4
        add     cx,4
        sub     cx,ax
        mov     bp,cx
        shr     bp,2
        mov     si,word ptr ds:[141Dh]
        and     word ptr [si],7FFFh
        add     cx,ax
        dec     cx
        sub     bx,dx
        inc     bx
        mov     si,0DCDh
        push    dx                 ; UW1 then steps by BP as worked out above (UW2 reloads it from 36A6)
L2E49:
        mov     word ptr [si],bx
        mov     word ptr [si+2],ax
        mov     word ptr [si+4],cx
        mov     word ptr [si+6],di
        inc     bx
        sub     di,bp
        add     si,8
        dec     dx
        jne     L2E49
        or      word ptr [si],8000h
        mov     word ptr ds:[141Dh],si
        mov     si,0DCDh
        call    word ptr ds:[XFER_RTN]
        pop     dx
        mov     bx,0DCDh
        xor     ax,ax                   ; UW1 numbers the records 0 up (UW2 0C7h down)
L2E72:
        mov     word ptr [bx],ax
        inc     ax
        add     bx,8
        dec     dx
        jne     L2E72
        retn
        call    _seg003_3D42
        mov     ax,cx
        shl     cx,1
        shl     cx,1
        mov     si,415Ch
        mov     di,si
        add     di,cx
        mov     cx,8
        rep movsw
        mov     cx,ax
        mov     word ptr ds:[1B88h],2D40h
        mov     ch,cl
        mov     cl,5
        mov     si,4160h
        mov     di,1424h
        mov     word ptr ds:[19ECh],0
L2EA9:
        mov     ax,word ptr [si]
        mov     bx,word ptr [si+2]
        cmp     bx,word ptr [si+6]
        je      L2F2C
        jg      L2EE4
        shl     ax,cl
        mov     word ptr [di+4],ax
        mov     word ptr [di+6],bx
        mov     dx,word ptr [si+4]
        shl     dx,cl
        mov     word ptr [di+2],dx
        mov     bp,word ptr [si+6]
        mov     word ptr [di],bp
        sub     ax,dx
        je      L2ED8
        cwd
        sub     bx,bp
        neg     bx
        idiv    bx
        sub     word ptr [di+2],ax
L2ED8:
        mov     word ptr [di+8],ax
        cmp     bp,word ptr [si+0Ah]
        jg      L2F12
        dec     word ptr [di]
        jmp     short L2F12
L2EE4:
        shl     ax,cl
        mov     word ptr [di+2],ax
        mov     word ptr [di],bx
        mov     dx,word ptr [si+4]
        shl     dx,cl
        mov     word ptr [di+4],dx
        mov     bp,word ptr [si+6]
        mov     word ptr [di+6],bp
        sub     dx,ax
        mov     ax,dx
        je      L2F07
        cwd
        sub     bx,bp
        idiv    bx
        sub     word ptr [di+2],ax
L2F07:
        mov     word ptr [di+8],ax
        cmp     bp,word ptr [si+0Ah]
        jl      L2F12
        inc     word ptr [di+6]
L2F12:
        add     di,0Ch
        inc     word ptr ds:[19ECh]
        add     si,4
        dec     ch
        jg      L2EA9
        jmp     L2FA4
L2F23:
        add     si,4
        dec     ch
        jne     L2F2F
        jmp     short L2FA4
L2F2C:
        mov     bp,word ptr [si-2]
L2F2F:
        mov     dx,bx
        sub     dx,word ptr [si+0Ah]
        je      L2F23
        sub     bp,bx
        neg     bp
        xor     bp,dx
        js      L2F63
        shl     ax,cl
        mov     word ptr [di+2],ax
        mov     word ptr [di+4],ax
        mov     word ptr [di],bx
        mov     word ptr [di+6],bx
        mov     word ptr [di+8],0
        add     di,0Ch
        inc     word ptr ds:[19ECh]
        add     si,4
        dec     ch
        jle     L2F61
        jmp     L2EA9
L2F61:
        jmp     short L2FA4
L2F63:
        or      dx,dx
        js      L2F9A
        shl     ax,cl
        mov     word ptr [di+2],ax
        mov     word ptr [di+4],ax
        mov     word ptr [di],bx
        mov     word ptr [di+6],bx
        mov     word ptr [di+8],0
        mov     ax,word ptr [si+4]
        shl     ax,cl
        mov     word ptr [di+0Eh],ax
        mov     word ptr [di+10h],ax
        mov     word ptr [di+0Ch],bx
        mov     word ptr [di+12h],bx
        mov     word ptr [di+14h],0
        add     di,18h
        inc     word ptr ds:[19ECh]
        inc     word ptr ds:[19ECh]
L2F9A:
        add     si,4
        dec     ch
        jle     L2FA4
        jmp     L2EA9
L2FA4:
        mov     cx,word ptr ds:[19ECh]
        cmp     cx,2
        jge     L2FAE
        retn
L2FAE:
        mov     si,1424h
        xor     ax,ax
        mov     bp,0Ch
        mov     dl,0FFh
L2FB8:
        mov     bx,word ptr [si]
        mov     byte ptr [bx+1B8Ch],dl
        cmp     ax,bx
        jg      L2FC4
        mov     ax,bx
L2FC4:
        shl     bx,1
        mov     di,si
        xchg    di,word ptr [bx+19F2h]
        mov     word ptr [si+0Ah],di
        mov     bx,word ptr [si+6]
        mov     byte ptr [bx+1B8Bh],dl
        add     si,bp
        loop    L2FB8
        mov     word ptr ds:[1422h],ax
        mov     word ptr ds:[1B86h],ax
        mov     word ptr ds:[19EEh],0
L2FE6:
        mov     bx,word ptr ds:[1B86h]
        mov     di,19C4h
        mov     si,di
        mov     cx,word ptr ds:[19EEh]
        jcxz    L3000
L2FF5:
        lodsw
        mov     bp,ax
        cmp     bx,word ptr [bp+6]
        jl      L2FFE
        stosw
L2FFE:
        loop    L2FF5
L3000:
        mov     byte ptr [bx+1B8Ch],cl
        mov     si,bx
        shl     si,1
        mov     ax,word ptr [si+19F2h]
        mov     word ptr [si+19F2h],cx
        or      ax,ax
        je      L3028
L3014:
        mov     si,ax
        stosw
        mov     ax,word ptr [si+0Ah]
        or      ax,ax
        je      L3028
        mov     si,ax
        stosw
        mov     ax,word ptr [si+0Ah]
        or      ax,ax
        jne     L3014
L3028:
        mov     dx,di
        sub     dx,19C4h
        shr     dx,1
        mov     word ptr ds:[19EEh],dx
        or      dx,dx
        jne     L303B
        jmp     L320D
L303B:
        cmp     dl,2
        je      L3043
        jmp     L313E
L3043:
        mov     bx,word ptr ds:[19C4h]
        mov     bp,word ptr ds:[19C6h]
        mov     ax,word ptr [bx+2]
        add     ax,word ptr [bx+8]
        sub     ax,word ptr [bp+8]
        cmp     ax,word ptr [bp+2]
        jne     L305F
        mov     ax,word ptr [bx+4]
        cmp     ax,word ptr [bp+4]
L305F:
        jl      L3063
        xchg    bx,bp
L3063:
        mov     ax,word ptr [bx+4]
        cmp     ax,word ptr [bp+4]
        jle     L306E
        jmp     L318E
L306E:
        mov     di,word ptr ds:[1B88h]
        mov     cl,5
        mov     word ptr ds:[19C4h],bx
        mov     word ptr ds:[19C6h],bp
        mov     dx,word ptr [bx+2]
        mov     si,word ptr [bx+8]
        mov     ax,word ptr [bp+8]
        mov     bp,word ptr [bp+2]
        sub     dx,10h
        add     bp,10h
        mov     bx,word ptr ds:[1B86h]
        mov     word ptr ds:[1420h],ax
        cmp     si,ax
        jle     L30EB
L3099:
        mov     ax,bx
        stosw
        add     dx,si
        add     bp,word ptr ds:[1420h]
        mov     ax,dx
        sar     ax,cl
        stosw
        mov     ax,bp
        sar     ax,cl
        stosw
        dec     bx
        test    byte ptr [bx+1B8Ch],cl
        jne     L311F
        cmp     dx,bp
        jg      L30D5
        mov     ax,bx
        stosw
        add     dx,si
        add     bp,word ptr ds:[1420h]
        mov     ax,dx
        sar     ax,cl
        stosw
        mov     ax,bp
        sar     ax,cl
        stosw
        dec     bx
        test    byte ptr [bx+1B8Ch],cl
        jne     L311F
        cmp     dx,bp
        jle     L3099
L30D5:
        xchg    dx,bp
        xchg    si,word ptr ds:[1420h]
        mov     ax,word ptr ds:[19C4h]
        xchg    ax,word ptr ds:[19C6h]
        mov     word ptr ds:[19C4h],ax
        sub     dx,20h
        add     bp,20h
L30EB:
        mov     ax,bx
        stosw
        add     dx,si
        add     bp,word ptr ds:[1420h]
        mov     ax,dx
        sar     ax,cl
        stosw
        mov     ax,bp
        sar     ax,cl
        stosw
        dec     bx
        test    byte ptr [bx+1B8Ch],cl
        jne     L311F
        mov     ax,bx
        stosw
        add     dx,si
        add     bp,word ptr ds:[1420h]
        mov     ax,dx
        sar     ax,cl
        stosw
        mov     ax,bp
        sar     ax,cl
        stosw
        dec     bx
        test    byte ptr [bx+1B8Ch],cl
        je      L30EB
L311F:
        add     dx,10h
        sub     bp,10h
        mov     word ptr ds:[1B88h],di
        mov     word ptr ds:[1B86h],bx
        mov     si,word ptr ds:[19C4h]
        mov     word ptr [si+2],dx
        mov     si,word ptr ds:[19C6h]
        mov     word ptr [si+2],bp
        jmp     L2FE6
L313E:
        test    dl,1
        je      L318E
        mov     di,word ptr ds:[1B88h]
        mov     ax,bx
        stosw
        mov     cl,dl
        xor     ch,ch
        mov     dx,8000h
        mov     bp,7FFFh
        mov     si,19C4h
L3157:
        lodsw
        mov     bx,ax
        mov     ax,word ptr [bx+8]
        add     ax,word ptr [bx+2]
        mov     word ptr [bx+2],ax
        cmp     dx,ax
        jg      L3169
        mov     dx,ax
L3169:
        cmp     bp,ax
        jl      L316F
        mov     bp,ax
L316F:
        loop    L3157
        mov     cl,5
        mov     ax,bp
        sub     ax,10h
        sar     ax,cl
        stosw
        mov     ax,dx
        add     ax,10h
        sar     ax,cl
        stosw
        mov     word ptr ds:[1B88h],di
        dec     word ptr ds:[1B86h]
        jmp     L2FE6
L318E:
        mov     dh,dl
        dec     dh
        xor     ch,ch
L3194:
        mov     cl,dh
        mov     si,19C4h
        xor     bl,bl
L319B:
        lodsw
        mov     di,ax
        mov     bp,word ptr [si]
        mov     ax,word ptr [di+2]
        add     ax,word ptr [di+8]
        sub     ax,word ptr [bp+8]
        cmp     ax,word ptr [bp+2]
        jl      L31B5
        inc     bl
        mov     word ptr [si-2],bp
        mov     word ptr [si],di
L31B5:
        loop    L319B
        or      bl,bl
        je      L31BF
        dec     dh
        jne     L3194
L31BF:
        mov     di,word ptr ds:[1B88h]
        mov     si,19C4h
        mov     bp,word ptr ds:[1B86h]
        mov     dh,dl
        shr     dh,1
        mov     cl,5
L31D0:
        mov     ax,bp
        stosw
        lodsw
        mov     bx,ax
        mov     ax,word ptr [bx+2]
        add     ax,word ptr [bx+8]
        mov     word ptr [bx+2],ax
        sub     ax,10h
        sar     ax,cl
        stosw
        lodsw
        mov     bx,ax
        mov     ax,word ptr [bx+2]
        add     ax,word ptr [bx+8]
        mov     word ptr [bx+2],ax
        add     ax,10h
        sar     ax,cl
        stosw
        dec     dh
        jne     L31D0
        mov     word ptr ds:[1B88h],di
        dec     bp
        mov     word ptr ds:[1B86h],bp
        test    byte ptr [bp+1B8Ch],cl
        je      L318E
        jmp     L2FE6
L320D:
        mov     si,2D40h
        mov     bx,word ptr ds:[1B88h]
        mov     word ptr [bx],0FFFFh
        jmp     word ptr ds:[SPAN_WRITER]
_seg003_321C:
        retn
_seg003_2BBA endp

; seg003_321D  (+321D)
; init_graphics: needs a VGA (_33FB, else returns with carry set). Records the CPU type from
; FD72:0110, sets mode X (SetVideoMode), saves the CRTC maximum scan line, builds the edge mask
; tables, and falls into the mode set. _3249 sets the mode number AX first; _324C applies 3DE8:
; copies the mode's size to 3DEA and to FD72:0AA8, sets the scan doubling (the saved value for
; mode 0, 1 line per row otherwise), sets the window to the whole screen, works out the row
; length and the two pages and the video memory left after them (4104, 4108), builds Ytab,
; initialises the pens, flips to page 0, and builds the text masks (GRLIBI's _4462).
_seg003_321D proc near
        push    ds
        mov     ax,seg seg063
        mov     ds,ax
        mov     ax,word ptr ds:[110h]
        pop     ds
        mov     word ptr ds:[36A0h],ax
        call    _seg003_33FB
        jae     L3230
        ret
L3230:
        mov     word ptr ds:[36A2h],0FFFFh
        call    _seg003_341C
        mov     dx,CRTC_INDEX
        mov     al,9
        out     dx,al
        inc     dx
        in      al,dx
        mov     byte ptr ds:[2D33h],al
        call    _seg003_33C5
        jmp     short _seg003_324C
_seg003_3249:
        mov     word ptr ds:[3DE8h],ax
_seg003_324C:
        mov     bx,word ptr ds:[3DE8h]
        cmp     bx,word ptr ds:[2CDAh]
        jne     L3257
        ret
L3257:
        mov     word ptr ds:[2CDAh],bx
        shl     bx,1
        mov     ax,bx
        shl     bx,1
        add     bx,ax
        lea     si,[bx+2CDCh]
        mov     di,3DEAh
        movsw
        movsw
        movsw
        mov     si,3DE8h
        push    es
        mov     bx,seg seg063
        mov     es,bx
        mov     di,0AA8h
        movsw
        movsw
        movsw
        pop     es
        test    ax,ax
        jne     L328E
        mov     dx,CRTC_INDEX
        mov     al,9
        out     dx,al
        inc     dx
        mov     al,byte ptr ds:[2D33h]
        out     dx,al
        jmp     short L3299
L328E:
        mov     dx,CRTC_INDEX
        mov     al,9
        out     dx,al
        inc     dx
        in      al,dx
        and     al,0E0h
        out     dx,al
L3299:
        sub     ax,ax
        mov     word ptr ds:[WIN_LEFT],ax
        mov     word ptr ds:[WIN_BOTTOM],ax
        mov     ax,word ptr ds:[SCR_LASTX]
        mov     word ptr ds:[WIN_RIGHT],ax
        mov     ax,word ptr ds:[SCR_LASTY]
        mov     word ptr ds:[WIN_TOP],ax
        mov     si,3DF2h
        call    _seg003_3A68
        mov     ax,word ptr ds:[SCR_LASTX]
        inc     ax
        shr     ax,1
        shr     ax,1
        mov     word ptr ds:[ROW_BYTES],ax
        mov     ax,word ptr ds:[SCR_LASTY]
        inc     ax
        mul     word ptr ds:[ROW_BYTES]
        mov     bx,ax
        add     ax,0FFh
        sub     al,al
        mov     word ptr ds:[2CD6h],ax
        add     ax,bx
        mov     word ptr ds:[2CD8h],ax
        add     ax,word ptr ds:[ROW_BYTES]
        mov     word ptr ds:[4104h],ax
        mov     word ptr ds:[4108h],ax
        sub     ax,ax
        call    _seg003_3386
        call    _seg003_3A3F
        call    _seg003_3357
        call    _seg003_4C74
        mov     ax,0
        call    _seg003_4462
        ret
_seg003_321D endp

; seg003_32F4  (+32F4)
; _32F4: when 3DFC is set, clear it, copy a palette (183h words) from SI to 3DFE and load it.
; _330D: wait for vertical retrace and write 256 colours from SI to the DAC (_3325).
_seg003_32F4 proc near
        cmp     word ptr ds:[3DFCh],0
        jne     L32FC
        ret
L32FC:
        mov     word ptr ds:[3DFCh],0
        mov     di,3DFEh
        mov     cx,183h
        rep movsw
        mov     si,3DFEh
_seg003_330D:
        cli
        mov     dx,INPUT_STATUS
L3311:
        in      al,dx
        and     al,8
        je      L3311
        mov     al,0
        mov     dx,DAC_WRITE_INDEX
        out     dx,al
        inc     dx
        mov     cx,100h
        call    _seg003_3325
        sti
        ret
_seg003_32F4 endp

; seg003_3325  (+3325)
; _3325: write CX colours from SI to the DAC data port (DX = 3C9h), with rep outsb on a 186 or
; later (36A0 at least 0BAh), else with single out instructions, since the 8086 has no outsb.
_seg003_3325 proc near
        cmp     word ptr ds:[36A0h],0BAh
        jl      L3336
        mov     ax,cx
        shl     ax,1
        add     cx,ax
        rep outsb
        ret
L3336:
        lodsb
        out     dx,al
        lodsb
        out     dx,al
        lodsb
        out     dx,al
        loop    L3336
        ret
_seg003_3325 endp

; seg003_333F  (+333F)
; _333F: set the CRTC display start (registers 0Ch, 0Dh) to the drawing page, Ytab of the
; highest row.
_seg003_333F proc near
        mov     bx,word ptr ds:[SCR_LASTY]
        shl     bx,1
        mov     ax,word ptr [bx+YTAB]
        mov     bx,ax
        mov     dx,CRTC_INDEX
        mov     al,0Ch
        out     dx,ax
        mov     ah,bl
        inc     al
        out     dx,ax
        ret
_seg003_333F endp

; seg003_3357  (+3357)
; grPageFlip: display the drawing page (_333F), then fall into grSoftPageFlip (_335A): keep the
; current row table as the displayed page's (383E), work out the other page's start and the
; offset between them (410A), and rebuild Ytab for it, so drawing goes to the page not on
; screen.
_seg003_3357 proc near
        call    _seg003_333F
_seg003_335A:
        mov     si,36AAh
        mov     di,383Eh
        mov     cx,word ptr ds:[SCR_LASTY]
        inc     cx
        rep movsw
        mov     ax,word ptr ds:[2CD6h]
        mov     bx,word ptr ds:[SCR_LASTY]
        shl     bx,1
        sub     ax,word ptr [bx+YTAB]
        mov     bx,ax
        shl     bx,1
        neg     bx
        add     bx,word ptr ds:[2CD6h]
        mov     word ptr ds:[PAGE_DIFF],bx
        call    _seg003_3386
        ret
_seg003_3357 endp

; seg003_3386  (+3386)
; _3386: build Ytab from AX: the highest row (3DEC) gets AX and each lower row 36A6 more (ten
; rows a pass). Marks 2D1A as -1.
_seg003_3386 proc near
        mov     di,word ptr ds:[SCR_LASTY]
        shl     di,1
        add     di,36AAh
        mov     bx,word ptr ds:[ROW_BYTES]
        mov     cx,word ptr ds:[SCR_LASTY]
        inc     cx
        std
L339A:
        stosw
        add     ax,bx
        stosw
        add     ax,bx
        stosw
        add     ax,bx
        stosw
        add     ax,bx
        stosw
        add     ax,bx
        stosw
        add     ax,bx
        stosw
        add     ax,bx
        stosw
        add     ax,bx
        stosw
        add     ax,bx
        stosw
        add     ax,bx
        sub     cx,0Ah
        ja      L339A
        cld
        mov     word ptr ds:[2D1Ah],0FFFFh
        ret
_seg003_3386 endp

; seg003_33C5  (+33C5)
; _33C5: build the edge plane mask tables: 3B62 from the four bytes at 2D2A and 3CA6 from those
; at 2D2F, each repeated 80 times (_33D4), so 3B62[x] and 3CA6[x] are the planes at and right
; of, and at and left of, pixel x within its group of four.
_seg003_33C5 proc near
        mov     si,2D2Ah
        mov     di,3B62h
        call    _seg003_33D4
        mov     si,2D2Fh
        mov     di,3CA6h
_seg003_33C5 endp

; seg003_33D4  (+33D4)
_seg003_33D4 proc near
        mov     cx,50h
L33D7:
        push    cx
        push    si
        mov     cx,4
        rep movsb
        pop     si
        pop     cx
        loop    L33D7
        ret
_seg003_33D4 endp

; seg003_33E3  (+33E3)
; _33E3: set the linear buffer _2C11 draws into (GRCORE's _58E0): DX its segment (39CE), and a
; row table at 39D0 for CX rows of stride BX starting at AX, highest row first.
_seg003_33E3 proc near
        mov     word ptr ds:[39CEh],dx
        mov     di,39D0h
        mov     dx,cx
        dec     dx
        shl     dx,1
        shl     dx,1
        add     di,dx
        std
L33F4:
        stosw
        add     ax,bx
        loop    L33F4
        cld
        ret
_seg003_33E3 endp

; seg003_33FB  (+33FB)
; _33FB: is there a VGA? int 10h, AX = 1A00h (read display combination): carry clear for an
; active VGA (codes 7 or 8), set otherwise.
_seg003_33FB proc near
        mov     ax,VID_DISPLAY_CODE shl 8
        int     10h
        cmp     al,1Ah
        jne     L341A
        cmp     bl,0Bh
        je      L341A
        cmp     bl,0Ch
        je      L341A
        cmp     bl,7
        je      L3418
        cmp     bl,8
        jne     L341A
L3418:
        clc
        ret
L341A:
        stc
        ret
_seg003_33FB endp

; seg003_341C  (+341C)
; SetVideoMode: mode 13h through the BIOS, then unchained (mode X): chain-4 off in the
; sequencer's memory mode, odd/even off in the graphics controller, bit mask and map mask all
; on, the whole 256 KB cleared, and the CRTC switched from doubleword to byte addressing. Leaves
; the sequencer index at the map mask.
_seg003_341C proc near
        mov     ax,(VID_SET_MODE shl 8) + 13h
        int     10h
        mov     dx,SC_INDEX
        mov     al,4
        out     dx,al
        inc     dx
        in      al,dx
        and     al,0F7h
        or      al,4
        out     dx,al
        mov     dx,GC_INDEX
        mov     al,5
        out     dx,al
        inc     dx
        in      al,dx
        and     al,0EFh
        out     dx,al
        dec     dx
        mov     al,6
        out     dx,al
        inc     dx
        in      al,dx
        and     al,0FDh
        out     dx,al
        mov     ax,0FF08h
        mov     dx,GC_INDEX
        out     dx,ax
        mov     dx,SC_INDEX
        mov     ax,0F02h
        out     dx,ax
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
        sub     di,di
        mov     ax,di
        mov     cx,8000h
        rep stosw
        pop     es
        mov     dx,CRTC_INDEX
        mov     al,14h
        out     dx,al
        inc     dx
        in      al,dx
        and     al,0BFh
        out     dx,al
        dec     dx
        mov     al,17h
        out     dx,al
        inc     dx
        in      al,dx
        or      al,40h
        out     dx,al
        mov     dx,SC_INDEX
        mov     al,2
        out     dx,al
        ret
_seg003_341C endp

; seg003_347A  (+347A)
; _347A: guard the new window's edges for the unclipped writers: the Ytab entries just above and
; below the window point at the spare area 2CD8, and the edge mask entries just left and right
; of it get partial masks (2D2B, 2D2E). _34C5 undoes this for the old window. Both are called by
; set_the_window.
;
; The span writers follow (in 2C44's proc). Each takes SI = a list of 6-byte spans (y, left x,
; right x) ended by a y with its top bit set, and writes the left and right partial groups with
; the edge masks and the whole groups between with all four planes:
;   _351B  XOR colour 0Fh into the spans (graphics controller function XOR, latches loaded
;          first), probably for a highlight or cursor
;   _358E  solid fill on the drawing page (_3604), then the same on the other page (383E)
_seg003_347A proc near
        mov     ax,word ptr ds:[WIN_TOP]
        shl     ax,1
        add     ax,36ACh
        mov     bx,ax
        mov     ax,word ptr ds:[2CD8h]
        xchg    ax,word ptr [bx]
        mov     ax,word ptr ds:[WIN_BOTTOM]
        shl     ax,1
        add     ax,36A8h
        mov     bx,ax
        mov     ax,word ptr ds:[2CD8h]
        xchg    ax,word ptr [bx]
        mov     ax,3B62h
        mov     bx,word ptr ds:[WIN_LEFT]
        dec     bx
        add     ax,bx
        mov     bp,ax
        and     bx,3
        mov     al,byte ptr [bx+2D2Bh]
        mov     byte ptr [bp],al
        mov     ax,3CA6h
        mov     bx,word ptr ds:[WIN_RIGHT]
        inc     bx
        add     ax,bx
        mov     bp,ax
        and     bx,3
        mov     al,byte ptr [bx+2D2Eh]
        mov     byte ptr [bp],al
        ret
_seg003_347A endp

; seg003_34C5  (+34C5)
_seg003_34C5 proc near
        mov     ax,3B62h
        mov     bx,word ptr ds:[WIN_LEFT]
        dec     bx
        add     ax,bx
        mov     bp,ax
        and     bx,3
        mov     al,byte ptr [bx+2D2Ah]
        mov     byte ptr [bp],al
        mov     ax,3CA6h
        mov     bx,word ptr ds:[WIN_RIGHT]
        inc     bx
        add     ax,bx
        mov     bp,ax
        and     bx,3
        mov     al,byte ptr [bx+2D2Fh]
        mov     byte ptr [bp],al
        mov     bx,word ptr ds:[WIN_TOP]
        shl     bx,1
        mov     ax,word ptr [bx+YTAB]
        sub     ax,word ptr ds:[ROW_BYTES]
        mov     word ptr [bx+36ACh],ax
        mov     bx,word ptr ds:[WIN_BOTTOM]
        shl     bx,1
        mov     ax,word ptr [bx+YTAB]
        add     ax,word ptr ds:[ROW_BYTES]
        mov     word ptr [bx+36A8h],ax
        ret
        ret
        ret
        ret
        ret
        ret
_seg003_351B:
        mov     ax,1803h
        mov     dx,GC_INDEX
        out     dx,ax
        mov     dx,SC_DATA
        mov     bl,0Fh
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
        jmp     short L353B
L352E:
        add     di,word ptr [bp+YTAB]
        and     al,bh
        out     dx,al
        mov     al,bl
        test    byte ptr es:[di],al
        stosb
L353B:
        lodsw
        shl     ax,1
        jb      L3582
        mov     cx,ax
        lodsw
        mov     di,ax
        lodsw
        mov     bp,ax
        mov     bh,byte ptr [bp+RMASKS]
        mov     al,byte ptr [di+LMASKS]
        xchg    cx,bp
        sar     di,1
        sar     di,1
        sar     cx,1
        sar     cx,1
        sub     cx,di
        jle     L352E
        out     dx,al
        add     di,word ptr [bp+YTAB]
        mov     al,bl
        test    byte ptr es:[di],al
        stosb
        dec     cx
        jcxz    L3577
        mov     al,0Fh
        out     dx,al
        mov     al,bl
L3571:
        test    byte ptr es:[di],al
        stosb
        loop    L3571
L3577:
        mov     al,bh
        out     dx,al
        mov     al,bl
        test    byte ptr es:[di],al
        stosb
        jmp     L353B
L3582:
        mov     ax,3
        mov     dx,GC_INDEX
        out     dx,ax
        mov     al,8
        out     dx,al
        pop     es
        ret
_seg003_358E:
        push    si
        call    _seg003_3604
        pop     si
        jmp     short L359A
L3595:
        pop     es
        ret
        call    _seg003_4C74
L359A:
        mov     dx,SC_DATA
        mov     bl,byte ptr ds:[PEN_COLOR]
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
        jmp     short L35C4
L35A8:
        shl     al,1
        shr     bh,1
        not     al
        not     bh
        xchg    al,bh
        add     di,cx
        neg     cx
        jmp     short L35E7
L35B8:
        jl      L35A8
        add     di,word ptr [bp+YTAB2]
        and     al,bh
        out     dx,al
        mov     al,bl
        stosb
L35C4:
        lodsw
        shl     ax,1
        jb      L3595
        mov     cx,ax
        lodsw
        mov     di,ax
        lodsw
        mov     bp,ax
        mov     bh,byte ptr [bp+RMASKS]
        mov     al,byte ptr [di+LMASKS]
        xchg    cx,bp
        sar     di,1
        sar     di,1
        sar     cx,1
        sar     cx,1
        sub     cx,di
        jle     L35B8
L35E7:
        out     dx,al
        add     di,word ptr [bp+YTAB2]
        mov     al,bl
        stosb
        dec     cx
        mov     al,0Fh
        out     dx,al
        mov     al,bl
        rep stosb
        mov     al,bh
        out     dx,al
        mov     al,bl
        stosb
        jmp     L35C4
L35FF:
        pop     es
        ret
        call    _seg003_4C74
_seg003_34C5 endp

; seg003_3604  (+3604)
; _3604: fill the spans with the colour in 410F (FM solid_ylr). The labels after it:
;   _3674  save: copy the spans' whole groups to video memory at 4114 through the latches
;   _36BB  copy the spans' whole groups from the other page (410A) through the latches
;   _36FA  copy from the other page (_3799), then the same onto the displayed page (410C)
_seg003_3604 proc near
        mov     dx,SC_DATA
        mov     bl,byte ptr ds:[PEN_COLOR]
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
        jmp     short L362E
L3612:
        shl     al,1
        shr     bh,1
        not     al
        not     bh
        xchg    al,bh
        add     di,cx
        neg     cx
        jmp     short L3651
L3622:
        jl      L3612
        add     di,word ptr [bp+YTAB]
        and     al,bh
        out     dx,al
        mov     al,bl
        stosb
L362E:
        lodsw
        shl     ax,1
        jb      L35FF
        mov     cx,ax
        lodsw
        mov     di,ax
        lodsw
        mov     bp,ax
        mov     bh,byte ptr [bp+RMASKS]
        mov     al,byte ptr [di+LMASKS]
        xchg    cx,bp
        sar     di,1
        sar     di,1
        sar     cx,1
        sar     cx,1
        sub     cx,di
        jle     L3622
L3651:
        out     dx,al
        add     di,word ptr [bp+YTAB]
        mov     al,bl
        stosb
        dec     cx
        mov     al,0Fh
        out     dx,al
        mov     al,bl
        rep stosb
        mov     al,bh
        out     dx,al
        mov     al,bl
        stosb
        jmp     L362E
L3669:
        mov     al,0FFh
        mov     dx,GC_DATA
        out     dx,al
        pop     es
        ret
        call    _seg003_4C74
_seg003_3674:
        mov     ax,0
        mov     dx,GC_DATA
        out     dx,al
        mov     dx,SC_DATA
        mov     al,0Fh
        out     dx,al
        mov     bx,si
        mov     di,word ptr ds:[PEN_WORD2]
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
L368C:
        mov     bp,word ptr [bx]
        shl     bp,1
        jb      L3669
        mov     si,word ptr [bx+2]
        mov     cx,word ptr [bx+4]
        add     bx,6
        sar     si,1
        sar     si,1
        sar     cx,1
        sar     cx,1
        sub     cx,si
        inc     cx
        add     si,word ptr [bp+YTAB]
        rep movs byte ptr es:[di],byte ptr es:[si]
        jmp     L368C
L36AF:
        mov     ax,0FFh
        mov     dx,GC_DATA
        out     dx,al
        pop     es
        ret
        call    _seg003_4C74
_seg003_36BB:
        mov     ax,0
        mov     dx,GC_DATA
        out     dx,al
        mov     dx,SC_DATA
        mov     al,0Fh
        out     dx,al
        mov     bx,si
        mov     dx,word ptr ds:[PAGE_DIFF]
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
L36D3:
        mov     bp,word ptr [bx]
        shl     bp,1
        jb      L36AF
        mov     si,word ptr [bx+2]
        mov     cx,word ptr [bx+4]
        add     bx,6
        sar     si,1
        sar     si,1
        sar     cx,1
        sar     cx,1
        sub     cx,si
        inc     cx
        add     si,word ptr [bp+YTAB]
        mov     di,si
        add     si,dx
        rep movs byte ptr es:[di],byte ptr es:[si]
        jmp     L36D3
_seg003_36FA:
        push    si
        call    _seg003_3799
        pop     si
        mov     ax,word ptr ds:[PAGE_DIFF]
        add     ax,word ptr ds:[YTAB]
        sub     ax,word ptr ds:[YTAB2]
        mov     word ptr ds:[410Ch],ax
        jmp     short L371B
L370F:
        mov     ax,0FFh
        mov     dx,GC_DATA
        out     dx,al
        pop     es
        ret
        call    _seg003_4C74
L371B:
        mov     ax,0
        mov     dx,GC_DATA
        out     dx,al
        mov     dx,SC_DATA
        mov     bx,si
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
        jmp     short L374D
L372E:
        shl     al,1
        shr     ah,1
        not     ax
        xchg    al,ah
        add     di,cx
        neg     cx
        jmp     short L3772
L373C:
        jl      L372E
        add     di,word ptr [si+YTAB2]
        mov     bp,word ptr ds:[410Ch]
        and     al,ah
        out     dx,al
        mov     al,byte ptr es:[bp+di]
        stosb
L374D:
        mov     si,word ptr [bx]
        shl     si,1
        jb      L370F
        mov     di,word ptr [bx+2]
        mov     bp,word ptr [bx+4]
        add     bx,6
        mov     al,byte ptr [di+LMASKS]
        mov     ah,byte ptr [bp+RMASKS]
        sar     di,1
        sar     di,1
        mov     cx,bp
        sar     cx,1
        sar     cx,1
        sub     cx,di
        jle     L373C
L3772:
        add     di,word ptr [si+YTAB2]
        mov     si,di
        add     si,word ptr ds:[410Ch]
        out     dx,al
        movs    byte ptr es:[di],byte ptr es:[si]
        dec     cx
        mov     al,0Fh
        out     dx,al
        rep movs byte ptr es:[di],byte ptr es:[si]
        mov     al,ah
        out     dx,al
        movs    byte ptr es:[di],byte ptr es:[si]
        jmp     L374D
L378D:
        mov     ax,0FFh
        mov     dx,GC_DATA
        out     dx,al
        pop     es
        ret
        call    _seg003_4C74
_seg003_3604 endp

; seg003_3799  (+3799)
; _3799: copy the spans from the other page (410A away) into the drawing page through the
; latches, edge masks included (FM copy_ylr). The labels after it:
;   _3817  restore: copy the spans' whole groups back from video memory at 4114
;   _3857, _38BC  plane-by-plane copies between the spans and the area at 4114, one plane at a
;          time (map mask or read map select); probably the save and restore of a pen whose
;          area is not in video memory, not traced
_seg003_3799 proc near
        mov     ax,0
        mov     dx,GC_DATA
        out     dx,al
        mov     dx,SC_DATA
        mov     bx,si
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
        jmp     short L37CB
L37AC:
        shl     al,1
        shr     ah,1
        not     ax
        xchg    al,ah
        add     di,cx
        neg     cx
        jmp     short L37F0
L37BA:
        jl      L37AC
        add     di,word ptr [si+YTAB]
        mov     bp,word ptr ds:[PAGE_DIFF]
        and     al,ah
        out     dx,al
        mov     al,byte ptr es:[bp+di]
        stosb
L37CB:
        mov     si,word ptr [bx]
        shl     si,1
        jb      L378D
        mov     di,word ptr [bx+2]
        mov     bp,word ptr [bx+4]
        add     bx,6
        mov     al,byte ptr [di+LMASKS]
        mov     ah,byte ptr [bp+RMASKS]
        sar     di,1
        sar     di,1
        mov     cx,bp
        sar     cx,1
        sar     cx,1
        sub     cx,di
        jle     L37BA
L37F0:
        add     di,word ptr [si+YTAB]
        mov     si,di
        add     si,word ptr ds:[PAGE_DIFF]
        out     dx,al
        movs    byte ptr es:[di],byte ptr es:[si]
        dec     cx
        mov     al,0Fh
        out     dx,al
        rep movs byte ptr es:[di],byte ptr es:[si]
        mov     al,ah
        out     dx,al
        movs    byte ptr es:[di],byte ptr es:[si]
        jmp     L37CB
L380B:
        mov     ax,0FFh
        mov     dx,GC_DATA
        out     dx,al
        pop     es
        ret
        call    _seg003_4C74
_seg003_3817:
        mov     ax,0
        mov     dx,GC_DATA
        out     dx,al
        mov     dx,SC_DATA
        mov     al,0Fh
        out     dx,al
        mov     bx,si
        mov     si,word ptr ds:[PEN_WORD2]
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
L382F:
        mov     bp,word ptr [bx]
        shl     bp,1
        jb      L380B
        mov     di,word ptr [bx+2]
        mov     cx,word ptr [bx+4]
        add     bx,6
        sar     di,1
        sar     di,1
        sar     cx,1
        sar     cx,1
        sub     cx,di
        inc     cx
        add     di,word ptr [bp+YTAB]
        rep movs byte ptr es:[di],byte ptr es:[si]
        jmp     L382F
L3852:
        pop     es
        ret
        call    _seg003_4C74
_seg003_3857:
        mov     dx,SC_DATA
        mov     al,0Fh
        out     dx,al
        mov     dx,SC_INDEX
        mov     al,2
        out     dx,al
        inc     dx
        mov     bp,si
        mov     di,word ptr ds:[PEN_WORD2]
        push    ds
        mov     ds,word ptr ds:[SCREEN_SEG]
L386F:
        mov     bx,word ptr [bp]
        shl     bx,1
        jb      L3852
        mov     si,word ptr [bp+2]
        mov     cx,word ptr [bp+4]
        add     bp,6
        sar     si,1
        sar     si,1
        sar     cx,1
        sar     cx,1
        sub     cx,si
        inc     cx
        add     si,word ptr ss:[bx+36AAh]
        mov     bx,cx
        mov     al,1
        out     dx,al
        rep movsb
        sub     si,bx
        mov     cx,bx
        shl     al,1
        out     dx,al
        rep movsb
        sub     si,bx
        mov     cx,bx
        shl     al,1
        out     dx,al
        rep movsb
        sub     si,bx
        mov     cx,bx
        shl     al,1
        out     dx,al
        rep movsb
        jmp     L386F
L38B3:
        pop     es
        dec     dx
        mov     al,8
        out     dx,al
        ret
        call    _seg003_4C74
_seg003_38BC:
        mov     dx,SC_DATA
        mov     al,0Fh
        out     dx,al
        mov     dx,GC_INDEX
        mov     al,4
        out     dx,al
        inc     dx
        mov     bp,si
        mov     si,word ptr ds:[PEN_WORD2]
        push    ds
        mov     ds,word ptr ds:[SCREEN_SEG]
L38D4:
        mov     bx,word ptr [bp]
        shl     bx,1
        jb      L38B3
        mov     di,word ptr [bp+2]
        mov     cx,word ptr [bp+4]
        add     bp,6
        sar     di,1
        sar     di,1
        sar     cx,1
        sar     cx,1
        sub     cx,di
        inc     cx
        add     di,word ptr ss:[bx+36AAh]
        mov     bx,cx
        sub     ax,ax
        out     dx,al
        rep movsb
        sub     di,bx
        mov     cx,bx
        inc     ax
        out     dx,al
        rep movsb
        sub     di,bx
        mov     cx,bx
        inc     ax
        out     dx,al
        rep movsb
        sub     di,bx
        mov     cx,bx
        inc     ax
        out     dx,al
        rep movsb
        jmp     L38D4
_seg003_3799 endp

; seg003_3915  (+3915)
; _3915 (plot): plot (AX, BX) in the colour 410F if it is inside the window. _392D is the
; unclipped entry: map mask from the edge tables, address from Ytab. _3950 and _3958 read a
; pixel (clipped or not) and compare it with CL, leaving the flags.
_seg003_3915 proc near
        cmp     bx,word ptr ds:[WIN_TOP]
        jg      L394F
        cmp     bx,word ptr ds:[WIN_BOTTOM]
        jl      L394F
        cmp     ax,word ptr ds:[WIN_RIGHT]
        jg      L394F
        cmp     ax,word ptr ds:[WIN_LEFT]
        jl      L394F
_seg003_392D:
        mov     dx,SC_DATA
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
        mov     di,ax
        mov     al,byte ptr [di+LMASKS]
        and     al,byte ptr [di+RMASKS]
        shr     di,1
        shr     di,1
        shl     bx,1
        add     di,word ptr [bx+YTAB]
        out     dx,al
        mov     al,byte ptr ds:[PEN_COLOR]
        stosb
        pop     es
L394F:
        ret
_seg003_3950:
        mov     ch,cl
        call    _seg003_3964
        cmp     al,ch
        ret
_seg003_3958:
        mov     ch,cl
        call    _seg003_397C
        cmp     al,ch
        ret
L3960:
        mov     ax,0FFFFh
        ret
_seg003_3915 endp

; seg003_3964  (+3964)
; _3964: read the pixel at (AX, BX), or AX = -1 outside the window; falls into _397C.
_seg003_3964 proc near
        cmp     bx,word ptr ds:[WIN_TOP]
        jg      L3960
        cmp     bx,word ptr ds:[WIN_BOTTOM]
        jl      L3960
        cmp     ax,word ptr ds:[WIN_RIGHT]
        jg      L3960
        cmp     ax,word ptr ds:[WIN_LEFT]
        jl      L3960
_seg003_3964 endp

; seg003_397C  (+397C)
; _397C: read the pixel at (AX, BX): select its plane for reading (graphics controller register
; 4) and read the byte. Out: AL = the colour. Leaves the graphics controller index at 8.
_seg003_397C proc near
        mov     di,ax
        mov     ah,al
        and     ah,3
        mov     al,4
        mov     dx,GC_INDEX
        out     dx,ax
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
        shr     di,1
        shr     di,1
        shl     bx,1
        add     di,word ptr [bx+YTAB]
        mov     ah,byte ptr es:[di]
        pop     es
        mov     al,8
        out     dx,al
        mov     al,ah
        ret
_seg003_397C endp

; seg003_39A2  (+39A2)
; _39A2: copy a vertical line (x AX, rows BX down to DX) from the other page (410A) through the
; latches (FM copy_uvline).
_seg003_39A2 proc near
        mov     di,ax
        mov     cx,bx
        sub     cx,dx
        inc     cx
        mov     ax,0
        mov     dx,GC_DATA
        out     dx,al
        mov     al,byte ptr [di+LMASKS]
        and     al,byte ptr [di+RMASKS]
        mov     dx,SC_DATA
        out     dx,al
        shr     di,1
        shr     di,1
        shl     bx,1
        add     di,word ptr [bx+YTAB]
        mov     bx,word ptr ds:[PAGE_DIFF]
        mov     dx,word ptr ds:[ROW_BYTES]
        dec     dx
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
L39D4:
        mov     al,byte ptr es:[bx+di]
        stosb
        add     di,dx
        loop    L39D4
        pop     es
        mov     ax,0FFh
        mov     dx,GC_DATA
        out     dx,al
        ret
_seg003_39A2 endp

; seg003_39E5  (+39E5)
; _39E5: a solid vertical line (x AX, rows BX down to DX) in the colour 410F (FM solid_uvline).
_seg003_39E5 proc near
        mov     di,ax
        mov     cx,bx
        sub     cx,dx
        inc     cx
        mov     al,byte ptr [di+LMASKS]
        and     al,byte ptr [di+RMASKS]
        mov     dx,SC_DATA
        out     dx,al
        shr     di,1
        shr     di,1
        shl     bx,1
        add     di,word ptr [bx+YTAB]
        mov     bx,word ptr ds:[ROW_BYTES]
        dec     bx
        mov     al,byte ptr ds:[PEN_COLOR]
        push    es
        mov     es,word ptr ds:[SCREEN_SEG]
L3A0F:
        stosb
        add     di,bx
        loop    L3A0F
        pop     es
        ret
_seg003_39E5 endp

; seg003_3A16  (+3A16)
; set_the_color: AX = a pen number (negative: no change). As GRENTRY's fbuf_setcolor, for the
; screen: colour bytes 410E and 4114 from the tables at 21E and 434, and the span writer from
; the table at 2CEC by the pen's mode, less 5044 (the concave-window variants).
_seg003_3A16 proc near
        shl     ax,1
        jb      L3A3E
        mov     bx,ax
        mov     ax,word ptr [bx+21Eh]
        mov     word ptr ds:[410Eh],ax
        mov     ax,word ptr [bx+434h]
        mov     word ptr ds:[PEN_WORD2],ax
        db      8Bh,9Fh,8h,0h                   ; match: mov bx,word ptr [bx+8]: disp16, the table at seg048:0008 (TASM gives disp8 for a number, and a label there would need that segment)
        cmp     bx,14h
        jae     L3A3E
        mov     ax,word ptr [bx+2CECh]
        sub     ax,word ptr ds:[5044h]
        mov     word ptr ds:[SPAN_WRITER],ax
L3A3E:
        ret
_seg003_3A16 endp

; seg003_3A3F  (+3A3F)
; init_colors: run each pen's set-up routine (the table at 2D02, by the pen's mode) for every
; pen, then store DI (lowered by any pen that took video memory) as the video memory limit 4106.
_seg003_3A3F proc near
        mov     di,0FFFFh
        call    _seg003_3A66
        mov     cx,21Eh
        db      81h,0E9h,8h,0h                  ; match: sub cx,8 with an imm16: the offset of the same table, which TASM gives as 83 for a number
        shr     cx,1
        sub     bp,bp
L3A50:
        db      8Bh,9Eh,8h,0h                   ; match: mov bx,word ptr [bp+8]: disp16, the table at seg048:0008 (TASM gives disp8 for a number, and a label there would need that segment)
        cmp     bx,14h
        jae     L3A5D
        call    word ptr [bx+2D02h]
L3A5D:
        inc     bp
        inc     bp
        loop    L3A50
        mov     word ptr ds:[4106h],di
        ret
_seg003_3A3F endp

; seg003_3A66  (+3A66)
_seg003_3A66 proc near
        ret
        ret
_seg003_3A66 endp

; seg003_3A68  (+3A68)
; set_the_window (far through 5ADE): SI = four words, left, top, right, bottom (y counting up).
; Removes the old window's edge guards (_34C5), copies the window to 3DF2, guards the new one
; (_347A), and copies it to FD72:0AA0, where the 3D renderer's render_3d reads it
; (INSTANCE.ASM).
_seg003_3A68 proc near
        call    _seg003_34C5
        push    si
        mov     di,3DF2h
        movsw
        movsw
        movsw
        movsw
        pop     si
        call    _seg003_347A
        push    es
        mov     ax,seg seg063
        mov     es,ax
        mov     di,0AA0h
        movsw
        movsw
        movsw
        movsw
        pop     es
        ret
_seg003_3A68 endp

SEG003_TEXT ends

        end
