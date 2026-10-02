/* target: ovr154 */
/* opts: -mm -1 -G -O -Y -d */
/* Skills, levelling, end-game statistics, sleep and dreams, eating, the void, death and
   traps: the whole of DOS overlay ovr154, in original order. Function and global names are
   the originals from the FM Towns symbol table; the source file's own name is not known. */

#include <string.h>
#include <stdlib.h>
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
#include "view3d.h"

#define OBJ_INDEX(o)    (((o)->id & ID_INMAJOR) >> 0)
#define OBJ_MINOR(o)    (((o)->id & ID_MINOR) >> 4)
#define OBJ_ISQUANT(o)  (((o)->id & ID_ISQUANT) >> 15)
#define OBJ_QUALITY(o)  ((o)->qn.f.quality)
#define OBJ_OWNER(o)    ((o)->ol.f.owner)
#define OBJ_LINK(o)     ((o)->ol.f.link)

struct VoidTile { unsigned char x, y; };

extern unsigned long far *Time;
extern unsigned char far *foreground_color;
extern struct FontInfo far *cur_font;
extern struct Inplist near *inplist;
extern void (far *npp_func)(void);

/* Elsewhere in the game. */
void far scroll_print(char far *s);
void far damage_item(struct Object far *who, void far *source, int a, int b,
                     unsigned char damage, int type);
void far load_digi_fx(int which);
void far Killorn_just_crashed(int how);
void far show_cutscene(int n);
void far grfx_clear(void);
void far display_screen(int a, int b);
void far Obj_FindInMap(int major, int minor, int type, int *x, int *y);
void far set_new_music(int n);
struct Object far * far CreateObj(int id, int b);
char far put_at(int x, int y, int z, struct Object far *obj, int a, int b);
struct Object far * far obj_deal(struct Object far *obj, int x, int y, int a);
int far near_mob_put_at(struct Object far *at, struct Object far *obj, int a, int b);
void far do_teleport(struct Object far *who, int x, int y, int level);
struct Object far * far Obj_IntTMem(int index);
void far load_new_music(int n, int m);
void far scroll_clear(int n);
struct Object far * far Obj_InList(unsigned far **head, int a, int major, int minor, int idx);
struct Object far * far Obj_PtrTMem(unsigned far *link);
void far get_name(char far *buf, struct Object far *obj, int a, int b);
void far UseTrigger(struct Object far *user, struct Object far *obj, struct Object far *trigger, int a);

/* Later in this file. */
char far player_eat(int nutrition);

char far use_skill(struct Object far *who, unsigned char skill, unsigned char value)
{
    if (who != ThePlayer)
        return 0;
    switch (skill)
    {
    case 12:
        mdetect(8, value);
        break;
    default:
        scroll_print(get_string((skill + 0x1F) | STR_CHARGEN));
        game_sprint(0x16F);
        return 1;
    case 10:
    case 11:
        break;
    }
    return 1;
}

char far player_use_skill(int skill)
{
    return use_skill(ThePlayer, skill + SKILL_TRAPS, player->skills[skill + SKILL_TRAPS]);
}

void far player_compute(char restore)
{
    int mana;

    playerdat->avghit = 30 + player->level * playerdat->attr[0] / 5;
    mana = (player->skills[SKILL_MANA] + 1) * playerdat->attr[2] >> 3;
    player->max_mana = mana;
    player->max_weight = playerdat->attr[0] * 13 + 300;
    if (restore)
        player->play_mana = player->max_mana;
    restore_mana(ThePlayer, 0);
}

void far advance(char levels)
{
    register char *s = " 0\n";

    player->level = player->level + levels;
    if (player->level >= 10)
        s[0] = player->level / 10 + '0';
    else
        s[0] = ' ';
    s[1] = player->level % 10 + '0';
    game_sprint(0xA1);
    scroll_print(s);
    player->skill_points = player->skill_points + levels;
    player_compute(0);
    panel_check();
}

int far prime(int skill)
{
    if (skill < 7)
        return 0;
    if (skill < 10)
        return 2;
    return 1;
}

