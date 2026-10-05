# The level map: candidates for the findings page

Candidates from `src/map` for the project's findings page; [map.md](map.md) describes the subsystem.

## Possible bugs

### Floor heights 14 and 15 convert to z 0

- **What happens:** `hgt_val` has 0 for heights 14 and 15, between 0x340 (13) and 0x400 (16).
- **Where:** [map/MAP.C](../../src/map/MAP.C); read by `motion/PHYSICS.C`'s `player_setup` and the 3D view.
- **Evidence:** code reading; both games (UW2's findings list it). None of the shipped UW1 levels has an open tile at height 14 or 15 (checked in `LEV.ARK`).
- **Confidence:** possible; no effect with the shipped levels.

### A texture block of the wrong size is used anyway

- **What happens:** `Txm_Load` notes a block that is not 0x7A bytes (it prints "bad tmap ids size" through the debug printer and returns 0) but still copies the buffer, which is then partly whatever the stack held.
- **Where:** [map/TEXTMAPS.C](../../src/map/TEXTMAPS.C).
- **Confidence:** possible; the shipped blocks are all 0x7A bytes, and `GAMEWRAP.C` ignores the result.

## Findings about the data and the engine

- UW1's level block is 0x7C08 bytes and ends at the magic word; the animation overlays and the texture map are blocks of their own (`level.h`). `LEV.ARK` has five kinds of block for nine levels, of which the shipped file holds three.
- UW1's `TERRAIN.DAT` keeps a floor's class in bits 4-5 (0x10 water, 0x20 lava); the collision code shifts it accordingly. UW2 moved the classes to bits 6-7.

## Dead code

- `OverwriteAllTiles_ovr128_37` in `MAP.C` and `ovr131_1DD` in `TEXTMAPS.C` have no caller in the matched C.
