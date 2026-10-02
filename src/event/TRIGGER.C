/* target: ovr166 */
/* opts: -mm -1 -G -O -Y -d */
/* Triggers and traps: running a trigger's trap chain, the per-type trap actions (UseTrap)
   and the special-purpose "hack" traps, quest and variable traps, removing triggers and
   traps, wandering monsters, closing doors, pressure plates and bridges. The whole of DOS
   overlay ovr166, in original order.

   What it does in the game: triggers and traps are MAJOR_TRAP objects (items 0x180..0x1BF;
   minor classes 0 and 1 traps, 2 and 3 triggers). A trigger sits on a tile or inside an
   object; its quality and owner fields name a target square, and its link points at a
   trap there. The game reports an action with UseTrigger(who, object, trigger, kind), where
   kind is a trigger mode from Triggers[] (OBJECTS.DAT's trigger type table; the Guide's
   "Trigger Type Table": 0 move, 2 pick up, 4 use, 5 look, 6 enter, 7 pressure, 8 open, 9
   close, 0xA timer, 0xB unlock, 0xC scheduled, 0xE exit, 0xF pressure release). A trigger of
   the matching mode runs its trap (SetOffTrap, UseTrap), and each trap passes on to the
   object its link names, so traps form chains; condition traps branch. A trigger without
   ID_FLAG10 is used up: its trap chain is deleted after it runs.

   Callers: using, looking at and picking up objects (OBJUSE.C, USEITEMS.C, INTERACT.C), doors
   (EFFECT.C, USEITEMS.C), movement and pressure plates (the motion code, check_pplate),
   the timer list (EFFECT.C), schedules (SCDEVENT.C) and spells (SPELLS2.C).

   Data owned: Triggers[16] (the trigger modes, read by trap_init), the trigger that is
   running (CharacterThatTriggeredTrap, TriggeringButton), the removal state, tile_walls,
   trap_teleport_data (the facing a teleport trap gives) and the pressure plate's tile.
   Function and global names are the originals from the FM Towns symbol table where it has
   them.
   Name: inferred (triggers and traps: UseTrigger, SetOffTrap, UseTrap; the job of System
   Shock's TRIGGER.C). */
#include <stdlib.h>
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

/* This file's _BSS, DS:863C..865B: the trigger modes, the trap being removed, and the
   trigger that is running. */
/* match: ovr163's LootCreature ends at 863B; seg045's starts at 865C. Laid out by name
   (tools/bssorder.py): RemoveTrapIndex 58, RemoveTrapFlags 74, ObjRunCodeAround 447,
   CharacterThatTriggeredTrap 459, TriggeringButton 460, Triggers 996. */
/* name: only Triggers has an FM Towns name (ovr162 uses it too); the others only this
   file uses, so they are static, their provisional names chosen for their keys. */
unsigned char Triggers[16];
static int RemoveTrapIndex;                             /* DS:863C */
static int RemoveTrapFlags;                             /* DS:863E */
static struct Object far *ObjRunCodeAround;             /* DS:8640 */
static struct Object far *CharacterThatTriggeredTrap;   /* DS:8644 */
static struct Object far *TriggeringButton;             /* DS:8648 */
void far fread(void near *dest, int count, int size, int handle);

/* Reads the 16-byte trigger type table from OBJECTS.DAT (handle positioned by the caller). */
/* name: IDA: LoadTriggerObjDat. FM Towns' trap_init, first in this run of functions: the
   same fread of 16 bytes into the trigger type table. */
void far trap_init(int handle)
{
    fread(Triggers, 1, 16, handle);
}

/* The class data of ActiveObj: a trigger's entry in Triggers[], none for a trap. */
/* name: IDA: MajorClass6TriggerType. FM Towns' trap_class_data: the same minor-class test
   on ActiveObj and the same index into the trigger table. */
unsigned char near * far trap_class_data(void)
{
    if (OBJ_MINOR(ActiveObj) & 2)
        return Triggers + OBJ_INCLASS(ActiveObj);
    return 0;
}

/* Fires trigger trig for an action of kind type by who on object start (type -1: reached
   along a trap chain, no checks). The trigger must be a trigger (minor class 2 or 3) of the
   mode type. A switch that is already on (class SWITCH, minor index above 7) passes the action
   to the next trigger in its list. Who may set it off: the player only if ID_FLAG11 is set
   (and a look trigger above floor level needs a Search skill check against its height),
   critters only with ID_ENCHANT set and not on a MAJOR_RECT trigger, other objects only with
   ID_FLAG9. The trap chain runs at the trigger's target square; a trigger without ID_FLAG10
   then deletes the chain (result | 0x20). A pressure trigger updates the plate afterwards.
   Returns 2 when nothing ran. */
