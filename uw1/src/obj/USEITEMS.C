/* target: seg040_352B */
/* opts: -mm -1 -G -O -Y -d */
/* Using objects: the dispatch on an object's class, removing a used object, putting a new
   object in the hand, keys, lockpicks, spikes, bones, poles, the anvil, the orb, food and
   drink, the rock hammer, oil, books, doors, switches, containers, locks, traps and the
   spells objects carry: the whole of UW1's DOS resident segment seg040_352B, in original
   order. UW2 split the same code into OBJUSE.C (resident seg040_34E7) and USEITEMS.C
   (overlay ovr138); UW1 has it in one segment, OBJUSE.C's functions first and last
   around the item handlers.

   UseObj is the entry point for every "use": the player using an object in the 3D view or
   in the inventory, a critter opening a door, two objects colliding. It dispatches on the
   object's major and minor class, then sets off any trap or trigger the object holds
   (checkTrap) and any spell it carries (checkSpell). An object that needs a second object
   ("Use <name> on what?") goes through UseThing, which puts it on the cursor and stores
   the function to call in ObjectActor; the *On functions (and the listing-named
   SpikeDoor_seg040_662, TybalsOrb_seg040_A12, seg040_352B_AFF) are those second halves,
   called with the object clicked next, the first being ObjectActing.

   UW1 against UW2: how is a plain char; no watch, crystal, key gems, horn, flam or tym
   runes; door spikes, Garamon's bones, Tybal's orb, incense, the silver tree and seed, the
   zanium stack and rotworm stew are UW1's own; locks open with checkTrap's how 6 and doors
   with 7; CloseDoor takes only the door and fires no trap; switches are flipped in
   UseRect; checkSpell prints nothing when it is too soon and spends one charge
   (useNSpellCharges has no count); food makes no sounds and item 0xB9 poisons; string
   numbers and item ids differ.

   Data owned: nextSpellTime and always_decode, and the literals.
   UW1 has no symbol-bearing build: the names are UW2's (the FM Towns symbol table) where
   the routine is the same, else the listing's. */

#include <string.h>
#include <stdlib.h>
#define UseObj UW2_UseObj
#define UseLockpickOn UW2_UseLockpickOn
#define UseKeyOn UW2_UseKeyOn
#define UseUnique UW2_UseUnique
#define UseLight UW2_UseLight
#define UseWand UW2_UseWand
#define UseFood UW2_UseFood
#define CloseDoor UW2_CloseDoor
#define decode_obj_spell UW2_decode_obj_spell
#define always_decode UW2_always_decode
#define ObjectActorArg UW2_ObjectActorArg
#define add_animobj UW2_add_animobj
#define player_eat UW2_player_eat
#define UseBook UW2_UseBook
#define UseRockHammerOn UW2_UseRockHammerOn
#define UseKey UW2_UseKey
#define UseOilOn UW2_UseOilOn
#include "combat.h"
#include "critter.h"
#include "event.h"
#include "gfx.h"
#include "inv.h"
#include "map.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"
#undef UseObj
#undef UseLockpickOn
#undef UseKeyOn
#undef UseUnique
#undef UseLight
#undef UseWand
#undef UseFood
#undef CloseDoor
#undef decode_obj_spell
#undef always_decode
#undef ObjectActorArg
#undef add_animobj
#undef player_eat
#undef UseBook
#undef UseRockHammerOn
#undef UseKey
#undef UseOilOn

/* UW1: how is a plain char throughout (object.h: unsigned char); CloseDoor takes only the
   door; decode_obj_spell's flag is a plain char; useNSpellCharges spends one charge and
   returns nothing. */
struct Object far * far UseObj(struct Object far *who, struct Object far *obj, char how);
void far UseLockpickOn(struct Object far *obj, char how);
void far UseKeyOn(struct Object far *obj, char how);
void far UseUnique(struct Object far *who, struct Object far *obj, char how);
void far UseLight(struct Object far *obj, char how);
void far UseWand(struct Object far *wand, char how);
int far UseFood(struct Object far *who, struct Object far *food, char how);
void far CloseDoor(struct Object far *door);
char far decode_obj_spell(struct Object far *obj, int16 *major, int16 *effect, char *flag);
void far useNSpellCharges(struct Object far *obj);
void far UseBonesOn(struct Object far *obj, char how);
void far TybalsOrb_seg040_A12(struct Object far *obj, char how, char other);
char far seg040_352B_9EA(struct Object far *npc, int div);
void far seg040_352B_AFF(struct Object far *obj, char how);
void far SpikeDoor_seg040_662(struct Object far *door);
void far SpikeDoor_seg040_352B_6F9(struct Object far *obj, char how);
void far UseOilOn(struct Object far *obj, char how, char other);
void far UseKey(struct Object far *obj, char how);
void far UseRockHammerOn(struct Object far *obj, char how, char other);
void far UseBook(struct Object far *obj, char how);
void far UseAnvilOn(struct Object far *obj, char how, char other);
void far UsePoleOn(struct Object far *obj);
void far moveDoor(struct Object far *door);

