; /* target: seg019_220 */
; /* opts: /ml */
;
; UW1: UW2's module (src/sys in UW2Decomp), matched to UW.EXE's seg019; the differences are
; marked UW1/UW2 where they occur. Other remarks are UW2's unless they say UW1.
;
; The BIOS keyboard (int 16h): read a key, flush the buffer, and far thunks for C.
;
; seg019 split into its original modules, as UW2's seg021: module 4 of 17, +220..+240
; of the segment (file 227C0h), a module of the second library (UWLIB.LIB).
; Procs: seg019_220, seg019_224, seg019_228, seg019_236.
; Evidence for the bounds: starts on a paragraph after 5 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.
; The game reads the keyboard through its own int 9 handler (KBDINT.ASM, KEYQUEUE.ASM), so
; these BIOS routines serve only to empty the BIOS buffer; the two far thunks are not called
; from anywhere in UW2.
; name: descriptive (system module 4, the BIOS keyboard).

; The BIOS keyboard (int 16h) functions used here.
KBD_READ        equ     00h
KBD_STATUS      equ     01h

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG019_TEXT segment para public 'CODE'
        assume  cs:SEG019_TEXT, ds:DGROUP, ss:DGROUP
        public  _seg019_220
        public  _seg019_224
        public  _seg019_228
        public  _seg019_236

; seg019_220  (+220)
; Far thunk: waits for a key through the BIOS (int 16h function 0) and returns it in AX.
_seg019_220 proc far
        call    L022E
        retf
_seg019_220 endp

; seg019_224  (+224)
; Far thunk for _236.
_seg019_224 proc far
        call    _seg019_236
        retf
_seg019_224 endp

; seg019_228  (+228)
; Takes a key from the BIOS buffer if there is one: AX = the key (scan code in AH, ASCII
; in AL) and ZF clear, or AX = 0 and ZF set if the buffer is empty. L022E, inside it, reads
; a key unconditionally.
_seg019_228 proc near
        mov     ah,KBD_STATUS
        int     16h
        je      short L0233
L022E:
        mov     ah,KBD_READ
        int     16h
        ret
L0233:
        sub     ax,ax
        ret
_seg019_228 endp

; seg019_236  (+236)
; Empties the BIOS keyboard buffer. An unnamed far entry in SYSENTRY.ASM reaches it.
_seg019_236 proc near
        call    _seg019_228
        jne     _seg019_236
        ret
_seg019_236 endp

SEG019_TEXT ends

        end
