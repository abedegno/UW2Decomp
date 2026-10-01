; /* target: seg003_0272_D1E */
; /* opts: /ml */
;
; _D1E, the XFER colour tables (_cXfer), and the scaled-bitmap span routines with the code
; they generate at run time (_1373 and its buffer).
;
; SEG003 split into its original modules: this is module 4 of 14, +D1E..+21D4 of the segment
; (file 416Eh), a module of the second library (UWLIB.LIB).
; Procs: seg003_0272_D1E, cXfer, seg003_0272_1373.
; Evidence for the bounds: the boundary at +D1E (see SEG003C.ASM). It ends before +21D4: its
; relocation (entry 2453, +D21) and the next one in this segment (2473, +283F) are separated
; by seg004's last module, so a module boundary lies between; +21D4 is the only routine start
; on a word boundary there (+1373 and +283D are odd), but the boundary could be at any word
; between +D24 and +283E.
; Its relocations are entries 2453 of UW2.EXE's relocation table.
; Its FD51 piece is 370D:0B0A..558A (relocation entries 2451-2452), defined before the code
; because those entries come before the code's.
;
; Its FD51 piece holds relocation entries 2451-2452 (the far pointers at B0A and B18), which
; come right before its code entry. It is taken to run from B0A to SEG003K's first pointer
; (558A), since no module linked in between has FD51 relocations; how the original divided
; those bytes between the modules is not known.

        .186

        extrn   _checkerboard1:far
        extrn   seg_370D:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

FD51    segment byte public 'FAR_DATA'
        dd      seg_370D-8h
        db      0AAh,14h,0ABh,13h,0ABh,13h,0F9h,13h,0D9h,13h
        dd      _checkerboard1
        db      08h,00h,08h,00h,12h,00h,25h,01h,40h,00h,31h,00h,00h,01h,02h,03h
        db      04h,05h,06h,07h,08h,09h,10h,11h,12h,13h,14h,15h,00h,00h,00h,00h
        db      286h dup (0)
        db      72h,53h
_ShowClip label byte
        db      00h
