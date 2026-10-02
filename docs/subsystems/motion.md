# Motion and physics

The motion subsystem moves the player, critters, thrown objects and missiles through the level's tile grid, keeps them out of walls and objects, makes them fall, slide, swim and bounce, and turns the player's mouse and keys into movement. It is five resident C files in `src/motion/`, with the declarations in `src/include/motion.h` and the collision record in `src/include/map.h`.

| File | Segment | What it does |
|------|---------|--------------|
| `PHYSICS.C` | seg008 | the player's physics: movement state, ice and currents, the step moves, applying the result to the player object, changing tile |
| `COLLIDE.C` | seg028 | terrain and object collision under a footprint; placing objects |
| `OBJPHYS.C` | seg030 | objects: to and from a physics record, static and mobile, object hits, settling a dropped object |
| `MOTION.C` | seg031 | `do_physics`: stepping a physics record through the grid and reacting to collisions |
| `PLAYMOVE.C` | seg035 | the physics clock, player input, head bob and shakes, footstep sounds, the camera |

The function and global names are the FM Towns originals wherever it has them. No file name is known: `PHYSICS.C` is inferred (System Shock has a `PHYSICS.C` with the same job), the other four are descriptive (`map/filenames.tsv`). System Shock's physics is built on the EDMS library and shares no code with these files, so it helps only with the naming.

## Units

- Positions in a physics record (`struct Phys`): x and y in 1/256 tiles, so `x >> 8` is the tile and `x >> 5` the 1/8-tile cell an object stores as its fine position; z in 1/8 of an object's z unit (`z >> 3` is the 0 to 127 height an object stores). A tile's floor is at its height times 8.
- Positions in the collision record (`struct MotionCalc`): x and y in 1/8 tiles, z in object units.
- Headings: a full turn is 0x10000; objects keep 3 bits (eighths) or, when mobile, 8 bits.
- Speed: `Phys.speed` counts 0x2F to one step of an object's stored speed. The player runs at `Run_FPS` 0x3AC.
- Time: the tick counter `*Time` (sys). `check_physics` moves the world by the time since its last call, at most 0x40 units; animated objects step once per 0x40 units, critters per 16.

## The records

