/* object.h: Objects: the object lists and their storage, object classes and their data,
   combining, damage, spilling, using and looking at objects, animated objects and timers. */
#ifndef OBJECT_H
#define OBJECT_H

#include "uw2.h"
#include "items.h"

struct ComObj;
struct Object;
struct StaticObj;
struct Tile;

#include "map.h"

/* One object type's common properties: DATA\COMOBJ.DAT, 11 bytes an entry, loaded into
   ComObjData (UW-Formats documents the file). Names not in the format document are
   provisional, taken from the sources that use the fields. */
struct ComObj {
    unsigned height:8;                  /* 0x00 */
    unsigned radius:3;                  /* 0x01 */
    unsigned animated:1;
    unsigned mass:12;                   /* in tenths of a stone */
    unsigned b3_0:1;                    /* 0x03 */
    unsigned solid:1;
    unsigned c3_2:1;
    unsigned no_hit:1;
    unsigned b3_4:1;
    unsigned pickup:1;
    unsigned stack:2;
    unsigned value;                     /* 0x04, monetary value */
    unsigned touch:1;                   /* 0x06 */
    unsigned usable:1;
    unsigned qualclass:2;               /* quality class: this times 6 plus the quality
                                           indexes string block 5 */
    unsigned light:1;
    unsigned bounce:4;
    unsigned fate:4;                    /* 0x07, bits 1-4 */
    unsigned pickable:1;
    unsigned b7_6:1;
    unsigned can_own:1;                 /* the object can have an owner */
    unsigned char resist;               /* 0x08 */
    unsigned char render:2;             /* 0x09 */
    unsigned char tenacity:4;
    unsigned char b9_6:2;
    unsigned qualtype:4;                /* 0x0A, quality type: a group of 6 strings in block 4 */
    unsigned lookable:1;                /* a printable "look at" description */
    unsigned bA_5:3;
};

/* An object: 27 bytes for a mobile one (critters, missiles and anything else that moves,
   in critdata); a static object (objdata) is only the first 8 bytes, struct StaticObj.
   UW-Formats (4.2, the master object list) documents the fields; names it lacks are
   provisional, taken from the sources that use them. */
struct Object {
    unsigned id;                        /* 0x00: item 0-8 (major class 6-8, minor 4-5,
                                           index 0-3), flags 9-12, tenacious 13,
                                           is_quant 15 */
    unsigned pos;                       /* 0x02: z 0-6, heading 7-9, fine y 10-12,
                                           fine x 13-15 */
    union {
        unsigned word;
        struct { unsigned quality:6, next:10; } f;
        union Link link;
    } qn;                               /* 0x04: the quality and the next object in the
                                           list */
    union {
        unsigned word;
        struct { unsigned owner:6, link:10; } f;
        union Link link;
    } ol;                               /* 0x06: the owner, and the contents (or, if
                                           is_quant, the quantity) */
    unsigned char hp;                   /* 0x08, mobile objects only from here on */
    unsigned char heading;              /* 0x09 */
    unsigned char b0A;                  /* 0x0A */
    unsigned goal_word;                 /* 0x0B: goal 0-3, target 4-11; a missile's
                                           fine x */
    unsigned attitude_word;             /* 0x0D: level 0-3, talked to 13, attitude
                                           14-15; a missile's fine y */
    unsigned b0F;                       /* 0x0F; a missile's fine z */
    unsigned char b11;                  /* 0x11 */
    unsigned char last_hit;             /* 0x12 */
    unsigned char b13;                  /* 0x13 */
    unsigned char b14;                  /* 0x14 */
    unsigned char b15;                  /* 0x15 */
    unsigned home;                      /* 0x16: y 4-9, x 10-15 */
    unsigned char b18;                  /* 0x18, fine heading in bits 0-4 */
    unsigned char b19;                  /* 0x19 */
    unsigned char whoami;               /* 0x1A, the conversation to run */
};

/* A static object: the first 8 bytes of struct Object, all a static object has. */
struct StaticObj {
    unsigned id;
    unsigned pos;
    union {
        unsigned word;
        struct { unsigned quality:6, next:10; } f;
        union Link link;
    } qn;
    union {
        unsigned word;
        struct { unsigned owner:6, link:10; } f;
        union Link link;
    } ol;
};