_Transparency label byte
        db      00h,00h,00h,00h,00h,00h,0C7h,00h,00h,00h,00h,00h,00h,00h,0C6h,00h
        db      00h,00h,00h,00h,00h,00h,0C5h,00h,00h,00h,00h,00h,00h,00h,0C4h,00h
        db      00h,00h,00h,00h,00h,00h,0C3h,00h,00h,00h,00h,00h,00h,00h,0C2h,00h
        db      00h,00h,00h,00h,00h,00h,0C1h,00h,00h,00h,00h,00h,00h,00h,0C0h,00h
        db      00h,00h,00h,00h,00h,00h,0BFh,00h,00h,00h,00h,00h,00h,00h,0BEh,00h
        db      00h,00h,00h,00h,00h,00h,0BDh,00h,00h,00h,00h,00h,00h,00h,0BCh,00h
        db      00h,00h,00h,00h,00h,00h,0BBh,00h,00h,00h,00h,00h,00h,00h,0BAh,00h
        db      00h,00h,00h,00h,00h,00h,0B9h,00h,00h,00h,00h,00h,00h,00h,0B8h,00h
        db      00h,00h,00h,00h,00h,00h,0B7h,00h,00h,00h,00h,00h,00h,00h,0B6h,00h
        db      00h,00h,00h,00h,00h,00h,0B5h,00h,00h,00h,00h,00h,00h,00h,0B4h,00h
        db      00h,00h,00h,00h,00h,00h,0B3h,00h,00h,00h,00h,00h,00h,00h,0B2h,00h
        db      00h,00h,00h,00h,00h,00h,0B1h,00h,00h,00h,00h,00h,00h,00h,0B0h,00h
        db      00h,00h,00h,00h,00h,00h,0AFh,00h,00h,00h,00h,00h,00h,00h,0AEh,00h
        db      00h,00h,00h,00h,00h,00h,0ADh,00h,00h,00h,00h,00h,00h,00h,0ACh,00h
        db      00h,00h,00h,00h,00h,00h,0ABh,00h,00h,00h,00h,00h,00h,00h,0AAh,00h
        db      00h,00h,00h,00h,00h,00h,0A9h,00h,00h,00h,00h,00h,00h,00h,0A8h,00h
        db      00h,00h,00h,00h,00h,00h,0A7h,00h,00h,00h,00h,00h,00h,00h,0A6h,00h
        db      00h,00h,00h,00h,00h,00h,0A5h,00h,00h,00h,00h,00h,00h,00h,0A4h,00h
        db      00h,00h,00h,00h,00h,00h,0A3h,00h,00h,00h,00h,00h,00h,00h,0A2h,00h
        db      00h,00h,00h,00h,00h,00h,0A1h,00h,00h,00h,00h,00h,00h,00h,0A0h,00h
        db      00h,00h,00h,00h,00h,00h,9Fh,00h,00h,00h,00h,00h,00h,00h,9Eh,00h
        db      00h,00h,00h,00h,00h,00h,9Dh,00h,00h,00h,00h,00h,00h,00h,9Ch,00h
        db      00h,00h,00h,00h,00h,00h,9Bh,00h,00h,00h,00h,00h,00h,00h,9Ah,00h
        db      00h,00h,00h,00h,00h,00h,99h,00h,00h,00h,00h,00h,00h,00h,98h,00h
        db      00h,00h,00h,00h,00h,00h,97h,00h,00h,00h,00h,00h,00h,00h,96h,00h
        db      00h,00h,00h,00h,00h,00h,95h,00h,00h,00h,00h,00h,00h,00h,94h,00h
        db      00h,00h,00h,00h,00h,00h,93h,00h,00h,00h,00h,00h,00h,00h,92h,00h
        db      00h,00h,00h,00h,00h,00h,91h,00h,00h,00h,00h,00h,00h,00h,90h,00h
        db      00h,00h,00h,00h,00h,00h,8Fh,00h,00h,00h,00h,00h,00h,00h,8Eh,00h
        db      00h,00h,00h,00h,00h,00h,8Dh,00h,00h,00h,00h,00h,00h,00h,8Ch,00h
        db      00h,00h,00h,00h,00h,00h,8Bh,00h,00h,00h,00h,00h,00h,00h,8Ah,00h
        db      00h,00h,00h,00h,00h,00h,89h,00h,00h,00h,00h,00h,00h,00h,88h,00h
        db      00h,00h,00h,00h,00h,00h,87h,00h,00h,00h,00h,00h,00h,00h,86h,00h
        db      00h,00h,00h,00h,00h,00h,85h,00h,00h,00h,00h,00h,00h,00h,84h,00h
        db      00h,00h,00h,00h,00h,00h,83h,00h,00h,00h,00h,00h,00h,00h,82h,00h
        db      00h,00h,00h,00h,00h,00h,81h,00h,00h,00h,00h,00h,00h,00h,80h,00h
        db      00h,00h,00h,00h,00h,00h,7Fh,00h,00h,00h,00h,00h,00h,00h,7Eh,00h
        db      00h,00h,00h,00h,00h,00h,7Dh,00h,00h,00h,00h,00h,00h,00h,7Ch,00h
        db      00h,00h,00h,00h,00h,00h,7Bh,00h,00h,00h,00h,00h,00h,00h,7Ah,00h
        db      00h,00h,00h,00h,00h,00h,79h,00h,00h,00h,00h,00h,00h,00h,78h,00h
        db      00h,00h,00h,00h,00h,00h,77h,00h,00h,00h,00h,00h,00h,00h,76h,00h
        db      00h,00h,00h,00h,00h,00h,75h,00h,00h,00h,00h,00h,00h,00h,74h,00h
        db      00h,00h,00h,00h,00h,00h,73h,00h,00h,00h,00h,00h,00h,00h,72h,00h
        db      00h,00h,00h,00h,00h,00h,71h,00h,00h,00h,00h,00h,00h,00h,70h,00h
        db      00h,00h,00h,00h,00h,00h,6Fh,00h,00h,00h,00h,00h,00h,00h,6Eh,00h
        db      00h,00h,00h,00h,00h,00h,6Dh,00h,00h,00h,00h,00h,00h,00h,6Ch,00h
        db      00h,00h,00h,00h,00h,00h,6Bh,00h,00h,00h,00h,00h,00h,00h,6Ah,00h
        db      00h,00h,00h,00h,00h,00h,69h,00h,00h,00h,00h,00h,00h,00h,68h,00h
        db      00h,00h,00h,00h,00h,00h,67h,00h,00h,00h,00h,00h,00h,00h,66h,00h
        db      00h,00h,00h,00h,00h,00h,65h,00h,00h,00h,00h,00h,00h,00h,64h,00h
        db      00h,00h,00h,00h,00h,00h,63h,00h,00h,00h,00h,00h,00h,00h,62h,00h
        db      00h,00h,00h,00h,00h,00h,61h,00h,00h,00h,00h,00h,00h,00h,60h,00h
        db      00h,00h,00h,00h,00h,00h,5Fh,00h,00h,00h,00h,00h,00h,00h,5Eh,00h
        db      00h,00h,00h,00h,00h,00h,5Dh,00h,00h,00h,00h,00h,00h,00h,5Ch,00h
        db      00h,00h,00h,00h,00h,00h,5Bh,00h,00h,00h,00h,00h,00h,00h,5Ah,00h
        db      00h,00h,00h,00h,00h,00h,59h,00h,00h,00h,00h,00h,00h,00h,58h,00h
        db      00h,00h,00h,00h,00h,00h,57h,00h,00h,00h,00h,00h,00h,00h,56h,00h
        db      00h,00h,00h,00h,00h,00h,55h,00h,00h,00h,00h,00h,00h,00h,54h,00h
        db      00h,00h,00h,00h,00h,00h,53h,00h,00h,00h,00h,00h,00h,00h,52h,00h
        db      00h,00h,00h,00h,00h,00h,51h,00h,00h,00h,00h,00h,00h,00h,50h,00h
        db      00h,00h,00h,00h,00h,00h,4Fh,00h,00h,00h,00h,00h,00h,00h,4Eh,00h
        db      00h,00h,00h,00h,00h,00h,4Dh,00h,00h,00h,00h,00h,00h,00h,4Ch,00h
        db      00h,00h,00h,00h,00h,00h,4Bh,00h,00h,00h,00h,00h,00h,00h,4Ah,00h
        db      00h,00h,00h,00h,00h,00h,49h,00h,00h,00h,00h,00h,00h,00h,48h,00h
        db      00h,00h,00h,00h,00h,00h,47h,00h,00h,00h,00h,00h,00h,00h,46h,00h
        db      00h,00h,00h,00h,00h,00h,45h,00h,00h,00h,00h,00h,00h,00h,44h,00h
        db      00h,00h,00h,00h,00h,00h,43h,00h,00h,00h,00h,00h,00h,00h,42h,00h
        db      00h,00h,00h,00h,00h,00h,41h,00h,00h,00h,00h,00h,00h,00h,40h,00h
        db      00h,00h,00h,00h,00h,00h,3Fh,00h,00h,00h,00h,00h,00h,00h,3Eh,00h
        db      00h,00h,00h,00h,00h,00h,3Dh,00h,00h,00h,00h,00h,00h,00h,3Ch,00h
        db      00h,00h,00h,00h,00h,00h,3Bh,00h,00h,00h,00h,00h,00h,00h,3Ah,00h
        db      00h,00h,00h,00h,00h,00h,39h,00h,00h,00h,00h,00h,00h,00h,38h,00h
        db      00h,00h,00h,00h,00h,00h,37h,00h,00h,00h,00h,00h,00h,00h,36h,00h
        db      00h,00h,00h,00h,00h,00h,35h,00h,00h,00h,00h,00h,00h,00h,34h,00h
        db      00h,00h,00h,00h,00h,00h,33h,00h,00h,00h,00h,00h,00h,00h,32h,00h
        db      00h,00h,00h,00h,00h,00h,31h,00h,00h,00h,00h,00h,00h,00h,30h,00h
        db      00h,00h,00h,00h,00h,00h,2Fh,00h,00h,00h,00h,00h,00h,00h,2Eh,00h
        db      00h,00h,00h,00h,00h,00h,2Dh,00h,00h,00h,00h,00h,00h,00h,2Ch,00h
        db      00h,00h,00h,00h,00h,00h,2Bh,00h,00h,00h,00h,00h,00h,00h,2Ah,00h
        db      00h,00h,00h,00h,00h,00h,29h,00h,00h,00h,00h,00h,00h,00h,28h,00h
        db      00h,00h,00h,00h,00h,00h,27h,00h,00h,00h,00h,00h,00h,00h,26h,00h
        db      00h,00h,00h,00h,00h,00h,25h,00h,00h,00h,00h,00h,00h,00h,24h,00h
        db      00h,00h,00h,00h,00h,00h,23h,00h,00h,00h,00h,00h,00h,00h,22h,00h
        db      00h,00h,00h,00h,00h,00h,21h,00h,00h,00h,00h,00h,00h,00h,20h,00h
        db      00h,00h,00h,00h,00h,00h,1Fh,00h,00h,00h,00h,00h,00h,00h,1Eh,00h
        db      00h,00h,00h,00h,00h,00h,1Dh,00h,00h,00h,00h,00h,00h,00h,1Ch,00h
        db      00h,00h,00h,00h,00h,00h,1Bh,00h,00h,00h,00h,00h,00h,00h,1Ah,00h
        db      00h,00h,00h,00h,00h,00h,19h,00h,00h,00h,00h,00h,00h,00h,18h,00h
        db      00h,00h,00h,00h,00h,00h,17h,00h,00h,00h,00h,00h,00h,00h,16h,00h
        db      00h,00h,00h,00h,00h,00h,15h,00h,00h,00h,00h,00h,00h,00h,14h,00h
        db      00h,00h,00h,00h,00h,00h,13h,00h,00h,00h,00h,00h,00h,00h,12h,00h
        db      00h,00h,00h,00h,00h,00h,11h,00h,00h,00h,00h,00h,00h,00h,10h,00h
        db      00h,00h,00h,00h,00h,00h,0Fh,00h,00h,00h,00h,00h,00h,00h,0Eh,00h
        db      00h,00h,00h,00h,00h,00h,0Dh,00h,00h,00h,00h,00h,00h,00h,0Ch,00h
        db      00h,00h,00h,00h,00h,00h,0Bh,00h,00h,00h,00h,00h,00h,00h,0Ah,00h
        db      00h,00h,00h,00h,00h,00h,09h,00h,00h,00h,00h,00h,00h,00h,08h,00h
        db      00h,00h,00h,00h,00h,00h,07h,00h,00h,00h,00h,00h,00h,00h,06h,00h
        db      00h,00h,00h,00h,00h,00h,05h,00h,00h,00h,00h,00h,00h,00h,04h,00h
        db      00h,00h,00h,00h,00h,00h,03h,00h,00h,00h,00h,00h,00h,00h,02h,00h
        db      00h,00h,00h,00h,00h,00h,01h
        db      0Fh dup (0)
        db      0FFh,0FFh,00h,00h,00h,00h,00h,00h,0FEh,0FFh,00h,00h,00h,00h,00h,00h
        db      0CBh,0Dh
        db      18BBh dup (0)
        db      0FFh,0FFh,0FFh,0FFh,0C8h,00h,3Fh,01h,0C7h,00h,0D4h,00h,3Fh,01h,8Fh,01h
        db      0ACh,01h,00h,00h,00h,00h,83h,2Dh,97h,2Ch,99h,2Ch,98h,2Ch,98h,2Ch
        db      98h,2Ch,18h,2Fh,95h,2Ch,0F3h,2Dh,96h,2Fh,96h,2Ch,0E6h,31h,0E6h,31h
        db      0E6h,31h,0E6h,31h,0E6h,31h,0E6h,31h,0E6h,31h,0E6h,31h,0E6h,31h,0E6h,31h
        db      0E6h,31h,00h,00h,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0FFh,0FFh,0FFh,0FFh,0Fh,0Eh,0Ch,08h,00h,01h,03h,07h,0Fh,00h,01h,00h
        db      02h,00h,0Fh,0Fh
        db      96Ch dup (0)
        db      50h
        db      196h dup (0)
        db      0A0h
        db      5ACh dup (0)
        db      3Fh,01h,0C7h,00h,00h,01h,0Dh,00h,00h,00h,0C7h,00h,3Fh,01h,00h,00h
        db      00h,0A0h
        db      314h dup (0)
        db      0C9h,52h,0FFh,0FFh
        db      9 dup (0)
        db      2Ah,00h,1Ch,00h,00h,2Ah,2Ah,2Ah,00h,00h,2Ah,00h,2Ah,2Ah,15h,00h
        db      2Ah,2Ah,2Ah,15h,15h,15h,15h,15h,3Fh,15h,3Fh,15h,15h,3Fh,3Fh,3Fh
        db      15h,15h,3Fh,15h,3Fh,3Fh,3Fh,15h,3Fh,3Fh,3Fh,00h,00h,00h,00h,00h
        db      00h,0FFh,0FFh
        db      338h dup (0)
        db      0D2h,35h,0D5h,35h,0DEh,35h,05h,36h,0D2h,35h,0E8h,35h,0FBh,35h,05h,36h
        db      0D2h,35h,0D5h,35h,0DEh,35h,05h,36h,00h,00h,00h,00h,05h,37h,1Eh,37h
        db      23h,37h,23h,37h,3Ch,37h,3Ch,37h,3Ch,37h,3Ch,37h,41h,37h,41h,37h
        db      41h,37h,41h,37h,41h,37h,41h,37h,41h,37h,41h,37h,00h,00h,00h,00h
        db      00h,00h,00h,00h,0C7h,00h,00h,00h,00h,00h,0C6h,00h,00h,00h,00h,00h
        db      0C5h,00h,00h,00h,00h,00h,0C4h,00h,00h,00h,00h,00h,0C3h,00h,00h,00h
        db      00h,00h,0C2h,00h,00h,00h,00h,00h,0C1h,00h,00h,00h,00h,00h,0C0h,00h
        db      00h,00h,00h,00h,0BFh,00h,00h,00h,00h,00h,0BEh,00h,00h,00h,00h,00h
        db      0BDh,00h,00h,00h,00h,00h,0BCh,00h,00h,00h,00h,00h,0BBh,00h,00h,00h
        db      00h,00h,0BAh,00h,00h,00h,00h,00h,0B9h,00h,00h,00h,00h,00h,0B8h,00h
        db      00h,00h,00h,00h,0B7h,00h,00h,00h,00h,00h,0B6h,00h,00h,00h,00h,00h
        db      0B5h,00h,00h,00h,00h,00h,0B4h,00h,00h,00h,00h,00h,0B3h,00h,00h,00h
        db      00h,00h,0B2h,00h,00h,00h,00h,00h,0B1h,00h,00h,00h,00h,00h,0B0h,00h
        db      00h,00h,00h,00h,0AFh,00h,00h,00h,00h,00h,0AEh,00h,00h,00h,00h,00h
        db      0ADh,00h,00h,00h,00h,00h,0ACh,00h,00h,00h,00h,00h,0ABh,00h,00h,00h
        db      00h,00h,0AAh,00h,00h,00h,00h,00h,0A9h,00h,00h,00h,00h,00h,0A8h,00h
        db      00h,00h,00h,00h,0A7h,00h,00h,00h,00h,00h,0A6h,00h,00h,00h,00h,00h
        db      0A5h,00h,00h,00h,00h,00h,0A4h,00h,00h,00h,00h,00h,0A3h,00h,00h,00h
        db      00h,00h,0A2h,00h,00h,00h,00h,00h,0A1h,00h,00h,00h,00h,00h,0A0h,00h
        db      00h,00h,00h,00h,9Fh,00h,00h,00h,00h,00h,9Eh,00h,00h,00h,00h,00h
        db      9Dh,00h,00h,00h,00h,00h,9Ch,00h,00h,00h,00h,00h,9Bh,00h,00h,00h
        db      00h,00h,9Ah,00h,00h,00h,00h,00h,99h,00h,00h,00h,00h,00h,98h,00h
        db      00h,00h,00h,00h,97h,00h,00h,00h,00h,00h,96h,00h,00h,00h,00h,00h
        db      95h,00h,00h,00h,00h,00h,94h,00h,00h,00h,00h,00h,93h,00h,00h,00h
        db      00h,00h,92h,00h,00h,00h,00h,00h,91h,00h,00h,00h,00h,00h,90h,00h
        db      00h,00h,00h,00h,8Fh,00h,00h,00h,00h,00h,8Eh,00h,00h,00h,00h,00h
        db      8Dh,00h,00h,00h,00h,00h,8Ch,00h,00h,00h,00h,00h,8Bh,00h,00h,00h
        db      00h,00h,8Ah,00h,00h,00h,00h,00h,89h,00h,00h,00h,00h,00h,88h,00h
        db      00h,00h,00h,00h,87h,00h,00h,00h,00h,00h,86h,00h,00h,00h,00h,00h
        db      85h,00h,00h,00h,00h,00h,84h,00h,00h,00h,00h,00h,83h,00h,00h,00h
        db      00h,00h,82h,00h,00h,00h,00h,00h,81h,00h,00h,00h,00h,00h,80h,00h
        db      00h,00h,00h,00h,7Fh,00h,00h,00h,00h,00h,7Eh,00h,00h,00h,00h,00h
        db      7Dh,00h,00h,00h,00h,00h,7Ch,00h,00h,00h,00h,00h,7Bh,00h,00h,00h
        db      00h,00h,7Ah,00h,00h,00h,00h,00h,79h,00h,00h,00h,00h,00h,78h,00h
        db      00h,00h,00h,00h,77h,00h,00h,00h,00h,00h,76h,00h,00h,00h,00h,00h
        db      75h,00h,00h,00h,00h,00h,74h,00h,00h,00h,00h,00h,73h,00h,00h,00h
        db      00h,00h,72h,00h,00h,00h,00h,00h,71h,00h,00h,00h,00h,00h,70h,00h
        db      00h,00h,00h,00h,6Fh,00h,00h,00h,00h,00h,6Eh,00h,00h,00h,00h,00h
        db      6Dh,00h,00h,00h,00h,00h,6Ch,00h,00h,00h,00h,00h,6Bh,00h,00h,00h
        db      00h,00h,6Ah,00h,00h,00h,00h,00h,69h,00h,00h,00h,00h,00h,68h,00h
        db      00h,00h,00h,00h,67h,00h,00h,00h,00h,00h,66h,00h,00h,00h,00h,00h
        db      65h,00h,00h,00h,00h,00h,64h,00h,00h,00h,00h,00h,63h,00h,00h,00h
        db      00h,00h,62h,00h,00h,00h,00h,00h,61h,00h,00h,00h,00h,00h,60h,00h
        db      00h,00h,00h,00h,5Fh,00h,00h,00h,00h,00h,5Eh,00h,00h,00h,00h,00h
        db      5Dh,00h,00h,00h,00h,00h,5Ch,00h,00h,00h,00h,00h,5Bh,00h,00h,00h
        db      00h,00h,5Ah,00h,00h,00h,00h,00h,59h,00h,00h,00h,00h,00h,58h,00h
        db      00h,00h,00h,00h,57h,00h,00h,00h,00h,00h,56h,00h,00h,00h,00h,00h
        db      55h,00h,00h,00h,00h,00h,54h,00h,00h,00h,00h,00h,53h,00h,00h,00h
        db      00h,00h,52h,00h,00h,00h,00h,00h,51h,00h,00h,00h,00h,00h,50h,00h
        db      00h,00h,00h,00h,4Fh,00h,00h,00h,00h,00h,4Eh,00h,00h,00h,00h,00h
        db      4Dh,00h,00h,00h,00h,00h,4Ch,00h,00h,00h,00h,00h,4Bh,00h,00h,00h
        db      00h,00h,4Ah,00h,00h,00h,00h,00h,49h,00h,00h,00h,00h,00h,48h,00h
        db      00h,00h,00h,00h,47h,00h,00h,00h,00h,00h,46h,00h,00h,00h,00h,00h
        db      45h,00h,00h,00h,00h,00h,44h,00h,00h,00h,00h,00h,43h,00h,00h,00h
        db      00h,00h,42h,00h,00h,00h,00h,00h,41h,00h,00h,00h,00h,00h,40h,00h
        db      00h,00h,00h,00h,3Fh,00h,00h,00h,00h,00h,3Eh,00h,00h,00h,00h,00h
        db      3Dh,00h,00h,00h,00h,00h,3Ch,00h,00h,00h,00h,00h,3Bh,00h,00h,00h
        db      00h,00h,3Ah,00h,00h,00h,00h,00h,39h,00h,00h,00h,00h,00h,38h,00h
        db      00h,00h,00h,00h,37h,00h,00h,00h,00h,00h,36h,00h,00h,00h,00h,00h
        db      35h,00h,00h,00h,00h,00h,34h,00h,00h,00h,00h,00h,33h,00h,00h,00h
        db      00h,00h,32h,00h,00h,00h,00h,00h,31h,00h,00h,00h,00h,00h,30h,00h
        db      00h,00h,00h,00h,2Fh,00h,00h,00h,00h,00h,2Eh,00h,00h,00h,00h,00h
        db      2Dh,00h,00h,00h,00h,00h,2Ch,00h,00h,00h,00h,00h,2Bh,00h,00h,00h
        db      00h,00h,2Ah,00h,00h,00h,00h,00h,29h,00h,00h,00h,00h,00h,28h,00h
        db      00h,00h,00h,00h,27h,00h,00h,00h,00h,00h,26h,00h,00h,00h,00h,00h
        db      25h,00h,00h,00h,00h,00h,24h,00h,00h,00h,00h,00h,23h,00h,00h,00h
        db      00h,00h,22h,00h,00h,00h,00h,00h,21h,00h,00h,00h,00h,00h,20h,00h
        db      00h,00h,00h,00h,1Fh,00h,00h,00h,00h,00h,1Eh,00h,00h,00h,00h,00h
        db      1Dh,00h,00h,00h,00h,00h,1Ch,00h,00h,00h,00h,00h,1Bh,00h,00h,00h
        db      00h,00h,1Ah,00h,00h,00h,00h,00h,19h,00h,00h,00h,00h,00h,18h,00h
        db      00h,00h,00h,00h,17h,00h,00h,00h,00h,00h,16h,00h,00h,00h,00h,00h
        db      15h,00h,00h,00h,00h,00h,14h,00h,00h,00h,00h,00h,13h,00h,00h,00h
        db      00h,00h,12h,00h,00h,00h,00h,00h,11h,00h,00h,00h,00h,00h,10h,00h
        db      00h,00h,00h,00h,0Fh,00h,00h,00h,00h,00h,0Eh,00h,00h,00h,00h,00h
        db      0Dh,00h,00h,00h,00h,00h,0Ch,00h,00h,00h,00h,00h,0Bh,00h,00h,00h
        db      00h,00h,0Ah,00h,00h,00h,00h,00h,09h,00h,00h,00h,00h,00h,08h,00h
        db      00h,00h,00h,00h,07h,00h,00h,00h,00h,00h,06h,00h,00h,00h,00h,00h
        db      05h,00h,00h,00h,00h,00h,04h,00h,00h,00h,00h,00h,03h,00h,00h,00h
        db      00h,00h,02h,00h,00h,00h,00h,00h,01h,00h,00h,00h,00h,00h,00h,00h
        db      00h,00h,00h,00h,0FFh,0FFh,00h,00h,00h,00h,0FEh,0FFh,00h,00h,00h,00h
        db      0BDh,37h,0B2h,38h,0A8h,38h,9Eh,38h,94h,38h,8Ah,38h,80h,38h,76h,38h
        db      6Ch,38h,62h,38h,58h,38h,4Eh,38h,44h,38h,3Ah,38h,30h,38h,26h,38h
        db      1Ch,38h,0AEh,49h
        db      16h dup (0)
        db      0BAh,39h,0FAh,3Ah,0F2h,3Ah,0EAh,3Ah,0E2h,3Ah,0DAh,3Ah,0D2h,3Ah,0CAh,3Ah
        db      0C2h,3Ah,0BAh,3Ah,0B2h,3Ah,0AAh,3Ah,0A2h,3Ah,9Ah,3Ah,92h,3Ah,8Ah,3Ah
        db      82h,3Ah,7Ah,3Ah,72h,3Ah,6Ah,3Ah,62h,3Ah,5Ah,3Ah,52h,3Ah,4Ah,3Ah
        db      42h,3Ah,3Ah,3Ah,32h,3Ah,2Ah,3Ah,22h,3Ah,1Ah,3Ah,12h,3Ah,0Ah,3Ah
        db      02h,3Ah
        db      11h dup (0)
        db      08h,04h,0Ch,02h,0Ah,06h,0Eh,01h,09h,05h,0Dh,03h,0Bh,07h,0Fh,00h
        db      1FFh dup (0)
        db      21h,42h,1Eh,42h,1Bh,42h,18h,42h,15h,42h,12h,42h,0Fh,42h,0Ch,42h
        db      09h,42h,06h,42h,03h,42h,00h,42h,0FDh,41h,0FAh,41h,0F7h,41h,0F4h,41h
        db      0F1h,41h,0EEh,41h,0EBh,41h,0E8h,41h,0E5h,41h,0E2h,41h,0DFh,41h,0DCh,41h
        db      0D9h,41h,0D6h,41h,0D3h,41h,0D0h,41h,0CDh,41h,0CAh,41h,0C7h,41h,0C4h,41h
        db      0C1h,41h,0BEh,41h,0BBh,41h,0B8h,41h,0B5h,41h,0B2h,41h,0AFh,41h,0ACh,41h
        db      0A9h,41h,0A6h,41h,0A3h,41h,0A0h,41h,9Dh,41h,9Ah,41h,97h,41h,94h,41h
        db      91h,41h,8Eh,41h,8Bh,41h,88h,41h,85h,41h,06h,41h,0F1h,40h,0DCh,40h
        db      0C7h,40h,0B2h,40h,9Dh,40h,88h,40h,73h,40h,5Eh,40h,49h,40h,34h,40h
        db      1Fh,40h,0Ah,40h,0F5h,3Fh,0E0h,3Fh,0CBh,3Fh,0B6h,3Fh,0A1h,3Fh,8Ch,3Fh
        db      77h,3Fh,62h,3Fh,4Dh,3Fh,38h,3Fh,23h,3Fh,0Eh,3Fh,0F9h,3Eh,0E4h,3Eh
        db      0CFh,3Eh,0BAh,3Eh,0A5h,3Eh,90h,3Eh,7Bh,3Eh,66h,3Eh,51h,3Eh,3Ch,3Eh
        db      27h,3Eh,12h,3Eh,0FDh,3Dh,0E8h,3Dh,0D3h,3Dh,0BEh,3Dh,0A9h,3Dh,94h,3Dh
        db      7Fh,3Dh,6Ah,3Dh,55h,3Dh,40h,3Dh,2Bh,3Dh,16h,3Dh,01h,3Dh,0ECh,3Ch
        db      0D7h,3Ch,0C2h,3Ch
        db      0Eh dup (0)
        db      01h,00h,08h,00h,08h,00h,08h,00h,01h,00h,08h,00h,25h,42h,61h,3Ch
        db      870h dup (0)
        db      48h,55h
