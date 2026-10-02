/* target: seg029_2A8E */
/* opts: -mm -1 -G -O -d -Y */
/* The object lists: the map's object storage, allocating and freeing objects, adding
   and removing them from lists, the conversions between object pointers and indices,
   searching lists, pressure-plate weights, consistency checks and garbage collection.
   The whole of DOS resident segment seg029_2A8E, in original order. Function and global
   names are the originals from the FM Towns symbol table where it has them.

   Every object of a level lives in the level block (struct LevelBlock, level.h): 256
   mobile records of 27 bytes (critdata; index 0 is unused and index 1 is the player) and
   768 static records of 8 bytes (objdata, indices 0x100-0x3FF). An object is named by its
   index, and lists are chained through link words (union Link): each tile's list head,
   each object's next link (qn) and, unless the object is is_quant, its contents link
   (ol). Free objects sit on two stacks of indices, critbot..critptr (254 mobile, indices
   2 to 0xFF) and objbot..objptr (768 static); Obj_Alloc pops one and Obj_Free pushes it
   back. ActiveMob..LastActiveMob is a byte list of the mobile objects in use, which the
   critter code walks each frame.
   Main entry points: Obj_Alloc and Obj_Free; Obj_Add, Obj_AddEnd and Obj_Rem on a list;
   Obj_Punt, which deletes an object and its contents; Obj_PtrTMem, Obj_IntTMem and
   Obj_MemTPtr, between indices and pointers; the searches Obj_Find, Obj_InList,
   Obj_FindInMap; check_weight for pressure plates (TRIGGER.C); Obj_GarbageCollect, which
   culls distant objects when a free list runs out and when the player sleeps (SKILLS.C);
   ObjCrunch, the consistency check MAP.C runs before saving a level. Map_ObjFix (called by
   MAP.C's Map_Init and UWEDIT.C's clearobj) lays the store out empty. Nearly every
   subsystem calls into this file.
   Name: inferred (the Obj_ prefix, and System Shock's object lists are OBJECTS.C).

   match: -Y: Obj_Elem_Fate passes chkTenacious as a far function pointer, and the EXE
   pushes the segment as a relocated constant (push 28A1h) where the switches without -Y
   give push cs; nothing else in the file changes with it. */

#include <stdlib.h>
#include "event.h"
#include "file.h"
#include "inv.h"
#include "map.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"
#include "uw2.h"
#include "view3d.h"

/* An object pointer's index in the object store. */
#define MEMTPTR(o)      ((o) < (struct Object far *)objdata ? (o) - critdata \
                         : (struct StaticObj far *)(o) - objdata + NUM_MOBILE)

/* The object store, laid out in the map block by Map_ObjFix. */
/* match: this is the file's _BSS, DS:2588..25BB. Turbo C lays out uninitialised data by a
   hash of the names, not in definition order, so the names fix the layout: the publics are
   the FM Towns names and land in the EXE's order. */
/* name: the four statics have no FM Towns names (they are _Valor+0x47 to +0x4D there); the
   names below are ours, picked from candidates compiled as probes because they hash into
   the right places (mobcount and objcount after FM Towns's animcount and timercount). */
struct Object far *critdata;            /* the mobile objects */
unsigned char far *LastActiveMob;       /* the end of the active mobile list */
union Link far *Obj_Find_Head;         /* the list in which Obj_Find found its object */
static int mobcount;                    /* Obj_ListOkay's counts */
unsigned char far *ActiveMob;           /* the active mobile list */
static int total_weight;                /* check_weight's running total */
static int cull_range;                  /* chkTenacious's limit */
unsigned far *objtop;                   /* the free static list */
unsigned far *objbot;
unsigned far *objptr;
unsigned far *crittop;                  /* the free mobile list */
unsigned far *critbot;
unsigned far *critptr;
struct StaticObj far *objdata;          /* the static objects */
static int objcount;

/* Empties the object store: clears every tile's object list, points critdata, objdata and
   the free stacks into the level block, fills the free stacks with every index from 2 to
   0x3FF (mobile 2-0xFF, then static 0x100-0x3FF; 0 is "none" and 1 the player), clears the
   player's links and inventory, and empties the active mobile list and the animation and
   timer counts. MAP.C's Map_Load then overwrites the stacks with the level's own. */
