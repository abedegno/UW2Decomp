; /* target: seg004_4980 */
; /* opts: /ml */
;
; The renderer's frame entry and its transforms. render_3d (called by seg019's cRender) sets up
; the projection from the clip window, builds the view matrix, and runs the render database
; through the model interpreter (INTERP.ASM). The rest is the geometry the interpreter's opcodes
; call: turning model points into view space (mini_xlate_rotate_pnt, load_xlate_rotate_pnt,
; mxmul), the frustum clip codes of a view-space point (code_pnt), the sub-object rotations by
; heading, pitch and bank (the instance_ routines), 3 x 3 matrix products (mm9x9, mm3x9_bpsi,
; mm3x9t), and the divide-overflow handlers the renderer installs on int 0.
;
; Fixed point: matrix entries are 1.15 (7FFFh is 1.0); a product of two is shifted left once and
; its high word kept. Points are 16-bit view or model coordinates.
;
; UW1 against UW2: the same routines, mostly the same code with UW1's data offsets, and these
; differences. render_3d starts at +4987 (the table's seg004_4980 is the 7-byte int 3 handler
; before it, as in UW2) and does more (see its comment). check_flat also keeps NOCLIP.ASM's
; handler set in step, and seg004_5040 swaps the opcode table's entries 78h..9Ch between the
; two sets. UW1 has more matrix and point routines (seg004_520F, 52FF, 5374, 5821, 5834, 5937,
; 5A83). seg004_51A3
; patches four places in SPHERE.ASM where UW2 patches one (modify_axis2). The overflow
; handlers save BP at L5BCD, and overflow_handler_reg counts into overflow_count after it.
;
; Self-modifying code: render_3d writes biasx and biasy into immediates in other modules
; (SMOOTH.ASM's seg004_2A4 .. 322, INTERP.ASM's modify_biasx, modify_bx2 ..., and NOCLIP.ASM's
; seg004_2173 .. 21DA), self_modify writes the eye and shift into the xlate routines
; here, and check_flat copies one of two mxmul bodies (L512B, L517E) over mxmul.
;
; name: UW2's (inferred there, from System Shock's 3D library instance.asm); the routine names
; are FM Towns', through UW2.
;
; .186, where UW2 has .386: UW1's seg004 has no 386 instruction and none of the nop padding
; that TASM's .386 mode puts after forward conditional jumps.
;
; SEG004 split into its original modules: this is module 7 of 9, +4980..+5C30 of the segment
; (file E9F0h), a module of the second library.
; match: evidence for the bounds: starts on a paragraph after 11 zero bytes following INTERP's
; last ret, and ends with 11 zero bytes before TMAPOPS.ASM.
; Its relocations are entries 2641-2661 of UW.EXE's relocation table. Entries 2639 (seg063:04D7,
; the segment word of the far pointer the int 0 handler jumps through) and 2640 (seg051:2862,
; a far pointer to seg053) come just before them, so they belong to this module's far data, as
; UW2's FD71 piece does to UW2's INSTANCE.ASM, defined before the code because those entries
; come before the code's.
;
; Its far data is two pieces, each taken from the far pointer that holds its relocation:
; FD72 (segment table entry 72, seg063, the system library's data segment) from 5624:04D5 to
; the segment's end at 0AB0h, and FD58 (entry 58, seg051, the 3D data segment) from
; 4723:2860 to B006h, where TMAPOPS.ASM's piece starts. Where each piece starts is not known:
; the bytes before them come from the EXE (tools/extract.py), as in UW2. FD72 starts with the
; far pointer the int 0 handler at seg063:04D0 jumps through: its offset is set at run time
; (mov word ptr ss:[4D5h],offset _overflow_handler_reg), its segment is seg004's, written as
; a pointer to seg004:0000 (uncmp_pal, EXPAND.ASM), as UW2 has it. After it come the tables
; IMATH.ASM reads (the sines at 04E0h, the arcsines at 0762h and arccosines at 0864h): no
; relocation ties them to any module, so whose they were is not known either, and the piece
; runs over them to the segment's end, as UW2's does. FD58 starts with the far pointer
; render_3d sets to seg053:000Eh (mov word ptr ds:[2860h],0Eh).
; name: the labels in the pieces are symbols.tsv's (provisional, UW2's where it has them).
;
; The routine comments are UW2's, with UW1's addresses, except where marked UW1.

; The model interpreter's opcode table: a word per opcode, the offset of its handler
; (INTERP.ASM), in seg051, the data segment while interpreting (UW2: 24F4h).
OPCODE_TABLE    equ     2738h

