/* target: ovr101 */
/* opts: -mm -1 -G -O -Y -d */
/* Creature class data: the critters table of DATA\OBJECTS.DAT and the starting state of
   a new critter. The whole of UW1's DOS overlay ovr101 (UW2's ovr104), in original order.

   What it does in the game: every critter type's fixed properties (struct Creature,
   critter.h: hit points, attributes, speed, senses, attacks, spells, loot and so on)
   live in Creature[64], indexed by the low 6 bits of the item id. creature_init reads
   the table and creature_class_data finds the active object's entry. creature_obj_init
   gives ActiveObj, a newly made critter, its starting state (seg038 calls it with
   ActiveObj pointed at the new object).

   UW1 against UW2: there is no init_this_critter taking an object; creature_obj_init
   does its work on ActiveObj itself and returns 1. A new critter starts with animation
   sequence 0x20 (UW2 0), and its terrain bits are not cleared.

   Data owned: Creature[], and cst, cr_class and cr_type, the entry, class and type last
   looked up.

   UW1 has no symbol-bearing build: the names are UW2's (the FM Towns symbol table), the
   routines being the same.
   Name: inferred (creature_init and creature_class_data: the class prefix). */

#include <stdio.h>
#include <stdlib.h>
#include "critter.h"
#include "object.h"

/* match: this file's _BSS, in UW1 DS:4A30..5653 (UW2 DS:492A..554D), laid out by name
   (tools/bssorder.py): cr_type 931, cst 955, cr_unused 971, Creature 979, cr_class 1019.
   Nothing uses DS:4A34..4A51, but it lies between cst and Creature. */
/* name: cr_class and cr_type are the FM Towns names (its creature_class_data_ sets them
   as this one does), and their keys put them exactly where UW2 has them, either side of
   Creature. */
uint16 cr_type;                         /* DS:4A30, the creature's type within its class */
struct Creature *cst;                   /* DS:4A32 */
static char cr_unused[0x1E];            /* DS:4A34, never used */
struct Creature Creature[NUM_CREATURES];         /* DS:4A52 */
uint16 cr_class;                        /* DS:5652, the creature's class */

/* Read the 64 critter records, 48 bytes each, from the open OBJECTS.DAT. */
void far creature_init(FILE *fd)
{
    fread(Creature, sizeof(struct Creature), NUM_CREATURES, fd);
}

/* Write the critter table back to a file. Only its overlay stub refers to it in the IDA
   listing, so nothing seems to call it. */
/* name: creature_init's counterpart, which FM Towns lacks; provisional, UW2Decomp's
   creature_save_ovr104_17 with UW1's address. Its tools/bssorder.py key (427) puts it in
   the EXE's overlay stub order, between creature_init (235) and creature_obj_init (491);
   the listing's ovr101_17 (567) would not. */
void far creature_save_ovr101_17(FILE *fd)
{
    fwrite(Creature, sizeof(struct Creature), NUM_CREATURES, fd);
}

/* The Creature entry of ActiveObj: minor class * 16 + type within the class, the same as
   the low 6 bits of the item id. */
struct Creature * far creature_class_data(void)
{
    cr_class = OBJ_MINOR(ActiveObj);
    cr_type = OBJ_INCLASS(ActiveObj);
    return &Creature[cr_class * 16 + cr_type];
}

/* A new critter's starting state, for ActiveObj: current and home tile fields set to
   32, 32 (the middle of the map; presumably the caller places it), hit points between
   half and about 1.2 times its type's average (avghit * (16 + rand() % 24) / 32), the
   fine heading from its coarse one, goal 8, attitude 2 (mellow), walk rate 4,
   sequence 0x20, level pitch (0x10), and every other AI field cleared. Always returns 1. */
/* name: symbols.tsv's (stub 59A8:002A); UW2's creature_obj_init calls init_this_critter
   for ActiveObj, and UW1 has that body here. */
char far creature_obj_init(void)
{
    register int x;
    register int y;

    x = 0x20;
    y = 0x20;
    SET_HOMEX(ActiveObj, x);
    SET_HOMEY(ActiveObj, y);
    ActiveObj->qn.f.quality = x;
    ActiveObj->ol.f.owner = y;
    cst = &Creature[OBJ_INMAJOR_NOSHIFT(ActiveObj)];
    ActiveObj->hp = (cst->avghit * (rand() % 0x18 + 0x10)) / 0x20;
    ActiveObj->heading = OBJ_HEADING(ActiveObj) << 5;
    SET_GOAL(ActiveObj, GOAL_MILL);
    SET_GTARG(ActiveObj, 0);
    SET_OLDGOAL(ActiveObj, GOAL_STAND);
    SET_DESTX(ActiveObj, 0);
    SET_DESTY(ActiveObj, 0);
    SET_TARGETZ(ActiveObj, 0);
    SET_NOHEAL(ActiveObj, 0);
    SET_POWERFUL(ActiveObj, 0);
    SET_B0D_11(ActiveObj, 0);
    SET_TEMP(ActiveObj, 0);
    SET_B18_5(ActiveObj, 0);
    SET_ATKFRAME(ActiveObj, 0);
    SET_BIN(ActiveObj, 0);
    SET_RATE(ActiveObj, 4);
    SET_SEQ(ActiveObj, SEQ_STAND);
    SET_FRAME(ActiveObj, 0);
    SET_PITCH(ActiveObj, 0x10);
    SET_GRAVITY(ActiveObj, 0);
    SET_SPEED(ActiveObj, 0);
    ActiveObj->b11 = 0;
    ActiveObj->last_hit = 0;
    SET_B15_7(ActiveObj, 0);
    SET_B18_7(ActiveObj, 0);
    SET_B18_6(ActiveObj, 0);
    SET_PATH(ActiveObj, 0);
    SET_B15_6(ActiveObj, 0);
    ActiveObj->whoami = 0;
    SET_B19_0(ActiveObj, 0);
    SET_B19_1(ActiveObj, 0);
    SET_B19_4(ActiveObj, 0);
    SET_B19_5(ActiveObj, 0);
    SET_ALLY(ActiveObj, 0);
    SET_FED(ActiveObj, 0);
    SET_HAS_INV(ActiveObj, 0);
    SET_TALKEDTO(ActiveObj, 0);
    SET_ATTITUDE(ActiveObj, ATT_MELLOW);
    SET_LONER(ActiveObj, 0);
    SET_CAST(ActiveObj, 0);
    return 1;
}
