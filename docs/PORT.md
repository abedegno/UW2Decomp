# Porting UW2 to modern systems

This page is the design for a native port of UW2 built from the matched sources. Milestones 1 and 2 are done: the C compiles on a modern host with no errors, and `make port` links it with stubs into a native macOS binary that starts and stops at the first stub. Their numbers are below, followed by the plan for Milestone 3. Nothing else described here exists yet.

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
| `src/port/3d/`, `src/port/gfx/`, `src/port/sys/`, `src/port/sound/` | C written for the port to replace the assembly modules and the DOS-only C files, named after the module each replaces | no |
| `src/port/mem/` | the paragraph map, EMS and the far heap | no |
| `src/port/stubs/` | the link stubs, one file per source of the names (written by `tools/portstubs.py`); a replacement deletes its stubs | no |
| `platform/include/plat.h` | the platform API | no |
| `platform/sdl3/` | the SDL3 implementation of the platform API | no |
| `tools/portcheck.py` | the compile measurement (`make port-check`) | no |
| `tools/portbuild.py` | the port build and link (`make port`) | no |
| `tools/portstubs.py` | writes `src/port/stubs/` from the names a link still needs | no |
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
- It packs every struct the game declares (`#pragma pack(1)`), as Turbo C does with no `-a`, after it has included every host header the game uses, so the host's own structs keep their alignment. Port C that includes a game header must include `compat.h` first.

The game's translation units see only ISO C and POSIX 2008 names from the host (`-D_POSIX_C_SOURCE=200809L`). Without that, macOS's headers declare `valloc`, which collides with VALLOC.ASM's routine of the same name.

The shared sources also include `src/include/portable.h`, through `uw2.h`. It holds what both builds need: under Turbo C (`__TURBOC__`) every name in it is the original type or the original tokens, so the DOS bytes cannot change, and on the host it takes the host's meaning. It has the explicit-width types (`int16`, `uint16`, `int32`, `uint32`), `NEARPTR` and `UNEARPTR`, `OLDSTYLE`, `FAR_COPY`, `NULLTRAP` and `FARNULLTRAP`, and `HOST_LAYOUT_BEGIN` and `HOST_LAYOUT_END`. Each is described below where it is used.

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
| 2. Compile and link (done) | gated source changes for the errors and the pointer, prototype and Borland issues; explicit widths; the layout check; the promotion audit; the null-pointer static pass; the far pointer macros; link stubs; `make port` | `make port-check` shows 0 errors and no pointer truncation in the shared C; `make port` links a native binary with stubs that abort; `tools/layoutcheck.py` shows every file record identical; the gate passes |
| 3. Boot to the title | the platform layer on SDL3 (video, keyboard, time, files); the paragraph map and EMS; Borland's library; seg021 in C; the parts of seg003, MODEX and the VGA emulation that the first title screen needs; AIL as "no driver found"; `port_null_near` and `port_null_far` | the port runs `init_world` up to `display_screen(5, 6)` and shows the first title screen, and its VGA scan-out matches a DOS screenshot of that frame byte for byte |
| 4. Replay hooks | `GAME_TIME()` and the input hooks in the shared sources; the replay DOS build; `state_dump()`; the null-pointer write check | a DOS session recorded and replayed in DOS gives identical dumps twice in a row; quitting after it prints no "Null pointer assignment", and DS:0 to DS:3 and the vector table are as before |
| 5. System and screen | the rest of seg003, the sprite, VALLOC and MODEX modules, the mouse; a `-fsanitize=null` debug build | the port plays the title cutscene and shows the main menu; screens match DOS byte for byte at the menu |
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

## The Milestone 3 plan

Milestone 3 boots the port to the first title screen. `init_world` (UWEDIT.C) brings the game up in a fixed order, and the first screen it shows is `display_screen(5, 6)` after `grfx_init`. So the work is everything that order calls up to that point, replaced in the order it is called, each stub file shrinking as its names get real code:

1. **Process start.** `getvect` and `setvect` keep a table of handlers, and `int0_trap` becomes the port's divide handler (a signal handler for `SIGFPE` that calls `DivideByZeroError`); `_SS` and `_SP` read the register file; `bc_time`, `bc_srand` and `bc_rand` with Borland's generator. The port's `main` sets the data and save directories and calls the game's `main`.
2. **Memory.** The paragraph map (`port_mk_fp`, `port_fp_seg`, `port_fp_off`), `farmalloc`, `farfree`, `coreleft` and `farcoreleft` (reporting what DOS reported on the reference set-up, since `OkEnoughMem` tests it), and EMS: the five EMS.C functions and the page frame as a region, with the TMAPOPS and PGCACHE page state (`EmsBuff`, `tmap_fpage`, `crit_fpage` and the rest) as plain variables. `stdat`, `cmpbuf1_start`, `cmpbuf2_start`, `gr_offs` and the rest of FARDATA.ASM become arrays in the paragraph map, with `ATM_Strings` inside `stdat`.
3. **Files.** The `bc_` file functions and `tell`, `eof`, `filelength`, `getcurdir`, `findfirst`, `findnext` and `getdfree` on the platform layer's path mapping, so `check_dirs`, `check_fds`, `init_strings` (STRINGS.PAK), `Map_Init` and `ReadCfg` run on the user's data. `intdosx` and `bc_geninterrupt` cover the DOS calls the C makes itself.
4. **Sound off.** The AIL entry points report no driver, the path a DOS machine without a sound card takes, so `init_sounds` and `init_timers` succeed silently; the 256 Hz clock is a platform timer that increments `*Time` from the start.
5. **Graphics.** `grfx_init` and `display_screen` need GRCORE's set-up (`init_graphics`, `init_colors`, `setup_font`, `set_the_window`), `palette` and MODEX's `local_do_palette`, the screen copy (`show`, `vcopy`, `grPageFlip`), and the archive and decompression C, which already compiles. The VGA emulation needs the planes, the map mask and the DAC; the platform layer presents the scan-out in a window. The FM Towns routines of the same names are the template, and the DOS assembly is the authority.
6. **Null pointers.** `port_null_near` and `port_null_far` with the DGROUP and vector table images described above.

The exit test is a screenshot: the port's VGA scan-out at the first title screen against the same frame captured in DOS through dos-mcp, byte for byte. Milestone 3 also starts the replay hooks' DOS side (Milestone 4), which needs no port code.

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
