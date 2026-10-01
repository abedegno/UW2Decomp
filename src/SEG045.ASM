; /* target: seg045_379C */
; /* opts: /ml */
;
; The label table of the conversation (babl) bytecode assembler: grdb_blank clears it,
; Ref emits a reference to a label, gr_putlab places one and patches the references
; already emitted, gr_getpre/gr_getlab/gr_freelab hand out label numbers.
;
; This segment was COMPILED C, not hand-written assembly: Turbo C idioms throughout
; (`push 0FFECh; call _exit; inc sp; inc sp`, long division through F_LDIV@ for a far
; pointer difference, short jumps wherever they reach, never a nop), and above all its
; nine uninitialised globals sit in _BSS in exactly the order Turbo C's name hash gives
; (tools/bssorder.py: lstrel, prelblnum, lstpos, freelblptr, refs, dbptr, lblrefs, loc,
; freelbls), which no assembler would produce. It is kept here as assembly that
; reproduces the compiler's output byte for byte; a C version would be the truer source.
;
; Names are the FM Towns ones (tools/fmt.py): every function, and the globals, matched
; by what the FM code does with them (grdb_blank stores _cDbase into _dbptr, gr_entry
; writes _dbptr through _cDbbase, gr_tostrt loads _dbptr from _cEntryStrt, and so on).

        .model  medium
        .186

        extrn   F_LDIV@:far
        extrn   _exit:far

        .data
        extrn   _cDbase:dword                   ; DS:216C, start of the bytecode buffer
        extrn   _cEntryStrt:dword               ; DS:2170
        extrn   _cDbbase:dword                  ; DS:2180, where gr_entry records _dbptr

; This file's uninitialised data, in Turbo C's _BSS order (DS:86F8 to DS:8C84).
        .data?
        public  _lstrel, _prelblnum, _lstpos, _freelblptr, _refs
        public  _dbptr, _lblrefs, _loc, _freelbls
_lstrel         dw      ?                       ; DS:86F8
_prelblnum      db      ?                       ; DS:86FA
                db      ?                       ; padding: the next int is word-aligned
_lstpos         dw      ?                       ; DS:86FC
_freelblptr     db      ?                       ; DS:86FE
_refs           db      20h dup (?)             ; DS:86FF, references per label
                db      ?                       ; padding: word-aligned again
_dbptr          dd      ?                       ; DS:8720, output pointer
_lblrefs        dw      200h dup (?)            ; DS:8724, [32][16] reference positions
_loc            dw      0A0h dup (?)            ; DS:8B24, label positions
_freelbls       db      20h dup (?)             ; DS:8C64, free label stack

        .code

        public  _grdb_blank
        public  _grdb_size
        public  _Clk
        public  _gr_entry
        public  _gr_tostrt
        public  _Ref
        public  _gr_getpre
        public  _gr_getlab
        public  _gr_putlab
        public  _gr_freelab

; seg045_379C_6  (+6)
_grdb_blank proc far
        mov     ax,word ptr _cDbase+2
        mov     dx,word ptr _cDbase
        mov     word ptr _dbptr,dx
        mov     word ptr _dbptr+2,ax
        xor     cx,cx
        jmp     short L003B
L0018:
        mov     bx,cx
        shl     bx,1
        mov     word ptr _loc[bx],0FFFFh
        mov     bx,cx
        mov     al,cl
        mov     byte ptr _freelbls[bx],al
        shl     bx,5
        mov     word ptr _lblrefs[bx],0FFFFh
        mov     bx,cx
        mov     byte ptr _refs[bx],0
        inc     cx
L003B:
        cmp     cx,20h
        jl      L0018
        jmp     short L004D
L0042:
        mov     bx,cx
        shl     bx,1
        mov     word ptr _loc[bx],0FFFFh
        inc     cx
L004D:
        cmp     cx,60h
        jl      L0042
        jmp     short L008C
L0054:
        mov     ax,cx
        shl     ax,1
        les     bx,dword ptr _dbptr
        add     bx,ax
        cmp     word ptr es:[bx-164h],0FFFFh
        je      short L0081
        mov     ax,cx
        shl     ax,1
        mov     bx,word ptr _dbptr
        add     bx,ax
        mov     ax,word ptr es:[bx-164h]
        sar     ax,1
        mov     bx,cx
        shl     bx,1
        mov     word ptr _loc[bx],ax
        jmp     short L008B
