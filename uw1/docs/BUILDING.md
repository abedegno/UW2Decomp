# Building

The README's [Building](../README.md#building) section has what you need and the everyday commands. This page has the rest: where Exhume comes in, the make targets, and how to record and replay a session of the game in DOS and check it against its golden reference.

## Exhume

The gate, the links and the replay tools are Exhume's, and the DOS build also uses Exhume's runtime in place:

- `runtime/include/portable.h` holds the macros the shared sources use to build for DOS and for a native port alike (explicit widths, `NULLTRAP`, `STACK_JUNK` and the record and replay hooks). The build stages it beside `src/include` for every compile, so the gate proves that it costs no byte. It includes UW1's `src/include/hookgame.h` first, which gives the original tokens of the hooks: `ORIG_GAME_TIME()` is `(*Time)`, `ORIG_KEY()` is `key()`, and so on.
- `runtime/replay/replay.c` is the record and replay code. Only the replay DOS build compiles it, with UW1's `src/include/rpgame.h`, which says what the hooks read and what the state dumps hold.

`tools/exhume.py` finds the checkout: `$EXHUME`, else `.exhume` in this repository, else a checkout named `Exhume` beside this one, else `~/Exhume`. `tools/exhume-ref` names the Exhume commit this tree was proved with. `tools/link.py`, which every `make check` and `make game` runs, prints a warning when your checkout does not contain that commit; the warning never stops the build, and nothing is said when `git` is not on your `PATH`. `python3 tools/exhume.py` prints the checkout and checks the commit on its own.

## Make targets

| Target | What it does |
| --- | --- |
| `make check` | the gate: every source matches and verifies, symbols.tsv rebuilds, the exact link is your `UW.EXE`, and the modding build with no change is the same |
| `make game` | the modding build, `build/MODLINK/out/UW.EXE` |
| `make golden` | every session's golden reference made again from DOS (each replayed twice, checked identical) |
| `make golden-check` | every session replayed twice in DOS by the replay DOS build and compared with its committed golden; writes nothing |

The port targets (`make port`, `make verify`, `make test`) are there for the native port, which does not exist yet.

## Recording and replaying a session

A session is a recording of everything the game read from the outside world while someone played it in DOS: each read of the game clock, the keyboard, the mouse and its buttons, the time of day, and the sound hardware's state, in order. Replayed, the game reads the same values in the same order, so it does exactly what it did, however fast the machine is. At checkpoints the replay writes the game state to `STATE.OUT`: the player record, the level, the graphics library's data, Borland's rand seed, C0's null-pointer area and, in a full dump, the palette, the CRT controller and all of video memory. Exhume's docs/port.md, "Verifying a port", is the method, and `runtime/replay/replay.c` has the file formats.

The hooks are the macros `GAME_TIME()`, `KEY()`, `MOUSE()`, `MBUTTONS()`, `WALL_TIME()`, `SRAND()`, `SND_READ()`, `SLAVE_TIMER()` and `CHECKPOINT()` in the C sources (`JOY_READ()` and `JOY_BUTTONS()` exist too, but no UW1 source reads the joystick). Under Turbo C they are the original tokens, so the DOS build is unchanged. `CHECKPOINT(1)` to `CHECKPOINT(5)` are in `src/game/UWEDIT.C`: after the start-up, after the title, after the main menu, when the game screen is drawn and when it has faded in.

All the commands are Exhume's `tools/replay.py`, run with UW1's `exhume.toml`, whose `[replay]` section names the sessions, their steps and their sound configurations:

```sh
R="python3 $HOME/Exhume/tools/replay.py --config exhume.toml"
$R build                                   # the replay DOS build: build/replay/UW.EXE
$R record OUT --session newgame            # record a session in js-dos, from [replay.steps]
$R record OUT w:9000 k:Escape s:menu       # or from steps of your own
$R dos tests/replay/walk.rec OUT           # replay a session in DOS
$R compare OUT1 OUT2 [--same-build]        # compare two runs' dumps, checkpoint by checkpoint
$R show OUT/STATE.OUT PNGDIR               # list a dump, and write each full checkpoint's screen
$R nulls OUT                               # the null-pointer write check of a DOS run
$R log tests/replay/walk.rec               # what a recording holds
$R golden [SESSION ...]                    # make goldens from DOS
$R golden [SESSION ...] --check            # replay in DOS and compare with the committed goldens
```

The replay DOS build is the modding build with every source that uses a hook compiled with `-DREPLAY`, every source with a `NULLTRAP` mark compiled with `-DNULLTRAP`, and `replay.c` linked in as one more resident module (`tools/link.py --mod --add`). The other commands build it when it is out of date; run `make check` first, since the modding link starts from the exact link's snapshot.

Recording runs in js-dos through dos-mcp, which takes the inputs in real time; the steps are Exhume's `tools/replaydos.mjs` steps (`w:MS` wait, `k:KEY` keys, `t:TEXT` typed text, `h:KEY,MS` a key held down, `M` the pointer to the top left corner, `m:DX,DY` a pointer move in the 640 by 400 frame, `c:BUTTON,MS` a click, `s:NAME` a screenshot). F12 ends a recording. Replays run in DOSBox-X when it is installed, else in js-dos.

Each session has its own `DATA\UW.CFG`, `NAME.cfg` beside `NAME.rec`, so no session depends on the configuration in your copy of the game. Every session also starts with no saved games: the saved games already in your game's `SAVE1` to `SAVE4` are left out of the staged copy.

