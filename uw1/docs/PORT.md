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

## Milestone 4: into the game

Run on 5 October 2026 (Apple clang 21, SDL 3, DOSBox-X and js-dos for the DOS replays, Unicorn 2.1.4 for the driver check). All nine sessions replay in the port identical to DOS at every checkpoint, the saves included (`make verify`), and so do the UBSan build (`make port-debug`) and an AddressSanitizer build, which reports nothing:

| Session | Checkpoints | Also |
| --- | --- | --- |
| `newgame` | 41 | |
| `walk` | 66 | |
| `sound` | 70 | Sound Blaster FM and digital: every driver read DOS's |
| `soundfm` | 70 | every driver read DOS's |
| `soundmt` | 93 | MT-32: every driver read DOS's |
| `items` | 100 | the saved game identical |
| `talk` | 627 | |
| `load` | 17 | the saved game identical |
| `intro` | 73 | |

The goldens were made again from DOS (twice each, identical), since the replay DOS build had changed since they were made.

### Divergences found and fixed

- **Self-modifying code the translation missed.** Both write into the code segment through a computed address, which asm2c's patch check cannot see: `check_flat` copies the flat or the general body over `mxmul` with `rep movs` (INSTANCE.ASM), and `setup_frame_buf` writes a `ret` into the unrolled frame-buffer copier at `_BEC` plus the frame's width (GRENTRY.ASM). Each is now an override in `tools/asm2c_spec.py` that runs what the code block holds: without them a level view's y came out 81 where DOS has 82, and the 3D view was copied over both side panels.
- **A 32-bit parameter.** MOTION.C's `rehead(unsigned heading)`: on the host `heading += 0x8000` passed 16 bits and the compare with `Ppd.heading` failed, so a wall slide skipped a `rand()`. Now `uint16`.
- **A null pointer.** INTERACT.C's `inv_look` calls `checkTrap` before it picks the object, with 0: DOS reads the vector table. `checkTrap` (USEITEMS.C) reads through `FARNULLTRAP`.
- **The panel turns' EMS handles.** PANELS.C takes three one-page handles of its own (EMS.C's `seg012_141`, `_15E`, `_1B1`, which its comment said nothing calls); `mem/ems.c` has them, on the runtime's extra handles.
- **The automap's picture pointer.** `MK_FP(conv_ws_seg, 0)` is a paragraph into a heap block; the paragraph map split it by the block, and seg048's row records differed by 10h. The runtime now keeps the far heap's `MK_FP(seg, 0)` results for an exact match.
- **8.3 names.** PGCACHE.ASM opens the critter pages as `crit\crNNpage.nNN$`; DOS cuts the extension to three characters. The runtime's file layer now does too; the `int 2` at 06E7:7C5D that the talk session stopped at is the open's failure path.
- **The NMI.** `int 2` (MISCUTIL.C's `dbg_break`, INTERP.ASM's unused opcodes) returns at once: in the DOS replays' vector table int 2 points at 0070:000E, DOS's default handler for ints 1 to 4. int 67h's function 5000h (map a list of pages) is emulated.
- **A nested entry into seg019.** `do_mouseq` runs MousQUp in the middle of a frame (`_102B`), and MousQUp can come back into a seg019 entry, which saves its own caller's SS:SP over the frame's; the glue now runs the translated `_FDD` and `_1004` around the call, as C3DENTRY.ASM does.
- **Host layouts in BABL.C.** `load_script` passed a `char arc[12]` to `open_arc` as a `struct Arc`, which is 24 bytes on the host (the stack protector stopped the port); it is a `struct Arc` now. `bab_malloc` returned `current + 1`, 16 bytes on the host, so every block's tag overwrote the end of its string ("Abyss.8"); now `(char far *)current + 8`, as UW2's.
- **A local read from a file.** ARC.C's `open_arc` reads the block count as two bytes into an `unsigned`, whose upper bytes are junk on the host (under ASan's layout the archive failed to open); now `uint16`.
- **A literal written through.** SCROLLIO.C's `scroll_wrap` passes `"\n"` to `scroll_print3`, which cuts a final newline off its text: in DOS the literal is `""` from the first time on. `PERSISTENT_STR` (portable.h) gives the host the same.

