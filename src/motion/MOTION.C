/* target: seg031_2CFA */
/* opts: -mm -1 -G -O -Y -d */
/* Motion: stepping a physics record through the tile grid, bouncing off walls, floors and
   objects. The whole of DOS resident segment seg031_2CFA, in original order. Function and
   global names are the originals from the FM Towns symbol table; the source file's own
   name is not known. */

#include <stdlib.h>
#include "map.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "sys.h"

/* This file's _BSS, DS:25C4..26E9 (seg030's ends at 25C3, seg032's starts at 26EA), laid
   out by name (tools/bssorder.py): Ppd 144, bounce_flag 298, PN 336, CN1..CN4 371,
   hit_obj 568, hit_flag 624, CP 731, targ_ceil 764, terr_type 820, PT 848, trycnt 868,
   TP 884, CT1..CT4 931. FM Towns keeps TP, CP, CN4..CN1, PT, PN and CT4..CT1 together, and
   the six without names as statics after _CT1 (+0xC..+0x11), so those are static here,
   with provisional names chosen for their keys. PN, the CNs, PT and the CTs are used only
   by other files (seg006, seg007, seg008, seg035 and overlays). */
struct MotionCalc Ppd;                      /* DS:25C4 */
static signed char bounce_flag;             /* DS:25DB, _CT1+0xF */
struct Phys PN;                             /* DS:25DC */
struct Phys CN1, CN2, CN3, CN4;             /* DS:2604, 262C, 2654, 267C */
static unsigned char hit_obj;               /* DS:26A4, _CT1+0x10 */
static unsigned char hit_flag;              /* DS:26A5, _CT1+0xD */
struct Phys near *CP;                       /* DS:26A6 */
static unsigned char targ_ceil;             /* DS:26A8, _CT1+0x11 */
static unsigned char terr_type;             /* DS:26A9, _CT1+0xE */
struct Handler PT;                          /* DS:26AA */
static unsigned char trycnt;                /* DS:26B6, _CT1+0xC */
struct Handler near *TP;                    /* DS:26B8 */
struct Handler CT1, CT2, CT3, CT4;          /* DS:26BA, 26C6, 26D2, 26DE */

unsigned char res_to_terr[18] = { 0, 0, 1, 0, 2, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 4 };

struct MotionParams MP = {
    0, (int *)&Ppd, { 0 }, { 0 }, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    { 0, 0xE000, 0xC000, 0xA000, 0x8000, 0x6000, 0x4000, 0x2000 }
};

/* Elsewhere in the game. */
void far TerrainCheck(char radius);
void far ObjectCheck(char a, int b);
void far play_effect(char fx, int x, int y, char vol);
long far labs(long v);

/* Later in this file. */
unsigned char far set_resterr(unsigned bits);

void far do_physics(struct Phys *pp, struct Handler *tp)
{
    CP = pp;
    TP = tp;
    MP.vel = CP->vel;
    terr_type = CP->terrain;
    bounce_flag = 0;
    trycnt = 0;
    if (!space_to_motion(1, 1))
        return;
    while (MP.steps + 1 > MP.done) {
        if (trycnt++ == 0x10)
            goto stuck;
        if (grid_move(1))
            check_positions();
    }
    back_to_space();
    return;
stuck:
    CP->speed = CP->vel[2] = CP->acc[2] = 0;
}

/* IDA CopyMotionValsFromInitialToBase25C4: FM Towns space_to_pos_, same position and code. */
void far space_to_pos(void)
{
    curP = &Ppd;
    Ppd.heading = CP->heading;
    Ppd.radius = CP->radius;
    Ppd.height = CP->height;
    Ppd.index = CP->index;
    MP.pos[0] = CP->x >> 5;
    MP.pos[1] = CP->y >> 5;
    MP.pos[2] = CP->z >> 3;
    MP.frac[0] = (CP->x & 0x1F) << 8;
    MP.frac[1] = (CP->y & 0x1F) << 8;
    MP.frac[2] = (CP->z & 7) << 8;
}

