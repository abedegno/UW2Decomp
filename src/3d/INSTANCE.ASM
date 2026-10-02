; /* target: seg004_0849_3000 */
; /* opts: /ml */
;
; The renderer's frame entry and its transforms. render_3d (called by seg021's cRender) sets up
; the projection from the clip window, builds the view matrix, and runs the render database
; through the model interpreter (INTERP.ASM). The rest is the geometry the interpreter's opcodes
; call: turning model points into view space (mini_xlate_rotate_pnt, load_xlate_rotate_pnt,
; mxmul), the frustum clip codes of a view-space point (code_pnt), the sub-object rotations by
; heading, pitch and bank (the instance_ routines), 3 x 3 matrix products (mm9x9, mm3x9_bpsi,
; mm3x9t), and the divide-overflow handlers the renderer installs on int 0.
;
; Fixed point: matrix entries are 1.15 (7FFFh is 1.0); a product of two is shifted left once and
; its high word kept. Points are 16-bit view or model coordinates.
; Data (FD58, DS; FM Towns names where they read the same variables):
;   14B2..14C2  the view matrix, nine words (three rows of three)
;   14A0, 14A2  the caller's SP and SS, saved by render_3d; 14AA a frame counter
;   14CC        DbEntry, the start of the render database (VIEW3D.C)
;   2470, 2472  scrh and scrw: half the view's height and width; 2474, 2476 biasx and biasy,
;               the view's centre on the screen (all four from the clip window at SS:0C30..0C36)
;   248C, 248E, 2490, 2492  the screen-aligned axes do_scalebm stands sprites on
;   24C8        a temporary for mxmul; 24CA, 24CC, 24CE  cos, sin and -sin of an instance angle
;   24E6        the zoom (cZoom); 24E8, 24EA, 24EC, 24EE, 24F0  scale factors scale_matrix keeps
;   24F4        the opcode table (vector_table)
;   257A..2590  the opcode table's entries for the relative-move opcodes, which check_flat swaps
;   25D4        nonzero while the flat (no pitch or bank) handlers are installed
;   25E0        the point shift (patched into the xlate routines by self_modify)
;   25E6, 25E8, 25EA  the eye in the current object's frame (FM vvars): x, y, z
; Self-modifying code: render_3d writes biasx and biasy into immediates in other modules
; (INTERP.ASM's _seg004_0849_644 .. 6C2, modify_biasx, modify_bx2 ...), self_modify writes the eye
; and shift into the xlate routines here, check_flat copies one of two mxmul bodies over mxmul, and
; seg004_0849_35FF patches modify_axis2 in SPHERE.ASM.
;
; name: inferred. The routines have their FM Towns names; System Shock's 3D library has
; instance.asm with instance_x, instance_y and instance_z, the same job for its axes.
;
; SEG004 split into its original modules: this is module 5 of 14, +3000..+3CF0 of the segment
; (file C1C0h), a module of the second library (UWLIB.LIB).
; Procs: seg004_0849_3000, render_3d, scale_matrix, check_flat, set_accept, self_modify,
; mini_xlate_rotate_pnt, load_xlate_rotate_pnt, mxmul, seg004_0849_35FF, instance_vvars_head,
; instance_head, instance_vvars_pitch, instance_pitch, instance_vvars_bank, instance_bank,
; code_pnt, mm9x9, mm3x9_bpsi, mm3x9t, overflow_handler_reg, overflow_handler_mem,
; overflow_handler_special_bp.
; Evidence for the bounds: starts on a paragraph after 5 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros). It is separate from the module before it: seg003's and seg021's modules are linked
; between their relocations.
; Its relocations are entries 2352-2366 of UW2.EXE's relocation table.
; Its FD71 piece is 60B9:05A5..0C40 (relocation entries 2351), defined before the code because
; those entries come before the code's.
;
; The FD71 piece (segment table entry 71, seg021's data segment, dseg062_62a6) holds the one
; relocation the segment has, a far pointer to seg004:0000 at 05A5, and its relocation entry
; (2351) falls between this module's code entries and the module before's, so it is this
; module's. Where the piece starts is not known: it is taken from that pointer, and it runs to
; the end of the segment; the bytes before it come from the EXE (tools/extract.py).

        .386

        extrn   _seg021_22FD_C10:far
        extrn   _sincos:far
        extrn   _SquareRoot_seg021_22FD_A30:far
        extrn   _uncmp_pal:far
        extrn   dseg062_62a6:far
        extrn   seg052_519C:far
        extrn   seg_370D:far

_DATA   segment word public use16 'DATA'
_DATA   ends
DGROUP  group   _DATA

FD71    segment byte public use16 'FAR_DATA'
        dd      _uncmp_pal
        db      9 dup (0)
        db      24h,03h,48h,06h,6Bh,09h,8Ch,0Ch,0ABh,0Fh,0C8h,12h,0E2h,15h,0F9h,18h
        db      0Ch,1Ch,1Ah,1Fh,24h,22h,28h,25h,27h,28h,1Fh,2Bh,11h,2Eh,0FCh,30h
        db      0DFh,33h,0BAh,36h,8Dh,39h,57h,3Ch,17h,3Fh,0CEh,41h,7Bh,44h,1Dh,47h
        db      0B4h,49h,40h,4Ch,0C0h,4Eh,34h,51h,9Bh,53h,0F6h,55h,43h,58h,82h,5Ah
        db      0B4h,5Ch,0D7h,5Eh,0ECh,60h,0F2h,62h,0E9h,64h,0D0h,66h,0A7h,68h,6Eh,6Ah
        db      24h,6Ch,0CAh,6Dh,5Fh,6Fh,0E3h,70h,55h,72h,0B6h,73h,05h,75h,42h,76h
        db      6Ch,77h,85h,78h,8Ah,79h,7Dh,7Ah,5Dh,7Bh,2Ah,7Ch,0E4h,7Ch,8Ah,7Dh
        db      1Eh,7Eh,9Dh,7Eh,0Ah,7Fh,62h,7Fh,0A7h,7Fh,0D9h,7Fh,0F6h,7Fh,0FFh,7Fh
        db      0F6h,7Fh,0D9h,7Fh,0A7h,7Fh,62h,7Fh,0Ah,7Fh,9Dh,7Eh,1Eh,7Eh,8Ah,7Dh
        db      0E4h,7Ch,2Ah,7Ch,5Dh,7Bh,7Dh,7Ah,8Ah,79h,85h,78h,6Ch,77h,42h,76h
        db      05h,75h,0B6h,73h,55h,72h,0E3h,70h,5Fh,6Fh,0CAh,6Dh,24h,6Ch,6Eh,6Ah
        db      0A7h,68h,0D0h,66h,0E9h,64h,0F2h,62h,0ECh,60h,0D7h,5Eh,0B4h,5Ch,82h,5Ah
        db      43h,58h,0F6h,55h,9Bh,53h,34h,51h,0C0h,4Eh,40h,4Ch,0B4h,49h,1Dh,47h
        db      7Bh,44h,0CEh,41h,17h,3Fh,57h,3Ch,8Dh,39h,0BAh,36h,0DFh,33h,0FCh,30h
        db      11h,2Eh,1Fh,2Bh,27h,28h,28h,25h,24h,22h,1Ah,1Fh,0Ch,1Ch,0F9h,18h
        db      0E2h,15h,0C8h,12h,0ABh,0Fh,8Ch,0Ch,6Bh,09h,48h,06h,24h,03h,00h,00h
        db      0DCh,0FCh,0B8h,0F9h,95h,0F6h,74h,0F3h,55h,0F0h,38h,0EDh,1Eh,0EAh,07h,0E7h
        db      0F4h,0E3h,0E6h,0E0h,0DCh,0DDh,0D8h,0DAh,0D9h,0D7h,0E1h,0D4h,0EFh,0D1h,04h,0CFh
        db      21h,0CCh,46h,0C9h,73h,0C6h,0A9h,0C3h,0E9h,0C0h,32h,0BEh,85h,0BBh,0E3h,0B8h
        db      4Ch,0B6h,0C0h,0B3h,40h,0B1h,0CCh,0AEh,65h,0ACh,0Ah,0AAh,0BDh,0A7h,7Eh,0A5h
        db      4Ch,0A3h,29h,0A1h,14h,9Fh,0Eh,9Dh,17h,9Bh,30h,99h,59h,97h,92h,95h
        db      0DCh,93h,36h,92h,0A1h,90h,1Dh,8Fh,0ABh,8Dh,4Ah,8Ch,0FBh,8Ah,0BEh,89h
        db      94h,88h,7Bh,87h,76h,86h,83h,85h,0A3h,84h,0D6h,83h,1Ch,83h,76h,82h
        db      0E2h,81h,63h,81h,0F6h,80h,9Eh,80h,59h,80h,27h,80h,0Ah,80h,01h,80h
        db      0Ah,80h,27h,80h,59h,80h,9Eh,80h,0F6h,80h,63h,81h,0E2h,81h,76h,82h
        db      1Ch,83h,0D6h,83h,0A3h,84h,83h,85h,76h,86h,7Bh,87h,94h,88h,0BEh,89h
        db      0FBh,8Ah,4Ah,8Ch,0ABh,8Dh,1Dh,8Fh,0A1h,90h,36h,92h,0DCh,93h,92h,95h
        db      59h,97h,30h,99h,17h,9Bh,0Eh,9Dh,14h,9Fh,29h,0A1h,4Ch,0A3h,7Eh,0A5h
        db      0BDh,0A7h,0Ah,0AAh,65h,0ACh,0CCh,0AEh,40h,0B1h,0C0h,0B3h,4Ch,0B6h,0E3h,0B8h
        db      85h,0BBh,32h,0BEh,0E9h,0C0h,0A9h,0C3h,73h,0C6h,46h,0C9h,21h,0CCh,04h,0CFh
        db      0EFh,0D1h,0E1h,0D4h,0D9h,0D7h,0D8h,0DAh,0DCh,0DDh,0E6h,0E0h,0F4h,0E3h,07h,0E7h
        db      1Eh,0EAh,38h,0EDh,55h,0F0h,74h,0F3h,95h,0F6h,0B8h,0F9h,0DCh,0FCh,00h,00h
        db      24h,03h,48h,06h,6Bh,09h,8Ch,0Ch,0ABh,0Fh,0C8h,12h,0E2h,15h,0F9h,18h
        db      0Ch,1Ch,1Ah,1Fh,24h,22h,28h,25h,27h,28h,1Fh,2Bh,11h,2Eh,0FCh,30h
        db      0DFh,33h,0BAh,36h,8Dh,39h,57h,3Ch,17h,3Fh,0CEh,41h,7Bh,44h,1Dh,47h
        db      0B4h,49h,40h,4Ch,0C0h,4Eh,34h,51h,9Bh,53h,0F6h,55h,43h,58h,82h,5Ah
        db      0B4h,5Ch,0D7h,5Eh,0ECh,60h,0F2h,62h,0E9h,64h,0D0h,66h,0A7h,68h,6Eh,6Ah
        db      24h,6Ch,0CAh,6Dh,5Fh,6Fh,0E3h,70h,55h,72h,0B6h,73h,05h,75h,42h,76h
        db      6Ch,77h,85h,78h,8Ah,79h,7Dh,7Ah,5Dh,7Bh,2Ah,7Ch,0E4h,7Ch,8Ah,7Dh
        db      1Eh,7Eh,9Dh,7Eh,0Ah,7Fh,62h,7Fh,0A7h,7Fh,0D9h,7Fh,0F6h,7Fh,0FFh,7Fh
        db      00h,00h,51h,00h,0A2h,00h,0F4h,00h,46h,01h,97h,01h,0E9h,01h,3Ah,02h
        db      8Ch,02h,0DDh,02h,2Fh,03h,81h,03h,0D3h,03h,25h,04h,77h,04h,0C9h,04h
        db      1Bh,05h,6Dh,05h,0BFh,05h,12h,06h,64h,06h,0B7h,06h,09h,07h,5Ch,07h
        db      0AFh,07h,02h,08h,55h,08h,0A8h,08h,0FCh,08h,4Fh,09h,0A3h,09h,0F7h,09h
        db      4Bh,0Ah,9Fh,0Ah,0F4h,0Ah,48h,0Bh,9Dh,0Bh,0F2h,0Bh,47h,0Ch,9Dh,0Ch
        db      0F3h,0Ch,48h,0Dh,9Fh,0Dh,0F5h,0Dh,4Ch,0Eh,0A3h,0Eh,0FAh,0Eh,51h,0Fh
        db      0A9h,0Fh,01h,10h,59h,10h,0B2h,10h,0Bh,11h,64h,11h,0BEh,11h,18h,12h
        db      73h,12h,0CDh,12h,29h,13h,84h,13h,0E0h,13h,3Dh,14h,9Ah,14h,0F7h,14h
        db      55h,15h,0B3h,15h,12h,16h,71h,16h,0D1h,16h,32h,17h,93h,17h,0F4h,17h
        db      57h,18h,0BAh,18h,1Dh,19h,81h,19h,0E6h,19h,4Ch,1Ah,0B2h,1Ah,19h,1Bh
        db      81h,1Bh,0EAh,1Bh,54h,1Ch,0BEh,1Ch,2Ah,1Dh,96h,1Dh,04h,1Eh,73h,1Eh
        db      0E2h,1Eh,53h,1Fh,0C5h,1Fh,38h,20h,0ADh,20h,23h,21h,9Ah,21h,13h,22h
        db      8Dh,22h,09h,23h,87h,23h,07h,24h,88h,24h,0Ch,25h,91h,25h,19h,26h
        db      0A4h,26h,31h,27h,0C1h,27h,54h,28h,0EAh,28h,84h,29h,21h,2Ah,0C2h,2Ah
        db      68h,2Bh,13h,2Ch,0C4h,2Ch,7Ah,2Dh,37h,2Eh,0FCh,2Eh,0C9h,2Fh,0A1h,30h
        db      84h,31h,76h,32h,79h,33h,93h,34h,0C9h,35h,29h,37h,0C9h,38h,0E7h,3Ah
        db      00h,40h,00h,40h,0AEh,3Fh,5Dh,3Fh,0Bh,3Fh,0B9h,3Eh,68h,3Eh,16h,3Eh
        db      0C5h,3Dh,73h,3Dh,22h,3Dh,0D0h,3Ch,7Eh,3Ch,2Ch,3Ch,0DAh,3Bh,88h,3Bh
        db      36h,3Bh,0E4h,3Ah,92h,3Ah,40h,3Ah,0EDh,39h,9Bh,39h,48h,39h,0F6h,38h
        db      0A3h,38h,50h,38h,0FDh,37h,0AAh,37h,57h,37h,03h,37h,0B0h,36h,5Ch,36h
        db      08h,36h,0B4h,35h,60h,35h,0Bh,35h,0B7h,34h,62h,34h,0Dh,34h,0B8h,33h
        db      62h,33h,0Ch,33h,0B7h,32h,60h,32h,0Ah,32h,0B3h,31h,5Ch,31h,05h,31h
        db      0AEh,30h,56h,30h,0FEh,2Fh,0A6h,2Fh,4Dh,2Fh,0F4h,2Eh,9Bh,2Eh,41h,2Eh
        db      0E7h,2Dh,8Ch,2Dh,32h,2Dh,0D6h,2Ch,7Bh,2Ch,1Fh,2Ch,0C2h,2Bh,65h,2Bh
        db      08h,2Bh,0AAh,2Ah,4Ch,2Ah,0EDh,29h,8Eh,29h,2Eh,29h,0CDh,28h,6Ch,28h
        db      0Bh,28h,0A8h,27h,45h,27h,0E2h,26h,7Eh,26h,19h,26h,0B3h,25h,4Dh,25h
        db      0E6h,24h,7Eh,24h,15h,24h,0ABh,23h,41h,23h,0D5h,22h,69h,22h,0FBh,21h
        db      8Ch,21h,1Dh,21h,0ACh,20h,3Ah,20h,0C7h,1Fh,52h,1Fh,0DCh,1Eh,65h,1Eh
        db      0ECh,1Dh,72h,1Dh,0F6h,1Ch,78h,1Ch,0F8h,1Bh,77h,1Bh,0F3h,1Ah,6Eh,1Ah
        db      0E6h,19h,5Bh,19h,0CEh,18h,3Eh,18h,0ABh,17h,15h,17h,7Bh,16h,0DEh,15h
        db      3Dh,15h,97h,14h,0ECh,13h,3Bh,13h,85h,12h,0C8h,11h,03h,11h,36h,10h
        db      5Eh,0Fh,7Bh,0Eh,89h,0Dh,86h,0Ch,6Ch,0Bh,36h,0Ah,0D6h,08h,36h,07h
        db      18h,05h
        db      0Eh dup (0)
        db      36h,00h,01h
        db      35h dup (0)
        db      1Dh,00h,0FFh,0FFh,00h,00h,00h,00h,00h,00h,01h,00h,00h,00h,3Dh,6Ah
        db      8 dup (0)
        db      0C7h,00h,3Fh,01h,00h,00h,90h,0Ah
        db      1A6h dup (0)
FD71    ends

SEG004_TEXT segment para public use16 'CODE'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _create_matrix:near
        extrn   _do_rod:near
        extrn   _do_x_rel:near
        extrn   _do_xy_rel:near
        extrn   _do_xz_rel:near
        extrn   _do_y_rel:near
        extrn   _do_yz_rel:near
        extrn   _do_z_rel:near
        extrn   _flat_rod:near
        extrn   _flat_x_rel:near
        extrn   _flat_xy_rel:near
        extrn   _flat_xz_rel:near
        extrn   _flat_y_rel:near
        extrn   _flat_yz_rel:near
        extrn   _flat_z_rel:near
        extrn   _seg004_0849_644:near
        extrn   _seg004_0849_64F:near
        extrn   _seg004_0849_6B2:near
        extrn   _seg004_0849_6C2:near
        extrn   modify_axis2:near
        extrn   modify_biasx:near
        extrn   modify_biasy:near
        extrn   modify_bx2:near
        extrn   modify_by2:near
        public  _check_flat
        public  _code_pnt
        public  _instance_bank
        public  _instance_head
        public  _instance_pitch
        public  _instance_vvars_bank
        public  _instance_vvars_head
        public  _instance_vvars_pitch
        public  _load_xlate_rotate_pnt
        public  _mini_xlate_rotate_pnt
        public  _mm3x9_bpsi
        public  _mm3x9t
        public  _mm9x9
        public  _mxmul
        public  _overflow_handler_mem
        public  _overflow_handler_reg
        public  _overflow_handler_special_bp
        public  _render_3d
        public  _scale_matrix
        public  _seg004_0849_3000
        public  _seg004_0849_35FF
        public  _self_modify
        public  _set_accept
        public  overflow_count

; seg004_0849_3000  (+3000)
; seg004_0849_3000: an opcode handler that breaks into the debugger (int 3) and goes on to the
; next opcode; probably the table's entry for unused opcodes.
_seg004_0849_3000 proc near
        int     3
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_seg004_0849_3000 endp

; render_3d  (+3007): FM render_3d
; render_3d: draw the frame whose render database starts at DbEntry. Called by cRender
; (src/sys/C3DENTRY.ASM) with SS on cRender's private stack in FD71 (dseg062_62a6, seg021's data
; segment), where it reads the view window: the copy of the clip window that set_the_window
; (VIDMODE.ASM) keeps at FD71:0C30, 0C30 left, 0C32 top, 0C34 right, 0C36 bottom, y counting up
; from the bottom of the screen. Works out scrw, scrh, biasx and biasy from it and patches biasx
; and biasy into the projection code; counts the frame; builds the view matrix (create_matrix in
; INTERP.ASM, then seg004_0849_35FF, scale_matrix, check_flat).
;
; It then switches to a stack in seg_370D (SP 5548h) with seg_370D:2CEE set to 9C9h, and calls
; seg021_22FD_C10, which only returns (SYSLIBP.ASM): a hook that does nothing in this build.
; Finally it restores the stack and runs the database: the first opcode at DbEntry is called
; through the opcode table, and the chain of handlers returns here at the end of the frame
; (do_eof). Out: DS = ES = SS; seg_370D:2CEE restored.
_render_3d proc far
        mov     ax,seg seg052_519C
        mov     ds,ax
        mov     es,ax
        mov     ax,word ptr ss:[0C34h]
        sub     ax,word ptr ss:[0C30h]
        inc     ax
        shr     ax,1
        mov     word ptr ds:[2472h],ax
        mov     ax,word ptr ss:[0C32h]
        sub     ax,word ptr ss:[0C36h]
        inc     ax
        shr     ax,1
        mov     word ptr ds:[2470h],ax
        mov     ax,word ptr ss:[0C34h]
        add     ax,word ptr ss:[0C30h]
        inc     ax
        shr     ax,1
        mov     word ptr ds:[2474h],ax
        mov     word ptr cs:[_seg004_0849_644+1],ax
        mov     word ptr cs:[_seg004_0849_6B2+1],ax
        mov     word ptr cs:[modify_biasx+1],ax
        mov     word ptr cs:[modify_bx2+1],ax
        mov     ax,word ptr ss:[0C32h]
        add     ax,word ptr ss:[0C36h]
        shr     ax,1
        mov     word ptr ds:[2476h],ax
        mov     word ptr cs:[_seg004_0849_64F+1],ax
        mov     word ptr cs:[_seg004_0849_6C2+1],ax
        mov     word ptr cs:[modify_biasy+1],ax
        mov     word ptr cs:[modify_by2+1],ax
        inc     word ptr ds:[14AAh]
        mov     byte ptr ds:[25E0h],0
        call    _create_matrix
        call    _seg004_0849_35FF
        call    _scale_matrix
        call    _check_flat
        mov     ax,ss
        mov     word ptr ds:[14A2h],ax
        mov     word ptr ds:[14A0h],sp
        mov     si,seg seg_370D
        mov     ds,si
        mov     ax,word ptr ds:[2CEEh]
        mov     word ptr cs:L30DA,ax
        mov     word ptr ds:[2CEEh],9C9h
        mov     ax,seg dseg062_62a6
        mov     es,ax
        mov     es,si
        cli
        mov     ss,si
        mov     sp,5548h
        sti
        call    far ptr _seg021_22FD_C10
        mov     bp,seg seg052_519C
        mov     ds,bp
        mov     es,bp
        cli
        mov     ss,word ptr ds:[14A2h]
        mov     sp,word ptr ds:[14A0h]
        sti
        mov     si,word ptr ds:[14CCh]
        lodsw
        xchg    bx,ax
        call    word ptr [bx+24F4h]
        mov     ax,seg seg_370D
        mov     ds,ax
        mov     ax,word ptr cs:L30DA
        mov     word ptr ds:[2CEEh],ax
        mov     ax,ss
        mov     ds,ax
        mov     es,ax
        retf
