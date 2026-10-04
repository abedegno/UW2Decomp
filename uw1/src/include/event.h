/* event.h: World events: teleports, the hack traps and plot deaths (WORLDEV.C), and
   triggers and traps (TRIGGER.C). The header of src/event: the tile wall table's bits and
   the prototypes of those files. UW1 has no SCD schedules, so UW2's SCD.ARK records and
   its SCHEDULE.C and SCDEVENT.C declarations are gone from this copy of its header. Names
   are UW2's FM Towns ones through UW2Decomp, or chosen for UW1's stub order (WORLDEV.C,
   TRIGGER.C). */
#ifndef EVENT_H
#define EVENT_H

#include "uw2.h"

struct Object;
struct Tile;
union Link;

#include "map.h"
#include "object.h"

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

#endif
