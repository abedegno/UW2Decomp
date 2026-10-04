# The game: start-up, the main loop, the player and saves

This page describes how UW1 starts and runs, the player record and its rules (skills, experience, mantras, sleep, death, the ending), the passage of time, and saving, restoring and changing level. The sources are in `src/game`; the declarations, `struct Player` and the player's names (skills, classes, quests, movement state) are in `src/include/player.h`. The candidates for the findings page are in [game-findings.md](game-findings.md).

## Files

| File | Segment | What it does |
|---|---|---|
| [`UWEDIT.C`](../../src/game/UWEDIT.C) | ovr109 | `main`, start-up and shutdown, the screen dispatcher, moving the player, resetting after death or a restore, reading `UW.CFG` |
| [`MAINLOOP.C`](../../src/game/MAINLOOP.C) | seg011_1CA3 | the main loop and the change dispatcher (`do_changes`, `editchng`) |
| [`PLAYER.C`](../../src/game/PLAYER.C) | ovr134 | the player object, the key and mouse bindings, the 3D view's regions, the camera, the moongate vortex |
| [`PLAYDATA.C`](../../src/game/PLAYDATA.C) | ovr133 | `PLAYER.DAT` reading and writing, `FixPlayerEquips` and the spell effects, the maze spell |
| [`SKILLS.C`](../../src/game/SKILLS.C) | ovr143 | skills and levels, mantras, the end-game statistics, sleep and dreams, eating, the silver tree, death, the ending, traps on objects |
| [`SKILLCHK.C`](../../src/game/SKILLCHK.C) | seg037_32E6 | `skill_check`, experience, the stats panel's numbers |
| [`PLAYTIME.C`](../../src/game/PLAYTIME.C) | seg028_2985 | the timed updates: active spells, lights, poison, hunger, drunkenness, drowning |
| [`CHARGEN.C`](../../src/game/CHARGEN.C) | ovr098 | character creation |
| [`GAMEWRAP.C`](../../src/game/GAMEWRAP.C) | ovr140 | the save slots, saving and restoring, loading and saving a level, the per-level special cases |
| [`OVR126.C`](../../src/game/OVR126.C) | ovr126 | the credits |

UW1 has no build with symbols. The names are UW2's (FM Towns) where the routine is the same; the functions UW1 alone has (mantras, the silver tree, the ending) have descriptive names chosen to reproduce the EXE's overlay stub order. The file names are UW2Decomp's, with their evidence there.

## Start-up and the main loop

`main` (`UWEDIT.C`) calls `init_world`, which starts every subsystem in UW2's order (memory, data directories, strings, the map, input, `UW.CFG`, sound, timers, graphics with the `PRES1.BYT` and `PRES2.BYT` pictures, the mouse, objects, the 3D view, the player, critter AI, a blank character, lighting, the `SAVE0` working directory, which needs 0x9B0A0 bytes free, a working copy of `LEV.ARK`, conversations). It plays the title (cutscene 9), runs the start menu (`ui/MAINMENU.C`'s `real_start`) and then `mainloop` until `notdone` is cleared; Alt+X clears it at once.

`mainloop` alternates `do_changes` and `input_dispatch`. Work for the next pass is requested with `editchng(bits)`; `do_changes` runs `editor_dispatch[scrnum][bit]` for each bit set. There are three screens (0 the 3D view, 1 the automap, 2 a conversation), each with sixteen handlers, 0 starting the screen and 15 ending it; `change_state` keeps bits 11 to 13 of the 3D view (the physics clock, `display_scr`, `update_screen`) set every frame. UW1's 3D view runs `SKILLS.C`'s `check_victory` as handler 10.

Moving the player (traps, spells, moonstones, death, the ending) only sets `NewPlayerX`, `NewPlayerY` and `NewPlayerLevel`; `new_player_pos`, handler 3, does the move on the next pass, changing level through `GAMEWRAP.C`'s `ChangeLevel`, running `npp_func` on arrival (`do_mstone`, `do_resurrect`), and killing the player if no free square is found near the target.

## The player record

`struct Player` (`player.h`, 0xD2 bytes) is stored in `PLAYER.C`'s `PlayerDat` and reached through `player`; the player is also the critter `ThePlayer` (`critdata[1]`) with the creature record `playerdat` (`Creature[63]`, whose `attr[]` are strength, dexterity and intelligence). It holds the name, attributes, the 20 skills, hit points and mana, level and experience, the three active spells, the rune bag and shelf, the moonstone and silver tree levels, the quest flags (`QUEST_*`, `QB_CRUX`), the talismans left, the dreams seen, 64 game variables, the options, the movement state, the last critter hit and the game clock.

