/* target: seg006_1413 */
/* opts: -mm -1 -G -O -Y -d */
/* Critter motion, homing projectiles, path traversal and doors: the whole of DOS resident
   segment seg006_1413, in original order.

   What it does in the game: it gets a critter to a square. crit_head_for_loc, called by
   AI.C's goals, picks the way: follow a stored path, walk straight at the square when
   beeline finds the tiles between walkable, or search for a path with flood_path (a
   breadth-first flood over at most 40 steps in a box 5 tiles around the start and the
   destination) and store it in one of 16 shared path slots, or give up and wander. A
   critter that bumps into a door tries to open it (try_to_open_door). It also has the
   collision handlers the physics engine calls for walking, flying and swimming critters
   (crit_hndlr_walk, _fly, _swim, installed by init_ai), move_me_joe for mobile objects
   that are not critters, the steering of homing darts (check_homing) and of satellites
   (check_sat), line_of_sight, and build_corpse, which leaves a dead critter's remains.

   Data owned: the per-critter scratch globals AI.C works with (meptr, mycst, myid, the
   critter's tile, fine position, height and home, its target's position and distances,
   and the flags of the last physics step: control, failed, aligned, hitwall, didhitobj,
   hitadoor, didmove); the path squares of the search (pathsq, pathlen), the 16 stored
   paths (paths) with their free mask (freepaths), the flood frontiers, and the step
   tables (PathingOffset, path_turns, slope_for_dir). The flood search writes its
   per-square records (struct StaticTile, critter.h) into stdat, a 64 by 64 far buffer
   shared with other code.

   Path terrain: hyp_move's flags argument is the handler's noclimb word and costflags
   its w6 word (init_ai): 0x1000 asks for height checks, and bit 8 << class forbids
   (flags) or adds 2 to the danger of (costflags) a floor of terrain class 0 plain, 1
   water, 2 lava or 3 ice. So walkers avoid water and treat lava as dangerous, and
   swimmers (noclimb 0x10A8) keep off plain floor and lava. A path may also drop to a
   lower floor; the danger a path collects (drops cost their height less one) must stay
   within the critter's acceptable_danger (AI.C).

   Neighbours: AI.C (seg007) calls crit_head_for_loc, deltatotheta, line_of_sight,
   set_loc and build_corpse; MOTION.C's do_physics moves the critters and calls the
   handlers; CRITTIME.C uses flood_path to bring wandering monsters to the sleeping
   player; OBJUSE.C's checkLock and UseObj open doors.

   name: function names are the FM Towns originals; the FM Towns build has the functions
   in the same order, which fixes the names of the ones the target table lists under IDA
   names: store_targ (HomingDartTargeting), crit_hndlr_fly and crit_hndlr_swim
   (FlierNPCCollision, SwimmerNPCCollision), crit_hndlr_obj (static here, the tail of
   crit_hndlr_walk's range; init_ai installs the four handlers in FM Towns as in DOS),
   hyp_move (TraverseMultipleTiles), make_path_from_flood_data (StorePath),
   add_to_beeline_path (TestStraightPathTraversal), find_free_path
   (FindSetIndexOfBitField), store_path (UpdateSeg57TurningValues) and close_to_square
   (CheckIfAtOrNearTargetTile).
   Name: inferred (the job of System Shock's PATHFIND.C: critter motion and path finding,
   flood_path, find_free_path, move_along_path). */
#include <stdlib.h>
#include <dos.h>
#include "combat.h"
#include "critter.h"
#include "event.h"
#include "file.h"
#include "gfx.h"
#include "map.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"
#include "uw2.h"

struct PathOffset { signed char x,y; };
/* match: uninitialised data, DS:222C..227F: this file's _BSS. Turbo C lays it out by a
   hash of each name (tools/bssorder.py), so the definitions below are in that order.
   Many of these are used only by seg007 (critter AI), which declares them extern. */
/* name: the names are the FM Towns ones; FM Towns keeps the six marked static as statics
   (at _tp_act+N), so their names are chosen here to land at the right address. */
int16 crit_terr;                        /* 222C */
unsigned char doorx, doory;             /* 222E */
int16 tdx, tdy;                         /* 2230 */
unsigned char control;                  /* 2234 */
static int16 proj_ycoord;               /* 2236, homing dart's fine y; name chosen for layout */
struct Object far *collobject;          /* 2238 */
struct Handler near *tp_act;             /* 223C */
unsigned char txpos;                    /* 223E */
static int16 hdist;                     /* 2240, nearest homing target's distance; for layout */
unsigned char aligned;                  /* 2242 */
static int16 htarget;                   /* 2244, nearest homing target; for layout */
unsigned char dontchangedz;             /* 2246 */
uint16 txpost;                          /* 2248 */
unsigned char myid;                     /* 224A */
static unsigned char jump;              /* 224B, the last step needs a jump; for layout */
uint16 tdistsqr;                        /* 224C */
unsigned char failed;                   /* 224E */
unsigned char typos;                    /* 224F */
uint32 tdisttsqr;                       /* 2250 */
struct Creature near *mycst;            /* 2254 */
struct Object far *meptr;               /* 2256 */
unsigned char pathlen;                  /* 225A */
unsigned char myxpos, myypos, myzpos;   /* 225B */
unsigned char hitwall;                  /* 225E */
uint16 typost;                          /* 2260 */
struct Object far *mytarget;            /* 2262 */
unsigned char myxhome, myyhome;         /* 2266 */
unsigned char myheight;                 /* 2268 */
unsigned char didhitobj;                /* 2269 */
int16 myxpost, myypost;                 /* 226A */
unsigned char didmove;                  /* 226E */
unsigned char hitadoor;                 /* 226F */
struct Phys near *pn_act;           /* 2270 */
static unsigned char cur_danger;        /* 2272, danger a path may cross; for layout */
unsigned char myoldspeed;               /* 2273 */
signed char tzpos;                      /* 2274 */
int16 XP;                               /* 2276 */
unsigned char myoldfacing;              /* 2278 */
int16 YP;                               /* 227A */
unsigned char myoldheading;             /* 227C */
static int16 projxpos;                  /* 227E, homing dart's fine x; name chosen for layout */
/* The squares of the path being built (pathsq, pathlen of them; beeline keeps each
   square's floor height in the unused byte, flood_path marks a step that needs a jump in
   flag) and the 16 stored paths, one per critter following a path (its slot in the low
   nibble of its home word, b15 bit 7 set while it has one). */
