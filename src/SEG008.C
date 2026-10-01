/* target: seg008_1B09 */
/* opts: -mm -1 -G -O -Y -d */
/* The player's physics: the terrain the player stands in, the simple step-and-turn
   movement, the per-tick motion setup from the input and the floor, applying the result
   to the player object, and moving the player between tiles. The whole of DOS resident
   segment seg008_1B09, in original order. Function and global names are the originals
   from the FM Towns symbol table where it has them. */

#include <stdlib.h>


/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0x21];
    unsigned char skills[20];           /* 0x21: acrobat 0x11, swimming 0x13 */
    char pad35[0x4A - 0x35];
    unsigned weight;                    /* 0x4A, carried */
    unsigned max_weight;                /* 0x4C */
    char pad4E[0x60 - 0x4E];
    unsigned b60:1;                     /* 0x60 */
    unsigned poison:4;
    unsigned active_spells:4;
    unsigned b60_9:3;
    unsigned shrooms:2;
    unsigned drunk:6;                   /* word 0x61, bits 6..11 */
    unsigned automap:1;                 /* word 0x62, bit 4 */
    unsigned b62_5:1;
    unsigned sleepbits:3;
    unsigned in_void:1;
    unsigned in_pits:1;
    unsigned b63_3:5;
    char pad64[0x303 - 0x64];
    unsigned fps:3;                     /* 0x303, the motion state newFPS last set */
    unsigned b303_3:5;
    char pad304;
    unsigned char paralyzed;            /* 0x305 */
    unsigned char motion_state;         /* 0x306: 1 water, 4 ice */
    unsigned char swim_count;           /* 0x307 */
};

/* A mobile object, 27 bytes. The first 8 bytes are shared with static objects. */
struct Object {
    unsigned id;                        /* item 0-8 */
    unsigned pos;                       /* z 0-6, heading 7-9, y fine 10-12, x fine 13-15 */
    union {
        unsigned word;
        struct { unsigned quality:6, next:10; } f;
    } qn;
    unsigned ol;
    unsigned char hp;                   /* 0x08 */
    char pad09[0x0B - 0x09];
    unsigned goal_word;                 /* 0x0B, animation frame in bits 12-15 */
    char pad0D[0x15 - 0x0D];
    unsigned char b15;                  /* 0x15 */
    unsigned home;                      /* 0x16, x in bits 10-15, y in bits 4-9 */
    unsigned char b18;                  /* 0x18, fine heading in bits 0-4 */
    char pad19[0x1B - 0x19];
};

#define SET_Z(o, v)       ((o)->pos = (o)->pos & 0xFF80 | (v) & 0x7F)
#define SET_HEADING(o, v) ((o)->pos = (o)->pos & 0xFC7F | ((v) & 7) << 7)
#define SET_FINEX(o, v)   ((o)->pos = (o)->pos & 0x1FFF | ((unsigned)(v) & 7) << 13)
#define SET_FINEY(o, v)   ((o)->pos = (o)->pos & 0xE3FF | (v) << 10)
#define SET_HOMEX(o, v)   ((o)->home = (o)->home & 0x3FF | ((v) & 0x3F) << 10)
#define SET_HOMEY(o, v)   ((o)->home = (o)->home & 0xFC0F | ((v) & 0x3F) << 4)
#define SET_FRAME(o, v)   ((o)->goal_word = (o)->goal_word & 0xFFF | ((v) & 0xF) << 12)
#define SET_FINEHEAD(o, v) ((o)->b18 = (o)->b18 & 0xE0 | ((v) & 0x1F) << 0)

struct Tile {
    unsigned type:4;
    unsigned height:4;
    unsigned light:2;
    unsigned floor:4;
    unsigned b14:2;
    unsigned objects;                   /* 0x02, head of the tile's object list */
};

