/* target: ovr107 */
/* opts: -mm -1 -G -O -Y -d */
/* Critters between moments: changing a critter's goal, the yearly checkup, what happens
   to the critters and mobile objects while the player sleeps, monsters that find the
   sleeping player, teleporting a critter, the last critter hit, theft noticed by the
   owners, the pit warriors in the arena of fire, and the castle NPCs' schedule: the whole
   of DOS overlay ovr107, in original order. Function and global names are the originals
   from the FM Towns symbol table; the source file's own name is not known. */

#include <stdlib.h>
#include <string.h>

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0xD6];
    unsigned long questsD6;             /* 0xD6 */
    char padDA[0xE7 - 0xDA];
    unsigned char arena_wins;           /* 0xE7 */
    char padE8[0x308 - 0xE8];
    unsigned char crithit;              /* 0x308 */
    unsigned char typehit;              /* 0x309 */
    long crithittime;                   /* 0x30A */
    unsigned char hitx;                 /* 0x30E */
    unsigned char hity;                 /* 0x30F */
    char pad310[0x360 - 0x310];
    unsigned char pit_fighters[5];      /* 0x360 */
    char pad365[0x369 - 0x365];
    unsigned long game_clock;           /* 0x369 */
    char pad36D[0x36E - 0x36D];
    unsigned char xclock1;              /* 0x36E */
    char pad36F[0x37B - 0x36F];
    unsigned char arena_best;           /* 0x37B */
};

/* A link to an object, with six bits of something else below it. */
union Link {
    unsigned word;
    struct { unsigned low:6, index:10; } f;
};

/* A mobile object, 0x1B bytes. The first 8 bytes are shared with static objects. */
struct Object {
    unsigned id;                        /* item 0-8 (major class 6-8) */
    unsigned pos;                       /* z 0-6, heading 7-9, y fine 10-12, x fine 13-15 */
    union Link qn;                      /* quality 0-5, the next object 6-15 */
    union Link ol;                      /* owner 0-5, the contents or a link 6-15 */
    unsigned char hp;                   /* 0x08 */
    unsigned char b09;                  /* 0x09 */
    unsigned char b0A;                  /* 0x0A */
    unsigned goal_word;                 /* 0x0B, goal in bits 0-3 */
    unsigned attitude_word;             /* 0x0D, attitude in bits 14-15 */
    char pad0F[0x15 - 0x0F];
    unsigned char b15;                  /* 0x15 */
    unsigned home;                      /* 0x16, x in bits 10-15, y in bits 4-9 */
    unsigned char b18;                  /* 0x18 */
    unsigned char b19;                  /* 0x19 */
    unsigned char whoami;               /* 0x1A */
};

#define OBJ_ITEM(o)     ((o)->id & 0x1FF)
#define OBJ_MAJOR(o)    (((o)->id & 0x1C0) >> 6)
#define OBJ_CLASS(o)    (((o)->id & 0x1F0) >> 4)
#define OBJ_MINOR(o)    (((o)->id & 0x30) >> 4)
#define OBJ_MINOR4(o)   ((o)->id & 0xF)
#define OBJ_TYPE(o)     (((o)->id & 0x3F) >> 0)
#define OBJ_Z(o)        ((o)->pos & 0x7F)
#define OBJ_FINEY(o)    (((o)->pos & 0x1C00) >> 10)
#define OBJ_FINEX(o)    (((o)->pos & 0xE000) >> 13)
#define OBJ_HOMEX(o)    (((o)->home & 0xFC00) >> 10)
#define OBJ_HOMEY(o)    (((o)->home & 0x3F0) >> 4)
#define OBJ_GOAL(o)     (((o)->goal_word & 0xF) >> 0)
#define OBJ_ATTITUDE(o) (((o)->attitude_word & 0xC000) >> 14)
#define OBJ_TEMP(o)     (((o)->attitude_word & 0x100) >> 8)
#define OBJ_NOHEAL(o)   (((o)->attitude_word & 0x200) >> 9)
#define OBJ_B0A_7(o)    (((o)->b0A & 0x80) >> 7)
#define OBJ_B19_0(o)    (((o)->b19 & 1) >> 0)

