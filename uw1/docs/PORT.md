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

## Milestone 2: link

`make port` (Exhume's `tools/portbuild.py`) compiles the game's C, UW1's port C and Exhume's runtime where it is, and links `build/port/uw1port`. `tools/portstubs.py` writes a stub for every name nothing defines yet: a function that names itself and stops the port, or zeroed storage for data. Run on 4 October 2026 (Apple clang 21, SDL 3), the port links, with no warning in the port's C, a 1 MB arm64 executable. Run on the GOG release's data, it loads the far data from `UW.EXE`, starts the game's thread and stops at the first stub it reaches, MODEX.ASM's console print `seg015_1F9B_E8`.

The first set of stubs had 85 functions and 81 variables in 20 files. UW1's own port C, written from UW2Decomp's (UW1's system library is UW2's almost byte for byte) and from the link's map for every address, took them to **69 functions and 17 variables in 9 files**:

| File | Replaces | What |
| --- | --- | --- |
| `src/port/portgame.h`, `asmgame.h`, `ailgame.h` | | UW1's bindings for the runtime: the names and DGROUP (5AACh) and `_ctype` (DS:1E00), the exit chain, the black box, the window, finding the game (`UW.EXE` in a `UW1` folder, GOG's product 1207658937), the far data blocks; the code segments seg003 (0090h) and seg004 (06E7h) for the x86 machine; the game's AIL declarations (no FM extension yet) |
| `mem/fardata.c` | FARDATA.ASM and the far data extract.py takes from the EXE | FD51 to FD63 (395B:0000 to 54D7:105C) as one block in the EXE's layout with the C's names as labels at their offsets, seg019's data seg063 (FD72, 5624:0000), seg003 and seg004 as a code block, DGROUP's image; all read from the user's `UW.EXE` (547,248 bytes, CRC-32 F2BF2527h), with the EXE's relocations applied at the port's load segment |
| `mem/dgroup.c` | the DGROUP gaps | the eleven names extract.py places; `dseg_5c99_12B6` gets its byte (64h) from the EXE's DGROUP |
| `mem/ems.c` | EMS.C (dos-only) | seg012's entry points on the runtime's EMS memory; the three routines nothing calls stop the port |
| `mem/nulls.c` | | what DOS reads through a null pointer: DS:0..3 `3F F1 02 00`, int 0 at `int0_trap` (1DEA:000E) |
| `sys/sysentry.c` | SYSENTRY, STARTUP, SYSINIT, CPUTYPE, VIDSAVE, SYSLIBL, TICKS, TICKREAD, JOYPORT's start-up | seg019's start-up and shut-down, the far pointers into seg063 (MouseDx, MouseDy and cExitMessage at 0448, 044A and 044C, where UW2's are at 0510..0514; no `cJoyInit`), `mouse`, `mbuttons`, the clock |
| `sys/keyqueue.c`, `sys/mousedrv.c` | KEYQUEUE, KBDINT, MOUSEDRV | UW2Decomp's, renamed; UW1's KEYQUEUE.ASM has an `int 2` when a shifted key goes down, which the port leaves out |
| `sys/int0trap.c`, `sys/overlay.c` | INT0TRAP (seg016), OVERLAY.ASM | UW2Decomp's |
| `sys/main.c` | | the port's main and options, UW2Decomp's adapted: `--sound` takes UW1's card numbers (SOUND.C's driver tables) and writes UW.CFG's three lines; a first run has no sound cards until the port's sound is checked against UW1's drivers |
| `3d/pgcache.c` | PGCACHE.ASM's data | `grs_off` and `obj_tab`, into seg051 |
| `x86/modtab.c`, `x86/glue.c` | | the translated modules' and the glue's tables, empty until a module is translated |

The stubs left are the graphics library (GRCORE.ASM 22 functions and 11 variables, MODEX.ASM 14, SPRITE.ASM 8, VALLOC.ASM 5, LPFDELTA.ASM 1), the 3D entry points (C3DENTRY.ASM 13 and 6), SETPNT, four Borland calls (`delay`, `int86`, `sound`, `nosound`) and `port_render_tag`.

## Milestone 3: boot to the title, and the way to the game

Run on 5 October 2026 (Apple clang 21, SDL 3, DOSBox-X for the DOS replays). The port boots the GOG release's data through the Origin and Blue Sky screens, the title, the introduction and the main menu, and through character creation to the game screen. Replayed against the goldens made in DOS (`make verify`, Exhume's `replay.py verify`), the screens and the game state are DOS's byte for byte all the way:

| Session | Identical checkpoints | What they cover |
| --- | --- | --- |
| `intro` | all 73 | boot, the title, the whole introduction (four and a half minutes) and the main menu |
| `newgame` | 39 of 41 | boot, the title, Escape into the introduction and out, the main menu, every screen of character creation, the name, the game screen drawn (`CHECKPOINT(4)`) |
| `walk`, `sound`, `soundfm`, `items`, `talk` | the first 35 (to `CHECKPOINT(4)`) | the same way in; `sound` and `soundfm` with a Sound Blaster, through the runtime's sound library |
| `soundmt` | the first 41 | the same with the MT-32 |