L30DA   dw      0                       ; the seg_370D word at 2CEEh, saved over the render
_render_3d endp

; seg004_0849_30DC  (+30DC)
; scale_matrix: fold the screen's shape and the zoom into the view matrix. Scales matrix entries
; by the ratio of the view's half width to half height (with 14A4, whose sign picks which axis
; is scaled; probably so x and y project to the square frustum CLIP.ASM's 45-degree planes
; assume) and by the zoom word 24E6 (the depth column when it is positive, the other entries
; when negative), and works out 248C/248E, a unit horizontal direction on the screen, and from
; it 2490/2492, the axes do_scalebm (TMAPOPS.ASM) uses to place sprites facing the viewer.
; Installs _overflow_handler_reg for the divides: cRender points int 0 at FD71:05A0, which holds
; `jmp far cs:[5A5]`, and the far pointer at 5A5 (FM overflow_ptr) is into seg004, so writing
; its offset word selects the handler. Uses SquareRoot_seg021_22FD_A30 (IMATH.ASM).
_scale_matrix proc near
        mov     word ptr ds:[24E8h],7FFFh
        mov     word ptr ds:[24EAh],7FFFh
        mov     ax,word ptr ds:[14B8h]
        mov     word ptr ds:[14C4h],ax
        mov     ax,word ptr ds:[14BAh]
        mov     word ptr ds:[14C6h],ax
        mov     ax,word ptr ds:[14BCh]
        mov     word ptr ds:[14C8h],ax
        mov     bx,word ptr ds:[2470h]
        mov     cx,word ptr ds:[2472h]
        mov     ax,word ptr ds:[14A4h]
        or      ax,ax
        js      short L3113
        imul    cx
        shl     ax,1
        rcl     dx,1
        xor     cx,cx
        jmp     short L3123
