; /* target: seg004_0849_6D20 */
; /* opts: /ml */
;
; The image and page cache: uncmp_tab, lightabs, do_fetchmap, do_settmobj, do_uwobj,
; do_uwcrit, update_cache, unload_cpg, load_cpg, lcpg_pghere, parse_cpg, do_setbmcol,
; do_mouseq and do_uwcolv (FM Towns names; FM Towns has them in this order in one place too).
; Its _DATA is the far pointers grs_off and obj_tab and their neighbours.
;
; SEG004 split into its original modules: this is module 14 of 14, +6D20..+8154 of the segment
; (file FEE0h), a module of the second library (UWLIB.LIB).
; Procs: do_fetchmap, do_settmobj, do_uwobj, do_uwcrit, seg004_0849_7EDE, seg004_0849_7F0C,
; update_cache, unload_cpg, load_cpg, lcpg_pghere, parse_cpg, do_setbmcol, do_mouseq,
; do_uwcolv.
; Evidence for the bounds: starts on a paragraph after 5 zero bytes following a ret: TLINK's
; padding before a paragraph-aligned module (TASM 2.0 pads `align 16` with 87 DB/90, not
; zeros). It opens with 16 bytes of data that seg003 reads (es:6D20h).
; Its relocations are entries 2457-2472 of UW2.EXE's relocation table.
; Its _DATA is DS:2194..21A4 (relocation entries 2454-2456), defined before the code because
; those entries come before the code's.
;
; The empty SEG021_TEXT declaration at the end is the later paragraph-aligned contribution
; that makes seg021's segment end at 1080h, 15 zero bytes after its last routine (the segment
; table counts them as seg021's). Which module made it is not known; this is the first module
; linked after seg021's last one that refers to seg021.

        .386

        extrn   _CmapCache:far
        extrn   _seg021_22FD_102B:far
        extrn   _seg_5DFD:far
        extrn   seg052_519C:far
        extrn   seg_370D:far

_DATA   segment word public use16 'DATA'
_grs_off label dword
        dd      seg052_519C+0D049h
_obj_tab label dword
        dd      seg052_519C+0D849h
        dd      seg_370D+0B10h
        db      00h,00h,00h,00h
        extrn   _Palettes:byte                  ; OVR119's object palettes, DS:6742
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public use16 'CODE'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _exp_4run:near
        extrn   _exp_4str:near
        extrn   _exp_5run:near
        extrn   _exp_8run:near
        extrn   _exp_8str:near
        extrn   in_scalebm:near
        extrn   setcol_common:near
        public  _grs_off
        public  _obj_tab
        public  _do_fetchmap
        public  _do_mouseq
        public  _do_setbmcol
        public  _do_settmobj
        public  _do_uwcolv
        public  _do_uwcrit
        public  _do_uwobj
        public  _lcpg_pghere
        public  _load_cpg
        public  _parse_cpg
        public  _seg004_0849_6D20
        public  _seg004_0849_7EDE
        public  _seg004_0849_7F0C
        public  _unload_cpg
        public  _update_cache
        public  lightabs

_seg004_0849_6D20   db      10h dup (0)             ; seg003 reads its first byte (es:6D20h)

; uncmp_tab: FM uncmp_tab, the decoder for each image format byte (cFrmtoRaw, do_uwobj and
; parse_cpg call through it with BX = format * 2)
uncmp_tab       dw      offset L6D3C, offset _exp_8run, offset _exp_8str
                dw      offset _exp_5run, offset _exp_4run, offset _exp_4str
L6D3C:                                  ; format 0: not an image
        int     3
        ret

