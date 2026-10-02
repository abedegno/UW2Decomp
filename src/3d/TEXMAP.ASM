; /* target: seg004_0849_54C0 */
; /* opts: /ml */
;
; compute_min_max_y and _asm_texture_map.
;
; SEG004 split into its original modules: this is module 12 of 14, +54C0..+6240 of the segment
; (file E680h), a module of the second library (UWLIB.LIB).
; Procs: seg004_0849_54C0, seg004_0849_5712, compute_min_max_y, _asm_texture_map,
; seg004_0849_5CDF, seg004_0849_5D2E, seg004_0849_5D66.
; Evidence for the bounds: starts on a paragraph after 8 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; Its relocations are entries 2437 of UW2.EXE's relocation table.

        .386

        extrn   seg_370D:far

_DATA   segment word public use16 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public use16 'CODE'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _asm_texture_map_scanline_lin_tlp:near
        extrn   _asm_texture_map_scanline_lin_tp:near
        extrn   _asm_texture_map_scanline_per_tlp:near
        extrn   _asm_texture_map_scanline_per_tp:near
        public  __asm_texture_map
        public  _compute_min_max_y
        public  _seg004_0849_54C0
        public  _seg004_0849_5712
        public  _seg004_0849_5CDF
        public  _seg004_0849_5D2E
        public  _seg004_0849_5D66

; seg004_0849_54C0  (+54C0)
_seg004_0849_54C0 proc near
        mov     ax,word ptr ds:[0CF52h]
        dec     ax
        jns     short L54CA
        add     ax,word ptr [bp+8]
        nop
L54CA:
        imul    ax,20h
        mov     bx,ax
        add     ax,word ptr [bp+6]
        nop
        mov     word ptr ds:[0CF4Ch],ax
        sub     bx,20h
        jns     short L54E3
        mov     ax,word ptr [bp+8]
        imul    ax,20h
        add     bx,ax
L54E3:
        add     bx,word ptr [bp+6]
        nop
        mov     word ptr ds:[0CF4Eh],bx
        mov     bx,word ptr ds:[0CF4Ch]
        mov     eax,dword ptr [bx+0Ch]
        mov     dword ptr ds:[0CF5Ch],eax
        cmp     word ptr [bp+0Ah],1
        nop
        ja      short L5509
        mov     dword ptr ds:[0CF74h],10000h
        jmp     short L5535
L5509:
        mov     eax,100h
        cmp     dword ptr [bx+8],eax
        ja      short L551D
        mov     word ptr ds:[0CF44h],1
        jmp     short L5531
L551D:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
L5531:
        mov     dword ptr ds:[0CF74h],eax
L5535:
        mov     eax,dword ptr [bx+1Ch]
        mov     dword ptr ds:[0CF7Ch],eax
        mov     eax,dword ptr ds:[0CF74h]
        imul    dword ptr [bx+14h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF60h],eax
        mov     eax,dword ptr [bx+18h]
        imul    dword ptr ds:[0CF74h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF64h],eax
        mov     si,word ptr ds:[0CF4Eh]
        mov     eax,dword ptr [si+10h]
        add     eax,41h
        sar     eax,10h
        mov     ecx,dword ptr [bx+10h]
        add     ecx,41h
        sar     ecx,10h
        sub     eax,ecx
        shl     eax,10h
        mov     dword ptr ds:[0CF48h],eax
        jne     short L55A7
        sub     eax,eax
        mov     dword ptr ds:[0CF84h],eax
        mov     dword ptr ds:[0CF88h],eax
        mov     dword ptr ds:[0CF8Ch],eax
        mov     dword ptr ds:[0CF9Ch],eax
        mov     dword ptr ds:[0CFA4h],eax
        jmp     L5711
L55A7:
        mov     eax,dword ptr [si+1Ch]
        sub     eax,dword ptr [bx+1Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CFA4h],eax
        mov     eax,dword ptr [si+0Ch]
        sub     eax,dword ptr [bx+0Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF84h],eax
        cmp     word ptr [bp+0Ah],1
        nop
        ja      short L5626
        mov     eax,dword ptr [si+14h]
        sub     eax,dword ptr [bx+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF88h],eax
        mov     eax,dword ptr [si+18h]
        sub     eax,dword ptr [bx+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF8Ch],eax
        mov     dword ptr ds:[0CF9Ch],0
        jmp     L5711
