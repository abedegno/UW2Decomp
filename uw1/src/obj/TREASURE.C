/* target: ovr150 */
/* opts: -mm -1 -G -O -Y -d */
/* Spilling a container's contents onto the map, and the loot a critter is generated with:
   the whole of DOS overlay ovr150, in original order. Seeded from UW2Decomp's
   src/obj/TREASURE.C (UW2's ovr163): the same seven functions in the same order.

   What it does in the game: generate_inventory gives a critter, the first time it needs
   one (OBJ_HAS_INV clear), the loot its Creature record describes (the Guide's "Loot sub
   table"): treasure scaled by the dungeon level, food, its two weapons and two other
   items, each by chance. drop_link_chain empties a container or a dead critter onto the
   floor where it stands, giving the container to owner (drop_some_objects: the critter's
   race).

   UW1 differences, all from the bytes: drop_link_chain neither removes locks, nor puts out
   lit lights, nor sets off falling traps, and it tests and sets the owner on the container
   itself rather than on each item; generate_treasure rolls the type from the dungeon level
   (-(33 - 3l) to 6, l the level; UW2 uses the world) and has no type 1 exception;
   generate_equipment has no fixed quality for quality type 0xF.

   Data owned: LootCreature (DS:736C), the Creature record being looted.
   Function and global names are UW2's (the FM Towns originals); UW1 has no symbols of
   its own. drop_link_chain is the table's ovr150_0.
   Name: descriptive, UW2Decomp's (spilling containers and generating loot). */

#include <stdlib.h>
#include "combat.h"
#include "critter.h"
#include "event.h"
#include "map.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"

/* Declared in each file that uses it, its own way (no header). */
unsigned char far IsMobElem(struct Object far *obj);

/* This file's _BSS, DS:736C (UW2 DS:863A). */
/* name: only this file uses it; no FM Towns name, so static. */
static struct Creature near *LootCreature;

/* Empties container cont onto the floor at its position (MapObj_X, MapObj_Y for a static
   container). For each object, when owner is nonzero and the container can be owned, the
   container is given owner. Returns 1 if there was anything inside. */
char far drop_link_chain(struct Object far *cont, int owner)
{
    struct Object far *obj;
    struct Object far *next;
    int z;
    int x, y;

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
            /* UW1: the owner test and store are on the container, not the item */
            if (owner && ComObjData[OBJ_ITEM(cont)].can_own)
                cont->ol.f.owner = owner;
            put_at(x, y, z, obj, 6, 0);
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
   treasure type is rolled from -(33 - 3l) to 6, l the dungeon level; anything below 0
   becomes type 0, so deeper levels give better treasure more often. The type's value
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
    type = rand() % (40 - PlayerLevel * 3) - (33 - PlayerLevel * 3);
    if (type < 0)
        type = 0;
    if ((value = (unsigned char)ComObjData[type + FIRST_TREASURE].value) == 0)
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
        if (OBJ_MINOR(obj) == MINOR_MISSILE && (unsigned char)Missile[OBJ_INCLASS(obj)].ammo == 0xC0)
            obj->ol.f.link = rand() % 8 + 4;
        Obj_Add(&npc->ol.link, obj);
    }
}

/* Adds each of the two other items with its chance in 16, at a random quality as for
   weapons. */
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
            obj->qn.f.quality = quality;
            Obj_Add(&npc->ol.link, obj);
        }
    }
}

/* Gives npc its loot once and sets OBJ_HAS_INV so it is not given again. */
void far generate_inventory(struct Object far *npc)
{
    int minor, index;

    if (OBJ_HAS_INV(npc))
        return;
    minor = OBJ_MINOR(npc);
    index = OBJ_INCLASS(npc);
    LootCreature = &Creature[(minor << 4) + index];
    generate_treasure(npc);
    generate_food(npc);
    generate_weapons(npc);
    generate_equipment(npc);
    SET_HAS_INV(npc, 1);
}
