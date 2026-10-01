; /* target: seg004_0849_40B0 */
; /* opts: /ml */
;
; Polygon clipping: the clip_vertex routines and _asm_clip_polygon with its four edges.
;
; SEG004 split into its original modules: this is module 7 of 14, +40B0..+46F0 of the segment
; (file D270h), a module of the second library (UWLIB.LIB).
; Procs: clip_vertex_x_neg_z, clip_vertex_y_neg_z, clip_vertex_x_z, clip_vertex_y_z,
; _asm_clip_polygon_left, _asm_clip_polygon_bottom, _asm_clip_polygon_right,
; _asm_clip_polygon_top, _asm_clip_polygon.
; Evidence for the bounds: starts on a paragraph after 14 zero bytes following a iret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros).
; It has no relocations.

        .386

_DATA   segment word public use16 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public use16 'CODE'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        public  __asm_clip_polygon
        public  __asm_clip_polygon_bottom
        public  __asm_clip_polygon_left
        public  __asm_clip_polygon_right
        public  __asm_clip_polygon_top
        public  _clip_vertex_x_neg_z
        public  _clip_vertex_x_z
        public  _clip_vertex_y_neg_z
        public  _clip_vertex_y_z

; seg004_0849_40B0  (+40B0)
_clip_vertex_x_neg_z proc near
        push    cx
        mov     ecx,dword ptr [bx]
        sub     ecx,dword ptr [si]
        add     ecx,dword ptr [bx+8]
        sub     ecx,dword ptr [si+8]
        je      L4152
        mov     eax,dword ptr [si+8]
        neg     eax
        sub     eax,dword ptr [si]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    ecx
        mov     ecx,eax
        mov     eax,dword ptr [bx]
        sub     eax,dword ptr [si]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si]
        mov     dword ptr [di],eax
        mov     eax,dword ptr [bx+4]
        sub     eax,dword ptr [si+4]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+4]
        mov     dword ptr [di+4],eax
        mov     eax,dword ptr [bx+14h]
        sub     eax,dword ptr [si+14h]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+14h]
        mov     dword ptr [di+14h],eax
        mov     eax,dword ptr [bx+18h]
        sub     eax,dword ptr [si+18h]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+18h]
        mov     dword ptr [di+18h],eax
        mov     eax,dword ptr [di]
        neg     eax
        mov     dword ptr [di+8],eax
        jne     short L4150
        jmp     L46E5
L4150:
        jmp     short L4164
L4152:
        jmp     L46E5
        push    cx
        push    es
        push    si
        push    di
        push    ds
        pop     es
        mov     cx,20h
        rep movsb
        pop     di
        pop     si
        pop     es
        pop     cx
L4164:
        pop     cx
        ret
_clip_vertex_x_neg_z endp

; seg004_0849_4166  (+4166)
_clip_vertex_y_neg_z proc near
        push    cx
        mov     ecx,dword ptr [bx+4]
        sub     ecx,dword ptr [si+4]
        add     ecx,dword ptr [bx+8]
        sub     ecx,dword ptr [si+8]
        je      L420C
        mov     eax,dword ptr [si+8]
        neg     eax
        sub     eax,dword ptr [si+4]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    ecx
        mov     ecx,eax
        mov     eax,dword ptr [bx]
        sub     eax,dword ptr [si]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si]
        mov     dword ptr [di],eax
        mov     eax,dword ptr [bx+4]
        sub     eax,dword ptr [si+4]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+4]
        mov     dword ptr [di+4],eax
        mov     eax,dword ptr [bx+14h]
        sub     eax,dword ptr [si+14h]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+14h]
        mov     dword ptr [di+14h],eax
        mov     eax,dword ptr [bx+18h]
        sub     eax,dword ptr [si+18h]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+18h]
        mov     dword ptr [di+18h],eax
        mov     eax,dword ptr [di+4]
        neg     eax
        mov     dword ptr [di+8],eax
        jne     short L420A
        jmp     L46E5
L420A:
        jmp     short L421E
L420C:
        jmp     L46E5
        push    cx
        push    es
        push    si
        push    di
        push    ds
        pop     es
        mov     cx,20h
        rep movsb
        pop     di
        pop     si
        pop     es
        pop     cx
L421E:
        pop     cx
        ret
