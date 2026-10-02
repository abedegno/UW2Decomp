/* target: ovr135 */
/* opts: -mm -1 -G -O -Y -d */
/* The player's timed updates: active spells running down, light sources burning out,
   poison, hunger, drunkenness, drowning, and the hourly schedule. The whole of DOS
   overlay ovr135, in original order. Function and global names are the originals from
   the FM Towns symbol table; the source file's own name is not known. */

#include <stdlib.h>
#include "combat.h"
#include "critter.h"
#include "event.h"
#include "gfx.h"
#include "inv.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"

#define OBJ_QUALITY(o)  ((o)->qn.f.quality)

struct Light {
    unsigned char duration;             /* burn rate, 0 for an unlit light */
    unsigned char pad;
};

#define SPELL_CLASS(s)  ((s) & 0x0F)
#define SPELL_SUB(s)    (((s) & 0xF0) >> 4)
#define SPELL_STAB(s)   ((s) >> 8)

extern unsigned char plyregen[];        /* [0] regeneration bits, [1] update counter */
extern char ValidLightSlots[];
extern struct Light Lights[];

void far Killorn_just_crashed(int how);
void far damage_item(struct Object far *who, void far *source, int a, int b,
                     unsigned char damage, int type);
char far player_eat(int nutrition);
int far get_workspace(void);

char far dispel_spell(int *i)
{
    if (SPELL_CLASS(player->spells[*i]) == 1
        && (SPELL_SUB(player->spells[*i]) == 3 || SPELL_SUB(player->spells[*i]) == 5))
    {
        player->spells[*i] = (player->spells[*i] >> 8 << 8) + 0x21;
        player->spells[*i] = (player->spells[*i] & 0xFF) + 0x100;
    }
    else
    {
        if (SPELL_CLASS(player->spells[*i]) == 0xB && SPELL_SUB(player->spells[*i]) == 1)
        {
            attach_eye(1);
            GameInputMode -= 8;
        }
        if (SPELL_CLASS(player->spells[*i]) == 1)
            fiz_update = 1;
        player->active_spells--;
        if ((*i)-- < player->active_spells)
            player->spells[*i + 1] = player->spells[player->active_spells];
    }
    return 1;
}

void far duration_check(void)
{
    int i;
    char changed;
    int dmg;
    int stab;

    changed = 0;
    plyregen[1]++;
    for (i = 0; i < player->active_spells; i++)
    {
        stab = SPELL_STAB(player->spells[i]);
        if (stab == 1)
            changed = dispel_spell(&i);
        else
            player->spells[i] = (player->spells[i] & 0xFF) + ((stab - 1) << 8);
    }
    if (DegradeLights(1, plyregen[1]))
        changed = 1;
    if (player->shrooms)
    {
        if (--player->shrooms == 0)
            changed = 1;
    }
    if (changed)
    {
        FixPlayerEquips();
        editchng(2);
    }
    if (plyregen[0] != 0)
    {
        if (plyregen[0] & 1)
            restore_hp(ThePlayer, -1);
        if (plyregen[0] & 2)
            restore_mana(ThePlayer, -1);
    }
    if (player->swim_count > 0x50)
        sink_sink_sink();
    if ((int)((player->quests[12] & 4) >> 2) && !(int)((player->quests[13] & 4) >> 2))
    {
        do_sfx(4, 0x2C);
        if (!TimeStop && player->quest_bytes[6]-- == 0)
        {
            Killorn_just_crashed(0);
            damage_item(ThePlayer, 0L, 0, 0, 0xFF, 0);
        }
    }
    if (player->sleepbits)
    {
        player->sleepbits--;
        if (!player->sleepbits && player->in_void)
            punt_void();
    }
    if (plyregen[1] % 3 == 0)
    {
        if (player->poison)
        {
            dmg = player->poison--;
            damage_item(ThePlayer, 0L, 0, 0, dmg, 0x10);
        }
        if ((dmg = skill_check(player->skills[SKILL_MANA], 10)) > 0)
            restore_mana(ThePlayer, -dmg);
    }
    if (plyregen[1] % 30 == 0)
    {
        player_eat(-3 - (rand() & 3));
        if (player->drunk)
            player->drunk--;
        if ((rand() & 3) == 0)
            DoWanderingMonsters(1);
        yearly_checkup();
        for (i = 0; i < 3; i++)
            if ((&player->fatigue)[i] < 0xFF)
                (&player->fatigue)[i]++;
        if (skill_check(playerdat->attr[0], 15) > 0)
            restore_hp(ThePlayer, -1);
    }
    if (plyregen[1] % 60 == 0)
    {
        player->xclock[XC_TIME]++;
        player->xclock[XC_TIME] = player->xclock[XC_TIME] % 72;
        if (get_workspace())
        {
            Sched_SetBuf(0, set_workspace());
            Sched_Load(0);
            Sched_WrapTime(player->xclock[XC_TIME], 72, 1);
            Sched_Save(0);
            release_workspace();
        }
        plyregen[1] = 0;
    }
}

