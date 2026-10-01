/* target: seg040_34E7 */
/* opts: -mm -1 -G -O -d */
/* Using objects: the dispatch on an object's class, removing a used object, putting a new
   object in the hand, keys, wands, potions, switches, locks, spells and traps carried by
   objects, and their charges. The whole of DOS resident segment seg040_34E7, in original
   order. Function and global names are the originals from the FM Towns symbol table; the
   source file's own name is not known. */

#include <string.h>
#include <stdlib.h>

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0x369];
    unsigned long game_clock;           /* 0x369 */
};

/* A mobile object. The first 8 bytes are shared with static objects. */
struct Object {
    unsigned id;                        /* item 0-8 (major 6-8, minor 4-5, type 0-3), flags 9-12, is_quant 15 */
    unsigned pos;                       /* z 0-6 */
    union {
        unsigned word;                  /* the next object in this list */
        struct { unsigned quality:6, next:10; } f;
    } qn;
    union {
        unsigned word;                  /* the head of the contents list, or the quantity */
        struct { unsigned owner:6, link:10; } f;
    } ol;
};

#define OBJ_ID(o)       ((o)->id & 0x1FF)
#define OBJ_INDEX(o)    (((o)->id & 0x3F) >> 0)
#define OBJ_TYPE(o)     ((o)->id & 0xF)
#define OBJ_MAJOR(o)    (((o)->id & 0x1C0) >> 6)
#define OBJ_CLASS(o)    (((o)->id & 0x1F0) >> 4)
#define OBJ_MINOR(o)    (((o)->id & 0x30) >> 4)
#define OBJ_FLAGS(o)    (((o)->id & 0x1E00) >> 9)
#define OBJ_ISQUANT(o)  (((o)->id & 0x8000) >> 15)
#define OBJ_Z(o)        ((o)->pos & 0x7F)

struct Tile {
    char pad0[2];
    unsigned objects;                   /* 0x02, head of the tile's object list */
};

struct Inplist {
    char pad0[8];
    int field8;
};

extern struct Player near *player;
extern struct Inplist near *inplist;
extern struct Object far *ThePlayer;
extern struct Object far *CursorObjPtr;
extern struct Object far *ObjectActing;
extern void (far *ObjectActor)();
extern char ObjectActorArg;
extern int GameInputMode;
extern int MapObj_X, MapObj_Y;
extern unsigned far *Obj_Find_Head;

/* The next time the player may cast from an object, and a flag that makes
   decode_obj_spell always identify the spell. */
long nextSpellTime = 0;
unsigned char always_decode = 0;

void far missile_newhit(struct Object far *proj, struct Object far *hit);
void far UseCont(struct Object far *who, struct Object far *obj, char how);
void far UseLight(struct Object far *obj, char how);
void far UseWatch(void);
void far UseCrystal(int quality);
int far UseFood(struct Object far *who, struct Object far *food, char how);
void far UseUtil(struct Object far *obj, char how);
void far UseUnique(struct Object far *who, struct Object far *obj, char how);
void far UseMagic(struct Object far *who, struct Object far *obj, char how);
void far UseBook(struct Object far *obj, char how);
void far UseRect(struct Object far *who, struct Object far *obj);
void far UseRune(struct Object far *who, struct Object far *rune);
void far CloseDoor(struct Object far *who, struct Object far *door);
void far OpenDoor(struct Object far *who, struct Object far *door);
void far UseLockpickOn(struct Object far *obj, unsigned char how);
void far UseKeyOn(struct Object far *obj, unsigned char how);
struct Object far * far Obj_PtrTMem(unsigned far *link);
int far Obj_MemTPtr(struct Object far *obj);
void far Obj_FreeLinkChain(unsigned far *head, struct Object far *obj);
void far InvRemoveOneObject(struct Object far *obj);
struct Object far * far Obj_Punt(unsigned far *head, struct Object far *obj, char how);
struct Tile far * far Map_GetAddr(int x, int y);
struct Object far * far Obj_Find(unsigned far *head, int a, int index);
void far Obj_FreeChain(unsigned far *head);
struct Object far * far Obj_InList(unsigned far **head, int a, int major, int minor, int idx);
char far Obj_Rem(unsigned far *head, struct Object far *obj);
void far Obj_Free(struct Object far *obj);
struct Object far * far CreateObj(int item, char mobile);
void far editchng(int bits);
void far force_mouse_cursor(int id);
void far unforce_mouse_cursor(int n);
void far mouse_release(int n);
void far get_name(char far *buf, struct Object far *obj, int article, char plural);
void far scroll_print(char far *s);
void far game_sprint(int id);
int far FindSlot(struct Object far *obj);
void far RedisplayInvSlot(int slot);
void far play_effect(char type, int x, int y, int a);
char far play_effect_here(int fx, int vol, int c);
int far skill_check(int value, int target);
void far inanimate_spell(int x, int y, struct Object far *src, struct Object far *who,
                         int major, int effect);
