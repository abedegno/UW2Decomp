/* target: ovr158 */
/* opts: -mm -1 -G -O -Y -d */
/* The character panel's statistics page: the name, class and level header, the three
   attributes, hit points, mana, experience, a scrolling list of six skills, and the click
   handler that scrolls it: the whole of DOS overlay ovr158, in original order. Function and
   global names are the originals from the FM Towns symbol table where it has them; the
   source file's own name is not known.

   RedispStat is panel_dispatch[2] in PANELS.C (drawn when the right-hand panel shows the
   statistics page, after the panel art), mous_in_stat is INTERACT.C's mous_in_panel's
   handler for that page, and panel_check redraws the page in mode 1. Everything is drawn
   with the 5x6 italic font in colour 0xC4 (header and numbers) or 0xC9 (the skill list),
   right-aligned at x 0x135.

   Data owned: skill_top (the first skill row shown), the level ordinal suffixes and
   spsave, two saved screen areas allocated on the first draw (the skill list's
   background, which sp_skill copies back before writing a row).

   Name: descriptive (the character panel's statistics page). */

#include <string.h>
#include <stdlib.h>
#include "critter.h"
#include "gfx.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"

/* The first row shown in the skill list (DS:1B92): 0 shows the unspent skill points
   first, n starts at skill n - 1. */
/* name: FM Towns has no name for it (it shows as _dtypes+0x1C there), so it was static;
   the name is ours. */
static unsigned char skill_top = 0;
/* Level ordinals, DS:1B93: levels 1 to 3 take ST, ND, RD and every higher level TH. */
/* name: DOS only (FM Towns prints no ordinal), so the name is ours. */
static char ordinals[4][3] = { "ST", "ND", "RD", "TH" };
/* Saved screen areas, DS:1B9F. FM Towns has 12 bytes here, three pointers. */
int16 spsave[3] = { 0, 0, 0 };


/* The header: the name (up to 15 characters, upper-cased, centred), the class and the
   level with its ordinal ("12TH"). */
void far sp_hdr(void)
{
    char n;
    char buf[30];

    strncpy(buf, player->name, 0xF);
    buf[0xF] = 0;
    strupr(buf);
    string_to_screen(buf, (0x48 - string_width(buf)) / 2 + 0xF0, 0xBD);
    /* "Fighter" .. "Shepherd", upper-cased in the string buffer */
    string_to_screen(seg039_3452_814(get_string((player->pclass + 0x17) | STR_CHARGEN)), 0xF0, 0xB6);
    itoa(player->level, buf, 10);
    n = player->level < 4 ? player->level - 1 : 3;
    strcat(buf, ordinals[n]);
    string_to_screen(buf, 0x135 - string_width(buf), 0xB6);
}

/* Attribute i (0 to 2, as playerdat->attr holds them), one row each from y 0xB5 down. */
/* name: IDA's PrintPlayerAttribute_ovr158_F3. FM Towns has sp_att_ here, between sp_hdr_
   and sp_hp_, and RedispStat_ calls it for 0 to 2 just as the DOS code calls this. */
void far sp_att(unsigned char i)
{
    char buf[4];

    itoa(playerdat->attr[i], buf, 10);
    string_to_screen(buf, 0x135 - string_width(buf), (2 - i) * 7 + 0xA1);
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
    string_to_screen(buf, 0x135 - string_width(buf), 0x9A);
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
    string_to_screen(buf, 0x135 - string_width(buf), 0x93);
}

/* Experience is shown divided by ten. */
void far sp_xp(void)
{
    char buf[10];

    ltoa(player->exp / 10, buf, 10);
    string_to_screen(buf, 0x135 - string_width(buf), 0x8C);
}

/* Row i of the six-row skill list: the unspent skill points when the row is the first
   and the list is at its top, else skill i + skill_top - 1, each with its name from block
   2 (string 0x1F + skill: "Attack" .. "Swimming"; 0x1F + 100 = 131 is "Skill Pt"). */
void far sp_skill(unsigned char i)
{
    char buf[4];
    char sk;

    if (i + skill_top > 0) {
        sk = i + skill_top - 1;
        itoa(player->skills[sk], buf, 10);
    } else {
        sk = 100;
        itoa(player->skill_points, buf, 10);
    }
    seg003_0272_5025(spsave[0], 0xF0, 0x84 - i * 7 + 1, 0x4C, (i + 1) * 7, 0, i * 7);
    string_to_screen(seg039_3452_814(get_string((sk + 0x1F) | STR_CHARGEN)), 0xF0, 0x84 - i * 7);
    string_to_screen(buf, 0x135 - string_width(buf), 0x84 - i * 7);
}

void far RedispStat(void)
{
    unsigned char i;

    if (spsave[0] == 0) {
        spsave[0] = valloc(0x4C, 0x2A);
        if (spsave[0] != 0)
            save_rect(spsave[0], 0xF0, 0x85, 0x4B, 0x2B);
        spsave[1] = valloc(0x24, 0x15);
        if (spsave[1] != 0)
            save_rect(spsave[1], 0x112, 0x9A, 0x23, 0x15);
    }
    *foreground_color = *background_color = 0xC4;
    mouse_hide();
    grfx_quikfont(FONT_5X6I);
    sp_hdr();
    for (i = 0; i < 3; i++)
        sp_att(i);
    sp_hp();
    sp_mp();
    sp_xp();
    *foreground_color = *background_color = 0xC9;
    for (i = 0; i < 6; i++)
        sp_skill(i);
    grfx_quikfont(FONT_5X6P);
    mouse_show();
}

/* A click on the statistics page: in the arrow strip at the bottom (y under 10) scrolls
   the skill list one row back (left half) or on (right half), skill_top staying within
   0..15 (mvcheck), then
   waits up to 8 ticks for the button to come up. The first dir, from the button, is
   overwritten and has no effect. */
void far mous_in_stat(void)
{
    int16 v;
    uint32 start;
    int key;
    int dir;

    grfx_quikfont(FONT_5X6I);
    *foreground_color = *background_color = 0xC9;
    dir = (inplist->cmd & 1) ? -1 : 1;
    if (inplist->y < 10) {
        v = skill_top;
        dir = inplist->x < 0x26 ? -1 : 1;
        if (mvcheck(&v, dir == 1 ? 0xF : 0, 1, dir)) {
            skill_top = v;
            mouse_hide();
            for (v = 0; v < 6; v++)
                sp_skill(v);
            mouse_show();
        }
    }
    grfx_quikfont(FONT_5X6P);
    start = GAME_TIME();
    do
        key = mouse_get_input_sp();
    while (GAME_TIME() - start < 8 && key < 3 && (key & 3));
}

void far panel_check(void)
{
    if (RightPanel == 2 && inplist->mode == 1)
        pretty_panelagain();
}
