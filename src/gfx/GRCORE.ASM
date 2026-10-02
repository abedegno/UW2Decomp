; /* target: seg003_0272_43F0 */
; /* opts: /ml */
;
; The graphics library's C entry points: setup_font, string_to_screen, string_width,
; init_graphics, grPageFlip, plot_pixel, set_the_color, set_the_window, rectangle, box, show,
; fbshow, vcopyfb, fbuf_setcolor and the rest, and the table of near jumps at _5278. Its _DATA
; is the graphics globals C uses: palette, Color_data_ptr, foreground_color, background_color,
; cur_font, bytefont, wtop, wbot, wright, wleft, pixel_color and Ytab, mostly far pointers
; into seg_370D.
;
; SEG003 split into its original modules: this is module 10 of 14, +43F0..+52EE of the segment
; (file 7840h), a module of the second library (UWLIB.LIB).
; Procs: nullsub_2, seg003_0272_43F5, setup_font, string_to_screen, seg003_0272_44DC,
; string_width, seg003_0272_45BA, init_graphics, seg003_0272_4665, grPageFlip, grSoftPageFlip,
; seg003_0272_4722, gr_read_pixel, seg003_0272_47A0, plot_pixel, seg003_0272_4824,
; seg003_0272_4866, seg003_0272_48AD, set_the_color, init_colors, seg003_0272_496F,
; seg003_0272_49AE, seg003_0272_49F4, seg003_0272_4A3A, vscreen_focus, set_the_window,
; copy_hidden_to_visible, copy_visible_to_hidden, seg003_0272_4B93, seg003_0272_4BDA, uhline,
; seg003_0272_4C64, uvline, rectangle, urectangle, clear_window, box, seg003_0272_4E0C,
; seg003_0272_4E56, seg003_0272_4EB0, seg003_0272_4EFA, seg003_0272_4F39, seg003_0272_4F78,
; show, seg003_0272_5025, fbshow, seg003_0272_50E7, seg003_0272_511C, vcopyfb, vcopy,
; seg003_0272_5220, fbuf_setcolor, seg003_0272_5278, seg003_0272_527B, seg003_0272_527E,
; seg003_0272_5287, seg003_0272_528D, seg003_0272_5293, seg003_0272_5296, seg003_0272_5299,
; seg003_0272_529C, seg003_0272_529F, seg003_0272_52A2, seg003_0272_52A5, seg003_0272_52A8,
; seg003_0272_52B1, seg003_0272_52B4, seg003_0272_52B7, seg003_0272_52BA, seg003_0272_52BD,
; seg003_0272_52C0, seg003_0272_52E4.
; Evidence for the bounds: starts after one zero byte following a ret: TLINK's padding before
; a word-aligned module (TASM 2.0's `even` pads with 90h).
; Its relocations are entries 2509-2566 of UW2.EXE's relocation table.
; Its _DATA is DS:21A4..2220 (relocation entries 2478-2508), defined before the code because
; those entries come before the code's.

        .186

        extrn   seg_370D:far

_DATA   segment word public 'DATA'
        dd      seg_370D+4156h
        dd      seg_370D+4FA0h
_palette label dword
        dd      seg_370D+5040h
        dd      seg_370D
        dd      seg_370D+216h
_Color_data_ptr label dword
        dd      seg_370D+42Ch
        dd      seg_370D+642h
        dd      seg_370D+2D2Eh
_foreground_color label dword
        dd      seg_370D+2D32h
_background_color label dword
        dd      seg_370D+2D33h
_cur_font label dword
        dd      seg_370D+4D00h
_bytefont label dword
        dd      seg_370D+1C4Ch
        dd      seg_370D+3DF8h
        dd      seg_370D+3DF6h
_wtop label dword
        dd      seg_370D+3DEEh
_wbot label dword
        dd      seg_370D+3DF2h
_wright label dword
        dd      seg_370D+3DF0h
_wleft label dword
        dd      seg_370D+3DECh
        dd      seg_370D+3DECh
_pixel_color label dword
        dd      seg_370D+4109h
_dseg_67d6_21F4 label dword
        dd      seg_370D+3832h
_Ytab label dword
        dd      seg_370D+36A4h
        dd      seg_370D+410Eh
        dd      seg_370D+4102h
        dd      seg_370D+4100h
        dd      seg_370D+36A0h
        dd      seg_370D+503Eh
        dd      seg_370D+503Eh
        dd      seg_370D+503Eh
        dd      seg_370D+503Eh
        dd      seg_370D+369Ah
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'CODE'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg003_0272_21D4:near
        extrn   _seg003_0272_21ED:near
        extrn   _seg003_0272_2214:near
        extrn   _seg003_0272_222B:near
        extrn   _seg003_0272_2242:near
        extrn   _seg003_0272_2250:near
        extrn   _seg003_0272_283C:near
        extrn   _seg003_0272_283D:near
        extrn   _seg003_0272_2869:near
        extrn   _seg003_0272_286C:near
        extrn   _seg003_0272_2914:near
        extrn   _seg003_0272_292D:near
        extrn   _seg003_0272_295F:near
        extrn   _seg003_0272_2977:near
        extrn   _seg003_0272_2A0D:near
        extrn   _seg003_0272_2ABE:near
        extrn   _seg003_0272_2AC1:near
        extrn   _seg003_0272_2B62:near
        extrn   _seg003_0272_2C9A:near
        extrn   _seg003_0272_2D0D:near
        extrn   _seg003_0272_2D83:near
        extrn   _seg003_0272_2DF3:near
        extrn   _seg003_0272_2E3A:near
        extrn   _seg003_0272_2E79:near
        extrn   _seg003_0272_2F18:near
        extrn   _seg003_0272_2F96:near
        extrn   _seg003_0272_2FD6:near
        extrn   _seg003_0272_303B:near
        extrn   _seg003_0272_3094:near
        extrn   _seg003_0272_30AC:near
        extrn   _seg003_0272_30CF:near
        extrn   _seg003_0272_30D7:near
        extrn   _seg003_0272_30E3:near
        extrn   _seg003_0272_30FB:near
        extrn   _seg003_0272_3121:near
        extrn   _seg003_0272_3164:near
        extrn   _seg003_0272_3195:near
        extrn   _seg003_0272_31BE:near
        extrn   _seg003_0272_31E7:near
        extrn   _seg003_0272_3206:near
        extrn   _seg003_0272_321B:near
        extrn   _seg003_0272_327D:near
        extrn   _seg003_0272_328F:near
        extrn   _seg003_0272_331A:near
        extrn   _seg003_0272_3321:near
        extrn   _seg003_0272_3324:near
        extrn   _seg003_0272_336E:near
        extrn   _seg003_0272_3371:near
        extrn   _seg003_0272_341E:near
        extrn   _seg003_0272_3423:near
        extrn   _seg003_0272_342E:near
        extrn   _seg003_0272_347F:near
        extrn   _seg003_0272_34AE:near
        extrn   _seg003_0272_368E:near
        extrn   _seg003_0272_375C:near
        extrn   _seg003_0272_38B4:near
        extrn   _seg003_0272_3B0D:near
        extrn   _seg003_0272_3B1E:near
        extrn   _seg003_0272_3B26:near
        extrn   _seg003_0272_3B2E:near
        extrn   _seg003_0272_3B36:near
        extrn   _seg003_0272_3B3E:near
        extrn   _seg003_0272_3B8F:near
        extrn   _seg003_0272_3BC8:near
        extrn   _seg003_0272_3BE2:near
        extrn   _seg003_0272_3BFD:near
        extrn   _seg003_0272_43C5:near
        extrn   _seg003_0272_B9B:near
        public  _background_color
        public  _bytefont
        public  _Color_data_ptr
        public  _cur_font
        public  _dseg_67d6_21F4
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
        public  _nullsub_2
        public  _plot_pixel
        public  _rectangle
        public  _seg003_0272_43F5
        public  _seg003_0272_44DC
        public  _seg003_0272_45BA
        public  _seg003_0272_4665
        public  _seg003_0272_4722
        public  _seg003_0272_47A0
        public  _seg003_0272_4824
        public  _seg003_0272_4866
        public  _seg003_0272_48AD
        public  _seg003_0272_496F
        public  _seg003_0272_49AE
        public  _seg003_0272_49F4
        public  _seg003_0272_4A3A
        public  _seg003_0272_4B93
        public  _seg003_0272_4BDA
        public  _seg003_0272_4C64
        public  _seg003_0272_4E0C
        public  _seg003_0272_4E56
        public  _seg003_0272_4EB0
        public  _seg003_0272_4EFA
        public  _seg003_0272_4F39
        public  _seg003_0272_4F78
        public  _seg003_0272_5025
        public  _seg003_0272_50E7
        public  _seg003_0272_511C
        public  _seg003_0272_5220
        public  _seg003_0272_5278
        public  _seg003_0272_527B
        public  _seg003_0272_527E
        public  _seg003_0272_5287
        public  _seg003_0272_528D
        public  _seg003_0272_5293
        public  _seg003_0272_5296
        public  _seg003_0272_5299
        public  _seg003_0272_529C
        public  _seg003_0272_529F
        public  _seg003_0272_52A2
        public  _seg003_0272_52A5
        public  _seg003_0272_52A8
        public  _seg003_0272_52B1
        public  _seg003_0272_52B4
        public  _seg003_0272_52B7
        public  _seg003_0272_52BA
        public  _seg003_0272_52BD
        public  _seg003_0272_52C0
        public  _seg003_0272_52C9
        public  _seg003_0272_52CC
        public  _seg003_0272_52CF
        public  _seg003_0272_52D2
        public  _seg003_0272_52D8
        public  _seg003_0272_52DB
        public  _seg003_0272_52E4
        public  _seg003_0272_52E7
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
        public  _vscreen_focus

L43F0   dw      0                       ; the caller's ss and sp
L43F2   dw      0

; nullsub_2  (+43F4)
_nullsub_2 proc near
        ret
_nullsub_2 endp

; seg003_0272_43F5  (+43F5): no caller found; switches to the private stack and calls
; through the vector at _seg003_0272_5278
_seg003_0272_43F5 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_5278
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_43F5 endp

; setup_font  (+4431)
_setup_font proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_527B
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_setup_font endp

; WriteTextToScreen_seg003_0272_446D  (+446D)
_string_to_screen proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
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
        mov     di,4FA8h
        mov     ax,seg seg_370D
        mov     es,ax
        rep movsb
        pop     ds
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+0Eh]
        mov     si,word ptr [bp+10h]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        mov     si,4FA8h
        call    _seg003_0272_527E
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_string_to_screen endp

