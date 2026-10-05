; /* target: seg021_22FD_730 */
; /* opts: /ml */
;
; The video start-up and shut-down glue, probably FM Towns' initvideo and resetvideo (FM Towns
; has initvideo, saveMode, colorScreen, resetvideo and restoreMode together, as stubs, beside
; its init and exit code; these two call VIDSAVE.ASM's save and restore the same way). _730
; (from SYSINIT.ASM's init) saves the video mode and the BIOS equipment byte and marks the
; display as colour; _73E (from STARTUP.ASM's exit path) puts them back.
;
; name: descriptive (module 12 of seg021). Its job, found here, is the video save and restore at
; start-up and exit; the routine names above are probable, by position and job.
;
; SEG021 split into its original modules: this is module 12 of 17, +730..+750 of the segment
; (file 24430h), a module of the second library (UWLIB.LIB).
; Procs: seg021_22FD_730, seg021_22FD_73E.
; Evidence for the bounds: starts on a paragraph after 15 zero bytes following a retf: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG021_TEXT segment para public 'CODE'
        assume  cs:SEG021_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg021_22FD_680:near
        extrn   _seg021_22FD_692:near
        extrn   _seg021_22FD_6A0:near
        public  _seg021_22FD_730
        public  _seg021_22FD_73E

; seg021_22FD_730  (+730)
; _730 (initvideo, probably): save the mode (_680), set the equipment byte to colour (_6A0). The
; `clc` makes the following jae always jump, so the decrement of FD71:0370 never runs; probably
; a disabled failure path.
_seg021_22FD_730 proc near
        call    _seg021_22FD_680
        call    _seg021_22FD_6A0
        clc
        jae     short L073D
        dec     word ptr ds:[370h]
L073D:
        ret
_seg021_22FD_730 endp

; seg021_22FD_73E  (+73E)
; _73E (resetvideo, probably): restore the equipment byte and the mode (_692).
_seg021_22FD_73E proc near
        jmp     _seg021_22FD_692
_seg021_22FD_73E endp

SEG021_TEXT ends

        end
