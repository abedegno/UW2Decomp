# The system layer: start-up, memory, files and the assembly system library

This page describes the parts of UW1 that sit between the game and DOS: the assembly system library (DOS segment seg019, 17 modules in `src/sys`) with its keyboard, mouse, timer, video and maths code and the C entry points into the 3D renderer; EMS and the shared workspace; the archive files; error exits; small helpers; the debugging leftovers; and the overlay manager (`src/lib`). The declarations and the screen modes are in `src/include/sys.h`, the file and archive routines in `src/include/file.h`. The game's start-up and main loop are the game subsystem's (`src/game/UWEDIT.C`, `MAINLOOP.C`). The candidates for the findings page are in [sys-findings.md](sys-findings.md).

## Files

| File | Segment | What it does |
|---|---|---|
| `STARTUP.ASM` | seg019 module 1 | the entry glue `init` returns through, the shut-down, the EGA palette save and restore, setting the DOS clock |
| `CRITERR.ASM` | seg019 module 2 | the DOS critical-error handler (int 24h) |
| `CPUTYPE.ASM` | seg019 module 3 | CPU detection |
| `BIOSKEY.ASM` | seg019 module 4 | the BIOS keyboard (int 16h) |
| `SYSINIT.ASM` | seg019 module 5 | `init`, its exit routine and `errorexit` |
| `JOYPORT.ASM` | seg019 module 6 | the analogue joystick (probed at start-up, never read in UW1) |
| `KEYQUEUE.ASM` | seg019 module 7 | raw scan codes into key events, shift states, the scan code table |
| `KBDINT.ASM` | seg019 module 8 | the game's int 9 keyboard handler |
| `VIDSAVE.ASM` | seg019 module 9 | the video mode at start-up and exit |
| `MOUSEDRV.ASM` | seg019 module 10 | the mouse through int 33h |
| `TICKS.ASM` | seg019 module 11 | the game clock, 1/256 seconds |
| `SYSLIBL.ASM` | seg019 module 12 | the video start-up and shut-down glue |
| `SYSENTRY.ASM` | seg019 module 13 | far C entries for input and start-up, and the far pointers C reaches seg019's data through |
| `IMATH.ASM` | seg019 module 14 | integer sines, arctangent and square roots |
| `TICKREAD.ASM` | seg019 module 15 | reads the clock's low word |
| `SYSLIBP.ASM` | seg019 module 16 | an empty far routine `render_3d` calls each frame |
| `C3DENTRY.ASM` | seg019 module 17 | the far C entries into the 3D renderer and the frame buffer (`cRender`, `cPlaceFB` ...) |
| `INT0TRAP.ASM` | seg016 | the divide-by-zero trap the C start-up installs |
| `FARDATA.ASM` | far data | the zero-filled far buffers no C file defines (`stdat`, `fade_buffer`, the map notes ...) |
| `EMS.C` | seg012 | EMS driver calls (int 67h) |
| `TMPALLOC.C` | seg042 | the EMS page layout and the 64 KB workspace |
| `ARC.C` | ovr091 | the .ark archives (LEV.ARK, CNV.ARK) |
| `MISCUTIL.C` | ovr154 | directions, start-up checks, far-buffer file I/O, the PLAYER.DAT cipher |
| `ERROR.C` | ovr110 | error codes and fatal exits |
| `UTIL.C` | seg041_37AA | stepping a value, moving along a heading, waiting, dice |
| `DEBUG.C` | ovr106 | `init_debug` (Alt+F4) and the empty `dprintf` |
| `OVR090.C`, `OVR152.C` | ovr090, ovr152 | empty hooks and two that switch to screen mode 16; nothing calls them |
| `OVR127.C` | ovr127 | raises the COM1 interrupt (Alt+F4) |
| `OVERLAY.ASM` (src/lib) | seg046 | Borland's VROOMM overlay manager, the five OVERLAY.LIB modules |

seg019 is UW2's seg021 almost byte for byte: the same 17 modules at the same offsets (docs/NOTES.md, "seg019, the system library"). Its data is seg063 (paragraph 5624, FD72; UW2's `dseg062_62a6`). The modules came from the second library (UWLIB.LIB) except STARTUP, which was linked as an object.