; lightabs: FM lightabs, 16 shading rows of 256 colours each, row = distance
; shade 0 (no change)
lightabs        db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,18h,19h,1Ah,1Bh,1Ch,1Dh,1Eh,1Fh
                db      20h,21h,22h,23h,24h,25h,26h,27h,28h,29h,2Ah,2Bh,2Ch,2Dh,2Eh,2Fh
                db      30h,31h,32h,33h,34h,35h,36h,37h,38h,39h,3Ah,3Bh,3Ch,3Dh,3Eh,3Fh
                db      40h,41h,42h,43h,44h,45h,46h,47h,48h,49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh
                db      50h,51h,52h,53h,54h,55h,56h,57h,58h,59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh
                db      60h,61h,62h,63h,64h,65h,66h,67h,68h,69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh
                db      70h,71h,72h,73h,74h,75h,76h,77h,78h,79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh
                db      80h,81h,82h,83h,84h,85h,86h,87h,88h,89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh
                db      90h,91h,92h,93h,94h,95h,96h,97h,98h,99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh
                db      0A0h,0A1h,0A2h,0A3h,0A4h,0A5h,0A6h,0A7h,0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh
                db      0B0h,0B1h,0B2h,0B3h,0B4h,0B5h,0B6h,0B7h,0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh
                db      0C0h,0C1h,0C2h,0C3h,0C4h,0C5h,0C6h,0C7h,0C8h,0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh
                db      0D0h,0D1h,0D2h,0D3h,0D4h,0D5h,0D6h,0D7h,0D8h,0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh
                db      0E0h,0E1h,0E2h,0E3h,0E4h,0E5h,0E6h,0E7h,0E8h,0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 1
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,19h,1Ah,1Bh,1Ch,1Dh,1Eh,1Fh,1Fh
                db      21h,22h,23h,24h,25h,26h,27h,27h,29h,2Ah,2Bh,2Ch,2Dh,2Eh,2Fh,2Fh
                db      30h,31h,32h,33h,34h,35h,36h,37h,38h,39h,3Ah,3Bh,3Ch,3Dh,3Eh,3Fh
                db      41h,42h,43h,44h,45h,46h,47h,48h,49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh
                db      51h,52h,53h,54h,55h,56h,57h,58h,59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh
                db      61h,62h,63h,64h,65h,66h,67h,68h,69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh
                db      71h,72h,73h,74h,75h,76h,77h,78h,79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh
                db      81h,82h,83h,84h,85h,86h,87h,88h,89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh
                db      91h,92h,93h,94h,95h,96h,97h,98h,99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh
                db      0A1h,0A2h,0A3h,0A4h,0A5h,0A6h,0A7h,0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh
                db      0B1h,0B2h,0B3h,0B4h,0B5h,0B6h,0B7h,0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh
                db      0C1h,0C2h,0C3h,0C4h,0C5h,0C6h,0C7h,0C8h,0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh
                db      0D1h,0D2h,0D3h,0D4h,0D5h,0D6h,0D7h,0D8h,0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh
                db      0E1h,0E2h,0E3h,0E4h,0E5h,0E6h,0E7h,0E8h,0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 2
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Ah,1Bh,1Ch,1Dh,1Eh,1Fh,1Fh,1Fh
                db      22h,23h,24h,25h,26h,27h,27h,27h,2Ah,2Bh,2Ch,2Dh,2Eh,2Fh,2Fh,2Fh
                db      34h,35h,36h,37h,38h,39h,3Ah,3Bh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      42h,43h,44h,45h,46h,47h,48h,49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh
                db      52h,53h,54h,55h,56h,57h,58h,59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh
                db      62h,63h,64h,65h,66h,67h,68h,69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh
                db      72h,73h,74h,75h,76h,77h,78h,79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh
                db      82h,83h,84h,85h,86h,87h,88h,89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh
                db      92h,93h,94h,95h,96h,97h,98h,99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh
                db      0A2h,0A3h,0A4h,0A5h,0A6h,0A7h,0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh
                db      0B2h,0B3h,0B4h,0B5h,0B6h,0B7h,0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh
                db      0C2h,0C3h,0C4h,0C5h,0C6h,0C7h,0C8h,0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh
                db      0D2h,0D3h,0D4h,0D5h,0D6h,0D7h,0D8h,0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh
                db      0E2h,0E3h,0E4h,0E5h,0E6h,0E7h,0E8h,0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 3
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Bh,1Ch,1Dh,1Eh,1Fh,1Fh,1Fh,1Fh
                db      23h,24h,25h,26h,27h,27h,27h,27h,2Bh,2Ch,2Dh,2Eh,2Fh,2Fh,2Fh,2Fh
                db      34h,35h,36h,37h,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      43h,44h,45h,46h,47h,48h,49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh
                db      53h,54h,55h,56h,57h,58h,59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh
                db      63h,64h,65h,66h,67h,68h,69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh
                db      73h,74h,75h,76h,77h,78h,79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh
                db      83h,84h,85h,86h,87h,88h,89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh
                db      93h,94h,95h,96h,97h,98h,99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh
                db      0A3h,0A4h,0A5h,0A6h,0A7h,0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh
                db      0B3h,0B4h,0B5h,0B6h,0B7h,0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh
                db      0C3h,0C4h,0C5h,0C6h,0C7h,0C8h,0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh
                db      0D3h,0D4h,0D5h,0D6h,0D7h,0D8h,0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh
                db      0E3h,0E4h,0E5h,0E6h,0E7h,0E8h,0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 4
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Ch,1Dh,1Eh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      24h,25h,26h,27h,27h,27h,27h,27h,2Ch,2Dh,2Eh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      44h,45h,46h,47h,48h,49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      54h,55h,56h,57h,58h,59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      64h,65h,66h,67h,68h,69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      74h,75h,76h,77h,78h,79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      84h,85h,86h,87h,88h,89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      94h,95h,96h,97h,98h,99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0A4h,0A5h,0A6h,0A7h,0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0B4h,0B5h,0B6h,0B7h,0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0C4h,0C5h,0C6h,0C7h,0C8h,0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0D4h,0D5h,0D6h,0D7h,0D8h,0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0E4h,0E5h,0E6h,0E7h,0E8h,0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 5
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Dh,1Eh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      25h,26h,27h,27h,27h,27h,27h,27h,2Dh,2Eh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      45h,46h,47h,48h,49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      55h,56h,57h,58h,59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      65h,66h,67h,68h,69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      75h,76h,77h,78h,79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      85h,86h,87h,88h,89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      95h,96h,97h,98h,99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0A5h,0A6h,0A7h,0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0B5h,0B6h,0B7h,0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0C5h,0C6h,0C7h,0C8h,0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0D5h,0D6h,0D7h,0D8h,0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0E5h,0E6h,0E7h,0E8h,0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 6
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Eh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      26h,27h,27h,27h,27h,27h,27h,27h,2Eh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      46h,47h,48h,49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      56h,57h,58h,59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      66h,67h,68h,69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      76h,77h,78h,79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      86h,87h,88h,89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      96h,97h,98h,99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0A6h,0A7h,0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0B6h,0B7h,0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0C6h,0C7h,0C8h,0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0D6h,0D7h,0D8h,0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0E6h,0E7h,0E8h,0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 7
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      27h,27h,27h,27h,27h,27h,27h,27h,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      47h,48h,49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      57h,58h,59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      67h,68h,69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      77h,78h,79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      87h,88h,89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      97h,98h,99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0A7h,0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0B7h,0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0C7h,0C8h,0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0D7h,0D8h,0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0E7h,0E8h,0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 8
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      27h,27h,27h,27h,27h,27h,27h,27h,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      48h,49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      58h,59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      68h,69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      78h,79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      88h,89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      98h,99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0A8h,0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0B8h,0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0C8h,0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0D8h,0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0E8h,0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 9
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      27h,27h,27h,27h,27h,27h,27h,27h,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      49h,4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      59h,5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      69h,6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      79h,7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      89h,8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      99h,9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0A9h,0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0B9h,0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0C9h,0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0D9h,0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0E9h,0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 10
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      27h,27h,27h,27h,27h,27h,27h,27h,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      4Ah,4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      5Ah,5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      6Ah,6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      7Ah,7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      8Ah,8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      9Ah,9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0AAh,0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0BAh,0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0CAh,0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0DAh,0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0EAh,0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 11
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      27h,27h,27h,27h,27h,27h,27h,27h,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      4Bh,4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      5Bh,5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      6Bh,6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      7Bh,7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      8Bh,8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      9Bh,9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0ABh,0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0BBh,0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0CBh,0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0DBh,0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0EBh,0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 12
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      27h,27h,27h,27h,27h,27h,27h,27h,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      4Ch,4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      5Ch,5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      6Ch,6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      7Ch,7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      8Ch,8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      9Ch,9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0ACh,0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0BCh,0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0CCh,0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0DCh,0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0ECh,0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 13
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      27h,27h,27h,27h,27h,27h,27h,27h,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      4Dh,4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      5Dh,5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      6Dh,6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      7Dh,7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      8Dh,8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      9Dh,9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0ADh,0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0BDh,0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0CDh,0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0DDh,0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0EDh,0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 14
                db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
                db      10h,11h,12h,13h,14h,15h,16h,17h,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
                db      27h,27h,27h,27h,27h,27h,27h,27h,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh,2Fh
                db      3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh,3Ch,3Dh,3Eh,3Fh
                db      4Eh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh,4Fh
                db      5Eh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh,5Fh
                db      6Eh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh,6Fh
                db      7Eh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh,7Fh
                db      8Eh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh,8Fh
                db      9Eh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh,9Fh
                db      0AEh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh,0AFh
                db      0BEh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh,0BFh
                db      0CEh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh,0CFh
                db      0DEh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh,0DFh
                db      0EEh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh,0EFh
                db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; shade 15
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
                db      00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h

