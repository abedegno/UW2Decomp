/* target: ovr131 */
/* opts: -mm -1 -G -O -Y -d */
/* The credits: the end-credits cutscene, then back to the game screen. */

#include "sys.h"

void far show_cutscene(int n);

void far show_credits(void)
{
    show_cutscene(10);
    editchng(0x7FFE);
}
