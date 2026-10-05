# Conversations

This page describes how a conversation in Ultima Underworld I and II runs: who will talk, how a conversation script runs, every built-in function a script can call, the variables handed to the script and back, and bartering. It is written for someone who wants to reproduce the conversation system or write scripts for it and will not read the C. Every rule comes from the matched sources of UW2Decomp and [UW1Decomp](https://github.com/abedegno/UW1Decomp).

Each rule is marked **both**, **UW1** or **UW2**. [UW1-UW2-DIFFERENCES.md](../UW1-UW2-DIFFERENCES.md#conversations) lists the differences in more detail. The script file format, `CNV.ARK`, is UW-Formats section 7; [FORMATS.md](../FORMATS.md) has corrections to it. Nothing here was checked in a running game.

**Argument numbering.** A built-in reads its arguments from the script's stack. Following UW-Formats, "arg1" is the last argument the script pushed before the count, "arg2" the one before, and so on. The order in which the original script source wrote them is not known. Every argument is passed as the address of a script variable, so a built-in can write results back into it.

## Contents

- [Who will talk](#who-will-talk)
- [A conversation from start to end](#a-conversation-from-start-to-end)
- [The script machine](#the-script-machine)
- [Text and variable substitution](#text-and-variable-substitution)
- [Variables in and out](#variables-in-and-out)
- [Built-ins: talking and menus](#built-ins-talking-and-menus)
- [Built-ins: strings and numbers](#built-ins-strings-and-numbers)
- [Built-ins: quests, variables, skills and time](#built-ins-quests-variables-skills-and-time)
- [Built-ins: critters, doors and objects](#built-ins-critters-doors-and-objects)
- [Built-ins: trading](#built-ins-trading)
- [How bartering works](#how-bartering-works)
- [The Pits of Carnage and other hacks (UW2)](#the-pits-of-carnage-and-other-hacks-uw2)

## Who will talk

**Both.** When the player talks to something, or a critter with goal 10 reaches the player ([npc-ai.md](npc-ai.md#the-goals)), the game decides in this order:

1. **UW1.** A shrine starts a mantra instead. A picture of the princess answers "There is no reaction from the princess." **UW2.** A wisp starts conversation 0x30 through a temporary voice.
2. Anything that is not a creature: "You cannot talk to that!"
3. **UW2.** A held critter (goal 15), or any critter while time is stopped: "You get no response."
4. Some conversations always talk: **UW1** 0x16, 0x8E and 0xE7, **UW2** 0x8C.
5. A critter with goal 10 (talk) always talks.
6. Otherwise a critter refuses ("You get no response.") when its conversation number is 0xFF, or when it is hostile or is attacking, fleeing or cornered with the player as its target, unless it is an ally.
7. The conversation is the critter's conversation number in `CNV.ARK`, or 0x100 plus its creature type for conversation 0 (a generic critter). If that conversation does not exist: "You get no response."

Source: `TalkTo` ([CONVERSE.C:95](../../src/conv/CONVERSE.C#L95)); UW1 [CONVERSE.C:117](https://github.com/abedegno/UW1Decomp/blob/main/src/conv/CONVERSE.C#L117).

## A conversation from start to end

**Both.**

1. The conversation screen opens. It shows the player's portrait and the NPC's (by conversation number, else by creature type) and names, and sets up the trade slots and the NPC's trading temper ([How bartering works](#how-bartering-works)).
2. The script is loaded, its built-ins are bound by name, and its saved private variables are read from the save's `bglobals.dat`.
3. The game's state is copied into the variables the script imports ([Variables in and out](#variables-in-and-out)).
4. An NPC without an inventory gets one generated.
5. The script runs until it exits.
6. The variables a conversation may change are copied back. The private variables are saved back to `bglobals.dat`, and strings the script made are dropped.
7. The last line stays on screen for a moment if the player spoke last, and not at all if the NPC spoke last or the script left the NPC hostile.
8. When the screen closes: anything left in the trade slots goes back to its owner (the player's things to the player, the NPC's to the NPC, at their feet if need be). **UW2** then brings the schedules up to date ([schedules.md](schedules.md#how-a-schedule-runs)), runs 8 animation steps, carries out any teleport the script asked for, and plays a cutscene the script left pending.

Source: `strt_converse`, `Converse`, `free_converse` ([CONVERSE.C:166](../../src/conv/CONVERSE.C#L166), [CONVERSE.C:256](../../src/conv/CONVERSE.C#L256), [CONVERSE.C:228](../../src/conv/CONVERSE.C#L228)), `load_script`, `bab_get_globals` ([BABL.C:367](../../src/conv/BABL.C#L367), [BABL.C:312](../../src/conv/BABL.C#L312)).

## The script machine

**Both.** A script is a list of 16-bit words run on a small stack machine. Its memory holds the script's variables (the imported ones, then its private globals) followed by a stack of 0x800 words. The machine starts at word 0, which must be START. It stops at EXIT_OP, at a RET with nothing on the stack, or at an unknown opcode. Nothing checks the stack or the program counter.

| Code | Name | What it does |
| --- | --- | --- |
| 0x00 | NOP | nothing |
| 0x01 to 0x05 | OPADD, OPMUL, OPSUB, OPDIV, OPMOD | pop b, pop a, push a + b, a * b, a - b, a / b, a % b. Division or remainder by 0 gives 0x7FFF |
| 0x06, 0x07 | OPOR, OPAND | logical: push 1 if a or b (a and b) is non-zero, else 0 |
| 0x08 | OPNOT | 1 if the top is 0, else 0 |
| 0x09 to 0x0E | TSTGT, TSTGE, TSTLT, TSTLE, TSTEQ, TSTNE | pop b, pop a, push 1 or 0 for a > b, >=, <, <=, ==, != |
| 0x0F | JMP n | go to word n |
| 0x10, 0x11 | BEQ n, BNE n | pop; branch by n (relative to the operand word) if it was 0 (BEQ) or not 0 (BNE) |
| 0x12 | BRA n | branch by n, relative to the operand word |
| 0x13 | CALL n | push the return address, go to word n |
| 0x14 | CALLI n | call built-in n (below) |
| 0x15 | RET | pop the return address, or end the script if the stack is empty |
| 0x16 | PUSHI n | push n |
| 0x17 | PUSHI_EFF n | push the address of frame slot bp + n |
| 0x18 | POP | drop the top |
| 0x19 | SWAP | swap the top two |
| 0x1A to 0x1D | PUSHBP, POPBP, SPTOBP, BPTOSP | frame pointer handling |
| 0x1E | ADDSP | pop n and reserve n words |
| 0x1F | FETCHM | replace the address on top with the value there |
| 0x20 | STO | pop a value and an address, store |
| 0x21 | OFFSET | pop an index and an address, push address + index - 1, so arrays count from 1 |
| 0x22 | START | marks the start |
| 0x23, 0x24 | SAVE_REG, PUSH_REG | copy the top to the result register, push the result register |
| 0x25 | STRCMP | pop two string ids, push 1 if the strings are equal after @-substitution, case sensitive |
| 0x26 | EXIT_OP | end the script |
| 0x27, 0x28 | SAY_OP, RESPOND_OP | pop a string id, substitute it, and pass it to "say" or "respond" |
| 0x29 | OPNEG | negate the top |

**Calling a built-in.** The script pushes the arguments' addresses and then their count, and executes CALLI with the built-in's number from its import table. The built-in's result replaces the count on the stack and goes into the result register. An import the game does not bind returns 0.

Source: `babl_run` and the opcode routines ([BABL.C:756](../../src/conv/BABL.C#L756) to [BABL.C:1142](../../src/conv/BABL.C#L1142)).

## Text and variable substitution

**Both.** Strings are string ids. Ids below 0x200 are the conversation's own block of `STRINGS.PAK` (block 0xE00 + the conversation number). Strings a script builds go into a dynamic block that is cleared when the script ends.

Before conversation text is shown, `@`-variables in it are replaced. Each is `@`, a kind letter, a type letter (except for C), a number and an optional index:

- Kind **G** reads global variable `number + index - 1`.
- Kind **S** reads stack slot `bp + number + index - 1`, a local of the current function.
- Kind **P** reads the address in stack slot `bp + number` (a parameter passed by reference), and the variable at that address `+ index - 1`.
- Kind **C** is the number itself.
- Type **I** shows the value as a decimal number. Any other type treats the value as a string id and shows that string, itself substituted.
- The index is another `@`-variable without the `@`, and only an integer one works.
- `@@` is a literal `@`.

Source: `convert_string`, `AtIndex` ([BABL.C:574](../../src/conv/BABL.C#L574), [BABL.C:646](../../src/conv/BABL.C#L646)).

## Variables in and out

**Both.** Before the script runs, the game fills in only the variables the script imports. Afterwards it copies back only the ones listed as "out".

| Variable | In | Out |
| --- | --- | --- |
| `npc_whoami` | conversation number | |
| `npc_hunger` | 0x10 if the NPC is fed, else 0xC0 | fed if below 0x20 |
| `npc_health` | `hp * 256 / creature hit points` (0x80 if those are 0) | |
| `npc_hp` | hit points | hit points |
| `npc_arms` | the creature's first attack chance | |
| `npc_power` | the creature's strength plus caster value | |
| `npc_goal`, `npc_gtarg` | goal and goal target | both, set together as by a spell or schedule ([npc-ai.md](npc-ai.md#the-goals)) |
| `npc_talkedto` | the talked-to bit | the bit is always set |
| `npc_level` | the creature's level | |
| `npc_xhome`, `npc_yhome` | the home square | the home square |
| `npc_name` | string id of the NPC's name | |
| `npc_attitude` | 0 if attacking the player (goal 5, target 1), 6 for an ally, else 0 to 3 | 0 to 3. Above 3 means friendly and an ally |
| `play_hunger` | the player's hunger | hunger |
| `play_health` | `hp * 256 / maximum` | |
| `play_hp` | hit points | hit points |
| `play_arms` | Attack skill + strength | |
| `play_power` | dexterity + mana + Missile skill | |
| `play_mana` | mana | mana |
| `play_level` | experience level | |
| `dungeon_level` | level number | |
| `game_time` | `game_clock / 0x3BC4` | |
| `game_mins` | `game_time % 1440` | |
| `game_days` | `game_clock / 0x1502E80` | |
| `new_player_exp` | 0 | if not 0, given as experience |
| `play_sex` | 0 male, 1 female | |
| `play_poison` | poison | poison |
| `play_drawn` | weapon drawn | |
| `play_name` | string id of the player's name | |

A conversation "minute" is 0x3BC4 = 15,300 clock units, a little less than a minute of game time (15,360 units), so `game_mins` and `game_days` run about 0.4% fast against the game's hour (inferred from the two constants). UW1 hands over and takes back the same variables by name (see UW1's [CONVVARS.C](https://github.com/abedegno/UW1Decomp/blob/main/src/conv/CONVVARS.C)). How each value is worked out was compared only for the attitude, which is the same.

Source: `setup_converse_data`, `update_converse_data` ([CONVVARS.C:40](../../src/conv/CONVVARS.C#L40), [CONVVARS.C:126](../../src/conv/CONVVARS.C#L126)).

## Built-ins: talking and menus

**Both.**

| Built-in | Arguments | What it does | Returns |
| --- | --- | --- | --- |
| `say` | a string | the NPC's line, in the NPC's scroll | |
| `respond` | a string | a line in the player's scroll | |
| `print` | arg1 string | narration, substituted, in colour 2 | |
| `babl_menu` | arg1 a 0-terminated array of string ids | shows them as a numbered menu, waits for a choice (keys 1 to 5, or a click), echoes it as the player's line | the number chosen, from 1 |
| `babl_fmenu` | arg1 strings, arg2 flags | the same, showing only the strings whose flag is non-zero | the chosen string's id |
| `babl_ask` | | lets the player type up to 0x32 characters | a string id holding the text, the same id each call |
| `pause` | arg1 n | waits `n * 0x1F4` scroll wait units | 1 |
| `sex` | arg2, arg1 | | arg2 for a male player, arg1 for a female one |
| `switch_pic` (UW2) | arg1 n | shows another portrait and name for the NPC: conversation n below 0x100, else generic head `n - 0x100` (up to 0x140) | 1 if a portrait loaded |

Source: [CONVERSE.C:365](../../src/conv/CONVERSE.C#L365) to [CONVERSE.C:576](../../src/conv/CONVERSE.C#L576), `switch_pic` ([CONVERSE.C:821](../../src/conv/CONVERSE.C#L821)), `sex` ([BABLHACK.C:549](../../src/conv/BABLHACK.C#L549)).

## Built-ins: strings and numbers

**Both.** These are bound by the script machine itself.

| Built-in | Arguments | Returns |
| --- | --- | --- |
| `random` | arg1 n | a number from 1 to n |
| `compare` | arg1, arg2 strings | 1 if equal after @-substitution, ignoring case |
| `contains` | arg1 text, arg2 word | 1 if the text contains the word as a whole word (bounded by the ends, spaces or punctuation). Case is ignored only when no substitution happened |
| `append` | arg1, arg2 strings | a new string, arg2's text followed by arg1's |
| `copy` | arg1 string | a new string, a copy |
| `find` | arg1 value, arg2 count, arg3 array | the 1-based position of the value among the array's first count words, or 0 |
| `length` | arg1 string | its length |
| `val` | arg1 string | its value as a decimal number |
| `plural` | arg3 count, arg2 singular, arg1 plural | arg2's string id if count is 1 or less, else arg1's |

Source: [BABL.C:415](../../src/conv/BABL.C#L415) to [BABL.C:549](../../src/conv/BABL.C#L549).

## Built-ins: quests, variables, skills and time

| Built-in | Games | Arguments | What it does | Returns |
| --- | --- | --- | --- | --- |
| `get_quest` | both | arg1 quest | **UW2.** Quests 0 to 127 are flags, 128 to 143 the quest bytes, above that one byte of the player record whose meaning is not known. **UW1.** Quests 0 to 31 are flags, 32 to 35 bytes, above that the talisman count. Negative gives 0 | the value |
| `set_quest` | both | arg2 quest, arg1 value | sets a quest. **UW2.** A flag's value is added unmasked, so a value other than 0 or 1 spills into the next flags. **UW1.** Any non-zero value sets the flag, but the bit is computed in 16 bits, so only quests 0 to 15 work and setting 15 also sets 16 to 31 (UW1Decomp's [FINDINGS.md](https://github.com/abedegno/UW1Decomp/blob/main/docs/FINDINGS.md)) | |
| `x_traps` | both | arg2 variable, arg1 value | sets a game variable, the ones traps use. **UW1.** One of 64 variables, for values 0 to 0x3F. **UW2.** A numbered variable ([schedules.md](schedules.md#the-row-events)), for values 0 to 0x3FF, stored as a byte | the variable |
| `x_skills` | both | arg2 skill, arg1 value | 0 to 30 sets the skill. 10000 raises it as a skill point would, without spending one. **UW2.** Above 10000 spends one of the player's skill points on the skill, or with a skill of -1, -2 or -3 on a random skill of a group (combat, magic, other) | the skill, or (above 10000) 1 if raised |
| `x_clock` | UW2 | arg2 clock, arg1 value | a value above 0x100 reads X clock arg2. Otherwise sets it. Setting clock 0, the time of day, moves the game clock by 20 minutes for each step changed | the clock, or 0 |
| `x_exp` | UW2 | arg1 n | gives n experience ([player-upkeep.md](player-upkeep.md#experience-and-levels)) | experience / 16 |

**Raising a skill** (both, as `x_skills` 10000 does and a trainer's skill point does): it fails if the skill is above twice its governing attribute or at 30. Otherwise the skill goes up by 1, by 1 more if the attribute is not strength and half the attribute is still above the skill, and by 1 more with chance `(attribute - skill) / d` while below the attribute, where d is 25, 30 or 15 for strength, dexterity or intelligence. At most 30.

Source: [BABLHACK.C:382](../../src/conv/BABLHACK.C#L382) to [BABLHACK.C:546](../../src/conv/BABLHACK.C#L546), `get_skill`, `grant_skill_advance` ([SKILLS.C:157](../../src/game/SKILLS.C#L157), [SKILLS.C:192](../../src/game/SKILLS.C#L192)); UW1 [BABLHACK.C:103](https://github.com/abedegno/UW1Decomp/blob/main/src/conv/BABLHACK.C#L103), [BABLHACK.C:206](https://github.com/abedegno/UW1Decomp/blob/main/src/conv/BABLHACK.C#L206).

## Built-ins: critters, doors and objects

**Both**, unless marked. "The talker" is the NPC being talked to.

| Built-in | Arguments | What it does | Returns |
| --- | --- | --- | --- |
| `set_attitude` | arg2 conversation, arg1 attitude | sets the attitude of the first active critter with that conversation number | |
| `set_race_attitude` | arg3 race, arg2 attitude, arg1 range | sets the attitude of every critter of the talker's item type and that race, not a loner, within range tiles of the talker | |
| `set_sequence` (UW2) | arg3 conversation, arg2 sequence, arg1 frame | sets the animation of the first critter with that conversation number | |
| `transform_talker` (UW2) | arg4 item, arg3 conversation, arg2 powerful, arg1 a fourth value | turns the talker into another creature (-1 keeps each value) and settles it on the floor. The fourth value is written to a field the code calls terrain; what it means is not known | |
| `remove_talker` | | removes the talker from the map | |
| `teleport_player` (UW2) | arg3 x, arg2 y, arg1 level | records a teleport, done when the conversation screen closes | |
| `teleport_talker` (UW2) | arg2 x, arg1 y | records a move of the talker on this level, done when the screen closes | 1 |
| `gronk_door` | arg3 x, arg2 y, arg1 how | opens (0), closes (1) or toggles (2) the door on that square | 0 if there is no door, else 1 |
| `place_object` | arg3 object, arg2 x, arg1 y | takes the object from the talker and puts it on that square, or at the player's feet if x is negative | 1 if placed, 0 off the map or without room |
| `take_from_npc_inv` | arg1 n | | the n-th object (from 0) in the talker's inventory, or 0. Nothing moves |
| `add_to_npc_inv` | arg1 object | adds the object to the talker's inventory | |
| `x_obj_stuff` | arg9 object, arg8 set, arg7 heading, arg6 owner, arg5 flags, arg4 link, arg3 flag10, arg2 flag9, arg1 quality | with set non-zero writes these fields of the object, else reads them into the variables. A variable holding -1 is skipped. Read back, flag10 and flag9 are 0x400 and 0x200, not 1 | |
| `x_obj_pos` | arg5 object, arg4 mode, arg3 x, arg2 y, arg1 z | mode 1 sets the fine position and height (z above 0x7F means the floor of square x, y), mode 2 reads the tile and height, others read the fine position and height. -1 skips | |

Source: [BABLHACK.C:263](../../src/conv/BABLHACK.C#L263) to [BABLHACK.C:689](../../src/conv/BABLHACK.C#L689), `transform_creature` ([WORLDEV.C:1775](../../src/event/WORLDEV.C#L1775)).

## Built-ins: trading

**Both.** "Selected" means an item in a trade slot that its owner has clicked to offer. Item numbers from 1000 name a group: `find_barter`, `find_inv` and the likes lists read them as a class or as a major and minor class, as each entry says.

| Built-in | Arguments | What it does | Returns |
| --- | --- | --- | --- |
| `setup_to_barter` | | fills the NPC's trade slots from its inventory (below) | |
| `show_inv` | arg1 indices out, arg2 items out | fills two arrays with the player's selected items | how many |
| `find_barter` | arg1 item | | the player's selected item of that type, or 0 |
| `find_barter_total` | arg4 item, arg3 count out, arg2 indices out, arg1 total out | finds the player's selected items of that type and their total quantity. A group number never matches, because of the code's test | 1 if any |
| `give_to_npc` | arg2 count, arg1 indices | gives the listed objects to the NPC, if every one is among the player's selected items | 1 if given |
| `give_ptr_npc` | arg2 object, arg1 quantity | gives the NPC that many of one object, splitting a stack, from the trade slots or else from the inventory | 1 if given |
| `take_from_npc` | arg1 item | the NPC hands over an item of that type (or of class `item - 1000`): onto the cursor if the player can carry it, else into a free player trade slot, else at the NPC's feet | 1, 2 dropped, 3 failed, 0 if there is none or the cursor already holds something |
| `take_id_from_npc` | arg1 object | the same for one object | as above |
| `find_inv` | arg2 item, arg1 from player | looks for an item in the NPC's (or the player's) inventory, containers included | its index, or 0 |
| `do_inv_create` | arg1 item | creates the item (quality 63) in the NPC's inventory | its index |
| `do_inv_delete` | arg1 item | deletes the first item of that type from the NPC's inventory | 1 if found |
| `identify_inv` | arg4 object, arg3 with article, arg2 name out, arg1 identified | puts the object's name in a new string | its value to the NPC |
| `count_inv` | arg1 object | | its quantity, or 1 |
| `check_inv_quality` | arg1 object | reads the quality | whatever was left in a register, which holds the quality (the code never returns it explicitly) |
| `set_inv_quality` | arg2 object, arg1 quality | sets the quality (6 bits) | 1 |
| `set_likes_dislikes` | arg2 likes, arg1 dislikes | two script arrays ending in -1 | 1 |
| `do_offer` | arg5 to arg1: lines for accepted, not enough, worse, out of patience, nothing offered | the player's offer (below) | 1 if the trade is made |
| `do_demand` | arg3 nothing selected, arg2 gives in, arg1 refuses | the player's demand (below) | 1 if the NPC gives in |
| `do_decline` | | the NPC's items go back | |
| `do_judgement` | | the player's own appraisal (below) | |
| `end_barter` | | returns everything in the slots | |
| `give_all_stuff` (UW2) | | the NPC offers everything in its slots, as after an accepted offer | 1 if there was anything |

Source: [CONVERSE.C:586](../../src/conv/CONVERSE.C#L586) to [CONVERSE.C:817](../../src/conv/CONVERSE.C#L817), [BARTER.C:81](../../src/conv/BARTER.C#L81) to [BARTER.C:1078](../../src/conv/BARTER.C#L1078).

## How bartering works

**Both.** Each side has six trade slots in UW2 and four in UW1. The rules below are UW2's. [UW1-UW2-DIFFERENCES.md](../UW1-UW2-DIFFERENCES.md#conversations) lists UW1's differences.

**The NPC's temper** is set when the conversation starts. The random generator is seeded with the NPC's object index, so the same NPC always has the same temper. `range(b, lo, hi)` means `b` plus a random `lo` to `hi` percent of `b`.

- Greed, the gain in % the NPC wants: `range(haggle * 6, -25, 25)`, minus twice the player's Charisma.
- Patience, how many bad offers it takes: `range(patience, -20, 100)`, plus half the Charisma.
- Appraisal error in %: `range((15 - shrewd) * 6, -25, 50)`.

**Filling the NPC's slots** (`setup_to_barter`). The game walks the NPC's inventory (at most 40 objects), skipping its first weapon and anything worthless. Once all six slots are full it goes round again, and each further item replaces a slot's item with chance 3 in 8.

**Value.** An item's value to the NPC is its object value times its quantity times `quality / 64` (coins count as quality 63), at least 1 unless the quality is 0. It is then blurred by up to the appraisal error, the same way each time for the same object. With the NPC's likes, a disliked or worthless item is worth 0 and a liked one 1.5 times as much.

**An offer** (`do_offer`).

1. Out of patience (below 0): the "out of patience" line, and nothing more.
2. Nothing selected on either side: the "nothing offered" line.
3. The gain is `(player's side - NPC's side) * 100 / NPC's side`, both valued by the NPC with its likes (100 if the NPC's side is worth 0).
4. If the gain is at least the greed, the trade is made. The NPC's unselected items go back to its inventory, its selected items stay in the slots for the player to take, and the player's selected items that the NPC does not dislike go to it.
5. Otherwise the first offer below half the greed costs 1 patience. An offer worse than the last costs 2. A later offer that closed less than a third of the gap left by the last one (`(greed - last) * 3 / 2 > greed - gain`) costs 1.

**A demand** (`do_demand`).

- Player's score: level + 1 if a weapon is drawn + health + Charisma / 6, where health is `2 - 2 * damage taken / maximum`.
- NPC's score: creature level + mood + its health likewise + value demanded / 10, where mood is -1 for an ally, 1 for an attitude below 2, else 0. **UW2.** On levels 9 and 17 the NPC's score is 1.5 times as much.
- If the player scores more, the NPC gives in and its attitude falls by one while above 1. Otherwise it refuses and attacks the player (goal 5).
- **UW1.** An ally always gives in, and the attitude falls while above 0.
- Demanding nothing also lowers the attitude by one while above 1.

**The player's appraisal** (`do_judgement`). Both sides are valued without likes, blurred by `50 - Appraise * 1.5` %, and the gain is worked out as for an offer, with a fudge value added to the NPC's side (set by a hack, below). The verdict runs from "a terrible deal" (gain above 50) through 35, 25, 10, -10, -25, -35 and -50 to "an excellent deal" (-50 or less). It is prefixed by a certainty from "I guess" (Appraise below 6) through 12, 18 and 24 to "I know".

Source: `barter_init`, `setup_to_barter`, `assess_value`, `range`, `do_offer`, `do_demand`, `do_judgement`, `does_npc_like` ([BARTER.C:131](../../src/conv/BARTER.C#L131), [BARTER.C:81](../../src/conv/BARTER.C#L81), [BARTER.C:784](../../src/conv/BARTER.C#L784), [BARTER.C:818](../../src/conv/BARTER.C#L818), [BARTER.C:588](../../src/conv/BARTER.C#L588), [BARTER.C:649](../../src/conv/BARTER.C#L649), [BARTER.C:724](../../src/conv/BARTER.C#L724), [BARTER.C:1036](../../src/conv/BARTER.C#L1036)); UW1 [BARTER.C:719](https://github.com/abedegno/UW1Decomp/blob/main/src/conv/BARTER.C#L719).

## The Pits of Carnage and other hacks (UW2)

**UW2.** `babl_hack` (arg1 mode, then more arguments) does odd jobs by number:

| Mode | What it does | Returns |
| --- | --- | --- |
| 0 | the talker challenges the player: it becomes pit fighter 0, and the player is in the arena after the next level change | 0 |
| 1 | reports, once, that the player ran from a pit fight | 1 once, else 0 |
| 2 | sets up a fight (arg4 power, arg3 corner, arg2 count, below) | how many fighters were placed |
| 3 | collects Jospur's debt (quest byte 133) and sets it to 0 | the debt |
| 4 | | 1 if the player is in the pits |
| 5 | makes every critter with conversation arg2 a loner | 0 |
| 6 | | whether speech is available |
| 7 | recharges object arg2 by arg3 charges (inferred from the routine it calls) | |
| 8 | multiplies the NPC's greed by arg2 | the new greed |
| 9 | sets the appraisal fudge to arg2 | arg2 |
| 10 | | 1 if the player wears the Guardian's signet ring on either hand |

**Setting up a fight** (mode 2). The fighters are placed from the arena's centre, tile (0x1F, 0x1F), towards corner arg3 (0 is +x +y, 1 is -x +y, 2 is -x -y, 3 is +x -y). With a count of 5, the first fighter is always strong and stands 6 tiles along x and 4 along y. The rest fill a triangle at offsets (4, 4), then (4, 5) and (5, 4), then (4, 6), (5, 5) and (6, 4), until the count is used. A fighter that can't be placed still uses up a place.

Each fighter is a human (0x75 to 0x77 one time in 16, else 0x78 or 0x79). With chance power in 3 it is strong, and two in five of those become instead a human 0x7B or a great troll, not flagged strong. Fighters are temporary, hostile loners with conversation 0x66, attacking the player. Jospur's debt becomes 8, 12, 20 or 40 for 2, 3, 4 or 5 fighters placed, else 0.

Source: `babl_hack`, `place_pitfighter` ([BABLHACK.C:97](../../src/conv/BABLHACK.C#L97), [BABLHACK.C:202](../../src/conv/BABLHACK.C#L202)).
