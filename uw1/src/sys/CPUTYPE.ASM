; /* target: seg019_1D0 */
; /* opts: /ml */
;
; UW1: UW2's module (src/sys in UW2Decomp), matched to UW.EXE's seg019; the differences are
; marked UW1/UW2 where they occur. Other remarks are UW2's unless they say UW1.
;
; CPU detection (_1D0): 8086, 80186, 80286 or 80386, stored at offset 110h of the module data
; segment.
;
; seg019 split into its original modules, as UW2's seg021: module 3 of 17, +1D0..+220
; of the segment (file 22770h), a module of the second library (UWLIB.LIB).
; Procs: seg019_1D0.
; Evidence for the bounds: starts on a paragraph after 13 zero bytes following a iret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.
; name: descriptive (system module 3, CPU detection).

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG019_TEXT segment para public 'CODE'
        assume  cs:SEG019_TEXT, ds:DGROUP, ss:DGROUP
        public  _seg019_1D0

; seg019_1D0  (+1D0)
; Identifies the processor with the classic flag and shift tests and stores the result in
; DS:110h (seg019's data segment): 386h if FLAGS bits 12-14 can be set (386), 286h if
; they cannot but bits 12-15 do not stick at 1 (286); otherwise 186h if `shl ax,cl` with
; CL = 21h leaves a bit (186 and later mask the count to 5 bits), 30h if `mul al` of 40h
; leaves ZF set (presumably the NEC V20/V30 test, the flags after MUL being undefined on
; Intel parts, so "30" for V30), and 86h for an 8086/8088. Called by init; nothing in the
; C sources reads the result, but VIDMODE.ASM's init_graphics copies it and writes the
; palette with rep outsb from 186h up.
_seg019_1D0 proc near
        pushf
        xor     ax,ax
        push    ax
        popf
        pushf
        pop     ax
        and     ax,0F000h
        cmp     ax,0F000h
        je      short L01F5
        mov     ax,7000h
        push    ax
        popf
        pushf
        pop     ax
        and     ax,7000h
        je      short L01F0
        mov     ax,386h
        jmp     short L0216
L01F0:
        mov     ax,286h
        jmp     short L0216
L01F5:
        push    cx
        mov     ax,0FFFFh
        mov     cl,21h
        shl     ax,cl
        jne     short L0212
        pop     cx
        xor     al,al
        mov     al,40h
        mul     al
        je      short L020D
        mov     ax,86h
        jmp     short L0216
L020D:
        mov     ax,30h
        jmp     short L0216
L0212:
        mov     ax,186h
        pop     cx
L0216:
        popf
        mov     word ptr ds:[110h],ax
        ret
_seg019_1D0 endp

SEG019_TEXT ends

        end
