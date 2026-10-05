# The map and levels

This page describes how UW2 holds a level in memory and moves it to and from `LEV.ARK`: the tile map, the level's texture map and terrain types, and the light level of the view. The sources are in `src/map`; the declarations are in `src/include/map.h` and `src/include/level.h`. Collision with the terrain is in `src/motion/COLLIDE.C` (see the motion page), although its structures are declared in `map.h`. Statements marked "probably" are inferences; the reason is given with each.

## Files

| File | Segment | Name | What it does |
|---|---|---|---|
| `MAP.C` | ovr128 | inferred (Map_Init, Map_Load, Map_Save are FM Towns names; System Shock's `MAP.C` has `map_init`) | allocates the level block, loads and saves it, and the animation and timer lists inside it |
| `MAPADDR.C` | seg036 | descriptive | `Map_GetAddr`, the bounds-checked tile lookup, and `CreateObj`, which makes a new object |
| `TEXTMAPS.C` | ovr140 | inferred (the FM Towns names; System Shock's `TEXTMAPS.C` does the same job) | the level's texture map, door textures and terrain types |
| `LIGHTING.C` | ovr152 | descriptive | the light maps and the light level (`set_light`) |

Function and global names are the FM Towns originals where that build has them. `map/filenames.tsv` has the evidence for each file name.

## The level block

A level in memory is one 0x7E08-byte far block, `mapdata`, allocated once by `Map_Init` and laid out as `struct LevelBlock` (`level.h`):

| Offset | Contents |
|---|---|
| 0000 | 64 by 64 `struct Tile`, 4 bytes each |
| 4000 | 256 mobile object records (`struct Object`, 27 bytes) |
| 5B00 | 768 static object records (`struct StaticObj`, 8 bytes) |
| 7300 | the free list of mobile objects (objects 2 to 0xFF) |
| 74FC | the free list of static objects |
| 7AFC | the active mobile objects |
| 7C00 | three counts: active, mobile free, static free |
| 7C06 | 0x7577, "uw" |
| 7C08 | 64 animation overlays (`struct Anim`, 6 bytes) |
| 7D88 | 64 timer words |

The part up to 7C06 is UW-Formats 4.1's level block; the animation and timer parts are what `Anim_Load` and `Anim_Save` copy. `Map_ObjFix` (`obj/OBJECTS.C`) points the object store's globals (`critdata`, `objdata`, `critbot`, `objbot`, `ActiveMob` ...) into the block, so the object lists live in the level block itself and saving a level saves its objects with it.

`LEV.ARK` (read and written by `sys/ARC.C`, which decompresses and compresses blocks per their flags) has four blocks a level for 80 levels: the map block at `level - 1`, the texture block at `0x4F + level` (80 to 159), then the automap and the map notes (UW-Formats 4.1), which `ui/AUTOMAP.C` handles. `game/GAMEWRAP.C`'s `GetLevel` opens the archive once and reads the three it needs (`Map_Load`, `Txm_Load`, `GetAutoMapLevel`) before closing it; `SaveLevel` writes them back the same way. `Map_Load` and `Map_Save` can use `DATA\LEV.ARK` or the one in the save directory (`HomeDir`); `GetLevel` and `SaveLevel` always ask for the save directory's.

`Map_Load` turns the stored counts back into pointers: `critptr` and `objptr` point at the top entry of each free list, so each stored count is the number of free entries less one (as UW-Formats says), and `LastActiveMob` is `ActiveMob` plus the active count. A block without the magic word is fatal, "Underworld can no longer run. Error code A003." (`pfatal_code(3)`). `Map_Save` stores the counts, checks the object lists with `ObjCrunch` (which also zeroes the free records) and asks the player whether to go on if the check fails. The animation and timer lists are stored without counts; `Anim_Load` counts entries up to the first empty one.

## Tiles

`struct Tile` (`map.h`) is UW-Formats 4.2's tile: type (solid, open, four diagonals, four slopes; `enum TileType`), floor height, a light flag, the floor texture index, a no-magic bit and a door bit, and a word whose top ten bits head the tile's object list and whose low six bits are the wall texture index (`TILE_WALL`). The map's index is `x + y * 64`; `Map_GetAddr` returns a null pointer outside 0 to 63. `hgt_val` (`MAP.C`) converts a floor height to the z the physics and the renderer use, 0x40 a step.

## Textures and terrain

A tile's floor and wall indices go through the level's texture map, `TxmID` (64 texture numbers into `T64.TR`; in UW2 floors and walls share one table, UW-Formats 4.4), and the six door textures `ActDoors`. `Txm_Load` reads them from the level's 0x86-byte texture block and loads the textures (`load_txtmaps`: `load_tr_ems("t64")` and `load_doors`, in `gfx/LOADGR.C`). `Load_Terrains` reads each texture's word from `DATA\TERRAIN.DAT` into `TxmTerr`, whose top two bits are the terrain class (water, lava, ice; `map.h`) that movement, combat, fishing, path finding, the automap and the renderer test.

## Light

The renderer shades through 16 colour maps of 256 bytes, `cLightTabs`, filled from `DATA\LIGHT.DAT` (`init_lighting`, which also loads `DATA\XFER.DAT` into `cXfer`, the five colour translation tables in `gfx/SCALEBM.ASM`). `set_light(level)` reads row `level` of `DATA\SHADES.DAT`, eight rows of six words, into the renderer's shading parameters (`smooth_div`, `smooth_base`, `smooth_lowpass`) and the view's reach (`curvrad`, the vision grid radius, 3 to 7 from darkest to brightest, and `distpoly`, `dist8`); level 5 also swaps in `DATA\MONO.DAT`'s grey maps. The game calls it as the player's light source changes and as the player crosses tiles whose light flag differs (`motion/PHYSICS.C`). `random_light` is one of the mushroom effects (`game/PLAYDATA.C`'s `set_drugged`): it fills the light maps with 4 KB of low memory from segment 0, which the FM Towns build does with the start of its own data segment.

## Creating objects

`CreateObj(item, mobile)` takes a record from the mobile or static free list (`Obj_Alloc`) and gives it the defaults: quality 40, fine position 3,3, z 0, heading 0, no flags, no owner, no next object, and a quantity of 1 when the item's `ComObj` stack field is 0 or 2. A creature is then set up by `init_this_critter`. Placing it in a tile is the caller's job.

## Open questions

[FINDINGS.md](../FINDINGS.md) collects the open questions and likely bugs of every subsystem in one place.

- Why level 5 is the grey (`MONO.DAT`) level, and what sets it: UW-Formats says the mono maps are for invisibility; the callers pass computed levels, which have not been traced.
- `hgt_val` has zeros for heights 14 and 15 and 0x400 at index 16. Whether heights 14 and 15 occur in UW2's levels is not checked.
- `OverwriteAllTiles_ovr128_37` (fill the map with one tile) and `ovr140_4F` (draw a texture at 16 by 16) have no callers and no FM Towns names; probably leftovers of the level editor (the program was linked as `uwedit.exe`).
- `Txm_Load` opens only the save directory's archive, while `Map_Load` can open `DATA\`'s; `GetLevel` keeps the map's archive open for it, so the difference matters only to another caller, and none has been looked for.
- `Map_Save` asks a yes/no question (string 0x96) when the object lists fail their check; the string has not been looked up.