/* The common object properties, one 11-byte record per item. */
struct ComObj {
    unsigned height:8;                  /* 0x00 */
    unsigned radius:3;
    unsigned c0_11:5;
    char pad2[11 - 2];
};

/* The player's motion record. */
struct Motion {
    int x, y, z;                        /* 0x00 */
    int vx, vy;                         /* 0x06 */
    int pitch;                          /* 0x0A */
    int dx, dy, dz;                     /* 0x0C */
    int speed;                          /* 0x12 */
    int momentum;                       /* 0x14 */
    unsigned char b16;                  /* 0x16 */
    unsigned char b17;                  /* 0x17 */
    char pad18[0x1E - 0x18];
    int heading;                        /* 0x1E */
    int w20;                            /* 0x20 */
    char pad22[0x24 - 0x22];
    unsigned char b24;                  /* 0x24 */
    unsigned char terr;                 /* 0x25 */
    unsigned fall;                      /* 0x26 */
};

/* The player's motion handler record. */
struct PhysThing {
    int flags;                          /* 0x00 */
    int w2;                             /* 0x02 */
    char pad4[8 - 4];
    char (far *handler)(unsigned *w);   /* 0x08 */
};

/* The motion calculation record, reached through `curP`. */
struct MotionCalc {
    int x, y, z;                        /* 0x00 */
    char pad6[8 - 6];
    unsigned char radius;               /* 0x08 */
    unsigned char height;               /* 0x09 */
    int index;                          /* 0x0A */
    unsigned hits0, hits1;              /* 0x0C */
    char pad10[0x15 - 0x10];
    unsigned char count;                /* 0x15, collisions found */
    signed char first;                  /* 0x16, the first of them in oCollisions */
    char pad17;
};

/* One collision found by ObjectCheck, 6 bytes. */
struct Collision {
    unsigned char top;                  /* 0x00 */
    unsigned char bottom;               /* 0x01 */
    unsigned link;                      /* 0x02 */
    int offset;                         /* 0x04 */
};

extern struct Player near *player;
extern struct Object far *ThePlayer;
extern struct Motion PN;
extern struct PhysThing PT;
extern struct MotionCalc near *curP;
extern struct Collision oCollisions[];
extern struct ComObj ComObjData[];
extern struct Tile far *mapdata;
extern unsigned TxmTerr[];
extern int hgt_val[];
extern unsigned char tile_walls[];
extern unsigned char PlayersMap[];
extern unsigned long far *Time;
extern int PlayerFacing;
extern int PlayerHeading;
extern int PlayerLevel;
extern int playerMod;
extern int PlayerInput;
extern int PlayerTurn;
extern int ForwInpRate;
extern int TurnInpRate;
extern int nvokTerr;
extern int nvokHgt;
extern char light_mod;
extern char light_act;
extern char loc_lght;
extern unsigned char light_hi;
extern unsigned char last_light;

/* Uninitialised data, DS:229A onwards. Turbo C lays _BSS out in an order set by the
   names, not by declaration. The publics are FM Towns names; the statics have none there
   (FM Towns keeps them as unnamed statics), so old_dz, lasth, lasts and saved_dz were chosen
   from names compiled as probes because they land where the EXE has them. saved_dz is
   the four bytes at DS:22B0, which nothing in the game references. */
int oldh;                               /* the heading before this tick's ice or current */
int olds;                               /* the speed before it */
unsigned char frictionless;             /* standing on ice */
static int old_dz;                      /* PN.dz when the motion was set up */
int pTurn;                              /* the turn rate for the current motion state */
int GrSq;                               /* the player's tile, as an index into mapdata */
int pFPS[3];                            /* forward, side and backward speeds */
static int lasth;                       /* the heading and speed a current set, or -1 */
static int lasts;
static long saved_dz;
int lastTerr;                           /* the terrain bits parse_player_terr last saw */

