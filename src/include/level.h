/* level.h: a level's block in LEV.ARK, which needs both struct Tile (map.h) and struct
   Object (object.h) complete. map.h and object.h include each other before their
   definitions, so each includes this at its end, and the struct is defined once both are
   done. */
#if defined(MAP_H_COMPLETE) && defined(OBJECT_H_COMPLETE) && !defined(LEVEL_H)
#define LEVEL_H

/* A level's block in LEV.ARK, 0x7E08 bytes, which Map_Load reads to mapdata (UW-Formats
   4.1 has the layout to 0x7C06; the rest is Map_Load's and Anim_Load's). Map_ObjFix
   points the object store's pointers into it. */
struct LevelBlock {
    struct Tile tiles[MAP_SIZE * MAP_SIZE]; /* 0x0000 */
    struct Object mobile[NUM_MOBILE];   /* 0x4000, critdata */
    struct StaticObj statics[NUM_STATIC]; /* 0x5B00, objdata */
    unsigned mobfree[0xFE];             /* 0x7300, critbot: the free mobile objects
                                           (objects 2 to 0xFF) */
    unsigned staticfree[NUM_STATIC];    /* 0x74FC, objbot: the free static objects */
    unsigned char active[0x104];        /* 0x7AFC, ActiveMob: the active mobile objects */
    unsigned nactive;                   /* 0x7C00, LastActiveMob - ActiveMob */
    unsigned nmobfree;                  /* 0x7C02, critptr - critbot */
    unsigned nstaticfree;               /* 0x7C04, objptr - objbot */
    unsigned magic;                     /* 0x7C06, LEVEL_MAGIC */
    struct Anim anims[0x40];            /* 0x7C08, animlist */
    int timers[0x40];                   /* 0x7D88, timerlist */
};                                      /* 0x7E08 */
#define LEVEL_MAGIC     0x7577          /* "uw" */
/* mapdata as the level block */
#define LEVEL           ((struct LevelBlock far *)mapdata)

#endif
