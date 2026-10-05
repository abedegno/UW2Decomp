# Motion and physics

The motion subsystem moves the player, critters, thrown objects and missiles through the level's tile grid, keeps them out of walls and objects, makes them fall, slide, swim and bounce, and turns the player's mouse and keys into movement. It is five resident C files in `src/motion/`, with the declarations, the footing bits (`FOOT_*`) and the movement commands (`PIN_*`) in `src/include/motion.h`, the collision record in `src/include/map.h`, and the player's movement state (`MS_*`, `FPS_*`, `MB_*`) in `src/include/player.h`. The candidates for the findings page are in [motion-findings.md](motion-findings.md).

| File | Segment | What it does |
|------|---------|--------------|
| [`PHYSICS.C`](../../src/motion/PHYSICS.C) | seg008_1B2A | the player's physics: movement state, the step moves, applying the result to the player object, placing the player, quake traps |
| [`COLLIDE.C`](../../src/motion/COLLIDE.C) | seg026 | terrain and object collision under a footprint; placing objects |
| [`OBJPHYS.C`](../../src/motion/OBJPHYS.C) | seg029_29EE | objects to and from a physics record, static and mobile, object hits, settling a dropped object |
| [`MOTION.C`](../../src/motion/MOTION.C) | seg030_2B26 | `do_physics`: stepping a physics record through the grid and reacting to collisions |
| [`PLAYMOVE.C`](../../src/motion/PLAYMOVE.C) | seg034_2F89 | the physics clock, player input, head bob and shakes, footstep and swimming sounds, the camera |

UW1 has no build with symbols: the function and global names are UW2's FM Towns originals where the routine is the same. The engine is UW2's a year earlier; each file's header lists what UW1 lacks or does differently (no ice, no water currents, no pressure plates in the player's tile change, a simpler `do_zbounce`, three quake functions of UW1's own).

## Units

