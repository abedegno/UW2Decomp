/* target: ovr154 */
/* opts: -mm -1 -G -O -Y -d */
/* Skills, levelling, end-game statistics, sleep and dreams, eating, the void, death and
   traps: the whole of DOS overlay ovr154, in original order. Function and global names are
   the originals from the FM Towns symbol table.

   Skills: struct Player's skills[], 0 to 30 each, each governed by an attribute (prime:
   strength for the combat skills 0 to 6, intelligence for mana, lore and casting, 7 to
   9, dexterity for the rest). add_to_skill is character creation's raise, get_skill a
   trainer's or a skill point's raise, grant_skill_advance picks one at random from a
   group. player_compute derives maximum HP, mana and carrying weight; advance gains
   levels (SKILLCHK.C's player_get_exp calls it).
   Sleep: player_sleep (the sleep key, beds and bedrolls in USEITEMS.C, a sleep trap in
   TRIGGER.C, and passing out drunk) passes time, heals, feeds and sobers the player,
   and may show a dream (dream) or send the player to the Ethereal Void (go_void,
   punt_void).
   Death: player_is_dead (INTERACT.C, when HP reaches 0) jails, wakes, or returns the
   player to the blackrock gem, or ends the game (UWEDIT.C's real_death).
   The end: cs_check (after a conversation) plays a pending cutscene, and cutscene 2
   means the game is won (player_won_game, game_stats).
   Traps on objects: DetectedTrap and RemoveTrap (INTERACT.C, and the spells).
   Data: void_tiles, skill_rng.
   Name: descriptive (skills, sleep, dreams, eating, the void and death: use_skill,
   player_sleep); until the renaming this file was PLAYER.C, which is now ovr143. */

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

struct VoidTile { unsigned char x, y; };

/* The use-skill key: only track (12) does anything, running mdetect(8, value) to
   report nearby creatures. Traps (10) and search (11) do nothing here; any other skill
   just prints its name. */
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
        game_sprint(0x16F);             /* "\n" */
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

/* The derived maxima: HP 30 + level * strength / 5, mana (mana skill + 1) *
   intelligence / 8, carrying weight strength * 13 + 300. With restore, mana is filled. */
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

/* Gains levels: prints the new level, gives one skill point a level, recomputes the
   maxima. */
void far advance(char levels)
{
    register char *s = " 0\n";

    player->level = player->level + levels;
    if (player->level >= 10)
        s[0] = player->level / 10 + '0';
    else
        s[0] = ' ';
    s[1] = player->level % 10 + '0';
    game_sprint(0xA1);                  /* "Congratulations! You've reached experience level " */
    scroll_print(s);
    player->skill_points = player->skill_points + levels;
    player_compute(0);
    panel_check();
}

/* The attribute that governs a skill: 0 strength, 1 dexterity, 2 intelligence. */
int far prime(int skill)
{
    if (skill < 7)
        return 0;
    if (skill < 10)
        return 2;
    return 1;
}

/* Character creation's raise. A new skill gets 3 + attribute / 9 + 0 to 2 at random,
   plus three skill_checks of the attribute against 20 (each -1 to 2); a skill already
   held gets 1 + attribute / 13 + 0 or 1, plus two checks. At most 30. */
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
    player->skills[skill] += (int32)rand() * rolls / 0x8000L;
    for (; rolls > 0; rolls--)
        player->skills[skill] += skill_check(stat, 20);
    if (player->skills[skill] > 30)
        player->skills[skill] = 30;
}

/* The divisor for the random bonus when a skill rises, by governing attribute. */
static unsigned char skill_rng[3] = { 25, 30, 15 };

/* Raises a skill by spending a skill point (from a trainer, via BABLHACK.C's x_skills,
   or grant_skill_advance). Fails (returns 0) if the skill is already above twice its
   attribute or at 30. Otherwise +1; +1 more if the attribute is not strength and half
   the attribute is still above the skill; and +1 more with chance (attribute - skill) /
   skill_rng[attribute] while below the attribute. Raising lore also resets the lore
   tries and records the lore skill for this level. */
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

/* Raises one skill of a group: -1 the seven combat skills (3 tries), -2 mana, lore and
   casting (2 tries, mana favoured while below 8), -3 the ten others (4 tries), or which
   itself. Each try raises a random skill of the group through get_skill; returns 1 if
   any rose. */
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
            skill = start + (int)(((int32)rand() * count) / 0x8000L);
        if (get_skill(skill))
            result = 1;
    }
    player_compute(0);
    return result;
}