; seg003_0272_44DC  (+44DC)
_seg003_0272_44DC proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
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
        mov     di,4FA8h
        mov     ax,seg seg_370D
        mov     es,ax
        rep movsb
        pop     ds
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+0Eh]
        mov     si,word ptr [bp+10h]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        mov     si,4FA8h
        call    _seg003_0272_5287
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_44DC endp

; WritingTextToScreen_seg003_0272_454B  (+454B)
_string_width proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
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
        mov     di,4FA8h
        mov     ax,seg seg_370D
        mov     es,ax
        rep movsb
        pop     ds
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+0Eh]
        mov     si,word ptr [bp+10h]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        mov     si,4FA8h
        call    _seg003_0272_43C5
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_string_width endp

; seg003_0272_45BA  (+45BA)
_seg003_0272_45BA proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
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
        mov     di,4FA8h
        mov     ax,seg seg_370D
        mov     es,ax
        rep movsb
        pop     ds
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+0Eh]
        mov     si,word ptr [bp+10h]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        mov     si,4FA8h
        call    _seg003_0272_528D
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_45BA endp

; init_graphics  (+4629)
_init_graphics proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_52BD
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_init_graphics endp

; seg003_0272_4665  (+4665)
_seg003_0272_4665 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     si,word ptr [bp+6]
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_5293
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4665 endp

