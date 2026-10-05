# Where the rules of UW1 and UW2 differ

The full catalogue is in UW2Decomp, at [docs/UW1-UW2-DIFFERENCES.md](https://github.com/abedegno/UW2Decomp/blob/main/docs/UW1-UW2-DIFFERENCES.md). It lists, routine by routine, where the two games play by different rules, with links to the matched source of both games and what UnderworldGodot does for each rule. This page gives the main differences only.

Of 2,056 UW1 functions, 646 compile to the same instructions as their UW2 pair, 358 differ in constants only, and 688 differ in code. Most of the code differences are compiler-level or engine changes, not rule changes. 364 have no UW2 pair, 166 of them Borland's C library. The catalogue explains how the pairs were made and counted.

## Main differences

- **Casting.** UW1's circle is the spell index / 6 + 1, and its roll is `skill_check(Casting + 5, 2 * circle)`. UW2's circle is index / 8 + 1, its roll `skill_check(Casting, 3 * circle)`, and it waits twice as long between casts ([RUNES.C](../src/combat/RUNES.C)).
- **Experience.** UW1 gives every gain in full, a skill point every 3,000, and ignores gains above 0x17700. UW2 halves every gain and gives a skill point every 1,500 ([SKILLCHK.C](../src/game/SKILLCHK.C)).
- **Carrying capacity.** UW1's is strength * 20 tenths of a stone, and UW2's strength * 13 + 300 ([SKILLS.C](../src/game/SKILLS.C)).
- **The slow clock.** UW1's hunger, fatigue and healing cycle runs every 24 steps, UW2's every 30. Food heals `food_heal / 8` in UW1 and `/ 6` in UW2 ([PLAYTIME.C](../src/game/PLAYTIME.C)).
- **Area spells.** UW1's act 3d4 times within 2 squares of a point 4 ahead. UW2 has a table of counts, distances and radii ([SPELLS.C](../src/combat/SPELLS.C)).
- **Damage.** UW2 doubles fire on cold-resistant creatures and cold on fire-resistant ones, and UW1 doesn't ([DAMAGE.C](../src/combat/DAMAGE.C)). UW2 adds special weapons and Poison Weapon.
- **Critters.** In UW1 a critter that is not hostile never opens a door, and Tybal's guards never cast while the orb is whole ([PATHFIND.C](../src/critter/PATHFIND.C), [AI.C](../src/critter/AI.C)).
- **Lock picking.** UW2 adds 1 to Picklock and can break the pick on a critical failure ([USEITEMS.C](../src/obj/USEITEMS.C)).
- **Conversations.** UW1 keeps 32 quest bits, 4 quest bytes and 64 six-bit variables. UW2 keeps 128 quest flags, 16 bytes and 256 sixteen-bit variables. In UW1 an ally always gives in to a demand ([BABLHACK.C](../src/conv/BABLHACK.C), [BARTER.C](../src/conv/BARTER.C)).
- **PLAYER.DAT.** UW1's record is 0xD2 bytes, UW2's 0x37D, with different ciphers ([MISCUTIL.C](../src/sys/MISCUTIL.C)).

## UnderworldGodot

At commit `d7025471`, UnderworldGodot applies UW2's rule to UW1 for the casting roll, the halving of experience, carrying capacity, the length of the slow clock, healing from food, lock picking, critters opening doors and the turn rate. Its UW1 area spells and UW1 Smite Undead differ from DOS. The catalogue's last section lists every gap with its source line.
