/* motion.h: Motion and physics: moving objects through the tile grid, collisions, and the
   clock that drives them. The collision record struct MotionCalc, struct Collision and
   COLLIDE.C's checks are in map.h. docs/subsystems/motion.md describes the subsystem. */
#ifndef MOTION_H
#define MOTION_H

#include "uw2.h"

struct Handler;
struct MotionCalc;
struct MotionParams;
struct Object;
struct Phys;

#include "map.h"
#include "object.h"

/* A physics record, 40 bytes: the player's (PN) and the four scratch records CN1..CN4 are
   seg031's, which steps them through the tile grid through the near pointer CP. CN1 is
   for walking critters, CN2 flying, CN4 swimming (critter/AI.C) and CN3 for moving
   objects and missiles (critter/PATHFIND.C); OBJPHYS.C's get_phys_data fills one from an
   object. The field names are ours, from their use. */
struct Phys {
    int x, y, z;                        /* 0x00, x and y in 1/256 tiles, z in 1/8 of an
                                           object's z unit */
    int vel[3];                         /* 0x06, x and y are set from heading and speed */
    int acc[3];                         /* 0x0C, acc[2] is gravity: -4, -2 (Leap or a
                                           jump trap) or 0 */
    int time;                           /* 0x12, the time units left to move */
    int speed;                          /* 0x14, along heading; 0x2F to an object's speed
                                           step */
    unsigned char bounce;               /* 0x16, 0 (dead) to 15 (fully elastic) */
    unsigned char flags;                /* 0x17, 0x80 slide along walls, 0x40 never turn */
    int mass;                           /* 0x18, from ComObjData */
    unsigned char light;                /* 0x1A, from ComObjData; get_phys_data also uses
                                           it to set a resting object's speed */
    unsigned char hp;                   /* 0x1B, hit points, or a static object's quality */
    unsigned char resist;               /* 0x1C, from ComObjData */
    unsigned char b1D;                  /* 0x1D, set to 0 and not read here */
    int heading;                        /* 0x1E, a full turn is 0x10000 */
    int index;                          /* 0x20, the object's number; 1 is the player */
    unsigned char radius;               /* 0x22 */
    unsigned char height;               /* 0x23 */
    unsigned char b24;                  /* 0x24, the step height: how far up or down the
                                           floor may change in a step (8 for the player
                                           and creatures, 0 for objects) */
    unsigned char terrain;              /* 0x25, one bit per terrain type: 1 floor,
                                           2 water, 4 lava, 8 ice, 0x10 air (MOTION.C) */
    unsigned impact;                    /* 0x26, the speed lost in collisions this move,
                                           which becomes damage */
};

/* A mover's handler, 12 bytes, reached through the near pointer TP while seg031 moves a
   physics record: which collision bits to ignore, which to pass to special, and which
   stop it climbing onto objects. The player's is PT, the critters' CT1..CT4 (set up by
   critter/PATHFIND.C's init_ai, the same split as CN1..CN4). The bits are MOTION.C's
   collision state. */
struct Handler {
    unsigned ignore;                    /* 0x00, 0x1000 means no falling (flying) */
    unsigned mask;                      /* 0x02 */
    unsigned noclimb;                   /* 0x04, bit 0x80 forbids climbing onto objects;
                                           any bit set in the state forbids stepping */
    unsigned w6;                        /* 0x06, only critter path finding reads it, with
                                           noclimb */
    unsigned char (far *special)();     /* 0x08, called with the collision state's address
                                           when state & mask */
};

/* The stepping state: a Bresenham walk along the major axis of the velocity, one cell
   (1/8 tile) a step. MOTION.C's space_to_motion sets it up. */
/* name: FM Towns's _MP, one initialised struct at DS:3FA (FM Towns reads DS:416 and
   DS:417 as _MP+0x20 and _MP+0x21, so they are fields, not separate globals; its
   pointers are 4 bytes). The field names are ours. */
struct MotionParams {
    int *vel;                           /* 0x00, CP->vel */
    int *pos;                           /* 0x02, Ppd's x, y and z */
    int frac[3];                        /* 0x04, the fraction of pos, 0x2000 to a unit
                                           (0x800 in z) */
    int step[3];                        /* 0x0A, added to frac each step; the major
                                           axis's is +-0x2000, z's +-0x800 */
    int major;                          /* 0x10, 0 x or 1 y, the faster */
    int minor;                          /* 0x12 */
    int steps;                          /* 0x14, whole steps in this move */
    int rem;                            /* 0x16, the fraction of a last step */
    int dt;                             /* 0x18, time units per step */
    int done;                           /* 0x1A, steps taken; set past steps to stop */
    signed char hit;                    /* 0x1C, DS:416, the oCollisions index at targz,
                                           or -1 */
    int item;                           /* 0x1D, DS:417, that object's item */
    int targz;                          /* 0x1F, the height the mover stops at in z */
    int f21;                            /* 0x21, not used in MOTION.C */
    int f23;                            /* 0x23, the lowest bottom of an object overhead */
    int zspeed;                         /* 0x25, z change per whole step, in 1/64 of a
                                           z unit (full_move adds zspeed << 5 to frac) */
    unsigned headings[8];               /* 0x27, wall normals, by ComputeHeading's
                                           direction */
};

/* OBJPHYS.C: object physics */
void far get_phys_data(struct Object far *obj, struct Phys *pp);
unsigned char far set_phys_data(struct Object far *obj, struct Phys *pp);
struct Object far * far static_to_mob(struct Object far *obj);
void far mob_init(struct Object far *obj, int x, int y);
struct Object far * far mob_to_static(struct Object far *obj);
void far update_hack_vecs(struct Phys *pp);
void far missile_newhit(struct Object far *proj, struct Object far *hit);
int far do_objhit(int ci, int index);

/* MOTION.C: motion */
extern struct MotionCalc Ppd;
extern struct Phys PN;
extern struct Phys CN1;
extern struct Phys CN2;
extern struct Phys CN3;
extern struct Phys CN4;
extern struct Phys near *CP;
extern struct Handler PT;
extern struct Handler CT1;
extern struct Handler CT2;
extern struct Handler CT3;
extern struct Handler CT4;
extern unsigned char res_to_terr[18];
extern struct MotionParams MP;  /* DS:3FA; hit at DS:416, item at DS:417 */
unsigned char far space_to_motion(char pos, char check);
unsigned char far grid_move(int dir);
void far check_positions(void);
int far get_pcoll(void);
void far back_to_space(void);
void far set_targz(char how);
void far do_physics(struct Phys *pp, struct Handler *tp);
void far set_jmp(int force, char stop, int min);
struct Object far * far IsaDoor(unsigned char *x, unsigned char *y);
struct Object far * far CollObject(void);

/* PLAYMOVE.C: player input and motion */
extern unsigned char pmouseHandled;
extern unsigned char combEfflen;
extern unsigned char tremEfflen;
extern unsigned char slidEfflen;
extern int PlayerInput;
extern int PlayerTurn;
extern char MoveCamera;
extern int ForwInpRate;
extern int TurnInpRate;
extern int PlayerBank;
extern int campos[3];
extern int camang[3];
extern int vort_theta;
extern unsigned char vort_x;
extern unsigned char vort_y;
void far parse_playin(int command);
void far do_player_keyboard(void);
void far move_physics(int incr, int frames, unsigned char easy);
void far move_player(int incr);
void far make_noise(char easy);
void far set_sound(char easy);
void far parse_effect(void);
void far player_mous_move(void);
void far player_simple_move(int dir);
void far check_physics(void);
void far finish_player(void);
void far get_eye(void);

#endif