; MaybeApplyPalette_seg003_0272_46AA  (+46AA)
_grPageFlip proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_5296
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_grPageFlip endp

; GraphicsCall_seg003_0272_46E6  (+46E6)
_grSoftPageFlip proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_5299
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_grSoftPageFlip endp

; seg003_0272_4722  (+4722)
_seg003_0272_4722 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_529C
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4722 endp

; seg003_0272_475E  (+475E)
_gr_read_pixel proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_529F
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_gr_read_pixel endp

; seg003_0272_47A0  (+47A0)
_seg003_0272_47A0 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_52A2
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_47A0 endp

; seg003_0272_47E2  (+47E2)
_plot_pixel proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_52A5
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_plot_pixel endp

; seg003_0272_4824  (+4824)
_seg003_0272_4824 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_52A8
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4824 endp

; seg003_0272_4866  (+4866)
_seg003_0272_4866 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        xchg    cx,dx
        call    _seg003_0272_52B1
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4866 endp

; seg003_0272_48AD  (+48AD)
_seg003_0272_48AD proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        xchg    cx,dx
        call    _seg003_0272_52B4
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_48AD endp

; seg003_0272_48F4  (+48F4)
_set_the_color proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_52B7
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_set_the_color endp

; init_colors  (+4933)
_init_colors proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_52BA
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_init_colors endp

