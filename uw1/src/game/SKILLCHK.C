/* target: seg037_32E6 */
/* opts: -mm -1 -G -O -d */
/* Skill checks, experience and levelling, and the stat panel's HP/MP/XP redraw: the whole
   of DOS resident segment seg037_32E6, in original order. Seeded from UW2Decomp's
   src/game/SKILLCHK.C (UW2's seg038_342C). Function and global names are UW2's (the FM
   Towns originals), the routines being the same; UW1 has no symbols of its own.
   skill_check is the game's one dice roll against a skill or attribute; combat, spells,
   traps, swimming and the timed updates (PLAYTIME.C) all call it.
   player_get_exp adds or takes experience, hands out skill points and calls SKILLS.C's
   advance when a level threshold is crossed. panel_check_hpmp redraws the stats panel's
   numbers.

   UW1 against UW2: player_get_exp does not halve every gain, compares the player's level
   with twice the dungeon level plus two (UW2 the world), gives a skill point every 3000
   (UW2 1500) without redrawing the panel, and stops at 0x17700 before adding (UW2 also
   caps at 0x7FFF0). panel_check_hpmp does not test the input mode, uses colour 0xF1, and
   loads fonts by file name. level_table has no trailing zero.

   Data: level_table, the experience needed for each level, and the two font names.
   Resident (not an overlay), so the many callers in overlays reach it directly.
   Name: descriptive (skill checks and experience: skill_check, player_get_exp). */

#include <stdlib.h>
#include "gfx.h"
#include "player.h"
#include "sys.h"
#include "ui.h"

/* Levels by experience / 500. */
/* name: UW2Decomp's (FM Towns has the table unnamed, static), so this name is ours. */
/* This file's _DATA, DS:0980..098E, then the literal pool to 09A8: the experience
   thresholds, in units of 500 points, for levels 2 to 16: level 2 at 500, level 3 at
   1000, ... level 16 at 96000 (192 * 500). level_table[level - 1] is the threshold of
   the next level. UW1: 15 bytes (UW2 has a 16th, zero). */
static unsigned char level_table[15] = { 1, 2, 3, 4, 6, 8, 12, 16, 24, 32, 48, 64, 96, 128, 192 };

/* match: SKILLS.C defines advance(char); this file's caller pushes SI unconverted, so
   the declaration it saw took an int. */
void far advance(int levels);

/* UW1: GRFX.C's font loader takes the font's file name (UW2's grfx_quikfont an index). */
unsigned char far grfx_load_font(char *name);

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

/* Gives the player n experience (in the units of player->exp). A loss (n < 0) is taken
   off with a floor of 0 and nothing else happens. A gain is ignored once experience is
   above 0x17700 (96000), and halved (plus one) when the player's level is above twice
   the dungeon level plus two, so fighting high up pays less once the player has
   outgrown it. Every 3000 of total experience earns a skill point (skill_points,
   counted against skill_points_earned so none are given twice). Levels stop at 16. The
   panel's numbers are redrawn when exp >> 4 changes. */
void far player_get_exp(int n)
{
    int points;
    int redraw;
    register int levels;

    if (n < 0)
    {
        if (-n > player->exp)
            player->exp = 0;
        else
            player->exp += n;
        return;
    }
    /* UW1: experience stops growing once it passes 0x17700, checked first */
    if (player->exp > 0x17700L)
        return;
    /* UW1: PlayerLevel is the dungeon level itself, and no halving of every gain */
    if ((PlayerLevel * 2 + 2) < player->level)
        n = n / 2 + 1;
    /* UW1: a skill point every 3000, and the panel is not redrawn here */
    if ((points = (player->exp + n) / 3000) > player->skill_points_earned)
    {
        player->skill_points = player->skill_points + (points - player->skill_points_earned);
        player->skill_points_earned = points;
    }
    redraw = player->exp >> 4;
    player->exp += n;
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
   the stats (RightPanel 2). */
void far panel_check_hpmp(void)
{
    if (RightPanel == 2)
    {
        /* UW1: two stores, foreground first (UW2 one chained store) */
        *foreground_color = 0xF1;
        *background_color = 0xF1;
        mouse_hide();
        grfx_load_font("font5x6i.sys");
        if (spsave[1])
            restore_rect(spsave[1]);
        sp_hp();
        sp_mp();
        sp_xp();
        grfx_load_font("font5x6p.sys");
        mouse_show();
    }
}
