/* target: ovr163 */
/* opts: -mm -1 -G -O -Y -d */
/* Spilling a container's contents onto the map, and the loot a critter is generated with:
   the whole of DOS overlay ovr163, in original order. Function and global names are the
   originals from the FM Towns symbol table; the source file's own name is not known. */

#include <stdlib.h>

/* A mobile object. The first 8 bytes are shared with static objects. */
struct Object {
    unsigned id;                        /* item id 0-8 (major 6-8, minor 4-5, index 0-3) */
    unsigned pos;                       /* z 0-6, heading 7-9, x 10-12, y 13-15 */
    union {
        unsigned word;                  /* the next object in this list */
        struct { unsigned quality:6, next:10; } f;
    } qn;
    union {
        unsigned word;                  /* the head of the contents list, or the quantity */
        struct { unsigned owner:6, link:10; } f;
    } ol;
    char pad08[0x0D - 0x08];
    unsigned flags0D;                   /* 0x0D, loot generated in bit 12 */
    char pad0F[0x16 - 0x0F];
    unsigned home;                      /* 0x16, x in bits 10-15, y in bits 4-9 */
};

#define OBJ_ID(o)       ((o)->id & 0x1FF)
#define OBJ_MAJOR(o)    (((o)->id & 0x1C0) >> 6)
#define OBJ_MINOR(o)    (((o)->id & 0x30) >> 4)
#define OBJ_INDEX(o)    (((o)->id & 0x3F) >> 0)

/* One critter type's record, 0x30 bytes. */
struct Creature {
    char pad00[9];
    unsigned char race;                 /* 0x09 */
    char pad0A[0x20 - 0x0A];
    struct {
        unsigned char present:1;
        unsigned char item:7;
    } arms[2];                          /* 0x20 */
    struct {
        unsigned prob:4;
        unsigned item:12;
    } other[2];                         /* 0x22 */
    unsigned treasure_prob:4;           /* 0x26 */
    unsigned treasure_rate:4;
    unsigned food_prob:4;               /* 0x27 */
    unsigned food_item:4;
    char pad28[0x30 - 0x28];
};

/* One object type's common properties, 11 bytes. */
struct ComObj {
    char pad0[4];
    unsigned char value;                /* 0x04 */
    char pad5[7 - 5];
    unsigned b7:7;                      /* 0x07 */
    unsigned can_own:1;
    unsigned b8:8;                      /* 0x08 */
    unsigned b9:8;                      /* 0x09 */
    unsigned qualtype:4;                /* 0x0A */
    unsigned bA:4;
};

struct MissileDat {
    char pad0[2];
    unsigned char ammo;                 /* 0x02 */
};

extern struct Object far *ThePlayer;
extern int PlayerLevel;
extern int MapObj_X, MapObj_Y;
extern struct Creature Creature[];
extern struct ComObj ComObjData[];
extern struct MissileDat Missile[];
/* This file's _BSS, DS:863A: only this file uses it; no FM Towns name, so static. */
static struct Creature near *LootCreature;

void far remove_lock(struct Object far *obj, int how);
struct Object far * far Obj_PtrTMem(unsigned far *link);
int far Obj_MemTPtr(struct Object far *obj);
char far IsMobElem(struct Object far *obj);
char far put_at(int x, int y, int z, struct Object far *obj, int a, int b);
void far UseTrigger(struct Object far *who, int a, int b, struct Object far *trig, int how);
struct Object far * far CreateObj(int id, int b);
void far Obj_Add(unsigned far *head, struct Object far *obj);
int far rollem(int dice, int sides);

char far drop_link_chain(struct Object far *cont, int owner)
{
    struct Object far *obj;
    struct Object far *next;
    int z;
    int x, y;

    remove_lock(cont, 1);
    if (cont->ol.f.link != 0) {
        obj = Obj_PtrTMem(&cont->ol.word);
        cont->ol.f.link = 0;
        if (IsMobElem(cont)) {
            x = (cont->home & 0xFC00) >> 10;
            y = (cont->home & 0x3F0) >> 4;
        } else {
            x = MapObj_X;
            y = MapObj_Y;
        }
        z = cont->pos & 0x7F;
        x = (x << 3) + ((cont->pos & 0xE000) >> 13);
        y = (y << 3) + ((cont->pos & 0x1C00) >> 10);
        while (obj != 0) {
            next = Obj_PtrTMem(&obj->qn.word);
            if (owner && ComObjData[OBJ_ID(obj)].can_own)
                obj->ol.f.owner = owner;
            if (OBJ_ID(obj) >= 0x94 && OBJ_ID(obj) <= 0x97)
                obj->id = obj->id & 0xFE00 | (OBJ_ID(obj) - 4) & 0x1FF;
            put_at(x, y, z, obj, 6, 0);
            if (OBJ_MAJOR(obj) == 6 && OBJ_MINOR(obj) >= 2) {
                cont->ol.f.link = Obj_MemTPtr(next);
                UseTrigger(ThePlayer, 0, 0, obj, 4);
                next = Obj_PtrTMem(&cont->ol.word);
                cont->ol.f.link = 0;
            }
            obj = next;
        }
        return 1;
    }
    return 0;
}

