/* target: ovr106 */
/* opts: -mm -1 -G -O -Y -d */
/* Handing variables to a conversation and taking them back: before a conversation the
   NPC's and the player's state is copied into the conversation's imported variables, and
   afterwards the ones a conversation may change are copied back. The whole of DOS overlay
   ovr106, in original order. Function and global names are the originals from the FM
   Towns symbol table; the source file's own name is not known. */

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0x1E];
    unsigned char b1E;                  /* 0x1E */
    unsigned char b1F;                  /* 0x1F */
    char pad20[0x21 - 0x20];
    unsigned char skills[20];           /* 0x21 */
    char pad1[0x37 - 0x35];
    unsigned char play_mana;            /* 0x37 */
    unsigned char max_mana;             /* 0x38 */
    unsigned char hunger;               /* 0x39 */
    char pad2[0x3D - 0x3A];
    unsigned char level;                /* 0x3D */
    char pad3[0x60 - 0x3E];
    unsigned drawn:1;                   /* 0x60 */
    unsigned poison:4;
    unsigned b60_5:11;
    char pad62[0x64 - 0x62];
    unsigned b64:9;                     /* 0x64 */
    unsigned sex:1;
    unsigned b64_10:3;
    unsigned pclass:3;
    char pad5[0x369 - 0x66];
    unsigned long game_clock;           /* 0x369 */
};

/* A mobile object. The first 8 bytes are shared with static objects. */
struct Object {
    unsigned id;                        /* item id 0-8 (major 6-8, minor 4-5, index 0-3) */
    unsigned pos;                       /* z 0-6, heading 7-9, x 10-12, y 13-15 */
    union {
        unsigned word;                  /* the next object in this list */
        struct { unsigned quality:6, next:10; } f;
    } qn;
    union {
        unsigned word;                  /* the head of the contents list */
        struct { unsigned owner:6, link:10; } f;
    } ol;
    unsigned char hp;                   /* 0x08 */
    char pad09[0x0B - 0x09];
    unsigned goal_word;                 /* 0x0B, goal in bits 0-3, target in bits 4-11 */
    unsigned attitude_word;             /* 0x0D, talked to in bit 13, attitude in 14-15 */
    char pad0F[0x19 - 0x0F];
    unsigned char b19;                  /* 0x19, fed in bit 7 */
    unsigned char whoami;               /* 0x1A */
};

#define OBJ_ID(o)       ((o)->id & 0x1FF)
#define OBJ_INDEX(o)    (((o)->id & 0x3F) >> 0)
#define OBJ_QUALITY(o)  ((o)->qn.f.quality)
#define OBJ_OWNER(o)    ((o)->ol.f.owner)
#define OBJ_GOAL(o)     (((o)->goal_word & 0xF) >> 0)
#define OBJ_GTARG(o)    (((o)->goal_word & 0xFF0) >> 4)
#define OBJ_TALKEDTO(o) (((o)->attitude_word & 0x2000) >> 13)
#define OBJ_ATTITUDE(o) (((o)->attitude_word & 0xC000) >> 14)
#define OBJ_B19_6(o)    (((o)->b19 & 0x40) >> 6)
#define OBJ_FED(o)      (((o)->b19 & 0x80) >> 7)
#define SET_FED(o, v)   ((o)->b19 = (o)->b19 & 0x7F | ((v) & 1) << 7)

/* One critter type's record, 0x30 bytes. */
struct Creature {
    char pad00[4];
    unsigned char avghit;               /* 0x04 */
    unsigned char strength;             /* 0x05 */
    char pad06[0x0D - 0x06];
    unsigned level:4;                   /* 0x0D */
    unsigned b0D_4:12;
    char pad0F[0x13 - 0x0F];
    signed char arms;                   /* 0x13 */
    char pad14[0x2D - 0x14];
    unsigned char b2D:1;                /* 0x2D */
    unsigned char power:7;
    char pad2E[0x30 - 0x2E];
};

extern struct Player near *player;
extern struct Object far *ThePlayer;
extern int PlayerLevel;
extern int player_name_handle;
extern struct Creature Creature[];

void far bab_var(char *name, int *val, int n);
void far bab_var_out(char *name, int *val, int n);
void far change_critter_goal(struct Object far *npc, char goal, int gtarg);
void far player_get_exp(int n);

