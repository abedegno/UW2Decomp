; /* target: seg002 */
; /* opts: /ml */
;
; One far routine, seg002_2(src far *, dst far *): it decodes a run/skip/dump stream (the
; frame encoding of DeluxePaint Animation LPF files) from src into dst and returns the
; final dst offset in AX. Ops, by the signed control byte c:
;   c > 0         dump: copy the next c bytes
;   c == 0        run: a count byte and a value byte; store the value count times
;   c < 0, c != 80h   skip c - 80h destination bytes
;   c == 80h      a word w follows: w > 0 skips w bytes; w == 0 ends; otherwise
;                 w - 8000h below 4000h dumps that many bytes, and w - 0C000h is a run
;                 of the next byte; long dumps and runs move words, aligning first.
;
; Hand-written assembly using only 8086 instructions (every forward jump, none padded
; with a nop, is written short). Nothing in the EXE calls it: there is no far call to
; 007B:0002 in the resident code or through the overlay segment table, and IDA has no
; reference. The FM Towns build has no counterpart either (its draw_rsd_ is a clipped C
; decoder, and draw_into_buffer_ is an empty stub), so it keeps the provisional name.

        .model  medium

        .code

        public  _seg002_2

_seg002_2 proc far
        push    bp
        mov     bp,sp
        push    si
        push    di
        push    es
        mov     si,word ptr [bp+6]      ; ds:si = src
        mov     di,word ptr [bp+0Ah]    ; es:di = dst
        mov     es,word ptr [bp+0Ch]
        push    ds
        mov     ds,word ptr [bp+8]
        sub     ch,ch
        jmp     short L0020
L0019:                                  ; c < 0
        sub     cl,80h
        je      short L0045
        add     di,cx                   ; short skip
L0020:                                  ; next op
        mov     cl,byte ptr [si]
        inc     si
        jcxz    L0034
        or      cl,cl
        jl      L0019
L0029:                                  ; short dump
        rep movsb
        mov     cl,byte ptr [si]
        inc     si
        or      cl,cl
        jl      L0019
        jg      L0029
L0034:                                  ; short run
        mov     cl,byte ptr [si]
        inc     si
        lodsb
        rep stosb
        mov     cl,byte ptr [si]
        inc     si
        jcxz    L0034
        or      cl,cl
        jl      L0019
        jmp     L0029
L0045:                                  ; c == 80h: a word op
        lodsw
        or      ax,ax
        jle     short L004E
        add     di,ax                   ; long skip
        jmp     L0020
L004E:
        je      short L008C             ; 0 ends the frame
        mov     cx,ax
        sub     ch,80h
        cmp     ch,40h
        jge     short L0071
        test    si,1                    ; long dump
        je      short L0062
        movsb
        dec     cx
L0062:
        shr     cx,1
        jb      short L006C
        rep movsw
L0068:
        sub     ch,ch
        jmp     L0020
L006C:
        rep movsw
        movsb
        jmp     L0068
L0071:
        sub     ch,40h                  ; long run
        lodsb
        mov     ah,al
        test    di,1
        je      short L007F
        stosb
        dec     cx
L007F:
        shr     cx,1
        jb      short L0087
        rep stosw
        jmp     L0068
L0087:
        rep stosw
        stosb
        jmp     L0068
L008C:
        pop     ds
        mov     ax,di
        pop     es
        pop     di
        pop     si
        pop     bp
        retf
_seg002_2 endp

        end
