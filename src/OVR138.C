/* target: ovr138 */
/* opts: -mm -1 -G -O -Y -d */
/* Using objects: bones, the watch, crystals, gems, poles, anvils, food, oil, books, lockpicks,
   keys, containers, lights, doors, runes and the rest: the whole of DOS overlay ovr138, in
   original order. Function and global names are the originals from the FM Towns symbol
   table; the source file's own name is not known. */

#include <string.h>
#include <stdlib.h>
#include "combat.h"
#include "critter.h"
#include "event.h"
#include "inv.h"
#include "map.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"

#define OBJ_ID(o)       ((o)->id & 0x1FF)
#define OBJ_INDEX(o)    (((o)->id & 0x3F) >> 0)
#define OBJ_TYPE(o)     ((o)->id & 0xF)
#define DOOR_STATE(o)   (((o)->id & 0x1E00) >> 9)
#define SET_DOOR_STATE(o, v)    ((o)->id = (o)->id & 0xE1FF | ((v) & 0xF) << 9)
#define OBJ_MAJOR(o)    (((o)->id & 0x1C0) >> 6)
#define OBJ_CLASS(o)    (((o)->id & 0x1F0) >> 4)
#define OBJ_MINOR(o)    (((o)->id & 0x30) >> 4)
#define OBJ_ISQUANT(o)  (((o)->id & 0x8000) >> 15)
#define OBJ_LINK(o)     ((o)->ol.f.link)
#define OBJ_QUALITY(o)  ((o)->qn.f.quality)
#define OBJ_OWNER(o)    ((o)->ol.f.owner)

extern char UsingPole;
extern struct Player PlayerDat;
extern struct Inplist near *inplist;
extern signed char Food[];
extern long nextSpellTime;
extern char ValidLightSlots[];
/* This file's _BSS, DS:8184 (ovr137's ends there): of the files before ovr140's TxmTerr,
   only this one uses it (seg044 does too). */
char door_type;

void far scroll_print(char far *s);
char far using_punt(struct Object far *obj, char a, int b);
unsigned char far player_eat(int nutrition);
void far set_effect(int which, char amount);
void far get_name(char far *buf, struct Object far *obj, int a, int b);
struct Object far * far CreateObj(int id, int b);
void far update_map_scraps(int scrap, int owner, char link);
void far show_cutscene(int n);
int far add_animobj(int index, int len, int a, char x, char y);
void far play_effect(char type, int x, int y, int a);
void far damage_item(struct Object far *who, void far *source, int a, int b,
                     unsigned char damage, int type);
void far flip_switch(struct Object far *obj, int how);
void far mouse_release(int n);
char far decode_obj_spell(struct Object far *obj, int *spell, int *power, char *flag);
struct Object far * far Obj_Find(unsigned far *head, int a, int index);
void far Obj_Punt(unsigned far *head, struct Object far *obj, int how);
unsigned char far put_at(int x, int y, int z, struct Object far *obj, int a, int b);

/* Later in this file. */
void far play_effect_here(unsigned char a, int b, int c);
void far UseObj(struct Object far *who, struct Object far *obj, int how);
void far repair_item(struct Object far *obj, int skill, int how);

void far UseBonesOn(struct Object far *obj, char how)
{
    unsigned char used;

    unforce_mouse_cursor(3);
    CursorObjPtr = 0;
    GameInputMode = 0;
    used = 0;
    if (OBJ_ID(obj) == 0x165)
    {
        used = 1;
        game_sprint(0x94);
    }
    if (!used)
        game_sprint(0x92);
    else
        using_punt(ObjectActing, how, 1);
}

void far UseWatch(void)
{
    char hours;
    char minutes;
    char text[50];
    register int i;

    hours = player->game_clock / 0xE1000L % 12;
    if (hours == 0)
        hours = 12;
    minutes = player->game_clock / 0x3C00L % 60;
    str_copy(text, get_string(0x227));
    i = strlen(text);
    if (hours > 9)
        text[i++] = hours / 10 + '0';
    text[i++] = hours % 10 + '0';
    text[i++] = ':';
    text[i++] = minutes / 10 + '0';
    text[i++] = minutes % 10 + '0';
    text[i++] = 0;
    scroll_print(text);
    game_sprint(0x60);
}

