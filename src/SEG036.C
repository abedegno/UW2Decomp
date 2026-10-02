/* target: seg036_32A9 */
/* opts: -mm -1 -G -O -d */
/* Map_GetAddr and CreateObj: the whole of DOS segment seg036_32A9, in original order.
   Names are the originals from the FM Towns symbol table. */

#include "critter.h"
#include "map.h"
#include "object.h"

extern struct Tile far *mapdata;

struct Object far * far Obj_Alloc(char mobile);

struct Tile far * far Map_GetAddr(int x, int y)
{
    return ((x & ~0x3F) + (y & ~0x3F)) == 0 ? mapdata + (x + (y << 6)) : 0L;
}

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
    if (((obj->id & ID_MAJOR) >> 6) == MAJOR_CREATURE)
        init_this_critter(obj);
    return obj;
}
