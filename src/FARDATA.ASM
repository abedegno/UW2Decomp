; /* fardata */
; /* opts: /ml */
;
; The far data that is program data: the zero-filled far buffers and the three small
; tables among the 29 far data segments (segment table entries 50 to 78, file 39C50 to
; 665A4). tools/extract.py links these from here and still takes the rest from your EXE:
; entry 51 (seg_370D, the graphics module's data, 5E76h bytes), entry 58 (seg052_519C,
; the 3D renderer's data with the object models, E4D6h bytes) and entry 71 (dseg062_62a6,
; C40h bytes), which hold mixed tables and relocations, and the empty entries 55 to 57,
; 59, 61, 64 and 74.
;
; Each segment is named FDnn after its segment table entry, as tools/extract.py names
; the ones it takes from the EXE. extract.py declares all 29 in the EXE's order before
; this module, so each segment here joins its declaration and lands where the EXE has it,
; and it compares every segment of this module with the EXE before the link. No segment
; here holds a relocation.
;
; Who defined them. Turbo C gives each far variable a paragraph-aligned segment of its
; own (named after the file and a number, class FAR_DATA), and TLINK orders segments by
; first definition. Entries 65 to 78 are single variables on paragraphs, and in the
; order of the modules that use them: pathsq, paths, flood_list0 and flood_list1 (SEG006
; uses all four, and the order rules out the other users), atk_charge (only SEG007),
; effects (SEG016; SEG044 and OVR108 come too late), StringBuffer and Strings (only
; SEG039), arcfile (only OVR093), seg066_0 (only OVR095), speech_fpage (OVR108) and spells
; (an overlay after OVR108). Entry 71 (seg004's data, whose bytes come late in the
; relocation order) falls between SEG016 and SEG039, where SEG021.ASM, which uses it, is
; linked. So those were most likely far variables in the files named, but the bytes do
; not prove it, and tools/verify.py does not yet check a reference into a file's own far
; segment, so they are kept here. Entries 50 to 63 come before those, so on the same
; reading no C file can have defined them, and entries 52 and 53 hold two and three
; variables in one segment, which Turbo C does not do: they were assembly, from a module
; that left no relocations (so its place in the link is unknown), or declared there and
; filled later as entries 51 and 58 are. Their owner is unknown.
;
; Names: FM Towns names where it has them (checkerboard1, stdat, ATM_Strings,
; cmpbuf1_start, cmpbuf2_start, gr_offs, sp_inf_tab, pathsq, paths, effects, spells); the
; others are the provisional names in symbols.tsv.

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

; entry 65, 6062:0000, FCh bytes, zero
FD65    segment para public 'FAR_DATA'
        public  _pathsq
_pathsq db      0FCh dup (0)
FD65    ends

; entry 66, 6072:0000, 1C0h bytes, zero
FD66    segment para public 'FAR_DATA'
        public  _paths
_paths  db      1C0h dup (0)
FD66    ends

; entries 67 and 68, 608E:0000 and 6096:0000, 80h bytes each, zero
FD67    segment para public 'FAR_DATA'
        public  _flood_list0
_flood_list0 db 80h dup (0)
FD67    ends
FD68    segment para public 'FAR_DATA'
        public  _flood_list1
_flood_list1 db 80h dup (0)
FD68    ends

; entry 69, 609E:0000, 20h bytes: sixteen two-byte records, the charge of a critter's
; attack by attack frame (SEG007.C, struct AtkCharge: the charge, then a zero byte)
FD69    segment para public 'FAR_DATA'
        public  _atk_charge
_atk_charge label byte
        db      50,0      ;  0
        db      60,0      ;  1
        db      70,0      ;  2
        db      80,0      ;  3
        db      90,0      ;  4
        db      100,0     ;  5
        db      110,0     ;  6
        db      120,0     ;  7
        db      130,0     ;  8
        db      140,0     ;  9
        db      155,0     ; 10
        db      170,0     ; 11
        db      185,0     ; 12
        db      205,0     ; 13
        db      230,0     ; 14
        db      255,0     ; 15
FD69    ends

; entry 70, 60A0:0000, 188h bytes, zero
FD70    segment para public 'FAR_DATA'
        public  _effects
_effects db     188h dup (0)
FD70    ends

; entries 72 and 73, 617D:0000 and 627D:0000, zero: the string decoder's buffer and the
; two cached string blocks (SEG039.C, struct StringBlock Strings[2])
FD72    segment para public 'FAR_DATA'
        public  _StringBuffer
_StringBuffer db 1000h dup (0)
FD72    ends
FD73    segment para public 'FAR_DATA'
        public  _Strings
_Strings db     1008h dup (0)
FD73    ends

; entry 75, 637E:0000, 58h bytes, zero: OVR093.C's struct ArcFile
FD75    segment para public 'FAR_DATA'
        public  _arcfile
_arcfile db     58h dup (0)
FD75    ends

; entry 76, 6384:0000, 40h bytes, zero (OVR095.C)
FD76    segment para public 'FAR_DATA'
        public  _seg066_0
_seg066_0 db    40h dup (0)
FD76    ends

; entry 77, 6388:0000, one byte, zero (OVR108.C)
FD77    segment para public 'FAR_DATA'
        public  _speech_fpage
_speech_fpage db 0
FD77    ends

; entry 78, 6389:0000, 114h bytes: the 69 rune spells, four bytes each (OVR123.C's
; struct Spell: the class in the top five bits of the first byte, the runes, the subclass
; in the last byte). FM Towns has it as spells, between area1_spells and dtypes.
FD78    segment para public 'FAR_DATA'
        public  _spells