These variables, when set, pass through to the replay DOS build (`replay.c`'s first comment has them all): `UWRPCK=n` a periodic dump every n clock ticks (hex) instead of 400h; `UWRPFULL=lo,hi` full periodic dumps while the clock is in that range; `UWRPHOOK=lo,hi` a full dump at every hook call whose count is in that range; `UWRPTRACE=lo,hi` every hook call in that clock range, with its caller, to `TRACE.OUT`; `UWRPFB=1` the 3D view's frame buffer in every dump. Together they find where two runs part: the checkpoints bracket it, `UWRPCK=1` finds the tick, `UWRPHOOK` the hook call.

### The sessions

The recordings and their goldens are committed under `tests/replay`. The moves in `items` and `talk` use the step keys (Shift+W half a tile, Shift+A and Shift+D 45 degrees), which move by fixed amounts, so that the same steps reach the same place in every recording. They hold inputs and digests only, no game data.

| Session | What it covers |
| --- | --- |
| `newgame` | boot, the title, the main menu, a new character (every default, the name Avatar), sixteen seconds in the game |
| `walk` | the way into the game, then walking, turning, looking up and down, stepping back, sliding and a click in the 3D view |
| `sound` | the way into the game with a Sound Blaster (FM music, SBFM.ADV, and digital speech, SBDIG.ADV), fight mode, swings, walking and turning, and the music played on until the theme changes |
| `soundfm` | the same with the FM music alone (no speech card) |
| `soundmt` | the same on a Roland MT-32 (MT32MPU.ADV at 330h), every wait doubled for the timbre uploads |
| `items` | the sack by the start: picking it up, opening it, the map in it (the automap, with a note), look mode, the statistics panel, a save to slot I and a restore |
| `talk` | the Red Key from the pack in the chest by the start, the locked door west of it unlocked and opened, the walk to Bragit's room in the human encampment, and a conversation with Bragit (whoami 67, the first man to talk to) with three answers. Talk mode clicks over a grid of the view in four directions, since Bragit wanders about his room: the misses say "You cannot talk to that", the first hit starts the conversation, and the rest fall in the conversation screen. Its golden is the largest (11 MB), since every click is a full checkpoint |
| `load` | slot I, the save `items` makes, loaded from the main menu ("Journey Onward") |
| `intro` | the title and the whole introduction, about four and a half minutes, to the main menu |

`load` replays the saved game `items` writes (`[replay] stage_from`); the golden tools take it from `items`' own DOS run. To replay `load` by hand, give it a directory holding that `SAVE1` with `--stage DIR`.

`sound` replays in js-dos (`[replay] dos_backend`): in DOSBox-X its replay hangs once the introduction starts its speech on SBDIG.ADV (tried with the Sound Blaster 16 and Pro 2, either CPU core, with the mixer on and off), while js-dos replays it to the end, the same twice. The other sessions replay in DOSBox-X, and their goldens come out the same in js-dos.

### Golden references

A golden is what DOS did with a session: `tests/replay/golden/NAME/golden.json` holds, for each checkpoint, its header and a 64-bit digest of each section as DOS and a port are compared, and a PNG of the screen at each full checkpoint. `make golden` makes them again (each session replayed twice, the two runs required identical, and the null-pointer check passed); `make golden-check` replays every session twice with the current replay DOS build and compares both runs with the committed golden, writing nothing. Until there is a port, `make golden-check` is the test that the replay build and the sessions still agree with DOS.

Some bytes are the machinery of the graphics library rather than game state, and `exhume.toml` masks them with the reason for each (`[[replay.mask]]`): its two private stacks in seg048 (3963:4DFE..4FA6 and 547E..5546), which interrupts push onto whenever they come, the clock word its retrace wait keeps (0652), and, between DOS and a port, the stack pointer it saves (5588..558B). The far pointers stored in seg048 are compared as the blocks they name (`[replay.segments]`), since DOSBox-X, js-dos and a port load the program at different segments.

### Adding a session

1. Write its steps into `[replay.steps]` and its `DATA\UW.CFG` text into `[replay.cfgs]` in `exhume.toml`.
2. Record it: `$R record OUT --session NAME`. Look at the screenshots it took, then copy `OUT/RECORD.OUT` to `tests/replay/NAME.rec` and `OUT/UW.CFG` to `tests/replay/NAME.cfg`.
3. If it starts from another session's saved game, add it to `[replay] stage_from`.
4. `$R golden NAME`, and commit the recording and its golden together.

A recording is only as good as the replay build that made it: when the replay build changes in a way that changes what the game does (a `STACK_JUNK` mark, say), record the sessions again and make their goldens again.

### What the replays found

- **A stack-junk read in the cutscene player.** `src/gfx/CUTS.C`'s `show_anm` never sets its state's flag b7 (a pause that waits for the speech) before the pause loop reads it, so in DOS a pause waits or not by whatever the stack held. DOSBox-X and js-dos took different paths on the same recording. `STACK_JUNK_SET(st.flags.bit.b7, 0)` gives the replay build and a port the path with no wait; the DOS build is unchanged.
- **The game polls the mouse buttons on a small stack.** Some of UW1's calls to `mbuttons()` run on the 0D4h-byte stack at the start of SYSENTRY.ASM's `_DATA` (DS:2444 in the replay build), just above the C library's near heap variables. A full dump made from that hook overran it, corrupted the near heap, and a later `realloc` wrote into DS:4. `replay.c` now does its deep work (its files, the dumps, the trace, the end) on a stack of its own.
