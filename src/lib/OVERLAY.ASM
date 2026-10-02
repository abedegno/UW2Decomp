; /* target: seg046_37CD */
; /* opts: /ml */
;
; What it does in UW2. Most of UW2's C code is in overlays (the ovrNNN segments): only one
; overlay's code at a time is in a buffer in conventional memory, and a call to a function in
; an overlay goes through a small stub in the resident image. If the overlay is not loaded,
; the stub's int 3Fh enters this manager, which reads the overlay from the EXE (or copies it
; from EMS, when TMPALLOC.C has called _OvrInitEms) into the buffer, possibly throwing out
; others, and patches the stubs. UWEDIT.C sizes the buffer through _ovrbuffer. The game's own
; code never calls the manager except for _OvrInitEms.
;
; name: inferred (map/filenames.tsv): named after the library, OVERLAY.LIB, whose five
; modules it holds.
;
; Borland's VROOMM overlay manager, from OVERLAY.LIB of Turbo C++ 1.01 (XLIB.ZIP on disk 3).
; The segment is the _OVRTEXT_ code of five library modules, linked in library order, and
; each one equals the library object byte for byte outside its fixups:
;
;       +0005  OVRMAN    8D2h bytes   __OVRGROUP, __DGROUP, __OVRINIT, __OVREXIT,
;                                     __InitModules, __ReadOvrDisk, __ISOVERLAYMOD,
;                                     __CHECKSTACK, __CHECKOVERLAY
;       +08D7  OVRSWAP   43Ch bytes   __OvrInitEms, __OvrInitExt, __SwapOverlay
;       +0D13  OVRDATA     2 bytes    (a ret and a retf, the default hooks in _OVRDATA_)
;       +0D15  OVRUSER    5Bh bytes   __OvrPrepare (called through the _INIT_ table)
;       +0D70  OVRDETEC  4D3h bytes   __INITEMS, __INITEMSPAGE, __EXITEMS, __INITEXT,
;                                     __INITEXTMEM, __EXITEXT, __EXTCOPY
;
; Every public name above comes from the library's PUBDEF records, at exactly these
; offsets; the internal routines keep IDA's names. 9A far calls into this segment are
; the calls between those modules (OVRSWAP to OVRDETEC, OVRUSER to OVRMAN) and OVRMAN's
; own far call to seg046_37CD_135, which the library object also holds as a far call.
; The original was several separately assembled modules; here it is one file.
;
; match: NOSMART: with it TASM 2.0 keeps `call far ptr` into the same segment as 9A (backward
; and forward, no push cs) and keeps `lea reg,[direct]` as lea instead of making it
; `mov reg,offset`; both occur here (tested; a backward `jmp near ptr` is still made
; short). .186: __OvrInitExt has `shr ax,4` (C1 E8 04) at +0B4C.
;
; Most routines run with DS (or ES) on _OVRGROUP_ (paragraph 639B, IDA's seg069), the
; overlay manager's data group, not DGROUP. Its variables are given here as equates at
; their link-time offsets in that group, named from the library's PUBDEFs (OVRDATA and
; OVRDAT) and placed by the fixups in these modules: verify.py only understands DGROUP
; data, and those offsets are absolute in the EXE. The original declared them extrn in
; the group's segments. DGROUP data (__psp, __envseg, __OvrSize, __ovrbuffer) are real
; externs. The segment names _OVRGROUP_, _STUB_, _EXEINFO_, _EXTSEG_, _EMSSEG_ and
; _VDISKSEG_ are the library's own group and segment names, declared here as externs only
; to give their segment values.
;
; name: _OvrInitEms is __OvrInitEms (seg042 calls it as _OvrInitEms(0, 0, 0) after printing
; "EMS allocated"); it keeps the symbols.tsv name until that file is changed.

        .186

; match: two encodings TASM's default SMART mode would change: a far call into this segment
; (SMART makes it push cs; call near) and lea of a direct address (SMART makes it mov).
; The library has both, while elsewhere it uses the sign-extended 83 forms that NOSMART
; gives up, so NOSMART is switched on for just these instructions. MASM 5.1 produces all
; three without switches, which suggests Borland assembled the library with it.
FCALL   macro   target
        nosmart
        call    far ptr target
        smart
        endm

LEAD    macro   reg, addr
        nosmart
        lea     reg,addr
        smart
        endm

; _OVRGROUP_ offsets.  The module-local _OVRDATA_ bytes have no public names.
ovrman_data     equ     0               ; OVRMAN's own _OVRDATA_ (6 bytes)
ovrswap_data    equ     10h             ; OVRSWAP's own _OVRDATA_ (1 byte)
__CALLEXTEMSSWAP equ    20h             ; OVRDATA, _OVRDATA_
__CALLEXTEXIT   equ     22h
__CALLEMSEXIT   equ     24h
__OVRHOOK__     equ     26h
__OVRFILENAME   equ     2Ch
ovrdetec_data   equ     0B0h            ; OVRDETEC's own _OVRDATA_ (5 bytes)
__EmsPageFrame  equ     0C0h            ; OVRDAT, _OVRDATA_
__EmsHandle     equ     0C2h
__EmsFirstPage  equ     0C4h
__EmsPages      equ     0C6h
__EmsChecked    equ     0C8h
__EmsVersion    equ     0C9h
__ExtStartAddress equ   0CAh
__ExtEndAddress equ     0CEh
__ExtChecked    equ     0D2h
__XmsControl    equ     0D3h
__XmsHandle     equ     0D7h
__XmsBase       equ     0D9h
__XMSGDT        equ     0DDh            ; __XMSGDT and __EXTGDT are one address
__EXTGDT        equ     0DDh
__OVRTRAP__     equ     110h            ; OVRDATA, _STUB_
__OvrMemorySwap equ     112h
__OvrFileBase   equ     114h
__OvrRetrySize  equ     118h
__OvrMinHeapSize equ    11Ah
__OvrTrapCount  equ     11Ch
__OvrHeapPtr    equ     120h
__OvrCodeList   equ     122h
__OvrHeapOrg    equ     124h
__OvrHeapEnd    equ     126h
__OvrDosHandle  equ     128h
__OvrLoadCount  equ     12Ah
__OvrLoadList   equ     12Ch
__ExtRec        equ     130h            ; OVRDAT, _EXTSEG_
__EmsRec        equ     140h            ; OVRDAT, _EMSSEG_
__SEGTABLE__    equ     1A0h            ; made by the linker
__SEGTABEND__   equ     710h
; _VDISKSEG_ offset
__FakeVdisk     equ     0               ; OVRDAT, _VDISKSEG_

_DATA   segment word public 'DATA'
        extrn   __psp:word
        extrn   __envseg:word
        extrn   __OvrSize:dword
        extrn   __ovrbuffer:word
