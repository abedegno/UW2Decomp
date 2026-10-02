; /* target: seg021_22FD_C10 */
; /* opts: /ml */
;
; _C10: a far routine that only returns. INSTANCE.ASM's render_3d calls it on a stack in
; seg_370D in the middle of each frame, so it is probably a hook left empty in this build (a
; debugging or timing call in the development version; nothing in UW2 shows which).
;
; name: descriptive (module 16 of seg021); its job is not known beyond being empty.
;
; SEG021 split into its original modules: this is module 16 of 17, +C10..+C20 of the segment
; (file 24910h), a module of the second library (UWLIB.LIB).
; Procs: seg021_22FD_C10.
; Evidence for the bounds: starts on a paragraph after 11 zero bytes following a retf: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG021_TEXT segment para public 'CODE'
        assume  cs:SEG021_TEXT, ds:DGROUP, ss:DGROUP
        public  _seg021_22FD_C10

; seg021_22FD_C10  (+C10)
_seg021_22FD_C10 proc far
        retf
_seg021_22FD_C10 endp

SEG021_TEXT ends

        end
