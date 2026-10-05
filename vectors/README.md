# Test vectors from UW2.EXE

Each CSV file here is a table of inputs and outputs for one game rule of Ultima Underworld II. The outputs were computed by running UW2.EXE's own code for that rule, so they are what the original game does. They are meant for checking a reimplementation, such as UnderworldGodot, against the original.

The files hold numbers only. They are outputs of UW2.EXE's routines run on synthetic inputs, not code or data from the game.

## How they are made

`make vectors` regenerates every table. It runs Exhume's `tools/vectors.py` with this repository's `tools/vector_targets.py` (the routines and how their inputs are chosen) and `tools/vectorhost-uw2.c` (the port's side).

1. For each routine, a generator seeded with 1 makes a few hundred cases: edge values first, then random ones. The same seed always gives the same cases.
2. Each case runs the routine's bytes from your UW2.EXE (SHA-256 `bf233abbfeb5b664...`, the GOG release) in an x86 emulator, Unicorn 2.1.4. The EXE is loaded as DOS loads it, with its relocations applied. The routine is called the way the game's C calls it: a far call with its arguments on the stack and DS = SS = the data group. Any global the routine reads is set from the case first.
3. The same case also runs through the port's C for the routine (the matched source compiled for the host). The run fails if the port and the EXE disagree on any case. They agree on every case of every table.
4. The table is written as CSV: a header row of column names, then one row per case in decimal.

Regeneration is byte-identical. `make vectors VECTORS=--check` regenerates in memory and fails if any file would change.

## Random numbers

Some routines call the C library's `rand()`. For those, the input column `seed` is the whole 32-bit state of the generator before the call, and `seed_out` is its state after. Turbo C++'s `rand()` sets `seed = seed * 0x015A4E35 + 1` and returns bits 16 to 30 of the new seed (0 to 32767). So the draws a case used, and how many, follow from `seed` and `seed_out`. A reimplementation that uses another generator can still be checked: feed it the values the original drew, in the form the original's call site uses them (each routine below says which).

## The tables

Source references are to this repository. UnderworldGodot references are to its `main` at `d7025471`.

### skill_check.csv

The game's one skill roll, used for combat to-hit, spells, traps, bartering and more. It rolls `value - target + rand() % 31` and grades the score: above 28 gives 2 (great success), 16 to 28 gives 1 (success), 3 to 15 gives 0 (near miss), 2 or less gives -1 (failure).

- Inputs: `value` (the skill or attribute), `target` (the difficulty, such as a creature's defence), `seed`.
- Outputs: `result` (-1, 0, 1 or 2), `seed_out`. One draw, used as `rand() % 31`.
- Cases: every `value - target` from -35 to +35, then random values 0 to 63.
- Source: `src/game/SKILLCHK.C:39`, `skill_check` (UW2.EXE 323F:0009).
- UnderworldGodot: `playerdat.SkillCheck`, `src/player/playerdatskills.cs:19`.

### rollem.csv

Dice: `dice` dice of `sides` sides, each counting 1 to `sides`. Returns `dice` unchanged when either is 0 or less.

- Inputs: `dice`, `sides`, `seed`.
- Outputs: `total`, `seed_out`. One draw per die, used as `rand() * sides / 0x8000` (0 to `sides - 1`).
- Source: `src/sys/UTIL.C:73`, `rollem` (33EA:011F).
- UnderworldGodot: `Rng.DiceRoll`, `src/utility/rng.cs:43`.

### pickloc.csv

Which body location a melee blow lands on, from the heights of the defender and of the blow. A blow whose middle is below the defender's bottom gives 2, and one whose middle is above the defender's top gives 3. Otherwise a weighted random choice favours 2 or 3 by which half of the defender the blow's middle is in, and then 0 over 1. The location indexes the creature's armour values.

- Inputs: `dz`, `dtop` (the defender's bottom and top), `az`, `atop` (the blow's bottom and top), all heights in the map's height units, and `seed`.
- Outputs: `loc` (0 to 3), `seed_out`. Up to two draws, used as `rand() % 2` or `rand() % 3`, then `rand() % 3`.
- Cases: the blow's middle at every height round a defender from 40 to 64, then random heights.
- Source: `src/combat/COMBAT.C:98`, `pickloc` (22FC:0003).
- UnderworldGodot: `combat.PickBodyHitPoint`, `src/interaction/combat/combat.cs:946`.

### move_along.csv

Moves a point a distance along a heading, as combat, missiles, spells and the player's steps do. The step on each axis is the table sine or cosine divided by 0x80, times the distance, divided by 0x100, and then moved one further from zero if it is not zero.

- Inputs: `heading` (a byte angle, 256 to the turn, in the game's compass convention), `dist`, `x`, `y` (`dist` is in the same unit as `x` and `y`).
- Outputs: `x_out`, `y_out`.
- Cases: every heading, then random ones, with distances 0 to 128. No caller in the game passes more than 128 (the player's step, 0x80). Above 128 the routine's 16-bit product overflows, so those inputs are left out.
- Source: `src/sys/UTIL.C:35`, `move_along` (33EA:004E).
- UnderworldGodot: `motion.GetCoordinateInDirection`, `src/physics/motion_projectile.cs:237`.

### check_res.csv

The damage left after an object type's resistances. `resist` is the type's resistance byte from COMOBJ.DAT, and `type` the damage type bits (3 magic, 4 physical, 8 fire, 0x10 poison, 0x20 cold, 0x40 missiles). Magic is resisted by chance: the blow is ignored when `rand() % 3 < (resist & 3)`. Any other type bit the object resists cancels the damage. Fire against a cold resister, or cold against a fire resister that does not also resist cold, doubles the damage, up to 0xFF.

- Inputs: `item` (the object's item id; it only picks the COMOBJ.DAT row, which the case sets to `resist`), `resist`, `damage` (0 to 255), `type`, `seed`.
- Outputs: `result` (0 to 255), `seed_out`. At most one draw, used as `rand() % 3`.
- Cases: every pairing of 13 type and 13 resistance values with edge damages, then the fire and cold doubling over damage 0 to 255, then random bytes.
- Source: `src/combat/DAMAGE.C:210`, `check_res` (24B4:0493).
- UnderworldGodot: `damage.ScaleDamage`, `src/interaction/damage.cs:487` (UW2's body at line 538).

### compute_hitangle.csv

How far round from the defender's front the attacker stands, in eighths of a turn: 0 face to face, 4 from behind. It is added to the attack skill and to the damage of a melee blow.

- Inputs: `def_pos` and `att_pos`, the position words of the defender (object 2) and the attacker (object 1, the player). The heading is bits 7 to 9; the other bits are random and must not matter. `def_item` is the defender's item id. It does not change the original's result. It is there because UnderworldGodot's version depends on it.
- Output: `hitangle` (0 to 4).
- Source: `src/combat/COMBAT.C:516`, `compute_hitangle` (22FC:0E74).
- UnderworldGodot: `combat.CalcFlankingBonus`, `src/interaction/combat/combat.cs:1031`.

### is_sharp.csv

Whether a weapon is edged or pointed, which decides whether the Poison Weapon spell adds damage and which swing sound plays. It is true for weapons and missiles (major class 0, minor class 0 or 1) other than items 7 to 9 and the sling (0x18).

- Input: `item` (the object's id word, flags clear).
- Output: `sharp` (1 or 0).
- Source: `src/combat/COMBAT.C:275`, `is_sharp` (22FC:06D4).
- UnderworldGodot: `combat.checkforPoisonableWeapon`, `src/interaction/combat/combat.cs:1000`.

### debris_type.csv

The item a destroyed object turns into. A melee weapon (items 0 to 15) destroyed by anything but fire (damage type 8) becomes its skill's broken weapon (`Weapons[item & 15].skill + 0xC5`), and the dagger becomes the broken dagger (0xC7). Furniture becomes wood chips (0xDC). Anything else, a weapon burnt by fire included, becomes a pile of debris (0xD6).

- Inputs: `item`, `type` (the damage type), `weapon_skill` (the value the case sets in `Weapons[item & 15].skill`, from OBJECTS.DAT in the game).
- Output: `debris` (an item id).
- Source: `src/combat/DAMAGE.C:190`, `debris_type` (24B4:0444).
- UnderworldGodot: `damage.GetObjectTypeDebris`, `src/interaction/damage.cs:445`.

### sqrt.csv

The integer square root of the 3D library's maths module. UnderworldGodot uses its version to build its light tables.

- Input: `value` (0 to 2^31 - 1).
- Output: `root` (16 bits). It is not always rounded down: the table has `sqrt(3) = 2` and `sqrt(15) = 3`.
- Cases: edge values, the squares 0 to 176 squared with one either side, then random values.
- Source: `src/sys/C3DENTRY.ASM:551`, `cSqRt` (2110:0F3F), which calls IMATH.ASM's square root (`src/sys/IMATH.ASM:103`).
- UnderworldGodot: `UnderWorldSqrt.sqrt_vanilla`, `src/utility/uwMath.cs:174`.

### sincos.csv

The interpolated sine and cosine table, used for headings in motion, missiles and the camera.

- Input: `angle` (65536 to the turn).
- Outputs: `a` (the sine: 0 at angle 0, 32767 at 0x4000) and `b` (the cosine: 32767 at angle 0), from -32767 to 32767.
- Cases: every multiple of 0x100 (the table's own points), random angles, then every multiple of 0x40 for a quarter turn.
- Source: `src/sys/C3DENTRY.ASM:469`, `cSinCos` (2110:0EAE), which calls IMATH.ASM's interpolation (`src/sys/IMATH.ASM:59`).
- UnderworldGodot: `motion.SomethingProjectileHeading_seg021_22FD_EAE`, `src/physics/motion_calc.cs:232`.