void far UseCrystal(int quality)
{
    char label[6];
    int i;
    int v;

    game_sprint(0x160);
    v = (0x49 - quality) * 2;
    for (i = 0; i < 4; i++)
    {
        label[i] = (i & 1) ? v % 10 + '0' : v % 26 + 'A';
        v += quality + 0x1E;
    }
    label[4] = '\n';
    label[5] = 0;
    scroll_print(label);
}

void far UseKeyGem(struct Object far *obj, char how, unsigned char other)
{
    int gem;

    gem = (ObjectActing->id & 0xF) - 7;
    unforce_mouse_cursor(3);
    CursorObjPtr = 0;
    GameInputMode = 0;
    if (how != 0 && !other && OBJ_ID(obj) == 0x168 && OBJ_OWNER(ObjectActing))
    {
        if (gem == 4)
            player->xclock[1]++;
        do_sfx(4, player->xclock[2] << 2);
        game_sprint(0x152);
        player->xclock[2]++;
        game_sprint(player->xclock[2] + 0x152);
        if ((gem & 6) == 6)
            gem = 13 - gem;
        player->quest_bytes[2] |= 1 << (gem - 1);
        player->vars[6] = gem - 1;
        using_punt(ObjectActing, how, 1);
        play_effect_here(0x12, 0x40, 0x28);
        if (player->quest_bytes[2] != 0xFF)
            play_effect_here(0x2A, 0x40, 0x14);
        else
            play_effect_here(0x2C, 0x40, 0x14);
    }
    else
        game_sprint(0x15B);
}

void far UsePoleOn(struct Object far *obj)
{
    UsingPole = 0;
    FixPlayerEquips();
    if (OBJ_CLASS(obj) == 0x17)
    {
        game_sprint(0xAB);
        UseObj(ThePlayer, obj, 0);
    }
    else
        game_sprint(0xAC);
}

void far UseAnvilOn(struct Object far *obj, unsigned char how, unsigned char other)
{
    if (!how || !other)
        return;
    unforce_mouse_cursor(3);
    CursorObjPtr = 0;
    GameInputMode = 0;
    repair_item(obj, player->skills[14], 1);
}

