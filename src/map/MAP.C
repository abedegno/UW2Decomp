/* target: ovr128 */
/* opts: -mm -1 -G -O -Y -d */
/* MAP.C: allocating the level block and moving it between memory and LEV.ARK. The whole
   of DOS overlay ovr128, in original order.

   A level is one 0x7E08-byte block (struct LevelBlock, level.h): the 64 by 64 tile map,
   the mobile and static object records with their free lists and the active list, a
   magic word, then the animation overlays and timers. Map_Init allocates it once at
   start-up (mapdata) and has OBJECTS.C's Map_ObjFix point the object store into it.
   Map_Load reads level N from block N-1 of LEV.ARK, in DATA\ or in the save directory
   (HomeDir), and rebuilds the free-list pointers from the counts stored before the
   magic word; Map_Save writes those counts back, checks the object lists with ObjCrunch
   and writes the block. Anim_Load and Anim_Save copy the block's last 0x200 bytes to and
   from the live animation and timer lists. GAMEWRAP.C's GetLevel and SaveLevel are the
   callers; TEXTMAPS.C loads the texture block of the same level next, through the same
   open archive.

   name: inferred. Map_Init, Map_Load and Map_Save are FM Towns names, and System Shock's
   MAP.C has map_init. */

#include <dos.h>
#include <alloc.h>
#include <mem.h>
#include "file.h"
#include "map.h"
#include "object.h"
#include "sys.h"

#include "ui.h"
/* hgt_val converts a tile's 4-bit floor height to a z in the units PHYSICS.C and
   GRIDDB.C use, 0x40 a step. Entries 14 and 15 are 0; entry 16 (0x400) is reached by
   GRIDDB.C's hgt_val[ht + *hm]. mapdata is the level block (struct LevelBlock), MapDirty
   is set by edits to it and cleared by loading and saving. */
/* name: This file's _DATA starts at DS:1874 (ovr126's strings end at 1873, odd). hgt_val
   and mapdata lie between ovr126's data and MapDirty; of the two files there, ovr127 is
   Okumura's LZSS.C and uses neither, this file loads the map mapdata points at. FM Towns
   has hgt_val as the same 34 bytes, after ovr126's strings. */
int hgt_val[17] = { 0x000, 0x040, 0x080, 0x0C0, 0x100, 0x140, 0x180, 0x1C0, 0x200,
                    0x240, 0x280, 0x2C0, 0x300, 0x340, 0, 0, 0x400 };      /* DS:1874 */
struct Tile far *mapdata = 0;           /* DS:1896 */
/* name: FM Towns _MapDirty: its Map_Load_ and Map_Save_ clear it as these do. DS:189A. */
unsigned char MapDirty = 0;


/* Allocates the level block on first use and lays the object store out in it. A failed
   allocation is fatal (first_punt, low memory). */
char far Map_Init(void)
{
    if (mapdata == 0) {
        if ((mapdata = farmalloc(0x7E08L)) == 0)
            first_punt(ERR_LOWMEM | 2);
    }
    Map_ObjFix();
    return 1;
}

/* Sets all 4096 tiles to one 4-byte tile value and marks the map dirty. Nothing in the
   sources calls it, and FM Towns has no name for it; probably an editor routine. */
void far OverwriteAllTiles_ovr128_37(unsigned long *tile)
{
    unsigned long far *p;
    unsigned i;

    p = (unsigned long far *)mapdata;
    for (i = 0; i < 0x1000; i++) {
        *p = *tile;
        p++;
    }
    MapDirty = 1;
}

/* Reads level `level` (1-based) from LEV.ARK into mapdata. folderType bit 1 reads the
   save directory (HomeDir) rather than DATA\; bit 0 closes the archive afterwards,
   otherwise it stays open for the texture and automap blocks (GetLevel passes 2 and
   closes it itself). arc is not read. A block without the "uw" magic word is fatal:
   pfatal_code(3) prints "Error code A003". On success the free-list and active-list
   pointers are set from the three counts before the magic word and MapDirty is cleared.
   Returns 0 only when the archive cannot be opened. */