; The renderer's data in seg051 (DS while interpreting), as INTERP.ASM's header
; describes it. The names are ours, from the routines that use the data.
MAT_XX          equ     1602h   ; the view matrix, 1.15 fixed point: MAT_ab is input a's
MAT_XY          equ     1604h   ; coefficient in output b (mxmul: x' = x*MAT_XX + y*MAT_YX
MAT_XZ          equ     1606h   ; + z*MAT_ZX, and so on; z' is the depth)
MAT_YX          equ     1608h
MAT_YY          equ     160Ah
MAT_YZ          equ     160Ch
MAT_ZX          equ     160Eh
MAT_ZY          equ     1610h
MAT_ZZ          equ     1612h
PROJ_Y          equ     26B0h   ; the y projection scale (half the window height)
PROJ_X          equ     26B2h   ; the x projection scale (half the window width)
CENTRE_X        equ     26B4h   ; the screen centre
CENTRE_Y        equ     26B6h
OBJ_SHIFT       equ     2880h   ; the current object's coordinate shift
OBJ_X           equ     2886h   ; the current object's origin, eye-relative and negated
OBJ_Y           equ     2888h
OBJ_Z           equ     288Ah

        .186

        extrn   seg048:far
        extrn   seg051:far
        extrn   seg063:far
        extrn   _seg019_C10:far
        extrn   seg053:far
        extrn   _SquareRoot_seg019_A30:far
        extrn   _sincos:far
        extrn   _uncmp_pal:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

; seg063 from 04D5h to its end
FD72    segment byte public 'FAR_DATA'
        dd      _uncmp_pal
        db      00h,00h,00h,00h,00h,00h,00h,00h,00h,24h,03h,48h,06h,6Bh,09h,8Ch
        db      0Ch,0ABh,0Fh,0C8h,12h,0E2h,15h,0F9h,18h,0Ch,1Ch,1Ah,1Fh,24h,22h,28h
        db      25h,27h,28h,1Fh,2Bh,11h,2Eh,0FCh,30h,0DFh,33h,0BAh,36h,8Dh,39h,57h
        db      3Ch,17h,3Fh,0CEh,41h,7Bh,44h,1Dh,47h,0B4h,49h,40h,4Ch,0C0h,4Eh,34h
        db      51h,9Bh,53h,0F6h,55h,43h,58h,82h,5Ah,0B4h,5Ch,0D7h,5Eh,0ECh,60h,0F2h
        db      62h,0E9h,64h,0D0h,66h,0A7h,68h,6Eh,6Ah,24h,6Ch,0CAh,6Dh,5Fh,6Fh,0E3h
        db      70h,55h,72h,0B6h,73h,05h,75h,42h,76h,6Ch,77h,85h,78h,8Ah,79h,7Dh
        db      7Ah,5Dh,7Bh,2Ah,7Ch,0E4h,7Ch,8Ah,7Dh,1Eh,7Eh,9Dh,7Eh,0Ah,7Fh,62h
        db      7Fh,0A7h,7Fh,0D9h,7Fh,0F6h,7Fh,0FFh,7Fh,0F6h,7Fh,0D9h,7Fh,0A7h,7Fh,62h
        db      7Fh,0Ah,7Fh,9Dh,7Eh,1Eh,7Eh,8Ah,7Dh,0E4h,7Ch,2Ah,7Ch,5Dh,7Bh,7Dh
        db      7Ah,8Ah,79h,85h,78h,6Ch,77h,42h,76h,05h,75h,0B6h,73h,55h,72h,0E3h
        db      70h,5Fh,6Fh,0CAh,6Dh,24h,6Ch,6Eh,6Ah,0A7h,68h,0D0h,66h,0E9h,64h,0F2h
        db      62h,0ECh,60h,0D7h,5Eh,0B4h,5Ch,82h,5Ah,43h,58h,0F6h,55h,9Bh,53h,34h
        db      51h,0C0h,4Eh,40h,4Ch,0B4h,49h,1Dh,47h,7Bh,44h,0CEh,41h,17h,3Fh,57h
        db      3Ch,8Dh,39h,0BAh,36h,0DFh,33h,0FCh,30h,11h,2Eh,1Fh,2Bh,27h,28h,28h
        db      25h,24h,22h,1Ah,1Fh,0Ch,1Ch,0F9h,18h,0E2h,15h,0C8h,12h,0ABh,0Fh,8Ch
        db      0Ch,6Bh,09h,48h,06h,24h,03h,00h,00h,0DCh,0FCh,0B8h,0F9h,95h,0F6h,74h
        db      0F3h,55h,0F0h,38h,0EDh,1Eh,0EAh,07h,0E7h,0F4h,0E3h,0E6h,0E0h,0DCh,0DDh,0D8h
        db      0DAh,0D9h,0D7h,0E1h,0D4h,0EFh,0D1h,04h,0CFh,21h,0CCh,46h,0C9h,73h,0C6h,0A9h
        db      0C3h,0E9h,0C0h,32h,0BEh,85h,0BBh,0E3h,0B8h,4Ch,0B6h,0C0h,0B3h,40h,0B1h,0CCh
        db      0AEh,65h,0ACh,0Ah,0AAh,0BDh,0A7h,7Eh,0A5h,4Ch,0A3h,29h,0A1h,14h,9Fh,0Eh
        db      9Dh,17h,9Bh,30h,99h,59h,97h,92h,95h,0DCh,93h,36h,92h,0A1h,90h,1Dh
        db      8Fh,0ABh,8Dh,4Ah,8Ch,0FBh,8Ah,0BEh,89h,94h,88h,7Bh,87h,76h,86h,83h
        db      85h,0A3h,84h,0D6h,83h,1Ch,83h,76h,82h,0E2h,81h,63h,81h,0F6h,80h,9Eh
        db      80h,59h,80h,27h,80h,0Ah,80h,01h,80h,0Ah,80h,27h,80h,59h,80h,9Eh
        db      80h,0F6h,80h,63h,81h,0E2h,81h,76h,82h,1Ch,83h,0D6h,83h,0A3h,84h,83h
        db      85h,76h,86h,7Bh,87h,94h,88h,0BEh,89h,0FBh,8Ah,4Ah,8Ch,0ABh,8Dh,1Dh
        db      8Fh,0A1h,90h,36h,92h,0DCh,93h,92h,95h,59h,97h,30h,99h,17h,9Bh,0Eh
        db      9Dh,14h,9Fh,29h,0A1h,4Ch,0A3h,7Eh,0A5h,0BDh,0A7h,0Ah,0AAh,65h,0ACh,0CCh
        db      0AEh,40h,0B1h,0C0h,0B3h,4Ch,0B6h,0E3h,0B8h,85h,0BBh,32h,0BEh,0E9h,0C0h,0A9h
        db      0C3h,73h,0C6h,46h,0C9h,21h,0CCh,04h,0CFh,0EFh,0D1h,0E1h,0D4h,0D9h,0D7h,0D8h
        db      0DAh,0DCh,0DDh,0E6h,0E0h,0F4h,0E3h,07h,0E7h,1Eh,0EAh,38h,0EDh,55h,0F0h,74h
        db      0F3h,95h,0F6h,0B8h,0F9h,0DCh,0FCh,00h,00h,24h,03h,48h,06h,6Bh,09h,8Ch
        db      0Ch,0ABh,0Fh,0C8h,12h,0E2h,15h,0F9h,18h,0Ch,1Ch,1Ah,1Fh,24h,22h,28h
        db      25h,27h,28h,1Fh,2Bh,11h,2Eh,0FCh,30h,0DFh,33h,0BAh,36h,8Dh,39h,57h
        db      3Ch,17h,3Fh,0CEh,41h,7Bh,44h,1Dh,47h,0B4h,49h,40h,4Ch,0C0h,4Eh,34h
        db      51h,9Bh,53h,0F6h,55h,43h,58h,82h,5Ah,0B4h,5Ch,0D7h,5Eh,0ECh,60h,0F2h
        db      62h,0E9h,64h,0D0h,66h,0A7h,68h,6Eh,6Ah,24h,6Ch,0CAh,6Dh,5Fh,6Fh,0E3h
        db      70h,55h,72h,0B6h,73h,05h,75h,42h,76h,6Ch,77h,85h,78h,8Ah,79h,7Dh
        db      7Ah,5Dh,7Bh,2Ah,7Ch,0E4h,7Ch,8Ah,7Dh,1Eh,7Eh,9Dh,7Eh,0Ah,7Fh,62h
        db      7Fh,0A7h,7Fh,0D9h,7Fh,0F6h,7Fh,0FFh,7Fh,00h,00h,51h,00h,0A2h,00h,0F4h
        db      00h,46h,01h,97h,01h,0E9h,01h,3Ah,02h,8Ch,02h,0DDh,02h,2Fh,03h,81h
        db      03h,0D3h,03h,25h,04h,77h,04h,0C9h,04h,1Bh,05h,6Dh,05h,0BFh,05h,12h
        db      06h,64h,06h,0B7h,06h,09h,07h,5Ch,07h,0AFh,07h,02h,08h,55h,08h,0A8h
        db      08h,0FCh,08h,4Fh,09h,0A3h,09h,0F7h,09h,4Bh,0Ah,9Fh,0Ah,0F4h,0Ah,48h
        db      0Bh,9Dh,0Bh,0F2h,0Bh,47h,0Ch,9Dh,0Ch,0F3h,0Ch,48h,0Dh,9Fh,0Dh,0F5h
        db      0Dh,4Ch,0Eh,0A3h,0Eh,0FAh,0Eh,51h,0Fh,0A9h,0Fh,01h,10h,59h,10h,0B2h
        db      10h,0Bh,11h,64h,11h,0BEh,11h,18h,12h,73h,12h,0CDh,12h,29h,13h,84h
        db      13h,0E0h,13h,3Dh,14h,9Ah,14h,0F7h,14h,55h,15h,0B3h,15h,12h,16h,71h
        db      16h,0D1h,16h,32h,17h,93h,17h,0F4h,17h,57h,18h,0BAh,18h,1Dh,19h,81h
        db      19h,0E6h,19h,4Ch,1Ah,0B2h,1Ah,19h,1Bh,81h,1Bh,0EAh,1Bh,54h,1Ch,0BEh
        db      1Ch,2Ah,1Dh,96h,1Dh,04h,1Eh,73h,1Eh,0E2h,1Eh,53h,1Fh,0C5h,1Fh,38h
        db      20h,0ADh,20h,23h,21h,9Ah,21h,13h,22h,8Dh,22h,09h,23h,87h,23h,07h
        db      24h,88h,24h,0Ch,25h,91h,25h,19h,26h,0A4h,26h,31h,27h,0C1h,27h,54h
        db      28h,0EAh,28h,84h,29h,21h,2Ah,0C2h,2Ah,68h,2Bh,13h,2Ch,0C4h,2Ch,7Ah
        db      2Dh,37h,2Eh,0FCh,2Eh,0C9h,2Fh,0A1h,30h,84h,31h,76h,32h,79h,33h,93h
        db      34h,0C9h,35h,29h,37h,0C9h,38h,0E7h,3Ah,00h,40h,00h,40h,0AEh,3Fh,5Dh
        db      3Fh,0Bh,3Fh,0B9h,3Eh,68h,3Eh,16h,3Eh,0C5h,3Dh,73h,3Dh,22h,3Dh,0D0h
        db      3Ch,7Eh,3Ch,2Ch,3Ch,0DAh,3Bh,88h,3Bh,36h,3Bh,0E4h,3Ah,92h,3Ah,40h
        db      3Ah,0EDh,39h,9Bh,39h,48h,39h,0F6h,38h,0A3h,38h,50h,38h,0FDh,37h,0AAh
        db      37h,57h,37h,03h,37h,0B0h,36h,5Ch,36h,08h,36h,0B4h,35h,60h,35h,0Bh
        db      35h,0B7h,34h,62h,34h,0Dh,34h,0B8h,33h,62h,33h,0Ch,33h,0B7h,32h,60h
        db      32h,0Ah,32h,0B3h,31h,5Ch,31h,05h,31h,0AEh,30h,56h,30h,0FEh,2Fh,0A6h
        db      2Fh,4Dh,2Fh,0F4h,2Eh,9Bh,2Eh,41h,2Eh,0E7h,2Dh,8Ch,2Dh,32h,2Dh,0D6h
        db      2Ch,7Bh,2Ch,1Fh,2Ch,0C2h,2Bh,65h,2Bh,08h,2Bh,0AAh,2Ah,4Ch,2Ah,0EDh
        db      29h,8Eh,29h,2Eh,29h,0CDh,28h,6Ch,28h,0Bh,28h,0A8h,27h,45h,27h,0E2h
        db      26h,7Eh,26h,19h,26h,0B3h,25h,4Dh,25h,0E6h,24h,7Eh,24h,15h,24h,0ABh
        db      23h,41h,23h,0D5h,22h,69h,22h,0FBh,21h,8Ch,21h,1Dh,21h,0ACh,20h,3Ah
        db      20h,0C7h,1Fh,52h,1Fh,0DCh,1Eh,65h,1Eh,0ECh,1Dh,72h,1Dh,0F6h,1Ch,78h
        db      1Ch,0F8h,1Bh,77h,1Bh,0F3h,1Ah,6Eh,1Ah,0E6h,19h,5Bh,19h,0CEh,18h,3Eh
        db      18h,0ABh,17h,15h,17h,7Bh,16h,0DEh,15h,3Dh,15h,97h,14h,0ECh,13h,3Bh
        db      13h,85h,12h,0C8h,11h,03h,11h,36h,10h,5Eh,0Fh,7Bh,0Eh,89h,0Dh,86h
        db      0Ch,6Ch,0Bh,36h,0Ah,0D6h,08h,36h,07h,18h,05h,00h,00h,00h,00h,00h
        db      00h,00h,00h,00h,00h,00h,00h,00h,00h,36h,00h,01h
        db      35h dup (0)
        db      1Dh,00h,0FFh,0FFh,00h,00h,00h,00h,00h,00h,01h,00h,00h,00h,3Dh,6Ah
        db      00h,00h,00h,00h,00h,00h,00h,00h,0C7h,00h,3Fh,01h,00h,00h,0C0h,09h
        db      0E6h dup (0)
FD72    ends

; seg051 from 2860h to B006h, where TMAPOPS.ASM's piece starts
FD58    segment byte public 'FAR_DATA'
        public  _seg051_28A0, __dblen, _bmhgtoff, _bmsegoff
        dd      seg053+0Eh
        db      00h,00h,06h,00h,00h,00h,0Ch,00h,0FFh
        db      1Fh dup (0)
        db      1Dh,00h,2Fh
        db      11h dup (0)
_seg051_28A0 label byte
        db      0B6h,4Ah,06h,40h,38h,29h,0FCh,25h,02h,00h,70h,3Bh,86h,34h,60h,2Ch
        db      0BCh,45h,16h,42h,1Ch,43h,0Eh,00h,34h,48h,34h,48h,0EEh,49h,08h,4Ah
        db      22h,4Ah,22h,4Ah,22h,4Ah,0D8h,43h,22h,4Ah,0B4h,4Ah,66h,4Ah,0F6h,44h
        db      0A8h,09h,8Ah,1Bh,06h,13h,0D4h,16h,30h,22h,00h,00h,0B8h,4Ah,0B8h,4Ah
        db      0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah
        db      0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah
        db      0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah
        db      0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah,0B8h,4Ah
        db      63h,00h,30h,00h,50h
        db      17h dup (0)
        db      0DEh,00h,00h,00h,66h,00h,0DDh,00h,34h,00h,7Ah,00h,00h,00h,00h,00h
        db      6Eh,00h,00h,00h,78h,00h,00h,00h,33h,00h,6Eh,00h,1Ah,00h,00h,00h
        db      82h,00h,03h,00h,00h,00h,0E7h,0FFh,0F6h,0FFh,20h,00h,0DFh,0FFh,12h,00h
        db      18h,00h,0D7h,0FFh,1Ah,00h,00h,00h,8Ah,00h,00h,00h,14h,00h,18h,00h
        db      90h,00h,0E6h,0FFh,5Eh,00h,00h,00h,20h,00h,8Ah,00h,20h,00h,14h,00h
        db      28h,00h,88h,00h,28h,00h,26h,00h,30h,00h,88h,00h,20h,00h,26h,00h
        db      38h,00h,90h,00h,09h,00h,80h,00h,18h,00h,40h,00h,8Ah,00h,40h,00h
        db      0ECh,0FFh,48h,00h,86h,00h,48h,00h,20h,00h,50h,00h,86h,00h,40h,00h
        db      20h,00h,58h,00h,86h,00h,30h,00h,66h,00h,60h,00h,86h,00h,38h,00h
        db      66h,00h,68h,00h,86h,00h,28h,00h,66h,00h,70h,00h,86h,00h,20h,00h
        db      66h,00h,78h,00h,86h,00h,00h,00h,32h,00h,80h,00h,90h,00h,24h,00h
        db      67h,00h,00h,00h,88h,00h,8Ah,00h,88h,00h,14h,00h,90h,00h,86h,00h
        db      18h,00h,32h,00h,98h,00h,90h,00h,31h,00h,96h,00h,00h,00h,0A0h,00h
        db      8Ah,00h,0A0h,00h,14h,00h,0A8h,00h,88h,00h,80h,00h,9Fh,00h,0B0h,00h
        db      88h,00h,98h,00h,9Fh,00h,0B8h,00h,88h,00h,80h,00h,0A8h,00h,0C0h,00h
        db      88h,00h,98h,00h,0A8h,00h,0C8h,00h,90h,00h,2Dh,00h,0B3h,00h,00h,00h
        db      0D0h,00h,8Ah,00h,0D0h,00h,14h,00h,0D8h,00h,88h,00h,90h,00h,52h,00h
        db      0E0h,00h,88h,00h,88h,00h,52h,00h,0E8h,00h,90h,00h,19h,00h,0BDh,00h
        db      00h,00h,0F0h,00h,8Ah,00h,0F0h,00h,14h,00h,0F8h,00h,88h,00h,0F0h,00h
        db      0CEh,0FFh,00h,01h,88h,00h,0F8h,00h,0CEh,0FFh,08h,01h,90h,00h,1Fh,00h
        db      94h,00h,18h,00h,10h,01h,8Ah,00h,10h,01h,0ECh,0FFh,18h,01h,90h,00h
        db      22h,00h,9Ch,00h,18h,00h,20h,01h,8Ah,00h,20h,01h,0ECh,0FFh,28h,01h
        db      88h,00h,20h,01h,07h,00h,30h,01h,88h,00h,28h,01h,07h,00h,38h,01h
        db      90h,00h,20h,00h,0A7h,00h,18h,00h,40h,01h,8Ah,00h,40h,01h,0ECh,0FFh
        db      48h,01h,90h,00h,1Dh,00h,0A9h,00h,18h,00h,50h,01h,8Ah,00h,50h,01h
        db      0ECh,0FFh,58h,01h,88h,00h,0F8h,00h,0EDh,0FFh,60h,01h,88h,00h,0F0h,00h
        db      0EDh,0FFh,68h,01h,86h,00h,48h,01h,0F1h,0FFh,70h,01h,86h,00h,40h,01h
        db      0F1h,0FFh,78h,01h,86h,00h,50h,01h,0F8h,0FFh,80h,01h,86h,00h,58h,01h
        db      0F8h,0FFh,88h,01h,86h,00h,38h,01h,0EEh,0FFh,90h,01h,86h,00h,30h,01h
        db      0EEh,0FFh,98h,01h,86h,00h,28h,01h,0EEh,0FFh,0A0h,01h,86h,00h,20h,01h
        db      0EEh,0FFh,0A8h,01h,86h,00h,18h,01h,0F4h,0FFh,0B0h,01h,86h,00h,10h,01h
        db      0F4h,0FFh,0B8h,01h,86h,00h,0D0h,00h,0D7h,0FFh,0C0h,01h,86h,00h,0D8h,00h
        db      0D7h,0FFh,0C8h,01h,86h,00h,0E0h,00h,0EAh,0FFh,0D0h,01h,86h,00h,0E8h,00h
        db      0EAh,0FFh,0D8h,01h,88h,00h,00h,00h,0A8h,00h,0E0h,01h,88h,00h,18h,00h
        db      0A8h,00h,0E8h,01h,88h,00h,00h,00h,9Fh,00h,0F0h,01h,88h,00h,18h,00h
        db      9Fh,00h,0F8h,01h,86h,00h,0A0h,00h,0D0h,0FFh,00h,02h,86h,00h,0A8h,00h
        db      0D0h,0FFh,08h,02h,86h,00h,08h,00h,42h,00h,10h,02h,8Ah,00h,08h,00h
        db      0DCh,0FFh,18h,02h,8Ah,00h,10h,02h,0DCh,0FFh,20h,02h,86h,00h,10h,00h
        db      52h,00h,28h,02h,8Ah,00h,10h,00h,0CCh,0FFh,30h,02h,8Ah,00h,28h,02h
        db      0CCh,0FFh,38h,02h,86h,00h,90h,00h,0EAh,0FFh,40h,02h,86h,00h,88h,00h
        db      0EAh,0FFh,48h,02h,88h,00h,78h,00h,1Ah,00h,50h,02h,88h,00h,70h,00h
        db      1Ah,00h,58h,02h,90h,00h,03h,00h,14h,00h,00h,00h,60h,02h,86h,00h
        db      60h,02h,2Ch,00h,68h,02h,86h,00h,18h,02h,07h,00h,70h,02h,86h,00h
        db      18h,02h,3Bh,00h,78h,02h,0D4h,00h,50h,00h,20h,29h,00h,00h,05h,18h
        db      00h,05h,20h,00h,02h,28h,00h,03h,30h,00h,05h,38h,00h,04h,40h,00h
        db      04h,48h,00h,03h,50h,00h,04h,58h,00h,05h,60h,00h,05h,68h,00h,04h
        db      70h,00h,03h,78h,00h,03h,80h,00h,05h,88h,00h,03h,90h,00h,03h,98h
        db      00h,05h,0A0h,00h,03h,0A8h,00h,03h,0B0h,00h,03h,0B8h,00h,04h,0C0h,00h
        db      04h,0C8h,00h,05h,0D0h,00h,05h,0D8h,00h,06h,0E0h,00h,07h,0E8h,00h,06h
        db      0F0h,00h,06h,0F8h,00h,07h,00h,01h,04h,08h,01h,05h,10h,01h,04h,18h
        db      01h,04h,20h,01h,04h,28h,01h,03h,30h,01h,03h,38h,01h,03h,40h,01h
        db      02h,48h,01h,01h,50h,01h,00h,58h,01h,01h,60h,01h,01h,68h,01h,01h
        db      70h,01h,02h,78h,01h,03h,80h,01h,01h,88h,01h,01h,90h,01h,03h,98h
        db      01h,04h,0A0h,01h,04h,0A8h,01h,05h,0B0h,01h,04h,0B8h,01h,05h,0C0h,01h
        db      05h,0C8h,01h,05h,0D0h,01h,06h,0D8h,01h,06h,0E0h,01h,03h,0E8h,01h,04h
        db      0F0h,01h,03h,0F8h,01h,03h,00h,02h,03h,08h,02h,03h,08h,00h,06h,10h
        db      02h,06h,18h,02h,05h,20h,02h,05h,10h,00h,05h,28h,02h,05h,30h,02h
        db      04h,38h,02h,05h,40h,02h,03h,48h,02h,03h,50h,02h,03h,58h,02h,04h
        db      60h,02h,04h,68h,02h,04h,70h,02h,05h,78h,02h,05h,62h,00h,0Eh,00h
        db      0FEh,0B8h,0F9h,0FFh,7Dh,95h,39h,0FFh,0D6h,00h,7Eh,00h,04h,00h,48h,01h
        db      40h,01h,50h,01h,58h,01h,62h,00h,0Eh,00h,0EFh,0E0h,0FCh,0FFh,0BCh,83h
        db      37h,0FFh,0D6h,00h,7Eh,00h,04h,00h,58h,01h,50h,01h,60h,01h,68h,01h
        db      62h,00h,0Eh,00h,44h,39h,04h,00h,77h,8Dh,37h,0FFh,0D6h,00h,7Eh,00h
        db      04h,00h,88h,01h,80h,01h,78h,01h,70h,01h,62h,00h,0Eh,00h,11h,1Fh
        db      04h,00h,0BCh,83h,37h,0FFh,0D6h,00h,7Eh,00h,04h,00h,88h,01h,68h,01h
        db      60h,01h,80h,01h,10h,00h,88h,0Eh,33h,00h,2Dh,7Fh,5Ch,0FFh,2Eh,03h
        db      9Ah,00h,64h,00h,0Eh,00h,01h,80h,33h,00h,0D6h,00h,7Eh,00h,04h,00h
        db      20h,00h,28h,00h,30h,00h,38h,00h,64h,00h,12h,00h,0FFh,7Fh,0CDh,0FFh
        db      0D6h,00h,7Eh,00h,06h,00h,50h,02h,68h,00h,60h,00h,58h,02h,70h,00h
        db      78h,00h,62h,00h,0Eh,00h,02h,47h,0F5h,0FFh,83h,6Ah,27h,0FFh,0D6h,00h
        db      7Eh,00h,04h,00h,0E8h,00h,0E0h,00h,0D8h,00h,0D0h,00h,62h,00h,0Eh,00h
        db      0C2h,2Bh,0F5h,0FFh,56h,78h,27h,0FFh,0D6h,00h,7Eh,00h,04h,00h,0E8h,00h
        db      0F0h,00h,0F8h,00h,0E0h,00h,62h,00h,0Eh,00h,23h,0BEh,15h,00h,0C6h,6Dh
        db      2Dh,0FFh,0D6h,00h,7Eh,00h,04h,00h,0C0h,01h,0C8h,01h,0D0h,01h,0D8h,01h
        db      62h,00h,0Eh,00h,3Eh,0D4h,00h,00h,56h,78h,23h,0FFh,0D6h,00h,7Eh,00h
        db      04h,00h,0F0h,00h,0D8h,01h,0D0h,01h,0F8h,00h,00h,00h,62h,00h,0Eh,00h
        db      88h,0Eh,33h,00h,2Dh,7Fh,5Ch,0FFh,0D6h,00h,7Eh,00h,04h,00h,38h,00h
        db      30h,00h,40h,00h,48h,00h,62h,00h,0Eh,00h,78h,0F1h,0F0h,0FFh,2Dh,7Fh
        db      60h,0FFh,0D6h,00h,7Eh,00h,04h,00h,50h,00h,58h,00h,60h,00h,68h,00h
        db      62h,00h,0Eh,00h,7Dh,95h,00h,00h,02h,47h,55h,0FFh,0D6h,00h,7Eh,00h
        db      04h,00h,00h,01h,08h,01h,10h,01h,18h,01h,62h,00h,0Eh,00h,19h,88h
        db      0FAh,0FFh,0F6h,2Ch,4Ch,0FFh,0D6h,00h,7Eh,00h,04h,00h,18h,01h,10h,01h
        db      20h,01h,28h,01h,64h,00h,0Eh,00h,01h,80h,0F7h,0FFh,0D6h,00h,7Eh,00h
        db      04h,00h,28h,01h,20h,01h,30h,01h,38h,01h,62h,00h,0Eh,00h,77h,8Dh
        db      0F7h,0FFh,0BCh,0C6h,3Dh,0FFh,0D6h,00h,7Eh,00h,04h,00h,38h,01h,30h,01h
        db      40h,01h,48h,01h,62h,00h,0Eh,00h,44h,7Ch,08h,00h,0EFh,0E0h,39h,0FFh
        db      0D6h,00h,7Eh,00h,04h,00h,70h,01h,78h,01h,98h,01h,90h,01h,64h,00h
        db      0Eh,00h,0FFh,7Fh,09h,00h,0D6h,00h,7Eh,00h,04h,00h,90h,01h,98h,01h
        db      0A8h,01h,0A0h,01h,62h,00h,0Eh,00h,0E7h,77h,09h,00h,0F6h,2Ch,44h,0FFh
        db      0D6h,00h,7Eh,00h,04h,00h,0A0h,01h,0A8h,01h,0B8h,01h,0B0h,01h,62h,00h
        db      0Eh,00h,83h,6Ah,06h,00h,02h,47h,4Ch,0FFh,0D6h,00h,7Eh,00h,04h,00h
        db      0B0h,01h,0B8h,01h,08h,01h,00h,01h,62h,00h,0Eh,00h,0AAh,87h,19h,00h
        db      0C2h,2Bh,38h,0FFh,0D6h,00h,7Eh,00h,04h,00h,0E8h,01h,0C8h,01h,0C0h,01h
        db      0E0h,01h,64h,00h,0Eh,00h,01h,80h,19h,00h,0D6h,00h,7Eh,00h,04h,00h
        db      0F8h,01h,0E8h,01h,0E0h,01h,0F0h,01h,62h,00h,0Eh,00h,0C9h,80h,18h,00h
        db      0DEh,0F1h,4Ah,0FFh,0D6h,00h,7Eh,00h,04h,00h,08h,02h,0F8h,01h,0F0h,01h
        db      00h,02h,62h,00h,0Eh,00h,0B0h,87h,10h,00h,40h,0D4h,60h,0FFh,0D6h,00h
        db      7Eh,00h,04h,00h,40h,00h,08h,02h,00h,02h,48h,00h,68h,00h,3Ch,00h
        db      01h,80h,0Ah,00h,0BCh,00h,20h,29h,03h,00h,40h,00h,7Eh,00h,18h,00h
        db      0B0h,01h,48h,00h,00h,02h,0F0h,01h,0E0h,01h,0C0h,01h,0D8h,01h,0F0h,00h
        db      0E8h,00h,0D0h,00h,0C0h,00h,0B0h,00h,0A0h,00h,50h,00h,18h,01h,28h,01h
        db      38h,01h,48h,01h,58h,01h,68h,01h,88h,01h,70h,01h,90h,01h,0A0h,01h
        db      68h,00h,3Ah,00h,0FFh,7Fh,0F6h,0FFh,0BCh,00h,20h,29h,04h,00h,40h,00h
        db      7Eh,00h,17h,00h,10h,01h,58h,00h,0A8h,00h,0B8h,00h,0C8h,00h,0D8h,00h
        db      0E0h,00h,0F8h,00h,0D0h,01h,0C8h,01h,0E8h,01h,0F8h,01h,08h,02h,40h,00h
        db      0B8h,01h,0A8h,01h,98h,01h,78h,01h,80h,01h,60h,01h,40h,01h,30h,01h
        db      20h,01h,62h,00h,0Eh,00h,50h,78h,0F0h,0FFh,40h,0D4h,60h,0FFh,0D6h,00h
        db      7Eh,00h,04h,00h,50h,00h,0A0h,00h,0A8h,00h,58h,00h,62h,00h,0Eh,00h
        db      37h,7Fh,0E8h,0FFh,0DEh,0F1h,4Ah,0FFh,0D6h,00h,7Eh,00h,04h,00h,0A0h,00h
        db      0B0h,00h,0B8h,00h,0A8h,00h,64h,00h,0Eh,00h,0FFh,7Fh,0E7h,0FFh,0D6h,00h
        db      7Eh,00h,04h,00h,0B0h,00h,0C0h,00h,0C8h,00h,0B8h,00h,62h,00h,0Eh,00h
        db      8Dh,74h,0E7h,0FFh,0FAh,34h,38h,0FFh,0D6h,00h,7Eh,00h,04h,00h,0C0h,00h
        db      0D0h,00h,0D8h,00h,0C8h,00h,68h,00h,1Ah,00h,01h,80h,0Ah,00h,0BCh,00h
        db      20h,29h,03h,00h,40h,00h,7Eh,00h,07h,00h,48h,00h,0B0h,01h,00h,01h
        db      18h,01h,50h,00h,68h,00h,50h,02h,68h,00h,1Ch,00h,0FFh,7Fh,0F6h,0FFh
        db      0BCh,00h,20h,29h,04h,00h,40h,00h,7Eh,00h,08h,00h,58h,02h,60h,00h
        db      58h,00h,10h,01h,08h,01h,0B8h,01h,40h,00h,30h,00h,00h,00h,62h,00h
        db      0Eh,00h,0E8h,0E3h,0F5h,0FFh,21h,83h,79h,0FFh,0D6h,00h,7Eh,00h,04h,00h
        db      88h,00h,78h,00h,70h,00h,90h,00h,62h,00h,0Eh,00h,18h,1Ch,33h,00h
        db      21h,83h,82h,0FFh,0D6h,00h,7Eh,00h,04h,00h,20h,00h,48h,02h,40h,02h
        db      28h,00h,0Ch,00h,81h,5Ah,0E8h,0FFh,81h,5Ah,0EEh,0FFh,9Eh,00h,02h,00h
        db      00h,00h,5Eh,00h,0Eh,00h,81h,5Ah,0E8h,0FFh,81h,5Ah,0EEh,0FFh,0D6h,00h
        db      7Eh,00h,04h,00h,08h,00h,10h,02h,98h,00h,18h,00h,62h,00h,0Eh,00h
        db      2Bh,81h,19h,00h,3Dh,11h,0E0h,0FFh,0D6h,00h,7Eh,00h,04h,00h,18h,00h
        db      40h,02h,48h,02h,60h,02h,62h,00h,0Eh,00h,0D5h,7Eh,0EAh,0FFh,3Dh,11h
        db      0CCh,0FFh,0D6h,00h,7Eh,00h,04h,00h,68h,02h,88h,00h,90h,00h,98h,00h
        db      68h,00h,1Eh,00h,01h,80h,0Ah,00h,0BCh,00h,20h,29h,03h,00h,40h,00h
        db      7Eh,00h,09h,00h,60h,02h,48h,02h,20h,00h,38h,00h,48h,00h,50h,02h
        db      78h,00h,88h,00h,68h,02h,68h,00h,1Ch,00h,0FFh,7Fh,0F6h,0FFh,0BCh,00h
        db      20h,29h,04h,00h,40h,00h,7Eh,00h,08h,00h,98h,00h,90h,00h,70h,00h
        db      58h,02h,30h,00h,28h,00h,40h,02h,18h,00h,00h,00h,0Ch,00h,7Ah,28h
        db      00h,00h,92h,86h,1Ah,00h,72h,00h,50h,00h,5Eh,00h,0Eh,00h,7Ah,28h
        db      00h,00h,6Eh,79h,0E6h,0FFh,0D6h,00h,7Eh,00h,04h,00h,28h,02h,10h,02h
        db      08h,00h,10h,00h,62h,00h,0Eh,00h,92h,86h,21h,00h,7Ah,28h,0E8h,0FFh
        db      0D6h,00h,7Eh,00h,04h,00h,18h,02h,30h,02h,10h,00h,08h,00h,62h,00h
        db      0Eh,00h,6Dh,79h,0D7h,0FFh,79h,28h,00h,00h,0D6h,00h,7Eh,00h,04h,00h
        db      38h,02h,20h,02h,10h,02h,28h,02h,00h,00h,5Eh,00h,12h,00h,7Ah,28h
        db      00h,00h,92h,86h,1Ah,00h,0D6h,00h,7Eh,00h,06h,00h,30h,02h,18h,02h
        db      70h,02h,78h,02h,20h,02h,38h,02h,00h,00h,10h,00h,2Bh,81h,19h,00h
        db      3Dh,11h,0E0h,0FFh,50h,00h,02h,00h,00h,00h,62h,00h,0Eh,00h,79h,0A5h
        db      19h,00h,87h,5Ah,0E0h,0FFh,0D6h,00h,7Eh,00h,04h,00h,00h,00h,18h,02h
        db      08h,00h,18h,00h,62h,00h,0Ch,00h,2Bh,81h,19h,00h,3Dh,11h,0E0h,0FFh
        db      0D6h,00h,7Eh,00h,03h,00h,00h,00h,18h,00h,60h,02h,5Eh,00h,0Ch,00h
        db      85h,5Ah,0E8h,0FFh,7Bh,0A5h,12h,00h,0D6h,00h,7Eh,00h,03h,00h,70h,02h
        db      18h,02h,00h,00h,00h,00h,10h,00h,0D5h,7Eh,0E7h,0FFh,3Dh,11h,0E0h,0FFh
        db      50h,00h,02h,00h,00h,00h,62h,00h,0Eh,00h,87h,5Ah,0E7h,0FFh,87h,5Ah
        db      0E0h,0FFh,0D6h,00h,7Eh,00h,04h,00h,80h,00h,98h,00h,10h,02h,20h,02h
        db      62h,00h,0Ch,00h,0D5h,7Eh,0E7h,0FFh,3Dh,11h,0E0h,0FFh,0D6h,00h,7Eh,00h
        db      03h,00h,80h,00h,68h,02h,98h,00h,5Eh,00h,0Ch,00h,85h,5Ah,0E0h,0FFh
        db      7Bh,0A5h,0Ah,00h,0D6h,00h,7Eh,00h,03h,00h,80h,00h,20h,02h,78h,02h
        db      00h,00h,68h,00h,14h,00h,01h,80h,0Ah,00h,0BCh,00h,20h,29h,03h,00h
        db      40h,00h,7Eh,00h,04h,00h,00h,00h,60h,02h,68h,02h,80h,00h,5Eh,00h
        db      0Eh,00h,85h,5Ah,0E0h,0FFh,7Bh,0A5h,0Ah,00h,0D6h,00h,7Eh,00h,04h,00h
        db      00h,00h,80h,00h,78h,02h,70h,02h,00h,00h,75h,00h,00h,00h,90h,00h
        db      57h,00h,6Ch,00h,7Ah,00h,00h,00h,00h,00h,2Bh,00h,00h,00h,78h,00h
        db      00h,00h,48h,00h,2Bh,00h,36h,00h,00h,00h,82h,00h,03h,00h,00h,00h
        db      0CAh,0FFh,0CAh,0FFh,54h,00h,0CCh,0FFh,0CEh,0FFh,57h,00h,28h,00h,0D8h,0FFh
        db      00h,00h,86h,00h,08h,00h,68h,00h,18h,00h,86h,00h,00h,00h,6Ch,00h
        db      20h,00h,88h,00h,20h,00h,0F5h,0FFh,28h,00h,88h,00h,00h,00h,0F5h,0FFh
        db      30h,00h,92h,00h,78h,00h,10h,00h,08h,00h,38h,00h,92h,00h,7Eh,00h
        db      12h,00h,00h,00h,40h,00h,88h,00h,40h,00h,0F5h,0FFh,48h,00h,8Ah,00h
        db      38h,00h,44h,00h,50h,00h,8Ah,00h,40h,00h,48h,00h,58h,00h,8Ah,00h
        db      48h,00h,48h,00h,60h,00h,8Ah,00h,18h,00h,64h,00h,68h,00h,8Ah,00h
        db      20h,00h,6Ch,00h,70h,00h,8Ah,00h,28h,00h,6Ch,00h,78h,00h,8Ah,00h
        db      00h,00h,6Ch,00h,80h,00h,8Ah,00h,30h,00h,6Ch,00h,88h,00h,8Ah,00h
        db      08h,00h,64h,00h,90h,00h,92h,00h,5Fh,00h,0Dh,00h,30h,00h,98h,00h
        db      86h,00h,10h,00h,0FEh,0FFh,0A0h,00h,86h,00h,98h,00h,0FBh,0FFh,0A8h,00h
        db      92h,00h,63h,00h,0Eh,00h,30h,00h,0B0h,00h,92h,00h,03h,00h,02h,00h
        db      10h,00h,0B8h,00h,8Ah,00h,0B0h,00h,08h,00h,0C0h,00h,8Ah,00h,0B8h,00h
        db      04h,00h,0C8h,00h,8Ah,00h,98h,00h,0Ah,00h,0D0h,00h,8Ah,00h,10h,00h
        db      08h,00h,0D8h,00h,8Ah,00h,0A8h,00h,0Ah,00h,0E0h,00h,8Ah,00h,0A0h,00h
        db      08h,00h,0E8h,00h,86h,00h,0C0h,00h,0F3h,0FFh,0F0h,00h,86h,00h,0C8h,00h
        db      0F7h,0FFh,0F8h,00h,86h,00h,0B0h,00h,0F3h,0FFh,00h,01h,86h,00h,0B8h,00h
        db      0F7h,0FFh,08h,01h,8Ah,00h,0A8h,00h,48h,00h,10h,01h,8Ah,00h,0A0h,00h
        db      48h,00h,18h,01h,8Ah,00h,08h,01h,48h,00h,20h,01h,8Ah,00h,00h,01h
        db      48h,00h,28h,01h,8Ah,00h,08h,01h,4Ch,00h,30h,01h,8Ah,00h,00h,01h
        db      50h,00h,38h,01h,8Ah,00h,0A0h,00h,50h,00h,40h,01h,8Ah,00h,0A8h,00h
        db      52h,00h,48h,01h,8Ah,00h,10h,00h,50h,00h,50h,01h,8Ah,00h,98h,00h
        db      52h,00h,58h,01h,8Ah,00h,0B8h,00h,4Ch,00h,60h,01h,8Ah,00h,0B0h,00h
        db      50h,00h,68h,01h,8Ah,00h,0B8h,00h,48h,00h,70h,01h,8Ah,00h,0B0h,00h
        db      48h,00h,78h,01h,8Ah,00h,10h,00h,48h,00h,80h,01h,8Ah,00h,98h,00h
        db      48h,00h,88h,01h,86h,00h,60h,00h,70h,0FFh,90h,01h,86h,00h,58h,00h
        db      70h,0FFh,98h,01h,86h,00h,50h,00h,78h,0FFh,0A0h,01h,86h,00h,48h,00h
        db      70h,0FFh,0A8h,01h,86h,00h,40h,00h,70h,0FFh,0B0h,01h,86h,00h,38h,00h
        db      78h,0FFh,0B8h,01h,86h,00h,98h,00h,0AEh,0FFh,0C0h,01h,86h,00h,10h,00h
        db      0B0h,0FFh,0C8h,01h,86h,00h,0B8h,00h,0AAh,0FFh,0D0h,01h,86h,00h,0B0h,00h
        db      0A6h,0FFh,0D8h,01h,86h,00h,0C8h,00h,0AAh,0FFh,0E0h,01h,86h,00h,0C0h,00h
        db      0A6h,0FFh,0E8h,01h,86h,00h,0D8h,00h,0B0h,0FFh,0F0h,01h,86h,00h,0D0h,00h
        db      0AEh,0FFh,0F8h,01h,86h,00h,0D8h,00h,0B2h,0FFh,00h,02h,86h,00h,0D0h,00h
        db      0B3h,0FFh,08h,02h,86h,00h,0C8h,00h,0B3h,0FFh,10h,02h,86h,00h,0C0h,00h
        db      0B3h,0FFh,18h,02h,86h,00h,0B8h,00h,0B3h,0FFh,20h,02h,86h,00h,0B0h,00h
        db      0B3h,0FFh,28h,02h,86h,00h,10h,00h,0B2h,0FFh,30h,02h,86h,00h,98h,00h
        db      0B3h,0FFh,38h,02h,86h,00h,88h,01h,0AEh,0FFh,40h,02h,86h,00h,80h,01h
        db      0B0h,0FFh,48h,02h,86h,00h,70h,01h,0AAh,0FFh,50h,02h,86h,00h,78h,01h
        db      0A6h,0FFh,58h,02h,86h,00h,60h,01h,0AAh,0FFh,60h,02h,86h,00h,68h,01h
        db      0A6h,0FFh,68h,02h,86h,00h,50h,01h,0B0h,0FFh,70h,02h,86h,00h,58h,01h
        db      0AEh,0FFh,78h,02h,86h,00h,50h,01h,0B2h,0FFh,80h,02h,86h,00h,58h,01h
        db      0B3h,0FFh,88h,02h,86h,00h,60h,01h,0B3h,0FFh,90h,02h,86h,00h,68h,01h
        db      0B3h,0FFh,98h,02h,86h,00h,70h,01h,0B3h,0FFh,0A0h,02h,86h,00h,78h,01h
        db      0B3h,0FFh,0A8h,02h,86h,00h,80h,01h,0B2h,0FFh,0B0h,02h,86h,00h,88h,01h
        db      0B3h,0FFh,0B8h,02h,0D4h,00h,58h,00h,20h,29h,00h,00h,00h,08h,00h,05h
        db      18h,00h,05h,20h,00h,00h,28h,00h,00h,30h,00h,00h,38h,00h,05h,40h
        db      00h,01h,48h,00h,00h,50h,00h,05h,58h,00h,02h,60h,00h,00h,68h,00h
        db      05h,70h,00h,02h,78h,00h,00h,80h,00h,02h,88h,00h,00h,90h,00h,05h
        db      98h,00h,00h,10h,00h,00h,0A0h,00h,00h,0A8h,00h,00h,0B0h,00h,01h,0B8h
        db      00h,01h,0C0h,00h,02h,0C8h,00h,02h,0D0h,00h,02h,0D8h,00h,02h,0E0h,00h
        db      02h,0E8h,00h,02h,0F0h,00h,01h,0F8h,00h,01h,00h,01h,00h,08h,01h,00h
        db      10h,01h,00h,18h,01h,00h,20h,01h,00h,28h,01h,00h,30h,01h,01h,38h
        db      01h,01h,40h,01h,02h,48h,01h,02h,50h,01h,02h,58h,01h,02h,60h,01h
        db      02h,68h,01h,02h,70h,01h,01h,78h,01h,01h,80h,01h,00h,88h,01h,00h
        db      90h,01h,00h,98h,01h,01h,0A0h,01h,05h,0A8h,01h,00h,0B0h,01h,00h,0B8h
        db      01h,05h,0C0h,01h,00h,0C8h,01h,00h,0D0h,01h,00h,0D8h,01h,00h,0E0h,01h
        db      01h,0E8h,01h,01h,0F0h,01h,02h,0F8h,01h,02h,00h,02h,02h,08h,02h,02h
        db      10h,02h,02h,18h,02h,02h,20h,02h,01h,28h,02h,01h,30h,02h,00h,38h
        db      02h,00h,40h,02h,00h,48h,02h,00h,50h,02h,00h,58h,02h,00h,60h,02h
        db      01h,68h,02h,01h,70h,02h,02h,78h,02h,02h,80h,02h,02h,88h,02h,02h
        db      90h,02h,02h,98h,02h,02h,0A0h,02h,01h,0A8h,02h,01h,0B0h,02h,00h,0B8h
        db      02h,00h,66h,00h,16h,00h,01h,80h,0B7h,0FFh,0D6h,00h,7Eh,00h,08h,00h
        db      30h,00h,28h,00h,48h,00h,60h,00h,78h,00h,88h,00h,90h,01h,0A8h,01h
        db      58h,00h,0Eh,00h,0C4h,0C6h,0DCh,0FFh,0DEh,0FCh,0B7h,0FFh,88h,8Dh,0E1h,0FFh
        db      0D6h,00h,7Eh,00h,04h,00h,10h,01h,18h,01h,20h,01h,28h,01h,58h,00h
        db      0Eh,00h,0C9h,0C6h,0E0h,0FFh,4Dh,0FBh,0B7h,0FFh,6Fh,72h,20h,00h,0D6h,00h
        db      7Eh,00h,04h,00h,0F0h,00h,0F8h,00h,0E8h,00h,0E0h,00h,58h,00h,0Eh,00h
        db      3Ch,39h,24h,00h,0DEh,0FCh,0B7h,0FFh,78h,72h,1Fh,00h,0D6h,00h,7Eh,00h
        db      04h,00h,08h,02h,00h,02h,10h,02h,18h,02h,58h,00h,0Eh,00h,37h,39h
        db      20h,00h,4Dh,0FBh,0B7h,0FFh,91h,8Dh,0E0h,0FFh,0D6h,00h,7Eh,00h,04h,00h
        db      0A8h,02h,0A0h,02h,0B0h,02h,0B8h,02h,0Ch,00h,40h,0FEh,0B7h,0FFh,01h,80h
        db      0E1h,0FFh,16h,04h,0Ch,03h,58h,00h,0Eh,00h,00h,0B9h,29h,00h,92h,0FDh
        db      0B7h,0FFh,80h,95h,29h,00h,0D6h,00h,7Eh,00h,04h,00h,0C0h,01h,0C8h,01h
        db      0D0h,01h,0D8h,01h,5Eh,00h,0Eh,00h,40h,0FEh,0B7h,0FFh,01h,80h,29h,00h
        db      0D6h,00h,7Eh,00h,04h,00h,38h,02h,30h,02h,0C8h,01h,0C0h,01h,62h,00h
        db      1Ah,00h,0Dh,80h,2Dh,00h,7Fh,0FCh,0B7h,0FFh,0D6h,00h,7Eh,00h,04h,00h
        db      0D8h,01h,0D0h,01h,0E0h,01h,0E8h,01h,7Eh,00h,04h,00h,58h,02h,50h,02h
        db      60h,02h,68h,02h,58h,00h,0Eh,00h,07h,0B9h,2Dh,00h,24h,0FBh,0B7h,0FFh
        db      75h,6Ah,0D8h,0FFh,0D6h,00h,7Eh,00h,04h,00h,68h,02h,60h,02h,70h,02h
        db      78h,02h,5Eh,00h,0Eh,00h,40h,0FEh,0B7h,0FFh,0FFh,7Fh,0D7h,0FFh,0D6h,00h
        db      7Eh,00h,04h,00h,78h,02h,70h,02h,80h,02h,88h,02h,58h,00h,0Eh,00h
        db      00h,47h,0D7h,0FFh,92h,0FDh,0B7h,0FFh,80h,6Ah,0D7h,0FFh,0D6h,00h,7Eh,00h
        db      04h,00h,58h,01h,50h,01h,60h,01h,68h,01h,5Eh,00h,0Eh,00h,40h,0FEh
        db      0B7h,0FFh,0FFh,7Fh,0D7h,0FFh,0D6h,00h,7Eh,00h,04h,00h,48h,01h,40h,01h
        db      50h,01h,58h,01h,5Eh,00h,0Eh,00h,40h,0FEh,0B7h,0FFh,01h,80h,29h,00h
        db      0D6h,00h,7Eh,00h,04h,00h,98h,00h,10h,00h,0A0h,00h,0A8h,00h,58h,00h
        db      0Eh,00h,0F9h,46h,0D3h,0FFh,24h,0FBh,0B7h,0FFh,8Bh,95h,28h,00h,0D6h,00h
        db      7Eh,00h,04h,00h,0B0h,00h,0B8h,00h,10h,00h,98h,00h,62h,00h,1Ah,00h
        db      0F3h,7Fh,0D3h,0FFh,7Fh,0FCh,0B7h,0FFh,0D6h,00h,7Eh,00h,04h,00h,68h,01h
        db      60h,01h,70h,01h,78h,01h,7Eh,00h,04h,00h,0C0h,00h,0C8h,00h,0B8h,00h
        db      0B0h,00h,5Eh,00h,12h,00h,65h,66h,0ACh,0FFh,34h,0B3h,36h,00h,0BCh,00h
        db      22h,29h,04h,00h,7Eh,00h,04h,00h,00h,00h,08h,00h,18h,00h,20h,00h
        db      68h,00h,0Eh,00h,01h,80h,36h,00h,0D6h,00h,7Eh,00h,04h,00h,20h,00h
        db      28h,00h,30h,00h,00h,00h,58h,00h,12h,00h,48h,34h,0CAh,0FFh,90h,68h
        db      0ACh,0FFh,0B8h,0CBh,36h,00h,0BCh,00h,22h,29h,04h,00h,7Eh,00h,04h,00h
        db      20h,00h,18h,00h,38h,00h,40h,00h,58h,00h,12h,00h,0B8h,0CBh,48h,00h
        db      90h,68h,0ACh,0FFh,0B8h,0CBh,24h,00h,0BCh,00h,22h,29h,04h,00h,7Eh,00h
        db      04h,00h,0B0h,01h,0B8h,01h,08h,00h,00h,00h,60h,00h,0Eh,00h,82h,5Ah
        db      0B8h,0FFh,7Eh,0A5h,24h,00h,0D6h,00h,7Eh,00h,04h,00h,40h,00h,48h,00h
        db      28h,00h,20h,00h,60h,00h,0Eh,00h,7Eh,0A5h,36h,00h,7Eh,0A5h,36h,00h
        db      0D6h,00h,7Eh,00h,04h,00h,00h,00h,30h,00h,0A8h,01h,0B0h,01h,62h,00h
        db      12h,00h,0CCh,4Ch,0B8h,0FFh,65h,66h,0ACh,0FFh,0BCh,00h,22h,29h,05h,00h
        db      7Eh,00h,04h,00h,40h,00h,38h,00h,50h,00h,58h,00h,62h,00h,12h,00h
        db      34h,0B3h,48h,00h,65h,66h,0ACh,0FFh,0BCh,00h,22h,29h,04h,00h,7Eh,00h
        db      04h,00h,98h,01h,0A0h,01h,0B8h,01h,0B0h,01h,64h,00h,0Eh,00h,0FFh,7Fh
        db      0B8h,0FFh,0D6h,00h,7Eh,00h,04h,00h,58h,00h,60h,00h,48h,00h,40h,00h
        db      64h,00h,0Eh,00h,01h,80h,48h,00h,0D6h,00h,7Eh,00h,04h,00h,0B0h,01h
        db      0A8h,01h,90h,01h,98h,01h,58h,00h,12h,00h,48h,34h,0B8h,0FFh,90h,68h
        db      0ACh,0FFh,48h,34h,0DCh,0FFh,0BCh,00h,22h,29h,05h,00h,7Eh,00h,04h,00h
        db      58h,00h,50h,00h,68h,00h,70h,00h,58h,00h,12h,00h,0B8h,0CBh,36h,00h
        db      90h,68h,0ACh,0FFh,48h,34h,0CAh,0FFh,0BCh,00h,22h,29h,05h,00h,7Eh,00h
        db      04h,00h,80h,00h,90h,00h,0A0h,01h,98h,01h,60h,00h,0Eh,00h,82h,5Ah
        db      0B8h,0FFh,82h,5Ah,0DCh,0FFh,0D6h,00h,7Eh,00h,04h,00h,60h,00h,58h,00h
        db      70h,00h,78h,00h,60h,00h,0Eh,00h,7Eh,0A5h,36h,00h,82h,5Ah,0CAh,0FFh
        db      0D6h,00h,7Eh,00h,04h,00h,88h,00h,80h,00h,98h,01h,90h,01h,68h,00h
        db      0Eh,00h,0FFh,7Fh,0CAh,0FFh,0D6h,00h,7Eh,00h,04h,00h,78h,00h,70h,00h
        db      80h,00h,88h,00h,5Eh,00h,12h,00h,65h,66h,0ACh,0FFh,0CCh,4Ch,0CAh,0FFh
        db      0BCh,00h,22h,29h,05h,00h,7Eh,00h,04h,00h,70h,00h,68h,00h,90h,00h
        db      80h,00h,66h,00h,16h,00h,0FFh,7Fh,0A9h,0FFh,0D6h,00h,7Eh,00h,08h,00h
        db      08h,00h,0B8h,01h,0A0h,01h,90h,00h,68h,00h,50h,00h,38h,00h,18h,00h
        db      00h,00h,5Eh,00h,1Ah,00h,40h,0FEh,0B7h,0FFh,01h,80h,0E1h,0FFh,0D6h,00h
        db      7Eh,00h,04h,00h,88h,01h,80h,01h,18h,01h,10h,01h,7Eh,00h,04h,00h
        db      0B8h,02h,0B0h,02h,48h,02h,40h,02h,62h,00h,0Eh,00h,0Dh,80h,0E0h,0FFh
        db      7Fh,0FCh,0B7h,0FFh,0D6h,00h,7Eh,00h,04h,00h,00h,01h,08h,01h,0F8h,00h
        db      0F0h,00h,58h,00h,0Eh,00h,0C4h,0C6h,0DCh,0FFh,0DEh,0FCh,0B7h,0FFh,88h,8Dh
        db      29h,00h,0D6h,00h,7Eh,00h,04h,00h,0A8h,00h,0A0h,00h,08h,01h,00h,01h
        db      58h,00h,0Eh,00h,07h,0B9h,2Dh,00h,24h,0FBh,0B7h,0FFh,75h,6Ah,20h,00h
        db      0D6h,00h,7Eh,00h,04h,00h,0E8h,01h,0E0h,01h,0F0h,01h,0F8h,01h,5Eh,00h
        db      0Eh,00h,40h,0FEh,0B7h,0FFh,0FFh,7Fh,1Fh,00h,0D6h,00h,7Eh,00h,04h,00h
        db      0F8h,01h,0F0h,01h,00h,02h,08h,02h,62h,00h,0Eh,00h,0F3h,7Fh,20h,00h
        db      7Fh,0FCh,0B7h,0FFh,0D6h,00h,7Eh,00h,04h,00h,18h,02h,10h,02h,20h,02h
        db      28h,02h,58h,00h,0Eh,00h,37h,39h,20h,00h,4Dh,0FBh,0B7h,0FFh,91h,8Dh
        db      28h,00h,0D6h,00h,7Eh,00h,04h,00h,28h,02h,20h,02h,30h,02h,38h,02h
        db      58h,00h,0Eh,00h,00h,47h,0D7h,0FFh,92h,0FDh,0B7h,0FFh,80h,6Ah,1Fh,00h
        db      0D6h,00h,7Eh,00h,04h,00h,0D0h,00h,0D8h,00h,0C8h,00h,0C0h,00h,5Eh,00h
        db      0Eh,00h,40h,0FEh,0B7h,0FFh,0FFh,7Fh,1Fh,00h,0D6h,00h,7Eh,00h,04h,00h
        db      0E0h,00h,0E8h,00h,0D8h,00h,0D0h,00h,00h,00h,58h,00h,0Eh,00h,0C9h,0C6h
        db      0E0h,0FFh,4Dh,0FBh,0B7h,0FFh,6Fh,72h,0D8h,0FFh,0D6h,00h,7Eh,00h,04h,00h
        db      38h,01h,30h,01h,40h,01h,48h,01h,62h,00h,0Eh,00h,0Dh,80h,0E0h,0FFh
        db      7Fh,0FCh,0B7h,0FFh,0D6h,00h,7Eh,00h,04h,00h,28h,01h,20h,01h,30h,01h
        db      38h,01h,58h,00h,0Eh,00h,00h,0B9h,29h,00h,92h,0FDh,0B7h,0FFh,80h,95h
        db      0E1h,0FFh,0D6h,00h,7Eh,00h,04h,00h,40h,02h,48h,02h,50h,02h,58h,02h
        db      58h,00h,0Eh,00h,3Ch,39h,24h,00h,0DEh,0FCh,0B7h,0FFh,78h,72h,0D7h,0FFh
        db      0D6h,00h,7Eh,00h,04h,00h,88h,02h,80h,02h,90h,02h,98h,02h,62h,00h
        db      0Eh,00h,0F3h,7Fh,20h,00h,7Fh,0FCh,0B7h,0FFh,0D6h,00h,7Eh,00h,04h,00h
        db      98h,02h,90h,02h,0A0h,02h,0A8h,02h,58h,00h,0Eh,00h,0F9h,46h,0D3h,0FFh
        db      24h,0FBh,0B7h,0FFh,8Bh,95h,0E0h,0FFh,0D6h,00h,7Eh,00h,04h,00h,78h,01h
        db      70h,01h,80h,01h,88h,01h,00h,00h,47h,00h,00h,00h,3Ch,00h,2Fh,00h
        db      49h,00h,7Ah,00h,00h,00h,06h,00h,17h,00h,00h,00h,78h,00h,00h,00h
        db      1Eh,00h,18h,00h,24h,00h,00h,00h,82h,00h,02h,00h,00h,00h,1Eh,00h
        db      0E2h,0FFh,2Fh,00h,0E8h,0FFh,0EDh,0FFh,0FFh,0FFh,86h,00h,00h,00h,0C4h,0FFh
        db      10h,00h,8Ah,00h,10h,00h,49h,00h,18h,00h,8Ah,00h,00h,00h,49h,00h
        db      20h,00h,88h,00h,00h,00h,0FCh,0FFh,28h,00h,88h,00h,20h,00h,0FCh,0FFh
        db      30h,00h,88h,00h,18h,00h,0FCh,0FFh,38h,00h,88h,00h,10h,00h,0FCh,0FFh
        db      40h,00h,94h,00h,03h,00h,2Ch,00h,08h,00h,48h,00h,8Ah,00h,48h,00h
        db      0FAh,0FFh,50h,00h,8Ah,00h,08h,00h,0FDh,0FFh,58h,00h,86h,00h,50h,00h
        db      06h,00h,60h,00h,86h,00h,58h,00h,03h,00h,68h,00h,86h,00h,08h,00h
        db      03h,00h,70h,00h,86h,00h,48h,00h,06h,00h,78h,00h,86h,00h,48h,00h
        db      2Ah,00h,80h,00h,86h,00h,08h,00h,2Dh,00h,88h,00h,86h,00h,08h,00h
        db      30h,00h,90h,00h,86h,00h,48h,00h,30h,00h,98h,00h,86h,00h,58h,00h
        db      2Dh,00h,0A0h,00h,86h,00h,50h,00h,2Ah,00h,0A8h,00h,86h,00h,58h,00h
        db      30h,00h,0B0h,00h,86h,00h,50h,00h,30h,00h,0B8h,00h,8Ah,00h,90h,00h
        db      33h,00h,0C0h,00h,8Ah,00h,98h,00h,2Dh,00h,0C8h,00h,8Ah,00h,98h,00h
        db      33h,00h,0D0h,00h,8Ah,00h,90h,00h,36h,00h,0D8h,00h,8Ah,00h,80h,00h
        db      33h,00h,0E0h,00h,8Ah,00h,88h,00h,36h,00h,0E8h,00h,8Ah,00h,88h,00h
        db      33h,00h,0F0h,00h,8Ah,00h,80h,00h,2Dh,00h,0F8h,00h,8Ah,00h,78h,00h
        db      2Dh,00h,00h,01h,8Ah,00h,70h,00h,33h,00h,08h,01h,8Ah,00h,08h,00h
        db      33h,00h,10h,01h,8Ah,00h,48h,00h,2Dh,00h,18h,01h,8Ah,00h,70h,00h
        db      36h,00h,20h,01h,8Ah,00h,78h,00h,33h,00h,28h,01h,8Ah,00h,08h,00h
        db      36h,00h,30h,01h,8Ah,00h,48h,00h,33h,00h,38h,01h,0D4h,00h,28h,00h
        db      20h,29h,00h,00h,02h,10h,00h,01h,18h,00h,02h,20h,00h,03h,28h,00h
        db      02h,30h,00h,05h,38h,00h,02h,40h,00h,00h,08h,00h,02h,48h,00h,02h
        db      50h,00h,00h,58h,00h,00h,60h,00h,02h,68h,00h,02h,70h,00h,05h,78h
        db      00h,05h,80h,00h,02h,88h,00h,02h,90h,00h,05h,98h,00h,05h,0A0h,00h
        db      00h,0A8h,00h,00h,0B0h,00h,02h,0B8h,00h,02h,0C0h,00h,02h,0C8h,00h,02h
        db      0D0h,00h,05h,0D8h,00h,05h,0E0h,00h,02h,0E8h,00h,02h,0F0h,00h,00h,0F8h
        db      00h,00h,00h,01h,02h,08h,01h,02h,10h,01h,00h,18h,01h,00h,20h,01h
        db      05h,28h,01h,05h,30h,01h,02h,38h,01h,02h,66h,00h,0Eh,00h,01h,80h
        db      0D1h,0FFh,0D6h,00h,7Eh,00h,04h,00h,18h,00h,10h,00h,00h,00h,20h,00h
        db      10h,00h,0B4h,7Fh,15h,00h,4Bh,0F7h,01h,00h,94h,01h,02h,01h,64h,00h
        db      0Eh,00h,01h,80h,18h,00h,0D6h,00h,7Eh,00h,04h,00h,30h,01h,38h,01h
        db      18h,01h,10h,01h,68h,00h,1Ah,00h,0FFh,7Fh,0DDh,0FFh,0D6h,00h,7Eh,00h
        db      04h,00h,20h,01h,28h,01h,38h,01h,30h,01h,7Eh,00h,04h,00h,0D8h,00h
        db      0D0h,00h,0E0h,00h,0E8h,00h,64h,00h,1Ah,00h,0FFh,7Fh,0E8h,0FFh,0D6h,00h
        db      7Eh,00h,04h,00h,0C0h,00h,0C8h,00h,0D0h,00h,0D8h,00h,7Eh,00h,04h,00h
        db      0B0h,00h,0B8h,00h,98h,00h,90h,00h,68h,00h,1Ah,00h,01h,80h,16h,00h
        db      0D6h,00h,7Eh,00h,04h,00h,0A0h,00h,0A8h,00h,0B8h,00h,0B0h,00h,7Eh,00h
        db      04h,00h,58h,00h,50h,00h,60h,00h,68h,00h,64h,00h,0Eh,00h,01h,80h
        db      18h,00h,0D6h,00h,7Eh,00h,04h,00h,08h,00h,48h,00h,50h,00h,58h,00h
        db      66h,00h,0Eh,00h,0FFh,7Fh,0D1h,0FFh,0D6h,00h,7Eh,00h,04h,00h,00h,00h
        db      10h,00h,18h,00h,20h,00h,64h,00h,0Eh,00h,0FFh,7Fh,0E2h,0FFh,0D6h,00h
        db      7Eh,00h,04h,00h,28h,00h,00h,00h,20h,00h,30h,00h,68h,00h,0Eh,00h
        db      0FFh,7Fh,0D5h,0FFh,0D6h,00h,7Eh,00h,04h,00h,30h,00h,20h,00h,18h,00h
        db      38h,00h,64h,00h,0Eh,00h,01h,80h,1Eh,00h,0D6h,00h,7Eh,00h,04h,00h
        db      38h,00h,18h,00h,10h,00h,40h,00h,68h,00h,0Eh,00h,01h,80h,1Eh,00h
        db      0D6h,00h,7Eh,00h,04h,00h,40h,00h,10h,00h,00h,00h,28h,00h,00h,00h
        db      62h,00h,1Ah,00h,0B4h,7Fh,15h,00h,4Bh,0F7h,01h,00h,0D6h,00h,7Eh,00h
        db      04h,00h,20h,01h,08h,01h,00h,01h,28h,01h,7Eh,00h,04h,00h,60h,00h
        db      78h,00h,70h,00h,68h,00h,5Eh,00h,0Eh,00h,4Bh,0F7h,0D5h,0FFh,4Ch,80h
        db      0E3h,0FFh,0D6h,00h,7Eh,00h,04h,00h,0C8h,00h,0C0h,00h,0F0h,00h,0F8h,00h
        db      62h,00h,0Eh,00h,4Ch,80h,0EBh,0FFh,4Bh,0F7h,01h,00h,0D6h,00h,7Eh,00h
        db      04h,00h,0A0h,00h,88h,00h,80h,00h,0A8h,00h,5Eh,00h,0Eh,00h,4Bh,0F7h
        db      0D5h,0FFh,0B4h,7Fh,10h,00h,0D6h,00h,7Eh,00h,04h,00h,80h,00h,88h,00h
        db      90h,00h,98h,00h,62h,00h,0Eh,00h,4Ch,80h,0EEh,0FFh,4Bh,0F7h,0D5h,0FFh
        db      0D6h,00h,7Eh,00h,04h,00h,0E0h,00h,0F8h,00h,0F0h,00h,0E8h,00h,00h,00h
        db      5Eh,00h,0Eh,00h,4Bh,0F7h,0D5h,0FFh,4Ch,80h,0E3h,0FFh,0D6h,00h,7Eh,00h
        db      04h,00h,00h,01h,08h,01h,10h,01h,18h,01h,5Eh,00h,0Eh,00h,4Bh,0F7h
        db      0D5h,0FFh,0B4h,7Fh,10h,00h,0D6h,00h,7Eh,00h,04h,00h,48h,00h,08h,00h
        db      70h,00h,78h,00h,00h,00h,5Fh,00h,00h,00h,30h,00h,5Ah,00h,40h,00h
        db      7Ah,00h,00h,00h,00h,00h,2Dh,00h,00h,00h,78h,00h,00h,00h,21h,00h
        db      2Dh,00h,21h,00h,00h,00h,82h,00h,02h,00h,00h,00h,0F6h,0FFh,0E8h,0FFh
        db      00h,00h,0F1h,0FFh,0E1h,0FFh,1Eh,00h,92h,00h,1Bh,00h,0FEh,0FFh,08h,00h
        db      10h,00h,92h,00h,13h,00h,0FFh,0FFh,00h,00h,18h,00h,92h,00h,2Fh,00h
        db      10h,00h,08h,00h,20h,00h,92h,00h,22h,00h,0Eh,00h,00h,00h,28h,00h
        db      92h,00h,30h,00h,2Bh,00h,08h,00h,30h,00h,92h,00h,24h,00h,21h,00h
        db      00h,00h,38h,00h,92h,00h,0F1h,0FFh,0Fh,00h,00h,00h,40h,00h,92h,00h
        db      0EEh,0FFh,13h,00h,08h,00h,48h,00h,88h,00h,08h,00h,1Eh,00h,50h,00h
        db      88h,00h,10h,00h,1Eh,00h,58h,00h,88h,00h,20h,00h,1Eh,00h,60h,00h
        db      88h,00h,30h,00h,1Eh,00h,68h,00h,88h,00h,48h,00h,1Eh,00h,70h,00h
        db      88h,00h,00h,00h,5Ah,00h,78h,00h,88h,00h,18h,00h,5Ah,00h,80h,00h
        db      88h,00h,28h,00h,5Ah,00h,88h,00h,88h,00h,38h,00h,5Ah,00h,90h,00h
        db      88h,00h,40h,00h,5Ah,00h,98h,00h,92h,00h,0F0h,0FFh,2Eh,00h,50h,00h
        db      0A0h,00h,92h,00h,0F2h,0FFh,23h,00h,78h,00h,0A8h,00h,92h,00h,01h,00h
        db      32h,00h,78h,00h,0B0h,00h,92h,00h,03h,00h,40h,00h,50h,00h,0B8h,00h
        db      88h,00h,0A0h,00h,0E2h,0FFh,0C0h,00h,88h,00h,0B8h,00h,0E2h,0FFh,0C8h,00h
        db      88h,00h,0A8h,00h,0A6h,0FFh,0D0h,00h,88h,00h,0B0h,00h,0A6h,0FFh,0D8h,00h
        db      92h,00h,1Eh,00h,3Fh,00h,50h,00h,0E0h,00h,92h,00h,15h,00h,30h,00h
        db      78h,00h,0E8h,00h,88h,00h,0E0h,00h,0E2h,0FFh,0F0h,00h,88h,00h,0E8h,00h
        db      0A6h,0FFh,0F8h,00h,0D4h,00h,20h,00h,20h,29h,00h,00h,00h,08h,00h,00h
        db      10h,00h,01h,18h,00h,00h,20h,00h,02h,28h,00h,01h,30h,00h,02h,38h
        db      00h,01h,40h,00h,00h,48h,00h,01h,50h,00h,03h,58h,00h,03h,60h,00h
        db      03h,68h,00h,04h,70h,00h,03h,78h,00h,03h,80h,00h,03h,88h,00h,03h
        db      90h,00h,04h,98h,00h,03h,0A0h,00h,03h,0A8h,00h,03h,0B0h,00h,04h,0B8h
        db      00h,04h,0C0h,00h,02h,0C8h,00h,02h,0D0h,00h,01h,0D8h,00h,01h,0E0h,00h
        db      05h,0E8h,00h,05h,0F0h,00h,02h,0F8h,00h,01h,58h,00h,0Eh,00h,0D1h,0F6h
        db      0Ah,00h,8Bh,0E1h,00h,00h,04h,84h,18h,00h,0D6h,00h,7Eh,00h,04h,00h
        db      00h,00h,08h,00h,10h,00h,18h,00h,58h,00h,0Eh,00h,0C4h,52h,0F7h,0FFh
        db      34h,0DFh,00h,00h,0Ah,0A4h,19h,00h,0D6h,00h,7Eh,00h,04h,00h,18h,00h
        db      10h,00h,20h,00h,28h,00h,58h,00h,0Eh,00h,68h,7Bh,0E8h,0FFh,55h,0DEh
        db      00h,00h,6Eh,0FBh,0Ah,00h,0D6h,00h,7Eh,00h,04h,00h,28h,00h,20h,00h
        db      30h,00h,38h,00h,58h,00h,0Eh,00h,1Fh,0A6h,19h,00h,85h,0DFh,00h,00h
        db      0DAh,0AAh,09h,00h,0D6h,00h,7Eh,00h,04h,00h,40h,00h,48h,00h,08h,00h
        db      00h,00h,60h,00h,12h,00h,8Ch,0F6h,0Fh,00h,5Ah,80h,1Fh,00h,0BCh,00h
        db      20h,29h,02h,00h,7Eh,00h,04h,00h,08h,00h,50h,00h,58h,00h,10h,00h
        db      60h,00h,12h,00h,0A0h,55h,0F4h,0FFh,0DCh,0A0h,21h,00h,0BCh,00h,20h,29h
        db      02h,00h,7Eh,00h,04h,00h,10h,00h,58h,00h,60h,00h,20h,00h,60h,00h
        db      12h,00h,0EAh,7Fh,0E0h,0FFh,44h,0FBh,0Fh,00h,0BCh,00h,20h,29h,03h,00h
        db      7Eh,00h,04h,00h,20h,00h,60h,00h,68h,00h,30h,00h,60h,00h,12h,00h
        db      13h,0A3h,21h,00h,0F7h,0A7h,0Ch,00h,0BCh,00h,20h,29h,02h,00h,7Eh,00h
        db      04h,00h,48h,00h,70h,00h,50h,00h,08h,00h,58h,00h,0Eh,00h,0D1h,0F6h
        db      0Fh,00h,75h,1Eh,0C4h,0FFh,04h,84h,1Fh,00h,0D6h,00h,7Eh,00h,04h,00h
        db      50h,00h,78h,00h,80h,00h,58h,00h,58h,00h,0Eh,00h,0C4h,52h,0F4h,0FFh
        db      0CCh,20h,0C4h,0FFh,0Ah,0A4h,21h,00h,0D6h,00h,7Eh,00h,04h,00h,58h,00h
        db      80h,00h,88h,00h,60h,00h,58h,00h,0Eh,00h,68h,7Bh,0E0h,0FFh,0ABh,21h
        db      0C4h,0FFh,6Eh,0FBh,0Fh,00h,0D6h,00h,7Eh,00h,04h,00h,60h,00h,88h,00h
        db      90h,00h,68h,00h,58h,00h,0Eh,00h,8Dh,0A6h,0Fh,00h,0AEh,22h,0C4h,0FFh
        db      42h,0ABh,1Fh,00h,0D6h,00h,7Eh,00h,04h,00h,50h,00h,70h,00h,98h,00h
        db      78h,00h,58h,00h,0Eh,00h,0F1h,83h,18h,00h,2Ch,1Eh,0A6h,0FFh,30h,09h
        db      0F5h,0FFh,0D6h,00h,7Eh,00h,04h,00h,0A8h,00h,98h,00h,70h,00h,0A0h,00h
        db      58h,00h,0Eh,00h,5Dh,0AAh,0Ch,00h,0A7h,1Dh,0C4h,0FFh,65h,5Ah,0DFh,0FFh
        db      0D6h,00h,7Eh,00h,04h,00h,0B8h,00h,0B0h,00h,0A8h,00h,0A0h,00h,60h,00h
        db      12h,00h,5Ah,80h,1Fh,00h,74h,09h,0F1h,0FFh,0BCh,00h,20h,29h,02h,00h
        db      7Eh,00h,04h,00h,0C0h,00h,0A0h,00h,70h,00h,48h,00h,60h,00h,12h,00h
        db      0F7h,0A7h,0Ch,00h,0EDh,5Ch,0DFh,0FFh,0BCh,00h,20h,29h,03h,00h,7Eh,00h
        db      04h,00h,0C8h,00h,0B8h,00h,0A0h,00h,0C0h,00h,58h,00h,0Eh,00h,0F1h,83h
        db      18h,00h,0D4h,0E1h,00h,00h,30h,09h,0F5h,0FFh,0D6h,00h,7Eh,00h,04h,00h
        db      0D0h,00h,0C0h,00h,48h,00h,40h,00h,58h,00h,0Eh,00h,0E8h,0A7h,09h,00h
        db      0A3h,0E2h,00h,00h,18h,58h,0E6h,0FFh,0D6h,00h,7Eh,00h,04h,00h,0D8h,00h
        db      0C8h,00h,0C0h,00h,0D0h,00h,58h,00h,0Eh,00h,81h,5Ch,0DFh,0FFh,0E8h,1Dh
        db      0C4h,0FFh,41h,53h,0F4h,0FFh,0D6h,00h,7Eh,00h,04h,00h,68h,00h,90h,00h
        db      0E8h,00h,0E0h,00h,58h,00h,0Eh,00h,92h,04h,0F1h,0FFh,87h,21h,0C4h,0FFh
        db      74h,7Bh,0E0h,0FFh,0D6h,00h,7Eh,00h,04h,00h,0E0h,00h,0E8h,00h,0B0h,00h
        db      0B8h,00h,60h,00h,12h,00h,24h,5Fh,0DFh,0FFh,0A0h,55h,0F4h,0FFh,0BCh,00h
        db      20h,29h,03h,00h,7Eh,00h,04h,00h,30h,00h,68h,00h,0E0h,00h,0F0h,00h
        db      60h,00h,12h,00h,0BCh,04h,0F1h,0FFh,0EAh,7Fh,0E0h,0FFh,0BCh,00h,20h,29h
        db      03h,00h,7Eh,00h,04h,00h,0F0h,00h,0E0h,00h,0B8h,00h,0C8h,00h,58h,00h
        db      0Eh,00h,18h,58h,0E6h,0FFh,0A3h,0E2h,00h,00h,18h,58h,0F7h,0FFh,0D6h,00h
        db      7Eh,00h,04h,00h,38h,00h,30h,00h,0F0h,00h,0F8h,00h,58h,00h,0Eh,00h
        db      92h,04h,0F5h,0FFh,79h,0DEh,00h,00h,74h,7Bh,0E8h,0FFh,0D6h,00h,7Eh,00h
        db      04h,00h,0F8h,00h,0F0h,00h,0C8h,00h,0D8h,00h,66h,00h,1Ah,00h,0FFh,7Fh
        db      0A6h,0FFh,0BCh,00h,22h,29h,05h,00h,7Eh,00h,08h,00h,98h,00h,0A8h,00h
        db      0B0h,00h,0E8h,00h,90h,00h,88h,00h,80h,00h,78h,00h,66h,00h,16h,00h
        db      01h,80h,00h,00h,0D6h,00h,7Eh,00h,08h,00h,00h,00h,18h,00h,28h,00h
        db      38h,00h,0F8h,00h,0D8h,00h,0D0h,00h,40h,00h,00h,00h,47h,00h,00h,00h
        db      28h,00h,38h,00h,64h,00h,7Ah,00h,00h,00h,00h,00h,1Ch,00h,00h,00h
        db      78h,00h,00h,00h,14h,00h,1Ch,00h,32h,00h,00h,00h,82h,00h,05h,00h
        db      00h,00h,0F0h,0FFh,0D1h,0FFh,00h,00h,0ECh,0FFh,0CEh,0FFh,28h,00h,0F8h,0FFh
        db      0D4h,0FFh,35h,00h,0Bh,00h,0D0h,0FFh,2Fh,00h,0Eh,00h,30h,00h,2Ch,00h
        db      86h,00h,08h,00h,28h,00h,28h,00h,86h,00h,00h,00h,22h,00h,30h,00h
        db      94h,00h,0FEh,0FFh,24h,00h,00h,00h,38h,00h,90h,00h,02h,00h,03h,00h
        db      00h,00h,40h,00h,86h,00h,40h,00h,1Eh,00h,48h,00h,86h,00h,38h,00h
        db      20h,00h,50h,00h,8Ah,00h,38h,00h,62h,00h,58h,00h,8Ah,00h,08h,00h
        db      64h,00h,60h,00h,8Ah,00h,28h,00h,64h,00h,68h,00h,8Ah,00h,50h,00h
        db      62h,00h,70h,00h,8Ah,00h,00h,00h,5Eh,00h,78h,00h,8Ah,00h,40h,00h
        db      5Eh,00h,80h,00h,8Ah,00h,48h,00h,5Eh,00h,88h,00h,8Ah,00h,30h,00h
        db      5Eh,00h,90h,00h,94h,00h,04h,00h,0FEh,0FFh,28h,00h,98h,00h,94h,00h
        db      04h,00h,04h,00h,30h,00h,0A0h,00h,8Ah,00h,0A0h,00h,56h,00h,0A8h,00h
        db      8Ah,00h,98h,00h,5Ch,00h,0B0h,00h,86h,00h,0B0h,00h,0D8h,0FFh,0B8h,00h
        db      86h,00h,98h,00h,0D8h,0FFh,0C0h,00h,86h,00h,0A0h,00h,0DEh,0FFh,0C8h,00h
        db      86h,00h,0A8h,00h,0DEh,0FFh,0D0h,00h,90h,00h,01h,00h,30h,00h,00h,00h
        db      0D8h,00h,90h,00h,0EEh,0FFh,34h,00h,0A0h,00h,0E0h,00h,86h,00h,10h,00h
        db      11h,00h,0E8h,00h,86h,00h,0D8h,00h,1Eh,00h,0F0h,00h,86h,00h,08h,00h
        db      24h,00h,0F8h,00h,90h,00h,0F4h,0FFh,0Bh,00h,98h,00h,00h,01h,90h,00h
        db      08h,00h,0FFh,0FFh,10h,00h,08h,01h,86h,00h,00h,01h,0F0h,0FFh,10h,01h
        db      90h,00h,0E8h,0FFh,0FFh,0FFh,18h,00h,18h,01h,86h,00h,08h,00h,04h,00h
        db      20h,01h,8Ah,00h,0F0h,00h,05h,00h,28h,01h,8Ah,00h,28h,00h,04h,00h
        db      30h,01h,8Ah,00h,0E8h,00h,04h,00h,38h,01h,8Ah,00h,0E0h,00h,04h,00h
        db      40h,01h,8Ah,00h,10h,00h,04h,00h,48h,01h,88h,00h,40h,00h,2Dh,00h
        db      50h,01h,8Ah,00h,50h,01h,05h,00h,58h,01h,8Ah,00h,08h,00h,04h,00h
        db      60h,01h,8Ah,00h,10h,00h,54h,00h,68h,01h,8Ah,00h,0E0h,00h,52h,00h
        db      70h,01h,8Ah,00h,0E0h,00h,56h,00h,78h,01h,8Ah,00h,10h,00h,58h,00h
        db      80h,01h,8Ah,00h,0E8h,00h,58h,00h,88h,01h,8Ah,00h,0E8h,00h,54h,00h
        db      90h,01h,8Ah,00h,50h,01h,59h,00h,98h,01h,88h,00h,80h,00h,2Dh,00h
        db      0A0h,01h,8Ah,00h,0F0h,00h,5Eh,00h,0A8h,01h,8Ah,00h,0F0h,00h,59h,00h
        db      0B0h,01h,8Ah,00h,08h,00h,60h,00h,0B8h,01h,8Ah,00h,28h,00h,60h,00h
        db      0C0h,01h,8Ah,00h,0D8h,00h,5Eh,00h,0C8h,01h,86h,00h,60h,00h,24h,00h
        db      0D0h,01h,8Ah,00h,00h,01h,5Ch,00h,0D8h,01h,8Ah,00h,08h,01h,58h,00h
        db      0E0h,01h,8Ah,00h,10h,01h,5Ch,00h,0E8h,01h,8Ah,00h,18h,01h,60h,00h
        db      0F0h,01h,86h,00h,60h,00h,04h,00h,0F8h,01h,8Ah,00h,28h,00h,36h,00h
        db      00h,02h,8Ah,00h,28h,00h,2Eh,00h,08h,02h,92h,00h,03h,00h,0CEh,0FFh
        db      20h,00h,10h,02h,8Ah,00h,10h,02h,04h,00h,18h,02h,0D4h,00h,20h,00h
        db      20h,29h,00h,00h,00h,08h,00h,00h,28h,00h,01h,30h,00h,00h,60h,00h
        db      01h,68h,00h,02h,78h,00h,00h,90h,00h,01h,0D8h,00h,01h,10h,00h,01h
        db      0E0h,00h,01h,0E8h,00h,01h,0F0h,00h,01h,28h,01h,04h,30h,01h,03h,38h
        db      01h,05h,40h,01h,05h,48h,01h,04h,58h,01h,03h,60h,01h,03h,68h,01h
        db      04h,70h,01h,05h,78h,01h,03h,80h,01h,03h,88h,01h,03h,90h,01h,05h
        db      98h,01h,03h,0A8h,01h,03h,0B0h,01h,04h,0B8h,01h,03h,0C0h,01h,03h,0C8h
        db      01h,03h,62h,00h,2Ch,00h,86h,28h,0F7h,0FFh,94h,79h,0CBh,0FFh,0BCh,00h
        db      22h,29h,05h,00h,7Eh,00h,04h,00h,0E8h,00h,0E0h,00h,40h,01h,38h,01h
        db      7Eh,00h,04h,00h,90h,01h,70h,01h,78h,01h,88h,01h,0D6h,00h,7Eh,00h
        db      04h,00h,38h,01h,40h,01h,70h,01h,90h,01h,62h,00h,2Ch,00h,0F6h,0D2h
        db      08h,00h,1Dh,78h,0CBh,0FFh,0BCh,00h,22h,29h,05h,00h,7Eh,00h,04h,00h
        db      48h,01h,40h,01h,0E0h,00h,10h,00h,7Eh,00h,04h,00h,80h,01h,78h,01h
        db      70h,01h,68h,01h,0D6h,00h,7Eh,00h,04h,00h,40h,01h,48h,01h,68h,01h
        db      70h,01h,62h,00h,0Eh,00h,0F2h,51h,0F1h,0FFh,56h,62h,0D0h,0FFh,0D6h,00h
        db      7Eh,00h,04h,00h,28h,01h,38h,01h,90h,01h,0B0h,01h,62h,00h,0Eh,00h
        db      0Fh,0AEh,08h,00h,55h,62h,0CBh,0FFh,0D6h,00h,7Eh,00h,04h,00h,48h,01h
        db      58h,01h,98h,01h,68h,01h,0Ch,00h,6Eh,0F6h,00h,00h,5Dh,80h,2Fh,00h
        db      7Eh,02h,24h,02h,58h,00h,12h,00h,0D4h,7Fh,0EEh,0FFh,5Dh,0F9h,00h,00h
        db      0AFh,0FCh,2Fh,00h,0BCh,00h,22h,29h,03h,00h,7Eh,00h,04h,00h,30h,00h
        db      28h,00h,98h,00h,0A0h,00h,58h,00h,12h,00h,2Dh,81h,10h,00h,65h,0F0h
        db      0FCh,0FFh,33h,0F8h,2Bh,00h,0BCh,00h,22h,29h,02h,00h,7Eh,00h,04h,00h
        db      0C8h,00h,0C0h,00h,08h,00h,00h,00h,58h,00h,12h,00h,0B3h,7Fh,0EEh,0FFh
        db      25h,0F8h,0FCh,0FFh,0EDh,03h,0D5h,0FFh,0BCh,00h,22h,29h,03h,00h,7Eh,00h
        db      04h,00h,0A8h,00h,0B0h,00h,68h,00h,90h,00h,58h,00h,12h,00h,0D5h,80h
        db      10h,00h,0CAh,0F2h,00h,00h,9Bh,06h,0D1h,0FFh,0BCh,00h,22h,29h,02h,00h
        db      7Eh,00h,04h,00h,78h,00h,60h,00h,0B8h,00h,0D0h,00h,64h,00h,12h,00h
        db      0FFh,7Fh,0ECh,0FFh,0BCh,00h,22h,29h,03h,00h,7Eh,00h,04h,00h,68h,00h
        db      0B0h,00h,98h,00h,28h,00h,64h,00h,12h,00h,01h,80h,14h,00h,0BCh,00h
        db      22h,29h,02h,00h,7Eh,00h,04h,00h,08h,00h,0C0h,00h,0B8h,00h,60h,00h
        db      5Eh,00h,3Ch,00h,0A3h,35h,0D8h,0FFh,0C9h,8Bh,32h,00h,0D6h,00h,7Eh,00h
        db      07h,00h,08h,00h,0D8h,00h,10h,00h,0E0h,00h,0E8h,00h,0F0h,00h,28h,00h
        db      0BCh,00h,22h,29h,03h,00h,40h,00h,7Eh,00h,0Eh,00h,08h,00h,0D8h,00h
        db      10h,00h,0E0h,00h,0E8h,00h,0F0h,00h,28h,00h,0F8h,00h,18h,00h,00h,01h
        db      08h,01h,10h,01h,18h,01h,20h,01h,5Eh,00h,3Ch,00h,0A3h,35h,0D8h,0FFh
        db      37h,74h,0CEh,0FFh,0D6h,00h,7Eh,00h,07h,00h,68h,00h,0A8h,01h,88h,01h
        db      78h,01h,80h,01h,0C8h,01h,60h,00h,0BCh,00h,22h,29h,04h,00h,40h,00h
        db      7Eh,00h,0Eh,00h,0F8h,01h,0F0h,01h,0E8h,01h,0E0h,01h,0D8h,01h,20h,00h
        db      0D0h,01h,68h,00h,0A8h,01h,88h,01h,78h,01h,80h,01h,0C8h,01h,60h,00h
        db      62h,00h,1Eh,00h,0Ch,52h,0F1h,0FFh,75h,62h,0D0h,0FFh,0BCh,00h,22h,29h
        db      05h,00h,7Eh,00h,04h,00h,0F0h,00h,0E8h,00h,38h,01h,28h,01h,7Eh,00h
        db      04h,00h,0B0h,01h,90h,01h,88h,01h,0A8h,01h,62h,00h,1Eh,00h,0F4h,0ADh
        db      0Eh,00h,75h,62h,0D0h,0FFh,0BCh,00h,22h,29h,04h,00h,7Eh,00h,04h,00h
        db      58h,01h,48h,01h,10h,00h,50h,01h,7Eh,00h,04h,00h,0A0h,01h,80h,01h
        db      68h,01h,98h,01h,62h,00h,2Ch,00h,9Bh,99h,14h,00h,0CCh,4Ch,0D8h,0FFh
        db      0BCh,00h,22h,29h,04h,00h,7Eh,00h,04h,00h,60h,01h,58h,01h,50h,01h
        db      08h,00h,7Eh,00h,04h,00h,60h,00h,0A0h,01h,98h,01h,0B8h,01h,0D6h,00h
        db      7Eh,00h,04h,00h,58h,01h,60h,01h,0B8h,01h,98h,01h,62h,00h,3Eh,00h
        db      0B3h,6Ch,0ECh,0FFh,0F0h,43h,0D8h,0FFh,0BCh,00h,22h,29h,04h,00h,7Eh,00h
        db      04h,00h,28h,00h,0F0h,00h,28h,01h,30h,01h,7Eh,00h,04h,00h,0C0h,01h
        db      0B0h,01h,0A8h,01h,68h,00h,0D6h,00h,7Eh,00h,04h,00h,28h,01h,0B0h,01h
        db      0C0h,01h,30h,01h,0BCh,00h,22h,29h,04h,00h,7Eh,00h,04h,00h,00h,02h
        db      08h,02h,10h,02h,18h,02h,00h,00h,5Eh,00h,4Ah,00h,6Eh,0F6h,00h,00h
        db      5Dh,80h,2Fh,00h,0D6h,00h,7Eh,00h,04h,00h,00h,00h,08h,00h,28h,00h
        db      30h,00h,0BCh,00h,22h,29h,02h,00h,7Eh,00h,04h,00h,00h,00h,08h,00h
        db      38h,00h,40h,00h,7Eh,00h,04h,00h,48h,00h,50h,00h,28h,00h,30h,00h
        db      7Eh,00h,04h,00h,00h,00h,40h,00h,48h,00h,30h,00h,0BCh,00h,22h,29h
        db      01h,00h,7Eh,00h,04h,00h,38h,00h,08h,00h,28h,00h,50h,00h,00h,00h
        db      0Ch,00h,6Eh,0F6h,00h,00h,0A3h,7Fh,0D1h,0FFh,5Ch,00h,02h,00h,00h,00h
        db      5Eh,00h,4Ah,00h,6Eh,0F6h,00h,00h,0A3h,7Fh,0D1h,0FFh,0D6h,00h,7Eh,00h
        db      04h,00h,90h,00h,68h,00h,60h,00h,78h,00h,0BCh,00h,22h,29h,03h,00h
        db      7Eh,00h,04h,00h,80h,00h,58h,00h,60h,00h,78h,00h,7Eh,00h,04h,00h
        db      90h,00h,68h,00h,70h,00h,88h,00h,7Eh,00h,04h,00h,90h,00h,88h,00h
        db      80h,00h,78h,00h,0BCh,00h,22h,29h,02h,00h,7Eh,00h,04h,00h,70h,00h
        db      68h,00h,60h,00h,58h,00h,00h,00h,62h,00h,20h,00h,0D6h,7Fh,0EEh,0FFh
        db      9Ch,0F9h,00h,00h,0D6h,00h,7Eh,00h,04h,00h,30h,00h,28h,00h,68h,00h
        db      90h,00h,0BCh,00h,22h,29h,03h,00h,7Eh,00h,04h,00h,30h,00h,0A0h,00h
        db      0A8h,00h,90h,00h,62h,00h,20h,00h,0A3h,80h,10h,00h,44h,0F3h,00h,00h
        db      0D6h,00h,7Eh,00h,04h,00h,78h,00h,60h,00h,08h,00h,00h,00h,0BCh,00h
        db      22h,29h,02h,00h,7Eh,00h,04h,00h,78h,00h,0D0h,00h,0C8h,00h,00h,00h
        db      00h,00h,0B4h,00h,00h,00h,53h,00h,0ACh,00h,46h,00h,7Ah,00h,00h,00h
        db      00h,00h,56h,00h,00h,00h,78h,00h,00h,00h,2Bh,00h,56h,00h,23h,00h
        db      00h,00h,7Ah,00h,28h,00h,23h,00h,4Ch,00h,00h,00h,90h,00h,03h,00h
        db      0B4h,0FFh,00h,00h,08h,00h,8Ah,00h,08h,00h,0F8h,0FFh,10h,00h,94h,00h
        db      0EFh,0FFh,0F2h,0FFh,00h,00h,18h,00h,8Ah,00h,18h,00h,0DCh,0FFh,20h,00h
        db      8Ah,00h,08h,00h,0C2h,0FFh,28h,00h,8Ah,00h,08h,00h,0BAh,0FFh,30h,00h
        db      8Ah,00h,00h,00h,0BAh,0FFh,38h,00h,86h,00h,38h,00h,0CDh,0FFh,40h,00h
        db      92h,00h,0C6h,0FFh,0DDh,0FFh,00h,00h,48h,00h,86h,00h,00h,00h,0CDh,0FFh
        db      50h,00h,90h,00h,0B7h,0FFh,0ACh,00h,30h,00h,58h,00h,90h,00h,0EDh,0FFh
        db      60h,00h,48h,00h,60h,00h,8Ah,00h,58h,00h,46h,00h,68h,00h,86h,00h
        db      00h,00h,0C0h,0FFh,70h,00h,8Ah,00h,60h,00h,23h,00h,78h,00h,86h,00h
        db      60h,00h,0FAh,0FFh,80h,00h,86h,00h,48h,00h,0F4h,0FFh,88h,00h,8Ah,00h
        db      60h,00h,0DDh,0FFh,90h,00h,86h,00h,38h,00h,0C0h,0FFh,98h,00h,86h,00h
        db      08h,00h,0ADh,0FFh,0A0h,00h,86h,00h,08h,00h,0BAh,0FFh,0A8h,00h,88h,00h
        db      50h,00h,0F2h,0FFh,0B0h,00h,86h,00h,0B0h,00h,25h,00h,0B8h,00h,86h,00h
        db      08h,00h,0F7h,0FFh,0C0h,00h,86h,00h,10h,00h,0ADh,0FFh,0C8h,00h,86h,00h
        db      30h,00h,0ADh,0FFh,0D0h,00h,86h,00h,28h,00h,0ADh,0FFh,0D8h,00h,90h,00h
        db      0BDh,0FFh,0FEh,0FFh,20h,00h,0E0h,00h,8Ah,00h,0E0h,00h,24h,00h,0E8h,00h
        db      86h,00h,30h,00h,0BAh,0FFh,0F0h,00h,88h,00h,40h,00h,0F2h,0FFh,0F8h,00h
        db      8Ah,00h,0B8h,00h,0BAh,0FFh,00h,01h,86h,00h,30h,00h,0F7h,0FFh,08h,01h
        db      86h,00h,18h,00h,0BDh,0FFh,10h,01h,86h,00h,20h,00h,0BDh,0FFh,18h,01h
        db      0D4h,00h,22h,00h,20h,29h,00h,00h,03h,08h,00h,02h,10h,00h,02h,18h
        db      00h,02h,20h,00h,02h,28h,00h,02h,30h,00h,01h,38h,00h,02h,40h,00h
        db      02h,48h,00h,03h,50h,00h,03h,58h,00h,02h,60h,00h,03h,68h,00h,03h
        db      70h,00h,01h,78h,00h,02h,80h,00h,01h,88h,00h,00h,90h,00h,01h,98h
        db      00h,00h,0A0h,00h,02h,0A8h,00h,02h,0B0h,00h,02h,0B8h,00h,02h,0C0h,00h
        db      02h,0C8h,00h,02h,0D0h,00h,01h,0D8h,00h,02h,0E0h,00h,02h,0E8h,00h,02h
        db      0F0h,00h,00h,0F8h,00h,00h,00h,01h,00h,08h,01h,00h,58h,00h,12h,00h
        db      46h,0ABh,0E6h,0FFh,12h,0F5h,0C2h,0FFh,0AFh,0A0h,0DDh,0FFh,0BCh,00h,22h,29h
        db      02h,00h,7Eh,00h,04h,00h,0B8h,00h,18h,00h,10h,00h,0C0h,00h,58h,00h
        db      12h,00h,0AEh,9Dh,0DEh,0FFh,51h,0F3h,00h,00h,0F8h,50h,23h,00h,0BCh,00h
        db      22h,29h,02h,00h,7Eh,00h,04h,00h,08h,01h,28h,00h,20h,00h,00h,01h
        db      0Ch,00h,0FFh,7Fh,0B4h,0FFh,00h,00h,23h,00h,32h,01h,9Eh,00h,68h,00h
        db      22h,00h,0FFh,7Fh,0DDh,0FFh,0BCh,00h,20h,29h,02h,00h,40h,00h,7Eh,00h
        db      0Bh,00h,70h,00h,0A0h,00h,0A8h,00h,0B0h,00h,0B8h,00h,0C0h,00h,08h,00h
        db      00h,00h,50h,00h,68h,00h,78h,00h,68h,00h,22h,00h,01h,80h,23h,00h
        db      0BCh,00h,20h,29h,01h,00h,40h,00h,7Eh,00h,0Bh,00h,90h,00h,58h,00h
        db      40h,00h,38h,00h,30h,00h,08h,01h,00h,01h,0F8h,00h,0F0h,00h,0D0h,00h
        db      98h,00h,66h,00h,18h,00h,0FFh,7Fh,54h,0FFh,0BCh,00h,20h,29h,03h,00h
        db      40h,00h,7Eh,00h,06h,00h,78h,00h,68h,00h,60h,00h,58h,00h,90h,00h
        db      80h,00h,62h,00h,1Ch,00h,0E7h,7Fh,0D5h,0FFh,0Ch,05h,00h,00h,0BCh,00h
        db      20h,29h,02h,00h,40h,00h,7Eh,00h,08h,00h,28h,00h,30h,00h,38h,00h
        db      00h,00h,08h,00h,10h,00h,18h,00h,20h,00h,00h,00h,66h,00h,10h,00h
        db      0FFh,7Fh,0B4h,0FFh,0D6h,00h,7Eh,00h,05h,00h,38h,00h,40h,00h,48h,00h
        db      50h,00h,00h,00h,58h,00h,0Eh,00h,38h,7Bh,12h,00h,59h,18h,0B4h,0FFh
        db      0A4h,18h,00h,00h,0D6h,00h,7Eh,00h,04h,00h,48h,00h,40h,00h,58h,00h
        db      60h,00h,58h,00h,0Eh,00h,38h,7Bh,12h,00h,59h,18h,0B4h,0FFh,5Ch,0E7h
        db      00h,00h,0D6h,00h,7Eh,00h,04h,00h,48h,00h,60h,00h,68h,00h,50h,00h
        db      58h,00h,0Eh,00h,0F2h,82h,18h,00h,1Bh,0EFh,0B4h,0FFh,70h,15h,0DDh,0FFh
        db      0D6h,00h,7Eh,00h,04h,00h,70h,00h,78h,00h,80h,00h,88h,00h,58h,00h
        db      0Eh,00h,0F2h,82h,1Eh,00h,1Bh,0EFh,0B4h,0FFh,90h,0EAh,00h,00h,0D6h,00h
        db      7Eh,00h,04h,00h,88h,00h,80h,00h,90h,00h,98h,00h,00h,00h,58h,00h
        db      12h,00h,36h,41h,1Bh,00h,0FAh,0E1h,0C4h,0FFh,07h,96h,0EEh,0FFh,0BCh,00h
        db      22h,29h,02h,00h,7Eh,00h,04h,00h,0E8h,00h,0B0h,00h,0A8h,00h,0C8h,00h
        db      66h,00h,1Ah,00h,01h,80h,0C2h,0FFh,0BCh,00h,20h,29h,00h,00h,7Eh,00h
        db      08h,00h,00h,01h,20h,00h,18h,00h,0B8h,00h,0B0h,00h,10h,01h,18h,01h
        db      0F8h,00h,58h,00h,12h,00h,0Fh,5Ch,28h,00h,0A1h,0DFh,00h,00h,0D5h,52h
        db      1Bh,00h,0BCh,00h,22h,29h,02h,00h,7Eh,00h,04h,00h,0D8h,00h,0F0h,00h
        db      0F8h,00h,0E0h,00h,62h,00h,1Ch,00h,0BFh,82h,28h,00h,5Eh,1Ah,00h,00h
        db      0BCh,00h,20h,29h,02h,00h,40h,00h,7Eh,00h,08h,00h,0C8h,00h,0A0h,00h
        db      70h,00h,98h,00h,0D0h,00h,0D8h,00h,0E0h,00h,0E8h,00h,00h,00h,66h,00h
        db      00h,00h,31h,00h,34h,00h,94h,00h,7Ah,00h,0ECh,0FFh,00h,00h,1Ah,00h
        db      00h,00h,78h,00h,00h,00h,18h,00h,1Ah,00h,4Ah,00h,00h,00h,7Ah,00h
        db      04h,00h,4Ah,00h,30h,00h,00h,00h,8Ah,00h,00h,00h,6Ch,0FFh,08h,00h
        db      86h,00h,08h,00h,0CFh,0FFh,10h,00h,86h,00h,00h,00h,0CFh,0FFh,18h,00h
        db      88h,00h,00h,00h,04h,00h,20h,00h,88h,00h,08h,00h,04h,00h,28h,00h
        db      88h,00h,10h,00h,04h,00h,30h,00h,88h,00h,18h,00h,04h,00h,38h,00h
        db      92h,00h,0FCh,0FFh,0F4h,0FFh,00h,00h,40h,00h,90h,00h,02h,00h,0D0h,0FFh
        db      40h,00h,48h,00h,8Ah,00h,48h,00h,0F8h,0FFh,50h,00h,94h,00h,0EFh,0FFh
        db      0F7h,0FFh,40h,00h,58h,00h,8Ah,00h,58h,00h,0A6h,0FFh,60h,00h,8Ah,00h
        db      48h,00h,8Ch,0FFh,68h,00h,8Ah,00h,48h,00h,84h,0FFh,70h,00h,8Ah,00h
        db      40h,00h,84h,0FFh,78h,00h,86h,00h,48h,00h,0D3h,0FFh,80h,00h,86h,00h
        db      48h,00h,0D9h,0FFh,88h,00h,90h,00h,0E0h,0FFh,0F7h,0FFh,40h,00h,90h,00h
        db      86h,00h,90h,00h,17h,00h,98h,00h,86h,00h,48h,00h,0FAh,0FFh,0A0h,00h
        db      86h,00h,40h,00h,0D7h,0FFh,0A8h,00h,86h,00h,70h,00h,0D3h,0FFh,0B0h,00h
        db      86h,00h,70h,00h,0D9h,0FFh,0B8h,00h,8Ah,00h,90h,00h,84h,0FFh,0C0h,00h
        db      8Ah,00h,98h,00h,84h,0FFh,0C8h,00h,86h,00h,70h,00h,0FAh,0FFh,0D0h,00h
        db      86h,00h,78h,00h,0D7h,0FFh,0D8h,00h,86h,00h,50h,00h,0D3h,0FFh,0E0h,00h
        db      86h,00h,58h,00h,0D7h,0FFh,0E8h,00h,86h,00h,60h,00h,0D7h,0FFh,0F0h,00h
        db      86h,00h,68h,00h,0D3h,0FFh,0F8h,00h,0D4h,00h,20h,00h,20h,29h,00h,00h
        db      05h,08h,00h,04h,10h,00h,03h,18h,00h,04h,20h,00h,05h,28h,00h,04h
        db      30h,00h,03h,38h,00h,04h,40h,00h,03h,48h,00h,03h,50h,00h,02h,58h
        db      00h,00h,60h,00h,00h,68h,00h,02h,70h,00h,02h,78h,00h,02h,80h,00h
        db      02h,88h,00h,02h,90h,00h,00h,98h,00h,00h,0A0h,00h,02h,0A8h,00h,02h
        db      0B0h,00h,01h,0B8h,00h,02h,0C0h,00h,00h,0C8h,00h,00h,0D0h,00h,01h,0D8h
        db      00h,01h,0E0h,00h,01h,0E8h,00h,00h,0F0h,00h,00h,0F8h,00h,02h,58h,00h
        db      0Eh,00h,96h,8Fh,04h,00h,97h,0F1h,00h,00h,83h,3Bh,3Eh,00h,0D6h,00h
        db      7Eh,00h,04h,00h,0D0h,00h,68h,00h,60h,00h,0C8h,00h,58h,00h,0Eh,00h
        db      6Ah,70h,25h,00h,97h,0F1h,00h,00h,7Dh,0C4h,0C2h,0FFh,0D6h,00h,7Eh,00h
        db      04h,00h,88h,00h,0E0h,00h,0E8h,00h,90h,00h,58h,00h,0Eh,00h,2Eh,90h
        db      0FEh,0FFh,9Bh,0ECh,00h,00h,0CDh,0C4h,0CAh,0FFh,0D6h,00h,7Eh,00h,04h,00h
        db      50h,00h,0A0h,00h,98h,00h,58h,00h,58h,00h,0Eh,00h,0D2h,6Fh,2Bh,00h
        db      9Bh,0ECh,00h,00h,33h,3Bh,36h,00h,0D6h,00h,7Eh,00h,04h,00h,0F8h,00h
        db      0B8h,00h,0C0h,00h,0F0h,00h,66h,00h,16h,00h,01h,80h,0D9h,0FFh,0D6h,00h
        db      7Eh,00h,08h,00h,0C0h,00h,0C8h,00h,60h,00h,58h,00h,98h,00h,90h,00h
        db      0E8h,00h,0F0h,00h,64h,00h,1Ch,00h,01h,80h,29h,00h,0BCh,00h,20h,29h
        db      02h,00h,40h,00h,7Eh,00h,08h,00h,0D8h,00h,0B0h,00h,0F8h,00h,0F0h,00h
        db      0E8h,00h,0E0h,00h,80h,00h,0A8h,00h,64h,00h,1Ch,00h,0FFh,7Fh,00h,00h
        db      0BCh,00h,20h,29h,03h,00h,40h,00h,7Eh,00h,08h,00h,40h,00h,48h,00h
        db      50h,00h,58h,00h,60h,00h,68h,00h,70h,00h,78h,00h,68h,00h,1Ch,00h
        db      0FFh,7Fh,0C2h,0FFh,0BCh,00h,20h,29h,03h,00h,40h,00h,7Eh,00h,08h,00h
        db      80h,00h,88h,00h,90h,00h,98h,00h,0A0h,00h,48h,00h,40h,00h,0A8h,00h
        db      68h,00h,1Ch,00h,01h,80h,3Eh,00h,0BCh,00h,20h,29h,02h,00h,40h,00h
        db      7Eh,00h,08h,00h,0D8h,00h,78h,00h,70h,00h,0D0h,00h,0C8h,00h,0C0h,00h
        db      0B8h,00h,0B0h,00h,66h,00h,0Eh,00h,0FFh,7Fh,0D0h,0FFh,0D6h,00h,7Eh,00h
        db      04h,00h,00h,00h,08h,00h,10h,00h,18h,00h,66h,00h,0Eh,00h,0FFh,7Fh
        db      0CCh,0FFh,0D6h,00h,7Eh,00h,04h,00h,20h,00h,28h,00h,30h,00h,38h,00h
        db      64h,00h,0Eh,00h,0FFh,7Fh,0FCh,0FFh,0D6h,00h,7Eh,00h,04h,00h,08h,00h
        db      28h,00h,20h,00h,00h,00h,68h,00h,0Eh,00h,0FFh,7Fh,0B6h,0FFh,0D6h,00h
        db      7Eh,00h,04h,00h,00h,00h,20h,00h,38h,00h,18h,00h,64h,00h,0Eh,00h
        db      01h,80h,2Dh,00h,0D6h,00h,7Eh,00h,04h,00h,18h,00h,38h,00h,30h,00h
        db      10h,00h,68h,00h,0Eh,00h,01h,80h,4Ah,00h,0D6h,00h,7Eh,00h,04h,00h
        db      10h,00h,30h,00h,28h,00h,08h,00h,00h,00h,0CDh,00h,00h,00h,00h,01h
        db      10h,00h,00h,01h,7Ah,00h,10h,00h,10h,00h,08h,00h,00h,00h,78h,00h
        db      00h,00h,80h,00h,08h,00h,80h,00h,00h,00h,7Ah,00h,90h,0FFh,90h,0FFh
        db      00h,00h,00h,00h,8Ah,00h,00h,00h,00h,01h,08h,00h,86h,00h,08h,00h
        db      00h,01h,10h,00h,86h,00h,00h,00h,00h,01h,18h,00h,88h,00h,00h,00h
        db      10h,00h,20h,00h,88h,00h,08h,00h,10h,00h,28h,00h,88h,00h,10h,00h
        db      10h,00h,30h,00h,88h,00h,18h,00h,10h,00h,38h,00h,0BCh,00h,36h,29h
        db      01h,00h,66h,00h,1Ch,00h,0FFh,7Fh,0F0h,0FFh,0B4h,00h,04h,00h,28h,00h
        db      00h,00h,00h,00h,30h,00h,0FFh,0FFh,00h,00h,38h,00h,0FFh,0FFh,0FFh,0FFh
        db      20h,00h,00h,00h,0FFh,0FFh,66h,00h,1Ch,00h,01h,80h,00h,00h,0B4h,00h
        db      04h,00h,00h,00h,00h,00h,0FFh,0FFh,18h,00h,0FFh,0FFh,0FFh,0FFh,10h,00h
        db      0FFh,0FFh,00h,00h,08h,00h,00h,00h,00h,00h,0BCh,00h,20h,29h,00h,00h
        db      64h,00h,0Ch,00h,0FFh,7Fh,70h,0FFh,7Eh,00h,04h,00h,38h,00h,30h,00h
        db      10h,00h,18h,00h,64h,00h,0Ch,00h,01h,80h,70h,00h,7Eh,00h,04h,00h
        db      28h,00h,20h,00h,00h,00h,08h,00h,00h,00h,0F5h,00h,00h,00h,80h,00h
        db      0D0h,00h,08h,00h,7Ah,00h,40h,00h,04h,00h,68h,00h,00h,00h,78h,00h
        db      00h,00h,40h,00h,68h,00h,04h,00h,00h,00h,7Ah,00h,00h,00h,00h,00h
        db      00h,00h,00h,00h,88h,00h,00h,00h,0D0h,00h,08h,00h,86h,00h,08h,00h
        db      80h,00h,10h,00h,86h,00h,00h,00h,80h,00h,18h,00h,8Ah,00h,00h,00h
        db      08h,00h,20h,00h,8Ah,00h,08h,00h,08h,00h,28h,00h,8Ah,00h,10h,00h
        db      08h,00h,30h,00h,8Ah,00h,18h,00h,08h,00h,38h,00h,0BCh,00h,20h,29h
        db      01h,00h,68h,00h,1Eh,00h,01h,80h,00h,00h,0A8h,00h,06h,00h,04h,00h
        db      08h,00h,0FFh,0FFh,00h,30h,10h,00h,00h,00h,00h,30h,18h,00h,00h,00h
        db      0FFh,0FFh,00h,00h,0FFh,0FFh,0FFh,0FFh,68h,00h,1Eh,00h,0FFh,7Fh,0F8h,0FFh
        db      0A8h,00h,06h,00h,04h,00h,30h,00h,00h,00h,00h,30h,28h,00h,0FFh,0FFh
        db      00h,30h,20h,00h,0FFh,0FFh,0FFh,0FFh,38h,00h,00h,00h,0FFh,0FFh,14h,00h
        db      68h,00h,26h,29h,00h,00h,64h,00h,12h,00h,01h,80h,00h,00h,0BCh,00h
        db      20h,29h,00h,00h,7Eh,00h,04h,00h,20h,00h,28h,00h,08h,00h,00h,00h
        db      66h,00h,12h,00h,0FFh,7Fh,30h,0FFh,0BCh,00h,20h,29h,02h,00h,7Eh,00h
        db      04h,00h,28h,00h,30h,00h,10h,00h,08h,00h,64h,00h,12h,00h,0FFh,7Fh
        db      80h,0FFh,0BCh,00h,20h,29h,00h,00h,7Eh,00h,04h,00h,18h,00h,10h,00h
        db      30h,00h,38h,00h,66h,00h,12h,00h,01h,80h,00h,00h,0BCh,00h,20h,29h
        db      02h,00h,7Eh,00h,04h,00h,00h,00h,18h,00h,38h,00h,20h,00h,00h,00h
        db      0F5h,00h,00h,00h,80h,00h,0D0h,00h,08h,00h,7Ah,00h,40h,00h,04h,00h
        db      68h,00h,00h,00h,78h,00h,00h,00h,40h,00h,68h,00h,04h,00h,00h,00h
        db      7Ah,00h,00h,00h,00h,00h,00h,00h,00h,00h,88h,00h,00h,00h,0D0h,00h
        db      08h,00h,86h,00h,08h,00h,80h,00h,10h,00h,86h,00h,00h,00h,80h,00h
        db      18h,00h,8Ah,00h,00h,00h,08h,00h,20h,00h,8Ah,00h,08h,00h,08h,00h
        db      28h,00h,8Ah,00h,10h,00h,08h,00h,30h,00h,8Ah,00h,18h,00h,08h,00h
        db      38h,00h,0BCh,00h,20h,29h,00h,00h,68h,00h,1Ch,00h,01h,80h,00h,00h
        db      0B4h,00h,04h,00h,08h,00h,00h,40h,00h,30h,10h,00h,0FFh,0BFh,00h,30h
        db      18h,00h,0FFh,0BFh,0FFh,0FFh,00h,00h,00h,40h,0FFh,0FFh,68h,00h,1Ch,00h
        db      0FFh,7Fh,0F8h,0FFh,0B4h,00h,04h,00h,30h,00h,00h,40h,00h,30h,28h,00h
        db      0FFh,0BFh,00h,30h,20h,00h,0FFh,0BFh,0FFh,0FFh,38h,00h,00h,40h,0FFh,0FFh
        db      14h,00h,68h,00h,26h,29h,00h,00h,64h,00h,12h,00h,01h,80h,00h,00h
        db      0BCh,00h,20h,29h,00h,00h,7Eh,00h,04h,00h,20h,00h,28h,00h,08h,00h
        db      00h,00h,66h,00h,12h,00h,0FFh,7Fh,30h,0FFh,0BCh,00h,20h,29h,02h,00h
        db      7Eh,00h,04h,00h,28h,00h,30h,00h,10h,00h,08h,00h,64h,00h,12h,00h
        db      0FFh,7Fh,80h,0FFh,0BCh,00h,20h,29h,00h,00h,7Eh,00h,04h,00h,18h,00h
        db      10h,00h,30h,00h,38h,00h,66h,00h,12h,00h,01h,80h,00h,00h,0BCh,00h
        db      20h,29h,02h,00h,7Eh,00h,04h,00h,00h,00h,18h,00h,38h,00h,20h,00h
        db      00h,00h,7Ch,00h,00h,00h,7Eh,00h,69h,00h,67h,00h,7Ah,00h,0FEh,0FFh
        db      0Ch,00h,3Ch,00h,00h,00h,78h,00h,00h,00h,41h,00h,2Dh,00h,53h,00h
        db      00h,00h,82h,00h,12h,00h,00h,00h,0EFh,0FFh,0EBh,0FFh,4Eh,00h,0DCh,0FFh
        db      00h,00h,52h,00h,0CBh,0FFh,04h,00h,47h,00h,0E5h,0FFh,0DCh,0FFh,54h,00h
        db      0BDh,0FFh,0FCh,0FFh,35h,00h,0DAh,0FFh,0D4h,0FFh,41h,00h,04h,00h,0E3h,0FFh
        db      5Ch,00h,13h,00h,0B9h,0FFh,3Dh,00h,0FCh,0FFh,0FAh,0FFh,69h,00h,35h,00h
        db      0DAh,0FFh,26h,00h,20h,00h,08h,00h,5Fh,00h,3Fh,00h,20h,00h,43h,00h
        db      06h,00h,54h,00h,3Fh,00h,0E0h,0FFh,35h,00h,56h,00h,0E7h,0FFh,26h,00h
        db      0Fh,00h,07h,00h,0BFh,0FFh,3Eh,00h,25h,00h,0E1h,0FFh,48h,00h,0F0h,0FFh
        db      0ECh,0FFh,50h,00h,92h,00h,19h,00h,0E5h,0FFh,00h,00h,90h,00h,92h,00h
        db      5Dh,00h,0DFh,0FFh,10h,00h,98h,00h,90h,00h,0D2h,0FFh,02h,00h,58h,00h
        db      0A0h,00h,94h,00h,1Fh,00h,0EBh,0FFh,58h,00h,0A8h,00h,94h,00h,57h,00h
        db      0CFh,0FFh,50h,00h,0B0h,00h,92h,00h,11h,00h,5Fh,00h,00h,00h,0B8h,00h
        db      92h,00h,0FEh,0FFh,1Dh,00h,68h,00h,0C0h,00h,90h,00h,82h,0FFh,0E1h,0FFh
        db      58h,00h,0C8h,00h,92h,00h,0FCh,0FFh,0EBh,0FFh,70h,00h,0D0h,00h,90h,00h
        db      36h,00h,0DAh,0FFh,20h,00h,0D8h,00h,90h,00h,13h,00h,0A6h,0FFh,40h,00h
        db      0E0h,00h,92h,00h,3Dh,00h,0E7h,0FFh,70h,00h,0E8h,00h,94h,00h,22h,00h
        db      0B0h,0FFh,50h,00h,0F0h,00h,92h,00h,24h,00h,13h,00h,70h,00h,0F8h,00h
        db      94h,00h,99h,0FFh,3Eh,00h,0F8h,00h,00h,01h,94h,00h,0E3h,0FFh,0E3h,0FFh
        db      08h,00h,08h,01h,94h,00h,0A9h,0FFh,0DEh,0FFh,68h,00h,10h,01h,92h,00h
        db      37h,00h,0EBh,0FFh,28h,00h,18h,01h,0D4h,00h,24h,00h,20h,29h,00h,00h
        db      04h,08h,00h,04h,10h,00h,04h,18h,00h,04h,20h,00h,02h,28h,00h,02h
        db      30h,00h,03h,90h,00h,03h,38h,00h,02h,40h,00h,03h,98h,00h,04h,48h
        db      00h,02h,50h,00h,04h,58h,00h,03h,0A0h,00h,05h,0A8h,00h,03h,0B0h,00h
        db      02h,0B8h,00h,04h,60h,00h,04h,68h,00h,04h,0C0h,00h,04h,0C8h,00h,01h
        db      70h,00h,00h,0D0h,00h,00h,0D8h,00h,00h,0E0h,00h,00h,0E8h,00h,01h,0F0h
        db      00h,01h,0F8h,00h,01h,00h,01h,04h,08h,01h,00h,10h,01h,00h,78h,00h
        db      03h,80h,00h,04h,18h,01h,04h,88h,00h,03h,06h,00h,16h,0BCh,11h,00h
        db      43h,4Dh,0B2h,0FFh,0D6h,0B3h,15h,00h,66h,03h,26h,02h,58h,00h,0Ch,00h
        db      2Ah,8Bh,35h,00h,0AAh,30h,0B9h,0FFh,10h,13h,0FCh,0FFh,0D6h,00h,7Eh,00h
        db      03h,00h,10h,00h,0C8h,00h,0C0h,00h,58h,00h,0Eh,00h,8Eh,0C8h,43h,00h
        db      0E7h,0A2h,0CBh,0FFh,0DFh,0BBh,04h,00h,0D6h,00h,7Eh,00h,04h,00h,20h,00h
        db      28h,00h,08h,01h,0D0h,00h,58h,00h,0Ch,00h,0Bh,0A2h,1Bh,00h,0C2h,49h
        db      0ACh,0FFh,03h,0D2h,24h,00h,0D6h,00h,7Eh,00h,03h,00h,18h,00h,28h,00h
        db      20h,00h,58h,00h,0Ch,00h,3Fh,0A5h,1Bh,00h,96h,54h,0ACh,0FFh,80h,0E0h
        db      24h,00h,0D6h,00h,7Eh,00h,03h,00h,18h,00h,20h,00h,10h,00h,58h,00h
        db      0Ch,00h,15h,0F7h,0FCh,0FFh,0E6h,65h,0A4h,0FFh,0Ah,0B3h,1Dh,00h,0D6h,00h
        db      7Eh,00h,03h,00h,30h,00h,90h,00h,18h,00h,58h,00h,0Ch,00h,75h,61h
        db      0D8h,0FFh,45h,37h,0B9h,0FFh,1Dh,0C2h,1Dh,00h,0D6h,00h,7Eh,00h,03h,00h
        db      98h,00h,48h,00h,38h,00h,58h,00h,0Ch,00h,90h,5Fh,0E0h,0FFh,60h,4Fh
        db      0A1h,0FFh,2Dh,0E1h,0F8h,0FFh,0D6h,00h,7Eh,00h,03h,00h,50h,00h,58h,00h
        db      98h,00h,58h,00h,0Ch,00h,0BAh,36h,00h,00h,89h,4Eh,0B2h,0FFh,0F8h,54h
        db      0B6h,0FFh,0D6h,00h,7Eh,00h,03h,00h,0B8h,00h,60h,00h,0A8h,00h,58h,00h
        db      0Ch,00h,0DEh,6Dh,0CBh,0FFh,9Eh,35h,0DAh,0FFh,18h,0DAh,26h,00h,0D6h,00h
        db      7Eh,00h,03h,00h,48h,00h,98h,00h,58h,00h,66h,00h,14h,00h,01h,80h
        db      0F1h,0FFh,0D6h,00h,7Eh,00h,07h,00h,70h,00h,0D0h,00h,0D8h,00h,0E0h,00h
        db      0E8h,00h,0F0h,00h,0F8h,00h,58h,00h,16h,00h,0B6h,0FAh,0Dh,00h,0C4h,97h
        db      0F1h,0FFh,0E8h,0B5h,04h,00h,0D6h,00h,7Eh,00h,03h,00h,0D8h,00h,38h,00h
        db      0E0h,00h,7Eh,00h,03h,00h,0E0h,00h,38h,00h,48h,00h,58h,00h,0Ch,00h
        db      5Ah,24h,0CBh,0FFh,09h,8Ch,0DAh,0FFh,0D2h,0D7h,26h,00h,0D6h,00h,7Eh,00h
        db      03h,00h,48h,00h,0E8h,00h,0E0h,00h,58h,00h,0Ch,00h,27h,3Bh,0C1h,0FFh
        db      06h,9Eh,0D2h,0FFh,4Eh,39h,0C1h,0FFh,0D6h,00h,7Eh,00h,03h,00h,0A8h,00h
        db      0B0h,00h,0F0h,00h,58h,00h,0Ch,00h,6Ch,2Bh,0E0h,0FFh,10h,98h,0F1h,0FFh
        db      0CBh,3Ch,0D6h,0FFh,0D6h,00h,7Eh,00h,03h,00h,0F0h,00h,0B0h,00h,0F8h,00h
        db      58h,00h,0Ch,00h,09h,0C7h,0E0h,0FFh,30h,0C1h,0D2h,0FFh,0E0h,5Fh,0A1h,0FFh
        db      0D6h,00h,7Eh,00h,03h,00h,0B0h,00h,0C0h,00h,0C8h,00h,58h,00h,0Ch,00h
        db      58h,0CCh,3Fh,00h,0E1h,0B4h,0DCh,0FFh,0D7h,59h,0E0h,0FFh,0D6h,00h,7Eh,00h
        db      03h,00h,0C8h,00h,0F8h,00h,0B0h,00h,58h,00h,0Ch,00h,0EAh,0C0h,19h,00h
        db      46h,91h,0F1h,0FFh,04h,0Ch,0DAh,0FFh,0D6h,00h,7Eh,00h,03h,00h,70h,00h
        db      0C8h,00h,0D0h,00h,58h,00h,0Ch,00h,34h,0B4h,1Dh,00h,0BCh,9Fh,0F1h,0FFh
        db      0F7h,0DAh,0EFh,0FFh,0D6h,00h,7Eh,00h,03h,00h,0D0h,00h,0C8h,00h,20h,00h
        db      58h,00h,10h,00h,26h,0D3h,0Dh,00h,41h,0A8h,0F1h,0FFh,51h,0AEh,04h,00h
        db      0D6h,00h,7Eh,00h,05h,00h,0D8h,00h,10h,01h,28h,00h,78h,00h,38h,00h
        db      00h,00h,58h,00h,0Ch,00h,16h,0BCh,11h,00h,43h,4Dh,0B2h,0FFh,0D6h,0B3h
        db      15h,00h,0D6h,00h,7Eh,00h,03h,00h,00h,00h,10h,00h,08h,00h,06h,00h
        db      4Ah,0B3h,04h,00h,73h,4Bh,97h,0FFh,0ACh,0BAh,06h,00h,0B0h,00h,1Eh,00h
        db      58h,00h,0Ch,00h,0ABh,0B3h,24h,00h,4Eh,0ABh,0CBh,0FFh,0D7h,0C5h,1Dh,00h
        db      0D6h,00h,7Eh,00h,03h,00h,08h,01h,28h,00h,10h,01h,00h,00h,58h,00h
        db      0Eh,00h,4Ah,0B3h,04h,00h,73h,4Bh,97h,0FFh,0ACh,0BAh,06h,00h,0D6h,00h
        db      7Eh,00h,04h,00h,88h,00h,40h,00h,30h,00h,00h,00h,58h,00h,0Ch,00h
        db      0F0h,0DEh,0F9h,0FFh,5Ah,5Bh,0C2h,0FFh,0AAh,0ACh,41h,00h,0D6h,00h,7Eh,00h
        db      03h,00h,78h,00h,28h,00h,90h,00h,58h,00h,0Ch,00h,0D8h,06h,11h,00h
        db      30h,78h,0B2h,0FFh,83h,2Bh,15h,00h,0D6h,00h,7Eh,00h,03h,00h,00h,00h
        db      18h,00h,10h,00h,58h,00h,0Ch,00h,0C7h,0D6h,0FCh,0FFh,83h,64h,0A4h,0FFh
        db      0B0h,43h,1Dh,00h,0D6h,00h,7Eh,00h,03h,00h,30h,00h,18h,00h,00h,00h
        db      58h,00h,0Ch,00h,27h,0E5h,0F8h,0FFh,74h,3Dh,0B2h,0FFh,0F7h,92h,30h,00h
        db      0D6h,00h,7Eh,00h,03h,00h,90h,00h,28h,00h,18h,00h,00h,00h,58h,00h
        db      0Ch,00h,0CDh,54h,04h,00h,29h,0BCh,97h,0FFh,0D7h,43h,06h,00h,0D6h,00h
        db      7Eh,00h,03h,00h,40h,00h,90h,00h,30h,00h,58h,00h,18h,00h,7Ah,35h
        db      0F5h,0FFh,2Ch,68h,0B3h,0FFh,50h,0CCh,2Eh,00h,0D6h,00h,7Eh,00h,03h,00h
        db      00h,01h,90h,00h,40h,00h,7Eh,00h,04h,00h,18h,01h,38h,00h,90h,00h
        db      00h,01h,58h,00h,0Ch,00h,0F0h,0DEh,0F8h,0FFh,5Ah,5Bh,0B2h,0FFh,0AAh,0ACh
        db      30h,00h,0D6h,00h,7Eh,00h,03h,00h,90h,00h,38h,00h,78h,00h,00h,00h
        db      06h,00h,98h,62h,0E0h,0FFh,6Ch,0BCh,0F1h,0FFh,39h,0D2h,0D6h,0FFh,8Eh,00h
        db      2Ah,00h,58h,00h,18h,00h,88h,32h,0E0h,0FFh,5Ah,67h,0A1h,0FFh,0E3h,0C7h
        db      0F8h,0FFh,0D6h,00h,7Eh,00h,03h,00h,50h,00h,98h,00h,40h,00h,7Eh,00h
        db      04h,00h,40h,00h,98h,00h,80h,00h,00h,01h,00h,00h,58h,00h,16h,00h
        db      0D2h,6Fh,0CBh,0FFh,71h,0C2h,0DAh,0FFh,7Ch,09h,26h,00h,0D6h,00h,7Eh,00h
        db      03h,00h,48h,00h,58h,00h,0E8h,00h,7Eh,00h,03h,00h,0E8h,00h,58h,00h
        db      0F0h,00h,58h,00h,0Ch,00h,98h,62h,0E0h,0FFh,6Ch,0BCh,0F1h,0FFh,39h,0D2h
        db      0D6h,0FFh,0D6h,00h,7Eh,00h,03h,00h,0F0h,00h,58h,00h,0A8h,00h,58h,00h
        db      0Eh,00h,68h,35h,0DBh,0FFh,48h,68h,0B8h,0FFh,78h,0CCh,1Fh,00h,0D6h,00h
        db      7Eh,00h,04h,00h,80h,00h,98h,00h,38h,00h,18h,01h,00h,00h,58h,00h
        db      0Ch,00h,9Ah,04h,0EFh,0FFh,0E6h,69h,0BBh,0FFh,0BDh,47h,0E0h,0FFh,0D6h,00h
        db      7Eh,00h,03h,00h,0A0h,00h,0A8h,00h,58h,00h,06h,00h,9Ah,3Fh,20h,00h
        db      0FEh,6Eh,0AAh,0FFh,62h,04h,0CBh,0FFh,66h,01h,02h,00h,00h,00h,58h,00h
        db      0Ch,00h,9Ah,3Fh,20h,00h,0FEh,6Eh,0AAh,0FFh,62h,04h,0CBh,0FFh,0D6h,00h
        db      7Eh,00h,03h,00h,68h,00h,0C0h,00h,60h,00h,58h,00h,0Ch,00h,0E1h,3Dh
        db      22h,00h,0A1h,6Dh,0AAh,0FFh,2Bh,17h,0AEh,0FFh,0D6h,00h,7Eh,00h,03h,00h
        db      0C0h,00h,0B0h,00h,60h,00h,06h,00h,0A5h,0F5h,0EFh,0FFh,0B6h,59h,0BBh,0FFh
        db      0B7h,5Ah,0E0h,0FFh,0E0h,00h,56h,00h,58h,00h,0Ch,00h,19h,0C7h,20h,00h
        db      0F9h,2Eh,0AAh,0FFh,9Ah,68h,0CBh,0FFh,0D6h,00h,7Eh,00h,03h,00h,68h,00h
        db      60h,00h,0B8h,00h,58h,00h,0Ch,00h,93h,03h,00h,00h,49h,79h,0B2h,0FFh
        db      0C1h,28h,0B6h,0FFh,0D6h,00h,7Eh,00h,03h,00h,0B8h,00h,40h,00h,68h,00h
        db      58h,00h,0Ch,00h,89h,96h,20h,00h,0A8h,42h,0AAh,0FFh,6Bh,0E3h,0CBh,0FFh
        db      0D6h,00h,7Eh,00h,03h,00h,68h,00h,40h,00h,88h,00h,00h,00h,58h,00h
        db      0Ch,00h,0A5h,0F5h,0EFh,0FFh,0B6h,59h,0BBh,0FFh,0B7h,5Ah,0E0h,0FFh,0D6h,00h
        db      7Eh,00h,03h,00h,0A0h,00h,50h,00h,40h,00h,06h,00h,0BAh,36h,00h,00h
        db      89h,4Eh,0B2h,0FFh,0F8h,54h,0B6h,0FFh,3Eh,00h,1Eh,00h,58h,00h,0Ch,00h
        db      0D0h,39h,00h,00h,30h,72h,0B2h,0FFh,0EFh,0FEh,0B6h,0FFh,0D6h,00h,7Eh,00h
        db      03h,00h,0B8h,00h,0A8h,00h,0A0h,00h,00h,00h,58h,00h,0Ch,00h,0DEh,31h
        db      0FAh,0FFh,88h,6Bh,0C1h,0FFh,50h,30h,0ACh,0FFh,0D6h,00h,7Eh,00h,03h,00h
        db      60h,00h,0B0h,00h,0A8h,00h,00h,00h,58h,00h,0Ch,00h,47h,61h,0EFh,0FFh
        db      2Ch,50h,0BBh,0FFh,31h,16h,0E0h,0FFh,0D6h,00h,7Eh,00h,03h,00h,0A0h,00h
        db      40h,00h,0B8h,00h,00h,00h,58h,00h,0Ch,00h,68h,35h,0F5h,0FFh,48h,68h
        db      0B3h,0FFh,78h,0CCh,2Eh,00h,0D6h,00h,7Eh,00h,03h,00h,00h,01h,80h,00h
        db      18h,01h,58h,00h,0Ch,00h,0B8h,03h,0EFh,0FFh,9Dh,55h,0BBh,0FFh,13h,5Fh
        db      0E0h,0FFh,0D6h,00h,7Eh,00h,03h,00h,0A0h,00h,58h,00h,50h,00h,00h,00h
        db      58h,00h,0Ch,00h,89h,96h,11h,00h,0A8h,42h,0B2h,0FFh,6Bh,0E3h,15h,00h
        db      0D6h,00h,7Eh,00h,03h,00h,00h,00h,68h,00h,88h,00h,58h,00h,0Ch,00h
        db      96h,0CCh,0F5h,0FFh,0CDh,0BEh,0F1h,0FFh,6Ah,61h,0C7h,0FFh,0D6h,00h,7Eh,00h
        db      03h,00h,0F8h,00h,0C8h,00h,70h,00h,58h,00h,0Eh,00h,0ABh,0B3h,1Dh,00h
        db      4Eh,0ABh,0F1h,0FFh,0D7h,0C5h,0EFh,0FFh,0D6h,00h,7Eh,00h,04h,00h,0D0h,00h
        db      08h,01h,10h,01h,0D8h,00h,58h,00h,0Ch,00h,22h,98h,35h,00h,88h,3Eh
        db      0B9h,0FFh,12h,29h,0FCh,0FFh,0D6h,00h,7Eh,00h,03h,00h,10h,00h,20h,00h
        db      0C8h,00h,58h,00h,0Ch,00h,0DCh,0Eh,24h,00h,0AFh,7Eh,0AEh,0FFh,52h,0F5h
        db      00h,00h,0D6h,00h,7Eh,00h,03h,00h,08h,00h,68h,00h,00h,00h,58h,00h
        db      0Ch,00h,0E9h,0B9h,24h,00h,0Bh,6Bh,0AEh,0FFh,7Dh,0FCh,00h,00h,0D6h,00h
        db      7Eh,00h,03h,00h,08h,00h,10h,00h,0C0h,00h,58h,00h,0Ch,00h,0C1h,0C4h
        db      24h,00h,63h,71h,0AEh,0FFh,0EAh,0FBh,00h,00h,0D6h,00h,7Eh,00h,03h,00h
        db      08h,00h,0C0h,00h,68h,00h,00h,00h,53h,00h,00h,00h,54h,00h,46h,00h
        db      3Fh,00h,7Ah,00h,0FFh,0FFh,07h,00h,28h,00h,00h,00h,78h,00h,00h,00h
        db      2Bh,00h,1Eh,00h,37h,00h,00h,00h,82h,00h,0Fh,00h,00h,00h,0F5h,0FFh
        db      0F2h,0FFh,34h,00h,0EAh,0FFh,37h,00h,39h,00h,0DDh,0FFh,03h,00h,30h,00h
        db      0EEh,0FFh,0E8h,0FFh,38h,00h,0D3h,0FFh,0FDh,0FFh,23h,00h,0E7h,0FFh,0E3h,0FFh
        db      2Bh,00h,0Dh,00h,0D0h,0FFh,29h,00h,0FDh,0FFh,0FCh,0FFh,46h,00h,23h,00h
        db      0E7h,0FFh,19h,00h,15h,00h,06h,00h,3Fh,00h,2Ah,00h,15h,00h,2Dh,00h
        db      04h,00h,38h,00h,2Ah,00h,0EFh,0FFh,19h,00h,0Ah,00h,0E8h,0FFh,0EAh,0FFh
        db      25h,00h,0FAh,0FFh,0F9h,0FFh,40h,00h,92h,00h,3Eh,00h,0E9h,0FFh,10h,00h
        db      78h,00h,92h,00h,11h,00h,0EEh,0FFh,00h,00h,80h,00h,90h,00h,0E1h,0FFh
        db      01h,00h,50h,00h,88h,00h,94h,00h,15h,00h,0F2h,0FFh,50h,00h,90h,00h
        db      94h,00h,39h,00h,0E0h,0FFh,48h,00h,98h,00h,92h,00h,0Bh,00h,3Fh,00h
        db      00h,00h,0A0h,00h,92h,00h,09h,00h,0EDh,0FFh,08h,00h,0A8h,00h,90h,00h
        db      0ACh,0FFh,0EBh,0FFh,50h,00h,0B0h,00h,92h,00h,0FDh,0FFh,0F2h,0FFh,60h,00h
        db      0B8h,00h,90h,00h,25h,00h,0E7h,0FFh,20h,00h,0C0h,00h,90h,00h,0Dh,00h
        db      0C4h,0FFh,38h,00h,0C8h,00h,92h,00h,29h,00h,0EFh,0FFh,60h,00h,0D0h,00h
        db      94h,00h,16h,00h,0CBh,0FFh,48h,00h,0D8h,00h,92h,00h,18h,00h,0Dh,00h
        db      60h,00h,0E0h,00h,94h,00h,11h,00h,0Ah,00h,28h,00h,0E8h,00h,92h,00h
        db      18h,00h,0ECh,0FFh,20h,00h,0F0h,00h,0D4h,00h,1Fh,00h,20h,29h,00h,00h
        db      04h,08h,00h,04h,10h,00h,04h,18h,00h,04h,20h,00h,02h,28h,00h,01h
        db      78h,00h,04h,80h,00h,04h,30h,00h,01h,38h,00h,04h,40h,00h,01h,48h
        db      00h,04h,50h,00h,03h,88h,00h,04h,90h,00h,03h,98h,00h,02h,0A0h,00h
        db      04h,58h,00h,04h,0A8h,00h,04h,0B0h,00h,01h,60h,00h,00h,0B8h,00h,00h
        db      0C0h,00h,00h,0C8h,00h,00h,0D0h,00h,01h,0D8h,00h,01h,0E0h,00h,01h,0E8h
        db      00h,05h,68h,00h,00h,0F0h,00h,00h,70h,00h,03h,90h,06h,00h,9Ah,0F5h
        db      03h,00h,75h,41h,0BAh,0FFh,7Fh,92h,04h,00h,0FAh,02h,4Ch,02h,58h,00h
        db      0Ch,00h,0B0h,8Bh,23h,00h,54h,31h,0D0h,0FFh,8Ah,14h,0FDh,0FFh,0D6h,00h
        db      7Eh,00h,03h,00h,10h,00h,0B0h,00h,08h,00h,58h,00h,0Eh,00h,4Dh,0CAh
        db      2Dh,00h,2Ch,0A3h,0DDh,0FFh,21h,0BAh,03h,00h,0D6h,00h,7Eh,00h,04h,00h
        db      20h,00h,28h,00h,68h,00h,0B8h,00h,58h,00h,0Ch,00h,0D6h,0A1h,12h,00h
        db      40h,46h,0C8h,0FFh,2Fh,0CDh,18h,00h,0D6h,00h,7Eh,00h,03h,00h,18h,00h
        db      28h,00h,20h,00h,58h,00h,0Eh,00h,5Bh,0A5h,12h,00h,83h,54h,0C8h,0FFh
        db      0F8h,0DFh,18h,00h,0D6h,00h,7Eh,00h,04h,00h,0E8h,00h,18h,00h,20h,00h
        db      10h,00h,58h,00h,0Ch,00h,91h,0E4h,0FAh,0FFh,7Bh,39h,0CCh,0FFh,0F5h,90h
        db      20h,00h,0D6h,00h,7Eh,00h,03h,00h,80h,00h,28h,00h,18h,00h,58h,00h
        db      0Ch,00h,9Eh,64h,0E5h,0FFh,93h,30h,0D0h,0FFh,8Ch,0C1h,14h,00h,0D6h,00h
        db      7Eh,00h,03h,00h,78h,00h,40h,00h,30h,00h,58h,00h,0Ch,00h,19h,5Ch
        db      0EBh,0FFh,86h,54h,0C1h,0FFh,7Eh,0E4h,0FAh,0FFh,0D6h,00h,7Eh,00h,03h,00h
        db      48h,00h,50h,00h,78h,00h,58h,00h,0Ch,00h,19h,36h,00h,00h,47h,50h
        db      0CCh,0FFh,0C4h,53h,0CFh,0FFh,0D6h,00h,7Eh,00h,03h,00h,0A0h,00h,58h,00h
        db      90h,00h,58h,00h,16h,00h,96h,31h,0E5h,0FFh,43h,6Ah,0D0h,0FFh,0A4h,0CCh
        db      14h,00h,0D6h,00h,7Eh,00h,03h,00h,78h,00h,30h,00h,80h,00h,7Eh,00h
        db      03h,00h,48h,00h,78h,00h,38h,00h,58h,00h,0Ch,00h,0BBh,70h,0DDh,0FFh
        db      6Ch,2Fh,0E7h,0FFh,3Ah,0DAh,19h,00h,0D6h,00h,7Eh,00h,03h,00h,40h,00h
        db      78h,00h,50h,00h,66h,00h,14h,00h,01h,80h,0F6h,0FFh,0D6h,00h,7Eh,00h
        db      07h,00h,60h,00h,0B8h,00h,0C0h,00h,0C8h,00h,0D0h,00h,0D8h,00h,0E0h,00h
        db      58h,00h,16h,00h,0E6h,0FBh,08h,00h,8Ch,97h,0F6h,0FFh,21h,0B6h,03h,00h
        db      0D6h,00h,7Eh,00h,03h,00h,0C0h,00h,30h,00h,0C8h,00h,7Eh,00h,03h,00h
        db      0C8h,00h,30h,00h,40h,00h,58h,00h,0Ch,00h,29h,23h,0DDh,0FFh,0F6h,8Bh
        db      0E7h,0FFh,0FAh,0D6h,19h,00h,0D6h,00h,7Eh,00h,03h,00h,40h,00h,0D0h,00h
        db      0C8h,00h,58h,00h,0Ch,00h,8Fh,3Ah,0D6h,0FFh,67h,9Eh,0E1h,0FFh,8Fh,3Ah
        db      0D6h,0FFh,0D6h,00h,7Eh,00h,03h,00h,90h,00h,98h,00h,0D8h,00h,58h,00h
        db      0Ch,00h,28h,2Ch,0EBh,0FFh,0F7h,98h,0F6h,0FFh,0D2h,3Dh,0E4h,0FFh,0D6h,00h
        db      7Eh,00h,03h,00h,0D8h,00h,98h,00h,0E0h,00h,58h,00h,0Ch,00h,8Ah,0C7h
        db      0EBh,0FFh,0F7h,0BFh,0E1h,0FFh,5Eh,5Fh,0C1h,0FFh,0D6h,00h,7Eh,00h,03h,00h
        db      98h,00h,08h,00h,0B0h,00h,58h,00h,0Ch,00h,88h,0CBh,2Ah,00h,0C1h,0B6h
        db      0E8h,0FFh,0E9h,5Ah,0EBh,0FFh,0D6h,00h,7Eh,00h,03h,00h,0B0h,00h,0E0h,00h
        db      98h,00h,58h,00h,0Ch,00h,27h,0C0h,11h,00h,0E4h,91h,0F6h,0FFh,0AEh,0Dh
        db      0E7h,0FFh,0D6h,00h,7Eh,00h,03h,00h,60h,00h,0B0h,00h,0B8h,00h,58h,00h
        db      0Ch,00h,0Ch,0B3h,14h,00h,0B8h,9Fh,0F6h,0FFh,7Eh,0DDh,0F5h,0FFh,0D6h,00h
        db      7Eh,00h,03h,00h,0B8h,00h,0B0h,00h,20h,00h,58h,00h,0Eh,00h,0A0h,0D2h
        db      08h,00h,63h,0A8h,0F6h,0FFh,78h,0AEh,03h,00h,0D6h,00h,7Eh,00h,04h,00h
        db      0C0h,00h,0F0h,00h,28h,00h,30h,00h,00h,00h,58h,00h,0Eh,00h,9Ah,0F5h
        db      03h,00h,75h,41h,0BAh,0FFh,7Fh,92h,04h,00h,0D6h,00h,7Eh,00h,04h,00h
        db      70h,00h,38h,00h,78h,00h,00h,00h,58h,00h,0Ch,00h,0E8h,0B7h,18h,00h
        db      2Ch,0AAh,0DBh,0FFh,34h,0C2h,16h,00h,0D6h,00h,7Eh,00h,03h,00h,68h,00h
        db      28h,00h,0F0h,00h,58h,00h,0Ch,00h,0Dh,0DDh,0FAh,0FFh,0CBh,5Dh,0CCh,0FFh
        db      3Ah,0B0h,20h,00h,0D6h,00h,7Eh,00h,03h,00h,80h,00h,30h,00h,28h,00h
        db      58h,00h,0Ch,00h,69h,09h,0Bh,00h,0ABh,78h,0CCh,0FFh,0ADh,29h,0Eh,00h
        db      0D6h,00h,7Eh,00h,03h,00h,00h,00h,18h,00h,0E8h,00h,58h,00h,0Ch,00h
        db      1Ch,16h,0E5h,0FFh,08h,7Eh,0D0h,0FFh,51h,03h,14h,00h,0D6h,00h,7Eh,00h
        db      03h,00h,78h,00h,80h,00h,18h,00h,58h,00h,0Ch,00h,6Eh,12h,0E5h,0FFh
        db      8Bh,79h,0D0h,0FFh,0B6h,23h,14h,00h,0D6h,00h,7Eh,00h,03h,00h,78h,00h
        db      18h,00h,00h,00h,00h,00h,58h,00h,0Ch,00h,69h,09h,19h,00h,0ABh,78h
        db      0CBh,0FFh,0ADh,29h,0Ch,00h,0D6h,00h,7Eh,00h,03h,00h,0E8h,00h,10h,00h
        db      00h,00h,06h,00h,66h,62h,0EBh,0FFh,0E1h,0BBh,0F6h,0FFh,96h,0D2h,0E4h,0FFh
        db      48h,00h,1Eh,00h,58h,00h,0Ch,00h,66h,62h,0EBh,0FFh,0E1h,0BBh,0F6h,0FFh
        db      96h,0D2h,0E4h,0FFh,0D6h,00h,7Eh,00h,03h,00h,0D8h,00h,50h,00h,90h,00h
        db      00h,00h,58h,00h,16h,00h,09h,70h,0DDh,0FFh,0D7h,0C2h,0E7h,0FFh,8Bh,09h
        db      19h,00h,0D6h,00h,7Eh,00h,03h,00h,40h,00h,50h,00h,0D0h,00h,7Eh,00h
        db      03h,00h,0D0h,00h,50h,00h,0D8h,00h,00h,00h,58h,00h,0Ch,00h,6Fh,03h
        db      0F5h,0FFh,78h,6Ah,0D2h,0FFh,0FBh,46h,0EBh,0FFh,0D6h,00h,7Eh,00h,03h,00h
        db      88h,00h,90h,00h,50h,00h,06h,00h,74h,3Dh,0Dh,00h,75h,6Ch,0C7h,0FFh
        db      1Ch,1Dh,0DCh,0FFh,22h,01h,02h,00h,00h,00h,58h,00h,16h,00h,74h,3Dh
        db      0Dh,00h,75h,6Ch,0C7h,0FFh,1Ch,1Dh,0DCh,0FFh,0D6h,00h,7Eh,00h,03h,00h
        db      0A8h,00h,08h,00h,58h,00h,7Eh,00h,03h,00h,08h,00h,98h,00h,58h,00h
        db      58h,00h,0Ch,00h,85h,0F3h,0F5h,0FFh,56h,58h,0D2h,0FFh,0CBh,5Bh,0EBh,0FFh
        db      0D6h,00h,7Eh,00h,03h,00h,88h,00h,48h,00h,38h,00h,06h,00h,19h,36h
        db      00h,00h,47h,50h,0CCh,0FFh,0C4h,53h,0CFh,0FFh,92h,00h,72h,00h,58h,00h
        db      0Ch,00h,0F1h,38h,00h,00h,9Dh,72h,0CCh,0FFh,0D0h,0FDh,0CFh,0FFh,0D6h,00h
        db      7Eh,00h,03h,00h,0A0h,00h,90h,00h,88h,00h,58h,00h,0Ch,00h,27h,0AFh
        db      0Dh,00h,30h,21h,0C7h,0FFh,9Dh,5Dh,0DCh,0FFh,0D6h,00h,7Eh,00h,03h,00h
        db      0A8h,00h,58h,00h,0A0h,00h,58h,00h,0Ch,00h,0C7h,05h,00h,00h,2Ch,79h
        db      0CCh,0FFh,0D3h,28h,0CFh,0FFh,0D6h,00h,7Eh,00h,03h,00h,0A0h,00h,38h,00h
        db      0A8h,00h,58h,00h,0Ch,00h,9Dh,8Dh,0Dh,00h,85h,38h,0C7h,0FFh,0C6h,0F5h
        db      0DCh,0FFh,0D6h,00h,7Eh,00h,03h,00h,0A8h,00h,38h,00h,70h,00h,00h,00h
        db      58h,00h,0Ch,00h,3Ah,31h,0FCh,0FFh,68h,6Bh,0D6h,0FFh,3Ah,31h,0C8h,0FFh
        db      0D6h,00h,7Eh,00h,03h,00h,58h,00h,98h,00h,90h,00h,00h,00h,58h,00h
        db      0Ch,00h,0FDh,61h,0F5h,0FFh,83h,4Fh,0D2h,0FFh,75h,15h,0EBh,0FFh,0D6h,00h
        db      7Eh,00h,03h,00h,88h,00h,38h,00h,0A0h,00h,58h,00h,0Ch,00h,0B3h,02h
        db      0F5h,0FFh,0C4h,53h,0D2h,0FFh,0BDh,60h,0EBh,0FFh,0D6h,00h,7Eh,00h,03h,00h
        db      88h,00h,50h,00h,48h,00h,00h,00h,58h,00h,0Ch,00h,9Dh,8Dh,0Bh,00h
        db      85h,38h,0CCh,0FFh,0C6h,0F5h,0Eh,00h,0D6h,00h,7Eh,00h,03h,00h,00h,00h
        db      0A8h,00h,70h,00h,58h,00h,0Ch,00h,0B3h,0CBh,0F9h,0FFh,32h,0BEh,0F6h,0FFh
        db      8Eh,60h,0DAh,0FFh,0D6h,00h,7Eh,00h,03h,00h,0E0h,00h,0B0h,00h,60h,00h
        db      58h,00h,0Eh,00h,0E8h,0B7h,14h,00h,2Ch,0AAh,0F6h,0FFh,34h,0C2h,0F5h,0FFh
        db      0D6h,00h,7Eh,00h,04h,00h,0B8h,00h,68h,00h,0F0h,00h,0C0h,00h,58h,00h
        db      0Ch,00h,81h,97h,23h,00h,5Fh,3Dh,0D0h,0FFh,30h,29h,0FDh,0FFh,0D6h,00h
        db      7Eh,00h,03h,00h,10h,00h,20h,00h,0B0h,00h,58h,00h,16h,00h,0A2h,0E1h
        db      0Bh,00h,94h,7Bh,0CCh,0FFh,35h,0F2h,0Eh,00h,0D6h,00h,7Eh,00h,03h,00h
        db      00h,00h,10h,00h,08h,00h,7Eh,00h,03h,00h,08h,00h,0A8h,00h,00h,00h
        db      00h,00h,20h,00h,00h,00h,1Dh,00h,16h,00h,1Fh,00h,7Ah,00h,00h,00h
        db      03h,00h,0Ch,00h,00h,00h,78h,00h,00h,00h,10h,00h,09h,00h,15h,00h
        db      00h,00h,82h,00h,08h,00h,00h,00h,02h,00h,0F4h,0FFh,14h,00h,0F8h,0FFh
        db      15h,00h,16h,00h,0F7h,0FFh,0F5h,0FFh,10h,00h,0EFh,0FFh,0FFh,0FFh,0Dh,00h
        db      04h,00h,0EEh,0FFh,0Fh,00h,0Dh,00h,0F7h,0FFh,09h,00h,10h,00h,08h,00h
        db      11h,00h,0FAh,0FFh,09h,00h,03h,00h,92h,00h,04h,00h,0F9h,0FFh,08h,00h
        db      40h,00h,94h,00h,08h,00h,0FBh,0FFh,30h,00h,48h,00h,92h,00h,0F8h,0FFh
        db      08h,00h,48h,00h,50h,00h,92h,00h,0FEh,0FFh,1Fh,00h,00h,00h,58h,00h
        db      90h,00h,09h,00h,0FAh,0FFh,08h,00h,60h,00h,92h,00h,0E3h,0FFh,11h,00h
        db      28h,00h,68h,00h,92h,00h,0FFh,0FFh,0FBh,0FFh,38h,00h,70h,00h,90h,00h
        db      0Eh,00h,0F6h,0FFh,18h,00h,78h,00h,86h,00h,78h,00h,06h,00h,80h,00h
        db      92h,00h,0Fh,00h,0FAh,0FFh,38h,00h,88h,00h,92h,00h,0Eh,00h,01h,00h
        db      38h,00h,90h,00h,94h,00h,1Ah,00h,0EFh,0FFh,00h,00h,98h,00h,0D4h,00h
        db      14h,00h,20h,29h,00h,00h,04h,08h,00h,04h,10h,00h,03h,18h,00h,01h
        db      40h,00h,04h,20h,00h,02h,28h,00h,02h,30h,00h,03h,48h,00h,03h,50h
        db      00h,02h,58h,00h,04h,60h,00h,05h,68h,00h,02h,38h,00h,00h,70h,00h
        db      00h,78h,00h,00h,80h,00h,00h,88h,00h,01h,90h,00h,01h,98h,00h,00h
        db      06h,00h,88h,2Ch,04h,00h,0C9h,65h,0EAh,0FFh,9Dh,3Fh,0F2h,0FFh,7Ch,02h
        db      0Ch,02h,58h,00h,0Ch,00h,88h,2Ch,04h,00h,0C9h,65h,0EAh,0FFh,9Dh,3Fh
        db      0F2h,0FFh,0D6h,00h,7Eh,00h,03h,00h,40h,00h,48h,00h,30h,00h,58h,00h
        db      0Ch,00h,4Eh,0A0h,04h,00h,0F1h,11h,0EAh,0FFh,0BCh,53h,0F2h,0FFh,0D6h,00h
        db      7Eh,00h,03h,00h,40h,00h,60h,00h,58h,00h,58h,00h,0Ch,00h,2Ch,0D3h
        db      0FEh,0FFh,17h,76h,0ECh,0FFh,43h,0EBh,0Ch,00h,0D6h,00h,7Eh,00h,03h,00h
        db      00h,00h,10h,00h,08h,00h,58h,00h,0Ch,00h,0A4h,0D8h,0FEh,0FFh,0DEh,56h
        db      0ECh,0FFh,7Eh,0AAh,0Ch,00h,0D6h,00h,7Eh,00h,03h,00h,00h,00h,20h,00h
        db      10h,00h,58h,00h,0Ch,00h,0FCh,31h,00h,00h,7Bh,3Eh,0ECh,0FFh,0F8h,63h
        db      0EDh,0FFh,0D6h,00h,7Eh,00h,03h,00h,58h,00h,60h,00h,48h,00h,58h,00h
        db      0Ch,00h,01h,0E4h,08h,00h,0FFh,7Bh,0EAh,0FFh,01h,0F0h,0EBh,0FFh,0D6h,00h
        db      7Eh,00h,03h,00h,08h,00h,40h,00h,00h,00h,58h,00h,0Ch,00h,6Eh,98h
        db      09h,00h,76h,4Ah,0F0h,0FFh,47h,0F5h,0Bh,00h,0D6h,00h,7Eh,00h,03h,00h
        db      10h,00h,68h,00h,08h,00h,66h,00h,14h,00h,01h,80h,0FDh,0FFh,0D6h,00h
        db      7Eh,00h,07h,00h,38h,00h,70h,00h,78h,00h,80h,00h,88h,00h,90h,00h
        db      98h,00h,5Eh,00h,16h,00h,64h,97h,0FDh,0FFh,28h,0B6h,01h,00h,0D6h,00h
        db      7Eh,00h,03h,00h,78h,00h,20h,00h,80h,00h,7Eh,00h,03h,00h,80h,00h
        db      20h,00h,28h,00h,58h,00h,0Ch,00h,58h,1Fh,0F3h,0FFh,11h,8Dh,0F7h,0FFh
        db      0FCh,0D0h,09h,00h,0D6h,00h,7Eh,00h,03h,00h,28h,00h,88h,00h,80h,00h
        db      58h,00h,0Ch,00h,0E9h,3Ch,0F0h,0FFh,41h,0A1h,0F4h,0FFh,0E9h,3Ch,0F0h,0FFh
        db      0D6h,00h,7Eh,00h,03h,00h,48h,00h,50h,00h,90h,00h,58h,00h,0Ch,00h
        db      6Dh,2Bh,0F8h,0FFh,0ABh,9Ah,0FDh,0FFh,24h,41h,0F6h,0FFh,0D6h,00h,7Eh,00h
        db      03h,00h,90h,00h,50h,00h,98h,00h,58h,00h,18h,00h,42h,0C7h,0F8h,0FFh
        db      38h,0C2h,0F4h,0FFh,0B3h,60h,0E8h,0FFh,0D6h,00h,7Eh,00h,03h,00h,50h,00h
        db      08h,00h,68h,00h,0D6h,00h,7Eh,00h,03h,00h,68h,00h,98h,00h,50h,00h
        db      58h,00h,0Ch,00h,75h,0C9h,0FEh,0FFh,0A4h,0B3h,0FDh,0FFh,45h,57h,0F2h,0FFh
        db      0D6h,00h,7Eh,00h,03h,00h,98h,00h,68h,00h,38h,00h,58h,00h,0Ch,00h
        db      74h,0BDh,06h,00h,4Eh,93h,0FDh,0FFh,4Fh,0Dh,0F7h,0FFh,0D6h,00h,7Eh,00h
        db      03h,00h,38h,00h,68h,00h,70h,00h,58h,00h,0Ch,00h,7Ah,0B1h,07h,00h
        db      0A6h,0A0h,0FDh,0FFh,59h,0DEh,0FCh,0FFh,0D6h,00h,7Eh,00h,03h,00h,70h,00h
        db      68h,00h,18h,00h,58h,00h,0Ch,00h,0D5h,0C8h,11h,00h,22h,0A5h,0F3h,0FFh
        db      9Bh,0B8h,01h,00h,0D6h,00h,7Eh,00h,03h,00h,18h,00h,10h,00h,70h,00h
        db      58h,00h,0Ch,00h,0CCh,0CDh,03h,00h,81h,0AAh,0FDh,0FFh,0FBh,0AEh,01h,00h
        db      0D6h,00h,7Eh,00h,03h,00h,78h,00h,10h,00h,20h,00h,00h,00h,58h,00h
        db      0Ch,00h,0B4h,43h,04h,00h,8Fh,65h,0EAh,0FFh,0B0h,26h,0F2h,0FFh,0D6h,00h
        db      7Eh,00h,03h,00h,40h,00h,08h,00h,60h,00h,58h,00h,0Ch,00h,0D0h,45h
        db      08h,00h,0B9h,68h,0EAh,0FFh,0BBh,0E8h,0EBh,0FFh,0D6h,00h,7Eh,00h,03h,00h
        db      08h,00h,50h,00h,60h,00h,58h,00h,0Ch,00h,91h,2Ch,0FFh,0FFh,6Ch,6Fh
        db      0F0h,0FFh,91h,2Ch,0EBh,0FFh,0D6h,00h,7Eh,00h,03h,00h,60h,00h,50h,00h
        db      48h,00h,62h,00h,0Ch,00h,46h,39h,00h,00h,8Ch,72h,0ECh,0FFh,0D6h,00h
        db      7Eh,00h,03h,00h,58h,00h,48h,00h,40h,00h,00h,00h,06h,00h,00h,7Ch
        db      04h,00h,0D0h,10h,0EAh,0FFh,52h,1Bh,0F2h,0FFh,0B2h,00h,28h,00h,58h,00h
        db      16h,00h,23h,71h,0F3h,0FFh,93h,0C4h,0F7h,0FFh,0FFh,07h,09h,00h,0D6h,00h
        db      7Eh,00h,03h,00h,28h,00h,30h,00h,88h,00h,7Eh,00h,03h,00h,88h,00h
        db      30h,00h,90h,00h,00h,00h,58h,00h,0Ch,00h,00h,7Ch,04h,00h,0D0h,10h
        db      0EAh,0FFh,52h,1Bh,0F2h,0FFh,0D6h,00h,7Eh,00h,03h,00h,40h,00h,20h,00h
        db      00h,00h,06h,00h,0D6h,0Bh,0F3h,0FFh,87h,72h,0F7h,0FFh,05h,0C8h,09h,00h
        db      3Eh,00h,1Eh,00h,58h,00h,0Ch,00h,0D6h,0Bh,0F3h,0FFh,87h,72h,0F7h,0FFh
        db      05h,0C8h,09h,00h,0D6h,00h,7Eh,00h,03h,00h,28h,00h,40h,00h,30h,00h
        db      00h,00h,58h,00h,0Ch,00h,0F1h,49h,04h,00h,64h,68h,0EAh,0FFh,0A7h,0FBh
        db      0F2h,0FFh,0D6h,00h,7Eh,00h,03h,00h,40h,00h,28h,00h,20h,00h,00h,00h
        db      58h,00h,0Ch,00h,0F6h,66h,0F8h,0FFh,66h,0BFh,0FDh,0FFh,0A0h,0D7h,0F6h,0FFh
        db      0D6h,00h,7Eh,00h,03h,00h,90h,00h,30h,00h,48h,00h,00h,00h,58h,00h
        db      0Ch,00h,77h,0B3h,07h,00h,93h,0ADh,0FDh,0FFh,0C5h,0C2h,0FCh,0FFh,0D6h,00h
        db      7Eh,00h,03h,00h,70h,00h,10h,00h,78h,00h,58h,00h,0Ch,00h,7Dh,12h
        db      09h,00h,9Eh,74h,0F0h,0FFh,0C6h,31h,0Bh,00h,0D6h,00h,7Eh,00h,03h,00h
        db      10h,00h,18h,00h,68h,00h,00h,00h,0Bh,04h,00h,00h,0C0h,00h,00h,04h
        db      08h,00h,7Ah,00h,10h,00h,04h,00h,00h,02h,00h,00h,78h,00h,00h,00h
        db      80h,00h,00h,02h,04h,00h,00h,00h,7Ah,00h,0D0h,0FFh,00h,00h,00h,00h
        db      00h,00h,88h,00h,00h,00h,0D0h,00h,08h,00h,86h,00h,08h,00h,80h,00h
        db      10h,00h,86h,00h,00h,00h,80h,00h,18h,00h,8Ah,00h,00h,00h,08h,00h
        db      20h,00h,8Ah,00h,08h,00h,08h,00h,28h,00h,8Ah,00h,10h,00h,08h,00h
        db      30h,00h,8Ah,00h,18h,00h,08h,00h,38h,00h,86h,00h,00h,00h,0C0h,0FFh
        db      40h,00h,86h,00h,20h,00h,0C0h,0FFh,48h,00h,86h,00h,00h,00h,0C0h,00h
        db      50h,00h,86h,00h,20h,00h,0C0h,00h,58h,00h,86h,00h,08h,00h,0C0h,0FFh
        db      60h,00h,86h,00h,28h,00h,0C0h,0FFh,68h,00h,86h,00h,08h,00h,0C0h,00h
        db      70h,00h,86h,00h,28h,00h,0C0h,00h,78h,00h,8Ch,00h,60h,00h,00h,08h
        db      80h,00h,8Ch,00h,68h,00h,00h,08h,88h,00h,8Ch,00h,70h,00h,00h,08h
        db      90h,00h,8Ch,00h,78h,00h,00h,08h,98h,00h,14h,00h,74h,00h,26h,29h
        db      00h,00h,64h,00h,12h,00h,0FFh,7Fh,30h,00h,0BCh,00h,20h,29h,01h,00h
        db      7Eh,00h,04h,00h,00h,00h,08h,00h,28h,00h,20h,00h,66h,00h,12h,00h
        db      01h,80h,30h,0FFh,0BCh,00h,20h,29h,02h,00h,7Eh,00h,04h,00h,08h,00h
        db      10h,00h,30h,00h,28h,00h,64h,00h,12h,00h,01h,80h,0B0h,0FFh,0BCh,00h
        db      20h,29h,01h,00h,7Eh,00h,04h,00h,10h,00h,18h,00h,38h,00h,30h,00h
        db      66h,00h,1Eh,00h,01h,80h,00h,00h,0BCh,00h,20h,29h,02h,00h,7Eh,00h
        db      04h,00h,00h,00h,20h,00h,48h,00h,40h,00h,7Eh,00h,04h,00h,38h,00h
        db      18h,00h,50h,00h,58h,00h,68h,00h,60h,00h,01h,80h,0F0h,0FFh,0BCh,00h
        db      20h,29h,00h,00h,0B4h,00h,04h,00h,80h,00h,00h,00h,00h,00h,90h,00h
        db      0FFh,0FFh,00h,00h,70h,00h,0FFh,0FFh,0FFh,0FFh,60h,00h,00h,00h,0FFh,0FFh
        db      0BEh,00h,2Eh,29h,2Ch,29h,0B4h,00h,04h,00h,60h,00h,00h,00h,00h,30h
        db      08h,00h,0FFh,3Fh,00h,30h,00h,00h,0FFh,3Fh,0FFh,0FFh,40h,00h,00h,00h
        db      0FFh,0FFh,0B4h,00h,04h,00h,10h,00h,00h,0C0h,00h,30h,70h,00h,0FFh,0FFh
        db      00h,30h,50h,00h,0FFh,0FFh,0FFh,0FFh,18h,00h,00h,0C0h,0FFh,0FFh,68h,00h
        db      60h,00h,0FFh,7Fh,0E8h,0FFh,0BCh,00h,20h,29h,00h,00h,0B4h,00h,04h,00h
        db      98h,00h,0FFh,0FFh,00h,00h,88h,00h,00h,00h,00h,00h,68h,00h,00h,00h
        db      0FFh,0FFh,78h,00h,0FFh,0FFh,0FFh,0FFh,0BEh,00h,2Eh,29h,2Ch,29h,0B4h,00h
        db      04h,00h,28h,00h,0FFh,3Fh,00h,30h,68h,00h,00h,00h,00h,30h,48h,00h
        db      00h,00h,0FFh,0FFh,20h,00h,0FFh,3Fh,0FFh,0FFh,0B4h,00h,04h,00h,78h,00h
        db      0FFh,0FFh,00h,30h,30h,00h,00h,0C0h,00h,30h,38h,00h,00h,0C0h,0FFh,0FFh
        db      58h,00h,0FFh,0FFh,0FFh,0FFh,00h,00h,01h,01h,00h,00h,10h,00h,10h,00h
        db      00h,01h,7Ah,00h,08h,00h,80h,00h,08h,00h,00h,00h,78h,00h,00h,00h
        db      08h,00h,08h,00h,80h,00h,00h,00h,7Ah,00h,00h,00h,00h,00h,00h,00h
        db      00h,00h,88h,00h,00h,00h,10h,00h,08h,00h,86h,00h,08h,00h,10h,00h
        db      10h,00h,86h,00h,00h,00h,10h,00h,18h,00h,8Ah,00h,00h,00h,00h,01h
        db      20h,00h,8Ah,00h,08h,00h,00h,01h,28h,00h,8Ah,00h,10h,00h,00h,01h
        db      30h,00h,8Ah,00h,18h,00h,00h,01h,38h,00h,0D4h,00h,08h,00h,20h,29h
        db      00h,00h,00h,08h,00h,03h,10h,00h,04h,18h,00h,00h,20h,00h,00h,28h
        db      00h,04h,30h,00h,05h,38h,00h,01h,68h,00h,0Eh,00h,01h,80h,00h,00h
        db      0D6h,00h,7Eh,00h,04h,00h,00h,00h,08h,00h,10h,00h,18h,00h,68h,00h
        db      0Eh,00h,0FFh,7Fh,00h,0FFh,0D6h,00h,7Eh,00h,04h,00h,38h,00h,30h,00h
        db      28h,00h,20h,00h,64h,00h,0Eh,00h,01h,80h,00h,00h,0D6h,00h,7Eh,00h
        db      04h,00h,20h,00h,28h,00h,08h,00h,00h,00h,66h,00h,0Eh,00h,0FFh,7Fh
        db      0F0h,0FFh,0D6h,00h,7Eh,00h,04h,00h,28h,00h,30h,00h,10h,00h,08h,00h
        db      64h,00h,0Eh,00h,0FFh,7Fh,0F0h,0FFh,0D6h,00h,7Eh,00h,04h,00h,18h,00h
        db      10h,00h,30h,00h,38h,00h,66h,00h,0Eh,00h,01h,80h,00h,00h,0D6h,00h
        db      7Eh,00h,04h,00h,00h,00h,18h,00h,38h,00h,20h,00h,00h,00h,01h,04h
        db      00h,00h,20h,00h,00h,04h,20h,00h,7Ah,00h,00h,00h,00h,00h,00h,02h
        db      00h,00h,78h,00h,00h,00h,10h,00h,00h,02h,10h,00h,00h,00h,7Ah,00h
        db      0F0h,0FFh,0F0h,0FFh,00h,00h,00h,00h,8Ah,00h,00h,00h,20h,00h,08h,00h
        db      86h,00h,08h,00h,20h,00h,10h,00h,86h,00h,00h,00h,20h,00h,18h,00h
        db      8Ch,00h,00h,00h,00h,08h,20h,00h,8Ch,00h,08h,00h,00h,08h,28h,00h
        db      8Ch,00h,10h,00h,00h,08h,30h,00h,8Ch,00h,18h,00h,00h,08h,38h,00h
        db      68h,00h,0Eh,00h,01h,80h,10h,00h,0BCh,00h,20h,29h,01h,00h,0A0h,00h
        db      06h,00h,04h,07h,03h,00h,64h,00h,0Eh,00h,0FFh,7Fh,0F0h,0FFh,0BCh,00h
        db      20h,29h,00h,00h,0A0h,00h,06h,00h,07h,06h,02h,03h,68h,00h,0Eh,00h
        db      0FFh,7Fh,0F0h,0FFh,0BCh,00h,20h,29h,01h,00h,0A0h,00h,06h,00h,06h,05h
        db      01h,02h,64h,00h,0Eh,00h,01h,80h,10h,00h,0BCh,00h,20h,29h,00h,00h
        db      0A0h,00h,06h,00h,05h,04h,00h,01h,00h,00h,80h,00h,00h,00h,40h,00h
        db      60h,00h,08h,00h,7Ah,00h,00h,00h,00h,00h,30h,00h,00h,00h,78h,00h
        db      00h,00h,20h,00h,30h,00h,04h,00h,00h,00h,7Ah,00h,0E0h,0FFh,0FCh,0FFh
        db      00h,00h,00h,00h,88h,00h,00h,00h,60h,00h,08h,00h,86h,00h,08h,00h
        db      40h,00h,10h,00h,86h,00h,00h,00h,40h,00h,18h,00h,8Ah,00h,00h,00h
        db      08h,00h,20h,00h,8Ah,00h,08h,00h,08h,00h,28h,00h,8Ah,00h,10h,00h
        db      08h,00h,30h,00h,8Ah,00h,18h,00h,08h,00h,38h,00h,0BCh,00h,20h,29h
        db      01h,00h,68h,00h,1Eh,00h,01h,80h,04h,00h,0A8h,00h,06h,00h,04h,00h
        db      08h,00h,0FFh,0FFh,00h,40h,10h,00h,00h,00h,00h,40h,18h,00h,00h,00h
        db      0FFh,0FFh,00h,00h,0FFh,0FFh,0FFh,0FFh,68h,00h,1Eh,00h,0FFh,7Fh,0FCh,0FFh
        db      0A8h,00h,06h,00h,04h,00h,30h,00h,00h,00h,00h,40h,28h,00h,0FFh,0FFh
        db      00h,40h,20h,00h,0FFh,0FFh,0FFh,0FFh,38h,00h,00h,00h,0FFh,0FFh,64h,00h
        db      12h,00h,01h,80h,00h,00h,0BCh,00h,20h,29h,00h,00h,7Eh,00h,04h,00h
        db      20h,00h,28h,00h,08h,00h,00h,00h,66h,00h,12h,00h,0FFh,7Fh,0A0h,0FFh
        db      0BCh,00h,20h,29h,02h,00h,7Eh,00h,04h,00h,28h,00h,30h,00h,10h,00h
        db      08h,00h,64h,00h,12h,00h,0FFh,7Fh,0C0h,0FFh,0BCh,00h,20h,29h,00h,00h
        db      7Eh,00h,04h,00h,18h,00h,10h,00h,30h,00h,38h,00h,66h,00h,12h,00h
        db      01h,80h,00h,00h,0BCh,00h,20h,29h,02h,00h,7Eh,00h,04h,00h,00h,00h
        db      18h,00h,38h,00h,20h,00h,00h,00h,90h,01h,00h,00h,0C0h,00h,40h,01h
        db      01h,00h,7Ah,00h,60h,00h,01h,00h,0A0h,00h,00h,00h,78h,00h,00h,00h
        db      62h,00h,0A0h,00h,01h,00h,00h,00h,7Ah,00h,0A0h,0FFh,00h,00h,00h,00h
        db      00h,00h,86h,00h,00h,00h,0C0h,00h,08h,00h,88h,00h,08h,00h,40h,01h
        db      10h,00h,88h,00h,00h,00h,40h,01h,18h,00h,86h,00h,00h,00h,60h,00h
        db      20h,00h,88h,00h,00h,00h,0A0h,00h,28h,00h,86h,00h,28h,00h,0C0h,00h
        db      30h,00h,88h,00h,20h,00h,40h,01h,38h,00h,86h,00h,28h,00h,60h,00h
        db      40h,00h,0D4h,00h,09h,00h,20h,29h,00h,00h,05h,08h,00h,05h,10h,00h
        db      05h,18h,00h,05h,20h,00h,02h,28h,00h,02h,30h,00h,02h,38h,00h,02h
        db      40h,00h,00h,90h,0D6h,00h,7Eh,00h,04h,00h,28h,00h,40h,00h,20h,00h
        db      00h,00h,0D6h,00h,7Eh,00h,04h,00h,18h,00h,38h,00h,40h,00h,28h,00h
        db      0D6h,00h,7Eh,00h,04h,00h,38h,00h,10h,00h,30h,00h,40h,00h,0D6h,00h
        db      7Eh,00h,04h,00h,40h,00h,30h,00h,08h,00h,20h,00h,00h,00h,38h,00h
        db      00h,00h,0Eh,00h,0Eh,00h,60h,00h,7Ah,00h,00h,00h,04h,00h,00h,00h
        db      00h,00h,78h,00h,00h,00h,07h,00h,07h,00h,34h,00h,00h,00h,82h,00h
        db      02h,00h,00h,00h,0FEh,0FFh,0D0h,0FFh,0FEh,0FFh,00h,00h,38h,00h,00h,00h
        db      86h,00h,00h,00h,04h,00h,10h,00h,88h,00h,10h,00h,04h,00h,18h,00h
        db      88h,00h,00h,00h,04h,00h,20h,00h,8Ah,00h,00h,00h,60h,00h,28h,00h
        db      8Ah,00h,10h,00h,60h,00h,30h,00h,8Ah,00h,18h,00h,60h,00h,38h,00h
        db      8Ah,00h,20h,00h,60h,00h,40h,00h,90h,00h,0FBh,0FFh,0FBh,0FFh,00h,00h
        db      48h,00h,86h,00h,48h,00h,0Eh,00h,50h,00h,88h,00h,50h,00h,0Eh,00h
        db      58h,00h,88h,00h,48h,00h,0Eh,00h,60h,00h,8Ah,00h,00h,00h,06h,00h
        db      68h,00h,8Ah,00h,10h,00h,06h,00h,70h,00h,8Ah,00h,18h,00h,06h,00h
        db      78h,00h,8Ah,00h,20h,00h,06h,00h,80h,00h,0D4h,00h,08h,00h,20h,29h
        db      00h,00h,00h,10h,00h,01h,18h,00h,03h,20h,00h,03h,28h,00h,00h,30h
        db      00h,00h,38h,00h,05h,40h,00h,04h,10h,00h,70h,0A5h,02h,00h,90h,5Ah
        db      02h,00h,16h,01h,88h,00h,68h,00h,0Eh,00h,01h,80h,30h,00h,0D6h,00h
        db      7Eh,00h,04h,00h,20h,00h,18h,00h,10h,00h,00h,00h,5Eh,00h,10h,00h
        db      82h,83h,02h,00h,1Fh,1Fh,0D0h,0FFh,0BCh,00h,22h,29h,00h,00h,7Eh,00h
        db      03h,00h,28h,00h,30h,00h,08h,00h,60h,00h,10h,00h,7Eh,7Ch,0FEh,0FFh
        db      1Fh,1Fh,0D0h,0FFh,0BCh,00h,22h,29h,02h,00h,7Eh,00h,03h,00h,30h,00h
        db      38h,00h,08h,00h,5Eh,00h,10h,00h,7Eh,7Ch,0FEh,0FFh,1Fh,1Fh,0D0h,0FFh
        db      0BCh,00h,22h,29h,03h,00h,7Eh,00h,03h,00h,38h,00h,40h,00h,08h,00h
        db      60h,00h,10h,00h,82h,83h,02h,00h,1Fh,1Fh,0D0h,0FFh,0BCh,00h,22h,29h
        db      01h,00h,7Eh,00h,03h,00h,40h,00h,28h,00h,08h,00h,00h,00h,62h,00h
        db      1Ah,00h,70h,0A5h,02h,00h,90h,5Ah,02h,00h,0BCh,00h,22h,29h,02h,00h
        db      7Eh,00h,03h,00h,00h,00h,48h,00h,68h,00h,7Eh,00h,03h,00h,78h,00h
        db      58h,00h,18h,00h,66h,00h,0Eh,00h,0FFh,7Fh,0FEh,0FFh,0D6h,00h,7Eh,00h
        db      04h,00h,18h,00h,20h,00h,40h,00h,38h,00h,64h,00h,0Eh,00h,01h,80h
        db      02h,00h,0D6h,00h,7Eh,00h,04h,00h,20h,00h,00h,00h,28h,00h,40h,00h
        db      62h,00h,10h,00h,90h,5Ah,02h,00h,90h,5Ah,0FEh,0FFh,0BCh,00h,22h,29h
        db      03h,00h,7Eh,00h,03h,00h,20h,00h,60h,00h,80h,00h,62h,00h,10h,00h
        db      70h,0A5h,02h,00h,70h,0A5h,0FEh,0FFh,0BCh,00h,22h,29h,00h,00h,7Eh,00h
        db      03h,00h,80h,00h,60h,00h,20h,00h,00h,00h,62h,00h,1Ah,00h,90h,5Ah
        db      0FEh,0FFh,70h,0A5h,0FEh,0FFh,0BCh,00h,22h,29h,01h,00h,7Eh,00h,03h,00h
        db      18h,00h,58h,00h,78h,00h,7Eh,00h,03h,00h,68h,00h,48h,00h,00h,00h
        db      66h,00h,0Eh,00h,01h,80h,02h,00h,0D6h,00h,7Eh,00h,04h,00h,00h,00h
        db      10h,00h,30h,00h,28h,00h,64h,00h,0Eh,00h,0FFh,7Fh,0FEh,0FFh,0D6h,00h
        db      7Eh,00h,04h,00h,10h,00h,18h,00h,38h,00h,30h,00h,62h,00h,10h,00h
        db      70h,0A5h,0FEh,0FFh,70h,0A5h,02h,00h,0BCh,00h,22h,29h,00h,00h,7Eh,00h
        db      03h,00h,10h,00h,50h,00h,70h,00h,62h,00h,10h,00h,90h,5Ah,0FEh,0FFh
        db      90h,5Ah,02h,00h,0BCh,00h,22h,29h,03h,00h,7Eh,00h,03h,00h,70h,00h
        db      50h,00h,10h,00h,00h,00h,0DAh,00h,00h,00h,80h,00h,0D0h,00h,04h,00h
        db      7Ah,00h,10h,00h,04h,00h,68h,00h,00h,00h,78h,00h,00h,00h,40h,00h
        db      68h,00h,00h,00h,00h,00h,7Ah,00h,00h,00h,04h,00h,00h,00h,0F8h,03h
        db      86h,00h,0F8h,03h,0D0h,0FFh,0F0h,03h,16h,00h,48h,00h,2Ah,29h,04h,00h
        db      88h,00h,0F0h,03h,2Ah,00h,00h,00h,12h,00h,0E2h,00h,16h,00h,34h,00h
        db      2Ah,29h,03h,00h,88h,00h,0F0h,03h,54h,00h,00h,00h,12h,00h,0CEh,00h
        db      16h,00h,20h,00h,2Ah,29h,02h,00h,88h,00h,0F0h,03h,7Eh,00h,00h,00h
        db      12h,00h,0BAh,00h,16h,00h,0Ch,00h,2Ah,29h,01h,00h,88h,00h,0F0h,03h
        db      0A8h,00h,00h,00h,12h,00h,0A6h,00h,86h,00h,0F8h,03h,0EAh,0FFh,00h,00h
        db      12h,00h,1Ah,00h,86h,00h,0F8h,03h,10h,00h,00h,00h,12h,00h,0Eh,00h
        db      86h,00h,0F8h,03h,36h,00h,00h,00h,12h,00h,02h,00h,00h,00h,86h,00h
        db      00h,00h,0FCh,0FFh,08h,00h,86h,00h,00h,00h,04h,00h,10h,00h,8Ch,00h
        db      00h,00h,00h,04h,18h,00h,86h,00h,18h,00h,0FCh,0FFh,20h,00h,86h,00h
        db      18h,00h,04h,00h,28h,00h,0D4h,00h,06h,00h,20h,29h,00h,00h,00h,08h
        db      00h,07h,10h,00h,07h,18h,00h,00h,20h,00h,07h,28h,00h,07h,0D6h,00h
        db      68h,00h,1Ch,00h,01h,80h,0F0h,0FFh,7Eh,00h,04h,00h,18h,00h,28h,00h
        db      10h,00h,00h,00h,7Eh,00h,04h,00h,20h,00h,18h,00h,00h,00h,08h,00h
        db      48h,00h,18h,00h,7Eh,00h,04h,00h,28h,00h,18h,00h,00h,00h,10h,00h
        db      7Eh,00h,04h,00h,18h,00h,20h,00h,00h,00h,08h,00h,00h,00h,88h,00h
        db      00h,00h,0FAh,0FFh,08h,00h,88h,00h,00h,00h,06h,00h,10h,00h,86h,00h
        db      00h,00h,80h,00h,18h,00h,88h,00h,18h,00h,0FAh,0FFh,20h,00h,88h,00h
        db      18h,00h,06h,00h,28h,00h,0D4h,00h,06h,00h,20h,29h,00h,00h,00h,08h
        db      00h,07h,10h,00h,07h,18h,00h,00h,20h,00h,07h,28h,00h,07h,0D6h,00h
        db      68h,00h,1Ch,00h,01h,80h,0F0h,0FFh,7Eh,00h,04h,00h,10h,00h,28h,00h
        db      18h,00h,00h,00h,7Eh,00h,04h,00h,00h,00h,18h,00h,20h,00h,08h,00h
        db      48h,00h,18h,00h,7Eh,00h,04h,00h,28h,00h,10h,00h,00h,00h,18h,00h
        db      7Eh,00h,04h,00h,18h,00h,00h,00h,08h,00h,20h,00h,00h,00h,38h,00h
        db      00h,00h,0Eh,00h,0Eh,00h,60h,00h,0C2h,00h,2Ah,29h,0DAh,0FBh,00h,00h
        db      0F5h,00h,00h,00h,0C0h,00h,0D0h,00h,08h,00h,4Ah,00h,0D0h,0FFh,00h,00h
        db      00h,00h,0BAh,00h,2Ah,29h,1Ch,0E0h,00h,00h,0F5h,00h,00h,00h,0C0h,00h
        db      0D0h,00h,08h,00h,4Ah,00h,0D0h,0FFh,00h,00h,00h,00h,0BAh,00h,2Ah,29h
        db      28h,0E1h,00h,00h,40h,00h,00h,00h,20h,00h,20h,00h,10h,00h,68h,00h
        db      30h,00h,01h,80h,00h,00h,7Ah,00h,0F0h,0FFh,10h,00h,00h,00h,00h,00h
        db      86h,00h,00h,00h,40h,00h,08h,00h,88h,00h,08h,00h,40h,00h,10h,00h
        db      88h,00h,00h,00h,40h,00h,18h,00h,0BCh,00h,20h,29h,00h,00h,0A0h,00h
        db      06h,00h,03h,02h,01h,00h,00h,00h,00h,02h,00h,00h,00h,01h,00h,01h
        db      10h,00h,68h,00h,44h,00h,01h,80h,00h,00h,7Ah,00h,90h,0FFh,10h,00h
        db      00h,00h,00h,00h,86h,00h,00h,00h,00h,01h,08h,00h,88h,00h,08h,00h
        db      00h,01h,10h,00h,88h,00h,00h,00h,00h,01h,18h,00h,0BCh,00h,20h,29h
        db      00h,00h,0CEh,00h,04h,00h,18h,00h,00h,00h,00h,00h,10h,00h,0FFh,0FFh
        db      00h,00h,08h,00h,0FFh,0FFh,0FFh,0FFh,00h,00h,00h,00h,0FFh,0FFh
        db      3C0Ah dup (0)
__dblen label byte
        db      54h,87h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
_bmhgtoff label byte
        db      80h,0B0h
_bmsegoff label byte
        db      82h,0B0h,0B6h,0B0h
FD58    ends

SEG004_TEXT segment para public 'code'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _seg004_2173:near
        extrn   _seg004_2181:near
        extrn   _seg004_21CE:near
        extrn   _seg004_21DA:near
        extrn   _flat_rod:near
        extrn   _flat_x_rel:near
        extrn   _flat_xy_rel:near
        extrn   _flat_xz_rel:near
        extrn   _flat_y_rel:near
        extrn   _flat_yz_rel:near
        extrn   _seg004_2023:near
        extrn   _seg004_20CC:near
        extrn   _seg004_223B:near
        extrn   _seg004_227C:near
        extrn   _seg004_22BD:near
        extrn   _seg004_2322:near
        extrn   _seg004_2348:near
        extrn   _seg004_2383:near
        extrn   _seg004_23BD:near
        extrn   _seg004_241B:near
        extrn   _seg004_2479:near
        extrn   _seg004_24D7:near
        extrn   _seg004_2527:near
        extrn   _seg004_256C:near
        extrn   _create_matrix:near
        extrn   _do_rod:near
        extrn   _do_x_rel:near
        extrn   _do_xy_rel:near
        extrn   _do_xz_rel:near
        extrn   _do_y_rel:near
        extrn   _do_yz_rel:near
        extrn   _do_z_rel:near
        extrn   _flat_z_rel:near
        extrn   _seg004_17AB:near
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
        public  _seg004_5A83
        public  _mm9x9
        public  _mxmul
        public  _overflow_handler_mem
        public  _overflow_handler_reg
        public  _overflow_handler_special_bp
        public  _render_3d
        public  _scale_matrix
        public  _seg004_4980
        public  _seg004_5040
        public  _seg004_51A3
        public  _seg004_520F
        public  _seg004_52FF
        public  _seg004_5374
        public  _seg004_5821
        public  _seg004_5834
        public  _seg004_5937
        public  _mm3x9t
        public  _self_modify
        public  _set_accept
        public  overflow_count

; seg004_4980  (+4980)
; seg004_4980: an opcode handler that breaks into the debugger (int 3) and goes on to the
; next opcode; probably the table's entry for unused opcodes.
_seg004_4980 proc near
        int     3
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_seg004_4980 endp

; seg004_4987  (+4987)
; render_3d (FM name): draw the frame whose render database starts at DbEntry (161Ch). Called
; by cRender (src/sys/C3DENTRY.ASM) with SS on cRender's private stack in seg063, where it
; reads the view window: the copy of the clip window that set_the_window (VIDMODE.ASM) keeps
; at seg063:0AA0 (left, top, right, bottom). Works out scrw, scrh, biasx and biasy from it and
; patches biasx and biasy into the projection code; counts the frame (15FAh); builds the view
; matrix (create_matrix in CAMERA.ASM, then seg004_51A3, scale_matrix, check_flat).
; It then switches to a stack in seg048 (SP 5546h) with seg048:2CECh set to 0D2Dh, and calls
; seg019_C10, which only returns (SYSLIBP.ASM). Finally it restores the stack and runs the
; database: the first opcode at DbEntry is called through the opcode table, and the chain of
; handlers returns here at the end of the frame (do_eof). Out: DS = ES = SS; seg048:2CECh
; restored. (UW2's comment, with UW1's addresses.)
; UW1 only: on entry and again at the end, when any of the 255 bytes from seg048:0008 is
; nonzero, it copies seg048:658h (first set to 50h, at the end to 0C0h, when it is 0) to
; seg048:410Fh; it sets the words 2860h..286Ah and byte 286Ch first; after create_matrix it
; walks the records from 161Eh, calling seg004_17AB (SPHERE.ASM) for each up to the one at
; 15Ch; after the frame it raises int 2 when 2860h is above 68h, calls seg004_520F unless
; 286Ch is set, and calls the handler at es:[2732h + type] for each word from 2864h to 2868h
; (in seg053).
_render_3d proc far
        push    es
        push    di
        push    ax
        mov     ax,seg seg048
        mov     es,ax
        mov     ax,0
        mov     cx,0FFh
        mov     di,8
        repe scasb
        jcxz    L49AE
        mov     al,byte ptr es:[658h]
        test    al,0FFh
        jne     short L49AA
        mov     al,50h
        mov     byte ptr es:[658h],al
L49AA:
        mov     byte ptr es:[410Fh],al
L49AE:
        pop     ax
        pop     di
        pop     es

        mov     ax,seg seg051
        mov     ds,ax
        mov     es,ax
        mov     word ptr ds:[2860h],0Eh
        mov     word ptr ds:[2864h],0
        mov     word ptr ds:[2866h],6
        mov     word ptr ds:[2868h],0
        mov     word ptr ds:[286Ah],0Ch
        mov     byte ptr ds:[286Ch],0FFh
        mov     ax,word ptr ss:[0AA4h]
        sub     ax,word ptr ss:[0AA0h]
        inc     ax
        shr     ax,1
        mov     word ptr ds:[PROJ_X],ax
        mov     ax,word ptr ss:[0AA2h]
        sub     ax,word ptr ss:[0AA6h]
        inc     ax
        shr     ax,1
        mov     word ptr ds:[PROJ_Y],ax
        mov     ax,word ptr ss:[0AA4h]
        add     ax,word ptr ss:[0AA0h]
        inc     ax
        shr     ax,1
        mov     word ptr ds:[CENTRE_X],ax
        mov     word ptr cs:[2A5h],ax
        mov     word ptr cs:[313h],ax
        mov     word ptr cs:[modify_biasx+1],ax
        mov     word ptr cs:[modify_bx2+1],ax
        mov     word ptr cs:[_seg004_21CE+1],ax
        mov     word ptr cs:[_seg004_2173+1],ax
        mov     ax,word ptr ss:[0AA2h]
        add     ax,word ptr ss:[0AA6h]
        shr     ax,1
        mov     word ptr ds:[CENTRE_Y],ax
        mov     word ptr cs:[2B0h],ax
        mov     word ptr cs:[323h],ax
        mov     word ptr cs:[modify_biasy+1],ax
        mov     word ptr cs:[modify_by2+1],ax
        mov     word ptr cs:[_seg004_21DA+1],ax
        mov     word ptr cs:[_seg004_2181+1],ax
        inc     word ptr ds:[15FAh]
        mov     byte ptr ds:[OBJ_SHIFT],0
        call    _create_matrix
        call    _seg004_51A3
        call    _scale_matrix
        call    _check_flat
        mov     bp,word ptr ds:[161Eh]
L4A5F:
        test    word ptr [bp],0FFFFh
        je      short L4A7B
        js      short L4A83
        cmp     bp,word ptr ds:[15Ch]
        je      short L4A7B
        add     bp,6
        push    bp
        call    _seg004_17AB
        pop     bp
        add     bp,word ptr [bp-4]
        jmp     L4A5F
L4A7B:
        add     bp,word ptr [bp+2]
        add     bp,6
        jmp     L4A5F
L4A83:
        mov     ax,ss
        mov     word ptr ds:[15F2h],ax
        mov     word ptr ds:[15F0h],sp
        mov     si,seg seg048
        mov     ds,si
        mov     ax,word ptr ds:[2CECh]
        mov     word ptr cs:L4B42,ax
        mov     word ptr ds:[2CECh],0D2Dh
        mov     ax,seg seg063
        mov     es,ax
        mov     es,si
        cli
        mov     ss,si
        mov     sp,5546h
        sti
        call    far ptr _seg019_C10
        mov     bp,seg seg051
        mov     ds,bp
        mov     es,bp
        cli
        mov     ss,word ptr ds:[15F2h]
        mov     sp,word ptr ds:[15F0h]
        sti
        mov     si,word ptr ds:[161Ch]
        lodsw
        xchg    bx,ax
        call    word ptr [bx+OPCODE_TABLE]
        mov     ax,word ptr ds:[2860h]
        cmp     ax,68h
        jle     short L4AD7
        int     2
        nop
L4AD7:
        push    ds
        push    si
        test    byte ptr ds:[286Ch],0FFh
        jne     short L4AE3
        call    _seg004_520F
L4AE3:
        mov     si,word ptr ds:[2864h]
        mov     cx,word ptr ds:[2868h]
        sub     cx,si
        shr     cx,1
        je      short L4B03
        mov     ax,seg seg053
        mov     ds,ax
L4AF6:
        lodsw
        mov     di,ax
        mov     bx,word ptr [di+4]
        call    word ptr es:[bx+2732h]
        loop    L4AF6
L4B03:
        pop     si
        pop     ds
        push    es
        push    di
        push    ax
        mov     ax,seg seg048
        mov     es,ax
        mov     ax,0
        mov     cx,0FFh
        mov     di,8
        repe scasb
        jcxz    L4B2C
        mov     al,byte ptr es:[658h]
        test    al,0FFh
        jne     short L4B28
        mov     al,0C0h
        mov     byte ptr es:[658h],al
L4B28:
        mov     byte ptr es:[410Fh],al
L4B2C:
        pop     ax
        pop     di
        pop     es
        mov     ax,seg seg048
        mov     ds,ax
        mov     ax,word ptr cs:L4B42
        mov     word ptr ds:[2CECh],ax
        mov     ax,ss
        mov     ds,ax
        mov     es,ax
        retf
_render_3d endp

L4B42   dw      0                       ; render_3d's copy of seg048:2CECh

; seg004_4B44  (+4B44)
; scale_matrix: fold the screen's shape and the zoom into the view matrix. Scales matrix entries
; by the ratio of the view's half width to half height (with 15F4, whose sign picks which axis
; is scaled; probably so x and y project to the square frustum CLIP.ASM's 45-degree planes
; assume) and by the zoom word 2726 (the depth column when it is positive, the other entries
; when negative), and works out 26CC/26CE, a unit horizontal direction on the screen, and from
; it 26D0/26D2, the axes do_scalebm (TMAPOPS.ASM) uses to place sprites facing the viewer.
; Installs _overflow_handler_reg for the divides: cRender points int 0 at seg063:05A0, which holds
; `jmp far cs:[4D5]`, and the far pointer at 4D5 (FM overflow_ptr) is into seg004, so writing
; its offset word selects the handler. Uses SquareRoot_seg019_A30 (IMATH.ASM).
_scale_matrix proc near
        mov     word ptr ds:[2728h],7FFFh
        mov     word ptr ds:[272Ah],7FFFh
        mov     ax,word ptr ds:[MAT_YX]
        mov     word ptr ds:[1614h],ax
        mov     ax,word ptr ds:[MAT_YY]
        mov     word ptr ds:[1616h],ax
        mov     ax,word ptr ds:[MAT_YZ]
        mov     word ptr ds:[1618h],ax
        mov     bx,word ptr ds:[PROJ_Y]
        mov     cx,word ptr ds:[PROJ_X]
        mov     ax,word ptr ds:[15F4h]
        or      ax,ax
        js      short L4B7B
        imul    cx
        shl     ax,1
        rcl     dx,1
        xor     cx,cx
        jmp     short L4B8B
