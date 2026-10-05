# The map, the names and the matching metadata

Four things at the top of the repository describe `UW2.EXE` rather than rebuild it:

- `map/` lays out the whole program: every segment and function, its original name where one was found, and every source file's name and the evidence for it.
- `targets/` has one target table per source: the segment's base in the EXE, its size and the functions in it. `match.py` and `verify.py` compare a source with its table.
- `symbols.tsv` maps every name the sources share to its address in `UW2.EXE`.
- `matched.txt` lists the segments whose source matches whole and verifies.

## The map

Turbo C puts each source file in its own code segment, so each DOS segment is one original source file. The Japanese FM Towns release kept 3237 of Looking Glass's original names, and it was linked from the same object list in the same order, so the two builds can be aligned function by function.

- `map/files.tsv`: every DOS code segment, whether it is C (99 segments, about 1480 functions and 337 KB), assembly (graphics, the 3D renderer and sound) or library, how much of it is named, and whether it is matched. Its sizes for the assembly come from IDA's function map and overstate them; the target tables have the true extents.
- `map/functions.tsv`: every DOS function with its original name where one was found, and how: `anchor` (proven), `confirmed` (aligned, and its callers and callees agree with the FM Towns call graph), `size only`, or `size, calls disagree`.
- 884 of the 1886 non-library functions have a confirmed original name. Tested by holding out known pairs, confirmed names were right 80 times out of 82, and both misses disagree with a hand-made anchor rather than a proven one.
- The rest are mostly DOS-only code with no FM Towns counterpart: the assembly, a few DOS-specific C files (ovr095 and the small resident segments seg011 to seg019), and a stretch at the very end (ovr158 onwards) past the last anchor.
- `map/filenames.tsv`: every source's file name, its segment, the name it had before 2 October 2026, and how good the name is (below).

The committed map files and these counts are from 1 October 2026. Rerunning the pipeline now, with every segment matched, finds 1422 anchors against the committed 829, so the counts would change.

### Rebuilding it

The pipeline needs the IDA listing and the FM Towns symbols ([BUILDING.md](BUILDING.md#requirements)). Run, in this order:

1. `tools/doslist.py > map/dos_procs.tsv` lists every DOS segment's procedures from the IDA listing; `grep -v '^stub' map/dos_procs.tsv > map/dos_code.tsv` drops the overlay stubs.
2. `tools/locate.py` finds each segment in `UW2.EXE` and writes `map/segments.tsv` and `map/procs.tsv`.
3. `tools/callgraphs.py` writes both call graphs, `map/dos_calls.tsv` and `map/fm_calls.tsv`.
4. `tools/callpairs.py src/FILE.C > map/pairs_SEG.tsv`, after match.py has built the file, pairs a matched file's far callees with IDA's names.
5. `tools/anchors.py` collects the proven pairs into `map/anchors.tsv`: names from the target tables of matched segments, `symbols.tsv`, the `pairs_*.tsv` files and UWReverseEngineering's string anchors.
6. `tools/align.py` aligns the two builds between the anchors and writes `map/functions.tsv`. `--holdout WHY` scores it on one kind of anchor.
7. `tools/files.py` summarises it per segment into `map/files.tsv`.

## Target tables

`tools/targets.py SEG > targets/SEG.tsv` writes a segment's table from the map: verified segment bases and function offsets, and original names where the map confirms them. Code segments are byte-aligned, so a file starts at its first function, which may be a few bytes past the segment's paragraph; the table's `org` records that. The last function's size runs to the first far return after it.

A segment split into several source files has one table per module, named `SEG_OFFSET`: seg003's 14 modules are `seg003_0272_0` to `seg003_0272_5B50`, and SetPnt, the assembly at the start of seg019, is `seg019_21BA_C` beside `seg019_21BA` for GRIDDB.C.

`match.py` finds each function by the table's name for it, so a table row whose name the source no longer defines is skipped. After renaming functions, `tools/syncnames.py src/FILE.C` renames the rows to the names the built object defines.

## symbols.tsv

`symbols.tsv` holds every public and extern of the matched sources with its address in `UW2.EXE`: `DS:offset` for near data, `segment:offset` for far code and data (load-relative paragraphs). Each name is marked `FM Towns` (an original name), `library` or `provisional`. `verify.py --update` merges a file's names into it and refuses a second name for an address or a second address for a name. The gate rebuilds it from scratch and requires it to equal the committed file.

## matched.txt

One segment per line, by its IDA name as in `map/files.tsv`. A segment split into modules is listed once. `tools/files.py` reads it for the `matched` column of `map/files.tsv`, and `tools/anchors.py` for which target tables count as proven. Every segment that holds code, apart from the C runtime library, is listed now.

## Source file names

No UW2 build, string or document names a UW2 source file, so the names come from three kinds of evidence, recorded per file in `map/filenames.tsv`:

- **original** (9 files): the name is in a related code base with the same functions doing the same job. Eight come from System Shock's source release (Looking Glass, 1994, built on the Underworld engine, its RCS headers giving each file's original path): `gfx/VALLOC.ASM` (valloc, vfree), `3d/INTERP.ASM` (17 of the model interpreter's opcode handlers, System Shock's `interp.asm`), `ui/INPUT.C` (init_input), `combat/DAMAGE.C` (damage_object), `ui/GAMESTRN.C` (init_strings, get_string), `ui/WRAPPER.C` (draw_button), `game/GAMEWRAP.C` (copy_file) and `game/PLAYER.C` (init_player; this is ovr143, which sets up the player, while ovr154, once called `PLAYER.C`, is `game/SKILLS.C`). `sound/AIL.ASM` is the module of Miles' public-domain AIL 2.14 source that has the same procedures.
- **inferred** (34): consistent with the FM Towns names or with a System Shock file's job but not proven. That is a module prefix (`obj/OBJECTS.C` for Obj_, `event/SCHEDULE.C` for Sched_, `sys/ARC.C` for arc_, `gfx/GRFX.C`, `gfx/CUTS.C`, `conv/BABL.C`, and the object class files `obj/ANIMOBJ.C`, `HACK.C`, `MISC.C` and others after their `*_class_data`), a file named after its first function (`game/MAINLOOP.C`, `ui/AUTOMAP.C`), or a System Shock file with the same job (`critter/AI.C`, `critter/PATHFIND.C`, `motion/PHYSICS.C`, `ui/MOUSE.C`, `3d/GAMESORT.C`, `obj/EFFECT.C`, `event/TRIGGER.C` and others). `game/UWEDIT.C`, the file with main, is named after the program: TLINK stored `uwedit.exe` in UW2.EXE, and the file has init_edit and editexit.
- **descriptive** (111): our name for what the file does, such as `conv/BARTER.C` or `combat/SPELLS.C`. Modules of seg003 and seg021 whose job is not yet known keep a family prefix and their module letter (`gfx/GRLIBF.ASM`, `sys/SYSLIBL.ASM`).

Renaming a source file changes its object's segment names but not the EXE; [LINKING.md](LINKING.md#segment-names) says why.
