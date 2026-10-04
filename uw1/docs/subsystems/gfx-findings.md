# Graphics: candidates for the findings page

Candidates from `src/gfx` for the project's findings page; [gfx.md](gfx.md) describes the subsystem. Each bug candidate was re-read in the matched source. What the code does is read from the code; the effect in the game is an inference unless it says it was checked.

## Possible bugs

### A full-screen picture that cannot be read counts as shown

- **What happens:** `LoadBitMap_ovr141_0` returns 0 only when it had no buffer (the workspace was lent out and `farmalloc` failed). When `bltfromdrive` cannot read the file it skips the drawing and still returns 1.
- **Where:** [gfx/SHOWPIC.C](../../src/gfx/SHOWPIC.C). The one caller that tests the result is `strt_demscr` in [game/UWEDIT.C](../../src/game/UWEDIT.C): `if (!LoadBitMap_ovr141_0(-1, "DATA\\main.byt")) pfatal_code(ERR_READ | 0xB);`.
- **Evidence:** code reading.
- **Confidence:** possible (the result may have been meant as "had memory").
- **Effect:** with `DATA\MAIN.BYT` missing or short, the game does not stop with error B00B but starts with whatever the buffer held as the screen frame. Not tried.
- **For a port:** matching DOS means ignoring a failed read here.

### `mem_set` leaves the last byte of an odd count

- **What happens:** MODEX.ASM's far `mem_set` stores `count / 2` words and nothing more, so an odd count leaves the last byte unset. UW2's copy stores the odd byte.
- **Where:** `_mem_set` in [gfx/MODEX.ASM](../../src/gfx/MODEX.ASM).
- **Evidence:** the bytes against UW2's (docs/NOTES.md, assembly wave 1).
- **Confidence:** possible, with no effect: its only callers (BABL.C, 0x1000 bytes; PATHFIND.C, 0x5000 bytes) pass even counts.

## Engine findings

- **Two wall texture mappers.** seg004 calls seg003's texture mappers through a far pointer (seg051:B006): WALLMAP.ASM's (`_seg003_1DA`, UW1 only) for floor-style polygons (`do_tmap`, which GRIDDB.C's `txtflr` emits for every textured floor, `do_gtmap`, `do_compact_tmap`, or `do_set_gmap_ctxt` with a zero flag) and POLYFILL.ASM's (`_seg003_545`) for wall-style ones (`do_compact_wtmap`, `do_gwtmap`). So both are in use. UW2 has a 386 mapper in seg004 instead and leaves POLYFILL uncalled.
- **LPFDELTA is live in UW1.** UW1's cutscene player decodes its run/skip/dump deltas with `seg002_A` into a linear buffer; UW2 has the same routine but no caller.
- **The icon numbers follow the art files.** An icon number from 1000h or 2000h is a running index across the .GR files in `load_all_gr`'s order, so gfx.h's `ICON_*` bases follow from the picture counts in UW1's file headers; every literal icon number in the UI code falls inside the file it is drawn from.
- **The presents screens use palettes 5 and 6.** `UWEDIT.C` shows `pres1.byt` with palette 5 and `pres2.byt` with palette 6, where UW-Formats' list gives 5 for both.
- **Debug traps.** POLYFILL.ASM and GRDISP.ASM each have an `int 2`, and MAPDATA.ASM's words record the last wall polygon for nobody to read; probably aids for a debugger in the development setup.

## Dead code

- `QUADFIT.ASM`: the quadratic fit, no caller (UW2 keeps it at the end of POLYFILL.ASM, also uncalled).
- `MAPDATA.ASM`: written by POLYFILL, never read.
- `PLANECPY.ASM`: the 4 by 4 block copy, no caller (as in UW2).
- `VSTATS.ASM`'s `valloc_stats`: UW1 only, no caller.
- `GRSPIC.C`'s static `seg009_2C6`: reads an icon's size, no caller; its video memory branch passes the addresses of its own parameters, so its result would be lost.

## Open questions

- What QUADFIT's coefficients were for (UW2Decomp's reading: another stepping for the wall mapper's texture coordinate).