int far UseFood(struct Object far *who, struct Object far *food, unsigned char how)
{
    int qty;
    int slot;
    int leftover;
    int msg;
    unsigned char is_potion;
    int sfx;
    char text[76];
    int nutrition;
    int taste;

    taste = 0;
    nutrition = 0xFF;
    is_potion = 0;
    if (OBJ_ISQUANT(food) && !(OBJ_LINK(food) & 0x200))
        qty = OBJ_LINK(food);
    else
        qty = 1;
    if (food == CursorObjPtr)
    {
        msg = -1;
        if (qty > 1)
            msg = 0x84;
        else if (inplist->mode != 1)
            msg = 0x16B;
        if (msg != -1)
            return -2;
    }
    else if (!how)
        return -2;
    if (OBJ_CLASS(food) == 0xB)
        nutrition = Food[food->id & 0xF];
    switch (OBJ_ID(food))
    {
    case 0xB9:
        if (skill_check(playerdat->attr[2], 20))
            restore_mana(ThePlayer, -(rand() * 3L / 0x8000L));
        if (player->shrooms < 3)
            player->shrooms = player->shrooms + 1;
        FixPlayerEquips();
        taste++;
    case 0x110:
        taste++;
    case 0x92:
        taste++;
    case 0x125:
        taste += 0xF4;
        break;
    case 0xCE:
        taste = 0xFB;
        nutrition = 4;
        break;
    case 0xCF:
    case 0x114:
        taste = 0xF8;
        nutrition = 0x17;
        break;
    case 0xE1:
    case 0xE2:
    case 0xE3:
    case 0xE4:
    case 0xE5:
    case 0xE6:
    case 0xE7:
        is_potion = 1;
        taste++;
    case 0xBB:
        taste++;
    case 0xBD:
        taste++;
    case 0xBC:
        taste += 0xFC;
        break;
    default:
        if (nutrition == 0xFF)
            return -1;
    }
    if (nutrition > 0)
    {
        if (nutrition != 0xFF && !player_eat(nutrition))
        {
            game_sprint(0x8C);
            return 0;
        }
        if (!is_potion)
        {
            if (player->hunger >= 0xC0)
                sfx = 0x25;
            else if (player->hunger > 0x5A)
                sfx = 0x1F;
            else
                sfx = 0x21;
        }
        else
            sfx = 0x1E;
        play_effect_here(sfx, 0x40, 0);
        if (taste == 0)
        {
            strcpy(text, "That ");
            get_name(text + strlen(text), food, 0, 0);
            taste = ((int)(rand() * 20L / 0x8000L) + OBJ_QUALITY(food)) >> 4;
            if (taste > 4)
                taste = 4;
            scroll_print(text);
            taste += 0xBB;
        }
        game_sprint(taste);
        if (OBJ_ID(food) == 0x114 && !player->in_void)
            player->sleepbits = rand() % 4 + 2;
    }
    else if (nutrition < 0)
    {
        play_effect_here(0x1E, 0x40, 0);
        game_sprint(taste);
        if (nutrition < -1 && nutrition > -0x7F)
        {
            if (player->drunk - nutrition > 0x3F)
                player->drunk = 0x3F;
            else
                player->drunk = player->drunk - nutrition;
            switch (skill_check(playerdat->attr[0], player->drunk))
            {
            case -1:
                game_sprint(0x100);
                player_sleep(-2);
                if (ThePlayer->hp != 0)
                {
                    game_sprint(0x102);
                    set_effect(0x40, player->drunk / 6 + 10);
                }
                break;
            case 0:
                set_effect(0x40, player->drunk / 6);
                break;
            case 2:
                game_sprint(0x101);
                restore_hp(ThePlayer, -2);
                break;
            }
        }
    }
    else
        game_sprint(taste);
    switch (OBJ_ID(food))
    {
    case 0xBE:
        leftover = 0x13E;
        break;
    case 0xB0:
        leftover = 0xC5;
        break;
    case 0xBA:
        leftover = 0xD2;
        break;
    case 0xBB:
    case 0xBC:
    case 0xBD:
        leftover = 0x13D;
        break;
    default:
        leftover = 0;
    }
    nextSpellTime = 0;
    checkSpell(MapObj_X, MapObj_Y, who, food, 1);
    nextSpellTime = 0;
    checkTrap(who, food, 4, MapObj_X, MapObj_Y);
    if (leftover > 0 && GameInputMode != 1 && qty == 1)
    {
        slot = FindSlot(food);
        PlayerDat.weight -= ComObjData[OBJ_ID(food)].mass;
        food->id = food->id & 0xFE00 | leftover & 0x1FF;
        PlayerDat.weight += ComObjData[OBJ_ID(food)].mass;
        displayEnc(0);
        leftover = 0;
        RedisplayInvSlot(slot);
    }
    else if (using_punt(food, how, 1) && GameInputMode == 1)
        CursorObjPtr = 0;
    if (leftover > 0)
    {
        CursorObjPtr = CreateObj(leftover, 0);
        if (CursorObjPtr != 0)
        {
            unforce_mouse_cursor(3);
            force_mouse_cursor(leftover);
            GameInputMode = 1;
        }
    }
    return 1;
}

void far UseOilOn(struct Object far *obj, unsigned char how, unsigned char other)
{
    int off;
    int id;

    id = OBJ_ID(obj);
    off = (id == 0x90 || id == 0x94) ? 0 : 4;
    unforce_mouse_cursor(3);
    CursorObjPtr = 0;
    GameInputMode = 0;
    if (!how || !other)
        return;
    if (id >= 0xCC && id <= 0xCD)
    {
        if (OBJ_ISQUANT(obj) && !(OBJ_LINK(obj) & 0x200) && OBJ_LINK(obj) > 1)
        {
            InvRemoveOneObject(obj);
            CursorObjPtr = obj;
            force_mouse_cursor(id);
            GameInputMode = 1;
        }
        game_sprint(0xC4);
        using_punt(ObjectActing, how, 1);
        if (CursorObjPtr == obj)
        {
            PlayerDat.weight -= ComObjData[OBJ_ID(obj)].mass;
            obj->id = obj->id & 0xFE00 | 0x91;
            PlayerDat.weight += ComObjData[OBJ_ID(obj)].mass;
        }
        else
            obj->id = obj->id & 0xFE00 | 0x91;
        obj->qn.f.quality = 0x28;
        displayEnc(0);
        RedisplayInvSlot(FindSlot(obj));
    }
    else if (id == 0x90 || id == 0x91)
    {
        if (OBJ_QUALITY(obj) == 0x3F)
            game_sprint(off + 0xC3);
        else
        {
            if (OBJ_ISQUANT(obj) && !(OBJ_LINK(obj) & 0x200) && OBJ_LINK(obj) > 1)
            {
                InvRemoveOneObject(obj);
                CursorObjPtr = obj;
                force_mouse_cursor(id);
                GameInputMode = 1;
            }
            if (OBJ_QUALITY(obj) < 0x20)
                obj->qn.f.quality = OBJ_QUALITY(obj) + 0x20;
            else
                obj->qn.f.quality = 0x3F;
            game_sprint(off + 0xC2);
            using_punt(ObjectActing, how, 1);
        }
    }
    else if (id == 0x94 || id == 0x95)
        game_sprint(off + 0xC1);
    else
        game_sprint(0xC0);
}