; seg004_0849_7D3E  (+7D3E)
_do_fetchmap proc near
        lodsw
        mov     di,ax
        lodsw
        mov     bx,ax
        push    es
        push    ds
        push    si
        mov     ax,bx
        shr     ax,2
        add     al,byte ptr ds:[0E4CFh]
        and     bx,3
        add     bx,0Ch
        shl     bx,8
        mov     si,bx
        cmp     al,byte ptr ds:[0E4CEh]
        je      short L7D77
        mov     byte ptr ds:[0E4CEh],al         ; map that EMS page into physical page 3
        xor     bh,bh
        mov     bl,al
        mov     ah,44h
        mov     al,3
        mov     dx,word ptr ds:[0E4D4h]
        int     67h
        or      ah,ah
        je      short L7D77
        int     3
L7D77:
        mov     ax,si
        mov     bx,seg seg052_519C
        mov     es,bx
        mov     si,0C802h
        shl     di,1
        inc     di
        shl     di,2
        add     si,di
        add     ax,word ptr ds:[0E4D2h]
        mov     word ptr es:[si],ax
        mov     ds,si
        xor     ah,ah
        mov     al,byte ptr ds:[0]
        mov     word ptr es:[2696h],ax
        pop     si
        pop     ds
        pop     es
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_fetchmap endp

