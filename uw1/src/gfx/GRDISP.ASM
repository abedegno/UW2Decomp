; /* target: seg003_5AE8 */
; /* opts: /ml */
;
; Graphics library module 15: the dispatcher that GRMISC's far thunks jump to, and the start of
; the graphics data segment's last piece (the dispatcher's stack words and the Gouraud span
; table).
;
; The dispatcher. A caller outside the library (seg004's 3D code) puts a near routine of seg003
; in BP and its arguments in AX..DX, SI, DI, and far-jumps or calls here. The dispatcher sets DS
; = ES = seg048 (3963, the library's data segment), saves SP at 3963:5588, switches to the
; private stack SS:SP = [558A]:[5586]
; (seg048 itself), calls BP, then puts SS back to seg063 (FD72, seg019's data segment,
; which holds cRender's private stack, the one seg004 runs on) and the saved SP, and
; far-returns. The C wrappers in GRCORE do the same job with their own stack (3963:4FA6) and
; save area (cs:4C70).
;
; The FD52 piece (3963:5588..5E7C): 5588 the saved SP; 558A the private stack's segment (the far
; pointer seg048, so 3963); 558C an 8-byte rectangle buffer _5B5D fills; then, from 5690,
; two hundred 10-byte span records with their y words already filled in (C7h down to 1):
; GRLIBM's Gouraud polygon writes its spans (y, left, right, left intensity, right intensity)
; into these and hands them to GRENTRY's _F77; then terminator words; and at 5E74 four near
; offsets into GRLIBN (645A, 6472, 6486, 6490), the per-case routines GRLIBN's clipper calls
; through [bx+5E74h].
;
; name: descriptive (UW2Decomp's map/filenames.tsv: "graphics library module 11: the dispatcher the thunks
; jump to"). No FM Towns counterpart: the 32-bit build has one flat segment and calls the
; routines directly.
;
; UW1 against UW2: UW2Decomp's src/gfx/GRDISP.ASM (UW2's seg003_0272_52EE), the same code and
; length, and the same far data piece, 2 bytes lower in the data segment (5588..5E7C, UW2
; 558A..5E7E). Only numbers differ, and the four offsets into GRLIBN at the piece's end.
;
; SEG003 split into its original modules: this is module 15 of UW1's 18, +5AE8..+5B69.
; Evidence for the bounds: starts after one zero byte following a jmp (TLINK's padding before a
; word-aligned module), as in UW2.
; Its relocations are entries 2855-2860 of UW.EXE's relocation table. Its FD52 piece (segment
; table entry 52, 3963, the library's data segment; UW2's FD52) holds entry 2854 (the far
; pointer at 5588), which comes right before this module's code entries; it is taken to run
; from there to the end of the segment (5E7C), as in UW2.

        .186

        extrn   seg063:far
        extrn   seg048:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

FD52    segment byte public 'FAR_DATA'
        dd      seg048
        db      104h dup (0)
        db      0C7h
        db      9 dup (0)
        db      0C6h
        db      9 dup (0)
        db      0C5h
        db      9 dup (0)
        db      0C4h
        db      9 dup (0)
        db      0C3h
        db      9 dup (0)
        db      0C2h
        db      9 dup (0)
        db      0C1h
        db      9 dup (0)
        db      0C0h
        db      9 dup (0)
        db      0BFh
        db      9 dup (0)
        db      0BEh
        db      9 dup (0)
        db      0BDh
        db      9 dup (0)
        db      0BCh
        db      9 dup (0)
        db      0BBh
        db      9 dup (0)
        db      0BAh
        db      9 dup (0)
        db      0B9h
        db      9 dup (0)
        db      0B8h
        db      9 dup (0)
        db      0B7h
        db      9 dup (0)
        db      0B6h
        db      9 dup (0)
        db      0B5h
        db      9 dup (0)
        db      0B4h
        db      9 dup (0)
        db      0B3h
        db      9 dup (0)
        db      0B2h
        db      9 dup (0)
        db      0B1h
        db      9 dup (0)
        db      0B0h
        db      9 dup (0)
        db      0AFh
        db      9 dup (0)
        db      0AEh
        db      9 dup (0)
        db      0ADh
        db      9 dup (0)
        db      0ACh
        db      9 dup (0)
        db      0ABh
        db      9 dup (0)
        db      0AAh
        db      9 dup (0)
        db      0A9h
        db      9 dup (0)
        db      0A8h
        db      9 dup (0)
        db      0A7h
        db      9 dup (0)
        db      0A6h
        db      9 dup (0)
        db      0A5h
        db      9 dup (0)
        db      0A4h
        db      9 dup (0)
        db      0A3h
        db      9 dup (0)
        db      0A2h
        db      9 dup (0)
        db      0A1h
        db      9 dup (0)
        db      0A0h
        db      9 dup (0)
        db      9Fh
        db      9 dup (0)
        db      9Eh
        db      9 dup (0)
        db      9Dh
        db      9 dup (0)
        db      9Ch
        db      9 dup (0)
        db      9Bh
        db      9 dup (0)
        db      9Ah
        db      9 dup (0)
        db      99h
        db      9 dup (0)
        db      98h
        db      9 dup (0)
        db      97h
        db      9 dup (0)
        db      96h
        db      9 dup (0)
        db      95h
        db      9 dup (0)
        db      94h
        db      9 dup (0)
        db      93h
        db      9 dup (0)
        db      92h
        db      9 dup (0)
        db      91h
        db      9 dup (0)
        db      90h
        db      9 dup (0)
        db      8Fh
        db      9 dup (0)
        db      8Eh
        db      9 dup (0)
        db      8Dh
        db      9 dup (0)
        db      8Ch
        db      9 dup (0)
        db      8Bh
        db      9 dup (0)
        db      8Ah
        db      9 dup (0)
        db      89h
        db      9 dup (0)
        db      88h
        db      9 dup (0)
        db      87h
        db      9 dup (0)
        db      86h
        db      9 dup (0)
        db      85h
        db      9 dup (0)
        db      84h
        db      9 dup (0)
        db      83h
        db      9 dup (0)
        db      82h
        db      9 dup (0)
        db      81h
        db      9 dup (0)
        db      80h
        db      9 dup (0)
        db      7Fh
        db      9 dup (0)
        db      7Eh
        db      9 dup (0)
        db      7Dh
        db      9 dup (0)
        db      7Ch
        db      9 dup (0)
        db      7Bh
        db      9 dup (0)
        db      7Ah
        db      9 dup (0)
        db      79h
        db      9 dup (0)
        db      78h
        db      9 dup (0)
        db      77h
        db      9 dup (0)
        db      76h
        db      9 dup (0)
        db      75h
        db      9 dup (0)
        db      74h
        db      9 dup (0)
        db      73h
        db      9 dup (0)
        db      72h
        db      9 dup (0)
        db      71h
        db      9 dup (0)
        db      70h
        db      9 dup (0)
        db      6Fh
        db      9 dup (0)
        db      6Eh
        db      9 dup (0)
        db      6Dh
        db      9 dup (0)
        db      6Ch
        db      9 dup (0)
        db      6Bh
        db      9 dup (0)
        db      6Ah
        db      9 dup (0)
        db      69h
        db      9 dup (0)
        db      68h
        db      9 dup (0)
        db      67h
        db      9 dup (0)
        db      66h
        db      9 dup (0)
        db      65h
        db      9 dup (0)
        db      64h
        db      9 dup (0)
        db      63h
        db      9 dup (0)
        db      62h
        db      9 dup (0)
        db      61h
        db      9 dup (0)
        db      60h
        db      9 dup (0)
        db      5Fh
        db      9 dup (0)
        db      5Eh
        db      9 dup (0)
        db      5Dh
        db      9 dup (0)
        db      5Ch
        db      9 dup (0)
        db      5Bh
        db      9 dup (0)
        db      5Ah
        db      9 dup (0)
        db      59h
        db      9 dup (0)
        db      58h
        db      9 dup (0)
        db      57h
        db      9 dup (0)
        db      56h
        db      9 dup (0)
        db      55h
        db      9 dup (0)
        db      54h
        db      9 dup (0)
        db      53h
        db      9 dup (0)
        db      52h
        db      9 dup (0)
        db      51h
        db      9 dup (0)
        db      50h
        db      9 dup (0)
        db      4Fh
        db      9 dup (0)
        db      4Eh
        db      9 dup (0)
        db      4Dh
        db      9 dup (0)
        db      4Ch
        db      9 dup (0)
        db      4Bh
        db      9 dup (0)
        db      4Ah
        db      9 dup (0)
        db      49h
        db      9 dup (0)
        db      48h
        db      9 dup (0)
        db      47h
        db      9 dup (0)
        db      46h
        db      9 dup (0)
        db      45h
        db      9 dup (0)
        db      44h
        db      9 dup (0)
        db      43h
        db      9 dup (0)
        db      42h
        db      9 dup (0)
        db      41h
        db      9 dup (0)
        db      40h
        db      9 dup (0)
        db      3Fh
        db      9 dup (0)
        db      3Eh
        db      9 dup (0)
        db      3Dh
        db      9 dup (0)
        db      3Ch
        db      9 dup (0)
        db      3Bh
        db      9 dup (0)
        db      3Ah
        db      9 dup (0)
        db      39h
        db      9 dup (0)
        db      38h
        db      9 dup (0)
        db      37h
        db      9 dup (0)
        db      36h
        db      9 dup (0)
        db      35h
        db      9 dup (0)
        db      34h
        db      9 dup (0)
        db      33h
        db      9 dup (0)
        db      32h
        db      9 dup (0)
        db      31h
        db      9 dup (0)
        db      30h
        db      9 dup (0)
        db      2Fh
        db      9 dup (0)
        db      2Eh
        db      9 dup (0)
        db      2Dh
        db      9 dup (0)
        db      2Ch
        db      9 dup (0)
        db      2Bh
        db      9 dup (0)
        db      2Ah
        db      9 dup (0)
        db      29h
        db      9 dup (0)
        db      28h
        db      9 dup (0)
        db      27h
        db      9 dup (0)
        db      26h
        db      9 dup (0)
        db      25h
        db      9 dup (0)
        db      24h
        db      9 dup (0)
        db      23h
        db      9 dup (0)
        db      22h
        db      9 dup (0)
        db      21h
        db      9 dup (0)
        db      20h
        db      9 dup (0)
        db      1Fh
        db      9 dup (0)
        db      1Eh
        db      9 dup (0)
        db      1Dh
        db      9 dup (0)
        db      1Ch
        db      9 dup (0)
        db      1Bh
        db      9 dup (0)
        db      1Ah
        db      9 dup (0)
        db      19h
        db      9 dup (0)
        db      18h
        db      9 dup (0)
        db      17h
        db      9 dup (0)
        db      16h
        db      9 dup (0)
        db      15h
        db      9 dup (0)
        db      14h
        db      9 dup (0)
        db      13h
        db      9 dup (0)
        db      12h
        db      9 dup (0)
        db      11h
        db      9 dup (0)
        db      10h
        db      9 dup (0)
        db      0Fh
        db      9 dup (0)
        db      0Eh
        db      9 dup (0)
        db      0Dh
        db      9 dup (0)
        db      0Ch
        db      9 dup (0)
        db      0Bh
        db      9 dup (0)
        db      0Ah
        db      9 dup (0)
        db      09h
        db      9 dup (0)
        db      08h
        db      9 dup (0)
        db      07h
        db      9 dup (0)
        db      06h
        db      9 dup (0)
        db      05h
        db      9 dup (0)
        db      04h
        db      9 dup (0)
        db      03h
        db      9 dup (0)
        db      02h
        db      9 dup (0)
        db      01h
        db      13h dup (0)
        db      0FFh,0FFh
        db      8 dup (0)
        db      0FEh,0FFh
        db      8 dup (0)
        db      5Ah,64h,72h,64h,86h,64h,90h,64h
FD52    ends

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg003_5AB1:near
        extrn   _seg003_5ADE:far
        public  _seg003_5AF9
        public  _seg003_5B0B
        public  _seg003_5B5D
        public  _seg003_5B65

; L5AE8: copy CX words from DS:SI to 3963:558C, then enter _5AF9 with SI = 558C. Reached only
; from _5B5D.
;
; _5AF9 (near, entered by jmp with a far return address on the stack, BP = routine): the
; dispatcher for callers on another stack. If SS already is seg048 the library has been
; re-entered; it then executes int 2 (a break into a debugger) and calls BP on the current
; stack. Otherwise it takes the path at L5B34 below, which ends with DS = ES = seg063
; rather than the caller's values. Used by GRMISC's _18.
L5AE8:
        mov     di,558Ch
        push    ax
        push    es
        mov     ax,seg seg048
        mov     es,ax
        rep movsw
        pop     es
        pop     ax
        mov     si,558Ch
_seg003_5AF9:
        push    ax
        push    bx
        mov     ax,ss
        mov     bx,seg seg048
        cmp     ax,bx
        pop     bx
        pop     ax
        jne     L5B34
        int     2
        call    bp
        retf

; seg003_5B0B  (+5B0B)
; _5B0B (far, entered by GRMISC's thunks with jmp far): save DS and ES, set DS = ES = seg048,
; save SP at DS:5588, switch to the private stack [558A]:[5586] with interrupts off, call BP,
; switch back to SS = seg063 (FD72, seg019's data segment, where cRender's private stack
; is) and the saved SP, restore DS and ES, retf. AX..DX, SI and DI reach the routine unchanged
; and come back as it leaves them.
_seg003_5B0B proc far
        push    ds
        push    es
        push    ax
        mov     ax,seg seg048
        mov     ds,ax
        mov     es,ax
        pop     ax
        mov     word ptr ds:[5588h],sp
        cli
        mov     ss,word ptr ds:[558Ah]
        mov     sp,word ptr ds:[5586h]
        sti
        call    bp
        mov     bp,seg seg063
        cli
        mov     ss,bp
        mov     sp,word ptr ds:[5588h]
        sti
        pop     es
        pop     ds
        retf
L5B34:
        push    ax
        mov     ax,seg seg048
        mov     ds,ax
        mov     es,ax
        pop     ax
        mov     word ptr ds:[5588h],sp
        cli
        mov     ss,word ptr ds:[558Ah]
        mov     sp,word ptr ds:[5586h]
        sti
        call    bp
        mov     bp,seg seg063
        cli
        mov     ss,bp
        mov     sp,word ptr ds:[5588h]
        sti
        mov     ds,bp
        mov     es,bp
        retf
_seg003_5B0B endp

; seg003_5B5D  (+5B5D)
; _5B5D: set the clip window from the 4 words (left, top, right, bottom) at the caller's DS:SI,
; through the dispatcher: CX = 4 words are copied to 3963:558C and the window routine (_5ADE,
; VIDMODE's _3A68) is called with SI pointing at the copy. No caller was found in the sources.
_seg003_5B5D proc near
        mov     cx,4
        mov     bp,offset _seg003_5ADE
        jmp     L5AE8
_seg003_5B5D endp

; seg003_5B65  (+5B65)
; _5B65 (far): call _5AB1 (set_the_color's vector, VIDMODE's _3A16) with the caller's DS and
; stack, without switching; it is only correct when DS already is seg048. No caller was found
; in the sources.
_seg003_5B65 proc far
        call    _seg003_5AB1
        retf
_seg003_5B65 endp

SEG003_TEXT ends

        end
