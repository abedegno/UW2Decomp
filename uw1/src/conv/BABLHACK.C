/* target: ovr094 */
/* opts: -mm -1 -G -O -Y -d */
/* Conversation built-ins that reach into the game: setting critters' attitudes, skills,
   game variables and quest flags, objects taken and placed, doors, and reading or writing
   an object's fields: the whole of UW1's DOS overlay ovr094 (UW2's ovr096, whose babl_hack,
   pit fighter, animation, teleport, transform_talker and x_clock built-ins UW1 does not
   have), in original order.

   The conversation code binds each of these to the script import of the same name; the
   script calls them through CALLI. They act on talking_to, the NPC being talked to, and
   on player. Each built-in gets a far pointer to the top of the conversation stack,
   args[0] being the argument count. args[-1] is what UW-Formats calls arg1 (the last
   value pushed before the count), args[-2] arg2, and so on. Each argument is the address
   of a conversation variable, which getmem reads and getmem_addr points at.

   UW1 against UW2: the player record is laid out differently (player.h has UW1's); x_skills
   has no skill-point advance; x_traps keeps its 64 variables in the player record rather
   than going through set_numbered_variable; quests 0..31 are the bits of one long, 32..35
   bytes; place_object ignores Obj_Rem's result; CloseDoor takes the door alone;
   x_obj_stuff sets bit 9 of a link it writes and masks it off one it reads; x_obj_pos has
   no tile-reading mode.

   Name: descriptive (map/filenames.tsv: conversation built-ins reaching into the game,
   after babl_hack). UW1 has no symbol-bearing build: the function names are UW2's (the
   FM Towns symbol table), the routines being the same and the script imports having the
   same names. The file owns no data. */

#include <stdlib.h>
#include <time.h>
#include "combat.h"
#include "conv.h"
#include "critter.h"
#include "event.h"
#include "inv.h"
#include "map.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"
#include "uw2.h"

/* set_attitude(arg2 whoami, arg1 attitude), through gronk_whoami. */
char far set_mob_att(struct Object far *npc, int att)
{
    SET_ATTITUDE(npc, att);
    return 0;
}

/* set_attitude(arg2 whoami, arg1 attitude): the first active critter with that whoami
   takes the attitude (set_mob_att). */
void far set_attitude(int16 far *args)
{
    int att;
    int who;

    att = getmem(args[-1]);
    who = getmem(args[-2]);
    gronk_whoami(who, 0, att, (WhoamiFn)set_mob_att);
}

/* set_race_attitude(arg3 race, arg2 attitude, arg1 range): set the attitude of every
   critter of the talker's item type and the given race, not a loner, within range tiles
   of the talker's home (clipped to 1..63). */
void far set_race_attitude(int16 far *args)
{
    int range, att, race;
    int y;
    int x0, y0, x1, y1;
    union Link far *head;
    struct Object far *obj;
    int item, x;

    range = getmem(args[-1]);
    att = getmem(args[-2]);
    race = getmem(args[-3]);
    item = OBJ_ITEM(talking_to);
    x0 = OBJ_HOMEX(talking_to) - range;
    if (x0 < 1)
        x0 = 1;
    y0 = OBJ_HOMEY(talking_to) - range;
    if (y0 < 1)
        y0 = 1;
    x1 = OBJ_HOMEX(talking_to) + range;
    if (x1 >= MAP_SIZE)
        x1 = 0x3F;
    y1 = OBJ_HOMEY(talking_to) + range;
    if (y1 >= MAP_SIZE)
        y1 = 0x3F;
    for (y = y0; y <= y1; y++)
        for (x = x0; x <= x1; x++) {
            head = &Map_GetAddr(x, y)->objects;
            for (obj = Obj_PtrTMem(head); obj; obj = Obj_PtrTMem(&obj->qn.link))
                if (OBJ_ITEM(obj) == item && !OBJ_LONER(obj)
                    && Creature[OBJ_INMAJOR(obj)].race == race)
                    SET_ATTITUDE(obj, att);
        }
}

/* x_skills(arg2 skill, arg1 value): 10000 calls get_skill; 0..30 sets the skill;
   anything else only reads it. Returns the skill's value. */
int far x_skills(int16 far *args)
{
    int skill, val;

    skill = getmem(args[-2]);
    val = getmem(args[-1]);
    if (val == 10000)
        get_skill(skill);
    else if (val >= 0 && val <= 30)
        player->skills[skill] = val;
    return player->skills[skill];
}

/* x_traps(arg2 variable, arg1 value): set one of the 64 game variables in the player
   record (the ones traps use, inferred from the name) when the value is 0..0x3F;
   returns its value. */