L5626:
        mov     eax,dword ptr [bx+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
        mov     ecx,eax
        mov     eax,dword ptr [si+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF88h],eax
        mov     eax,dword ptr [bx+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
        mov     ecx,eax
        mov     eax,dword ptr [si+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF8Ch],eax
        mov     eax,100h
        cmp     dword ptr [bx+8],eax
        ja      short L56BC
        mov     word ptr ds:[0CF44h],1
        jmp     short L56D0
L56BC:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
L56D0:
        mov     ecx,eax
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L56E7
        mov     word ptr ds:[0CF44h],1
        jmp     short L56FB
L56E7:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L56FB:
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF9Ch],eax
L5711:
        ret
_seg004_0849_54C0 endp

; seg004_0849_5712  (+5712)
_seg004_0849_5712 proc near
        mov     ax,word ptr ds:[0CF54h]
        inc     ax
        cmp     ax,word ptr [bp+8]
        nop
        jb      short L5720
        sub     ax,word ptr [bp+8]
        nop
L5720:
        mov     bx,ax
        imul    ax,20h
        add     ax,word ptr [bp+6]
        nop
        mov     word ptr ds:[0CF4Ch],ax
        mov     ax,bx
        inc     ax
        cmp     ax,word ptr [bp+8]
        nop
        jb      short L5739
        sub     ax,word ptr [bp+8]
        nop
L5739:
        imul    ax,20h
        add     ax,word ptr [bp+6]
        nop
        mov     word ptr ds:[0CF4Eh],ax
        mov     bx,word ptr ds:[0CF4Ch]
        mov     eax,dword ptr [bx+0Ch]
        mov     dword ptr ds:[0CF68h],eax
        cmp     word ptr [bp+0Ah],1
        nop
        ja      short L5761
        mov     dword ptr ds:[0CF78h],10000h
        jmp     short L578D
L5761:
        mov     eax,100h
        cmp     dword ptr [bx+8],eax
        ja      short L5775
        mov     word ptr ds:[0CF44h],1
        jmp     short L5789
L5775:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
L5789:
        mov     dword ptr ds:[0CF78h],eax
L578D:
        mov     eax,dword ptr [bx+1Ch]
        mov     dword ptr ds:[0CF80h],eax
        mov     eax,dword ptr ds:[0CF78h]
        imul    dword ptr [bx+14h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF6Ch],eax
        mov     eax,dword ptr [bx+18h]
        imul    dword ptr ds:[0CF78h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF70h],eax
        mov     si,word ptr ds:[0CF4Eh]
        mov     eax,dword ptr [si+10h]
        add     eax,41h
        sar     eax,10h
        mov     ecx,dword ptr [bx+10h]
        add     ecx,41h
        sar     ecx,10h
        sub     eax,ecx
        shl     eax,10h
        mov     dword ptr ds:[0CF48h],eax
        jne     short L57FF
        sub     eax,eax
        mov     dword ptr ds:[0CF90h],eax
        mov     dword ptr ds:[0CF94h],eax
        mov     dword ptr ds:[0CF98h],eax
        mov     dword ptr ds:[0CFA0h],eax
        mov     dword ptr ds:[0CFA8h],eax
        jmp     L5965
L57FF:
        mov     eax,dword ptr [si+1Ch]
        sub     eax,dword ptr [bx+1Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CFA8h],eax
        mov     eax,dword ptr [si+0Ch]
        sub     eax,dword ptr [bx+0Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF90h],eax
        cmp     word ptr [bp+0Ah],1
        nop
        ja      short L587C
        mov     eax,dword ptr [si+14h]
        sub     eax,dword ptr [bx+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF94h],eax
        mov     eax,dword ptr [si+18h]
        sub     eax,dword ptr [bx+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF98h],eax
        sub     eax,eax
        mov     dword ptr ds:[0CFA0h],eax
        jmp     L5965
L587C:
        mov     eax,dword ptr [bx+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
        mov     ecx,eax
        mov     eax,dword ptr [si+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF94h],eax
        mov     eax,dword ptr [bx+18h]
        cdq
        shld    edx,eax,17h
        shl     eax,17h
        idiv    dword ptr [bx+8]
        mov     ecx,eax
        mov     eax,dword ptr [si+18h]
        cdq
        shld    edx,eax,17h
        shl     eax,17h
        idiv    dword ptr [si+8]
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CF98h],eax
        mov     eax,100h
        cmp     dword ptr [bx+8],eax
        ja      short L5910
        mov     word ptr ds:[0CF44h],1
        jmp     short L5924
L5910:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
L5924:
        mov     ecx,eax
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L593B
        mov     word ptr ds:[0CF44h],1
        jmp     short L594F
L593B:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L594F:
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF48h]
        mov     dword ptr ds:[0CFA0h],eax
