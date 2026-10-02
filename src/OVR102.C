/* target: ovr102 */
/* opts: -mm -1 -G -O -Y -d */

#include "object.h"

struct Combination {
    unsigned first, second, output;
};

/* This file's _BSS, DS:47C0..47FB: between ovr101's and ovr103's, and only this file uses
   it. FM Towns has no name for it (its disassembly names the storage after _chroff, the
   last of ovr101's variables before it), so it was static. */
static struct Combination ObjectCombinations[10];
void far bltfromdrive(char *name, void far *dest, int size);
struct Object far * far CreateObj(unsigned id, int mobile);

void far init_combinables(void)
{
    bltfromdrive("DATA\\cmb.dat", ObjectCombinations, 0x3C);
}

int far ObjsBeCombinable(struct Object far *a, struct Object far *b)
{
    int ids[2];
    register struct Combination *entry;
    register int i;
    unsigned first, second;

    if (((a->id & ID_ISQUANT) >> 15) && a->ol.f.link > 1 ||
        !((a->id & ID_ISQUANT) >> 15) && a->ol.f.link > 0)
        return -1;
    if (((b->id & ID_ISQUANT) >> 15) && b->ol.f.link > 1 ||
        !((b->id & ID_ISQUANT) >> 15) && b->ol.f.link > 0)
        return -1;
    ids[1] = a->id & ID_ITEM;
    ids[0] = b->id & ID_ITEM;
    entry = ObjectCombinations;
    for (i = 0; i < 10; entry++, i++) {
        first = entry->first & ID_ITEM;
        second = entry->second & ID_ITEM;
        if ((first == ids[1] && second == ids[0]) ||
            (first == ids[0] && second == ids[1]))
            break;
    }
    return i != 10 ? i : -1;
}

struct Object far * far CombineObjs(int combo)
{
    return CreateObj(((unsigned *)ObjectCombinations)[combo * 3 + 2], 0);
}

char far RemoveAfterCombine(struct Object far *obj, int combo)
{
    register int id;
    register unsigned *entry;

    id = obj->id & ID_ITEM;
    entry = (unsigned *)&ObjectCombinations[combo];
    if ((*entry & ID_ITEM) == id)
        ;
    else
        entry++;
    return (*entry & 0x8000) != 0;
}

char far make_stew(void)
{
    return 0;
}
