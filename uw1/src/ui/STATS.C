/* target: ovr145 */
/* opts: -mm -1 -G -O -Y -d */
/* The character panel's statistics page: the name, class and level header, the three
   attributes, hit points, mana, experience, a scrolling list of six skills, and the click
   handler that scrolls it: the whole of UW1's DOS overlay ovr145 (UW2's ovr158), in
   original order.

   RedispStat draws the page when the right-hand panel shows it, mous_in_stat is its click
   handler, and ovr145_4FB (UW2's panel_check) redraws it when it is showing. Everything
   is drawn with the 5x6 italic font in colour 0xF1 (header and numbers) or 0x68 (the skill
   list), right-aligned at x 0x138.

   UW1 against UW2: the page's y coordinates are 4 less and its x mostly 2 more; the skill
   list has no unspent skill points row, so row i is skill i + skill_top, and skill_top
   runs 0..14 over the 20 skills; the click handler only waits for the button
   to come up (mouse_release); ovr145_4FB does not test the input mode; fonts are loaded
   by file name; the class is bits 5-7 of the player record's byte 0x64.

   Data owned: skill_top (the first skill row shown), the level ordinal suffixes and
   spsave, two saved screen areas allocated on the first draw (the skill list's
   background, which sp_skill copies back before writing a row).
   UW1 has no symbol-bearing build: the names are UW2's (the FM Towns symbol table), the
   routines being the same.
   Name: descriptive (the character panel's statistics page). */

#include <string.h>
#include <stdlib.h>
#include "critter.h"
#include "gfx.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"

/* UW1: the font loader takes a file name (UW2: grfx_quikfont(int)); UW1's name for
   UW2's seg003_0272_5025 (copy part of a saved screen area back). */
unsigned char far grfx_load_font(char *name);

/* The first skill shown in the skill list (UW1 DS:1D5C, UW2 DS:1B92). */
/* name: UW2's FM Towns build has no name for it, so it was static; the name is ours. */
static unsigned char skill_top = 0;
/* Level ordinals, UW1 DS:1D5D: levels 1 to 3 take ST, ND, RD and every higher level TH. */
/* name: DOS only (FM Towns prints no ordinal), so the name is ours. */
static char ordinals[4][3] = { "ST", "ND", "RD", "TH" };
/* Saved screen areas, UW1 DS:1D69. */
int16 spsave[3] = { 0, 0, 0 };

/* The x of the right edge everything is aligned to (UW2 0x135). */
#define RIGHT   0x138

/* The header: the name (up to 15 characters, upper-cased, centred), the class and the
   level with its ordinal ("12TH"). */
void far sp_hdr(void)
{
    char n;
    char buf[30];

    strncpy(buf, player->name, 0xF);
    buf[0xF] = 0;
    strupr(buf);
    string_to_screen(buf, (0x48 - string_width(buf)) / 2 + 0xF2, 0xB9);
    /* the class name, upper-cased in the string buffer */
    string_to_screen(seg039_3452_814(get_string((player->pclass + 0x17) | STR_CHARGEN)), 0xF2, 0xB2);
    itoa(player->level, buf, 10);
    n = player->level < 4 ? player->level - 1 : 3;
    strcat(buf, ordinals[n]);
    string_to_screen(buf, RIGHT - string_width(buf), 0xB2);
}

/* Attribute i (0 to 2, as playerdat->attr holds them), one row each from y 0xB1 down. */
/* name: IDA's PrintPlayerAttribute. UW2's FM Towns build has sp_att_ here, between sp_hdr_
   and sp_hp_, and RedispStat_ calls it for 0 to 2 just as the DOS code calls this. */
void far sp_att(unsigned char i)
{
    char buf[4];

    itoa(playerdat->attr[i], buf, 10);
    string_to_screen(buf, RIGHT - string_width(buf), (2 - i) * 7 + 0x9D);
}

void far sp_hp(void)
{
    char buf[8];
    int n;

    itoa(ThePlayer->hp, buf, 10);
    n = strlen(buf);
    buf[n] = '/';
    n++;
    itoa(playerdat->avghit, buf + n, 10);
    string_to_screen(buf, RIGHT - string_width(buf), 0x96);
}

void far sp_mp(void)
{
    char buf[8];
    int n;

    itoa(player->play_mana, buf, 10);
    n = strlen(buf);
    buf[n] = '/';
    n++;
    itoa(player->max_mana, buf + n, 10);
    string_to_screen(buf, RIGHT - string_width(buf), 0x8F);
}

/* Experience is shown divided by ten. */
void far sp_xp(void)
{
    char buf[10];

    ltoa(player->exp / 10, buf, 10);
    string_to_screen(buf, RIGHT - string_width(buf), 0x88);
}

/* Row i of the six-row skill list: skill i + skill_top with its name from block 2 (string
   0x1F + skill). */
void far sp_skill(unsigned char i)
{
    char buf[4];

    itoa(player->skills[i + skill_top], buf, 10);
    seg003_581E(spsave[0], 0xF0, 0x80 - i * 7 + 1, 0x4C, (i + 1) * 7, 0, i * 7);
    string_to_screen(seg039_3452_814(get_string((i + skill_top + 0x1F) | STR_CHARGEN)), 0xF2, 0x80 - i * 7);
    string_to_screen(buf, RIGHT - string_width(buf), 0x80 - i * 7);
}

void far RedispStat(void)
{
    unsigned char i;

    if (spsave[0] == 0) {
        spsave[0] = valloc(0x4B, 0x2A);
        if (spsave[0] != 0)
            save_rect(spsave[0], 0xF0, 0x81, 0x4B, 0x2B);
        spsave[1] = valloc(0x23, 0x15);
        if (spsave[1] != 0)
            save_rect(spsave[1], 0x115, 0x96, 0x23, 0x15);
    }
    *foreground_color = 0xF1;
    *background_color = 0xF1;
    mouse_hide();
    grfx_load_font("font5x6i.sys");
    sp_hdr();
    for (i = 0; i < 3; i++)
        sp_att(i);
    sp_hp();
    sp_mp();
    sp_xp();
    *foreground_color = 0x68;
    *background_color = 0x68;
    for (i = 0; i < 6; i++)
        sp_skill(i);
    grfx_load_font("font5x6p.sys");
    mouse_show();
}

/* A click on the statistics page: in the arrow strip at the bottom (y under 8) scrolls
   the skill list one row back (left half) or on (right half), skill_top staying within
   0..14 (mvcheck), then waits for the button to come up. The first dir, from the button,
   is overwritten and has no effect. */
void far mous_in_stat(void)
{
    int16 v;
    register int dir;

    grfx_load_font("font5x6i.sys");
    *foreground_color = 0x68;
    *background_color = 0x68;
    dir = (inplist->cmd & 1) ? -1 : 1;
    if (inplist->y < 8) {
        v = skill_top;
        dir = inplist->x < 0x25 ? -1 : 1;
        if (mvcheck(&v, dir == 1 ? 0xE : 0, 1, dir)) {
            skill_top = v;
            mouse_hide();
            for (v = 0; v < 6; v++)
                sp_skill(v);
            mouse_show();
        }
    }
    grfx_load_font("font5x6p.sys");
    mouse_release(1);
}

/* name: symbols.tsv's (SKILLS.C calls it); UW2's panel_check, the same routine. */
void far ovr145_4FB(void)
{
    if (RightPanel == PANEL_STATS)
        pretty_panelagain();
}