void far punt_fightmode(void);
void far fill_FB(int colour);
void far set_effect(int which, char amount);
void far set_screen_frame(int which, int frame);
void far move_along(int heading, int dist, int *x, int *y);
unsigned char far can_place(int item, int index, int x, int y, int z, char b, int dist);
void far ObjectCheck(int a, int b);
void far process_objlist(void);
struct Object far * far Obj_PtrTMem(unsigned far *link);
void far UseTrigger(struct Object far *who, void far *a, struct Object far *trig, int how);
void far cFstSinCos(int heading, int *x, int *y);
int far cSqRt(long v);
int far cAtan2(int x, int y);
int far Obj_MemTPtr(struct Object far *obj);
void far TerrainCheck(char a);
unsigned char far set_resterr(int bits);
void far parse_effect(void);
int far skill_check(int value, int target);
char far damage_item(struct Object far *obj, struct Object far *who, int x, int y,
                     unsigned char damage, unsigned char type);
void far play_effect_here(int fx, int vol, char c);
void far Obj_Rem(unsigned far *list, struct Object far *obj);
void far Obj_Add(unsigned far *list, struct Object far *obj);
char far check_pplate(struct Object far *obj, struct Tile far *tile, int z, int how);
void far set_light(int level);

void far newFPS(char state);
void far change_GrSq(int sq, int z);
void far hgt_change(struct Object far *obj, struct Tile far *tile, int z);
void far player_newsq(int sq);
/* IDA's StopPlayerMotion. Named from FM Towns: player_sqhandler_ follows player_newsq_
   there and is the same test (bit 0x1000, no pitch, slow, lastTerr & 0xA) clearing PN+6
   and PN+8; player_setup stores it as PT's handler, as FM Towns does. */
char far player_sqhandler(unsigned *w);
/* IDA's CalculateMotionFromCommand. Named from FM Towns: do_player_input_ is next there,
   is called by set_player_phys_params_ with PlayerInput as here, and has the same
   14-entry switch on the input. */
void far do_player_input(int input, int rate, int *speed);

/* Initialised data, DS:C8 onwards. */
int MaxPlayerAccel = 0x60;
int Back_FPS = 0xBC;
int Side_FPS = 0xEB;
int Run_FPS = 0x3AC;
int plyMoType = 0;
unsigned char motionbits = 0;
unsigned char fiz_update = 1;
/* The kind of slope or current carrying the player, -1 for none. No FM Towns name. */
static int slide = -1;

unsigned char far water_set(int terr)
{
    unsigned char swimming;

    swimming = 0;
    if (terr & 2)
    {
        swimming = 1;
        player->swim_count = 0x60;
        punt_fightmode();
    }
    else
        player->swim_count = 0x10;
    return swimming;
}

void far parse_player_terr(int terr, char force)
{
    unsigned char swimming;
    int state;

    if (lastTerr != terr || force)
    {
        swimming = 0;
        state = 0;
        lastTerr = terr;
        if (terr & 0x22)
        {
            if ((motionbits & 8) == 0)
            {
                swimming = water_set(terr);
                state = 1;
            }
        }
        else if (terr & 4)
            state = 2;
        else if (terr & 8)
            state = 3;
        else if (terr & 0x10)
        {
            if (motionbits & 4)
                state = 4;
            else if (motionbits & 0x10)
                state = 5;
            else if (motionbits & 2)
                state = 6;
        }
        newFPS(state);
        if (!swimming)
            player->swim_count = 0;
    }
    if (terr & 0x10)
    {
        if (motionbits & 0x14)
        {
            PN.dz = 0;
            if (abs(PN.pitch) > 10)
                PN.pitch = (PN.pitch << 2) / 5;
            else
                PN.pitch = 0;
        }
        else
        {
            if (PN.dz == 0)
                PN.dz = -4;
            if (motionbits & 2 && PN.pitch <= -94)
            {
                PN.pitch = -94;
                if (PN.momentum > 20)
                    PN.momentum = PN.momentum / 2;
                else
                    PN.momentum = 0;
            }
        }
    }
}