L5965:
        ret
_seg004_0849_5712 endp

; seg004_0849_5966  (+5966)
_compute_min_max_y proc near
        mov     dword ptr ds:[0CFCCh],7FFFFFFFh
        mov     dword ptr ds:[0CFD0h],80000001h
        mov     bx,word ptr [bp+6]
        nop
        sub     dx,dx
L597E:
        mov     eax,dword ptr [bx+10h]
        cmp     eax,dword ptr ds:[0CFCCh]
        jge     short L5991
        mov     dword ptr ds:[0CFCCh],eax
        mov     word ptr ds:[0CF52h],dx
L5991:
        cmp     eax,dword ptr ds:[0CFD0h]
        jle     short L599C
        mov     dword ptr ds:[0CFD0h],eax
L599C:
        inc     dx
        add     bx,20h
        cmp     dx,word ptr [bp+8]
        nop
        jb      L597E
        mov     ax,word ptr ds:[0CF52h]
        mov     word ptr ds:[0CF54h],ax
        mov     bx,20h
        imul    bx,word ptr ds:[0CF52h]
        add     bx,word ptr [bp+6]
        nop
        mov     edx,dword ptr [bx+10h]
        add     edx,41h
        sar     edx,10h
        mov     bx,word ptr ds:[0CF52h]
        inc     bx
        cmp     bx,word ptr [bp+8]
        nop
        jb      short L59D1
        sub     bx,bx
L59D1:
        mov     cx,bx
        imul    bx,20h
        add     bx,word ptr [bp+6]
        nop
        mov     eax,dword ptr [bx+10h]
        add     eax,41h
        sar     eax,10h
        cmp     ax,dx
        jne     short L59EE
        mov     word ptr ds:[0CF54h],cx
L59EE:
        mov     bx,word ptr ds:[0CF52h]
        dec     bx
        jns     short L59F9
        add     bx,word ptr [bp+8]
        nop
L59F9:
        imul    bx,20h
        add     bx,word ptr [bp+6]
        nop
        mov     eax,dword ptr [bx+10h]
        add     eax,41h
        sar     eax,10h
        cmp     ax,dx
        jne     short L5A23
        mov     ax,word ptr ds:[0CF52h]
        mov     word ptr ds:[0CF54h],ax
        dec     word ptr ds:[0CF52h]
        jns     short L5A23
        mov     ax,word ptr [bp+8]
        add     word ptr ds:[0CF52h],ax
L5A23:
        mov     bx,word ptr ds:[2470h]
        shl     bx,1
        mov     eax,dword ptr ds:[0CFCCh]
        shr     eax,10h
        or      ax,ax
        jns     short L5A37
        sub     ax,ax
L5A37:
        cmp     ax,bx
        jb      short L5A3E
        lea     ax,[bx-1]
L5A3E:
        mov     word ptr ds:[0CF56h],ax
        mov     eax,dword ptr ds:[0CFD0h]
        shr     eax,10h
        cmp     ax,bx
        jb      short L5A50
        lea     ax,[bx-1]
L5A50:
        mov     word ptr ds:[0CF58h],ax
        ret
_compute_min_max_y endp

