/* target: seg007_17A2 */
/* opts: -mm -1 -G -O -Y -d */
/* Critter movement and AI: the animation sequences, the goals a critter pursues (wander,
   mill about, guard, attack, flee, defend, talk, hover), choosing and finding targets,
   turning, the per-tick setup of the critter globals, critter_ai and critter_mv, death
   and damage, and the loop over the active mobile objects. The whole of DOS resident
   segment seg007_17A2, in original order. Function and global names are the originals
   from the FM Towns symbol table where it has them; the source file's own name is not
   known. */

#include <dos.h>
#include <stdlib.h>
#include "combat.h"
#include "conv.h"
#include "critter.h"
#include "map.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"
#include "view3d.h"

#define OBJ_ITEM(o)      ((o)->id & 0x1FF)
#define OBJ_MAJOR(o)     (((o)->id & 0x1C0) >> 6)
#define OBJ_MINOR(o)     (((o)->id & 0x30) >> 4)
#define OBJ_INDEX(o)     (((o)->id & 0x3F) >> 0)
#define CRIT_INDEX(o)    ((o)->id & 0x3F)
#define FINEHEAD(o)      ((o)->b18 & 0x1F)
#define SPEED(o)         ((o)->b13 & 0x7F)
#define BIN(o)           ((o)->b0A & 0xF)
#define RATE(o)          ((o)->b14 & 7)
#define PITCH(o)         (((o)->b14 & 0xF8) >> 3)
#define CAST(o)          (((o)->b19 & 0xC) >> 2)
#define B19_4(o)         (((o)->b19 & 0x10) >> 4)
#define B19_6(o)         (((o)->b19 & 0x40) >> 6)
#define B0A_7(o)         (((o)->b0A & 0x80) >> 7)
#define DAMAGE(o)        (((o)->b11 & 0xFF) >> 0)
#define ID_B13(o)        (((o)->id & 0x2000) >> 13)
#define OBJ_Z(o)         ((o)->pos & 0x7F)
#define OBJ_HEADING(o)   (((o)->pos & 0x380) >> 7)
#define OBJ_FINEY(o)     (((o)->pos & 0x1C00) >> 10)
#define OBJ_FINEX(o)     (((o)->pos & 0xE000) >> 13)
#define OBJ_HOMEX(o)     (((o)->home & 0xFC00) >> 10)
#define OBJ_HOMEY(o)     (((o)->home & 0x3F0) >> 4)
#define OBJ_PATH(o)      ((o)->home & 0xF)
#define GOAL(o)          (((o)->goal_word & 0xF) >> 0)
#define GTARG(o)         (((o)->goal_word & 0xFF0) >> 4)
#define FRAME(o)         (((o)->goal_word & 0xF000) >> 12)
#define OLDGOAL(o)       (((o)->attitude_word & 0xF) >> 0)
#define ATTITUDE(o)      (((o)->attitude_word & 0xC000) >> 14)
#define ATKFRAME(o)      (((o)->b0F & 0xF000) >> 12)
#define SEQ(o)           ((o)->b15 & 0x3F)
#define B15_6(o)         (((o)->b15 & 0x40) >> 6)
#define B15_7(o)         (((o)->b15 & 0x80) >> 7)
#define B19_0(o)         (((o)->b19 & 1) >> 0)
#define B19_1(o)         (((o)->b19 & 2) >> 1)
#define B19_5(o)         (((o)->b19 & 0x20) >> 5)
#define B18_6(o)         (((o)->b18 & 0x40) >> 6)
#define DESTX(o)         (((o)->b0F & 0x3F) >> 0)
#define DESTY(o)         (((o)->b0F & 0xFC0) >> 6)

#define SET_HEADING(o, v)  ((o)->pos = (o)->pos & 0xFC7F | ((v) & 7) << 7)
#define SET_BIN(o, v)      ((o)->b0A = (o)->b0A & 0xF0 | ((v) & 0xF) << 0)
#define SET_DAMAGE(o, v)   ((o)->b11 = (o)->b11 & 0 | ((v) & 0xFF) << 0)
#define SET_GOAL(o, v)     ((o)->goal_word = (o)->goal_word & 0xFFF0 | ((v) & 0xF) << 0)
#define SET_GTARG(o, v)    ((o)->goal_word = (o)->goal_word & 0xF00F | ((v) & 0xFF) << 4)
#define SET_FRAME(o, v)    ((o)->goal_word = (o)->goal_word & 0xFFF | ((v) & 0xF) << 12)
#define SET_OLDGOAL(o, v)  ((o)->attitude_word = (o)->attitude_word & 0xFFF0 | ((v) & 0xF) << 0)
#define SET_ATTITUDE(o, v) ((o)->attitude_word = (o)->attitude_word & 0x3FFF | ((v) & 3) << 14)
#define SET_ATKFRAME(o, v) ((o)->b0F = (o)->b0F & 0xFFF | ((v) & 0xF) << 12)
#define SET_SPEED(o, v)    ((o)->b13 = (o)->b13 & 0x80 | ((v) & 0x7F) << 0)
#define SET_RATE(o, v)     ((o)->b14 = (o)->b14 & 0xF8 | (v))
#define SET_PITCH(o, v)    ((o)->b14 = (o)->b14 & 7 | ((v) & 0x1F) << 3)
#define SET_SEQ(o, v)      ((o)->b15 = (o)->b15 & 0xC0 | ((v) & 0x3F) << 0)
#define SET_B15_6(o, v)    ((o)->b15 = (o)->b15 & 0xBF | (v) << 6)
#define SET_B15_7(o, v)    ((o)->b15 = (o)->b15 & 0x7F | (v) << 7)
#define SET_FINEHEAD(o, v) ((o)->b18 = (o)->b18 & 0xE0 | ((v) & 0x1F) << 0)
#define SET_B19_0(o, v)    ((o)->b19 = (o)->b19 & 0xFE | (v) << 0)
#define SET_B19_1(o, v)    ((o)->b19 = (o)->b19 & 0xFD | ((v) & 1) << 1)
#define SET_B19_4(o, v)    ((o)->b19 = (o)->b19 & 0xEF | (v) << 4)
#define SET_B19_5(o, v)    ((o)->b19 = (o)->b19 & 0xDF | (v) << 5)
#define SET_B18_5(o, v)    ((o)->b18 = (o)->b18 & 0xDF | (v) << 5)
#define SET_CAST(o, v)     ((o)->b19 = (o)->b19 & 0xF3 | ((v) & 3) << 2)

extern struct Object far *objdata;
extern unsigned char far *ActiveMob;
extern unsigned char far *LastActiveMob;
extern unsigned long far *Time;
extern int freepaths;
int lastXeye, lastYeye;                 /* DS:2294, this file's _BSS (see below) */
extern int missile_try;
long lastcombattime;                    /* DS:2280, this file's _BSS (see below) */

