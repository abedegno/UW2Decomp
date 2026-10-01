; /* target: seg019_21BA_C */
; /* opts: /ml */
;
; SetPnt: the assembly routine at the start of DOS segment seg019_21BA (1FCD:000C-0041),
; before SEG019.C's set_pix_xfer. SEG019.C's grdb_elem calls it with far calls, which
; Turbo C only emits for a function outside the file, so it was a separate module whose
; code segment has the C file's name (SEG019_TEXT): the linker put both into one segment.
; It starts at an odd address right after the divide-by-zero trap, so the segment is
; byte-aligned and written out in full (`.model medium` would make it word-aligned).
;
; Hand-written: stosw into the render database, shl reg,imm (hence .186), and a frame that
; pops di and bp by hand (with .186, TASM's ARG frames would be enter/leave).
;
; SetPnt(x, y, z) appends a 6-byte point record at dbptr: the word 0B6h, then
; bytes (x - 10h) << 3 and y << 3, then the bytes z << 1 and ptnuminq's low byte; it advances dbptr
; by 6 and returns ptnuminq before incrementing it. FM Towns SetPnt_ (f884) does the same
; with register arguments. Names: SetPnt, dbptr and ptnuminq are FM Towns names
; (symbols.tsv).

        .186

DGROUP  group   _DATA
_DATA   segment byte public 'DATA'
        extrn   _dbptr:dword                    ; DS:8720, far pointer into the render database
        extrn   _ptnuminq:word                  ; DS:2F72, points queued
_DATA   ends

SEG019_TEXT segment byte public 'CODE'
        assume  cs:SEG019_TEXT, ds:DGROUP

        public  _SetPnt

_SetPnt proc far
        push    bp
        mov     bp,sp
        push    di
        les     di,_dbptr
        mov     ax,0B6h
        stosw
        mov     ah,[bp+8]
        shl     ah,3
        mov     al,[bp+6]
        sub     al,10h
        shl     al,3
        stosw
        mov     bx,_ptnuminq
        mov     al,[bp+0Ah]
        shl     al,1
        mov     ah,bl
        stosw
        add     word ptr _dbptr,6
        inc     _ptnuminq
        mov     ax,bx
        pop     di
        pop     bp
        ret
_SetPnt endp

SEG019_TEXT ends
        end
