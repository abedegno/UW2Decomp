# The game: candidates for the findings page

Candidates from `src/game` for the project's findings page; [game.md](game.md) describes the subsystem. Each was re-read in the matched source; strings quoted are the shipped `STRINGS.PAK`'s.

## Likely bugs

### Item toughness on a two-location item goes to one location only

- **What happens:** for an enchantment of class 12 with the 8 bit set (toughness) on an item worn in a slot above 4 (counted for hit locations 0 and 1), `player_affected_by` loops over both locations but adds the toughness to `armour[slots[0]]` each time, so location 0 gets it twice and location 1 not at all. The to-hit protection in the same loop uses `slots[i]` correctly.
- **Where:** `player_affected_by` in [game/PLAYDATA.C](../../src/game/PLAYDATA.C).
- **Evidence:** code reading.
- **Confidence:** likely (the neighbouring line indexes by `i`).
- **Effect:** such an item protects one hit location twice over and leaves the other bare. Which items carry such enchantments was not checked.

### Chanting FANLO says "None of your skills improved."

- **What happens:** the mantra that gives the key of truth places the key and then prints block 1 string 0x1E, "None of your skills improved.", the message `report_advance` prints for a group mantra that raised nothing.
- **Where:** `mantra_advance` in [game/SKILLS.C](../../src/game/SKILLS.C) (marked there).
- **Evidence:** code reading against the shipped strings.
- **Confidence:** possible (a wrong string number seems likely, but the message may have been accepted as it was).
- **Effect:** the player gets the key with a misleading message.

## Possible bugs

- **The NO mantra prints nothing:** it prints block 1 string 0x1F, which is empty in the shipped strings (`mantra_advance`).
- **A mana increase on level 7 after the orb is gone:** `player_compute` on level 7 always writes the new maximum mana into `saved_mana`, but `do_level_hacks` only puts `saved_mana` back while the orb stands. So a level gained there after the orb is destroyed leaves the maximum mana as it was until the next `player_compute` on another level.
- **New objects used unchecked:** `player_is_dead` passes the bones from `CreateObj` to `put_at` without testing for a null pointer, and `plant_seed` sets fields of the tree before testing it. Both fail only when the level has no free static object.
- **`player_affected_by` reads past `slots`:** its loop tests `slots[i] != -1` before `i < 2`, so with both slots in use it reads `slots[2]`, a stack word, before stopping. The value does not change the outcome.

## Game rules recovered

See [game.md](game.md), "Rules found in the code". The ones that answer open questions elsewhere:

- Why there is no magic in Tybal's lair (a TODO in the Guide): `do_level_hacks` keeps the player's maximum mana aside and sets it and the mana to 0 on arriving at level 7 while Tybal's orb stands, and gives back the maximum and a quarter of it as mana on leaving. Building mana up there is blocked too: `player_compute` writes the new maximum into the saved value on level 7.
- The level 9 automap switch is kept in the same record byte (`saved_mana`, 0xB0) as level 7's maximum mana; the two never overlap, since leaving level 7 restores the mana before any arrival.
- Experience stops growing at 0x17700 and is halved above twice the dungeon level plus two; a skill point comes every 3000.
- The silver tree only grows on certain floor textures (texture numbers 5 to 11, 18 to 22, 27 to 31 and 35 to 40 in the floor textures, through the level's `floor_IDs`).

## Dead code

- `MaybePlayerDayLoadrelated_ovr142_0` (empty) in `PLAYDATA.C` and `ovr112_389` in `UWEDIT.C` have no caller in the matched C.
