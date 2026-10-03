; /* target: seg004_0849_6240 */
; /* opts: /ml */
;
; The vertical polygon texture mapper, for walls: _asm_texture_map_v fills a clipped, projected
; polygon column by column, left to right, and hands each column to the vertical span drawer in
; SCANLINE.ASM (_seg004_0849_51B0, probably FM Towns' _asm_texture_map_scanline_lin_tlpv). It is
; TEXMAP.ASM's _asm_texture_map turned on its side: the same edge walk with x and y exchanged
; (the edges run along sx, +0Ch of the vertex record, and carry sy), and the same variables
; where the two agree. The only caller is _asm_texture_map_1 (PROJPOLY.ASM), when the classifier
; there has set the vertical flag (CCEE), which it does for the wall opcodes.
;
; Down a column of an upright wall the depth does not change while the view is level, so each
; column is perspective-correct at its two ends (u * w and v * w divided by w there,
; seg004_0849_68B5) and drawn linearly between them; the edges always carry u * w, v * w and w =
; 128 / z. Every wall column is shaded: there is no unshaded variant.
;
; Arguments: as _asm_texture_map (TEXMAP.ASM); the descriptor's pixel offset must be 0 (else int 2).
; Variables beyond TEXMAP.ASM's (FD58, 16.16):
;   CCC4 _x, the column being drawn (shared with the horizontal span drawers)
;   D008, D00C  the least and greatest sx; D022, D024 the first and last column, clamped
;   D010, D014  the two edges' y at the current column; D01C, D01E the left-walking edge's
;               current and next vertex
;   CFE0 .. CFFC, D000, D004  the column's ends as the span drawer reads them (SCANLINE.ASM)
;
; name: descriptive (module 13 of seg004). _asm_texture_map_v has its FM Towns name; the helpers
; are unnamed there too.
;
; SEG004 split into its original modules: this is module 13 of 14, +6240..+6D20 of the segment
; (file F400h), a module of the second library (UWLIB.LIB).
; Procs: seg004_0849_6240, seg004_0849_6437, seg004_0849_6632, _asm_texture_map_v,
; seg004_0849_6866, seg004_0849_68B5, seg004_0849_6952.
; Evidence for the bounds: starts on a paragraph after 10 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; Its relocations are entries 2438 of UW2.EXE's relocation table.

        .386

        extrn   seg_370D:far

_DATA   segment word public use16 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public use16 'code'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg004_0849_51B0:near
        public  __asm_texture_map_v
        public  _seg004_0849_6240
        public  _seg004_0849_6437
        public  _seg004_0849_6632
        public  _seg004_0849_6866
        public  _seg004_0849_68B5
        public  _seg004_0849_6952

; seg004_0849_6240  (+6240)
; seg004_0849_6240: step the edge that walks the vertices backwards (index - 1, wrapping) to its
; next vertex: its starting y (D010), w, l, u * w and v * w, and its steps per column (CF84 ..
; CFA4), each difference divided by the edge's width in columns. A vertex at z <= 1/256 sets
; CF44 and the polygon is abandoned. In: BP = _asm_texture_map_v's frame.
_seg004_0849_6240 proc near
        mov     ax,word ptr ds:[0CF52h]
        dec     ax
        jns     short L624A
        add     ax,word ptr [bp+8]
        nop
L624A:
        imul    ax,20h
        mov     bx,ax
        add     ax,word ptr [bp+6]
        nop
        mov     word ptr ds:[0D01Ch],ax
        sub     bx,20h
        jns     short L6263
        mov     ax,word ptr [bp+8]
        imul    ax,20h
        add     bx,ax
L6263:
        add     bx,word ptr [bp+6]
        nop
        mov     word ptr ds:[0D01Eh],bx
        mov     bx,word ptr ds:[0D01Ch]
        mov     eax,dword ptr [bx+10h]
        mov     dword ptr ds:[0D010h],eax
        mov     eax,100h
        cmp     dword ptr [bx+8],eax
        ja      short L628B
        mov     word ptr ds:[0CF44h],1
        jmp     short L629F
L628B:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
L629F:
        mov     dword ptr ds:[0CF74h],eax
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
        mov     si,word ptr ds:[0D01Eh]
        mov     eax,dword ptr [si+0Ch]
        add     eax,41h
        sar     eax,10h
        mov     ecx,dword ptr [bx+0Ch]
        add     ecx,41h
        sar     ecx,10h
        sub     eax,ecx
        shl     eax,10h
        mov     dword ptr ds:[0D018h],eax
        jne     short L6315
        sub     eax,eax
        mov     dword ptr ds:[0CF84h],eax
        mov     dword ptr ds:[0CF88h],eax
        mov     dword ptr ds:[0CF8Ch],eax
        mov     dword ptr ds:[0CF9Ch],eax
        mov     dword ptr ds:[0CFA4h],eax
        jmp     L6436
L6315:
        mov     eax,dword ptr [si+1Ch]
        sub     eax,dword ptr [bx+1Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0D018h]
        mov     dword ptr ds:[0CFA4h],eax
        mov     eax,dword ptr [si+10h]
        sub     eax,dword ptr [bx+10h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0D018h]
        mov     dword ptr ds:[0CF84h],eax
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
        idiv    dword ptr ds:[0D018h]
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
        idiv    dword ptr ds:[0D018h]
        mov     dword ptr ds:[0CF8Ch],eax
        mov     eax,100h
        cmp     dword ptr [bx+8],eax
        ja      short L63E1
        mov     word ptr ds:[0CF44h],1
        jmp     short L63F5
L63E1:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
L63F5:
        mov     ecx,eax
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L640C
        mov     word ptr ds:[0CF44h],1
        jmp     short L6420
L640C:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L6420:
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0D018h]
        mov     dword ptr ds:[0CF9Ch],eax