L4B7B:
        neg     ax
        imul    bx
        shl     ax,1
        rcl     dx,1
        mov     bx,dx
        mov     dx,cx
        mov     cx,ax
        xor     ax,ax
L4B8B:
        cmp     dx,bx
        jne     short L4B92
        jmp     L4C33
L4B92:
        jl      short L4BE6
        xchg    dx,bx
        xchg    cx,ax
        shr     dx,1
        rcr     ax,1
        idiv    bx
        mov     cx,ax
        mov     word ptr ds:[2728h],cx
        mov     ax,word ptr ds:[MAT_XX]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[MAT_XX],dx
        mov     ax,word ptr ds:[MAT_YX]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[MAT_YX],dx
        mov     ax,word ptr ds:[MAT_ZX]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[MAT_ZX],dx
        mov     ax,word ptr ds:[1616h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1616h],dx
        mov     ax,word ptr ds:[1618h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1618h],dx
        jmp     short L4C33
L4BE6:
        shr     dx,1
        rcr     ax,1
        idiv    bx
        mov     cx,ax
        mov     word ptr ds:[272Ah],cx
        mov     ax,word ptr ds:[MAT_XY]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[MAT_XY],dx
        mov     ax,word ptr ds:[MAT_YY]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[MAT_YY],dx
        mov     ax,word ptr ds:[MAT_ZY]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[MAT_ZY],dx
        mov     ax,word ptr ds:[1614h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1614h],dx
        mov     ax,word ptr ds:[1618h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1618h],dx
