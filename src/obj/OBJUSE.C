/* target: seg040_34E7 */
/* opts: -mm -1 -G -O -d */
/* Using objects: the dispatch on an object's class, removing a used object, putting a new
   object in the hand, keys, wands, potions, switches, locks, spells and traps carried by
   objects, and their charges. The whole of DOS resident segment seg040_34E7, in original
   order. Function and global names are the originals from the FM Towns symbol table; the
   source file's own name is not known.

   UseObj is the entry point for every "use": the player using an object in the 3D view or
   in the inventory (INTERACT.C, BAGS.C), a critter opening a door (PATHFIND.C) and two
   objects colliding (OBJPHYS.C). It dispatches on the object's major and minor class to the
   Use functions, most of them in USEITEMS.C (ovr138), then sets off any trap or trigger
   the object holds (checkTrap) and any spell it carries (checkSpell). An object that needs
   a second object ("Use key on what?") goes through UseThing, which puts it on the cursor
   and stores the function to call in ObjectActor; INTERACT.C makes that call on the next
   click. This file also owns the lock rules (checkLock), the reading of spells held by
   objects (decode_obj_spell, useNSpellCharges) and the delay between casts from objects
   (nextSpellTime).
   Name: inferred (the job of System Shock's OBJUSE.C, object_use and door_locked). */

#include <string.h>
#include <stdlib.h>
#include "combat.h"
#include "event.h"
#include "inv.h"
#include "map.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"

extern struct Inplist near *inplist;
extern void (far *ObjectActor)();
extern char ObjectActorArg;

/* The next time the player may cast from an object, and a flag that makes
   decode_obj_spell always identify the spell. */
long nextSpellTime = 0;
unsigned char always_decode = 0;

void far UseKeyOn(struct Object far *obj, unsigned char how);
struct Object far * far CreateObj(int item, char mobile);
void far mouse_release(int n);
void far get_name(char far *buf, struct Object far *obj, int article, char plural);
void far scroll_print(char far *s);
void far play_effect(char type, int x, int y, int a);
char far play_effect_here(int fx, int vol, int c);
void far UseTrigger(struct Object far *who, struct Object far *obj, struct Object far *trigger, int how);

/* Later in this file. */
char far decode_obj_spell(struct Object far *obj, int *major, int *effect, unsigned char *flag);
int far useNSpellCharges(struct Object far *obj, char n);

/* Uses obj on behalf of who (the player or a critter). how is 1 when the player uses it
   from the inventory and 0 in the world (the 3D view, a critter, a collision). In input
   mode 4 (probably a conversation) only containers can be used. After the class's handler,
   traps and triggers inside the object fire with checkTrap's how 4, and a spell it holds
   is cast, except for wands, food, potions, books and the flam and tym runes, whose
   handlers do that themselves or not at all. Returns obj. */
struct Object far * far UseObj(struct Object far *who, struct Object far *obj, unsigned char how)
{
    int minor;
    char trap;
    struct Object far *fount;

