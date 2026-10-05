# The ports

Each game's native port is a second build of the same C the byte-matching decompilation compiles to the original EXE, for a modern host: macOS, Linux and Windows. Nothing is rewritten to make it portable that the gate does not prove still builds the same DOS bytes. The game's own code runs unchanged in meaning: the same rules, the same bugs, the same random numbers, the same saved games.

Each game's record of its port, milestone by milestone, with what each step found and the measurements behind it, is that game's PORT.md: [uw2/docs/PORT.md](../uw2/docs/PORT.md) (the first port, and the fuller account) and [uw1/docs/PORT.md](../uw1/docs/PORT.md).

## What the port is made of

- **The game's C,** compiled for the host. Where DOS and a modern host differ (the width of `int` and pointers, far pointers, struct layout, stack contents a routine reads before it sets them), the shared sources use the macros of Exhume's `runtime/include/portable.h`. Under Turbo C each is the original token, so the DOS build is unchanged and the gate proves it. A port-only guard is written `#ifndef __TURBOC__`, and only where DOS itself can never take the other path.
- **The game's assembly,** translated to C by Exhume's `tools/asm2c.py` (the graphics library and the 3D renderer), and checked against the original bytes by the routine fuzzing.
- **Exhume's runtime** (`exhume/runtime`), the generic half shared by both ports:
  - the paragraph map that gives far pointers a meaning;
  - EMS memory;
  - the emulated VGA, PIT and keyboard;
  - Borland's C library;
  - the Miles AIL sound library and its drivers, on emulated OPL and MT-32 chips;
  - the x86 machine the translated code runs on;
  - the SDL3 platform layer;
  - the black box and record and replay.
- **Each game's glue** in `uwN/src/port`: its bindings to the runtime, its far data and its start-up.

## How a port is proved

A recorded session is everything the game read from the outside world while someone played it in DOS: the clock, the keys, the mouse, the time of day and the sound hardware. Replayed, the game reads the same values in the same order, so it does exactly what it did. Each session has a golden reference made from DOS: a digest of the game state, and the screen, at each checkpoint. `make test` replays every session in the port and requires every checkpoint to match, the saved games byte for byte. CI does the same on Linux, macOS and Windows, and from the players' Windows zip itself ([BUILDING.md](BUILDING.md#continuous-integration)).

Single routines are tested on inputs no session gives them by the routine fuzzing, which runs the original bytes in an emulator beside the port's code for them. The [test vectors](../uw2/vectors/README.md) come from the same harness.

## The black box

A player's run of a port records its session to `recordings/` in the port's home folder (the newest five are kept). If the game crashes or misbehaves, that folder replays the session exactly: `python3 exhume/tools/replay.py --config uwN/exhume.toml port RECORD.OUT OUT`, or for UW2 `uw2/tools/replay.py port ...`. A bug report with that folder attached is reproducible here, and becomes a test.
