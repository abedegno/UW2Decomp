/* event.h: World events: teleports, the hack traps and plot deaths (WORLDEV.C), and
   triggers and traps (TRIGGER.C). The header of src/event: the tile wall table's bits and
   the prototypes of those files. UW1 has no SCD schedules: the SCD records and the
   SCHEDULE.C and SCDEVENT.C sections below are UW2's, inherited with the header, and no
   UW1 source uses them. Names are UW2's FM Towns ones through UW2Decomp, or chosen for
   UW1's stub order (WORLDEV.C, TRIGGER.C). */
#ifndef EVENT_H
#define EVENT_H

#include "uw2.h"

struct Object;
struct SCDRow;
struct Tile;
union Link;

#include "map.h"
#include "object.h"

/* A row of a schedule in SCD.ARK, 16 bytes: an event and when and where it runs. The
   header bytes are Sched_DoEvent's; the parameters are the handlers' in ovr113, with names
   from what each handler does with them (UnderworldGodot's scd.cs reads them the same
   way). Many events act on a set of critters chosen by a word (critters): its low byte
   is gronk_critid's mode (0 by whoami, 1 by race, 2 one object, 3 all), the high byte
   what that mode matches. */
union SCDParams {
    unsigned char b[11];
    struct {                            /* 1 change goal, 3 kill, 8 attitude, 11 remove */
        uint16 critters;                /* 0x05 */
        unsigned char arg[9];           /* 0x07: the goal and target; the attitude */
    } npc;
    struct {                            /* 2 teleport */
        unsigned char x, y;             /* 0x05, the destination */
        uint16 critters;                /* 0x07 */
        unsigned char level;            /* 0x09 */
        unsigned char unseen;           /* 0x0A, move even when the player could see it */
        unsigned char sethome;          /* 0x0B, make the destination its home */
        unsigned char retry;            /* 0x0C, try again this many steps later */
    } teleport;
    struct {                            /* 4 set a quest bit */
        unsigned char quest;            /* 0x05 */
        unsigned char value;            /* 0x06 */
    } qbit;
    struct {                            /* 7 a special case: hack chooses the handler */
        unsigned char hack;             /* 0x05 */
        unsigned char arg[10];          /* 0x06 */
    } hack;
    struct {                            /* hack 3 */
        unsigned char hack;             /* 0x05 */
        uint32 values;                  /* 0x06: the floor to change, its new texture, and
                                           (high word) the height to add */
    } freeze;
    struct {                            /* 9 set a variable */
        uint16 var;                     /* 0x05 */
        unsigned char op;               /* 0x07 */
        uint16 value;                   /* 0x08 */
    } trapvar;
    struct {                            /* 10 test variables */
        uint16 var;                     /* 0x05, the first */
        unsigned char count;            /* 0x07 */
        unsigned char op;               /* 0x08, how to combine them */
        unsigned char invert;           /* 0x09 */
        int16 value;                    /* 0x0A, what to compare with */
    } checkvar;
};
struct SCDRow {
    uint16 time;                        /* 0x00 */
    unsigned char level;                /* 0x02: 0xFF any level, 0xF6 + n world n */
    unsigned char once;                 /* 0x03, delete the row once it has run */
    signed char event;                  /* 0x04, negative to skip the row */
    union SCDParams p;                  /* 0x05 */
};

/* WORLDEV.C: world events */
int far whack_thing(int index, int damage, int how, int extra);
int far inanimate_spell(int x, int y, struct Object far *trap, struct Object far *who, int major,
                        int effect);
unsigned char far go_fish(void);
void far player_did_bad(int owner);
void far eight_pos_switch(int flags, struct Object far *trap, int x, int y);
void far clear_all_loretries(void);
char far find_good_x_and_y(struct Object far *obj, int x, int y, int16 *nx, int16 *ny, char clear);
int far do_teleport(struct Object far *who, int x, int y, int level);
int far change_terrain(int x, int y, int wall, int floor, int height, int type, int dx, int dy, int adjust);
char far death_check(struct Object far *obj, char mode);
void far repair_item(struct Object far *obj, int skill, char who);
void far work_bullfrog_tiles(int mode, int x, int y);
void far emerald_trap(struct Object far *trap, int x, int y);
void far talking_door_trap(struct Object far *trap, int x, int y);
void far do_arial_talking(struct Object far *trap, int x, int y);

/* SCDEVENT.C: SCD event handling */
/* A schedule event's handler (NestedSCDEventCodeJumps_dseg_67d6_1344 and
   SCDEventCodeJumps_dseg_67d6_1364), given the event row. */
typedef char (far *SCDEventFn)(unsigned char far *);

/* TRIGGER.C: triggers and traps */
extern unsigned char Triggers[16];
void far delete_trap(union Link far *head, struct Object far *trap);
int far SetOffTrap(struct Object far *who, struct Object far *context, struct Object far *trap,
                   int x, int y);
/* tile_walls[type]: which sides of a tile of each type (enum TileType) are wall, from the
   table's values and PATHFIND.C's moves (moving east into a tile is blocked by its west
   wall). The 3D view (VIEW3D.C) reads it through trans_grid, by sides relative to the view. */
#define TW_DIAG         0x01            /* a diagonal */
#define TW_WEST         0x02
#define TW_EAST         0x04
#define TW_SOUTH        0x08
#define TW_NORTH        0x10
#define TW_SLOPE        0x20
extern unsigned char tile_walls[16];
int far do_trap_hack(struct Object far *trap, int x, int y);
char far dont_create_wandering_monster_here(struct Object far *obj);
void far DoWanderingMonsters(char is_player);
void far DoClosingDoors(char is_player);
void far trap_init(FILE *handle);
unsigned char near * far trap_class_data(void);
int far UseTrigger(struct Object far *who, struct Object far *start, struct Object far *trig, int type);
int far UseTrap(struct Object far *trap, int x, int y);
void far trap_obj_del(union Link far *head, struct Object far *obj);
char far check_alert(char is_player, int x, int y);
char far gronkify_talkto_player(struct Object far *npc, NEARPTR arg);
int far cast_trap_spell(int x, int y, int sub);

/* SCHEDULE.C: SCD schedules */
/* match: declared before the rest of its file because TLINK numbers the overlay's stub
   entries in the order Turbo C lists the publics, which for names with the same hash key
   is the order they were first seen: the EXE's stub has Sched_IncrTime before
   Sched_WrapTime. */
/* One level's place in a schedule: the time it has reached and the next row to run. */
struct SCDClock { uint16 time, next; };
/* The work area: the migration queue, then the block as stored in SCD.ARK (row count,
   block number, the 80 clocks, the rows). */
struct SCDWork {
    int16 migrations;
    struct SCDRow migrationRecord[16];
    uint16 rows;
    unsigned char block;
    char spare;
    struct SCDClock clocks[80];
    struct SCDRow record[1];
};

#endif