- Positions in a physics record (`struct Phys`): x and y in 1/256 tiles (`x >> 8` the tile, `x >> 5` the 1/8-tile cell an object stores), z in 1/8 of an object's z unit. A tile's floor is at its height times 8 (`MAP.C`'s `hgt_val` in physics units).
- Positions in the collision record (`struct MotionCalc`): x and y in 1/8 tiles, z in object units.
- Headings: a full turn is 0x10000; objects keep 3 bits or, when mobile, 8 bits.
- Speed: `Phys.speed` counts 0x2F to one step of an object's stored speed. The player runs at `Run_FPS` 0x3AC.
- Time: the 256 Hz tick counter `*Time` (`sound/SOUND.C`'s timer). `check_physics` moves the world by the time since its last call, at most 0x40 ticks.

## The records

- `struct Phys` (40 bytes) is what moves: position, velocity, gravity (`acc[2]`, normally -4), the time to run, heading and speed, bounce (0 to 15), size, the step height `b24`, the footing and the impact (speed lost in collisions, which becomes damage). `PN` is the player's; `CN1`, `CN2`, `CN4` walking, flying and swimming critters', `CN3` other mobile objects'.
- `struct Handler` (12 bytes) says how a mover reacts: state bits to ignore, bits to pass to its `special` function (which gets the state word's address), and bits that forbid stepping or climbing. `PT` is the player's (`player_sqhandler`, which stops a slow walker at a ledge); `CT1` to `CT4` are set by `critter/PATHFIND.C`'s `init_ai`.
- `struct MotionCalc` (23 bytes, `map.h`) is the collision record `COLLIDE.C` fills through `curP`; `Ppd` is the one `do_physics` uses.
- `struct MotionParams`, the single global `MP`, is the stepping state of the move in progress (FM Towns' `_MP`).

## Stepping: do_physics

`do_physics(pp, tp)` runs a record for its `time`. `space_to_motion` turns heading and speed into a velocity and sets up a Bresenham walk along the faster axis, one 1/8-tile cell a step; `grid_move` takes the steps (`flat_move`, or `full_move` with vertical motion); after each step `check_positions` asks `get_pcoll` what the mover overlaps (a word of state bits, from `COLLIDE.C` and `OBJPHYS.C`'s `do_objhit`) and reacts: steps back, stops, calls the handler's special function, bounces off a wall (`do_2dbounce`, `rehead`), or starts a fall. Reaching the target height in z goes to `do_zbounce`. After 16 colliding steps the mover is stopped. `back_to_space` writes the result back.

The state bits are read from the code (nothing names them; `MOTION.C`'s header lists them): bits 0-1 the floor's terrain class, 4 on the floor, 8 << class a footprint corner on that class, 0x80 on a solid object, 0x100 too high, 0x200 a wall, 0x400 an object in the way, 0x800 a drop, 0x1000 nothing underneath, 0x2000 a slope, 0x4000 and 0x8000 an object hit that stops the mover or ends the move. `set_resterr` reduces them to the footing: `FOOT_FLOOR`, `FOOT_WATER`, `FOOT_LAVA`, `FOOT_ICE`, `FOOT_AIR`, and `FOOT_SHORE` for the player on water with a corner on another floor. UW1's floor classes come from `TERRAIN.DAT` shifted right 4 (0x10 water, 0x20 lava; `map.h`'s `TERR_*`); its levels have no ice, though the engine handles it.

## Collision and placing: COLLIDE.C

`TerrainCheck` reads the 3x3 tiles around the mover and tests five points, the centre and the four corners of a square footprint; `GetHgt` gives a point's floor height (0x80 in a wall or the closed half of a diagonal, rising across a slope). `ObjectCheck` collects the objects whose squares overlap the footprint into `oCollisions` (at most 8) and `process_objlist` sorts them into those underfoot and those in the way. `can_place`, `put_at`, `near_mob_put_at` and `drop_around_place` place objects all over the game; `drop_around_place` tries up to 24 random spots.

## Objects: OBJPHYS.C

A static object (8 bytes) has no motion state; a mobile one keeps heading, speed, pitch, gravity and footing, and a mobile non-creature keeps its exact position in `goal_word`, `attitude_word` and `b0F`. `get_phys_data` and `set_phys_data` convert between an object and a `struct Phys`. `set_phys_data` moves the object between tile lists, turns impact above 0x100 into damage, burns objects on lava one time in five, makes a moving static object mobile and settles a stopped mobile one (`mob_to_static`, `obj_deal`). `mob_to_static` is where a talisman thrown into the lava at the centre of level 8 is destroyed, and where nothing is kept on level 9.

## The player: PHYSICS.C and PLAYMOVE.C

The 3D view calls `check_physics` every frame: it adds the elapsed time to the game clock and calls `move_physics`, which runs `move_player` (`set_player_phys_params`, `do_physics(&PN, &PT)`, `phys_affect_player`), the critters (`critter/AI.C`'s `move_mobile`), the screen effects, the footsteps and the player's noise.

`PlayerInput` is the movement command (`PIN_*`): forward at the rates the mouse or keys set, back, sideways, the two jumps, and up and down while levitating or flying. The movement state (`newFPS`, `FPS_*`) sets the speeds: swimming at 3/10 of the full speeds, lava 5/10, levitating 1/10, flying 7/10, slow falling 2/10; UW1 has no swimming skill in it. Carrying more than half the most the player can lowers `MaxPlayerAccel`. Falls hurt through `PN.impact`: `impact >> 8`, doubled in a fall, cut by an Acrobat skill check, dealt above 3.

The step moves (`simple_fizix`, the capital letters and the arrows over the view) bypass `do_physics`: a 45 degree turn, a half-tile step forward or a quarter-tile step back, tested with `can_place`.

## Open questions

- Bit 0x10 of `motion_state` is tested with swimming (0x11) but `newFPS` never sets it.
- `Phys.b1D`, `MotionParams.f21` and `Handler.w6`'s meaning beyond critter path costs are not known.
- `EtherealVoidSpecialEffects_seg008_150` (a flash, lost hit points, a shake) is called from `ui/INTERACT.C`; under what condition was not traced.
