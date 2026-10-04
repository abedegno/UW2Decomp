# Critters: AI, path finding and the critter table

This page describes how UW1 moves and runs its critters: the per-frame AI, the goals, path finding, what happens to critters between moments (sleep, level changes, theft), the creature table and the critter art cache. The sources are in `src/critter`; the declarations, the goal, attitude and animation names and the path records are in `src/include/critter.h`. The candidates for the findings page are in [critter-findings.md](critter-findings.md).

## Files

| File | Segment | What it does |
|---|---|---|
| [`AI.C`](../../src/critter/AI.C) | seg007_1798 | `move_mobile` (the per-frame update of every mobile object), `critter_ai`, the goals, perception, fleeing, damage to critters |
| [`PATHFIND.C`](../../src/critter/PATHFIND.C) | seg006_1477 | the per-critter globals, `crit_head_for_loc`, `beeline`, `flood_path` and the 16 stored paths, `line_of_sight`, doors, the critters' collision handlers, `move_me_joe` for other mobile objects, corpses |
| [`CRITTIME.C`](../../src/critter/CRITTIME.C) | ovr104 | goals set from outside the AI, the yearly checkup, time passing for a level while the player sleeps, wandering monsters, the last critter hit, theft noticed by owners |
| [`CREATURE.C`](../../src/critter/CREATURE.C) | ovr101 | `Creature[64]`, the critter table of `DATA\OBJECTS.DAT`, and a new critter's starting state |
| [`CRPAGES.C`](../../src/critter/CRPAGES.C) | ovr113 | the EMS cache of critter art pages (`CRIT\CRxxPAGE.Nyy`), and lending two pages to hold the conventional workspace |

UW1 has no build with symbols. The names are UW2's (the FM Towns originals) where the routine is the same one, else the listing's or ours, as each source says. The file names are UW2Decomp's.

## The update

`move_mobile` (`AI.C`) runs every frame from `motion/PLAYMOVE.C`'s `move_physics`. It advances a 16-step bin clock (`curBin`) and runs every active mobile object whose bin (the low nibble of `b0A`) has come round: critters through `critter_ai`, everything else (missiles, thrown objects) through `PATHFIND.C`'s `move_me_joe`. An object is due while its bin is up to 4 behind `curBin`, and each update moves its bin on by its rate, so an object with a small rate is updated several times in a frame and one with a large rate once or not at all (`timetodo`). A routine that removed its object returns 0 and the loop looks at the same slot again.

`critter_ai` loads the per-critter globals (`meptr`, `mycst`, the tile, fine position and home; they are `PATHFIND.C`'s), skips a critter more than 10 tiles from both the view and the player for half a cycle (unless its goal is `GOAL_FOLLOW`), runs its motion through the physics engine (`get_phys_data`, `do_crit_phys`, `set_phys_data`), and then acts by animation sequence: a dying critter is removed at its last frame and leaves its inventory and corpse; an attack sequence lands its blow on frame 4 (`COMBAT.C`'s `critter_attack`, with a charge from `atk_charge`); the casting and firing sequences cast or fire on frame 4. Anything else goes to `critter_mv`, which reacts to being hit, joins fights and runs the goal.

The physics record and handler depend on how the critter moves: `CN1`/`CT1` walkers, `CN2`/`CT2` fliers, `CN4`/`CT4` swimmers (`MOTION.C` owns them; `init_ai` sets the handlers up).

## Goals, attitudes and animations

A critter's goal is the low nibble of its goal word (`OBJ_GOAL`), with a target (`OBJ_GTARG`, a mobile index, 1 the player). `critter.h` names them from what `critter_mv` runs for each: stand (0 and 7), go home (1), wander (2), follow (3, UW1 only), guard (4), attack (5), flee (6), mill (8), cornered (9), talk (10), flutter (11) and hover (12). A guarding critter remembers guarding in its old goal while another goal runs, and goes back to it when that goal is dropped (`critter_discard_goal`); otherwise it wanders.

The attitude (bits 14-15 of the attitude word) is UW-Formats' hostile, upset, mellow, friendly. A new critter is mellow and mills (`creature_obj_init`).

The animation sequence (`OBJ_SEQ`) is written out with fixed numbers in UW1 (UW2 looks the lengths up): 0x20 standing, 0x2C walking, 0 the combat stance, 1 to 3 the melee attacks, 5 firing, 7 backing away, 0xC dying, 0xD casting. Each has four frames.

