/* target: ovr131 */
/* opts: -mm -1 -G -O -Y -d */
/* The credits (the main menu's third button, Acknowledgements): plays cutscene 10, the
   credits, then marks every change bit but bit 0 so the next pass of do_changes redraws
   the current screen.
   Entry point: show_credits, called from MAINMENU.C's start menu (choice 2). SKILLS.C
   also plays cutscene 10 itself at the end of the game.
   Data: none.
   Name: descriptive (show_credits). */

#include "sys.h"

void far show_cutscene(int n);

void far show_credits(void)
{
    show_cutscene(10);
    editchng(0x7FFE);
}
