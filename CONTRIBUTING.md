# Contributing

Thank you for helping. Bug reports with a recording are as welcome as code.

## Reporting a bug

Use the [bug report form](https://github.com/abedegno/underworld-exhumed/issues/new/choose), and attach the newest folder from `recordings/` in the port's home folder, zipped with the log beside it. The recording replays your session exactly, so the bug can be found and then kept as a test.

## Setting up

```sh
git clone --recursive https://github.com/abedegno/underworld-exhumed.git
cd underworld-exhumed
make setup
```

[docs/BUILDING.md](docs/BUILDING.md) has the requirements: your own copies of the games, the Turbo C++ 1.01 and Turbo Assembler 2.0 disk images for the byte-matching builds, Node, Python, mtools and 7z. A port alone needs none of the Borland tools and no game data to build.

## How a change is proved

DOS is the authority. A change is right when the original program, rebuilt from the sources, is still the same, and the port still does what DOS does.

1. **`make check`, the gate.** Every source compiles in DOS to its part of the original EXE, byte for byte, with its fixups and data verified, and the exact link is the original EXE. A change to a shared source, a header or a comment must keep it passing.
2. **`make test`.** The gate, the port, the routine fuzzing against the original bytes, and every recorded session replayed in the port against its DOS golden reference, checkpoint by checkpoint, the saved games byte for byte.
3. **`make test-full`** for anything that touches the runtime, the replay build or the sound: goldens made again from DOS, the UBSan build, the sound drivers against the real ones, the deep fuzzing.

`GAME=uw1` or `GAME=uw2` runs a target for one game. CI runs the same on Linux, macOS and Windows.

## Rules the code keeps

- **The port keeps the original's behaviour,** its bugs included. A fix that would make the port do something DOS does not is wrong, however sensible. Record what you find in the game's `docs/FINDINGS.md` instead.
- **A port-only guard is written `#ifndef __TURBOC__`,** and only for a path DOS itself can never take: a host that stalls a thread, a 64-bit pointer, a write past an array that lands somewhere else on the host. Say in a comment why DOS never takes it.
- **Host differences go through Exhume's `portable.h` macros** (widths, far pointers, `NULLTRAP`, `STACK_JUNK_SET`), which are the original tokens under Turbo C, so the gate proves they cost no byte.
- **No game data, Borland software or secrets** in the repository. `make repocheck` checks the first two.
- **A change to Exhume** (the submodule) is made and proved in Exhume first, then the submodule is moved to it here, with `make test-full` passing for both games.

## Adding a recorded session

Each game's `docs/BUILDING.md` has the steps: write the steps, record the session in DOS, make its golden reference (DOS twice, checked identical), and commit the recording and the golden together.