; seg004_0849_7DA4  (+7DA4)
_do_settmobj proc near
        lodsw
        mov     cx,ax
        lodsw
        push    es
        push    ds
        push    si
        push    ax
        mov     ax,cx
        mov     bx,0D049h
        shl     ax,1
        add     bx,ax
        mov     ax,word ptr [bx]
        call    _seg004_0849_7EDE
        mov     bx,ax
        push    bx
        mov     di,0C832h
        mov     ax,seg seg052_519C
        mov     es,ax
        mov     ds,bx
        xor     ah,ah
        mov     al,byte ptr ds:[1]
        stosw
        mov     ax,word ptr ds:[3]
        mov     bx,ax
        dec     ax
        stosw
        mov     ax,seg _seg_5DFD
        stosw
        mov     al,byte ptr ds:[1]
        mov     cl,8
L7DDD:
        dec     cl
        shl     al,1
        jae     L7DDD
        mov     al,byte ptr ds:[2]
        dec     al
        xor     ah,ah
        shl     ax,cl
        stosw
        mov     cx,bx
        pop     dx
        pop     bx
        mov     si,5
        mov     ds,dx
        mov     di,0
        mov     ax,seg _seg_5DFD
        mov     es,ax
        rep movsb
        pop     si
        pop     ds
        pop     es
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
        lodsw