L6436:
        ret
_seg004_0849_6240 endp

; seg004_0849_6437  (+6437)
; seg004_0849_6437: as seg004_0849_6240 for the edge that walks forwards (index + 1, wrapping),
; into D014 and CF68 .. CF80, CF90 .. CFA8.
_seg004_0849_6437 proc near
        mov     ax,word ptr ds:[0CF54h]
        inc     ax
        cmp     ax,word ptr [bp+8]
        nop
        jb      short L6445
        sub     ax,word ptr [bp+8]
        nop
L6445:
        mov     bx,ax
        imul    ax,20h
        add     ax,word ptr [bp+6]
        nop
        mov     word ptr ds:[0D01Ch],ax
        mov     ax,bx
        inc     ax
        cmp     ax,word ptr [bp+8]
        nop
        jb      short L645E
        sub     ax,word ptr [bp+8]
        nop
L645E:
        imul    ax,20h
        add     ax,word ptr [bp+6]
        nop
        mov     word ptr ds:[0D01Eh],ax
        mov     bx,word ptr ds:[0D01Ch]
        mov     eax,dword ptr [bx+10h]
        mov     dword ptr ds:[0D014h],eax
        mov     eax,100h
        cmp     dword ptr [bx+8],eax
        ja      short L6488
        mov     word ptr ds:[0CF44h],1
        jmp     short L649C
L6488:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
L649C:
        mov     dword ptr ds:[0CF78h],eax
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
        mov     si,word ptr ds:[0D01Eh]
        mov     eax,dword ptr [si+0Ch]
        add     eax,41h
        sar     eax,10h
        mov     ecx,dword ptr [bx+0Ch]
        add     ecx,41h
        sar     ecx,10h
        sub     eax,ecx
        shl     eax,10h
        mov     dword ptr ds:[0D018h],eax
        jne     short L6512
        sub     eax,eax
        mov     dword ptr ds:[0CF90h],eax
        mov     dword ptr ds:[0CF94h],eax
        mov     dword ptr ds:[0CF98h],eax
        mov     dword ptr ds:[0CFA0h],eax
        mov     dword ptr ds:[0CFA8h],eax
        jmp     L6631
