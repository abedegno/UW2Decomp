# Linking

There are two links. The exact link (`make exact`, `tools/link.py`) rebuilds `UW2.EXE` from the matched objects and proves the sources. The modding build (`make`, `tools/link.py --mod`) links sources that may change by any size. Both run TLINK 3.01 in headless DOS.

## The exact link

`tools/link.py` links `build/LINK/out/UW2.EXE` and compares it with your `UW2.EXE` (`tools/exediff.py`). The result is the same size as the original, its header, overlay area and resident image are identical except for two bytes, and it runs: it reaches the title and main menu, and a changed string in a C source shows up in the game.

- **What the repository does not contain is taken from your own EXE at build time.** `tools/extract.py` writes data-only TASM modules under `build/LINK` (never committed) for the far data segments, the DGROUP gaps between files, and any code with no source. Their publics are the names in `symbols.tsv`, and each relocation in them becomes a `dd`/`dw seg` fixup.
- **The link order comes from the EXE.** TLINK writes relocations module by module and lists every segment in the overlay manager's segment table, so both record the original order. seg000 to seg004 precede C0's `_TEXT`, and seg003, seg004 and seg045 come from a second library linked after `CM.LIB`. A new source file needs a place in extract.py's link order before it can be linked.
- **TLINK stores some of its environment in the EXE.** The output name must be `uwedit.exe` (written into `__EXENAME__`) and the DOS date 12 May 1993 (`__EXEDATE__`). UW2's start-up code is `C0.ASM` with three small changes, which `link.py` applies to a copy.
- **Objects link exactly as compiled.** An overlay whose publics come out in the wrong order (Turbo C lists them by a hash of the name, and TLINK numbers stub entries from that order), an unknown name, a public UW2 had `static`, or a missing relocation stops the link and names the source to correct.
- **Original module structure.** seg003, seg004 and seg021 were libraries of 14, 14 and 17 assembly modules (`src/gfx/GRMISC.ASM` to `GRLIBN.ASM`, `src/3d/EXPAND.ASM` to `PGCACHE.ASM`, `src/sys/STARTUP.ASM` to `C3DENTRY.ASM`; `map/filenames.tsv` lists them in module order), recovered from the relocation order and TLINK's zero padding between modules. seg045 is one Turbo C module.
- `--obj STEM=PATH` links a changed object in place of the matched one.

### The two bytes

The linked EXE matches `UW2.EXE` byte for byte, relocation table order included, except the overlay segment table's code flag for seg003 and seg004 (file offsets 0x6676C and 0x66774: 1 where UW2 has 0). TLINK 3.01 sets that flag only for a class spelled exactly `CODE`; a class `Code` gives 0 but moves the segments, so the original's combination is still unexplained. It may have been a different TLINK 3.0x.

### What still comes from your EXE

Every byte of code has source, including seg000 (the sprite module), seg018 (the divide-by-zero trap) and `SetPnt`. DGROUP's data is in the sources too, apart from eight small unreferenced gaps no evidence can attribute and the second library's data.

Of the 22 far data segments that hold bytes, 12 are `far` variables in the C files that defined them (Turbo C gives each its own segment, and their link order shows the owner; `verify.py` checks them like any other data), and 7, whose owner is unknown, come from `src/sys/FARDATA.ASM`. The three left, about 83 KB, are the graphics and 3D modules' data, including the 3D object models; the bytes before each module's first relocated pointer are still taken from your EXE.

### Segment names

Turbo C names a file's code segment `FILE_TEXT` and each `far` variable's segment `FILE<n>_FAR`, and TASM does the same for an assembly module written with `.model` and `.code` (`VALLOC_TEXT`, `AIL_TEXT`). Other assembly modules name their segments themselves and keep the names they had (`SEG003_TEXT`, shared by seg003's 14 modules). The EXE stores no segment names, and TLINK places segments in the order it first meets them, so names only have to agree where two modules share a segment: `3d/SETPNT.ASM` declares `GRIDDB_TEXT`, the code segment of the C file it shares seg019 with. `tools/extract.py` takes the names it declares early (seg000 to seg004, seg020 to seg022) from the objects. This is why renaming a source file changes its object but not the EXE.

## The modding build

`make game` (`tools/link.py --mod`) links an EXE from sources that may change by any size, in code or data, resident or overlay. It reaches the main menu, character creation and the 3D view with a longer overlay string and more overlay code, more resident code and initialised data, and a larger `_BSS`, each on its own and all together ([LAYOUT.md](LAYOUT.md#proof) has the tests).

- **Run the exact link once first.** `python3 tools/link.py`, with every source matching, keeps in `build/LINK/base` a copy of each matched object, the SHA-1 of its source and where verify.py found its data (extract.py writes it only when every object verifies). The modding build works out the layout from those, so the extracted modules keep their places next to their neighbours whatever the changed objects do.
- **Then edit and link.** Each source whose text differs from that run's is compiled with its own `/* opts: */` into `build/MODLINK/src`. The matched objects in `build/` are left alone, so the exact link still works once you revert. The EXE goes to `build/MODLINK/out/UW2.EXE` and is not compared with yours. With no source changed it is the exact link's EXE, and the gate checks that.
- **What moves safely:** everything the linker places. Calls, globals, strings and pointer initialisers in the C are fixups; the extracted modules hold no relocated word and no DGROUP pointer; and the four DGROUP addresses the sources wrote as numbers are names now. In this build the model interpreter's opcode table in the extracted data is written as names too (from the IDA listing, when present). `tools/addrscan.py` lists numbers that could be addresses, and [LAYOUT.md](LAYOUT.md) is the audit.
- **What is still fixed:** the internal layout of the far data segments seg_370D, seg052_519C and dseg062_62a6 (partly taken from your EXE, partly from the assembly), and of the code in seg003, seg004 and seg021, which the assembly addresses by number. Change those modules and `src/sys/FARDATA.ASM` only at the same length. DGROUP has about 22 KB to spare.
- **Same-length changes only in the exact link.** Its extracted modules keep literal DGROUP offsets, so a change that alters data sizes shifts data under them. Use the modding build for anything else.
- `--obj STEM=PATH` works here too, in place of the compiled object.
