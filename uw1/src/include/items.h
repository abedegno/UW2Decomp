/* items.h: object item ids and the classes they fall in.

   An object's item id is the low nine bits of its id word: bits 6-8 the major class,
   bits 4-5 the minor class, bits 0-3 the index within it (UW-Formats, 6). The names of
   the eight major classes are the original ones, from UW2's FM Towns build, which names
   each class's loader and lookup (hack_init and hack_class_data, creature_class_data,
   misc_init and misc_class_data, stuff_class_data, spec_class_data, rect_class_data,
   trap_init and trap_class_data, animobj_load and animobj_class_data); UW1 has the same
   loaders and lookups (OBJCLASS.C's class table, ovr129). The class names (id >> 4) follow
   the tables and Use functions FM Towns names for them (Weapons, Missile, Armor,
   Containers, Lights, Food; UseWand, UseKey, UseUnique, UseMagic, UseBook) and the ranges
   in UW-Formats, 6 (the Underworld Adventures document, which is UW1's).

   The item names are UW1's own: string block 4 of UW1's DATA\STRINGS.PAK, the name
   printed for each item, with the article dropped and a hex suffix where several items
   share a name (the method and scripts are Exhume's examples/uw2/consts/, run on UW1's
   data). Items with an empty name in block 4 have no entry. Hand corrections, as UW2's:
   ITEM_DRAGON_SKIN_BOOTS (the string begins "a pair of"), ITEM_SPELL_TRAP and
   ITEM_PICKUP_TRIGGER (the strings run the words together or apart), ITEM_TMAP_C and
   ITEM_TMAP_S (both strings are the same; the format document names them tmap_c and
   tmap_s). UW1's items are not UW2's: the same id often names another item (0x00A, 0x1C5,
   0x1CB), and UW1 has 17 traps and 7 triggers where UW2 has 25 and 32. */
#ifndef ITEMS_H
#define ITEMS_H

/* Major classes: the id's bits 6-8, (id & ID_MAJOR) >> 6. */
enum ObjMajor {
    MAJOR_HACK,                         /* 0x000-0x03F: weapons, missiles and armour */
    MAJOR_CREATURE,                     /* 0x040-0x07F: creatures, the player 0x7F */
    MAJOR_MISC,                         /* 0x080-0x0BF: containers, lights, wands,
                                           treasure and food */
    MAJOR_STUFF,                        /* 0x0C0-0x0FF: scenery, the keys of Truth,
                                           Love and Courage and runestones */
    MAJOR_SPEC,                         /* 0x100-0x13F: keys, quest items, magic items
                                           and books */
    MAJOR_RECT,                         /* 0x140-0x17F: doors, 3D objects and switches */
    MAJOR_TRAP,                         /* 0x180-0x1BF: traps and triggers */
    MAJOR_ANIMOBJ                       /* 0x1C0-0x1FF: animated objects */
};

/* Classes: the id's bits 4-8, (id & ID_CLASS) >> 4, sixteen items each. */
#define CLASS_WEAPON        0x00        /* melee weapons */
#define CLASS_MISSILE       0x01        /* missiles, then missile weapons from 0x18 */
#define CLASS_ARMOR         0x02        /* body armour, leggings, gloves, boots, helms */
#define CLASS_ARMOR2        0x03        /* crowns, rings and shields: Armor + 16 */
#define CLASS_CREATURE      0x04        /* the first of the four creature classes */
#define CLASS_CONTAINER     0x08
#define CLASS_LIGHT         0x09        /* lights 0-7 (UseLight), wands 8-15 (UseWand) */
#define CLASS_TREASURE      0x0A
#define CLASS_FOOD          0x0B        /* food, drink and the two potions */
#define CLASS_SCENERY       0x0C        /* scenery and junk (UseUtil) */
#define CLASS_SCENERY2      0x0D
#define CLASS_KEY_PARTS     0x0E        /* the blank runestone, the keys 0xE1-0xE7, the
                                           runestones An to Ex */
