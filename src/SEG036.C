/* target: seg036_32A9 */
/* opts: -mm -1 -G -O -d */
/* Map_GetAddr and CreateObj: the whole of DOS segment seg036_32A9, in original order.
   Names are the originals from the FM Towns symbol table. */

struct Object {
    unsigned id;                        /* item 0-8, flags 9-12, door 13, invis 14, is_quant 15 */
    unsigned pos;                       /* z 0-6, heading 7-9, y 10-12, x 13-15 */
    union {
        unsigned word;
        struct { unsigned quality:6, next:10; } f;
    } qn;
    union {
        unsigned word;
        struct { unsigned owner:6, link:10; } f;
    } ol;
};

struct Tile {
    unsigned type:4;
    unsigned height:4;
    char pad1;
    unsigned objects;
};

/* One entry of the common object table, 11 bytes per item id. */
struct ComObj {
    char pad0[3];
    unsigned b3:6;
    unsigned stack:2;                   /* 0x03, bits 6-7 */
    char pad4[0x0B - 0x04];
};

extern struct Tile far *mapdata;
extern struct ComObj ComObjData[];

struct Object far * far Obj_Alloc(char mobile);
void far init_this_critter(struct Object far *obj);

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
    obj->id = obj->id & 0xFE00 | item & 0x1FF;
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
        obj->id = obj->id & 0x7FFF | 0x8000;
    } else {
        obj->id = obj->id & 0x7FFF;
        obj->ol.f.link = 0;
    }
    if (((obj->id & 0x1C0) >> 6) == 1)
        init_this_critter(obj);
    return obj;
}