void far setup_converse_data(struct Object far *npc)
{
    int val;
    struct Creature near *crit;
    int who;

    crit = &Creature[OBJ_INDEX(npc)];
    val = npc->whoami;
    bab_var("npc_whoami", &val, 1);
    val = OBJ_FED(npc) ? 0x10 : 0xC0;
    bab_var("npc_hunger", &val, 1);
    if (crit->avghit != 0)
        val = (npc->hp << 8) / crit->avghit;
    else
        val = 0x80;
    bab_var("npc_health", &val, 1);
    val = npc->hp;
    bab_var("npc_hp", &val, 1);
    val = crit->arms;
    bab_var("npc_arms", &val, 1);
    val = crit->strength + crit->power;
    bab_var("npc_power", &val, 1);
    val = OBJ_GOAL(npc);
    bab_var("npc_goal", &val, 1);
    val = OBJ_GTARG(npc);
    bab_var("npc_gtarg", &val, 1);
    val = OBJ_TALKEDTO(npc);
    bab_var("npc_talkedto", &val, 1);
    val = crit->level;
    bab_var("npc_level", &val, 1);
    val = OBJ_QUALITY(npc);
    bab_var("npc_xhome", &val, 1);
    val = OBJ_OWNER(npc);
    bab_var("npc_yhome", &val, 1);
    who = npc->whoami;
    val = who ? (who + 0x10) | 0xE00 : OBJ_ID(npc) | 0x800;
    bab_var("npc_name", &val, 1);
    if (OBJ_GOAL(npc) == 5 && OBJ_GTARG(npc) == 1)
        val = 0;
    else if (OBJ_B19_6(npc))
        val = 6;
    else
        val = OBJ_ATTITUDE(npc);
    bab_var("npc_attitude", &val, 1);

    crit = &Creature[OBJ_INDEX(ThePlayer)];
    val = player->hunger;
    bab_var("play_hunger", &val, 1);
    if (crit->avghit != 0)
        val = (ThePlayer->hp << 8) / crit->avghit;
    else
        val = 0x80;
    bab_var("play_health", &val, 1);
    val = ThePlayer->hp;
    bab_var("play_hp", &val, 1);
    val = player->skills[0] + player->b1E;
    bab_var("play_arms", &val, 1);
    val = player->b1F + player->play_mana + player->skills[6];
    bab_var("play_power", &val, 1);
    val = player->play_mana;
    bab_var("play_mana", &val, 1);
    val = player->level;
    bab_var("play_level", &val, 1);
    val = PlayerLevel;
    bab_var("dungeon_level", &val, 1);
    val = player->game_clock / 0x3BC4L;
    bab_var("game_time", &val, 1);
    val = player->game_clock / 0x3BC4L % 0x5A0L;
    bab_var("game_mins", &val, 1);
    val = player->game_clock / 0x1502E80L;
    bab_var("game_days", &val, 1);
    val = 0;
    bab_var("new_player_exp", &val, 1);
    val = player->sex;
    bab_var("play_sex", &val, 1);
    val = player->poison;
    bab_var("play_poison", &val, 1);
    val = player->drawn;
    bab_var("play_drawn", &val, 1);
    val = player_name_handle;
    bab_var("play_name", &val, 1);
}

char far update_converse_data(struct Object far *npc)
{
    int val;
    int gtarg;
    char killed;

    killed = 0;
    bab_var_out("npc_hunger", &val, 1);
    SET_FED(npc, val < 0x20 ? 1 : 0);
    bab_var_out("npc_hp", &val, 1);
    npc->hp = val;
    bab_var_out("npc_xhome", &val, 1);
    npc->qn.f.quality = val;
    bab_var_out("npc_yhome", &val, 1);
    npc->ol.f.owner = val;
    bab_var_out("npc_goal", &val, 1);
    bab_var_out("npc_gtarg", &gtarg, 1);
    change_critter_goal(npc, val, gtarg);
    npc->attitude_word = npc->attitude_word & 0xDFFF | 0x2000;
    bab_var_out("npc_attitude", &val, 1);
    if (val > 3) {
        npc->attitude_word = npc->attitude_word & 0x3FFF | 0xC000;
        npc->b19 = npc->b19 & 0xBF | 0x40;
    } else
        npc->attitude_word = npc->attitude_word & 0x3FFF | (val & 3) << 14;
    if (val == 0)
        killed = 1;
    npc->attitude_word = npc->attitude_word & 0xDFFF | 0x2000;
    bab_var_out("play_hunger", &val, 1);
    player->hunger = val;
    bab_var_out("play_hp", &val, 1);
    ThePlayer->hp = val;
    bab_var_out("play_mana", &val, 1);
    player->play_mana = val;
    bab_var_out("play_poison", &val, 1);
    player->poison = val;
    bab_var_out("new_player_exp", &val, 1);
    if (val != 0)
        player_get_exp(val);
    return killed;
}
