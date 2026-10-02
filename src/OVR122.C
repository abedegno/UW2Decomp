/* target: ovr122 */
/* opts: -mm -1 -G -O -Y -d */
/* Saving and restoring the player's inventory in player.dat: the player object, the object
   in the cursor and every object reachable from them are copied into the workspace, with
   their links renumbered to the copy's own indexes, and back again. The whole of DOS
   overlay ovr122, in original order. Function names are the originals from the FM Towns
   symbol table (the four helpers IDA left as ovr122_336, _38A, _3A4 and _3C6 are
   replaceInInv, allocSaveObj, getSaveObj and putInInv, the FM Towns functions at the same
   positions doing the same work); the source file's own name is not known.

   The workspace holds the player object at 0, the cursor object at 0x1B, a copy of the
   inventory slots at 0x23, and from 0x5B the saved objects, numbered from 1. */

#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <stat.h>                       /* sys\stat.h; the build keeps it flat */
#include <string.h>
#include "file.h"
#include "inv.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"
#include "uw2.h"

#define OBJ_ISQUANT(o)  (((o)->id & ID_ISQUANT) >> 15)
#define OBJ_HOMEX(o)    (((o)->home & HOME_X) >> 10)
#define OBJ_HOMEY(o)    (((o)->home & HOME_Y) >> 4)

extern union Link Inventory[];

/* FM Towns keeps these as statics after BagSaveHandles, so their names are not known; these
   were chosen to land in _BSS in the EXE's order (DS:6A86, 6A8A, 6A8E, 6A90, 6A94). */
static union Link far *invSlots;        /* the inventory slots in the workspace */
static struct StaticObj far *saveObjs;     /* the saved objects, numbered from 1 */
static int saveNum;                     /* the number of saved objects */
static void far *saveBuf;               /* the workspace */
static char cursorSaved;                /* the copy holds a cursor object */

struct Object far * far Obj_PtrTMem(unsigned far *link);
void far Obj_FreeChain(unsigned far *head);
struct Object far * far Obj_Alloc(char mobile);
char far Obj_Rem(unsigned far *head, struct Object far *obj);
unsigned far get_workspace(void);

void far Punt_player_inv(void)
{
    FixBagArea();
    FreePlayerInv(&ThePlayer->ol.word);
    ClearInventory();
}

void far makePlayerInvCopy(void far *ws)
{
    struct Object far *p;
    struct StaticObj far *cursor;
    union Link chain;
    int i;

    FixBagArea();
    p = ws;
    *p = *ThePlayer;
    p->qn.f.next = 0;
    cursor = (struct StaticObj far *)(p + 1);
    invSlots = (union Link far *)(cursor + 1);
    saveObjs = (struct StaticObj far *)(invSlots + 0x1C);
    saveNum = 0;
    for (i = 0; i <= 0x12; i++)
        invSlots[i].f.index = invSlots[i].f.low = 0;
    InvSaveNexts(&ThePlayer->ol.word, &p->ol.word);
    if (GameInputMode == 1 || (GameInputMode == 0 && CursorObjPtr != 0)) {
        *cursor = *(struct StaticObj far *)CursorObjPtr;
        if (!OBJ_ISQUANT(CursorObjPtr))
            InvSaveNexts(&CursorObjPtr->ol.word, &cursor->ol.word);
        chain.f.index = Obj_MemTPtr(CursorObjPtr);
        Obj_FreeChain(&chain.word);
        CursorObjPtr = 0;
        cursorSaved = 1;
    } else
        cursorSaved = 0;
}

char far SavePlayerInv(char *name)
{
    char ok = 1;
    char path[0x42];
    register int fd;

    saveBuf = MK_FP(get_workspace(), 0);
    if (saveBuf == 0)
        return 0;
    makePlayerInvCopy(saveBuf);
    saveNum++;
    if (name) {
        strcpy(path, name);
        strcat(path, "player.dat");
        fd = open(path, O_RDWR | O_CREAT | O_TRUNC | O_BINARY, S_IREAD | S_IWRITE);
        if (fd >= 0) {
            save_player_data(fd);
            FarWrite_ovr167_627(fd, &saveNum, 2);
            FarWrite_ovr167_627(fd, saveBuf, saveNum * 8 + 0x5B);
            close(fd);
        } else
            ok = 0;
        release_workspace();
        saveBuf = 0;
        editchng(0x200);
    }
    return ok;
}

