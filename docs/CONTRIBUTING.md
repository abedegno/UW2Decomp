# Contributing

The sources rebuild `UW2.EXE` byte for byte, and every change has to keep it that way. The gate proves it.

## The rule

- Every change must pass `make check` before it is pushed; `make hooks` makes that automatic ([BUILDING.md](BUILDING.md#the-gate)).
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
- A few functions are declared without a prototype, `f()`, because their callers pass arguments the definition does not take (`editexit`, `bab_fun`, `RestoreGame`; each says so).
- Buffers several files lay out their own way have one declaration and a macro per view (`stdat` is `STILES` in `critter.h`, `STDAT` in `CUTS.C`), and `PlayerDat` is a union of `struct Player` and its 0x37E bytes.

Six names stay declared in their files, because one shared declaration would change a caller's bytes and no cast at the call can undo it: a `char` parameter where callers push an `int` (`advance`, `set_light`, `useNSpellCharges`, `set_numbered_variable`), a name tied with another file's statics (`missile_try`), and an uninitialised far variable (`pathsq`, which an earlier `extern` would make near data). Each declaration says why.

### Adding a name

Declare it in its subsystem's header and delete the file's own declarations of it, then run `make check`. If a file's bytes change, that file needs the name declared its own way: put the declaration back in that file and take the name out of the header.

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
