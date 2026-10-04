# Conversations: candidates for the findings page

Candidates from `src/conv` for the project's findings page; [conversations.md](conversations.md) describes the subsystem. Each bug candidate was re-read in the matched source; what the code does is read from the code, and the effect in the game is an inference unless it says it was checked. "Both games" means UW2Decomp's matched source has the same code.

## Likely bugs

### The script heap never frees anything

- **What happens:** `bab_free` takes the block header 8 bytes before the data and accepts the block only if the tag at its end equals the data address and the header's next field points at itself. UW1 compares the tag with the header's address instead of the data's, so the test never passes and nothing is freed. The code behind the test is broken too (it adds sizes to struct pointers, scaling them by 8, and its loop does not advance after a merge), so it is as well that it never runs.
- **Where:** `bab_free` in [conv/BABL.C](../../src/conv/BABL.C).
- **Evidence:** code reading (docs/NOTES.md, wave 3). UW1 only: UW2's `bab_free` checks the tag (`tag_check`) on the data pointer before it steps back to the header.
- **Confidence:** likely.
- **Effect:** every menu's substituted texts and every string a script builds stay allocated until the conversation ends; the heap is 0xFFFF bytes, so only a very long conversation could run out (callers then stop the game with `pfatal_code`). Not checked in the game.
- **For a port:** freeing would change nothing a player sees unless the heap runs out.

### Conversation quests 15 to 31 do not work

- **What happens:** `set_quest` and `get_quest` make a quest's bit as `1 << quest` in a 16-bit int and then widen it to the 32-bit quest word. For quests 16..31 the shift leaves 0, so they can be neither set nor read (always 0); quest 15 gives 0x8000, which widens with its sign to 0xFFFF8000, so setting it sets bits 15..31, clearing it clears them, and reading it reports any of them.
- **Where:** `set_quest` and `get_quest` in [conv/BABLHACK.C](../../src/conv/BABLHACK.C).
- **Evidence:** code reading. UW1 only (UW2 keeps its quests in an array of bytes). The trap code (`TRIGGER.C`) builds the same bits with `1L << d`, correctly.
- **Confidence:** likely.
- **Effect:** none if no conversation uses quests above 14; UW-Formats names quests 0..11 only. Which quests the shipped conversations use was not checked.
- **For a port:** matching DOS means the 16-bit shift.

### find_barter_total cannot search by class

- **What happens:** the built-in tests `wanted < 1000 ? ids[i] == wanted : (ids[i] >> 4) == wanted - 1000`, a class search for item numbers from 1000, but the whole loop is inside `if (wanted < 1000)`, so a class search finds nothing and returns 0.
- **Where:** `find_barter_total` in [conv/CONVERSE.C](../../src/conv/CONVERSE.C).
- **Evidence:** code reading; both games.
- **Confidence:** likely.
- **Effect:** a script asking for a class of item in the trade slots is told there is none. Whether a shipped script asks was not checked.

## Possible bugs

### A conversation's minute is shorter than the game's

- **What happens:** `setup_converse_data` gives scripts `game_time` and `game_mins` as `game_clock / 0x3BC4` and `game_days` as `game_clock / 0x1502E80` (0x3BC4 * 1440). The rest of the game counts 0x3C00 ticks a minute (0xE1000 an hour, `SKILLS.C`).
- **Where:** `setup_converse_data` in [conv/CONVVARS.C](../../src/conv/CONVVARS.C); the constants are `CONV_MINUTE` and `CONV_DAY` in `conv.h`.
- **Evidence:** code reading; both games.
- **Confidence:** possible (0x3BC4 may be a deliberate value).
- **Effect:** a conversation's day ends about 86,400 ticks (5.6 game minutes) earlier each day, so after many days a script's day count runs ahead of the clock's.

## Engine findings

- `find_barter_total`'s arrays hold 5 entries; UW1's four trade slots never overrun them (UW2's six could).
- The conversation's built-in list in `Converse` binds 42 built-ins; UW2's `babl_hack`, `give_all_stuff`, `set_sequence`, `transform_talker`, `x_clock`, `x_exp`, `teleport` and `switch_pic` are not in UW1.
- `GRDB.C` sits under `src/conv` but is the 3D view's render database label table.

## Dead code

- The text-mode debugging built-ins in [conv/BABL.C](../../src/conv/BABL.C) (`ovr093_9F8`, `ovr093_A7D`, `ovr093_ADB`, `ovr093_BE7`, `ovr093_E4B`, `ovr093_E61`): print, ask, menu, fmenu, say and respond on the console through scanf and dprintf. Nothing binds them.
- `grdb_size`, `gr_getpre`, `gr_getlab` and `gr_freelab` in [conv/GRDB.C](../../src/conv/GRDB.C): nothing calls them. They carry latent faults: `gr_getlab` tests an unsigned stack pointer for being negative, and `gr_freelab` bounds its push at 0x5F where the stack has 32 entries.
- `unbound` looks the current built-in up in the import table and does nothing with what it finds.