extern struct MissileInfo Missile[];
/* The charge of a critter's blow, by attack frame. Static in FM Towns: provisional name.
   A far variable, so its own segment (SEG0075_FAR, 609E:0000, segment table entry 69,
   right after SEG006's four): only this file uses it. */
struct AtkCharge {
    unsigned char charge;
    char b1;
};
static struct AtkCharge far atk_charge[16] = {
    { 50, 0 }, { 60, 0 }, { 70, 0 }, { 80, 0 }, { 90, 0 }, { 100, 0 }, { 110, 0 }, { 120, 0 },
    { 130, 0 }, { 140, 0 }, { 155, 0 }, { 170, 0 }, { 185, 0 }, { 205, 0 }, { 230, 0 }, { 255, 0 } };

/* Uninitialised data, DS:222C..2299. This block is one module's _BSS (Turbo C lays a
   module's _BSS out by name), and variables only seg006 uses (DS:222E, 2236, 2238, 2240,
   2244, 225A, 2272, 227E) sit between the ones below, so it belongs to seg006 or to this
   file and cannot be split. Until seg006 is matched it is declared here as extern; the
   names are the FM Towns ones. */

/* The current critter, set up by set_critter_vars and critter_ai. */
extern unsigned myxpost, myypost;
/* This file's _BSS, DS:2280..2299, laid out by name (tools/bssorder.py): lastcombattime 84,
   crithittime 139, hitx and hity 520, hitpz 552, curBin 555, victim 574, seq_len 603,
   seqptr 659, lastXeye and lastYeye 820, seq_lframe 859 (Turbo C puts anything wider than
   a byte on an even offset, so DS:228F and DS:2299 are padding).
   The critter type last damaged, and where and when the player last hit a critter.
   hitpz (DS:228A) is static in FM Towns (_seq_lframe+1 there, read by critter_mv_ for
   set_loc beside _hitx and _hity), so it has no original name: provisional, chosen for its
   key. FM Towns' _hitz is another variable, seg024's combat height (DS:24CE). */
unsigned long crithittime;
unsigned char hitx, hity;
static unsigned char hitpz;
signed char curBin;
struct Creature near *victim;
unsigned char seq_len;
unsigned char seq_lframe;

/* An animation sequence, 64 bytes, in the critter animation pages. */
struct Seq {
    char pad0[7];
    unsigned char len;                  /* 0x07 */
    char pad8[0x40 - 8];
};
struct Seq far *seqptr;

struct Object far * far Obj_IntTMem(int index);
void far set_loc(unsigned char x, unsigned char y, char z);
unsigned char far anti_magic_p(int x, int y);
void far do_crit_phys(struct Phys *pn, struct Handler *tp);
char far death_check(struct Object far *obj, char how);
unsigned char far Obj_Rem(unsigned far *head, struct Object far *obj);
void far player_killed_a(struct Object far *npc);
void far play_effect(char type, int x, int y, char vol);
void far set_new_music(int n);
unsigned char far line_of_sight(int x1, int y1, int z1, int x2, int y2, int z2);

void far critter_set_goal(unsigned char goal, int target);

/* Initialised data, DS:C4: the time bin of the last pass over the mobile objects, and the
   critter the player last hit and its type. */
signed char lastbin = 0;
unsigned char crithit = 0;
unsigned char typehit = 0xFF;

void far change_or_inc_seq(int seq, char force)
{
    if (SEQ(meptr) != seq) {
        SET_SEQ(meptr, seq);
        SET_FRAME(meptr, 0);
    } else if (force || rand() % 2 == 0)
        SET_FRAME(meptr, (FRAME(meptr) + 1) % seq_len);
}

void far set_htx(int heading)
{
    meptr->heading = heading << 5;
    SET_HEADING(meptr, heading);
    SET_FINEHEAD(meptr, 0);
}

void far crit_drunkwalk(void)
{
    unsigned char head;
    unsigned char seq;
    unsigned char r;
    struct Tile far *tile;
    int pitch;
    int range;

    seq = 1;
    if (B15_7(meptr)) {
        freepaths |= 1 << OBJ_PATH(meptr);
        SET_B15_7(meptr, 0);
    }
    tile = Map_GetAddr(myxpos, myypos);
    if (!control) {
        SET_RATE(meptr, 1);
        return;
    }
    if (ATTITUDE(meptr) == 0 && rand() % 2) {
        crit_guard();
        return;
    }
    if (mycst->flier) {
        pitch = 0xE;
        range = 3;
        if (myzpos <= 0xE) {
            if (myzpos < tile->height + 2)
                pitch = 0x10;
            else
                range = 5;
        }
        SET_PITCH(meptr, pitch + rand() % range);
    }
    if (SEQ(meptr) == 0) {
        r = rand() % 16;
        if (mycst->lazy <= r || FRAME(meptr) != seq_lframe)
            seq = 0xFF;
    } else {
        r = rand() % 16;
        if (mycst->lazy < r && FRAME(meptr) == seq_lframe)
            seq = 0;
    }
    if (SEQ(meptr) != seq && seq != 0xFF) {
        SET_SEQ(meptr, seq);
        set_cur_seq_len();
        if (FRAME(meptr) > seq_lframe)
            SET_FRAME(meptr, seq_lframe);
    }
    if (SEQ(meptr) == 1) {
        if (failed && !aligned) {
            head = (meptr->heading + (rand() % 2 * 2 - 1) * 0x40 + 0x100) % 0x100;
            meptr->heading = head;
            SET_HEADING(meptr, head >> 5);
            SET_FINEHEAD(meptr, head);
            SET_SPEED(meptr, 0);
            return;
        }
        r = rand() % 0x40;
        if (r < mycst->lazy + 8)
            head = (meptr->heading + rand() % 0x40 + 0xE0) % 0x100;
        else
            head = meptr->heading;
        if (!aligned)
            head = crit_avoid_player(head, 10);
    } else {
        r = rand() % 0x80;
        if (mycst->lazy > r)
            head = (meptr->heading + rand() % 0x40 + 0xE0) % 0x100;
    }
    meptr->heading = head;
    SET_HEADING(meptr, head >> 5);
    SET_FINEHEAD(meptr, head);
    if (SEQ(meptr) == 0) {
        SET_B15_6(meptr, 1);
        SET_SPEED(meptr, 0);
        SET_RATE(meptr, 6);
        if (rand() % 2)
            SET_FRAME(meptr, (FRAME(meptr) + 1) % seq_len);
    } else {
        SET_B15_6(meptr, 0);
        SET_SPEED(meptr, mycst->speed);
        SET_RATE(meptr, 4);
        SET_FRAME(meptr, (FRAME(meptr) + 1) % seq_len);
    }
    check_out_player();
}

