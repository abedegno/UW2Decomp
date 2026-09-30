/* target: seg038_342C */
/* opts: -mm -1 -G -O -d */
/* Skill checks, experience and levelling, and the stat panel's HP/MP/XP redraw: the whole
   of resident segment seg038_342C, in original order. Function and global names are the
   originals from the FM Towns symbol table. */

#include <stdlib.h>

struct Player {
    char pad0[0x3D];
    unsigned char level;                /* 0x3D */
    char pad1[0x4E - 0x3E];
    unsigned long exp;                  /* 0x4E, in tenths */
    unsigned char skill_points;         /* 0x52 */
    unsigned char skill_points_earned;  /* 0x53, exp / 1500 already paid out; our name */
};

struct Inplist {
    char pad0[8];
    int field8;
};

extern struct Player near *player;
extern int PlayerLevel;
extern unsigned char RightPanel;
extern struct Inplist near *inplist;
extern unsigned char far *background_color;
extern unsigned char far *foreground_color;
extern int spsave[];                    /* DS:1B9F; FM Towns reads _spsave+4 */
/* DS:8E1, levels by experience / 500. No FM Towns name (it sits just past
   _ShowStupidFirstPersonWeapon there), so this name is ours. */
extern unsigned char level_table[];

void far panel_check(void);
/* PLAYER.C defines advance(char); this file's caller pushes SI unconverted, so the
   declaration it saw took an int. */
void far advance(int levels);
void far mouse_hide(void);
void far mouse_show(void);
void far set_font_size(int size);
void far restore_rect(int which);
void far sp_hp(void);
void far sp_mp(void);
void far sp_xp(void);

void far panel_check_hpmp(void);

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
    while (level_table[player->level + levels] <= points
           && player->level + levels < 16)
        levels++;
    if (levels)
        advance(levels);
    if (redraw)
        panel_check_hpmp();
}

void far panel_check_hpmp(void)
{
    if (RightPanel == 2 && inplist->field8 == 1)
    {
        *foreground_color = *background_color = 0xC4;
        mouse_hide();
        set_font_size(4);
        if (spsave[1])
            restore_rect(spsave[1]);
        sp_hp();
        sp_mp();
        sp_xp();
        set_font_size(1);
        mouse_show();
    }
}
