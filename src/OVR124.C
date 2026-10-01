/* target: ovr124 */
/* opts: -mm -1 -G -O -Y -d */
/* The player's inventory as data: what is in each slot, adding objects to it and taking
   them out (whole stacks or part of one, from the paperdoll, the backpack or the open bag),
   searching it for an object by class, damage to worn equipment, and object weight: the
   whole of DOS overlay ovr124, in original order. Function and global names are the
   originals from the FM Towns symbol table except ovr124_2C, which has no FM Towns
   counterpart; the source file's own name is not known. */

#include <string.h>

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0x65];
    unsigned lefty:1;                   /* 0x65 */
};

/* The player's statistics block. */
struct PlayerStats {
    char pad0[0x4A];
    unsigned weight;                    /* 0x4A, weight carried */
    unsigned capacity;                  /* 0x4C, weight that can be carried */
};

/* A link word: the low six bits belong to the owner, the rest is an object index. */
union Link {
    unsigned word;
    struct { unsigned low:6, link:10; } f;
};

/* A mobile object. The first 8 bytes are shared with static objects. */
struct Object {
    unsigned id;
    unsigned pos;
    union {
        unsigned word;                  /* the next object in this list */
        struct { unsigned quality:6, next:10; } f;
    } qn;
    union {
        unsigned word;                  /* the head of the contents list, or the quantity */
        struct { unsigned owner:6, link:10; } f;
    } ol;
};

#define OBJ_ID(o)       ((o)->id & 0x1FF)
#define OBJ_MAJOR(o)    (((o)->id & 0x1C0) >> 6)
#define OBJ_MINOR(o)    (((o)->id & 0x30) >> 4)
#define OBJ_INDEX(o)    ((o)->id & 0xF)
#define OBJ_CLASS(o)    (((o)->id & 0x1F0) >> 4)
#define OBJ_ISQUANT(o)  (((o)->id & 0x8000) >> 15)

/* One open bag (see ovr121). */
struct Bag {
    struct Bag far *next;
    struct Bag far *prev;               /* 0x04, the bag this one was opened from */
    union Link obj;                     /* 0x08 */
    int weight;                         /* 0x0A */
};

/* One object type's common properties, 11 bytes. */
struct ComObj {
    unsigned char height;               /* 0x00 */
    unsigned radius:4;                  /* 0x01 */
    unsigned mass:12;
    char pad3[0x0B - 0x03];
};

struct Inplist {
    int x, y;                           /* 0x00 */
};

extern struct Player near *player;
extern struct PlayerStats PlayerDat;
extern struct Object far *ThePlayer;
extern union Link Inventory[];
extern char SlotToDisplay[];
extern char DisplayToSlot[];
extern struct Bag far *OpenBag;
extern struct Inplist near *inplist;
extern struct ComObj ComObjData[];
extern unsigned far *Obj_Find_Head;

struct Object far * far Obj_PtrTMem(unsigned far *link);
int far Obj_MemTPtr(struct Object far *obj);
struct Object far * far Obj_Find(unsigned far *head, int recurse, int index);
struct Object far * far Obj_Alloc(int mobile);
void far Obj_Add(unsigned far *head, struct Object far *obj);
void far Obj_AddEnd(unsigned far *head, struct Object far *obj);
unsigned char far Obj_Rem(unsigned far *head, struct Object far *obj);
void far Obj_Punt(unsigned far *head, struct Object far *obj, int how);
void far DisplayInvObject(int slot);
int far ItemFitsSlot(struct Object far *obj, int slot);
int far FindInventoryHit(int x, int y);
void far FixPlayerEquips(void);
void far FixOpenBag(void);
void far DisplayOpenBag(void);
void far BagWeight(unsigned far *head, int far *total);
char far damage_item(struct Object far *obj, struct Object far *who, int x, int y,
                     unsigned char damage, unsigned char type);
int far debris_type(int item, char type);
struct Object far * far CreateObj(int item, char mobile);
int far near_mob_put_at(struct Object far *at, struct Object far *obj, int a, int b);
void far get_name(char far *buf, struct Object far *obj, int article, char plural);
void far scroll_print(char far *s);

int far ItemWeight(struct Object far *obj);
struct Object far * far find_obj(int major, int minor, int cls, struct Object far **list);
struct Object far * far AskInventory(int slot);
struct Object far * far RemoveAllFromSlot(int major, int minor, int cls, int slot);
char far invRemoveObject(struct Object far *obj, int qty);
struct Object far * far removeFromSlot(int major, int minor, int cls, int slot, int qty);
struct Object far * far takeFromSlot(int major, int minor, int cls, int slot, int qty);

