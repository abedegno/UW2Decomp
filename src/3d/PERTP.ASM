; /* target: seg004_0849_4B30 */
; /* opts: /ml */
;
; The unshaded ("tp") pair of the horizontal span drawers _asm_texture_map (TEXMAP.ASM) calls
; through DS:CF42: perspective-correct (per) and linear (lin). They are used when shadem
; (DS:2688) is 0, and draw texels as they are, without lightabs. Inputs, variables and texel
; addressing are those of PERTLP.ASM's shaded pair, without the shade.
;
; name: descriptive (module 9 of seg004, after its first routine); both routines have FM Towns
; names. They are plain labels, not procs, hence "Procs: none" below.
;
; SEG004 split into its original modules: this is module 9 of 14, +4B30..+4E10 of the segment
; (file DCF0h), a module of the second library (UWLIB.LIB).
; Procs: none (data).
; Evidence for the bounds: starts on a paragraph after 4 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; Its relocations are entries 2430-2431 of UW2.EXE's relocation table.

        .386

        extrn   seg_370D:far

_DATA   segment word public use16 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public use16 'CODE'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        public  _asm_texture_map_scanline_lin_tp
        public  _asm_texture_map_scanline_per_tp

; _asm_texture_map_scanline_per_tp: a perspective-correct unshaded span, one pair of divides (u
; * w / w, v * w / w) for every pixel. log2 width is written into the two `shl ax,2`
; instructions at L4C69+2 and L4CAA+2 (self-modifying code). Nothing is drawn if the right end
; is left of the left end or any u or v end is negative.
_asm_texture_map_scanline_per_tp:
        mov     eax,dword ptr ds:[0CF74h]
        mov     dword ptr ds:[0CCB8h],eax
        mov     eax,dword ptr ds:[0CF60h]
        mov     dword ptr ds:[0CCB0h],eax
        mov     eax,dword ptr ds:[0CF64h]
        mov     dword ptr ds:[0CCB4h],eax
        mov     eax,dword ptr ds:[0CF68h]
        sub     eax,dword ptr ds:[0CF5Ch]
        js      _seg004_0849_4CB9
        mov     ebx,dword ptr ds:[0CF68h]
        add     ebx,41h
        sar     ebx,10h
        mov     word ptr ds:[0CCC8h],bx
        mov     eax,dword ptr ds:[0CF5Ch]
        add     eax,41h
        sar     eax,10h
        mov     word ptr ds:[0CCC6h],ax
        mov     word ptr ds:[0CCC4h],ax
        mov     di,word ptr ds:[0CF5Ah]
        push    ds
        mov     ax,seg seg_370D
        mov     ds,ax
        shl     di,1
        mov     di,word ptr [di+95Ch]
        pop     ds
        add     di,word ptr ds:[0CCC4h]
        mov     eax,dword ptr ds:[0CF60h]
        or      eax,dword ptr ds:[0CF6Ch]
        or      eax,dword ptr ds:[0CF64h]
        or      eax,dword ptr ds:[0CF70h]
        js      _seg004_0849_4CB9
        sub     bx,word ptr ds:[0CCC4h]
        je      _seg004_0849_4C8E
        mov     eax,10000h
        shl     ebx,10h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    ebx
        mov     ebx,eax
        mov     eax,dword ptr ds:[0CF78h]
        sub     eax,dword ptr ds:[0CF74h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CCA8h],eax
        mov     eax,dword ptr ds:[0CF6Ch]
        xor     eax,dword ptr ds:[0CF60h]
        shr     eax,10h
        je      _seg004_0849_4C01
        mov     eax,dword ptr ds:[0CF6Ch]
        sub     eax,dword ptr ds:[0CF60h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
_seg004_0849_4C01:
        mov     dword ptr ds:[0CCA0h],eax
        mov     eax,dword ptr ds:[0CF70h]
        xor     eax,dword ptr ds:[0CF64h]
        shr     eax,10h
        je      L4C29
        mov     eax,dword ptr ds:[0CF70h]
        sub     eax,dword ptr ds:[0CF64h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
L4C29:
        mov     dword ptr ds:[0CCA4h],eax
        mov     cx,word ptr ds:[0CCC8h]
        sub     cx,word ptr ds:[0CCC4h]
        mov     ebx,dword ptr ds:[0CCB8h]
        mov     ebp,dword ptr ds:[0CCB0h]
        mov     word ptr ds:[0CCD0h],cx
        mov     ecx,dword ptr ds:[0CCB4h]
        mov     al,byte ptr ds:[0CCECh]
        mov     byte ptr cs:[L4C69+2],al
        mov     byte ptr cs:[L4CAA+2],al
L4C53:
        mov     eax,ebp
        cdq
        idiv    ebx
        and     ax,word ptr ds:[0CCE8h]
        mov     si,ax
        mov     eax,ecx
        cdq
        idiv    ebx
L4C69:
        shl     ax,2
        and     ax,word ptr ds:[0CCEAh]
        add     si,ax
        mov     al,byte ptr gs:[si]
        mov     byte ptr es:[di],al
        inc     di
        add     ebp,dword ptr ds:[0CCA0h]
        add     ecx,dword ptr ds:[0CCA4h]
        add     ebx,dword ptr ds:[0CCA8h]
        dec     word ptr ds:[0CCD0h]
        jne     L4C53
_seg004_0849_4C8E:
        mov     eax,dword ptr ds:[0CF6Ch]
        cdq
        idiv    dword ptr ds:[0CF78h]
        and     ax,word ptr ds:[0CCE8h]
        mov     si,ax
        mov     eax,dword ptr ds:[0CF70h]
        cdq
        idiv    dword ptr ds:[0CF78h]
L4CAA:
        shl     ax,2
        and     ax,word ptr ds:[0CCEAh]
        add     si,ax
        mov     al,byte ptr gs:[si]
        mov     byte ptr es:[di],al
_seg004_0849_4CB9:
        ret
; _asm_texture_map_scanline_lin_tp: a linear unshaded span, one pixel per loop, v kept
; multiplied by the width. A nonzero u or v step gets 14h (about 1/3300 of a texel) added; the
; shaded and vertical routines do not do that.
_asm_texture_map_scanline_lin_tp:
        mov     eax,dword ptr ds:[0CF60h]
        mov     dword ptr ds:[0CCB0h],eax
        mov     eax,dword ptr ds:[0CF64h]
        mov     dword ptr ds:[0CCB4h],eax
        mov     eax,dword ptr ds:[0CF68h]
        sub     eax,dword ptr ds:[0CF5Ch]
        js      L4E03
        mov     ebx,dword ptr ds:[0CF68h]
        add     ebx,41h
        sar     ebx,10h
        mov     word ptr ds:[0CCC8h],bx
        mov     eax,dword ptr ds:[0CF5Ch]
        add     eax,41h
        sar     eax,10h
        mov     word ptr ds:[0CCC6h],ax
        mov     word ptr ds:[0CCC4h],ax
        mov     di,word ptr ds:[0CF5Ah]
        push    ds
        mov     ax,seg seg_370D
        mov     ds,ax
        shl     di,1
        mov     di,word ptr [di+95Ch]
        pop     ds
        add     di,word ptr ds:[0CCC4h]
        mov     eax,dword ptr ds:[0CF60h]
        or      eax,dword ptr ds:[0CF6Ch]
        or      eax,dword ptr ds:[0CF64h]
        or      eax,dword ptr ds:[0CF70h]
        js      L4E03
        sub     bx,word ptr ds:[0CCC4h]
        je      L4DE7
        mov     eax,10000h
        shl     ebx,10h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    ebx
        mov     ebx,eax
        mov     eax,dword ptr ds:[0CF6Ch]
        xor     eax,dword ptr ds:[0CF60h]
        shr     eax,10h
        je      L4D70
        mov     eax,dword ptr ds:[0CF6Ch]
        sub     eax,dword ptr ds:[0CF60h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
        add     eax,14h
L4D70:
        mov     dword ptr ds:[0CCA0h],eax
        mov     eax,dword ptr ds:[0CF70h]
        xor     eax,dword ptr ds:[0CF64h]
        shr     eax,10h
        je      L4D9C
        mov     eax,dword ptr ds:[0CF70h]
        sub     eax,dword ptr ds:[0CF64h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
        add     eax,14h
L4D9C:
        mov     dword ptr ds:[0CCA4h],eax
        mov     cx,word ptr ds:[0CCC8h]
        sub     cx,word ptr ds:[0CCC4h]
        mov     ebx,dword ptr ds:[0CCA0h]
        mov     ebp,dword ptr ds:[0CCA4h]
        mov     edx,ebx
        push    cx
        mov     cx,word ptr ds:[0CCECh]
        shl     dword ptr ds:[0CCB4h],cl
        shl     ebp,cl
        pop     cx
L4DC3:
        mov     si,word ptr ds:[0CCB6h]
        and     si,word ptr ds:[0CCEAh]
        mov     bx,word ptr ds:[0CCB2h]
        and     bx,word ptr ds:[0CCE8h]
        add     dword ptr ds:[0CCB0h],edx
        mov     al,byte ptr gs:[bx+si]
        mov     byte ptr es:[di],al
        inc     di
        add     dword ptr ds:[0CCB4h],ebp
        dec     cx
        jne     L4DC3
L4DE7:
        mov     si,word ptr ds:[0CF72h]
        mov     cx,word ptr ds:[0CCECh]
        shl     si,cl
        and     si,word ptr ds:[0CCEAh]
        mov     bx,word ptr ds:[0CF6Eh]
        and     bx,word ptr ds:[0CCE8h]
        mov     al,byte ptr gs:[bx+si]
        mov     byte ptr es:[di],al
L4E03:
        ret

SEG004_TEXT ends

        end
