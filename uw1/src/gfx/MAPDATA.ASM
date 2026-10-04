; /* target: seg003_3AC */
; /* opts: /ml */
;
; Graphics library module 3: the code-segment words the wall mapper (POLYFILL.ASM, FM Towns
; wmap_bitmap) fills as it sets up a polygon, a marker and a copy of its vertex list. Nothing
; reads them; they are probably a record of the last polygon for inspecting in a debugger, which
; POLYFILL's int 2 breaks into.
;   _3AC  12345678h, a marker
;   _3B0 .. _3CA  words POLYFILL stores with cs: overrides (bounds, vertex count, vertex
;          pointers, step values)
;   _3CC  the caller's ES
;   _3CE  a copy of the 54h-byte vertex list at 3963:0658 (six 14-byte points)
;
; UW1 against UW2: in UW2 the same words are UW2Decomp's GRMISC.ASM _76 .. _98 (dd 12345678h,
; then the words), after int 2 and 'map bitmap error' and a dword 0 at _60; UW1 has no 'map
; bitmap error', and the older wall mapper (WALLMAP.ASM) sits where UW2 has it.
;
; name: descriptive (the wall mapper's data).
;
; SEG003 split into its original modules: this is module 3 of UW1's 18, +3AC..+421.
; Evidence for the bounds: WALLMAP's two code-segment words end at +3AB (odd), and one zero
; byte follows before the marker at +3AC; TASM never pads with zero, so that is TLINK's padding
; before a word-aligned module. It ends where POLYFILL's code starts (+422). Neither wall mapper
; uses it but POLYFILL, which addresses it from outside.
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        public  _seg003_3AC
        public  _seg003_3B0
        public  _seg003_3B2
        public  _seg003_3B4
        public  _seg003_3B6
        public  _seg003_3B8
        public  _seg003_3BA
        public  _seg003_3BC
        public  _seg003_3BE
        public  _seg003_3C0
        public  _seg003_3C2
        public  _seg003_3C4
        public  _seg003_3C6
        public  _seg003_3C8
        public  _seg003_3CA
        public  _seg003_3CC
        public  _seg003_3CE

_seg003_3AC       dd      12345678h
_seg003_3B0       dw      0
_seg003_3B2       dw      0
_seg003_3B4       dw      0
_seg003_3B6       dw      0
_seg003_3B8       dw      0
_seg003_3BA       dw      0
_seg003_3BC       dw      0
_seg003_3BE       dw      0
_seg003_3C0       dw      0
_seg003_3C2       dw      0
_seg003_3C4       dw      0
_seg003_3C6       dw      0
_seg003_3C8       dw      0
_seg003_3CA       dw      0
_seg003_3CC:
        dw      0                       ; the caller's es
_seg003_3CE:
        db      54h dup (0)             ; a copy of the 54h bytes at 3963:0658

SEG003_TEXT ends

        end
