/* target: ovr113 */
/* opts: -mm -1 -G -O -Y -d */
/* The SCD event handlers: what a schedule row does when it falls due. The whole of DOS
   overlay ovr113, in original order.

   What it does in the game: SCHEDULE.C walks the rows of SCD.ARK and calls Sched_DoEvent for
   each one that is due. A row (struct SCDRow, event.h) applies to one level, to any level
   (0xFF) or to a whole world (0xF6 + world); its event code (byte 4; negative means
   disabled) picks a handler from SCDEventCodeJumps: 0 nothing, 1 change goal, 2 teleport,
   3 kill, 4 set a quest bit, 5 fire the timer-like triggers on a square, 6 nothing, 7 a
   special case chosen by the row's hack byte (NestedSCDEventCodeJumps), 8 set attitude, 9
   set a variable, 10 test variables and enable the rows after it, 11 remove. A row with its
   once flag set is deleted after it runs. Most handlers act on a set of critters named by a
   word: gronk_critid selects by whoami, by race, one object, or all, and calls a gronkify_
   callback for each. These rows move NPCs about their daily routines (by the day clock,
   block 0) and carry the plot forward as the X clocks advance.

   Data owned: the two handler tables at the end of the file.
   Function names are the FM Towns originals: FM Towns has 31 functions in this file and DOS
   31; aligned in order they agree in body where checked (ev_trapvar_ calls
   set_numbered_variable with the row's words at +5, +7 and +8 as
   SCDOperationOnQuestOrVariables did), and every name's tools/bssorder.py key puts it where
   the EXE's overlay stub order needs it, so all are the FM Towns names.
   Name: descriptive (the SCD event handlers: ev_*, gronkify_*, Sched_DoEvent). */
#include <dos.h>
#include <stdlib.h>
#include "conv.h"
#include "critter.h"
#include "event.h"
#include "map.h"
#include "object.h"
#include "player.h"

extern unsigned LastActiveMob;
extern unsigned char far *ActiveMob;

/* Calls code(npc, param) for the first (loop clear) or every active critter of a race,
   skipping loners. code returns true when it removed the critter from the active list. */
void far gronk_race(int race, unsigned char loop, int param,
                                 char (far *code)(struct Object far *, int))
{
    unsigned char far *list;
    struct Object far *npc;

    list = (unsigned char far *)MK_FP(FP_SEG(ActiveMob), FP_OFF(ActiveMob));
    while ((unsigned)list < LastActiveMob) {
        npc = Obj_IntTMem(*list);
        if (OBJ_MAJOR(npc) == MAJOR_CREATURE &&
            Creature[npc->id & ID_INMAJOR].race == race &&
            !OBJ_LONER(npc)) {
            if (code(npc, param))
                list--;
            if (!loop)
                break;
        }
        list++;
    }
}

/* As gronk_race, for every active critter. */
void far gronk_all_critters(unsigned char loop, int param,
                                    char (far *code)(struct Object far *, int))
{
    unsigned char far *list;
    struct Object far *npc;

    list = (unsigned char far *)MK_FP(FP_SEG(ActiveMob), FP_OFF(ActiveMob));
    while ((unsigned)list < LastActiveMob) {
        npc = Obj_IntTMem(*list);
        if (OBJ_MAJOR(npc) == MAJOR_CREATURE) {
            if (code(npc, param))
                list--;
            if (!loop)
                break;
        }
        list++;
    }
}

void far gronk_whoami(int whoami, unsigned char all, int arg,
                     char (far *fn)(struct Object far *, int));

/* Selects critters by params: the low byte is the mode (0 by whoami, 1 by race, 2 the
   object with that index, 3 all), the high byte the value matched. */
void far gronk_critid(int params, unsigned char all, int row,
                      char (far *fn)(struct Object far *, int))
{
    int mode = params & 0xFF;
    int filter = (unsigned char)(params >> 8);
    switch (mode) {
    case 0: gronk_whoami(filter, all, row, fn); break;
    case 1: gronk_race(filter, all, row, fn); break;
    case 2: fn(Obj_IntTMem(filter), row); break;
    case 3: gronk_all_critters(all, row, fn); break;
    }
}

char far ev_donothing(void) { return 0; }