void far Map_ObjFix(void)
{
    struct Tile far *t;
    unsigned far *p;
    int i;

    for (t = mapdata, i = 0; i < MAP_SIZE * MAP_SIZE; i++, t++)
        t->objects.f.index = 0;
    critdata = LEVEL->mobile;
    objdata = (struct StaticObj far *)(critdata + NUM_MOBILE);
    critbot = LEVEL->mobfree;
    critptr = crittop = critbot + 0xFD;
    objbot = crittop + 1;
    objptr = objtop = objbot + 0x2FF;
    for (p = critbot, i = 2; i < 0x400; i++, p++)
        *p = i;
    if (ThePlayer) {
        ThePlayer->ol.f.link = ThePlayer->qn.f.next = 0;
        ClearInventory();
    }
    animcount = 0;
    timercount = 0;
    ActiveMob = (unsigned char far *)(objtop + 1);
    LastActiveMob = ActiveMob;
}

/* Returns 1 if fn is true for obj or anything after it in its list, searching each
   object's contents depth first before moving to the next one; 0 for an empty list. */
unsigned char far Obj_Check(struct Object far *obj, unsigned char (far *fn)(struct Object far *obj))
{
    if (obj == 0)
        return 0;
    while (!(*fn)(obj)) {
        if (!OBJ_ISQUANT(obj) && obj->ol.f.link > 0) {
            /* match: the EXE compares and discards the result; FM Towns also
               calls Obj_PtrTMem twice here, and drops the comparison */
            if (Obj_PtrTMem(&obj->ol.link) == ThePlayer)
                ;
            if (Obj_Check(Obj_PtrTMem(&obj->ol.link), fn))
                return 1;
        }
        if (obj->qn.f.next == 0)
            return 0;
        obj = Obj_PtrTMem(&obj->qn.link);
    }
    return 1;
}

/* Whether an object resists culling: id bit 13 set, or its ComObj tenacity plus half of
   (quantity - 1) is above cull_range. A big stack is harder to cull than a single item. */
/* name: IDA ObjectCullingRngTest; FM Towns chkTenacious_ sits at the same place, between
   Obj_Check_ and Obj_Elem_Fate_, and does the same: flag bit 13, half the quantity plus
   the 4-bit field at byte 9 of ComObjData, compared with the static Obj_Elem_Fate sets.
   Obj_Elem_Fate passes it to Obj_Check in both builds. */
unsigned char far chkTenacious(struct Object far *obj)
{
    int extra;

    if (OBJ_DOORDIR(obj))
        return 1;
    if (OBJ_ISQUANT(obj) && !(obj->ol.f.link & LINK_SPECIAL))
        extra = obj->ol.f.link - 1;
    else
        extra = 0;
    return ComObjData[OBJ_ITEM(obj)].tenacity + extra / 2 > cull_range;
}

/* Decides whether an object may be deleted (1) or must stay (0). A nonzero range gains a
   random 0 to 2; the object stays if it or anything inside it is tenacious against that
   (chkTenacious), and otherwise goes with chance range in 10. So range 0 never deletes,
   and Obj_Punt's range 10 deletes everything but objects with tenacity above 10 to 12
   (or holding one). */
unsigned char far Obj_Elem_Fate(int range, struct Object far *obj)
{
    if (obj == 0)
        return 0;
    if (range)
        range += (int)(((long)rand() * 3) / 0x8000L);
    cull_range = range;
    if (chkTenacious(obj))
        return 0;
    if (!OBJ_ISQUANT(obj) && obj->ol.f.link > 0) {
        if (Obj_Check(Obj_PtrTMem(&obj->ol.link), chkTenacious))
            return 0;
    }
    if ((int)(((long)rand() * 10) / 0x8000L) < cull_range)
        return 1;
    return 0;
}

unsigned char far Obj_Fate(int range, union Link far *head)
{
    struct Object far *obj;

    if (head->f.index == 0)
        return 0;
    obj = Obj_PtrTMem(head);
    return Obj_Elem_Fate(range, obj);
}