void far UseTrigger(struct Object far *who, struct Object far *obj, struct Object far *trigger, int how);
void far SetOffTrap(struct Object far *who, struct Object far *obj, struct Object far *trap, int x, int y);
void far delete_trap(unsigned far *head, struct Object far *trap);
void far release_missile(struct Object far *obj, char arg);

/* Later in this file. */
void far UseKey(struct Object far *obj, unsigned char how);
void far UseWand(struct Object far *wand, unsigned char how);
char far UseReag(struct Object far *who, struct Object far *obj, char how);
char far checkSpell(int x, int y, struct Object far *who, struct Object far *obj, char how);
void far checkTrap(struct Object far *who, struct Object far *obj, int how, int x, int y);
char far decode_obj_spell(struct Object far *obj, int *major, int *effect, unsigned char *flag);
int far useNSpellCharges(struct Object far *obj, char n);

struct Object far * far UseObj(struct Object far *who, struct Object far *obj, unsigned char how)
{
    int minor;
    char trap;
    struct Object far *fount;

    minor = OBJ_MINOR(obj);
    trap = 1;
    if (inplist->field8 == 4 && (OBJ_MAJOR(obj) != 2 || OBJ_MINOR(obj) != 0))
        return obj;
    switch (OBJ_MAJOR(obj)) {
    case 0:
        if (minor == 1 && !how && who != 0)
            missile_newhit(obj, who);
        break;
    case 2:
        switch (minor) {
        case 0:
            UseCont(who, obj, how);
            break;
        case 1:
            if (OBJ_TYPE(obj) < 8)
                UseLight(obj, how);
            else if (OBJ_TYPE(obj) >= 8) {
                UseWand(obj, how);
                trap = 0;
            }
            break;
        case 2:
            if (!how)
                break;
            if (OBJ_TYPE(obj) == 0xF)
                UseWatch();
            else if (OBJ_ID(obj) == 0xA1)
                UseCrystal(obj->qn.f.quality);
            break;
        case 3:
            UseFood(who, obj, how);
            trap = 0;
            break;
        }
        break;
    case 3:
        switch (minor) {
        case 0:
        case 1:
            UseUtil(obj, how);
            break;
        case 2:
            trap = UseReag(who, obj, how);
            break;
        }
        break;
    case 4:
        switch (minor) {
        case 0:
            UseKey(obj, how);
            break;
        case 1:
            UseUnique(who, obj, how);
            break;
        case 2:
            UseMagic(who, obj, how);
            break;
        case 3:
            UseBook(obj, how);
            trap = 0;
            break;
        }
        break;
    case 5:
        UseRect(who, obj);
        break;
    case 6:
        if (OBJ_ID(obj) == 0x19E || OBJ_ID(obj) == 0x19F)
            UseRune(who, obj);
        break;
    case 7:
        switch (OBJ_TYPE(obj)) {
        case 0xF:
            if ((obj->ol.f.owner & 0xF) < 8)
                CloseDoor(who, obj);
            else
                OpenDoor(who, obj);
            break;
        case 9:
            if (obj->qn.f.next != 0) {
                fount = Obj_PtrTMem(&obj->qn.word);
                if (OBJ_ID(fount) == 0x12E)
                    UseMagic(who, fount, how);
            }
            break;
        }
        break;
    }
    if (trap) {
        checkTrap(who, obj, 4, MapObj_X, MapObj_Y);
        checkSpell(MapObj_X, MapObj_Y, who, obj, how);
    }
    return obj;
}

int far using_punt(struct Object far *obj, char inv, char how)
{
    struct Tile far *tile;
    union {
        unsigned word;
        struct { unsigned owner:6, link:10; } f;
    } head;

    if (inv) {
        if (!OBJ_ISQUANT(obj) && obj->ol.f.link > 0)
            Obj_FreeLinkChain(&obj->ol.word, Obj_PtrTMem(&obj->ol.word));
        InvRemoveOneObject(obj);
        obj->ol.f.link = 0;
        obj = Obj_Punt(0L, obj, how);
    } else {
        tile = Map_GetAddr(MapObj_X, MapObj_Y);
        if (Obj_Find(&tile->objects, 1, Obj_MemTPtr(obj)) != 0) {
            obj = Obj_Punt(Obj_Find_Head, obj, how);
            editchng(2);
        } else {
            head.f.link = Obj_MemTPtr(obj);
            Obj_FreeChain(&head.word);
            obj = 0;
        }
    }
    return obj == 0;
}