; seg003_0272_496F  (+496F)
_seg003_0272_496F proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_52C0
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_496F endp

; seg003_0272_49AE  (+49AE)
_seg003_0272_49AE proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     bx,word ptr [bp+6]
        mov     ax,bx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_3206
        jae     L49DE
        mov     ax,0
L49DE:
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_49AE endp

; seg003_0272_49F4  (+49F4)
_seg003_0272_49F4 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     bx,word ptr [bp+6]
        mov     ax,bx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_321B
        jae     L4A24
        mov     ax,0
L4A24:
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_49F4 endp

; seg003_0272_4A3A  (+4A3A)
_seg003_0272_4A3A proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_2977
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4A3A endp

; seg003_0272_4A7F  (+4A7F)
_vscreen_focus proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_2A0D
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_vscreen_focus endp

; DrawBitmapScreen_seg003_0272_4AC1  (+4AC1)
_set_the_window proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     di,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     word ptr ds:[5040h],bx
        mov     word ptr ds:[503Eh],ax
        mov     word ptr ds:[5044h],di
        mov     word ptr ds:[5042h],cx
        mov     si,503Eh
        call    near ptr _seg003_0272_52E4
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_set_the_window endp

; seg003_0272_4B1B  (+4B1B)
_copy_hidden_to_visible proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_328F
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_copy_hidden_to_visible endp

; GraphicsCall_seg003_0272_4B57  (+4B57)
_copy_visible_to_hidden proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_327D
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_copy_visible_to_hidden endp

; seg003_0272_4B93  (+4B93)
_seg003_0272_4B93 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        xchg    cx,dx
        call    _seg003_0272_331A
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4B93 endp

; seg003_0272_4BDA  (+4BDA)
_seg003_0272_4BDA proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_347F
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4BDA endp

; seg003_0272_4C1F  (+4C1F)
_uhline proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_34AE
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_uhline endp

; seg003_0272_4C64  (+4C64)
_seg003_0272_4C64 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        xchg    cx,dx
        call    _seg003_0272_3321
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4C64 endp

; seg003_0272_4CAB  (+4CAB)
_uvline proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        xchg    cx,dx
        call    _seg003_0272_3324
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_uvline endp

; seg003_0272_4CF2  (+4CF2)
_rectangle proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        call    _seg003_0272_341E
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_rectangle endp

; seg003_0272_4D3C  (+4D3C)
_urectangle proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        call    _seg003_0272_342E
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_urectangle endp

; clear_window  (+4D86)
_clear_window proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_3423
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_clear_window endp

; box  (+4DC2)
_box proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        call    _seg003_0272_336E
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_box endp

; seg003_0272_4E0C  (+4E0C)
_seg003_0272_4E0C proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        call    _seg003_0272_3371
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4E0C endp

; seg003_0272_4E56  (+4E56)
_seg003_0272_4E56 proc far
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
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,bp
        push    word ptr ds:[4112h]
        mov     word ptr ds:[4112h],offset _seg003_0272_52CC
        call    _seg003_0272_342E
        pop     word ptr ds:[4112h]
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4E56 endp

; seg003_0272_4EB0  (+4EB0)
_seg003_0272_4EB0 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        call    _seg003_0272_368E
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4EB0 endp

; seg003_0272_4EFA  (+4EFA)
_seg003_0272_4EFA proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     cx,word ptr [bp+6]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_3B0D
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4EFA endp

; seg003_0272_4F39  (+4F39)
_seg003_0272_4F39 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     cx,word ptr [bp+6]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_38B4
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4F39 endp

