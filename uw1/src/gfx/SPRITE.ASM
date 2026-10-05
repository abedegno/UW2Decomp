; /* target: seg000 */
; /* opts: /ml */
;
; Sprites: DOS segment seg000 (0000:0000-04EA), the first code in UW.EXE, before seg001's
; screen-rectangle routines. UW1's module is UW2's (seg000 there too) byte for byte at the
; same offsets; only the sprite data's place differs (see below). map/files.tsv's "C" for
; seg000 is wrong: this is the assembly module, as in UW2.
;
; A sprite is a picture the game redraws in place on the screen, over whatever is under
; it: PANELS.C uses them for the screen furniture around the 3D view (the vitality and
; mana flasks with their draining and bubbling, the compass, the eyes). Up to 64 sprites,
; each on one of four layers. The calls that change a sprite only record the change and
; queue it for redrawing (sp_changed); update_sprites then puts back what the queued
; sprites covered, top layer first, and draws them again, bottom layer first, saving
; what each will cover in its block of video memory (save_rect and restore_rect,
; VALLOC.ASM). A sprite overlapping a changed one on a higher layer is queued too, so the
; layers stay in order.
;
; name: descriptive (the sprite routines, FM Towns create_sprite_ .. update_sprites_).
; tools/align.py had it as library code; it is the game's own sprite module. FM Towns has
; the same routines in the same order, between SetPnt_ and save_rect_: create_sprite_,
; destroy_sprite_, change_sprite_, draw_sprite_, draw_mask_, erase_sprite_, move_sprite_,
; resize_sprite_, set_yoff_, an unnamed helper that they all call (FM set_yoff_+0x22,
; here sp_changed), and update_sprites_. Each does the same work on the same tables, so
; those are their names (the C entry points are in symbols.tsv).
;
; match: hand-written assembly with .186 (push imm, shl reg,imm): the C entries save ds,
; es, si and di, point DS and ES at the sprite data and end `mov sp,bp; pop bp` with no
; locals, which neither Turbo C nor TASM's ARG frames produce; the helper is a near
; procedure with repne scasw list searches; and the "something changed" flag is a word in
; the code segment (sp_dirty, at offset 0, written through cs:). Forward jumps carry no
; padding, so they were written `short` (or are out of short range).
;
; The data: a far segment of its own (the listing's seg054, paragraph 5477, 604h
; bytes from offset 8, which UW2 has at 5EFD:0000). Its layout is FM Towns': sp_inf_tab,
; 40h records of 10h bytes; sp_inf_end (FM also calls it sp_chg_tab), four 40h-byte lists of sprites to redraw,
; one per layer, each a count of bytes then sprite numbers; sp_map_tab (FM sp_chg_end),
; four lists of the sprites on screen; then two words the redraw loop keeps (FM keeps them
; elsewhere, unnamed).
; match: the bytes show labels there, not numbers: [bx+0] is assembled with a 16-bit zero
; displacement, which TASM only gives a relocatable label. The segment itself is still
; taken from your EXE by tools/extract.py (it holds no bytes of its own and sits among the
; far data segments, which the link declares together in XFAR), so this module names it
; through sp_inf_tab, defined there at the segment's start, and the other names are
; equates from it.
;
; A record in sp_inf_tab, as the code uses it: +0 flags (1 in use, 2 visible, 4 nothing
; saved to restore, 8 destroyed, 10h drawn transparent), +2 x (word), +4 y (byte), +5 width
; (word), +7 height (byte), +8 the byte set_yoff sets, +9 layer (0-3, which list), +0Bh
; picture, +0Dh the valloc block holding what it covers. FM Towns' records hold the same
; fields, with the flags in a byte at +3.
;
; Calls into seg001 use its register entries: seg001_5A (valloc's), save_rect and
; restore_rect (FM names, public in VALLOC.ASM), and vfree's at 004E:0112, which
; VALLOC.ASM has only as a local label (L0112), so it is reached as _vfree+0Dh.

        .186

        extrn   _mouse_hide:far
        extrn   _mouse_show:far
        extrn   _pic_to_screen:far
        extrn   _mask_to_screen:far
        extrn   _seg001_5A:far             ; valloc's register entry: AX size, BX count
        extrn   _vfree:far                      ; +0Dh is its register entry: AX the block
        extrn   save_rect:far
        extrn   restore_rect:far
        extrn   _Transparency:byte              ; 3963:0DC7 (UW2 370D:0DC5)
        extrn   sp_inf_tab:byte                 ; 5477:0008 (seg054), the start of the sprite data

sp_inf_end      equ     sp_inf_tab+400h
sp_map_tab      equ     sp_inf_tab+500h
sp_list         equ     sp_inf_tab+600h         ; the list update_sprites' helper is scanning
sp_left         equ     sp_inf_tab+602h         ; and how many entries it has left

DGROUP  group   _DATA
_DATA   segment byte public 'DATA'
_DATA   ends

; Class ASMCODE: seg000 to seg002 are a class of their own that ends in CODE (docs/LINKING.md,
; Segment classes).
SEG000_TEXT segment byte public 'ASMCODE'
        assume  cs:SEG000_TEXT, ds:nothing, es:nothing

        public  _create_sprite
        public  _destroy_sprite
        public  _change_sprite
        public  _draw_sprite
        public  _draw_mask
        public  _erase_sprite
        public  _move_sprite
        public  _resize_sprite
        public  _set_yoff
        public  _update_sprites

sp_dirty        dw      0               ; set when a sprite changes, cleared by update_sprites

; int create_sprite(int layer, int a, int b): the number of a free record, marked in use
; (and transparent if Transparency is set), with the block valloc(a, b) when layer is not
; 0; -1 if no record or no block is free
_create_sprite proc far
        push    bp
        mov     bp,sp
        push    ds
        push    es
        push    si
        push    di
        push    seg sp_inf_tab
        pop     ds
        push    seg sp_inf_tab
        pop     es
        assume  ds:seg sp_inf_tab
        mov     si,offset sp_inf_tab
L0014:  test    word ptr [si],1
        je      short L0028
        add     si,10h
        cmp     si,offset sp_inf_end
        jb      L0014
        mov     ax,0FFFFh
        jmp     short L006B
L0028:  mov     word ptr [si],5
        mov     ax,seg _Transparency
        mov     es,ax
        cmp     es:_Transparency,0
        je      short L003C
        or      word ptr [si],10h
L003C:  mov     ax,[bp+6]
        mov     [si+9],ax
        or      ax,ax
        je      short L005A
        mov     ax,[bp+8]
        mov     bx,[bp+0Ah]
        call    _seg001_5A
        or      ax,ax
        jne     short L005C
        mov     ax,0FFFFh
        jmp     short L006B
L005A:  xor     ax,ax
L005C:  mov     [si+0Dh],ax
        mov     byte ptr [si+8],0
        mov     ax,si
        sub     ax,offset sp_inf_tab
        shr     ax,4
L006B:  pop     di
        pop     si
        pop     es
        pop     ds
        mov     sp,bp
        pop     bp
        ret
_create_sprite endp

; destroy_sprite(int n): mark it destroyed and not visible
_destroy_sprite proc far
        push    bp
        mov     bp,sp
        push    ds
        push    es
        push    si
        push    di
        push    seg sp_inf_tab
        pop     ds
        push    seg sp_inf_tab
        pop     es
        mov     ax,[bp+6]
        cmp     ax,40h
        ja      short L009E
        mov     bx,ax
        shl     bx,4
        or      word ptr sp_inf_tab[bx],8
        and     word ptr sp_inf_tab[bx],0FFFDh
        call    sp_changed
        xor     ax,ax
L009E:  pop     di
        pop     si
        pop     es
        pop     ds
        mov     sp,bp
        pop     bp
        ret
_destroy_sprite endp

; change_sprite(int n, int x, int y, int w, int h)
_change_sprite proc far
        push    bp
        mov     bp,sp
        push    ds
        push    es
        push    si
        push    di
        push    seg sp_inf_tab
        pop     ds
        push    seg sp_inf_tab
        pop     es
        mov     ax,[bp+6]
        cmp     ax,40h
        ja      short L00E3
        mov     bx,ax
        shl     bx,4
        mov     dx,[bp+8]
        mov     word ptr sp_inf_tab[bx+2],dx
        mov     dx,[bp+0Ah]
        mov     byte ptr sp_inf_tab[bx+4],dl
        mov     dx,[bp+0Ch]
        mov     word ptr sp_inf_tab[bx+5],dx
        mov     dx,[bp+0Eh]
        mov     byte ptr sp_inf_tab[bx+7],dl
        call    sp_changed
        xor     ax,ax
L00E3:  pop     di
        pop     si
        pop     es
        pop     ds
        mov     sp,bp
        pop     bp
        ret
_change_sprite endp

; draw_sprite(int n, int pic)
_draw_sprite proc far
        push    bp
        mov     bp,sp
        push    ds
        push    es
        push    si
        push    di
        push    seg sp_inf_tab
        pop     ds
        push    seg sp_inf_tab
        pop     es
        mov     ax,[bp+6]
        cmp     ax,40h
        ja      short L0118
        mov     bx,ax
        shl     bx,4
        mov     dx,[bp+8]
        mov     word ptr sp_inf_tab[bx+0Bh],dx
        or      word ptr sp_inf_tab[bx],2
        call    sp_changed
        xor     ax,ax
L0118:  pop     di
        pop     si
        pop     es
        pop     ds
        mov     sp,bp
        pop     bp
        ret
_draw_sprite endp

; draw_mask(int n, int pic): draw_sprite, drawn transparent
_draw_mask proc far
        push    bp
        mov     bp,sp
        push    ds
        push    es
        push    si
        push    di
        push    seg sp_inf_tab
        pop     ds
        push    seg sp_inf_tab
        pop     es
        mov     ax,[bp+6]
        cmp     ax,40h
        ja      short L014D
        mov     bx,ax
        shl     bx,4
        mov     dx,[bp+8]
        mov     word ptr sp_inf_tab[bx+0Bh],dx
        or      word ptr sp_inf_tab[bx],12h
        call    sp_changed
        xor     ax,ax
L014D:  pop     di
        pop     si
        pop     es
        pop     ds
        mov     sp,bp
        pop     bp
        ret
_draw_mask endp

; erase_sprite(int n): hide it if it is visible
_erase_sprite proc far
        push    bp
        mov     bp,sp
        push    ds
        push    es
        push    si
        push    di
        push    seg sp_inf_tab
        pop     ds
        push    seg sp_inf_tab
        pop     es
        mov     ax,[bp+6]
        cmp     ax,40h
        ja      short L0183
        mov     bx,ax
        shl     bx,4
        test    word ptr sp_inf_tab[bx],2
        je      short L0181
        and     word ptr sp_inf_tab[bx],0FFFDh
        call    sp_changed
L0181:  xor     ax,ax
L0183:  pop     di
        pop     si
        pop     es
        pop     ds
        mov     sp,bp
        pop     bp
        ret
_erase_sprite endp

; move_sprite(int n, int x, int y)
_move_sprite proc far
        push    bp
        mov     bp,sp
        push    ds
        push    es
        push    si
        push    di
        push    seg sp_inf_tab
        pop     ds
        push    seg sp_inf_tab
        pop     es
        mov     ax,[bp+6]
        cmp     ax,40h
        ja      short L01BA
        mov     bx,ax
        shl     bx,4
        mov     dx,[bp+8]
        mov     word ptr sp_inf_tab[bx+2],dx
        mov     dx,[bp+0Ah]
        mov     byte ptr sp_inf_tab[bx+4],dl
        call    sp_changed
        xor     ax,ax
L01BA:  pop     di
        pop     si
        pop     es
        pop     ds
        mov     sp,bp
        pop     bp
        ret
_move_sprite endp

; resize_sprite(int n, int w, int h)
_resize_sprite proc far
        push    bp
        mov     bp,sp
        push    ds
        push    es
        push    si
        push    di
        push    seg sp_inf_tab
        pop     ds
        push    seg sp_inf_tab
        pop     es
        mov     ax,[bp+6]
        cmp     ax,40h
        ja      short L01F1
        mov     bx,ax
        shl     bx,4
        mov     dx,[bp+8]
        mov     word ptr sp_inf_tab[bx+5],dx
        mov     dx,[bp+0Ah]
        mov     byte ptr sp_inf_tab[bx+7],dl
        call    sp_changed
        xor     ax,ax
L01F1:  pop     di
        pop     si
        pop     es
        pop     ds
        mov     sp,bp
        pop     bp
        ret
_resize_sprite endp

; set_yoff(int n, int yoff)
_set_yoff proc far
        push    bp
        mov     bp,sp
        push    ds
        push    es
        push    si
        push    di
        push    seg sp_inf_tab
        pop     ds
        push    seg sp_inf_tab
        pop     es
        mov     ax,[bp+6]
        cmp     ax,40h
        ja      short L0221
        mov     bx,ax
        shl     bx,4
        mov     dx,[bp+8]
        mov     byte ptr sp_inf_tab[bx+8],dl
        call    sp_changed
        xor     ax,ax
L0221:  pop     di
        pop     si
        pop     es
        pop     ds
        mov     sp,bp
        pop     bp
        ret
_set_yoff endp

; sp_changed: AX the sprite. Sets sp_dirty, adds the sprite to its layer's redraw list,
; adds it to or removes it from its layer's on-screen list as it is visible or not, then
; adds every visible sprite of a higher layer that overlaps it to that layer's redraw list.
; DS and ES address the sprite data.
sp_changed proc near
        mov     cs:sp_dirty,1
        mov     si,ax
        shl     si,4
        mov     bx,word ptr sp_inf_tab[si+9]
        shl     bx,6
        mov     di,offset sp_inf_end
        add     di,bx
        mov     cx,word ptr sp_inf_end[bx]
        shr     cx,1
        add     di,2
        repne   scasw
        je      short L0255
        add     word ptr sp_inf_end[bx],2
        mov     [di],ax
L0255:  mov     bx,word ptr sp_inf_tab[si+9]
        shl     bx,6
        mov     di,offset sp_map_tab
        add     di,bx
        mov     cx,word ptr sp_map_tab[bx]
        shr     cx,1
        add     di,2
        repne   scasw
        je      short L0281
        mov     cx,word ptr sp_inf_tab[si]
        test    cx,2
        je      short L029F
        add     word ptr sp_map_tab[bx],2
        mov     [di],ax
        jmp     short L029F
L0281:  mov     cx,word ptr sp_inf_tab[si]
        test    cx,2
        jne     short L029F
        sub     di,2
        push    si
        mov     si,word ptr sp_map_tab[bx]
        mov     cx,word ptr sp_map_tab[bx+si]
        mov     [di],cx
        sub     word ptr sp_map_tab[bx],2
        pop     si
L029F:  mov     bx,word ptr sp_inf_tab[si+9]
        shl     bx,6
L02A6:  add     bx,40h
        mov     word ptr sp_list,bx
        cmp     bx,100h
        jae     short L032F
        mov     cx,word ptr sp_map_tab[bx]
        shr     cx,1
        je      L02A6
        mov     word ptr sp_left,cx
L02BF:  add     bx,2
        mov     di,word ptr sp_map_tab[bx]
        shl     di,4
        mov     ax,word ptr sp_inf_tab[di+2]
        mov     dx,word ptr sp_inf_tab[si+2]
        mov     cx,dx
        add     cx,word ptr sp_inf_tab[si+5]
        cmp     ax,cx
        jg      short L0322
        add     ax,word ptr sp_inf_tab[di+5]
        cmp     ax,dx
        jl      short L0322
        mov     al,byte ptr sp_inf_tab[di+4]
        mov     dl,byte ptr sp_inf_tab[si+4]
        mov     cl,dl
        sub     cl,byte ptr sp_inf_tab[si+7]
        cmp     cl,al
        jg      short L0322
        sub     al,byte ptr sp_inf_tab[di+7]
        cmp     dl,al
        jl      short L0322
        mov     ax,di
        shr     ax,4
        mov     bx,word ptr sp_inf_tab[di+9]
        shl     bx,6
        mov     di,offset sp_inf_end
        add     di,bx
        mov     cx,word ptr sp_inf_end[bx]
        shr     cx,1
        add     di,2
        repne   scasw
        je      short L0322
        add     word ptr sp_inf_end[bx],2
        mov     [di],ax
L0322:  dec     word ptr sp_left
        jne     L02BF
        mov     bx,word ptr sp_list
        jmp     L02A6
L032F:  ret
sp_changed endp

; update_sprites(void): if anything changed, with the mouse hidden: from the top layer
; down, restore what the sprites to be redrawn had covered (and free the blocks of freed
; sprites); then from the bottom layer up save what each visible one will cover and draw
; it (mask_to_screen, with byte +8 as its last argument, when that byte is set, else
; pic_to_screen); empty the redraw lists and clear sp_dirty.
_update_sprites proc far
        push    bp
        mov     bp,sp
        push    ds
        push    es
        push    si
        push    di
        cmp     cs:sp_dirty,0
        jne     short L0342
        jmp     L04DB
L0342:  mov     ax,DGROUP
        mov     ds,ax
        mov     es,ax
        assume  ds:DGROUP
        call    _mouse_hide
        mov     ax,seg sp_inf_tab
        mov     ds,ax
        assume  ds:seg sp_inf_tab
        mov     bx,offset sp_map_tab
L0356:  sub     bx,40h
L0359:  mov     cx,[bx]
        shr     cx,1
        je      short L03A6
        mov     si,2
L0362:  mov     di,[bx+si]
        shl     di,4
        test    word ptr sp_inf_tab[di],4
        je      short L0376
        and     word ptr sp_inf_tab[di],0FFFBh
        jmp     short L0383
L0376:  push    bx
        push    cx
        mov     ax,word ptr sp_inf_tab[di+0Dh]
        call    restore_rect
        pop     cx
        pop     bx
L0383:  test    word ptr sp_inf_tab[di],8
        je      short L03A1
        and     word ptr sp_inf_tab[di],0FFFEh
        mov     ax,word ptr sp_inf_tab[di+0Dh]
        or      ax,ax
        je      short L03A1
        push    bx
        push    cx
        call    far ptr _vfree+0Dh
        pop     cx
        pop     bx
L03A1:  add     si,2
        loop    L0362
L03A6:  sub     bx,40h
        cmp     bx,offset sp_inf_end
        ja      L0359
        mov     bx,offset sp_inf_end
        jmp     short L03FC
L03B4:  mov     cx,[bx]
        shr     cx,1
        jne     short L03BD
        jmp     L04C3
L03BD:  mov     si,2
L03C0:  mov     di,[bx+si]
        shl     di,4
        test    word ptr sp_inf_tab[di],2
        jne     short L03D4
        or      word ptr sp_inf_tab[di],4
        jmp     short L03F7
L03D4:  push    bx
        push    cx
        push    si
        mov     si,word ptr sp_inf_tab[di+0Dh]
        mov     ax,word ptr sp_inf_tab[di+2]
        xor     bh,bh
        mov     bl,byte ptr sp_inf_tab[di+4]
        mov     cx,word ptr sp_inf_tab[di+5]
        xor     dh,dh
        mov     dl,byte ptr sp_inf_tab[di+7]
        call    save_rect
        pop     si
        pop     cx
        pop     bx
L03F7:  add     si,2
        loop    L03C0
L03FC:  mov     cx,[bx]
        mov     word ptr [bx],0
        shr     cx,1
        jne     short L0409
        jmp     L04C3
L0409:  mov     si,2
L040C:  mov     di,[bx+si]
        shl     di,4
        test    word ptr sp_inf_tab[di],2
        jne     short L041C
        jmp     L04B9
L041C:  push    bx
        push    cx
        mov     al,byte ptr sp_inf_tab[di+8]
        or      al,al
        je      short L046C
        xor     ah,ah
        push    ax
        push    word ptr sp_inf_tab[di+5]
        mov     al,byte ptr sp_inf_tab[di+7]
        push    ax
        mov     al,byte ptr sp_inf_tab[di+4]
        push    ax
        push    word ptr sp_inf_tab[di+2]
        push    word ptr sp_inf_tab[di+0Bh]
        test    word ptr sp_inf_tab[di],10h
        mov     ax,DGROUP
        mov     ds,ax
        je      short L0457
        mov     ax,seg _Transparency
        mov     es,ax
        mov     es:_Transparency,1
L0457:  call    _mask_to_screen
        add     sp,0Ch
        mov     ax,seg _Transparency
        mov     es,ax
        mov     es:_Transparency,0
        jmp     short L04AF
L046C:  push    word ptr sp_inf_tab[di+5]
        xor     ah,ah
        mov     al,byte ptr sp_inf_tab[di+7]
        push    ax
        mov     al,byte ptr sp_inf_tab[di+4]
        push    ax
        push    word ptr sp_inf_tab[di+2]
        push    word ptr sp_inf_tab[di+0Bh]
        test    word ptr sp_inf_tab[di],10h
        mov     ax,DGROUP
        mov     ds,ax
        je      short L049C
        mov     ax,seg _Transparency
        mov     es,ax
        mov     es:_Transparency,1
L049C:  call    _pic_to_screen
        add     sp,0Ah
        mov     ax,seg _Transparency
        mov     es,ax
        mov     es:_Transparency,0
L04AF:  push    seg sp_inf_tab
        pop     ds
        push    seg sp_inf_tab
        pop     es
        pop     cx
        pop     bx
L04B9:  add     si,2
        loop    L04C0
        jmp     short L04C3
L04C0:  jmp     L040C
L04C3:  add     bx,40h
        cmp     bx,offset sp_map_tab
        jae     short L04CF
        jmp     L03B4
L04CF:  mov     ax,DGROUP
        mov     ds,ax
        mov     es,ax
        call    _mouse_show
L04DB:  mov     cs:sp_dirty,0
        pop     di
        pop     si
        pop     es
        pop     ds
        mov     sp,bp
        pop     bp
        ret
_update_sprites endp

SEG000_TEXT ends
        end