void far UseBook(struct Object far *obj, unsigned char how)
{
    char far *str;
    int where;
    int owner;
    char link;
    char text[100];
    int scrap;

    if (!how)
        return;
    if ((obj->id & 0xF) > 0xA)
        return;
    if (OBJ_ID(obj) == 0x13A)
    {
        game_sprint(0x8E);
        if (inplist->mode == 1)
            newscr(2);
    }
    else if (OBJ_ID(obj) == 0x139)
    {
        if (FindObj(4, 3, 0xA, 4, &where))
        {
            scrap = OBJ_QUALITY(obj) + 0x47;
            owner = OBJ_OWNER(obj);
            link = OBJ_LINK(obj) & 0xFF;
            if (link == 0)
            {
                if (inplist->mode == 1)
                {
                    player->map_scrap = ~owner;
                    newscr(2);
                }
            }
            else
            {
                obj->ol.f.link = 0x200;
                game_sprint(9);
                update_map_scraps(scrap, owner, link);
            }
        }
        else
            game_sprint(0xA);
    }
    else if (!(obj->id & 0x1000) || OBJ_MAJOR(obj) == 5)
    {
        if (obj->id & 0x400)
            show_cutscene((OBJ_LINK(obj) & 0x1FF) + 0x100);
        else if ((OBJ_LINK(obj) & 0x1FF) < 0x100)
        {
            strcpy(text, "You read the ");
            get_name(text + strlen(text), obj, 0, 0);
            strcat(text, "...\n");
            scroll_print(text);
            if ((OBJ_LINK(obj) & 0x1FF) == 6)
                player->quests[26] = (player->quests[26] & 0xFFFFFFFBL) + 4;
            str = get_string((OBJ_LINK(obj) & 0x1FF) | 0x600);
            scroll_print(str);
            scroll_print("\n");
        }
        else
            make_stew();
    }
    else
    {
        checkTrap(ThePlayer, obj, 4, MapObj_X, MapObj_Y);
        if (checkSpell(MapObj_X, MapObj_Y, ThePlayer, obj, how))
            using_punt(obj, how, 0);
    }
}

void far UseLockpickOn(struct Object far *obj, unsigned char how)
{
    int result;
    int skill;

    if (!how)
        return;
    unforce_mouse_cursor(3);
    CursorObjPtr = 0;
    GameInputMode = 0;
    skill = -(player->skills[16] + 1);
    result = checkLock(ThePlayer, obj, skill);
    if (result == 5 && skill_check(player->dexterity, 20) > 0 || result == 4 && skill == -1)
        result = 0;
    switch (result)
    {
    case 1:
        game_sprint(3);
        break;
    case 5:
        using_punt(ObjectActing, 1, 1);
        game_sprint(0x86);
        break;
    case 0:
        game_sprint(0x85);
        break;
    case 4:
        game_sprint(0x88);
        break;
    default:
        play_effect_here(0x13, 0x40, 0);
        game_sprint(0x87);
        break;
    }
}

void far UseKeyOn(struct Object far *obj, unsigned char how)
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

void far UseCont(struct Object far *who, struct Object far *obj, char how)
{
    char name[20];

    if (checkLock(who, obj, 0) == 0)
    {
        get_name(name, obj, 0, 0);
        scroll_print("The ");
        scroll_print(name);
        game_sprint(0x170);
    }
    else if (how)
        OpenTheBag(FindSlot(obj));
    else
        DumpTheBag(obj, who == ThePlayer);
}

