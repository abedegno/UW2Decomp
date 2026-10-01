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

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0x21];
    unsigned char skills[20];           /* 0x21 */
    char pad35[0x4E - 0x35];
    unsigned long exp;                  /* 0x4E */
    unsigned char skill_points;         /* 0x52 */
    char pad53[0x62 - 0x53];
    unsigned b62:10;                    /* 0x62 */
    unsigned in_pits:1;                 /* word 0x62, bit 10 */
    unsigned b63_3:5;
    unsigned b64:8;                     /* 0x64 */
    unsigned lefty:1;                   /* 0x65 */
    unsigned female:1;
    unsigned b65_2:6;
    unsigned long quests[32];           /* 0x66, quests 0-127 (four to a long) */
    unsigned char quest_bytes[16];      /* 0xE6, quests 128-143 */
    unsigned char bF6;                  /* 0xF6 */
    char padF7[0x360 - 0xF7];
    unsigned char pit_fighters[5];      /* 0x360 */
    char pad365[0x369 - 0x365];
    unsigned long game_clock;           /* 0x369 */
    unsigned char sched_hour[16];       /* 0x36D, the x-clocks */
};

/* Jospur's debt to the player for fights won in the pits: quest 133. */
#define JOSPUR_DEBT     quest_bytes[0x85 - 0x80]

/* A link to an object, with six bits of something else below it. */
union Link {
    unsigned word;
    struct { unsigned low:6, index:10; } f;
};

/* A mobile object, 0x1B bytes. The first 8 bytes are shared with static objects. */
struct Object {
    unsigned id;                        /* item 0-8 (major class 6-8), flags 9-12 */
    unsigned pos;                       /* z 0-6, heading 7-9, y fine 10-12, x fine 13-15 */
    union Link qn;                      /* quality 0-5, the next object 6-15 */
    union Link ol;                      /* owner 0-5, the contents or a link 6-15 */
    unsigned char hp;                   /* 0x08 */
    unsigned char b09;                  /* 0x09 */
    unsigned char b0A;                  /* 0x0A */
    unsigned goal_word;                 /* 0x0B, goal 0-3, goal target 4-11, frame 12-15 */
    unsigned attitude_word;             /* 0x0D, attitude in bits 14-15 */
    char pad0F[0x15 - 0x0F];
    unsigned char b15;                  /* 0x15, animation in bits 0-5 */
    unsigned home;                      /* 0x16, x in bits 10-15, y in bits 4-9 */
    unsigned char b18;                  /* 0x18 */
    unsigned char b19;                  /* 0x19 */
    unsigned char whoami;               /* 0x1A */
};

#define OBJ_ITEM(o)     ((o)->id & 0x1FF)
#define OBJ_MAJOR(o)    (((o)->id & 0x1C0) >> 6)
#define OBJ_TYPE(o)     (((o)->id & 0x3F) >> 0)
#define OBJ_FLAGS(o)    (((o)->id & 0x1E00) >> 9)
#define OBJ_Z(o)        ((o)->pos & 0x7F)
#define OBJ_HEADING(o)  (((o)->pos & 0x380) >> 7)
#define OBJ_FINEY(o)    (((o)->pos & 0x1C00) >> 10)
#define OBJ_FINEX(o)    (((o)->pos & 0xE000) >> 13)
#define OBJ_HOMEX(o)    (((o)->home & 0xFC00) >> 10)
#define OBJ_HOMEY(o)    (((o)->home & 0x3F0) >> 4)
#define OBJ_B0A_7(o)    (((o)->b0A & 0x80) >> 7)

