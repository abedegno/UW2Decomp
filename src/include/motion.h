/* motion.h: Motion and physics: moving objects through the tile grid, collisions, and the
   clock that drives them. */
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
   seg031's, which steps them through the tile grid through the near pointer CP. */
struct Phys {
    int x, y, z;                        /* 0x00, x and y in 1/256 tiles, z in 1/8 */
    int vel[3];                         /* 0x06 */
    int acc[3];                         /* 0x0C, acc[2] is gravity */
    int time;                           /* 0x12 */
    int speed;                          /* 0x14 */
    unsigned char bounce;               /* 0x16 */
    unsigned char flags;                /* 0x17 */
    int mass;                           /* 0x18 */
    unsigned char light;                /* 0x1A */
    unsigned char hp;                   /* 0x1B */
    unsigned char resist;               /* 0x1C */
    unsigned char b1D;                  /* 0x1D */
    int heading;                        /* 0x1E */
    int index;                          /* 0x20 */
    unsigned char radius;               /* 0x22 */
    unsigned char height;               /* 0x23 */
    unsigned char b24;                  /* 0x24 */
    unsigned char terrain;              /* 0x25, one bit per terrain type */
    unsigned impact;                    /* 0x26 */
};

/* A mover's handler, 12 bytes, reached through the near pointer TP while seg031 moves a
   physics record: which collision bits to ignore, which to pass to special, and which
   stop it climbing onto objects. The player's is PT, the critters' CT1..CT4. */
struct Handler {
    unsigned ignore;                    /* 0x00 */
    unsigned mask;                      /* 0x02 */
    unsigned noclimb;                   /* 0x04 */
    unsigned w6;                        /* 0x06 */
    unsigned char (far *special)();     /* 0x08, called with the collision state's address
                                           when state & mask */
};

/* The stepping state: a Bresenham walk along the major axis of the velocity. FM Towns's
   _MP, one initialised struct at DS:3FA (FM Towns reads DS:416 and DS:417 as _MP+0x20 and
   _MP+0x21, so they are fields, not separate globals). */
struct MotionParams {
    int *vel;                           /* 0x00, CP->vel */
    int *pos;                           /* 0x02, Ppd's x, y and z */
    int frac[3];                        /* 0x04, the fraction of pos, 0x2000 to a unit */
    int step[3];                        /* 0x0A */
    int major;                          /* 0x10 */
    int minor;                          /* 0x12 */
    int steps;                          /* 0x14 */
    int rem;                            /* 0x16 */
    int dt;                             /* 0x18 */
    int done;                           /* 0x1A */
    signed char hit;                    /* 0x1C, DS:416 */
    int item;                           /* 0x1D, DS:417 */
    int targz;                          /* 0x1F */
    int f21;                            /* 0x21 */
    int f23;                            /* 0x23 */
    int zspeed;                         /* 0x25 */
    unsigned headings[8];               /* 0x27 */
};

/* SEG030.C: object physics */
void far get_phys_data(struct Object far *obj, struct Phys *pp);
unsigned char far set_phys_data(struct Object far *obj, struct Phys *pp);
struct Object far * far static_to_mob(struct Object far *obj);
void far mob_init(struct Object far *obj, int x, int y);
struct Object far * far mob_to_static(struct Object far *obj);
void far update_hack_vecs(struct Phys *pp);
void far missile_newhit(struct Object far *proj, struct Object far *hit);
int far do_objhit(int ci, int index);

/* SEG031.C: motion */
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

/* SEG035.C: player input and motion */
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
