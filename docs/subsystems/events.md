# Events: schedules, triggers, traps and world events

This page describes how UW2 makes things happen in the world: triggers and the trap chains they run, the SCD schedules that move NPCs and the plot along by the clocks, and the world events and quest special cases behind them. The sources are in `src/event`; the declarations are in `src/include/event.h`, with the trap types in `items.h`. The timer list is in `obj/EFFECT.C` and the quest variables in `player.h`.

## Files

| File | Segment | Name | What it does |
|---|---|---|---|
| [`TRIGGER.C`](../../src/event/TRIGGER.C) | ovr166 | inferred (System Shock's `TRIGGER.C`) | triggers and trap chains, every trap type, the hack traps' dispatch, numbered variables, wandering monsters, closing doors, pressure plates, bridges |
| [`TRIGSAVE.C`](../../src/event/TRIGSAVE.C) | ovr162 | descriptive | writing the trigger type table |
| [`SCHEDULE.C`](../../src/event/SCHEDULE.C) | ovr151 | inferred (the `Sched_` prefix; System Shock's `SCHEDULE.C`) | the schedules in `SCD.ARK`, their clocks, running the rows that fall due |
| [`SCDEVENT.C`](../../src/event/SCDEVENT.C) | ovr113 | descriptive | what each schedule row does |
| [`WORLDEV.C`](../../src/event/WORLDEV.C) | ovr110 | descriptive | teleports, terrain changes, the plot's special deaths, repair, fishing, and the hack traps of particular places |

Function and global names are the FM Towns originals where that build has them. `map/filenames.tsv` has the evidence for each file name.

## Triggers and traps

Triggers and traps are objects of major class 6 (items 0x180 to 0x1BF): minor classes 0 and 1 are traps, 2 and 3 triggers. A trigger lies on a tile or inside an object; its quality and owner fields name a target square and its link a trap there. The game reports an action with `UseTrigger(who, object, trigger, mode)`. The mode of each kind of trigger is in `Triggers[]`, the trigger type table of `OBJECTS.DAT` (the Guide's "Trigger Type Table"): 0 move, 2 pick up, 4 use, 5 look, 6 enter, 7 pressure, 8 open, 9 close, 0xA timer, 0xB unlock, 0xC scheduled, 0xE exit, 0xF pressure release. A trigger of the matching mode runs its trap, and each trap passes on to the object its link names, so traps form chains.

Who may set a trigger off is in its id bits: the player only with `ID_FLAG11`, critters only with `ID_ENCHANT`, other objects only with `ID_FLAG9`; a look trigger above the floor also needs a Search check against its height. A trigger without `ID_FLAG10` is used up: its chain is deleted after it runs. A trap knows how many triggers point at it (its flags), and deleting the last of them deletes the trap.

Where the actions come from: using, looking at and picking up objects (`obj/OBJUSE.C`'s `checkTrap`, `ui/INTERACT.C`), doors opening, closing and being unlocked (`obj/USEITEMS.C`, `obj/EFFECT.C`), stepping on squares and pressure plates (`check_pplate`, from the motion code), the timer list (`obj/EFFECT.C`: a timer trigger fires every z + 1 ticks while the player is within 8 squares of its target) and the schedules (`SCDEVENT.C`'s event 5).

The trap types (`items.h`'s `TRAP_*`; `UseTrap` has a comment for each):

- damage (negative damage poisons), arrow (a missile of item quality * 32 + owner), special effect (screen flashes and shakes, sounds), spell (`inanimate_spell`), ward (a rune of warding);
- teleport (to a square and level, with a facing), jump;
- change terrain, change-from and change-to (every matching tile in an area gets new values), oscillator (a tile's height, floor or wall steps up and down), pit, bridge (lays or removes a row of bridges);
- create object (copies its template onto the square with a chance; a template "adventurer" is a random monster scaled by the world, the castle plot and the player's level), delete object, door (open, close or toggle, and change the lock);
- set variable, check variable, skill check, proximity, inventory check: the last four are conditions that branch to the object after the trap's link instead of continuing;
- experience (may be negative), text string (block 9), and hack (a special case by number, below).

## Numbered variables

Traps, schedules and conversations share one numbering of the game's variables (`set_numbered_variable`, `get_numbered_variable`): 0 to 0xFF the player's `vars[]`, 0x100 to 0x17F one bit each of the quest variables (bit `n & 3` of `quests[n / 4]`), 0x180 to 0x18F the quest bytes (quests 128 to 143), 0x190 to 0x19F the X clocks. The operations (`do_math_op`) are add, subtract, set, and, or, xor, shift left, and "count up while equal" (value + 1 if it equals the operand, else 0).

## Schedules

`SCD.ARK` (in the save directory) has 16 blocks, one schedule per X clock (`player->xclock[]`; `player.h` names `XC_TIME` the day clock, `XC_CASTLE` the castle plot, `XC_GEMS`, `XC_DJINN`, `XC_PIT_KILLS` and `XC_CHANGED`). A block is a list of 16-byte rows (`struct SCDRow`, `event.h`) sorted by time, and a table of 80 clocks, one per level, each recording the time that level has reached on this schedule and the next row to run. Bringing a schedule up to date runs every row whose time has passed, but only rows for the current level, its world (0xF6 + world) or every level (0xFF); a level the player is away from catches up when he returns.

`Sched_SetAllClocks` sets all 16 clocks from the X clocks (the day clock wrapping at 72 steps of 20 minutes); it runs after conversations, level changes, restores and X clock changes. `game/PLAYTIME.C` advances the day clock. A row that must move an NPC elsewhere is queued (`Sched_Migrate`) and added to block 0 when the block in hand is saved. Many plot events count up `xclock[XC_CHANGED]` so the schedules notice.

The row events (`SCDEVENT.C`): 1 change goal, 2 teleport (only where the player can see neither the NPC nor its destination, retrying later if it cannot), 3 kill, 4 set a quest bit, 5 fire the scheduled triggers on a square, 7 a special case (Nystul's heading, Mors Gotha's and the soldiers' moves, the gargoyle's variable, the ice caverns freezing over, doors closing), 8 set attitude, 9 set a variable, 10 test variables, 11 remove. Most act on a set of critters chosen by whoami, by race, one object or all. Event 10 makes a conditional block: the disabled rows after it (negative event codes) run only when the test passes. A row marked "once" deletes itself after running.

## World events

`WORLDEV.C` holds the general actions and the special cases of places and quests; its 68 functions are each commented with what they do. Some of the rules:

- **Teleports** (`do_teleport`) find the nearest free square to the destination (`find_good_x_and_y`). Only the player changes level, recorded for the main loop to perform. Leaving the jail cell while jailed sets quest 124 (an escape, inferred); leaving the arena mid-fight is running away.
- **Changing tiles** (`change_terrain`) moves everything on a tile, and objects overlapping from its neighbours, up or down with the floor; objects pushed into the ceiling are destroyed.
- **Plot deaths** (`death_check`). Before a critter with a conversation dies, and when it dies, its whoami decides: some set quest flags, some yield and talk instead of dying, the Listener can only be killed with Altara's dagger, Praecor Loth's death kills his liches, and the castle's people cannot be killed at all (the guards are called). The source lists every case with the NPC's name.
- **Repair** (`repair_item`) states the difficulty ("trivial" to "very difficult" by durability against skill), takes time, and can improve, do nothing, damage or destroy the item, by a skill check against the item's durability.
- **Fishing** (`go_fish`) needs water ahead and below; a catch depends on the Track skill.
- **The blackrock gem** (`black_gem_trip`, `black_gem_rotate`): each of its eight facets leads to a world, open once that world's key gem has been used on it, or when it is the facet the gem currently shows; the facet shown changes at random among the first 1, 3, 6 or 8 by the castle plot's stage.
- **Jail** (`put_player_in_jail`): a ninth of the experience lost, the player in the cell on level 1, the guards calmed.
- **The castle's day and decline** (`move_folks_around`, `courtyard_hacking`): Lord British's household moves round the castle by the time of day; as the castle plot advances the courtyard's plants wilt, the fountains dry and mushrooms spread.
- **Places**: the Pits of Carnage arena and its prize (`change_weapon_playerbest`), the Scintillus Academy's pillars, telekinesis wand and spoiled potions, Bliy Skup Ductosnore's chamber, the q*bert floor, vending machines (half the item's value plus one, in coins put on the machine), bottle returns, force fields and fraznium, Killorn Keep's crash.

## Open questions

- `gronkify_slay` (`SCDEVENT.C`) tests byte 6 of the schedule work area against 15 as if it were the block number; in `SCHEDULE.C`'s layout that byte belongs to the migration queue.
- `Sched_InsertLong` (`SCHEDULE.C`) moves the current level's next-row index once for each level whose clock has passed the new row, rather than each level's own; probably a slip.
- Several hack traps (`do_trap_hack`) are known only by their FM Towns names (`skup_ductosnore`, `do_qbert`); what the player sees has not been checked in the game.
- The names of the races used by the plot (0xB, 0x15, 0x17, 0x1C, 6) are taken from the owner race strings of block 1, assuming the race number and the owner field share a numbering.
