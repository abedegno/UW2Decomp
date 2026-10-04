/* target: ovr153 */
/* opts: -mm -1 -G -O -Y -d */
/* Triggers and traps: running a trigger's trap chain, the per-type trap actions (UseTrap)
   and the special-purpose "hack" traps, the variable traps, removing triggers and traps,
   wandering monsters and closing doors. The whole of UW1's DOS overlay ovr153 (UW2's
   ovr166), in original order.

   What it does in the game: triggers and traps are MAJOR_TRAP objects (items 0x180..0x1BF;
   minor class 0 traps, 2 triggers). A trigger sits on a tile or inside an object; its
   quality and owner fields name a target square, and its link points at a trap there. The
   game reports an action with UseTrigger(who, object, trigger, kind), where kind is a
   trigger mode from Triggers[] (OBJECTS.DAT's trigger type table). A trigger of the
   matching mode runs its trap (SetOffTrap, UseTrap), and each trap passes on to the object
   its link names, so traps form chains; the variable trap branches. A trigger without
   ID_FLAG10 is used up: its trap chain is deleted after it runs.

   UW1 against UW2: triggers are minor class 2 only; there are no pressure plates, bridges,
   oscillators, pits, skill, proximity, experience or jump traps, and the variables are
   UW1's (quest bits and the player's 64 game variables, both in the player record) and
   handled inside UseTrap; a variable check that fails runs the else chain only through a
   trigger; a missing inventory item ends the chain; the hack traps are UW1's own;
   gronkify_talkto_player (new) starts a conversation with a waiting critter. UW1's player
   record differs (Player1Trap below).

   Data owned: Triggers[16] (the trigger modes, read by trap_init), the trigger that is
   running (CharacterThatTriggeredTrap, TriggeringButton), the removal state, and
   tile_walls.
   UW1 has no symbol-bearing build: function and global names are UW2's (the FM Towns
   originals) where the routine is the same, else chosen for the overlay's stub order
   (Turbo C lists a file's publics by the tools/bssorder.py key of each name, and TLINK
   numbers the stub entries from the last one listed).
   Name: UW2Decomp's (triggers and traps: UseTrigger, SetOffTrap, UseTrap; the job of
   System Shock's TRIGGER.C). */
#include <stdlib.h>
#include <stdio.h>
#include "combat.h"
#include "critter.h"
#include "event.h"
#include "file.h"
#include "inv.h"
#include "map.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"
#include "uw2.h"

/* Declared in each file that uses it, its own way (no header). */
unsigned char far IsMobElem(struct Object far *obj);

/* UW1: declarations the headers do not have (or have in UW2's form). */
void far ExplodingBook_ovr107_1259(struct Object far *trap, int x, int y);

/* This file's _BSS, DS:736E..738D: the trigger modes, the trap being removed, and the
   trigger that is running. */
/* match: laid out by name (tools/bssorder.py): RemoveTrapIndex 58, RemoveTrapFlags 74,
   ObjRunCodeAround 447, CharacterThatTriggeredTrap 459, TriggeringButton 460,
   Triggers 996. */
/* name: only Triggers has an FM Towns name; the others only this file uses, so they are
   static, their provisional names (UW2Decomp's) chosen for their keys. */
unsigned char Triggers[16];
static int16 RemoveTrapIndex;                           /* DS:736E */
static int16 RemoveTrapFlags;                           /* DS:7370 */
static struct Object far *ObjRunCodeAround;             /* DS:7372 */
static struct Object far *CharacterThatTriggeredTrap;   /* DS:7376 */
static struct Object far *TriggeringButton;             /* DS:737A */

/* This file's _DATA, DS:1D8A..1DB3, in definition order, then its string. */
unsigned char tile_walls[16] = { 30, 0, 19, 21, 11, 13, 32, 32, 32, 32, 0, 0, 0, 0, 0, 30 };
/* name: UW2Decomp's CreatedObjectFound_dseg_67d6_1BB8 (provisional), with UW1's address. */
unsigned char CreatedObjectFound_dseg_5c99_1D9A = 0;            /* DS:1D9A */

