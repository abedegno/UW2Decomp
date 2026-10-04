# Events: candidates for the findings page

Candidates from `src/event` for the project's findings page; [events.md](events.md) describes the subsystem. Each candidate was re-read in the matched source; what the code does is read from the code, and the effect in the game is an inference unless it says it was checked.

## Engine findings

- UW1's trap and trigger items differ from UW2's: 17 traps (0x180..0x190) and 7 triggers (0x1A0..0x1A6), all of minor class 2; `items.h` names them from UW1's string block 4 (`enum TrapType`, `enum TriggerType`).
- The ward trap (0x189) and the tell trap (0x18A) are handled by the same case of `UseTrap`: both hit the critter that set them off. Why the tell trap does so is not known.
- A trap is deleted with every trigger that points at it: the trap's flags count its triggers, and `delete_trap` searches the whole map (4096 tiles) until the count reaches 0.
- `kill_triggers` writes to a trigger after freeing it, clearing its link so the search does not descend into it, and then reads its next link. With UW1's allocator a freed record keeps its contents, so this works; UW2 reads the next link before freeing.
- The variable traps work on the player record: z 0 the 32 quest bits (`1L << d`), else game variable z of 64, kept to six bits after each operation.

## Recovered rules

- Wandering monsters (`DoWanderingMonsters`, run by `SKILLS.C` when the player sleeps and by `PLAYTIME.C`'s regeneration tick one time in four) are the create-object traps (class 7, flags 0) whose template is a mobile, each run unless another created critter is within four squares; from the regeneration tick (`is_player` set) only where the player is at least 8 squares away, while he sleeps anywhere.
- When the player sleeps (`SKILLS.C`), open doors close by themselves with chance 3 in 10 each (`DoClosingDoors`), except on a tile whose door byte has bit 1, and the moving doors are run 8 frames on. (With `is_player` set, which no UW1 caller passes, only doors 8 or more squares from the player would close.)
- A create-object trap makes its copy with chance (63 - quality) in 63.
- The emerald puzzle: an emerald (0xA7) at each corner of the 9 by 9 square around the trap makes a Vas runestone (0xFD) beside it and destroys the four emeralds.
- Plot deaths (`death_check`): whoami 0x16 surrenders once (500 experience) and talks instead of dying; 0x0B talks and is left with 60 hit points; Tyball's death (0xE7) removes his allies and the move triggers on his square; the deaths of whoamis 0x6E, 0x8E and 0x18 set quests 4, 11 and 6.
