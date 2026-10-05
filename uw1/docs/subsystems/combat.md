# Combat and magic

This page describes melee and missile combat, damage to objects, missiles, the rune bag and spell casting in UW1. The sources are in `src/combat`; the declarations are in `src/include/combat.h`, except `DAMAGE.C`'s, which are in `object.h` with the damage types. The per-item tables (`ComObjData`, `Creature`) are in `object.h` and `critter.h`. The candidates for the findings page are in [combat-findings.md](combat-findings.md).

## Files

| File | Segment | What it does |
|---|---|---|
| [`COMBAT.C`](../../src/combat/COMBAT.C) | seg022_230E | a blow: where it lands, what it hits, the to-hit roll, the damage roll, the miss sounds, the player's swing and charge, critter attacks, experience for a kill |
| [`DAMAGE.C`](../../src/combat/DAMAGE.C) | seg023 | resistances, damage to objects that are not critters, what a broken object becomes |
| [`MISSILE.C`](../../src/combat/MISSILE.C) | seg025 | launching missiles for the player, critters, spells and traps; throwing and dropping |
| [`RUNES.C`](../../src/combat/RUNES.C) | ovr119 | the rune bag, the rune shelf, the active spell icons, casting from runes |
| [`SPELLS.C`](../../src/combat/SPELLS.C) | seg038_3307 | the spell table and `do_spell`, healing and mana, missile, area and targeted spells, creation and summoning, Detect Monster, the object spells and the extra spells |

UW1's `SPELLS.C` holds what UW2 split into `SPELLS.C` and `SPELLS2.C`, and it is resident. Names are UW2's (the FM Towns originals) where the routine is the same one; UW1 has no symbols of its own. The file names are UW2Decomp's.

## Melee

A blow is a set of globals in `COMBAT.C` filled in by the attacker's side, then `do_attack`:

- the player: `player_attack`, called every frame from the input code. The swing is the ninth of the 3D view clicked; its row picks the swing kind (`swing_kind`: stab, slash or bash, indexing the weapon's three damage bytes) and the height of the blow. Holding the button charges `play_pow` by the weapon's speed every 16 ticks, to 100; at the strike frame the power is the weapon's minimum charge plus the charged share of its range (`struct Weapon`). A bow, crossbow or sling instead switches the cursor to aiming and fires on release.
- a critter: `critter_attack` from the AI, with the attack type's damage and chance from its `Creature` record; a powerful critter adds 7..12 to hit and 4..15 damage.

`do_attack` then finds the target ahead (`resolve_attack`: a motion probe; a wall makes a spark), refuses blows between critters on the same side, works out the angle (`compute_hitangle`, 0 face to face to 4 from behind, added to skill and damage), rolls to hit (`frp_check`: `skill_check(askill + hitangle, defence)`; a critical doubles the damage at even odds and wears the player's armour when he is hit; a bad miss by the player wears his weapon) and rolls the damage (`do_damage`: `(d / 6)d6 + 1d(d % 6)`, d at least 2, scaled by power / 128, less the critter's armour at the hit location, halved for the player in easy mode). The player's attack skill is half the Attack skill plus the weapon skill plus dexterity / 7 (plus 7 on easy); damage is the weapon's damage for the swing kind plus strength / 9, or bare handed 2/5 of the barehand skill plus strength / 6 plus 4. A weapon enchantment adds (effect & 7) + 1 to damage (effect bit 3) or to hit.

A kill gives 4 * exp + 2d(exp) experience from the `Creature` record (1.5 to 3 times for a powerful critter), makes the frame's dragons nod and plays the victory music.

## Damage

`damage_item` (`DAMAGE.C`) is the single entry for damage to anything: the target's resistances first (`check_res`), then critters go to `damage_critter` (`critter/AI.C`) and anything else to `damage_object`. The damage type is a bit set, named in `object.h` after UW2's Study Monster list: `DMG_MAGIC` (0x03; resisted by chance, `(resist & 3)` in 3), `DMG_PHYSICAL` 4, `DMG_FIRE` 8, `DMG_POISON` 0x10, `DMG_COLD` 0x20, `DMG_MISSILE` 0x40, and `RES_UNDEAD` (0x80, a resist bit only). Any other resisted bit cancels the damage. UW1 has no fire against cold doubling.

`damage_object` shifts the damage right by the item's quality class (class 3, and anything with id bit 13, cannot be damaged) and takes it off a mobile's hit points or a static object's quality; a spiked door wears down its strength in the owner field without breaking. A destroyed object (`remove_object`) becomes one of the two piles of debris (UW1 has no broken weapons), a door opens and loses its locks, a chest or barrel spills, a bag drops its contents.

## Missiles

`missile_fire` (`MISSILE.C`) creates the projectile as a mobile object at five sixths of the source's height, turned by the aim, steps it clear of the source and puts it on the map; the motion code flies it and `missile_thwack` (`COMBAT.C`) applies its damage. The front ends are `player_fire` (the projectile keeps the ammunition object's quantity, quality and owner), `critter_fire`, `spell_fire`, `trap_fire` and `ReturnObject`, which throws the cursor object when the pointer is low in the view and otherwise drops it in front of the player.

## Casting

The rune bag holds a bit per rune (`add_rune`); the player puts up to three runes on the shelf and casts (`try_cast`), which looks the packed runes up in the first 48 entries of `spells[]` (eight circles of six). `player_cast` then checks, in order: the player's level ((level + 1) / 2 at least the circle), mana (3 * circle) and `skill_check(Casting + 5, 2 * circle)`; a -1 backfires. The delay before the next cast is (2 * circle - level) * 4 + 0x40 ticks. Mana is paid at once, except for missile spells, which pay when the aimed missile is released.

`do_spell` (`SPELLS.C`) is the one dispatcher, whoever casts: runes, wands, scrolls, enchanted objects (`cast`, by `spells[]` index), critters and traps. A spell is refused on an anti-magic square (tile door byte bit 0: the caster's, or for an object casting classes 0..11 the object's square) and, except for those object casts, on level 9. By class (`SPELLC_*` in `combat.h`): 0..3 start a timed active spell (`set_curmagic`, `game/PLAYTIME.C`), 4 heals, 5 fires a missile (for the player after aiming), 6 hits an area four squares ahead 3d4 times (radius 2), 7 hits a critter in front of the player (fear, smite undead, ally, poison, paralyse), 8 creates food, a rune of warding or a summoned monster, 9 backfires, 10 restores mana, 11 is the extra spells (speed, Detect Monster, the object spells Remove Trap, Name Enchantment and Open, cure poison, Roaming Sight, telekinesis, tremor, gate travel by the moonstone, freeze time, Armageddon), 13 the bullfrog and a hallucination, 14 a cutscene.

## Open questions

- Which of `do_miss`'s effects 7 and 8 is which sound (metal against metal, inferred).
- What the area spell target mode 0xC0 (`AREA_ALL`) was meant to select apart from 0x80: `process_area` treats them alike.
