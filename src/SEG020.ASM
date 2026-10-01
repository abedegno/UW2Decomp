; /* target: seg020 */
; /* opts: /ml */
;
; One routine: copies a small block to the planar VGA screen, plane by plane, four bytes
; down each column (row offsets from _Ytab), storing the value 90h as it is and
; subtracting a constant from every other byte. Hand-written assembly; it uses no 186
; instructions, so it is written without a CPU directive. Its two forward jumps carry
; no padding, so they were written `short`.
;
; The routine starts at seg020:0001 (IDA put its proc at +24, inside it); nothing calls
; it directly, so it has neither an FM Towns nor a symbols.tsv name.

        .model  medium


        .data
        extrn   _Ytab:byte

        .code

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

        end
