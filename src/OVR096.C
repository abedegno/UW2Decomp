/* target: ovr096 */
/* opts: -mm -1 -G -O -Y -d */
/* Conversation built-ins that reach into the game: the "babl_hack" grab bag (the pit
   fighters of the arena, Jospur's debt, recharging a wand, trade adjustments), setting a
   critter's animation and attitude, teleports done once a conversation ends, skills,
   traps and quest flags, the clock, objects given, taken and placed, doors, and reading or
   writing an object's fields: the whole of DOS overlay ovr096, in original order.
   Function and global names are the originals from the FM Towns symbol table, which lists
   the same 27 functions in the same order; the source file's own name is not known.

   Each built-in gets a far pointer just past its arguments on the conversation stack:
   args[-1] is the first argument, args[-2] the second and so on. Each argument is the
   address of a conversation variable, which getmem reads and getmem_addr points at. */

#include <stdlib.h>
#include <time.h>
#include "conv.h"
#include "critter.h"
#include "event.h"
#include "map.h"
#include "object.h"
#include "player.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"
#include "uw2.h"

/* Jospur's debt to the player for fights won in the pits: quest 133. */
#define JOSPUR_DEBT     quest_bytes[0x85 - 0x80]

extern union Link Inventory[];
extern void (far *npp_func)();
extern unsigned char stay_centered;
extern long lastDurCheck;

struct Object far * far CreateObj(int item, char mobile);
unsigned char far can_place(int item, int index, int x, int y, int z, char flier, char dist);
unsigned char far put_at(int x, int y, int z, struct Object far *obj, int range,
                         unsigned char nocull);
struct Object far * far obj_deal(struct Object far *obj, int x, int y, char how);
int far useNSpellCharges(struct Object far *obj, int n);
void far gronk_whoami(int whoami, unsigned char all, int arg,
                      char (far *fn)());
void far do_teleport(struct Object far *who, int x, int y, int level);
char far teleport_critter(struct Object far *critter, int x, int y, int how);
void far set_numbered_variable(int var, int how, int val);

/* Set when the player runs from a pit fight, read and cleared by babl_hack. */
char running_away = 0;
/* Where teleport_player and teleport_talker asked to go, done by do_babl_teleport once
   the conversation is over; -1 for nothing pending. */
static char tele_level = -1, tele_x = 0, tele_y = 0;
static char talker_x = -1, talker_y = -1;

/* Run on the next level change: the player is now in the arena. */
void far set_me_inarena(void)
{
    player->in_pits = 1;
    npp_func = 0;
}

char far set_me_loner(struct Object far *npc, char loner)
{
    SET_LONER(npc, loner);
    return 0;
}

/* Odd jobs a conversation asks for by number. The cases end in `return 0` rather than
   `break`: in DOS a failed test jumps short to a `jmp` to the shared final return, which
   Turbo C only leaves in place when the tails are merged returns; a `break` gets its
   jumps threaded straight to the end and the function comes out 3 bytes long. */
int far babl_hack(int far *args)
{
    int mode;
    int i, n;

    mode = getmem(args[-1]);
    switch (mode) {
    case 10: {                          /* is the player wearing the Guardian's signet
                                           ring on either hand? */
        struct Object far *ring;

        ring = Obj_PtrTMem(&Inventory[9]);
        if (ring && OBJ_ITEM(ring) == ITEM_GUARDIAN_SIGNET_RING)
            return 1;
        ring = Obj_PtrTMem(&Inventory[10]);
        if (ring && OBJ_ITEM(ring) == ITEM_GUARDIAN_SIGNET_RING)
            return 1;
        return 0;
    }
    case 9:
        return fudge = getmem(args[-2]);
    case 8:
        return greed *= getmem(args[-2]);
    case 7:                             /* recharge a wand */
        return useNSpellCharges(Obj_IntTMem(getmem(args[-2])), getmem(args[-3]));
    case 0:                             /* challenge the talker to a pit fight */
        npp_func = set_me_inarena;
        player->pit_fighters[0] = Obj_MemTPtr(talking_to);
        return 0;
    case 1:
        if (running_away) {
            running_away = 0;
            return 1;
        }
        return 0;
    case 2: {                           /* set up a fight in the arena */
        int power, side;
        int dx, dy;
        int row, depth;
        int count;
        int debt;
        struct Object far *fighter;

        power = getmem(args[-4]);
        side = getmem(args[-3]);
        n = getmem(args[-2]);
        dx = 1;
        dy = 1;
        count = 0;
        srand(time(0));
        if (side == 1 || side == 2)
            dx = -1;
        if (side > 1)
            dy = -1;
        if (n == 5) {
            fighter = place_pitfighter(0x63, dx * 6 + 0x1F, dy * 4 + 0x1F);
            if (fighter)
                count++;
            player->pit_fighters[n] = Obj_MemTPtr(fighter);
            n--;
        }
        for (row = 0; row < 3; row++) {
            for (i = 0; i <= row; i++) {
                depth = row - i;
                fighter = place_pitfighter(power, dx * (i + 4) + 0x1F, dy * (depth + 4) + 0x1F);
                if (fighter)
                    count++;
                n--;
                player->pit_fighters[n] = Obj_MemTPtr(fighter);
                if (n == 0)
                    goto placed;
            }
        }
placed:
        switch (count) {
        case 2: debt = 8; break;
        case 3: debt = 12; break;
        case 4: debt = 20; break;
        case 5: debt = 40; break;
        default: debt = 0; break;
        }
        player->JOSPUR_DEBT = debt;
        npp_func = set_me_inarena;
        return count;
    }
    case 3:
        i = player->JOSPUR_DEBT;
        player->JOSPUR_DEBT = 0;
        return i;
    case 4:
        return player->in_pits;
    case 5:
        gronk_whoami(getmem(args[-2]), 0, 1, set_me_loner);
        return 0;
    case 6:
        return speech_available();
    }
    return 0;
}

