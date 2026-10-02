/* combat.h: Combat, missiles and spells. */
#ifndef COMBAT_H
#define COMBAT_H

#include "uw2.h"

struct Object;
struct Spell;
struct Tile;

#include "map.h"
#include "object.h"

/* One spell's runes, 4 bytes. */
/* Spell classes, struct Spell's cls >> 3: do_spell's switch (ovr156) sends each class to
   the function FM Towns names. Classes 0 to 3 start an active spell (set_curmagic); their
   effects are PLAYDATA.C's player_affected_by. */
#define SPELLC_LIGHT    0               /* the light level */
#define SPELLC_MOTION   1               /* leap, slow fall, levitate, water walk, fly, bounce */
#define SPELLC_ARMOUR   2               /* armour at every hit location */
#define SPELLC_PROTECT  3               /* stealth, resistances, valor, poisoned weapon */
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

/* COMBAT.C: combat */
int far check_ammo(int weapon);
void far clear_fight_state(void);
char far critter_attack(struct Object far *npc, int swing, unsigned char charge, int type,
                        int poison);
extern signed char cmbModTH[4];
extern unsigned char using_altaras_dagger;
void far player_attack(int swing);
void far missile_thwack(int attacker, struct Object far *missile, struct Object far *def, int x, int y, int dmg, unsigned char type);
void far player_killed_a(struct Object far *npc);

/* MISSILE.C: missiles */
struct Object far * far missile_fire(void);
unsigned char far push_missile(struct Object far *proj, struct Object far *src, char launch);
void far player_fire(int weapon);
void far critter_fire(struct Object far *who, int item, int type);
char far spell_fire(struct Object far *who, int spell);
char far ReturnObject(struct Object far *obj, char message);
void far trap_fire(struct Object far *trap, int x, int y);

/* SPELLS.C: casting spells */
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
unsigned char far anti_magic_p(int x, int y);
/* An area spell's action on one square (gronk_area, process_area). */
typedef char (far *SpellFn)(int x, int y, struct Object far *target, struct Tile far *tile,
                            unsigned char src);
void far gronk_area(struct Object far *who, char count, SpellFn fn, unsigned char type, unsigned char dist, unsigned char radius);
/* What gronk_whoami (and gronk_race) do to each critter they find. */
typedef char (far *WhoamiFn)(struct Object far *npc, int arg);
void far gronk_whoami(int whoami, unsigned char all, int arg, WhoamiFn fn);
void far process_area(char count, unsigned char src, SpellFn fn, unsigned char type, char x0, char y0, char w, char h);
extern struct Spell far spells[69];

/* SPELLS2.C: spells */
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
char far sp_study_monster(struct Object far *caster, struct Object far *target);

/* RUNES.C: the rune bag and casting from runes */
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
