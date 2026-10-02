; /* target: seg020 */
; /* opts: /ml */
;
; One routine, seg020_1(char near *src, int x, int y, int value): copies a 4 by 4 pixel
; block from DS:src (four rows of four bytes) to the planar VGA screen at x, y (row
; offsets from _Ytab), a column per plane, storing the byte 90h as it is and subtracting
; the low byte of value from every other byte. Registers: SI the source, DI the screen
; byte, AH the map mask bit, CH the value, BP counts the columns (the routine reuses BP
; after its frame is set up).
;
; After the plane bit wraps from 8 to 1 the next column is written one byte on, but the
; column after that reloads DI from BX, the block's first byte, so a block starting at
; x & 3 of 2 or 3 would put its last column in the wrong byte. The routine starts at
; seg020:0001 (IDA put its proc at +24, inside it); nothing calls it directly, so the
; slip may never have mattered.
;
; name: descriptive (copies a block to the planar VGA screen); the routine has neither an
; FM Towns nor a symbols.tsv name.
; match: hand-written assembly; it uses no 186 instructions, so it is written without a
; CPU directive. Its two forward jumps carry no padding, so they were written `short`.
; match: the segments are declared by hand, not with .model medium and .code: seg020
; starts at an odd address in the EXE, right after seg019's code, so its code segment
; was byte-aligned, and .model medium declares SEG020_TEXT word-aligned (and refuses
; another alignment).

SEG020_TEXT segment byte public 'CODE'
SEG020_TEXT ends
DGROUP  group   _DATA
_DATA   segment word public 'DATA'
        extrn   _Ytab:byte
_DATA   ends

SEG020_TEXT segment byte public 'CODE'
        assume  cs:SEG020_TEXT, ds:DGROUP

        public  _seg020_1

_seg020_1 proc far
        push    bp
        mov     bp,sp
        push    si
        push    di
        push    es
        mov     dx,3C5h
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
L003B:
        mov     al,ah
        out     dx,al
        mov     cl,4
L0040:
        lodsb
        cmp     al,90h
        je      short L0047
        sub     al,ch
L0047:
        stosb
        add     si,3
        add     di,4Fh
        dec     cl
        jne     L0040
        inc     bp
        cmp     bp,4
        je      short L0069
        sub     si,0Fh
        mov     di,bx
        shl     ah,1
        cmp     ah,8
        jle     L003B
        mov     ah,1
        inc     di
        jmp     L003B
L0069:
        pop     es
        pop     di
        pop     si
        pop     bp
        retf
_seg020_1 endp

SEG020_TEXT ends
        end
