; /* target: seg003_60 */
; /* opts: /ml */
;
; Graphics library module 2: an older wall texture mapper, the sibling of POLYFILL.ASM (FM Towns'
; wmap_bitmap with get_wright and get_wleft). UW2 does not have it: where it sits in UW1, UW2 has
; int 2 and 'map bitmap error' (UW2Decomp's GRMISC.ASM _60), a stub break, and nothing else.
; Its far entry is the unlabelled code at +1DA, after _121's ret; seg004 stores 01DAh or 0545h
; (POLYFILL's entry) as the mapper routine, so in UW1 both are used.
;
; How it works, as far as read (by its likeness to POLYFILL): the polygon's vertices are 14-byte
; records at 3963:0659; the texture comes from the record ES:[0B07Ch] points at (three words: a
; size less 1 kept at 7AD, after a skipped word the texture's segment, kept at 7AF, and a row
; mask patched into L02E3); the right and left edges are walked down the screen (_60 and _121),
; the x and two texture coordinates of each stepped in 16.16, and each row is copied texel by
; texel (movsb from the texture, its row masked by the AND at L02E3) into the 3D frame buffer
; (segment 95A, row offsets 95E; GRENTRY.ASM), with no column table and no perspective divide,
; which POLYFILL adds. The steps are patched into the instructions (the 1234h immediates at L02E3
; .. L0354; two are written 1234 decimal, 4D2h).
;
; Data (seg048): 7A9, 7AB the vertex list's last and end; 7B1 .. 7CD the two edges' current
; vertex, x and texture coordinates; 7CF the row; 7D1 the vertices left; 7D4 the segment DS is
; reloaded from after each row; 5586 the library's stack.
;
; name: descriptive (a wall mapper); no FM Towns counterpart, being older than the one it has.
;
; SEG003 split into its original modules: this is module 2 of UW1's 18, +60..+3AA.
; Evidence for the bounds: it starts after GRMISC's fifteen zero bytes (as UW2's _60 does); its
; two code-segment words end at +3AB (odd) and one zero byte follows before MAPDATA at +3AC,
; which TASM would not write (`even` pads with 90h): TLINK's padding before a word-aligned
; module. Its relocations (entries 2724-2725, +1DD and +375) come after seg004's and seg019's
; and before POLYFILL's far data entry (2726), so a module boundary lies between it and POLYFILL.

        .186

        extrn   seg048:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        public  _seg003_121
        public  _seg003_1DA
        public  _seg003_60

; seg003_60  (+60)
; _60 (get_wright in POLYFILL's terms): advance the right-hand edge to the next vertex (forwards
; through the list at 659, wrapping at 7AB): load its x and its two texture coordinates, and patch
; the per-row steps of all three (16.16, from divides by the edge's height) into the drawing
; loop. Out: ZF set when no vertices are left. L0093 is the re-entry used while drawing.
_seg003_60 proc near
        mov     bx,word ptr ds:[7B5h]
        mov     ax,word ptr [bx+0Ah]
        mov     ax,word ptr [bx+0Ch]
        mov     ax,word ptr [bx+4]
        mov     word ptr ds:[7C7h],ax
        mov     ax,word ptr [bx+6]
        mov     word ptr ds:[7C9h],ax
        mov     ax,word ptr [bx]
        mov     word ptr ds:[7C3h],ax
        mov     ax,8000h
        mov     word ptr ds:[7C5h],ax
        mov     word ptr ds:[7CBh],ax
        mov     word ptr ds:[7CDh],ax
        add     bx,0Eh
        cmp     bx,word ptr ds:[7ABh]
        jne     short L0093
        mov     bx,659h
L0093:
        mov     word ptr ds:[7B5h],bx
        dec     word ptr ds:[7D1h]
        jne     short L00A0
        jmp     L0120
L00A0:
        mov     ax,word ptr [bx+2]
        mov     word ptr cs:L0354,ax
        mov     cx,word ptr ds:[7CFh]
        sub     cx,ax
        je      _seg003_60
        mov     ax,word ptr [bx]
        sub     ax,word ptr ds:[7C3h]
        cwd
        idiv    cx
        mov     word ptr cs:L0346,ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr cs:L0340,ax
        add     word ptr cs:L0346,dx
        mov     ax,word ptr [bx+4]
        sub     ax,word ptr ds:[7C7h]
        cwd
        idiv    cx
        mov     word ptr cs:L032E,ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr cs:L0328,ax
        add     word ptr cs:L032E,dx
        mov     ax,word ptr [bx+6]
        sub     ax,word ptr ds:[7C9h]
        cwd
        idiv    cx
        mov     word ptr cs:L033A,ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr cs:L0334,ax
        add     word ptr cs:L033A,dx
        test    word ptr ds:[7D1h],0FFFFh
L0120:
        ret
_seg003_60 endp

; seg003_121  (+121)
; _121 (get_wleft): the same for the left-hand edge, backwards through the list. After its ret is
; the mapper's far entry (+1DA): save the caller's stack and switch to the library's, take the
; vertex count in CX and the texture record from ES:[0B07Ch], find the extremes, then draw row
; by row until both edges run out (each row stepping the two texture coordinates across it,
; the texture's row through the AND mask patched at L02E3 and the column in DH), set the colour
; from 3963:0658 when the pen-mode table is in use (as POLYFILL does in UW1, here with 0B0h for
; 0A0h), and return far on the caller's stack.
_seg003_121 proc near
        mov     bx,word ptr ds:[7B3h]
        mov     ax,word ptr [bx+4]
        mov     word ptr ds:[7BBh],ax
        mov     ax,word ptr [bx+6]
        mov     word ptr ds:[7BDh],ax
        mov     ax,word ptr [bx]
        mov     word ptr ds:[7B7h],ax
        mov     ax,8000h
        mov     word ptr ds:[7B9h],ax
        mov     word ptr ds:[7BFh],ax
        mov     word ptr ds:[7C1h],ax
        sub     bx,0Eh
        cmp     bx,64Bh
        jne     short L014F
        mov     bx,word ptr ds:[7A9h]
L014F:
        mov     word ptr ds:[7B3h],bx
        dec     word ptr ds:[7D1h]
        je      L0120
        mov     ax,word ptr [bx+2]
        mov     word ptr cs:L034E,ax
        mov     cx,word ptr ds:[7CFh]
        sub     cx,ax
        je      _seg003_121
        mov     ax,word ptr [bx]
        sub     ax,word ptr ds:[7B7h]
        cwd
        idiv    cx
        mov     word ptr cs:L0322,ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr cs:L031C,ax
        add     word ptr cs:L0322,dx
        mov     ax,word ptr [bx+4]
        sub     ax,word ptr ds:[7BBh]
        cwd
        idiv    cx
        mov     word ptr cs:L030A,ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr cs:L0304,ax
        add     word ptr cs:L030A,dx
        mov     ax,word ptr [bx+6]
        sub     ax,word ptr ds:[7BDh]
        cwd
        idiv    cx
        mov     word ptr cs:L0316,ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr cs:L0310,ax
        add     word ptr cs:L0316,dx
        test    word ptr ds:[7D1h],0FFFFh
        ret
; the mapper's far entry (+1DA), named because TMAPOPS stores its offset (the link needs the
; name to take this module from the library)
_seg003_1DA label far
        push    es
        push    ds
        mov     ax,seg seg048
        mov     ds,ax
        mov     bx,ss
        mov     word ptr cs:L03A7,bx
        mov     word ptr cs:L03A9,sp
        cli
        mov     ss,ax
        mov     sp,word ptr ds:[5586h]
        sti
        mov     ax,0Eh
        mov     ax,cx
        shl     ax,1
        shl     ax,1
        shl     ax,1
        sub     ax,cx
        shl     ax,1
        add     ax,659h
        mov     word ptr ds:[7ABh],ax
        sub     ax,0Eh
        mov     word ptr ds:[7A9h],ax
        inc     cx
        mov     word ptr ds:[7D1h],cx
        mov     si,word ptr es:[0B07Ch]
        lods    word ptr es:[si]
        dec     ax
        mov     word ptr ds:[7ADh],ax
        add     si,2
        lods    word ptr es:[si]
        mov     word ptr ds:[7AFh],ax
        lods    word ptr es:[si]
        mov     word ptr cs:L02E3,ax
        mov     bx,0D8F0h
        mov     bp,2710h
        mov     si,659h
        mov     di,si
        dec     cx
L023A:
        lodsw
        cmp     bx,ax
        jge     short L0241
        mov     bx,ax
L0241:
        cmp     bp,ax
        jle     short L0247
        mov     bp,ax
L0247:
        lodsw
        cmp     ax,word ptr [di+2]
        jle     short L0250
        lea     di,[si-4]
L0250:
        add     si,0Ah
        loop    L023A
        mov     word ptr ds:[7B1h],di
        mov     es,word ptr ds:[95Ah]
        mov     si,word ptr ds:[7B1h]
        mov     word ptr ds:[7B3h],si
        mov     word ptr ds:[7B5h],si
        lodsw
        lodsw
        mov     word ptr ds:[7CFh],ax
        call    _seg003_121
        jne     short L0276
        jmp     L0363
L0276:
        call    _seg003_60
        jne     short L027E
        jmp     L0363
L027E:
        mov     si,word ptr ds:[7B7h]
        mov     di,word ptr ds:[7CFh]
        shl     di,1
        mov     di,word ptr [di+95Eh]
        add     di,si
        mov     cx,word ptr ds:[7C3h]
        sub     cx,si
        je      short L02C8
        mov     ax,word ptr ds:[7C7h]
        sub     ax,word ptr ds:[7BBh]
        cwd
        idiv    cx
        mov     word ptr cs:L02EC,ax
        mov     ax,word ptr ds:[7C9h]
        sub     ax,word ptr ds:[7BDh]
        cwd
        idiv    cx
        mov     word ptr cs:L02F3,ax
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    cx
        cwd
        shl     ax,1
        rcl     dx,1
        mov     word ptr cs:L02F0,ax
        add     word ptr cs:L02F3,dx
L02C8:
        inc     cx
        jg      short L02CE
        jmp     L03A5
L02CE:
        mov     ax,word ptr ds:[7BDh]
        mov     dx,word ptr ds:[7BBh]
        xor     bx,bx
        mov     bp,word ptr ds:[7C1h]
        mov     ds,word ptr ds:[7AFh]
L02DF:
        mov     si,ax
L02E3 equ $+2
        and     si,1234h
        mov     bl,dh
        add     si,bx
        movsb
L02EC equ $+2
        add     dx,1234h
L02F0 equ $+2
        add     bp,1234h
L02F3 equ $+1
        adc     ax,1234h
        loop    L02DF
        mov     ds,word ptr ss:[7D4h]
        dec     word ptr ds:[7CFh]
L0304 equ $+4
        add     word ptr ds:[7BFh],1234h
L030A equ $+4
        adc     word ptr ds:[7BBh],1234h
L0310 equ $+4
        add     word ptr ds:[7C1h],1234h
L0316 equ $+4
        adc     word ptr ds:[7BDh],1234     ; 1234 decimal here, 1234h elsewhere; patched (L0322)
L031C equ $+4
        add     word ptr ds:[7B9h],1234h
L0322 equ $+4
        adc     word ptr ds:[7B7h],1234h
L0328 equ $+4
        add     word ptr ds:[7CBh],1234h
L032E equ $+4
        adc     word ptr ds:[7C7h],1234h
L0334 equ $+4
        add     word ptr ds:[7CDh],1234h
L033A equ $+4
        adc     word ptr ds:[7C9h],1234     ; patched (L0346)
L0340 equ $+4
        add     word ptr ds:[7C5h],1234h
L0346 equ $+4
        adc     word ptr ds:[7C3h],1234h
        mov     cx,word ptr ds:[7CFh]
L034E equ $+2
        cmp     cx,1234h
        jle     short L039C
L0352:
L0354 equ $+2
        cmp     cx,1234h
        jle     short L035B
        jmp     L027E
L035B:
        call    _seg003_60
        je      short L0363
        jmp     L027E
L0363:
        cli
        mov     ss,word ptr cs:L03A7
        mov     sp,word ptr cs:L03A9
        sti
        pop     ds
        pop     es
        push    es
        push    di
        push    ax
        mov     ax,seg seg048
        mov     es,ax
        mov     ax,0
        mov     cx,0FFh
        mov     di,8
        repe scasb
        jcxz    L0398
        mov     al,byte ptr es:[658h]
        test    al,0FFh
        jne     short L0394
        mov     al,0B0h
        mov     byte ptr es:[658h],al
L0394:
        mov     byte ptr es:[410Fh],al
L0398:
        pop     ax
        pop     di
        pop     es
        retf
L039C:
        call    _seg003_121
        mov     cx,word ptr ds:[7CFh]
        jne     L0352
L03A5:
        jmp     L0363
L03A7   dw      0                       ; the caller's ss
L03A9   dw      0                       ; and sp
_seg003_121 endp

SEG003_TEXT ends

        end
