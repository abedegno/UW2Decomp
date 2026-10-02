; /* target: seg017_2179 */
; /* opts: /ml */
;
; VGA (mode X) screen access and far string helpers: DOS resident segment seg017.
;
; The game runs the VGA unchained (mode X): four planes, each byte of the 80-byte rows
; holding every fourth pixel, the plane chosen with the sequencer's map mask (3C4h index
; 2) for writes and the graphics controller's read map (3CEh index 4) for reads. The
; game's y counts up from the bottom, so line 199 - y is the screen row, offset from the
; current page's start (the word _dseg_67d6_21F4 points at). grab reads a block of the
; screen (the GIF writer, SCRSHOT.C), gr_pixel plots one pixel, and local_do_palette loads
; DAC entries from the library's palette (every palette change in GRFX.C, COLCYCLE.C and
; CUTS.C ends here). The three DRAW_RELATED routines keep pictures in spare video memory
; for LOADGR.C and GRSPIC.C: 2A2 stores an 8-bit bitmap behind a one-byte header (4, w
; and h in planes 0, 1 and 2), 320 reads that header back, and 361 allocates a block and
; copies a picture into it as it is. The string routines are far-pointer versions of the
; C library's (memset, strlen, strncpy, strcpy, strchr, strstr, strcmp) used across the
; game.
;
; name: descriptive (VGA mode X screen access and far string helpers). Routine names are
; those in symbols.tsv; grab and local_do_palette are the FM Towns names.
; match: this segment is an assembly module, not C. str_ncopy ends by jumping into the
; middle of str_copy, which no C compiler emits, and every routine keeps its own register
; conventions: DS, ES and BP are saved and reused freely, results come back in DX:AX
; straight from string instructions, and several frames end in `mov sp,bp; pop bp`
; with no locals, which neither Turbo C (`pop bp` or `leave`) nor TASM's own
; ARG/LOCAL frames produce. So the frames are written out by hand here. The 186
; instructions (shr reg,imm, outsb) need .186; with .186 TASM's generated frames
; would be enter/leave, which the EXE never has, another sign the frames were
; written by hand. Forward jumps carry no padding, so they were written `short`.

; The DOS (int 21h) functions used here.
DOS_PRINT_STRING equ     09h
; The VGA ports used here.
SC_INDEX        equ     3C4h  ; sequencer index
SC_DATA         equ     3C5h  ; sequencer data
DAC_WRITE_INDEX equ     3C8h  ; DAC write index
GC_INDEX        equ     3CEh  ; graphics controller index
GC_DATA         equ     3CFh  ; graphics controller data
INPUT_STATUS    equ     3DAh  ; input status 1 (vertical retrace)

        .model  medium
        .186

        extrn   _seg001_023B_5A:far             ; register entry inside valloc (004E:004A): AX size, BX count

        .data
        extrn   _palette:dword                  ; DS:21AC, far pointer to the RGB triples
        extrn   _dseg_67d6_21F4:dword           ; DS:21F4, far pointer to the screen's page offset

        .code

        public  _grab
        public  _PrintStringToConsole_seg017_DE
        public  _mem_set
        public  _str_len
        public  _str_ncopy
        public  _str_copy
        public  _FindStringDelimiter
        public  _str_str
        public  _str_cmp
        public  _local_do_palette
        public  _gr_pixel
        public  _DRAW_RELATED_seg017_2179_2A2
        public  _DRAW_RELATED_seg017_2179_320
        public  _DRAW_RELATED_seg017_2179_361

; void far grab(void far *dst, int x, int y, int w, int h)
; Copies a w by h block of the planar screen, top row at screen line 199 - y, into dst.
; dst is filled as a linear w-wide image: the routine reads one plane at a time (read map
; select), storing every fourth byte; each of its two passes walks down the rows for one
; plane and back up (std) for the next.
_grab   proc    far
        push    bp
        mov     bp,sp
        push    ds
        push    si
        push    di
        mov     dx,GC_INDEX                         ; graphics controller: read map select
        mov     al,4
        out     dx,al
        mov     ax,0C7h
        sub     ax,[bp+0Ch]
        mov     bx,50h
        mul     bx
        les     di,_dseg_67d6_21F4
        add     ax,es:[di]
        mov     si,ax
        mov     ax,[bp+0Eh]
        mov     dx,[bp+10h]
        mov     dh,al
        add     ax,3
        shr     ax,2
        sub     bx,ax
        mov     ax,[bp+0Ah]
        mov     cx,ax
        shr     cx,2
        add     si,cx
        and     al,3
        les     di,[bp+6]
        mov     cx,0A000h
        mov     ds,cx
        push    bp
        mov     bp,bx
        mov     bl,dh
        and     bx,3
        jz      grab_planes
        sub     bx,4