int far UseTrigger(struct Object far *who, struct Object far *start,
                   struct Object far *trig, register int type)
{
    struct Object far *trap;
    int quality;
    int owner;
    int sub;
    int minor;
    register int result;
    minor = OBJ_MINOR(trig);
    sub = trig->id & ID_INCLASS;
    if (!(minor & 2)) return 2;
    if (type >= 0) {
        if (start != 0 && OBJ_CLASS(start) == CLASS_SWITCH &&
            OBJ_INCLASS(start) > 7 && trig->qn.f.next != 0) {
            trig = Obj_PtrTMem(&trig->qn.link);
            return UseTrigger(who, start, trig, type);
        }
        if (Triggers[sub] != type) return 2;
        if (OBJ_ITEM(who) == ITEM_ADVENTURER) {
            if (!(trig->id & ID_FLAG11)) return 2;
            if (type == 5 && OBJ_Z(trig) > 0 &&
                skill_check(player->skills[SKILL_SEARCH], trig->pos & POS_Z) <= 0)
                return 2;
        } else {
            if (OBJ_MAJOR(who) == MAJOR_CREATURE) {
                if (!(trig->id & ID_ENCHANT) ||
                    OBJ_MAJOR(trig) == MAJOR_RECT) return 2;
            }
            if (OBJ_MAJOR(who) != MAJOR_CREATURE && !(trig->id & ID_FLAG9))
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
    if ((type & 0xF07) == 7)
        update_pplate(trig);
    return result;
}
int far UseTrap(struct Object far *trap, int x, int y);
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
}
/* The variable traps' operations: 0 add, 1 subtract, 2 set, 3 and, 4 or, 5 xor, 6 shift
   left, 7 count up while equal (value + 1 if value == right, else 0). */
/* name: IDA: PerformVariableOperation. FM Towns' do_math_op, between SetOffTrap and
   set_numbered_variable: the same eight operations. */
int far do_math_op(int value, int op, int right)
{
    switch (op) {
    case 0: value += right; break;
    case 1: value -= right; break;
    case 2: value = right; break;
    case 3: value &= right; break;
    case 4: value |= right; break;
    case 5: value ^= right; break;
    case 6: value <<= right; break;
    case 7: if (value == right) value++; else value = 0; break;
    }
    return value;
}
/* The variables traps, schedules and conversations share, by number: 0..0xFF the player's
   vars[] (bytes), 0x100..0x17F a bit each, bit (n & 3) of quests[n / 4] (op 5 toggles it,
   op 1 leaves it, others set it to right > 0), 0x180..0x18F the quest bytes (quest 128 on),
   0x190..0x19F the X clocks. */
void far set_numbered_variable(int left, unsigned char op, register int right)
{
    if (left < 0x100) {
        register int value = player->vars[left];
        player->vars[left] = (unsigned char)do_math_op(value, op, right) & 0x3FF;
    } else {
        left -= 0x100;
        if (left < 0x80) {
            switch (op) {
            case 5:
                player->quests[(unsigned)left >> 2] =
                    (player->quests[(unsigned)left >> 2] & ~(1 << (left & 3))) +
                    (((int)((player->quests[(unsigned)left >> 2] & (1 << (left & 3))) >> (left & 3)) ^ 1) << (left & 3));
                break;
            case 1:
                player->quests[(unsigned)left >> 2] =
                    (player->quests[(unsigned)left >> 2] & ~(1 << (left & 3))) +
                    (((int)((player->quests[(unsigned)left >> 2] & (1 << (left & 3))) >> (left & 3)) & 1) << (left & 3));
                break;
            default:
                player->quests[(unsigned)left >> 2] =
                    (player->quests[(unsigned)left >> 2] & ~(1 << (left & 3))) +
                    ((right > 0 ? 1 : 0) << (left & 3));
                break;
            }
        } else {
            left -= 0x80;
            if (left < 0x10) {
                register int value = player->quest_bytes[left];
                player->quest_bytes[left] = do_math_op(value, op, right);
            } else {
                left -= 0x10;
                if (left < 0x10) {
                    register int value = player->xclock[left];
                    player->xclock[left] = do_math_op(value, op, right);
                }
            }
        }
    }
}
/* Reads a numbered variable (see set_numbered_variable); 0 beyond 0x19F. */
int far get_numbered_variable(int index)
{
    if (index < 0x100) return player->vars[index];
    index -= 0x100;
    if (index < 0x80)
        return (int)((player->quests[(unsigned)index >> 2] & (long)(1 << (index & 3))) >> (index & 3));
    index -= 0x80;
    if (index < 0x10) return player->quest_bytes[index];
    index -= 0x10;
    if (index < 0x10) return player->xclock[index];
    return 0;
}
extern unsigned char stay_centered;
/* This file's _DATA, DS:1BA6..1BBC, in definition order. */
/* match: ovr158's strings end at 1BA5 (odd), ovr167's data starts at 1BBE. This file uses
   three of the four; FM Towns keeps tile_walls and map_sq (TriggerChainTileData here)
   together, the same 16 bytes then the pointer, and moves the small scalars between them
   elsewhere, as it does all of them. */
unsigned char tile_walls[16] = { 30, 0, 19, 21, 11, 13, 32, 32, 32, 32, 0, 0, 0, 0, 0, 30 };
int trap_teleport_data = -1;                                    /* DS:1BB6 */
unsigned char CreatedObjectFound_dseg_67d6_1BB8 = 0;            /* DS:1BB8 */
struct Tile far *TriggerChainTileData_dseg_67d6_1BB9 = 0;       /* DS:1BB9, FM Towns map_sq */
int far do_teleport(struct Object far *who, int x, int y, int level);
int far change_terrain(int x, int y, int wall, int floor, int height,
                              int type, int dx, int dy, int extra);
unsigned char far put_at(int x, int y, int z, struct Object far *obj, int range, int nocull);
int far add_animobj(int index, int len, int a, char x, char y);
void far trap_obj_del(union Link far *head, struct Object far *obj);
void far scroll_print(char far *s);

/* Condition traps (variable, skill, proximity, inventory) branch by running the trap or
   trigger that follows the first object the trap links to, and return its result. */
#define RUN_ELSE_CHAIN()                                                            \
    obj = Obj_PtrTMem(&trap->ol.link);                                              \
    if (obj->qn.f.next > 0)                                                         \
        return (OBJ_MINOR(Obj_PtrTMem(&obj->qn.link)) & 2)                          \
            ? UseTrigger(CharacterThatTriggeredTrap,                 \
                         TriggeringButton,                           \
                         Obj_PtrTMem(&obj->qn.link), -1)                            \
            : SetOffTrap(CharacterThatTriggeredTrap,                 \
                         TriggeringButton,                           \
                         Obj_PtrTMem(&obj->qn.link), x, y);                         \
    return 2

/* Performs one trap at square x, y, then, unless the trap ends the chain, passes on to the
   trap or trigger its link names. Returns 2, or the result of the chain. By trap type
   (items.h's TRAP_*; parameters in the trap's quality, owner, z, heading and fine position):
   - ARROW fires the trap's missile (MISSILE.C); SPECIAL_EFFECT plays an effect.
   - TELEPORT moves who to square (quality, owner) of level z (0: this level, inferred); fine x bits
     choose the facing and whether the player loses the level's map.
   - CHANGE_TERRAIN, OSCILLATOR (steps a tile's height, floor or wall by one between quality
     and owner, reversing at the ends), PIT (opens or fills a pit) and BRIDGE (lays or removes
     a row of bridges) change the map.
   - SET_VARIABLE and CHECK_VARIABLE use the numbered variables; SET_VARIABLE with fine y
     set also counts XC_CHANGED.
   - Condition traps branch (RUN_ELSE_CHAIN): CHECK_VARIABLE when the variables do not
     match, SKILL when the check passes (quality 0..2 an attribute, 3 the value 15, 4 and up
     a skill; owner * 3 the difficulty; heading 1 a skill_check, else a plain comparison),
     PROXIMITY when who is outside the rectangle (quality, owner) from x, y or on the wrong
     side of the trap's height, INVENTORY when the player lacks the item (quality * 32 +
     owner; fine x: it must be worn; z: at least that many).
   - EXPERIENCE gives (quality * 8 + (owner & 7) - 256) << (owner >> 3) experience, which
     can be negative; DAMAGE hits who for quality (heals when owner is set).
   - CREATE_OBJECT copies its linked object onto the square with chance (63 - quality) in 63,
     not when another created object is within four squares, and never in the Tombs (world
     6) once bit 3 of quest 1 is set. A linked "adventurer" (0x7F) is a template for a random
     monster scaled by the level within the world, the castle plot (XC_CASTLE) and the
     player's level; it is made hostile, temporary and homed there.
   - DOOR opens (1), closes (2) or toggles (3) the door on the square, and with owner set
     replaces its lock by a copy of the trap's linked lock.
   - DELETE_OBJECT removes its linked object from square (quality, owner).
   - WARD (a rune of warding) hits the critter that steps on it, of class quality (0x3F any),
     for 3 + rand * Casting, and tells the player.
   - TEXT_STRING prints string quality * 32 + owner of block 9 (if owner's high bit is set,
     only while player->b62_5 is set).
   - JUMP, CHANGE_FROM, SPELL (inanimate_spell) and HACK (do_trap_hack) hand on to other
     files. */
int far UseTrap(struct Object far *trap, int x, int y)
{
    struct Tile far *tile;
    struct Object far *linked;
    struct Object far *obj;
    struct Object far *lock;
    union Link far *head;
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    unsigned char mode;
    int spell;
    int power;
    int result = 2;
    unsigned char continue_chain = 1;
    int slot;
    register int i;
    register int v;

    switch (OBJ_INMAJOR(trap)) {
    case TRAP_ARROW:
        trap_fire(trap, x, y);
        break;
    case TRAP_SPECIAL_EFFECT:
        do_sfx(trap->qn.f.quality, trap->ol.f.owner);
        break;
    case TRAP_TELEPORT:
        v = trap->qn.f.quality;
        a = trap->ol.f.owner;
        b = OBJ_Z(trap);
        trap_teleport_data = OBJ_FINEX(trap) & 1;
        if ((OBJ_FINEX(trap) & 2) >> 1)
            trap_teleport_data = trap_teleport_data | (OBJ_HEADING(trap) + 8) << 2;
        if (CharacterThatTriggeredTrap == ThePlayer &&
            ((OBJ_FINEX(trap) & 4) >> 2) == 1) {
            if (player->automap)
                player->map_scrap = PlayerLevel;
            player->automap = 0;
        }
        result = do_teleport(CharacterThatTriggeredTrap, v, a, b);
        break;
    case TRAP_CHANGE_TERRAIN:
        d = trap->ol.f.owner;
        c = trap->qn.f.quality >> 1;
        b = OBJ_Z(trap) >> 3;
        i = (OBJ_HEADING(trap) << 1) + (trap->qn.f.quality & 1);
        if (i == 15)
            i = 10;
        e = OBJ_FINEX(trap);
        f = OBJ_FINEY(trap);
        result = change_terrain(x, y, d, c, b, i, e, f, 0);
        break;
    case TRAP_JUMP:
        set_jmp(trap->qn.f.quality, trap->ol.f.owner, OBJ_HEADING(trap));
        break;
    case TRAP_SET_VARIABLE:
        i = OBJ_Z(trap) + (OBJ_FINEX(trap) << 7);
        c = (trap->qn.f.quality << 6) + trap->ol.f.owner;
        b = OBJ_HEADING(trap);
        set_numbered_variable(i, b, c);
        if (OBJ_FINEY(trap))
            player->xclock[XC_CHANGED]++;
        break;
    case TRAP_CHECK_VARIABLE:
        i = OBJ_Z(trap) | (OBJ_FINEX(trap) & 3) << 7;
        b = i + OBJ_HEADING(trap);
        c = (trap->qn.f.quality << 5 | trap->ol.f.owner) << 3 | OBJ_FINEY(trap);
        for (v = 0; i <= b; i++) {
            if (OBJ_FINEX(trap) >> 2)
                v += get_numbered_variable(i);
            else {
                v <<= 3;
                v |= get_numbered_variable(i) & 7;
            }
        }
        if (v != c) {
            if (trap->ol.f.link > 0) {
                RUN_ELSE_CHAIN();
            }
        }
        break;
    case TRAP_SKILL: {
        int which;
        int value;
        int difficulty;
        unsigned char failed;
        which = trap->qn.f.quality;
        if (which < 3)
            value = playerdat->attr[which];
        else if (which == 3)
            value = 15;
        else
            value = player->skills[which - 4];
        difficulty = trap->ol.f.owner * 3;
        if (OBJ_HEADING(trap) == 1)
            failed = skill_check(value, difficulty) <= 0;
        else
            failed = value < difficulty;
        if (!failed) {
            RUN_ELSE_CHAIN();
        }
        break;
    }
    case TRAP_PROXIMITY:
        if (OBJ_HOMEX(CharacterThatTriggeredTrap) >= x &&
            OBJ_HOMEY(CharacterThatTriggeredTrap) >= y &&
            OBJ_HOMEX(CharacterThatTriggeredTrap) - x <= trap->qn.f.quality &&
            OBJ_HOMEY(CharacterThatTriggeredTrap) - y <= trap->ol.f.owner &&
            (OBJ_FINEX(trap) ||
             OBJ_Z(CharacterThatTriggeredTrap) <= OBJ_Z(trap)) &&
            (OBJ_FINEY(trap) ||
             OBJ_Z(CharacterThatTriggeredTrap) > OBJ_Z(trap)))
            break;
        RUN_ELSE_CHAIN();
    case TRAP_CHANGE_FROM:
        obj = Obj_PtrTMem(&trap->ol.link);
        do_change_grokking(trap, obj, x, y);
        break;
    case TRAP_OSCILLATOR: {
        int floor;
        int wall;
        int height;
        int limit = 0x3F;
        int now;
        int type = 0xF;
        tile = Map_GetAddr(x, y);
        mode = (OBJ_FINEX(trap) & 6) >> 1;
        floor = 0xF;
        wall = 0x3F;
        height = 0x3F;
        i = OBJ_FINEX(trap) & 1;
        if (i == 0)
            i--;
        switch (mode) {
        case 0:
            g = tile->height;
            limit = 0xF;
            now = height = g + i;
            if (tile->type == TILE_SOLID && i < 0) {
                type = TILE_OPEN;
                height = 0xF;
            } else if (tile->type == TILE_OPEN && height == 0x10)
                type = TILE_SOLID;
            if (trap->ol.f.owner == 0x10)
                limit = 0x3F;
            break;
        case 1:
            g = tile->floor;
            now = floor = g + i;
            break;
        case 2:
            g = TILE_WALL(tile);
            now = wall = g + i;
            break;
        }
        if (trap->qn.f.quality <= now && trap->ol.f.owner >= now)
            change_terrain(x, y, wall, floor, height, type, 0, 0, 4);
        if (i == 1) {
            if (trap->ol.f.owner == now && trap->qn.f.quality < limit)
                SET_FINEX(trap, OBJ_FINEX(trap) & 6);
        } else if (trap->qn.f.quality == now && trap->ol.f.owner < limit) {
            SET_FINEX(trap, (OBJ_FINEX(trap) & 6) + 1);
            if (PlayerLevel == 0x44)
                check_for_sunken_moongate();
        }
        break;
    }
    case TRAP_PIT:
        tile = Map_GetAddr(x, y);
        g = tile->type;
        i = tile->height;
        if (i == 0) {
            i = OBJ_FINEX(trap) + ((OBJ_FINEY(trap) & 7) << 3);
            b = trap->ol.f.owner;
        } else {
            i = 0;
            b = trap->qn.f.quality;
        }
        if (i > 15)
            g = 0;
        else if (g != 1)
            g = 1;
        else
            g = 0x3F;
        change_terrain(x, y, 0x3F, b, i, g, 0, 0, 4);
        break;
    case TRAP_BRIDGE: {
        int bx = x;
        int by = y;
        int n;
        int dx = 0;
        int dy = 0;
        struct Object far *bridge;
        SET_HEADING(trap, OBJ_HEADING(trap) & 6);
        n = (OBJ_HEADING(trap) < 4 ? 1 : 0) * 2 - 1;
        if (OBJ_HEADING(trap) & 2)
            dx = n;
        else
            dy = n;
        for (n = trap->qn.f.quality; n > 0; bx += dx, by += dy, n--) {
            switch (trap->ol.f.owner >> 4) {
            case 0:
                break;
            case 1:
                bridge = place_bridge(bx, by, OBJ_Z(trap), OBJ_HEADING(trap));
                break;
            case 2:
                destroy_bridge(bx, by, OBJ_Z(trap), OBJ_HEADING(trap));
                bridge = 0;
                break;
            default:
                bridge = 0;
            }
            if (bridge)
                SET_FLAGS(bridge, trap->ol.f.owner);
        }
        break;
    }
    case TRAP_EXPERIENCE:
        g = ((trap->qn.f.quality << 3) + (trap->ol.f.owner & 7) - 0x100) *
            (1 << (trap->ol.f.owner >> 3));
        player_get_exp(g);
        break;
    case TRAP_DAMAGE:
        if ((int)(((long)rand() * 10) / 0x8000L) < 7)
            g = 2;
        else
            g = 0;
        result = whack_thing(Obj_MemTPtr(CharacterThatTriggeredTrap),
                             trap->qn.f.quality * (trap->ol.f.owner ? -1 : 1), 4, g);
        break;
    case TRAP_HACK:
        result = do_trap_hack(trap, x, y);
        break;
    case TRAP_SPELL:
        spell = trap->qn.f.quality;
        power = trap->ol.f.owner;
        result = inanimate_spell(x, y, trap, CharacterThatTriggeredTrap,
                                 spell, power);
        break;
    case TRAP_CREATE_OBJECT: {
        unsigned char random_critter = 0;
        unsigned char placed;
        if ((int)(((long)rand() * 0x3F) / 0x8000L) < trap->qn.f.quality)
            return 2;
        continue_chain = 0;
        if OBJ_ISQUANT(trap)
            break;
        linked = Obj_PtrTMem(&trap->ol.link);
        if (linked == 0)
            break;
        if (OBJ_MAJOR(linked) == MAJOR_CREATURE && dont_create_wandering_monster_here(linked))
            break;
        if ((int)((player->quests[1] & 8) >> 3) && (PlayerLevel - 1) / 8 == 6)
            break;
        if (OBJ_ITEM(linked) == ITEM_ADVENTURER) {
            int lo;
            int range;
            int item;
            lo = (((PlayerLevel - 1) % 8 + 1) * 3 + player->xclock[XC_CASTLE]) / 9;
            range = player->level + player->xclock[XC_CASTLE] + 1;
            if (range > 16)
                range = 16;
            if (lo < 1)
                lo = 1;
            item = (rand() % lo << 4) + rand() % range + FIRST_CREATURE;
            while (!eligible_castle_monster(item))
                item = (rand() % lo << 4) + rand() % range + FIRST_CREATURE;
            random_critter = 1;
            SET_ITEM(linked, item);
            init_this_critter(linked);
            SET_LONER(linked, 1);
        }
        obj = Obj_Alloc(IsMobElem(linked));
        if (obj != 0) {
            if (IsMobElem(linked))
                *(struct Object far *)obj = *(struct Object far *)linked;
            else
                *(struct StaticObj far *)obj = *(struct StaticObj far *)linked;
            if (random_critter) {
                SET_ITEM(linked, ITEM_ADVENTURER);
                SET_ATTITUDE(obj, 0);
                SET_TEMP(obj, 1);
                obj->qn.f.quality = OBJ_HOMEX(obj);
                obj->ol.f.owner = OBJ_HOMEY(obj);
            }
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
    case TRAP_DOOR: {
        int savex;
        int savey;
        tile = Map_GetAddr(x, y);
        head = &tile->objects;
        linked = Obj_InList(&head, 0, MAJOR_RECT, 0, -1);
        savex = MapObj_X;
        savey = MapObj_Y;
        MapObj_X = x;
        MapObj_Y = y;
        if (linked != 0) {
            switch (trap->qn.f.quality) {
            case 1:
                OpenDoor(CharacterThatTriggeredTrap, linked);
                break;
            case 2:
                CloseDoor(CharacterThatTriggeredTrap, linked);
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
                    CloseDoor(CharacterThatTriggeredTrap, linked);
                }
            } else {
                switch (trap->qn.f.quality) {
                case 1:
                case 3:
                    OpenDoor(CharacterThatTriggeredTrap, linked);
                }
            }
        }
        MapObj_X = savex;
        MapObj_Y = savey;
        if (linked != 0 && trap->ol.f.owner != 0) {
            head = &linked->ol.link;
            lock = Obj_InList(&head, 0, MAJOR_SPEC, 0, 0xF);
            continue_chain = 0;
            if (lock != 0 && Obj_Rem(head, lock))
                Obj_Free(lock);
            if (!OBJ_ISQUANT(trap) && trap->ol.f.link != 0) {
                lock = Obj_PtrTMem(&trap->ol.link);
                if ((obj = Obj_Alloc(0)) != 0) {
                    *(struct StaticObj far *)obj = *(struct StaticObj far *)lock;
                    Obj_Add(head, obj);
                }
            }
        }
        break;
    }
    case TRAP_DELETE_OBJECT:
        head = &Map_GetAddr(trap->qn.f.quality, trap->ol.f.owner)->objects;
        lock = Obj_PtrTMem(&trap->ol.link);
        if (OBJ_MAJOR(lock) == MAJOR_TRAP)
            trap_obj_del(head, lock);
        else if (OBJ_MAJOR(lock) == MAJOR_ANIMOBJ) {
            if (Obj_Rem(head, lock)) {
                rem_anim_from_list(Obj_MemTPtr(lock));
                Obj_Free(lock);
            }
        } else
            Obj_FreeLinkChain(head, lock);
        editchng(2);
        continue_chain = 0;
        break;
    case TRAP_INVENTORY: {
        unsigned char trip = 0;
        i = trap->qn.f.quality << 5 | trap->ol.f.owner;
        obj = FindObj(i >> 6, (i & 0x30) >> 4, i & 0xF, 4, &slot);
        if (obj == 0)
            trip = 1;
        else if (OBJ_FINEX(trap) && !ObjWorn(i, slot))
            trip = 1;
        else if (OBJ_Z(trap) > 0 && OBJ_ISQUANT(obj) &&
                 !(obj->ol.f.link & LINK_SPECIAL) && OBJ_Z(trap) > obj->ol.f.link)
            trip = 1;
        if (trip) {
            RUN_ELSE_CHAIN();
        }
        break;
    }
    case TRAP_WARD:
        if (CharacterThatTriggeredTrap != 0 &&
            (trap->qn.f.quality == 0x3F ||
             trap->qn.f.quality == OBJ_INCLASS(CharacterThatTriggeredTrap))) {
            int damage;
            damage = (int)(((long)rand() * player->skills[SKILL_CASTING]) / 0x8000L) + 3;
            print_path_to(get_string(0x304), OBJ_HOMEX(ThePlayer), OBJ_HOMEY(ThePlayer), 0,
            /* 'Your Rune of Warding has been set off ' */
                          OBJ_HOMEX(CharacterThatTriggeredTrap),
                          OBJ_HOMEY(CharacterThatTriggeredTrap), 0, 0);
            result = whack_thing(Obj_MemTPtr(CharacterThatTriggeredTrap),
                                 damage, 4, 0);
        }
        continue_chain = 0;
        break;
    case TRAP_TEXT_STRING: {
        char far *str = get_string(trap->qn.f.quality << 5 | trap->ol.f.owner & 0x1F | STR_TRAPTEXT);
        if (str != 0 && (!(trap->ol.f.owner >> 5) || player->b62_5))
            scroll_print(str);
        break;
    }
    }
    if (continue_chain && trap->ol.f.link > 0) {
        struct Object far *next = Obj_PtrTMem(&trap->ol.link);
        if (OBJ_MAJOR(next) == MAJOR_TRAP) {
            if (OBJ_MINOR(next) & 2)
                result |= UseTrigger(CharacterThatTriggeredTrap,
                                     TriggeringButton, next, -1);
            else
                result |= SetOffTrap(CharacterThatTriggeredTrap,
                                     TriggeringButton, next, x, y);
        }
    }
    return result;
}
/* Removes from the list head, and recursively from containers, every trigger whose link
   is RemoveTrapIndex (and its timer), counting RemoveTrapFlags down. */
void far kill_triggers(union Link far *head)
{
    struct Object far *obj;
    struct Object far *next;
    union Link far *link;
    obj = Obj_PtrTMem(head);
    while (obj != 0) {
        next = Obj_PtrTMem(&obj->qn.link);
        if (OBJ_MAJOR(obj) == MAJOR_TRAP &&
            OBJ_MINOR(obj) >= 2 &&
            obj->ol.f.link == RemoveTrapIndex) {
            if (Triggers[obj->id & ID_INCLASS] == 10)
                rem_timer_obj(Obj_MemTPtr(obj));
            if (Obj_Rem(head, obj)) Obj_Free(obj);
            obj->ol.word &= 0x3F;
            RemoveTrapFlags--;
        }
        if (!OBJ_ISQUANT(obj)) {
            if ((link = &obj->ol.link)->f.index != 0)
                kill_triggers(link);
        }
        obj = next;
    }
}
extern struct Tile far *mapdata;
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
void far Sched_SetAllClocks(int how);
void far gronkify_change_goal();
void far gronk_whoami(int who, int how, unsigned char near *info, void (far *fn)());
/* The hack traps (TRAP_HACK), each a special case of the game, chosen by the trap's
   quality: 2 the crystal ball, 3 and 4 the eight-position switch, 5 a crime against owner,
   10 the best weapon for the arena, 11 fraznium, 12 a standing wave, 14 a cycling floor, 17
   breaking ice, 18 flipping switches, 19 resetting arrow pillars, 20 toggling pillars, 21
   and 22 raising or lowering an object, 23 and 28 set the linked object's owner, 24
   graffiti, 25 skup_ductosnore, 26 a force field, 27 set the linked object's quality, 29
   the switch puzzle, 30 and 31 running and cheating in the Pits of Carnage arena, 32 qbert,
   33 redeeming bottles, 34 the courtyard, 35 recharging light bulbs, 36 the castle NPCs'
   moves, 37 bringing the schedules up to date, 38 spoiling cure potions, 39 showing or
   hiding the linked object, 40..42 vending machines, 43 changing a critter's goal, 44
   sleep, 45 hiding terrain, 54 and 55 the blackrock gem, 62 setting the goal of the critter
   that set it off. Most handlers are in WORLDEV.C. Returns 2 (55 returns its own result). */
int far do_trap_hack(struct Object far *trap, register int x, register int y)
{
    int owner = 0;
    unsigned char info[16];
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
    case 10:
        change_weapon_playerbest(x, y, trap->ol.f.owner);
        break;
    case 11:
        check_fraznium(x, y, trap->ol.f.owner);
        break;
    case 12:
        owner = trap->ol.f.owner;
        standing_wave(x, y, owner);
        trap->ol.f.owner = (owner + 1) & 0xF;
        break;
    case 14:
        owner = trap->ol.f.owner;
        cycle_floor(x, y, OBJ_FINEX(trap) * 2, OBJ_FINEY(trap) * 2,
                    trap->ol.f.owner, OBJ_Z(trap), OBJ_HEADING(trap));
        break;
    case 17:
        do_ice_hack(trap);
        break;
    case 18:
        switch_flip_hack(x, y, trap->ol.f.owner);
        break;
    case 19:
        reset_arrow_pillars(x, y, trap->ol.f.owner);
        break;
    case 20:
        owner = ((trap->ol.f.owner & 3) + 1) * 2;
        toggle_pillars_hack(x, y, owner, owner + 2 + ((trap->ol.f.owner & 0x1C) >> 1),
                            OBJ_FINEX(trap) | OBJ_FINEY(trap) << 3 |
                            OBJ_HEADING(trap) << 6 | OBJ_Z(trap) << 9);
        break;
    case 21:
        owner = 1;
    case 22:
        toggle_object_height(trap, owner);
        break;
    case 24:
        do_graffiti(x, y, trap->ol.f.owner);
        break;
    case 25:
        skup_ductosnore();
        break;
    case 26:
        find_and_gronk_force_field(x, y);
        break;
    case 27:
        Obj_IntTMem(trap->ol.f.link)->qn.f.quality = trap->ol.f.owner;
        editchng(2);
        break;
    case 23:
    case 28:
        Obj_IntTMem(trap->ol.f.link)->ol.f.owner = trap->ol.f.owner;
        editchng(2);
        break;
    case 29:
        play_with_switches(x, y);
        break;
    case 30:
        if (player->in_pits) {
            if (CharacterThatTriggeredTrap == ThePlayer)
                arena_player_runs();
            else
                arena_opponent_runs(CharacterThatTriggeredTrap);
        }
        break;
    case 31:
        if (player->in_pits)
            maybe_cheat_arena_fire();
        break;
    case 32:
        do_qbert(trap->ol.f.owner);
        break;
    case 33:
        redeem_all_bottles(x, y);
        break;
    case 34:
        courtyard_hacking(x, y, trap);
        break;
    case 35:
        recharge_lightbulbs(x, y);
        break;
    case 36:
        move_folks_around();
        break;
    case 37:
        Sched_SetAllClocks(1);
        break;
    case 38:
        ruin_cure_potions(x, y);
        break;
    case 39:
        SET_INVIS(Obj_IntTMem(trap->ol.f.link), trap->ol.f.owner);
        editchng(2);
        break;
    case 40:
    case 41:
    case 42:
        go_vend(trap->qn.f.quality, trap->ol.f.owner, x, y,
                OBJ_FLAGS(TriggeringButton));
        break;
    case 43:
        info[7] = trap->ol.f.owner;
        info[8] = OBJ_FINEY(trap) > 0 ? 1
                : Obj_MemTPtr(CharacterThatTriggeredTrap);
        gronk_whoami(((OBJ_HEADING(trap) > 0 ? 1 : 0) << 7) + OBJ_Z(trap),
                     OBJ_FINEX(trap) > 0 ? 1 : 0, info, gronkify_change_goal);
        break;
    case 44:
        player_sleep(trap->ol.f.owner);
        break;
    case 45:
        make_terrain_unseen(x, y, trap->ol.f.owner, trap->ol.f.owner);
        break;
    case 54:
        black_gem_rotate();
        break;
    case 55:
        return black_gem_trip();
    case 62:
        if (Obj_IntTMem(trap->ol.f.link) == CharacterThatTriggeredTrap ||
            trap->ol.f.link == 1)
            SET_GOAL(CharacterThatTriggeredTrap, trap->ol.f.owner);
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
    trigger->id = trigger->id & 0xBFFF | (1 & 1) << 14;
    trigger->id = trigger->id & 0xDFFF | (1 & 1) << 13;
    SET_MAJOR(trigger, 6);
    SET_MINOR(trigger, 2);
    SET_INCLASS(trigger, 0);
    SET_Z(trigger, tile->height << 3);
    trigger->id = trigger->id & 0x7FFF | (1 & 1) << 15;
    SET_FINEX(trigger, 3);
    SET_FINEY(trigger, 3);
    SET_HEADING(trigger, 0);
    trigger->id = trigger->id & 0xF7FF | (0 & 1) << 11;
    trigger->id = trigger->id & 0xEFFF | (1 & 1) << 12;
    trigger->id = trigger->id & 0xFDFF | (0 & 1) << 9;
    trigger->id = trigger->id & 0xFBFF | (0 & 1) << 10;
    trigger->qn.f.next = trigger->qn.f.quality = 0;
    trigger->ol.f.link = Obj_MemTPtr(trap);
    trigger->qn.f.quality = x;
    trigger->ol.f.owner = y;
    Obj_Add(&tile->objects, trigger);
    SET_MAJOR(trap, 6);
    SET_MINOR(trap, 0);
    SET_INCLASS(trap, sub);
    trap->id = trap->id & 0xBFFF | (1 & 1) << 14;
    trap->id = trap->id & 0xDFFF | (1 & 1) << 13;
    SET_Z(trap, tile->height << 3);
    trap->id = trap->id & 0x7FFF | (1 & 1) << 15;
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
    } else if (Obj_Rem(head, obj)) {
        linked->id = linked->id & 0xE1FF | (((flags - 1) & 0xF) << 9);
        Obj_Free(obj);
    }
    if (Triggers[obj->id & ID_INCLASS] == 10)
        rem_timer_obj(Obj_MemTPtr(obj));
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
/* name: FM Towns calls this callback is_this_wandering; its bit and exclusions match. */
char far is_this_wandering(int x, int y, struct Object far *obj)
{
    struct Object far *p = obj;
    if (OBJ_TEMP(p) &&
        obj != ObjRunCodeAround &&
        p != ThePlayer)
        CreatedObjectFound_dseg_67d6_1BB8 = 1;
    return CreatedObjectFound_dseg_67d6_1BB8;
}
typedef char (far *AreaFn)(int, int, struct Object far *);
void far gronk_area(struct Object far *who, char count, AreaFn fn,
                    unsigned char type, unsigned char dist, unsigned char radius);
/* True if a created (temporary) object is already within four squares of obj, so a
   create-object trap should not add another. */
char far dont_create_wandering_monster_here(struct Object far *obj)
{
    CreatedObjectFound_dseg_67d6_1BB8 = 0;
    ObjRunCodeAround = obj;
    gronk_area(obj, 1, is_this_wandering, 0, 0, 4);
    return CreatedObjectFound_dseg_67d6_1BB8;
}
/* For the wandering monster and door updates: 0 when the player is within 8 squares of
   x, y, so the change would be seen; 1 otherwise, or when is_player is clear. */
char far check_alert(unsigned char is_player, int x, int y)
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
   template is a mobile, out of the player's sight, marking the template temporary. Called
   when time passes for the level (inferred). */
void far DoWanderingMonsters(unsigned char is_player)
{
    int x = 0;
    int y = 0;
    struct Object far *obj;
    struct Object far *newobj;
    while ((obj = Obj_FindInMap(6, 0, 7, &x, &y)) != 0) {
        if (OBJ_FLAGS(obj) == 0) {
            newobj = Obj_PtrTMem(&obj->ol.link);
            if (IsMobElem(newobj)) {
                newobj->attitude_word = newobj->attitude_word & 0xFEFF | 0x100;
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
void far DoClosingDoors(unsigned char is_player)
{
    int x = 0;
    int y = 0;
    struct Object far *door;
    int i;
    quick_time = 1;
    while ((door = Obj_FindInMap(5, 0, -1, &x, &y)) != 0) {
        if (((Map_GetAddr(x, y)->door & 2) >> 1) == 0 &&
            OBJ_INCLASS(door) >= 8 &&
            (int)(((long)rand() * 10) / 0x8000L) < 3) {
            MapObj_X = x;
            MapObj_Y = y;
            if (check_alert(is_player, x, y))
                CloseDoor(0, door);
        }
        x++;
    }
    if (!is_player && DoAnimO) {
        for (i = 0; i < 8; i++)
            update_animobj(1);
    }
    quick_time = 0;
}
/* The weight the player carries, the cursor object included. */
int far Ply_Weight(void)
{
    int weight = player->weight;
    if (CursorObjPtr != 0)
        weight += ItemWeight(CursorObjPtr);
    return weight;
}
/* Fires the enter (6) or pressure plate triggers on tile for who at height z (how is the
   mode: 6 or 14 enter and exit; 7 or 15 pressure and release). A pressure trigger fires
   when the weight on the plate (check_weight with the player's load) crosses its threshold:
   fine y bit 0 says whether it is now pressed, bit 2 picks the heavier threshold (84 rather
   than 4). Returns 1 when the tile has a plate. */
char far check_pplate(struct Object far *who, struct Tile far *tile, int z, int how)
{
    register int type = how;
    unsigned char result = 0;
    unsigned char ran = 0;
    struct Object far *trig;
    struct Object far *previous = 0;
    register int weight_result;
    TriggerChainTileData_dseg_67d6_1BB9 = tile;
    trig = Obj_PtrTMem(&TriggerChainTileData_dseg_67d6_1BB9->objects);
    while (trig != 0) {
        if (((OBJ_CLASS(trig) & 0x1E) == CLASS_TRIGGER)) {
            if (Triggers[trig->id & ID_INCLASS] == type && (type & 7) == 6)
                UseTrigger(who, 0, trig, type);
            if ((type & 7) == 6) type++;
            if ((type & 7) == 7 && Triggers[trig->id & ID_INCLASS] == type) {
                    result = 1;
                    if (OBJ_Z(trig) != z) goto advance_pressure;
                    {
                        if (((OBJ_FINEY(trig) & 1) && type == 15) ||
                            (!(OBJ_FINEY(trig) & 1) && type == 7)) {
                        weight_result = check_weight(&tile->objects,
                            4 + (((OBJ_FINEY(trig) & 4) >> 2) * 80),
                            trig->pos & POS_Z, Ply_Weight());
                        if ((weight_result == 0 && type == 15) ||
                            (weight_result == 1 && type == 7)) {
                            UseTrigger(ThePlayer, 0, trig, type);
                            ran = 1;
                        }
                        }
                    }
            } else if ((type & 7) == 7 &&
                       (Triggers[trig->id & ID_INCLASS] & 7) == 7)
                previous = trig;
        }
advance_pressure:
        trig = Obj_PtrTMem(&trig->qn.link);
    }
    if (previous != 0 && !ran) {
        result = 1;
        trig = previous;
        if (((OBJ_FINEY(trig) & 1) && type == 15) ||
            (!(OBJ_FINEY(trig) & 1) && type == 7)) {
            weight_result = check_weight(&tile->objects,
                4 + (((OBJ_FINEY(trig) & 4) >> 2) * 80),
                trig->pos & POS_Z, Ply_Weight());
            if ((weight_result == 0 && type == 15) ||
                (weight_result == 1 && type == 7))
                update_pplate(trig);
        }
    }
    TriggerChainTileData_dseg_67d6_1BB9 = 0;
    return result;
}
/* A pressure plate changed state: every plate trigger on the tile flips its pressed bit,
   and with fine y bit 1 the floor texture steps to show the plate up or down. */
void far update_pplate(struct Object far *trig)
{
    struct Object far *next;
    register int yset = OBJ_FINEY(trig) & 1;
    register int notset = yset ? 0 : 1;
    next = Obj_PtrTMem(&TriggerChainTileData_dseg_67d6_1BB9->objects);
    while (next != 0) {
        if (((OBJ_CLASS(next) & 0x1E) == CLASS_TRIGGER) &&
            ((Triggers[next->id & ID_INCLASS] & 7) == 7))
            next->pos = next->pos & 0xE3FF |
                ((notset + (OBJ_FINEY(next) & 6)) & 7) << 10;
        next = Obj_PtrTMem(&next->qn.link);
    }
    if (((OBJ_FINEY(trig) & 2) >> 1) != 0) {
        yset = TriggerChainTileData_dseg_67d6_1BB9->floor;
        if (notset) yset++; else yset--;
        TriggerChainTileData_dseg_67d6_1BB9->floor = yset;
    }
}
/* Hack 17, thin ice: if the player stands on floor texture owner and fails an Acrobat
   check (difficulty his weight / 12 when that is over 20, else 0), the tile drops by one
   and takes the trap's texture (broken ice, inferred). */
void far do_ice_hack(struct Object far *trap)
{
    struct Tile far *tile;
    int texture;
    int skill_result;
    register int height;
    register int weight;
    tile = Map_GetAddr(OBJ_HOMEX(ThePlayer),
                       OBJ_HOMEY(ThePlayer));
    height = tile->height - 1;
    texture = (OBJ_FINEY(trap) << 3) |
              OBJ_FINEX(trap);
    weight = player->weight / 12;
    skill_result = skill_check(player->skills[SKILL_ACROBAT], weight > 20 ? weight : 0);
    if (tile->floor == trap->ol.f.owner && skill_result < 0) {
        if (height < 0) height = 0;
        change_terrain(OBJ_HOMEX(ThePlayer),
                               OBJ_HOMEY(ThePlayer),
                               0x3F, texture, height, 0x10, 0, 0, 0);
    }
}
struct Object far * far CreateObj(int id, int extra);
unsigned char far put_at(int x, int y, int z, struct Object far *obj, int a, int b);
/* Puts a bridge object on x, y at height z facing heading, unless one is there. */
struct Object far * far place_bridge(int x, int y, int zarg, int headingarg)
{
    struct Object far *obj;
    union Link far *head;
    register int z = zarg;
    register int heading = headingarg;
    head = &Map_GetAddr(x, y)->objects;
    obj = Obj_PtrTMem(head);
    while (obj != 0) {
        if (OBJ_ITEM(obj) == ITEM_BRIDGE &&
            OBJ_HEADING(obj) == heading &&
            OBJ_Z(obj) == z)
            break;
        obj = Obj_PtrTMem(&obj->qn.link);
    }
    if (obj == 0) {
        obj = CreateObj(ITEM_BRIDGE, 0);
        if (obj == 0) return 0;
        if (!put_at((x << 3) + (heading > 2 ? 1 : 0) + 3,
                    (y << 3) + (heading == 2 || heading == 4 ? 1 : 0) + 3,
                    z, obj, 0, 1)) return 0;
        obj->pos = obj->pos & 0xFC7F | (heading & 7) << 7;
        obj->pos = obj->pos & 0xFF80 | z & 0x7F;
    }
    return obj;
}
/* Removes the bridge at x, y with that height and heading. */
void far destroy_bridge(int x, int y, int zarg, int headingarg)
{
    struct Object far *obj;
    union Link far *head;
    register int z = zarg;
    register int heading = headingarg;
    head = &Map_GetAddr(x, y)->objects;
    obj = Obj_PtrTMem(head);
    while (obj != 0) {
        if (OBJ_ITEM(obj) == ITEM_BRIDGE &&
            OBJ_HEADING(obj) == heading &&
            OBJ_Z(obj) == z)
            break;
        obj = Obj_PtrTMem(&obj->qn.link);
    }
    if (obj != 0)
        if (Obj_Rem(&Map_GetAddr(x, y)->objects, obj))
            Obj_Free(obj);
}
/* Whether a random wandering monster may be this creature: never for strength 0, else one
   chance in its strength, so strong creatures turn up less often. */
/* name: IDA: Critter_RNG_STR_CHECK. FM Towns' eligible_castle_monster, between
   destroy_bridge and check_for_sunken_moongate. */
unsigned char far eligible_castle_monster(int npc)
{
    if (Creature[npc & ID_INMAJOR].attr[0] == 0) return 0;
    return rand() % Creature[npc & ID_INMAJOR].attr[0] == 0;
}
/* Level 0x44: once the tile at (0x18, 2) has been raised, lowers it and raises (2, 0x15),
   the sunken moongate (from the name). */
void far check_for_sunken_moongate(void)
{
    struct Tile far *tile = Map_GetAddr(0x18, 2);
    if (tile->height) {
        change_terrain(0x18, 2, 0x17, 4, 0, 1, 6, 5, 0);
        change_terrain(2, 0x15, 0x14, 4, 4, 1, 0, 0, 0);
    }
}
