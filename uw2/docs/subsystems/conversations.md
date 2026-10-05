# Conversations, the babl machine and bartering

This page describes how UW2 runs a conversation: starting it, the compiled script and the stack machine that runs it ("babl"), the variables handed in and out, the built-in functions scripts call, and bartering. The sources are in `src/conv`; the declarations are in `src/include/conv.h`. The file `GRDB.C` is also in this directory but belongs to the 3D renderer (see the end).

## Files

| File | Segment | Name | What it does |
|---|---|---|---|
| [`CONVERSE.C`](../../src/conv/CONVERSE.C) | ovr103 | descriptive | starting a conversation, the conversation screen, binding the built-ins, menus, say and respond, the inventory built-ins |
| [`BABL.C`](../../src/conv/BABL.C) | ovr095 | inferred (`init_babl`, `babl_run`, the `bab_` prefix) | loading a script, the stack machine, imports, the script heap, string substitution |
| [`CONVVARS.C`](../../src/conv/CONVVARS.C) | ovr106 | descriptive | the variables copied into a script and back |
| [`BABLHACK.C`](../../src/conv/BABLHACK.C) | ovr096 | descriptive | the built-ins that reach into the game (`babl_hack`, quests, skills, teleports, objects, doors) |
| [`BARTER.C`](../../src/conv/BARTER.C) | ovr097 | descriptive | the trade slots, offers, demands, valuation |
| [`GRDB.C`](../../src/conv/GRDB.C) | seg045 | inferred (the `grdb_` prefix) | the 3D render database's label table (not conversation code) |

Function and global names are the FM Towns originals where that build has them. `map/filenames.tsv` has the evidence for each file name.

## A conversation from start to end

1. `TalkTo` (`CONVERSE.C`) is reached from the player's talk command (`ui/INTERACT.C`), from a critter that wants to talk (`critter/AI.C`'s goal 10), from the plot's deaths (`event/WORLDEV.C`'s `stop_and_talk`) and from `talk_to_disembodied`, which makes a temporary rotworm to carry a voice. It checks the NPC will talk, sets `talking_to` and `cnv_id` (the NPC's `whoami`, or 0x100 plus the creature class when it has none) and switches to the conversation screen.
2. The screen's start, `strt_converse`, draws both portraits and names, sets up the trade slots (`BARTER.C`'s `barter_init`) and calls `Converse`.
3. `Converse` loads the script (`BABL.C`'s `load_script`: block `cnv_id` of `DATA\CNV.ARK`, its import table and code, and the conversation's saved globals from `bglobals.dat`), binds about fifty built-ins by name (`bab_fun`), copies the game's state into the script's imported variables (`CONVVARS.C`'s `setup_converse_data`), runs it (`babl_run`), and copies back what a conversation may change (`update_converse_data`).
4. The screen's exit, `free_converse`, puts things back and performs any teleport the script asked for (`BABLHACK.C`'s `do_babl_teleport`), so a script's teleports happen after the conversation. `game/SKILLS.C`'s `cs_check` then plays a cutscene the script left pending.

## The babl machine

A script is an array of 16-bit words. Memory (`mem`) is the script's variables (the imports, then the conversation's private globals) followed by a stack of 0x800 words; `sp` and `bp` index the stack, `pc` the code, and `reg` holds a result. Variable addresses are indexes into `mem`. The opcodes, in order (`opcode_text`): NOP, OPADD, OPMUL, OPSUB, OPDIV, OPMOD, OPOR, OPAND, OPNOT, TSTGT, TSTGE, TSTLT, TSTLE, TSTEQ, TSTNE, JMP, BEQ, BNE, BRA, CALL, CALLI, RET, PUSHI, PUSHI_EFF, POP, SWAP, PUSHBP, POPBP, SPTOBP, BPTOSP, ADDSP, FETCHM, STO, OFFSET, START, SAVE_REG, PUSH_REG, STRCMP, EXIT_OP, SAY_OP, RESPOND_OP, OPNEG (UW-Formats describes the same machine for `CNV.ARK`).

CALLI calls a built-in through `funcs[]` with a far pointer to the top of the stack: `args[0]` is the argument count and `args[-1]`, `args[-2]` ... the arguments, each the address of a script variable (`getmem`, `getmem_addr`). Strings are string ids: below 0x200 they are the conversation's own block of `STRINGS.PAK` (block 0xE00 + the conversation), and strings a script builds go into block 0x7C, cleared when it ends. `convert_string` substitutes `@`-variables into text. A small first-fit heap (`bab_malloc`) lives in the conversation screen's buffer.

Each conversation's globals persist in `bglobals.dat` (a copy of `DATA\BABGLOBS.DAT` made for a new game by `init_babl`), saved back when the script ends.

## Variables in and out

