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
    uint16 level:4;                     /* 0x0D-0x0E, the trading temper (FM Towns reads the
                                           same nibbles); bartering calls this one wit */
    uint16 shrewd:4;
    uint16 haggle:4;                    /* 0x0E */
    uint16 patience:4;
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
        uint16 prob:4;
        uint16 item:12;
    } other[2];                         /* 0x22 */
    uint16 treasure_prob:4;             /* 0x26 */
    uint16 treasure_rate:4;
    uint16 food_prob:4;                 /* 0x27 */
    uint16 food_item:4;
    int16 exp;                          /* 0x28 */
    unsigned char spells[3];            /* 0x2A */
    unsigned char b2D_0:1;              /* 0x2D */
    unsigned char caster:7;
    unsigned char locks;                /* 0x2E */
    unsigned char b2F;
};

/* struct Creature's attr[]: the three attributes, in the order of the character screen
   (STRINGS.PAK block 2's "Str:", "Dex:", "Int:") and of the player record's strength,
   dexterity and intelligence (PLAYDATA.C copies attr[0..2] to and from them). */
#define ATTR_STR        0
#define ATTR_DEX        1
#define ATTR_INT        2

/* A critter's goal, the low nibble of its goal word (OBJ_GOAL), with the goal target
   (OBJ_GTARG, a mobile index, 1 the player) beside it. The names are ours, from what
   AI.C's critter_mv runs for each; UW-Formats (7.x, npc_goal) knows only that 5 kills
   the player. Other values only slow the critter (rate 7). */
#define GOAL_STAND      0               /* stand, watching for the player (crit_guard) */
#define GOAL_GO_HOME    1               /* head home (myxhome, myyhome), then mill */
#define GOAL_WANDER     2               /* wander (crit_drunkwalk) */
#define GOAL_FOLLOW     3               /* UW1: keep close to the target, jumping to it
                                           from afar (seg007_1798_3E4); never skipped
                                           for distance from the player */
#define GOAL_GUARD      4               /* guard: watch, attack what comes near; kept in
                                           the old goal while another goal runs */
#define GOAL_ATTACK     5               /* attack the target (crit_offense) */
#define GOAL_FLEE       6               /* flee the target (crit_flee) */
#define GOAL_STAND_7    7               /* as GOAL_STAND */
#define GOAL_MILL       8               /* mill about near home (crit_mill); a new
                                           critter's goal */
#define GOAL_CORNERED   9               /* fight back when cornered (crit_defense) */
#define GOAL_TALK       10              /* come and talk to the player (crit_talk) */
#define GOAL_FLUTTER    11              /* drift at random, whatever the sequence, not
                                           reacting to blows */
#define GOAL_HOVER      12              /* stand at home (crit_hover) */

/* A critter's attitude to the player, bits 14-15 of its attitude word (OBJ_ATTITUDE):
   the names are UW-Formats' (7.x, npc_attitude: 0 hostile, 1 upset, 2 mellow,
   3 friendly). */
#define ATT_HOSTILE     0
#define ATT_UPSET       1
#define ATT_MELLOW      2               /* a new critter's */
#define ATT_FRIENDLY    3

/* A critter's animation sequence (OBJ_SEQ), four frames each (OBJ_FRAME counts them).
   The names are ours, from what AI.C and PATHFIND.C do with each: UW1 writes the
   sequence numbers out where UW2 looks them up (UW2's change_or_inc_seq). */
#define SEQ_COMBAT      0               /* the combat stance */
#define SEQ_ATTACK1     1               /* the three melee attacks (1 to 3), by the
                                           creature's attacks[]; the blow lands on frame 4 */
#define SEQ_ATTACK3     3
#define SEQ_FIRE        5               /* firing a missile */
#define SEQ_BACK_OFF    7               /* backing away */
#define SEQ_DYING       0xC             /* dying; removed after frame 3 */
#define SEQ_CAST        0xD             /* casting a spell */
#define SEQ_STAND       0x20            /* standing */
#define SEQ_WALK        0x2C            /* walking */

/* Turns the current critter (meptr) to face eighth h (0 to 7): its heading byte, the
   3-bit heading and a zero fine heading. UW2 has set_htx as a function in AI.C; UW1
   writes the three stores out at each use (AI.C, PATHFIND.C), so it is a macro here. */
#define set_htx(h) { meptr->heading = (h) << 5; SET_HEADING(meptr, (h)); SET_FINEHEAD(meptr, 0); }

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
/* stdat (gfx.h) as the pathfinder's 64 by 64 squares. */
#define STILES ((struct StaticTile (far *)[MAP_SIZE])stdat)

