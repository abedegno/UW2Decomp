# The level map

The map subsystem keeps the level in memory and moves it to and from `LEV.ARK`: the 64 by 64 tile map with the object store behind it, the level's texture map, and the light level of the 3D view. The sources are in `src/map`; the declarations, `struct Tile`, the tile types, the terrain and texture constants and the special levels are in `src/include/map.h`, and the level block's layout and the `LEV.ARK` block numbers in `src/include/level.h`. The candidates for the findings page are in [map-findings.md](map-findings.md).

| File | Segment | What it does |
|---|---|---|
| [`MAP.C`](../../src/map/MAP.C) | ovr123 | allocating the level block (`mapdata`), reading and writing it as a block of `LEV.ARK`, with the animation overlays |
| [`MAPADDR.C`](../../src/map/MAPADDR.C) | seg035_3073 | `Map_GetAddr`, the bounds-checked way to a tile, and `CreateObj` |
| [`TEXTMAPS.C`](../../src/map/TEXTMAPS.C) | ovr131 | a level's wall, floor and door textures: the default set, loading and saving them, loading the bitmaps, the terrain types |
| [`LIGHTING.C`](../../src/map/LIGHTING.C) | ovr142 | the shading tables and light levels, and the mushroom effect |

The names are UW2's (FM Towns) where the routine is the same; `TEXTMAPS.C`'s data names were chosen to reproduce the EXE's layout.

## The level block

A level is one 0x7C08-byte block (`struct LevelBlock`, `LEVEL_SIZE`): the tiles from 0, 256 mobile object records (27 bytes) from 0x4000, 768 static ones (8 bytes) from 0x5B00, the two free lists, the active mobile list, three counts and the magic word 0x7577 at 0x7C06. `Map_Init` allocates it once; `OBJECTS.C`'s `Map_ObjFix` points the object store into it. `Map_Load` reads level n from block n - 1 and turns the stored counts back into the free-list and active-list pointers; a block without the magic word is fatal. `Map_Save` stores the counts and writes it back. Both take an open archive (`struct Arc`, `GAMEWRAP.C`'s) and then do the level's animation overlays (`obj/EFFECT.C`).

Unlike UW2's, UW1's level block stops at the magic word: the animation overlays are a block of their own. `LEV.ARK` has 135 blocks, five kinds for each of the nine levels (`level.h`'s `LEVARK_*`): the map (0 to 8), the animation overlays (9 to 17, 384 bytes), the texture map (18 to 26, 0x7A bytes), the automap (27 to 35) and the map notes (36 to 44). The shipped file holds only the first three kinds; the automap and notes blocks are written when the game is saved (`ui/AUTOMAP.C`).

A tile (`struct Tile`, 4 bytes, UW-Formats 4.2) has a type (solid, open, four diagonals, four slopes: `enum TileType`), a floor height 0 to 15, a light bit, the floor texture index, the no-magic and door bits, and the head of its object list, whose low six bits are the wall texture. `Map_GetAddr(x, y)` returns a null pointer off the map; `MAP.C`'s `hgt_val` turns a floor height into z.

## Textures and terrain

A level has 48 wall textures (`TxmID`), 10 floor textures (`floor_IDs`) and 6 door textures (`ActDoors`), read from its texture block by `Txm_Load`. The first 12 walls and all floors are loaded into conventional memory from `W64.TR`, `W16.TR`, `F32.TR` and `F16.TR` (`load_tr_mem`); the rest of the walls go to EMS (`gfx/LOADGR.C`). `init_txtlib` fills the 3D view's tables of where each texture is.

Each texture's terrain word comes from `DATA\TERRAIN.DAT` (walls from 0, floors from 0x200): UW-Formats 4.8's list, named in `map.h` as `TERR_*`. Floors use 0x10 water and 0x20 lava; walls mark ankhs, stairs, pipes, gratings, drains, the chained-up princess, windows, tapestries and the textured door. The collision code takes a floor's word shifted right 4 as its terrain class (1 water, 2 lava; `TERRAIN_*`). UW2's file keeps its classes in bits 6-7 instead.

## Light

`set_light` picks one of eight light levels from `DATA\SHADES.DAT` (six shading parameters each, including the vision radius `curvrad`), swapping in `MONO.DAT`'s grey maps for level 5; `FixPlayerEquips` calls it with the brightest light carried or spell in force (6 under Wizard Eye). `random_light`, one of the three mushroom effects, replaces the 16 shading maps with 4 KB of low memory.

## Special levels

`map.h` names level 7, Tybal's lair (no mana while his orb stands, his guards, the maze spell's floor), and level 9, the Ethereal Void (no automap, no sleep, nothing dropped is kept, no silver tree). Level 8's centre is where the talismans are thrown into the lava (`motion/OBJPHYS.C`).

## Open questions

- `OverwriteAllTiles_ovr128_37` has no caller; it was probably an editor routine.
- `ovr131_1DD`, which draws picture `n + 58` at 16 by 16, has no caller in the matched C; what it was for is not known.
