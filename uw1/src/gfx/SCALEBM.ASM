; /* target: seg003_1082 */
; /* opts: /ml */
;
; Graphics library module 7: scaled bitmaps, FM Towns' scale_nibble_bitmap with its span writers
; (really_slow_xfer, transparent_pixel_xfer, color_pixel_xfer) and its code generators
; (byte_compiler, scale_up_by_2, scale_down and their inner and outer loops). It draws the 3D
; view's sprites: seg004's do_scalebm (TMAPOPS.ASM) fills in the sprite record at seg048:0B0C
; and far-calls _1082, which draws the bitmap stretched or shrunk to its screen size into the
; frame buffer (GRENTRY.ASM). Rather than step through the source pixel by pixel, _1082 has a
; generator write a straight-line scaler for the sprite's width (lodsb/stosb/movsb sequences,
; with xlat to unpack four-bit pixels) into the buffer L1F3A in this code segment, and calls it
; once per source row. It also holds the five colour tables of XFER.DAT (_cXfer); UW2's
; LIGHTING loads the file over them at start-up, and UW1 ships an XFER.DAT too.
;
; The sprite record (seg048, from TMAPOPS.ASM's do_scalebm and PGCACHE.ASM's decoders):
;   0B10 the generator to use, 0B12 the span writer; 0B1A, 0B1C the source's offset and
;   segment; 0B1E its width; 0B20, 0B21 its height; 0B22, 0B24 the screen x and the top
;   y (y counts up); 0B26, 0B28 the height and width on the screen
; Working data: 0B3A the source step per screen pixel (8.8), 0B3C per screen row; 0B3E rows
; left; 0B40 the pixels clipped off the left; 0B42 the unpacked row; 0DC2 the source's bytes
; per row; 0DC3 bmcolor (pick frames).
; Pixel colours from 0FBh up are not drawn as themselves by the translucent writer: each picks
; a cXfer table that remaps the pixel already on the screen, so a sprite can tint or darken
; what is behind it (probably ghosts and similar effects; not checked in the game).
;
; name: descriptive (UW2Decomp: module 4 of seg003, "XFER tables and scaled-bitmap spans"); the
; routine names given here are FM Towns'.
;
; UW1 against UW2: UW2Decomp's src/gfx/SCALEBM.ASM (UW2's seg003_0272_D1E) with two additions.
; _1082 ends, before its far return, with the colour block POLYFILL also has in UW1 (here
; with 60h for a colour of 0), and 658h zero bytes follow the tables (L1701), which nothing
; refers to. The tables hold other bytes: in the EXE UW1's table 0 and 1 are filled and 2 to 4
; are zero. The translucent writer takes colours from 0FBh up as table numbers (UW2 0F9h).
; The library's data sits 2 bytes higher (the sprite record at 0B0C, UW2 0B0A).
;
; SEG003 split into its original modules: this is module 7 of UW1's 18, +1082..+2BB9.
; Evidence for the bounds: GRENTRY ends at +1081 (see GRENTRY.ASM); its relocations (entries
; 2741-2742, +1085, +11DA) come right after its FD52 pair (2739-2740) and are separated from
; the next ones in this segment (2763, +321F) by seg004's. It ends at +2BBA, the start of
; VIDMODE: the code buffer L1F3A runs to there, as UW2's L1554 runs to +21D4.
; Its FD52 piece (3963:0B0C..5588) holds entries 2739-2740 (the far pointers at 0B0C and 0B1A),
; which come right before its code entries. It is taken to run from 0B0C to GRDISP's first
; pointer (5588), as in UW2; how the original divided those bytes between the modules is not
; known.

        .186

        extrn   _checkerboard1:far
        extrn   seg048:far

_DATA   segment word public 'DATA'
_DATA   ends
DGROUP  group   _DATA

