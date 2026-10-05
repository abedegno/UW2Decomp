# Player upkeep

This page describes what happens to the player over time in Ultima Underworld I and II: the slow update and what runs in it, hunger, fatigue, healing, poison, drink, lights, sleep and dreams, and experience and levels. It is written for someone who wants to reproduce or mod the rules and will not read the C. Every number comes from the matched sources of UW2Decomp and [UW1Decomp](https://github.com/abedegno/UW1Decomp).

Each rule is marked **both**, **UW1** or **UW2**. [UW1-UW2-DIFFERENCES.md](../UW1-UW2-DIFFERENCES.md#skills-experience-and-the-player-record) lists the differences in more detail. Nothing here was checked in a running game.

## Contents

- [The slow update](#the-slow-update)
- [Hunger and eating](#hunger-and-eating)
- [Fatigue](#fatigue)
- [Healing and mana while awake](#healing-and-mana-while-awake)
- [Poison](#poison)
- [Drink](#drink)
- [Drowning (UW2)](#drowning-uw2)
- [Lights](#lights)
- [Sleep](#sleep)
- [Dreams](#dreams)
- [Experience and levels](#experience-and-levels)
- [Death](#death)

## The slow update

**Both.** The game clock counts 256 units a second and runs only while the 3D view runs, at most a quarter of a second per frame. The slow update (`duration_check`) runs once every 20 seconds of game time. A counter numbers the updates, and the work is spread over it:

| When | What |
| --- | --- |
| every update (20 s) | active spells lose a step ([magic.md](magic.md#active-spells-and-their-durations)); lights burn; a mushroom high wears off by one step; the regeneration effects give 1 hit point or 1 mana; drowning is checked (UW2). **UW2** also runs Killorn's countdown and the dream plant's counter |
| every 3rd (1 minute) | poison does its damage; a mana roll |
| every 24th in UW1, 30th in UW2 (8 or 10 minutes) | hunger; sobering up; wandering monsters (chance 1 in 4); the critters' checkup; fatigue and the food counter count up; a hit point roll |
| every 60th, UW2 (20 minutes) | the time of day moves on and the day's schedule runs ([schedules.md](schedules.md#game-time)) |

Source: `duration_check` and its caller ([PLAYTIME.C:71](../../src/game/PLAYTIME.C#L71), [INTERACT.C:118](../../src/ui/INTERACT.C#L118)); UW1 [PLAYTIME.C:128](https://github.com/abedegno/UW1Decomp/blob/main/src/game/PLAYTIME.C#L128).

## Hunger and eating

**Both.** Hunger is a byte from 0 (starving) to 255 (stuffed). Higher is fuller.

- **Getting hungrier.** Every 24 (UW1) or 30 (UW2) updates, hunger falls by `3 + (rand() & 3)`, 3 to 6. Sleep takes more (below).
- **Eating** adds the food's nutrition. If that would take hunger above 255, the player is "too full to eat that now" and the food is not eaten. A new game starts at 0xC0.
- **Healing from food.** A food counter (`food_heal`) counts up by 1 every 24 or 30 updates, to at most 255, and starts at 0x40 in UW1 and 0x30 in UW2. Eating heals `food_heal / 8` hit points in UW1 and `food_heal / 6` in UW2, at most 8, and resets the counter to 0. So food heals more the longer it has been since the player last ate.
- **UW2.** Eating the dream plant (outside the Void) sets the dream counter to 2 to 5 ([Dreams](#dreams)). Eating leaves a stick, bone, wax or bottle for some foods.
- **The compass** reports hunger as one of nine words, by `hunger / 30`.
- **Starving.** Hunger 0 has an effect only at sleep: the player takes 2 damage instead of healing.

Source: `player_eat` ([SKILLS.C:578](../../src/game/SKILLS.C#L578)), eating ([USEITEMS.C:263](../../src/obj/USEITEMS.C#L263)), the compass ([GAMESCR.C:64](../../src/ui/GAMESCR.C#L64)); UW1 [SKILLS.C:640](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLS.C#L640).

## Fatigue

**Both.** Fatigue counts the time since the player last slept: it goes up by 1 every 24 or 30 updates, to at most 255, and sleep sets it to 0. A new game starts it at 0x40 (UW1) or 0x30 (UW2). Fatigue does nothing while awake. At sleep it sets how much the night heals ([Sleep](#sleep)). The compass reports it as one of six words, by `fatigue / 23` (at most 5). Restoration (UW2) sets it to 0.

Source: [PLAYTIME.C:147](../../src/game/PLAYTIME.C#L147), [GAMESCR.C:64](../../src/ui/GAMESCR.C#L64).

## Healing and mana while awake

**Both.**

- **Hit points.** Every 24 (UW1) or 30 (UW2) updates, if `skill_check(strength, 15)` succeeds, the player gains exactly 1 hit point.
- **Mana.** Every 3rd update, `skill_check(Mana, 10)` gives 1 mana on a success and 2 on a great success. **UW2.** Not in most of the Scintillus Academy ([magic.md](magic.md#mana)).
- **Regeneration effects** (class 11 minors 14 and 15, from enchanted items, inferred) give 1 hit point or 1 mana every update.
- **The healing routine**, used by sleep and spells: a strength n above 0 gives `max * (n + rand() % 4) / 16 + 1`. A negative n gives exactly -n. A strength of 0 gives nothing. The result is capped at the maximum.

The maxima, worked out again at every level and whenever the figures change:

- Hit points: `30 + level * strength / 5`.
- Mana: `(Mana + 1) * intelligence / 8`. **UW1.** On Tybal's level the new value is put aside rather than set, while the orb stands.
- Carrying capacity: **UW1** `strength * 20`, **UW2** `strength * 13 + 300`, in tenths of a stone.

Source: [PLAYTIME.C:127](../../src/game/PLAYTIME.C#L127), [PLAYTIME.C:137](../../src/game/PLAYTIME.C#L137), `restore_hp`, `restore_mana` ([SPELLS.C:176](../../src/combat/SPELLS.C#L176), [SPELLS.C:155](../../src/combat/SPELLS.C#L155)), `player_compute` ([SKILLS.C:74](../../src/game/SKILLS.C#L74)); UW1 [SKILLS.C:112](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLS.C#L112).

## Poison

**Both.** Poison is a strength. Every 3rd update (1 minute), the player takes damage equal to the poison, of type poison, and the poison goes down by 1. So poison n does `n * (n + 1) / 2` damage over n minutes. A player who resists poison takes no damage, but the poison still wears off. Sleep does all the remaining damage at once and cures it. Cure Poison and Restoration set it to 0.

A critter's poisonous blow raises the poison to the critter's value ([combat.md](combat.md#a-critters-blow)).

Source: [PLAYTIME.C:127](../../src/game/PLAYTIME.C#L127), sleep [SKILLS.C:475](../../src/game/SKILLS.C#L475).

## Drink

**UW2** (the code was read for UW2 only). Drink raises the drunkenness by the drink's strength, to at most 0x3F. Then the game rolls `skill_check(strength, drunkenness)`:

| Result | Effect |
| --- | --- |
| -1 | "As the alcohol hits you, you stumble and collapse into sleep." The player passes out ([Sleep](#sleep)), and wakes with the view shaking for `drunkenness / 6 + 10` |
| 0 | the view shakes for `drunkenness / 6` |
| 1 | nothing |
| 2 | "The drink makes you feel a little better for now." and 2 hit points |

Drunkenness falls by 1 every 30 updates, by 16 on an interrupted sleep and by 32 on a full night.

**Passing out** where the player stands: while swimming or on lava (motion state bits 0 and 1, inferred) the player dies. While in the air without Slow Fall, Levitate or Fly, the fall does `12 + 10 * (rand() % 6)` damage of type poison. In the Pits of Carnage, passing out kills.

Source: [USEITEMS.C:297](../../src/obj/USEITEMS.C#L297), `drop_drunk_player` ([SKILLS.C:387](../../src/game/SKILLS.C#L387)).

## Drowning (UW2)

**UW2.** A swimming counter rises while the player is under water (in the motion code). Each update while it is above 0x50:

1. The load is `weight * 32 / maximum weight`.
2. If `skill_check(Swimming, load)` fails or is a near miss (result below 1), the counter rises by `rollem(3 - result, 4)`, while below 0x8C.
3. Above 0x78 the player also takes `rollem(2, hurt + 2)` damage, where hurt is 2 minus a second Swimming check against the load, and the screen flashes. A great success on that check (hurt 0) avoids the damage.

Source: `sink_sink_sink` ([PLAYTIME.C:219](../../src/game/PLAYTIME.C#L219)).

## Lights

**Both.** The player's light is the brightest of:

- the lit lights in the four light slots and on the cursor;
- an active light spell's level ([magic.md](magic.md#active-spells-and-their-durations));
- 6 while Roaming Sight is active (UW2);
- **UW2.** the level's minimum light from `DL.DAT`. Each level has one byte: a value of 10 or more makes `value % 10` the level's minimum, e.g., 14 on level 1, Lord British's castle.

**Burning.** Each update, every lit light in the four light slots with burn rate r (from its light data) loses 1 quality when the update counter is a multiple of r. Sleep burns 180 steps an hour at once, `hours * 180 / r + 1` quality. A light goes out at quality 0 in UW1 and at quality 1 in UW2, turning into its unlit item, and can't then be lit again. **UW2.** Lights do not burn while time is stopped.

Source: `DegradeLights` ([PLAYTIME.C:174](../../src/game/PLAYTIME.C#L174)), `FixPlayerEquips` and `load_dl` ([PLAYDATA.C:401](../../src/game/PLAYDATA.C#L401), [PLAYDATA.C:428](../../src/game/PLAYDATA.C#L428)); UW1 [PLAYTIME.C:179](https://github.com/abedegno/UW1Decomp/blob/main/src/game/PLAYTIME.C#L179).

## Sleep

**Both.** The player sleeps from the sleep key (in a bedroll if he carries one, else on the ground), in a bed (UW2), from a sleep trap, or by passing out drunk. A chosen sleep is refused:

- while moving, falling or swimming: "You can't go to sleep here!";
- **UW1.** on level 9, the same message;
- **UW2.** in the Ethereal Void, where sleeping wakes the player from the dream instead;
- while a critter that is attacking, guarding or cornered and knows where the player is stands within 2 tiles: "There are hostile creatures near!" ([npc-ai.md](npc-ai.md#while-the-player-is-away-or-asleep));
- **UW2.** in the Pits of Carnage, the same message.

**The night**, in order:

1. Open doors far from the player may close ([schedules.md](schedules.md#other-timed-events)).
2. 2 to 6 hours pass at once (`rand() % 5 + 2`). Every active spell ends and a mushroom high ends. **UW2.** The schedules catch up.
3. **UW2.** If Killorn Keep is due to crash, it crashes and the player dies.
4. Lights burn for those hours, and all remaining poison damage is done at once and the poison cured.
5. Passing out drunk applies its own dangers ([Drink](#drink)). If the player is now dead, the night ends.
6. **A monster may find the player** ([npc-ai.md](npc-ai.md#while-the-player-is-away-or-asleep)). Then "Your sleep is interrupted!": fatigue falls by 0x20 in UW1 or 0x18 in UW2, hunger by `12 + (rand() & 0xF)`, drunkenness by 16, and there is no healing.
7. **Otherwise the night is full.** Time passes for the level ([npc-ai.md](npc-ai.md#while-the-player-is-away-or-asleep)) and wandering monsters may appear. The rest of a 7 to 10 hour night passes (`rand() % 4 + 7` hours in all), 1 or 2 hours more if the player has fewer than 10 hit points. Then:
   - `regen = fatigue / 2 + 2`, at most 5, and fatigue becomes 0;
   - comfort is 1 if hunger is above 0x40 and the player is in a bed or bedroll, else 0;
   - if hunger is not 0: hit points by the healing routine with strength `regen + regen * comfort - 1`, then 6 mana, then mana with strength `regen + (regen + 1) * comfort - 1`;
   - if hunger is 0: "You are starving." and 2 damage;
   - hunger falls by `24 + (rand() & 0x1F)`, and drunkenness by 32;
   - the player may dream ([Dreams](#dreams)), and then reads "You feel rested." (comfort 1) or "Your sleep is uneasy." (comfort 0).
8. **UW2.** A player who reached 0 hit points in the night is kept alive through the dream, and then dies.

Source: `player_sleep` ([SKILLS.C:421](../../src/game/SKILLS.C#L421)); UW1 [SKILLS.C:525](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLS.C#L525).

## Dreams

**UW2.** After a full night (not after passing out):

1. If the player ate the dream plant (the dream counter is not 0), the dream is the Ethereal Void: "Your dreams are vivid, showing a shifting colored scene...", quest 48 is set and the player goes to the Void. The counter falls by 1 every update, and when it reaches 0 while the player is in the Void, he wakes.
2. Otherwise the castle story dreams 0 to 3 come due as the castle clock (X clock 1) reaches 4, 6, 10 and 14. The latest one that is due and not yet seen is shown.
3. With none due, with chance 1 in `4 + 4 * comfort`, one of dreams 4 to 6 at random is shown if not yet seen. A comfortable night makes these dreams rarer.
4. Dream n is cutscene `0x18 + n`. Once it is shown, its bit in the dream flags is flipped.

**UW1.** Garamon's dreams: dream 0 until it is seen, then dream 1 once the player is off level 1, then dreams 2 and 3 while their bits are set (by other code). With none due, chance 1 in `4 + 4 * comfort` of one of dreams 4 to 9 not yet seen. No dreams once Garamon is buried.

Source: `dream` ([SKILLS.C:334](../../src/game/SKILLS.C#L334)), the counter ([PLAYTIME.C:123](../../src/game/PLAYTIME.C#L123)); UW1 [SKILLS.C:476](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLS.C#L476).

## Experience and levels

**Both.** Experience is kept in tenths (the end screen shows `exp / 10`, UW2). A gain goes through these steps in order:

1. **A loss** (negative n) is taken off, not below 0, and nothing else happens.
2. **UW1.** If experience is already above 0x17700 (96,000), the gain is ignored entirely.
3. **UW2.** Every gain is halved, rounding up or down at random: `(n + rand() % 2) / 2`.
4. **Outgrown.** If the player's level is above `2 * area + 2`, the gain becomes `n / 2 + 1`. The area is the dungeon level in UW1 and the world number (`(level - 1) / 8`) in UW2.
5. **Skill points.** One skill point for every 3,000 (UW1) or 1,500 (UW2) of total experience, never given twice.
6. **UW2.** A gain that would take experience past 0x7FFF0 is refused. Above 0x17700 experience still grows, but no more levels come.
7. **Levels.** The level rises while `experience / 500` reaches the next threshold, to at most 16.

| Level | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Experience | 500 | 1,000 | 1,500 | 2,000 | 3,000 | 4,000 | 6,000 | 8,000 | 12,000 | 16,000 | 24,000 | 32,000 | 48,000 | 64,000 | 96,000 |

**Gaining a level** prints the new level, gives one skill point per level gained (on top of step 5), and works out the maxima again ([Healing and mana](#healing-and-mana-while-awake)). It does not restore hit points or mana.

**Where experience comes from:** kills ([combat.md](combat.md#experience-for-a-kill)), conversations (`new_player_exp` and `x_exp`, [conversations.md](conversations.md#variables-in-and-out)), experience traps, and newly seen squares of the map (`seen * PlayerLevel / 10` in UW1, `seen * (PlayerLevel / 8 + 1) / 10` in UW2, see [UW1-UW2-DIFFERENCES.md](../UW1-UW2-DIFFERENCES.md#light-exploration-and-the-automap)).

**Spending skill points** is described with the conversation built-in `x_skills` ([conversations.md](conversations.md#built-ins-quests-variables-skills-and-time)).

Source: `player_get_exp`, `level_table` ([SKILLCHK.C:63](../../src/game/SKILLCHK.C#L63), [SKILLCHK.C:29](../../src/game/SKILLCHK.C#L29)), `advance` ([SKILLS.C:89](../../src/game/SKILLS.C#L89)); UW1 [SKILLCHK.C:70](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLCHK.C#L70).

## Death

**Both.** When the player's hit points reach 0, the outcome depends on the game and the place. [UW1-UW2-DIFFERENCES.md](../UW1-UW2-DIFFERENCES.md#death) has the rules: in UW1 the silver tree and the talismans, in UW2 jail, the Void, the pits and the blackrock gem. Death costs an eighth of the experience in UW1 and a ninth in UW2 where the game goes on.

Source: `player_is_dead` ([SKILLS.C:802](../../src/game/SKILLS.C#L802)).