FD51    ends

SEG003_TEXT segment word public 'CODE'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        public  _ShowClip
        public  _Transparency
        public  _cXfer
        public  _seg003_0272_1373
        public  _seg003_0272_D1E

; seg003_0272_D1E  (+D1E)
_seg003_0272_D1E proc far
        push    es
        push    ds
        mov     ax,seg seg_370D
        mov     ds,ax
        mov     bx,ss
        mov     word ptr cs:L1550,bx
        mov     word ptr cs:L1552,sp
        cli
        mov     ss,ax
        mov     sp,word ptr ds:[5588h]
        sti
        mov     cx,word ptr ds:[0B26h]
        cmp     cx,2
        jg      L0D45
        jmp     L0E64
L0D45:
        cmp     word ptr ds:[0B24h],2
        jg      L0D4F
        jmp     L0E64
L0D4F:
        mov     ax,word ptr ds:[0B1Ch]
        dec     ax
        xor     dx,dx
        mov     word ptr ds:[0B3Eh],dx
        mov     dl,ah
        mov     ah,al
        xor     al,al
        dec     cx
        div     cx
        inc     cx
        or      ax,ax
        jne     L0D6A
        jmp     L0E64
L0D6A:
        mov     word ptr ds:[0B38h],ax
        mov     ax,cs
        mov     es,ax
        mov     ax,word ptr ds:[0B20h]
        add     ax,cx
        dec     ax
        sub     ax,word ptr ds:[3DF8h]
        dec     ax
        jle     L0D89
        sub     cx,ax
        jg      L0D85
        jmp     L0E64