/* Reads the 16-byte trigger type table from OBJECTS.DAT (handle positioned by the caller). */
void far trap_init(FILE *handle)
{
    fread(Triggers, 1, 16, handle);
}

/* The class data of ActiveObj: a trigger's entry in Triggers[], none for a trap.
   UW1: triggers are minor class 2 only. */
unsigned char near * far trap_class_data(void)
{
    if (OBJ_MINOR(ActiveObj) == 2)
        return Triggers + OBJ_INCLASS(ActiveObj);
    return 0;
}

/* Fires trigger trig for an action of kind type by who on object start (type -1: reached
   along a trap chain, no checks). The trigger must be a trigger (minor class 2) of the
   mode type. A switch that is already on (class SWITCH, minor index above 7) passes the
   action to the next trigger in its list. Who may set it off: the player only if
   ID_FLAG11 is set (and a look trigger above floor level needs a Search skill check
   against its height), critters only with ID_ENCHANT set and not on a MAJOR_RECT trigger,
   other objects unless the trigger has ID_ENCHANT, is not on a MAJOR_RECT and lacks
   ID_FLAG11. The trap chain runs at the trigger's target square; a trigger without
   ID_FLAG10 then deletes the chain (result | 0x20). Returns 2 when nothing ran. */
int far UseTrigger(struct Object far *who, struct Object far *start,
                   struct Object far *trig, int type)
{
    struct Object far *trap;
    int quality;
    int owner;
    int minor;
    register int sub;
    register int result;
    minor = OBJ_MINOR(trig);
    sub = trig->id & ID_INCLASS;
    if (minor != 2) return 2;
    if (type >= 0) {
        if (start != 0 && OBJ_CLASS(start) == CLASS_SWITCH &&
            OBJ_INCLASS(start) > 7 && trig->qn.f.next != 0) {
            trig = Obj_PtrTMem(&trig->qn.link);
            return UseTrigger(who, start, trig, type);
        }
        if (Triggers[sub] != type) return 2;
        if (OBJ_ITEM(who) == ITEM_ADVENTURER) {
            if (!(trig->id & ID_FLAG11)) return 2;
            if (type == 5 && (trig->pos & POS_Z) > 0 &&
                skill_check(player->skills[11], trig->pos & POS_Z) <= 0)
                return 2;
        } else {
            if (OBJ_MAJOR(who) == MAJOR_CREATURE) {
                if (!(trig->id & ID_ENCHANT) ||
                    OBJ_MAJOR(trig) == MAJOR_RECT) return 2;
            }
            if (OBJ_MAJOR(who) != MAJOR_CREATURE && (trig->id & ID_ENCHANT) &&
                OBJ_MAJOR(trig) != MAJOR_RECT && !(trig->id & ID_FLAG11))
                return 2;
        }
    }
    trap = Obj_PtrTMem(&trig->ol.link);
    quality = trig->qn.f.quality;
    owner = trig->ol.f.owner;
    if (trap == 0) return 2;
    result = SetOffTrap(who, start, trap, quality, owner);
    if (!(trig->id & ID_FLAG10) && trig->ol.f.link > 0) {
        delete_trap(&Map_GetAddr(quality, owner)->objects, trap);
        result |= 0x20;
    }
    return result;
}
/* Runs trap at square x, y, remembering who set the chain off and with which object.
   The function has no return statement, as in DOS, so its callers get whatever is
   left in AX. */
int far SetOffTrap(struct Object far *who, struct Object far *context,
                    struct Object far *trap, int x, int y)
{
    register int result;
    if (CharacterThatTriggeredTrap == 0) {
        CharacterThatTriggeredTrap = who;
        TriggeringButton = context;
    }
    result = UseTrap(trap, x, y);
    CharacterThatTriggeredTrap = 0;
    AX_RESULT(result);                  /* AX still holds UseTrap's result */
}