void far UseLight(struct Object far *obj, unsigned char how)
{
    int slot;
    int qty;
    int id;
    int newslot;
    struct Object far *inslot;
    int i;
    int j;

    if (OBJ_ISQUANT(obj) && !(OBJ_LINK(obj) & 0x200))
        qty = OBJ_LINK(obj);
    else
        qty = 1;
    if (!how)
    {
        game_sprint(0x89);
        return;
    }
    if (OBJ_QUALITY(obj) <= 1)
    {
        game_sprint(0x8A);
        return;
    }
    slot = FindSlot(obj);
    for (j = 0; j < 4; j++)
    {
        if (ValidLightSlots[j] == slot && qty == 1)
            break;
    }
    if (j == 4)
    {
        newslot = 0;
        id = OBJ_ID(obj);
        if (id == 0x91 || id == 0x92 || id == 0x90 || id == 0x93)
        {
            for (i = 5; i <= 8; i++)
            {
                inslot = AskInventory(i);
                if (inslot == 0 && newslot == 0)
                    newslot = i;
                else if (inslot == obj && qty == 1)
                {
                    newslot = i;
                    break;
                }
            }
            if (newslot == 0)
            {
                game_sprint(0x105);
                return;
            }
            InvRemoveOneObject(obj);
            AddToInventory(obj, newslot);
            DisplayInventory();
            slot = newslot;
        }
    }
    if ((obj->id & 0xF) >= 4)
        obj->id = obj->id & 0xFFF0 | ((obj->id & 0xF) - 4) & 0xF;
    else
    {
        obj->id = obj->id & 0xFFF0 | ((obj->id & 0xF) + 4) & 0xF;
        play_effect_here(0x20, 0x40, 0);
    }
    FixPlayerEquips();
    RedisplayInvSlot(slot);
}

void far UseUnique(struct Object far *who, struct Object far *obj, unsigned char how)
{
    int n = 0;

    switch (OBJ_ID(obj))
    {
    case 0x116:
        game_sprint(0x8D);
        break;
    case 0x111:
        if (player->xclock[3] >= 6)
            n++;
        if (n)
            play_instrument(2);
        game_sprint(n + 0x161);
        break;
    case 0x114:
        UseFood(ThePlayer, obj, how);
        break;
    default:
        if (OBJ_ID(obj) >= 0x118 && how)
            UseThing(obj, UseKeyGem);
        break;
    }
}

void far moveDoor(struct Object far *door)
{
    int len = 5;

    if ((OBJ_TYPE(door) & 7) == 6)
        len = 4;
    door->ol.f.owner = OBJ_INDEX(door);
    door->id = door->id & 0xFE00 | 0x1CF;
    add_animobj(Obj_MemTPtr(door), len, 0, MapObj_X, MapObj_Y);
}

void far changeDoor(struct Object far *door)
{
    int cur;
    int len = 5;

    if (OBJ_MAJOR(door) == 5 && (OBJ_TYPE(door) & 7) == 6
        || OBJ_MAJOR(door) == 7 && (OBJ_OWNER(door) & 7) == 6)
        len = 4;
    if (DOOR_STATE(door) & 8)
        SET_DOOR_STATE(door, DOOR_STATE(door) & 7);
    else
        SET_DOOR_STATE(door, (DOOR_STATE(door) & 7) + 8);
    checkTrap(0L, door, (DOOR_STATE(door) & 8) ? 9 : 8, XP, YP);
    cur = get_animlen(door);
    if (cur >= 0)
        set_animlen(door, len - cur);
}

void far OpenDoor(struct Object far *who, struct Object far *door)
{
    unsigned char type;
    int state;

    type = OBJ_TYPE(door) & 7;
    if (OBJ_ID(door) == 0x1CF)
    {
        state = (OBJ_OWNER(door) >> 0) & 0xF;
        if (state < 8)
            return;
        door->ol.f.owner = state - 8;
        changeDoor(door);
    }
    else if (OBJ_TYPE(door) < 8)
    {
        door->ol.f.owner = OBJ_OWNER(door) & 0xFFFE;
        if (OBJ_TYPE(door) != 6)
            door->pos = door->pos & 0xFF80 | ((door->pos & 0x7F) + 0x18) & 0x7F;
        checkTrap(who, door, 8, MapObj_X, MapObj_Y);
        moveDoor(door);
    }
    else
        return;
    if (type == 6)
        type = 0x14;
    else
        type = 0xB;
    play_effect(type, (MapObj_X << 3) + ((door->pos & 0xE000) >> 13),
                (MapObj_Y << 3) + ((door->pos & 0x1C00) >> 10), 0);
}

