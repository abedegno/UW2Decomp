/* items.h: object item ids and the classes they fall in.

   An object's item id is the low nine bits of its id word: bits 6-8 the major class,
   bits 4-5 the minor class, bits 0-3 the index within it (UW-Formats, 6). The names of
   the eight major classes are the original ones: FM Towns names each class's loader and
   lookup (hack_init and hack_class_data, creature_class_data, misc_init and
   misc_class_data, stuff_class_data, spec_class_data, rect_class_data, trap_init and
   trap_class_data, animobj_load and animobj_class_data; see ovr134's init_objects and
   get_class_data). The class names (id >> 4) follow the tables and Use functions FM Towns
   names for them (Weapons, Missile, Armor, Containers, Lights, Food; UseWand, UseKey,
   UseUnique, UseMagic, UseBook, UseRune) and the ranges in UW-Formats, 6. The item names
   are the game's own: string block 4 of DATA\STRINGS.PAK, the name printed for each item,
   with the article dropped and a hex suffix where several items share a name. Items
   with an empty name in block 4 have no entry. */
#ifndef ITEMS_H
#define ITEMS_H

/* Major classes: the id's bits 6-8, (id & ID_MAJOR) >> 6. */
enum ObjMajor {
    MAJOR_HACK,                         /* 0x000-0x03F: weapons, missiles and armour */
    MAJOR_CREATURE,                     /* 0x040-0x07F: creatures, the player 0x7F */
    MAJOR_MISC,                         /* 0x080-0x0BF: containers, lights, wands,
                                           treasure and food */
    MAJOR_STUFF,                        /* 0x0C0-0x0FF: scenery, potions and runestones */
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
#define CLASS_FOOD          0x0B
#define CLASS_SCENERY       0x0C        /* scenery and junk (UseUtil) */
#define CLASS_SCENERY2      0x0D
#define CLASS_REAG          0x0E        /* potions and the first runestones (UseReag) */
#define CLASS_RUNESTONE     0x0F
#define CLASS_KEY           0x10        /* keys, the lockpick and the lock (UseKey) */
#define CLASS_UNIQUE        0x11        /* UseUnique */
#define CLASS_MAGIC         0x12        /* UseMagic */
#define CLASS_BOOK          0x13        /* books, scrolls and maps (UseBook) */
#define CLASS_DOOR          0x14
#define CLASS_FURNITURE     0x15
#define CLASS_DECAL         0x16        /* pillar, painting, bridge, writing, force field,
                                           special tmap objects */
#define CLASS_SWITCH        0x17
#define CLASS_TRAP          0x18
#define CLASS_TRAP2         0x19        /* more traps, and the flam and tym runes */
#define CLASS_TRIGGER       0x1A
#define CLASS_TRIGGER2      0x1B
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

/* Trap types: a trap's item less FIRST_TRAP, the index UseTrap (ovr166) switches on;
   named after the trap items 0x180-0x198 in string block 4. */
enum TrapType {
    TRAP_DAMAGE,                         /* 0x00 */
    TRAP_TELEPORT,
    TRAP_ARROW,
    TRAP_HACK,
    TRAP_SPECIAL_EFFECT,
    TRAP_CHANGE_TERRAIN,
    TRAP_SPELL,
    TRAP_CREATE_OBJECT,
    TRAP_DOOR,
    TRAP_WARD,
    TRAP_SKILL,
    TRAP_DELETE_OBJECT,
    TRAP_INVENTORY,
    TRAP_SET_VARIABLE,
    TRAP_CHECK_VARIABLE,
    TRAP_NULL,
    TRAP_TEXT_STRING,                         /* 0x10 */
    TRAP_EXPERIENCE,
    TRAP_JUMP,
    TRAP_CHANGE_FROM,
    TRAP_CHANGE_TO,
    TRAP_OSCILLATOR,
    TRAP_PROXIMITY,
    TRAP_PIT,
    TRAP_BRIDGE                         /* 0x18 */
};

/* The first item id of each range, for range tests and arithmetic on item ids. */
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
#define FIRST_POTION        0x0E1
#define FIRST_RUNESTONE     0x0E8       /* An; the 24 runes run in alphabetical order */
#define FIRST_SPEC          0x100
#define FIRST_KEY           0x102
#define FIRST_BLACKROCK_GEM 0x118
#define FIRST_BOOK          0x130
#define FIRST_RECT          0x140       /* the doors */
#define FIRST_OPEN_DOOR     0x148       /* an open door is its closed item + 8 */
#define FIRST_FURNITURE     0x150
#define FIRST_SWITCH        0x170
#define FIRST_TRAP          0x180
#define FIRST_TRIGGER       0x1A0
#define FIRST_ANIMOBJ       0x1C0

/* Weapons (class 0x00), hack_class_data's Weapons table */
#define ITEM_HAND_AXE                    0x000  /* a hand axe */
#define ITEM_BATTLE_AXE                  0x001  /* a battle axe */
#define ITEM_AXE                         0x002  /* an axe */
#define ITEM_DAGGER                      0x003  /* a dagger */
#define ITEM_SHORTSWORD                  0x004  /* a shortsword */
#define ITEM_LONGSWORD                   0x005  /* a longsword */
#define ITEM_BROADSWORD                  0x006  /* a broadsword */
#define ITEM_CUDGEL                      0x007  /* a cudgel */
#define ITEM_LIGHT_MACE                  0x008  /* a light mace */
#define ITEM_MACE                        0x009  /* a mace */
#define ITEM_JEWELLED_DAGGER             0x00A  /* a jewelled dagger */
#define ITEM_JEWELED_AXE                 0x00B  /* a jeweled axe */
#define ITEM_BLACK_SWORD                 0x00C  /* a black sword */
#define ITEM_JEWELED_SWORD               0x00D  /* a jeweled sword */
#define ITEM_JEWELED_MACE                0x00E  /* a jeweled mace */
#define ITEM_FIST                        0x00F  /* a fist */

/* Missiles and missile weapons (class 0x01), the Missile table */
#define ITEM_SLING_STONE                 0x010  /* a sling stone */
#define ITEM_CROSSBOW_BOLT_11            0x011  /* a crossbow bolt */
#define ITEM_ARROW_12                    0x012  /* an arrow */
#define ITEM_SKULL_13                    0x013  /* a skull */
#define ITEM_FIREBALL_14                 0x014  /* a fireball */
#define ITEM_LIGHTNING_BOLT              0x015  /* a lightning bolt */
#define ITEM_ACID                        0x016  /* acid */
#define ITEM_MAGIC_ARROW                 0x017  /* a magic arrow */
#define ITEM_SLING                       0x018  /* a sling */
#define ITEM_BOW                         0x019  /* a bow */
#define ITEM_CROSSBOW                    0x01A  /* a crossbow */
#define ITEM_HOMING_DART                 0x01B  /* a homing dart */
#define ITEM_SNOWBALL                    0x01C  /* a snowball */
#define ITEM_FIREBALL_1D                 0x01D  /* a fireball */
#define ITEM_SATELLITE                   0x01E  /* a satellite */
#define ITEM_JEWELED_BOW                 0x01F  /* a jeweled bow */

/* Armour and clothing (classes 0x02 and 0x03), the Armor table */
#define ITEM_LEATHER_VEST                0x020  /* a leather vest */
#define ITEM_MAIL_SHIRT                  0x021  /* a mail shirt */
#define ITEM_BREASTPLATE                 0x022  /* a breastplate */
#define ITEM_LEATHER_LEGGINGS            0x023  /* leather leggings */
#define ITEM_MAIL_LEGGINGS               0x024  /* mail leggings */
#define ITEM_PLATE_LEGGINGS              0x025  /* plate leggings */
#define ITEM_LEATHER_GLOVES              0x026  /* leather gloves */
#define ITEM_CHAIN_GAUNTLETS             0x027  /* chain gauntlets */
#define ITEM_PLATE_GAUNTLETS             0x028  /* plate gauntlets */
#define ITEM_LEATHER_BOOTS               0x029  /* leather boots */
#define ITEM_CHAIN_BOOTS                 0x02A  /* chain boots */
#define ITEM_PLATE_BOOTS                 0x02B  /* plate boots */
#define ITEM_LEATHER_CAP                 0x02C  /* a leather cap */
#define ITEM_CHAIN_COWL                  0x02D  /* a chain cowl */
#define ITEM_HELMET                      0x02E  /* a helmet */
#define ITEM_SWAMP_BOOTS                 0x02F  /* a pair of swamp boots */
#define ITEM_CROWN_30                    0x030  /* a crown */
#define ITEM_CROWN_31                    0x031  /* a crown */
#define ITEM_CROWN_32                    0x032  /* a crown */
#define ITEM_FRAZNIUM_GAUNTLETS          0x033  /* fraznium gauntlets */
#define ITEM_FRAZNIUM_CIRCLET            0x034  /* a fraznium circlet */
#define ITEM_GUARDIAN_SIGNET_RING        0x035  /* a Guardian signet ring */
#define ITEM_STRANGE_ARTIFACT            0x036  /* a strange artifact */
#define ITEM_COPPER_RING                 0x037  /* a copper ring */
#define ITEM_GOLD_RING                   0x038  /* a gold ring */
#define ITEM_SILVER_RING                 0x039  /* a silver ring */
#define ITEM_RED_RING                    0x03A  /* a red ring */
#define ITEM_TOWER_SHIELD                0x03B  /* a tower shield */
#define ITEM_WOODEN_SHIELD               0x03C  /* a wooden shield */
#define ITEM_SMALL_SHIELD                0x03D  /* a small shield */
#define ITEM_BUCKLER                     0x03E  /* a buckler */
#define ITEM_JEWELED_SHIELD              0x03F  /* a jeweled shield */

/* Creatures (major class 1); 0x7F is the player */
#define ITEM_ROTWORM                     0x040  /* a rotworm */
#define ITEM_CAVE_BAT                    0x041  /* a cave bat */
#define ITEM_VAMPIRE_BAT                 0x042  /* a vampire bat */
#define ITEM_GIANT_TAN_RAT               0x043  /* a giant tan rat */
#define ITEM_GIANT_GREY_RAT              0x044  /* a giant grey rat */
#define ITEM_FLESH_SLUG                  0x045  /* a flesh slug */
#define ITEM_ACID_SLUG                   0x046  /* an acid slug */
#define ITEM_MONGBAT                     0x047  /* a mongbat */
#define ITEM_SKELETON                    0x048  /* a skeleton */
#define ITEM_GOBLIN_49                   0x049  /* a goblin */
#define ITEM_GOBLIN_4A                   0x04A  /* a goblin */
#define ITEM_IMP                         0x04B  /* an imp */
#define ITEM_GIANT_SPIDER                0x04C  /* a giant spider */
#define ITEM_LURKER                      0x04D  /* a lurker */
#define ITEM_BLOODWORM                   0x04E  /* a bloodworm */
#define ITEM_STICKMAN                    0x04F  /* a stickman */
#define ITEM_WHITE_WORM                  0x050  /* a white worm */
#define ITEM_SNOW_CAT                    0x051  /* a snow cat */
#define ITEM_YETI                        0x052  /* a yeti */
#define ITEM_HEADLESS                    0x053  /* a headless */
#define ITEM_TALORID                     0x054  /* a Talorid */
#define ITEM_GHOST                       0x055  /* a ghost */
#define ITEM_WOLF_SPIDER                 0x056  /* a wolf spider */
#define ITEM_TRILKHUN                    0x057  /* a trilkhun */
#define ITEM_BRAIN_CREATURE              0x058  /* a brain creature */
#define ITEM_DEEP_LURKER                 0x059  /* a deep lurker */
#define ITEM_DREAD_SPIDER                0x05A  /* a dread spider */
#define ITEM_HUMAN_5B                    0x05B  /* a human */
#define ITEM_GREAT_TROLL                 0x05C  /* a great troll */
#define ITEM_SPECTRE                     0x05D  /* a spectre */
#define ITEM_HORDLING                    0x05E  /* a hordling */
#define ITEM_EARTH_GOLEM                 0x05F  /* an earth golem */
#define ITEM_FIRE_ELEMENTAL              0x060  /* a fire elemental */
#define ITEM_ICE_GOLEM                   0x061  /* an ice golem */
#define ITEM_DIRE_GHOST                  0x062  /* a dire ghost */
#define ITEM_REAPER                      0x063  /* a reaper */
#define ITEM_DESPOILER                   0x064  /* a despoiler */
#define ITEM_METAL_GOLEM                 0x065  /* a metal golem */
#define ITEM_HAUNT                       0x066  /* a haunt */
#define ITEM_DIRE_REAPER                 0x067  /* a dire reaper */
#define ITEM_DESTROYER                   0x068  /* a destroyer */
#define ITEM_LICHE_69                    0x069  /* a liche */
#define ITEM_LICHE_6A                    0x06A  /* a liche */
#define ITEM_LICHE_6B                    0x06B  /* a liche */
#define ITEM_HUMAN_6C                    0x06C  /* a human */
#define ITEM_VORZ                        0x06D  /* a vorz */
#define ITEM_FIGHTER                     0x06E  /* a fighter */
#define ITEM_GAZER                       0x06F  /* a gazer */
#define ITEM_HUMAN_70                    0x070  /* a human */
#define ITEM_HUMAN_71                    0x071  /* a human */
#define ITEM_HUMAN_72                    0x072  /* a human */
#define ITEM_HUMAN_73                    0x073  /* a human */
#define ITEM_HUMAN_74                    0x074  /* a human */
#define ITEM_HUMAN_75                    0x075  /* a human */
#define ITEM_HUMAN_76                    0x076  /* a human */
#define ITEM_HUMAN_77                    0x077  /* a human */
#define ITEM_HUMAN_78                    0x078  /* a human */
#define ITEM_HUMAN_79                    0x079  /* a human */
#define ITEM_HUMAN_7A                    0x07A  /* a human */
#define ITEM_HUMAN_7B                    0x07B  /* a human */
#define ITEM_HUMAN_7E                    0x07E  /* a human */
#define ITEM_ADVENTURER                  0x07F  /* an adventurer */

/* Containers (class 0x08), misc_class_data's Containers table */
#define ITEM_SACK                        0x080  /* a sack */
#define ITEM_OPEN_SACK                   0x081  /* an open sack */
#define ITEM_PACK                        0x082  /* a pack */
#define ITEM_OPEN_PACK                   0x083  /* an open pack */
#define ITEM_BOX                         0x084  /* a box */
#define ITEM_OPEN_BOX                    0x085  /* an open box */
#define ITEM_POUCH                       0x086  /* a pouch */
#define ITEM_OPEN_POUCH                  0x087  /* an open pouch */
#define ITEM_MAP_CASE                    0x088  /* a map case */
#define ITEM_OPEN_MAP_CASE               0x089  /* an open map case */
#define ITEM_GOLD_COFFER                 0x08A  /* a gold coffer */
#define ITEM_OPEN_GOLD_COFFER            0x08B  /* an open gold coffer */
#define ITEM_KEY_RING                    0x08C  /* a key ring */
#define ITEM_QUIVER                      0x08D  /* a quiver */
#define ITEM_BOWL                        0x08E  /* a bowl */
#define ITEM_RUNE_BAG                    0x08F  /* a rune bag */

/* Light sources (0x90-0x97, the Lights table) and wands (0x98-0x9F) */
#define ITEM_LANTERN                     0x090  /* a lantern */
#define ITEM_TORCH                       0x091  /* a torch */
#define ITEM_CANDLE                      0x092  /* a candle */
#define ITEM_LIGHT_SPHERE                0x093  /* a light-sphere */
#define ITEM_LIT_LANTERN                 0x094  /* a lit lantern */
#define ITEM_LIT_TORCH                   0x095  /* a lit torch */
#define ITEM_LIT_CANDLE                  0x096  /* a lit candle */
#define ITEM_LIT_LIGHT_SPHERE            0x097  /* a lit light-sphere */
#define ITEM_WAND_98                     0x098  /* a wand */
#define ITEM_WAND_99                     0x099  /* a wand */
#define ITEM_WAND_9A                     0x09A  /* a wand */
#define ITEM_WAND_9B                     0x09B  /* a wand */
#define ITEM_BROKEN_WAND_9C              0x09C  /* a broken wand */
#define ITEM_BROKEN_WAND_9D              0x09D  /* a broken wand */
#define ITEM_BROKEN_WAND_9E              0x09E  /* a broken wand */
#define ITEM_BROKEN_WAND_9F              0x09F  /* a broken wand */

/* Treasure (class 0x0A) */
#define ITEM_COIN                        0x0A0  /* a coin */
#define ITEM_STORAGE_CRYSTAL             0x0A1  /* a storage crystal */
#define ITEM_RUBY                        0x0A2  /* a ruby */
#define ITEM_RED_GEM                     0x0A3  /* a red gem */
#define ITEM_SMALL_BLUE_GEM              0x0A4  /* a small blue gem */
#define ITEM_LARGE_BLUE_GEM              0x0A5  /* a large blue gem */
#define ITEM_SAPPHIRE                    0x0A6  /* a sapphire */
#define ITEM_EMERALD                     0x0A7  /* an emerald */
#define ITEM_BLACK_PEARL                 0x0A8  /* a black pearl */
#define ITEM_GOBLET                      0x0A9  /* a goblet */
#define ITEM_SCEPTRE                     0x0AA  /* a sceptre */
#define ITEM_BLACK_STONE                 0x0AB  /* a black stone */
#define ITEM_WHITE_STONE                 0x0AC  /* a white stone */
#define ITEM_GREY_STONE                  0x0AD  /* a grey stone */
#define ITEM_DELGNIGZATOR                0x0AE  /* a delgnigzator */
#define ITEM_POCKETWATCH                 0x0AF  /* a pocketwatch */

/* Food and drink (class 0x0B), the Food table */
#define ITEM_PIECE_OF_MEAT_B0            0x0B0  /* a piece of meat */
#define ITEM_PIECE_OF_MEAT_B1            0x0B1  /* a piece of meat */
#define ITEM_PIECE_OF_CHEESE             0x0B2  /* a piece of cheese */
#define ITEM_APPLE                       0x0B3  /* an apple */
#define ITEM_EAR_OF_CORN                 0x0B4  /* an ear of corn */
#define ITEM_LOAF_OF_BREAD               0x0B5  /* a loaf of bread */
#define ITEM_FISH                        0x0B6  /* a fish */
#define ITEM_POPCORN                     0x0B7  /* some popcorn */
#define ITEM_PASTRY                      0x0B8  /* a pastry */
#define ITEM_MUSHROOM                    0x0B9  /* a mushroom */
#define ITEM_HONEYCOMB                   0x0BA  /* a honeycomb */
#define ITEM_BOTTLE_OF_ALE               0x0BB  /* a bottle of ale */
#define ITEM_BOTTLE_OF_WATER             0x0BC  /* a bottle of water */
#define ITEM_BOTTLE_OF_WINE              0x0BD  /* a bottle of wine */
#define ITEM_MEAT_ON_A_STICK             0x0BE  /* some meat-on-a-stick */
#define ITEM_NUTRITIOUS_WAFER            0x0BF  /* a nutritious wafer */

/* Scenery and junk (classes 0x0C and 0x0D) */
#define ITEM_PLANT_C0                    0x0C0  /* a plant */
#define ITEM_GRASS                       0x0C1  /* some grass */
#define ITEM_SKULL_C2                    0x0C2  /* a skull */
#define ITEM_SKULL_C3                    0x0C3  /* a skull */
#define ITEM_BONE_C4                     0x0C4  /* a bone */
#define ITEM_BONE_C5                     0x0C5  /* a bone */
#define ITEM_PILE_OF_BONES_C6            0x0C6  /* a pile of bones */
#define ITEM_BROKEN_DAGGER               0x0C7  /* a broken dagger */
#define ITEM_BROKEN_SWORD                0x0C8  /* a broken sword */
#define ITEM_BROKEN_AXE                  0x0C9  /* a broken axe */
#define ITEM_BROKEN_MACE                 0x0CA  /* a broken mace */
#define ITEM_BROKEN_SHIELD               0x0CB  /* a broken shield */
#define ITEM_PIECE_OF_WOOD_CC            0x0CC  /* a piece of wood */
#define ITEM_PIECE_OF_WOOD_CD            0x0CD  /* a piece of wood */
#define ITEM_PLANT_CE                    0x0CE  /* a plant */
#define ITEM_PLANT_CF                    0x0CF  /* a plant */
#define ITEM_PILE_OF_DEBRIS_D0           0x0D0  /* a pile of debris */
#define ITEM_PILE_OF_DEBRIS_D1           0x0D1  /* a pile of debris */
#define ITEM_LUMP_OF_WAX                 0x0D2  /* a lump of wax */
#define ITEM_STALACTITE                  0x0D3  /* a stalactite */
#define ITEM_PLANT_D4                    0x0D4  /* a plant */
#define ITEM_ICICLE                      0x0D5  /* an icicle */
#define ITEM_PILE_OF_DEBRIS_D6           0x0D6  /* a pile of debris */
#define ITEM_ANVIL                       0x0D7  /* an anvil */
#define ITEM_POLE                        0x0D8  /* a pole */
#define ITEM_PLANT_D9                    0x0D9  /* a plant */
#define ITEM_PLANT_DA                    0x0DA  /* a plant */
#define ITEM_RUBBLE                      0x0DB  /* some rubble */
#define ITEM_PILE_OF_WOOD_CHIPS          0x0DC  /* a pile of wood chips */
#define ITEM_PILE_OF_BONES_DD            0x0DD  /* a pile of bones */
#define ITEM_BLOOD_STAIN_DE              0x0DE  /* a blood stain */
#define ITEM_BLOOD_STAIN_DF              0x0DF  /* a blood stain */

/* Potions and runestones (classes 0x0E and 0x0F) */
#define ITEM_RUNESTONE                   0x0E0  /* a runestone */
#define ITEM_BLACK_POTION                0x0E1  /* a black potion */
#define ITEM_PURPLE_POTION               0x0E2  /* a purple potion */
#define ITEM_YELLOW_POTION               0x0E3  /* a yellow potion */
#define ITEM_GREEN_POTION                0x0E4  /* a green potion */
#define ITEM_RED_POTION                  0x0E5  /* a red potion */
#define ITEM_COLORLESS_POTION            0x0E6  /* a colorless potion */
#define ITEM_BROWN_POTION                0x0E7  /* a brown potion */
#define ITEM_AN_STONE                    0x0E8  /* an An stone */
#define ITEM_BET_STONE                   0x0E9  /* a Bet stone */
#define ITEM_CORP_STONE                  0x0EA  /* a Corp stone */
#define ITEM_DES_STONE                   0x0EB  /* a Des stone */
#define ITEM_EX_STONE                    0x0EC  /* an Ex stone */
#define ITEM_FLAM_STONE                  0x0ED  /* a Flam stone */
#define ITEM_GRAV_STONE                  0x0EE  /* a Grav stone */
#define ITEM_HUR_STONE                   0x0EF  /* a Hur stone */
#define ITEM_IN_STONE                    0x0F0  /* an In stone */
#define ITEM_JUX_STONE                   0x0F1  /* a Jux stone */
#define ITEM_KAL_STONE                   0x0F2  /* a Kal stone */
#define ITEM_LOR_STONE                   0x0F3  /* a Lor stone */
#define ITEM_MANI_STONE                  0x0F4  /* a Mani stone */
#define ITEM_NOX_STONE                   0x0F5  /* a Nox stone */
#define ITEM_ORT_STONE                   0x0F6  /* an Ort stone */
#define ITEM_POR_STONE                   0x0F7  /* a Por stone */
#define ITEM_QUAS_STONE                  0x0F8  /* a Quas stone */
#define ITEM_REL_STONE                   0x0F9  /* a Rel stone */
#define ITEM_SANCT_STONE                 0x0FA  /* a Sanct stone */
#define ITEM_TYM_STONE                   0x0FB  /* a Tym stone */
#define ITEM_UUS_STONE                   0x0FC  /* an Uus stone */
#define ITEM_VAS_STONE                   0x0FD  /* a Vas stone */
#define ITEM_WIS_STONE                   0x0FE  /* a Wis stone */
#define ITEM_YLEM_STONE                  0x0FF  /* a Ylem stone */

/* Keys, the lockpick and the lock (class 0x10) */
#define ITEM_CURIOUS_IMPLEMENT           0x100  /* a curious implement */
#define ITEM_LOCKPICK                    0x101  /* a lockpick */
#define ITEM_KEY_102                     0x102  /* a key */
#define ITEM_KEY_103                     0x103  /* a key */
#define ITEM_KEY_104                     0x104  /* a key */
#define ITEM_KEY_105                     0x105  /* a key */
#define ITEM_KEY_106                     0x106  /* a key */
#define ITEM_KEY_107                     0x107  /* a key */
#define ITEM_KEY_108                     0x108  /* a key */
#define ITEM_KEY_109                     0x109  /* a key */
#define ITEM_KEY_10A                     0x10A  /* a key */
#define ITEM_KEY_10B                     0x10B  /* a key */
#define ITEM_KEY_10C                     0x10C  /* a key */
#define ITEM_KEY_10D                     0x10D  /* a key */
#define ITEM_KEY_10E                     0x10E  /* a key */
#define ITEM_LOCK                        0x10F  /* a lock */

/* Unique and quest items (class 0x11) */
#define ITEM_EYEBALL                     0x110  /* an eyeball */
#define ITEM_HORN                        0x111  /* a horn */
#define ITEM_PEARL_TIPPED_ROD            0x112  /* a pearl-tipped rod */
#define ITEM_BLACK_EGGSHELL              0x113  /* a black eggshell */
#define ITEM_PLANT_114                   0x114  /* a plant */
#define ITEM_SERPENT_STATUE              0x115  /* a serpent statue */
#define ITEM_BOTTLE_116                  0x116  /* a bottle */
#define ITEM_AMETHYST_ROD                0x117  /* an amethyst rod */
#define ITEM_BLACKROCK_GEM_118           0x118  /* a blackrock gem */
#define ITEM_BLACKROCK_GEM_119           0x119  /* a blackrock gem */
#define ITEM_BLACKROCK_GEM_11A           0x11A  /* a blackrock gem */
#define ITEM_BLACKROCK_GEM_11B           0x11B  /* a blackrock gem */
#define ITEM_BLACKROCK_GEM_11C           0x11C  /* a blackrock gem */
#define ITEM_BLACKROCK_GEM_11D           0x11D  /* a blackrock gem */
#define ITEM_BLACKROCK_GEM_11E           0x11E  /* a blackrock gem */
#define ITEM_BLACKROCK_GEM_11F           0x11F  /* a blackrock gem */

/* Magic and useful items (class 0x12) */
#define ITEM_SPELL                       0x120  /* a spell */
#define ITEM_BEDROLL                     0x121  /* a bedroll */
#define ITEM_ORB                         0x122  /* an orb */
#define ITEM_MANDOLIN                    0x123  /* a mandolin */
#define ITEM_FLUTE                       0x124  /* a flute */
#define ITEM_LEECHES                     0x125  /* some leeches */
#define ITEM_MOONSTONE                   0x126  /* a moonstone */
#define ITEM_FORCE_FIELD_127             0x127  /* a force field */
#define ITEM_ROCK_HAMMER                 0x128  /* a rock hammer */
#define ITEM_RESILIENT_SPHERE_129        0x129  /* a resilient sphere */
#define ITEM_CAMPFIRE                    0x12A  /* a campfire */
#define ITEM_FISHING_POLE                0x12B  /* a fishing pole */
#define ITEM_THREAD                      0x12C  /* some thread */
#define ITEM_OIL_FLASK                   0x12D  /* an oil flask */
#define ITEM_FOUNTAIN_12E                0x12E  /* a fountain */
#define ITEM_BANNER                      0x12F  /* a banner */

/* Books, scrolls and maps (class 0x13) */
#define ITEM_BOOK_130                    0x130  /* a book */
#define ITEM_BOOK_131                    0x131  /* a book */
#define ITEM_BOOK_132                    0x132  /* a book */
#define ITEM_BOOK_133                    0x133  /* a book */
#define ITEM_SCROLL_134                  0x134  /* a scroll */
#define ITEM_SCROLL_135                  0x135  /* a scroll */
#define ITEM_SCROLL_136                  0x136  /* a scroll */
#define ITEM_SCROLL_137                  0x137  /* a scroll */
#define ITEM_BOOK_138                    0x138  /* a book */
#define ITEM_BIT_OF_A_MAP                0x139  /* a bit of a map */
#define ITEM_MAP                         0x13A  /* a map */
#define ITEM_DEAD_PLANT_13B              0x13B  /* a dead plant */
#define ITEM_DEAD_PLANT_13C              0x13C  /* a dead plant */
#define ITEM_BOTTLE_13D                  0x13D  /* a bottle */
#define ITEM_STICK                       0x13E  /* a stick */
#define ITEM_RESILIENT_SPHERE_13F        0x13F  /* a resilient sphere */

/* Doors (class 0x14); 0x148-0x14F are the open versions of 0x140-0x147 */
#define ITEM_DOOR_140                    0x140  /* a door */
#define ITEM_DOOR_141                    0x141  /* a door */
#define ITEM_DOOR_142                    0x142  /* a door */
#define ITEM_DOOR_143                    0x143  /* a door */
#define ITEM_DOOR_144                    0x144  /* a door */
#define ITEM_DOOR_145                    0x145  /* a door */
#define ITEM_PORTCULLIS                  0x146  /* a portcullis */
#define ITEM_SECRET_DOOR_147             0x147  /* a secret door */
#define ITEM_OPEN_DOOR_148               0x148  /* an open door */
#define ITEM_OPEN_DOOR_149               0x149  /* an open door */
#define ITEM_OPEN_DOOR_14A               0x14A  /* an open door */
#define ITEM_OPEN_DOOR_14B               0x14B  /* an open door */
#define ITEM_OPEN_DOOR_14C               0x14C  /* an open door */
#define ITEM_OPEN_DOOR_14D               0x14D  /* an open door */
#define ITEM_OPEN_PORTCULLIS             0x14E  /* an open portcullis */
#define ITEM_SECRET_DOOR_14F             0x14F  /* a secret door */

/* Furniture and other 3D objects (classes 0x15 and 0x16) */
#define ITEM_BENCH                       0x150  /* a bench */
#define ITEM_ARROW_151                   0x151  /* an arrow */
#define ITEM_CROSSBOW_BOLT_152           0x152  /* a crossbow bolt */
#define ITEM_LARGE_BOULDER_153           0x153  /* a large boulder */
#define ITEM_LARGE_BOULDER_154           0x154  /* a large boulder */
#define ITEM_BOULDER                     0x155  /* a boulder */
#define ITEM_SMALL_BOULDER               0x156  /* a small boulder */
#define ITEM_SHRINE                      0x157  /* a shrine */
#define ITEM_TABLE                       0x158  /* a table */
#define ITEM_BEAM                        0x159  /* a beam */
#define ITEM_MOONGATE                    0x15A  /* a moongate */
#define ITEM_BARREL                      0x15B  /* a barrel */
#define ITEM_CHAIR                       0x15C  /* a chair */
#define ITEM_CHEST                       0x15D  /* a chest */
#define ITEM_NIGHTSTAND                  0x15E  /* a nightstand */
#define ITEM_LOTUS_TURBO_ESPRIT          0x15F  /* a lotus turbo esprit */
#define ITEM_PILLAR                      0x160  /* a pillar */
#define ITEM_LEVER_161                   0x161  /* a lever */
#define ITEM_SWITCH_162                  0x162  /* a switch */
#define ITEM_PAINTING                    0x163  /* a painting */
#define ITEM_BRIDGE                      0x164  /* a bridge */
#define ITEM_GRAVESTONE                  0x165  /* a gravestone */
#define ITEM_WRITING                     0x166  /* some writing */
#define ITEM_BED                         0x167  /* a bed */
#define ITEM_LARGE_BLACKROCK_GEM         0x168  /* a large blackrock gem */
#define ITEM_SHELF                       0x169  /* a shelf */
#define ITEM_FORCE_FIELD_16D             0x16D  /* force field */
#define ITEM_TMAP_C                      0x16E  /* special tmap obj */
#define ITEM_TMAP_S                      0x16F  /* special tmap obj */

/* Buttons, switches, levers and pull chains (class 0x17); +8 is the other state */
#define ITEM_BUTTON_170                  0x170  /* a button */
#define ITEM_BUTTON_171                  0x171  /* a button */
#define ITEM_BUTTON_172                  0x172  /* a button */
#define ITEM_SWITCH_173                  0x173  /* a switch */
#define ITEM_SWITCH_174                  0x174  /* a switch */
#define ITEM_LEVER_175                   0x175  /* a lever */
#define ITEM_PULL_CHAIN_176              0x176  /* a pull chain */
#define ITEM_PULL_CHAIN_177              0x177  /* a pull chain */
#define ITEM_BUTTON_178                  0x178  /* a button */
#define ITEM_BUTTON_179                  0x179  /* a button */
#define ITEM_BUTTON_17A                  0x17A  /* a button */
#define ITEM_SWITCH_17B                  0x17B  /* a switch */
#define ITEM_SWITCH_17C                  0x17C  /* a switch */
#define ITEM_LEVER_17D                   0x17D  /* a lever */
#define ITEM_PULL_CHAIN_17E              0x17E  /* a pull chain */
#define ITEM_PULL_CHAIN_17F              0x17F  /* a pull chain */

/* Traps (classes 0x18 and 0x19) */
#define ITEM_DAMAGE_TRAP                 0x180  /* a damage trap */
#define ITEM_TELEPORT_TRAP               0x181  /* a teleport trap */
#define ITEM_ARROW_TRAP                  0x182  /* a arrow trap */
#define ITEM_HACK_TRAP                   0x183  /* a hack trap */
#define ITEM_SPECIAL_EFFECT_TRAP         0x184  /* a special effects trap */
#define ITEM_CHANGE_TERRAIN_TRAP         0x185  /* a change terrain trap */
#define ITEM_SPELL_TRAP                  0x186  /* a spelltrap */
#define ITEM_CREATE_OBJECT_TRAP          0x187  /* a create object trap */
#define ITEM_DOOR_TRAP                   0x188  /* a door trap */
#define ITEM_WARD_TRAP                   0x189  /* a ward trap */
#define ITEM_SKILL_TRAP                  0x18A  /* a skill trap */
#define ITEM_DELETE_OBJECT_TRAP          0x18B  /* a delete object trap */
#define ITEM_INVENTORY_TRAP              0x18C  /* an inventory trap */
#define ITEM_SET_VARIABLE_TRAP           0x18D  /* a set variable trap */
#define ITEM_CHECK_VARIABLE_TRAP         0x18E  /* a check variable trap */
#define ITEM_NULL_TRAP                   0x18F  /* a null trap */
#define ITEM_TEXT_STRING_TRAP            0x190  /* a text string trap */
#define ITEM_EXPERIENCE_TRAP             0x191  /* an experience trap */
#define ITEM_JUMP_TRAP                   0x192  /* a jump trap */
#define ITEM_CHANGE_FROM_TRAP            0x193  /* a change from trap */
#define ITEM_CHANGE_TO_TRAP              0x194  /* a change to trap */
#define ITEM_OSCILLATOR_TRAP             0x195  /* an oscillator trap */
#define ITEM_PROXIMITY_TRAP              0x196  /* a proximity trap */
#define ITEM_PIT_TRAP                    0x197  /* a pit trap */
#define ITEM_BRIDGE_TRAP                 0x198  /* a bridge trap */
#define ITEM_FLAM_RUNE                   0x19E  /* a flam rune */
#define ITEM_TYM_RUNE                    0x19F  /* a tym rune */

/* Triggers (classes 0x1A and 0x1B) */
#define ITEM_MOVE_TRIGGER_1A0            0x1A0  /* a move trigger */
#define ITEM_PICKUP_TRIGGER_1A1          0x1A1  /* a pick up trigger */
#define ITEM_USE_TRIGGER_1A2             0x1A2  /* a use trigger */
#define ITEM_LOOK_TRIGGER_1A3            0x1A3  /* a look trigger */
#define ITEM_PRESSURE_TRIGGER_1A4        0x1A4  /* a pressure trigger */
#define ITEM_RELEASE_TRIGGER_1A5         0x1A5  /* a pressure release trigger */
#define ITEM_ENTER_TRIGGER_1A6           0x1A6  /* an enter trigger */
#define ITEM_EXIT_TRIGGER_1A7            0x1A7  /* an exit trigger */
#define ITEM_UNLOCK_TRIGGER_1A8          0x1A8  /* an unlock trigger */
#define ITEM_TIMER_TRIGGER_1A9           0x1A9  /* a timer trigger */
#define ITEM_OPEN_TRIGGER_1AA            0x1AA  /* an open trigger */
#define ITEM_CLOSE_TRIGGER_1AB           0x1AB  /* a close trigger */
#define ITEM_SCHEDULED_TRIGGER_1AC       0x1AC  /* a scheduled trigger */
#define ITEM_MOVE_TRIGGER_1B0            0x1B0  /* a move trigger */
#define ITEM_PICKUP_TRIGGER_1B1          0x1B1  /* a pick up trigger */
#define ITEM_USE_TRIGGER_1B2             0x1B2  /* a use trigger */
#define ITEM_LOOK_TRIGGER_1B3            0x1B3  /* a look trigger */
#define ITEM_PRESSURE_TRIGGER_1B4        0x1B4  /* a pressure trigger */
#define ITEM_RELEASE_TRIGGER_1B5         0x1B5  /* a pressure release trigger */
#define ITEM_ENTER_TRIGGER_1B6           0x1B6  /* an enter trigger */
#define ITEM_EXIT_TRIGGER_1B7            0x1B7  /* an exit trigger */
#define ITEM_UNLOCK_TRIGGER_1B8          0x1B8  /* an unlock trigger */
#define ITEM_TIMER_TRIGGER_1B9           0x1B9  /* a timer trigger */
#define ITEM_OPEN_TRIGGER_1BA            0x1BA  /* an open trigger */
#define ITEM_CLOSE_TRIGGER_1BB           0x1BB  /* a close trigger */
#define ITEM_SCHEDULED_TRIGGER_1BC       0x1BC  /* a scheduled trigger */

/* Animated objects (major class 7) */
#define ITEM_BLOOD                       0x1C0  /* some blood */
#define ITEM_MIST_CLOUD                  0x1C1  /* a mist cloud */
#define ITEM_EXPLOSION_1C2               0x1C2  /* an explosion */
#define ITEM_EXPLOSION_1C3               0x1C3  /* an explosion */
#define ITEM_EXPLOSION_1C4               0x1C4  /* an explosion */
#define ITEM_LIGHTNING_1C5               0x1C5  /* lightning */
#define ITEM_SPLASH                      0x1C6  /* a splash */
#define ITEM_SPACEY_TWINKLES             0x1C7  /* some spacey twinkles */
#define ITEM_SMOKE                       0x1C8  /* some smoke */
#define ITEM_FOUNTAIN_1C9                0x1C9  /* a fountain */
#define ITEM_FROST                       0x1CA  /* some frost */
#define ITEM_FLASH                       0x1CB  /* a flash */
#define ITEM_LIGHTNING_1CC               0x1CC  /* lightning */
#define ITEM_WISP                        0x1CD  /* a wisp */
#define ITEM_VAPOR_TRAIL                 0x1CE  /* a vapor trail */
#define ITEM_MOVING_DOOR                 0x1CF  /* a moving door */

#endif
