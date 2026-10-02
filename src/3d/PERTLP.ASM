; /* target: seg004_0849_46F0 */
; /* opts: /ml */
;
; The texture-mapping scanline routine _asm_texture_map_scanline_per_tlp.
;
; SEG004 split into its original modules: this is module 8 of 14, +46F0..+4B30 of the segment
; (file D8B0h), a module of the second library (UWLIB.LIB).
; Procs: none (data).
; Evidence for the bounds: starts on a paragraph after 2 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; Its relocations are entries 2428-2429 of UW2.EXE's relocation table.

        .386

        extrn   seg_370D:far

_DATA   segment word public use16 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public use16 'CODE'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   lightabs:byte
        public  _asm_texture_map_scanline_lin_tlp
        public  _asm_texture_map_scanline_per_tlp

_asm_texture_map_scanline_per_tlp:
        mov     eax,dword ptr ds:[0CF74h]
        mov     dword ptr ds:[0CCB8h],eax
        mov     eax,dword ptr ds:[0CF60h]
        mov     dword ptr ds:[0CCB0h],eax
        mov     eax,dword ptr ds:[0CF64h]
        mov     dword ptr ds:[0CCB4h],eax
        mov     eax,dword ptr ds:[0CF7Ch]
        mov     dword ptr ds:[0CCBCh],eax
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
        js      _seg004_0849_4924
        sub     bx,word ptr ds:[0CCC4h]
        je      _seg004_0849_48ED
        js      _seg004_0849_4924
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
        je      _seg004_0849_47C0
        mov     eax,dword ptr ds:[0CF6Ch]
        sub     eax,dword ptr ds:[0CF60h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
_seg004_0849_47C0:
        mov     dword ptr ds:[0CCA0h],eax
        mov     eax,dword ptr ds:[0CF70h]
        xor     eax,dword ptr ds:[0CF64h]
        shr     eax,10h
        je      L47E8
        mov     eax,dword ptr ds:[0CF70h]
        sub     eax,dword ptr ds:[0CF64h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
L47E8:
        mov     dword ptr ds:[0CCA4h],eax
        mov     eax,dword ptr ds:[0CF80h]
        sub     eax,dword ptr ds:[0CF7Ch]
        imul    ebx
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CCACh],eax
        mov     cx,word ptr ds:[0CCC8h]
        sub     cx,word ptr ds:[0CCC4h]
        mov     word ptr ds:[0CCCCh],cx
        mov     esi,dword ptr ds:[0CCB8h]
        mov     ebp,dword ptr ds:[0CCB0h]
        mov     ecx,dword ptr ds:[0CCB4h]
        mov     eax,dword ptr ds:[0CCACh]
        shl     eax,8
        mov     dword ptr cs:[L48AC+5],eax
        mov     dword ptr cs:[L48CF+5],eax
        mov     eax,dword ptr ds:[0CCBCh]
        mov     dword ptr ds:[0CCC0h],eax
        mov     ax,word ptr ds:[0CF5Eh]
        xor     ax,word ptr ds:[0CF5Ah]
        shr     ax,1
        jb      short L484E
        add     dword ptr ds:[0CCC0h],8000h
        jmp     short L4857
L484E:
        add     dword ptr ds:[0CCBCh],8000h
L4857:
        shl     dword ptr ds:[0CCBCh],8
        shl     dword ptr ds:[0CCC0h],8
        mov     al,byte ptr ds:[0CCECh]
        mov     byte ptr cs:[L488F+2],al
        shl     dword ptr ds:[0CCA0h],1
        shl     dword ptr ds:[0CCA4h],1
        shl     dword ptr ds:[0CCA8h],1
L4879:
        mov     eax,ebp
        cdq
        idiv    esi
        mov     bx,ax
        mov     eax,ecx
        cdq
        idiv    esi
        and     bx,word ptr ds:[0CCE8h]
L488F:
        shl     ax,2
        and     ax,word ptr ds:[0CCEAh]
        add     bx,ax
        mov     word ptr ds:[0CCCEh],bx
        mov     bl,byte ptr gs:[bx]
        mov     bh,byte ptr ds:[0CCBFh]
        mov     al,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],al
        inc     di
L48AC:
        add     dword ptr ds:[0CCBCh],12345678h
        dec     word ptr ds:[0CCCCh]
        je      short _seg004_0849_48ED
        mov     bx,word ptr ds:[0CCCEh]
        mov     bl,byte ptr gs:[bx]
        mov     bh,byte ptr ds:[0CCC3h]
        mov     al,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],al
        inc     di
L48CF:
        add     dword ptr ds:[0CCC0h],12345678h
        add     ebp,dword ptr ds:[0CCA0h]
        add     ecx,dword ptr ds:[0CCA4h]
        add     esi,dword ptr ds:[0CCA8h]
        dec     word ptr ds:[0CCCCh]
        jne     L4879