void far CloseDoor(struct Object far *who, struct Object far *door)
{
    unsigned char type;
    int state;

    door_type = OBJ_TYPE(door) & 7;
    if (OBJ_ID(door) == 0x1CF)
    {
        state = (OBJ_OWNER(door) >> 0) & 0xF;
        if (state >= 8)
            return;
        door->ol.f.owner = state + 8;
        changeDoor(door);
    }
    else
    {
        if (OBJ_TYPE(door) < 8)
            return;
        checkTrap(who, door, 9, MapObj_X, MapObj_Y);
        moveDoor(door);
    }
    if (!quick_time)
    {
        type = door_type;
        if (type == 6)
            type = 0x14;
        else
            type = 0xB;
        play_effect(type, (MapObj_X << 3) + ((door->pos & 0xE000) >> 13),
                    (MapObj_Y << 3) + ((door->pos & 0x1C00) >> 10), 0);
    }
}

void far ToggleDoor(struct Object far *who, struct Object far *door)
{
    if (OBJ_TYPE(door) < 8)
        OpenDoor(who, door);
    else
        CloseDoor(who, door);
}

void far DumpTheBag(struct Object far *bag, char to_player)
{
    char dumped;
    char text[80];
    int owner = 0;

    if (ComObjData[OBJ_ID(bag)].can_own)
        owner = OBJ_OWNER(bag);
    dumped = drop_link_chain(bag, owner);
    player_did_bad(owner);
    if (to_player)
    {
        if (dumped)
            strcpy(text, "You empty the ");
        else
            strcpy(text, "The ");
        get_name(text + strlen(text), bag, 0, 0);
        if (dumped)
            strcat(text, ".\n");
        else
            strcat(text, " is empty.\n");
        scroll_print(text);
    }
    editchng(2);
}

void far UseRune(struct Object far *who, struct Object far *rune)
{
    if (OBJ_ID(rune) == 0x19E)
    {
        damage_item(who, rune, MapObj_X, MapObj_Y, rollem(3, 4) + 4, 8);
        rune->id = rune->id & 0xFE00 | 0x1C2;
        if (add_animobj(Obj_MemTPtr(rune), 4, 0, MapObj_X, MapObj_Y) == -1)
            return;
        fireball_effect(rune, MapObj_X, MapObj_Y);
    }
    else
    {
        if (who == ThePlayer)
        {
            player->paralyzed = (rand() & 0xF) + 4;
            PN.vel[0] = PN.vel[1] = PN.speed = 0;
            game_sprint(0x163);
        }
        else
            hit_critter_goal(0xF, 1, ((rand() & 0x3F) << 2) + 0x40, who, MapObj_X, MapObj_Y);
        using_punt(rune, 0, 0);
    }
}

void far UseRect(struct Object far *who, struct Object far *obj)
{
    char name[20];
    register int state;

    switch (OBJ_MINOR(obj))
    {
    case 0:
        if (OBJ_TYPE(obj) < 8)
        {
            if (checkLock(who, obj, 0) == 0)
            {
                if (OBJ_ID(who) == 0x7F)
                {
                    get_name(name, obj, 0, 0);
                    scroll_print("The ");
                    scroll_print(name);
                    game_sprint(0x170);
                    play_effect(0x2D, (MapObj_X << 3) + ((obj->pos & 0xE000) >> 13),
                                (MapObj_Y << 3) + ((obj->pos & 0x1C00) >> 10), 0);
                }
            }
            else if (who == ThePlayer || !(OBJ_OWNER(obj) & 1))
                OpenDoor(who, obj);
        }
        else
            CloseDoor(who, obj);
        break;
    case 2:
        if (OBJ_TYPE(obj) == 1 || OBJ_TYPE(obj) == 2)
        {
            SET_DOOR_STATE(obj, state = DOOR_STATE(obj) + 1 & 7);
            editchng(2);
        }
        else if (OBJ_TYPE(obj) == 7)
            player_sleep(2);
        else
            RectLook(obj, -1);
        break;
    case 1:
        if ((OBJ_TYPE(obj) == 0xB || OBJ_TYPE(obj) == 0xD) && !OBJ_ISQUANT(obj))
        {
            UseCont(who, obj, 0);
            if (ComObjData[OBJ_ID(obj)].can_own)
                player_did_bad(OBJ_OWNER(obj));
        }
        break;
    case 3:
        flip_switch(obj, 3);
        break;
    }
}