L0D85:
        mov     word ptr ds:[0B26h],cx
L0D89:
        mov     ax,word ptr ds:[0B20h]
        mov     si,word ptr ds:[3DF4h]
        sub     ax,si
        jge     L0DAB
        neg     ax
        jg      L0D9B
        jmp     L0E64
L0D9B:
        mov     word ptr ds:[0B3Eh],ax
        mov     word ptr ds:[0B20h],si
        sub     word ptr ds:[0B26h],ax
        jg      L0DAB
        jmp     L0E64
L0DAB:
        cmp     word ptr ds:[0B26h],200h
        jl      L0DB6
        jmp     L0E64
L0DB6:
        mov     di,offset L1554
        call    word ptr ds:[0B0Eh]
        xor     ax,ax
        xor     dx,dx
        mov     ah,byte ptr ds:[0B1Eh]
        mov     dl,byte ptr ds:[0B1Fh]
        div     word ptr ds:[0B24h]
        mov     word ptr ds:[0B3Ah],ax
        xor     dx,dx
        mov     cx,word ptr ds:[0B24h]
        mov     bp,word ptr ds:[0B22h]
        cmp     bp,word ptr ds:[3DF6h]
        jle     L0DF4
        sub     bp,word ptr ds:[3DF6h]
        sub     cx,bp
        jle     L0E64
        mov     ax,bp
        mul     word ptr ds:[0B3Ah]
        mov     dx,ax
        mov     bp,word ptr ds:[3DF6h]