`setup_converse_data` (`CONVVARS.C`) fills only the variables the script imports: the NPC's whoami, hunger, health (hit points * 256 / its class's average), hit points, arms, power, goal and target, talked-to bit, level, home square, name and attitude (0 when attacking the player, 6 for an ally, else 0..3); the player's hunger, health, hit points, arms, power, mana, level, sex, poison, whether a weapon is drawn and name; the dungeon level and the time (`game_time`, `game_mins`, `game_days`). Afterwards it takes back the NPC's hunger, hit points, home, goal and attitude (above 3 means friendly and an ally), the player's hunger, hit points, mana and poison, and `new_player_exp`, which is given as experience.

## Built-ins

The names `Converse` binds: `babl_menu`, `babl_fmenu`, `say`, `respond`, `get_quest`, `set_quest`, `sex`, `babl_ask`, `print`, `show_inv`, `give_to_npc`, `find_inv`, `take_from_npc`, `take_id_from_npc`, `identify_inv`, `do_offer`, `do_demand`, `do_decline`, `do_judgement`, `end_barter`, `setup_to_barter`, `pause`, `set_likes_dislikes`, `do_inv_create`, `do_inv_delete`, `check_inv_quality`, `set_inv_quality`, `count_inv`, `babl_hack`, `give_all_stuff`, `gronk_door`, `set_sequence`, `set_attitude`, `set_race_attitude`, `take_from_npc_inv`, `add_to_npc_inv`, `place_object`, `transform_talker`, `remove_talker`, `x_skills`, `x_traps`, `x_obj_stuff`, `x_obj_pos`, `find_barter`, `find_barter_total`, `give_ptr_npc`, `x_clock`, `x_exp`, `teleport_player`, `teleport_talker`, `switch_pic`. Each is commented in its source file with its arguments.

Rules found in the code:

- **Quests** (`set_quest`, `get_quest`). Quests 0 to 127 are bits, four to each `quests[]` entry; 128 to 143 are the bytes `quest_bytes[0..15]`. A value other than 0 or 1 is added unmasked and spills into the neighbouring bits.
- **Skills** (`x_skills`). A value above 10000 spends one of the player's skill points on the skill (training); exactly 10000 raises it as a skill point would; 0 to 30 sets it.
- **The clock** (`x_clock`). Setting X clock 0, the day clock, moves the game clock 20 minutes for each step.
- **The Pits of Carnage** (`babl_hack` modes 0 to 4). A challenge makes the talker pit fighter 0; mode 1 tells the script, once, that the player ran from a fight (`event/WORLDEV.C`'s `arena_player_runs` sets `running_away` and starts a conversation with a fighter); mode 2 places 2 to 5 opponents on a corner of the arena, the first of five always strong, and sets Jospur's debt (quest 133) to 8, 12, 20 or 40 by the number placed; mode 3 collects it.
- **Teleports** asked for in a conversation (`teleport_player`, `teleport_talker`) are only recorded; `do_babl_teleport` moves the player or the NPC once the conversation screen has closed.

## Bartering

Each side has six trade slots (`BARTER.C`). The player drags items into his slots and selects them; a script fills the NPC's slots (`setup_to_barter`) and calls `do_offer` or `do_demand`. The NPC's temper is set when the conversation starts (`barter_init`), seeded with its object index so the same NPC always has the same temper: greed (the % gain it wants), patience (bad offers it will take) and how far off its valuations are, from its creature record; the player's Charisma lowers greed and raises patience.

- **Value** (`assess_value`): the item's value (`ComObjData`) times quantity times quality / 64 (coins count as quality 63), blurred by up to the NPC's assessment error, the same each time for the same object. A disliked item is worth nothing to the NPC and a liked one half as much again (`set_likes_dislikes`).
- **Offers** (`do_offer`): the gain is (player's side - NPC's side) * 100 / NPC's side; at or above greed the NPC accepts. Otherwise it loses patience on a mean first offer, on an offer worse than the last, and on an offer that closes little of the gap; at no patience it ends the haggling.
- **Demands** (`do_demand`): the player's level, readiness to fight, health and Charisma against the NPC's level, mood, health and the value demanded (times 1.5 on dungeon levels 9 and 0x11). If the player scores more the NPC gives in and likes him less; otherwise it attacks.
- **Appraisal** (`do_judgement`): the player's own verdict, from "a terrible deal" to "an excellent deal", blurred by his Appraise skill and prefixed by how sure he is.

## GRDB.C is 3D code

`GRDB.C` is in `src/conv` because `map/filenames.tsv`'s evidence once called it the conversation bytecode assembler's label table. Its comment explains why that is wrong: it is the label table of the 3D view's render database, every caller is 3D code, and in FM Towns its functions sit between `draw_solid_tmap` and `cZoom_`, `cRender_`. Its subsystem is the 3D view ([3d.md](3d.md)).

## Open questions

[FINDINGS.md](../FINDINGS.md) collects the open questions and likely bugs of every subsystem in one place.

- The order in which a script's source wrote a built-in's arguments is not known; the comments number them as UW-Formats does (arg1 the last pushed).
- `npc_wit` is computed in `barter_init` and never read.
