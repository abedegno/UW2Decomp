# Layout assumptions

The exact link (`tools/link.py`) proves the sources by rebuilding `UW2.EXE`, and for that every byte may sit where the original has it. The modding build (`tools/link.py --mod`, see the README) lets sources change size, so anything that holds an address as a plain number, rather than as a fixup the linker fills in, breaks when the thing it points at moves. This file records where such numbers are, how they were found, what was changed, and what still depends on the original layout.

The scan is `tools/addrscan.py` (needs `.venv/bin/python`). It reads the matched objects, the EXE's relocations, symbols.tsv and the IDA listing, follows the segment registers through the assembly, and lists every operand with no fixup that could be an address. Lines it marks with `*` are the ones that may go through DGROUP or through a segment it cannot name; each of those was read by hand (below).

## What already moves with the link

Every relocation in the EXE is a fixup somewhere: in the objects (verify.py checks each one), and in the extracted modules, where `tools/extract.py` turns each relocated word into `dd NAME` or `dw seg NAME`. Every C global, call, string and pointer initialiser is a fixup too, since Turbo C emits one for each. Overlay stubs, the overlay manager's segment table, `__SEGTABLE__` and the FBOV header are written by TLINK. So the numbers that can break are near offsets: DGROUP addresses, offsets into a far data segment, and code offsets, written as numbers in assembly sources, in C, or in the extracted bytes.

## The extracted modules

`tools/extract.py` takes these from your EXE:

| Module | Address | Bytes | Contents |
| --- | --- | --- | --- |
| XFAR | 370D:0008..07D0 (FD51, seg_370D) | 1992 | the graphics module's data before SEG003B's piece |
| XFAR | 4FAF:0000..C788 (FD58, seg052_519C) | 51080 | the 3D renderer's data and object models before SEG004F's piece |
| XFAR | 60B9:0000..05A5 (FD71, dseg062_62a6) | 1445 | seg021's data before SEG004E's piece |
| XA016 | DS:034D | 7 | zero |
| XB101 | DS:0C0C | 6 | zero (`_curelem`) |
| XB110 | DS:1142 | 14 | `00 00 00 00 64 57 4A 3D 30 23 16 09 01 00`, byte values |
| XA128 | DS:18A1 | 3 | zero |
| XA037, XB104, XA123, XB136 | DS:349B, 4928, 6A97, 8174 | 21, 2, 57, 4 | `_BSS` space, with the names placed in it |

The rest (XORDER, XEMPTY18, XSEG020, XF006, XF039, the empty overlays) only declare segments, which keeps the segment order and holds no bytes.

- **Relocations:** none of these ranges holds one.
- **Near DGROUP pointers:** none. The DGROUP gaps are zero or byte values. In the far data heads, the IDA listing types no word as a DGROUP offset, and the code that reads these segments runs with DS or ES on them (next section), never on DGROUP. The C code that reads names in the heads uses them as numbers (`smooth_div`, `smooth_base`, `smooth_lowpass`, `_dblen`) or as offsets into FD58 itself (`MK_FP(FP_SEG(&bmsegoff), bmsegoff)` in SEG032.C, `bmhgtoff` in SEG019.C).
- **Near code offsets:** FD58 holds the model interpreter's opcode table at 4FAF:24F4..25D2, 111 words that seg004 jumps through (`jmp word ptr [bx+24F4h]` with DS on FD58). 110 are the offsets of named handlers, `_do_eof` to `_do_bcompact_map`, the same handlers in the same order as FM Towns' opcodes 0x00 to 0xDC (`model_opcodes.tsv` in UWReverseEngineering). The entry for 0xDA is 065C:0D7F, a data word in SEG004C (`L0D7F`) with no name. In `--mod` the 110 named entries are written `dw offset NAME`; the IDA listing types the table's first entries, and the run continues while each word is the offset of a name in seg004. `--mod` with no source changed links the same EXE as the exact link, which shows these fixups give the original values.
- **Offsets into their own segment:** the heads hold offsets into FD58 itself (`bmsegoff`, `bmhgtoff`, model data), which stay right while the segment's own layout does.

