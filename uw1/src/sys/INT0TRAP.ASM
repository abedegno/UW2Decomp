; /* target: seg016 */
; /* opts: /ml */
;
; The divide-by-zero trap: segment table entry 18, 1DEA:0002-0061, between seg015 and
; seg017. UWEDIT.C's start-up code (ovr109) saves the int 0 vector, points it at
; int0_trap, and stores its own SS and SP in int0_ss and int0_sp; div_zero_ovr112_661
; (also UWEDIT.C) is the C side of the trap.
;
; It is UW2's src/sys/INT0TRAP.ASM byte for byte, apart from the address of the C
; handler (UW1's ovr109 stub, UW2's ovr112).
;
; Hand-written assembly, not C: its variables live in the code segment and are reached
; with cs: overrides, it ends in iret after popping the registers it pushed (a Turbo C
; interrupt function pushes ax bx cx dx es ds si di bp and loads DS from DGROUP@ instead),
; and it switches stacks by hand.
;
; match: only 8086 instructions, so no .186. The segment starts right after seg015's code
; (1DEA:0002). That address is even, so byte or word alignment would both put it there; it
; is byte-aligned as UW2's (whose start is odd), so its segments are written out in full
; (`.model medium` would make it word-aligned).
;
; What it does: note the faulting stack (trap_sp, trap_ss), save the registers, switch to
; the stack the game was started on (int0_ss:int0_sp, with DS = SS, DGROUP), call
; div_zero_ovr112_661(trap_sp, trap_ss), switch back and return from the interrupt.
;
; name: descriptive, as UW2's. int0_trap, int0_ss and int0_sp are symbols.tsv's
; (provisional), div_zero_ovr112_661 is UWEDIT.C's. The other four variables are unnamed
; in every source and are named here for what they hold.

        extrn   _div_zero_ovr112_661:far

DGROUP  group   _DATA
_DATA   segment byte public 'DATA'
_DATA   ends

SEG016_TEXT segment byte public 'CODE'
        assume  cs:SEG016_TEXT, ds:nothing, es:nothing

        public  _int0_ss
        public  _int0_sp
        public  _int0_trap

save_ss         dw      0               ; the stack the trap ran on, while the C handler runs
save_sp         dw      0
_int0_ss        dw      0               ; the game's own stack, set by UWEDIT.C at start-up
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

SEG016_TEXT ends
        end