_DATA   ends
DGROUP  group   _DATA

        extrn   _farmalloc:far
        extrn   _abort:far              ; symbols.tsv has no name for 0E72:022E yet
        extrn   __OverlayHalt:far       ; nor for 0E72:3B27 (OVRHALT, the library's)
        extrn   __EXENAME__:far         ; _EXEINFO_:0570, made by the linker
        extrn   __BootHandler:far       ; OVRDAT, _VDISKSEG_:003F
        extrn   _OVRGROUP_:far
        extrn   _STUB_:far
        extrn   _EXEINFO_:far
        extrn   _EMSSEG_:far
        extrn   _EXTSEG_:far
        extrn   _VDISKSEG_:far

_OVRTEXT_ segment byte public 'CODE'
        assume  cs:_OVRTEXT_, ds:nothing, es:nothing

        public  __OVRGROUP
        public  __DGROUP
        public  __OVRINIT
        public  _seg046_37CD_135
        public  __OVREXIT
        public  _seg046_37CD_1A8
        public  _seg046_37CD_1B4
        public  _seg046_37CD_1F4
        public  _seg046_37CD_251
        public  _ReadCXFileBytes_seg046_277
        public  __InitModules
        public  _seg046_37CD_307
        public  _ReadFromFilePosition_seg046_3D4
        public  _seg046_37CD_40D
        public  _seg046_37CD_452
        public  __ReadOvrDisk
        public  _Interrupt_3F_seg046_37CD_4E0
        public  _seg046_37CD_546
        public  _seg046_37CD_590
        public  _seg046_37CD_60B
        public  _seg046_37CD_623
        public  _seg046_37CD_65E
        public  _seg046_37CD_67F
        public  _seg046_37CD_69D
        public  _seg046_37CD_6D0
        public  _seg046_37CD_721
        public  _seg046_37CD_73F
        public  _seg046_37CD_74B
        public  _seg046_37CD_771
        public  _seg046_37CD_78D
        public  _seg046_37CD_799
        public  __ISOVERLAYMOD
        public  __CHECKSTACK
        public  __CHECKOVERLAY
        public  __OvrInitEms
        public  __OvrInitExt
        public  _seg046_37CD_BBB
        public  _seg046_37CD_C0E
        public  _seg046_37CD_C1A
        public  _seg046_37CD_CC1
        public  _seg046_37CD_CF0
        public  __SwapOverlay
        public  __OvrPrepare
        public  __INITEMS
        public  __INITEMSPAGE
        public  __EXITEMS
        public  __INITEXT
        public  __INITEXTMEM
        public  __EXITEXT
        public  __EXTCOPY

; ---- OVRMAN (+0005)
__OVRGROUP:
        dw      seg _OVRGROUP_
__DGROUP:
        dw      DGROUP
L0009:
        db      'PATH='                 ; looked for in the environment by seg046_37CD_1F4
L000E:
        dw      0

; seg046_37CD_10  (+10)
; __OVRINIT(exe name far, heap start, heap end) (far, pascal-style: retf 8): open the program's
; own EXE (the name passed, else __EXENAME__, else searched for on the PATH), find the overlay
; data that TLINK appended after the load image (the chain of blocks headed "FB" "OV"), record
; its file position and size (__OvrFileBase, __OvrSize), install the int 3Fh trap (_135), set up
; the overlay heap between the two paragraphs given, build the module tables (__InitModules) and
; check the heap holds the largest overlay. Out: AX = 0, or a negative error (-2 file not found,
; -3 read error, -4 not an overlay EXE, -5 heap too small, -1 other).
__OVRINIT proc far
        push    bp
        mov     bp,sp
        sub     sp,14h
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        push    si
        push    di
        cld
        cmp     word ptr ds:[__OVRTRAP__],0
        jne     short L0029
        jmp     L012A
L0029:
        call    _seg046_37CD_1B4
        jae     short L005A
        mov     ax,word ptr [bp+6]
        or      ax,word ptr [bp+8]
        jne     short L0040
        mov     word ptr [bp+6],offset __EXENAME__
        mov     word ptr [bp+8],seg __EXENAME__
L0040:
        call    _seg046_37CD_1A8
        jae     short L005A
        call    _seg046_37CD_1F4
        jae     short L005A
        mov     ax,0FFFEh
        jmp     L012C
L0050:
        mov     ah,3Eh
        int     21h
        mov     ax,0FFFFh
        jmp     L012C
L005A:
        mov     bx,ax
        mov     word ptr ds:[__OvrDosHandle],bx
        mov     cx,14h
        call    _ReadCXFileBytes_seg046_277
        mov     cx,0FFFDh
        jb      L0050
        xor     ax,ax
        xor     dx,dx
        cmp     word ptr [bp-14h],5A4Dh
        je      short L007B
        mov     cx,0FFFCh
        jmp     L0050
L007B:
        mov     ax,word ptr [bp-10h]
        mov     cx,word ptr [bp-12h]
        jcxz    L0084
        dec     ax
L0084:
        mov     dx,200h
        mul     dx
        add     ax,cx
L008B:
        push    dx
        push    ax
        mov     cx,dx
        mov     dx,ax
        mov     ax,4200h
        int     21h
        mov     cx,10h
        call    _ReadCXFileBytes_seg046_277
        pop     ax
        pop     dx
        mov     cx,0FFFDh
        jb      L0050
        add     ax,10h
        adc     dx,0
        cmp     word ptr [bp-14h],4246h
        mov     cx,0FFFCh
        jne     L0050
        cmp     word ptr [bp-12h],564Fh
        je      short L00C2
        add     ax,word ptr [bp-10h]
        adc     dx,word ptr [bp-0Eh]
        jmp     L008B
L00C2:
        mov     word ptr ds:[__OvrFileBase],ax
        mov     word ptr ds:[__OvrFileBase+2],dx
        mov     es,word ptr cs:__DGROUP
        mov     ax,word ptr [bp-10h]
        mov     word ptr es:[__OvrSize],ax
        mov     ax,word ptr [bp-0Eh]
        mov     word ptr es:[__OvrSize+2],ax
        mov     ah,3Eh
        int     21h
        mov     word ptr ds:[__OvrDosHandle],0
        FCALL   _seg046_37CD_135
        mov     ax,word ptr [bp+0Ch]
        inc     ax
        mov     word ptr ds:[__OvrHeapOrg],ax
        mov     word ptr ds:[__OvrHeapPtr],ax
        mov     bx,word ptr [bp+0Ah]
        mov     word ptr ds:[__OvrHeapEnd],bx
        call    __InitModules
        mov     bx,word ptr ds:[__OvrHeapEnd]
        sub     bx,word ptr ds:[__OvrHeapOrg]
        cmp     bx,word ptr ds:[__OvrMinHeapSize]
        jae     short L0113
        mov     cx,0FFFBh
        jmp     L0050
L0113:
        shr     bx,1
        shr     bx,1
        mov     word ptr ds:[__OvrRetrySize],bx
        call    _seg046_37CD_307
        jae     short L012A
        mov     bx,word ptr ds:[__OvrDosHandle]
        mov     cx,0FFFFh
        jmp     L012C
L012A:
        xor     ax,ax
L012C:
        pop     di
        pop     si
        pop     ds
        mov     sp,bp
        pop     bp
        retf    8
__OVRINIT endp

; seg046_37CD_135  (+135)
; _135: swap the int 3Fh vector (the overlay trap, whose number is __OVRTRAP__+1) with the one
; saved in OVRMAN's data, and open or close the EXE (__OvrDosHandle) to match; called once to
; install and once to remove.
_seg046_37CD_135 proc far
        push    bp
        mov     bp,sp
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        mov     al,byte ptr ds:[__OVRTRAP__+1]
        mov     ah,35h
        int     21h
        push    es
        push    bx
        push    ds
        mov     al,byte ptr ds:[__OVRTRAP__+1]
        mov     dx,word ptr ds:[ovrman_data+2]
        mov     ds,word ptr ds:[ovrman_data+4]
        mov     ah,25h
        int     21h
        pop     ds
        pop     word ptr ds:[ovrman_data+2]
        pop     word ptr ds:[ovrman_data+4]
        cmp     word ptr ds:[__OvrDosHandle],0
        je      short L0177
        mov     bx,word ptr ds:[__OvrDosHandle]
        mov     ah,3Eh
        int     21h
        mov     word ptr ds:[__OvrDosHandle],0
        jmp     short L0183
L0177:
        LEAD    dx, ds:[__OVRFILENAME]
        mov     ax,3D00h
        int     21h
        mov     word ptr ds:[__OvrDosHandle],ax
L0183:
        pop     ds
        pop     bp
        retf
_seg046_37CD_135 endp

; seg046_37CD_186  (+186)
; __OVREXIT: remove the trap, and call the EMS and extended-memory exit hooks (__CALLEMSEXIT,
; __CALLEXTEXIT) so their memory is freed.
__OVREXIT proc far
        push    bp
        mov     bp,sp
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        cmp     word ptr ds:[__OvrDosHandle],0
        je      short L019B
        FCALL   _seg046_37CD_135
L019B:
        push    cs
        call    word ptr ds:[__CALLEMSEXIT]
        push    cs
        call    word ptr ds:[__CALLEXTEXIT]
        pop     ds
        pop     bp
        retf
__OVREXIT endp

; seg046_37CD_1A8  (+1A8)
_seg046_37CD_1A8 proc near
        push    ds
        LEAD    di, ds:[__OVRFILENAME]
        push    ds
        pop     es
        call    _seg046_37CD_251
        pop     ds
        ret
_seg046_37CD_1A8 endp

; seg046_37CD_1B4  (+1B4)
_seg046_37CD_1B4 proc near
        mov     ah,30h
        int     21h
        cmp     al,3
        jb      short L01F3
        push    ds
        mov     ax,DGROUP
        mov     ds,ax
        mov     ds,word ptr ds:[__envseg]
        xor     si,si
        cld
L01C9:
        lodsb
        or      al,al
        jne     L01C9
        lodsb
        or      al,al
        jne     L01C9
        lodsw
        LEAD    di, ds:[__OVRFILENAME]
        mov     ax,seg _OVRGROUP_
        mov     es,ax
        mov     bx,di
L01DF:
        lodsb
        stosb
        or      al,al
        je      short L01ED
        cmp     al,5Ch
        jne     L01DF
        mov     bx,di
        jmp     L01DF
L01ED:
        mov     di,bx
        call    _seg046_37CD_251
        pop     ds
L01F3:
        ret
_seg046_37CD_1B4 endp

; seg046_37CD_1F4  (+1F4)
_seg046_37CD_1F4 proc near
        push    ds
        mov     ax,DGROUP
        mov     ds,ax
        mov     ds,word ptr ds:[__envseg]
        xor     si,si
L0200:
        mov     di,offset L0009
        push    cs
        pop     es
        mov     cx,5
        cld
        repe cmpsb
        je      short L021A
        dec     si
L020E:
        lodsb
        or      al,al
        jne     L020E
        cmp     al,byte ptr [si]
        jne     L0200
L0217:
        pop     ds
        stc
        ret
L021A:
        cmp     byte ptr [si],0
        je      L0217
        LEAD    di, ds:[__OVRFILENAME]
        mov     ax,seg _OVRGROUP_
        mov     es,ax
        xor     al,al
L022A:
        mov     ah,al
        lodsb
        or      al,al
        je      short L0238
        cmp     al,3Bh
        je      short L0239
        stosb
        jmp     L022A
L0238:
        dec     si
L0239:
        cmp     ah,3Ah
        je      short L0246
        cmp     ah,5Ch
        je      short L0246
        mov     al,5Ch
        stosb
L0246:
        push    ds
        push    si
        call    _seg046_37CD_251
        pop     si
        pop     ds
        jb      L021A
        pop     ds
        ret
_seg046_37CD_1F4 endp

; seg046_37CD_251  (+251)
_seg046_37CD_251 proc near
        lds     si,dword ptr [bp+6]
        mov     ax,ds
        or      ax,si
        je      short L0268
        mov     cx,0Ch
L025D:
        lodsb
        stosb
        or      al,al
        je      short L0268
        loop    L025D
        sub     al,al
        stosb
L0268:
        LEAD    dx, ds:[__OVRFILENAME]
        mov     ax,seg _OVRGROUP_
        mov     ds,ax
        mov     ax,3D00h
        int     21h
        ret
_seg046_37CD_251 endp

; ReadCXFileBytes_seg046_277  (+277)
_ReadCXFileBytes_seg046_277 proc near
        push    ds
        lea     dx,[bp-14h]
        push    ss
        pop     ds
        mov     ah,3Fh
        int     21h
        pop     ds
        jb      short L0286
        cmp     ax,cx
L0286:
        ret
_ReadCXFileBytes_seg046_277 endp

; seg046_37CD_287  (+287)
__InitModules proc near
        mov     ax,DGROUP
        mov     es,ax
        mov     ax,word ptr es:[__psp]
        add     ax,10h
        mov     word ptr cs:L000E,ax
        mov     ax,seg _STUB_
        mov     es,ax
        xor     bx,bx
        xor     di,di
        LEAD    si, ds:[__SEGTABLE__]
L02A4:
        test    word ptr [si+4],2
        je      short L02B1
        cmp     word ptr [si+2],0
        jne     short L02B4
L02B1:
        jmp     L02F2
L02B4:
        mov     ax,word ptr [si]
        push    es
        mov     word ptr es:[12h],ax
        mov     es,ax
        cmp     byte ptr es:[1Ah],0FFh
        jne     short L02D0
        pop     es
        mov     word ptr es:[12h],0
        jmp     L02F2
L02D0:
        pop     ax
        mov     word ptr es:[18h],offset __ReadOvrDisk
        mov     ax,word ptr ds:[__OvrFileBase]
        mov     dx,word ptr ds:[__OvrFileBase+2]
        add     word ptr es:[4],ax
        adc     word ptr es:[6],dx
        call    _seg046_37CD_799
        cmp     bx,dx
        jae     short L02F2
        xchg    bx,dx
L02F2:
        add     si,8
        cmp     si,__SEGTABEND__
        jae     short L02FD
        jmp     L02A4
L02FD:
        xor     ax,ax
        add     bx,2
        mov     word ptr ds:[__OvrMinHeapSize],bx
        ret
__InitModules endp

; seg046_37CD_307  (+307)
_seg046_37CD_307 proc near
        mov     cx,word ptr ds:[__OvrCodeList]
        mov     word ptr ds:[__OvrLoadList],cx
        mov     bx,cx
        mov     si,word ptr ds:[__OvrHeapOrg]
        mov     di,word ptr ds:[__OvrHeapEnd]
        push    ds
L031A:
        mov     ds,cx
        mov     cx,word ptr ds:[12h]
        jcxz    L0350
        mov     es,cx
        mov     ax,word ptr es:[4]
        mov     dx,word ptr es:[6]
        sub     ax,word ptr ds:[4]
        sbb     dx,word ptr ds:[6]
        mov     cx,10h
        div     cx
        add     ax,si
        cmp     ax,di
        ja      short L0350
        mov     word ptr ds:[10h],si
        mov     si,ax
        mov     cx,es
        mov     word ptr ds:[1Ch],cx
        mov     bx,ds
        jmp     L031A
L0350:
        mov     ds,bx
        mov     word ptr ds:[1Ch],0
        pop     ds
        mov     word ptr ds:[__OvrHeapPtr],si
        mov     ax,word ptr ds:[__OvrHeapOrg]
        sub     si,ax
        jne     short L0367
        jmp     L03C5
L0367:
        mov     cl,4
        rol     si,cl
        mov     di,si
        and     di,0Fh
        and     si,-16
        mov     es,word ptr ds:[__OvrLoadList]
        mov     dx,word ptr es:[4]
        mov     cx,word ptr es:[6]
        call    _ReadFromFilePosition_seg046_3D4
        jb      short L03C6
        mov     ax,word ptr ds:[__OvrLoadList]
        push    ds
L038A:
        mov     es,ax
        mov     cx,word ptr es:[0Ah]
        jcxz    L0396
        call    _seg046_37CD_40D
L0396:
        cmp     word ptr es:[0Ch],0
        je      short L03A1
        call    _seg046_37CD_67F
L03A1:
        mov     ax,word ptr es:[10h]
        dec     ax
        mov     ds,ax
        mov     word ptr ds:[0Eh],es
        push    es
        mov     ax,0FFFFh
        mov     bx,ax
        mov     ds,word ptr cs:__OVRGROUP
        call    dword ptr ds:[__OVRHOOK__]
        pop     es
        mov     ax,word ptr es:[1Ch]
        or      ax,ax
        jne     L038A
        pop     ds
L03C5:
        clc
L03C6:
        ret
        mov     cl,4
        shr     ax,cl
        ror     dx,cl
        and     dx,0F000h
        or      ax,dx
        ret
_seg046_37CD_307 endp

; ReadFromFilePosition_seg046_37CD_3D4  (+3D4)
_ReadFromFilePosition_seg046_3D4 proc near
        push    ax
        mov     bx,word ptr ds:[__OvrDosHandle]
        mov     ax,4200h
        int     21h
        pop     ax
        push    ds
        mov     ds,ax
        jmp     short L03EB
L03E4:
        mov     ax,ds
        add     ax,0FFFh
        mov     ds,ax
L03EB:
        mov     cx,0FFF0h
        or      di,di
        jne     short L03F4
        mov     cx,si
L03F4:
        xor     dx,dx
        mov     ah,3Fh
        int     21h
        jb      short L040B
        cmp     ax,cx
        jb      short L040B
        sub     si,ax
        sbb     di,0
        mov     ax,si
        or      ax,di
        jne     L03E4
L040B:
        pop     ds
        ret
_ReadFromFilePosition_seg046_3D4 endp

; seg046_37CD_40D  (+40D)
_seg046_37CD_40D proc near
        push    ds
        push    es
        mov     ax,word ptr es:[8]
        mov     si,ax
        and     si,0Fh
        shr     ax,1
        shr     ax,1
        shr     ax,1
        shr     ax,1
        mov     dx,word ptr es:[10h]
        add     ax,dx
        mov     ds,ax
        mov     es,dx
        shr     cx,1
        cld
L042E:
        lodsw
        mov     bx,ax
        mov     di,word ptr es:[bx]
        push    ds
        mov     ax,seg _EXEINFO_
        mov     ds,ax
        mov     ax,di
        and     di,-8
        mov     dx,word ptr [di]
        mov     word ptr es:[bx],dx
        test    ax,1
        je      short L044C
        call    _seg046_37CD_452
L044C:
        pop     ds
        loop    L042E
        pop     es
        pop     ds
        ret
_seg046_37CD_40D endp

; seg046_37CD_452  (+452)
_seg046_37CD_452 proc near
        push    cx
        mov     ds,dx
        mov     ah,byte ptr es:[bx-1]
        mov     al,ah
        and     ax,0F807h
        cmp     ah,0B8h
        jne     short L04AE
        mov     cx,ax
        mov     ah,byte ptr es:[bx+2]
        mov     al,ah
        and     ax,0F807h
        cmp     ah,50h
        jne     short L04AE
        cmp     al,cl
        jne     short L04AE
        mov     dx,ax
        mov     ah,byte ptr es:[bx+3]
        mov     al,ah
        and     ax,0F807h
        cmp     cx,ax
        jne     short L04AE
        mov     ah,byte ptr es:[bx+6]
        mov     al,ah
        and     ax,0F807h
        cmp     ax,dx
        jne     short L04AE
        mov     di,20h
        mov     cx,word ptr ds:[0Ch]
        mov     ax,word ptr es:[bx+4]
L049E:
        cmp     ax,word ptr [di+2]
        je      short L04AA
        add     di,5
        loop    L049E
        jmp     short L04B0
L04AA:
        mov     word ptr es:[bx+4],di
L04AE:
        pop     cx
        ret
L04B0:
        pop     cx
        ret
_seg046_37CD_452 endp

; seg046_37CD_4B2  (+4B2)
__ReadOvrDisk proc near
        mov     si,word ptr es:[8]
        xor     di,di
        add     si,word ptr es:[0Ah]
        adc     di,0
        mov     dx,word ptr es:[4]
        mov     cx,word ptr es:[6]
        mov     ax,word ptr es:[10h]
        call    _ReadFromFilePosition_seg046_3D4
        jb      short L04DF
        mov     cx,word ptr es:[0Ah]
        jcxz    L04DE
        call    _seg046_37CD_40D
L04DE:
        clc
L04DF:
        ret
__ReadOvrDisk endp

; Interrupt_3F_seg046_37CD_4E0  (+4E0)
; The int 3Fh handler: every call to a function in an overlay that is not loaded lands here
; through the overlay's stub (the int 3Fh in the stub's entry). It finds the stub from the
; return address, loads the overlay (_590), and returns into the now-patched stub, which jumps
; to the code; __OVRHOOK__ is called afterwards. An odd BP means the stack is not a normal frame
; and the program is halted (__OverlayHalt).
_Interrupt_3F_seg046_37CD_4E0 proc far
        push    bp
        mov     bp,sp
        test    bp,1
        je      short L04EE
        jmp     far ptr __OverlayHalt
