; /* target: seg003_5A72 */
; /* opts: /ml */
;
; Graphics library module 14: the near jump table _5A72 .. _5AE4, one jmp per library entry, so
; GRCORE's C wrappers and other modules (GRLIBF, GRDISP's _5B65, VIDMODE's span writer vector at
; DS:4110) reach the routines by fixed offsets. The span writers are entries 5AC3 .. 5AE1 (see
; VIDMODE.ASM's header for the full list); _5ADE is set_the_window, entered far by GRDISP's
; _5B5D.
;
; In UW2 (UW2Decomp's src/gfx/GRCORE.ASM, seg003_0272_5278 .. 52EA) the same table ends GRCORE,
; and nothing there separates them: GRCORE's last routine ends on an odd byte (5277) and the
; table starts on the next, even. In UW1 GRCORE's last routine, fbuf_setcolor, ends at +5A70
; and one zero byte follows before the table at +5A72. TASM pads with 90h (`even`), never with
; zero; a zero byte there is TLINK's padding before a word-aligned module, so in UW1 the table
; is a module of its own. The same entries, in the same order, jump to the same routines as
; UW2's.
;
; name: descriptive (the jump table of the graphics library); no FM Towns counterpart, since the
; 32-bit build calls the routines directly.
;
; SEG003 split into its original modules: this is module 14 of UW1's 18, +5A72..+5AE6.
; Evidence for the bounds: starts after one zero byte following GRCORE's retf (above); it ends
; at the zero byte at +5AE7, GRDISP starting at +5AE8, as in UW2.
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg003_321C:near
        extrn   _seg003_321D:near
        extrn   _seg003_3249:near
        extrn   _seg003_324C:near
        extrn   _seg003_32F4:near
        extrn   _seg003_330D:near
        extrn   _seg003_333F:near
        extrn   _seg003_3357:near
        extrn   _seg003_335A:near
        extrn   _seg003_351B:near
        extrn   _seg003_358E:near
        extrn   _seg003_3604:near
        extrn   _seg003_3674:near
        extrn   _seg003_36BB:near
        extrn   _seg003_36FA:near
        extrn   _seg003_3799:near
        extrn   _seg003_3817:near
        extrn   _seg003_3857:near
        extrn   _seg003_38BC:near
        extrn   _seg003_3915:near
        extrn   _seg003_392D:near
        extrn   _seg003_3950:near
        extrn   _seg003_3958:near
        extrn   _seg003_3964:near
        extrn   _seg003_397C:near
        extrn   _seg003_39A2:near
        extrn   _seg003_39E5:near
        extrn   _seg003_3A16:near
        extrn   _seg003_3A3F:near
        extrn   _seg003_3A68:near
        extrn   _seg003_439E:near
        extrn   _seg003_43A6:near
        extrn   _seg003_43AE:near
        extrn   _seg003_43B6:near
        extrn   _seg003_43BE:near
        extrn   _seg003_440F:near
        extrn   _seg003_4448:near
        extrn   _seg003_4462:near
        extrn   _seg003_447D:near
        public  _seg003_5A72
        public  _seg003_5A75
        public  _seg003_5A78
        public  _seg003_5A81
        public  _seg003_5A87
        public  _seg003_5A8D
        public  _seg003_5A90
        public  _seg003_5A93
        public  _seg003_5A96
        public  _seg003_5A99
        public  _seg003_5A9C
        public  _seg003_5A9F
        public  _seg003_5AA2
        public  _seg003_5AAB
        public  _seg003_5AAE
        public  _seg003_5AB1
        public  _seg003_5AB4
        public  _seg003_5AB7
        public  _seg003_5ABA
        public  _seg003_5AC3
        public  _seg003_5AC6
        public  _seg003_5AC9
        public  _seg003_5ACC
        public  _seg003_5AD2
        public  _seg003_5AD5
        public  _seg003_5ADE
        public  _seg003_5AE1

; seg003_5A72  (+5A72)
_seg003_5A72 proc near
        jmp     _seg003_4462
_seg003_5A72 endp

; seg003_5A75  (+5A75)
_seg003_5A75 proc near
        jmp     _seg003_447D
_seg003_5A75 endp

; seg003_5A78  (+5A78)
_seg003_5A78 proc near
        jmp     _seg003_43B6
        jmp     _seg003_43BE
        jmp     _seg003_43AE
_seg003_5A78 endp

; seg003_5A81  (+5A81)
_seg003_5A81 proc near
        jmp     _seg003_440F
        jmp     _seg003_4448
_seg003_5A81 endp

; seg003_5A87  (+5A87)
_seg003_5A87 proc near
        jmp     _seg003_439E
        jmp     _seg003_43A6
_seg003_5A87 endp

; seg003_5A8D  (+5A8D)
_seg003_5A8D proc near
        jmp     _seg003_32F4
_seg003_5A8D endp

; seg003_5A90  (+5A90)
_seg003_5A90 proc near
        jmp     _seg003_3357
_seg003_5A90 endp

; seg003_5A93  (+5A93)
_seg003_5A93 proc near
        jmp     _seg003_335A
_seg003_5A93 endp

; seg003_5A96  (+5A96)
_seg003_5A96 proc near
        jmp     _seg003_333F
_seg003_5A96 endp

; seg003_5A99  (+5A99)
_seg003_5A99 proc near
        jmp     _seg003_397C
_seg003_5A99 endp

; seg003_5A9C  (+5A9C)
_seg003_5A9C proc near
        jmp     _seg003_3964
_seg003_5A9C endp

; seg003_5A9F  (+5A9F)
_seg003_5A9F proc near
        jmp     _seg003_3915
_seg003_5A9F endp

; seg003_5AA2  (+5AA2)
_seg003_5AA2 proc near
        jmp     _seg003_392D
        jmp     _seg003_3958
        jmp     _seg003_3950
_seg003_5AA2 endp

; seg003_5AAB  (+5AAB)
_seg003_5AAB proc near
        jmp     _seg003_39E5
_seg003_5AAB endp

; seg003_5AAE  (+5AAE)
_seg003_5AAE proc near
        jmp     _seg003_39A2
_seg003_5AAE endp

; seg003_5AB1  (+5AB1)
_seg003_5AB1 proc near
        jmp     _seg003_3A16
_seg003_5AB1 endp

; seg003_5AB4  (+5AB4)
_seg003_5AB4 proc near
        jmp     _seg003_3A3F
_seg003_5AB4 endp

; seg003_5AB7  (+5AB7)
_seg003_5AB7 proc near
        jmp     _seg003_321D
_seg003_5AB7 endp

; seg003_5ABA  (+5ABA)
_seg003_5ABA proc near
        jmp     _seg003_324C
        jmp     _seg003_3249
        jmp     _seg003_330D
_seg003_5AC3:
        jmp     _seg003_3604
_seg003_5AC6:
        jmp     _seg003_3799
_seg003_5AC9:
        jmp     _seg003_358E
_seg003_5ACC:
        jmp     _seg003_36FA
        jmp     _seg003_36BB
_seg003_5AD2:
        jmp     _seg003_38BC
_seg003_5AD5:
        jmp     _seg003_3857
        jmp     _seg003_3817
        jmp     _seg003_3674
_seg003_5ABA endp

; seg003_5ADE  (+5ADE)
_seg003_5ADE proc far
        jmp     _seg003_3A68
_seg003_5AE1:
        jmp     _seg003_351B
        jmp     _seg003_321C
_seg003_5ADE endp

SEG003_TEXT ends

        end