/* No FM Towns counterpart and no callers: a flash, a hit to the player's hit points
   scaled by how many are left, a shake and a random compass frame. */
void far unreferenced_seg008_1B09_160(void)
{
    int hp;

    fill_FB(0x20);
    hp = ThePlayer->hp;
    if (hp > 100)
        hp -= rand() % 6;
    else if (hp > 50)
        hp -= rand() % 4;
    else if (hp > 20 && (rand() & 3) < 1)
        hp -= rand() % 3;
    else if (hp > 1 && !(rand() & 7))
        hp--;
    ThePlayer->hp = hp;
    if (rand() % 12)
        set_effect(0x40, rand() % 30 + 15);
    set_screen_frame(2, rand() & 0xF);
}

char far simple_fizix(int turn)
{
    int heading;
    int x;
    int y;
    unsigned char backwards;
    unsigned char flying;
    int sq;
    struct MotionCalc near *oldP;
    struct Object far *obj;
    struct MotionCalc calc;
    register int i;
    register int dist;

    if (PN.dz == 0 && PN.momentum < MaxPlayerAccel)
    {
        backwards = 0;
        flying = 0;
        switch (turn)
        {
        case 2:
            backwards = 1;
        case 0:
            dist = 0x80;
            heading = PlayerFacing / 256;
            break;
        case -2:
            dist = 0x40;
            heading = (unsigned)(PlayerFacing + 0x8000) >> 8;
            break;
        default:
            if (PlayerFacing & 0x1FFF)
                PlayerFacing = (PlayerFacing & 0xE000) + ((turn > 0 ? 1 : 0) << 13);
            else
                PlayerFacing = PlayerFacing + (turn << 13);
            SET_HEADING(ThePlayer, PlayerFacing >> 13);
            SET_FINEHEAD(ThePlayer, PlayerFacing >> 8);
            return 1;
        }
        if (player->motion_state & 4)
            dist /= 16;
        if (motionbits & 0x14)
            flying = 1;
        x = PN.x;
        y = PN.y;
        move_along(heading, dist, &x, &y);
        if (can_place(0x7F, 1, x / 32, y / 32, ThePlayer->pos & 0x7F, backwards | flying, 8))
        {
            if (!backwards && nvokTerr != 1 && nvokTerr != lastTerr
                && (nvokTerr != 0x10 || !flying))
                return 0;
            {
                PN.x = x;
                PN.y = y;
                if ((sq = (PN.x >> 8) + ((PN.y >> 8) << 6)) != GrSq)
                {
                    change_GrSq(sq, nvokHgt);
                    SET_HOMEX(ThePlayer, PN.x >> 8);
                    SET_HOMEY(ThePlayer, PN.y >> 8);
                }
                SET_FINEX(ThePlayer, (PN.x >> 5) & 7);
                SET_FINEY(ThePlayer, (PN.y >> 5) & 7);
                if (!(backwards || flying) || (PN.z >> 3) - 8 <= nvokHgt)
                {
                    SET_Z(ThePlayer, nvokHgt);
                    PN.z = nvokHgt << 3;
                }
                else if (PN.dz == 0 && !flying)
                    PN.dz = -4;
                parse_player_terr(nvokTerr, 0);
                SET_FRAME(ThePlayer, ((unsigned)*Time & 0xFF) >> 6);
                oldP = curP;
                curP = &calc;
                curP->index = 1;
                curP->radius = ComObjData[0x7F].radius;
                curP->height = ComObjData[0x7F].height;
                curP->x = x / 32;
                curP->y = y / 32;
                curP->z = ThePlayer->pos & 0x7F;
                ObjectCheck(0, 0);
                process_objlist();
                for (i = curP->first; i < curP->first + curP->count; i++)
                {
                    obj = Obj_PtrTMem(&oCollisions[i].link);
                    if ((obj->id & 0x1FF) == 0x1A0)
                        UseTrigger(ThePlayer, 0L, obj, 0);
                }
                curP = oldP;
                return 1;
            }
        }
    }
    return 0;
}