void far set_targz(char how)
{
    char solid;
    int i;

    targ_ceil = 1;
    process_objlist();
    MP.hit = 0xFF;
    MP.f23 = 0x7F;
    if (CP->vel[2] > 0) {
        if (Ppd.found > 0 && Ppd.first + Ppd.count < Ppd.found && Ppd.first >= 0) {
            MP.targz = oCollisions[Ppd.first + Ppd.count].bottom - CP->height;
            MP.hit = Ppd.first + Ppd.count;
        } else
            MP.targz = 0x80 - CP->height;
        targ_ceil = 0;
        if (MP.pos[2] + CP->radius < Ppd.top) {
            MP.targz = Ppd.top;
            targ_ceil = 1;
        }
    } else if (CP->vel[2] == 0) {
        MP.targz = Ppd.floor;
        MP.targz = MP.pos[2] + CP->b24 >= Ppd.top ? Ppd.top : Ppd.floor;
        solid = Ppd.count == 0 && Ppd.first > 0 && Ppd.first <= Ppd.found;
        for (i = 0; i < Ppd.found; i++) {
            if (ComObjData[OBJ_ITEM(Obj_IntTMem(oCollisions[i].link.f.index))].touch) {
                if (Ppd.first <= i) {
                    if (oCollisions[i].bottom - 1 < MP.f23)
                        MP.f23 = oCollisions[i].bottom - 1;
                    if (Ppd.first + Ppd.count > i && oCollisions[i].top > MP.targz
                        && oCollisions[i].top < 0x80 - CP->height) {
                        MP.hit = i;
                        MP.targz = oCollisions[i].top;
                    }
                } else if (solid && oCollisions[i].top >= MP.targz
                           && (MP.hit == -1 || !(oCollisions[MP.hit].link.f.low & 0x10)
                               || !(!oCollisions[i].link.f.low & 0x10))) {
                    MP.targz = oCollisions[i].top;
                    MP.hit = i;
                }
            }
        }
    } else {
        MP.targz = Ppd.top;
        if (Ppd.first > 0 && Ppd.first <= Ppd.found && oCollisions[Ppd.first - 1].top > MP.targz) {
            MP.targz = oCollisions[Ppd.first - 1].top;
            MP.hit = Ppd.first - 1;
        }
        targ_ceil = 0;
    }
    if (MP.hit != -1)
        MP.item = OBJ_ITEM(Obj_PtrTMem(&oCollisions[MP.hit].link));
}

unsigned char far space_to_motion(char pos, char check)
{
    int s;
    int c;
    long t;
    long a;
    long b;

    cSinCos(CP->heading, &s, &c);
    t = s;
    CP->vel[0] = t * CP->speed >> 15;
    t = c;
    CP->vel[1] = t * CP->speed >> 15;
    MP.vel[0] += CP->acc[0] * CP->time;
    MP.vel[1] += CP->acc[1] * CP->time;
    MP.vel[2] += CP->acc[2] * CP->time;
    if ((MP.vel[0] | MP.vel[1] | MP.vel[2]) == 0)
        return 0;
    if (pos)
        space_to_pos();
    if (abs(CP->vel[0]) > abs(CP->vel[1]))
        MP.major = 0;
    else
        MP.major = 1;
    MP.minor = (MP.major + 1) % 2;
    if (MP.vel[MP.major] > 0)
        MP.step[MP.major] = 0x2000;
    else
        MP.step[MP.major] = 0xE000;
    if (MP.vel[MP.major] != 0) {
        MP.step[MP.minor] = MP.step[MP.major] / 0x100 * MP.vel[MP.minor] / MP.vel[MP.major] << 8;
        t = MP.vel[MP.major] * CP->time;
        MP.steps = abs(t) >> 13;
        MP.rem = abs(t) & 0x1FFF;
        MP.dt = abs(0x2000 / MP.vel[MP.major]);
    } else {
        MP.step[MP.minor] = 1;
        MP.step[MP.major] = 1;
        t = 0;
        MP.steps = 0;
        MP.rem = 0;
        MP.dt = CP->time;
    }
    MP.done = 0;
    if ((Ppd.index == 1 || MP.vel[2] != 0) && check) {
        TerrainCheck(CP->b24);
        ObjectCheck(0, 0);
    }
    if (MP.vel[2] != 0) {
        if (MP.vel[2] > 0)
            MP.step[2] = 0x800;
        else
            MP.step[2] = 0xF800;
        set_targz(check);
        if (MP.vel[MP.major] != 0) {
            a = MP.vel[2];
            a *= MP.step[MP.major] / 0x2000;
            b = MP.vel[MP.major];
            b *= MP.step[2] / 0x800;
            a *= 0x100;
            a = a / b;
            if (a > 0x7FFF || a < -0x8000L) {
                MP.rem = 0;
                MP.zspeed = abs((MP.vel[2] >> 1) * CP->time >> 5);
            } else
                MP.zspeed = a;
        } else
            MP.zspeed = abs((MP.vel[2] >> 1) * CP->time >> 5);
    } else
        MP.step[2] = 0;
    return 1;
}