/* Copies the list at src, and every list inside it, to the save area, pointing dst at it. */
void far InvSaveNexts(unsigned far *src, unsigned far *dst)
{
    struct Object far *obj;
    struct Object far *copy;
    union Link far *contents;
    union Link far *copied;

    for (; obj = Obj_PtrTMem(src), obj; ) {
        copy = allocSaveObj();
        *(struct StaticObj far *)copy = *(struct StaticObj far *)obj;
        ((union Link far *)dst)->f.index = saveNum;
        replaceInInv((union Link far *)src, (union Link far *)dst);
        src = &obj->qn.word;
        dst = &copy->qn.word;
        contents = &obj->ol.link;
        copied = &copy->ol.link;
        if (!OBJ_ISQUANT(obj) && contents->f.index != 0)
            InvSaveNexts(&contents->word, &copied->word);
    }
}

/* An inventory slot holding the object at old is pointed at its copy instead. */
void far replaceInInv(union Link far *old, union Link far *new)
{
    int i;

    for (i = 0; i <= 0x12; i++)
        if (Inventory[i].f.index == old->f.index)
            invSlots[i].f.index = new->f.index;
}

struct Object far * far allocSaveObj(void)
{
    ++saveNum;
    return (struct Object far *)(saveObjs + saveNum);
}

struct Object far * far getSaveObj(int n)
{
    if (n == 0)
        return 0;
    return (struct Object far *)(saveObjs + n);
}

/* An inventory slot holding the saved object at saved is pointed at its restored copy. */
void far putInInv(union Link far *mem, union Link far *saved)
{
    int i;

    for (i = 0; i <= 0x12; i++)
        if (invSlots[i].f.index == saved->f.index)
            Inventory[i].f.index = mem->f.index;
}

/* Restores the saved list at src, and every list inside it, pointing dst at it. */
void far InvRestoreNexts(unsigned far *dst, unsigned far *src)
{
    struct Object far *obj;
    struct Object far *saved;
    union Link far *contents;
    union Link far *copied;

    for (; saved = getSaveObj(((union Link far *)src)->f.index), saved; ) {
        obj = Obj_Alloc(0);
        *(struct StaticObj far *)obj = *(struct StaticObj far *)saved;
        ((union Link far *)dst)->f.index = Obj_MemTPtr(obj);
        putInInv((union Link far *)dst, (union Link far *)src);
        dst = &obj->qn.word;
        src = &saved->qn.word;
        contents = &obj->ol.link;
        copied = &saved->ol.link;
        if (!OBJ_ISQUANT(saved) && copied->f.index != 0)
            InvRestoreNexts(&contents->word, &copied->word);
    }
}

void far FreePlayerInv(unsigned far *head)
{
    struct Object far *obj;
    union Link far *link;

    obj = Obj_PtrTMem(head);
    if (obj == 0)
        return;
    link = &obj->ol.link;
    if (!OBJ_ISQUANT(obj) && link->f.index != 0)
        FreePlayerInv(&obj->ol.word);
    link = &obj->qn.link;
    if (link->f.index != 0)
        FreePlayerInv(&link->word);
    if (Obj_Rem(head, obj))
        Obj_Free(obj);
}

void far getPlayerInvCopy(void far *ws)
{
    struct Object far *p;
    struct StaticObj far *cursor;

    p = ws;
    cursor = (struct StaticObj far *)(p + 1);
    invSlots = (union Link far *)(cursor + 1);
    saveObjs = (struct StaticObj far *)(invSlots + 0x1C);
    *ThePlayer = *p;
    InvRestoreNexts(&ThePlayer->ol.word, &p->ol.word);
    if (cursorSaved) {
        CursorObjPtr = Obj_Alloc(0);
        *(struct StaticObj far *)CursorObjPtr = *cursor;
        if (!OBJ_ISQUANT(cursor))
            InvRestoreNexts(&CursorObjPtr->ol.word, &cursor->ol.word);
    }
}

char far RestorePlayerInv(char *name)
{
    char ok = 1;
    char path[0x42];
    register int fd;
    register int sq;

    if (name) {
        sq = GrSq;
        change_GrSq(-1, -1);
    }
    Punt_player_inv();
    if (saveBuf == 0) {
        saveBuf = MK_FP(get_workspace(), 0);
        if (saveBuf == 0)
            return 0;
    }
    if (name) {
        cursorSaved = 0;
        strcpy(path, name);
        strcat(path, "player.dat");
        fd = open(path, O_RDWR | O_BINARY);
        if (fd >= 0) {
            read_player_data(fd);
            intoFarBuffer_ovr167_5DA(fd, &saveNum, 2);
            intoFarBuffer_ovr167_5DA(fd, saveBuf, saveNum * 8 + 0x5B);
            close(fd);
            load_inventory_pix();
        } else {
            ok = 0;
            goto done;
        }
    }
    getPlayerInvCopy(saveBuf);
    FixPlayerEquips();
done:
    release_workspace();
    saveBuf = 0;
    if (name) {
        sq = (OBJ_HOMEY(ThePlayer) << 6) + OBJ_HOMEX(ThePlayer);
        change_GrSq(sq, -1);
    }
    return ok;
}