## Numbers in the sources

### Changed

Each of these was a DGROUP address written as a number. The new spelling assembles or compiles to the same bytes with a fixup; each file still matches its whole segment (`tools/match.py`) and verifies (`tools/verify.py`, symbols.tsv unchanged).

| File | Was | Now | What it is |
| --- | --- | --- | --- |
| SEG039.C | `((signed char near *)0x1bf7)[(signed char)s[i]] & 1` (and `& 2`) | `(_ctype + 1)[(signed char)s[i]] & _IS_SP` (and `_IS_DIG`) | the C library's `_ctype` (DS:1BF6), which follows all of the game's `_DATA` |
| SEG021A.ASM | `mov bx,92h` before `mov ax,es:[bx]` with ES = DGROUP | `mov bx,offset DGROUP:__psp` | C0's `__psp` |
| SEG004N.ASM | `mov si,6742h` before `mov ax,DGROUP; mov ds,ax` | `mov si,offset DGROUP:_Palettes` | OVR119's object palettes, handed to the image decoders |
| SEG021Q.ASM | `mov ax,211Ch; mov sp,ax` before `mov ax,DGROUP; mov ss,ax` | `mov ax,offset DGROUP:_joy_position` | the top of the 0CEh-byte stack that opens SEG021M's `_DATA`, used by the mouse callback |

The SEG004N one was found by booting. With any of the proof edits below, the 3D view drew every object sprite on a coloured box: the decoders read their palette at DS:6742 while `Palettes` had moved. Padding `_BSS` in different files narrowed it to OVR119's `_BSS`, where `Palettes` is. The scan's first version missed it, because the number goes into SI before DS is set, and is used in a routine called through a table; the scan now follows numbers into pointer registers and through calls, and the SEG021Q stack was found when the same check was widened to `mov sp`. Both are reported by the scan when it is run on the original objects.

### Checked and not addresses

The scan's remaining `*` lines, all read by hand:

