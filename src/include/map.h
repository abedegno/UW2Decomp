/* map.h: The level map: loading and saving tiles, textures and lighting, and collision with
   the terrain. */
#ifndef MAP_H
#define MAP_H

#include "uw2.h"

struct Collision;
struct MotionCalc;
struct Object;
struct Tile;

#include "object.h"

/* The motion calculation record, 23 bytes (seg031's Ppd is one, and the next variable
   follows at +0x17), reached through curP: what seg028's collision checks find under and
   around a mover's footprint. */
struct MotionCalc {
    int x, y, z;                        /* 0x00, in 1/8 tiles */
    unsigned heading;                   /* 0x06, the heading in bits 13-15 */
    unsigned char radius;               /* 0x08 */
    unsigned char height;               /* 0x09 */
    int index;                          /* 0x0A */
    int hits0, hits1;                   /* 0x0C */
    unsigned char floor;                /* 0x10, the height under the centre */
    unsigned char top;                  /* 0x11, the highest under the footprint */
    unsigned char slope;                /* 0x12 */
    unsigned char open;                 /* 0x13 */
    unsigned char found;                /* 0x14, collisions found */
    unsigned char count;                /* 0x15, those in the way */
    signed char first;                  /* 0x16, the first of them in oCollisions */
};

/* A square of the level map, 4 bytes; the map (mapdata) is 64 by 64 of them. UW-Formats
   (4.2, the tilemap) documents the fields. */
struct Tile {
    unsigned type:4;                    /* 0x00: solid, open, the diagonals and slopes */
    unsigned height:4;
    unsigned light:2;                   /* 0x01 */
    unsigned floor:4;                   /* the floor texture */
    unsigned door:2;
    union Link objects;                 /* 0x02, the head of the tile's object list; its
                                           low six bits are the wall texture */
};

/* A collision record, 6 bytes: what seg028's object checks found in a mover's way, in
   oCollisions. */
struct Collision {
    unsigned char top;                  /* 0x00 */
    unsigned char bottom;               /* 0x01 */
    union Link link;                    /* 0x02: the object, with flags in the low six bits */
    int offset;                         /* 0x04, the tile's offset in the map */
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
#define NUM_LEVELS      0x50            /* 80: LEV.ARK holds four blocks a level for 80
                                           levels (UW-Formats 4.1) */
#define LEVELS_PER_WORLD 8              /* each world has eight levels; (level - 1) / 8
                                           is the world (Guide, "The Worlds and Level
                                           Concept") */

/* A texture's terrain type (TxmTerr, from DATA\TERRAIN.DAT): bits 6-7 are a class the
   code tests with (TxmTerr[t] & 0xC0) >> 6. UW2's TERRAIN.DAT uses 0x40, 0x80 and 0xC0
   (Underworld Adventures' format document, 4.7: water, lava, ice); fishing needs class 1
   (ovr110), and a changed floor of class 2 turns solid at random (ovr110's change terrain
   trap). */
#define TERR_CLASS      0xC0
#define TERRAIN_WATER   1
#define TERRAIN_LAVA    2
#define TERRAIN_ICE     3

/* OVR128.C: loading and saving the level map */
extern int hgt_val[17];
char far Anim_Load(char far *source);
char far Anim_Save(char far *destination);
unsigned char far Map_Load(int arc, int level, int folderType);
char far Map_Save(int arc, int level, int folderType);

/* OVR140.C: a level's texture map */
extern int TxmID[0x40];
void far load_txtmaps(void);
void far Load_Terrains(int *ids);

/* OVR152.C: lighting */
void far init_lighting(void);
void far random_light(char enabled);

/* SEG028.C: terrain and object collision for anything that moves or is placed */
extern struct Collision oCollisions[8];
extern struct MotionCalc near *curP;
extern int nvokHgt;
extern int nvokTerr;
int far GetSlopeHgt(int x, int y);
void far ComputeHeading(void);
int far get_home_tile(void);
void far process_objlist(void);
unsigned char far drop_around_place(struct Object far *obj, int x, int y, int z, int range);

/* Defined where no source has it yet: data the link takes from the EXE. */
extern unsigned char far ModelData_seg052_519C_2600;

#endif
