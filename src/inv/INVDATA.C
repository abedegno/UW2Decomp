/* target: ovr124 */
/* opts: -mm -1 -G -O -Y -d */
/* The player's inventory as data: what is in each slot, adding objects to it and taking
   them out (whole stacks or part of one, from the paperdoll, the backpack or the open bag),
   searching it for an object by class, damage to worn equipment, and object weight: the
   whole of DOS overlay ovr124, in original order. Function and global names are the
   originals from the FM Towns symbol table except FindEmptySlot, which has no FM Towns
   counterpart and a provisional name chosen for its key (see below); the source file's own
   name is not known.

   Turbo C lists a file's publics in descending order of the tools/bssorder.py key of each
   name, and TLINK numbers overlay stub entries from the last one listed, so the EXE's stub
   order constrains the names. FM Towns gives AskInventory and WhatsInSlot one address (the
   two functions are identical, and DOS has both), so its code cannot tell them apart; the
   stub order can: the function at +15 must have a key between 321 and 481 and the one at
   +5A4 between 630 and 702, which AskInventory (465) and WhatsInSlot (655) meet only this way
   round. FindEmptySlot (1022) must sort above takeFromSlot (1004). */

#include <string.h>
#include "inv.h"
#include "object.h"
#include "player.h"
#include "ui.h"
#include "uw2.h"

extern struct Player PlayerDat;
extern union Link Inventory[];
extern struct Inplist near *inplist;

char far damage_item(struct Object far *obj, struct Object far *who, int x, int y,
                     unsigned char damage, unsigned char type);
struct Object far * far CreateObj(int item, char mobile);
int far near_mob_put_at(struct Object far *at, struct Object far *obj, int a, int b);
void far get_name(char far *buf, struct Object far *obj, int article, char plural);
void far scroll_print(char far *s);

char far invRemoveObject(struct Object far *obj, int qty);

void far RedisplayInvSlot(int slot)
{
    DisplayInvObject(SlotToDisplay[slot]);
}

struct Object far * far AskInventory(int slot)
{
    return Obj_PtrTMem(&Inventory[slot]);
}

/* The first empty slot from 5 to 18, or -1. Not in the FM Towns build and not called; IDA's
   ovr124_2C, renamed for its key. */
int far FindEmptySlot(void)
{
    register int slot;

    for (slot = 5; slot <= 18; slot++)
        if (Inventory[slot].f.index == 0)
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
                owner = Obj_PtrTMem(&OpenBag->obj);
                for (bag = OpenBag; bag; bag = bag->prev)
                    bag->weight += mass;
            }
            Inventory[slot].f.index = Obj_MemTPtr(obj);
        }
        Obj_AddEnd(&owner->ol.link, obj);
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
        if (Inventory[slot].f.index != 0) {
            if (Inventory[slot].f.index == index)
                return slot;
            inslot = Obj_PtrTMem(&Inventory[slot]);
            if (!OBJ_ISQUANT(inslot) && Inventory[slot].f.index != OpenBag->obj.f.index
                && inslot != 0 && Obj_Find(&inslot->ol.link, 1, index) != 0)
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
        objs[i] = Obj_PtrTMem(&Inventory[i]);
        if (objs[i] != 0 && (major < 0 || OBJ_MAJOR(objs[i]) == major)
            && (minor < 0 || OBJ_MINOR(objs[i]) == minor)
            && (cls < 0 || OBJ_INCLASS(objs[i]) == cls)) {
            *where = i;
            return objs[i];
        }
    }
    if (how == 1)
        return 0;
    for (; i <= 18; i++) {
        objs[i] = Obj_PtrTMem(&Inventory[i]);
        if (objs[i] != 0 && (major < 0 || OBJ_MAJOR(objs[i]) == major)
            && (minor < 0 || OBJ_MINOR(objs[i]) == minor)
            && (cls < 0 || OBJ_INCLASS(objs[i]) == cls)) {
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
            contents = Obj_PtrTMem(&objs[i]->ol.link);
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
            && (cls < 0 || OBJ_INCLASS(*list) == cls)) {
            found = *list;
            *list = 0;
            return found;
        }
        if (!OBJ_ISQUANT(*list) && (next = Obj_PtrTMem(&(*list)->ol.link)) != 0
            && (found = find_obj(major, minor, cls, &next)) != 0) {
            if (next != 0)
                *list = next;
            return found;
        }
        *list = Obj_PtrTMem(&(*list)->qn.link);
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
        return WhatsInSlot(DisplayToSlot[hit]);
    return RemoveAllFromSlot(-1, -1, -1, DisplayToSlot[hit]);
}