### Sound

UW1's drivers are AIL's 1991 release ("Copyright (C) 1991 John Miles"), which the runtime now recognises from the driver file. Comparing SBFM.ADV's code with UW2's DM03.ADV routine by routine, and with AIL 2.14's YAMAHA.INC and XMIDI.ASM, showed:

- Origin's TVFX code (ALE.INC) is UW2's instruction for instruction, 0Dh bytes lower (0523h..0ACDh), so `src/port/sound/tvfx.c` is UW2Decomp's, plugged in as `uw1_tvfx` (`src/port/ailgame.h`, `[sound] extensions`).
- YAMAHA.INC differs in four places: `timbre_status` returns the index + 1; `delete_LRU` keeps the channels' and keys' timbre indexes; `assign_voice` takes the first free voice from 0; `update_priority` returns when no more slots are active than there are voices, and has no test for 0. XMIDI.ASM in four: the beat fraction starts at 0 and `CLEAR_BEAT_BAR` sets it to 0; a time signature sets only the time fraction; `branch_index` leaves the FOR loops; `shutdown_driver` has no `init_OK` test and no `shutdown_synth`. The runtime has each, marked "1991".

`tools/ailcheck.py` runs the user's own drivers in Unicorn against the port's: SBFM.ADV 21,820 register writes in `soundfm` and 21,836 in `sound`, MT32MPU.ADV 24,688 MIDI bytes in `soundmt`, all identical. In `sound`, js-dos's Sound Blaster ended the introduction's speech buffer 6.4 ms before its nominal end, past the runtime's 5 ms window; `ailgame.h` widens it to 8 ms (`AIL_SB_WIN_EARLY`). DOS replaying its own recordings differs from them in 5,711 reads (`sound`), 4 (`soundfm`) and 2 (`soundmt`); the port in none.

### The last stubs

Borland's `delay`, `int86`, `sound` and `nosound` are the runtime's now (`int86`: int 21h through `intdos`, ints 1 to 4 return; `sound`: a PC speaker square wave in the mixer), so the port links with no stubs. No session reaches them.

### Host hazards

- **Literals written through.** clang's `-Wwrite-strings` over the game's C lists 384 places a literal reaches a `char *`; read one by one (grouped by callee), one is written through, SCROLLIO.C's (above). The three UW2 found are UW1's too and were marked before.
- **Reads past the end.** The AddressSanitizer build (with `-fsanitize-recover=address`) replays all nine sessions identically and reports nothing. Its other data layout found the two bugs above (ARC.C's count, and the paragraph map's windows, which were reused in turn: the third new window could take the first's slot while a pointer split through it was still in use, and a far copy then copied a string onto itself; windows are now reused least recently used first). GRCORE.ASM's string entries copy one byte past the string's 0, which on the host read past a literal; `gfx/grcore.c` gives them a padded copy (the byte is junk in DOS too, and not compared). `-fsanitize=array-bounds` reports only GRIDDB.C's `PlayersMap[0] + i`, an index into the whole 64 by 64 map, within the array (UW2 has the same).
- **Null pointers.** clang's static analyzer over the game's C: of its five null dereferences and four uninitialised arguments, one is real, `bab_realloc`'s split with an empty free list (an original bug, now `FARNULLREC`); the rest are paths the code cannot take. UW2's null sites were checked in UW1's sources: TRIGGER.C's `who` and `TriggeringButton` and COMBINE.C's `OpenBag` (UW2's BAGS.C case) are marked as UW2's are.

Changes to shared sources, each proved by `make check`: MOTION.C, USEITEMS.C, BABL.C, ARC.C, SCROLLIO.C, TRIGGER.C, COMBINE.C, EMS.C (a comment).