/* IDA's ApplyWaterCurrentIceSliding. Named from FM Towns: munge_vectors_ sits between
   simple_fizix_ and set_player_phys_params_ there, takes the same eight arguments
   (kind 2 ice or 3 current, two heading/speed pairs, a strength, two results) and does
   the same sin/cos, blend, square root and atan2. */
void far munge_vectors(int type, int heading, int speed, int heading2, int speed2,
                       int strength, int *outh, register int *outs)
{
    int x1;
    int y1;
    int x2;
    int y2;
    int dxa;
    int dya;
    int vx;
    long lx;
    long ly;
    register int vy;

    if (strength <= 0)
    {
        *outs = speed;
        *outh = heading;
        return;
    }
    if (strength > 0x40)
        strength = 0x40;
    cFstSinCos(heading, &x1, &y1);
    cFstSinCos(heading2, &x2, &y2);
    x1 /= 0x800;
    y1 /= 0x800;
    x2 /= 0x800;
    y2 /= 0x800;
    x1 *= speed;
    y1 *= speed;
    x2 *= speed2;
    y2 *= speed2;
    switch (type)
    {
    case 2:
        dxa = x2 - x1;
        dya = y2 - y1;
        vx = x1 + (int)((long)dxa * strength / 0x40);
        vy = y1 + (int)((long)dya * strength / 0x40);
        break;
    case 3:
        vx = x2 + x1;
        vy = y2 + y1;
        break;
    }
    lx = vx / 16;
    ly = vy / 16;
    *outs = cSqRt(lx * lx + ly * ly);
    if (*outs <= 1)
        *outs = 0;
    if (*outs != 0)
    {
        lx *= 0x7FFF;
        ly *= 0x7FFF;
        lx = lx / *outs;
        ly = ly / *outs;
        vx = lx;
        vy = ly;
        *outh = cAtan2(vx, vy);
    }
    else
        *outh = heading;
}