void far recalc_vecs(char how)
{
    CP->time -= MP.dt * MP.done;
    if (CP->time <= 0 || space_to_motion(0, how) == 0)
        MP.done = MP.steps + 1;
}

void far stop_me(void)
{
    CP->acc[2] = CP->vel[2] = CP->acc[0] = CP->vel[0] = CP->acc[1] = CP->vel[1] = 0;
    CP->speed = 0;
    MP.done = MP.steps + 1;
}

void far back_to_space(void)
{
    CP->x = (MP.pos[0] << 5) + (MP.frac[0] >> 8);
    CP->y = (MP.pos[1] << 5) + (MP.frac[1] >> 8);
    CP->z = (MP.pos[2] << 3) + (MP.frac[2] >> 8);
    if (Ppd.hits0 & 0x2000 && abs(MP.pos[2] - Ppd.floor) <= CP->radius
        && Ppd.index == 1 && CP->vel[2] == 0)
        CP->z = GetSlopeHgt(CP->x, CP->y);
    CP->heading = Ppd.heading;
}

unsigned char far flat_move(int crossed, int dir)
{
    if (MP.steps + (dir == -1) > MP.done) {
        if (dir == 1)
            MP.frac[MP.minor] += MP.step[MP.minor];
        else
            MP.frac[MP.minor] -= MP.step[MP.minor];
        if (MP.step[MP.major] * dir > 0)
            MP.pos[MP.major]++;
        else
            MP.pos[MP.major]--;
        if (MP.frac[MP.minor] & 0xE000) {
            crossed = 1;
            if (MP.frac[MP.minor] > 0)
                MP.pos[MP.minor]++;
            else
                MP.pos[MP.minor]--;
            MP.frac[MP.minor] &= 0x1FFF;
        }
        MP.done += dir;
        return 1;
    }
    if (dir == 1)
        MP.frac[MP.minor] += (MP.step[MP.minor] >> 5) * (MP.rem >> 8);
    else
        MP.frac[MP.minor] -= (MP.step[MP.minor] >> 5) * (MP.rem >> 8);
    if (MP.step[MP.major] * dir > 0)
        MP.frac[MP.major] += MP.rem;
    else
        MP.frac[MP.major] -= MP.rem;
    if (MP.frac[0] & 0xE000) {
        crossed = 1;
        if (MP.frac[0] > 0)
            MP.pos[0]++;
        else
            MP.pos[0]--;
        MP.frac[0] &= 0x1FFF;
    }
    if (MP.frac[1] & 0xE000) {
        crossed = 1;
        if (MP.frac[1] > 0)
            MP.pos[1]++;
        else
            MP.pos[1]--;
        MP.frac[1] &= 0x1FFF;
    }
    MP.done += dir;
    return crossed;
}

