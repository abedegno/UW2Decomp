/* target: seg044_368F */
/* opts: -mm -1 -G -O -Y -d */
/* Animated objects and timers: the list of running animation overlays (moving doors,
   explosions, splashes and other class 7 effects), stepping them each frame, ending
   them, creating the effect objects that carry them, and the list of timer triggers.
   The whole of DOS resident segment seg044_368F, in original order.

   What it does in the game: an animated object (MAJOR_ANIMOBJ, items 0x1C0..0x1CF) is an
   ordinary object in a tile's list plus an entry in animlist, which says how many frames it
   has left and on which tile it is. Its animation class (the low four bits of its item,
   animclassd, loaded with the class data in ANIMOBJ.C) says what happens each frame: the
   frame number, kept in the owner field, cycles or is picked at random, and a moving door
   swings or rises. update_animobj, called from the main loop, steps every entry, ends the
   expired ones (toast_animobj: a moving door becomes a door again, open or closed; an
   effect is removed from the map) and fires the timer triggers. put_effect makes the blood,
   dust, sparks and spell effects of combat and magic (COMBAT.C, SPELLS.C); fireball_effect
   adds the scattered flames of an explosion; mts_doanim turns a fireball or lightning
   missile that has struck into its explosion.

   Data owned: animlist and animcount (at most 64 running animations), animclassd (16
   classes), timerlist and timercount (at most 64 timer triggers), and timer_tick. The
   level block keeps copies of the animation and timer lists, which MAP.C's Anim_Load and
   Anim_Save move in and out.
   Function and global names are the originals from the FM Towns symbol table where it has
   them.
   Name: inferred (add_animobj, update_animobj, put_effect; the job of System Shock's
   EFFECT.C, which has add_obj_to_animlist and do_special_effect). */

#include <stdlib.h>
#include "combat.h"
#include "critter.h"
#include "event.h"
#include "map.h"
#include "object.h"
#include "player.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"
#include "uw2.h"

#include "view3d.h"
unsigned char lengset = 0;              /* DS:98E, check_door set the length itself */
unsigned char DoAnimO = 1;              /* DS:98F */
static int16 timer_tick = 0;            /* DS:990; name: FM Towns keeps it in _spec_col */

char animcount;                         /* DS:34B4 */
struct AnimClass animclassd[16];        /* DS:34B6 */
int16 timerlist[0x40];                  /* DS:34F6 */
char timercount;                        /* DS:3576 */
struct Anim animlist[0x40];             /* DS:3578 */



/* Removes animation n's object from its tile and frees it. */
void far rem_anim_from_map(int n)
{
    struct Tile far *t;
    struct Object far *obj;

    obj = Obj_PtrTMem(&animlist[n].link);
    t = Map_GetAddr(animlist[n].x, animlist[n].y);
    if (Obj_Rem(&t->objects, obj))
        Obj_Free(obj);
}

/* Drops the animation of object index from animlist (the last entry fills its place). */
void far rem_anim_from_list(int index)
{
    int i;

    for (i = 0; i < animcount; i++)
        if (animlist[i].link.f.index == index)
            break;
    if (i < animcount && --animcount > 0 && animcount != i)
        animlist[i] = animlist[animcount];
}

/* Ends animation n. A class with flag 0x80 first plays its remaining frames. A moving
   door (class 0xF; its owner field holds the door's minor class and state) becomes a door
   again: closing (flag 8) it becomes the closed door, unless something now stands in the
   doorway, when it turns round (changeDoor) and keeps moving; opening it becomes the open
   door (state | 8). It plays the door sound (0x26 for a portcullis, door_type 6, else 0xC).
   A class with flag 0x20 is then removed from the map. */
