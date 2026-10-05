# Graphics, art and cutscenes

This page describes how UW1 puts pictures on the screen: the graphics library (DOS segment seg003, 18 assembly modules in `src/gfx`), the smaller assembly modules around it (`SPRITE.ASM`, `VALLOC.ASM`, `VSTATS.ASM`, `LPFDELTA.ASM`, `MODEX.ASM`, `PLANECPY.ASM`), and the C that loads art, manages palettes, saves screenshots and plays cutscenes. The declarations, the icon numbers, the palette numbers and the screen furniture's element numbers are in `src/include/gfx.h`. The 3D view is drawn by seg004 into a frame buffer that this library copies to the screen; [3d.md](3d.md) describes that side. The candidates for the findings page are in [gfx-findings.md](gfx-findings.md).

## Files

| File | Segment | What it does |
|---|---|---|
| `GRMISC.ASM` | seg003 module 1 | far thunks for seg004's points and lines, the frame-timing hook, the retrace wait |
| `WALLMAP.ASM` | seg003 module 2 | the older wall texture mapper (UW1 only), seg004's floor-style mapper |
| `MAPDATA.ASM` | seg003 module 3 | words POLYFILL fills as it sets up a polygon; nothing reads them |
| `POLYFILL.ASM` | seg003 module 4 | the wall texture mapper, FM Towns' `wmap_bitmap`; seg004's wall-style mapper |
| `QUADFIT.ASM` | seg003 module 5 | the coefficients of a quadratic through three values; nothing calls it |
| `GRENTRY.ASM` | seg003 module 6 | the 3D frame buffer: clear, light, copy to the screen, its span writers and transfers |
| `SCALEBM.ASM` | seg003 module 7 | scaled bitmaps (the 3D view's sprites), code generated per sprite; the XFER tables |
| `VIDMODE.ASM` | seg003 module 8 | mode X, the row table, pages, the palette, the screen span writers, bitmap transfers |
| `GRLIBF.ASM` | seg003 module 9 | points, lines, boxes, rectangles, the polygon clipper, video memory allocation |
| `GRLIBG.ASM` | seg003 module 10 | `cline`, a clipped line |
| `GRLIBH.ASM` | seg003 module 11 | `uline`, an unclipped line |
| `GRLIBI.ASM` | seg003 module 12 | filled polygons, text |
| `GRCORE.ASM` | seg003 module 13 | the C entry points |
| `GRJUMPS.ASM` | seg003 module 14 | the near jump table (UW1: a module of its own) |
| `GRDISP.ASM` | seg003 module 15 | the dispatcher the thunks jump to |
| `GRLIBL.ASM` | seg003 module 16 | bitmap row transfers |
| `GRLIBM.ASM` | seg003 module 17 | the Gouraud (smooth) polygon |
| `GRLIBN.ASM` | seg003 module 18 | the clipper for smooth polygons |
| `SPRITE.ASM` | seg000 | sprites: pictures redrawn in place over the screen, in layers |
| `VALLOC.ASM` | seg001 | spare video memory blocks; saving and restoring screen rectangles |
| `VSTATS.ASM` | seg001 | `valloc_stats`, a report on that memory (UW1 only; nothing calls it) |
| `LPFDELTA.ASM` | seg002 | the Deluxe Paint run/skip/dump decoder (UW1's cutscene player uses it) |
| `MODEX.ASM` | seg015_1F9B | mode X screen access, the palette loader, far string helpers |
| `PLANECPY.ASM` | seg018 | copies a 4 by 4 block to the screen; nothing calls it |
| `GRSPIC.C` | seg009 | icon numbers to pictures; drawing them on the screen or into the frame buffer |
| `COLCYCLE.C` | seg021 | palette colour cycling |
| `GRFX.C` | ovr114 | graphics start-up, fonts, palettes, fades, 3D view dissolves |
| `LOADGR.C` | ovr115 | loading .GR art into EMS or video memory, and .TR textures into EMS |
| `SCRSHOT.C` | ovr112 | saving the screen as a GIF |
| `SHOWPIC.C` | ovr141 | full-screen .BYT pictures |
| `CUTS.C` | ovr105 | the cutscene player |

seg003's modules were found from TLINK's padding and the relocation order (docs/NOTES.md, "seg003, the graphics library"). UW1 has four modules UW2 lacks (WALLMAP, MAPDATA, QUADFIT, GRJUMPS: pieces UW2 folded into other modules or dropped) and no virtual or split screen in VIDMODE. The routines have no names in the DOS build; the comments give FM Towns' names where its graphics library lines up by order or job.

## The screen

The game runs the VGA in mode X (mode 13h unchained), so each byte address holds four pixels, one per plane, and video memory holds two pages and spare room. Every routine reaches a screen row through the row table `Ytab` (seg048:36AA, equate `YTAB`), built from the highest row down, so **y counts up from the bottom of the screen**. The clip window (`WIN_LEFT` 3DF2, `WIN_TOP` 3DF4, `WIN_RIGHT` 3DF6, `WIN_BOTTOM` 3DF8) follows the same rule.

Drawing goes to the page not on screen; `grPageFlip` shows it and switches the row table, `grSoftPageFlip` switches without showing. Video memory past the two pages is handed out by a bump allocator (GRLIBF.ASM), and what it leaves becomes VALLOC.ASM's pool, where LOADGR.C keeps the screen furniture's pictures and SPRITE.ASM saves what sprites cover.

## How the library is entered

seg003 keeps its data in its own far segment, seg048 (paragraph 3963; UW2's seg_370D), and its routines expect DS = ES = SS = seg048. There are three ways in: from C through GRCORE.ASM's far wrappers, which switch to the library's stack at 3963:4FA6 and call the routine, usually through GRJUMPS.ASM's jump table; from seg004 through GRMISC.ASM's far thunks and GRDISP.ASM's dispatcher (BP names the routine); and from seg019 through far entries in GRENTRY.ASM that C3DENTRY.ASM's `cRender`, `cFBtoScreen` and `cPlaceFB` call. The equates at the top of each module name the data it uses (`SPAN_WRITER`, `PEN_COLOR`, `VERTS` and so on); GRLIBF.ASM's and VIDMODE.ASM's headers map the segment.

## Drawing primitives

Points, lines, rectangles and polygons turn their shape into a list of spans (y, left x, right x) and jump through `SPAN_WRITER` (seg048:4110), the current span writer, which `set_the_color` chooses from a pen number: solid, copy from the other page, save to or restore from video memory, XOR, or the same on both pages. Clipping comes first: `clip_rect`, Cohen-Sutherland in `cline`, and a Sutherland-Hodgman clipper (`shclip`) that patches its own comparison opcodes for each edge. Bitmaps (`show`, `fbshow`, `vcopy`, `vcopyfb`) are clipped into 8-byte row records and handed to the transfer in `XFER_RTN` (seg048:0DC4): opaque or transparent (colour 0 skipped when `Transparency` is set), from a linear bitmap, from video memory, or within the screen.

Text: a font (`DATA\FONT*.SYS`, gfx.h `struct FontInfo`) is loaded by GRFX.C's `grfx_load_font`, which UW1 calls with the file name (UW2 with an index), and taken up by `setup_font`. A string is rasterised into a one-bit buffer and written four pixels at a time with map masks (GRLIBI.ASM, the masks at `TEXT_MASKS`).

## Art

The game's pictures are .GR files, loaded once at start-up by LOADGR.C's `load_all_gr` into EMS (16 KB pages, mapped into physical page 2 when needed) or, for the screen furniture, video memory. Each picture gets a slot in `grs_off`; an object's picture is found through `obj_tab`. The game names a picture by an icon number (gfx.h's `ICON_*`): below 1000h an object, from 1000h the screen art in EMS (buttons, cursors, 3dwin), from 2000h the pictures in video memory (lfti, flasks, compass, dragons, inv, power, eyes, chains, spells, scrledge, optb), each file's pictures following the previous file's. GRSPIC.C turns icon numbers into slots and draws them, expanding 4-bit pictures through one of the 16-colour palettes of `DATA\ALLPALS.DAT` with seg004's decoders (`cFrmtoRaw`). Level textures (.TR) are loaded per level into EMS (`load_tr_ems`); UW1 keeps each texture's EMS page and segment in two tables in seg051 (`seg009_38C` reads them), where UW2 computes them.

Sprites (SPRITE.ASM) are the moving parts of the screen furniture. A change only queues the sprite; `update_sprites` restores what the queued sprites covered and redraws them in layer order.

## Palettes and colour

The palette is 256 entries kept in seg048 (`palette`), loaded into the DAC by MODEX.ASM's `local_do_palette`. GRFX.C loads palettes from `DATA\PALS.DAT` (768 bytes each; gfx.h's `PAL_*` name the ones the code uses, after UW-Formats' file list) and runs fades as time-stepped ramps. COLCYCLE.C rotates palette banks (UW1: colours 30h to 3Fh in four banks of four, and the runs 10h to 14h and 15h to 17h) every 64 ticks. XFER.DAT's colour tables (`cXfer`, SCALEBM.ASM) remap colours for translucent sprites and lighting.

## Full-screen pictures and screenshots

UW1 has no BYT.ARK: its full-screen pictures are separate `DATA\*.BYT` files of 0xFA00 bytes, shown by SHOWPIC.C's `LoadBitMap_ovr141_0` (and MAINMENU.C and AUTOMAP.C, which read theirs with `bltfromdrive`). Alt+Q saves the screen as `UWPICnnn.GIF` (SCRSHOT.C, the same code as UW2's).

## Cutscenes

CUTS.C plays `CUTS\csNNN.nXX` files: `.n00` is a script and the rest are Deluxe Paint Animator LPF animations. Cutscene numbers below 100h fill the screen (the introduction, the dreams, the ending), from 100h they play in the 3D view's window. The script is a list of records (frame, opcode, arguments); UW1 has 16 opcodes (txt, erase, func, pause, skip, next, end, loop, data, fadeout, fadein, jump, punt, say, wait, clang). Frames are LPF large pages read into EMS through three 64 KB windows; a frame is a whole picture or a run/skip/dump delta, decoded by LPFDELTA.ASM's `seg002_A` into a linear buffer and drawn with `show` (UW2 decodes straight into planar memory and leaves LPFDELTA unused). There is no virtual screen, panning, background or music opcode in UW1.

## Key structures

- `struct FontInfo`, `struct Bitmap`, `struct CutsState` (UW1's is 0x4A bytes) and the icon, palette and element numbers (gfx.h).
- seg048 (FD52), the library's data: the row tables, the clip window, the colours and pens, the span tables, the font. It is partly defined by the modules (`FD52` pieces) and partly taken from the EXE at link time (docs/LAYOUT.md).
- `grs_off`, `obj_tab`, `first_anim` and the other slot tables (view3d.h, LOADGR.C).

## Open questions

- QUADFIT.ASM's quadratic, MAPDATA.ASM's words and PLANECPY.ASM's block copy have no callers; what they served is unknown.
- The plane-by-plane span copies in VIDMODE.ASM and the third word of each screen-mode record have not been traced (as in UW2).
- `_C10`, the call from `render_3d` into seg019's SYSLIBP.ASM on the library's stack, does nothing in this build.