/* The same as AskInventory; the FM Towns map gives both names one address. */
struct Object far * far WhatsInSlot(int slot)
{
    return Obj_PtrTMem(&Inventory[slot]);
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
        if (Inventory[i].f.index == index)
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
        obj = Obj_Find(&ThePlayer->ol.link, 1, index);
        if (obj == 0)
            return 0;
        if (qty > 0 && OBJ_ISQUANT(obj) && !(obj->ol.f.link & LINK_SPECIAL)
            && (have = obj->ol.f.link) > 1 && qty < have) {
            copy = Obj_Alloc(0);
            *(struct StaticObj far *)copy = *(struct StaticObj far *)obj;
            copy->ol.f.link = have - qty;
            obj->ol.f.link = qty;
            Obj_Add(&obj->qn.link, copy);
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
    if ((inslot = Obj_PtrTMem(&Inventory[slot])) != 0 && OBJ_MAJOR(inslot) == MAJOR_MISC
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
    if ((inslot = Obj_PtrTMem(&Inventory[slot])) != 0 && OBJ_MAJOR(inslot) == MAJOR_MISC
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
    obj = Obj_PtrTMem(&Inventory[slot]);
    if (obj == 0)
        return 0;
    if (slot <= 18)
        owner = ThePlayer;
    else
        owner = Obj_PtrTMem(&OpenBag->obj);
    if (major >= 0 || minor >= 0 || cls >= 0) {
        if ((major >= 0 && OBJ_MAJOR(obj) != major) || (minor >= 0 && OBJ_MINOR(obj) != minor)
            || (cls >= 0 && OBJ_INCLASS(obj) != cls)) {
            if ((obj = find_obj(major, minor, cls, &owner)) == 0)
                return 0;
        }
    }
    if (qty != 0 && OBJ_ISQUANT(obj) && !(obj->ol.f.link & LINK_SPECIAL)
        && (have = obj->ol.f.link) > 1 && qty < have) {
        copy = Obj_Alloc(0);
        *(struct StaticObj far *)copy = *(struct StaticObj far *)obj;
        copy->ol.f.link = have - qty;
        obj->ol.f.link = qty;
        Obj_Add(&obj->qn.link, copy);
    }
    if (owner == ThePlayer || slot >= 20) {
        if (copy != 0)
            Inventory[slot].f.index = Obj_MemTPtr(copy);
        else
            Inventory[slot].f.index = 0;
    }
    if (!Obj_Rem(&owner->ol.link, obj))
        return 0;
    mass = ItemWeight(obj);
    PlayerDat.weight -= mass;
    if (OpenBag != 0 && Obj_MemTPtr(owner) == OpenBag->obj.f.index) {
        index = Obj_MemTPtr(obj);
        for (i = 20; i <= 27; i++) {
            if (Inventory[i].f.index == index) {
                Inventory[i].f.index = copy == 0 ? 0 : Obj_MemTPtr(copy);
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
    if ((id >> 6) == MAJOR_HACK && (id & ID_MINOR) >> 4 >= 2 && (id & ID_INCLASS) >= 0xB && (id & ID_INCLASS) <= 0xF)
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

    if ((obj = AskInventory(slot)) == 0)
        return -2;
    if (how != 2) {
        if (how == 0) {
            if (OBJ_CLASS(obj) != CLASS_WEAPON)
                return -2;
        } else if (!ObjWorn(OBJ_ITEM(obj), slot))
            return -2;
    }
    quality = obj->qn.f.quality;
    if (damage_item(obj, 0L, -1, -1, damage, type)) {
        if (debris) {
            junk = CreateObj(debris_type(OBJ_ITEM(obj), type), 0);
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
        obj->id = obj->id & 0xFE00 | ITEM_FIST;
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

    com = &ComObjData[OBJ_ITEM(obj)];
    if (OBJ_ISQUANT(obj) && !(obj->ol.f.link & LINK_SPECIAL))
        mass = obj->ol.f.link * com->mass;
    else {
        mass = com->mass;
        if (!OBJ_ISQUANT(obj) && obj->ol.f.link > 0)
            BagWeight(&obj->ol.link, &mass);
    }
    return mass;
}

unsigned char far EncumCheck(struct Object far *obj)
{
    if (ItemWeight(obj) + PlayerDat.weight > PlayerDat.max_weight)
        return 0;
    return 1;
}