void far toast_animobj(int n, int frames)
{
    struct Object far *obj;
    unsigned type;
    int cls;
    int z;
    int major;
    int minor;
    int sound;
    unsigned state;

    obj = Obj_PtrTMem(&animlist[n].link);
    cls = OBJ_INCLASS(obj);
    type = animclassd[cls].flags;
    if ((type & 0x80) && animlist[n].len != 0)
        do_animobj(n, animlist[n].len);
    switch (cls) {
    case 0xF:
        major = MAJOR_RECT;
        minor = obj->ol.f.owner >> 4;
        state = (obj->ol.f.owner >> 0) & 0xF;
        z = OBJ_Z(obj);
        if (OBJ_FLAGS(obj) & 8) {
            if (state >= 8)
                state -= 8;
            if ((state & 7) != 6)
                z -= 0x18;
            XP = animlist[n].x;
            YP = animlist[n].y;
            if (!can_place((major << 6) + (minor << 4) + state, Obj_MemTPtr(obj),
                           (XP << 3) + OBJ_FINEX(obj), (YP << 3) + OBJ_FINEY(obj), z, 1, 8)) {
                obj->ol.f.owner = state;
                changeDoor(obj);
                return;
            }
            if (!quick_time) {
                if (door_type != 6)
                    sound = 0xC;
                else
                    sound = 0x26;
                play_effect(sound, (XP << 3) + OBJ_FINEX(obj), (YP << 3) + OBJ_FINEY(obj), 0);
            }
        } else
            state |= 8;
        SET_Z(obj, z);
        SET_MAJOR(obj, major);
        SET_MINOR(obj, minor);
        SET_INCLASS(obj, state);
        obj->ol.f.owner = 0;
        if (OBJ_FLAGS(obj) & 8)
            SET_FLAGS(obj, OBJ_FLAGS(obj) & 7);
        else
            SET_FLAGS(obj, (OBJ_FLAGS(obj) & 7) + 8);
    }
    if (type & 0x20)
        rem_anim_from_map(n);
    if (--animcount > 0 && animcount != n)
        animlist[n] = animlist[animcount];
}

/* Ends the animation of object index at once, if it has one. No FM Towns counterpart. */
void far seg044_368F_392(int index)
{
    int i;

    for (i = 0; i < animcount; i++)
        if (animlist[i].link.f.index == index) {
            toast_animobj(i, 0);
            break;
        }
}

/* The animation of object from now belongs to object to (an object was replaced). */
void far Change_AnimPtr(struct Object far *to, struct Object far *from)
{
    int toidx;
    int fromidx;
    int i;

    toidx = Obj_MemTPtr(to);
    fromidx = Obj_MemTPtr(from);
    for (i = 0; i < animcount; i++)
        if (animlist[i].link.f.index == fromidx) {
            animlist[i].link.f.index = toidx;
            break;
        }
}

/* Starts an animation of len frames (-1 for ever) for object index on tile x, y, with
   frame a (modulo the class's frame count) as its first. Returns the new count, or -1 when
   the list is full. */
int far add_animobj(int index, int len, unsigned char a, unsigned char x, unsigned char y)
{
    struct Object far *obj;
    register int frame;

    if (animcount + 1 > 0x40)
        return -1;
    animlist[animcount].link.f.index = index;
    animlist[animcount].len = len;
    animlist[animcount].x = x;
    animlist[animcount].y = y;
    obj = Obj_PtrTMem(&animlist[animcount].link);
    frame = animclassd[OBJ_INCLASS(obj)].start;
    if (frame >= 0)
        obj->ol.f.owner = animclassd[OBJ_INCLASS(obj)].count
            ? animclassd[OBJ_INCLASS(obj)].start + a % animclassd[OBJ_INCLASS(obj)].count
            : animclassd[OBJ_INCLASS(obj)].start;
    AnimObjInPipe = 1;
    return ++animcount;
}

/* Steps animation n by frames, for each flag of its class: 1 the next frame (wrapping),
   2 a random frame, 4 door motion (the open angle in the low three flag bits, opening or
   closing by flag 8; a portcullis, state 6, rises or falls 6 units a frame; a closing door
   is checked for obstruction by check_door). */
void far do_animobj(int n, register int frames)
{
    struct Object far *obj;
    unsigned type;
    unsigned mask;
    unsigned owner;
    register int cls;

    obj = Obj_PtrTMem(&animlist[n].link);
    if (OBJ_CLASSID(obj) != FIRST_ANIMOBJ)
        return;
    cls = OBJ_INCLASS(obj);
    type = animclassd[cls].flags;
    for (mask = 1; type > 0; type &= ~mask, mask = mask << 1) {
        switch (type & mask) {
        case 1:
            owner = obj->ol.f.owner;
            if (animclassd[cls].start + animclassd[cls].count - 1 > owner)
                obj->ol.f.owner++;
            else
                obj->ol.f.owner = animclassd[cls].start;
            break;
        case 2:
            obj->ol.f.owner = animclassd[cls].start + rand() % animclassd[cls].count;
            break;
        case 4:
            if (OBJ_FLAGS(obj) & 8)
                frames = -frames;
            if (((obj->ol.f.owner >> 0) & 7) == 6)
                SET_Z(obj, OBJ_Z(obj) + frames * 6);
            SET_FLAGS(obj, (OBJ_FLAGS(obj) & 7) + frames + (OBJ_FLAGS(obj) & 8));
            if (OBJ_FLAGS(obj) & 8)
                check_door(n, frames);
            break;
        }
    }
}

