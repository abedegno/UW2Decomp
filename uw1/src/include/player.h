/* player.h: The player: skills and levelling, timed updates, set-up, character creation,
   the player record, and the player's physics. */
#ifndef PLAYER_H
#define PLAYER_H

#include "uw2.h"

struct Creature;
struct Object;
struct Player;
struct Tile;

#include "critter.h"
#include "map.h"
#include "object.h"

/* UW1's player record, 0xD2 bytes, saved in PLAYER.DAT and reached through the near
   pointer player (PLAYER.C's PlayerDat holds it). Reconciled from the bytes of the 30
   files that use it: every field below is at the offset, width and signedness the code
   compiled from. Fields 0x00..0x5D are laid out as in UW2; from 0x5E on UW1 packs its
   flags far tighter (UW2's lefty at 0x65 is UW1's 0x64, its game clock 0x369 UW1's 0xCE).
   Unsigned bitfield runs take a byte at a time (the byte holding the next free bit), so
   the 0x64 run is one byte and quests follows at 0x65. Names are provisional, taken from
   the sources that use the fields. */
struct Player {
    char name[0x1E];                    /* 0x00 */
    unsigned char strength;             /* 0x1E */
    unsigned char dexterity;            /* 0x1F */
    unsigned char intelligence;         /* 0x20 */
    unsigned char skills[20];           /* 0x21, enum Skill */
    unsigned char health;               /* 0x35 */
    unsigned char maxhealth;            /* 0x36 */
    unsigned char play_mana;            /* 0x37 */
    unsigned char max_mana;             /* 0x38 */
    unsigned char hunger;               /* 0x39 */
    unsigned char fatigue;              /* 0x3A */
    unsigned char food_heal;            /* 0x3B */
    unsigned char b3C;                  /* 0x3C */
    unsigned char level;                /* 0x3D */
    uint16 spells[3];                   /* 0x3E, the active spells */
    unsigned char runebag[3];           /* 0x44, a bit per rune */
    unsigned char shelf[3];             /* 0x47, the runes on the shelf */
    uint16 weight;                      /* 0x4A */
    uint16 max_weight;                  /* 0x4C */
    uint32 exp;                         /* 0x4E, in tenths */
    unsigned char skill_points;         /* 0x52 */
    unsigned char skill_points_earned;  /* 0x53 */
    int16 saved_x;                      /* 0x54 */
    int16 saved_y;
    int16 saved_z;
    int16 saved_facing;                 /* 0x5A */
    int16 saved_level;                  /* 0x5C */
    unsigned char moonstone:4;          /* 0x5E: the level the moonstone is on */
    unsigned char tree:4;               /* the silver tree's level, 0 for none */
    uint16 b5F_0:1;                     /* word 0x5F */
    uint16 drawn:1;                     /* the weapon is drawn */
    uint16 poison:4;
    uint16 active_spells:4;             /* bits 6-9 */
    uint16 nrunes:2;                    /* 0x60, bits 2-3 */
    uint16 armageddon:1;                /* bit 4: Armageddon cast (clearobj on arrival) */
    uint16 orb:1;                       /* bit 5: the orb is destroyed */
    uint16 key:1;                       /* bit 6: the key of truth given */
    uint16 cup:1;                       /* bit 7: the cup of wonder found */
    uint16 incense:2;                   /* 0x61 */
    uint16 shrooms:2;
    uint16 drunk:6;                     /* word 0x61, bits 4-9 */
    uint16 talisman_ok:1;               /* 0x62, bit 2 */
    uint16 garamon:1;                   /* bit 3: Garamon is buried */
    uint16 maze:1;                      /* bit 4: the maze navigation spell */
    uint16 b62_5:3;
    unsigned char light;                /* 0x63 */
    uint16 lefty:1;                     /* 0x64 (the run takes one byte) */
    uint16 female:1;
    uint16 body:3;
    uint16 pclass:3;                    /* enum PlayerClass */
    int32 quests;                       /* 0x65: quests 0..31, a bit each */
    unsigned char quest_bytes[4];       /* 0x69: quests 32..35 */
    unsigned char talismans;            /* 0x6D: talismans left, 0xFF once the last went */
    uint16 dreams;                      /* 0x6E: the dreams seen, a bit each */
    unsigned char game_vars[0x40];      /* 0x70: the game variables (x_traps) */
    unsigned char saved_mana;           /* 0xB0: the maximum mana saved on level 7 */
    unsigned char bB1[3];               /* 0xB1 */
    unsigned char easy;                 /* 0xB4 */
    uint16 sound:2;                     /* 0xB5 */
    uint16 music:2;
    uint16 detail:4;
    uint16 fps:3;                       /* word 0xB6 */
    uint16 terrain:8;                   /* word 0xB6, bits 3-10: PN.terrain, saved */
    uint16 bB7_3:5;
    unsigned char motion_state;         /* 0xB8 */
    unsigned char swim_count;           /* 0xB9 */
    unsigned char crithit;              /* 0xBA */
    unsigned char typehit;              /* 0xBB */
    int32 crithittime;                  /* 0xBC */
    unsigned char hitx;                 /* 0xC0 */
    unsigned char hity;                 /* 0xC1 */
    unsigned char lore[8];              /* 0xC2, the lore skill by level */
    unsigned char bCA[4];               /* 0xCA */
    uint32 game_clock;                  /* 0xCE */
};