/* UW1: player_eat returns a plain char (player.h: unsigned char). */
char far player_eat(int nutrition);

/* UW1: ObjectActorArg is a char; add_animobj takes the tile as chars. */
extern char ObjectActorArg;
int far add_animobj(int index, int len, char a, char x, char y);

/* UW1: not in the headers. */
void far TalkTo(struct Object far *npc);
void far ExplodingBook_ovr107_1259(void);
void far mantra_advance(int n);         /* SKILLS.C defines it (void); this caller pushes 0 */
char far plant_seed(void);
extern int16 dseg_5c99_720C[];

/* UW1's player record, the bytes used here (player.h has UW2's layout). */
struct Player1Use {
    char pad0[0x5E];
    unsigned char moonstone:4;          /* 0x5E */
    unsigned char tree:4;               /* cleared when the silver tree is picked */
    uint16 b5F_0:1;                     /* 0x5F */
    uint16 drawn:1;
    uint16 poison:4;
    uint16 active_spells:4;
    uint16 nrunes:2;                    /* 0x60, bits 2-3 */
    uint16 armageddon:1;
    uint16 orb:1;                       /* bit 5: the orb is destroyed */
    uint16 b60_6:2;
    uint16 incense:2;                   /* 0x61 */
    uint16 shrooms:2;
    uint16 drunk:6;
    uint16 b62_2:1;                     /* 0x62, bit 2: talisman_ok */
    uint16 garamon:1;                   /* bit 3: Garamon buried */
    char pad63[0xB0 - 0x63];
    unsigned char bB0;                  /* 0xB0, the mana restored with the orb */
    char padB1[0xCE - 0xB1];
    uint32 game_clock;                  /* 0xCE */
};
#define PLAYER1 ((struct Player1Use *)player)

/* The next time the player may cast from an object, and a flag that makes
   decode_obj_spell always identify the spell. */
int32 nextSpellTime = 0;
char always_decode = 0;

struct Object far * far UseObj(struct Object far *who, struct Object far *obj, char how)
{
    int minor;
    char trap;
    struct Object far *fount;

