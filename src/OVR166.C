/* target: ovr166 */
/* opts: -mm -1 -G -O -Y -d */
/* Triggers and traps: running a trigger's trap chain, the per-type trap actions (UseTrap)
   and the special-purpose "hack" traps, quest and variable traps, removing triggers and
   traps, wandering monsters, closing doors, pressure plates and bridges. */
#include <stdlib.h>

union LinkBits { unsigned word; struct { unsigned owner:6, link:10; } f; };
struct Object {
    unsigned id;
    unsigned pos;
    union { unsigned word; struct { unsigned quality:6, next:10; } f; } qn;
    union { unsigned word; struct { unsigned owner:6, link:10; } f; } ol;
    char pad08[0x0A - 0x08];
    unsigned char b0A;
    unsigned goal_word;
    unsigned flags0D;
    char pad0F[0x16 - 0x0F];
    unsigned home;
};
struct Player {
    char pad0[0x21];
    unsigned char skills[20];           /* 0x21; search is 11, acrobat 17 */
    char pad35[0x3D - 0x35];
    unsigned char b3D;                  /* 0x3D */
    char pad3E[0x4A - 0x3E];
    unsigned weight;                    /* 0x4A */
    char pad4C[0x62 - 0x4C];
    unsigned b62_0:4;                   /* word 0x62 */
    unsigned b62_4:1;
    unsigned b62_5:1;
    unsigned b62_6:4;
    unsigned in_pits:1;                 /* word 0x62, bit 10 */
    unsigned b63_3:5;
    char pad64[0x66 - 0x64];
    unsigned long quest[32];            /* 0x66, quests 0-127, four to a long */
    unsigned char special_quest[16];    /* 0xE6 */
    char padF6[0xF8 - 0xF6];
    unsigned char bF8;                  /* 0xF8 */
    unsigned vars[256];                 /* 0xF9 */
    char pad2F9[0x36D - 0x2F9];
    unsigned char clocks[16];           /* 0x36D */
};
extern struct Player near *player;
struct Tile {
    unsigned type:4, height:4, b8:2, floor:4, door:2;
    union { unsigned word; struct { unsigned wall:6, link:10; } f; } objects;
};
#define OBJ_ITEM(o)     ((o)->id & 0x1FF)
#define OBJ_TYPE(o)     (((o)->id & 0x3F) >> 0)
#define OBJ_MAJOR(o)    (((o)->id & 0x1C0) >> 6)
#define OBJ_MINOR(o)    (((o)->id & 0x30) >> 4)
#define OBJ_HOMEX(o)    (((o)->home & 0xFC00) >> 10)
#define OBJ_HOMEY(o)    (((o)->home & 0x3F0) >> 4)
#define OBJ_FLAGS(o)    (((o)->id & 0x1E00) >> 9)
#define OBJ_Z(o)        ((o)->pos & 0x7F)
#define OBJ_HEADING(o)  (((o)->pos & 0x380) >> 7)
#define OBJ_FINEY(o)    (((o)->pos & 0x1C00) >> 10)
#define OBJ_FINEX(o)    (((o)->pos & 0xE000) >> 13)
#define SET_ID_14(o, v)   ((o)->id = (o)->id & 0xBFFF | ((v) & 1) << 14)
#define SET_GOAL(o, v)    ((o)->goal_word = (o)->goal_word & 0xFFF0 | ((v) & 0xF) << 0)
#define SET_ITEM(o, v)    ((o)->id = (o)->id & 0xFE00 | (v) & 0x1FF)
#define SET_LONER(o, v)   ((o)->b0A = (o)->b0A & 0x7F | ((v) & 1) << 7)
#define SET_TEMP(o, v)    ((o)->flags0D = (o)->flags0D & 0xFEFF | ((v) & 1) << 8)
#define SET_ATTITUDE(o, v) ((o)->flags0D = (o)->flags0D & 0x3FFF | ((v) & 3) << 14)
#define SET_MAJOR(o, v)   ((o)->id = (o)->id & 0xFE3F | ((v) & 7) << 6)
#define SET_MINOR(o, v)   ((o)->id = (o)->id & 0xFFCF | ((v) & 3) << 4)
#define SET_SUB(o, v)     ((o)->id = (o)->id & 0xFFF0 | (v) & 0xF)
#define SET_FLAGS(o, v)   ((o)->id = (o)->id & 0xE1FF | ((v) & 0xF) << 9)
#define SET_Z(o, v)       ((o)->pos = (o)->pos & 0xFF80 | (v) & 0x7F)
#define SET_HEADING(o, v) ((o)->pos = (o)->pos & 0xFC7F | ((v) & 7) << 7)
#define SET_FINEX(o, v)   ((o)->pos = (o)->pos & 0x1FFF | ((v) & 7) << 13)
#define SET_FINEY(o, v)   ((o)->pos = (o)->pos & 0xE3FF | ((v) & 7) << 10)
extern unsigned char Triggers[16];
extern struct Object far *ActiveObj;
extern struct Object far *ThePlayer;
void far fread(void near *dest, int count, int size, int handle);

/* IDA: LoadTriggerObjDat. FM Towns' trap_init, first in this run of functions: the same
   fread of 16 bytes into the trigger type table. */
void far trap_init(int handle)
{
    fread(Triggers, 1, 16, handle);
}

/* IDA: MajorClass6TriggerType. FM Towns' trap_class_data: the same minor-class test on
   ActiveObj and the same index into the trigger table. */