#define CLASS_RUNESTONE     0x0F        /* the runestones Flam to Ylem */
#define CLASS_KEY           0x10        /* keys, the lockpick and the lock (UseKey) */
#define CLASS_UNIQUE        0x11        /* UseUnique */
#define CLASS_MAGIC         0x12        /* UseMagic */
#define CLASS_BOOK          0x13        /* books, scrolls and the map (UseBook) */
#define CLASS_DOOR          0x14
#define CLASS_FURNITURE     0x15
#define CLASS_DECAL         0x16        /* pillar, lever, switch, bridge, gravestone,
                                           writing, force field, special tmap objects */
#define CLASS_SWITCH        0x17
#define CLASS_TRAP          0x18
#define CLASS_TRAP2         0x19        /* the text string trap, 0x190 */
#define CLASS_TRIGGER       0x1A
#define CLASS_TRIGGER2      0x1B        /* no items in UW1 */
#define CLASS_ANIMOBJ       0x1C

/* Minor classes, OBJ_MINOR: a class's place within its major class (its CLASS_ value's low
   two bits). */
#define MINOR_WEAPON        0           /* MAJOR_HACK: Weapons[] */
#define MINOR_MISSILE       1           /* MAJOR_HACK: Missile[] */
#define MINOR_ARMOR         2           /* MAJOR_HACK: Armor[] */
#define MINOR_ARMOR2        3           /* MAJOR_HACK: Armor[] + 16 */
#define MINOR_CONTAINER     0           /* MAJOR_MISC */
#define MINOR_DOOR          0           /* MAJOR_RECT */
#define MINOR_TRAP          0           /* MAJOR_TRAP: 0 and 1 traps, 2 and 3 triggers */
#define MINOR_TRIGGER       2
#define MINOR_TRIGGER2      3

/* Trap types: a trap's item less FIRST_TRAP, named after UW1's trap items 0x180-0x190
   in string block 4. */
enum TrapType {
    TRAP_DAMAGE,                        /* 0x00 */
    TRAP_TELEPORT,
    TRAP_ARROW,
    TRAP_DO,
    TRAP_PIT,
    TRAP_CHANGE_TERRAIN,
    TRAP_SPELL,
    TRAP_CREATE_OBJECT,
    TRAP_DOOR,
    TRAP_WARD,
    TRAP_TELL,
    TRAP_DELETE_OBJECT,
    TRAP_INVENTORY,
    TRAP_SET_VARIABLE,
    TRAP_CHECK_VARIABLE,
    TRAP_COMBINATION,
    TRAP_TEXT_STRING                    /* 0x10 */
};

/* Trigger types: a trigger's item less FIRST_TRIGGER, named after UW1's trigger items
   0x1A0-0x1A6 in string block 4. */
enum TriggerType {
    TRIG_MOVE,                          /* 0x00 */
    TRIG_PICKUP,
    TRIG_USE,
    TRIG_LOOK,
    TRIG_STEP_ON,
    TRIG_OPEN,
    TRIG_UNLOCK                         /* 0x06 */
};

/* The first item id of each range, for range tests and arithmetic on item ids (UW-Formats,
   6, and the item names). */
#define FIRST_HACK          0x000
#define FIRST_MISSILE       0x010
#define FIRST_MISSILE_WEAPON 0x018
#define FIRST_ARMOR         0x020
#define FIRST_CREATURE      0x040
#define FIRST_MISC          0x080       /* the containers */
#define FIRST_LIGHT         0x090
#define FIRST_LIT_LIGHT     0x094       /* a lit light is its unlit item + 4 */
#define FIRST_WAND          0x098
#define FIRST_BROKEN_WAND   0x09C
#define FIRST_TREASURE      0x0A0
#define FIRST_FOOD          0x0B0
#define FIRST_STUFF         0x0C0
#define FIRST_KEY_PART      0x0E1       /* ITEM_KEY_OF_TRUTH: the keys of Truth, Love
                                           and Courage, the three pieces of the two
                                           part key, the Key of Infinity (0xE1-0xE7) */