#define SET_Z(o, v)       ((o)->pos = (o)->pos & 0xFF80 | (v) & 0x7F)
#define SET_HEADING(o, v) ((o)->pos = (o)->pos & 0xFC7F | ((v) & 7) << 7)
#define SET_FINEX(o, v)   ((o)->pos = (o)->pos & 0x1FFF | ((unsigned)(v) & 7) << 13)
#define SET_FINEY(o, v)   ((o)->pos = (o)->pos & 0xE3FF | ((v) & 7) << 10)
#define SET_HOMEX(o, v)   ((o)->home = (o)->home & 0x3FF | ((v) & 0x3F) << 10)
#define SET_HOMEY(o, v)   ((o)->home = (o)->home & 0xFC0F | ((v) & 0x3F) << 4)
#define SET_GOAL(o, v)    ((o)->goal_word = (o)->goal_word & 0xFFF0 | (v) & 0xF)
#define SET_ATTITUDE(o, v) ((o)->attitude_word = (o)->attitude_word & 0x3FFF | ((v) & 3) << 14)
#define SET_FED(o, v)     ((o)->b19 = (o)->b19 & 0x7F | ((v) & 1) << 7)
#define SET_B19_0(o, v)   ((o)->b19 = (o)->b19 & 0xFE | ((v) & 1) << 0)


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
    char pad00[4];
    unsigned char avghit;               /* 0x04 */
    char pad05[0x09 - 0x05];
    unsigned char race;                 /* 0x09 */
    unsigned char b0A_0:7;              /* 0x0A */
    unsigned char flier:1;
    char pad0B[0x1C - 0x0B];
    unsigned char b1C_0:4;              /* 0x1C */
    unsigned char hunt:4;
    char pad1D[0x1E - 0x1D];
    unsigned char b1E_0:4;              /* 0x1E */
    unsigned char hearing:4;
    char pad1F[0x30 - 0x1F];
};

/* The common object properties, one 11-byte record per item. */
struct ComObj {
    unsigned height:8;                  /* 0x00 */
    char pad1[6 - 1];
    unsigned c6_0:15;                   /* 0x06 */
    unsigned owned:1;                   /* word 0x06, bit 15 */
    char pad8[0x0B - 0x08];
};

/* One step of the path flood_path found. */
struct PathSq {
    unsigned char x, y;
    char pad2[2];
};

/* The static data of a map square. */
struct StDat {
    char pad0[2];
    unsigned char height;               /* 0x02 */
    char pad3[5 - 3];
};

typedef char (far *SpellFn)(int x, int y, struct Object far *target, struct Tile far *tile,
                            unsigned char src);

extern struct Player near *player;
extern struct Object far *ThePlayer;
extern struct Object far *critdata;
extern unsigned char far *ActiveMob;
extern unsigned char far *LastActiveMob;
extern struct Creature Creature[];
extern struct ComObj ComObjData[];
extern unsigned TxmTerr[];
extern struct Object far *meptr;
extern struct Creature near *mycst;
extern unsigned char pathlen;
extern unsigned char myxpos, myypos;
extern int XP, YP;
extern int MapObj_X, MapObj_Y;
extern unsigned char stay_centered;
/* This file's _BSS, DS:554E (ovr104's ends at 554D; ovr108's starts at 5550): only this file
   uses it, and FM Towns has it as a static. Provisional name. */
static char wander_found;
extern char crithit;
extern char typehit;
extern long crithittime;
extern unsigned char hitx, hity;
extern int freepaths;
extern struct PathSq far pathsq[];
extern struct StDat far stdat[64][64];

void far critter_set_goal(char goal, int gtarg);
void far set_critter_vars(struct Object far *obj);
struct Tile far * far Map_GetAddr(int x, int y);
void far Obj_FreeLinkChain(union Link far *head, struct Object far *obj);
int far Obj_MemTPtr(struct Object far *obj);
struct Object far * far Obj_PtrTMem(union Link far *link);
struct Object far * far Obj_IntTMem(int index);
unsigned char far can_place(int item, int index, int x, int y, int z, char flier, char dist);
unsigned char far Obj_Rem(union Link far *head, struct Object far *obj);
void far Obj_Add(union Link far *head, struct Object far *obj);
struct Object far * far Obj_Punt(union Link far *head, struct Object far *obj, int how);
struct Object far * far mob_to_static(struct Object far *obj);
unsigned char far drop_around_place(struct Object far *obj, int x, int y, int z, int how);
void far gronk_area(struct Object far *who, char count, SpellFn fn, unsigned char type,
                    unsigned char dist, unsigned char radius);
