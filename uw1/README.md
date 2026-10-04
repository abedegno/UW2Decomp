# UW1Decomp

Ultima Underworld: The Stygian Abyss (DOS, 1992; developed by Blue Sky Productions, later Looking Glass Technologies, published by Origin Systems), as source code: a byte-matching decompilation. The C and assembly in `src/` compile with the original Borland tools to the same bytes as the shipped `UW.EXE`.

It is the sibling of [UW2Decomp](https://github.com/abedegno/UW2Decomp), the matched decompilation of Ultima Underworld II, and was seeded from it: the two games share a compiler, libraries and much of an engine, a year apart. It is built with [Exhume](https://github.com/abedegno/Exhume), the toolkit both projects use.

No game data is included. You need your own copy of UW1 (the GOG release). This is a fan research project, not affiliated with or endorsed by the rights holders; see [Licence and legal notice](#licence-and-legal-notice).

## Status

Every code segment is matched. Each source compiles with Turbo C++ 1.01 (C) or assembles with Turbo Assembler 2.0 (assembly) to its segment's bytes in `UW.EXE`, and its fixups, data and names are verified: 353,727 of the program's 369,167 bytes of code, in 89 C files and 50 assembly modules. The rest is Borland's C runtime library, which the link takes from the library, as UW2Decomp's does.

The relinked EXE is identical to `UW.EXE` (the same MD5) and boots to the main menu, and the modding build with no change gives the same EXE ([LINKING.md](docs/LINKING.md)). What no source holds yet (four small pieces of code, 442 bytes, and the far data and DGROUP gaps no source owns) is extracted from your own `UW.EXE` at build time and never committed.

Still to do ([docs/PLAN.md](docs/PLAN.md)):

1. The rest of the data as source.
2. The gate, the layout audit for the modding build, and the readability pass: shared headers that match UW1 (the matched files still declare some of UW1's differences from UW2 locally), named constants and struct fields, and notes on what each file does in the game.
3. A port, on the same port layer as UW2Decomp's.

[docs/NOTES.md](docs/NOTES.md) collects what matching found: where UW1 differs from UW2 (an earlier sound library, a graphics library of 18 modules where UW2 has 14, a 3D renderer of 9 modules with no 386 code, an options panel and screen-frame dragons of its own), the module boundaries, and items for the readability pass.

## Names

UW1 has no build with symbols. Its names come from UW2's matched sources, whose names are the FM Towns build's originals: a UW1 function whose code pairs with a UW2 function (`tools/kin.py` in Exhume, `map/kin.tsv`) takes that name. Where the overlay manager's stub order rules a name out (the linker numbers an overlay's entries in the order of a hash of their names), a function takes a descriptive name whose hash fits, and the source says so.

## Building

This needs your own `UW.EXE`, the Turbo C++ 1.01 disk images (which Borland released free of charge) and the Turbo Assembler 2.0 disk image, Python 3 with Exhume's requirements, and the toolchain set up as UW2Decomp's `make setup` does. None of these is in the repository.

`exhume.toml` names where things are, each overridable from the environment: `UW1_EXE` (default `~/UWGOG/UW1/UW.EXE`), `UW1_ASM` (the disassembly listing that the map was made from), and `EXHUME_TC` and `EXHUME_TASM` (the Borland toolchain). With Exhume checked out:

```sh
python3 path/to/Exhume/tools/match.py src/ui/OPTIONS.C     # compile in DOS and compare with the segment
python3 path/to/Exhume/tools/verify.py src/ui/OPTIONS.C    # fixups, data and names
python3 path/to/Exhume/tools/build.py --all                # every object
python3 tools/link.py                                      # build/LINK/out/UW.EXE, compared with yours
python3 tools/link.py --mod                                # the modding build
```

The tools find Exhume through `$EXHUME`, or a checkout named `Exhume` next to this one.

`python3 tools/repocheck.py` runs the checks CI runs on every push: no game data or Borland binary is committed, and the Markdown links resolve.

## Licence and legal notice

The project's own work (the tools, the documentation, and the comments, names and structure added to the decompiled code) is under the MIT licence ([LICENSE](LICENSE)). The decompiled code is derived from `UW.EXE`, whose copyright belongs to its owners; the licence grants no right in it. [NOTICE](NOTICE) has the details, including the Borland and Miles code in the repository.