#define FIRST_RUNESTONE     0x0E8       /* An; the 24 runes run in alphabetical order */
#define FIRST_SPEC          0x100
#define FIRST_BOOK          0x130
#define FIRST_RECT          0x140       /* the doors */
#define FIRST_OPEN_DOOR     0x148       /* an open door is its closed item + 8 */
#define FIRST_FURNITURE     0x150
#define FIRST_SWITCH        0x170
#define FIRST_TRAP          0x180
#define FIRST_TRIGGER       0x1A0
#define FIRST_ANIMOBJ       0x1C0

/* Weapons (class 0x00), hack_class_data's Weapons table */
#define ITEM_HAND_AXE                    0x000
#define ITEM_BATTLE_AXE                  0x001
#define ITEM_AXE                         0x002
#define ITEM_DAGGER                      0x003
#define ITEM_SHORTSWORD                  0x004
#define ITEM_LONGSWORD                   0x005
#define ITEM_BROADSWORD                  0x006
#define ITEM_CUDGEL                      0x007
#define ITEM_LIGHT_MACE                  0x008
#define ITEM_MACE                        0x009
#define ITEM_SHINY_SWORD                 0x00A
#define ITEM_JEWELED_AXE                 0x00B
#define ITEM_BLACK_SWORD                 0x00C
#define ITEM_JEWELED_SWORD               0x00D
#define ITEM_JEWELED_MACE                0x00E
#define ITEM_FIST                        0x00F

/* Missiles (0x10-0x17) and missile weapons (0x18-0x1F), the Missile table */
#define ITEM_SLING_STONE                 0x010
#define ITEM_CROSSBOW_BOLT_11            0x011
#define ITEM_ARROW_12                    0x012
#define ITEM_STONE                       0x013
#define ITEM_FIREBALL                    0x014
#define ITEM_LIGHTNING_BOLT              0x015
#define ITEM_ACID                        0x016
#define ITEM_MAGIC_MISSILE               0x017
#define ITEM_SLING                       0x018
#define ITEM_BOW                         0x019
#define ITEM_CROSSBOW                    0x01A
#define ITEM_JEWELED_BOW                 0x01F

/* Armour (classes 0x02 and 0x03): body armour, leggings, gloves, boots and
   helms, then crowns, rings and shields; the Armor table */
#define ITEM_LEATHER_VEST                0x020
#define ITEM_MAIL_SHIRT                  0x021
#define ITEM_BREASTPLATE                 0x022
#define ITEM_LEATHER_LEGGINGS            0x023
#define ITEM_MAIL_LEGGINGS               0x024
#define ITEM_PLATE_LEGGINGS              0x025
#define ITEM_LEATHER_GLOVES              0x026
#define ITEM_CHAIN_GAUNTLETS             0x027
#define ITEM_PLATE_GAUNTLETS             0x028
#define ITEM_LEATHER_BOOTS               0x029
#define ITEM_CHAIN_BOOTS                 0x02A
#define ITEM_PLATE_BOOTS                 0x02B
#define ITEM_LEATHER_CAP                 0x02C
#define ITEM_CHAIN_COWL                  0x02D
#define ITEM_HELMET                      0x02E
#define ITEM_DRAGON_SKIN_BOOTS           0x02F
#define ITEM_CROWN_30                    0x030
#define ITEM_CROWN_31                    0x031
#define ITEM_CROWN_32                    0x032
#define ITEM_IRON_RING                   0x036
#define ITEM_SHINY_SHIELD                0x037
#define ITEM_GOLD_RING                   0x038
#define ITEM_SILVER_RING                 0x039
#define ITEM_RED_RING                    0x03A
#define ITEM_TOWER_SHIELD                0x03B
#define ITEM_WOODEN_SHIELD               0x03C
#define ITEM_SMALL_SHIELD                0x03D
#define ITEM_BUCKLER                     0x03E
#define ITEM_JEWELED_SHIELD              0x03F

