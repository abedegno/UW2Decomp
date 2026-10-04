# UW1Decomp: plan and state

A byte-matching decompilation of Ultima Underworld: The Stygian Abyss (UW.EXE, DOS, 1992, the GOG release, MD5 8afc3c29ef5667a3ab6667e8cbc8d63a), built with [Exhume](https://github.com/abedegno/Exhume) and seeded from [UW2Decomp](https://github.com/abedegno/UW2Decomp), the matched decompilation of its sequel.

## What is known

- Toolchain: Turbo C++ 1.01, TASM 2.0, TLINK 3.01, medium model, VROOMM overlays, the same as UW2 (fingerprint.py: all four signature strings). Proved: UW2's `src/sys/UTIL.C`, with only its target line changed, compiles to the whole of UW1's seg041 (`-mm -1 -G -O -Y -d`).
- The map, from UWReverseEngineering's `UW1_asm.asm` (input MD5 as above): 111 segments, 72 resident and 66 overlays (12 of them empty), 1825 functions, 369 KB of code. IDA's segment names carry the load paragraph plus 0x1ED, as UW2's do (DGROUP `dseg_5c99` is paragraph 0x5AAC). IDA left 8 overlays without code and seg004 unplaced (it starts with a data table, as UW2's does); `tools/segmap.py` places them from the VROOMM segment table and the overlay stubs.
- Kin with UW2 (`tools/kin.py`, map/kin.tsv): by bytes, 16% of UW1's code is the same as a UW2 function once addresses are masked, 34% near (90% or more), 30% like (60% or more), 18% none. 97 of the 99 segments with code take most of their kin from one UW2 source file (map/seeds.tsv); seg002 and ovr141 have none. Names come from UW2's matched sources through the kin (map/functions.tsv, how `kin-same` or `kin-near`); UW1 has no build with symbols (its FM Towns release is stripped).

## Method

For each segment: copy its UW2 source (map/seeds.tsv) to the same path here, change its target line, and match (skills/match-file, match-asm). UW1's headers start as a copy of UW2's `src/include`; a file that needs a UW1 difference declares it locally, with a comment, and the readability pass reconciles the headers later. Three agents at a time, one file each, never nested; the orchestrator merges one file at a time and commits.

## Milestones

1. Map, kin, seeds, the toolchain proof. Done.
2. Match every segment (C, then assembly). Done 2026-10-04: every code segment but seg005 (Borland's C runtime library, which the link takes from CM.LIB, as UW2's) matches whole and verifies, 353,727 of 369,167 bytes; seg003, seg004 and seg019 as per-module tables (docs/NOTES.md).
3. Data to source, the link, an EXE byte-identical to UW.EXE (Exhume's link stage: UW2's examples/uw2/link.py as the reference).
4. The gate, the modding build, the readability pass.
5. The port, on Exhume's runtime and UW2Decomp's port layer.
