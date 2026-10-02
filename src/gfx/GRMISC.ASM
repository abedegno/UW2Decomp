; /* target: seg003_0272_0 */
; /* opts: /ml */
;
; The dispatcher thunks (_0, _8, _10: far jumps into _5311 in GRDISP), the vertical retrace
; wait (_28, _46) and the debug break with its 'map bitmap error' message (_seg003_0272_60,
; which FD58's mapper_rtn points at).
;
; SEG003 split into its original modules: this is module 1 of 14, +0..+EC of the segment (file
; 3450h), a module of the second library (UWLIB.LIB).
; Procs: seg003_0272_0, seg003_0272_8, seg003_0272_10, seg003_0272_18, seg003_0272_28,
; seg003_0272_46.
; Evidence for the bounds: it starts the segment. It ends at +EC: its relocations (entries
; 2367-2371, +6..+49) come between seg004's first group and seg021's last module, while the
; next ones in this segment (+21D onwards) come after seg045's, so a module boundary lies
; between +4A and +21D, and +EC is the only routine start there on a word boundary (+183 is
; odd). Its three far jumps to _5311 use EA, which TASM would shorten within a module.
; Its relocations are entries 2367-2371 of UW2.EXE's relocation table.

        .186

        extrn   _seg021_22FD_C00:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'CODE'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg003_0272_32B4:near
        extrn   _seg003_0272_368E:near
        extrn   _seg003_0272_375C:near
        extrn   _seg003_0272_52FF:near
        extrn   _seg003_0272_5311:far
        public  _seg003_0272_0
        public  _seg003_0272_10
        public  _seg003_0272_18
        public  _seg003_0272_28
        public  _seg003_0272_46
        public  _seg003_0272_60
        public  _seg003_0272_7A
        public  _seg003_0272_7C
        public  _seg003_0272_7E
        public  _seg003_0272_8
        public  _seg003_0272_80
        public  _seg003_0272_82
        public  _seg003_0272_84
        public  _seg003_0272_86
        public  _seg003_0272_88
        public  _seg003_0272_8A
        public  _seg003_0272_8C
        public  _seg003_0272_8E
        public  _seg003_0272_90
        public  _seg003_0272_92
        public  _seg003_0272_94
        public  _seg003_0272_96
        public  _seg003_0272_98

_seg003_0272_0 proc near
        mov     bp,offset _seg003_0272_32B4
        db      0EAh                     ; jmp far ptr _seg003_0272_5311, which TASM would shorten
        dw      offset _seg003_0272_5311
        dw      seg _seg003_0272_5311
_seg003_0272_0 endp

; seg003_0272_8  (+8)
_seg003_0272_8 proc near
        mov     bp,offset _seg003_0272_375C
        db      0EAh                     ; jmp far ptr _seg003_0272_5311, which TASM would shorten
        dw      offset _seg003_0272_5311
        dw      seg _seg003_0272_5311
_seg003_0272_8 endp

; seg003_0272_10  (+10)
_seg003_0272_10 proc near
        mov     bp,offset _seg003_0272_368E
        db      0EAh                     ; jmp far ptr _seg003_0272_5311, which TASM would shorten
        dw      offset _seg003_0272_5311
        dw      seg _seg003_0272_5311
_seg003_0272_10 endp

; seg003_0272_18  (+18)
_seg003_0272_18 proc far
        mov     bp,offset _seg003_0272_46
        jmp     _seg003_0272_52FF
L001E:
        mov     bp,offset L004F
        jmp     _seg003_0272_52FF
        call    _seg003_0272_28
        retf
_seg003_0272_18 endp

; seg003_0272_28  (+28)
_seg003_0272_28 proc near
        mov     dx,3DAh
L002B:
        call    far ptr _seg021_22FD_C00
        neg     ax
        add     ax,word ptr ds:[652h]
        cmp     ax,4
        jge     L0045
        cmp     ax,0FFFCh
        jle     L0045
        in      al,dx
        test    al,8
        je      L002B
L0045:
        ret
_seg003_0272_28 endp

; seg003_0272_46  (+46)
_seg003_0272_46 proc near
        call    far ptr _seg021_22FD_C00
        mov     word ptr ds:[652h],ax
        ret
L004F:
        ret                             ; the null routine L001E hands the dispatcher
        retf
        db      0Fh dup (0)
_seg003_0272_60:
        int     2                       ; the debug break the dispatcher at _seg003_0272_52FF uses,
        db      'map bitmap error'      ; followed by its message
        dd      0
        dd      12345678h
_seg003_0272_7A   dw      0
_seg003_0272_7C   dw      0
_seg003_0272_7E   dw      0
_seg003_0272_80   dw      0
_seg003_0272_82   dw      0
_seg003_0272_84   dw      0
_seg003_0272_86   dw      0
_seg003_0272_88   dw      0
_seg003_0272_8A   dw      0
_seg003_0272_8C   dw      0
_seg003_0272_8E   dw      0
_seg003_0272_90   dw      0
_seg003_0272_92   dw      0
_seg003_0272_94   dw      0
_seg003_0272_96:
        dw      0                       ; the caller's es
_seg003_0272_98:
        db      54h dup (0)             ; a copy of the 54h bytes at DS:658
_seg003_0272_46 endp

SEG003_TEXT ends

        end