/* Creatures (major class 1); 0x7F is the player */
#define ITEM_ROTWORM                     0x040
#define ITEM_FLESH_SLUG                  0x041
#define ITEM_CAVE_BAT                    0x042
#define ITEM_GIANT_RAT_43                0x043
#define ITEM_GIANT_SPIDER                0x044
#define ITEM_ACID_SLUG                   0x045
#define ITEM_GOBLIN_46                   0x046
#define ITEM_GOBLIN_47                   0x047
#define ITEM_GIANT_RAT_48                0x048
#define ITEM_VAMPIRE_BAT                 0x049
#define ITEM_SKELETON                    0x04A
#define ITEM_IMP                         0x04B
#define ITEM_GOBLIN_4C                   0x04C
#define ITEM_GOBLIN_4D                   0x04D
#define ITEM_GOBLIN_4E                   0x04E
#define ITEM_GOBLIN_50                   0x050
#define ITEM_MONGBAT                     0x051
#define ITEM_BLOODWORM                   0x052
#define ITEM_WOLF_SPIDER                 0x053
#define ITEM_MOUNTAINMAN_54              0x054
#define ITEM_GREEN_LIZARDMAN             0x055
#define ITEM_MOUNTAINMAN_56              0x056
#define ITEM_LURKER                      0x057
#define ITEM_RED_LIZARDMAN               0x058
#define ITEM_GRAY_LIZARDMAN              0x059
#define ITEM_OUTCAST                     0x05A
#define ITEM_HEADLESS                    0x05B
#define ITEM_DREAD_SPIDER                0x05C
#define ITEM_FIGHTER_5D                  0x05D
#define ITEM_FIGHTER_5E                  0x05E
#define ITEM_FIGHTER_5F                  0x05F
#define ITEM_TROLL                       0x060
#define ITEM_GHOST_61                    0x061
#define ITEM_FIGHTER_62                  0x062
#define ITEM_GHOUL_63                    0x063
#define ITEM_GHOST_64                    0x064
#define ITEM_GHOST_65                    0x065
#define ITEM_GAZER                       0x066
#define ITEM_MAGE_67                     0x067
#define ITEM_FIGHTER_68                  0x068
#define ITEM_DARK_GHOUL                  0x069
#define ITEM_MAGE_6A                     0x06A
#define ITEM_MAGE_6B                     0x06B
#define ITEM_MAGE_6C                     0x06C
#define ITEM_MAGE_6D                     0x06D
#define ITEM_GHOUL_6E                    0x06E
#define ITEM_FERAL_TROLL                 0x06F
#define ITEM_GREAT_TROLL                 0x070
#define ITEM_DIRE_GHOST                  0x071
#define ITEM_EARTH_GOLEM                 0x072
#define ITEM_MAGE_73                     0x073
#define ITEM_DEEP_LURKER                 0x074
#define ITEM_SHADOW_BEAST                0x075
#define ITEM_REAPER                      0x076
#define ITEM_STONE_GOLEM                 0x077
#define ITEM_FIRE_ELEMENTAL              0x078
#define ITEM_METAL_GOLEM                 0x079
#define ITEM_WISP                        0x07A
#define ITEM_ADVENTURER                  0x07F

/* Containers (class 0x08), misc_class_data's Containers table; an open container
   is its closed item + 1 */
#define ITEM_SACK                        0x080
#define ITEM_OPEN_SACK                   0x081
#define ITEM_PACK                        0x082
#define ITEM_OPEN_PACK                   0x083
#define ITEM_BOX                         0x084
#define ITEM_OPEN_BOX                    0x085
#define ITEM_POUCH                       0x086
#define ITEM_OPEN_POUCH                  0x087
#define ITEM_MAP_CASE                    0x088
#define ITEM_OPEN_MAP_CASE               0x089
#define ITEM_GOLD_COFFER                 0x08A
#define ITEM_OPEN_GOLD_COFFER            0x08B
#define ITEM_URN                         0x08C
#define ITEM_QUIVER                      0x08D
#define ITEM_BOWL                        0x08E
#define ITEM_RUNE_BAG                    0x08F

/* Light sources (0x90-0x97, the Lights table; a lit light is its unlit item + 4)
   and wands (0x98-0x9B, broken 0x9C-0x9F) */
