# How critters think

This page describes what a critter (a monster or an NPC) in Ultima Underworld I and II does, step by step, in the order the game decides it. It is written for someone who wants to reproduce or mod the behaviour and will not read the C. Every number comes from the matched sources of UW2Decomp and [UW1Decomp](https://github.com/abedegno/UW1Decomp), which compile to the same bytes as the shipped `UW2.EXE` and `UW.EXE`.

Each rule is marked **both**, **UW1** or **UW2**. Where the games differ only in a detail, both values are given in place. [UW1-UW2-DIFFERENCES.md](../UW1-UW2-DIFFERENCES.md) has the full list of differences between the games. Where the purpose of a rule is inferred rather than evident from the code, the text says so. Nothing here was checked in a running game.

The code-oriented notes for the same sources are in [subsystems/critters.md](../subsystems/critters.md).

## Contents

- [Units and words](#units-and-words)
- [When a critter thinks](#when-a-critter-thinks)
- [One step, in order](#one-step-in-order)
- [Reacting before the goal](#reacting-before-the-goal)
- [The goals](#the-goals)
- [Perception](#perception)
- [Melee](#melee)
- [Spells and missiles](#spells-and-missiles)
- [Fleeing and being cornered](#fleeing-and-being-cornered)
- [Getting to a square](#getting-to-a-square)
- [How hostility starts and spreads](#how-hostility-starts-and-spreads)
- [While the player is away or asleep](#while-the-player-is-away-or-asleep)
- [Death](#death)
- [Open questions](#open-questions)

## Units and words

- **Tile** is one map square. **Fine unit** is an eighth of a tile. Distances in this page are squared, as the game compares them, e.g., "squared fine distance under 0x64" means closer than 10 fine units, 1.25 tiles.
- **Heading** is a byte, 256 to a full turn, so 0x20 is 45 degrees. Some rules use the coarse heading, 8 to a turn.
- **Height** in the map is 0 to 15 per tile. An object's height `z` is in eighths of that (0 to 127).
- **Time.** The game clock runs at 256 units a second of play (`GAME_TIME`). An hour of game time is 0xE1000 units, so game time passes at the same rate as real play time.
- **Goal** is the low 4 bits of the critter's goal word. **Goal target** (gtarg) is the next 8 bits, a mobile object index, where 1 is the player.
- **Attitude** is 0 hostile, 1 upset, 2 mellow, 3 friendly. A new critter is mellow (attitude 2).
- **Home** in this page means the square in the critter's quality and owner fields (`myxhome`, `myyhome`). The object's own "home" fields are its current tile.
- **Ally** is bit 6 of byte 0x19. **Loner** is bit 7 of byte 0x0A. A loner does not count towards its race (see below).
- Byte 0x19 also holds flags the code reads as "knows where its target is" (bit 0), "heard something" (bit 1), "keeps fighting when cornered" (bit 4) and "committed to the attack" (bit 5). The names come from how the code uses them.
- **Creature values** come from the critter table in `OBJECTS.DAT` (`struct Creature` in [critter.h](../../src/include/critter.h)): hit points, dexterity, speed, run speed, hearing, sight, noise, visibility, laziness, alertness, nerve (the low 4 bits of byte 0x1C), range (its high 4 bits), the three attacks, the three spells and the caster value.

## When a critter thinks

**Both.** Critters do not think every frame. Each mobile object has a bin, 0 to 15, and a rate, 0 to 7. A bin clock advances 16 times a second. An object is due when its bin is up to 4 behind the clock, and each update moves its bin on by its rate. So a critter with rate 4 updates about 4 times a second, and one with rate 1 about 16 times. The goals set the rate as they run, e.g., 4 while moving or fighting, 6 while standing, 7 while frozen.

- A critter more than 10 tiles from both the player and the point the 3D view was last drawn from (squared tile distance over 0x64 to each) does nothing. Its bin moves on by 8, so it is checked again half a second later. A critter with goal 3 is never skipped this way.
- Critters stand still while time is stopped (`MoveCrits` is clear or the time stop spell is on).

Source: `move_mobile`, `timetodo`, `critter_ai` ([AI.C:1551](../../src/critter/AI.C#L1551), [AI.C:1539](../../src/critter/AI.C#L1539), [AI.C:1026](../../src/critter/AI.C#L1026)); UW1 [AI.C:1539](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L1539), [AI.C:1055](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L1055); the clock in [PLAYMOVE.C:326](../../src/motion/PLAYMOVE.C#L326).

## One step, in order

**Both**, unless marked. When a critter is due, the game does the following.

1. **Physics.** The critter moves by its speed and heading through the physics engine, as a walker, a flier or a swimmer. A creature whose object data resists fire (resist bit 8) ignores lava for the step. The walker's collision rules decide what stops it.
   - Falling off a ledge starts gravity and takes away control for that step.
   - A walker whose whole footprint is on water drowns. It splashes and goes to the last frame of its dying sequence, so it dies on its next step.
   - A walker that is not following a stored path stops at water, at the edge of a drop, and at lava it is not already standing on. A wall or a step that is too high also stops it.
   - Bumping into an object records the object. A door is noted as a door.
2. **Animation.** What happens next depends on the critter's animation sequence.
   - A critter with goal 11 (flutter) or goal 3 skips the sequence handling and goes straight to the decision in step 3.
   - **Dying.** On the last frame of the dying sequence the critter is removed. Its scripted death is run (`death_check`), its inventory is generated and dropped, and its corpse is built (see [Death](#death)). Otherwise the frame advances.
   - **Attacking.** On the first frame, if the target is the player, the combat music starts. The blow lands on frame 3 in UW2 and on frame 4 in UW1. The blow's strength is the charge for the critter's wind-up counter (see [Melee](#melee)). After the last frame the critter goes back to its combat stance and the wind-up counter is reset to 0.
   - **Casting or firing.** On frame 3 in UW2 and frame 4 in UW1 the critter casts the spell in its cast slot, or fires its missile weapon if the slot is 0. Spells and missiles are aimed up or down by the height difference to the target (see [Spells and missiles](#spells-and-missiles)).
   - **Anything else.** The critter decides what to do (step 3).
3. **Decision.** The critter reacts to what has happened (see [Reacting before the goal](#reacting-before-the-goal)), runs its goal (see [The goals](#the-goals)), and then limits its turn.
4. **Turn limit.** In one step a critter's facing may change by at most 45 degrees from where it faced before the step. While it was moving and is still moving at a speed above 1, its direction of travel may also change by at most 45 degrees, and a turn of more than 90 degrees stops it. A critter whose heading the physics changed in this step keeps its old heading of travel.
5. The bin moves on by the rate.

UW2 looks up each animation's length in `CRIT\CR.AN`. In UW1 every sequence has 4 frames.

Source: `critter_ai`, `constrain_movement` ([AI.C:1026](../../src/critter/AI.C#L1026), [AI.C:856](../../src/critter/AI.C#L856)), `crit_hndlr_walk` ([PATHFIND.C:387](../../src/critter/PATHFIND.C#L387)); UW1 [AI.C:1055](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L1055), [PATHFIND.C:236](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/PATHFIND.C#L236).

## Reacting before the goal

**Both.** Before running its goal, the critter checks two things in this order. A critter with goal 11 skips both, and so does a critter with goal 15 (held, UW2). A creature with the "ignores fights" bit (bit 1 of byte 0x0A of its creature entry) skips both as well.

**1. Joining a fight.** The game remembers the last critter the player hit: its race, its square, its height and the time (see [How hostility starts and spreads](#how-hostility-starts-and-spreads)). A critter joins the fight when all of these hold:

- either it is not an ally, is not the critter that was hit, is not a loner, and is of the same race as the critter that was hit; or it is an ally of the player;
- the hit was less than 0x200 clock units ago (2 seconds);
- the critter's tile distance to the square of the hit, counted as |dx| + |dy|, is less than its hearing value.

A critter that joins becomes hostile and knows where its target is. Unless it is fleeing (goal 6) or cornered (goal 9), it takes goal 5 (attack). Its target is the player, or, for an ally, the critter the player hit. Its destination is the square of the hit.

**2. Being hit.** A critter that was hit since its last step (its `last_hit` field holds the attacker) reacts when the attacker is the player and the critter is not an ally, or the critter is an ally, or the attacker is an ally. The attacker becomes its target. If the target is dead, it skips to its goal. If the attacker is the player, the critter becomes hostile and its destination is the player's square. Then it takes the first of these that applies:

| Test | New goal |
| --- | --- |
| The target is more than 1 tile away (squared tile distance over 2) and the creature has no ranged-spell bit (bit 0 of byte 0x2D), or stands on an anti-magic square | 5, attack, and it becomes committed to the attack |
| It is already committed to the attack | 5, attack |
| It is not marked "keeps fighting when cornered" and [should_i_flee](#fleeing-and-being-cornered) says it flees | 6, flee |
| It is marked "keeps fighting when cornered" | 9, cornered |
| Otherwise | 5, attack |

The record of the hit and the damage tally (`b11`, see fleeing) are then cleared.

Walking critters also play a footstep sound on odd frames of the walking sequence, chosen by the creature's sound kind. UW1 and UW2 use different sound numbers.

Source: `critter_mv` ([AI.C:1168](../../src/critter/AI.C#L1168)); UW1 [AI.C:1194](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L1194).

## The goals

**Both**, unless marked. A critter that is not under control this step (falling or jumping) skips most goal work.

| Goal | Name | What the critter does |
| --- | --- | --- |
| 0, 7 | stand | Watches for the player if hostile (the guard check below), otherwise stands. |
| 1 | go home | Heads for its home square. On arrival it takes goal 8. |
| 2 | wander | Wanders (below). |
| 3 | follow (UW1) | **UW1.** Within about 1.4 tiles of its target (squared tile distance 2 or less) it faces the target and plays its first attack animation, without a blow. More than 8 tiles away (squared tile distance over 0x40) it is moved at once to a square a quarter of the way back along the line from the target to itself, at floor height. In between it heads for the target's square. **UW2.** The goal switch does nothing, so the critter only drifts with its physics. |
| 4 | guard | The guard check below, then mills about. |
| 5 | attack | See [Melee](#melee) and [Spells and missiles](#spells-and-missiles). |
| 6 | flee | See [Fleeing](#fleeing-and-being-cornered). |
| 8 | mill | A hostile critter takes goal 4 (guard) with the player as target. Otherwise it wanders while within its range of home (squared tile distance up to range squared), and heads home when further. |
| 9 | cornered | See [Fleeing](#fleeing-and-being-cornered). |
| 10 | talk | Waits for the player (below). |
| 11 | flutter | Speed 0 or 1 at random, a random heading and a random pitch each step, rate 4. It ignores everything else. |
| 12 | hover | A hostile critter takes goal 4. Otherwise it heads home and stands there. |
| 15 | held (UW2) | **UW2.** The critter is frozen. Its goal target counts down by 1 each step, and at 0 the goal is dropped. UW2's Paralyze spell uses it. |
| other | none | Rate 7, nothing else. |

Goals 5, 6 and 9 need a living target. If the target has died, the critter drops the goal.

**Dropping a goal.** When a guarding critter (goal 4) is given another goal, the game remembers goal 4 as its old goal. Dropping a goal returns the critter to its old goal with the player as target, or, if there is none, to goal 2 (wander) with no target.

**The guard check** (goals 0, 4 and 7, and half the time for a hostile wanderer). Only a hostile critter does it, and its target becomes the player.

1. If it already knows where the player is, it takes goal 5 (attack).
2. If it heard something earlier, then with chance (15 minus alertness) in 16 it forgets the sound. Otherwise it turns one eighth towards the player.
3. With chance alertness in 16 it looks for the player ([Perception](#perception)). If it perceives the player clearly, it notes the player's square and takes goal 5. If it hears the player faintly, it remembers the sound, and half the time it walks towards the player's square for this step.
4. Then it acts on its goal. Goal 2 wanders. Goals 0 and 7 stand at rate 6. Any other goal mills about.

**Wandering** (goal 2, and the fallback when no path is found).

- A hostile wanderer does the guard check half the time instead.
- Fliers pick a new pitch each step. Below 2 height units above the floor they climb or stay level. Above height 14 they sink or stay level. In between they pick at random.
- The critter switches between standing and walking at random, at the end of an animation. A standing critter starts walking with chance laziness in 16. A walking critter stops with chance (15 minus laziness) in 16. The name "laziness" (`lazy`, the low 4 bits of byte 0x1F) is provisional, and the rule suggests the value means the opposite.
- A walker that bumped into something turns 90 degrees left or right and stops for the step.
- Otherwise a walker drifts up to 45 degrees off its heading with chance (laziness + 8) in 64, and steers round the player when the player is within 10 fine units. A standing critter turns with chance laziness in 128.
- Walking uses the creature's speed at rate 4. Standing uses rate 6.
- A standing critter, or any critter while the player has a weapon drawn, turns to face the player and stops when the player is within 1.5 tiles (squared fine distance under 0x90).

**Talking** (goal 10).

- **UW2.** A hostile critter takes goal 4 (guard) instead.
- The critter stands at rate 6 and looks for the player. If it perceives the player at all and the player is within 2.5 tiles (squared fine distance under 0x190), it turns to face the player.
- Within 1.5 tiles (under 0x90), if the player is facing it (the player's coarse heading points at the critter, within 45 degrees either side), it starts the conversation itself.

Source: `critter_mv`, `crit_guard`, `crit_drunkwalk`, `crit_mill`, `crit_talk`, `check_out_player`, `crit_hover`, `critter_set_goal`, `critter_discard_goal` ([AI.C:1168](../../src/critter/AI.C#L1168), [AI.C:268](../../src/critter/AI.C#L268), [AI.C:150](../../src/critter/AI.C#L150), [AI.C:243](../../src/critter/AI.C#L243), [AI.C:693](../../src/critter/AI.C#L693), [AI.C:735](../../src/critter/AI.C#L735), [AI.C:756](../../src/critter/AI.C#L756), [AI.C:1393](../../src/critter/AI.C#L1393), [AI.C:1403](../../src/critter/AI.C#L1403)); UW1 [AI.C:211](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L211) (goal 3), [AI.C:275](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L275), [AI.C:726](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L726).

## Perception

**Both.** A critter perceives its target by hearing or by sight. Each creature has hearing and sight values, and each kind of target has noise and visibility values. The player's own entry is creature 63, and the game sets the player's noise and visibility each round from movement and the Stealth skill (`set_sound` in [PLAYMOVE.C](../../src/motion/PLAYMOVE.C)).

- Hearing range, in tiles, is `hearing * noise / 16`, rounded down. Sight range is `sight * visibility / 16`.
- The test uses the squared tile distance `d` between the critter's tile and the target's tile.

The critter decides in this order:

1. If `d` is less than a quarter of the hearing range squared (the target is within half the hearing range), it perceives the target clearly.
2. If `d` is at most the sight range squared, and the target is within 45 degrees of the way the critter faces (its coarse heading, or one eighth either side), and there is a line of sight between the two at their eye heights, it perceives the target clearly and now knows where the target is.
3. If `d` is less than 4 times the hearing range squared (within twice the hearing range), it hears the target faintly.
4. Otherwise it does not perceive the target, and it forgets where the target is.

While attacking, a critter rechecks its target one time in eight when the target has left the square it was heading for. If it still perceives the target clearly, or hears it faintly and wins a coin toss, it updates its destination. Otherwise it drops the goal. The two games set the "heard something" flag differently on dropping: UW1 sets it when the target was heard faintly, and UW2 sets it when the target was not perceived.

Source: `target_found`, `crit_offense_find_target` ([AI.C:783](../../src/critter/AI.C#L783), [AI.C:452](../../src/critter/AI.C#L452)); UW1 [AI.C:820](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L820), [AI.C:464](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L464).

## Melee

**Both.** A critter with goal 5 fights hand to hand when it is in reach: the squared fine distance to the target is under 0x64 (10 fine units) or both stand on the same tile, and the height difference is under 4 height units (fliers ignore height). A critter that attacks the player becomes hostile.

In reach, each step the critter first faces the target and then moves by distance:

| Squared fine distance | What it does |
| --- | --- |
| under 0x31 (too close) | Three times in four it backs straight away at speed 2. One time in four it sidesteps 90 degrees left or right at two thirds of its speed, in its combat stance. |
| over 0x51 | It walks in at speed 2. |
| otherwise | With chance dexterity in 64 it dodges in a random direction at speed 1. Otherwise it stands in its combat stance. |

Fliers also pitch towards the target's height. Then, if the squared fine distance is at most 0x64:

- One time in four it starts a blow. It rolls `r = rand() % 100` and picks the first of its three attacks whose probability is above `r`, subtracting each skipped attack's probability from `r`. If neither of the first two is picked, it uses the third. The probabilities in the creature table are percentages.
- Otherwise its wind-up counter goes up by 1, to at most 15.

When the blow lands, its charge comes from the wind-up counter:

| Counter | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Charge | 50 | 60 | 70 | 80 | 90 | 100 | 110 | 120 | 130 | 140 | 155 | 170 | 185 | 205 | 230 | 255 |

So a critter that circles for a while before striking hits harder. The blow itself (to-hit, damage, poison) is described with combat. The blow is passed a random swing kind 0 to 8, the attack number and the creature's poison value (byte 0x0F).

**UW1.** After a melee step, UW1's goal 5 also runs the closing-in step below, where UW2 stops. So one time in eight a UW1 critter in melee rechecks whether it perceives its target, and it drops the goal if it does not.

Source: `crit_offense`, `crit_attack`, `atk_charge` ([AI.C:331](../../src/critter/AI.C#L331), [AI.C:383](../../src/critter/AI.C#L383), [AI.C:78](../../src/critter/AI.C#L78)); UW1 [AI.C:340](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L340), [AI.C:395](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L395).

## Spells and missiles

**Both.** Out of reach, a critter with goal 5 tries a ranged attack.

- **Caster** (caster value above 0). First it may cast its defensive spell: if its third spell is not 0xFF, then with chance caster in 256, and not on an anti-magic square, it starts casting that spell. Otherwise, if it has the ranged-spell bit (bit 0 of byte 0x2D), it tries a spell attack. The spell attack is possible when it is not on an anti-magic square, the target is within 8 tiles (squared tile distance under 0x40), there is a line of sight, and the critter has turned to face the target (it turns 45 degrees a step). Then, with chance caster in 128, it starts casting its first spell (11 times in 16) or its second (5 times in 16).
- **Archer** (not a caster, and its first weapon slot holds a missile class item). The missile attack is possible within 4 tiles (squared tile distance under 0x10), with line of sight and facing the target. Then, with chance (dexterity + 1) in 192, it starts firing.
- If the attack was possible, whether or not it started, the critter holds its combat stance and stands still.
- **Aim.** The vertical aim is `4 * dz / distance`, clamped to -15 to 15. For a missile it adds `distance * 3 / missile speed`, which lobs a slow missile higher at range.

If no ranged attack was possible, the critter closes in:

- A critter that was guarding (its old goal is 4), is more than 2 tiles from the target (squared fine distance over 0x100), is not committed to the attack, and is further from home than twice its range, gives up. It forgets the target and goes back to guarding.
- Otherwise it heads for the target. A critter with the ranged-spell bit stops closing in at 4 tiles. Others close to the target's tile, or keep heading in while the height difference is 4 or more.
- If it gives up on the route (see [Getting to a square](#getting-to-a-square)), it drops the goal.

**UW1, Tybal's guards.** On level 7, while Tybal's orb is whole, critters of race 0x13 never cast, neither attack spells nor defensive ones, and close right in to melee.

Source: `crit_offense`, `maybe_cast_defensive_spell`, `crit_magik_attack`, `crit_missile_attack`, `compute_trz_or_try_rather`, `crit_offense_find_target` ([AI.C:331](../../src/critter/AI.C#L331), [AI.C:481](../../src/critter/AI.C#L481), [AI.C:499](../../src/critter/AI.C#L499), [AI.C:522](../../src/critter/AI.C#L522), [AI.C:949](../../src/critter/AI.C#L949), [AI.C:452](../../src/critter/AI.C#L452)); UW1 [AI.C:340](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L340), [AI.C:506](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L506), [AI.C:526](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L526).

## Fleeing and being cornered

**Both. Whether to flee.** When it is hit, a critter decides whether to flee from its creature hit points (`max`, the table's average), its current hit points (`hp`), its nerve and the damage it has taken since it last decided (`damage`):

1. If `hp` is above three quarters of `max`, it does not flee.
2. If `hp` is below an eighth of `max`, it does not flee. A nearly dead critter fights on.
3. If `damage` is above half of `max`, it flees.
4. Otherwise it flees when `hp * 16 / max + rand() % 4` is at most `15 - nerve`.

So a critter with nerve 15 flees only after one heavy blow, and a critter with nerve 0 flees whenever it is between an eighth and three quarters of its hit points, if `hp * 16 / max + rand() % 4` is at most 15, which always holds there.

**Fleeing** (goal 6). The critter runs from its target.

- Fliers pick a pitch that rises while they are low (z up to 0x6E in UW2).
- **Close** (squared tile distance 3 or less, and height difference under 0x10): with chance `nerve / 8` in 256 (so only a critter with nerve 8 or more, and then 1 in 256), or at once if it is stuck against something, it turns to fight. It takes goal 9 and is marked "keeps fighting when cornered". Otherwise it backs away from the target at half its speed, facing the target.
- **Stuck further away**: within 3 tiles (squared tile distance under 9) it turns to fight (goal 9), or if already cornered it stands and faces the target. Beyond that it turns 90 degrees left or right plus up to 45 degrees.
- **Otherwise** it may first cast its defensive spell (chance caster in 256). Then it runs, drifting up to 45 degrees off its heading with chance (laziness + 8) in 64, and steering away from the player when the player is within 24 fine units. It runs at its run speed within 8 tiles of the target (squared tile distance under 0x40) and at its walking speed beyond.

**Cornered** (goal 9). The critter fights back.

- Within 1.5 tiles (squared fine distance under 0x90), or on the target's tile, it fights in melee, as above.
- Beyond 2 tiles (squared tile distance over 4) it tries its defensive spell, then a spell attack if it is a caster, or a missile attack if it is an archer. A critter with neither flees for this step.
- In between it stands in its combat stance and faces the target.

Source: `should_i_flee`, `crit_flee`, `crit_defense`, `crit_avoid_player` ([AI.C:1355](../../src/critter/AI.C#L1355), [AI.C:579](../../src/critter/AI.C#L579), [AI.C:541](../../src/critter/AI.C#L541), [AI.C:652](../../src/critter/AI.C#L652)); UW1 [AI.C:1356](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L1356), [AI.C:610](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L610), [AI.C:571](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L571).

## Getting to a square

**Both**, unless marked. Going home, closing in and walking to a sound all use one routine, `crit_head_for_loc`. Each step it decides in this order.

1. **Arrived.** On the destination tile, a critter going home (goal 1) takes goal 8 (mill). Any other critter stops and stands.
2. **No control** (falling or in a jump). It only keeps its stored path in step, at rate 1.
3. **Bumped into something** this step, and not already given up on the destination:
   - A door. The critter stops. One time in four it gives up on the destination. Otherwise it tries to open the door (below).
   - Two critters that are both attacking ignore each other.
   - **UW2.** A flier that meets an open door passes it by pitching down.
   - Anything else makes it give up on the destination.
4. **Following a stored path.** It walks to the next square of the path. A path step that needs a jump makes the critter walk to the edge of the square and leap (rate 1, pitch 22, speed 11).
5. **Straight line.** If the line was clear before and the destination has not changed, it walks straight at the destination.
6. **Given up.** If it gave up on the destination, it wanders, and each step it forgets that it gave up with chance 1 in 8.
7. **Straight line check.** If every tile on the straight line to the destination can be walked without a jump, it walks straight.
8. **Path search.** If one of the 16 shared path slots is free, it searches for a path. If one is found, the path is stored in the slot and the critter follows it.
9. **No way.** It gives up on the destination and wanders.

A moving critter walks at its run speed when attacking (goal 5) and at its walking speed otherwise, at rate 4. Fliers aim to stay 0x14 z units (2.5 height units) above the floor at the start and at the destination, never above z 0x78.

**The path search.**

- It searches breadth first, in the four straight directions only.
- It searches at most 40 steps in UW2 and 32 in UW1.
- It searches only a box that reaches 5 tiles beyond the start and the destination on each side. (UW2's code has a reach of 10 for one case, but that case can only be level 0, so in play the reach is always 5.)
- A step may climb at most one height unit. A slope counts one higher unless it is climbed the way it rises.
- The walls between squares block, including the walls of diagonal tiles.
- A door that is locked against the critter blocks the step when its frame lies across the move.
- The terrain rules depend on how the creature moves. Walkers never enter water and treat lava as danger. Swimmers keep off plain floor and lava. Fliers ignore floor heights and terrain.
- **Danger.** A path collects danger. A drop to a lower floor costs its height minus 1, entering a dangerous floor costs 2, and a jump costs 1. A step whose danger would pass the critter's limit is refused.
- The limit is 0 for a critter that is not hostile, so peaceful critters never take drops or cross lava. For a hostile critter it is `hp * 4 / creature hit points + nerve / 4`.
- **UW1.** The critter with conversation 0x16 on level 6 always has a limit of 0, even when hostile.
- Only creatures with the jump bit (bit 5 of byte 0x0A of the creature entry) take a path step that needs a jump.

**Doors.** A walker that bumps into a closed door tries to open it, three times in four (step 3 above).

- Secret doors (item ids ending in 7) are never touched.
- **UW2.** On level 10 critters never open doors. **UW1.** A critter that is not hostile never tries.
- A creature with a lock value uses the door. On a closed door it then tries the lock half the time, as a lock pick check with its lock value.
- Otherwise, one time in four, it bashes the door for `rand() % (damage of its first attack)`.

Source: `crit_head_for_loc`, `flood_path`, `hyp_move`, `acceptable_danger`, `try_to_open_door`, `do_that_jump_kinda_thing`, `adjust_height` ([PATHFIND.C:1240](../../src/critter/PATHFIND.C#L1240), [PATHFIND.C:749](../../src/critter/PATHFIND.C#L749), [PATHFIND.C:498](../../src/critter/PATHFIND.C#L498), [AI.C:1381](../../src/critter/AI.C#L1381), [PATHFIND.C:1392](../../src/critter/PATHFIND.C#L1392)); UW1 [PATHFIND.C:1113](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/PATHFIND.C#L1113), [PATHFIND.C:609](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/PATHFIND.C#L609), [AI.C:1382](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L1382), [PATHFIND.C:1248](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/PATHFIND.C#L1248).

## How hostility starts and spreads

**Both**, unless marked. These are all the ways the sources make a critter hostile or less friendly.

- **Being hit by the player.** The critter becomes hostile and reacts ([Being hit](#reacting-before-the-goal)). Any critter whose attack goal targets the player becomes hostile.
- **Kin hear the fight.** When the player damages a critter that is not a loner, the game records its race, square, height and the time. For the next 2 seconds, every critter of that race within hearing distance of the square (as |dx| + |dy| in tiles, below its hearing value) becomes hostile and attacks the player. The player's allies within range attack the critter the player hit. The record is saved with the game.
- **Theft.** When the player picks up an object with an owner, up to 20 critters of the owner's race within a 15 by 15 tile square centred on the object, that are within their sight range of the object and have a line of sight to it, each become one step less friendly, to at least hostile. Each says so in the message scroll: "is angered by your action" (now hostile), "is annoyed by your action" (now upset) or "notes your action" (now mellow). Loners are told only when the owner value has bit 0x20 set. The object then has no owner.
  - **UW1.** Owner 0x0D does not care once the Knight of the Crux quest byte is 3 or more.
  - **UW2.** The owner field covers races up to 0x1D, and everything inside a taken container also loses its owner.
- **Other bad deeds (UW2).** Damaging an owned object, opening an owned container or emptying an owned bag, and some world events, count as theft from the owner's race (`player_did_bad`).
- **Conversations** set attitude and goals directly through the conversation variables. An attitude above 3 written by a conversation makes the critter friendly and an ally.
- **Spells.** Fear, Charm, Ally and Paralyze change goals and attitudes; see the magic notes.
- **Talking to a hostile critter.** **UW2.** A hostile critter with goal 10 turns to guarding.
- **Time passing.** Races drift towards their majority mood (below).

Allies are not permanent. Every 30 duration checks in UW2 (24 in UW1), about every 10 minutes of play in UW2, the game clears the ally bit of every mobile object three times in four, and rolls each one's "fed" bit at random. Sleep and level changes clear the ally bit as well.

Source: `damage_critter` ([AI.C:1469](../../src/critter/AI.C#L1469)), `critter_mv` ([AI.C:1168](../../src/critter/AI.C#L1168)), `player_grabbed`, `critter_get_told`, `yearly_checkup` ([CRITTIME.C:535](../../src/critter/CRITTIME.C#L535), [CRITTIME.C:474](../../src/critter/CRITTIME.C#L474), [CRITTIME.C:76](../../src/critter/CRITTIME.C#L76)), `player_did_bad` ([WORLDEV.C:578](../../src/event/WORLDEV.C#L578)), the conversation's attitude ([CONVVARS.C:145](../../src/conv/CONVVARS.C#L145)); UW1 [AI.C:1458](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L1458), [CRITTIME.C:420](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/CRITTIME.C#L420), [CRITTIME.C:470](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/CRITTIME.C#L470).

## While the player is away or asleep

**Both. Time passes for a level** when the player sleeps, when the player returns to a level, and on some world events. For every critter on the level:

1. A summoned critter (temporary bit) is removed.
2. Its ally bit and the flags of byte 0x19 are cleared, so it forgets its target.
3. It faces a random way, unless its goal is 10 (talk).
4. If it is hurt and may heal, it heals half way to its creature hit points: `hp = (hp + max) / 2`.
5. If it is not a loner, it counts towards its race's mood: -1 if hostile, +1 if friendly.
6. It moves to its home square at once, if there is room there.

Mobile objects that are not critters, such as thrown objects and missiles, come to rest. Fireballs and resilient spheres are left alone.

Then every critter that is not a loner shifts its attitude by its race's count, kept between hostile and friendly. So a race with more hostile members than friendly ones grows more hostile, by the difference, and the other way round.

**Sleep is refused** while a critter that is attacking, guarding or cornered and knows where its target is stands within 2 tiles of the player.

**A monster may find the sleeper.** During sleep the game looks at the hostile critters in a 17 by 17 tile square round the player, one at a time. It skips each one half the time, and skips any that is further from the player than `range * sqrt(3)` tiles. For the first one that has a walking path to the player with no danger at all, at least 2 squares long, the game moves the critter to the square two before the player's, knowing where the player is. Ward traps on the path go off as it passes. The player then wakes.

Source: `update_all_critters_whilst_player_snoozes`, `up_crit`, `up_mob`, `hostile_creatures_near`, `wander_that_monster`, `wandering_monster_check` ([CRITTIME.C:232](../../src/critter/CRITTIME.C#L232), [CRITTIME.C:130](../../src/critter/CRITTIME.C#L130), [CRITTIME.C:292](../../src/critter/CRITTIME.C#L292), [CRITTIME.C:305](../../src/critter/CRITTIME.C#L305), [CRITTIME.C:414](../../src/critter/CRITTIME.C#L414)), `gronk_area` ([SPELLS.C:674](../../src/combat/SPELLS.C#L674)); UW1 [CRITTIME.C:230](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/CRITTIME.C#L230), [CRITTIME.C:303](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/CRITTIME.C#L303).

## Death

**Both.** When damage takes a critter to 0 hit points, it starts its dying sequence, unless it has a conversation and its scripted death (`death_check`) keeps it alive. Several plot NPCs surrender this way instead of dying. The kill is credited to whoever struck: the attacker, or for a missile or thrown object, its shooter. A kill by the player gives experience.

**Death cry.** **UW2** plays a cry by the dying critter's death kind (none, or one of four sounds). **UW1** decides by the creature the AI processed last, which is a bug; see UW1Decomp's [FINDINGS.md](https://github.com/abedegno/UW1Decomp/blob/main/docs/FINDINGS.md).

**Combat music.** When the player damages a critter, the music becomes the "foe hurt" theme if the critter is below a quarter of its hit points (`hp * 64 / (max + 1)` under 16), else the combat theme. When a critter damages the player, the music becomes the "danger" theme if the player is below a quarter of his maximum, else the combat theme.

**Remains.** On the last frame of dying, the critter's inventory is generated and dropped and its remains are built.

- If the creature has blood (fluids), a blood stain of that kind is placed where it died, with quality 0x28.
- If it has a corpse, the corpse is left 7 times in 16. **UW2.** In world 7 the corpse is always left. The corpse's owner field records the creature type.

Source: `damage_critter`, `crit_die`, `go_into_dying_sequence` ([AI.C:1469](../../src/critter/AI.C#L1469), [AI.C:1432](../../src/critter/AI.C#L1432), [AI.C:1417](../../src/critter/AI.C#L1417)), `build_corpse` ([PATHFIND.C:146](../../src/critter/PATHFIND.C#L146)); UW1 [AI.C:1436](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L1436), [PATHFIND.C:146](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/PATHFIND.C#L146).

## Open questions

- The names of several creature bits come from how the code uses them, not from a source: "ignores fights" (byte 0x0A bit 1), "can jump" (byte 0x0A bit 5), "ranged-spell bit" (byte 0x2D bit 0), nerve and range (byte 0x1C). The meaning of the byte 0x19 flags is also inferred.
- Which critters use goal 3 (follow) in UW1, and goal 11 (flutter) in either game, was not checked against the level data.
- Fire resistance is inferred for resist bit 8, from the lava rule it unlocks.
- [FINDINGS.md](../FINDINGS.md) lists the likely bugs in these routines, e.g., the critter placed in a north-west diagonal tile, and the flameproof critter that leaves lava marked in the shared walking rules.
