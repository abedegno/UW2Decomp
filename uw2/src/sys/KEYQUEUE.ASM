; /* target: seg021_22FD_450 */
; /* opts: /ml */
;
; Reading the keyboard: turning the raw scan codes KBDINT.ASM's int 9 handler queues into key
; events. It reads the ring buffer at FD71:0300..033F (the read pointer at 026C, the write
; pointer the handler keeps in _590), assembles the E0 and E1 prefixed sequences of the extended
; keys, keeps a down count for every key (01EB + scan code), ignores the hardware autorepeat
; unless 01E0 is set, works out the shift states, and translates scan codes to characters
; through the table at FD71:0010 (256 entries: the scan code, plus 80h when shifted). _450 is
; the far entry; FM Towns has key and key_ here.
;
; Data (FD71): 01E0 allow repeats; 01E2..01E7 the six modifier states (_51A); 01E8 the lock
; state; 01E9 the keyboard layout (0, 2 or 4; see _46B); 026B the byte after an E0 prefix;
; 026E the 13 scan codes _454 remaps for layout 4, and their replacements 12 bytes on;
; 0288 the 16 E0-prefixed scan codes the game uses; 0299, 02A1 the two keys of each modifier,
; by layout.
;
; name: descriptive (module 7 of seg021, "the keyboard input queue").
;
; SEG021 split into its original modules: this is module 7 of 17, +450..+590 of the segment
; (file 24150h), a module of the second library (UWLIB.LIB).
; Procs: seg021_22FD_450, seg021_22FD_454, seg021_22FD_46B, seg021_22FD_4A8, seg021_22FD_51A,
; seg021_22FD_552, seg021_22FD_562, seg021_22FD_577.
; Evidence for the bounds: starts on a paragraph after 10 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG021_TEXT segment para public 'CODE'
        assume  cs:SEG021_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg021_22FD_590:near
        public  _seg021_22FD_450
        public  _seg021_22FD_454
        public  _seg021_22FD_46B
        public  _seg021_22FD_4A8
        public  _seg021_22FD_51A
        public  _seg021_22FD_552
        public  _seg021_22FD_562
        public  _seg021_22FD_577

; seg021_22FD_450  (+450)
; _450 (key, probably): far entry to _4A8.
_seg021_22FD_450 proc far
        call    _seg021_22FD_4A8
        retf
_seg021_22FD_450 endp

; seg021_22FD_454  (+454)
; _454: remap AL (top bit kept) through the 13-entry table at 026E, for layout 4.
_seg021_22FD_454 proc near
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
_seg021_22FD_454 endp

; seg021_22FD_46B  (+46B)
; _46B: choose the keyboard layout once (when 01E9 is 0): 2 when the BIOS reports an enhanced
; 101/102-key keyboard (0040:0096 bit 4), else 4 when FD71:0120 is set, else 0. FD71:0120 is
; probably a setting from the command line or configuration (not traced). The code after the ret
; (L049B, L049E) belongs to _4A8: no key, and the Pause sequence (E1), returned as E0h.
_seg021_22FD_46B proc near
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
        call    _seg021_22FD_577
        call    _seg021_22FD_577
        mov     al,0E0h
        jmp     short L04DA
_seg021_22FD_46B endp

; seg021_22FD_4A8  (+4A8)
; _4A8: the next key event. Takes a complete scan-code sequence from the buffer (_562; none: AX
; = 0); an E0-prefixed key becomes an index into the table at 0288 (the fake shifts E0 2A and E0
; 36 are skipped). For a key going down, counts it (a key already down is ignored unless 01E0 is
; set), translates it with the shift state, and updates the modifiers; for a key going up (top
; bit), clears its count and the modifiers and reads on. Out: AL = the translated character, AH
; = the scan code, BX = 1.
_seg021_22FD_4A8 proc near
        call    _seg021_22FD_562
        jb      L049B
        cmp     al,0E1h
        je      L049E
        cmp     al,0E0h
        jne     short L04DA
        call    _seg021_22FD_577
        mov     byte ptr ds:[26Bh],al
        and     al,7Fh
        cmp     al,2Ah
        je      _seg021_22FD_4A8
        cmp     al,36h
        je      _seg021_22FD_4A8
        mov     di,288h
        mov     cx,10h
        repne scasb
        jne     _seg021_22FD_4A8
        sub     di,229h
        mov     al,byte ptr ds:[26Bh]
        and     al,80h
        or      ax,di