void far add_to_skill(int skill)
{
    int rolls;
    int base;
    int divisor;
    int stat;

    if (player->skills[skill] != 0)
    {
        base = 1;
        divisor = 13;
        rolls = 2;
    }
    else
    {
        base = 3;
        divisor = 9;
        rolls = 3;
    }
    stat = playerdat->attr[prime(skill)];
    player->skills[skill] += base;
    player->skills[skill] += stat / divisor;
    player->skills[skill] += (long)rand() * rolls / 0x8000L;
    for (; rolls > 0; rolls--)
        player->skills[skill] += skill_check(stat, 20);
    if (player->skills[skill] > 30)
        player->skills[skill] = 30;
}

/* The divisor for the random bonus when a skill rises, by governing attribute. */
static unsigned char skill_rng[3] = { 25, 30, 15 };

char far get_skill(char skill)
{
    int stat_rng;
    char result = 1;
    int attribute;
    int stat;

    stat = prime(skill);
    stat_rng = skill_rng[stat];
    attribute = playerdat->attr[stat];
    if (attribute * 2 < player->skills[skill] || player->skills[skill] >= 30)
        result = 0;
    else
    {
        player->skills[skill] = player->skills[skill] + 1;
        if (stat != 0 && attribute / 2 > player->skills[skill])
            player->skills[skill] = player->skills[skill] + 1;
        if (player->skills[skill] < attribute
            && rand() % stat_rng < attribute - player->skills[skill])
            player->skills[skill] = player->skills[skill] + 1;
        if (player->skills[skill] > 30)
            player->skills[skill] = 30;
    }
    if (skill == 8)
    {
        clear_all_loretries();
        player->lore[PlayerLevel] = player->skills[SKILL_LORE];
    }
    return result;
}

char far grant_skill_advance(int which)
{
    char tried[4];
    int start;
    int count;
    char result = 0;
    int skill;
    int tries;
    int left;

    switch (which)
    {
    case -1: start = SKILL_ATTACK; count = 7; tries = 3; break;
    case -2: start = SKILL_MANA; count = 3; tries = 2; break;
    case -3: start = SKILL_TRAPS; count = 10; tries = 4; break;
    default: start = which; count = 1; tries = 1; break;
    }
    memset(tried, 0xFF, 4);
    left = count;
    for (left = count; tries > 0 && left > 0; tries--, left--)
    {
        if (start == SKILL_MANA && player->skills[SKILL_MANA] < 8 && (rand() & 2))
            skill = SKILL_MANA;
        else
            skill = start + (int)(((long)rand() * count) / 0x8000L);
        if (get_skill(skill))
            result = 1;
    }
    player_compute(0);
    return result;
}

void far game_stats(void)
{
    char far *str;
    char numbuf[10];
    int y;
    int days;
    int hours;
    int value;
    char text[50];
    register int i;
    register int x;

    grfx_quikfont(FONT_CHAR);
    *foreground_color = *background_color = 0x52;
    str = get_string(player_name_handle);
    y = 0xAB;
    x = 0xA0 - string_width(str) / 2;
    string_to_screen(str, x, y);

    str = str_copy(text, get_string(0x2CA));
    i = strlen(text);
    if (player->level > 9)
        text[i++] = player->level / 10 + '0';
    text[i++] = player->level % 10 + '0';
    text[i++] = ' ';
    text[i++] = 0;
    str_cat(text, get_string((player->pclass + 0x17) | STR_CHARGEN));
    y -= cur_font->height;
    str = get_string(0x2CB);
    x = 0xA0 - (string_width(text) + string_width(str)) / 2;
    string_to_screen(text, x, y);
    x += string_width(text) + 4;
    string_to_screen(str, x, y);

    hours = player->game_clock / 0x1C2000L;
    days = hours / 12;
    str_copy(text, get_string(0x2CC));
    str_cat(text, itoa(days, numbuf, 10));
    str_cat(text, get_string(0x2CD));
    x = 0xA0 - string_width(text) / 2;
    y -= cur_font->height;
    string_to_screen(text, x, y);
    y -= cur_font->height;

    for (i = 0; i < 6; i++)
    {
        x = i / 3 ? 0xBE : 0x50;
        value = i % 3 * cur_font->height;
        str = get_string((i + 0x11) | STR_CHARGEN);
        switch (i)
        {
        case 0:
        case 1:
        case 2:
            itoa(playerdat->attr[i], text, 10);
            break;
        case 3:
            itoa(playerdat->avghit, text, 10);
            break;
        case 4:
            itoa(player->max_mana, text, 10);
            break;
        case 5:
            ltoa(player->exp / 10, text, 10);
            break;
        }
        string_to_screen(str, x, y - value);
        string_to_screen(text, x + 0x2D, y - value);
    }

    y -= cur_font->height * 2;
    for (i = 0; i < 20; i++)
    {
        value = player->skills[i];
        str = get_string((i + 0x1F) | STR_CHARGEN);
        text[0] = value > 9 ? value / 10 + '0' : value + '0';
        text[1] = value > 9 ? value % 10 + '0' : 0;
        text[2] = 0;
        x = i % 3 * 0x4A + 0x32;
        if (i % 3 == 0)
            y -= cur_font->height;
        string_to_screen(str, x, y);
        string_to_screen(text, x + 0x46 - string_width(text), y);
    }

    x = 0;
    for (i = 0; i < 3; i++)
        x += playerdat->attr[i];
    if (x > 0x40)
    {
        text[0] = 0;
        str_cat(text, get_string(0x2CE));
        *foreground_color = *background_color = 0x1F;
        string_to_screen(text, 0x32, 0x14);
    }
}