char far DegradeLights(int amount, unsigned char counter)
{
    int light;
    struct Object far *obj;
    char changed;
    register int i;
    register int burn;

    changed = 0;
    if (TimeStop == 0)
    {
        for (i = 0; i < 4; i++)
        {
            if ((obj = AskInventory(ValidLightSlots[i])) == 0)
                continue;
            light = obj->id & ID_INCLASS;
            if ((obj->id & ID_CLASS) >> 4 != CLASS_LIGHT || light < 4 || light >= 8)
                continue;
            if ((light = Lights[light].duration) == 0)
                continue;
            burn = 0;
            if (counter % light == 0)
                burn = 1;
            if (amount > 1)
                burn += amount / light;
            if (burn == 0)
                continue;
            if (OBJ_QUALITY(obj) - 1 > burn)
                OBJ_QUALITY(obj) = OBJ_QUALITY(obj) - burn;
            else
            {
                OBJ_QUALITY(obj) = 1;
                obj->id = obj->id & 0xFFF0 | ((obj->id & ID_INCLASS) - 4) & 0x0F;
                RedisplayInvSlot(ValidLightSlots[i]);
                changed = 1;
            }
        }
    }
    return changed;
}

void far sink_sink_sink(void)
{
    unsigned char load;
    register int skill;
    register int hurt;

    load = 0;
    if (player->max_weight != 0)
        load += (player->weight << 5) / player->max_weight;
    if ((skill = skill_check(player->skills[SKILL_SWIMMING], load)) < 1 && player->swim_count < 0x8C)
        player->swim_count += rollem(3 - skill, 4);
    if (player->swim_count > 0x78)
    {
        hurt = 2 - skill_check(player->skills[SKILL_SWIMMING], load);
        if (hurt)
        {
            fill_FB(0x50);
            editchng(2);
            damage_item(ThePlayer, 0L, 0, 0, rollem(2, hurt + 2), 0);
        }
    }
}

char far set_curmagic(unsigned char cls, unsigned char sub, unsigned char stability)
{
    unsigned char stab;

    if (player->active_spells == 3)
        return 0;
    player->spells[player->active_spells] =
        (player->spells[player->active_spells] >> 8 << 8) + cls + (sub << 4);
    switch (stability)
    {
    case 1:
        stab = 1;
        break;
    case 0:
        stab = rollem(2, 3);
        break;
    case 0x40:
        stab = rollem(2, 8) + 6;
        break;
    case 0x80:
        stab = rollem(3, 20) + 24;
        break;
    }
    player->spells[player->active_spells] =
        (player->spells[player->active_spells] & 0xFF) + (stab << 8);
    player->active_spells++;
    FixPlayerEquips();
    return 1;
}
