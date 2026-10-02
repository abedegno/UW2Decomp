/* critter.h: Critters: their motion, AI and goals, their class data, and the critter art
   pages. */
#ifndef CRITTER_H
#define CRITTER_H

#include "uw2.h"

struct Creature;
struct Handler;
struct Object;
struct PathRec;
struct PathSq;
struct Phys;

#include "motion.h"
#include "object.h"

/* A creature type's properties, 48 bytes: the critters table of DATA\OBJECTS.DAT, 64 of
   them, loaded into Creature[]. The player's own entry is Creature[63], reached through
   playerdat. The names are provisional, taken from the sources that use the fields. */
struct Creature {
    unsigned char armour[4];            /* 0x00, by hit location; ovr157 reads armour[0]
                                           as the creature's level */
    unsigned char avghit;               /* 0x04, average hit points; for the player,
                                           maximum vitality */
    unsigned char attr[3];              /* 0x05: strength, dexterity, intelligence */
    unsigned char death:3;              /* 0x08 */
    unsigned char blood:2;
    unsigned char corpse:3;
    unsigned char race;                 /* 0x09 */
    unsigned char passive:1;            /* 0x0A */
    unsigned char bA_1:1;
    unsigned char remains:3;
    unsigned char bA_5:1;
    unsigned char swims:1;
    unsigned char flier:1;
    unsigned char speed;                /* 0x0B */
    unsigned char run;                  /* 0x0C */
    unsigned level:4;                   /* 0x0D-0x0E, the trading temper (FM Towns reads the
                                           same nibbles); bartering calls this one wit */
    unsigned shrewd:4;
    unsigned haggle:4;                  /* 0x0E */
    unsigned patience:4;
    unsigned char b0F;                  /* 0x0F */
    unsigned char sound:4;              /* 0x10 */
    unsigned char armour_kind:2;
    unsigned char weapon_kind:2;
    signed char equip;                  /* 0x11 */
    signed char defence;                /* 0x12 */
    struct {
        signed char chance;
        unsigned char damage;           /* attacks[0].damage is also the damage done to doors */
        unsigned char prob;
    } attacks[3];                       /* 0x13 */
    unsigned char b1C_0:4;              /* 0x1C */
    unsigned char range:4;
    unsigned char noise:4;              /* 0x1D */
    unsigned char visibility:4;
    unsigned char hearing:4;            /* 0x1E */
    unsigned char sight:4;
    unsigned char lazy:4;               /* 0x1F */
    unsigned char alert:4;
    struct {
        unsigned char present:1;
        unsigned char item:7;
    } arms[2];                          /* 0x20, arms[0].item is the missile it fires */
    struct {
        unsigned prob:4;
        unsigned item:12;
    } other[2];                         /* 0x22 */
    unsigned treasure_prob:4;           /* 0x26 */
    unsigned treasure_rate:4;
    unsigned food_prob:4;               /* 0x27 */
    unsigned food_item:4;
    int exp;                            /* 0x28 */
    unsigned char spells[3];            /* 0x2A */
    unsigned char b2D_0:1;              /* 0x2D */
    unsigned char caster:7;
    unsigned char locks;                /* 0x2E */
    unsigned char b2F;
};

/* One square of a critter's path, 4 bytes. */
struct PathSq { unsigned char x, y, unused, flag; };

/* A critter's path, 28 bytes: two bits of direction and one bit of slope for each of up
   to 64 steps, the steps taken (flag.bits.count) and the path's length (index). */
struct PathRec {
    unsigned char x, y;
    union {
        unsigned char raw;
        struct { unsigned char count:7, slope:1; } bits;
    } flag;
    unsigned char index, directions[16], slopes[8];
};

/* A map square's path-finding record, 5 bytes: seg006 fills stdat (the shared far
   buffer) with 64 by 64 of them while it searches for a path. */
struct StaticTile {
    unsigned char pathx, pathy;         /* 0x00, the square the path came from */
    unsigned char height;               /* 0x02 */
    unsigned char pathflag:1, dist:7;   /* 0x03 */
    unsigned char step;                 /* 0x04 */
};