L4C33:
        mov     cx,word ptr ds:[2726h]
        or      cx,cx
        jns     short L4C3E
        jmp     L4CC6
L4C3E:
        mov     word ptr ds:[2730h],cx
        mov     ax,word ptr ds:[MAT_XZ]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[MAT_XZ],dx
        mov     ax,word ptr ds:[MAT_YZ]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[MAT_YZ],dx
        mov     ax,word ptr ds:[MAT_ZZ]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[MAT_ZZ],dx
        mov     ax,word ptr ds:[1616h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1616h],dx
        mov     ax,word ptr ds:[1614h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1614h],dx
        mov     ax,cx
        imul    ax
        mov     bp,ax
        mov     si,dx
        mov     ax,word ptr ds:[2728h]
        mov     word ptr ds:[272Ch],ax
        imul    ax
        add     ax,bp
        adc     dx,si
        mov     bx,dx
        xor     cx,cx
        call    far ptr _SquareRoot_seg019_A30
        mov     ax,di
        xchg    ah,al
        mov     word ptr ds:[2728h],ax
        mov     ax,word ptr ds:[272Ah]
        mov     word ptr ds:[272Eh],ax
        imul    ax
        add     ax,bp
        adc     dx,si
        mov     bx,dx
        xor     cx,cx
        call    far ptr _SquareRoot_seg019_A30
        mov     ax,di
        xchg    al,ah
        mov     word ptr ds:[272Ah],ax
        jmp     L4D85
