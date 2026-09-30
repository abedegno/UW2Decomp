/* target: seg025_26A1 */
/* opts: -mm -1 -G -O -d */
/* Damage to objects: resistances, damage to doors and containers, and what an object
   leaves behind when it breaks. The whole of DOS resident segment seg025_26A1, in original
   order. Function and global names are the originals from the FM Towns symbol table. */

#include <stdlib.h>

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0xCE];
    unsigned long flagsCE;              /* 0xCE */
    char pad1[0x36E - 0xD2];
    unsigned char xclock1;              /* 0x36E */
    char pad2[0x370 - 0x36F];
    unsigned char xclock3;              /* 0x370 */
};

/* A mobile object. The first 8 bytes are shared with static objects. */
struct Object {
    unsigned id;                        /* item:9, ..., is_quant:1 */
    unsigned pos;
    union {
        unsigned word;                  /* the next object in this list */
        struct { unsigned quality:6, next:10; } f;
    } qn;
    union {
        unsigned word;                  /* the head of the contents list */
        struct { unsigned owner:6, link:10; } f;
    } ol;
    unsigned char hp;                   /* 0x08 */
    char pad09[0x16 - 0x09];
    unsigned home;                      /* 0x16, x in bits 10-15, y in bits 4-9 */
};

#define OBJ_ITEM(o)     ((o)->id & 0x1FF)
#define OBJ_MAJOR(o)    (((o)->id & 0x1C0) >> 6)
#define OBJ_CLASS(o)    (((o)->id & 0x1F0) >> 4)
#define OBJ_FLAG13(o)   (((o)->id & 0x2000) >> 13)
#define OBJ_ISQUANT(o)  (((o)->id & 0x8000) >> 15)
#define OBJ_QUALITY(o)  ((o)->qn.f.quality)
#define OBJ_OWNER(o)    ((o)->ol.f.owner)
#define OBJ_LINK(o)     ((o)->ol.f.link)

#define ITEM_CLASS(i)   (((i) & 0x1F0) >> 4)

struct Tile {
    unsigned type:4;
    unsigned height:4;
    char pad1;
    unsigned objects;                   /* 0x02, head of the tile's object list */
};

/* Weapon data, 8 bytes per weapon. */
struct Weapon {
    char pad0[6];
    unsigned char skill;                /* 0x06 */
    char pad7;
};

/* Data common to every object type, 11 bytes per item. */
struct ComObj {
    char pad0[6];
    unsigned b6_0:2;
    unsigned scale:2;                   /* 0x06, bits 2-3: damage is shifted down by this */
    unsigned b6_4:11;
    unsigned trespass:1;                /* 0x07, bit 7 */
    unsigned char resist;               /* 0x08 */
    char pad9[2];
};

extern struct Player near *player;
extern struct Object far *ThePlayer;
extern int PlayerLevel;
extern int MapObj_X, MapObj_Y;
extern struct Object far *objdata;
extern struct Weapon Weapons[];
extern struct ComObj ComObjData[];

/* Elsewhere in the game. */
struct Object far * far Obj_InList(unsigned far **head, int a, int major, int minor, int idx);
char far Obj_Rem(unsigned far *head, struct Object far *obj);
void far Obj_Free(struct Object far *obj);
struct Object far * far Obj_Punt(unsigned far *head, struct Object far *obj, int a);
void far Obj_FreeChain(unsigned far *head);
char far IsMobElem(struct Object far *obj);
unsigned char far Obj_Elem_Fate(int how, struct Object far *obj);
struct Tile far * far Map_GetAddr(int x, int y);
struct Object far * far obj_deal(struct Object far *obj, int x, int y, int a);
void far OpenDoor(struct Object far *who, struct Object far *door);
void far UseCont(struct Object far *who, struct Object far *obj, int a);
void far DumpTheBag(struct Object far *obj, int a);
void far game_sprint(int id);
void far fill_FB(int colour);
int far rollem(int n, int sides);
void far put_effect(struct Object far *obj, int type, int size, int a, int b, int x, int y);
char far damage_critter(struct Object far *obj, unsigned char damage, struct Object far *who);
void far player_did_bad(int owner);
void far checkTrap(struct Object far *who, struct Object far *obj, int how, int x, int y);
int far debris_type(int item, char type);
char far damage_item(struct Object far *obj, struct Object far *who, int x, int y,
                     unsigned char damage, unsigned char type);
char far damage_object(struct Object far *obj, struct Object far *who, int damage, int x, int y);

char far remove_lock(struct Object far *obj, char all)
{
    unsigned far *head;
    struct Object far *lock;
    char found = 0;

    if (!OBJ_ISQUANT(obj) && OBJ_LINK(obj) > 0)
    {
        for (head = &obj->ol.word; (lock = Obj_InList(&head, 1, 4, 0, 0xF)) != 0; )
        {
            if (Obj_Rem(head, lock))
                Obj_Free(lock);
            if (all == 0)
                break;
            found = 1;
        }
    }
    else
        found = 0;
    return found;
}

char far try_remove(unsigned far *head, struct Object far *obj)
{
    if (obj >= objdata)
        return Obj_Punt(head, obj, 0) == 0;
    obj->hp = 0;
    return 0;
}