unsigned char far dream(int sleepfactor)
{
    int found = -1;
    int counter;
    unsigned char xclocks[4] = { 4, 6, 10, 14 };
    unsigned long timer;
    int dreamflags;

    if (player->sleepbits != 0)
    {
        game_sprint(0x18);
        player->quests[12] = (player->quests[12] & 0xFFFFFFFEL) + 1;
        go_void();
        return 0;
    }
    dreamflags = player->dreamflags;
    for (counter = 0; counter < 4; counter++)
    {
        if (player->xclock[XC_CASTLE] >= xclocks[counter])
        {
            if ((dreamflags & (1 << counter)) != (1 << counter))
                found = counter;
        }
    }
    if (found < 0)
    {
        if (rand() % (4 + (sleepfactor << 2)) == 0)
        {
            if (player->sleepbits == 0)
            {
                found = 4 + rand() % 3;
                if ((1 << found) & dreamflags)
                    found = -1;
            }
        }
    }
    if (found >= 0)
    {
        show_cutscene(found + 0x18);
        player->dreamflags ^= 1 << found;
        game_sprint(0x13 - sleepfactor);
        return 1;
    }
    timer = *Time;
    while (timer + 0x180 > *Time)
        ;
    game_sprint(0x13 - sleepfactor);
    return 0;
}

void far drop_drunk_player(void)
{
    unsigned char damage;

    if (player->motion_state & 3)
        damage_item(ThePlayer, 0L, 0, 0, 0xFF, 0);
    FixPlayerEquips();
    finish_player();
    if ((player->motion_state & 8) && (motionbits & 0x16) == 0)
    {
        damage = rand() % 6 * 10 + 12;
        damage_item(ThePlayer, 0L, 0, 0, damage, 0x10);
    }
    if (player->motion_state & 3)
        damage_item(ThePlayer, 0L, 0, 0, 0xFF, 0);
}

