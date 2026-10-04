; /* target: seg018 */
; /* opts: /ml */
;
; One routine, seg018_6(char near *src, int x, int y, int value): copies a 4 by 4 pixel
; block from DS:src (four rows of four bytes) to the planar VGA screen at x, y (row
; offsets from _Ytab), a column per plane, storing the byte 0EAh as it is and subtracting
; the low byte of value from every other byte. Registers: SI the source, DI the screen
; byte, AH the map mask bit, CH the value, BP counts the columns (the routine reuses BP
; after its frame is set up).
;
; It is UW2's src/gfx/PLANECPY.ASM (seg020) but for the byte it leaves alone: 0EAh here,
; 90h in UW2. Segment table entry 20, 1F32:0006-0073, between seg017 and seg019.
;
; After the plane bit wraps from 8 to 1 the next column is written one byte on, but the
; column after that reloads DI from BX, the block's first byte, so a block starting at
; x & 3 of 2 or 3 would put its last column in the wrong byte. Nothing in UW.EXE calls it
; (no relocation in the segment, no far call to 1F32:0006), so the slip may never have
; mattered.
;
; name: descriptive (copies a block to the planar VGA screen), as UW2's; the routine has
; no name in symbols.tsv or the listing.
; match: hand-written assembly; it uses no 186 instructions, so it is written without a
; CPU directive. Its two forward jumps carry no padding, so they were written `short`.
; match: the segments are declared by hand, not with .model medium and .code, as UW2's:
; UW2's seg020 starts at an odd address, so its code segment was byte-aligned, and .model
; medium declares the segment word-aligned (and refuses another alignment). UW1's seg018
; starts right after seg017's code at an even address (1F32:0006), where byte and word
; alignment give the same place.

; The VGA ports used here.
SC_DATA         equ     3C5h  ; sequencer data

SEG018_TEXT segment byte public 'CODE'
SEG018_TEXT ends
DGROUP  group   _DATA
_DATA   segment word public 'DATA'
        extrn   _Ytab:byte
_DATA   ends

SEG018_TEXT segment byte public 'CODE'
        assume  cs:SEG018_TEXT, ds:DGROUP

        public  _seg018_6

_seg018_6 proc far
        push    bp
        mov     bp,sp
        push    si
        push    di
        push    es
        mov     dx,SC_DATA
        mov     si,word ptr [bp+6]
        mov     bx,word ptr [bp+0Ah]
        shl     bx,1
        les     di,dword ptr _Ytab
        mov     ax,word ptr es:[bx+di]
        mov     bx,word ptr [bp+8]
        shr     bx,1
        shr     bx,1
        add     bx,ax
        mov     di,bx
        mov     ax,0A000h
        mov     es,ax
        mov     cl,byte ptr [bp+8]
        and     cl,3
        mov     ah,1
        shl     ah,cl
        mov     cx,word ptr [bp+0Ch]
        xchg    ch,cl
        mov     bp,0
L0040:
        mov     al,ah
        out     dx,al
        mov     cl,4
L0045:
        lodsb
        cmp     al,0EAh
        je      short L004C
        sub     al,ch
L004C:
        stosb
        add     si,3
        add     di,4Fh
        dec     cl
        jne     L0045
        inc     bp
        cmp     bp,4
        je      short L006E
        sub     si,0Fh
        mov     di,bx
        shl     ah,1
        cmp     ah,8
        jle     L0040
        mov     ah,1
        inc     di
        jmp     L0040
L006E:
        pop     es
        pop     di
        pop     si
        pop     bp
        retf
_seg018_6 endp

SEG018_TEXT ends
        end
