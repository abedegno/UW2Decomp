/* target: ovr104 */
/* opts: -mm -1 -G -O -Y -d */
/* Creature class data: the critters table of DATA\OBJECTS.DAT and the starting state of
   a new critter. The whole of DOS overlay ovr104.

   What it does in the game: every critter type's fixed properties (struct Creature,
   critter.h: hit points, attributes, speed, senses, attacks, spells, loot and so on)
   live in Creature[64], indexed by the low 6 bits of the item id (the player's own
   entry is Creature[63]). OBJCLASS.C's init_objects calls creature_init to read the
   table and get_class_data calls creature_class_data to find the active object's entry.
   init_this_critter gives a newly made critter its starting state; MAPADDR.C's
   CreateObj calls it for new creatures, and creature_obj_init (from SPELLS2.C's
   creature summoning and BABLHACK.C) calls it for ActiveObj.

   Data owned: Creature[], and cst, cr_class and cr_type, the entry, class and type last
   looked up.

   Neighbours: AI.C, PATHFIND.C, CRITTIME.C and the combat code read Creature[] through
   mycst and similar pointers.
   Name: inferred (creature_init and creature_class_data: the class prefix). */

#include <stdio.h>
#include <stdlib.h>
#include "critter.h"
#include "object.h"

/* match: this file's _BSS, DS:492A..554D, laid out by name (tools/bssorder.py): cr_type
   931, cst 955, cr_unused 971, Creature 979, cr_class 1019. Nothing uses DS:492E..494B,
   but it lies between cst and Creature. (DS:4928..4929, also never used, is left out: it
   may as well be ovr103's.) */
/* name: cr_class and cr_type are the FM Towns names (its creature_class_data_ sets them
   as this one does), and their keys put them exactly where UW2 has them, either side of
   Creature. */
uint16 cr_type;                         /* DS:492A, the creature's type within its class */
struct Creature *cst;                   /* DS:492C */
static char cr_unused[0x1E];            /* DS:492E, never used */
struct Creature Creature[NUM_CREATURES];         /* DS:494C */
uint16 cr_class;                        /* DS:554C, the creature's class */

/* Read the 64 critter records, 48 bytes each, from the open OBJECTS.DAT. */
void far creature_init(FILE *fd)
{
    fread(Creature, 0x30, NUM_CREATURES, fd);
}

/* Write the critter table back to a file. Only its overlay stub refers to it in the IDA
   listing, so nothing in UW2 seems to call it. */
/* name: creature_init's counterpart, which FM Towns lacks; the name is provisional (IDA's
   ovr104_17), chosen so that its tools/bssorder.py key puts it in the EXE's overlay stub
   order. */
void far creature_save_ovr104_17(FILE *fd)
{
    fwrite(Creature, 0x30, NUM_CREATURES, fd);
}

/* The Creature entry of ActiveObj: minor class * 16 + type within the class, the same as
   the low 6 bits of the item id. */
struct Creature * far creature_class_data(void)
{
    cr_class = OBJ_MINOR(ActiveObj);
    cr_type = ActiveObj->id & ID_INCLASS;
    return &Creature[cr_class * 16 + cr_type];
}

void far creature_obj_init(void)
{
    init_this_critter(ActiveObj);
}

/* A new critter's starting state: current and home tile fields set to 32, 32 (the
   middle of the map; presumably the caller places it), hit points between half and about
   1.2 times its type's average (avghit * (16 + rand() % 24) / 32), the fine heading from
   its coarse one, goal 8 (mill about near home), attitude 2 (mellow), walk rate 4, level pitch
   (0x10), and every other AI field cleared. Always returns 1. */
/* name: FM Towns identifies the target table's InitialiseCritterValues as
   init_this_critter: creature_obj_init calls it and MAPADDR's CreateObj calls it for new
   creatures. */
char far init_this_critter(struct Object far *obj)
{
    register int x;
    register int y;

    x = 0x20;
    y = 0x20;
    SET_HOMEX(obj, x);
    SET_HOMEY(obj, y);
    obj->qn.f.quality = x;
    obj->ol.f.owner = y;
    cst = &Creature[obj->id & ID_INMAJOR];
    obj->hp = (cst->avghit * (rand() % 0x18 + 0x10)) / 0x20;
    obj->heading = OBJ_HEADING(obj) << 5;
    SET_GOAL(obj, 8);
    SET_GTARG(obj, 0);
    SET_OLDGOAL(obj, 0);
    SET_DESTX(obj, 0);
    SET_DESTY(obj, 0);
    SET_TARGETZ(obj, 0);
    SET_NOHEAL(obj, 0);
    SET_POWERFUL(obj, 0);
    SET_B0D_11(obj, 0);
    SET_TEMP(obj, 0);
    SET_B18_5(obj, 0);
    SET_ATKFRAME(obj, 0);
    SET_BIN(obj, 0);
    SET_RATE(obj, 4);
    SET_SEQ(obj, 0);
    SET_FRAME(obj, 0);
    SET_PITCH(obj, 0x10);
    SET_GRAVITY(obj, 0);
    SET_SPEED(obj, 0);
    obj->b11 = 0;
    obj->last_hit = 0;
    SET_B15_7(obj, 0);
    SET_B18_7(obj, 0);
    SET_B18_6(obj, 0);
    SET_PATH(obj, 0);
    SET_B15_6(obj, 0);
    obj->whoami = 0;
    SET_B19_0(obj, 0);
    SET_B19_1(obj, 0);
    SET_B19_4(obj, 0);
    SET_B19_5(obj, 0);
    SET_ALLY(obj, 0);
    SET_FED(obj, 0);
    SET_HAS_INV(obj, 0);
    SET_TALKEDTO(obj, 0);
    SET_ATTITUDE(obj, 2);
    SET_LONER(obj, 0);
    SET_CAST(obj, 0);
    SET_TERRAIN(obj, 0);
    return 1;
}
