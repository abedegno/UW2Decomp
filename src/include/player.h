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

/* The player's record, 0x37D bytes, saved in PLAYER.DAT (ovr142 writes it xor-encoded)
   and reached through the near pointer player. Names are provisional, taken from the
   sources that use the fields. */
struct Player {
    char name[0x1E];                    /* 0x00 (ovr101 and ovr158 declare 0x21 bytes);
                                           save_player_data's key is name[0] */
    unsigned char strength;             /* 0x1E */
    unsigned char dexterity;            /* 0x1F */
    unsigned char intelligence;         /* 0x20 */
    unsigned char skills[20];           /* 0x21: attack, defence, ...; [11] search,
                                           [12] track, [13] sneak, [17] acrobat */
    unsigned char health;               /* 0x35 */
    unsigned char maxhealth;            /* 0x36 */
    unsigned char play_mana;            /* 0x37 */
    unsigned char max_mana;             /* 0x38 */
    unsigned char hunger;               /* 0x39 */
    unsigned char fatigue;              /* 0x3A */
    unsigned char food_heal;            /* 0x3B */
    unsigned char b3C;                  /* 0x3C */
    unsigned char level;                /* 0x3D */
    unsigned spells[3];                 /* 0x3E, the active spells */
    unsigned char runebag[3];           /* 0x44, a bit per rune */
    unsigned char shelf[3];             /* 0x47, the runes on the shelf */
    unsigned weight;                    /* 0x4A */
    unsigned max_weight;                /* 0x4C */
    unsigned long exp;                  /* 0x4E, in tenths */
    unsigned char skill_points;         /* 0x52 */
    unsigned char skill_points_earned;  /* 0x53 */
    int saved_x;                        /* 0x54: PN's x, y and z, PlayerFacing and */
    int saved_y;                        /* PlayerLevel, stored here when saving */
    int saved_z;
    int saved_facing;                   /* 0x5A */
    int saved_level;                    /* 0x5C */
    unsigned char moonstones[2];        /* 0x5E, the level each moonstone is on */
    unsigned drawn:1;                   /* word 0x60: the weapon is drawn */
    unsigned poison:4;
    unsigned active_spells:4;
    unsigned nrunes:2;                  /* word 0x61, bits 1-2 */
    unsigned b60_11:1;
    unsigned shrooms:2;
    unsigned drunk:6;                   /* word 0x61, bits 6-11 */
    unsigned automap:1;                 /* word 0x62, bit 4 */
    unsigned b62_5:1;
    unsigned sleepbits:3;               /* word 0x62, bits 6-8 */
    unsigned in_void:1;                 /* word 0x63, bit 1 */
    unsigned in_pits:1;
    unsigned b63_3:5;
    unsigned char light;                /* 0x64 */
    unsigned lefty:1;                   /* 0x65 */
    unsigned female:1;
    unsigned body:3;
    unsigned pclass:3;
    unsigned long quests[32];           /* 0x66, quests 0-127, four to a long */
    unsigned char quest_bytes[16];      /* 0xE6: [0] lines cut, [1] arena wins, [2] key gems,
                                           [3] worlds, [5] Jospur's debt (pit fights won,
                                           unpaid), [6] Killorn's countdown, [7] worms
                                           killed, [13] worlds visited, [15] the pending
                                           cutscene plus one */
    unsigned char bF6;                  /* 0xF6 */
    unsigned char dreamflags;           /* 0xF7 */
    unsigned char map_scrap;            /* 0xF8 */
    unsigned vars[256];                 /* 0xF9, the numbered variables: [0..5] the vending
                                           machines' selections, [6] the last gem,
                                           [100..106] the pyramid's colour sequence, a done
                                           flag and the last trigger tile */
    int saved_automap;                  /* 0x2F9 */
    int dream_x;                        /* 0x2FB */
    int dream_y;                        /* 0x2FD */
    unsigned dream_pos;                 /* 0x2FF, level << 8 plus heading */
    unsigned char easy;                 /* 0x301 */
    unsigned sound:2;                   /* word 0x302 */
    unsigned music:2;
    unsigned detail:4;
    unsigned fps:3;                     /* 0x303 */
    unsigned terrain:8;                 /* word 0x303, bits 3-10: PN.terrain, saved */
    unsigned b304_3:5;
    unsigned char paralyzed;            /* 0x305 */
    unsigned char motion_state;         /* 0x306 */
    unsigned char swim_count;           /* 0x307 */
    unsigned char crithit;              /* 0x308 */
    unsigned char typehit;              /* 0x309 */
    long crithittime;                   /* 0x30A */
    unsigned char hitx;                 /* 0x30E */
    unsigned char hity;                 /* 0x30F */
    unsigned char lore[0x50];           /* 0x310, the lore skill by level */
    unsigned char pit_fighters[5];      /* 0x360 */
    char pad365[0x369 - 0x365];
    unsigned long game_clock;           /* 0x369 */
    unsigned char xclock[16];           /* 0x36D: [0] the day, [1..3] ..., [14] the best
                                           arena record */
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

/* Quest variables 128 and up, struct Player's quest_bytes[] (quest_bytes[n] is quest
   128 + n). Only those whose meaning the Guide to the Ultima Underworlds ("UW2 Quests",
   the PLAYER.DAT table) states and the code agrees with are named. */
#define QB_LINES_OF_POWER 0             /* quest 128: the lines of power cut, a bit each */
#define QB_PIT_RECORD   1               /* quest 129: the win-loss record in the pits */
#define QB_GEMS_USED    2               /* quest 130: a bit per blackrock gem used up
                                           (UseKeyGem sets them) */
#define QB_WORLDS_VISITED 13            /* quest 141: a bit per world visited (the
                                           automap's world list tests them) */

/* The X clocks, struct Player's xclock[] (Guide, "The X Clock"). */
#define XC_TIME         0               /* the time of day in 72 steps (ovr135 and ovr110
                                           set it, the schedules read it) */
