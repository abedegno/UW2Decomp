; /* target: seg004_0849_51B0 */
; /* opts: /ml */
;
; A texture-mapping scanline routine (_51B0).
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

SEG004_TEXT segment para public use16 'CODE'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   lightabs:byte
        public  _seg004_0849_51B0

; seg004_0849_51B0  (+51B0)
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
