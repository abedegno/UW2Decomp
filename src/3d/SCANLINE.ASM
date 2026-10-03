; /* target: seg004_0849_51B0 */
; /* opts: /ml */
;
; The vertical span drawer: one screen column of a wall, for _asm_texture_map_v (TEXMAPV.ASM,
; its only caller, through _seg004_0849_68B5). It is FM Towns'
; _asm_texture_map_scanline_lin_tlpv: linear, textured, lit, vertical.
;
; Inputs (FD58, 16.16; FM Towns names):
;   DS:CCC4 _x (an integer column)
;   DS:CFE0 _v_y0, CFE4 _v_y1   the column's top and bottom
;   DS:CFE8 _v_u0, CFEC _v_u1, CFF0 _v_v0, CFF4 _v_v1, CFF8 _v_l0, CFFC _v_l1
;   DS:2688 shadem; CCE8, CCEA, CCEC as for the horizontal spans; CEFC mk_lastwid, the frame
;   buffer's row stride
;   GS = the texture's segment, ES = the frame buffer's
; (_seg004_0849_68B5 also stores the two w values at D000 and D004; this routine does not read
; them.)
; Working variables: DS:CF00 _du, CF04 _dv, CF0C _dl; CF10 _u, CF14 _v, CF1C _l, CF20 _l2;
; CF24 _x0_my_floor, CF26 _x1_my_floor, CF5A _y; CF2C the texel column.
;
; The column is drawn from y0 down to y1 with the texel column fixed at u0 (u is the same all
; the way down a column of an upright wall; _du is worked out but not used), stepping v linearly
; (v kept multiplied by the width) and the shade. The same checks as the horizontal routines
; apply: nothing is drawn if any u or v end is negative or the column is empty, and the last
; pixel is computed exactly from v1 and l1. There are three loops, each two pixels per pass with
; dithered shades (see PERTLP.ASM): stepping shade; constant shade, when l0 and l1 have the same
; integer part (the two dither rows then sit in DL and DH); and no shading, when shadem is 0.
;
; name: descriptive (module 11 of seg004). The routine is seg004_0849_51B0 in symbols.tsv, with
; no name of its own; FM Towns' _asm_texture_map_scanline_lin_tlpv reads the same variables in
; the same order and is the routine FM Towns' _asm_texture_map_v calls, so that is probably its
; original name.
;
; SEG004 split into its original modules: this is module 11 of 14, +51B0..+54C0 of the segment
; (file E370h), a module of the second library (UWLIB.LIB).
; Procs: seg004_0849_51B0.
; Evidence for the bounds: starts on a paragraph after 14 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; Its relocations are entries 2436 of UW2.EXE's relocation table.

        .386

        extrn   seg_370D:far

_DATA   segment word public use16 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public use16 'code'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   lightabs:byte
        public  _seg004_0849_51B0

; seg004_0849_51B0  (+51B0)
; _asm_texture_map_scanline_lin_tlpv (probable name): draw one shaded vertical column. No
; register inputs (see the module header); changes EAX..EDX, ESI, EDI, EBP.
_seg004_0849_51B0 proc near
        mov     eax,dword ptr ds:[0CFE8h]
        mov     dword ptr ds:[0CF10h],eax
        mov     eax,dword ptr ds:[0CFF0h]
        mov     dword ptr ds:[0CF14h],eax
        mov     eax,dword ptr ds:[0CFF8h]
        mov     dword ptr ds:[0CF1Ch],eax
        mov     ebx,dword ptr ds:[0CFE4h]
        add     ebx,41h
        sar     ebx,10h
        mov     word ptr ds:[0CF26h],bx
        mov     eax,dword ptr ds:[0CFE0h]
        add     eax,41h
        sar     eax,10h
        mov     word ptr ds:[0CF24h],ax
        mov     word ptr ds:[0CF5Ah],ax
        mov     di,word ptr ds:[0CFE2h]
        push    ds
        mov     ax,seg seg_370D
        mov     ds,ax
        shl     di,1
        mov     di,word ptr [di+95Ch]
        pop     ds
        add     di,word ptr ds:[0CCC4h]
        mov     eax,dword ptr ds:[0CFE8h]
        or      eax,dword ptr ds:[0CFECh]
        or      eax,dword ptr ds:[0CFF0h]
        or      eax,dword ptr ds:[0CFF4h]
        js      L53B7
        mov     ax,word ptr ds:[0CF12h]
        and     ax,word ptr ds:[0CCE8h]
        mov     word ptr ds:[0CF2Ch],ax
        sub     bx,word ptr ds:[0CF5Ah]
        je      L5396
        js      L5396
        mov     eax,10000h
        shl     ebx,10h
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    ebx
        mov     ebx,eax
        mov     eax,dword ptr ds:[0CFECh]
        xor     eax,dword ptr ds:[0CFE8h]
        shr     eax,10h
        je      L526B
        mov     eax,dword ptr ds:[0CFECh]
        sub     eax,dword ptr ds:[0CFE8h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
L526B:
        mov     dword ptr ds:[0CF00h],eax
        mov     eax,dword ptr ds:[0CFF4h]
        xor     eax,dword ptr ds:[0CFF0h]
        shr     eax,10h
        je      L5293
        mov     eax,dword ptr ds:[0CFF4h]
        sub     eax,dword ptr ds:[0CFF0h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
L5293:
        mov     dword ptr ds:[0CF04h],eax
        mov     eax,dword ptr ds:[0CFFCh]
        sub     eax,dword ptr ds:[0CFF8h]
        imul    ebx
        shrd    eax,edx,10h
        cdq
        mov     dword ptr ds:[0CF0Ch],eax
        mov     cx,word ptr ds:[0CF26h]
        sub     cx,word ptr ds:[0CF5Ah]
        mov     esi,dword ptr ds:[0CF00h]
        mov     ebp,dword ptr ds:[0CF04h]
        mov     edx,dword ptr ds:[0CF0Ch]
        push    cx
        mov     cx,word ptr ds:[0CCECh]
        shl     dword ptr ds:[0CF14h],cl
        shl     ebp,cl
        pop     cx
        shl     dword ptr ds:[0CF1Ch],8
        shl     edx,8
        mov     eax,dword ptr ds:[0CF1Ch]
        mov     dword ptr ds:[0CF20h],eax
        mov     ax,word ptr ds:[0CFE2h]
        xor     ax,word ptr ds:[0CCC4h]
        shr     ax,1
        jb      L52FD
        add     dword ptr ds:[0CF20h],800000h
        jmp     short L5306
L52FD:
        add     dword ptr ds:[0CF1Ch],800000h
L5306:
        test    word ptr ds:[2688h],0FFFFh
        je      L544B
        mov     ax,word ptr ds:[0CFFAh]
        cmp     ax,word ptr ds:[0CFFEh]
        je      L53B8
        mov     ax,word ptr ds:[0CCEAh]
        shr     cx,1
        pushf
        je      short L5374
L5323:
        mov     bx,word ptr ds:[0CF16h]
        and     bx,ax
        add     bx,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[bx]
        mov     bh,byte ptr ds:[0CF1Fh]
        mov     bl,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],bl
        add     di,word ptr ds:[0CEFCh]
        add     dword ptr ds:[0CF14h],ebp
        add     dword ptr ds:[0CF1Ch],edx
        mov     bx,word ptr ds:[0CF16h]
        and     bx,ax
        add     bx,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[bx]
        mov     bh,byte ptr ds:[0CF23h]
        mov     bl,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],bl
        add     di,word ptr ds:[0CEFCh]
        add     dword ptr ds:[0CF14h],ebp
        add     dword ptr ds:[0CF20h],edx
        dec     cx
        jne     L5323