char far remove_object(struct Object far *obj, struct Object far *who, char type, int x, int y)
{
    int debris = -2;
    int item;
    unsigned far *head;

    if (x < 0)
        return 1;
    if (ITEM_CLASS(item = OBJ_ITEM(obj)) == 0x14)
    {
        if ((item & 0xF) <= 7)
        {
            MapObj_X = x;
            MapObj_Y = y;
            OpenDoor(who, obj);
        }
        remove_lock(obj, 1);
        debris = -1;
    }
    else if (item == 0x15D || item == 0x15B)
    {
        if (OBJ_FLAG13(obj))
            debris = -1;
        else
        {
            MapObj_X = x;
            MapObj_Y = y;
            remove_lock(obj, 0);
            UseCont(0L, obj, 0);
        }
    }
    else if (ITEM_CLASS(item) == 0 && type != 8)
    {
        if (item == 3 || item == 10)
            debris = 0xC7;
        else
            debris = Weapons[item & 0xF].skill + 0xC5;
    }
    else if (ITEM_CLASS(item) == 8)
    {
        if (!Obj_Elem_Fate(10, obj))
            debris = -1;
        else
            DumpTheBag(obj, 0);
    }
    else if (item == 0x116)
    {
        if (player->xclock3 < 5)
        {
            game_sprint(0x171);
            damage_item(ThePlayer, 0L, (ThePlayer->home & 0xFC00) >> 10,
                        (ThePlayer->home & 0x3F0) >> 4, 0xFF, 0);
        }
        else if (PlayerLevel == 0x45 && x >= 0x15 && x <= 0x16 && y >= 0x34 && y <= 0x35)
        {
            fill_FB(2);
            player->xclock3 = 6;
            game_sprint(0x150);
            if (player->xclock1 == 0xD)
                player->xclock1 = 0xE;
            player->flagsCE = (player->flagsCE & 0xFFFFFFFDL) + 2;
            if (try_remove(&Map_GetAddr(x, y)->objects, obj))
                return 1;
            debris = -1;
        }
        else
            fill_FB(2);
    }
    else
    {
        if (type & 8)
        {
            if (OBJ_ITEM(obj) == 0xD6)
            {
                if (try_remove(&Map_GetAddr(x, y)->objects, obj))
                    return 1;
                debris = -1;
            }
            else if (!(rand() & 3))
            {
                put_effect(obj, 8, rollem(6, 10), 0, 0, x, y);
                debris = 0xD6;
            }
        }
        if (!OBJ_ISQUANT(obj) && OBJ_LINK(obj) > 0)
        {
            head = &obj->ol.word;
            Obj_FreeChain(head);
        }
    }
    if (debris < -1)
        debris = debris_type(item, type);
    if (debris >= 0)
    {
        obj->id = obj->id & 0xFE00 | debris & 0x1FF;
        if (IsMobElem(obj))
            obj->hp = 0x28;
        obj->qn.f.quality = 0x28;
        if (obj >= objdata && !obj_deal(obj, x, y, 1))
            return 1;
    }
    return debris < -1;
}

int far debris_type(int item, char type)
{
    if (ITEM_CLASS(item) == 0 && type != 8)
    {
        if (item == 3)
            return 0xC7;
        return Weapons[item & 0xF].skill + 0xC5;
    }
    if (ITEM_CLASS(item) == 0x15)
        return 0xDC;
    return 0xD6;
}

unsigned char far check_res(struct Object far *obj, unsigned char damage, unsigned char type)
{
    unsigned char res = ComObjData[OBJ_ITEM(obj)].resist;

    if (type & res)
    {
        if (type & 3)
        {
            if (rand() % 3 < (res & 3))
                return 0;
            type = type & 0xFC;
        }
        if (res & type)
            return 0;
    }
    if ((type & 8 && res & 0x20) || (type & 0x20 && res & 8 && (res & 0x28) != 0x28))
    {
        if (damage < 0x7F)
            damage = damage << 1;
        else
            damage = 0xFF;
    }
    return damage;
}

char far damage_item(struct Object far *obj, struct Object far *who, int x, int y,
                     unsigned char damage, unsigned char type)
{
    damage = check_res(obj, damage, type);
    if (OBJ_MAJOR(obj) == 1)
        return damage_critter(obj, damage, who);
    if (damage_object(obj, who, damage, x, y))
        return remove_object(obj, who, type, x, y);
    return 0;
}

char far damage_object(struct Object far *obj, struct Object far *who, int damage, int x, int y)
{
    char destroyed = 0;
    unsigned char mobile;
    struct ComObj *co = &ComObjData[OBJ_ITEM(obj)];
    int scale;
    int hp;

    if ((OBJ_FLAG13(obj) && OBJ_CLASS(obj) != 0x14) || (scale = co->scale) == 3)
        return 0;
    damage >>= scale;
    if (damage <= 0)
        return 0;
    mobile = IsMobElem(obj);
    if (mobile)
    {
        hp = obj->hp - damage;
        if (hp <= 0)
        {
            hp = 0;
            destroyed = 1;
        }
        obj->hp = hp;
    }
    else if (OBJ_ITEM(obj) >= 0x140 && OBJ_ITEM(obj) <= 0x147 && (OBJ_OWNER(obj) & 1)
             && OBJ_OWNER(obj) >> 1 > 0)
    {
        hp = (OBJ_OWNER(obj) >> 1) - damage;
        if (hp <= 0)
            hp = 0;
        obj->ol.f.owner = OBJ_OWNER(obj) & 1 | hp << 1;
    }
    else
    {
        hp = OBJ_QUALITY(obj) - damage;
        if (hp <= 0)
        {
            destroyed = 1;
            hp = 0;
        }
        obj->qn.f.quality = hp;
        if (ComObjData[OBJ_ITEM(obj)].trespass)
            player_did_bad(OBJ_OWNER(obj));
    }
    if (destroyed && !mobile && x > -1)
        checkTrap(who, obj, 4, x, y);
    return destroyed;
}