void far crit_mill(void)
{
    signed char dx;
    signed char dy;

    if (!control)
        return;
    if (ATTITUDE(meptr) == 0 && GOAL(meptr) != 4) {
        critter_set_goal(4, 1);
        return;
    }
    dx = myxhome - myxpos;
    dy = myyhome - myypos;
    if (dx * dx + dy * dy > mycst->range * mycst->range)
        crit_head_for_loc(myxhome, myyhome, Map_GetAddr(myxhome, myyhome)->height);
    else
        crit_drunkwalk();
}

void far crit_guard(void)
{
    unsigned char found;
    unsigned char x;
    unsigned char y;

    if (!control)
        return;
    switch (ATTITUDE(meptr)) {
    case 0:
        SET_GTARG(meptr, 1);
        set_up_target();
        if (B19_0(meptr)) {
            critter_set_goal(5, 1);
            return;
        }
        if (B19_1(meptr)) {
            if (rand() % 16 > mycst->alert)
                SET_B19_1(meptr, 0);
            else
                look_for_target(0);
        }
        if (rand() % 16 < mycst->alert) {
            found = target_found(&x, &y);
            switch (found) {
            case 0:
                SET_B19_0(meptr, 1);
                set_loc(x, y, tzpos);
                critter_set_goal(5, 1);
                return;
            case 2:
                SET_B19_1(meptr, 1);
                if (rand() % 2 == 0) {
                    crit_head_for_loc(x, y, tzpos);
                    return;
                }
                break;
            }
        }
    }
    switch (GOAL(meptr)) {
    case 2:
        crit_drunkwalk();
        break;
    case 0:
    case 7:
        SET_RATE(meptr, 6);
        SET_SPEED(meptr, 0);
        change_or_inc_seq(0, 0);
        break;
    default:
        crit_mill();
    }
}

void far crit_offense(void)
{
    signed char dx;
    signed char dy;
    unsigned char attacked = 0;
    unsigned char how = 4;
    unsigned dist;
    unsigned homedist;

    if (!control)
        return;
    dist = tdx * tdx + tdy * tdy;
    dx = myxhome - myxpos;
    dy = myyhome - myypos;
    homedist = dx * dx + dy * dy;
    if (GTARG(meptr) == 1)
        SET_ATTITUDE(meptr, 0);
    if ((dist < 0x64 || myxpos == txpos && myypos == typos)
        && (abs((signed char)myzpos - tzpos) < 4 || mycst->flier)) {
        crit_attack(dist);
        return;
    }
    if (mycst->caster > 0) {
        if (!maybe_cast_defensive_spell() && mycst->b2D_0)
            attacked = crit_magik_attack();
    } else if (mycst->arms[0].item >> 4 == 1)
        attacked = crit_missile_attack();
    if (attacked) {
        if (SEQ(meptr) == 6 || SEQ(meptr) == 6 || SEQ(meptr) == 3)
            return;
        change_or_inc_seq(2, 1);
        SET_RATE(meptr, 4);
        SET_SPEED(meptr, 0);
        return;
    }
    if (dist > 0x100 && OLDGOAL(meptr) == 4 && !B19_5(meptr)
        && mycst->range * mycst->range * 4 < homedist) {
        SET_B19_0(meptr, 0);
        SET_B19_1(meptr, 0);
        critter_set_goal(4, 0);
        return;
    }
    crit_offense_find_target(txpos, typos, mycst->b2D_0 ? how : 1);
}

unsigned char far crit_attack(unsigned dist)
{
    signed char dz;
    unsigned char head;
    int seq;
    int speed;
    int i;
    int r;

    head = deltatotheta(tdx, tdy);
    set_htx(head);
    SET_B15_6(meptr, 0);
    seq = 0x20;
    if (dist < 0x31) {
        if (rand() % 4 == 0) {
            head = (head + (rand() % 2 * 2 - 1) * 2 + 8) % 8;
            seq = 2;
            speed = mycst->speed * 2 / 3;
        } else {
            head = (head + 4) % 8;
            seq = 1;
            speed = 2;
        }
    } else if (dist > 0x51) {
        seq = 1;
        speed = 2;
    } else if (rand() % 0x40 < mycst->attr[1]) {
        head = rand() % 8;
        seq = 2;
        speed = 1;
    } else {
        seq = 2;
        speed = 0;
    }
    meptr->heading = head << 5;
    SET_SPEED(meptr, speed);
    if (SEQ(meptr) != seq) {
        SET_SEQ(meptr, seq);
        SET_FRAME(meptr, 0);
    } else
        SET_FRAME(meptr, (FRAME(meptr) + 1) % seq_len);
    if (mycst->flier) {
        dz = OBJ_Z(mytarget) + 0xE - OBJ_Z(meptr);
        if (dz > 1)
            SET_PITCH(meptr, 0x12);
        else if (dz < -1)
            SET_PITCH(meptr, 0xE);
        else
            SET_PITCH(meptr, rand() % 3 + 0xF);
    }
    if (dist <= 0x64) {
        if (rand() % 4 == 0) {
            r = rand() % 100;
            for (i = 0; mycst->attacks[i].prob <= r && i < 2; i++)
                r -= mycst->attacks[i].prob;
            SET_SEQ(meptr, i + 3);
            SET_FRAME(meptr, 0);
        } else if (ATKFRAME(meptr) < 0xF)
            SET_ATKFRAME(meptr, ATKFRAME(meptr) + 1);
    }
    SET_RATE(meptr, 4);
    return 1;
}

void far crit_offense_find_target(unsigned char x, unsigned char y, unsigned char how)
{
    unsigned char found;

    if ((DESTX(meptr) != x || DESTY(meptr) != y) && rand() % 8 == 0) {
        found = target_found(&x, &y);
        if (found == 0 || found == 2 && rand() % 2 == 0)
            set_loc(x, y, tzpos);
        else {
            SET_B19_0(meptr, 0);
            SET_B19_1(meptr, found == 1);
            critter_discard_goal();
        }
    }
    if (x == myxpos && y == myypos && abs((signed char)myzpos - tzpos) < 4)
        return;
    if (how > 1 && how * how < tdistsqr || how * how * 8 * 8 < tdisttsqr
        || how <= 1 && abs((signed char)myzpos - tzpos) >= 4) {
        crit_head_for_loc(x, y, tzpos);
        if (B18_6(meptr)) {
            critter_discard_goal();
            SET_B19_1(meptr, 0);
        }
    }
}