void far process_area(char count, unsigned char src, SpellFn fn, unsigned char type,
                      char x0, char y0, char w, char h);
char far flood_path(char x, char y, int z, int tx, int ty, int tz, int how);
void far UseTrap(struct Object far *trap, int x, int y);
void far set_loc(int x, int y, int z);
char far line_of_sight(int x, int y, int z, int tx, int ty, int tz);
void far get_name(char far *buf, struct Object far *obj, int article, int plural);
char far * far get_string(int id);
char far * far str_cat(char far *dst, char far *src);
void far scroll_print(char far *s);
void far Obj_Check(struct Object far *obj, char (far *fn)(struct Object far *obj));
unsigned char far player_looking(int x, int y);
int far octant(char x, char y);
void far remove_opponent(struct Object far *npc);

void far change_critter_goal(struct Object far *npc, char goal, int gtarg)
{
    struct Object far *save;

    save = meptr;
    meptr = npc;
    critter_set_goal(goal, gtarg);
    meptr = save;
}

/* IDA: SomethingWithUpdatingAllNPCHunger. FM Towns has yearly_checkup next after
   change_critter_goal, and its code is this loop. */
void far yearly_checkup(void)
{
    unsigned char far *p;
    struct Object far *npc;

    for (p = ActiveMob; p < LastActiveMob; p++) {
        npc = &critdata[*p];
        SET_FED(npc, rand() % 2);
        if (rand() % 4 != 1)
            npc->b19 = npc->b19 & 0xBF;
    }
}

unsigned char far set_gridx_and_y_based_on_tile_type(unsigned char type, unsigned char *x,
                                                     unsigned char *y)
{
    switch (type) {
    case 0:
        return 0;
    case 2:
        *x = 6;
        *y = 1;
        break;
    case 3:
        *x = 1;
        *y = 1;
        break;
    case 4:
        *x = 6;
        *y = 6;
        break;
    case 5:
        *x = 6;
        *y = 1;
        break;
    case 1:
    default:
        *x = 4;
        *y = 4;
        break;
    }
    return 1;
}

void far up_crit(struct Object far *npc, char *counts)
{
    struct Tile far *tile;
    struct Tile far *hometile;
    unsigned char xhome;
    unsigned char yhome;
    unsigned char x;
    unsigned char y;
    unsigned char fx;
    unsigned char fy;
    unsigned char z;
    struct Creature near *cst;

    cst = &Creature[OBJ_TYPE(npc)];
    xhome = OBJ_HOMEX(npc);
    yhome = OBJ_HOMEY(npc);
    hometile = Map_GetAddr(xhome, yhome);
    if (OBJ_TEMP(npc)) {
        Obj_FreeLinkChain(&hometile->objects, npc);
        return;
    }
    npc->b19 = npc->b19 & 0x7F;
    npc->b19 = npc->b19 & 0xBF;
    npc->b19 = npc->b19 & 0xEF;
    npc->b19 = npc->b19 & 0xDF;
    npc->b19 = npc->b19 & 0xFE;
    npc->b19 = npc->b19 & 0xFD;
    if (OBJ_GOAL(npc) != 10)
        SET_HEADING(npc, rand() % 8);
    if (npc->hp < cst->avghit && !OBJ_NOHEAL(npc))
        npc->hp = (npc->hp + cst->avghit) / 2;
    if (!OBJ_B0A_7(npc)) {
        switch (OBJ_ATTITUDE(npc)) {
        case 0:
            counts[cst->race] = counts[cst->race] - 1;
            break;
        case 3:
            counts[cst->race] = counts[cst->race] + 1;
            break;
        }
    }
    x = npc->qn.f.low;
    y = npc->ol.f.low;
    tile = Map_GetAddr(x, y);
    if (xhome == x && yhome == y)
        return;
    if (!set_gridx_and_y_based_on_tile_type(tile->type, &fx, &fy))
        return;
    if (cst->flier)
        z = (tile->height << 3) + 0x80 >> 1;
    else
        z = tile->height << 3;
    if (can_place(OBJ_ITEM(npc), Obj_MemTPtr(npc), (x << 3) + fx, (y << 3) + fy, z, cst->flier,
                  8) == 0)
        return;
    if (Obj_Rem(&hometile->objects, npc) == 0)
        return;
    Obj_Add(&tile->objects, npc);
    SET_HOMEX(npc, x);
    SET_HOMEY(npc, y);
    SET_FINEX(npc, fx);
    SET_FINEY(npc, fy);
    SET_Z(npc, z);
}