void far RedisplayInvSlot(int slot)
{
    DisplayInvObject(SlotToDisplay[slot]);
}

struct Object far * far WhatsInSlot(int slot)
{
    return Obj_PtrTMem(&Inventory[slot].word);
}

/* The first empty slot from 5 to 18, or -1. Not in the FM Towns build and not called. */
int far ovr124_2C(void)
{
    register int slot;

    for (slot = 5; slot <= 18; slot++)
        if (Inventory[slot].f.link == 0)
            return slot;
    return -1;
}

unsigned char far AddToInventory(struct Object far *obj, int slot)
{
    char done;
    struct Object far *owner;
    struct Bag far *bag;
    register int mass;
    register int fits;

    done = 0;
    owner = ThePlayer;
    if (slot == -1)
        fits = 1;
    else
        fits = ItemFitsSlot(obj, slot);
    if (fits > 0) {
        mass = ItemWeight(obj);
        if (slot >= 0) {
            if (slot > 18) {
                owner = Obj_PtrTMem(&OpenBag->obj.word);
                for (bag = OpenBag; bag; bag = bag->prev)
                    bag->weight += mass;
            }
            Inventory[slot].f.link = Obj_MemTPtr(obj);
        }
        Obj_AddEnd(&owner->ol.word, obj);
        PlayerDat.weight += mass;
        done = 1;
    } else if (fits == -1)
        done = 0;
    FixPlayerEquips();
    return done;
}

/* The slot holding obj, minus the slot of a container holding it, or -1. */
int far FindSlot(struct Object far *obj)
{
    int index;
    struct Object far *inslot;
    register int slot;
    register int i;

    index = Obj_MemTPtr(obj);
    for (i = 0; i < 20; i++) {
        slot = DisplayToSlot[i];
        if (Inventory[slot].f.link != 0) {
            if (Inventory[slot].f.link == index)
                return slot;
            inslot = Obj_PtrTMem(&Inventory[slot].word);
            if (!OBJ_ISQUANT(inslot) && Inventory[slot].f.link != OpenBag->obj.f.link
                && inslot != 0 && Obj_Find(&inslot->ol.word, 1, index) != 0)
                return -slot;
        }
    }
    return -1;
}

struct Object far * far FindObj(int major, int minor, int cls, int how, register int *where)
{
    struct Object far *contents;
    struct Object far *objs[19];
    register int i;

    for (i = 0; i < 11; i++) {
        objs[i] = Obj_PtrTMem(&Inventory[i].word);
        if (objs[i] != 0 && (major < 0 || OBJ_MAJOR(objs[i]) == major)
            && (minor < 0 || OBJ_MINOR(objs[i]) == minor)
            && (cls < 0 || OBJ_INDEX(objs[i]) == cls)) {
            *where = i;
            return objs[i];
        }
    }
    if (how == 1)
        return 0;
    for (; i <= 18; i++) {
        objs[i] = Obj_PtrTMem(&Inventory[i].word);
        if (objs[i] != 0 && (major < 0 || OBJ_MAJOR(objs[i]) == major)
            && (minor < 0 || OBJ_MINOR(objs[i]) == minor)
            && (cls < 0 || OBJ_INDEX(objs[i]) == cls)) {
            *where = i;
            return objs[i];
        }
    }
    if (how == 2)
        return 0;
    if (how == 3)
        return 0;
    for (i = 0; i <= 18; i++) {
        if (objs[i] != 0 && !OBJ_ISQUANT(objs[i])) {
            contents = Obj_PtrTMem(&objs[i]->ol.word);
            objs[i] = find_obj(major, minor, cls, &contents);
            if (objs[i] != 0) {
                *where = i;
                return objs[i];
            }
        }
    }
    return 0;
}

struct Object far * far find_obj(int major, int minor, register int cls, register struct Object far **list)
{
    struct Object far *next;
    struct Object far *found;

    while (*list != 0) {
        if ((major < 0 || OBJ_MAJOR(*list) == major)
            && (minor < 0 || OBJ_MINOR(*list) == minor)
            && (cls < 0 || OBJ_INDEX(*list) == cls)) {
            found = *list;
            *list = 0;
            return found;
        }
        if (!OBJ_ISQUANT(*list) && (next = Obj_PtrTMem(&(*list)->ol.word)) != 0
            && (found = find_obj(major, minor, cls, &next)) != 0) {
            if (next != 0)
                *list = next;
            return found;
        }
        *list = Obj_PtrTMem(&(*list)->qn.word);
    }
    return 0;
}