void far player_sleep(int how)
{
    int comfort;
    int regen;
    int revived = 0;
    char fade = 1;
    int hours;

    if (how >= 0)
    {
        if ((player->motion_state & 0x1B) || PN.acc[2] != 0)
        {
            game_sprint(0x14);
            return;
        }
        if ((PlayerLevel - 1) / 8 == 8 && player->in_void)
        {
            punt_void();
            return;
        }
        if (hostile_creatures_near() || player->in_pits)
        {
            game_sprint(0xE);
            return;
        }
        if (how == 0 || how > 2)
            game_sprint(0xF);
        else
            game_sprint(how + 0x15);
    }
    render_FB();
    set_random_walking_music(0);
    change_music_maybe();
    fadeout3d(5);
    punt_all_digi_fx();
    load_digi_fx(1);
    load_digi_fx(2);
    load_digi_fx(0xB);
    if (how >= 0)
        game_sprint(0x10);
    DoClosingDoors(0);
    Obj_GarbageCollect(1, 0x14);
    hours = rand() % 5 + 2;
    player->active_spells = 0;
    player->shrooms = 0;
    pass_time(hours * 3600L);
    if ((int)((player->quests[12] & 4) >> 2) && !(int)((player->quests[13] & 4) >> 2))
    {
        Killorn_just_crashed(0);
        damage_item(ThePlayer, 0L, 0, 0, 0xFF, 0);
    }
    DegradeLights(hours * 180, 0);
    if (player->poison)
    {
        comfort = (player->poison + 1) * player->poison >> 1;
        damage_item(ThePlayer, 0L, 0, 0, comfort, 0x10);
        player->poison = 0;
    }
    if (how < 0)
    {
        drop_drunk_player();
        if (player->in_pits)
            damage_item(ThePlayer, 0L, 0, 0, 0xFF, 0);
    }
    if (ThePlayer->hp == 0)
    {
        set_random_walking_music(-1);
        return;
    }
    if (wandering_monster_check())
    {
        if (player->fatigue > 0x18)
            player->fatigue -= 0x18;
        else
            player->fatigue = 0;
        game_sprint(0x15);
        player_eat(-12 - (rand() & 0xF));
        if (player->drunk < 0x10)
            player->drunk = 0;
        else
            player->drunk -= 0x10;
    }
    else
    {
        update_all_critters_whilst_player_snoozes();
        DoWanderingMonsters(0);
        hours = rand() % 4 + 7 - hours;
        if (ThePlayer->hp < 10)
            hours += rand() % 2 + 1;
        pass_time(hours * 3600L);
        DegradeLights(hours * 180, 0);
        comfort = player->hunger > 0x40 && how > 0;
        regen = player->fatigue / 2 + 2;
        if (regen > 5)
            regen = 5;
        player->fatigue = 0;
        if (player->hunger != 0)
        {
            restore_hp(ThePlayer, regen + regen * comfort - 1);
            restore_mana(ThePlayer, -6);
            restore_mana(ThePlayer, regen + (regen + 1) * comfort - 1);
        }
        else
        {
            game_sprint(0x11);
            damage_item(ThePlayer, 0L, 0, 0, 2, 0);
        }
        player_eat(-24 - (rand() & 0x1F));
        if (player->drunk < 0x20)
            player->drunk = 0;
        else
            player->drunk -= 0x20;
        if (ThePlayer->hp == 0 && how >= 0)
        {
            ThePlayer->hp = 1;
            revived = 1;
        }
        if (how >= 0)
            fade = !dream(comfort);
        else
            game_sprint(0x13 - comfort);
        if (revived)
        {
            revived = 0;
            ThePlayer->hp = 0;
        }
    }
    FixPlayerEquips();
    NightCleanCritPages();
    PN.speed = 0;
    PN.acc[0] = PN.acc[1] = PN.acc[2] = 0;
    PN.vel[0] = PN.vel[1] = PN.vel[2] = 0;
    panel_check();
    render_FB();
    set_random_walking_music(-1);
    if (fade)
        fadein3d(5);
    else
        send_FB();
}

void far player_key_sleep(int bedroll)
{
    int where;

    if (FindObj(4, 2, 1, 4, &where) == 0)
        bedroll = 0;
    else
        bedroll = 1;
    player_sleep(bedroll);
}

char far player_eat(int nutrition)
{
    int value;

    value = player->hunger;
    value += nutrition;
    if (value > 0xFF)
        return 0;
    if (value < 0)
        player->hunger = 0;
    else
        player->hunger = value;
    if (nutrition > 0)
    {
        value = player->food_heal / 6;
        if (value > 8)
            value = 8;
        get_hp_back(ThePlayer, value);
        player->food_heal = 0;
    }
    return 1;
}

void far player_won_game(void)
{
    inplist->mode = 0;
    LeftPanel = 2;
    show_cutscene(2);
    mouse_hide();
    grfx_clear();
    display_screen(7, 8);
    while (mouse_get_input() < 0)
        ;
    grSoftPageFlip();
    display_screen(-1, 9);
    game_stats();
    grSoftPageFlip();
    copy_visible_to_hidden();
    while (mouse_get_input() < 0)
        ;
    show_cutscene(10);
    real_death(0);
}

void far cs_check(void)
{
    unsigned char *cs = &player->quest_bytes[15];

    if (*cs > 0)
    {
        if (*cs - 1 == 2)
        {
            change_screen(1);
            strt_demscr();
            player_won_game();
            *cs = 0;
        }
        else
            show_cutscene(*cs - 1);
        *cs = 0;
    }
}

char far moveto(int level, int item)
{
    int x = 0;
    int y = 0;

    npp_func = 0;
    if (level == PlayerLevel)
    {
        Obj_FindInMap(item >> 6, (item & ID_MINOR) >> 4, item & ID_INCLASS, &x, &y);
        NewPlayerX = x;
        NewPlayerY = y;
        return 1;
    }
    return 0;
}

