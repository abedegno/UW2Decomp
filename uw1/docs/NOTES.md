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