    minor = OBJ_MINOR(obj);
    trap = 1;
    if (inplist->mode == 4 && (OBJ_MAJOR(obj) != MAJOR_MISC || OBJ_MINOR(obj) != MINOR_CONTAINER))
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
            else {
                UseWand(obj, how);
                trap = 0;
            }
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
            if (OBJ_ITEM(obj) == 0xE7 && how)
                UseThing(obj, (void (far *)())seg040_352B_AFF);
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
    case MAJOR_ANIMOBJ:
        switch (OBJ_INCLASS(obj)) {
        case 0xF:
            if ((obj->ol.f.owner & 0xF) < 8)
                CloseDoor(obj);
            else
                OpenDoor(who, obj);
            break;
        case 0xA:
            if ((char)using_punt(obj, how, 1)) {
                game_sprint(9);
                obj = place_new(0L, 0x122);
                PLAYER1->tree = 0;
                obj->id = obj->id & 0xDFFF | 0x2000;
                return 0;
            }
            break;
        case 9:
            if (obj->qn.f.next != 0) {
                fount = Obj_PtrTMem(&obj->qn.link);
                if (OBJ_ITEM(fount) == 0x12E)
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
    union Link head;

    if (inv) {
        InvRemoveOneObject(obj);
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

struct Object far * far place_new(struct Object far *obj, register int item)
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

void far UseLockpickOn(struct Object far *obj, char how)
{
    int result;
    int skill;

    if (!how)
        return;
    unforce_mouse_cursor(3);
    CursorObjPtr = 0;
    GameInputMode = 0;
    skill = -player->skills[0x10];
    result = checkLock(ThePlayer, obj, skill);
    switch (result) {
    case 1:
        game_sprint(3);
        break;
    case 0:
        game_sprint(0x78);
        break;
    case 4:
        game_sprint(0x7A);
        break;
    default:
        play_effect_here(0x13, 0x40, 0);
        game_sprint(0x79);
        break;
    }
}

void far UseKeyOn(struct Object far *obj, char how)
{
    int result;

    if (!how)
        return;
    unforce_mouse_cursor(3);
    CursorObjPtr = 0;
    GameInputMode = 0;
    result = checkLock(ThePlayer, obj, OBJ_OWNER(ObjectActing));
    game_sprint(result + 2);
}

void far UseKey(struct Object far *obj, char how)
{
    if (!how)
        return;
    if (OBJ_ITEM(obj) == 0x101)
        UseThing(obj, (void (far *)())UseLockpickOn);
    else if (OBJ_ITEM(obj) < 0x10F)
        UseThing(obj, (void (far *)())UseKeyOn);
}

void far UseThing(struct Object far *obj, void (far *fn)())
{
    char buf[40];

    strcpy(buf, "Use ");
    if (!get_name(buf + strlen(buf), obj, 0, 0))
        strcat(buf, "UNNAMED");
    strcat(buf, " on what?\n");
    scroll_print(buf);
    force_mouse_cursor(OBJ_ITEM(obj));
    CursorObjPtr = obj;
    GameInputMode = 2;
    ObjectActing = obj;
    ObjectActor = fn;
}

/* A spike used on a door (0x140..0x147) sets its owner bit 0 (spiked) and is used up;
   anything else, "You can only spike closed doors." */
void far SpikeDoor_seg040_662(struct Object far *door)
{
    if (OBJ_ITEM(door) < 0x140 || OBJ_ITEM(door) > 0x147)
        game_sprint(0x80);
    else {
        game_sprint(0x81);
        door->ol.f.owner = door->ol.f.owner & 0xFFFE | 1;
        door->ol.f.owner = door->ol.f.owner & 1 | 0x3E;
        using_punt(ObjectActing, 1, 1);
    }
    unforce_mouse_cursor(3);
    CursorObjPtr = 0;
    GameInputMode = 0;
}

void far SpikeDoor_seg040_352B_6F9(struct Object far *obj, char how)
{
    if (!how)
        return;
    UseThing(obj, (void (far *)())SpikeDoor_seg040_662);
}

void far UseBonesOn(struct Object far *obj, char how)
{
    char used;
    struct Object far *o;
    struct Object tmp;

    unforce_mouse_cursor(3);
    CursorObjPtr = 0;
    GameInputMode = 0;
    used = 0;
    if (OBJ_ITEM(obj) == 0x165) {
        if (OBJ_OWNER(ObjectActing) == 0x3E) {
            if (OBJ_ISQUANT(obj) && (obj->ol.f.link & 0x200) && (obj->ol.f.link & 0x1FF) == 0x21) {
                PLAYER1->b62_2 = 1;
                PLAYER1->garamon = 1;
                obj->ol.f.link = 0x222;
                tmp.id = 0;
                SET_ITEM(&tmp, 0x7E);
                tmp.goal_word = tmp.goal_word & 0xFFF0 | 7;
                tmp.attitude_word = tmp.attitude_word & 0x3FFF | 0xC000;
                tmp.whoami = 0x1B;
                TalkTo(&tmp);
                o = Obj_PtrTMem(&Map_GetAddr(0x36, 0x34)->objects);
                if (o != 0)
                    UseTrigger(ThePlayer, 0L, o, 0);
                used = 1;
            } else {
                game_sprint(0x103);
                return;
            }
        } else {
            used = 1;
            game_sprint(0x86);
        }
    }
    if (!used)
        game_sprint(0x84);
    else
        using_punt(ObjectActing, how, 1);
}

void far UsePoleOn(struct Object far *obj)
{
    UsingPole = 0;
    FixPlayerEquips();
    if (OBJ_CLASS(obj) == CLASS_SWITCH) {
        game_sprint(0x9D);
        UseObj(ThePlayer, obj, 0);
    } else
        game_sprint(0x9E);
}

void far UseAnvilOn(struct Object far *obj, char how, char other)
{
    if (!how || !other)
        return;
    unforce_mouse_cursor(3);
    CursorObjPtr = 0;
    GameInputMode = 0;
    repair_item(obj, player->skills[0xE], 1);
}

void far UseUtil(struct Object far *obj, char how)
{
    if (OBJ_ITEM(obj) >= 0xC2 && OBJ_ITEM(obj) <= 0xC6) {
        if (how)
            UseThing(obj, (void (far *)())UseBonesOn);
    } else if (OBJ_ITEM(obj) == 0xD7)
        UseThing(obj, (void (far *)())UseAnvilOn);
    else if (OBJ_ITEM(obj) == 0xD8) {
        UsingPole = 1;
        FixPlayerEquips();
        UseThing(obj, (void (far *)())UsePoleOn);
    } else if (how && (OBJ_ITEM(obj) == 0xD9 || OBJ_ITEM(obj) == 0xCE || OBJ_ITEM(obj) == 0xCF))
        UseFood(ThePlayer, obj, how);
}

/* A gronk_whoami callback: divides npc's hit points by div (plus one) and sets bit 9 of its
   attitude word. */
char far seg040_352B_9EA(struct Object far *npc, int div)
{
    npc->hp = npc->hp / div + 1;
    npc->attitude_word = npc->attitude_word & 0xFDFF | 0x200;
    return 0;
}

/* The orb rock used on the orb (0x117): it shatters into an effect and is removed, the
   player record's orb bit is set, mana is restored from byte 0xB0, and every critter of
   whoami 0xE7 is weakened (seg040_352B_9EA). UseUnique calls it directly from the world
   with the user in obj (ObjectActing set to the rock). other is unused but passed. */
void far TybalsOrb_seg040_A12(struct Object far *obj, char how, char other)
{
    if (OBJ_ITEM(obj) != 0x117) {
        if (how)
            game_sprint(0x84);
    } else {
        game_sprint(0x85);
        if (how)
            using_punt(ObjectActing, how, 1);
        put_effect(obj, 4, 5, 0, 0, MapObj_X, MapObj_Y);
        Obj_Punt(&Map_GetAddr(MapObj_X, MapObj_Y)->objects, obj, 1);
        MapObj_X = -1;
        PLAYER1->orb = 1;
        player->max_mana = PLAYER1->bB0;
        player->play_mana = PLAYER1->bB0;
        gronk_whoami(0xE7, 0, 2, (WhoamiFn)seg040_352B_9EA);
    }
    if (CursorObjPtr != 0) {
        unforce_mouse_cursor(3);
        CursorObjPtr = 0;
        GameInputMode = 0;
    }
}

/* Item 0xE7 used on object 0x16E standing on texture class 0xB (dseg_5c99_720C) is used
   up and sets off the object's traps with how 7; anything else, "It seems to have no
   effect." */
void far seg040_352B_AFF(struct Object far *obj, char how)
{
    unforce_mouse_cursor(3);
    CursorObjPtr = 0;
    GameInputMode = 0;
    if (OBJ_ITEM(obj) == 0x16E && (dseg_5c99_720C[obj->ol.f.owner] & 0xFF) == 0xB) {
        using_punt(ObjectActing, how, 1);
        checkTrap(ThePlayer, obj, 7, MapObj_X, MapObj_Y);
    } else
        game_sprint(0x84);
}

void far UseUnique(struct Object far *who, struct Object far *obj, char how)
{
    register int n;

    switch (OBJ_ITEM(obj)) {
    case 0x115:
        n = rand() % 3;
        if (PLAYER1->incense < 3)
            n = 3 - (PLAYER1->incense = PLAYER1->incense + 1);
        show_cutscene(n + 0xB);
        SET_ITEM(obj, 0xD5);
        if (how)
            DisplayInventory();
        else
            editchng(2);
        break;
    case 0x112:
        if (how)
            UseThing(obj, (void (far *)())TybalsOrb_seg040_A12);
        else {
            ObjectActing = obj;
            TybalsOrb_seg040_A12(who, 0, 0);
        }
        break;
    case 0x114:
        if (how)
            ExplodingBook_ovr107_1259();
        break;
    case 0x11B:
        UseFood(who, obj, how);
        break;
    }
}

void far UseLight(struct Object far *obj, char how)
{
    int slot;
    int qty;
    int id;
    int newslot;
    struct Object far *inslot;
    int i;
    int j;

    if (OBJ_ISQUANT(obj) && !(OBJ_LINK(obj) & LINK_SPECIAL))
        qty = OBJ_LINK(obj);
    else
        qty = 1;
    if (!how) {
        game_sprint(0x7B);
        return;
    }
    if (OBJ_QUALITY(obj) == 0) {
        game_sprint(0x7C);
        return;
    }
    slot = FindSlot(obj);
    for (j = 0; j < 4; j++) {
        if (ValidLightSlots[j] == slot && qty == 1)
            break;
    }
    if (j == 4) {
        newslot = 0;
        id = OBJ_ITEM(obj);
        if (id == 0x91 || id == 0x92 || id == 0x90 || id == 0x93) {
            for (i = 5; i <= 8; i++) {
                inslot = AskInventory(i);
                if (inslot == 0 && newslot == 0)
                    newslot = i;
                else if (inslot == obj && qty == 1) {
                    newslot = i;
                    break;
                }
            }
            if (newslot == 0) {
                game_sprint(0xF6);
                return;
            }
            InvRemoveOneObject(obj);
            AddToInventory(obj, newslot);
            DisplayInventory();
            slot = newslot;
        }
    }
    if (OBJ_INCLASS(obj) >= 4)
        SET_INCLASS(obj, OBJ_INCLASS(obj) - 4);
    else
        SET_INCLASS(obj, OBJ_INCLASS(obj) + 4);
    FixPlayerEquips();
    RedisplayInvSlot(slot);
}

void far UseWand(struct Object far *wand, char how)
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
                SET_INCLASS(wand, OBJ_INCLASS(wand) + 4);
                game_sprint(0x7D);
                RedisplayInvSlot(FindSlot(wand));
            }
        }
    }
}

