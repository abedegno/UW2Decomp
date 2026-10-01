/* target: seg029_2A8E */
/* opts: -mm -1 -G -O -d -Y */
/* The object lists: the map's object storage, allocating and freeing objects, adding
   and removing them from lists, the conversions between object pointers and indices,
   searching lists, pressure-plate weights, consistency checks and garbage collection.
   The whole of DOS resident segment seg029_2A8E, in original order. Function and global
   names are the originals from the FM Towns symbol table where it has them.
   -Y: Obj_Elem_Fate passes chkTenacious as a far function pointer, and the EXE pushes
   the segment as a relocated constant (push 28A1h) where the switches without -Y give
   push cs; nothing else in the file changes with it. */

#include <stdlib.h>

/* The low 6 bits of a list word hold the quality or owner; the top 10 bits an object
   index (0 is none, 1..0xFF a mobile object, 0x100..0x3FF a static one). */
struct Link {
    unsigned bits:6;
    unsigned index:10;
};

/* A mobile object, 0x1B bytes. */
struct Object {
    unsigned id;                        /* item 0-8 (major class 6-8), 13, is_quant 15 */
    unsigned pos;                       /* z 0-6 */
    struct Link next;                   /* 0x04, the next object in this list */
    struct Link link;                   /* 0x06, contents, or a quantity if is_quant */
    char pad8[0x16 - 0x08];
    unsigned home;                      /* 0x16, x in bits 10-15, y in bits 4-9 */
    char pad18[0x1B - 0x18];
};

/* A static object: the first 8 bytes of a mobile one. */
struct StaticObj {
    unsigned id;
    unsigned pos;
    struct Link next;
    struct Link link;
};

#define OBJ_ITEM(o)     ((o)->id & 0x1FF)
#define OBJ_MAJOR(o)    (((o)->id & 0x1C0) >> 6)
#define OBJ_MINOR(o)    (((o)->id & 0x30) >> 4)
#define OBJ_INDEX(o)    ((o)->id & 0xF)
#define OBJ_TENACIOUS(o) (((o)->id & 0x2000) >> 13)
#define OBJ_ISQUANT(o)  (((o)->id & 0x8000) >> 15)
#define OBJ_Z(o)        ((o)->pos & 0x7F)
#define OBJ_HOMEX(o)    (((o)->home & 0xFC00) >> 10)
#define OBJ_HOMEY(o)    (((o)->home & 0x3F0) >> 4)

/* An object pointer's index in the object store. */
#define MEMTPTR(o)      ((o) < (struct Object far *)objdata ? (o) - critdata \
                         : (struct StaticObj far *)(o) - objdata + 0x100)

struct Tile {
    unsigned type:4;
    unsigned height:4;
    char pad1;
    struct Link objects;                /* 0x02, head of the tile's object list */
};

/* The common object properties, one 11-byte record per item. */
struct ComObj {
    unsigned height:8;                  /* 0x00 */
    unsigned radius:3;
    unsigned c0_11:1;
    unsigned mass:12;                   /* 0x01, bits 4-15 */
    char pad3[9 - 3];
    unsigned char c9:2;                 /* 0x09 */
    unsigned char tenacity:4;
    unsigned char c9_6:2;
    char padA;
};

/* The object store, laid out in the map block by Map_ObjFix. This is the file's _BSS,
   DS:2588..25BB. Turbo C lays out uninitialised data by a hash of the names, not in
   definition order, so the names fix the layout: the publics are the FM Towns names and
   land in the EXE's order. The four statics have no FM Towns names (they are _Valor+0x47
   to +0x4D there); the names below are ours, picked from candidates compiled as probes
   because they hash into the right places (mobcount and objcount after FM Towns's
   animcount and timercount). */
struct Object far *critdata;            /* the mobile objects */
unsigned char far *LastActiveMob;       /* the end of the active mobile list */
struct Link far *Obj_Find_Head;         /* the list in which Obj_Find found its object */
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

extern struct Tile far *mapdata;
extern struct Object far *ThePlayer;
extern struct Object far *UsPtr;
extern struct Object far *CursorObjPtr;
extern int GameInputMode;
extern int GrSq;
extern unsigned char animcount;
extern unsigned char timercount;
extern struct ComObj ComObjData[];

/* Elsewhere in the game. */
void far ClearInventory(void);
void far release_camera(int index);
void far rem_anim_from_list(int index);
void far trap_obj_del(struct Link far *head, struct Object far *obj);
struct Tile far * far Map_GetAddr(int x, int y);
void far errmsg(char *msg, char *detail);
void far mem_set(void far *p, int value, int count);  /* seg017's far memset; our name */

