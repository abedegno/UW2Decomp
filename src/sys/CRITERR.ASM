; /* target: seg021_22FD_150 */
; /* opts: /ml */
;
; The DOS critical-error handler: setvect24 hooks int 24h with INT24, _174 restores it,
; diskerr returns and clears the code INT24 left.
;
; SEG021 split into its original modules: this is module 2 of 17, +150..+1D0 of the segment
; (file 23E50h), a module of the second library (UWLIB.LIB).
; Procs: setvect24, seg021_22FD_174, diskerr.
; Evidence for the bounds: starts on a paragraph after 9 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.
; Runs with DS on seg021's data segment (see STARTUP.ASM), though nothing here uses it: the
; saved vector and the error code live in the code segment.
; name: inferred, System Shock's critical-error handler is CRITERR.C (critical_error).

; The DOS (int 21h) functions used here.
DOS_SET_VECTOR  equ     25h
DOS_GET_VECTOR  equ     35h

        .186

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG021_TEXT segment para public 'CODE'
        assume  cs:SEG021_TEXT, ds:DGROUP, ss:DGROUP
        public  _diskerr
        public  _seg021_22FD_174
        public  _setvect24

; seg021_22FD_150  (+150)
; Saves the int 24h vector in L0170:L0172 (segment, offset) and points it at INT24. Called
; by init (SYSINIT.ASM).
_setvect24 proc near
        push    bp
        push    ds
        push    es
        mov     ax,(DOS_GET_VECTOR shl 8) + 24h
        int     21h
        mov     word ptr cs:L0170,es
        mov     word ptr cs:L0172,bx
        push    cs
        pop     ds
        mov     dx,offset INT24
        mov     ax,(DOS_SET_VECTOR shl 8) + 24h
        int     21h
        pop     es
        pop     ds
        pop     bp
        ret
L0170:
        db      0h,0h
L0172:
        db      0h,0h
_setvect24 endp

; seg021_22FD_174  (+174)
; Puts the saved int 24h vector back. Called at shutdown by _17 (STARTUP.ASM).
_seg021_22FD_174 proc near
        push    bp
        push    ds
        mov     ax,word ptr cs:L0170
        push    ax
        pop     ds
        mov     dx,word ptr cs:L0172
        mov     ax,(DOS_SET_VECTOR shl 8) + 24h
        int     21h
        pop     ds
        pop     bp
        ret
_seg021_22FD_174 endp

; seg021_22FD_189  (+189)
; Returns in AX the code INT24 recorded (0 if none) and clears it. Reached from C only
; through an unnamed far entry in SYSENTRY.ASM, which nothing in the sources calls.
_diskerr proc near
        mov     al,byte ptr cs:L0195
        xor     ah,ah
        mov     byte ptr cs:L0195,ah
        ret
L0195:
        db      0h                              ; the error code _diskerr hands back

; The DOS critical-error handler (int 24h): remember why and tell DOS to ignore the error
; (AL = 0), so no "Abort, Retry, Fail" prompt appears over the game. DOS passes the error
; code in DI and, for a non-disk error (AH bit 7), the device header in BP:SI; if that
; device is not a character device (attribute bit 15 clear: a bad FAT on a block device),
; the code becomes 0Dh. The code plus 14h is kept in L0195, unless one is already there.
INT24:
        cmp     byte ptr cs:L0195,0
        jne     short L01C0
        and     ah,80h
        cmp     ah,0
        je      short L01B8
        push    ds
        push    bp
        pop     ds
        mov     ax,word ptr [si+4]
        pop     ds
        and     ax,8000h
        cmp     ax,0
        jne     short L01B8
        mov     di,0Dh
L01B8:
        mov     ax,di
        add     al,14h
        mov     byte ptr cs:L0195,al
L01C0:
        mov     al,0
        iret
_diskerr endp

SEG021_TEXT ends

        end