grab_planes:
        mov     cx,2
grab_pass:
        push    cx
        push    dx
        mov     dx,GC_DATA
        out     dx,al
        pop     dx
        mov     ah,dl
grab_down:
        mov     cl,dh
        add     cl,3
        shr     cl,2
grab_down_row:
        movsb
        add     di,3
        loop    grab_down_row
        add     si,bp
        add     di,bx
        dec     ah
        jnz     grab_down
        inc     al
        and     al,3
        jnz     grab_down_next
        inc     si
grab_down_next:
        dec     dh
        test    dh,3
        jnz     grab_down_plane
        inc     bp
        add     bx,4
grab_down_plane:
        push    dx
        mov     dx,GC_DATA
        out     dx,al
        pop     dx
        sub     di,3
        sub     di,bx
        sub     si,bp
        dec     si
        std
        mov     ah,dl
grab_up:
        mov     cl,dh
        add     cl,3
        shr     cl,2
grab_up_row:
        movsb
        sub     di,3
        loop    grab_up_row
        sub     si,bp
        sub     di,bx
        dec     ah
        jnz     grab_up
        cld
        inc     al
        and     al,3
        jnz     grab_up_next
        inc     si
grab_up_next:
        dec     dh
        add     si,bp
        inc     si
        add     di,5
        add     di,bx
        test    dh,3
        jnz     grab_up_plane
        inc     bp
        add     bx,4
grab_up_plane:
        pop     cx
        dec     cx
        jnz     grab_pass
        mov     dx,GC_INDEX                         ; graphics controller: bit mask FF
        mov     ax,0FF08h
        out     dx,ax
        pop     bp
        pop     di
        pop     si
        pop     ds
        mov     sp,bp
        pop     bp
        ret
_grab   endp

; void far PrintStringToConsole_seg017_DE(char far *s)    DOS print, '$' terminated
_PrintStringToConsole_seg017_DE proc far
        push    bp
        mov     bp,sp
        push    dx
        push    ds
        lds     dx,[bp+6]
        mov     ah,DOS_PRINT_STRING
        int     21h
        pop     ds
        pop     dx
        pop     bp
        ret
_PrintStringToConsole_seg017_DE endp

; void far mem_set(void far *dst, char value, unsigned count)
_mem_set proc   far
        push    bp
        mov     bp,sp
        push    di
        les     di,[bp+6]
        mov     cx,[bp+0Ch]
        mov     ax,[bp+0Ah]
        mov     ah,al
        shr     cx,1
        rep     stosw
        jnc     mem_set_done
        mov     es:[di],al
mem_set_done:
        pop     di
        pop     bp
        ret
_mem_set endp

; int far str_len(char far *s)
_str_len proc   far
        push    bp
        mov     bp,sp
        push    di
        les     di,[bp+6]
        mov     cx,0FFFFh
        xor     al,al
        repne   scasb
        not     cx
        dec     cx
        mov     ax,cx
        pop     di
        pop     bp
        ret
_str_len endp

; char far * far str_ncopy(char far *dst, char far *src, int n)
; Copies src with its terminator, but at most n bytes; shares str_copy's copy loop.
_str_ncopy proc far
        push    bp
        mov     bp,sp
        push    ds
        push    es
        push    si
        push    di
        xor     al,al
        mov     cx,0FFFFh
        les     di,[bp+0Ah]
        mov     si,di
        repne   scasb
        not     cx
        mov     dx,[bp+0Eh]
        cmp     cx,dx
        jle     short str_copy_count
        mov     cx,dx
        jmp     short str_copy_count
_str_ncopy endp

; char far * far str_copy(char far *dst, char far *src)
_str_copy proc  far
        push    bp
        mov     bp,sp
        push    ds
        push    es
        push    si
        push    di
        xor     al,al
        mov     cx,0FFFFh
        les     di,[bp+0Ah]
        mov     si,di
        repne   scasb
        not     cx
str_copy_count:
        mov     bx,cx
        shr     cx,1
        mov     ax,es
        mov     ds,ax
        les     di,[bp+6]
        mov     dx,es
        mov     ax,di
        rep     movsw
        test    bx,1
        jz      str_copy_done
        movsb
str_copy_done:
        pop     di
        pop     si
        pop     es
        pop     ds
        mov     sp,bp
        pop     bp
        ret
_str_copy endp

