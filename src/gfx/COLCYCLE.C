/* target: seg023 */
/* opts: -mm -1 -G -O -d */
/* Palette colour cycling: DOS resident segment seg023, in original order.

   The game animates some colours by rotating palette entries rather than redrawing pixels.
   cycle_colors is called from the input loop (INTERACT.C, with the low byte of *Time) and
   rotates its banks once every 64 ticks; rotate_bank is also used by the cutscene player
   (CUTS.C anm_cycle) for the colour cycles of an LPF file. Both work on the in-memory
   palette (palette, owned by GRCORE.ASM) and send the changed entries to the VGA DAC with
   local_do_palette (MODEX.ASM). The file owns only its phase bytes and a one-entry save.

   name: descriptive (our name for what the file does); rotate_bank and cycle_colors are the
   FM Towns names. */

#include "gfx.h"
#include "sys.h"

static unsigned char last_phase = 0;            /* DS:354 */
static unsigned char last_half = 0;            /* DS:355 */
static unsigned char saved[3];              /* DS:24AC */

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
   four 4-entry banks at E0h..EFh one place down and the runs 3..7 and 8..10 one place up,
   then uploads E0h..EFh and 3..10. */
void far cycle_colors(unsigned char t)
{
    if ((t >> 5) == last_phase)
        return;
    last_phase = t >> 5;
    if ((last_phase >> 1) == last_half)
        return;
    rotate_bank(0xE0, 4, 0);
    rotate_bank(0xE4, 4, 0);
    rotate_bank(0xE8, 4, 0);
    rotate_bank(0xEC, 4, 0);
    local_do_palette(0x10, 0xE0);
    rotate_bank(3, 5, 1);
    rotate_bank(8, 3, 1);
    local_do_palette(8, 3);
    last_half = last_phase >> 1;
}