/* IDA seg031_2CFA_A3F: unreferenced, and FM Towns has nothing between flat_move and
   rehead. Its whole body is a compare with no jump, an empty if. */
static void seg031_2CFA_A3F(void)
{
    if (Ppd.open == 9)
        ;
}

unsigned char far rehead(unsigned heading)
{
    register int diff;
    int t;
    unsigned char fx;
    unsigned char fy;
    unsigned char tile;

    if (CP->flags & 0x40)
        return 0;
    if (CP->vel[2] != 0) {
        diff = CP->vel[2] / 16;
        diff *= CP->bounce + 1;
        CP->vel[2] = diff;
    }
    diff = heading - Ppd.heading;
    if (diff > 0x4000 || diff < -0x4000) {
        heading += 0x8000;
        diff += 0x8000;
    }
    if (CP->flags & 0x80) {
        if (abs(diff) > 0x3000 && abs(diff) < 0x5000) {
            CP->impact += CP->speed;
            return 0;
        }
        if (Ppd.heading != heading) {
            Ppd.heading = heading;
            if (heading & 0x2000) {
                tile = get_home_tile();
                if (tile >= 2 && tile <= 5) {
                    fy = 0;
                    fx = heading & 0x4000 ? 0 : 1;
                    if ((diff < 0) ^ ((heading & 0x8000) != 0)) {
                        fx ^= 1;
                        fy ^= 1;
                    }
                    MP.frac[0] = fx * 0x1F00;
                    MP.frac[1] = fy * 0x1F00;
                } else {
                    MP.frac[0] = 0x1000;
                    MP.frac[1] = 0x1000;
                }
            }
        } else {
            unsigned char gx, gy;

            gy = 0;
            gx = heading & 0x4000 ? 0 : 1;

            if ((diff > 0) ^ ((heading & 0x8000) != 0)) {
                gx ^= 1;
                gy ^= 1;
            }
            MP.frac[0] = gx * 0x1F00;
            MP.frac[1] = gy * 0x1F00;
        }
    } else if (abs(diff) > 0x3000 && abs(diff) < 0x5000)
        Ppd.heading = heading + diff;
    else {
        t = diff / 15;
        Ppd.heading = heading + t * CP->bounce;
    }
    if ((CP->flags & 0x80) == 0) {
        CP->impact += CP->speed * (15 - CP->bounce) / 15;
        CP->speed = CP->speed * CP->bounce / 15;
    }
    CP->heading = Ppd.heading;
    return 1;
}

void far do_2dbounce(char how)
{
    register int h;

    if (bounce_flag > 0)
        grid_move(-1);
    else {
        if (how) {
            ComputeHeading();
            h = Ppd.slope;
            if (h == 9)
                h = MP.major * 2;
        } else
            h = MP.major * 2;
        grid_move(-1);
        if (rehead(MP.headings[h])) {
            recalc_vecs(1);
            bounce_flag = 2;
            return;
        }
    }
    MP.done = MP.steps + 1;
}

/* IDA MaybeReflection: FM Towns bounce_that_guy_, same position and code. */
void far bounce_that_guy(void)
{
    CP->vel[2] = 0xBC;
    CP->acc[2] = -4;
    if (CP->speed < 0xEB)
        CP->speed = 0xEB;
    CP->terrain = 0x10;
    if (!(rand() & 3)) {
        CP->heading -= 0x3000;
        CP->heading += rand() % 0x6000;
    }
    hit_obj = 1;
}

