; /* target: seg046 */
; /* opts: /ml */
;
; UW1: DOS segment seg046, the same five library modules as UW2's seg046_37CD, starting 7
; bytes later in the segment (at +0Ch; UW2 +5h), so every label and offset here is UW2's
; plus 7. The one difference in the bytes is the end of the overlay segment table,
; __SEGTABEND__ (6A8h, UW2 710h: UW1's table has 13 fewer entries); the linker makes that
; table, so the library code is the same. The bytes after +124Ah are padding and then
; seg047 (data), which the original target table took in.
;
; What it does in UW2 (and UW1). Most of UW2's C code is in overlays (the ovrNNN segments): only one
; overlay's code at a time is in a buffer in conventional memory, and a call to a function in
; an overlay goes through a small stub in the resident image. If the overlay is not loaded,
; the stub's int 3Fh enters this manager, which reads the overlay from the EXE (or copies it
; from EMS, when TMPALLOC.C has called _OvrInitEms) into the buffer, possibly throwing out
; others, and patches the stubs. UWEDIT.C sizes the buffer through _ovrbuffer. The game's own
; code never calls the manager except for _OvrInitEms.
;
; name: inferred (UW2Decomp's map/filenames.tsv): named after the library, OVERLAY.LIB, whose five
; modules it holds.
;
; Borland's VROOMM overlay manager, from OVERLAY.LIB of Turbo C++ 1.01 (XLIB.ZIP on disk 3).
; The segment is the _OVRTEXT_ code of five library modules, linked in library order, and
; each one equals the library object byte for byte outside its fixups:
;
;       +000C  OVRMAN    8D2h bytes   __OVRGROUP, __DGROUP, __OVRINIT, __OVREXIT,
;                                     __InitModules, __ReadOvrDisk, __ISOVERLAYMOD,
;                                     __CHECKSTACK, __CHECKOVERLAY
;       +08DE  OVRSWAP   43Ch bytes   __OvrInitEms, __OvrInitExt, __SwapOverlay
;       +0D1A  OVRDATA     2 bytes    (a ret and a retf, the default hooks in _OVRDATA_)
;       +0D1C  OVRUSER    5Bh bytes   __OvrPrepare (called through the _INIT_ table)
;       +0D77  OVRDETEC  4D3h bytes   __INITEMS, __INITEMSPAGE, __EXITEMS, __INITEXT,
;                                     __INITEXTMEM, __EXITEXT, __EXTCOPY
;
; Every public name above comes from the library's PUBDEF records, at exactly these
; offsets; the internal routines keep IDA's names. 9A far calls into this segment are
; the calls between those modules (OVRSWAP to OVRDETEC, OVRUSER to OVRMAN) and OVRMAN's
; own far call to seg046_13C, which the library object also holds as a far call.
; The original was several separately assembled modules; here it is one file.
;
; match: NOSMART: with it TASM 2.0 keeps `call far ptr` into the same segment as 9A (backward
; and forward, no push cs) and keeps `lea reg,[direct]` as lea instead of making it
; `mov reg,offset`; both occur here (tested; a backward `jmp near ptr` is still made
; short). .186: __OvrInitExt has `shr ax,4` (C1 E8 04) at +0B53.
;
; Most routines run with DS (or ES) on _OVRGROUP_ (paragraph 58E2 in UW1, 639B in UW2), the
; overlay manager's data group, not DGROUP. Its variables are given here as equates at
; their link-time offsets in that group, named from the library's PUBDEFs (OVRDATA and
; OVRDAT) and placed by the fixups in these modules: verify.py only understands DGROUP
; data, and those offsets are absolute in the EXE. The original declared them extrn in
; the group's segments. DGROUP data (__psp, __envseg, __OvrSize, __ovrbuffer) are real
; externs. The segment names _OVRGROUP_, _STUB_, _EXEINFO_, _EXTSEG_, _EMSSEG_ and
; _VDISKSEG_ are the library's own group and segment names, declared here as externs only
; to give their segment values.
;
; name: _OvrInitEms is __OvrInitEms (TMPALLOC.C calls it; in UW2, seg042 calls it as _OvrInitEms(0, 0, 0) after printing
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
__SEGTABEND__   equ     6A8h            ; UW1: 6A8h (UW2 710h), the table of 8-byte entries is 13 shorter
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
        extrn   _abort:far              ; symbols.tsv has no name for 0EC5:022E yet
        extrn   __OverlayHalt:far       ; nor for 0EC5:3C37 (OVRHALT, the library's)
        extrn   __EXENAME__:far         ; _EXEINFO_:0508 (UW2 0570), made by the linker
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
        public  _seg046_13C
        public  __OVREXIT
        public  _seg046_1AF
        public  _seg046_1BB
        public  _seg046_1FB
        public  _seg046_258
        public  _seg046_27E
        public  __InitModules
        public  _seg046_30E
        public  _ReadFromFilePosition_seg046_3D4
        public  _seg046_414
        public  _seg046_459
        public  __ReadOvrDisk
        public  _Interrupt_3F_seg046_4E7
        public  _seg046_54D
        public  _seg046_597
        public  _seg046_612
        public  _seg046_62A
        public  _seg046_665
        public  _seg046_686
        public  _seg046_6A4
        public  _seg046_6D7
        public  _seg046_728
        public  _seg046_746
        public  _seg046_752
        public  _seg046_778
        public  _seg046_794
        public  _seg046_7A0
        public  __ISOVERLAYMOD
        public  __CHECKSTACK
        public  __CHECKOVERLAY
        public  __OvrInitEms
        public  __OvrInitExt
        public  _seg046_BC2
        public  _seg046_C15
        public  _seg046_C21
        public  _seg046_CC8
        public  _seg046_CF7
        public  __SwapOverlay
        public  __OvrPrepare
        public  __INITEMS
        public  __INITEMSPAGE
        public  __EXITEMS
        public  __INITEXT
        public  __INITEXTMEM
        public  __EXITEXT
        public  __EXTCOPY

; ---- OVRMAN (+0013)
__OVRGROUP:
        dw      seg _OVRGROUP_
__DGROUP:
        dw      DGROUP
L0010:
        db      'PATH='                 ; looked for in the environment by seg046_1FB
L0015:
        dw      0

; seg046_17  (+17)
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
        jne     short L0030
        jmp     L0131
L0030:
        call    _seg046_1BB
        jae     short L0061
        mov     ax,word ptr [bp+6]
        or      ax,word ptr [bp+8]
        jne     short L0047
        mov     word ptr [bp+6],offset __EXENAME__
        mov     word ptr [bp+8],seg __EXENAME__
L0047:
        call    _seg046_1AF
        jae     short L0061
        call    _seg046_1FB
        jae     short L0061
        mov     ax,0FFFEh
        jmp     L0133
L0057:
        mov     ah,3Eh
        int     21h
        mov     ax,0FFFFh
        jmp     L0133
L0061:
        mov     bx,ax
        mov     word ptr ds:[__OvrDosHandle],bx
        mov     cx,14h
        call    _seg046_27E
        mov     cx,0FFFDh
        jb      L0057
        xor     ax,ax
        xor     dx,dx
        cmp     word ptr [bp-14h],5A4Dh
        je      short L0082
        mov     cx,0FFFCh
        jmp     L0057
L0082:
        mov     ax,word ptr [bp-10h]
        mov     cx,word ptr [bp-12h]
        jcxz    L008B
        dec     ax
L008B:
        mov     dx,200h
        mul     dx
        add     ax,cx
L0092:
        push    dx
        push    ax
        mov     cx,dx
        mov     dx,ax
        mov     ax,4200h
        int     21h
        mov     cx,10h
        call    _seg046_27E
        pop     ax
        pop     dx
        mov     cx,0FFFDh
        jb      L0057
        add     ax,10h
        adc     dx,0
        cmp     word ptr [bp-14h],4246h
        mov     cx,0FFFCh
        jne     L0057
        cmp     word ptr [bp-12h],564Fh
        je      short L00C9
        add     ax,word ptr [bp-10h]
        adc     dx,word ptr [bp-0Eh]
        jmp     L0092
L00C9:
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
        FCALL   _seg046_13C
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
        jae     short L011A
        mov     cx,0FFFBh
        jmp     L0057
L011A:
        shr     bx,1
        shr     bx,1
        mov     word ptr ds:[__OvrRetrySize],bx
        call    _seg046_30E
        jae     short L0131
        mov     bx,word ptr ds:[__OvrDosHandle]
        mov     cx,0FFFFh
        jmp     L0133
L0131:
        xor     ax,ax
L0133:
        pop     di
        pop     si
        pop     ds
        mov     sp,bp
        pop     bp
        retf    8
__OVRINIT endp

; seg046_13C  (+13C)
; _135: swap the int 3Fh vector (the overlay trap, whose number is __OVRTRAP__+1) with the one
; saved in OVRMAN's data, and open or close the EXE (__OvrDosHandle) to match; called once to
; install and once to remove.
_seg046_13C proc far
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
        je      short L017E
        mov     bx,word ptr ds:[__OvrDosHandle]
        mov     ah,3Eh
        int     21h
        mov     word ptr ds:[__OvrDosHandle],0
        jmp     short L018A
L017E:
        LEAD    dx, ds:[__OVRFILENAME]
        mov     ax,3D00h
        int     21h
        mov     word ptr ds:[__OvrDosHandle],ax
L018A:
        pop     ds
        pop     bp
        retf
_seg046_13C endp

; seg046_18D  (+18D)
; __OVREXIT: remove the trap, and call the EMS and extended-memory exit hooks (__CALLEMSEXIT,
; __CALLEXTEXIT) so their memory is freed.
__OVREXIT proc far
        push    bp
        mov     bp,sp
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        cmp     word ptr ds:[__OvrDosHandle],0
        je      short L01A2
        FCALL   _seg046_13C
L01A2:
        push    cs
        call    word ptr ds:[__CALLEMSEXIT]
        push    cs
        call    word ptr ds:[__CALLEXTEXIT]
        pop     ds
        pop     bp
        retf
__OVREXIT endp

; seg046_1AF  (+1AF)
_seg046_1AF proc near
        push    ds
        LEAD    di, ds:[__OVRFILENAME]
        push    ds
        pop     es
        call    _seg046_258
        pop     ds
        ret
_seg046_1AF endp

; seg046_1BB  (+1BB)
_seg046_1BB proc near
        mov     ah,30h
        int     21h
        cmp     al,3
        jb      short L01FA
        push    ds
        mov     ax,DGROUP
        mov     ds,ax
        mov     ds,word ptr ds:[__envseg]
        xor     si,si
        cld
L01D0:
        lodsb
        or      al,al
        jne     L01D0
        lodsb
        or      al,al
        jne     L01D0
        lodsw
        LEAD    di, ds:[__OVRFILENAME]
        mov     ax,seg _OVRGROUP_
        mov     es,ax
        mov     bx,di
L01E6:
        lodsb
        stosb
        or      al,al
        je      short L01F4
        cmp     al,5Ch
        jne     L01E6
        mov     bx,di
        jmp     L01E6
L01F4:
        mov     di,bx
        call    _seg046_258
        pop     ds
L01FA:
        ret
_seg046_1BB endp

; seg046_1FB  (+1FB)
_seg046_1FB proc near
        push    ds
        mov     ax,DGROUP
        mov     ds,ax
        mov     ds,word ptr ds:[__envseg]
        xor     si,si
L0207:
        mov     di,offset L0010
        push    cs
        pop     es
        mov     cx,5
        cld
        repe cmpsb
        je      short L0221
        dec     si
L0215:
        lodsb
        or      al,al
        jne     L0215
        cmp     al,byte ptr [si]
        jne     L0207
L021E:
        pop     ds
        stc
        ret
L0221:
        cmp     byte ptr [si],0
        je      L021E
        LEAD    di, ds:[__OVRFILENAME]
        mov     ax,seg _OVRGROUP_
        mov     es,ax
        xor     al,al
L0231:
        mov     ah,al
        lodsb
        or      al,al
        je      short L023F
        cmp     al,3Bh
        je      short L0240
        stosb
        jmp     L0231
L023F:
        dec     si
L0240:
        cmp     ah,3Ah
        je      short L024D
        cmp     ah,5Ch
        je      short L024D
        mov     al,5Ch
        stosb
L024D:
        push    ds
        push    si
        call    _seg046_258
        pop     si
        pop     ds
        jb      L0221
        pop     ds
        ret
_seg046_1FB endp

; seg046_258  (+258)
_seg046_258 proc near
        lds     si,dword ptr [bp+6]
        mov     ax,ds
        or      ax,si
        je      short L026F
        mov     cx,0Ch
L0264:
        lodsb
        stosb
        or      al,al
        je      short L026F
        loop    L0264
        sub     al,al
        stosb
L026F:
        LEAD    dx, ds:[__OVRFILENAME]
        mov     ax,seg _OVRGROUP_
        mov     ds,ax
        mov     ax,3D00h
        int     21h
        ret
_seg046_258 endp

; seg046_27E  (+27E)
_seg046_27E proc near
        push    ds
        lea     dx,[bp-14h]
        push    ss
        pop     ds
        mov     ah,3Fh
        int     21h
        pop     ds
        jb      short L028D
        cmp     ax,cx
L028D:
        ret
_seg046_27E endp

; seg046_28E  (+28E)
__InitModules proc near
        mov     ax,DGROUP
        mov     es,ax
        mov     ax,word ptr es:[__psp]
        add     ax,10h
        mov     word ptr cs:L0015,ax
        mov     ax,seg _STUB_
        mov     es,ax
        xor     bx,bx
        xor     di,di
        LEAD    si, ds:[__SEGTABLE__]
L02AB:
        test    word ptr [si+4],2
        je      short L02B8
        cmp     word ptr [si+2],0
        jne     short L02BB
L02B8:
        jmp     L02F9
L02BB:
        mov     ax,word ptr [si]
        push    es
        mov     word ptr es:[12h],ax
        mov     es,ax
        cmp     byte ptr es:[1Ah],0FFh
        jne     short L02D7
        pop     es
        mov     word ptr es:[12h],0
        jmp     L02F9
L02D7:
        pop     ax
        mov     word ptr es:[18h],offset __ReadOvrDisk
        mov     ax,word ptr ds:[__OvrFileBase]
        mov     dx,word ptr ds:[__OvrFileBase+2]
        add     word ptr es:[4],ax
        adc     word ptr es:[6],dx
        call    _seg046_7A0
        cmp     bx,dx
        jae     short L02F9
        xchg    bx,dx
L02F9:
        add     si,8
        cmp     si,__SEGTABEND__
        jae     short L0304
        jmp     L02AB
L0304:
        xor     ax,ax
        add     bx,2
        mov     word ptr ds:[__OvrMinHeapSize],bx
        ret
__InitModules endp

; seg046_30E  (+30E)
_seg046_30E proc near
        mov     cx,word ptr ds:[__OvrCodeList]
        mov     word ptr ds:[__OvrLoadList],cx
        mov     bx,cx
        mov     si,word ptr ds:[__OvrHeapOrg]
        mov     di,word ptr ds:[__OvrHeapEnd]
        push    ds
L0321:
        mov     ds,cx
        mov     cx,word ptr ds:[12h]
        jcxz    L0357
        mov     es,cx
        mov     ax,word ptr es:[4]
        mov     dx,word ptr es:[6]
        sub     ax,word ptr ds:[4]
        sbb     dx,word ptr ds:[6]
        mov     cx,10h
        div     cx
        add     ax,si
        cmp     ax,di
        ja      short L0357
        mov     word ptr ds:[10h],si
        mov     si,ax
        mov     cx,es
        mov     word ptr ds:[1Ch],cx
        mov     bx,ds
        jmp     L0321
L0357:
        mov     ds,bx
        mov     word ptr ds:[1Ch],0
        pop     ds
        mov     word ptr ds:[__OvrHeapPtr],si
        mov     ax,word ptr ds:[__OvrHeapOrg]
        sub     si,ax
        jne     short L036E
        jmp     L03CC
L036E:
        mov     cl,4
        rol     si,cl
        mov     di,si
        and     di,0Fh
        and     si,-16
        mov     es,word ptr ds:[__OvrLoadList]
        mov     dx,word ptr es:[4]
        mov     cx,word ptr es:[6]
        call    _ReadFromFilePosition_seg046_3D4
        jb      short L03CD
        mov     ax,word ptr ds:[__OvrLoadList]
        push    ds
L0391:
        mov     es,ax
        mov     cx,word ptr es:[0Ah]
        jcxz    L039D
        call    _seg046_414
L039D:
        cmp     word ptr es:[0Ch],0
        je      short L03A8
        call    _seg046_686
L03A8:
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
        jne     L0391
        pop     ds
L03CC:
        clc
L03CD:
        ret
        mov     cl,4
        shr     ax,cl
        ror     dx,cl
        and     dx,0F000h
        or      ax,dx
        ret
_seg046_30E endp

; ReadFromFilePosition_seg046_3DB  (+3DB)
_ReadFromFilePosition_seg046_3D4 proc near
        push    ax
        mov     bx,word ptr ds:[__OvrDosHandle]
        mov     ax,4200h
        int     21h
        pop     ax
        push    ds
        mov     ds,ax
        jmp     short L03F2
L03EB:
        mov     ax,ds
        add     ax,0FFFh
        mov     ds,ax
L03F2:
        mov     cx,0FFF0h
        or      di,di
        jne     short L03FB
        mov     cx,si
L03FB:
        xor     dx,dx
        mov     ah,3Fh
        int     21h
        jb      short L0412
        cmp     ax,cx
        jb      short L0412
        sub     si,ax
        sbb     di,0
        mov     ax,si
        or      ax,di
        jne     L03EB
L0412:
        pop     ds
        ret
_ReadFromFilePosition_seg046_3D4 endp

; seg046_414  (+414)
_seg046_414 proc near
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
L0435:
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
        je      short L0453
        call    _seg046_459
L0453:
        pop     ds
        loop    L0435
        pop     es
        pop     ds
        ret
_seg046_414 endp

; seg046_459  (+459)
_seg046_459 proc near
        push    cx
        mov     ds,dx
        mov     ah,byte ptr es:[bx-1]
        mov     al,ah
        and     ax,0F807h
        cmp     ah,0B8h
        jne     short L04B5
        mov     cx,ax
        mov     ah,byte ptr es:[bx+2]
        mov     al,ah
        and     ax,0F807h
        cmp     ah,50h
        jne     short L04B5
        cmp     al,cl
        jne     short L04B5
        mov     dx,ax
        mov     ah,byte ptr es:[bx+3]
        mov     al,ah
        and     ax,0F807h
        cmp     cx,ax
        jne     short L04B5
        mov     ah,byte ptr es:[bx+6]
        mov     al,ah
        and     ax,0F807h
        cmp     ax,dx
        jne     short L04B5
        mov     di,20h
        mov     cx,word ptr ds:[0Ch]
        mov     ax,word ptr es:[bx+4]
L04A5:
        cmp     ax,word ptr [di+2]
        je      short L04B1
        add     di,5
        loop    L04A5
        jmp     short L04B7
L04B1:
        mov     word ptr es:[bx+4],di
L04B5:
        pop     cx
        ret
L04B7:
        pop     cx
        ret
_seg046_459 endp

; seg046_4B9  (+4B9)
__ReadOvrDisk proc near
        mov     si,word ptr es:[8]
        xor     di,di
        add     si,word ptr es:[0Ah]
        adc     di,0
        mov     dx,word ptr es:[4]
        mov     cx,word ptr es:[6]
        mov     ax,word ptr es:[10h]
        call    _ReadFromFilePosition_seg046_3D4
        jb      short L04E6
        mov     cx,word ptr es:[0Ah]
        jcxz    L04E5
        call    _seg046_414
L04E5:
        clc
L04E6:
        ret
__ReadOvrDisk endp

; Interrupt_3F_seg046_4E7  (+4E7)
; The int 3Fh handler: every call to a function in an overlay that is not loaded lands here
; through the overlay's stub (the int 3Fh in the stub's entry). It finds the stub from the
; return address, loads the overlay (_590), and returns into the now-patched stub, which jumps
; to the code; __OVRHOOK__ is called afterwards. An odd BP means the stack is not a normal frame
; and the program is halted (__OverlayHalt).
_Interrupt_3F_seg046_4E7 proc far
        push    bp
        mov     bp,sp
        test    bp,1
        je      short L04F5
        jmp     far ptr __OverlayHalt
L04F5:
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
        jne     short L0514
        call    _seg046_597
        jmp     short L052C
L0514:
        add     bp,6
        mov     ax,word ptr [bp]
        xchg    ax,word ptr [bp-6]
        mov     word ptr [bp],ax
        call    _seg046_597
        mov     ax,word ptr [bp]
        xchg    ax,word ptr [bp-6]
        mov     word ptr [bp],ax
L052C:
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
_Interrupt_3F_seg046_4E7 endp

; seg046_54D  (+54D)
_seg046_54D proc near
        push    es
        inc     word ptr ds:[__OvrLoadCount]
        call    _seg046_7A0
        jmp     short L0585
L0557:
        popf
        push    dx
        jae     short L055E
        call    _seg046_62A
L055E:
        mov     es,word ptr ds:[__OvrLoadList]
        mov     ax,word ptr es:[1Ch]
        mov     word ptr ds:[__OvrLoadList],ax
        cmp     byte ptr es:[1Bh],0
        jne     short L0579
        call    _seg046_612
        call    _seg046_794
        jmp     short L0584
L0579:
        dec     byte ptr es:[1Bh]
        call    _seg046_6D7
        call    _seg046_728
L0584:
        pop     dx
L0585:
        call    _seg046_778
        pushf
        cmp     dx,ax
        ja      L0557
        popf
        pop     es
        mov     ax,word ptr ds:[__OvrHeapPtr]
        mov     word ptr es:[10h],ax
        ret
_seg046_54D endp

; seg046_597  (+597)
_seg046_597 proc near
        inc     word ptr ds:[__OvrTrapCount]
        cmp     word ptr es:[10h],0
        je      short L05B2
        mov     byte ptr es:[1Bh],1
        or      byte ptr es:[1Ah],4
        jmp     L05CE
L05B2:
        or      byte ptr es:[1Ah],8
        call    _seg046_54D
        push    ds
        dec     ax
        mov     ds,ax
        mov     word ptr ds:[0Eh],es
        pop     ds
        call    word ptr es:[18h]
        jb      short L060D
        call    _seg046_728
L05CE:
        call    _seg046_665
        mov     al,byte ptr es:[1Ah]
        and     al,3
        add     byte ptr es:[1Bh],al
        push    es
        call    _seg046_778
        mov     es,word ptr ds:[__OvrLoadList]
L05E4:
        mov     cx,word ptr es:[1Ch]
        jcxz    L060B
        cmp     ax,word ptr ds:[__OvrRetrySize]
        jae     short L060B
        push    cx
        push    ax
        cmp     byte ptr es:[1Bh],0
        je      short L05FF
        xor     ax,ax
        jmp     short L0605
L05FF:
        call    _seg046_6A4
        call    _seg046_794
L0605:
        pop     cx
        pop     es
        add     ax,cx
        jmp     L05E4
L060B:
        pop     es
        ret
L060D:
        jmp     far ptr __OverlayHalt
_seg046_597 endp

; seg046_612  (+612)
_seg046_612 proc near
        call    _seg046_6A4
        cmp     word ptr es:[18h],offset __ReadOvrDisk
        jne     short L0622
        call    word ptr ds:[__CALLEXTEMSSWAP]
L0622:
        mov     word ptr es:[10h],0
        ret
_seg046_612 endp

; seg046_62A  (+62A)
_seg046_62A proc near
        mov     ax,word ptr ds:[__OvrLoadList]
        xor     cx,cx
L062F:
        inc     cx
        push    ax
        mov     es,ax
        mov     ax,word ptr es:[1Ch]
        or      ax,ax
        jne     L062F
        mov     word ptr ds:[__OvrLoadList],ax
        mov     ax,word ptr ds:[__OvrHeapEnd]
        mov     word ptr ds:[__OvrHeapPtr],ax
L0644:
        pop     es
        push    cx
        mov     ax,word ptr ds:[__OvrLoadList]
        mov     word ptr es:[1Ch],ax
        mov     word ptr ds:[__OvrLoadList],es
        call    _seg046_794
        sub     word ptr ds:[__OvrHeapPtr],ax
        call    _seg046_6D7
        pop     cx
        loop    L0644
        mov     ax,word ptr ds:[__OvrHeapOrg]
        mov     word ptr ds:[__OvrHeapPtr],ax
        ret
_seg046_62A endp

; seg046_665  (+665)
_seg046_665 proc near
        cmp     word ptr es:[0Ch],0
        jne     short L066E
        ret
L066E:
        cmp     byte ptr es:[20h],0EAh
        je      short L06A3
        mov     cx,word ptr es:[2]
        jcxz    _seg046_686
        mov     ax,word ptr es:[10h]
        mov     dx,es
        call    _seg046_746
_seg046_665 endp

; seg046_686  (+686)
_seg046_686 proc near
        mov     bx,word ptr es:[10h]
        mov     cx,word ptr es:[0Ch]
        mov     di,20h
        cld
L0694:
        mov     dx,word ptr es:[di+2]
        mov     al,0EAh
        stosb
        mov     ax,dx
        stosw
        mov     ax,bx
        stosw
        loop    L0694
L06A3:
        ret
_seg046_686 endp

; seg046_6A4  (+6A4)
_seg046_6A4 proc near
        cmp     byte ptr es:[20h],0CDh
        je      short L06D6
        mov     ax,es
        mov     dx,word ptr es:[10h]
        xor     cx,cx
        call    _seg046_746
        mov     word ptr es:[2],cx
        mov     cx,word ptr es:[0Ch]
        mov     di,20h
        cld
L06C6:
        mov     dx,word ptr es:[di+1]
        mov     ax,word ptr ds:[__OVRTRAP__]
        stosw
        mov     ax,dx
        stosw
        xor     al,al
        stosb
        loop    L06C6
L06D6:
        ret
_seg046_6A4 endp

; seg046_6D7  (+6D7)
_seg046_6D7 proc near
        mov     ax,word ptr ds:[__OvrHeapPtr]
        mov     dx,word ptr es:[10h]
        mov     word ptr es:[10h],ax
        mov     cx,word ptr es:[8]
        inc     cx
        shr     cx,1
        xor     si,si
        cld
        cmp     ax,dx
        jb      short L06F8
        mov     si,cx
        dec     si
        shl     si,1
        std
L06F8:
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
        je      short L0727
        call    _seg046_752
        mov     cx,word ptr es:[0Ch]
        mov     di,23h
        cld
L0721:
        stosw
        add     di,3
        loop    L0721
L0727:
        ret
_seg046_6D7 endp

; seg046_728  (+728)
_seg046_728 proc near
        call    _seg046_794
        add     word ptr ds:[__OvrHeapPtr],ax
        push    ds
        mov     ax,seg _STUB_
L0733:
        mov     ds,ax
        mov     ax,word ptr ds:[1Ch]
        or      ax,ax
        jne     L0733
        mov     word ptr ds:[1Ch],es
        mov     word ptr es:[1Ch],ax
        pop     ds
        ret
_seg046_728 endp

; seg046_746  (+746)
_seg046_746 proc near
        call    _seg046_752
        or      bx,bx
        je      short L0751
        xchg    cx,word ptr ss:[bx+2]
L0751:
        ret
_seg046_746 endp

; seg046_752  (+752)
_seg046_752 proc near
        xor     bx,bx
        push    cx
        push    bp
        jmp     short L075C
L0758:
        shl     cx,1
        mov     bp,cx
L075C:
        mov     cx,word ptr [bp]
        shr     cx,1
        je      short L0775
        jb      L0758
        cmp     dx,word ptr [bp+4]
        jne     L0758
        mov     word ptr [bp+4],ax
        or      bx,bx
        jne     L0758
        mov     bx,bp
        jmp     L0758
L0775:
        pop     bp
        pop     cx
        ret
_seg046_752 endp

; seg046_778  (+778)
_seg046_778 proc near
        mov     ax,word ptr ds:[__OvrLoadList]
        or      ax,ax
        je      short L078B
        mov     es,ax
        mov     ax,word ptr es:[10h]
        sub     ax,word ptr ds:[__OvrHeapPtr]
        jae     short L0793
L078B:
        mov     ax,word ptr ds:[__OvrHeapEnd]
        sub     ax,word ptr ds:[__OvrHeapPtr]
        stc
L0793:
        ret
_seg046_778 endp

; seg046_794  (+794)
_seg046_794 proc near
        mov     ax,word ptr es:[8]
        add     ax,11h
        mov     cl,4
        shr     ax,cl
        ret
_seg046_794 endp

; seg046_7A0  (+7A0)
_seg046_7A0 proc near
        mov     cl,4
        mov     ax,word ptr es:[8]
        add     ax,11h
        shr     ax,cl
        mov     dx,word ptr es:[0Ah]
        add     dx,0Fh
        shr     dx,cl
        add     dx,ax
        ret
_seg046_7A0 endp

; seg046_7B8  (+7B8)
; __ISOVERLAYMOD: AX = a segment. If it is a block in the overlay heap holding an overlay's
; code, AX = that overlay's stub segment; otherwise AX = 0 and carry set.
__ISOVERLAYMOD proc far
        push    ds
        push    es
        mov     ds,word ptr cs:__OVRGROUP
        cmp     ax,word ptr ds:[__OvrHeapOrg]
        jb      short L07E8
        cmp     ax,word ptr ds:[__OvrHeapEnd]
        jae     short L07E8
        dec     ax
        mov     es,ax
        mov     es,word ptr es:[0Eh]
        inc     ax
        mov     bx,word ptr ds:[__OVRTRAP__]
        cmp     bx,word ptr es:[0]
        jne     short L07E8
        cmp     ax,word ptr es:[10h]
        mov     ax,es
        je      short L07EB
L07E8:
        xor     ax,ax
        stc
L07EB:
        pop     es
        pop     ds
        retf
__ISOVERLAYMOD endp

; seg046_7EE  (+7EE)
; __CHECKSTACK: walk the chain of BP frames (up to CX) and mark the stubs of the overlays that
; frames on the stack will return into, so the loader can deal with them before it throws code
; out of the buffer (how it deals with them is in the code after L083D; read only this far).
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
        jmp     short L080F
L080B:
        shl     si,1
        mov     bx,si
L080F:
        cmp     bx,cx
        jae     short L083D
        mov     si,word ptr ss:[bx]
        shr     si,1
        je      short L083D
        jb      L080B
        mov     di,word ptr ss:[bx+4]
        cmp     di,dx
        jae     L080B
        mov     es,di
        mov     di,word ptr ss:[bx+2]
        or      di,di
        jne     L080B
        cmp     word ptr es:[di],ax
        jne     L080B
        mov     byte ptr [bp-2],1
        mov     word ptr es:[di+2],di
        jmp     L080B
L083D:
        cmp     byte ptr [bp-2],1
        je      short L084A
        jmp     L087F
L0846:
        shl     si,1
        mov     bx,si
L084A:
        mov     si,word ptr ss:[bx]
        shr     si,1
        je      short L087F
        jb      L0846
        mov     di,word ptr ss:[bx+4]
        cmp     di,dx
        jae     L0846
        mov     es,di
        mov     di,word ptr ss:[bx+2]
        or      di,di
        je      L0846
        cmp     word ptr es:[0],ax
        jne     L0846
        cmp     word ptr es:[2],0
        jne     L0846
        xchg    di,word ptr es:[2]
        mov     word ptr ss:[bx+2],di
        jmp     L0846
L087F:
        pop     di
        pop     si
        pop     ds
        mov     sp,bp
        pop     bp
        retf
__CHECKSTACK endp

; seg046_886  (+886)
; __CHECKOVERLAY: the loader's check of one overlay against the stack walk above.
__CHECKOVERLAY proc far
        push    bp
        mov     bp,sp
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        mov     es,word ptr [bp+8]
        mov     cx,word ptr ds:[__OVRTRAP__]
        cmp     cx,word ptr es:[0]
        jne     short L08DB
        cmp     word ptr es:[10h],0
        je      short L08AD
        cmp     byte ptr es:[20h],0CDh
        jne     short L08D4
L08AD:
        mov     word ptr [bp+8],0
        push    si
        push    di
        call    _seg046_597
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
L08D4:
        mov     ax,word ptr es:[10h]
        mov     word ptr [bp+8],ax
L08DB:
        pop     ds
        pop     bp
        retf
__CHECKOVERLAY endp

; ---- OVRSWAP (+08E5)

; _OvrInitEms  (+8DE)
; _OvrInitEms(handle, first page, pages) (Borland's _OvrInitEms): keep overlays in EMS as well
; as in the buffer, so reloading one is a copy from EMS rather than a read from disk. TMPALLOC.C
; calls it as _OvrInitEms(0, 0, 0) (all defaults) after it has set up its own EMS use.
__OvrInitEms proc far
        push    bp
        mov     bp,sp
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        test    byte ptr ds:[ovrswap_data],1
        je      short L08F4
L08EE:
        mov     ax,0FFFFh
        jmp     L09BB
L08F4:
        cmp     word ptr [bp+6],0
        je      short L090B
        mov     cx,word ptr [bp+0Ah]
        jcxz    L08EE
        cmp     cx,word ptr [bp+8]
        jbe     L08EE
        or      byte ptr ds:[__EmsChecked],1
        jmp     short L0928
L090B:
        FCALL   __INITEMS
        or      ax,ax
        je      L08EE
        mov     bx,word ptr [bp+8]
        cmp     bx,ax
        jae     L08EE
        sub     ax,bx
        mov     cx,word ptr [bp+0Ah]
        jcxz    L0926
        cmp     cx,ax
        jb      short L0928
L0926:
        mov     cx,ax
L0928:
        mov     es,word ptr cs:__DGROUP
        mov     bx,4000h
        mov     ax,word ptr es:[__OvrSize]
        mov     dx,word ptr es:[__OvrSize+2]
        add     ax,3FFFh
        adc     dx,0
        div     bx
        cmp     cx,ax
        jbe     short L0947
        mov     cx,ax
L0947:
        cmp     cx,4
        jae     short L0959
        mov     ax,400h
        mul     cx
        cmp     ax,word ptr ds:[__OvrMinHeapSize]
        jae     short L0959
        jmp     L08EE
L0959:
        push    word ptr [bp+6]
        push    word ptr [bp+8]
        mov     word ptr [bp+0Ah],cx
        push    cx
        FCALL   __INITEMSPAGE
        or      ax,ax
        jne     short L09BB
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
        mov     word ptr ds:[__EmsRec+12],offset L0A3E
        mov     word ptr ds:[__EmsRec+14],0
        mov     word ptr ds:[__OvrMemorySwap],1
        or      byte ptr ds:[ovrswap_data],2
        mov     word ptr ds:[__CALLEMSEXIT],offset __EXITEMS
        mov     word ptr ds:[__CALLEXTEMSSWAP],offset __SwapOverlay
        xor     ax,ax
        pop     di
L09BB:
        pop     ds
        pop     bp
        retf
L09BE:
        mov     ax,seg _EMSSEG_
        call    _seg046_C21
        jb      short L0A3D
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
        jb      short L0A38
        mov     dx,word ptr es:[8]
        xor     si,si
L09F7:
        mov     cx,4000h
        sub     cx,di
        cmp     cx,dx
        jb      short L0A02
        mov     cx,dx
L0A02:
        sub     dx,cx
        push    dx
        mov     dx,word ptr ds:[__EmsHandle]
        mov     ax,4400h
        int     67h
        pop     dx
        shl     ah,1
        jb      short L0A2C
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
        jne     L09F7
        clc
L0A2C:
        pushf
        mov     dx,word ptr ds:[__EmsHandle]
        mov     ah,48h
        int     67h
        popf
        jae     short L0A3D
L0A38:
        jmp     far ptr __OverlayHalt
L0A3D:
        retn
L0A3E:
        mov     dx,word ptr ds:[__EmsHandle]
        mov     ah,47h
        int     67h
        shl     ah,1
        jb      short L0AA2
        mov     ax,word ptr es:[14h]
        mov     dx,word ptr es:[16h]
        mov     bx,4000h
        div     bx
        mov     bx,ax
        mov     si,dx
        mov     dx,word ptr es:[8]
        xor     di,di
L0A63:
        mov     cx,4000h
        sub     cx,si
        cmp     cx,dx
        jb      short L0A6E
        mov     cx,dx
L0A6E:
        sub     dx,cx
        push    dx
        mov     dx,word ptr ds:[__EmsHandle]
        mov     ax,4400h
        int     67h
        pop     dx
        shl     ah,1
        jb      short L0A96
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
        jne     L0A63
L0A96:
        pushf
        mov     dx,word ptr ds:[__EmsHandle]
        mov     ah,48h
        int     67h
        popf
        jae     short L0AA7
L0AA2:
        jmp     far ptr __OverlayHalt
L0AA7:
        retn
__OvrInitEms endp

; seg046_AA8  (+AA8)
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
        je      short L0AC2
        xor     ax,ax
        jmp     L0BBB
L0AC2:
        mov     byte ptr [bp-2],0
        FCALL   __INITEXT
        or      ax,dx
        jne     short L0AD5
L0ACF:
        mov     ax,0FFFFh
        jmp     L0BBB
L0AD5:
        mov     bx,word ptr [bp+6]
        mov     cx,word ptr [bp+8]
        cmp     cx,word ptr ds:[__ExtStartAddress+2]
        jb      short L0AE9
        ja      short L0AF1
        cmp     bx,word ptr ds:[__ExtStartAddress]
        jae     short L0AF1
L0AE9:
        mov     cx,word ptr ds:[__ExtStartAddress+2]
        mov     bx,word ptr ds:[__ExtStartAddress]
L0AF1:
        cmp     cx,word ptr ds:[__ExtEndAddress+2]
        ja      L0ACF
        jb      short L0AFF
        cmp     bx,word ptr ds:[__ExtEndAddress]
        ja      L0ACF
L0AFF:
        mov     word ptr [bp+6],bx
        mov     word ptr [bp+8],cx
        mov     ax,word ptr ds:[__ExtEndAddress]
        mov     dx,word ptr ds:[__ExtEndAddress+2]
        sub     ax,bx
        sbb     dx,cx
        mov     si,word ptr [bp+0Ah]
        or      si,word ptr [bp+0Ch]
        je      short L0B2B
        cmp     dx,word ptr [bp+0Ch]
        jb      short L0B2B
        ja      short L0B25
        cmp     ax,word ptr ds:[__ExtEndAddress]
        jbe     short L0B2B
L0B25:
        mov     ax,word ptr [bp+0Ah]
        mov     dx,word ptr [bp+0Ch]
L0B2B:
        mov     es,word ptr cs:__DGROUP
        cmp     dx,word ptr es:[__OvrSize+2]
        jb      short L0B49
        ja      short L0B40
        cmp     ax,word ptr es:[__OvrSize]
        jbe     short L0B49
L0B40:
        mov     dx,word ptr es:[__OvrSize+2]
        mov     ax,word ptr es:[__OvrSize]
L0B49:
        mov     word ptr [bp+0Ah],ax
        mov     word ptr [bp+0Ch],dx
        or      dx,dx
        jne     short L0B61
        push    ax
        shr     ax,4
        cmp     ax,word ptr ds:[__OvrMinHeapSize]
        pop     ax
        jae     short L0B61
        jmp     L0ACF
L0B61:
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
        jne     short L0BBB
        cmp     byte ptr [bp-2],0
        jne     short L0B96
        inc     byte ptr [bp-2]
        jmp     L0AD5
L0B96:
        mov     word ptr ds:[__ExtRec+12],offset L0BF1
        mov     word ptr ds:[__ExtRec+14],0
        mov     word ptr ds:[__OvrMemorySwap],1
        or      byte ptr ds:[ovrswap_data],1
        mov     word ptr ds:[__CALLEXTEXIT],offset __EXITEXT
        mov     word ptr ds:[__CALLEXTEMSSWAP],offset __SwapOverlay
        xor     ax,ax
L0BBB:
        pop     di
        pop     si
        pop     ds
        mov     sp,bp
        pop     bp
        retf
__OvrInitExt endp

; seg046_BC2  (+BC2)
_seg046_BC2 proc near
        mov     ax,seg _EXTSEG_
        call    _seg046_C21
        jb      short L0BF0
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
        jae     short L0BF0
        jmp     far ptr __OverlayHalt
L0BF0:
        ret
L0BF1:
        call    _seg046_C15
        mov     ax,word ptr es:[10h]
        mov     dx,10h
        mul     dx
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr es:[14h]
        mov     dx,word ptr es:[16h]
        call    __EXTCOPY
        jae     short L0C14
        jmp     far ptr __OverlayHalt
L0C14:
        ret
_seg046_BC2 endp

; seg046_C15  (+C15)
_seg046_C15 proc near
        mov     si,word ptr es:[8]
        inc     si
        and     si,-2
        xor     di,di
        ret
_seg046_C15 endp

; seg046_C21  (+C21)
_seg046_C21 proc near
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
        call    _seg046_C15
        sub     ax,si
        sbb     dx,di
        jae     short L0C84
        pop     di
        pop     si
        mov     bp,word ptr ds:[0Ch]
        call    _seg046_CC8
        or      ax,ax
        je      short L0C6D
        mov     es,ax
        mov     dx,word ptr ds:[0Eh]
        mov     ax,word ptr es:[0Eh]
        mov     word ptr ds:[0Eh],ax
        mov     word ptr es:[0Eh],0
        call    _seg046_CF7
        mov     word ptr es:[0Eh],dx
L0C6D:
        mov     bx,word ptr ds:[0]
        mov     word ptr ds:[8],bx
        mov     cx,word ptr ds:[2]
        mov     word ptr ds:[0Ah],cx
        pop     es
        push    es
        call    _seg046_C15
        jmp     short L0C87
L0C84:
        add     sp,4
L0C87:
        mov     bp,offset __ReadOvrDisk
        call    _seg046_CC8
        or      ax,ax
        je      short L0C9A
        mov     es,ax
        mov     ax,word ptr es:[0Eh]
        mov     word ptr ds:[0Eh],ax
L0C9A:
        pop     es
        pop     bp
        add     word ptr ds:[8],si
        adc     word ptr ds:[0Ah],di
        mov     word ptr es:[14h],bx
        mov     word ptr es:[16h],cx
        mov     ax,word ptr ds:[0Ch]
        mov     word ptr es:[18h],ax
        mov     dx,es
        call    _seg046_CF7
        mov     word ptr es:[0Eh],dx
        mov     es,dx
        mov     word ptr es:[0Eh],ax
        clc
        pop     ds
        ret
_seg046_C21 endp

; seg046_CC8  (+CC8)
_seg046_CC8 proc near
        push    ds
        xor     ax,ax
        mov     es,ax
        jmp     short L0CEC
L0CCF:
        mov     ds,ax
        mov     ax,word ptr ds:[14h]
        sub     ax,bx
        mov     dx,word ptr ds:[16h]
        sbb     dx,cx
        jb      short L0CF3
        sub     ax,si
        sbb     dx,di
        jae     short L0CF3
        mov     word ptr ds:[18h],bp
        mov     ax,ds
        mov     es,ax
L0CEC:
        mov     ax,word ptr ds:[0Eh]
        or      ax,ax
        jne     L0CCF
L0CF3:
        mov     ax,es
        pop     ds
        ret
_seg046_CC8 endp

; seg046_CF7  (+CF7)
_seg046_CF7 proc near
        mov     ax,ds
L0CF9:
        mov     es,ax
        mov     ax,word ptr es:[0Eh]
        or      ax,ax
        jne     L0CF9
        ret
_seg046_CF7 endp

; seg046_D04  (+D04)
; __SwapOverlay: copy an overlay to or from the cache: EMS (_BBB) when bit 0 of OVRSWAP's flag
; byte is set, extended memory (L09BE) when bit 1 is.
__SwapOverlay proc near
        test    byte ptr ds:[ovrswap_data],1
        je      short L0D0F
        call    _seg046_BC2
        ret
L0D0F:
        test    byte ptr ds:[ovrswap_data],2
        je      short L0D19
        call    L09BE
L0D19:
        ret
__SwapOverlay endp

; ---- OVRDATA (+0D21): the default targets of the hooks in its _OVRDATA_, which hold
; 0D13 (__CALLEXTEMSSWAP) and 0D14 (__CALLEXTEXIT, __CALLEMSEXIT) in the EXE
L0D1A:
        ret
L0D1B:
        retf

; ---- OVRUSER (+0D23)

; seg046_D1C  (+D1C)
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
        jae     short L0D43
        mov     bx,word ptr es:[__ovrbuffer]
        jmp     short L0D45
L0D43:
        shl     bx,1
L0D45:
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
        jcxz    L0D72
        inc     dx
        add     bx,dx
        push    dx
        push    bx
        xor     ax,ax
        push    ax
        push    ax
        FCALL   __OVRINIT
        or      ax,ax
        jne     short L0D72
        pop     di
        pop     si
        pop     ds
        retf
L0D72:
        jmp     far ptr _abort
__OvrPrepare endp

; ---- OVRDETEC (+0D7E)
L0D77:
        db      'EMMXXXX0',0            ; the EMS driver's device name

; EMMXXXX_seg046_D80  (+D80)
__INITEMS proc far
        push    bp
        mov     bp,sp
        sub     sp,102h
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        push    si
        push    di
        test    byte ptr ds:[__EmsChecked],1
        je      short L0DA6
        test    byte ptr ds:[__EmsChecked],2
        jne     short L0DA0
        jmp     L0E0E
L0DA0:
        mov     ax,word ptr ds:[__EmsPages]
        jmp     L0E2B
L0DA6:
        push    ds
        push    cs
        pop     ds
        mov     dx,offset L0D77
        mov     ax,3D00h
        int     21h
        pop     ds
        mov     bx,ax
        jb      short L0DD8
        mov     ax,4400h
        int     21h
        jb      short L0DD4
        test    dx,80h
        je      short L0DD4
        mov     ax,4407h
        int     21h
        jb      short L0DD4
        push    ax
        mov     ah,3Eh
        int     21h
        pop     ax
        cmp     al,0FFh
        je      short L0DDB
L0DD4:
        mov     ah,3Eh
        int     21h
L0DD8:
        jmp     L0E24
L0DDB:
        mov     ax,3567h
        int     21h
        mov     ax,es
        or      ax,bx
        je      L0DD8
        mov     ah,40h
        int     67h
        or      ah,ah
        jne     L0DD8
        mov     byte ptr ds:[__EmsChecked],1
        mov     ah,46h
        int     67h
        or      ah,ah
        jne     L0DD8
        cmp     al,32h
        jb      L0DD8
        mov     byte ptr ds:[__EmsVersion],al
        mov     ah,41h
        int     67h
        or      ah,ah
        jne     L0DD8
        mov     word ptr ds:[__EmsPageFrame],bx
L0E0E:
        mov     ah,42h
        int     67h
        or      ah,ah
        jne     short L0E24
        mov     word ptr ds:[__EmsPages],bx
        mov     word ptr ds:[__EmsHandle],0FFFFh
        mov     ax,bx
        jmp     short L0E2B
L0E24:
        xor     ax,ax
        mov     byte ptr ds:[__EmsChecked],0
L0E2B:
        pop     di
        pop     si
        pop     ds
        mov     sp,bp
        pop     bp
        retf
__INITEMS endp

; seg046_E32  (+E32)
__INITEMSPAGE proc far
        push    bp
        mov     bp,sp
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        test    byte ptr ds:[__EmsChecked],1
        jne     short L0E45
        jmp     L0E8F
L0E45:
        test    byte ptr ds:[__EmsChecked],2
        je      short L0E51
        xor     ax,ax
        jmp     L0E92
L0E51:
        or      byte ptr ds:[__EmsChecked],2
        mov     ax,word ptr [bp+8]
        mov     word ptr ds:[__EmsFirstPage],ax
        mov     bx,word ptr [bp+6]
        mov     word ptr ds:[__EmsPages],bx
        mov     dx,word ptr [bp+0Ah]
        or      dx,dx
        jne     short L0E77
        mov     ah,43h
        int     67h
        or      ah,ah
        jne     short L0E8F
        or      byte ptr ds:[__EmsChecked],4
L0E77:
        mov     word ptr ds:[__EmsHandle],dx
        mov     ah,47h
        int     67h
        or      ah,ah
        jne     short L0E8F
        mov     dx,word ptr ds:[__EmsHandle]
        mov     ah,48h
        int     67h
        mov     al,ah
        jmp     short L0E92
L0E8F:
        mov     ax,0FFFFh
L0E92:
        pop     ds
        pop     bp
        retf    6
__INITEMSPAGE endp

; seg046_E97  (+E97)
__EXITEMS proc far
        push    bp
        mov     bp,sp
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        test    byte ptr ds:[__EmsChecked],4
        je      short L0EBE
        mov     dx,word ptr ds:[__EmsHandle]
        or      dx,dx
        js      short L0EBE
        mov     ah,45h
        int     67h
        mov     word ptr ds:[__EmsHandle],0FFFFh
        mov     byte ptr ds:[__EmsChecked],0
L0EBE:
        pop     ds
        pop     bp
        retf
__EXITEMS endp

; seg046_EC1  (+EC1)
__INITEXT proc far
        push    bp
        mov     bp,sp
        sub     sp,22h
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        push    si
        push    di
        test    byte ptr ds:[__ExtChecked],1
        je      short L0EE0
        test    byte ptr ds:[__ExtChecked],2
        je      short L0EE5
        jmp     L1056
L0EE0:
        mov     byte ptr ds:[__ExtChecked],1
L0EE5:
        mov     word ptr ds:[__ExtStartAddress],0
        mov     word ptr ds:[__ExtStartAddress+2],10h
        mov     word ptr ds:[__ExtEndAddress],0
        mov     word ptr ds:[__ExtEndAddress+2],10h
        push    sp
        pop     ax
        cmp     ax,sp
        jne     short L0F0B
        mov     ah,30h
        int     21h
        cmp     al,0Ah
        jb      short L0F0E
L0F0B:
        jmp     L1067
L0F0E:
        xchg    al,ah
        mov     word ptr [bp-22h],ax
        mov     ax,0F000h
        mov     es,ax
        mov     al,byte ptr es:[0FFFEh]
        cmp     al,0FCh
        ja      L0F0B
        cmp     al,0FBh
        je      L0F0B
        cmp     al,0F9h
        je      L0F0B
        cmp     word ptr [bp-22h],31h
        jae     short L0F31
L0F2E:
        jmp     L0FB0
L0F31:
        mov     ax,4300h
        int     2Fh
        cmp     al,80h
        jne     L0F2E
        mov     ax,4310h
        int     2Fh
        mov     word ptr ds:[__XmsControl],bx
        mov     word ptr ds:[__XmsControl+2],es
        xor     ax,ax
        call    dword ptr ds:[__XmsControl]
        cmp     ah,2
        jb      short L0FB0
        mov     ah,8
        call    dword ptr ds:[__XmsControl]
        or      ax,ax
        je      short L0FAD
        mov     si,ax
        xchg    dx,ax
        mov     ah,9
        call    dword ptr ds:[__XmsControl]
        or      ax,ax
        je      short L0FAD
        mov     word ptr ds:[__XmsHandle],dx
        mov     ah,0Ch
        call    dword ptr ds:[__XmsControl]
        or      ax,ax
        je      short L0F93
        mov     word ptr ds:[__ExtStartAddress],bx
        mov     word ptr ds:[__ExtStartAddress+2],dx
        mov     ax,400h
        mul     si
        add     ax,word ptr ds:[__ExtStartAddress]
        adc     dx,word ptr ds:[__ExtStartAddress+2]
        mov     word ptr ds:[__ExtEndAddress],ax
        mov     word ptr ds:[__ExtEndAddress+2],dx
L0F93:
        mov     ah,0Dh
        mov     dx,word ptr ds:[__XmsHandle]
        call    dword ptr ds:[__XmsControl]
        mov     ah,0Ah
        mov     dx,word ptr ds:[__XmsHandle]
        call    dword ptr ds:[__XmsControl]
        mov     word ptr ds:[__XmsHandle],0
L0FAD:
        jmp     L1056
L0FB0:
        mov     ah,88h
        int     15h
        or      ax,ax
        jne     short L0FBB
L0FB8:
        jmp     L1067
L0FBB:
        mov     dx,400h
        mul     dx
        add     dl,10h
        mov     word ptr ds:[__ExtEndAddress],ax
        mov     word ptr ds:[__ExtEndAddress+2],dx
        mov     cx,word ptr ds:[__XmsControl]
        or      cx,word ptr ds:[__XmsControl+2]
        jcxz    L0FDF
        mov     byte ptr ds:[__ExtStartAddress+2],11h
        mov     word ptr ds:[__ExtStartAddress],0
L0FDF:
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
        jb      L0FB8
        lea     di,[bp-1Dh]
        push    ss
        pop     es
        LEAD    si, ds:[ovrdetec_data]
        mov     cx,5
        cld
        repe cmpsb
        jne     short L1020
        mov     ax,word ptr [bp-15h]
        mul     word ptr [bp-0Dh]
        add     dl,10h
        mov     word ptr ds:[__ExtStartAddress],ax
        mov     byte ptr ds:[__ExtStartAddress+2],dl
L1020:
        xor     ax,ax
        mov     es,ax
        mov     es,word ptr es:[66h]
        mov     di,12h
        LEAD    si, ds:[ovrdetec_data]
        mov     cx,5
        cld
        repe cmpsb
        jne     short L1056
        mov     ax,word ptr es:[2Ch]
        mov     dl,byte ptr es:[2Eh]
        cmp     dl,byte ptr ds:[__ExtStartAddress+2]
        jb      short L1056
        ja      short L104F
        cmp     ax,word ptr ds:[__ExtStartAddress]
        jbe     short L1056
L104F:
        mov     word ptr ds:[__ExtStartAddress],ax
        mov     byte ptr ds:[__ExtStartAddress+2],dl
L1056:
        mov     ax,word ptr ds:[__ExtEndAddress]
        mov     dx,word ptr ds:[__ExtEndAddress+2]
        sub     ax,word ptr ds:[__ExtStartAddress]
        sbb     dx,word ptr ds:[__ExtStartAddress+2]
        jmp     short L106A
L1067:
        xor     ax,ax
        cwd
L106A:
        pop     di
        pop     si
        pop     ds
        mov     sp,bp
        pop     bp
        retf
__INITEXT endp

; seg046_1071  (+1071)
__INITEXTMEM proc far
        push    bp
        mov     bp,sp
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        push    si
        push    di
        test    byte ptr ds:[__ExtChecked],1
        jne     short L1086
        jmp     L113F
L1086:
        test    byte ptr ds:[__ExtChecked],2
        je      short L1092
        xor     ax,ax
        jmp     L1142
L1092:
        or      byte ptr ds:[__ExtChecked],2
        mov     ax,word ptr [bp+0Ah]
        mov     dx,word ptr [bp+0Ch]
        mov     si,word ptr [bp+6]
        mov     di,word ptr [bp+8]
        mov     bx,word ptr ds:[__XmsControl]
        or      bx,word ptr ds:[__XmsControl+2]
        je      short L10F0
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
        je      short L10ED
        mov     word ptr ds:[__XmsHandle],dx
        mov     ah,0Ch
        call    dword ptr ds:[__XmsControl]
        or      ax,ax
        je      short L10ED
        mov     word ptr ds:[__ExtStartAddress],bx
        mov     word ptr ds:[__ExtStartAddress+2],dx
        mov     word ptr ds:[__XmsBase],bx
        mov     word ptr ds:[__XmsBase+2],dx
        mov     ax,bx
        add     ax,si
        adc     dx,di
        jmp     short L1134
L10ED:
        jmp     L113F
L10F0:
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
L1134:
        mov     word ptr ds:[__ExtEndAddress],ax
        mov     word ptr ds:[__ExtEndAddress+2],dx
        xor     ax,ax
        jmp     short L1142
L113F:
        mov     ax,0FFFFh
L1142:
        pop     di
        pop     si
        pop     ds
        pop     bp
        retf    8
__INITEXTMEM endp

; seg046_1149  (+1149)
__EXITEXT proc far
        push    bp
        mov     bp,sp
        push    ds
        mov     ds,word ptr cs:__OVRGROUP
        cmp     byte ptr ds:[__ExtChecked],0
        je      short L119A
        cmp     word ptr ds:[__XmsHandle],0
        je      short L1176
        mov     ah,0Dh
        mov     dx,word ptr ds:[__XmsHandle]
        call    dword ptr ds:[__XmsControl]
        mov     ah,0Ah
        mov     dx,word ptr ds:[__XmsHandle]
        call    dword ptr ds:[__XmsControl]
        jmp     short L119A
L1176:
        xor     ax,ax
        mov     es,ax
        mov     ax,word ptr es:[66h]
        cmp     ax,seg _VDISKSEG_
        jne     short L119A
        push    ds
        mov     ds,ax
        mov     ax,word ptr ds:[__FakeVdisk+2Fh]
        mov     word ptr es:[64h],ax
        mov     ax,word ptr ds:[__FakeVdisk+31h]
        mov     word ptr es:[66h],ax
        pop     ds
        mov     byte ptr ds:[__ExtChecked],0
L119A:
        pop     ds
        pop     bp
        retf
__EXITEXT endp

; seg046_119D  (+119D)
__EXTCOPY proc near
        push    ds
        push    es
        add     si,1
        adc     di,0
        and     si,-2
        cld
        mov     ds,word ptr cs:__OVRGROUP
        cmp     word ptr ds:[__XmsHandle],0
        je      short L121F
        mov     word ptr ds:[__XMSGDT],si
        mov     word ptr ds:[__XMSGDT+2],di
        cmp     dl,10h
        jb      short L11D5
        mov     si,word ptr ds:[__XmsHandle]
        mov     word ptr ds:[__XMSGDT+4],si
        sub     ax,word ptr ds:[__XmsBase]
        sbb     dx,word ptr ds:[__XmsBase+2]
        jmp     L11DE
L11D5:
        mov     word ptr ds:[__XMSGDT+4],0
        ror     dx,4
L11DE:
        mov     word ptr ds:[__XMSGDT+6],ax
        mov     word ptr ds:[__XMSGDT+8],dx
        cmp     cl,10h
        jb      short L11FD
        mov     si,word ptr ds:[__XmsHandle]
        mov     word ptr ds:[__XMSGDT+10],si
        sub     bx,word ptr ds:[__XmsBase]
        sbb     cx,word ptr ds:[__XmsBase+2]
        jmp     L1206
L11FD:
        mov     word ptr ds:[__XMSGDT+10],0
        ror     cx,4
L1206:
        mov     word ptr ds:[__XMSGDT+12],bx
        mov     word ptr ds:[__XMSGDT+14],cx
        mov     ah,0Bh
        LEAD    si, ds:[__XMSGDT]
        call    dword ptr ds:[__XmsControl]
        or      ax,ax
        jne     short L1245
        stc
        jmp     short L1240
L121F:
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
        jae     short L1245
L1240:
        sbb     ax,ax
        stc
        jmp     short L1247
L1245:
        xor     ax,ax
L1247:
        pop     es
        pop     ds
        ret
__EXTCOPY endp

_OVRTEXT_ ends
        end
