# Schedules and timed events

This page describes how Ultima Underworld II moves its NPCs through the day and moves the plot on: the X clocks, the schedules in `SCD.ARK` and what each row does, the castle household's daily round, and the other things that happen by the clock. It is written for someone who wants to reproduce or mod the behaviour and will not read the C. Every number comes from the matched sources of both games in this repository ([uw1/](../../uw1/), [uw2/](../../uw2/)).

Each rule is marked **both**, **UW1** or **UW2**. The schedules are UW2 only. UW1 has no schedules and no X clocks, and its NPCs move only by their AI goals ([npc-ai.md](npc-ai.md)), by conversations and by traps. The timed events UW1 does have are listed in [Other timed events](#other-timed-events). Statements about the shipped data were measured from the GOG release's `DATA\SCD.ARK` and say so. Nothing here was checked in a running game.

The file layout of `SCD.ARK` is in [FORMATS.md](../FORMATS.md#scdark). The code-oriented notes are in [subsystems/events.md](../../uw2/docs/subsystems/events.md#schedules).

## Contents

- [Game time](#game-time)
- [The X clocks](#the-x-clocks)
- [How a schedule runs](#how-a-schedule-runs)
- [Which critters a row acts on](#which-critters-a-row-acts-on)
- [The row events](#the-row-events)
- [Conditional blocks](#conditional-blocks)
- [What the player can see](#what-the-player-can-see)
- [The castle household's day](#the-castle-households-day)
- [Other timed events](#other-timed-events)
- [Open questions](#open-questions)

## Game time

**Both.** The game clock (`game_clock` in the player record) counts 256 units a second. It runs only while the 3D view is running, and it moves on by at most 0x40 units (a quarter of a second) per frame, so a slow frame loses time. An hour is 0xE1000 units, so an hour of game time is an hour of play. A step key moves it on by 0x40.

The player's slow updates (`duration_check`) run once every 20 seconds of game time. They are counted 1, 2, 3 ... and the count drives everything slower:

| Every | UW1 | UW2 |
| --- | --- | --- |
| update (20 s) | active spells, lights, mushrooms, regeneration spells, drowning | the same, plus Killorn's countdown and the dream plant's counter |
| 3rd update (1 minute) | poison, a mana roll | the same |
| 24th update (8 minutes) | hunger, sobering, wandering monsters, critters' checkup, fatigue, a hit point roll | (not used) |
| 30th update (10 minutes) | (not used) | the same list as UW1's 24th |
| 60th update (20 minutes) | (not used) | the time of day moves on one step and the day's schedule runs |

[player-upkeep.md](player-upkeep.md) describes the player's side of these. The two uses that concern the world are the wandering monsters and the critters' checkup, below, and the time of day.

**UW2.** A new game starts at clock 0x465000, which is 5:00 in the morning of day 1, X clock 0 step 15. **UW1** starts at 0x10B3000.

Source: `duration_check` UW2 [PLAYTIME.C:71](../../uw2/src/game/PLAYTIME.C#L71), its caller [INTERACT.C:118](../../uw2/src/ui/INTERACT.C#L118), the clock [PLAYMOVE.C:298](../../uw2/src/motion/PLAYMOVE.C#L298); UW1 [PLAYTIME.C:128](../../uw1/src/game/PLAYTIME.C#L128); new game UW2 [CHARGEN.C:84](../../uw2/src/game/CHARGEN.C#L84).

## The X clocks

**UW2.** The player record holds 16 X clocks, one byte each (`xclock[16]` in [player.h](../../uw2/src/include/player.h)). Each schedule block in `SCD.ARK` follows the X clock with its number.

| Clock | Meaning | How it moves |
| --- | --- | --- |
| 0 | the time of day, 72 steps of 20 minutes, wrapping | one step every 60 slow updates; set from the game clock (`game_clock / 0x4B000 % 72`) when time passes by sleep or a world event |
| 1 | the castle plot | conversations and traps |
| 2 | the blackrock gems treated (Nystul) | conversations and traps |
| 3 | the djinn capture | conversations and traps |
| 14 | the most pit fighters beaten | the Pits of Carnage |
| 15 | a counter of plot events | many plot events add 1 so the schedules notice |

The other clocks are set by conversations (`x_clock`) and traps. When a conversation sets clock 0, the game clock moves by the same number of 20 minute steps (0x4B000 units a step).

In the shipped `SCD.ARK`, only blocks 0, 1, 14 and 15 have rows: 51, 78, 6 and 84 rows (measured, see [FORMATS.md](../FORMATS.md#scdark)).

Source: [player.h](../../uw2/src/include/player.h), `pass_time` ([WORLDEV.C:163](../../uw2/src/event/WORLDEV.C#L163)), `x_clock` ([BABLHACK.C:531](../../uw2/src/conv/BABLHACK.C#L531)).

## How a schedule runs

**UW2.** A schedule block is a list of 16-byte rows sorted by time. Each row has a time (a value of its X clock), a level, a "once" flag, an event code and 11 bytes of parameters. Each block also keeps a clock for each of the 80 levels: the time that level has reached on this schedule, and the next row that level has not looked at yet.

**When schedules run.** All 16 blocks are brought up to date (`Sched_SetAllClocks`) at these moments:

- when a conversation ends;
- when the player enters a level, or a saved game is restored;
- when time passes by sleep or a world event (`pass_time`);
- when hack trap 37 fires;
- when Killorn Keep crashes, unless the player is entering the level.

Separately, every 60 slow updates the time of day moves on one step and block 0 alone is brought up to date.

**Bringing one block up to date,** for the level the player is on:

1. The block's clock for this level is set to the block's X clock. Block 0 uses the time of day and wraps. The others take the X clock as it is.
2. If the clock went forward, every row from this level's next row whose time is at or before the new clock time is offered to the event handler in order. If the clock went back, the level starts again from row 0. Block 15 always starts again from row 0.
3. **Block 0 wraps.** If the time of day is now earlier than this level's clock (a new day started), the rows up to the end of the day (step 71) run first, then the level's clock goes back to 0 and the rows up to the new time run. A level the player left for more than a day still runs each row at most once on returning.
4. A row runs only if its level byte matches: the current level number, 0xFF for any level, or 0xF6 + n for every level of world n (worlds are 8 levels, so world n is levels 8n + 1 to 8n + 8). A row with a negative event code is disabled and skipped. An event code of 12 or more stops the block with an error.
5. A row with the "once" flag is deleted after it runs.
6. If anything changed, the block is saved back to the save directory's `SCD.ARK`.

So each level keeps its own place in each schedule. A level the player is not on does not change. When the player arrives, its schedules catch up on everything that fell due while he was away, in order.

Source: `Sched_SetAllClocks`, `Sched_SetTime`, `Sched_WrapTime`, `FindSCDRowsToExecute` ([SCHEDULE.C:204](../../uw2/src/event/SCHEDULE.C#L204), [SCHEDULE.C:169](../../uw2/src/event/SCHEDULE.C#L169), [SCHEDULE.C:187](../../uw2/src/event/SCHEDULE.C#L187), [SCHEDULE.C:56](../../uw2/src/event/SCHEDULE.C#L56)), `Sched_DoEvent` ([SCDEVENT.C:488](../../uw2/src/event/SCDEVENT.C#L488)); callers [CONVERSE.C:238](../../uw2/src/conv/CONVERSE.C#L238), [GAMEWRAP.C:402](../../uw2/src/game/GAMEWRAP.C#L402), [WORLDEV.C:168](../../uw2/src/event/WORLDEV.C#L168), [TRIGGER.C:873](../../uw2/src/event/TRIGGER.C#L873), [WORLDEV.C:2327](../../uw2/src/event/WORLDEV.C#L2327), the hourly step [PLAYTIME.C:155](../../uw2/src/game/PLAYTIME.C#L155).

## Which critters a row acts on

**UW2.** The events that act on critters (change goal, teleport, kill, remove, set attitude, and hacks 1, 2, 6 and 7) name them with a word. Its low byte is the mode and its high byte the value:

| Mode | Acts on |
| --- | --- |
| 0 | every active critter whose conversation number (whoami) is the value |
| 1 | every active critter of race value that is not a loner |
| 2 | the object with index value |
| 3 | every active critter |

"Active" means on the level the player is on.

Source: `gronk_critid`, `gronk_race`, `gronk_all_critters` ([SCDEVENT.C:86](../../uw2/src/event/SCDEVENT.C#L86), [SCDEVENT.C:42](../../uw2/src/event/SCDEVENT.C#L42), [SCDEVENT.C:64](../../uw2/src/event/SCDEVENT.C#L64)), `gronk_whoami` ([SPELLS.C:703](../../uw2/src/combat/SPELLS.C#L703)).

## The row events

**UW2.** Offsets are bytes within the 16-byte row.

| Code | Event | What it does |
| --- | --- | --- |
| 0, 6 | nothing | |
| 1 | change goal | The critters (word at 5) get goal byte 7 with goal target byte 8. If the critter was guarding, guarding is remembered as its old goal ([npc-ai.md](npc-ai.md#the-goals)). |
| 2 | teleport | Each critter (word at 7) is moved on its own level to square (byte 5, byte 6), unless the player could see the critter or the destination (below). Byte 10 set moves it even when seen. Byte 11 set makes the destination its home. If it could not move and byte 12 is above 0, a one-off copy of the row is added to block 0 for the time of day plus byte 12 (modulo 72), so it tries again then. Byte 9 is passed on but not used. |
| 3 | kill | The critters (word at 5) die at once. If byte 7 is 0, the NPC the player is talking to is spared. In that case, if byte 8 is above 0, the row is not a "once" row and byte 6 of the schedule work area is 15, X clock 15 goes up by 1. What that last test was meant to see is an open question ([FINDINGS.md](../../uw2/docs/FINDINGS.md)). |
| 4 | set a quest bit | Quest flag byte 5 is set to byte 6 (0 or 1). |
| 5 | fire triggers | Every scheduled trigger (trigger minor class 0xC) on square (byte 5, byte 6) of the level is set off, as by nobody. |
| 7 | special case | Byte 5 picks a hack, below. |
| 8 | set attitude | The critters (word at 5) get attitude byte 7 (0 hostile to 3 friendly). Any attitude but hostile also makes them forget who last hit them. |
| 9 | set a variable | Numbered variable (word at 5) is changed by operation byte 7 with value (word at 8), as a set-variable trap does. |
| 10 | test variables | Starts a conditional block (below). |
| 11 | remove | The critters (word at 5) are deleted from the map. |

**The special cases of event 7** (byte 5):

| Hack | What it does |
| --- | --- |
| 0, 5 | nothing |
| 1 | For each critter (word at 6) standing on square (byte 8, byte 9), numbered variable (word at 10) is changed by operation byte 12 with value (word at 13). The name `gronkify_garg` suggests the gargoyle (inferred). |
| 2 | For each critter (word at 8): if square (byte 6, byte 7) is (0, 0), or the critter stands on it, the critter is moved to a random free square of the rectangle (byte 10, byte 11) to (byte 12, byte 13), which becomes its home. It tries as many times as the rectangle has squares. The name suggests the soldiers (inferred). |
| 3 | Every tile of the level whose floor texture is byte 6 gets, with chance 1 in 2, floor texture byte 7 and its height raised by the word at 8. The name suggests the ice caverns freezing over (inferred). |
| 4 | Doors close (below, [Other timed events](#other-timed-events)), with byte 6 as the "only where unseen" flag. |
| 6 | The critters (word at 6) turn to face coarse heading byte 8 (eighths of a turn). The name says Nystul. |
| 7 | The critters (word at 6) are moved to the first free square of each row of the rectangle (byte 8, byte 9) to (byte 10, byte 11). The search stops only the inner loop, so the critter ends on the last row of the rectangle that had a free square, and its home is set to the square after the loops end, not where it stands. The name says Mors Gotha. Whether the result is what was meant is not known. |

**Numbered variables** are shared with traps and conversations: 0 to 0xFF the player's game variables, 0x100 to 0x17F one quest flag each, 0x180 to 0x18F the quest bytes, 0x190 to 0x19F the X clocks. The operations are:

| Op | Result |
| --- | --- |
| 0 | add |
| 1 | subtract |
| 2 | set |
| 3 | and |
| 4 | or |
| 5 | xor |
| 6 | shift left |
| 7 | value + 1 if the value equals the operand, else 0 |

A quest flag takes these rules instead: op 5 toggles it, op 1 leaves it as it is, and any other op sets it to 1 if the operand is above 0, else 0. A game variable is stored as a byte, so a result is kept modulo 256.

Source: the handlers ([SCDEVENT.C:99](../../uw2/src/event/SCDEVENT.C#L99) to [SCDEVENT.C:482](../../uw2/src/event/SCDEVENT.C#L482)), the tables ([SCDEVENT.C:510](../../uw2/src/event/SCDEVENT.C#L510)), the row layout ([event.h:21](../../uw2/src/include/event.h#L21)), `do_math_op` and `set_numbered_variable` ([TRIGGER.C:151](../../uw2/src/event/TRIGGER.C#L151), [TRIGGER.C:169](../../uw2/src/event/TRIGGER.C#L169)).

## Conditional blocks

**UW2.** Event 10 makes the rows that follow it conditional. Its parameters are a first variable (word at 5), a count (byte 7), an operation (byte 8), an invert flag (byte 9) and a value (word at 10).

1. The game reads `count` numbered variables starting at the first, and combines them in order with the operation: `v = var[0]`, then `v = op(v, var[i])` for each next one.
2. Without invert, the test passes when `v` equals the value. With invert, it passes when `v` differs.
3. If the test fails, nothing more happens. The disabled rows after it stay disabled, and the main loop skips them.
4. If the test passes, each following row with a negative event code is enabled, run, and disabled again, in order. A "once" row deletes itself instead of being disabled again. The block ends after the next row that is a disabled test (event -10), which is itself run, so blocks can follow one another.

In the shipped `SCD.ARK` every test row has count 1, operation 0 and invert 0, and compares with a value from 0 to 6 (measured). So in practice each test asks whether one variable equals a number.

Source: `ev_checkvar` ([SCDEVENT.C:446](../../uw2/src/event/SCDEVENT.C#L446)).

## What the player can see

**UW2.** Teleports by schedule, the castle household's moves and door closing avoid changing what the player is looking at. Two tests are used.

- **Seen** (`player_looking`) holds for a point when it is within 8 tiles of the player on both axes (each of |dx| and |dy| below 8) and the direction to it is within one eighth of a turn of the player's facing, on the coarse 8 directions. Walls are not considered.
- **Near** (`check_alert`) holds when the point is within 8 tiles on both axes. Door closing and wandering monsters use this one, and a caller can turn it off.

Source: `player_looking` ([SCDEVENT.C:120](../../uw2/src/event/SCDEVENT.C#L120)), `check_alert` ([TRIGGER.C:1020](../../uw2/src/event/TRIGGER.C#L1020)).

## The castle household's day

**UW2.** Lord British's household on level 1 moves round the castle by the time of day. Hack trap 36 runs the round. In the shipped data a scheduled trigger on level 1 sets it off from block 0 every six steps of the day clock, that is every two hours, while the player is on level 1 ([FINDINGS.md](../../uw2/docs/FINDINGS.md), measured).

Each time it runs:

1. Nothing happens unless quest 109 is set (the player has spoken to Lord British, inferred from FINDINGS) and the castle clock (X clock 1) is below 16.
2. Each castle person, conversations 0x81 to 0x8F and then 0xA8, picks a place, in that order. These are skipped: Lord British (0x8E) once quest 112 is set, 0x8D once the castle clock reaches 8, and 0x8B and 0x8C once it reaches 11.
3. The place becomes the person's home and the person's goal becomes 1, go home.
4. If the player can see neither where the person is nor the middle of the destination tile, the person is moved there at once.

**Choosing the place.** The hour is `game_clock / 0xE1000 % 24`, and the stage of the day is `((hour + 1) / 2 + 11) % 12`. Stage 11 is 11 pm to 1 am, stage 0 is 1 to 3 am, and so on in two-hour stages. The game picks a random place 0 to 5. Then, one time in three, and always at stages 0 to 2, the stage decides instead:

| Stage | Place |
| --- | --- |
| 0, 1, 2, 11 (11 pm to 7 am) | 0, the person's own spot |
| 3, 5, 9 | 4 |
| 6, 7, 8 (11 am to 5 pm) | 1 |
| 4, 10 | the random place |

Some people always go to their own spot: Miranda (0x88), everyone while the castle clock is 0, Lord British once bit 3 of quest variable 28 is set (quest 115), and Nystul (0x82) once the castle clock reaches 12.

| Place | Where |
| --- | --- |
| 0 | the person's own spot, from a table by conversation number (0xA8 has 0x2A, 0x24) |
| 1 | a random square of x 0x1B to 0x23, y 0x22 to 0x25 |
| 2 | a random square of x 0x1E to 0x20, y 0x27 to 0x2B, moved one east if it lands on 0x1F, 0x29 |
| 3 | a random square of x 0x1D to 0x21, y 0x2D to 0x33 |
| 4 | stays on the tile where it stands (the code sets a rectangle x 0x23 to 0x2F, y 0x2F to 0x34 but does not use it) |
| 5 | stays at its current home |

The own spots, for conversations 0x82 to 0x8F in order, are x 0x2A, 0x24, 0x15, 0x25, 0x16, 0x19, 0x1B, 0x2C, 0x2B, 0x16, 0x15, 0x18, 0x1A, 0x19 and y 0x2B, 0x33, 0x2A, 0x23, 0x33, 0x2B, 0x24, 0x30, 0x31, 0x25, 0x22, 0x27, 0x30, 0x22. Conversation 0x81 has no entry and reads past the start of the tables, giving x 0x22 and y 0.

The room names in comments of other projects (lobby, throne room, kitchens) were not checked against the map here.

Source: `move_folks_around` ([WORLDEV.C:1161](../../uw2/src/event/WORLDEV.C#L1161)), `maybe_go_hang_out`, `where_shall_we_hang_out` ([CRITTIME.C:617](../../uw2/src/critter/CRITTIME.C#L617), [CRITTIME.C:646](../../uw2/src/critter/CRITTIME.C#L646)).

## Other timed events

**Wandering monsters (both).** Every 24 slow updates in UW1 and every 30 in UW2, with chance 1 in 4, the game runs every create-object trap on the level that has no trigger left pointing at it and whose template is a mobile object (a critter, in practice), unless the trap's square is near the player (within 8 tiles on both axes). The template is marked temporary first, so the critter it makes is temporary and sleep removes it again (inferred from the copy). Sleeping also runs them, without the nearness test.

**Critters' checkup (both).** At the same moment, every mobile object gets its "fed" bit set at random and loses its ally bit three times in four.

**Doors close (both).** Each open door on the level, on a tile whose door bit 1 is clear, closes with chance 3 in 10, unless it is near the player. **UW1** closes doors this way whenever the player sleeps, without the nearness test. **UW2** does it only by schedule (hack 4 above).

**Timer triggers (UW2)** fire on their own clock while the player is near. They are described with the traps.

Source: `DoWanderingMonsters`, `DoClosingDoors` ([TRIGGER.C:1033](../../uw2/src/event/TRIGGER.C#L1033), [TRIGGER.C:1054](../../uw2/src/event/TRIGGER.C#L1054)), `yearly_checkup` ([CRITTIME.C:76](../../uw2/src/critter/CRITTIME.C#L76)), [PLAYTIME.C:143](../../uw2/src/game/PLAYTIME.C#L143), sleep [SKILLS.C:506](../../uw2/src/game/SKILLS.C#L506); UW1 [TRIGGER.C:656](../../uw1/src/event/TRIGGER.C#L656), [TRIGGER.C:677](../../uw1/src/event/TRIGGER.C#L677), [PLAYTIME.C:128](../../uw1/src/game/PLAYTIME.C#L128), [SKILLS.C:543](../../uw1/src/game/SKILLS.C#L543).

## Open questions

- Event 3's test of byte 6 of the work area, and `Sched_InsertLong`'s index fix-up, are listed in [FINDINGS.md](../../uw2/docs/FINDINGS.md) as probable slips.
- Hack 7's loop (above) moves the critter row after row. Whether Mors Gotha's rows in the data make this visible was not checked.
- What the plot meaning of each shipped row is was not traced here. The rows can be listed with a short script from the layout in [FORMATS.md](../FORMATS.md#scdark).
