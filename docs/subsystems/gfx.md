# Graphics, art and cutscenes

This page describes how UW2 puts pictures on the screen: the graphics library (DOS segment seg003, 14 assembly modules in `src/gfx/`), the smaller assembly modules around it (`SPRITE.ASM`, `VALLOC.ASM`, `LPFDELTA.ASM`, `MODEX.ASM`, `PLANECPY.ASM`), and the C that loads art, manages palettes, saves screenshots and plays cutscenes (`GRFX.C`, `GRSPIC.C`, `LOADGR.C`, `COLCYCLE.C`, `SHOWPIC.C`, `SCRSHOT.C`, `CUTS.C`). The declarations are in `src/include/gfx.h`. The 3D view is drawn by seg004 into a frame buffer that this library copies to the screen; [the 3D renderer](3d.md) describes that side. Statements marked "probably" are inferences; the reason is given with each.

## Files

| File | Segment | Name | What it does |
|---|---|---|---|
| `GRMISC.ASM` | seg003 module 1 | descriptive | far thunks for seg004's points and lines, the frame-timing hook, the retrace wait, the "map bitmap error" break |
| `POLYFILL.ASM` | seg003 module 2 | descriptive (the name predates knowing its job) | an older wall texture mapper, FM Towns' `wmap_bitmap`, never called in DOS |
| `GRENTRY.ASM` | seg003 module 3 | descriptive | the 3D frame buffer: clear, dim, light, copy to the screen, its span writers and the Gouraud spans |
| `SCALEBM.ASM` | seg003 module 4 | descriptive | scaled bitmaps (sprites in the 3D view) with code generated per sprite; the XFER colour tables |
| `VIDMODE.ASM` | seg003 module 5 | descriptive | mode X, the row table, pages, scrolling, the palette, the screen span writers, bitmap transfers |
| `GRLIBF.ASM` | seg003 module 6 | descriptive | points, lines, boxes and rectangles, clipped and not; the polygon clipper; video memory allocation |
| `GRLIBG.ASM` | seg003 module 7 | descriptive | `cline`, a clipped line |
| `GRLIBH.ASM` | seg003 module 8 | descriptive | `uline`, an unclipped line |
| `GRLIBI.ASM` | seg003 module 9 | descriptive | filled polygons; text (fonts, string drawing and width) |
| `GRCORE.ASM` | seg003 module 10 | descriptive | the C entry points and the near jump table |
| `GRDISP.ASM` | seg003 module 11 | descriptive | the dispatcher the thunks jump to |
| `GRLIBL.ASM` | seg003 module 12 | descriptive | bitmap row transfers (opaque, transparent, latched copies) |
| `GRLIBM.ASM` | seg003 module 13 | descriptive | the Gouraud (smooth) polygon |
| `GRLIBN.ASM` | seg003 module 14 | descriptive | the clipper for smooth polygons |
| `SPRITE.ASM` | seg000 | descriptive | sprites: pictures redrawn in place over the screen, in four layers |
| `VALLOC.ASM` | seg001 | original (System Shock's `valloc.c`) | allocating spare video memory; saving and restoring screen rectangles |
| `LPFDELTA.ASM` | seg002 | descriptive | a Deluxe Paint run/skip/dump decoder nothing calls |
| `MODEX.ASM` | seg017 | descriptive | mode X screen access, the palette loader, far string helpers |
| `PLANECPY.ASM` | seg020 | descriptive | copies a 4 by 4 block to the screen; nothing calls it |
| `GRSPIC.C` | seg009 | descriptive | icon numbers to pictures; drawing them on the screen or into the frame buffer |
| `COLCYCLE.C` | seg023 | descriptive | palette colour cycling |
| `GRFX.C` | ovr118 | inferred (the `grfx_` prefix) | graphics start-up, fonts, palettes, fades, 3D view dissolves |
| `LOADGR.C` | ovr119 | descriptive | loading .GR art into EMS or video memory and .TR textures into EMS |
| `SCRSHOT.C` | ovr116 | descriptive | saving the screen as a GIF |
| `SHOWPIC.C` | ovr150 | descriptive | full-screen pictures from BYT.ARK |
| `CUTS.C` | ovr108 | inferred (the `cuts_` and `cutsop_` prefixes) | the cutscene player |

`map/filenames.tsv` has the evidence for each name. seg003's modules were found from TLINK's padding and the relocation order (README, "Linking"); their routines have no names in the DOS build, so the comments give FM Towns' names where the FM Towns graphics library lines up with them by order or by job.

## The screen

The game runs the VGA in mode X: the BIOS's 320 by 200 mode 13h unchained (`SetVideoMode`, VIDMODE.ASM), so each byte address holds four pixels, one per plane, and the 256 KB of video memory hold two pages and spare room. A write chooses planes with the sequencer's map mask; a read chooses one plane with the graphics controller's read map; copies within video memory go four pixels at a time through the latches.

Every routine reaches a screen row through the row table Ytab (seg_370D:36AC). `_2AED` builds it from the highest row down, and the display start is set to the highest row's address, so **y counts up from the bottom of the screen**: row 0 is the bottom line. The clip window (3DF4 left, 3DF6 top, 3DF8 right, 3DFA bottom) follows the same rule. MODEX.ASM, SCALEBM.ASM, the frame buffer and `cPlaceFB` all agree.

Pages: drawing goes to the page not on screen; `grPageFlip` shows it and switches the row table to the other page; `grSoftPageFlip` switches without showing. Video memory past the two pages is handed out by a bump allocator (`seg003_0272_49AE`, GRLIBF.ASM), and what it leaves becomes VALLOC.ASM's pool, where LOADGR.C keeps the screen furniture's pictures and SPRITE.ASM saves what sprites cover. VIDMODE.ASM also supports a virtual screen wider or taller than 320 by 200 with a split line, scrolled with `vscreen_focus` by display start and pixel panning; the cutscene player uses it for wide scenes and LBACK backgrounds.

## How the library is entered

seg003 keeps its data in its own far segment, seg_370D, and its routines expect DS = ES = seg_370D and a stack in that segment. There are three ways in:

- **From C**, through GRCORE.ASM's far wrappers (`rectangle`, `string_to_screen`, `show`, `set_the_window` ...): each switches to the library's stack at 370D:4FA8, loads the C arguments into registers and calls the routine, usually through the jump table at `_5278` .. `_52EA`.
- **From seg004** (the 3D renderer), through GRMISC.ASM's far thunks: BP names the routine and GRDISP.ASM's dispatcher switches stacks and calls it.
- **From seg021**, through far entries in GRENTRY.ASM that C3DENTRY.ASM's `cRender`, `cFBtoScreen`, `cPlaceFB` and the rest call.

The symbol `seg_370D` is 370D:0008, so the far pointers GRCORE.ASM gives C (`seg_370D+X`) address offset X + 8; the code itself uses plain offsets from 370D:0000.

## Drawing primitives

Points, lines, rectangles and polygons do not write video memory themselves. They turn the shape into a list of spans (6 bytes: y, left x, right x, ended by a y with its top bit set) and jump through seg_370D:4112, the current span writer. `set_the_color` and `fbuf_setcolor` set both the colour and the writer from a pen number: a pen's mode picks solid, copy from the other page, save to or restore from video memory, XOR, or the same on both pages (VIDMODE.ASM lists them). So one line routine draws a line, erases one by copying back the hidden page, or saves what is under it, depending on the pen. The frame buffer has its own writers (GRENTRY.ASM), used when the 3D view draws into it.

Clipping is done before the spans are built: `clip_rect`, Cohen-Sutherland in `cline`, and a Sutherland-Hodgman clipper for polygons (`shclip`) that patches its own comparison opcodes for each edge.

Bitmaps (`show`, `fbshow`, `vcopy`, `vcopyfb`) are clipped as rectangles into 8-byte row records (y, left, right, source offset) and handed to a transfer routine (GRLIBL.ASM, GRENTRY.ASM): opaque or transparent (colour 0 skipped), from a linear bitmap, from video memory, or within the screen.

Text: a font (`DATA\FONT*.SYS`, gfx.h `struct FontInfo`) is loaded by GRFX.C into seg_370D and indexed by `setup_font`. A string is rasterised into a one-bit buffer, already shifted for its x position, then written to the screen four pixels at a time with map masks (GRLIBI.ASM). Shadowed text is drawn twice, the shadow one pixel right and down. Text is clipped whole: a string that does not fit the window is not drawn.

## Art

The game's pictures are .GR files (UW-Formats 3.2), loaded once at start-up by LOADGR.C's `load_all_gr` into EMS (16 KB pages, mapped into physical page 2 when needed) or, for the screen furniture, video memory. Each picture gets a slot in `grs_off`; an object's picture is found through `obj_tab`. GRSPIC.C turns the game's icon numbers into slots and draws them, decoding 4-bit pictures through one of the 16-colour auxiliary palettes (`ALLPALS.DAT`) with seg004's decoders (`cFrmtoRaw`). The 3D view reads the same tables from assembly (PGCACHE.ASM). Textures (.TR) are loaded per level into EMS. Critter animations (`CRIT\CR*.*`) are paged into EMS on demand by the 3D renderer.

Sprites (SPRITE.ASM) are the moving parts of the screen furniture: the flasks, the compass, the eyes. A change only queues the sprite; `update_sprites` restores what the queued sprites covered and redraws them in layer order.

## Palettes and colour

The palette is 256 entries kept in seg_370D (`palette`), loaded into the DAC by MODEX.ASM's `local_do_palette`. GRFX.C loads palettes from `DATA\PALS.DAT` (768 bytes each) and runs fades as time-stepped ramps. COLCYCLE.C rotates palette banks every 64 ticks for animated colours. XFER.DAT's five 256-entry tables (`cXfer`, SCALEBM.ASM) remap colours: translucent sprites (colours 0F9h up pick a table that remaps the pixel underneath), and `cLiteFB`. `lightabs` (PGCACHE.ASM) is the 16-row distance shading table used for the 3D view.

## Cutscenes

CUTS.C plays `CUTS\csNNN.nXX` files: `.n00` is a script and the rest are Deluxe Paint Animator LPF animations. The script is a list of records (frame, opcode, arguments) run as each frame is reached; the 28 opcodes cover text, palette fades, music, speech (`SOUND\BSPnn.VOC`, streamed through EMS), virtual-screen panning and backgrounds. Frames are LPF large pages read into EMS (reusing the level textures' pages, which are reloaded afterwards) and drawn either whole or as run/skip/dump deltas written straight into planar video memory (`draw_rsd`). CUTS.C's header has the detail. LPFDELTA.ASM decodes the same delta format into a linear buffer, but nothing calls it.

## Key structures

- `struct FontInfo`, `struct Bitmap`, `struct CutsState` and the font and palette enums (gfx.h).
- seg_370D (FD51), the library's data: the row tables, the clip window, the colours and pens, the span tables, the font. The module headers give the offsets each module uses; it is partly defined by the modules (`FD51` pieces) and partly taken from the EXE at link time.
- `grs_off`, `obj_tab`, `first_anim` and the other slot tables (view3d.h, LOADGR.C).

## Open questions

[FINDINGS.md](../FINDINGS.md) collects the open questions and likely bugs of every subsystem in one place.

- POLYFILL.ASM's wall mapper is never called in DOS (seg004's `mapper_rtn` only marks wall polygons for the vertical mapper); LPFDELTA.ASM and PLANECPY.ASM have no callers either, and PLANECPY.ASM has a slip in its column addressing. They are probably left from earlier versions.
- The plane-by-plane span copies `_2FD6` and `_303B` (VIDMODE.ASM) have not been traced to their pens; nor has the third word of each screen-mode record (mode 0 is 320 by 200, mode 1 320 by 400).
- What `_2977`'s split-screen virtual screen is used for besides the cutscenes.
- `_C10`'s call from `render_3d` on a stack in seg_370D (SYSLIBP.ASM) is a hook that does nothing in this build.