L7E0A:
        sub     ax,1C0h
        add     ax,word ptr ds:[0D030h]
        jmp     short L7E26
_do_settmobj endp

; seg004_0849_7E13  (+7E13)
_do_uwobj proc near
        lodsw
        cmp     ax,1C0h
        jge     L7E0A
        mov     bx,0D849h
        shl     ax,2
        add     bx,ax
        mov     ax,word ptr [bx]
        and     ax,3FFh
L7E26:
        mov     bx,0D049h
        shl     ax,1
        add     bx,ax
        mov     ax,word ptr [bx]
        call    _seg004_0849_7EDE
        mov     bx,ax
        lodsw
        mov     ch,al
        mov     dx,bx
        push    si
        push    bx
        mov     ds,bx
        xor     bh,bh
        mov     bl,byte ptr ds:[0]
        mov     al,byte ptr ds:[3]
        cmp     al,20h
        jb      short L7E4E
        sub     al,8
        shl     al,1
L7E4E:
        mov     cl,4
        xor     ah,ah
        shl     ax,cl
        mov     si,offset DGROUP:_Palettes
        add     si,ax
        mov     ax,DGROUP
        mov     ds,ax
        mov     ax,dx
        mov     dh,ch
        mov     bp,4
        call    word ptr cs:uncmp_tab[bx]
        pop     ds
        mov     di,0C856h
        mov     bx,seg seg052_519C
        mov     es,bx
        stosw
        mov     si,1
        movsb
        inc     di
        movsb
        mov     ax,seg seg052_519C
        mov     ds,ax
        pop     si
        lodsw
        mov     di,ax
        add     di,14D0h
        mov     ax,1Ch
        db      0EAh                     ; jmp far ptr in_scalebm: TASM would shorten it
        dw      offset in_scalebm
        dw      seg in_scalebm
_do_uwobj endp

; seg004_0849_7E8F  (+7E8F)
_do_uwcrit proc near
        lodsw
        mov     cx,ax
        mov     bx,0E049h
        add     bx,ax
        add     bx,ax
        mov     al,byte ptr [bx]
        xor     ah,ah
        cmp     ax,0FFh
        je      short L7ED5
        mov     dl,byte ptr [bx+1]
        mov     bx,0E2C9h
        mov     bp,ax
        shl     ax,3
        add     bx,ax
        lodsw
        or      ax,ax
        je      short L7EB6
        mov     dl,3
L7EB6:
        lodsw
        mov     dh,al
        push    dx
        lodsw
        mov     cx,7
L7EBE:
        cmp     al,byte ptr [bx]
        jbe     short L7EC5
        inc     bx
        loop    L7EBE
L7EC5:
        mov     bx,7
        sub     bx,cx
        mov     dx,bp
        mov     bp,ax
        call    _load_cpg
        pop     dx
        jmp     _parse_cpg
L7ED5:
        add     si,8
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_uwcrit endp

; seg004_0849_7EDE  (+7EDE)
_seg004_0849_7EDE proc near
        mov     di,ax
        mov     cl,0Ch
        shr     ax,cl
        cmp     al,byte ptr ds:[0E4D1h]
        je      short L7EFD
        mov     byte ptr ds:[0E4D1h],al
        mov     bx,ax
        mov     ah,44h
        mov     al,2
        mov     dx,word ptr ds:[0E4D4h]
        int     67h
        or      ah,ah
        jne     short L7F0A
L7EFD:
        mov     ax,di
        and     ax,3FFh
        add     ax,word ptr ds:[0E4D2h]
        add     ax,800h
        ret
L7F0A:
        int     3
        ret
_seg004_0849_7EDE endp

; seg004_0849_7F0C  (+7F0C)
_seg004_0849_7F0C proc far
        call    word ptr cs:uncmp_tab[bx]
        retf
