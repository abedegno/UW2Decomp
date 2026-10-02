; /* target: seg021_22FD_240 */
; /* opts: /ml */
;
; init, its exit routine and errorexit (FM Towns names): installs the handlers, hooks the exit
; chain, and on exit restores everything and prints the error message.
;
; SEG021 split into its original modules: this is module 5 of 17, +240..+2E0 of the segment
; (file 23F40h), a module of the second library (UWLIB.LIB).
; Procs: seg021_22FD_240, init, seg021_22FD_2B6, seg021_22FD_2BD.
; Evidence for the bounds: starts on a paragraph after 4 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; Its relocations are entries 2276-2277 of UW2.EXE's relocation table.

        .186

        extrn   dseg062_62a6:far
        extrn   seg052_519C:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG021_TEXT segment para public 'CODE'
        assume  cs:SEG021_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg021_22FD_17:near
        extrn   _seg021_22FD_1D0:near
        extrn   _seg021_22FD_30A:near
        extrn   _seg021_22FD_598:near
        extrn   _seg021_22FD_5C8:near
        extrn   _seg021_22FD_6C8:near
        extrn   _seg021_22FD_730:near
        extrn   _setvect24:near
        public  _init
        public  _seg021_22FD_240
        public  _seg021_22FD_2B6
        public  _seg021_22FD_2BD

; seg021_22FD_240  (+240)
_seg021_22FD_240 proc far
        call    errorexit
        retf
_seg021_22FD_240 endp

; seg021_22FD_244  (+244)
_init proc near
        call    _seg021_22FD_6C8
        call    _seg021_22FD_1D0
        call    _seg021_22FD_2B6
        call    _setvect24
        call    _seg021_22FD_2BD
        call    _seg021_22FD_30A
        call    _seg021_22FD_598
        call    _seg021_22FD_730
        push    ds
        lds     si,dword ptr ds:[0]
        mov     di,18Ch
        movsw
        movsw
        pop     ds
        push    es
        les     di,dword ptr ds:[0]
        mov     ax,offset L028C                 ; the exit routine below
        stosw
        mov     ax,cs
        stosw
        pop     es
        mov     cx,word ptr ds:[370h]
        jcxz    L028B
        cmp     word ptr ds:[190h],0
        jne     short errorexit
        mov     si,560h
errorexit:
        mov     word ptr ds:[190h],si
        jmp     _seg021_22FD_17
L028B:
        ret

; Reached through the far pointer init stored at DS:0 (the exit chain).
L028C:
        mov     ax,seg dseg062_62a6
        mov     es,ax
        mov     ds,ax
        cli
        mov     ss,ax
        mov     ax,122h
        mov     sp,ax
        sti
        call    word ptr ds:[192h]
        call    _seg021_22FD_5C8
        cmp     word ptr ds:[190h],0
        je      short L02B2
        mov     dx,word ptr ds:[190h]
        mov     ah,9
        int     21h
L02B2:
        jmp     dword ptr ds:[18Ch]
_init endp

; seg021_22FD_2B6  (+2B6)
_seg021_22FD_2B6 proc near
        mov     ax,seg seg052_519C
        mov     word ptr ds:[18Ah],ax
        ret
_seg021_22FD_2B6 endp

; seg021_22FD_2BD  (+2BD)
_seg021_22FD_2BD proc near
        push    ds
        lds     si,dword ptr ds:[186h]
        lodsb
        cmp     al,0FFh
        jne     short L02D6
        mov     si,0C000h
        lodsb
        cmp     al,21h
        jne     short L02D6
        mov     word ptr ss:[120h],0FFFFh
L02D6:
        pop     ds
        ret
_seg021_22FD_2BD endp

SEG021_TEXT ends

        end
