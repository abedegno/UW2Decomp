; /* target: seg004_0849_4E10 */
; /* opts: /ml */
;
; _asm_project_polygon, _asm_texture_map_1, the divide overflow handler,
; asm_texture_map_from_draw_poly, compute_lighting_value and compute_square_root_32.
;
; SEG004 split into its original modules: this is module 10 of 14, +4E10..+51B0 of the segment
; (file DFD0h), a module of the second library (UWLIB.LIB).
; Procs: _asm_project_polygon, _asm_texture_map_1, divide_overflow_handler, seg004_0849_4F5C,
; seg004_0849_4F73, asm_texture_map_from_draw_poly, compute_lighting_value,
; compute_square_root_32.
; Evidence for the bounds: starts on a paragraph after 12 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; Its relocations are entries 2432-2435 of UW2.EXE's relocation table.

        .386

        extrn   dseg062_62a6:far
        extrn   seg_370D:far

_DATA   segment word public use16 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public use16 'CODE'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   __asm_clip_polygon:near
        extrn   __asm_texture_map:near
        extrn   __asm_texture_map_v:near
        public  __asm_project_polygon
        public  __asm_texture_map_1
        public  _asm_texture_map_from_draw_poly
        public  _compute_lighting_value
        public  _compute_square_root_32
        public  _divide_overflow_handler
        public  _seg004_0849_4F5C
        public  _seg004_0849_4F73

; seg004_0849_4E10  (+4E10)
__asm_project_polygon proc near
        push    cx
        push    si
        push    ebx
        push    ebp
        push    edi
        mov     ax,word ptr ds:[2476h]
        shl     eax,10h
        mov     dword ptr ds:[0CCE4h],eax
        movsx   ebp,word ptr ds:[2472h]
        movsx   edi,word ptr ds:[2470h]
        mov     bx,word ptr ds:[2474h]
        shl     ebx,10h
L4E37:
        mov     eax,dword ptr [si]
        imul    ebp
        shld    edx,eax,10h
        shl     eax,10h
        idiv    dword ptr [si+8]
        add     eax,ebx
        mov     dword ptr [si+0Ch],eax
        mov     eax,dword ptr [si+4]
        imul    edi
        shld    edx,eax,10h
        shl     eax,10h
        idiv    dword ptr [si+8]
        add     eax,dword ptr ds:[0CCE4h]
        jns     L4E71
        sub     eax,eax
L4E71:
        mov     dword ptr [si+10h],eax
        call    _compute_lighting_value
        mov     dword ptr [si+1Ch],eax
        add     si,20h
        loop    L4E37
        pop     edi
        pop     ebp
        pop     ebx
        pop     si
        pop     cx
        ret
__asm_project_polygon endp

; seg004_0849_4E8A  (+4E8A)
__asm_texture_map_1 proc near
        push    bp
        mov     bp,sp
        push    si
        push    di
        push    bx
        cmp     word ptr [bp+0Ah],2
        je      L4EA4
        cmp     word ptr [bp+0Ch],0
        jl      L4EF9
        jne     L4EF4
L4EA4:
        test    byte ptr ds:[25D3h],0FFh
        jne     short L4EBA
        mov     si,word ptr [bp+6]
        mov     cx,word ptr [bp+8]
        call    __asm_clip_polygon
        or      cx,cx
        je      L4EF4
L4EBA:
        test    byte ptr cs:_divide_overflow_has_occurred,0FFh
        jne     short L4EF1
        call    __asm_project_polygon
        test    byte ptr cs:_divide_overflow_has_occurred,0FFh
        jne     short L4EF1
        push    word ptr [bp+10h]
        push    word ptr [bp+0Eh]
        push    word ptr [bp+0Ah]
        push    cx
        push    si
        push    word ptr [bp+4]
        test    word ptr ds:[0CCEEh],0FFFFh
        je      L4EEB
        call    __asm_texture_map_v
        jmp     L4EEE
L4EEB:
        call    __asm_texture_map
L4EEE:
        add     sp,0Ch
L4EF1:
        jmp     L4EF4
L4EF4:
        pop     bx
        pop     di
        pop     si
        pop     bp
        ret
L4EF9:
        int     2
        nop
        jmp     L4EF4
_divide_overflow_has_occurred db 0     ; FM _divide_overflow_has_occurred
__asm_texture_map_1 endp

