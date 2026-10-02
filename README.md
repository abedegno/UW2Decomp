# UW2Decomp

The method and tools are generalised in [Exhume](https://github.com/abedegno/Exhume), a toolkit and set of Claude Code skills for byte-matching decompilation of old DOS games; this repository is its first case study.

A byte-matching decompilation of Ultima Underworld II: Labyrinth of Worlds (DOS, 1993; developed by Looking Glass Technologies, published by Origin Systems). The C in `src/` recompiles with Borland Turbo C++ 1.01 to the same machine code as the shipped `UW2.EXE`.

This is a fan research project, not affiliated with or endorsed by the rights holders. This repository holds no game data and no Borland software. You need your own copy of UW2 (the GOG release works) and the Turbo C++ 1.01 disk images, which Borland released free of charge.

## Status

**All of UW2's C code is matched: 337,327 of 337,327 bytes, in 99 source files under `src/`.** Every file compiles with Turbo C++ 1.01 to the same machine code as the shipped `UW2.EXE`, and `tools/verify.py` confirms its fixups (every call and global reference), its initialised data and its uninitialised data layout. Function and global names are the originals from the FM Towns build wherever it has them.

**All of the assembly is matched too: ten modules, 71,920 bytes, assembled with Turbo Assembler 2.0** and verified the same way. They are seg001 (screen memory and rectangle save/restore), seg002 (a run/skip/dump decoder), seg003 (graphics), seg004 (the 3D renderer, 386 code), seg017, seg020, seg021 (startup and input), seg022 (the Miles AIL 2.0 sound API), seg045 (now `SEG045.C`) and seg046 (Borland's overlay manager from OVERLAY.LIB). seg013 is C with inline assembly. Every code segment in `UW2.EXE` outside the C runtime library now rebuilds byte for byte.

## Linking

`make exact` (`python3 tools/link.py`) links `build/LINK/out/UW2.EXE` with TLINK in headless DOS and compares it with your `UW2.EXE` (`tools/exediff.py`). The result is the same size as the original, its header, overlay area and resident image are identical except for two bytes, and it runs: it reaches the title and main menu, and a changed string in a C source shows up in the game.

- **What the repo does not contain is taken from your own EXE at build time.** `tools/extract.py` writes data-only TASM modules under `build/LINK` (never committed) for the far data segments, the DGROUP gaps between files, and the code that has no source yet. Their publics are the names in `symbols.tsv`, and each relocation in them becomes a `dd`/`dw seg` fixup.
- **The link order comes from the EXE.** TLINK writes relocations module by module and lists every segment in the overlay manager's segment table, so both record the original order. seg000 to seg004 precede C0's `_TEXT`, and seg003, seg004 and seg045 come from a second library linked after `CM.LIB`.
- **TLINK stores some of its environment in the EXE.** The output name must be `uwedit.exe` (written into `__EXENAME__`) and the DOS date 12 May 1993 (`__EXEDATE__`). UW2's start-up code is `C0.ASM` with three small changes, which `link.py` applies to a copy.
- **Objects link exactly as compiled.** An overlay whose publics come out in the wrong order (Turbo C lists them by a hash of the name, and TLINK numbers stub entries from that order), an unknown name, a public UW2 had `static` or a missing relocation stops the link with the source to correct.
- **Original module structure:** seg003, seg004 and seg021 were libraries of 14, 14 and 17 assembly modules (`src/SEG003A..N.ASM`, `SEG004A..N`, `SEG021A..Q`), recovered from the relocation order and TLINK's zero padding between modules; seg045 is one Turbo C module.
- **Remaining difference: two bytes.** The linked EXE matches `UW2.EXE` byte for byte, relocation table order included, except the overlay segment table's code flag for seg003 and seg004 (1 where UW2 has 0). TLINK 3.01 sets that flag only for a class spelled exactly `CODE`; a class `Code` gives 0 but moves the segments, so the original's combination is still unexplained (possibly a different TLINK 3.0x).
- **Same-length source changes are safe.** The extracted modules keep literal DGROUP offsets, so a change that alters data sizes shifts data under them.
- `--obj STEM=PATH` links a changed object in place of the matched one.

Every byte of code now has source, including seg000 (the sprite module), seg018 (the divide-by-zero trap) and `SetPnt`. DGROUP's data is in the sources too, apart from eight small unreferenced gaps no evidence can attribute and the second library's data, which would need seg003, seg004 and seg021 split into their original modules. Of the 22 far data segments that hold bytes, 12 are `far` variables in the C files that defined them (Turbo C gives each its own segment, and their link order shows the owner; `verify.py` checks them like any other data) and 7, whose owner is unknown, come from `src/FARDATA.ASM`; the three left, about 83 KB, are the graphics and 3D modules' data, including the 3D object models, and the bytes before each module's first relocated pointer are still taken from your EXE.

`matched.txt` lists the matched segments; `map/files.tsv` has per-file status.

## Modding build

`make game` (`python3 tools/link.py --mod`) links an EXE from sources that may change by any size, in code or data, resident or overlay. It reaches the main menu, character creation and the 3D view with a longer overlay string and more overlay code, more resident code and initialised data, and a larger `_BSS`, each on its own and all together (docs/LAYOUT.md has the tests).

- **Run the exact link once first.** `python3 tools/link.py`, with every source matching, keeps in `build/LINK/base` a copy of each matched object, the SHA-1 of its source and where verify.py found its data (extract.py writes it only when every object verifies). The modding build works out the layout from those, so the extracted modules keep their places next to their neighbours whatever the changed objects do.
- **Then edit and link.** Each source whose text differs from that run's is compiled with its own `/* opts: */` into `build/MODLINK/src`; the matched objects in `build/` are left alone, so the exact link still works once you revert. The EXE goes to `build/MODLINK/out/UW2.EXE` and is not compared with yours. With no source changed it is the exact link's EXE.
- **What moves safely:** everything the linker places. Calls, globals, strings and pointer initialisers in the C are fixups; the extracted modules hold no relocated word and no DGROUP pointer; and the four DGROUP addresses the sources wrote as numbers are names now. In this build the model interpreter's opcode table in the extracted data is written as names too. `tools/addrscan.py` lists numbers that could be addresses, and docs/LAYOUT.md is the audit.
- **What is still fixed:** the internal layout of the far data segments seg_370D, seg052_519C and dseg062_62a6 (partly taken from your EXE, partly from the assembly) and of the code in seg003, seg004 and seg021, which the assembly addresses by number. Change those modules and `src/FARDATA.ASM` only at the same length. DGROUP has about 22 KB to spare. A new source file needs a place in extract.py's link order first.
- `--obj STEM=PATH` works here too, in place of the compiled object.

## The map

`map/` lays out the whole program. Turbo C puts each source file in its own code segment, so each DOS segment is one original source file. The Japanese FM Towns release kept 3237 of Looking Glass's original names, and it was linked from the same object list in the same order, so the two builds can be aligned function by function.

- `map/files.tsv`: every DOS code segment, whether it is C (99 segments, about 1480 functions and 337 KB), assembly (graphics, the 3D renderer and sound; `map/files.tsv` sizes come from IDA's function map and overstate them, the target tables have the true extents) or library, how much of it is named, and whether it is matched.
- `map/functions.tsv`: every DOS function with its original name where one was found, and how: `anchor` (proven), `confirmed` (aligned, and its callers and callees agree with the FM Towns call graph), `size only`, or `size, calls disagree`.
- 884 of the 1886 non-library functions have a confirmed original name. Tested by holding out known pairs, confirmed names were right 80 times out of 82, and both misses disagree with a hand-made anchor rather than a proven one.
- The rest are mostly DOS-only code with no FM Towns counterpart: the assembly, a few DOS-specific C files (ovr095 and the small resident segments seg011 to seg019), and a stretch at the very end (ovr158 onwards) past the last anchor.

`map/filenames.tsv` proposes original file names from the System Shock source release (Looking Glass, 1994, built on the Underworld engine): six strong candidates where a function name and the file's job both agree, eleven plausible ones. They are lineage names, not recovered ones, so the sources keep their segment names for now.

Rebuild it with `tools/doslist.py`, `tools/callpairs.py`, `tools/anchors.py`, `tools/callgraphs.py`, `tools/align.py` and `tools/files.py`, in that order; each describes itself.

`match.py` compares code bytes with fixups masked. `verify.py` then checks what that masks: every extern resolves to one address everywhere it is used and no two externs share one, every reference into the file's own code lands where it should, and the file's initialised data matches the EXE's data segment byte for byte. `symbols.tsv` is the resulting map of names to addresses in `UW2.EXE`, each marked as an original FM Towns name, a library routine or provisional.


## Building

You need Node 20 or later, Python 3, mtools and 7z (`brew install mtools p7zip`), the Turbo C++ 1.01 and Turbo Assembler 2.0 disk images, and UW2's `UW2.EXE` at `~/UWGOG/UW2/UW2.EXE` (or set `UW2_EXE`; the GOG release works).

- `make setup TC_DISKS=DIR TASM_DISKS=DIR` extracts Turbo C++ into `TC/` and TASM into `TASM/` from the directories holding their disk images and checks both are the expected builds, makes the `.venv` with `iced-x86` (for instruction diffs), and runs `npm install` (which fetches [dos-mcp](https://www.npmjs.com/package/dos-mcp) to run the tools headless in js-dos). It skips whatever is already in place, so it is safe to run again.
- `make` (or `make game`) is the modding build, `tools/link.py --mod`, and prints the path of the EXE.
- `make exact` links the matched objects exactly and compares the result with your `UW2.EXE`; it passes when only the two known bytes differ.
- `make check` is the gate. It compiles every source with a `/* target: */` line (several to a DOS session, three sessions at once), and requires `match.py` to report WHOLE SEGMENT MATCHES and `verify.py` "fixups and data verified" for each. It then rebuilds `symbols.tsv` from scratch in a scratch directory and requires the same names at the same addresses as the committed file, requires the exact link to equal `UW2.EXE` except the bytes at 0x6676C and 0x66774, and requires the modding build with no changes to be byte-identical to the exact link. It recompiles only the sources whose text or object has changed since they last passed (`build/check/state.json`); `make check-all` recompiles everything. On this machine `make check-all` takes about a minute and `make check` with nothing changed about ten seconds.
- `make boot` boots the modding build in headless DOS and saves screenshots of the title, the intro and the main menu under `build/boot/`. Look at them.
- `make hooks` installs a git pre-push hook that runs `make check` and stops the push when it fails. Hosted CI cannot run the gate, because the toolchain and the game cannot be on GitHub, so this hook is the gate. Bypass it for one push with `git push --no-verify` (or `SKIP_CHECK=1 git push`). It checks the working tree, not the commits being pushed, so commit or stash first. The GitHub workflow runs only `tools/repocheck.py`: script syntax, Markdown links, and that no game data or Borland binary is committed.

Optional: the Japanese FM Towns release of UW2, for `tools/fmt.py`, which disassembles a function by its original name. Extract `UW2.EXP` from the disc and use `uw2fmt.py` from UWReverseEngineering's `UW2 FM Towns` folder to write `fmtowns/uw2fmt.img` (`unpack`) and `fmtowns/syms.tsv` (`syms`). `tools/targets.py` reads the IDA listing `uw2_asm.asm` from [UWReverseEngineering](https://github.com/hankmorgan/UWReverseEngineering) (set `UW2_ASM` if it is not at `~/UWReverseEngineering/uw2_asm.asm`).

### Shared headers

The declarations the sources share live in `src/include/`, one header per subsystem: `sys.h` (start-up, the main loop, memory and EMS, errors, seg017's helpers), `file.h` (archives, file I/O, compression, saves), `object.h` (object lists, classes, use and look), `map.h` (tiles, textures, lighting, terrain collision), `player.h`, `critter.h`, `motion.h` (physics records and stepping), `combat.h` (combat, missiles, spells), `gfx.h`, `view3d.h`, `ui.h` (input, mouse, panels, the message scroll, strings, the automap), `inv.h`, `sound.h`, `conv.h` and `event.h` (world events, SCD schedules, traps). Every header includes `uw2.h`, which holds `union Link`, the link word that chains objects into lists. Each header declares the struct tags its declarations name and includes the headers that define them, so every tag a source sees is complete. A source includes the headers whose names it uses.

- **Where a declaration goes:** in the header of the subsystem that defines the name, in the section for its defining file, in the order that file first mentions the names. Functions are declared as the defining file defines them. The struct and union types live with the subsystem that owns them (`struct Object` in `object.h`, `struct Tile` in `map.h`, `struct Player` in `player.h`); a struct only one source looks inside stays in that source. Where files viewed the same bytes differently, the shared type keeps every view the code needs (`struct Object`'s `qn` is a word, two bitfields or a `union Link`), and a file that read a field with another type casts at that use.
- **Order matters.** A header declaration is the first sight of a name for every file that includes it, including the file that defines it. Turbo C orders a file's publics, and lays out its uninitialised globals, by a hash of the name with ties in order of first sight, so a name tied with a name the headers leave out must stay out too (the gate catches a wrong order: a verify `_BSS` problem or a link stop on the overlay stub order).
- **Some declarations stay in their files,** because the files that use the name compile differently with one shared type: a `char` parameter where another caller pushes an `int`, an old-style declaration, a different return type, a view of the same data through another type, or a pointer type that would draw warnings (seg029's object-list functions take `union Link far *`, and most callers pass `unsigned far *`). Each such file declares the name itself, and the name is in no header. Declarations of a file's own statics and file-local types stay in the file.
- **Building:** `tools/tcc.mjs` copies `src/include` into the DOS build directory beside the source, where `#include "name.h"` finds it (a header may not take the name of one of TC's own). `tools/srcdeps.py` hashes a source together with the headers it includes, so the gate, `link.py --mod` and the base layout recompile a source when one of its headers changes; editing a widely used header recompiles most of the tree (about a minute).
- **Adding a name:** declare it in its subsystem's header and delete the file's own declarations of it, then run `make check`. If a file's bytes change, that file needs the name declared its own way: put the declaration back in that file and take the name out of the header.

### Working on one file

`.venv/bin/python tools/match.py src/PLAYER.C` compiles the file and reports every function as MATCH or where it differs. `--dis NAME` shows an instruction diff. `.venv/bin/python tools/verify.py src/PLAYER.C --update` then checks fixups and data, and merges the file's externs into `symbols.tsv`, refusing any conflict. See [MATCHING.md](MATCHING.md) for the compiler switches and what the compiler's output reveals about the original source.

## Contributing

Every change must pass `make check` before it is pushed; `make hooks` makes that automatic. A readability change (names, shared headers, `#define`s and enums, struct fields, comments, file renames) must keep the bytes identical, and the gate passing is the proof. [Shared headers](#shared-headers) says where a declaration goes. Renaming a function or global also renames it in `symbols.tsv`: change it in every file that uses it, replace its line in `symbols.tsv`, and the gate's from-scratch rebuild of `symbols.tsv` shows anything missed. A change meant to alter the program is for the modding build (see above), not the matched sources.
