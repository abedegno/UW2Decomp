/* pgcache.c: replaces the data of src/3d/PGCACHE.ASM that C reaches: the far pointers grs_off
   (the picture slots' offsets, 4723:B10F) and obj_tab (4723:B90F), into seg051, seg004's
   data (PGCACHE.ASM's _DATA, DS:23A4 and 23A8). The page cache's code is the 3D renderer's,
   a later milestone. */
#include "compat.h"
#include "view3d.h"
#include "port.h"

uint16 *grs_off = (uint16 *)(seg051 + 0xB10F);
uint16 *obj_tab = (uint16 *)(seg051 + 0xB90F);