/* This file. */
struct Object far * far Obj_PtrTMem(struct Link far *link);
int far Obj_MemTPtr(struct Object far *obj);
struct Object far * far Obj_IntTMem(int index);
void far Obj_FreeChain(struct Link far *head);
void far Obj_FreeLinkChain(struct Link far *head, struct Object far *obj);
void far active_critter(int index);
void far free_critter(int index);

void far Map_ObjFix(void)
{
    struct Tile far *t;
    unsigned far *p;
    int i;

    for (t = mapdata, i = 0; i < 0x1000; i++, t++)
        t->objects.index = 0;
    critdata = (struct Object far *)(mapdata + 0x1000);
    objdata = (struct StaticObj far *)(critdata + 0x100);
    critbot = (unsigned far *)(mapdata + 0x1CC0);
    critptr = crittop = critbot + 0xFD;
    objbot = crittop + 1;
    objptr = objtop = objbot + 0x2FF;
    for (p = critbot, i = 2; i < 0x400; i++, p++)
        *p = i;
    if (ThePlayer) {
        ThePlayer->link.index = ThePlayer->next.index = 0;
        ClearInventory();
    }
    animcount = 0;
    timercount = 0;
    ActiveMob = (unsigned char far *)(objtop + 1);
    LastActiveMob = ActiveMob;
}

unsigned char far Obj_Check(struct Object far *obj, unsigned char (far *fn)(struct Object far *obj))
{
    if (obj == 0)
        return 0;
    while (!(*fn)(obj)) {
        if (!OBJ_ISQUANT(obj) && obj->link.index > 0) {
            /* the EXE compares and discards the result; FM Towns also calls
               Obj_PtrTMem twice here, and drops the comparison */
            if (Obj_PtrTMem(&obj->link) == ThePlayer)
                ;
            if (Obj_Check(Obj_PtrTMem(&obj->link), fn))
                return 1;
        }
        if (obj->next.index == 0)
            return 0;
        obj = Obj_PtrTMem(&obj->next);
    }
    return 1;
}

/* IDA ObjectCullingRngTest; FM Towns chkTenacious_ sits at the same place, between
   Obj_Check_ and Obj_Elem_Fate_, and does the same: flag bit 13, half the quantity plus
   the 4-bit field at byte 9 of ComObjData, compared with the static Obj_Elem_Fate sets.
   Obj_Elem_Fate passes it to Obj_Check in both builds. */
unsigned char far chkTenacious(struct Object far *obj)
{
    int extra;

    if (OBJ_TENACIOUS(obj))
        return 1;
    if (OBJ_ISQUANT(obj) && !(obj->link.index & 0x200))
        extra = obj->link.index - 1;
    else
        extra = 0;
    return ComObjData[OBJ_ITEM(obj)].tenacity + extra / 2 > cull_range;
}

unsigned char far Obj_Elem_Fate(int range, struct Object far *obj)
{
    if (obj == 0)
        return 0;
    if (range)
        range += (int)(((long)rand() * 3) / 0x8000L);
    cull_range = range;
    if (chkTenacious(obj))
        return 0;
    if (!OBJ_ISQUANT(obj) && obj->link.index > 0) {
        if (Obj_Check(Obj_PtrTMem(&obj->link), chkTenacious))
            return 0;
    }
    if ((int)(((long)rand() * 10) / 0x8000L) < cull_range)
        return 1;
    return 0;
}

unsigned char far Obj_Fate(int range, struct Link far *head)
{
    struct Object far *obj;

    if (head->index == 0)
        return 0;
    obj = Obj_PtrTMem(head);
    return Obj_Elem_Fate(range, obj);
}

void far Obj_GarbageCollect(int range, int count)
{
    struct Link far *tilehead;
    struct Link link;
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
    for (freed = y = 0; y < 0x40; y++) {
        dy = abs(py - y);
        for (x = 0; x < 0x40; t++, x++) {
            if (dy + abs(px - x) > 10 - range) {
                tilehead = &t->objects;
                link = *tilehead;
                while (link.index > 0) {
                    next = Obj_PtrTMem(&link)->next.index;
                    if (Obj_Fate(range, &link)) {
                        Obj_FreeLinkChain(tilehead, Obj_IntTMem(link.index));
                        freed++;
                        if (freed >= count)
                            return;
                    }
                    link.index = next;
                }
            }
        }
    }
}

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
        return (struct Object far *)(objdata + (*objptr-- - 0x100));
    }
}

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
        *objptr = (struct StaticObj far *)obj - objdata + 0x100;
    }
}