_clip_vertex_y_neg_z endp

; seg004_0849_4220  (+4220)
_clip_vertex_x_z proc near
        push    cx
        mov     ecx,dword ptr [bx]
        sub     ecx,dword ptr [si]
        sub     ecx,dword ptr [bx+8]
        add     ecx,dword ptr [si+8]
        je      L42BF
        mov     eax,dword ptr [si+8]
        sub     eax,dword ptr [si]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    ecx
        mov     ecx,eax
        mov     eax,dword ptr [bx]
        sub     eax,dword ptr [si]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si]
        mov     dword ptr [di],eax
        mov     eax,dword ptr [bx+4]
        sub     eax,dword ptr [si+4]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+4]
        mov     dword ptr [di+4],eax
        mov     eax,dword ptr [bx+14h]
        sub     eax,dword ptr [si+14h]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+14h]
        mov     dword ptr [di+14h],eax
        mov     eax,dword ptr [bx+18h]
        sub     eax,dword ptr [si+18h]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+18h]
        mov     dword ptr [di+18h],eax
        mov     eax,dword ptr [di]
        or      eax,eax
        mov     dword ptr [di+8],eax
        jne     short L42BD
        jmp     L46E5
L42BD:
        jmp     short L42D1
L42BF:
        jmp     L46E5
        push    cx
        push    es
        push    si
        push    di
        push    ds
        pop     es
        mov     cx,20h
        rep movsb
        pop     di
        pop     si
        pop     es
        pop     cx
L42D1:
        pop     cx
        ret
_clip_vertex_x_z endp

; seg004_0849_42D3  (+42D3)
_clip_vertex_y_z proc near
        push    cx
        mov     ecx,dword ptr [bx+4]
        sub     ecx,dword ptr [si+4]
        sub     ecx,dword ptr [bx+8]
        add     ecx,dword ptr [si+8]
        je      L4376
        mov     eax,dword ptr [si+8]
        sub     eax,dword ptr [si+4]
        rol     eax,10h
        movsx   edx,ax
        sub     ax,ax
        idiv    ecx
        mov     ecx,eax
        mov     eax,dword ptr [bx]
        sub     eax,dword ptr [si]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si]
        mov     dword ptr [di],eax
        mov     eax,dword ptr [bx+4]
        sub     eax,dword ptr [si+4]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+4]
        mov     dword ptr [di+4],eax
        mov     eax,dword ptr [bx+14h]
        sub     eax,dword ptr [si+14h]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+14h]
        mov     dword ptr [di+14h],eax
        mov     eax,dword ptr [bx+18h]
        sub     eax,dword ptr [si+18h]
        imul    ecx
        shrd    eax,edx,10h
        cdq
        add     eax,dword ptr [si+18h]
        mov     dword ptr [di+18h],eax
        mov     eax,dword ptr [di+4]
        or      eax,eax
        mov     dword ptr [di+8],eax
        jne     short L4374
        jmp     L46E5
L4374:
        jmp     short L4388
L4376:
        jmp     L46E5
        push    cx
        push    es
        push    si
        push    di
        push    ds
        pop     es
        mov     cx,20h
        rep movsb
        pop     di
        pop     si
        pop     es
        pop     cx
L4388:
        pop     cx
        ret
_clip_vertex_y_z endp

; seg004_0849_438A  (+438A)
__asm_clip_polygon_left proc near
        mov     word ptr ds:[0C88Ah],0
        mov     word ptr ds:[0C888h],si
        mov     byte ptr ds:[0C88Ch],0FFh
L4399:
        mov     eax,dword ptr [si]
        neg     eax
        cmp     eax,dword ptr [si+8]
        jge     L43D9
        test    byte ptr ds:[0C88Ch],0FFh
        jne     short L43BB
        lea     bx,[si-20h]
        call    _clip_vertex_x_neg_z
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L43BB:
        mov     byte ptr ds:[0C88Ch],1
        push    cx
        push    es
        push    si
        push    di
        push    ds
        pop     es
        mov     cx,20h
        rep movsb
        pop     di
        pop     si
        pop     es
        pop     cx
        add     di,20h
        inc     word ptr ds:[0C88Ah]
        jmp     L43F6