void far do_zbounce(void)
{
    int dz;
    register int r;
    register int mass;

    hit_obj = 0;
    mass = ComObjData[OBJ_ITEM(Obj_IntTMem(Ppd.index))].mass;
    if (MP.zspeed > 4) {
        dz = abs(MP.pos[2] - MP.targz) * MP.dt;
        dz = (dz << 4) / (MP.zspeed / 4);
        CP->time -= dz;
    } else
        CP->time = 0;
    MP.pos[2] = MP.targz;
    MP.frac[2] = 0;
    if (MP.hit == -1 && (3 & Ppd.hits0) == 1 && Ppd.floor + Ppd.radius >= MP.pos[2]
        && CP->vel[2] <= 0) {
        int fx;

        stop_me();
        CP->terrain = 2;
        if (mass < 20)
            fx = 0x18;
        else if (mass < 100)
            fx = 0x19;
        else
            fx = 5;
        play_effect(fx, CP->x >> 5, CP->y >> 5, 0);
        return;
    }
    {
        int vol;

        vol = abs(CP->vel[2]) / 10 + (mass - 600) / 50 - 40;
        play_effect(0xF, CP->x >> 5, CP->y >> 5, vol);
    }
    r = do_objhit(MP.hit, Ppd.index);
    if (r & 0x18) {
        if (r & 0x10)
            stop_me();
        else
            MP.done = MP.steps + 1;
        return;
    }
    if (r & 4) {
        char up;
        int speed;
        struct Object far *obj;
        int item;

        up = CP->vel[2] <= 0;
        speed = CP->speed;
        if (CP->index == 1 && (motionbits & 0x20) == 0x20 || CP->bounce == 15)
            CP->vel[2] = -CP->vel[2];
        else {
            CP->vel[2] = -(CP->vel[2] / 15);
            CP->impact = abs(CP->vel[2] * (15 - CP->bounce));
            CP->vel[2] = CP->vel[2] * CP->bounce;
            if (CP->bounce)
                CP->speed -= (15 - CP->bounce) * CP->speed / 30;
            else
                CP->speed = 0;
        }
        if (CP->index != 1 && (TP->ignore & 0x1000) == 0x1000) {
            if (CP->time > 1)
                CP->time--;
        } else if (up && CP->vel[2] < 0x8D) {
            CP->vel[2] = 0;
            CP->acc[2] = 0;
            if (MP.hit != -1) {
                obj = Obj_IntTMem(oCollisions[MP.hit].link.f.index);
                item = OBJ_ITEM(obj);
                if (ComObjData[item].solid)
                    CP->terrain = 1;
                else if (OBJ_MAJOR(Obj_IntTMem(CP->index)) != MAJOR_CREATURE)
                    bounce_that_guy();
                else
                    CP->terrain = 1;
            } else if (Ppd.floor + Ppd.radius >= MP.pos[2])
                CP->terrain = 1 << (Ppd.hits0 & 3);
            else if (OBJ_MAJOR(Obj_IntTMem(CP->index)) != MAJOR_CREATURE)
                bounce_that_guy();
            else if (Ppd.hits1 & 0x10)
                CP->terrain = 2;
            else if (Ppd.hits1 & 0x20)
                CP->terrain = 4;
            else if (Ppd.hits1 & 0x40)
                CP->terrain = 8;
            else
                CP->terrain = 1;
            if (!hit_obj)
                CP->time = 0;
        }
        if ((Ppd.hits0 & 3) == 3 && Ppd.hits0 & 4 || Ppd.hits1 & 0x40 && !(Ppd.hits1 & 0x800))
            CP->speed = speed;
        recalc_vecs(0);
    } else {
        if (MP.hit != -1)
            oCollisions[MP.hit] = oCollisions[--Ppd.found];
        recalc_vecs(0);
    }
}

