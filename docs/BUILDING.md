# Building

Everything runs on your own machine: Turbo C++, TASM and TLINK run headless in a DOS emulator (emu2 when `make setup` has built it, else DOSBox-X when it is installed, else js-dos through the [dos-mcp](https://www.npmjs.com/package/dos-mcp) npm package; see [Choosing the DOS](#choosing-the-dos)), and every build is compared with your own `UW2.EXE`. The Makefile only names the entry points; the logic is in `tools/uw2.py`.

## Requirements

- Node 20 or later, Python 3, mtools and 7z (`brew install mtools p7zip` on macOS).
- The Turbo C++ 1.01 disk images (four 720K images, which Borland released free of charge) and the Turbo Assembler 2.0 disk image. Neither is in this repository.
- UW2's `UW2.EXE` at `~/UWGOG/UW2/UW2.EXE`, or set `UW2_EXE`. The GOG release works. No game data is in this repository either.

Optional, for speed: git, make and a C compiler (the Xcode command line tools), with which `make setup` builds emu2, a DOS that runs the toolchain about twenty times faster than js-dos ([Choosing the DOS](#choosing-the-dos)). Or DOSBox-X (`brew install dosbox-x`), nearly as fast.

Optional, for the map tools and the assembly drafts (see [MAP.md](MAP.md)):

- The IDA listing `uw2_asm.asm` from [UWReverseEngineering](https://github.com/hankmorgan/UWReverseEngineering), expected at `~/UWReverseEngineering/uw2_asm.asm` (or set `UW2_ASM`). It is large and the shell's grep may skip it as binary; use `grep -a`. The modding build uses it, when present, to write the 3D model interpreter's opcode table as names.
- The Japanese FM Towns release of UW2, for `tools/fmt.py`, which disassembles a function by its original name. Extract `UW2.EXP` from the disc and use `uw2fmt.py` from UWReverseEngineering's `UW2 FM Towns` folder to write `fmtowns/uw2fmt.img` (`unpack`) and `fmtowns/syms.tsv` (`syms`). `verify.py --update` reads `fmtowns/syms.tsv`, when present, to mark names in `symbols.tsv` as original.

## Make targets

- `make setup TC_DISKS=DIR TASM_DISKS=DIR` extracts Turbo C++ into `TC/` and TASM into `TASM/` from the directories holding their disk images and checks both are the expected builds (by MD5), makes `.venv` with `iced-x86` (for instruction diffs), runs `npm install`, and builds emu2 into `tools/emu2` (`make setup-emu2` does only that; a failure there is not fatal, the tools then use DOSBox-X or js-dos). It skips whatever is already in place, so it is safe to run again, and prints which DOS the toolchain will use.
- `make` (or `make game`) is the modding build, `tools/link.py --mod`, and prints the path of the EXE. See [LINKING.md](LINKING.md#the-modding-build).
- `make exact` links the matched objects exactly (`tools/link.py`) and compares the result with your `UW2.EXE` (`tools/exediff.py`); it passes when only the two known bytes differ.
- `make check` is the gate, below. `make check-all` is the same with every source recompiled.
- `make boot` boots the modding build in headless DOS and saves screenshots of the title, the intro and the main menu under `build/boot/`. Look at them.
- `make hooks` installs a git pre-push hook that runs `make check` and stops the push when it fails.
- `make port-check` compiles every C source for the host with Apple clang, compile only, and summarises the errors, warnings and unresolved names ([PORT.md](PORT.md#milestone-1-baseline)). It never touches the DOS build.
- `make port` compiles every C source for the host, compiles the port's own C (`src/port`) and links them with SDL3 into `build/port/uw2port` ([the native port](#the-native-port), below). It never touches the DOS build either.
- `make help` prints this list.

Everything built goes under `build/`, which is never committed.

## The native port

The port ([PORT.md](PORT.md)) is a second build of the same C, for a modern host. Today it builds on macOS on Apple Silicon.

What it needs: Apple's clang (the Xcode command line tools), Python 3, `pkg-config` and SDL3 (`brew install sdl3 pkg-config`; 3.4.16 is the version tested). No Turbo C, DOS or game data is needed to build it, only to run it.

- `make port` compiles all 98 C sources the port uses and the port's own C under `src/port` (the SDL3 backend with SDL's flags), and links `build/port/uw2port`. It prints any warning in the port's own C.
- `make port-check` is the compile-only measurement of the game's C ([PORT.md](PORT.md#milestone-1-baseline)).
- `python3 tools/portstubs.py` rewrites the link stubs in `src/port/stubs` after a replacement lands (or after a game source starts using a new name); `--check` reports whether they are up to date. A stub stops the game where it is called and names itself.
- `python3 tools/portshot.py` is the screen test: it builds DOS EXEs that stop on each of the opening screens, screenshots them in headless DOS, and compares them with the port's screens pixel by pixel. The DOS builds take several minutes the first time and are cached in `build/portshot`.

Running it:

```sh
build/port/uw2port --data ~/UWGOG/UW2
```

`--data` names the directory of your copy of UW2 (`UW2.EXE`, `DATA`, `CRIT`, `CUTS`, `SOUND`); the port reads it and never writes it. Files the game creates or changes (its scratch files, `SAVE0`) go to the home directory, `--home DIR`, else `$UW2PORT_HOME`, else `~/.uw2port`. The port checks that `UW2.EXE` is the GOG release's by its size and CRC-32, and reads the far data no source defines yet from it. Anything on the command line that is not an option goes to the game, as its own command line would.

Window options: `--scale N` (3), `--no-aspect` (square pixels instead of 200 lines shown as 240), `--no-integer` (any scale, not whole multiples), `--hidden` (no window). Debug options: `--screenshot-after MS` and `--screenshot FILE` write the screen as a PNG, `--window-shot FILE` also the window's scaled contents, `--shot-at-flip K:FILE` the screen right after the game's K-th page flip, `--exit-after MS` quits, `--exit-on-halt` quits where the port stops instead of leaving the window up, and `-v` (or `UW2PORT_TRACE=1`) traces file opens, the paragraph map and the call stack at a stub. `UW2PORT_ARGS="--data ~/UWGOG/UW2" python3 tools/portbuild.py --run` builds and runs in one step.

At Milestone 3 the port runs the game's start-up to the end, shows the Origin and Looking Glass screens, starts the title cutscene and stops at the first graphics routine the cutscene needs that has no C yet. [PORT.md](PORT.md#milestone-3-results) has the detail.

## The gate

`make check` does four things:

1. Compiles every source with a `/* target: */` line (eight to a DOS session, one session per core up to 12 at once, or three in js-dos) and requires `match.py` to report WHOLE SEGMENT MATCHES and `verify.py` "fixups and data verified" for each.
2. Rebuilds `symbols.tsv` from scratch in a scratch directory and requires the same names at the same addresses as the committed file.
3. Requires the exact link to equal `UW2.EXE` except the bytes at 0x6676C and 0x66774.
4. Requires the modding build with no changes to be byte-identical to the exact link.

It recompiles only the sources whose text, headers or object have changed since they last passed (`build/check/state.json`). With emu2, `make check-all` takes about 7 seconds and `make check` with nothing changed about 4; with js-dos, two and a half minutes and about ten seconds.

Hosted CI cannot run the gate, because the toolchain and the game cannot be on GitHub, so the pre-push hook is the gate. Bypass it for one push with `git push --no-verify` (or `SKIP_CHECK=1 git push`). It checks the working tree, not the commits being pushed, so commit or stash first. The GitHub workflow runs only `tools/repocheck.py`: script syntax, relative Markdown links, and that no game data or Borland binary is committed. Run it locally with `.venv/bin/python tools/repocheck.py`.

## Choosing the DOS

Every compile, assembly and link goes through `tools/tcc.mjs` or `tools/dosrun.mjs`, which stage a directory as `C:\` and run a batch file in the DOS that `tools/dosbackend.mjs` picks. `UW2_DOS` chooses it:

- `emu2`: [emu2](https://github.com/dmsc/emu2), a small emulator for DOS command-line programs. Each batch line runs as its own emu2 process with the staged directory as `C:`; there is no COMMAND.COM, so a line must be a program with an optional `>` or `>>`, or `echo`. `make setup` (or `make setup-emu2`) clones it into `tools/emu2` at a pinned commit and applies `tools/emu2-date.patch`: emu2 cannot set the DOS date, which the link needs, and the patch keeps a date set through INT 21h AH=2Bh in a file the later programs of the run read.
- `dosbox-x`: DOSBox-X (`brew install dosbox-x`), headless (SDL's dummy video driver), on a copy of the staged directory mounted as `C:`.
- `staging`: DOSBox Staging (`~/Applications/DOSBox Staging.app`, or `dosbox` on the PATH). Its macOS build has no headless video, so every run opens a window for a second or two; it is never picked automatically.
- `jsdos`: js-dos, DOSBox compiled to WebAssembly, in headless Chrome through dos-mcp. It needs nothing beyond `npm install`.
- `auto`, or unset: emu2 if it is built, else DOSBox-X if it is installed, else js-dos. `node tools/dosbackend.mjs` prints the one in use.

`UW2_EMU2`, `UW2_DOSBOX_X` and `UW2_DOSBOX_STAGING` name a binary. `UW2_DOS_SESSIONS` overrides how many DOS sessions the gate and the modding build run at once (one per core up to 12 for the native ones, three for js-dos). The game itself (`make boot`, `tools/rungame.mjs`, `portshot.py`'s screenshots) always runs in js-dos.

All four build the same bytes. Every one of the 153 sources was compiled in each and compared with js-dos's objects: they differ only in the time of day that Turbo C records for each source and header (the Borland dependency records, from the file's time stamp, and those records' checksums), which differs between two js-dos builds too. With the staged files' time stamps fixed, emu2, DOSBox-X and DOSBox Staging give identical objects for all 153; js-dos stamps the time it loaded the files instead. The exact links (EXE and map) are identical in all four, and an emu2 without the date patch gets three bytes of the link date wrong. `make check-all` passes in each.

Measured on an Apple M4 Pro (14 cores):

| | emu2 | DOSBox-X | DOSBox Staging | js-dos |
| --- | --- | --- | --- | --- |
| `match.py` on one file (SKILLS.C) | 0.4 s | 1.6 s | 1.4 s | 6.3 s |
| the exact link (`link.py`) | 1.3 s | 2.6 s | 3.3 s | 3.1 s |
| all 153 sources, 3 sessions | 7 s | 13 s | 13 s | 64 s, 23 left for single builds |
| all 153 sources, 12 sessions | 3 s | 4 s | 6 s | (3 sessions) |
| `make check-all` | 7 s | 10 s | 12 s | 149 s |
| `make check`, nothing changed | 4 s | 7 s | 7 s | 10 s |
| `make` after a change to `portable.h` (93 sources), one at a time as before | 27 s | 140 s | | 585 s |
| the same, batched and in parallel | 4 s | 6 s | 8 s | 128 s |

js-dos often stops a session part of the way through a batch, so in js-dos the gate and the modding build compile the sources a batch missed again on their own.

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
| Building in headless DOS | `tcc.mjs` (compile or assemble), `dosrun.mjs` (batch lines, used by the link), `dosbackend.mjs` (the DOS they run in), `dosbatch.py` (many sources at once, for the gate and `link.py --mod`), `setup-emu2.sh` with `emu2-date.patch`, `rungame.mjs` (boot and screenshot, always js-dos) |
| Matching one file | `match.py`, `verify.py`, `bssorder.py` (predicts `_BSS` order), `asmgen.py` (first draft of an assembly module), `fmt.py` (FM Towns disassembly) |
| Object files | `omf.py`, `fixups.py` |
| Sources | `sources.py` (finding a source), `srcdeps.py` (a source's headers) |
| Linking | `link.py`, `extract.py`, `exediff.py`, `addrscan.py` (numbers that could be addresses) |
| Target tables and names | `targets.py`, `syncnames.py` |
| The map | `doslist.py`, `locate.py`, `callgraphs.py`, `callpairs.py`, `anchors.py`, `align.py`, `files.py` |
| The port | `portcheck.py` (`make port-check`: compiles the C for the host, compile only), `portbuild.py` (`make port`: compiles and links it), `portstubs.py` (writes the link stubs), `portshot.py` (the port's screens against DOS's), `widths.py` (explicit integer widths), `layoutcheck.py` (struct layouts under Turbo C against the host), `intaudit.py` (the promotion and overflow audit) ([PORT.md](PORT.md)) |