/* Event 1: the critter gets goal row[7] with target row[8]. */
char far gronkify_change_goal(struct Object far *npc, char *row)
{
    change_critter_goal(npc, row[7], (unsigned char)row[8]);
    return 0;
}

char far ev_change_goal(char far *row)
{
    struct SCDRow copy;
    movedata(FP_SEG(row), FP_OFF(row), FP_SEG(&copy), FP_OFF(&copy), 16);
    gronk_critid(copy.p.npc.critters, 1, (int)&copy,
                 (char (far *)(struct Object far *, int))gronkify_change_goal);
    return 0;
}

char far check_alert(int mode, int x, int y);

/* True when the player could see fine position x, y: it is roughly in front of him
   (within one eighth of a turn of his facing) and check_alert does not rule it out. */
unsigned char far player_looking(int x, int y)
{
    int px = OBJ_HOMEX(ThePlayer) * 8 +
             OBJ_FINEX(ThePlayer);
    int py = OBJ_HOMEY(ThePlayer) * 8 +
             OBJ_FINEY(ThePlayer);
    int heading, facing;
    if (check_alert(1, x >> 3, y >> 3))
        return 0;
    heading = deltatotheta((char)x - (char)px, (char)y - (char)py);
    facing = (PlayerFacing >> 13) & 7;
    if (abs(heading - facing) < 2 || abs(heading - facing) > 6)
        return 1;
    return 0;
}

struct EventRow { unsigned char b[16]; };
unsigned char far teleport_critter(struct Object far *npc, int x, int y, int mode);
void far Sched_Migrate(struct EventRow far *row);

/* Event 2: moves the NPC to the row's square (and level), but only where the player can
   see neither the NPC nor the destination, unless the row's unseen flag says otherwise; with
   sethome the destination becomes its home. If it could not move and the row has a retry
   delay, a one-off copy of the row is queued (Sched_Migrate) for the day clock
   (xclock[XC_TIME] + retry, modulo 72). */
char far gronkify_teleport(struct Object far *npc, unsigned char *row)
{
    struct EventRow copy;
    if (row[10] || (player_looking((unsigned)row[5] << 3, (unsigned)row[6] << 3) == 0 &&
                    player_looking(OBJ_HOMEX(npc) * 8 +
                                    OBJ_FINEX(npc),
                                    OBJ_HOMEY(npc) * 8 +
                                    OBJ_FINEY(npc)) == 0)) {
        if (teleport_critter(npc, row[5], row[6], row[9])) {
            if (row[11]) {
                npc->qn.f.quality = row[5];
                npc->ol.f.owner = row[6];
            }
            return 0;
        }
    }
    if (row[12] > 0) {
        copy = *(struct EventRow *)row;
        *(unsigned *)copy.b = (unsigned)(player->xclock[XC_TIME] + row[12]) % 0x48;
        copy.b[3] = 1;
        Sched_Migrate(&copy);
    }
    return 0;
}

char far ev_teleport(char far *row)
{
    struct SCDRow copy;
    movedata(FP_SEG(row), FP_OFF(row), FP_SEG(&copy), FP_OFF(&copy), 16);
    gronk_critid(copy.p.teleport.critters, 1, (int)&copy,
                 (char (far *)(struct Object far *, int))gronkify_teleport);
    return 0;
}

/* Event 11: removes the critter from the map. */
int far gronkify_remove(struct Object far *obj)
{
    struct Tile far *tile = Map_GetAddr(OBJ_HOMEX(obj),
                                        OBJ_HOMEY(obj));
    return Obj_Punt(&tile->objects, obj, 1) == 0;
}

extern unsigned char far *SCD_dseg_67d6_8634;
void far instant_kill(struct Object far *npc);

/* Event 3: kills the critter outright, except the NPC the player is talking to when
   row[7] is 0; in that case, for a repeating row with row[8] set and byte 6 of the
   schedule work area equal to 15, it counts up xclock[XC_CHANGED] instead. (In SCHEDULE.C's
   SCDWork layout byte 6 is inside the first queued migration row, not the block number;
   what the test was meant to see is an open question.) */
char far gronkify_slay(struct Object far *npc, unsigned char *row)
{
    if (!row[7] && npc == talking_to) {
        if (row[8] > 0 && SCD_dseg_67d6_8634[6] == 15 && !row[3])
            player->xclock[XC_CHANGED]++;
        return 0;
    }
    instant_kill(npc);
    return 1;
}