## How C reaches seg019

C never calls the near routines directly. `SYSENTRY.ASM` and `C3DENTRY.ASM` hold far entries that save the caller's SS:SP, load DS, ES and SS with seg063 and SP with 448h (a private stack), call the near routine and switch back. C reaches seg019's data through far pointers in those modules' `_DATA` (`Alt`, `Asc`, `MouseDx`, `Time`, `cPlayer`, `cDbase` ...). `cRender` draws a frame: it switches stacks, notes the tick count, points int 0 at the renderer's divide-overflow handler, clears the frame buffer, runs `render_3d` and `update_cache`, and returns the ticks the frame took. `do_mouseq` lets the renderer run the C routine `MousQUp` mid-frame: `_102B` copies the private stack away and runs it on a small DGROUP stack.

## Input and time

`KBDINT.ASM`'s int 9 handler puts raw scan codes in a 64-byte ring at seg063:0300; `KEYQUEUE.ASM` turns them into key events, counts keys held, tracks the six modifiers and translates through the scan code table `Asc` (seg063:0010; ui.h has the codes). E0-prefixed keys become 60h to 6Fh first, through a table of the 16 the game knows (seg063:0288). `MOUSEDRV.ASM` reads int 33h. The game clock (`TICKS.ASM`, through `Time`) counts 1/256 seconds; SOUND.C's timer callback, which AIL calls 256 times a second, advances it.

## Memory

`TMPALLOC.C`'s `init_mem` allocates an EMS handle of 54 to 68 pages (an even number chosen at random, at least 30 needed) through `EMS.C`, and `mem_setup` lays the pages out: 0 to 3 the workspace, 9 the screen graphics, the textures from 10, the critter animations from 22 in pairs. The four physical pages of the frame are shared, and `crit_inpage`, `obj_inpage1` and `tmap_inpage` record what each holds (0FFh unknown). Code that needs a 64 KB scratch buffer takes the whole frame with `get_workspace` and gives it back with `release_workspace`. UW1 also takes 64 KB of conventional memory (`conv_ws`). `stdat` (FARDATA.ASM) is the big shared far buffer every subsystem lays out its own way: the archive tables (`STDAT_ARC_OFFTAB`, `STDAT_ARC_NAMES`, file.h), the pathfinder's squares, the cutscene state, the pick buffer, panel pictures.

## Archives

A UW1 .ark file is a word, the block count n, then n longs, the blocks' offsets (0 for an absent block); a block's length is the distance to the next block or to the end of the file. There are no flags, lengths or compression (UW2's archives have all three). The caller keeps the open archive in its own `struct Arc` (11 bytes, file.h). `put_arc` appends a new block, overwrites one of the same length in place, and otherwise rewrites the whole archive through `_arc.tmp` with the block moved to the end. `SAVE0\LEV.ARK` holds the levels, the automaps (block 0x1A + level) and the map notes (0x23 + level).

## Errors

Before the game runs, `first_punt` prints the reason and a code with DOS function 9, frees memory and exits; once it runs, `pfatal_code` and `pfatal` copy the message into `cExitMessage` and exit through `free_world`, and seg019's exit routine prints it. A code is an `ERR_*` kind (sys.h) and a number, shown as a letter and three octal digits (`ERR_EMS | 3` is "C003").

## The overlay manager

seg046 is Borland's overlay manager, the same five OVERLAY.LIB modules as UW2's, 7 bytes later in the segment; the link takes it from the library. The overlay segment table ends 13 entries earlier than UW2's.

## Open questions

- What the development build did with Alt+F4's COM1 interrupt (`ovr127_0`), `dprintf` and screen mode 16 (`ovr090_5`, `ovr152_5`).
- Why the start-up still probes the joystick when nothing reads it (UW1 may have had joystick support that was dropped late).
- The IMATH tables (sines, arcsines, arccosines in seg063) have no relocation tying them to a module, so whose data they are is not known (as in UW2).
