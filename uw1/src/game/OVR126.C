/* target: ovr126 */
/* opts: -mm -1 -G -O -Y -d */
/* The credits (the main menu's Acknowledgements): plays cutscene 10, the credits, then
   marks every change bit but bit 0 so the next pass of do_changes redraws the current
   screen. Byte for byte UW2's show_credits (UW2Decomp src/game/CREDITS.C).
   File name: provisional (the original name is unknown; named after the segment; UW2's
   copy is CREDITS.C).
   Data: none.
   name: show_credits, UW2's (FM Towns) name for the same function. */

#include "gfx.h"
#include "sys.h"


void far show_credits(void)
{
    runcutscene(10);
    editchng(0x7FFE);
}
