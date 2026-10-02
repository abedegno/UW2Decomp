# Combat and magic

This page describes melee and missile combat, damage to objects, missiles, the rune bag and spell casting. The sources are in `src/combat`; the declarations are in `src/include/combat.h`, with the per-item tables (`ComObjData`, `Creature`) in `object.h` and `critter.h`.

## Files

| File | Segment | Name | What it does |
|---|---|---|---|
| [`COMBAT.C`](../../src/combat/COMBAT.C) | seg024 | descriptive | a blow: where it lands, what it hits, the to-hit roll, the damage roll, miss sounds, the player's swing and charge, critter attacks, experience for a kill |
| [`DAMAGE.C`](../../src/combat/DAMAGE.C) | seg025 | original (System Shock's `DAMAGE.C` has `damage_object`) | resistances, damage to objects, what a broken object becomes |
| [`MISSILE.C`](../../src/combat/MISSILE.C) | seg027 | descriptive | launching missiles for the player, critters, spells and traps, throwing and dropping |
| [`RUNES.C`](../../src/combat/RUNES.C) | ovr123 | descriptive | the rune bag, the rune shelf, the active spell icons, casting from runes |
| [`SPELLS.C`](../../src/combat/SPELLS.C) | ovr156 | descriptive | the spell dispatcher `do_spell`, the spell table, healing, missiles, area and targeted spells, object spells, special spells |
| [`SPELLS2.C`](../../src/combat/SPELLS2.C) | ovr157 | descriptive | study monster, enchanting and recharging, reveal, creating and summoning, detect monster, tremor, the class 11 spells, lines of power |

## Melee

A blow is a set of globals in `COMBAT.C` filled in by the attacker's side, then `do_attack`:

- the player: `player_attack`, called every frame while a swing is in progress. The swing is the ninth of the 3D view clicked; its row picks the swing kind (`swing_kind`: stab, slash or bash, indexing the weapon's three damage bytes) and the height of the blow. Holding the button charges `play_pow` by the weapon's charge rate every 16 ticks, to 100; at the strike frame the power is the weapon's minimum plus the charged share of its range. `GetPlayerWeapon` and `DoPlayerWeapon` set the attack skill and damage.
- a critter: `critter_attack` from the AI (`critter/AI.C`), with the attack type's damage and chance from its `Creature` record.

`do_attack` then:

1. `resolve_attack` probes ahead of the attacker at the swing's height; on a wall it makes a spark, on an object it picks the nearest (`set_hitobj`) and a hit location (`pickloc`).
2. Critters do not hit their own side.
3. `compute_hitangle` gives 0 (face to face) to 4 (from behind); it is added to both the attack skill and the damage.
4. `frp_check` rolls `skill_check(askill + hitangle, defence)` (`game/SKILLCHK.C`: the difference plus 0..30; over 28 gives 2, a critical; over 15 gives 1; over 2 gives 0; else -1). Results 2 and 1 hit, 0 misses and -1 is a bad miss that can wear the player's weapon; a critical multiplies damage by 1 or 2 at even odds.
5. `do_damage` turns the damage figure into dice, `(d / 6)d6 + 1d(d % 6)` with d at least 2, scales the roll by power / 128, adds the hit angle, subtracts the critter's armour at the hit location (5/3 of it for a powerful critter) and halves damage to the player in easy mode. The result goes to `damage_item`.

Player attack skill is half the Attack skill plus the weapon skill plus `Valor` plus dexterity / 7 (plus 7 on easy). Damage is the weapon's damage for the swing kind plus strength / 9; bare handed, 2/5 of the barehand skill plus strength / 6 plus 4. Weapon enchantments of major class 0xC add damage (effects 0..3: 2e + 1) or accuracy (effects 4..7: 2e - 7); effects 8 and up are special weapons (`specweap`) whose power fires after a hit, on a critical or, for one, every hit: life stealing, repelling undead, a fireball, holding the target, opening doors.

Experience for a kill (`player_killed_a`) is 4 * exp + 2d(exp) from the `Creature` record, 1.5 to 3 times that for a powerful critter, and `player_get_exp` halves it again (rounding at random).

## Damage

`damage_item` (`DAMAGE.C`) is the single entry point for damage to anything. It applies the target's resistances (`check_res`) and sends critters to `damage_critter` (`critter/AI.C`) and everything else to `damage_object`.

The damage type is a bit set. Study Monster names the bits (`dtypes` in `SPELLS2.C`, strings 0x146..0x14B of block 1):

| Bit | Type |
|---|---|
| 0x03 | magic |
| 0x04 | physical attacks |
| 0x08 | fire |
| 0x10 | poison |
| 0x20 | cold |
| 0x40 | missiles |
| 0x80 | undead (a resist bit only: `SPELLS.C` tests it to tell the undead) |

Magic resistance is a chance, `(resist & 3)` in 3; any other resisted bit cancels the damage. Fire against something that resists cold, or cold against something that resists fire but not cold, does double damage. Melee is type 4; spells combine magic with an element (0x13 poison, 0x23 frost, 0x0B the fire of Flame Wind).

`damage_object` shifts the damage right by the item's toughness (`ComObjData`'s qualclass; class 3 is indestructible) and takes it off a mobile's hit points or a static object's quality. Doors keep a separate strength in their owner field and wear down without breaking. Damaging an owned object counts against the player with its owner. A destroyed object becomes debris (`remove_object`, `debris_type`): a weapon its skill's broken weapon (unless the damage was pure fire), furniture wood chips, anything else a pile of debris; doors open and lose their locks, chests and barrels spill.

## Missiles

`missile_fire` (`MISSILE.C`) creates the projectile as a mobile object at the source's eye height, turned by the aim, steps it clear of the source and puts it on the map; the motion code flies it and `missile_thwack` (`COMBAT.C`) applies its damage on a hit. The front ends are `player_fire` (bow, crossbow, sling; the projectile keeps the ammunition object's identity), `critter_fire`, `spell_fire`, `trap_fire` (the missile item is the trap's quality * 32 + owner) and `ReturnObject`, which throws the cursor object when the pointer is low in the view and otherwise drops it in front of the player.

## Casting

The rune bag is `player->runebag`, a bit per rune; the shelf is three runes. `try_cast` (`RUNES.C`) looks the shelf up in `spells[]` (`SPELLS.C`), the 69 spells in the order of their names in string block 6 from 256, eight to a circle; the last five have no runes and are cast only by objects. Each row of the table in the source is commented with its spells' names.

`player_cast`'s checks, with circle = index / 8 + 1:

- In Britannia (world 0, levels 1..8) spells above circle 3 fail ("Casting was not successful").
- The player's level must be at least 2 * circle - 1, and his mana at least 3 * circle.
- `skill_check(Casting, 3 * circle)`: 0 is a failed incantation, -1 a backfire (class 9, (circle / 2)d8 damage to the caster, never below 3 hit points).
- The next cast must wait (2 * circle - level) * 4 + 128 ticks.
- Mana is paid at once, except for missile and targeted spells, which keep the cost in `mspell_mused` until they are released and waive it if they find nothing to act on.

`do_spell` dispatches by class (combat.h's `SPELLC_*`): 0..3 start a timed active spell (`set_curmagic`), 4 heals ((minor)d8, or all for Greater Heal), 5 fires a missile, 6 hits an area ahead of the caster (`area_spells`: the per-square handler, how many times, how far ahead, the radius; the top two bits of the minor choose critters, random squares, every square or every object), 7 waits for a target (`target_spells` for critters, `obj_spells` for objects), 8 creates things, 9 is a backfire, 10 restores mana, 11 is `xt_spells`, 13 the special spells, 14 a cutscene. An anti-magic tile (bit 0 of its door byte) stops every spell.

Some of the spell rules:

- Mana cannot be restored by magic in the Scintillus Academy (world 5) except on its first level and part of its eighth.
- Map Area, Locate and Roaming Sight do nothing in the Ethereal Void (world 8); there Tremor drops the floor's things (skulls, mushrooms, fish, spheres, coins) instead of boulders.
- Repel Undead has a budget of Casting * 10 hit points; undead already fleeing take damage instead.
- Charm works on a critter only if its `whoami` character in string 0x15E or 0x15F is '+'.
- Enchantment on a weapon or armour raises it a tier while the tier is within a limit set by level and Casting, and destroys it past the limit; on a charged object it recharges, with a chance of destruction that grows with the charges and shrinks with the caster's level.
- Gate Travel needs a moonstone placed on another level (`player->moonstones`), or a second moonstone on this one.
- Special spell 0 finds a Guardian magic marker (an invisible Guardian signet ring) within two squares and cuts that world's line of power, a bit in `QB_LINES_OF_POWER`; when all eight are cut quest 3 gets bit 2.
- Casting Iron Flesh while the djinn quest stands at stage 4 glazes the baked mud (stage 5); breaking the djinn's bottle on level 0x45 at the right squares after capture absorbs the djinn (stage 6), and breaking it earlier kills the player (`DAMAGE.C`).

## Open questions

[FINDINGS.md](../FINDINGS.md) collects the open questions and likely bugs of every subsystem in one place.

- Spell 64 (Mass Paralyze by the string order) is class 6, minor 5, which indexes `area_spells[4]`, `sp_repel_undead`, with target mode 0 (critters). Either the string order is off by one there or object-cast Mass Paralyze repels undead; not checked in the game.
- Race 0x17 is special in several places (Smite Undead does half the hit points, Study Monster lists extra spells and a resistance). By the owner race strings (block 1, 0x172 + race, used by `obj/LOOK.C`) it is the liche, if the owner field and the `Creature` race number share a numbering, which is not checked.
- `do_miss`'s two sounds (effects 7 and 8) are chosen by weapon kind and armour kind; which sound is which is not checked.
- The meaning of `Creature` fields `bA_1`, `b0F` (poisonous attacks, from Study Monster's string) and the summoning exclusions 0x7B and 0x7C.