/* The player's skills, struct Player's skills[]: string block 2 names them from string
   0x1F in this order (SKILLS.C and ovr158 print skill + 0x1F), and player.h's own
   comments on struct Player agree (search 11, track 12, sneak 13, acrobat 17). */
enum Skill {
    SKILL_ATTACK, SKILL_DEFENSE, SKILL_BAREHAND, SKILL_SWORD, SKILL_AXE, SKILL_MACE,
    SKILL_MISSILE, SKILL_MANA, SKILL_LORE, SKILL_CASTING, SKILL_TRAPS, SKILL_SEARCH,
    SKILL_TRACK, SKILL_STEALTH, SKILL_REPAIR, SKILL_CHARISMA, SKILL_PICKLOCK,
    SKILL_ACROBAT, SKILL_APPRAISE, SKILL_SWIMMING,
    NUM_SKILLS
};

/* The player's class, struct Player's pclass: string block 2 names them from string
   0x17 (ovr158 and SKILLS.C print pclass + 0x17). */
enum PlayerClass {
    PCLASS_FIGHTER, PCLASS_MAGE, PCLASS_BARD, PCLASS_TINKER, PCLASS_DRUID,
    PCLASS_PALADIN, PCLASS_RANGER, PCLASS_SHEPHERD
};

/* SKILLS.C: skills, levelling, sleep, eating, death and traps (ovr154) */
char far player_use_skill(int skill);
void far player_compute(char restore);
void far add_to_skill(int skill);
char far get_skill(char skill);
void far player_sleep(int how);
void far player_key_sleep(int bedroll);
void far do_mstone(void);
void far player_is_dead(void);
int far DetectedTrap(struct Object far *obj, int skill);
int far RemoveTrap(struct Object far *obj, int skill);
extern char EndGameMode_dseg_1C8F;
void far check_victory(void);
char far plant_seed(void);

/* SKILLCHK.C: skill checks, experience and levelling */
void far panel_check_hpmp(void);
int far skill_check(int value, int target);
void far player_get_exp(int n);

/* PLAYTIME.C: the player's timed updates */
char far DegradeLights(int amount, unsigned char counter);
void far sink_sink_sink(void);
char far dispel_spell(int16 *i);
void far duration_check(void);
char far set_curmagic(unsigned char cls, unsigned char sub, unsigned char stability);

/* PLAYER.C: setting up the player */
extern unsigned char MoveCrits;
extern uint32 nextstep;
extern uint32 watertime;
extern int16 PHgt;
extern int16 PLeft;
extern int16 curvrad;
extern int16 PBot;
extern int16 PWid;
void far chg_plyp(int step);
void far show_version(void);
void far report_loc(void);
void far init_player(void);
void far mous_player(int left, int bot, int wid, int hgt);
void far demous_player(void);
void far home_cam(int index);
void far move_cam(int how);
void far attach_eye(int mode);
void far release_camera(int index);
void far crystal_ball(struct Object far *obj, int x, int y);
/* The player record's storage as its users see it (player points at it). PLAYER.C defines
   PlayerDat as unsigned char[0xD2] and the files that reach it by name declare it
   themselves as this union, so it is in no header. */
union PlayerStore {
    struct Player rec;
    char bytes[0xD2];
};

/* CHARGEN.C: character creation */
char far create_player(void);
void far init_char(char blank);

/* PLAYDATA.C: the player record's load and save, and spell effects */
extern int16 player_name_handle;
extern struct Player *player;
extern struct Creature *playerdat;
extern struct Object far *ThePlayer;
extern int16 PlayerLevel;
extern int16 PlayerFacing;
extern int16 PlayerHeading;
void far save_player_data(int fd);
void far read_player_data(int fd);
void far parse_aspells(unsigned char *out);
void far FixPlayerEquips(void);
extern unsigned char plyNotice[2];
void far set_drugged(char on);
extern char DragonSkinBoots_dseg_5c99_1B01;
void far set_maze(char on);

/* PHYSICS.C: the player's physics */
extern int16 GrSq;
/* name: IDA's StopPlayerMotion. Named from FM Towns: player_sqhandler_ follows player_newsq_
   there and is the same test (bit 0x1000, no pitch, slow, lastTerr & 0xA) clearing PN+6
   and PN+8; player_setup stores it as PT's handler, as FM Towns does. */
char far player_sqhandler(uint16 *w);
/* name: IDA's CalculateMotionFromCommand. Named from FM Towns: do_player_input_ is next there,
   is called by set_player_phys_params_ with PlayerInput as here, and has the same
   14-entry switch on the input. */
void far do_player_input(int input, int rate, int16 *speed);
extern unsigned char motionbits;
extern unsigned char fiz_update;
char far simple_fizix(int turn);
void far set_player_phys_params(int rate);
void far phys_affect_player(void);
void far phys_bounce_up(struct Object far *obj);
void far fizix_update(void);
extern int16 pFPS[3];
void far parse_player_terr(int terr, char force);
void far newFPS(char state);
void far EtherealVoidSpecialEffects_seg008_150(void);
void far QuakeTrap_seg008_DE7(int type, int intensity);

/* Defined where no source has it yet: data the link takes from the EXE. */
extern struct Object far *curelem;

#endif