    minor = OBJ_MINOR(obj);
    trap = 1;
    if (inplist->mode == 4 && (OBJ_MAJOR(obj) != MAJOR_MISC || OBJ_MINOR(obj) != 0))
        return obj;
    switch (OBJ_MAJOR(obj)) {
    case MAJOR_HACK:
        if (minor == 1 && !how && who != 0)
            missile_newhit(obj, who);
        break;
    case MAJOR_MISC:
        switch (minor) {
        case 0:
            UseCont(who, obj, how);
            break;
        case 1:
            if (OBJ_INCLASS(obj) < 8)
                UseLight(obj, how);
            else if (OBJ_INCLASS(obj) >= 8) {
                UseWand(obj, how);
                trap = 0;
            }
            break;
        case 2:
            if (!how)
                break;
            if (OBJ_INCLASS(obj) == 0xF)
                UseWatch();
            else if (OBJ_ITEM(obj) == ITEM_STORAGE_CRYSTAL)
                UseCrystal(obj->qn.f.quality);
            break;
        case 3:
            UseFood(who, obj, how);
            trap = 0;
            break;
        }
        break;
    case MAJOR_STUFF:
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
    case MAJOR_SPEC:
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
    case MAJOR_RECT:
        UseRect(who, obj);
        break;
    case MAJOR_TRAP:
        if (OBJ_ITEM(obj) == ITEM_FLAM_RUNE || OBJ_ITEM(obj) == ITEM_TYM_RUNE)
            UseRune(who, obj);
        break;
    case MAJOR_ANIMOBJ:
        switch (OBJ_INCLASS(obj)) {
        case 0xF:
            if ((obj->ol.f.owner & 0xF) < 8)
                CloseDoor(who, obj);
            else
                OpenDoor(who, obj);
            break;
        case 9:
            if (obj->qn.f.next != 0) {
                fount = Obj_PtrTMem(&obj->qn.link);
                if (OBJ_ITEM(fount) == ITEM_FOUNTAIN_12E)
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

/* Deletes a used-up object: from the inventory when inv is set (taking one from a
   stack), otherwise from the list it is in at the tile (MapObj_X, MapObj_Y), or as a loose
   chain if it is not there. how is passed to Obj_Punt as its force flag. Returns 1 when
   the object is gone. */
int far using_punt(struct Object far *obj, char inv, char how)
{
    struct Tile far *tile;
    union Link head;

    if (inv) {
        if (!OBJ_ISQUANT(obj) && obj->ol.f.link > 0)
            Obj_FreeLinkChain(&obj->ol.link, Obj_PtrTMem(&obj->ol.link));
        InvRemoveOneObject(obj);
        obj->ol.f.link = 0;
        obj = Obj_Punt(0L, obj, how);
    } else {
        tile = Map_GetAddr(MapObj_X, MapObj_Y);
        if (Obj_Find(&tile->objects, 1, Obj_MemTPtr(obj)) != 0) {
            obj = Obj_Punt(Obj_Find_Head, obj, how);
            editchng(2);
        } else {
            head.f.index = Obj_MemTPtr(obj);
            Obj_FreeChain(&head);
            obj = 0;
        }
    }
    return obj == 0;
}

/* Puts obj, or a new static object of item when obj is 0, on the mouse cursor (input
   mode 1, carrying), unless the cursor already holds something (then returns 0). */
struct Object far * far place_new(struct Object far *obj, int item)
{
    if (CursorObjPtr != 0)
        return 0;
    if (obj != 0)
        item = OBJ_ITEM(obj);
    else
        obj = CreateObj(item, 0);
    GameInputMode = 1;
    CursorObjPtr = obj;
    force_mouse_cursor(item);
    return obj;
}

/* A key or lockpick used from the inventory asks for the lock to use it on; the lock item
   itself (ITEM_LOCK) does nothing. */
void far UseKey(struct Object far *obj, unsigned char how)
{
    if (!how)
        return;
    if (OBJ_ITEM(obj) == ITEM_LOCKPICK)
        UseThing(obj, UseLockpickOn);
    else if (OBJ_ITEM(obj) < ITEM_LOCK)
        UseThing(obj, UseKeyOn);
}

/* Starts a two-object use: prints "Use <name> on what?", puts obj on the cursor and
   enters input mode 2; the next click calls fn (ObjectActor) on the chosen object with
   ObjectActing set to obj. */
void far UseThing(struct Object far *obj, void (far *fn)())
{
    char buf[40];

    strcpy(buf, "Use ");
    get_name(buf + strlen(buf), obj, 0, 0);
    strcat(buf, " on what?\n");
    scroll_print(buf);
    force_mouse_cursor(OBJ_ITEM(obj));
    CursorObjPtr = obj;
    GameInputMode = 2;
    ObjectActing = obj;
    ObjectActor = fn;
}

/* Using a wand from the inventory casts its spell (checkSpell) and fires its traps. A
   wand whose spell object is gone (useNSpellCharges removes it) becomes the matching
   broken wand, item + 4, with "With a loud <SNAP!>, the wand cracks." Broken wands
   (0x9C-0x9F) do nothing. */
void far UseWand(struct Object far *wand, unsigned char how)
{
    union Link far *link;
    struct Object far *spell;

    if (!how)
        return;
    if (OBJ_INCLASS(wand) < 0xC || OBJ_INCLASS(wand) > 0xF) {
        checkTrap(ThePlayer, wand, 4, MapObj_X, MapObj_Y);
        checkSpell(MapObj_X, MapObj_Y, ThePlayer, wand, how);
        if (!OBJ_ISQUANT(wand)) {
            link = &wand->ol.link;
            spell = Obj_InList(&link, 0, MAJOR_SPEC, 2, 0);
            if (spell == 0) {
                wand->id = wand->id & 0xFFF0 | (OBJ_INCLASS(wand) + 4) & 0xF;
                game_sprint(0x8B); /* "With a loud <SNAP!>, the wand cracks." */
                RedisplayInvSlot(FindSlot(wand));
            }
        }
    }
}

/* Drinking a potion (0xE1-0xE7) through UseFood when used from the inventory; returns 0
   for a potion and 1 for anything else in the class (a runestone), which tells UseObj to
   go on and check its traps and spell. */
/* name: IDA calls this PotionDrink; the FM Towns function in the same place, between
   UseWand_ and flip_switch_, is UseReag_, with the same 0xE1..0xE7 test and call to
   UseFood_. */
char far UseReag(struct Object far *who, struct Object far *obj, char how)
{
    int id;

    id = OBJ_ITEM(obj);
    if (id >= 0xE1 && id <= 0xE7) {
        if (how != 0)
            UseFood(who, obj, how);
        return 0;
    }
    return 1;
}

/* Flips a switch (class 0x17) to its other state, item index +8 or -8, with a click
   sound. state 3 always flips; state 1 or 2 only flips a switch in its first state and
   state 4 and up only one in its second (state > 2 against index > 7); state 0 never.
   Returns 1 when it flipped. */
char far flip_switch(struct Object far *obj, int state)
{
    int type;

    type = OBJ_INCLASS(obj);
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

/* The lock rules. door is a door or container; key is 0 to only ask whether it is locked,
   a key's lock number (the key's owner field) to use a key, or minus the lockpicking skill
   plus one to pick it. The lock is a lock object (ITEM_LOCK) in door's contents: id bit 9
   set when locked, its link the number of the key that fits, its z the difficulty.
   Returns 0 locked (or the attempt failed), 1 no lock, 2 the key locked it, 3 unlocked,
   4 not locked (or open), 5 a lockpick fumble (skill_check's -1).
   Picking rolls skill_check(skill, z * 3); a z of 0xE needs a skill of at least 0x20 and a
   z of 0xF cannot be picked. Unlocking sets off the door's traps with how 0xB, then deletes
   the lock, or only clears its locked bit when its id bit 10 is set (a lock that can be
   locked again). A key can lock an unlocked lock, but not on an open door or an open
   container (odd item below 0x8C, inferred), which answers 4. */
int far checkLock(struct Object far *who, struct Object far *door, int key)
{
    struct Object far *lock;
    union Link far *link;
    int result;

    if (!OBJ_ISQUANT(door) && door->ol.f.link > 0) {
        link = &door->ol.link;
        lock = Obj_InList(&link, 0, MAJOR_SPEC, 0, 0xF);
        if (lock == 0)
            return 1;
        if (!(lock->id & ID_FLAG9)) {
            if (key > 0) {
                if ((OBJ_CLASS(door) == CLASS_DOOR && OBJ_INCLASS(door) >= 8) ||
                    (OBJ_CLASS(door) == CLASS_CONTAINER && OBJ_INCLASS(door) < 0xC && (door->id & 1)))
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
    if (!(lock->id & ID_FLAG10)) {
        if (Obj_Rem(link, lock))
            Obj_Free(lock);
    } else
        lock->id = lock->id & 0xFDFF;
    return 3;
}

/* Casts the spell an object holds, if any (decode_obj_spell) and if it is the kind cast
   on use (id bit 11 of the spell object). Used by the player (how nonzero), the caster is
   who, and casts are limited to one per 0x2FD game clock ticks, about 3 game seconds (the
   watch in USEITEMS.C counts 0x3C00 ticks a minute); otherwise it prints "You are not yet
   ready to cast another spell." In the world the object itself is the source, and the
   player touching an unbroken wand (0x98-0x9B) casts nothing. Spends one charge. Returns 1
   when cast. */
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
                game_sprint(0xB); /* "You are not yet ready to cast another spell." */
                return 0;
            }
            nextSpellTime = player->game_clock + 0x2FD;
            src = who;
        } else {
            if (who == ThePlayer && OBJ_ITEM(obj) >= ITEM_WAND_98 && OBJ_ITEM(obj) <= ITEM_WAND_9B)
                return 0;
            src = obj;
        }
        inanimate_spell(x, y, src, who, major, effect);
        useNSpellCharges(obj, 1);
        return 1;
    }
    return 0;
}

/* Runs the traps and triggers in obj's contents. how says what happened to the object
   (UseObj passes 4 for use, OpenDoor and CloseDoor 8 and 9, checkLock 0xB for an unlock).
   A trigger (trap minor class 2 or 3) goes to UseTrigger, and the next one in the list
   runs after it; a switch runs only one, and a switch in its second state (index above 7)
   runs the second trap in its list. A plain trap (minor 0 or 1) with no flags, other than
   item 0x188 (TRAP_DOOR), goes off only on use (how 4): SetOffTrap, then delete_trap
   removes it. */
void far checkTrap(struct Object far *who, struct Object far *obj, int how, int x, int y)
{
    union Link far *link;
    struct Object far *trap;
    struct Object far *next;
    char runnext;

    runnext = 1;
    if (OBJ_ISQUANT(obj) || obj->ol.f.link == 0)
        return;
    link = &obj->ol.link;
    trap = Obj_InList(&link, 0, MAJOR_TRAP, -1, -1);
    if (OBJ_CLASS(obj) == CLASS_SWITCH) {
        runnext = 0;
        if (OBJ_INCLASS(obj) > 7) {
            link = &trap->qn.link;
            next = Obj_InList(&link, 0, MAJOR_TRAP, -1, -1);
            if (next != 0)
                trap = next;
        }
    }
    while (trap != 0) {
        if (OBJ_MINOR(trap) >= 2) {
            link = &trap->qn.link;
            next = Obj_InList(&link, 0, MAJOR_TRAP, -1, -1);
            UseTrigger(who, obj, trap, how);
            if (runnext)
                trap = next;
            else
                trap = 0;
        } else if (OBJ_FLAGS(trap) == 0 && how == 4 && OBJ_INMAJOR(trap) != 8) {
            SetOffTrap(who, obj, trap, x, y);
            delete_trap(link, trap);
            trap = 0;
        } else
            trap = 0;
    }
}

/* The ObjectActor for throwing or firing: releases the missile ObjectActing with
   ObjectActorArg and ends the targeting mode. */
void far BlastFunction(void)
{
    release_missile(ObjectActing, ObjectActorArg);
    GameInputMode = 0;
    unforce_mouse_cursor(3);
    mouse_release(1);
}

/* Finds the spell an object carries: a spell object (item 0x120, MAJOR_SPEC minor 2 index
   0) in its contents, or, for an is_quant object with the enchanted bit, the object
   itself (not for major class 5, MAJOR_RECT, nor for traps). A spell object with
   quality 0 is hidden 4 times in 10 unless always_decode is set. With id bit 11 set (a
   spell cast on use, its charges shown by LOOK.C) major is 12 plus link bits 6-8, or -1
   when they are 0, and effect is link bits 0-5; otherwise major is link >> 4 and effect
   the low 4 bits (probably a passive enchantment, such as a weapon's). *flag is bit 11. Returns 1
   when there is a spell. */
char far decode_obj_spell(struct Object far *obj, int *major, int *effect, unsigned char *flag)
{
    union Link far *link;
    struct Object far *spell;

    spell = 0;
    if (OBJ_MAJOR(obj) == MAJOR_TRAP)
        return 0;
    if (!OBJ_ISQUANT(obj) && obj->ol.f.link > 0) {
        link = &obj->ol.link;
        spell = Obj_InList(&link, 0, MAJOR_SPEC, 2, 0);
        if (spell != 0 && spell->qn.f.quality == 0 && !always_decode &&
            (int)(((long)rand() * 10) / 0x8000L) < 4)
            return 0;
    } else if (OBJ_ISQUANT(obj) && (obj->id & ID_ENCHANT) && OBJ_MAJOR(obj) != MAJOR_RECT)
        spell = obj;
    if (spell == 0)
        return 0;
    if ((*flag = (spell->id & ID_FLAG11) ? 1 : 0) == 1) {
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

/* Clears an is_quant object's enchanted bit (id bit 12). */
/* name: IDA calls this ClearsEnchantmentFlag; the FM Towns function in the same place,
   between decode_obj_spell_ and useNSpellCharges_, is remove_spell_, with the same three
   tests and the same clearing of bit 12. */
void far remove_spell(struct Object far *obj)
{
    if (OBJ_ISQUANT(obj) && (obj->id & ID_ENCHANT) && OBJ_MAJOR(obj) != MAJOR_RECT)
        obj->id = obj->id & 0xEFFF;
}

/* Spends n charges of the cast-on-use spell in obj's contents; its quality is the charge
   count. When there are not enough charges the spell object is deleted, 4 times in 10.
   Returns the charges left (0 when none). */
int far useNSpellCharges(struct Object far *obj, char n)
{
    union Link far *link;
    struct Object far *spell;
    char charges;

    if (OBJ_ISQUANT(obj) || obj->ol.f.link == 0)
        return 0;
    link = &obj->ol.link;
    spell = Obj_InList(&link, 0, MAJOR_SPEC, 2, 0);
    if (spell != 0 && (spell->id & ID_FLAG11)) {
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