L6512:
        mov     eax,dword ptr [si+1Ch]
        sub     eax,dword ptr [bx+1Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0D018h]
        mov     dword ptr ds:[0CFA8h],eax
        mov     eax,dword ptr [si+10h]
        sub     eax,dword ptr [bx+10h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0D018h]
        mov     dword ptr ds:[0CF90h],eax
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
        idiv    dword ptr ds:[0D018h]
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
        idiv    dword ptr ds:[0D018h]
        mov     dword ptr ds:[0CF98h],eax
        mov     eax,100h
        cmp     dword ptr [bx+8],eax
        ja      short L65DC
        mov     word ptr ds:[0CF44h],1
        jmp     short L65F0
L65DC:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [bx+8]
L65F0:
        mov     ecx,eax
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L6607
        mov     word ptr ds:[0CF44h],1
        jmp     short L661B
L6607:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L661B:
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0D018h]
        mov     dword ptr ds:[0CFA0h],eax
L6631:
        ret
_seg004_0849_6437 endp

; seg004_0849_6632  (+6632)
; seg004_0849_6632: compute_min_max_y's counterpart for columns: finds the least and greatest
; sx, starts both edges at the leftmost vertex (or at the two ends of a vertical left side), and
; clamps the first and last column to 0 .. 2 * scrw - 1 into D022 and D024.
_seg004_0849_6632 proc near
        mov     dword ptr ds:[0D008h],7FFFFFFFh
        mov     dword ptr ds:[0D00Ch],80000001h
        mov     bx,word ptr [bp+6]
        nop
        sub     dx,dx
L664A:
        mov     eax,dword ptr [bx+0Ch]
        cmp     eax,dword ptr ds:[0D008h]
        jge     short L665D
        mov     dword ptr ds:[0D008h],eax
        mov     word ptr ds:[0CF52h],dx
L665D:
        cmp     eax,dword ptr ds:[0D00Ch]
        jle     short L6668
        mov     dword ptr ds:[0D00Ch],eax
L6668:
        inc     dx
        add     bx,20h
        cmp     dx,word ptr [bp+8]
        nop
        jb      L664A
        mov     ax,word ptr ds:[0CF52h]
        mov     word ptr ds:[0CF54h],ax
        mov     bx,20h
        imul    bx,word ptr ds:[0CF52h]
        add     bx,word ptr [bp+6]
        nop
        mov     edx,dword ptr [bx+0Ch]
        add     edx,41h
        sar     edx,10h
        mov     bx,word ptr ds:[0CF52h]
        inc     bx
        cmp     bx,word ptr [bp+8]
        nop
        jb      short L669D
        sub     bx,bx
L669D:
        mov     cx,bx
        imul    bx,20h
        add     bx,word ptr [bp+6]
        nop
        mov     eax,dword ptr [bx+0Ch]
        add     eax,41h
        sar     eax,10h
        cmp     ax,dx
        jne     short L66BA
        mov     word ptr ds:[0CF54h],cx
L66BA:
        mov     bx,word ptr ds:[0CF52h]
        dec     bx
        jns     short L66C5
        add     bx,word ptr [bp+8]
        nop
L66C5:
        imul    bx,20h
        add     bx,word ptr [bp+6]
        nop
        mov     eax,dword ptr [bx+0Ch]
        add     eax,41h
        sar     eax,10h
        cmp     ax,dx
        jne     short L66EF
        mov     ax,word ptr ds:[0CF52h]
        mov     word ptr ds:[0CF54h],ax
        dec     word ptr ds:[0CF52h]
        jns     short L66EF
        mov     ax,word ptr [bp+8]
        add     word ptr ds:[0CF52h],ax
L66EF:
        mov     bx,word ptr ds:[2472h]
        shl     bx,1
        mov     eax,dword ptr ds:[0D008h]
        shr     eax,10h
        or      ax,ax
        jns     short L6703
        sub     ax,ax