/* Make a pit fighter at (x, y); the higher power, the likelier a strong one. */
struct Object far * far place_pitfighter(int power, int x, int y)
{
    struct Object far *obj;
    struct Object far *save;
    struct Tile far *tile;
    unsigned char strong;
    char roll;
    int item;

    if (rand() % 16)
        item = rand() % 2 + ITEM_HUMAN_78;
    else
        item = rand() % 3 + ITEM_HUMAN_75;
    if (rand() % 3 < power) {
        strong = 1;
        if ((roll = rand() % 5) < 2) {
            item = roll ? ITEM_HUMAN_7B : ITEM_GREAT_TROLL;
            strong = 0;
        }
    }
    tile = Map_GetAddr(x, y);
    if (can_place(item, 0, (x << 3) + 3, (y << 3) + 3, tile->height << 3, 1, 3)) {
        obj = CreateObj(item, 1);
        SET_FINEX(obj, 3);
        SET_FINEY(obj, 3);
        save = ActiveObj;
        ActiveObj = obj;
        creature_obj_init();
        ActiveObj = save;
        SET_HOMEX(obj, x);
        obj->qn.f.quality = x;
        SET_HOMEY(obj, y);
        obj->ol.f.owner = y;
        SET_Z(obj, tile->height << 3);
        obj->whoami = 0x66;
        SET_POWERFUL(obj, strong);
        SET_TEMP(obj, 1);
        SET_ATTITUDE(obj, 0);
        SET_GOAL(obj, 5);
        SET_GTARG(obj, 1);
        SET_LONER(obj, 1);
        Obj_Add(&tile->objects, obj);
        editchng(2);
        return obj;
    }
    return 0;
}

char far set_mob_seq_n_frame(struct Object far *npc, int n)
{
    int frame = n & 7;
    int seq = n >> 3;

    SET_SEQ(npc, seq);
    SET_FRAME(npc, frame);
    return 0;
}

void far set_sequence(int far *args)
{
    int frame;
    int seq;
    int who;

    frame = getmem(args[-1]);
    seq = getmem(args[-2]);
    who = getmem(args[-3]);
    gronk_whoami(who, 0, seq << 3 | frame & 7, set_mob_seq_n_frame);
}

int far x_exp(int far *args)
{
    int n;

    n = getmem(args[-1]);
    player_get_exp(n);
    set_workspace();
    return player->exp >> 4;
}

void far teleport_player(int far *args)
{
    int x, y, level;

    x = getmem(args[-3]);
    y = getmem(args[-2]);
    level = getmem(args[-1]);
    tele_level = level;
    tele_x = x;
    tele_y = y;
}

int far teleport_talker(int far *args)
{
    talker_x = getmem(args[-2]);
    talker_y = getmem(args[-1]);
    return 1;
}

void far do_babl_teleport(void)
{
    if (tele_level >= 0) {
        NewPlyFade = 0;
        do_teleport(ThePlayer, tele_x, tele_y, tele_level);
        new_player_pos();
        NewPlyFade |= 1;
        tele_level = -1;
    }
    if (talker_x >= 0) {
        teleport_critter(talking_to, talker_x, talker_y, PlayerLevel);
        talker_x = -1;
    }
}

char far set_mob_att(struct Object far *npc, int att)
{
    SET_ATTITUDE(npc, att);
    return 0;
}

void far set_attitude(int far *args)
{
    int att;
    int who;

    att = getmem(args[-1]);
    who = getmem(args[-2]);
    gronk_whoami(who, 0, att, set_mob_att);
}

/* Set the attitude of every critter of the talker's kind and the given race within
   range of the talker's home. */
void far set_race_attitude(int far *args)
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