L0081:
        mov     bx,cx
        shl     bx,1
        mov     word ptr _loc[bx],0FFFFh
L008B:
        inc     cx
L008C:
        cmp     cx,0A0h
        jl      L0054
        mov     byte ptr _freelblptr,1Fh
        mov     byte ptr _prelblnum,20h
        retf
_grdb_blank endp

; seg045_379C_6  (+9D)
_grdb_size proc far
        push    0
        push    2
        mov     ax,word ptr _dbptr
        xor     dx,dx
        sub     ax,word ptr _cDbase
        sbb     dx,0
        push    dx
        push    ax
        call    far ptr F_LDIV@
        shl     ax,1
        retf
_grdb_size endp

; seg045_379C_B7  (+B7)
_Clk proc far
        push    bp
        mov     bp,sp
        mov     ax,word ptr [bp+6]
        shl     ax,1
        push    ax
        mov     ax,word ptr _cDbase
        sub     ax,24h
        pop     dx
        add     ax,dx
        pop     bp
        retf
_Clk endp

; seg045_379C_CB  (+CB)
_gr_entry proc far
        les     bx,dword ptr _cDbbase
        mov     ax,word ptr _dbptr
        mov     word ptr es:[bx],ax
        retf
_gr_entry endp

; seg045_379C_D6  (+D6)
_gr_tostrt proc far
        mov     ax,word ptr _cEntryStrt+2
        mov     dx,word ptr _cEntryStrt
        mov     word ptr _dbptr,dx
        mov     word ptr _dbptr+2,ax
        retf
_gr_tostrt endp

; seg045_379C_E5  (+E5)
_Ref proc far
        push    bp
        mov     bp,sp
        cmp     byte ptr [bp+6],0A0h
        jne     short L011A
        push    0
        push    2
        mov     ax,word ptr _dbptr
        xor     dx,dx
        sub     ax,word ptr _cDbase
        sbb     dx,0
        push    dx
        push    ax
        call    far ptr F_LDIV@
        mov     word ptr _lstpos,ax
        mov     ax,word ptr [bp+8]
        mov     word ptr _lstrel,ax
        les     bx,dword ptr _dbptr
        mov     word ptr es:[bx],0
        jmp     L01D4
L011A:
        mov     al,byte ptr [bp+6]
        mov     ah,0
        shl     ax,1
        mov     bx,ax
        cmp     word ptr _loc[bx],0FFFFh
        jne     short L01A4
        mov     al,byte ptr [bp+6]
        mov     ah,0
        mov     bx,ax
        cmp     byte ptr _refs[bx],10h
        je      short L0144
        cmp     byte ptr [bp+6],20h
        jae     short L0144
        cmp     byte ptr [bp+6],0
        jae     short L014D
L0144:
        push    0FFECh
        call    far ptr _exit
        inc     sp
        inc     sp
L014D:
        push    0
        push    2
        mov     ax,word ptr _dbptr
        xor     dx,dx
        sub     ax,word ptr _cDbase
        sbb     dx,0
        push    dx
        push    ax
        call    far ptr F_LDIV@
        mov     dl,byte ptr [bp+6]
        mov     dh,0
        shl     dx,5
        mov     bl,byte ptr [bp+6]
        mov     bh,0
        mov     bl,byte ptr _refs[bx]
        mov     bh,0
        shl     bx,1
        add     dx,bx
        mov     bx,dx
        mov     word ptr _lblrefs[bx],ax
        mov     al,byte ptr [bp+6]
        mov     ah,0
        mov     bx,ax
        mov     al,byte ptr _refs[bx]
        inc     al
        mov     dl,byte ptr [bp+6]
        mov     dh,0
        mov     bx,dx
        mov     byte ptr _refs[bx],al
        les     bx,dword ptr _dbptr
        mov     word ptr es:[bx],0
        jmp     short L01D4
L01A4:
        push    0
        push    2
        mov     ax,word ptr _dbptr
        xor     dx,dx
        sub     ax,word ptr _cDbase
        sbb     dx,0
        push    dx
        push    ax
        call    far ptr F_LDIV@
        mov     dl,byte ptr [bp+6]
        mov     dh,0
        shl     dx,1
        mov     bx,dx
        mov     dx,word ptr _loc[bx]
        sub     dx,ax
        dec     dx
        shl     dx,1
        les     bx,dword ptr _dbptr
        mov     word ptr es:[bx],dx
