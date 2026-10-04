; /* target: seg004_6950 */
; /* opts: /ml */
;
; The art side of the renderer: the opcode handlers that fetch textures and pictures from EMS
; and draw them (do_fetchmap, do_settmobj, do_uwobj, do_uwcrit, do_uwcolv, do_setbmcol,
; do_mouseq), and the critter page cache that pages critter animation files into EMS on demand
; (load_cpg, unload_cpg, lcpg_pghere, parse_cpg, update_cache). It also holds uncmp_tab, the
; image decoders by format (EXPAND.ASM), and lightabs, the 16 distance-shading rows the
; renderer's shading reads. Its _DATA is the far pointers grs_off and obj_tab that the C
; (gfx/GRSPIC.C, gfx/LOADGR.C) uses to reach the same tables.
;
; UW1 against UW2: the same routines with UW1's data offsets; parse_cpg reads UW1's critter
; page layout, and do_uwobj and parse_cpg jump to in_scalebm as UW2's do. lightabs is UW1's
; own table.
;
; name: UW2's (descriptive there). The routines' names are FM Towns', through UW2.
;
; .186, where UW2 has .386: UW1's seg004 has no 386 instruction and none of the nop padding
; that TASM's .386 mode puts after forward conditional jumps.
;
; SEG004 split into its original modules: this is module 9 of 9, +6950..+7DDC of the segment
; (file 109C0h), a module of the second library; it ends the segment (4 zero bytes to 7DE0h).
; match: evidence for the bounds: starts on a paragraph after 12 zero bytes following TMAPOPS'
; last iret. It opens with 16 bytes of data that seg003 reads (es:6950h).
; Its relocations are entries 2746-2762 of UW.EXE's relocation table.
; Its _DATA is DS:23A4..23B4 (relocation entries 2743-2745), defined before the code because
; those entries come before the code's.
;
; The empty SEG019_TEXT declaration at the end is the later paragraph-aligned contribution
; that makes seg019's segment end at 1080h, 15 zero bytes after its last routine (the segment
; table counts them as seg019's; the link showed it, entry 21's length being 1071h without
; it). Which module made it is not known; as in UW2 (UW2Decomp's PGCACHE.ASM), this is the
; first module linked after seg019's last one that calls into seg019.
;
; The routine comments are UW2's, with UW1's addresses, except where marked UW1.

; The model interpreter's opcode table: a word per opcode, the offset of its handler
; (INTERP.ASM), in seg051, the data segment while interpreting (UW2: 24F4h).
OPCODE_TABLE    equ     2738h

; The renderer's data in seg051 (DS while interpreting), as INTERP.ASM's header
; describes it. The names are ours, from the routines that use the data.
PNT_X           equ     1620h   ; the point table, 8 bytes a point: view x
DOS_OPEN        equ     3Dh
DOS_CLOSE       equ     3Eh
DOS_READ        equ     3Fh
; The EMS (int 67h) functions used here.
EMS_MAP_PAGE    equ     44h     ; map one logical page into a physical page
EMS_MAP_PAGES   equ     50h

        .186

        extrn   seg051:far
        extrn   seg048:far
        extrn   _seg_5DFD:far
        extrn   _CmapCache:far
        extrn   _seg019_102B:far

_DATA   segment word public 'DATA'
_grs_off label dword
        dd      seg051+0B10Fh
_obj_tab label dword
        dd      seg051+0B90Fh
        dd      seg048+0B1Ah
        db      00h,00h,00h,00h
        extrn   _Palettes:byte                  ; LOADGR's object palettes, DS:573E
_DATA   ends
DGROUP  group   _DATA

SEG004_TEXT segment para public 'code'
        assume  cs:SEG004_TEXT, ds:DGROUP, ss:DGROUP
        extrn   _exp_4run:near
        extrn   _exp_4str:near
        extrn   _exp_5run:near
        extrn   _exp_8run:near
        extrn   _exp_8str:near
        extrn   in_scalebm:near
        extrn   setcol_common:near
        public  _do_fetchmap
        public  _do_mouseq
        public  _do_setbmcol
        public  _do_settmobj
        public  _do_uwcolv
        public  _do_uwcrit
        public  _do_uwobj
        public  _grs_off
        public  _lcpg_pghere
        public  _load_cpg
        public  _parse_cpg
        public  _obj_tab
        public  _seg004_7B09
        public  _seg004_7B37
        public  _seg004_7BBA
        public  _unload_cpg
        public  _update_cache
        public  lightabs

_seg004_6950    db      10h dup (0)             ; seg003 reads its first byte (es:6950h)

; uncmp_tab: FM uncmp_tab, the decoder for each image format byte (do_uwobj and
; parse_cpg call through it with BX = format * 2)
uncmp_tab       dw      offset L696C, offset _exp_8run, offset _exp_8str
                dw      offset _exp_5run, offset _exp_4run, offset _exp_4str
L696C:                                  ; format 0: not an image
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

; seg004_796E  (+796E)
; do_fetchmap (opcode 0x3E): load a wall or floor texture into a bitmap slot. Operands: the
; slot, the texture number, the next opcode. Textures are 4 KB (64 x 64) and four share a 16 KB
; EMS page: page tmap_fpage + n / 4 is mapped into physical page 3 (int 67h, 44h, unless it is
; already there; an EMS error breaks with int 3) and the slot's word +4 gets the texture's
; segment, EmsBuff + 0C00h + (n & 3) * 100h.
;
; It then sets tmcolor (2936) from the first byte of the texture, but DOS loads DS from SI (the
; slot's address) where FM Towns reads the texture itself, so the byte it reads is not the
; texture's. Probably a DOS slip; what reads tmcolor has not been traced.
_do_fetchmap proc near
        lodsw
        mov     dx,ax
        lodsw
        mov     bx,ax
        lodsw
        shr     ax,4
        mov     cx,ax
        push    es
        push    ds
        push    si
        push    dx
        mov     al,byte ptr [bx-3C88h]
        shl     bx,1
        mov     si,word ptr [bx-3C14h]
        cmp     al,0
        je      short L79A8
        cmp     al,byte ptr ds:[0C374h]
        je      short L79A8
        mov     byte ptr ds:[0C374h],al         ; map that EMS page into physical page 3
        xor     bh,bh
        mov     bl,al
        mov     ah,EMS_MAP_PAGE
        mov     al,3
        mov     dx,word ptr ds:[0C4D7h]
        int     67h
        or      ah,ah
        je      short L79A8
        int     3
L79A8:
        pop     dx
        mov     bx,seg seg051
        mov     es,bx
        mov     bx,0B07Eh
        mov     ax,dx
        shl     ax,1
        inc     ax
        shl     ax,2
        add     bx,ax
        mov     word ptr es:[bx],si
        mov     ds,si
        xor     ah,ah
        mov     al,byte ptr ds:[0]
        mov     word ptr es:[2936h],ax
        pop     si
        pop     ds
        pop     es
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_fetchmap endp

; seg004_79D2  (+79D2)
; do_settmobj (opcode 0xC0): use an object's picture as the texture of bitmap slot 6 (B0AE, FM
; bm_number_6), for a 3D model drawn with an object's own art. Operands: the grs_off slot, an
; unused word, the next opcode. The picture is reached through seg004_7B09; the slot gets
; the picture's width (byte 1), its byte count (word 3) - 1, the segment of the 4 KB buffer
; _seg_5DFD, and (height - 1) shifted by 8 - log2(width) as the row mask; then the picture's
; pixels (from byte 5, word 3 of them) are copied into _seg_5DFD, where the mapper reads them.
; (The `lodsw` after the dispatch jump is never reached. L7A38 is do_uwobj's branch for
; animations.)
_do_settmobj proc near
        lodsw
        mov     cx,ax
        lodsw
        push    es
        push    ds
        push    si
        push    ax
        mov     ax,cx
        mov     bx,0B10Fh
        shl     ax,1
        add     bx,ax
        mov     ax,word ptr [bx]
        call    _seg004_7B09
        mov     bx,ax
        push    bx
        mov     di,0B0AEh
        mov     ax,seg seg051
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
L7A0B:
        dec     cl
        shl     al,1
        jae     L7A0B
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
        jmp     word ptr [bx+OPCODE_TABLE]
        lodsw
L7A38:
        sub     ax,1C0h
        add     ax,word ptr ds:[0B0F0h]
        jmp     short L7A54
_do_settmobj endp

; seg004_7A41  (+7A41)
; do_uwobj (opcode 0x3A): draw an object's sprite. Operands: the object type, the shade (dh for
; the decoder), the point. Types below 1C0h take their picture slot from obj_tab (bits 0-9);
; 1C0h and up are animations, slot first_anim + type - 1C0h. The picture (byte 0 the format, 1
; and 2 width and height, 3 its auxiliary palette, pixels from 4) is decoded through uncmp_tab
; with the 16-byte palette from LOADGR's _Palettes (palette numbers from 20h are folded down: (n
; - 8) * 2), the decoded bitmap's address and size go to the sprite record at B0D2 (FM
; sbm_inventory), and it is drawn by in_scalebm (TMAPOPS.ASM) with record offset 1Ch.
_do_uwobj proc near
        lodsw
        cmp     ax,1C0h
        jge     L7A38
        mov     bx,0B90Fh
        shl     ax,2
        add     bx,ax
        mov     ax,word ptr [bx]
        and     ax,3FFh