/* match: this file's far data. Turbo C gives each far variable a paragraph-aligned
   segment of its own (SEG006n_FAR), in definition order, and TLINK places the segments
   in the order it first sees them: so pathsq (6062:0000, 63 squares), paths (6072:0000,
   16 paths), flood_list0 (608E:0000) and flood_list1 (6096:0000) are the far segments
   that PATHFIND defined, segment table entries 65 to 68, the first after those no C file
   defined. */
struct PathSq far pathsq[63];
struct PathRec far paths[16];
/* The two flood fill frontiers (64 squares each). */
/* name: static in FM Towns, provisional names. */
struct PathPt { unsigned char x, y; };
static struct PathPt far flood_list0[64], far flood_list1[64];
/* Initialised data, DS:00AC..00C2: the step for each of the four path directions (0 +y,
   1 +x, 2 -y, 3 -x), the free path slots (a bit per entry of paths), the direction from
   one square to the next by [dx + 1][dy + 1], and the slope tile type climbed in each
   direction (TILE_SLOPE_N ... W). */
/* name: FM Towns keeps the three tables as statics (__D16Infoseg+N), so their names are
   provisional. */
static struct PathOffset PathingOffset[4] = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };
uint16 freepaths = 0xFFFF;
static unsigned char path_turns[3][3] = {
    { 0xFF, 3, 0xFF }, { 2, 0xFF, 0 }, { 0xFF, 1, 0xFF } };
static unsigned char slope_for_dir[4] = { 6, 8, 7, 9 };
static unsigned char far crit_hndlr_obj(struct Phys *pn);

/* Leave a dead critter's remains. fluids (the creature's corpse field, AI.C passes it
   first) makes item 0xD9 + fluids where the critter lay (0xDB rubble, 0xDC wood chips,
   0xDD bones, 0xDE and 0xDF blood stains, ...). corpse (its remains field) makes item
   0xC0 + corpse (0xC2 skull, 0xC4 bone, ...) placed near the critter, with the critter
   type as owner; outside the Pits of Carnage (world 7) only 7 times in 16. */
void far build_corpse(struct Object far *obj, char fluids, char corpse) {
    struct Object far *remains;
    struct Tile far *home = Map_GetAddr(OBJ_HOMEX(obj),
                                        OBJ_HOMEY(obj));
    if (fluids == 0) goto corpse_part;
    if ((remains = CreateObj(0xD9 + (unsigned char)fluids, 0)) == 0)
        goto corpse_part;
    SET_FINEX(remains, OBJ_FINEX(obj));
    SET_FINEY(remains, OBJ_FINEY(obj));
    SET_Z(remains, OBJ_Z(obj));
    remains->qn.f.quality = 0x28;
    Obj_Add(&home->objects, remains);
    obj_deal(remains, XP, YP, 1);
corpse_part:
    if (corpse) {
        if (rand() % 16 >= 7 && (PlayerLevel - 1) / LEVELS_PER_WORLD != 7) return;
        if ((remains = CreateObj(0xC0 + (unsigned char)corpse, 0)) != 0) {
            remains->ol.f.owner = OBJ_INMAJOR(obj);
            near_mob_put_at(obj, remains, 4, 0);
        }
    }
}

/* One step for a mobile object that is not a critter (a missile, a thrown object ...),
   called by AI.C's move_mobile. A spent object (hp 0) of quality class below 3 is
   deleted (Obj_Punt; returns 0 so the loop revisits the slot), or given 1 hit point if
   Obj_Punt keeps it. Objects marked no_hit ignore
   falling (CT3.ignore 0x1000). After the physics step its time bin advances, and homing
   darts and satellites steer. */
unsigned char far move_me_joe(void) {
    unsigned char result;
    unsigned char unused;
    if (meptr->hp == 0
        && ComObjData[meptr->id & ID_ITEM].qualclass < 3) {
        if (Obj_Punt(&Map_GetAddr(OBJ_HOMEX(meptr),
                                  OBJ_HOMEY(meptr))->objects,
                     meptr, 0))
            meptr->hp = 1;
        else return 0;
    }
    if (ComObjData[meptr->id & ID_ITEM].no_hit)
        CT3.ignore = 0x1000;
    else CT3.ignore = 0;
    get_phys_data(meptr, &CN3);
    do_crit_phys(&CN3, &CT3);
    XP = OBJ_HOMEX(meptr);
    YP = OBJ_HOMEY(meptr);
    result = set_phys_data(meptr, &CN3);
    if (result) {
        SET_BIN(meptr, OBJ_BIN(meptr) + OBJ_RATE(meptr));
        if (OBJ_ITEM(meptr) == ITEM_HOMING_DART) check_homing();
        else if (OBJ_ITEM(meptr) == ITEM_SATELLITE) check_sat();
    }
    (void)unused;
    return result;
}
/* process_area callback for check_homing: remember the object nearest the dart
   (htarget, by fine Manhattan distance). */