/* Deletes up to count objects (with their contents) from tiles more than 10 - range tiles
   from the player (taxicab distance), each tile's list in order, using Obj_Fate's random
   rule. Obj_Alloc calls it with range 3 when a free list is empty (5 mobile or 10 static
   objects, whichever kind is freed); SKILLS.C calls it with range 1 for 20 objects when
   the player sleeps. */
void far Obj_GarbageCollect(int range, int count)
{
    union Link far *tilehead;
    union Link link;
    int x;
    int y;
    int dy;
    int px;
    int py;
    struct Tile far *t;
    int freed;
    int next;

    px = OBJ_HOMEX(ThePlayer);
    py = OBJ_HOMEY(ThePlayer);
    t = mapdata;
    for (freed = y = 0; y < MAP_SIZE; y++) {
        dy = abs(py - y);
        for (x = 0; x < MAP_SIZE; t++, x++) {
            if (dy + abs(px - x) > 10 - range) {
                tilehead = &t->objects;
                link = *tilehead;
                while (link.f.index > 0) {
                    next = Obj_PtrTMem(&link)->qn.f.next;
                    if (Obj_Fate(range, &link)) {
                        Obj_FreeLinkChain(tilehead, Obj_IntTMem(link.f.index));
                        freed++;
                        if (freed >= count)
                            return;
                    }
                    link.f.index = next;
                }
            }
        }
    }
}

/* Pops a free mobile (mobile nonzero) or static object, garbage collecting once if the
   stack is empty; returns 0 if still none. A mobile object is added to the active list.
   The record is not cleared here (CreateObj in MAPADDR.C fills it in). */
struct Object far * far Obj_Alloc(char mobile)
{
    if (mobile) {
        if (critptr < critbot) {
            Obj_GarbageCollect(3, 5);
            if (critptr < critbot)
                return 0;
        }
        active_critter(*critptr);
        return critdata + *critptr--;
    } else {
        if (objptr < objbot) {
            Obj_GarbageCollect(3, 10);
            if (objptr < objbot)
                return 0;
        }
        return (struct Object far *)(objdata + (*objptr-- - NUM_MOBILE));
    }
}

/* Pushes an object back on its free stack; a mobile one also leaves the active list and,
   if it is the camera object (UsPtr), releases the camera. It must already be off any
   list. */
void far Obj_Free(struct Object far *obj)
{
    if (obj < (struct Object far *)objdata) {
        critptr++;
        *critptr = obj - critdata;
        if (obj == UsPtr)
            release_camera(*critptr);
        free_critter(*critptr);
    } else {
        objptr++;
        *objptr = (struct StaticObj far *)obj - objdata + NUM_MOBILE;
    }
}

/* Obj_Add puts obj at the head of a list, Obj_AddEnd at its tail; Obj_Rem unlinks it and
   returns 1, or 0 if it was not in the list (1 for a null obj). */
void far Obj_Add(union Link far *head, struct Object far *obj)
{
    obj->qn.f.next = head->f.index;
    head->f.index = MEMTPTR(obj);
}

void far Obj_AddEnd(union Link far *head, struct Object far *obj)
{
    union Link far *p;
    struct Object far *o;

    for (p = head; (o = Obj_PtrTMem(p)) != 0; p = &o->qn.link)
        ;
    obj->qn.f.next = 0;
    p->f.index = MEMTPTR(obj);
}

unsigned char far Obj_Rem(union Link far *head, struct Object far *obj)
{
    union Link far *p;
    struct Object far *o;

    if (obj == 0)
        return 1;
    for (p = head; (o = Obj_PtrTMem(p)) != 0; p = &o->qn.link) {
        if (o == obj) {
            p->f.index = obj->qn.f.next;
            obj->qn.f.next = 0;
            return 1;
        }
    }
    return 0;
}

/* Deletes obj and its contents, from list head if given (otherwise it is assumed to be
   off every list), when force is set or Obj_Elem_Fate(10) allows it. An animated object
   also leaves the animation list. Returns 0 when deleted, else obj. */
struct Object far * far Obj_Punt(union Link far *head, struct Object far *obj, char force)
{
    union Link link;