char far ev_kill(char far *row)
{
    char copy[18];
    movedata(FP_SEG(row), FP_OFF(row), FP_SEG(copy), FP_OFF(copy), 18);
    gronk_critid(*(unsigned *)(copy + 5), 1, (int)copy,
                 (char (far *)(struct Object far *, int))gronkify_slay);
    return 0;
}

char far ev_remove(char far *row)
{
    struct SCDRow copy;
    movedata(FP_SEG(row), FP_OFF(row), FP_SEG(&copy), FP_OFF(&copy), 16);
    gronk_critid(copy.p.npc.critters, 1, (int)&copy,
                 (char (far *)(struct Object far *, int))gronkify_remove);
    return 0;
}

/* Event 4: sets bit (quest & 3) of quest variable quest / 4 to value (the code's
   packing; inferred to address quest flags four to a variable). */
char far ev_set_qbit(unsigned char far *row)
{
    struct SCDRow far *params = (struct SCDRow far *)row;
    player->quests[(unsigned)params->p.qbit.quest >> 2] =
        (player->quests[(unsigned)params->p.qbit.quest >> 2]
         & (long)~(1 << (params->p.qbit.quest & 3)))
        + (long)(params->p.qbit.value << (params->p.qbit.quest & 3));
    return 0;
}

void far UseTrigger(long a, long b, struct Object far *trigger, int kind);

/* Event 5: fires every trigger of minor class 0xC on square (row[5], row[6]) with
   UseTrigger kind 0xC (the scheduled triggers' mode in the Guide's trigger type
   table). */
char far ev_trigger(unsigned char far *row)
{
    unsigned char far *params = row;
    struct Tile far *tile = Map_GetAddr((char)row[5], (char)row[6]);
    union Link far *head = &tile->objects;
    struct Object far *obj;
    struct Object far *next = 0;
    if (params == row) ;                /* match: an empty test DOS has */
    obj = Obj_PtrTMem(head);
    while (obj) {
        if ((OBJ_CLASS(obj) & 0x1E) == CLASS_TRIGGER &&
            OBJ_INCLASS(obj) == 0xC) {
            next = Obj_PtrTMem(&obj->qn.link);
            UseTrigger(0L, 0L, obj, 0xC);
        } else {
            next = Obj_PtrTMem(&obj->qn.link);
        }
        obj = next;
    }
    return 0;
}

/* Hack 6 (Nystul): turns the critter to face heading row[8] (eighths of a turn). */
char far gronkify_nystul(struct Object far *npc, unsigned char *row)
{
    register unsigned char *p;
    register unsigned char *r = row;
    register unsigned char heading;
    p = r + 6;
    heading = p[2];
    npc->heading = heading << 5;
    npc->pos = npc->pos & 0xFC7F | ((heading & 7) << 7);
    npc->b18 &= 0xE0;
    return 0;
}

char far ev_nystul_hack(char far *row)
{
    struct EventRow copy;
    unsigned char *p;
    copy = *(struct EventRow far *)row;
    p = copy.b + 6;
    gronk_critid(*(unsigned *)p, 1, (int)copy.b,
                 (char (far *)(struct Object far *, int))gronkify_nystul);
    return 0;
}

/* Hack 7: moves the critter to a free square in the rectangle row[8..11], scanning row
   by row, and makes it its home. */
char far gronkify_gotha(struct Object far *npc, unsigned char *row)
{
    unsigned char *eventRow = row;
    unsigned char *p = eventRow + 6;
    unsigned char sx = p[2], sy = p[3], ex = p[4], ey = p[5];
    int x, y;
    for (y = sy; y <= ey; y++) {
        for (x = sx; x <= ex; x++) {
            if (teleport_critter(npc, x, y, 0))
                break;
        }
    }
    npc->qn.f.quality = x;
    npc->ol.f.owner = y;
    return 0;
}

char far ev_gotha_hack(char far *row)
{
    struct EventRow copy;
    unsigned char *p;
    copy = *(struct EventRow far *)row;
    p = copy.b + 6;
    gronk_critid(*(unsigned *)p, 1, (int)copy.b,
                 (char (far *)(struct Object far *, int))gronkify_gotha);
    return 0;
}

void far set_numbered_variable(int left, int op, int right);