struct Object far * far place_new(struct Object far *obj, int item)
{
    if (CursorObjPtr != 0)
        return 0;
    if (obj != 0)
        item = OBJ_ID(obj);
    else
        obj = CreateObj(item, 0);
    GameInputMode = 1;
    CursorObjPtr = obj;
    force_mouse_cursor(item);
    return obj;
}

void far UseThing(struct Object far *obj, void (far *fn)());

void far UseKey(struct Object far *obj, unsigned char how)
{
    if (!how)
        return;
    if (OBJ_ID(obj) == 0x101)
        UseThing(obj, UseLockpickOn);
    else if (OBJ_ID(obj) < 0x10F)
        UseThing(obj, UseKeyOn);
}

void far UseThing(struct Object far *obj, void (far *fn)())
{
    char buf[40];

    strcpy(buf, "Use ");
    get_name(buf + strlen(buf), obj, 0, 0);
    strcat(buf, " on what?\n");
    scroll_print(buf);
    force_mouse_cursor(OBJ_ID(obj));
    CursorObjPtr = obj;
    GameInputMode = 2;
    ObjectActing = obj;
    ObjectActor = fn;
}

void far UseWand(struct Object far *wand, unsigned char how)
{
    unsigned far *link;
    struct Object far *spell;

    if (!how)
        return;
    if (OBJ_TYPE(wand) < 0xC || OBJ_TYPE(wand) > 0xF) {
        checkTrap(ThePlayer, wand, 4, MapObj_X, MapObj_Y);
        checkSpell(MapObj_X, MapObj_Y, ThePlayer, wand, how);
        if (!OBJ_ISQUANT(wand)) {
            link = &wand->ol.word;
            spell = Obj_InList(&link, 0, 4, 2, 0);
            if (spell == 0) {
                wand->id = wand->id & 0xFFF0 | (OBJ_TYPE(wand) + 4) & 0xF;
                game_sprint(0x8B);
                RedisplayInvSlot(FindSlot(wand));
            }
        }
    }
}

/* Drinking a potion. IDA calls this PotionDrink; the FM Towns function in the same place,
   between UseWand_ and flip_switch_, is UseReag_, with the same 0xE1..0xE7 test and call
   to UseFood_. */
char far UseReag(struct Object far *who, struct Object far *obj, char how)
{
    int id;

    id = OBJ_ID(obj);
    if (id >= 0xE1 && id <= 0xE7) {
        if (how != 0)
            UseFood(who, obj, how);
        return 0;
    }
    return 1;
}

char far flip_switch(struct Object far *obj, int state)
{
    int type;

    type = OBJ_TYPE(obj);
    if (state == 0)
        return 0;
    if (state != 3 && (type > 7) != (state > 2))
        return 0;
    play_effect(0x13, (MapObj_X << 3) + 3, (MapObj_Y << 3) + 3, 0);
    type = (type + 8) & 0xF;
    obj->id = obj->id & 0xFFF0 | type & 0xF;
    editchng(2);
    return 1;
}

int far checkLock(struct Object far *who, struct Object far *door, int key)
{
    struct Object far *lock;
    unsigned far *link;
    int result;

    if (!OBJ_ISQUANT(door) && door->ol.f.link > 0) {
        link = &door->ol.word;
        lock = Obj_InList(&link, 0, 4, 0, 0xF);
        if (lock == 0)
            return 1;
        if (!(lock->id & 0x200)) {
            if (key > 0) {
                if ((OBJ_CLASS(door) == 0x14 && OBJ_TYPE(door) >= 8) ||
                    (OBJ_CLASS(door) == 8 && OBJ_TYPE(door) < 0xC && (door->id & 1)))
                    return 4;
                if ((lock->ol.f.link & 0x1FF) == key) {
                    lock->id = lock->id & 0xFDFF | 0x200;
                    return 2;
                }
                return 0;
            }
            return 4;
        } else if (key < 0) {
            result = skill_check(-key, OBJ_Z(lock) * 3);
            if ((OBJ_Z(lock) == 0xE && -key < 0x20) || OBJ_Z(lock) == 0xF || result == 0)
                return result == -1 ? 5 : 0;
            if (result == -1)
                return 5;
        } else if (key > 0) {
            if (!(lock->ol.f.link & 0x1FF) || (lock->ol.f.link & 0x1FF) != key)
                return 0;
        } else
            return 0;
    } else
        return 1;
    checkTrap(who, door, 0xB, MapObj_X, MapObj_Y);
    if (!(lock->id & 0x400)) {
        if (Obj_Rem(link, lock))
            Obj_Free(lock);
    } else
        lock->id = lock->id & 0xFDFF;
    return 3;
}

