/* target: seg036_32A9 */
/* opts: -mm -1 -G -O -d */
/* MAPADDR.C: Map_GetAddr and CreateObj, the whole of DOS resident segment seg036_32A9, in
   original order. Map_GetAddr is the bounds-checked way into the tile map (mapdata, owned
   by MAP.C) that most of the game uses; CreateObj takes a free record from the object
   store (OBJECTS.C's Obj_Alloc) and gives it a new object's default fields. The file owns
   no data.

   name: descriptive (the file's own name is not known). The function names are the
   originals from the FM Towns symbol table. */

#include "critter.h"
#include "map.h"
#include "object.h"

extern struct Tile far *mapdata;

/* The tile at x, y, or a null pointer when either is outside 0..63. */
struct Tile far * far Map_GetAddr(int x, int y)
{
    return ((x & ~0x3F) + (y & ~0x3F)) == 0 ? mapdata + (x + (y << 6)) : 0L;
}

/* A new object of type `item` from the mobile (mobile nonzero) or static free list, or a
   null pointer when the list is empty. The new object has quality 40, no owner, no next
   link, z 0, heading 0, fine x and y 3 (near the middle of a tile), and the id flags
   (bits 9-14) clear. An item whose ComObj stack field is 0 or 2 gets a quantity of 1
   (ID_ISQUANT with link 1); any other has a link of 0. A creature is then set up by
   init_this_critter. The object is not placed in any tile list. */
struct Object far * far CreateObj(int item, char mobile)
{
    struct Object far *obj;
    struct ComObj *com;

    obj = Obj_Alloc(mobile);
    com = &ComObjData[item];
    if (obj == 0L)
        return 0L;
    obj->qn.f.quality = 40;
    obj->id = obj->id & 0xFE00 | item & ID_ITEM;
    obj->pos = obj->pos & 0xFF80;
    obj->id = obj->id & 0xDFFF;
    obj->id = obj->id & 0xBFFF;
    obj->id = obj->id & 0xEFFF;
    obj->id = obj->id & 0xF7FF;
    obj->pos = obj->pos & 0x1FFF | 0x6000;
    obj->pos = obj->pos & 0xE3FF | 0x0C00;
    obj->pos = obj->pos & 0xFC7F;
    obj->id = obj->id & 0xE1FF;
    obj->qn.f.next = 0;
    obj->ol.f.owner = 0;
    if (com->stack == 0 || com->stack == 2) {
        obj->ol.f.link = 1;
        obj->id = obj->id & 0x7FFF | ID_ISQUANT;
    } else {
        obj->id = obj->id & 0x7FFF;
        obj->ol.f.link = 0;
    }
    if (OBJ_MAJOR(obj) == MAJOR_CREATURE)
        init_this_critter(obj);
    return obj;
}
