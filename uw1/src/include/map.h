/* map.h: The level map: loading and saving tiles, textures and lighting, and collision with
   the terrain. The tile map itself is mapdata, the start of the level block (struct
   LevelBlock, level.h), 64 by 64 struct Tile with the origin at the south west corner and
   x + y * 64 as a tile's index (Map_GetAddr). Sources: MAP.C, MAPADDR.C, TEXTMAPS.C and
   LIGHTING.C in src/map; COLLIDE.C (src/motion) owns struct MotionCalc and struct
   Collision. docs/subsystems/map.md describes the subsystem. */
#ifndef MAP_H
#define MAP_H

#include "uw2.h"

struct Collision;
struct MotionCalc;
struct Object;
struct Tile;
struct Arc;

#include "object.h"

/* The motion calculation record, 23 bytes (seg031's Ppd is one, and the next variable
   follows at +0x17), reached through curP: what seg028's collision checks find under and
   around a mover's footprint. */
struct MotionCalc {
    int16 x, y, z;                      /* 0x00, in 1/8 tiles */
    uint16 heading;                     /* 0x06, the heading in bits 13-15 */
    unsigned char radius;               /* 0x08 */
    unsigned char height;               /* 0x09 */
    int16 index;                        /* 0x0A */
    int16 hits0, hits1;                 /* 0x0C */
    unsigned char floor;                /* 0x10, the height under the centre */
    unsigned char top;                  /* 0x11, the highest under the footprint */
    unsigned char slope;                /* 0x12 */
    unsigned char open;                 /* 0x13 */
    unsigned char found;                /* 0x14, collisions found */
    unsigned char count;                /* 0x15, those in the way */
    signed char first;                  /* 0x16, the first of them in oCollisions */
};

/* A square of the level map, 4 bytes; the map (mapdata) is 64 by 64 of them. UW-Formats
   (4.2, the tilemap) documents the fields. light's bit 0 (bit 8 of the word) is the
   tile's light flag PHYSICS.C tests when the player moves; door's bit 0 (bit 14) is the
   no-magic flag (SPELLS.C's anti_magic_p) and its bit 1 (bit 15) marks a door
   (TRIGGER.C), as UW-Formats has them. floor indexes TxmID; the wall index is in the
   object link (TILE_WALL). */
struct Tile {
    uint16 type:4;                      /* 0x00: solid, open, the diagonals and slopes */
    uint16 height:4;
    uint16 light:2;                     /* 0x01 */
    uint16 floor:4;                     /* the floor texture */
    uint16 door:2;
    union Link objects;                 /* 0x02, the head of the tile's object list; its
                                           low six bits are the wall texture */
};
/* A tile's wall texture, the low six bits of its object link (an lvalue) */
#define TILE_WALL(t)    ((t)->objects.f.low)

/* A collision record, 6 bytes: what seg028's object checks found in a mover's way, in
   oCollisions. */
struct Collision {
    unsigned char top;                  /* 0x00 */
    unsigned char bottom;               /* 0x01 */
    union Link link;                    /* 0x02: the object, with flags in the low six bits */
    int16 offset;                       /* 0x04, the tile's offset in the map */
};

/* Tile types, struct Tile's type (UW-Formats 4.2, "Underworld tile types"). The
   diagonals are named by their open half and the slopes by the way they rise. */
enum TileType {
    TILE_SOLID,                         /* a wall */
    TILE_OPEN,
    TILE_DIAG_SE,                       /* diagonal, open to the south east */
    TILE_DIAG_SW,
    TILE_DIAG_NE,
    TILE_DIAG_NW,
    TILE_SLOPE_N,                       /* sloping up to the north */
    TILE_SLOPE_S,
    TILE_SLOPE_E,
    TILE_SLOPE_W
};

#define MAP_SIZE        0x40            /* the map is 64 by 64 tiles (UW-Formats 4.2) */
#define MAP_MASK        0x3F            /* keeps a tile coordinate on the map */
#define MAP_TILES       0x1000          /* MAP_SIZE * MAP_SIZE: a byte a tile in PlayersMap */
#define NUM_LEVELS      0x50            /* 80: LEV.ARK holds four blocks a level for 80
                                           levels (UW-Formats 4.1) */