/* One running animation (EFFECT.C's animlist): the animated object, the frames it has
   left (-1 for ever), and its tile. */
struct Anim {
    union Link link;
    int len;
    unsigned char x, y;
};

/* Per animation class (an object of class 7, by its low 4 bits): what to do each frame,
   and the run of frames it cycles through. */
struct AnimClass {
    unsigned flags;                     /* 1 cycle, 2 random, 4 door, 0x20 remove at end,
                                           0x80 finish the motion first */
    char start;
    unsigned char count;
};

/* The fields of an object's first four words (UW-Formats 4.2, "general object info").
   The id word: */
#define ID_ITEM         0x1FF           /* bits 0-8, the item id (items.h) */
#define ID_CLASS        0x1F0           /* bits 4-8, the major and minor class */
#define ID_MAJOR        0x1C0           /* bits 6-8, the major class (enum ObjMajor) */
#define ID_MINOR        0x30            /* bits 4-5, the minor class */
#define ID_INMAJOR      0x3F            /* bits 0-5, the item within its major class */
#define ID_INCLASS      0xF             /* bits 0-3, the item within its class */
#define ID_FLAGS        0x1E00          /* bits 9-12; for a door, its state */
#define ID_FLAG9        0x200           /* bit 9: a lock is locked (UW-Formats, 010f;
                                           UseKey, checkLock); a trigger may be set off
                                           by creatures (ovr166) */
#define ID_FLAG10       0x400           /* bit 10: a book plays a cutscene (UseBook);
                                           a trigger keeps its trap after use (ovr166) */
#define ID_FLAG11       0x800           /* bit 11: a spell object's charges are shown
                                           (ovr126); a trigger may be set off by the
                                           player (ovr166) */
#define ID_ENCHANT      0x1000          /* bit 12, the object is enchanted */
#define ID_DOORDIR      0x2000          /* bit 13: doordir in UW-Formats; chkTenacious
                                           keeps an object with it set from culling */
#define ID_INVIS        0x4000          /* bit 14, not drawn */
#define ID_ISQUANT      0x8000          /* bit 15: the link word is a quantity or a
                                           special property, not an object */
/* The position word: */
#define POS_Z           0x7F            /* bits 0-6, height in the tile */
#define POS_HEADING     0x380           /* bits 7-9, in eighths of a turn */
#define POS_YFINE       0x1C00          /* bits 10-12, y within the tile, 0-7 */
#define POS_XFINE       0xE000          /* bits 13-15, x within the tile, 0-7 */
/* An is_quant object's link field: below this a quantity, from it a special property
   (the link less 0x200: an enchantment, a string, ...). */
#define LINK_SPECIAL    0x200
/* A mobile object's home word: */
#define HOME_Y          0x3F0           /* bits 4-9 */
#define HOME_X          0xFC00          /* bits 10-15 */

/* Accessors for an object's fields. The getters are written mask then shift, as the
   original's macros were (docs/MATCHING.md, "Bitfields versus macros"). A setter rewrites the
   whole word or byte.
   Names of the mobile fields come from the code that uses them; UnderworldGodot's
   uwobject.cs (Hank Morgan's reading of the same bytes) agrees where noted, and names
   with only a byte and bit (OBJ_B19_4) are fields whose meaning is not known. */
/* match: a shift by 0 is kept, because Turbo C emits it (shr ax,0). Two
   fields need a second spelling, because files compile them differently:
   OBJ_INMAJOR_NOSHIFT leaves out the shift by 0 (one instruction fewer; ovr157, seg007),
   and SET_FINEX_UNSIGNED casts the new value to unsigned, which stops Turbo C merging the
   macro's "& 7" with the same mask in the argument (SET_FINEX(o, x & 7) is one "and", the
   _UNSIGNED form two; seg008, seg024, seg027, seg028, seg030). Other differences between
   the files' old copies (an unmasked value, a missing "<< 0") changed no bytes, because
   those files pass constants or values already masked. */