## Perception and fighting

- `target_found`: a critter hears its target within its hearing times the target's noise over 16 tiles (a quarter of that squared distance counts as certain), and sees it within its sight times the target's visibility over 16, within 45 degrees of its facing and in line of sight. The player's noise and visibility are set each round by `motion/PLAYMOVE.C`'s `set_sound` from speed and the stealth skill.
- `critter_mv`: when the player hits a critter that is not a loner, its race, place and time are recorded (`crithit`, `typehit`, `hitx`, `hity`, `crithittime`); for 0x200 game clock units, every critter of that race (or every ally of the player) within its hearing of the place turns hostile and attacks. A critter that was hit attacks its attacker, or flees if `should_i_flee` says so.
- `should_i_flee`: never above three quarters of its average hit points, never below an eighth; it flees if the damage since it last decided is over half its average hit points, otherwise by a roll against its nerve (`b1C_0`).
- `crit_offense`: in reach it fights hand to hand (`crit_attack`, which backs off, closes in or dodges and starts one of three attacks by the creature's attack probabilities); out of reach a caster casts and a critter with a missile weapon shoots. In Tybal's lair (level 7), while his orb is whole, his guards (race 0x13) close right in and do not cast.
- `damage_critter` picks the combat music: `MUSIC_FOE_HURT` when the player's foe is below a quarter of its hit points, `MUSIC_DANGER` when the player is, else `MUSIC_COMBAT`.

## Moving to a square

`crit_head_for_loc` (`PATHFIND.C`) chooses how to get there: follow the critter's stored path; walk straight when `beeline` finds the tiles on the line walkable without a jump; otherwise search with `flood_path` (breadth first over at most 32 steps, in a box 5 tiles beyond the start and the destination, with `hyp_move` as the step test) and store the path in one of 16 shared slots (`paths`, free mask `freepaths`); or give up and wander for a while. `hyp_move` checks the tile walls between squares (`tile_walls`), closed doors across the move, floor and object heights (one height step up at most; a slope counts one higher unless climbed the right way) and the floor's terrain against the handler's `noclimb` (forbidden) and `w6` (costly) words. A path may drop to a lower floor if the drops' danger stays within `acceptable_danger` (0 for any critter that is not hostile).

A walker that bumps into a closed door tries to open it (`try_to_open_door`) when it is hostile, three times in four: a critter with a locks value uses the door and may try the lock, otherwise it bashes it one time in four. A non-hostile critter never opens a door it bumps into (UW1 only).

## Between moments

`CRITTIME.C`:

- `update_all_critters_whilst_player_snoozes` (sleep, and leaving a level): summoned critters vanish; the others heal half way to their average hit points, turn a random way and go home if there is room; mobile objects come to rest. Then each race's attitude moves by its members' net mood: every critter not a loner shifts by the count of friendly less hostile members of its race, clamped to hostile..friendly.
- `wandering_monster_check`: half the hostile critters within 8 tiles may walk to the sleeping player along a safe path, triggering ward traps on the way, and stop two squares short.
- `player_grabbed`: taking an owned object angers up to 20 critters of the owning race within 7 tiles who see it, one step each, with a message. Owner 0x0D does not care once quest 32 (the Knight of the Crux) has reached 3, the writ of Lorne found.
- `set_creatures_to_saved_game` and `..._from_...` keep the record of the last critter hit in the player record across saves.

## The critter table and art

`Creature[64]` (`struct Creature`, 48 bytes, `critter.h`) is read from `DATA\OBJECTS.DAT` after the weapon, missile and armour tables (offset 0x132). Entry 63 is the player's own (`playerdat`). `CRPAGES.C` keeps the critter animation pages in EMS, two 16K pages each, evicting the least recently used; `preload_cr` reads the frame counts from each page file's header and `CRIT\ASSOC.ANM`.

## Open questions

- Which races 0x0D and 0x13 are: the code shows only that 0x0D's theft check follows the Knight of the Crux quest and that 0x13 guards Tybal's lair.
- `GOAL_FLUTTER` (11) drifts at random and ignores blows; which critters have it was not traced. The UnderworldGodot port notes it only on Ethereal Void creatures.
- `Creature`'s unnamed fields (`bA_1` ignores fights, `bA_5` can jump, `b0F` the poison of its blow, `b2D_0` keeps its distance) are named by use only.