_seg004_0849_7F0C endp

; update_cache  (+7F12): FM update_cache. It saves DS in BX and then reuses BX, so it
; returns with DS = 0E3C9h; cRender reloads DS afterwards.
_update_cache proc far
        mov     bx,ds
        mov     ax,seg _CmapCache
        mov     ds,ax
        mov     di,0FCh
        mov     bx,0E3C9h
L7F1F:
        mov     al,byte ptr [bx+di]
        cmp     al,0FFh
        je      short L7F2B
        dec     al
        je      short L7F2B
        mov     byte ptr [bx+di],al
L7F2B:
        dec     di
        jne     L7F1F
        mov     ds,bx
        retf
_update_cache endp

; seg004_0849_7F31  (+7F31)
_unload_cpg proc near
        mov     di,0FFh
        mov     dl,0FFh
        mov     dh,0FFh
L7F38:
        mov     al,byte ptr [di-1C37h]
        cmp     al,dl
        jae     short L7F46
        mov     dl,al
        mov     ax,di
        mov     dh,al
L7F46:
        dec     di
        jne     L7F38
        cmp     dh,0FFh
        je      short L7F81
        mov     di,0E1C9h
        mov     ax,bx
        xchg    dh,dl
        xor     dh,dh
        mov     bx,dx
        mov     byte ptr [bx-1C37h],0FFh
        mov     cl,byte ptr [bx-1F37h]
        mov     byte ptr [bx-1F37h],0FFh
        mov     bl,cl
        mov     byte ptr [bx-1E37h],al
        mov     bx,ax
        mov     byte ptr [bx-1F37h],cl
        xor     ch,ch
        mov     bx,ax
        mov     dx,ax
        and     ax,7
        shr     dx,3
        jmp     short L7FC2
L7F81:
        add     sp,4
        add     si,4
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_unload_cpg endp

; seg004_0849_7F8D  (+7F8D)
_load_cpg proc near
        shl     dx,3
        add     bx,dx
        mov     al,byte ptr [bx-1F37h]
        cmp     al,0FFh
        je      short L7F9D
        jmp     _lcpg_pghere
L7F9D:
        mov     di,0E1C9h
        mov     al,0FEh
        mov     cx,ds
        mov     es,cx
        mov     cx,word ptr ds:[0E4CBh]
        repne scasb
        jne     _unload_cpg
foundfreepage:                          ; FM foundfreepage
        dec     di
        mov     ax,bx
        stosb
        sub     ax,dx
        shr     dx,3
        neg     cx
        add     cx,word ptr ds:[0E4CBh]
        dec     cx
        mov     byte ptr [bx-1F37h],cl
L7FC2:
        mov     word ptr ds:[0D047h],bx
        push    cx
        mov     ch,al
        mov     di,0D03Ah
        add     di,7
        mov     ax,dx
        mov     cl,3
        shr     ax,cl
        add     al,30h
        stosb
        mov     ax,dx
        and     ax,7
        add     al,30h
        stosb
        inc     di
        mov     al,ch
        shr     al,cl
        add     al,30h
        stosb
        mov     al,ch
        and     al,7
        add     al,30h
        stosb
        mov     dx,0D03Ah
        mov     ax,3D00h
        int     21h
        jb      short L802D
        mov     dx,ax
        pop     ax
        push    dx
        call    _lcpg_pghere
        pop     bx
        mov     cx,7FFFh
        mov     dx,0
        mov     ax,word ptr ds:[0E4D2h]
        mov     ds,ax
        mov     ax,3F00h
        int     21h
        lahf
        mov     cl,ah
        mov     ax,3E00h
        int     21h
        mov     ah,cl
        sahf
        mov     ax,seg seg052_519C
        mov     ds,ax
        jb      short L8032
        mov     bx,word ptr ds:[0D047h]
        mov     byte ptr [bx-1C37h],10h
        ret
L802D:
        int     2
        add     sp,2