    if (force || Obj_Elem_Fate(10, obj)) {
        link.f.index = Obj_MemTPtr(obj);
        if (OBJ_MAJOR(obj) == MAJOR_ANIMOBJ)
            rem_anim_from_list(link.f.index);
        if (head)
            Obj_FreeLinkChain(head, obj);
        else
            Obj_FreeChain(&link);
        return 0;
    }
    return obj;
}

/* Frees every object in a list, last first, and everything inside them. A trap goes
   through trap_obj_del instead (the trap code unlinks and frees it). */
void far Obj_FreeChain(union Link far *head)
{
    struct Object far *obj;

    obj = Obj_PtrTMem(head);
    if (obj == 0)
        return;
    if (obj->qn.f.next > 0)
        Obj_FreeChain(&obj->qn.link);
    if (OBJ_MAJOR(obj) == MAJOR_TRAP) {
        trap_obj_del(head, obj);
    } else {
        if (!OBJ_ISQUANT(obj) && obj->ol.f.link > 0)
            Obj_FreeChain(&obj->ol.link);
        if (Obj_Rem(head, obj))
            Obj_Free(obj);
    }
}

void far Obj_FreeLinkChain(union Link far *head, struct Object far *obj)
{
    if (!OBJ_ISQUANT(obj) && obj->ol.f.link > 0)
        Obj_FreeChain(&obj->ol.link);
    if (head) {
        if (Obj_Rem(head, obj))
            Obj_Free(obj);
    } else
        Obj_Free(obj);
}

struct Object far * far Obj_PtrTMem(union Link far *link)
{
    if (link == 0 || link->f.index == 0)
        return 0;
    if (link->f.index < NUM_MOBILE)
        return critdata + link->f.index;
    return (struct Object far *)(objdata + (link->f.index - NUM_MOBILE));
}

int far Obj_MemTPtr(struct Object far *obj)
{
    if (obj == 0)
        return 0;
    return MEMTPTR(obj);
}

struct Object far * far Obj_IntTMem(int index)
{
    if (index == 0)
        return 0;
    if (index < NUM_MOBILE)
        return critdata + index;
    return (struct Object far *)(objdata + index - NUM_MOBILE);
}

/* Finds the object with index index in a list, searching contents too when recurse is
   set. On success Obj_Find_Head is the list head it was in (top level only: a match
   inside a container leaves it at the container's list). */
struct Object far * far Obj_Find(union Link far *head, char recurse, int index)
{
    struct Object far *found;
    struct Object far *obj;

    if (head->f.index == 0)
        return 0;
    Obj_Find_Head = head;
    for (obj = Obj_PtrTMem(head); Obj_MemTPtr(obj) != index; obj = Obj_PtrTMem(&obj->qn.link)) {
        if (recurse && !OBJ_ISQUANT(obj) && obj->ol.f.link != 0) {
            if ((found = Obj_Find(&obj->ol.link, 1, index)) != 0)
                return found;
        }
        if (obj->qn.f.next == 0)
            return 0;
    }
    Obj_Find_Head = head;
    return obj;
}

unsigned char far IsMobElem(struct Object far *obj)
{
    if (obj == 0)
        return 0;
    if (obj < (struct Object far *)objdata)
        return 1;
    return 0;
}

/* Appends a mobile index to the active list. */
/* name: IDA AddtoEndOfTileMapMobiles; FM Towns active_critter_ is at the same place after
   IsMobElem_ and appends a byte at LastActiveMob the same way. */
void far active_critter(int index)
{
    *LastActiveMob++ = index;
}

/* Removes a mobile index from the active list, moving the last entry into the hole. */
/* name: IDA RemoveFromMobilesList; FM Towns free_critter_ follows active_critter_ and does
   the same search of ActiveMob..LastActiveMob. */
/* match: the byte compare (cmp al,cl) with a word parameter is the cast. */
void far free_critter(int index)
{
    unsigned char far *p;

    for (p = ActiveMob; p < LastActiveMob; p++) {
        if (*p == (unsigned char)index) {
            LastActiveMob--;
            if (p < LastActiveMob)
                *p = *LastActiveMob;
            return;
        }
    }
}

/* Whether an index is on a free list. */
/* name: no FM Towns counterpart (it goes straight from free_critter_ to Obj_InList_) and
   nothing calls it, so it keeps the IDA name. */
