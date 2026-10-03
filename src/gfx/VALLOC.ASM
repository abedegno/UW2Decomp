; /* target: seg001_023B */
; /* opts: /ml */
;
; A small block allocator for spare video memory, and the save/restore of screen
; rectangles into its blocks: DOS segment seg001.
;
; The VGA's 256 KB hold more than the visible pages. seg001_023B_C, called once from
; UWEDIT.C at start-up, takes all the video memory the graphics library has not claimed
; (its bump pointer at seg_370D:410Ah, limit at 4108h, advanced by seg003_0272_49AE) as
; this allocator's pool. valloc hands out blocks of it, measured as one plane's bytes
; ((w / 4 + 1) * h); LOADGR.C keeps the screen furniture's pictures in such blocks, and
; SPRITE.ASM saves what each sprite covers in one. save_rect and restore_rect copy a
; screen rectangle into a block and back through the graphics library's rectangle with
; the special pens 109h and 10Ah (probably copy-to and copy-from the block whose offset
; is put at seg_370D:646h or 648h; that is how the routines use them, the library side
; is not yet read).
;
; The allocator's data is the far segment seg_5F5D: [4] the pool's start, [6] its end,
; [8] the bump pointer, [0Ah] the end of the block records, which start at 0Ch, 0Dh
; bytes each: +0 video offset, +2 size, +4 1 if free, +5 x, +7 y, +9 w, +0Bh h (the
; rectangle saved in it). Freed blocks are merged with free neighbours, and a free block
; at the top of the pool goes back to the bump pointer.
;
; Each routine has two entries. The C entry (valloc, vfree, save_rect, restore_rect)
; builds a bp frame, loads its stack arguments into registers, sets c_entry and falls
; into the register entry, which the assembly callers use directly (seg000 calls
; save_rect and restore_rect, MODEX calls seg001_023B_5A). On the way out the register
; routine pops the C entry's frame only when c_entry is set, then clears it.
;
; name: original. valloc and vfree are in System Shock's 2D library VALLOC.C (RCS
; n:/project/lib/src/2d/valloc.c, "video memory management"), the same job.
; match: hand-written assembly with .186 (push 109h assembles as 68 09 01, and valloc has
; shr ax,2 as C1 E8 02; TASM without .186 would expand the push).
; match: the module starts at segment offset 0Ah with the c_entry word (IDA's org 0Ah, dw
; 0): every entry writes cs:[0Ah], and the word at file 30EA follows seg000's retf at
; 30E9, so the target table now starts there (base 0x30EA, org 0xA) rather than at 0Ch.
; name: valloc, vfree, save_rect and restore_rect are in symbols.tsv (FM Towns). The FM
; Towns build also has the register entries of the last two: save_rect (fd91: stores the
; rectangle in its record, set_the_color 109h, rectangle) and restore_rect (fdc4: colour
; 10Ah, reloads the rectangle, rectangle), the same work as +1EF and +268 here, so those
; take the FM names (without an underscore, as assembly symbols; the C names have one).
; FM's valloc and vfree are malloc/free wrappers with no register entry, so +5A keeps its
; symbols.tsv name and +112 is a local label. seg_5F5D (the allocator's data) and
; seg_370D keep provisional names.

        .186

        extrn   _rectangle:far
        extrn   _seg003_0272_49AE:far
        extrn   _set_the_color:far
        extrn   seg_370D:far
        extrn   seg_5F5D:far

; Class ASMCODE, as SPRITE.ASM's: seg000 to seg002 are a class of their own that ends in CODE
; (docs/LINKING.md, Segment classes). TASM keeps a .model segment's class, so the segment
; and DGROUP are declared here, as .model medium would, under the name it gave.
VALLOC_TEXT segment word public 'ASMCODE'
        assume  cs:VALLOC_TEXT, ds:DGROUP

        public  _seg001_023B_C
        public  _valloc
        public  _vfree
        public  _save_rect
        public  _restore_rect
        public  _seg001_023B_5A
        public  save_rect
        public  restore_rect

c_entry dw      0                       ; 1 while a C entry's frame is live

; seg001_023B_C  (+C): void seg001_023B_C(void)
; Initialises the pool: claims the graphics library's free video memory (all of it but
; one byte) and sets the pool to run from what that returns up to the library's limit,
; with one block record (the first, a sentinel for vfree's merge with the block before).
_seg001_023B_C proc far
        push    ds
        push    es
        push    si
        push    di
        mov     ax,seg seg_370D
        mov     es,ax
        mov     ax,word ptr es:[4108h]
        sub     ax,word ptr es:[410Ah]
        dec     ax
        push    ax
        call    far ptr _seg003_0272_49AE
        add     sp,2
        mov     bx,word ptr es:[4108h]
        mov     cx,seg seg_5F5D
        mov     ds,cx
        mov     word ptr ds:[6],bx
        mov     word ptr ds:[4],ax
        mov     word ptr ds:[8],ax
        mov     ax,0Ch
        add     ax,0Dh
        mov     word ptr ds:[0Ah],ax
        pop     di
        pop     si
        pop     es
        pop     ds
        retf
_seg001_023B_C endp

; seg001_023B_4A  (+4A): int valloc(int w, int h)
; Register entry seg001_023B_5A: AX = w, BX = h. Returns AX = the block's video offset,
; or 0 when the pool is full or a split finds the record table full (its end at 104Ch;
; only the split path checks it, after it has already moved the records up). Size is
; (w / 4 + 1) * h.
; First fit: a free record of exactly that size is reused; a larger free record is split,
; the records after it moved up to make room for the remainder; failing both, a new block
; comes from the bump pointer.
_valloc proc far
        push    bp
        mov     bp,sp
        mov     ax,word ptr [bp+6]
        mov     bx,word ptr [bp+8]
        mov     word ptr cs:c_entry,1
_seg001_023B_5A label far
        push    ds
        push    es
        push    si
        push    di
        mov     cx,seg seg_5F5D
        mov     ds,cx
        mov     es,cx
        shr     ax,2
        inc     ax
        mul     bx
        mov     bx,0Ch
L006E:
        cmp     byte ptr [bx+4],0
        je      short L007D
        mov     dx,word ptr [bx+2]
        cmp     ax,dx
        je      short L00BE
        jb      short L0088
L007D:
        add     bx,0Dh
        cmp     bx,word ptr ds:[0Ah]
        jb      L006E
        jmp     short L00C6
L0088:
        mov     cx,word ptr ds:[0Ah]
        mov     si,cx
        sub     cx,bx
        mov     di,si
        add     di,0Dh
        std
        rep movsb
        cld
        mov     cx,104Ch
        cmp     word ptr ds:[0Ah],cx
        jb      short L00A6
        xor     ax,ax
        jmp     short L00EE
L00A6:
        add     word ptr ds:[0Ah],0Dh
        mov     word ptr [bx+2],ax
        sub     dx,ax
        mov     word ptr [bx+0Fh],dx
        mov     byte ptr [bx+11h],1
        mov     si,word ptr [bx]
        add     ax,si
        mov     word ptr [bx+0Dh],ax
L00BE:
        mov     byte ptr [bx+4],0
        mov     ax,word ptr [bx]
        jmp     short L00EE
L00C6:
        mov     dx,word ptr ds:[8]
        mov     cx,dx
        add     cx,ax
        jb      short L00D6
        cmp     cx,word ptr ds:[6]
        jbe     short L00DA
L00D6:
        xor     ax,ax
        jmp     short L00EE
L00DA:
        mov     word ptr ds:[8],cx
        mov     word ptr [bx],dx
        mov     word ptr [bx+2],ax
        mov     byte ptr [bx+4],0
        add     word ptr ds:[0Ah],0Dh
        mov     ax,word ptr [bx]
L00EE:
        pop     di
        pop     si
        pop     es
        pop     ds
        cmp     word ptr cs:c_entry,1
        jne     short L00FD
        mov     sp,bp
        pop     bp
L00FD:
        mov     word ptr cs:c_entry,0
        retf
_valloc endp

; seg001_023B_105  (+105): int vfree(int block)
; Register entry L0112 (_vfree+0Dh, which SPRITE.ASM calls): AX = the block's offset.
; Returns 1 if no record has that offset, else 0. Merges the block with a free record
; just before or after it (removing the merged records), else marks it free; a block
; that ends at the bump pointer is returned to it.
_vfree proc far
        push    bp
        mov     bp,sp
        mov     ax,word ptr [bp+6]
        mov     word ptr cs:c_entry,1
L0112:
        push    ds
        push    es
        push    si
        push    di
        mov     bx,seg seg_5F5D
        mov     ds,bx
        mov     es,bx
        mov     bx,0Ch
L0120:
        cmp     word ptr [bx],ax
        je      short L0133
        add     bx,0Dh
        cmp     bx,word ptr ds:[0Ah]
        jne     L0120
        mov     ax,1
        jmp     L01BE
L0133:
        mov     si,bx
        mov     di,bx
        xor     bx,bx
        xor     cx,cx
        cmp     byte ptr [di-9],0
        je      short L015D
        mov     dx,word ptr [di-0Dh]
        mov     bx,word ptr [di-0Bh]
        add     dx,bx
        cmp     ax,dx
        jne     short L015D
        mov     cx,word ptr ds:[0Ah]
        sub     word ptr ds:[0Ah],0Dh
        sub     cx,di
        add     si,0Dh
        jmp     short L0163
L015D:
        cmp     byte ptr [di+11h],0
        je      short L01A4
L0163:
        cmp     byte ptr [di+11h],0
        je      short L0191
        add     ax,word ptr [di+2]
        mov     dx,word ptr [di+0Dh]
        cmp     ax,dx
        jne     short L0191
        sub     word ptr ds:[0Ah],0Dh
        add     bx,word ptr [di+0Fh]
        add     si,0Dh
        or      cx,cx
        jne     short L0191
        mov     bx,word ptr [di+2]
        add     si,0Dh
        mov     cx,word ptr ds:[0Ah]
        sub     cx,di
        add     di,0Dh
L0191:
        add     bx,word ptr [di+2]
        mov     word ptr [di-0Bh],bx
        mov     byte ptr [di-9],1
        rep movsb
        xor     ax,ax
        sub     di,0Dh
        jmp     short L01A8
L01A4:
        mov     byte ptr [di+4],1
L01A8:
        mov     bx,word ptr [di]
        mov     cx,word ptr [di+2]
        add     bx,cx
        cmp     bx,word ptr ds:[8]
        jne     short L01BE
        sub     word ptr ds:[8],cx
        sub     word ptr ds:[0Ah],0Dh
L01BE:
        pop     di
        pop     si
        pop     es
        pop     ds
        cmp     word ptr cs:c_entry,1
        jne     short L01CD
        mov     sp,bp
        pop     bp
L01CD:
        mov     word ptr cs:c_entry,0
        retf
_vfree endp

; seg001_023B_1D5  (+1D5): int save_rect(int block, int x, int y, int w, int h)
; Register entry save_rect: SI block, AX x, BX y (the top row, bottom-up), CX w, DX h.
; Records the rectangle in the block's record and copies the screen's x, y .. x+w-1,
; y-h+1 into the block (pen 109h). Returns 0, or 1 if the block is unknown.
_save_rect proc far
        push    bp
        mov     bp,sp
        push    si
        mov     si,word ptr [bp+6]
        mov     ax,word ptr [bp+8]
        mov     bx,word ptr [bp+0Ah]
        mov     cx,word ptr [bp+0Ch]
        mov     dx,word ptr [bp+0Eh]
        mov     word ptr cs:c_entry,1
save_rect label far
        push    ds
        push    es
        push    di
        mov     di,seg seg_5F5D
        mov     ds,di
        mov     di,seg seg_370D
        mov     es,di
        mov     di,0Ch
L01FF:
        cmp     word ptr [di],si
        je      short L0211
        add     di,0Dh
        cmp     di,word ptr ds:[0Ah]
        jb      L01FF
        mov     ax,1
        jmp     short L0244
L0211:
        mov     word ptr [di+5],ax
        mov     word ptr [di+7],bx
        mov     word ptr [di+9],cx
        mov     word ptr [di+0Bh],dx
        add     cx,ax
        dec     cx
        mov     dx,bx
        sub     dx,word ptr [di+0Bh]
        inc     dx
        push    dx
        push    cx
        push    bx
        push    ax
        mov     word ptr es:[646h],si
        push    109h
        call    far ptr _set_the_color
        add     sp,2
        call    far ptr _rectangle
        add     sp,8
        xor     ax,ax
L0244:
        pop     di
        pop     es
        pop     ds
        cmp     word ptr cs:c_entry,1
        jne     short L0253
        pop     si
        mov     sp,bp
        pop     bp
L0253:
        mov     word ptr cs:c_entry,0
        retf
_save_rect endp

; MaybeRedrawSlot_seg001_023B_25B  (+25B): int restore_rect(int block)
; Register entry restore_rect: AX block. Copies the block back to the rectangle recorded
; by save_rect (pen 10Ah). Returns 0, or 1 if the block is unknown.
_restore_rect proc far
        push    bp
        mov     bp,sp
        mov     ax,word ptr [bp+6]
        mov     word ptr cs:c_entry,1
restore_rect label far
        push    ds
        push    si
        push    di
        mov     dx,seg seg_5F5D
        mov     ds,dx
        mov     dx,seg seg_370D
        mov     es,dx
        mov     si,0Ch
        mov     cx,word ptr ds:[0Ah]
L027C:
        cmp     word ptr [si],ax
        je      short L028C
        add     si,0Dh
        cmp     si,cx
        jb      L027C
        mov     ax,1
        jmp     short L02BB
L028C:
        mov     word ptr es:[648h],ax
        push    10Ah
        call    far ptr _set_the_color
        add     sp,2
        mov     ax,word ptr [si+5]
        mov     bx,word ptr [si+7]
        mov     cx,ax
        add     cx,word ptr [si+9]
        dec     cx
        mov     dx,bx
        sub     dx,word ptr [si+0Bh]
        inc     dx
        push    dx
        push    cx
        push    bx
        push    ax
        call    far ptr _rectangle
        add     sp,8
        xor     ax,ax
L02BB:
        pop     di
        pop     si
        pop     ds
        cmp     word ptr cs:c_entry,1
        jne     short L02C9
        mov     sp,bp
        pop     bp
L02C9:
        mov     word ptr cs:c_entry,0
        retf
_restore_rect endp

VALLOC_TEXT ends

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

        end
