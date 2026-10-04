# The native port

This page is the port of UW1 to modern systems: a native program built from the matched sources, on [Exhume](https://github.com/abedegno/Exhume)'s runtime, the same way UW2Decomp's port is built. Exhume's `docs/port.md` is the method and `skills/port-and-verify` the working order; UW2Decomp's `docs/PORT.md` is the worked example, milestone by milestone. This page records UW1's milestones and what each found.

## One tree, two builds

The DOS build is unchanged: `make check` still compiles every source with Turbo C++ 1.01 and TASM and proves the exact link and the modding build. The port compiles the same C with clang, with the runtime's portability layer, and replaces the assembly modules and the DOS-only C with C written for the port. Every change to a shared source keeps the DOS bytes, and the gate proves it.

| Path | What is in it | Seen by the DOS build |
| --- | --- | --- |
| `src/<subsystem>/*.C`, `*.ASM` | the game, shared by both builds (the assembly by the DOS build only) | yes |
| `src/include/hookgame.h`, `rpgame.h` | UW1's bindings for the runtime's `portable.h` and `replay.c` | yes (`rpgame.h`: only the replay build) |
| `src/port/portgame.h` | UW1's bindings for the runtime's port C | no |
| `src/port/...` | the port's own C | no |
| Exhume's `runtime/` | the portability layer, the platform layer, the emulated hardware, Borland's library, the sound library, the x86 machine, record and replay; compiled where it is, never copied | `portable.h` (staged), `replay.c` (only the replay build) |

`exhume.toml`'s `[port]` section names the port's directory (`src/port`, which `[project] exclude` keeps out of the DOS build), its output (`build/port/uw1port`) and the records that must keep their DOS layout (`[port.layout]`). The make targets are Exhume's: `make port-check`, `make port`.

## Milestone 1: measure and compile

`make port-check` (Exhume's `tools/portcheck.py`) compiles every C source for the host with the portability layer, compile only, and sorts the diagnostics. Run on 4 October 2026 with Apple clang 21.

The first run failed in every source that includes `sys.h`: UW1's debug print is `dprintf` (DEBUG.C, ovr106_1B), which POSIX 2008 also declares. `portgame.h` renames it for the port (`uw1_dprintf`), after compat.h has included the host's headers. With that, before any source was changed:

- 79 of the 89 sources compile; the 10 that fail have 31 errors between them;
- 66 warnings over the whole tree;
- 373 names that a link would still need.

| File | Errors | Why |
| --- | --- | --- |
| `src/inv/BAGS.C`, `src/inv/INVDATA.C` | 9, 7 | `inv.h` declared `SlotToDisplay` only for Turbo C; the port keeps it and `DisplayToSlot` as one array and had no declaration of it. BAGS.C also called `grfx_load_font` with no declaration |
| `src/ui/MAINMENU.C` | 4 | `grfx_load_font` called with no declaration, its argument cast to `int` |
| `src/gfx/CUTS.C` | 3 | the early prototype of `cutsop_stop` (there for the public order) said `unsigned far *` where the definition says `uint16 far *` |
| `src/obj/OBJECTS.C` | 2 | `Obj_Rem` declared `unsigned char` with bare `return`s |
| `src/game/PLAYER.C`, `SKILLS.C`, `src/combat/SPELLS.C`, `src/event/TRIGGER.C`, `src/motion/OBJPHYS.C` | 2, 1, 1, 1, 1 | calls with no declaration in scope (`FixBagArea`, `ClearInventory`, `flush_keys`, `play_effect_on_mobile`, `TalkTo`, `editchng`) |

### What changed

Every change below keeps the DOS bytes; `make check` passed after each.

- `inv.h` declares the port's `SlotToDisplay` array; `ui.h` declares `flush_keys` (MOUSE.C).
- `grfx_load_font` is declared in MAINMENU.C and BAGS.C, as in its other callers, and the `(int)` casts of its argument are gone.
- CUTS.C includes `uw2.h` before its early prototypes, so `cutsop_stop`'s says `uint16`.
- `Obj_Rem` is `void`; nothing uses a result.
- PLAYER.C, OBJPHYS.C, TRIGGER.C and SPELLS.C include the headers that declare what they call.
- `init_mem` (TMPALLOC.C) and `Vortex_ovr143_E09` (PLAYER.C) take the argument their callers pass and they do not read, as an unused `int` (UW2's way: an unused parameter costs Turbo C nothing).
- `tools/widths.py` rewrote 8 specifiers the readability pass had left (two local arrays whose address goes out as `int16 *`, ARC.C's `(int *)&diff`, a `long` cast, a static); TEXTMAPS.C's `n`, whose address goes to `load_tr_mem` as an `int16 *`, is `int16` by hand.
- SOUND.C reads UW.CFG's card numbers with `%d` into byte globals (`sound_card`, DS:13A; `speech_card`, 13B), and Borland's `%d` stores 16 bits, so the speech card's high byte lands in the low byte of `music_driver` (DS:13C, FFFFh in the EXE). The port keeps the three as one record, and Exhume's runtime now gives `sscanf` and `scanf` Borland's widths (`bc_sscanf`: `%d` stores 16 bits, `%ld` 32).

After these: **89 of 89 sources compile, 0 errors, 44 warnings, 259 names for the link.** The warnings left are 27 narrowings of the host library's 64-bit results and of pointer differences (`strlen`, `lseek`, `ftell`, `filelength`; none a pointer), 15 `char *` against `unsigned char *` (11 of them CONVERSE.C's `convoPics`), an empty `if` kept for matching and a `/*` in a comment.

### Layout and promotion

`tools/layoutcheck.py` compares every record in `src/include` under Turbo C (in DOS) and on the host: 43 records, 34 identical, 9 different, all 9 with pointer fields (`Handler`, `MotionParams`, `Arc`, `CutsState`, `Bag`, `SoundBuff`, `DrvrDesc`, `buttongroup`, `Button`). The 19 file records in `[port.layout] file_records` are all identical, among them `Player` (0xD2 bytes) and `LevelBlock` (0x7C08). `get_arc` and `put_arc` are not in `io_calls`: UW1's take the archive's `struct Arc` as their handle.

`tools/intaudit.py` lists 1,495 sites where a value can leave its 16-bit range before it is truncated. UW1's sources were seeded from UW2's after UW2's audit, and the casts it added that were checked here are already in them (DRAWOBJ.C's facing, PHYSICS.C's heading tests, GRDB.C's `(uint16)-1`). The rest is the list to read when a replay disagrees (`build/port/intaudit.txt`).
