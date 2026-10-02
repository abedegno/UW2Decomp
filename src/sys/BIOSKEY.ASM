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
; The game reads the keyboard through its own int 9 handler (KBDINT.ASM, KEYQUEUE.ASM), so
; these BIOS routines serve only to empty the BIOS buffer; the two far thunks are not called
; from anywhere in UW2.
; name: descriptive (system module 4, the BIOS keyboard).

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
; Far thunk: waits for a key through the BIOS (int 16h function 0) and returns it in AX.
_seg021_22FD_220 proc far
        call    L022E
        retf
_seg021_22FD_220 endp

; seg021_22FD_224  (+224)
; Far thunk for _236.
_seg021_22FD_224 proc far
        call    _seg021_22FD_236
        retf
_seg021_22FD_224 endp

; seg021_22FD_228  (+228)
; Takes a key from the BIOS buffer if there is one: AX = the key (scan code in AH, ASCII
; in AL) and ZF clear, or AX = 0 and ZF set if the buffer is empty. L022E, inside it, reads
; a key unconditionally.
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
; Empties the BIOS keyboard buffer. An unnamed far entry in SYSENTRY.ASM reaches it.
_seg021_22FD_236 proc near
        call    _seg021_22FD_228
        jne     _seg021_22FD_236
        ret
_seg021_22FD_236 endp

SEG021_TEXT ends

        end