L04EE:
        push    ax
        push    bx
        push    cx
        push    dx
        push    si
        push    di
        push    ds
        push    es
        mov     ax,seg _OVRGROUP_
        mov     ds,ax
        sti
        les     bx,dword ptr [bp+2]
        push    word ptr es:[bx]
        sub     word ptr [bp+2],2
        jne     short L050D
        call    _seg046_37CD_590
        jmp     short L0525
L050D:
        add     bp,6
        mov     ax,word ptr [bp]
        xchg    ax,word ptr [bp-6]
        mov     word ptr [bp],ax
        call    _seg046_37CD_590
        mov     ax,word ptr [bp]
        xchg    ax,word ptr [bp-6]
        mov     word ptr [bp],ax
L0525:
        pop     bx
        mov     al,byte ptr es:[1Ah]
        and     al,8
        and     byte ptr es:[1Ah],0F7h
        cbw
        mov     ds,word ptr cs:__OVRGROUP
        call    dword ptr ds:[__OVRHOOK__]
        pop     es
        pop     ds
        pop     di
        pop     si
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        pop     bp
        iret
_Interrupt_3F_seg046_37CD_4E0 endp

; seg046_37CD_546  (+546)
_seg046_37CD_546 proc near
        push    es
        inc     word ptr ds:[__OvrLoadCount]
        call    _seg046_37CD_799
        jmp     short L057E
