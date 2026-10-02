/* target: ovr128 */
/* opts: -mm -1 -G -O -Y -d */

#include <dos.h>
#include "file.h"
#include "map.h"
#include "object.h"
#include "sys.h"

/* This file's _DATA starts at DS:1874 (ovr126's strings end at 1873, odd). hgt_val and
   mapdata lie between ovr126's data and MapDirty; of the two files there, ovr127 is
   Okumura's LZSS.C and uses neither, this file loads the map mapdata points at. FM Towns
   has hgt_val as the same 34 bytes, after ovr126's strings. */
int hgt_val[17] = { 0x000, 0x040, 0x080, 0x0C0, 0x100, 0x140, 0x180, 0x1C0, 0x200,
                    0x240, 0x280, 0x2C0, 0x300, 0x340, 0, 0, 0x400 };      /* DS:1874 */
char far * near mapdata = 0;            /* DS:1896 */
/* FM Towns _MapDirty: its Map_Load_ and Map_Save_ clear it as these do. DS:189A. */
unsigned char MapDirty = 0;
extern char far *ActiveMob;
extern char far *LastActiveMob;
extern char far *objbot;
extern char far *objptr;
extern char far *critbot;
extern char far *critptr;
extern char animcount;
extern unsigned char timerlist[];
extern char timercount;
extern unsigned char animlist[];
struct OverlayWord { unsigned pad:6; unsigned id:10; };

extern void far * far farmalloc(unsigned long size);
extern int far get_arc(int type, int block, void far *dst);
extern unsigned char far put_arc(int type, int block, void far *src, unsigned size);
extern void far close_arc(int close);
extern unsigned char far ObjCrunch(char n);
extern int far wyorn(int a, int b, char *answer);
extern void far movedata(unsigned srcseg, unsigned srcoff,
                                                unsigned dstseg, unsigned dstoff, unsigned size);

char far Map_Init(void)
{
    if (mapdata == 0) {
        if ((mapdata = farmalloc(0x7E08L)) == 0)
            first_punt(ERR_LOWMEM | 2);
    }
    Map_ObjFix();
    return 1;
}

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

unsigned char far Map_Load(int arc, int level, int folderType)
{
    unsigned char far *end;

    end = (unsigned char far *)mapdata + 0x7C06;
    *(unsigned far *)end = 0;
    if (!open_arc(1,
            folderType & 2 ? HomeDir : "DATA\\"))
        return 0;
    get_arc(1, level - 1, mapdata);
    if ((folderType & 1) || *(unsigned far *)end != 0x7577)
        close_arc(1);
    if (*(unsigned far *)end != 0x7577) {
        pfatal_code(3);
    } else {
        critptr = (char far *)critbot
            + *(unsigned far *)(end - 4) * 2;
        objptr = (char far *)objbot
            + *(unsigned far *)(end - 2) * 2;
        LastActiveMob =
            (char far *)ActiveMob + *(unsigned far *)(end - 6);
        MapDirty = 0;
    }
    Anim_Load((char far *)mapdata + 0x7C08);
    return 1;
}

char far Map_Save(int arc, int level, int folderType)
{
    unsigned char far *end;
    unsigned result;
    unsigned char answer;

    end = (unsigned char far *)mapdata + 0x7C06;
    *(unsigned far *)(end - 6) = LastActiveMob - ActiveMob;
    *(unsigned far *)(end - 4) = ((long)FP_OFF(critptr)
        - (long)FP_OFF(critbot)) / 2L;
    *(unsigned far *)(end - 2) = ((long)FP_OFF(objptr)
        - (long)FP_OFF(objbot)) / 2L;
    *(unsigned far *)end = 0x7577;
    MapDirty = 0;
    Anim_Save((char far *)mapdata + 0x7C08);
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
                         mapdata, 0x7E08);
    if (folderType & 1)
        close_arc(1);
    return result;
}

/* FM Towns Anim_Load_ and Anim_Save_ follow Map_Save_ here and do the same copies (Map_Load_
   and Map_Save_ call them as these are called); the names' keys also put them in the EXE's
   overlay stub order. */
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
        if (((struct OverlayWord *)(animlist + count * 6))->id == 0)
            break;
    animcount = count;
    count = 0;
    while (count < 0x40) {
        if (*(unsigned *)(timerlist + count * 2) == 0)
            break;
        count++;
    }
    timercount = count;
    return 1;
}

char far Anim_Save(char far *destination)
{
    mem_set((char far *)animlist
                             + animcount * 6, 0,
                             0x180 - animcount * 6);
    mem_set(timerlist
                             + timercount * 2, 0,
                             0x80 - timercount * 2);
    movedata(FP_SEG(animlist), FP_OFF(animlist),
                                    FP_SEG(destination), FP_OFF(destination), 0x180);
    movedata(FP_SEG(timerlist), FP_OFF(timerlist),
                                    FP_SEG(destination + 0x180), FP_OFF(destination + 0x180), 0x80);
    return 1;
}
