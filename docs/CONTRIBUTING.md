# Contributing

The sources rebuild `UW2.EXE` byte for byte, and every change has to keep it that way. The gate proves it.

## The rule

- Every change must pass `make test` before it is pushed: the gate (`make check`), the port build, the routine fuzzing and every replay session in the port against its DOS golden. `make hooks` makes that automatic ([BUILDING.md](BUILDING.md#testing)).
- A readability change (names, shared headers, `#define`s and enums, struct fields, comments, file renames) must keep the bytes identical, and the gate passing is the proof.
- A change meant to alter the program, such as a fix or a mod, belongs in the modding build ([LINKING.md](LINKING.md#the-modding-build)), not in the matched sources.
- Use the original names where the FM Towns build has them. When two files disagree about a name, the FM Towns code decides. DOS is the authority on bytes.

## Comments

Each source file starts with a comment on what it does in the game. Two tags mark comments that explain the code rather than the game:

- `match:` explains a code shape kept for byte matching, such as an odd cast, a redundant statement or an open-coded write a macro would change.
- `name:` gives the evidence for a name.

Findings that matter beyond one file go in the subsystem's page in [subsystems/](subsystems/) and, if they are bugs, rules or open questions, in [FINDINGS.md](FINDINGS.md).

## Renaming

Renaming a function or global also renames it in `symbols.tsv`: change it in every file that uses it, replace its line in `symbols.tsv`, and the gate's from-scratch rebuild of `symbols.tsv` shows anything missed. Then run `tools/syncnames.py` on the defining file so its target table uses the new name ([MAP.md](MAP.md#target-tables)).

Renaming or moving a source file needs no tool change; record the new name and its evidence in `map/filenames.tsv`.

## Shared headers

The declarations the sources share live in `src/include/`, one header per subsystem:

| Header | Covers |
| --- | --- |
| `uw2.h` | included by every header: `union Link`, the link word that chains objects into lists, and Turbo C's `<stdio.h>` |
| `portable.h` | included by `uw2.h`: the explicit-width types and the macros both builds share, each the original tokens under Turbo C ([PORT.md](PORT.md)) |
| `sys.h` | start-up, the main loop, memory and EMS, errors, seg017's helpers |
| `file.h` | archives, file I/O, compression, saves |
| `object.h` | object lists, classes, use and look |
| `items.h` | item ids, classes and trap types (included by `object.h`) |
| `map.h` | tiles, textures, lighting, terrain collision |
| `level.h` | a level's block in LEV.ARK, which needs both `map.h` and `object.h` complete, so both include it at their ends |
| `player.h`, `critter.h`, `inv.h` | the player, critters, the inventory |
| `motion.h` | physics records and stepping |
| `combat.h` | combat, missiles, spells |
| `gfx.h`, `view3d.h` | graphics and the 3D view |
| `ui.h` | input, the mouse, panels, the message scroll, strings, the automap |
| `sound.h`, `conv.h`, `event.h` | sound, conversations, world events, SCD schedules and traps |

Each header declares the struct tags its declarations name and includes the headers that define them, so every tag a source sees is complete. A source includes the headers whose names it uses. `tools/tcc.mjs` copies `src/include` into the DOS build directory beside the source, where `#include "name.h"` finds it, so a header may not take the name of one of Turbo C's own. `tools/srcdeps.py` hashes a source together with the headers it includes, so the gate and the modding build recompile a source when one of its headers changes; editing a widely used header recompiles most of the tree.

### Where a declaration goes

- In the header of the subsystem that defines the name, in the section for its defining file, in the order that file first mentions the names. Functions are declared as the defining file defines them.
- Struct and union types live with the subsystem that owns them (`struct Object` in `object.h`, `struct Tile` in `map.h`, `struct Player` in `player.h`). A struct only one source looks inside stays in that source.
- Where files viewed the same bytes differently, the shared type keeps every view the code needs (`struct Object`'s `qn` is a word, two bitfields or a `union Link`), and a file that read a field with another type casts at that use.
- Declarations of a file's own statics and file-local types stay in the file.

### Order matters

A header declaration is the first sight of a name for every file that includes it, including the file that defines it. Turbo C orders a file's publics, and lays out its uninitialised globals, by a hash of the name with ties in order of first sight. So a name tied with a name the headers leave out must stay out too. The gate catches a wrong order as a verify `_BSS` problem or a link stop on the overlay stub order.

### Where files disagreed

The callers were made to agree. Each name has one declaration, the defining file's type unless another type compiles the same in the definer (a function returning only constants can be `unsigned char` or `char`; `TxmTerr` is `unsigned`), and a caller that read it another way casts at the use: `(unsigned char)f()` for a return its file tested with the other signedness, `*(int *)&PickUp` where a file read a byte global as a word.

- Function-pointer parameters have typedefs (`InputFn`, `ArtAllocFn`, `WhoamiFn` and others), and handlers of another type are cast where they are registered.
- Where callers pass an argument the definition does not take, the definition now takes it as an unused `int` (`editexit`, `clearobj`, `player_look_grave`, `seg011_6`, `seg011_2C6`), which costs no byte. Where a caller pushes an `int` for a `char` parameter, the declaration is `OLDSTYLE((params))` from `portable.h` (`RestoreGame`, `SaveGame`, and `create_sprite`, which is called with one argument and with three): Turbo C sees `f()`, the port the real prototype.
- Buffers several files lay out their own way have one declaration and a macro per view (`stdat` is `STILES` in `critter.h`, `STDAT` in `CUTS.C`), and `PlayerDat` is a union of `struct Player` and its 0x37E bytes.

Six names stay declared in their files, because one shared declaration would change a caller's bytes and no cast at the call can undo it: a `char` parameter where callers push an `int` (`advance`, `set_light`, `useNSpellCharges`, `set_numbered_variable`), a name tied with another file's statics (`missile_try`), and an uninitialised far variable (`pathsq`, which an earlier `extern` would make near data). Each declaration says why.

### Adding a name

Declare it in its subsystem's header and delete the file's own declarations of it, then run `make check`. If a file's bytes change, that file needs the name declared its own way: put the declaration back in that file and take the name out of the header.

## Writing for both builds

The sources also build the native port (PORT.md). Every change still has to pass the gate, and these keep a change portable:

- Declare struct fields, globals and file-scope statics with the explicit widths of `src/include/portable.h` (`int16`, `uint16`, `int32`, `uint32`), and never write `long`. A pointer to an integer points to an explicit width. Plain `int` is fine for a scalar local, parameter or return value whose 16-bit wrap does not matter (PORT.md, "Integer widths and wrap"). `tools/widths.py FILE` applies the policy to a file.
- A value that carries a near pointer is a `NEARPTR`; a far pointer's offset is `FP_OFF(p)`; a far copy is `FAR_COPY(dst, src, n)`.
- Cast handlers of another type where they are registered; never leave a call without a prototype.
- A struct with pointer fields goes between `HOST_LAYOUT_BEGIN` and `HOST_LAYOUT_END`.
- A dereference that can see a null pointer in DOS is written through `NULLTRAP(p)` (near) or `FARNULLTRAP(p)` (far), with a comment.
- A file whose code is DOS-only says `port: dos-only` in its header comment, and the port replaces it.
- A function the original wrote with no return statement, whose callers use what it left in AX, ends with `AX_RESULT(v)`, where `v` is that value, with a comment saying where it comes from; when the value is the result of the function's last call, that statement is `AX_LAST(call)` instead.
- Read such compiled behaviour (AX at a function's end, what an uninitialised local holds, how an overflow wraps) from our own build: `tools/match.py --dis NAME` on the matched object shows the same instructions as `UW2.EXE`, with our names. The C cannot say, because it must leave the undefined behaviour in place to keep the bytes; the port-only macro (`AX_RESULT`, `STACK_JUNK`, the width types) records the answer.
- Every read of the world outside the program goes through a replay hook of `portable.h`, so that a session can be recorded and replayed in both builds (PORT.md, "The differential test"): a read of the game clock is `GAME_TIME()`, never `*Time`; and `key()`, `mouse()`, `mbuttons()`, the joystick reads, `time()` and `srand()` are `KEY()`, `MOUSE()`, `MBUTTONS()`, `JOY_READ()`, `JOY_BUTTONS()`, `WALL_TIME()` and `SRAND()`. `CHECKPOINT(n)` marks a place where both builds dump the game state. A read of a sound driver's state is `SND_READ(drv, x)`, and game code a timer interrupt runs is registered through `SLAVE_TIMER(f)` (PORT.md, "Replays with a sound card").
- A store through a far pointer into the VGA's window at A000:0000 is `PLANAR_STORE(p, v)`.
- A local the original reads before it ever sets it is declared with `STACK_JUNK(v)` after its name, with a comment; [FINDINGS.md](FINDINGS.md) lists such sites.
- Where the C writes a sprite or critter opcode into the render database, `RENDER_TAG(o);` before it names the object for the port's per-object sprite hook ([PORT.md](PORT.md#the-render-interface-and-the-sprite-hook)); it is an empty statement under Turbo C.
- A local or field that a file read (`fread`, `read`, `intoFarBuffer_ovr167_5DA`) fills with a word has an explicit width; a plain `int` would keep two bytes of junk on the host.
- A struct with pointer fields that the code lays straight over file data is reached through `FILE_RECORDS(T, p, n, layout)` and `FILE_RECORDS_END(q, n)` (CHARGEN.C's `DATA\chrgen.dat`).
- Port-only C (the replacements for the assembly modules, the emulated hardware, the platform layer) lives under `src/port` and never in a game source; a file there says in its first line which module it replaces, or "replaces nothing".
- A routine of a translated module has one implementation in the port (PORT.md, "One implementation per routine"). To replace the translation of a routine with C written by hand, add its range to `tools/asm2c.py`'s `HANDWRITTEN`, run the tool, give every place the remaining translation reaches in it a glue entry (`x86/glue.c`; the port lists any it lacks at start-up), and replay the sessions that reach it. If no session reaches it, add the routine to `tools/fuzzasm.py`'s targets, so that its C is compared with the original's bytes on every `make test`.

Each of these is the original tokens under Turbo C (port-only C is never seen by it). After a change, `make port-check` should show no new error or warning, and `make test` must pass: it builds the port and replays every session against its golden.

A session that differs from its golden after a change to the port is a bug in the change; fix the port, never the golden. The goldens are made again (`make golden`, or `make test-full`) only when a recording or the replay DOS build changes (a change to `src/replay/REPLAY.C` or to a source the replay build compiles with `-DREPLAY`), and the regenerated files go in the same commit as that change. `replay.py verify` says when a golden is stale or was made by another replay build.

## Named constants

Named constants live in the same headers, beside the declarations they go with:

- `items.h`: every item id by its name in the game's own string block 4, the major classes by their FM Towns names (`MAJOR_HACK` to `MAJOR_ANIMOBJ`), the minor classes, the classes and range starts, and the trap types.
- `object.h`: the object word masks (`ID_*`, `POS_*`, `HOME_*`, `LINK_SPECIAL`), list sizes and the container kinds.
- `ui.h`: the string blocks (`STR_*`, `STRBLK_*`), key modifiers, and the special keys' codes from seg021's scan code table (`KEY_F1`, `KEY_UP` and so on, written `KEY_ALT | 'h'` where a modifier is added).
- `gfx.h`: fonts and palettes. `map.h`: tile types, map size, levels and terrain classes. `player.h`: skills, classes, quest bytes, X clocks and the day's 72 steps. `combat.h`: spell classes and runes. `sys.h`: error kinds. `sound.h`: the music themes (theme n is `UWAnn.XMI` with n in octal). `event.h`: the wall bits of `tile_walls`.

The assembly modules name the BIOS, DOS, mouse and EMS functions they call, the VGA ports and the model interpreter's opcode table with `equ`s defined before their first use (a forward `equ` makes single-pass TASM reserve room and pad with `nop`).

Each group says where its names and values come from. A literal stays a literal where nothing shows what it means. [MATCHING.md](MATCHING.md#named-constants) has what the compiler allows.

## Struct fields, not offsets

Code reads a struct's bytes through its fields, never through a cast pointer plus an offset. Where files viewed the same bytes as different structs, the shared struct has the fields (struct Player's `saved_x` to `saved_level` and `terrain`, struct Tile's wall texture through `TILE_WALL`). Records with no shared type before have one now: `struct LevelBlock` (`level.h`, with `LEVEL` for `mapdata` seen as one), `struct Bitmap` and `struct FontInfo` (`gfx.h`), `struct SCDRow` and its parameters (`event.h`), `struct Armour` (`object.h`), `struct StaticTile` (`critter.h`), `struct Gloc` (`view3d.h`), and `struct Anim` and `struct AnimClass` (`object.h`). Each struct comment says where its field names come from and which are provisional.

## Accessor macros

The accessor macros for an object's packed fields are in `object.h`, one set for every file: `OBJ_ITEM`, `OBJ_MAJOR` to `OBJ_HOMEX` read a field (mask then shift, as the original's macros did) and `SET_ITEM` to `SET_HOMEX` write one. Two have a second spelling because files compile them differently, and the comment above the set says which.

Open-coded reads that are token for token a macro's body use the macro, and so do open-coded writes of a field: `x->pos = x->pos & 0xFC7F | (h & 7) << 7` is `SET_HEADING(x, h)`, and a constant or a cleared field folds to the same code. The two writes a macro would change keep their open form with a `match:` comment.
