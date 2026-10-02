; /* target: seg004_0849_0 */
; /* opts: /ml */
;
; The image decoders: uncmp_pal, and exp_8str, exp_4str, exp_4run, exp_5run and exp_8run (FM
; Towns names) with their helpers.
;
; SEG004 split into its original modules: this is module 1 of 14, +0..+260 of the segment
; (file 91C0h), a module of the second library (UWLIB.LIB).
; Procs: exp_8str, exp_4str, seg004_0849_63, seg004_0849_CB, exp_4run, exp_5run, exp_8run,
; seg004_0849_18D, seg004_0849_194.
; Evidence for the bounds: it starts the segment (paragraph-aligned in the segment table); it
; ends where the next module's padding begins.
; Its relocations are entries 2310-2318 of UW2.EXE's relocation table.

        .386

        extrn   _cmpbuf1_start:far

_DATA   segment word public use16 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public use16 'CODE'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   lightabs:byte
        public  _exp_4run
        public  _exp_4str
        public  _exp_5run
        public  _exp_8run
        public  _exp_8str
        public  _seg004_0849_18D
        public  _seg004_0849_194
        public  _seg004_0849_63
        public  _seg004_0849_CB
        public  _uncmp_pal

_uncmp_pal      db      0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah
                db      0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah

; seg004_0849_20  (+20)
_exp_8str proc near
        call    _seg004_0849_CB
        mov     si,bp
        mov     dx,seg _cmpbuf1_start
        mov     es,dx
        mov     di,0
        lodsw
        mov     cx,ax
L0030:
        lodsb
        xlat    byte ptr cs:_uncmp_pal[bx]
        stosb
        loop    L0030
        mov     ax,dx
        ret
_exp_8str endp

; seg004_0849_39  (+39)
_exp_4str proc near
        mov     cx,1
        call    _seg004_0849_63
        mov     si,bp
        mov     dx,seg _cmpbuf1_start
        mov     es,dx
        mov     di,0
        lodsw
        mov     cx,ax
L004C:
        lodsb
        mov     ah,al
        and     al,0F0h
        shr     al,4
        xlat    byte ptr cs:_uncmp_pal[bx]
        stosb
        mov     al,ah
        and     al,0Fh
        xlat    byte ptr cs:_uncmp_pal[bx]
        stosb
        loop    L004C
        mov     ax,dx
        ret
_exp_4str endp

; seg004_0849_63  (+63)
_seg004_0849_63 proc near
        mov     di,0
        mov     bx,SEG004_TEXT
        mov     es,bx
        cmp     dh,0FFh
        jne     short L0079
        mov     dx,ax
        shl     cx,4
        rep movsb
        jmp     short L00C5
L0079:
        mov     bh,dh
        xor     bl,bl
        add     bx,offset lightabs
        mov     dx,ax
L0083:
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        loop    L0083
L00C5:
        mov     bx,0
        mov     ds,dx
        ret
_seg004_0849_63 endp

; seg004_0849_CB  (+CB)
_seg004_0849_CB proc near
        cmp     dh,0FFh
        mov     bh,dh
        xor     bl,bl
        add     bx,offset lightabs
        mov     ds,ax
        ret
_seg004_0849_CB endp

; seg004_0849_D9  (+D9)
_exp_4run proc near
        mov     cx,1
        call    _seg004_0849_63
        mov     si,bp
        mov     dx,seg _cmpbuf1_start
        mov     es,dx
        mov     di,0
        lodsw
        mov     dx,ax
        inc     ax
        shr     ax,1
        mov     cx,ax
L00F1:
        lodsb
        mov     ah,al
        and     al,0F0h
        shr     al,4
        stosb
        mov     al,ah
        and     al,0Fh
        stosb
        loop    L00F1
        mov     cx,dx
        mov     ax,seg _cmpbuf1_start
        mov     ds,ax
        mov     si,0
        mov     ax,seg _cmpbuf1_start
        mov     es,ax
        mov     di,5400h
        call    _seg004_0849_18D
        mov     ax,es
        add     ax,540h
        ret
_exp_4run endp