; seg004_0849_4EFF  (+4EFF)
_divide_overflow_handler proc far
        push    bp
        mov     bp,sp
        push    ax
        push    bx
        push    es
        mov     byte ptr cs:_divide_overflow_has_occurred,1
        mov     bx,word ptr [bp+2]
        mov     es,word ptr [bp+4]
        cmp     byte ptr es:[bx],66h
        jne     short L4F18
        inc     bx
L4F18:
        dec     bx
L4F19:
        inc     bx
        mov     al,byte ptr es:[bx]
        cmp     al,36h
        je      L4F19
        cmp     al,2Eh
        je      L4F19
        cmp     al,3Eh
        je      L4F19
        cmp     al,26h
        je      L4F19
        inc     bx
        mov     al,byte ptr es:[bx]
        cmp     al,3Eh
        je      short L4F49
        and     al,0F0h
        cmp     al,30h
        je      short L4F4B
        cmp     al,70h
        je      short L4F4A
        cmp     al,0B0h
        je      short L4F4A
        cmp     al,0F0h
        je      short L4F4B
        int     2
L4F49:
        inc     bx
L4F4A:
        inc     bx
L4F4B:
        inc     bx
        mov     word ptr [bp+2],bx
        pop     es
        pop     bx
        pop     ax
        mov     eax,30000000h
        pop     bp
        iret
old_divide_overflow_handler dw 0       ; FM old_divide_overflow_handler: the handler it replaced
_divide_overflow_handler endp

; seg004_0849_4F5C  (+4F5C)
_seg004_0849_4F5C proc near
        push    es
        mov     ax,seg dseg062_62a6
        mov     es,ax
        mov     ax,word ptr es:[5A5h]
        mov     word ptr cs:old_divide_overflow_handler,ax
        mov     word ptr es:[5A5h],offset _divide_overflow_handler
        pop     es
        ret
_seg004_0849_4F5C endp

; seg004_0849_4F73  (+4F73)
_seg004_0849_4F73 proc near
        push    es
        mov     ax,seg dseg062_62a6
        mov     es,ax
        mov     ax,word ptr cs:old_divide_overflow_handler
        mov     word ptr es:[5A5h],ax
        pop     es
        ret
L4F83   dw      0
_seg004_0849_4F73 endp

; seg004_0849_4F85  (+4F85)
_asm_texture_map_from_draw_poly proc near
        pusha
        call    _seg004_0849_4F5C
        mov     byte ptr cs:_divide_overflow_has_occurred,0
        push    es
        mov     ax,seg seg_370D
        mov     es,ax
        mov     ax,word ptr es:[0AEEh]
        add     ax,2
        mov     word ptr ds:[0CEFCh],ax
        pop     es
        cmp     cx,8
        jbe     short L4FAB
        int     2
        nop
        mov     cx,8
L4FAB:
        mov     word ptr cs:L4F83,cx
        mov     ax,word ptr [bx+4]
        mov     word ptr ds:[0CCF8h],ax
        mov     word ptr ds:[0CCF6h],0
        mov     ax,word ptr [bx]
        push    cx
        bsf     cx,ax
        mov     word ptr ds:[0CCF2h],ax
        mov     word ptr ds:[0CCECh],cx
        dec     ax
        mov     word ptr ds:[0CCE8h],ax
        mov     ax,word ptr [bx+2]
        inc     ax
        push    ax
        sub     dx,dx
        div     word ptr ds:[0CCF2h]
        mov     word ptr ds:[0CCF4h],ax
        pop     ax
        sub     ax,word ptr ds:[0CCF2h]
        mov     word ptr ds:[0CCEAh],ax
        pop     cx
        mov     ax,word ptr [bx+6]
        mov     word ptr ds:[0CCEAh],ax
        mov     di,cx
        dec     di
        imul    di,20h
        add     di,0CCFCh
        mov     dx,cx
