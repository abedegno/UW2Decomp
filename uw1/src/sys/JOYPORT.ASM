; /* target: seg019_2E0 */
; /* opts: /ml */
;
; UW1: UW2's module (src/sys in UW2Decomp), matched to UW.EXE's seg019; the differences are
; marked UW1/UW2 where they occur. Other remarks are UW2's unless they say UW1.
;
; The analogue joystick on the game port (201h): finding which of the four axes are connected,
; timing them, turning the counts into positions from -127 to 127 around the centre found at
; start-up, and reading the four buttons. FM Towns has initjoy, joystick and jbuttons here (as
; stubs). The far entries (_2E0, _2E4, _2E8, _369, _36D, _371) are for C (SYSENTRY.ASM's
; joy_position and, in UW2, the debug key Ctrl+J in DEBUG.C).
;
; UW1: SYSINIT.ASM's start-up still finds the axes (_30A), but nothing reads the joystick:
; no C calls SYSENTRY.ASM's _7CD or _809, UW1 has no JOYSTICK.C, and MOUSE.C has no
; joystick code. The module is otherwise unused.
;
; Data (seg063): 01A0 the four positions (-127..127); 01A8 each axis's centre count (0: not
; connected); 01B0 the four buttons (0 or -1); 01B8 the raw counts from the last timing;
; 01C0 a trim subtracted from each position; 01C8 a saved set of positions; 01D0 the number of
; axes connected; 01D2 their mask (bit per axis, as on port 201h).
;
; name: descriptive (module 6 of seg019, "the joystick port").
;
; seg019 split into its original modules, as UW2's seg021: module 6 of 17, +2E0..+450
; of the segment (file 22880h), a module of the second library (UWLIB.LIB).
; Procs: seg019_2E0, seg019_2E4, seg019_2E8, seg019_2EC, seg019_30A,
; seg019_369, seg019_36D, seg019_371, seg019_375, seg019_380,
; seg019_38F, seg019_3A3, seg019_3FD.
; Evidence for the bounds: starts on a paragraph after 8 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG019_TEXT segment para public 'CODE'
        assume  cs:SEG019_TEXT, ds:DGROUP, ss:DGROUP
        public  _seg019_2E0
        public  _seg019_2E4
        public  _seg019_2E8
        public  _seg019_2EC
        public  _seg019_30A
        public  _seg019_369
        public  _seg019_36D
        public  _seg019_371
        public  _seg019_375
        public  _seg019_380
        public  _seg019_38F
        public  _seg019_3A3
        public  _seg019_3FD

; seg019_2E0  (+2E0)
; _2E0: far entry to _30A (find and centre the axes).
_seg019_2E0 proc far
        call    _seg019_30A
        retf
_seg019_2E0 endp

; seg019_2E4  (+2E4)
; _2E4: far entry to _3A3 (read the positions).
_seg019_2E4 proc far
        call    _seg019_3A3
        retf
_seg019_2E4 endp

; seg019_2E8  (+2E8)
; _2E8: far entry to _2EC (read the buttons).
_seg019_2E8 proc far
        call    _seg019_2EC
        retf
_seg019_2E8 endp

; seg019_2EC  (+2EC)
; _2EC (jbuttons, probably): read port 201h and store each button (bits 4-7, pressed when 0) as
; -1 or 0 at 01B0.., for the axes that are connected.
_seg019_2EC proc near
        mov     dx,201h
        in      al,dx
        not     al
        mov     ah,al
        mov     di,1B6h
        mov     cx,4
L02FA:
        cmp     word ptr [di-8],0
        je      short L0303
        cwd
        mov     word ptr [di],dx
L0303:
        shl     ax,1
        dec     di
        dec     di
        loop    L02FA
        ret
_seg019_2EC endp

; seg019_30A  (+30A)
; _30A (initjoy, probably): clear the data, time all four axes once (_3FD), and keep each count
; between 10h and 3E8h as that axis's centre; an axis outside that range is not connected (and
; when seg063:0120 is set, the first such axis drops all the rest too; 0120 is also read by
; KEYQUEUE.ASM's _46B, meaning not known). Sets 01D0 and 01D2.
_seg019_30A proc near
        sub     ax,ax
        mov     di,1A0h
        mov     cx,10h
        rep stosw
        mov     byte ptr ds:[1D2h],0Fh
        mov     word ptr ds:[1D0h],4
        call    _seg019_3FD
        mov     si,1B8h
        mov     di,1A8h
        mov     cx,4
L032B:
        lodsw
        cmp     ax,10h
        jb      short L0336
        cmp     ax,3E8h
        jbe     short L034B
