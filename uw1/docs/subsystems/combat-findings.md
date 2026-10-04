# Combat and magic: candidates for the findings page

Candidates from `src/combat` for the project's findings page; [combat.md](combat.md) describes the subsystem. Each bug candidate was re-read in the matched source; what the code does is read from the code, and the effect in the game is an inference unless it says it was checked. "Both games" means UW2Decomp's matched source has the same code.

## Likely bugs

### A blocked blow's sound uses the attacker's armour

- **What happens:** `do_miss` picks the sound of a blow that was blocked from the weapon kind and the armour kind (effect 7 for metal on metal, else 8). For a critter target the armour kind is read as `Creature[fromwho & ID_INMAJOR].armour_kind`, and `fromwho` is the attacker, not the target (`hitobj`).
- **Where:** `do_miss` in [combat/COMBAT.C](../../src/combat/COMBAT.C).
- **Evidence:** code reading; both games (UW2 reads it through `attitem`, the attacker's item). For the player as target the same function reads the player's own armour, which shows the target was meant.
- **Confidence:** likely.
- **Effect:** a blocked blow between critters sounds by the attacker's armour. Not checked in the game.
- **For a port:** matching DOS means using the attacker.

## Possible bugs

### A dropped lit taper stays lit

- **What happens:** `ReturnObject` puts out a lit light that is dropped only for minor classes 4..6 of the light class; the lit taper is 7. `PutObjectInBag` (BAGS.C) and `ItemFitsSlot` (INVPANEL.C) treat 4..7 as lit.
- **Where:** `ReturnObject` in [combat/MISSILE.C](../../src/combat/MISSILE.C).
- **Evidence:** code reading; both games.
- **Confidence:** possible: the lit taper (0x97) is the Taper of Sacrifice, a talisman, which may have been meant to stay lit.

### The rune bag accepts a 25th rune

- **What happens:** `add_rune` accepts `rune > NUM_RUNES` as out of range, so rune 24 (item 0x100, a key) would be taken as a rune and set bit 7 of `runebag[3]`, past the bag's three bytes.
- **Where:** `add_rune` in [combat/RUNES.C](../../src/combat/RUNES.C).
- **Evidence:** code reading; both games. Its only caller, `PutObjectInBag`, runs `ItemFitsSlot` first, whose rune bag rule lets through only runestones (0xE8..0xFF).
- **Confidence:** possible; unreachable in the game.

### An object casting a class 13 or 14 spell is tested at a home it does not have

- **What happens:** `do_spell` tests an object caster (at or above `objdata`) for anti-magic at `inanmMapX`, `inanmMapY` only for classes up to 11. For classes 13 (special) and 14 (cutscene) it falls to the critter test, `anti_magic_p(OBJ_HOMEX(who), OBJ_HOMEY(who))`, and the home word is at offset 0x16 of a mobile record: a static object's record is 8 bytes, so this reads the next records' bytes.
- **Where:** `do_spell` in [combat/SPELLS.C](../../src/combat/SPELLS.C).
- **Evidence:** code reading; whether any object in the shipped data carries a class 13 or 14 spell was not checked.
- **Confidence:** possible.
- **Effect:** such a spell may be refused, or allowed, by the anti-magic bit of an unrelated tile.

## Engine findings

- `do_miss` stores the attacker's item id into the global `fromwho` (UW2 uses a local). The tests that follow therefore compare an item id: `fromwho == 1` is item 1 (the battle axe), never the player, and the player's blocked blows take their weapon kind from his own `Creature` record (0x3F) rather than from what he wields. `fromwho` holds the item id until the next blow sets it.
- `clear_runes` clears 8 bytes from `runebag`, which also clears the shelf and the carried weight that follow it in the player record. Its one caller, Armageddon, clears both anyway.
- The rune spells are the first 48 of the 53 `spells[]` entries; the other five are reached only through `cast` (wands, scrolls, objects).
- `do_spell` refuses a spell on level 9 only when the caster is a critter (or the player), or the class is above 11: an object casting a class 0..11 spell is tested for anti-magic at its square (`inanmMapX`, `inanmMapY`) and not for the level.

## Recovered rules

- Casting: level (player level + 1) / 2 at least the circle (spell index / 6 + 1), mana 3 * circle, then `skill_check(Casting + 5, 2 * circle)`; -1 backfires at strength circle / 2. The next cast waits (2 * circle - level) * 4 + 0x40 ticks.
- Smite Undead hits only a critter whose resist byte has 0x80; mind spells (confusion, fear, paralyse, ally) are resisted with chance (resist & 3) in 3.
- Summon Monster picks a creature from lvl to 2 * lvl - 1 (lvl the Casting skill, or a critter caster's dungeon level * 4, at least 2), rerolling swimmers, creatures with no hit points or with `Creature` bit `bA_1`, and items 0x7B and 0x7C.