unsigned char far maybe_cast_defensive_spell(void)
{
    if (mycst->spells[2] != 0xFF && rand() % 0x100 < mycst->caster
        && !anti_magic_p(myxpos, myypos)) {
        SET_SPEED(meptr, 0);
        SET_SEQ(meptr, 6);
        SET_FRAME(meptr, 0);
        SET_CAST(meptr, 3);
        return 1;
    }
    return 0;
}

unsigned char far crit_magik_attack(void)
{
    int r;

    if (anti_magic_p(myxpos, myypos) == 0 && tdistsqr < 0x40 && !anti_magic_p(myxpos, myypos)
        && line_of_sight(myxpost, myypost, OBJ_Z(meptr) + ComObjData[OBJ_ITEM(meptr)].height,
                         txpost, typost, OBJ_Z(mytarget) + ComObjData[OBJ_ITEM(mytarget)].height)
        && look_for_target(1)) {
        r = rand() % 0x80;
        if (mycst->caster > r) {
            SET_SPEED(meptr, 0);
            SET_SEQ(meptr, 6);
            SET_FRAME(meptr, 0);
            SET_CAST(meptr, rand() % 16 < 0xB ? 1 : 2);
        }
        return 1;
    }
    return 0;
}

unsigned char far crit_missile_attack(void)
{
    if (tdistsqr < 0x10
        && line_of_sight(myxpost, myypost, OBJ_Z(meptr) + ComObjData[OBJ_ITEM(meptr)].height,
                         txpost, typost, OBJ_Z(mytarget) + ComObjData[OBJ_ITEM(mytarget)].height)
        && look_for_target(1)) {
        if (rand() % 0xC0 <= mycst->attr[1]) {
            SET_SPEED(meptr, 0);
            SET_SEQ(meptr, 6);
            SET_FRAME(meptr, 0);
        }
        return 1;
    }
    return 0;
}

void far crit_defense(void)
{
    unsigned char head;
    unsigned dist;

    if (!control)
        return;
    dist = tdx * tdx + tdy * tdy;
    if (dist < 0x90 || txpos == myxpos && myypos == typos) {
        crit_attack(dist);
        return;
    }
    if (tdistsqr > 4) {
        if (maybe_cast_defensive_spell())
            return;
        if (mycst->caster > 0)
            crit_magik_attack();
        else if (mycst->arms[0].item >> 4 == 1)
            crit_missile_attack();
        else
            crit_flee();
    } else {
        head = deltatotheta(tdx, tdy);
        SET_SPEED(meptr, 0);
        set_htx(head);
        SET_RATE(meptr, 4);
        change_or_inc_seq(2, 1);
    }
}

void far crit_flee(void)
{
    unsigned char head;
    unsigned char r;
    unsigned char newh;
    signed char dz;
    int pitch;

    if (!control)
        return;
    head = deltatotheta(tdx, tdy);
    dz = OBJ_Z(mytarget) - OBJ_Z(meptr);
    if (mycst->flier) {
        pitch = 0;
        if (OBJ_Z(meptr) <= 0x6E)
            pitch = 2;
        SET_PITCH(meptr, pitch + rand() % 5 + 0xD);
    }
    if (tdistsqr <= 3 && abs(dz) < 0x10) {
        if (rand() % 0x100 < mycst->b1C_0 >> 3 || failed && !aligned) {
            SET_B19_4(meptr, 1);
            critter_set_goal(9, GTARG(meptr));
            return;
        }
        meptr->heading = (head + 4) % 8 << 5;
        SET_HEADING(meptr, head);
        SET_FINEHEAD(meptr, 0);
        change_or_inc_seq(1, 1);
        SET_SPEED(meptr, (mycst->speed + 1) / 2);
        return;
    }
    if (failed && !aligned) {
        if (tdistsqr < 9) {
            if (GOAL(meptr) == 9) {
                SET_SPEED(meptr, 0);
                set_htx(head);
                SET_RATE(meptr, 4);
                change_or_inc_seq(2, 1);
                return;
            }
            SET_B19_4(meptr, 1);
            critter_set_goal(9, GTARG(meptr));
            return;
        }
        newh = (((meptr->heading >> 5) + (rand() % 2 * 2 - 1) * 2 + 8) % 8 << 5) + rand() % 0x20;
    } else {
        if (maybe_cast_defensive_spell())
            return;
        r = rand() % 0x40;
        if (r < mycst->lazy + 8)
            newh = (meptr->heading + rand() % 0x40 + 0xE0) % 0x100;
        else
            newh = meptr->heading;
        if (!aligned)
            newh = crit_avoid_player(newh, 0x18);
        /* The comparison survives, but its if has no body. */
        if (meptr->heading != newh)
            ;
    }
    meptr->heading = newh;
    SET_HEADING(meptr, newh >> 5);
    SET_FINEHEAD(meptr, newh);
    if (tdistsqr < 0x40)
        SET_SPEED(meptr, mycst->run);
    else
        SET_SPEED(meptr, mycst->speed);
    change_or_inc_seq(1, 1);
    SET_RATE(meptr, 4);
}

unsigned char far crit_avoid_player(unsigned char heading, int how)
{
    struct Object far *plyr;
    int px;
    int dx;
    int dy;
    unsigned char th;
    unsigned char newh;
    unsigned char diff;
    register int py;
    register unsigned dist;

    plyr = Obj_IntTMem(1);
    px = (OBJ_HOMEX(plyr) << 3) + OBJ_FINEX(plyr);
    py = (OBJ_HOMEY(plyr) << 3) + OBJ_FINEY(plyr);
    dx = px - myxpost;
    dy = py - myypost;
    dist = dx * dx + dy * dy;
    if (how * how > dist) {
        th = (deltatotheta(dx, dy) + 4) % 8 << 5;
        diff = (th + 0x100 - heading) % 0x100;
        if (diff < 0x40 || diff > 0xC0)
            newh = heading;
        else if (diff < 0x60)
            newh = (th + 0xE0) % 0x100;
        else if (diff < 0x80)
            newh = (heading + 0x20) % 0x100;
        else if (diff > 0xA0)
            newh = (th + 0x20) % 0x100;
        else
            newh = (heading + 0xE0) % 0x100;
        return newh;
    }
    return heading;
}