unsigned char near * far trap_class_data(void)
{
    if (((ActiveObj->id & 0x30) >> 4) & 2)
        return Triggers + (ActiveObj->id & 0xF);
    return 0;
}

struct Object far * far Obj_PtrTMem(unsigned far *link);
struct Tile far * far Map_GetAddr(int x, int y);
int far skill_check(int skill, int difficulty);
void far update_pplate(struct Object far *trig);
void far delete_trap(unsigned far *head, struct Object far *trap);
int far SetOffTrap(struct Object far *who, struct Object far *context,
                    struct Object far *trap, int x, int y);
int far UseTrigger(struct Object far *who, struct Object far *start,
                   struct Object far *trig, register int type)
{
    struct Object far *trap;
    int quality;
    int owner;
    int sub;
    int minor;
    register int result;
    minor = (trig->id & 0x30) >> 4;
    sub = trig->id & 0xF;
    if (!(minor & 2)) return 2;
    if (type >= 0) {
        if (start != 0 && ((start->id & 0x1F0) >> 4) == 0x17 &&
            (start->id & 0xF) > 7 && trig->qn.f.next != 0) {
            trig = Obj_PtrTMem(&trig->qn.word);
            return UseTrigger(who, start, trig, type);
        }
        if (Triggers[sub] != type) return 2;
        if ((who->id & 0x1FF) == 0x7F) {
            if (!(trig->id & 0x800)) return 2;
            if (type == 5 && (trig->pos & 0x7F) > 0 &&
                skill_check(player->skills[11], trig->pos & 0x7F) <= 0)
                return 2;
        } else {
            if (((who->id & 0x1C0) >> 6) == 1) {
                if (!(trig->id & 0x1000) ||
                    ((trig->id & 0x1C0) >> 6) == 5) return 2;
            }
            if (((who->id & 0x1C0) >> 6) != 1 && !(trig->id & 0x200))
                return 2;
        }
    }
    trap = Obj_PtrTMem(&trig->ol.word);
    quality = trig->qn.f.quality;
    owner = trig->ol.f.owner;
    if (trap == 0) return 2;
    result = SetOffTrap(who, start, trap, quality, owner);
    if (!(trig->id & 0x400) && trig->ol.f.link > 0) {
        delete_trap(&Map_GetAddr(quality, owner)->objects.word, trap);
        result |= 0x20;
    }
    if ((type & 0xF07) == 7)
        update_pplate(trig);
    return result;
}
int far UseTrap(struct Object far *trap, int x, int y);
extern struct Object far *CharacterThatTriggeredTrap_dseg_67d6_8644;
extern struct Object far *TriggeringButton_dseg_67d6_8648;
int far SetOffTrap(struct Object far *who, struct Object far *context,
                    struct Object far *trap, int x, int y)
{
    register int result;
    if (CharacterThatTriggeredTrap_dseg_67d6_8644 == 0) {
        CharacterThatTriggeredTrap_dseg_67d6_8644 = who;
        TriggeringButton_dseg_67d6_8648 = context;
    }
    result = UseTrap(trap, x, y);
    CharacterThatTriggeredTrap_dseg_67d6_8644 = 0;
}
/* IDA: PerformVariableOperation. FM Towns' do_math_op, between SetOffTrap and
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
                player->quest[(unsigned)left >> 2] =
                    (player->quest[(unsigned)left >> 2] & ~(1 << (left & 3))) +
                    (((int)((player->quest[(unsigned)left >> 2] & (1 << (left & 3))) >> (left & 3)) ^ 1) << (left & 3));
                break;
            case 1:
                player->quest[(unsigned)left >> 2] =
                    (player->quest[(unsigned)left >> 2] & ~(1 << (left & 3))) +
                    (((int)((player->quest[(unsigned)left >> 2] & (1 << (left & 3))) >> (left & 3)) & 1) << (left & 3));
                break;
            default:
                player->quest[(unsigned)left >> 2] =
                    (player->quest[(unsigned)left >> 2] & ~(1 << (left & 3))) +
                    ((right > 0 ? 1 : 0) << (left & 3));
                break;
            }
        } else {
            left -= 0x80;
            if (left < 0x10) {
                register int value = player->special_quest[left];
                player->special_quest[left] = do_math_op(value, op, right);
            } else {
                left -= 0x10;
                if (left < 0x10) {
                    register int value = player->clocks[left];
                    player->clocks[left] = do_math_op(value, op, right);
                }
            }
        }
    }
}
int far get_numbered_variable(int index)
{
    if (index < 0x100) return player->vars[index];
    index -= 0x100;
    if (index < 0x80)
        return (int)((player->quest[(unsigned)index >> 2] & (long)(1 << (index & 3))) >> (index & 3));
    index -= 0x80;
    if (index < 0x10) return player->special_quest[index];
    index -= 0x10;
    if (index < 0x10) return player->clocks[index];
    return 0;
}
/* Byte images of an object for struct copies: a static object is 8 bytes, a mobile 0x1B. */
struct ObjStatic { char b[8]; };
struct ObjMobile { char b[0x1B]; };
struct Critter {
    char pad0[5];
    unsigned char attr[3];              /* 0x05: STR, DEX, INT */
};
extern struct Critter near *playerdat;
extern int PlayerLevel;
extern int MapObj_X, MapObj_Y;
extern unsigned char stay_centered;
extern int trap_teleport_data;
void far trap_fire(struct Object far *trap, int x, int y);
void far do_sfx(int quality, int owner);
int far do_teleport(struct Object far *who, int x, int y, int level);
int far change_terrain(int x, int y, int wall, int floor, int height,
                              int type, int dx, int dy, int extra);