unsigned char far store_targ(int unused1, int unused2,
                                                      struct Object far *target) {
    int x = (OBJ_HOMEX(target) << 3)
          + OBJ_FINEX(target);
    int y;
    register int distance;
    y = (OBJ_HOMEY(target) << 3)
      + OBJ_FINEY(target);
    distance = abs(x - projxpos) + abs(y - proj_ycoord);
    if (distance < hdist) {
        htarget = Obj_MemTPtr(target);
        hdist = distance;
    }
    return 0;
}
/* Turn angle src (0-255) magnitude/64 of the way towards dst, the short way round. */
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
/* Steer a homing dart: look for the nearest object in a 3 by 4 tile box ahead of it
   (process_area, skipping its shooter, last_hit) and turn towards it, harder the closer
   it is, and pitch towards its height; with no target it weaves at random. It trails a
   sparkle effect (type 14) half the time and loses a hit point one step in four, which
   limits its flight; a dart with the loner bit set dies at once. */
void far check_homing(void) {
    int xhome, yhome, width, height;
    register int step, cardinal;
    width = height = 3;
    xhome = OBJ_HOMEX(meptr) - 1;
    yhome = OBJ_HOMEY(meptr) - 1;
    cardinal = (((unsigned char)meptr->heading + 0x20) & 0xFF) >> 6;
    cardinal = ((OBJ_HEADING(meptr) + 1) & 7) >> 1;
    if (cardinal > 1) {
        if (cardinal == 3) xhome--;
        else yhome--;
    }
    if (cardinal & 1) width++;
    else height++;
    htarget = -1;
    hdist = 0x7FFF;
    projxpos = (OBJ_HOMEX(meptr) << 3)
        + OBJ_FINEX(meptr);
    proj_ycoord = (OBJ_HOMEY(meptr) << 3)
        + OBJ_FINEY(meptr);
    process_area(1, meptr->last_hit,
        (SpellFn)store_targ, 0,
        xhome, yhome, width, height);
    if (htarget > 0) {
        struct Object far *target;
        unsigned vector;
        int heading, pitch;
        unsigned heightdiff, desiredpitch, newpitch;
        target = Obj_IntTMem(htarget);
        xhome = (OBJ_HOMEX(target) << 3)
            + OBJ_FINEX(target);
        yhome = (OBJ_HOMEY(target) << 3)
            + OBJ_FINEY(target);
        vector = get_theta(
            projxpos, proj_ycoord, xhome, yhome);
        if (hdist < 12) step = 2;
        else if (hdist < 32) step = 4;
        else step = 6;
        heading = merge_angles(meptr->heading, vector >> 8, (step + 8) << 2);
        meptr->heading = heading;
        pitch = OBJ_PITCH(meptr);
        heightdiff = OBJ_Z(target) - OBJ_Z(meptr);
        desiredpitch = heightdiff / 24 + 16;
        newpitch = (pitch * (step + 4) + desiredpitch * (8 - step)) / 12;
        meptr->b14 = OBJ_RATE(meptr) | ((newpitch & 0x1F) << 3);
    } else {
        int pitch;
        meptr->heading = (meptr->heading + (rand() & 7) - 3) & 0xFF;
        pitch = OBJ_PITCH(meptr);
        if (!(rand() & 4)) {
            if (pitch < 14) pitch++;
            else if (pitch > 18) pitch--;
            else pitch = pitch + (rand() & 3) - 1;
        }
        meptr->b14 = OBJ_RATE(meptr) | ((pitch & 0x1F) << 3);
    }
    if (!(rand() & 2))
        put_effect(meptr, 14, 3, 0, -OBJ_Z(meptr),
                   OBJ_HOMEX(meptr),
                   OBJ_HOMEY(meptr));
    if (OBJ_LONER(meptr) != 0) meptr->hp = 0;
    else if (!(rand() & 3) && meptr->hp) meptr->hp = meptr->hp - 1;
}
/* Steer a satellite round its source (last_hit, the caster): its heading mixes the
   tangent to the circle round the source with a pull back towards a distance of about
   4 fine units, its pitch follows the source's height, and its speed is kept at 15. It
   loses a hit point about one step in 32. */
void far check_sat(void) {
    int satx, saty, srcx, srcy;
    int dist, turn, pitch, zdiff, zpitch, newpitch;
    struct Object far *src;
    int heading;
    uint16 theta;
    int divisor = 2;
    register uint16 vector;
    register int diff;
    src = Obj_IntTMem(meptr->last_hit);
    srcx = (OBJ_HOMEX(src) << 3) + OBJ_FINEX(src);
    srcy = (OBJ_HOMEY(src) << 3) + OBJ_FINEY(src);
    satx = (OBJ_HOMEX(meptr) << 3) + OBJ_FINEX(meptr);
    saty = (OBJ_HOMEY(meptr) << 3) + OBJ_FINEY(meptr);
    vector = get_theta(satx, saty, srcx, srcy);
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
    pitch = OBJ_PITCH(meptr);
    zdiff = OBJ_Z(src) - OBJ_Z(meptr) + 0xF;
    zpitch = zdiff / 8 + 0x10;
    newpitch = (pitch + zpitch) / 2;
    if (!(rand() & 3)) {
        if (newpitch < 0xF) newpitch++;
        else if (newpitch > 0x11) newpitch--;
        else newpitch = newpitch + (rand() & 3) - 1;
    }
    meptr->b14 = OBJ_RATE(meptr) | ((newpitch & 0x1F) << 3);
    if (OBJ_SPEED(meptr) < 0xF)
        SET_SPEED(meptr, 0xF);
    if (OBJ_LONER(meptr) != 0) meptr->hp = 0;
    else if ((rand() & 0x1F) == 1 && meptr->hp > 0) meptr->hp = meptr->hp - 1;
}
/* Set up the critter physics records and handlers: CN1/CT1 walkers, CN2/CT2 fliers
   (never fall: ignore 0x1000), CN3/CT3 other mobile objects, CN4/CT4 swimmers (ignore
   water, 0x10). mask is the collision bits passed to the special handler; noclimb and
   w6 also serve flood_path as forbidden and costly terrain (see the file header). */