/* PATHFIND.C: critter motion, homing projectiles, path traversal and doors */
extern int16 crit_terr;
extern int16 tdx;
extern int16 tdy;
extern unsigned char control;
extern struct Handler near *tp_act;
extern unsigned char txpos;
extern unsigned char aligned;
extern unsigned char dontchangedz;
extern uint16 txpost;
extern unsigned char myid;
extern uint16 tdistsqr;
extern unsigned char failed;
extern unsigned char typos;
extern uint32 tdisttsqr;
extern struct Creature near *mycst;
extern struct Object far *meptr;
extern unsigned char pathlen;
extern unsigned char myxpos;
extern unsigned char myypos;
extern unsigned char myzpos;
extern unsigned char hitwall;
extern uint16 typost;
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
extern int16 XP;
extern unsigned char myoldfacing;
extern int16 YP;
extern unsigned char myoldheading;
void far make_path_from_flood_data(unsigned char length, unsigned char x, unsigned char y);
void far try_to_open_door(struct Object far *door);
unsigned char far crit_hndlr_walk(uint16 *state);
unsigned char far crit_hndlr_fly(uint16 *state);
unsigned char far crit_hndlr_swim(uint16 *state);
void far do_that_jump_kinda_thing(struct PathRec far *path);
unsigned char far deltatotheta(char x, char y);
unsigned char far add_to_beeline_path(unsigned char x, unsigned char y);
void far adjust_height(unsigned char x, unsigned char y);
void far build_corpse(struct Object far *obj, char fluids, char corpse);
unsigned char far move_me_joe(void);
void far init_ai(void);
int far get_terrain(struct Object far *obj);
void far crit_head_for_loc(unsigned char x, unsigned char y, char z);
/* The current critter's position, set up by set_critter_vars and critter_ai (AI.C). */
extern int16 myxpost;
extern int16 myypost;
extern uint16 freepaths;
unsigned char far do_crit_phys(struct Phys *pn, struct Handler *tp);
unsigned char far flood_path(char x, char y, unsigned char height0, char destx, char desty, char destz, unsigned char range);
unsigned char far line_of_sight(int x1, int y1, int z1, int x2, int y2, int z2);
void far set_loc(unsigned char x, unsigned char y, unsigned char z);
void far clear_paths(void);

/* AI.C: critter movement and AI */
extern int16 lastXeye;
extern int16 lastYeye;
extern unsigned char hitx;
extern unsigned char hity;
void far crit_drunkwalk(void);
void far crit_mill(void);
void far crit_guard(void);
unsigned char far crit_attack(unsigned dist);
void far crit_offense_find_target(unsigned char x, unsigned char y, unsigned char how);
char far maybe_cast_defensive_spell(void);
unsigned char far crit_magik_attack(void);
unsigned char far crit_missile_attack(void);
void far crit_flee(void);
unsigned char far crit_avoid_player(unsigned char heading, int how);
void far check_out_player(void);
unsigned char far target_found(unsigned char *x, unsigned char *y);
unsigned char far look_for_target(char how);
unsigned char far turn_real_fine(signed char dx, signed char dy);
void far critter_mv(void);
unsigned char far go_into_dying_sequence(struct Object far *obj);
unsigned char far crit_die(struct Object far *obj);
unsigned char far timetodo(int bin, int rate);
unsigned char far should_i_flee(unsigned char maxhp, unsigned char hp, unsigned char nerve,
                                unsigned char damage);
char far set_up_target(void);
void far critter_discard_goal(void);
void far set_critter_vars(struct Object far *obj);
unsigned char far acceptable_danger(void);
unsigned char far damage_critter(struct Object far *obj, unsigned char damage,
                                 struct Object far *from);
extern int32 lastcombattime;
extern uint32 crithittime;
extern signed char curBin;
extern unsigned char crithit;
extern unsigned char typehit;
void far critter_set_goal(unsigned char goal, int target);

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

/* CREATURE.C: creature class data */
#define NUM_CREATURES   0x40            /* creature types: OBJECTS.DAT's creature table */
extern struct Creature Creature[NUM_CREATURES];
char far creature_obj_init(void);
void far creature_init(FILE *fd);
struct Creature * far creature_class_data(void);

/* CRPAGES.C: critter art pages */
void far NightCleanCritPages(void);
char far preload_cr(char load_pages);
int far ovr113_2A2(int count, int16 *out);
int far swap_ws_out(void);
void far unswap_ws(unsigned handle);

#endif
