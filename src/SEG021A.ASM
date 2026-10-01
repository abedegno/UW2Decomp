; /* target: seg021_22FD_0 */
; /* opts: /ml */
;
; System module, startup part: the entry glue init returns through (seg021_22FD_0, _17), the
; EGA palette save and restore (_65, _A1), and setting the DOS clock from the real-time clock
; (_E9, _112, _11B).
;
; SEG021 split into its original modules: this is module 1 of 17, +0..+150 of the segment
; (file 23D00h), linked as an object, not from the library.
; Procs: seg021_22FD_0, seg021_22FD_17, seg021_22FD_65, seg021_22FD_A1, seg021_22FD_E9,
; seg021_22FD_112, seg021_22FD_11B.
; Evidence for the bounds: it starts the segment; it ends where the next module's paragraph
; padding begins. Its three relocations (entries 860-862) sit between seg000's and seg001's,
; apart from the other seg021 entries (2276 onwards), so this one module was linked as an
; object in the resident list and the rest of the segment came from the library.
; Its relocations are entries 860-862 of UW2.EXE's relocation table.

        .186

_DATA   segment word public 'DATA'
        extrn   __psp:word                      ; C0's, at DS:92
_DATA   ends
DGROUP  group   _DATA

SEG021_TEXT segment para public 'CODE'
        assume  cs:SEG021_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _init:near
        extrn   _seg021_22FD_174:near
        extrn   _seg021_22FD_73E:near
        public  _seg021_22FD_0
        public  _seg021_22FD_112
        public  _seg021_22FD_11B
        public  _seg021_22FD_17
        public  _seg021_22FD_65
        public  _seg021_22FD_A1
        public  _seg021_22FD_E9

_seg021_22FD_0 proc near
        push    es
        mov     ax,DGROUP
        mov     es,ax
        mov     bx,offset DGROUP:__psp
        mov     ax,word ptr es:[bx]
        mov     word ptr ds:[2],ax
        pop     es
        call    _seg021_22FD_65
        call    _init
        ret
_seg021_22FD_0 endp

; seg021_22FD_17  (+17)
_seg021_22FD_17 proc near
        call    _seg021_22FD_73E
        call    _seg021_22FD_174
        call    _seg021_22FD_A1
        call    _seg021_22FD_E9
        ret
L0024:
        db      0h
L0025:
        db      0h
L0026:
        db      2Eh dup (0)
L0054:
        db      11h dup (0)
_seg021_22FD_17 endp

; seg021_22FD_65  (+65)
_seg021_22FD_65 proc near
        push    si
        push    di
        push    es
        mov     ax,1009h
        mov     dx,SEG021_TEXT
        mov     es,dx
        mov     dx,54h
        int     10h
        mov     di,0
        mov     si,0
L007B:
        mov     ax,1015h
        mov     bl,byte ptr cs:L0054[si]
        int     10h
        mov     byte ptr cs:L0024[di],dh
        mov     byte ptr cs:L0025[di],ch
        mov     byte ptr cs:L0026[di],cl
        inc     si
        add     di,3
        cmp     di,30h
        jne     L007B
        pop     es
        pop     di
        pop     si
        ret
_seg021_22FD_65 endp

; MaybePaletteCycling_seg021_22FD_A1  (+A1)
_seg021_22FD_A1 proc near
        push    si
        push    di
        mov     ax,1002h
        mov     dx,SEG021_TEXT
        push    es
        mov     es,dx
        mov     dx,54h
        mov     bx,0
        mov     cx,10h
        int     10h
        mov     cx,10h
        mov     si,0
        mov     di,0
        mov     bx,0
L00C3:
        mov     ax,1010h
        mov     bl,byte ptr cs:L0054[si]
        mov     dh,byte ptr cs:L0024[di]
        mov     ch,byte ptr cs:L0025[di]
        mov     cl,byte ptr cs:L0026[di]
        int     10h
        add     di,3
        inc     si
        cmp     si,10h
        jne     L00C3
        pop     es
        pop     di
        pop     si
        ret
_seg021_22FD_A1 endp

; seg021_22FD_E9  (+E9)
_seg021_22FD_E9 proc near
        mov     ah,4
        int     1Ah
        push    cx
        push    dx
        mov     ah,2
        int     1Ah
        call    _seg021_22FD_112
        xor     dl,dl
        mov     ah,2Dh
        int     21h
        pop     dx
        pop     cx
        call    _seg021_22FD_112
        xor     al,al
        xchg    ah,al
        mov     ch,64h
        mul     ch
        xor     ch,ch
        add     cx,ax
        mov     ah,2Bh
        int     21h
        ret
_seg021_22FD_E9 endp

; seg021_22FD_112  (+112)
_seg021_22FD_112 proc near
        push    cx
        mov     ax,dx
        call    _seg021_22FD_11B
        mov     dx,ax
        pop     ax
_seg021_22FD_112 endp

; seg021_22FD_11B  (+11B)
_seg021_22FD_11B proc near
        xor     bx,bx
        mov     cx,4
        xchg    ch,ah
        xchg    ch,al
        ror     ax,cl
        shr     ah,cl
        xchg    bh,ah
        mov     cl,0Ah
        mul     cl
        add     al,bh
        xchg    ch,al
        xor     ah,ah
        mov     cl,4
        ror     ax,cl
        shr     ah,cl
        xchg    bl,ah
        mov     cl,0Ah
        mul     cl
        add     al,bl
        mov     ah,ch
        mov     cl,al
        ret
_seg021_22FD_11B endp

SEG021_TEXT ends

        end