L0550:
        popf
        push    dx
        jae     short L0557
        call    _seg046_37CD_623
L0557:
        mov     es,word ptr ds:[__OvrLoadList]
        mov     ax,word ptr es:[1Ch]
        mov     word ptr ds:[__OvrLoadList],ax
        cmp     byte ptr es:[1Bh],0
        jne     short L0572
        call    _seg046_37CD_60B
        call    _seg046_37CD_78D
        jmp     short L057D
L0572:
        dec     byte ptr es:[1Bh]
        call    _seg046_37CD_6D0
        call    _seg046_37CD_721
L057D:
        pop     dx
L057E:
        call    _seg046_37CD_771
        pushf
        cmp     dx,ax
        ja      L0550
        popf
        pop     es
        mov     ax,word ptr ds:[__OvrHeapPtr]
        mov     word ptr es:[10h],ax
        ret
_seg046_37CD_546 endp

; seg046_37CD_590  (+590)
_seg046_37CD_590 proc near
        inc     word ptr ds:[__OvrTrapCount]
        cmp     word ptr es:[10h],0
        je      short L05AB
        mov     byte ptr es:[1Bh],1
        or      byte ptr es:[1Ah],4
        jmp     L05C7
L05AB:
        or      byte ptr es:[1Ah],8
        call    _seg046_37CD_546
        push    ds
        dec     ax
        mov     ds,ax
        mov     word ptr ds:[0Eh],es
        pop     ds
        call    word ptr es:[18h]
        jb      short L0606
        call    _seg046_37CD_721
L05C7:
        call    _seg046_37CD_65E
        mov     al,byte ptr es:[1Ah]
        and     al,3
        add     byte ptr es:[1Bh],al
        push    es
        call    _seg046_37CD_771
        mov     es,word ptr ds:[__OvrLoadList]
L05DD:
        mov     cx,word ptr es:[1Ch]
        jcxz    L0604
        cmp     ax,word ptr ds:[__OvrRetrySize]
        jae     short L0604
        push    cx
        push    ax
        cmp     byte ptr es:[1Bh],0
        je      short L05F8
        xor     ax,ax
        jmp     short L05FE
L05F8:
        call    _seg046_37CD_69D
        call    _seg046_37CD_78D
L05FE:
        pop     cx
        pop     es
        add     ax,cx
        jmp     L05DD
L0604:
        pop     es
        ret
L0606:
        jmp     far ptr __OverlayHalt
_seg046_37CD_590 endp

; seg046_37CD_60B  (+60B)
_seg046_37CD_60B proc near
        call    _seg046_37CD_69D
        cmp     word ptr es:[18h],offset __ReadOvrDisk
        jne     short L061B
        call    word ptr ds:[__CALLEXTEMSSWAP]
L061B:
        mov     word ptr es:[10h],0
        ret
_seg046_37CD_60B endp

; seg046_37CD_623  (+623)
_seg046_37CD_623 proc near
        mov     ax,word ptr ds:[__OvrLoadList]
        xor     cx,cx
L0628:
        inc     cx
        push    ax
        mov     es,ax
        mov     ax,word ptr es:[1Ch]
        or      ax,ax
        jne     L0628
        mov     word ptr ds:[__OvrLoadList],ax
        mov     ax,word ptr ds:[__OvrHeapEnd]
        mov     word ptr ds:[__OvrHeapPtr],ax
L063D:
        pop     es
        push    cx
        mov     ax,word ptr ds:[__OvrLoadList]
        mov     word ptr es:[1Ch],ax
        mov     word ptr ds:[__OvrLoadList],es
        call    _seg046_37CD_78D
        sub     word ptr ds:[__OvrHeapPtr],ax
        call    _seg046_37CD_6D0
        pop     cx
        loop    L063D
        mov     ax,word ptr ds:[__OvrHeapOrg]
        mov     word ptr ds:[__OvrHeapPtr],ax
        ret
_seg046_37CD_623 endp

; seg046_37CD_65E  (+65E)
_seg046_37CD_65E proc near
        cmp     word ptr es:[0Ch],0
        jne     short L0667
        ret
L0667:
        cmp     byte ptr es:[20h],0EAh
        je      short L069C
        mov     cx,word ptr es:[2]
        jcxz    _seg046_37CD_67F
        mov     ax,word ptr es:[10h]
        mov     dx,es
        call    _seg046_37CD_73F
_seg046_37CD_65E endp

; seg046_37CD_67F  (+67F)
_seg046_37CD_67F proc near
        mov     bx,word ptr es:[10h]
        mov     cx,word ptr es:[0Ch]
        mov     di,20h
        cld
L068D:
        mov     dx,word ptr es:[di+2]
        mov     al,0EAh
        stosb
        mov     ax,dx
        stosw
        mov     ax,bx
        stosw
        loop    L068D
L069C:
        ret
_seg046_37CD_67F endp

; seg046_37CD_69D  (+69D)
_seg046_37CD_69D proc near
        cmp     byte ptr es:[20h],0CDh
        je      short L06CF
        mov     ax,es
        mov     dx,word ptr es:[10h]
        xor     cx,cx
        call    _seg046_37CD_73F
        mov     word ptr es:[2],cx
        mov     cx,word ptr es:[0Ch]
        mov     di,20h
        cld
L06BF:
        mov     dx,word ptr es:[di+1]
        mov     ax,word ptr ds:[__OVRTRAP__]
        stosw
        mov     ax,dx
        stosw
        xor     al,al
        stosb
        loop    L06BF
L06CF:
        ret
_seg046_37CD_69D endp

; seg046_37CD_6D0  (+6D0)
_seg046_37CD_6D0 proc near
        mov     ax,word ptr ds:[__OvrHeapPtr]
        mov     dx,word ptr es:[10h]
        mov     word ptr es:[10h],ax
        mov     cx,word ptr es:[8]
        inc     cx
        shr     cx,1
        xor     si,si
        cld
        cmp     ax,dx
        jb      short L06F1
        mov     si,cx
        dec     si
        shl     si,1
        std
L06F1:
        mov     di,si
        push    ds
        push    es
        mov     ds,dx
        mov     es,ax
        rep movsw
        cld
        dec     ax
        mov     ds,ax
        pop     es
        mov     word ptr ds:[0Eh],es
        inc     ax
        pop     ds
        cmp     byte ptr es:[20h],0CDh
        je      short L0720
        call    _seg046_37CD_74B
        mov     cx,word ptr es:[0Ch]
        mov     di,23h
        cld
L071A:
        stosw
        add     di,3
        loop    L071A
L0720:
        ret
_seg046_37CD_6D0 endp

; seg046_37CD_721  (+721)
_seg046_37CD_721 proc near
        call    _seg046_37CD_78D
        add     word ptr ds:[__OvrHeapPtr],ax
        push    ds
        mov     ax,seg _STUB_
L072C:
        mov     ds,ax
        mov     ax,word ptr ds:[1Ch]
        or      ax,ax
        jne     L072C
        mov     word ptr ds:[1Ch],es
        mov     word ptr es:[1Ch],ax
        pop     ds
        ret
_seg046_37CD_721 endp

; seg046_37CD_73F  (+73F)
_seg046_37CD_73F proc near
        call    _seg046_37CD_74B
        or      bx,bx
        je      short L074A
        xchg    cx,word ptr ss:[bx+2]
L074A:
        ret
_seg046_37CD_73F endp

; seg046_37CD_74B  (+74B)
_seg046_37CD_74B proc near
        xor     bx,bx
        push    cx
        push    bp
        jmp     short L0755
L0751:
        shl     cx,1
        mov     bp,cx
L0755:
        mov     cx,word ptr [bp]
        shr     cx,1
        je      short L076E
        jb      L0751
        cmp     dx,word ptr [bp+4]
        jne     L0751
        mov     word ptr [bp+4],ax
        or      bx,bx
        jne     L0751
        mov     bx,bp
        jmp     L0751
L076E:
        pop     bp
        pop     cx
        ret
_seg046_37CD_74B endp

; seg046_37CD_771  (+771)
_seg046_37CD_771 proc near
        mov     ax,word ptr ds:[__OvrLoadList]
        or      ax,ax
        je      short L0784
        mov     es,ax
        mov     ax,word ptr es:[10h]
        sub     ax,word ptr ds:[__OvrHeapPtr]
        jae     short L078C
L0784:
        mov     ax,word ptr ds:[__OvrHeapEnd]
        sub     ax,word ptr ds:[__OvrHeapPtr]
        stc
L078C:
        ret
_seg046_37CD_771 endp

; seg046_37CD_78D  (+78D)
_seg046_37CD_78D proc near
        mov     ax,word ptr es:[8]
        add     ax,11h
        mov     cl,4
        shr     ax,cl
        ret