L0DF4:
        mov     bx,bp
        sub     bx,cx
        inc     bx
        cmp     bx,word ptr ds:[3DFAh]
        jge     L0E07
        sub     bx,word ptr ds:[3DFAh]
        add     cx,bx
        jle     L0E64
L0E07:
        shl     bp,1
        add     bp,95Ch
        mov     bx,0B28h
        mov     word ptr ds:[0B3Ch],cx
        mov     cl,0FFh
L0E16:
        cmp     cl,dh
        je      L0E40
        mov     al,dh
        mul     byte ptr ds:[0DC0h]
        mov     si,word ptr ds:[0B18h]
        add     si,word ptr ds:[0B3Eh]
        mov     es,word ptr ds:[0B0Ch]
        mov     ds,word ptr ds:[0B1Ah]
        add     si,ax
        mov     di,0B40h
        mov     cx,0F04h
        call    near ptr L1554
        mov     ds,word ptr ss:[0B0Ch]
L0E40:
        mov     es,word ptr ds:[958h]
        mov     di,word ptr [bp]
        dec     bp
        dec     bp
        add     di,word ptr ds:[0B20h]
        mov     si,0B40h
        mov     cx,word ptr ds:[0B26h]
        call    word ptr ds:[0B10h]
        mov     cl,dh
        add     dx,word ptr ds:[0B3Ah]
        dec     word ptr ds:[0B3Ch]
        jne     L0E16