void far init_ai(void) {
    CN1.acc[0] = 0; CN1.acc[1] = 0; CN1.flags = 0x80;
    CN2.acc[0] = 0; CN2.acc[1] = 0; CN2.flags = 0x80;
    CN3.acc[0] = 0; CN3.acc[1] = 0; CN3.flags = 0;
    CN4.acc[0] = 0; CN4.acc[1] = 0; CN4.flags = 0x80;
    CT1.mask = 0x1F30; CT1.noclimb = 0x1010; CT1.w6 = 0x20;
    CT1.ignore = 0; CT1.special = (unsigned char (far *)(uint16 *))crit_hndlr_walk;
    CT2.mask = 0x700; CT2.noclimb = 0x80; CT2.w6 = 0;
    CT2.ignore = 0x1000; CT2.special = (unsigned char (far *)(uint16 *))crit_hndlr_fly;
    CT3.mask = 0; CT3.noclimb = 0; CT3.w6 = 0;
    CT3.ignore = 0; CT3.special = (unsigned char (far *)(uint16 *))crit_hndlr_obj;
    CT4.mask = 0x1728; CT4.noclimb = 0x10A8; CT4.w6 = 0;
    CT4.ignore = 0x10; CT4.special = (unsigned char (far *)(uint16 *))crit_hndlr_swim;
}
/* The collision bits (MOTION.C's list) for obj where it stands, from TerrainCheck. */
int far get_terrain(struct Object far *obj) {
    struct MotionCalc calc;
    curP = &calc;
    curP->index = Obj_MemTPtr(obj);
    curP->radius = ComObjData[obj->id & ID_ITEM].radius;
    curP->height = ComObjData[obj->id & ID_ITEM].height;
    curP->x = (OBJ_HOMEX(obj) << 3) + OBJ_FINEX(obj);
    curP->y = (OBJ_HOMEY(obj) << 3) + OBJ_FINEY(obj);
    curP->z = obj->pos & POS_Z;
    TerrainCheck(8);
    return curP->hits0 | curP->hits1;
}
/* The walking critter's collision handler (CT1.special), called by do_physics with the
   collision bits. Falling (0x1000) starts gravity and takes control away. A critter
   whose footprint is on water and nothing else ((bits & 0xF8) == 0x10) drowns: a splash
   (effect 6) and the last frame of its dying sequence. Otherwise water, and a drop
   (0x800) or lava (0x20) it was not already on, stop it (failed) unless it is following
   a stored path; a wall or high step (0x300) fails; an object (0x400) is noted in
   collobject, with hitadoor for a door. */