L4CC6:
        mov     word ptr ds:[2730h],7FFFh
        neg     cx
        mov     ax,word ptr ds:[MAT_XX]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[MAT_XX],dx
        mov     ax,word ptr ds:[MAT_YX]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[MAT_YX],dx
        mov     ax,word ptr ds:[MAT_ZX]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[MAT_ZX],dx
        mov     ax,word ptr ds:[MAT_XY]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[MAT_XY],dx
        mov     ax,word ptr ds:[MAT_YY]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[MAT_YY],dx
        mov     ax,word ptr ds:[MAT_ZY]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[MAT_ZY],dx
        mov     ax,word ptr ds:[1618h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[1618h],dx
        mov     si,cx
        mov     ax,word ptr ds:[2728h]
        imul    cx
        shl     ax,1
        rcl     dx,1
        mov     bp,dx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[272Ch],dx
        mov     dx,bp
        mov     ax,dx
        imul    ax
        mov     bx,dx
        add     bx,3FFFh
        xor     cx,cx
        call    far ptr _SquareRoot_seg019_A30
        mov     ax,di
        xchg    ah,al
        mov     word ptr ds:[2728h],ax
        mov     ax,word ptr ds:[272Ah]
        imul    si
        shl     ax,1
        rcl     dx,1
        mov     bp,dx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[272Eh],dx
        mov     dx,bp
        mov     ax,dx
        imul    ax
        mov     bx,dx
        add     bx,3FFFh
        xor     cx,cx
        call    far ptr _SquareRoot_seg019_A30
        mov     ax,di
        xchg    al,ah
        mov     word ptr ds:[272Ah],ax
L4D85:
        mov     word ptr ss:[4D5h],offset _overflow_handler_reg
        mov     ax,word ptr ds:[MAT_XZ]
        imul    word ptr ds:[MAT_XZ]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[MAT_ZZ]
        imul    word ptr ds:[MAT_ZZ]
        add     bx,ax
        adc     cx,dx
        or      ch,ch
        je      short L4DD7
        call    far ptr _SquareRoot_seg019_A30
        mov     dx,word ptr ds:[MAT_XZ]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L4DBD
        inc     ax
L4DBD:
        mov     word ptr ds:[26CCh],ax
        mov     dx,word ptr ds:[MAT_ZZ]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L4DD2
        inc     ax
L4DD2:
        mov     word ptr ds:[26CEh],ax
        jmp     short L4E2B
L4DD7:
        mov     ax,word ptr ds:[MAT_XY]
        imul    word ptr ds:[MAT_XY]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[MAT_ZY]
        imul    word ptr ds:[MAT_ZY]
        add     bx,ax
        adc     cx,dx
        call    far ptr _SquareRoot_seg019_A30
        mov     dx,word ptr ds:[MAT_XY]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L4E04
        inc     ax
L4E04:
        mov     bx,ax
        mov     dx,word ptr ds:[MAT_ZY]
        xor     ax,ax
        sar     dx,1
        rcr     ax,1
        idiv    di
        cmp     ax,8000h
        jne     short L4E18
        inc     ax
L4E18:
        test    word ptr ds:[MAT_YZ],0FFFFh
        js      short L4E24
        neg     bx
        neg     ax
L4E24:
        mov     word ptr ds:[26CCh],bx
        mov     word ptr ds:[26CEh],ax
L4E2B:
        mov     ax,word ptr ds:[26CEh]
        imul    word ptr ds:[MAT_XX]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[26CCh]
        neg     ax
        imul    word ptr ds:[MAT_ZX]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        adc     dx,0
        mov     word ptr ds:[26D0h],dx
        mov     ax,word ptr ds:[26CEh]
        imul    word ptr ds:[MAT_XY]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[26CCh]
        neg     ax
        imul    word ptr ds:[MAT_ZY]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        shl     ax,1
        adc     dx,0
        mov     word ptr ds:[26D2h],dx
        ret
_scale_matrix endp

; seg004_4E76  (+4E76)
; check_flat: when the view is level (the matrix's middle row and column are 0 except the middle
; entry, at least 7FFBh), install the flat variants of the relative-move opcode handlers
; (flat_x_rel ... flat_rod, INTERP.ASM) in the opcode table and copy the short mxmul body
; (L517E) over mxmul; otherwise install the general handlers and the full body (L512B). 25D4
; records which set is in, so the copy is done only on a change. A level view is probably the
; common case: the view pitches only while the player looks up or down.
_check_flat proc near
        test    word ptr ds:[MAT_YX],0FFFFh
        jne     short L4EA1
        test    word ptr ds:[MAT_XY],0FFFFh
        jne     short L4EA1
        test    word ptr ds:[MAT_ZY],0FFFFh
        jne     short L4EA1
        test    word ptr ds:[MAT_YZ],0FFFFh
        jne     short L4EA1
        cmp     word ptr ds:[MAT_YY],7FFBh
        jl      short L4EA1
        jmp     L4F72
L4EA1:
        test    byte ptr ds:[286Fh],0FFh
        jne     short L4EAB
        jmp     L4F71
L4EAB:
        mov     byte ptr ds:[286Fh],0
        mov     word ptr ds:[2824h],offset _do_y_rel
        mov     word ptr ds:[2822h],offset _do_x_rel
        mov     word ptr ds:[2826h],offset _do_z_rel
        mov     word ptr ds:[2848h],offset _seg004_223B
        mov     word ptr ds:[284Ah],offset _seg004_227C
        mov     word ptr ds:[284Ch],offset _seg004_22BD
        mov     word ptr ds:[282Ch],offset _do_xy_rel
        mov     word ptr ds:[282Eh],offset _do_xz_rel
        mov     word ptr ds:[2830h],offset _do_yz_rel
        mov     word ptr ds:[2852h],offset _seg004_241B
        mov     word ptr ds:[2856h],offset _seg004_2479
        mov     word ptr ds:[2854h],offset _seg004_23BD
        mov     word ptr ds:[2838h],offset _do_rod
        mov     word ptr ds:[285Eh],offset _seg004_2023
L4F04:
        test    byte ptr ds:[286Eh],0FFh
        je      short L4F37
        mov     word ptr ds:[27BEh],offset _seg004_223B
        mov     word ptr ds:[27C0h],offset _seg004_227C
        mov     word ptr ds:[27C2h],offset _seg004_22BD
        mov     word ptr ds:[27C8h],offset _seg004_241B
        mov     word ptr ds:[27CCh],offset _seg004_2479
        mov     word ptr ds:[27CAh],offset _seg004_23BD
        mov     word ptr ds:[27D4h],offset _seg004_2023
        jmp     short L4F61
L4F37:
        mov     word ptr ds:[27BEh],offset _do_x_rel
        mov     word ptr ds:[27C0h],offset _do_y_rel
        mov     word ptr ds:[27C2h],offset _do_z_rel
        mov     word ptr ds:[27C8h],offset _do_xy_rel
        mov     word ptr ds:[27CCh],offset _do_yz_rel
        mov     word ptr ds:[27CAh],offset _do_xz_rel
        mov     word ptr ds:[27D4h],offset _do_rod
L4F61:
        mov     si,offset L512B
        mov     di,offset _mxmul
        push    es
        push    cs
        pop     es
        mov     cx,54h
        rep movs byte ptr es:[di],byte ptr cs:[si]
        pop     es
L4F71:
        ret
is_flat:
L4F72:
        test    byte ptr ds:[286Fh],0FFh
        jne     L4F71
        mov     byte ptr ds:[286Fh],0FFh
        mov     word ptr ds:[2824h],offset _flat_y_rel
        mov     word ptr ds:[2822h],offset _flat_x_rel
        mov     word ptr ds:[2826h],offset _flat_z_rel
        mov     word ptr ds:[2848h],offset _seg004_2348
        mov     word ptr ds:[284Ah],offset _seg004_2322
        mov     word ptr ds:[284Ch],offset _seg004_2383
        mov     word ptr ds:[282Ch],offset _flat_xy_rel
        mov     word ptr ds:[282Eh],offset _flat_xz_rel
        mov     word ptr ds:[2830h],offset _flat_yz_rel
        mov     word ptr ds:[2852h],offset _seg004_2527
        mov     word ptr ds:[2856h],offset _seg004_256C
        mov     word ptr ds:[2854h],offset _seg004_24D7
        mov     word ptr ds:[2838h],offset _flat_rod
        mov     word ptr ds:[285Eh],offset _seg004_20CC
        test    byte ptr ds:[286Eh],0FFh
        je      short L5005
        mov     word ptr ds:[27BEh],offset _seg004_2348
        mov     word ptr ds:[27C0h],offset _seg004_2322
        mov     word ptr ds:[27C2h],offset _seg004_2383
        mov     word ptr ds:[27C8h],offset _seg004_2527
        mov     word ptr ds:[27CCh],offset _seg004_256C
        mov     word ptr ds:[27CAh],offset _seg004_24D7
        mov     word ptr ds:[27D4h],offset _seg004_20CC
        jmp     short L502F
L5005:
        mov     word ptr ds:[27BEh],offset _flat_x_rel
        mov     word ptr ds:[27C0h],offset _flat_y_rel
        mov     word ptr ds:[27C2h],offset _flat_z_rel
        mov     word ptr ds:[27C8h],offset _flat_xy_rel
        mov     word ptr ds:[27CCh],offset _flat_yz_rel
        mov     word ptr ds:[27CAh],offset _flat_xz_rel
        mov     word ptr ds:[27D4h],offset _flat_rod
L502F:
        mov     si,offset L517E
        mov     di,offset _mxmul
        push    es
        push    cs
        pop     es
        mov     cx,26h
        rep movs byte ptr es:[di],byte ptr cs:[si]
        pop     es
        ret
_check_flat endp

; seg004_5040  (+5040)
; UW1 only: when byte 286Eh is set, clears it and copies the normal handlers for opcodes
; 78h..9Ch (seg051:2814h) into the opcode table (27B0h). The branch that would copy
; NOCLIP.ASM's set (283Ah) is never taken: the xor that clears 286Eh leaves ZF set.
_seg004_5040 proc near
        xor     ax,ax
        xor     al,byte ptr ds:[286Eh]
        je      short L505F
        push    si
        push    di
        push    cx
        mov     di,27B0h
        mov     cx,13h
        xor     byte ptr ds:[286Eh],al
        je      short L5060
        mov     si,283Ah
        rep movsw
        pop     cx
        pop     di
        pop     si
L505F:
        ret
L5060:
        mov     si,2814h
        rep movsw
        pop     cx
        pop     di
        pop     si
_seg004_5040 endp

; seg004_5068  (+5068)
; set_accept: empty here and in FM Towns (accept_flag, which it would set, is never written; see
; CLIP.ASM).
_set_accept proc near
        ret
_set_accept endp

; seg004_5069  (+5069)
; self_modify: patch the eye position in the current object's frame (2886, 2888, 288A) and the
; point shift (2880) into the immediates of mini_xlate_rotate_pnt and load_xlate_rotate_pnt, so
; that turning a model point into view space needs no memory reads for them.
_self_modify proc near
        mov     ax,word ptr ds:[OBJ_X]
        mov     word ptr cs:[modify_mex+1],ax
        mov     word ptr cs:[L509D+1],ax
        mov     ax,word ptr ds:[OBJ_Y]
        mov     word ptr cs:[modify_mey+1],ax
        mov     word ptr cs:[L50B5+1],ax
        mov     ax,word ptr ds:[OBJ_Z]
        mov     word ptr cs:[modify_mez+1],ax
        mov     word ptr cs:[L50A9+1],ax
        mov     al,byte ptr ds:[OBJ_SHIFT]
        mov     byte ptr cs:[modify_mes+1],al
        mov     byte ptr cs:[_mini_xlate_rotate_pnt+1],al
        ret
_self_modify endp

; seg004_5096  (+5096)
; mini_xlate_rotate_pnt: in: SI = three signed bytes, a model point in the order x, z, y, each
; times 32. Subtracts the patched eye, shifts by the patched shift and falls into mxmul. Out:
; BX, CX, BP = the view-space point (see mxmul); SI advanced by 3. load_xlate_rotate_pnt below
; is the same for three words.
_mini_xlate_rotate_pnt proc near
        mov     cl,5                    ; the shift and the three origins are patched by self_modify
        lodsb
        cbw
        shl     ax,5
L509D:
        sub     ax,1234h
        shl     ax,cl
        mov     bx,ax
        lodsb
        cbw
        shl     ax,5
L50A9:
        sub     ax,1234h
        shl     ax,cl
        mov     bp,ax
        lodsb
        cbw
        shl     ax,5
L50B5:
        sub     ax,1234h
        shl     ax,cl
        mov     cx,ax
        jmp     short _mxmul
_mini_xlate_rotate_pnt endp

; seg004_50BE  (+50BE)
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

; seg004_50D8  (+50D8)
; mxmul: rotate a point by the view matrix. In: BX, CX, BP = the point's x, y (vertical) and z.
; Out: BX, CX, BP = view x, y and depth z, each the high word of its 1.15 products (so at half
; scale); DI, DX changed. The general body (copied from L512B) does all nine products; the flat
; body (L517E, for a level view) skips the products with the zero entries and takes y as it is,
; halved.
_mxmul proc near
        mov     ax,bx
        imul    word ptr ds:[MAT_XX]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[MAT_YX]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[MAT_ZX]
        add     di,dx
        mov     word ptr ds:[2708h],di
        mov     ax,bx
        imul    word ptr ds:[MAT_XY]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[MAT_YY]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[MAT_ZY]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[MAT_ZZ]
        mov     bp,dx
        mov     ax,bx
        imul    word ptr ds:[MAT_XZ]
        add     bp,dx
        mov     ax,cx
        imul    word ptr ds:[MAT_YZ]
        add     bp,dx
        mov     cx,di
        mov     bx,word ptr ds:[2708h]
        ret
; L512B and L517E: the two bodies check_flat and is_flat copy over the one above
L512B:
        mov     ax,bx
        imul    word ptr ds:[MAT_XX]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[MAT_YX]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[MAT_ZX]
        add     di,dx
        mov     word ptr ds:[2708h],di
        mov     ax,bx
        imul    word ptr ds:[MAT_XY]
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[MAT_YY]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[MAT_ZY]
        add     di,dx
        mov     ax,bp
        imul    word ptr ds:[MAT_ZZ]
        mov     bp,dx
        mov     ax,bx
        imul    word ptr ds:[MAT_XZ]
        add     bp,dx
        mov     ax,cx
        imul    word ptr ds:[MAT_YZ]
        add     bp,dx
        mov     cx,di
        mov     bx,word ptr ds:[2708h]
        ret
L517E:
        mov     ax,bx
        imul    word ptr ds:[MAT_XX]
        mov     di,dx
        mov     ax,bp
        imul    word ptr ds:[MAT_ZX]
        add     di,dx
        mov     ax,bx
        mov     bx,di
        imul    word ptr ds:[MAT_XZ]
        mov     ax,bp
        mov     bp,dx
        imul    word ptr ds:[MAT_ZZ]
        add     bp,dx
        sar     cx,1
        ret
_mxmul endp

; seg004_51A3  (+51A3)
; seg004_51A3: choose the axis for SPHERE.ASM's first cull (modify_axis2): y when the view
; is pitched by more than 45 degrees (|160C| > 5A82h, sin 45 in 1.15), else whichever of x and z
; the view looks along more. Patches the instruction to add or subtract (by the sign) the eye's
; coordinate on that axis (2886 + offset).
_seg004_51A3 proc near
        mov     ax,word ptr ds:[MAT_YZ]
        cwd
        xor     ax,dx
        sub     ax,dx
        cmp     ax,5A82h
        jle     short L51B5
        mov     ax,2
        jmp     short L51D7
L51B5:
        mov     ax,word ptr ds:[MAT_XZ]
        cwd
        xor     ax,dx
        sub     ax,dx
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[MAT_ZZ]
        cwd
        xor     ax,dx
        sub     ax,dx
        cmp     bx,ax
        jg      short L51D2
        mov     ax,4
        jmp     short L51D7
L51D2:
        mov     ax,0
        mov     dx,cx
L51D7:
        mov     bl,2Bh
        test    dx,0FFFFh
        jns     short L51E1
        mov     bl,3
L51E1:
        mov     byte ptr cs:[1876h],bl
        mov     byte ptr cs:[1681h],bl
        mov     byte ptr cs:[1CD4h],bl
        mov     byte ptr cs:[1B6Ch],bl
        add     ax,1C8h
        mov     word ptr cs:[1878h],ax
        mov     word ptr cs:[1683h],ax
        sub     ax,1C8h
        add     ax,OBJ_X
        mov     word ptr cs:[modify_axis2+2],ax
        mov     word ptr cs:[1B6Eh],ax
        ret
_seg004_51A3 endp

; seg004_520F  (+520F)
; UW1 only: sorts the words from 2864h to 2868h (offsets of records in seg053) by the records'
; words +2 and +0 (an insertion sort), and sets byte 286Ch. render_3d calls it after the
; frame unless 286Ch is already set.
_seg004_520F proc near
        mov     byte ptr ds:[286Ch],0FFh
        mov     cx,word ptr ds:[2868h]
        mov     si,word ptr ds:[2864h]
        mov     dx,si
        add     si,2
        cmp     cx,si
        jle     short L5258
        push    ds
        push    es
        mov     ax,seg seg053
        mov     ds,ax
        mov     es,ax
L522E:
        mov     di,si
        lodsw
        xchg    bx,ax
L5232:
        cmp     di,dx
        jle     short L5250
        mov     bp,word ptr [di-2]
        mov     ax,word ptr [bx+2]
        cmp     ax,word ptr ds:[bp+2]
        jl      short L5250
        mov     ax,word ptr [bx]
        cmp     ax,word ptr ds:[bp]
        jle     short L5250
        mov     word ptr [di],bp
        dec     di
        dec     di
        jmp     L5232
L5250:
        mov     word ptr [di],bx
        cmp     cx,si
        jne     L522E
        pop     es
        pop     ds
L5258:
        ret
        mov     cx,word ptr ds:[286Ah]
        mov     si,0Ch
        sub     cx,si
        shr     cx,1
        dec     cx
        jle     short L5298
        mov     dx,si
        add     si,2
        push    ds
        push    es
        mov     ax,seg seg053
        mov     ds,ax
        mov     es,ax
L5275:
        mov     di,si
        sub     di,2
        lodsw
        mov     bx,ax
        mov     ax,word ptr [bx]
L527F:
        mov     bp,word ptr [di]
        cmp     ax,word ptr ds:[bp]
        jl      short L5291
        mov     word ptr [di+2],bp
        sub     di,2
        cmp     di,dx
        jne     L527F
L5291:
        mov     word ptr [di+2],bx
        loop    L5275
        pop     es
        pop     ds
L5298:
        ret
        test    byte ptr ds:[286Ch],0FFh
        jne     short L52A3
        call    _seg004_520F
L52A3:
        mov     di,word ptr ds:[2866h]
        mov     si,word ptr ds:[2864h]
        mov     bp,0Ch
        mov     dx,word ptr ds:[2868h]
        mov     cx,word ptr ds:[286Ah]
        mov     word ptr ds:[2866h],si
        mov     word ptr ds:[2864h],di
        push    ds
        push    es
        mov     ax,seg seg053
        mov     es,ax
        mov     ds,ax
L52C7:
        mov     bx,word ptr [si]
        mov     ax,word ptr [bx]
        mov     bx,word ptr ds:[bp]
        cmp     ax,word ptr [bx]
        jl      short L52E7
        movsw
        cmp     si,dx
        jne     L52C7
        mov     si,bp
        sub     cx,si
        shr     cx,1
        rep movsw
        pop     es
        pop     ds
        mov     word ptr ds:[2868h],di
        ret
L52E7:
        mov     ax,bx
        stosw
        inc     bp
        inc     bp
        cmp     bp,cx
        jne     L52C7
        mov     cx,dx
        sub     cx,si
        shr     cx,1
        rep movsw
        pop     es
        pop     ds
        mov     word ptr ds:[2868h],di
        ret
_seg004_520F endp

; seg004_52FF  (+52FF)
; UW1 only: the vector BX, CX, BP times the view matrix, in 32 bits, at 26D4h..26DEh (for
; INTERP.ASM's seg004_3DC6).
_seg004_52FF proc near
        push    si
        mov     ax,bx
        imul    word ptr ds:[MAT_XX]
        mov     si,ax
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[MAT_YX]
        add     si,ax
        adc     di,dx
        mov     ax,bp
        imul    word ptr ds:[MAT_ZX]
        add     si,ax
        adc     di,dx
        mov     word ptr ds:[26D4h],si
        mov     word ptr ds:[26D6h],di
        mov     ax,bx
        imul    word ptr ds:[MAT_XY]
        mov     si,ax
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[MAT_YY]
        add     si,ax
        adc     di,dx
        mov     ax,bp
        imul    word ptr ds:[MAT_ZY]
        add     si,ax
        adc     di,dx
        mov     word ptr ds:[26D8h],si
        mov     word ptr ds:[26DAh],di
        mov     ax,bx
        imul    word ptr ds:[MAT_XZ]
        mov     si,ax
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[MAT_YZ]
        add     si,ax
        adc     di,dx
        mov     ax,bp
        imul    word ptr ds:[MAT_ZZ]
        add     si,ax
        adc     di,dx
        mov     word ptr ds:[26DCh],si
        mov     word ptr ds:[26DEh],di
        pop     si
        ret
_seg004_52FF endp

; seg004_5374  (+5374)
; UW1 only: as seg004_52FF, the results at 26E0h..26EAh.
_seg004_5374 proc near
        push    si
        mov     ax,bx
        imul    word ptr ds:[MAT_XX]
        mov     si,ax
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[MAT_YX]
        add     si,ax
        adc     di,dx
        mov     ax,bp
        imul    word ptr ds:[MAT_ZX]
        add     si,ax
        adc     di,dx
        mov     word ptr ds:[26E0h],si
        mov     word ptr ds:[26E2h],di
        mov     ax,bx
        imul    word ptr ds:[MAT_XY]
        mov     si,ax
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[MAT_YY]
        add     si,ax
        adc     di,dx
        mov     ax,bp
        imul    word ptr ds:[MAT_ZY]
        add     si,ax
        adc     di,dx
        mov     word ptr ds:[26E4h],si
        mov     word ptr ds:[26E6h],di
        mov     ax,bx
        imul    word ptr ds:[MAT_XZ]
        mov     si,ax
        mov     di,dx
        mov     ax,cx
        imul    word ptr ds:[MAT_YZ]
        add     si,ax
        adc     di,dx
        mov     ax,bp
        imul    word ptr ds:[MAT_ZZ]
        add     si,ax
        adc     di,dx
        mov     word ptr ds:[26E8h],si
        mov     word ptr ds:[26EAh],di
        pop     si
        ret
_seg004_5374 endp

; seg004_53E9  (+53E9)
; instance_vvars_head: rotate the eye position in the object's frame (2886 x, 288A z) by the
; instance heading in 270A..270E, so later points can be taken relative to the rotated object.
_instance_vvars_head proc near
        mov     ax,word ptr ds:[OBJ_X]
        imul    word ptr ds:[270Ah]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[OBJ_Z]
        imul    word ptr ds:[270Eh]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        mov     ax,word ptr ds:[OBJ_X]
        mov     word ptr ds:[OBJ_X],cx
        imul    word ptr ds:[270Ch]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[OBJ_Z]
        imul    word ptr ds:[270Ah]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[OBJ_Z],dx
        ret
_instance_vvars_head endp

; seg004_5426  (+5426)
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
        mov     word ptr ds:[270Ch],ax
        mov     word ptr ds:[270Ah],bx
        neg     ax
        mov     word ptr ds:[270Eh],ax
        call    _instance_vvars_head
        mov     ax,word ptr ds:[270Ah]
        imul    word ptr ds:[MAT_XX]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[270Eh]
        imul    word ptr ds:[MAT_ZX]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L5468
        jg      short L5465
        mov     cx,8000h
        jmp     short L5468
L5465:
        mov     cx,7FFFh
L5468:
        mov     word ptr ds:[26D4h],cx
        mov     ax,word ptr ds:[270Ah]
        imul    word ptr ds:[MAT_XY]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[270Eh]
        imul    word ptr ds:[MAT_ZY]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L5492
        jg      short L548F
        mov     cx,8000h
        jmp     short L5492
L548F:
        mov     cx,7FFFh
L5492:
        mov     word ptr ds:[26D6h],cx
        mov     ax,word ptr ds:[270Ah]
        imul    word ptr ds:[MAT_XZ]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[270Eh]
        imul    word ptr ds:[MAT_ZZ]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L54BC
        jg      short L54B9
        mov     cx,8000h
        jmp     short L54BC
L54B9:
        mov     cx,7FFFh
L54BC:
        mov     word ptr ds:[26D8h],cx
        mov     ax,word ptr ds:[270Ch]
        imul    word ptr ds:[MAT_XX]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[270Ah]
        imul    word ptr ds:[MAT_ZX]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L54E6
        jg      short L54E3
        mov     cx,8000h
        jmp     short L54E6
L54E3:
        mov     cx,7FFFh
L54E6:
        mov     word ptr ds:[MAT_ZX],cx
        mov     ax,word ptr ds:[270Ch]
        imul    word ptr ds:[MAT_XY]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[270Ah]
        imul    word ptr ds:[MAT_ZY]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L5510
        jg      short L550D
        mov     cx,8000h
        jmp     short L5510
L550D:
        mov     cx,7FFFh
L5510:
        mov     word ptr ds:[MAT_ZY],cx
        mov     ax,word ptr ds:[270Ch]
        imul    word ptr ds:[MAT_XZ]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[270Ah]
        imul    word ptr ds:[MAT_ZZ]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L553A
        jg      short L5537
        mov     cx,8000h
        jmp     short L553A
L5537:
        mov     cx,7FFFh
L553A:
        mov     word ptr ds:[MAT_ZZ],cx
        mov     ax,word ptr ds:[26D4h]
        mov     word ptr ds:[MAT_XX],ax
        mov     ax,word ptr ds:[26D6h]
        mov     word ptr ds:[MAT_XY],ax
        mov     ax,word ptr ds:[26D8h]
        mov     word ptr ds:[MAT_XZ],ax
        ret
_instance_head endp

; seg004_5551  (+5551)
; instance_vvars_pitch and instance_pitch, instance_vvars_bank and instance_bank: as
; instance_vvars_head and instance_head for rotations about the other two axes (pitch and bank).
_instance_vvars_pitch proc near
        mov     ax,word ptr ds:[OBJ_Y]
        imul    word ptr ds:[2716h]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[OBJ_Z]
        imul    word ptr ds:[271Ah]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        mov     ax,word ptr ds:[OBJ_Y]
        mov     word ptr ds:[OBJ_Y],dx
        imul    word ptr ds:[2718h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[OBJ_Z]
        imul    word ptr ds:[2716h]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        mov     word ptr ds:[OBJ_Z],dx
        ret
_instance_vvars_pitch endp

; seg004_558E  (+558E)
_instance_pitch proc near
        mov     ax,ss
        mov     ds,ax
        call    far ptr _sincos
        mov     dx,es
        mov     ds,dx
        mov     word ptr ds:[2718h],ax
        mov     word ptr ds:[2716h],bx
        neg     ax
        mov     word ptr ds:[271Ah],ax
        call    _instance_vvars_pitch
        mov     ax,word ptr ds:[2716h]
        imul    word ptr ds:[MAT_YX]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[271Ah]
        imul    word ptr ds:[MAT_ZX]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L55D0
        jg      short L55CD
        mov     cx,8000h
        jmp     short L55D0
L55CD:
        mov     cx,7FFFh
L55D0:
        mov     word ptr ds:[26D4h],cx
        mov     ax,word ptr ds:[2716h]
        imul    word ptr ds:[MAT_YY]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[271Ah]
        imul    word ptr ds:[MAT_ZY]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L55FA
        jg      short L55F7
        mov     cx,8000h
        jmp     short L55FA
L55F7:
        mov     cx,7FFFh
L55FA:
        mov     word ptr ds:[26D6h],cx
        mov     ax,word ptr ds:[2716h]
        imul    word ptr ds:[MAT_YZ]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[271Ah]
        imul    word ptr ds:[MAT_ZZ]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L5624
        jg      short L5621
        mov     cx,8000h
        jmp     short L5624
L5621:
        mov     cx,7FFFh
L5624:
        mov     word ptr ds:[26D8h],cx
        mov     ax,word ptr ds:[2718h]
        imul    word ptr ds:[MAT_YX]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2716h]
        imul    word ptr ds:[MAT_ZX]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L564E
        jg      short L564B
        mov     cx,8000h
        jmp     short L564E
L564B:
        mov     cx,7FFFh
L564E:
        mov     word ptr ds:[MAT_ZX],cx
        mov     ax,word ptr ds:[2718h]
        imul    word ptr ds:[MAT_YY]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2716h]
        imul    word ptr ds:[MAT_ZY]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L5678
        jg      short L5675
        mov     cx,8000h
        jmp     short L5678
L5675:
        mov     cx,7FFFh
L5678:
        mov     word ptr ds:[MAT_ZY],cx
        mov     ax,word ptr ds:[2718h]
        imul    word ptr ds:[MAT_YZ]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2716h]
        imul    word ptr ds:[MAT_ZZ]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L56A2
        jg      short L569F
        mov     cx,8000h
        jmp     short L56A2