L01D4:
        add     word ptr _dbptr,2
        pop     bp
        retf
_Ref endp

; seg045_379C_1DB  (+1DB)
_gr_getpre proc far
        cmp     byte ptr _prelblnum,60h
        jne     short L01EB
        push    0FFECh
        call    far ptr _exit
        inc     sp
        inc     sp
L01EB:
        mov     al,byte ptr _prelblnum
        inc     byte ptr _prelblnum
        retf
_gr_getpre endp

; seg045_379C_1DB  (+1F3)
_gr_getlab proc far
        cmp     byte ptr _freelblptr,0
        jae     short L0203
        push    0FFECh
        call    far ptr _exit
        inc     sp
        inc     sp
L0203:
        mov     al,byte ptr _freelblptr
        dec     byte ptr _freelblptr
        mov     ah,0
        mov     bx,ax
        mov     al,byte ptr _freelbls[bx]
        retf
_gr_getlab endp

; seg045_379C_213  (+213)
_gr_putlab proc far
        push    bp
        mov     bp,sp
        push    si
        cmp     byte ptr [bp+6],0A0h
        jne     short L0250
        push    0
        push    2
        mov     ax,word ptr _dbptr
        xor     dx,dx
        sub     ax,word ptr _cDbase
        sbb     dx,0
        push    dx
        push    ax
        call    far ptr F_LDIV@
        sub     ax,word ptr _lstpos
        sub     ax,word ptr _lstrel
        shl     ax,1
        mov     dx,word ptr _lstpos
        shl     dx,1
        les     bx,dword ptr _cDbase
        add     bx,dx
        mov     word ptr es:[bx],ax
        jmp     L02D3
L0250:
        push    0
        push    2
        mov     ax,word ptr _dbptr
        xor     dx,dx
        sub     ax,word ptr _cDbase
        sbb     dx,0
        push    dx
        push    ax
        call    far ptr F_LDIV@
        mov     dl,byte ptr [bp+6]
        mov     dh,0
        shl     dx,1
        mov     bx,dx
        mov     word ptr _loc[bx],ax
        cmp     byte ptr [bp+6],20h
        jae     short L02D3
        xor     si,si
        jmp     short L02C2
L027E:
        mov     al,byte ptr [bp+6]
        mov     ah,0
        shl     ax,5
        mov     dx,si
        shl     dx,1
        add     ax,dx
        mov     dl,byte ptr [bp+6]
        mov     dh,0
        shl     dx,1
        mov     bx,dx
        mov     dx,word ptr _loc[bx]
        mov     bx,ax
        sub     dx,word ptr _lblrefs[bx]
        dec     dx
        shl     dx,1
        mov     al,byte ptr [bp+6]
        mov     ah,0
        shl     ax,5
        mov     bx,si
        shl     bx,1
        add     ax,bx
        mov     bx,ax
        mov     ax,word ptr _lblrefs[bx]
        shl     ax,1
        les     bx,dword ptr _cDbase
        add     bx,ax
        mov     word ptr es:[bx],dx
        inc     si
L02C2:
        mov     al,byte ptr [bp+6]
        mov     ah,0
        mov     bx,ax
        mov     al,byte ptr _refs[bx]
        mov     ah,0
        cmp     ax,si
        jg      L027E
L02D3:
        pop     si
        pop     bp
        retf
_gr_putlab endp

; seg045_379C_2D6  (+2D6)
_gr_freelab proc far
        push    bp
        mov     bp,sp
        mov     cl,byte ptr [bp+6]
        mov     al,cl
        mov     ah,0
        shl     ax,1
        mov     bx,ax
        mov     word ptr _loc[bx],0FFFFh
        cmp     cl,20h
        jae     short L0313
        mov     al,cl
        mov     ah,0
        mov     bx,ax
        mov     byte ptr _refs[bx],0
        cmp     byte ptr _freelblptr,5Fh
        jae     short L0313
        mov     al,byte ptr _freelblptr
        inc     al
        mov     byte ptr _freelblptr,al
        mov     ah,0
        mov     dl,cl
        mov     bx,ax
        mov     byte ptr _freelbls[bx],dl
L0313:
        pop     bp
        retf
_gr_freelab endp

        end