L6703:
        cmp     ax,bx
        jl      short L670A
        lea     ax,[bx-1]
L670A:
        mov     word ptr ds:[0D022h],ax
        mov     eax,dword ptr ds:[0D00Ch]
        shr     eax,10h
        cmp     ax,bx
        jl      short L671C
        lea     ax,[bx-1]
L671C:
        mov     word ptr ds:[0D024h],ax
        ret
_seg004_0849_6632 endp

; seg004_0849_6720  (+6720)
; _asm_texture_map_v(descriptor, vertices, count, mapping, shaded): draw the polygon into the
; frame buffer (ES = seg_370D:0958, GS = the texture's segment) one column at a time from D022
; to D024, advancing either edge to its next vertex when the column passes it, drawing the
; column (seg004_0849_68B5) and stepping both edges. The last column is drawn after the loop.
; Keeps SI, DI, ES, GS, BP; clears CF44 on the way out.
__asm_texture_map_v proc near
        push    bp
        mov     bp,sp
        push    si
        push    di
        push    es
        mov     ax,seg seg_370D
        mov     es,ax
        mov     es,word ptr es:[958h]
        push    gs
        mov     bx,word ptr [bp+4]
        mov     ax,word ptr [bx+4]
        or      ax,ax
        je      L6741
        int     2
        nop
L6741:
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
        call    _seg004_0849_6632
        call    _seg004_0849_6952
        mov     ax,word ptr ds:[0D022h]
        mov     word ptr ds:[0CCC4h],ax
L6776:
        mov     si,word ptr ds:[0CF52h]
        dec     si
        jns     short L6780
        add     si,word ptr [bp+8]
L6780:
        imul    si,20h
        mov     bx,word ptr [bp+6]
        mov     eax,dword ptr [bx+si+0Ch]
        add     eax,41h
        sar     eax,10h
        cmp     ax,word ptr ds:[0CCC4h]
        jg      short L67A8
        call    _seg004_0849_6240
        dec     word ptr ds:[0CF52h]
        jns     short L67A8
        mov     ax,word ptr [bp+8]
        add     word ptr ds:[0CF52h],ax
L67A8:
        mov     bx,word ptr ds:[0CF54h]
        inc     bx
        cmp     bx,word ptr [bp+8]
        jb      short L67B4
        sub     bx,bx
L67B4:
        imul    bx,20h
        add     bx,word ptr [bp+6]
        mov     eax,dword ptr [bx+0Ch]
        add     eax,41h
        sar     eax,10h
        cmp     ax,word ptr ds:[0CCC4h]
        jg      short L67E1
        call    _seg004_0849_6437
        inc     word ptr ds:[0CF54h]
        mov     ax,word ptr ds:[0CF54h]
        cmp     ax,word ptr [bp+8]
        jb      short L67E1
        mov     word ptr ds:[0CF54h],0
L67E1:
        cmp     word ptr ds:[0CF44h],1
        je      L6859
        call    _seg004_0849_68B5
        mov     eax,dword ptr ds:[0CF84h]
        add     dword ptr ds:[0D010h],eax
        mov     eax,dword ptr ds:[0CF88h]
        add     dword ptr ds:[0CF60h],eax
        mov     eax,dword ptr ds:[0CF8Ch]
        add     dword ptr ds:[0CF64h],eax
        mov     eax,dword ptr ds:[0CF9Ch]
        add     dword ptr ds:[0CF74h],eax
        mov     eax,dword ptr ds:[0CFA4h]
        add     dword ptr ds:[0CF7Ch],eax
        mov     eax,dword ptr ds:[0CF90h]
        add     dword ptr ds:[0D014h],eax
        mov     eax,dword ptr ds:[0CF94h]
        add     dword ptr ds:[0CF6Ch],eax
        mov     eax,dword ptr ds:[0CF98h]
        add     dword ptr ds:[0CF70h],eax
        mov     eax,dword ptr ds:[0CFA0h]
        add     dword ptr ds:[0CF78h],eax
        mov     eax,dword ptr ds:[0CFA8h]
        add     dword ptr ds:[0CF80h],eax
        inc     word ptr ds:[0CCC4h]
        mov     ax,word ptr ds:[0CCC4h]
        cmp     ax,word ptr ds:[0D024h]
        jl      L6776
        call    _seg004_0849_68B5
L6859:
        mov     word ptr ds:[0CF44h],0
        pop     gs
        pop     es
        pop     di
        pop     si
        pop     bp
        ret
__asm_texture_map_v endp

; seg004_0849_6866  (+6866)
; seg004_0849_6866: clamp the column: y ends below 0 become 0, y ends at or past 2 * scrh become
; 2 * scrh - 1, and a column at or past 2 * scrw becomes the last one. Changes AX.
_seg004_0849_6866 proc near
        test    word ptr ds:[0CFE6h],0FFFFh
        jns     short L6877
        mov     dword ptr ds:[0CFE4h],0
L6877:
        test    word ptr ds:[0CFE2h],0FFFFh
        jns     short L6888
        mov     dword ptr ds:[0CFE0h],0
L6888:
        mov     ax,word ptr ds:[2470h]
        shl     ax,1
        cmp     word ptr ds:[0CFE6h],ax
        jb      short L689A
        mov     word ptr ds:[0CFE6h],ax
        dec     word ptr ds:[0CFE6h]
L689A:
        cmp     word ptr ds:[0CFE2h],ax
        jb      short L68A4
        dec     ax
        mov     word ptr ds:[0CFE2h],ax
L68A4:
        mov     ax,word ptr ds:[2472h]
        shl     ax,1
        cmp     word ptr ds:[0CCC4h],ax
        jae     short L68B0
        ret
L68B0:
        dec     ax
        mov     word ptr ds:[0CCC4h],ax
        ret
_seg004_0849_6866 endp

; seg004_0849_68B5  (+68B5)
; seg004_0849_68B5: draw the current column. Its ends are the two edges' y, widened by a pixel
; each way (one end's integer part less one, the other's plus one) and clamped
; (seg004_0849_6866); u, v at both ends are u * w / w and v * w / w; l and w are copied; then
; the span drawer _seg004_0849_51B0 (SCANLINE.ASM) is called. Keeps BP.
_seg004_0849_68B5 proc near
        mov     eax,dword ptr ds:[0D010h]
        mov     dword ptr ds:[0CFE4h],eax
        mov     eax,dword ptr ds:[0D014h]
        mov     dword ptr ds:[0CFE0h],eax
        dec     word ptr ds:[0CFE2h]
        inc     word ptr ds:[0CFE6h]
        call    _seg004_0849_6866
        mov     eax,dword ptr ds:[0CF6Ch]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF78h]
        mov     dword ptr ds:[0CFE8h],eax
        mov     eax,dword ptr ds:[0CF60h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF74h]
        mov     dword ptr ds:[0CFECh],eax
        mov     eax,dword ptr ds:[0CF70h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF78h]
        mov     dword ptr ds:[0CFF0h],eax
        mov     eax,dword ptr ds:[0CF64h]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CF74h]
        mov     dword ptr ds:[0CFF4h],eax
        mov     eax,dword ptr ds:[0CF80h]
        mov     dword ptr ds:[0CFF8h],eax
        mov     eax,dword ptr ds:[0CF7Ch]
        mov     dword ptr ds:[0CFFCh],eax
        mov     eax,dword ptr ds:[0CF78h]
        mov     dword ptr ds:[0D000h],eax
        mov     eax,dword ptr ds:[0CF74h]
        mov     dword ptr ds:[0D004h],eax
        push    bp
        call    _seg004_0849_51B0
        pop     bp
        ret