/* Hack 1 (the gargoyle, inferred from the name): if the critter stands on square
   (row[8], row[9]), applies set_numbered_variable(word at row[10], op row[12], word at
   row[13]). */
char far gronkify_garg(struct Object far *npc, unsigned char *row)
{
    register unsigned char *p;
    register unsigned char *r = row;
    p = r + 6;
    if (OBJ_HOMEX(npc) == p[2] &&
        OBJ_HOMEY(npc) == p[3])
        set_numbered_variable(
            *(unsigned *)(p + 4), p[6], *(unsigned *)(p + 7));
    return 0;
}

char far ev_garg_hack(char far *row)
{
    struct EventRow copy;
    unsigned char *p;
    copy = *(struct EventRow far *)row;
    p = copy.b + 6;
    gronk_critid(*(unsigned *)p, 1, (int)copy.b,
                 (char (far *)(struct Object far *, int))gronkify_garg);
    return 0;
}

/* Hack 2: if the critter is at square (row[6], row[7]) (or that square is 0, 0), moves
   it to a random free square of the rectangle row[10..13] on this level and makes that its
   home. */
char far gronkify_soldier(struct Object far *npc, unsigned char *row)
{
    unsigned char *eventRow = row;
    int x, y, max;
    register unsigned char *p = eventRow + 6;
    register int retry;
    if (p[0] || p[1]) {
        if (OBJ_HOMEX(npc) == p[0]) {
            if (OBJ_HOMEY(npc) == p[1])
                goto move;
        }
        goto end;
    }
move:
    max = (p[6] - p[4] + 1) * (p[7] - p[5] + 1);
    for (retry = 0; retry < max; retry++) {
        x = p[4] + (int)(((long)rand() * (p[6] - p[4] + 1)) / 0x8000L);
        y = p[5] + (int)(((long)rand() * (p[7] - p[5] + 1)) / 0x8000L);
        if (teleport_critter(npc, x, y, PlayerLevel)) {
            npc->qn.f.quality = x;
            npc->ol.f.owner = y;
            break;
        }
    }
end:
    return 0;
}

char far ev_soldier_hack(char far *row)
{
    struct EventRow copy;
    unsigned char *p;
    copy = *(struct EventRow far *)row;
    p = copy.b + 6;
    gronk_critid(*(unsigned *)(p + 2), 1, (int)copy.b,
                 (char (far *)(struct Object far *, int))gronkify_soldier);
    return 0;
}

void far change_terrain(int x, int y, int wall, int floor, int height,
                               int type, int a, int b, int c);
/* Hack 3: across the whole map, about half the tiles whose floor is the given texture
   get the new floor texture and are raised by the given height (change_terrain; the ice
   caverns' freezing, inferred from the name). */
char far ev_freeze_hack(unsigned char far *row)
{
    struct SCDRow far *params = (struct SCDRow far *)row;
    unsigned long values = params->p.freeze.values;
    struct Tile far *tile;
    int x, y;
    for (x = 0; x < MAP_SIZE; x++) {
        for (y = 0; y < MAP_SIZE; y++) {
            tile = Map_GetAddr(x, y);
            if (tile->floor == ((unsigned char *)&values)[0] &&
                rand() % 2 == 1) {
                change_terrain(x, y, 0x3F, ((unsigned char *)&values)[1],
                    tile->height + (int)(values >> 16),
                    0x10, 0, 0, 0);
            }
        }
    }
    return 0;
}

/* Hack 4: closes doors (DoClosingDoors with the row's argument). */
char far ev_door_hack(unsigned char far *row)
{
    struct SCDRow far *params = (struct SCDRow far *)row;
    unsigned char value[2];
    value[0] = params->p.hack.arg[0];
    DoClosingDoors(value[0]);
    return 0;
}

extern char (far *NestedSCDEventCodeJumps_dseg_67d6_1344[])(unsigned char far *);
/* Event 7: dispatches by the row's hack byte. */
void far ev_hack(unsigned char far *row)
{
    struct SCDRow far *params = (struct SCDRow far *)row;
    NestedSCDEventCodeJumps_dseg_67d6_1344[params->p.hack.hack](row);
}

/* Event 8: sets the critter's attitude (0 hostile .. 3 friendly); any attitude but
   hostile also forgets who last hit it. */
