# Conversations and bartering

This page describes how UW1 runs a conversation: starting it, the conversation screen, the script interpreter and its built-ins, the variables handed over, and bartering. The sources are in `src/conv`; the declarations, the opcodes and the trade constants are in `src/include/conv.h`. The candidates for the findings page are in [conversations-findings.md](conversations-findings.md).

## Files

| File | Segment | What it does |
|---|---|---|
| [`CONVERSE.C`](../../src/conv/CONVERSE.C) | ovr100 | `TalkTo`, the conversation screen, `Converse` (load, bind, run), menus, the say, respond and print built-ins, the inventory built-ins |
| [`BABL.C`](../../src/conv/BABL.C) | ovr093 | the interpreter: loading a script from `DATA\CNV.ARK`, binding imports, the stack machine, string substitution, the script heap, the conversations' saved globals |
| [`BABLHACK.C`](../../src/conv/BABLHACK.C) | ovr094 | built-ins that reach into the game: attitudes, skills, game variables and quests, doors, objects' fields and positions |
| [`BARTER.C`](../../src/conv/BARTER.C) | ovr095 | the trade slots, offers, demands, appraisal, the NPC's likes and dislikes, an object's value |
| [`CONVVARS.C`](../../src/conv/CONVVARS.C) | ovr103 | handing the NPC's and the player's state to the script and taking it back |
| [`GRDB.C`](../../src/conv/GRDB.C) | seg045 | the label table of the 3D view's render database (in `conv/` by UW2's placement; it has nothing to do with conversations) |

Names are UW2's (the FM Towns originals, or UW2Decomp's provisional names for FM Towns statics), the routines being the same; the listing's names (`name_ovrNNN_XXX`) remain where neither has one. The file names are UW2Decomp's.

## Starting a conversation

`TalkTo` (`CONVERSE.C`) checks that the critter will talk: a few whoamis always do; otherwise one that is not attacking or fleeing from the player (or cornered by him), not hostile unless an ally, and with a conversation, or one whose goal is to talk. A shrine is a mantra (`mantra_advance`) and a picture of the princess answers for itself. The conversation is the critter's whoami slot of `CNV.ARK`, or 0x100 plus its creature class for whoami 0. The screen's start function `strt_converse` draws the screen, both portraits and names, sets up the trade slots (`barter_init`) and calls `Converse`, which loads the script into the screen buffer, binds the built-ins by name (`bab_fun`), hands over the variables (`setup_converse_data`), generates the NPC's inventory if it has none, runs the script (`babl_run`) and takes the variables back.

## The interpreter

A script is an array of words run on a small stack machine (`BABL.C`): `mem` is the variables (the imports and the conversation's private globals) followed by a stack of 0x800 words; `sp`, `bp` and `pc` index the stack and the code; `reg` holds a built-in's result. The opcodes are UW-Formats' (7.4), named in `conv.h` (`enum BablOp`). Built-ins are called by `CALLI` through `funcs[]` with a pointer to the top of the stack, the argument count first and the arguments' addresses below it. Strings are string ids: below 0x200 the conversation's own block of `STRINGS.PAK`, and strings a script builds go to the dynamic block 0x7C, cleared when the script ends. `convert_string` substitutes `@` variables in conversation text. Each conversation's private globals are saved in `SAVE0\bglobals.dat` when it ends.

The script heap (`bab_malloc`) is first fit from one 0xFFFF-byte free block, each block 8 bytes of header and a 4-byte tag. In UW1 `bab_free` never frees anything (see the findings).

## Variables and built-ins

`setup_converse_data` (`CONVVARS.C`) writes the imported variables a script asks for: the NPC's whoami, hunger, health, hit points, goal and target, attitude (0 when attacking the player, 6 for an ally), level and home; the player's hunger, health, hit points, mana, level, sex, poison and name; the dungeon level, the time (in a conversation's minutes, 0x3BC4 clock ticks each) and the day. `update_converse_data` copies back what a conversation may change; an attitude above 3 means friendly and an ally.

The built-ins in `BABLHACK.C` set attitudes (one critter, or a race around the talker), skills, the 64 game variables and the quests (bits 0..31 of one long, 32..35 bytes), open and close doors, move objects between the talker and the map, and read or write an object's fields.

## Bartering

Each side has four trade slots (`NUM_TRADE_SLOTS`). The player drags items into his slots and clicks to select; the script fills the NPC's (`setup_to_barter`) and calls `do_offer` or `do_demand`. The NPC's temper comes from its creature class, reseeding the random generator with its object index so that it is the same every time: greed (the % gain it wants), patience (bad offers it takes) and `npc_assess` (how far off its valuations are); the player's Charisma lowers greed and raises patience.

- An item's value (`assess_value`) is its class value times the quantity times quality / 64, blurred by up to `accuracy` %, the same each time for the same object; with likes, a disliked item is worth 0 and a liked one half as much again. Script item numbers from 1000 (`BARTER_CLASS`) name a class or a major and minor.
- `do_offer`: the gain is (player's - NPC's) * 100 / NPC's. At or above greed the trade is made. Otherwise a first offer below half of greed, an offer worse than the last, or one that closes less than a third of the remaining gap costs patience; out of patience the NPC refuses to trade.
- `do_demand`: the player's level, readiness to fight, health and Charisma against the NPC's level, mood, health and the value demanded. An ally always gives in; a refusal sets the NPC to attack the player.
- `do_judgement` is the player's own appraisal, by the Appraise skill.

## Open questions

- The meaning of the import variable types 0x126 to 0x12B that `bab_var_clear` tests (int, int array, string, string array is a guess; UW-Formats lists only return types).
- What the text-mode debugging built-ins of `BABL.C` were run from: nothing binds them.