; seg004_0849_5A54  (+5A54)
__asm_texture_map proc near
        push    bp
        mov     bp,sp
        push    si
        push    di
        push    es
        mov     ax,seg seg_370D
        mov     es,ax
        mov     es,word ptr es:[958h]
        push    gs
        call    _seg004_0849_5D2E
        mov     bx,word ptr [bp+4]
        mov     ax,word ptr [bx+4]
        mov     word ptr ds:[0CF3Ch],ax
        mov     gs,word ptr [bx+6]
        mov     ax,word ptr [bx]
        mov     word ptr ds:[0CF3Eh],ax
        mov     word ptr ds:[0CF36h],ax
        mov     word ptr ds:[0CF34h],0
        mov     ax,word ptr [bx+2]
        mov     word ptr ds:[0CF40h],ax
        mov     word ptr ds:[0CF3Ah],ax
        mov     word ptr ds:[0CF38h],0
        mov     al,byte ptr [bx+8]
        mov     byte ptr ds:[0CF46h],al
        call    _compute_min_max_y
        call    _seg004_0849_5D66
        mov     ax,word ptr ds:[0CF56h]
        mov     word ptr ds:[0CF5Ah],ax
        mov     eax,dword ptr ds:[0CF60h]
        mov     dword ptr ds:[0CFACh],eax
        mov     eax,dword ptr ds:[0CF64h]
        mov     dword ptr ds:[0CFB4h],eax
        mov     eax,dword ptr ds:[0CF6Ch]
        mov     dword ptr ds:[0CFB0h],eax
        mov     eax,dword ptr ds:[0CF70h]
        mov     dword ptr ds:[0CFB8h],eax
L5AC4:
        mov     si,word ptr ds:[0CF52h]
        dec     si
        jns     short L5ACE
        add     si,word ptr [bp+8]
L5ACE:
        imul    si,20h
        mov     bx,word ptr [bp+6]
        mov     eax,dword ptr [bx+si+10h]
        add     eax,41h
        sar     eax,10h
        cmp     ax,word ptr ds:[0CF5Ah]
        jg      short L5B06
        call    _seg004_0849_54C0
        dec     word ptr ds:[0CF52h]
        jns     short L5AF6
        mov     ax,word ptr [bp+8]
        add     word ptr ds:[0CF52h],ax
L5AF6:
        mov     eax,dword ptr ds:[0CF60h]
        mov     dword ptr ds:[0CFACh],eax
        mov     eax,dword ptr ds:[0CF64h]
        mov     dword ptr ds:[0CFB4h],eax
L5B06:
        mov     bx,word ptr ds:[0CF54h]
        inc     bx
        cmp     bx,word ptr [bp+8]
        jb      short L5B12
        sub     bx,bx
L5B12:
        imul    bx,20h
        add     bx,word ptr [bp+6]
        mov     eax,dword ptr [bx+10h]
        add     eax,41h
        sar     eax,10h
        cmp     ax,word ptr ds:[0CF5Ah]
        jg      short L5B4F
        call    _seg004_0849_5712
        inc     word ptr ds:[0CF54h]
        mov     ax,word ptr ds:[0CF54h]
        cmp     ax,word ptr [bp+8]
        jb      short L5B3F
        mov     word ptr ds:[0CF54h],0
L5B3F:
        mov     eax,dword ptr ds:[0CF6Ch]
        mov     dword ptr ds:[0CFB0h],eax
        mov     eax,dword ptr ds:[0CF70h]
        mov     dword ptr ds:[0CFB8h],eax
L5B4F:
        mov     al,byte ptr ds:[0CF31h]
        mov     byte ptr ds:[0CF30h],al
        test    word ptr ds:[0CCF0h],0FFFFh
        je      short L5BBE
        mov     byte ptr ds:[0CF30h],5
        mov     eax,dword ptr ds:[0CFB0h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF78h]
        mov     dword ptr ds:[0CF6Ch],eax
        mov     eax,dword ptr ds:[0CFACh]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF74h]
        mov     dword ptr ds:[0CF60h],eax
        mov     eax,dword ptr ds:[0CFB8h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF78h]
        mov     dword ptr ds:[0CF70h],eax
        mov     eax,dword ptr ds:[0CFB4h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF74h]
        mov     dword ptr ds:[0CF64h],eax