L3113:
        neg     ax
        imul    bx
        shl     ax,1
        rcl     dx,1
        mov     bx,dx
        mov     dx,cx
        mov     cx,ax
        xor     ax,ax
L3123:
        cmp     dx,bx
        jne     short L312A
        jmp     L31CB
L312A:
        jl      short L317E
        xchg    dx,bx
        xchg    cx,ax
        shr     dx,1
        rcr     ax,1
        idiv    bx
        mov     cx,ax
        mov     word ptr ds:[24E8h],cx
        mov     ax,word ptr ds:[14B2h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14B2h],dx
        mov     ax,word ptr ds:[14B8h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14B8h],dx
        mov     ax,word ptr ds:[14BEh]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14BEh],dx
        mov     ax,word ptr ds:[14C6h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C6h],dx
        mov     ax,word ptr ds:[14C8h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C8h],dx
        jmp     short L31CB
L317E:
        shr     dx,1
        rcr     ax,1
        idiv    bx
        mov     cx,ax
        mov     word ptr ds:[24EAh],cx
        mov     ax,word ptr ds:[14B4h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14B4h],dx
        mov     ax,word ptr ds:[14BAh]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14BAh],dx
        mov     ax,word ptr ds:[14C0h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C0h],dx
        mov     ax,word ptr ds:[14C4h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C4h],dx
        mov     ax,word ptr ds:[14C8h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C8h],dx