/* The end-of-game screen: the name, "A level N <class> freed Castle British after D
   days imprisoned within the Jewel.", the attributes, HP, mana and experience
   (exp / 10: exp is kept in tenths), and the twenty skills. If strength, dexterity and
   intelligence add up to more than 64 it adds "AND CHEATED ON THEIR CHARACTER" in
   another colour: chargen's rolls cannot get there. */
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

    str = str_copy(text, get_string(0x2CA));    /* "A level " */
    i = strlen(text);
    if (player->level > 9)
        text[i++] = player->level / 10 + '0';
    text[i++] = player->level % 10 + '0';
    text[i++] = ' ';
    text[i++] = 0;
    str_cat(text, get_string((player->pclass + 0x17) | STR_CHARGEN));
    y -= cur_font->height;
    str = get_string(0x2CB);            /* "freed Castle British" */
    x = 0xA0 - (string_width(text) + string_width(str)) / 2;
    string_to_screen(text, x, y);
    x += string_width(text) + 4;
    string_to_screen(str, x, y);

    hours = player->game_clock / 0x1C2000L;
    days = hours / 12;
    str_copy(text, get_string(0x2CC));  /* "after " */
    str_cat(text, itoa(days, numbuf, 10));
    str_cat(text, get_string(0x2CD));   /* " days imprisoned within the Jewel." */
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
        str_cat(text, get_string(0x2CE));       /* "AND CHEATED ON THEIR CHARACTER" */
        *foreground_color = *background_color = 0x1F;
        string_to_screen(text, 0x32, 0x14);
    }
}

/* The dream after a night's sleep. If the player ate the dream plant (sleepbits set,
   USEITEMS.C) the dream is the Ethereal Void: quest 48 is set and go_void moves the
   player there. Otherwise the castle story dreams 0 to 3 come due as X clock 1 (the
   castle plot) reaches 4, 6, 10 and 14; the latest due one not yet seen is shown. With
   none due, there is a 1 in 4 + 4 * sleepfactor chance of one of dreams 4 to 6 not yet
   seen. Dream n is cutscene 0x18 + n, and its dreamflags bit is flipped once shown.
   sleepfactor is 1 for a comfortable night (fed, in a bed or bedroll), which makes the
   random dreams rarer. Returns 1 if a cutscene played. */
unsigned char far dream(int sleepfactor)
{
    int found = -1;
    int counter;
    unsigned char xclocks[4] = { 4, 6, 10, 14 };
    uint32 timer;
    int dreamflags;

    if (player->sleepbits != 0)
    {
        game_sprint(0x18);              /* "Your dreams are vivid, showing a shifting colored scene..." */
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
        game_sprint(0x13 - sleepfactor);        /* "Your sleep is uneasy." or "You feel rested." */
        return 1;
    }
    timer = *Time;
    while (timer + 0x180 > *Time)
        ;
    game_sprint(0x13 - sleepfactor);
    return 0;
}

/* Passing out drunk where the player stands: in motion states 1 and 2 (bits 0 and 1;
   probably swimming or similar) the player dies. With state bit 3 (probably falling)
   and no slow fall, levitate or fly (motionbits 0x16), the fall does 12 to 62 damage. */
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

/* Sleeps. how: 0 camping on the ground, 1 a bedroll, 2 a bed, a negative value passing
   out (drunk, -2). A chosen sleep is refused while moving or swimming, in a fight
   (hostile creatures near) or in the pits; sleeping in the Ethereal Void wakes the
   player instead. The night:
   - 2 to 6 hours pass at once: active spells and mushrooms end, Killorn's crash comes
     if due (quest 50 without 54) and kills, lights burn 180 steps an hour, and poison
     strength n does n(n + 1) / 2 damage and is cured. Passing out also runs
     drop_drunk_player, and in the pits kills.
   - wandering_monster_check may interrupt: fatigue drops by 0x18, hunger by 12 to 27,
     drunkenness by 16, and no healing.
   - otherwise the rest of a 7 to 10 hour night passes (1 or 2 more below 10 HP).
     regen = fatigue / 2 + 2, at most 5 (fatigue counts time awake); comfort is 1 when
     hunger is above 0x40 and the player is in a bed or bedroll. HP and mana come back
     by restore_hp and restore_mana amounts built from regen and comfort; a starving
     player instead takes 2 damage. Hunger then drops by 24 to 55 and drunkenness by
     32, and the player dreams (dream). A player who would die in the night is kept at
     1 HP through the dream and then dies. */
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
            game_sprint(0x14);          /* "You can't go to sleep here!" */
            return;
        }
        if ((PlayerLevel - 1) / 8 == 8 && player->in_void)
        {
            punt_void();
            return;
        }
        if (hostile_creatures_near() || player->in_pits)
        {
            game_sprint(0xE);           /* "There are hostile creatures near!" */
            return;
        }
        if (how == 0 || how > 2)
            game_sprint(0xF);           /* "You make camp." */
        else
            game_sprint(how + 0x15);    /* "You unroll your sleeping bag ...", "You climb into the bed." */
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
        game_sprint(0x10);              /* "You go to sleep." */
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
        game_sprint(0x15);              /* "Your sleep is interrupted!" */
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
            game_sprint(0x11);          /* "You are starving." */
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
            game_sprint(0x13 - comfort);        /* "Your sleep is uneasy." or "You feel rested." */
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

