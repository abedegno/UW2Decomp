; /* fardata */
; /* opts: /ml */
;
; The far data that no C file defines: the zero-filled far buffers and the checkerboard
; among the first far data segments (segment table entries 51 to 63, file 3C7B0 to
; 58FC4). tools/extract.py links these from here and still takes from your EXE the heads
; of entry 52 (seg048, the graphics modules' data), entry 58 (seg051, the 3D renderer's
; data with the object models) and entry 72 (seg063, the system library's data): the
; bytes before the first piece a source holds, which hold mixed tables and no evidence of
; their owner. It declares the empty entries 55 to 57, 59 and 64.
;
; This is UW2's src/sys/FARDATA.ASM with UW1's entries (UW2's 50, 52, 53, 54, 60, 62 and 63
; are UW1's 51, 53, 54, 60, 62 and 63 and, new in UW1, 61).
;
; Each segment is named FDnn after its segment table entry, as tools/extract.py names
; the ones it takes from the EXE. extract.py declares them in the EXE's order before
; this module, so each segment here joins its declaration and lands where the EXE has it,
; and it compares every segment of this module with the EXE before the link. No segment
; here holds a relocation.
;
; Who defined them. Turbo C gives each far variable a paragraph-aligned segment of its
; own (named after the file and a number, class FAR_DATA), and TLINK orders segments by
; first definition. Entries 65 to 76 are single variables on paragraphs, in the order of
; the modules that use them, and the C files define them: PATHFIND.C, AI.C, SOUND.C,
; SPELLS.C, GAMESTRN.C and BABL.C, each checked by tools/verify.py and placed by the link.
; Entries 51 to 63 come before all of those, so on the same reading no C file can have
; defined them, and entry 53 holds two variables in one segment, which Turbo C does not
; do: they were assembly, from a module that left no relocations (so its place in the
; link is unknown), or declared there and filled later as entries 52 and 58 are. Their
; owner is unknown, so they stay here. tools/extract.py declares them, empty, before
; PATHFIND so that TLINK places them first.
;
; name: descriptive, as UW2's. The names are the provisional ones in symbols.tsv (UW2's,
; FM Towns where UW2 has them: checkerboard1, stdat, ATM_Strings, cmpbuf1_start,
; sp_inf_tab) or the listing's segment names (seg053, seg055), which the sources use for
; these segments.

; entry 51, 395B:0000, 88h bytes: the graphics module's 16 by 17 pattern of colour 7
; inside a border of 0, four bits a pixel, the same 136 bytes as UW2's checkerboard1.
; seg003 reaches it through a far pointer in its own data (seg048).
FD51    segment para public 'FAR_DATA'
        public  _checkerboard1
_checkerboard1 label byte
        db      000h,000h,000h,000h,000h,000h,000h,000h
        db      007h,077h,077h,077h,077h,077h,077h,070h
        db      007h,077h,077h,077h,077h,077h,077h,070h
        db      007h,077h,077h,077h,077h,077h,077h,070h
        db      007h,077h,077h,077h,077h,077h,077h,070h
        db      007h,077h,077h,077h,077h,077h,077h,070h
        db      007h,077h,077h,077h,077h,077h,077h,070h
        db      007h,077h,077h,077h,077h,077h,077h,070h
        db      007h,077h,077h,077h,077h,077h,077h,070h
        db      007h,077h,077h,077h,077h,077h,077h,070h
        db      007h,077h,077h,077h,077h,077h,077h,070h
        db      007h,077h,077h,077h,077h,077h,077h,070h
        db      007h,077h,077h,077h,077h,077h,077h,070h
        db      007h,077h,077h,077h,077h,077h,077h,070h
        db      007h,077h,077h,077h,077h,077h,077h,070h
        db      007h,077h,077h,077h,077h,077h,077h,070h
        db      000h,000h,000h,000h,000h,000h,000h,000h
FD51    ends

; entry 53, 3F4B:0000, 4D7Eh bytes, zero: stdat, a work area several files use as
; different things (the pathfinder's 64 by 64 square records in PATHFIND, the cutscene
; player's work area in CUTS, the zeros BABL writes into a new bglobals.dat), with the
; automap notes (ATM_Strings) after it, as in UW2 (where they are 468Eh and 2418h bytes).
FD53    segment para public 'FAR_DATA'
        public  _stdat, _ATM_Strings
_stdat  db      3336h dup (0)
_ATM_Strings db 1A48h dup (0)
FD53    ends

; entry 54, 4423:0000, 3000h bytes, zero: one buffer with two names. EXPAND.ASM unpacks
; compressed bitmaps into it (cmpbuf1_start, UW2's name for its first compression
; buffer; UW1 has no second one), and the C files use it as fade_buffer (gfx.h).
FD54    segment para public 'FAR_DATA'
        public  _cmpbuf1_start, _fade_buffer
_cmpbuf1_start label byte
_fade_buffer db 3000h dup (0)
FD54    ends

; entry 60, 5371:0000, 1000h bytes, zero: a buffer VIEW3D.C and seg004 use by segment
; (PGCACHE.ASM copies an object's picture into it to use as a texture), as UW2's
FD60    segment para public 'FAR_DATA'
        public  _seg_5DFD
_seg_5DFD db    1000h dup (0)
FD60    ends

; entry 61, 5471:0000, 68h bytes, zero: records INSTANCE.ASM reaches by segment (its far
; data holds a far pointer to seg053:000E, and it sorts record offsets into this
; segment); UW1 only, UW2's entry 61 is empty.
FD61    segment para public 'FAR_DATA'
        public  seg053
seg053  db      68h dup (0)
FD61    ends

; entry 62, 5477:0008, 604h bytes, zero: the sprite tables (see SPRITE.ASM for the
; layout). Byte-aligned: it starts where entry 61 ends, eight bytes into paragraph 5477
; (UW2's is on a paragraph).
FD62    segment byte public 'FAR_DATA'
        public  sp_inf_tab
sp_inf_tab db   604h dup (0)
FD62    ends

; entry 63, 54D7:000C, 1048h bytes, zero: the off-screen block allocator's records (see
; VALLOC.ASM). Byte-aligned: it starts where entry 62 ends, 0Ch bytes into paragraph
; 54D7.
FD63    segment byte public 'FAR_DATA'
        public  seg055
seg055  db      1048h dup (0)
FD63    ends

        end