L5BBE:
        cmp     word ptr ds:[0CF44h],1
        je      L5CD2
        call    _seg004_0849_5CDF
        push    bp
        call    word ptr ds:[0CF42h]
        pop     bp
        mov     eax,dword ptr ds:[0CF84h]
        add     dword ptr ds:[0CF5Ch],eax
        mov     eax,dword ptr ds:[0CF9Ch]
        add     dword ptr ds:[0CF74h],eax
        mov     eax,dword ptr ds:[0CFA4h]
        add     dword ptr ds:[0CF7Ch],eax
        mov     eax,dword ptr ds:[0CF90h]
        add     dword ptr ds:[0CF68h],eax
        mov     eax,dword ptr ds:[0CFA0h]
        add     dword ptr ds:[0CF78h],eax
        mov     eax,dword ptr ds:[0CFA8h]
        add     dword ptr ds:[0CF80h],eax
        test    word ptr ds:[0CCF0h],0FFFFh
        jne     short L5C34
        mov     eax,dword ptr ds:[0CF88h]
        add     dword ptr ds:[0CF60h],eax
        mov     eax,dword ptr ds:[0CF8Ch]
        add     dword ptr ds:[0CF64h],eax
        mov     eax,dword ptr ds:[0CF94h]
        add     dword ptr ds:[0CF6Ch],eax
        mov     eax,dword ptr ds:[0CF98h]
        add     dword ptr ds:[0CF70h],eax
        jmp     short L5C58
L5C34:
        mov     eax,dword ptr ds:[0CF88h]
        add     dword ptr ds:[0CFACh],eax
        mov     eax,dword ptr ds:[0CF8Ch]
        add     dword ptr ds:[0CFB4h],eax
        mov     eax,dword ptr ds:[0CF94h]
        add     dword ptr ds:[0CFB0h],eax
        mov     eax,dword ptr ds:[0CF98h]
        add     dword ptr ds:[0CFB8h],eax
L5C58:
        inc     word ptr ds:[0CF5Ah]
        mov     ax,word ptr ds:[0CF5Ah]
        cmp     ax,word ptr ds:[0CF58h]
        jl      L5AC4
        test    word ptr ds:[0CCF0h],0FFFFh
        je      short L5CCB
        mov     eax,dword ptr ds:[0CFB0h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF78h]
        mov     dword ptr ds:[0CF6Ch],eax
        mov     eax,dword ptr ds:[0CFACh]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF74h]
        mov     dword ptr ds:[0CF60h],eax
        mov     eax,dword ptr ds:[0CFB8h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF78h]
        mov     dword ptr ds:[0CF70h],eax
        mov     eax,dword ptr ds:[0CFB4h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF74h]
        mov     dword ptr ds:[0CF64h],eax
L5CCB:
        call    _seg004_0849_5CDF
        call    word ptr ds:[0CF42h]
L5CD2:
        mov     word ptr ds:[0CF44h],0
        pop     gs
        pop     es
        pop     di
        pop     si
        pop     bp
        ret
__asm_texture_map endp

; seg004_0849_5CDF  (+5CDF)
_seg004_0849_5CDF proc near
        test    word ptr ds:[0CF5Eh],0FFFFh
        jns     short L5CF0
        mov     dword ptr ds:[0CF5Ch],0
L5CF0:
        test    word ptr ds:[0CF6Ah],0FFFFh
        jns     short L5D01
        mov     dword ptr ds:[0CF68h],0
L5D01:
        mov     ax,word ptr ds:[2472h]
        shl     ax,1
        cmp     word ptr ds:[0CF5Eh],ax
        jb      short L5D13
        mov     word ptr ds:[0CF5Eh],ax
        dec     word ptr ds:[0CF5Eh]
L5D13:
        cmp     word ptr ds:[0CF6Ah],ax
        jb      short L5D1D
        dec     ax
        mov     word ptr ds:[0CF6Ah],ax
L5D1D:
        mov     ax,word ptr ds:[2470h]
        shl     ax,1
        cmp     word ptr ds:[0CF5Ah],ax
        jae     short L5D29
        ret
L5D29:
        dec     ax
        mov     word ptr ds:[0CF5Ah],ax
        ret
_seg004_0849_5CDF endp

