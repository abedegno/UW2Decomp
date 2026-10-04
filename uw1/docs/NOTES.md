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

## From wave 2 (C)

- Renamed for the stub order (bssorder keys): ovr106_1B `dbg_printf` → `dprintf` (key 452 between init_debug 145 and ovr106_20 575); ovr114's font loader `grfx_quikfont` → `grfx_load_font` (UW2's name, key 343, between fill_FB 342 and fadein 558). BAGS.C and MAINMENU.C still cast its argument to UW2's int: tidy in the readability pass.
- kin.py now takes UW2 names only where UW2's map anchored or confirmed them (it had passed on rejected candidates: free_speech_stuff for the console printer seg015_1F9B_E8, free_timers for _ld_close ovr115_1AB, grs_plot, destroy/create/change_sprite, grfx_init for ovr113_2A2).
- gfx.h: fadein/fadeout(src, count); gronk_gr, load_tr_ems return char; font loader (char *name) returning unsigned char; fade_buffer extern far (seg050, 0x3000 bytes). sys.h: ws_active char; EMS functions seg012_B, _A6, _BB (char), _10F (char), _12C; console printer seg015_1F9B_E8(char far *); seg042_19B; seg019_755, _791, _CB7, _CEA. file.h: copy_file(src, dst); LoadBitMap_ovr141_0(int pal, char *name) (UW1's display_screen); GetLevel (ovr140_92), do_level_hacks (ovr140_906); ovr113_0(int) (probably preload_cr). gfx/view3d: seg009_1 and seg009_6D for UW2's seg009_7 and seg009_73; seg015_1F9B_325, seg003_581E; far externs in seg051 (C375..C378, C3EC; C4D7 is UW2's seg052_519C_E4D4); dseg_5c99_10C (UW2 dseg_67d6_120), dseg_5c99_7178. LOADGR's tmpoffs is a near uint32 *.
- UW1 has UW2's SCROLL.C and SCROLLIO.C in one segment (seg043). valloc is called at seg001:004A (functions.tsv says seg001_5A).
- Check for WHOLE SEGMENT MATCHES, not only per-function MATCHes: EMS.C matched every function in UW2's order but not the segment (UW1 has the four-page mapper first).

## From wave 3 (A, combat)

- UW1's player record, more: moonstone:4 at 0x5E (unsigned char bitfield); word 0x5F: drawn bit 1, poison bits 2-5, active_spells bits 6-9; byte 0x60: nrunes bits 2-3, Armageddon flag bit 4; byte 0x61: shrooms bits 2-3, drunk bits 4-9 of word 0x61; easy 0xB4. Local structs Player1Combat, Player1Runes, Player1Spells, Player1Time, Player1Swim.
- combat.h: `char push_missile(proj, src)` (no launch); `char hit_critter_goal(goal, attitude, npc, x, y)` (no gtarg); `gronk_whoami(int, char all, ...)`; `struct Spell far spells[53]` (defined in SPELLS.C, the listing's seg064); `char resolve_attack(void)`; `do_damage(int type)`. object.h: decode_obj_spell's flag plain char. map.h: can_place returns char. player.h: player_setup is called with three arguments (0, 0, 0) in SPELLS' gate travel, against PHYSICS.C's two-argument definition: settle at the link.
- Externs new to UW1: Bullfrog_ovr107_EA1 (no UW2 relative), cast_trap_spell = ovr153_110A, do_teleport = ovr107_949, do_mstone = ovr143_15F4, DetectedTrap = ovr143_1829, RemoveTrap (j_DefuseTrap stub), LookAt = ovr122_0, creature_obj_init = ovr101_5E (stub101_2A), pretty_panelagain = seg036_1A25, print_path_to and mpos (ovr154), home_cam and attach_eye (ovr134), clearobj (ovr109).
- DGROUP ownership: DS:234-235 COLCYCLE; COMBAT's _DATA from 236; SPELLS owns inanmMapX/Y at DS:3636 (provisional; 3630-3635 unreferenced); DS:9D9-9DF after SPELLS' sq_type belong to the next file. UW1's do_miss stores the attacker's item into the global fromwho (UW2: a local).
- SPELLS.C and PLAYTIME.C are resident in UW1 but compile with -Y (UW2's overlay opts) and match.

## From wave 3 (B, conversation and events)

- Renamed for the stub order: ovr107_EA1 `Bullfrog_ovr107_EA1` (SPELLS.C's extern) → `work_bullfrog_tiles` (key 863; the EXE has it 19th). WORLDEV.C's other new names (emerald_trap, talking_door_trap, do_arial_talking) were chosen for the stub order too.
- A real UW1 bug: BABL.C's bab_free checks the tag against the block's header rather than its data, so it never frees anything.
- GRDB.C (seg045) compiles with `-mm -Y -d`, no -1, -G or -O (the bytes show `mov ax,0FFECh; push ax`, `pop cx`, `mov sp,bp`, `jmp $+2`). Its table hides grdb_size (0xB8), gr_getpre (0x221), gr_getlab (0x240), and stops short of gr_freelab (0x332..0x37C); its reset_db row is gr_tostrt (kin false hit).
- Declarations: ovr091 (archive): `char ovr091_0(char far *arc, char *name)`, `char ovr091_153(char far *arc)`, `int ovr091_61A(char far *arc, int block, void far *buf)`, `int ovr091_798(char *file, int block)`. ui: seg039_3495_85A/89D (UW2's seg039_3452_857/89A). gfx: `seg015_1F9B_25A(int x, int y, int color)` plots a pixel. inv: `char EncumCheck(obj)`, `char invRemoveObject(obj, int)`. conv: `char move_convpic(...)`; UseTradeSlot, total_offering, nothing_there take signed `char *` selections; nothing_there returns char. event: `char find_good_x_and_y(...)`, `char death_check(obj, char mode)`. Other: `seg024_24DC_D0A(obj, int)` (resident wrapper calling LookAt), `ovr143_48D(int)`, `seg014_1DC5_15C5(void)`, `extern int16 dseg_5c99_720C[]`.
- Player record: 0x29 lore, 0x30 charisma, 0x33 appraise, 0x36 maxhealth, 0x3D level; 0x64 body bits 2-4. Items: coin 0xA1, fish 0xB6.
- Inferred names (not proven): change_music_maybe (seg014 160D), update_sprites (0000:0330), copy_visible_to_hidden (seg003_5350), uhline (5418), grPageFlip (4F2A).
- Compiler: without -Z, a plain int local takes DI when one explicit register takes SI (`int len; register int id;`).

## From wave 3 (C, game)

- Renamed for the stub order: ovr143_48D → `mantra_advance` (key 5; first in ovr143's stub table). SKILLS.C's other new names were chosen by key: report_advance, great_advance, check_victory, plant_seed, do_resurrect; PLAYDATA's set_maze (the maze spell on level 7).
- targets/ovr143.tsv now runs to 0x1B01 with RemoveTrap at 0x18CD (targets.py had dropped it with IDA's stray `DefuseTrap_sub_8E27D`).
- UW1's player record is 0xD2 bytes (PlayerDat). More fields: 0x60 bits 4-7 (orb 5, key 6, cup 7); 0x62 bit 3 Garamon buried, bit 4 maze; 0x63 light; 0x64 pclass; 0x6E dreams; 0xB0 saved max mana; 0xB5 sound, music, detail; 0xB6 terrain; 0xC2 lore[8].
- sound.h: fx_is_on, music_is_on return char. gfx.h: read_quikpal, grfx_init return char; LoadBitMap_ovr141_0(int, char *) returns char. file.h: clear_dir, init_save, blttodrive, SaveGame, SaveLevel(int) return char (MAINMENU.C declares clear_dir unsigned: tidy). sys.h: init_world calls init_mem(2), TMPALLOC.C defines it void: settle. No HomeDir variable in UW1 (GAMEWRAP uses the literal "SAVE0\\"). New globals ProbablyAutomapEnabled_dseg_5c99_546 (char), EndGameMode_dseg_1C8F; swap_tmap(unsigned char), set_maze(char), check_victory; dream, set_sklmnu return char. In UW1 bltfromdrive is the character-creation background loader; advance() calls ovr145_4FB where UW2 calls panel_check.
- Kin false hit: ovr105_14E3 is not reset_db (VIEW3D's reset_db is 2ABA:0246).

## From wave 4 (C, 3D and UI)

- seg017_1FDD starts with SetPnt (0x35 bytes, assembly, kin same as UW2's SETPNT.ASM): its own table targets/seg017_1FDD_1.tsv, as UW2's seg019_21BA_C; GRIDDB.C's table starts at org 0x36.
- GRIDDB.C defines `ProbablyAutomapEnabled_dseg_5c99_546` (DS:546, char 1, UW1's automap switch).
- Headers should take: `SpecShadeMode` signed char; `flat_case` char; `color_to_map`/`color_to_obj` of 192 entries and no `TxmCol`; `unsigned far seg009_38C(int)`; `extern unsigned char dseg_5c99_12B6` (data no matched file owns yet, value 0x64). GRIDDB.C uses `#define` renames against view3d.h for now.
- AUTOMAP.C: the stub order gave ovr092's three called entries UW2's names, SaveAutoMapLevel, GetAutoMapLevel and ClearAutoMap (GAMEWRAP.C follows, renaming ui.h's out of the way). UW1's signatures: Save/GetAutoMapLevel(struct Arc *arcp, int lev) return unsigned char, arcp 0 meaning open "SAVE0\lev.ark"; GetTheWords(int) returns void; struct Arc is 11 bytes. Headers should also take: notes_dirty char; swap_ws_out/unswap_ws (CRPAGES) as UW1's workspace; `void far input_del(int)`; `void far ovr154_2D1(int, int)`; the ovr091 archive prototypes; grfx_load_font(char *) returning unsigned char; seg015_1F9B_25A as the pixel plot.

## From wave 4 (B, objects)

- seg040_352B is UW2's OBJUSE.C and USEITEMS.C in one segment (src/obj/USEITEMS.C, `-Y`: it pushes its own functions' segments). New in UW1: SpikeDoor (two), a gronk_whoami callback halving hit points, TybalsOrb (three arguments, the third unused), item E7h used on object 16Eh.
- Headers should take: `how` a plain char in the Use* handlers; CloseDoor(door) alone; decode_obj_spell's flag a plain char; `void useNSpellCharges(obj)`; `char always_decode`; `extern char ObjectActorArg`; `int add_animobj(int, int, char, char x, char y)`; `char player_eat(int)`; TalkTo, ExplodingBook_ovr107_1259, `char plant_seed(void)`, UseBonesOn, UsePoleOn, UseAnvilOn, UseOilOn, UseRockHammerOn. Player record bits at 5E-62, B0 and CE (unsigned game_clock): USEITEMS.C's local struct Player1Use.
- To settle at the link: USEITEMS.C calls mantra_advance with one argument (0); SKILLS.C defines it with none.
- INTERACT.C (seg024_24DC) is UW2's GAMESCR.C and INTERACT.C in one segment: GAMESCR's pull_chain..clear_gamedisp first, check_save/check_rest last; UW2's check_around is absent. New: new_IconSelect/new_IconUnselect (icon pictures 0x200B/0x200A - 2*(mode-1)) and the handle iconsMshandle (DS:2694). The table's mous_in_inv row hid mous_in_panel (public, at +0x131D): split.
- Headers should take: UsingPole char; IsMobElem, EncumCheck and BlockingTerrain return char; TERRAIN the whole TxmTerr word; ovr130_0(int), ovr130_6D6(int x, int y), stop_music, seg027_2861_EF9(void), EtherealVoidSpecialEffects_seg008_150(void); `extern unsigned char dseg_5c99_5626`, `realDScheck`; `extern int16 floor_terrainrelated_dseg_5c99_717C[]` (read at [txt - 0x30]). Player record fields: INTERACT.C's local struct Player1Scr.

## From wave 5 (A, sound and utilities)

- seg014_1DC5's table started 0x12 bytes late: cllbck_tst, the 256 Hz timer callback init_timers registers (push seg seg014; push offset 0Bh), sits at +0xB outside every IDA proc. The table now starts there (org 0xB). Its row at +0x754 is fx_is_on. Its cnames are still mostly listing names: SOUND.C's names to copy in at the readability pass.
- Renamed by the stub order: ovr154_2D1 do_beep, ovr154_374 memcheck, ovr142_11A init_lighting, ovr123_76 Map_Load, ovr123_16E Map_Save (callers AUTOMAP, UWEDIT, GAMEWRAP follow; GAMEWRAP renames map.h's UW2 Map_Load/Map_Save out of the way). New: dbg_break (int86 2), check_dirs, xorread, xorwrite; set_light, random_light.
- Headers should take: plain char returns of music_is_on, fx_is_on, init_timbres, install_timbre, read_file_to_mbuf, read_fx_data, init_fx, music_over, init_speech, and bltfromdrive as its callers use it; load_sound_driver(name) alone; turn_music and turn_fx take a char; UW1's Map_Load/Map_Save(archive record, level). SOUND.C includes neither sound.h nor file.h for now.

## From wave 5 (B, cutscenes)

- CUTS.C (ovr105): the stub order renamed six entries from UW2's names: get_cut_banks (was get_cuts_ems), get_cuts_block (set_cuts_ems), free_block (ovr105_3AB), runcutscene (show_cutscene), init_cutscene (ovr105_14E3), cuts_skipline (ovr105_1639; fgets(line, 99, fp)); value_cuts is PANELS.C's writeCutsValue. Renamed in every caller and in gfx.h. New for their keys: anm_lptab, read_anim_hdr, read_lp_inc, do_sound, cutsop_stop (UW2's cutsop_next). UW1 has 16 cutscene opcodes.
- TMPALLOC.C's conv_ws is public (CUTS.C's init_cutscene reads it); the bytes are unchanged.
- Headers should take: UW1's CutsState, 0x4A bytes, no repeat43, speech43/fade45/fade47 at 0x43/45/47, flags at 0x49, bit 0 "drawing"; fadein/fadeout(src, count); speech_available returns char; seg002_A(src, dst), the delta decoder.

## From wave 4 (C, panels)

- PANELS.C (seg036_3087): adjust_eyes's row hid the publics adr_weapon (+0x12C6) and move_weapon (+0x131E): split. New in UW1: two animated dragons (adjust_dragons, elements 4 and 5); the right panel turning over in a pseudo-3D squeeze from three EMS handles (init_panelflip, do_panel_frame, flip_scale, flip_column), plain redraws without them; weapons.dat holds positions only; player_look_shaft and player_look_grave call the cutscene module (value_cuts).
- BSS layout, tested: uninitialised function-level statics come first in _BSS, in definition order and not word-aligned (update_screen's old_time at DS:359A, adjust_dragons' dcount[2] at the odd DS:359B); file-scope names follow from 35A0.
- Matching tricks: jiggle_weapon's case 1 as if/else, case 2 a ternary with no break, wfo = 1 in every case; an empty `case 1: case 2:` gives adjust_weapon its jump table; `ok &= (char)read_gr_far(...)` gives a byte `and`.
- Headers should take: `extern unsigned char far seg051_C377` (UW2's scrgr_fpage); EMS.C's seg012_10F/141/15E/1B1; init_panelflip(panel, x, y, w, h); RightPanel unsigned char; `int16 dseg_5c99_720C[]`; UW1 player record: shelf 0x47, lefty bit 0 and body bits 2-4 of 0x64, detail bits 4-7 of 0xB5.
- TEXTMAPS.C (ovr131): the stub order renamed init_txtlib (ovr131_0), load_tr_mem (LoadTextureFile), Txm_Load (ApplyTerrainData), Txm_Save (ovr131_2E2); its BSS (DS:717A..726D) names chosen by key: f16p, floor_IDs (was floor_terrainrelated_dseg_5c99_717C), f32_buf, floor_num, w16_buf, w64_types, w64_buf, w64_num; TxmTerr and TxmID keep theirs. Callers follow. map.h now has UW1's Txm_Load(char *arc, int lev) (unsigned char), Txm_Save (char) and Load_Terrains(walls, floors); all 85 matched files rebuilt and verified after the change. Still to take: TxmID[0x30], TxmTerr[10], the far seg051 tables C378/C3B2 (bytes) and C3EC/C460 (words); errors go through pfatal_code.

## From assembly wave 1

- seg000 (SPRITE.ASM) and seg015_1F9B (MODEX.ASM) are assembly, as in UW2 (map/files.tsv says C). SPRITE is UW2's module byte for byte; its table starts at the sp_dirty word (+0). MODEX's mem_set has no odd-byte store after `rep stosw` (an odd count leaves the last byte).
- seg020 (AIL.ASM) is an earlier AIL 2 release, version word 0CAh (UW2 0D3h): the int 8 vector through DOS (int 21h 35h/25h), register_timer does not set the period to -1, release_timer_handle has no -1 check, init_driver no null check after find_proc, no format_sound_buffer or format_VOC_file stubs. Its table ran into seg021 (+0CB5h).
- seg046 (OVERLAY.ASM): the same five OVERLAY.LIB modules as UW2, at +0Ch (UW2 +5); __SEGTABEND__ 6A8h (13 fewer segment table entries). The table had run into padding and seg047's data.
- seg001 (VALLOC.ASM): UW2's code at the same offsets; only far data moves (seg055's records 8 bytes later, seg048:4106h/4108h). valloc is the C entry at +4A.
- Not yet in symbols.tsv: _dseg_5c99_2404, _seg001_5A, _seg003_522E, seg048, seg055, sp_inf_tab (5477:0008).
- ARC.C (ovr091): UW2's names by the stub order, open_arc, close_arc, put_arc, get_arc, check_arc (count_arc already); callers follow. check_arc returns 1 or 0 for whether the block is there, -1 if the file cannot be read. file.h's UW2 archive prototypes are removed until the readability pass gives UW1's: struct Arc, 11 bytes (fd 0, tmpfd 2, count 4, uint32 far *offtab 6, unsigned char dirty 0xA); open_arc(struct Arc far *, char *name), close_arc(struct Arc far *), put_arc(arc, blk, void far *buf, len) (unsigned char), get_arc(arc, blk, buf) (int, no return statement), check_arc(char *name, blk), count_arc(char *name). All 86 matched files re-verified after the change.
- CONVVARS.C (ovr103): UW2's setup_converse_data and update_converse_data, unchanged but for the player record (drawn and poison in the word at 0x5F, female bit 1 of 0x64, game_clock 0xCE).
- seg005_10B2 (15440 bytes, "mixed") is Borland's C runtime library (CM.LIB), as UW2's seg005_105F: no source, taken from the library at the link (UW2Decomp docs/LINKING.md). It is not matching work; the link stage accounts for it.