/* The id word */
#define OBJ_ITEM(o)         ((o)->id & ID_ITEM)
#define OBJ_MAJOR(o)        (((o)->id & ID_MAJOR) >> 6)
#define OBJ_MINOR(o)        (((o)->id & ID_MINOR) >> 4)
#define OBJ_CLASS(o)        (((o)->id & ID_CLASS) >> 4)
#define OBJ_CLASSID(o)      ((o)->id & ID_CLASS)        /* the class's first item id */
#define OBJ_INMAJOR(o)      (((o)->id & ID_INMAJOR) >> 0)
#define OBJ_INMAJOR_NOSHIFT(o) ((o)->id & ID_INMAJOR)
#define OBJ_INCLASS(o)      ((o)->id & ID_INCLASS)
#define OBJ_FLAGS(o)        (((o)->id & ID_FLAGS) >> 9) /* for a door, its state */
#define OBJ_DOORDIR(o)      (((o)->id & ID_DOORDIR) >> 13)
#define OBJ_INVIS(o)        (((o)->id & ID_INVIS) >> 14)
#define OBJ_ISQUANT(o)      (((o)->id & ID_ISQUANT) >> 15)
#define ITEM_CLASS(item)    (((item) & ID_CLASS) >> 4)  /* of an item id, not an object */
#define SET_ITEM(o, v)      ((o)->id = (o)->id & 0xFE00 | (v) & ID_ITEM)
#define SET_MAJOR(o, v)     ((o)->id = (o)->id & 0xFE3F | ((v) & 7) << 6)
#define SET_MINOR(o, v)     ((o)->id = (o)->id & 0xFFCF | ((v) & 3) << 4)
#define SET_INCLASS(o, v)   ((o)->id = (o)->id & 0xFFF0 | (v) & 0xF)
#define SET_FLAGS(o, v)     ((o)->id = (o)->id & 0xE1FF | ((v) & 0xF) << 9)
#define SET_FLAG9(o, v)     ((o)->id = (o)->id & 0xFDFF | ((v) & 1) << 9)
#define SET_FLAG10(o, v)    ((o)->id = (o)->id & 0xFBFF | ((v) & 1) << 10)
#define SET_FLAG11(o, v)    ((o)->id = (o)->id & 0xF7FF | ((v) & 1) << 11)
#define SET_ENCHANTED(o, v) ((o)->id = (o)->id & 0xEFFF | ((v) & 1) << 12)
#define SET_DOORDIR(o, v)   ((o)->id = (o)->id & 0xDFFF | ((v) & 1) << 13)
#define SET_INVIS(o, v)     ((o)->id = (o)->id & 0xBFFF | ((v) & 1) << 14)
#define SET_ISQUANT(o, v)   ((o)->id = (o)->id & 0x7FFF | ((v) & 1) << 15)
/* The position word */
#define OBJ_Z(o)            ((o)->pos & POS_Z)
#define OBJ_HEADING(o)      (((o)->pos & POS_HEADING) >> 7)
#define OBJ_FINEY(o)        (((o)->pos & POS_YFINE) >> 10)
#define OBJ_FINEX(o)        (((o)->pos & POS_XFINE) >> 13)
#define SET_Z(o, v)         ((o)->pos = (o)->pos & 0xFF80 | (v) & POS_Z)
#define SET_HEADING(o, v)   ((o)->pos = (o)->pos & 0xFC7F | ((v) & 7) << 7)
#define SET_FINEY(o, v)     ((o)->pos = (o)->pos & 0xE3FF | ((v) & 7) << 10)
#define SET_FINEX(o, v)     ((o)->pos = (o)->pos & 0x1FFF | ((v) & 7) << 13)
#define SET_FINEX_UNSIGNED(o, v) ((o)->pos = (o)->pos & 0x1FFF | ((unsigned)(v) & 7) << 13)
/* The quality and next word, and the owner and link word */
#define OBJ_QUALITY(o)      ((o)->qn.f.quality)
#define OBJ_OWNER(o)        ((o)->ol.f.owner)
#define OBJ_LINK(o)         ((o)->ol.f.link)
#define SET_ENCHANT(o, v)   ((o)->ol.f.link = (o)->ol.f.link & 0x1F0 | (v) & 0xF | LINK_SPECIAL)
/* A mobile object's byte 0x0A: bits 0-3 a frame counter (NextFrame in uwobject.cs), 4-6
   the terrain it stands on (TileState), 7 set for a critter that keeps to itself */
