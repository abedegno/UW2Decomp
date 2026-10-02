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
;
; How seg021 runs. seg021 is a library of small assembly modules (the system layer: start-up
; and shutdown, the keyboard, mouse, joystick and timer, integer maths, and the entry points
; into the 3D code). It has its own data segment, dseg062_62a6 (60B9:0000, C40h bytes,
; taken from the EXE at link time), and its own stack at the top of that segment's first
; 510h bytes. C calls it only through the far entry points in SYSENTRY.ASM and
; C3DENTRY.ASM, which save the caller's SS:SP in the code segment, load DS, ES and SS with
; dseg062_62a6 and SP with 510h, call the near routine and switch back. So in every module
; of the segment, `ds:[n]` is offset n of dseg062_62a6, whatever the `assume ds:DGROUP`
; says (the assume only gives TASM a default; the addresses are plain numbers).
; SYSENTRY.ASM's header has a map of that segment.
;
; This module: GRFX.C's grfx_init calls _755 (SYSENTRY.ASM), which runs seg021_22FD_0, and
; grfx_close calls _791, which runs seg021_22FD_17.
; name: descriptive (system module 1, start-up).

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

; Start-up. Stores the PSP segment (C0's __psp, from DGROUP) as the segment of the far
; pointer at DS:0, whose offset is 000Ah: so DS:0 points at the PSP's terminate address,
; which init (SYSINIT.ASM) replaces with its exit routine. Then saves the EGA palette and
; runs init.
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
; Shutdown, the reverse of init: restores the video mode and equipment byte (_73E, in
; SYSLIBL.ASM), the int 24h vector (_174), the EGA palette (_A1), and sets the DOS clock from
; the real-time clock (_E9), probably because the game's timer interrupt handling leaves
; the DOS time wrong. init also jumps here when it fails.
; The 49 bytes after the ret are the palette save area: L0054, the 17 bytes of palette
; registers int 10h/1009h returns (16 registers and the overscan), and L0024/L0025/L0026,
; three interleaved byte arrays for the 16 registers' red, green and blue DAC values (each
; entry 3 bytes apart; L0026 runs 2Eh bytes so that the last triple fits).
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
; Saves the EGA palette: reads the 16 palette registers and overscan (int 10h 1009h) into
; L0054, then for each of the 16 registers reads the DAC colour it selects (1015h) into
; the triples at L0024.
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
; Restores what _65 saved: the palette registers (1002h) and the 16 DAC colours (1010h).
; Not palette cycling, whatever IDA's name says.
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
; Sets the DOS date and time from the real-time clock: int 1Ah function 4 (date, BCD in
; CX:DX) and function 2 (time, BCD in CX:DX), converted to binary by _112 and _11B and set
; with int 21h 2Dh (time) and 2Bh (date, century * 100 + year).
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
; Converts four packed BCD bytes to binary: CX and DX in, both returned converted (each
; byte separately). It converts DX by calling _11B and then falls through into _11B for CX,
; so the two share its ret.
_seg021_22FD_112 proc near
        push    cx
        mov     ax,dx
        call    _seg021_22FD_11B
        mov     dx,ax
        pop     ax
_seg021_22FD_112 endp

; seg021_22FD_11B  (+11B)
; Converts the two packed BCD bytes of AX to binary, each byte separately, returning the
; result in both AX and CX; BX is scratch.
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
