# Linking

There are two links. The exact link (`make exact`, `tools/link.py`) rebuilds `UW2.EXE` from the matched objects and proves the sources. The modding build (`make`, `tools/link.py --mod`) links sources that may change by any size. Both run TLINK 3.01 in headless DOS.

## The exact link

`tools/link.py` links `build/LINK/out/UW2.EXE` and compares it with your `UW2.EXE` (`tools/exediff.py`). The result is byte-identical to the original (`cmp` finds no difference), and it runs: it reaches the title and main menu, and a changed string in a C source shows up in the game.

- **What the repository does not contain is taken from your own EXE at build time.** `tools/extract.py` writes data-only TASM modules under `build/LINK` (never committed) for the far data segments, the DGROUP gaps between files, and any code with no source. Their publics are the names in `symbols.tsv`, and each relocation in them becomes a `dd`/`dw seg` fixup.
- **The link order comes from the EXE.** TLINK writes relocations module by module and lists every segment in the overlay manager's segment table, so both record the original order. seg000 to seg004 precede C0's `_TEXT`, and seg003, seg004 and seg045 come from a second library linked after `CM.LIB`. A new source file needs a place in extract.py's link order before it can be linked.
- **TLINK stores some of its environment in the EXE.** The output name must be `uwedit.exe` (written into `__EXENAME__`) and the DOS date 12 May 1993 (`__EXEDATE__`). UW2's start-up code is `C0.ASM` with three small changes, which `link.py` applies to a copy.
- **Objects link exactly as compiled.** An overlay whose publics come out in the wrong order (Turbo C lists them by a hash of the name, and TLINK numbers stub entries from that order), an unknown name, a public UW2 had `static`, or a missing relocation stops the link and names the source to correct.
- **Original module structure.** seg003, seg004 and seg021 were libraries of 14, 14 and 17 assembly modules (`src/gfx/GRMISC.ASM` to `GRLIBN.ASM`, `src/3d/EXPAND.ASM` to `PGCACHE.ASM`, `src/sys/STARTUP.ASM` to `C3DENTRY.ASM`; `map/filenames.tsv` lists them in module order), recovered from the relocation order and TLINK's zero padding between modules. seg045 is one Turbo C module.
- `--obj STEM=PATH` links a changed object in place of the matched one.

### Segment classes

The code segments came in three classes, and that is what made the overlay segment table's code flags for seg003 and seg004 (file offsets 0x6676C and 0x66774) 0 where every other code segment has 1. Until the classes were found, the exact link differed from `UW2.EXE` in those two bytes and nowhere else.

TLINK uses a segment's class in two ways, both read from TLINK 3.01's code (offsets in its load image):

- **Order.** TLINK lays segments out in blocks by class, the classes in the order it first meets them, and each block in the order it first meets each segment name. A test link of three segments with classes `CODE`, `XC` and `CODE`, in that order, comes out A, C, B.
- **The code flag.** TLINK groups the segments into physical frames (0x5A94 to 0x5E0E), and the first segment to create a frame sets the frame's type. The classifier at 0x7FEF returns 1 for a class that ends in `CODE`, in upper case: TLINK compares the bytes, so `Code` and `code` do not count. The writer of the overlay segment table at 0x6BAA sets bit 0 of an entry's flags when its frame's type is above 0. With `/c` (case-sensitive linking, which TCC passes) a class `Code` is also a different class from `CODE`, so it gets its own block.

So the original link had three blocks, in this order:

| Block | Segments | Class |
|---|---|---|
| K1 | seg000 to seg002 (`gfx/SPRITE.ASM`, `VALLOC.ASM`, `LPFDELTA.ASM`) | ends in `CODE`, but is not `CODE`, so it comes first and keeps the flag |
| K2 | seg003 and seg004 (the 14 `src/gfx` modules that declare `SEG003_TEXT` and the 14 `src/3d` modules that declare `SEG004_TEXT`) | does not end in upper-case `CODE`, so the flag is 0 |
| K3 | XEMPTY05, C0's `_TEXT` and everything after it, every C file included | `CODE` |

That much is proved. K1 has to be a class other than `CODE` (as `CODE` it would join K3's block, which would then come first) that still ends in upper-case `CODE` (for the flag). K2 has to be a class that does not end in upper-case `CODE`. K3 is `CODE`, the class Turbo C gives every C file. The EXE stores no class names, so the spellings are a choice. The sources use `ASMCODE` for K1 and `code` for K2. Lower-case `code` is how a period `segment 'code'` comes out of TASM with `/ml` (or MASM with `/Ml`); MASM 5.1 without `/Ml` upper-cases class names, which would have made it `CODE`. Other spellings link to the same EXE (`XCODE` and `GRAPH`, `LIBCODE` and `code`, `FAR_CODE` and `Code`, and separate classes for the graphics and 3D modules all give 0 differences), while making K1 `CODE` too, with K2 `GRAPH`, moves seg000 to seg002 behind seg004 and changes 228,676 bytes. The overlay manager's own `_OVRTEXT_` (in `OVERLAY.LIB`) has class `CODE` and flag 1, which fits.

`VALLOC.ASM` and `LPFDELTA.ASM` used `.model medium` and `.code`, and TASM does not let a segment that `.model` declares take another class, so they declare their segments themselves: `VALLOC_TEXT segment word public 'ASMCODE'`, and after it the empty `_DATA` and the `DGROUP` group that `.model` used to add. Their objects are unchanged apart from the class name. `tools/extract.py` gives the segments it declares early in `XORDER` (seg000 to seg004) the class of the object that fills each, and the tools find an object's code segment as the one whose class ends in `CODE`, in any case.

The linker version cannot be told from this. TLINK 3.0 also links the same EXE once C0's `_DATA` is word-aligned (with C0's paragraph-aligned `_DATA` it puts `_DATA` at file 65EA0, where 3.01 puts it at 65E94), and TLINK 4.0, from Borland C++ 2.0, links the same EXE as well. The build uses TLINK 3.01, the one in Turbo C++ 1.01.