_spells label byte
        db      040h,097h,021h,001h ;  0: class 8, subclass 1
        db      018h,0C8h,005h,001h ;  1: class 3, subclass 1
        db      029h,038h,039h,001h ;  2: class 5, subclass 1
        db      010h,012h,005h,002h ;  3: class 2, subclass 2
        db      03Ah,038h,059h,00Dh ;  4: class 7, subclass 13
        db      000h,078h,021h,083h ;  5: class 0, subclass 131
        db      008h,06Fh,050h,006h ;  6: class 1, subclass 6
        db      058h,0C4h,006h,003h ;  7: class 11, subclass 3
        db      038h,058h,040h,001h ;  8: class 7, subclass 1
        db      018h,002h,040h,00Ah ;  9: class 3, subclass 10
        db      028h,0C9h,03Ah,005h ; 10: class 5, subclass 5
        db      020h,02Ch,020h,002h ; 11: class 4, subclass 2
        db      040h,0A9h,020h,002h ; 12: class 8, subclass 2
        db      008h,06Fh,044h,002h ; 13: class 1, subclass 2
        db      008h,0F8h,051h,001h ; 14: class 1, subclass 1
        db      058h,097h,041h,00Dh ; 15: class 11, subclass 13
        db      038h,098h,025h,000h ; 16: class 7, subclass 0
        db      058h,0B8h,001h,006h ; 17: class 11, subclass 6
        db      038h,02Eh,001h,008h ; 18: class 7, subclass 8
        db      029h,0D8h,038h,002h ; 19: class 5, subclass 2
        db      000h,078h,041h,085h ; 20: class 0, subclass 133
        db      030h,042h,001h,085h ; 21: class 6, subclass 133
        db      058h,06Fh,046h,000h ; 22: class 11, subclass 0
        db      008h,0F8h,05Dh,044h ; 23: class 1, subclass 68
        db      030h,005h,020h,087h ; 24: class 6, subclass 135
        db      020h,098h,021h,004h ; 25: class 4, subclass 4
        db      018h,037h,035h,00Bh ; 26: class 3, subclass 11
        db      03Ah,038h,001h,00Ah ; 27: class 7, subclass 10
        db      018h,0B8h,048h,046h ; 28: class 3, subclass 70
        db      010h,058h,022h,043h ; 29: class 2, subclass 67
        db      03Ah,098h,059h,007h ; 30: class 7, subclass 7
        db      018h,04Fh,01Ah,005h ; 31: class 3, subclass 5
        db      008h,0EFh,050h,003h ; 32: class 1, subclass 3
        db      029h,0B8h,03Ch,003h ; 33: class 5, subclass 3
        db      03Ah,0D7h,03Ah,00Bh ; 34: class 7, subclass 11
        db      03Ah,0F8h,012h,00Ch ; 35: class 7, subclass 12
        db      038h,04Ch,000h,002h ; 36: class 7, subclass 2
        db      040h,069h,022h,003h ; 37: class 8, subclass 3
        db      03Ah,057h,046h,009h ; 38: class 7, subclass 9
        db      05Ah,0F7h,039h,008h ; 39: class 11, subclass 8
        db      038h,036h,042h,003h ; 40: class 7, subclass 3
        db      030h,0C6h,055h,042h ; 41: class 6, subclass 66
        db      000h,00Bh,055h,086h ; 42: class 0, subclass 134
        db      038h,02Fh,056h,00Fh ; 43: class 7, subclass 15
        db      020h,00Ch,055h,00Fh ; 44: class 4, subclass 15
        db      018h,04Bh,056h,044h ; 45: class 3, subclass 68
        db      068h,098h,058h,007h ; 46: class 13, subclass 7
        db      038h,08Fh,000h,005h ; 47: class 7, subclass 5
        db      030h,016h,054h,083h ; 48: class 6, subclass 131
        db      03Ah,0D7h,055h,00Eh ; 49: class 7, subclass 14
        db      030h,010h,038h,081h ; 50: class 6, subclass 129
        db      030h,0F8h,024h,086h ; 51: class 6, subclass 134
        db      040h,00Ch,028h,005h ; 52: class 8, subclass 5
        db      058h,0F7h,055h,009h ; 53: class 11, subclass 9
        db      058h,097h,054h,001h ; 54: class 11, subclass 1
        db      040h,0E6h,039h,006h ; 55: class 8, subclass 6
        db      030h,0F8h,014h,084h ; 56: class 6, subclass 132
        db      008h,0EFh,054h,005h ; 57: class 1, subclass 5
        db      058h,078h,002h,00Bh ; 58: class 11, subclass 11
        db      010h,0B2h,022h,045h ; 59: class 2, subclass 69
        db      058h,098h,055h,002h ; 60: class 11, subclass 2
        db      058h,0F6h,039h,007h ; 61: class 11, subclass 7
        db      038h,02Ch,055h,006h ; 62: class 7, subclass 6
        db      058h,042h,055h,00Ch ; 63: class 11, subclass 12
        db      030h,018h,063h,005h ; 64: class 6, subclass 5
        db      028h,018h,063h,004h ; 65: class 5, subclass 4
        db      058h,018h,063h,00Dh ; 66: class 11, subclass 13
        db      050h,018h,063h,003h ; 67: class 10, subclass 3
        db      050h,018h,063h,009h ; 68: class 10, subclass 9
FD78    ends

        end
