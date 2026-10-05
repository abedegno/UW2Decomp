# Linking

There are two links. The exact link (`tools/link.py`) rebuilds `UW.EXE` from the matched objects and proves the sources. The modding build (`tools/link.py --mod`) links sources that may change by any size. Both run TLINK 3.01 in headless DOS. Both tools were adapted from Exhume's `examples/uw2/extract.py` and `link.py` (UW2Decomp's link): the rules are TLINK's and the same for both games, and every fact below was measured again from `UW.EXE`.

## Running it

You need your own `UW.EXE`, Exhume (the repository's submodule `exhume/`, or `EXHUME` set to a checkout) with its Python environment (`make setup-exhume` at the top), and the toolchain `exhume.toml` names (`uw2/TC` and `uw2/TASM`, which `make -C uw2 setup` unpacks). From the repository root:

```sh
PY=~/Exhume/.venv/bin/python3
$PY ~/Exhume/tools/build.py --all                  # compile and assemble every source into build/
$PY tools/link.py                                  # extract, link and compare: build/LINK/out/UW.EXE
$PY ~/Exhume/tools/exediff.py build/LINK/out/UW.EXE
node ~/Exhume/tools/rungame.mjs --data ~/UWGOG/UW1 --as UW.EXE build/LINK/out/UW.EXE shot- w:8000 s:title k:Escape w:4000 s:menu
$PY tools/link.py --mod                            # the modding build: build/MODLINK/out/UW.EXE
```

The link takes about two seconds under emu2. `exediff.py` reports `IDENTICAL`, `cmp` finds no difference, and the EXE boots in js-dos to the title screen and, after Escape, to the main menu. With no source changed, `link.py --mod` gives the same EXE.

## The exact link

`tools/extract.py` writes data-only TASM modules under `build/LINK` (never committed) for everything no source holds, and a manifest. `tools/link.py` assembles them, puts the second library together with TLIB, sets the DOS date and runs:

```
TLINK /c /m /s /i XORDER C0UW1 <resident objects> /o <overlay objects> /o-, uwedit.exe, uwedit.map, CM.LIB UWLIB.LIB OVERLAY.LIB
```

It stops, naming the source to correct, when an object disagrees with the EXE: an unknown name, a name for the middle of another file's data, a public `UW.EXE` had `static`, an overlay whose publics are listed out of stub order, or a constant where the EXE has a relocation. Nothing is patched into an object.

### Facts measured from UW.EXE

| Fact | UW1 | UW2, for comparison |
|---|---|---|
| Header size, relocations | 3200h bytes, 3065 | 2C00h, 2791 |
| Segment table (`__SEGTABLE__`, from the FBOV block at the end of the load image, file 65700h) | file 5C1C0h, A1h entries | 66750h, AEh |
| Code segments | entries 0 to 49: 0 to 4 before C0, 5 empty, 6 C0's `_TEXT` with the C library, 49 the overlay manager | 0 to 48 |
| Empty code segments the original link had | entries 5, 16, 23 | 5, 18 |
| C0's `_FARDATA`, far data | entry 50; entries 51 to 76 | 49; 50 to 78 |
| Overlay stubs | entries 89 to 154, ovr089 to ovr154 (12 of the 66 empty) | 91 to 167 |
| DGROUP | entry 155, paragraph 5AACh (file 5DCC0h), `_DATA` from DS:4 | entry 168 |
| C0's `_TEXT` | 249h bytes, the C library's code after it | the same |
| C library `_DATA` | DS:1DFE (ATEXIT, then `__ctype` at 1E00) to 2258 | 1BF4 to 204E |
| Second library `_DATA` | DS:2258 to 242C, where `__OvrSize` is | 204E to 2220 |
| C library `_BSS` | 9Ch bytes from DS:738E, then seg045's (GRDB) to DS:79B6 | 9Ch from 865C |
| `__EXENAME__`, `__EXEDATE__` | `uwedit.exe` (file 5C6C8h), 12 May 1993 | the same |

The output name and the date are UW2's: both games were linked as `uwedit.exe`, and this `UW.EXE` (the GOG one) records 12 May 1993, the same day as `UW2.EXE`.

The C library's data ranges come from TLINK's map of this link (`build/LINK/out/UW.MAP`), as UW2's did. The C library's modules are the ones the objects pull in, so the linker decides them.

### C0

The startup module is Turbo C++'s `C0.ASM` with the same three changes as UW2's, which `link.py` applies to a copy. Each was tested by linking without it:

1. The self-modifying patch of the exit-table scan comes after `main` returns, not before `main` is called. Without it, 28 bytes of C0 and one relocation differ.
2. The null-pointer checksum starts at `DATASEG@` (DS:4). Without it, 12,361 bytes and 88 relocations differ.
3. `_FARDATA` is not paragraph-aligned. Entry 50 is an empty segment at 395A:000A, right after `_OVRTEXT_` ends. With `PARA`, three bytes of the segment table differ. It is `WORD` here, as in UW2. `BYTE` would give the same result at that even address.

### How the link order was found

TLINK writes relocations module by module, and a library's modules in library order. So the relocation table records the original module order, and the segment table records the segment order. `tools/extract.py` lists the order, and each list names sources by their DOS segment, not by file name.

- **Resident:** C0, seg006 to seg015, seg016 (`INT0TRAP.ASM`), `SETPNT.ASM` and seg018 (`PLANECPY.ASM`), seg000, seg019's first module (`STARTUP.ASM`), seg001 (`VALLOC.ASM`) and the routine after it (`VSTATS.ASM`), seg002, seg020 (AIL), seg021 to seg031, seg017's C (`GRIDDB.C`), seg032 to seg044, then the far data (XFAR, `FARDATA.ASM`). seg002 has no relocations, so its place is free. SetPnt and seg018 have to be seen after seg016 and before `STARTUP.ASM` declares seg019's segment, since TLINK places segments in the order it first sees their names.
- **Overlays:** ovr089 to ovr154 in order.
- **Second library (`UWLIB.LIB`),** after the C library and before the overlay manager. Its order is seg019's modules B to M, seg004's A to G, seg003's A, seg019's N to Q, seg045, seg004's H, seg003's B to G, seg004's I, and seg003's H to R. That is 44 modules. UW1's seg003 has 18 modules, seg004 9 and seg019 17. Modules with no relocations sit next to their neighbours in the same segment.
- **Segment classes,** as in UW2: seg000 to seg002 are `ASMCODE` (code flag 1, first block), seg003 and seg004 are `code` (flag 0), and everything from C0's `_TEXT` on is `CODE`. The sources already had these classes, and the link confirms them.
- **Segments declared early:** XORDER declares seg000 to seg004 before C0's `_TEXT` and holds entry 5. XEMPTY16 holds entry 16, between seg014 and seg015. XSEG019 declares seg019 and seg020 on paragraphs and holds entry 23, after seg018 and before `STARTUP.ASM`.

### What still comes from your EXE

Every byte of code has source. The last four pieces, 442 bytes, were turned into source as UW2's were:

| Piece | Source | Bytes | |
|---|---|---|---|
| seg016 (entry 18), the divide-by-zero trap | `src/sys/INT0TRAP.ASM` | 5Fh | UW2's `INT0TRAP.ASM` byte for byte but for the C handler's address (ovr109's stub, UW2's ovr112) |
| SetPnt, the first 35h bytes of entry 19 | `src/3d/SETPNT.ASM` | 35h | UW2's `SETPNT.ASM`, with UW1's `dbptr` and `ptnuminq` |
| seg018 (entry 20) | `src/gfx/PLANECPY.ASM` | 6Dh | UW2's `PLANECPY.ASM` (its seg020), but for the byte it copies as it is: 0EAh, where UW2 has 90h |
| the far routine after `VALLOC.ASM`'s code in seg001 | `src/gfx/VSTATS.ASM` | B9h | UW1 only. `valloc_stats` counts and sizes the video memory pool's free and used blocks. Nothing in `UW.EXE` calls it. Its one relocation, entry 708, follows VALLOC's (695 to 707) |

Each has its own target table (`targets/seg016.tsv`, `seg017_1FDD_1.tsv`, `seg018.tsv`, `seg001_2D1.tsv`) and matches whole and verifies. Whether `VSTATS.ASM` was a module of its own or the end of `VALLOC.ASM` cannot be told: VALLOC's code ends at the odd offset 2D1h and the routine starts there with no padding, so either gives the same EXE. It is a separate module here, its segment byte-aligned, which leaves `VALLOC.ASM` as it was.

The far data:

- 11 of the far data entries are the C files' own `far` variables (PATHFIND, AI, SOUND, SPELLS, GAMESTRN, BABL), which `verify.py` places and compares.
- The pieces of three entries come from the assembly modules whose relocations they hold, placed by `# far` lines in their target tables. Entry 52 (seg048) is filled from its first relocated pointer by `POLYFILL`, `GRENTRY`, `SCALEBM` and `GRDISP`. Entries 72 and 58 are filled by `INSTANCE.ASM` (5624:04D5 to the end of entry 72, 5DBh bytes, and 4723:2860 to B006, 87A6h bytes, from relocation entries 2639 and 2640) and `TMAPOPS.ASM` (4723:B006 to the end of entry 58, 14D3h bytes, from entries 2709 to 2720), as UW2's `INSTANCE.ASM` and `TMAPOPS.ASM` hold their pieces. Each module defines its pieces before its code, since their relocations come before the code's in the relocation table. Until they had source, these bytes were extracted into library modules of their own, `XP004G` and `XP004H`, linked just before INSTANCE and TMAPOPS.
- `src/sys/FARDATA.ASM` (marked `/* fardata */`, as UW2's) holds the seven entries whose owner is unknown and which hold no relocation: entry 51, the checkerboard (88h bytes), and the zero-filled entries 53 (`stdat` and `ATM_Strings`, 4D7Eh), 54 (`cmpbuf1_start`, also `fade_buffer`, 3000h), 60 (`seg_5DFD`, 1000h), 61 (`seg053`, 68h, UW1 only), 62 (`sp_inf_tab`, 604h) and 63 (`seg055`, VALLOC's records, 1048h). They come before every C file's far variable, so no C file can have defined them.

What is still extracted from your EXE, measured from the link's map:

| What | Bytes | Why it stays |
|---|---|---|
| Entry 52 (seg048), 3963:0008 to 07D2, before `POLYFILL.ASM`'s piece | 7CAh (1,994) | The graphics library's data before its first relocated pointer, with `smooth_div`, `smooth_base` and `smooth_lowpass`, which the C files use. No relocation ties it to a module. |
| Entry 58 (seg051), 4723:0000 to 2860, before `INSTANCE.ASM`'s piece | 2860h (10,336) | The 3D renderer's data before INSTANCE's first relocated pointer, with the model interpreter's opcode table at 2738h (near offsets of seg004's handlers, which carry no relocation). No relocation ties it to a module. |
| Entry 72 (seg063), 5624:0000 to 04D5, before `INSTANCE.ASM`'s piece | 4D5h (1,237) | The system library's data segment (SYSENTRY's stack below 448h, KBDINT's key buffer at 0300h). No relocation ties it to a module. |
| Six `_DATA` gaps in DGROUP | 31 | See below. |
| Five `_BSS` gaps in DGROUP | 83, uninitialised | See below. They take no bytes from your EXE, only their size and the names in them. |

So 13,598 bytes of initialised data, and no code, still come from your EXE. Before this, it was 97,766: the 442 bytes of code, 55,737 bytes of far data in XFAR (the three heads above and FARDATA.ASM's 42,170), INSTANCE's and TMAPOPS's 41,556 bytes in XP004G and XP004H, and the 31 bytes of `_DATA` gaps. The rest of what extract.py writes holds no bytes: XORDER, XEMPTY16 and XSEG019 declare segments, XF006 and XF038 declare far data entries early, the XOVRnnn modules are the empty overlays, and XPULL names QUADFIT.

### The DGROUP gaps

Each gap lies between two files' data in link order, so it is the end of the file before, the start of the file after, or a module with no code. No evidence picks one, so none has source. All are named by other files, which refer to them as externs. From the link's map:

| Gap | Between | Bytes | Names in it |
|---|---|---|---|
| `_DATA` DS:010B to 010E | EMS.C and MOUSE.C | 3, zero | `dseg_5c99_10C` (EMS.C uses it) at 10C, after an alignment byte |
| `_DATA` DS:0232 to 0234 | AIL.ASM and COLCYCLE.C | 2, zero | none |
| `_DATA` DS:03F7 to 03FA | OBJECTS.C and PLAYTIME.C | 3, zero | `plyregen` at 3F8 (PLAYTIME.C and PLAYDATA.C use it; UW2's WRAPPER.C defines it) |
| `_DATA` DS:0DBB to 0DC0 | BARTER.C and OVR096.C | 5, zero | `curelem` at 0DBC (CRITTIME.C and PLAYER.C use it) |
| `_DATA` DS:12B1 to 12C0 | CUTS.C and DEBUG.C | 15 | `realDScheck` at 12B3 (INTERACT.C and GAMEWRAP.C; UW2's INTERACT.C defines it), and at 12B6 the bytes 64 57 4A 3D 30 23 16 09 01 00, `dseg_5c99_12B6` (GRIDDB.C) |
| `_DATA` DS:19C7 to 19CA | MAP.C and MISC.C | 3, zero | none |
| `_BSS` DS:3638 to 364A | UTIL.C and TMPALLOC.C | 18 | `StringsPak_Address_Indices`, `StringsPak_NoOfNodes`, `StringsPak_FileHandle`, `string_bits` and `CutsceneOrConversationStringBloc`: UW2's GAMESTRN.C defines the first four, but UW1's GAMESTRN.C is not next to them in the link |
| `_BSS` DS:4A2E to 4A30 | CONVERSE.C and CREATURE.C | 2 | none |
| `_BSS` DS:5A91 to 5ACA | RUNES.C and INVDATA.C | 57 | `Inventory` at 5A92 (56 bytes), after an alignment byte |
| `_BSS` DS:716E to 7172 | OBJCLASS.C and OPTIONS.C | 4 | none |
| `_BSS` DS:7178 to 717A | OPTIONS.C and TEXTMAPS.C | 2 | `dseg_5c99_7178` (TEXTMAPS.C and TMPALLOC.C) |

`plyregen` and `Inventory` start on a word right after the file before ends at an odd address, which fits the start of a Turbo C file's data (PLAYTIME.C's or INVDATA.C's), but a module with no code fits as well. Giving either to a C file would mean editing that file, and would be a guess until something rules out the other readings.

## What the link found in the sources

Each change below was matched again with `match.py`, still giving WHOLE SEGMENT MATCHES, and checked with `verify.py`. Every source verifies after them, and a full rebuild links byte-identical.

- **`sys/TMPALLOC.C`:** `conv_ws_seg` was `static`. AUTOMAP.C and CRPAGES.C use it, so it is public.
- **`ui/INTERACT.C`:** `dseg_5c99_5626` is `Creature[63].avghit`, the player creature's maximum vitality, at Creature + BD4h. A name for the middle of another file's array cannot link.
- **`game/PLAYDATA.C`:** `MazeNavigationTextureMaybe_dseg_5c99_7184` is `floor_IDs[4]` (TEXTMAPS.C's array, + 8).
- **`gfx/CUTS.C`:** two pairs of publics with equal `bssorder.py` keys were listed in the wrong order. Those pairs are `runcutscene` and `read_lp_inc`, and `cutsop_loop` and `cutsop_stop`. Turbo C lists equal keys in the reverse of the order it first sees the names. Early prototypes of `read_lp_inc` and `cutsop_stop` now put the four in the EXE's stub order. `verify.py` compares keys only and had passed the file. The object's old order fits `cutsop_loop` being "seen" at the `#define` that renames gfx.h's prototype out of the way, which suggests the preprocessor shares the symbol table. That is inferred, not tested.
- **`gfx/WALLMAP.ASM`, `gfx/POLYFILL.ASM`, `3d/TMAPOPS.ASM`:** TLINK takes a library module only for a reference to it by name. Nothing named WALLMAP, POLYFILL or QUADFIT, so the first link left them out and seg003 came out 9E6h bytes short. TMAPOPS stored the two mappers' far entries as the numbers `1DAh` and `545h`. They are now `offset _seg003_1DA` and `offset _seg003_545`, labels at the unlabelled entries. The bytes are the same, and the modding build can now move those modules. `verify.py --update` added the two names to `symbols.tsv`. QUADFIT has no caller anywhere, so `XPULL`, an empty generated module, names it. The original must have named it from somewhere.
- **`sys/SYSENTRY.ASM`:** its `_DATA` opened with 0EAh zero bytes, placed at DS:2242. The link shows that DS:2242 to 2257 are the C library's: the `_DATA` of NEARHEAP, SETARGV and SETENVP (6, 14 and 2 zero bytes), which TLINK takes from CM.LIB after CVTFAK. The run is 0D4h bytes and the module starts at DS:2258. Until this was fixed, the second library's data sat 16h bytes high. `sys/C3DENTRY.ASM`'s comment on that stack was corrected to match.
- **`3d/PGCACHE.ASM`:** it now ends with an empty paragraph-aligned `SEG019_TEXT`, as UW2's does. This is the later contribution that makes seg019's segment 1080h long, with 15 zero bytes after its last routine. Without it, segment table entry 21 says 1071h. Which module made the contribution is not known. PGCACHE is the first module linked after seg019's last one that calls into seg019.

## Surprises

- **`seg048` is 3963:0000, but entry 52 starts at 3963:0008,** because entry 51 ends in the same paragraph. The sources write `seg048+X` from that frame. `XFAR` defines `seg048 equ $+0FFF8h` at the segment's start, which TLINK resolves to 3963:0000 in 16-bit arithmetic. `$-8` does not work: TASM writes a 32-bit public for it, which TLINK refuses ("32-bit record encountered").
- **TLINK does not clear the memory it builds the image in, unless told to.** Without `/i`, the bytes no record covers (word-alignment padding between modules' `_DATA`, and the part of SETARGV's `_DATA` no record covers) come out of TLINK's buffers as it left them: stale bytes of an object it read earlier (GAMESTRN's code). Which bytes, and whether any, depends on the emulator's memory size and on the module list: under emu2 with 512 KB the link was clean until `INT0TRAP.ASM` replaced the extracted trap, and then 16 bytes in DGROUP were stale; with `FARDATA.ASM` 40; with 640 KB it was 35 in an earlier link. The link used to work around it with `EMU2_LOWMEM`. TLINK's `/i` ("initialize all segments") makes it write every byte, zero where no record gives one, and with it the link is byte-identical under emu2 with 512 and 640 KB and under DOSBox-X, the EXE otherwise unchanged in content and size. `link.py` passes `/i`, and no longer sets `EMU2_LOWMEM`. Whether the original link used `/i` cannot be told from the EXE.
- **Library modules nobody names are not linked.** UW2 never met this, because all its library modules were referenced.

## The modding build

`python3 tools/link.py --mod` works as UW2Decomp's does (Exhume's docs/link.md, "The modding build"). The exact link keeps a snapshot in `build/LINK/base`, and changed sources are compiled into `build/MODLINK/src`. With no source changed, the EXE in `build/MODLINK/out` equals the exact link's and `UW.EXE`. The layout audit is in [LAYOUT.md](LAYOUT.md). It found no DGROUP address written as a number. Two builds that moved DGROUP, the far data and the resident code booted through character creation into the 3D view, the same as the exact build. It also lists what must still keep its length: the assembly modules' far data and the code offsets in it, including the model interpreter's opcode table in the extracted FD58.