L569F:
        mov     cx,7FFFh
L56A2:
        mov     word ptr ds:[MAT_ZZ],cx
        mov     ax,word ptr ds:[26D4h]
        mov     word ptr ds:[MAT_YX],ax
        mov     ax,word ptr ds:[26D6h]
        mov     word ptr ds:[MAT_YY],ax
        mov     ax,word ptr ds:[26D8h]
        mov     word ptr ds:[MAT_YZ],ax
        ret
_instance_pitch endp

; seg004_56B9  (+56B9)
_instance_vvars_bank proc near
        mov     ax,word ptr ds:[OBJ_Y]
        imul    word ptr ds:[2710h]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[OBJ_X]
        imul    word ptr ds:[2714h]
        add     ax,bx
        adc     dx,cx
        shl     ax,1
        rcl     dx,1
        mov     ax,word ptr ds:[OBJ_Y]
        mov     word ptr ds:[OBJ_Y],dx
        imul    word ptr ds:[2712h]
        mov     cx,dx
        mov     bx,ax
        mov     ax,word ptr ds:[OBJ_X]
        imul    word ptr ds:[2710h]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        mov     word ptr ds:[OBJ_X],cx
        ret
_instance_vvars_bank endp

; seg004_56F6  (+56F6)
_instance_bank proc near
        mov     ax,ss
        mov     ds,ax
        call    far ptr _sincos
        mov     dx,es
        mov     ds,dx
        mov     word ptr ds:[2712h],ax
        mov     word ptr ds:[2710h],bx
        neg     ax
        mov     word ptr ds:[2714h],ax
        call    _instance_vvars_bank
        mov     ax,word ptr ds:[2710h]
        imul    word ptr ds:[MAT_XX]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2712h]
        imul    word ptr ds:[MAT_YX]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L5738
        jg      short L5735
        mov     cx,8000h
        jmp     short L5738
