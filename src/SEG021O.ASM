; /* target: seg021_22FD_C00 */
; /* opts: /ml */
;
; _C00: reads the tick counter (far).
;
; SEG021 split into its original modules: this is module 15 of 17, +C00..+C10 of the segment
; (file 24900h), a module of the second library (UWLIB.LIB).
; Procs: seg021_22FD_C00.
; Evidence for the bounds: starts on a paragraph after 7 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG021_TEXT segment para public 'CODE'
        assume  cs:SEG021_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg021_22FD_710:near
        public  _seg021_22FD_C00

; seg021_22FD_C00  (+C00)
_seg021_22FD_C00 proc far
        mov     ax,word ptr cs:_seg021_22FD_710
        retf
_seg021_22FD_C00 endp

SEG021_TEXT ends

        end
