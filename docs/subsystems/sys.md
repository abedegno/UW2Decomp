# The system layer

This page describes the parts of UW2 under the game: start-up and shutdown, the keyboard, mouse, joystick and clock drivers, integer maths, expanded memory and the shared workspace, the .ark archives and their compression, error exits, and Borland's overlay manager. The sources are in `src/sys/` and `src/lib/`; the declarations are in `src/include/sys.h` and `src/include/file.h`. Statements marked "probably" are inferences; the reason is given with each.

## Files

seg021 is a library of 17 small assembly modules, the "system layer" (README, "Linking"; `map/filenames.tsv` lists them in module order):

| File | Name | What it does |
|---|---|---|
| `STARTUP.ASM` | descriptive | start-up and shutdown glue, the EGA palette save and restore, setting the DOS clock from the real-time clock |
| `CRITERR.ASM` | inferred (System Shock's `CRITERR.C`) | the DOS critical-error handler (int 24h) |
| `CPUTYPE.ASM` | descriptive | the CPU type: 8086, V20/V30, 186, 286, 386 |
| `BIOSKEY.ASM` | descriptive | the BIOS keyboard (int 16h) |
| `SYSINIT.ASM` | descriptive | `init`, the exit routine and `errorexit` |
| `JOYPORT.ASM` | descriptive | the analogue joystick on port 201h |
| `KEYQUEUE.ASM` | descriptive | scan codes to key events: prefixes, key states, modifiers, translation |
| `KBDINT.ASM` | descriptive | the game's own int 9 keyboard handler and the keyboard LEDs |
| `VIDSAVE.ASM` | descriptive | saving and restoring the video mode and the BIOS equipment byte |
| `MOUSEDRV.ASM` | descriptive | the mouse through int 33h |
| `TICKS.ASM` | descriptive | the game clock, a doubleword counting 1/256 seconds |
| `SYSLIBL.ASM` | descriptive | the video save and restore at start-up and exit (probably FM Towns' `initvideo` and `resetvideo`) |
| `SYSENTRY.ASM` | descriptive | the far C entry points for input and start-up; the far pointers C uses to reach seg021's data |
| `IMATH.ASM` | descriptive | table sines and cosines, arcsines, `atan2`, integer square roots |
| `TICKREAD.ASM` | descriptive | reads the clock |
| `SYSLIBP.ASM` | descriptive | an empty far routine the renderer calls each frame |
| `C3DENTRY.ASM` | descriptive | the far C entry points into the 3D code (`cRender`, `cPlaceFB` ...) and the maths |

And the rest:

| File | Segment | Name | What it does |
|---|---|---|---|
| `INT0TRAP.ASM` | seg018 | descriptive | the divide-by-zero trap for the C code |
| `FARDATA.ASM` | (far data) | descriptive | the far data no C file defines: the frame buffer and work area `stdat`, compression buffers, sprite and allocator tables |
| `EMS.C` | seg013 | descriptive | EMS driver calls (int 67h, EMS 4.0) |
| `TMPALLOC.C` | seg042 | inferred (System Shock's `TMPALLOC.C`) | the EMS page layout and the 64 KB workspace |
| `UTIL.C` | seg041 | descriptive | small helpers: `mvcheck`, `move_along`, a tick wait, `rollem` |
| `ARC.C` | ovr093 | inferred (the `arc_` prefix) | the .ark archives |
| `LZSS.C` | ovr127 | inferred (Okumura's file) | Haruhiko Okumura's LZSS, for archive blocks |
| `ACLZW.C` | ovr153 | descriptive | reading and writing a compressed archive block |
| `ERROR.C` | ovr114 | descriptive | error codes and fatal exits |
| `MISCUTIL.C` | ovr167 | descriptive | directions, start-up checks, far-buffer file I/O, the PLAYER.DAT cipher |
| `DEBUG.C`, `STUBS.C`, `STUBS2.C`, `EMPTY.C`, `COM1INT.C` | ovr109, ovr092, ovr165, ovr146, ovr132 | descriptive | debugging hooks and empty or unused functions |
| `src/lib/OVERLAY.ASM` | seg046 | inferred (named after OVERLAY.LIB) | Borland's VROOMM overlay manager, five library modules |

## seg021: how C reaches it

seg021 has its own data segment, `dseg062_62a6` (FD71, 60B9:0000, C40h bytes, taken from the EXE at link time), and its own stack in it. C never calls a seg021 routine directly: it calls a far entry point in SYSENTRY.ASM or C3DENTRY.ASM, which saves the caller's SS:SP in its code segment, loads DS, ES and SS with FD71 and SP with 510h, calls the near routine and switches back. C reads seg021's state through far pointers in DGROUP (`Shift`, `Alt`, `Ctrl`, `key_on`, `Asc`, `MouseDx`, `MouseOn`, `joy_position`, `Time`, `cPlayer` ...). SYSENTRY.ASM's header has a map of FD71.

The 3D renderer also runs on that stack (`cRender`), and its data includes seg021's tables (the sine table, the camera `cPlayer`, the view window). During a frame the renderer calls back into C for the mouse (`do_mouseq`); C3DENTRY.ASM's `_102B` copies the renderer's part of the private stack away first, because the C routine may itself enter seg021 and reset SP to 510h.

## Start-up and shutdown

GRFX.C's `grfx_init` calls SYSENTRY.ASM's `_755`, which runs STARTUP.ASM's `_0`: it points FD71:0 at the PSP's terminate address, saves the EGA palette and calls `init` (SYSINIT.ASM). `init` resets the mouse, finds the CPU type, records the 3D data segment, hooks int 24h, checks for a Tandy, finds the joystick, installs the int 9 keyboard handler and saves the video mode. It then puts its own exit routine in the PSP's terminate address, so it runs however the program ends: it calls an exit hook (empty in UW2), removes the keyboard handler, and prints the message `cPerror` points at, which is how `pfatal` (ERROR.C) reports a fatal error after the screen has left graphics mode. `grfx_close` runs `_17`, the reverse of `init`, which also sets the DOS clock from the real-time clock.

Errors before the game is running go through `first_punt` (ERROR.C), which prints to the console and exits; codes are a kind (sys.h `ERR_*`) and a number, shown as a letter and three octal digits.

## Input and time

**Keyboard.** The game replaces int 9 (KBDINT.ASM): the handler stores raw scan codes in a 64-byte ring buffer and does not chain to the BIOS. `key` (KEYQUEUE.ASM) assembles the prefixed sequences, keeps a down count per key (`key_on`), ignores hardware repeats unless allowed, works out the modifiers and translates through `Asc`. The keyboard layout is chosen once: enhanced 101/102-key keyboards are told apart by the BIOS flag at 0040:0096.

**Mouse.** MOUSEDRV.ASM resets the int 33h driver and reads relative motion (scaled to half a mickey per unit), position and buttons; `mouse` leaves the motion in `MouseDx` and `MouseDy`.

**Joystick.** JOYPORT.ASM times the four axes of the game port, keeps a centre per connected axis found at start-up, and gives positions from -127 to 127 with a dead zone of 4; JOYSTICK.C uses it.

**Clock.** The game clock is the doubleword `Time` points at (TICKS.ASM), incremented 256 times a second by an AIL timer callback (SOUND.C's `cllbck_tst`). The 3D renderer times frames with it and `cInit3d` resets it.

**Maths.** IMATH.ASM's tables give sines and cosines at 1/256 turn with linear interpolation, arcsines and arccosines, an `atan2` for unit vectors, and Newton's-method square roots; angles are words, 10000h to the turn.

## Memory

UW2 needs EMS 4.0 (EMS.C). TMPALLOC.C lays the logical pages out: the 64 KB workspace (pages 0 to 3), screen graphics, the level's textures, digitised sounds, the critter animation table and the critter animation cache (pages from 33), with the art LOADGR.C loads probably in pages 4 to 9. The four physical pages of the frame normally hold a critter page pair, an art page and a texture page, and the code that maps them (here, PGCACHE.ASM, PANELS.C, SOUND.C) keeps track of what is mapped. Code that needs a large scratch buffer takes the whole frame with `get_workspace` and gives it back with `release_workspace`.

`stdat`, the large far buffer in FARDATA.ASM, is shared by several jobs, which probably never overlap in time (see the open questions): the 3D view's frame buffer (seg003 calls it `seg049_3EE2`), the LZSS work area and file buffer (ACLZW.C), the archive tables (ARC.C), the cutscene player's state (CUTS.C) and a copy of the level's tiles for CRITTIME.

## Archives and compression

The data files LEV.ARK, CNV.ARK, BYT.ARK, SCD.ARK and the rest are archives (ARC.C): a block count, then tables of each block's offset, flags, length and reserved room. One archive is open at a time. A block flagged compressed holds its uncompressed length and an LZSS stream (ACLZW.C, LZSS.C): Okumura's 1989 LZSS with a 4096-byte ring buffer, which UW-Formats describes for UW2's .ark files. A block flagged compress-on-write is compressed when saved; a block that has outgrown its room makes `put_arc` rebuild the whole file.

## The overlay manager

Most of UW2's C code is in overlays: one buffer in conventional memory holds the overlay code in use, and calls into an overlay go through stubs whose int 3Fh brings the overlay in when needed (OVERLAY.ASM). The buffer is 300h paragraphs (UWEDIT.C's `_ovrbuffer`) or twice the largest overlay, whichever is larger, allocated before `main`. TMPALLOC.C calls `_OvrInitEms(0, 0, 0)` so that overlays are kept in EMS and reloaded from there. The module is Borland's own code from OVERLAY.LIB, equal to the library objects outside their fixups.

## Key structures

- FD71 (`dseg062_62a6`), seg021's data: the map is in SYSENTRY.ASM's header.
- `struct Camera` (`cPlayer`, view3d.h), which lives in FD71.
- `struct ArcFile` and the archive tables (ARC.C), `struct LzwWork` (LZSS.C).
- The EMS page variables `crit_fpage`, `tmap_fpage`, `EmsBuff` ... (view3d.h; defined in the renderer's far data by TMAPOPS.ASM).

## Open questions

- FD71:0120 is read by the keyboard layout choice and by the joystick set-up, and nothing found writes it; it is probably a setting, but which is not known.
- `stdat` (FARDATA.ASM entry 52) holds the automap notes (`ATM_Strings`, 468Eh in), and seg003's `clear_fbuf` clears the frame buffer over the same bytes (34E9h words from offset 2) at the start of every 3D frame. The notes can only survive if they are kept there just while no 3D frame is drawn (the map screen) and saved elsewhere otherwise; not checked. ACLZW.C's work area also runs past `stdat`'s 6AA6h bytes into the compression buffers after it.
- The joystick calibration routines (`_375`, `_380`, `_38F`) have no caller traced.
- The far entries after `_809` in SYSENTRY.ASM have no labels and so no callers.
- `_C10` (SYSLIBP.ASM) is empty; `render_3d` calls it on a separate stack every frame, so it was probably a hook for something removed from the shipped game.