unsigned char far crit_hndlr_walk(struct Phys *pn) {
    struct Object far *door;
    register struct Phys *motion = pn;
    if (motion->x & 0x1000) {
        if (CN1.acc[2] == 0) CN1.acc[2] = -4;
        SET_RATE(meptr, 1);
        failed = 1;
        control = 0;
        return 0;
    }
    if (motion->x & 0x10) {
        if ((motion->x & 0xF8) == 0x10) {
            failed = 1;
            control = 0;
            put_effect(meptr, 6, 3, 0, 0, CN1.x >> 8, CN1.y >> 8);
            SET_SEQ(meptr, 7);
            set_cur_seq_len();
            SET_FRAME(meptr, seq_lframe);
            SET_RATE(meptr, 1);
            return 1;
        }
        if (OBJ_B15_7(meptr) == 0) {
            failed = 1;
            CN1.vel[0] = CN1.vel[1] = 0;
            return 1;
        }
    }
    if ((motion->x & 0x800) && !(crit_terr & 0x800)) {
        if OBJ_B15_7(meptr) return 0;
        failed = 1;
        CN1.vel[0] = CN1.vel[1] = 0;
        return 1;
    }
    if ((motion->x & 0x20) && !(crit_terr & 0x20)) {
        if OBJ_B15_7(meptr) return 0;
        failed = 1;
        CN1.vel[0] = CN1.vel[1] = 0;
        return 1;
    }
    if (motion->x & 0x300) {
        failed = 1;
        return 0;
    }
    if (motion->x & 0x400) {
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
static unsigned char far crit_hndlr_obj(struct Phys *pn) {
    (void)pn; return 0;
}
unsigned char far crit_hndlr_fly(struct Phys *pn) {
    control = 1;
    if (pn->x & 0x200) {
        failed = 1;
        return 0;
    } else {
        if (pn->x & 0x100) {
            CN2.vel[2] = 0x80;
            hitwall = 1;
        }
        if (pn->x & 0x400) {
            failed = 1;
            didhitobj = 1;
            collobject = CollObject();
        }
        return failed && control;
    }
}
unsigned char far crit_hndlr_swim(struct Phys *pn) {
    if (pn->x & 0x300) {
        CN4.vel[0] = CN4.vel[1] = 0;
        failed = 1;
        return 0;
    }
    if (pn->x & 0x400) {
        CN4.vel[0] = CN4.vel[1] = 0;
        failed = 1;
        didhitobj = 1;
        collobject = CollObject();
    }
    if (pn->x & 8) {
        CN4.vel[0] = CN4.vel[1] = 0;
        failed = 1;
    }
    return failed && control;
}
unsigned char far do_crit_phys(struct Phys *pn, struct Handler *tp) {
    pn->time = OBJ_RATE(meptr) << 4;
    do_physics(pn, tp);
    return 1;
}
/* Can a critter move from square (x2, y2) to (x3, y3), having come from (x1, y1)?
   x1 == 0 means (x2, y2) is the start, x3 == 0 that (x2, y2) is the destination.
   Checks the tile walls between the squares (tile_walls), locked doors (checkLock 0)
   whose frame lies across the move, given by the door's heading and its fine position,
   the floor and solid object heights (a step up of more than one height unit fails,
   a slope counts one higher unless climbed the right way) and the terrain (see the file
   header). height is the critter's floor height on entering (x2, y2); *out gets the
   height after the move and *dist collects danger, which must stay within cur_danger.
   Sets jump when the move needs one, which only creatures with bA_5 make. Returns 1
   if the move is possible. */
unsigned char far hyp_move(unsigned char x1, unsigned char y1,
    unsigned char x2, unsigned char y2, unsigned char x3, unsigned char y3,
    int flags, int costflags, unsigned char height, unsigned char far *out,
    unsigned char far *dist) {
    struct Tile far *tile2, far *tile1, far *tile3;
    unsigned char type2, type3;
    int terr2, terr3;
    unsigned char h12, h23, h23b, objh, blocked, onobj;
    union Link far *link;
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
        if (x3 > x2 && (tile_walls[type3] & TW_WEST)) return 0;
        if (x3 < x2 && (tile_walls[type3] & TW_EAST)) return 0;
        if (y3 > y2 && (tile_walls[type3] & TW_SOUTH)) return 0;
        if (y3 < y2 && (tile_walls[type3] & TW_NORTH)) return 0;
        if (x3 > x2 && (tile_walls[type2] & TW_EAST)) return 0;
        if (x3 < x2 && (tile_walls[type2] & TW_WEST)) return 0;
        if (y3 > y2 && (tile_walls[type2] & TW_NORTH)) return 0;
        if (y3 < y2 && (tile_walls[type2] & TW_SOUTH)) return 0;
        if (!(0x1000 & flags)) return 1;
        if (type3 >= TILE_SLOPE_N && type3 <= TILE_SLOPE_W
            && slope_for_dir[path_turns[x3 - x2 + 1][y3 - y2 + 1]] != type3)
            h23++;
        if (h23 > height + 1) return 0;
        return 1;
    } else if (x3 == 0) {
        *out = height;
        if (!(0x1000 & flags)) return 1;
        objh = 0;
        for (link = (union Link far *)&tile2->objects.word; link->f.index && objh == 0;
             link = (union Link far *)&obj->qn) {
            obj = Obj_PtrTMem(link);
            com = &ComObjData[obj->id & ID_ITEM];
            if (com->solid)
                objh = (OBJ_Z(obj) + com->height) >> 3;
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
        if ((tile_walls[type3] & TW_WEST) || (tile_walls[type2] & TW_EAST)) return 0;
    } else if (x3 < x2) {
        if ((tile_walls[type3] & TW_EAST) || (tile_walls[type2] & TW_WEST)) return 0;
    } else if (y3 > y2) {
        if ((tile_walls[type3] & TW_SOUTH) || (tile_walls[type2] & TW_NORTH)) return 0;
    } else if (y3 < y2) {
        if ((tile_walls[type3] & TW_NORTH) || (tile_walls[type2] & TW_SOUTH)) return 0;
    }
    objh = 0;
    for (link = (union Link far *)&tile2->objects.word; link->f.index && objh == 0;
         link = (union Link far *)&obj->qn) {
        obj = Obj_PtrTMem(link);
        com = &ComObjData[obj->id & ID_ITEM];
        if (OBJ_MAJOR(obj) == MAJOR_RECT && OBJ_MINOR(obj) == MINOR_DOOR
            && OBJ_INCLASS(obj) < 8) {
            if (checkLock(meptr, obj, 0) == 0) {
                dhead = OBJ_HEADING(obj) & 3;
                dxp = OBJ_FINEX(obj);
                dyp = OBJ_FINEY(obj);
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
        } else if (com->solid)
            objh = (OBJ_Z(obj) + com->height) >> 3;
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
                if (mycst->bA_5) {
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
        if (mycst->bA_5) {
            *dist = *dist + 1;
            if (*dist >= cur_danger) return 0;
            jump = 1;
            return 1;
        } else return 0;
    }
    *out = h23b;
    return 1;
}
/* line_of_sight's step test: can a sight line at height (fine units) pass from square
   (x1, y1) into (x2, y2)? Fails on a wall between them or a floor above the line. */
unsigned char far hyp_see(unsigned char oldx, unsigned char oldy,
    unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2,
    unsigned char height) {
    struct Tile far *tile1 = Map_GetAddr(x1, y1);
    struct Tile far *tile2 = Map_GetAddr(x2, y2);
    unsigned char type1 = tile1->type;
    unsigned char type2 = tile2->type;
    if (oldx == 0 || (oldx == x1 && oldy == y1)) {
        if (x2 > x1 && (tile_walls[type2] & TW_WEST)) return 0;
        if (x2 < x1 && (tile_walls[type2] & TW_EAST)) return 0;
        if (y2 > y1 && (tile_walls[type2] & TW_SOUTH)) return 0;
        if (y2 < y1 && (tile_walls[type2] & TW_NORTH)) return 0;
        if (x2 > x1 && (tile_walls[type1] & TW_EAST)) return 0;
        if (x2 < x1 && (tile_walls[type1] & TW_WEST)) return 0;
        if (y2 > y1 && (tile_walls[type1] & TW_NORTH)) return 0;
        if (y2 < y1 && (tile_walls[type1] & TW_SOUTH)) return 0;
        if ((int)height >> 3 < tile2->height) return 0;
        else return 1;
    } else {
        if (x2 == 0) return 1;
        if (x2 > x1 && (tile_walls[type2] & TW_WEST)) return 0;
        if (x2 < x1 && (tile_walls[type2] & TW_EAST)) return 0;
        if (y2 > y1 && (tile_walls[type2] & TW_SOUTH)) return 0;
        if (y2 < y1 && (tile_walls[type2] & TW_NORTH)) return 0;
        if (x2 > x1 && (tile_walls[type1] & TW_EAST)) return 0;
        if (x2 < x1 && (tile_walls[type1] & TW_WEST)) return 0;
        if (y2 > y1 && (tile_walls[type1] & TW_NORTH)) return 0;
        if (y2 < y1 && (tile_walls[type1] & TW_SOUTH)) return 0;
        if ((int)height >> 3 < tile2->height) return 0;
    }
    return 1;
}
/* Search for a walkable path from (x, y) at floor height height0 to (destx, desty),
   breadth first, with hyp_move as the step test and range as the danger allowance.
   The search stays in a box reaching 5 tiles beyond the start and destination, takes
   at most 40 steps and keeps at most 64 squares in each frontier; where two routes meet
   it prefers the one ending nearer destz. On success the path is in pathsq and pathlen
   and it returns 1. */
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
    /* The test reads "world 0, level 0 of the world", which is true only for PlayerLevel
       0, so in play the reach is always 5. */
    if ((PlayerLevel - 1) / LEVELS_PER_WORLD == 0 && (PlayerLevel - 1) % LEVELS_PER_WORLD + 1 == 0) reach = 10;
    else reach = 5;
    minx = x < destx ? (x - reach > 1 ? x - reach : 1)
                     : (destx - reach > 1 ? destx - reach : 1);
    miny = y < desty ? (y - reach > 1 ? y - reach : 1)
                     : (desty - reach > 1 ? desty - reach : 1);
    maxx = x > destx ? (x + reach < MAP_SIZE ? x + reach : MAP_SIZE)
                     : (destx + reach < MAP_SIZE ? destx + reach : MAP_SIZE);
    maxy = y > desty ? (y + reach < MAP_SIZE ? y + reach : MAP_SIZE)
                     : (desty + reach < MAP_SIZE ? desty + reach : MAP_SIZE);
    STILES[x][y].pathx = x;
    STILES[x][y].height = height0;
    STILES[x][y].step = 0;
    for (d = 0; d < 4; d++) {
        nx = x + PathingOffset[d].x;
        ny = y + PathingOffset[d].y;
        np = &STILES[nx][ny];
        dist = 0;
        if (hyp_move(0, 0, x, y, nx, ny,
                tp_act->noclimb, tp_act->w6, height0, &np->height, &dist)) {
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
            rec = &STILES[tx][ty];
            for (d = 0; d < 4; d++) {
                nx = tx + PathingOffset[d].x;
                ny = ty + PathingOffset[d].y;
                if (nx < minx || nx > maxx || ny < miny || ny > maxy) continue;
                np = &STILES[nx][ny];
                dist = rec->dist;
                if (rec->pathx == nx && rec->pathy == ny) continue;
                ok = hyp_move(rec->pathx, rec->pathy,
                    tx, ty, nx, ny, tp_act->noclimb, tp_act->w6, rec->height,
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
                            tp_act->noclimb, tp_act->w6, np->height, &np->height, &dist)) {
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
/* Read the path back from the flood records in stdat, from the destination (x, y) to
   the start, into pathsq, with each step's jump flag. */
void far make_path_from_flood_data(unsigned char length, unsigned char x,
    unsigned char y) {
    struct StaticTile far *tile;
    unsigned char i;
    pathlen = length + 1;
    pathsq[length + 1].x = x;
    pathsq[length + 1].y = y;
    pathsq[0].flag = 0;
    for (i = length + 1; i > 0; i = i - 1) {
        tile = &STILES[pathsq[i].x][pathsq[i].y];
        pathsq[i - 1].x = tile->pathx;
        pathsq[i - 1].y = tile->pathy;
        pathsq[i].flag = tile->pathflag;
    }
}
/* Is the straight line of tiles from (x1, y1) to (x2, y2) walkable, with no jumps?
   Walks the line tile by tile (a DDA with a 7-bit fraction), testing each step with
   hyp_move and building pathsq. Returns 1 if so, 0 if not, -1 for the same tile. */
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
            slope = ((int32)dy << 7) / dx;
            step = 1;
            side = dy > 0 ? 1 : -1;
        } else {
            major = &y; minor = &x;
            slope = ((int32)dx << 7) / dy;
            step = -1;
            side = dx > 0 ? 1 : -1;
        }
    } else {
        if (dx >= -dy) {
            major = &y; minor = &x;
            slope = ((int32)dx << 7) / dy;
            step = 1;
            side = dx > 0 ? 1 : -1;
        } else {
            major = &x; minor = &y;
            slope = ((int32)dy << 7) / dx;
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
        tp_act->noclimb, tp_act->w6, pathsq[pathlen - 2].unused,
        (unsigned char far *)&pathsq[pathlen - 2].unused, &unused);
}
/* Can a point at (x1, y1, z1) see (x2, y2, z2)? Positions in fine units (8 to a tile),
   heights in object units. Steps the line tile by tile, with the height interpolated,
   testing walls and floors with hyp_see; a line longer than 10 tiles is never clear. */
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
                slope = ((int32)dy << 7) / dx;
                frac = ((y1 & 7) << 4) + slope * (7 - (x1 & 7)) / 8;
            } else {
                slope = ((int32)-dy << 7) / dx;
                frac = ((7 - (y1 & 7)) << 4) + slope * (7 - (x1 & 7)) / 8;
            }
        } else {
            major = &y;
            diff = -dy >> 3;
            minor = &x;
            step = -1;
            dir = dx > 0 ? 1 : -1;
            if (dir == 1) {
                slope = ((int32)dx << 7) / -dy;
                frac = ((7 - (x1 & 7)) << 4) + slope * (y1 & 7) / 8;
            } else {
                slope = ((int32)-dx << 7) / -dy;
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
                slope = ((int32)dx << 7) / dy;
                frac = ((x1 & 7) << 4) + slope * (7 - (y1 & 7)) / 8;
            } else {
                slope = ((int32)-dx << 7) / dy;
                frac = ((7 - (x1 & 7)) << 4) + slope * (7 - (y1 & 7)) / 8;
            }
        } else {
            major = &x;
            diff = -dx >> 3;
            minor = &y;
            step = -1;
            dir = dy > 0 ? 1 : -1;
            if (dir == 1) {
                slope = ((int32)dy << 7) / -dx;
                frac = ((7 - (y1 & 7)) << 4) + slope * (x1 & 7) / 8;
            } else {
                slope = ((int32)-dy << 7) / -dx;
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
/* beeline's step: append (x, y) to pathsq and test the move into it. Fails past 63
   squares or where the step would need a jump. */
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
            tp_act->noclimb, tp_act->w6, pathsq[0].unused,
            (unsigned char far *)&pathsq[1].unused, &unused);
        return traversable && !jump;
    } else {
        traversable = hyp_move(
            pathsq[pathlen - 3].x, pathsq[pathlen - 3].y,
            pathsq[pathlen - 2].x, pathsq[pathlen - 2].y,
            pathsq[pathlen - 1].x, pathsq[pathlen - 1].y,
            tp_act->noclimb, tp_act->w6, pathsq[pathlen - 3].unused,
            (unsigned char far *)&pathsq[pathlen - 2].unused, &unused);
        return traversable && !jump;
    }
}
/* The first free path slot (a set bit of freepaths) in *found; 0 if all 16 are taken. */
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
/* Pack pathsq into a stored path: the start square, the length, two bits of direction
   per step and one jump bit per step. */
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
/* Advance a stored path to its next square (path->x, path->y), noting whether the step
   is a jump; returns 0 at the end of the path. */
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
/* Is a critter on tile (xhome, yhome) at fine position (xpos, ypos) at the path square
   (pathx, pathy)? Unless the step is a jump (flag), being within two fine units of the
   edge towards the square counts as being there. */
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
/* Follow a stored path: step to the next square when at the current one, then head for
   the square's centre (or its near edge), or jump for a jump step. Returns 0 when the
   path is done. */
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
        if (mycst->flier)
            adjust_height(OBJ_DESTX(meptr),
                          OBJ_DESTY(meptr));
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
/* A jump step on a path: walk to the edge of the square, then leap towards the next
   square (rate 1, pitch 22, speed 11). */
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
        heading = deltatotheta((char)x - (char)(OBJ_HOMEX(meptr)),
                               (char)y - (char)(OBJ_HOMEY(meptr)));
        didmove = 1;
        SET_RATE(meptr, 1);
        SET_PITCH(meptr, 0x16);
        SET_SPEED(meptr, 11);
    } else {
        heading = deltatotheta((char)x - (char)myxpost,
                               (char)y - (char)myypost);
    }
    set_htx(heading);
}
/* The nearest of the 8 headings (0 is +y, counting clockwise towards +x) for the vector
   (x, y). */
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
/* Set the current critter's destination square and height (the destination fields of
   the word at 0x0F and the target height). A new destination sets b18 bit 5 (destination
   changed) and clears bit 6 (gave up). */
