; /* target: seg019_730 */
; /* opts: /ml */
;
; UW1: UW2's module (src/sys in UW2Decomp), matched to UW.EXE's seg019; the differences are
; marked UW1/UW2 where they occur. Other remarks are UW2's unless they say UW1.
;
; The video start-up and shut-down glue, probably FM Towns' initvideo and resetvideo (FM Towns
; has initvideo, saveMode, colorScreen, resetvideo and restoreMode together, as stubs, beside
; its init and exit code; these two call VIDSAVE.ASM's save and restore the same way). _730
; (from SYSINIT.ASM's init) saves the video mode and the BIOS equipment byte and marks the
; display as colour; _73E (from STARTUP.ASM's exit path) puts them back.
;
; name: descriptive (module 12 of seg019). Its job, found here, is the video save and restore at
; start-up and exit; the routine names above are probable, by position and job.
;
; seg019 split into its original modules, as UW2's seg021: module 12 of 17, +730..+750
; of the segment (file 22CD0h), a module of the second library (UWLIB.LIB).
; Procs: seg019_730, seg019_73E.
; Evidence for the bounds: starts on a paragraph after 15 zero bytes following a retf: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG019_TEXT segment para public 'CODE'
        assume  cs:SEG019_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg019_680:near
        extrn   _seg019_692:near
        extrn   _seg019_6A0:near
        public  _seg019_730
        public  _seg019_73E

; seg019_730  (+730)
; _730 (initvideo, probably): save the mode (_680), set the equipment byte to colour (_6A0). The
; `clc` makes the following jae always jump, so the decrement of seg063:0370 never runs; probably
; a disabled failure path.
_seg019_730 proc near
        call    _seg019_680
        call    _seg019_6A0
        clc
        jae     short L073D
        dec     word ptr ds:[370h]
L073D:
        ret
_seg019_730 endp

; seg019_73E  (+73E)
; _73E (resetvideo, probably): restore the equipment byte and the mode (_692).
_seg019_73E proc near
        jmp     _seg019_692
_seg019_73E endp

SEG019_TEXT ends

        end