Every session first differs at `CHECKPOINT(5)`, when the game screen has faded in: the 3D view and both side panels differ (milestone 4). `talk` stops at the `int 2` that do_uwsetup's opcode DAh leads to (06E7:7C5D, docs/NOTES.md), `items` crashes in the game, and `load`, which replays `items`' saved game, waits for it.

### The graphics library and the rest of the assembly: translated

UW1's port translates its assembly modules instruction by instruction with Exhume's `tools/asm2c.py`, where UW2Decomp's wrote much of its graphics library by hand: the routines the game's C calls with C arguments get a small entry in C that pushes the arguments as Turbo C's far call did and runs the translation from the routine's first instruction (`x86/entry.c`), so the library runs the original's own code from the call on, its data in seg048 is DOS's, and the one thing written by hand is how the arguments get there. `tools/asm2c_spec.py` is UW1's spec: 32 modules, about 92,000 lines of generated C.

| Modules | Code | Entry points in C |
| --- | --- | --- |
| seg003, the graphics library: 17 modules (all but MAPDATA, which is data) | `gfx/*.c` | `gfx/grcore.c` (GRCORE.ASM's 25 C entries and its _DATA, the graphics globals) |
| SPRITE (seg000), VALLOC (seg001), LPFDELTA (seg002), MODEX (seg015_1F9B) | `gfx/sprite.c`, `valloc.c`, `lpfdelta.c`, `modex.c` | `gfx/asmentry.c` |
| seg004, the 3D renderer: all 9 modules | `3d/*.c` | through C3DENTRY |
| IMATH and C3DENTRY (seg019) | `sys/imath.c`, `sys/c3dentry_x.c` | `sys/c3dentry.c` (the 13 entries and the far pointers of its _DATA) |

What the translations need from UW1's side, all in `tools/asm2c_spec.py` or `src/port`:

- Overrides for single instructions: MODEX.ASM's three loads of GRCORE's far pointers in DGROUP (`palette`, `dseg_5c99_2404`, C objects in the port); SPRITE.ASM's four `mov ax,DGROUP` (the C stack's segment); PGCACHE.ASM's `_Palettes` (as UW2's); EXPAND.ASM's patched `ret` at 01A6 (as UW2's); SCALEBM.ASM's call of the scaler its generators write at L1F3A, which `gfx/scalebm_code.c` interprets (UW2Decomp's interpreter, at UW1's address).
- C3DENTRY's `_102B` (the renderer's MousQUp on DGROUP's stack) is C (`x86/glue.c`), with glue for the C that SPRITE.ASM calls (`mouse_hide`, `mouse_show`, `pic_to_screen`, `mask_to_screen`) and TICKREAD's and SYSLIBP's entries.
- `x86/glue.c`'s `asm_int`: int 10h's mode set and display combination (a VGA with a colour display), int 21h's string print, set vector (int 0, which cRender points at its handler), and file calls, int 67h's page map.
- `x86/divfault.c`: the renderer's divide fault handlers, by the handler offset it keeps at seg063:04D5: INSTANCE.ASM's three in C, the recovery paths run as translated code with the CPU's frame pushed.
- The game clock is the doubleword at seg019:0710 in the code block, where DOS keeps it, so the translations and the C read the same clock.
- `SETPNT.ASM` is UW2Decomp's `setpnt.c` (the module is UW2's byte for byte).

### Found on the way

- **The far heap starts where DOS's does.** DOS's first far heap block is 6955h paragraphs above the load segment (past the image, the stack and the overlay manager's buffer); the runtime's default heap start put the port's first block elsewhere, and seg048:55EA, the segment VIDMODE draws to, differed from the first key press on. `portgame.h` sets `PORT_HEAP_FIRST`.
- **A 16-bit clock difference.** MAINMENU.C's colour cycling tests `(unsigned)GAME_TIME() - cycle_time >= 0xE`, which on the host is a 32-bit difference: when the clock passed 10000h the menu cycled the palette on every pass where DOS waited. Now `(uint16)((uint16)GAME_TIME() - cycle_time)`, the same bytes under Turbo C (the intro session found it, at the main menu after four minutes).
- **`create_sprite`** is called with one argument and with three, and PANELS.C had no declaration; `gfx.h` now declares it `OLDSTYLE((int layer, ...))`, so the host's calls pass the variadic arguments as its definition reads them (Apple's arm64 passes variadic arguments on the stack).
- **The player's saved games.** The DOS replays leave the game folder's SAVE1 to SAVE4 out (`[replay] data_skip`); the port's replays did not, so a port replay of `newgame` found saved games and skipped the introduction. Exhume's `replay.py` now gives the port the same view.

Changes to shared sources in this milestone, each proved by `make check`: MAINMENU.C's clock difference, gfx.h's `create_sprite`. Stubs left: four Borland calls (`delay`, `int86`, `sound`, `nosound`) in one file.
