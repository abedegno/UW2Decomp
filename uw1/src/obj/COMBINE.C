/* target: ovr099 */
/* opts: -mm -1 -G -O -Y -d */
/* Combining objects: the player drops one object on another in the inventory and a new
   object is made, and rotworm stew. The rules are DATA\CMB.DAT, up to ten entries of three
   words, first, second and output item (UW-Formats 6.4): the low nine bits of each word
   are an item id, and bit 15 of a source word says that source is used up. UW1's file has
   nine rules and a tenth of zeros. UWEDIT.C calls init_combinables at start-up; INVPANEL.C's drop-on-object code calls
   ObjsBeCombinable, then CombineObjs and RemoveAfterCombine for each source. make_stew
   (UseBook, USEITEMS.C, reading the recipe) turns a bowl of the right ingredients into
   rotworm stew; UW2 kept only an empty stub of it.
   The whole of UW1's DOS overlay ovr099 (UW2's ovr102), in original order.
   UW1 has no symbol-bearing build: the names are UW2's (descriptive there: combining
   objects, init_combinables and CombineObjs), the routines being the same. */

#include "file.h"
#include "inv.h"
#include "object.h"
#include "ui.h"
#include "sys.h"

/* Declared in each file that uses it, its own way (no header). */
unsigned char far bltfromdrive(char *name, void far *buf, unsigned n);

struct Combination {
    uint16 first, second, output;
};

/* match: this file's _BSS, in UW1 DS:48C6..4901 (UW2 DS:47C0..47FB); only this file uses
   it. */
/* name: UW2's FM Towns build has no name for it, so it was static. */
static struct Combination ObjectCombinations[10];

void far init_combinables(void)
{
    bltfromdrive("DATA\\cmb.dat", ObjectCombinations, 0x3C);
}

/* Returns the index of the CMB.DAT rule that combines a and b, in either order, or -1.
   Neither object may hold anything or be a stack of more than one (an is_quant link above
   1, or a container link other than 0). UW1: only a rule that uses up one of its sources
   counts (an unused entry, 0 in UW1's file, never does), and each step goes to the debug
   printer. */
int far ObjsBeCombinable(struct Object far *a, struct Object far *b)
{
    int16 ids[2];
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
    dprintf("checking if %d and %d are combinable...\n", ids[1], ids[0]);
    entry = ObjectCombinations;
    for (i = 0; i < 10; entry++, i++) {
        first = entry->first & ID_ITEM;
        second = entry->second & ID_ITEM;
        dprintf("combination %d is %d and %d.\n", i, first, second);
        if ((entry->first & 0x8000 | entry->second & 0x8000) != 0 &&
            ((first == ids[1] && second == ids[0]) ||
             (first == ids[0] && second == ids[1])))
            break;
    }
    dprintf("objsbecombinable returns %d\n", i == 10 ? -1 : i);
    return i == 10 ? -1 : i;
}

/* Creates a new static object of rule combo's output item; returns 0 if none is free. */
struct Object far * far CombineObjs(int combo)
{
    return CreateObj(((uint16 *)ObjectCombinations)[combo * 3 + 2], 0);
}

/* Returns 1 when rule combo uses obj up: bit 15 of whichever source word names obj's item
   (the second word when the first does not). */
char far RemoveAfterCombine(struct Object far *obj, int combo)
{
    register int id;
    register uint16 *entry;

    id = obj->id & ID_ITEM;
    entry = (uint16 *)&ObjectCombinations[combo];
    if ((*entry & ID_ITEM) == id)
        ;
    else
        entry++;
    return (*entry & 0x8000) != 0;
}

/* Rotworm stew, made when the player reads the recipe (UseBook, USEITEMS.C, for a book
   whose link is 0x100 or more; UW2 kept only an empty make_stew). The player needs a bowl
   (FindObj(2, 0, 0xE, 4, ...), anywhere in the inventory) holding only the three
   ingredients (items 0xD9, 0xB8 and 0xBE), each at least once. The bowl's contents are freed and it
   becomes a bowl of rotworm stew (item 0x11B); if it was the open bag, the bag is closed
   first. Messages: 0x96 no bowl, 0x94 the wrong ingredients, 0x95 success (block 1).
   Returns 1 when the stew was made. */
char far make_stew(void)
{
    int recipe[3] = { 0xD9, 0xB8, 0xBE };
    int found[3] = { 0, 0, 0 };
    int16 where;
    struct Object far *bowl;
    struct Object far *obj;
    register int i;
    register int ok = 0;

    if ((bowl = FindObj(MAJOR_MISC, 0, 0xE, 4, &where)) == 0) {
        game_sprint(0x96);
        return 0;
    }
    obj = Obj_PtrTMem(&bowl->ol.link);
    while (obj != 0) {
        ok = 0;
        for (i = 0; i < 3; i++) {
            found[i] += recipe[i] == (obj->id & ID_ITEM);
            ok |= recipe[i] == (obj->id & ID_ITEM);
        }
        if (!ok)
            goto wrong;
        obj = Obj_PtrTMem(&obj->qn.link);
    }
    ok = 1;
    for (i = 0; i < 3; i++)
        ok &= found[i] != 0;
    if (!ok)
        goto wrong;
    if (Obj_MemTPtr(bowl) == OpenBag->obj.f.index)
        CloseTheBag();
    Obj_FreeChain(&bowl->ol.link);
    bowl->id = bowl->id & 0xFE00 | 0x11B;
    DisplayInventory();
    game_sprint(0x95);
    return 1;
wrong:
    game_sprint(0x94);
    return 0;
}