void far crit_talk(void)
{
    unsigned char head;
    signed char rel;
    unsigned char found;
    unsigned char x;
    unsigned char y;
    unsigned dist;

    if (!control)
        return;
    if (ATTITUDE(meptr) == 0 && GOAL(meptr) != 4) {
        critter_set_goal(4, 1);
        return;
    }
    SET_GTARG(meptr, 1);
    set_up_target();
    dist = tdx * tdx + tdy * tdy;
    found = target_found(&x, &y);
    SET_RATE(meptr, 6);
    SET_SPEED(meptr, 0);
    change_or_inc_seq(0, 0);
    if (found == 1)
        return;
    if (dist < 0x190) {
        head = deltatotheta(tdx, tdy);
        set_htx(head);
        if (dist < 0x90) {
            rel = OBJ_HEADING(ThePlayer) + 8 - head & 7;
            if (rel >= 3 && rel <= 5) {
                pmouseHandled = 0;
                mouse_freereign();
                TalkTo(meptr);
                map_crit_pages();
                seqptr = MK_FP(EmsBuff + 0xC00, 0);
            }
        }
    }
}

void far check_out_player(void)
{
    unsigned char head;
    unsigned dist;

    if ((meptr->b13 & 0x7F) <= 0 || player->drawn) {
        SET_GTARG(meptr, 1);
        set_up_target();
        dist = tdx * tdx + tdy * tdy;
        if (dist < 0x90) {
            head = deltatotheta(tdx, tdy);
            SET_SPEED(meptr, 0);
            SET_RATE(meptr, 6);
            change_or_inc_seq(0, 0);
            SET_HEADING(meptr, head);
            SET_FINEHEAD(meptr, 0);
        }
    }
}

void far crit_hover(void)
{
    signed char dx;
    signed char dy;

    if (!control)
        return;
    dx = myxhome - myxpos;
    dy = myyhome - myypos;
    if (ATTITUDE(meptr) == 0 && GOAL(meptr) != 4)
        critter_set_goal(4, 1);
    else if (dx != 0 || dy != 0)
        crit_head_for_loc(myxhome, myyhome, Map_GetAddr(myxhome, myyhome)->height);
    else {
        SET_RATE(meptr, 6);
        SET_SPEED(meptr, 0);
        change_or_inc_seq(0, 0);
    }
}

unsigned char far target_found(unsigned char *x, unsigned char *y)
{
    signed char dx;
    signed char dy;
    signed char th;
    signed char myh;
    signed char rel;
    int seen;
    register int dist;
    register int heard;

    *x = txpos;
    *y = typos;
    dx = txpos - myxpos;
    dy = typos - myypos;
    dist = dx * dx + dy * dy;
    heard = mycst->hearing * Creature[CRIT_INDEX(mytarget)].noise / 16
          * (mycst->hearing * Creature[CRIT_INDEX(mytarget)].noise / 16);
    if (heard / 4 > dist)
        return 0;
    seen = mycst->sight * Creature[CRIT_INDEX(mytarget)].visibility / 16
         * (mycst->sight * Creature[CRIT_INDEX(mytarget)].visibility / 16);
    if (dist <= seen) {
        th = deltatotheta(dx, dy);
        myh = OBJ_HEADING(meptr);
        rel = (th - myh + 8) % 8;
        if ((rel == 0 || rel == 1 || rel == 7)
            && line_of_sight(myxpost, myypost, OBJ_Z(meptr) + ComObjData[OBJ_ITEM(meptr)].height,
                             txpost, typost, OBJ_Z(mytarget) + ComObjData[OBJ_ITEM(mytarget)].height)) {
            SET_B19_0(meptr, 1);
            return 0;
        }
    }
    if (heard * 4 > dist)
        return 2;
    SET_B19_0(meptr, 0);
    return 1;
}

unsigned char far look_for_target(char how)
{
    signed char dx;
    signed char dy;
    signed char th;
    signed char myh;
    signed char rel;

    dx = txpost - myxpost;
    dy = typost - myypost;
    th = deltatotheta(dx, dy);
    myh = OBJ_HEADING(meptr);
    rel = (th - myh + 8) % 8;
    if (how)
        return turn_real_fine(dx, dy);
    if (rel == 0)
        return 1;
    if (rel <= 4)
        SET_HEADING(meptr, OBJ_HEADING(meptr) + 1);
    else
        SET_HEADING(meptr, OBJ_HEADING(meptr) - 1);
    return 0;
}

/* IDA's NPC_Move. Named from FM Towns: constrain_movement_ follows look_for_target_ there
   and makes the same limits of the new facing and heading to 0x20 either side of
   myoldfacing and myoldheading; critter_mv calls it last, as here. */
void far constrain_movement(void)
{
    unsigned char oldh;
    unsigned char newf;
    register unsigned char diff;

    oldh = meptr->heading;
    newf = (OBJ_HEADING(meptr) << 5) + FINEHEAD(meptr);
    diff = (newf + 0x100 - myoldfacing) % 0x100;
    if (diff >= 0x20 && diff <= 0xE0) {
        if (diff < 0x80)
            newf = (myoldfacing + 0x20) % 0x100;
        else
            newf = (myoldfacing + 0xE0) % 0x100;
    }
    SET_HEADING(meptr, newf >> 5);
    SET_FINEHEAD(meptr, newf);
    if (aligned)
        meptr->heading = myoldheading;
    else if (myoldspeed > 1 && SPEED(meptr) > 1) {
        diff = (oldh + 0x100 - myoldheading) % 0x100;
        if (diff < 0x20 || diff > 0xE0)
            meptr->heading = oldh;
        else if (diff < 0x40)
            meptr->heading = (myoldheading + 0x20) % 0x100;
        else if (diff > 0xC0)
            meptr->heading = (myoldheading + 0xE0) % 0x100;
        else {
            SET_SPEED(meptr, 0);
            meptr->heading = myoldheading;
        }
    }
}

/* IDA's TurnTowardsVector. Named from FM Towns: turn_real_fine_ comes next there, is
   called by look_for_target_, and makes the same cSqRt of tdx and tdy, cAtan2 and turn of
   0x20 at most. */
unsigned char far turn_real_fine(signed char dx, signed char dy)
{
    long d2;
    unsigned ang;
    unsigned char cur;
    unsigned char want;
    unsigned char diff;
    unsigned char newf;
    int sy;
    long ly;
    long lx;
    unsigned char done = 0;
    register unsigned dist;
    register int sx;

    cur = (OBJ_HEADING(meptr) << 5) + FINEHEAD(meptr);
    d2 = tdx * tdx + tdy * tdy;
    dist = cSqRt(d2);
    ly = dx;
    lx = dy;
    if (dist == 0)
        return 1;
    if (lx == dist)
        sy = 0x7FFF;
    else if (-lx == dist)
        sy = 0x8000;
    else
        sy = (lx << 15) / dist;
    if (ly == dist)
        sx = 0x7FFF;
    else if (-ly == dist)
        sx = 0x8000;
    else
        sx = (ly << 15) / dist;
    ang = cAtan2(sy, sx);
    want = 0x40 - (ang >> 8) & 0xFF;
    diff = want - cur;
    if (diff < 0x20 || diff > 0xE0) {
        newf = want;
        done = 1;
    } else if (diff < 0x80)
        newf = (cur + 0x20) % 0x100;
    else
        newf = (cur + 0xE0) % 0x100;
    SET_HEADING(meptr, newf >> 5);
    SET_FINEHEAD(meptr, newf);
    return done;
}