int far UseFood(struct Object far *who, struct Object far *food, char how)
{
    int qty;
    char text[76];
    register int taste;
    register int nutrition;

    taste = 0;
    nutrition = 0xFF;
    if (OBJ_ISQUANT(food) && !(OBJ_LINK(food) & LINK_SPECIAL))
        qty = OBJ_LINK(food);
    else
        qty = 1;
    if (food == CursorObjPtr) {
        if (qty > 1) {
            game_sprint(0x77);
            return -2;
        }
    } else if (!how)
        return -2;
    if (OBJ_CLASS(food) == CLASS_FOOD)
        nutrition = Food[food->id & ID_INCLASS];
    switch (OBJ_ITEM(food)) {
    case 0xBF:
        game_sprint(0x7F);
        return 0;
    case 0xB8:
        if (skill_check(playerdat->attr[2], 20))
            restore_mana(ThePlayer, -(rand() * 3L / 0x8000L));
        if (PLAYER1->shrooms < 3)
            PLAYER1->shrooms = PLAYER1->shrooms + 1;
        FixPlayerEquips();
        taste++;
    case 0xB9:
        taste++;
    case 0x92:
        taste++;
    case 0x125:
        taste += 0xE5;
        break;
    case 0xCE:
        taste = 0xEC;
        nutrition = 4;
        break;
    case 0xCF:
        taste = 0xE9;
        nutrition = 0x17;
        break;
    case 0xD9:
        taste = 0xEA;
        nutrition = 4;
        break;
    case 0x11B:
        taste = 0xEB;
        nutrition = 0x40;
        break;
    case 0xBB:
    case 0xBC:
        taste++;
    case 0xBA:
        taste++;
    case 0xBE:
        taste++;
    case 0xBD:
        taste += 0xED;
        break;
    default:
        if (nutrition == 0xFF)
            return -1;
    }
    if (nutrition > 0) {
        if (nutrition != 0xFF && !player_eat(nutrition)) {
            game_sprint(0x7E);
            return 0;
        }
        if (taste == 0) {
            strcpy(text, "That ");
            if (!get_name(text + strlen(text), food, 0, 0))
                strcat(text, "UNNAMED");
            taste = ((int)(rand() * 20L / 0x8000L) + OBJ_QUALITY(food)) >> 4;
            if (taste > 4)
                taste = 4;
            scroll_print(text);
            taste += 0xAC;
        }
        game_sprint(taste);
        if (OBJ_ITEM(food) == 0xB9) {
            if (PLAYER1->poison < 4)
                PLAYER1->poison = 4;
            else if (PLAYER1->poison < 0xD)
                PLAYER1->poison = PLAYER1->poison + 2;
        }
    } else if (nutrition < 0) {
        game_sprint(taste);
        if (nutrition < -1 && nutrition > -0x7F) {
            if (PLAYER1->drunk - nutrition > 0x3F)
                PLAYER1->drunk = 0x3F;
            else
                PLAYER1->drunk = PLAYER1->drunk - nutrition;
            switch (skill_check(playerdat->attr[0], PLAYER1->drunk)) {
            case -1:
                game_sprint(0xF1);
                player_sleep(-2);
                if (ThePlayer->hp != 0) {
                    game_sprint(0xF3);
                    set_effect(0x40, PLAYER1->drunk / 6 + 10);
                }
                break;
            case 0:
                set_effect(0x40, PLAYER1->drunk / 6);
                break;
            case 2:
                game_sprint(0xF2);
                restore_hp(ThePlayer, -2);
                break;
            }
        }
    } else
        game_sprint(taste);
    checkSpell(MapObj_X, MapObj_Y, who, food, 1);
    checkTrap(who, food, 4, MapObj_X, MapObj_Y);
    if ((char)using_punt(food, how, 1) && GameInputMode == 1)
        CursorObjPtr = 0;
    return 1;
}