L31CB:
        mov     cx,word ptr ds:[24E6h]
        or      cx,cx
        jns     short L31D6
        jmp     L325E
L31D6:
        mov     word ptr ds:[24F0h],cx
        mov     ax,word ptr ds:[14B6h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14B6h],dx
        mov     ax,word ptr ds:[14BCh]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14BCh],dx
        mov     ax,word ptr ds:[14C2h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C2h],dx
        mov     ax,word ptr ds:[14C6h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C6h],dx
        mov     ax,word ptr ds:[14C4h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C4h],dx
        mov     ax,cx
        imul    ax
        mov     bp,ax
        mov     si,dx
        mov     ax,word ptr ds:[24E8h]
        mov     word ptr ds:[24ECh],ax
        imul    ax
        add     ax,bp
        adc     dx,si
        mov     bx,dx
        xor     cx,cx
        call    far ptr _SquareRoot_seg021_22FD_A30
        mov     ax,di
        xchg    ah,al
        mov     word ptr ds:[24E8h],ax
        mov     ax,word ptr ds:[24EAh]
        mov     word ptr ds:[24EEh],ax
        imul    ax
        add     ax,bp
        adc     dx,si
        mov     bx,dx
        xor     cx,cx
        call    far ptr _SquareRoot_seg021_22FD_A30
        mov     ax,di
        xchg    al,ah
        mov     word ptr ds:[24EAh],ax
        jmp     L331D
