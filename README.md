# UW2Decomp

A byte-matching decompilation of Ultima Underworld II: Labyrinth of Worlds (DOS, 1993; developed by Looking Glass Technologies, published by Origin Systems). The C and assembly in `src/` rebuild the shipped `UW2.EXE` with the original Borland tools.

This is a fan research project, not affiliated with or endorsed by the rights holders.

## Status

- **Every code segment is matched.** All of UW2's C, 337,327 bytes in 99 files, compiles with Turbo C++ 1.01 to the same machine code as `UW2.EXE`, and all of its assembly, 71,920 bytes, assembles to the same bytes with Turbo Assembler 2.0. Each file's fixups and data are verified too. Only Borland's C runtime library has no source here.
- **The relinked EXE is identical except for two bytes**, two flags in the overlay segment table ([LINKING.md](docs/LINKING.md#the-two-bytes)). It runs.
- **The game plays from source.** The modding build links sources changed by any size into a working EXE.
- **The source is readable.** Function and global names are the originals from the FM Towns build wherever it has them. The sources are grouped by subsystem, with shared headers, named constants and struct fields, and every file says what it does in the game.

The method and tools are generalised in [Exhume](https://github.com/abedegno/Exhume), a toolkit and set of Claude Code skills for byte-matching decompilation of old DOS games. This repository is its first case study.

## What you need

This repository holds no game data and no Borland software. You need:

- your own copy of UW2 (the GOG release works), with `UW2.EXE` at `~/UWGOG/UW2/UW2.EXE` or named by `UW2_EXE`;
- the Turbo C++ 1.01 disk images, which Borland released free of charge, and the Turbo Assembler 2.0 disk image;
- Node 20 or later, Python 3, mtools and 7z.

[BUILDING.md](docs/BUILDING.md) has the details and the optional extras.

## Quick start

```sh
make setup TC_DISKS="/path/to/Turbo C++ 1.01" TASM_DISKS="/path/to/Turbo Assembler 2.0"
make            # the modding build; prints the path of the EXE
make check      # the gate: every file matches, and the exact link equals UW2.EXE except the two bytes
make boot       # boot the modding build to the main menu and screenshot it
```

`make setup` unpacks the toolchain into `TC/` and `TASM/` and installs the Python and Node packages; it is safe to run again. `make exact` links the matched objects and compares the result with your EXE. `make hooks` installs a pre-push hook that runs `make check`.

## The sources

Each DOS code segment is one original source file. A file's DOS segment is its `/* target: */` line.

| Directory | What is in it | Notes |
| --- | --- | --- |
| `src/game/` | main and start-up (`UWEDIT.C`), the main loop, the player, skills, saving and restoring | [game.md](docs/subsystems/game.md) |
| `src/obj/` | the object lists, object classes, using and looking at objects, animated objects | [objects.md](docs/subsystems/objects.md) |
| `src/inv/` | the inventory | [inventory.md](docs/subsystems/inventory.md) |
| `src/critter/` | critter AI and path finding, critter classes and art pages | [critters.md](docs/subsystems/critters.md) |
| `src/motion/` | physics, collision and movement of the player and objects | [motion.md](docs/subsystems/motion.md) |
| `src/combat/` | combat, damage, missiles, runes and spells | [combat.md](docs/subsystems/combat.md) |
| `src/map/` | the level map, texture maps, lighting | [map.md](docs/subsystems/map.md) |
| `src/3d/` | the 3D view: the render database and object sorting in C, seg004's 14 renderer modules | [3d.md](docs/subsystems/3d.md) |
| `src/gfx/` | seg003's 14 graphics library modules, sprites, art loading, cutscenes, palettes | [gfx.md](docs/subsystems/gfx.md) |
| `src/ui/` | input, the mouse, panels, the message scroll, menus, options, strings, the automap | [ui.md](docs/subsystems/ui.md) |
| `src/conv/` | the conversation interpreter, its built-ins, bartering | [conversations.md](docs/subsystems/conversations.md) |
| `src/event/` | SCD schedules and events, triggers and traps, world events | [events.md](docs/subsystems/events.md) |
| `src/sound/` | sound and music, and the Miles AIL 2.0 API (`AIL.ASM`) | [sound.md](docs/subsystems/sound.md) |
| `src/sys/` | seg021's 17 system modules, memory and EMS, archives and compression, errors, helpers | [sys.md](docs/subsystems/sys.md) |
| `src/lib/` | Borland's overlay manager (`OVERLAY.ASM`, from OVERLAY.LIB) | [sys.md](docs/subsystems/sys.md#the-overlay-manager) |
| `src/include/` | the shared headers, one per subsystem | [CONTRIBUTING.md](docs/CONTRIBUTING.md#shared-headers) |

No UW2 build names its source files, so the file names come from System Shock's source release, the FM Towns names, or what the file does. `map/filenames.tsv` gives the evidence for each ([MAP.md](docs/MAP.md#source-file-names)).

The rest of the top level:

- `docs/`: the documents below.
- `tools/`: the build, matching, linking and map tools ([BUILDING.md](docs/BUILDING.md#the-tools)).
- `map/`, `targets/`, `symbols.tsv` and `matched.txt`: what the tools know about `UW2.EXE`. That is the map of every function, the target table each source is compared with, every shared name's address, and the list of matched segments ([MAP.md](docs/MAP.md)).

## Documents

- [FINDINGS.md](docs/FINDINGS.md): what matching found in the game: likely bugs in the original, game rules, engine findings, dead code and open questions.
- [docs/subsystems/](docs/subsystems/): one page per subsystem on its architecture, data, rules and open questions.
- [BUILDING.md](docs/BUILDING.md): requirements, the make targets, the gate, working on one file, and the tools.
- [CONTRIBUTING.md](docs/CONTRIBUTING.md): the rule every change follows, comments, renaming, shared headers, constants and struct fields.
- [MATCHING.md](docs/MATCHING.md): the compiler switches, the assembler, and what Turbo C's output reveals about the original source.
- [LINKING.md](docs/LINKING.md): the exact link, the two bytes, what is taken from your EXE, and the modding build.
- [LAYOUT.md](docs/LAYOUT.md): every address written as a number, and what still depends on the original layout.
- [MAP.md](docs/MAP.md): the map, the target tables, `symbols.tsv`, `matched.txt` and the source file names.