char far checkSpell(int x, int y, struct Object far *who, struct Object far *obj, char how)
{
    int major;
    int effect;
    unsigned char flag;
    struct Object far *src;

    if (decode_obj_spell(obj, &major, &effect, &flag) && flag) {
        if (how != 0) {
            if (player->game_clock < nextSpellTime) {
                play_effect_here(0x15, 0x40, 0);
                game_sprint(0xB);
                return 0;
            }
            nextSpellTime = player->game_clock + 0x2FD;
            src = who;
        } else {
            if (who == ThePlayer && OBJ_ID(obj) >= 0x98 && OBJ_ID(obj) <= 0x9B)
                return 0;
            src = obj;
        }
        inanimate_spell(x, y, src, who, major, effect);
        useNSpellCharges(obj, 1);
        return 1;
    }
    return 0;
}

void far checkTrap(struct Object far *who, struct Object far *obj, int how, int x, int y)
{
    unsigned far *link;
    struct Object far *trap;
    struct Object far *next;
    char runnext;

    runnext = 1;
    if (OBJ_ISQUANT(obj) || obj->ol.f.link == 0)
        return;
    link = &obj->ol.word;
    trap = Obj_InList(&link, 0, 6, -1, -1);
    if (OBJ_CLASS(obj) == 0x17) {
        runnext = 0;
        if (OBJ_TYPE(obj) > 7) {
            link = &trap->qn.word;
            next = Obj_InList(&link, 0, 6, -1, -1);
            if (next != 0)
                trap = next;
        }
    }
    while (trap != 0) {
        if (OBJ_MINOR(trap) >= 2) {
            link = &trap->qn.word;
            next = Obj_InList(&link, 0, 6, -1, -1);
            UseTrigger(who, obj, trap, how);
            if (runnext)
                trap = next;
            else
                trap = 0;
        } else if (OBJ_FLAGS(trap) == 0 && how == 4 && OBJ_INDEX(trap) != 8) {
            SetOffTrap(who, obj, trap, x, y);
            delete_trap(link, trap);
            trap = 0;
        } else
            trap = 0;
    }
}

void far BlastFunction(void)
{
    release_missile(ObjectActing, ObjectActorArg);
    GameInputMode = 0;
    unforce_mouse_cursor(3);
    mouse_release(1);
}

char far decode_obj_spell(struct Object far *obj, int *major, int *effect, unsigned char *flag)
{
    unsigned far *link;
    struct Object far *spell;

    spell = 0;
    if (OBJ_MAJOR(obj) == 6)
        return 0;
    if (!OBJ_ISQUANT(obj) && obj->ol.f.link > 0) {
        link = &obj->ol.word;
        spell = Obj_InList(&link, 0, 4, 2, 0);
        if (spell != 0 && spell->qn.f.quality == 0 && !always_decode &&
            (int)(((long)rand() * 10) / 0x8000L) < 4)
            return 0;
    } else if (OBJ_ISQUANT(obj) && (obj->id & 0x1000) && OBJ_MAJOR(obj) != 5)
        spell = obj;
    if (spell == 0)
        return 0;
    if ((*flag = (spell->id & 0x800) ? 1 : 0) == 1) {
        if ((*major = (spell->ol.f.link & 0x1FF) >> 6) == 0)
            *major = -1;
        else
            *major += 12;
        *effect = spell->ol.f.link & 0x3F;
    } else {
        *major = (spell->ol.f.link & 0x1FF) >> 4;
        *effect = spell->ol.f.link & 0xF;
    }
    return 1;
}

/* IDA calls this ClearsEnchantmentFlag; the FM Towns function in the same place, between
   decode_obj_spell_ and useNSpellCharges_, is remove_spell_, with the same three tests and
   the same clearing of bit 12. */
void far remove_spell(struct Object far *obj)
{
    if (OBJ_ISQUANT(obj) && (obj->id & 0x1000) && OBJ_MAJOR(obj) != 5)
        obj->id = obj->id & 0xEFFF;
}

int far useNSpellCharges(struct Object far *obj, char n)
{
    unsigned far *link;
    struct Object far *spell;
    char charges;

    if (OBJ_ISQUANT(obj) || obj->ol.f.link == 0)
        return 0;
    link = &obj->ol.word;
    spell = Obj_InList(&link, 0, 4, 2, 0);
    if (spell != 0 && (spell->id & 0x800)) {
        charges = spell->qn.f.quality - n;
        if (charges >= 0)
            spell->qn.f.quality = charges < 0x40 ? charges : 0x3F;
        else if ((int)(((long)rand() * 10) / 0x8000L) < 4) {
            if (Obj_Rem(link, spell))
                Obj_Free(spell);
        }
    }
    return charges > 0 ? charges : 0;
}
