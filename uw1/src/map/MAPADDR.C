/* target: seg035_3073 */
/* opts: -mm -1 -G -O -d */
/* MAPADDR.C: Map_GetAddr and CreateObj, the whole of UW1's DOS resident segment
   seg035_3073 (UW2's seg036_32A9), in original order. Map_GetAddr is the bounds-checked way into the tile map (mapdata, owned
   by MAP.C) that most of the game uses; CreateObj takes a free record from the object
   store (OBJECTS.C's Obj_Alloc) and gives it a new object's default fields. The file owns
   no data.

   name: descriptive (the file's own name is not known). UW1 has no symbol-bearing build:
   the function names are UW2's, from the FM Towns symbol table, the routines being the
   same.

   Entry points: Map_GetAddr, called from nearly every file that touches the map;
   CreateObj, from the spells, traps, death and the silver tree (SKILLS.C), critter remains
   (PATHFIND.C) and wherever else a new object is made. */

#include "critter.h"
#include "map.h"
#include "object.h"

/* The tile at x, y, or a null pointer when either is outside 0..63. */
struct Tile far * far Map_GetAddr(int x, int y)
{
    return ((x & ~0x3F) + (y & ~0x3F)) == 0 ? mapdata + (x + (y << 6)) : 0L;
}

/* A new object of type `item` from the mobile (mobile nonzero) or static free list, or a
   null pointer when the list is empty. The new object has quality 40, no owner, no next
   link, z 0, heading 0, fine x and y 3 (near the middle of a tile), and the id flags
   (bits 9-14) clear. An item whose ComObj stack field is 0 or 2 gets a quantity of 1
   (ID_ISQUANT with link 1); any other has a link of 0. Unlike UW2's, UW1's CreateObj
   does not call init_this_critter for a creature. The object is not placed in any tile
   list. */
struct Object far * far CreateObj(int item, char mobile)
{
    struct Object far *obj;
    struct ComObj *com;

    obj = Obj_Alloc(mobile);
    com = &ComObjData[item];
    if (obj == 0L)
        return 0L;
    obj->qn.f.quality = 40;
    SET_ITEM(obj, item);
    SET_Z(obj, 0);
    SET_DOORDIR(obj, 0);
    SET_INVIS(obj, 0);
    SET_ENCHANTED(obj, 0);
    SET_FLAG11(obj, 0);
    SET_FINEX(obj, 3);
    SET_FINEY(obj, 3);
    SET_HEADING(obj, 0);
    SET_FLAGS(obj, 0);
    obj->qn.f.next = 0;
    obj->ol.f.owner = 0;
    if (com->stack == 0 || com->stack == 2) {
        obj->ol.f.link = 1;
        SET_ISQUANT(obj, 1);
    } else {
        SET_ISQUANT(obj, 0);
        obj->ol.f.link = 0;
    }
    return obj;
}