/* Performs one trap at square x, y, then, unless the trap ends the chain, passes on to the
   trap or trigger its link names. Returns 2, or the result of the chain. By trap type
   (the item within MAJOR_TRAP):
   - 0 damage hits who for quality (heals when owner is set); 1 teleports who to square
     (quality, owner) of level z; 2 fires an arrow; 3 a hack trap (do_trap_hack);
     5 changes the terrain; 6 casts a spell (inanimate_spell).
   - 7 creates a copy of its linked object on the square with chance (63 - quality) in 63,
     not when another created object is within four squares.
   - 8 opens (1), closes (2) or toggles (3) the door on the square, replacing its lock by
     a copy of the trap's linked lock.
   - 9 and 10 a rune of warding: hits the critter that steps on it, of class quality (0x3F
     any), for 3 + rand * Casting, and tells the player. 11 deletes its linked object from
     square (quality, owner).
   - 12 ends the chain unless the player has the item quality * 32 + owner (with z: at
     least that many).
   - 13 sets a variable: z 0 a quest bit (heading 5 toggles, 1 clears, others set), else
     game variable z by operation heading (add, subtract, set, and, or, xor, shift left;
     kept to 6 bits). 14 checks variables z..z + heading (summed with fine x set, else
     packed 3 bits each) against the value, and on a mismatch runs the else chain through
     the trigger after its linked object.
   - 16 prints string quality * 32 + owner of block 9. */