unsigned char far UNREFERENCED_seg029_2A8E_B04(int index)
{
    unsigned far *p;

    if (index <= 0xFF) {
        for (p = critptr; p >= critbot; p--)
            if (*p == index)
                return 1;
    } else {
        for (p = objptr; p >= objbot; p--)
            if (*p == index)
                return 1;
    }
    return 0;
}

/* Finds the first object in *head's list whose major class, minor class and index within
   the class match (-1 matches anything), searching contents when recurse is set. When the
   object is inside a container, *head is changed to the container's list. */
struct Object far * far Obj_InList(union Link far **head, char recurse, int major, int minor, int index)
{
    struct Object far *obj;
    struct Object far *found;
    union Link far *sub;
    union Link far *list;

    for (obj = Obj_PtrTMem(*head); obj; obj = Obj_PtrTMem(&obj->qn.link)) {
        if ((major == -1 || OBJ_MAJOR(obj) == major)
         && (minor == -1 || OBJ_MINOR(obj) == minor)
         && (index == -1 || OBJ_INCLASS(obj) == index))
            return obj;
        if (recurse && !OBJ_ISQUANT(obj) && (sub = &obj->ol.link)->f.index != 0) {
            list = sub;
            if ((found = Obj_InList(&list, recurse, major, minor, index)) != 0) {
                *head = list;
                return found;
            }
        }
    }
    return 0;
}

/* Whether obj is item id or holds one anywhere inside it. */
unsigned char far HasOrIsObj(struct Object far *obj, int id)
{
    union Link far *list;

    if (OBJ_ITEM(obj) == id)
        return 1;
    if (!OBJ_ISQUANT(obj)) {
        list = &obj->ol.link;
        if (Obj_InList(&list, 1, id >> 6, (id & ID_MINOR) >> 4, id & ID_INCLASS))
            return 1;
    }
    return 0;
}

/* Scans the map's tile lists (and their contents) for a match from tile (*x, *y) onward,
   row by row, and returns it with *x and *y at its tile. The caller resumes the search by
   advancing *x; an *x past the row moves to the next row. */
struct Object far * far Obj_FindInMap(int major, int minor, int index, int *x, int *y)
{
    struct Tile far *t;
    union Link far *head;
    struct Object far *found = 0;

    if (*x >= MAP_SIZE) {
        *x = 0;
        (*y)++;
    }
    t = mapdata + (*y << 6) + *x;
    for (; *y < MAP_SIZE; (*y)++) {
        for (; *x < MAP_SIZE; (*x)++, t++) {
            head = &t->objects;
            if (head->f.index > 0) {
                if ((found = Obj_InList(&head, 1, major, minor, index)) != 0)
                    return found;
            }
        }
        *x = 0;
    }
    return 0;
}

struct Object far * far Obj_FindInMapSquare(int major, int minor, int index, int x, int y)
{
    union Link far *head;

    head = &Map_GetAddr(x, y)->objects;
    return Obj_InList(&head, 0, major, minor, index);
}

/* The weight on a list, in ComObj mass units (tenths of a stone), counting each stack's
   quantity and the contents of containers; only objects at height z unless z is negative.
   When adjust is not -1, the player counts adjust instead of the player's inventory.
   min -1 returns the total; min 0 or above returns whether the total exceeds min,
   stopping early; min -2 is the recursive call, adding to the running total. TRIGGER.C
   uses it for pressure plates. */
int far check_weight(union Link far *head, int min, int z, int adjust)
{
    struct Object far *obj;
    int quantity;

    if (min != -2)
        total_weight = 0;
    for (obj = Obj_PtrTMem(head); obj; obj = Obj_PtrTMem(&obj->qn.link)) {
        if (z < 0 || OBJ_Z(obj) == z) {
            if (OBJ_ISQUANT(obj) && !(obj->ol.f.link & LINK_SPECIAL))
                quantity = obj->ol.f.link;
            else
                quantity = 1;
            total_weight = total_weight + ComObjData[OBJ_ITEM(obj)].mass * quantity;
            if (adjust != -1 && obj == ThePlayer)
                total_weight += adjust;
            else if (!OBJ_ISQUANT(obj) && &obj->ol.link != 0)
                check_weight(&obj->ol.link, -2, z, adjust);
            if (min >= 0 && total_weight > min)
                return 1;
        }
    }
    if (min >= 0)
        return total_weight > min;
    return total_weight;
}