unsigned char far Map_Load(int arc, int level, int folderType)
{
    unsigned far *end;                  /* the block's magic word; the counts before it */

    end = &LEVEL->magic;
    *end = 0;
    if (!open_arc(1,
            folderType & 2 ? HomeDir : "DATA\\"))
        return 0;
    get_arc(1, level - 1, (char far *)mapdata);
    if ((folderType & 1) || *end != LEVEL_MAGIC)
        close_arc(1);
    if (*end != LEVEL_MAGIC) {
        pfatal_code(3);
    } else {
        critptr = critbot + end[-2];        /* nmobfree */
        objptr = objbot + end[-1];          /* nstaticfree */
        LastActiveMob = ActiveMob + end[-3]; /* nactive */
        MapDirty = 0;
    }
    Anim_Load((char far *)LEVEL->anims);
    return 1;
}

/* Writes mapdata back as level `level` of LEV.ARK: stores the list counts and the magic
   word, saves the animation lists into the block, then checks and cleans the object
   lists (ObjCrunch zeroes the free records). If that check fails the player is asked a
   yes/no question (string 0x96) and a "no" abandons the save. folderType as for
   Map_Load, except that a save directory that cannot be opened is not retried in DATA\.
   Returns put_arc's result. */
char far Map_Save(int arc, int level, int folderType)
{
    unsigned far *end;
    unsigned result;
    unsigned char answer;

    end = &LEVEL->magic;
    end[-3] = LastActiveMob - ActiveMob;    /* nactive */
    end[-2] = ((long)FP_OFF(critptr)       /* nmobfree */
        - (long)FP_OFF(critbot)) / 2L;
    end[-1] = ((long)FP_OFF(objptr)        /* nstaticfree */
        - (long)FP_OFF(objbot)) / 2L;
    *end = LEVEL_MAGIC;
    MapDirty = 0;
    Anim_Save((char far *)LEVEL->anims);
    if (!ObjCrunch(0)) {
        answer = 0;
        wyorn(0, 0x96, &answer);
        if (!answer)
            return 0;
    }
    if (open_arc(1,
            folderType & 2 ? HomeDir : "DATA\\") == 0)
        return 0;
    result = put_arc(1, level - 1,
                         (char far *)mapdata, sizeof(struct LevelBlock));
    if (folderType & 1)
        close_arc(1);
    return result;
}

/* Anim_Load and Anim_Save move the level block's animation list (64 six-byte struct
   Anim) and timer list (64 words) to and from animlist and timerlist. The counts are not
   stored: Anim_Load finds them as the first entry with a zero object index and the first
   zero timer, and Anim_Save zeroes everything past them before copying out. */
/* name: FM Towns Anim_Load_ and Anim_Save_ follow Map_Save_ here and do the same copies
   (Map_Load_ and Map_Save_ call them as these are called); the names' keys also put them
   in the EXE's overlay stub order. */
char far Anim_Load(char far *source)
{
    int count;

    animcount = 0;
    timercount = 0;
    movedata(FP_SEG(source), FP_OFF(source), FP_SEG(animlist),
                                    FP_OFF(animlist), 0x180);
    movedata(FP_SEG(source + 0x180), FP_OFF(source + 0x180), FP_SEG(timerlist),
                                    FP_OFF(timerlist), 0x80);
    for (count = 0; count < 0x40; count++)
        if (animlist[count].link.f.index == 0)
            break;
    animcount = count;
    count = 0;
    while (count < 0x40) {
        if (timerlist[count] == 0)
            break;
        count++;
    }
    timercount = count;
    return 1;
}

char far Anim_Save(char far *destination)
{
    mem_set((char far *)&animlist[animcount], 0,
                             0x180 - animcount * 6);
    mem_set((char far *)&timerlist[timercount], 0,
                             0x80 - timercount * 2);
    movedata(FP_SEG(animlist), FP_OFF(animlist),
                                    FP_SEG(destination), FP_OFF(destination), 0x180);
    movedata(FP_SEG(timerlist), FP_OFF(timerlist),
                                    FP_SEG(destination + 0x180), FP_OFF(destination + 0x180), 0x80);
    return 1;
}