int far UseTrap(struct Object far *trap, int x, int y)
{
    struct Tile far *tile;
    struct Object far *linked;
    struct Object far *obj;
    struct Object far *lock;
    union Link far *head;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    int h;
    int spell;
    int power;
    int result = 2;
    unsigned char continue_chain = 1;
    int16 slot;
    register int i;
    register int v;

    switch (OBJ_INMAJOR(trap)) {
    case 2:
        trap_fire(trap, x, y);
        break;
    case 1:
        v = trap->qn.f.quality;
        b = trap->ol.f.owner;
        c = OBJ_Z(trap);
        result = do_teleport(CharacterThatTriggeredTrap, v, b, c);
        break;
    case 5:
        e = trap->ol.f.owner;
        d = trap->qn.f.quality >> 1;
        c = OBJ_Z(trap) >> 3;
        i = (OBJ_HEADING(trap) << 1) + (trap->qn.f.quality & 1);
        if (i == 15)
            i = 10;
        f = OBJ_FINEX(trap);
        g = OBJ_FINEY(trap);
        result = change_terrain(x, y, e, d, c, i, f, g, 0);
        break;
    case 0xD:
        i = OBJ_Z(trap);
        d = (trap->qn.f.quality << 5 | trap->ol.f.owner) << 3 | OBJ_FINEY(trap);
        c = OBJ_HEADING(trap);
        if (i == 0) {
            switch (c) {
            case 5:
                player->quests ^= 1L << d;
                break;
            case 1:
                player->quests &= ~(1L << d);
                break;
            default:
                player->quests |= 1L << d;
                break;
            }
        } else {
            switch (c) {
            case 0: player->game_vars[i] += d; break;
            case 1: player->game_vars[i] -= d; break;
            case 2: player->game_vars[i] = d; break;
            case 3: player->game_vars[i] &= d; break;
            case 4: player->game_vars[i] |= d; break;
            case 5: player->game_vars[i] ^= d; break;
            case 6: player->game_vars[i] <<= d; break;
            }
            player->game_vars[i] = player->game_vars[i] & 0x3F;
        }
        break;
    case 0xE:
        i = OBJ_Z(trap);
        c = i + OBJ_HEADING(trap);
        d = (trap->qn.f.quality << 5 | trap->ol.f.owner) << 3 | OBJ_FINEY(trap);
        for (v = 0; i <= c; i++) {
            if (OBJ_FINEX(trap))
                v += player->game_vars[i];
            else {
                v <<= 3;
                v |= player->game_vars[i] & 7;
            }
        }
        if (v != d) {
            if (trap->ol.f.link > 0) {
                obj = Obj_PtrTMem(&trap->ol.link);
                if (obj->qn.f.next > 0)
                    return UseTrigger(CharacterThatTriggeredTrap, TriggeringButton,
                                      Obj_PtrTMem(&obj->qn.link), -1);
                return 2;
            }
        }
        break;
    case 0:
        if ((int)(((int32)rand() * 10) / 0x8000L) < 7)
            h = 2;
        else
            h = 0;
        result = whack_thing(Obj_MemTPtr(CharacterThatTriggeredTrap),
                             trap->qn.f.quality * (trap->ol.f.owner ? -1 : 1), 4, h);
        break;
    case 3:
        result = do_trap_hack(trap, x, y);
        break;
    case 6:
        spell = trap->qn.f.quality;
        power = trap->ol.f.owner;
        result = inanimate_spell(x, y, trap, CharacterThatTriggeredTrap,
                                 spell, power);
        break;
    case 7: {
        unsigned char placed;
        if ((int)(((int32)rand() * 0x3F) / 0x8000L) < trap->qn.f.quality)
            return 2;
        continue_chain = 0;
        if OBJ_ISQUANT(trap)
            break;
        linked = Obj_PtrTMem(&trap->ol.link);
        if (linked == 0)
            break;
        if (OBJ_MAJOR(linked) == MAJOR_CREATURE && dont_create_wandering_monster_here(linked))
            break;
        if ((obj = Obj_Alloc(IsMobElem(linked))) != 0) {
            if (IsMobElem(linked))
                *(struct Object far *)obj = *(struct Object far *)linked;
            else
                *(struct StaticObj far *)obj = *(struct StaticObj far *)linked;
            stay_centered = 1;
            placed = put_at((x << 3) + OBJ_FINEX(obj), (y << 3) + OBJ_FINEY(obj),
                            OBJ_Z(obj), obj, 4, 0);
            stay_centered = 0;
            {
                struct Object far *copy;
                if (placed && !OBJ_ISQUANT(obj) && obj->ol.f.link > 0 &&
                    (copy = Obj_Alloc(0)) != 0) {
                    *(struct StaticObj far *)copy =
                        *(struct StaticObj far *)Obj_IntTMem(obj->ol.f.link);
                    obj->ol.f.link = Obj_MemTPtr(copy);
                    while (copy->qn.f.next != 0)
                        copy->qn.f.next = 0;
                    if (!OBJ_ISQUANT(copy) && copy->ol.f.link > 0)
                        copy->ol.f.link = 0;
                }
            }
            if (placed && OBJ_MAJOR(obj) == MAJOR_ANIMOBJ) {
                int index = Obj_MemTPtr(obj);
                add_animobj(index, -1, 0, x, y);
            }
        }
        result = 2;
        break;
    }
    case 8:
        continue_chain = 0;
        tile = Map_GetAddr(x, y);
        head = &tile->objects;
        linked = Obj_InList(&head, 0, MAJOR_RECT, 0, -1);
        MapObj_X = x;
        MapObj_Y = y;
        if (linked != 0) {
            head = &linked->ol.link;
            lock = Obj_InList(&head, 0, MAJOR_SPEC, 0, 0xF);
            if (lock != 0) {
                Obj_Rem(head, lock);
                Obj_Free(lock);
            }
            if (!OBJ_ISQUANT(trap) && trap->ol.f.link != 0) {
                lock = Obj_PtrTMem(&trap->ol.link);
                if ((obj = Obj_Alloc(0)) != 0) {
                    *(struct StaticObj far *)obj = *(struct StaticObj far *)lock;
                    Obj_Add(head, obj);
                }
            }
            switch (trap->qn.f.quality) {
            case 1:
                OpenDoor(CharacterThatTriggeredTrap, linked);
                break;
            case 2:
                CloseDoor(linked);
                break;
            case 3:
                ToggleDoor(CharacterThatTriggeredTrap, linked);
                break;
            }
        } else if ((linked = Obj_InList(&head, 0, MAJOR_ANIMOBJ, -1, 0xF)) != 0) {
            if ((linked->ol.f.owner & 0xF) < 8) {
                switch (trap->qn.f.quality) {
                case 2:
                case 3:
                    CloseDoor(linked);
                }
            } else {
                switch (trap->qn.f.quality) {
                case 1:
                case 3:
                    OpenDoor(CharacterThatTriggeredTrap, linked);
                }
            }
        }
        break;
    case 0xB:
        head = &Map_GetAddr(trap->qn.f.quality, trap->ol.f.owner)->objects;
        lock = Obj_PtrTMem(&trap->ol.link);
        Obj_FreeLinkChain(head, lock);
        editchng(2);
        continue_chain = 0;
        break;
    case 0xC:
        i = trap->qn.f.quality << 5 | trap->ol.f.owner;
        obj = FindObj(i >> 6, (i & 0x30) >> 4, i & 0xF, 4, &slot);
        if (obj == 0)
            return 2;
        if (OBJ_Z(trap) > 0 && OBJ_ISQUANT(obj) &&
            !(obj->ol.f.link & LINK_SPECIAL) && OBJ_Z(trap) > obj->ol.f.link)
            return 2;
        break;
    case 9:
    case 0xA:
        if (CharacterThatTriggeredTrap != 0 &&
            (trap->qn.f.quality == 0x3F ||
             trap->qn.f.quality == OBJ_INCLASS(CharacterThatTriggeredTrap))) {
            int damage;
            damage = (int)(((int32)rand() * player->skills[9]) / 0x8000L) + 3;
            print_path_to(get_string(0x2F5), OBJ_HOMEX(ThePlayer), OBJ_HOMEY(ThePlayer), 0,
                          OBJ_HOMEX(CharacterThatTriggeredTrap),
                          OBJ_HOMEY(CharacterThatTriggeredTrap), 0, 0);
            result = whack_thing(Obj_MemTPtr(CharacterThatTriggeredTrap),
                                 damage, 4, 0);
        }
        continue_chain = 0;
        break;
    case 0x10: {
        char far *str = get_string(trap->qn.f.quality << 5 | trap->ol.f.owner | STR_TRAPTEXT);
        dprintf("Look, it's a text trap\n");
        if (str != 0)
            scroll_print(str);
        break;
    }
    }
    if (continue_chain && trap->ol.f.link > 0) {
        struct Object far *next = Obj_PtrTMem(&trap->ol.link);
        if (OBJ_MAJOR(next) == MAJOR_TRAP) {
            if (OBJ_MINOR(next) < 2)
                result |= UseTrap(next, x, y);
            else
                result |= UseTrigger(CharacterThatTriggeredTrap,
                                     TriggeringButton, next, -1);
        }
    }
    return result;
}
/* Removes from the list head, and recursively from containers, every trigger (class
   0x1A, MAJOR_TRAP minor 2) whose link is RemoveTrapIndex, counting RemoveTrapFlags
   down. */
