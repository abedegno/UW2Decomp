/* target: seg006_1413 */
/* opts: -mm -1 -G -O -Y -d */
/* Critter motion, homing projectiles, path traversal and doors: the whole of DOS resident
   segment seg006_1413, in original order. Function names are the FM Towns originals; the
   FM Towns build has the functions in the same order, which fixes the names of the ones
   the target table lists under IDA names: store_targ (HomingDartTargeting), crit_hndlr_fly
   and crit_hndlr_swim (FlierNPCCollision, SwimmerNPCCollision), crit_hndlr_obj (static
   here, the tail of crit_hndlr_walk's range; init_ai installs the four handlers in FM
   Towns as in DOS), hyp_move (TraverseMultipleTiles), make_path_from_flood_data
   (StorePath), add_to_beeline_path (TestStraightPathTraversal), find_free_path
   (FindSetIndexOfBitField), store_path (UpdateSeg57TurningValues) and close_to_square
   (CheckIfAtOrNearTargetTile). */
#include <stdlib.h>
#include <dos.h>

struct Object {
    unsigned id, pos;
    struct { unsigned char quality:6, nextlo:2; unsigned char nexthi; } qn;
    struct { unsigned char owner:6, linklo:2; unsigned char linkhi; } ol;
    unsigned char hp, heading, b0A;
    unsigned goal_word, b0D, b0F;
    unsigned char b11, last_hit, b13, b14, b15;
    unsigned home;
    unsigned char b18, b19, whoami;
};
struct Tile { unsigned type:4, height:4; unsigned b8:2, floor:4, b14:2; unsigned objects; };
struct Link { unsigned b0:6, next:10; };
struct PhysNode { int w[0x14]; };
struct PhysTp { unsigned flags, w2; int w4; unsigned w6;
    unsigned char (far *handler)(struct PhysNode *); };
struct PathRec { unsigned char x,y; union { unsigned char raw; struct { unsigned char count:7, slope:1; } bits; } flag; unsigned char index,directions[16],slopes[8]; };
struct PathOffset { signed char x,y; };
struct PathSq { unsigned char x,y,unused,flag; };
struct StaticTile { unsigned char pathx, pathy, height;
    unsigned char pathflag:1, dist:7; unsigned char step; };
struct MotionCalc { int x,y,z; char pad6[2]; unsigned char radius,height;
    int index; unsigned hits0,hits1; char pad10[8]; };
struct ComObj { unsigned height:8; unsigned radius:3; unsigned other:5;
    char pad2; unsigned b3_0:1, b3_1:1, b3_2:1, b3_3:1, b3_4:4;
    char pad4[2]; unsigned b6_0:2, b6_2:2, b6_4:4; char pad7[4]; };
struct Creature { char pad0[0x0A]; struct { unsigned char other:5, bA_5:1, swims:1, flier:1; } b0A;
    unsigned char speed, run; char pad0D[0x14-0x0D];
    unsigned char door_damage; char pad15[0x2E-0x15]; unsigned char locks; };
/* Uninitialised data, DS:222C..227F: this file's _BSS. Turbo C lays it out by a hash of
   each name (tools/bssorder.py), so the definitions below are in that order. Names are
   the FM Towns ones; FM Towns keeps the six marked static as statics (at _tp_act+N), so
   their names are chosen here to land at the right address. Many of these are used only
   by seg007 (critter AI), which declares them extern. */
int crit_terr;                          /* 222C */
unsigned char doorx, doory;             /* 222E */
int tdx, tdy;                           /* 2230 */
unsigned char control;                  /* 2234 */
static int proj_ycoord;                 /* 2236, homing dart's fine y; name chosen for layout */
struct Object far *collobject;          /* 2238 */
struct PhysTp near *tp_act;             /* 223C */
unsigned char txpos;                    /* 223E */
static int hdist;                       /* 2240, nearest homing target's distance; for layout */
unsigned char aligned;                  /* 2242 */
static int htarget;                     /* 2244, nearest homing target; for layout */
unsigned char dontchangedz;             /* 2246 */
unsigned txpost;                        /* 2248 */
unsigned char myid;                     /* 224A */
static unsigned char jump;              /* 224B, the last step needs a jump; for layout */
unsigned tdistsqr;                      /* 224C */
unsigned char failed;                   /* 224E */
unsigned char typos;                    /* 224F */
unsigned long tdisttsqr;                /* 2250 */
struct Creature near *mycst;            /* 2254 */
struct Object far *meptr;               /* 2256 */
unsigned char pathlen;                  /* 225A */
unsigned char myxpos, myypos, myzpos;   /* 225B */
unsigned char hitwall;                  /* 225E */
unsigned typost;                        /* 2260 */
struct Object far *mytarget;            /* 2262 */
unsigned char myxhome, myyhome;         /* 2266 */
unsigned char myheight;                 /* 2268 */
unsigned char didhitobj;                /* 2269 */
int myxpost, myypost;                   /* 226A */
unsigned char didmove;                  /* 226E */
unsigned char hitadoor;                 /* 226F */
struct PhysNode near *pn_act;           /* 2270 */
static unsigned char cur_danger;        /* 2272, danger a path may cross; for layout */
unsigned char myoldspeed;               /* 2273 */
signed char tzpos;                      /* 2274 */
int XP;                                 /* 2276 */
unsigned char myoldfacing;              /* 2278 */
int YP;                                 /* 227A */
unsigned char myoldheading;             /* 227C */
static int projxpos;                    /* 227E, homing dart's fine x; name chosen for layout */
extern unsigned char seq_len;
extern int MapObj_X, MapObj_Y;
extern struct PhysNode CN2, CN4;
extern struct PhysNode CN1, CN3;
extern struct PhysTp CT1, CT2, CT3, CT4;
extern struct MotionCalc near *curP;
extern struct ComObj ComObjData[];
extern int PlayerLevel;
extern unsigned char tile_walls[];
extern unsigned char seq_lframe;
extern struct PathSq far pathsq[];
extern struct StaticTile far stdat[64][64];
/* The two flood fill frontiers, far segments 608E and 6096 (64 squares each); static
   in FM Towns, provisional names. */
struct PathPt { unsigned char x, y; };
extern struct PathPt far flood_list0[64], far flood_list1[64];
extern void far mem_set(void far *p, int value, int count);
void far make_path_from_flood_data(unsigned char length, unsigned char x,
    unsigned char y);
extern struct Tile far * far Map_GetAddr(int x, int y);
extern struct Object far * far CreateObj(int id, int owner);
extern void far set_htx(int heading);
extern void far do_physics(struct PhysNode *pn, struct PhysTp *tp);
extern int far Obj_MemTPtr(struct Object far *obj);
extern void far TerrainCheck(int flags);
extern struct Object far * far Obj_Punt(unsigned far *head, struct Object far *obj, int mode);
extern void far get_phys_data(struct Object far *obj, struct PhysNode *pn);
extern unsigned char far set_phys_data(struct Object far *obj, struct PhysNode *pn);
extern struct Object far * far CollObject(void);
extern void far UseObj(struct Object far *user, struct Object far *target, int mode);
extern int far checkLock(struct Object far *user, struct Object far *door, int skill);
extern void far damage_item(struct Object far *target, struct Object far *source,
    int x, int y, int damage, int type);
extern void far Obj_Add(unsigned far *head, struct Object far *obj);
extern struct Object far * far obj_deal(struct Object far *obj, int x, int y, int mode);
extern int far near_mob_put_at(struct Object far *source, struct Object far *obj,
    int mode, int flags);
