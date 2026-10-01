/* target: ovr113 */
/* opts: -mm -1 -G -O -Y -d */
#include <dos.h>
#include <stdlib.h>

struct Object {
    unsigned id, pos;
    unsigned qn:6, qn_hi:2;
    unsigned char b05;
    unsigned ol:6, ol_hi:2;
    unsigned char b07;
    unsigned char hp, b09, b0A;
    unsigned goal_word, attitude_word;
    char pad0F[6];
    unsigned char b15;
    unsigned home;
    unsigned char b18, b19, whoami;
};

extern unsigned LastActiveMob;
/* The creature table (ovr104's Creature, 0x30 bytes a type); only the race byte, +9, is read. */
extern unsigned char Creature[][0x30];
extern unsigned char far *ActiveMob;
struct Object far * far Obj_IntTMem(int index);

void far gronk_race(int race, unsigned char loop, int param,
                                 char (far *code)(struct Object far *, int))
{
    unsigned char far *list;
    struct Object far *npc;

    list = (unsigned char far *)MK_FP(FP_SEG(ActiveMob), FP_OFF(ActiveMob));
    while ((unsigned)list < LastActiveMob) {
        npc = Obj_IntTMem(*list);
        if (((npc->id & 0x1C0) >> 6) == 1 &&
            Creature[npc->id & 0x3F][9] == race &&
            !((npc->b0A & 0x80) >> 7)) {
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
        if (((npc->id & 0x1C0) >> 6) == 1) {
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
void far change_critter_goal(struct Object far *npc, char goal, int gtarg);

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

char far SetAL_ToZero_ovr113_18F(void) { return 0; }

char far gronkify_change_goal(struct Object far *npc, char *row)
{
    change_critter_goal(npc, row[7], (unsigned char)row[8]);
    return 0;
}

char far SCDSetGoalAndGTARG_ovr113_1BA(char far *row)
{
    char copy[16];
    movedata(FP_SEG(row), FP_OFF(row), FP_SEG(copy), FP_OFF(copy), 16);
    gronk_critid(*(unsigned *)(copy + 5), 1, (int)copy,
                 (char (far *)(struct Object far *, int))gronkify_change_goal);
    return 0;
}

extern struct Object far *ThePlayer;
extern int PlayerFacing;
char far check_alert(int mode, int x, int y);
unsigned char far deltatotheta(char dx, char dy);

unsigned char far player_looking(int x, int y)
{
    int px = ((ThePlayer->home & 0xFC00) >> 10) * 8 +
             ((ThePlayer->pos & 0xE000) >> 13);
    int py = ((ThePlayer->home & 0x3F0) >> 4) * 8 +
             ((ThePlayer->pos & 0x1C00) >> 10);
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
struct Player { unsigned char pad[0x36D], day, pad2[14], xclock15; };
extern struct Player near *player;
unsigned char far teleport_critter(struct Object far *npc, int x, int y, int mode);
void far Sched_Migrate(struct EventRow far *row);

char far gronkify_teleport(struct Object far *npc, unsigned char *row)
{
    struct EventRow copy;
    if (row[10] || (player_looking((unsigned)row[5] << 3, (unsigned)row[6] << 3) == 0 &&
                    player_looking(((npc->home & 0xFC00) >> 10) * 8 +
                                    ((npc->pos & 0xE000) >> 13),
                                    ((npc->home & 0x3F0) >> 4) * 8 +
                                    ((npc->pos & 0x1C00) >> 10)) == 0)) {
        if (teleport_critter(npc, row[5], row[6], row[9])) {
            if (row[11]) {
                npc->qn = row[5];
                npc->ol = row[6];
            }
            return 0;
        }
    }
    if (row[12] > 0) {
        copy = *(struct EventRow *)row;
        *(unsigned *)copy.b = (unsigned)(player->day + row[12]) % 0x48;
        copy.b[3] = 1;
        Sched_Migrate(&copy);
    }
    return 0;
}

char far SCDRunCodeOnNPCSToMoveTile_ovr113_3AB(char far *row)
{
    char copy[16];
    movedata(FP_SEG(row), FP_OFF(row), FP_SEG(copy), FP_OFF(copy), 16);
    gronk_critid(*(unsigned *)(copy + 7), 1, (int)copy,
                 (char (far *)(struct Object far *, int))gronkify_teleport);
    return 0;
}

struct Tile { unsigned pad, head; };
struct Tile far * far Map_GetAddr(int x, int y);
struct Object far * far Obj_Punt(unsigned far *head, struct Object far *obj, int how);

int far gronkify_remove(struct Object far *obj)
{
    struct Tile far *tile = Map_GetAddr((obj->home & 0xFC00) >> 10,
                                        (obj->home & 0x3F0) >> 4);
    return Obj_Punt(&tile->head, obj, 1) == 0;
}

extern struct Object far *talking_to;
extern unsigned char far *SCD_dseg_67d6_8634;
void far instant_kill(struct Object far *npc);

char far SCDKillCNPC_ovr113_433(struct Object far *npc, unsigned char *row)
{
    if (!row[7] && npc == talking_to) {
        if (row[8] > 0 && SCD_dseg_67d6_8634[6] == 15 && !row[3])
            player->xclock15++;
        return 0;
    }
    instant_kill(npc);
    return 1;
}

char far SCDRunCodeOnNPCAndKill_ovr113_48E(char far *row)
{
    char copy[18];
    movedata(FP_SEG(row), FP_OFF(row), FP_SEG(copy), FP_OFF(copy), 18);
    gronk_critid(*(unsigned *)(copy + 5), 1, (int)copy,
                 (char (far *)(struct Object far *, int))SCDKillCNPC_ovr113_433);
    return 0;
}

char far SCDRemoveObjectFromTile_ovr113_4C3(char far *row)
{
    char copy[16];
    movedata(FP_SEG(row), FP_OFF(row), FP_SEG(copy), FP_OFF(copy), 16);
    gronk_critid(*(unsigned *)(copy + 5), 1, (int)copy,
                 (char (far *)(struct Object far *, int))gronkify_remove);
    return 0;
}

char far SCDChangeQuest_ovr113_4F8(unsigned char far *row)
{
    unsigned char far *params = row;
    *(unsigned long *)((char *)player + 0x66 + (((unsigned)params[5] >> 2) << 2)) =
        (*(unsigned long *)((char *)player + 0x66 + (((unsigned)params[5] >> 2) << 2)) &
         (long)~(1 << (params[5] & 3))) +
        (long)(params[6] << (params[5] & 3));
    return 0;
}

struct Object far * far Obj_PtrTMem(unsigned far *link);
void far UseTrigger(long a, long b, struct Object far *trigger, int kind);

char far ev_trigger(unsigned char far *row)
{
    unsigned char far *params = row;
    struct Tile far *tile = Map_GetAddr((char)row[5], (char)row[6]);
    unsigned far *head = &tile->head;
    struct Object far *obj;
    struct Object far *next = 0;
    if (params == row) ;
    obj = Obj_PtrTMem(head);
    while (obj) {
        if (((obj->id & 0x1F0) >> 4 & 0x1E) == 0x1A &&
            (obj->id & 0xF) == 0xC) {
            next = Obj_PtrTMem((unsigned far *)((char far *)obj + 4));
            UseTrigger(0L, 0L, obj, 0xC);
        } else {
            next = Obj_PtrTMem((unsigned far *)((char far *)obj + 4));
        }
        obj = next;
    }
    return 0;
}

char far SCDChangeNPCHeading_ovr113_666(struct Object far *npc, unsigned char *row)
{
    register unsigned char *p;
    register unsigned char *r = row;
    register unsigned char heading;
    p = r + 6;
    heading = p[2];
    npc->b09 = heading << 5;
    npc->pos = npc->pos & 0xFC7F | ((heading & 7) << 7);
    npc->b18 &= 0xE0;
    return 0;
}

char far SCDChangeHeading_ovr113_6AC(char far *row)
{
    struct EventRow copy;
    unsigned char *p;
    copy = *(struct EventRow far *)row;
    p = copy.b + 6;
    gronk_critid(*(unsigned *)p, 1, (int)copy.b,
                 (char (far *)(struct Object far *, int))SCDChangeNPCHeading_ovr113_666);
    return 0;
}

char far SCDMoveNPCToTileRange_ovr113_6E5(struct Object far *npc, unsigned char *row)
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
    npc->qn = x;
    npc->ol = y;
    return 0;
}

char far SCDMoveNPCToTileRange_ovr113_775(char far *row)
{
    struct EventRow copy;
    unsigned char *p;
    copy = *(struct EventRow far *)row;
    p = copy.b + 6;
    gronk_critid(*(unsigned *)p, 1, (int)copy.b,
                 (char (far *)(struct Object far *, int))SCDMoveNPCToTileRange_ovr113_6E5);
    return 0;
}

void far set_numbered_variable(int left, int op, int right);

char far SCDVariableOperation_ovr113_7AE(struct Object far *npc, unsigned char *row)
{
    register unsigned char *p;
    register unsigned char *r = row;
    p = r + 6;
    if (((npc->home & 0xFC00) >> 10) == p[2] &&
        ((npc->home & 0x3F0) >> 4) == p[3])
        set_numbered_variable(
            *(unsigned *)(p + 4), p[6], *(unsigned *)(p + 7));
    return 0;
}

char far SCDRunVariableOperationOnNPCs_ovr113_800(char far *row)
{
    struct EventRow copy;
    unsigned char *p;
    copy = *(struct EventRow far *)row;
    p = copy.b + 6;
    gronk_critid(*(unsigned *)p, 1, (int)copy.b,
                 (char (far *)(struct Object far *, int))SCDVariableOperation_ovr113_7AE);
    return 0;
}

extern int PlayerLevel;

char far SCDMoveNPCToTileRandom_ovr113_839(struct Object far *npc, unsigned char *row)
{
    unsigned char *eventRow = row;
    int x, y, max;
    register unsigned char *p = eventRow + 6;
    register int retry;
    if (p[0] || p[1]) {
        if (((npc->home & 0xFC00) >> 10) == p[0]) {
            if (((npc->home & 0x3F0) >> 4) == p[1])
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
            npc->qn = x;
            npc->ol = y;
            break;
        }
    }
end:
    return 0;
}

char far SCDMoveNPCToRandom_ovr113_961(char far *row)
{
    struct EventRow copy;
    unsigned char *p;
    copy = *(struct EventRow far *)row;
    p = copy.b + 6;
    gronk_critid(*(unsigned *)(p + 2), 1, (int)copy.b,
                 (char (far *)(struct Object far *, int))SCDMoveNPCToTileRandom_ovr113_839);
    return 0;
}

void far change_terrain(int x, int y, int wall, int floor, int height,
                               int type, int a, int b, int c);
struct TileFlags { unsigned shape:4, height:4, wall:2, floor:4; };
char far ev_freeze_hack(unsigned char far *row)
{
    unsigned char far *params = row;
    unsigned long values = *(unsigned long far *)(params + 6);
    struct Tile far *tile;
    int x, y;
    for (x = 0; x < 64; x++) {
        for (y = 0; y < 64; y++) {
            tile = Map_GetAddr(x, y);
            if (((struct TileFlags far *)tile)->floor == ((unsigned char *)&values)[0] &&
                rand() % 2 == 1) {
                change_terrain(x, y, 0x3F, ((unsigned char *)&values)[1],
                    ((struct TileFlags far *)tile)->height + (int)(values >> 16),
                    0x10, 0, 0, 0);
            }
        }
    }
    return 0;
}

void far DoClosingDoors(unsigned char x);
char far SCDFindAndCloseDoor_ovr113_A3A(unsigned char far *row)
{
    unsigned char far *params = row;
    unsigned char value[2];
    value[0] = params[6];
    DoClosingDoors(value[0]);
    return 0;
}

extern char (far *NestedSCDEventCodeJumps_dseg_67d6_1344[])(unsigned char far *);
void far CallNestedSCDCodeJumps_ovr113_A62(unsigned char far *row)
{
    unsigned char far *params = row;
    NestedSCDEventCodeJumps_dseg_67d6_1344[params[5]](row);
}

char far SetAttitude_ovr113_A91(struct Object far *npc, int attitude)
{
    npc->attitude_word = npc->attitude_word & 0x3FFF | ((attitude & 3) << 14);
    if (attitude)
        ((unsigned char far *)npc)[0x12] = 0;
    return 0;
}

char far SCDSetAttitude_ovr113_ABD(char far *row)
{
    char copy[16];
    movedata(FP_SEG(row), FP_OFF(row), FP_SEG(copy), FP_OFF(copy), 16);
    gronk_critid(*(unsigned *)(copy + 5), 1, (unsigned char)copy[7],
                 (char (far *)(struct Object far *, int))SetAttitude_ovr113_A91);
    return 0;
}

int far get_numbered_variable(int index);
int far do_math_op(int value, int op, int right);
char far Sched_DoEvent(unsigned char far *row);

char far ev_checkvar(unsigned char far *row)
{
    unsigned char far *params = row;
    int upper;
    unsigned char invert;
    register int value, index;
    index = *(unsigned far *)(params + 5);
    upper = index + params[7] - 1;
    for (value = get_numbered_variable(index);
         (++index, index) <= upper;
         value = do_math_op(value, params[8],
                                          get_numbered_variable(index))) ;
    if ((value != *(int far *)(params + 10)) == !params[9])
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

char far SCDOperationOnQuestOrVariables_ovr113_BD8(unsigned char far *row)
{
    unsigned char far *params = row;
    set_numbered_variable(
        *(unsigned far *)(params + 5), params[7], *(unsigned far *)(params + 8));
    return 0;
}

extern char (far *SCDEventCodeJumps_dseg_67d6_1364[])(unsigned char far *);
void far Sched_Delete(unsigned char far *row);

char far Sched_DoEvent(unsigned char far *row)
{
    unsigned char matching;
    unsigned char result;
    matching = row[2] == PlayerLevel || row[2] == 0xFF ||
               (PlayerLevel - 1) / 8 == row[2] - 0xF6;
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
    (SCDEventFn)SetAL_ToZero_ovr113_18F, (SCDEventFn)SCDRunVariableOperationOnNPCs_ovr113_800,
    (SCDEventFn)SCDMoveNPCToRandom_ovr113_961, ev_freeze_hack, SCDFindAndCloseDoor_ovr113_A3A,
    (SCDEventFn)SetAL_ToZero_ovr113_18F, (SCDEventFn)SCDChangeHeading_ovr113_6AC,
    (SCDEventFn)SCDMoveNPCToTileRange_ovr113_775 };
SCDEventFn SCDEventCodeJumps_dseg_67d6_1364[12] = {
    (SCDEventFn)SetAL_ToZero_ovr113_18F, (SCDEventFn)SCDSetGoalAndGTARG_ovr113_1BA,
    (SCDEventFn)SCDRunCodeOnNPCSToMoveTile_ovr113_3AB, (SCDEventFn)SCDRunCodeOnNPCAndKill_ovr113_48E,
    SCDChangeQuest_ovr113_4F8, ev_trigger, (SCDEventFn)SetAL_ToZero_ovr113_18F,
    (SCDEventFn)CallNestedSCDCodeJumps_ovr113_A62, (SCDEventFn)SCDSetAttitude_ovr113_ABD,
    SCDOperationOnQuestOrVariables_ovr113_BD8, ev_checkvar,
    (SCDEventFn)SCDRemoveObjectFromTile_ovr113_4C3 };