#define LEVELS_PER_WORLD 8              /* each world has eight levels; (level - 1) / 8
                                           is the world (Guide, "The Worlds and Level
                                           Concept") */

/* Levels the code treats specially (PlayerLevel, 1 to 9). The names are the Guide's
   (Ultima Underworld's levels: Tybal's Lair, the Ethereal Void endgame map); what each
   gets is read from the code: on level 7 the player has no mana while Tybal's orb stands
   (GAMEWRAP.C's do_level_hacks) and Tybal's guards close in (AI.C), and it has the maze
   spell's floor; on level 9 the automap is off, nothing dropped is kept (OBJPHYS.C), and
   the player cannot sleep, plant the silver seed or come back at the tree. */
#define LEVEL_TYBAL     7
#define LEVEL_VOID      9

/* A texture's terrain type, DATA\TERRAIN.DAT's word for it (walls from offset 0 into
   w64_types, floors from 0x200 into TxmTerr): the values are UW-Formats' (4.8, "Terrain
   texture properties", Ultima Underworld's list). The wall types mark special walls;
   CONVERSE.C, LOOK.C and USEITEMS.C test 8, 9 and 0xB. */
#define TERR_NORMAL     0x00            /* a normal (solid) wall or floor */
#define TERR_ANKH       0x02            /* an ankh mural (shrines) */
#define TERR_STAIRS_UP  0x03
#define TERR_STAIRS_DOWN 0x04
#define TERR_PIPE       0x05
#define TERR_GRATING    0x06
#define TERR_DRAIN      0x07
#define TERR_PRINCESS   0x08            /* the chained-up princess */
#define TERR_WINDOW     0x09
#define TERR_TAPESTRY   0x0A
#define TERR_TEXTURED_DOOR 0x0B         /* the textured door */
#define TERR_WATER      0x10            /* floors: water */
#define TERR_LAVA       0x20            /* floors: lava */

/* A floor's terrain class: UW1 takes TERRAIN.DAT's word shifted right 4 (PATHFIND.C's
   hyp_move, WORLDEV.C), and COLLIDE.C's tile terrain word has it in bits 8-9 (the word
   shifted left 4), from which MOTION.C's collision state takes it as bits 0-1. UW2's
   TERRAIN.DAT keeps its classes in bits 6-7 instead (0xC0). UW1's floors use only
   classes 0 to 2; the engine still handles ice (class 3), as UW2 does. */
#define TERRAIN_WATER   1
#define TERRAIN_LAVA    2
#define TERRAIN_ICE     3

/* A level's texture map (TEXTMAPS.C): its wall textures (TxmID), floor textures
   (floor_IDs) and door textures (ActDoors), and its block in LEV.ARK: 48 wall words, 10
   floor words, then the six door textures a byte each in three words. */
#define TXM_WALLS       0x30
#define TXM_FLOORS      10
#define TXM_DOORS       6
#define TXM_BLOCK_SIZE  0x7A            /* (TXM_WALLS + TXM_FLOORS + TXM_DOORS / 2) * 2 */

/* MAP.C: loading and saving the level map. hgt_val converts a floor height to a z. */
extern int16 hgt_val[17];
int far Anim_Load(char *name, int level);
char far Anim_Save(char *name, int level);
char far Map_Load(struct Arc *arc, int level);
char far Map_Save(struct Arc *arc, int level);
extern struct Tile far *mapdata;
char far Map_Init(void);

/* TEXTMAPS.C: a level's texture map */
extern int16 TxmID[TXM_WALLS];
void far load_txtmaps(void);
void far Load_Terrains(int16 *walls, int16 *floors); /* UW1: walls and floors apart */
extern uint16 TxmTerr[TXM_FLOORS];
char far init_txtlib(void);
/* UW1: an open archive (ovr091) and the level, as GAMEWRAP.C passes them */
unsigned char far Txm_Load(char *arc, int lev);
char far Txm_Save(char *arc, int lev);
extern uint16 f16p;
extern int16 floor_IDs[TXM_FLOORS];
extern uint16 f32_buf;
void far load_tr_mem(char *name, int16 *ids, int16 *count, uint16 seg);

/* LIGHTING.C: lighting */
void far init_lighting(void);
void far random_light(char enabled);

/* COLLIDE.C: terrain and object collision for anything that moves or is placed */
extern struct Collision oCollisions[8];
extern struct MotionCalc near *curP;
extern int16 nvokHgt;
extern int16 nvokTerr;
int far GetSlopeHgt(int x, int y);
void far ComputeHeading(void);
void far process_objlist(void);
unsigned char far drop_around_place(struct Object far *obj, int x, int y, int z, int range);
void far TerrainCheck(unsigned char range);
void far ObjectCheck(unsigned char flat, unsigned char useflag);
unsigned char far can_place(int item, int index, int x, int y, int z, unsigned char flier, unsigned char range);
unsigned char far put_at(int x, int y, int z, struct Object far *obj, int range, unsigned char nocull);
unsigned char far near_mob_put_at(struct Object far *src, struct Object far *obj, int range, unsigned char nocull);
extern char stay_centered;

/* Defined where no source has it yet: data the link takes from the EXE. */
extern uint16 dseg_5c99_7178;

#define MAP_H_COMPLETE
#include "level.h"

#endif
