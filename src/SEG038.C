/* target: seg038_342C */
/* opts: -mm -1 -G -O -d */
/* Skill checks, experience and levelling, and the stat panel's HP/MP/XP redraw: the whole
   of resident segment seg038_342C, in original order. Function and global names are the
   originals from the FM Towns symbol table. */

#include <stdlib.h>
#include "gfx.h"
#include "player.h"
#include "sys.h"
#include "ui.h"

extern unsigned char RightPanel;
extern struct Inplist near *inplist;
extern unsigned char far *foreground_color;
/* DS:8E1, levels by experience / 500. No FM Towns name (it sits just past
   _ShowStupidFirstPersonWeapon there), so this name is ours. */
/* This file's _DATA, DS:08E2..08F1 (seg037's ends at 08E1, odd, so this starts a file): the
   experience thresholds, in units of 500 points, for levels 2 to 16. FM Towns has it too,
   unnamed (static), read the same way, one byte before the table. */
static unsigned char level_table[16] = { 1, 2, 3, 4, 6, 8, 12, 16, 24, 32, 48, 64, 96, 128, 192, 0 };

/* PLAYER.C defines advance(char); this file's caller pushes SI unconverted, so the
   declaration it saw took an int. */
void far advance(int levels);

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
