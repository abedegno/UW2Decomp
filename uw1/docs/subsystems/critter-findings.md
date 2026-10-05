# Critters: candidates for the findings page

Candidates from `src/critter` for the project's findings page; [critter.md](critter.md) describes the subsystem. Each bug candidate was re-read in the matched source; what the code does is read from the code, and the effect in the game is an inference unless it says it was checked. "Both games" means UW2Decomp's matched source has the same code.

## Likely bugs

### A dying critter's cry is chosen by the wrong critter

- **What happens:** `crit_die` decides whether to play the death sound (effect 6) by `mycst->death` and plays it at `myxpost`, `myypost`: the creature type and position of whichever critter the AI processed last, not the critter that is dying. When the player kills a critter, `mycst` is left over from the last AI step.
- **Where:** `crit_die` in [critter/AI.C](../../src/critter/AI.C).
- **Evidence:** code reading. UW2's `crit_die` uses the victim's creature type (`victim->death`), which shows what was meant.
- **Confidence:** likely.
- **Effect:** a death cry can be missing, or come from another critter's position. Not checked in the game.
- **For a port:** matching DOS means reading the stale `mycst`.

### A critter placed in a north-west diagonal tile goes to the wrong corner

- **What happens:** `set_gridx_and_y_based_on_tile_type` gives `TILE_DIAG_NW` the fine position (6, 1), the same as `TILE_DIAG_SE`, where its open corner is (1, 6).
- **Where:** [critter/CRITTIME.C](../../src/critter/CRITTIME.C); used by `up_crit` (going home during sleep) and `wander_that_monster`.
- **Evidence:** code reading; both games (UW2's findings list it).
- **Confidence:** likely.
- **Effect:** the critter is aimed at the closed half of the tile, so `can_place` probably refuses and it stays where it was.

### Flameproof fliers and swimmers leave lava marked in the shared handlers

- **What happens:** for a creature whose `ComObjData` resist has 8 (flameproof), `critter_ai` clears 0x20 (a footprint corner on lava) from its handler's `mask` and `w6` and sets it in `ignore` for its step, then sets 0x20 in `mask` and `w6` and clears it from `ignore`. `init_ai` gave `CT2` (fliers) neither bit and `CT4` (swimmers) no `w6` bit, so after the first flameproof flier or swimmer moves, those shared handlers keep 0x20.
- **Where:** `critter_ai` in [critter/AI.C](../../src/critter/AI.C); `init_ai` in [critter/PATHFIND.C](../../src/critter/PATHFIND.C).
- **Evidence:** code reading; both games.
- **Confidence:** likely (that the restore was meant to undo the change exactly).
- **Effect:** for every flier and swimmer afterwards, `flood_path` counts a lava floor as danger 2 (`w6` is its cost mask), and `crit_hndlr_fly` is also called for lava corners. Probably small; not checked.

## Possible bugs

### The path search box reaches one past the map

- **What happens:** `flood_path` clamps the low ends of its search box to 1 but the high ends to `MAP_SIZE` (64), so it can step to x or y 64, where `Map_GetAddr` returns a null pointer and `STILES` indexes past the 64 by 64 records.
- **Where:** `flood_path` in [critter/PATHFIND.C](../../src/critter/PATHFIND.C).
- **Evidence:** code reading. The shipped `LEV.ARK` has no open tile on any level's edge row or column, so a search never stands on square 63 to step further.
- **Confidence:** possible; no effect with the shipped levels.

### swap_ws_out tests the wrong thing for failure

- **What happens:** `swap_ws_out` ignores `ovr113_2A2`'s -1 and tests the returned pages for 0xFF, which `ovr113_2A2` never stores; with fewer than two critter pages the second entry would be uninitialised.
- **Where:** [critter/CRPAGES.C](../../src/critter/CRPAGES.C).
- **Confidence:** possible; `crit_nlpages` is always far above two, so it cannot fail in practice.

## Candidates that did not hold up

- **A door bashed for `rand() % 0` damage.** `try_to_open_door` divides by the creature's first attack damage. Only the mongbat (item 0x51) among real creatures has damage 0 in `OBJECTS.DAT`, and it flies: `crit_hndlr_fly` never reports a door, so it never gets there.

## Game rules recovered

- Critters join a fight: for 0x200 game clock units after the player hits a critter that is not a loner, every critter of the same race within its hearing of the spot turns hostile and attacks (`critter_mv`).
- When a level's time passes (sleep, leaving the level), each race's attitude drifts towards its members' majority mood (`update_all_critters_whilst_player_snoozes`).
- A hostile critter may walk up to the sleeping player, half the hostile critters within 8 tiles being tried, along a path without danger (`wandering_monster_check`).
- Taking an owned object angers its owners who see it; one race (owner 0x0D) stops caring once quest 32, the Knight of the Crux, reaches 3 (the writ of Lorne found).
- Tybal's guards (race 0x13) on level 7 close right in and never cast while his orb is whole; the critter with conversation 0x16 on level 6 accepts no danger on its paths.
- Critters that are not hostile never open a door they bump into.

## Dead code

- `creature_save_ovr101_17` (writing the critter table back) has no caller.
- `crit_hndlr_obj`, `CT3`'s special function, is never called: `CT3.mask` is 0.

## Open questions

- Races 0x0D and 0x13 and goal 11 (see [critter.md](critter.md)).