signed char far compute_trz_or_try_rather(unsigned speed, unsigned char flag)
{
    int trz;
    register int dist;
    register int dz;

    set_up_target();
    dz = OBJ_Z(mytarget) - OBJ_Z(meptr);
    dist = cSqRt(tdisttsqr);
    if (dist == 0)
        return dz > 0 ? 0xF : 0xF1;
    trz = (dz << 2) / dist;
    if (trz > 0xF)
        trz = 0xF;
    if (trz < -0xF)
        trz = -0xF;
    if (!flag || speed == 0)
        return trz;
    trz += dist * 3 / speed;
    return trz;
}

void far set_critter_vars(struct Object far *obj)
{
    meptr = obj;
    myid = Obj_MemTPtr(meptr);
    mycst = &Creature[OBJ_INDEX(meptr)];
    myxpos = OBJ_HOMEX(meptr);
    myypos = OBJ_HOMEY(meptr);
    myzpos = OBJ_Z(meptr) >> 3;
    myxpost = (myxpos << 3) + OBJ_FINEX(meptr);
    myypost = (myypos << 3) + OBJ_FINEY(meptr);
    myxhome = meptr->qn.f.quality;
    myyhome = meptr->ol.f.owner;
    myoldheading = meptr->heading;
    myoldfacing = (OBJ_HEADING(meptr) << 5) + FINEHEAD(meptr);
    myoldspeed = meptr->b13 & 0x7F;
    myheight = ComObjData[OBJ_ITEM(meptr)].height;
    if (mycst->flier) {
        pn_act = &CN2;
        tp_act = &CT2;
    } else if (mycst->swims) {
        pn_act = &CN4;
        tp_act = &CT4;
    } else {
        pn_act = &CN1;
        tp_act = &CT1;
    }
}

void far set_cur_seq_len(void)
{
    register int page;

    page = grs_3dinf[CRIT_INDEX(meptr)].page;
    seq_len = seqptr[(page << 3) + SEQ(meptr)].len;
    seq_lframe = seq_len - 1;
}

unsigned char far critter_ai(void)
{
    struct Object far *plyr;
    signed char px;
    signed char py;
    int eyedist;
    unsigned char oldh;
    register int type;
    register int pdist;

    myid = Obj_MemTPtr(meptr);
    mycst = &Creature[OBJ_INDEX(meptr)];
    myxpos = OBJ_HOMEX(meptr);
    myypos = OBJ_HOMEY(meptr);
    plyr = Obj_IntTMem(1);
    px = OBJ_HOMEX(plyr);
    py = OBJ_HOMEY(plyr);
    eyedist = ((signed char)myxpos - lastXeye) * ((signed char)myxpos - lastXeye)
            + ((signed char)myypos - lastYeye) * ((signed char)myypos - lastYeye);
    pdist = ((signed char)myxpos - px) * ((signed char)myxpos - px)
          + ((signed char)myypos - py) * ((signed char)myypos - py);
    if (eyedist > 0x64 && pdist > 0x64 && GOAL(meptr) != 3) {
        SET_BIN(meptr, (BIN(meptr) + 8) % 16);
        return 1;
    }
    if (mycst->flier) {
        pn_act = &CN2;
        tp_act = &CT2;
    } else if (mycst->swims) {
        pn_act = &CN4;
        tp_act = &CT4;
    } else {
        pn_act = &CN1;
        tp_act = &CT1;
    }
    if (ComObjData[OBJ_ITEM(meptr)].resist & 8) {
        tp_act->mask &= ~0x20;
        tp_act->w6 &= ~0x20;
        tp_act->ignore |= 0x20;
    }
    failed = 0;
    control = 1;
    hitwall = 0;
    aligned = 0;
    didhitobj = 0;
    hitadoor = 0;
    dontchangedz = 0;
    if (SEQ(meptr) != 1 && SEQ(meptr) != 0 && B15_7(meptr)) {
        freepaths |= 1 << OBJ_PATH(meptr);
        SET_B15_7(meptr, 0);
    }
    if (!B15_6(meptr) || SPEED(meptr) != 0 || PITCH(meptr) != 0x10) {
        get_phys_data(meptr, pn_act);
        oldh = meptr->heading;
        crit_terr = get_terrain(meptr);
        do_crit_phys(pn_act, tp_act);
        XP = OBJ_HOMEX(meptr);
        YP = OBJ_HOMEY(meptr);
        set_phys_data(meptr, pn_act);
        if (meptr->heading != oldh)
            aligned = 1;
    }
    if (ComObjData[OBJ_ITEM(meptr)].resist & 8) {
        tp_act->mask |= 0x20;
        tp_act->w6 |= 0x20;
        tp_act->ignore &= ~0x20;
    }
    myxpos = OBJ_HOMEX(meptr);
    myypos = OBJ_HOMEY(meptr);
    myzpos = OBJ_Z(meptr) >> 3;
    myxpost = (myxpos << 3) + OBJ_FINEX(meptr);
    myypost = (myypos << 3) + OBJ_FINEY(meptr);
    myxhome = meptr->qn.f.quality;
    myyhome = meptr->ol.f.owner;
    myoldheading = meptr->heading;
    myoldfacing = (OBJ_HEADING(meptr) << 5) + FINEHEAD(meptr);
    myoldspeed = meptr->b13 & 0x7F;
    myheight = ComObjData[OBJ_ITEM(meptr)].height;
    set_cur_seq_len();
    if (GOAL(meptr) == 0xB || GOAL(meptr) == 3)
        critter_mv();
    else if (SEQ(meptr) == 7) {
        if (FRAME(meptr) == seq_lframe) {
            death_check(meptr, 1);
            XP = OBJ_HOMEX(meptr);
            YP = OBJ_HOMEY(meptr);
            if (Obj_Rem(&Map_GetAddr(XP, YP)->objects.word, meptr)) {
                generate_inventory(meptr);
                build_corpse(meptr, mycst->corpse, mycst->remains);
                drop_some_objects(meptr);
                Obj_Free(meptr);
                return 0;
            }
        } else
            SET_FRAME(meptr, FRAME(meptr) + 1);
    } else if (SEQ(meptr) >= 3 && SEQ(meptr) <= 5) {
        if (FRAME(meptr) == 0 && GTARG(meptr) == 1) {
            if (get_current_music() < 2 || get_current_music() > 4)
                set_new_music(3);
            lastcombattime = *Time;
        }
        if (FRAME(meptr) == 3)
            critter_attack(meptr, rand() % 9, atk_charge[ATKFRAME(meptr)].charge, SEQ(meptr) - 3,
                           mycst->b0F);
        if (FRAME(meptr) == seq_lframe) {
            SET_SEQ(meptr, 2);
            SET_FRAME(meptr, 0);
            SET_ATKFRAME(meptr, 0);
        } else
            SET_FRAME(meptr, FRAME(meptr) + 1);
    } else if (SEQ(meptr) == 6) {
        if (FRAME(meptr) == 3) {
            if (CAST(meptr)) {
                missile_try = compute_trz_or_try_rather(0x1E, 0);
                cast(mycst->spells[CAST(meptr) - 1], meptr, 0L);
            } else {
                type = mycst->arms[0].item & 0xF;
                missile_try = compute_trz_or_try_rather(Missile[type].type, 1);
                critter_fire(meptr, type, Missile[type].type);
            }
        }
        if (FRAME(meptr) == seq_lframe) {
            SET_SEQ(meptr, 2);
            SET_FRAME(meptr, 0);
            SET_CAST(meptr, 0);
            SET_ATKFRAME(meptr, 0);
        } else
            SET_FRAME(meptr, FRAME(meptr) + 1);
    } else
        critter_mv();
    SET_BIN(meptr, (BIN(meptr) + RATE(meptr)) % 16);
    return 1;
}

