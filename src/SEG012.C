/* target: seg012_1D31 */
/* opts: -mm -1 -G -O -d */
/* The main loop and the per-screen change dispatcher. Names are the originals from the
   FM Towns symbol table. */

extern int notdone;
extern void *inplist;
extern unsigned char dsfx_playing;
extern int changed;
extern int scrnum;
extern void (far *editor_dispatch[][16])(void);
extern int change_state[];
extern unsigned char pmouseHandled;

void far update_digi_playback(void);
void far input_dispatch(void *list);
void far mouse_freereign(void);
void far do_changes(void);

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