L5374:
        popf
        jae     L5396
        mov     bx,word ptr ds:[0CF16h]
        and     bx,ax
        add     bx,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[bx]
        mov     bh,byte ptr ds:[0CF1Fh]
        mov     bl,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],bl
        add     di,word ptr ds:[0CEFCh]
L5396:
        mov     si,word ptr ds:[0CFF6h]
        mov     cx,word ptr ds:[0CCECh]
        shl     si,cl
        and     si,word ptr ds:[0CCEAh]
        add     si,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[si]
        mov     bh,byte ptr ds:[0CFFEh]
        mov     al,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],al
L53B7:
        ret
L53B8:
        mov     si,word ptr ds:[0CEFCh]
        mov     dl,byte ptr ds:[0CF1Fh]
        mov     dh,byte ptr ds:[0CF23h]
        mov     ax,word ptr ds:[0CCEAh]
        shr     cx,1
        pushf
        je      short L540B
L53CC:
        mov     bx,word ptr ds:[0CF16h]
        and     bx,ax
        add     bx,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[bx]
        mov     bh,dl
        mov     bl,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],bl
        add     di,si
        add     dword ptr ds:[0CF14h],ebp
        mov     bx,word ptr ds:[0CF16h]
        and     bx,ax
        add     bx,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[bx]
        mov     bh,dh
        mov     bl,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],bl
        add     di,si
        add     dword ptr ds:[0CF14h],ebp
        dec     cx
        jne     L53CC
L540B:
        popf
        jae     L5429
        mov     bx,word ptr ds:[0CF16h]
        and     bx,ax
        add     bx,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[bx]
        mov     bh,dl
        mov     bl,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],bl
        add     di,si
L5429:
        mov     si,word ptr ds:[0CFF6h]
        mov     cx,word ptr ds:[0CCECh]
        shl     si,cl
        and     si,word ptr ds:[0CCEAh]
        add     si,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[si]
        mov     bh,byte ptr ds:[0CFFEh]
        mov     al,byte ptr cs:lightabs[bx]
        mov     byte ptr es:[di],al
        ret
L544B:
        mov     si,word ptr ds:[0CEFCh]
        mov     ax,word ptr ds:[0CCEAh]
        shr     cx,1
        pushf
        je      short L5488
L5457:
        mov     bx,word ptr ds:[0CF16h]
        and     bx,ax
        add     bx,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[bx]
        mov     byte ptr es:[di],bl
        add     di,si
        add     dword ptr ds:[0CF14h],ebp
        mov     bx,word ptr ds:[0CF16h]
        and     bx,ax
        add     bx,word ptr ds:[0CF2Ch]
        mov     bl,byte ptr gs:[bx]
        mov     byte ptr es:[di],bl
        add     di,si
        add     dword ptr ds:[0CF14h],ebp
        dec     cx
        jne     L5457
L5488:
        popf
        jae     L549F
        mov     bx,word ptr ds:[0CF16h]
        and     bx,ax
        add     bx,word ptr ds:[0CF2Ch]
        mov     al,byte ptr gs:[bx]
        mov     byte ptr es:[di],al
        add     di,si
L549F:
        mov     si,word ptr ds:[0CFF6h]
        mov     cx,word ptr ds:[0CCECh]
        shl     si,cl
        and     si,word ptr ds:[0CCEAh]
        add     si,word ptr ds:[0CF2Ch]
        mov     al,byte ptr gs:[si]
        mov     byte ptr es:[di],al
        ret
_seg004_0849_51B0 endp

SEG004_TEXT ends

        end