void far kill_triggers(union Link far *head)
{
    struct Object far *obj;
    union Link far *link;
    obj = Obj_PtrTMem(head);
    while (obj != 0) {
        if (OBJ_CLASS(obj) == 0x1A &&
            obj->ol.f.link == RemoveTrapIndex) {
            Obj_Rem(head, obj);
            Obj_Free(obj);
            obj->ol.word &= 0x3F;
            RemoveTrapFlags--;
        }
        if (!OBJ_ISQUANT(obj)) {
            if ((link = &obj->ol.link)->f.index != 0)
                kill_triggers(link);
        }
        obj = Obj_PtrTMem(&obj->qn.link);
    }
}
/* Deletes trap and its chain. Its flags count the triggers pointing at it, so the whole
   map is searched to remove them too. */
void far delete_trap(union Link far *head, struct Object far *trap)
{
    struct Tile far *tile;
    union Link far *tilehead;
    register unsigned i;
    RemoveTrapFlags = OBJ_FLAGS(trap);
    if (RemoveTrapFlags != 0) {
        RemoveTrapIndex = Obj_MemTPtr(trap);
        tile = mapdata;
        for (i = 0; RemoveTrapFlags > 0 && i < 0x1000; i++, tile++) {
            tilehead = &tile->objects;
            if (tilehead->f.index > 0)
                kill_triggers(tilehead);
        }
    }
    if ((trap = Obj_Find(head, 1, Obj_MemTPtr(trap))) != 0)
        Obj_FreeLinkChain(Obj_Find_Head, trap);
}
/* gronk_whoami callback (do_trap_hack 0x32): the critter talks to the player; one that
   was waiting (goal 7) goes home (goal 1) afterwards. */
/* name: chosen for the stub order (tools/bssorder.py key 783, between SetOffTrap's 731
   and is_this_wandering's 793); no original name, UW2 having no such function. */