L325E:
        mov     word ptr ds:[24F0h],7FFFh
        neg     cx
        mov     ax,word ptr ds:[14B2h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14B2h],dx
        mov     ax,word ptr ds:[14B8h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14B8h],dx
        mov     ax,word ptr ds:[14BEh]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14BEh],dx
        mov     ax,word ptr ds:[14B4h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14B4h],dx
        mov     ax,word ptr ds:[14BAh]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14BAh],dx
        mov     ax,word ptr ds:[14C0h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C0h],dx
        mov     ax,word ptr ds:[14C8h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[14C8h],dx
        mov     si,cx
        mov     ax,word ptr ds:[24E8h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     bp,dx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[24ECh],dx
        mov     dx,bp
        mov     ax,dx
        imul    ax
        mov     bx,dx
        add     bx,3FFFh
        xor     cx,cx
        call    far ptr _SquareRoot_seg021_22FD_A30
        mov     ax,di
        xchg    ah,al
        mov     word ptr ds:[24E8h],ax
        mov     ax,word ptr ds:[24EAh]
        imul    si
        shl     ax,1
        rcl     dx,1
        mov     bp,dx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[24EEh],dx
        mov     dx,bp
        mov     ax,dx
        imul    ax
        mov     bx,dx
        add     bx,3FFFh
        xor     cx,cx
        call    far ptr _SquareRoot_seg021_22FD_A30
        mov     ax,di
        xchg    al,ah
        mov     word ptr ds:[24EAh],ax
L331D:
        mov     word ptr ss:[5A5h],offset _overflow_handler_reg
        mov     ax,word ptr ds:[14B6h]
        imul    word ptr ds:[14B6h]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[14C2h]
        imul    word ptr ds:[14C2h]
        add     bx,ax
        adc     cx,dx
        or      ch,ch
        je      short L336F
        call    far ptr _SquareRoot_seg021_22FD_A30
        mov     dx,word ptr ds:[14B6h]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L3355
        inc     ax
L3355:
        mov     word ptr ds:[248Ch],ax
        mov     dx,word ptr ds:[14C2h]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L336A
        inc     ax
L336A:
        mov     word ptr ds:[248Eh],ax
        jmp     short L33C3
L336F:
        mov     ax,word ptr ds:[14B4h]
        imul    word ptr ds:[14B4h]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[14C0h]
        imul    word ptr ds:[14C0h]
        add     bx,ax
        adc     cx,dx
        call    far ptr _SquareRoot_seg021_22FD_A30
        mov     dx,word ptr ds:[14B4h]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L339C
        inc     ax
L339C:
        mov     bx,ax
        mov     dx,word ptr ds:[14C0h]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L33B0
        inc     ax
L33B0:
        test    word ptr ds:[14BCh],0FFFFh
        js      short L33BC
        neg     bx
        neg     ax
L33BC:
        mov     word ptr ds:[248Ch],bx
        mov     word ptr ds:[248Eh],ax
L33C3:
        mov     ax,word ptr ds:[248Eh]
        imul    word ptr ds:[14B2h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[248Ch]
        neg     ax
        imul    word ptr ds:[14BEh]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        adc     dx,0
        mov     word ptr ds:[2490h],dx
        mov     ax,word ptr ds:[248Eh]
        imul    word ptr ds:[14B4h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[248Ch]
        neg     ax
        imul    word ptr ds:[14C0h]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        adc     dx,0
        mov     word ptr ds:[2492h],dx
        ret
_scale_matrix endp

; seg004_0849_340E  (+340E)
; check_flat: when the view is level (the matrix's middle row and column are 0 except the middle
; entry, at least 7FFBh), install the flat variants of the relative-move opcode handlers
; (flat_x_rel ... flat_rod, INTERP.ASM) in the opcode table and copy the short mxmul body
; (L35DA) over mxmul; otherwise install the general handlers and the full body (L3587). 25D4
; records which set is in, so the copy is done only on a change. A level view is probably the
; common case: the view pitches only while the player looks up or down.
_check_flat proc near
        test    word ptr ds:[14B8h],0FFFFh
        jne     short L3436
        test    word ptr ds:[14B4h],0FFFFh
        jne     short L3436
        test    word ptr ds:[14C0h],0FFFFh
        jne     short L3436
        test    word ptr ds:[14BCh],0FFFFh
        jne     short L3436
        cmp     word ptr ds:[14BAh],7FFBh
        jge     short is_flat
L3436:
        test    byte ptr ds:[25D4h],0FFh
        je      short L347C
        mov     byte ptr ds:[25D4h],0
        mov     word ptr ds:[257Ah],offset _do_x_rel
        mov     word ptr ds:[257Ch],offset _do_y_rel
        mov     word ptr ds:[257Eh],offset _do_z_rel
        mov     word ptr ds:[2584h],offset _do_xy_rel
        mov     word ptr ds:[2588h],offset _do_yz_rel
        mov     word ptr ds:[2586h],offset _do_xz_rel
        mov     word ptr ds:[2590h],offset _do_rod
        mov     si,offset L3587
        mov     di,offset _mxmul
        push    es
        push    cs
        pop     es
        mov     cx,54h
        rep movs byte ptr es:[di],byte ptr cs:[si]
        pop     es
L347C:
        ret
is_flat:
        test    byte ptr ds:[25D4h],0FFh
        jne     L347C
        mov     byte ptr ds:[25D4h],0FFh
        mov     word ptr ds:[257Ah],offset _flat_x_rel
        mov     word ptr ds:[257Ch],offset _flat_y_rel
        mov     word ptr ds:[257Eh],offset _flat_z_rel
        mov     word ptr ds:[2584h],offset _flat_xy_rel
        mov     word ptr ds:[2588h],offset _flat_yz_rel
        mov     word ptr ds:[2586h],offset _flat_xz_rel
        mov     word ptr ds:[2590h],offset _flat_rod
        mov     si,offset L35DA
        mov     di,offset _mxmul
        push    es
        push    cs
        pop     es
        mov     cx,26h
        rep movs byte ptr es:[di],byte ptr cs:[si]
        pop     es
        ret
_check_flat endp

; seg004_0849_34C4  (+34C4)
; set_accept: empty here and in FM Towns (accept_flag, which it would set, is never written; see
; CLIP.ASM).
_set_accept proc near
        ret
_set_accept endp

; seg004_0849_34C5  (+34C5)
; self_modify: patch the eye position in the current object's frame (25E6, 25E8, 25EA) and the
; point shift (25E0) into the immediates of mini_xlate_rotate_pnt and load_xlate_rotate_pnt, so
; that turning a model point into view space needs no memory reads for them.
_self_modify proc near
        mov     ax,word ptr ds:[25E6h]
        mov     word ptr cs:[modify_mex+1],ax
        mov     word ptr cs:[L34F9+1],ax
        mov     ax,word ptr ds:[25E8h]
        mov     word ptr cs:[modify_mey+1],ax
        mov     word ptr cs:[L3511+1],ax
        mov     ax,word ptr ds:[25EAh]
        mov     word ptr cs:[modify_mez+1],ax
        mov     word ptr cs:[L3505+1],ax
        mov     al,byte ptr ds:[25E0h]
        mov     byte ptr cs:[modify_mes+1],al
        mov     byte ptr cs:[_mini_xlate_rotate_pnt+1],al
        ret
_self_modify endp

; seg004_0849_34F2  (+34F2)
; mini_xlate_rotate_pnt: in: SI = three signed bytes, a model point in the order x, z, y, each
; times 32. Subtracts the patched eye, shifts by the patched shift and falls into mxmul. Out:
; BX, CX, BP = the view-space point (see mxmul); SI advanced by 3. load_xlate_rotate_pnt below
; is the same for three words.
_mini_xlate_rotate_pnt proc near
        mov     cl,5                    ; the shift and the three origins are patched by self_modify
        lodsb
        cbw
        shl     ax,5
L34F9:
        sub     ax,1234h
        shl     ax,cl
        mov     bx,ax
        lodsb
        cbw
        shl     ax,5
L3505:
        sub     ax,1234h
        shl     ax,cl
        mov     bp,ax
        lodsb
        cbw
        shl     ax,5
L3511:
        sub     ax,1234h
        shl     ax,cl
        mov     cx,ax
        jmp     short _mxmul
_mini_xlate_rotate_pnt endp

; seg004_0849_351A  (+351A)
_load_xlate_rotate_pnt proc near
modify_mes:                             ; FM names for the patched shift and origins
        mov     cl,5
        lodsw
modify_mex:
        sub     ax,1234h
        shl     ax,cl
        mov     bx,ax
        lodsw
modify_mez:
        sub     ax,1234h
        shl     ax,cl
        mov     bp,ax
        lodsw
modify_mey:
        sub     ax,1234h
        shl     ax,cl
        mov     cx,ax
_load_xlate_rotate_pnt endp

; seg004_0849_3534  (+3534)
; mxmul: rotate a point by the view matrix. In: BX, CX, BP = the point's x, y (vertical) and z.
; Out: BX, CX, BP = view x, y and depth z, each the high word of its 1.15 products (so at half
; scale); DI, DX changed. The general body (copied from L3587) does all nine products; the flat
; body (L35DA, for a level view) skips the products with the zero entries and takes y as it is,
; halved.
_mxmul proc near
        mov     ax,bx
        imul    word ptr ds:[14B2h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[14B8h]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14BEh]
        add     di,dx
        mov     word ptr ds:[24C8h],di
        mov     ax,bx
        imul    word ptr ds:[14B4h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[14BAh]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14C0h]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14C2h]
        mov     bp,dx
        mov     ax,bx
        imul    word ptr ds:[14B6h]
        add     bp,dx
        mov     ax,cx
        imul    word ptr ds:[14BCh]
        add     bp,dx
        mov     cx,di
        mov     bx,word ptr ds:[24C8h]
        ret
; L3587 and L35DA: the two bodies check_flat and is_flat copy over the one above
L3587:
        mov     ax,bx
        imul    word ptr ds:[14B2h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[14B8h]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14BEh]
        add     di,dx
        mov     word ptr ds:[24C8h],di
        mov     ax,bx
        imul    word ptr ds:[14B4h]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[14BAh]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14C0h]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[14C2h]
        mov     bp,dx
        mov     ax,bx
        imul    word ptr ds:[14B6h]
        add     bp,dx
        mov     ax,cx
        imul    word ptr ds:[14BCh]
        add     bp,dx
        mov     cx,di
        mov     bx,word ptr ds:[24C8h]
        ret
L35DA:
        mov     ax,bx
        imul    word ptr ds:[14B2h]
        mov     di,dx
        mov     ax,bp
        imul    word ptr ds:[14BEh]
        add     di,dx
        mov     ax,bx
        mov     bx,di
        imul    word ptr ds:[14B6h]
        mov     ax,bp
        mov     bp,dx
        imul    word ptr ds:[14C2h]
        add     bp,dx
        sar     cx,1
        ret
_mxmul endp

; seg004_0849_35FF  (+35FF)
; seg004_0849_35FF: choose the axis for SPHERE.ASM's first cull (modify_axis2): y when the view
; is pitched by more than 45 degrees (|14BC| > 5A82h, sin 45 in 1.15), else whichever of x and z
; the view looks along more. Patches the instruction to add or subtract (by the sign) the eye's
; coordinate on that axis (25E6 + offset).
_seg004_0849_35FF proc near
        mov     ax,word ptr ds:[14BCh]
        cwd
        xor     ax,dx
        sub     ax,dx
        cmp     ax,5A82h
        jle     short L3611
        mov     ax,2
        jmp     short L3633
L3611:
        mov     ax,word ptr ds:[14B6h]
        cwd
        xor     ax,dx
        sub     ax,dx
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[14C2h]
        cwd
        xor     ax,dx
        sub     ax,dx
        cmp     bx,ax
        jg      short L362E
        mov     ax,4
        jmp     short L3633
L362E:
        mov     ax,0
        mov     dx,cx
L3633:
        mov     bl,2Bh
        test    dx,0FFFFh
        jns     short L363D
        mov     bl,3
L363D:
        mov     byte ptr cs:modify_axis2,bl
        add     ax,8
        sub     ax,8
        add     ax,25E6h
        mov     word ptr cs:[modify_axis2+2],ax
        ret
_seg004_0849_35FF endp

; seg004_0849_3650  (+3650)
; instance_vvars_head: rotate the eye position in the object's frame (25E6 x, 25EA z) by the
; instance heading in 24CA..24CE, so later points can be taken relative to the rotated object.
_instance_vvars_head proc near
        mov     ax,word ptr ds:[25E6h]
        imul    word ptr ds:[24CAh]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[25EAh]
        imul    word ptr ds:[24CEh]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        mov     ax,word ptr ds:[25E6h]
        mov     word ptr ds:[25E6h],cx
        imul    word ptr ds:[24CCh]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[25EAh]
        imul    word ptr ds:[24CAh]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[25EAh],dx
        ret
_instance_vvars_head endp

; seg004_0849_368D  (+368D)
; instance_head: enter a sub-object turned by a heading. In: AX = the angle. Gets sin and cos
; (sincos, IMATH.ASM, called with DS = SS), rotates the eye (instance_vvars_head) and multiplies
; the view matrix by the rotation about the vertical axis, saturating each entry to 1.15. The
; interpreter saves and restores the matrix around the sub-object (INTERP.ASM, opcode 50h and
; the do_ihcall family).
_instance_head proc near
        mov     ax,ss
        mov     ds,ax
        call    far ptr _sincos
        mov     dx,es
        mov     ds,dx
        mov     word ptr ds:[24CCh],ax
        mov     word ptr ds:[24CAh],bx
        neg     ax
        mov     word ptr ds:[24CEh],ax
        call    _instance_vvars_head
        mov     ax,word ptr ds:[24CAh]
        imul    word ptr ds:[14B2h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24CEh]
        imul    word ptr ds:[14BEh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L36CF
        jg      short L36CC
        mov     cx,8000h
        jmp     short L36CF
L36CC:
        mov     cx,7FFFh
L36CF:
        mov     word ptr ds:[2494h],cx
        mov     ax,word ptr ds:[24CAh]
        imul    word ptr ds:[14B4h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24CEh]
        imul    word ptr ds:[14C0h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L36F9
        jg      short L36F6
        mov     cx,8000h
        jmp     short L36F9
L36F6:
        mov     cx,7FFFh
L36F9:
        mov     word ptr ds:[2496h],cx
        mov     ax,word ptr ds:[24CAh]
        imul    word ptr ds:[14B6h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24CEh]
        imul    word ptr ds:[14C2h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L3723
        jg      short L3720
        mov     cx,8000h
        jmp     short L3723
L3720:
        mov     cx,7FFFh
L3723:
        mov     word ptr ds:[2498h],cx
        mov     ax,word ptr ds:[24CCh]
        imul    word ptr ds:[14B2h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24CAh]
        imul    word ptr ds:[14BEh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L374D
        jg      short L374A
        mov     cx,8000h
        jmp     short L374D
L374A:
        mov     cx,7FFFh
L374D:
        mov     word ptr ds:[14BEh],cx
        mov     ax,word ptr ds:[24CCh]
        imul    word ptr ds:[14B4h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24CAh]
        imul    word ptr ds:[14C0h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L3777
        jg      short L3774
        mov     cx,8000h
        jmp     short L3777
L3774:
        mov     cx,7FFFh
L3777:
        mov     word ptr ds:[14C0h],cx
        mov     ax,word ptr ds:[24CCh]
        imul    word ptr ds:[14B6h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24CAh]
        imul    word ptr ds:[14C2h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L37A1
        jg      short L379E
        mov     cx,8000h
        jmp     short L37A1
L379E:
        mov     cx,7FFFh
L37A1:
        mov     word ptr ds:[14C2h],cx
        mov     ax,word ptr ds:[2494h]
        mov     word ptr ds:[14B2h],ax
        mov     ax,word ptr ds:[2496h]
        mov     word ptr ds:[14B4h],ax
        mov     ax,word ptr ds:[2498h]
        mov     word ptr ds:[14B6h],ax
        ret
_instance_head endp

; seg004_0849_37B8  (+37B8)
; instance_vvars_pitch and instance_pitch, instance_vvars_bank and instance_bank: as
; instance_vvars_head and instance_head for rotations about the other two axes (pitch and bank).
_instance_vvars_pitch proc near
        mov     ax,word ptr ds:[25E8h]
        imul    word ptr ds:[24D6h]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[25EAh]
        imul    word ptr ds:[24DAh]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        mov     ax,word ptr ds:[25E8h]
        mov     word ptr ds:[25E8h],dx
        imul    word ptr ds:[24D8h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[25EAh]
        imul    word ptr ds:[24D6h]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[25EAh],dx
        ret
_instance_vvars_pitch endp

; seg004_0849_37F5  (+37F5)
_instance_pitch proc near
        mov     ax,ss
        mov     ds,ax
        call    far ptr _sincos
        mov     dx,es
        mov     ds,dx
        mov     word ptr ds:[24D8h],ax
        mov     word ptr ds:[24D6h],bx
        neg     ax
        mov     word ptr ds:[24DAh],ax
        call    _instance_vvars_pitch
        mov     ax,word ptr ds:[24D6h]
        imul    word ptr ds:[14B8h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24DAh]
        imul    word ptr ds:[14BEh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L3837
        jg      short L3834
        mov     cx,8000h
        jmp     short L3837
L3834:
        mov     cx,7FFFh
L3837:
        mov     word ptr ds:[2494h],cx
        mov     ax,word ptr ds:[24D6h]
        imul    word ptr ds:[14BAh]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24DAh]
        imul    word ptr ds:[14C0h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L3861
        jg      short L385E
        mov     cx,8000h
        jmp     short L3861
L385E:
        mov     cx,7FFFh
L3861:
        mov     word ptr ds:[2496h],cx
        mov     ax,word ptr ds:[24D6h]
        imul    word ptr ds:[14BCh]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24DAh]
        imul    word ptr ds:[14C2h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L388B
        jg      short L3888
        mov     cx,8000h
        jmp     short L388B
L3888:
        mov     cx,7FFFh
L388B:
        mov     word ptr ds:[2498h],cx
        mov     ax,word ptr ds:[24D8h]
        imul    word ptr ds:[14B8h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24D6h]
        imul    word ptr ds:[14BEh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L38B5
        jg      short L38B2
        mov     cx,8000h
        jmp     short L38B5
L38B2:
        mov     cx,7FFFh
L38B5:
        mov     word ptr ds:[14BEh],cx
        mov     ax,word ptr ds:[24D8h]
        imul    word ptr ds:[14BAh]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24D6h]
        imul    word ptr ds:[14C0h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L38DF
        jg      short L38DC
        mov     cx,8000h
        jmp     short L38DF
L38DC:
        mov     cx,7FFFh
L38DF:
        mov     word ptr ds:[14C0h],cx
        mov     ax,word ptr ds:[24D8h]
        imul    word ptr ds:[14BCh]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24D6h]
        imul    word ptr ds:[14C2h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L3909
        jg      short L3906
        mov     cx,8000h
        jmp     short L3909
L3906:
        mov     cx,7FFFh
L3909:
        mov     word ptr ds:[14C2h],cx
        mov     ax,word ptr ds:[2494h]
        mov     word ptr ds:[14B8h],ax
        mov     ax,word ptr ds:[2496h]
        mov     word ptr ds:[14BAh],ax
        mov     ax,word ptr ds:[2498h]
        mov     word ptr ds:[14BCh],ax
        ret
_instance_pitch endp

; seg004_0849_3920  (+3920)
_instance_vvars_bank proc near
        mov     ax,word ptr ds:[25E8h]
        imul    word ptr ds:[24D0h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[25E6h]
        imul    word ptr ds:[24D4h]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        mov     ax,word ptr ds:[25E8h]
        mov     word ptr ds:[25E8h],dx
        imul    word ptr ds:[24D2h]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[25E6h]
        imul    word ptr ds:[24D0h]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        mov     word ptr ds:[25E6h],cx
        ret
_instance_vvars_bank endp

; seg004_0849_395D  (+395D)
_instance_bank proc near
        mov     ax,ss
        mov     ds,ax
        call    far ptr _sincos
        mov     dx,es
        mov     ds,dx
        mov     word ptr ds:[24D2h],ax
        mov     word ptr ds:[24D0h],bx
        neg     ax
        mov     word ptr ds:[24D4h],ax
        call    _instance_vvars_bank
        mov     ax,word ptr ds:[24D0h]
        imul    word ptr ds:[14B2h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24D2h]
        imul    word ptr ds:[14B8h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L399F
        jg      short L399C
        mov     cx,8000h
        jmp     short L399F
L399C:
        mov     cx,7FFFh
L399F:
        mov     word ptr ds:[2494h],cx
        mov     ax,word ptr ds:[24D0h]
        imul    word ptr ds:[14B4h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24D2h]
        imul    word ptr ds:[14BAh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L39C9
        jg      short L39C6
        mov     cx,8000h
        jmp     short L39C9
L39C6:
        mov     cx,7FFFh
L39C9:
        mov     word ptr ds:[2496h],cx
        mov     ax,word ptr ds:[24D0h]
        imul    word ptr ds:[14B6h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24D2h]
        imul    word ptr ds:[14BCh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L39F3
        jg      short L39F0
        mov     cx,8000h
        jmp     short L39F3
L39F0:
        mov     cx,7FFFh
L39F3:
        mov     word ptr ds:[2498h],cx
        mov     ax,word ptr ds:[24D4h]
        imul    word ptr ds:[14B2h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24D0h]
        imul    word ptr ds:[14B8h]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L3A1D
        jg      short L3A1A
        mov     cx,8000h
        jmp     short L3A1D
L3A1A:
        mov     cx,7FFFh
L3A1D:
        mov     word ptr ds:[14B8h],cx
        mov     ax,word ptr ds:[24D4h]
        imul    word ptr ds:[14B4h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24D0h]
        imul    word ptr ds:[14BAh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L3A47
        jg      short L3A44
        mov     cx,8000h
        jmp     short L3A47
L3A44:
        mov     cx,7FFFh
L3A47:
        mov     word ptr ds:[14BAh],cx
        mov     ax,word ptr ds:[24D4h]
        imul    word ptr ds:[14B6h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[24D0h]
        imul    word ptr ds:[14BCh]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L3A71
        jg      short L3A6E
        mov     cx,8000h
        jmp     short L3A71
L3A6E:
        mov     cx,7FFFh
L3A71:
        mov     word ptr ds:[14BCh],cx
        mov     ax,word ptr ds:[2494h]
        mov     word ptr ds:[14B2h],ax
        mov     ax,word ptr ds:[2496h]
        mov     word ptr ds:[14B4h],ax
        mov     ax,word ptr ds:[2498h]
        mov     word ptr ds:[14B6h],ax
        ret
_instance_bank endp

; seg004_0849_3A88  (+3A88)
; code_pnt: the clip codes of a view-space point. In: BX = x, CX = y, BP = z. Out: AL = 0 inside
; the frustum, else bits 1 (x < -z), 2 (x > z), 4 (y > z), 8 (y < -z), and 80h when z <= 0
; (behind the eye, with the side bits set from the signs). The four planes are CLIP.ASM's.
; Changes AX only.
_code_pnt proc near
        mov     ax,bp
        neg     ax
        jge     short L3AC9
        cmp     bx,ax
        jl      short L3AA1
        cmp     bx,bp
        jg      short L3AB2
        cmp     cx,bp
        jg      short L3AC3
        cmp     cx,ax
        jl      short L3AC6
        xor     al,al
        ret
L3AA1:
        cmp     cx,bp
        jg      short L3AAC
        cmp     cx,ax
        jl      short L3AAF
        mov     al,1
        ret
L3AAC:
        mov     al,5
        ret
L3AAF:
        mov     al,9
        ret
L3AB2:
        cmp     cx,bp
        jg      short L3ABD
        cmp     cx,ax
        jl      short L3AC0
        mov     al,2
        ret
L3ABD:
        mov     al,6
        ret
L3AC0:
        mov     al,0Ah
        ret
L3AC3:
        mov     al,4
        ret
L3AC6:
        mov     al,8
        ret
L3AC9:
        cmp     bx,ax
        jl      short L3AD4
        cmp     cx,bp
        jg      short L3AFA
        mov     al,8Ah
        ret
L3AD4:
        cmp     bx,bp
        jg      short L3ADF
        cmp     cx,bp
        jg      short L3AF0
        mov     al,89h
        ret
L3ADF:
        cmp     cx,bp
        jg      short L3AE6
        mov     al,8Bh
        ret
L3AE6:
        cmp     cx,ax
        jl      short L3AED
        mov     al,87h
        ret
L3AED:
        mov     al,8Fh
        ret
L3AF0:
        cmp     cx,ax
        jl      short L3AF7
        mov     al,85h
        ret
L3AF7:
        mov     al,8Dh
        ret
L3AFA:
        cmp     cx,ax
        jl      short L3B01
        mov     al,86h
        ret
L3B01:
        mov     al,8Eh
        ret
_code_pnt endp

; seg004_0849_3B04  (+3B04)
; mm9x9: the view matrix (written through ES) = the 3 x 3 matrix at BP times the one at SI, row
; by row through mm3x9_bpsi. BP is advanced by 18 bytes.
_mm9x9 proc near
        call    _mm3x9_bpsi
        mov     word ptr es:[14B2h],bx
        mov     word ptr es:[14B4h],cx
        mov     word ptr es:[14B6h],di
        add     bp,6
        call    _mm3x9_bpsi
        mov     word ptr es:[14B8h],bx
        mov     word ptr es:[14BAh],cx
        mov     word ptr es:[14BCh],di
        add     bp,6
        call    _mm3x9_bpsi
        mov     word ptr es:[14BEh],bx
        mov     word ptr es:[14C0h],cx
        mov     word ptr es:[14C2h],di
        ret
_mm9x9 endp

; seg004_0849_3B41  (+3B41)
; mm3x9_bpsi: a row times a matrix: BP = a row of three 1.15 words, SI = a 3 x 3 matrix by rows.
; Out: BX, CX, DI = the row's three products, saturated to +-8001h on overflow (8000h becomes
; 8001h, so negation is safe). The overflow fix-up of the third product reads and writes CX, not
; DI, so an overflow there overwrites the second result and leaves the third wrapped; FM Towns'
; mm3x9_bpsi saturates DI. Probably a slip in the DOS source.
_mm3x9_bpsi proc near
        mov     ax,word ptr [bp]
        imul    word ptr [si]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr [bp+2]
        imul    word ptr [si+6]
        add     bx,ax
        adc     cx,dx
        mov     ax,word ptr [bp+4]
        imul    word ptr [si+0Ch]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        jno     short L3B6E
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L3B6E:
        cmp     cx,8000h
        jne     short L3B77
        mov     cx,8001h
L3B77:
        push    cx
        mov     ax,word ptr [bp]
        imul    word ptr [si+2]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr [bp+2]
        imul    word ptr [si+8]
        add     bx,ax
        adc     cx,dx
        mov     ax,word ptr [bp+4]
        imul    word ptr [si+0Eh]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        jno     short L3BA6
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L3BA6:
        cmp     cx,8000h
        jne     short L3BAF
        mov     cx,8001h
L3BAF:
        mov     ax,word ptr [bp]
        imul    word ptr [si+4]
        mov     bx,ax
        mov     di,dx
        mov     ax,word ptr [bp+2]
        imul    word ptr [si+0Ah]
        add     bx,ax
        adc     di,dx
        mov     ax,word ptr [bp+4]
        imul    word ptr [si+10h]
        add     bx,ax
        adc     di,dx
        shl     bx,1
        rcl     di,1
        jno     short L3BDD
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L3BDD:
        cmp     di,8000h
        jne     short L3BE6
        mov     di,8001h
L3BE6:
        pop     bx
        ret
_mm3x9_bpsi endp

; seg004_0849_3BE8  (+3BE8)
; mm3x9t: as mm3x9_bpsi with the matrix at BP taken transposed (a row at SI times the columns of
; BP's matrix), with the same slip in the third product's overflow fix-up.
_mm3x9t proc near
        mov     ax,word ptr [si]
        imul    word ptr [bp]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr [si+2]
        imul    word ptr [bp+2]
        add     bx,ax
        adc     cx,dx
        mov     ax,word ptr [si+4]
        imul    word ptr [bp+4]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        jno     short L3C15
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L3C15:
        cmp     cx,8000h
        jne     short L3C1E
        mov     cx,8001h
L3C1E:
        push    cx
        mov     ax,word ptr [si]
        imul    word ptr [bp+6]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr [si+2]
        imul    word ptr [bp+8]
        add     bx,ax
        adc     cx,dx
        mov     ax,word ptr [si+4]
        imul    word ptr [bp+0Ah]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        jno     short L3C4C
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L3C4C:
        cmp     cx,8000h
        jne     short L3C55
        mov     cx,8001h
L3C55:
        mov     ax,word ptr [si]
        imul    word ptr [bp+0Ch]
        mov     bx,ax
        mov     di,dx
        mov     ax,word ptr [si+2]
        imul    word ptr [bp+0Eh]
        add     bx,ax
        adc     di,dx
        mov     ax,word ptr [si+4]
        imul    word ptr [bp+10h]
        add     bx,ax
        adc     di,dx
        shl     bx,1
        rcl     di,1
        jno     short L3C82
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L3C82:
        cmp     di,8000h
        jne     short L3C8B
        mov     di,8001h
L3C8B:
        pop     bx
        ret
_mm3x9t endp

; The divide overflow handlers. ss:[5A5h] holds the one int 0 dispatches to (FM overflow_ptr);
; each skips the faulting instruction and returns a saturated result.
L3C8D           dw      0               ; bp, saved while a handler runs
overflow_count  dw      0               ; FM overflow_count, cleared by in_scalebm

; seg004_0849_3C91  (+3C91)
; overflow_handler_reg: the int 0 handler for a 2-byte `idiv reg`: skips the instruction and
; returns AX = +-7FFFh by the sign of DX (DX the sign extension), and counts the overflow
; (do_scalebm abandons a sprite that overflowed).
_overflow_handler_reg proc far
        mov     word ptr cs:L3C8D,bp
        mov     bp,sp
        inc     word ptr [bp]
        inc     word ptr [bp]
        mov     ax,7FFFh
        test    dx,0FFFFh
        jns     short L3CA9
        neg     ax
L3CA9:
        cwd
        mov     bp,word ptr cs:L3C8D
        inc     word ptr cs:overflow_count
        iret
_overflow_handler_reg endp

; seg004_0849_3CB5  (+3CB5)
; overflow_handler_mem: for a 4-byte divide by a memory operand: skips it, AX = 7FFFh, DX = 0.
_overflow_handler_mem proc far
        mov     word ptr cs:L3C8D,bp
        mov     bp,sp
        add     word ptr [bp],4
        mov     ax,7FFFh
        xor     dx,dx
        mov     bp,word ptr cs:L3C8D
        iret
_overflow_handler_mem endp

; seg004_0849_3CCB  (+3CCB)
; overflow_handler_special_bp: for a 2-byte divide by BP: skips it and returns +-7FFFh by the
; sign of DX xor BP (the quotient's sign), DX = 0.
_overflow_handler_special_bp proc far
        mov     word ptr cs:L3C8D,bp
        mov     bp,sp
        inc     word ptr [bp]
        inc     word ptr [bp]
        mov     bp,word ptr cs:L3C8D
        mov     ax,dx
        xor     ax,bp
        cwd
        mov     ax,7FFFh
        xor     ax,dx
        sub     ax,dx
        xor     dx,dx
        iret
_overflow_handler_special_bp endp

SEG004_TEXT ends

        end