; char far * far FindStringDelimiter(char far *s, int c)    strchr: the first c in s, or 0
_FindStringDelimiter proc far
        push    bp
        mov     bp,sp
        push    di
        les     di,[bp+6]
        mov     cx,0FFFFh
        xor     ax,ax
        repne   scasb
        not     cx
        mov     ax,[bp+0Ah]
        les     di,[bp+6]
        repne   scasb
        jnz     short find_none
        mov     dx,es
        mov     ax,di
        dec     ax
        jmp     short find_done
find_none:
        xor     dx,dx
        xor     ax,ax
find_done:
        pop     di
        pop     bp
        ret
_FindStringDelimiter endp

; char far * far str_str(char far *s, char far *find)    strstr: the first find in s, or 0
_str_str proc   far
        push    bp
        mov     bp,sp
        push    ds
        push    si
        push    di
        les     di,[bp+0Ah]
        mov     ax,es
        mov     ds,ax
        mov     si,di
        xor     al,al
        mov     cx,0FFFFh
        repne   scasb
        not     cx
        dec     cx
        mov     dx,cx
        les     di,[bp+6]
        mov     cx,0FFFFh
        repne   scasb
        not     cx
        sub     di,cx
        dec     cx
        mov     bx,si
        dec     di
        mov     bp,di
str_str_next:
        mov     si,bx
        inc     bp
        mov     di,bp
        mov     al,[si]
        repne   scasb
        jnz     short str_str_none
        dec     di
        mov     bp,di
        inc     cx
        cmp     cx,dx
        jb      short str_str_none
        mov     ax,cx
        mov     cx,dx
        repe    cmpsb
        mov     cx,ax
        jnz     str_str_next
        sub     di,dx
        mov     dx,es
        mov     ax,di
        jmp     short str_str_done
str_str_none:
        xor     dx,dx
        xor     ax,ax
str_str_done:
        pop     di
        pop     si
        pop     ds
        pop     bp
        ret
_str_str endp

; int far str_cmp(char far *a, char far *b)
_str_cmp proc   far
        push    bp
        mov     bp,sp
        push    ds
        push    si
        push    di
        xor     ax,ax
        les     di,[bp+6]
        mov     cx,0FFFFh
        repne   scasb
        not     cx
        sub     di,cx
        lds     si,[bp+0Ah]
        repe    cmpsb
        mov     al,es:[di-1]
        xor     dx,dx
        mov     dl,[si-1]
        sub     ax,dx
        pop     di
        pop     si
        pop     ds
        pop     bp
        ret
_str_cmp endp

; void far local_do_palette(int count, unsigned char first)
; Waits for vertical retrace and loads count DAC entries from palette, starting at first.
_local_do_palette proc far
        push    bp
        mov     bp,sp
        push    si
        push    di
        mov     dx,INPUT_STATUS
palette_wait:
        in      al,dx
        and     al,8
        jz      palette_wait
        mov     al,[bp+8]
        xor     ah,ah
        mov     di,ax
        mov     dx,DAC_WRITE_INDEX
        out     dx,al
        inc     dx
        mov     cx,[bp+6]
        push    ds
        lds     si,_palette
        add     si,di
        add     si,di
        add     si,di
        mov     ax,cx
        shl     ax,1
        add     cx,ax
palette_out:
        outsb
        loop    palette_out
        pop     ds
        pop     di
        pop     si
        pop     bp
        ret
_local_do_palette endp

; void far gr_pixel(int x, int y, int colour)    screen line 199 - y
_gr_pixel proc  far
        push    bp
        mov     bp,sp
        sub     sp,0Eh
        push    si
        push    di
        les     bx,_dseg_67d6_21F4
        mov     ax,es:[bx]
        mov     bx,ax
        mov     ax,0A000h
        mov     es,ax
        mov     ax,50h
        mov     dx,0C7h
        sub     dx,[bp+8]
        imul    dx
        mov     dx,[bp+6]
        mov     cl,2
        shr     dx,cl
        add     dx,ax
        add     bx,dx
        mov     dx,SC_INDEX                         ; sequencer: map mask
        mov     al,2
        out     dx,al
        mov     dx,SC_DATA
        mov     cx,[bp+6]
        and     cx,3
        mov     ax,1
        shl     ax,cl
        out     dx,al
        mov     ax,[bp+0Ah]
        mov     es:[bx],al
        pop     di
        pop     si
        mov     sp,bp
        pop     bp
        ret
_gr_pixel endp

