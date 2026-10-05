# UW1's player record, struct Player in player.h, reconciled from the 29 local copies the
# matched sources declared (struct Player1Pan, Player1Scr, UW1PlayerHand ...: before this
# step each file renamed its copy to the tag Player1 for structrec.py). The canonical text
# below has the tag Player1 for the conversion; RETAG gives the files struct Player.
KIND = 'struct'; TAG = 'Player1'; HEADER = 'player.h'; RETAG = 'Player'; EMIT = False
TEXT = r'''
struct Player1 {
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
'''
# BABLHACK's skills[] runs to 0x63 (its get_quest-style reads index past the 20 skills);
# CHARGEN declares quests unsigned and WORLDEV game_clock signed: the gate decides whether
# the canonical type compiles the same in them.
OVERRIDES = {'BABLHACK.C': {'skills': 'skills'}, 'CHARGEN.C': {'quests': 'quests'},
             'WORLDEV.C': {'game_clock': 'game_clock'}}
PREFER = []