FD52    segment byte public 'FAR_DATA'
        dd      seg048
        db      90h,1Eh,91h,1Dh,91h,1Dh,0DFh,1Dh,0BFh,1Dh   ; 1E90h, 1D91h, 1D91h, 1DDFh, 1DBFh: the generator and span writer offsets
        dd      _checkerboard1
        db      08h,00h,08h,00h,12h,00h,25h,01h,40h,00h,31h,00h,00h,01h,02h,03h
        db      04h,05h,06h,07h,08h,09h,10h,11h,12h,13h,14h,15h
        db      28Ah dup (0)
        db      6Ch,5Bh                 ; 5B6Ch: GRLIBL's _5B6C, the default transfer (0DC4)
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
        db      0CDh,0Dh
        db      18BBh dup (0)
        db      0FFh,0FFh,3Fh,01h,0C7h,00h,0D4h,00h,3Fh,01h,8Fh,01h,0ACh,01h,00h,00h
        db      00h,00h,04h,36h,18h,35h,1Ah,35h,19h,35h,19h,35h,19h,35h,99h,37h
        db      16h,35h,74h,36h,17h,38h,17h,35h,67h,3Ah,67h,3Ah,67h,3Ah,67h,3Ah
        db      67h,3Ah,67h,3Ah,67h,3Ah,67h,3Ah,67h,3Ah,67h,3Ah,67h,3Ah,00h,00h
        db      0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
        db      0Fh,0Eh,0Ch,08h,00h,01h,03h,07h,0Fh,00h,01h,00h,02h,00h,0Fh,0Fh
        db      96Ch dup (0)
        db      50h
        db      196h dup (0)
        db      0A0h
        db      5ACh dup (0)
        db      3Fh,01h,0C7h,00h,00h,01h,0Dh,00h,00h,00h,0C7h,00h,3Fh,01h,00h,00h
        db      00h,0A0h
        db      314h dup (0)
        db      0C3h,5Ah,0FFh,0FFh
        db      9 dup (0)
        db      2Ah,00h,1Ch,00h,00h,2Ah,2Ah,2Ah,00h,00h,2Ah,00h,2Ah,2Ah,15h,00h
        db      2Ah,2Ah,2Ah,15h,15h,15h,15h,15h,3Fh,15h,3Fh,15h,15h,3Fh,3Fh,3Fh
        db      15h,15h,3Fh,15h,3Fh,3Fh,3Fh,15h,3Fh,3Fh,3Fh,00h,00h,00h,00h,00h
        db      00h,0FFh,0FFh
        db      338h dup (0)
        db      52h,3Eh,55h,3Eh,5Eh,3Eh,85h,3Eh,52h,3Eh,68h,3Eh,7Bh,3Eh,85h,3Eh
        db      52h,3Eh,55h,3Eh,5Eh,3Eh,85h,3Eh,00h,00h,00h,00h,85h,3Fh,9Eh,3Fh
        db      0A3h,3Fh,0A3h,3Fh,0BCh,3Fh,0BCh,3Fh,0BCh,3Fh,0BCh,3Fh,0C1h,3Fh,0C1h,3Fh
        db      0C1h,3Fh,0C1h,3Fh,0C1h,3Fh,0C1h,3Fh,0C1h,3Fh,0C1h,3Fh
        db      8 dup (0)
        db      0C7h,00h,00h,00h,00h,00h,0C6h,00h,00h,00h,00h,00h,0C5h,00h,00h,00h
        db      00h,00h,0C4h,00h,00h,00h,00h,00h,0C3h,00h,00h,00h,00h,00h,0C2h,00h
        db      00h,00h,00h,00h,0C1h,00h,00h,00h,00h,00h,0C0h,00h,00h,00h,00h,00h
        db      0BFh,00h,00h,00h,00h,00h,0BEh,00h,00h,00h,00h,00h,0BDh,00h,00h,00h
        db      00h,00h,0BCh,00h,00h,00h,00h,00h,0BBh,00h,00h,00h,00h,00h,0BAh,00h
        db      00h,00h,00h,00h,0B9h,00h,00h,00h,00h,00h,0B8h,00h,00h,00h,00h,00h
        db      0B7h,00h,00h,00h,00h,00h,0B6h,00h,00h,00h,00h,00h,0B5h,00h,00h,00h
        db      00h,00h,0B4h,00h,00h,00h,00h,00h,0B3h,00h,00h,00h,00h,00h,0B2h,00h
        db      00h,00h,00h,00h,0B1h,00h,00h,00h,00h,00h,0B0h,00h,00h,00h,00h,00h
        db      0AFh,00h,00h,00h,00h,00h,0AEh,00h,00h,00h,00h,00h,0ADh,00h,00h,00h
        db      00h,00h,0ACh,00h,00h,00h,00h,00h,0ABh,00h,00h,00h,00h,00h,0AAh,00h
        db      00h,00h,00h,00h,0A9h,00h,00h,00h,00h,00h,0A8h,00h,00h,00h,00h,00h
        db      0A7h,00h,00h,00h,00h,00h,0A6h,00h,00h,00h,00h,00h,0A5h,00h,00h,00h
        db      00h,00h,0A4h,00h,00h,00h,00h,00h,0A3h,00h,00h,00h,00h,00h,0A2h,00h
        db      00h,00h,00h,00h,0A1h,00h,00h,00h,00h,00h,0A0h,00h,00h,00h,00h,00h
        db      9Fh,00h,00h,00h,00h,00h,9Eh,00h,00h,00h,00h,00h,9Dh,00h,00h,00h
        db      00h,00h,9Ch,00h,00h,00h,00h,00h,9Bh,00h,00h,00h,00h,00h,9Ah,00h
        db      00h,00h,00h,00h,99h,00h,00h,00h,00h,00h,98h,00h,00h,00h,00h,00h
        db      97h,00h,00h,00h,00h,00h,96h,00h,00h,00h,00h,00h,95h,00h,00h,00h
        db      00h,00h,94h,00h,00h,00h,00h,00h,93h,00h,00h,00h,00h,00h,92h,00h
        db      00h,00h,00h,00h,91h,00h,00h,00h,00h,00h,90h,00h,00h,00h,00h,00h
        db      8Fh,00h,00h,00h,00h,00h,8Eh,00h,00h,00h,00h,00h,8Dh,00h,00h,00h
        db      00h,00h,8Ch,00h,00h,00h,00h,00h,8Bh,00h,00h,00h,00h,00h,8Ah,00h
        db      00h,00h,00h,00h,89h,00h,00h,00h,00h,00h,88h,00h,00h,00h,00h,00h
        db      87h,00h,00h,00h,00h,00h,86h,00h,00h,00h,00h,00h,85h,00h,00h,00h
        db      00h,00h,84h,00h,00h,00h,00h,00h,83h,00h,00h,00h,00h,00h,82h,00h
        db      00h,00h,00h,00h,81h,00h,00h,00h,00h,00h,80h,00h,00h,00h,00h,00h
        db      7Fh,00h,00h,00h,00h,00h,7Eh,00h,00h,00h,00h,00h,7Dh,00h,00h,00h
        db      00h,00h,7Ch,00h,00h,00h,00h,00h,7Bh,00h,00h,00h,00h,00h,7Ah,00h
        db      00h,00h,00h,00h,79h,00h,00h,00h,00h,00h,78h,00h,00h,00h,00h,00h
        db      77h,00h,00h,00h,00h,00h,76h,00h,00h,00h,00h,00h,75h,00h,00h,00h
        db      00h,00h,74h,00h,00h,00h,00h,00h,73h,00h,00h,00h,00h,00h,72h,00h
        db      00h,00h,00h,00h,71h,00h,00h,00h,00h,00h,70h,00h,00h,00h,00h,00h
        db      6Fh,00h,00h,00h,00h,00h,6Eh,00h,00h,00h,00h,00h,6Dh,00h,00h,00h
        db      00h,00h,6Ch,00h,00h,00h,00h,00h,6Bh,00h,00h,00h,00h,00h,6Ah,00h
        db      00h,00h,00h,00h,69h,00h,00h,00h,00h,00h,68h,00h,00h,00h,00h,00h
        db      67h,00h,00h,00h,00h,00h,66h,00h,00h,00h,00h,00h,65h,00h,00h,00h
        db      00h,00h,64h,00h,00h,00h,00h,00h,63h,00h,00h,00h,00h,00h,62h,00h
        db      00h,00h,00h,00h,61h,00h,00h,00h,00h,00h,60h,00h,00h,00h,00h,00h
        db      5Fh,00h,00h,00h,00h,00h,5Eh,00h,00h,00h,00h,00h,5Dh,00h,00h,00h
        db      00h,00h,5Ch,00h,00h,00h,00h,00h,5Bh,00h,00h,00h,00h,00h,5Ah,00h
        db      00h,00h,00h,00h,59h,00h,00h,00h,00h,00h,58h,00h,00h,00h,00h,00h
        db      57h,00h,00h,00h,00h,00h,56h,00h,00h,00h,00h,00h,55h,00h,00h,00h
        db      00h,00h,54h,00h,00h,00h,00h,00h,53h,00h,00h,00h,00h,00h,52h,00h
        db      00h,00h,00h,00h,51h,00h,00h,00h,00h,00h,50h,00h,00h,00h,00h,00h
        db      4Fh,00h,00h,00h,00h,00h,4Eh,00h,00h,00h,00h,00h,4Dh,00h,00h,00h
        db      00h,00h,4Ch,00h,00h,00h,00h,00h,4Bh,00h,00h,00h,00h,00h,4Ah,00h
        db      00h,00h,00h,00h,49h,00h,00h,00h,00h,00h,48h,00h,00h,00h,00h,00h
        db      47h,00h,00h,00h,00h,00h,46h,00h,00h,00h,00h,00h,45h,00h,00h,00h
        db      00h,00h,44h,00h,00h,00h,00h,00h,43h,00h,00h,00h,00h,00h,42h,00h
        db      00h,00h,00h,00h,41h,00h,00h,00h,00h,00h,40h,00h,00h,00h,00h,00h
        db      3Fh,00h,00h,00h,00h,00h,3Eh,00h,00h,00h,00h,00h,3Dh,00h,00h,00h
        db      00h,00h,3Ch,00h,00h,00h,00h,00h,3Bh,00h,00h,00h,00h,00h,3Ah,00h
        db      00h,00h,00h,00h,39h,00h,00h,00h,00h,00h,38h,00h,00h,00h,00h,00h
        db      37h,00h,00h,00h,00h,00h,36h,00h,00h,00h,00h,00h,35h,00h,00h,00h
        db      00h,00h,34h,00h,00h,00h,00h,00h,33h,00h,00h,00h,00h,00h,32h,00h
        db      00h,00h,00h,00h,31h,00h,00h,00h,00h,00h,30h,00h,00h,00h,00h,00h
        db      2Fh,00h,00h,00h,00h,00h,2Eh,00h,00h,00h,00h,00h,2Dh,00h,00h,00h
        db      00h,00h,2Ch,00h,00h,00h,00h,00h,2Bh,00h,00h,00h,00h,00h,2Ah,00h
        db      00h,00h,00h,00h,29h,00h,00h,00h,00h,00h,28h,00h,00h,00h,00h,00h
        db      27h,00h,00h,00h,00h,00h,26h,00h,00h,00h,00h,00h,25h,00h,00h,00h
        db      00h,00h,24h,00h,00h,00h,00h,00h,23h,00h,00h,00h,00h,00h,22h,00h
        db      00h,00h,00h,00h,21h,00h,00h,00h,00h,00h,20h,00h,00h,00h,00h,00h
        db      1Fh,00h,00h,00h,00h,00h,1Eh,00h,00h,00h,00h,00h,1Dh,00h,00h,00h
        db      00h,00h,1Ch,00h,00h,00h,00h,00h,1Bh,00h,00h,00h,00h,00h,1Ah,00h
        db      00h,00h,00h,00h,19h,00h,00h,00h,00h,00h,18h,00h,00h,00h,00h,00h
        db      17h,00h,00h,00h,00h,00h,16h,00h,00h,00h,00h,00h,15h,00h,00h,00h
        db      00h,00h,14h,00h,00h,00h,00h,00h,13h,00h,00h,00h,00h,00h,12h,00h
        db      00h,00h,00h,00h,11h,00h,00h,00h,00h,00h,10h,00h,00h,00h,00h,00h
        db      0Fh,00h,00h,00h,00h,00h,0Eh,00h,00h,00h,00h,00h,0Dh,00h,00h,00h
        db      00h,00h,0Ch,00h,00h,00h,00h,00h,0Bh,00h,00h,00h,00h,00h,0Ah,00h
        db      00h,00h,00h,00h,09h,00h,00h,00h,00h,00h,08h,00h,00h,00h,00h,00h
        db      07h,00h,00h,00h,00h,00h,06h,00h,00h,00h,00h,00h,05h,00h,00h,00h
        db      00h,00h,04h,00h,00h,00h,00h,00h,03h,00h,00h,00h,00h,00h,02h,00h
        db      00h,00h,00h,00h,01h
        db      0Bh dup (0)
        db      0FFh,0FFh,00h,00h,00h,00h,0FEh,0FFh,00h,00h,00h,00h,3Dh,40h,32h,41h
        db      28h,41h,1Eh,41h,14h,41h,0Ah,41h,00h,41h,0F6h,40h,0ECh,40h,0E2h,40h
        db      0D8h,40h,0CEh,40h,0C4h,40h,0BAh,40h,0B0h,40h,0A6h,40h,9Ch,40h,0ACh,49h
        db      16h dup (0)
        db      3Ah,42h,7Ah,43h,72h,43h,6Ah,43h,62h,43h,5Ah,43h,52h,43h,4Ah,43h
        db      42h,43h,3Ah,43h,32h,43h,2Ah,43h,22h,43h,1Ah,43h,12h,43h,0Ah,43h
        db      02h,43h,0FAh,42h,0F2h,42h,0EAh,42h,0E2h,42h,0DAh,42h,0D2h,42h,0CAh,42h
        db      0C2h,42h,0BAh,42h,0B2h,42h,0AAh,42h,0A2h,42h,9Ah,42h,92h,42h,8Ah,42h
        db      82h,42h
        db      11h dup (0)
        db      08h,04h,0Ch,02h,0Ah,06h,0Eh,01h,09h,05h,0Dh,03h,0Bh,07h,0Fh
        db      200h dup (0)
        db      0A1h,4Ah,9Eh,4Ah,9Bh,4Ah,98h,4Ah,95h,4Ah,92h,4Ah,8Fh,4Ah,8Ch,4Ah
        db      89h,4Ah,86h,4Ah,83h,4Ah,80h,4Ah,7Dh,4Ah,7Ah,4Ah,77h,4Ah,74h,4Ah
        db      71h,4Ah,6Eh,4Ah,6Bh,4Ah,68h,4Ah,65h,4Ah,62h,4Ah,5Fh,4Ah,5Ch,4Ah
        db      59h,4Ah,56h,4Ah,53h,4Ah,50h,4Ah,4Dh,4Ah,4Ah,4Ah,47h,4Ah,44h,4Ah
        db      41h,4Ah,3Eh,4Ah,3Bh,4Ah,38h,4Ah,35h,4Ah,32h,4Ah,2Fh,4Ah,2Ch,4Ah
        db      29h,4Ah,26h,4Ah,23h,4Ah,20h,4Ah,1Dh,4Ah,1Ah,4Ah,17h,4Ah,14h,4Ah
        db      11h,4Ah,0Eh,4Ah,0Bh,4Ah,08h,4Ah,05h,4Ah,86h,49h,71h,49h,5Ch,49h
        db      47h,49h,32h,49h,1Dh,49h,08h,49h,0F3h,48h,0DEh,48h,0C9h,48h,0B4h,48h
        db      9Fh,48h,8Ah,48h,75h,48h,60h,48h,4Bh,48h,36h,48h,21h,48h,0Ch,48h
        db      0F7h,47h,0E2h,47h,0CDh,47h,0B8h,47h,0A3h,47h,8Eh,47h,79h,47h,64h,47h
        db      4Fh,47h,3Ah,47h,25h,47h,10h,47h,0FBh,46h,0E6h,46h,0D1h,46h,0BCh,46h
        db      0A7h,46h,92h,46h,7Dh,46h,68h,46h,53h,46h,3Eh,46h,29h,46h,14h,46h
        db      0FFh,45h,0EAh,45h,0D5h,45h,0C0h,45h,0ABh,45h,96h,45h,81h,45h,6Ch,45h
        db      57h,45h,42h,45h
        db      0Eh dup (0)
        db      01h,00h,08h,00h,08h,00h,08h,00h,01h,00h,08h,00h,0A5h,4Ah,0E1h,44h
        db      870h dup (0)
        db      46h,55h