void far UseRockHammerOn(struct Object far *obj, char how, char other)
{
    int id;
    struct Tile far *tile;
    struct Object far *rock;
    register int n;
    register int count;

    unforce_mouse_cursor(3);
    CursorObjPtr = 0;
    GameInputMode = 0;
    if (!how || other)
        return;
    if (Obj_Find(&ThePlayer->ol.link, 1, Obj_MemTPtr(obj)) != 0)
        return;
    id = OBJ_ITEM(obj);
    if (id >= 0x153 && id <= 0x156) {
        game_sprint(0x87);
        tile = Map_GetAddr(MapObj_X, MapObj_Y);
        for (count = (int)(rand() * 2L / 0x8000L) + (0x155 - id) + 1; count > 0; count--) {
            rock = CreateObj(1, 0);
            if (rock == 0)
                break;
            *(struct StaticObj far *)rock = *(struct StaticObj far *)obj;
            n = id + (int)(rand() * 2L / 0x8000L) + 1;
            if (n > 0x156)
                n = 0x10;
            SET_ITEM(rock, n);
            if (n == 0x10) {
                SET_ISQUANT(rock, 1);
                rock->ol.f.link = rand() % 6 + 3;
            }
            put_at((MapObj_X << 3) + OBJ_FINEX(obj), (MapObj_Y << 3) + OBJ_FINEY(obj),
                   obj->pos & POS_Z, rock, 6, 0);
        }
        Obj_Punt(&tile->objects, obj, 1);
        editchng(2);
    } else
        game_sprint(0x84);
}

