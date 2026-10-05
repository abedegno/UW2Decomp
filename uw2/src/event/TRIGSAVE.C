/* target: ovr162 */
/* opts: -mm -1 -G -O -Y -d */

/* Writes the trigger table, TRIGGER.C's Triggers[16], to an open save file. The 16
   bytes are written raw with fwrite; nothing in this file reads them back.
   Entry point: ovr162_0 (no caller in the matched C sources: it is reached only through
   its overlay stub, probably from the save code).
   Data: none of its own.
   Name: descriptive (writes the trigger table). */

#include <stdio.h>
#include "event.h"


/* name: FM Towns trap_save_ is the same size, but nothing else ties the two, so the
   IDA name is kept. */
void far ovr162_0(FILE *fd)
{
    fwrite(Triggers, 1, 16, fd);
}