char far gronkify_attitude(struct Object far *npc, int attitude)
{
    npc->attitude_word = npc->attitude_word & 0x3FFF | ((attitude & 3) << 14);
    if (attitude)
        npc->last_hit = 0;
    return 0;
}

char far ev_attitude(char far *row)
{
    struct SCDRow copy;
    movedata(FP_SEG(row), FP_OFF(row), FP_SEG(&copy), FP_OFF(&copy), 16);
    gronk_critid(copy.p.npc.critters, 1, copy.p.npc.arg[0],
                 (char (far *)(struct Object far *, int))gronkify_attitude);
    return 0;
}

/* Event 10: combines count numbered variables from var with op and compares the result
   with value (inverted by invert). If the test passes, the disabled rows that follow (event
   code negative) are run in turn, each enabled for the call and disabled again unless it is
   a once row (which deletes itself), up to the next test row (-10). Rows after a failing
   test stay disabled: a conditional block. */
char far ev_checkvar(unsigned char far *row)
{
    struct SCDRow far *params = (struct SCDRow far *)row;
    int upper;
    unsigned char invert;
    register int value, index;
    index = params->p.checkvar.var;
    upper = index + params->p.checkvar.count - 1;
    for (value = get_numbered_variable(index);
         (++index, index) <= upper;
         value = do_math_op(value, params->p.checkvar.op,
                                          get_numbered_variable(index))) ;
    if ((value != params->p.checkvar.value) == !params->p.checkvar.invert)
        return 0;
    else {
        for (row += 16; (signed char)row[4] < 0; row += 16) {
            invert = !row[3];
            row[4] = -row[4];
            if (Sched_DoEvent(row) == 4)
                row -= 16;
            if (invert)
                row[4] = -row[4];
            if (-(signed char)row[4] == 10)
                break;
        }
    }
    return 0;
}

/* Event 9: set_numbered_variable(var, op, value) (the same operation as a variable trap). */
char far ev_trapvar(unsigned char far *row)
{
    struct SCDRow far *params = (struct SCDRow far *)row;
    set_numbered_variable(
        params->p.trapvar.var, params->p.trapvar.op, params->p.trapvar.value);
    return 0;
}

extern char (far *SCDEventCodeJumps_dseg_67d6_1364[])(unsigned char far *);
void far Sched_Delete(unsigned char far *row);

/* Runs one schedule row if it applies to this level and is enabled. Returns 5 when it did
   not apply, 6 for an unknown event code, 4 when the row deleted itself (a once row), else
   the handler's result (0). */
char far Sched_DoEvent(unsigned char far *row)
{
    unsigned char matching;
    unsigned char result;
    matching = row[2] == PlayerLevel || row[2] == 0xFF ||
               (PlayerLevel - 1) / LEVELS_PER_WORLD == row[2] - 0xF6;
    result = 5;
    if (!matching || (signed char)row[4] < 0)
        return result;
    if ((signed char)row[4] >= 12)
        return 6;
    result = SCDEventCodeJumps_dseg_67d6_1364[(signed char)row[4]](row);
    if (row[3]) {
        Sched_Delete(row);
        return 4;
    }
    return result;
}

/* This file's _DATA, DS:1344..1393: the schedule event handlers, by the event row's
   sub-code (the hack byte of event 7) and code. */
/* match: ovr112's data ends at 1343, odd; ovr114's starts at 1394. */
typedef char (far *SCDEventFn)(unsigned char far *);
SCDEventFn NestedSCDEventCodeJumps_dseg_67d6_1344[8] = {
    (SCDEventFn)ev_donothing, (SCDEventFn)ev_garg_hack,
    (SCDEventFn)ev_soldier_hack, ev_freeze_hack, ev_door_hack,
    (SCDEventFn)ev_donothing, (SCDEventFn)ev_nystul_hack,
    (SCDEventFn)ev_gotha_hack };
SCDEventFn SCDEventCodeJumps_dseg_67d6_1364[12] = {
    (SCDEventFn)ev_donothing, (SCDEventFn)ev_change_goal,
    (SCDEventFn)ev_teleport, (SCDEventFn)ev_kill,
    ev_set_qbit, ev_trigger, (SCDEventFn)ev_donothing,
    (SCDEventFn)ev_hack, (SCDEventFn)ev_attitude,
    ev_trapvar, ev_checkvar,
    (SCDEventFn)ev_remove };