FD52    ends

SEG003_TEXT segment word public 'code'
        assume  cs:SEG003_TEXT, ds:DGROUP, ss:DGROUP
        public  _ShowClip
        public  _Transparency
        public  _cXfer
        public  _seg003_1D59
        public  _seg003_1082

; seg003_1082  (+1082)
; scale_nibble_bitmap (FM Towns; far): draw the sprite described at 0B0C .. 0B3F. Gives up for a
; sprite 2 pixels or less in either direction, one wider than 200h, or one wholly outside the
; window. Works out the steps, clips left, right, top and bottom, generates the row scaler into
; L1F3A (through 0B10), then for each screen row from the top down: when the source row changes,
; unpack and scale it into 0B42 by calling L1F3A; write the scaled row into the frame buffer at
; row 095E[y] + x through the span writer (0B12). Runs on the library's stack.
_seg003_1082 proc far
        push    es
        push    ds
        mov     ax,seg seg048
        mov     ds,ax
        mov     bx,ss
        mov     word ptr cs:L1F36,bx
        mov     word ptr cs:L1F38,sp
        cli
        mov     ss,ax
        mov     sp,word ptr ds:[5586h]
        sti
        mov     cx,word ptr ds:[0B28h]
        cmp     cx,2
        jg      L10A9
        jmp     L11C8
