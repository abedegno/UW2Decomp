# Events: triggers, traps and world events

This page describes UW1's triggers and traps and the world events behind them: teleports, terrain changes, the special-case "hack" traps, the deaths that matter to the plot, and repair. The sources are in `src/event`; the declarations are in `src/include/event.h`, and the trap and trigger types in `items.h`. The candidates for the findings page are in [events-findings.md](events-findings.md).

## Files

| File | Segment | What it does |
|---|---|---|
| [`TRIGGER.C`](../../src/event/TRIGGER.C) | ovr153 | running a trigger and its trap chain (`UseTrigger`, `SetOffTrap`, `UseTrap`), the hack trap dispatcher, removing triggers and traps, wandering monsters and closing doors, the trigger mode table |
| [`WORLDEV.C`](../../src/event/WORLDEV.C) | ovr107 | `do_teleport`, `change_terrain`, the damage and spell trap actions, fishing, the hack traps' handlers, plot deaths, repair, the Lore retries |
| [`TRIGSAVE.C`](../../src/event/TRIGSAVE.C) | ovr149 | writing the trigger mode table to a save file |

UW1 has no SCD schedules: `event.h` still carries UW2's schedule records, which no UW1 source uses. Names are UW2's (the FM Towns originals) where the routine is the same, else chosen for the overlay's stub order (Turbo C lists a file's publics by a hash of each name, and TLINK numbers the stub entries from that list). The file names are UW2Decomp's.

## Triggers and traps

Triggers and traps are objects of major class 6 (items 0x180..0x1BF): minor class 0 traps (17 types in UW1, `enum TrapType`), minor class 2 triggers (7, `enum TriggerType`). A trigger sits on a tile or inside an object; its quality and owner fields name a target square, and its link a trap there. The game reports an action with `UseTrigger(who, object, trigger, kind)`; a trigger whose mode (`Triggers[]`, from `OBJECTS.DAT`) matches the kind runs its trap. The player sets off a trigger only with `ID_FLAG11`, and for kind 5 (a search, inferred from Reveal's use of it) a Search skill check against the trigger's height; a critter only with the enchanted bit. A trigger without `ID_FLAG10` is used up: its trap chain is deleted after it runs, and every other trigger pointing at the trap (the trap's flags count them) is found and removed (`delete_trap`, which searches the whole map).

`UseTrap` performs one trap, then passes on to what its link names, so traps form chains:

- damage (the player hit, or healed, by quality), teleport (to square quality, owner of level z), arrow (`trap_fire`), the hack traps (by quality, below), change terrain, cast a spell;
- create object: a copy of the linked object with chance (63 - quality) in 63, not when another created object is within four squares (wandering monsters are made this way, `DoWanderingMonsters`: while the player sleeps, and now and then while he is awake where he is at least 8 squares off);
- door: open, close or toggle the door on the square, replacing its lock by a copy of the trap's;
- ward and tell (both item types act alike): hit the critter of the given class that stepped on it for 3 plus a random share of the player's Casting skill;
- delete object, inventory (the chain ends unless the player has the item, with z at least that many), set variable and check variable (z 0 the quest bits; else the player's 64 game variables, six bits each, by add, subtract, set, and, or, xor and shift), text string (block 9).

The hack traps (`do_trap_hack`) are UW1's special cases: the crystal ball, the eight-position switch (sets the height of a tile or object), a crime against an owner, the bullfrog puzzle, the emerald puzzle (four emeralds at the corners of a 9 by 9 square make a runestone), the exploding book, the talking door, critters waiting to talk, Arial's scene, the earthquakes, and the end of the game.

## World events

`do_teleport` (`WORLDEV.C`) moves a critter within the level, or the player within it or to another level (the main loop carries the player's move out). On the same level it finds room near the destination (`find_good_x_and_y`, a breadth-first search of the 9 by 9 square around it), unless x or y is 0x3F (gate travel passes 0x3F, 0x3F). `change_terrain` changes a tile's wall, floor, height and type and moves the objects standing on it. `death_check` handles the deaths that matter to the plot: Garamon's talisman bit, Tyball (his allies are removed and the move triggers on his square destroyed), a critter that surrenders and talks instead of dying, and the quest bits some deaths set. `repair_item` estimates the difficulty from the item's durability and the Repair skill, asks, takes time, and may destroy the item.

## Open questions

- Why the tell trap (0x18A) acts as a ward trap.
- `kill_triggers` writes to a trigger after freeing it (to stop the search descending into it); harmless with UW1's allocator, which keeps a freed record as it was.
