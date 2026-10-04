# Linking

There are two links. The exact link (`tools/link.py`) rebuilds `UW.EXE` from the matched objects and proves the sources. The modding build (`tools/link.py --mod`) links sources that may change by any size. Both run TLINK 3.01 in headless DOS. Both tools were adapted from Exhume's `examples/uw2/extract.py` and `link.py` (UW2Decomp's link): the rules are TLINK's and the same for both games, and every fact below was measured again from `UW.EXE`.

## Running it

You need your own `UW.EXE`, Exhume checked out beside this repository (or `EXHUME` set to it) with its Python environment, and the toolchain `exhume.toml` names (UW2Decomp's `TC` and `TASM` directories). From the repository root:

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
TLINK /c /m /s XORDER C0UW1 <resident objects> /o <overlay objects> /o-, uwedit.exe, uwedit.map, CM.LIB UWLIB.LIB OVERLAY.LIB
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

- **Resident:** C0, seg006 to seg015, seg016, seg000, seg019's first module (`STARTUP.ASM`), seg001 with the routine after it, seg002, seg020 (AIL), seg021 to seg031, seg017's C (`GRIDDB.C`), seg032 to seg044, then the far data. seg002 has no relocations, so its place is free.
- **Overlays:** ovr089 to ovr154 in order.
- **Second library (`UWLIB.LIB`),** after the C library and before the overlay manager. Its order is seg019's modules B to M, seg004's A to G, seg003's A, seg019's N to Q, seg045, seg004's H, seg003's B to G, seg004's I, and seg003's H to R. That is 46 modules with the two far data pieces below. UW1's seg003 has 18 modules, seg004 9 and seg019 17. Modules with no relocations sit next to their neighbours in the same segment.
- **Segment classes,** as in UW2: seg000 to seg002 are `ASMCODE` (code flag 1, first block), seg003 and seg004 are `code` (flag 0), and everything from C0's `_TEXT` on is `CODE`. The sources already had these classes, and the link confirms them.
- **Segments declared early:** XORDER declares seg000 to seg004 before C0's `_TEXT` and holds entry 5. XEMPTY16 holds entry 16, between seg014 and seg015. XSEG019 declares seg019 and seg020 on paragraphs and holds entry 23, after seg018 and before `STARTUP.ASM`.

### What still comes from your EXE

- **Code with no source:** seg016 (entry 18, 5Fh bytes, the divide-by-zero trap, which UW2 has as `INT0TRAP.ASM`), SetPnt (the first 35h bytes of entry 19, UW2's `SETPNT.ASM`), seg018 (entry 20, 6Dh bytes), and a far routine of B9h bytes that follows `VALLOC.ASM`'s code in seg001. The routine at seg001 has one relocation, entry 708, right after VALLOC's. These 442 bytes are the only code no matched source covers.
- **Far data:** 11 of the far data entries are the C files' own `far` variables (PATHFIND, AI, SOUND, SPELLS, GAMESTRN, BABL), which `verify.py` places and compares. Entry 52 (seg048) is filled from its first relocated pointer by the pieces in `POLYFILL`, `GRENTRY`, `SCALEBM` and `GRDISP`. The rest is extracted.
- **DGROUP:** 6 small `_DATA` gaps and 5 `_BSS` gaps between files that no source owns.
- **The 3D modules' far data:** INSTANCE's pieces of entries 72 and 58, and TMAPOPS's of entry 58, have no source. Their relocations (entries 2639, 2640 and 2709 to 2720) sit among those modules' own in the relocation table. So the bytes from each piece's first relocated pointer go into library modules of their own, `XP004G` and `XP004H`, linked just before INSTANCE and TMAPOPS. While those bytes sat in the resident far data module, 320 relocation entries were out of order.

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
- **TLINK does not clear the memory it builds the image in.** Under emu2 with its full 640 KB, 35 bytes in DGROUP came out as stale bytes of GAMESTRN's code once the XP modules were in the library. Those bytes are word-alignment padding and the part of SETARGV's `_DATA` no record covers. Under DOSBox-X, or emu2 limited to 512 KB (`EMU2_LOWMEM`), they are 0, as in `UW.EXE`, so `link.py` sets `EMU2_LOWMEM`. The earlier link had them as 0 under 640 KB too, so this depends on TLINK's memory use and should be checked again when the module list changes. UW2's modding build met the same effect in overlay padding.
- **Library modules nobody names are not linked.** UW2 never met this, because all its library modules were referenced.

## The modding build

`python3 tools/link.py --mod` works as UW2Decomp's does (Exhume's docs/link.md, "The modding build"). The exact link keeps a snapshot in `build/LINK/base`, and changed sources are compiled into `build/MODLINK/src`. With no source changed, the EXE in `build/MODLINK/out` equals the exact link's and `UW.EXE`. Nothing has been changed and booted through it yet. The layout audit (Exhume's `tools/addrscan.py` and skills/modding-build) has not been done for UW1 either, so addresses written as numbers are still unknown. Until it is done, a change that moves code or data may break the program in ways the link cannot see.