L10A9:
        cmp     word ptr ds:[0B26h],2
        jg      L10B3
        jmp     L11C8
L10B3:
        mov     ax,word ptr ds:[0B1Eh]
        dec     ax
        xor     dx,dx
        mov     word ptr ds:[0B40h],dx
        mov     dl,ah
        mov     ah,al
        xor     al,al
        dec     cx
        div     cx
        inc     cx
        or      ax,ax
        jne     L10CE
        jmp     L11C8
L10CE:
        mov     word ptr ds:[0B3Ah],ax
        mov     ax,cs
        mov     es,ax
        mov     ax,word ptr ds:[0B22h]
        add     ax,cx
        dec     ax
        sub     ax,word ptr ds:[3DF6h]
        dec     ax
        jle     L10ED
        sub     cx,ax
        jg      L10E9
        jmp     L11C8
L10E9:
        mov     word ptr ds:[0B28h],cx
L10ED:
        mov     ax,word ptr ds:[0B22h]
        mov     si,word ptr ds:[3DF2h]
        sub     ax,si
        jge     L110F
        neg     ax
        jg      L10FF
        jmp     L11C8
L10FF:
        mov     word ptr ds:[0B40h],ax
        mov     word ptr ds:[0B22h],si
        sub     word ptr ds:[0B28h],ax
        jg      L110F
        jmp     L11C8