L7A54:
        mov     bx,0B10Fh
        shl     ax,1
        add     bx,ax
        mov     ax,word ptr [bx]
        call    _seg004_7B09
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
        jb      short L7A7C
        sub     al,8
        shl     al,1
L7A7C:
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
        mov     di,0B0D2h
        mov     bx,seg seg051
        mov     es,bx
        stosw
        mov     si,1
        movsb
        inc     di
        movsb
        mov     ax,seg seg051
        mov     ds,ax
        pop     si
        lodsw
        mov     di,ax
        add     di,PNT_X
        mov     ax,1Ch
        db      0EAh                     ; match: jmp far ptr in_scalebm as bytes, as in UW2
        dw      offset in_scalebm
        dw      seg in_scalebm
_do_uwobj endp

; seg004_7ABD  (+7ABD)
; do_uwcrit (opcode 0x5A): draw a critter's animation frame. Operands: the critter type (index
; into grs_3dinf), a flag (nonzero: palette 3 in place of the critter's own; probably the
; shading used for a critter being hit or a special state, not checked), the shade, the frame
; number, the point. A type with no critter map (0FFh) is skipped. The frame picks the page: the
; first of the critter map's eight CmapFrm bytes not below it. load_cpg makes sure the page is
; in the cache and mapped; parse_cpg decodes and draws the frame.
_do_uwcrit proc near
        lodsw
        mov     cx,ax
        mov     bx,0C10Fh
        add     bx,ax
        add     bx,ax
        mov     al,byte ptr [bx]
        xor     ah,ah
        cmp     ax,0FFh
        je      short L7B00
        mov     dl,byte ptr [bx+1]
        mov     bx,0C28Fh
        add     bx,ax
        add     bx,ax
        add     bx,ax
        mov     bp,ax
