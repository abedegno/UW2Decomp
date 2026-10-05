/* level.h: a level's block in LEV.ARK, which needs both struct Tile (map.h) and struct
   Object (object.h) complete. map.h and object.h include each other before their
   definitions, so each includes this at its end, and the struct is defined once both are
   done. */
#if defined(MAP_H_COMPLETE) && defined(OBJECT_H_COMPLETE) && !defined(LEVEL_H)
#define LEVEL_H

/* A level's block in LEV.ARK, 0x7C08 bytes, which MAP.C's Map_Load reads to mapdata
   (UW-Formats 4.1 has the layout; Map_Init allocates LEVEL_SIZE bytes and Map_Save
   writes as many). OBJECTS.C's Map_ObjFix points the object store's pointers into it.
   The three counts before magic are stored by Map_Save from the live pointers and turned
   back into them by Map_Load: critptr and objptr point at the top entry of each free list
   (so a count is the entries less one, as UW-Formats says), and LastActiveMob is
   ActiveMob plus nactive. UW1's block ends at the magic word: the animation overlays are
   a block of their own (EFFECT.C's animlist), where UW2's level block carries them and
   its timers after magic. */
struct LevelBlock {
    struct Tile tiles[MAP_SIZE * MAP_SIZE]; /* 0x0000 */
    struct Object mobile[NUM_MOBILE];   /* 0x4000, critdata */
    struct StaticObj statics[NUM_STATIC]; /* 0x5B00, objdata */
    uint16 mobfree[0xFE];               /* 0x7300, critbot: the free mobile objects
                                           (objects 2 to 0xFF) */
    uint16 staticfree[NUM_STATIC];      /* 0x74FC, objbot: the free static objects */
    unsigned char active[0x104];        /* 0x7AFC, ActiveMob: the active mobile objects */
    uint16 nactive;                     /* 0x7C00, LastActiveMob - ActiveMob */
    uint16 nmobfree;                    /* 0x7C02, critptr - critbot */
    uint16 nstaticfree;                 /* 0x7C04, objptr - objbot */
    uint16 magic;                       /* 0x7C06, LEVEL_MAGIC */
};                                      /* 0x7C08 */
#define LEVEL_SIZE      0x7C08          /* sizeof (struct LevelBlock) */
#define LEVEL_MAGIC     0x7577          /* "uw" */
/* mapdata as the level block */
#define LEVEL           ((struct LevelBlock far *)mapdata)

/* LEV.ARK's blocks for level lev (1 to 9): five kinds, nine levels each, by the block
   numbers the loaders pass to get_arc and put_arc (MAP.C, EFFECT.C, TEXTMAPS.C and
   AUTOMAP.C). The shipped archive declares 135 blocks and holds the first three kinds;
   the automap and notes blocks are made when a game is saved. */
#define LEVARK_MAP(lev)     ((lev) - 1)     /* struct LevelBlock, LEVEL_SIZE bytes */
#define LEVARK_ANIM(lev)    ((lev) + 8)     /* the animation overlays, EFFECT.C */
#define LEVARK_TXM(lev)     ((lev) + 0x11)  /* the texture map, TXM_BLOCK_SIZE bytes */
#define LEVARK_AUTOMAP(lev) ((lev) + 0x1A)  /* PlayersMap, AUTOMAP.C */
#define LEVARK_NOTES(lev)   ((lev) + 0x23)  /* the map notes, AUTOMAP.C */

#endif
