; /* target: seg002 */
; /* opts: /ml */
;
; One far routine, seg002_A(src far *, dst far *): it decodes a run/skip/dump stream (the
; frame encoding of DeluxePaint Animation LPF files) from src into dst and returns the
; final dst offset in AX. Ops, by the signed control byte c:
;   c > 0         dump: copy the next c bytes
;   c == 0        run: a count byte and a value byte; store the value count times
;   c < 0, c != 80h   skip c - 80h destination bytes
;   c == 80h      a word w follows: w > 0 skips w bytes; w == 0 ends; otherwise
;                 w - 8000h below 4000h dumps that many bytes, and w - 0C000h is a run
;                 of the next byte; long dumps and runs move words, aligning first.
;
; UW2Decomp's src/gfx/LPFDELTA.ASM (UW2's seg002_2), the same 0x92 bytes. UW1 against UW2:
; UW1 uses it. CUTS.C's show_anm decodes each delta record with it into the linear frame
; buffer cuts_screen, where UW2 has its own C decoder (draw_rsd) and leaves this one
; unreferenced. In UW1 the routine starts at segment offset 0Ah (UW2: 2), at file 3A6Ah,
; straight after the retf that ends seg001: the segment's paragraph begins inside seg001.
;
; Assembly, not C: it swaps DS for the source segment (push ds; mov ds,...), keeps the
; count in CX with sub ch,ch, falls between the run and dump loops without a frame of
; locals, and uses rep movsb/stosb, lodsw and jcxz throughout, none of which Turbo C
; emits.
;
; name: descriptive, the listing's (the Deluxe Paint LPF run/skip/dump decoder). Neither
; FM Towns build has a counterpart (UW2's draw_rsd_ is a clipped C decoder), so the
; routine keeps the provisional name CUTS.C already calls.
; match: hand-written assembly using only 8086 instructions (every forward jump, none
; padded with a nop, is written short).

; Class ASMCODE, as SPRITE.ASM's: seg000 to seg002 are a class of their own that ends in CODE
; (docs/LINKING.md, Segment classes). TASM keeps a .model segment's class, so the segment
; and DGROUP are declared here, as .model medium would, under the name it gave.
LPFDELTA_TEXT segment word public 'ASMCODE'
        assume  cs:LPFDELTA_TEXT, ds:DGROUP

        public  _seg002_A

_seg002_A proc far
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
_seg002_A endp

LPFDELTA_TEXT ends

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

        end
