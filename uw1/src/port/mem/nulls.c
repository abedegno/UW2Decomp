/* nulls.c: replaces nothing; UW1's port_game_nulls for the paragraph map (Exhume's
   runtime/port/mem/parmap.c): what DOS reads through a null pointer, written into the port's
   copies of DS:0 (which start as DGROUP's image) and of the interrupt vector table. */
#include <string.h>
#include "port.h"

void port_game_nulls(unsigned char *null_near, unsigned char *null_far)
{
    /* DS:0..3, the tail of the last overlay stub entry while its overlay is not loaded:
       3F F1 02 00 in UW.EXE (exhume.toml's [replay.nulls] ds_forms; DGROUP's image has it) */
    null_near[0] = 0x3F; null_near[1] = 0xF1; null_near[2] = 0x02; null_near[3] = 0x00;
    /* int 1 to 7 as DOSBox sets them up (js-dos, where the replays are recorded; UW2Decomp's
       nulls.c has the same set-up's values) */
    memcpy(null_far + 4, "\xf4\x00\x70\x00\xf4\x00\x70\x00\xf4\x00\x70\x00\xf4\x00\x70\x00"
                         "\x54\xff\x00\xf0\x60\x10\x00\xf0\x60\x10\x00\xf0", 28);
    /* int 0 -> int0_trap: 1DEA:000E in the link's map, plus the load segment */
    null_far[0] = 0x0E; null_far[1] = 0x00;
    null_far[2] = (unsigned char)((0x1DEA + PORT_LOAD_SEG) & 0xFF);
    null_far[3] = (unsigned char)((0x1DEA + PORT_LOAD_SEG) >> 8);
}