struct Object far * far pick_inv(int how)
{
    int x;
    register int hit;
    register int y;

    x = inplist->x + 0xF0;
    y = inplist->y + 0x51;
    hit = FindInventoryHit(x, y);
    if (hit < 0 || hit >= 20)
        return 0;
    if (how == 2)
        return AskInventory(DisplayToSlot[hit]);
    return RemoveAllFromSlot(-1, -1, -1, DisplayToSlot[hit]);
}

/* The same as WhatsInSlot; the FM Towns map gives this name the same address. */
struct Object far * far AskInventory(int slot)
{
    return Obj_PtrTMem(&Inventory[slot].word);
}

char far InvRemoveObject(struct Object far *obj)
{
    return invRemoveObject(obj, -1);
}

char far InvRemoveOneObject(struct Object far *obj)
{
    return invRemoveObject(obj, 1);
}

char far invRemoveObject(struct Object far *obj, int qty)
{
    int index;
    int have;
    struct Object far *copy;
    struct Bag far *bag;
    register int i;
    register int mass;

    mass = ItemWeight(obj);
    index = Obj_MemTPtr(obj);
    for (i = 0; i < 28; i++)
        if (Inventory[i].f.link == index)
            break;
    if (i < 28) {
        removeFromSlot(-1, -1, -1, i, qty);
        if (i >= 19) {
            FixOpenBag();
            DisplayOpenBag();
            for (bag = OpenBag; bag; bag = bag->prev)
                bag->weight -= mass;
        } else
            DisplayInvObject(SlotToDisplay[i]);
    } else {
        obj = Obj_Find(&ThePlayer->ol.word, 1, index);
        if (obj == 0)
            return 0;
        if (qty > 0 && OBJ_ISQUANT(obj) && !(obj->ol.f.link & 0x200)
            && (have = obj->ol.f.link) > 1 && qty < have) {
            copy = Obj_Alloc(0);
            *copy = *obj;
            copy->ol.f.link = have - qty;
            obj->ol.f.link = qty;
            Obj_Add(&obj->qn.word, copy);
        }
        if (!Obj_Rem(Obj_Find_Head, obj))
            return 0;
        PlayerDat.weight -= mass;
        DisplayInvObject(0x13);
        FixPlayerEquips();
    }
    return 1;
}

struct Object far * far RemoveAllFromSlot(int major, int minor, int cls, register int slot)
{
    struct Object far *taken;
    struct Object far *inslot;

    taken = removeFromSlot(major, minor, cls, slot, 0);
    if ((inslot = Obj_PtrTMem(&Inventory[slot].word)) != 0 && OBJ_MAJOR(inslot) == 2
        && OBJ_MINOR(inslot) == 0 && OpenBag != 0) {
        FixOpenBag();
        DisplayOpenBag();
    } else
        DisplayInvObject(SlotToDisplay[slot]);
    return taken;
}

struct Object far * far RemoveOneFromSlot(int major, int minor, int cls, register int slot)
{
    struct Object far *taken;
    struct Object far *inslot;

    taken = removeFromSlot(major, minor, cls, slot, 1);
    if ((inslot = Obj_PtrTMem(&Inventory[slot].word)) != 0 && OBJ_MAJOR(inslot) == 2
        && OBJ_MINOR(inslot) == 0 && OpenBag != 0) {
        FixOpenBag();
        DisplayOpenBag();
    } else
        DisplayInvObject(SlotToDisplay[slot]);
    return taken;
}

struct Object far * far removeFromSlot(int major, int minor, int cls, int slot, int qty)
{
    struct Object far *obj;

    obj = takeFromSlot(major, minor, cls, slot, qty);
    FixPlayerEquips();
    return obj;
}

struct Object far * far takeFromSlot(int major, int minor, int cls, int slot, int qty)
{
    struct Object far *obj;
    struct Object far *copy;
    struct Object far *owner;
    int have;
    int index;
    struct Bag far *bag;
    register int i;
    register int mass;

