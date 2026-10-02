/* object.h: Objects: the object lists and their storage, object classes and their data,
   combining, damage, spilling, using and looking at objects, animated objects and timers. */
#ifndef OBJECT_H
#define OBJECT_H

#include "uw2.h"

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

/* SEG029.C: the object lists */
extern struct Object far *critdata;
int far Obj_MemTPtr(struct Object far *obj);
void far active_critter(int index);
void far free_critter(int index);
void far Map_ObjFix(void);
unsigned char far Obj_Elem_Fate(int range, struct Object far *obj);
void far Obj_GarbageCollect(int range, int count);
void far Obj_Free(struct Object far *obj);
struct Object far * far Obj_FindInMapSquare(int major, int minor, int index, int x, int y);

/* SEG036.C: Map_GetAddr and CreateObj */
struct Tile far * far Map_GetAddr(int x, int y);

/* OVR134.C: object class data */
extern struct Object far *ActiveObj;
extern struct ComObj ComObjData[512];
int far init_objects(void);
char * far get_class_data(void);

/* OVR091.C: animated object class data */
char * far animobj_class_data(void);

/* OVR102.C: combining objects */
void far init_combinables(void);
int far ObjsBeCombinable(struct Object far *a, struct Object far *b);
struct Object far * far CombineObjs(int combo);
char far RemoveAfterCombine(struct Object far *obj, int combo);
char far make_stew(void);

/* OVR120.C: hack class data */
char * far hack_class_data(void);

/* OVR130.C: misc class data */
char * far misc_class_data(void);

/* SEG025.C: damage to objects */
int far debris_type(int item, char type);
char far damage_object(struct Object far *obj, struct Object far *who, int damage, int x, int y);
char far remove_lock(struct Object far *obj, char all);
unsigned char far check_res(struct Object far *obj, unsigned char damage, unsigned char type);

/* OVR163.C: spilling a container's contents, and critter loot */
char far drop_link_chain(struct Object far *cont, int owner);
void far drop_some_objects(struct Object far *critter);
void far generate_inventory(struct Object far *npc);

/* SEG040.C: using objects */
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

/* OVR138.C: using objects */
extern char door_type;
void far DumpTheBag(struct Object far *bag, char to_player);
void far UseRockHammerOn(struct Object far *obj, unsigned char how, char other);
void far UseWatch(void);
void far UseCrystal(int quality);
/* Declared before the rest of its file because TLINK numbers the overlay's stub entries in
   the order Turbo C lists the publics, which for names with the same hash key is the order
   they were first seen: the EXE's stub has UseBook before UseFood. */
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

/* OVR126.C: looking at things */
char far do_mods(struct Object far *obj, int lore, char *s);
char far do_of(struct Object far *obj, int lore, char *s);
void far RectLook(struct Object far *obj, int look);
void far CritterLook(struct Object far *obj, char *s);
void far SpecialLook(struct Object far *obj, int print);
void far LookAt(struct Object far *obj, int lore);
int far GetObjDesc(struct Object far *obj, int lore, char *s);

/* SEG044.C: animated objects and timers */
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

#endif
