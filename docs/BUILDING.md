# Building

Everything runs on your own machine: Turbo C++, TASM and TLINK run headless in a DOS emulator (emu2 when `make setup` has built it, else DOSBox-X when it is installed, else js-dos through the [dos-mcp](https://www.npmjs.com/package/dos-mcp) npm package; see [Choosing the DOS](#choosing-the-dos)), and every build is compared with your own `UW2.EXE`. The Makefile only names the entry points; the logic is in `tools/uw2.py`.

## Requirements

- Node 20 or later, Python 3, mtools and 7z (`brew install mtools p7zip` on macOS, `apt install mtools p7zip-full` on Ubuntu).
- The Turbo C++ 1.01 disk images (four 720K images, which Borland released free of charge) and the Turbo Assembler 2.0 disk image. Neither is in this repository.
- UW2's `UW2.EXE` at `~/UWGOG/UW2/UW2.EXE`, or set `UW2_EXE`. The GOG release works. No game data is in this repository either.

Optional, for speed: git, make and a C compiler (the Xcode command line tools), with which `make setup` builds emu2, a DOS that runs the toolchain about twenty times faster than js-dos ([Choosing the DOS](#choosing-the-dos)). Or DOSBox-X (`brew install dosbox-x`), nearly as fast. DOSBox-X is also where the replays' DOS side runs when it is installed, three and a half times faster than js-dos ([Testing](#testing)), The tests need Unicorn in the `.venv` for the routine fuzzing and `tools/ailcheck.py`; `make setup` installs it with iced-x86 from `tools/requirements.txt`.

Optional, for the map tools and the assembly drafts (see [MAP.md](MAP.md)):

- The IDA listing `uw2_asm.asm` from [UWReverseEngineering](https://github.com/hankmorgan/UWReverseEngineering), expected at `~/UWReverseEngineering/uw2_asm.asm` (or set `UW2_ASM`). It is large and the shell's grep may skip it as binary; use `grep -a`. The modding build uses it, when present, to write the 3D model interpreter's opcode table as names.
- The Japanese FM Towns release of UW2, for `tools/fmt.py`, which disassembles a function by its original name. Extract `UW2.EXP` from the disc and use `uw2fmt.py` from UWReverseEngineering's `UW2 FM Towns` folder to write `fmtowns/uw2fmt.img` (`unpack`) and `fmtowns/syms.tsv` (`syms`). `verify.py --update` reads `fmtowns/syms.tsv`, when present, to mark names in `symbols.tsv` as original.

## Make targets

- `make setup TC_DISKS=DIR TASM_DISKS=DIR` extracts Turbo C++ into `TC/` and TASM into `TASM/` from the directories holding their disk images and checks both are the expected builds (by MD5), makes `.venv` with `iced-x86` (for instruction diffs) and `unicorn` (for the tests) from `tools/requirements.txt`, runs `npm install`, builds emu2 into `tools/emu2` (`make setup-emu2` does only that; a failure there is not fatal, the tools then use DOSBox-X or js-dos), and fetches the port's OPL emulator into `tools/nuked-opl3` (`make setup-sound` does only that, [Sound](#sound)). On Linux, `make setup-libs` builds SDL3 and libmt32emu from source into `tools/libs` ([The native port](#the-native-port)). It skips whatever is already in place, so it is safe to run again, and prints which DOS the toolchain will use.
- `make` (or `make game`) is the modding build, `tools/link.py --mod`, and prints the path of the EXE. See [LINKING.md](LINKING.md#the-modding-build).
- `make exact` links the matched objects exactly (`tools/link.py`) and compares the result with your `UW2.EXE` (`tools/exediff.py`); it passes when the two are byte-identical.
- `make check` is the gate, below. `make check-all` is the same with every source recompiled.
- `make boot` boots the modding build in headless DOS and saves screenshots of the title, the intro and the main menu under `build/boot/`. Look at them.
- `make test` is the test every change to the port runs before it is pushed: the gate, the port build, the routine fuzzing's quick run and every replay session in the port against its golden ([Testing](#testing)). `make test-full` is the long tier. `make verify`, `make golden`, `make fuzz` and `make coverage` run one part each.
- `make hooks` installs a git pre-push hook that runs `make test` and stops the push when it fails.
- `make port-check` compiles every C source for the host with clang, compile only, and summarises the errors, warnings and unresolved names ([PORT.md](PORT.md#milestone-1-baseline)). It never touches the DOS build.
- `make port` compiles every C source for the host, compiles the port's own C (`src/port`) and links them with SDL3 into `build/port/uw2port` ([the native port](#the-native-port), below). It never touches the DOS build either.
- `make help` prints this list.

Everything built goes under `build/`, which is never committed.

## The native port

The port ([PORT.md](PORT.md)) is a second build of the same C, for a modern host. It builds on macOS on Apple Silicon, on Linux (Ubuntu 24.04, x86-64 and arm64) and on Windows (MSYS2's CLANG64 environment). The replays and the fuzzing pass on macOS and on Linux; the Windows build is compiled and linked by CI and has not been run ([Continuous integration](#continuous-integration)).

What it needs: clang, Python 3, `pkg-config` and SDL3 (3.4.16 is the version tested); for sound, Nuked OPL3 and libmt32emu ([Sound](#sound)), without which the port builds and those chips are silent. No Turbo C, DOS or game data is needed to build it, only to run it. The tools use `$CC` when it is set, else `clang`, else `cc`; the flags are clang's.

- macOS: the Xcode command line tools, and `brew install sdl3 mt32emu pkgconf`.
- Linux: `apt install clang pkg-config cmake ninja-build`, then `make setup-libs`, which builds SDL3 3.4.16 and libmt32emu 2.8.3 from source into `tools/libs` (ignored by git; no Ubuntu release before 25.04 packages SDL3, and none packages libmt32emu). Without the X11 or Wayland development headers SDL3 is built without windows, which is enough for `--hidden`. Then `export PKG_CONFIG_PATH=$PWD/tools/libs/lib/pkgconfig LD_LIBRARY_PATH=$PWD/tools/libs/lib`.
- Windows: MSYS2's CLANG64 shell with `pacman -S make mingw-w64-clang-x86_64-{clang,pkgconf,sdl3,python}`, then `make PY=python port`. The build has no MT-32 (MSYS2 has no libmt32emu) and links `build/port/uw2port.exe`.

- `make port` compiles all 98 C sources the port uses and the port's own C under `src/port` (the SDL3 backend with SDL's flags), and links `build/port/uw2port`. It prints any warning in the port's own C.
- `make port-check` is the compile-only measurement of the game's C ([PORT.md](PORT.md#milestone-1-baseline)).
- `make port-debug` builds `build/port-debug/uw2port` with `-g` and UBSan's `-fsanitize=null`: a null dereference the faithful port does not handle is reported with its file and line and the program goes on, so a replay lists every one it meets.
- `python3 tools/portstubs.py` rewrites the link stubs in `src/port/stubs` after a replacement lands (or after a game source starts using a new name); `--check` reports whether they are up to date. A stub stops the game where it is called and names itself.
- `python3 tools/portshot.py` is the screen test: it builds DOS EXEs that stop on each of the opening screens, screenshots them in headless DOS, and compares them with the port's screens pixel by pixel. The DOS builds take several minutes the first time and are cached in `build/portshot`.

Running it:

```sh
build/port/uw2port --data ~/UWGOG/UW2
```

`--data` names the directory of your copy of UW2 (`UW2.EXE`, `DATA`, `CRIT`, `CUTS`, `SOUND`); the port reads it and never writes it. Files the game creates or changes (its scratch files, `SAVE0`) go to the home directory, `--home DIR`, else `$UW2PORT_HOME`, else `~/.uw2port`. The port checks that `UW2.EXE` is the GOG release's by its size and CRC-32, and reads the far data no source defines yet from it. Anything on the command line that is not an option goes to the game, as its own command line would.

Record and replay ([PORT.md](PORT.md#the-differential-test-input-record-and-replay)): `--record` records the session to `RECORD.OUT` in the home directory (F12 ends it), and `--replay FILE` replays a recording instead of reading the clock, keyboard and mouse; both write the state dumps to `STATE.OUT` in the home directory, and the game quits at the end of the recording (`--exit-on-halt` makes it quit where the port stops, too).

Window options: `--scale N` (3), `--no-aspect` (square pixels instead of 200 lines shown as 240), `--no-integer` (any scale, not whole multiples), `--hidden` (no window). Debug options: `--screenshot-after MS` and `--screenshot FILE` write the screen as a PNG, `--window-shot FILE` also the window's scaled contents, `--shot-at-flip K:FILE` the screen right after the game's K-th page flip, `--exit-after MS` quits, `--exit-on-halt` quits where the port stops instead of leaving the window up, and `-v` (or `UW2PORT_TRACE=1`) traces file opens, the paragraph map and the call stack at a stub. `UW2PORT_ARGS="--data ~/UWGOG/UW2" python3 tools/portbuild.py --run` builds and runs in one step.

At Milestone 5 the port runs the game from boot into the world with music and effects: recorded DOS sessions through character creation, the first rooms, fighting, the inventory, the automap, a conversation, saving and loading replay in it with the same game state and the same video memory as DOS at every checkpoint, the 3D frames included, and its saves are DOS's byte for byte. [PORT.md](PORT.md#milestone-5-results) has the detail. `UW2PORT_ASMTRACE=1` writes every entry into the translated renderer, with the registers, to the standard error, and `UW2PORT_SPRITEHOOK=1` installs a sprite hook that only counts (PORT.md, "The render interface and the sprite hook").

### Sound

The port plays the game's music and effects through C versions of the game's own sound drivers ([PORT.md](PORT.md#sound)), on emulated hardware that comes from two libraries, neither of them in the repository:

| What | Library | License | Where from |
| --- | --- | --- | --- |
| the FM chips (Ad Lib, Sound Blaster, Pro Audio Spectrum) | Nuked OPL3, commit `765ec96` | LGPL-2.1 | `make setup-sound` (`tools/setup-sound.sh`) fetches `opl3.c` and `opl3.h` into `tools/nuked-opl3`, ignored by git, and checks them by SHA-256; `make port` compiles them when they are there |
| the Roland MT-32 and CM-32L | libmt32emu (munt) 2.8.3 | LGPL-2.1-or-later | `brew install mt32emu`, or `make setup-libs` on Linux; `make port` links it when `pkg-config` finds it |
| the audio output | SDL3 | zlib | `brew install sdl3`, or `make setup-libs` on Linux |

`make port` says which it found (`sound: Nuked OPL3, libmt32emu`).

The game takes its sound cards from `DATA\UW.CFG`, whose two lines the GOG release sets to no card. `--sound CARD[,SPEECH]` writes that file into the port's home directory (where the game looks first): music card 2 Ad Lib, 3 Sound Blaster, 4 Sound Blaster Pro (two OPL2s), 5 MT-32, 6 Pro Audio Spectrum, 7 Sound Blaster Pro (OPL3), 0 none; speech card 1 Sound Blaster, 2 Sound Blaster Pro, 3 Pro Audio Spectrum, 0 none. `--sound 3,1` is a Sound Blaster with its digitised effects; the setting stays in the home directory until changed. The PC speaker (card 1) is not emulated.

The MT-32 needs the user's own ROM images, which the repository never holds: `--mt32-roms DIR` (or `UW2PORT_MT32_ROMS=DIR`), a directory with `CM32L_CONTROL.ROM` and `CM32L_PCM.ROM` (used first) or `MT32_CONTROL.ROM` and `MT32_PCM.ROM`. Without them the MT-32 driver runs and the music is silent; its MIDI stream is still checked against `DM05.ADV`'s (`ailcheck.py`, below), which needs no ROM.

`--audio-wav FILE` writes everything the cards play to a 44100 Hz stereo WAV file, in the game's own time (under `--replay` the whole session, however fast the port runs it); `--no-audio` opens no audio device. `--ail-log FILE` and `--hw-log FILE` write every call the game makes to its sound drivers and every register write or MIDI byte the drivers make, and `tools/ailcheck.py AIL_LOG HW_LOG` runs the same calls through the user's real `.ADV` driver in an x86 emulator and compares the writes (it needs Unicorn, 2.1.4 tested, a development tool only: `.venv/bin/pip install unicorn==2.1.4` puts it where `replay.py check` finds it).

## Recording and replaying a session

`tools/replay.py` records a session in DOS and replays it in DOS and in the port, comparing the state dumps ([PORT.md](PORT.md#milestone-4-results) has the formats and results):

- `python3 tools/replay.py build` builds the replay DOS EXE, `build/replay/UW2.EXE`: the modding build with the sources that use the replay hooks compiled with `-DREPLAY` (and those with `NULLTRAP` marks with `-DNULLTRAP`), and `src/replay/REPLAY.C` linked in as one more resident module (`tools/link.py --mod --add`). The other commands build it when it is out of date.
- `python3 tools/replay.py record OUT --session newgame` records the standard session (boot, the title, the main menu, a new character, sixteen seconds in the game) in headless DOS, and `--session walk` the same way into the game and then about forty seconds in its first rooms (walking, turning, looking up and down, a click in the view); `--session sound` does that with a Sound Blaster's music and digitised effects, fight mode and swings into the air, and stays long enough for a music theme to end, `--session soundfm` the same with every effect on the FM chip (no speech card), and `--session soundmt` the same on a Roland MT-32 at 330h (no speech card; every wait doubled, since DM05.ADV's timbre uploads make the way in slower); `--session items` goes in and handles things in the first room (picking up, a container, the automap and a note, look mode, the statistics panel, a save to slot 1 and a restore), `--session talk` walks to the great hall and talks to Nystul, and `--session load` loads a saved game from the main menu (record and replay it with `--stage DIR`, DIR holding the `SAVE1` the items session wrote); `record OUT step ...` records your own steps (`tools/replaydos.mjs` lists them; `h:KEY,MS` holds a key down, as walking needs). The recording is `OUT/RECORD.OUT`, the dumps `OUT/STATE.OUT`, and a session with a sound card also `OUT/UW.CFG`.
- A recording with a sound card has its `DATA\UW.CFG` beside it: `NAME.cfg` next to `NAME.rec` (`tests/replay/sound.cfg`), or `UW.CFG` in the recording's directory. Every replay of it, in DOS (`replaydos.mjs --cfg`, which puts the file into the staged game; js-dos has a Sound Blaster 16 at 220h, IRQ 7, DMA 1, with an OPL3) and in the port (into its home directory), uses it.
- `python3 tools/replay.py dos REC OUT` and `port REC OUT [--debug]` replay a recording in DOS or in the port (the `make port-debug` build with `--debug`).
- `python3 tools/replay.py compare A B` compares two runs' dumps checkpoint by checkpoint; `show STATE [PNGDIR]` lists a dump and writes the screen of each full checkpoint as a PNG; `nulls DIR` is the null-pointer write check of a DOS run; `log REC` describes a recording.
- `python3 tools/replay.py check REC OUT` does it all: two DOS replays, the port, the debug port, and the comparisons of the dumps and of the saved games; with a sound card also each run's sound driver against the recording (DOS's `SNDCHECK.OUT`, the port's count, which must be 0) and `ailcheck.py` on the port's driver logs.
- With a sound card the recording also holds, for each read of the sound hardware, the moment within the clock's tick at which DOS made it (recording format 3, [PORT.md](PORT.md#replays-with-a-sound-card)); recordings of format 2 still replay.

The DOS replays run in DOSBox-X when it is installed and otherwise in js-dos ([Testing](#testing) has the timings; `--backend jsdos` on `replaydos.mjs`, or `UW2_REPLAY_DOS=jsdos`, chooses js-dos); recording always runs in js-dos, which takes the inputs in real time. `UWRPCK=n` (hex) makes the periodic dumps every n clock ticks instead of 400h, `UWRPFB=1` puts the 3D view's frame buffer in every dump once a level is in, so that with a short interval every 3D frame is compared, not only those on the screen at a full checkpoint, and `UWRPTRACE=lo,hi` writes every hook call while the clock is in that range, with its caller, to `TRACE.OUT`, to find where two runs part. Each works in both builds.

The recordings are small (15 KB to 333 KB) and hold only the inputs, so the canonical ones are committed under `tests/replay`, while the dumps (14 MB and more) and the replays' working directories stay under `build/replay`.

## Testing

The port is tested against DOS in three ways, in two tiers.

| Command | What it runs | Time |
| --- | --- | --- |
| `make test` | `make check`; `make port`; the routine fuzzing's quick run (`tools/fuzzasm.py`); every replay session in the port against its golden (`tools/replay.py verify all`) | about 20 seconds |
| `make test-full` | `make check`; `make port` and `make port-debug`; every golden made again from DOS, each session replayed twice and the two runs checked identical (`replay.py golden all`); every session against them in the port and in the UBSan build; `tools/ailcheck.py` on the three sessions with a music card; the fuzzing's deep run; the coverage report | about 12 minutes, ten of them the deep fuzzing |

Both print a summary of their steps with the time each took, and exit 1 if any failed. `make test` is what every change to the port runs before it is pushed, and the pre-push hook runs it (`make hooks`; bypass with `git push --no-verify` or `SKIP_CHECK=1 git push`). The parts run alone: `make verify`, `make golden`, `make fuzz` (`FUZZ=--deep` for the long run) and `make coverage`.

### Golden references

Each session in `tests/replay` (every `NAME.rec`, with `NAME.cfg` for a sound card) has a golden reference in `tests/replay/golden/NAME`: `golden.json` and a PNG of the screen at each full checkpoint, made from DOS. `golden.json` holds no dump and no game data. For each checkpoint it has the header (kind, number, hook calls, clock) and a 64-bit BLAKE2b digest of each section of the dump, taken as `replay.py compare` compares DOS with the port: the program's machinery (`replay.py`'s `ALWAYS` and `CROSS` ranges, the byte after a string GRCORE copies) zeroed, and the words that hold a far block's segment replaced by the block they name (`tools/golden.py`'s `SEG_SLOTS`), since DOS and the port load the program at different segments. The `NULL` and `SEGS` sections are left out: they are the DOS set-up's own. Beside the digests it records the SHA-256 of the recording, of its `.cfg`, of the saved games the session writes and, for `load`, of the saved game it starts from, and the SHA-256 of the replay DOS build that made it.

- `python3 tools/replay.py golden [SESSION ...|all]` makes them: it replays each session in DOS twice at once, requires the two runs to be identical (`compare --same-build`, the saved games byte for byte) and the null-pointer check to pass, and writes the golden from the first run. `--check` writes nothing and compares the new runs with the committed goldens instead.
- `python3 tools/replay.py verify [SESSION ...|all]` replays the sessions in the port only and compares each checkpoint's digests with the golden's. It names the first checkpoint that differs and its sections, writes a DOS, port and difference picture (the differing pixels in red) for each screen that differs, as `build/replay/verify/NAME/port/diff_ckNNNN.png`, and checks the saved games and the port's count of sound driver reads that differ from DOS's (which must be 0). `--debug` replays with the UBSan build and fails on any report.
- A golden whose recording or `.cfg` has changed is stale, and `verify` fails on it until it is made again. One made by another replay DOS build (`build/replay/UW2.EXE` has another SHA-256) is flagged in the output, since that build's dumps could differ; `make test-full` makes every golden again and says if any differs from the committed one.
- `load` replays the saved game the `items` session makes, so it waits for `items`: `golden` stages items' first DOS run's `SAVE1`, `verify` the port's, once its digests are shown to be the golden's.

The goldens of the eight sessions take 7 MB, almost all of it the 355 PNGs; the same goldens come out of DOSBox-X and of js-dos (below).

### Fast replays

In a replay the game's clock and every input come from the recording, so nothing needs to wait for real time:

- **The port.** Under `--replay`, input status 1's retrace bit comes from a count of the reads instead of the host's clock (`gfx/vga.c`: every eighth read starts a retrace), so the game's waits for vertical retrace, which took a third of a replay's time, take a few reads; with no audio device and no `--audio-wav` (as `--hidden` has), the sound chips' synthesis is skipped (`sound/audio.c`), while the drivers still program them and every read the game makes of them is timed by the recorded moments as before. And `make port` compiles the port's C in `src/port/3d`, `src/port/gfx` and `src/port/x86` (the translated modules, the machine they run on, and the graphics C) with `-O2`; that code shares nothing with another thread but the screen the platform layer reads. The rest of the port's C and all of the game's stay unoptimised, and the debug and coverage builds compile everything without optimisation.
- **DOS.** `tools/replaydos.mjs` replays in native DOSBox-X (`brew install dosbox-x`) when it is installed: headless (SDL's dummy drivers), the game's directory mounted from the host as `C:` (the replay build writes `STATE.OUT` and the saved games there), and the dynamic core at a fixed 300,000 cycles a millisecond with turbo on, so that emulated time runs as fast as the host can run the guest instead of with the wall clock. It has js-dos's hardware: a Sound Blaster 16 at 220h, IRQ 7, DMA 1 and 5 with an OPL3, an intelligent MPU-401 at 330h with no synthesiser, 16 MB. DOSBox-X loads the game at another segment than js-dos does, which the goldens' digests allow for; every golden of the eight sessions made in DOSBox-X is identical to the one made in js-dos, checkpoint for checkpoint and section for section, and so are the saved games. DOSBox-X has no memory read after the game exits, so its runs rely on the dumps' `NULL` sections for the null-pointer check (C0's checksum is in every one). js-dos remains the fallback (`--backend jsdos`, `UW2_REPLAY_DOS=jsdos`) and is always used to record.

Measured on an Apple M4 Pro (14 cores), one session at a time:

| Session | DOS, js-dos | DOS, DOSBox-X | port, before | port, after |
| --- | --- | --- | --- | --- |
| `newgame` | 38 s | 11 s | 3.9 s | 0.8 s |
| `walk` | 61 s | 16 s | 7.8 s | 2.0 s |
| `sound` | 91 s | 27 s | 8.5 s | 1.8 s |
| `soundfm` | 89 s | 27 s | 8.6 s | 1.8 s |
| `soundmt` | 155 s | 43 s | 10.5 s | 2.9 s |
| `items` | 66 s | 18 s | 5.9 s | 1.7 s |
| `talk` | 70 s | 18 s | 10.9 s | 2.8 s |
| `load` | 20 s | 5 s | 1.9 s | 0.4 s |
| all eight | 590 s | 165 s | 58 s | 14 s |

The sessions run at once, one per core up to eight (`-j N` changes it; four for js-dos, whose every run is a headless Chrome): `verify all` takes 3.5 seconds, and `golden all` (sixteen DOS runs) 59 seconds in DOSBox-X and 219 seconds in js-dos.

### Routine fuzzing

`tools/fuzzasm.py` tests single routines on inputs no session gives them. For each target it runs the routine's own bytes from `UW2.EXE` (which the gate proves the matched sources build) in Unicorn, and the port's code for it (the translation, the hand-written C through the glue, or a C entry such as `cSqRt`) in `tools/fuzzhost.c`, a program linked from the port's objects in place of `main.c`, on the same registers and the same memory (the EXE's image at the port's load segment, so segment values agree, and two scratch segments). It compares the registers, the flags the routine's callers read, and every byte of memory the routine changed. The inputs are random, from a seed that is printed and that `--seed` repeats, and edge cases (0, 1, -1, 7FFFh, 8000h, FFFFh and each routine's own boundaries). A divide that faults stops both, and that counts as agreement. `--quick` (the default) runs 200 cases of each target, 40 of those that write a whole frame buffer or a 64 KB image, in about thirteen seconds; `--deep` fifty times as many, with a new seed each time; `--only` and `--list` choose and list the targets. Unicorn must be in the `.venv` (`.venv/bin/pip install unicorn==2.1.4`). PORT.md's Milestone 6a results list the targets and what they found.

### Coverage

`make coverage` (`tools/coverage.py`) builds the port with clang's source-based coverage, replays every session in it against its golden, runs the fuzzing on the same objects, and writes [COVERAGE.md](COVERAGE.md): the share of lines, functions and regions run, by directory and by file, the functions only the fuzzing reaches, and the functions nothing reaches, which is the list to record new sessions from. It takes about half a minute.

### Adding a session

1. Write its steps into `tools/replay.py`'s `SESSIONS` (the steps of `tools/replaydos.mjs`) and, for a sound card, its `UW.CFG` lines into `CFGS`.
2. Record it in DOS: `python3 tools/replay.py record build/replay/NAME --session NAME`, and copy `RECORD.OUT` to `tests/replay/NAME.rec` (and `UW.CFG` to `tests/replay/NAME.cfg`). Look at the screenshots it took to see that it did what you meant.
3. If it starts from another session's saved game, add it to `tools/golden.py`'s `STAGE_FROM`.
4. `python3 tools/replay.py golden NAME` makes its golden (DOS twice, checked identical), and `python3 tools/replay.py verify NAME` replays it in the port. Commit the recording and `tests/replay/golden/NAME` together.

## The gate

`make check` does four things:

1. Compiles every source with a `/* target: */` line (eight to a DOS session, one session per core up to 12 at once, or three in js-dos) and requires `match.py` to report WHOLE SEGMENT MATCHES and `verify.py` "fixups and data verified" for each.
2. Rebuilds `symbols.tsv` from scratch in a scratch directory and requires the same names at the same addresses as the committed file.
3. Requires the exact link to be byte-identical to `UW2.EXE`.
4. Requires the modding build with no changes to be byte-identical to the exact link.

It recompiles only the sources whose text, headers or object have changed since they last passed (`build/check/state.json`). With emu2, `make check-all` takes about 7 seconds and `make check` with nothing changed about 4; with js-dos, two and a half minutes and about ten seconds.

The pre-push hook runs the gate before anything leaves your machine, and CI runs it again ([Continuous integration](#continuous-integration)). The hook runs `make test`, the gate and the port's tests together ([Testing](#testing)); `make hooks` again updates a hook an older version installed. Bypass it for one push with `git push --no-verify` (or `SKIP_CHECK=1 git push`). It checks the working tree, not the commits being pushed, so commit or stash first. `tools/repocheck.py` checks script syntax, relative Markdown links, and that no game data or Borland binary is committed; run it locally with `.venv/bin/python tools/repocheck.py`.

## Continuous integration

Four workflows run on GitHub's standard hosted runners, in this public repository:

| Workflow | When | What it runs | Needs the bundle |
| --- | --- | --- | --- |
| `port.yml` | every push and pull request, forks included | `make port` and `make port-check` on Ubuntu 24.04, macOS and Windows (MSYS2 CLANG64); nothing is run | no |
| `repocheck.yml` | every push and pull request, forks included | `tools/repocheck.py` | no |
| `accuracy.yml` | pushes to `main`, pull requests from branches of this repository, and by hand | `make setup` and `make test` on Ubuntu 24.04: the gate, the port build, the quick fuzzing and the eight sessions against their goldens | yes |
| `nightly.yml` | 03:17 UTC each day, and by hand | `make test-full` on Ubuntu 24.04, with DOSBox-X from Ubuntu (2024.03.01, whose goldens are the committed ones); fails if a regenerated golden differs from the committed one, and puts the step times and the coverage totals in the job summary | yes |

A pull request from a fork gets `port.yml` and `repocheck.yml` only: GitHub gives a fork's pull request no secrets, and `accuracy.yml` skips itself for one rather than fail.

### The bundle

The gate and the replays need the game and the Borland toolchain, which cannot be published. They come from the private repository `abedegno/uw2-ci-assets` (its Actions are disabled), which holds one file, `uw2-ci-assets.tar.gz.age`: an [age](https://age-encryption.org)-encrypted tar.gz of `game/UW2` (the owner's GOG copy of the game), `tc/Disk01.img` to `Disk04.img` (Turbo C++ 1.01), `tasm/Disk01.img` (TASM 2.0), `MANIFEST.txt` and `SHA256SUMS`. Two secrets of this repository open it: `UW2_ASSETS_DEPLOY_KEY`, the private half of an ed25519 deploy key that can only read `uw2-ci-assets`, and `UW2_ASSETS_AGE_KEY`, the age secret key.

The local action `.github/actions/uw2-assets` clones the bundle over SSH with the deploy key (written to `$RUNNER_TEMP` with mode 600 and deleted once the clone is done, and checked against GitHub's published host key), and `tools/ci-assets.sh` decrypts it into `$RUNNER_TEMP/uw2-assets`, with the age key on age's standard input rather than in a file. It checks every file against `SHA256SUMS` and prints only how many passed, and sets `UW2_EXE`, `UW2_DIR`, `TC_DISKS` and `TASM_DISKS`, which `make setup` and the tools read. `.github/actions/linux-tools` installs everything else: the Ubuntu packages, Python with `tools/requirements.txt` in `.venv`, Node with `npm ci` (without dos-mcp's Chrome, which only js-dos uses), and emu2, SDL3, libmt32emu and Nuked OPL3, built by their setup scripts.

### What keeps the data private

- Nothing decrypted, and nothing built from it, is cached. The caches hold only emu2 (`tools/emu2`), SDL3 and libmt32emu (`tools/libs`), Nuked OPL3 (`tools/nuked-opl3`), and pip's and npm's downloads, each keyed on the script or file that pins it.
- A failed run uploads only pictures and text: the difference pictures of a session that differs from its golden (`diff_ck*.png`, the DOS and port screens beside each other), the port's log of each replay, the run's own output and, at night, the changed golden screens and the list of changed golden files. Never a state dump, a saved game, an object, an EXE or anything else under `build/`.
- No step lists or prints the decrypted tree. `tools/ci-assets.sh` reports a count, and the tools print only the path of `UW2.EXE`.
- The last step of each job, run whatever happened before it, deletes `$RUNNER_TEMP/uw2-assets`, `TC/`, `TASM/` and `build/`. The runner is discarded after the job in any case.
- No workflow uses `pull_request_target`. The jobs with secrets run on pushes to `main`, on the schedule, by hand, and on pull requests whose branch is in this repository, whose authors can already push here.
- The deploy key can read `uw2-ci-assets` and nothing else, and the bundle is useless without the age key.

### Linux and Windows

Three things differ on Linux, and the tools allow for each. The file system is case-sensitive, and emu2 creates a file under the case the DOS program gave (`TCC` writes `skills.obj`), so `tools/dosbackend.mjs` finds a DOS run's outputs, and the programs a batch runs, without regard to case. `fmtowns/syms.tsv` is made from the FM Towns release, which CI does not have, so without it the gate's rebuilt `symbols.tsv` cannot mark a name as original: `make check` then compares the names and addresses but not that mark, and says so. And `make setup` now takes `OVERLAY.LIB` from Turbo C++'s `XLIB.ZIP` as well: the link needs it, and `tools/setup-tc.sh` did not extract it before. With these, `make test` passes on Ubuntu 24.04 on x86-64 and on arm64, and so does every step of `make test-full` but the deep fuzzing, which was not tried there; DOSBox-X 2024.03.01 from Ubuntu makes goldens identical to the committed ones.

x86-64 found one difference of its own. BAGS.C's `OpenTheBag` writes past the end of `SlotToDisplay` into `DisplayToSlot`, the next variable in DGROUP, as DOS does on purpose. On arm64 the host compiler happened to put the two arrays end to end, but x86-64 puts an array of 16 bytes or more on a 16-byte boundary, so the write missed and the `items` session went another way after a bag was opened. The port now keeps the two in one array (`inv.h`, `INVPANEL.C`; the DOS build is unchanged, and the gate proves it).

The Windows build needs the port's few POSIX calls replaced: the EMS frame's double mapping (`mem/frame.c`) uses a file mapping viewed twice, the crash handler prints no call stack, and `mkdir` and `unsetenv` have Windows forms. `src/port/include/io.h` brings in MinGW's own `io.h`, which it would otherwise hide. On Windows `long` is 32 bits, as in DOS, where macOS and Linux have 64.

### Rotating the keys and the bundle

The age key is kept in the owner's macOS Keychain, service `uw2-ci-assets-age` (`security find-generic-password -s uw2-ci-assets-age -w` prints it, and `... -w | age-keygen -y` its recipient).

- A new bundle (new game files, say): put `game/UW2`, `tc/Disk01.img` to `Disk04.img`, `tasm/Disk01.img` and a `MANIFEST.txt` in an empty directory, and in it run `find game tc tasm MANIFEST.txt -type f | sort | xargs shasum -a 256 > SHA256SUMS`, then `COPYFILE_DISABLE=1 tar --no-xattrs -czf - game tc tasm MANIFEST.txt SHA256SUMS | age -r "$(security find-generic-password -s uw2-ci-assets-age -w | age-keygen -y)" -o uw2-ci-assets.tar.gz.age`. Commit that one file to `uw2-ci-assets` in place of the old one.
- A new age key: `age-keygen -o new.key`, store its `AGE-SECRET-KEY-1...` line with `security add-generic-password -U -s uw2-ci-assets-age -a "$USER" -w "$(grep AGE-SECRET-KEY new.key)"`, make the bundle again with the new recipient (above), set the secret with `security find-generic-password -s uw2-ci-assets-age -w | gh secret set UW2_ASSETS_AGE_KEY -R abedegno/UW2Decomp`, and delete `new.key`. Until the new bundle is pushed, the jobs that need it fail.
- A new deploy key: `ssh-keygen -t ed25519 -N '' -C 'UW2Decomp CI (read-only)' -f deploy`, `gh repo deploy-key add deploy.pub -R abedegno/uw2-ci-assets -t 'UW2Decomp CI (read-only)'` (read-only unless `-w` is given), `gh secret set UW2_ASSETS_DEPLOY_KEY -R abedegno/UW2Decomp < deploy`, then remove the old key (`gh repo deploy-key list -R abedegno/uw2-ci-assets`, `gh repo deploy-key delete ID -R abedegno/uw2-ci-assets`) and delete `deploy` and `deploy.pub`.

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
| Toolchain setup | `setup-tc.sh`, `setup-tasm.sh`, `setup-sound.sh` (the port's OPL emulator), `setup-libs.sh` (SDL3 and libmt32emu from source, for Linux), `requirements.txt` (the Python packages), `ci-assets.sh` (CI only: decrypts the asset bundle) |
| Building in headless DOS | `tcc.mjs` (compile or assemble), `dosrun.mjs` (batch lines, used by the link), `dosbackend.mjs` (the DOS they run in), `dosbatch.py` (many sources at once, for the gate and `link.py --mod`), `setup-emu2.sh` with `emu2-date.patch`, `rungame.mjs` (boot and screenshot, always js-dos) |
| Matching one file | `match.py`, `verify.py`, `bssorder.py` (predicts `_BSS` order), `asmgen.py` (first draft of an assembly module), `fmt.py` (FM Towns disassembly) |
| Object files | `omf.py`, `fixups.py` |
| Sources | `sources.py` (finding a source), `srcdeps.py` (a source's headers) |
| Linking | `link.py`, `extract.py`, `exediff.py`, `addrscan.py` (numbers that could be addresses) |
| Target tables and names | `targets.py`, `syncnames.py` |
| The map | `doslist.py`, `locate.py`, `callgraphs.py`, `callpairs.py`, `anchors.py`, `align.py`, `files.py` |
| The port | `portcheck.py` (`make port-check`: compiles the C for the host, compile only), `portbuild.py` (`make port`, `make port-debug`: compiles and links it), `portstubs.py` (writes the link stubs), `portshot.py` (the port's screens against DOS's), `replay.py` and `replaydos.mjs` (record and replay sessions, compare the state dumps), `golden.py` (the sessions' golden references and the port against them: `replay.py golden` and `verify`), `fuzzasm.py` with `fuzzhost.c` (the routine fuzzing), `coverage.py` (`make coverage`), `test.py` (`make test`, `make test-full`), `asm2c.py` (translates the renderer's assembly modules to C; `--check` says whether the committed C is up to date), `ailcheck.py` (the port's sound drivers against the real `.ADV` files), `widths.py` (explicit integer widths), `layoutcheck.py` (struct layouts under Turbo C against the host), `intaudit.py` (the promotion and overflow audit) ([PORT.md](PORT.md)) |

The canonical replay sessions are committed as `tests/replay/newgame.rec` (boot, the title, a new character, into the game), `tests/replay/walk.rec` (the same, then walking, turning, looking up and down and a click in the 3D view), `tests/replay/sound.rec`, `soundfm.rec` and `soundmt.rec` with their `.cfg` files (the same way in with a Sound Blaster, an FM chip alone or a Roland MT-32, then fight mode, swings, walking and a minute of music), `items.rec` (handling things, the automap, a save and a restore), `talk.rec` (a conversation with Nystul) and `load.rec` (loading the items session's save from the main menu; replay it with `--stage DIR`, DIR holding that `SAVE1`). They hold only recorded inputs, no game data: replay one with `python3 tools/replay.py dos tests/replay/walk.rec OUT` and `python3 tools/replay.py port tests/replay/walk.rec OUT`, then `compare`, or run `python3 tools/replay.py check tests/replay/walk.rec OUT`.
