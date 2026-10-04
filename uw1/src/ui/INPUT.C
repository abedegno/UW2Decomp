/* target: seg010 */
/* opts: -mm -1 -G -O -Y -d */
/* The input dispatcher: tables of mouse regions and key handlers, each entry with a
   handle, a mode mask and a far handler, and the dispatch of one mouse or key event to
   the first entry that takes it. The whole of UW1's DOS resident segment seg010, in
   original order. UW1 has no symbol-bearing build; the names are UW2's (the FM Towns
   symbol table), the code being the same routines. The one UW1 difference: input_del
   reports an unknown key handle through the debug printer dprintf (DEBUG.C, ovr106_1B,
   empty in this build), whose string "bad kbd hndl %d\n" is in this file's data.

   How it is used: each screen registers its click areas with input_addmouse and its keys
   with _input_addkey, giving a mask of the screen modes (inplist->mode: in UW2 1 the 3D view,
   2 the automap, 4 a conversation, set by UWEDIT.C's change_screen) in which the entry is
   live, and removes them with input_del. MAINLOOP.C's mainloop and CONVERSE.C call
   input_dispatch once a pass with inplist; it reads one event through MOUSE.C's
   mouse_get_input_sp and calls the handler with the entry's argument, after filling in
   inplist (the click position relative to the region, the buttons or key code, mouse or
   key). Handlers read inplist to find out what happened.

   Data owned: the two dispatch tables (mous_dispatch, key_dispatch, grown and shrunk with
   realloc on the near heap), their counts, the handle counters (mouse handles count up from
   1, key handles down from -1, so the sign of a handle says which table it is in) and the
   Inplist record inp that inplist points at.

   Name: UW2's originals (init_input is in System Shock's INPUT.C, RCS r:/prj/cit/src/input.c,
   the input dispatcher in both). */

#include <stdlib.h>
#include "sys.h"
#include "ui.h"

/* A mouse region, 0x12 bytes. Screen y counts up from the bottom row (0) to the top (199)
   in this engine (MOUSE.C: the up key warps the pointer to y 199), so despite the "upper
   left" and "lower right" in the field names (ulx, uly) is the bottom left corner and
   (lrx, lry) the top right:
   a point is inside when ulx <= x <= lrx and uly <= y <= lry. */
HOST_LAYOUT_BEGIN
struct MouseDispatch {
    int16 hndl;
    int16 lrx, lry;                     /* 0x02: the corner with the larger x and y */
    int16 ulx, uly;                     /* 0x06: the corner with the smaller x and y */
    NEARPTR arg;                        /* 0x0A: handed to the handler */
    int16 mask;                         /* 0x0C: the modes it answers in */
    void (far *func)(NEARPTR arg);          /* 0x0E */
};
HOST_LAYOUT_END

/* A key handler, 0x0C bytes. */
HOST_LAYOUT_BEGIN
struct KeyDispatch {
    int16 hndl;
    int16 key;                          /* 0x02 */
    NEARPTR arg;                        /* 0x04 */
    int16 mask;                         /* 0x06 */
    void (far *func)(NEARPTR arg);          /* 0x08 */
};
HOST_LAYOUT_END

/* Uninitialised data, DS:24A2 to DS:24B5. */
int16 mcurhndl;                         /* the next mouse handle, counting up from 1 */
struct MouseDispatch *mous_dispatch;
int16 kdispcnt;
int16 mdispcnt;
static struct Inplist inp;
struct KeyDispatch *key_dispatch;

/* Initialised data, DS:E2 onwards. */
struct Inplist *inplist = &inp;
int16 kcurhndl = -666;                  /* the next key handle, counting down from -1;
                                           -666 while the tables are not allocated */

void far init_input(void)
{
    mous_dispatch = malloc(sizeof(struct MouseDispatch));
    key_dispatch = malloc(sizeof(struct KeyDispatch));
    if (mous_dispatch == 0 || key_dispatch == 0)
        first_punt(ERR_LOWMEM | 3);
    mdispcnt = 0;
    kdispcnt = 0;
    mcurhndl = 1;
    kcurhndl = -1;
    inplist->cmd = 0;
}

void far free_input(void)
{
    if (kcurhndl != -666) {
        free(mous_dispatch);
        free(key_dispatch);
        kcurhndl = -666;
    }
}