L8032:
        int     2
        add     si,2
        add     sp,4
        mov     bx,word ptr ds:[0D047h]
        xor     ch,ch
        mov     byte ptr [bx-1C37h],0FFh
        mov     cl,byte ptr [bx-1F37h]
        mov     byte ptr [bx-1F37h],0FFh
        mov     bl,cl
        mov     byte ptr [bx-1E37h],0FEh
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_load_cpg endp

; seg004_0849_805B  (+805B)
_lcpg_pghere proc near
        mov     dl,byte ptr [bx-1C37h]
        cmp     dl,0F0h
        jae     short L8069
        add     byte ptr [bx-1C37h],2
L8069:
        cmp     al,byte ptr ds:[0E4CDh]
        jne     short lcpg_swap
        ret
lcpg_swap:
        mov     byte ptr ds:[0E4CDh],al
        xor     ah,ah
        shl     ax,1
        add     ax,word ptr ds:[0E4C9h]
        push    si
        mov     si,0D032h
        mov     word ptr [si],ax
        inc     ax
        add     si,4
        mov     word ptr [si],ax
        sub     si,4
        mov     cx,2
        mov     ax,5000h
        mov     dx,word ptr ds:[0E4D4h]
        int     67h
        pop     si
        or      ah,ah
        jne     short L809C
        ret
L809C:
        int     2
        ret
_lcpg_pghere endp

; seg004_0849_809F  (+809F)
_parse_cpg proc near
        mov     bx,bp
        mov     ax,word ptr ds:[0E4D2h]
        mov     ds,ax
        xor     ah,ah
        mov     al,dl
        mov     bp,ax
        mov     cx,5
        shl     bp,cl
        mov     di,80h
        shl     bx,1
        mov     cx,word ptr [bx+di]
        push    si
        push    ds
        push    cx
        mov     si,bp
        mov     ax,ds
        mov     bp,cx
        mov     bl,byte ptr ds:[bp+4]
        xor     bh,bh
        add     bp,5
        call    word ptr cs:uncmp_tab[bx]
        pop     si
        pop     ds
        mov     di,0C848h
        mov     bx,seg seg052_519C
        mov     es,bx
        stosw
        movsb
        inc     di
        movsb
        inc     di
        movsb
        inc     di
        movsb
        pop     si
        mov     ax,seg seg052_519C
        mov     ds,ax
        lodsw
        mov     di,ax
        add     di,14D0h
        mov     ax,0Eh
        db      0EAh                     ; jmp far ptr in_scalebm: TASM would shorten it
        dw      offset in_scalebm
        dw      seg in_scalebm
_parse_cpg endp

; seg004_0849_80F6  (+80F6)
_do_setbmcol proc near
        mov     bx,es
        mov     ax,seg seg_370D
        mov     es,ax
        lodsw
        mov     byte ptr es:[0DC1h],al
        mov     es,bx
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_setbmcol endp

; seg004_0849_810A  (+810A)
_do_mouseq proc near
        call    far ptr _seg021_22FD_102B
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+24F4h]
_do_mouseq endp

; seg004_0849_8115  (+8115)
_do_uwcolv proc near
        test    word ptr ds:[2692h],0FFh
        jne     short L8141
        lodsw
        mov     di,ax
        lodsw
        mov     bx,ax
        mov     ax,word ptr [di]
        xchg    bh,bl
        mov     cx,word ptr ds:[2694h]
        add     bh,cl
        cmp     bh,0Fh
        jbe     short L8134
        mov     bh,0Fh
L8134:
        add     bx,offset lightabs
        add     bx,ax
        mov     al,byte ptr cs:[bx]
        xor     ah,ah
        jmp     short L8151
L8141:
        push    es
        mov     ax,seg seg_370D
        mov     es,ax
        mov     al,byte ptr es:[0DC1h]
        xor     ah,ah
        pop     es
        add     si,4
L8151:
        jmp     setcol_common
_do_uwcolv endp

SEG004_TEXT ends

SEG021_TEXT segment para public use16 'CODE'
SEG021_TEXT ends

        end