void far do_gem(void)
{
    int x;
    int y;
    int xoff;
    struct Tile far *tile;
    struct Object far *stain;
    int yoff;
    int z;

    npp_func = 0;
    x = 0x1C;
    y = 0x28;
    xoff = rand() % 5 + 1;
    yoff = rand() % 5 + 1;
    move_along(rand(), 8, &x, &y);
    NewPlayerX = x;
    NewPlayerY = y;
    if (playerdat->avghit > 8)
        ThePlayer->hp = playerdat->avghit - 2 - rand() * 3L / 0x8000L;
    else
        ThePlayer->hp = playerdat->avghit;
    player->play_mana = player->max_mana;
    if (player->max_mana > 8)
        player->play_mana -= player->max_mana / 8 + 2;
    ThePlayer->b15 = (ThePlayer->b15 & 0xC0) | 1;
    player->poison = 0;
    player->active_spells = 0;
    FixPlayerEquips();
    set_new_music(10);
    tile = Map_GetAddr(x, y);
    if (tile->type == TILE_OPEN)
    {
        stain = CreateObj(ITEM_BLOOD_STAIN_DF, 0);
        z = tile->height << 3;
        if (put_at((x << 3) + xoff, (y << 3) + yoff, z, stain, 0, 0))
        {
            stain->pos = (stain->pos & 0xFF80) | (z & 0x7F);
            stain->pos = (stain->pos & 0x1FFF) | ((yoff & 7) << 13);
            stain->pos = (stain->pos & 0xE3FF) | ((xoff & 7) << 10);
            obj_deal(stain, x, y, 1);
        }
    }
    player->automap = 1;
}

void far do_dreamret(void)
{
    set_new_music(10);
    NewPlayerX = player->dream_x;
    NewPlayerY = player->dream_y;
    trap_teleport_data = ((player->dream_pos >> 5) + 8) << 2;
    npp_func = 0;
}

void far do_mstone(void)
{
    moveto(area_spell_state, 0x126);
}

void far punt_mouse_obj(void)
{
    if (CursorObjPtr != 0)
    {
        if (GameInputMode == 1 || GameInputMode == 0)
            near_mob_put_at(ThePlayer, CursorObjPtr, 6, 0);
        else if (GameInputMode != 2)
            return;
        CursorObjPtr = 0;
        GameInputMode = 0;
        unforce_mouse_cursor(3);
    }
}

void far punt_void(void)
{
    player->in_void = 0;
    player->sleepbits = 0;
    punt_mouse_obj();
    npp_func = do_dreamret;
    do_teleport(ThePlayer, 0x3F, 0x3F, (player->dream_pos >> 8) & 0xFF);
    player_setup(0, 0, -1);
    game_sprint(0x19);
    editchng(0x7FFE);
}

/* Where the void dream can put the player. */
static struct VoidTile void_tiles[4] = { { 32, 28 }, { 25, 13 }, { 38, 32 }, { 14, 40 } };

void far go_void(void)
{
    int n;
    int x;
    int y;

    player->sleepbits = rand() % 4 + 2;
    player->in_void = 1;
    player->dream_x = (ThePlayer->home & HOME_X) >> 10;
    player->dream_y = (ThePlayer->home & HOME_Y) >> 4;
    player->dream_pos = ((PlayerHeading >> 8) & 0xFF) + (PlayerLevel << 8);
    n = rand() & 7;
    if (n >= 4)
        n = 0;
    x = void_tiles[n].x;
    y = void_tiles[n].y;
    if (n != 0)
    {
        x += rand() & 1;
        y += rand() & 1;
    }
    do_teleport(ThePlayer, x, y, 0x45);
    player_setup(0, 0, -1);
    editchng(0x7FFE);
}