unsigned char far up_mob(struct Object far *obj)
{
    struct Tile far *tile;
    union Link far *head;
    struct Object far *stopped;
    int x;
    int y;

    if (OBJ_ITEM(obj) == 0x1D || OBJ_ITEM(obj) == 0x13F)
        return 0;
    XP = OBJ_HOMEX(obj);
    x = (XP << 3) + OBJ_FINEX(obj);
    YP = OBJ_HOMEY(obj);
    y = (YP << 3) + OBJ_FINEY(obj);
    tile = Map_GetAddr(XP, YP);
    head = &tile->objects;
    if (!Obj_Punt(head, obj, 0))
        return 1;
    stopped = mob_to_static(obj);
    if (stopped == 0)
        return 1;
    Obj_Rem(head, stopped);
    stay_centered = 1;
    if (!drop_around_place(stopped, x, y, tile->height << 3, 6)) {
        SET_Z(stopped, tile->height << 3);
        Obj_Add(head, stopped);
    }
    return 1;
}

void far update_all_critters_whilst_player_snoozes(void)
{
    struct Object far *npc;
    unsigned char far *p;
    char c;
    char counts[64];
    struct Creature near *cst;

    memset(counts, 0, 0x40);
    for (p = ActiveMob; p < LastActiveMob; p++) {
        npc = &critdata[*p];
        if (OBJ_MAJOR(npc) == 1)
            up_crit(npc, counts);
        else if (up_mob(npc))
            p--;
    }
    for (p = ActiveMob; p < LastActiveMob; p++) {
        npc = &critdata[*p];
        if (OBJ_B0A_7(npc) != 0)
            continue;
        cst = &Creature[OBJ_TYPE(npc)];
        c = counts[cst->race];
        if (c != 0) {
            char att;

            att = OBJ_ATTITUDE(npc) + c;
            if (c < 0) {
                if (att < 0)
                    att = 0;
            } else if (att > 3)
                att = 3;
            SET_ATTITUDE(npc, att);
        }
    }
}

/* Statics: FM Towns keeps these three after curelem, with no names of their own. */
static char hostile_found = 0;

/* IDA: TestForNPCHostileAndAwareOfPlayer. FM Towns has check_for_hostile_creature in this
   place, and hostile_creatures_near passes it to gronk_area as here. */
char far check_for_hostile_creature(int x, int y, struct Object far *target,
                                    struct Tile far *tile, unsigned char src)
{
    struct Object far *npc;

    if (Obj_MemTPtr(target) == Obj_MemTPtr(ThePlayer))
        return 0;
    npc = target;
    if (OBJ_GOAL(npc) == 5 || OBJ_GOAL(npc) == 4 || OBJ_GOAL(npc) == 9)
        if (OBJ_B19_0(npc))
            hostile_found = 1;
    return 0;
}

char far hostile_creatures_near(void)
{
    hostile_found = 0;
    gronk_area(ThePlayer, 0x7F, check_for_hostile_creature, 0, 0, 2);
    return hostile_found;
}

