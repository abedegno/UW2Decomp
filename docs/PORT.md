# Porting UW2 to modern systems

This page is the design for a native port of UW2 built from the matched sources. Nothing described here beyond Milestone 1 exists yet. Milestone 1, a compile-only measurement of the C on a modern host, is done, and its numbers are below.

## Goals and non-goals

The port comes in two phases. Phase 1 is a faithful port, and phase 2 adds enhancements on top of it.

Phase 1 runs the original game as it was. It draws the original 320 by 200 picture with 256 colours, keeps the original timing (the 256 Hz game clock, the 18.2 Hz BIOS tick the game assumes, the 16 Hz effects timer), plays the original music and sound, and shows the result in a scaled window. Save files and data files stay byte-compatible with DOS, so a save moves between the port and DOS in both directions. The likely bugs in [FINDINGS.md](FINDINGS.md) are reproduced by default, and each fix becomes an option later.

Phase 2 adds options to the same port where they are cheap, e.g. integer scaling, aspect correction, mouselook and the FINDINGS fixes. A higher resolution or widescreen 3D view needs a new renderer, and that work goes in a fork that keeps phase 1 as its reference. Chocolate Doom and Crispy Doom split their work the same way.

The first platform is macOS on Apple Silicon. The code is 64-bit clean from the first day, so Linux and Windows on x86-64 should need only build files. Where it costs little, the platform layer and the renderer are kept independent of UW2, so that a later UW1 decompilation can use them too.

These are not goals:

- emulating the DOS executable, or bundling DOSBox;
- shipping game data, or any build that embeds it (users bring their own copy; the repository and the binary hold none);
- binaries for download (the port is distributed as source only, as the rest of the repository is);
- changing the matched sources in a way that changes `UW2.EXE` (the gate keeps proving the DOS build);
- networking, new content, or mod tools in phase 1.

## One tree, two builds

The DOS build and the port are built from the same sources. The SM64 decompilation works this way too, and its PC port is a second build of the same tree.

The DOS build is unchanged. `make check` still compiles every source with Turbo C++ 1.01 and TASM in DOS, and still proves the exact link and the modding build. The port compiles the same C files with a modern compiler, swaps the assembly modules for C written for the port, and links them with a platform layer.

Every change to a shared source has to pass the gate. So a change made for the port must compile to the same bytes under Turbo C. Most portability changes can do that, because a typedef, a macro that expands to the original tokens, or a prototype that agrees with the definition costs no bytes. [MATCHING.md](MATCHING.md) lists what Turbo C allows. A change that would alter the DOS bytes goes in port-only code, never in a shared source.

### Directory layout

| Path | What is in it | Seen by the DOS build |
| --- | --- | --- |
| `src/<subsystem>/*.C` | the game's C, shared by both builds | yes |
| `src/<subsystem>/*.ASM` | the original assembly, DOS build only | yes |
| `src/include/` | the shared headers | yes |
| `src/port/compat.h` | the portability layer, force-included into every port compile | no |
| `src/port/include/` | stand-ins for Borland's headers (`dos.h`, `alloc.h`, `mem.h`, `io.h`, `stat.h`, `dir.h`, `conio.h`, `bios.h`) | no |
| `src/port/3d/`, `src/port/gfx/`, `src/port/sys/`, `src/port/sound/` | C written for the port to replace the assembly modules and the DOS-only C files, named after the module each replaces | no |
| `src/port/mem/` | the paragraph map, EMS and the far heap | no |
| `platform/include/plat.h` | the platform API | no |
| `platform/sdl3/` | the SDL3 implementation of the platform API | no |
| `tools/portcheck.py` | the Milestone 1 measurement (`make port-check`) | no |
| `build/port/` | port objects and logs, never committed | no |

The DOS tools never pick up port files, for three reasons:

- `tools/sources.py` lists every `.C` and `.ASM` under `src/` except `src/include` and `src/port`, so the gate, the link and the modding build never compile a port file.
- `tools/tcc.mjs` copies only `src/include` into the DOS build directory, so the stand-in headers never shadow Turbo C's own.
- `tools/srcdeps.py` hashes only the headers in `src/include`, so editing a port header never recompiles a DOS object.

Port files use lower-case `.c` names, and each one starts with a comment naming the module it replaces.

## The portability layer

The portability layer is `src/port/compat.h` and the stand-in headers. The port compiles every game source with `-include src/port/compat.h -I src/port/include`, so the game sources need no `#ifdef` to get it. The DOS build never sees these files.

At Milestone 1 the layer does these things:

