/* pgcache.c: replaces the data of src/3d/PGCACHE.ASM that C reaches: the far pointers grs_off
   (the picture slots' offsets, 4FAF:D049) and obj_tab (4FAF:D849), into seg004's data. The
   page cache's code (do_fetchmap, do_mouseq) is the 3D renderer's, a later milestone. */
#include "compat.h"
#include "view3d.h"
#include "port.h"

uint16 *grs_off = (uint16 *)(seg052_519C + 0xD049);
uint16 *obj_tab = (uint16 *)(seg052_519C + 0xD849);
