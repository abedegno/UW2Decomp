/* target: ovr163 */
/* opts: -mm -1 -G -O -Y -d */
/* Spilling a container's contents onto the map, and the loot a critter is generated with:
   the whole of DOS overlay ovr163, in original order. Function and global names are the
   originals from the FM Towns symbol table; the source file's own name is not known. */

#include <stdlib.h>
#include "combat.h"
#include "critter.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"

extern struct MissileInfo Missile[];
/* This file's _BSS, DS:863A: only this file uses it; no FM Towns name, so static. */
static struct Creature near *LootCreature;

char far put_at(int x, int y, int z, struct Object far *obj, int a, int b);
void far UseTrigger(struct Object far *who, int a, int b, struct Object far *trig, int how);
struct Object far * far CreateObj(int id, int b);

char far drop_link_chain(struct Object far *cont, int owner)
{
    struct Object far *obj;
    struct Object far *next;
    int z;
    int x, y;

    remove_lock(cont, 1);
    if (cont->ol.f.link != 0) {
        obj = Obj_PtrTMem(&cont->ol.link);
        cont->ol.f.link = 0;
        if (IsMobElem(cont)) {
            x = OBJ_HOMEX(cont);
            y = OBJ_HOMEY(cont);
        } else {
            x = MapObj_X;
            y = MapObj_Y;
        }
        z = cont->pos & POS_Z;
        x = (x << 3) + OBJ_FINEX(cont);
        y = (y << 3) + OBJ_FINEY(cont);
        while (obj != 0) {
            next = Obj_PtrTMem(&obj->qn.link);
            if (owner && ComObjData[OBJ_ITEM(obj)].can_own)
                obj->ol.f.owner = owner;
            if (OBJ_ITEM(obj) >= FIRST_LIT_LIGHT && OBJ_ITEM(obj) <= ITEM_LIT_LIGHT_SPHERE)
                obj->id = obj->id & 0xFE00 | (OBJ_ITEM(obj) - 4) & ID_ITEM;
            put_at(x, y, z, obj, 6, 0);
            if (OBJ_MAJOR(obj) == MAJOR_TRAP && OBJ_MINOR(obj) >= 2) {
                cont->ol.f.link = Obj_MemTPtr(next);
                UseTrigger(ThePlayer, 0, 0, obj, 4);
                next = Obj_PtrTMem(&cont->ol.link);
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
    drop_link_chain(critter, Creature[OBJ_INMAJOR(critter)].race);
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
    if ((value = (unsigned char)ComObjData[type + 0xA0].value) == 0)
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
    obj = CreateObj(type + FIRST_TREASURE, 0);
    obj->ol.f.link = qty;
    Obj_Add(&npc->ol.link, obj);
}

void far generate_food(struct Object far *npc)
{
    unsigned char prob;
    unsigned char item;
    struct Object far *obj;

    prob = LootCreature->food_prob;
    item = LootCreature->food_item;
    if (rand() % 16 < prob) {
        obj = CreateObj(item + FIRST_FOOD, 0);
        Obj_Add(&npc->ol.link, obj);
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
        if (OBJ_MINOR(obj) == 1 && (unsigned char)Missile[obj->id & ID_INCLASS].ammo == 0xC0)
            obj->ol.f.link = rand() % 8 + 4;
        Obj_Add(&npc->ol.link, obj);
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
            Obj_Add(&npc->ol.link, obj);
        }
    }
}

void far generate_inventory(struct Object far *npc)
{
    int minor, index;

    if OBJ_HAS_INV(npc)
        return;
    minor = OBJ_MINOR(npc);
    index = npc->id & ID_INCLASS;
    LootCreature = &Creature[(minor << 4) + index];
    generate_treasure(npc);
    generate_food(npc);
    generate_weapons(npc);
    generate_equipment(npc);
    npc->attitude_word = npc->attitude_word & 0xEFFF | 0x1000;
}
