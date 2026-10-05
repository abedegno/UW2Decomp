# Motion: candidates for the findings page

Candidates from `src/motion` for the project's findings page; [motion.md](motion.md) describes the subsystem. Each was re-read in the matched source. "Both games" means UW2Decomp's matched source has the same code.

## Likely bugs

### set_targz's preference for an object under the centre is never applied

- **What happens:** when a mover stands still, `set_targz` picks the highest touchable object below it to rest on, with a test meant to prefer one under the mover's centre (link bit 0x10): `MP.hit == -1 || !(oCollisions[MP.hit].link.f.low & 0x10) || !(!oCollisions[i].link.f.low & 0x10)`. The last term applies `!` before `&`: `!x` is 0 or 1, `& 0x10` makes it 0, and the outer `!` makes the term always true. So the whole test always passes.
- **Where:** `set_targz` in [motion/MOTION.C](../../src/motion/MOTION.C) (marked there).
- **Evidence:** code reading; the matched source compiles to the shipped bytes as written. Both games.
- **Confidence:** likely (operator precedence; the parenthesisation shows the intent).
- **Effect:** with several objects underfoot, the highest touchable one wins even when the current choice is under the centre and it is not. Probably rarely visible; not checked in the game.
- **For a port:** keep the expression to match DOS.

## Possible bugs

### Floor heights 14 and 15 convert to z 0

- **What happens:** `hgt_val` (`map/MAP.C`), which `player_setup` uses to place the player on a tile's floor, has 0 for heights 14 and 15.
- **Evidence:** code reading; both games (UW2's findings list it). The shipped UW1 `LEV.ARK` has no open tile at height 14 or 15 on any of its nine levels.
- **Confidence:** possible; no effect with the shipped levels.

## Game rules recovered

- Fall damage: `PN.impact >> 8`, doubled while falling, reduced by `(30 - acrobat) / 30` on a successful Acrobat check against twice the damage, dealt only above 3 (`phys_affect_player`).
- Movement speeds by state, out of 10 of the full speeds: walking 10, swimming 3, lava 5, levitating 1, flying 7, slow falling 2; turning is scaled the same way on the ground (`newFPS`).
- Carrying more than half the maximum weight lowers the most the speed can change in a tick, linearly to 0 at the maximum (`newFPS`'s `MaxPlayerAccel`).
- A jump starts at vertical speed 0x263, five sixths of it above z 0x280 and two thirds of that above 0x2C0; Leap halves gravity (`do_player_input`).
- Wearing the dragon skin boots spares the player the lava's burn (1 point one time in five, `parse_effect`).
- A thrown talisman (fate 10) landing in the lava within 6 tiles of the centre of level 8 is destroyed in an explosion while `player->talisman_ok` is set; when the last one goes the game prints "A rending sound fills the air." and `SKILLS.C`'s `check_victory` opens the moongate. Without `talisman_ok`, bit 3 of the dreams word is set instead, which brings dream 3 (`SKILLS.C`'s `dream`) (`mob_to_static`).
- Nothing dropped on level 9 survives coming to rest (`mob_to_static`).
- Objects that land in water are destroyed; in lava, unless their quality class is 3, they survive only a fire resistance check (`obj_deal`).

## Dead code

- `seg031_2CFA_A3F` in `MOTION.C`, an empty function with no caller (UW2 has it too).
- `IgnoringInput`, `oldPlayerInput` and `dseg_5c99_761` in `PLAYMOVE.C`, and `saved_dz` in `PHYSICS.C`, are never read.
- `MS_ICE` and `FPS_ICE`: UW1 keeps UW2's ice movement state, but `parse_player_terr` never picks it.