void far Obj_Add(struct Link far *head, struct Object far *obj)
{
    obj->next.index = head->index;
    head->index = MEMTPTR(obj);
}

void far Obj_AddEnd(struct Link far *head, struct Object far *obj)
{
    struct Link far *p;
    struct Object far *o;

    for (p = head; (o = Obj_PtrTMem(p)) != 0; p = &o->next)
        ;
    obj->next.index = 0;
    p->index = MEMTPTR(obj);
}

unsigned char far Obj_Rem(struct Link far *head, struct Object far *obj)
{
    struct Link far *p;
    struct Object far *o;

    if (obj == 0)
        return 1;
    for (p = head; (o = Obj_PtrTMem(p)) != 0; p = &o->next) {
        if (o == obj) {
            p->index = obj->next.index;
            obj->next.index = 0;
            return 1;
        }
    }
    return 0;
}

struct Object far * far Obj_Punt(struct Link far *head, struct Object far *obj, char force)
{
    struct Link link;

    if (force || Obj_Elem_Fate(10, obj)) {
        link.index = Obj_MemTPtr(obj);
        if (OBJ_MAJOR(obj) == 7)
            rem_anim_from_list(link.index);
        if (head)
            Obj_FreeLinkChain(head, obj);
        else
            Obj_FreeChain(&link);
        return 0;
    }
    return obj;
}

void far Obj_FreeChain(struct Link far *head)
{
    struct Object far *obj;

    obj = Obj_PtrTMem(head);
    if (obj == 0)
        return;
    if (obj->next.index > 0)
        Obj_FreeChain(&obj->next);
    if (OBJ_MAJOR(obj) == 6) {
        trap_obj_del(head, obj);
    } else {
        if (!OBJ_ISQUANT(obj) && obj->link.index > 0)
            Obj_FreeChain(&obj->link);
        if (Obj_Rem(head, obj))
            Obj_Free(obj);
    }
}

void far Obj_FreeLinkChain(struct Link far *head, struct Object far *obj)
{
    if (!OBJ_ISQUANT(obj) && obj->link.index > 0)
        Obj_FreeChain(&obj->link);
    if (head) {
        if (Obj_Rem(head, obj))
            Obj_Free(obj);
    } else
        Obj_Free(obj);
}

struct Object far * far Obj_PtrTMem(struct Link far *link)
{
    if (link == 0 || link->index == 0)
        return 0;
    if (link->index < 0x100)
        return critdata + link->index;
    return (struct Object far *)(objdata + (link->index - 0x100));
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
    if (index < 0x100)
        return critdata + index;
    return (struct Object far *)(objdata + index - 0x100);
}

struct Object far * far Obj_Find(struct Link far *head, char recurse, int index)
{
    struct Object far *found;
    struct Object far *obj;