; seg003_0272_4F78  (+4F78)
_seg003_0272_4F78 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,si
        call    _seg003_0272_375C
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_4F78 endp

; DrawArtToScreen_seg003_0272_4FC2  (+4FC2)
_show proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,seg seg_370D
        mov     es,ax
        mov     ax,word ptr [bp+12h]
        mov     word ptr es:[0DC6h],ax
        mov     ax,word ptr [bp+14h]
        mov     word ptr es:[0DC8h],ax
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     di,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+10h]
        mov     bp,word ptr [bp+0Eh]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,bp
        call    _seg003_0272_21D4
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_show endp

; DRAW_RELATED_seg003_0272_5025  (+5025)
_seg003_0272_5025 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,seg seg_370D
        mov     es,ax
        mov     ax,word ptr [bp+10h]
        mov     word ptr es:[0DC6h],ax
        mov     ax,word ptr [bp+12h]
        mov     word ptr es:[0DC8h],ax
        mov     di,word ptr [bp+6]
        mov     si,0A000h
        mov     ax,word ptr [bp+8]
        mov     bx,word ptr [bp+0Ah]
        mov     cx,word ptr [bp+0Ch]
        mov     bp,word ptr [bp+0Eh]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,bp
        call    _seg003_0272_21ED
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_5025 endp

; seg003_0272_5088  (+5088)
_fbshow proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,seg seg_370D
        mov     es,ax
        xor     ax,ax
        mov     word ptr es:[0DC6h],ax
        mov     word ptr es:[0DC8h],ax
        mov     di,word ptr [bp+6]
        mov     si,word ptr [bp+8]
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+0Eh]
        mov     bp,word ptr [bp+10h]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,bp
        call    _seg003_0272_2214
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_fbshow endp

; seg003_0272_50E7  (+50E7)
_seg003_0272_50E7 proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     di,word ptr [bp+8]
        mov     bx,word ptr [bp+0Ah]
        mov     cx,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,di
        call    _seg003_0272_2B62
        retf
_seg003_0272_50E7 endp

; seg003_0272_511C  (+511C)
_seg003_0272_511C proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,seg seg_370D
        mov     es,ax
        xor     ax,ax
        mov     word ptr es:[0DC6h],ax
        mov     word ptr es:[0DC8h],ax
        mov     di,word ptr [bp+6]
        mov     si,word ptr [bp+8]
        mov     ax,word ptr [bp+0Ah]
        mov     bx,word ptr [bp+0Ch]
        mov     cx,word ptr [bp+0Eh]
        mov     bp,word ptr [bp+10h]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,bp
        call    _seg003_0272_222B
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_seg003_0272_511C endp

; seg003_0272_517B  (+517B)
_vcopyfb proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     di,word ptr [bp+0Eh]
        mov     bp,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,bp
        call    _seg003_0272_2242
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_vcopyfb endp

; vcopy  (+51C8)
_vcopy proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     cx,word ptr [bp+0Ah]
        mov     si,word ptr [bp+0Eh]
        mov     di,word ptr [bp+10h]
        mov     bp,word ptr [bp+0Ch]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        mov     dx,bp
        call    _seg003_0272_2250
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
L5218:
        lodsb
        stosb
        or      al,al
        jne     L5218
        dec     di
        retf
_vcopy endp

; seg003_0272_5220  (+5220)
_seg003_0272_5220 proc near
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
        call    _seg003_0272_5220
        pop     di
        repe cmpsb
        pop     si
        retf
_seg003_0272_5220 endp

; seg003_0272_5239  (+5239)
_fbuf_setcolor proc far
        push    bp
        mov     bp,sp
        push    di
        push    si
        push    ds
        push    es
        mov     dx,ss
        mov     word ptr cs:L43F0,dx
        mov     dx,sp
        mov     word ptr cs:L43F2,dx
        mov     ax,word ptr [bp+6]
        mov     dx,seg seg_370D
        mov     ds,dx
        mov     es,dx
        cli
        mov     ss,dx
        mov     sp,4FA8h
        sti
        call    _seg003_0272_B9B
        cli
        mov     dx,word ptr cs:L43F0
        mov     ss,dx
        mov     dx,word ptr cs:L43F2
        mov     sp,dx
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     bp
        retf
_fbuf_setcolor endp