_seg046_37CD_78D endp

; seg046_37CD_799  (+799)
_seg046_37CD_799 proc near
        mov     cl,4
        mov     ax,word ptr es:[8]
        add     ax,11h
        shr     ax,cl
        mov     dx,word ptr es:[0Ah]
        add     dx,0Fh
        shr     dx,cl
        add     dx,ax
        ret
_seg046_37CD_799 endp

; seg046_37CD_7B1  (+7B1)
; __ISOVERLAYMOD: AX = a segment. If it is a block in the overlay heap holding an overlay's
; code, AX = that overlay's stub segment; otherwise AX = 0 and carry set.
__ISOVERLAYMOD proc far
        push    ds
        push    es
        mov     ds,word ptr cs:__OVRGROUP
        cmp     ax,word ptr ds:[__OvrHeapOrg]
        jb      short L07E1
        cmp     ax,word ptr ds:[__OvrHeapEnd]
        jae     short L07E1
        dec     ax
        mov     es,ax
        mov     es,word ptr es:[0Eh]
        inc     ax
        mov     bx,word ptr ds:[__OVRTRAP__]
        cmp     bx,word ptr es:[0]
        jne     short L07E1
        cmp     ax,word ptr es:[10h]
        mov     ax,es
        je      short L07E4
L07E1:
        xor     ax,ax
        stc
L07E4:
        pop     es
        pop     ds
        retf
__ISOVERLAYMOD endp

; seg046_37CD_7E7  (+7E7)
; __CHECKSTACK: walk the chain of BP frames (up to CX) and mark the stubs of the overlays that
; frames on the stack will return into, so the loader can deal with them before it throws code
; out of the buffer (how it deals with them is in the code after L0836; read only this far).
__CHECKSTACK proc far
        push    bp
        mov     bp,sp
        sub     sp,2
        mov     byte ptr [bp-2],0
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        push    si
        push    di
        mov     bx,bp
        mov     ax,word ptr ds:[__OVRTRAP__]
        mov     dx,word ptr ds:[__OvrHeapOrg]
        jmp     short L0808
L0804:
        shl     si,1
        mov     bx,si
L0808:
        cmp     bx,cx
        jae     short L0836
        mov     si,word ptr ss:[bx]
        shr     si,1
        je      short L0836
        jb      L0804
        mov     di,word ptr ss:[bx+4]
        cmp     di,dx
        jae     L0804
        mov     es,di
        mov     di,word ptr ss:[bx+2]
        or      di,di
        jne     L0804
        cmp     word ptr es:[di],ax
        jne     L0804
        mov     byte ptr [bp-2],1
        mov     word ptr es:[di+2],di
        jmp     L0804
L0836:
        cmp     byte ptr [bp-2],1
        je      short L0843
        jmp     L0878
L083F:
        shl     si,1
        mov     bx,si
L0843:
        mov     si,word ptr ss:[bx]
        shr     si,1
        je      short L0878
        jb      L083F
        mov     di,word ptr ss:[bx+4]
        cmp     di,dx
        jae     L083F
        mov     es,di
        mov     di,word ptr ss:[bx+2]
        or      di,di
        je      L083F
        cmp     word ptr es:[0],ax
        jne     L083F
        cmp     word ptr es:[2],0
        jne     L083F
        xchg    di,word ptr es:[2]
        mov     word ptr ss:[bx+2],di
        jmp     L083F
L0878:
        pop     di
        pop     si
        pop     ds
        mov     sp,bp
        pop     bp
        retf
__CHECKSTACK endp

; seg046_37CD_87F  (+87F)
; __CHECKOVERLAY: the loader's check of one overlay against the stack walk above.
__CHECKOVERLAY proc far
        push    bp
        mov     bp,sp
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        mov     es,word ptr [bp+8]
        mov     cx,word ptr ds:[__OVRTRAP__]
        cmp     cx,word ptr es:[0]
        jne     short L08D4
        cmp     word ptr es:[10h],0
        je      short L08A6
        cmp     byte ptr es:[20h],0CDh
        jne     short L08CD
L08A6:
        mov     word ptr [bp+8],0
        push    si
        push    di
        call    _seg046_37CD_590
        pop     di
        pop     si
        mov     bx,word ptr [bp+6]
        push    es
        mov     al,byte ptr es:[1Ah]
        and     al,8
        and     byte ptr es:[1Ah],0F7h
        cbw
        mov     ds,word ptr cs:__OVRGROUP
        call    dword ptr ds:[__OVRHOOK__]
        pop     es
L08CD:
        mov     ax,word ptr es:[10h]
        mov     word ptr [bp+8],ax
L08D4:
        pop     ds
        pop     bp
        retf
__CHECKOVERLAY endp

; ---- OVRSWAP (+08D7)

