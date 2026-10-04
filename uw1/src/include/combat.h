/* combat.h: Combat, missiles and spells: the header of src/combat but DAMAGE.C (whose
   prototypes and damage types are in object.h). The spell table's record and classes, the
   area spell modes, the rune counts, and the prototypes of COMBAT.C, MISSILE.C, SPELLS.C
   and RUNES.C. Function names are UW2's FM Towns ones through UW2Decomp, the routines
   being the same; the constants are named from the code that dispatches on them. */
#ifndef COMBAT_H
#define COMBAT_H

#include "uw2.h"

struct Object;
struct Spell;
struct Tile;

#include "map.h"
#include "object.h"

/* Spell classes, struct Spell's cls >> 3: do_spell's switch (SPELLS.C) sends each class to
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
#define SPELLC_SPECIAL  13              /* in do_spell itself: the bullfrog, hallucination */
#define SPELLC_CUTSCENE 14              /* runcutscene */

/* A spell's minor (struct Spell's sub, do_spell's sub): bits 0-5 the minor itself, bits 6
   and 7 flags, passed to set_curmagic for the active spell classes and the target mode for
   the area spells (process_area's type). */
#define SPELL_MINOR     0x3F
#define SPELL_FLAGS     0xC0
#define AREA_CRITTERS   0x00            /* every critter but the caster */
#define AREA_RANDOM     0x40            /* random open squares */
#define AREA_OBJECTS    0x80            /* every object */
#define AREA_ALL        0xC0            /* every object */
/* The spell table, spells[] (SPELLS.C; cast's spell numbers). */
#define NUM_SPELLS      53
/* The spells a player can cast from runes: spells[0..0x2F], eight circles of six (RUNES.C's
   try_cast searches them; player_cast's circle is idx / 6 + 1). */
#define NUM_RUNE_SPELLS 0x30

#define NUM_RUNES       0x18            /* An to Ylem, items FIRST_RUNESTONE on */
#define RUNE_NONE       0x18            /* an empty place on the rune shelf */

/* One entry of spells[], 4 bytes. */
struct Spell {
    unsigned char cls;                  /* class in bits 3-7 */
    int16 runes;                        /* the three runes, 5 bits each */
    unsigned char sub;
};
/* A spell's class (SPELLC_*), from struct Spell's cls. */
#define SPELL_CLASS(s)  (((s).cls & 0xF8) >> 3)

/* COMBAT.C: combat */
int far check_ammo(int weapon);
void far clear_fight_state(void);
char far critter_attack(struct Object far *npc, int swing, unsigned char charge, int type,
                        int poison);
extern signed char cmbModTH[4];
void far player_attack(int swing);
void far missile_thwack(int attacker, struct Object far *missile, struct Object far *def, int x, int y, int dmg, unsigned char type);
void far player_killed_a(struct Object far *npc);

/* MISSILE.C: missiles */
struct Object far * far missile_fire(void);
char far push_missile(struct Object far *proj, struct Object far *src);
void far player_fire(int weapon);
void far critter_fire(struct Object far *who, int item, int type);
char far spell_fire(struct Object far *who, int spell);
char far ReturnObject(struct Object far *obj, char message);
void far trap_fire(struct Object far *trap, int x, int y);

/* SPELLS.C: casting spells */
extern unsigned char inanmMapX;
extern unsigned char inanmMapY;
extern unsigned char mspell_mused;
void far restore_mana(struct Object far *who, char amount);
void far healing(struct Object far *who, char sub);
void far backfire(struct Object far *who, char sub);
void far release_missile(struct Object far *who, char sub);
void far nail_area(struct Object far *who, unsigned char sub);
void far nail_1area(struct Object far *who, unsigned char sub);
void far damage_square(int x, int y, unsigned char kind, unsigned char src);
char far do_spell(unsigned char cls, unsigned char sub, struct Object far *who,
                  struct Object far *target);
void far cast(unsigned char spell, struct Object far *who, struct Object far *target);
void far restore_hp(struct Object far *who, char amount);
void far get_hp_back(struct Object far *who, unsigned char amount);
struct Object far * far build_new_obj(int item, struct Tile far *tile);
char far sp_ward_undead(int x, int y, struct Object far *target, struct Tile far *tile,
                        unsigned char src);
char far hit_critter_goal(char goal, char attitude, struct Object far *npc, int x, int y);
char far sp_hold(int x, int y, struct Object far *target, struct Tile far *tile, unsigned char src);
void far obj_spells(struct Object far *target, int how, unsigned char b);
unsigned char far anti_magic_p(int x, int y);
/* An area spell's action on one square (gronk_area, process_area). */
typedef char (far *SpellFn)(int x, int y, struct Object far *target, struct Tile far *tile,
                            unsigned char src);
void far gronk_area(struct Object far *who, char count, SpellFn fn, unsigned char type, unsigned char dist, unsigned char radius);
/* What gronk_whoami (and gronk_race) do to each critter they find. */
typedef char (far *WhoamiFn)(struct Object far *npc, NEARPTR arg);
void far gronk_whoami(int whoami, char all, NEARPTR arg,
                      char (far *fn)(struct Object far *npc, NEARPTR arg));
void far process_area(char count, unsigned char src, SpellFn fn, unsigned char type, char x0, char y0, char w, char h);
char far sp_sheet_light(int x, int y, struct Object far *target, struct Tile far *tile,
                        unsigned char src);
char far sp_meteor(int x, int y, struct Object far *target, struct Tile far *tile,
                   unsigned char src);
char far sp_poison(int x, int y, struct Object far *target, struct Tile far *tile,
                   unsigned char src);
char far sp_charm(int x, int y, struct Object far *target);
char far sp_confusion(int x, int y, struct Object far *target, struct Tile far *tile,
                      unsigned char src);
char far sp_fear(int x, int y, struct Object far *target, struct Tile far *tile, unsigned char src);
void far print_monster(unsigned char dir, unsigned char n);

/* SPELLS.C: the routines UW2 moved to SPELLS2.C */
char far sp_true_sight(struct Object far *caster, struct Object far *target);
char far tremor_area(int x, int y, struct Object far *target, struct Tile far *tile,
                     unsigned char src);
void far creat_spell(struct Object far *caster, char which);
void far mdetect(int dist, int skill);
void far xt_spells(struct Object far *caster, char stab, char sub);

/* RUNES.C: the rune bag and casting from runes */
extern uint32 lstime;
char far add_rune(struct Object far *obj);
void far clear_runes(void);
void far RedispRune(void);
void far clear_shelf(void);
void far mous_in_rune(void);
void far try_clear(void);
char far player_cast(unsigned char idx);
void far try_cast(int how);

#endif