char far gronkify_talkto_player(struct Object far *npc, NEARPTR arg)
{
    if (OBJ_GOAL(npc) == 7)
        SET_GOAL(npc, 1);
    mouse_freereign();
    TalkTo(npc);
    return 0;
}
/* The hack traps (trap type 3), each a special case of the game, chosen by the trap's
   quality: 2 the crystal ball, 3 and 4 the eight-position switch, 5 a crime against
   owner, 0x18 the bullfrog puzzle, 0x28 the emerald puzzle, 0x29 the exploding book, 0x2A
   the talking door, 0x32 makes waiting critters with conversation 0xD8 talk to the
   player, 0x39 Arial's scene, 0x3C..0x3E an earthquake (type quality - 0x3B, strength
   owner) when the player set it off, 0x3F the end of the game (EndGameMode = owner + 1).
   Most handlers are in WORLDEV.C. Returns 2. */
int far do_trap_hack(struct Object far *trap, register int x, register int y)
{
    switch (trap->qn.f.quality) {
    case 2:
        crystal_ball(trap, x, y);
        break;
    case 3:
    case 4:
        eight_pos_switch(OBJ_FLAGS(TriggeringButton), trap, x, y);
        break;
    case 5:
        player_did_bad(trap->ol.f.owner);
        break;
    case 0x18:
        work_bullfrog_tiles(trap->ol.f.owner, x, y);
        break;
    case 0x28:
        emerald_trap(trap, x, y);
        break;
    case 0x29:
        ExplodingBook_ovr107_1259(trap, x, y);
        break;
    case 0x2A:
        talking_door_trap(trap, x, y);
        break;
    case 0x32:
        gronk_whoami(0xD8, 0, 0, (WhoamiFn)gronkify_talkto_player);
        break;
    case 0x39:
        do_arial_talking(trap, x, y);
        break;
    case 0x3C:
    case 0x3D:
    case 0x3E:
        if (CharacterThatTriggeredTrap == ThePlayer)
            QuakeTrap_seg008_DE7(trap->qn.f.quality - 0x3B, trap->ol.f.owner);
        break;
    case 0x3F:
        EndGameMode_dseg_1C8F = trap->ol.f.owner + 1;
        editchng(0x400);
        break;
    }
    return 2;
}
/* Builds a move trigger (minor 2, mode 0) and a trap of type sub at square x, y, the
   trigger aimed at its own square: a trap laid by a spell. Returns the trigger's index. */
int far cast_trap_spell(int x, int y, int sub)
{
    struct Object far *trigger;
    struct Object far *trap;
    struct Tile far *tile;
    trigger = Obj_Alloc(0);
    if (trigger == 0) return 0;
    trap = Obj_Alloc(0);
    if (trap == 0) {
        Obj_Free(trigger);
        return 0;
    }
    tile = Map_GetAddr(x, y);
    SET_INVIS(trigger, 1);
    SET_DOORDIR(trigger, 1);
    SET_MAJOR(trigger, 6);
    SET_MINOR(trigger, 2);
    SET_INCLASS(trigger, 0);
    SET_Z(trigger, tile->height << 3);
    SET_ISQUANT(trigger, 1);
    SET_FINEX(trigger, 3);
    SET_FINEY(trigger, 3);
    SET_HEADING(trigger, 0);
    SET_FLAG11(trigger, 0);
    SET_ENCHANTED(trigger, 1);
    SET_FLAG10(trigger, 0);
    trigger->qn.f.next = trigger->qn.f.quality = 0;
    trigger->ol.f.link = Obj_MemTPtr(trap);
    trigger->qn.f.quality = x;
    trigger->ol.f.owner = y;
    Obj_Add(&tile->objects, trigger);
    SET_MAJOR(trap, 6);
    SET_MINOR(trap, 0);
    SET_INCLASS(trap, sub);
    SET_INVIS(trap, 1);
    SET_DOORDIR(trap, 1);
    SET_Z(trap, tile->height << 3);
    SET_ISQUANT(trap, 1);
    SET_FINEX(trap, 3);
    SET_FINEY(trap, 3);
    trap->qn.f.next = 0;
    trap->ol.f.link = 0;
    trap->qn.f.quality = 0x3F;
    SET_FLAGS(trap, 1);
    Obj_Add(&tile->objects, trap);
    return Obj_MemTPtr(trigger);
}
/* Deletes trigger obj: the last trigger of a trap deletes the trap too, otherwise the
   trap's trigger count drops. */