/* Called each game frame with the frames elapsed. Steps every animation and ends those
   that run out; while time is frozen (TimeStop) only moving doors move. Then the timer
   triggers: each fires (UseTrigger with how 10, the timer mode) once every z + 1 ticks of
   timer_tick, but only while the player or the roaming eye is within 8 squares of the
   trigger's target (its quality and owner fields hold the target square). */
void far update_animobj(int frames)
{
    struct Object far *obj;
    int i;
    int left;

    if (AnimObjInPipe)
        editchng(2);
    for (i = 0; i < animcount; i++) {
        if (TimeStop && OBJ_INCLASS(Obj_PtrTMem(&animlist[i].link)) != 0xF)
            continue;
        if (animlist[i].len == -1)
            do_animobj(i, frames);
        else {
            left = animlist[i].len - frames;
            if (left < 0)
                toast_animobj(i, frames);
            else {
                do_animobj(i, frames);
                if (lengset)
                    lengset = 0;
                else
                    animlist[i].len = left;
            }
        }
    }
    if (!TimeStop) {
        for (i = 0; i < timercount; i++) {
            obj = Obj_IntTMem(timerlist[i]);
            left = (timer_tick + frames) / (OBJ_Z(obj) + 1) - timer_tick / (OBJ_Z(obj) + 1);
            if (left <= 0)
                continue;
            if (abs(OBJ_HOMEX(ThePlayer) - obj->qn.f.quality) < 8
                && abs(OBJ_HOMEY(ThePlayer) - obj->ol.f.owner) < 8
                || abs(lastXeye - obj->qn.f.quality) < 8 && abs(lastYeye - obj->ol.f.owner) < 8)
                while (left > 0) {
                    UseTrigger(ThePlayer, 0L, obj, 10);
                    left--;
                }
        }
    }
    timer_tick = (timer_tick + frames) & 0x7F;
}

/* Scatters three to five flame copies of the explosion src around it on tile x, y, each a
   random explosion frame item (the item + 1 or + 2), offset a little and short-lived. */
void far fireball_effect(struct Object far *src, int x, int y)
{
    struct Object far *copy;
    int r;
    int off;
    int n;

    for (n = rand() % 3 + 2; n >= 0; n--) {
        copy = Obj_Alloc(0);
        *(struct StaticObj far *)copy = *(struct StaticObj far *)src;
        SET_ITEM(copy, OBJ_ITEM(copy) + (rand() & 1) + 1);
        r = OBJ_FINEX(copy);
        do
            off = rand() % 5 - 2;
        while (r + off < 0 || r + off > 7);
        SET_FINEX(copy, r + off);
        r = OBJ_FINEY(copy);
        do
            off = rand() % 5 - 2;
        while (r + off < 0 || r + off > 7);
        SET_FINEY(copy, r + off);
        SET_Z(copy, OBJ_Z(copy) - 8 + (rand() & 0xF));
        Obj_Add(&Map_GetAddr(x, y)->objects, copy);
        r = rand() % 3;
        off = 2 - r + rand() % 3;
        if (add_animobj(Obj_MemTPtr(copy), off, r, x, y) == -1) {
            if (Obj_Rem(&Map_GetAddr(x, y)->objects, copy))
                Obj_Free(copy);
            n = -1;
        }
    }
}

/* A missile has struck at x, y: a fireball becomes an explosion with flames and
   damage_square kind 1 (magic and fire), a lightning bolt a lightning flash with kind 2
   (magic). Returns 0 for any other missile (inferred: the caller then handles it as a plain
   missile). */
unsigned char far mts_doanim(struct Object far *obj, int x, int y, char who)
{
    int16 from[3] = { ITEM_FIREBALL_14, ITEM_LIGHTNING_BOLT, ITEM_FIREBALL_1D };
    int16 to[3] = { ITEM_EXPLOSION_1C2, ITEM_LIGHTNING_1C5, ITEM_EXPLOSION_1C2 };
    int i;

    for (i = 0; i < 3; i++)
        if (from[i] == OBJ_ITEM(obj))
            break;
    if (i >= 3)
        return 0;
    SET_ITEM(obj, to[i]);
    if (add_animobj(Obj_MemTPtr(obj), 4, 0, x, y) == -1)
        return 0;
    if (i != 1)
        fireball_effect(obj, x, y);
    damage_square(x, y, i + 1, who);
    return 1;
}

/* Creates effect cls (an animation class, item FIRST_ANIMOBJ + cls) on tile x, y for len
   frames starting at frame: at who's fine position, and at height -z when z is negative,
   else who's z plus z eighths of who's height. Returns 0 when it could not be made. */