int far x_traps(int16 far *args)
{
    int var, val;

    var = getmem(args[-2]);
    val = getmem(args[-1]);
    if (val >= 0 && val <= 0x3F)
        player->game_vars[var] = val;
    return player->game_vars[var];
}

/* place_object(arg3 object, arg2 x, arg1 y): take an object from the talker and put it
   at tile (x, y), or at the player's feet if x is negative. Returns 1 when placed (or
   when it was in the talker's inventory but could not be unlinked), 0 when off the map
   or there is no room. */
int far place_object(int16 far *args)
{
    int index;
    struct Object far *obj;
    struct Tile far *tile;
    union Link far *link;
    int x, y;

    index = getmem(args[-3]);
    obj = Obj_IntTMem(index);
    x = getmem(args[-2]);
    y = getmem(args[-1]);
    for (link = &talking_to->ol.link; link->f.index && link->f.index != index;
         link = &Obj_PtrTMem(link)->qn.link)
        ;
    if (link->f.index)
        Obj_Rem(&talking_to->ol.link, obj);
    if (x < 0) {
        stay_centered = 1;
        put_at(OBJ_HOMEX(ThePlayer) << 3, OBJ_HOMEY(ThePlayer) << 3, OBJ_Z(ThePlayer), obj, 6, 1);
        stay_centered = 0;
        return 1;
    }
    if (x < 1 || x >= MAP_SIZE || y < 1 || y >= MAP_SIZE)
        return 0;
    tile = Map_GetAddr(x, y);
    SET_Z(obj, tile->height << 3);
    if (can_place(OBJ_ITEM(obj), index, x << 3, y << 3, tile->height << 3, 1,
                  ComObjData[OBJ_ITEM(obj)].radius + 4)) {
        Obj_AddEnd(&tile->objects, obj);
        obj_deal(obj, x, y, 1);
        return 1;
    }
    return 0;
}

/* take_from_npc_inv(arg1 n): the object index n links into the talker's inventory (0
   the first), or 0 past the end. Nothing is moved. */
int far take_from_npc_inv(int16 far *args)
{
    union Link far *link;
    int i, n;

    n = getmem(args[-1]);
    link = &talking_to->ol.link;
    for (i = 0; i < n && link->f.index; i++)
        link = &Obj_PtrTMem(link)->qn.link;
    return link->f.index;
}

/* add_to_npc_inv(arg1 object index): appends the object to the talker's inventory. */
void far add_to_npc_inv(int16 far *args)
{
    int index;

    index = getmem(args[-1]);
    Obj_AddEnd(&talking_to->ol.link, Obj_IntTMem(index));
}

/* remove_talker: take the talker off the map (Obj_Punt). */
void far remove_talker(void)
{
    struct Tile far *tile;

    tile = Map_GetAddr(OBJ_HOMEX(talking_to), OBJ_HOMEY(talking_to));
    Obj_Punt(&tile->objects, talking_to, 1);
}

/* set_quest(arg2 quest, arg1 value). Quests 0..31 are bits of one long, set for any
   non-zero value; 32..35 are the bytes quest_bytes[0..3]; others are ignored. The bit is
   an int shifted and then widened with its sign, so as written only quests 0..15 work, and
   setting 15 also sets bits 16..31 (clearing 15 clears them). */
void far set_quest(int16 far *args)
{
    int quest, val;

    quest = getmem(args[-2]);
    val = getmem(args[-1]);
    if (quest >= 0) {
        if (quest < FIRST_QUEST_BYTE) {
            if (val)
                player->quests |= 1 << quest;
            else
                player->quests &= ~(1 << quest);
        } else if (quest < FIRST_QUEST_BYTE + 4)
            player->quest_bytes[quest - FIRST_QUEST_BYTE] = val;
    }
}

/* get_quest(arg1 quest): as set_quest (a bit quest gives 0 or 1), and above 35 the
   player record's byte 0x6D; negative gives 0. */
int far get_quest(int16 far *args)
{
    int quest;

    quest = getmem(args[-1]);
    if (quest < 0)
        return 0;
    if (quest < FIRST_QUEST_BYTE)
        return (player->quests & (1 << getmem(args[-1]))) != 0;
    if (quest < FIRST_QUEST_BYTE + 4)
        return player->quest_bytes[quest - FIRST_QUEST_BYTE];
    return player->talismans;
}

/* sex: arg2 for a male player, arg1 for a female one (player->female is 0 or 1). */
int far sex(int16 far *args)
{
    return getmem(args[-2 + player->female]);
}

/* gronk_door(arg3 x, arg2 y, arg1 how): open (0), close (1) or toggle (2) the door at
   tile (x, y): the first MAJOR_RECT object of minor 0 there, or else an animated object
   of minor 0, index 0xF (Obj_InList). Returns 0 when there is none. */