void far UseOilOn(struct Object far *obj, char how, char other)
{
    register int id;
    register int off;

    id = OBJ_ITEM(obj);
    off = (id == 0x90 || id == 0x94) ? 0 : 4;
    unforce_mouse_cursor(3);
    CursorObjPtr = 0;
    GameInputMode = 0;
    if (!how || !other)
        return;
    if (id >= 0xCC && id <= 0xCD) {
        game_sprint(0xB5);
        using_punt(ObjectActing, how, 1);
        SET_ITEM(obj, 0x91);
        RedisplayInvSlot(FindSlot(obj));
    } else if (id == 0x90 || id == 0x91) {
        if (OBJ_QUALITY(obj) == 0x3F)
            game_sprint(off + 0xB4);
        else {
            if (OBJ_QUALITY(obj) < 0x20)
                obj->qn.f.quality = OBJ_QUALITY(obj) + 0x20;
            else
                obj->qn.f.quality = 0x3F;
            game_sprint(off + 0xB3);
            using_punt(ObjectActing, how, 1);
        }
    } else if (id == 0x94 || id == 0x95)
        game_sprint(off + 0xB2);
    else
        game_sprint(0xB1);
}

void far UseMagic(struct Object far *who, struct Object far *obj, char how)
{
    struct Object far *o;
    int16 where;
    register int msg;
    register int found;

    if (how) {
        switch (OBJ_ITEM(obj)) {
        case 0x121:
            if (inplist->mode == 1)
                player_sleep(1);
            break;
        case 0x123:
        case 0x124:
            play_instrument(OBJ_ITEM(obj) - 0x123);
            break;
        case 0x125:
            PLAYER1->poison = 0;
            backfire(ThePlayer, 2);
            using_punt(obj, how, 1);
            game_sprint(0xE0);
            break;
        case 0x127:
            SpikeDoor_seg040_352B_6F9(obj, how);
            break;
        case 0x122:
            msg = 9;
            switch (plant_seed()) {
            case 1:
                msg++;
            case -1:
                msg++;
                using_punt(obj, how, 1);
            case 0:
                msg++;
            }
            game_sprint(msg);
            break;
        case 0x128:
            UseThing(obj, (void (far *)())UseRockHammerOn);
            break;
        case 0x12B:
            if (go_fish()) {
                o = place_new(0L, 0xB6);
                o->qn.f.quality = 0x3F;
            }
            mouse_release(1);
            break;
        case 0x12D:
            UseThing(obj, (void (far *)())UseOilOn);
            break;
        }
    } else {
        switch (OBJ_ITEM(obj)) {
        case 0x12E:
            {
                /* match: block-scoped, sharing slots with the zanium case's */
                int16 spell;
                int16 power;
                char flag;

                if (decode_obj_spell(obj, &spell, &power, &flag)) {
                    inanimate_spell(MapObj_X, MapObj_Y, obj, who, spell, power);
                    if (spell == 4)
                        game_sprint(0xF9);
                    else
                        game_sprint(0xED);
                } else
                    game_sprint(0xED);
            }
            break;
        case 0x129:
            if (who == ThePlayer) {
                int major = 4;
                int minor = 2;
                int cls = 9;

                if (CursorObjPtr != 0 && OBJ_ITEM(CursorObjPtr) == 0x129) {
                    o = CursorObjPtr;
                    found = 0;
                } else if ((o = FindObj(major, minor, cls, 2, &where)) != 0)
                    found = 1;
                else if ((o = FindObj(major, minor, cls, 4, &where)) != 0)
                    found = 2;
                else
                    o = 0;
                if (o != 0) {
                    o->ol.f.link = o->ol.f.link + obj->ol.f.link;
                    switch (found) {
                    case 1:
                        RedisplayInvSlot(where);
                        break;
                    case 2:
                        DisplayOpenBag();
                        break;
                    }
                    using_punt(obj, how, 1);
                }
            }
            break;
        case 0x12F:
            game_sprint(0x112);
            break;
        }
    }
}

void far UseBook(struct Object far *obj, char how)
{
    char far *str;
    char text[100];

    if (!how)
        return;
    if (OBJ_ITEM(obj) == 0x13B) {
        if (inplist->mode == 1)
            newscr(2);
    } else if (!(obj->id & ID_ENCHANT) || OBJ_MAJOR(obj) == MAJOR_RECT) {
        if (obj->id & ID_FLAG10)
            show_cutscene((OBJ_LINK(obj) & 0x1FF) + 0x100);
        else if ((OBJ_LINK(obj) & 0x1FF) < 0x100) {
            strcpy(text, "You read the ");
            if (!get_name(text + strlen(text), obj, 0, 0))
                strcat(text, "UNNAMED");
            strcat(text, "...\n");
            scroll_print(text);
            str = get_string((OBJ_LINK(obj) & 0x1FF) | STR_BOOKS);
            scroll_print(str);
            scroll_print("\n");
        } else
            make_stew();
    } else {
        checkTrap(ThePlayer, obj, 4, MapObj_X, MapObj_Y);
        checkSpell(MapObj_X, MapObj_Y, ThePlayer, obj, how);
        using_punt(obj, how, 0);
    }
}