- It defines away Turbo C's memory-model keywords (`far`, `near`, `huge`, `interrupt`, `cdecl`, `pascal`).
- It turns `MK_FP`, `FP_SEG` and `FP_OFF` into calls into the paragraph map (below).
- It maps the pseudo-registers `_AX`, `_AH` and the rest onto a register file that the port's interrupt emulation reads and writes.
- It gives Borland's values to the `open` flags and permission bits. The sources also write them as numbers, e.g. `open(name, 0x8001)` in MISCUTIL.C and the mode `0x80` in ARC.C, and Borland's `O_RDONLY` is 1 where the host's is 0.
- It renames the Borland library calls whose host versions behave differently to `bc_` names, so that the port supplies Borland's behaviour. These are `open`, `read`, `write`, `close`, `lseek`, `creat`, `access`, `unlink`, `mkdir`, `chdir`, `stat`, `fstat`, `fopen`, `rand`, `srand`, `time` and `clock`. The reasons are DOS paths and case, Borland's text mode and flags, one-argument `mkdir`, Borland's `rand` (its own generator with `RAND_MAX` 7FFFh, called 272 times in the game), `clock` at 18.2 Hz, and the need for the replay harness to control `time` and `rand`.
- It declares Borland's extra library names (`itoa`, `ltoa`, `strupr`, `strnicmp`, `max`, `min`, `environ`) and Borland's `_ctype` table, which GAMESTRN.C reads directly.

The game's translation units see only ISO C and POSIX 2008 names from the host (`-D_POSIX_C_SOURCE=200809L`). Without that, macOS's headers declare `valloc`, which collides with VALLOC.ASM's routine of the same name.

From Milestone 2 the layer also gets the explicit-width types the game's declarations will use, described next.

## Memory model and data layout

UW2 is a 16-bit medium-model program. Code is far, data is near unless a declaration says `far`, `int` is 16 bits and `long` is 32. On macOS on Apple Silicon, `int` is 32 bits, `long` is 64 bits and pointers are 64 bits. Each difference below is a place where the port can silently compute something DOS does not.

### Integer widths and wrap

Turbo C's `int` wraps at 16 bits, and the game relies on that. For example, headings are words with 10000h to the turn, so adding two headings wraps for free. Another example is `move_along` in UTIL.C, which multiplies a sine by a distance in an `int`.

The plan has two parts.

The first part is to declare every variable with an explicit width. A mechanical rewrite of the shared sources replaces `int` with `int16`, `unsigned` with `uint16`, `long` with `int32` and `unsigned long` with `uint32`. Under Turbo C the typedefs are the original types, so the gate proves the rewrite changes no byte. On the host they are `int16_t` and the rest, so stored values wrap as they did in DOS, and `long` is 32 bits again. The rewrite is done by a tool, one subsystem at a time, each step gated.

The second part is an audit of integer promotion. C promotes a 16-bit operand to the host's 32-bit `int` before arithmetic. A result that is stored back into a 16-bit variable is truncated and comes out as in DOS. A result that is compared, divided, shifted right or passed on without being stored first can differ. A `uint16` is also promoted to a signed `int`, so `u - 1 > 0` is true in DOS for `u == 0` and false on the host. A checker built on clang's AST (`-Xclang -ast-dump=json`) lists every arithmetic expression on 16-bit operands whose value reaches a comparison, a division, a right shift, an index or a wider type before it is truncated. Each one gets a cast that gives the DOS result, and each cast is gated. The differential replay test (below) catches what the audit misses.

The alternative of defining `int` as `short` with a macro was rejected. It breaks `long int` and the host's headers, it does not stop promotion, and it hides the types from the debugger.

`char` is signed in Turbo C and on Apple Silicon, and the port passes `-fsigned-char` so that other hosts agree.

### Far pointers and segments

In the port every pointer is a flat 64-bit host pointer. The sources still build far pointers from segment and offset, take them apart, and do arithmetic on segment numbers. Examples:

- `MK_FP(EmsBuff + 0x800, off)` addresses page 2 of the EMS page frame (`EmsBuff` is a segment number, and 800h paragraphs is 32 KB);
- `MK_FP(FP_SEG(stdat) + 0x134, 0)` in CUTS.C, and `FP_SEG(drv_mem[n]) + 1` in SOUND.C;
- `movedata(FP_SEG(src), FP_OFF(src), FP_SEG(dst), FP_OFF(dst), n)`, 34 lines in 14 files;
- `(unsigned)list < FP_OFF(LastActiveMob)` in SCDEVENT.C, which compares far pointers by their offsets only, as Turbo C does for `<`.

