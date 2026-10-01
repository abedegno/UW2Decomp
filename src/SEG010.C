/* target: seg010_1CA1 */
/* opts: -mm -1 -G -O -Y -d */
/* The input dispatcher: tables of mouse regions and key handlers, each entry with a
   handle, a mode mask and a far handler, and the dispatch of one mouse or key event to
   the first entry that takes it. The whole of DOS resident segment seg010_1CA1, in
   original order. Function and global names are the originals from the FM Towns symbol
   table. */

#include <stdlib.h>

/* The mouse and keyboard state handed to a handler, 10 bytes. */
struct Inplist {
    int x, y;                           /* relative to the region that took the click */
    int mouse;                          /* 0x04: 1 for a mouse event, 0 for a key */
    int cmd;                            /* 0x06: the input code */
    int mode;                           /* 0x08: mask of the screen modes */
};

/* A mouse region, 0x12 bytes. */
struct MouseDispatch {
    int hndl;
    int lrx, lry;                       /* 0x02: bottom right corner */
    int ulx, uly;                       /* 0x06: top left corner */
    int arg;                            /* 0x0A: handed to the handler */
    int mask;                           /* 0x0C: the modes it answers in */
    void (far *func)(int arg);          /* 0x0E */
};

/* A key handler, 0x0C bytes. */
struct KeyDispatch {
    int hndl;
    int key;                            /* 0x02 */
    int arg;                            /* 0x04 */
    int mask;                           /* 0x06 */
    void (far *func)(int arg);          /* 0x08 */
};

void far first_punt(int code);
void far pfatal_code(int code);
int far mouse_get_input_sp(void);
void far mouse_Qgetxy(int *x, int *y);
void far dispatch_key(struct Inplist *in, int code);

/* Uninitialised data, DS:22B6 to DS:22C9. */
int mcurhndl;                           /* the next mouse handle, counting up from 1 */
struct MouseDispatch *mous_dispatch;
int kdispcnt;
int mdispcnt;
static struct Inplist inp;
struct KeyDispatch *key_dispatch;

/* Initialised data, DS:E4 onwards. */
struct Inplist *inplist = &inp;
int kcurhndl = -666;                    /* the next key handle, counting down from -1;
                                           -666 while the tables are not allocated */

void far init_input(void)
{
    mous_dispatch = malloc(sizeof(struct MouseDispatch));
    key_dispatch = malloc(sizeof(struct KeyDispatch));
    if (mous_dispatch == 0 || key_dispatch == 0)
        first_punt(0x1003);
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

int far input_addmouse(int ulx, int uly, int lrx, int lry, int arg, int mask,
                       void (far *func)(int))
{
    struct MouseDispatch *p;

    mdispcnt++;
    if ((p = realloc(mous_dispatch, mdispcnt * sizeof(struct MouseDispatch))) == 0)
        pfatal_code(0x1005);
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

int far _input_addkey(int key, int arg, int mask, void (far *func)(int))
{
    struct KeyDispatch *p;

    kdispcnt++;
    if ((p = realloc(key_dispatch, kdispcnt * sizeof(struct KeyDispatch))) == 0)
        pfatal_code(0x1006);
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
                pfatal_code(0x1005);
        } else
            mdispcnt--;
    } else {
        for (kp = key_dispatch; i++ < kdispcnt && kp->hndl != hndl; kp++)
            ;
        if (kdispcnt + 1 == i)
            return;
        if (i < kdispcnt)
            *kp = key_dispatch[kdispcnt - 1];
        if (kdispcnt > 1) {
            kdispcnt--;
            if ((key_dispatch = realloc(key_dispatch,
                                        kdispcnt * sizeof(struct KeyDispatch))) == 0)
                pfatal_code(0x1006);
        } else
            kdispcnt--;
    }
}

void far input_dispatch(struct Inplist *in)
{
    int code;
    int x, y;
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

/* IDA's ProcessEventHandlers. Named from FM Towns: dispatch_key_ follows input_dispatch_
   there, is what input_dispatch_ calls for a key, and makes the same search of
   key_dispatch on the key code and the mode mask. */
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