_seg004_0849_68B5 endp

; seg004_0849_6952  (+6952)
; seg004_0849_6952: set up both edges from the leftmost vertex at once (TEXMAP.ASM's
; seg004_0849_5D66 turned on its side; an edge of zero columns is taken as one column).
_seg004_0849_6952 proc near
        mov     ax,20h
        imul    word ptr ds:[0CF52h]
        add     ax,word ptr [bp+6]
        mov     word ptr ds:[0CFC4h],ax
        mov     ax,word ptr ds:[0CF52h]
        dec     ax
        jns     short L6968
        add     ax,word ptr [bp+8]
L6968:
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
        jb      short L6989
        sub     ax,ax
L6989:
        imul    ax,20h
        add     ax,word ptr [bp+6]
        mov     word ptr ds:[0CFCAh],ax
        mov     si,word ptr ds:[0CFC6h]
        mov     eax,dword ptr [si+0Ch]
        add     eax,41h
        sar     eax,10h
        mov     si,word ptr ds:[0CFC4h]
        mov     ecx,dword ptr [si+0Ch]
        add     ecx,41h
        sar     ecx,10h
        sub     ax,cx
        jne     short L69B7
        inc     ax
L69B7:
        shl     eax,10h
        mov     dword ptr ds:[0CFBCh],eax
        mov     si,word ptr ds:[0CFCAh]
        mov     eax,dword ptr [si+0Ch]
        add     eax,41h
        sar     eax,10h
        mov     si,word ptr ds:[0CFC8h]
        mov     ecx,dword ptr [si+0Ch]
        add     ecx,41h
        sar     ecx,10h
        sub     ax,cx
        jne     short L69E4
        inc     ax