#define OBJ_BIN(o)          ((o)->b0A & 0xF)
#define OBJ_TERRAIN(o)      (((o)->b0A & 0x70) >> 4)
#define OBJ_LONER(o)        (((o)->b0A & 0x80) >> 7)
#define SET_BIN(o, v)       ((o)->b0A = (o)->b0A & 0xF0 | ((v) & 0xF) << 0)
#define SET_TERRAIN(o, v)   ((o)->b0A = (o)->b0A & 0x8F | ((v) & 7) << 4)
#define SET_LONER(o, v)     ((o)->b0A = (o)->b0A & 0x7F | ((v) & 1) << 7)
/* The goal word (0x0B): the goal, its target, and an animation frame */
#define OBJ_GOAL(o)         (((o)->goal_word & 0xF) >> 0)
#define OBJ_GTARG(o)        (((o)->goal_word & 0xFF0) >> 4)
#define OBJ_FRAME(o)        (((o)->goal_word & 0xF000) >> 12)
#define SET_GOAL(o, v)      ((o)->goal_word = (o)->goal_word & 0xFFF0 | ((v) & 0xF) << 0)
#define SET_GTARG(o, v)     ((o)->goal_word = (o)->goal_word & 0xF00F | ((v) & 0xFF) << 4)
#define SET_FRAME(o, v)     ((o)->goal_word = (o)->goal_word & 0xFFF | ((v) & 0xF) << 12)
/* The attitude word (0x0D): bits 0-3 the goal saved while another runs (npc_level in
   UW-Formats), 4-7 a target height (TargetZHeight, provisional), 8 a summoned, temporary critter (SpawnedCritter), 9 no healing
   (StopHPRegen), 10 powerful (IsPowerful; ovr157 reads it as undead), 12 its inventory
   has been generated (LootSpawnedFlag), 13 talked to, 14-15 the attitude */
#define OBJ_OLDGOAL(o)      (((o)->attitude_word & 0xF) >> 0)
#define OBJ_TARGETZ(o)      (((o)->attitude_word & 0xF0) >> 4)
#define OBJ_TEMP(o)         (((o)->attitude_word & 0x100) >> 8)
#define OBJ_NOHEAL(o)       (((o)->attitude_word & 0x200) >> 9)
#define OBJ_POWERFUL(o)     (((o)->attitude_word & 0x400) >> 10)
#define OBJ_B0D_11(o)       (((o)->attitude_word & 0x800) >> 11)
#define OBJ_HAS_INV(o)      (((o)->attitude_word & 0x1000) >> 12)
#define OBJ_TALKEDTO(o)     (((o)->attitude_word & 0x2000) >> 13)
#define OBJ_ATTITUDE(o)     (((o)->attitude_word & 0xC000) >> 14)
#define SET_OLDGOAL(o, v)   ((o)->attitude_word = (o)->attitude_word & 0xFFF0 | ((v) & 0xF) << 0)
#define SET_TARGETZ(o, v)   ((o)->attitude_word = (o)->attitude_word & 0xFF0F | ((v) & 0xF) << 4)
#define SET_TEMP(o, v)      ((o)->attitude_word = (o)->attitude_word & 0xFEFF | ((v) & 1) << 8)
#define SET_NOHEAL(o, v)    ((o)->attitude_word = (o)->attitude_word & 0xFDFF | ((v) & 1) << 9)
#define SET_POWERFUL(o, v)  ((o)->attitude_word = (o)->attitude_word & 0xFBFF | ((v) & 1) << 10)
#define SET_B0D_11(o, v)    ((o)->attitude_word = (o)->attitude_word & 0xF7FF | ((v) & 1) << 11)
#define SET_HAS_INV(o, v)   ((o)->attitude_word = (o)->attitude_word & 0xEFFF | ((v) & 1) << 12)
#define SET_TALKEDTO(o, v)  ((o)->attitude_word = (o)->attitude_word & 0xDFFF | ((v) & 1) << 13)
#define SET_ATTITUDE(o, v)  ((o)->attitude_word = (o)->attitude_word & 0x3FFF | ((v) & 3) << 14)
/* The word at 0x0F: a destination tile (TargetTileX/Y) and an attack frame
   (SwingChargeIndex) */
