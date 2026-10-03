# Notes from matching, for the headers and the map

Things agents found that belong to the shared headers or the map, to settle in the readability pass.

## Headers (UW1 differs from UW2's src/include)

- `dbg_printf(char *fmt, ...)` (ovr106_1B): UW1's debug print, called about 25 times with format strings; INPUT.C declares it locally. The name is descriptive.
- view3d.h: `SpecShadeMode` is a signed char in UW1 (`cbw`), unsigned char in UW2 (VIEW3D.C declares it locally).

## Map

- ovr106 is probably UW1's debug module (init_debug at ovr106_0 and two empty functions). kin named both ovr106_1B and ovr106_20 `mous_3d_show` after an empty UW2 function; ovr106_1B is `dbg_printf` (above). Whoever matches ovr106 settles the names.
- seg031's last table row covers two functions (IDA made no procedure of the second); VIEW3D.C makes the second `static`.

## From wave 1 (B and C)

- player.h: UW1's `struct Player` differs; `lefty` is bit 0 of byte 0x64 (UW2: 0x65). INVDATA.C reads it through a local `struct UW1PlayerHand`. `weight` (+0x4A) and `max_weight` (+0x4C) are as in UW2; PlayerDat is DS:7288.
- object.h: `Obj_Elem_Fate` returns `char`; inventory.h/ovr: `ObjWorn` is tested signed (char) in UW1.
- map.h: `ObjectCheck(unsigned char flat, char useflag)`; `can_place`'s `flier` is `char`. UW1's terrain class: `(TxmTerr[floor] & 0xFF) << 4` (UW2 `& TERR_CLASS) << 2`): check TERRAIN.DAT's bits.
- file.h: SCRSHOT's GIF helpers are `ovr112_194(int fd, char size)`, `ovr112_2A3(int fd, int bits)` (UW2's ovr116_*).
- Not in any header: COLLIDE.C's `GetHgt`, `SolvePnt`, `SolveCenter`.
- The three `return 0` overlays ovr139, ovr144, ovr146 are `rect_class_data`, `spec_class_data` and `stuff_class_data` (OBJCLASS.C's class table calls their stubs).

## Map and tools

- kin.py named short bodies (empty functions, `return 0`) after arbitrary UW2 twins (`mous_3d_show`, `make_stew`, `unbound`, `pfatal`); fixed in Exhume: only unique, six-instruction-or-longer pairings pass a name. Tables regenerated for the segments no agent held.
- verify.py does not check the order of overlay stub entries (the publics' order in the stub table, which bssorder's key decides); agent C checked it with a script. To add to Exhume.
- symbols.tsv grows with merges; extern names in early files were checked only for consistency, and are checked properly at the link.

## From wave 2 (A)

- **UW1's `struct Player`** (from the bytes; CRITTIME, BAGS, BABLHACK and PLAYMOVE each declare a local struct and a PLAYER1 macro until player.h is reconciled): skills 0x21; moonstone (4-bit) 0x5E; drawn (word bitfield, bit 1) 0x5F; lefty bit 0, female bit 1 of 0x64; quests (int32) 0x65; quest_bytes[4] 0x69; b6D 0x6D; game_vars[0x40] 0x70; motion_state 0xB8; swim_count 0xB9; crithit, typehit, crithittime, hitx, hity 0xBA..0xC1; game_clock 0xCE; weight 0x4A, max_weight 0x4C.
- Signed `char` in UW1 where UW2's headers say `unsigned char`: returns of space_to_motion, IsMobElem, drop_around_place, Obj_ListOkay, the Obj_Check callback, set_gridx_and_y_based_on_tile_type; globals TimeStop, InvUpArrow/InvDownArrow, KeybUsed; parameters move_physics's easy, fix_name_string's article.
- Signatures: Obj_Rem returns nothing; CloseDoor(door) takes one argument; grfx_quikfont takes a file name (char *); clear_paths is in seg006 (PATHFIND) in UW1.
- kin false hit: `free_scrgr` at seg036, seg039, ovr105 and ovr109 (a one-call body).
- Table rows that hide a second function: seg027 A78 (free_critter + AC4), seg027 EEE (EF9 + F23), seg030 899 (flat_move + UW2's static A3F).
- Inferred, not proven, names: animcount (DS:3656), MoveCamera (DS:0762), nextstep/watertime/water_eff (DS:0779/077F/077E), errmsg (stub 5AA4:0075).

## From wave 2 (B)

- More of UW1's `struct Player`: fps bits 0-2 of byte 0xB6; word 0x62 bit 2 (talisman_ok, mob_to_static); 0x6D talismans left; word 0x6E (bit 3). Bitfield runs declared `uint16` take one byte where only low bits are used. Files keep local structs (CRITTIME Player1Hit, BAGS Player1Bags, BABLHACK Player1Conv, PLAYMOVE Player1Move, INVDATA UW1PlayerHand, PHYSICS UW1PlayerMotion, OBJPHYS UW1PlayerQuest) until player.h is reconciled: they type overlapping bytes differently (BABLHACK's skills[] spans 0x21..0x63).
- map.h: `int far Anim_Load(char *name, int level)`, `char far Anim_Save(char *name, int level)` live in EFFECT.C in UW1 (UW2: MAP.C). player.h: `player_setup(int x, int y)`. critter.h: control, aligned, didmove signed; add_to_beeline_path and set_next_square_on_path return char; close_to_square(char flag, ...); no set_htx. mts_doanim returns char; Phys.light signed.
- ovr091 (unmatched, LEV.ARK access): ovr091_61A(char far *, int, void far *) returns int; ovr091_22C(char far *, int, void far *, int) returns char.
- Two-function table rows: seg006 367 (get_terrain + crit_hndlr_walk), 5FE (crit_hndlr_fly + crit_hndlr_swim), seg044 E9 (+ UW2's seg044_368F_392, static), SpawnClass7Object (+ CreateAnimoForSrcObject, static), phys_affect_player (+ player_sqhandler, public).
- Spurious kin: DoTrapScreenShake ↔ NotASpellMessage; UW2's `unreferenced_seg008_1B09_160` is called in UW1 (EtherealVoidSpecialEffects_seg008_150).