void far UseRect(struct Object far *who, struct Object far *obj)
{
    char name[20];
    register int state;

    switch (OBJ_MINOR(obj)) {
    case 0:
        if (OBJ_INCLASS(obj) < 8) {
            if (checkLock(who, obj, 0) == 0) {
                if (OBJ_ITEM(who) == 0x7F) {
                    if (!get_name(name, obj, 0, 0))
                        strcpy(name, "UNNAMED");
                    scroll_print("The ");
                    scroll_print(name);
                    scroll_print(" is locked.\n");
                }
            } else if (who == ThePlayer || !(OBJ_OWNER(obj) & 1))
                OpenDoor(who, obj);
        } else
            CloseDoor(obj);
        break;
    case 2:
        if (OBJ_INCLASS(obj) == 1 || OBJ_INCLASS(obj) == 2) {
            SET_FLAGS(obj, state = OBJ_FLAGS(obj) + 1 & 7);
            editchng(2);
        } else
            RectLook(obj, -1);
        break;
    case 1:
        if (OBJ_INCLASS(obj) == 7)
            mantra_advance(0);
        else if ((OBJ_INCLASS(obj) == 0xB || OBJ_INCLASS(obj) == 0xD) && !OBJ_ISQUANT(obj))
            UseCont(who, obj, 0);
        break;
    case 3:
        state = OBJ_INCLASS(obj);
        play_effect(0x13, (MapObj_X << 3) + 3, (MapObj_Y << 3) + 3, 0);
        state = state + 8 & 0xF;
        SET_INCLASS(obj, state);
        editchng(2);
        break;
    }
}

/* The lock rules, as UW2's (OBJUSE.C): 0 locked or the attempt failed, 1 no lock, 2 the
   key locked it, 3 unlocked, 4 not locked. Picking needs a skill of 0x1F for z 0xE. */
int far checkLock(struct Object far *who, struct Object far *door, register int key)
{
    struct Object far *lock;
    union Link far *link;

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
                    SET_FLAG9(lock, 1);
                    return 2;
                }
                return 0;
            }
            return 4;
        } else if (key < 0) {
            if ((OBJ_Z(lock) == 0xE && -key < 0x1F) || OBJ_Z(lock) == 0xF
                || skill_check(-key, OBJ_Z(lock) * 3) <= 0)
                return 0;
        } else if (key > 0) {
            if (!(lock->ol.f.link & 0x1FF) || (lock->ol.f.link & 0x1FF) != key)
                return 0;
        } else
            return 0;
        checkTrap(who, door, 6, MapObj_X, MapObj_Y);
        if (!(lock->id & ID_FLAG10)) {
            Obj_Rem(link, lock);
            Obj_Free(lock);
            return 3;
        }
    } else
        return 1;
    SET_FLAG9(lock, 0);
    return 3;
}

char far checkSpell(int x, int y, struct Object far *who, struct Object far *obj, char how)
{
    int16 major;
    int16 effect;
    char flag;
    struct Object far *src;

    if (decode_obj_spell(obj, &major, &effect, &flag) && flag) {
        if (how != 0) {
            if (PLAYER1->game_clock < nextSpellTime) {
                play_effect_here(0x15, 0x40, 0);
                return 0;
            }
            nextSpellTime = PLAYER1->game_clock + 0x2FD;
            src = who;
        } else {
            if (who == ThePlayer && OBJ_ITEM(obj) >= 0x98 && OBJ_ITEM(obj) <= 0x9B)
                return 0;
            src = obj;
        }
        inanimate_spell(x, y, src, who, major, effect);
        useNSpellCharges(obj);
        return 1;
    }
    return 0;
}

void far checkTrap(struct Object far *who, struct Object far *obj, int how, int x, int y)
{
    union Link far *link;
    struct Object far *trap;

    if (OBJ_ISQUANT(obj) || obj->ol.f.link == 0)
        return;
    link = &obj->ol.link;
    trap = Obj_InList(&link, 0, MAJOR_TRAP, -1, -1);
    if (trap != 0) {
        if (OBJ_MINOR(trap) >= 2)
            UseTrigger(who, obj, trap, how);
        else if (OBJ_FLAGS(trap) == 0 && how == 4) {
            SetOffTrap(who, obj, trap, x, y);
            delete_trap(link, trap);
        }
    }
}

void far moveDoor(struct Object far *door)
{
    register int len = 5;

    if ((OBJ_INCLASS(door) & 7) == 6)
        len = 4;
    door->ol.f.owner = OBJ_INMAJOR(door);
    SET_ITEM(door, 0x1CF);
    add_animobj(Obj_MemTPtr(door), len, 0, MapObj_X, MapObj_Y);
}