#define ITEM_LANTERN                     0x090
#define ITEM_TORCH                       0x091
#define ITEM_CANDLE                      0x092
#define ITEM_TAPER                       0x093
#define ITEM_LIT_LANTERN                 0x094
#define ITEM_LIT_TORCH                   0x095
#define ITEM_LIT_CANDLE                  0x096
#define ITEM_LIT_TAPER                   0x097
#define ITEM_WAND_98                     0x098
#define ITEM_WAND_99                     0x099
#define ITEM_WAND_9A                     0x09A
#define ITEM_WAND_9B                     0x09B
#define ITEM_BROKEN_WAND_9C              0x09C
#define ITEM_BROKEN_WAND_9D              0x09D
#define ITEM_BROKEN_WAND_9E              0x09E
#define ITEM_BROKEN_WAND_9F              0x09F

/* Treasure (class 0x0A) */
#define ITEM_COIN                        0x0A0
#define ITEM_GOLD_COIN                   0x0A1
#define ITEM_RUBY                        0x0A2
#define ITEM_RED_GEM                     0x0A3
#define ITEM_SMALL_BLUE_GEM              0x0A4
#define ITEM_LARGE_BLUE_GEM              0x0A5
#define ITEM_SAPPHIRE                    0x0A6
#define ITEM_EMERALD                     0x0A7
#define ITEM_AMULET                      0x0A8
#define ITEM_GOBLET                      0x0A9
#define ITEM_SCEPTRE                     0x0AA
#define ITEM_GOLD_CHAIN                  0x0AB
#define ITEM_GOLD_PLATE                  0x0AC
#define ITEM_ANKH_PENDANT                0x0AD
#define ITEM_SHINY_CUP                   0x0AE
#define ITEM_LARGE_GOLD_NUGGET           0x0AF

/* Food and drink (class 0x0B), the Food table */
#define ITEM_PIECE_OF_MEAT               0x0B0
#define ITEM_LOAF_OF_BREAD_B1            0x0B1
#define ITEM_PIECE_OF_CHEESE             0x0B2
#define ITEM_APPLE                       0x0B3
#define ITEM_EAR_OF_CORN                 0x0B4
#define ITEM_LOAF_OF_BREAD_B5            0x0B5
#define ITEM_FISH                        0x0B6
#define ITEM_POPCORN                     0x0B7
#define ITEM_MUSHROOM                    0x0B8
#define ITEM_TOADSTOOL                   0x0B9
#define ITEM_BOTTLE_OF_ALE               0x0BA
#define ITEM_RED_POTION                  0x0BB
#define ITEM_GREEN_POTION                0x0BC
#define ITEM_BOTTLE_OF_WATER             0x0BD
#define ITEM_FLASK_OF_PORT               0x0BE
#define ITEM_BOTTLE_OF_WINE              0x0BF

/* Scenery and junk (classes 0x0C and 0x0D) */
#define ITEM_PLANT_C0                    0x0C0
#define ITEM_GRASS                       0x0C1
#define ITEM_SKULL_C2                    0x0C2
#define ITEM_SKULL_C3                    0x0C3
#define ITEM_BONE_C4                     0x0C4
#define ITEM_BONE_C5                     0x0C5
#define ITEM_PILE_OF_BONES_C6            0x0C6
#define ITEM_VINES                       0x0C7
#define ITEM_BROKEN_AXE                  0x0C8
#define ITEM_BROKEN_SWORD                0x0C9
#define ITEM_BROKEN_MACE                 0x0CA
#define ITEM_BROKEN_SHIELD               0x0CB
#define ITEM_PIECE_OF_WOOD_CC            0x0CC
#define ITEM_PIECE_OF_WOOD_CD            0x0CD
#define ITEM_PLANT_CE                    0x0CE
#define ITEM_PLANT_CF                    0x0CF
#define ITEM_PILE_OF_DEBRIS_D0           0x0D0
#define ITEM_PILE_OF_DEBRIS_D1           0x0D1
#define ITEM_PILE_OF_DEBRIS_D2           0x0D2
#define ITEM_STALACTITE                  0x0D3
#define ITEM_PLANT_D4                    0x0D4
#define ITEM_PILE_OF_DEBRIS_D5           0x0D5
#define ITEM_PILE_OF_DEBRIS_D6           0x0D6
#define ITEM_ANVIL                       0x0D7
#define ITEM_POLE                        0x0D8
#define ITEM_DEAD_ROTWORM                0x0D9
#define ITEM_RUBBLE                      0x0DA
#define ITEM_PILE_OF_WOOD_CHIPS          0x0DB
#define ITEM_PILE_OF_BONES_DC            0x0DC
#define ITEM_BLOOD_STAIN_DD              0x0DD
#define ITEM_BLOOD_STAIN_DE              0x0DE
#define ITEM_BLOOD_STAIN_DF              0x0DF