void far set_jmp(int quality, int owner, int heading);
void far do_change_grokking(struct Object far *trap, struct Object far *obj, int x, int y);
struct Object far * far place_bridge(int x, int y, int zarg, int headingarg);
void far destroy_bridge(int x, int y, int zarg, int headingarg);
void far check_for_sunken_moongate(void);
void far player_get_exp(int n);
int far whack_thing(int index, int damage, int how, int extra);
int far do_trap_hack(struct Object far *trap, register int x, register int y);
int far inanimate_spell(int x, int y, struct Object far *obj, struct Object far *who,
                        int spell, int power);
char far dont_create_wandering_monster_here(struct Object far *obj);
unsigned char far eligible_castle_monster(int npc);
char far init_this_critter(struct Object far *obj);
char far IsMobElem(struct Object far *obj);
struct Object far * far Obj_Alloc(char mobile);
unsigned char far put_at(int x, int y, int z, struct Object far *obj, int range, int nocull);
int far add_animobj(int index, int len, int a, char x, char y);
struct Object far * far Obj_IntTMem(int index);
int far Obj_MemTPtr(struct Object far *obj);
struct Object far * far Obj_InList(unsigned far **head, int recurse, int major, int minor,
                                   int index);
void far OpenDoor(struct Object far *who, struct Object far *door);
void far CloseDoor(struct Object far *who, struct Object far *door);
void far ToggleDoor(struct Object far *who, struct Object far *door);
char far Obj_Rem(unsigned far *head, struct Object far *obj);
void far Obj_Free(struct Object far *obj);
void far Obj_Add(unsigned far *head, struct Object far *obj);
void far Obj_FreeLinkChain(unsigned far *head, struct Object far *obj);
void far trap_obj_del(unsigned far *head, struct Object far *obj);
void far rem_anim_from_list(int index);
void far editchng(int what);
struct Object far * far FindObj(int major, int minor, int cls, int how, int *where);
unsigned char far ObjWorn(int id, int slot);
char far * far get_string(int id);
void far scroll_print(char far *s);
void far print_path_to(char far *s, int fx, int fy, int fz, int tx, int ty, int tz, int how);

/* Condition traps (variable, skill, proximity, inventory) branch by running the trap or
   trigger that follows the first object the trap links to, and return its result. */
#define RUN_ELSE_CHAIN()                                                            \
    obj = Obj_PtrTMem(&trap->ol.word);                                              \
    if (obj->qn.f.next > 0)                                                         \
        return (OBJ_MINOR(Obj_PtrTMem(&obj->qn.word)) & 2)                          \
            ? UseTrigger(CharacterThatTriggeredTrap_dseg_67d6_8644,                 \
                         TriggeringButton_dseg_67d6_8648,                           \
                         Obj_PtrTMem(&obj->qn.word), -1)                            \
            : SetOffTrap(CharacterThatTriggeredTrap_dseg_67d6_8644,                 \
                         TriggeringButton_dseg_67d6_8648,                           \
                         Obj_PtrTMem(&obj->qn.word), x, y);                         \
    return 2