void far drop_some_objects(struct Object far *critter)
{
    drop_link_chain(critter, Creature[OBJ_INDEX(critter)].race);
}

void far generate_treasure(struct Object far *npc)
{
    char type;
    char value;
    char qty;
    char dice;
    unsigned char prob;
    unsigned char rate;
    struct Object far *obj;

    prob = LootCreature->treasure_prob;
    rate = LootCreature->treasure_rate;
    if (rand() % 16 >= rate)
        return;
    type = rand() % (37 - (PlayerLevel - 1) / 8 * 3) - (30 - (PlayerLevel - 1) / 8 * 3);
    if (type < 0)
        type = 0;
    if (type == 1)
        type = 0;
    if ((value = ComObjData[type + 0xA0].value) == 0)
        value = 1;
    if (value >= 12)
        value = value * 8 - 68;
    else if (value >= 8)
        value = value * 4 - 20;
    else if (value >= 4)
        value = value * 2 - 4;
    if ((prob << 2) < value) {
        if (rand() % value >= (prob << 2))
            return;
        qty = 1;
    } else {
        dice = ((prob << 2) / value) << 1;
        qty = rollem(4, dice) >> 2;
    }
    if (qty < 1)
        return;
    obj = CreateObj(type + 0xA0, 0);
    obj->ol.f.link = qty;
    Obj_Add(&npc->ol.word, obj);
}

void far generate_food(struct Object far *npc)
{
    unsigned char prob;
    unsigned char item;
    struct Object far *obj;

    prob = LootCreature->food_prob;
    item = LootCreature->food_item;
    if (rand() % 16 < prob) {
        obj = CreateObj(item + 0xB0, 0);
        Obj_Add(&npc->ol.word, obj);
    }
}

void far generate_weapons(struct Object far *npc)
{
    unsigned char item;
    unsigned char i;
    struct Object far *obj;
    int quality, id;

    for (i = 0; i < 2; i = i + 1) {
        if (LootCreature->arms[i].present == 0)
            continue;
        item = LootCreature->arms[i].item;
        id = (((item >> 4) & 3) << 4) + (item & 0xF);
        obj = CreateObj(id, 0);
        if (rand() % 2 == 0)
            quality = (PlayerLevel << 2) + rand() % (PlayerLevel << 2);
        else
            quality = rand() % 64;
        obj->qn.f.quality = quality;
        if (OBJ_MINOR(obj) == 1 && Missile[obj->id & 0xF].ammo == 0xC0)
            obj->ol.f.link = rand() % 8 + 4;
        Obj_Add(&npc->ol.word, obj);
    }
}

void far generate_equipment(struct Object far *npc)
{
    unsigned char i;
    struct Object far *obj;
    int quality, id;

    for (i = 0; i < 2; i = i + 1) {
        if (rand() % 16 < LootCreature->other[i].prob) {
            id = LootCreature->other[i].item;
            obj = CreateObj(id, 0);
            if (rand() % 2 == 0)
                quality = (PlayerLevel << 2) + rand() % (PlayerLevel << 2);
            else
                quality = rand() % 64;
            if (ComObjData[id].qualtype == 0xF)
                quality = 40;
            obj->qn.f.quality = quality;
            Obj_Add(&npc->ol.word, obj);
        }
    }
}

void far generate_inventory(struct Object far *npc)
{
    int minor, index;

    if ((npc->flags0D & 0x1000) >> 12)
        return;
    minor = OBJ_MINOR(npc);
    index = npc->id & 0xF;
    LootCreature = &Creature[(minor << 4) + index];
    generate_treasure(npc);
    generate_food(npc);
    generate_weapons(npc);
    generate_equipment(npc);
    npc->flags0D = npc->flags0D & 0xEFFF | 0x1000;
}