L0E64:
        cli
        mov     ss,word ptr cs:L1550
        mov     sp,word ptr cs:L1552
        sti
        pop     ds
        pop     es
        retf
_seg003_0272_D1E endp

; XFER.DAT, five 256-byte colour translation tables. OVR152 loads the file over these
; bytes at startup (lget("xfer.dat", ..., 500h)); FM Towns calls them _cXfer.
_cXfer label byte
; table 0
        db      25h,24h,1Eh,2Fh,20h,20h,21h,22h,2Eh,2Fh,2Fh,23h,23h,1Eh,0B7h,0B7h
        db      2Fh,2Fh,0B9h,0B9h,0B9h,0BAh,0BAh,0BAh,0BBh,23h,1Eh,1Fh,1Fh,20h,20h,20h
        db      21h,21h,22h,22h,23h,23h,24h,1Eh,1Fh,1Fh,1Fh,1Fh,20h,2Fh,30h,0BAh
        db      0BAh,0BAh,0BAh,0BBh,23h,23h,0B7h,2Fh,2Fh,2Fh,30h,0B9h,0BAh,0BAh,0BBh,0BEh
        db      0B7h,7Ch,7Ch,7Dh,7Eh,7Eh,7Eh,0BBh,0BCh,1Eh,1Fh,1Fh,5Bh,5Bh,5Ch,5Fh
        db      5Fh,60h,60h,60h,60h,24h,24h,1Fh,1Fh,20h,20h,21h,22h,22h,22h,23h
        db      23h,23h,24h,1Fh,1Fh,20h,20h,7Eh,21h,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh
        db      20h,20h,7Eh,7Eh,22h,22h,23h,23h,1Fh,20h,20h,20h,21h,21h,22h,0B8h
        db      0B9h,0B9h,0BAh,0BAh,0BBh,0BCh,23h,23h,1Fh,1Fh,1Fh,1Fh,20h,7Dh,7Eh,7Eh
        db      0BAh,0BAh,22h,22h,23h,23h,1Eh,1Fh,1Fh,1Fh,1Fh,1Fh,20h,21h,21h,21h
        db      22h,22h,23h,23h,0B7h,0B7h,7Bh,7Ch,7Ch,7Ch,7Dh,0B9h,7Eh,7Eh,7Eh,0BBh
        db      0BCh,0BCh,0BCh,1Fh,1Fh,1Fh,20h,20h,21h,0BAh,22h,22h,22h,22h,23h,23h
        db      23h,1Eh,1Eh,1Fh,1Fh,1Fh,1Fh,1Fh,1Fh,21h,21h,21h,21h,22h,23h,23h
        db      24h,1Eh,1Fh,1Fh,1Fh,7Ch,7Ch,7Dh,7Dh,7Eh,21h,7Eh,7Eh,0BCh,23h,23h
        db      23h,23h,23h,5Ch,23h,23h,23h,5Ch,23h,23h,23h,23h,23h,23h,23h,23h
        db      25h,25h,24h,24h,0BEh,24h,7Eh,7Ch,24h,22h,7Ch,0DAh,21h,22h,0F6h,7Eh
; table 1
        db      0F4h,0F4h,0A8h,81h,82h,0F6h,90h,91h,80h,81h,82h,0B1h,0B1h,0A8h,0A9h,80h
        db      81h,81h,82h,82h,82h,83h,0AFh,84h,0B0h,0B1h,0A8h,43h,43h,44h,72h,73h
        db      90h,91h,92h,93h,34h,3Fh,48h,0A9h,43h,43h,43h,81h,81h,3Ch,3Ch,3Ch
        db      83h,3Dh,84h,84h,0B1h,0B2h,0A9h,80h,81h,81h,82h,82h,0ADh,0AEh,0B0h,0B1h
        db      0AAh,0AAh,0ABh,0ABh,0ACh,0ADh,0AEh,0B0h,0B1h,0A9h,0AAh,0AAh,44h,9Fh,0A0h,0Ch
        db      0Ch,0Ch,0Ch,0Bh,0A3h,0A3h,0F4h,0A9h,43h,44h,0CBh,0CCh,0DCh,0DEh,0DEh,0DEh
        db      0DEh,0CFh,48h,43h,44h,44h,45h,0ADh,46h,42h,43h,43h,43h,43h,44h,44h
        db      44h,45h,0ADh,0AEh,0AFh,0B0h,0B1h,0B2h,44h,44h,45h,45h,45h,46h,84h,0ABh
        db      0ABh,0ACh,0ACh,0AEh,0AFh,0B0h,0B2h,48h,43h,43h,43h,44h,44h,0ACh,0ADh,0AEh
        db      0AFh,0AFh,0B0h,0B0h,0B1h,0B1h,0A8h,0A9h,0AAh,0AAh,44h,44h,0ADh,0ADh,0ADh,0AEh
        db      0AEh,0B0h,0B1h,0B1h,0A9h,0A9h,0AAh,0ABh,0ABh,0ABh,0ABh,0ACh,0ADh,0AEh,0AEh,0AFh
        db      0B0h,0B0h,0B0h,0A9h,43h,44h,44h,45h,45h,3Dh,84h,84h,3Eh,3Eh,3Eh,0B1h
        db      0B2h,0A8h,0A9h,0A9h,0AAh,0AAh,0AAh,44h,0ACh,0ADh,0ADh,0AEh,0AEh,0B0h,0B0h,0B1h
        db      0B2h,0A9h,0A9h,0AAh,0AAh,44h,44h,0ACh,0ADh,0ADh,0AEh,0AFh,0AFh,0B0h,0B1h,0B1h
        db      0B0h,0B1h,0B1h,0B0h,0B1h,0B1h,0B1h,0B0h,0B1h,0B1h,0B1h,0B0h,0B1h,0B1h,0B1h,0B1h
        db      0F4h,0F4h,0F4h,0F4h,0B2h,0B2h,0AEh,0ABh,0F4h,0DCh,0AAh,0ABh,45h,47h,0ABh,0AEh
