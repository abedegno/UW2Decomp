; /* fardata */
; /* opts: /ml */
;
; The far data that no C file defines: the zero-filled far buffers and the checkerboard
; among the first far data segments (segment table entries 50 to 63, file 39C50 to
; 6321C). tools/extract.py links these from here and still takes from your EXE entry 51
; (seg_370D, the graphics module's data, 5E76h bytes), entry 58 (seg052_519C, the 3D
; renderer's data with the object models, E4D6h bytes) and entry 71 (dseg062_62a6, C40h
; bytes), which hold mixed tables and relocations, and declares the empty entries 55 to
; 57, 59, 61, 64 and 74.
;
; Each segment is named FDnn after its segment table entry, as tools/extract.py names
; the ones it takes from the EXE. extract.py declares them in the EXE's order before
; this module, so each segment here joins its declaration and lands where the EXE has it,
; and it compares every segment of this module with the EXE before the link. No segment
; here holds a relocation.
;
; Who defined them. Turbo C gives each far variable a paragraph-aligned segment of its
; own (named after the file and a number, class FAR_DATA), and TLINK orders segments by
; first definition. Entries 65 to 78 are single variables on paragraphs, in the order of
; the modules that use them, and the C files now define them: SEG006.C (pathsq, paths,
; flood_list0, flood_list1), SEG007.C (atk_charge), SEG016.C (effects), SEG039.C
; (str_buff, Strings), OVR093.C (arcfile), OVR095.C (seg066_0), OVR108.C (speech_fpage)
; and OVR156.C (spells), each checked by tools/verify.py and placed by the link. Entry 71
; (seg004's data) falls between SEG016's and SEG039's and is still taken from the EXE.
; Entries 50 to 63 come before all of those, so on the same reading no C file can have
; defined them, and entries 52 and 53 hold two and three variables in one segment, which
; Turbo C does not do: they were assembly, from a module that left no relocations (so its
; place in the link is unknown), or declared there and filled later as entries 51 and 58
; are. Their owner is unknown, so they stay here. tools/extract.py declares them, empty,
; before SEG006 so that TLINK places them first.
;
; Names: FM Towns names where it has them (checkerboard1, stdat, ATM_Strings,
; cmpbuf1_start, cmpbuf2_start, gr_offs, sp_inf_tab); the others are the provisional names
; in symbols.tsv.

; entry 50, 3705:0000, 88h bytes: the graphics module's 16 by 17 pattern of colour 7
; inside a border of 0, four bits a pixel. FM Towns has the same 136 bytes as
; checkerboard1. seg003 reaches it through a far pointer in its own data (seg_370D).
FD50    segment para public 'FAR_DATA'
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
FD50    ends

; entry 52, 3CF5:0000, 6AA6h bytes, zero: stdat is a work area that several files use
; as different things (the level's tile data in OVR107, the cutscene player's state in
; OVR108, archive tables in OVR093); the automap notes (ATM_Strings) follow it. FM Towns
; has the two adjacent too, ATM_Strings 468Eh after stdat. seg003 names the segment
; seg049_3EE2 (IDA's name).
FD52    segment para public 'FAR_DATA'
        public  seg049_3EE2, _stdat, _ATM_Strings
seg049_3EE2 label byte
_stdat  db      468Eh dup (0)
_ATM_Strings db 2418h dup (0)
FD52    ends

; entry 53, 43A0:0000, B0F0h bytes, zero: two compression buffers of 5400h bytes and
; the graphics offsets table, adjacent in FM Towns too.
FD53    segment para public 'FAR_DATA'
        public  _cmpbuf1_start, _cmpbuf2_start, _gr_offs
_cmpbuf1_start db 5400h dup (0)
_cmpbuf2_start db 5400h dup (0)
_gr_offs db     8F0h dup (0)
FD53    ends

; entry 54, 4EAF:0000, 1000h bytes, zero: the digital sound effects' buffer (SEG016.C)
FD54    segment para public 'FAR_DATA'
        public  _dfx_buffer
_dfx_buffer db  1000h dup (0)
FD54    ends

; entry 60, 5DFD:0000, 1000h bytes, zero: a buffer SEG032.C and seg004 use by segment
FD60    segment para public 'FAR_DATA'
        public  _seg_5DFD
_seg_5DFD db    1000h dup (0)
FD60    ends

; entry 62, 5EFD:0000, 604h bytes, zero: the sprite tables (see SEG000.ASM for the layout)
FD62    segment para public 'FAR_DATA'
        public  sp_inf_tab
sp_inf_tab db   604h dup (0)
FD62    ends

; entry 63, 5F5D:0004, 1048h bytes, zero: the off-screen block allocator's records (see
; SEG001.ASM). Byte-aligned: it starts where entry 62 ends, four bytes into paragraph 5F5D.
FD63    segment byte public 'FAR_DATA'
        public  seg_5F5D
seg_5F5D db     1048h dup (0)
FD63    ends

        end