/* The sleep key: sleeps in a bedroll if the player carries one (FindObj(4, 2, 1, 4)),
   else camps. */
void far player_key_sleep(int bedroll)
{
    int16 where;

    if (FindObj(4, 2, 1, 4, &where) == 0)
        bedroll = 0;
    else
        bedroll = 1;
    player_sleep(bedroll);
}

/* Changes hunger by nutrition (higher is fuller). Returns 0, eating nothing, if it
   would go past 255 (too full). Eating also heals food_heal / 6 HP (at most 8) and
   resets food_heal. PLAYTIME.C and player_sleep call it with negative values. */
unsigned char far player_eat(int nutrition)
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

/* The end: cutscene 2 (the ending), screen images 8 and 9 with game_stats, cutscene
   10 (the credits), then back to the start menu. */
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

/* Plays the cutscene a conversation left pending in quest_bytes[15] (quest 143: the
   cutscene number plus one, 0 for none). Cutscene 2 is the ending: the game is won. */
void far cs_check(void)
{
    unsigned char *cs = &player->quest_bytes[QB_CUTSCENE];

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

/* Sets the pending teleport to the square of the first item of the given id on the
   current level; returns 0 if level is not the current one. */
char far moveto(int level, int item)
{
    int16 x = 0;
    int16 y = 0;

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

/* npp_func after death outside Britannia: places the player near (28, 40) on level 5,
   the blackrock gem's cavern, moved from it by move_along(rand(), 8), with HP 2 to 4 below the
   maximum and mana down by an eighth plus 2, poison and spells gone, and a blood stain
   left on the floor of an open square. */
void far do_gem(void)
{
    int16 x;
    int16 y;
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
    SET_SEQ(ThePlayer, 1);
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
            SET_Z(stain, z);
            SET_FINEX(stain, yoff);
            SET_FINEY(stain, xoff);
            obj_deal(stain, x, y, 1);
        }
    }
    player->automap = 1;
}

/* npp_func on waking from the Void: back to the square and heading stored by go_void
   (trap_teleport_data carries the heading, see UWEDIT.C's new_player_pos). */
void far do_dreamret(void)
{
    set_new_music(10);
    NewPlayerX = player->dream_x;
    NewPlayerY = player->dream_y;
    trap_teleport_data = ((player->dream_pos >> 5) + 8) << 2;
    npp_func = 0;
}

/* npp_func of the moonstone spells: arrive at the moonstone (ITEM_MOONSTONE) on the
   destination level. */
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

/* Wakes the player from the Ethereal Void: drops anything held on the cursor and
   returns to the level, square and heading saved in dream_pos, dream_x and dream_y. */
void far punt_void(void)
{
    player->in_void = 0;
    player->sleepbits = 0;
    punt_mouse_obj();
    npp_func = do_dreamret;
    do_teleport(ThePlayer, 0x3F, 0x3F, (player->dream_pos >> 8) & 0xFF);
    player_setup(0, 0, -1);
    game_sprint(0x19);                  /* "You awaken after a night of extremely lifelike dreams..." */
    editchng(0x7FFE);
}

/* Where the void dream can put the player on level 69 (0x45, the Ethereal Void's
   fifth). */
static struct VoidTile void_tiles[4] = { { 32, 28 }, { 25, 13 }, { 38, 32 }, { 14, 40 } };

/* Sends the sleeping player to the Ethereal Void for 2 to 5 duration checks (sleepbits,
   counted down by PLAYTIME.C, which calls punt_void at zero). The return point is
   saved; the arrival square is void_tiles[0] five times in eight, or one of the other
   three moved by up to one square. */