void far set_loc(unsigned char x, unsigned char y, unsigned char z) {
    if (OBJ_DESTX(meptr) != x
        || OBJ_DESTY(meptr) != y
        || OBJ_TARGETZ(meptr) != z) {
        SET_DESTX(meptr, x);
        SET_DESTY(meptr, y);
        SET_TARGETZ(meptr, z);
        SET_B18_5(meptr, 1);
        SET_B18_6(meptr, 0);
    }
}
/* Move the current critter towards square (x, y) at height z. The b18 flags: bit 5 the
   destination changed, bit 6 gave up on it (wander awhile, cleared at random one step
   in eight), bit 7 the straight line is clear; b15 bit 7 a stored path is in use.
   At the destination it stops (a critter going home, goal 1, starts milling, goal 8).
   Without control (falling, jumping) it only keeps its path in step. After bumping into
   something: a door is tried (try_to_open_door) or, one time in four, given up on; two
   attacking critters ignore each other; a flier passes an open door by changing pitch;
   anything else makes it give up. Then it follows its path, walks straight if beeline
   allows, searches for a path (flood_path, within acceptable_danger) into a free slot,
   or wanders (crit_drunkwalk). A moving critter walks at its run speed when attacking
   (goal 5), else at its walking speed. */
void far crit_head_for_loc(unsigned char x, unsigned char y, char z) {
    char dx, dy;
    unsigned char heading, slot, blocked, opening;
    int speed;
    register int item;
    blocked = 0;
    opening = 0;
    set_loc(x, y, z);
    if (((meptr->b18 & 0x20) >> 5) && OBJ_B15_7(meptr)) {
        freepaths |= 1 << OBJ_PATH(meptr);
        SET_B15_7(meptr, 0);
    }
    dx = x - myxpos;
    dy = y - myypos;
    if (dx == 0 && dy == 0) {
        if OBJ_B15_7(meptr) {
            freepaths |= 1 << OBJ_PATH(meptr);
            SET_B15_7(meptr, 0);
        }
        if (OBJ_GOAL(meptr) == 1)
            critter_set_goal(8, 0);
        else if (control) {
            SET_SPEED(meptr, 0);
            SET_B15_6(meptr, 1);
            SET_SEQ(meptr, 0);
            SET_FRAME(meptr, 0);
            return;
        }
    }
    if (!control) {
        SET_RATE(meptr, 1);
        if (OBJ_B15_7(meptr)
            && paths[meptr->home & 0xF].x == OBJ_HOMEX(meptr)
            && paths[meptr->home & 0xF].y == OBJ_HOMEY(meptr))
            set_next_square_on_path(&paths[meptr->home & 0xF]);
        return;
    }
    if (failed && !aligned && !OBJ_B18_6(meptr)) {
        if (didhitobj) {
            if (hitadoor) {
                SET_SEQ(meptr, 0);
                SET_FRAME(meptr, 0);
                if (rand() % 4 == 0)
                    SET_B18_6(meptr, 1);
                else
                    try_to_open_door(collobject);
            } else if ((((item = collobject->id & ID_ITEM) & ID_MAJOR) >> 6) == MAJOR_CREATURE
                && item != ITEM_ADVENTURER
                && OBJ_GOAL(meptr) == 5
                && OBJ_GOAL(collobject) == 5) {
            } else if ((item >> 4) == CLASS_DOOR && (item & ID_INCLASS) >= 8
                && mycst->flier) {
                SET_PITCH(meptr, 0xE);
                failed = 0;
                dontchangedz = 1;
                opening = 1;
            } else
                SET_B18_6(meptr, 1);
        }
        if (failed) {
            if OBJ_B15_7(meptr) {
                freepaths |= 1 << OBJ_PATH(meptr);
                SET_B15_7(meptr, 0);
            }
            SET_B18_7(meptr, 0);
            blocked = 1;
        }
    }
    if OBJ_B15_7(meptr) {
        if (!move_along_path(&paths[meptr->home & 0xF])) {
            freepaths |= 1 << OBJ_PATH(meptr);
            SET_B15_7(meptr, 0);
        }
    } else if (!((meptr->b18 & 0x20) >> 5) && OBJ_B18_7(meptr)) {
        heading = deltatotheta(dx, dy);
        set_htx(heading);
        if (mycst->flier) adjust_height(x, y);
    } else if (!((meptr->b18 & 0x20) >> 5) && OBJ_B18_6(meptr)) {
        if (rand() % 8 == 0) SET_B18_6(meptr, 0);
        crit_drunkwalk();
        return;
    } else if (!blocked && beeline(myxpos, myypos, x, y) == 1) {
        SET_B18_7(meptr, 1);
        heading = deltatotheta(dx, dy);
        set_htx(heading);
        SET_B18_6(meptr, 0);
        if OBJ_B15_7(meptr) {
            freepaths |= 1 << OBJ_PATH(meptr);
            SET_B15_7(meptr, 0);
        }
    } else if (find_free_path(&slot)
        && flood_path(myxpos, myypos, OBJ_Z(meptr) >> 3, x, y, z,
                      acceptable_danger())) {
        freepaths &= ~(1 << slot);
        store_path(&paths[slot]);
        SET_B18_6(meptr, 0);
        SET_B15_7(meptr, 1);
        /* match: open-coded, as SET_PATH has no shift by 0 */
        meptr->home = meptr->home & 0xFFF0 | (slot & 0xF) << 0;
        move_along_path(&paths[meptr->home & 0xF]);
    } else {
        SET_B18_6(meptr, 1);
        SET_B18_7(meptr, 0);
        crit_drunkwalk();
        return;
    }
    if (!didmove) {
        SET_B15_6(meptr, 0);
        if (OBJ_SEQ(meptr) != 1) {
            SET_SEQ(meptr, 1);
            set_cur_seq_len();
            if (OBJ_FRAME(meptr) > seq_lframe - 1)
                SET_FRAME(meptr, 0);
        } else
            SET_FRAME(meptr, (OBJ_FRAME(meptr) + 1) % seq_len);
        if (opening) speed = 0;
        else speed = OBJ_GOAL(meptr) == 5 ? mycst->run : mycst->speed;
        SET_SPEED(meptr, speed);
        SET_RATE(meptr, 4);
    }
}
/* A flier's pitch on the way to (x, y): keep about 20 height units above the floor here
   and at the destination (never above 0x78), with a little random bobbing. */
