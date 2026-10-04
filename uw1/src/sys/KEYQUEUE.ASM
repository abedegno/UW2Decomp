; /* target: seg019_450 */
; /* opts: /ml */
;
; UW1: UW2's module (src/sys in UW2Decomp), matched to UW.EXE's seg019; the differences are
; marked UW1/UW2 where they occur. Other remarks are UW2's unless they say UW1.
;
; Reading the keyboard: turning the raw scan codes KBDINT.ASM's int 9 handler queues into key
; events. It reads the ring buffer at seg063:0300..033F (the read pointer at 026C, the write
; pointer the handler keeps in _590), assembles the E0 and E1 prefixed sequences of the extended
; keys, keeps a down count for every key (01EB + scan code), ignores the hardware autorepeat
; unless 01E0 is set, works out the shift states, and translates scan codes to characters
; through the table at seg063:0010 (256 entries: the scan code, plus 80h when shifted). _450 is
; the far entry; FM Towns has key and key_ here.
;
; Data (seg063): 01E0 allow repeats; 01E2..01E7 the six modifier states (_51C); 01E8 the lock
; state; 01E9 the keyboard layout (0, 2 or 4; see _46B); 026B the byte after an E0 prefix;
; 026E the 13 scan codes _454 remaps for layout 4, and their replacements 12 bytes on;
; 0288 the 16 E0-prefixed scan codes the game uses; 0299, 02A1 the two keys of each modifier,
; by layout.
;
; name: descriptive (module 7 of seg019, "the keyboard input queue").
;
; seg019 split into its original modules, as UW2's seg021: module 7 of 17, +450..+590
; of the segment (file 229F0h), a module of the second library (UWLIB.LIB).
; Procs: seg019_450, seg019_454, seg019_46B, seg019_4A8, seg019_51C,
; seg019_554, seg019_564, seg019_579.
; Evidence for the bounds: starts on a paragraph after 10 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG019_TEXT segment para public 'CODE'
        assume  cs:SEG019_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg019_590:near
        public  _seg019_450
        public  _seg019_454
        public  _seg019_46B
        public  _seg019_4A8
        public  _seg019_51C
        public  _seg019_554
        public  _seg019_564
        public  _seg019_579

; seg019_450  (+450)
; _450 (key, probably): far entry to _4A8.
_seg019_450 proc far
        call    _seg019_4A8
        retf
_seg019_450 endp

; seg019_454  (+454)
; _454: remap AL (top bit kept) through the 13-entry table at 026E, for layout 4.
_seg019_454 proc near
        mov     ah,al
        and     ah,80h
        and     al,7Fh
        mov     di,26Eh
        mov     cx,0Dh
        repne scasb
        jne     short L0468
        mov     al,byte ptr [di+0Ch]
L0468:
        or      al,ah
        ret
_seg019_454 endp

; seg019_46B  (+46B)
; _46B: choose the keyboard layout once (when 01E9 is 0): 2 when the BIOS reports an enhanced
; 101/102-key keyboard (0040:0096 bit 4), else 4 when seg063:0120 is set, else 0. seg063:0120 is
; probably a setting from the command line or configuration (not traced). The code after the ret
; (L049B, L049E) belongs to _4A8: no key, and the Pause sequence (E1), returned as E0h.
_seg019_46B proc near
        cmp     word ptr ds:[1E9h],0
        jne     short L048C
        push    ds
        sub     ax,ax
        mov     ds,ax
        and     byte ptr ds:[496h],10h
        pop     ds
        jne     short L0494
        cmp     word ptr ds:[120h],0
        jne     short L048D
        mov     word ptr ds:[1E9h],0
L048C:
        ret
L048D:
        mov     word ptr ds:[1E9h],4
        ret
L0494:
        mov     word ptr ds:[1E9h],2
        ret
L049B:
        sub     ax,ax
        ret
L049E:
        call    _seg019_579
        call    _seg019_579
        mov     al,0E0h
        jmp     short L04DA
_seg019_46B endp