The port keeps a paragraph map. The map is a table of regions, and each region is a block of host memory with a range of virtual paragraph numbers. The EMS page frame, the workspace, every `farmalloc` block and each far data array (`stdat` and the rest of FARDATA.ASM, the graphics and renderer data) is a region. `MK_FP(seg, off)` finds the region that holds linear address `seg * 16 + off` and returns the host address. `FP_SEG` and `FP_OFF` find the region that holds a host pointer and return its paragraph and the offset from it. So segment arithmetic inside a region works as in DOS. A pointer that is in no region, e.g. a near DGROUP array passed to `movedata`, gets a temporary region based at that pointer, so that `MK_FP(FP_SEG(p), FP_OFF(p) + n)` is `p + n`. Debug builds report each temporary region, so that the idiom can be replaced with a named macro later.

From Milestone 2 the common idioms become macros in the shared headers whose Turbo C expansion is exactly the original tokens, e.g. a `FAR_COPY(dst, src, n)` for the `movedata` form. A macro that expands to the same tokens compiles to the same bytes. On the host the macro is a `memcpy`, and the paragraph map is then needed only for real segment numbers such as `EmsBuff`.

Offsets wrap at 64 KB inside a DOS segment, and a flat pointer does not wrap. No case of the game relying on the wrap is known. The replay test would show one.

`huge` pointers are not used anywhere.

### EMS

UW2 needs EMS 4.0. The port gives it an EMS emulation with the same calls EMS.C makes, a store of 16 KB logical pages and a 64 KB page frame that is a region of the paragraph map. Mapping a page copies the page that was in that frame slot back to the store and copies the new page in. The game keeps track of what is mapped (PGCACHE.ASM, PANELS.C, SOUND.C), so the frame must hold exactly what DOS would hold. Apple Silicon's memory pages are 16 KB, the same size as an EMS page, so the copies can later be replaced by `mmap` of the store into the frame with `MAP_FIXED`.

### Near pointers kept in integers

A near pointer is 16 bits in DOS, so the sources pass and store them as `int` or `unsigned`. On the host these are 64-bit pointers, and storing one in 16 or 32 bits truncates it. Milestone 1 found 33 of these, e.g. `gronk_critid(..., (int)&copy, ...)` in SCDEVENT.C, which passes a pointer through an `int` parameter, and `*cDbbase = (int)dbptr` in GRDB.C, which writes a pointer into the render database. The render database is a bytecode with 16-bit words, so the port stores offsets from the database's start there, never host pointers. The parameters that carry pointers become pointer-sized types through a typedef that is `int` under Turbo C.

### Struct layout and bitfields

Turbo C lays structs out with byte alignment and puts bitfields in 16-bit units. [MATCHING.md](MATCHING.md) records how Borland places each bitfield: in the 16-bit window that starts at the byte holding the next free bit, so a field can straddle a word boundary. The bits pack as one stream, low bit first, and a struct takes only the bytes it needs.

On the host, clang with `__attribute__((packed))` and 16-bit bitfield types also packs bitfields as one stream, low bit first, so the layouts should agree. That is a hypothesis, and Milestone 2 tests it. A tool, `tools/layoutcheck.py`, generates one C program that, for every struct in the shared headers, sets each field in turn to all ones in a zeroed record and prints the bytes. The tool compiles the program with Turbo C in DOS and with clang, runs both and compares the output byte for byte. Every file record must come out identical before any code that reads or writes it is trusted. Those records are `struct Player` (`PlayerDat`, 37Eh bytes), `struct LevelBlock` (7E08h bytes), `struct Object`, `struct Tile`, the SCD rows and the automap notes.

The tree has 73 struct and union definitions. Ten have bitfields, and 17 have pointer fields. The pointer fields grow from 2 or 4 bytes to 8 on the host. None of the 17 is a file record. Several are laid over shared buffers at fixed offsets, e.g. `struct CutsState` and `struct Stdat` in `stdat`. The port keeps them in memory only and never copies them as bytes to or from a file. The layout check lists every struct with a pointer field and fails if one is used with `fread`, `fwrite` or a size-based copy.

All file records are little-endian, and so are Apple Silicon and x86-64. The port reads them as packed structs and has no byte swapping.

### Names longer than 31 characters

Turbo C and TASM keep 32 characters of a name with its underscore, so the C name `CallbackFunctionSleepRelated_seg021_22FD_CB7` and the assembly label `_CallbackFunctionSleepRelated_seg` are the same symbol in DOS. On the host they differ. The C written for the port defines the full C name.

### Function prototypes and char parameters