int far UseTrap(struct Object far *trap, int x, int y)
{
    struct Tile far *tile;
    struct Object far *linked;
    struct Object far *obj;
    struct Object far *lock;
    unsigned far *head;
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

    switch (OBJ_TYPE(trap)) {
    case 2:
        trap_fire(trap, x, y);
        break;
    case 4:
        do_sfx(trap->qn.f.quality, trap->ol.f.owner);
        break;
    case 1:
        v = trap->qn.f.quality;
        a = trap->ol.f.owner;
        b = OBJ_Z(trap);
        trap_teleport_data = OBJ_FINEX(trap) & 1;
        if ((OBJ_FINEX(trap) & 2) >> 1)
            trap_teleport_data = trap_teleport_data | (OBJ_HEADING(trap) + 8) << 2;
        if (CharacterThatTriggeredTrap_dseg_67d6_8644 == ThePlayer &&
            ((OBJ_FINEX(trap) & 4) >> 2) == 1) {
            if (player->b62_4)
                player->bF8 = PlayerLevel;
            player->b62_4 = 0;
        }
        result = do_teleport(CharacterThatTriggeredTrap_dseg_67d6_8644, v, a, b);
        break;
    case 5:
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
    case 18:
        set_jmp(trap->qn.f.quality, trap->ol.f.owner, OBJ_HEADING(trap));
        break;
    case 13:
        i = OBJ_Z(trap) + (OBJ_FINEX(trap) << 7);
        c = (trap->qn.f.quality << 6) + trap->ol.f.owner;
        b = OBJ_HEADING(trap);
        set_numbered_variable(i, b, c);
        if (OBJ_FINEY(trap))
            player->clocks[15]++;
        break;
    case 14:
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
    case 10: {
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
    case 22:
        if (OBJ_HOMEX(CharacterThatTriggeredTrap_dseg_67d6_8644) >= x &&
            OBJ_HOMEY(CharacterThatTriggeredTrap_dseg_67d6_8644) >= y &&
            OBJ_HOMEX(CharacterThatTriggeredTrap_dseg_67d6_8644) - x <= trap->qn.f.quality &&
            OBJ_HOMEY(CharacterThatTriggeredTrap_dseg_67d6_8644) - y <= trap->ol.f.owner &&
            (OBJ_FINEX(trap) ||
             OBJ_Z(CharacterThatTriggeredTrap_dseg_67d6_8644) <= OBJ_Z(trap)) &&
            (OBJ_FINEY(trap) ||
             OBJ_Z(CharacterThatTriggeredTrap_dseg_67d6_8644) > OBJ_Z(trap)))
            break;
        RUN_ELSE_CHAIN();
    case 19:
        obj = Obj_PtrTMem(&trap->ol.word);
        do_change_grokking(trap, obj, x, y);
        break;
    case 21: {
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
            if (tile->type == 0 && i < 0) {
                type = 1;
                height = 0xF;
            } else if (tile->type == 1 && height == 0x10)
                type = 0;
            if (trap->ol.f.owner == 0x10)
                limit = 0x3F;
            break;
        case 1:
            g = tile->floor;
            now = floor = g + i;
            break;
        case 2:
            g = tile->objects.f.wall;
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
    case 23:
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
    case 24: {
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
    case 17:
        g = ((trap->qn.f.quality << 3) + (trap->ol.f.owner & 7) - 0x100) *
            (1 << (trap->ol.f.owner >> 3));
        player_get_exp(g);
        break;
    case 0:
        if ((int)(((long)rand() * 10) / 0x8000L) < 7)
            g = 2;
        else
            g = 0;
        result = whack_thing(Obj_MemTPtr(CharacterThatTriggeredTrap_dseg_67d6_8644),
                             trap->qn.f.quality * (trap->ol.f.owner ? -1 : 1), 4, g);
        break;
    case 3:
        result = do_trap_hack(trap, x, y);
        break;
    case 6:
        spell = trap->qn.f.quality;
        power = trap->ol.f.owner;
        result = inanimate_spell(x, y, trap, CharacterThatTriggeredTrap_dseg_67d6_8644,
                                 spell, power);
        break;
    case 7: {
        unsigned char random_critter = 0;
        unsigned char placed;
        if ((int)(((long)rand() * 0x3F) / 0x8000L) < trap->qn.f.quality)
            return 2;
        continue_chain = 0;
        if ((trap->id & 0x8000) >> 15)
            break;
        linked = Obj_PtrTMem(&trap->ol.word);
        if (linked == 0)
            break;
        if (OBJ_MAJOR(linked) == 1 && dont_create_wandering_monster_here(linked))
            break;
        if ((int)((player->quest[1] & 8) >> 3) && (PlayerLevel - 1) / 8 == 6)
            break;
        if (OBJ_ITEM(linked) == 0x7F) {
            int lo;
            int range;
            int item;
            lo = (((PlayerLevel - 1) % 8 + 1) * 3 + player->clocks[1]) / 9;
            range = player->b3D + player->clocks[1] + 1;
            if (range > 16)
                range = 16;
            if (lo < 1)
                lo = 1;
            item = (rand() % lo << 4) + rand() % range + 0x40;
            while (!eligible_castle_monster(item))
                item = (rand() % lo << 4) + rand() % range + 0x40;
            random_critter = 1;
            SET_ITEM(linked, item);
            init_this_critter(linked);
            SET_LONER(linked, 1);
        }
        obj = Obj_Alloc(IsMobElem(linked));
        if (obj != 0) {
            if (IsMobElem(linked))
                *(struct ObjMobile far *)obj = *(struct ObjMobile far *)linked;
            else
                *(struct ObjStatic far *)obj = *(struct ObjStatic far *)linked;
            if (random_critter) {
                SET_ITEM(linked, 0x7F);
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
                if (placed && !((obj->id & 0x8000) >> 15) && obj->ol.f.link > 0 &&
                    (copy = Obj_Alloc(0)) != 0) {
                    *(struct ObjStatic far *)copy =
                        *(struct ObjStatic far *)Obj_IntTMem(obj->ol.f.link);
                    obj->ol.f.link = Obj_MemTPtr(copy);
                    while (copy->qn.f.next != 0)
                        copy->qn.f.next = 0;
                    if (!((copy->id & 0x8000) >> 15) && copy->ol.f.link > 0)
                        copy->ol.f.link = 0;
                }
            }
            if (placed && OBJ_MAJOR(obj) == 7) {
                int index = Obj_MemTPtr(obj);
                add_animobj(index, -1, 0, x, y);
            }
        }
        result = 2;
        break;
    }
    case 8: {
        int savex;
        int savey;
        tile = Map_GetAddr(x, y);
        head = &tile->objects.word;
        linked = Obj_InList(&head, 0, 5, 0, -1);
        savex = MapObj_X;
        savey = MapObj_Y;
        MapObj_X = x;
        MapObj_Y = y;
        if (linked != 0) {
            switch (trap->qn.f.quality) {
            case 1:
                OpenDoor(CharacterThatTriggeredTrap_dseg_67d6_8644, linked);
                break;
            case 2:
                CloseDoor(CharacterThatTriggeredTrap_dseg_67d6_8644, linked);
                break;
            case 3:
                ToggleDoor(CharacterThatTriggeredTrap_dseg_67d6_8644, linked);
                break;
            }
        } else if ((linked = Obj_InList(&head, 0, 7, -1, 0xF)) != 0) {
            if ((linked->ol.f.owner & 0xF) < 8) {
                switch (trap->qn.f.quality) {
                case 2:
                case 3:
                    CloseDoor(CharacterThatTriggeredTrap_dseg_67d6_8644, linked);
                }
            } else {
                switch (trap->qn.f.quality) {
                case 1:
                case 3:
                    OpenDoor(CharacterThatTriggeredTrap_dseg_67d6_8644, linked);
                }
            }
        }
        MapObj_X = savex;
        MapObj_Y = savey;
        if (linked != 0 && trap->ol.f.owner != 0) {
            head = &linked->ol.word;
            lock = Obj_InList(&head, 0, 4, 0, 0xF);
            continue_chain = 0;
            if (lock != 0 && Obj_Rem(head, lock))
                Obj_Free(lock);
            if (!((trap->id & 0x8000) >> 15) && trap->ol.f.link != 0) {
                lock = Obj_PtrTMem(&trap->ol.word);
                if ((obj = Obj_Alloc(0)) != 0) {
                    *(struct ObjStatic far *)obj = *(struct ObjStatic far *)lock;
                    Obj_Add(head, obj);
                }
            }
        }
        break;
    }
    case 11:
        head = &Map_GetAddr(trap->qn.f.quality, trap->ol.f.owner)->objects.word;
        lock = Obj_PtrTMem(&trap->ol.word);
        if (OBJ_MAJOR(lock) == 6)
            trap_obj_del(head, lock);
        else if (OBJ_MAJOR(lock) == 7) {
            if (Obj_Rem(head, lock)) {
                rem_anim_from_list(Obj_MemTPtr(lock));
                Obj_Free(lock);
            }
        } else
            Obj_FreeLinkChain(head, lock);
        editchng(2);
        continue_chain = 0;
        break;
    case 12: {
        unsigned char trip = 0;
        i = trap->qn.f.quality << 5 | trap->ol.f.owner;
        obj = FindObj(i >> 6, (i & 0x30) >> 4, i & 0xF, 4, &slot);
        if (obj == 0)
            trip = 1;
        else if (OBJ_FINEX(trap) && !ObjWorn(i, slot))
            trip = 1;
        else if (OBJ_Z(trap) > 0 && (obj->id & 0x8000) >> 15 &&
                 !(obj->ol.f.link & 0x200) && OBJ_Z(trap) > obj->ol.f.link)
            trip = 1;
        if (trip) {
            RUN_ELSE_CHAIN();
        }
        break;
    }
    case 9:
        if (CharacterThatTriggeredTrap_dseg_67d6_8644 != 0 &&
            (trap->qn.f.quality == 0x3F ||
             trap->qn.f.quality == (CharacterThatTriggeredTrap_dseg_67d6_8644->id & 0xF))) {
            int damage;
            damage = (int)(((long)rand() * player->skills[9]) / 0x8000L) + 3;
            print_path_to(get_string(0x304), OBJ_HOMEX(ThePlayer), OBJ_HOMEY(ThePlayer), 0,
                          OBJ_HOMEX(CharacterThatTriggeredTrap_dseg_67d6_8644),
                          OBJ_HOMEY(CharacterThatTriggeredTrap_dseg_67d6_8644), 0, 0);
            result = whack_thing(Obj_MemTPtr(CharacterThatTriggeredTrap_dseg_67d6_8644),
                                 damage, 4, 0);
        }
        continue_chain = 0;
        break;
    case 16: {
        char far *str = get_string(trap->qn.f.quality << 5 | trap->ol.f.owner & 0x1F | 0x1200);
        if (str != 0 && (!(trap->ol.f.owner >> 5) || player->b62_5))
            scroll_print(str);
        break;
    }
    }
    if (continue_chain && trap->ol.f.link > 0) {
        struct Object far *next = Obj_PtrTMem(&trap->ol.word);
        if (OBJ_MAJOR(next) == 6) {
            if (OBJ_MINOR(next) & 2)
                result |= UseTrigger(CharacterThatTriggeredTrap_dseg_67d6_8644,
                                     TriggeringButton_dseg_67d6_8648, next, -1);
            else
                result |= SetOffTrap(CharacterThatTriggeredTrap_dseg_67d6_8644,
                                     TriggeringButton_dseg_67d6_8648, next, x, y);
        }
    }
    return result;
}
extern int TrapIndexToRemove_dseg_67d6_863C;
extern int RemoveTrapFlags_dseg_67d6_863E;
int far Obj_MemTPtr(struct Object far *obj);
void far rem_timer_obj(int index);
char far Obj_Rem(unsigned far *head, struct Object far *obj);
void far Obj_Free(struct Object far *obj);
void far kill_triggers(unsigned far *head)
{
    struct Object far *obj;
    struct Object far *next;
    unsigned far *link;
    obj = Obj_PtrTMem(head);
    while (obj != 0) {
        next = Obj_PtrTMem(&obj->qn.word);
        if (((obj->id & 0x1C0) >> 6) == 6 &&
            ((obj->id & 0x30) >> 4) >= 2 &&
            obj->ol.f.link == TrapIndexToRemove_dseg_67d6_863C) {
            if (Triggers[obj->id & 0xF] == 10)
                rem_timer_obj(Obj_MemTPtr(obj));
            if (Obj_Rem(head, obj)) Obj_Free(obj);
            obj->ol.word &= 0x3F;
            RemoveTrapFlags_dseg_67d6_863E--;
        }
        if (!((obj->id & 0x8000) >> 15)) {
            if (((union LinkBits far *)(link = &obj->ol.word))->f.link != 0)
                kill_triggers(link);
        }
        obj = next;
    }
}
extern struct Tile far *mapdata;
extern unsigned far *Obj_Find_Head;
struct Object far * far Obj_Find(unsigned far *head, int recurse, int index);
void far Obj_FreeLinkChain(unsigned far *head, struct Object far *obj);
void far delete_trap(unsigned far *head, struct Object far *trap)
{
    struct Tile far *tile;
    unsigned far *tilehead;
    register unsigned i;
    RemoveTrapFlags_dseg_67d6_863E = (trap->id & 0x1E00) >> 9;
    if (RemoveTrapFlags_dseg_67d6_863E != 0) {
        TrapIndexToRemove_dseg_67d6_863C = Obj_MemTPtr(trap);
        tile = mapdata;
        for (i = 0; RemoveTrapFlags_dseg_67d6_863E > 0 && i < 0x1000; i++, tile++) {
            tilehead = &tile->objects.word;
            if (((union LinkBits far *)tilehead)->f.link > 0)
                kill_triggers(tilehead);
        }
    }
    if ((trap = Obj_Find(head, 1, Obj_MemTPtr(trap))) != 0)
        Obj_FreeLinkChain(Obj_Find_Head, trap);
}
void far crystal_ball(struct Object far *trap, int x, int y);
void far eight_pos_switch(int flags, struct Object far *trap, int x, int y);
void far player_did_bad(int owner);
void far change_weapon_playerbest(int x, int y, int owner);
void far check_fraznium(int x, int y, int owner);
void far standing_wave(int x, int y, int owner);
void far cycle_floor(int x, int y, int finex, int finey, int owner, int z, int heading);
void far do_ice_hack(struct Object far *trap);
void far switch_flip_hack(int x, int y, int owner);
void far reset_arrow_pillars(int x, int y, int owner);
void far toggle_pillars_hack(int x, int y, int lo, int hi, int where);
void far toggle_object_height(struct Object far *trap, char up);
void far do_graffiti(int x, int y, int owner);
void far skup_ductosnore(void);
void far find_and_gronk_force_field(int x, int y);
struct Object far * far Obj_IntTMem(int index);
void far editchng(int what);
void far play_with_switches(int x, int y);
void far arena_player_runs(void);
void far arena_opponent_runs(struct Object far *obj);
void far maybe_cheat_arena_fire(void);
void far do_qbert(int owner);
void far redeem_all_bottles(int x, int y);
void far courtyard_hacking(int x, int y, struct Object far *trap);
void far recharge_lightbulbs(int x, int y);
void far move_folks_around(void);
void far Sched_SetAllClocks(int how);
void far ruin_cure_potions(int x, int y);
void far go_vend(int quality, int owner, int x, int y, int flags);
void far gronkify_change_goal();
void far gronk_whoami(int who, int how, unsigned char near *info, void (far *fn)());
void far player_sleep(int owner);
void far make_terrain_unseen(int x, int y, int from, int to);
void far black_gem_rotate(void);
int far black_gem_trip(void);
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
        eight_pos_switch(OBJ_FLAGS(TriggeringButton_dseg_67d6_8648), trap, x, y);
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
            if (CharacterThatTriggeredTrap_dseg_67d6_8644 == ThePlayer)
                arena_player_runs();
            else
                arena_opponent_runs(CharacterThatTriggeredTrap_dseg_67d6_8644);
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
        SET_ID_14(Obj_IntTMem(trap->ol.f.link), trap->ol.f.owner);
        editchng(2);
        break;
    case 40:
    case 41:
    case 42:
        go_vend(trap->qn.f.quality, trap->ol.f.owner, x, y,
                OBJ_FLAGS(TriggeringButton_dseg_67d6_8648));
        break;
    case 43:
        info[7] = trap->ol.f.owner;
        info[8] = OBJ_FINEY(trap) > 0 ? 1
                : Obj_MemTPtr(CharacterThatTriggeredTrap_dseg_67d6_8644);
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
        if (Obj_IntTMem(trap->ol.f.link) == CharacterThatTriggeredTrap_dseg_67d6_8644 ||
            trap->ol.f.link == 1)
            SET_GOAL(CharacterThatTriggeredTrap_dseg_67d6_8644, trap->ol.f.owner);
        break;
    }
    return 2;
}
struct Object far * far Obj_Alloc(char mobile);
void far Obj_Add(unsigned far *head, struct Object far *obj);
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
    SET_SUB(trigger, 0);
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
    Obj_Add(&tile->objects.word, trigger);
    SET_MAJOR(trap, 6);
    SET_MINOR(trap, 0);
    SET_SUB(trap, sub);
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
    Obj_Add(&tile->objects.word, trap);
    return Obj_MemTPtr(trigger);
}
void far trigger_obj_del(unsigned far *head, struct Object far *obj)
{
    struct Object far *linked;
    int x;
    register int flags;
    register int y;
    linked = Obj_PtrTMem(&obj->ol.word);
    flags = (linked->id & 0x1E00) >> 9;
    if (flags == 1) {
        x = obj->qn.f.quality;
        y = obj->ol.f.owner;
        delete_trap(&Map_GetAddr(x, y)->objects.word, linked);
    } else if (Obj_Rem(head, obj)) {
        linked->id = linked->id & 0xE1FF | (((flags - 1) & 0xF) << 9);
        Obj_Free(obj);
    }
    if (Triggers[obj->id & 0xF] == 10)
        rem_timer_obj(Obj_MemTPtr(obj));
}
void far trap_obj_del(unsigned far *head, struct Object far *obj)
{
    if (((obj->id & 0x30) >> 4) > 1)
        trigger_obj_del(head, obj);
    else
        delete_trap(head, obj);
}
/* FM Towns calls this callback is_this_wandering; its bit and exclusions match. */
extern struct Object far *ObjectToRunCodeAround_dseg_67d6_8640;
extern unsigned char CreatedObjectFound_dseg_67d6_1BB8;
char far is_this_wandering(int x, int y, struct Object far *obj)
{
    struct Object far *p = obj;
    if (((p->flags0D & 0x100) >> 8) &&
        obj != ObjectToRunCodeAround_dseg_67d6_8640 &&
        p != ThePlayer)
        CreatedObjectFound_dseg_67d6_1BB8 = 1;
    return CreatedObjectFound_dseg_67d6_1BB8;
}
typedef char (far *AreaFn)(int, int, struct Object far *);
void far gronk_area(struct Object far *who, char count, AreaFn fn,
                    unsigned char type, unsigned char dist, unsigned char radius);