#define OBJ_DESTX(o)        (((o)->b0F & 0x3F) >> 0)
#define OBJ_DESTY(o)        (((o)->b0F & 0xFC0) >> 6)
#define OBJ_ATKFRAME(o)     (((o)->b0F & 0xF000) >> 12)
#define SET_ATKFRAME(o, v)  ((o)->b0F = (o)->b0F & 0xFFF | ((v) & 0xF) << 12)
#define SET_DESTX(o, v)     ((o)->b0F = (o)->b0F & 0xFFC0 | ((v) & 0x3F) << 0)
#define SET_DESTY(o, v)     ((o)->b0F = (o)->b0F & 0xF03F | ((v) & 0x3F) << 6)
/* Byte 0x11, damage taken (AccumulatedDamage) */
#define OBJ_DAMAGE(o)       (((o)->b11 & 0xFF) >> 0)
#define SET_DAMAGE(o, v)    ((o)->b11 = (o)->b11 & 0 | ((v) & 0xFF) << 0)
/* Byte 0x13: speed, and bit 7 gravity */
#define OBJ_SPEED(o)        ((o)->b13 & 0x7F)
#define SET_SPEED(o, v)     ((o)->b13 = (o)->b13 & 0x80 | ((v) & 0x7F) << 0)
#define SET_GRAVITY(o, v)   ((o)->b13 = (o)->b13 & 0x7F | ((v) & 1) << 7)
/* Byte 0x14: a rate and a pitch (Projectile_Speed and Projectile_Pitch) */
#define OBJ_RATE(o)         ((o)->b14 & 7)
#define OBJ_PITCH(o)        (((o)->b14 & 0xF8) >> 3)
#define SET_RATE(o, v)      ((o)->b14 = (o)->b14 & 0xF8 | (v))
#define SET_PITCH(o, v)     ((o)->b14 = (o)->b14 & 7 | ((v) & 0x1F) << 3)
/* Byte 0x15: the animation sequence (npc_animation), and two flags */
#define OBJ_SEQ(o)          ((o)->b15 & 0x3F)
#define OBJ_B15_6(o)        (((o)->b15 & 0x40) >> 6)
#define OBJ_B15_7(o)        (((o)->b15 & 0x80) >> 7)
#define SET_SEQ(o, v)       ((o)->b15 = (o)->b15 & 0xC0 | ((v) & 0x3F) << 0)
#define SET_B15_6(o, v)     ((o)->b15 = (o)->b15 & 0xBF | (v) << 6)
#define SET_B15_7(o, v)     ((o)->b15 = (o)->b15 & 0x7F | (v) << 7)
/* The home word (0x16): bits 0-3 a path index (PathFindIndex), then the home tile */
#define OBJ_PATH(o)         ((o)->home & 0xF)
#define OBJ_HOMEY(o)        (((o)->home & HOME_Y) >> 4)
#define OBJ_HOMEX(o)        (((o)->home & HOME_X) >> 10)
#define SET_PATH(o, v)      ((o)->home = (o)->home & 0xFFF0 | (v) & 0xF)
#define SET_HOMEY(o, v)     ((o)->home = (o)->home & 0xFC0F | ((v) & 0x3F) << 4)
#define SET_HOMEX(o, v)     ((o)->home = (o)->home & 0x3FF | ((v) & 0x3F) << 10)
/* Byte 0x18: the fine heading, and three flags */
#define OBJ_FINEHEAD(o)     ((o)->b18 & 0x1F)
#define OBJ_B18_6(o)        (((o)->b18 & 0x40) >> 6)
#define OBJ_B18_7(o)        (((o)->b18 & 0x80) >> 7)
#define SET_FINEHEAD(o, v)  ((o)->b18 = (o)->b18 & 0xE0 | ((v) & 0x1F) << 0)
#define SET_B18_5(o, v)     ((o)->b18 = (o)->b18 & 0xDF | (v) << 5)
#define SET_B18_6(o, v)     ((o)->b18 = (o)->b18 & 0xBF | ((v) & 1) << 6)
#define SET_B18_7(o, v)     ((o)->b18 = (o)->b18 & 0x7F | ((v) & 1) << 7)
/* Byte 0x19: flags, the spell being cast (npc_spellindex), bit 6 on the player's side
   (IsAlly) and bit 7 fed */