unsigned char far put_effect(struct Object far *who, int cls, int len, int frame,
                             int z, int x, int y)
{
    struct Object far *p;
    unsigned char h;

    if ((p = CreateObj(cls + FIRST_ANIMOBJ, 0)) == 0)
        return 0;
    if (who) {
        SET_FINEX(p, OBJ_FINEX(who));
        SET_FINEY(p, OBJ_FINEY(who));
    }
    if (z < 0)
        SET_Z(p, -z);
    else if (who) {
        h = ComObjData[OBJ_ITEM(who)].height >> 3;
        if (h == 0)
            h = 1;
        h = h * z;
        SET_Z(p, OBJ_Z(who) + h);
    }
    if (add_animobj(Obj_MemTPtr(p), len, frame, x, y) == -1) {
        Obj_Free(p);
        return 0;
    }
    Obj_AddEnd(&Map_GetAddr(x, y)->objects, p);
    return 1;
}

/* put_effect with the fine position given rather than copied, and the effect added at
   the head of the tile's list. */
/* name: no FM Towns counterpart; IDA's name. */
unsigned char far CreateAnimoForSrcObject_seg044_368F_CE3(struct Object far *src, int cls,
        int len, unsigned char frame, int z, int x, int y, int finex, int finey)
{
    struct Object far *p;
    unsigned char h;

    if ((p = CreateObj(cls + FIRST_ANIMOBJ, 0)) == 0)
        return 0;
    SET_FINEX(p, finex);
    SET_FINEY(p, finey);
    if (z < 0)
        SET_Z(p, -z);
    else {
        h = ComObjData[OBJ_ITEM(src)].height >> 3;
        if (h == 0)
            h = 1;
        h = h * z;
        SET_Z(p, OBJ_Z(src) + h);
    }
    if (add_animobj(Obj_MemTPtr(p), len, frame, x, y) == -1) {
        Obj_Free(p);
        return 0;
    }
    Obj_Add(&Map_GetAddr(x, y)->objects, p);
    return 1;
}

/* The animlist entry of obj, or -1. */
int far find_anim(struct Object far *obj)
{
    int index;
    int i;

    index = Obj_MemTPtr(obj);
    for (i = 0; i < animcount; i++)
        if (animlist[i].link.f.index == index)
            break;
    if (i == animcount)
        return -1;
    return i;
}

int far get_animlen(struct Object far *obj)
{
    int i;

    i = find_anim(obj);
    if (i < 0)
        return -2;
    return animlist[i].len;
}

void far set_animlen(struct Object far *obj, int len)
{
    int i;

    i = find_anim(obj);
    if (i >= 0)
        animlist[i].len = len;
}

/* A closing door (animation n) checks its doorway. If something is in the way it fires
   the door's open triggers (checkTrap with mode 8, the Guide's open trigger), turns the
   door round and shortens the run. Returns 1 when the way is clear. */
unsigned char far check_door(int n, int frames)
{
    struct Object far *obj;
    int item;
    int minor;
    int len;
    int z;
    int limit;

    limit = 5;
    obj = Obj_PtrTMem(&animlist[n].link);
    minor = (obj->ol.f.owner >> 0) & 0xF;
    item = (obj->ol.f.owner >> 4 << 4) + minor + FIRST_RECT;
    z = OBJ_Z(obj);
    if ((minor & 7) != 6)
        z -= 0x18;
    XP = animlist[n].x;
    YP = animlist[n].y;
    if (!can_place(item, Obj_MemTPtr(obj), (XP << 3) + OBJ_FINEX(obj),
                   (YP << 3) + OBJ_FINEY(obj), z, 1, 8)) {
        if (OBJ_MAJOR(obj) == MAJOR_RECT && (obj->id & 7) == 6
            || OBJ_MAJOR(obj) == MAJOR_ANIMOBJ && (obj->ol.f.owner & 7) == 6)
            limit = 4;
        SET_FLAGS(obj, OBJ_FLAGS(obj) & 7);
        checkTrap(0L, obj, 8, XP, YP);
        SET_FLAGS(obj, (OBJ_FLAGS(obj) & 8) + ((OBJ_FLAGS(obj) & 7) - frames - 1));
        len = get_animlen(obj);
        if (len >= 0) {
            animlist[n].len = limit - len + 1;
            lengset = 1;
        }
        return 0;
    }
    return 1;
}

/* Adds an object to the timer list. */
/* name: no FM Towns counterpart and no caller. */
static unsigned char far add_timer_obj(int index)
{
    if (timercount < 0x40) {
        timerlist[timercount] = index;
        timercount++;
    } else
        return 0;
    return 1;
}

unsigned char far rem_timer_obj(int index)
{
    int i;

    for (i = 0; i < timercount; i++)
        if (timerlist[i] == index) {
            timerlist[i] = timerlist[--timercount];
            return 1;
        }
    return 0;
}
