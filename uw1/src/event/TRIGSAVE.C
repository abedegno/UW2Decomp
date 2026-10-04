/* target: ovr149 */
/* opts: -mm -1 -G -O -Y -d */

/* Writes the trigger table, Triggers[16] (DS:737E), to an open save file. The 16 bytes
   are written raw with fwrite; ovr153_0, its twin, reads them back with fread.
   Entry point: trap_save (reached only through its overlay stub, from the save code).
   Data: none of its own.
   Name: descriptive (writes the trigger table). UW1's DOS overlay ovr149; UW2's ovr162. */

#include <stdio.h>
#include "event.h"

/* name: the target table's name: FM Towns trap_save_ is UW2's twin of this function (the
   same size and shape), and UW1's ovr153_0, the matching fread of the same 16 bytes, is
   the load side. */
void far trap_save(FILE *fd)
{
    fwrite(Triggers, 1, 16, fd);
}
