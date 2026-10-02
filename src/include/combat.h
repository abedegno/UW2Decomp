/* combat.h: Combat, missiles and spells. */
#ifndef COMBAT_H
#define COMBAT_H

#include "uw2.h"

struct MissileInfo;
struct Object;
struct Spell;
struct Tile;
struct Weapon;

#include "map.h"
#include "object.h"

/* A missile weapon, 3 bytes: the ranged weapons table of DATA\OBJECTS.DAT, in Missile[]. */
struct MissileInfo {
    unsigned char damage;               /* 0x00 */
    unsigned char type;                 /* 0x01, the missile type */
    signed char ammo;                   /* 0x02, the ammunition it fires */
};

/* One spell's runes, 4 bytes. */
/* Spell classes, struct Spell's cls >> 3: do_spell's switch (ovr156) sends each class to
   the function FM Towns names. Classes 0 to 3 start an active spell (set_curmagic). */
#define SPELLC_HEAL     4               /* healing */
#define SPELLC_MISSILE  5               /* release_missile, or aim one for the player */
#define SPELLC_AREA     6               /* nail_area */
#define SPELLC_1AREA    7               /* nail_1area */
#define SPELLC_CREATE   8               /* creat_spell */
#define SPELLC_BACKFIRE 9               /* backfire */
#define SPELLC_MANA     10              /* restore_mana */
#define SPELLC_XT       11              /* xt_spells */
#define SPELLC_SPECIAL  13              /* special_spells */
#define SPELLC_CUTSCENE 14              /* show_cutscene */

#define NUM_RUNES       0x18            /* An to Ylem, items FIRST_RUNESTONE on */
#define RUNE_NONE       0x18            /* an empty place on the rune shelf */

struct Spell {
    unsigned char cls;                  /* class in bits 3-7 */
    int runes;                          /* the three runes, 5 bits each */
    unsigned char sub;
};

/* A melee weapon, 8 bytes: the melee weapons table of DATA\OBJECTS.DAT. */
struct Weapon {
    unsigned char damage[3];            /* by swing kind: slash, bash, stab */
    unsigned char min_charge;           /* 0x03 */
    unsigned char speed;                /* 0x04 */
    unsigned char max_charge;           /* 0x05 */
    unsigned char skill;                /* 0x06, the skill it uses */
    unsigned char durability;           /* 0x07 */
};

/* One armour or wearable's properties, 4 bytes, 32 of them from OBJECTS.DAT (UW-Formats,
   "Armour and wearables table"), in ovr120's Armor. */
struct Armour {
    unsigned char protection;           /* 0x00 */
    unsigned char durability;           /* 0x01 */
    unsigned char b2;                   /* 0x02 */
    unsigned char category;             /* 0x03: 0 shield, 1 body armour, 3 leggings,
                                           4 gloves, 5 boots, 8 hat, 9 ring */
};

/* SEG024.C: combat */
int far check_ammo(int weapon);
void far clear_fight_state(void);
char far critter_attack(struct Object far *npc, int swing, unsigned char charge, int type,
                        int poison);

/* SEG027.C: missiles */
struct Object far * far missile_fire(void);
unsigned char far push_missile(struct Object far *proj, struct Object far *src, char launch);
void far player_fire(int weapon);
void far critter_fire(struct Object far *who, int item, int type);
char far spell_fire(struct Object far *who, int spell);
char far ReturnObject(struct Object far *obj, char message);
void far trap_fire(struct Object far *trap, int x, int y);

/* OVR156.C: casting spells */
extern unsigned char inanmMapX;
extern unsigned char inanmMapY;
extern int area_spell_state;
extern unsigned char mspell_mused;
void far restore_mana(struct Object far *who, char amount);
void far healing(struct Object far *who, char sub);
void far backfire(struct Object far *who, char sub);
void far release_missile(struct Object far *who, char sub);
void far nail_area(struct Object far *who, unsigned char sub);
void far nail_1area(struct Object far *who, unsigned char sub);
void far special_spells(struct Object far *who, struct Object far *target, char sub);
void far damage_square(int x, int y, unsigned char kind, unsigned char src);
char far do_spell(unsigned char cls, unsigned char sub, struct Object far *who,
                  struct Object far *target);
void far cast(unsigned char spell, struct Object far *who, struct Object far *target);
void far restore_hp(struct Object far *who, char amount);
void far get_hp_back(struct Object far *who, unsigned char amount);
struct Object far * far build_new_obj(int item, struct Tile far *tile);
char far sp_ward_undead(int x, int y, struct Object far *target, struct Tile far *tile,
                        unsigned char src);
char far hit_critter_goal(char goal, char attitude, int gtarg, struct Object far *npc, int x,
                          int y);
char far sp_hold(int x, int y, struct Object far *target, struct Tile far *tile, unsigned char src);
void far obj_spells(struct Object far *target, int how, unsigned char b);

/* OVR157.C: spells */
void far sp_enchant(struct Object far *obj, unsigned char inv, int x, int y);
char far mendable(struct Object far *obj);
char far sp_true_sight(struct Object far *caster, struct Object far *target);
char far tremor_area(int x, int y, struct Object far *target, struct Tile far *tile,
                     unsigned char src);
void far creat_spell(struct Object far *caster, char which);
void far mdetect(int dist, int skill);
void far xt_spells(struct Object far *caster, char stab, char sub);
char far check_Guardian_magic_marker(int x, int y, struct Object far *obj, struct Tile far *tile,
                                     unsigned char src);
void far thump_your_magic_twanger_froggie(void);

/* OVR123.C: the rune bag and casting from runes */
extern unsigned long lstime;
char far add_rune(struct Object far *obj);
void far clear_runes(void);
void far RedispRune(void);
void far clear_shelf(void);
void far mous_in_rune(void);
void far try_clear(void);
char far player_cast(unsigned char idx);
void far try_cast(int how);

#endif