; seg004_0849_5D2E  (+5D2E)
_seg004_0849_5D2E proc near
        test    word ptr ds:[0CCF0h],0FFFFh
        jne     L5D3E
        cmp     word ptr [bp+0Ah],1
        ja      short L5D51
L5D3E:
        test    word ptr [bp+0Ch],0FFFFh
        je      short L5D4A
        mov     ax,offset _asm_texture_map_scanline_lin_tlp
        jmp     short L5D4F
L5D4A:
        mov     ax,offset _asm_texture_map_scanline_lin_tp
        jmp     short L5D4F
L5D4F:
        jmp     short L5D62
L5D51:
        test    word ptr [bp+0Ch],0FFFFh
        je      short L5D5D
        mov     ax,offset _asm_texture_map_scanline_per_tlp
        jmp     short L5D62
L5D5D:
        mov     ax,offset _asm_texture_map_scanline_per_tp
        jmp     short L5D62
L5D62:
        mov     word ptr ds:[0CF42h],ax
        ret
_seg004_0849_5D2E endp

; seg004_0849_5D66  (+5D66)
_seg004_0849_5D66 proc near
        mov     ax,20h
        imul    word ptr ds:[0CF52h]
        add     ax,word ptr [bp+6]
        mov     word ptr ds:[0CFC4h],ax
        mov     ax,word ptr ds:[0CF52h]
        dec     ax
        jns     short L5D7C
        add     ax,word ptr [bp+8]
L5D7C:
        imul    ax,20h
        add     ax,word ptr [bp+6]
        mov     word ptr ds:[0CFC6h],ax
        mov     ax,20h
        imul    word ptr ds:[0CF54h]
        add     ax,word ptr [bp+6]
        mov     word ptr ds:[0CFC8h],ax
        mov     ax,word ptr ds:[0CF54h]
        inc     ax
        cmp     ax,word ptr [bp+8]
        jb      short L5D9D
        sub     ax,ax
L5D9D:
        imul    ax,20h
        add     ax,word ptr [bp+6]
        mov     word ptr ds:[0CFCAh],ax
        mov     si,word ptr ds:[0CFC6h]
        mov     eax,dword ptr [si+10h]
        add     eax,41h
        sar     eax,10h
        mov     si,word ptr ds:[0CFC4h]
        mov     ecx,dword ptr [si+10h]
        add     ecx,41h
        sar     ecx,10h
        sub     ax,cx
        jne     short L5DCB
        inc     ax
L5DCB:
        shl     eax,10h
        mov     dword ptr ds:[0CFBCh],eax
        mov     si,word ptr ds:[0CFCAh]
        mov     eax,dword ptr [si+10h]
        add     eax,41h
        sar     eax,10h
        mov     si,word ptr ds:[0CFC8h]
        mov     ecx,dword ptr [si+10h]
        add     ecx,41h
        sar     ecx,10h
        sub     ax,cx
        jne     short L5DF8
        inc     ax