void far go_void(void)
{
    int n;
    int x;
    int y;

    player->sleepbits = rand() % 4 + 2;
    player->in_void = 1;
    player->dream_x = OBJ_HOMEX(ThePlayer);
    player->dream_y = OBJ_HOMEY(ThePlayer);
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

/* HP has reached 0:
   - killed on level 1 by a castle guard (whoami 0x81 to 0x8F, 0x95 or 0xA8) that is not
     a loner: put in jail instead (WORLDEV.C's put_player_in_jail);
   - in the Ethereal Void: wake up (punt_void);
   - in the pits: the fight is lost, the pit fighters are reset and the pits record
     and Jospur's debt (quest_bytes 1 and 5) cleared, and death goes on.
   Death costs a ninth of the experience. Outside Britannia (level above 8) the player
   wakes at the blackrock gem on level 5 (do_gem); in Britannia it is the end
   (real_death(1)). */
void far player_is_dead(void)
{
    int i;

    if (PlayerLevel == 1 && ThePlayer->last_hit > 0)
    {
        struct Object far *killer = Obj_IntTMem(ThePlayer->last_hit);
        if ((killer->whoami >= 0x81 && killer->whoami <= 0x8F)
            || killer->whoami == 0xA8 || killer->whoami == 0x95)
        {
            if (!OBJ_LONER(killer))
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
        player->quest_bytes[QB_JOSPUR_DEBT] = 0;
    }
    kill_all_effects();
    load_new_music(MUSIC_DEATH, 1);
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
            game_sprint(0x169);         /* "You regain awareness in the cavern containing the pulsating blackrock gem." */
        }
        else
            real_death(1);
    }
    else
        real_death(1);
}

/* Tests for a trap on obj: a trap object of minor class 3 linked from it. Returns the
   skill_check of skill (search, or 45 for a spell) against 10 + twice the world
   number, or 0 when there is no trap. */
int far DetectedTrap(struct Object far *obj, int skill)
{
    union Link far *head;
    struct Object far *trig;

    if (OBJ_ISQUANT(obj) || OBJ_LINK(obj) == 0)
        return 0;
    for (head = &obj->ol.link; trig = Obj_InList(&head, 0, MAJOR_TRAP, -1, -1), trig;
         head = &trig->qn.link)
    {
        if (OBJ_MINOR(trig) == MINOR_TRIGGER2)
            return skill_check(skill, ((PlayerLevel - 1) / 8 << 1) + 10);
    }
    return 0;
}

/* Tries to disarm the trap on obj: skill_check of skill (traps) against 8 + the world
   number. Success removes the trap; failure (-1) sets it off; 0 is "Unable to defuse
   trap." */
int far RemoveTrap(struct Object far *obj, int skill)
{
    int quality;
    union Link far *head;
    struct Object far *trap;
    struct Object far *trig;
    char name[20];
    int result = 0;
    int owner;

    if (OBJ_ISQUANT(obj) || OBJ_LINK(obj) == 0)
        return 0;
    for (head = &obj->ol.link; trig = Obj_InList(&head, 0, MAJOR_TRAP, -1, -1), trig;
         head = &trig->qn.link)
    {
        if (OBJ_MINOR(trig) == MINOR_TRIGGER2)
            break;
    }
    if (trig)
    {
        if (OBJ_MINOR(trig) >= MINOR_TRIGGER)
            trap = Obj_PtrTMem(&trig->ol.link);
        else
        {
            trap = trig;
            trig = 0;
        }
        result = skill_check(skill, (PlayerLevel - 1) / 8 + 8);
        if (result > 0)
        {
            if (OBJ_INMAJOR(trap) == 15)
                strcpy(name, "trap");
            else if (OBJ_INMAJOR(trap) == 0 && OBJ_OWNER(trap))
                strcpy(name, "poison trap");
            else
                get_name(name, trap, 0, 0);
            scroll_print("The ");
            scroll_print(name);
            scroll_print(" on the ");
            get_name(name, obj, 0, 0);
            scroll_print(name);
            game_sprint(0x168);         /* " was successfully disarmed." */
            if (trig)
            {
                quality = OBJ_QUALITY(trig);
                owner = OBJ_OWNER(trig);
                delete_trap(&Map_GetAddr(quality, owner)->objects, trap);
            }
        }
        else if (result < 0)
        {
            game_sprint(0x166);         /* "Your bumbling attempts have set off the " */
            if (OBJ_INMAJOR(trap) == 15)
                strcpy(name, "trap");
            else if (OBJ_INMAJOR(trap) == 0 && OBJ_OWNER(trap))
                strcpy(name, "poison trap");
            else
                get_name(name, trap, 0, 0);
            scroll_print(name);
            game_sprint(0x60);          /* ".\n" */
            if (trig)
                UseTrigger(ThePlayer, obj, trig, -1);
            else
            {
                SetOffTrap(ThePlayer, obj, trap, MapObj_X, MapObj_Y);
                delete_trap(head, trap);
            }
        }
        else
            game_sprint(0x167);         /* "Unable to defuse trap." */
    }
    return result;
}