; table 2
        db      0Bh,0Bh,4Ch,0D8h,0D9h,5Bh,5Bh,5Ch,0D7h,0D8h,0D9h,0Ch,0Ch,4Ch,9Bh,9Ch
        db      9Ch,9Dh,9Eh,9Fh,9Fh,0A0h,0A0h,0A1h,0A1h,0A2h,4Ch,4Ch,4Dh,5Ah,5Bh,5Bh
        db      5Ch,5Ch,5Ch,5Ch,60h,0Bh,0Bh,4Ch,4Ch,9Ch,9Ch,9Dh,0D8h,72h,0DAh,0DAh
        db      0DBh,0DBh,0A1h,0A1h,0A2h,0Bh,9Bh,9Ch,44h,44h,0CAh,0CBh,0CCh,0A0h,0A1h,0Ch
        db      4Dh,9Dh,9Eh,9Fh,9Fh,0A0h,0A0h,0A1h,0Ch,4Ch,4Dh,4Dh,4Eh,4Eh,4Eh,4Fh
        db      4Fh,50h,50h,0Ch,0Ch,0Ch,0Bh,4Ch,4Dh,4Dh,4Eh,4Eh,5Ch,5Ch,5Ch,5Fh
        db      0Ch,0Bh,0Bh,4Dh,9Dh,9Eh,9Fh,0A0h,0A0h,4Ch,4Dh,4Dh,4Dh,4Dh,4Dh,9Dh
        db      9Eh,9Fh,0A0h,0A0h,0A0h,0A1h,0Ch,0Ch,9Dh,9Eh,9Fh,9Fh,0A0h,0A0h,0A1h,9Dh
        db      9Eh,9Fh,0A0h,0A0h,0A1h,0A1h,0Bh,0Bh,4Ch,9Ch,9Dh,9Dh,9Eh,9Fh,9Fh,0A0h
        db      0A0h,0A1h,0A1h,0A1h,0A1h,0Ch,4Ch,4Dh,4Dh,4Dh,4Dh,4Eh,4Eh,4Eh,4Eh,4Eh
        db      4Eh,0A1h,0Ch,0Ch,4Ch,9Ch,9Ch,9Dh,9Dh,9Eh,9Fh,0A0h,0A0h,0A0h,0A0h,0A1h
        db      0A1h,0Ch,0Ch,4Ch,4Dh,9Dh,9Eh,9Fh,0A0h,0A0h,0DCh,0A1h,0A1h,0A1h,0A1h,0A2h
        db      0A2h,4Ch,4Ch,4Dh,4Dh,4Dh,4Dh,4Dh,4Eh,4Eh,4Eh,0A0h,0A0h,0A1h,0A1h,0Ch
        db      0Ch,4Ch,4Ch,4Dh,4Dh,4Dh,4Dh,9Fh,9Fh,0A0h,0A0h,0A0h,0A1h,0A1h,0Ch,0Ch
        db      0Ch,0Ch,0Ch,4Eh,0Ch,0Ch,0Ch,0Ch,0Ch,0Ch,0Ch,0Ch,0Ch,0Ch,0Ch,0Ch
        db      0Bh,0Bh,0Bh,0Bh,0Ch,0Ch,0A0h,9Dh,0Bh,0A0h,4Dh,4Eh,4Eh,0A1h,0ADh,0A0h
; table 3  (_seg003_0272_788 maps the screen through it)
        db      0DFh,0A3h,2h,10h,2Ch,0B7h,7Ch,20h,37h,10h,2Bh,0A1h,0A0h,0Dh,0Eh,0Eh
        db      0Fh,10h,0F7h,0F7h,13h,8Dh,8Eh,8Fh,90h,91h,1Ah,1Bh,1Bh,1Ch,1Dh,1Eh
        db      1Fh,1Fh,20h,21h,23h,23h,0DEh,27h,69h,28h,29h,2Ah,2Bh,2Ch,2Ch,13h
        db      14h,14h,8Fh,90h,92h,93h,36h,36h,37h,39h,80h,80h,81h,82h,46h,47h
        db      40h,40h,41h,42h,43h,44h,44h,45h,47h,49h,4Ah,4Ah,4Bh,4Ch,4Dh,4Fh
        db      0Ch,0Ch,0Ch,0Ch,0Ch,0Bh,0Bh,57h,58h,58h,59h,5Ah,5Bh,5Ch,5Ch,5Ch
        db      5Ch,0DEh,0DEh,6Bh,6Dh,64h,65h,66h,66h,0B3h,69h,0D2h,6Bh,6Ch,63h,6Dh
        db      64h,65h,0D7h,0D8h,0D9h,0DBh,0DCh,0DDh,6Dh,78h,79h,65h,7Ah,7Bh,67h,7Fh
        db      7Fh,80h,81h,82h,0F6h,91h,93h,76h,69h,0B4h,89h,8Ah,8Bh,8Ch,8Ch,8Dh
        db      8Eh,73h,73h,0DBh,0DCh,0DCh,96h,97h,97h,98h,99h,9Ah,9Ah,9Bh,9Ch,9Dh
        db      9Eh,9Fh,0A0h,0A1h,0A4h,0A4h,0A5h,0A6h,0A7h,0A8h,0A8h,0AAh,44h,44h,44h,45h
        db      45h,46h,46h,0B3h,0D2h,6Ch,6Fh,0B6h,7Ah,8Eh,8Fh,0B9h,90h,90h,91h,92h
        db      93h,0C1h,0C2h,0C3h,0C3h,0C4h,0C5h,0C6h,0C7h,0C7h,0C8h,0C9h,0CAh,0CBh,0CCh,0CDh
        db      0CEh,0D1h,0B3h,0D2h,0D3h,0D4h,0D5h,0D6h,0D7h,0D7h,0D8h,0D9h,0DAh,0DBh,0DCh,0DDh
        db      0A0h,0A1h,0A0h,9Fh,0A0h,0A1h,0A0h,0A0h,0A1h,0A1h,0A1h,0A0h,0A1h,0A1h,0A1h,0A0h
        db      0DFh,0DFh,0CFh,0DFh,0B1h,0A2h,8Eh,0F7h,95h,7Ch,0A6h,4Ch,9Bh,5Bh,0A9h,0D7h
; table 4
        db      1h,1h,0C8h,16h,90h,0BBh,0BCh,24h,82h,15h,16h,0F5h,0Bh,0D8h,0D8h,0F6h
        db      0F6h,0F6h,90h,91h,92h,93h,34h,34h,0C0h,35h,0C9h,0D9h,0D9h,73h,7Eh,7Eh
        db      23h,23h,24h,25h,25h,26h,0F8h,0D8h,0D8h,72h,67h,68h,90h,32h,32h,17h
        db      18h,18h,34h,0C0h,35h,0F8h,44h,82h,3Ch,83h,3Dh,84h,3Eh,85h,3Fh,0F4h
        db      0CAh,45h,46h,46h,47h,47h,0B2h,48h,0F4h,9Dh,9Eh,9Fh,0A0h,0A1h,0Ch,53h
        db      53h,54h,54h,55h,55h,56h,1h,9Dh,0DAh,0DBh,74h,0DEh,60h,60h,61h,61h
        db      62h,62h,0F3h,0DAh,0DBh,74h,0DCh,75h,93h,0D8h,0D9h,0D9h,0DAh,0DAh,0DAh,0DBh
        db      74h,0DCh,75h,76h,76h,95h,77h,0D0h,73h,74h,74h,92h,75h,93h,19h,45h
        db      46h,84h,3Eh,85h,3Fh,3Fh,87h,0F8h,0D8h,0D9h,73h,73h,74h,92h,92h,93h
        db      94h,95h,95h,95h,77h,77h,0C9h,9Eh,9Eh,9Fh,0A0h,0A0h,0A1h,0A1h,0A1h,0CEh
        db      0A2h,0A3h,0A3h,0F5h,44h,44h,45h,45h,46h,46h,0AFh,0B0h,0B0h,0B1h,0B1h,0B2h
        db      48h,0F4h,0F4h,0D8h,0D9h,73h,74h,74h,92h,93h,19h,0BFh,0BFh,0C0h,86h,35h
        db      87h,0C9h,0C9h,0CAh,0CAh,0CBh,0CCh,0CCh,0CDh,0CDh,0DDh,0CEh,0CEh,0CFh,0DFh,0D0h
        db      0F2h,0D8h,0D9h,0D9h,0DAh,0DBh,0DBh,0DCh,0DDh,0DDh,0CEh,76h,0DEh,0CFh,77h,0D0h
        db      0Bh,0Bh,0Bh,0Ch,0Bh,0A3h,0Bh,0Bh,0Bh,0F5h,0A3h,0Bh,0Bh,0F5h,0F5h,0Bh
        db      1h,1h,0F2h,0F3h,0F2h,1h,93h,0F6h,0F8h,0DEh,0ADh,0Ch,0A1h,0DEh,0AEh,0DDh

