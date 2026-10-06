# Magic

This page describes how spells work in Ultima Underworld I and II: casting from runes and the checks it makes, mana, how long active spells last, and what each class of spell does. The spell list is UW2's. It is written for someone who wants to reproduce or mod the rules and will not read the C. Every number comes from the matched sources of both games in this repository ([uw1/](../../uw1/), [uw2/](../../uw2/)).

Each rule is marked **both**, **UW1** or **UW2**. [UW1-UW2-DIFFERENCES.md](../UW1-UW2-DIFFERENCES.md#magic) lists the differences in more detail, and this page does not repeat all of them. UW1's own spell list (`spells[]` in UW1's [SPELLS.C:838](../../uw1/src/combat/SPELLS.C#L838)) is not tabulated here. Nothing here was checked in a running game.

Damage from spells goes through the resistance check described in [combat.md](combat.md#resistances). Spells cast by critters are chosen by the AI ([npc-ai.md](npc-ai.md#spells-and-missiles)).

## Contents

- [Casting from runes](#casting-from-runes)
- [Where magic fails](#where-magic-fails)
- [Mana](#mana)
- [Active spells and their durations](#active-spells-and-their-durations)
- [The spell classes](#the-spell-classes)
- [Area spells](#area-spells)
- [Spells on a target](#spells-on-a-target)
- [Spells on an object](#spells-on-an-object)
- [Other spells](#other-spells)
- [The UW2 spell list](#the-uw2-spell-list)
- [Open questions](#open-questions)

## Casting from runes

**Both.** The player puts up to three runes on the shelf and casts. The game packs the three runes, five bits each, and looks them up in the first 64 entries of the spell table. No match prints "Not a spell". A match is cast with these checks, in order. The circle is `index / 8 + 1` in UW2 and `index / 6 + 1` in UW1.

1. **UW2.** A paralysed player can't cast, and nothing happens.
2. **Too soon.** If less time has passed since the last cast than the delay set by that cast, the game plays a sound and refuses. **UW2** also prints "You are not yet ready to cast another spell."
3. **UW2, Britannia.** On levels 1 to 8, a spell above circle 3 fails with "Casting was not successful."
4. **Level.** The player's experience level must be at least `2 * circle - 1`. Otherwise "You are not experienced enough to cast spells of that circle."
5. **Mana.** The player needs at least `3 * circle` mana. Otherwise "You do not have enough mana".
6. **Skill roll.** **UW2** rolls `skill_check(Casting, 3 * circle)`. **UW1** rolls `skill_check(Casting + 5, 2 * circle)`. A result of 0 fails with "The incantation failed." and costs nothing. A result of -1 is a backfire: "The spell backfires." and the spell is replaced by a backfire of strength `circle / 2`.
7. **Delay.** The next cast must wait `(2 * circle - level) * 4 + 0x80` game clock units in UW2, and `+ 0x40` in UW1, where level is the experience level. The game clock counts 256 units a second, so a circle 1 spell by a level 16 character waits 72 units in UW2, about a quarter of a second.
8. **Payment.** The cost is `3 * circle` mana. It is paid now, except for missile spells and spells aimed at one target (UW2) or missile spells only (UW1). Those keep the cost until they are released, and the player pays only if the spell found something to act on.
9. **Cast.** If the spell does not take (e.g., an anti-magic square, or a fourth active spell), the game prints "Casting was not successful." A spell paid in step 8 is not refunded.

Source: `try_cast`, `player_cast`, `fail_spell` ([RUNES.C:167](../../uw2/src/combat/RUNES.C#L167), [RUNES.C:219](../../uw2/src/combat/RUNES.C#L219), [RUNES.C:202](../../uw2/src/combat/RUNES.C#L202)); UW1 [RUNES.C:236](../../uw1/src/combat/RUNES.C#L236).

## Where magic fails

- **Both.** On an anti-magic square (bit 0 of the tile's door byte), no spell works, whoever casts it. An object that casts is tested at its own square.
- **UW1.** On level 9, the Ethereal Void, no spell cast by the player or a critter works.
- **UW2.** In the Ethereal Void (world 8), Map Area, Locate and Roaming Sight have no effect, and Tremor drops the floor's things instead of boulders.

Source: `do_spell`, `anti_magic_p` ([SPELLS.C:83](../../uw2/src/combat/SPELLS.C#L83), [SPELLS.C:54](../../uw2/src/combat/SPELLS.C#L54)); UW1 [SPELLS.C:95](../../uw1/src/combat/SPELLS.C#L95).

## Mana

**Both.** Mana comes back in these ways:

- **Every minute** (every third slow update, see [schedules.md](schedules.md#game-time)), the game rolls `skill_check(Mana, 10)`. A success gives 1 mana and a great success 2.
- **The regeneration effect** (an active class 11 effect, minor 15) gives 1 mana every slow update, every 20 seconds.
- **Mana spells** (class 10) of strength n give `max_mana * (n + rand() % 4) / 16 + 1`.
- **Sleep** gives mana through the same rules ([player-upkeep.md](player-upkeep.md)).

Mana never goes above the maximum.

**UW2, the Scintillus Academy.** In world 5 (levels 41 to 48), none of these give mana, except on the Academy's first level, and on its eighth level at x 25 or more.

**UW2** also calls the mana routine with strength 0 after every level change and every recalculation of the player's figures. Strength 0 adds nothing, so the call only caps mana at the maximum and redraws the panel.

Source: `restore_mana` ([SPELLS.C:155](../../uw2/src/combat/SPELLS.C#L155)), `duration_check` ([PLAYTIME.C:71](../../uw2/src/game/PLAYTIME.C#L71)); UW1 [SPELLS.C:168](../../uw1/src/combat/SPELLS.C#L168).

## Active spells and their durations

**Both.** Spells of classes 0 to 3 (light, motion, armour, protection), and some of class 11, become active spells on the player. A critter casting one of these gets nothing.

- The player can have at most three active spells. A fourth is refused.
- The duration is counted in slow updates (20 seconds each), from the spell's stability, which is the top two bits of its minor:

| Stability | Duration in updates | In time |
| --- | --- | --- |
| 0x00 | `rollem(2, 3)`, 2 to 6 | 40 seconds to 2 minutes |
| 0x40 | `rollem(2, 8) + 6`, 8 to 22 | about 3 to 7 minutes |
| 0x80 | `rollem(3, 20) + 24`, 27 to 84 | 9 to 28 minutes |

- Each slow update takes one step off every active spell, and a spell at its last step ends.
- Levitate and Fly do not end at once. They become Slow Fall for one more update, so the player is not dropped.
- The player can check or end an active spell from the spell icons. The game reports "is nearly done" (2 updates or fewer left), "is unstable" (up to 10) or "is stable".

**What the active effects do,** worked out whenever equipment or spells change ([combat.md](combat.md#the-players-armour-and-defence)):

| Class, minor | Effect |
| --- | --- |
| 0, n | light of level n, if brighter than the light carried |
| 1, n | motion: 1 leaping, 2 slow fall, 3 levitate, 4 water walk, 5 fly, 6 bouncing (from the Guide's order of the motion spells) |
| 2, n | armour: n added to the armour at every hit location (the best one counts) |
| 3, 1 | protection: 3 off every attacker's score at every location |
| 3, 2 to 4 | stealth: noise down by 16 (2), visibility down by 5 (3), both visibility steps (4) |
| 3, 5 to 9 | resistance to missiles, fire, poison, magic, and the second magic bit |
| 3, 10 (UW2) | Valor, `10 + Casting / 5` added to the attack score |
| 3, 11 (UW2) | Poison Weapon ([combat.md](combat.md#the-to-hit-roll)) |
| 11, 0 | time stop |
| 11, 1 | roaming sight (the camera leaves the player) |
| 11, 2 | haste |
| 11, 3 | telekinesis (sets the pick distance to 0, inferred to lift the reach limit) |
| 11, 14 | 1 hit point every update |
| 11, 15 | 1 mana every update |

Source: `set_curmagic`, `dispel_spell` ([PLAYTIME.C:246](../../uw2/src/game/PLAYTIME.C#L246), [PLAYTIME.C:47](../../uw2/src/game/PLAYTIME.C#L47)), `player_affected_by`, `parse_spells` ([PLAYDATA.C:197](../../uw2/src/game/PLAYDATA.C#L197), [PLAYDATA.C:294](../../uw2/src/game/PLAYDATA.C#L294)), the spell icons ([RUNES.C:135](../../uw2/src/combat/RUNES.C#L135)).

## The spell classes

**Both.** Every spell, whoever casts it (the player, a critter, a wand, a scroll, a trap or a conversation), goes through one dispatcher by class:

| Class | What it does |
| --- | --- |
| 0 to 3 | an active spell on the player (above) |
| 4 | heal: `rollem(minor, 8)` hit points, or all of them for minor 15. **UW2** heals the caster, **UW1** the target. |
| 5 | a missile: the player aims with the cursor and the spell flies when he clicks. **UW2** minors: 1 magic arrow, 2 lightning, 3 fireball, 4 acid, 5 deadly seeker (homing), 6 snowball. "There is not enough room to release that spell." if it can't be launched. |
| 6 | an area spell ahead of the caster (below) |
| 7 | the player picks a target with the cursor: minors 0 to 7 a critter, 8 and up an object (below) |
| 8 | create something ahead of the caster (below) |
| 9 | backfire: `rollem(minor, 8)` damage to the caster, never taking it below 3 hit points |
| 10 | mana: `max_mana * (minor + rand() % 4) / 16 + 1` |
| 11 | the extra spells (below) |
| 13 | special spells (below) |
| 14 | a cutscene |

Source: `do_spell`, `healing`, `backfire`, `release_missile` ([SPELLS.C:83](../../uw2/src/combat/SPELLS.C#L83), [SPELLS.C:207](../../uw2/src/combat/SPELLS.C#L207), [SPELLS.C:222](../../uw2/src/combat/SPELLS.C#L222), [SPELLS.C:246](../../uw2/src/combat/SPELLS.C#L246)).

## Area spells

**UW2.** An area spell acts on the squares round a point ahead of the caster. Its minor picks one of seven handlers, each with how many times it may act, how many squares ahead the centre is, and the radius of the square of squares round it. The top two bits of the minor say what it acts on:

| Mode | Acts on |
| --- | --- |
| 0x00 | every critter except the caster |
| 0x40 | random open squares, each with chance count in `w * h + 3`, in up to five passes until the count is used |
| 0x80 | every open square, and every object on it |
| 0xC0 | every object |

| Minor (low bits) | Handler | Times | Ahead | Radius | Per square |
| --- | --- | --- | --- | --- | --- |
| 1 | Reveal | 100 | 1 | 2 | for each object there that holds a look trigger, sets the trigger off as a search with Search skill 0x2D (reveals hidden things, inferred) |
| 2 | Sheet Lightning | 6 | 4 | 2 | lightning, `rollem(6, 5)` damage of type 3 (magic) to every object there |
| 3 | Mass Confusion | 12 | 4 | 2 | a critter wanders (goal 2) and becomes upset, unless it resists magic |
| 4 | Flame Wind | 10 | 4 | 2 | an explosion, `rollem(10, 6)` of type 11 (magic and fire) to every object there; at most 5 critters, never the same square twice in a row, an empty square only one time in three |
| 5 | Repel Undead | 50 | 4 | 2 | player only. Up to a budget of `Casting * 10` hit points, a floating skull is destroyed, an undead critter is made to flee, and one already fleeing takes `Casting` damage of type 3 |
| 6 | Shockwave | 50 | 0 | 1 | `Casting / 2 + 15` damage of type 3 to every critter but the caster |
| 7 | Frost | 5 | 3 | 1 | frost effects, 10 damage of type 0x23 (magic and cold) |

**UW1.** Every area spell acts `rollem(3, 4)` times on squares within 2 of the square 4 ahead. UW1's Flame Wind also hits the four squares beside each target. See [UW1-UW2-DIFFERENCES.md](../UW1-UW2-DIFFERENCES.md#area-spells-class-6).

Source: `nail_area`, `area_spells`, `process_area`, `gronk_area`, the handlers, `damage_square` ([SPELLS.C:746](../../uw2/src/combat/SPELLS.C#L746), [SPELLS.C:729](../../uw2/src/combat/SPELLS.C#L729), [SPELLS.C:602](../../uw2/src/combat/SPELLS.C#L602), [SPELLS.C:674](../../uw2/src/combat/SPELLS.C#L674), [SPELLS.C:277](../../uw2/src/combat/SPELLS.C#L277) to [SPELLS.C:581](../../uw2/src/combat/SPELLS.C#L581), [SPELLS.C:1061](../../uw2/src/combat/SPELLS.C#L1061)).

## Spells on a target

**UW2.** The player clicks a critter. The mana is paid only if the spell took. "Resists magic" below means the critter passes a magic resistance roll, chance `(resist & 3)` in 3.

| Minor | Spell | Effect |
| --- | --- | --- |
| 0 | Bleeding | `Casting / 2 + 10` damage of type 4, only to a creature that bleeds ("That creature does not bleed.") |
| 1 | Cause Fear | the critter becomes upset, then flees (goal 6) unless it resists magic |
| 2 | Smite Undead | a floating skull is destroyed. An undead critter takes 255 damage of type 3, or half its hit points if its race is 0x17 (a liche, inferred) |
| 3 | Charm | unless it resists magic, and only if its conversation number is marked `+` in string 0x15E (0x15F for conversations 0x8C and up), it becomes friendly and mills (goal 8). In the Pits of Carnage it leaves the fight |
| 4 | Poison | `rollem(5, 4)` damage of type 0x13 (magic and poison) |
| 5 | Paralyze | a critter that is not undead is held (goal 15) for `16 + rand() % 16 * power` AI steps, power `Casting / 3` for the player and 8 for others, unless it resists magic, and becomes upset |
| 6 | Smite Foe | `Casting * 3 + 110` damage of type 4, only to a creature that bleeds |
| 7 | Study Monster | names the critter's hit points, attacks, resistances and spells |

Only creatures are affected by Poison, Fear, Charm and Confusion. **UW1** has five targeted handlers, cast on the square 4 ahead without aiming (Fear, Smite Undead, Ally, Poison, Paralyze); see [UW1-UW2-DIFFERENCES.md](../UW1-UW2-DIFFERENCES.md#spells-on-one-critter-class-7).

The held critter's count is in AI steps, and a held critter thinks at rate 7, about twice a second ([npc-ai.md](npc-ai.md#when-a-critter-thinks)).

Source: `target_spells`, `area1_spells`, `sp_bleed`, `sp_fear`, `sp_ward_undead`, `sp_charm`, `sp_poison`, `sp_hold`, `sp_smite`, `hit_critter_goal` ([SPELLS.C:758](../../uw2/src/combat/SPELLS.C#L758), [SPELLS.C:739](../../uw2/src/combat/SPELLS.C#L739), [SPELLS.C:473](../../uw2/src/combat/SPELLS.C#L473), [SPELLS.C:533](../../uw2/src/combat/SPELLS.C#L533), [SPELLS.C:331](../../uw2/src/combat/SPELLS.C#L331), [SPELLS.C:379](../../uw2/src/combat/SPELLS.C#L379), [SPELLS.C:351](../../uw2/src/combat/SPELLS.C#L351), [SPELLS.C:581](../../uw2/src/combat/SPELLS.C#L581), [SPELLS.C:490](../../uw2/src/combat/SPELLS.C#L490), [SPELLS.C:364](../../uw2/src/combat/SPELLS.C#L364)), `sp_study_monster` ([SPELLS2.C:121](../../uw2/src/combat/SPELLS2.C#L121)).

## Spells on an object

**UW2.** The player clicks an object. Where the spell finds nothing to work on, its cost is waived.

| Minor | Spell | Effect |
| --- | --- | --- |
| 8 | Dispel Rune | removes a rune of flame or of stasis ("That is not a rune.") |
| 9 | Mending | a mendable object's quality becomes 0x3F |
| 10 | Remove Trap | tries to find and disarm a trap on it, as with Search 0x2D |
| 11 | Name Enchantment | identifies the object |
| 12 | Open | tries the lock as a lock pick of skill 45 ("The spell unlocks the lock.") |
| 13 | Detect Trap | looks for a trap, as with Search 0x2D |
| 14 | Enchantment | below |
| 15 | Gate Travel | on a moonstone: to the level where the other moonstone was left, or to the other moonstone on this level ("The moonstone is not available.") |

**Enchantment.** As the source summarises it:

- An object with spell charges is recharged with `1 + Casting / 15` charges, or destroyed in a blaze of fire with a chance that grows with the charges and shrinks with the caster's level. Food, reagents, books and two spells can't be recharged, and trying destroys the object.
- A weapon with a class 12 enchantment goes up a tier while the tier is at most `(level - 8) / 4 + Casting / 11`, and is destroyed past that. An armour piece likewise while the tier is at most `level + Casting / 11 - 10`.
- An unenchanted weapon or armour with nothing inside gets the lowest tier of one of its two kinds at random.

Source: `obj_spells` ([SPELLS.C:806](../../uw2/src/combat/SPELLS.C#L806)), `sp_enchant`, `charge_object` ([SPELLS2.C:310](../../uw2/src/combat/SPELLS2.C#L310), [SPELLS2.C:262](../../uw2/src/combat/SPELLS2.C#L262)).

## Other spells

**Creating** (class 8), **UW2.** The object appears at a square about 9 fine units ahead of the caster, turned up to about 18 degrees either side at random. "There is no room to create that." if it does not fit.

| Minor | Creates |
| --- | --- |
| 1 | one of seven foods |
| 2 | a rune of flame, at the caster's height |
| 3 | a rune of stasis, at the caster's height |
| 4 | a random creature numbered `lvl` to `2 * lvl - 1` from the first creature, `lvl` being the player's Casting or a critter caster's dungeon level times 4 (at least 2). Swimmers, creatures with no hit points or the "ignores fights" bit, and items 0x7B and 0x7C are rerolled. The player's summon becomes an ally |
| 5 | a demon by `(Casting + rand() % 30) / 12`: imp, imp, hordling, despoiler, destroyer. **The demon is always hostile and heads for the player, even when the player summoned it.** |
| 6 | a magic satellite that circles |

Summoned creatures are temporary, so they vanish when time passes for the level ([npc-ai.md](npc-ai.md#while-the-player-is-away-or-asleep)).

**The extra spells** (class 11), **UW2**, the player's only except Tremor:

| Minor | Spell | Effect |
| --- | --- | --- |
| 0 | Speed | active haste |
| 1 | Portal | moves the player 2 squares ahead, if the floor there is at most 2 higher and there is room ("There is no suitable space.") |
| 2 | Restoration | cures poison, drugs, mushrooms, drink, paralysis, hunger and fatigue, and restores all hit points |
| 3 | Locate | turns the automap on ("Your position is revealed unto you."), not in the Void |
| 6 | Cure Poison | poison to 0 |
| 7 | Roaming Sight | active, the camera leaves the player; not in the Void |
| 8 | Telekinesis | active, see above |
| 9 | Tremor | `rollem(8, 3)` falling objects on random squares within 3 of a point 5 ahead, and the screen shakes |
| 10 | Gate Travel | to the level of the first moonstone |
| 11 | Freeze Time | active time stop |
| 12 | Armageddon | destroys the player's inventory and every object on the level, and empties the rune bag and shelf |
| 13 | Dispel Hunger | hunger to 0xC0 ("You feel well fed.") |

**The special spells** (class 13), **UW2**:

| Minor | Effect |
| --- | --- |
| 0 | finds a Guardian magic marker within two squares and cuts that world's line of power |
| 2 | Mind Blast by a critter: strength is the critter's intelligence minus the player's, plus `rand() % 6 - rand() % 6`, at least 2. The player takes half of it as damage and loses a quarter of it in mana, and the screen shakes |
| 3, 4 | the view swings 3 either way, and unless the player is already hallucinating or passes `skill_check(intelligence, 20)`, he hallucinates (level 2) |
| 5 | hallucination level 3 |
| 7 | Map Area: maps a circle of radius `r / 5 + 2` round the player, where r is Casting, plus `Casting - 13` more above 15; not in the Void |
| 8, 9 | the acid and snowball missiles |
| 12 to 15 | mana of strength 3, 7, 11 and 15 |

Source: `creat_spell`, `xt_spells`, `special_spells` ([SPELLS2.C:471](../../uw2/src/combat/SPELLS2.C#L471), [SPELLS2.C:695](../../uw2/src/combat/SPELLS2.C#L695), [SPELLS.C:944](../../uw2/src/combat/SPELLS.C#L944)).

## The UW2 spell list

**UW2.** The 69 spells, in the order of their names in string block 6 from 256. The first 64 have runes, eight to a circle. The last five have no runes and are cast by objects. Class and minor are from the spell table. A minor with 0x40 or 0x80 added carries the stability or the area mode.

| # | Spell | Circle | Class | Minor |
| --- | --- | --- | --- | --- |
| 0 | Create Food | 1 | 8 create | 1 |
| 1 | Luck | 1 | 3 protection | 1 |
| 2 | Magic Arrow | 1 | 5 missile | 1 |
| 3 | Resist Blows | 1 | 2 armour | 2 |
| 4 | Detect Trap | 1 | 7 target | 0xD |
| 5 | Light | 1 | 0 light | 0x83 |
| 6 | Bouncing | 1 | 1 motion | 6 |
| 7 | Locate | 1 | 11 extra | 3 |
| 8 | Cause Fear | 2 | 7 target | 1 |
| 9 | Valor | 2 | 3 protection | 0xA |
| 10 | Deadly Seeker | 2 | 5 missile | 5 |
| 11 | Lesser Heal | 2 | 4 heal | 2 |
| 12 | Rune of Flame | 2 | 8 create | 2 |
| 13 | Slow Fall | 2 | 1 motion | 2 |
| 14 | Leaping | 2 | 1 motion | 1 |
| 15 | Dispel Hunger | 2 | 11 extra | 0xD |
| 16 | Bleeding | 3 | 7 target | 0 |
| 17 | Cure Poison | 3 | 11 extra | 6 |
| 18 | Dispel Rune | 3 | 7 target | 8 |
| 19 | Lightning | 3 | 5 missile | 2 |
| 20 | Night Vision | 3 | 0 light | 0x85 |
| 21 | Repel Undead | 3 | 6 area | 0x85 |
| 22 | Speed | 3 | 11 extra | 0 |
| 23 | Water Walk | 3 | 1 motion | 0x44 |
| 24 | Frost | 4 | 6 area | 0x87 |
| 25 | Heal | 4 | 4 heal | 4 |
| 26 | Poison Weapon | 4 | 3 protection | 0xB |
| 27 | Remove Trap | 4 | 7 target | 0xA |
| 28 | Flameproof | 4 | 3 protection | 0x46 |
| 29 | Thick Skin | 4 | 2 armour | 0x43 |
| 30 | Study Monster | 4 | 7 target | 7 |
| 31 | Missile Protection | 4 | 3 protection | 5 |
| 32 | Levitate | 5 | 1 motion | 3 |
| 33 | Fireball | 5 | 5 missile | 3 |
| 34 | Name Enchantment | 5 | 7 target | 0xB |
| 35 | Open | 5 | 7 target | 0xC |
| 36 | Smite Undead | 5 | 7 target | 2 |
| 37 | Rune of Stasis | 5 | 8 create | 3 |
| 38 | Mending | 5 | 7 target | 9 |
| 39 | Telekinesis | 5 | 11 extra | 8 |
| 40 | Charm | 6 | 7 target | 3 |
| 41 | Sheet Lightning | 6 | 6 area | 0x42 |
| 42 | Daylight | 6 | 0 light | 0x86 |
| 43 | Gate Travel | 6 | 7 target | 0xF |
| 44 | Greater Heal | 6 | 4 heal | 0xF |
| 45 | Invisibility | 6 | 3 protection | 0x44 |
| 46 | Map Area | 6 | 13 special | 7 |
| 47 | Paralyze | 6 | 7 target | 5 |
| 48 | Mass Confusion | 7 | 6 area | 0x83 |
| 49 | Enchantment | 7 | 7 target | 0xE |
| 50 | Reveal | 7 | 6 area | 0x81 |
| 51 | Shockwave | 7 | 6 area | 0x86 |
| 52 | Summon Demon | 7 | 8 create | 5 |
| 53 | Tremor | 7 | 11 extra | 9 |
| 54 | Portal | 7 | 11 extra | 1 |
| 55 | Magic Satellite | 7 | 8 create | 6 |
| 56 | Flame Wind | 8 | 6 area | 0x84 |
| 57 | Fly | 8 | 1 motion | 5 |
| 58 | Freeze Time | 8 | 11 extra | 0xB |
| 59 | Iron Flesh | 8 | 2 armour | 0x45 |
| 60 | Restoration | 8 | 11 extra | 2 |
| 61 | Roaming Sight | 8 | 11 extra | 7 |
| 62 | Smite Foe | 8 | 7 target | 6 |
| 63 | Armageddon | 8 | 11 extra | 0xC |
| 64 | Mass Paralyze | | 6 area | 5 |
| 65 | Acid | | 5 missile | 4 |
| 66 | Local Teleport | | 11 extra | 0xD |
| 67 | Mana Boost | | 10 mana | 3 |
| 68 | Restore Mana | | 10 mana | 9 |

**UW2, Iron Flesh and the djinn.** Casting Iron Flesh while the djinn quest stands at stage 4 (X clock 3) glazes the baked mud on the player and moves the quest to stage 5 ("The baked mud hardens into a clear glaze."). Casting Levitate or Fly gives a little upward bounce.

Source: `spells[]` ([SPELLS.C:1033](../../uw2/src/combat/SPELLS.C#L1033)), `do_spell` ([SPELLS.C:83](../../uw2/src/combat/SPELLS.C#L83)).

## Open questions

- Two rows of the list do not match their names. Mass Paralyze (64) has class 6 minor 5, which is the Repel Undead handler with critters as the target mode. Local Teleport (66) has class 11 minor 0xD, which is Dispel Hunger. Either the name order is off there or these object spells do what their class says. [FINDINGS.md](../../uw2/docs/FINDINGS.md) lists the first.
- `cast` reads a byte that is a spell index only below 0x40, so how objects cast entries 64 to 68 was not traced.
- Each spell row has three low bits in its class byte (e.g., 0x29 for Magic Arrow) whose use was not found.
- Race 0x17 as the liche is inferred from the owner race strings.