/* flog_list (a tile's list) and count_list (any chain) count how often each index is
   reached, for Obj_ListOkay; an object reached twice makes them return 1 ("bad"). */
unsigned char far flog_list(int x, int y, unsigned char *counts, union Link far *head)
{
    struct Object far *obj;
    unsigned char bad = 0;
    int i;

    for (obj = Obj_PtrTMem(head); obj; obj = Obj_PtrTMem(&obj->qn.link)) {
        i = Obj_MemTPtr(obj);
        if (++counts[i] > 1) {
            bad = 1;
            if (counts[i] > 10)
                return 1;
        }
        if (!OBJ_ISQUANT(obj) && obj->ol.f.link > 0)
            bad = flog_list(x, y, counts, &obj->ol.link);
        if (IsMobElem(obj))
            mobcount++;
        else
            objcount++;
    }
    return bad;
}

unsigned char far count_list(struct Object far *obj, unsigned char *counts)
{
    unsigned char bad = 0;
    int i;

    for (; obj; obj = Obj_PtrTMem(&obj->qn.link)) {
        i = Obj_MemTPtr(obj);
        if (++counts[i] > 1) {
            bad = 1;
            if (counts[i] > 10)
                return 1;
        }
        if (!OBJ_ISQUANT(obj) && obj->ol.f.link > 0)
            bad = count_list(Obj_PtrTMem(&obj->ol.link), counts);
        if (IsMobElem(obj))
            mobcount++;
        else
            objcount++;
    }
    return bad;
}

/* Checks the object store: no index twice on the free stacks or in the lists, and every
   object either free or reachable from a tile, the player (when GrSq is negative,
   probably when the player is off the map) or the object on the cursor. Returns 1 when
   all is consistent. ObjCrunch passes its argument on, but nothing here reads it. */
unsigned char far Obj_ListOkay(char how)
{
    unsigned far *p;
    int nstatic;
    int y;
    int nmobile;
    struct Tile far *t;
    union Link far *head;
    unsigned char bad;
    int x;
    unsigned char *counts;

    if ((counts = calloc(1, 0x400)) == 0)
        return 1;
    bad = 0;
    nmobile = 0;
    for (p = critptr; p >= critbot; p--) {
        if (counts[*p])
            bad = 1;
        counts[*p]++;
        nmobile++;
    }
    nstatic = 0;
    for (p = objptr; p >= objbot; p--) {
        if (counts[*p])
            bad = 1;
        counts[*p]++;
        nstatic++;
    }
    objcount = mobcount = 0;
    t = mapdata;
    for (y = 0; y < MAP_SIZE; y++) {
        for (x = 0; x < MAP_SIZE; x++, t++) {
            head = &t->objects;
            if (head->f.index > 0)
                bad = flog_list(x, y, counts, head) || bad;
        }
    }
    if (GrSq < 0)
        bad = count_list(ThePlayer, counts) || bad;
    if (GameInputMode == 1 || GameInputMode == 0 && CursorObjPtr != 0)
        bad = count_list(CursorObjPtr, counts) || bad;
    if (nmobile + mobcount != 0xFF)
        bad = 1;
    if (nstatic + objcount != NUM_STATIC)
        bad = 1;
    free(counts);
    return !bad;
}

/* Called by MAP.C before a level is saved: if the lists are consistent, zeroes every free
   record (so the saved block is clean) and returns 1; otherwise reports "badobjlist"
   through errmsg and returns 0. */
unsigned char far ObjCrunch(char how)
{
    unsigned far *p;

    if (!Obj_ListOkay(how)) {
        errmsg("Cantcrunch", "badobjlist");
        return 0;
    }
    for (p = critptr; p >= critbot; p--)
        mem_set(Obj_IntTMem(*p), 0, MOBILE_SIZE);
    for (p = objptr; p >= objbot; p--)
        mem_set(Obj_IntTMem(*p), 0, 8);
    return 1;
}
