/* target: ovr123 */
/* opts: -mm -1 -G -O -Y -d */
/* MAP.C: allocating the level block and moving it between memory and LEV.ARK. The whole
   of UW1's DOS overlay ovr123 (UW2's ovr128), in original order. Seeded from UW2Decomp's
   src/map/MAP.C.

   A level is one 0x7C08-byte block of LEV.ARK: the 64 by 64 tile map, the mobile and
   static object records with their free lists and the active list, then a magic word
   (struct LevelBlock up to magic; UW1's block has no animation overlays, which EFFECT.C
   keeps in a block of their own). Map_Init allocates it once at start-up (mapdata) and
   has OBJECTS.C's Map_ObjFix point the object store into it. Map_Load reads level N
   from block N-1 of the archive and rebuilds the free-list pointers from the counts
   stored before the magic word; Map_Save writes those counts back and writes the block.
   Both take an open archive record (ovr091) from GAMEWRAP.C, or with none open
   SAVE0\LEV.ARK for themselves, and then load or save the level's animation overlays
   (EFFECT.C's Anim_Load and Anim_Save) through the same archive.

   UW1 against UW2: the archive is a record passed in, not a slot and a directory flag;
   the block is 0x7C08 bytes and the animation lists are EFFECT.C's; Map_Save neither
   checks the object lists (ObjCrunch) nor asks the player.

   name: UW2's (FM Towns Map_Init, Map_Load, Map_Save; UW2Decomp's OverwriteAllTiles_ovr128_37),
   which also keep the EXE's overlay stub order (Map_Load 597, Map_Init 661,
   OverwriteAllTiles_ovr128_37 679, Map_Save 765). */

#include <dos.h>
#include <alloc.h>
#include <mem.h>
#include "file.h"
#include "map.h"
#include "object.h"
#include "sys.h"

/* ovr091: LEV.ARK access */
char far open_arc(char far *arc, char *name);
char far close_arc(char far *arc);
char far put_arc(char far *arc, int block, void far *buf, int n);
int far get_arc(char far *arc, int block, void far *buf);
/* EFFECT.C */

/* hgt_val converts a tile's 4-bit floor height to a z, 0x40 a step. mapdata is the level
   block, MapDirty is set by edits to it and cleared by loading and saving. */
/* name: DS:1992 to DS:19B8, this file's _DATA. */
int16 hgt_val[17] = { 0x000, 0x040, 0x080, 0x0C0, 0x100, 0x140, 0x180, 0x1C0, 0x200,
                    0x240, 0x280, 0x2C0, 0x300, 0x340, 0, 0, 0x400 };      /* DS:1992 */
struct Tile far *mapdata = 0;           /* DS:19B4 */
unsigned char MapDirty = 0;             /* DS:19B8 */

/* Allocates the level block on first use and lays the object store out in it. A failed
   allocation is fatal (first_punt, low memory). */
char far Map_Init(void)
{
    if (mapdata == 0) {
        if ((mapdata = farmalloc(0x7C08L)) == 0)
            first_punt(ERR_LOWMEM | 2);
    }
    Map_ObjFix();
    return 1;
}

/* Sets all 4096 tiles to one 4-byte tile value and marks the map dirty. Nothing calls
   it; probably an editor routine. */
void far OverwriteAllTiles_ovr128_37(uint32 *tile)
{
    uint32 far *p;
    unsigned i;

    p = (uint32 far *)mapdata;
    for (i = 0; i < 0x1000; i++) {
        *p = *tile;
        p++;
    }
    MapDirty = 1;
}

/* Reads level `level` (1-based) from the archive arc (or, with none, SAVE0\LEV.ARK,
   opened and closed here) into mapdata. A block without the "uw" magic word is fatal:
   pfatal_code(3). On success the free-list and active-list pointers are set from the
   three counts before the magic word and MapDirty is cleared. Then the animation overlays
   (Anim_Load), whose result it returns; 0 when the archive cannot be opened. */
char far Map_Load(struct Arc *arc, int level)
{
    uint16 far *end;                    /* the block's magic word; the counts before it */
    int result;
    struct Arc a;

    if (arc == 0) {
        if (!open_arc((char far *)&a, "SAVE0\\lev.ark"))
            return 0;
    } else
        a = *arc;
    end = &LEVEL->magic;
    *end = 0;
    get_arc((char far *)&a, level - 1, mapdata);
    if (*end != LEVEL_MAGIC) {
        pfatal_code(3);
    } else {
        critptr = critbot + end[-2];        /* nmobfree */
        objptr = objbot + end[-1];          /* nstaticfree */
        LastActiveMob = ActiveMob + end[-3]; /* nactive */
        MapDirty = 0;
    }
    result = Anim_Load((char *)&a, level);
    if (arc == 0)
        close_arc((char far *)&a);
    return result;
}

/* Writes mapdata back as level `level` of the archive (as for Map_Load): stores the list
   counts and the magic word, writes the block, then the animation overlays (Anim_Save).
   Returns 0 if either fails. */
char far Map_Save(struct Arc *arc, int level)
{
    uint16 far *end;
    int result;
    struct Arc a;

    if (arc == 0) {
        if (!open_arc((char far *)&a, "SAVE0\\lev.ark"))
            return 0;
    } else
        a = *arc;
    end = &LEVEL->magic;
    end[-3] = LastActiveMob - ActiveMob;    /* nactive */
    end[-2] = ((int32)FP_OFF(critptr)      /* nmobfree */
        - (int32)FP_OFF(critbot)) / 2L;
    end[-1] = ((int32)FP_OFF(objptr)       /* nstaticfree */
        - (int32)FP_OFF(objbot)) / 2L;
    *end = LEVEL_MAGIC;
    MapDirty = 0;
    if ((result = put_arc((char far *)&a, level - 1, mapdata, 0x7C08)) != 0)
        result = Anim_Save((char *)&a, level);
    if (arc == 0)
        close_arc((char far *)&a);
    return result;
}
