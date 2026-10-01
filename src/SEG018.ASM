; /* target: seg018_21B4 */
; /* opts: /ml */
;
; The divide-by-zero trap: segment table entry 20, 1FC7:000D-006C, between seg017 and
; SetPnt. ovr112's start-up code (OVR112.C) saves the int 0 vector, points it at
; int0_trap, and stores its own SS and SP in int0_ss and int0_sp; DivideByZeroError
; (also OVR112.C) is the C side of the trap.
;
; Hand-written assembly, not C: its variables live in the code segment and are reached
; with cs: overrides, it ends in iret after popping the registers it pushed (a Turbo C
; interrupt function pushes ax bx cx dx es ds si di bp and loads DS from DGROUP@ instead),
; and it switches stacks by hand. Only 8086 instructions, so no .186. The segment starts
; at an odd address right after seg017, so it is byte-aligned, written out in full
; (`.model medium` would make it word-aligned).
;
; What it does: note the faulting stack (trap_sp, trap_ss), save the registers, switch to
; the stack the game was started on (int0_ss:int0_sp, with DS = SS, DGROUP), call
; DivideByZeroError(trap_sp, trap_ss), switch back and return from the interrupt.
;
; Names: int0_trap, int0_ss and int0_sp are symbols.tsv's (provisional; FM Towns has no
; counterpart, being a different machine), div_zero_ovr112_661 likewise. The
; other four variables are unnamed in every source and are named here for what they
; hold.

        extrn   _div_zero_ovr112_661:far

DGROUP  group   _DATA
_DATA   segment byte public 'DATA'
_DATA   ends

SEG018_TEXT segment byte public 'CODE'
        assume  cs:SEG018_TEXT, ds:nothing, es:nothing

        public  _int0_ss
        public  _int0_sp
        public  _int0_trap

save_ss         dw      0               ; the stack the trap ran on, while the C handler runs
save_sp         dw      0
_int0_ss        dw      0               ; the game's own stack, set by ovr112 at start-up
_int0_sp        dw      0
trap_sp         dw      0               ; SS:SP at the fault: the interrupt frame
trap_ss         dw      0

_int0_trap proc far
        mov     cs:trap_sp,sp
        mov     cs:trap_ss,ss
        push    ax
        push    bx
        push    cx
        push    dx
        push    di
        push    si
        push    ds
        push    es
        cli
        mov     ax,ss
        mov     cs:save_ss,ax
        mov     cs:save_sp,sp
        mov     ax,cs:_int0_sp
        mov     sp,ax
        mov     ax,cs:_int0_ss
        mov     ss,ax
        sti
        mov     ds,ax
        push    cs:trap_ss
        push    cs:trap_sp
        call    _div_zero_ovr112_661
        pop     ax
        pop     ax
        cli
        mov     ss,cs:save_ss
        mov     sp,cs:save_sp
        sti
        pop     es
        pop     ds
        pop     si
        pop     di
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        iret
_int0_trap endp

SEG018_TEXT ends
        end
