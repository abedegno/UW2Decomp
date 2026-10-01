# UW2Decomp

A byte-matching decompilation of Ultima Underworld II: Labyrinth of Worlds (DOS, 1993; developed by Looking Glass Technologies, published by Origin Systems). The C in `src/` recompiles with Borland Turbo C++ 1.01 to the same machine code as the shipped `UW2.EXE`.

This is a fan research project, not affiliated with or endorsed by the rights holders. This repository holds no game data and no Borland software. You need your own copy of UW2 (the GOG release works) and the Turbo C++ 1.01 disk images, which Borland released free of charge.

## Status

**All of UW2's C code is matched: 337,327 of 337,327 bytes, in 99 source files under `src/`.** Every file compiles with Turbo C++ 1.01 to the same machine code as the shipped `UW2.EXE`, and `tools/verify.py` confirms its fixups (every call and global reference), its initialised data and its uninitialised data layout. Function and global names are the originals from the FM Towns build wherever it has them.

Also matched: seg013 (C with inline assembly, through Turbo Assembler 2.0) and the assembly modules seg001 (screen memory and rectangle save/restore), seg002 (a run/skip/dump decoder), seg003 (graphics), seg017, seg020, seg021 (startup and input), seg022 (the Miles AIL 2.0 sound API), seg045 (compiled C, kept as assembly for now) and seg046 (Borland's overlay manager from OVERLAY.LIB).

Still to do: seg004, the 3D renderer (about 33 KB of 386 assembly). `tools/asmgen.py --fix` drafts it from the EXE. They match Turbo Assembler 2.0's output; see MATCHING.md.

`matched.txt` lists the matched segments; `map/files.tsv` has per-file status.

## The map

`map/` lays out the whole program. Turbo C puts each source file in its own code segment, so each DOS segment is one original source file. The Japanese FM Towns release kept 3237 of Looking Glass's original names, and it was linked from the same object list in the same order, so the two builds can be aligned function by function.

- `map/files.tsv`: every DOS code segment, whether it is C (99 segments, about 1480 functions and 337 KB), assembly (7 segments, 88 KB: graphics, the 3D renderer and sound) or library, how much of it is named, and whether it is matched.
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