#define XC_CASTLE       1               /* the castle plot's progress */
#define XC_GEMS         2               /* Nystrul and the blackrock gems treated */
#define XC_DJINN        3               /* the djinn capture's progress */
#define XC_PIT_KILLS    14              /* most enemies killed in the pits */
#define XC_CHANGED      15              /* counted up when an X clock event happens */

/* SKILLS.C: skills, levelling, sleep, eating, death and traps (ovr154) */
void far punt_void(void);
void far go_void(void);
void far do_gem(void);
char far player_use_skill(int skill);
void far player_compute(char restore);
void far add_to_skill(int skill);
char far get_skill(char skill);
char far grant_skill_advance(int which);
void far player_sleep(int how);
void far player_key_sleep(int bedroll);
void far cs_check(void);
void far do_mstone(void);
void far player_is_dead(void);
int far DetectedTrap(struct Object far *obj, int skill);

/* SKILLCHK.C: skill checks, experience and levelling */
void far panel_check_hpmp(void);
int far skill_check(int value, int target);
void far player_get_exp(int n);

/* PLAYTIME.C: the player's timed updates */
char far DegradeLights(int amount, unsigned char counter);
void far sink_sink_sink(void);
char far dispel_spell(int *i);
void far duration_check(void);

/* PLAYER.C: setting up the player */
extern unsigned char MoveCrits;
extern unsigned long nextstep;
extern unsigned long watertime;
extern int PHgt;
extern int PLeft;
extern int curvrad;
extern int PBot;
extern int PWid;
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

/* CHARGEN.C: character creation */
char far create_player(void);

/* PLAYDATA.C: the player record's load and save, and spell effects */
extern int player_name_handle;
extern struct Player *player;
extern struct Creature *playerdat;
extern struct Object far *ThePlayer;
extern int PlayerLevel;
extern int PlayerFacing;
extern int PlayerHeading;
extern unsigned char light_hi;
extern unsigned char last_light;
void far save_player_data(int fd);
void far read_player_data(int fd);
void far parse_aspells(unsigned char *out);
void far FixPlayerEquips(void);
void far load_dl(void);

/* PHYSICS.C: the player's physics */
extern unsigned char frictionless;
extern int GrSq;
void far change_GrSq(int sq, int z);
void far hgt_change(struct Object far *obj, struct Tile far *tile, int z);
void far player_newsq(int sq);
/* IDA's StopPlayerMotion. Named from FM Towns: player_sqhandler_ follows player_newsq_
   there and is the same test (bit 0x1000, no pitch, slow, lastTerr & 0xA) clearing PN+6
   and PN+8; player_setup stores it as PT's handler, as FM Towns does. */
char far player_sqhandler(unsigned *w);
/* IDA's CalculateMotionFromCommand. Named from FM Towns: do_player_input_ is next there,
   is called by set_player_phys_params_ with PlayerInput as here, and has the same
   14-entry switch on the input. */
void far do_player_input(int input, int rate, int *speed);
extern unsigned char motionbits;
extern unsigned char fiz_update;
char far simple_fizix(int turn);
void far set_player_phys_params(int rate);
void far player_setup(int x, int y, int how);
void far phys_affect_player(void);
void far phys_bounce_up(struct Object far *obj);
void far fizix_update(void);

/* Defined where no source has it yet: data the link takes from the EXE. */
char far GetItemEnchantment(struct Object far *obj, int *major, int *effect, unsigned char *flag);
extern struct Object far *curelem;

#endif
