; /* target: seg003_0272_0 */
; /* opts: /ml */
;
; Graphics library module 1: the far thunks the 3D model interpreter uses to plot points and
; draw lines, a frame-timing hook seg021's cRender calls, the vertical-retrace wait, the 'map
; bitmap error' break and a block of code-segment variables POLYFILL's wall mapper writes.
;
; How the library is entered. seg003 keeps its working data in its own far data segment,
; seg_370D (segment table entry FD51, "the graphics data segment"; offsets below are from
; 370D:0000, and the symbol seg_370D itself is 370D:0008). Its drawing routines are near
; procedures that expect DS = ES = seg_370D and run on a private stack in that segment. There
; are three ways in: the C wrappers in GRCORE (C argument order, used by the game's C code),
; GRDISP's dispatcher _5311 reached through the thunks here (register arguments, used by the 3D
; code in seg004), and seg021's C3DENTRY far calls into GRENTRY (the frame buffer).
;
; Entry points:
; - _0, _8, _10: far thunks. Each loads BP with a near routine (_0: GRLIBF's _32B4, plot a
;   clipped point; _8: GRLIBH's _375C, an unclipped line; _10: GRLIBG's _368E, a clipped line) and
;   far-jumps to GRDISP's _5311, which switches to seg_370D and the private stack and calls BP.
;   The coordinates pass through in AX, BX (and CX, DX for a line's second end). Callers: seg004's
;   INTERP.ASM, do_pntres (_0) and do_lnres (_8 when both ends are inside the view, _10
;   otherwise).
; - _18: far, called by seg021's cRender (C3DENTRY) right after render_3d; it runs _46 through
;   GRDISP's _52FF.
; - _28, _46: the frame-timing pair (see each).
; - _60: a far address seg004's TMAPOPS stores as a routine pointer (FM Towns calls the variable
;   it initialises mapper_rtn): int 2 followed by the text 'map bitmap error', a trap for a bitmap
;   mapper called before one was installed.
;
; name: descriptive (map/filenames.tsv: "graphics library module 1: dispatcher thunks, retrace
; wait, debug break"). FM Towns has the same library as flat 32-bit code with its labels, which
; is where the names quoted in seg003's comments come from; it calls the routines directly, so
; it has no counterpart of these thunks.
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

; The VGA ports used here.
INPUT_STATUS    equ     3DAh  ; input status 1 (vertical retrace)

        .186

        extrn   _seg021_22FD_C00:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
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
        db      0EAh                     ; match: jmp far ptr _seg003_0272_5311 as bytes; TASM would shorten it within the segment
        dw      offset _seg003_0272_5311
        dw      seg _seg003_0272_5311
_seg003_0272_0 endp

; seg003_0272_8  (+8)
; _8: draw a line from (AX,BX) to (CX,DX) without clipping (GRLIBH's _375C, FM Towns uline),
; through the dispatcher. Used by do_lnres when both projected ends are inside the view.
_seg003_0272_8 proc near
        mov     bp,offset _seg003_0272_375C
        db      0EAh                     ; match: jmp far ptr _seg003_0272_5311 as bytes; TASM would shorten it within the segment
        dw      offset _seg003_0272_5311
        dw      seg _seg003_0272_5311
_seg003_0272_8 endp

; seg003_0272_10  (+10)
; _10: draw a line from (AX,BX) to (CX,DX) clipped to the window (GRLIBG's _368E, FM Towns
; cline), through the dispatcher. Used by do_lnres when an end may lie outside the view.
_seg003_0272_10 proc near
        mov     bp,offset _seg003_0272_368E
        db      0EAh                     ; match: jmp far ptr _seg003_0272_5311 as bytes; TASM would shorten it within the segment
        dw      offset _seg003_0272_5311
        dw      seg _seg003_0272_5311
_seg003_0272_10 endp

; seg003_0272_18  (+18)
; _18 (far): runs _46 (stamp the tick count) through GRDISP's _52FF. seg021's cRender calls it
; after render_3d, so DS:652 holds the time the last 3D frame finished drawing.
;
; L001E does the same with the null routine L004F; no reference to it was found. The far routine
; at +24 (call _28, retf) has no label, so nothing in the sources jumps to it directly; no
; reference to its address was found either.
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
; _28: wait for the start of the next vertical retrace (input status port 3DAh, bit 3), but
; return at once when the tick counter (seg021's _C00, TICKREAD) is 4 or more ticks away from
; the value _46 stored at DS:652. In: DS = seg_370D. Out: AX, DX changed. It keeps a frame that
; is already late from waiting a further retrace.
_seg003_0272_28 proc near
        mov     dx,INPUT_STATUS
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
; _46: store the tick count (seg021's _C00) at DS:652. Run by _18 after each 3D frame.
;
; The bytes after it, to the end of the module, are data in the code segment:
; - L004F: a near ret (the null routine L001E passes the dispatcher) and a far retf.
; - _60: int 2, then 'map bitmap error' and two dwords (0 and 12345678h): the default
;   bitmap-mapper routine. int 2 is the NMI vector, used here as a break into a debugger.
; - _7A .. _94: words POLYFILL's wall mapper (FM Towns wmap_bitmap) stores with cs: overrides as
;   it sets up a polygon (bounds, vertex count, vertex pointers, step values); nothing reads them.
;   With _96 (the caller's ES) and _98 (a copy of the 54h-byte vertex list at 370D:0658, six
;   14-byte points) they are probably a record of the last polygon for inspecting in a debugger
;   after the 'map bitmap error' break.
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