L7ADE:
        lodsw
        mov     dh,al
        push    dx
        lodsw
        mov     cx,3
L7AE6:
        cmp     al,byte ptr [bx]
        jb      short L7AED
        inc     bx
        loop    L7AE6
L7AED:
        mov     bx,3
        sub     bx,cx
        mov     dx,bp
        mov     bp,ax
        call    _seg004_7BBA
        pop     dx
        lodsw
        mov     bx,ax
        jmp     _parse_cpg
L7B00:
        add     si,8
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_uwcrit endp

; seg004_7B09  (+7B09)
; seg004_7B09 (DOS only; FM Towns has flat memory): turn a grs_off word into a segment. AX
; = the word: bits 12-15 the EMS logical page, mapped into physical page 2 unless it is the one
; there (obj_inpage1; an EMS error breaks with int 3), bits 0-9 the paragraph within the 16 KB
; page. Out: AX = EmsBuff + 800h + paragraph. Changes BX, CL, DX, DI. GRSPIC.C's C code does the
; same for the 2D side.
_seg004_7B09 proc near
        mov     di,ax
        mov     cl,0Ch
        shr     ax,cl
        cmp     al,byte ptr ds:[0C4D4h]
        je      short L7B28
        mov     byte ptr ds:[0C4D4h],al
        mov     bx,ax
        mov     ah,EMS_MAP_PAGE
        mov     al,2
        mov     dx,word ptr ds:[0C4D7h]
        int     67h
        or      ah,ah
        jne     short L7B35
