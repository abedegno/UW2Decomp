/* target: seg012_1D31 */
/* opts: -mm -1 -G -O -d */
/* The main loop and the per-screen change dispatcher. Names are the originals from the
   FM Towns symbol table. */

#include "motion.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"

extern void *inplist;
unsigned char dsfx_playing = 0;      /* this file's _DATA: DS:010A */

void far input_dispatch(void *list);

void far mainloop(void)
{
    while (notdone)
    {
        do_changes();
        input_dispatch(inplist);
    }
}

void far do_changes(void)
{
    unsigned i;
    int bit;

    if (dsfx_playing)
        update_digi_playback();
    if (changed)
    {
        for (i = 0, bit = 1; i < 15; i++, bit <<= 1)
        {
            if (changed & bit)
            {
                changed = changed & ~bit;
                if (editor_dispatch[scrnum][i])
                    editor_dispatch[scrnum][i]();
            }
        }
        changed |= change_state[scrnum];
        if (pmouseHandled > 0)
        {
            if (--pmouseHandled == 0)
                mouse_freereign();
        }
    }
}

void far editchng(int bits)
{
    changed |= bits;
}