; seg004_0849_11C  (+11C)
_exp_5run proc near
        mov     cx,2
        call    _seg004_0849_63
        mov     si,bp
        mov     dx,seg _cmpbuf1_start
        mov     es,dx
        mov     di,0
        lodsw
        mov     dx,ax
        add     ax,7
        shr     ax,3
        mov     cx,ax
L0137:
        lodsb
        xor     ah,ah
        ror     ax,3
        stosb
        lodsb
        shr     ah,5
        ror     ax,6
        stosb
        shr     ax,3
        xchg    ah,al
        stosb
        lodsb
        shr     ah,7
        ror     ax,4
        stosb
        lodsb
        shr     ah,4
        ror     ax,7
        stosb
        xor     al,al
        rol     ax,5
        stosb
        lodsb
        shr     ah,6
        ror     ax,5
        stosb
        shr     ax,0Bh
        stosb
        loop    L0137
        mov     cx,dx
        mov     ax,seg _cmpbuf1_start
        mov     ds,ax
        mov     si,0
        mov     ax,seg _cmpbuf1_start
        mov     es,ax
        mov     di,5400h
        call    _seg004_0849_18D
        mov     ax,es
        add     ax,540h
        ret
_exp_5run endp

; seg004_0849_18B  (+18B)
_exp_8run proc near
        ret
L018C:
        ret
_exp_8run endp

; seg004_0849_18D  (+18D)
_seg004_0849_18D proc near
        mov     bp,cx
        add     bp,si
        mov     dx,3
_seg004_0849_18D endp

; seg004_0849_194  (+194)
_seg004_0849_194 proc near
        cmp     si,bp
        jae     L018C
        sub     ax,ax
        lodsb
        cmp     al,2
        jbe     short L01F9
        mov     cx,ax
        lodsb
        xlat    byte ptr cs:_uncmp_pal[bx]
        rep stosb
L01A6:
        sub     ax,ax
        lodsb
        mov     cx,ax
        jcxz    L01CE
L01AD:
        lodsb
        xlat    byte ptr cs:_uncmp_pal[bx]
        stosb
        loop    L01AD
        jmp     _seg004_0849_194
L01B5:
        mov     cl,4
        lodsb
        shl     ax,cl
        or      al,byte ptr [si]
        shl     ax,cl
        or      al,byte ptr [si+1]
        shl     ax,cl
        or      al,byte ptr [si+2]
        add     si,dx
        mov     cx,ax
        xor     ah,ah
        jmp     L01AD
L01CE:
        lodsb
        test    al,al
        je      L01B5
        shl     ax,4
        mov     cx,ax
        lodsb
        or      cx,ax
        jmp     L01AD
L01DD:
        mov     cl,4
        lodsb
        shl     ax,cl
        or      al,byte ptr [si]
        shl     ax,cl
        or      al,byte ptr [si+1]
        shl     ax,cl
        or      al,byte ptr [si+2]
        add     si,dx
        mov     cx,ax
        lodsb
        xlat    byte ptr cs:_uncmp_pal[bx]
        rep stosb
        jmp     L01A6
L01F9:
        je      short L0213
        test    al,al
        jne     short L0211
        lodsb
        test    al,al
        je      L01DD
        shl     ax,4
        mov     cx,ax
        lodsb
        or      cx,ax
        lodsb
        xlat    byte ptr cs:_uncmp_pal[bx]
        rep stosb
L0211:
        jmp     L01A6
L0213:
        mov     byte ptr cs:L01A6,0C3h
        sub     ax,ax
        lodsb
        mov     cx,ax
        jcxz    L0247
L0220:
        push    cx
        call    _seg004_0849_194
        pop     cx
        loop    L0220
        mov     byte ptr cs:L01A6,29h
        jmp     L01A6
L0230:
        mov     cl,4
        lodsb
        shl     ax,cl
        or      al,byte ptr [si]
        shl     ax,cl
        or      al,byte ptr [si+1]
        shl     ax,cl
        or      al,byte ptr [si+2]
        add     si,dx
        mov     cx,ax
        jmp     L0220
L0247:
        lodsb
        test    al,al
        je      L0230
        shl     ax,4
        mov     cx,ax
        lodsb
        or      cx,ax
        jmp     L0220
_seg004_0849_194 endp

SEG004_TEXT ends

        end