; int far DRAW_RELATED_seg017_2179_2A2(void far *src, int dst, int w, int h)
; Writes a header byte at dst holding 4, w and h in planes 0, 1 and 2, then the w by h
; image after it, a plane at a time; returns dst.
_DRAW_RELATED_seg017_2179_2A2 proc far
        push    bp
        mov     bp,sp
        sub     sp,2
        push    ds
        push    si
        push    di
        mov     dx,SC_INDEX
        mov     al,2
        out     dx,al
        mov     ax,0A000h
        mov     es,ax
        mov     ax,1001h
        mov     dx,SC_DATA
        mov     di,[bp+0Ah]
        out     dx,al
        mov     byte ptr es:[di],4
        shl     al,1
        out     dx,al
        mov     cx,[bp+0Ch]
        mov     es:[di],cl
        shl     al,1
        out     dx,al
        mov     cx,[bp+0Eh]
        mov     es:[di],cl
        inc     word ptr [bp+0Ah]
        mov     dx,si
        lds     bx,[bp+6]
        mov     ax,1001h
        xor     dx,dx
draw_plane:
        mov     dx,SC_DATA
        out     dx,al
        mov     dx,[bp+0Eh]
        mov     [bp-2],dx
        mov     di,[bp+0Ah]
draw_row:
        mov     si,bx
        mov     cx,[bp+0Ch]
        add     cx,3
        shr     cx,2
draw_byte:
        movsb
        add     si,3
        loop    draw_byte
        add     bx,[bp+0Ch]
        dec     word ptr [bp-2]
        jnz     draw_row
        inc     word ptr [bp+6]
        mov     bx,[bp+6]
        shl     al,1
        cmp     al,ah
        jb      draw_plane
        mov     ax,[bp+0Ah]
        dec     ax
        pop     di
        pop     si
        pop     ds
        mov     sp,bp
        pop     bp
        ret
_DRAW_RELATED_seg017_2179_2A2 endp

; void far DRAW_RELATED_seg017_2179_320(unsigned offset, int far *width, int far *height)
; Reads planes 1 and 2 of the byte at offset (a header written by 2A2) into two ints.
_DRAW_RELATED_seg017_2179_320 proc far
        push    bp
        mov     bp,sp
        push    ds
        push    si
        mov     dx,GC_INDEX
        mov     al,4
        out     dx,al
        mov     bx,0A000h
        mov     es,bx
        mov     bx,[bp+6]
        mov     dx,GC_DATA
        mov     al,1
        out     dx,al
        mov     cl,es:[bx]
        lds     si,[bp+8]
        mov     [si],cl
        mov     byte ptr [si+1],0
        inc     al
        out     dx,al
        lds     si,[bp+0Ch]
        mov     cl,es:[bx]
        mov     [si],cl
        mov     byte ptr [si+1],0
        mov     dx,GC_INDEX
        mov     ax,0FF08h
        out     dx,ax
        pop     si
        pop     ds
        mov     sp,bp
        pop     bp
        ret
_DRAW_RELATED_seg017_2179_320 endp

; unsigned far DRAW_RELATED_seg017_2179_361(void far *src, int size)
; Allocates video memory for size bytes and copies src into it a plane at a time;
; returns the video offset, or 0. The map mask is set only before planes 1 to 3, so the
; first quarter goes to whichever planes the mask last enabled.
_DRAW_RELATED_seg017_2179_361 proc far
        push    bp
        mov     bp,sp
        push    ds
        push    si
        push    di
        mov     dx,SC_INDEX
        mov     al,2
        out     dx,al
        mov     ax,[bp+0Ah]
        mov     bx,1
        call    _seg001_023B_5A
        or      ax,ax
        jz      short store_done
        mov     bx,ax
        mov     dx,[bp+0Ah]
        add     dx,3
        shr     dx,2
        mov     ax,0A000h
        mov     es,ax
        mov     ax,1001h
store_plane:
        lds     si,[bp+6]
        mov     di,bx
        mov     cx,dx
store_byte:
        movsb
        add     si,3
        loop    store_byte
        inc     word ptr [bp+6]
        shl     al,1
        cmp     al,ah
        jnb     short store_result
        push    dx
        mov     dx,SC_DATA
        out     dx,al
        pop     dx
        jmp     store_plane
store_result:
        mov     ax,bx
store_done:
        pop     di
        pop     si
        pop     ds
        mov     sp,bp
        pop     bp
        ret
_DRAW_RELATED_seg017_2179_361 endp

; An empty routine at the end of the segment, called from nowhere.
seg017_2179_3B6 proc far
        push    bp
        mov     bp,sp
        mov     sp,bp
        pop     bp
        ret
seg017_2179_3B6 endp

        end
