/* target: seg021 */
/* opts: -mm -1 -G -O -d */
/* Palette colour cycling: the whole of UW1's DOS resident segment seg021, in original
   order. UW1 has no symbol-bearing build; the names are UW2's (the FM Towns symbol table;
   UW2's COLCYCLE.C, seg023), the code being the same routines.

   The game animates some colours by rotating palette entries rather than redrawing pixels.
   cycle_colors rotates its banks once every 64 ticks of the time byte it is given;
   rotate_bank is also used by the main menu (MAINMENU.C ovr138_94). Both work on the
   in-memory palette (palette) and send the changed entries to the VGA DAC with
   local_do_palette. The file owns only its phase bytes and a one-entry save.

   UW1's difference: the banks are other colours (30h..3Fh in four banks of four, and the
   runs 10h..14h and 15h..17h; UW2 E0h..EFh, 3..7 and 8..10).

   name: descriptive (our name for what the file does); rotate_bank and cycle_colors are the
   FM Towns names (UW2). */

#include "gfx.h"
#include "sys.h"

static unsigned char last_phase = 0;            /* DS:234 */
static unsigned char last_half = 0;            /* DS:235 */
static unsigned char saved[3];              /* DS:2650 */

/* Rotate COUNT palette entries starting at FIRST by one place, upward when UP is set. */
void far rotate_bank(unsigned char first, unsigned char count, unsigned char up)
{
    unsigned char far *p;
    int j;
    int step;
    int i;

    p = palette + first * 3;
    step = up ? 3 : -3;
    if (up)
        p += (count - 1) * 3;
    saved[0] = p[0];
    saved[1] = p[1];
    saved[2] = p[2];
    for (i = 0; i < count - 1; i++, p -= step)
        for (j = 0; j < 3; j++)
            p[j] = (p - step)[j];
    p[0] = saved[0];
    p[1] = saved[1];
    p[2] = saved[2];
}

/* Called with a running time byte. last_phase follows t >> 5 and last_half t >> 6, so the
   banks rotate only when t >> 6 changes, every 64 ticks. Each call that rotates moves the
   four 4-entry banks at 30h..3Fh one place down and the runs 10h..14h and 15h..17h one
   place up, then uploads 30h..3Fh and 10h..17h. */
void far cycle_colors(unsigned char t)
{
    if ((t >> 5) == last_phase)
        return;
    last_phase = t >> 5;
    if ((last_phase >> 1) == last_half)
        return;
    rotate_bank(0x30, 4, 0);
    rotate_bank(0x34, 4, 0);
    rotate_bank(0x38, 4, 0);
    rotate_bank(0x3C, 4, 0);
    local_do_palette(0x10, 0x30);
    rotate_bank(0x10, 5, 1);
    rotate_bank(0x15, 3, 1);
    local_do_palette(8, 0x10);
    last_half = last_phase >> 1;
}