L7B28:
        mov     ax,di
        and     ax,3FFh
        add     ax,word ptr ds:[0C4D5h]
        add     ax,800h
        ret
L7B35:
        int     3
        ret
_seg004_7B09 endp

; seg004_7B37  (+7B37)
; seg004_7B37: far entry to the decoders for C (cFrmtoRaw), BX = format * 2, other
; registers as the decoder wants (EXPAND.ASM).
_seg004_7B37 proc far
        call    word ptr cs:uncmp_tab[bx]
        retf
_seg004_7B37 endp

; update_cache  (+7B3D): FM update_cache. It saves DS in BX and then reuses BX, so it
; returns with DS = 0C2EFh; cRender reloads DS afterwards.
; update_cache: called by cRender after each frame. Ages every loaded critter page (CmapCache
; entries 1 to 0FCh not 0FFh) by one, stopping at 1, so the least recently used page has the
; lowest count.
_update_cache proc far
        mov     bx,ds
        mov     ax,seg _CmapCache
        mov     ds,ax
        mov     di,7Fh
        mov     bx,0C2EFh
L7B4A:
        mov     al,byte ptr [bx+di]
        cmp     al,0FFh
        je      short L7B56
        dec     al
        je      short L7B56
        mov     byte ptr [bx+di],al
L7B56:
        dec     di
        jne     L7B4A
        mov     ds,bx
        retf
_update_cache endp

; seg004_7B5C  (+7B5C)
; unload_cpg: entered from load_cpg when no cache slot is free, BX = the cache index wanted.
; Finds the loaded page with the lowest age, takes its slot (marks it unloaded), gives the slot
; to BX and joins load_cpg to read the file. If nothing is loaded either, it drops the critter:
; discards load_cpg's return address and the pushed shade, skips the remaining operand and
; dispatches the next opcode.
_unload_cpg proc near
        mov     di,7Fh
        mov     dl,0FFh
        mov     dh,0FFh
L7B63:
        mov     al,byte ptr [di-3D11h]
        cmp     al,dl
        jae     short L7B71
        mov     dl,al
        mov     ax,di
        mov     dh,al
L7B71:
        dec     di
        jne     L7B63
        cmp     dh,0FFh
        je      short L7BAC
        mov     di,0C20Fh
        mov     ax,bx
        xchg    dh,dl
        xor     dh,dh
        mov     bx,dx
        mov     byte ptr [bx-3D11h],0FFh
        mov     cl,byte ptr [bx-3E71h]
        mov     byte ptr [bx-3E71h],0FFh
        mov     bl,cl
        mov     byte ptr [bx-3DF1h],al
        mov     bx,ax
        mov     byte ptr [bx-3E71h],cl
        xor     ch,ch
        mov     bx,ax
        mov     dx,ax
        and     ax,3
        shr     dx,2
        jmp     short L7BF0
L7BAC:
        int     2
L7BAE:
        add     sp,4
        add     si,4
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_unload_cpg endp

; seg004_7BBA  (+7BBA)
; UW1 only: load_cpg's entry that doubles DX first (do_uwcrit calls it).
_seg004_7BBA proc near
        shl     dx,1
_seg004_7BBA endp

