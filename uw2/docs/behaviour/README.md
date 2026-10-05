# What the games do

These pages describe how Ultima Underworld I and II behave, as rules and decision procedures with exact numbers, in the order the game applies them. They are for game programmers and modders who want to reproduce or change the games and will not read the C.

Every rule is read from the matched sources of this repository and of [UW1Decomp](https://github.com/abedegno/UW1Decomp), which compile to the same bytes as the shipped `UW2.EXE` and `UW.EXE`. Each section ends with a "Source" line that links the routines it describes. Each rule is marked **both**, **UW1** or **UW2**, and where the games differ the page gives both rules or links [UW1-UW2-DIFFERENCES.md](../UW1-UW2-DIFFERENCES.md). Where the purpose of some code is inferred rather than evident, the page says so. Nothing on these pages was checked in a running game.

The code-oriented notes on the same sources are in [docs/subsystems/](../subsystems/), the file layouts in [FORMATS.md](../FORMATS.md), and the likely bugs and open questions in [FINDINGS.md](../FINDINGS.md).

| Page | What it covers |
| --- | --- |
| [npc-ai.md](npc-ai.md) | how critters think: when they update, the goals, perception, melee and the wind-up, spells and missiles, fleeing, path finding and doors, how hostility starts and spreads, time passing while the player is away, death and remains |
| [schedules.md](schedules.md) | game time, the X clocks, the schedules in `SCD.ARK` and every row event (UW2), the castle household's day, wandering monsters and doors closing |
| [combat.md](combat.md) | the player's swing, a critter's blow, the to-hit roll, criticals, the damage roll, armour, missiles, resistances, damage to objects, special weapons, experience for a kill |
| [magic.md](magic.md) | casting from runes, where magic fails, mana, active spells and their durations, every spell class, the UW2 spell list |
| [conversations.md](conversations.md) | who will talk, the script machine, @-variables, the variables in and out, every built-in function, bartering, the Pits of Carnage |
| [player-upkeep.md](player-upkeep.md) | the slow update, hunger, fatigue, healing, poison, drink, drowning, lights, sleep, dreams, experience and levels |
| [traps-triggers.md](traps-triggers.md) | every trigger kind and who may set it off, pressure plates and timers, how traps chain and branch, every trap kind and its fields, the hack traps of both games, used triggers and removing traps, finding and disarming traps |

Some formulas have test vectors made by running the original EXEs' own code, listed in [vectors/README.md](../../vectors/README.md) and linked from [combat.md](combat.md).