L110F:
        cmp     word ptr ds:[0B28h],200h
        jl      L111A
        jmp     L11C8
L111A:
        mov     di,offset L1F3A
        call    word ptr ds:[0B10h]
        xor     ax,ax
        xor     dx,dx
        mov     ah,byte ptr ds:[0B20h]
        mov     dl,byte ptr ds:[0B21h]
        div     word ptr ds:[0B26h]
        mov     word ptr ds:[0B3Ch],ax
        xor     dx,dx
        mov     cx,word ptr ds:[0B26h]
        mov     bp,word ptr ds:[0B24h]
        cmp     bp,word ptr ds:[3DF4h]
        jle     L1158
        sub     bp,word ptr ds:[3DF4h]
        sub     cx,bp
        jle     L11C8
        mov     ax,bp
        mul     word ptr ds:[0B3Ch]
        mov     dx,ax
        mov     bp,word ptr ds:[3DF4h]
L1158:
        mov     bx,bp
        sub     bx,cx
        inc     bx
        cmp     bx,word ptr ds:[3DF8h]
        jge     L116B
        sub     bx,word ptr ds:[3DF8h]
        add     cx,bx
        jle     L11C8
L116B:
        shl     bp,1
        add     bp,95Eh
        mov     bx,0B2Ah
        mov     word ptr ds:[0B3Eh],cx
        mov     cl,0FFh
L117A:
        cmp     cl,dh
        je      L11A4
        mov     al,dh
        mul     byte ptr ds:[0DC2h]
        mov     si,word ptr ds:[0B1Ah]
        add     si,word ptr ds:[0B40h]
        mov     es,word ptr ds:[0B0Eh]
        mov     ds,word ptr ds:[0B1Ch]
        add     si,ax
        mov     di,0B42h
        mov     cx,0F04h
        call    near ptr L1F3A
        mov     ds,word ptr ss:[0B0Eh]
