# Combat

This page describes how a blow, a missile or any other damage is worked out in Ultima Underworld I and II, in the order the game applies the steps. It covers the player's swing, a critter's blow, the to-hit roll, criticals, the damage roll, armour, resistances, damage to objects, missiles, poison, special weapons and the experience for a kill. It is written for someone who wants to reproduce or mod the rules and will not read the C. Every number comes from the matched sources of UW2Decomp and [UW1Decomp](https://github.com/abedegno/UW1Decomp).

Each rule is marked **both**, **UW1** or **UW2**. [UW1-UW2-DIFFERENCES.md](../UW1-UW2-DIFFERENCES.md#combat) lists the differences in more detail. Nothing here was checked in a running game.

Several of the formulas have test vectors, tables of inputs and outputs made by running the original EXE's own code. They are listed in [vectors/README.md](../../uw2/vectors/README.md) and linked below where they apply.

How critters decide to attack, and the strength of their wind-up, is in [npc-ai.md](npc-ai.md#melee). Spells are in the magic notes.

## Contents

- [The dice](#the-dice)
- [The player's swing](#the-players-swing)
- [A critter's blow](#a-critters-blow)
- [Resolving a melee blow](#resolving-a-melee-blow)
- [The to-hit roll](#the-to-hit-roll)
- [The damage roll](#the-damage-roll)
- [Missiles](#missiles)
- [Resistances](#resistances)
- [Damage to critters, the player and objects](#damage-to-critters-the-player-and-objects)
- [The player's armour and defence](#the-players-armour-and-defence)
- [Special weapons (UW2)](#special-weapons-uw2)
- [Experience for a kill](#experience-for-a-kill)

## The dice

**Both.** Two random routines are used throughout.

- **skill_check(value, target)** rolls `value - target + rand() % 31` and grades it. Above 28 is 2, a great success. 16 to 28 is 1, a success. 3 to 15 is 0, a near miss. 2 or less is -1, a failure. With value equal to target, the chances are 2, 13, 13 and 3 in 31. Vectors: [skill_check.csv](../../uw2/vectors/skill_check.csv).
- **rollem(n, s)** rolls n dice of s sides, each 1 to s. Vectors: [rollem.csv](../../uw2/vectors/rollem.csv).

Source: `skill_check` ([SKILLCHK.C:39](../../uw2/src/game/SKILLCHK.C#L39)), `rollem` ([UTIL.C:73](../../uw2/src/sys/UTIL.C#L73)).

## The player's swing

**Both.** The player swings by clicking in the 3D view (or with the attack keys) while his weapon is drawn.

1. **Where.** The view is split into ninths, numbered 1 to 9 from the bottom left. The bottom row is a stab, the middle row a slash and the top row a bash. The kind picks one of the weapon's three damage values. The ninth's number also sets the height of the blow: `z + height * (ninth / 3) / 3`, shifted by the player's pitch, so higher ninths strike higher.
2. **Charge.** While the button or key is held, the power bar goes up by the weapon's charge rate every 16 ticks (1/16 second), to at most 100. Holding the swing makes the player's noise 10. Striking makes it 15. Critters hear by noise ([npc-ai.md](npc-ai.md#perception)).
3. **Power.** At the strike, `power = min + (max - min) * bar / 100`, where min and max are the weapon's power values. Power 128 is a full-strength blow ([The damage roll](#the-damage-roll)).
4. **Attack score.**
   - **UW1.** `Attack / 2 + weapon skill + dexterity / 7`, plus 7 on easy.
   - **UW2.** The same plus Valor, which is 10 + Casting / 5 while the Valor spell is active, else 0.
   - The weapon skill is Sword, Axe or Mace for those weapons, and Unarmed for anything else, including a missile weapon held in melee.
5. **Damage figure.**
   - With a weapon, the weapon's damage for the swing kind plus strength / 9.
   - Bare handed, `Unarmed * 2 / 5 + strength / 6 + 4`.
6. **Weapon enchantment.** A weapon enchantment of class 12 changes the figures.
   - **UW1.** If bit 8 of the effect is set, damage goes up by `(effect & 7) + 1`. Otherwise the attack score does.
   - **UW2.** Effects 0 to 3 add `2 * effect + 1` damage. Effects 4 to 7 add `2 * effect - 7` to the attack score. Effects 8 and up make a special weapon (below).
7. A bow, crossbow or sling instead switches the cursor to aim and fires on release ([Missiles](#missiles)). Without ammunition the game prints "Sorry, you have no ..." and the swing does not start.

The weapon's reach is its object radius (fist: the radius of item 15).

Source: `player_attack`, `GetPlayerWeapon`, `DoPlayerWeapon` ([COMBAT.C:700](../../uw2/src/combat/COMBAT.C#L700), [COMBAT.C:573](../../uw2/src/combat/COMBAT.C#L573), [COMBAT.C:616](../../uw2/src/combat/COMBAT.C#L616)); UW1 [COMBAT.C:650](../../uw1/src/combat/COMBAT.C#L650), [COMBAT.C:572](../../uw1/src/combat/COMBAT.C#L572).

## A critter's blow

**Both.** When a critter's attack animation reaches its strike frame, the blow is set up from its creature entry and the attack it chose:

- **Power** is the charge from its wind-up counter, 50 to 255 ([npc-ai.md](npc-ai.md#melee)).
- **Damage figure** is the attack's damage plus the creature's strength / 5.
- **Attack score** is the attack's chance plus half the creature's equipment value (byte 0x11).
- A **powerful** critter (attitude word bit 10) adds `7 + rand() % 6` to the attack score and `4 + rand() % 12` to the damage figure.
- The reach is 2 fine units, and the height of the blow is chosen at random as for a random ninth.

**Poison.** If the blow hits the player, the attack is poisonous (the creature's poison value, byte 0x0F, is above 0) and the player's poison is below that value, the player may be poisoned:

- **UW1.** Unless he resists poison, his poison becomes the creature's value.
- **UW2.** First `rand() % (poison + 6)` must be greater than twice the player's armour at the location hit. Then, unless he resists poison, his poison becomes the creature's value.

Source: `critter_attack` ([COMBAT.C:891](../../uw2/src/combat/COMBAT.C#L891)); UW1 [COMBAT.C:788](../../uw1/src/combat/COMBAT.C#L788).

## Resolving a melee blow

**Both.** Once the attacker's figures are set, the game resolves the blow in this order.

1. **Find the target.** The game probes a short distance ahead of the attacker, at the height of the swing: a probe of radius reach + 1, centred reach + 3 fine units along the attacker's heading. If it meets objects, it picks the nearest to the attacker, skipping traps and the attacker. The player's blows skip his allies unless an ally is the only thing there. If it meets a wall instead, a spark appears on the wall and the blow is a miss.
2. **Same side.** A critter's blow on another critter does nothing when both are allies of the player or neither is.
3. **Hit location** (0 to 3) is chosen from the heights of the blow and the target. A blow whose middle is below the target's bottom hits location 2. One whose middle is above the target's top hits location 3. Otherwise the game chooses at random: if the middle of the blow is in the lower half of the target, location 2 half the time, and if it is in the upper half, location 3 one time in three. Failing that, location 0 two times in three, else location 1. The locations index the creature's armour values. For the player, location 0 is the body, 1 the hands, 2 the legs and feet, and 3 the head (from the slot table, inferred). Vectors: [pickloc.csv](../../uw2/vectors/pickloc.csv).
4. **Hit angle** is how far round from the target's front the attacker stands, in eighths of a turn, from 0 (face to face) to 4 (from behind), using the two coarse headings. It is added to the attack score and later to the damage. Vectors: [compute_hitangle.csv](../../uw2/vectors/compute_hitangle.csv).
5. **To-hit roll** (below). On a miss, the target is still sent a damage of 0, so it notices the attack (inferred), and the miss sound plays.
6. **Damage roll** (below) on a hit.

Source: `do_attack`, `resolve_attack`, `set_hitobj`, `pickloc`, `compute_hitangle` ([COMBAT.C:526](../../uw2/src/combat/COMBAT.C#L526), [COMBAT.C:229](../../uw2/src/combat/COMBAT.C#L229), [COMBAT.C:124](../../uw2/src/combat/COMBAT.C#L124), [COMBAT.C:98](../../uw2/src/combat/COMBAT.C#L98), [COMBAT.C:516](../../uw2/src/combat/COMBAT.C#L516)); UW1 [COMBAT.C:491](../../uw1/src/combat/COMBAT.C#L491), [COMBAT.C:107](../../uw1/src/combat/COMBAT.C#L107).

## The to-hit roll

**Both.**

1. **A target that is not a creature** (a door, a barrel) is always hit. If the player strikes a door, his weapon may wear: with chance `2 * (door id & 7)` in 12 it takes `rollem(2, 4)` wear.
2. **Against a creature**, if the target is the player, his protection at the hit location is taken off the attack score first ([The player's armour](#the-players-armour-and-defence)).
3. The game rolls `skill_check(attack score + hit angle, target's defence)`. The defence is the creature's defence value. The player's defence is worked out from his skills.
4. **UW2, poisoned weapon.** While the Poison Weapon spell is active, if the player's weapon is sharp and the target bleeds, the damage figure goes up by `damage * (Casting + 30) / 40`. The game applies this for any blow while the spell is on, before it looks at the result, and only the player's weapon is tested. Vectors for "sharp": [is_sharp.csv](../../uw2/vectors/is_sharp.csv).
5. **Great success (2)** is a critical. The damage figure is multiplied by 1 or 2 at even odds. If the player was hit, the screen flashes and one item takes `rollem(2, 4)` wear: the helm for a head hit, the leggings for a leg hit (the boots one time in five), and the item in the off hand for a body or hand hit (slot names inferred from the slot numbers). The blow hits.
6. **Success (1)** hits.
7. **Near miss (0)** misses.
8. **Failure (-1)** misses. If the player made the blow and the target is not a passive creature, his weapon takes `rollem(2, 3)` wear.

**Miss sounds.** A swing at nothing makes a whoosh (sound 10) unless it hit a wall. A blocked blow makes sound 7 for a blunt weapon, or a sharp weapon against metal armour, and sound 8 otherwise. On the player, leather pieces count as not metal. **UW2** records which kind of weapon the player swings, to choose these sounds. **UW1** plays fixed sounds, and its test of the attacker has a bug described in UW1Decomp's [FINDINGS.md](../../uw1/docs/FINDINGS.md).

Source: `frp_check`, `do_miss`, `is_sharp` ([COMBAT.C:300](../../uw2/src/combat/COMBAT.C#L300), [COMBAT.C:466](../../uw2/src/combat/COMBAT.C#L466), [COMBAT.C:275](../../uw2/src/combat/COMBAT.C#L275)); UW1 [COMBAT.C:282](../../uw1/src/combat/COMBAT.C#L282), [COMBAT.C:429](../../uw1/src/combat/COMBAT.C#L429).

## The damage roll

**Both.** On a hit the damage figure becomes dice, and then armour comes off.

1. The figure is at least 2.
2. The roll is `rollem(figure / 6, 6) + rollem(1, figure % 6)`. So a figure of 14 rolls 2d6 + 1d2.
3. The roll is scaled by the power: `roll * power / 128`, rounded down. A full-power player blow (power 128) keeps the roll, a weak one less, and a critter's wound-up blow (up to 255) up to about twice.
4. The hit angle is added.
5. **Armour.** If the target is a creature, its armour at the hit location is taken off, to a minimum of 0. An armour value of 0xFF means "use location 0". A powerful critter's armour counts 5/3 (`armour * 5 / 3`). The player's armour comes from his equipment.
6. **Easy mode** halves the damage to the player.
7. The result goes to the target with damage type 4 (physical) through the resistance check ([Resistances](#resistances)).

When the player hits a critter, the view frame shows its health in three steps (`hp * 3 / creature hit points`). A bleeding creature splashes blood, twice on the player's critical.

Source: `do_damage` ([COMBAT.C:358](../../uw2/src/combat/COMBAT.C#L358)); UW1 [COMBAT.C:333](../../uw1/src/combat/COMBAT.C#L333).

## Missiles

**Both.** A missile (an arrow, a bolt, a sling stone, a spell's missile, a thrown object) flies by the physics. There is no to-hit roll: if the missile touches an object, it hits.

1. The damage figure is the missile type's damage value.
2. For a missile the player fired whose type has the ammunition value 0xC0 (the arrows, bolts and stones, inferred), the figure is scaled by the Missile skill. The scale is `Missile * 8 + 0xC0` in 256ths, so 0.75 plus Missile / 32. Then the game rolls `skill_check(Missile, 10)`. A failure takes 0x80 off the scale and a great success adds 0xC0.
3. The hit is then resolved as a damage roll ([The damage roll](#the-damage-roll)) with power 128 (no scaling), a hit angle of 0, no critical, and a hit location from the heights (the blood height uses a separate table for missile hits). The damage type is minus the missile type's ammunition byte, so 0xC0 gives type 0x40 (missiles).

**Critters' missiles.** A critter fires its missile weapon on its strike frame ([npc-ai.md](npc-ai.md#spells-and-missiles)). The missile is aimed up or down by the height difference, and the damage follows the same steps without the skill scaling.

Source: `missile_newhit` ([OBJPHYS.C:90](../../uw2/src/motion/OBJPHYS.C#L90)), `missile_thwack` ([COMBAT.C:861](../../uw2/src/combat/COMBAT.C#L861)); UW1 [OBJPHYS.C:102](../../uw1/src/motion/OBJPHYS.C#L102), [COMBAT.C:761](../../uw1/src/combat/COMBAT.C#L761).

## Resistances

**Both.** Every source of damage passes through one check. The damage has a type, a set of bits, and each kind of object has a resist byte in its object data. The bits, named by the Study Monster spell's list:

| Bit | Type |
| --- | --- |
| 0x03 | magic (the two low bits) |
| 0x04 | physical blows |
| 0x08 | fire |
| 0x10 | poison |
| 0x20 | cold |
| 0x40 | missiles |
| 0x80 | marks the undead (a resist bit only) |

The check, in order:

1. If no bit of the damage type is in the resist byte, nothing changes, except for step 4 in UW2.
2. **Magic.** If the damage has a magic bit, the target ignores it entirely with chance `(resist & 3)` in 3. Otherwise the magic bits are taken off the damage type.
3. **Other types.** If any remaining bit is resisted, the damage becomes 0.
4. **UW2 only.** Fire damage on something that resists cold, or cold damage on something that resists fire but not cold, is doubled, to at most 0xFF.

Spells combine magic with an element, e.g., 0x13 poison or 0x23 frost, so a magic resistance roll comes first and then the element. Vectors for both games: [check_res.csv](../../uw2/vectors/check_res.csv).

Source: `check_res` ([DAMAGE.C:210](../../uw2/src/combat/DAMAGE.C#L210)); UW1 [DAMAGE.C:166](../../uw1/src/combat/DAMAGE.C#L166).

## Damage to critters, the player and objects

**Both.** After resistances, damage goes to a critter (the player counts as one) or to an object.

**Critters and the player.** The hit points go down by the damage. The damage is also added to the critter's damage tally, which its decision to flee reads ([npc-ai.md](npc-ai.md#fleeing-and-being-cornered)). At 0 the critter dies, unless a plot NPC's scripted death keeps it alive ([npc-ai.md](npc-ai.md#death)). The kill is credited to the attacker, or to the shooter of a missile. Hitting a critter angers it and its kin ([npc-ai.md](npc-ai.md#how-hostility-starts-and-spreads)).

**Objects** take damage by their toughness (the object data's quality class 0 to 3):

1. **UW1.** An object with a door direction can't be damaged. **UW2.** Only objects other than doors with that bit are undamageable.
2. Toughness 3 is indestructible. Otherwise the damage is shifted right by the toughness, so toughness 1 halves it and 2 quarters it. If nothing is left, nothing happens.
3. A mobile object loses hit points.
4. **UW2.** A door (items 0x140 to 0x147) whose owner field has bit 0 set keeps a strength in the rest of the owner field. The strength wears down and the door is never destroyed by damage.
5. Any other object loses quality. **UW2.** Damaging an object that can be owned counts against the player with its owner, as theft does ([npc-ai.md](npc-ai.md#how-hostility-starts-and-spreads)).
6. At 0 the object is destroyed. A destroyed static object sets off any trap attached to it, as if it were used.

**What a destroyed object becomes.**

- **UW1.** One of two debris items at random.
- **UW2.** A weapon (items 0 to 15) becomes its skill's broken weapon, and a dagger a broken dagger, unless the damage was pure fire. Furniture becomes wood chips. Anything else becomes a pile of debris. Vectors: [debris_type.csv](../../uw2/vectors/debris_type.csv).

Source: `damage_item`, `damage_object`, `debris_type`, `remove_object` ([DAMAGE.C:238](../../uw2/src/combat/DAMAGE.C#L238), [DAMAGE.C:257](../../uw2/src/combat/DAMAGE.C#L257), [DAMAGE.C:190](../../uw2/src/combat/DAMAGE.C#L190), [DAMAGE.C:83](../../uw2/src/combat/DAMAGE.C#L83)), `damage_critter` ([AI.C:1469](../../uw2/src/critter/AI.C#L1469)); UW1 [DAMAGE.C:205](../../uw1/src/combat/DAMAGE.C#L205), [DAMAGE.C:140](../../uw1/src/combat/DAMAGE.C#L140).

## The player's armour and defence

**Both.** The player's defensive figures are worked out again whenever his equipment or active spells change.

- **Armour by location.** Each worn piece in body slots 0 to 4 gives `quality * protection / 64 + 1` (protection from the armour table, shifted right after the multiply) to one location: slot 0 to location 3, slot 1 to location 0, slot 2 to location 1, slots 3 and 4 to location 2. A shield (items 11 to 15 of the armour class) in the off hand adds its value to locations 0 and 1.
- **Defence**, the target value critters roll against, is the Defense skill plus half the skill of the weapon in hand: Sword, Axe or Mace for a hand weapon (clamped to those three), else Unarmed.
- **Protection spells.** A protection spell (class 3, minor 1) adds 3 to every location's protection, which comes off a critter's attack score. An armour spell (class 2) adds its level to every location's armour. Enchanted worn items add their own protection (minor bits 0 to 2, plus 1) or, with the 8 bit set, toughness to armour. For items in slots above 4 both locations 0 and 1 are meant, but the toughness is always added to the first location, a slip noted in [FINDINGS.md](../../uw2/docs/FINDINGS.md).
- **Resistance spells** (class 3, minors 5 to 9) add missile, fire, poison and magic resistance bits to the player's resist byte.

Source: `FixPlayerEquips`, `armor_val`, `player_affected_by`, `parse_spells` ([PLAYDATA.C:347](../../uw2/src/game/PLAYDATA.C#L347), [PLAYDATA.C:322](../../uw2/src/game/PLAYDATA.C#L322), [PLAYDATA.C:197](../../uw2/src/game/PLAYDATA.C#L197), [PLAYDATA.C:294](../../uw2/src/game/PLAYDATA.C#L294)).

## Special weapons (UW2)

**UW2.** A weapon enchantment of effect 8 or more makes a special weapon, kind `effect - 7`. After a hit that is a critical, or after every hit for kind 6, its power fires:

| Kind | Power |
| --- | --- |
| 1 | the player gains hit points equal to the damage done |
| 2 | Smite Undead (repel undead) on the target |
| 3 | a fireball on the target's square |
| 4 | Paralyze on the target |
| 5, 6 | if the target is a door, it is unlocked and opened |

The special weapons also have damage and attack bonus tables. Every entry is 0 except one byte of 5. By the layout of the data, that byte is the attack bonus of an unused kind 0, or the damage bonus of kind 8 if the damage table runs on into it. The bytes do not show which, so whether any special weapon has a bonus is not known.

Source: `player_attack`, `spec_skill` ([COMBAT.C:790](../../uw2/src/combat/COMBAT.C#L790), [COMBAT.C:59](../../uw2/src/combat/COMBAT.C#L59)).

## Experience for a kill

**Both.** When the player kills a critter, the victory music plays and the player gains `4 * exp + rollem(2, exp)` experience, where exp is the creature's experience value. A powerful critter gives `* (24 + rand() % 24) / 16` of that, 1.5 to nearly 3 times. The gain then goes through the general experience rules, which halve it in UW2 and may halve it again in both games when the player has outgrown the area ([player-upkeep.md](player-upkeep.md#experience-and-levels)).

Source: `player_killed_a` ([COMBAT.C:935](../../uw2/src/combat/COMBAT.C#L935)); UW1 [COMBAT.C:830](../../uw1/src/combat/COMBAT.C#L830).