- **Data decoded as code** (IDA has no instruction there, and the bytes are tables): OVR157 +1190 (a switch table), SEG003D +172..+2A3 and +1DA, +232 (palette tables after a `retf`), SEG004N +2A3 onwards (tables), SEG021B +45.
- **Not addresses:** OVR126 `mov si,160h` and SEG019 `test si,0F000h` (IDA calls the registers `..offset`), OVR158 `push 9Ah` (a y coordinate), SEG017 `mov bx,50h` and SEG022 `mov bx,20BCh`, `mov bx,2E9Ch` (multiplier and divisors), SEG022 +B3A (BX is a call's result, not the earlier number), SEG032.C `curZoom = 0x6062` (a zoom value that happens to equal a paragraph).
- **BIOS and interrupt vectors:** SEG021G `ds:[496h]` and SEG021H `es:[417h]` (segment 0, keyboard flags), SEG021J `mov di,0CCh` (the int 33h vector).
- **seg021's own data, FD71:** SEG021E +44 (`errorexit`, reached from `_init`, which seg021_22FD_0 calls with DS on FD71, and from the exit routine after it sets DS = ES = SS = dseg062_62a6), SEG021F and SEG021G (88 operands, `?|FD71`: every caller the scan can follow holds FD71, the `?` comes from code it can only seed), SEG021H's keyboard handler (`lds di,cs:_seg021_22FD_590`, whose segment word is `dw seg dseg062_62a6`), SEG021I, SEG021J, SEG021N +13F (the tangent table, read the same way at +158 with DS known to be FD71), SEG021Q +3C1..+3F7 (it copies the private stack, with DS = SS = dseg062_62a6).
- **seg003 with DS on FD51 or FD52:** SEG003C +AC (`mov bx,seg seg049_3EE2; mov ds,bx` just before) and +531..+564 (a polygon list read from FD51 between `push ds` and `pop ds` in a dispatched routine).

### Still numbers, and what they depend on

These are addresses, but not of DGROUP or of C code, so no C change moves them. Counted by the scan (`--all`, assembly only):

| Modules | Through | Operands |
| --- | --- | --- |
| seg003 (SEG003A..N) | FD51 (seg_370D), some paths FD52 or FD71 | 1072 |
| seg004 (SEG004A..N) | FD58 (seg052_519C), FD71, FD51 | 2605 |
| seg021 (SEG021A..Q) | FD71 (dseg062_62a6), a few FD51 and FD58 | 71 |
| seg001 | FD51 | 5 |
| seg003, seg004 | each other's code segment as data (`lightabs` at 065C:6D3E, `es:[bx+6D3Eh]` in SEG003C) | 11 |

They stay right while each far segment keeps its internal layout: the head extract.py takes from the EXE, followed by the assembly modules' pieces (SEG003B, C, D, K in FD51; SEG004F in FD58; SEG004E in FD71). The same holds for the code offsets written as numbers: the seg003 handler tables in SEG003D's and SEG003K's FD51 pieces (for example 370D:0B0E `dw 14AAh, 13ABh`), the one unnamed entry of the opcode table, and SEG021H's `mov dx,62Eh` (the int 9 handler's offset in SEG021_TEXT). seg000 to seg004 come before C0 in the image, so nothing in C moves them.

## Assumptions in the tools

| Tool | What it assumes | In `--mod` |
| --- | --- | --- |
| extract.py | your EXE is the one it was written for (header 0x2C00, 0xAE segments, DGROUP at file 0x68A90); C0's `_DATA` at DS:4..AC; the C library's and the second library's `_DATA` and `_BSS` bounds (DS:204E, 2220, 865C) | still read from your EXE, which is unchanged |
| extract.py | each object's `_DATA`, `_BSS` and far segments sit where verify.py finds them, by the EXE's bytes; gaps between objects are placed by EXE address; far pieces must run to the end of their segment; overlay publics have stub entries | worked out once, from the matched objects in build/LINK/base, never from changed ones |
| verify.py | DGROUP at file 0x68A90, paragraph 0x65E9 | not run |
| link.py | an overlay's publics come in the EXE's stub order; the result equals your EXE but for two bytes (exediff.py) | both skipped: TLINK numbers a changed overlay's stub entries itself, and every call to them is a fixup |

## Limits that still apply

- DGROUP must stay under 64 KB: its static data ends at DS:8C84, and C0 adds `_stklen` (0x1000) and `_heaplen` (0xC00) from OVR112.C, so the static data can grow by about 22 KB.
- The overlay buffer is `_ovrbuffer` = 0x300 paragraphs (OVR112.C), already smaller than ovr110's code (20626 bytes). How the overlay manager sizes its buffer was not examined, and growing an overlay's code was tested only as far as the ovr101 edit below goes.
- `--mod` links the source files the last exact run had. A new file needs a place in the link order (extract.py's lists) first.
- The assembly modules in seg003, seg004 and seg021, and src/FARDATA.ASM, may change only at the same length, or after their numbers above are written as names.

## Proof

Each edit was built with `python3 tools/link.py --mod`, booted with `tools/rungame.mjs` through the menu and character creation into the 3D view, compared with the original EXE, and reverted (match.py and verify.py again):

| Edit | What moves | Result |
| --- | --- | --- |
| OVR101.C: `"Str:"` to `"Strength:"`, and a new "S+D" line (two more calls and a sum) in `show_atts` | ovr101's code; every `_DATA` after ovr101's and all of `_BSS` | the stats panel shows "Strength" and "S+D"; the game starts and the 3D view matches the original |
| SEG039.C: a new 38-byte initialised array and a loop that changes it in `init_strings` | seg039's code and every resident segment after it, the far data, DGROUP's paragraph, every `_DATA` from DS:8F2 | the marker reads `UW2MOD-1` in memory after boot; menu, character creation and 3D view as the original |
| SEG006.C: a new 333-byte `_BSS` array | all of `_BSS` after DS:222C | menu, character creation and 3D view as the original |
| all three together | all of the above | as the original, including walking about |
