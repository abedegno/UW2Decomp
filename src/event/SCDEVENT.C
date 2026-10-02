/* target: ovr113 */
/* opts: -mm -1 -G -O -Y -d */
/* SCD event handling. FM Towns has 31 functions in this file and DOS 31; aligned in order
   they agree in body where checked (ev_trapvar_ calls set_numbered_variable with the row's
   words at +5, +7 and +8 as SCDOperationOnQuestOrVariables did), and every name's
   tools/bssorder.py key puts it where the EXE's overlay stub order needs it, so all are
   the FM Towns names. */
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

int far gronkify_remove(struct Object far *obj)
{
    struct Tile far *tile = Map_GetAddr(OBJ_HOMEX(obj),
                                        OBJ_HOMEY(obj));
    return Obj_Punt(&tile->objects, obj, 1) == 0;
}

extern unsigned char far *SCD_dseg_67d6_8634;
void far instant_kill(struct Object far *npc);

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

char far ev_trigger(unsigned char far *row)
{
    unsigned char far *params = row;
    struct Tile far *tile = Map_GetAddr((char)row[5], (char)row[6]);
    union Link far *head = &tile->objects;
    struct Object far *obj;
    struct Object far *next = 0;
    if (params == row) ;
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

char far ev_door_hack(unsigned char far *row)
{
    struct SCDRow far *params = (struct SCDRow far *)row;
    unsigned char value[2];
    value[0] = params->p.hack.arg[0];
    DoClosingDoors(value[0]);
    return 0;
}

extern char (far *NestedSCDEventCodeJumps_dseg_67d6_1344[])(unsigned char far *);
void far ev_hack(unsigned char far *row)
{
    struct SCDRow far *params = (struct SCDRow far *)row;
    NestedSCDEventCodeJumps_dseg_67d6_1344[params->p.hack.hack](row);
}

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

char far ev_trapvar(unsigned char far *row)
{
    struct SCDRow far *params = (struct SCDRow far *)row;
    set_numbered_variable(
        params->p.trapvar.var, params->p.trapvar.op, params->p.trapvar.value);
    return 0;
}

extern char (far *SCDEventCodeJumps_dseg_67d6_1364[])(unsigned char far *);
void far Sched_Delete(unsigned char far *row);

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

/* This file's _DATA, DS:1344..1393 (ovr112's data ends at 1343, odd; ovr114's starts at
   1394): the schedule event handlers, by the event row's sub-code and code. */
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