void far UseMagic(struct Object far *who, struct Object far *obj, char how)
{
    struct Object far *fish;
    int spell;
    int power;
    char flag;

    if (how)
    {
        switch (OBJ_ID(obj))
        {
        case 0x121:
            if (inplist->mode == 1)
                player_sleep(1);
            break;
        case 0x123:
        case 0x124:
            play_instrument(OBJ_ID(obj) - 0x123);
            break;
        case 0x125:
            if (player->poison > 0)
                game_sprint(0xEF);
            player->poison = 0;
            backfire(ThePlayer, 2);
            using_punt(obj, how, 1);
            break;
        case 0x128:
            UseThing(obj, UseRockHammerOn);
            break;
        case 0x12B:
            if (go_fish())
            {
                fish = place_new(0L, 0xB6);
                fish->qn.f.quality = 0x3F;
            }
            mouse_release(1);
            break;
        case 0x12D:
            UseThing(obj, UseOilOn);
            break;
        }
    }
    else
    {
        switch (OBJ_ID(obj))
        {
        case 0x12E:
            if (OBJ_QUALITY(obj) == 0)
                game_sprint(0x93);
            else if (decode_obj_spell(obj, &spell, &power, &flag))
            {
                inanimate_spell(MapObj_X, MapObj_Y, obj, who, spell, power);
                if (spell == 4)
                    game_sprint(0x108);
                else
                    game_sprint(0xFC);
            }
            else
                game_sprint(0xFC);
            break;
        }
    }
}

void far UseRockHammerOn(struct Object far *obj, unsigned char how, char other)
{
    int z;
    int xoff;
    int yoff;
    int id;
    struct Tile far *tile;
    struct Object far *rock;
    int n;
    int count;

    unforce_mouse_cursor(3);
    CursorObjPtr = 0;
    GameInputMode = 0;
    if (!how || other)
        return;
    if (Obj_Find(&ThePlayer->ol.word, 1, Obj_MemTPtr(obj)) != 0)
        return;
    id = OBJ_ID(obj);
    if (id >= 0x153 && id <= 0x156)
    {
        game_sprint(0x95);
        tile = Map_GetAddr(MapObj_X, MapObj_Y);
        xoff = (obj->pos & 0xE000) >> 13;
        yoff = (obj->pos & 0x1C00) >> 10;
        z = obj->pos & 0x7F;
        Obj_Punt(&tile->objects.word, obj, 1);
        for (count = (int)(rand() * 2L / 0x8000L) + (0x155 - id) + 1; count > 0; count--)
        {
            rock = CreateObj(1, 0);
            if (rock == 0)
                break;
            n = id + (int)(rand() * 2L / 0x8000L) + 1;
            if (n > 0x156)
                n = 0x10;
            rock->id = rock->id & 0xFE00 | n & 0x1FF;
            if (n == 0x10)
            {
                rock->id = rock->id & 0x7FFF | 0x8000;
                rock->ol.f.link = rand() % 6 + 3;
            }
            if (!put_at((MapObj_X << 3) + xoff, (MapObj_Y << 3) + yoff, z, rock, 6, 0))
                ;
        }
        editchng(2);
    }
    else
        game_sprint(0x92);
}

void far UseUtil(struct Object far *obj, char how)
{
    if (OBJ_ID(obj) >= 0xC2 && OBJ_ID(obj) <= 0xC6)
    {
        if (how)
            UseThing(obj, UseBonesOn);
    }
    else if (OBJ_ID(obj) == 0xD7)
        UseThing(obj, UseAnvilOn);
    else if (OBJ_ID(obj) == 0xD8)
    {
        UsingPole = 1;
        FixPlayerEquips();
        UseThing(obj, UsePoleOn);
    }
    else if (how && (OBJ_ID(obj) == 0xCE || OBJ_ID(obj) == 0xCF))
        UseFood(ThePlayer, obj, how);
}
