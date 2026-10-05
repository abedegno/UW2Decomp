# The game: start-up, the main loop, the player and saves

This page describes how UW2 starts and runs, the player record and its rules (skills, experience, sleep, death), the passage of time, and saving, restoring and changing level. The sources are in `src/game`; the declarations are in `src/include/player.h` and `src/include/sys.h`, with the save files' routines in `file.h`.

## Files

| File | Segment | Name | What it does |
|---|---|---|---|
| [`UWEDIT.C`](../../src/game/UWEDIT.C) | ovr112 | inferred (TLINK stored `uwedit.exe`; the file has `main`, `init_edit` and `editexit`) | `main`, start-up and shutdown, the screen dispatcher, moving the player, resetting after death or a restore |
| [`MAINLOOP.C`](../../src/game/MAINLOOP.C) | seg012 | inferred (System Shock's `MAINLOOP.C`) | the main loop and the change dispatcher (`do_changes`, `editchng`) |
| [`PLAYER.C`](../../src/game/PLAYER.C) | ovr143 | original (`init_player` is in System Shock's `PLAYER.C`) | the player object, key and mouse bindings, the 3D view's regions, the camera |
| [`PLAYDATA.C`](../../src/game/PLAYDATA.C) | ovr142 | descriptive | `PLAYER.DAT` reading and writing, `FixPlayerEquips` |
| [`SKILLS.C`](../../src/game/SKILLS.C) | ovr154 | descriptive | skills and levels, sleep and dreams, eating, the Ethereal Void, death, the ending, object traps |
| [`SKILLCHK.C`](../../src/game/SKILLCHK.C) | seg038 | descriptive | `skill_check`, experience, the stat panel's numbers |
| [`PLAYTIME.C`](../../src/game/PLAYTIME.C) | ovr135 | descriptive | timed updates: spells, lights, poison, hunger, the day clock |
| [`CHARGEN.C`](../../src/game/CHARGEN.C) | ovr101 | descriptive | character creation |
| [`GAMEWRAP.C`](../../src/game/GAMEWRAP.C) | ovr149 | original (`copy_file` is in System Shock's `GAMEWRAP.C`) | the save slots, saving and restoring, loading and saving levels, per-level special cases |
| [`CREDITS.C`](../../src/game/CREDITS.C) | ovr131 | descriptive | the credits |

Function and global names are the FM Towns originals where that build has them. `map/filenames.tsv` has the evidence for each file name.

## Start-up and the main loop

`main` (`UWEDIT.C`) calls `init_world`, which starts every subsystem in the order FM Towns has them, including `init_save` (the `SAVE0` working directory) and `init_player`. It plays the title (cutscene 9), runs the main menu (`ui/MAINMENU.C`'s `real_start`, which creates or restores a character) and then `mainloop` until `notdone` is cleared.

`mainloop` alternates two calls: `do_changes` and `input_dispatch` (`ui/INPUT.C`), which reads the mouse and keyboard and calls the current screen's handlers. Work for the next pass is requested with `editchng(bits)`, which sets bits in `changed`; `do_changes` runs `editor_dispatch[scrnum][bit]` for each set bit. There are three screens (`scrnum` 0 the 3D view, 1 the automap, 2 a conversation), each with sixteen handlers; handler 0 starts a screen and 15 ends it (`newscr`). `change_state[scrnum]` is or-ed into `changed` after every pass, so the 3D view's physics, `display_scr` and `update_screen` (bits 11 to 13, 0x3800) run every frame.

Anything that moves the player to another square or level (traps, spells, moonstones, the blackrock gem) only sets `NewPlayerX`, `NewPlayerY` and `NewPlayerLevel` and the change bit; `new_player_pos`, handler 3 of the 3D view, does the move on the next pass, through `GAMEWRAP.C`'s `ChangeLevel` when the level changes. `npp_func` can name a function to run on arrival (`do_gem`, `do_mstone`, `do_dreamret` in `SKILLS.C`).

## The player record

`struct Player` (`player.h`) is the player's record, `PlayerDat`, reached through the near pointer `player`; the player is also a critter, `ThePlayer` (`critdata[1]`), with the critter record `playerdat` (`Creature[63]`, whose `attr[]` are strength, dexterity and intelligence). The record holds the skills, attributes, experience, hit points and mana, the active spells, hunger, fatigue, drunkenness and poison, the rune bag and shelf, the moonstones, the quest variables (`quests[]`, `quest_bytes[]`) and the X clocks (`xclock[]`). `player.h` names the X clocks and quest bytes whose meaning is known (`XC_*`, `QB_*`).

`PLAYER.DAT` is one key byte (the first letter of the name xor 0xAA) and the 0x37D-byte record xor-encoded with it (`PLAYDATA.C`'s `xorwrite`, `xorread`), followed by the inventory (see [inventory.md](inventory.md)). Before writing, the values that live elsewhere during play (attributes, hit points, position, heading, level, sound settings) are copied in; after reading they are copied out.

`FixPlayerEquips` (`PLAYDATA.C`) recomputes everything that depends on equipment and active spells: armour by hit location (`cmbModTH`), defence, the weapon's animation, the brightest light carried, the effects of active spells and enchanted worn items, stealth and the light level. It runs after every change of equipment.

## Rules found in the code

- **Skill checks.** `skill_check(value, target)` is the one roll: `value - target + rand() % 31`; over 28 gives 2 (a critical), over 15 gives 1, over 2 gives 0, else -1. Combat, magic, traps, bartering, swimming, repair, lore and the timed updates all use it.
- **Experience** (`player_get_exp`). Gains are halved (rounding at random), and halved again (plus one) for a player whose level is above twice the world number plus 2, so lingering in early worlds pays less. Every 1500 points earn a skill point. Levels come at 500 times the values of `level_table` (1, 2, 3, 4, 6, 8, 12, 16, 24, 32, 48, 64, 96, 128, 192), so level 16 needs 96000; experience is capped at 0x7FFF0. The end-game screen shows experience divided by 10, so it is kept in tenths.
- **Derived values** (`player_compute`). Maximum hit points `30 + level * strength / 5`, maximum mana `(mana skill + 1) * intelligence / 8`, carrying capacity `strength * 13 + 300`.
- **Raising skills** (`get_skill`). A skill point raises a skill by 1, by 1 more if the governing attribute is not strength and half the attribute is still above the skill, and by 1 more with a chance (attribute - skill) / 25, 30 or 15 (by attribute); a skill above twice its attribute, or at 30, cannot rise. The attribute governing each skill (`prime`): strength for the combat skills 0 to 6, intelligence for mana, lore and casting, dexterity for the rest.
- **Time.** `game_clock` runs in 1/256 seconds (`WORLDEV.C`'s `pass_time` shifts seconds left by 8). `display_scr` (`ui/INTERACT.C`) calls `duration_check` every 20 seconds of game time; spells and lights run down every call, poison every 3rd, hunger, sobering, wandering monsters and the critters' yearly check every 30th, and every 60th (20 minutes) the day clock `xclock[XC_TIME]` moves one of its 72 steps and the day's schedule runs.
- **Sleep** (`player_sleep`). Refused while moving, swimming, fighting or in the Pits. A night is 7 to 10 hours, in two parts with a chance of a wandering monster between them; healing and mana come back by fatigue and comfort (fed and in a bed or bedroll), a starving player loses hit points instead, and hunger and drunkenness drop. Then the player may dream (`dream`): the castle story's dreams 0 to 3 come due as the castle plot (`xclock[XC_CASTLE]`) reaches 4, 6, 10 and 14, otherwise a random one of dreams 4 to 6; dream n is cutscene 0x18 + n. Having eaten the dream plant, the sleeper goes to the Ethereal Void instead (`go_void`).
- **Death** (`player_is_dead`). Killed in Britannia's level 1 by a castle guard: jail (`WORLDEV.C`'s `put_player_in_jail`). In the Ethereal Void: waking up. In the Pits: the fight is lost. Otherwise death costs a ninth of the experience, and outside Britannia the player wakes by the blackrock gem on level 5 (`do_gem`); in Britannia it is the end of the game.
- **The ending.** A conversation can leave a cutscene pending in quest byte 15; `cs_check` plays it after the conversation, and cutscene 2 is the ending (`player_won_game`). The statistics screen adds "AND CHEATED ON THEIR CHARACTER" when strength, dexterity and intelligence total more than 64, which character creation cannot produce.
- **Character creation** (`CHARGEN.C`). Eight questions (sex, hand, class, skills, portrait, difficulty, name, confirm). The class sets the attributes from `DATA\SKILLS.DAT` and spreads its bonus points over them (at most 30 each); then five skill entries per class, each a fixed skill or a choice, each raised by `add_to_skill`.
- **Traps on objects** (`DetectedTrap`, `RemoveTrap` in `SKILLS.C`): finding one is a Search check against 10 + twice the world number, disarming a Traps check against 8 + the world number; a bad failure sets it off.

## Saving, restoring and changing level

The game in progress lives in `UWHOME\SAVE0\` (`HomeDir`): at start-up the pristine `LEV.ARK` and `SCD.ARK` are copied there, and every level change writes the level left back into it (`SaveLevel`) and reads the next (`GetLevel`). A saved game is a copy of that directory in `SAVE1` to `SAVE4`, with a 30-character description in `DESC`. `SaveGame` writes `DESC`, `PLAYER.DAT` and the current level and copies `SAVE0` to the slot; `RestoreGame` copies the slot back, resets the player (`reset_game`), reads `PLAYER.DAT` and loads the level.

`ChangeLevel` runs `do_level_hacks` for the level left and the level entered, the place where several worlds' special rules live: the Prison Tower's alarm, Killorn Keep's crash, the Academy's telekinesis wand being taken back, the liches dying with Praecor Loth in the Tombs, and the automap being switched off in the Ethereal Void. It then brings the schedules up to date (`event/SCHEDULE.C`'s `Sched_SetAllClocks`; see [events.md](events.md)).

## Open questions

[FINDINGS.md](../FINDINGS.md) collects the open questions and likely bugs of every subsystem in one place.

- `player_sleep`'s motion states 1 and 2 (death when passing out) and 8 (a fall) are named only by what the code does with them; probably swimming and falling.
- The meaning of several `struct Player` fields named by offset (`b3C`, `b60_11`, `b62_5`); `b60_11` is set by Armageddon.
- Whether the hourly schedule (`Sched_WrapTime` in `PLAYTIME.C`) and `pass_time`'s `Sched_SetAllClocks` ever disagree about the day clock; both compute it from `game_clock`.