L5735:
        mov     cx,7FFFh
L5738:
        mov     word ptr ds:[26D4h],cx
        mov     ax,word ptr ds:[2710h]
        imul    word ptr ds:[MAT_XY]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2712h]
        imul    word ptr ds:[MAT_YY]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L5762
        jg      short L575F
        mov     cx,8000h
        jmp     short L5762
L575F:
        mov     cx,7FFFh
L5762:
        mov     word ptr ds:[26D6h],cx
        mov     ax,word ptr ds:[2710h]
        imul    word ptr ds:[MAT_XZ]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2712h]
        imul    word ptr ds:[MAT_YZ]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L578C
        jg      short L5789
        mov     cx,8000h
        jmp     short L578C
L5789:
        mov     cx,7FFFh
L578C:
        mov     word ptr ds:[26D8h],cx
        mov     ax,word ptr ds:[2714h]
        imul    word ptr ds:[MAT_XX]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2710h]
        imul    word ptr ds:[MAT_YX]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L57B6
        jg      short L57B3
        mov     cx,8000h
        jmp     short L57B6
L57B3:
        mov     cx,7FFFh
L57B6:
        mov     word ptr ds:[MAT_YX],cx
        mov     ax,word ptr ds:[2714h]
        imul    word ptr ds:[MAT_XY]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2710h]
        imul    word ptr ds:[MAT_YY]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L57E0
        jg      short L57DD
        mov     cx,8000h
        jmp     short L57E0