L5DF8:
        shl     eax,10h
        mov     dword ptr ds:[0CFC0h],eax
        mov     si,word ptr ds:[0CFC4h]
        mov     ecx,dword ptr [si+0Ch]
        mov     dword ptr ds:[0CF5Ch],ecx
        mov     si,word ptr ds:[0CFC6h]
        mov     eax,dword ptr [si+0Ch]
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CF84h],eax
        mov     si,word ptr ds:[0CFC8h]
        mov     ecx,dword ptr [si+0Ch]
        mov     dword ptr ds:[0CF68h],ecx
        mov     si,word ptr ds:[0CFCAh]
        mov     eax,dword ptr [si+0Ch]
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CF90h],eax
        mov     si,word ptr ds:[0CFC4h]
        mov     di,word ptr ds:[0CFC6h]
        cmp     word ptr [bp+0Ah],1
        ja      L5F65
        mov     eax,dword ptr [si+1Ch]
        mov     dword ptr ds:[0CF7Ch],eax
        mov     dword ptr ds:[0CF74h],10000h
        mov     eax,dword ptr [si+14h]
        mov     dword ptr ds:[0CF60h],eax
        mov     eax,dword ptr [si+18h]
        mov     dword ptr ds:[0CF64h],eax
        sub     eax,eax
        mov     dword ptr ds:[0CF9Ch],eax
        mov     eax,dword ptr [di+14h]
        sub     eax,dword ptr [si+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CF88h],eax
        mov     eax,dword ptr [di+18h]
        sub     eax,dword ptr [si+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CF8Ch],eax
        mov     eax,dword ptr [di+1Ch]
        sub     eax,dword ptr [si+1Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CFA4h],eax
        mov     dword ptr ds:[0CF78h],10000h
        mov     bx,word ptr ds:[0CFC8h]
        mov     eax,dword ptr [bx+1Ch]
        mov     dword ptr ds:[0CF80h],eax
        mov     eax,dword ptr [bx+14h]
        mov     dword ptr ds:[0CF6Ch],eax
        mov     eax,dword ptr [bx+18h]
        mov     dword ptr ds:[0CF70h],eax
        sub     eax,eax
        mov     dword ptr ds:[0CFA0h],eax
        push    si
        mov     si,word ptr ds:[0CFCAh]
        mov     eax,dword ptr [si+14h]
        sub     eax,dword ptr [bx+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CF94h],eax
        mov     eax,dword ptr [si+18h]
        sub     eax,dword ptr [bx+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CF98h],eax
        mov     eax,dword ptr [si+1Ch]
        sub     eax,dword ptr [bx+1Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CFA8h],eax
        pop     si
        jmp     L6227
L5F65:
        mov     eax,dword ptr [si+1Ch]
        mov     dword ptr ds:[0CF7Ch],eax
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L5F81
        mov     word ptr ds:[0CF44h],1
        jmp     short L5F95
L5F81:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L5F95:
        mov     dword ptr ds:[0CF74h],eax
        imul    dword ptr [si+14h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF60h],eax
        mov     eax,dword ptr [si+18h]
        imul    dword ptr ds:[0CF74h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF64h],eax
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L5FD0
        mov     word ptr ds:[0CF44h],1
        jmp     short L5FE4
L5FD0:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L5FE4:
        mov     ecx,eax
        mov     eax,100h
        cmp     dword ptr [di+8],eax
        ja      short L5FFB
        mov     word ptr ds:[0CF44h],1
        jmp     short L600F
L5FFB:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
L600F:
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CF9Ch],eax
        mov     eax,dword ptr [di+1Ch]
        sub     eax,dword ptr [si+1Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CFA4h],eax
        mov     eax,dword ptr [si+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        mov     ecx,eax
        mov     eax,dword ptr [di+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CF88h],eax
        mov     eax,dword ptr [si+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        mov     ecx,eax
        mov     eax,dword ptr [di+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CF8Ch],eax
        mov     si,word ptr ds:[0CFC8h]
        mov     eax,dword ptr [si+1Ch]
        mov     dword ptr ds:[0CF80h],eax
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L60E2
        mov     word ptr ds:[0CF44h],1
        jmp     short L60F6
L60E2:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L60F6:
        mov     dword ptr ds:[0CF78h],eax
        imul    dword ptr [si+14h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF6Ch],eax
        mov     eax,dword ptr [si+18h]
        imul    dword ptr ds:[0CF78h]
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF70h],eax
        mov     di,word ptr ds:[0CFCAh]
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L6135
        mov     word ptr ds:[0CF44h],1
        jmp     short L6149
L6135:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L6149:
        mov     ecx,eax
        mov     eax,100h
        cmp     dword ptr [di+8],eax
        ja      short L6160
        mov     word ptr ds:[0CF44h],1
        jmp     short L6174
L6160:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
L6174:
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CFA0h],eax
        mov     eax,dword ptr [di+1Ch]
        sub     eax,dword ptr [si+1Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CFA8h],eax
        mov     eax,dword ptr [si+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        mov     ecx,eax
        mov     eax,dword ptr [di+14h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CF94h],eax
        mov     eax,dword ptr [si+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
        mov     ecx,eax
        mov     eax,dword ptr [di+18h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
        sub     eax,ecx
        shl     eax,7
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CF98h],eax
L6227:
        ret
        neg     ax
        je      short L622F
        cwde
        ret
L622F:
        mov     eax,10000h
        ret
_seg004_0849_5D66 endp

SEG004_TEXT ends

        end
