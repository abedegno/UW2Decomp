/* target: seg038_342C */
/* opts: -mm -1 -G -O -d */
/* Skill checks, experience and levelling, and the stat panel's HP/MP/XP redraw: the whole
   of resident segment seg038_342C, in original order. Function and global names are the
   originals from the FM Towns symbol table.
   skill_check is the game's one dice roll against a skill or attribute; combat, spells,
   traps, bartering, swimming and the timed updates (PLAYTIME.C) all call it.
   player_get_exp adds or takes experience, hands out skill points and calls SKILLS.C's
   advance when a level threshold is crossed. panel_check_hpmp redraws the stats panel's
   numbers.
   Data: level_table, the experience needed for each level.
   Resident (not an overlay), so the many callers in overlays reach it directly.
   Name: descriptive (skill checks and experience: skill_check, player_get_exp). */

#include <stdlib.h>
#include "gfx.h"
#include "player.h"
#include "sys.h"
#include "ui.h"

extern unsigned char RightPanel;
extern struct Inplist near *inplist;
extern unsigned char far *foreground_color;
/* DS:8E1, levels by experience / 500. */
/* name: no FM Towns name (it sits just past _ShowStupidFirstPersonWeapon there), so this
   name is ours. FM Towns has the table too, unnamed (static), read the same way, one byte
   before the table. */
/* This file's _DATA, DS:08E2..08F1 (seg037's ends at 08E1, odd, so this starts a file): the
   experience thresholds, in units of 500 points, for levels 2 to 16: level 2 at 500,
   level 3 at 1000, ... level 16 at 96000 (192 * 500). level_table[level - 1] is the
   threshold of the next level. */
static unsigned char level_table[16] = { 1, 2, 3, 4, 6, 8, 12, 16, 24, 32, 48, 64, 96, 128, 192, 0 };

/* match: SKILLS.C defines advance(char); this file's caller pushes SI unconverted, so
   the declaration it saw took an int. */
void far advance(int levels);

/* Rolls value - target + 0..30 and grades it: above 28 returns 2 (a great success),
   16 to 28 returns 1 (success), 3 to 15 returns 0 (a near miss), and 2 or less -1
   (failure). So with value equal to target the chances are 2 in 31 for 2, 13 for 1,
   13 for 0 and 3 for -1. Callers mostly test the result for > 0. */
int far skill_check(int value, int target)
{
    int score;

    score = value - target;
    score += rand() % 31;
    if (score > 28)
        return 2;
    if (score > 15)
        return 1;
    if (score > 2)
        return 0;
    return -1;
}

/* Gives the player n experience (in the units of player->exp). A gain is halved first,
   rounding up or down at random, and halved again (plus one) when the player's level is
   above twice the world number plus two (world = (PlayerLevel - 1) / 8, eight levels to
   a world), so fighting in early worlds pays less once the player has outgrown them. A
   loss (n < 0) is taken off with a floor of 0 and nothing else happens.
   Every 1500 of total experience earns a skill point (skill_points, counted against
   skill_points_earned so none are given twice). Experience stops at 0x7FFF0; levels
   stop at 0x17700 (96000) and at level 16. The panel's numbers are redrawn when
   exp >> 4 changes. */
void far player_get_exp(int n)
{
    int points;
    int redraw;
    register int levels;

    if (n > 0)
        n = (n + rand() % 2) / 2;
    if (n < 0)
    {
        if (-n > player->exp)
            player->exp = 0;
        else
            player->exp += n;
        return;
    }
    if (((PlayerLevel - 1) / 8 * 2 + 2) < player->level)
        n = n / 2 + 1;
    if ((points = (player->exp + n) / 1500) > player->skill_points_earned)
    {
        player->skill_points = player->skill_points + (points - player->skill_points_earned);
        player->skill_points_earned = points;
        panel_check();
    }
    redraw = player->exp >> 4;
    if (player->exp + n > 0x7FFF0L)
        return;
    player->exp += n;
    if (player->exp > 0x17700L)
        return;
    redraw = (player->exp >> 4) > redraw;
    levels = 0;
    points = player->exp / 500;
    while (level_table[player->level + levels - 1] <= points
           && player->level + levels < 16)
        levels++;
    if (levels)
        advance(levels);
    if (redraw)
        panel_check_hpmp();
}

/* Redraws the health, mana and experience figures, but only when the right panel shows
   the stats (RightPanel 2) and the game screen is up (input mode 1). */
void far panel_check_hpmp(void)
{
    if (RightPanel == 2 && inplist->mode == 1)
    {
        *foreground_color = *background_color = 0xC4;
        mouse_hide();
        grfx_quikfont(FONT_5X6I);
        if (spsave[1])
            restore_rect(spsave[1]);
        sp_hp();
        sp_mp();
        sp_xp();
        grfx_quikfont(FONT_5X6P);
        mouse_show();
    }
}
