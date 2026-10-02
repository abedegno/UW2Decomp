/* target: ovr102 */
/* opts: -mm -1 -G -O -Y -d */
/* Combining objects: the player drops one object on another in the inventory and a new
   object is made (a pole and some thread make a fishing pole). The rules are DATA\CMB.DAT,
   up to ten entries of three words, first, second and output item (UW-Formats 6.4): the
   low nine bits of each word are an item id, and bit 15 of a source word says that source
   is used up. UW2's file has five rules and fills the other five with 0xFFFF: pole and
   thread make a fishing pole, thread and a lump of wax a candle, a lit torch and an ear of
   corn popcorn, a lit torch and a honeycomb a lump of wax, and a nutritious wafer (0xBF)
   and a bottle of water a bottle of ale (the torch is kept; every other source is used
   up). UWEDIT.C calls init_combinables at
   start-up; INVPANEL.C's drop-on-object code calls ObjsBeCombinable, then CombineObjs and
   RemoveAfterCombine for each source. make_stew is an empty stub (FM Towns has the same
   one), called from USEITEMS.C.
   Name: descriptive (combining objects, init_combinables and CombineObjs). */

#include "file.h"
#include "object.h"

struct Combination {
    unsigned first, second, output;
};

/* match: this file's _BSS, DS:47C0..47FB: between ovr101's and ovr103's, and only this file
   uses it. */
/* name: FM Towns has no name for it (its disassembly names the storage after _chroff, the
   last of ovr101's variables before it), so it was static. */
static struct Combination ObjectCombinations[10];

void far init_combinables(void)
{
    bltfromdrive("DATA\\cmb.dat", ObjectCombinations, 0x3C);
}

/* Returns the index of the CMB.DAT rule that combines a and b, in either order, or -1.
   Neither object may hold anything or be a stack of more than one (an is_quant link above
   1, or a container link other than 0). */
int far ObjsBeCombinable(struct Object far *a, struct Object far *b)
{
    int ids[2];
    register struct Combination *entry;
    register int i;
    unsigned first, second;

    if (OBJ_ISQUANT(a) && a->ol.f.link > 1 ||
        !OBJ_ISQUANT(a) && a->ol.f.link > 0)
        return -1;
    if (OBJ_ISQUANT(b) && b->ol.f.link > 1 ||
        !OBJ_ISQUANT(b) && b->ol.f.link > 0)
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

/* Creates a new static object of rule combo's output item; returns 0 if none is free. */
struct Object far * far CombineObjs(int combo)
{
    return CreateObj(((unsigned *)ObjectCombinations)[combo * 3 + 2], 0);
}

/* Returns 1 when rule combo uses obj up: bit 15 of whichever source word names obj's item
   (the second word when the first does not). */
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

/* A stub that returns 0 (FM Towns has the same; any stew making was left out). */
char far make_stew(void)
{
    return 0;
}