#define SET_FLAGS(o, v)   ((o)->id = (o)->id & 0xE1FF | ((v) & 0xF) << 9)
#define SET_ID_10(o, v)   ((o)->id = (o)->id & 0xFBFF | ((v) & 1) << 10)
#define SET_ID_9(o, v)    ((o)->id = (o)->id & 0xFDFF | ((v) & 1) << 9)
#define SET_Z(o, v)       ((o)->pos = (o)->pos & 0xFF80 | (v) & 0x7F)
#define SET_HEADING(o, v) ((o)->pos = (o)->pos & 0xFC7F | ((v) & 7) << 7)
#define SET_FINEX(o, v)   ((o)->pos = (o)->pos & 0x1FFF | ((v) & 7) << 13)
#define SET_FINEY(o, v)   ((o)->pos = (o)->pos & 0xE3FF | ((v) & 7) << 10)
#define SET_HOMEX(o, v)   ((o)->home = (o)->home & 0x3FF | ((v) & 0x3F) << 10)
#define SET_HOMEY(o, v)   ((o)->home = (o)->home & 0xFC0F | ((v) & 0x3F) << 4)
#define SET_GOAL(o, v)    ((o)->goal_word = (o)->goal_word & 0xFFF0 | (v) & 0xF)
#define SET_GTARG(o, v)   ((o)->goal_word = (o)->goal_word & 0xF00F | ((v) & 0xFF) << 4)
#define SET_FRAME(o, v)   ((o)->goal_word = (o)->goal_word & 0x0FFF | ((v) & 0xF) << 12)
#define SET_TEMP(o, v)    ((o)->attitude_word = (o)->attitude_word & 0xFEFF | ((v) & 1) << 8)
#define SET_POWER(o, v)   ((o)->attitude_word = (o)->attitude_word & 0xFBFF | ((v) & 1) << 10)
#define SET_ATTITUDE(o, v) ((o)->attitude_word = (o)->attitude_word & 0x3FFF | ((v) & 3) << 14)
#define SET_SEQ(o, v)     ((o)->b15 = (o)->b15 & 0xC0 | ((v) & 0x3F) << 0)
#define SET_LONER(o, v)   ((o)->b0A = (o)->b0A & 0x7F | ((v) & 1) << 7)

struct Tile {
    unsigned type:4;
    unsigned height:4;
    unsigned b8:2;
    unsigned floor:4;                   /* bits 10-13 */
    unsigned b14:2;
    union Link objects;                 /* 0x02, head of the tile's object list */
};

/* One critter type's record, 0x30 bytes. */
struct Creature {
    char pad00[0x09];
    unsigned char race;                 /* 0x09 */
    char pad0A[0x30 - 0x0A];
};

/* The common object properties, one 11-byte record per item. */
struct ComObj {
    unsigned height:8;                  /* 0x00 */
    unsigned radius:3;                  /* 0x01, bits 0-2 */
    unsigned b1_3:5;
    char pad2[9 - 2];
    unsigned char b9_0:2;               /* 0x09 */
    unsigned char b9_2:6;
    char padA[0x0B - 0x0A];
};

extern struct Player near *player;
extern struct Object far *ThePlayer;
extern struct Object far *talking_to;
extern struct Object far *ActiveObj;
extern union Link Inventory[];
extern struct Creature Creature[];
extern struct ComObj ComObjData[];
extern void (far *npp_func)();
extern int PlayerLevel;
extern char NewPlyFade;
extern unsigned char stay_centered;
extern int MapObj_X, MapObj_Y;
extern long lastDurCheck;
extern int greed;

int far getmem(int addr);
int far * far getmem_addr(int addr);
struct Tile far * far Map_GetAddr(int x, int y);
struct Object far * far Obj_PtrTMem(union Link far *link);
int far Obj_MemTPtr(struct Object far *obj);
struct Object far * far Obj_IntTMem(int index);
void far Obj_Add(union Link far *head, struct Object far *obj);
void far Obj_AddEnd(union Link far *head, struct Object far *obj);
unsigned char far Obj_Rem(union Link far *head, struct Object far *obj);
struct Object far * far Obj_Punt(union Link far *head, struct Object far *obj, int how);
struct Object far * far Obj_InList(union Link far **head, int recurse, int major, int minor,
                                   int index);
struct Object far * far CreateObj(int item, char mobile);
unsigned char far can_place(int item, int index, int x, int y, int z, char flier, char dist);
unsigned char far put_at(int x, int y, int z, struct Object far *obj, int range,
                         unsigned char nocull);
struct Object far * far obj_deal(struct Object far *obj, int x, int y, char how);
void far creature_obj_init(void);
void far editchng(int bits);
int far useNSpellCharges(struct Object far *obj, int n);
void far gronk_whoami(int whoami, unsigned char all, int arg,
                      char (far *fn)());
unsigned char far speech_available(void);
void far player_get_exp(int n);
int far set_workspace(void);
void far do_teleport(struct Object far *who, int x, int y, int level);
unsigned char far new_player_pos(void);
char far teleport_critter(struct Object far *critter, int x, int y, int how);
void far transform_creature(struct Object far *critter, int a, int b, int c, int d);
char far grant_skill_advance(int which);
char far get_skill(char skill);
void far set_numbered_variable(int var, int how, int val);
int far get_numbered_variable(int var);
void far OpenDoor(struct Object far *who, struct Object far *door);
void far CloseDoor(struct Object far *who, struct Object far *door);
void far ToggleDoor(struct Object far *who, struct Object far *door);

struct Object far * far place_pitfighter(int power, int x, int y);

