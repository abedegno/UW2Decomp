# The 3D renderer: candidates for the findings page

Candidates from `src/3d` for the project's findings page; [3d.md](3d.md) describes the subsystem. Each bug candidate was re-read in the matched source. What the code does is read from the code; the effect in the game is an inference unless it says it was checked.

## Likely bugs

### `get_dist` adds the y term's sign correction to the wrong register

- **What happens:** `get_dist` takes absolute values of three 32-bit coordinates by `cwd; xor low,dx; xor high,dx; neg dx; add low,dx; adc high,0`. For the y term the `xor` goes to SI but the `add` to CX, which holds the other term already chosen: a negative y comes out one too small and CX one too large.
- **Where:** `get_dist` in [3d/SPHERE.ASM](../../src/3d/SPHERE.ASM).
- **Evidence:** code reading (UW2Decomp's comment on its copy notes the same slip). UW2 never calls `get_dist`; UW1's object headers `seg004_15AC` and `seg004_17AB` do, so in UW1 it is live.
- **Confidence:** likely.
- **Effect:** a distance estimate off by a unit or two for objects on one side of the eye; probably invisible. Not checked in play.
- **For a port:** matching DOS means keeping the off-by-one.

### `do_compact_map` uses the address of the mapper word as the mapper

- **What happens:** opcode 0D2h stores the constant 0B00Ah, the address of the g-map context's routine word, as the texture mapper's offset (`mov word ptr ds:[0B006h],0B00Ah`), where UW2's handler loads the word itself; it then reads a bitmap slot operand like `do_compact_tmap`, where UW2's takes the context's block.
- **Where:** `do_compact_map` in [3d/TMAPOPS.ASM](../../src/3d/TMAPOPS.ASM).
- **Evidence:** code reading against UW2Decomp's `do_compact_map`.
- **Confidence:** likely (as a slip); no effect known.
- **Effect:** a program with opcode 0D2h would call seg003 at offset 0B00Ah, inside SCALEBM.ASM's code buffer. The C never emits 0D2h; whether any 3D model's bytecode does was not checked.

## Possible bugs

### Camera type 2 writes its view one word off what type 4 reads

- **What happens:** camera type 2 (`seg004_E02`) builds a type 4 view description at seg051:00C8h as the eye, the word 1, the view matrix and the zoom 7ED2h, and makes it current. Camera type 4 (`seg004_CF4`) reads a description as the eye, the zoom, a flag and the matrix, so it would take the zoom from the 1 and the matrix one word late.
- **Where:** [3d/CAMERA.ASM](../../src/3d/CAMERA.ASM).
- **Evidence:** code reading.
- **Confidence:** possible, and probably unreachable: the current view description (seg051:104h) is 0 in the EXE, which selects `head_view` (type 0), and only seg004's own far entries (`seg004_A10` .. `seg004_11DD`), which nothing in the EXE calls, would change it. See the dead code below.

### More than 191 objects in a pick frame reuse pick colours

- **What happens:** `do_obj` gives each object drawn in a pick frame the next colour from 1 and wraps `PickUp` back to 1 at `PICK_WALL` (0xC0), overwriting `color_to_obj` and `color_to_map` entries.
- **Where:** `do_obj` in [3d/DRAWOBJ.C](../../src/3d/DRAWOBJ.C).
- **Evidence:** code reading.
- **Confidence:** possible (a deliberate limit).
- **Effect:** with that many objects in view, a click can pick a different object drawn in the same colour. Probably never reached.

## Engine findings

- **One camera.** `create_matrix` dispatches on the current view description's type through six camera routines and can blend from one view to the next (UW1 only), but nothing outside seg004 ever changes the description from type 0, so the game always uses `head_view`, the player's eyes. The other five types, the blend and the far entries are probably left from a camera system the shipped game does not use.
- **Self-modifying code.** `render_3d` patches the screen centre into immediates in SMOOTH.ASM, INTERP.ASM and NOCLIP.ASM; `self_modify` patches the eye and shift into the point loaders; `check_flat` copies one of two `mxmul` bodies over `mxmul`; seg003's polygon clippers patch their comparison opcodes. A port has to do these with variables.
- **No 386 code.** Every seg004 module assembles under `.186`, where UW2's need `.386`.
- **Debug traps.** Unused opcode slots run `do_int2` (`int 2`); the slot for 0DAh (`do_uwsetup` in UW2) points at an `int 2` after `smooth_over`; `seg004_4980` is an `int 3` handler; PGCACHE.ASM has `int 2` and `int 3` on its error paths.

## Dead code

- `NOCLIP.ASM`: the whole second handler set for opcodes 78h to 9Ch. `seg004_5040` would swap it in, but the `xor` that clears the flag at 286Eh leaves ZF set, so it only ever copies the normal set.
- `CAMERA.ASM`: camera types 2, 4, 6, 8 and 0Ah (`seg004_E02`, `CF4`, `E87`, `D2D`, `CAB`), the view blend in `create_matrix`, and the far entries `seg004_A10`, `A1E`, `A32` and `11DD` .. `1245`, as above.
- `VIEW3D.C`'s `seg031_189` and static `seg031_193`: no callers (probably debugging toggles: one sets `quad`, the other toggles `SpecShadeMode`).