#define OBJ_B19_0(o)        (((o)->b19 & 1) >> 0)
#define OBJ_B19_1(o)        (((o)->b19 & 2) >> 1)
#define OBJ_CAST(o)         (((o)->b19 & 0xC) >> 2)
#define OBJ_B19_4(o)        (((o)->b19 & 0x10) >> 4)
#define OBJ_B19_5(o)        (((o)->b19 & 0x20) >> 5)
#define OBJ_ALLY(o)         (((o)->b19 & 0x40) >> 6)
#define OBJ_FED(o)          (((o)->b19 & 0x80) >> 7)
#define SET_B19_0(o, v)     ((o)->b19 = (o)->b19 & 0xFE | ((v) & 1) << 0)
#define SET_B19_1(o, v)     ((o)->b19 = (o)->b19 & 0xFD | ((v) & 1) << 1)
#define SET_CAST(o, v)      ((o)->b19 = (o)->b19 & 0xF3 | ((v) & 3) << 2)
#define SET_B19_4(o, v)     ((o)->b19 = (o)->b19 & 0xEF | (v) << 4)
#define SET_B19_5(o, v)     ((o)->b19 = (o)->b19 & 0xDF | (v) << 5)
#define SET_ALLY(o, v)      ((o)->b19 = (o)->b19 & 0xBF | ((v) & 1) << 6)
#define SET_FED(o, v)       ((o)->b19 = (o)->b19 & 0x7F | ((v) & 1) << 7)

/* The master object list (UW-Formats 4.2): 1024 objects, the first 256 mobile (27 bytes,
   struct Object) and the rest static (8 bytes, struct StaticObj); an index below
   NUM_MOBILE is a mobile object. */
#define NUM_OBJECTS     0x400
#define NUM_MOBILE      0x100
#define NUM_STATIC      0x300
#define MOBILE_SIZE     0x1B
#define STATIC_SIZE     8

/* OBJECTS.C: the object lists */
extern struct Object far *critdata;
extern union Link far *Obj_Find_Head;
int far Obj_MemTPtr(struct Object far *obj);
void far active_critter(int index);
void far free_critter(int index);
void far Map_ObjFix(void);
unsigned char far Obj_Elem_Fate(int range, struct Object far *obj);
void far Obj_GarbageCollect(int range, int count);
void far Obj_Free(struct Object far *obj);
struct Object far * far Obj_FindInMapSquare(int major, int minor, int index, int x, int y);
struct Object far * far Obj_PtrTMem(union Link far *link);
struct Object far * far Obj_IntTMem(int index);
void far Obj_FreeChain(union Link far *head);
void far Obj_FreeLinkChain(union Link far *head, struct Object far *obj);
unsigned char far Obj_Check(struct Object far *obj, unsigned char (far *fn)(struct Object far *obj));
struct Object far * far Obj_Alloc(char mobile);
void far Obj_Add(union Link far *head, struct Object far *obj);
void far Obj_AddEnd(union Link far *head, struct Object far *obj);
unsigned char far Obj_Rem(union Link far *head, struct Object far *obj);
struct Object far * far Obj_Punt(union Link far *head, struct Object far *obj, char force);
struct Object far * far Obj_Find(union Link far *head, char recurse, int index);
unsigned char far IsMobElem(struct Object far *obj);
struct Object far * far Obj_InList(union Link far **head, char recurse, int major, int minor, int index);
unsigned char far HasOrIsObj(struct Object far *obj, int id);
struct Object far * far Obj_FindInMap(int major, int minor, int index, int *x, int *y);
int far check_weight(union Link far *head, int min, int z, int adjust);
unsigned char far ObjCrunch(char how);
extern struct StaticObj far *objdata;
extern unsigned char far *LastActiveMob;
extern unsigned char far *ActiveMob;
extern unsigned far *objtop;  /* the free static list; objtop and crittop tie objbot and critbot */
extern unsigned far *objbot;
extern unsigned far *objptr;
extern unsigned far *crittop;  /* the free mobile list */
extern unsigned far *critbot;
extern unsigned far *critptr;

