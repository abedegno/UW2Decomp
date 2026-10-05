/* nulls.c: replaces nothing; UW2's port_game_nulls for the paragraph map (Exhume's
   runtime/port/mem/parmap.c): what DOS reads through a null pointer, written into the port's
   copies of DS:0 (which start as DGROUP's image) and of the interrupt vector table
   (docs/PORT.md, "Null pointers", has the table). */
#include <string.h>
#include "port.h"

void port_game_nulls(unsigned char *null_near, unsigned char *null_far)
{
    /* DS:0..2, the tail of ovr167's last overlay stub entry while ovr167 is not loaded */
    null_near[0] = 0x27; null_near[1] = 0x06; null_near[2] = 0x00; null_near[3] = 0x00;
    /* int 1 to 7 as DOSBox (js-dos, where the replays are recorded) sets them up and the game
       leaves them: a null object pointer reads them as the object's fields after int 0's
       (OBJUSE.C's checkTrap reads the link word at 0000:0006, int 1's segment, 0070h) */
    memcpy(null_far + 4, "\xf4\x00\x70\x00\xf4\x00\x70\x00\xf4\x00\x70\x00\xf4\x00\x70\x00"
                         "\x54\xff\x00\xf0\x60\x10\x00\xf0\x60\x10\x00\xf0", 28);
    /* int 0 -> int0_trap: offset 0019h, segment 1FC7h plus the load segment */
    null_far[0] = 0x19; null_far[1] = 0x00;
    null_far[2] = (unsigned char)((0x1FC7 + PORT_LOAD_SEG) & 0xFF);
    null_far[3] = (unsigned char)((0x1FC7 + PORT_LOAD_SEG) >> 8);
}