extern void far put_effect(struct Object far *obj, int type, int size,
    int a, int b, int x, int y);
extern void far set_cur_seq_len(void);
extern struct Object far * far IsaDoor(unsigned char *x, unsigned char *y);
extern struct Object far * far Obj_IntTMem(int index);
extern void far process_area(char count, unsigned char src,
    unsigned char (far *callback)(int, int, struct Object far *),
    unsigned char type, char x, char y, char w, char h);
/* FM Towns get_theta_; OVR167.C defines it under the IDA name, which is what links. */
extern int far DartSatelliteVectoring_ovr167_313(int x1, int y1, int x2, int y2);
/* Initialised data, DS:00AC..00C2: the step for each of the four path directions, the
   free path slots (a bit per entry of paths), the direction from one square to the next
   by [dx + 1][dy + 1], and the slope tile type climbed in each direction. FM Towns keeps
   the three tables as statics (__D16Infoseg+N), so their names are provisional. */
static struct PathOffset PathingOffset[4] = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };
unsigned freepaths = 0xFFFF;
static unsigned char path_turns[3][3] = {
    { 0xFF, 3, 0xFF }, { 2, 0xFF, 0 }, { 0xFF, 1, 0xFF } };
static unsigned char slope_for_dir[4] = { 6, 8, 7, 9 };
extern unsigned TxmTerr[];
extern struct Object far * far Obj_PtrTMem(struct Link far *link);
extern struct PathRec far paths[];
extern void far critter_set_goal(unsigned char goal, int target);
extern void far crit_drunkwalk(void);
extern unsigned char far acceptable_danger(void);
void far try_to_open_door(struct Object far *door);
unsigned char far crit_hndlr_walk(struct PhysNode *pn);
unsigned char far crit_hndlr_fly(struct PhysNode *pn);
unsigned char far crit_hndlr_swim(struct PhysNode *pn);
static unsigned char far crit_hndlr_obj(struct PhysNode *pn);
void far check_homing(void);
void far check_sat(void);
unsigned char far do_crit_phys(struct PhysNode *pn, struct PhysTp *tp);
void far do_that_jump_kinda_thing(struct PathRec far *path);
unsigned char far deltatotheta(char x, char y);
unsigned char far add_to_beeline_path(unsigned char x,
    unsigned char y);
void far adjust_height(unsigned char x, unsigned char y);

void far build_corpse(struct Object far *obj, char fluids, char corpse) {
    struct Object far *remains;
    struct Tile far *home = Map_GetAddr((obj->home & 0xFC00) >> 10,
                                        (obj->home & 0x3F0) >> 4);
    if (fluids == 0) goto corpse_part;
    if ((remains = CreateObj(0xD9 + (unsigned char)fluids, 0)) == 0)
        goto corpse_part;
    remains->pos = (remains->pos & 0x1FFF)
        | (((obj->pos & 0xE000) >> 13) & 7) << 13;
    remains->pos = (remains->pos & 0xE3FF)
        | (((obj->pos & 0x1C00) >> 10) & 7) << 10;
    remains->pos = (remains->pos & 0xFF80) | (obj->pos & 0x7F);
    remains->qn.quality = 0x28;
    Obj_Add(&home->objects, remains);
    obj_deal(remains, XP, YP, 1);
corpse_part:
    if (corpse) {
        if (rand() % 16 >= 7 && (PlayerLevel - 1) / 8 != 7) return;
        if ((remains = CreateObj(0xC0 + (unsigned char)corpse, 0)) != 0) {
            remains->ol.owner = (obj->id & 0x3F) >> 0;
            near_mob_put_at(obj, remains, 4, 0);
        }
    }
}