L69E4:
        shl     eax,10h
        mov     dword ptr ds:[0CFC0h],eax
        mov     si,word ptr ds:[0CFC4h]
        mov     ecx,dword ptr [si+10h]
        mov     dword ptr ds:[0D010h],ecx
        mov     si,word ptr ds:[0CFC6h]
        mov     eax,dword ptr [si+10h]
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFBCh]
        mov     dword ptr ds:[0CF84h],eax
        mov     si,word ptr ds:[0CFC8h]
        mov     ecx,dword ptr [si+10h]
        mov     dword ptr ds:[0D014h],ecx
        mov     si,word ptr ds:[0CFCAh]
        mov     eax,dword ptr [si+10h]
        sub     eax,ecx
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr ds:[0CFC0h]
        mov     dword ptr ds:[0CF90h],eax
        mov     si,word ptr ds:[0CFC4h]
        mov     di,word ptr ds:[0CFC6h]
        mov     eax,dword ptr [si+1Ch]
        mov     dword ptr ds:[0CF7Ch],eax
        mov     eax,100h
        cmp     dword ptr [si+8],eax
        ja      short L6A66
        mov     word ptr ds:[0CF44h],1
        jmp     short L6A7A
L6A66:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L6A7A:
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
        ja      short L6AB5
        mov     word ptr ds:[0CF44h],1
        jmp     short L6AC9
L6AB5:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L6AC9:
        mov     ecx,eax
        mov     eax,100h
        cmp     dword ptr [di+8],eax
        ja      short L6AE0
        mov     word ptr ds:[0CF44h],1
        jmp     short L6AF4
L6AE0:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
L6AF4:
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
        ja      short L6BC7
        mov     word ptr ds:[0CF44h],1
        jmp     short L6BDB
L6BC7:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L6BDB:
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
        ja      short L6C1A
        mov     word ptr ds:[0CF44h],1
        jmp     short L6C2E
L6C1A:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [si+8]
L6C2E:
        mov     ecx,eax
        mov     eax,100h
        cmp     dword ptr [di+8],eax
        ja      short L6C45
        mov     word ptr ds:[0CF44h],1
        jmp     short L6C59
L6C45:
        mov     eax,800000h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    dword ptr [di+8]
L6C59:
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
        ret
        neg     ax
        je      short L6D14
        cwde
        ret
L6D14:
        mov     eax,10000h
        ret
_seg004_0849_6952 endp

SEG004_TEXT ends

        end