`PLAYER.DAT` is one key byte (the first letter of the name xor 0xAA) and the record xor-encoded with it, then the inventory (`inv/INVSAVE.C`). Before writing, the values kept elsewhere during play are copied in; after reading, out.

`FixPlayerEquips` recomputes everything that depends on equipment and spells: armour by hit location, defence (the defence skill plus half the weapon skill), the weapon animation, the brightest light, the effects of active spells and enchanted worn items, stealth (`plyNotice`), the light level, mushrooms, and the dragon skin boots.

## Rules found in the code

- **Skill checks.** `skill_check(value, target)` rolls `value - target + rand() % 31`: over 28 gives 2, over 15 gives 1, over 2 gives 0, else -1.
- **Attributes and maxima.** Vitality is 30 + level * strength / 5; maximum mana (mana skill + 1) * intelligence / 8; carrying capacity strength * 20 (`player_compute`). Skills are governed by strength (attack to missile), intelligence (mana, lore, casting) or dexterity (the rest) (`prime`).
- **Experience.** A gain is ignored once experience passes 0x17700, and halved (plus one) when the player's level is above twice the dungeon level plus two. Every 3000 of total experience gives a skill point. Levels follow `level_table` (in units of 500, up to level 16), each level giving one skill point (`player_get_exp`, `advance`). Death costs an eighth of the experience.
- **Raising a skill.** `get_skill` spends a skill point: +1, +1 more if the attribute is not strength and half of it is still above the skill, and +1 more by chance while below the attribute; never past twice the attribute or 30.
- **Mantras.** At a shrine the player types a mantra (string block 2 from 0x33): each skill's own mantra spends a point on two raises of that skill; INSAHN gives the cup of wonder's whereabouts, FANLO the key of truth (once), and SUMM RA, MU AHM and OM CAH spend a point on raises of up to three combat skills, two magic skills (mana favoured below 8) or four of the others (`mantra_advance`).
- **Sleep.** Refused while swimming, on lava, in the air, on level 9 or with hostile critters within 2 tiles. 2 to 6 hours pass, then a monster may interrupt; otherwise 7 to 10 in all, healing by fatigue and hunger, with a chance of a dream (`player_sleep`, `dream`). Garamon's dreams come in order (0, 1 once off level 1, 2 and 3 when their bits are set) and then at random, until Garamon is buried.
- **Death.** While talismans remain, a bones object is left, and with a silver tree planted (not on level 9) the player comes back at the tree with nearly full hit points; otherwise the game ends. Once the last talisman is gone the player cannot die (`player_is_dead`).
- **The silver tree** takes root only on an open square whose floor texture is one of four ranges (`plant_seed`).
- **The ending.** When the last talisman is destroyed, a moongate opens at the centre of the level, the Slasher of Veils is dragged through, and the player follows to level 9 (27, 23). When the game is won, the cutscene, the end pictures and the statistics are shown (`check_victory`, `game_stats`); the days in the Abyss are the game clock over 12 * 0x1C2000, which is 24 of the hours sleep adds (0xE1000 each).
- **Per-level cases** (`do_level_hacks`): on level 7 the player has no mana while Tybal's orb stands (the maximum is kept aside and a quarter of it given back on leaving); on level 9 the automap is off. After Armageddon, arriving on any level clears its objects.
- **Timed updates** (`duration_check`, once a tick of the slow clock): active spells run down (levitate and fly turn into slow fall for one more tick), lights burn, poison does its strength in damage every third tick and wears off, hunger falls and drunkenness wears off every 24th, with a 1 in 4 chance of wandering monsters; a swimmer weighed down sinks and drowns (`sink_sink_sink`).
- **A new character** starts at level 1 with one skill point, the moonstone on level 2, eight talismans to destroy, 3d4 in every skill and 2d10 + 10 in every attribute before the class's choices (`init_char`).

## Open questions

- Several record bytes have no known meaning (`b3C`, `bB1`, `bCA`), and game variable 0x1A starts at 0x35 without a known reason.
- `ovr112_389` (copying the change bits) and `MaybePlayerDayLoadrelated_ovr142_0` (empty) have overlay stubs but no caller in the matched C.