void far adjust_height(unsigned char x, unsigned char y) {
    struct Tile far *destinationTile, far *originTile;
    unsigned char z, destinationHeight, originHeight, newPitch;
    signed char change;
    change = 0;
    if (dontchangedz) return;
    destinationTile = Map_GetAddr(x, y);
    originTile = Map_GetAddr(myxpos, myypos);
    z = meptr->pos & POS_Z;
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
    meptr->b14 = OBJ_RATE(meptr) | ((newPitch & 0x1F) << 3);
}
/* A critter bumped into a door. Secret doors (item 0x147, 0x14F) are left alone, and on
   level 10 (Prison Tower, its second level) critters never open doors. A creature with a
   locks value uses the door (UseObj); on a closed door it then tries the lock half the
   time (checkLock with -locks, a lock picking skill check). Otherwise, one time in four,
   it bashes the door for rand() % its first attack's damage (damage_item, type 4). */
void far try_to_open_door(struct Object far *door) {
    if ((door->id & 7) == 7) return;
    if (PlayerLevel == 10) return;
    if (mycst->locks) {
        MapObj_X = doorx;
        MapObj_Y = doory;
        UseObj(meptr, door, 0);
    }
    if (OBJ_CLASS(door) != CLASS_DOOR || OBJ_INCLASS(door) >= 8) return;
    if (mycst->locks && rand() % 2) {
        checkLock(meptr, door, -mycst->locks);
        return;
    }
    if (rand() % 4 == 0)
        damage_item(door, meptr, doorx, doory, rand() % mycst->attacks[0].damage, 4);
}
