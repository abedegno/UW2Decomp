/* target: ovr128 */
/* opts: -mm -1 -G -O -Y -d */

#include <dos.h>

extern char far * near mapdata;
extern unsigned char map_dirty;
extern char far *ActiveMob;
extern char far *LastActiveMob;
extern char far *static_free_list;
extern char far *static_free_ptr;
extern char far *mobile_free_list;
extern char far *mobile_free_ptr;
extern char animcount;
extern unsigned char timer_triggers[];
extern char timercount;
extern unsigned char anim_overlays[];
struct OverlayWord { unsigned pad:6; unsigned id:10; };

extern char HomeDir[];

extern void far * far farmalloc(unsigned long size);
extern void far first_punt(int code);
extern void far Map_ObjFix(void);
extern unsigned char far open_arc(int type, char *folder);
extern int far get_arc(int type, int block, void far *dst);
extern unsigned char far put_arc(int type, int block, void far *src, unsigned size);
extern void far close_arc(int close);
extern void far pfatal_code(int code);
extern unsigned char far ObjCrunch(char n);
extern int far wyorn(int a, int b, char *answer);
extern void far mem_set(void far *dst, char value, int size);
extern void far movedata(unsigned srcseg, unsigned srcoff,
                                                unsigned dstseg, unsigned dstoff, unsigned size);

char far LoadAnimationOverlays_ovr128_271(char far *source);
char far StoreAnimationOverlaysToLevArk_ovr128_308(char far *destination);

char far Map_Init(void)
{
    if (mapdata == 0) {
        if ((mapdata = farmalloc(0x7E08L)) == 0)
            first_punt(0x1002);
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
    map_dirty = 1;
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
        mobile_free_ptr = (char far *)mobile_free_list
            + *(unsigned far *)(end - 4) * 2;
        static_free_ptr = (char far *)static_free_list
            + *(unsigned far *)(end - 2) * 2;
        LastActiveMob =
            (char far *)ActiveMob + *(unsigned far *)(end - 6);
        map_dirty = 0;
    }
    LoadAnimationOverlays_ovr128_271((char far *)mapdata + 0x7C08);
    return 1;
}

char far Map_Save(int arc, int level, int folderType)
{
    unsigned char far *end;
    unsigned result;
    unsigned char answer;

    end = (unsigned char far *)mapdata + 0x7C06;
    *(unsigned far *)(end - 6) = LastActiveMob - ActiveMob;
    *(unsigned far *)(end - 4) = ((long)FP_OFF(mobile_free_ptr)
        - (long)FP_OFF(mobile_free_list)) / 2L;
    *(unsigned far *)(end - 2) = ((long)FP_OFF(static_free_ptr)
        - (long)FP_OFF(static_free_list)) / 2L;
    *(unsigned far *)end = 0x7577;
    map_dirty = 0;
    StoreAnimationOverlaysToLevArk_ovr128_308((char far *)mapdata + 0x7C08);
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

char far LoadAnimationOverlays_ovr128_271(char far *source)
{
    int count;

    animcount = 0;
    timercount = 0;
    movedata(FP_SEG(source), FP_OFF(source), FP_SEG(anim_overlays),
                                    FP_OFF(anim_overlays), 0x180);
    movedata(FP_SEG(source + 0x180), FP_OFF(source + 0x180), FP_SEG(timer_triggers),
                                    FP_OFF(timer_triggers), 0x80);
    for (count = 0; count < 0x40; count++)
        if (((struct OverlayWord *)(anim_overlays + count * 6))->id == 0)
            break;
    animcount = count;
    count = 0;
    while (count < 0x40) {
        if (*(unsigned *)(timer_triggers + count * 2) == 0)
            break;
        count++;
    }
    timercount = count;
    return 1;
}

char far StoreAnimationOverlaysToLevArk_ovr128_308(char far *destination)
{
    mem_set((char far *)anim_overlays
                             + animcount * 6, 0,
                             0x180 - animcount * 6);
    mem_set(timer_triggers
                             + timercount * 2, 0,
                             0x80 - timercount * 2);
    movedata(FP_SEG(anim_overlays), FP_OFF(anim_overlays),
                                    FP_SEG(destination), FP_OFF(destination), 0x180);
    movedata(FP_SEG(timer_triggers), FP_OFF(timer_triggers),
                                    FP_SEG(destination + 0x180), FP_OFF(destination + 0x180), 0x80);
    return 1;
}
