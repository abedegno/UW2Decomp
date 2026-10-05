; /* target: seg001_2D1 */
; /* opts: /ml */
;
; One routine after VALLOC.ASM's code in seg001 (004E:02D1-038A, the end of segment table
; entry 1): valloc_stats, which reports how the video memory pool of VALLOC.ASM is used.
; UW2's seg001 ends with VALLOC.ASM's code; this routine is UW1's only.
;
;   void valloc_stats(int *nfree, int *nused, long *free_bytes, long *used_bytes,
;                     long *top_bytes, int *recs_left, int *gaps)
;
; The arguments are near pointers into DGROUP (the routine stores through ES, set to the
; caller's DS). It walks the block records after the sentinel, counting free blocks
; (+4 is 1) and used ones and adding up their sizes, and counts the places where a block
; does not start where the one before ended. Sizes are one plane's bytes, as valloc
; measures them, so each total is multiplied by four (two rcl pairs into a long). It also
; gives the pool left above the bump pointer (POOL_END - POOL_TOP, times four) and the
; number of records still free in the table plus one.
;
; Nothing in UW.EXE calls it: no far call to 004E:02D1 in the resident code or the
; overlays. It may have been a debugging aid.
;
; name: descriptive (no listing proc, no symbols.tsv name, no UW2 counterpart).
; match: one module or two cannot be told from the bytes. VALLOC.ASM's code ends at the odd
; offset 2D1h and this routine starts there with no padding, and its one relocation
; (entry 708) follows VALLOC.ASM's (695 to 707). It is a separate module here, its
; segment byte-aligned so that TLINK puts it right after VALLOC.ASM's code, which keeps
; VALLOC.ASM as it is; linked straight after VALLOC.ASM it gives the same EXE.
; match: 8086 instructions only, so no CPU directive.
; match: the segment has VALLOC.ASM's name and class (VALLOC_TEXT, ASMCODE: seg000 to
; seg002 are a class of their own, docs/LINKING.md).

; VALLOC.ASM's offsets in seg055, the allocator's far data.
POOL_END        equ     0Eh     ; the pool's end
POOL_TOP        equ     10h     ; the bump pointer
REC_END         equ     12h     ; the end of the block records
RECS            equ     14h     ; the first block record (0Dh bytes each)
RECS_END        equ     1054h   ; the record table's end
REC_SIZE        equ     0Dh     ; one block record: +0 video offset, +2 size, +4 1 if free

        extrn   seg055:far

VALLOC_TEXT segment byte public 'ASMCODE'
        assume  cs:VALLOC_TEXT, ds:nothing, es:nothing

        public  _valloc_stats

_valloc_stats proc far
        push    bp
        mov     bp,sp
        push    ds
        push    es
        push    si
        push    di
        push    bp
        mov     ax,ds
        mov     es,ax
        mov     ax,seg seg055
        mov     ds,ax
        xor     bp,bp                   ; gaps
        xor     cx,cx                   ; free blocks
        xor     dx,dx                   ; used blocks
        xor     si,si                   ; free size
        xor     di,di                   ; used size
        mov     bx,RECS
        add     bx,REC_SIZE             ; past the sentinel
        mov     ax,word ptr [bx]        ; where the next block should start
L02F4:
        cmp     byte ptr [bx+4],1
        je      short L0300
        inc     dx
        add     di,word ptr [bx+2]
        jmp     short L0304
L0300:
        inc     cx
        add     si,word ptr [bx+2]
L0304:
        cmp     ax,word ptr [bx]
        je      short L0309
        inc     bp
L0309:
        add     ax,word ptr [bx+2]
        add     bx,REC_SIZE
        cmp     bx,word ptr ds:[REC_END]
        jb      L02F4
        mov     ax,bp
        pop     bp
        mov     bx,word ptr [bp+6]
        mov     word ptr es:[bx],cx
        mov     bx,word ptr [bp+8]
        mov     word ptr es:[bx],dx
        mov     bx,word ptr [bp+12h]
        mov     word ptr es:[bx],ax
        xor     dx,dx
        mov     ax,RECS_END
        sub     ax,word ptr ds:[REC_END]
        mov     bx,REC_SIZE
        div     bx
        inc     ax
        mov     bx,word ptr [bp+10h]
        mov     word ptr es:[bx],ax
        xor     dx,dx
        rcl     si,1
        rcl     dx,1
        rcl     si,1
        rcl     dx,1
        mov     bx,word ptr [bp+0Ah]
        mov     word ptr es:[bx],si
        mov     word ptr es:[bx+2],dx
        xor     dx,dx
        rcl     di,1
        rcl     dx,1
        rcl     di,1
        rcl     dx,1
        mov     bx,word ptr [bp+0Ch]
        mov     word ptr es:[bx],di
        mov     word ptr es:[bx+2],dx
        xor     dx,dx
        mov     ax,word ptr ds:[POOL_END]
        sub     ax,word ptr ds:[POOL_TOP]
        rcl     ax,1
        rcl     dx,1
        rcl     ax,1
        rcl     dx,1
        mov     bx,word ptr [bp+0Eh]
        mov     word ptr es:[bx],ax
        mov     word ptr es:[bx+2],dx
        pop     di
        pop     si
        pop     es
        pop     ds
        mov     sp,bp
        pop     bp
        retf
_valloc_stats endp

VALLOC_TEXT ends

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

        end