void far trigger_obj_del(union Link far *head, struct Object far *obj)
{
    struct Object far *linked;
    int x;
    register int flags;
    register int y;
    linked = Obj_PtrTMem(&obj->ol.link);
    flags = OBJ_FLAGS(linked);
    if (flags == 1) {
        x = obj->qn.f.quality;
        y = obj->ol.f.owner;
        delete_trap(&Map_GetAddr(x, y)->objects, linked);
    } else {
        SET_FLAGS(linked, flags - 1);
        Obj_Rem(head, obj);
        Obj_Free(obj);
    }
}
/* Deletes a trap or trigger object. */
void far trap_obj_del(union Link far *head, struct Object far *obj)
{
    if (OBJ_MINOR(obj) > 1)
        trigger_obj_del(head, obj);
    else
        delete_trap(head, obj);
}
/* gronk_area callback: notes a temporary (wandering, created) object near
   ObjRunCodeAround, other than the player. */
char far is_this_wandering(int x, int y, struct Object far *obj)
{
    struct Object far *p = obj;
    if (OBJ_TEMP(p) &&
        obj != ObjRunCodeAround &&
        p != ThePlayer)
        CreatedObjectFound_dseg_5c99_1D9A = 1;
    return CreatedObjectFound_dseg_5c99_1D9A;
}
/* True if a created (temporary) object is already within four squares of obj, so a
   create-object trap should not add another. */
char far dont_create_wandering_monster_here(struct Object far *obj)
{
    CreatedObjectFound_dseg_5c99_1D9A = 0;
    ObjRunCodeAround = obj;
    gronk_area(obj, 1, (SpellFn)is_this_wandering, 0, 0, 4);
    return CreatedObjectFound_dseg_5c99_1D9A;
}
/* For the wandering monster and door updates: 0 when the player is within 8 squares of
   x, y, so the change would be seen; 1 otherwise, or when is_player is clear. */
char far check_alert(char is_player, int x, int y)
{
    int px, py;
    if (!is_player) return 1;
    px = OBJ_HOMEX(ThePlayer);
    py = OBJ_HOMEY(ThePlayer);
    if (abs(px - x) < 8 && abs(py - y) < 8)
        return 0;
    return 1;
}
/* Runs every create-object trap on the level (MAJOR_TRAP minor 0 class 7, flags 0) whose
   template is a mobile, out of the player's sight, marking the template temporary. */
void far DoWanderingMonsters(char is_player)
{
    int16 x = 0;
    int16 y = 0;
    struct Object far *obj;
    struct Object far *newobj;
    while ((obj = Obj_FindInMap(6, 0, 7, &x, &y)) != 0) {
        if (OBJ_FLAGS(obj) == 0) {
            newobj = Obj_PtrTMem(&obj->ol.link);
            if (IsMobElem(newobj)) {
                SET_TEMP(newobj, 1);
                if (check_alert(is_player, x, y))
                    UseTrap(obj, x, y);
            }
        }
        x++;
    }
}
/* Each open door on the level (not on a tile with door bit 1) closes with chance 3 in 10
   where the player cannot see it, as if someone shut it; with is_player clear the moving
   doors are also run to the end. */
void far DoClosingDoors(char is_player)
{
    int16 x = 0;
    int16 y = 0;
    struct Object far *door;
    register int i;
    while ((door = Obj_FindInMap(5, 0, -1, &x, &y)) != 0) {
        if (((Map_GetAddr(x, y)->door & 2) >> 1) == 0 &&
            OBJ_INCLASS(door) >= 8 &&
            (int)(((int32)rand() * 10) / 0x8000L) < 3) {
            MapObj_X = x;
            MapObj_Y = y;
            if (check_alert(is_player, x, y))
                CloseDoor(door);
        }
        x++;
    }
    if (!is_player && DoAnimO) {
        for (i = 0; i < 8; i++)
            update_animobj(1);
    }
}
