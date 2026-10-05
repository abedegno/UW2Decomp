; /* target: seg003_0272_52EE */
; /* opts: /ml */
;
; Graphics library module 11: the dispatcher that GRMISC's far thunks jump to, and the start of
; the graphics data segment's last piece (the dispatcher's stack words and the Gouraud span
; table).
;
; The dispatcher. A caller outside the library (seg004's 3D code) puts a near routine of seg003
; in BP and its arguments in AX..DX, SI, DI, and far-jumps or calls here. The dispatcher sets DS
; = ES = seg_370D, saves SP at 370D:558A, switches to the private stack SS:SP = [558C]:[5588]
; (seg_370D itself), calls BP, then puts SS back to dseg062_62a6 (FD71, seg021's data segment,
; which holds cRender's private stack, the one seg004 runs on) and the saved SP, and
; far-returns. The C wrappers in GRCORE do the same job with their own stack (370D:4FA8) and
; save area (cs:43F0).
;
; The FD51 piece (370D:558A..5E7E): 558A the saved SP; 558C the private stack's segment (the far
; pointer seg_370D-8, so 370D); 558E an 8-byte rectangle buffer _5363 fills; then, from 5692,
; two hundred 10-byte span records with their y words already filled in (C7h down to 1):
; GRLIBM's Gouraud polygon writes its spans (y, left, right, left intensity, right intensity)
; into these and hands them to GRENTRY's _C13; then terminator words; and at 5E76 four near
; offsets into GRLIBN (5C5A, 5C72, 5C86, 5C90), the per-case routines GRLIBN's clipper calls
; through [bx+5E76h].
;
; name: descriptive (map/filenames.tsv: "graphics library module 11: the dispatcher the thunks
; jump to"). No FM Towns counterpart: the 32-bit build has one flat segment and calls the
; routines directly.
;
; SEG003 split into its original modules: this is module 11 of 14, +52EE..+5370 of the segment
; (file 873Eh), a module of the second library (UWLIB.LIB).
; Procs: seg003_0272_5311, seg003_0272_5363, seg003_0272_536B.
; Evidence for the bounds: starts after one zero byte following a jmp: TLINK's padding before
; a word-aligned module (TASM 2.0's `even` pads with 90h).
; Its relocations are entries 2568-2573 of UW2.EXE's relocation table.
; Its FD51 piece is 370D:558A..5E7E (relocation entries 2567), defined before the code because
; those entries come before the code's.
;
; Its FD51 piece holds relocation entry 2567 (the far pointer at 558A), which comes right
; before this module's code entries; it is taken to run from there to the end of the segment.

        .186

        extrn   dseg062_62a6:far
        extrn   seg_370D:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

FD51    segment byte public 'FAR_DATA'
        dd      seg_370D-8h
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
        db      5Ah,5Ch,72h,5Ch,86h,5Ch,90h,5Ch
FD51    ends

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg003_0272_52B7:near
        extrn   _seg003_0272_52E4:far
        public  _seg003_0272_52FF
        public  _seg003_0272_5311
        public  _seg003_0272_5363
        public  _seg003_0272_536B

; L52EE: copy CX words from DS:SI to 370D:558E, then enter _52FF with SI = 558E. Reached only
; from _5363.
;
; _52FF (near, entered by jmp with a far return address on the stack, BP = routine): the
; dispatcher for callers on another stack. If SS already is seg_370D the library has been
; re-entered; it then executes int 2 (a break into a debugger) and calls BP on the current
; stack. Otherwise it takes the path at L533A below, which ends with DS = ES = dseg062_62a6
; rather than the caller's values. Used by GRMISC's _18.
L52EE:
        mov     di,558Eh
        push    ax
        push    es
        mov     ax,seg seg_370D
        mov     es,ax
        rep movsw
        pop     es
        pop     ax
        mov     si,558Eh
_seg003_0272_52FF:
        push    ax
        push    bx
        mov     ax,ss
        mov     bx,seg seg_370D
        cmp     ax,bx
        pop     bx
        pop     ax
        jne     L533A
        int     2
        call    bp
        retf

; seg003_0272_5311  (+5311)
; _5311 (far, entered by GRMISC's thunks with jmp far): save DS and ES, set DS = ES = seg_370D,
; save SP at DS:558A, switch to the private stack [558C]:[5588] with interrupts off, call BP,
; switch back to SS = dseg062_62a6 (FD71, seg021's data segment, where cRender's private stack
; is) and the saved SP, restore DS and ES, retf. AX..DX, SI and DI reach the routine unchanged
; and come back as it leaves them.
_seg003_0272_5311 proc far
        push    ds
        push    es
        push    ax
        mov     ax,seg seg_370D
        mov     ds,ax
        mov     es,ax
        pop     ax
        mov     word ptr ds:[558Ah],sp
        cli
        mov     ss,word ptr ds:[558Ch]
        mov     sp,word ptr ds:[5588h]
        sti
        call    bp
        mov     bp,seg dseg062_62a6
        cli
        mov     ss,bp
        mov     sp,word ptr ds:[558Ah]
        sti
        pop     es
        pop     ds
        retf
L533A:
        push    ax
        mov     ax,seg seg_370D
        mov     ds,ax
        mov     es,ax
        pop     ax
        mov     word ptr ds:[558Ah],sp
        cli
        mov     ss,word ptr ds:[558Ch]
        mov     sp,word ptr ds:[5588h]
        sti
        call    bp
        mov     bp,seg dseg062_62a6
        cli
        mov     ss,bp
        mov     sp,word ptr ds:[558Ah]
        sti
        mov     ds,bp
        mov     es,bp
        retf
_seg003_0272_5311 endp

; seg003_0272_5363  (+5363)
; _5363: set the clip window from the 4 words (left, top, right, bottom) at the caller's DS:SI,
; through the dispatcher: CX = 4 words are copied to 370D:558E and the window routine (_52E4,
; VIDMODE's _31E7) is called with SI pointing at the copy. No caller was found in the sources.
_seg003_0272_5363 proc near
        mov     cx,4
        mov     bp,offset _seg003_0272_52E4
        jmp     L52EE
_seg003_0272_5363 endp

; seg003_0272_536B  (+536B)
; _536B (far): call _52B7 (set_the_color's vector, VIDMODE's _3195) with the caller's DS and
; stack, without switching; it is only correct when DS already is seg_370D. No caller was found
; in the sources.
_seg003_0272_536B proc far
        call    _seg003_0272_52B7
        retf
_seg003_0272_536B endp

SEG003_TEXT ends

        end