_seg004_0849_48ED:
        mov     eax,dword ptr ds:[0CF6Ch]
        cdq
        idiv    dword ptr ds:[0CF78h]
        mov     si,ax
        mov     eax,dword ptr ds:[0CF70h]
        cdq
        idiv    dword ptr ds:[0CF78h]
        and     si,word ptr ds:[0CCE8h]
        mov     cx,word ptr ds:[0CCECh]
        shl     ax,cl
        and     ax,word ptr ds:[0CCEAh]
        add     si,ax
        mov     bl,byte ptr gs:[si]
        mov     bh,byte ptr ds:[0CF82h]
        mov     al,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],al
_seg004_0849_4924:
        ret
_asm_texture_map_scanline_lin_tlp:
        mov     eax,dword ptr ds:[0CF60h]
        mov     dword ptr ds:[0CCB0h],eax
        mov     eax,dword ptr ds:[0CF64h]
        mov     dword ptr ds:[0CCB4h],eax
        mov     eax,dword ptr ds:[0CF7Ch]
        mov     dword ptr ds:[0CCBCh],eax
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
        js      L4B2B
        sub     bx,word ptr ds:[0CCC4h]
        je      L4B05
        js      L4B05
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
        je      L49D6
        mov     eax,dword ptr ds:[0CF6Ch]
        sub     eax,dword ptr ds:[0CF60h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
L49D6:
        mov     dword ptr ds:[0CCA0h],eax
        mov     eax,dword ptr ds:[0CF70h]
        xor     eax,dword ptr ds:[0CF64h]
        shr     eax,10h
        je      L49FE
        mov     eax,dword ptr ds:[0CF70h]
        sub     eax,dword ptr ds:[0CF64h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
L49FE:
        mov     dword ptr ds:[0CCA4h],eax
        mov     eax,dword ptr ds:[0CF80h]
        sub     eax,dword ptr ds:[0CF7Ch]
        imul    ebx
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CCACh],eax
        mov     cx,word ptr ds:[0CCC8h]
        sub     cx,word ptr ds:[0CCC4h]
        mov     esi,dword ptr ds:[0CCA0h]
        mov     ebp,dword ptr ds:[0CCA4h]
        mov     edx,dword ptr ds:[0CCACh]
        push    cx
        mov     cx,word ptr ds:[0CCECh]
        shl     dword ptr ds:[0CCB4h],cl
        shl     ebp,cl
        pop     cx
        shl     dword ptr ds:[0CCBCh],8
        shl     edx,8
        mov     eax,dword ptr ds:[0CCBCh]
        mov     dword ptr ds:[0CCC0h],eax
        mov     ax,word ptr ds:[0CF5Eh]
        xor     ax,word ptr ds:[0CF5Ah]
        shr     ax,1
        jb      short L4A66
        add     dword ptr ds:[0CCC0h],800000h
        jmp     short L4A6F
L4A66:
        add     dword ptr ds:[0CCBCh],800000h
L4A6F:
        shr     cx,1
        pushf
        je      short L4AD7
L4A74:
        mov     ax,word ptr ds:[0CCB6h]
        mov     bx,word ptr ds:[0CCB2h]
        and     ax,word ptr ds:[0CCEAh]
        and     bx,word ptr ds:[0CCE8h]
        add     bx,ax
        mov     bl,byte ptr gs:[bx]
        mov     bh,byte ptr ds:[0CCBFh]
        mov     al,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],al
        inc     di
        add     dword ptr ds:[0CCB0h],esi
        add     dword ptr ds:[0CCB4h],ebp
        add     dword ptr ds:[0CCBCh],edx
        mov     ax,word ptr ds:[0CCB6h]
        mov     bx,word ptr ds:[0CCB2h]
        and     ax,word ptr ds:[0CCEAh]
        and     bx,word ptr ds:[0CCE8h]
        add     bx,ax
        mov     bl,byte ptr gs:[bx]
        mov     bh,byte ptr ds:[0CCC3h]
        mov     al,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],al
        inc     di
        add     dword ptr ds:[0CCB0h],esi
        add     dword ptr ds:[0CCB4h],ebp
        add     dword ptr ds:[0CCC0h],edx
        dec     cx
        jne     L4A74
L4AD7:
        popf
        jae     short L4B05
        mov     ax,word ptr ds:[0CCB6h]
        mov     bx,word ptr ds:[0CCB2h]
        and     ax,word ptr ds:[0CCEAh]
        and     bx,word ptr ds:[0CCE8h]
        add     bx,ax
        mov     bl,byte ptr gs:[bx]
        mov     bh,byte ptr ds:[0CCBFh]
        mov     al,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],al
        inc     di
        add     dword ptr ds:[0CCB0h],esi
        add     dword ptr ds:[0CCB4h],ebp
L4B05:
        mov     si,word ptr ds:[0CF6Eh]
        mov     ax,word ptr ds:[0CF72h]
        mov     cx,word ptr ds:[0CCECh]
        shl     ax,cl
        and     ax,word ptr ds:[0CCEAh]
        and     si,word ptr ds:[0CCE8h]
        add     si,ax
        mov     bl,byte ptr gs:[si]
        mov     bh,byte ptr ds:[0CF82h]
        mov     al,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],al
L4B2B:
        ret

SEG004_TEXT ends

        end
