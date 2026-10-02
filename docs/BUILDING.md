# Building

Everything runs on your own machine: Turbo C++, TASM and TLINK run headless in js-dos through the [dos-mcp](https://www.npmjs.com/package/dos-mcp) npm package, and every build is compared with your own `UW2.EXE`. The Makefile only names the entry points; the logic is in `tools/uw2.py`.

## Requirements

- Node 20 or later, Python 3, mtools and 7z (`brew install mtools p7zip` on macOS).
- The Turbo C++ 1.01 disk images (four 720K images, which Borland released free of charge) and the Turbo Assembler 2.0 disk image. Neither is in this repository.
- UW2's `UW2.EXE` at `~/UWGOG/UW2/UW2.EXE`, or set `UW2_EXE`. The GOG release works. No game data is in this repository either.

Optional, for the map tools and the assembly drafts (see [MAP.md](MAP.md)):

- The IDA listing `uw2_asm.asm` from [UWReverseEngineering](https://github.com/hankmorgan/UWReverseEngineering), expected at `~/UWReverseEngineering/uw2_asm.asm` (or set `UW2_ASM`). It is large and the shell's grep may skip it as binary; use `grep -a`. The modding build uses it, when present, to write the 3D model interpreter's opcode table as names.
- The Japanese FM Towns release of UW2, for `tools/fmt.py`, which disassembles a function by its original name. Extract `UW2.EXP` from the disc and use `uw2fmt.py` from UWReverseEngineering's `UW2 FM Towns` folder to write `fmtowns/uw2fmt.img` (`unpack`) and `fmtowns/syms.tsv` (`syms`). `verify.py --update` reads `fmtowns/syms.tsv`, when present, to mark names in `symbols.tsv` as original.

## Make targets

- `make setup TC_DISKS=DIR TASM_DISKS=DIR` extracts Turbo C++ into `TC/` and TASM into `TASM/` from the directories holding their disk images and checks both are the expected builds (by MD5), makes `.venv` with `iced-x86` (for instruction diffs), and runs `npm install`. It skips whatever is already in place, so it is safe to run again.
- `make` (or `make game`) is the modding build, `tools/link.py --mod`, and prints the path of the EXE. See [LINKING.md](LINKING.md#the-modding-build).
- `make exact` links the matched objects exactly (`tools/link.py`) and compares the result with your `UW2.EXE` (`tools/exediff.py`); it passes when only the two known bytes differ.
- `make check` is the gate, below. `make check-all` is the same with every source recompiled.
- `make boot` boots the modding build in headless DOS and saves screenshots of the title, the intro and the main menu under `build/boot/`. Look at them.
- `make hooks` installs a git pre-push hook that runs `make check` and stops the push when it fails.
- `make port-check` compiles every C source for the host with Apple clang, compile only, and summarises the errors, warnings and unresolved names ([PORT.md](PORT.md#milestone-1-baseline)). It never touches the DOS build.
- `make help` prints this list.

Everything built goes under `build/`, which is never committed.

## The gate

`make check` does four things:

1. Compiles every source with a `/* target: */` line (eight to a DOS session, three sessions at once) and requires `match.py` to report WHOLE SEGMENT MATCHES and `verify.py` "fixups and data verified" for each.
2. Rebuilds `symbols.tsv` from scratch in a scratch directory and requires the same names at the same addresses as the committed file.
3. Requires the exact link to equal `UW2.EXE` except the bytes at 0x6676C and 0x66774.
4. Requires the modding build with no changes to be byte-identical to the exact link.

It recompiles only the sources whose text, headers or object have changed since they last passed (`build/check/state.json`). `make check-all` takes two to three minutes; `make check` with nothing changed about ten seconds.

Hosted CI cannot run the gate, because the toolchain and the game cannot be on GitHub, so the pre-push hook is the gate. Bypass it for one push with `git push --no-verify` (or `SKIP_CHECK=1 git push`). It checks the working tree, not the commits being pushed, so commit or stash first. The GitHub workflow runs only `tools/repocheck.py`: script syntax, relative Markdown links, and that no game data or Borland binary is committed. Run it locally with `.venv/bin/python tools/repocheck.py`.

## Working on one file

A source is known to the tools by its stem, the file name without its extension (unique across the tree; its object is `build/STEM/STEM.OBJ`), and by its DOS segment, the `/* target: */` line near the top. `grep -rl 'target: ovr154' src` finds a segment's file. `tools/sources.py` finds sources in every directory under `src/`, and the link order in `tools/extract.py` is written by segment, so renaming or moving a file needs no tool change.

- `.venv/bin/python tools/match.py src/game/SKILLS.C` compiles the file and reports every function as MATCH or where it differs. `--dis NAME` shows an instruction diff for one function; `--no-build` re-compares the last build.
- `.venv/bin/python tools/verify.py src/game/SKILLS.C` checks what match.py masks: every fixup, and the file's initialised and uninitialised data. `--update` also merges the file's externs into `symbols.tsv`, refusing any conflict.
- Each file's compiler switches are in its `/* opts: */` line; match.py defaults to `-mm -1 -G -O -Z`.

[MATCHING.md](MATCHING.md) explains the switches, the assembler, and what the compiler's output reveals about the original source.

## The tools

All of them are in `tools/`, and each describes itself at the top.

| Role | Tools |
| --- | --- |
| Make targets and the gate | `uw2.py` (behind the Makefile), `install-hooks.sh`, `repocheck.py` |
| Toolchain setup | `setup-tc.sh`, `setup-tasm.sh` |
| Building in headless DOS | `tcc.mjs` (compile or assemble), `dosrun.mjs` (batch lines, used by the link), `rungame.mjs` (boot and screenshot) |
| Matching one file | `match.py`, `verify.py`, `bssorder.py` (predicts `_BSS` order), `asmgen.py` (first draft of an assembly module), `fmt.py` (FM Towns disassembly) |
| Object files | `omf.py`, `fixups.py` |
| Sources | `sources.py` (finding a source), `srcdeps.py` (a source's headers) |
| Linking | `link.py`, `extract.py`, `exediff.py`, `addrscan.py` (numbers that could be addresses) |
| Target tables and names | `targets.py`, `syncnames.py` |
| The map | `doslist.py`, `locate.py`, `callgraphs.py`, `callpairs.py`, `anchors.py`, `align.py`, `files.py` |
| The port | `portcheck.py` (`make port-check`: compiles the C for the host, compile only; [PORT.md](PORT.md#milestone-1-baseline)) |