unsigned char far full_move(int crossed, int dir)
{
    long z;
    register int dz;

    if (MP.steps + (dir == -1) > MP.done) {
        if (dir * MP.step[2] > 0)
            z = MP.zspeed << 5;
        else
            z = -(MP.zspeed << 5);
    } else if (MP.rem != 0) {
        z = MP.rem;
        z = z * (dir * (MP.step[2] / 0x800) * MP.zspeed);
        z = z / 0x100;
    } else {
        z = 0x40;
        z = z * (dir * (MP.step[2] / 0x800) * MP.zspeed);
    }
    z += MP.frac[2];
    dz = z >= 0 ? z / 0x800 : -(labs(z) / 0x800 + 1);
    MP.frac[2] = z & 0x7FF;
    if (dir == -1)
        MP.pos[2] += dz;
    else if (dz > 0) {
        CP->terrain = 0x10;
        if (MP.pos[2] + dz <= MP.targz)
            MP.pos[2] += dz;
        else {
            do_zbounce();
            return 0;
        }
    } else if (dz < 0) {
        CP->terrain = 0x10;
        if (MP.pos[2] + dz >= MP.targz)
            MP.pos[2] += dz;
        else {
            do_zbounce();
            return 0;
        }
    }
    return flat_move(crossed, dir);
}

unsigned char far grid_move(int dir)
{
    char check;
    unsigned char r;
    register int state;

    check = 0;
    if (dir == -1) {
        ObjectCheck(0, 0);
        set_targz(0);
        CP->terrain = terr_type;
        if (hit_flag)
            check = 1;
    } else
        bounce_flag = bounce_flag - 1;
    if (CP->vel[2] != 0)
        r = full_move(0, dir);
    else
        r = flat_move(0, dir);
    if (check) {
        state = get_pcoll();
        terr_type = CP->terrain;
        CP->terrain = set_resterr(state);
    }
    return r;
}

unsigned char far set_resterr(unsigned bits)
{
    if (bits & 0x1000)
        return 0x10;
    if (bits & 4) {
        if (Ppd.index == 1 && (bits & 3) == 1 && (bits & 0xF8) != (bits & 0x90))
            return 0x20;
        return 1 << (bits & 3);
    }
    if (!(bits & 0x88)) {
        if (bits & 0x10)
            return 2;
        if (bits & 0x20)
            return 4;
        return 8;
    }
    return 1;
}

int far get_pcoll(void)
{
    register unsigned state = 0;
    register int i;
    int r;
    unsigned char ok = 0;
    unsigned char climb = !(TP->noclimb & 0x80);
    unsigned char step;
    unsigned char c6 = 0;

    hit_flag = 0;
    TerrainCheck(CP->b24);
    ObjectCheck(0, 0);
    set_targz(0);
    step = !((state = Ppd.hits0 | Ppd.hits1) & TP->noclimb);
    if (Ppd.found > 0) {
        for (i = Ppd.first; Ppd.first + Ppd.count > i; i++) {
            r = do_objhit(i, Ppd.index);
            if (r & 4)
                state |= 0x400;
            if (r & 0x18) {
                if (r & 0x10)
                    state |= 0x4000;
                else
                    state = state | 0x8000;
                return state;
            }
        }
    }
    if (MP.pos[2] != MP.targz) {
        if (MP.hit == -1 && step) {
            ok = abs(MP.pos[2] - MP.targz) <= CP->b24;
            ok = ok || CP->vel[2] == 0 && !(Ppd.hits1 & 0x800) && Ppd.hits0 & 4;
        } else if (climb) {
            ok = ComObjData[MP.item].solid == 1;
            if (ok) {
                ok = abs(MP.pos[2] - oCollisions[MP.hit].top) <= CP->b24;
                if (ok)
                    state |= 0x80;
            }
        }
        ok = ok && targ_ceil && !c6;
        if (ok && MP.targz >= MP.f23)
            state &= ~0x100;
        else if (!ok && Ppd.top > MP.pos[2])
            state |= 0x100;
        if (ok && MP.targz + CP->height > 0x7F) {
            ok = 0;
            hit_flag = 1;
            state |= 0x200;
        } else if (ok && MP.hit == -1 && MP.targz + CP->height > MP.f23) {
            ok = 0;
            hit_flag = 1;
            state |= 0x400;
        }
        if (!(ok && state & 0x400) && ok && MP.hit != -1
            && (!ComObjData[MP.item].solid || !climb))
            ok = 0;
        if (ok) {
            hit_flag = 1;
            MP.pos[2] = MP.targz;
            if (abs(MP.targz - Ppd.floor) <= CP->radius)
                state |= 4;
            else
                state &= ~4;
        }
    } else if (MP.hit != -1 && ComObjData[MP.item].solid == 1 && climb) {
        state |= 0x80;
        state &= ~4;
    }
    if (hit_flag && ok) {
        process_objlist();
        state &= ~0x400;
        for (i = 0; i < Ppd.count; i++) {
            r = do_objhit(i, Ppd.index);
            if (r & 4)
                state |= 0x400;
        }
    }
    if (state & 0x80 && climb && (oCollisions[MP.hit].link.f.low & 0x10 || Ppd.hits0 & 4))
        state &= ~0x800;
    if (Ppd.hits1 & 0x100 && step && CP->vel[2] == 0 && MP.pos[2] + CP->b24 >= Ppd.top) {
        state &= ~0x100;
        MP.pos[2] = Ppd.top;
        if (MP.pos[2] == Ppd.floor)
            state |= 4;
        else
            state &= ~4;
    }
    if (!(state & 0xFC))
        state |= 0x1000;
    if (!(state & 0x80) && MP.pos[2] - CP->radius > Ppd.top)
        state |= 0x1000;
    return state;
}

