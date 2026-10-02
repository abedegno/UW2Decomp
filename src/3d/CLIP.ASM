; /* target: seg004_0849_40B0 */
; /* opts: /ml */
;
; Polygon clipping against the view frustum, for the texture mapper. _asm_clip_polygon takes a
; polygon in view space and clips it against four planes through the eye, x = -z, x = z, y = z
; and y = -z (FM Towns names the stages left, right, top and bottom), with one
; Sutherland-Hodgman pass per plane. The planes are at 45 degrees, which fits the projection in
; PROJPOLY.ASM: it scales by scrw and scrh, which compute_min_max_y and the span clamps treat as
; half the screen width and height. There is no near or far plane; a vertex the clipper would
; place at the eye (z = 0) rejects the whole polygon.
;
; The only caller is _asm_texture_map_1 (PROJPOLY.ASM), and only when accept_flag (DS:25D3, FM
; Towns name) is clear. Its initial value is 0 and no instruction in seg003, seg004 or seg021
; writes it (set_accept, in INSTANCE.ASM, is an empty routine in both builds), so every textured
; polygon goes through the clipper. The routines run with DS on the 3D data segment FD58
; (seg052_519C), as all of seg004 does.
;
; The vertex record (32 bytes, 16.16 fixed point; the same in PROJPOLY, TEXMAP and TEXMAPV):
;   +00 x, +04 y, +08 z   view space
;   +0C sx, +10 sy        screen position, written by _asm_project_polygon
;   +14 u, +18 v          texture coordinates
;   +1C l                 shade, 0 .. 15.0, written by _asm_project_polygon
; The clipper interpolates x, y, u and v and sets z on the plane; it leaves sx, sy and l alone,
; since projection comes after clipping.
;
; Data (FD58; names from FM Towns, which keeps the same order):
;   DS:C880 the stack pointer saved for the abort path
;   DS:C882 Clipped_through_eye_flag, cleared on entry and set by nothing in this module
;   DS:C888 _vertices, the first vertex of the stage's input
;   DS:C88A _vcount, the stage's output count
;   DS:C88C _inside, the previous vertex's state: 0FFh at the start, 1 inside, 0 outside
;   DS:C890 the input count
;   DS:C892 and DS:CA92 the two vertex buffers the stages alternate between, 200h bytes
;   (16 vertices) each
; The result is always left in C892.
;
; name: inferred. The routines have their FM Towns names; System Shock's 3D library clip.c holds
; g3_clip_polygon and its edge intersections, the same job in C.
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
; clip_vertex_x_neg_z: the point where the edge from A to B crosses the plane x = -z.
;   in:  SI = vertex A, BX = vertex B, DI = output vertex
;   out: [DI] = A + t (B - A) for x, y, u and v, with t = (-zA - xA) / ((xB - xA) + (zB - zA))
;        in 16.16; z = -x. CX is preserved.
;
; If the denominator is 0, or the new vertex lies at x = 0 (and so at the eye), it jumps to
; _asm_clip_polygon's abort path, which drops the stack back to _asm_clip_polygon's frame and
; returns CX = 0: the whole polygon is rejected.
;
; The push/rep movsb/pop sequence after each `jmp L46E5` in these four routines is never reached
; (it would copy A to the output unchanged).
;
; The other three are the same for the other planes: _y_neg_z (y = -z), _x_z (x = z), _y_z (y =
; z).
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
; _asm_clip_polygon_left: one Sutherland-Hodgman stage against x = -z, keeping the side where -x
; < z.
;   in:  SI = input vertices, CX = their count, DI = output buffer
;   out: CX = output count (0 if nothing is left); DI past the last output vertex
;
; For each vertex: inside after outside emits the crossing and then the vertex; inside after
; inside emits the vertex; outside after inside emits the crossing. The edge from the last
; vertex back to the first is done after the loop. The first vertex has no previous state
; (_inside = 0FFh), so its incoming edge is handled only by that closing step.
;
; _bottom (-y < z), _right (x < z) and _top (y < z) are the same with the other test and
; intersection routine.
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
; _asm_clip_polygon: clip a polygon against the four planes.
;   in:  SI = vertices (32-byte records), CX = count (at most 16, the buffers' size)
;   out: SI = DS:C892, the clipped polygon; CX = its vertex count, 0 if it is invisible or was
;        rejected
;
; Copies the input to C892, then runs left (C892 to CA92), right (back to C892), top and bottom,
; stopping as soon as a stage leaves nothing. It saves SP in C880 so the intersection routines
; can abandon the polygon from any depth (L46E5).
;
; The loop at L46A3 only advances SI, which is then reloaded: it does nothing.
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