char far dont_create_wandering_monster_here(struct Object far *obj)
{
    CreatedObjectFound_dseg_67d6_1BB8 = 0;
    ObjectToRunCodeAround_dseg_67d6_8640 = obj;
    gronk_area(obj, 1, is_this_wandering, 0, 0, 4);
    return CreatedObjectFound_dseg_67d6_1BB8;
}
char far check_alert(unsigned char is_player, int x, int y)
{
    int px, py;
    if (!is_player) return 1;
    px = (ThePlayer->home & 0xFC00) >> 10;
    py = (ThePlayer->home & 0x3F0) >> 4;
    if (abs(px - x) < 8 && abs(py - y) < 8)
        return 0;
    return 1;
}
struct Object far * far Obj_FindInMap(int major, int minor, int index, int *x, int *y);
struct Object far * far Obj_PtrTMem(unsigned far *link);
char far IsMobElem(struct Object far *obj);
void far DoWanderingMonsters(unsigned char is_player)
{
    int x = 0;
    int y = 0;
    struct Object far *obj;
    struct Object far *newobj;
    while ((obj = Obj_FindInMap(6, 0, 7, &x, &y)) != 0) {
        if (((obj->id & 0x1E00) >> 9) == 0) {
            newobj = Obj_PtrTMem(&obj->ol.word);
            if (IsMobElem(newobj)) {
                newobj->flags0D = newobj->flags0D & 0xFEFF | 0x100;
                if (check_alert(is_player, x, y))
                    UseTrap(obj, x, y);
            }
        }
        x++;
    }
}
struct Tile far * far Map_GetAddr(int x, int y);
extern unsigned char quick_time;
extern unsigned char DoAnimO;
extern int MapObj_X, MapObj_Y;
void far CloseDoor(struct Object far *who, struct Object far *door);
void far update_animobj(int n);
void far DoClosingDoors(unsigned char is_player)
{
    int x = 0;
    int y = 0;
    struct Object far *door;
    int i;
    quick_time = 1;
    while ((door = Obj_FindInMap(5, 0, -1, &x, &y)) != 0) {
        if (((Map_GetAddr(x, y)->door & 2) >> 1) == 0 &&
            (door->id & 0xF) >= 8 &&
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
extern struct Object far *CursorObjPtr;
int far ItemWeight(struct Object far *obj);
int far Ply_Weight(void)
{
    int weight = player->weight;
    if (CursorObjPtr != 0)
        weight += ItemWeight(CursorObjPtr);
    return weight;
}
extern struct Tile far *TriggerChainTileData_dseg_67d6_1BB9;
int far check_weight(unsigned far *head, int threshold, int z, int weight);
int far Ply_Weight(void);
char far check_pplate(struct Object far *who, struct Tile far *tile, int z, int how)
{
    register int type = how;
    unsigned char result = 0;
    unsigned char ran = 0;
    struct Object far *trig;
    struct Object far *previous = 0;
    register int weight_result;
    TriggerChainTileData_dseg_67d6_1BB9 = tile;
    trig = Obj_PtrTMem(&TriggerChainTileData_dseg_67d6_1BB9->objects.word);
    while (trig != 0) {
        if (((((trig->id & 0x1F0) >> 4) & 0x1E) == 0x1A)) {
            if (Triggers[trig->id & 0xF] == type && (type & 7) == 6)
                UseTrigger(who, 0, trig, type);
            if ((type & 7) == 6) type++;
            if ((type & 7) == 7 && Triggers[trig->id & 0xF] == type) {
                    result = 1;
                    if ((trig->pos & 0x7F) != z) goto advance_pressure;
                    {
                        if (((((trig->pos & 0x1C00) >> 10) & 1) && type == 15) ||
                            (!(((trig->pos & 0x1C00) >> 10) & 1) && type == 7)) {
                        weight_result = check_weight(&tile->objects.word,
                            4 + (((((trig->pos & 0x1C00) >> 10) & 4) >> 2) * 80),
                            trig->pos & 0x7F, Ply_Weight());
                        if ((weight_result == 0 && type == 15) ||
                            (weight_result == 1 && type == 7)) {
                            UseTrigger(ThePlayer, 0, trig, type);
                            ran = 1;
                        }
                        }
                    }
            } else if ((type & 7) == 7 &&
                       (Triggers[trig->id & 0xF] & 7) == 7)
                previous = trig;
        }
advance_pressure:
        trig = Obj_PtrTMem(&trig->qn.word);
    }
    if (previous != 0 && !ran) {
        result = 1;
        trig = previous;
        if (((((trig->pos & 0x1C00) >> 10) & 1) && type == 15) ||
            (!(((trig->pos & 0x1C00) >> 10) & 1) && type == 7)) {
            weight_result = check_weight(&tile->objects.word,
                4 + (((((trig->pos & 0x1C00) >> 10) & 4) >> 2) * 80),
                trig->pos & 0x7F, Ply_Weight());
            if ((weight_result == 0 && type == 15) ||
                (weight_result == 1 && type == 7))
                update_pplate(trig);
        }
    }
    TriggerChainTileData_dseg_67d6_1BB9 = 0;
    return result;
}
extern struct Tile far *TriggerChainTileData_dseg_67d6_1BB9;
void far update_pplate(struct Object far *trig)
{
    struct Object far *next;
    register int yset = ((trig->pos & 0x1C00) >> 10) & 1;
    register int notset = yset ? 0 : 1;
    next = Obj_PtrTMem(&TriggerChainTileData_dseg_67d6_1BB9->objects.word);
    while (next != 0) {
        if (((((next->id & 0x1F0) >> 4) & 0x1E) == 0x1A) &&
            ((Triggers[next->id & 0xF] & 7) == 7))
            next->pos = next->pos & 0xE3FF |
                ((notset + (((next->pos & 0x1C00) >> 10) & 6)) & 7) << 10;
        next = Obj_PtrTMem(&next->qn.word);
    }
    if (((((trig->pos & 0x1C00) >> 10) & 2) >> 1) != 0) {
        yset = TriggerChainTileData_dseg_67d6_1BB9->floor;
        if (notset) yset++; else yset--;
        TriggerChainTileData_dseg_67d6_1BB9->floor = yset;
    }
}
void far do_ice_hack(struct Object far *trap)
{
    struct Tile far *tile;
    int texture;
    int skill_result;
    register int height;
    register int weight;
    tile = Map_GetAddr((ThePlayer->home & 0xFC00) >> 10,
                       (ThePlayer->home & 0x3F0) >> 4);
    height = tile->height - 1;
    texture = (((trap->pos & 0x1C00) >> 10) << 3) |
              ((trap->pos & 0xE000) >> 13);
    weight = player->weight / 12;
    skill_result = skill_check(player->skills[17], weight > 20 ? weight : 0);
    if (tile->floor == trap->ol.f.owner && skill_result < 0) {
        if (height < 0) height = 0;
        change_terrain((ThePlayer->home & 0xFC00) >> 10,
                               (ThePlayer->home & 0x3F0) >> 4,
                               0x3F, texture, height, 0x10, 0, 0, 0);
    }
}
struct Object far * far CreateObj(int id, int extra);
unsigned char far put_at(int x, int y, int z, struct Object far *obj, int a, int b);
struct Object far * far place_bridge(int x, int y, int zarg, int headingarg)
{
    struct Object far *obj;
    unsigned far *head;
    register int z = zarg;
    register int heading = headingarg;
    head = &Map_GetAddr(x, y)->objects.word;
    obj = Obj_PtrTMem(head);
    while (obj != 0) {
        if ((obj->id & 0x1FF) == 0x164 &&
            ((obj->pos & 0x380) >> 7) == heading &&
            (obj->pos & 0x7F) == z)
            break;
        obj = Obj_PtrTMem(&obj->qn.word);
    }
    if (obj == 0) {
        obj = CreateObj(0x164, 0);
        if (obj == 0) return 0;
        if (!put_at((x << 3) + (heading > 2 ? 1 : 0) + 3,
                    (y << 3) + (heading == 2 || heading == 4 ? 1 : 0) + 3,
                    z, obj, 0, 1)) return 0;
        obj->pos = obj->pos & 0xFC7F | (heading & 7) << 7;
        obj->pos = obj->pos & 0xFF80 | z & 0x7F;
    }
    return obj;
}
void far destroy_bridge(int x, int y, int zarg, int headingarg)
{
    struct Object far *obj;
    unsigned far *head;
    register int z = zarg;
    register int heading = headingarg;
    head = &Map_GetAddr(x, y)->objects.word;
    obj = Obj_PtrTMem(head);
    while (obj != 0) {
        if ((obj->id & 0x1FF) == 0x164 &&
            ((obj->pos & 0x380) >> 7) == heading &&
            (obj->pos & 0x7F) == z)
            break;
        obj = Obj_PtrTMem(&obj->qn.word);
    }
    if (obj != 0)
        if (Obj_Rem(&Map_GetAddr(x, y)->objects.word, obj))
            Obj_Free(obj);
}
/* One critter type's record, 0x30 bytes. */
struct Creature {
    char pad00[5];
    unsigned char strength;             /* 0x05 */
    char pad06[0x30 - 0x06];
};
extern struct Creature Creature[];
/* IDA: Critter_RNG_STR_CHECK. FM Towns' eligible_castle_monster, between destroy_bridge and
   check_for_sunken_moongate: a zero strength byte fails, otherwise one chance in strength. */
unsigned char far eligible_castle_monster(int npc)
{
    if (Creature[npc & 0x3F].strength == 0) return 0;
    return rand() % Creature[npc & 0x3F].strength == 0;
}
void far check_for_sunken_moongate(void)
{
    struct Tile far *tile = Map_GetAddr(0x18, 2);
    if (tile->height) {
        change_terrain(0x18, 2, 0x17, 4, 0, 1, 6, 5, 0);
        change_terrain(2, 0x15, 0x14, 4, 4, 1, 0, 0, 0);
    }
}