char far wander_that_monster(int x, int y, struct Object far *target, struct Tile far *where,
                             unsigned char src)
{
    unsigned char fx;
    unsigned char fy;
    unsigned char z;
    int dist;
    struct Tile far *tile;
    unsigned char i;
    union Link far *link;
    struct Object far *next;
    struct Object far *trap;
    struct Object far *obj;
    int tx;
    int ty;

    if (Obj_MemTPtr(target) == 1)
        return 0;
    obj = target;
    if (OBJ_ATTITUDE(obj) > 0)
        return 0;
    if (rand() % 2 == 0)
        return 0;
    set_critter_vars(obj);
    dist = (myxpos - OBJ_HOMEX(ThePlayer)) * (myxpos - OBJ_HOMEX(ThePlayer))
         + (myypos - OBJ_HOMEY(ThePlayer)) * (myypos - OBJ_HOMEY(ThePlayer));
    if (mycst->hunt * mycst->hunt * 3 < dist)
        return 0;
    if (flood_path(myxpos, myypos, OBJ_Z(meptr) >> 3, OBJ_HOMEX(ThePlayer),
                   OBJ_HOMEY(ThePlayer), OBJ_Z(ThePlayer) >> 3, 0) && pathlen >= 2) {
        for (i = 0; i < pathlen; i = i + 1) {
            tile = Map_GetAddr(pathsq[i].x, pathsq[i].y);
            for (link = &tile->objects; link->f.index != 0; link = &next->qn) {
                next = Obj_PtrTMem(link);
                if (OBJ_CLASS(next) == 0x1A && next->ol.f.index > 0) {
                    trap = Obj_PtrTMem(&next->ol);
                    if (OBJ_MAJOR(trap) == 6 && OBJ_MINOR(trap) == 0 && OBJ_MINOR4(trap) == 9)
                        UseTrap(trap, pathsq[i].x, pathsq[i].y);
                }
            }
        }
        tx = pathsq[pathlen - 2].x;         /* FM Towns names pathsq - 8 objyloc */
        ty = pathsq[pathlen - 2].y;
        tile = Map_GetAddr(tx, ty);
        if (!set_gridx_and_y_based_on_tile_type(tile->type, &fx, &fy))
            return 0;
        z = stdat[tx][ty].height << 3;
        if (can_place(OBJ_ITEM(obj), Obj_MemTPtr(obj), (tx << 3) + fx, (ty << 3) + fy, z,
                      mycst->flier, 8)) {
            Obj_Rem(&Map_GetAddr(myxpos, myypos)->objects, obj);
            Obj_Add(&tile->objects, obj);
            SET_HOMEX(obj, tx);
            SET_HOMEY(obj, ty);
            SET_FINEX(obj, fx);
            SET_FINEY(obj, fy);
            SET_Z(obj, z);
            SET_B19_0(obj, 1);
            set_loc(OBJ_HOMEX(ThePlayer), OBJ_HOMEY(ThePlayer), OBJ_Z(ThePlayer) >> 3);
            wander_found = 1;
            return 1;
        } else
            return 0;
    }
    return 0;
}

char far teleport_critter(struct Object far *critter, int x, int y, int how)
{
    struct Object far *obj;
    struct Tile far *tile;
    unsigned char fx;
    unsigned char fy;
    unsigned char z;

    obj = critter;
    set_critter_vars(obj);
    if (OBJ_HOMEX(obj) == x && OBJ_HOMEY(obj) == y)
        return 1;
    tile = Map_GetAddr(x, y);
    if (mycst->flier)
        z = (tile->height << 3) + 0x80 >> 1;
    else
        z = tile->height << 3;
    if (!set_gridx_and_y_based_on_tile_type(tile->type, &fx, &fy))
        return 0;
    if (can_place(OBJ_ITEM(obj), Obj_MemTPtr(obj), (x << 3) + fx, (y << 3) + fy, z, mycst->flier,
                  8)) {
        if (!Obj_Rem(&Map_GetAddr(myxpos, myypos)->objects, obj))
            return 0;
        Obj_Add(&tile->objects, obj);
        SET_HOMEX(obj, x);
        SET_HOMEY(obj, y);
        SET_FINEX(obj, fx);
        SET_FINEY(obj, fy);
        SET_Z(obj, z);
        critter->b19 = critter->b19 & 0xEF;
        critter->b19 = critter->b19 & 0xDF;
        critter->b19 = critter->b19 & 0xFE;
        critter->b19 = critter->b19 & 0xFD;
        return 1;
    }
    return 0;
}

char far wandering_monster_check(void)
{
    wander_found = 0;
    gronk_area(ThePlayer, 1, wander_that_monster, 0, 0, 8);
    return wander_found;
}

/* IDA: LoadCombatState. FM Towns' set_creatures_from_saved_game copies the same five
   player fields at 0x308-0x30F into the same globals. */
void far set_creatures_from_saved_game(void)
{
    crithit = player->crithit;
    typehit = player->typehit;
    crithittime = player->crithittime;
    hitx = player->hitx;
    hity = player->hity;
}

/* IDA: SaveRecentCombatAction; FM Towns' set_creatures_to_saved_game, the reverse copy. */
void far set_creatures_to_saved_game(void)
{
    player->crithit = crithit;
    player->typehit = typehit;
    player->crithittime = crithittime;
    player->hitx = hitx;
    player->hity = hity;
}