/* MAPADDR.C: Map_GetAddr and CreateObj */
struct Tile far * far Map_GetAddr(int x, int y);
struct Object far * far CreateObj(int item, char mobile);

/* OBJCLASS.C: object class data */
extern struct Object far *ActiveObj;
extern struct ComObj ComObjData[512];
int far init_objects(void);
char * far get_class_data(void);

/* ANIMOBJ.C: animated object class data */
char * far animobj_class_data(void);
void far animobj_load(FILE *fd);

/* COMBINE.C: combining objects */
void far init_combinables(void);
int far ObjsBeCombinable(struct Object far *a, struct Object far *b);
struct Object far * far CombineObjs(int combo);
char far RemoveAfterCombine(struct Object far *obj, int combo);
char far make_stew(void);

/* HACK.C: hack class data */
char * far hack_class_data(void);
/* A missile weapon, 3 bytes: the ranged weapons table of DATA\OBJECTS.DAT, in Missile[]. */
struct MissileInfo {
    unsigned char damage;               /* 0x00 */
    unsigned char type;                 /* 0x01, the missile type */
    signed char ammo;                   /* 0x02, the ammunition it fires */
};

/* A melee weapon, 8 bytes: the melee weapons table of DATA\OBJECTS.DAT. */
struct Weapon {
    unsigned char damage[3];            /* by swing kind: slash, bash, stab */
    unsigned char min_charge;           /* 0x03 */
    unsigned char speed;                /* 0x04 */
    unsigned char max_charge;           /* 0x05 */
    unsigned char skill;                /* 0x06, the skill it uses */
    unsigned char durability;           /* 0x07 */
};

/* One armour or wearable's properties, 4 bytes, 32 of them from OBJECTS.DAT (UW-Formats,
   "Armour and wearables table"), in ovr120's Armor. */
struct Armour {
    unsigned char protection;           /* 0x00 */
    unsigned char durability;           /* 0x01 */
    unsigned char b2;                   /* 0x02 */
    unsigned char category;             /* 0x03: 0 shield, 1 body armour, 3 leggings,
                                           4 gloves, 5 boots, 8 hat, 9 ring */
};

extern struct MissileInfo Missile[16];
extern struct Weapon Weapons[16];
extern struct Armour Armor[32];
void far hack_init(FILE *fd);

/* MISC.C: misc class data */
char * far misc_class_data(void);
/* One container type, 3 bytes: OBJECTS.DAT's container table (Guide, "Containers table"). */
struct Container {
    unsigned char capacity;             /* 0x00, 0 for no limit */
    int mask;                           /* 0x01, what it accepts: an item id, 0x200.. a kind, or -1 */
};
/* The kinds a container's mask can name, as INVPANEL.C's ItemFitsSlot tests them. */
#define CONT_RUNES      0x200           /* runestones (the rune bag) */
#define CONT_MISSILES   0x201           /* sling stones, bolts, arrows and wands */
#define CONT_SCROLLS    0x202           /* scrolls and maps */
#define CONT_FOOD       0x203           /* food and reagents, not drinks */
#define CONT_KEYS       0x204           /* keys, or a container of keys */

/* One entry per light source type (item class, lit types 4 to 7). */
struct Light {
    unsigned char duration;             /* burn rate, 0 for an unlit light */
    unsigned char pad;
};

extern struct Container Containers[16];
extern struct Light Lights[16];
extern char Food[0x10];
void far misc_init(FILE *fd);

/* DAMAGE.C: damage to objects */
int far debris_type(int item, char type);
char far damage_object(struct Object far *obj, struct Object far *who, int damage, int x, int y);
char far remove_lock(struct Object far *obj, char all);
unsigned char far check_res(struct Object far *obj, unsigned char damage, unsigned char type);
char far damage_item(struct Object far *obj, struct Object far *who, int x, int y, unsigned char damage, unsigned char type);

/* TREASURE.C: spilling a container's contents, and critter loot */
char far drop_link_chain(struct Object far *cont, int owner);
void far drop_some_objects(struct Object far *critter);
void far generate_inventory(struct Object far *npc);