L11A4:
        mov     es,word ptr ds:[95Ah]
        mov     di,word ptr [bp]
        dec     bp
        dec     bp
        add     di,word ptr ds:[0B22h]
        mov     si,0B42h
        mov     cx,word ptr ds:[0B28h]
        call    word ptr ds:[0B12h]
        mov     cl,dh
        add     dx,word ptr ds:[0B3Ch]
        dec     word ptr ds:[0B3Eh]
        jne     L117A
L11C8:
        cli
        mov     ss,word ptr cs:L1F36
        mov     sp,word ptr cs:L1F38
        sti
        pop     ds
        pop     es
        ; UW1 only: as at the end of POLYFILL's mapper, set the colour byte 410F from 3963:0658
        ; (60h when that is 0) when the pen-mode table at seg048:0008 has any byte set.
        push    es
        push    di
        push    ax
        mov     ax,seg seg048
        mov     es,ax
        mov     ax,0
        mov     cx,0FFh
        mov     di,8
        repe scasb
        jcxz    L11FD
        mov     al,byte ptr es:[658h]
        test    al,0FFh
        jne     L11F9
        mov     al,60h
        mov     byte ptr es:[658h],al
L11F9:
        mov     byte ptr es:[410Fh],al
L11FD:
        pop     ax
        pop     di
        pop     es
        retf
_seg003_1082 endp

; XFER.DAT, five 256-byte colour translation tables. UW2's LIGHTING loads the file over these
; bytes at startup (lget("xfer.dat", ..., 500h)); FM Towns calls them _cXfer. UW1's bytes here
; are its own: tables 0 and 1 filled, 2 to 4 zero.
_cXfer label byte
; table 0
        db      00h,01h,02h,03h,04h,05h,06h,07h,08h,09h,0Ah,0Bh,0Ch,0Dh,0Eh,0Fh
        db      10h,11h,12h,13h,14h,15h,16h,17h,1Dh,1Dh,1Eh,1Fh,1Fh,6Dh,9Eh,6Eh
        db      24h,26h,27h,27h,0EAh,0EAh,0ABh,0ABh,2Bh,2Ch,2Ch,2Dh,2Eh,2Fh,7Fh,0AFh
        db      30h,31h,32h,33h,30h,31h,32h,33h,30h,31h,32h,33h,30h,31h,32h,33h
        db      45h,45h,46h,47h,48h,49h,4Ah,4Ah,4Bh,4Ch,4Dh,4Dh,4Eh,4Fh,0F5h,0F5h
        db      55h,56h,57h,58h,58h,59h,5Ah,5Bh,5Ch,5Ch,5Dh,5Dh,5Eh,5Fh,5Fh,0F3h
        db      64h,65h,66h,66h,67h,69h,6Ah,6Ah,6Bh,6Ch,6Dh,6Dh,6Eh,6Fh,6Fh,0F8h
        db      75h,76h,77h,77h,78h,79h,7Ah,7Ah,7Bh,7Ch,7Ch,7Dh,7Eh,7Fh,7Fh,0F4h
        db      84h,85h,86h,87h,88h,89h,89h,8Ah,8Bh,8Ch,8Ch,8Dh,8Eh,8Fh,8Fh,0F4h
        db      95h,95h,96h,97h,98h,99h,9Ah,9Bh,9Bh,9Ch,9Ch,9Dh,9Eh,9Fh,6Fh,0F8h
        db      0A4h,0A5h,0A6h,0A7h,0A8h,0A9h,0A9h,0AAh,0ABh,0ACh,0ADh,0ADh,0AEh,0AFh,0F4h,0F4h
        db      0B4h,0B5h,0B5h,0B7h,0B8h,0B9h,0BAh,0BBh,0BCh,0BCh,0BDh,0BEh,0BFh,0BFh,8Fh,0F4h
        db      0C5h,0C5h,0C6h,0C7h,0C8h,0C8h,0C9h,0CAh,0CBh,0CCh,0CCh,0CEh,0CFh,0CFh,0F2h,0F2h
        db      0D3h,0D4h,0D5h,0D6h,0D7h,0D8h,0DAh,0DBh,0DBh,0DCh,0DDh,0DEh,0DEh,0DFh,0F3h,0F3h
        db      0E4h,0E4h,0E5h,0E6h,0E7h,0E8h,0EAh,0EBh,0EBh,0ECh,0EDh,0EDh,0EEh,0EFh,0F4h,0F4h
        db      0F0h,0F1h,0F2h,0F3h,0F4h,0F5h,0F6h,0F7h,0F8h,0F9h,0FAh,0FBh,0FCh,0FDh,0FEh,0FFh