L43D9:
        cmp     byte ptr ds:[0C88Ch],1
        jne     short L43F1
        lea     bx,[si-20h]
        push    si
        xchg    bx,si
        call    _clip_vertex_x_neg_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L43F1:
        mov     byte ptr ds:[0C88Ch],0
L43F6:
        add     si,20h
        loop    L4399
        test    byte ptr ds:[0C88Ch],0FFh
        jne     short L4428
        mov     bx,word ptr ds:[0C888h]
        mov     eax,dword ptr [bx]
        neg     eax
        cmp     eax,dword ptr [bx+8]
        jge     L4447
        push    si
        sub     si,20h
        xchg    bx,si
        call    _clip_vertex_x_neg_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
        jmp     L4447
L4428:
        mov     bx,word ptr ds:[0C888h]
        mov     eax,dword ptr [bx]
        neg     eax
        cmp     eax,dword ptr [bx+8]
        jl      short L4447
        push    si
        sub     si,20h
        call    _clip_vertex_x_neg_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L4447:
        mov     cx,word ptr ds:[0C88Ah]
        ret
__asm_clip_polygon_left endp

; seg004_0849_444C  (+444C)
__asm_clip_polygon_bottom proc near
        mov     word ptr ds:[0C88Ah],0
        mov     word ptr ds:[0C888h],si
        mov     byte ptr ds:[0C88Ch],0FFh
L445B:
        mov     eax,dword ptr [si+4]
        neg     eax
        cmp     eax,dword ptr [si+8]
        jge     L449C
        test    byte ptr ds:[0C88Ch],0FFh
        jne     short L447E
        lea     bx,[si-20h]
        call    _clip_vertex_y_neg_z
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L447E:
        mov     byte ptr ds:[0C88Ch],1
        push    cx
        push    es
        push    si
        push    di
        push    ds
        pop     es
        mov     cx,20h
        rep movsb
        pop     di
        pop     si
        pop     es
        pop     cx
        add     di,20h
        inc     word ptr ds:[0C88Ah]
        jmp     L44B9
L449C:
        cmp     byte ptr ds:[0C88Ch],1
        jne     short L44B4
        lea     bx,[si-20h]
        push    si
        xchg    bx,si
        call    _clip_vertex_y_neg_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L44B4:
        mov     byte ptr ds:[0C88Ch],0
L44B9:
        add     si,20h
        loop    L445B
        test    byte ptr ds:[0C88Ch],0FFh
        jne     short L44EC
        mov     bx,word ptr ds:[0C888h]
        mov     eax,dword ptr [bx+4]
        neg     eax
        cmp     eax,dword ptr [bx+8]
        jge     L450C
        push    si
        sub     si,20h
        xchg    bx,si
        call    _clip_vertex_y_neg_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
        jmp     L450C
L44EC:
        mov     bx,word ptr ds:[0C888h]
        mov     eax,dword ptr [bx+4]
        neg     eax
        cmp     eax,dword ptr [bx+8]
        jl      short L450C
        push    si
        sub     si,20h
        call    _clip_vertex_y_neg_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L450C:
        mov     cx,word ptr ds:[0C88Ah]
        ret
__asm_clip_polygon_bottom endp

; seg004_0849_4511  (+4511)
__asm_clip_polygon_right proc near
        mov     word ptr ds:[0C88Ah],0
        mov     word ptr ds:[0C888h],si
        mov     byte ptr ds:[0C88Ch],0FFh
L4520:
        mov     eax,dword ptr [si]
        cmp     eax,dword ptr [si+8]
        jge     L455D
        test    byte ptr ds:[0C88Ch],0FFh
        jne     short L453F
        lea     bx,[si-20h]
        call    _clip_vertex_x_z
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L453F:
        mov     byte ptr ds:[0C88Ch],1
        push    cx
        push    es
        push    si
        push    di
        push    ds
        pop     es
        mov     cx,20h
        rep movsb
        pop     di
        pop     si
        pop     es
        pop     cx
        add     di,20h
        inc     word ptr ds:[0C88Ah]
        jmp     L457A
L455D:
        cmp     byte ptr ds:[0C88Ch],1
        jne     short L4575
        lea     bx,[si-20h]
        push    si
        xchg    bx,si
        call    _clip_vertex_x_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L4575:
        mov     byte ptr ds:[0C88Ch],0