void far critter_mv(void)
{
    unsigned char targeted = 0;
    unsigned char snd;

    didmove = 0;
    SET_B18_5(meptr, 0);
    SET_B15_6(meptr, 0);
    if (GOAL(meptr) == 0xB || GOAL(meptr) == 0xF)
        goto do_goal;
    if (SEQ(meptr) == 1 && (FRAME(meptr) & 1) == 1) {
        snd = 0xFF;
        switch (mycst->sound) {
            case 1:
                if (FRAME(meptr) == 1)
                    snd = 0x5A;
                else if (FRAME(meptr) == 3)
                    snd = 0x5B;
                break;
            case 6:
                if (FRAME(meptr) == 1)
                    snd = 0x2F;
                else if (FRAME(meptr) == 3)
                    snd = 0x30;
                break;
            case 7:
                if (FRAME(meptr) == 1)
                    snd = 0x1D;
                else if (FRAME(meptr) == 3)
                    snd = 0x1D;
                break;
            case 3:
                snd = 0x27;
                break;
            case 2:
                snd = 0x17;
                break;
            case 5:
                snd = 0xD;
                break;
            case 4:
                snd = 0xE;
                break;
            default:
                snd = 0xFF;
            }
            if (snd != 0xFF)
                play_effect(snd, myxpost, myypost, 0);
        }
    if (mycst->bA_1)
        goto do_goal;
    if ((!B19_6(meptr) && crithit != myid && mycst->race == typehit && !B0A_7(meptr) || B19_6(meptr))
        && crithittime + 0x200 > player->game_clock
        && abs(myxpos - hitx) + abs(myypos - hity) < mycst->hearing) {
        SET_ATTITUDE(meptr, 0);
        SET_B19_0(meptr, 1);
        if (GOAL(meptr) != 9 && GOAL(meptr) != 6) {
            critter_set_goal(5, B19_6(meptr) ? crithit : 1);
            set_loc(hitx, hity, hitpz);
        }
    }
    if (meptr->last_hit > 0
        && (meptr->last_hit == 1 && !B19_6(meptr) || B19_6(meptr)
            || B19_6(Obj_IntTMem(meptr->last_hit)))) {
        if (meptr->last_hit != GTARG(meptr))
            SET_GTARG(meptr, meptr->last_hit);
        if (!set_up_target())
            goto do_goal;
        targeted = 1;
        if (meptr->last_hit == 1) {
            SET_ATTITUDE(meptr, 0);
            set_loc(OBJ_HOMEX(ThePlayer), OBJ_HOMEY(ThePlayer), OBJ_Z(ThePlayer) >> 3);
            SET_B19_0(meptr, 1);
        }
        if (tdistsqr > 2 && (!mycst->b2D_0 || anti_magic_p(myxpos, myypos))) {
            SET_B19_5(meptr, 1);
            critter_set_goal(5, meptr->last_hit);
        } else if (B19_5(meptr))
            critter_set_goal(5, meptr->last_hit);
        else if (!B19_4(meptr) && should_i_flee(mycst->avghit, meptr->hp, mycst->b1C_0, DAMAGE(meptr)))
            critter_set_goal(6, meptr->last_hit);
        else if (B19_4(meptr)) {
            SET_B19_4(meptr, 1);
            critter_set_goal(9, meptr->last_hit);
        } else
            critter_set_goal(5, meptr->last_hit);
        meptr->last_hit = 0;
        meptr->b11 = 0;
    }
do_goal:
    switch (GOAL(meptr)) {
    case 0:
    case 7:
        crit_guard();
        break;
    case 1:
        crit_head_for_loc(myxhome, myyhome, Map_GetAddr(myxhome, myyhome)->height);
        break;
    case 2:
        crit_drunkwalk();
        break;
    case 4:
        crit_guard();
        break;
    case 5:
        if (!targeted && !set_up_target())
            critter_discard_goal();
        else
            crit_offense();
        break;
    case 6:
        if (!targeted && !set_up_target())
            critter_discard_goal();
        else
            crit_flee();
        break;
    case 8:
        crit_mill();
        break;
    case 9:
        if (!targeted && !set_up_target())
            critter_discard_goal();
        else
            crit_defense();
        break;
    case 10:
        crit_talk();
        break;
    case 11:
        SET_RATE(meptr, 4);
        SET_SPEED(meptr, rand() % 2);
        meptr->heading = rand() % 0x100;
        SET_PITCH(meptr, rand() % 3 + 0xF);
        SET_FRAME(meptr, (FRAME(meptr) + 1) % seq_len);
        SET_B15_6(meptr, 1);
        break;
    case 15:
        if (control) {
            SET_SPEED(meptr, 0);
            SET_PITCH(meptr, 0x10);
        }
        SET_RATE(meptr, 7);
        if (GTARG(meptr) > 0)
            SET_GTARG(meptr, GTARG(meptr) - 1);
        else
            critter_discard_goal();
        break;
    case 3:
        break;
    case 12:
        crit_hover();
        break;
    default:
        SET_RATE(meptr, 7);
    }
    constrain_movement();
}