/* The blank runestone, the keys of Truth, Love and Courage, the two part key's pieces,
   the Key of Infinity, and the 24 runestones (classes 0x0E and 0x0F) */
#define ITEM_RUNESTONE                   0x0E0
#define ITEM_KEY_OF_TRUTH                0x0E1
#define ITEM_KEY_OF_LOVE                 0x0E2
#define ITEM_KEY_OF_COURAGE              0x0E3
#define ITEM_TWO_PART_KEY_E4             0x0E4
#define ITEM_TWO_PART_KEY_E5             0x0E5
#define ITEM_TWO_PART_KEY_E6             0x0E6
#define ITEM_KEY_OF_INFINITY             0x0E7
#define ITEM_AN_STONE                    0x0E8
#define ITEM_BET_STONE                   0x0E9
#define ITEM_CORP_STONE                  0x0EA
#define ITEM_DES_STONE                   0x0EB
#define ITEM_EX_STONE                    0x0EC
#define ITEM_FLAM_STONE                  0x0ED
#define ITEM_GRAV_STONE                  0x0EE
#define ITEM_HUR_STONE                   0x0EF
#define ITEM_IN_STONE                    0x0F0
#define ITEM_JUX_STONE                   0x0F1
#define ITEM_KAL_STONE                   0x0F2
#define ITEM_LOR_STONE                   0x0F3
#define ITEM_MANI_STONE                  0x0F4
#define ITEM_NOX_STONE                   0x0F5
#define ITEM_ORT_STONE                   0x0F6
#define ITEM_POR_STONE                   0x0F7
#define ITEM_QUAS_STONE                  0x0F8
#define ITEM_REL_STONE                   0x0F9
#define ITEM_SANCT_STONE                 0x0FA
#define ITEM_TYM_STONE                   0x0FB
#define ITEM_UUS_STONE                   0x0FC
#define ITEM_VAS_STONE                   0x0FD
#define ITEM_WIS_STONE                   0x0FE
#define ITEM_YLEM_STONE                  0x0FF

/* Keys, the lockpick and the lock (class 0x10) */
#define ITEM_KEY_100                     0x100
#define ITEM_LOCKPICK                    0x101
#define ITEM_KEY_102                     0x102
#define ITEM_KEY_103                     0x103
#define ITEM_KEY_104                     0x104
#define ITEM_KEY_105                     0x105
#define ITEM_KEY_106                     0x106
#define ITEM_KEY_107                     0x107
#define ITEM_KEY_108                     0x108
#define ITEM_KEY_109                     0x109
#define ITEM_KEY_10A                     0x10A
#define ITEM_KEY_10B                     0x10B
#define ITEM_KEY_10C                     0x10C
#define ITEM_KEY_10D                     0x10D
#define ITEM_KEY_10E                     0x10E
#define ITEM_LOCK                        0x10F

/* Unique and quest items (class 0x11) */
#define ITEM_PICTURE_OF_TOM              0x110
#define ITEM_CRYSTAL_SPLINTER            0x111
#define ITEM_ORB_ROCK                    0x112
#define ITEM_GEM_CUTTER_OF_COULNES       0x113
#define ITEM_BOOK_114                    0x114
#define ITEM_BLOCK_OF_BURNING_INCENSE    0x115
#define ITEM_BLOCK_OF_INCENSE            0x116
#define ITEM_ORB                         0x117
#define ITEM_BROKEN_BLADE                0x118
#define ITEM_BROKEN_HILT                 0x119
#define ITEM_FIGURINE                    0x11A
#define ITEM_ROTWORM_STEW                0x11B
#define ITEM_STRONG_THREAD               0x11C
#define ITEM_DRAGON_SCALES               0x11D
#define ITEM_RESILIENT_SPHERE            0x11E
#define ITEM_STANDARD                    0x11F