    if (head->index == 0)
        return 0;
    Obj_Find_Head = head;
    for (obj = Obj_PtrTMem(head); Obj_MemTPtr(obj) != index; obj = Obj_PtrTMem(&obj->next)) {
        if (recurse && !OBJ_ISQUANT(obj) && obj->link.index != 0) {
            if ((found = Obj_Find(&obj->link, 1, index)) != 0)
                return found;
        }
        if (obj->next.index == 0)
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

/* IDA AddtoEndOfTileMapMobiles; FM Towns active_critter_ is at the same place after
   IsMobElem_ and appends a byte at LastActiveMob the same way. */
void far active_critter(int index)
{
    *LastActiveMob++ = index;
}

/* IDA RemoveFromMobilesList; FM Towns free_critter_ follows active_critter_ and does
   the same search of ActiveMob..LastActiveMob, moving the last entry into the hole.
   The byte compare (cmp al,cl) with a word parameter is the cast. */
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

/* Whether an index is on a free list. No FM Towns counterpart (it goes straight from
   free_critter_ to Obj_InList_) and nothing calls it, so it keeps the IDA name. */
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

struct Object far * far Obj_InList(struct Link far **head, char recurse, int major, int minor, int index)
{
    struct Object far *obj;
    struct Object far *found;
    struct Link far *sub;
    struct Link far *list;

    for (obj = Obj_PtrTMem(*head); obj; obj = Obj_PtrTMem(&obj->next)) {
        if ((major == -1 || OBJ_MAJOR(obj) == major)
         && (minor == -1 || OBJ_MINOR(obj) == minor)
         && (index == -1 || OBJ_INDEX(obj) == index))
            return obj;
        if (recurse && !OBJ_ISQUANT(obj) && (sub = &obj->link)->index != 0) {
            list = sub;
            if ((found = Obj_InList(&list, recurse, major, minor, index)) != 0) {
                *head = list;
                return found;
            }
        }
    }
    return 0;
}

unsigned char far HasOrIsObj(struct Object far *obj, int id)
{
    struct Link far *list;

    if (OBJ_ITEM(obj) == id)
        return 1;
    if (!OBJ_ISQUANT(obj)) {
        list = &obj->link;
        if (Obj_InList(&list, 1, id >> 6, (id & 0x30) >> 4, id & 0xF))
            return 1;
    }
    return 0;
}

struct Object far * far Obj_FindInMap(int major, int minor, int index, int *x, int *y)
{
    struct Tile far *t;
    struct Link far *head;
    struct Object far *found = 0;

    if (*x >= 0x40) {
        *x = 0;
        (*y)++;
    }
    t = mapdata + (*y << 6) + *x;
    for (; *y < 0x40; (*y)++) {
        for (; *x < 0x40; (*x)++, t++) {
            head = &t->objects;
            if (head->index > 0) {
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
    struct Link far *head;

    head = &Map_GetAddr(x, y)->objects;
    return Obj_InList(&head, 0, major, minor, index);
}

int far check_weight(struct Link far *head, int min, int z, int adjust)
{
    struct Object far *obj;
    int quantity;

    if (min != -2)
        total_weight = 0;
    for (obj = Obj_PtrTMem(head); obj; obj = Obj_PtrTMem(&obj->next)) {
        if (z < 0 || OBJ_Z(obj) == z) {
            if (OBJ_ISQUANT(obj) && !(obj->link.index & 0x200))
                quantity = obj->link.index;
            else
                quantity = 1;
            total_weight = total_weight + ComObjData[OBJ_ITEM(obj)].mass * quantity;
            if (adjust != -1 && obj == ThePlayer)
                total_weight += adjust;
            else if (!OBJ_ISQUANT(obj) && &obj->link != 0)
                check_weight(&obj->link, -2, z, adjust);
            if (min >= 0 && total_weight > min)
                return 1;
        }
    }
    if (min >= 0)
        return total_weight > min;
    return total_weight;
}

unsigned char far flog_list(int x, int y, unsigned char *counts, struct Link far *head)
{
    struct Object far *obj;
    unsigned char bad = 0;
    int i;

    for (obj = Obj_PtrTMem(head); obj; obj = Obj_PtrTMem(&obj->next)) {
        i = Obj_MemTPtr(obj);
        if (++counts[i] > 1) {
            bad = 1;
            if (counts[i] > 10)
                return 1;
        }
        if (!OBJ_ISQUANT(obj) && obj->link.index > 0)
            bad = flog_list(x, y, counts, &obj->link);
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

    for (; obj; obj = Obj_PtrTMem(&obj->next)) {
        i = Obj_MemTPtr(obj);
        if (++counts[i] > 1) {
            bad = 1;
            if (counts[i] > 10)
                return 1;
        }
        if (!OBJ_ISQUANT(obj) && obj->link.index > 0)
            bad = count_list(Obj_PtrTMem(&obj->link), counts);
        if (IsMobElem(obj))
            mobcount++;
        else
            objcount++;
    }
    return bad;
}

/* ObjCrunch passes its argument on, but nothing here reads it. */
unsigned char far Obj_ListOkay(char how)
{
    unsigned far *p;
    int nstatic;
    int y;
    int nmobile;
    struct Tile far *t;
    struct Link far *head;
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
    for (y = 0; y < 0x40; y++) {
        for (x = 0; x < 0x40; x++, t++) {
            head = &t->objects;
            if (head->index > 0)
                bad = flog_list(x, y, counts, head) || bad;
        }
    }
    if (GrSq < 0)
        bad = count_list(ThePlayer, counts) || bad;
    if (GameInputMode == 1 || GameInputMode == 0 && CursorObjPtr != 0)
        bad = count_list(CursorObjPtr, counts) || bad;
    if (nmobile + mobcount != 0xFF)
        bad = 1;
    if (nstatic + objcount != 0x300)
        bad = 1;
    free(counts);
    return !bad;
}

unsigned char far ObjCrunch(char how)
{
    unsigned far *p;

    if (!Obj_ListOkay(how)) {
        errmsg("Cantcrunch", "badobjlist");
        return 0;
    }
    for (p = critptr; p >= critbot; p--)
        mem_set(Obj_IntTMem(*p), 0, 0x1B);
    for (p = objptr; p >= objbot; p--)
        mem_set(Obj_IntTMem(*p), 0, 8);
    return 1;
}