int far gronk_door(int16 far *args)
{
    union Link far *head;
    struct Object far *door;
    int ox, oy;

    head = &Map_GetAddr(getmem(args[-3]), getmem(args[-2]))->objects;
    if ((door = Obj_InList(&head, 0, MAJOR_RECT, 0, -1)) == 0
        && (door = Obj_InList(&head, 0, MAJOR_ANIMOBJ, 0, 0xF)) == 0)
        return 0;
    ox = MapObj_X;
    oy = MapObj_Y;
    MapObj_X = getmem(args[-3]);
    MapObj_Y = getmem(args[-2]);
    switch (getmem(args[-1])) {
    case 0:
        OpenDoor(0, door);
        break;
    case 1:
        /* UW1: CloseDoor takes the door alone (UW2's takes two) */
        ((void (far *)(struct Object far *))CloseDoor)(door);
        break;
    case 2:
        ToggleDoor(0, door);
        break;
    }
    MapObj_X = ox;
    MapObj_Y = oy;
    return 1;
}

/* x_obj_stuff(arg9 object, arg8 set, arg7 heading, arg6 owner, arg5 flags, arg4 link,
   arg3 flag10, arg2 flag9, arg1 quality): with set non-zero write the fields from the
   variables, else read them into the variables; a variable holding -1 is left alone.
   The heading is skipped for MAJOR_RECT objects and those with render type 2. Read
   back, flag10 and flag9 are the raw bits 0x400 and 0x200, not 0 or 1. */
void far x_obj_stuff(int16 far *args)
{
    int16 far *heading;
    int16 far *owner;
    int16 far *flags;
    int16 far *link;
    int16 far *flag10;
    int16 far *flag9;
    int16 far *quality;
    struct Object far *obj;

    heading = getmem_addr(args[-7]);
    owner = getmem_addr(args[-6]);
    flags = getmem_addr(args[-5]);
    link = getmem_addr(args[-4]);
    flag10 = getmem_addr(args[-3]);
    flag9 = getmem_addr(args[-2]);
    quality = getmem_addr(args[-1]);
    obj = Obj_IntTMem(getmem(args[-9]));
    if (getmem(args[-8])) {
        if (*heading != -1 && OBJ_MAJOR(obj) != MAJOR_RECT && ComObjData[OBJ_ITEM(obj)].render != 2)
            SET_HEADING(obj, *heading);
        if (*owner != -1)
            obj->ol.f.owner = *owner;
        if (*flags != -1)
            SET_FLAGS(obj, *flags);
        if (*link != -1)
            obj->ol.f.link = LINK_SPECIAL | *link;     /* UW1: sets bit 9 of the link */
        if (*flag10 != -1)
            SET_FLAG10(obj, *flag10);
        if (*flag9 != -1)
            SET_FLAG9(obj, *flag9);
        if (*quality != -1)
            obj->qn.f.quality = *quality;
    } else {
        if (*heading != -1 && OBJ_MAJOR(obj) != MAJOR_RECT && ComObjData[OBJ_ITEM(obj)].render != 2)
            *heading = OBJ_HEADING(obj);
        if (*owner != -1)
            *owner = obj->ol.f.owner;
        if (*flags != -1)
            *flags = OBJ_FLAGS(obj);
        if (*link != -1)
            *link = obj->ol.f.link & 0x1FF;     /* UW1: without bit 9 */
        if (*flag10 != -1)
            *flag10 = obj->id & ID_FLAG10;
        if (*flag9 != -1)
            *flag9 = obj->id & ID_FLAG9;
        if (*quality != -1)
            *quality = obj->qn.f.quality;
    }
}

/* x_obj_pos(arg5 object, arg4 mode, arg3 x, arg2 y, arg1 z): mode 1 sets an object's
   fine position and height (a z above 0x7F means the floor height of tile (x, y)),
   anything else (UW1 has no mode 2) reads its fine position and height. A
   variable holding -1 is left alone. */
void far x_obj_pos(int16 far *args)
{
    int16 far *x;
    int16 far *y;
    int16 far *z;
    struct Object far *obj;

    x = getmem_addr(args[-3]);
    y = getmem_addr(args[-2]);
    z = getmem_addr(args[-1]);
    obj = Obj_IntTMem(getmem(args[-5]));
    if (getmem(args[-4])) {
        if (*x != -1)
            SET_FINEX(obj, *x);
        if (*y != -1)
            SET_FINEY(obj, *y);
        if (*z != -1) {
            if (*z > 0x7F)
                SET_Z(obj, Map_GetAddr(*x, *y)->height << 3);
            else
                SET_Z(obj, *z);
        }
    } else {
        if (*x != -1)
            *x = OBJ_FINEX(obj);
        if (*y != -1)
            *y = OBJ_FINEY(obj);
        if (*z != -1)
            *z = OBJ_Z(obj);
    }
}