void far player_is_dead(void)
{
    int i;

    if (PlayerLevel == 1 && ThePlayer->last_hit > 0)
    {
        struct Object far *killer = Obj_IntTMem(ThePlayer->last_hit);
        if ((killer->whoami >= 0x81 && killer->whoami <= 0x8F)
            || killer->whoami == 0xA8 || killer->whoami == 0x95)
        {
            if (!((killer->b0A & 0x80) >> 7))
            {
                put_player_in_jail();
                return;
            }
        }
    }
    if (player->in_void)
    {
        punt_void();
        return;
    }
    if (player->in_pits)
    {
        player->in_pits = 0;
        for (i = 0; i < 5; i++)
        {
            if (player->pit_fighters[i] > 0)
            {
                Obj_IntTMem(player->pit_fighters[i])->attitude_word =
                    Obj_IntTMem(player->pit_fighters[i])->attitude_word & 0x3FFF | 0x4000;
                Obj_IntTMem(player->pit_fighters[i])->last_hit = 0;
                Obj_IntTMem(player->pit_fighters[i])->goal_word =
                    Obj_IntTMem(player->pit_fighters[i])->goal_word & 0xFFF0 | 1;
            }
            player->pit_fighters[i] = 0;
        }
        player->quest_bytes[QB_PIT_RECORD] = 0;
        player->quest_bytes[5] = 0;
    }
    kill_all_effects();
    load_new_music(7, 1);
    player_get_exp(-(int)(player->exp / 9));
    render_FB();
    fadeout3d(5);
    clear_fight_state();
    punt_mouse_obj();
    if (PlayerLevel > 8)
    {
        char ok;

        do_teleport(ThePlayer, 0x3F, 0x3F, 5);
        npp_func = do_gem;
        NewPlyFade = 0;
        ok = new_player_pos();
        NewPlyFade = 3;
        if (ok)
        {
            cFillFB(1);
            scroll_clear(1);
            game_sprint(0x169);
        }
        else
            real_death(1);
    }
    else
        real_death(1);
}

int far DetectedTrap(struct Object far *obj, int skill)
{
    unsigned far *head;
    struct Object far *trig;

    if (OBJ_ISQUANT(obj) || OBJ_LINK(obj) == 0)
        return 0;
    for (head = &obj->ol.word; trig = Obj_InList(&head, 0, MAJOR_TRAP, -1, -1), trig;
         head = &trig->qn.word)
    {
        if (OBJ_MINOR(trig) == 3)
            return skill_check(skill, ((PlayerLevel - 1) / 8 << 1) + 10);
    }
    return 0;
}

int far RemoveTrap(struct Object far *obj, int skill)
{
    int quality;
    unsigned far *head;
    struct Object far *trap;
    struct Object far *trig;
    char name[20];
    int result = 0;
    int owner;

    if (OBJ_ISQUANT(obj) || OBJ_LINK(obj) == 0)
        return 0;
    for (head = &obj->ol.word; trig = Obj_InList(&head, 0, MAJOR_TRAP, -1, -1), trig;
         head = &trig->qn.word)
    {
        if (OBJ_MINOR(trig) == 3)
            break;
    }
    if (trig)
    {
        if (OBJ_MINOR(trig) >= 2)
            trap = Obj_PtrTMem(&trig->ol.word);
        else
        {
            trap = trig;
            trig = 0;
        }
        result = skill_check(skill, (PlayerLevel - 1) / 8 + 8);
        if (result > 0)
        {
            if (OBJ_INDEX(trap) == 15)
                strcpy(name, "trap");
            else if (OBJ_INDEX(trap) == 0 && OBJ_OWNER(trap))
                strcpy(name, "poison trap");
            else
                get_name(name, trap, 0, 0);
            scroll_print("The ");
            scroll_print(name);
            scroll_print(" on the ");
            get_name(name, obj, 0, 0);
            scroll_print(name);
            game_sprint(0x168);
            if (trig)
            {
                quality = OBJ_QUALITY(trig);
                owner = OBJ_OWNER(trig);
                delete_trap(&Map_GetAddr(quality, owner)->objects.word, trap);
            }
        }
        else if (result < 0)
        {
            game_sprint(0x166);
            if (OBJ_INDEX(trap) == 15)
                strcpy(name, "trap");
            else if (OBJ_INDEX(trap) == 0 && OBJ_OWNER(trap))
                strcpy(name, "poison trap");
            else
                get_name(name, trap, 0, 0);
            scroll_print(name);
            game_sprint(0x60);
            if (trig)
                UseTrigger(ThePlayer, obj, trig, -1);
            else
            {
                SetOffTrap(ThePlayer, obj, trap, MapObj_X, MapObj_Y);
                delete_trap(head, trap);
            }
        }
        else
            game_sprint(0x167);
    }
    return result;
}