void far clear_paths(void)
{
    struct Object far *obj;
    int i = 0x25;                       /* a dead store, but the DOS bytes have it */

    for (i = 2; i < 0x100; i++) {
        obj = Obj_IntTMem(i);
        obj->b15 = obj->b15 & 0x7F;
    }
    freepaths = 0xFFFF;
}

void far init_level_creature_stuff(void)
{
    crithit = 0;
    typehit = 0xFF;
    clear_paths();
}

static struct Object far *stolen = 0;
static unsigned char grab_owner = 0;

char far critter_get_told(int x, int y, struct Object far *target, struct Tile far *tile,
                          unsigned char src)
{
    struct Object far *npc;
    int nx;
    int ny;
    int nz;
    int ox;
    int oy;
    int oz;
    int dx;
    int dy;
    char text[80];
    struct Creature near *cst;
    int att;

    npc = target;
    cst = &Creature[OBJ_TYPE(npc)];
    if (cst->race != (grab_owner & 0x1F)
        || OBJ_B0A_7(npc) != 0 && !(grab_owner & 0x20)
        || grab_owner == 0x20 && !OBJ_B0A_7(npc))
        return 0;
    nx = (x << 3) + OBJ_FINEX(npc);
    ny = (y << 3) + OBJ_FINEY(npc);
    nz = OBJ_Z(npc) + ComObjData[OBJ_ITEM(npc)].height;
    ox = (MapObj_X << 3) + OBJ_FINEX(stolen);
    oy = (MapObj_Y << 3) + OBJ_FINEY(stolen);
    oz = OBJ_Z(stolen) + ComObjData[OBJ_ITEM(stolen)].height + 12;
    dx = (nx - ox) / 8;
    dy = (ny - oy) / 8;
    if (dx * dx + dy * dy > cst->hearing * cst->hearing)
        return 0;
    if (line_of_sight(nx, ny, nz, ox, oy, oz)) {
        att = OBJ_ATTITUDE(npc) - 1;
        if (att < 0)
            att = 0;
        SET_ATTITUDE(npc, att);
        get_name(text, npc, 1, 0);
        str_cat(text, get_string(att + 0xF0 | 0x200));
        if (text[0] >= 'a' && text[0] <= 'z')
            text[0] = text[0] - 0x20;
        scroll_print(text);
        return 1;
    }
    return 0;
}

/* IDA: ClearOwnerShip. FM Towns' clear_owner, which player_grabbed passes to Obj_Check. */
char far clear_owner(struct Object far *obj)
{
    if (ComObjData[OBJ_ITEM(obj)].owned)
        obj->ol.f.low = 0;
    return 0;
}

void far player_grabbed(struct Object far *obj, unsigned char owner)
{
    grab_owner = 0;
    if (owner > 0)
        grab_owner = owner;
    else if (ComObjData[OBJ_ITEM(obj)].owned)
        grab_owner = obj->ol.f.low;
    if (grab_owner != 0) {
        stolen = obj;
        process_area(0x14, 0, critter_get_told, 0, MapObj_X - 7, MapObj_Y - 7, 0xF, 0xF);
        if (owner >= 0 && (obj->ol.f.low & 0x1F) <= 0x1D)
            obj->ol.f.low = 0;
        if (OBJ_CLASS(obj) == 8)
            Obj_Check(obj, clear_owner);
    }
}

void far maybe_rescue_guy_from_fire(struct Object far *obj);

void far maybe_cheat_arena_fire(void)
{
    register int i;

    for (i = 0; i < 5; i++)
        maybe_rescue_guy_from_fire(Obj_IntTMem(player->pit_fighters[i]));
}

void far maybe_rescue_guy_from_fire(struct Object far *obj)
{
    int x;
    int y;
    int tx;
    int ty;
    int oct;
    int adj;

    if (obj == 0)
        return;
    x = (OBJ_HOMEX(obj) << 3) + OBJ_FINEX(obj);
    y = (OBJ_HOMEY(obj) << 3) + OBJ_FINEY(obj);
    if (((TxmTerr[Map_GetAddr(x, y)->floor] & 0xC0) >> 6) != 2)
        return;
    if (player_looking(x, y) != 0)
        return;
    oct = octant((char)x - 0x16, (char)y - 0x16);
    adj = oct < 4 ? oct : 7 - oct;
    tx = 0x19 - adj * 2;
    tx = (tx << 3) + 3;
    adj = (oct + 2) % 8;
    if (adj > 3)
        adj = 7 - adj;
    ty = adj * 2 + 0x13;
    ty = (ty << 3) + 3;
    if (player_looking(tx, ty) == 0)
        teleport_critter(obj, tx, ty, 0);
}