unsigned char far set_up_target(void)
{
    mytarget = Obj_IntTMem(GTARG(meptr));
    if (mytarget->hp <= 0)
        return 0;
    txpos = OBJ_HOMEX(mytarget);
    typos = OBJ_HOMEY(mytarget);
    tzpos = OBJ_Z(mytarget) >> 3;
    txpost = (txpos << 3) + OBJ_FINEX(mytarget);
    typost = (typos << 3) + OBJ_FINEY(mytarget);
    tdx = txpost - myxpost;
    tdy = typost - myypost;
    tdistsqr = (txpos - myxpos) * (txpos - myxpos) + (typos - myypos) * (typos - myypos);
    tdisttsqr = (txpost - myxpost) * (txpost - myxpost) + (typost - myypost) * (typost - myypost);
    return 1;
}

/* IDA's MaybeShouldNPCWithdraw. Named from FM Towns: should_i_flee_ is at the same place
   between set_up_target_ and acceptable_danger_, and makes the same tests of the hit
   points against three quarters, an eighth and half of the maximum, then the rand() % 4
   roll against 15 less the third argument. */
unsigned char far should_i_flee(unsigned char maxhp, unsigned char hp, unsigned char nerve,
                                unsigned char damage)
{
    register unsigned r;

    if (maxhp * 3 >> 2 < hp)
        return 0;
    if (maxhp >> 3 > hp)
        return 0;
    if (maxhp >> 1 < damage)
        return 1;
    if (maxhp == 0)
        return 0;
    r = (hp << 4) / maxhp + rand() % 4;
    if (0xF - nerve < r)
        return 0;
    return 1;
}

/* IDA's GetCritterRange. Named from FM Towns: acceptable_danger_ is next there and has
   the same body (attitude, the critter type's hit points, bit 13 of the item id, then
   hp * 4 / max hp plus a quarter of the low nibble at +0x1C). Nothing calls it. */
unsigned char far acceptable_danger(void)
{
    register unsigned char d;

    if (ATTITUDE(meptr) != 0 || mycst->avghit == 0 || ID_B13(meptr))
        return 0;
    d = (meptr->hp << 2) / mycst->avghit + mycst->b1C_0 / 4;
    return d;
}

void far critter_set_goal(unsigned char goal, int target)
{
    if (GOAL(meptr) == 4)
        SET_OLDGOAL(meptr, GOAL(meptr));
    SET_GOAL(meptr, goal);
    SET_GTARG(meptr, target);
}

void far critter_discard_goal(void)
{
    if (OLDGOAL(meptr)) {
        SET_GOAL(meptr, OLDGOAL(meptr));
        SET_GTARG(meptr, 1);
        SET_OLDGOAL(meptr, 0);
    } else {
        SET_GOAL(meptr, 2);
        SET_GTARG(meptr, 0);
    }
}

unsigned char far go_into_dying_sequence(struct Object far *obj)
{
    if (obj->whoami == 0 || death_check(obj, 0)) {
        SET_SEQ(obj, 7);
        SET_FRAME(obj, 0);
        SET_RATE(obj, 4);
        obj->hp = 0;
        return 1;
    }
    return 0;
}

unsigned char far crit_die(struct Object far *obj)
{
    unsigned char snd;

    if (!(SEQ(obj) == 7 || !go_into_dying_sequence(obj))) {
        switch (victim->death) {
        case 0:
            return 1;
        case 1:
            snd = 6;
            break;
        case 3:
            snd = 0x23;
            break;
        case 4:
            snd = 0x24;
            break;
        case 2:
            snd = 0x22;
            break;
        default:
            snd = 6;
        }
        play_effect(snd, (OBJ_HOMEX(obj) << 3) + OBJ_FINEX(obj), (OBJ_HOMEY(obj) << 3) + OBJ_FINEY(obj),
                    0);
        return 1;
    }
    return 0;
}

unsigned char far damage_critter(struct Object far *obj, unsigned char damage,
                                 struct Object far *from)
{
    unsigned char who;
    int minor;
    int index;
    int m;
    register struct Creature near *cr;
    register int ratio;

    minor = OBJ_MINOR(obj);
    index = obj->id & 0xF;
    cr = &Creature[(minor << 4) + index];
    victim = cr;
    SET_DAMAGE(obj, DAMAGE(obj) + damage);
    if (from == 0 || from >= objdata)
        who = 0;
    else if (OBJ_MAJOR(from) != 1)
        who = from->last_hit;
    else {
        m = Obj_MemTPtr(from);
        who = m >= 0x100 ? 0 : m;
    }
    if (who)
        obj->last_hit = who;
    if (who == 1 && !B0A_7(obj)) {
        typehit = cr->race;
        crithit = Obj_MemTPtr(obj);
        hitx = OBJ_HOMEX(obj);
        hity = OBJ_HOMEY(obj);
        hitpz = OBJ_Z(obj) >> 3;
        crithittime = player->game_clock;
    }
    if (obj->hp <= damage) {
        obj->hp = 0;
        if (crit_die(obj)) {
            if (who == 1)
                player_killed_a(obj);
            return 1;
        }
    } else {
        obj->hp = obj->hp - damage;
        if (obj == ThePlayer)
            panel_check_hpmp();
    }
    if (who == 1 && obj != ThePlayer) {
        ratio = (obj->hp << 6) / (cr->avghit + 1);
        if (ratio < 0x10)
            set_new_music(2);
        else
            set_new_music(3);
        lastcombattime = *Time;
    } else if (obj == ThePlayer && who) {
        ratio = (ThePlayer->hp << 6) / (playerdat->avghit + 1);
        if (ratio < 0x10)
            set_new_music(4);
        else
            set_new_music(3);
        lastcombattime = *Time;
    }
    return 0;
}

/* IDA's CheckIfUpdateNeeded. Named from FM Towns: timetodo_ is next there, before
   move_mobile_, which calls it with the same two arguments, and makes the same tests of
   curBin and lastbin. */
unsigned char far timetodo(int bin, int rate)
{
    if (curBin > bin && bin + 4 >= curBin
        || curBin + 16 > bin && lastbin <= bin && lastbin > curBin)
        return 1;
    return 0;
}

void far move_mobile(char delta)
{
    unsigned char far *p;
    unsigned char ok;

    curBin = lastbin + delta & 0xF;
    meptr = 0;
    map_crit_pages();
    seqptr = MK_FP(EmsBuff + 0xC00, 0);
    for (p = ActiveMob; p < LastActiveMob; p++) {
        meptr = &critdata[*p];
        while (timetodo(BIN(meptr), RATE(meptr))) {
            if (OBJ_MAJOR(meptr) == 1)
                ok = critter_ai();
            else
                ok = move_me_joe();
            if (!ok) {
                p--;
                break;
            }
        }
    }
    if (meptr != 0)
        editchng(2);
    lastbin = curBin;
}
