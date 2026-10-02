/* event.h: World events: teleports and quest hacks, SCD scheduled events, triggers and
   traps. */
#ifndef EVENT_H
#define EVENT_H

#include "uw2.h"

struct Object;
struct SCDRow;
struct Tile;

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
        unsigned critters;              /* 0x05 */
        unsigned char arg[9];           /* 0x07: the goal and target; the attitude */
    } npc;
    struct {                            /* 2 teleport */
        unsigned char x, y;             /* 0x05, the destination */
        unsigned critters;              /* 0x07 */
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
        unsigned long values;           /* 0x06: the floor to change, its new texture, and
                                           (high word) the height to add */
    } freeze;
    struct {                            /* 9 set a variable */
        unsigned var;                   /* 0x05 */
        unsigned char op;               /* 0x07 */
        unsigned value;                 /* 0x08 */
    } trapvar;
    struct {                            /* 10 test variables */
        unsigned var;                   /* 0x05, the first */
        unsigned char count;            /* 0x07 */
        unsigned char op;               /* 0x08, how to combine them */
        unsigned char invert;           /* 0x09 */
        int value;                      /* 0x0A, what to compare with */
    } checkvar;
};
struct SCDRow {
    unsigned time;                      /* 0x00 */
    unsigned char level;                /* 0x02: 0xFF any level, 0xF6 + n world n */
    unsigned char once;                 /* 0x03, delete the row once it has run */
    signed char event;                  /* 0x04, negative to skip the row */
    union SCDParams p;                  /* 0x05 */
};

/* WORLDEV.C: world events */
unsigned char far in_arena(int x, int y);
void far stop_and_talk(struct Object far *obj);
void far call_out_the_guards(int home_x, int home_y);
unsigned char far is_my_race(struct Object far *obj, int race);
void far fire_trigger_at(int x, int y);
void far genocide(int race);
void far arena_player_runs(void);
unsigned char far vend_check_gold(char x, char y, unsigned char money, unsigned char check);
void far pass_time(long seconds);
void far do_change_grokking(struct Object far *trap, struct Object far *link, int x, int y);
int far whack_thing(int index, int damage, int how, int extra);
int far inanimate_spell(int x, int y, struct Object far *trap, struct Object far *who, int major,
                        int effect);
unsigned char far go_fish(void);
void far player_did_bad(int owner);
void far eight_pos_switch(int flags, struct Object far *trap, int x, int y);
void far toggle_object_height(struct Object far *trap, char multiple);
void far do_sfx(int type, int arg);
void far clear_all_loretries(void);
void far toggle_pillars_hack(int x, int y, int lower, int upper, int mask);
void far move_folks_around(void);
void far courtyard_hacking(int x, int y, struct Object far *trap);
void far skup_ductosnore(void);
void far standing_wave(int x, int y, int owner);
void far cycle_floor(int x, int y, int width, int height, int first, int last, char wall);
void far do_graffiti(int x, int y, int owner);
void far reset_arrow_pillars(int x, int y, char owner);
void far change_weapon_playerbest(int x, int y, int owner);
void far check_fraznium(int x, int y, unsigned char owner);
void far switch_flip_hack(int x, int y, int owner);
void far play_with_switches(int x, int y);
void far recharge_lightbulbs(int x, int y);
void far redeem_all_bottles(int x, int y);
void far find_and_gronk_force_field(int x, int y);
unsigned char far destroy_floatskull(struct Object far *obj);
void far prison_alarm_check(void);
unsigned char far transform_creature(struct Object far *obj, int item, int whoami, int powerful,
                                     int attitude);
int far black_gem_trip(void);
void far black_gem_rotate(void);
void far do_qbert(int owner);
void far ruin_cure_potions(int x, int y);
void far remove_TK_wand(void);
void far go_vend(int which, int machine, int x, int y, int choice);
void far put_player_in_jail(void);

/* SCDEVENT.C: SCD event handling */
void far gronk_race(int race, unsigned char loop, int param,
                    char (far *code)(struct Object far *, int));
unsigned char far player_looking(int x, int y);
char far gronkify_attitude(struct Object far *npc, int attitude);
char far Sched_DoEvent(unsigned char far *row);

/* TRIGGER.C: triggers and traps */
extern unsigned char Triggers[16];
void far update_pplate(struct Object far *trig);
void far delete_trap(union Link far *head, struct Object far *trap);
int far SetOffTrap(struct Object far *who, struct Object far *context, struct Object far *trap,
                   int x, int y);
int far do_math_op(int value, int op, int right);
int far get_numbered_variable(int index);
extern unsigned char tile_walls[16];
extern int trap_teleport_data;
extern struct Tile far *TriggerChainTileData_dseg_67d6_1BB9;  /* name: FM Towns: map_sq */
struct Object far * far place_bridge(int x, int y, int zarg, int headingarg);
void far destroy_bridge(int x, int y, int zarg, int headingarg);
void far check_for_sunken_moongate(void);
int far do_trap_hack(struct Object far *trap, int x, int y);
char far dont_create_wandering_monster_here(struct Object far *obj);
unsigned char far eligible_castle_monster(int npc);
void far do_ice_hack(struct Object far *trap);
void far DoWanderingMonsters(unsigned char is_player);
void far DoClosingDoors(unsigned char is_player);
int far Ply_Weight(void);

/* SCHEDULE.C: SCD schedules */
unsigned char far Sched_Insert(struct SCDRow far *row, unsigned char run);
unsigned char far Sched_Load(unsigned char block);
unsigned char far Sched_Save(unsigned char block);
void far Sched_SetBuf(int ofs, int seg);
/* match: declared before the rest of its file because TLINK numbers the overlay's stub
   entries in the order Turbo C lists the publics, which for names with the same hash key
   is the order they were first seen: the EXE's stub has Sched_IncrTime before
   Sched_WrapTime. */
unsigned char far Sched_IncrTime(unsigned n, unsigned char mode);
unsigned char far Sched_WrapTime(unsigned time, unsigned span, unsigned char mode);

#endif