unsigned char far move_me_joe(void) {
    unsigned char result;
    unsigned char unused;
    if (meptr->hp == 0
        && ComObjData[meptr->id & 0x1FF].b6_2 < 3) {
        if (Obj_Punt(&Map_GetAddr((meptr->home & 0xFC00) >> 10,
                                  (meptr->home & 0x3F0) >> 4)->objects,
                     meptr, 0))
            meptr->hp = 1;
        else return 0;
    }
    if (ComObjData[meptr->id & 0x1FF].b3_3)
        CT3.flags = 0x1000;
    else CT3.flags = 0;
    get_phys_data(meptr, &CN3);
    do_crit_phys(&CN3, &CT3);
    XP = (meptr->home & 0xFC00) >> 10;
    YP = (meptr->home & 0x3F0) >> 4;
    result = set_phys_data(meptr, &CN3);
    if (result) {
        meptr->b0A = meptr->b0A & 0xF0
            | (((meptr->b0A & 0xF) + (meptr->b14 & 7)) & 0xF) << 0;
        if ((meptr->id & 0x1FF) == 0x1B) check_homing();
        else if ((meptr->id & 0x1FF) == 0x1E) check_sat();
    }
    (void)unused;
    return result;
}
unsigned char far store_targ(int unused1, int unused2,
                                                      struct Object far *target) {
    int x = (((target->home & 0xFC00) >> 10) << 3)
          + ((target->pos & 0xE000) >> 13);
    int y;
    register int distance;
    y = (((target->home & 0x3F0) >> 4) << 3)
      + ((target->pos & 0x1C00) >> 10);
    distance = abs(x - projxpos) + abs(y - proj_ycoord);
    if (distance < hdist) {
        htarget = Obj_MemTPtr(target);
        hdist = distance;
    }
    return 0;
}
int far merge_angles(int src, int dst, int magnitude) {
    volatile int tmp;
    int result;
    if (dst - src > 128) {
        src += 256;
        magnitude = 64 - magnitude;
        tmp = src;
        src = dst;
        dst = tmp;
    } else if (dst - src < -128) {
        dst += 256;
    }
    result = 0xFF & (magnitude * (dst - src) / 64 + src);
    return result;
}
void far check_homing(void) {
    int xhome, yhome, width, height;
    register int step, cardinal;
    width = height = 3;
    xhome = ((meptr->home & 0xFC00) >> 10) - 1;
    yhome = ((meptr->home & 0x3F0) >> 4) - 1;
    cardinal = (((unsigned char)meptr->heading + 0x20) & 0xFF) >> 6;
    cardinal = ((((meptr->pos & 0x380) >> 7) + 1) & 7) >> 1;
    if (cardinal > 1) {
        if (cardinal == 3) xhome--;
        else yhome--;
    }
    if (cardinal & 1) width++;
    else height++;
    htarget = -1;
    hdist = 0x7FFF;
    projxpos = (((meptr->home & 0xFC00) >> 10) << 3)
        + ((meptr->pos & 0xE000) >> 13);
    proj_ycoord = (((meptr->home & 0x3F0) >> 4) << 3)
        + ((meptr->pos & 0x1C00) >> 10);
    process_area(1, meptr->last_hit,
        store_targ, 0,
        xhome, yhome, width, height);
    if (htarget > 0) {
        struct Object far *target;
        unsigned vector;
        int heading, pitch;
        unsigned heightdiff, desiredpitch, newpitch;
        target = Obj_IntTMem(htarget);
        xhome = (((target->home & 0xFC00) >> 10) << 3)
            + ((target->pos & 0xE000) >> 13);
        yhome = (((target->home & 0x3F0) >> 4) << 3)
            + ((target->pos & 0x1C00) >> 10);
        vector = DartSatelliteVectoring_ovr167_313(
            projxpos, proj_ycoord, xhome, yhome);
        if (hdist < 12) step = 2;
        else if (hdist < 32) step = 4;
        else step = 6;
        heading = merge_angles(meptr->heading, vector >> 8, (step + 8) << 2);
        meptr->heading = heading;
        pitch = ((meptr->b14 & 0xF8) >> 3);
        heightdiff = (target->pos & 0x7F) - (meptr->pos & 0x7F);
        desiredpitch = heightdiff / 24 + 16;
        newpitch = (pitch * (step + 4) + desiredpitch * (8 - step)) / 12;
        meptr->b14 = (meptr->b14 & 7) | ((newpitch & 0x1F) << 3);
    } else {
        int pitch;
        meptr->heading = (meptr->heading + (rand() & 7) - 3) & 0xFF;
        pitch = ((meptr->b14 & 0xF8) >> 3);
        if (!(rand() & 4)) {
            if (pitch < 14) pitch++;
            else if (pitch > 18) pitch--;
            else pitch = pitch + (rand() & 3) - 1;
        }
        meptr->b14 = (meptr->b14 & 7) | ((pitch & 0x1F) << 3);
    }
    if (!(rand() & 2))
        put_effect(meptr, 14, 3, 0, -(meptr->pos & 0x7F),
                   (meptr->home & 0xFC00) >> 10,
                   (meptr->home & 0x3F0) >> 4);
    if (((meptr->b0A & 0x80) >> 7) != 0) meptr->hp = 0;
    else if (!(rand() & 3) && meptr->hp) meptr->hp = meptr->hp - 1;
}
void far check_sat(void) {
    int satx, saty, srcx, srcy;
    int dist, turn, pitch, zdiff, zpitch, newpitch;
    struct Object far *src;
    int heading;
    unsigned theta;
    int divisor = 2;
    register unsigned vector;
    register int diff;
    src = Obj_IntTMem(meptr->last_hit);
    srcx = (((src->home & 0xFC00) >> 10) << 3) + ((src->pos & 0xE000) >> 13);
    srcy = (((src->home & 0x3F0) >> 4) << 3) + ((src->pos & 0x1C00) >> 10);
    satx = (((meptr->home & 0xFC00) >> 10) << 3) + ((meptr->pos & 0xE000) >> 13);
    saty = (((meptr->home & 0x3F0) >> 4) << 3) + ((meptr->pos & 0x1C00) >> 10);
    vector = DartSatelliteVectoring_ovr167_313(satx, saty, srcx, srcy);
    theta = vector + 0x4000;
    diff = abs((theta >> 8) - meptr->heading);
    if (diff > 0x80) diff = 0xFF - diff;
    if (diff > 0x40) theta = theta + 0x8000;
    dist = abs(satx - srcx) + abs(saty - srcy);
    if (dist < 4) vector += 0x8000;
    else divisor = 8;
    turn = (abs(dist - 4) << 3) / divisor;
    if (turn > 0x40) turn = 0x40;
    heading = merge_angles(meptr->heading, theta >> 8, 0x20);
    heading = merge_angles(heading, vector >> 8, turn);
    meptr->heading = heading;
    pitch = ((meptr->b14 & 0xF8) >> 3);
    zdiff = (src->pos & 0x7F) - (meptr->pos & 0x7F) + 0xF;
    zpitch = zdiff / 8 + 0x10;
    newpitch = (pitch + zpitch) / 2;
    if (!(rand() & 3)) {
        if (newpitch < 0xF) newpitch++;
        else if (newpitch > 0x11) newpitch--;
        else newpitch = newpitch + (rand() & 3) - 1;
    }
    meptr->b14 = (meptr->b14 & 7) | ((newpitch & 0x1F) << 3);
    if ((meptr->b13 & 0x7F) < 0xF)
        meptr->b13 = meptr->b13 & 0x80 | (0xF & 0x7F) << 0;
    if (((meptr->b0A & 0x80) >> 7) != 0) meptr->hp = 0;
    else if ((rand() & 0x1F) == 1 && meptr->hp > 0) meptr->hp = meptr->hp - 1;
}
void far init_ai(void) {
    CN1.w[6] = 0; CN1.w[7] = 0; ((unsigned char *)&CN1)[0x17] = 0x80;
    CN2.w[6] = 0; CN2.w[7] = 0; ((unsigned char *)&CN2)[0x17] = 0x80;
    CN3.w[6] = 0; CN3.w[7] = 0; ((unsigned char *)&CN3)[0x17] = 0;
    CN4.w[6] = 0; CN4.w[7] = 0; ((unsigned char *)&CN4)[0x17] = 0x80;
    CT1.w2 = 0x1F30; CT1.w4 = 0x1010; CT1.w6 = 0x20;
    CT1.flags = 0; CT1.handler = (unsigned char (far *)(struct PhysNode *))crit_hndlr_walk;
    CT2.w2 = 0x700; CT2.w4 = 0x80; CT2.w6 = 0;
    CT2.flags = 0x1000; CT2.handler = crit_hndlr_fly;
    CT3.w2 = 0; CT3.w4 = 0; CT3.w6 = 0;
    CT3.flags = 0; CT3.handler = crit_hndlr_obj;
    CT4.w2 = 0x1728; CT4.w4 = 0x10A8; CT4.w6 = 0;
    CT4.flags = 0x10; CT4.handler = crit_hndlr_swim;
}
int far get_terrain(struct Object far *obj) {
    struct MotionCalc calc;
    curP = &calc;
    curP->index = Obj_MemTPtr(obj);
    curP->radius = ComObjData[obj->id & 0x1FF].radius;
    curP->height = ComObjData[obj->id & 0x1FF].height;
    curP->x = (((obj->home & 0xFC00) >> 10) << 3) + ((obj->pos & 0xE000) >> 13);
    curP->y = (((obj->home & 0x3F0) >> 4) << 3) + ((obj->pos & 0x1C00) >> 10);
    curP->z = obj->pos & 0x7F;
    TerrainCheck(8);
    return curP->hits0 | curP->hits1;
}
unsigned char far crit_hndlr_walk(struct PhysNode *pn) {
    struct Object far *door;
    register struct PhysNode *motion = pn;
    if (motion->w[0] & 0x1000) {
        if (CN1.w[8] == 0) CN1.w[8] = -4;
        meptr->b14 = meptr->b14 & 0xF8 | 1;
        failed = 1;
        control = 0;
        return 0;
    }
    if (motion->w[0] & 0x10) {
        if ((motion->w[0] & 0xF8) == 0x10) {
            failed = 1;
            control = 0;
            put_effect(meptr, 6, 3, 0, 0, CN1.w[0] >> 8, CN1.w[1] >> 8);
            meptr->b15 = meptr->b15 & 0xC0 | 7;
            set_cur_seq_len();
            meptr->goal_word = meptr->goal_word & 0xFFF | ((seq_lframe & 0xF) << 12);
            meptr->b14 = meptr->b14 & 0xF8 | 1;
            return 1;
        }
        if (((meptr->b15 & 0x80) >> 7) == 0) {
            failed = 1;
            CN1.w[3] = CN1.w[4] = 0;
            return 1;
        }
    }
    if ((motion->w[0] & 0x800) && !(crit_terr & 0x800)) {
        if ((meptr->b15 & 0x80) >> 7) return 0;
        failed = 1;
        CN1.w[3] = CN1.w[4] = 0;
        return 1;
    }
    if ((motion->w[0] & 0x20) && !(crit_terr & 0x20)) {
        if ((meptr->b15 & 0x80) >> 7) return 0;
        failed = 1;
        CN1.w[3] = CN1.w[4] = 0;
        return 1;
    }
    if (motion->w[0] & 0x300) {
        failed = 1;
        return 0;
    }
    if (motion->w[0] & 0x400) {
        if ((door = IsaDoor(&doorx, &doory)) != 0) {
            collobject = door;
            hitadoor = 1;
            failed = 1;
            didhitobj = 1;
        } else {
            failed = 1;
            didhitobj = 1;
            collobject = CollObject();
        }
    }
    return failed && control;
}
static unsigned char far crit_hndlr_obj(struct PhysNode *pn) {
    (void)pn; return 0;
}
unsigned char far crit_hndlr_fly(struct PhysNode *pn) {
    control = 1;
    if (pn->w[0] & 0x200) {
        failed = 1;
        return 0;
    } else {
        if (pn->w[0] & 0x100) {
            CN2.w[5] = 0x80;
            hitwall = 1;
        }
        if (pn->w[0] & 0x400) {
            failed = 1;
            didhitobj = 1;
            collobject = CollObject();
        }
        return failed && control;
    }
}
unsigned char far crit_hndlr_swim(struct PhysNode *pn) {
    if (pn->w[0] & 0x300) {
        CN4.w[3] = CN4.w[4] = 0;
        failed = 1;
        return 0;
    }
    if (pn->w[0] & 0x400) {
        CN4.w[3] = CN4.w[4] = 0;
        failed = 1;
        didhitobj = 1;
        collobject = CollObject();
    }
    if (pn->w[0] & 8) {
        CN4.w[3] = CN4.w[4] = 0;
        failed = 1;
    }
    return failed && control;
}
unsigned char far do_crit_phys(struct PhysNode *pn, struct PhysTp *tp) {
    pn->w[9] = (meptr->b14 & 7) << 4;
    do_physics(pn, tp);
    return 1;
}
unsigned char far hyp_move(unsigned char x1, unsigned char y1,
    unsigned char x2, unsigned char y2, unsigned char x3, unsigned char y3,
    int flags, int costflags, unsigned char height, unsigned char far *out,
    unsigned char far *dist) {
    struct Tile far *tile2, far *tile1, far *tile3;
    unsigned char type2, type3;
    int terr2, terr3;
    unsigned char h12, h23, h23b, objh, blocked, onobj;
    struct Link far *link;
    struct Object far *obj;
    unsigned char tested, dhead, dxp, dyp;
    register int dir;
    register struct ComObj *com;
    blocked = 0;
    onobj = 0;
    tested = 0;
    jump = 0;
    tile2 = Map_GetAddr(x2, y2);
    tile1 = Map_GetAddr(x1, y1);
    tile3 = Map_GetAddr(x3, y3);
    type2 = tile2->type;
    type3 = tile3->type;
    terr2 = (TxmTerr[tile2->floor] & 0xC0) >> 6;
    terr3 = (TxmTerr[tile3->floor] & 0xC0) >> 6;
    if (x1 == 0) {
        *out = height;
        h23 = tile3->height;
        if (x3 > x2 && (tile_walls[type3] & 2)) return 0;
        if (x3 < x2 && (tile_walls[type3] & 4)) return 0;
        if (y3 > y2 && (tile_walls[type3] & 8)) return 0;
        if (y3 < y2 && (tile_walls[type3] & 0x10)) return 0;
        if (x3 > x2 && (tile_walls[type2] & 4)) return 0;
        if (x3 < x2 && (tile_walls[type2] & 2)) return 0;
        if (y3 > y2 && (tile_walls[type2] & 0x10)) return 0;
        if (y3 < y2 && (tile_walls[type2] & 8)) return 0;
        if (!(0x1000 & flags)) return 1;
        if (type3 >= 6 && type3 <= 9
            && slope_for_dir[path_turns[x3 - x2 + 1][y3 - y2 + 1]] != type3)
            h23++;
        if (h23 > height + 1) return 0;
        return 1;
    } else if (x3 == 0) {
        *out = height;
        if (!(0x1000 & flags)) return 1;
        objh = 0;
        for (link = (struct Link far *)&tile2->objects; link->next && objh == 0;
             link = (struct Link far *)&obj->qn) {
            obj = Obj_PtrTMem(link);
            com = &ComObjData[obj->id & 0x1FF];
            if (com->b3_1)
                objh = ((obj->pos & 0x7F) + com->height) >> 3;
        }
        h23 = tile2->height;
        if (objh > h23) {
            h23 = objh;
            onobj = 1;
        }
        if (tile1->height > h23 + 1) {
            *out = h23;
            *dist += height - *out - 1;
            if (*dist > cur_danger) return 0;
        }
        if (!onobj) {
            if (flags & (8 << terr2)) return 0;
            if (costflags & (8 << terr2)) {
                *dist = *dist + 2;
                if (*dist > cur_danger) return 0;
            }
        }
        return 1;
    }
    *out = height;
    if (x3 > x2) {
        if ((tile_walls[type3] & 2) || (tile_walls[type2] & 4)) return 0;
    } else if (x3 < x2) {
        if ((tile_walls[type3] & 4) || (tile_walls[type2] & 2)) return 0;
    } else if (y3 > y2) {
        if ((tile_walls[type3] & 8) || (tile_walls[type2] & 0x10)) return 0;
    } else if (y3 < y2) {
        if ((tile_walls[type3] & 0x10) || (tile_walls[type2] & 8)) return 0;
    }
    objh = 0;
    for (link = (struct Link far *)&tile2->objects; link->next && objh == 0;
         link = (struct Link far *)&obj->qn) {
        obj = Obj_PtrTMem(link);
        com = &ComObjData[obj->id & 0x1FF];
        if (((obj->id & 0x1C0) >> 6) == 5 && ((obj->id & 0x30) >> 4) == 0
            && (obj->id & 0xF) < 8) {
            if (checkLock(meptr, obj, 0) == 0) {
                dhead = ((obj->pos & 0x380) >> 7) & 3;
                dxp = (obj->pos & 0xE000) >> 13;
                dyp = (obj->pos & 0x1C00) >> 10;
                if (!tested) {
                    if (x1 < x2) {
                        if (y2 < y3) dir = 0;
                        else if (y2 > y3) dir = 2;
                        else dir = 1;
                    } else if (x1 > x2) {
                        if (y2 < y3) dir = 3;
                        else if (y2 > y3) dir = 5;
                        else dir = 4;
                    } else if (y1 < y2) {
                        if (x2 < x3) dir = 8;
                        else if (x2 > x3) dir = 6;
                        else dir = 7;
                    } else {
                        if (x2 < x3) dir = 11;
                        else if (x2 > x3) dir = 9;
                        else dir = 10;
                    }
                    tested = 1;
                }
                switch (dir) {
                case 7: case 10:
                    if (dhead != 2) return 0;
                    break;
                case 1: case 4:
                    if (dhead != 0) return 0;
                    break;
                case 0: case 9:
                    switch (dhead) {
                    case 0: if (dyp > 3) return 0; break;
                    case 1: return 0;
                    case 2: if (dxp < 4) return 0; break;
                    }
                    break;
                case 2: case 6:
                    switch (dhead) {
                    case 0: if (dyp < 4) return 0; break;
                    case 2: if (dxp < 4) return 0; break;
                    case 3: return 0;
                    }
                    break;
                case 3: case 11:
                    switch (dhead) {
                    case 0: if (dyp > 3) return 0; break;
                    case 2: if (dxp > 3) return 0; break;
                    case 3: return 0;
                    }
                    break;
                case 5: case 8:
                    switch (dhead) {
                    case 0: if (dyp < 4) return 0; break;
                    case 1: return 0;
                    case 2: if (dxp > 3) return 0; break;
                    }
                    break;
                }
            }
        } else if (com->b3_1)
            objh = ((obj->pos & 0x7F) + com->height) >> 3;
    }
    if (!(0x1000 & flags)) {
        *out = 0x10 - ((myheight + 3) >> 2);
        return 1;
    }
    h12 = tile2->height > tile1->height ? tile2->height : tile1->height;
    h23b = h23 = tile2->height > tile3->height ? tile2->height : tile3->height;
    if (height > h12) h12 = height;
    if (type3 >= 6 && type3 <= 9
        && slope_for_dir[path_turns[x3 - x2 + 1][y3 - y2 + 1]] != type3)
        h23++;
    if (((h12 > h23 ? h12 : h23) << 3) + myheight > 0x7F) return 0;
    if (h12 > h23 + 1) {
        if (h12 > objh + 1) {
            blocked = 1;
            *out = h23 > objh ? h23 : objh;
            *dist += h12 - *out - 1;
            if (*dist > cur_danger) return 0;
        } else {
            h23b = h23 = objh;
            onobj = 1;
        }
    } else if (objh != 0 && h12 >= objh && h12 <= objh + 1)
        onobj = 1;
    if (h23 > h12 + 1) return 0;
    if (h12 > tile2->height + 1 && !blocked && !onobj) {
        if (flags & (8 << terr2)) return 0;
        if (costflags & (8 << terr3)) {
            *dist = *dist + 2;
            if (*dist >= cur_danger) return 0;
        }
        if (h12 > objh + 1) {
            if ((*out = h12) >= h23) {
                if (mycst->b0A.bA_5) {
                    *dist = *dist + 1;
                    if (*dist >= cur_danger) return 0;
                    jump = 1;
                    return 1;
                } else return 0;
            } else return 0;
        } else {
            *out = h23b;
            return 1;
        }
    } else if ((flags & (8 << terr2)) && !onobj) {
        if (flags & (8 << terr3)) return 0;
        if (costflags & (8 << terr3)) {
            *dist = *dist + 2;
            if (*dist > cur_danger) return 0;
        }
        if (mycst->b0A.bA_5) {
            *dist = *dist + 1;
            if (*dist >= cur_danger) return 0;
            jump = 1;
            return 1;
        } else return 0;
    }
    *out = h23b;
    return 1;
}
unsigned char far hyp_see(unsigned char oldx, unsigned char oldy,
    unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2,
    unsigned char height) {
    struct Tile far *tile1 = Map_GetAddr(x1, y1);
    struct Tile far *tile2 = Map_GetAddr(x2, y2);
    unsigned char type1 = tile1->type;
    unsigned char type2 = tile2->type;
    if (oldx == 0 || (oldx == x1 && oldy == y1)) {
        if (x2 > x1 && (tile_walls[type2] & 2)) return 0;
        if (x2 < x1 && (tile_walls[type2] & 4)) return 0;
        if (y2 > y1 && (tile_walls[type2] & 8)) return 0;
        if (y2 < y1 && (tile_walls[type2] & 0x10)) return 0;
        if (x2 > x1 && (tile_walls[type1] & 4)) return 0;
        if (x2 < x1 && (tile_walls[type1] & 2)) return 0;
        if (y2 > y1 && (tile_walls[type1] & 0x10)) return 0;
        if (y2 < y1 && (tile_walls[type1] & 8)) return 0;
        if ((int)height >> 3 < tile2->height) return 0;
        else return 1;
    } else {
        if (x2 == 0) return 1;
        if (x2 > x1 && (tile_walls[type2] & 2)) return 0;
        if (x2 < x1 && (tile_walls[type2] & 4)) return 0;
        if (y2 > y1 && (tile_walls[type2] & 8)) return 0;
        if (y2 < y1 && (tile_walls[type2] & 0x10)) return 0;
        if (x2 > x1 && (tile_walls[type1] & 4)) return 0;
        if (x2 < x1 && (tile_walls[type1] & 2)) return 0;
        if (y2 > y1 && (tile_walls[type1] & 0x10)) return 0;
        if (y2 < y1 && (tile_walls[type1] & 8)) return 0;
        if ((int)height >> 3 < tile2->height) return 0;
    }
    return 1;
}
unsigned char far flood_path(char x, char y, unsigned char height0,
    char destx, char desty, char destz, unsigned char range) {
    char tx, ty, nx, ny;
    unsigned char i;
    char minx, maxx, miny, maxy;
    struct PathPt far *cur;
    struct PathPt far *next;
    unsigned char ncur, nnext;
    struct StaticTile far *np;
    struct StaticTile far *rec;
    unsigned char step, dist, height, ok, isnew, reach;
    register int d;
    step = 0;
    cur_danger = range;
    ncur = 0;
    nnext = 0;
    cur = flood_list0;
    next = flood_list1;
    mem_set(stdat, 0, 0x5000);
    if ((PlayerLevel - 1) / 8 == 0 && (PlayerLevel - 1) % 8 + 1 == 0) reach = 10;
    else reach = 5;
    minx = x < destx ? (x - reach > 1 ? x - reach : 1)
                     : (destx - reach > 1 ? destx - reach : 1);
    miny = y < desty ? (y - reach > 1 ? y - reach : 1)
                     : (desty - reach > 1 ? desty - reach : 1);
    maxx = x > destx ? (x + reach < 0x40 ? x + reach : 0x40)
                     : (destx + reach < 0x40 ? destx + reach : 0x40);
    maxy = y > desty ? (y + reach < 0x40 ? y + reach : 0x40)
                     : (desty + reach < 0x40 ? desty + reach : 0x40);
    stdat[x][y].pathx = x;
    stdat[x][y].height = height0;
    stdat[x][y].step = 0;
    for (d = 0; d < 4; d++) {
        nx = x + PathingOffset[d].x;
        ny = y + PathingOffset[d].y;
        np = &stdat[nx][ny];
        dist = 0;
        if (hyp_move(0, 0, x, y, nx, ny,
                tp_act->w4, tp_act->w6, height0, &np->height, &dist)) {
            if (nx == destx && ny == desty) {
                pathlen = 1;
                pathsq[0].x = x;
                pathsq[0].y = y;
                pathsq[0].flag = 0;
                pathsq[1].x = nx;
                pathsq[1].y = ny;
                return 1;
            }
            np->pathx = x;
            np->pathy = y;
            np->step = 1;
            np->dist = dist;
            cur[ncur].x = nx;
            cur[ncur].y = ny;
            ncur = ncur + 1;
        }
    }
    for (step = 1; step < 0x28 && ncur > 0; step = step + 1) {
        nnext = 0;
        for (i = 0; i < ncur && nnext < 0x40; i = i + 1) {
            tx = cur[i].x;
            ty = cur[i].y;
            rec = &stdat[tx][ty];
            for (d = 0; d < 4; d++) {
                nx = tx + PathingOffset[d].x;
                ny = ty + PathingOffset[d].y;
                if (nx < minx || nx > maxx || ny < miny || ny > maxy) continue;
                np = &stdat[nx][ny];
                dist = rec->dist;
                if (rec->pathx == nx && rec->pathy == ny) continue;
                ok = hyp_move(rec->pathx, rec->pathy,
                    tx, ty, nx, ny, tp_act->w4, tp_act->w6, rec->height,
                    &height, &dist);
                isnew = np->pathx == 0;
                if (ok == 0) continue;
                if (isnew || (rec->step < np->step
                    && abs(height - destz) < abs(np->height - destz))) {
                    np->pathx = tx;
                    np->pathy = ty;
                    np->height = height;
                    np->step = step + 1;
                    np->dist = dist;
                    rec->pathflag = jump;
                    if (isnew) {
                        next[nnext].x = nx;
                        next[nnext].y = ny;
                        nnext = nnext + 1;
                    }
                    if (nx == destx && ny == desty
                        && hyp_move(tx, ty, nx, ny, 0, 0,
                            tp_act->w4, tp_act->w6, np->height, &np->height, &dist)) {
                        make_path_from_flood_data(step, destx, desty);
                        return 1;
                    }
                }
            }
        }
        if (cur == flood_list0) {
            cur = flood_list1;
            next = flood_list0;
        } else {
            cur = flood_list0;
            next = flood_list1;
        }
        ncur = nnext;
    }
    return 0;
}
void far make_path_from_flood_data(unsigned char length, unsigned char x,
    unsigned char y) {
    struct StaticTile far *tile;
    unsigned char i;
    pathlen = length + 1;
    pathsq[length + 1].x = x;
    pathsq[length + 1].y = y;
    pathsq[0].flag = 0;
    for (i = length + 1; i > 0; i = i - 1) {
        tile = &stdat[pathsq[i].x][pathsq[i].y];
        pathsq[i - 1].x = tile->pathx;
        pathsq[i - 1].y = tile->pathy;
        pathsq[i].flag = tile->pathflag;
    }
}
int far beeline(int x1, int y1, int x2, int y2) {
    signed char dx, dy;
    unsigned char x, y;
    signed char step, side, slope;
    unsigned char acc;
    struct Tile far *tile;
    unsigned char result, unused;
    register unsigned char *major, *minor;
    acc = 0x40;
    dx = x2 - x1;
    dy = y2 - y1;
    x = x1;
    y = y1;
    tile = Map_GetAddr(x1, y1);
    cur_danger = 0;
    if (dx == 0 && dy == 0) return -1;
    if (dx >= dy) {
        if (dx >= -dy) {
            major = &x; minor = &y;
            slope = ((long)dy << 7) / dx;
            step = 1;
            side = dy > 0 ? 1 : -1;
        } else {
            major = &y; minor = &x;
            slope = ((long)dx << 7) / dy;
            step = -1;
            side = dx > 0 ? 1 : -1;
        }
    } else {
        if (dx >= -dy) {
            major = &y; minor = &x;
            slope = ((long)dx << 7) / dy;
            step = 1;
            side = dx > 0 ? 1 : -1;
        } else {
            major = &x; minor = &y;
            slope = ((long)dy << 7) / dx;
            step = -1;
            side = dy > 0 ? 1 : -1;
        }
    }
    pathsq[0].x = x1;
    pathsq[0].y = y1;
    pathlen = 1;
    pathsq[0].unused = tile->height;
    do {
        *major += step;
        if (!add_to_beeline_path(x, y)) return 0;
        acc += slope;
        if (acc & 0x80) {
            acc = acc & 0x7F;
            *minor += side;
            if (!add_to_beeline_path(x, y)) return 0;
        }
    } while (x != x2 || y != y2);
    return result = hyp_move(
        pathsq[pathlen - 2].x, pathsq[pathlen - 2].y,
        pathsq[pathlen - 1].x, pathsq[pathlen - 1].y, 0, 0,
        tp_act->w4, tp_act->w6, pathsq[pathlen - 2].unused,
        (unsigned char far *)&pathsq[pathlen - 2].unused, &unused);
}
unsigned char far line_of_sight(int x1, int y1, int z1, int x2, int y2, int z2) {
    int dz;
    unsigned char lastx, lasty, z, destx, desty, oldx, oldy, x, y;
    unsigned char *major, *minor;
    unsigned char diff, count, step, dir, slope, frac, result;
    register int dx, dy;
    dx = x2 - x1;
    dy = y2 - y1;
    dz = z2 - z1;
    x = oldx = lastx = x1 >> 3;
    y = oldy = lasty = y1 >> 3;
    destx = x2 >> 3;
    desty = y2 >> 3;
    z = z1 >> 3;
    if (dx == 0 && dy == 0) return 1;
    if (dx >= dy) {
        if (-dy <= dx) {
            major = &x;
            diff = dx >> 3;
            minor = &y;
            step = 1;
            dir = dy > 0 ? 1 : -1;
            if (dir == 1) {
                slope = ((long)dy << 7) / dx;
                frac = ((y1 & 7) << 4) + slope * (7 - (x1 & 7)) / 8;
            } else {
                slope = ((long)-dy << 7) / dx;
                frac = ((7 - (y1 & 7)) << 4) + slope * (7 - (x1 & 7)) / 8;
            }
        } else {
            major = &y;
            diff = -dy >> 3;
            minor = &x;
            step = -1;
            dir = dx > 0 ? 1 : -1;
            if (dir == 1) {
                slope = ((long)dx << 7) / -dy;
                frac = ((7 - (x1 & 7)) << 4) + slope * (y1 & 7) / 8;
            } else {
                slope = ((long)-dx << 7) / -dy;
                frac = ((x1 & 7) << 4) + slope * (y1 & 7) / 8;
            }
        }
    } else {
        if (-dy <= dx) {
            major = &y;
            diff = dy >> 3;
            minor = &x;
            step = 1;
            dir = dx > 0 ? 1 : -1;
            if (dir == 1) {
                slope = ((long)dx << 7) / dy;
                frac = ((x1 & 7) << 4) + slope * (7 - (y1 & 7)) / 8;
            } else {
                slope = ((long)-dx << 7) / dy;
                frac = ((7 - (x1 & 7)) << 4) + slope * (7 - (y1 & 7)) / 8;
            }
        } else {
            major = &x;
            diff = -dx >> 3;
            minor = &y;
            step = -1;
            dir = dy > 0 ? 1 : -1;
            if (dir == 1) {
                slope = ((long)dy << 7) / -dx;
                frac = ((7 - (y1 & 7)) << 4) + slope * (x1 & 7) / 8;
            } else {
                slope = ((long)-dy << 7) / -dx;
                frac = ((y1 & 7) << 4) + slope * (x1 & 7) / 8;
            }
        }
    }
    z = z1;
    count = 0;
    for (;;) {
        if (frac & 0x80) {
            frac = frac & 0x7F;
            *minor += dir;
            if (!(result = hyp_see(oldx, oldy, lastx, lasty, x, y, z))) return 0;
            if (x == destx && y == desty) {
                result = hyp_see(lastx, lasty, x, y, 0, 0, z);
                return result;
            }
            oldx = lastx;
            oldy = lasty;
            lastx = x;
            lasty = y;
        }
        *major += step;
        count++;
        if (count > 10) return 0;
        if (diff) z = z1 + count * dz / diff;
        if (!(result = hyp_see(oldx, oldy, lastx, lasty, x, y, z))) return 0;
        if (x == destx && y == desty) {
            result = hyp_see(lastx, lasty, x, y, 0, 0, z);
            return result;
        }
        oldx = lastx;
        oldy = lasty;
        lastx = x;
        lasty = y;
        frac += slope;
    }
}
unsigned char far add_to_beeline_path(unsigned char x,
    unsigned char y) {
    unsigned char traversable, unused;
    cur_danger = 0;
    pathsq[pathlen].x = x;
    pathsq[pathlen].y = y;
    pathlen++;
    if (pathlen > 0x3F) return 0;
    if (pathlen == 2) {
        traversable = hyp_move(0, 0,
            pathsq[0].x, pathsq[0].y, pathsq[1].x, pathsq[1].y,
            tp_act->w4, tp_act->w6, pathsq[0].unused,
            (unsigned char far *)&pathsq[1].unused, &unused);
        return traversable && !jump;
    } else {
        traversable = hyp_move(
            pathsq[pathlen - 3].x, pathsq[pathlen - 3].y,
            pathsq[pathlen - 2].x, pathsq[pathlen - 2].y,
            pathsq[pathlen - 1].x, pathsq[pathlen - 1].y,
            tp_act->w4, tp_act->w6, pathsq[pathlen - 3].unused,
            (unsigned char far *)&pathsq[pathlen - 2].unused, &unused);
        return traversable && !jump;
    }
}
unsigned char far find_free_path(unsigned char *found) {
    unsigned char i;
    if (freepaths == 0) return 0;
    for (i = 0; i < 16; i = i + 1) {
        if (freepaths & (1 << i)) {
            *found = i;
            return 1;
        }
    }
    return 0;
}
void far store_path(struct PathRec far *path) {
    unsigned char i, j, direction, slope;
    path->flag.bits.count = 0;
    path->x = pathsq[0].x;
    path->y = pathsq[0].y;
    path->index = pathlen;
    if (pathlen > 1) { }
    for (i = 0; i < pathlen; i = i + 4) {
        direction = 0;
        for (j = 0; j < 4; j++) {
            direction = direction + ((path_turns[pathsq[i+j+1].x - pathsq[i+j].x + 1]
                [pathsq[i+j+1].y - pathsq[i+j].y + 1] & 3) << (j * 2));
        }
        path->directions[i / 4] = direction;
    }
    for (i = 0; i < pathlen; i = i + 8) {
        slope = 0;
        for (j = 0; j < 8; j++)
            slope = slope + ((pathsq[i+j+1].flag & 1) << j);
        path->slopes[i / 8] = slope;
    }
}
unsigned char far set_next_square_on_path(struct PathRec far *path) {
    register int direction;
    if (path->flag.bits.count >= path->index) return 0;
    direction = (path->directions[path->flag.bits.count / 4]
                 >> ((path->flag.bits.count % 4) << 1)) & 3;
    path->x += PathingOffset[direction].x;
    path->y += PathingOffset[direction].y;
    if ((path->slopes[path->flag.bits.count / 8]
        >> (path->flag.bits.count % 8)) & 1)
        path->flag.bits.slope = 1;
    else path->flag.bits.slope = 0;
    path->flag.bits.count = path->flag.bits.count + 1;
    return 1;
}
int far close_to_square(unsigned char flag,
    int xhome, int yhome, int xpos, int ypos, int pathx, int pathy) {
    if (!flag) {
        if (xpos >= 6 && pathx > xhome) xhome++;
        else if (xpos <= 1 && pathx < xhome) xhome--;
        if (ypos >= 6 && pathy > yhome) yhome++;
        else if (ypos <= 1 && pathy < yhome) yhome--;
    }
    if (xhome == pathx && yhome == pathy) return 1;
    return 0;
}
unsigned char far move_along_path(struct PathRec far *path) {
    int x,y;
    unsigned char tilex,tiley,heading;
    tilex = path->x;
    tiley = path->y;
    if ((unsigned char)close_to_square(
        path->flag.bits.slope, myxpos, myypos,
        myxpost & 7, myypost & 7, tilex, tiley)) {
        if (!set_next_square_on_path(path)) return 0;
    } else {
        tilex = myxpos;
        tiley = myypos;
    }
    if (path->flag.bits.slope) {
        do_that_jump_kinda_thing(path);
    } else {
        if (mycst->b0A.flier)
            adjust_height((meptr->b0F & 0x3F) >> 0,
                          (meptr->b0F & 0xFC0) >> 6);
        x = path->x << 3;
        if (path->x == tilex) x += 4;
        else if (path->x < tilex) x += 7;
        y = path->y << 3;
        if (path->y == tiley) y += 4;
        else if (path->y < tiley) y += 7;
        heading = deltatotheta((char)x - (char)myxpost,
                               (char)y - (char)myypost);
        set_htx(heading);
    }
    return 1;
}
void far do_that_jump_kinda_thing(struct PathRec far *path) {
    int x,y;
    unsigned char heading;
    register int direction;
    x = (path->x << 3) - 2;
    if (path->x == myxpos) x += 6;
    else if (path->x < myxpos) x += 11;
    y = (path->y << 3) - 2;
    if (path->y == myypos) y += 6;
    else if (path->y < myypos) y += 11;
    if (abs(x - myxpost) + abs(y - myypost) < 3) {
        direction = (path->directions[path->flag.bits.count / 4]
                    >> ((path->flag.bits.count % 4) << 1)) & 3;
        x = path->x + PathingOffset[direction].x;
        y = path->y + PathingOffset[direction].y;
        heading = deltatotheta((char)x - (char)((meptr->home & 0xFC00) >> 10),
                               (char)y - (char)((meptr->home & 0x3F0) >> 4));
        didmove = 1;
        meptr->b14 = meptr->b14 & 0xF8 | 1;
        meptr->b14 = meptr->b14 & 7 | 0xB0;
        meptr->b13 = meptr->b13 & 0x80 | 11;
    } else {
        heading = deltatotheta((char)x - (char)myxpost,
                               (char)y - (char)myypost);
    }
    set_htx(heading);
}
unsigned char far deltatotheta(char x, char y) {
    char dx = x << 1;
    char dy = y << 1;
    if (y > dx) {
        if (x > -(int)dy) return y > -(int)dx ? 0 : 7;
        else return x > dy ? 5 : 6;
    } else {
        if (x > -(int)dy) return x > dy ? 2 : 1;
        else return y > -(int)dx ? 3 : 4;
    }
}
void far set_loc(unsigned char x, unsigned char y, unsigned char z) {
    if (((meptr->b0F & 0x3F) >> 0) != x
        || ((meptr->b0F & 0xFC0) >> 6) != y
        || ((meptr->b0D & 0xF0) >> 4) != z) {
        meptr->b0F = meptr->b0F & 0xFFC0 | (x & 0x3F) << 0;
        meptr->b0F = meptr->b0F & 0xF03F | (y & 0x3F) << 6;
        meptr->b0D = meptr->b0D & 0xFF0F | (z & 0xF) << 4;
        meptr->b18 = meptr->b18 & 0xDF | 0x20;
        meptr->b18 = meptr->b18 & 0xBF;
    }
}
void far crit_head_for_loc(unsigned char x, unsigned char y, char z) {
    char dx, dy;
    unsigned char heading, slot, blocked, opening;
    int speed;
    register int item;
    blocked = 0;
    opening = 0;
    set_loc(x, y, z);
    if (((meptr->b18 & 0x20) >> 5) && ((meptr->b15 & 0x80) >> 7)) {
        freepaths |= 1 << (meptr->home & 0xF);
        meptr->b15 = meptr->b15 & 0x7F;
    }
    dx = x - myxpos;
    dy = y - myypos;
    if (dx == 0 && dy == 0) {
        if ((meptr->b15 & 0x80) >> 7) {
            freepaths |= 1 << (meptr->home & 0xF);
            meptr->b15 = meptr->b15 & 0x7F;
        }
        if (((meptr->goal_word & 0xF) >> 0) == 1)
            critter_set_goal(8, 0);
        else if (control) {
            meptr->b13 = meptr->b13 & 0x80;
            meptr->b15 = meptr->b15 & 0xBF | 0x40;
            meptr->b15 = meptr->b15 & 0xC0;
            meptr->goal_word = meptr->goal_word & 0xFFF;
            return;
        }
    }
    if (!control) {
        meptr->b14 = meptr->b14 & 0xF8 | 1;
        if ((meptr->b15 & 0x80) >> 7
            && paths[meptr->home & 0xF].x == (meptr->home & 0xFC00) >> 10
            && paths[meptr->home & 0xF].y == (meptr->home & 0x3F0) >> 4)
            set_next_square_on_path(&paths[meptr->home & 0xF]);
        return;
    }
    if (failed && !aligned && !((meptr->b18 & 0x40) >> 6)) {
        if (didhitobj) {
            if (hitadoor) {
                meptr->b15 = meptr->b15 & 0xC0;
                meptr->goal_word = meptr->goal_word & 0xFFF;
                if (rand() % 4 == 0)
                    meptr->b18 = meptr->b18 & 0xBF | 0x40;
                else
                    try_to_open_door(collobject);
            } else if ((((item = collobject->id & 0x1FF) & 0x1C0) >> 6) == 1
                && item != 0x7F
                && ((meptr->goal_word & 0xF) >> 0) == 5
                && ((collobject->goal_word & 0xF) >> 0) == 5) {
            } else if ((item >> 4) == 0x14 && (item & 0xF) >= 8
                && mycst->b0A.flier) {
                meptr->b14 = meptr->b14 & 7 | 0x70;
                failed = 0;
                dontchangedz = 1;
                opening = 1;
            } else
                meptr->b18 = meptr->b18 & 0xBF | 0x40;
        }
        if (failed) {
            if ((meptr->b15 & 0x80) >> 7) {
                freepaths |= 1 << (meptr->home & 0xF);
                meptr->b15 = meptr->b15 & 0x7F;
            }
            meptr->b18 = meptr->b18 & 0x7F;
            blocked = 1;
        }
    }
    if ((meptr->b15 & 0x80) >> 7) {
        if (!move_along_path(&paths[meptr->home & 0xF])) {
            freepaths |= 1 << (meptr->home & 0xF);
            meptr->b15 = meptr->b15 & 0x7F;
        }
    } else if (!((meptr->b18 & 0x20) >> 5) && ((meptr->b18 & 0x80) >> 7)) {
        heading = deltatotheta(dx, dy);
        set_htx(heading);
        if (mycst->b0A.flier) adjust_height(x, y);
    } else if (!((meptr->b18 & 0x20) >> 5) && ((meptr->b18 & 0x40) >> 6)) {
        if (rand() % 8 == 0) meptr->b18 = meptr->b18 & 0xBF;
        crit_drunkwalk();
        return;
    } else if (!blocked && beeline(myxpos, myypos, x, y) == 1) {
        meptr->b18 = meptr->b18 & 0x7F | 0x80;
        heading = deltatotheta(dx, dy);
        set_htx(heading);
        meptr->b18 = meptr->b18 & 0xBF;
        if ((meptr->b15 & 0x80) >> 7) {
            freepaths |= 1 << (meptr->home & 0xF);
            meptr->b15 = meptr->b15 & 0x7F;
        }
    } else if (find_free_path(&slot)
        && flood_path(myxpos, myypos, (meptr->pos & 0x7F) >> 3, x, y, z,
                      acceptable_danger())) {
        freepaths &= ~(1 << slot);
        store_path(&paths[slot]);
        meptr->b18 = meptr->b18 & 0xBF;
        meptr->b15 = meptr->b15 & 0x7F | 0x80;
        meptr->home = meptr->home & 0xFFF0 | (slot & 0xF) << 0;
        move_along_path(&paths[meptr->home & 0xF]);
    } else {
        meptr->b18 = meptr->b18 & 0xBF | 0x40;
        meptr->b18 = meptr->b18 & 0x7F;
        crit_drunkwalk();
        return;
    }
    if (!didmove) {
        meptr->b15 = meptr->b15 & 0xBF;
        if ((meptr->b15 & 0x3F) != 1) {
            meptr->b15 = meptr->b15 & 0xC0 | 1;
            set_cur_seq_len();
            if (((meptr->goal_word & 0xF000) >> 12) > seq_lframe - 1)
                meptr->goal_word = meptr->goal_word & 0xFFF;
        } else
            meptr->goal_word = meptr->goal_word & 0xFFF
                | ((((meptr->goal_word & 0xF000) >> 12) + 1) % seq_len & 0xF) << 12;
        if (opening) speed = 0;
        else speed = ((meptr->goal_word & 0xF) >> 0) == 5 ? mycst->run : mycst->speed;
        meptr->b13 = meptr->b13 & 0x80 | (speed & 0x7F) << 0;
        meptr->b14 = meptr->b14 & 0xF8 | 4;
    }
}
void far adjust_height(unsigned char x, unsigned char y) {
    struct Tile far *destinationTile, far *originTile;
    unsigned char z, destinationHeight, originHeight, newPitch;
    signed char change;
    change = 0;
    if (dontchangedz) return;
    destinationTile = Map_GetAddr(x, y);
    originTile = Map_GetAddr(myxpos, myypos);
    z = meptr->pos & 0x7F;
    destinationHeight = (destinationTile->height << 3) + 0x14;
    originHeight = (originTile->height << 3) + 0x14;
    if (destinationHeight > 0x78) destinationHeight = 0x78;
    if (originHeight > 0x78) originHeight = 0x78;
    if (z < originHeight - 12) change = 2;
    else {
        if ((hitwall && z < 0x78) || z < destinationHeight - 8) change = 2;
        else if (z > 0x78 || z > destinationHeight + 8) change = -2;
        else change = rand() % 3 - 1;
        if (z < originHeight - 4) change++;
        if (z > originHeight + 12) change--;
    }
    newPitch = change + 0x10;
    meptr->b14 = (meptr->b14 & 7) | ((newPitch & 0x1F) << 3);
}
void far try_to_open_door(struct Object far *door) {
    if ((door->id & 7) == 7) return;
    if (PlayerLevel == 10) return;
    if (mycst->locks) {
        MapObj_X = doorx;
        MapObj_Y = doory;
        UseObj(meptr, door, 0);
    }
    if (((door->id & 0x1F0) >> 4) != 0x14 || (door->id & 0xF) >= 8) return;
    if (mycst->locks && rand() % 2) {
        checkLock(meptr, door, -mycst->locks);
        return;
    }
    if (rand() % 4 == 0)
        damage_item(door, meptr, doorx, doory, rand() % mycst->door_damage, 4);
}
