# Test vectors from UW.EXE

Each CSV file here is a table of inputs and outputs for one game rule of Ultima Underworld: The Stygian Abyss. The outputs were computed by running UW.EXE's own code for that rule, so they are what the original game does. They are meant for checking a reimplementation, such as UnderworldGodot, against the original.

The files hold numbers only. They are outputs of UW.EXE's routines run on synthetic inputs, not code or data from the game.

## How they are made

`make vectors` regenerates every table. It runs Exhume's `tools/vectors.py` with this repository's `tools/vector_targets.py` (the routines and how their inputs are chosen) and `tools/vectorhost-uw1.c` (the port's side). Exhume finds them through `[vectors]` in `exhume.toml`.

1. For each routine, a generator seeded with 1 makes a few hundred cases: edge values first, then random ones. The same seed always gives the same cases.
2. Each case runs the routine's bytes from your UW.EXE (SHA-256 `dcb2724c7f1dab86...`, the GOG release) in an x86 emulator, Unicorn 2.1.4. The EXE is loaded as DOS loads it, with its relocations applied. The routine is called the way the game's C calls it: a far call with its arguments on the stack and DS = SS = the data group. Any global the routine reads is set from the case first.
3. The same case also runs through the port's C for the routine (the matched source compiled for the host). The run fails if the port and the EXE disagree on any case. They agree on every case of every table.
4. The table is written as CSV: a header row of column names, then one row per case in decimal.

Regeneration is byte-identical. `make vectors VECTORS=--check` regenerates in memory and fails if any file would change.

## Random numbers

Some routines call the C library's `rand()`. For those, the input column `seed` is the whole 32-bit state of the generator before the call, and `seed_out` is its state after. Turbo C++'s `rand()` sets `seed = seed * 0x015A4E35 + 1` and returns bits 16 to 30 of the new seed (0 to 32767). So the draws a case used, and how many, follow from `seed` and `seed_out`. A reimplementation that uses another generator can still be checked: feed it the values the original drew, in the form the original's call site uses them (each routine below says which).

## The tables

Source references are to this repository. UnderworldGodot references are to its `main` at `d7025471`. UW2Decomp's vectors/ has the same tables for UW2, and two more (is_sharp and debris_type) that UW1 has no routine for.

### skill_check.csv

The game's skill roll, used wherever a skill or attribute is tested, combat's to-hit roll among them. It rolls `value - target + rand() % 31` and grades the score: above 28 gives 2 (great success), 16 to 28 gives 1 (success), 3 to 15 gives 0 (near miss), 2 or less gives -1 (failure).

- Inputs: `value` (the skill or attribute), `target` (the difficulty, such as a creature's defence), `seed`.
- Outputs: `result` (-1, 0, 1 or 2), `seed_out`. One draw, used as `rand() % 31`.
- Cases: every `value - target` from -35 to +35, then random values 0 to 63.
- Source: `src/game/SKILLCHK.C:48`, `skill_check` (UW.EXE 30F9:000C).
- UnderworldGodot: `playerdat.SkillCheck`, `src/player/playerdatskills.cs:19`.

### rollem.csv

Dice: `dice` dice of `sides` sides, each counting 1 to `sides`. Returns `dice` unchanged when either is 0 or less.

- Inputs: `dice`, `sides`, `seed`.
- Outputs: `total`, `seed_out`. One draw per die, used as `rand() * sides / 0x8000` (0 to `sides - 1`).
- Source: `src/sys/UTIL.C:72`, `rollem` (35BD:011E).
- UnderworldGodot: `Rng.DiceRoll`, `src/utility/rng.cs:43`.

### pickloc.csv

Which body location a melee blow lands on, from the heights of the defender and of the blow. A blow whose middle is below the defender's bottom gives 2, and one whose middle is above the defender's top gives 3. Otherwise a weighted random choice favours 2 or 3 by which half of the defender the blow's middle is in, and then 0 over 1. The location indexes the creature's armour values.

- Inputs: `dz`, `dtop` (the defender's bottom and top), `az`, `atop` (the blow's bottom and top), all heights in the map's height units, and `seed`.
- Outputs: `loc` (0 to 3), `seed_out`. Up to two draws, used as `rand() % 2` or `rand() % 3`, then `rand() % 3`.
- Cases: the blow's middle at every height round a defender from 40 to 64, then random heights.
- Source: `src/combat/COMBAT.C:107`, `pickloc` (2121:000A).
- UnderworldGodot: `combat.PickBodyHitPoint`, `src/interaction/combat/combat.cs:946`.

### move_along.csv

Moves a point a distance along a heading, as combat, missiles, spells and the player's steps do. The step on each axis is the table sine or cosine divided by 0x80, times the distance, divided by 0x100, and then moved one further from zero if it is not zero.

- Inputs: `heading` (a byte angle, 256 to the turn, in the game's compass convention), `dist`, `x`, `y` (`dist` is in the same unit as `x` and `y`).
- Outputs: `x_out`, `y_out`.
- Cases: every heading, then random ones, with distances 0 to 128. No caller in the game passes more than 128 (the player's step, 0x80). Above 128 the routine's 16-bit product overflows, so those inputs are left out.
- Source: `src/sys/UTIL.C:34`, `move_along` (35BD:004D).
- UnderworldGodot: `motion.GetCoordinateInDirection`, `src/physics/motion_projectile.cs:237`.

### check_res.csv

The damage left after an object type's resistances. `resist` is the type's resistance byte from COMOBJ.DAT, and `type` the damage type bits (3 magic, 4 physical, 8 fire, 0x10 poison, 0x40 missiles; the names are from UW2's Study Monster list, as DAMAGE.C notes). Magic is resisted by chance: the blow is ignored when `rand() % 3 < (resist & 3)`. Any other type bit the object resists cancels the damage. Unlike UW2's, UW1's has no rule that doubles fire or cold damage.

- Inputs: `item` (the object's item id; it only picks the COMOBJ.DAT row, which the case sets to `resist`), `resist`, `damage` (0 to 255), `type`, `seed`.
- Outputs: `result` (0 to 255), `seed_out`. At most one draw, used as `rand() % 3`.
- Cases: every pairing of 13 type and 13 resistance values with edge damages, then fire on a cold resister and cold on a fire resister over damage 0 to 255 (the cases that double in UW2; here they pass unchanged), then random bytes.
- Source: `src/combat/DAMAGE.C:166`, `check_res` (229B:02FF).
- UnderworldGodot: `damage.ScaleDamage`, `src/interaction/damage.cs:487` (UW1's body at line 500).

### compute_hitangle.csv

How far round from the defender's front the attacker stands, in eighths of a turn: 0 face to face, 4 from behind. It is added to the attack skill and to the damage of a melee blow.

- Inputs: `def_pos` and `att_pos`, the position words of the defender (object 2) and the attacker (object 1, the player). The heading is bits 7 to 9; the other bits are random and must not matter. `def_item` is the defender's item id. It does not change the original's result. It is there because UnderworldGodot's version depends on it.
- Output: `hitangle` (0 to 4).
- Source: `src/combat/COMBAT.C:481`, `compute_hitangle` (2121:0D48).
- UnderworldGodot: `combat.CalcFlankingBonus`, `src/interaction/combat/combat.cs:1031`.

### sqrt.csv

The integer square root of the 3D library's maths module. UnderworldGodot uses its version to build its light tables.

- Input: `value` (0 to 2^31 - 1).
- Output: `root` (16 bits). It is not always rounded down: the table has `sqrt(3) = 2` and `sqrt(15) = 3`.
- Cases: edge values, the squares 0 to 176 squared with one either side, then random values.
- Source: `src/sys/C3DENTRY.ASM:562`, `cSqRt` (1F3A:0F3F), which calls IMATH.ASM's square root (`src/sys/IMATH.ASM:107`).
- UnderworldGodot: `UnderWorldSqrt.sqrt_vanilla`, `src/utility/uwMath.cs:174`.

### sincos.csv

The interpolated sine and cosine table, used for headings in motion, missiles and the camera.

- Input: `angle` (65536 to the turn).
- Outputs: `a` (the sine: 0 at angle 0, 32767 at 0x4000) and `b` (the cosine: 32767 at angle 0), from -32767 to 32767.
- Cases: every multiple of 0x100 (the table's own points), random angles, then every multiple of 0x40 for a quarter turn.
- Source: `src/sys/C3DENTRY.ASM:480`, `cSinCos` (1F3A:0EAE), which calls IMATH.ASM's interpolation (`src/sys/IMATH.ASM:63`).
- UnderworldGodot: `motion.SomethingProjectileHeading_seg021_22FD_EAE`, `src/physics/motion_calc.cs:232`.