/* OBJUSE.C: using objects */
void far UseKey(struct Object far *obj, unsigned char how);
void far UseWand(struct Object far *wand, unsigned char how);
char far UseReag(struct Object far *who, struct Object far *obj, char how);
char far checkSpell(int x, int y, struct Object far *who, struct Object far *obj, char how);
void far checkTrap(struct Object far *who, struct Object far *obj, int how, int x, int y);
struct Object far * far place_new(struct Object far *obj, int item);
void far UseThing(struct Object far *obj, void (far *fn)());
int far checkLock(struct Object far *who, struct Object far *door, int key);
void far BlastFunction(void);
void far remove_spell(struct Object far *obj);
extern long nextSpellTime;
extern unsigned char always_decode;
struct Object far * far UseObj(struct Object far *who, struct Object far *obj, unsigned char how);
int far using_punt(struct Object far *obj, char inv, char how);
char far flip_switch(struct Object far *obj, int state);
char far decode_obj_spell(struct Object far *obj, int *major, int *effect, unsigned char *flag);

/* USEITEMS.C: using objects */
extern char door_type;
void far DumpTheBag(struct Object far *bag, char to_player);
void far UseRockHammerOn(struct Object far *obj, unsigned char how, char other);
void far UseWatch(void);
void far UseCrystal(int quality);
/* match: declared before the rest of its file because TLINK numbers the overlay's stub
   entries in the order Turbo C lists the publics, which for names with the same hash key
   is the order they were first seen: the EXE's stub has UseBook before UseFood. */
void far UseBook(struct Object far *obj, unsigned char how);
int far UseFood(struct Object far *who, struct Object far *food, unsigned char how);
void far UseLockpickOn(struct Object far *obj, unsigned char how);
void far UseCont(struct Object far *who, struct Object far *obj, char how);
void far UseLight(struct Object far *obj, unsigned char how);
void far UseUnique(struct Object far *who, struct Object far *obj, unsigned char how);
void far changeDoor(struct Object far *door);
void far OpenDoor(struct Object far *who, struct Object far *door);
void far CloseDoor(struct Object far *who, struct Object far *door);
void far ToggleDoor(struct Object far *who, struct Object far *door);
void far UseRune(struct Object far *who, struct Object far *rune);
void far UseRect(struct Object far *who, struct Object far *obj);
void far UseMagic(struct Object far *who, struct Object far *obj, char how);
void far UseUtil(struct Object far *obj, char how);
void far UseOilOn(struct Object far *obj, unsigned char how, unsigned char other);  /* ties UseKeyOn */
void far UseKeyOn(struct Object far *obj, unsigned char how);

/* LOOK.C: looking at things */
char far do_mods(struct Object far *obj, int lore, char *s);
char far do_of(struct Object far *obj, int lore, char *s);
void far RectLook(struct Object far *obj, int look);
void far CritterLook(struct Object far *obj, char *s);
void far SpecialLook(struct Object far *obj, int print);
void far LookAt(struct Object far *obj, int lore);
int far GetObjDesc(struct Object far *obj, int lore, char *s);

/* EFFECT.C: animated objects and timers */
extern unsigned char DoAnimO;
void far do_animobj(int n, int frames);
unsigned char far check_door(int n, int frames);
void far rem_anim_from_list(int index);
void far toast_animobj(int n, int frames);
void far Change_AnimPtr(struct Object far *to, struct Object far *from);
void far update_animobj(int frames);
void far fireball_effect(struct Object far *src, int x, int y);
unsigned char far mts_doanim(struct Object far *obj, int x, int y, char who);
int far get_animlen(struct Object far *obj);
void far set_animlen(struct Object far *obj, int len);
unsigned char far rem_timer_obj(int index);
extern char animcount;
extern struct AnimClass animclassd[16];
extern int timerlist[0x40];
extern char timercount;
extern struct Anim animlist[0x40];
int far add_animobj(int index, int len, unsigned char a, unsigned char x, unsigned char y);
unsigned char far put_effect(struct Object far *who, int cls, int len, int frame, int z, int x, int y);
int far find_anim(struct Object far *obj);

#define OBJECT_H_COMPLETE
#include "level.h"

/* RECT.C */
char * far rect_class_data(void);

/* SPEC.C */
char * far spec_class_data(void);

/* STUFF.C */
char * far stuff_class_data(void);
#endif