L04DA:
        cmp     word ptr ds:[1E9h],4
        jne     short L04E4
        call    _seg021_22FD_454
L04E4:
        sub     ah,ah
        mov     bx,ax
        and     bl,7Fh
        test    al,80h
        jne     short L053B
        cmp     word ptr ds:[1E0h],0
        jne     short L04FD
        cmp     byte ptr [bx+1EBh],0
        jne     _seg021_22FD_4A8
L04FD:
        inc     byte ptr [bx+1EBh]
        cmp     byte ptr ds:[1E2h],0
        je      short L050B
        or      bl,80h
L050B:
        ; match: mov ah,[bx+10h] with a 16-bit displacement: the source named a relocatable
        ; label (offset 10h in the module's data segment), which always takes 16 bits.
        ; A plain number gives 8A 67 10, and a forward equate 8A 67 10 90.
        db      8Ah,0A7h,10h,0h
        push    ax
        call    _seg021_22FD_51A
        pop     ax
        sub     bx,bx
        xchg    al,ah
        inc     bx
        ret
_seg021_22FD_4A8 endp

; seg021_22FD_51A  (+51A)
; _51A: recompute the six modifier states at 01E2: each is down if either of its two keys (from
; the tables at 0299 and 02A1 for the current layout) is down. L053B is _4A8's key-up path.
_seg021_22FD_51A proc near
        mov     cx,6
        mov     bx,word ptr ds:[1E9h]
        mov     di,1E2h
L0524:
        mov     si,word ptr [bx+299h]
        mov     al,byte ptr [si+1EBh]
        mov     si,word ptr [bx+2A1h]
        or      al,byte ptr [si+1EBh]
        stosb
        add     bx,10h
        loop    L0524
        ret
L053B:
        mov     byte ptr [bx+1EBh],0
        call    _seg021_22FD_51A
        jmp     _seg021_22FD_4A8
L0546:
        call    _seg021_22FD_552
        je      short L058B
L054B:
        call    _seg021_22FD_552
        je      short L058B
        jmp     short _seg021_22FD_577
_seg021_22FD_51A endp

; seg021_22FD_552  (+552)
; _552: step the buffer index SI (wrapping at 0340h) and compare it with the handler's write
; pointer (ZF: no more bytes yet).
_seg021_22FD_552 proc near
        inc     si
        cmp     si,340h
        jne     short L055C
        mov     si,300h
L055C:
        cmp     si,word ptr cs:_seg021_22FD_590
        ret
_seg021_22FD_552 endp

; seg021_22FD_562  (+562)
; _562: is a complete sequence waiting? Carry set when the buffer is empty or an E0 or E1
; sequence has not all arrived (checked with _552), else falls into _577 for its first byte.
_seg021_22FD_562 proc near
        mov     si,word ptr ds:[26Ch]
        cmp     si,word ptr cs:_seg021_22FD_590
        je      short L058B
        mov     al,byte ptr [si]
        cmp     al,0E0h
        je      L054B
        cmp     al,0E1h
        je      L0546
_seg021_22FD_562 endp

; seg021_22FD_577  (+577)
; _577: AL = the next byte of the buffer, advancing the read pointer 026C (wrapping at 0340h);
; carry clear.
_seg021_22FD_577 proc near
        mov     si,word ptr ds:[26Ch]
        lodsb
        cmp     si,340h
        jne     short L0585
        mov     si,300h
L0585:
        mov     word ptr ds:[26Ch],si
        clc
        ret
L058B:
        stc
        ret
_seg021_22FD_577 endp

SEG021_TEXT ends

        end