void far check_positions(void)
{
    unsigned state;
    unsigned char bounce2d = 0;

    state = get_pcoll();
    terr_type = CP->terrain;
    CP->terrain = set_resterr(state);
    if (state & 0xC000) {
        grid_move(-1);
        if (state & 0x4000)
            stop_me();
        else
            MP.done = MP.steps + 1;
        return;
    }
    state = state & ~TP->ignore;
    if (state != 0) {
        if (state & TP->mask && TP->special(&state)) {
            grid_move(-1);
            MP.done = MP.steps + 1;
            return;
        }
        if (state & 0x700) {
            do_2dbounce(!(state & 0x400));
            bounce2d = 1;
        }
        if (state & 0x1000 && CP->acc[2] == 0) {
            CP->acc[2] = -4;
            recalc_vecs(bounce2d);
        }
    }
}

/* IDA JumpTrap: FM Towns set_jmp_. */
void far set_jmp(int force, char stop, int min)
{
    if (abs(CP->vel[0]) + abs(CP->vel[1]) >= min * 0x2F) {
        if (CP->acc[2] != -4)
            CP->acc[2] = -2;
        if (CP->index == 1)
            CP->vel[2] = force * 0x2F / 2;
        else
            CP->vel[2] = force * 0x8D / 4;
        if (stop)
            CP->vel[0] = CP->vel[1] = 0;
        else {
            CP->vel[0] = CP->vel[0] / 2;
            CP->vel[1] = CP->vel[1] / 2;
        }
    }
}

/* IDA FindClosedDoorCollision: FM Towns IsaDoor_. */
struct Object far * far IsaDoor(unsigned char *x, unsigned char *y)
{
    register int i;
    int item;
    int t;

    for (i = 0; i < Ppd.count; i++) {
        item = OBJ_ITEM(Obj_PtrTMem(&oCollisions[i + Ppd.first].link));
        t = oCollisions[i + Ppd.first].offset & 0x3F;
        *x = (Ppd.x >> 3) + t & 0x3F;
        t = *x - (Ppd.x >> 3);
        *y = (Ppd.y >> 3) + (oCollisions[i + Ppd.first].offset - t) / MAP_SIZE & 0x3F;
        if (item >> 4 == CLASS_DOOR && (item & ID_INCLASS) < 8)
            return Obj_PtrTMem(&oCollisions[i + Ppd.first].link);
    }
    return 0;
}

/* IDA GetCollisionObject: FM Towns CollObject_. */
struct Object far * far CollObject(void)
{
    if (Ppd.count)
        return Obj_PtrTMem(&oCollisions[Ppd.first].link);
    return 0;
}