Some functions are declared without a prototype, because their callers push an `int` where the definition takes a `char` ([CONTRIBUTING.md](CONTRIBUTING.md#where-files-disagreed)). Some function pointer tables also mix signatures, e.g. the spell table in SPELLS.C. Clang rejects both, and on the host an argument in the wrong type is undefined behaviour. The port needs one real type for each such function. The fix is a cast at the call or the table entry, or a typedef for the pointer type, each proved by the gate. Where no cast keeps the DOS bytes, the definition gets a port-only wrapper.

## Replacing the assembly modules

Every assembly module gets a C replacement in `src/port/`, written from the matched assembly. The matched assembly is the authority on behaviour. The modules fall into four groups:

| Group | Modules | Code bytes | Port |
| --- | --- | --- | --- |
| 3D renderer, seg004 | 14 modules in `src/3d` (EXPAND to PGCACHE), and SETPNT | 33,008 | C, from DOS and the FM Towns code (below) |
| Graphics library, seg003, and its neighbours | 14 modules in `src/gfx` (GRMISC to GRLIBN), SPRITE (seg000), VALLOC (seg001), MODEX (seg017) | 23,907 plus 2,915 | C, drawing into an emulated mode X screen |
| System layer, seg021 | 17 modules in `src/sys` (STARTUP to C3DENTRY), INT0TRAP (seg018) | 4,067 plus 95 | C on top of the platform API |
| Dropped | AIL (seg022), the overlay manager (seg046), LPFDELTA and PLANECPY (never called) | 3,321 and 4,670 | AIL's API is reimplemented in C (see Sound); the rest are not needed |

FARDATA.ASM's buffers become C arrays registered in the paragraph map. The far data that `tools/extract.py` still takes from the user's EXE (about 83 KB, mostly the renderer's tables and the 3D object models) is loaded from the user's own `UW2.EXE` at start-up, by the same offsets extract.py uses. That way the port never holds game data. The tables become C source only where their meaning is known and they hold no game content.

Each C replacement keeps the module's entry points and data names, so that the shared C calls it without a change. Where an assembly module switched stacks or data segments (seg021's private stack, seg003's own DS), the C version needs neither.

### The FM Towns build as a template

The FM Towns release of UW2 is a flat 32-bit build with 3,237 original symbol names ([BUILDING.md](BUILDING.md)). Its renderer and graphics library are hand-written assembly, not compiled C. Four observations show it:

- **Names.** Watcom C adds a trailing underscore to every C function, and 1,584 of the 2,050 code symbols have one. The other 466 have none, and they sit in runs where the assembly libraries are. Those runs are `render_3d`, `mxmul`, `do_movec`, `_asm_texture_map` and the rest of the renderer (198 names in one run), the graphics library (`show`, `cline`, `rectangle`, `fbuf_draw_ylrpp` and others), the keyboard handler, and AIL.
- **No frames.** The routines take their arguments in registers and have no stack frame. `do_movec` is four instructions, then `jmp dword [eax*2+vector_table]`, so the model interpreter dispatches each handler straight into the next one, as INTERP.ASM does in DOS.
- **Self-modifying code.** `_asm_texture_map_scanline_lin_tp` writes into its own code and does a far `jmp 000Ch:...` to flush the prefetch queue.
- **Thin C wrappers.** The C-callable entry points are thin Watcom wrappers around the assembly, e.g. `show_` moves its stack arguments into registers and calls `show`.

So the FM Towns code is not compiled C to lift, but it is a better template than the DOS assembly in three ways. First, it is flat 32-bit code, with no segment loads, no stack switches and no 64 KB limits, which is the memory model the port wants. Second, every routine and every data label has its original name, e.g. `fbufytab`, `_uleft`, `vector_table`, `dbase_data` and `gouraud_base_col`, where DOS has `seg003_0272_...` offsets. Third, its graphics library already separates the device: a table of 24 vectors (`_grSolidYLR`, `_grPageFlip`, `_grDoPalette`, `_grCopyFbToScreen`, `_grV4CopyYLRPP` and the rest) is the whole hardware edge, and everything above it draws through spans and bitmaps in memory.

The method for each renderer and graphics routine is to write the C from the DOS assembly, read the FM Towns routine beside it for names and for the 32-bit shape, and take DOS as the authority wherever they differ. Known differences include `do_fetchmap`'s `tmcolor`, the `mm3x9` saturation and `do_bcompact_map` ([3d.md](subsystems/3d.md#open-questions)). `tools/fmt.py` disassembles any FM Towns routine by name.

The FM Towns vector table is a good shape for the port's own graphics edge. Phase 1 keeps the DOS mode X behaviour behind it, and a phase 2 renderer can implement the same 24 primitives some other way.

### The screen

The DOS graphics library draws into VGA mode X with two pages, latched copies, a virtual screen wider than 320 for the cutscenes, pixel panning and a split line. CUTS.C also writes planar video memory itself, through `outportb(0x3C4, 2)` and `MK_FP(0xA000, 0)`. The port emulates the parts of the VGA that the game uses: 256 KB of video memory in four planes, the sequencer's map mask, the graphics controller's read map, the latches, the display start, pixel panning, the line compare and the DAC with 6-bit colour. `outportb` to the VGA ports goes to that emulation. Each frame the platform layer scans the visible page out to a 320 by 200 indexed image and converts it to RGB with the current DAC. The scan-out matches the hardware, so the port's screenshots can be compared with DOS byte for byte.

## The platform layer

The platform API is `platform/include/plat.h`, and it is the only code that knows about SDL3. Everything above it is portable C. The API covers these areas:

- **Video.** `plat_present(const uint8_t *indexed, int w, int h)` and `plat_set_palette(const uint8_t *rgb6, int first, int count)`. Window scaling, integer scaling and the 4:3 aspect correction from 200 to 240 lines are window options, so they never touch game code.
- **Keyboard.** The platform turns SDL key events into PC set-1 scan codes, with the E0 prefixes, and pushes them into a 64-byte ring buffer with the same semantics as KBDINT.ASM. The game's KEYQUEUE logic, ported to C, then reads the ring buffer unchanged. Held keys repeat as typematic make codes at the BIOS default rate, so the game's own filtering of repeats sees what it saw in DOS. The enhanced-keyboard flag the game reads from 0040:0096 is set as for a 101-key keyboard.
- **Mouse.** `int 33h` semantics: position in the driver's ranges, buttons, and relative motion in mickeys, which MOUSEDRV.ASM halves. Phase 2's mouselook reads the same motion.
- **Joystick.** Optional, through SDL gamepads, mapped onto JOYPORT's -127 to 127 axes with its dead zone.
- **Time.** A timer thread runs the AIL timers at their own rates: the game clock at 256 Hz increments `*Time`, the effects timer runs at 16 Hz, and the BIOS tick at 18.2 Hz. As in DOS, the clock advances while the game runs. Under replay the game reads the recorded values instead (below).
- **Files.** Every DOS path the game uses (`DATA\SKILLS.DAT`, `SAVE0\LEV.ARK`, `CRIT\CR01.00`) is mapped onto the user's data directory, with `\` as the separator and case-insensitive lookup. The data directory is read-only. Saves go to a separate directory, which plays the part of `UWHOME`.
- **Audio.** An output stream with a callback. The mixer runs on its own thread, so the game thread's hitches cause no underruns.

## Sound

SOUND.C stays shared and unchanged. The port replaces what is under it, which is AIL.ASM and the `.ADV` drivers that AIL loads.

The port implements the 37 AIL 2.0 functions the game calls (`AIL_startup`, `AIL_register_timer`, `AIL_register_sequence`, `AIL_start_digital_playback` and the rest) in C. John Miles released the AIL 2.14 source into the public domain, and the port translates it, including the XMIDI sequencer and the drivers' timbre handling, rather than inventing new behaviour.

Music is XMIDI (`UWAnn.XMI`, `UWRnn.XMI` for the MT-32), played through one of these backends:

- **OPL.** An emulated OPL2 or OPL3 chip (Nuked OPL3), driven by a C translation of AIL's AdLib driver with the game's own timbre bank (`UW.AD` or `UW.OPL`). This backend is the default, because it needs nothing the user does not have.
- **MT-32 and CM-32L.** libmt32emu with the user's own ROMs, fed the timbres from `UW.MT` as the MT-32 driver sends them.
- **General MIDI, phase 2.** A SoundFont synthesiser, for users with neither of the others.

The UnderworldGodot project already runs the same XMI files through mt32emu, a SoundFont synthesiser and ADLMIDI on Apple Silicon, with a dedicated producer thread. The port uses the same architecture in C.

Effects and speech are 8-bit `.VOC` samples on one digital channel. AIL's double-buffered playback is kept (two 2 KB buffers, refilled by `update_digi_playback`), so the game's EMS streaming and priorities work unchanged. The mixer resamples each buffer to the output rate. MIDI effects go through the music backend on the channels the game locks.

## The differential test: input record and replay

The test of faithfulness is to run the same session in DOS and in the port and compare the results. Doom's demo files and OpenRCT2's replays use the same method.

### What is recorded

Wall-clock input timing is not enough, because DOS UW2 reads its clock and its input devices at points that depend on how long the code takes. So the recording is made at the game's own boundary. It records each value the game reads from the outside world, in order:

- each read of `*Time`;
- each key event taken from the ring buffer;
- each mouse poll (position, buttons, motion);
- each joystick poll;
- the values of `time()` and the seeds given to `srand`.

Replayed in that order, the same values give the same `rand` sequence and the same decisions, whatever the speed of the machine.

### Where the hooks go

The hooks go in the shared sources, at the few places where the game reads the outside world. Reads of `*Time` become a `GAME_TIME()` macro, and input reads go through `get_input`, `key`, `mouse` and the joystick. Under the normal DOS build each macro expands to the original tokens, so the gate passes. A replay DOS build is the modding build compiled with `-DREPLAY`, which records or replays through a file. The port has the same hooks in its build. So the recording can be made in DOS (`tools/rungame.mjs` drives it in headless DOS through dos-mcp) and played in the port, or the other way round.

### What is compared

At chosen points, e.g. every N reads of the clock, at each level change and at each save, both builds call a shared `state_dump()` that writes the game state as bytes. The dump holds the player record, the level block (tiles, objects, free lists, animations, timers), the SCD state, the quest variables, the 3D frame buffer and the palette. The two dumps are compared byte for byte. The first difference names the subsystem and the moment where the port went wrong. Saves made in each build are compared byte for byte too, and are loaded in the other build. Screens are compared from the frame buffer dump and from the port's VGA scan-out, not from a scaled screenshot.

dos-mcp's `read_memory` and `search_memory` stay useful for questions the dump does not answer.

### Bugs from FINDINGS.md

The faithful port reproduces each likely bug, and the replay test proves it does. A fix becomes a named option, off by default, and the replay test runs with every option off.

## Phases and milestones

| Milestone | Work | Exit criteria |
| --- | --- | --- |
| 1. Measure (done) | `src/port/compat.h`, the stand-in headers, `make port-check` | every C source compiles or fails with a categorised reason; the unresolved names are listed; the gate passes |
| 2. Compile and link | gated source changes for the errors and the pointer, prototype and Borland issues below; explicit-width typedefs; the layout check; a stub platform | `make port-check` shows 0 errors and no pointer truncation in the shared C; the port links with stubs that abort; `tools/layoutcheck.py` shows every file record identical; the gate passes |
| 3. Replay hooks | `GAME_TIME()` and the input hooks in the shared sources; the replay DOS build; `state_dump()` | a DOS session recorded and replayed in DOS gives identical dumps twice in a row |
| 4. System and screen | the paragraph map, EMS, the platform layer on SDL3, seg021 in C, the VGA emulation, seg003 and the sprite, VALLOC and MODEX modules in C | the port boots, plays the title cutscene and shows the main menu; screens match DOS byte for byte at the menu |
| 5. 3D view | seg004 in C | the 3D frame buffer matches DOS byte for byte for a set of recorded positions, including pick frames |
| 6. Sound | the AIL API, the XMIDI sequencer, the OPL and MT-32 backends, the digital channel | every theme plays; effects, speech and the cutscene audio play; music timing matches a DOS recording |
| 7. Faithful release (phase 1) | the replay suite over whole sessions; save compatibility | a set of recorded sessions, from character creation into several worlds, replays in the port with identical dumps and saves; saves load both ways |
| 8. Enhancements (phase 2) | scaling, mouselook, the FINDINGS fixes as options, other platforms | each option is off by default, and the replay suite still passes with all options off |

## Milestone 1 baseline

The measurement is `make port-check`, which runs `tools/portcheck.py`. The tool compiles all 99 C sources with Apple clang 21 (`cc -x c -std=gnu89 -fsigned-char`) and the portability layer, with no game source edited. It writes the objects and the full log to `build/port/`. Each diagnostic is counted once, so a header's diagnostic is counted once, not once for each source that includes it. Run on 2 October 2026, the results are these.

92 of the 99 sources compile, and 45 of them have no diagnostic in the file itself. The 7 that fail have 18 errors between them, and there are 233 warnings over the whole tree:

| Category | Errors | Warnings | What it is |
| --- | --- | --- | --- |
| inline asm | 2 | 0 | EMS.C's `asm cld` and `asm mov bx,0` |
| far pointers and segments | 1 | 9 | lines with `MK_FP`, `FP_SEG`, `FP_OFF` or `movedata`, e.g. offset-only far pointer comparisons |
| near pointer held in an int | 0 | 33 | pointers cast to or from `int` or `unsigned` |
| 64-bit long and pointer differences | 0 | 67 | `long` arithmetic narrowed to `int`, and pointer differences, which are 64 bits on the host |
| Borland library internals | 3 | 0 | `grfp->fd`, a field of Borland's `FILE` (LOADGR.C) |
| missing declaration | 1 | 0 | `play_slot_hit_abs` called before its definition in BARTER.C |
| char parameters against old-style declarations | 11 | 0 | `f()` declarations whose definitions take `char`, and function pointers of mixed types (USEITEMS.C, OBJUSE.C, SPELLS.C, GAMEWRAP.C) |
| calls without prototypes | 0 | 92 | 51 of them in CONVERSE.C |
| pointer signedness and types | 0 | 14 | `char *` against `unsigned char *` |
| unsigned abs and always-true tests | 0 | 6 | `abs` of an unsigned expression, and an address tested against null |
| empty if bodies | 0 | 6 | kept for matching |
| return and control flow | 0 | 5 | non-void functions that fall off the end |
| comments | 0 | 1 | `/*` inside a comment |

The 7 sources that fail are these:

| File | Errors | Why |
| --- | --- | --- |
| `src/obj/USEITEMS.C` | 5 | prototyped handlers with `char` parameters passed as `void (*)()` |
| `src/gfx/LOADGR.C` | 4 | `FILE`'s `fd` field; `fileno(fp)` is Borland's macro for the same field and compiles to the same bytes |
| `src/game/GAMEWRAP.C` | 2 | `RestoreGame` and `SaveGame`, declared without prototypes and defined with `char` |
| `src/sys/EMS.C` | 2 | inline assembly; replaced by the port's EMS emulation |
| `src/combat/SPELLS.C` | 2 | the spell table holds functions of three signatures |
| `src/obj/OBJUSE.C` | 2 | handlers with `char` parameters passed as `void (*)()` |
| `src/conv/BARTER.C` | 1 | a call before the function's declaration |

These sources have the most warnings: CONVERSE.C (55), BABL.C (16), PANELS.C (13), SCDEVENT.C (11), GRDB.C (10) and MOTION.C (9).

The warnings most likely to break the port at run time are the 33 near pointers held in integers and the long arithmetic among the 67 narrowing warnings. The calls without prototypes are numerous but mechanical. Clang does not warn about 16-bit wrap or promotion at all, so the measurement cannot count them. The promotion audit in Milestone 2 counts them.

### What a link still needs

The objects reference 308 names that no compiled object defines and the host C library does not provide. Of those, 90 are defined in the 7 sources that do not compile yet (SPELLS.C 20, BARTER.C 25, GAMEWRAP.C 11, LOADGR.C 11, OBJUSE.C 10, USEITEMS.C 8, EMS.C 5), and they will resolve once those sources compile. The other 218 are the port's work. Names that only the failing sources use are not in the count yet.

| Where from | Names | Port work |
| --- | --- | --- |
| `src/gfx/GRCORE.ASM` | 36 | graphics library entry points and state: `show`, `rectangle`, `string_to_screen`, `setup_font`, `set_the_window`, `grPageFlip`, `palette`, `Ytab`, the window bounds |
| `src/sound/AIL.ASM` | 37 | the AIL API |
| `src/sys/SYSENTRY.ASM` | 22 | input and start-up: `key`, `mouse`, `mbuttons`, `Time`, `Shift`, `Ctrl`, `Alt`, `key_on`, `Asc`, `MouseDx`, `joy_position`, `cPerror` |
| `src/sys/C3DENTRY.ASM` | 19 | the 3D entry points and maths: `cRender`, `cInit3d`, `cFBtoScreen`, `cPlaceFB`, `cZoom`, `cSinCos`, `cFstSinCos`, `cAtan2`, `cSqRt`, `cFrmtoRaw`, `cPlayer`, `cDbase` |
| `src/3d/TMAPOPS.ASM` | 13 | the renderer's EMS page state and caches (`EmsBuff`, `tmap_fpage`, `crit_fpage`, `grs_3dinf`) |
| `src/gfx/MODEX.ASM` | 12 | the palette loader and the far string helpers (`str_len`, `str_copy`, `mem_set`) |
| `src/gfx/SPRITE.ASM` | 8 | sprites |
| `src/gfx/VALLOC.ASM` | 4 | video memory allocation, and saving and restoring rectangles |
| `src/sys/FARDATA.ASM`, `src/sys/INT0TRAP.ASM` | 3 and 3 | buffers; the divide trap |
| `src/gfx/SCALEBM.ASM`, `src/3d/PGCACHE.ASM`, `src/3d/SETPNT.ASM`, `src/lib/OVERLAY.ASM` | 3, 2, 1, 1 | `cXfer`, `grs_off`, `obj_tab`, `SetPnt`, `_OvrInitEms` |
| far data taken from the EXE | 10 | `smooth_div`, `smooth_base`, `smooth_lowpass`, `bmsegoff`, `bmhgtoff`, `_dblen`, model data, `ATM_Strings` (a name inside `stdat`), `cmpbuf2_start`, `sound_fpage` |
| DGROUP gaps placed by `tools/extract.py` | 7 | `Inventory`, `curelem`, the `STRINGS.PAK` state, `CutsceneOrConversationStringBlock` |
| Borland's library | 18 | `farmalloc`, `farfree`, `farcoreleft`, `coreleft`, `movedata`, `intdosx`, `getvect`, `setvect`, `outportb`, `filelength`, `tell`, `eof`, `getcurdir`, `itoa`, `ltoa`, `strupr`, `strnicmp`, `_ctype` |
| the portability layer | 19 | `port_mk_fp`, `port_fp_seg`, `port_fp_off`, the pseudo-register file, and the `bc_` file, time and random functions |

The C calls only the entry points, so the assembly modules' work is larger than these names. The whole of seg004 and seg003 has to be written, not only the routines C calls.

One name the game defines, `errmsg`, is also in the host C library. The game's definition wins in the link, but the port renames it to avoid confusion.

## The Milestone 2 plan

Milestone 2 makes the shared C compile and link on the host without errors, with no change to the DOS bytes. The steps, each a separate change that passes the gate, are these:

1. **Errors.** Write `fileno(grfp)` for `grfp->fd`. Give `RestoreGame`, `SaveGame` and the `void (*)()` handler parameters real types, with casts at the calls where the callers' bytes need the old form. Cast the mixed entries of the spell table to `SpellFn`. Declare `play_slot_hit_abs` before its first call. EMS.C moves to the port list, with its replacement in `src/port/mem/`.
2. **Near pointers in integers.** Introduce a `NEARPTR` integer typedef that is `int` under Turbo C and `intptr_t` on the host, for parameters and variables that carry pointers. Write the render database's label code (GRDB.C) in offsets from `cDbase`.
3. **Prototypes.** Add prototypes for the 92 calls without them, keeping the six per-file declarations that the gate needs.
4. **Explicit widths.** Add the `int16` family of typedefs to `uw2.h`, and rewrite each subsystem's declarations with a tool, one subsystem per gated change.
5. **Layout check.** Build `tools/layoutcheck.py`, which compiles one struct probe with Turbo C and with clang, runs both and compares the output. Make every file record identical, with `#pragma pack` and attributes in the port layer only.
6. **Promotion audit.** Build the clang AST checker, and fix or annotate every site it lists.
7. **Far pointer idioms.** Turn the `movedata` and `MK_FP` idioms into macros whose Turbo C expansion is the original tokens.
8. **Stub link.** Write `platform/stub/` and `src/port/` stubs that abort with the name of the function, so that the port links.

The exit criteria are in the milestone table above.

## Risks

- **Silent arithmetic differences.** 16-bit wrap and promotion differences produce no diagnostic and can change game logic far from their cause. The mitigation is the explicit widths, the promotion audit and the replay suite, which together catch them close to the cause.
- **Byte-identical changes may not exist.** Some portability changes may have no spelling that Turbo C compiles to the same bytes. Those changes go into port-only wrappers, which then need their own tests.
- **Bitfield placement.** The packed-struct hypothesis may fail for some field orders. The layout check finds every case before code depends on it.
- **Replay determinism in DOS.** The replay DOS build has to record every outside read, including the reads the renderer makes mid-frame through `do_mouseq`. A missed read shows up as two DOS replays that disagree, which Milestone 3 checks first.
- **The renderer's size.** seg004 is 33 KB of hand-written 386 code with self-modifying parts and a threaded interpreter, and seg003 is 24 KB more. The FM Towns names and the frame-buffer comparison keep the work checkable, but it is the largest single piece of the port.
- **Far data taken from the EXE.** About 83 KB of tables have no source yet. Loading them from the user's EXE keeps the port honest, but the offsets tie the port to the GOG `UW2.EXE`, which the port checks by its size and hash at start-up.
- **Asynchronous timers.** In DOS the clock interrupt can land between any two instructions. The port's timer thread gives the same effect, but data races that were harmless on one 386 can matter on a multi-core machine. Every value the timer callbacks share with the game is accessed atomically, and the replay suite runs with the clock recorded.

## Prior art

- **Chocolate Doom and Crispy Doom.** Chocolate Doom reproduces vanilla Doom's behaviour and limits, and uses demo compatibility as its test. Crispy Doom adds enhancements on top of the same code. This port follows the same split for its two phases.
- **The SM64 decompilation and its PC port.** One tree builds the matching N64 ROM and the PC port, with the platform code kept apart. That is the model for the two builds here.
- **Ship of Harkinian.** The Ocarina of Time PC port is built on a separate platform library, libultraship, which other ports have since reused. That is the reason this port keeps its platform layer independent of UW2 for a later UW1.
- **DevilutionX.** It began from Devilution, a Diablo decompilation that kept a build matching the original, and then moved to portable code step by step. It also records and replays demos for its regression tests.
- **OpenRCT2.** It began by replacing the original game's functions one at a time while the original still ran. It also has replays with game-state checksums. This port uses the same incremental replacement and the same kind of state comparison.