/* Magic and useful items (class 0x12) */
#define ITEM_SPELL                       0x120
#define ITEM_BEDROLL                     0x121
#define ITEM_SILVER_SEED                 0x122
#define ITEM_MANDOLIN                    0x123
#define ITEM_FLUTE                       0x124
#define ITEM_LEECHES                     0x125
#define ITEM_MOONSTONE                   0x126
#define ITEM_SPIKE                       0x127
#define ITEM_ROCK_HAMMER                 0x128
#define ITEM_GLOWING_ROCK                0x129
#define ITEM_CAMPFIRE                    0x12A
#define ITEM_FISHING_POLE                0x12B
#define ITEM_MEDALLION                   0x12C
#define ITEM_OIL_FLASK                   0x12D
#define ITEM_FOUNTAIN_12E                0x12E
#define ITEM_CAULDRON                    0x12F

/* Books, scrolls and maps (class 0x13) */
#define ITEM_BOOK_130                    0x130
#define ITEM_BOOK_131                    0x131
#define ITEM_BOOK_132                    0x132
#define ITEM_BOOK_133                    0x133
#define ITEM_BOOK_134                    0x134
#define ITEM_BOOK_135                    0x135
#define ITEM_BOOK_136                    0x136
#define ITEM_BOOK_137                    0x137
#define ITEM_SCROLL_138                  0x138
#define ITEM_SCROLL_139                  0x139
#define ITEM_SCROLL_13A                  0x13A
#define ITEM_MAP                         0x13B
#define ITEM_SCROLL_13C                  0x13C
#define ITEM_SCROLL_13D                  0x13D
#define ITEM_SCROLL_13E                  0x13E
#define ITEM_SCROLL_13F                  0x13F

/* Doors (class 0x14); 0x148-0x14F are the open versions of 0x140-0x147 */
#define ITEM_DOOR_140                    0x140
#define ITEM_DOOR_141                    0x141
#define ITEM_DOOR_142                    0x142
#define ITEM_DOOR_143                    0x143
#define ITEM_DOOR_144                    0x144
#define ITEM_DOOR_145                    0x145
#define ITEM_PORTCULLIS                  0x146
#define ITEM_SECRET_DOOR_147             0x147
#define ITEM_OPEN_DOOR_148               0x148
#define ITEM_OPEN_DOOR_149               0x149
#define ITEM_OPEN_DOOR_14A               0x14A
#define ITEM_OPEN_DOOR_14B               0x14B
#define ITEM_OPEN_DOOR_14C               0x14C
#define ITEM_OPEN_DOOR_14D               0x14D
#define ITEM_OPEN_PORTCULLIS             0x14E
#define ITEM_SECRET_DOOR_14F             0x14F

/* Furniture and other 3D objects (classes 0x15 and 0x16); tmap_c and tmap_s
   are the Underworld Adventures format document's names */
#define ITEM_BENCH                       0x150
#define ITEM_ARROW_151                   0x151
#define ITEM_CROSSBOW_BOLT_152           0x152
#define ITEM_LARGE_BOULDER_153           0x153
#define ITEM_LARGE_BOULDER_154           0x154
#define ITEM_BOULDER                     0x155
#define ITEM_SMALL_BOULDER               0x156
#define ITEM_SHRINE                      0x157
#define ITEM_TABLE                       0x158
#define ITEM_BEAM                        0x159
#define ITEM_MOONGATE                    0x15A
#define ITEM_BARREL                      0x15B
#define ITEM_CHAIR                       0x15C
#define ITEM_CHEST                       0x15D
#define ITEM_NIGHTSTAND                  0x15E
#define ITEM_LOTUS_TURBO_ESPRIT          0x15F
#define ITEM_PILLAR                      0x160
#define ITEM_LEVER_161                   0x161
#define ITEM_SWITCH_162                  0x162
#define ITEM_BRIDGE                      0x164
#define ITEM_GRAVESTONE                  0x165
#define ITEM_WRITING                     0x166
#define ITEM_FORCE_FIELD                 0x16D
#define ITEM_TMAP_C                      0x16E
#define ITEM_TMAP_S                      0x16F

