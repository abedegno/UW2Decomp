# Critters and their AI

This page describes how UW2 moves and thinks for its critters (monsters and NPCs): the per-frame AI, goals, perception, combat choices, fleeing, path finding, what happens to critters while the player is away or asleep, and the critter art cache. The sources are in `src/critter`; the declarations are in `src/include/critter.h`, with `struct Object`'s critter fields in `object.h`.

## Files

| File | Segment | Name | What it does |
|---|---|---|---|
| [`AI.C`](../../src/critter/AI.C) | seg007 | inferred (System Shock's `AI.C`) | the per-frame AI: goals, perception, melee, spells and missiles, fleeing, damage and death |
| [`PATHFIND.C`](../../src/critter/PATHFIND.C) | seg006 | inferred (System Shock's `PATHFIND.C`) | getting to a square: straight lines, flood-fill paths, doors, the physics handlers, homing missiles, corpses |
| [`CRITTIME.C`](../../src/critter/CRITTIME.C) | ovr107 | descriptive | critters between moments: goals set from outside, sleep and level re-entry, wandering monsters, theft, the arena, the castle NPCs' day |
| [`CREATURE.C`](../../src/critter/CREATURE.C) | ovr104 | inferred (the class prefix) | the creature table of `OBJECTS.DAT` and a new critter's starting state |
| [`CRPAGES.C`](../../src/critter/CRPAGES.C) | ovr117 | descriptive | the EMS cache of critter art |

Function and global names are the FM Towns originals where that build has them. `map/filenames.tsv` has the evidence for each file name.

## Data

A critter is a mobile object (`struct Object`, 27 bytes): besides the common fields it has hit points, a goal word (goal, goal target), an attitude word (attitude 0 hostile to 3 friendly, powerful, no-heal, temporary, has-inventory bits), its speed and heading, its animation sequence and frame, its time bin and the flag bytes `b19` (knows where its target is, ally, fed ...). `whoami` names an NPC with a conversation (0 for none). Its home in the AI's sense is kept in the quality and owner fields. `object.h` names the fields and has their accessors; several flag bits are named only by position.

Each kind of critter has a `struct Creature` (`critter.h`), 64 of them in `Creature[]` from `OBJECTS.DAT` (`Creature[63]` is the player): hit points (`avghit`), attributes, armour by hit location, attacks with damage, chance and probability, up to three spells and a caster value, senses (sight, hearing, noise, visibility), speed and range, nerve, race, loot (`TREASURE.C`) and what it leaves when it dies.

## The AI loop

`move_mobile` (`AI.C`, called each frame by the motion code while `MoveCrits` is set and time is not stopped) steps a 16-step bin clock and updates every mobile object whose bin has come due; each update advances its bin by its rate, so faster critters update more often. Critters go to `critter_ai`, other mobile objects (missiles, thrown objects) to `PATHFIND.C`'s `move_me_joe`. A critter more than 10 tiles from both the view and the player is updated half as often.

`critter_ai` runs the critter's motion through the physics engine (walkers, fliers and swimmers have their own collision handlers in `PATHFIND.C`), then plays out its sequence: an attack strikes on frame 3 (`combat/COMBAT.C`'s `critter_attack`), a cast or shot fires on frame 3 (`combat/SPELLS.C`'s `cast`, `combat/MISSILE.C`'s `critter_fire`), a dying critter is removed at its last frame, leaving its loot and corpse. Otherwise `critter_mv` decides what to do.

`critter_mv` first reacts: a critter whose kin the player has just hit, within its hearing, turns hostile and attacks; one that has been hit attacks its attacker, or flees if `should_i_flee` says so. Then it runs its goal:

| Goal | Behaviour |
|---|---|
| 0, 7 | stand (and watch for the player if hostile) |
| 1 | go home |
| 2 | wander (`crit_drunkwalk`) |
| 3 | nothing: moved by other code |
| 4 | guard: a hostile guard looks for the player and attacks what it perceives |
| 5 | attack (`crit_offense`) |
| 6 | flee (`crit_flee`) |
| 8 | mill about near home (`crit_mill`) |
| 9 | fight back when cornered (`crit_defense`) |
| 10 | wait to talk to the player (`crit_talk`) |
| 11 | flutter at random |
| 12 | hover at home (`crit_hover`) |
| 15 | frozen while the goal target counts down (paralysis) |

## Rules found in the code

- **Perception** (`target_found`). Hearing range is the critter's hearing times the target's noise over 16, sight range its sight times the target's visibility over 16, in tiles. The target is perceived within half the hearing range, or within sight range inside 45 degrees of the critter's facing and in line of sight; within twice the hearing range it is heard faintly. The player's noise rises when he swings a weapon (`COMBAT.C`).
- **Melee** (`crit_attack`). In reach, one time in four the critter starts a blow, choosing among its three attacks by their probabilities; otherwise it winds up a stronger blow, backs off, sidesteps, dodges (chance dexterity / 64) or closes in.
- **Ranged attacks.** A caster within 8 tiles, in sight and facing the target, casts with chance caster / 128 (its first spell 11 times in 16, else its second); its third spell is a defensive one, cast with chance caster / 256. A critter with a missile weapon shoots within 4 tiles with chance (dexterity + 1) / 192. Spells and missiles are aimed up or down by the height difference (`compute_trz_or_try_rather`).
- **Fleeing** (`should_i_flee`). Never above three quarters of its hit points or below an eighth; it flees when the damage since it last decided exceeds half its hit points, or when `hp * 16 / maxhp + rand() % 4` is at most 15 - nerve. A fleeing critter that is cornered or stuck turns and fights.
- **Joining a fight.** When the player hits a critter that is not a loner, the hit is recorded (race, place, time); for 0x200 clock units critters of that race within hearing of the place turn hostile.
- **Talking** (`crit_talk`). A critter with goal 10 starts the conversation itself when the player is within 1.5 tiles and facing it.
- **Paths** (`PATHFIND.C`). `crit_head_for_loc` walks straight at a square when the tiles between are walkable, else searches with `flood_path` (a breadth-first flood over at most 40 steps in a box 5 tiles around start and goal) and stores the path in one of 16 shared slots, else wanders. Walkers avoid water and treat lava as dangerous; swimmers keep off plain floor and lava. A path's danger (drops and bad terrain) must stay within `acceptable_danger`, which is 0 for a critter that is not hostile. A critter that bumps into a door tries to open it.
- **Damage and death** (`damage_critter`, `crit_die`). The kill is credited to whoever struck, or to the shooter of a missile; the player's kills give experience (`player_killed_a`). Combat music follows the fight: theme 2 when the player's foe is badly hurt, 4 when the player is, else 3. A critter with a conversation may refuse to die: `event/WORLDEV.C`'s `death_check` handles the plot's special deaths.
- **Time passing** (`update_all_critters_whilst_player_snoozes`). When the player sleeps or returns to a level, critters heal half way to full, go home, summoned critters vanish, loose mobile objects come to rest, and each race's attitude drifts by the net count of its friendly and hostile members, so a mostly hostile race grows more hostile.
- **Wandering monsters while asleep** (`wandering_monster_check`). Sleep is refused with hostile, alert critters within 2 tiles. During the night one hostile critter within 8 tiles and with a safe path may be moved to two squares from the sleeper, waking him.
- **Theft** (`player_grabbed`). Taking an owned object angers up to 20 critters of the owning race within 7 tiles who can see it: each grows one step less friendly and says so ("is angered", "is annoyed", "notes your action"). The object then belongs to nobody.
- **The castle's day** (`maybe_go_hang_out`, `where_shall_we_hang_out`). Lord British's household moves between places by two-hour stages of the day: their own spots at night, a common area in the afternoon, random squares otherwise; some stay put as the castle plot advances.
- **Critters periodically** (`yearly_checkup`, from `game/PLAYTIME.C`) get their fed bit rerolled and lose the ally bit three times in four.

## Critter art

Critter pictures are in `CRIT\CRxx.yy`, critter file xx (octal) split into fragments yy (UW-Formats 3.6.2). `CRPAGES.C` sets up an EMS cache of fragments that the renderer (`3d/PGCACHE.ASM`) fills on demand and ages every frame, and preloads `CRIT\AS.AN` (which file and palette each critter type uses) and `CRIT\CR.AN` (the animation sequences: 8 actions by 8 view angles; `AI.C`'s `set_cur_seq_len` reads their lengths). The 8 actions are the animation sequences `AI.C` uses: 0 standing, 1 walking, 2 combat stance, 3 to 5 the three attacks, 6 casting or firing, 7 dying.

## Open questions

[FINDINGS.md](../FINDINGS.md) collects the open questions and likely bugs of every subsystem in one place.

- Many `struct Creature` fields and critter flag bits are named by offset only (`bA_1`, `b0F`, `b2D_0`, `b1C_0` as nerve, `b19` bits); the names given in comments come from use.
- `maybe_rescue_guy_from_fire` (`CRITTIME.C`) mixes fine and tile units in the same way in both builds; whether the rescue can work is not known.
- `set_gridx_and_y_based_on_tile_type` places critters in diagonal tile 5 at the corner of type 2, probably a slip.
- What goal 11's fluttering is used for (bats and similar, by the behaviour) has not been checked against the data.