### What still comes from your EXE

Every byte of code has source, including seg000 (the sprite module), seg018 (the divide-by-zero trap) and `SetPnt`. DGROUP's data is in the sources too, apart from eight small unreferenced gaps no evidence can attribute and the second library's data.

Of the 22 far data segments that hold bytes, 12 are `far` variables in the C files that defined them (Turbo C gives each its own segment, and their link order shows the owner; `verify.py` checks them like any other data), and 7, whose owner is unknown, come from `src/sys/FARDATA.ASM`. The three left, about 83 KB, are the graphics and 3D modules' data, including the 3D object models; the bytes before each module's first relocated pointer are still taken from your EXE.

### Segment names

Turbo C names a file's code segment `FILE_TEXT` and each `far` variable's segment `FILE<n>_FAR`, and TASM does the same for an assembly module written with `.model` and `.code` (`MODEX_TEXT`, `AIL_TEXT`). Other assembly modules name their segments themselves and keep the names they had (`SEG003_TEXT`, shared by seg003's 14 modules; `VALLOC.ASM` and `LPFDELTA.ASM` keep `VALLOC_TEXT` and `LPFDELTA_TEXT`, the names `.model` gave them before they needed their own class). The EXE stores no segment names, and TLINK places segments in the order it first meets them, so names only have to agree where two modules share a segment: `3d/SETPNT.ASM` declares `GRIDDB_TEXT`, the code segment of the C file it shares seg019 with. `tools/extract.py` takes the names it declares early (seg000 to seg004, seg020 to seg022) from the objects. This is why renaming a source file changes its object but not the EXE.

## The modding build

`make game` (`tools/link.py --mod`) links an EXE from sources that may change by any size, in code or data, resident or overlay. It reaches the main menu, character creation and the 3D view with a longer overlay string and more overlay code, more resident code and initialised data, and a larger `_BSS`, each on its own and all together ([LAYOUT.md](LAYOUT.md#proof) has the tests).

- **Run the exact link once first.** `python3 tools/link.py`, with every source matching, keeps in `build/LINK/base` a copy of each matched object, the SHA-1 of its source and where verify.py found its data (extract.py writes it only when every object verifies). The modding build works out the layout from those, so the extracted modules keep their places next to their neighbours whatever the changed objects do.
- **Then edit and link.** Each source whose text differs from that run's is compiled with its own `/* opts: */` into `build/MODLINK/src`. The matched objects in `build/` are left alone, so the exact link still works once you revert. The EXE goes to `build/MODLINK/out/UW2.EXE` and is not compared with yours. With no source changed it is the exact link's EXE, and the gate checks that. One thing is evened out first: TLINK fills the paragraph padding after each overlay from a buffer it does not clear. In `UW2.EXE` and the exact link those bytes are all 0, but the modding link, whose extracted data names the model interpreter's handlers in seg004 where the exact link has numbers, leaves 4 bytes of old code after ovr167's fixups now that seg004 has a class of its own. Nothing reads them (the overlay manager loads an overlay's code and fixups by the sizes in its stub), so `link.py --mod` sets the padding to 0.
- **What moves safely:** everything the linker places. Calls, globals, strings and pointer initialisers in the C are fixups; the extracted modules hold no relocated word and no DGROUP pointer; and the four DGROUP addresses the sources wrote as numbers are names now. In this build the model interpreter's opcode table in the extracted data is written as names too (from the IDA listing, when present). `tools/addrscan.py` lists numbers that could be addresses, and [LAYOUT.md](LAYOUT.md) is the audit.
- **What is still fixed:** the internal layout of the far data segments seg_370D, seg052_519C and dseg062_62a6 (partly taken from your EXE, partly from the assembly), and of the code in seg003, seg004 and seg021, which the assembly addresses by number. Change those modules and `src/sys/FARDATA.ASM` only at the same length. DGROUP has about 22 KB to spare.
- **Same-length changes only in the exact link.** Its extracted modules keep literal DGROUP offsets, so a change that alters data sizes shifts data under them. Use the modding build for anything else.
- `--obj STEM=PATH` works here too, in place of the compiled object.