L4FF6:
        sub     ax,ax
        mov     word ptr [di],ax
        mov     word ptr [di+4],ax
        mov     word ptr [di+8],ax
        mov     word ptr [di+1Ch],ax
        lodsw
        mov     word ptr [di+2],ax
        lodsw
        mov     word ptr [di+6],ax
        lodsw
        mov     word ptr [di+0Ah],ax
        add     si,2
        movzx   eax,word ptr [si]
        shl     eax,8
        mov     ax,8000h
        mov     dword ptr [di+14h],eax
        movzx   eax,word ptr [si+2]
        mov     cx,10h
        sub     cx,word ptr ds:[0CCECh]
        shl     eax,cl
        mov     ax,8000h
        mov     dword ptr [di+18h],eax
        add     si,4
        sub     di,20h
        dec     dx
        jne     L4FF6
        mov     si,0CCFCh
        mov     cx,word ptr cs:L4F83
        call    L5072
        mov     word ptr ds:[0CCEEh],bx
        mov     word ptr ds:[0CCF0h],bp
        push    1
        push    word ptr ds:[2688h]
        push    cx
        push    ax
        push    word ptr cs:L4F83
        push    0CCFCh
        push    0CCF2h
        call    __asm_texture_map_1
        add     sp,0Eh
        call    _seg004_0849_4F73
        popa
        ret
L5071:
        ret
L5072:
        jcxz    L5071
        mov     bx,4000h
        mov     dx,0C000h
        push    di
        mov     di,word ptr [si+6]
        mov     bp,1
L5081:
        mov     ax,word ptr [si+0Ah]
        cmp     ax,bx
        jg      short L508A
        mov     bx,ax
L508A:
        cmp     ax,dx
        jl      short L5090
        mov     dx,ax
L5090:
        cmp     di,word ptr [si+6]
        je      L5099
        sub     bp,bp
L5099:
        add     si,20h
        loop    L5081
        pop     di
        or      bp,bp
        je      L50AE
        mov     ax,2
        sub     cx,cx
        sub     bx,bx
        jmp     short L50DF
L50AE:
        or      bx,bx
        js      short L50C9
        inc     bx
        shl     bx,2
        shr     bx,1
        cmp     dx,bx
        jg      short L50C2
        sub     ax,ax
        sub     cx,cx
        jmp     short L50CE
L50C2:
        mov     ax,2
        sub     cx,cx
        jmp     short L50CE
L50C9:
        mov     ax,2
        sub     cx,cx
L50CE:
        sub     bx,bx
        mov     dx,word ptr ds:[0C800h]
        cmp     word ptr ds:[0C788h],dx
        jne     short L50DF
        inc     bx
        sub     ax,ax
        sub     cx,cx
L50DF:
        ret
_asm_texture_map_from_draw_poly endp

; seg004_0849_50E0  (+50E0)
_compute_lighting_value proc near
        push    ecx
        push    ebx
        push    esi
        push    es
        mov     ax,seg seg_370D
        mov     es,ax
        mov     eax,dword ptr [si]
        imul    eax
        mov     ecx,eax
        mov     ebx,edx
        mov     eax,dword ptr [si+4]
        imul    eax
        add     ecx,eax
        adc     ebx,edx
        mov     eax,dword ptr [si+8]
        imul    eax
        add     eax,ecx
        adc     edx,ebx
        call    _compute_square_root_32
        shr     eax,5
        movzx   ecx,word ptr es:[64Ch]
        mul     ecx
        shr     eax,6
        mov     cx,word ptr es:[650h]
        shl     ecx,10h
        add     eax,ecx
        jns     short L5138
        xor     eax,eax
L5138:
        mov     cx,word ptr es:[64Eh]
        shl     ecx,10h
        add     eax,ecx
        cmp     eax,0F0000h
        jbe     short L5152
        mov     eax,0F0000h
L5152:
        pop     es
        pop     esi
        pop     ebx
        pop     ecx
        ret
_compute_lighting_value endp

; seg004_0849_515A  (+515A)
_compute_square_root_32 proc near
        push    edx
        mov     esi,eax
        mov     ebx,edx
        mov     ecx,edx
        inc     ecx
        shl     ecx,0Ch
        cmp     ecx,edx
        ja      short L517E
        lea     ecx,ds:[edx+1]
        sub     eax,eax
        sub     eax,1
        jcxz    L519F
L517E:
        mov     eax,esi
        mov     edx,ebx
        div     ecx
        add     ecx,eax
        rcr     ecx,1
        sub     eax,ecx
        add     eax,40h
        cmp     eax,80h
        ja      L517E
        mov     eax,ecx
L519F:
        pop     edx
        ret
_compute_square_root_32 endp

SEG004_TEXT ends

        end