L0336:
        sub     ax,ax
        cmp     word ptr ds:[120h],0
        je      short L0347
        sub     word ptr ds:[1D0h],cx
        rep stosw
        jmp     short L034E
L0347:
        dec     word ptr ds:[1D0h]
L034B:
        stosw
        loop    L032B
L034E:
        mov     cx,4
        mov     bx,1
        sub     ax,ax
        mov     dx,ax
        mov     di,1A8h
L035B:
        scasw
        je      short L0360
        or      dx,bx
L0360:
        shl     bx,1
        loop    L035B
        mov     byte ptr ds:[1D2h],dl
        ret
_seg019_30A endp

; seg019_369  (+369)
; _369, _36D, _371: far entries to _375, _380 and _38F.
_seg019_369 proc far
        call    _seg019_375
        retf
_seg019_369 endp

; seg019_36D  (+36D)
_seg019_36D proc far
        call    _seg019_380
        retf
_seg019_36D endp

; seg019_371  (+371)
_seg019_371 proc far
        call    _seg019_38F
        retf
_seg019_371 endp

; seg019_375  (+375)
; _375: clear the trims at 01C0.
_seg019_375 proc near
        mov     di,1C0h
        sub     ax,ax
        mov     cx,4
        rep stosw
        ret
_seg019_375 endp

; seg019_380  (+380)
; _380: read the positions and save them at 01C8.
_seg019_380 proc near
        call    _seg019_3A3
        mov     si,1A0h
        mov     di,1C8h
        mov     cx,4
        rep movsw
        ret
_seg019_380 endp

; seg019_38F  (+38F)
; _38F: read the positions and store at 01C0 each one less its saved value (01C8). Probably
; calibration steps of a joystick set-up screen; no caller in the C sources was traced.
_seg019_38F proc near
        call    _seg019_3A3
        mov     si,1A0h
        mov     di,1C0h
        mov     cx,4
L039B:
        lodsw
        sub     ax,word ptr [di+8]
        stosw
        loop    L039B
        ret
_seg019_38F endp

; seg019_3A3  (+3A3)
; _3A3 (joystick, probably): time the axes (_3FD) and, for each connected one, position = (count
; - centre) * 128 / centre - trim, with a dead zone of 4 around 0 and clamped to -127..127,
; stored at 01A0.
_seg019_3A3 proc near
        cmp     word ptr ds:[1D0h],0
        je      short L03EB
        call    _seg019_3FD
        mov     cx,4
        mov     si,1B8h
        mov     di,1A0h
        mov     bx,80h
L03B9:
        cmp     word ptr [si-10h],0
        je      short L03EC
        lodsw
        sub     ax,word ptr [di+8]
        imul    bx
        idiv    word ptr [di+8]
        sub     ax,word ptr [di+20h]
        cmp     ax,0
        jge     short L03D7
        add     ax,4
        jge     short L03DC
        jmp     short L03DE
L03D7:
        sub     ax,4
        jge     short L03DE
L03DC:
        sub     ax,ax
L03DE:
        cmp     ax,7Fh
        jg      short L03F8
        cmp     ax,0FF81h
        jl      short L03F3
L03E8:
        stosw
        loop    L03B9
L03EB:
        ret
L03EC:
        inc     si
        inc     si
        inc     di
        inc     di
        loop    L03B9
        ret
L03F3:
        mov     ax,0FF81h
        jmp     L03E8
L03F8:
        mov     ax,7Fh
        jmp     L03E8
_seg019_3A3 endp

; seg019_3FD  (+3FD)
; _3FD: time the connected axes: write to port 201h to start the one-shots, then with interrupts
; off count down until each axis bit falls, recording the count each time a bit changes; the
; counts go to 01B8.. for the axes that fell. CX running out (no change) ends the loop.
_seg019_3FD proc near
        mov     ah,byte ptr ds:[1D2h]
        sub     cx,cx
        mov     si,cx
        mov     dx,201h
        in      al,dx
        or      al,ah
        mov     bl,al
        cli
        out     dx,al
        jmp     short L0411
L0411:
        in      al,dx
        cmp     al,bl
        loope   L0411
        jcxz    L0429
        mov     bl,al
        not     al
        and     al,ah
        loope   L0411
        xor     ah,al
        inc     si
        push    cx
        push    ax
        test    ah,ah
        jne     L0411
L0429:
        sti
        test    si,si
        je      short L0445
L042E:
        pop     dx
        pop     ax
        neg     ax
        mov     cx,4
        mov     di,1B8h
L0438:
        sar     dx,1
        jae     short L043E
        mov     word ptr [di],ax
L043E:
        inc     di
        inc     di
        loop    L0438
        dec     si
        jne     L042E
L0445:
        ret
_seg019_3FD endp

SEG019_TEXT ends

        end