L457A:
        add     si,20h
        loop    L4520
        test    byte ptr ds:[0C88Ch],0FFh
        jne     short L45A9
        mov     bx,word ptr ds:[0C888h]
        mov     eax,dword ptr [bx]
        cmp     eax,dword ptr [bx+8]
        jge     L45C5
        push    si
        sub     si,20h
        xchg    bx,si
        call    _clip_vertex_x_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
        jmp     L45C5
L45A9:
        mov     bx,word ptr ds:[0C888h]
        mov     eax,dword ptr [bx]
        cmp     eax,dword ptr [bx+8]
        jl      short L45C5
        push    si
        sub     si,20h
        call    _clip_vertex_x_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L45C5:
        mov     cx,word ptr ds:[0C88Ah]
        ret
__asm_clip_polygon_right endp

; seg004_0849_45CA  (+45CA)
__asm_clip_polygon_top proc near
        mov     word ptr ds:[0C88Ah],0
        mov     word ptr ds:[0C888h],si
        mov     byte ptr ds:[0C88Ch],0FFh
L45D9:
        mov     eax,dword ptr [si+4]
        cmp     eax,dword ptr [si+8]
        jge     L4617
        test    byte ptr ds:[0C88Ch],0FFh
        jne     short L45F9
        lea     bx,[si-20h]
        call    _clip_vertex_y_z
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L45F9:
        mov     byte ptr ds:[0C88Ch],1
        push    cx
        push    es
        push    si
        push    di
        push    ds
        pop     es
        mov     cx,20h
        rep movsb
        pop     di
        pop     si
        pop     es
        pop     cx
        add     di,20h
        inc     word ptr ds:[0C88Ah]
        jmp     L4634
L4617:
        cmp     byte ptr ds:[0C88Ch],1
        jne     short L462F
        lea     bx,[si-20h]
        push    si
        xchg    bx,si
        call    _clip_vertex_y_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L462F:
        mov     byte ptr ds:[0C88Ch],0
L4634:
        add     si,20h
        loop    L45D9
        test    byte ptr ds:[0C88Ch],0FFh
        jne     short L4664
        mov     bx,word ptr ds:[0C888h]
        mov     eax,dword ptr [bx+4]
        cmp     eax,dword ptr [bx+8]
        jge     L4681
        push    si
        sub     si,20h
        xchg    bx,si
        call    _clip_vertex_y_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
        jmp     L4681
L4664:
        mov     bx,word ptr ds:[0C888h]
        mov     eax,dword ptr [bx+4]
        cmp     eax,dword ptr [bx+8]
        jl      short L4681
        push    si
        sub     si,20h
        call    _clip_vertex_y_z
        pop     si
        add     di,20h
        inc     word ptr ds:[0C88Ah]
L4681:
        mov     cx,word ptr ds:[0C88Ah]
        ret
__asm_clip_polygon_top endp

; seg004_0849_4686  (+4686)
__asm_clip_polygon proc near
        mov     word ptr ds:[0C890h],cx
        mov     word ptr ds:[0C888h],si
        push    di
        push    si
        push    es
        push    ds
        pop     es
        mov     di,0C892h
        imul    cx,20h
        rep movsb
        pop     es
        mov     cx,word ptr ds:[0C890h]
        mov     si,0C892h
L46A3:
        add     si,20h
        loop    L46A3
        mov     byte ptr ds:[0C882h],0
        mov     word ptr ds:[0C880h],sp
        mov     si,0C892h
        mov     di,0CA92h
        mov     cx,word ptr ds:[0C890h]
        call    __asm_clip_polygon_left
        jcxz    L46DF
        mov     si,0CA92h
        mov     di,0C892h
        call    __asm_clip_polygon_right
        jcxz    L46DF
        mov     si,0C892h
        mov     di,0CA92h
        call    __asm_clip_polygon_top
        jcxz    L46DF
        mov     si,0CA92h
        mov     di,0C892h
        call    __asm_clip_polygon_bottom
L46DF:
        pop     si
        pop     di
        mov     si,0C892h
        ret
L46E5:
        mov     sp,word ptr ds:[0C880h]
        pop     si
        pop     di
        sub     cx,cx
        ret
__asm_clip_polygon endp

SEG004_TEXT ends

        end