/* Buttons, switches, levers and pull chains (class 0x17); +8 is the other state */
#define ITEM_BUTTON_170                  0x170
#define ITEM_BUTTON_171                  0x171
#define ITEM_BUTTON_172                  0x172
#define ITEM_SWITCH_173                  0x173
#define ITEM_SWITCH_174                  0x174
#define ITEM_LEVER_175                   0x175
#define ITEM_PULL_CHAIN_176              0x176
#define ITEM_PULL_CHAIN_177              0x177
#define ITEM_BUTTON_178                  0x178
#define ITEM_BUTTON_179                  0x179
#define ITEM_BUTTON_17A                  0x17A
#define ITEM_SWITCH_17B                  0x17B
#define ITEM_SWITCH_17C                  0x17C
#define ITEM_LEVER_17D                   0x17D
#define ITEM_PULL_CHAIN_17E              0x17E
#define ITEM_PULL_CHAIN_17F              0x17F

/* Traps (class 0x18, and 0x190) */
#define ITEM_DAMAGE_TRAP                 0x180
#define ITEM_TELEPORT_TRAP               0x181
#define ITEM_ARROW_TRAP                  0x182
#define ITEM_DO_TRAP                     0x183
#define ITEM_PIT_TRAP                    0x184
#define ITEM_CHANGE_TERRAIN_TRAP         0x185
#define ITEM_SPELL_TRAP                  0x186
#define ITEM_CREATE_OBJECT_TRAP          0x187
#define ITEM_DOOR_TRAP                   0x188
#define ITEM_WARD_TRAP                   0x189
#define ITEM_TELL_TRAP                   0x18A
#define ITEM_DELETE_OBJECT_TRAP          0x18B
#define ITEM_INVENTORY_TRAP              0x18C
#define ITEM_SET_VARIABLE_TRAP           0x18D
#define ITEM_CHECK_VARIABLE_TRAP         0x18E
#define ITEM_COMBINATION_TRAP            0x18F
#define ITEM_TEXT_STRING_TRAP            0x190

/* Triggers (class 0x1A) */
#define ITEM_MOVE_TRIGGER                0x1A0
#define ITEM_PICKUP_TRIGGER              0x1A1
#define ITEM_USE_TRIGGER                 0x1A2
#define ITEM_LOOK_TRIGGER                0x1A3
#define ITEM_STEP_ON_TRIGGER             0x1A4
#define ITEM_OPEN_TRIGGER                0x1A5
#define ITEM_UNLOCK_TRIGGER              0x1A6
/* UW2's spelling of ITEM_MOVE_TRIGGER, kept while motion/PHYSICS.C still uses it. */
#define ITEM_MOVE_TRIGGER_1A0            ITEM_MOVE_TRIGGER

/* Animated objects (major class 7) */
#define ITEM_BLOOD                       0x1C0
#define ITEM_MIST_CLOUD                  0x1C1
#define ITEM_EXPLOSION_1C2               0x1C2
#define ITEM_EXPLOSION_1C3               0x1C3
#define ITEM_EXPLOSION_1C4               0x1C4
#define ITEM_SPLASH_1C5                  0x1C5
#define ITEM_SPLASH_1C6                  0x1C6
#define ITEM_SPELL_EFFECT                0x1C7
#define ITEM_SMOKE                       0x1C8
#define ITEM_FOUNTAIN_1C9                0x1C9
#define ITEM_SILVER_TREE                 0x1CA
#define ITEM_DAMAGE                      0x1CB
#define ITEM_SOUND_SOURCE                0x1CD
#define ITEM_CHANGING_TERRAIN            0x1CE
#define ITEM_MOVING_DOOR                 0x1CF

#endif