; seg004_7BBC  (+7BBC)
; load_cpg: make critter page (DX = critter map, BX = page 0-7) present and mapped. If CmaptoPg
; has a slot for it, lcpg_pghere maps it. Otherwise it takes a free slot (PgtoCmap 0FEh, or
; unload_cpg's), builds the file name crit\crMM.0P from the map number in two octal digits and
; the page, opens it, maps the slot and reads up to 7FFFh bytes into the page frame, and sets
; its age to 10h. Out: the slot mapped into physical pages 0 and 1. If the file does not open or
; read, it breaks with int 2, frees the slot and skips the critter (dispatching the next
; opcode).
_load_cpg proc near
        shl     dx,1
        add     bx,dx
        mov     al,byte ptr [bx-3E71h]
        cmp     al,0FFh
        je      short L7BCB
        jmp     _lcpg_pghere
L7BCB:
        mov     di,0C20Fh
        mov     al,0FEh
        mov     cx,ds
        mov     es,cx
        mov     cx,word ptr ds:[0C371h]
        repne scasb
        jne     _unload_cpg
foundfreepage:                          ; FM foundfreepage
        dec     di
        mov     ax,bx
        stosb
        sub     ax,dx
        shr     dx,2
        neg     cx
        add     cx,word ptr ds:[0C371h]
        dec     cx
        mov     byte ptr [bx-3E71h],cl
L7BF0:
        mov     word ptr ds:[0B10Dh],bx
        push    cx
        mov     ch,al
        mov     di,0B0FAh
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
        add     di,6
        mov     al,ch
        shr     al,cl
        add     al,30h
        stosb
        mov     al,ch
        and     al,7
        add     al,30h
        stosb
        mov     dx,0B0FAh
        mov     ax,DOS_OPEN shl 8
        int     21h
        jb      short L7C5D
        mov     dx,ax
        pop     ax
        push    dx
        call    _lcpg_pghere
        pop     bx
        mov     cx,7FFFh
        mov     dx,0
        mov     ax,word ptr ds:[0C4D5h]
        mov     ds,ax
        mov     ax,DOS_READ shl 8
        int     21h
        lahf
        mov     cl,ah
        mov     ax,DOS_CLOSE shl 8
        int     21h
        mov     ah,cl
        sahf
        mov     ax,seg seg051
        mov     ds,ax
        jb      short L7C62
        mov     bx,word ptr ds:[0B10Dh]
        mov     byte ptr [bx-3D11h],10h
        ret
L7C5D:
        int     2
        add     sp,2
L7C62:
        int     2
        add     si,4
        add     sp,4
        mov     bx,word ptr ds:[0B10Dh]
        xor     ch,ch
        mov     byte ptr [bx-3D11h],0FFh
        mov     cl,byte ptr [bx-3E71h]
        mov     byte ptr [bx-3E71h],0FFh
        mov     bl,cl
        mov     byte ptr [bx-3DF1h],0FEh
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_load_cpg endp

; seg004_7C8B  (+7C8B)
; lcpg_pghere: AL = cache slot, BX = cache index. Adds 2 to the page's age (up to 0F0h) and,
; unless the slot is already mapped (crit_inpage), maps its two EMS pages (crit_fpage + slot * 2
; and the next) into physical pages 0 and 1 with int 67h function 50h. An EMS error breaks with
; int 2.
_lcpg_pghere proc near
        mov     dl,byte ptr [bx-3D11h]
        cmp     dl,0F0h
        jae     short L7C99
        add     byte ptr [bx-3D11h],2
L7C99:
        cmp     al,byte ptr ds:[0C373h]
        jne     short lcpg_swap
        ret
lcpg_swap:
        mov     byte ptr ds:[0C373h],al
        xor     ah,ah
        shl     ax,1
        add     ax,word ptr ds:[0C36Fh]
        push    si
        mov     si,0B0F2h
        mov     word ptr [si],ax
        inc     ax
        add     si,4
        mov     word ptr [si],ax
        sub     si,4
        mov     cx,2
        mov     ax,EMS_MAP_PAGES shl 8
        mov     dx,word ptr ds:[0C4D7h]
        int     67h
        pop     si
        or      ah,ah
        jne     short L7CCC
        ret
L7CCC:
        int     2
        ret
L7CCF:
        mov     ax,seg seg051
        mov     ds,ax
        add     si,2
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_lcpg_pghere endp

; seg004_7CDD  (+7CDD)
; parse_cpg (FM name): decode and draw a frame from the mapped critter page, as UW2's does,
; but UW1's page layout differs: the frame is looked up through the page's tables, and a frame
; that is not there (0FFh) or a palette number not below the page's count skips the opcode
; (L7CCF, before this routine). In: BP = frame number, DL = the palette, DH = the shade.
_parse_cpg proc near
        mov     ax,word ptr ds:[0C4D5h]
        mov     ds,ax
        xor     di,di
        xor     ah,ah
        xor     ch,ch
        mov     al,byte ptr [di]
        sub     bp,ax
        mov     al,byte ptr [di+1]
        mov     cl,byte ptr ds:[bp+di+2]
        cmp     cl,0FFh
        je      L7CCF
        mov     bp,cx
        add     di,2
        add     di,ax
        mov     al,byte ptr [di]
        inc     di
        mov     cl,3
        shl     bp,cl
        add     bp,bx
        mov     bl,byte ptr ds:[bp+di]
        mov     bh,0FFh
        cmp     bl,bh
        jne     short L7D13
        mov     bl,0
L7D13:
        xor     bh,bh
        shl     ax,cl
        add     di,ax
        mov     al,byte ptr [di]
        inc     di
        cmp     al,dl
        jbe     L7CCF
        inc     cl
        inc     cl
        xor     ah,ah
        xchg    dh,ah
        mov     bp,dx
        xchg    dh,ah
        shl     bp,cl
        add     bp,di
        shl     ax,cl
        add     di,ax
        inc     di
        mov     al,byte ptr [di]
        shl     bx,1
        inc     di
        mov     cx,word ptr [bx+di]
        mov     bl,al
        xor     bh,bh
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
        mov     di,0B0C4h
        mov     bx,seg seg051
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
        mov     ax,seg seg051
        mov     ds,ax
        lodsw
        mov     di,ax
        add     di,PNT_X
        mov     ax,0Eh
        db      0EAh                     ; match: jmp far ptr in_scalebm as bytes, as in UW2
        dw      offset in_scalebm
        dw      seg in_scalebm
_parse_cpg endp

; seg004_7D7E  (+7D7E)
; do_setbmcol (opcode 0xAE): set bmcolor (seg048:DC3), the colour sprites are drawn in during
; a pick frame. Operand: the colour.
_do_setbmcol proc near
        mov     bx,es
        mov     ax,seg seg048
        mov     es,ax
        lodsw
        mov     byte ptr es:[0DC3h],al
        mov     es,bx
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_setbmcol endp

; seg004_7D92  (+7D92)
; do_mouseq (opcode 0xB0): service the mouse during a frame. Calls seg019_102B
; (C3DENTRY.ASM), which calls MousQUp; FM Towns calls MousQUp(1) directly. GRIDDB.C emits it at
; the end of each grid row, so the cursor keeps moving while a frame renders.
_do_mouseq proc near
        call    far ptr _seg019_102B
        lodsw
        xchg    bx,ax
        jmp     word ptr [bx+OPCODE_TABLE]
_do_mouseq endp

; seg004_7D9D  (+7D9D)
; do_uwcolv: set the colour for the next flat polygon from a colour word and the distance shade.
; Operands: the address (in DS) of the colour, a shade word whose low byte is added to cdist
; (2934), capped at 15, to pick the lightabs row. In a pick frame (pickup set) the colour is
; bmcolor instead. Ends in INTERP.ASM's setcol_common.
_do_uwcolv proc near
        test    word ptr ds:[2932h],0FFh
        jne     short L7DC9
        lodsw
        mov     di,ax
        lodsw
        mov     bx,ax
        mov     ax,word ptr [di]
        xchg    bh,bl
        mov     cx,word ptr ds:[2934h]
        add     bh,cl
        cmp     bh,0Fh
        jbe     short L7DBC
        mov     bh,0Fh
L7DBC:
        add     bx,696Eh
        add     bx,ax
        mov     al,byte ptr cs:[bx]
        xor     ah,ah
        jmp     short L7DD9
L7DC9:
        push    es
        mov     ax,seg seg048
        mov     es,ax
        mov     al,byte ptr es:[0DC3h]
        xor     ah,ah
        pop     es
        add     si,4
L7DD9:
        jmp     setcol_common
L8151:
_do_uwcolv endp

SEG004_TEXT ends

SEG019_TEXT segment para public 'CODE'
SEG019_TEXT ends

        end
