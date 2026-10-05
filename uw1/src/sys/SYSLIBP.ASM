; /* target: seg019_C10 */
; /* opts: /ml */
;
; UW1: UW2's module (src/sys in UW2Decomp), matched to UW.EXE's seg019; the differences are
; marked UW1/UW2 where they occur. Other remarks are UW2's unless they say UW1.
;
; _C10: a far routine that only returns. INSTANCE.ASM's render_3d calls it on a stack in
; seg048 (UW2's seg_370D) in the middle of each frame, so it is probably a hook left empty in this build (a
; debugging or timing call in the development version; nothing in UW2 shows which).
;
; name: descriptive (module 16 of seg019); its job is not known beyond being empty.
;
; seg019 split into its original modules, as UW2's seg021: module 16 of 17, +C10..+C20
; of the segment (file 231B0h), a module of the second library (UWLIB.LIB).
; Procs: seg019_C10.
; Evidence for the bounds: starts on a paragraph after 11 zero bytes following a retf: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG019_TEXT segment para public 'CODE'
        assume  cs:SEG019_TEXT, ds:DGROUP, ss:DGROUP
        public  _seg019_C10

; seg019_C10  (+C10)
_seg019_C10 proc far
        retf
_seg019_C10 endp

SEG019_TEXT ends

        end
