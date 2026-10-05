# Where the rules of UW1 and UW2 differ

Ultima Underworld I and II run on the same engine, a year apart, and most of their code is the same. This page lists the places where the two games play by different rules, routine by routine, so that a reimplementation such as [UnderworldGodot](https://github.com/hankmorgan/UnderworldGodot) knows where it needs a separate branch for each game.

Every statement about what a game does is read from the matched sources of the two decompilations, which compile to the same bytes as the shipped `UW.EXE` and `UW2.EXE`. Links to UW1's sources go to [UW1Decomp](https://github.com/abedegno/UW1Decomp) on GitHub, and links to UW2's sources are to this repository. Where a constant's meaning in the game is not known, the entry says so. Statements about the effect in play are inferences from the code, and none was checked in a running game.

Each entry also says what UnderworldGodot does today, at commit [`d7025471`](https://github.com/hankmorgan/UnderworldGodot/tree/d7025471a1362860afd9f68555e137a9ff4f131a). The verdict is one of these:

- **branches** means it has a separate path for each game and the paths agree with the sources;
- **one rule for both** means it applies one game's rule to both games;
- **differs** means it has a branch, but the branch does not match the source;
- **missing** means it has no code for the rule (found by searching, so a rule written under another name could have been missed);
- **not checked** means the entry was not compared.

UnderworldGodot was read, not run, and none of its gaps below was confirmed in play.

## Contents

- [How the comparison was made](#how-the-comparison-was-made)
- [Combat](#combat)
- [Magic](#magic)
- [Skills, experience and the player record](#skills-experience-and-the-player-record)
- [AI and critters](#ai-and-critters)
- [Items and objects](#items-and-objects)
- [Traps and triggers](#traps-and-triggers)
- [Movement and physics](#movement-and-physics)
- [Conversations](#conversations)
- [Light, exploration and the automap](#light-exploration-and-the-automap)
- [Systems in one game only](#systems-in-one-game-only)
- [UnderworldGodot summary](#underworldgodot-summary)

## How the comparison was made

The two games' functions were paired first. UW1Decomp's `map/kin.tsv` pairs each UW1 procedure with its closest UW2 procedure, and each repository's `map/crosswalk.tsv` gives the matched function at each address. A UW1 function was paired with the UW2 function of the same name when both games have one (1,346 pairs), and otherwise through `kin.tsv` (346 pairs). The 28 functions of UW2's `src/game/SKILLS.C` are missing from UW2's `crosswalk.tsv`, so they were added from `targets/ovr154.tsv`.

Each pair was then compared as machine code, with the instruction normaliser of Exhume's `tools/kin.py`. That normaliser hides the addresses of globals, calls and branches, so "identical" means the same instructions with the same constants. A pair that matches only once immediates and memory displacements are also hidden is counted as "constants only". Those pairs differ in numbers alone, e.g., structure offsets, string numbers or screen positions, but a changed number can still be a rule (e.g., `player_eat` divides by 8 in UW1 and by 6 in UW2), so they were read too. Every "different" and "constants only" pair in the subsystems below was read as a C diff of the two matched bodies.

| Subsystem | identical | constants only | different | UW1 only | UW2 only |
| --- | ---: | ---: | ---: | ---: | ---: |
| 3d | 96 | 54 | 51 | 89 | 47 |
| combat | 16 | 7 | 54 | 0 | 22 |
| conv | 55 | 26 | 54 | 2 | 25 |
| critter | 20 | 12 | 49 | 3 | 19 |
| event | 9 | 5 | 19 | 5 | 111 |
| game | 30 | 20 | 53 | 7 | 13 |
| gfx | 72 | 110 | 123 | 46 | 105 |
| inv | 22 | 21 | 21 | 0 | 0 |
| lib | 29 | 17 | 1 | 166 | 2 |
| map | 2 | 1 | 11 | 2 | 2 |
| motion | 22 | 9 | 37 | 2 | 9 |
| obj | 34 | 15 | 63 | 7 | 15 |
| sound | 75 | 3 | 35 | 6 | 23 |
| sys | 87 | 26 | 26 | 6 | 35 |
| ui | 77 | 31 | 91 | 21 | 54 |
| other | 0 | 1 | 0 | 2 | 0 |
| **all** | **646** | **358** | **688** | **364** | **482** |

The counts cover the 2,056 UW1 and 2,063 UW2 functions that the crosswalks place in a located segment. Read them with three cautions:

- "different" overstates the rule changes. Many of those pairs differ only because UW2 was compiled from slightly edited C, e.g., a `char` that became `unsigned char`, a removed debug print, or a sequence of animation frames whose length is now looked up. Fewer than a third of the "different" pairs in the rule subsystems change a rule.
- 166 of the UW1-only functions are Borland's C library (UW1's seg005), which has no source line in either crosswalk, so they could not be paired. They are the same library.
- A UW1 function that pairs with nothing is not always a UW1-only rule. Some UW1 work moved to a differently shaped UW2 routine.

## Combat

### Melee attack and weapon enchantments

The player's to-hit score and the damage of an enchanted weapon are worked out differently.

- **UW1.** The attack score is half the Attack skill plus the weapon skill ([UW1 src/combat/COMBAT.C:581](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/COMBAT.C#L581)). A weapon enchantment of class 12 adds `(effect & 7) + 1` to damage when bit 8 is set, and to the attack score otherwise ([UW1 src/combat/COMBAT.C:594](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/COMBAT.C#L594)).
- **UW2.** The attack score also adds `Valor`, the bonus of the Valor spell, 10 plus Casting / 5 ([UW2 src/game/PLAYDATA.C:222](../src/game/PLAYDATA.C#L222)), ([UW2 src/combat/COMBAT.C:628](../src/combat/COMBAT.C#L628)). An effect below 4 adds `2 * effect + 1` damage, an effect from 4 to 7 adds `2 * effect - 7` to the attack score, and an effect of 8 or more makes a special weapon, `specweap = effect - 7`, with its own damage and attack bonuses from `spec_damage` and `spec_skill` ([UW2 src/combat/COMBAT.C:641](../src/combat/COMBAT.C#L641)). A special weapon's extra effect fires after a hit, on a critical hit or always for kind 6 ([UW2 src/combat/COMBAT.C:790](../src/combat/COMBAT.C#L790)). The kinds are 1 health drain (`get_hp_back`), 2 Smite Undead, 3 a fireball, 4 Paralyze, and 5 and 6 opening a door.
- **UnderworldGodot.** Branches, `src/interaction/combat/combat.cs:138-181`.

### Poisoned weapon (UW2 only)

UW2's Poison Weapon spell sets `PoisonWeap` ([UW2 src/game/PLAYDATA.C:225](../src/game/PLAYDATA.C#L225)). While it lasts, a hit with a sharp weapon on a creature that bleeds adds `damage * (Casting + 30) / 40` to the damage, so it multiplies the damage by 1.75 to 2.5 ([UW2 src/combat/COMBAT.C:325](../src/combat/COMBAT.C#L325)).

- **UnderworldGodot.** Differs. It adds `(Casting + 30) / 40`, 0 to 1 point, without multiplying by the damage ([`src/interaction/combat/combat.cs:422`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/interaction/combat/combat.cs#L422)).

### A critter's poisonous blow

- **UW1.** A poisonous hit on the player poisons him unless he resists poison (`check_res`) ([UW1 src/combat/COMBAT.C:808](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/COMBAT.C#L808)).
- **UW2.** It first needs `rand() % (poison + 6)` to beat twice the player's armour at the hit location ([UW2 src/combat/COMBAT.C:911](../src/combat/COMBAT.C#L911)).
- **UnderworldGodot.** One rule for both, and it differs from UW2 too. It applies the armour roll in both games, and compares with the armour itself, not twice it ([`src/interaction/combat/combat.cs:224`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/interaction/combat/combat.cs#L224)).

### Resistances

`check_res` gives the damage an object takes after its resistances.

- **UW1.** Magic damage is resisted by chance and any other resisted type cancels the damage ([UW1 src/combat/DAMAGE.C:166](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/DAMAGE.C#L166)).
- **UW2.** The same, and fire damage on a creature that resists cold, or cold on one that resists fire but not cold, is doubled up to 255 ([UW2 src/combat/DAMAGE.C:225](../src/combat/DAMAGE.C#L225)).
- **UnderworldGodot.** Branches, `src/interaction/damage.cs:487`. Both games' test vectors (`vectors/check_res.csv`) cover the rule.

### Damaging objects and doors

- **UW1.** An object with a door direction can't be damaged ([UW1 src/combat/DAMAGE.C:213](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/DAMAGE.C#L213)). A destroyed object becomes one of two debris items at random ([UW1 src/combat/DAMAGE.C:140](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/DAMAGE.C#L140)).
- **UW2.** Doors can be damaged ([UW2 src/combat/DAMAGE.C:265](../src/combat/DAMAGE.C#L265)), and damaging an owned object angers its owners (`player_did_bad`) ([UW2 src/combat/DAMAGE.C:299](../src/combat/DAMAGE.C#L299)). A broken weapon becomes the broken item for its skill, and a dagger a broken dagger ([UW2 src/combat/DAMAGE.C:117](../src/combat/DAMAGE.C#L117)). Other debris comes from `debris_type` ([UW2 src/combat/DAMAGE.C:174](../src/combat/DAMAGE.C#L174)), which UW2's `vectors/debris_type.csv` covers.
- **UnderworldGodot.** Not checked.

### Sounds of combat

UW2 records the kind of weapon the player swings (`player_weapon`, 0 fist, 1 a weapon that is not sharp, 2 a sharp one, 3 a missile weapon) and picks the hit, miss and wall sounds from it. UW1 plays fixed effects. `FINDINGS.md` in UW1Decomp describes UW1's `do_miss`, whose test of the attacker is wrong. UnderworldGodot branches (issue #108 in its tracker).

## Magic

### Casting from runes

`player_cast` checks a rune spell before it is cast. The rules differ in almost every step.

| Step | UW1 | UW2 |
| --- | --- | --- |
| Circle | spell index / 6 + 1 ([UW1 src/combat/RUNES.C:242](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/RUNES.C#L242)) | spell index / 8 + 1, and 1 above circle 8 ([UW2 src/combat/RUNES.C:227](../src/combat/RUNES.C#L227)) |
| Place | any level but the Ethereal Void, see below | in Britannia (levels 1 to 8) a spell above circle 3 fails, string 0xE4 "Casting was not successful" ([UW2 src/combat/RUNES.C:232](../src/combat/RUNES.C#L232)) |
| Experience level | (level + 1) / 2 at least the circle ([UW1 src/combat/RUNES.C:245](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/RUNES.C#L245)) | the same |
| Mana | 3 * circle | the same |
| Skill roll | `skill_check(Casting + 5, 2 * circle)` ([UW1 src/combat/RUNES.C:249](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/RUNES.C#L249)) | `skill_check(Casting, 3 * circle)` ([UW2 src/combat/RUNES.C:238](../src/combat/RUNES.C#L238)) |
| Delay to the next cast | `(2 * circle - level) * 4 + 0x40` ([UW1 src/combat/RUNES.C:257](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/RUNES.C#L257)) | `... + 0x80` ([UW2 src/combat/RUNES.C:246](../src/combat/RUNES.C#L246)) |
| Mana paid later | missile spells | missile spells and spells aimed at one target ([UW2 src/combat/RUNES.C:249](../src/combat/RUNES.C#L249)) |
| Paralysed player | can cast | can't cast ([UW2 src/combat/RUNES.C:174](../src/combat/RUNES.C#L174)) |
| Too soon | a sound | a sound and string 0xB ([UW2 src/combat/RUNES.C:183](../src/combat/RUNES.C#L183)) |

- **UnderworldGodot.** The circle, the delay and the message branch (`src/magic/runicmagic.cs:55`, `:302`, `:320`). The skill roll is UW2's in both games ([`src/magic/runicmagic.cs:339`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/magic/runicmagic.cs#L339)), so UW1 casting fails much more often than in DOS, e.g., Casting 10 against a circle 4 spell rolls against 12 instead of 8 with a bonus of 5. The Britannia rule refuses circle 3 as well, with `SpellLevel >= 3` where DOS has `level > 3`, and prints string 0xE3 where DOS prints 0xE4 ([`src/magic/runicmagic.cs:476`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/magic/runicmagic.cs#L476)).

### Where magic works

- **UW1.** On level 9, the Ethereal Void, nothing that a critter or the player casts works ([UW1 src/combat/SPELLS.C:101](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/SPELLS.C#L101)).
- **UW2.** Only anti-magic squares stop spells ([UW2 src/combat/SPELLS.C:89](../src/combat/SPELLS.C#L89)). In world 5, the Scintillus Academy (levels 41 to 48), `restore_mana` gives no mana, except on its first level, and on its eighth level at x 25 or more ([UW2 src/combat/SPELLS.C:160](../src/combat/SPELLS.C#L160)). Regeneration and sleep both give mana through `restore_mana`.
- **UnderworldGodot.** The level 9 rule branches (`src/magic/spellcasting.cs:28`). The Academy rule differs. It computes the Academy level as `1 + dungeon_level % 8` ([`src/player/playerdatloop.cs:291`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdatloop.cs#L291)) where DOS has `(PlayerLevel - 1) % 8 + 1`, so it is one level out. It blocks mana on the Academy's levels 1 to 6 and on level 7 west of x 25, and allows it on level 8, where DOS blocks levels 2 to 7 and level 8 west of x 25. Its check is in `ManaRegenChange`, so other ways of gaining mana were not compared.

### Area spells (class 6)

- **UW1.** Every area spell acts 3d4 times on squares within 2 of the square 4 ahead of the caster ([UW1 src/combat/SPELLS.C:549](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/SPELLS.C#L549)).
- **UW2.** Each spell has its own count, distance and radius in the table `area_spells` (`src/combat/SPELLS.C:729`) ([UW2 src/combat/SPELLS.C:752](../src/combat/SPELLS.C#L752)), e.g., Sheet Lightning 6 times at distance 4 and radius 2, Frost 5 times at distance 3 and radius 1. Meteor (the flame wind) no longer hits the four squares beside each target ([UW1 src/combat/SPELLS.C:329](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/SPELLS.C#L329)). It hits up to 5 critters, never the same square twice in a row, and an empty square only one time in three ([UW2 src/combat/SPELLS.C:309](../src/combat/SPELLS.C#L309)).
- **UnderworldGodot.** UW2 branches. UW1 differs. It acts 2 times with a radius of 0 ([`src/magic/spellcasting_class_6.cs:66`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/magic/spellcasting_class_6.cs#L66)), where DOS rolls 3d4 within radius 2.

### Spells on one critter (class 7)

- **UW1.** The spell acts at once on the square 4 ahead of the player ([UW1 src/combat/SPELLS.C:558](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/SPELLS.C#L558)). The handlers are, in order, Fear, Smite Undead, Ally, Poison and Paralyze. Smite Undead does 255 magic damage to a target that has the undead resistance bit ([UW1 src/combat/SPELLS.C:347](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/SPELLS.C#L347)). Fear, Ally and Paralyze need the target to fail a magic resistance check ([UW1 src/combat/SPELLS.C:368](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/SPELLS.C#L368)). Paralyze puts the critter into goal 7 (stand) ([UW1 src/combat/SPELLS.C:409](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/SPELLS.C#L409)). Ally makes any critter that fails its own magic resistance check an ally ([UW1 src/combat/SPELLS.C:385](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/SPELLS.C#L385)).
- **UW2.** The player aims the spell with the cursor ([UW2 src/combat/SPELLS.C:793](../src/combat/SPELLS.C#L793)). Smite Undead does half the target's hit points to race 0x17 and destroys a floating skull ([UW2 src/combat/SPELLS.C:336](../src/combat/SPELLS.C#L336)). Paralyze lasts `(rand() & 0xF) * power + 0x10`, where power is Casting / 3 for the player and 8 for others, in a new goal 15 ([UW2 src/combat/SPELLS.C:591](../src/combat/SPELLS.C#L591)). Charm works only on critters marked `+` in string block 0x15E or 0x15F by their conversation slot ([UW2 src/combat/SPELLS.C:396](../src/combat/SPELLS.C#L396)). Poison, Fear, Charm and Confusion affect creatures only ([UW2 src/combat/SPELLS.C:354](../src/combat/SPELLS.C#L354)).
- **UnderworldGodot.** UW2 branches. In UW1, Smite Undead does nothing. Its UW1 path calls `SmiteUndeadObject`, which only prints "unimplemented" ([`src/magic/spellcasting_class_7.cs:442`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/magic/spellcasting_class_7.cs#L442)). UW1 Paralyze skips the magic resistance check ([`src/magic/spellcasting_class_7.cs:496`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/magic/spellcasting_class_7.cs#L496)).

### Other spell changes

- **Healing.** UW1 heals the spell's target ([UW1 src/combat/SPELLS.C:115](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/SPELLS.C#L115)), and UW2 heals the caster ([UW2 src/combat/SPELLS.C:107](../src/combat/SPELLS.C#L107)).
- **Tremor.** UW1 drops boulders ([UW1 src/combat/SPELLS.C:653](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/SPELLS.C#L653)). In world 8, UW2 drops a skull, mushroom, fish, sphere or coin by floor texture ([UW2 src/combat/SPELLS2.C:668](../src/combat/SPELLS2.C#L668)).
- **Spells of class 11.** UW1's minor 1 detects monsters ([UW1 src/combat/SPELLS.C:773](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/SPELLS.C#L773)), where UW2's minor 1 moves the player 2 squares ahead ([UW2 src/combat/SPELLS2.C:711](../src/combat/SPELLS2.C#L711)). UW2's minor 2 cures poison, drugs, drunkenness, paralysis, hunger and fatigue ([UW2 src/combat/SPELLS2.C:729](../src/combat/SPELLS2.C#L729)).
- **Detect Monster.** UW2 also names a creature seen on a great success ([UW2 src/combat/SPELLS2.C:616](../src/combat/SPELLS2.C#L616)).
- **Object spells.** UW1 `cast` looks every spell number up in the spell table ([UW1 src/combat/SPELLS.C:85](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/SPELLS.C#L85)). UW2 reads spells 0x40 and up as class 12 to 15 directly ([UW2 src/combat/SPELLS.C:67](../src/combat/SPELLS.C#L67)).
- **Summoning.** UW2's `creat_spell` adds the rune traps, the demon (chosen by Casting) and the satellite ([UW2 src/combat/SPELLS2.C:509](../src/combat/SPELLS2.C#L509)). In UW1, `creat_spell` with 3 lays a trap with `cast_trap_spell` ([UW1 src/combat/SPELLS.C:593](https://github.com/abedegno/UW1Decomp/blob/main/src/combat/SPELLS.C#L593)).

UnderworldGodot was not checked entry by entry for these.

## Skills, experience and the player record

### Experience

`player_get_exp` adds experience and awards skill points.

| Rule | UW1 | UW2 |
| --- | --- | --- |
| Every gain | as given | halved, rounding up or down at random ([UW2 src/game/SKILLCHK.C:70](../src/game/SKILLCHK.C#L70)) |
| Halved again (plus one) when the character level is above | twice the dungeon level plus 2 ([UW1 src/game/SKILLCHK.C:88](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLCHK.C#L88)) | twice the world number plus 2 ([UW2 src/game/SKILLCHK.C:79](../src/game/SKILLCHK.C#L79)) |
| Skill point | every 3,000 ([UW1 src/game/SKILLCHK.C:91](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLCHK.C#L91)) | every 1,500 ([UW2 src/game/SKILLCHK.C:81](../src/game/SKILLCHK.C#L81)) |
| Above 0x17700 (96,000) | gains are ignored entirely, no skill points either ([UW1 src/game/SKILLCHK.C:85](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLCHK.C#L85)) | gains still count and give skill points, but no more levels ([UW2 src/game/SKILLCHK.C:91](../src/game/SKILLCHK.C#L91)) |
| Ceiling | none | a gain that would pass 0x7FFF0 is refused ([UW2 src/game/SKILLCHK.C:88](../src/game/SKILLCHK.C#L88)) |

- **UnderworldGodot.** One rule for both. It halves every gain in both games ([`src/player/playerdat.cs:657`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdat.cs#L657)), so UW1 kills, traps and exploration give half the DOS experience. It also lacks UW1's stop above 0x17700, and it clamps at 0x7FFF0 where UW2 refuses the gain ([`src/player/playerdat.cs:687`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdat.cs#L687)). The skill point step and the halving test branch.

### Derived values

- **Carrying capacity.** UW1 is strength * 20 tenths of a stone ([UW1 src/game/SKILLS.C:112](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLS.C#L112)), and UW2 is strength * 13 + 300 ([UW2 src/game/SKILLS.C:81](../src/game/SKILLS.C#L81)). UnderworldGodot uses UW2's for both ([`src/player/playerdat.cs:794`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdat.cs#L794)), so a UW1 character of strength 20 can carry 56 stones instead of 40.
- **Maximum mana on Tybal's level (UW1).** While the orb stands, a level gained there sets the kept-aside mana, not the maximum ([UW1 src/game/SKILLS.C:109](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLS.C#L109)). UnderworldGodot leaves a TODO for this in its level-up code ([`src/player/playerdat.cs:787`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdat.cs#L787)).
- **Mana after a level change (UW2).** `player_compute` and `ChangeLevel` call `restore_mana(ThePlayer, 0)` ([UW2 src/game/SKILLS.C:84](../src/game/SKILLS.C#L84)), ([UW2 src/game/GAMEWRAP.C:401](../src/game/GAMEWRAP.C#L401)), which adds nothing: an amount of 0 takes the branch that subtracts it ([UW2 src/combat/SPELLS.C:166](../src/combat/SPELLS.C#L166)).
- **Hit points and mana.** The formulas are the same.

### Time, hunger, fatigue and healing

`duration_check` runs the player's slow clock. Every third step it applies poison and regenerates mana in both games. The longer cycle differs.

- **UW1.** Every 24 steps ([UW1 src/game/PLAYTIME.C:128](https://github.com/abedegno/UW1Decomp/blob/main/src/game/PLAYTIME.C#L128)), the player gets 3 to 6 points hungrier ([UW1 src/game/PLAYTIME.C:130](https://github.com/abedegno/UW1Decomp/blob/main/src/game/PLAYTIME.C#L130)), less drunk, more tired, wandering monsters may appear, and if `skill_check(strength, 15)` succeeds the player gains exactly 1 hit point ([UW1 src/game/PLAYTIME.C:139](https://github.com/abedegno/UW1Decomp/blob/main/src/game/PLAYTIME.C#L139)) (`restore_hp` with -1 adds 1).
- **UW2.** The same every 30 steps ([UW2 src/game/PLAYTIME.C:137](../src/game/PLAYTIME.C#L137)), with the same hit point rule ([UW2 src/game/PLAYTIME.C:148](../src/game/PLAYTIME.C#L148)), and every 60 steps the X clock of the time of day advances and the schedules (SCD) wrap ([UW2 src/game/PLAYTIME.C:151](../src/game/PLAYTIME.C#L151)).
- **UnderworldGodot.** One rule for both, and it differs from both. It uses 30 for both games ([`src/player/playerdatloop.cs:218`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdatloop.cs#L218)). Hunger falls by 3 to 5, not 3 to 6 ([`src/player/playerdatloop.cs:220`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdatloop.cs#L220)). The hit point roll is `SkillCheck(STR, 10)` ([`src/player/playerdatloop.cs:240`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdatloop.cs#L240)), and on success it calls `HPRegenerationChange` with the positive result ([`src/player/playerdatloop.cs:243`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdatloop.cs#L243)), which heals `1 + ((0..3 + result) * max_hp >> 4)`, about 4 to 19 points at 60 maximum hit points, where DOS heals 1.

### Eating, sleeping and dreams

- **Food.** Eating heals `food_heal / 8` in UW1 ([UW1 src/game/SKILLS.C:654](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLS.C#L654)) and `food_heal / 6` in UW2 ([UW2 src/game/SKILLS.C:592](../src/game/SKILLS.C#L592)), up to 8. UnderworldGodot divides by 6 in both ([`src/player/playerdatstatus.cs:676`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdatstatus.cs#L676)).
- **Interrupted sleep.** It takes 0x20 off fatigue in UW1 ([UW1 src/game/SKILLS.C:566](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLS.C#L566)) and 0x18 in UW2 ([UW2 src/game/SKILLS.C:493](../src/game/SKILLS.C#L493)). UnderworldGodot uses 0x18 for both ([`src/World/sleep.cs:238`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/World/sleep.cs#L238)).
- **Where you can sleep.** UW1 refuses on level 9 ([UW1 src/game/SKILLS.C:525](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLS.C#L525)). UW2 refuses in the pits ([UW2 src/game/SKILLS.C:441](../src/game/SKILLS.C#L441)), and sleeping while in the Void wakes the player from it.
- **Dreams.** UW1 shows Garamon's dreams in order and then at random, until he is buried ([UW1 src/game/SKILLS.C:476](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLS.C#L476)). UW2 shows the dreams whose castle X clock threshold (4, 6, 10, 14) has passed ([UW2 src/game/SKILLS.C:338](../src/game/SKILLS.C#L338)), and sends a player who ate the dream plant to the Ethereal Void. UnderworldGodot branches (`src/World/sleep.cs`).
- **New character.** Fatigue and `food_heal` start at 0x40 in UW1 ([UW1 src/game/CHARGEN.C:120](https://github.com/abedegno/UW1Decomp/blob/main/src/game/CHARGEN.C#L120)) and 0x30 in UW2 ([UW2 src/game/CHARGEN.C:105](../src/game/CHARGEN.C#L105)). The clock starts at 0x10B3000 ([UW1 src/game/CHARGEN.C:96](https://github.com/abedegno/UW1Decomp/blob/main/src/game/CHARGEN.C#L96)) and 0x465000 ([UW2 src/game/CHARGEN.C:84](../src/game/CHARGEN.C#L84)). UnderworldGodot starts fatigue at 0x30 for both ([`src/player/playerdatainit.cs:152`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdatainit.cs#L152)).

### Death

- **UW1.** Death costs an eighth of the experience ([UW1 src/game/SKILLS.C:832](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLS.C#L832)) and leaves bones where the player fell. With a silver tree planted (and not on level 9) the player comes back at the tree, and otherwise the game ends ([UW1 src/game/SKILLS.C:861](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLS.C#L861)). Once the last talisman is destroyed the player can't die and keeps 4 hit points ([UW1 src/game/SKILLS.C:825](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLS.C#L825)).
- **UW2.** On level 1, a player killed by a castle guard who is not a loner goes to jail ([UW2 src/game/SKILLS.C:814](../src/game/SKILLS.C#L814)). In the Void the player wakes, and in the pits the fight is lost. Otherwise death costs a ninth of the experience ([UW2 src/game/SKILLS.C:844](../src/game/SKILLS.C#L844)), and outside Britannia the player wakes beside the blackrock gem on level 5 ([UW2 src/game/SKILLS.C:854](../src/game/SKILLS.C#L854)). In Britannia the game ends.
- **UnderworldGodot.** Branches (`src/player/playerdatdeath.cs`).

### Finding and disarming traps on objects

- **UW1.** `DetectedTrap` and `RemoveTrap` roll against 8 ([UW1 src/game/SKILLS.C:898](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLS.C#L898)), ([UW1 src/game/SKILLS.C:928](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLS.C#L928)).
- **UW2.** Detecting rolls against 10 plus twice the world number ([UW2 src/game/SKILLS.C:885](../src/game/SKILLS.C#L885)), and disarming against 8 plus the world number ([UW2 src/game/SKILLS.C:923](../src/game/SKILLS.C#L923)).
- **UnderworldGodot.** Branches (`src/interaction/trapdisarming.cs:65-75`).

### Other player rules

- **Lore.** UW1 remembers the Lore skill per level only for levels 1 to 8 ([UW1 src/game/SKILLS.C:215](https://github.com/abedegno/UW1Decomp/blob/main/src/game/SKILLS.C#L215)).
- **Lights.** A burnt-out light goes out at quality 0 in UW1 ([UW1 src/game/PLAYTIME.C:179](https://github.com/abedegno/UW1Decomp/blob/main/src/game/PLAYTIME.C#L179)) and at quality 1 in UW2 ([UW2 src/game/PLAYTIME.C:205](../src/game/PLAYTIME.C#L205)). In UW2, lights don't burn while time is stopped ([UW2 src/game/PLAYTIME.C:183](../src/game/PLAYTIME.C#L183)).
- **Spells of protection.** UW2 adds Valor and Poison Weapon as minors 10 and 11 of the protection class ([UW2 src/game/PLAYDATA.C:222](../src/game/PLAYDATA.C#L222)). UW1 has a maze spell, minor 4 of its special class ([UW1 src/game/PLAYDATA.C:268](https://github.com/abedegno/UW1Decomp/blob/main/src/game/PLAYDATA.C#L268)).
- **Dragon skin boots (UW1).** Wearing them sets a flag that stops lava damage ([UW1 src/game/PLAYDATA.C:452](https://github.com/abedegno/UW1Decomp/blob/main/src/game/PLAYDATA.C#L452)). UW2 has no such item rule. UnderworldGodot branches.

### PLAYER.DAT

The two records are different files, and nothing in one can be read with the other's layout.

- **UW1.** The record is 0xD2 bytes (`src/include/player.h` in UW1Decomp), written with `sizeof(struct Player)` ([UW1 src/game/PLAYDATA.C:122](https://github.com/abedegno/UW1Decomp/blob/main/src/game/PLAYDATA.C#L122)). It holds 32 quest bits and 4 quest bytes, the talisman count, and 64 game variables of 6 bits each (`player->game_vars`).
- **UW2.** The record is 0x37D bytes ([UW2 src/game/PLAYDATA.C:101](../src/game/PLAYDATA.C#L101)). It holds 128 quest flags packed four to a long and 16 quest bytes (`quests[32]` and `quest_bytes[16]` in [player.h](../src/include/player.h)), 256 numbered variables of 16 bits, and 16 X clocks.
- **The cipher.** In UW1 each byte is XORed with a key that starts at the file's seed plus 3, grows by 3 a byte and restarts every 0x50 bytes ([UW1 src/sys/MISCUTIL.C:268](https://github.com/abedegno/UW1Decomp/blob/main/src/sys/MISCUTIL.C#L268)). UW2 chains each byte to the one before ([UW2 src/sys/MISCUTIL.C:304](../src/sys/MISCUTIL.C#L304)).

## AI and critters

- **Tybal's guards (UW1).** On Tybal's level, while his orb is whole, critters of race 0x13 close in and never cast ([UW1 src/critter/AI.C:353](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L353)), ([UW1 src/critter/AI.C:510](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L510)), ([UW1 src/critter/AI.C:531](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L531)). UnderworldGodot has UW1-only code for race 0x13 (`src/npc/npcai.cs:3035-3076`), not compared in detail.
- **Doors.** In UW1 a critter that is not hostile never opens a door it bumps into ([UW1 src/critter/PATHFIND.C:1154](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/PATHFIND.C#L1154)). In UW2 any critter tries three times in four ([UW2 src/critter/PATHFIND.C:1282](../src/critter/PATHFIND.C#L1282)), except on level 10 ([UW2 src/critter/PATHFIND.C:1394](../src/critter/PATHFIND.C#L1394)). UnderworldGodot uses UW2's rule for both ([`src/npc/npcai.cs:1620`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/npc/npcai.cs#L1620)) (it has the level 10 rule).
- **Paths.** UW2 floods up to 40 steps for a path where UW1 floods 32 ([UW1 src/critter/PATHFIND.C:664](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/PATHFIND.C#L664)), ([UW2 src/critter/PATHFIND.C:808](../src/critter/PATHFIND.C#L808)). UW2 also has a reach of 10 squares on a level whose number makes `(PlayerLevel - 1) % 8 + 1` zero ([UW2 src/critter/PATHFIND.C:770](../src/critter/PATHFIND.C#L770)). That can only be level 0, so the reach is always 5 in practice. What the line was meant to test is not known.
- **Held critters (UW2).** UW2's Paralyze uses goal 15, which counts down and then drops the goal ([UW2 src/critter/AI.C:1304](../src/critter/AI.C#L1304)). UW1 has no goal 15.
- **Goal 3.** UW1's goal 3 (follow) chases a target ([UW1 src/critter/AI.C:1277](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L1277)). In UW2 a critter with goal 3 does nothing in the goal switch.
- **Talking critters (UW2).** A hostile critter whose goal is talk changes to guard ([UW2 src/critter/AI.C:704](../src/critter/AI.C#L704)).
- **Corpses.** In world 7 UW2 always leaves remains ([UW2 src/critter/PATHFIND.C:161](../src/critter/PATHFIND.C#L161)), where UW1 leaves them 7 times in 16 ([UW1 src/critter/PATHFIND.C:161](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/PATHFIND.C#L161)).
- **Taking owned objects.** Owners up to race 0x1B care in UW1 ([UW1 src/critter/CRITTIME.C:480](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/CRITTIME.C#L480)) and up to 0x1D in UW2 ([UW2 src/critter/CRITTIME.C:545](../src/critter/CRITTIME.C#L545)). UW2 also clears the owner of everything inside a taken container ([UW2 src/critter/CRITTIME.C:548](../src/critter/CRITTIME.C#L548)). In UW1, race 0x0D stops caring once the Knight of the Crux quest byte reaches 3 ([UW1 src/critter/CRITTIME.C:442](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/CRITTIME.C#L442)).
- **One NPC (UW1).** On level 6, the critter with whoami 0x16 accepts no danger on its paths, as a peaceful critter does, even when hostile ([UW1 src/critter/AI.C:1389](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L1389)).
- **Death cries.** UW1 decides the cry from the critter the AI processed last (a bug, see UW1Decomp's `FINDINGS.md`) ([UW1 src/critter/AI.C:1441](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L1441)). UW2 picks one of four cries by the dying critter's death kind ([UW2 src/critter/AI.C:1437](../src/critter/AI.C#L1437)).
- **Who angers whom (UW2).** Damage from a static object counts as from nobody ([UW2 src/critter/AI.C:1484](../src/critter/AI.C#L1484)).
- **Animation.** UW1 critters have 4 frames to every sequence and strike on frame 4 ([UW1 src/critter/AI.C:1154](https://github.com/abedegno/UW1Decomp/blob/main/src/critter/AI.C#L1154)). UW2 looks up each sequence's length and strikes on frame 3 ([UW2 src/critter/AI.C:1127](../src/critter/AI.C#L1127)). That is engine work, but it changes the time between a swing and the hit.

UnderworldGodot was not checked for the entries without a verdict.

## Items and objects

- **Treasure.** The kind of treasure a critter carries is rolled as `rand() % (40 - 3 * lvl) - (33 - 3 * lvl)`, where lvl is the dungeon level in UW1 ([UW1 src/obj/TREASURE.C:107](https://github.com/abedegno/UW1Decomp/blob/main/src/obj/TREASURE.C#L107)), and as `rand() % (37 - 3 * world) - (30 - 3 * world)` in UW2 ([UW2 src/obj/TREASURE.C:103](../src/obj/TREASURE.C#L103)), which also never gives type 1 ([UW2 src/obj/TREASURE.C:106](../src/obj/TREASURE.C#L106)). UnderworldGodot branches (`src/npc/npcloot.cs:48-51`).
- **Equipment.** UW2 gives an item whose quality type is 0xF a quality of 40 ([UW2 src/obj/TREASURE.C:190](../src/obj/TREASURE.C#L190)).
- **A dead critter's belongings.** UW1 writes the owner onto the container, not the items ([UW1 src/obj/TREASURE.C:73](https://github.com/abedegno/UW1Decomp/blob/main/src/obj/TREASURE.C#L73)). UW2 marks each item with the owner ([UW2 src/obj/TREASURE.C:61](../src/obj/TREASURE.C#L61)), puts out lit lights ([UW2 src/obj/TREASURE.C:62](../src/obj/TREASURE.C#L62)), and fires any trigger inside ([UW2 src/obj/TREASURE.C:67](../src/obj/TREASURE.C#L67)).
- **Lock picking.** Both games refuse a lock of height 0xF, and a lock of height 0xE to a Picklock below 31 ([UW1 src/obj/USEITEMS.C:1027](https://github.com/abedegno/UW1Decomp/blob/main/src/obj/USEITEMS.C#L1027)), ([UW2 src/obj/OBJUSE.C:326](../src/obj/OBJUSE.C#L326)). UW1 rolls Picklock against 3 times the lock's height ([UW1 src/obj/USEITEMS.C:232](https://github.com/abedegno/UW1Decomp/blob/main/src/obj/USEITEMS.C#L232)). UW2 adds 1 to Picklock ([UW2 src/obj/USEITEMS.C:535](../src/obj/USEITEMS.C#L535)), and on a critical failure the pick breaks unless `skill_check(dexterity, 20)` succeeds ([UW2 src/obj/USEITEMS.C:537](../src/obj/USEITEMS.C#L537)). UnderworldGodot uses UW2's rule for both and has neither height rule ([`src/objects/lockpick.cs:44`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/objects/lockpick.cs#L44)). Whether any shipped lock has height 0xE or 0xF was not checked.
- **Combining.** UW1 combines a pair only when the entry in `cmb.dat` marks one of the two as used up ([UW1 src/obj/COMBINE.C:70](https://github.com/abedegno/UW1Decomp/blob/main/src/obj/COMBINE.C#L70)). UW2 combines any listed pair ([UW2 src/obj/COMBINE.C:57](../src/obj/COMBINE.C#L57)). Not checked in UnderworldGodot.
- **Food.** UW1 has rotworm stew, made in a bowl ([UW1 src/obj/COMBINE.C:145](https://github.com/abedegno/UW1Decomp/blob/main/src/obj/COMBINE.C#L145)), and the toadstool poisons ([UW1 src/obj/USEITEMS.C:681](https://github.com/abedegno/UW1Decomp/blob/main/src/obj/USEITEMS.C#L681)). In UW2 eating leaves a stick, bone, wax or bottle behind ([UW2 src/obj/USEITEMS.C:333](../src/obj/USEITEMS.C#L333)), and the dream plant sets `sleepbits` ([UW2 src/obj/USEITEMS.C:295](../src/obj/USEITEMS.C#L295)).
- **Lights.** UW1 refuses to light a light of quality 0 ([UW1 src/obj/USEITEMS.C:522](https://github.com/abedegno/UW1Decomp/blob/main/src/obj/USEITEMS.C#L522)), and UW2 refuses quality 1 or less ([UW2 src/obj/USEITEMS.C:620](../src/obj/USEITEMS.C#L620)).
- **Fixed objects.** In UW2 a bed lets the player sleep ([UW2 src/obj/USEITEMS.C:911](../src/obj/USEITEMS.C#L911)), and opening an owned container, or emptying an owned bag, angers its owners ([UW2 src/obj/USEITEMS.C:920](../src/obj/USEITEMS.C#L920)), ([UW2 src/obj/USEITEMS.C:825](../src/obj/USEITEMS.C#L825)). In UW1, using a shrine starts a mantra ([UW1 src/obj/USEITEMS.C:988](https://github.com/abedegno/UW1Decomp/blob/main/src/obj/USEITEMS.C#L988)).
- **Containers.** The containers take different items. UW2 adds a key ring (`CONT_KEYS`) ([UW2 src/inv/INVPANEL.C:626](../src/inv/INVPANEL.C#L626)). Its scroll case takes types 4 to 7, 9 and 10 ([UW2 src/inv/INVPANEL.C:633](../src/inv/INVPANEL.C#L633)) where UW1's takes 8 and up ([UW1 src/inv/INVPANEL.C:644](https://github.com/abedegno/UW1Decomp/blob/main/src/inv/INVPANEL.C#L644)). Its quiver also takes wands (misc class, minor 1, types 8 and up) ([UW2 src/inv/INVPANEL.C:630](../src/inv/INVPANEL.C#L630)). The food lists differ, e.g., UW1's takes candles and leeches ([UW1 src/inv/INVPANEL.C:647](https://github.com/abedegno/UW1Decomp/blob/main/src/inv/INVPANEL.C#L647)) and UW2's leaves out bottles ([UW2 src/inv/INVPANEL.C:636](../src/inv/INVPANEL.C#L636)).
- **Stacks.** A UW2 storage crystal never stacks ([UW2 src/inv/INVPANEL.C:705](../src/inv/INVPANEL.C#L705)).
- **Moonstones.** UW1 remembers one moonstone's level ([UW1 src/inv/BAGS.C:83](https://github.com/abedegno/UW1Decomp/blob/main/src/inv/BAGS.C#L83)), and UW2 remembers two ([UW2 src/inv/BAGS.C:73](../src/inv/BAGS.C#L73)).
- **Owners shown on look.** UW1 names owners up to race 0x1B ([UW1 src/obj/LOOK.C:121](https://github.com/abedegno/UW1Decomp/blob/main/src/obj/LOOK.C#L121)), and UW2 up to 0x1E ([UW2 src/obj/LOOK.C:114](../src/obj/LOOK.C#L114)).
- **Time stop (UW2).** Animations freeze while time is stopped ([UW2 src/obj/EFFECT.C:252](../src/obj/EFFECT.C#L252)), and timer triggers fire only within 8 squares of the player or the wizard eye ([UW2 src/obj/EFFECT.C:279](../src/obj/EFFECT.C#L279)).

## Traps and triggers

- **Trigger classes.** UW1 treats only minor class 2 as a trigger ([UW1 src/event/TRIGGER.C:90](https://github.com/abedegno/UW1Decomp/blob/main/src/event/TRIGGER.C#L90)), and UW2 treats 2 and 3 ([UW2 src/event/TRIGGER.C:72](../src/event/TRIGGER.C#L72)).
- **A thrown object on a trigger.** In UW1 an object that is not a critter sets off a trigger unless the trigger is enchanted ([UW1 src/event/TRIGGER.C:130](https://github.com/abedegno/UW1Decomp/blob/main/src/event/TRIGGER.C#L130)). In UW2 the trigger must have flag 9 ([UW2 src/event/TRIGGER.C:115](../src/event/TRIGGER.C#L115)).
- **Game variables.** UW1's set-variable trap either changes one of 32 quest bits by its heading or does arithmetic on one of 64 variables kept to 6 bits ([UW1 src/event/TRIGGER.C:255](https://github.com/abedegno/UW1Decomp/blob/main/src/event/TRIGGER.C#L255)). UW2's uses the 256 numbered variables through `set_numbered_variable` ([UW2 src/event/TRIGGER.C:342](../src/event/TRIGGER.C#L342)), and the check-variable trap reads them likewise ([UW2 src/event/TRIGGER.C:346](../src/event/TRIGGER.C#L346)).
- **Create object.** In UW2 a template of the adventurer item makes a random castle monster scaled by the level and the castle X clock ([UW2 src/event/TRIGGER.C:538](../src/event/TRIGGER.C#L538)), and nothing is created in world 6 once a quest bit is set ([UW2 src/event/TRIGGER.C:536](../src/event/TRIGGER.C#L536)).
- **Change terrain.** UW1 allows heights to 13, floors below 11 and walls below 48 ([UW1 src/event/WORLDEV.C:227](https://github.com/abedegno/UW1Decomp/blob/main/src/event/WORLDEV.C#L227)), ([UW1 src/event/WORLDEV.C:256](https://github.com/abedegno/UW1Decomp/blob/main/src/event/WORLDEV.C#L256)). UW2 allows height 14, floors below 15 and walls below 63 ([UW2 src/event/WORLDEV.C:358](../src/event/WORLDEV.C#L358)), ([UW2 src/event/WORLDEV.C:362](../src/event/WORLDEV.C#L362)), and moves objects on the neighbouring squares as well ([UW2 src/event/WORLDEV.C:394](../src/event/WORLDEV.C#L394)).
- **Inventory trap.** UW2 can also require the item to be worn ([UW2 src/event/TRIGGER.C:670](../src/event/TRIGGER.C#L670)).
- **Text trap.** In UW2 a text whose owner field has bit 5 set prints only when a player flag is set ([UW2 src/event/TRIGGER.C:697](../src/event/TRIGGER.C#L697)). What that flag means is not known.
- **Door trap.** UW2 swaps the lock only when the trap's owner field is not zero ([UW2 src/event/TRIGGER.C:633](../src/event/TRIGGER.C#L633)).
- **New in UW2.** UW2 adds the special effect ([UW2 src/event/TRIGGER.C:306](../src/event/TRIGGER.C#L306)), jump ([UW2 src/event/TRIGGER.C:335](../src/event/TRIGGER.C#L335)), skill check ([UW2 src/event/TRIGGER.C:364](../src/event/TRIGGER.C#L364)), proximity ([UW2 src/event/TRIGGER.C:386](../src/event/TRIGGER.C#L386)), change-from ([UW2 src/event/TRIGGER.C:397](../src/event/TRIGGER.C#L397)), oscillator ([UW2 src/event/TRIGGER.C:401](../src/event/TRIGGER.C#L401)), pit ([UW2 src/event/TRIGGER.C:450](../src/event/TRIGGER.C#L450)), bridge ([UW2 src/event/TRIGGER.C:469](../src/event/TRIGGER.C#L469)) and experience ([UW2 src/event/TRIGGER.C:501](../src/event/TRIGGER.C#L501)) traps.
- **Hack traps.** The two games' `do_trap_hack` lists share nothing ([UW1 src/event/TRIGGER.C:503](https://github.com/abedegno/UW1Decomp/blob/main/src/event/TRIGGER.C#L503)), ([UW2 src/event/TRIGGER.C:777](../src/event/TRIGGER.C#L777)).
- **Fishing.** UW1 catches a fish one time in five ([UW1 src/event/WORLDEV.C:331](https://github.com/abedegno/UW1Decomp/blob/main/src/event/WORLDEV.C#L331)). UW2 catches one when `rand() % 5` is at most `(Track + 7) / 8` ([UW2 src/event/WORLDEV.C:538](../src/event/WORLDEV.C#L538)). UnderworldGodot uses UW2's rule for both on purpose, with a comment saying so (`src/objects/fishingpole.cs:33`).
- **Plot deaths.** UW1's `death_check` handles seven NPCs, e.g., whoami 0x16 gives up and talks, worth 500 experience the first time ([UW1 src/event/WORLDEV.C:568](https://github.com/abedegno/UW1Decomp/blob/main/src/event/WORLDEV.C#L568)). UW2's handles many more, and its yielding NPCs give 50 ([UW2 src/event/WORLDEV.C:853](../src/event/WORLDEV.C#L853)).

UnderworldGodot implements the traps in `src/traps/` with many game branches. They were not compared entry by entry.

## Movement and physics

`newFPS` scales the player's speeds by a ratio for each kind of ground, in tenths in UW1 ([UW1 src/motion/PHYSICS.C:582](https://github.com/abedegno/UW1Decomp/blob/main/src/motion/PHYSICS.C#L582)) and in twentieths in UW2 ([UW2 src/motion/PHYSICS.C:748](../src/motion/PHYSICS.C#L748)).

| State | UW1 | UW2 |
| --- | --- | --- |
| walking, ice | 1 | 1 |
| swimming | 0.3 | (Swimming / 2 + 4) / 20, 0.2 to 0.95 ([UW2 src/motion/PHYSICS.C:760](../src/motion/PHYSICS.C#L760)) |
| lava | 0.5 | 0.7 |
| levitating | 0.1 | 0.05 |
| flying | 0.7 | 0.7 |
| slow falling | 0.2 | 0.2 |

UnderworldGodot branches the speeds.

- **Turning.** The turn rate is `ratio / 10` in UW1 ([UW1 src/motion/PHYSICS.C:596](https://github.com/abedegno/UW1Decomp/blob/main/src/motion/PHYSICS.C#L596)) and `ratio / 20` in UW2 ([UW2 src/motion/PHYSICS.C:767](../src/motion/PHYSICS.C#L767)), and UnderworldGodot divides by 20 in both ([`src/physics/motion_player.cs:949`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/physics/motion_player.cs#L949)), so UW1 turns at half speed on the ground.
- **Facing.** The view swings towards the direction of motion by up to 0x400 a step in UW1 ([UW1 src/motion/PHYSICS.C:411](https://github.com/abedegno/UW1Decomp/blob/main/src/motion/PHYSICS.C#L411)) and 0x600 in UW2 ([UW2 src/motion/PHYSICS.C:587](../src/motion/PHYSICS.C#L587)). UnderworldGodot uses 0x600 for both (`src/physics/motion_player.cs:541-551`).
- **Falling.** In world 8 UW2 does no fall damage ([UW2 src/motion/PHYSICS.C:602](../src/motion/PHYSICS.C#L602)). UnderworldGodot branches.
- **Paralysis (UW2).** A paralysed player can't move ([UW2 src/motion/PHYSICS.C:658](../src/motion/PHYSICS.C#L658)).
- **Ice and currents (UW2).** On ice the player slides and keeps momentum ([UW2 src/motion/PHYSICS.C:403](../src/motion/PHYSICS.C#L403)), and in water the floor texture can push the player ([UW2 src/motion/PHYSICS.C:439](../src/motion/PHYSICS.C#L439)).
- **Lava.** Lava burns 1 point one time in five in both games. UW1 skips the burn with dragon skin boots ([UW1 src/motion/PLAYMOVE.C:505](https://github.com/abedegno/UW1Decomp/blob/main/src/motion/PLAYMOVE.C#L505)) and UW2 has no exception ([UW2 src/motion/PLAYMOVE.C:520](../src/motion/PLAYMOVE.C#L520)). UnderworldGodot branches.
- **Objects that hit things.** An impact does damage above 0x100 in UW1 ([UW1 src/motion/OBJPHYS.C:305](https://github.com/abedegno/UW1Decomp/blob/main/src/motion/OBJPHYS.C#L305)) and above 0x180 in UW2 ([UW2 src/motion/OBJPHYS.C:299](../src/motion/OBJPHYS.C#L299)). In UW2 creatures take a quarter of it, and objects with fewer than 0x20 hit points take four times as much ([UW2 src/motion/OBJPHYS.C:305](../src/motion/OBJPHYS.C#L305)).
- **Bounces.** A bounced mover leaves at upward speed 0xEB in UW1 ([UW1 src/motion/MOTION.C:475](https://github.com/abedegno/UW1Decomp/blob/main/src/motion/MOTION.C#L475)) and 0xBC in UW2 ([UW2 src/motion/MOTION.C:480](../src/motion/MOTION.C#L480)), and UW2 turns it at random only one time in four.
- **Lava and the talismans (UW1).** A talisman thrown into lava near the centre of level 8 is destroyed ([UW1 src/motion/OBJPHYS.C:435](https://github.com/abedegno/UW1Decomp/blob/main/src/motion/OBJPHYS.C#L435)). On level 9 nothing that is dropped is kept ([UW1 src/motion/OBJPHYS.C:455](https://github.com/abedegno/UW1Decomp/blob/main/src/motion/OBJPHYS.C#L455)).

## Conversations

- **Quests.** UW1's `get_quest` and `set_quest` cover 32 quest bits ([UW1 src/conv/BABLHACK.C:215](https://github.com/abedegno/UW1Decomp/blob/main/src/conv/BABLHACK.C#L215)), 4 quest bytes and, above those, the talisman count ([UW1 src/conv/BABLHACK.C:236](https://github.com/abedegno/UW1Decomp/blob/main/src/conv/BABLHACK.C#L236)). UW2's cover 128 flags ([UW2 src/conv/BABLHACK.C:504](../src/conv/BABLHACK.C#L504)) and 16 bytes, and a higher number reads a byte of the record whose meaning is not known ([UW2 src/conv/BABLHACK.C:525](../src/conv/BABLHACK.C#L525)). UW1's 16-bit shift breaks quests 15 to 31 (UW1Decomp `FINDINGS.md`).
- **Game variables.** `x_traps` stores 0 to 0x3F in UW1 ([UW1 src/conv/BABLHACK.C:125](https://github.com/abedegno/UW1Decomp/blob/main/src/conv/BABLHACK.C#L125)) and 0 to 0x3FF in UW2 ([UW2 src/conv/BABLHACK.C:410](../src/conv/BABLHACK.C#L410)).
- **Skill points (UW2).** `x_skills` with a value above 10,000 spends a skill point on a random skill of a group ([UW2 src/conv/BABLHACK.C:388](../src/conv/BABLHACK.C#L388)).
- **Who will talk.** Both games refuse to talk to a critter that is fighting the player or hostile, unless it is an ally or its goal is talk. UW1 lets whoami 0x16, 0x8E and 0xE7 talk anyway ([UW1 src/conv/CONVERSE.C:136](https://github.com/abedegno/UW1Decomp/blob/main/src/conv/CONVERSE.C#L136)), and UW2 lets 0x8C ([UW2 src/conv/CONVERSE.C:117](../src/conv/CONVERSE.C#L117)). UW2 also refuses while time is stopped ([UW2 src/conv/CONVERSE.C:113](../src/conv/CONVERSE.C#L113)) and to a held critter ([UW2 src/conv/CONVERSE.C:106](../src/conv/CONVERSE.C#L106)). In UW1, talking to a shrine starts a mantra ([UW1 src/conv/CONVERSE.C:120](https://github.com/abedegno/UW1Decomp/blob/main/src/conv/CONVERSE.C#L120)). In UW2, talking to a wisp starts conversation 0x30 ([UW2 src/conv/CONVERSE.C:98](../src/conv/CONVERSE.C#L98)). UnderworldGodot has the wisp, but none of the refusals (`src/conversation/conversationinitialisation.cs:47` notes this as a TODO), so it talks to hostile critters in both games.
- **Demanding.** In UW1 an ally always gives in to a demand ([UW1 src/conv/BARTER.C:719](https://github.com/abedegno/UW1Decomp/blob/main/src/conv/BARTER.C#L719)), and giving in lowers the NPC's attitude while it is above 0 ([UW1 src/conv/BARTER.C:723](https://github.com/abedegno/UW1Decomp/blob/main/src/conv/BARTER.C#L723)). In UW2 an ally only scores one point lower, the NPC scores half as much again on levels 9 and 17 ([UW2 src/conv/BARTER.C:694](../src/conv/BARTER.C#L694)), the attitude falls only while above 1 ([UW2 src/conv/BARTER.C:700](../src/conv/BARTER.C#L700)), and an empty demand is turned down ([UW2 src/conv/BARTER.C:666](../src/conv/BARTER.C#L666)). UnderworldGodot's UW1 path has neither UW1 rule (`src/conversation/conversation_functions/do_demand.cs:38` and `:43`).
- **Trading.** UW1 has 4 trade slots a side ([UW1 src/conv/BARTER.C:172](https://github.com/abedegno/UW1Decomp/blob/main/src/conv/BARTER.C#L172)), and UW2 has 6 ([UW2 src/conv/BARTER.C:140](../src/conv/BARTER.C#L140)). UW2 values coins at full quality ([UW2 src/conv/BARTER.C:804](../src/conv/BARTER.C#L804)), adds a fudge to the NPC's side of a judgement ([UW2 src/conv/BARTER.C:737](../src/conv/BARTER.C#L737)), and can give part of a stack ([UW2 src/conv/CONVERSE.C:700](../src/conv/CONVERSE.C#L700)). UW1 merges a created item into a stack the NPC holds ([UW1 src/conv/BARTER.C:1033](https://github.com/abedegno/UW1Decomp/blob/main/src/conv/BARTER.C#L1033)).
- **Built-ins.** UW2 adds `babl_hack`, `give_all_stuff`, `set_sequence`, `transform_talker`, `x_clock`, `x_exp`, `teleport_player`, `teleport_talker` and `switch_pic` ([UW2 src/conv/CONVERSE.C:291](../src/conv/CONVERSE.C#L291)).

## Light, exploration and the automap

- **Exploration experience.** Newly seen squares give `seen * PlayerLevel / 10` experience in UW1, except on level 9 ([UW1 src/3d/GRIDDB.C:247](https://github.com/abedegno/UW1Decomp/blob/main/src/3d/GRIDDB.C#L247)), and `seen * (PlayerLevel / 8 + 1) / 10` in UW2 ([UW2 src/3d/GRIDDB.C:246](../src/3d/GRIDDB.C#L246)). UW2's divides the level by 8 without subtracting 1 first, so level 8 already gets the multiplier of the second world (2), and level 16 that of the third. UnderworldGodot branches (`src/utility/visionlos.cs:281-288`), but it does not leave out UW1's level 9, and both games' gains then go through its halving (see [Experience](#experience)).
- **Light.** UW2 lights the view by the brighter of the player's light and a second light level, `loc_lght`, the level's minimum light from DL.DAT (`load_dl`) ([UW2 src/game/PLAYDATA.C:424](../src/game/PLAYDATA.C#L424)). UW1 uses the player's light alone.
- **Automap.** In UW1 the map is off only on level 9. In UW2 the player can lose it, e.g., a teleport trap can switch it off, and the map scraps, the Map spell and world 8 change what it shows. See `AUTOMAP.C` in each repository.

## Systems in one game only

- **UW1 only.** Mantras and the shrine (`mantra_advance`), the silver tree, the talismans and Garamon, Tybal's orb and its mana drain, the rotworm stew, the incense, the screen-frame dragons, and the options panel.
- **UW2 only.** The schedules and SCD events (`src/event/SCDEVENT.C`, `src/event/SCHEDULE.C`), the X clocks, the pits of Carnage, the castle guards and jail, the Void reached by sleeping after eating the dream plant, the vending machines and other world hacks of `WORLDEV.C`, timer triggers, special weapons, the enchantment, study monster and mend spells, and the key ring.

## UnderworldGodot summary

These are the places where UnderworldGodot, at `d7025471`, applies one game's rule to both games, has a branch that doesn't match, or lacks the rule. Each is described in its section above.

| Rule | Games | UnderworldGodot | Verdict |
| --- | --- | --- | --- |
| Casting skill roll | UW1 | [`src/magic/runicmagic.cs:339`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/magic/runicmagic.cs#L339) | one rule for both |
| Britannia circle limit | UW2 | [`src/magic/runicmagic.cs:476`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/magic/runicmagic.cs#L476) | differs (circle 3 refused) |
| Halving of every experience gain | UW1 | [`src/player/playerdat.cs:657`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdat.cs#L657) | one rule for both |
| Experience above 0x17700 | UW1 | [`src/player/playerdat.cs:687`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdat.cs#L687) | one rule for both |
| Carrying capacity | UW1 | [`src/player/playerdat.cs:794`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdat.cs#L794) | one rule for both |
| Poisonous blow | both | [`src/interaction/combat/combat.cs:224`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/interaction/combat/combat.cs#L224) | one rule for both, and UW2's roll differs |
| Poisoned weapon damage | UW2 | [`src/interaction/combat/combat.cs:422`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/interaction/combat/combat.cs#L422) | differs |
| Hit point regeneration | both | [`src/player/playerdatloop.cs:240`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdatloop.cs#L240) | differs |
| Length of the slow cycle | UW1 | [`src/player/playerdatloop.cs:218`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdatloop.cs#L218) | one rule for both |
| Hunger per cycle | both | [`src/player/playerdatloop.cs:220`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdatloop.cs#L220) | differs (3 to 5, not 3 to 6) |
| Academy mana | UW2 | [`src/player/playerdatloop.cs:291`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdatloop.cs#L291) | differs (one level out) |
| Healing from food | UW1 | [`src/player/playerdatstatus.cs:676`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdatstatus.cs#L676) | one rule for both |
| Interrupted sleep | UW1 | [`src/World/sleep.cs:238`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/World/sleep.cs#L238) | one rule for both |
| Starting fatigue | UW1 | [`src/player/playerdatainit.cs:152`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdatainit.cs#L152) | one rule for both |
| Area spells | UW1 | [`src/magic/spellcasting_class_6.cs:66`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/magic/spellcasting_class_6.cs#L66) | differs |
| Smite Undead | UW1 | [`src/magic/spellcasting_class_7.cs:442`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/magic/spellcasting_class_7.cs#L442) | missing |
| Paralyze resistance | UW1 | [`src/magic/spellcasting_class_7.cs:496`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/magic/spellcasting_class_7.cs#L496) | missing |
| Lock picking | UW1, and the height rule in both | [`src/objects/lockpick.cs:44`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/objects/lockpick.cs#L44) | one rule for both |
| Doors and peaceful critters | UW1 | [`src/npc/npcai.cs:1620`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/npc/npcai.cs#L1620) | one rule for both |
| Turn rate | UW1 | [`src/physics/motion_player.cs:949`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/physics/motion_player.cs#L949) | one rule for both |
| Facing step | UW1 | [`src/physics/motion_player.cs:541`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/physics/motion_player.cs#L541) | one rule for both |
| Refusing to talk | both | [`src/conversation/conversationinitialisation.cs:47`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/conversation/conversationinitialisation.cs#L47) | missing |
| Demands and allies | UW1 | [`src/conversation/conversation_functions/do_demand.cs:38`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/conversation/conversation_functions/do_demand.cs#L38) | missing |
| Exploration on level 9 | UW1 | [`src/utility/visionlos.cs:287`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/utility/visionlos.cs#L287) | one rule for both |
| Mana from a level gained on Tybal's level | UW1 | [`src/player/playerdat.cs:787`](https://github.com/hankmorgan/UnderworldGodot/blob/d7025471a1362860afd9f68555e137a9ff4f131a/src/player/playerdat.cs#L787) | missing (marked TODO) |
| Fishing | UW1 | `src/objects/fishingpole.cs:33` | one rule for both, on purpose |
