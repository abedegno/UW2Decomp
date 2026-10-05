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
    int16 x, y, z;                      /* 0x00, x and y in 1/256 tiles, z in 1/8 of an
                                           object's z unit */
    int16 vel[3];                       /* 0x06, x and y are set from heading and speed */
    int16 acc[3];                       /* 0x0C, acc[2] is gravity: -4, -2 (Leap or a
                                           jump trap) or 0 */
    int16 time;                         /* 0x12, the time units left to move */
    int16 speed;                        /* 0x14, along heading; 0x2F to an object's speed
                                           step */
    unsigned char bounce;               /* 0x16, 0 (dead) to 15 (fully elastic) */
    unsigned char flags;                /* 0x17, 0x80 slide along walls, 0x40 never turn */
    int16 mass;                         /* 0x18, from ComObjData */
    unsigned char light;                /* 0x1A, from ComObjData; get_phys_data also uses
                                           it to set a resting object's speed */
    unsigned char hp;                   /* 0x1B, hit points, or a static object's quality */
    unsigned char resist;               /* 0x1C, from ComObjData */
    unsigned char b1D;                  /* 0x1D, set to 0 and not read here */
    int16 heading;                      /* 0x1E, a full turn is 0x10000 */
    int16 index;                        /* 0x20, the object's number; 1 is the player */
    unsigned char radius;               /* 0x22 */
    unsigned char height;               /* 0x23 */
    unsigned char b24;                  /* 0x24, the step height: how far up or down the
                                           floor may change in a step (8 for the player
                                           and creatures, 0 for objects) */
    unsigned char terrain;              /* 0x25, one bit per terrain type: 1 floor,
                                           2 water, 4 lava, 8 ice, 0x10 air (MOTION.C) */
    uint16 impact;                      /* 0x26, the speed lost in collisions this move,
                                           which becomes damage */
};

/* struct Phys's terrain, the footing: one bit, set by MOTION.C's set_resterr from the
   collision state (and by do_zbounce and the air moves), turned into the 0-4 code an
   object stores by res_to_terr. The names are ours, from those functions. */
#define FOOT_FLOOR      0x01            /* a plain floor, or a solid object */
#define FOOT_WATER      0x02
#define FOOT_LAVA       0x04
#define FOOT_ICE        0x08
#define FOOT_AIR        0x10            /* in the air: falling, jumping, flying */
#define FOOT_SHORE      0x20            /* the player on water with a corner of the
                                           footprint on another floor */

/* A mover's handler, 12 bytes, reached through the near pointer TP while seg031 moves a
   physics record: which collision bits to ignore, which to pass to special, and which
   stop it climbing onto objects. The player's is PT, the critters' CT1..CT4 (set up by
   critter/PATHFIND.C's init_ai, the same split as CN1..CN4). The bits are MOTION.C's
   collision state. */
HOST_LAYOUT_BEGIN
struct Handler {
    uint16 ignore;                      /* 0x00, 0x1000 means no falling (flying) */
    uint16 mask;                        /* 0x02 */
    uint16 noclimb;                     /* 0x04, bit 0x80 forbids climbing onto objects;
                                           any bit set in the state forbids stepping */
    uint16 w6;                          /* 0x06, only critter path finding reads it, with
                                           noclimb */
    unsigned char (far *special)(uint16 *state);    /* 0x08, called with the collision state's address
                                           when state & mask */
};
HOST_LAYOUT_END

/* The stepping state: a Bresenham walk along the major axis of the velocity, one cell
   (1/8 tile) a step. MOTION.C's space_to_motion sets it up. */
/* name: FM Towns's _MP, one initialised struct at DS:3FA (FM Towns reads DS:416 and
   DS:417 as _MP+0x20 and _MP+0x21, so they are fields, not separate globals; its
   pointers are 4 bytes). The field names are ours. */
HOST_LAYOUT_BEGIN
struct MotionParams {
    int16 *vel;                         /* 0x00, CP->vel */
    int16 *pos;                         /* 0x02, Ppd's x, y and z */
    int16 frac[3];                      /* 0x04, the fraction of pos, 0x2000 to a unit
                                           (0x800 in z) */
    int16 step[3];                      /* 0x0A, added to frac each step; the major
                                           axis's is +-0x2000, z's +-0x800 */
    int16 major;                        /* 0x10, 0 x or 1 y, the faster */
    int16 minor;                        /* 0x12 */
    int16 steps;                        /* 0x14, whole steps in this move */
    int16 rem;                          /* 0x16, the fraction of a last step */
    int16 dt;                           /* 0x18, time units per step */
    int16 done;                         /* 0x1A, steps taken; set past steps to stop */
    signed char hit;                    /* 0x1C, DS:416, the oCollisions index at targz,
                                           or -1 */
    int16 item;                         /* 0x1D, DS:417, that object's item */
    int16 targz;                        /* 0x1F, the height the mover stops at in z */
    int16 f21;                          /* 0x21, not used in MOTION.C */
    int16 f23;                          /* 0x23, the lowest bottom of an object overhead */
    int16 zspeed;                       /* 0x25, z change per whole step, in 1/64 of a
                                           z unit (full_move adds zspeed << 5 to frac) */
    uint16 headings[8];                 /* 0x27, wall normals, by ComputeHeading's
                                           direction */
};
HOST_LAYOUT_END

/* OBJPHYS.C: object physics */
void far get_phys_data(struct Object far *obj, struct Phys *pp);
unsigned char far set_phys_data(struct Object far *obj, struct Phys *pp);
struct Object far * far static_to_mob(struct Object far *obj);
void far mob_init(struct Object far *obj, int x, int y);
struct Object far * far mob_to_static(struct Object far *obj);
void far update_hack_vecs(struct Phys *pp);
void far missile_newhit(struct Object far *proj, struct Object far *hit);
int far do_objhit(int ci, int index);
struct Object far * far obj_deal(struct Object far *obj, int x, int y, char how);

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
struct Object far * far IsaDoor(unsigned char *x, unsigned char *y);
struct Object far * far CollObject(void);
unsigned char far set_resterr(unsigned bits);

/* PlayerInput, the movement command (PHYSICS.C's do_player_input carries it out;
   PLAYMOVE.C's parse_playin and do_player_keyboard set it from the mouse and keys). The
   names are ours, from do_player_input. */
#define PIN_NONE        0               /* stop */
#define PIN_FORWARD     1               /* forward and turn at ForwInpRate, TurnInpRate */
#define PIN_RUN_JUMP    6               /* a running jump */
#define PIN_JUMP        7
#define PIN_BACK        8
#define PIN_LEFT        9               /* sideways, a quarter turn left */
#define PIN_RIGHT       10
#define PIN_UP          12              /* while levitating or flying */
#define PIN_DOWN        13

/* PLAYMOVE.C: player input and motion */
extern unsigned char pmouseHandled;
extern unsigned char combEfflen;
extern unsigned char tremEfflen;
extern int16 PlayerInput;
extern int16 PlayerTurn;
extern char MoveCamera;
extern int16 ForwInpRate;
extern int16 TurnInpRate;
extern int16 PlayerBank;
extern int16 campos[3];
extern int16 camang[3];
extern int16 vort_theta;
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
extern int16 playerMod[4];
extern int16 PlayerPitch;
extern int16 vort_rad;
extern int16 vort_timer;
void far set_effect(unsigned char which, char amount);

#endif
