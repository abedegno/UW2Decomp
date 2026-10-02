; /* target: seg021_22FD_710 */
; /* opts: /ml */
;
; The tick counter: a doubleword at _seg021_22FD_710 (FM Towns time, which _Time points to)
; and the far routine that increments it.
;
; SEG021 split into its original modules: this is module 11 of 17, +710..+730 of the segment
; (file 24410h), a module of the second library (UWLIB.LIB).
; Procs: none (data).
; Evidence for the bounds: starts on a paragraph after 3 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG021_TEXT segment para public 'CODE'
        assume  cs:SEG021_TEXT, ds:DGROUP, ss:DGROUP
        public  _seg021_22FD_710
        public  _seg021_22FD_712

_seg021_22FD_710:
        db      0h,0h
_seg021_22FD_712:
        db      0h,0h
        add     word ptr cs:_seg021_22FD_710,1
        adc     word ptr cs:_seg021_22FD_712,0
        retf

SEG021_TEXT ends

        end
