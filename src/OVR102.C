/* target: ovr102 */
/* opts: -mm -1 -G -O -Y -d */

struct Object {
    unsigned id;
    unsigned pos;
    unsigned qn;
    unsigned quantity_low:6;
    unsigned quantity_count:10;
};

struct Combination {
    unsigned first, second, output;
};

/* DOS DS:47C0. FM Towns calls the corresponding storage _chroff, which
   conflicts with the chargen pointer of that name in the DOS sources. */
extern struct Combination ObjectCombinations[10];
void far LoadDATFile(char *name, void far *dest, int size);
struct Object far * far CreateObj(unsigned id, int mobile);

void far init_combinables(void)
{
    LoadDATFile("DATA\\cmb.dat", ObjectCombinations, 0x3C);
}

int far ObjsBeCombinable(struct Object far *a, struct Object far *b)
{
    int ids[2];
    register struct Combination *entry;
    register int i;
    unsigned first, second;

    if (((a->id & 0x8000) >> 15) && a->quantity_count > 1 ||
        !((a->id & 0x8000) >> 15) && a->quantity_count > 0)
        return -1;
    if (((b->id & 0x8000) >> 15) && b->quantity_count > 1 ||
        !((b->id & 0x8000) >> 15) && b->quantity_count > 0)
        return -1;
    ids[1] = a->id & 0x1FF;
    ids[0] = b->id & 0x1FF;
    entry = ObjectCombinations;
    for (i = 0; i < 10; entry++, i++) {
        first = entry->first & 0x1FF;
        second = entry->second & 0x1FF;
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

    id = obj->id & 0x1FF;
    entry = (unsigned *)&ObjectCombinations[combo];
    if ((*entry & 0x1FF) == id)
        ;
    else
        entry++;
    return (*entry & 0x8000) != 0;
}

char far make_stew(void)
{
    return 0;
}
