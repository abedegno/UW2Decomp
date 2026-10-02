; /* target: seg021_22FD_680 */
; /* opts: /ml */
;
; The video mode at start-up and exit (probably FM Towns' saveMode, restoreMode, colorScreen and
; monoScreen, which sit together in its symbol table). The data is in FD71 (DS = dseg062_62a6):
; 0350 the BIOS video mode found at start-up, 0351 a far pointer to the BIOS equipment byte
; 0000:0410 (its bits 4-5 are the initial video mode: 20h 80-column colour, 30h monochrome),
; 0355 that byte as found.
;
; name: descriptive (module 9 of seg021, "saving and restoring the video mode").
;
; SEG021 split into its original modules: this is module 9 of 17, +680..+6C0 of the segment
; (file 24380h), a module of the second library (UWLIB.LIB).
; Procs: seg021_22FD_680, seg021_22FD_692, seg021_22FD_6A0, seg021_22FD_6A6.
; Evidence for the bounds: starts on a paragraph after 11 zero bytes following a iret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG021_TEXT segment para public 'CODE'
        assume  cs:SEG021_TEXT, ds:DGROUP, ss:DGROUP
        public  _seg021_22FD_680
        public  _seg021_22FD_692
        public  _seg021_22FD_6A0
        public  _seg021_22FD_6A6

; seg021_22FD_680  (+680)
; _680 (saveMode): read the current mode (int 10h, AH = 0Fh) into 0350 and the equipment byte
; into 0355.
_seg021_22FD_680 proc near
        mov     ah,0Fh
        int     10h
        mov     byte ptr ds:[350h],al
        push    ds
        lds     si,dword ptr ds:[351h]
        lodsb
        pop     ds
        mov     byte ptr ds:[355h],al
        ret
_seg021_22FD_680 endp

; seg021_22FD_692  (+692)
; _692 (restoreMode): put the equipment byte back, then set the saved mode (int 10h, AH = 0).
_seg021_22FD_692 proc near
        mov     al,byte ptr ds:[355h]
        call    _seg021_22FD_6A6
        mov     ah,0
        mov     al,byte ptr ds:[350h]
        int     10h
        ret
_seg021_22FD_692 endp

; seg021_22FD_6A0  (+6A0)
; _6A0 (colorScreen): set the equipment byte's video bits to 20h (80-column colour). The `mov
; al,30h` after its jump is a second entry with no label (monoScreen, probably), which nothing
; calls.
_seg021_22FD_6A0 proc near
        mov     al,20h
        jmp     short _seg021_22FD_6A6
        mov     al,30h
_seg021_22FD_6A0 endp

; seg021_22FD_6A6  (+6A6)
; _6A6: write AL into the equipment byte's video bits (bits 4-5), keeping the others from the
; saved byte.
_seg021_22FD_6A6 proc near
        mov     ah,byte ptr ds:[355h]
        and     ah,0CFh
        or      al,ah
        push    es
        les     di,dword ptr ds:[351h]
        stosb
        pop     es
        ret
_seg021_22FD_6A6 endp

SEG021_TEXT ends

        end
