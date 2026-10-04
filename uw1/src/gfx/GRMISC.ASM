; /* target: seg003_0 */
; /* opts: /ml */
;
; Graphics library module 1: the far thunks the 3D model interpreter uses to plot points and
; draw lines, a frame-timing hook seg019's cRender calls and the vertical-retrace wait.
;
; UW2Decomp's src/gfx/GRMISC.ASM (UW2's seg003_0272_0), the same code at the same offsets;
; only its far targets move. UW1 against UW2: the module ends at +60 straight after the
; fifteen zero bytes. In UW2 int 2, 'map bitmap error', a dword 0 and the code-segment words
; POLYFILL writes follow (UW2Decomp keeps them in this module); in UW1 an older wall mapper
; stands there (module 2, WALLMAP.ASM), and the words are module 3 (MAPDATA.ASM), so UW1 has no
; 'map bitmap error'.
;
; How the library is entered (as in UW2). seg003 keeps its working data in its own far data
; segment (3963:0000 in UW1, UW2's seg_370D). Its drawing routines are near procedures that
; expect DS = ES = that segment and run on a private stack in it. There are three ways in:
; the C wrappers in GRCORE, GRDISP's dispatcher _5B0B reached through the thunks here
; (register arguments, used by the 3D code in seg004), and seg019's C3DENTRY far calls into
; GRENTRY (the frame buffer).
;
; Entry points:
; - _0, _8, _10: far thunks. Each loads BP with a near routine (_0: GRLIBF's _3B34, plot a
;   clipped point; _8: GRLIBH's _3FDC, an unclipped line; _10: GRLIBG's _3F0E, a clipped line)
;   and far-jumps to GRDISP's _5B0B, which switches to the library's segment and stack and
;   calls BP. The coordinates pass through in AX, BX (and CX, DX for a line's second end).
; - _18: far, called by seg019's cRender (C3DENTRY) after render_3d; it runs _46 through
;   GRDISP's _5AF9.
; - _28, _46: the frame-timing pair (see each).
;
; name: descriptive, UW2Decomp's (map/filenames.tsv there: "graphics library module 1:
; dispatcher thunks, retrace wait, debug break").
;
; SEG003 split into its original modules: this is module 1 of UW1's 18, +0..+60.
; Evidence for the bounds: it starts the segment. Its relocations (entries 2662-2666, +6..+49)
; are separated from the next ones in this segment (2724, +1DD) by seg004's and seg019's, so a
; module boundary lies between +4A and +1DD; +60 follows the fifteen zero bytes UW2 also has
; after the retf, and the older wall mapper starts there.
; Its three far jumps to _5B0B use EA, which TASM would shorten within a module.

; The VGA ports used here.
INPUT_STATUS    equ     3DAh  ; input status 1 (vertical retrace)

        .186

        extrn   _seg019_C00:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg003_3B34:near
        extrn   _seg003_3F0E:near
        extrn   _seg003_3FDC:near
        extrn   _seg003_5AF9:near
        extrn   _seg003_5B0B:far
        public  _seg003_0
        public  _seg003_10
        public  _seg003_18
        public  _seg003_28
        public  _seg003_46
        public  _seg003_8

; seg003_0  (+0)
; _0: plot the point (AX,BX) clipped to the window (GRLIBF's _3B34), through the dispatcher.
_seg003_0 proc near
        mov     bp,offset _seg003_3B34
        db      0EAh                     ; match: jmp far ptr _seg003_5B0B as bytes; TASM would shorten it within the segment
        dw      offset _seg003_5B0B
        dw      seg _seg003_5B0B
_seg003_0 endp

; seg003_8  (+8)
; _8: draw a line from (AX,BX) to (CX,DX) without clipping (GRLIBH's _3FDC, FM Towns uline),
; through the dispatcher.
_seg003_8 proc near
        mov     bp,offset _seg003_3FDC
        db      0EAh                     ; match: jmp far ptr _seg003_5B0B as bytes; TASM would shorten it within the segment
        dw      offset _seg003_5B0B
        dw      seg _seg003_5B0B
_seg003_8 endp

; seg003_10  (+10)
; _10: draw a line from (AX,BX) to (CX,DX) clipped to the window (GRLIBG's _3F0E, FM Towns
; cline), through the dispatcher.
_seg003_10 proc near
        mov     bp,offset _seg003_3F0E
        db      0EAh                     ; match: jmp far ptr _seg003_5B0B as bytes; TASM would shorten it within the segment
        dw      offset _seg003_5B0B
        dw      seg _seg003_5B0B
_seg003_10 endp

; seg003_18  (+18)
; _18 (far): runs _46 (stamp the tick count) through GRDISP's _5AF9, after each 3D frame, so
; DS:652 holds the time the last 3D frame finished drawing.
;
; L001E does the same with the null routine L004F; no reference to it was found. The far routine
; at +24 (call _28, retf) has no label; no reference to its address was found either.
_seg003_18 proc far
        mov     bp,offset _seg003_46
        jmp     _seg003_5AF9
L001E:
        mov     bp,offset L004F
        jmp     _seg003_5AF9
        call    _seg003_28
        retf
_seg003_18 endp

; seg003_28  (+28)
; _28: wait for the start of the next vertical retrace (input status port 3DAh, bit 3), but
; return at once when the tick counter (seg019's _C00, TICKREAD) is 4 or more ticks away from
; the value _46 stored at DS:652. In: DS = the library's segment. Out: AX, DX changed.
_seg003_28 proc near
        mov     dx,INPUT_STATUS
L002B:
        call    far ptr _seg019_C00
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
_seg003_28 endp

; seg003_46  (+46)
; _46: store the tick count (seg019's _C00) at DS:652. Run by _18 after each 3D frame.
; L004F: a near ret (the null routine L001E passes the dispatcher) and a far retf.
_seg003_46 proc near
        call    far ptr _seg019_C00
        mov     word ptr ds:[652h],ax
        ret
L004F:
        ret                             ; the null routine L001E hands the dispatcher
        retf
        db      0Fh dup (0)
_seg003_46 endp

SEG003_TEXT ends

        end