void far set_player_phys_params(int rate)
{
    int speed;
    int angle;
    int newh;
    int news;
    int bits;
    register int t;
    register int d;

    speed = 0;
    PN.b16 = 5;
    PN.b17 = 0;
    oldh = PN.heading;
    olds = PN.momentum;
    if (PN.dz == 0)
    {
        do_player_input(PlayerInput, rate, &speed);
        if (PN.dz == 0)
        {
            d = speed - PN.momentum;
            if (abs(d) > MaxPlayerAccel)
                d = (d > 0 ? 1 : -1) * MaxPlayerAccel;
            PN.momentum += d;
            if (PN.momentum > pFPS[0])
                PN.momentum = pFPS[0];
            else if (PN.momentum < 0)
                PN.momentum = 0;
        }
    }
    if (PN.dz != 0)
        PlayerFacing += rate * PlayerTurn * (TurnInpRate / 4) / 4;
    if (player->motion_state & 4)
    {
        bits = (TxmTerr[mapdata[GrSq].floor] & 0x38) >> 3;
        if (bits != 7)
        {
            frictionless = 1;
            t = mapdata[GrSq].type;
            if (t >= 6 && t <= 9)
            {
                slide = t - 6;
                angle = 0x2F;
            }
            PN.b16 = 0xF - bits;
            bits = (bits << 3) + 8 - (olds / 0x2F << 2);
            if (olds < 0x2F)
                bits += 0x10;
            else if (PN.momentum > olds)
            {
                if (slide == -1)
                    bits += 8;
                else
                    bits -= 4;
            }
            munge_vectors(2, oldh, olds, PlayerHeading, PN.momentum, bits, &newh, &news);
            PlayerHeading = newh;
            PN.momentum = news;
            if (olds + MaxPlayerAccel <= PN.momentum && !(rand() & 3)
                || olds - MaxPlayerAccel >= PN.momentum)
                set_effect(0x80, 4);
        }
        else
        {
            slide = -1;
            frictionless = 0;
        }
    }
    else if (player->motion_state & 1)
    {
        t = slide;
        slide = ((TxmTerr[mapdata[GrSq].floor] & 0x38) >> 3) - 1;
        if (slide == -1)
            slide = t;
        angle = 0x8D;
    }
    else
        slide = -1;
    lasts = -1;
    if (slide != -1)
    {
        t = 0;
        if (slide <= 3)
        {
            if (slide > 1)
                t = 0x4000;
            if (!(slide & 1))
                t = t + 0x8000;
        }
        newh = angle;
        oldh = PlayerHeading;
        olds = PN.momentum;
        munge_vectors(3, PlayerHeading, PN.momentum, t, newh, 0x20, &oldh, &olds);
        if (angle != 0x2F)
        {
            lasth = PlayerHeading;
            lasts = PN.momentum;
        }
        if (olds > pFPS[0])
            olds = pFPS[0];
        PlayerHeading = oldh;
        PN.momentum = olds;
    }
    PN.speed = rate;
    if (!(PN.dx | PN.dy | PN.dz) && (player->motion_state & 0x84) == 0)
        PN.b17 = 0x80;
    else if (PN.dz != 0 && (player->motion_state & 0x84))
        PN.b16 = 5;
    if (PN.momentum == 0)
        PlayerHeading = PlayerFacing;
    PN.heading = PlayerHeading;
    PT.flags = 0;
    if (motionbits & 0x14)
    {
        PT.flags = 0x1000;
        PN.b17 = 0x80;
    }
    PN.fall = 0;
    old_dz = PN.dz;
}

void far player_setup(int x, int y, int how)
{
    int sq;
    struct MotionCalc calc;

    change_GrSq(-1, -1);
    PT.handler = player_sqhandler;
    PT.w2 = 0x1100;
    PT.flags = 0;
    PN.momentum = 0;
    PN.dx = PN.dy = PN.dz = 0;
    PN.vx = PN.vy = PN.pitch = 0;
    PN.x = (x << 8) + 0x80;
    PN.y = (y << 8) + 0x80;
    PN.b24 = 8;
    PN.w20 = 1;
    GrSq = x + (y << 6);
    PN.z = hgt_val[mapdata[GrSq].height];
    if (tile_walls[mapdata[GrSq].type] & 0x20)
        PN.z += 0x20;
    if (how != -1 && 1000 - ComObjData[0x7F].height > PN.z)
    {
        PN.z = 1000 - (ComObjData[0x7F].height << 3);
        PN.dz = -4;
    }
    SET_Z(ThePlayer, PN.z >> 3);
    SET_HOMEX(ThePlayer, x);
    SET_HOMEY(ThePlayer, y);
    SET_FINEX(ThePlayer, 3);
    SET_FINEY(ThePlayer, 3);
    ThePlayer->b15 = ThePlayer->b15 & 0xC0 | 1;
    ThePlayer->qn.f.next = ThePlayer->qn.f.quality = 0;
    curP = &calc;
    curP->index = Obj_MemTPtr(ThePlayer);
    curP->radius = ComObjData[0x7F].radius;
    curP->height = ComObjData[0x7F].height;
    curP->x = (x << 3) + 3;
    curP->y = (y << 3) + 3;
    curP->z = PN.z >> 3;
    TerrainCheck(PN.b24);
    PN.terr = set_resterr(curP->hits0 | curP->hits1);
    parse_player_terr(PN.terr, 0);
    playerMod = 0;
    parse_effect();
    fiz_update = 1;
    sq = GrSq;
    GrSq = -1;
    change_GrSq(sq, -1);
}

