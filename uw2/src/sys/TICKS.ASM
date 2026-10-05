; /* target: seg021_22FD_710 */
; /* opts: /ml */
;
; The game clock: a doubleword in the code segment at _seg021_22FD_710 (FM Towns time, which the
; C pointer _Time in SYSENTRY.ASM's _DATA points to), counting 1/256 seconds. SOUND.C's
; cllbck_tst, which AIL calls 256 times a second (init_timers), increments it through *Time; the
; far routine here (+714) does the same increment but no caller of it was found. TICKREAD.ASM's
; _C00 reads the low word, cRender (C3DENTRY.ASM) times frames with it, and GRMISC.ASM's retrace
; wait gives up when it has moved 4 ticks.
;
; name: descriptive (module 11 of seg021, "the tick counter").
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

; The clock (low word, then high word at _712), then the increment.
_seg021_22FD_710:
        db      0h,0h
_seg021_22FD_712:
        db      0h,0h
        add     word ptr cs:_seg021_22FD_710,1
        adc     word ptr cs:_seg021_22FD_712,0
        retf

SEG021_TEXT ends

        end
