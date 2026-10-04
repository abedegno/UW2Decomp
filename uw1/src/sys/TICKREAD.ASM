; /* target: seg019_C00 */
; /* opts: /ml */
;
; UW1: UW2's module (src/sys in UW2Decomp), matched to UW.EXE's seg019; the differences are
; marked UW1/UW2 where they occur. Other remarks are UW2's unless they say UW1.
;
; _C00: AX = the low word of the game clock (TICKS.ASM), far. Called by GRMISC.ASM's
; frame-timing pair (_28, _46).
;
; name: descriptive (module 15 of seg019, "reads the tick counter").
;
; seg019 split into its original modules, as UW2's seg021: module 15 of 17, +C00..+C10
; of the segment (file 231A0h), a module of the second library (UWLIB.LIB).
; Procs: seg019_C00.
; Evidence for the bounds: starts on a paragraph after 7 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG019_TEXT segment para public 'CODE'
        assume  cs:SEG019_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg019_710:near
        public  _seg019_C00

; seg019_C00  (+C00)
_seg019_C00 proc far
        mov     ax,word ptr cs:_seg019_710
        retf
_seg019_C00 endp

SEG019_TEXT ends

        end
