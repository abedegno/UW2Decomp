; /* target: seg019_21BA_C */
; /* opts: /ml */
;
; SETPNT.ASM: SetPnt, which appends one point to the 3D view's render database.
;
; SetPnt(x, y, z) is called by GRIDDB.C's grdb_elem for each corner of a floor, ceiling or
; wall face. It writes a 6-byte do_minires record (opcode 0B6h) at dbptr: the word 0B6h,
; then the bytes (x - 10h) << 3 and y << 3, then the bytes z << 1 and ptnuminq's low byte.
; x and y are the grid cell (x relative to the eye's column 16), z a tile height (0 to 16).
; It advances dbptr by 6 and returns ptnuminq, the point's number, before incrementing it;
; grdb_elem passes the numbers to the face routines. FM Towns SetPnt_ (f884) does the same
; with register arguments.
;
; It is the assembly routine at the start of DOS segment seg019_21BA (1FCD:000C-0041),
; before GRIDDB.C's set_pix_xfer. GRIDDB.C's grdb_elem calls it with far calls, which
; Turbo C only emits for a function outside the file, so it was a separate module whose
; code segment has the C file's name (GRIDDB_TEXT): the linker put both into one segment.
;
; name: descriptive file name (map/filenames.tsv: "its one routine, SetPnt (the name it
; had)"). SetPnt, dbptr and ptnuminq are FM Towns names (symbols.tsv).
;
; match: it starts at an odd address right after the divide-by-zero trap, so the segment
; is byte-aligned and written out in full (`.model medium` would make it word-aligned).
; match: hand-written: stosw, shl reg,imm (hence .186), and a frame that pops di and bp by
; hand (with .186, TASM's ARG frames would be enter/leave).

        .186

DGROUP  group   _DATA
_DATA   segment byte public 'DATA'
        extrn   _dbptr:dword                    ; DS:8720, far pointer into the render database
        extrn   _ptnuminq:word                  ; DS:2F72, points queued
_DATA   ends

GRIDDB_TEXT segment byte public 'CODE'
        assume  cs:GRIDDB_TEXT, ds:DGROUP

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

GRIDDB_TEXT ends
        end
