# UW2Decomp

A byte-matching decompilation of Ultima Underworld II: Labyrinth of Worlds (DOS, 1993; developed by Looking Glass Technologies, published by Origin Systems). The C in `src/` recompiles with Borland Turbo C++ 1.01 to the same machine code as the shipped `UW2.EXE`.

This is a fan research project, not affiliated with or endorsed by the rights holders. This repository holds no game data and no Borland software. You need your own copy of UW2 (the GOG release works) and the Turbo C++ 1.01 disk images, which Borland released free of charge.

## Status

| Segment | Source | Functions | Bytes matched |
|---|---|---|---|
| ovr154 (skills, sleep, dreams, death, traps) | `src/PLAYER.C` | 26/26 | 6967/6967 code, 50/50 data; all 464 fixups verified |

Function names come from the symbol table in the FM Towns release of UW2, which kept 3237 of Looking Glass's original names. Both builds list functions in the same order, which gives the original grouping of functions into source files.

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