void far phys_affect_player(void)
{
    int sq;
    int dmg;
    int vol;
    register int h;
    register int z;

    z = ThePlayer->pos & 0x7F;
    if (lasts != -1)
    {
        PlayerHeading = lasth;
        PN.momentum = lasts;
    }
    SET_FINEX(ThePlayer, (PN.x >> 5) & 7);
    SET_FINEY(ThePlayer, (PN.y >> 5) & 7);
    SET_FRAME(ThePlayer, ((unsigned)*Time & 0xFF) >> 6);
    if ((sq = (PN.x >> 8) + ((PN.y >> 8) << 6)) != GrSq)
    {
        change_GrSq(sq, PN.z >> 3);
        SET_HOMEX(ThePlayer, PN.x >> 8);
        SET_HOMEY(ThePlayer, PN.y >> 8);
        player_newsq(GrSq);
    }
    else if (PN.z >> 3 != z)
        hgt_change(ThePlayer, mapdata + GrSq, PN.z >> 3);
    if (PN.fall && PN.heading == PlayerHeading && PN.dz == 0
        && (player->motion_state & 4) == 0 && lasts == -1)
        PN.momentum = 0;
    if (PN.heading != PlayerHeading)
    {
        PlayerHeading = PN.heading;
        h = PN.heading - (plyMoType << 14);
        if ((PN.b17 & 0x80) && slide == -1)
        {
            if (abs(PlayerFacing - h) < 0x600)
                PlayerFacing = h;
            else if ((unsigned)(PlayerFacing - h) < 0x7FFF)
                PlayerFacing -= 0x600;
            else
                PlayerFacing += 0x600;
        }
    }
    SET_HEADING(ThePlayer, PlayerFacing >> 13);
    SET_FINEHEAD(ThePlayer, PlayerFacing >> 8);
    if (PN.fall)
    {
        if (PN.b16 > 0)
        {
            dmg = PN.fall >> 8;
            if ((PlayerLevel - 1) / 8 == 8)
                dmg = 0;
            if (PN.pitch != 0)
                dmg <<= 1;
            if (skill_check(player->skills[0x11], dmg << 1) > 0)
                dmg = dmg * (30 - player->skills[0x11]) / 30;
            if (dmg > 3)
                damage_item(ThePlayer, 0L, 0, 0, dmg, 0);
            if (dmg > 1 || (PN.terr & 0x10))
            {
                vol = (dmg << 2) - 60;
                play_effect_here(0xF, 0x40, vol);
            }
        }
        PN.fall = 0;
    }
    parse_player_terr(PN.terr, 0);
    if ((player->motion_state & 4) || (player->motion_state & 1) && slide != -1)
        fiz_update = 1;
    else
        fiz_update = 0;
}

void far player_newsq(int sq)
{
    unsigned char m;

    m = PlayersMap[sq];
    if (!player->automap && (m & 0xF) < 10 && (m & 0xF))
        player->automap = 1;
}

char far player_sqhandler(unsigned *w)
{
    if ((*w & 0x1000) && PN.pitch == 0 && PN.momentum * 10 < pFPS[0] * 3 && !(lastTerr & 0xA))
    {
        PN.vx = PN.vy = 0;
        return 1;
    }
    return 0;
}