; seg003_0272_1373  (+1373): span routines and the scaler generators, reached through
; the pointers at DS:B0E and DS:B10
_seg003_0272_1373 proc near
        mov     bx,offset _cXfer+400h
L1376:
        lodsb
        or      al,al
        je      L1384
        mov     al,byte ptr es:[di]
        xlat    byte ptr cs:[bx]
L1380:
        stosb
        loop    L1376
        retn
L1384:
        inc     di
        loop    L1376
        retn
L1388:
        mov     bx,offset _cXfer+300h
L138B:
        lodsb
        or      al,al
        je      L1399
        mov     al,byte ptr es:[di]
        xlat    byte ptr cs:[bx]
        stosb
        loop    L138B
        retn
L1399:
        inc     di
        loop    L138B
        retn
L139D:
L139E equ $+1
        sub     al,0F9h
        mov     bx,offset _cXfer
        add     bh,al
        mov     al,byte ptr es:[di]
        xlat    byte ptr cs:[bx]
        jmp     short L13BA
        mov     bx,offset _cXfer
        lodsb
        dec     cx
        je      L13CC
        or      al,al
        je      L13C2
L13B6:
        cmp     al,0F9h
        jae     L139D
L13BA:
        stosb
        lodsb
        or      al,al
        loopne  L13B6
        jcxz    L13CE
L13C2:
        inc     di
        lodsb
        or      al,al
        loope   L13C2
        jcxz    L13CE
        jmp     L13B6
L13CC:
        or      al,al
L13CE:
        je      L13D5
        cmp     al,0F9h
        jae     L13D5
        stosb
L13D5:
        retn
        rep movsb
        retn
        lodsb
        dec     cx
        je      L13F3
        or      al,al
        je      L13E9
L13E1:
        stosb
        lodsb
        or      al,al
        loopne  L13E1
        jcxz    L13F5
L13E9:
        inc     di
        lodsb
        or      al,al
        loope   L13E9
        jcxz    L13F5
        jmp     L13E1
L13F3:
        or      al,al
L13F5:
        je      L13F8
        stosb
L13F8:
        retn
        mov     bl,byte ptr ds:[0DC1h]
        lodsb
        dec     cx
        je      L1419
        or      al,al
        je      L140F
L1405:
        mov     al,bl
        stosb
        lodsb
        or      al,al
        loopne  L1405
        jcxz    L141B
L140F:
        inc     di
        lodsb
        or      al,al
        loope   L140F
        jcxz    L141B
        jmp     L1405
L1419:
        or      al,al
L141B:
        je      L1420
        mov     al,bl
        stosb
L1420:
        retn
        xor     dx,dx
        xor     bp,bp
        mov     ax,word ptr ds:[0B1Ch]
        shr     ax,1
        mov     byte ptr ds:[0DC0h],al
        test    byte ptr ds:[0B39h],0FFh
        je      L146F
L1434:
        xor     bx,bx
        mov     bl,dh
        mov     si,0E820h
        shr     bx,1
        jb      L1442
        mov     si,0E8D2h
L1442:
        sub     bx,bp
        je      L1459
        add     bp,bx
        cmp     bl,0FFh
        jne     L1452
        mov     al,4Eh
        stosb
        jmp     short L1459
L1452:
        mov     ax,0C683h
        stosw
        mov     al,bl
        stosb
L1459:
        mov     al,0ACh
        stosb
        mov     ax,si
        stosw
        mov     al,36h
        stosb
        mov     ax,0AAD7h
        stosw
        inc     bp
        add     dx,word ptr ds:[0B38h]
        loop    L1434
        jmp     short L14A6
L146F:
        mov     bp,0FFF8h
L1472:
        xor     bx,bx
        mov     bl,dh
        cmp     bx,bp
        je      L149D
        mov     bp,bx
        test    bx,1
        je      L148C
        mov     ax,0E088h
        stosw
        mov     ax,0E820h
        stosw
        jmp     short L1497
L148C:
        mov     al,0ACh
        stosb
        mov     ax,0C488h
        stosw
        mov     ax,0E8D2h
        stosw
L1497:
        mov     al,36h
        stosb
        mov     al,0D7h
        stosb
L149D:
        mov     al,0AAh
        stosb
        add     dx,word ptr ds:[0B38h]
        loop    L1472
L14A6:
        mov     al,0C3h
        stosb
        retn
        mov     bp,word ptr ds:[0B26h]
        mov     ax,word ptr ds:[0B1Ch]
        mov     byte ptr ds:[0DC0h],al
        mov     ax,word ptr ds:[0B38h]
        test    word ptr ds:[0B3Eh],0FFFFh
        je      L14CF
        mov     bx,word ptr ds:[0B3Eh]
        imul    bx
        mov     al,ah
        mov     ah,dl
        mov     word ptr ds:[0B3Eh],ax
        mov     ax,word ptr ds:[0B38h]
L14CF:
        mov     bx,ax
        test    ah,0FFh
        jne     L152A
        cmp     ax,128h
        jle     L1511
        xor     bh,bh
        mov     al,51h
        stosb
        xor     si,si
L14E2:
        xor     dx,dx
L14E4:
        add     bh,bl
        inc     dx
        jae     L14E4
        cmp     dx,1
        jne     L14F1
        inc     si
        jmp     short L1502
L14F1:
        mov     al,0A4h
        mov     cx,si
        rep stosb
        xor     si,si
        mov     al,0ACh
        stosb
        mov     al,0AAh
        mov     cx,dx
        rep stosb
L1502:
        sub     bp,dx
        jg      L14E2
        mov     al,0A4h
        mov     cx,si
        rep stosb
        mov     al,59h
        stosb
        jmp     L14A6
L1511:
        xor     bh,bh
L1513:
        xor     dx,dx
L1515:
        inc     dx
        add     bh,bl
        jae     L1515
        mov     al,0ACh
        stosb
        mov     al,0AAh
        mov     cx,dx
        rep stosb
        sub     bp,dx
        jg      L1513
        jmp     L14A6
L152A:
        xor     dx,dx
L152C:
        mov     al,0A4h
        stosb
        add     dx,bx
        dec     dh
        je      L154A
        cmp     dh,1
        jg      L1541
        mov     al,46h
        dec     dh
        stosb
        jmp     short L154A
L1541:
        mov     ax,0C683h
        stosw
        mov     al,dh
        stosb
        xor     dh,dh
L154A:
        dec     bp
        jne     L152C
        jmp     L14A6
L1550   dw      0
L1552   dw      0
L1554   db      0C80h dup (0)           ; code the generators above write and _seg003_0272_D1E calls
_seg003_0272_1373 endp

SEG003_TEXT ends

        end