; seg003_0272_5278  (+5278)
_seg003_0272_5278 proc near
        jmp     _seg003_0272_3BE2
_seg003_0272_5278 endp

; seg003_0272_527B  (+527B)
_seg003_0272_527B proc near
        jmp     _seg003_0272_3BFD
_seg003_0272_527B endp

; seg003_0272_527E  (+527E)
_seg003_0272_527E proc near
        jmp     _seg003_0272_3B36
        jmp     _seg003_0272_3B3E
        jmp     _seg003_0272_3B2E
_seg003_0272_527E endp

; seg003_0272_5287  (+5287)
_seg003_0272_5287 proc near
        jmp     _seg003_0272_3B8F
        jmp     _seg003_0272_3BC8
_seg003_0272_5287 endp

; seg003_0272_528D  (+528D)
_seg003_0272_528D proc near
        jmp     _seg003_0272_3B1E
        jmp     _seg003_0272_3B26
_seg003_0272_528D endp

; seg003_0272_5293  (+5293)
_seg003_0272_5293 proc near
        jmp     _seg003_0272_2914
_seg003_0272_5293 endp

; seg003_0272_5296  (+5296)
_seg003_0272_5296 proc near
        jmp     _seg003_0272_2ABE
_seg003_0272_5296 endp

; seg003_0272_5299  (+5299)
_seg003_0272_5299 proc near
        jmp     _seg003_0272_2AC1
_seg003_0272_5299 endp

; seg003_0272_529C  (+529C)
_seg003_0272_529C proc near
        jmp     _seg003_0272_295F
_seg003_0272_529C endp

; seg003_0272_529F  (+529F)
_seg003_0272_529F proc near
        jmp     _seg003_0272_30FB
_seg003_0272_529F endp

; seg003_0272_52A2  (+52A2)
_seg003_0272_52A2 proc near
        jmp     _seg003_0272_30E3
_seg003_0272_52A2 endp

; seg003_0272_52A5  (+52A5)
_seg003_0272_52A5 proc near
        jmp     _seg003_0272_3094
_seg003_0272_52A5 endp

; seg003_0272_52A8  (+52A8)
_seg003_0272_52A8 proc near
        jmp     _seg003_0272_30AC
        jmp     _seg003_0272_30D7
        jmp     _seg003_0272_30CF
_seg003_0272_52A8 endp

; seg003_0272_52B1  (+52B1)
_seg003_0272_52B1 proc near
        jmp     _seg003_0272_3164
_seg003_0272_52B1 endp

; seg003_0272_52B4  (+52B4)
_seg003_0272_52B4 proc near
        jmp     _seg003_0272_3121
_seg003_0272_52B4 endp

; seg003_0272_52B7  (+52B7)
_seg003_0272_52B7 proc near
        jmp     _seg003_0272_3195
_seg003_0272_52B7 endp

; seg003_0272_52BA  (+52BA)
_seg003_0272_52BA proc near
        jmp     _seg003_0272_31BE
_seg003_0272_52BA endp

; seg003_0272_52BD  (+52BD)
_seg003_0272_52BD proc near
        jmp     _seg003_0272_283D
_seg003_0272_52BD endp

; seg003_0272_52C0  (+52C0)
_seg003_0272_52C0 proc near
        jmp     _seg003_0272_286C
        jmp     _seg003_0272_2869
        jmp     _seg003_0272_292D
_seg003_0272_52C9:
        jmp     _seg003_0272_2D83
_seg003_0272_52CC:
        jmp     _seg003_0272_2F18
_seg003_0272_52CF:
        jmp     _seg003_0272_2D0D
_seg003_0272_52D2:
        jmp     _seg003_0272_2E79
        jmp     _seg003_0272_2E3A
_seg003_0272_52D8:
        jmp     _seg003_0272_303B
_seg003_0272_52DB:
        jmp     _seg003_0272_2FD6
        jmp     _seg003_0272_2F96
        jmp     _seg003_0272_2DF3
_seg003_0272_52C0 endp

; seg003_0272_52E4  (+52E4)
_seg003_0272_52E4 proc far
        jmp     _seg003_0272_31E7
_seg003_0272_52E7:
        jmp     _seg003_0272_2C9A
        jmp     _seg003_0272_283C
_seg003_0272_52E4 endp

SEG003_TEXT ends

        end