void far do_player_input(int input, int rate, register int *speed)
{
    register int h;

    if (player->paralyzed)
    {
        *speed = 0;
        return;
    }
    h = PlayerFacing;
    switch (input)
    {
    case 1:
        PlayerFacing += rate * PlayerTurn * (TurnInpRate / 4) / 4;
        PlayerHeading = h = PlayerFacing;
        *speed = pFPS[0] * (ForwInpRate >> 2) / 32;
        plyMoType = 0;
        break;
    case 10:
        h += 0x4000;
        *speed = pFPS[1];
        plyMoType = 1;
        break;
    case 9:
        h -= 0x4000;
        *speed = pFPS[1];
        plyMoType = -1;
        break;
    case 8:
        h -= 0x8000;
        *speed = pFPS[2];
        plyMoType = -2;
        break;
    case 6:
        if (PN.pitch != 0 || PN.dz != 0 || PN.momentum != 0)
            break;
        PlayerHeading = h = PlayerFacing;
        PN.momentum = *speed = pFPS[0] / 2;
        plyMoType = 0;
    case 7:
        PN.pitch = 0x263;
        if (PN.z > 0x280)
        {
            PN.pitch = PN.pitch * 5 / 6;
            if (PN.z > 0x2C0)
                PN.pitch = (PN.pitch << 1) / 3;
        }
        if (motionbits & 1)
            PN.dz = -2;
        else
            PN.dz = -4;
        return;
    case 12:
        plyMoType = 0;
        PN.pitch = 0x8D;
        PN.dz = 0;
        break;
    case 13:
        plyMoType = 0;
        PN.pitch = -0x8D;
        PN.dz = 0;
        break;
    case 0:
        *speed = 0;
        return;
    }
    PlayerHeading = h;
}

/* IDA's StopFalling. Named from FM Towns: phys_bounce_up_ is next there and does the
   same (for the player, pitch 0x8D unless jumping, dz 0). */
void far phys_bounce_up(struct Object far *obj)
{
    if (obj == ThePlayer)
    {
        if ((PN.terr & 0x10) == 0)
            PN.pitch = 0x8D;
        PN.dz = 0;
    }
}

void far fizix_update(void)
{
    parse_player_terr(PN.terr, 1);
    fiz_update = 1;
}

void far newFPS(char state)
{
    unsigned char ratios[7] = { 20, 6, 14, 20, 1, 14, 4 };
    unsigned char trans[7] = { 0, 1, 2, 4, 8, 8, 0 };
    register int r;

    if (state == -1)
        state = player->fps;
    else
    {
        player->motion_state = (player->motion_state & 0xE0) + trans[state];
        player->fps = state;
    }
    if (state == 1)
        r = player->skills[0x13] / 2 + 4;
    else
        r = ratios[state];
    pFPS[0] = Run_FPS * r / 20;
    pFPS[1] = Side_FPS * r / 20;
    pFPS[2] = Back_FPS * r / 20;
    if (state < 4)
        pTurn = PlayerTurn * ratios[state] / 20;
    else
        pTurn = PlayerTurn;
    if (player->max_weight && player->weight * 2 > player->max_weight)
        MaxPlayerAccel = 0x60 - player->weight * 0x60 / (player->max_weight * 2);
    else
        MaxPlayerAccel = 0x60;
}

void far change_GrSq(int sq, register int z)
{
    int flag;

    if (GrSq >= 0)
    {
        Obj_Rem(&(mapdata + GrSq)->objects, ThePlayer);
        check_pplate(ThePlayer, mapdata + GrSq, PN.z >> 3, 0xE);
    }
    GrSq = sq;
    if (z == -1)
        z = PN.z >> 3;
    else
        SET_Z(ThePlayer, z);
    if (GrSq >= 0)
    {
        Obj_Add(&(mapdata + GrSq)->objects, ThePlayer);
        check_pplate(ThePlayer, mapdata + GrSq, z, 6);
        if (light_mod)
        {
            flag = (mapdata + GrSq)->light & 1;
            if (last_light != flag)
            {
                last_light = flag;
                loc_lght = (last_light ^ light_hi) ? light_mod : -1;
                set_light(light_act > loc_lght ? light_act : loc_lght);
            }
        }
    }
}

void far hgt_change(struct Object far *obj, struct Tile far *tile, int z)
{
    char r = 1;
    register int oldz = obj->pos & 0x7F;

    SET_Z(obj, z);
    r = check_pplate(obj, tile, oldz, 0xF);
    if (r)
        check_pplate(obj, tile, z, 7);
}