; table 1
        db      9Ah,9Ah,61h,9Ah,9Ah,71h,0E1h,82h,0C0h,82h,94h,60h,0C0h,40h,80h,9Ah
        db      0E0h,0E1h,82h,83h,83h,70h,0E0h,0E1h,64h,95h,95h,95h,96h,96h,97h,97h
        db      73h,73h,73h,73h,28h,28h,28h,28h,94h,95h,95h,96h,96h,97h,98h,98h
        db      66h,66h,66h,65h,66h,67h,66h,66h,66h,67h,67h,66h,67h,67h,67h,66h
        db      0C0h,0C0h,0C0h,0C0h,0C0h,64h,64h,64h,65h,65h,65h,66h,18h,18h,18h,19h
        db      70h,71h,72h,72h,72h,73h,74h,74h,74h,75h,75h,75h,76h,76h,98h,98h
        db      60h,91h,0F0h,62h,93h,63h,94h,64h,64h,96h,65h,96h,97h,97h,98h,98h
        db      70h,70h,93h,93h,94h,94h,94h,95h,95h,96h,96h,96h,97h,97h,98h,98h
        db      93h,80h,80h,80h,73h,73h,73h,28h,28h,28h,28h,97h,98h,98h,98h,99h
        db      60h,91h,0F0h,92h,93h,93h,94h,94h,95h,95h,95h,96h,96h,97h,98h,98h
        db      71h,71h,72h,72h,73h,73h,74h,74h,28h,28h,76h,76h,29h,98h,98h,99h
        db      80h,80h,81h,81h,82h,82h,83h,83h,20h,20h,29h,29h,29h,29h,2Ah,98h
        db      62h,0C0h,0C0h,0C0h,0C0h,0C1h,64h,65h,65h,65h,65h,66h,66h,67h,67h,67h
        db      92h,0D0h,0D0h,0D0h,0D1h,0D1h,0D2h,0D2h,75h,97h,97h,98h,98h,98h,98h,99h
        db      71h,71h,72h,72h,73h,73h,74h,28h,28h,28h,76h,76h,29h,98h,98h,99h
        db      91h,9Ah,67h,99h,99h,19h,99h,68h,99h,99h,9Ah,64h,97h,0D2h,9Ah,95h
; table 2
        db      100h dup (0)
; table 3
        db      100h dup (0)
; table 4
        db      100h dup (0)
L1701   db      658h dup (0)            ; UW1 only: 658h zero bytes; nothing refers to them

; seg003_1D59  (+1D59): span routines and the scaler generators, reached through
; The span writers (CX pixels from SI at 0B42 to ES:DI in the frame buffer): _1D59 remaps the
; screen pixel under each non-zero source pixel through cXfer table 0 (a translucent sprite),
; L1D6E the same with table 1 (UW2 tables 4 and 3); the unlabelled writer after L1D83 writes
; non-zero pixels, those
; from 0FBh through the cXfer table they name; then a plain copy (rep movsb), a transparent copy
; (skip 0), and a writer that puts bmcolor (0DC3) wherever the source is non-zero, for pick
; frames.
;
; The generators (write code to ES:DI = L1F3A, ending it with a ret): the first (after L1E06)
; unpacks four-bit pixels, choosing the nibble by the parity of the source position and
; translating through a 16-entry palette with xlat ss:; the one after L1E8C scales a byte row,
; emitting movsb for pixels kept once, lodsb then several stosb for pixels repeated when
; enlarging (step below 1.0), or skipping source bytes with inc si / add si,n when shrinking
; (step above 1.0).
_seg003_1D59 proc near
        mov     bx,offset _cXfer        ; UW1: table 0 (UW2 table 4, +400h)
L1D5C:
        lodsb
        or      al,al
        je      L1D6A
        mov     al,byte ptr es:[di]
        xlat    byte ptr cs:[bx]
L1D66:
        stosb
        loop    L1D5C
        retn
L1D6A:
        inc     di
        loop    L1D5C
        retn
L1D6E:
        mov     bx,offset _cXfer+100h   ; UW1: table 1 (UW2 table 3, +300h)
L1D71:
        lodsb
        or      al,al
        je      L1D7F
        mov     al,byte ptr es:[di]
        xlat    byte ptr cs:[bx]
        stosb
        loop    L1D71
        retn
L1D7F:
        inc     di
        loop    L1D71
        retn
L1D83:
L1D84 equ $+1
        sub     al,0FBh
        mov     bx,offset _cXfer
        add     bh,al
        mov     al,byte ptr es:[di]
        xlat    byte ptr cs:[bx]
        jmp     short L1DA0
        mov     bx,offset _cXfer
        lodsb
        dec     cx
        je      L1DB2
        or      al,al
        je      L1DA8
L1D9C:
        cmp     al,0FBh
        jae     L1D83
L1DA0:
        stosb
        lodsb
        or      al,al
        loopne  L1D9C
        jcxz    L1DB4
L1DA8:
        inc     di
        lodsb
        or      al,al
        loope   L1DA8
        jcxz    L1DB4
        jmp     L1D9C
L1DB2:
        or      al,al
L1DB4:
        je      L1DBB
        cmp     al,0FBh
        jae     L1DBB
        stosb
L1DBB:
        retn
        rep movsb
        retn
        lodsb
        dec     cx
        je      L1DD9
        or      al,al
        je      L1DCF
L1DC7:
        stosb
        lodsb
        or      al,al
        loopne  L1DC7
        jcxz    L1DDB