/* IDA: DefeatLivingPitWarrior. FM Towns' arena_opponent_runs: the same win count, goal 6
   and call to remove_opponent. */
void far arena_opponent_runs(struct Object far *obj)
{
    player->arena_wins++;
    if (player->arena_wins > player->arena_best)
        player->arena_best = player->arena_wins;
    SET_GOAL(obj, 6);
    remove_opponent(obj);
}

void far where_shall_we_hang_out(struct Object far *npc, int *x, int *y);

char far maybe_go_hang_out(struct Object far *npc)
{
    int x;
    int y;

    where_shall_we_hang_out(npc, &x, &y);
    npc->qn.f.low = x;
    npc->ol.f.low = y;
    SET_GOAL(npc, 1);
    if (!player_looking((OBJ_HOMEX(npc) << 3) + OBJ_FINEX(npc),
                        (OBJ_HOMEY(npc) << 3) + OBJ_FINEY(npc)))
        if (!player_looking((x << 3) + 3, (y << 3) + 3))
            teleport_critter(npc, x, y, 0);
    return 1;
}

/* IDA: CastleNPC_Schedule. FM Towns' where_shall_we_hang_out, called by maybe_go_hang_out
   with the same time of day switch and castle tables. */
void far where_shall_we_hang_out(struct Object far *npc, int *x, int *y)
{
    int hour;
    int x1;
    int y1;
    int x2;
    int y2;
    int stage;
    int loc;

    hour = (int)(player->game_clock / 0xE1000L) % 24;
    stage = (((hour + 1) >> 1) + 11) % 12;
    loc = rand() % 6;
    if (rand() % 3 == 0 || stage <= 2)
        switch (stage) {
        case 3: case 5: case 9:
            loc = 4;
            break;
        case 0: case 1: case 2: case 11:
            loc = 0;
            break;
        case 6: case 7: case 8:
            loc = 1;
            break;
        }
    if (npc->whoami == 0x88 || player->xclock1 == 0)
        loc = 0;
    else if (npc->whoami == 0x8E && (int)((player->questsD6 & 8) >> 3))
        loc = 0;
    else if (npc->whoami == 0x82 && player->xclock1 >= 0xC)
        loc = 0;
    if (loc == 5) {
        *x = npc->qn.f.low;
        *y = npc->ol.f.low;
    } else if (loc == 0) {
        char xs[14] = { 0x2A, 0x24, 0x15, 0x25, 0x16, 0x19, 0x1B,
                        0x2C, 0x2B, 0x16, 0x15, 0x18, 0x1A, 0x19 };
        char ys[14] = { 0x2B, 0x33, 0x2A, 0x23, 0x33, 0x2B, 0x24,
                        0x30, 0x31, 0x25, 0x22, 0x27, 0x30, 0x22 };

        if (npc->whoami == 0xA8) {
            *x = 0x2A;
            *y = 0x24;
        } else {
            *x = xs[npc->whoami - 0x82];
            *y = ys[npc->whoami - 0x82];
        }
    } else {
        switch (loc) {
        case 1:
            x1 = 0x1B;
            y1 = 0x22;
            x2 = 0x23;
            y2 = 0x25;
            break;
        case 2:
            x1 = 0x1E;
            y1 = 0x27;
            x2 = 0x20;
            y2 = 0x2B;
            break;
        case 3:
            x1 = 0x1D;
            y1 = 0x2D;
            x2 = 0x21;
            y2 = 0x33;
            break;
        case 4:
            x1 = 0x23;
            y1 = 0x2F;
            x2 = 0x2F;
            y2 = 0x34;
            /* no break: the DOS bytes fall into the default */
        default:
            *x = OBJ_HOMEX(npc);
            *y = OBJ_HOMEY(npc);
            return;
        }
        *x = x1 + rand() % (x2 - x1 + 1);
        *y = y1 + rand() % (y2 - y1 + 1);
        if (loc == 2 && *x == 0x1F && *y == 0x29)
            (*x)++;
    }
}