    copy = 0;
    obj = Obj_PtrTMem(&Inventory[slot].word);
    if (obj == 0)
        return 0;
    if (slot <= 18)
        owner = ThePlayer;
    else
        owner = Obj_PtrTMem(&OpenBag->obj.word);
    if (major >= 0 || minor >= 0 || cls >= 0) {
        if ((major >= 0 && OBJ_MAJOR(obj) != major) || (minor >= 0 && OBJ_MINOR(obj) != minor)
            || (cls >= 0 && OBJ_INDEX(obj) != cls)) {
            if ((obj = find_obj(major, minor, cls, &owner)) == 0)
                return 0;
        }
    }
    if (qty != 0 && OBJ_ISQUANT(obj) && !(obj->ol.f.link & 0x200)
        && (have = obj->ol.f.link) > 1 && qty < have) {
        copy = Obj_Alloc(0);
        *copy = *obj;
        copy->ol.f.link = have - qty;
        obj->ol.f.link = qty;
        Obj_Add(&obj->qn.word, copy);
    }
    if (owner == ThePlayer || slot >= 20) {
        if (copy != 0)
            Inventory[slot].f.link = Obj_MemTPtr(copy);
        else
            Inventory[slot].f.link = 0;
    }
    if (!Obj_Rem(&owner->ol.word, obj))
        return 0;
    mass = ItemWeight(obj);
    PlayerDat.weight -= mass;
    if (OpenBag != 0 && Obj_MemTPtr(owner) == OpenBag->obj.f.link) {
        index = Obj_MemTPtr(obj);
        for (i = 20; i <= 27; i++) {
            if (Inventory[i].f.link == index) {
                Inventory[i].f.link = copy == 0 ? 0 : Obj_MemTPtr(copy);
                for (bag = OpenBag; bag; bag = bag->prev)
                    bag->weight -= mass;
                break;
            }
        }
    }
    return obj;
}

/* Whether an object of type id is being worn or wielded in slot. */
unsigned char far ObjWorn(register int id, register int slot)
{
    if (slot >= 0 && slot <= 4)
        return 1;
    if (slot == 10 || slot == 9)
        return 1;
    if (player->lefty + 7 != slot)
        return 0;
    if ((id >> 6) == 0 && (id & 0x30) >> 4 >= 2 && (id & 0xF) >= 0xB && (id & 0xF) <= 0xF)
        return 1;
    return 0;
}

/* Damage the object in slot: -2 if there is none or it does not take this damage, -1 if
   it was unharmed, otherwise 1 if it was destroyed and 0 if only damaged. */
int far DamageInventory(int slot, unsigned char damage, unsigned char type, int how, char debris)
{
    struct Object far *obj;
    int quality;
    struct Object far *junk;
    char text[50];
    register char *msg;
    register int result;

    if ((obj = WhatsInSlot(slot)) == 0)
        return -2;
    if (how != 2) {
        if (how == 0) {
            if (OBJ_CLASS(obj) != 0)
                return -2;
        } else if (!ObjWorn(OBJ_ID(obj), slot))
            return -2;
    }
    quality = obj->qn.f.quality;
    if (damage_item(obj, 0L, -1, -1, damage, type)) {
        if (debris) {
            junk = CreateObj(debris_type(OBJ_ID(obj), type), 0);
            near_mob_put_at(ThePlayer, junk, 6, 0);
        }
        InvRemoveOneObject(obj);
        Obj_Punt(0L, obj, 1);
        FixPlayerEquips();
        msg = " destroyed.\n";
        result = 1;
    } else if (obj->qn.f.quality != quality) {
        msg = " damaged.\n";
        result = 0;
    } else
        return -1;
    strcpy(text, "Your ");
    if (obj == ThePlayer)
        obj->id = obj->id & 0xFE00 | 0xF;
    get_name(text + strlen(text), obj, 0, 0);
    if (text[strlen(text) - 1] == 's')
        strcat(text, " were");
    else
        strcat(text, " was");
    strcat(text, msg);
    scroll_print(text);
    RedisplayInvSlot(slot);
    return result;
}

int far ItemWeight(struct Object far *obj)
{
    int mass;
    register struct ComObj *com;

    com = &ComObjData[OBJ_ID(obj)];
    if (OBJ_ISQUANT(obj) && !(obj->ol.f.link & 0x200))
        mass = obj->ol.f.link * com->mass;
    else {
        mass = com->mass;
        if (!OBJ_ISQUANT(obj) && obj->ol.f.link > 0)
            BagWeight(&obj->ol.word, &mass);
    }
    return mass;
}

unsigned char far EncumCheck(struct Object far *obj)
{
    if (ItemWeight(obj) + PlayerDat.weight > PlayerDat.capacity)
        return 0;
    return 1;
}