/* Set when the player runs from a pit fight, read and cleared by babl_hack. */
char running_away = 0;
/* Where teleport_player and teleport_talker asked to go, done by do_babl_teleport once
   the conversation is over; -1 for nothing pending. */
static char tele_level = -1, tele_x = 0, tele_y = 0;
static char talker_x = -1, talker_y = -1;
/* A trade adjustment set by a conversation, read when bartering. */
char fudge = 0;

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
                                           ring (item 0x35) on either hand? */
        struct Object far *ring;

        ring = Obj_PtrTMem(&Inventory[9]);
        if (ring && OBJ_ITEM(ring) == 0x35)
            return 1;
        ring = Obj_PtrTMem(&Inventory[10]);
        if (ring && OBJ_ITEM(ring) == 0x35)
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
        item = rand() % 2 + 0x78;
    else
        item = rand() % 3 + 0x75;
    if (rand() % 3 < power) {
        strong = 1;
        if ((roll = rand() % 5) < 2) {
            item = roll ? 0x7B : 0x5C;
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
        obj->qn.f.low = x;
        SET_HOMEY(obj, y);
        obj->ol.f.low = y;
        SET_Z(obj, tile->height << 3);
        obj->whoami = 0x66;
        SET_POWER(obj, strong);
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
    if (x1 >= 0x40)
        x1 = 0x3F;
    y1 = OBJ_HOMEY(talking_to) + range;
    if (y1 >= 0x40)
        y1 = 0x3F;
    for (y = y0; y <= y1; y++)
        for (x = x0; x <= x1; x++) {
            head = &Map_GetAddr(x, y)->objects;
            for (obj = Obj_PtrTMem(head); obj; obj = Obj_PtrTMem(&obj->qn))
                if (OBJ_ITEM(obj) == item && !OBJ_B0A_7(obj)
                    && Creature[OBJ_TYPE(obj)].race == race)
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
    for (link = &talking_to->ol; link->f.index && link->f.index != index;
         link = &Obj_PtrTMem(link)->qn)
        ;
    if (link->f.index && !Obj_Rem(&talking_to->ol, obj))
        return 1;
    if (x < 0) {
        stay_centered = 1;
        put_at(OBJ_HOMEX(ThePlayer) << 3, OBJ_HOMEY(ThePlayer) << 3, OBJ_Z(ThePlayer), obj, 6, 1);
        stay_centered = 0;
        return 1;
    }
    if (x < 1 || x >= 0x40 || y < 1 || y >= 0x40)
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
    link = &talking_to->ol;
    for (i = 0; i < n && link->f.index; i++)
        link = &Obj_PtrTMem(link)->qn;
    return link->f.index;
}

void far add_to_npc_inv(int far *args)
{
    int index;

    index = getmem(args[-1]);
    Obj_AddEnd(&talking_to->ol, Obj_IntTMem(index));
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
        return player->sched_hour[clock];
    if (clock == 0) {
        player->game_clock += (long)(val - player->sched_hour[clock]) * 0x4B000L;
        lastDurCheck = player->game_clock >> 8;
    }
    player->sched_hour[clock] = val;
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
    if ((door = Obj_InList(&head, 0, 5, 0, -1)) == 0
        && (door = Obj_InList(&head, 0, 7, 0, 0xF)) == 0)
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
        if (*heading != -1 && OBJ_MAJOR(obj) != 5 && ComObjData[OBJ_ITEM(obj)].b9_0 != 2)
            SET_HEADING(obj, *heading);
        if (*owner != -1)
            obj->ol.f.low = *owner;
        if (*flags != -1)
            SET_FLAGS(obj, *flags);
        if (*link != -1)
            obj->ol.f.index = *link;
        if (*flag10 != -1)
            SET_ID_10(obj, *flag10);
        if (*flag9 != -1)
            SET_ID_9(obj, *flag9);
        if (*quality != -1)
            obj->qn.f.low = *quality;
    } else {
        if (*heading != -1 && OBJ_MAJOR(obj) != 5 && ComObjData[OBJ_ITEM(obj)].b9_0 != 2)
            *heading = OBJ_HEADING(obj);
        if (*owner != -1)
            *owner = obj->ol.f.low;
        if (*flags != -1)
            *flags = OBJ_FLAGS(obj);
        if (*link != -1)
            *link = obj->ol.f.index;
        if (*flag10 != -1)
            *flag10 = obj->id & 0x400;
        if (*flag9 != -1)
            *flag9 = obj->id & 0x200;
        if (*quality != -1)
            *quality = obj->qn.f.low;
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
