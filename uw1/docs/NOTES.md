# Notes from matching, for the headers and the map

Things agents found that belong to the shared headers or the map, to settle in the readability pass.

## Headers (UW1 differs from UW2's src/include)

- `dbg_printf(char *fmt, ...)` (ovr106_1B): UW1's debug print, called about 25 times with format strings; INPUT.C declares it locally. The name is descriptive.
- view3d.h: `SpecShadeMode` is a signed char in UW1 (`cbw`), unsigned char in UW2 (VIEW3D.C declares it locally).

## Map

- ovr106 is probably UW1's debug module (init_debug at ovr106_0 and two empty functions). kin named both ovr106_1B and ovr106_20 `mous_3d_show` after an empty UW2 function; ovr106_1B is `dbg_printf` (above). Whoever matches ovr106 settles the names.
- seg031's last table row covers two functions (IDA made no procedure of the second); VIEW3D.C makes the second `static`.
