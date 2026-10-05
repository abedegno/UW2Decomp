; /* target: seg004_0 */
; /* opts: /ml */
;
; The image decoders: uncmp_pal, and exp_8str, exp_4str, exp_4run, exp_5run and exp_8run (FM
; Towns names) with their helpers.
;
; What it does in the game: turns a .GR/.CR image (object sprites, critter frames) from its
; stored form into one byte per pixel, translated to screen colours through the image's
; auxiliary palette and, optionally, one row of the distance-shading table lightabs. The 3D
; view uses it for every object and critter sprite it draws.
;
; Entry points: the five exp_ routines, reached only through PGCACHE's uncmp_tab, which is
; indexed by the image's format byte used directly as a word offset: 2 exp_8run, 4 exp_8str,
; 6 exp_5run, 8 exp_4run, 0Ah exp_4str (UW-Formats 3.2: 04 8-bit uncompressed, 06 5-bit
; run-length, 08 4-bit run-length, 0A 4-bit uncompressed). Format 0 is PGCACHE's int 3 trap.
; Common inputs (from do_uwobj): AX = the paragraph of the image, whose header is at AX:0
; (format, width, height, auxiliary palette number); BP = the offset of its size word (4);
; DS:SI = the 16 or 32 byte auxiliary palette (LOADGR's _Palettes); DH = the lightabs row to
; shade with, 0FFh for none. Output: the pixels at AX:0 on return, AX = their paragraph
; (cmpbuf1_start for the uncompressed forms, cmpbuf1_start+180h for the run-length forms,
; which unpack their words to cmpbuf1_start:0 first and decode to cmpbuf1_start:1800h).
;
; Data it owns: uncmp_pal, the 32-byte translation table at CS:0 that seg004_63 fills
; from the auxiliary palette before each 4-bit or 5-bit decode (INSTANCE.ASM keeps a far
; pointer to it in UW2). The decoders write the code segment, so seg004 must be in writable memory,
; which it is in real mode.
;
; The run-length format (UW-Formats 3.2.2): a stream of 4- or 5-bit words alternating repeat
; records (a count and one word repeated) and run records (a count and that many literal
; words). A count word of 0 extends the count over more words; in a repeat record a count of
; 1 means no repeat (go straight to the run record) and 2 means "several repeat records in a
; row", whose number follows as a count. seg004_194 is that decoder; it works on the
; words after they have been unpacked one per byte.
;
; UW1: UW2's EXPAND.ASM (src/3d/EXPAND.ASM there) byte for byte but for the size of the
; unpacking area: the run-length forms decode to cmpbuf1_start:1800h (AX + 180h), where UW2
; uses 5400h (540h).
;
; name: UW2's (descriptive there); the routine names are FM Towns', through UW2.
;
; .186, where UW2 has .386: UW1's seg004 has no 386 instruction and none of the nop padding
; that TASM's .386 mode puts after forward conditional jumps.
;
; SEG004 split into its original modules: this is module 1 of 9, +0..+260 of the segment
; (file A070h), a module of the second library, as in UW2.
; Procs: exp_8str, exp_4str, seg004_63, seg004_CB, exp_4run, exp_5run, exp_8run,
; seg004_18D, seg004_194.
; match: evidence for the bounds: it starts the segment (paragraph-aligned in the segment
; table); it ends where the next module's padding begins.
; Its relocations are entries 2588-2596 of UW.EXE's relocation table.

        .186

        extrn   _cmpbuf1_start:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public 'code'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   lightabs:byte
        public  _exp_4run
        public  _exp_4str
        public  _exp_5run
        public  _exp_8run
        public  _exp_8str
        public  _seg004_18D
        public  _seg004_194
        public  _seg004_63
        public  _seg004_CB
        public  _uncmp_pal

_uncmp_pal      db      0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah
                db      0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah,0Ah

; seg004_20  (+20)
; exp_8str: format 4, 8-bit uncompressed. In: AX = image paragraph, BP = offset of its size
; word, DH = lightabs row. Copies `size` bytes to cmpbuf1_start:0, each translated through
; lightabs row DH (BX from seg004_CB; xlat reads cs:BX+AL). Out: AX = cmpbuf1_start,
; ES:DI past the last pixel. The aux palette is not used: 8-bit pixels are already colours.
_exp_8str proc near
        call    _seg004_CB
        mov     si,bp
        mov     dx,seg _cmpbuf1_start
        mov     es,dx
        mov     di,0
        lodsw
        mov     cx,ax
L0030:
        lodsb
        xlat    byte ptr cs:_uncmp_pal[bx]
        stosb
        loop    L0030
        mov     ax,dx
        ret
_exp_8str endp

; seg004_39  (+39)
; exp_4str: format 0Ah, 4-bit uncompressed. In: as exp_8str, plus DS:SI = the 16-colour aux
; palette. Builds the 16-entry uncmp_pal (seg004_63, CX = 1 row), then for each of
; `size` source bytes writes two pixels, high nibble first, each through uncmp_pal, to
; cmpbuf1_start:0. Out: AX = cmpbuf1_start. Note the loop counts bytes, two pixels each.
_exp_4str proc near
        mov     cx,1
        call    _seg004_63
        mov     si,bp
        mov     dx,seg _cmpbuf1_start
        mov     es,dx
        mov     di,0
        lodsw
        mov     cx,ax
L004C:
        lodsb
        mov     ah,al
        and     al,0F0h
        shr     al,4
        xlat    byte ptr cs:_uncmp_pal[bx]
        stosb
        mov     al,ah
        and     al,0Fh
        xlat    byte ptr cs:_uncmp_pal[bx]
        stosb
        loop    L004C
        mov     ax,dx
        ret
_exp_4str endp

; seg004_63  (+63)
; Builds uncmp_pal (at CS:0) for a 4-bit or 5-bit image. In: DS:SI = the aux palette, CX =
; its length in 16-byte rows (1 for 4-bit, 2 for 5-bit), AX = image paragraph, DH = the
; lightabs row or 0FFh. With DH = 0FFh the palette is copied as it is; otherwise each entry
; is passed through lightabs row DH, so the image comes out shaded for its distance. The loop
; is unrolled 16 times, one row per iteration. Out: DS = the image (from AX), BX = 0 (the
; offset of uncmp_pal, for the callers' xlat), SI past the palette, ES = SEG004_TEXT.
_seg004_63 proc near
        mov     di,0
        mov     bx,SEG004_TEXT
        mov     es,bx
        cmp     dh,0FFh
        jne     short L0079
        mov     dx,ax
        shl     cx,4
        rep movsb
        jmp     short L00C5
L0079:
        mov     bh,dh
        xor     bl,bl
        add     bx,offset lightabs
        mov     dx,ax
L0083:
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        lodsb
        xlat    byte ptr es:[bx]
        stosb
        loop    L0083
L00C5:
        mov     bx,0
        mov     ds,dx
        ret
_seg004_63 endp

; seg004_CB  (+CB)
; exp_8str's set-up: BX = offset lightabs + DH * 256 (the shading row), DS = AX (the image).
; The cmp's result is overwritten by the add, so DH = 0FFh is not treated specially here
; (it would index past lightabs' 16 rows); probably no caller asks for an unshaded 8-bit
; image, but that is not checked.
_seg004_CB proc near
        cmp     dh,0FFh
        mov     bh,dh
        xor     bl,bl
        add     bx,offset lightabs
        mov     ds,ax
        ret
_seg004_CB endp

; seg004_D9  (+D9)
; exp_4run: format 8, 4-bit run-length. In: as exp_4str; here the size word counts 4-bit
; words (nibbles). Builds uncmp_pal, unpacks the (size + 1) / 2 bytes to one nibble per byte
; at cmpbuf1_start:0 (high nibble first, untranslated), then runs the record decoder
; seg004_18D over those `size` bytes into cmpbuf1_start:1800h, translating each pixel
; through uncmp_pal. Out: AX = cmpbuf1_start + 180h, the paragraph of the pixels.
_exp_4run proc near
        mov     cx,1
        call    _seg004_63
        mov     si,bp
        mov     dx,seg _cmpbuf1_start
        mov     es,dx
        mov     di,0
        lodsw
        mov     dx,ax
        inc     ax
        shr     ax,1
        mov     cx,ax
L00F1:
        lodsb
        mov     ah,al
        and     al,0F0h
        shr     al,4
        stosb
        mov     al,ah
        and     al,0Fh
        stosb
        loop    L00F1
        mov     cx,dx
        mov     ax,seg _cmpbuf1_start
        mov     ds,ax
        mov     si,0
        mov     ax,seg _cmpbuf1_start
        mov     es,ax
        mov     di,1800h
        call    _seg004_18D
        mov     ax,es
        add     ax,180h
        ret
_exp_4run endp

; seg004_11C  (+11C)
; exp_5run: format 6, 5-bit run-length (UW-Formats: some critter frames). In and out as exp_4run, with a
; 32-entry uncmp_pal (CX = 2 rows). Unpacks the bit stream, most significant bit first, five
; bytes into eight 5-bit words per iteration, (size + 7) / 8 iterations, with rotates rather
; than a bit loop; then decodes as exp_4run does.
_exp_5run proc near
        mov     cx,2
        call    _seg004_63
        mov     si,bp
        mov     dx,seg _cmpbuf1_start
        mov     es,dx
        mov     di,0
        lodsw
        mov     dx,ax
        add     ax,7
        shr     ax,3
        mov     cx,ax
L0137:
        lodsb
        xor     ah,ah
        ror     ax,3
        stosb
        lodsb
        shr     ah,5
        ror     ax,6
        stosb
        shr     ax,3
        xchg    ah,al
        stosb
        lodsb
        shr     ah,7
        ror     ax,4
        stosb
        lodsb
        shr     ah,4
        ror     ax,7
        stosb
        xor     al,al
        rol     ax,5
        stosb
        lodsb
        shr     ah,6
        ror     ax,5
        stosb
        shr     ax,0Bh
        stosb
        loop    L0137
        mov     cx,dx
        mov     ax,seg _cmpbuf1_start
        mov     ds,ax
        mov     si,0
        mov     ax,seg _cmpbuf1_start
        mov     es,ax
        mov     di,1800h
        call    _seg004_18D
        mov     ax,es
        add     ax,180h
        ret
_exp_5run endp

; seg004_18B  (+18B)
; exp_8run: format 2 has no decoder; it returns at once, leaving AX as it was. Its second
; `ret` (L018C) is the end-of-input exit of the record decoder below.
_exp_8run proc near
        ret
L018C:
        ret
_exp_8run endp

; seg004_18D  (+18D)
; The record decoder's entry. In: DS:SI = the unpacked words (one per byte), CX = how many,
; ES:DI = output, BX = 0 (uncmp_pal). Sets BP = SI + CX, the end of the input, and DX = 3
; (the step past a four-word count's last three words), then falls into seg004_194.
_seg004_18D proc near
        mov     bp,cx
        add     bp,si
        mov     dx,3
_seg004_18D endp

; seg004_194  (+194)
; The run-length record decoder: one repeat record then one run record per pass, looping
; until SI reaches BP (exit through L018C). The first word is the repeat count:
;   3 up:   repeat the next word that many times;
;   0:      an extended count (L01F9 path): if the next word is nonzero, count = it << 4 |
;           the word after; if it is zero, the four words after form a 16-bit count
;           (UW-Formats describes this as 0 0 0 w3 w4 w5; the code takes the third zero
;           word as the count's top nibble, which is the same when it is zero);
;   1:      no repeat record this time;
;   2:      (L0213) a count of repeat records follows; each is decoded by a recursive call,
;           and to make the recursive call stop after its repeat record the routine
;           patches its own code: L01A6's first byte becomes C3h (ret) for the duration and
;           is restored to 29h (the first byte of `sub ax,ax`) afterwards.
; Then the run record at L01A6: a count (extended the same way, a zero count reaching L01CE)
; and that many words copied, each through uncmp_pal. In/out registers as seg004_18D;
; DI advances over the output.
_seg004_194 proc near
        cmp     si,bp
        jae     L018C
        sub     ax,ax
        lodsb
        cmp     al,2
        jbe     short L01F9
        mov     cx,ax
        lodsb
        xlat    byte ptr cs:_uncmp_pal[bx]
        rep stosb
L01A6:
        sub     ax,ax
        lodsb
        mov     cx,ax
        jcxz    L01CE
L01AD:
        lodsb
        xlat    byte ptr cs:_uncmp_pal[bx]
        stosb
        loop    L01AD
        jmp     _seg004_194
L01B5:
        mov     cl,4
        lodsb
        shl     ax,cl
        or      al,byte ptr [si]
        shl     ax,cl
        or      al,byte ptr [si+1]
        shl     ax,cl
        or      al,byte ptr [si+2]
        add     si,dx
        mov     cx,ax
        xor     ah,ah
        jmp     L01AD
L01CE:
        lodsb
        test    al,al
        je      L01B5
        shl     ax,4
        mov     cx,ax
        lodsb
        or      cx,ax
        jmp     L01AD
L01DD:
        mov     cl,4
        lodsb
        shl     ax,cl
        or      al,byte ptr [si]
        shl     ax,cl
        or      al,byte ptr [si+1]
        shl     ax,cl
        or      al,byte ptr [si+2]
        add     si,dx
        mov     cx,ax
        lodsb
        xlat    byte ptr cs:_uncmp_pal[bx]
        rep stosb
        jmp     L01A6
L01F9:
        je      short L0213
        test    al,al
        jne     short L0211
        lodsb
        test    al,al
        je      L01DD
        shl     ax,4
        mov     cx,ax
        lodsb
        or      cx,ax
        lodsb
        xlat    byte ptr cs:_uncmp_pal[bx]
        rep stosb
L0211:
        jmp     L01A6
L0213:
        mov     byte ptr cs:L01A6,0C3h
        sub     ax,ax
        lodsb
        mov     cx,ax
        jcxz    L0247
L0220:
        push    cx
        call    _seg004_194
        pop     cx
        loop    L0220
        mov     byte ptr cs:L01A6,29h
        jmp     L01A6
L0230:
        mov     cl,4
        lodsb
        shl     ax,cl
        or      al,byte ptr [si]
        shl     ax,cl
        or      al,byte ptr [si+1]
        shl     ax,cl
        or      al,byte ptr [si+2]
        add     si,dx
        mov     cx,ax
        jmp     L0220
L0247:
        lodsb
        test    al,al
        je      L0230
        shl     ax,4
        mov     cx,ax
        lodsb
        or      cx,ax
        jmp     L0220
_seg004_194 endp

SEG004_TEXT ends

        end
