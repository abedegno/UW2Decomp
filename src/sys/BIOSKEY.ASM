; /* target: seg021_22FD_220 */
; /* opts: /ml */
;
; The BIOS keyboard (int 16h): read a key, flush the buffer, and far thunks for C.
;
; SEG021 split into its original modules: this is module 4 of 17, +220..+240 of the segment
; (file 23F20h), a module of the second library (UWLIB.LIB).
; Procs: seg021_22FD_220, seg021_22FD_224, seg021_22FD_228, seg021_22FD_236.
; Evidence for the bounds: starts on a paragraph after 5 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG021_TEXT segment para public 'CODE'
        assume  cs:SEG021_TEXT, ds:DGROUP, ss:DGROUP
        public  _seg021_22FD_220
        public  _seg021_22FD_224
        public  _seg021_22FD_228
        public  _seg021_22FD_236

; seg021_22FD_220  (+220)
_seg021_22FD_220 proc far
        call    L022E
        retf
_seg021_22FD_220 endp

; seg021_22FD_224  (+224)
_seg021_22FD_224 proc far
        call    _seg021_22FD_236
        retf
_seg021_22FD_224 endp

; seg021_22FD_228  (+228)
_seg021_22FD_228 proc near
        mov     ah,1
        int     16h
        je      short L0233
L022E:
        mov     ah,0
        int     16h
        ret
L0233:
        sub     ax,ax
        ret
_seg021_22FD_228 endp

; seg021_22FD_236  (+236)
_seg021_22FD_236 proc near
        call    _seg021_22FD_228
        jne     _seg021_22FD_236
        ret
_seg021_22FD_236 endp

SEG021_TEXT ends

        end