- `struct Phys` (`motion.h`, 40 bytes) is what moves: position, velocity, gravity (`acc[2]`, normally -4), the time to run, heading and speed, bounce (0 to 15), size, the step height `b24`, the terrain byte and the impact (speed lost in collisions, which becomes damage). `PN` is the player's; `CN1`, `CN2` and `CN4` are walking, flying and swimming critters' (chosen in `critter/AI.C`), `CN3` moving objects' and missiles' (`critter/PATHFIND.C`'s `move_me_joe`). All are `MOTION.C`'s globals.
- `struct Handler` (`motion.h`, 12 bytes) says how a mover reacts: collision bits to ignore, bits to pass to its `special` function, and bits that forbid stepping or climbing. `PT` is the player's (special `player_sqhandler`, which stops a slow walker at a ledge); `CT1` to `CT4` go with `CN1` to `CN4` and are set by `critter/PATHFIND.C`'s `init_ai`.
- `struct MotionCalc` (`map.h`, 23 bytes) is the collision record `COLLIDE.C` fills through the pointer `curP`: the floor and highest floor under the footprint, the state bits `hits0` (centre) and `hits1` (corners), the wall and open directions, and the range of `oCollisions` (`struct Collision`, at most 8) that is underfoot or in the way. `Ppd` is the one `do_physics` uses; `can_place`, `obj_deal` and others use one on the stack and restore `curP`.
- `struct MotionParams` (`motion.h`), the single global `MP`, is the stepping state of the move in progress.

## Stepping: do_physics

`do_physics(pp, tp)` (`MOTION.C`) runs a record for its `time`:

1. `space_to_motion` turns heading and speed into an x and y velocity, adds `acc * time`, and sets up a Bresenham walk along the faster of x and y: one 1/8-tile cell per step, the slower axis and z carried as fractions (0x2000 to a cell, 0x800 in z), the number of whole steps, the remainder and the time per step. With vertical velocity, `set_targz` picks the height the mover will stop at: a floor, the ceiling, or the top or bottom of an object.
2. Each `grid_move(1)` takes one step (`flat_move`, or `full_move` with vertical motion, which calls `do_zbounce` on reaching the target height).
3. After each step `check_positions` calls `get_pcoll`, which runs `COLLIDE.C`'s checks at the new cell, lets `OBJPHYS.C`'s `do_objhit` handle each object touched, decides whether the mover may step up or down (by at most `b24`, and onto a solid object only if its handler allows), and returns a word of state bits. The reaction: an object hit that stops the mover or ends the move steps back (`grid_move(-1)`); bits in the handler's mask go to its special function; a wall, a high floor or an object bounces (`do_2dbounce`, `rehead`); nothing underneath starts gravity.
4. After 16 colliding steps the mover is stopped. `back_to_space` writes the cell position back, snapping a walker to the exact height of a slope.

A bounce (`rehead`) reflects or turns the heading off the wall direction `ComputeHeading` found and scales speed by `bounce / 15`, adding the loss to `impact`. Movers with `flags` 0x80 (critters, and the player on foot) slide along walls instead and are stopped by a wall met nearly head on. A vertical bounce (`do_zbounce`) reverses and damps the vertical speed, plays a thud by mass, and settles a slow mover on what is below; something that lands on a non-solid object or in the air is thrown off it (`bounce_that_guy`).

The state bits, read from `COLLIDE.C` and `MOTION.C` (nothing names them): bits 0-1 the floor's terrain class (0 plain, 1 water, 2 lava, 3 ice), 4 on the floor, 8 << class a corner on a floor of that class, 0x80 on a solid object, 0x100 a floor too high to step up, 0x200 a wall, 0x400 an object in the way, 0x800 a drop, 0x1000 nothing underneath, 0x2000 a slope, 0x4000 and 0x8000 an object hit that stops the mover or ends the move. `set_resterr` reduces them to the terrain byte: 1 plain floor, 2 water, 4 lava, 8 ice, 0x10 air, and 0x20 for the player on water with a corner on another floor.

## Collision: COLLIDE.C

`TerrainCheck` reads the 3x3 tiles around the mover (type, height and the floor texture's terrain class, from `TxmTerr`) and tests five points: the centre and the four corners of a square footprint `radius` wide. `GetHgt` gives a point's floor height: 0x80 in a solid tile or the closed half of a diagonal, and rising one unit per cell across a slope. Each point is a wall, too high, a drop or a floor of some class against the mover's z and step range; a corner whose tile's diagonal wall separates it from the centre (`tile_walls`) is a wall. `ComputeHeading` sums the blocked and the open corners into wall and open directions.

`ObjectCheck` walks the object lists of the tiles around and records the objects whose squares overlap the footprint (`obj_coll_check`), skipping the mover, flat static objects and mobile objects marked as having struck something (probably; `do_objhit` sets the mark). `process_objlist` sorts them into those underfoot and those in the way.

The placing functions are used by much of the game: `can_place` tests whether an object of a given type fits at a point and what it would rest on (`nvokHgt`, `nvokTerr`); `drop_around_place` tries up to 24 random spots within a range; `put_at` and `near_mob_put_at` drop an object at a place or at another object, culling it if there is no room.

## Objects: OBJPHYS.C

An object is static (an 8-byte record, `objdata` and above) or mobile (the longer records below it, `critdata`). Only mobile objects keep motion: a heading, a speed and pitch, gravity, a terrain code; mobile non-creatures also keep an exact position in `goal_word`, `attitude_word` and `b0F`, fields a creature uses for its AI. `get_phys_data` and `set_phys_data` convert an object to and from a `struct Phys`; the critter code calls them around each step, with the object's tile in `XP` and `YP`.

`set_phys_data` moves the object between tile lists (running pressure plates), turns `impact` above 0x180 into damage, burns objects on lava, and changes the object's kind: a static object given speed becomes mobile (`static_to_mob`, `mob_init`), and a mobile non-creature that has stopped becomes static (`mob_to_static`, which may break it by its type's fate) and is settled by `obj_deal`. `obj_deal` destroys an object in a wall or another object, splashes it in water, may burn it in lava, and sets it falling if it is over a drop or on the edge of something.

`do_objhit` is what touching an object does: usable objects are used on the mover, triggers fire, a usable mover such as a missile is used on what it hit (`obj/OBJUSE.C` then calls `missile_newhit`, which works out the damage), and anything touchable is pushed (`bounce_obj`, by the ratio of the masses).

## The player: PHYSICS.C and PLAYMOVE.C

The 3D view calls `check_physics` every frame. It measures the elapsed time, adds it to the game clock and calls `move_physics`, which runs `move_player` (`set_player_phys_params`, `do_physics(&PN, &PT)`, `phys_affect_player`), moves the critters and objects (`move_mobile`, in `critter/AI.C`), and updates the screen effects, the footstep and swimming sounds and the player's noise for critters to hear (`set_sound`).

`PlayerInput` is the movement command: 1 forward and turn at the rates the mouse position or the keys set, 8 back, 9 and 10 sideways, 6 and 7 the jumps, 12 and 13 up and down in flight. `parse_playin` reads it from the mouse in the view or from a key binding, `do_player_keyboard` from the keys held. `set_player_phys_params` accelerates towards the wanted speed by at most `MaxPlayerAccel` (lower when carrying more than half the most the player can), blends the motion on ice (`munge_vectors`) and adds currents in water.

The movement state (`newFPS`): on foot, swimming, lava, ice, and in the air levitating, flying or slow falling. It sets the speeds (swimming by the Swimming skill) and the low bits of `player->motion_state`. The motion spells are the bits of `motionbits`: Leap 1, Slow Fall 2, Levitate 4, Water Walk 8, Fly 0x10, Bouncing 0x20 (bit `minor - 1` of spell class 1, set in `game/PLAYDATA.C`; the order is the Guide's).

The step moves (`simple_fizix`, through `player_simple_move`) bypass `do_physics`: a 45 degree turn, or a half-tile step forward or quarter-tile step back tested with `can_place` and made directly.

Falls hurt through `PN.impact`: `phys_affect_player` deals `impact >> 8`, doubled in a fall, cut by an Acrobat skill check, and none in the ninth world.

`get_eye` places the 3D camera: at the player's eye with the bob and shake offsets of `playerMod`, at a fixed position, on another object, behind the player, or spinning in the moongate vortex.

## Data flow

```
check_physics (each frame)
  move_physics
    move_player
      set_player_phys_params   input, ice, currents -> PN
      do_physics(&PN, &PT)     MOTION.C, using COLLIDE.C and do_objhit
      phys_affect_player       PN -> the player object, fall damage
    move_mobile (critter/AI.C)
      get_phys_data -> do_physics(&CNn, &CTn) -> set_phys_data
    parse_effect, set_sound, make_noise
```

## Open questions

- The collision state bits and the terrain byte have no original names; the meanings above are from the code. Bit 0x10 of `player->motion_state` is tested with swimming (0x11) but `newFPS` never sets it.
- `Phys.b1D`, `MotionParams.f21` and `Handler.w6` (read only by critter path finding) have no known meaning.
- `player_newsq` sets `player->automap` from a nibble of `PlayersMap`; what either means is not known.
- `do_filanium` advances the djinn capture when an object carrying spell 0xD/3 lands in water on floor texture 0xC1. The name suggests the filanium step of that quest; string 0x14C has not been checked.
- `player_setup` with any `how` other than -1 puts the player near the ceiling to fall; which callers want that has not been traced.
- `missile_newhit` scales damage by the Missile skill only when the missile's `ammo` field is -64; what that value marks is not known.