L57DD:
        mov     cx,7FFFh
L57E0:
        mov     word ptr ds:[MAT_YY],cx
        mov     ax,word ptr ds:[2714h]
        imul    word ptr ds:[MAT_XZ]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr ds:[2710h]
        imul    word ptr ds:[MAT_YZ]
        add     bx,ax
        adc     cx,dx
        add     bx,bx
        adc     cx,cx
        jno     short L580A
        jg      short L5807
        mov     cx,8000h
        jmp     short L580A
L5807:
        mov     cx,7FFFh
L580A:
        mov     word ptr ds:[MAT_YZ],cx
        mov     ax,word ptr ds:[26D4h]
        mov     word ptr ds:[MAT_XX],ax
        mov     ax,word ptr ds:[26D6h]
        mov     word ptr ds:[MAT_XY],ax
        mov     ax,word ptr ds:[26D8h]
        mov     word ptr ds:[MAT_XZ],ax
        ret
_instance_bank endp

; seg004_5821  (+5821)
; UW1 only: the object origin (2886h, 2888h, 288Ah) times the matrix at BP (seg004_5937),
; stored back.
_seg004_5821 proc near
        mov     si,OBJ_X
        call    _seg004_5937
        mov     word ptr ds:[OBJ_X],bx
        mov     word ptr ds:[OBJ_Y],cx
        mov     word ptr ds:[OBJ_Z],di
        ret
_seg004_5821 endp

; seg004_5834  (+5834)
; UW1 only: seg004_5821, then the view matrix (1602h..1612h) from the matrix at SI (in SS)
; and the one at BP, a column at a time (seg004_5A83). Called by SPHERE.ASM's seg004_17AB.
_seg004_5834 proc near
        push    si
        call    _seg004_5821
        mov     si,bp
        pop     bp
        push    ds
        mov     ax,ss
        mov     ds,ax
        call    _seg004_5A83
        mov     word ptr es:[MAT_XX],bx
        mov     word ptr es:[MAT_XY],cx
        mov     word ptr es:[MAT_XZ],di
        add     si,2
        call    _seg004_5A83
        mov     word ptr es:[MAT_YX],bx
        mov     word ptr es:[MAT_YY],cx
        mov     word ptr es:[MAT_YZ],di
        add     si,2
        call    _seg004_5A83
        mov     word ptr es:[MAT_ZX],bx
        mov     word ptr es:[MAT_ZY],cx
        mov     word ptr es:[MAT_ZZ],di
        pop     ds
        ret
_seg004_5834 endp

; seg004_587E  (+587E)
; code_pnt: the clip codes of a view-space point. In: BX = x, CX = y, BP = z. Out: AL = 0 inside
; the frustum, else bits 1 (x < -z), 2 (x > z), 4 (y > z), 8 (y < -z), and 80h when z <= 0
; (behind the eye, with the side bits set from the signs). The four planes are CLIP.ASM's.
; Changes AX only.
_code_pnt proc near
        mov     ax,bp
        neg     ax
        jge     short L58BF
        cmp     bx,ax
        jl      short L5897
        cmp     bx,bp
        jg      short L58A8
        cmp     cx,bp
        jg      short L58B9
        cmp     cx,ax
        jl      short L58BC
        xor     al,al
        ret
L5897:
        cmp     cx,bp
        jg      short L58A2
        cmp     cx,ax
        jl      short L58A5
        mov     al,1
        ret
L58A2:
        mov     al,5
        ret
L58A5:
        mov     al,9
        ret
L58A8:
        cmp     cx,bp
        jg      short L58B3
        cmp     cx,ax
        jl      short L58B6
        mov     al,2
        ret
L58B3:
        mov     al,6
        ret
L58B6:
        mov     al,0Ah
        ret
L58B9:
        mov     al,4
        ret
L58BC:
        mov     al,8
        ret
L58BF:
        cmp     bx,ax
        jl      short L58CA
        cmp     cx,bp
        jg      short L58F0
        mov     al,8Ah
        ret
L58CA:
        cmp     bx,bp
        jg      short L58D5
        cmp     cx,bp
        jg      short L58E6
        mov     al,89h
        ret
L58D5:
        cmp     cx,bp
        jg      short L58DC
        mov     al,8Bh
        ret
L58DC:
        cmp     cx,ax
        jl      short L58E3
        mov     al,87h
        ret
L58E3:
        mov     al,8Fh
        ret
L58E6:
        cmp     cx,ax
        jl      short L58ED
        mov     al,85h
        ret
L58ED:
        mov     al,8Dh
        ret
L58F0:
        cmp     cx,ax
        jl      short L58F7
        mov     al,86h
        ret
L58F7:
        mov     al,8Eh
        ret
_code_pnt endp

; seg004_58FA  (+58FA)
; mm9x9: the view matrix (written through ES) = the 3 x 3 matrix at BP times the one at SI, row
; by row through mm3x9_bpsi. BP is advanced by 18 bytes.
_mm9x9 proc near
        call    _mm3x9_bpsi
        mov     word ptr es:[MAT_XX],bx
        mov     word ptr es:[MAT_XY],cx
        mov     word ptr es:[MAT_XZ],di
        add     bp,6
        call    _mm3x9_bpsi
        mov     word ptr es:[MAT_YX],bx
        mov     word ptr es:[MAT_YY],cx
        mov     word ptr es:[MAT_YZ],di
        add     bp,6
        call    _mm3x9_bpsi
        mov     word ptr es:[MAT_ZX],bx
        mov     word ptr es:[MAT_ZY],cx
        mov     word ptr es:[MAT_ZZ],di
        ret
_mm9x9 endp

; seg004_5937  (+5937)
; UW1 only: as mm3x9_bpsi with SI and BP the other way round: the row at SI times the columns
; of the matrix at BP (words +0, +6, +0Ch ...), saturated the same way.
_seg004_5937 proc near
        mov     ax,word ptr [si]
        imul    word ptr [bp]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr [si+2]
        imul    word ptr [bp+6]
        add     bx,ax
        adc     cx,dx
        mov     ax,word ptr [si+4]
        imul    word ptr [bp+0Ch]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        jno     short L5964
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5964:
        cmp     cx,8000h
        jne     short L596D
        mov     cx,8001h
L596D:
        push    cx
        mov     ax,word ptr [si]
        imul    word ptr [bp+2]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr [si+2]
        imul    word ptr [bp+8]
        add     bx,ax
        adc     cx,dx
        mov     ax,word ptr [si+4]
        imul    word ptr [bp+0Eh]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        jno     short L599B
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L599B:
        cmp     cx,8000h
        jne     short L59A4
        mov     cx,8001h
L59A4:
        mov     ax,word ptr [si]
        imul    word ptr [bp+4]
        mov     bx,ax
        mov     di,dx
        mov     ax,word ptr [si+2]
        imul    word ptr [bp+0Ah]
        add     bx,ax
        adc     di,dx
        mov     ax,word ptr [si+4]
        imul    word ptr [bp+10h]
        add     bx,ax
        adc     di,dx
        shl     bx,1
        rcl     di,1
        jno     short L59D1
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L59D1:
        cmp     cx,8000h
        jne     short L59DA
        mov     cx,8001h
L59DA:
        pop     bx
        ret
_seg004_5937 endp

; seg004_59DC  (+59DC)
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
        jno     short L5A09
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5A09:
        cmp     cx,8000h
        jne     short L5A12
        mov     cx,8001h
L5A12:
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
        jno     short L5A41
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5A41:
        cmp     cx,8000h
        jne     short L5A4A
        mov     cx,8001h
L5A4A:
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
        jno     short L5A78
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5A78:
        cmp     di,8000h
        jne     short L5A81
        mov     di,8001h
L5A81:
        pop     bx
        ret
_mm3x9_bpsi endp

; seg004_5A83  (+5A83)
; UW1 only: as mm3x9t, but the three products take the column at SI (words +0, +6, +0Ch)
; against the columns of BP's matrix; with the same slip in the third product's overflow
; fix-up. Called three times by seg004_5834.
_seg004_5A83 proc near
        mov     ax,word ptr [si]
        imul    word ptr [bp]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr [si+6]
        imul    word ptr [bp+6]
        add     bx,ax
        adc     cx,dx
        mov     ax,word ptr [si+0Ch]
        imul    word ptr [bp+0Ch]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        jno     short L5AB0
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5AB0:
        cmp     cx,8000h
        jne     short L5AB9
        mov     cx,8001h
L5AB9:
        push    cx
        mov     ax,word ptr [si]
        imul    word ptr [bp+2]
        mov     bx,ax
        mov     cx,dx
        mov     ax,word ptr [si+6]
        imul    word ptr [bp+8]
        add     bx,ax
        adc     cx,dx
        mov     ax,word ptr [si+0Ch]
        imul    word ptr [bp+0Eh]
        add     bx,ax
        adc     cx,dx
        shl     bx,1
        rcl     cx,1
        jno     short L5AE7
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5AE7:
        cmp     cx,8000h
        jne     short L5AF0
        mov     cx,8001h
L5AF0:
        mov     ax,word ptr [si]
        imul    word ptr [bp+4]
        mov     bx,ax
        mov     di,dx
        mov     ax,word ptr [si+6]
        imul    word ptr [bp+0Ah]
        add     bx,ax
        adc     di,dx
        mov     ax,word ptr [si+0Ch]
        imul    word ptr [bp+10h]
        add     bx,ax
        adc     di,dx
        shl     bx,1
        rcl     di,1
        jno     short L5B1D
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5B1D:
        cmp     di,8000h
        jne     short L5B26
        mov     di,8001h
L5B26:
        pop     bx
        ret
_seg004_5A83 endp

; seg004_5B28  (+5B28)
; mm3x9t: as mm3x9_bpsi with the matrix at BP taken transposed (a row at SI times the columns of
; BP's matrix), with the same slip in the third product's overflow fix-up. Called by head_view
; (CAMERA.ASM).
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
        jno     short L5B55
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5B55:
        cmp     cx,8000h
        jne     short L5B5E
        mov     cx,8001h
L5B5E:
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
        jno     short L5B8C
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5B8C:
        cmp     cx,8000h
        jne     short L5B95
        mov     cx,8001h
L5B95:
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
        jno     short L5BC2
        mov     ax,cx
        cwd
        mov     cx,8001h
        xor     cx,dx
        sub     cx,dx
L5BC2:
        cmp     di,8000h
        jne     short L5BCB
        mov     di,8001h
L5BCB:
        pop     bx
        ret
_mm3x9t endp

; The word the divide overflow handlers save BP in, and overflow_count (FM's name), which
; overflow_handler_reg counts in and do_scalebm (TMAPOPS.ASM) clears and tests.
L5BCD           dw      0
overflow_count  dw      0

; seg004_5BD1  (+5BD1)
; overflow_handler_reg: the int 0 handler for a 2-byte `idiv reg`: skips the instruction and
; returns AX = +-7FFFh by the sign of DX (DX the sign extension), and counts the overflow
; (do_scalebm abandons a sprite that overflowed).
_overflow_handler_reg proc far
        mov     word ptr cs:L5BCD,bp
        mov     bp,sp
        inc     word ptr [bp]
        inc     word ptr [bp]
        mov     ax,7FFFh
        xor     dx,dx
        mov     bp,word ptr cs:L5BCD
        inc     word ptr cs:overflow_count
        iret
_overflow_handler_reg endp

; seg004_5BEE  (+5BEE)
; overflow_handler_mem: for a 4-byte divide by a memory operand: skips it, AX = 7FFFh, DX = 0.
_overflow_handler_mem proc far
        mov     word ptr cs:L5BCD,bp
        mov     bp,sp
        add     word ptr [bp],4
        mov     ax,7FFFh
        xor     dx,dx
        mov     bp,word ptr cs:L5BCD
        iret
_overflow_handler_mem endp

; seg004_5C04  (+5C04)
; overflow_handler_special_bp: for a 2-byte divide by BP: skips it and returns +-7FFFh by the
; sign of DX xor BP (the quotient's sign), DX = 0.
_overflow_handler_special_bp proc far
        mov     word ptr cs:L5BCD,bp
        mov     bp,sp
        inc     word ptr [bp]
        inc     word ptr [bp]
        mov     bp,word ptr cs:L5BCD
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
