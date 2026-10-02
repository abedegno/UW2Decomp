# Porting UW2 to modern systems

This page is the design for a native port of UW2 built from the matched sources. Milestones 1 to 3 are done: the C compiles on a modern host with no errors, `make port` links it into a native macOS binary, and the binary boots the real game data in an SDL3 window, runs `init_world` to the end and shows the two opening screens exactly as DOS does. Milestone 4 is done up to the 3D renderer: a session recorded in DOS replays in the port from boot through the title cutscene, the main menu and character creation into the game, with the same game state and the same video memory as DOS at every checkpoint, and the port stops at the first 3D frame. Their results are below, followed by the plan for what is left and for Milestone 5, and the phase 2 enhancements. What the later milestones describe does not exist yet.

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
| `src/include/portable.h` | the types and macros both builds share (explicit widths, `NEARPTR`, `OLDSTYLE`, `FAR_COPY`, `NULLTRAP`, `HOST_LAYOUT_BEGIN`); each is the original tokens under Turbo C | yes |
| `src/port/compat.h` | the portability layer, force-included into every port compile | no |
| `src/port/include/` | stand-ins for Borland's headers (`dos.h`, `alloc.h`, `mem.h`, `io.h`, `stat.h`, `dir.h`, `conio.h`, `bios.h`) | no |
| `src/port/port.h` | what the port's own C shares: the paragraph map, the far data blocks, the emulated hardware | no |
| `src/port/3d/`, `src/port/gfx/`, `src/port/sys/`, `src/port/sound/` | C written for the port to replace the assembly modules and the DOS-only C files, named after the module each replaces, and the emulated hardware (`gfx/vga.c`, `sys/pit.c`) | no |
| `src/port/mem/` | the paragraph map, the far heap, EMS, and the far data loaded from the user's EXE | no |
| `src/port/stubs/` | the link stubs, one file per source of the names (written by `tools/portstubs.py`); a replacement deletes its stubs | no |
| `src/replay/REPLAY.C` | the record and replay hooks and the state dump, shared by the replay DOS build (compiled with `-DREPLAY` and linked in as one more resident module) and the port; the gate never compiles it | only the replay build |
| `src/port/platform/plat.h` | the platform API, with no SDL in it | no |
| `src/port/platform/files.c`, `png.c` | the parts of the platform layer any POSIX host shares: the data root and DOS paths, and the PNG writer | no |
| `src/port/platform/sdl3/` | the SDL3 backend of the platform API, the only code that includes SDL | no |
| `tools/portcheck.py` | the compile measurement (`make port-check`) | no |
| `tools/portbuild.py` | the port build and link (`make port`) | no |
| `tools/portstubs.py` | writes `src/port/stubs/` from the names a link still needs and the port's C does not define | no |
| `tools/portshot.py` | the screen comparison with DOS (Milestone 3's exit test) | no |
| `tools/replay.py`, `tools/replaydos.mjs` | the replay DOS build, recording and replaying sessions in DOS and the port, and comparing the state dumps (Milestone 4) | no |
| `tools/widths.py` | rewrites integer declarations to the explicit widths | no |
| `tools/layoutcheck.py` | compares every struct's layout under Turbo C (in DOS) and the port | no |
| `tools/intaudit.py` | the promotion and overflow audit, on clang's AST | no |
| `build/port/` | port objects and logs, never committed | no |

The DOS tools never pick up port files, for three reasons:

- `tools/sources.py` lists every `.C` and `.ASM` under `src/` except `src/include` and `src/port`, so the gate, the link and the modding build never compile a port file.
- The other way round, a game source whose header comment says `port: dos-only` is never compiled by the port. Only EMS.C says it: its int 67h calls are replaced by the port's EMS emulation.
- `tools/tcc.mjs` copies only `src/include` into the DOS build directory, so the stand-in headers never shadow Turbo C's own.
- `tools/srcdeps.py` hashes only the headers in `src/include`, so editing a port header never recompiles a DOS object.

Port files use lower-case `.c` names, and each one starts with a comment naming the module it replaces.

## The portability layer

The portability layer is `src/port/compat.h` and the stand-in headers. The port compiles every game source with `-include src/port/compat.h -I src/port/include`, so the game sources need no `#ifdef` to get it. The DOS build never sees these files.

The layer does these things:

- It defines away Turbo C's memory-model keywords (`far`, `near`, `huge`, `interrupt`, `cdecl`, `pascal`).
- It turns `MK_FP`, `FP_SEG` and `FP_OFF` into calls into the paragraph map (below).
- It maps the pseudo-registers `_AX`, `_AH` and the rest onto a register file that the port's interrupt emulation reads and writes.
- It gives Borland's values to the `open` flags and permission bits. The sources also write them as numbers, e.g. `open(name, 0x8001)` in MISCUTIL.C and the mode `0x80` in ARC.C, and Borland's `O_RDONLY` is 1 where the host's is 0.
- It renames the Borland library calls whose host versions behave differently to `bc_` names, so that the port supplies Borland's behaviour. These are `open`, `read`, `write`, `close`, `lseek`, `creat`, `access`, `unlink`, `mkdir`, `chdir`, `stat`, `fstat`, `fopen`, `rand`, `srand`, `time` and `clock`. The reasons are DOS paths and case, Borland's text mode and flags, one-argument `mkdir`, Borland's `rand` (its own generator with `RAND_MAX` 7FFFh, called 272 times in the game), `clock` at 18.2 Hz, and the need for the replay harness to control `time` and `rand`.
- It declares Borland's extra library names (`itoa`, `ltoa`, `strupr`, `strnicmp`, `max`, `min`, `environ`) and Borland's `_ctype` table, which GAMESTRN.C reads directly.
- It renames the game's `main` to `uw2_main`, because the port's own `main` (`src/port/sys/main.c`) sets up the platform and runs the game's on a thread of its own, and `exit` to `bc_exit`, which runs the termination chain seg021 hooked and ends the program through the platform layer.
- It packs every struct the game declares (`#pragma pack(1)`), as Turbo C does with no `-a`, after it has included every host header the game uses, so the host's own structs keep their alignment. Port C that includes a game header must include `compat.h` first.

The game's translation units see only ISO C and POSIX 2008 names from the host (`-D_POSIX_C_SOURCE=200809L`). Without that, macOS's headers declare `valloc`, which collides with VALLOC.ASM's routine of the same name.

The shared sources also include `src/include/portable.h`, through `uw2.h`. It holds what both builds need: under Turbo C (`__TURBOC__`) every name in it is the original type or the original tokens, so the DOS bytes cannot change, and on the host it takes the host's meaning. It has the explicit-width types (`int16`, `uint16`, `int32`, `uint32`), `NEARPTR` and `UNEARPTR`, `OLDSTYLE`, `FAR_COPY`, `NULLTRAP` and `FARNULLTRAP`, `HOST_LAYOUT_BEGIN` and `HOST_LAYOUT_END`, and `AX_RESULT`, for a function the original wrote with no return statement whose callers use what it left in AX (an empty statement under Turbo C, `return` of that value on the host). Each is described below where it is used.

## Memory model and data layout

UW2 is a 16-bit medium-model program. Code is far, data is near unless a declaration says `far`, `int` is 16 bits and `long` is 32. On macOS on Apple Silicon, `int` is 32 bits, `long` is 64 bits and pointers are 64 bits. Each difference below is a place where the port can silently compute something DOS does not.

### Integer widths and wrap

Turbo C's `int` wraps at 16 bits, and the game relies on that. For example, headings are words with 10000h to the turn, so adding two headings wraps for free. Another example is `move_along` in UTIL.C, which multiplies a sine by a distance in an `int`.

The port handles this in two parts.

The first part is explicit widths. `portable.h` defines `int16`, `uint16`, `int32` and `uint32`. Under Turbo C they are `int`, `unsigned`, `long` and `unsigned long`, so the gate proves that a declaration written with them compiles to the same bytes. On the host they are `int16_t` and the rest, so a stored value wraps as it did in DOS and `long` is 32 bits again. `tools/widths.py` rewrote the declarations by this policy:

- `long` and `unsigned long` become `int32` and `uint32` everywhere, since the host's `long` differs from DOS's both in storage and in arithmetic.
- Every struct and union field, every file-scope variable (globals, statics and `extern` declarations) and every static local is written with an explicit width. Their storage is what files, saves and other modules see.
- The pointee of every pointer to an integer, and the element of every array of them, is explicit everywhere (locals, parameters, return types and casts), because it is DOS-layout data and it sets the stride.
- `sizeof` of an integer type is explicit, so `sizeof(int16)` is 2 as `sizeof(int)` was in DOS.
- A scalar local, parameter or return type stays plain `int` or `unsigned`. Any width of at least 16 bits holds its values. The exceptions are the locals and parameters whose address is passed as an `int16 *` (clang finds them), and the ones the promotion audit shows depend on the 16-bit wrap.
- `char` and its signed and unsigned forms never change.

The second part is the promotion audit. C promotes a 16-bit operand to the host's 32-bit `int` before arithmetic. A result that is stored back into 16 bits is truncated and comes out as in DOS. A result that is compared, divided, shifted right or converted to a wider type before it is stored can differ. A `uint16` is also promoted to a signed `int`, so `u - 1 > 0` is true in DOS for `u == 0` and false on the host. `tools/intaudit.py` walks clang's AST (`-Xclang -ast-dump=json`). It gives each expression the type Turbo C would give it, computes the range of its true value from the ranges of its leaves, and lists every expression whose value can leave its DOS type's range before it is truncated, with where the value goes. Each listed site either gets a cast that gives the DOS result, proved by the gate, or is recorded. The differential replay test (below) catches what the audit misses.

The alternative of defining `int` as `short` with a macro was rejected. It breaks `long int` and the host's headers, it does not stop promotion, and it hides the types from the debugger.

`char` is signed in Turbo C and on Apple Silicon, and the port passes `-fsigned-char` so that other hosts agree.

### Far pointers and segments

In the port every pointer is a flat 64-bit host pointer. The sources still build far pointers from segment and offset, take them apart, and do arithmetic on segment numbers. Examples:

- `MK_FP(EmsBuff + 0x800, off)` addresses page 2 of the EMS page frame (`EmsBuff` is a segment number, and 800h paragraphs is 32 KB);
- `MK_FP(FP_SEG(stdat) + 0x134, 0)` in CUTS.C, and `FP_SEG(drv_mem[n]) + 1` in SOUND.C;
- `movedata(FP_SEG(src), FP_OFF(src), FP_SEG(dst), FP_OFF(dst), n)`, 34 lines in 14 files;
- `(unsigned)list < FP_OFF(LastActiveMob)` in SCDEVENT.C, which compares far pointers by their offsets only, as Turbo C does for `<`.

The port keeps a paragraph map. The map is a table of regions, and each region is a block of host memory with a range of virtual paragraph numbers. The EMS page frame, the workspace, every `farmalloc` block and each far data array (`stdat` and the rest of FARDATA.ASM, the graphics and renderer data) is a region. `MK_FP(seg, off)` finds the region that holds linear address `seg * 16 + off` and returns the host address. `FP_SEG` and `FP_OFF` find the region that holds a host pointer and return its paragraph and the offset from it. So segment arithmetic inside a region works as in DOS. A pointer that is in no region, e.g. a near DGROUP array passed to `movedata`, gets a temporary region based at that pointer, so that `MK_FP(FP_SEG(p), FP_OFF(p) + n)` is `p + n`. Debug builds report each temporary region, so that the idiom can be replaced with a named macro later.

The common idioms are macros whose Turbo C expansion is the original tokens, so they compile to the same bytes. `FAR_COPY(dst, src, n)` in `portable.h` expands to `movedata(FP_SEG(src), FP_OFF(src), FP_SEG(dst), FP_OFF(dst), n)`, and 32 of the 34 `movedata` calls use it. On the host it is a forward byte copy with no paragraph map lookup. The other two stay as they are: GRFX.C adds to the offsets, and LIGHTING.C passes a segment of 0 for a match reason. An offset comparison is written `FP_OFF(p) < FP_OFF(q)`, which is Turbo C's own `((unsigned)(p))`. The paragraph map is then needed only for real segment numbers such as `EmsBuff`.

Offsets wrap at 64 KB inside a DOS segment, and a flat pointer does not wrap. No case of the game relying on the wrap is known. The replay test would show one.

`huge` pointers are not used anywhere.

### EMS

UW2 needs EMS 4.0. The port gives it an EMS emulation with the same calls EMS.C makes, a store of 16 KB logical pages and a 64 KB page frame that is a region of the paragraph map. Mapping a page copies the page that was in that frame slot back to the store and copies the new page in. The game keeps track of what is mapped (PGCACHE.ASM, PANELS.C, SOUND.C), so the frame must hold exactly what DOS would hold. Apple Silicon's memory pages are 16 KB, the same size as an EMS page, so the copies can later be replaced by `mmap` of the store into the frame with `MAP_FIXED`.

### Near pointers kept in integers

A near pointer is 16 bits in DOS, so the sources pass and store them as `int` or `unsigned`. On the host these are 64-bit pointers, and storing one in 16 or 32 bits truncates it. Milestone 1 found 33 of these, e.g. `gronk_critid(..., (int)&copy, ...)` in SCDEVENT.C, which passes a pointer through an `int` parameter, and `*cDbbase = (int)dbptr` in GRDB.C, which writes a pointer into the render database.

Milestone 2 removed all of them. A value that carries a near pointer is a `NEARPTR` (`int` under Turbo C, `intptr_t` on the host), or `UNEARPTR` where two near pointers are compared as Turbo C compares them, unsigned. The channels that carry one are retyped: the argument of the `gronk_` family and `WhoamiFn`, the argument of input handlers (`InputFn` and the dispatch tables' `arg` field), and `seg009_2CC`'s two output pointers. Handlers whose own parameter is a plain `int` are cast where they are registered, as handlers of other types already were. Where the sources store a far pointer's offset in an `int` (the render database's `Clk` and `gr_entry`, `cPerror`, `lget`), they now say `FP_OFF`, which is Turbo C's own `((unsigned)(p))`, so the render database holds offsets as the paragraph map gives them and never host pointers.

### Struct layout and bitfields

Turbo C lays structs out with byte alignment and puts bitfields in 16-bit units. [MATCHING.md](MATCHING.md) records how Borland places each bitfield: in the 16-bit window that starts at the byte holding the next free bit, so a field can straddle a word boundary. The bits pack as one stream, low bit first, and a struct takes only the bytes it needs.

On the host the port packs every struct (`#pragma pack(1)` in `compat.h`), and with 16-bit bitfield types clang then also packs bitfields as one stream, low bit first. `tools/layoutcheck.py` tests this. For every struct and union in `src/include` it generates one C program that sets each field in turn to all ones in a zeroed record (a bitfield is assigned -1) and prints which bytes changed, with their values, and the record's size. It compiles the program with Turbo C in DOS and with clang, runs both and compares the outputs line by line. All 38 records without pointer fields come out identical, including the ten with bitfields, so the packed-struct hypothesis holds. The 21 file records the tool knows (`struct Player` (`PlayerDat`, 37Eh bytes), `struct LevelBlock` (7E08h bytes), `struct Object`, `struct StaticObj`, `struct Tile`, the animation and automap note records, the SCD rows, clocks and work area, the critter, weapon, armour, container and light tables from OBJECTS.DAT, `struct ComObj`, the font header and the bitmap header) are among them. Without the packing, `struct Player` is 388h bytes and `struct LevelBlock` 8108h on the host.

A struct with pointer fields cannot keep its DOS layout, since a host pointer is 8 bytes. Eight in the headers and eleven in the sources have them, and none is a file record. They are bracketed by `HOST_LAYOUT_BEGIN` and `HOST_LAYOUT_END` (`portable.h`, nothing under Turbo C), which give them natural alignment in the port: packed, their pointers would sit at odd offsets, and the macOS linker refuses a pointer in initialised data that is not aligned (SPELLS.C's `area_spells`). The layout check lists every struct in the headers with a pointer field and fails if a source copies one as bytes to or from a file or buffer; none does. Some are laid over shared buffers and grow on the host, which the port has to allow for (see the Milestone 2 results).

All file records are little-endian, and so are Apple Silicon and x86-64. The port reads them as packed structs and has no byte swapping.

### Names longer than 31 characters

Turbo C and TASM keep 32 characters of a name with its underscore, so the C name `CallbackFunctionSleepRelated_seg021_22FD_CB7` and the assembly label `_CallbackFunctionSleepRelated_seg` are the same symbol in DOS. On the host they differ. The C written for the port defines the full C name.

### Function prototypes and char parameters

Some functions were declared without a prototype, because their callers push an `int` where the definition takes a `char`, or pass an argument the definition does not take ([CONTRIBUTING.md](CONTRIBUTING.md#where-files-disagreed)). Some function pointer tables also mix signatures, e.g. the spell table in SPELLS.C. Clang rejects both, and on the host an argument of the wrong type is undefined behaviour. Milestone 2 gave each one real type, every change proved by the gate:

- `OLDSTYLE((params))` (`portable.h`) is `()` under Turbo C and the parameter list on the host, so the DOS callers keep their pushes and the port's callers convert as the DOS callee reads. `RestoreGame`, `SaveGame` and `create_sprite` (variadic on the host, since it is called with one argument and with three) use it.
- A function whose callers pass an argument it ignores now takes it, as an unused `int`: `editexit`, `clearobj`, `player_look_grave`, `seg011_6` and `seg011_2C6`. An unused parameter costs Turbo C no byte.
- Function pointers of mixed types are cast where they are stored or passed (the spell tables, `UseThing`'s handlers, `bab_fun`'s built-ins through a `BablFn` typedef, PANELS.C's `adjust` table, the collision handlers' `special`), as handlers of other types already were.
- `postodir`'s old-style definition with `char` parameters takes `int`s and casts them to `char`, which compiles the same.

No change needed a port-only wrapper. Calling a handler through a pointer of another type is still undefined behaviour on the host; it works on arm64 and x86-64 because both pass the arguments in registers and the callee reads only the bits its type has. The remaining cases are listed in the Milestone 2 results.

### Null pointers

A null pointer does not crash a DOS program, and where the game dereferences one the port has to give the same result.

**What DOS reads.** UW2 is a medium-model program, so a near pointer is an offset in DGROUP, and a near null pointer reads DS:0. In `UW2.EXE` (the GOG release) DGROUP's paragraph starts 4 bytes before C0's `_DATA`, so DS:0 to DS:2 are the last three bytes of the segment before it, the overlay stubs. They are the tail of the last stub entry of ovr167, the one for `FarWrite_ovr167_627` (MISCUTIL.C). While ovr167 is not loaded the entry is `CD 3F 27 06 00` (`int 3Fh`, the function's offset 0627h, 0), so DS:0 holds 27h 06h 00h. When the overlay manager loads ovr167 it rewrites each of its entries as a far jump, `EA 27 06 ss ss`, so DS:0 holds 06h and DS:1 and DS:2 hold the overlay's load segment, and it writes the `int 3Fh` form back when it unloads it (OVERLAY.ASM, `_67F` and `_69D`). What a near null read sees therefore depends on whether ovr167 is in the overlay buffer at that moment.

| DS: | Bytes | What they are |
| --- | --- | --- |
| 0-2 | 27 06 00, or 06 ss ss while ovr167 is loaded | the tail of ovr167's last stub entry |
| 3 | 00 | padding before `_DATA` |
| 4-7 | 00 00 00 00 | C0's null area |
| 8-30h | `Turbo C++ - Copyright 1990 Borland Intl.` and its 0 | C0's copyright |
| 31h on | `Null pointer assignment`, CR LF, then `Divide error` and `Abnormal program termination` | C0's messages |

So a string read through a near null pointer is `'` followed by byte 06h, or byte 06h followed by the segment's bytes up to a 0. A far null pointer is 0000:0000, the interrupt vector table. Its first entry, the one a null record pointer reads first, is int 0, which `init_world` points at `int0_trap` (INT0TRAP.ASM) at start-up: offset 0019h, which the link fixes, and segment 1FC7h plus the program's load segment, which depends on how much memory DOS and its drivers take below the game.

**What DOS checks.** UW2's C0 keeps Turbo C's null pointer check (tools/link.py's `c0_source` rebuilds it). The game leaves through `exit`, from `main`'s return or from `exit` calls in MAINMENU.C, ERROR.C and GRDB.C, and C0's `__exit` then sums the 2Dh bytes from DS:4 to DS:30h and compares the sum with 0CA5h. The code is at file offset 11460h: `mov ax,4; mov si,ax; xor ax,ax; mov cx,2Dh`, the add loop, then `sub ax,0CA5h`. If the sum differs it prints "Null pointer assignment". This is one of UW2's three changes to Turbo C's C0: the stock C0 sums from DS:0, and UW2's starts at `DATASEG@`, DS:4. So a write through a near null pointer is reported only if it lands in DS:4 to DS:30h. A write to DS:0 to DS:2 goes unreported and corrupts the stub entry, so a later call to `FarWrite_ovr167_627` goes astray. A far null write changes the interrupt vectors, and nothing checks them.

**What the host does.** On macOS every 64-bit process has a `__PAGEZERO` segment with no access over the low 4 GB, and an arm64 process cannot map memory at address 0, so any null dereference crashes. The port cannot reproduce DOS by mapping page 0. It finds each site instead, and decides each one.

**Finding them.** Writes and reads are found differently.

- Writes: a recorded session (Milestone 4's replay) is replayed in real DOS through dos-mcp (`tools/rungame.mjs`), the game is quit through its menu, and the output is checked for "Null pointer assignment". That covers DS:4 to DS:30h. DS:0 to DS:3 and the vector table are covered by reading them with dos-mcp's `read_memory` before and after each session.
- Reads, in three ways. clang's static analyzer runs over the port build (done in Milestone 2; the results are below). The `NULLTRAP` hook (`portable.h`) marks each audited dereference: a modding build of the sources concerned compiled with `-DNULLTRAP` (`tools/tcc.mjs`, then `tools/link.py --obj STEM=PATH`) appends the file and line of each null it sees to `NULLTRAP.LOG`, and goes on with the null pointer, as DOS does, so a replayed session in DOS lists the marked sites it reaches. And once the port runs, its debug build is compiled with UBSan's `-fsanitize=null` and run over the replay corpus, which finds every null dereference the sessions reach, marked or not.

**Deciding each site.** Each site gets one of three treatments:

1. The port serves the bytes DOS would read. `NULLTRAP(p)` and `FARNULLTRAP(p)` are `(p)` under Turbo C, so the gate proves they change nothing. On the host a null `p` becomes a pointer to the port's copy of DGROUP from DS:0 (`port_null_near`), or of the vector table (`port_null_far`), and the hit is logged. The copy of DS:0 to DS:2 follows the overlay state where the port models it, and otherwise holds the unloaded form; the vector table holds int 0 with the segment of the DOS set-up the replays are recorded on. Writes land in the copy, so they change nothing else, and they are logged.
2. A plain guard, where nothing depends on the bytes read.
3. A real bug in the original: it is recorded in [FINDINGS.md](FINDINGS.md) with its fix as a phase 2 option, and the default reproduces DOS.

The faithful port never crashes on a null pointer: a site the audits missed is a bug in the port.

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

The platform API is `src/port/platform/plat.h`. It names no SDL type, and `src/port/platform/sdl3/plat_sdl3.c` is the only file that includes SDL. Everything above it is portable C. SDL3 itself runs on macOS, Windows, Linux, iOS and iPadOS, Android and Emscripten; because the layer stays SDL-free, a web build without SDL or a libretro core is one more backend file, not a change to the port.

The model is the PC's. The game runs on a thread of its own, as the CPU did. The backend owns the window and the event loop on the host's main thread (which macOS and iOS require), and calls into the port the way the PC's hardware interrupted the CPU. Its hooks run on the backend's thread and do only what an interrupt handler could. The port's own timer thread plays the PIT.

- **Video.** The game never presents anything. Each pass of the event loop the backend calls the `scanout` hook, which reads the emulated VGA as its CRT controller would (the visible page, the offset, the panning and the line compare) and returns an indexed picture with the DAC's 256 6-bit entries. The backend converts it to RGB, `(v << 2) | (v >> 4)` as DOSBox does, and draws it scaled. The scaling is the backend's: integer multiples by default, and the 4:3 aspect correction from 200 to 240 lines, so it never touches game code.
- **Pointer events, not "mouse".** A pointer is a mouse, a pen or a finger. Each event carries its position in the game's 320 by 200 screen pixels, already undone from the window's scaling and letterbox, its motion, and its buttons. The port's mouse driver (`src/port/sys/mousedrv.c`) builds int 33h's state from them: the position in the driver's 640 by 200 range, the buttons, and the motion in mickeys at the driver's default rate. The game reads only the motion and the buttons and keeps its own cursor, so a touch screen drives it the same way.
- **Keyboard.** The backend turns each SDL key event into PC set-1 scan code bytes, make or break, with the E0 and E1 prefixes, through one table, and hands them over one byte at a time, as port 60h did. The port's int 9 handler (`src/port/sys/keyqueue.c`) stores them in KBDINT.ASM's 64-byte ring buffer at the same offsets of seg021's data, and `key`, KEYQUEUE.ASM in C, reads them out unchanged. The backend drops the host's own key repeat; the port's keyboard repeats a held key itself at the BIOS default (500 ms, then 10.9 a second), so the game's filter of repeats sees what it saw in DOS. The enhanced keyboard flag is set, as for a 101-key keyboard.
- **Time.** `plat_counter` and `plat_counter_hz` are the host's high-resolution counter, and `plat_sleep_ns` a precise sleep. The PIT (`src/port/sys/pit.c`) runs on a thread of its own from them: the AIL timers at their own rates (the game clock, SOUND.C's `cllbck_tst`, at 256 Hz), the BIOS tick at 18.2 Hz for `clock`, and the keyboard's repeat. A timer is due whenever its period has passed, so its rate is exact on average, and a late wake-up runs the missed calls at once, as queued interrupts did. Under replay (Milestone 4) the game reads the recorded values instead.
- **Files: the data root.** The user's own copy of UW2 is the data root, and it is never written. The port's home directory (`--home`, default `~/.uw2port`) plays the part of `UWHOME`: it is laid over the data root, so the game sees one tree. Every DOS path the game names (`DATA\uw.cfg`, `data`, `SAVE0\LEV.ARK`, `CRIT\CR01.00`, the scratch files `a.tmp` to `h.tmp`) is looked up one component at a time, without case, first in the home directory and then in the data root (`src/port/platform/files.c`). A file the game creates goes to the home directory. A file it opens to change stays in the data root until its first write, and is copied into the home directory then. Borland's text mode (CR LF, Ctrl-Z) is kept for handles opened without `O_BINARY`. The far data no source defines yet, and seg021's and seg003's data segments, are read from the user's `UW2.EXE` in the data root, which the port checks by its size and CRC-32 first.
- **Lifecycle events.** The backend reports suspend (a phone going to the background, a window minimised), resume, and a request to quit (the window's close box). A phone build has to stop the timers and save on suspend; the desktop build only logs them.
- **Audio.** An output stream of 16-bit stereo samples with a callback on the audio thread (`plat_audio_open`). Milestone 3 opens it and feeds silence. Sound is Milestone 7.
- **No JIT and no self-modifying code** anywhere in the port. iOS refuses writable executable memory. The DOS code that patched itself (seg004's texture mappers, seg003's polygon clipper, SCALEBM.ASM's generated sprite code) becomes C that chooses with data what the patch chose.
- **Debugging.** `--screenshot-after MS` writes the scan-out to a PNG, `--window-shot FILE` the window's own scaled contents, `--shot-at-flip K:FILE` the screen right after the game's K-th page flip, `--hidden` runs with no window, `--exit-after MS` quits, `--exit-on-halt` quits where the port stops instead of leaving the window up, and `-v` traces file opens, the paragraph map's windows and the call stack at a stub.

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

The hooks go in the shared sources, at the few places where the game reads the outside world (`portable.h`): every read of `*Time` is `GAME_TIME()`, and `key`, `mouse`, `mbuttons`, the joystick reads, `time()` and `srand` are `KEY()`, `MOUSE()`, `MBUTTONS()`, `JOY_READ()`, `JOY_BUTTONS()`, `WALL_TIME()` and `SRAND()`. Under the normal DOS build each macro expands to the original tokens, so the gate passes. A replay DOS build is the modding build with those sources compiled with `-DREPLAY` and `src/replay/REPLAY.C` linked in, which records or replays through a file. The port has the same hooks in its build. So the recording can be made in DOS (`tools/replaydos.mjs` drives it in headless DOS through dos-mcp) and played in the port, or the other way round. The Milestone 4 results have the formats.

### What is compared

At chosen points, e.g. every N reads of the clock, at each level change and at each save, both builds call a shared `state_dump()` that writes the game state as bytes. The dump holds the player record, the level block (tiles, objects, free lists, animations, timers), the SCD state, the quest variables, the 3D frame buffer and the palette. The two dumps are compared byte for byte. The first difference names the subsystem and the moment where the port went wrong. Saves made in each build are compared byte for byte too, and are loaded in the other build. Screens are compared from the frame buffer dump and from the port's VGA scan-out, not from a scaled screenshot.

dos-mcp's `read_memory` and `search_memory` stay useful for questions the dump does not answer.

### Bugs from FINDINGS.md

The faithful port reproduces each likely bug, and the replay test proves it does. A fix becomes a named option, off by default, and the replay test runs with every option off.

## Phases and milestones

| Milestone | Work | Exit criteria |
| --- | --- | --- |
| 1. Measure (done) | `src/port/compat.h`, the stand-in headers, `make port-check` | every C source compiles or fails with a categorised reason; the unresolved names are listed; the gate passes |
| 2. Compile and link (done) | gated source changes for the errors and the pointer, prototype and Borland issues; explicit widths; the layout check; the promotion audit; the null-pointer static pass; the far pointer macros; link stubs; `make port` | `make port-check` shows 0 errors and no pointer truncation in the shared C; `make port` links a native binary with stubs that abort; `tools/layoutcheck.py` shows every file record identical; the gate passes |
| 3. Boot to the title (done) | the platform layer on SDL3 (video, pointer events, keyboard, time, files, lifecycle, silent audio); the paragraph map, the far heap and EMS; Borland's library; seg021 in C; the parts of seg003, MODEX, VALLOC and the VGA emulation the opening screens need; AIL with no driver; `port_null_near` and `port_null_far` | the port runs `init_world` past `display_screen(5, 6)` and shows the first title screen, and its VGA scan-out matches a DOS screenshot of that frame byte for byte |
| 4. Replay, and the way into the game (done up to the 3D renderer) | `GAME_TIME()` and the input hooks in the shared sources; the replay DOS build; `state_dump()`; the null-pointer write check; the cutscene player's screen access, the rest of seg003, the sprites and the mouse cursor, text; a `-fsanitize=null` debug build | a DOS session recorded and replayed in DOS gives identical dumps twice in a row; the same session from boot through the title cutscene, the main menu and character creation to the first game screen replays in the port with identical dumps at each checkpoint, and the title, menu and character creation screens match DOS byte for byte |
| 5. The game screen | the panels, the inventory, the message scroll, the automap screen; everything but the 3D view | a recorded session in the first level replays in the port with identical dumps, and every screen outside the 3D view matches DOS byte for byte |
| 6. 3D view | seg004 in C | the 3D frame buffer matches DOS byte for byte for a set of recorded positions, including pick frames |
| 7. Sound | the AIL API, the XMIDI sequencer, the OPL and MT-32 backends, the digital channel | every theme plays; effects, speech and the cutscene audio play; music timing matches a DOS recording |
| 8. Faithful release (phase 1) | the replay suite over whole sessions; save compatibility; every null-pointer site decided | a set of recorded sessions, from character creation into several worlds, replays in the port with identical dumps and saves, with no null dereference reported by `-fsanitize=null`; saves load both ways |
| 9. Enhancements (phase 2) | scaling, mouselook, the FINDINGS fixes as options, other platforms | each option is off by default, and the replay suite still passes with all options off |

## Milestone 1 baseline

These are Milestone 1's numbers, measured before any source was changed; Milestone 2's follow. The measurement is `make port-check`, which runs `tools/portcheck.py`. The tool compiles all 99 C sources with Apple clang 21 (`cc -x c -std=gnu89 -fsigned-char`) and the portability layer, with no game source edited. It writes the objects and the full log to `build/port/`. Each diagnostic is counted once, so a header's diagnostic is counted once, not once for each source that includes it. Run on 2 October 2026, the results are these.

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

## Milestone 2 results

Milestone 2 made the shared C compile and link on the host, with no change to the DOS bytes. It took eight steps. Each ended with `make check` passing and `make port-check` run again, and the whole was proved at the end with `make check-all`. The gate rejected none of the changes. Run on 2 October 2026, with Apple clang 21, the counts are these:

| After step | Sources that compile | Errors | Warnings | What changed |
| --- | --- | --- | --- | --- |
| Milestone 1 | 92 of 99 | 18 | 233 | |
| 1. Errors | 98 of 98 | 0 | 225 | EMS.C left the port's list |
| 2. Near pointers in integers | 98 | 0 | 195 | 30 near pointers in integers to 0 |
| 3. Prototypes | 98 | 0 | 108 | 87 calls without prototypes to 0 |
| 4. Explicit widths | 98 | 0 | 68 | 64-bit narrowing from 67 to 33 |
| 5 to 8 | 98 | 0 | 61 | far pointer warnings from 9 to 0 |

The 61 warnings left are these: 33 narrowings of the host library's 64-bit results (`strlen`, `tell`, `filelength`, `ftell`, `st_size`) and of pointer differences, none of them a pointer; 14 `char *` against `unsigned char *`; 6 empty `if` bodies and 5 functions that fall off their end, both kept for matching; one `abs` of an unsigned value, one `/*` in a comment and one `printf` format in a call Turbo C drops (`if (0) printf(...)`).

### 1. Errors

- LOADGR.C reads `fileno(grfp)`, which is Borland's macro for `grfp->fd` and compiles the same.
- `RestoreGame` and `SaveGame` are declared with `OLDSTYLE` (above).
- The spell tables cast their two odd entries to `SpellFn`; OBJUSE.C and USEITEMS.C cast the eight handlers they pass to `UseThing`.
- `play_slot_hit_abs` is declared in `conv.h`.
- EMS.C is marked `port: dos-only`. Its pseudo-registers and inline assembly talk to the EMS driver, which the port replaces as a whole (`src/port/mem/`), so the port never compiles it, and `tools/portcheck.py` lists its names as the port's to provide.

### 2. Near pointers in integers

All 30 left after step 1 are gone (EMS.C had the other 3), as described under "Near pointers kept in integers". The input handlers and the `gronk_` callbacks whose own parameter stays an `int` are cast where they are registered: 62 input handlers and 4 critter callbacks.

### 3. Prototypes

All 87 calls without a prototype that were left after step 1 have one, as described under "Function prototypes and char parameters". The 51 built-ins CONVERSE.C binds with `bab_fun` are cast to `BablFn`. The six declarations CONTRIBUTING.md lists as kept in their files keep their `char` parameters; they already had prototypes, so the port compiles them unchanged.

### 4. Explicit widths

`tools/widths.py` rewrote 1,210 type specifiers by the policy above: 302 in the headers and 908 in the sources. clang then found 60 or so locals and parameters whose address is passed where an `int16 *` or `uint16 *` is now expected, and those were changed by hand to the explicit width, which is their DOS width. No narrowing of `long` to `int` is left.

### 5. Layout check

`tools/layoutcheck.py` (results above): 46 records in the headers, 38 identical and 8 different, all 8 with pointer fields. All 21 file records are identical, including every record with bitfields, so no file record needs accessor macros in place of its bitfields.

### 6. Promotion audit

`tools/intaudit.py` listed 1,894 sites on its first run. Fifteen sites in ten files are fixed, each with a cast or an explicit width that gives the DOS result and keeps the DOS bytes:

- the heading arithmetic that relies on the 16-bit wrap: DRAWOBJ.C's critter facing (`cPlayer->heading + headmod[quad]`, which without the wrap could index `dirtab` out of range), PATHFIND.C's `theta` and `vector`, PHYSICS.C's `h` in two functions with `abs((int16)(PlayerFacing - h))` and `(uint16)(PlayerFacing - h)`, and MOTION.C's `diff`;
- unsigned 16-bit results that DOS compares or shifts as unsigned and that can go below 0: a light's quality minus 1 in PLAYTIME.C (at quality 0 DOS takes the large unsigned value), a tile's height minus 1 in WORLDEV.C and in three slope tests in VIEW3D.C (at height 0), CUTS.C's frame and page counts minus 1 in three places, and TMPALLOC.C's EMS page count;
- GRDB.C's `loc[lab] == (uint16)-1`, where `loc` became `uint16` in step 4 and the host would compare 65535 with -1.

The final run lists 1,867 sites, by where the value goes and why it can differ:

| Category | Overflow | Signedness |
| --- | --- | --- |
| compare | 212 | 49 |
| divide | 222 | 8 |
| shift-right | 30 | 2 |
| condition | 8 | 1 |
| widen | 45 | 3 |
| index | 206 | 13 |
| int-store | 449 | 95 |
| int-arg | 511 | 35 |
| return | 19 | 0 |
| other | 23 | 0 |

Most of them cannot happen with the game's values: the tool assumes any `int` can hold any 16-bit value, so `skill + 0x1F` counts. The list (`build/port/intaudit.txt`) is the place to look when a replay disagrees. Two groups need a decision before the replay suite: the dot products in GRIDDB.C's face tests (lines 555 and 607), which DOS computes in 16 bits and tests for `< 0`, and the input turn rate in PHYSICS.C (`rate * PlayerTurn * (TurnInpRate / 4) / 4`). Both would overflow 16 bits only with values the game probably never has; casting the sum to `int16` before the test is the faithful fix if the replay shows otherwise.

### 7. Null pointers

clang's static analyzer (`clang --analyze`, all default checkers) over the port build reports 121 warnings. Eight concern null pointers: seven null dereferences and one null argument to `strcpy`. Three of them are real in the original, and reading the code found one more:

- COMBAT.C, melee damage: when a blow destroys a door, `def` is set to 0 and `OBJ_Z(def)` then reads the vector table at 0000:0002, the segment of `int0_trap`. The effect's height therefore depends on where DOS loaded the game. A far null read, marked `FARNULLTRAP`. ([FINDINGS.md](FINDINGS.md#smashing-a-door-reads-the-interrupt-vector-table))
- SCROLLIO.C, `wdialog`: Escape copies `initial` into the result, and CONVERSE.C passes 0 for a conversation's typed answer, so the answer becomes the string at DS:0. A near null read, marked `NULLTRAP`. ([FINDINGS.md](FINDINGS.md#escape-at-a-conversations-typed-answer-reads-ds0))
- BABL.C, `convert_string`: `bab_malloc`'s result is not checked, so a full conversation heap makes the substituted text overwrite the vector table, and `bab_realloc` of the null result reads below it. A far null write, marked `FARNULLTRAP`.
- LOADGR.C, `get_pals`: `PalStore` is never set, so `*PalStore` reads and writes DS:0, the overlay stub; but only `.CR` files reach it, and nothing loads one. Marked `NULLTRAP`.

The other four are false: GAMESORT.C's `do_objsort(0L)` reaches `link` only through `Obj_PtrTMem`, which returns 0 for a null link; and INPUT.C goes on past a failed `realloc` only into `pfatal_code`, which does not return. A known case the analyzer cannot see, since it crosses files, is CRITTIME.C's lava rescue, where `Map_GetAddr` returns 0 (already in FINDINGS.md). `-DNULLTRAP` compiles under Turbo C (checked with SCROLLIO.C and COMBAT.C).

### 8. Far pointers and the link

`FAR_COPY` replaced 32 of the 34 `movedata` calls, and SCDEVENT.C's two offset comparisons use `FP_OFF`. `tools/portstubs.py` wrote 19 stub files in `src/port/stubs`, with 160 functions and 80 variables, from the 240 names the link needs:

| Where from | Names |
| --- | --- |
| assembly modules | 171: GRCORE 39, AIL 37, SYSENTRY 22, C3DENTRY 19, TMAPOPS 14, MODEX 14, SPRITE 8, VALLOC 5, SCALEBM 3, FARDATA 3, INT0TRAP 3, PGCACHE 2, SETPNT 1, OVERLAY 1 |
| Borland's library | 21 |
| the port layer (`port_`, `bc_`) | 24 |
| far data taken from the EXE, and DGROUP gaps | 19 |
| EMS.C, DOS-only | 5 |

`make port` (`tools/portbuild.py`) compiles the 98 game sources and the stubs and links `build/port/uw2port`, a 788 KB arm64 Mach-O executable. Run, it stops at the first stub it reaches, `getvect` in `init_world`.

### Found on the way, for later milestones

- Structs with pointers laid over shared buffers grow on the host: CUTS.C's `struct Stdat` and `struct CutsState` over `stdat`, and BABL.C's heap blocks (`struct BabBlock`), whose header size the allocator writes as the numbers 8 and 12. The port's `stdat` must be large enough, and the conversation heap needs its header size from `sizeof`.
- `ATM_Strings` is a name inside `stdat` (offset 468Eh). Its stub is a separate array; the real one must sit inside `stdat`, since the notes and the LZSS work area overlap there (FINDINGS.md).
- Handlers called through a pointer of another type: `sp_true_sight` in the area spell table takes `(caster, target)` but is called with `(x, y, target, tile, type)`, and the critter collision handlers take a `struct Phys *` but are given the address of an `unsigned` collision state. Both work in DOS by layout; the port must keep them working the same way.
- The assembly replacements should take explicitly sized parameters: the C passes them `int` values computed without the 16-bit wrap, e.g. `cSinCos(cPlayer->heading + 0x2040, ...)`.

## Milestone 3 results

Milestone 3 boots the port to the title. Run on 2 October 2026 on an Apple Silicon Mac, with Apple clang 21 and SDL 3.4.16, `make port` links `build/port/uw2port` (about 1 MB, arm64), and `build/port/uw2port --data ~/UWGOG/UW2` boots the GOG release's data in an SDL3 window in about a second, as far as this:

1. `init_world` runs to its end. Every step of it works on the user's data: the divide trap; EMS (the emulated driver has 512 pages, and `init_mem` asks for 89 to 103 of them by `rand`); the data directories and the eight scratch files; STRINGS.PAK; the map; input; UW.CFG; sound with no card (UW.CFG says `0 -1 -1 -1 sound`) and the 256 Hz clock; the graphics library; the Origin screen, `display_screen(5, 6)`; the music (none, with no card); the video memory pool; all the art (ALLPALS.DAT and 20 .GR files, ten into EMS and ten into video memory); the Looking Glass screen, `display_screen(6, 7)`; the mouse, the objects, the 3D view's set-up, the player, the AI, lighting, the save directory, the working copies of LEV.ARK and SCD.ARK (written to the home directory's SAVE0), the conversation globals (BGLOBALS.DAT, also in SAVE0), and the game palette.
2. `titlescr` starts cutscene 9. `show_cutscene`, `show_anm` and `cuts_process_anm` run, and `cuts_process_lp` calls `copy_visible_to_hidden`, GRCORE.ASM's first entry with no C yet. The port stops there and leaves the window up (`--exit-on-halt` quits instead).

**The screens match DOS byte for byte.** `tools/portshot.py` builds, for each opening screen, a DOS EXE that stops on it (UWEDIT.C with `for (;;) ;` after the `display_screen` call, compiled with Turbo C and linked as the modding build), boots it in headless DOS and screenshots it, and compares that with the port's scan-out right after the same page flip (`--shot-at-flip`). js-dos doubles mode X to 640 by 400, so every second pixel is compared. Both convert the DAC's 6-bit colour the same way, so equal pixels mean equal palette indices under equal palettes:

| Screen | Call | Flip | Pixels that differ | Colours |
| --- | --- | --- | --- | --- |
| Origin, "presents" | `display_screen(5, 6)` | 1 | 0 of 64,000 | 223 |
| Looking Glass Technologies | `display_screen(6, 7)` | 2 | 0 of 64,000 | 199 |

Neither screen stays up long in either build: the first is replaced when the art has loaded, the second when the title cutscene starts. An unheld DOS boot under js-dos shows neither in screenshots taken 150 ms apart, which is why the comparison holds both builds on the screen instead of timing them.

### What replaces what

Each replacement is written from the DOS assembly, with its comments, and keeps the module's data at the assembly's offsets in the same segment, so a state dump can compare it with DOS. Where seg003's data holds near code offsets (the span writer in 4112, the pens' writers, the pen set-up table, the transfer in 0DC2), they keep their DOS values and the C dispatches on them, and an offset with no C yet stops the port with its address. The FM Towns routines with the same names were read beside them (`show`, `set_the_window`, `init_graphics_`, `lsqrt`, `sincos`, `atan2`, `mouse`, with `tools/fmt.py`). They do the same jobs for the FM Towns' own video hardware, and DOS was followed throughout. One difference touches arithmetic: FM Towns' `mouse` halves the motion with `sar`, which rounds down, where DOS's `_6F8` divides by 200 with `idiv`, which rounds towards zero; the port does what DOS does.

| Replaced | By | What |
| --- | --- | --- |
| SYSENTRY.ASM, STARTUP.ASM, SYSINIT.ASM, CPUTYPE.ASM, VIDSAVE.ASM, SYSLIBL.ASM, TICKS.ASM, TICKREAD.ASM, JOYPORT.ASM's `_30A` | `sys/sysentry.c` | seg021's start-up and shut-down, the exit chain and its message, the far pointers into dseg062_62a6, `mouse`, `mbuttons`, the game clock (a 386, a 101-key keyboard, no joystick) |
| KEYQUEUE.ASM, KBDINT.ASM | `sys/keyqueue.c` | the int 9 ring buffer and `key`, as described under the platform layer |
| MOUSEDRV.ASM | `sys/mousedrv.c` | int 33h from pointer events |
| IMATH.ASM, C3DENTRY.ASM's `cSinCos`, `cFstSinCos`, `cAtan2`, `cSqRt` | `sys/imath.c` | table sines with imul's middle word, arcsines, `atan2`, Newton square roots with div's carry |
| C3DENTRY.ASM's data and `cInit3d` (with GRDISP.ASM's `_5363`) | `sys/c3dentry.c` | the far pointers into the renderer's data; the 3D view's window |
| INT0TRAP.ASM | `sys/int0trap.c` | the divide trap, on SIGFPE |
| OVERLAY.ASM | `sys/overlay.c` | `_OvrInitEms`, which has nothing to do |
| Borland's C library | `sys/borland.c` | the `bc_` functions, handle I/O and text mode, `findfirst`, `intdosx` (3Fh, 40h), `getvect`, `getdfree`, port I/O, the string extras, `_ctype` (from the EXE's DGROUP), Borland's `rand` |
| EMS.C (DOS-only) | `mem/ems.c` | the emulated EMS 4.0 driver: a page store and the frame at E000h, by copying |
| FARDATA.ASM, the far data taken from the EXE, TMPALLOC's page variables | `mem/fardata.c` | the far segments 3705:0000 to 5F5D:104C as one block in the EXE's layout, with the names inside it as labels; dseg062_62a6; PGCACHE.ASM's `lightabs`; all read from the user's EXE |
| segment arithmetic, Borland's far heap | `mem/parmap.c` | the paragraph map, `farmalloc` and the rest, `movedata`, the null-pointer copies |
| GRCORE.ASM (in part) | `gfx/grcore.c` | the graphics globals, `init_graphics`, `init_colors`, `set_the_window`, `set_the_color`, `clear_window`, `rectangle`, `urectangle`, `show`, `setup_font`, `grPageFlip`, `grSoftPageFlip`, the video memory bump allocator `_49AE` |
| VIDMODE.ASM (in part) | `gfx/vidmode.c` | mode X, the mode's pages, Ytab, the edge masks, page flips, the window and its guards, pens, the solid span writer, show's clipping and row records |
| GRLIBF.ASM, GRLIBL.ASM, GRLIBI.ASM (in part) | `gfx/grlibf.c`, `grlibl.c`, `grlibi.c` | clear_window and urectangle; the opaque and transparent bitmap transfers; the text masks and `setup_font` |
| MODEX.ASM (all but `grab`) | `gfx/modex.c` | `local_do_palette`, the console print, the far string helpers, `gr_pixel`, the video memory pictures (2A2, 320, 361) |
| VALLOC.ASM (all but `save_rect` and `restore_rect`) | `gfx/valloc.c` | the video memory pool, `valloc`, `vfree` |
| PGCACHE.ASM's data | `3d/pgcache.c` | `grs_off`, `obj_tab` |
| AIL.ASM | `sound/ail.c` | the timer services on the PIT thread; every driver call returns 0, as AIL does for a handle with no driver |
| the VGA, the PIT | `gfx/vga.c`, `sys/pit.c` | the emulated hardware |

The stubs went from 160 functions and 80 variables in 19 files to 38 functions and 9 variables in 10 files. `tools/portstubs.py` now leaves out every name the port's own C defines. The functions left are the rest of GRCORE.ASM (text, lines, copies between pages, the virtual screen, the frame buffer), SPRITE.ASM, SCALEBM.ASM's `cXfer`, SETPNT.ASM, C3DENTRY.ASM's renderer entries and callbacks, the joystick reads, `save_rect`, `restore_rect` and `grab`. The variables left are DGROUP gaps and `sound_fpage`, which work as zeroed variables.

### Shared-source changes

One, proved by the gate: `AX_RESULT` (`portable.h`), used once, at the end of GRFX.C's `grfx_init`. The original has no return statement there, and `init_world` tests what `grfx_quikfont` left in AL, which is `grfx_load_font`'s result; on the host the function returned nothing defined and the port stopped with error D003. `make check` passes, and `make port-check` shows 0 errors and 60 warnings (61 before).

### Found on the way, for later milestones

- **Four more functions fall off their end** and have callers that use the result: CONVERSE.C (line 805), SCHEDULE.C (113, on some paths), TRIGGER.C's `SetOffTrap` (145) and CUTS.C (414). Each needs `AX_RESULT` with the value DOS leaves in AX, read from the code.
- **A stream and its handle together.** LOADGR.C reads `fileno(grfp)` after `fseek(grfp, 0L, 1)`, which Borland's library made agree with the handle by emptying its buffer. The host's stdio keeps its buffer, so the offsets table came back wrong and every art file failed (error D004). The port's streams are unbuffered.
- **String literals are read-only on the host.** `init_sounds` writes the driver's name into the literal `"dm00.adv"`, which crashes the host. It runs only with a sound card configured, so the GOG configuration never reaches it; Milestone 7 meets it first.
- **Division by zero does not trap on arm64.** x86-64 raises SIGFPE and the port runs the game's int 0 handler; Apple Silicon gives 0 and goes on. The promotion audit's divide sites are where to look if a replay disagrees.
- **CUTS.C writes the screen through a far pointer** (`MK_FP(0xA000, 0)`, with the map mask set by `outportb`). A host pointer cannot apply the map mask, so the planar writes need a macro whose Turbo C expansion is the original store and whose host expansion calls `vga_write`.
- **EMS mapping by copying** cannot show one logical page mapped into two frame slots at once, as real EMS can. No such mapping has been seen yet.
- **The clock is read without a barrier.** The game reads `*Time` directly while the PIT thread increments it. The port is built without optimisation, so every read happens; `GAME_TIME()` (Milestone 4) makes it explicit.
- **Paragraph map windows.** `intoFarBuffer_ovr167_5DA` and `FarWrite_ovr167_627` take `FP_SEG` of near buffers on the stack, which makes a window each time (`-v` logs them). The idiom deserves a named macro.

## Milestone 4 results

Milestone 4 built the record and replay harness and took the port from the title into the game, as far as the first 3D frame. Run on 2 October 2026 (Apple clang 21, SDL 3.4.16, js-dos through dos-mcp 0.3.0), with the standard session `newgame` (boot, the title cutscene, Escape twice through the introduction, the main menu, Create Character with every default, the name Avatar, then sixteen seconds in the game):

- **DOS replays itself exactly.** The session was recorded in DOS (28 million hook calls, a 27 KB recording) and replayed twice; all 43 checkpoints of the two replays are identical, in every section: the player record, the level, Borland's rand seed, seg003's data, the DAC and all 256 KB of video memory. Two things had to be found first: the stop key, and a local the game reads before setting it (below).
- **The port replays it exactly as far as it goes.** Every checkpoint from boot to the game screen, 37 of them, is identical to DOS's, in every section, including the video memory of every screen: the title cutscene, the introduction's first frames, the main menu, each of character creation's screens with its text and buttons, the name typed, the confirmation, and the game screen drawn before its first 3D frame (`CHECKPOINT(4)`). The port then stops at `cRender`, the 3D renderer, which has no C yet.
- **The exit test is met up to the renderer.** What is left is the first 3D frame and the rest of the session, which needs `cRender` for every frame.

### The replay harness

**Hooks.** `portable.h` has the hooks (CONTRIBUTING.md, "Writing for both builds"): `GAME_TIME()` (71 reads of `*Time` in 19 files), `KEY()`, `MOUSE()`, `MBUTTONS()`, `JOY_READ()`, `JOY_BUTTONS()`, `WALL_TIME()`, `SRAND()` and `CHECKPOINT(n)` (five, in `main` and `strt_demscr`). Under Turbo C each is the original tokens, and the gate proves it. Under `-DREPLAY` and on the host they call `src/replay/REPLAY.C`, which in DOS is linked into the modding build as one more resident module (`tools/link.py --mod --add`). Recording, a hook reads the world as the game would and writes what it got; replaying, it returns what was recorded and leaves the world alone (key events restore seg021's key state, `key_on` and the modifiers, from the recording). The DOS build replays when `REPLAY.IN` is in the game's directory and records otherwise; the port takes `--record` or `--replay FILE` and otherwise reads the world.

**The recording.** The game polls its inputs hundreds of thousands of times a minute and almost always gets what it got last time, so each kind of input is a stream of runs, a count and a value; the program makes its calls in the same order on replay, so the streams need not record how they interleave. The streams are written in chunks of up to 200 bytes, tagged with the stream, as their buffers fill. The header holds the call count at which recording stopped: F12 stops it (the game never sees the key), and a replay stops at the same call. The session above is 18.3 million clock reads in 8,443 runs and 3 million polls of each input in 47 to 107 runs.

**The dumps.** Both builds write `STATE.OUT`: a record per checkpoint (a `CHECKPOINT`, every 400h ticks of the replayed clock, each key event, each change of the buttons, the end), named by the call count and the clock. Each has the player record, the rand seed, the level block once one is loaded, the null-pointer area, the far blocks' segments and the calls per stream; the full ones (all but the periodic) add seg003's data, the DAC, six CRTC registers and the four planes of video memory. `src/replay/REPLAY.C` has the formats.

**The tools.** `tools/replay.py` builds the replay DOS EXE, records and replays in DOS through `tools/replaydos.mjs` (js-dos, about forty seconds a run), replays in the port, and compares two dumps checkpoint by checkpoint, the screen also as the CRT controller would show it; `check` does all of it. Two ranges of seg003's data are machinery and are not compared: its private stacks (370D:4E00..4FA7 and 5500..5547), where interrupts push whatever is in the registers, and the tick count its retrace wait keeps (0652). Words that are a far block's segment are compared as segments, since the load segment differs between the replay EXE, DOS set-ups and the port. `UWRPCK` sets the periodic interval, and `UWRPTRACE=lo,hi` writes every hook call in a clock range, with its caller's address, to `TRACE.OUT`; that is how the next point was found.

**Stack junk.** The first DOS replays parted in the game by one poll of the mouse. The trace showed `player_attack` reading its local `held` before setting it when there is no attack key, and polling the mouse only when the junk is zero ([FINDINGS.md](FINDINGS.md#the-attack-check-reads-an-uninitialised-flag)). The timer interrupts write the game's stack, so the junk differs from run to run. `STACK_JUNK(0)` gives it 0 in the replay build and the port; clang lists five more such locals, not reached yet.

**Two port bugs the replays found.** Besides those under "What replaces what": the port's macros that store a word into the emulated data segments evaluated the value twice, so `SETW(o, W(o) + n)` read back its own new low byte and got the high byte wrong whenever the low byte carried (it showed as a lost record in VALLOC's block list and one sprite's background never saved); and the string entries' copy to 370D:4FA8 takes one byte past the string's 0, from the caller's buffer, which is stack junk, so the comparison leaves that byte out.

**The js-dos side.** dos-mcp's `fsRead` refuses a second read of a path, even one that failed because the file was not there yet, so `replaydos.mjs` waits for the batch's `DONE.TXT` by listing the directory, and every call into the page has a time limit.

### Null pointers

- **DOS writes.** Every dump has DS:0 to 30h, C0's null-pointer checksum over DS:4..30h and the interrupt vector table. Over the session the checksum stays intact, DS:0..3 always holds one of the two forms of ovr167's stub entry (`27 06 00`, or `06` and the overlay's segment while it is loaded), and no vector changes after `init_world`. After the game exits, dos-mcp's `read_memory` finds DGROUP by C0's copyright string and reads the same: DS:0..3 `06 77 7A 00` (ovr167 loaded), checksum intact, so no "Null pointer assignment".
- **DOS reads.** The replay build compiles the `NULLTRAP`-marked sources with `-DNULLTRAP`; none of the marked sites was reached.
- **The port.** `make port-debug` builds with UBSan's `-fsanitize=null`; over the whole session it reports no null dereference, and the port's stand-ins for DS:0 and the vector table were not needed.

### What replaces what

Each replacement is written from the DOS assembly, as in Milestone 3, keeping the module's data at the assembly's offsets so that the dumps compare it with DOS.

| Replaced | By | What |
| --- | --- | --- |
| VIDMODE.ASM (the rest) | `gfx/vidmode.c` | the span writers (solid on both pages, copy, copy onto both pages, save, restore, xor), pixels, vertical lines, the virtual screen and `vscreen_focus` (line compare, display start, panning), the linear buffer, and the bitmap entries with their three record builders (show, fbshow, `_5025`, `_511C`, vcopyfb, vcopy) |
| GRLIBF.ASM (the rest) | `gfx/grlibf.c` | page copies, pixel, lines, boxes |
| GRLIBI.ASM (text) | `gfx/grlibi.c` | `string_width`, the rasteriser into the bit buffer, the single-colour blitter, `shift_left_1` and the shadowed string |
| GRLIBL.ASM (the rest) | `gfx/grlibl.c` | the transfers from video memory (plane by plane, latched, transparent), screen to screen both ways, into the linear buffer |
| GRENTRY.ASM | `gfx/grentry.c` | the frame buffer: clear, fill, dim, light, `cPlaceFB`'s set-up (the copier's self-written ret is a variable), `cFBtoScreen`, its span writers and the Gouraud spans |
| GRCORE.ASM (the rest but `grab`) | `gfx/grcore.c` | the C entries, including the string entries' copy to 370D:4FA8 |
| SPRITE.ASM | `gfx/sprite.c` | the sprites, quirks included (FINDINGS.md) |
| VALLOC.ASM's `save_rect`, `restore_rect` | `gfx/valloc.c` | through pens 109h and 10Ah |
| EXPAND.ASM, PGCACHE.ASM's `uncmp_tab` | `3d/expand.c` | the image decoders (the record decoder's self-modifying ret is a flag) |
| SETPNT.ASM | `3d/setpnt.c` | `SetPnt` |
| C3DENTRY.ASM (all but `cRender`) | `sys/c3dentry.c` | `cPlaceFB`, `cFBtoScreen`, `cFillFB`, `cDimFB`, `cLiteFB`, `cZoom`, `cFrmtoRaw` |

The port's own fixes: the EXE's relocations are applied to the far data it loads (the segment words in seg003's data were the EXE's, not the loaded ones); the CPU's window at A000:0000 is a region of the paragraph map whose stores go to the VGA (`PLANAR_STORE`, and `mem_set` into it), with slack either side for offsets that wrap; the EMS page frame is mapped twice in a row, so that a pointer run past E000:FFFF wraps to E000:0000 as a far pointer does (the cutscene player walks a large page's records that far, and on the host it overwrote the paragraph map); and Borland's `srand` keeps only the seed's low word. The stubs left are `cRender`, `grab`, the two joystick reads, and nine zeroed variables.

### Shared-source changes

All proved by the gate (`make check` passes; `make check-all` too):

- `portable.h`: the replay hooks, `PLANAR_STORE`, `STACK_JUNK`, `FILE_RECORDS` and `FILE_RECORDS_END`, `AX_LAST`.
- `GAME_TIME()` in 19 files; the input hooks in MOUSE.C, JOYSTICK.C, UWEDIT.C, CHARGEN.C, BABLHACK.C and BARTER.C; `CHECKPOINT` 1 to 5 in UWEDIT.C.
- CUTS.C's planar writes through `PLANAR_STORE` (five stores).
- COMBAT.C's `held` with `STACK_JUNK(0)`.
- Locals filled by a file read given their DOS width: GAMESTRN.C's `read_string` (on the host the upper bytes were junk, so every string from STRINGS.PAK came back empty) and ARC.C's three block counts.
- CHARGEN.C reads `DATA\chrgen.dat` through `FILE_RECORDS`: its question records are laid straight over the file's bytes, but `struct ChrOpt` holds pointers, so on the host it has another layout.
- `AX_RESULT` or `AX_LAST` at the end of CONVERSE.C's `check_inv_quality`, SCHEDULE.C's `do_migrations`, TRIGGER.C's `SetOffTrap` and CUTS.C's `readlp`, the values read from the code.

`make port-check` shows 0 errors. The tools: `link.py --add`, `sources.py` (src/replay, which the gate skips), `portbuild.py --debug`, `portcheck.py` and `portstubs.py` (which now include REPLAY.C), `replay.py` and `replaydos.mjs`.

### Screens

`tools/portshot.py`'s two held screens still match (0 of 64,000 pixels). The replay comparison is stronger than further hold points: it compares all four planes of video memory and the DAC at every full checkpoint, on every screen the session passes through, so no new hold points were added.

## What is left of Milestone 4, and the Milestone 5 plan

**The 3D renderer (Milestone 4's last step).** `cRender` and what it runs: seg004 in `src/3d` (INSTANCE, INTERP, CLIP, PROJPOLY, SCANLINE, TEXMAP, TEXMAPV, PERTP, PERTLP, SMOOTH, SPHERE, TMAPOPS, the rest of PGCACHE) and seg003's GRMISC, GRDISP, GRLIBM, GRLIBN and SCALEBM, about 15,000 lines of assembly, written in C from DOS with the FM Towns routines as a reading aid, as planned under "The FM Towns build as a template". The exit test is the newgame session replaying to its end with identical dumps (the frame buffer is in `stdat`, and every frame reaches video memory through `cFBtoScreen`). These constraints cost nothing now and keep phase 2 open; none may change the faithful output:

- `render_3d` and the C written for seg004 draw into a frame buffer they are given (a pointer, width, height and pitch), not into 320 by 200 globals, wherever that costs nothing.
- Every object and critter sprite is drawn through one call per object (the scaled-sprite path: SCALEBM.ASM and the renderer's object opcodes) that receives the object id, the frame, its world position and facing, its screen position and scale, and the light level, so that an enhancement can put something else there (a voxel model) without touching the faithful code.

**Milestone 5: sound, then the rest of the game loop.**

1. Sound: AIL's API in C, translated from the AIL 2.14 source; the XMIDI sequencer; the OPL backend (Nuked OPL3 with the AdLib driver's timbre handling and UW.AD) and the MT-32 one (libmt32emu); the digital channel with AIL's double buffering for effects, speech and the cutscenes. With a card configured, the 16 Hz effects timer changes state the game reads (`dsfx_playing`, the effect channels), so a replay with sound drives the AIL timers from the replayed clock rather than from the host's.
2. The rest of the game loop: the panels, inventory, message scroll and automap screens, conversations (BABL.C's four-byte reads into an `int` need care, FINDINGS.md), the locals clang lists as read before set, saves moving both ways, and longer recorded sessions into several worlds.

## Phase 2 enhancements

Phase 2 adds options; faithful mode, pixel for pixel DOS, stays the default and the replay suite runs with every option off.

- **Higher-resolution software rendering, with the palette lighting kept.** The renderer draws into a frame buffer it is given, so a larger one can be passed. What assumes 320 by 200 today has to be generalised: the frame buffer's row table (370D:095C, 200 rows) and its offsets in `stdat`, the projection constants and the view window (`cPlaceFB`, `place_3d_view`), the span and texture mappers' fixed-point steps, the pick buffer (one colour per object, read back at the cursor), the scaled sprites' clipping, and the 2D layer around the view, which stays at 320 by 200 and is scaled. Lighting keeps going through `lightabs` and the palette, so the look is the game's.
- **Presentation shaders through SDL3's GPU API**: integer and smooth scaling, aspect correction, and CRT-style shaders, applied to the scanned-out picture, never to the game's own drawing.
- **Voxel sprites, as an option.** The per-object sprite call above is the hook: an enhancement draws a voxel model there instead of the sprite, in software, with slab rendering in the style of Build's KVX and lit by the same palette tables. The models come from hand-made `.vox` or KVX files, or are generated on the user's own machine from the game's directional sprite frames by visual-hull carving and cleaned up by hand. Since they are derived from the game's art, generated models are never shipped.
- **A GPU renderer**, through SDL3's GPU API behind the same render interface. Low priority: UnderworldGodot already gives the game a modern 3D engine.

## Risks

- **Silent arithmetic differences.** 16-bit wrap and promotion differences produce no diagnostic and can change game logic far from their cause. The mitigation is the explicit widths, the promotion audit and the replay suite, which together catch them close to the cause.
- **Byte-identical changes may not exist.** Some portability changes may have no spelling that Turbo C compiles to the same bytes. Those changes go into port-only wrappers, which then need their own tests.
- **Bitfield placement.** Settled for every struct in the headers: the layout check shows Turbo C and the packed host layout agree. A struct added later is checked by running `tools/layoutcheck.py` again.
- **Structs with pointers over shared buffers.** They grow on the host, and code that sizes them with numbers (BABL.C's heap) or lays other data after them (`stdat`) needs port-side care; see the Milestone 2 results.
- **Replay determinism in DOS.** The replay DOS build has to record every outside read, including the reads the renderer makes mid-frame through `do_mouseq`. A missed read shows up as two DOS replays that disagree, which Milestone 4 checks first.
- **The renderer's size.** seg004 is 33 KB of hand-written 386 code with self-modifying parts and a threaded interpreter, and seg003 is 24 KB more. The FM Towns names and the frame-buffer comparison keep the work checkable, but it is the largest single piece of the port.
- **Far data taken from the EXE.** About 83 KB of tables have no source yet. Loading them from the user's EXE keeps the port honest, but the offsets tie the port to the GOG `UW2.EXE`, which the port checks by its size and hash at start-up.
- **Asynchronous timers.** In DOS the clock interrupt can land between any two instructions. The port's timer thread gives the same effect, but data races that were harmless on one 386 can matter on a multi-core machine. Every value the timer callbacks share with the game is accessed atomically, and the replay suite runs with the clock recorded.

## Prior art

- **Chocolate Doom and Crispy Doom.** Chocolate Doom reproduces vanilla Doom's behaviour and limits, and uses demo compatibility as its test. Crispy Doom adds enhancements on top of the same code. This port follows the same split for its two phases.
- **The SM64 decompilation and its PC port.** One tree builds the matching N64 ROM and the PC port, with the platform code kept apart. That is the model for the two builds here.
- **Ship of Harkinian.** The Ocarina of Time PC port is built on a separate platform library, libultraship, which other ports have since reused. That is the reason this port keeps its platform layer independent of UW2 for a later UW1.
- **DevilutionX.** It began from Devilution, a Diablo decompilation that kept a build matching the original, and then moved to portable code step by step. It also records and replays demos for its regression tests.
- **OpenRCT2.** It began by replacing the original game's functions one at a time while the original still ran. It also has replays with game-state checksums. This port uses the same incremental replacement and the same kind of state comparison.