int far x_skills(int far *args)
{
    int skill, val;

    skill = getmem(args[-2]);
    val = getmem(args[-1]);
    if (val > 10000) {
        if (player->skill_points > 0 && grant_skill_advance(skill)) {
            player->skill_points--;
            return 1;
        }
        return 0;
    }
    if (val == 10000)
        get_skill(skill);
    else if (val >= 0 && val <= 30)
        player->skills[skill] = val;
    return player->skills[skill];
}

int far x_traps(int far *args)
{
    int var, val;

    var = getmem(args[-2]);
    val = getmem(args[-1]);
    if (val >= 0 && val <= 0x3FF)
        set_numbered_variable(var, 2, val);
    return get_numbered_variable(var);
}

/* Take an object from the talker and put it at (x, y), or at the player's feet if x is
   negative. */
int far place_object(int far *args)
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
    if (link->f.index && !Obj_Rem(&talking_to->ol.link, obj))
        return 1;
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

int far take_from_npc_inv(int far *args)
{
    union Link far *link;
    int i, n;

    n = getmem(args[-1]);
    link = &talking_to->ol.link;
    for (i = 0; i < n && link->f.index; i++)
        link = &Obj_PtrTMem(link)->qn.link;
    return link->f.index;
}

void far add_to_npc_inv(int far *args)
{
    int index;

    index = getmem(args[-1]);
    Obj_AddEnd(&talking_to->ol.link, Obj_IntTMem(index));
}

void far transform_talker(int far *args)
{
    transform_creature(talking_to, getmem(args[-4]), getmem(args[-3]), getmem(args[-2]),
                       getmem(args[-1]));
}

void far remove_talker(void)
{
    struct Tile far *tile;

    tile = Map_GetAddr(OBJ_HOMEX(talking_to), OBJ_HOMEY(talking_to));
    Obj_Punt(&tile->objects, talking_to, 1);
}

void far set_quest(int far *args)
{
    int quest, val;

    quest = getmem(args[-2]);
    val = getmem(args[-1]);
    if (quest >= 0) {
        if (quest < 0x80)
            player->quests[(unsigned)quest >> 2] =
                (player->quests[(unsigned)quest >> 2] & ~(1 << (quest & 3)))
                + (val << (quest & 3));
        else if (quest < 0x90)
            player->quest_bytes[quest - 0x80] = val;
    }
}

int far get_quest(int far *args)
{
    int quest;

    quest = getmem(args[-1]);
    if (quest < 0)
        return 0;
    if (quest < 0x80)
        return (player->quests[(unsigned)quest >> 2] & (1 << (quest & 3))) >> (quest & 3);
    if (quest < 0x90)
        return player->quest_bytes[quest - 0x80];
    return player->bF6;
}

int far x_clock(int far *args)
{
    int clock;
    int val;

    clock = getmem(args[-2]);
    val = getmem(args[-1]);
    if (val > 0x100)
        return player->xclock[clock];
    if (clock == 0) {
        player->game_clock += (long)(val - player->xclock[clock]) * 0x4B000L;
        lastDurCheck = player->game_clock >> 8;
    }
    player->xclock[clock] = val;
    return 0;
}

/* The first argument for a male player, the second for a female one. */
int far sex(int far *args)
{
    return getmem(args[-2 + player->female]);
}

int far gronk_door(int far *args)
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
        CloseDoor(0, door);
        break;
    case 2:
        ToggleDoor(0, door);
        break;
    }
    MapObj_X = ox;
    MapObj_Y = oy;
    return 1;
}

/* Write an object's fields from conversation variables, or read them into them; a
   variable holding -1 is left alone. */
void far x_obj_stuff(int far *args)
{
    int far *heading;
    int far *owner;
    int far *flags;
    int far *link;
    int far *flag10;
    int far *flag9;
    int far *quality;
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
            obj->ol.f.link = *link;
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
            *link = obj->ol.f.link;
        if (*flag10 != -1)
            *flag10 = obj->id & 0x400;
        if (*flag9 != -1)
            *flag9 = obj->id & 0x200;
        if (*quality != -1)
            *quality = obj->qn.f.quality;
    }
}

/* Mode 1 sets an object's fine position and height, 2 reads its tile, anything else
   reads its fine position and height. */
void far x_obj_pos(int far *args)
{
    int far *x;
    int far *y;
    int far *z;
    struct Object far *obj;

    x = getmem_addr(args[-3]);
    y = getmem_addr(args[-2]);
    z = getmem_addr(args[-1]);
    obj = Obj_IntTMem(getmem(args[-5]));
    switch (getmem(args[-4])) {
    case 1:
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
        break;
    case 2:
        if (*x != -1)
            *x = OBJ_HOMEX(obj);
        if (*y != -1)
            *y = OBJ_HOMEY(obj);
        if (*z != -1)
            *z = OBJ_Z(obj);
        break;
    default:
        if (*x != -1)
            *x = OBJ_FINEX(obj);
        if (*y != -1)
            *y = OBJ_FINEY(obj);
        if (*z != -1)
            *z = OBJ_Z(obj);
        break;
    }
}
