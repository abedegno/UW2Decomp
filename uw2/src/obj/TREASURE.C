/* target: ovr163 */
/* opts: -mm -1 -G -O -Y -d */
/* Spilling a container's contents onto the map, and the loot a critter is generated with:
   the whole of DOS overlay ovr163, in original order.

   What it does in the game: generate_inventory gives a critter, the first time it needs
   one (OBJ_HAS_INV clear), the loot its Creature record describes (the Guide's "Loot sub
   table"): treasure scaled by the world, food, its two weapons and two other items, each
   by chance. drop_link_chain empties a container or a dead critter onto the floor where it
   stands; the items a critter drops belong to its race (drop_some_objects), so taking them
   can count as theft.

   Data owned: LootCreature, the Creature record being looted.
   Function and global names are the originals from the FM Towns symbol table.
   Name: descriptive (spilling containers and generating loot: generate_treasure). */

#include <stdlib.h>
#include "combat.h"
#include "critter.h"
#include "event.h"
#include "map.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"

/* This file's _BSS, DS:863A. */
/* name: only this file uses it; no FM Towns name, so static. */
static struct Creature near *LootCreature;


/* Empties container cont onto the floor at its position (MapObj_X, MapObj_Y for a static
   container), after removing its locks. Each object can be given owner (when nonzero and
   the item can be owned); lit lights go out; a trap trigger among the contents (MAJOR_TRAP
   minor 2 and up) is set off as it falls out (UseTrigger with how 4). Returns 1 if there was
   anything inside. */
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
                SET_ITEM(obj, OBJ_ITEM(obj) - 4);
            put_at(x, y, z, obj, 6, 0);
            if (OBJ_MAJOR(obj) == MAJOR_TRAP && OBJ_MINOR(obj) >= MINOR_TRIGGER) {
                cont->ol.f.link = Obj_MemTPtr(next);
                UseTrigger(ThePlayer, (struct Object far *)0, obj, 4);
                next = Obj_PtrTMem(&cont->ol.link);
                cont->ol.f.link = 0;
            }
            obj = next;
        }
        return 1;
    }
    return 0;
}

/* A critter drops what it carries; the objects belong to its race. */
void far drop_some_objects(struct Object far *critter)
{
    drop_link_chain(critter, Creature[OBJ_INMAJOR(critter)].race);
}

/* Maybe adds treasure (items 0xA0 on) to npc. With chance treasure_rate in 16, a
   treasure type is rolled from -(30 - 3w) to 6, w the world number; anything below 2
   becomes type 0, so deeper worlds give better treasure more often. The type's value
   (ComObjData, stretched above 4) against four times treasure_prob decides the quantity:
   one at a chance when the value is higher, else 4d(2 * (4 * prob / value)) / 4. */
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

/* With chance food_prob in 16, adds the critter's food item. */
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

/* Adds the critter's two weapons if present, at a random quality (half the time 4..8
   times the dungeon level, which the 6-bit quality field wraps on deep levels, otherwise
   0..63); ammunition (missile items whose Missile ammo
   byte is 0xC0) comes in a stack of 4..11. */
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
        if (OBJ_MINOR(obj) == MINOR_MISSILE && (unsigned char)Missile[obj->id & ID_INCLASS].ammo == 0xC0)
            obj->ol.f.link = rand() % 8 + 4;
        Obj_Add(&npc->ol.link, obj);
    }
}

/* Adds each of the two other items with its chance in 16, at a random quality as for
   weapons (quality type 0xF items always 40). */
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

/* Gives npc its loot once and sets OBJ_HAS_INV so it is not given again. */
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
    SET_HAS_INV(npc, 1);
}