L1DCF:
        inc     di
        lodsb
        or      al,al
        loope   L1DCF
        jcxz    L1DDB
        jmp     L1DC7
L1DD9:
        or      al,al
L1DDB:
        je      L1DDE
        stosb
L1DDE:
        retn
        mov     bl,byte ptr ds:[0DC3h]
        lodsb
        dec     cx
        je      L1DFF
        or      al,al
        je      L1DF5
L1DEB:
        mov     al,bl
        stosb
        lodsb
        or      al,al
        loopne  L1DEB
        jcxz    L1E01
L1DF5:
        inc     di
        lodsb
        or      al,al
        loope   L1DF5
        jcxz    L1E01
        jmp     L1DEB
L1DFF:
        or      al,al
L1E01:
        je      L1E06
        mov     al,bl
        stosb
L1E06:
        retn
        xor     dx,dx
        xor     bp,bp
        mov     ax,word ptr ds:[0B1Eh]
        shr     ax,1
        mov     byte ptr ds:[0DC2h],al
        test    byte ptr ds:[0B3Bh],0FFh
        je      L1E55
L1E1A:
        xor     bx,bx
        mov     bl,dh
        mov     si,0E820h
        shr     bx,1
        jb      L1E28
        mov     si,0E8D2h
L1E28:
        sub     bx,bp
        je      L1E3F
        add     bp,bx
        cmp     bl,0FFh
        jne     L1E38
        mov     al,4Eh
        stosb
        jmp     short L1E3F
L1E38:
        mov     ax,0C683h
        stosw
        mov     al,bl
        stosb
L1E3F:
        mov     al,0ACh
        stosb
        mov     ax,si
        stosw
        mov     al,36h
        stosb
        mov     ax,0AAD7h
        stosw
        inc     bp
        add     dx,word ptr ds:[0B3Ah]
        loop    L1E1A
        jmp     short L1E8C
L1E55:
        mov     bp,0FFF8h
L1E58:
        xor     bx,bx
        mov     bl,dh
        cmp     bx,bp
        je      L1E83
        mov     bp,bx
        test    bx,1
        je      L1E72
        mov     ax,0E088h
        stosw
        mov     ax,0E820h
        stosw
        jmp     short L1E7D
L1E72:
        mov     al,0ACh
        stosb
        mov     ax,0C488h
        stosw
        mov     ax,0E8D2h
        stosw
L1E7D:
        mov     al,36h
        stosb
        mov     al,0D7h
        stosb
L1E83:
        mov     al,0AAh
        stosb
        add     dx,word ptr ds:[0B3Ah]
        loop    L1E58
L1E8C:
        mov     al,0C3h
        stosb
        retn
        mov     bp,word ptr ds:[0B28h]
        mov     ax,word ptr ds:[0B1Eh]
        mov     byte ptr ds:[0DC2h],al
        mov     ax,word ptr ds:[0B3Ah]
        test    word ptr ds:[0B40h],0FFFFh
        je      L1EB5
        mov     bx,word ptr ds:[0B40h]
        imul    bx
        mov     al,ah
        mov     ah,dl
        mov     word ptr ds:[0B40h],ax
        mov     ax,word ptr ds:[0B3Ah]
L1EB5:
        mov     bx,ax
        test    ah,0FFh
        jne     L1F10
        cmp     ax,128h
        jle     L1EF7
        xor     bh,bh
        mov     al,51h
        stosb
        xor     si,si
L1EC8:
        xor     dx,dx
L1ECA:
        add     bh,bl
        inc     dx
        jae     L1ECA
        cmp     dx,1
        jne     L1ED7
        inc     si
        jmp     short L1EE8
L1ED7:
        mov     al,0A4h
        mov     cx,si
        rep stosb
        xor     si,si
        mov     al,0ACh
        stosb
        mov     al,0AAh
        mov     cx,dx
        rep stosb
L1EE8:
        sub     bp,dx
        jg      L1EC8
        mov     al,0A4h
        mov     cx,si
        rep stosb
        mov     al,59h
        stosb
        jmp     L1E8C
L1EF7:
        xor     bh,bh
L1EF9:
        xor     dx,dx
L1EFB:
        inc     dx
        add     bh,bl
        jae     L1EFB
        mov     al,0ACh
        stosb
        mov     al,0AAh
        mov     cx,dx
        rep stosb
        sub     bp,dx
        jg      L1EF9
        jmp     L1E8C
L1F10:
        xor     dx,dx
L1F12:
        mov     al,0A4h
        stosb
        add     dx,bx
        dec     dh
        je      L1F30
        cmp     dh,1
        jg      L1F27
        mov     al,46h
        dec     dh
        stosb
        jmp     short L1F30
L1F27:
        mov     ax,0C683h
        stosw
        mov     al,dh
        stosb
        xor     dh,dh
L1F30:
        dec     bp
        jne     L1F12
        jmp     L1E8C
L1F36   dw      0
L1F38   dw      0
L1F3A   db      0C80h dup (0)           ; code the generators above write and _seg003_1082 calls
_seg003_1D59 endp

SEG003_TEXT ends

        end