int far input_addmouse(int ulx, int uly, int lrx, int lry, NEARPTR arg, int mask,
                       void (far *func)(NEARPTR))
{
    struct MouseDispatch *p;

    mdispcnt++;
    if ((p = realloc(mous_dispatch, mdispcnt * sizeof(struct MouseDispatch))) == 0)
        pfatal_code(ERR_LOWMEM | 5);
    mous_dispatch = p;
    p = mous_dispatch + mdispcnt - 1;
    p->hndl = mcurhndl;
    mcurhndl++;
    p->arg = arg;
    p->mask = mask;
    p->func = func;
    p->ulx = ulx;
    p->uly = uly;
    p->lrx = lrx;
    p->lry = lry;
    return p->hndl;
}

int far _input_addkey(int key, NEARPTR arg, int mask, void (far *func)(NEARPTR))
{
    struct KeyDispatch *p;

    kdispcnt++;
    if ((p = realloc(key_dispatch, kdispcnt * sizeof(struct KeyDispatch))) == 0)
        pfatal_code(ERR_LOWMEM | 6);
    key_dispatch = p;
    p = key_dispatch + kdispcnt - 1;
    p->hndl = kcurhndl;
    kcurhndl--;
    p->arg = arg;
    p->mask = mask;
    p->func = func;
    p->key = key;
    return p->hndl;
}

/* Removes the entry with this handle from whichever table its sign selects. The last
   entry is moved into the hole, so the tables do not keep registration order after a
   delete. Handle 0 and unknown handles are ignored (an unknown key handle is reported
   through the empty debug printer). */
void far input_del(int hndl)
{
    int i = 0;
    register struct MouseDispatch *mp;
    register struct KeyDispatch *kp;

    if (hndl == 0)
        return;
    if (hndl > 0) {
        for (mp = mous_dispatch; i++ < mdispcnt && mp->hndl != hndl; mp++)
            ;
        if (mdispcnt + 1 == i)
            return;
        if (i < mdispcnt)
            *mp = mous_dispatch[mdispcnt - 1];
        if (mdispcnt > 1) {
            mdispcnt--;
            if ((mous_dispatch = realloc(mous_dispatch,
                                         mdispcnt * sizeof(struct MouseDispatch))) == 0)
                pfatal_code(ERR_LOWMEM | 5);
        } else
            mdispcnt--;
    } else {
        for (kp = key_dispatch; i++ < kdispcnt && kp->hndl != hndl; kp++)
            ;
        if (kdispcnt + 1 == i) {
            dprintf("bad kbd hndl %d\n", hndl);
            return;
        }
        if (i < kdispcnt)
            *kp = key_dispatch[kdispcnt - 1];
        if (kdispcnt > 1) {
            kdispcnt--;
            if ((key_dispatch = realloc(key_dispatch,
                                        kdispcnt * sizeof(struct KeyDispatch))) == 0)
                pfatal_code(ERR_LOWMEM | 6);
        } else
            kdispcnt--;
    }
}

/* Takes one input event and hands it to a handler. Codes 1 to 3 are the mouse buttons
   held (1 left, 2 right, 3 both; MOUSE.C), anything higher a key code. Mouse regions are
   searched from the most recently added backwards, so a region added later wins over one
   it overlaps; the click position is made relative to the region's (ulx, uly) corner. An
   event that no live entry takes is dropped. */
void far input_dispatch(struct Inplist *in)
{
    int code;
    int16 x, y;
    register int i;

    if ((code = mouse_get_input_sp()) < 0)
        return;
    if (code < 4) {
        mouse_Qgetxy(&x, &y);
        in->cmd = code;
        mouse_Qgetxy(&x, &y);
        in->mouse = 1;
        for (i = mdispcnt - 1; i >= 0; i--) {
            if (mous_dispatch[i].ulx <= x && mous_dispatch[i].uly <= y
                && mous_dispatch[i].lrx >= x && mous_dispatch[i].lry >= y
                && (mous_dispatch[i].mask & in->mode) && mous_dispatch[i].func) {
                in->x = x - mous_dispatch[i].ulx;
                in->y = y - mous_dispatch[i].uly;
                mous_dispatch[i].func(mous_dispatch[i].arg);
                return;
            }
        }
    } else {
        in->mouse = 0;
        dispatch_key(in, code);
    }
}

/* Calls the first key handler (oldest first) registered for this code in the current
   mode; only input_dispatch calls it. */
/* name: IDA's ProcessEventHandlers. Named from FM Towns: dispatch_key_ follows
   input_dispatch_ there, is what input_dispatch_ calls for a key, and makes the same search
   of key_dispatch on the key code and the mode mask. */
void far dispatch_key(struct Inplist *in, int code)
{
    register int i;

    for (i = 0; i < kdispcnt; i++) {
        if (key_dispatch[i].key == code && (key_dispatch[i].mask & in->mode)
            && key_dispatch[i].func) {
            key_dispatch[i].func(key_dispatch[i].arg);
            return;
        }
    }
}