; seg019_4A8  (+4A8)
; _4A8: the next key event. Takes a complete scan-code sequence from the buffer (_564; none: AX
; = 0); an E0-prefixed key becomes an index into the table at 0288 (the fake shifts E0 2A and E0
; 36 are skipped). For a key going down, counts it (a key already down is ignored unless 01E0 is
; set), translates it with the shift state, and updates the modifiers; for a key going up (top
; bit), clears its count and the modifiers and reads on. Out: AL = the translated character, AH
; = the scan code, BX = 1.
_seg019_4A8 proc near
        call    _seg019_564
        jb      L049B
        cmp     al,0E1h
        je      L049E
        cmp     al,0E0h
        jne     short L04DA
        call    _seg019_579
        mov     byte ptr ds:[26Bh],al
        and     al,7Fh
        cmp     al,2Ah
        je      _seg019_4A8
        cmp     al,36h
        je      _seg019_4A8
        mov     di,288h
        mov     cx,10h
        repne scasb
        jne     _seg019_4A8
        sub     di,229h
        mov     al,byte ptr ds:[26Bh]
        and     al,80h
        or      ax,di
L04DA:
        cmp     word ptr ds:[1E9h],4
        jne     short L04E4
        call    _seg019_454
L04E4:
        sub     ah,ah
        mov     bx,ax
        and     bl,7Fh
        test    al,80h
        jne     short L053D
        cmp     word ptr ds:[1E0h],0
        jne     short L04FD
        cmp     byte ptr [bx+1EBh],0
        jne     _seg019_4A8
L04FD:
        inc     byte ptr [bx+1EBh]
        cmp     byte ptr ds:[1E2h],0
        je      short L050D
        ; UW1 only: an int 2 (the NMI vector) before the shift, taken whenever a shifted key
        ; goes down; UW2 dropped it. Probably a debugging breakpoint left in.
        int     2
        or      bl,80h
L050D:
        ; match: mov ah,[bx+10h] with a 16-bit displacement: the source named a relocatable
        ; label (offset 10h in the module's data segment), which always takes 16 bits.
        ; A plain number gives 8A 67 10, and a forward equate 8A 67 10 90.
        db      8Ah,0A7h,10h,0h
        push    ax
        call    _seg019_51C
        pop     ax
        sub     bx,bx
        xchg    al,ah
        inc     bx
        ret
_seg019_4A8 endp

; seg019_51C  (+51C)
; _51C: recompute the six modifier states at 01E2: each is down if either of its two keys (from
; the tables at 0299 and 02A1 for the current layout) is down. L053D is _4A8's key-up path.
_seg019_51C proc near
        mov     cx,6
        mov     bx,word ptr ds:[1E9h]
        mov     di,1E2h
L0526:
        mov     si,word ptr [bx+299h]
        mov     al,byte ptr [si+1EBh]
        mov     si,word ptr [bx+2A1h]
        or      al,byte ptr [si+1EBh]
        stosb
        add     bx,10h
        loop    L0526
        ret
L053D:
        mov     byte ptr [bx+1EBh],0
        call    _seg019_51C
        jmp     _seg019_4A8
L0548:
        call    _seg019_554
        je      short L058D
L054D:
        call    _seg019_554
        je      short L058D
        jmp     short _seg019_579
_seg019_51C endp

; seg019_554  (+554)
; _554: step the buffer index SI (wrapping at 0340h) and compare it with the handler's write
; pointer (ZF: no more bytes yet).
_seg019_554 proc near
        inc     si
        cmp     si,340h
        jne     short L055E
        mov     si,300h
L055E:
        cmp     si,word ptr cs:_seg019_590
        ret
_seg019_554 endp

; seg019_564  (+564)
; _564: is a complete sequence waiting? Carry set when the buffer is empty or an E0 or E1
; sequence has not all arrived (checked with _554), else falls into _579 for its first byte.
_seg019_564 proc near
        mov     si,word ptr ds:[26Ch]
        cmp     si,word ptr cs:_seg019_590
        je      short L058D
        mov     al,byte ptr [si]
        cmp     al,0E0h
        je      L054D
        cmp     al,0E1h
        je      L0548
_seg019_564 endp

; seg019_579  (+579)
; _579: AL = the next byte of the buffer, advancing the read pointer 026C (wrapping at 0340h);
; carry clear.
_seg019_579 proc near
        mov     si,word ptr ds:[26Ch]
        lodsb
        cmp     si,340h
        jne     short L0587
        mov     si,300h
L0587:
        mov     word ptr ds:[26Ch],si
        clc
        ret
L058D:
        stc
        ret
_seg019_579 endp

SEG019_TEXT ends

        end