; _OvrInitEms  (+8D7)
; _OvrInitEms(handle, first page, pages) (Borland's _OvrInitEms): keep overlays in EMS as well
; as in the buffer, so reloading one is a copy from EMS rather than a read from disk. TMPALLOC.C
; calls it as _OvrInitEms(0, 0, 0) (all defaults) after it has set up its own EMS use.
__OvrInitEms proc far
        push    bp
        mov     bp,sp
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        test    byte ptr ds:[ovrswap_data],1
        je      short L08ED
L08E7:
        mov     ax,0FFFFh
        jmp     L09B4
L08ED:
        cmp     word ptr [bp+6],0
        je      short L0904
        mov     cx,word ptr [bp+0Ah]
        jcxz    L08E7
        cmp     cx,word ptr [bp+8]
        jbe     L08E7
        or      byte ptr ds:[__EmsChecked],1
        jmp     short L0921
L0904:
        FCALL   __INITEMS
        or      ax,ax
        je      L08E7
        mov     bx,word ptr [bp+8]
        cmp     bx,ax
        jae     L08E7
        sub     ax,bx
        mov     cx,word ptr [bp+0Ah]
        jcxz    L091F
        cmp     cx,ax
        jb      short L0921
L091F:
        mov     cx,ax
L0921:
        mov     es,word ptr cs:__DGROUP
        mov     bx,4000h
        mov     ax,word ptr es:[__OvrSize]
        mov     dx,word ptr es:[__OvrSize+2]
        add     ax,3FFFh
        adc     dx,0
        div     bx
        cmp     cx,ax
        jbe     short L0940
        mov     cx,ax
L0940:
        cmp     cx,4
        jae     short L0952
        mov     ax,400h
        mul     cx
        cmp     ax,word ptr ds:[__OvrMinHeapSize]
        jae     short L0952
        jmp     L08E7
L0952:
        push    word ptr [bp+6]
        push    word ptr [bp+8]
        mov     word ptr [bp+0Ah],cx
        push    cx
        FCALL   __INITEMSPAGE
        or      ax,ax
        jne     short L09B4
        push    di
        mov     di,__EmsRec
        push    ds
        pop     es
        cld
        mov     ax,4000h
        mul     word ptr [bp+8]
        mov     bx,ax
        stosw
        mov     cx,dx
        mov     ax,dx
        stosw
        mov     ax,4000h
        mul     word ptr [bp+0Ah]
        add     ax,bx
        adc     dx,cx
        stosw
        mov     ax,dx
        stosw
        mov     ax,bx
        stosw
        mov     ax,cx
        stosw
        mov     word ptr ds:[__EmsRec+12],offset L0A37
        mov     word ptr ds:[__EmsRec+14],0
        mov     word ptr ds:[__OvrMemorySwap],1
        or      byte ptr ds:[ovrswap_data],2
        mov     word ptr ds:[__CALLEMSEXIT],offset __EXITEMS
        mov     word ptr ds:[__CALLEXTEMSSWAP],offset __SwapOverlay
        xor     ax,ax
        pop     di
L09B4:
        pop     ds
        pop     bp
        retf
L09B7:
        mov     ax,seg _EMSSEG_
        call    _seg046_37CD_C1A
        jb      short L0A36
        push    es
        push    ax
        push    bx
        push    cx
        push    dx
        mov     ax,1
        call    dword ptr ds:[__OVRHOOK__]
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        pop     es
        mov     ax,bx
        mov     dx,cx
        mov     bx,4000h
        div     bx
        mov     bx,ax
        mov     di,dx
        mov     dx,word ptr ds:[__EmsHandle]
        mov     ah,47h
        int     67h
        shl     ah,1
        jb      short L0A31
        mov     dx,word ptr es:[8]
        xor     si,si
L09F0:
        mov     cx,4000h
        sub     cx,di
        cmp     cx,dx
        jb      short L09FB
        mov     cx,dx
L09FB:
        sub     dx,cx
        push    dx
        mov     dx,word ptr ds:[__EmsHandle]
        mov     ax,4400h
        int     67h
        pop     dx
        shl     ah,1
        jb      short L0A25
        push    ds
        push    es
        mov     ax,word ptr ds:[__EmsPageFrame]
        mov     ds,word ptr es:[10h]
        mov     es,ax
        cld
        rep movsb
        pop     es
        pop     ds
        inc     bx
        xor     di,di
        or      dx,dx
        jne     L09F0
        clc
L0A25:
        pushf
        mov     dx,word ptr ds:[__EmsHandle]
        mov     ah,48h
        int     67h
        popf
        jae     short L0A36
L0A31:
        jmp     far ptr __OverlayHalt
L0A36:
        retn
L0A37:
        mov     dx,word ptr ds:[__EmsHandle]
        mov     ah,47h
        int     67h
        shl     ah,1
        jb      short L0A9B
        mov     ax,word ptr es:[14h]
        mov     dx,word ptr es:[16h]
        mov     bx,4000h
        div     bx
        mov     bx,ax
        mov     si,dx
        mov     dx,word ptr es:[8]
        xor     di,di
L0A5C:
        mov     cx,4000h
        sub     cx,si
        cmp     cx,dx
        jb      short L0A67
        mov     cx,dx
L0A67:
        sub     dx,cx
        push    dx
        mov     dx,word ptr ds:[__EmsHandle]
        mov     ax,4400h
        int     67h
        pop     dx
        shl     ah,1
        jb      short L0A8F
        push    ds
        push    es
        mov     ds,word ptr ds:[__EmsPageFrame]
        mov     es,word ptr es:[10h]
        cld
        rep movsb
        pop     es
        pop     ds
        inc     bx
        xor     si,si
        or      dx,dx
        jne     L0A5C
L0A8F:
        pushf
        mov     dx,word ptr ds:[__EmsHandle]
        mov     ah,48h
        int     67h
        popf
        jae     short L0AA0
L0A9B:
        jmp     far ptr __OverlayHalt
L0AA0:
        retn
__OvrInitEms endp

; seg046_37CD_AA1  (+AA1)
; __OvrInitExt (Borland's _OvrInitExt): the same with extended memory; UW2 does not call it.
__OvrInitExt proc far
        push    bp
        mov     bp,sp
        sub     sp,2
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        push    si
        push    di
        test    byte ptr ds:[ovrswap_data],2
        je      short L0ABB
        xor     ax,ax
        jmp     L0BB4
L0ABB:
        mov     byte ptr [bp-2],0
        FCALL   __INITEXT
        or      ax,dx
        jne     short L0ACE
L0AC8:
        mov     ax,0FFFFh
        jmp     L0BB4
L0ACE:
        mov     bx,word ptr [bp+6]
        mov     cx,word ptr [bp+8]
        cmp     cx,word ptr ds:[__ExtStartAddress+2]
        jb      short L0AE2
        ja      short L0AEA
        cmp     bx,word ptr ds:[__ExtStartAddress]
        jae     short L0AEA
L0AE2:
        mov     cx,word ptr ds:[__ExtStartAddress+2]
        mov     bx,word ptr ds:[__ExtStartAddress]
L0AEA:
        cmp     cx,word ptr ds:[__ExtEndAddress+2]
        ja      L0AC8
        jb      short L0AF8
        cmp     bx,word ptr ds:[__ExtEndAddress]
        ja      L0AC8
L0AF8:
        mov     word ptr [bp+6],bx
        mov     word ptr [bp+8],cx
        mov     ax,word ptr ds:[__ExtEndAddress]
        mov     dx,word ptr ds:[__ExtEndAddress+2]
        sub     ax,bx
        sbb     dx,cx
        mov     si,word ptr [bp+0Ah]
        or      si,word ptr [bp+0Ch]
        je      short L0B24
        cmp     dx,word ptr [bp+0Ch]
        jb      short L0B24
        ja      short L0B1E
        cmp     ax,word ptr ds:[__ExtEndAddress]
        jbe     short L0B24
L0B1E:
        mov     ax,word ptr [bp+0Ah]
        mov     dx,word ptr [bp+0Ch]
L0B24:
        mov     es,word ptr cs:__DGROUP
        cmp     dx,word ptr es:[__OvrSize+2]
        jb      short L0B42
        ja      short L0B39
        cmp     ax,word ptr es:[__OvrSize]
        jbe     short L0B42
L0B39:
        mov     dx,word ptr es:[__OvrSize+2]
        mov     ax,word ptr es:[__OvrSize]
L0B42:
        mov     word ptr [bp+0Ah],ax
        mov     word ptr [bp+0Ch],dx
        or      dx,dx
        jne     short L0B5A
        push    ax
        shr     ax,4
        cmp     ax,word ptr ds:[__OvrMinHeapSize]
        pop     ax
        jae     short L0B5A
        jmp     L0AC8
L0B5A:
        push    cx
        push    bx
        push    dx
        push    ax
        mov     di,__ExtRec
        push    ds
        pop     es
        cld
        push    ax
        mov     ax,bx
        stosw
        mov     ax,cx
        stosw
        pop     ax
        add     ax,bx
        adc     dx,cx
        stosw
        mov     ax,dx
        stosw
        mov     ax,bx
        stosw
        mov     ax,cx
        stosw
        FCALL   __INITEXTMEM
        or      ax,ax
        jne     short L0BB4
        cmp     byte ptr [bp-2],0
        jne     short L0B8F
        inc     byte ptr [bp-2]
        jmp     L0ACE
L0B8F:
        mov     word ptr ds:[__ExtRec+12],offset L0BEA
        mov     word ptr ds:[__ExtRec+14],0
        mov     word ptr ds:[__OvrMemorySwap],1
        or      byte ptr ds:[ovrswap_data],1
        mov     word ptr ds:[__CALLEXTEXIT],offset __EXITEXT
        mov     word ptr ds:[__CALLEXTEMSSWAP],offset __SwapOverlay
        xor     ax,ax
L0BB4:
        pop     di
        pop     si
        pop     ds
        mov     sp,bp
        pop     bp
        retf
__OvrInitExt endp

; seg046_37CD_BBB  (+BBB)
_seg046_37CD_BBB proc near
        mov     ax,seg _EXTSEG_
        call    _seg046_37CD_C1A
        jb      short L0BE9
        push    es
        push    ax
        push    bx
        push    cx
        push    dx
        mov     ax,1
        call    dword ptr ds:[__OVRHOOK__]
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        pop     es
        push    cx
        mov     ax,word ptr es:[10h]
        mov     dx,10h
        mul     dx
        pop     cx
        call    __EXTCOPY
        jae     short L0BE9
        jmp     far ptr __OverlayHalt
L0BE9:
        ret
L0BEA:
        call    _seg046_37CD_C0E
        mov     ax,word ptr es:[10h]
        mov     dx,10h
        mul     dx
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr es:[14h]
        mov     dx,word ptr es:[16h]
        call    __EXTCOPY
        jae     short L0C0D
        jmp     far ptr __OverlayHalt
L0C0D:
        ret
_seg046_37CD_BBB endp

; seg046_37CD_C0E  (+C0E)
_seg046_37CD_C0E proc near
        mov     si,word ptr es:[8]
        inc     si
        and     si,-2
        xor     di,di
        ret
_seg046_37CD_C0E endp

; seg046_37CD_C1A  (+C1A)
_seg046_37CD_C1A proc near
        push    ds
        mov     ds,ax
        push    bp
        push    es
        mov     ax,word ptr ds:[4]
        mov     bx,word ptr ds:[8]
        sub     ax,bx
        push    ax
        mov     dx,word ptr ds:[6]
        mov     cx,word ptr ds:[0Ah]
        sbb     dx,cx
        push    dx
        call    _seg046_37CD_C0E
        sub     ax,si
        sbb     dx,di
        jae     short L0C7D
        pop     di
        pop     si
        mov     bp,word ptr ds:[0Ch]
        call    _seg046_37CD_CC1
        or      ax,ax
        je      short L0C66
        mov     es,ax
        mov     dx,word ptr ds:[0Eh]
        mov     ax,word ptr es:[0Eh]
        mov     word ptr ds:[0Eh],ax
        mov     word ptr es:[0Eh],0
        call    _seg046_37CD_CF0
        mov     word ptr es:[0Eh],dx
L0C66:
        mov     bx,word ptr ds:[0]
        mov     word ptr ds:[8],bx
        mov     cx,word ptr ds:[2]
        mov     word ptr ds:[0Ah],cx
        pop     es
        push    es
        call    _seg046_37CD_C0E
        jmp     short L0C80
L0C7D:
        add     sp,4
L0C80:
        mov     bp,offset __ReadOvrDisk
        call    _seg046_37CD_CC1
        or      ax,ax
        je      short L0C93
        mov     es,ax
        mov     ax,word ptr es:[0Eh]
        mov     word ptr ds:[0Eh],ax
L0C93:
        pop     es
        pop     bp
        add     word ptr ds:[8],si
        adc     word ptr ds:[0Ah],di
        mov     word ptr es:[14h],bx
        mov     word ptr es:[16h],cx
        mov     ax,word ptr ds:[0Ch]
        mov     word ptr es:[18h],ax
        mov     dx,es
        call    _seg046_37CD_CF0
        mov     word ptr es:[0Eh],dx
        mov     es,dx
        mov     word ptr es:[0Eh],ax
        clc
        pop     ds
        ret
_seg046_37CD_C1A endp

; seg046_37CD_CC1  (+CC1)
_seg046_37CD_CC1 proc near
        push    ds
        xor     ax,ax
        mov     es,ax
        jmp     short L0CE5
L0CC8:
        mov     ds,ax
        mov     ax,word ptr ds:[14h]
        sub     ax,bx
        mov     dx,word ptr ds:[16h]
        sbb     dx,cx
        jb      short L0CEC
        sub     ax,si
        sbb     dx,di
        jae     short L0CEC
        mov     word ptr ds:[18h],bp
        mov     ax,ds
        mov     es,ax
L0CE5:
        mov     ax,word ptr ds:[0Eh]
        or      ax,ax
        jne     L0CC8
L0CEC:
        mov     ax,es
        pop     ds
        ret
_seg046_37CD_CC1 endp

; seg046_37CD_CF0  (+CF0)
_seg046_37CD_CF0 proc near
        mov     ax,ds
L0CF2:
        mov     es,ax
        mov     ax,word ptr es:[0Eh]
        or      ax,ax
        jne     L0CF2
        ret
_seg046_37CD_CF0 endp

; seg046_37CD_CFD  (+CFD)
; __SwapOverlay: copy an overlay to or from the cache: EMS (_BBB) when bit 0 of OVRSWAP's flag
; byte is set, extended memory (L09B7) when bit 1 is.
__SwapOverlay proc near
        test    byte ptr ds:[ovrswap_data],1
        je      short L0D08
        call    _seg046_37CD_BBB
        ret
L0D08:
        test    byte ptr ds:[ovrswap_data],2
        je      short L0D12
        call    L09B7
L0D12:
        ret
__SwapOverlay endp

; ---- OVRDATA (+0D13): the default targets of the hooks in its _OVRDATA_, which hold
; 0D13 (__CALLEXTEMSSWAP) and 0D14 (__CALLEXTEXIT, __CALLEMSEXIT) in the EXE
L0D13:
        ret
L0D14:
        retf

; ---- OVRUSER (+0D15)

; seg046_37CD_D15  (+D15)
; __OvrPrepare: run at start-up from the C start-up's _INIT_ table, before main. Builds the
; module tables, allocates the overlay buffer with farmalloc, the larger of _ovrbuffer (UWEDIT.C
; sets 300h paragraphs, 12 KB) and twice the largest overlay, and calls __OVRINIT on it; any
; failure aborts the program.
__OvrPrepare proc far
        push    ds
        push    si
        push    di
        mov     ax,seg _OVRGROUP_
        mov     ds,ax
        call    __InitModules
        mov     bx,word ptr ds:[__OvrMinHeapSize]
        mov     ax,DGROUP
        mov     ds,ax
        mov     ax,seg __ovrbuffer
        mov     es,ax
        cmp     bx,word ptr es:[__ovrbuffer]
        jae     short L0D3C
        mov     bx,word ptr es:[__ovrbuffer]
        jmp     short L0D3E
L0D3C:
        shl     bx,1
L0D3E:
        inc     bx
        push    bx
        mov     ax,10h
        mul     bx
        push    dx
        push    ax
        call    far ptr _farmalloc
        pop     bx
        pop     bx
        pop     bx
        mov     cx,ax
        or      cx,dx
        jcxz    L0D6B
        inc     dx
        add     bx,dx
        push    dx
        push    bx
        xor     ax,ax
        push    ax
        push    ax
        FCALL   __OVRINIT
        or      ax,ax
        jne     short L0D6B
        pop     di
        pop     si
        pop     ds
        retf
L0D6B:
        jmp     far ptr _abort
__OvrPrepare endp

; ---- OVRDETEC (+0D70)
L0D70:
        db      'EMMXXXX0',0            ; the EMS driver's device name

; EMMXXXX_seg046_37CD_D79  (+D79)
__INITEMS proc far
        push    bp
        mov     bp,sp
        sub     sp,102h
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        push    si
        push    di
        test    byte ptr ds:[__EmsChecked],1
        je      short L0D9F
        test    byte ptr ds:[__EmsChecked],2
        jne     short L0D99
        jmp     L0E07
L0D99:
        mov     ax,word ptr ds:[__EmsPages]
        jmp     L0E24
L0D9F:
        push    ds
        push    cs
        pop     ds
        mov     dx,offset L0D70
        mov     ax,3D00h
        int     21h
        pop     ds
        mov     bx,ax
        jb      short L0DD1
        mov     ax,4400h
        int     21h
        jb      short L0DCD
        test    dx,80h
        je      short L0DCD
        mov     ax,4407h
        int     21h
        jb      short L0DCD
        push    ax
        mov     ah,3Eh
        int     21h
        pop     ax
        cmp     al,0FFh
        je      short L0DD4
L0DCD:
        mov     ah,3Eh
        int     21h
L0DD1:
        jmp     L0E1D
L0DD4:
        mov     ax,3567h
        int     21h
        mov     ax,es
        or      ax,bx
        je      L0DD1
        mov     ah,40h
        int     67h
        or      ah,ah
        jne     L0DD1
        mov     byte ptr ds:[__EmsChecked],1
        mov     ah,46h
        int     67h
        or      ah,ah
        jne     L0DD1
        cmp     al,32h
        jb      L0DD1
        mov     byte ptr ds:[__EmsVersion],al
        mov     ah,41h
        int     67h
        or      ah,ah
        jne     L0DD1
        mov     word ptr ds:[__EmsPageFrame],bx
L0E07:
        mov     ah,42h
        int     67h
        or      ah,ah
        jne     short L0E1D
        mov     word ptr ds:[__EmsPages],bx
        mov     word ptr ds:[__EmsHandle],0FFFFh
        mov     ax,bx
        jmp     short L0E24
L0E1D:
        xor     ax,ax
        mov     byte ptr ds:[__EmsChecked],0
L0E24:
        pop     di
        pop     si
        pop     ds
        mov     sp,bp
        pop     bp
        retf
__INITEMS endp

; seg046_37CD_E2B  (+E2B)
__INITEMSPAGE proc far
        push    bp
        mov     bp,sp
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        test    byte ptr ds:[__EmsChecked],1
        jne     short L0E3E
        jmp     L0E88
L0E3E:
        test    byte ptr ds:[__EmsChecked],2
        je      short L0E4A
        xor     ax,ax
        jmp     L0E8B
L0E4A:
        or      byte ptr ds:[__EmsChecked],2
        mov     ax,word ptr [bp+8]
        mov     word ptr ds:[__EmsFirstPage],ax
        mov     bx,word ptr [bp+6]
        mov     word ptr ds:[__EmsPages],bx
        mov     dx,word ptr [bp+0Ah]
        or      dx,dx
        jne     short L0E70
        mov     ah,43h
        int     67h
        or      ah,ah
        jne     short L0E88
        or      byte ptr ds:[__EmsChecked],4
L0E70:
        mov     word ptr ds:[__EmsHandle],dx
        mov     ah,47h
        int     67h
        or      ah,ah
        jne     short L0E88
        mov     dx,word ptr ds:[__EmsHandle]
        mov     ah,48h
        int     67h
        mov     al,ah
        jmp     short L0E8B
L0E88:
        mov     ax,0FFFFh
L0E8B:
        pop     ds
        pop     bp
        retf    6
__INITEMSPAGE endp

; seg046_37CD_E90  (+E90)
__EXITEMS proc far
        push    bp
        mov     bp,sp
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        test    byte ptr ds:[__EmsChecked],4
        je      short L0EB7
        mov     dx,word ptr ds:[__EmsHandle]
        or      dx,dx
        js      short L0EB7
        mov     ah,45h
        int     67h
        mov     word ptr ds:[__EmsHandle],0FFFFh
        mov     byte ptr ds:[__EmsChecked],0
L0EB7:
        pop     ds
        pop     bp
        retf
__EXITEMS endp

; seg046_37CD_EBA  (+EBA)
__INITEXT proc far
        push    bp
        mov     bp,sp
        sub     sp,22h
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        push    si
        push    di
        test    byte ptr ds:[__ExtChecked],1
        je      short L0ED9
        test    byte ptr ds:[__ExtChecked],2
        je      short L0EDE
        jmp     L104F
L0ED9:
        mov     byte ptr ds:[__ExtChecked],1
L0EDE:
        mov     word ptr ds:[__ExtStartAddress],0
        mov     word ptr ds:[__ExtStartAddress+2],10h
        mov     word ptr ds:[__ExtEndAddress],0
        mov     word ptr ds:[__ExtEndAddress+2],10h
        push    sp
        pop     ax
        cmp     ax,sp
        jne     short L0F04
        mov     ah,30h
        int     21h
        cmp     al,0Ah
        jb      short L0F07
L0F04:
        jmp     L1060
L0F07:
        xchg    al,ah
        mov     word ptr [bp-22h],ax
        mov     ax,0F000h
        mov     es,ax
        mov     al,byte ptr es:[0FFFEh]
        cmp     al,0FCh
        ja      L0F04
        cmp     al,0FBh
        je      L0F04
        cmp     al,0F9h
        je      L0F04
        cmp     word ptr [bp-22h],31h
        jae     short L0F2A
L0F27:
        jmp     L0FA9
L0F2A:
        mov     ax,4300h
        int     2Fh
        cmp     al,80h
        jne     L0F27
        mov     ax,4310h
        int     2Fh
        mov     word ptr ds:[__XmsControl],bx
        mov     word ptr ds:[__XmsControl+2],es
        xor     ax,ax
        call    dword ptr ds:[__XmsControl]
        cmp     ah,2
        jb      short L0FA9
        mov     ah,8
        call    dword ptr ds:[__XmsControl]
        or      ax,ax
        je      short L0FA6
        mov     si,ax
        xchg    dx,ax
        mov     ah,9
        call    dword ptr ds:[__XmsControl]
        or      ax,ax
        je      short L0FA6
        mov     word ptr ds:[__XmsHandle],dx
        mov     ah,0Ch
        call    dword ptr ds:[__XmsControl]
        or      ax,ax
        je      short L0F8C
        mov     word ptr ds:[__ExtStartAddress],bx
        mov     word ptr ds:[__ExtStartAddress+2],dx
        mov     ax,400h
        mul     si
        add     ax,word ptr ds:[__ExtStartAddress]
        adc     dx,word ptr ds:[__ExtStartAddress+2]
        mov     word ptr ds:[__ExtEndAddress],ax
        mov     word ptr ds:[__ExtEndAddress+2],dx
L0F8C:
        mov     ah,0Dh
        mov     dx,word ptr ds:[__XmsHandle]
        call    dword ptr ds:[__XmsControl]
        mov     ah,0Ah
        mov     dx,word ptr ds:[__XmsHandle]
        call    dword ptr ds:[__XmsControl]
        mov     word ptr ds:[__XmsHandle],0
L0FA6:
        jmp     L104F
L0FA9:
        mov     ah,88h
        int     15h
        or      ax,ax
        jne     short L0FB4
L0FB1:
        jmp     L1060
L0FB4:
        mov     dx,400h
        mul     dx
        add     dl,10h
        mov     word ptr ds:[__ExtEndAddress],ax
        mov     word ptr ds:[__ExtEndAddress+2],dx
        mov     cx,word ptr ds:[__XmsControl]
        or      cx,word ptr ds:[__XmsControl+2]
        jcxz    L0FD8
        mov     byte ptr ds:[__ExtStartAddress+2],11h
        mov     word ptr ds:[__ExtStartAddress],0
L0FD8:
        mov     ax,ss
        mov     dx,10h
        mul     dx
        lea     bx,[bp-20h]
        xor     cx,cx
        add     bx,ax
        adc     cx,dx
        mov     ax,0
        mov     dx,10h
        mov     si,20h
        xor     di,di
        call    __EXTCOPY
        jb      L0FB1
        lea     di,[bp-1Dh]
        push    ss
        pop     es
        LEAD    si, ds:[ovrdetec_data]
        mov     cx,5
        cld
        repe cmpsb
        jne     short L1019
        mov     ax,word ptr [bp-15h]
        mul     word ptr [bp-0Dh]
        add     dl,10h
        mov     word ptr ds:[__ExtStartAddress],ax
        mov     byte ptr ds:[__ExtStartAddress+2],dl
L1019:
        xor     ax,ax
        mov     es,ax
        mov     es,word ptr es:[66h]
        mov     di,12h
        LEAD    si, ds:[ovrdetec_data]
        mov     cx,5
        cld
        repe cmpsb
        jne     short L104F
        mov     ax,word ptr es:[2Ch]
        mov     dl,byte ptr es:[2Eh]
        cmp     dl,byte ptr ds:[__ExtStartAddress+2]
        jb      short L104F
        ja      short L1048
        cmp     ax,word ptr ds:[__ExtStartAddress]
        jbe     short L104F
L1048:
        mov     word ptr ds:[__ExtStartAddress],ax
        mov     byte ptr ds:[__ExtStartAddress+2],dl
L104F:
        mov     ax,word ptr ds:[__ExtEndAddress]
        mov     dx,word ptr ds:[__ExtEndAddress+2]
        sub     ax,word ptr ds:[__ExtStartAddress]
        sbb     dx,word ptr ds:[__ExtStartAddress+2]
        jmp     short L1063
L1060:
        xor     ax,ax
        cwd
L1063:
        pop     di
        pop     si
        pop     ds
        mov     sp,bp
        pop     bp
        retf
__INITEXT endp

; seg046_37CD_106A  (+106A)
__INITEXTMEM proc far
        push    bp
        mov     bp,sp
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        push    si
        push    di
        test    byte ptr ds:[__ExtChecked],1
        jne     short L107F
        jmp     L1138
L107F:
        test    byte ptr ds:[__ExtChecked],2
        je      short L108B
        xor     ax,ax
        jmp     L113B
L108B:
        or      byte ptr ds:[__ExtChecked],2
        mov     ax,word ptr [bp+0Ah]
        mov     dx,word ptr [bp+0Ch]
        mov     si,word ptr [bp+6]
        mov     di,word ptr [bp+8]
        mov     bx,word ptr ds:[__XmsControl]
        or      bx,word ptr ds:[__XmsControl+2]
        je      short L10E9
        mov     ax,si
        mov     dx,di
        add     ax,3FFh
        adc     dx,0
        mov     bx,400h
        div     bx
        xchg    dx,ax
        mov     ah,9
        call    dword ptr ds:[__XmsControl]
        or      ax,ax
        je      short L10E6
        mov     word ptr ds:[__XmsHandle],dx
        mov     ah,0Ch
        call    dword ptr ds:[__XmsControl]
        or      ax,ax
        je      short L10E6
        mov     word ptr ds:[__ExtStartAddress],bx
        mov     word ptr ds:[__ExtStartAddress+2],dx
        mov     word ptr ds:[__XmsBase],bx
        mov     word ptr ds:[__XmsBase+2],dx
        mov     ax,bx
        add     ax,si
        adc     dx,di
        jmp     short L112D
L10E6:
        jmp     L1138
L10E9:
        mov     word ptr ds:[__ExtStartAddress],ax
        mov     word ptr ds:[__ExtStartAddress+2],dx
        xor     bx,bx
        mov     es,bx
        push    ds
        mov     bx,seg _VDISKSEG_
        mov     ds,bx
        mov     bx,word ptr es:[64h]
        mov     word ptr ds:[__FakeVdisk+2Fh],bx
        mov     bx,word ptr es:[66h]
        mov     word ptr ds:[__FakeVdisk+31h],bx
        mov     word ptr es:[64h],offset __BootHandler
        mov     word ptr es:[66h],seg __BootHandler
        mov     word ptr ds:[__FakeVdisk+3Ah],ax
        mov     byte ptr ds:[__FakeVdisk+3Ch],dl
        add     ax,si
        adc     dx,di
        mov     word ptr ds:[__FakeVdisk+2Ch],ax
        mov     byte ptr ds:[__FakeVdisk+2Eh],dl
        pop     ds
L112D:
        mov     word ptr ds:[__ExtEndAddress],ax
        mov     word ptr ds:[__ExtEndAddress+2],dx
        xor     ax,ax
        jmp     short L113B
L1138:
        mov     ax,0FFFFh
L113B:
        pop     di
        pop     si
        pop     ds
        pop     bp
        retf    8
__INITEXTMEM endp

; seg046_37CD_1142  (+1142)
__EXITEXT proc far
        push    bp
        mov     bp,sp
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        cmp     byte ptr ds:[__ExtChecked],0
        je      short L1193
        cmp     word ptr ds:[__XmsHandle],0
        je      short L116F
        mov     ah,0Dh
        mov     dx,word ptr ds:[__XmsHandle]
        call    dword ptr ds:[__XmsControl]
        mov     ah,0Ah
        mov     dx,word ptr ds:[__XmsHandle]
        call    dword ptr ds:[__XmsControl]
        jmp     short L1193
L116F:
        xor     ax,ax
        mov     es,ax
        mov     ax,word ptr es:[66h]
        cmp     ax,seg _VDISKSEG_
        jne     short L1193
        push    ds
        mov     ds,ax
        mov     ax,word ptr ds:[__FakeVdisk+2Fh]
        mov     word ptr es:[64h],ax
        mov     ax,word ptr ds:[__FakeVdisk+31h]
        mov     word ptr es:[66h],ax
        pop     ds
        mov     byte ptr ds:[__ExtChecked],0
L1193:
        pop     ds
        pop     bp
        retf
__EXITEXT endp

; seg046_37CD_1196  (+1196)
__EXTCOPY proc near
        push    ds
        push    es
        add     si,1
        adc     di,0
        and     si,-2
        cld
        mov     ds,word ptr cs:__OVRGROUP
        cmp     word ptr ds:[__XmsHandle],0
        je      short L1218
        mov     word ptr ds:[__XMSGDT],si
        mov     word ptr ds:[__XMSGDT+2],di
        cmp     dl,10h
        jb      short L11CE
        mov     si,word ptr ds:[__XmsHandle]
        mov     word ptr ds:[__XMSGDT+4],si
        sub     ax,word ptr ds:[__XmsBase]
        sbb     dx,word ptr ds:[__XmsBase+2]
        jmp     L11D7
L11CE:
        mov     word ptr ds:[__XMSGDT+4],0
        ror     dx,4
L11D7:
        mov     word ptr ds:[__XMSGDT+6],ax
        mov     word ptr ds:[__XMSGDT+8],dx
        cmp     cl,10h
        jb      short L11F6
        mov     si,word ptr ds:[__XmsHandle]
        mov     word ptr ds:[__XMSGDT+10],si
        sub     bx,word ptr ds:[__XmsBase]
        sbb     cx,word ptr ds:[__XmsBase+2]
        jmp     L11FF
L11F6:
        mov     word ptr ds:[__XMSGDT+10],0
        ror     cx,4
L11FF:
        mov     word ptr ds:[__XMSGDT+12],bx
        mov     word ptr ds:[__XMSGDT+14],cx
        mov     ah,0Bh
        LEAD    si, ds:[__XMSGDT]
        call    dword ptr ds:[__XmsControl]
        or      ax,ax
        jne     short L123E
        stc
        jmp     short L1239
L1218:
        shr     di,1
        rcr     si,1
        mov     word ptr ds:[__EXTGDT+18],ax
        mov     byte ptr ds:[__EXTGDT+20],dl
        mov     word ptr ds:[__EXTGDT+26],bx
        mov     byte ptr ds:[__EXTGDT+28],cl
        mov     cx,si
        LEAD    si, ds:[__EXTGDT]
        push    ds
        pop     es
        mov     ah,87h
        int     15h
        jae     short L123E
L1239:
        sbb     ax,ax
        stc
        jmp     short L1240
L123E:
        xor     ax,ax
L1240:
        pop     es
        pop     ds
        ret
__EXTCOPY endp

_OVRTEXT_ ends
        end
