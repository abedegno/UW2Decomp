# UW2Decomp

A byte-matching decompilation of Ultima Underworld II: Labyrinth of Worlds (DOS, 1993; developed by Looking Glass Technologies, published by Origin Systems). The C in `src/` recompiles with Borland Turbo C++ 1.01 to the same machine code as the shipped `UW2.EXE`.

This is a fan research project, not affiliated with or endorsed by the rights holders. This repository holds no game data and no Borland software. You need your own copy of UW2 (the GOG release works) and the Turbo C++ 1.01 disk images, which Borland released free of charge.

## Status

**All of UW2's C code is matched: 337,327 of 337,327 bytes, in 99 source files under `src/`.** Every file compiles with Turbo C++ 1.01 to the same machine code as the shipped `UW2.EXE`, and `tools/verify.py` confirms its fixups (every call and global reference), its initialised data and its uninitialised data layout. Function and global names are the originals from the FM Towns build wherever it has them.

**All of the assembly is matched too: ten modules, 71,920 bytes, assembled with Turbo Assembler 2.0** and verified the same way. They are seg001 (screen memory and rectangle save/restore), seg002 (a run/skip/dump decoder), seg003 (graphics), seg004 (the 3D renderer, 386 code), seg017, seg020, seg021 (startup and input), seg022 (the Miles AIL 2.0 sound API), seg045 (now `SEG045.C`) and seg046 (Borland's overlay manager from OVERLAY.LIB). seg013 is C with inline assembly. Every code segment in `UW2.EXE` outside the C runtime library now rebuilds byte for byte.

## Linking

`python3 tools/link.py` links `build/LINK/out/UW2.EXE` with TLINK in headless DOS and compares it with your `UW2.EXE` (`tools/exediff.py`). The result is the same size as the original, its header, overlay area and resident image are identical except for two bytes, and it runs: it reaches the title and main menu, and a changed string in a C source shows up in the game.

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

## The map

`map/` lays out the whole program. Turbo C puts each source file in its own code segment, so each DOS segment is one original source file. The Japanese FM Towns release kept 3237 of Looking Glass's original names, and it was linked from the same object list in the same order, so the two builds can be aligned function by function.

- `map/files.tsv`: every DOS code segment, whether it is C (99 segments, about 1480 functions and 337 KB), assembly (graphics, the 3D renderer and sound; `map/files.tsv` sizes come from IDA's function map and overstate them, the target tables have the true extents) or library, how much of it is named, and whether it is matched.
- `map/functions.tsv`: every DOS function with its original name where one was found, and how: `anchor` (proven), `confirmed` (aligned, and its callers and callees agree with the FM Towns call graph), `size only`, or `size, calls disagree`.
- 884 of the 1886 non-library functions have a confirmed original name. Tested by holding out known pairs, confirmed names were right 80 times out of 82, and both misses disagree with a hand-made anchor rather than a proven one.
- The rest are mostly DOS-only code with no FM Towns counterpart: the assembly, a few DOS-specific C files (ovr095 and the small resident segments seg011 to seg019), and a stretch at the very end (ovr158 onwards) past the last anchor.

`map/filenames.tsv` proposes original file names from the System Shock source release (Looking Glass, 1994, built on the Underworld engine): six strong candidates where a function name and the file's job both agree, eleven plausible ones. They are lineage names, not recovered ones, so the sources keep their segment names for now.

Rebuild it with `tools/doslist.py`, `tools/callpairs.py`, `tools/anchors.py`, `tools/callgraphs.py`, `tools/align.py` and `tools/files.py`, in that order; each describes itself.

`match.py` compares code bytes with fixups masked. `verify.py` then checks what that masks: every extern resolves to one address everywhere it is used and no two externs share one, every reference into the file's own code lands where it should, and the file's initialised data matches the EXE's data segment byte for byte. `symbols.tsv` is the resulting map of names to addresses in `UW2.EXE`, each marked as an original FM Towns name, a library routine or provisional.

`src/THEME.C` and `src/CYCLE.C` are the first spike functions from other segments; they have no target tables yet.

## Setup

- Node 20+ and `npm install`, which fetches [dos-mcp](https://www.npmjs.com/package/dos-mcp) to run the compiler headless in js-dos.
- `tools/setup-tc.sh DIR` extracts Turbo C++ 1.01 from `Disk01.img`..`Disk04.img` into `TC/` and checks it is the expected build. Needs mtools and 7z.
- Python 3 with `iced-x86` for the disassembly diffs: `python3 -m venv .venv && .venv/bin/pip install iced-x86`.
- UW2's `UW2.EXE` at `~/UWGOG/UW2/UW2.EXE`, or set `UW2_EXE`. `tools/targets.py` also reads the IDA listing `uw2_asm.asm` from [UWReverseEngineering](https://github.com/hankmorgan/UWReverseEngineering) (set `UW2_ASM` if it isn't at `~/UWReverseEngineering/uw2_asm.asm`).
- Optional: the Japanese FM Towns release of UW2, for `tools/fmt.py`, which disassembles a function by its original name. Extract `UW2.EXP` from the disc and use `uw2fmt.py` from UWReverseEngineering's `UW2 FM Towns` folder to write `fmtowns/uw2fmt.img` (`unpack`) and `fmtowns/syms.tsv` (`syms`).

## Use

`.venv/bin/python tools/match.py src/PLAYER.C` compiles the file and reports every function as MATCH or where it differs. `--dis NAME` shows an instruction diff. `.venv/bin/python tools/verify.py src/PLAYER.C --update` then checks fixups and data, and merges the file's externs into `symbols.tsv`, refusing any conflict. See [MATCHING.md](MATCHING.md) for the compiler switches and what the compiler's output reveals about the original source.