void far changeDoor(struct Object far *door)
{
    register int cur;
    register int len = 5;

    if (OBJ_MAJOR(door) == MAJOR_RECT && (OBJ_INCLASS(door) & 7) == 6
        || OBJ_MAJOR(door) == MAJOR_ANIMOBJ && (OBJ_OWNER(door) & 7) == 6)
        len = 4;
    if (OBJ_FLAGS(door) & 8)
        SET_FLAGS(door, OBJ_FLAGS(door) & 7);
    else
        SET_FLAGS(door, (OBJ_FLAGS(door) & 7) + 8);
    cur = get_animlen(door);
    if (cur >= 0)
        set_animlen(door, len - cur);
}

void far OpenDoor(struct Object far *who, struct Object far *door)
{
    unsigned char type;
    register int state;

    if (OBJ_ITEM(door) == 0x1CF) {
        state = (OBJ_OWNER(door) >> 0) & 0xF;
        if (state < 8)
            return;
        door->ol.f.owner = state - 8;
        changeDoor(door);
    } else if (OBJ_INCLASS(door) < 8) {
        door->ol.f.owner = OBJ_OWNER(door) & 0xFFFE;
        if (OBJ_INCLASS(door) != 6)
            SET_Z(door, OBJ_Z(door) + 0x18);
        checkTrap(who, door, 7, MapObj_X, MapObj_Y);
        moveDoor(door);
    } else
        return;
    if ((OBJ_INCLASS(door) & 7) == 6)
        type = 0x14;
    else
        type = 0xB;
    play_effect(type, (MapObj_X << 3) + OBJ_FINEX(door),
                (MapObj_Y << 3) + OBJ_FINEY(door), 0);
}

void far CloseDoor(struct Object far *door)
{
    unsigned char type;
    register int state;

    if (OBJ_ITEM(door) == 0x1CF) {
        state = (OBJ_OWNER(door) >> 0) & 0xF;
        if (state >= 8)
            return;
        door->ol.f.owner = state + 8;
        changeDoor(door);
    } else {
        if (OBJ_INCLASS(door) < 8)
            return;
        moveDoor(door);
    }
    if ((OBJ_INCLASS(door) & 7) == 6)
        type = 0x14;
    else
        type = 0xB;
    play_effect(type, (MapObj_X << 3) + OBJ_FINEX(door),
                (MapObj_Y << 3) + OBJ_FINEY(door), 0);
}

void far ToggleDoor(struct Object far *who, struct Object far *door)
{
    if (OBJ_INCLASS(door) < 8)
        OpenDoor(who, door);
    else
        CloseDoor(door);
}

void far DumpTheBag(struct Object far *bag, char to_player)
{
    char text[80];
    register int owner = 0;

    if (ComObjData[OBJ_ITEM(bag)].can_own)
        owner = OBJ_OWNER(bag);
    if (!drop_link_chain(bag, owner) && to_player) {
        strcpy(text, "The ");
        get_name(text + strlen(text), bag, 0, 0);
        strcat(text, " is empty.\n");
        scroll_print(text);
    }
    editchng(2);
}

void far UseCont(struct Object far *who, struct Object far *obj, char how)
{
    char name[20];

    if (checkLock(who, obj, 0) == 0) {
        if (!get_name(name, obj, 0, 0))
            strcpy(name, "UNNAMED");
        scroll_print("The ");
        scroll_print(name);
        scroll_print(" is locked.\n");
    } else if (how)
        OpenTheBag(FindSlot(obj));
    else
        DumpTheBag(obj, who == ThePlayer);
}

void far BlastFunction(void)
{
    release_missile(ObjectActing, ObjectActorArg);
    GameInputMode = 0;
    unforce_mouse_cursor(3);
    mouse_release(1);
}

char far decode_obj_spell(struct Object far *obj, register int16 *major, register int16 *effect, char *flag)
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
            (int)(((int32)rand() * 10) / 0x8000L) < 4)
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

void far remove_spell(struct Object far *obj)
{
    if (OBJ_ISQUANT(obj) && (obj->id & ID_ENCHANT) && OBJ_MAJOR(obj) != MAJOR_RECT)
        SET_ENCHANTED(obj, 0);
}

void far useNSpellCharges(struct Object far *obj)
{
    union Link far *link;
    struct Object far *spell;

    if (OBJ_ISQUANT(obj) || obj->ol.f.link == 0)
        return;
    link = &obj->ol.link;
    spell = Obj_InList(&link, 0, MAJOR_SPEC, 2, 0);
    if (spell != 0 && (spell->id & ID_FLAG11)) {
        if (spell->qn.f.quality > 0)
            spell->qn.f.quality = spell->qn.f.quality - 1;
        else if ((int)(((int32)rand() * 10) / 0x8000L) < 4) {
            Obj_Rem(link, spell);
            Obj_Free(spell);
        }
    }
}