/* PATHFIND.C: critter motion, homing projectiles, path traversal and doors */
extern int crit_terr;
extern int tdx;
extern int tdy;
extern unsigned char control;
extern struct Handler near *tp_act;
extern unsigned char txpos;
extern unsigned char aligned;
extern unsigned char dontchangedz;
extern unsigned txpost;
extern unsigned char myid;
extern unsigned tdistsqr;
extern unsigned char failed;
extern unsigned char typos;
extern unsigned long tdisttsqr;
extern struct Creature near *mycst;
extern struct Object far *meptr;
extern unsigned char pathlen;
extern unsigned char myxpos;
extern unsigned char myypos;
extern unsigned char myzpos;
extern unsigned char hitwall;
extern unsigned typost;
/* The current critter's target, set up by set_up_target. */
extern struct Object far *mytarget;
extern unsigned char myxhome;
extern unsigned char myyhome;
extern unsigned char myheight;
extern unsigned char didhitobj;
extern unsigned char didmove;
extern unsigned char hitadoor;
extern struct Phys near *pn_act;
extern unsigned char myoldspeed;
extern signed char tzpos;
extern int XP;
extern unsigned char myoldfacing;
extern int YP;
extern unsigned char myoldheading;
void far make_path_from_flood_data(unsigned char length, unsigned char x, unsigned char y);
void far try_to_open_door(struct Object far *door);
unsigned char far crit_hndlr_walk(struct Phys *pn);
unsigned char far crit_hndlr_fly(struct Phys *pn);
unsigned char far crit_hndlr_swim(struct Phys *pn);
void far check_homing(void);
void far check_sat(void);
void far do_that_jump_kinda_thing(struct PathRec far *path);
unsigned char far deltatotheta(char x, char y);
unsigned char far add_to_beeline_path(unsigned char x, unsigned char y);
void far adjust_height(unsigned char x, unsigned char y);
void far build_corpse(struct Object far *obj, char fluids, char corpse);
unsigned char far move_me_joe(void);
void far init_ai(void);
int far get_terrain(struct Object far *obj);
void far crit_head_for_loc(unsigned char x, unsigned char y, char z);

/* AI.C: critter movement and AI */
extern int lastXeye;
extern int lastYeye;
extern unsigned char hitx;
extern unsigned char hity;
extern unsigned char seq_len;
extern unsigned char seq_lframe;
void far change_or_inc_seq(int seq, char force);
void far set_htx(int heading);
void far crit_drunkwalk(void);
void far crit_mill(void);
void far crit_guard(void);
unsigned char far crit_attack(unsigned dist);
void far crit_offense_find_target(unsigned char x, unsigned char y, unsigned char how);
unsigned char far maybe_cast_defensive_spell(void);
unsigned char far crit_magik_attack(void);
unsigned char far crit_missile_attack(void);
void far crit_flee(void);
unsigned char far crit_avoid_player(unsigned char heading, int how);
void far check_out_player(void);
unsigned char far target_found(unsigned char *x, unsigned char *y);
unsigned char far look_for_target(char how);
unsigned char far turn_real_fine(signed char dx, signed char dy);
void far set_cur_seq_len(void);
void far critter_mv(void);
unsigned char far go_into_dying_sequence(struct Object far *obj);
unsigned char far crit_die(struct Object far *obj);
unsigned char far timetodo(int bin, int rate);
unsigned char far should_i_flee(unsigned char maxhp, unsigned char hp, unsigned char nerve,
                                unsigned char damage);
unsigned char far set_up_target(void);
void far critter_discard_goal(void);
void far set_critter_vars(struct Object far *obj);
unsigned char far acceptable_danger(void);
unsigned char far damage_critter(struct Object far *obj, unsigned char damage,
                                 struct Object far *from);

/* CRITTIME.C: critters between moments */
void far change_critter_goal(struct Object far *npc, char goal, int gtarg);
void far yearly_checkup(void);
void far update_all_critters_whilst_player_snoozes(void);
char far hostile_creatures_near(void);
char far wandering_monster_check(void);
void far set_creatures_from_saved_game(void);
void far set_creatures_to_saved_game(void);
void far init_level_creature_stuff(void);
void far player_grabbed(struct Object far *obj, unsigned char owner);
void far maybe_rescue_guy_from_fire(struct Object far *obj);
void far maybe_cheat_arena_fire(void);
void far arena_opponent_runs(struct Object far *obj);
void far where_shall_we_hang_out(struct Object far *npc, int *x, int *y);
char far maybe_go_hang_out(struct Object far *npc);

/* CREATURE.C: creature class data */
extern struct Creature Creature[64];
char far init_this_critter(struct Object far *obj);
void far creature_obj_init(void);

/* CRPAGES.C: critter art pages */
void far NightCleanCritPages(void);
void far PreLoadCritPages(void);

#endif
