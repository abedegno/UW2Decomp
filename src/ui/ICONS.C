/* target: seg014 */
/* opts: -mm -1 -G -O -d */
/* The icon bar: the six command icons at the bottom right of the game screen (x 0xF2 to
   0x13E, y 0 to 0x1C; two rows of three, 26 by 15 pixels each), registering its click
   area, finding which icon was clicked, drawing an icon selected or not, and the Ctrl-key
   shortcuts to the options panel. DOS resident segment seg014, in original order.
   Function and global names are the originals from the FM Towns symbol table where it
   has them.

   The icons select the interaction mode, INTERACT.C's RightButtonThing (deal_with_icons
   is the click handler setup_icon_buttons registers). button_to_mode and mode_to_button
   map between icon positions and modes: icon 0 attack, 1 use, 2 get, 3 talk, 4 look (as
   RightButtonThing 2, 1, 4, 5, 3), and icon 5 opens the options panel.

   Data owned: the option panel's five button groups (gameopts, detail, quit, file and
   musicsound), whose handlers live in WRAPPER.C (the options overlay, which drives them in
   busywaiting_new_options), current_buttongroup, the highlighted button, the two icon and
   mode maps, and the icon bar's input handle. Pictures 0x20C1 and 0x20C2 are the icon bar
   drawn plain and selected; new_IconSelect redraws it selected and frames one icon.

   Name: descriptive (the icon bar: setup_icon_buttons, new_IconSelect). */

#include "gfx.h"
#include "ui.h"

/* _BSS. */
/* match: laid out by name (tools/bssorder.py): gameopts_done sorts before
   icon_button_handle. */
int gameopts_done;                     /* DS:67D6+22E2, also written by the options overlay */
static int icon_button_handle;         /* DS:67D6+22E4, only ever used in this file */

int far input_addmouse(int a, int b, int c, int d, int buttons, int mode, void far (*handler)(int));
void far mouse_release(int flag);
void far busywaiting_new_options(struct buttongroup *g);

/* This file's _DATA, DS:0120..0267, in definition order: the option panel's button groups.
   It starts a new file (word-aligned, after seg013's emm_id ends at DS:011F) and lies
   between seg013's data and seg015's in link order, so it is this file's. FM Towns keeps
   button_to_mode .. current_buttongroup together in this order. */
char dseg_67d6_120 = 0;                /* DS:0120, seg013's EMS page counter */
char dseg_67d6_121 = 0;                /* DS:0121, never referenced */
char current_hilit_button = -1;        /* DS:0122 */
unsigned char button_to_mode[6] = { 1, 0, 3, 4, 2, 5 };    /* DS:0123 */
unsigned char mode_to_button[6] = { 1, 0, 4, 2, 3, 5 };    /* DS:0129 */
extern struct buttongroup quit_buttongroup;
struct buttongroup gameopts_buttongroup = {                  /* DS:012F */
    game_group_fun, 0,
    { save_opt, restore_opt, music_opt, sound_opt, donothing_opt, donothing_opt, resume_play_opt },
    { 10, 11, 12, 13, 14, 0, 0 },
    { &file_buttongroup, &file_buttongroup, &musicsound_buttongroup, &musicsound_buttongroup,
      &detail_buttongroup, &quit_buttongroup, 0 } };
struct buttongroup detail_buttongroup = {                    /* DS:016D */
    detail_group_fun, 1,
    { 0, 0, detail_set_opt, detail_set_opt, detail_set_opt, detail_set_opt, donothing_opt },
    { 0, 0, 0, 1, 2, 3, 0 },
    { 0, 0, 0, 0, 0, 0, &gameopts_buttongroup } };
struct buttongroup quit_buttongroup = {                      /* DS:01AB */
    quit_group_fun, 2,
    { 0, 0, quit_do_opt, quit_do_opt, 0, 0, 0 },
    { 0, 0, 1, 0, 0, 0, 0 },
    { 0 } };
struct buttongroup file_buttongroup = {                      /* DS:01E9 */
    file_group_fun, 3,
    { 0, do_saverest_opt, do_saverest_opt, do_saverest_opt, do_saverest_opt, resume_play_opt, 0 },
    { 0, 1, 2, 3, 4, 0, 0 },
    { 0 } };
struct buttongroup musicsound_buttongroup = {                /* DS:0227 */
    musicsound_group_fun, 4,
    { 0, 0, do_musicsound_opt, do_musicsound_opt, donothing_opt, 0, 0 },
    { 0, 0, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 0, &gameopts_buttongroup, 0, 0 } };
struct buttongroup *current_buttongroup = &gameopts_buttongroup;   /* DS:0265 */

/* Registers the icon bar's click area (mode mask 1, the 3D view) with deal_with_icons(-1)
   as its handler, and draws the bar. */
/* name: from the FM Towns build: setup_icon_buttons_ comes just before term_icon_buttons_
   there, and its code corresponds call for call (register the icon bar's click area, hide
   the mouse, draw the icon bar, show the mouse). */
void far setup_icon_buttons(void)
{
    icon_button_handle = input_addmouse(0xF2, 0, 0x13E, 0x1C, -1, 1, deal_with_icons);
    mouse_hide();
    pic_to_screen(0x20C1, 0xF3, 0x1C, 0x4D, 0x1D);
    mouse_show();
}

void far term_icon_buttons(void)
{
    input_del(icon_button_handle);
}

/* The icon under the pointer, 0 to 5, or -1: the upper row (y 15 and up) is 0 to 2, the
   lower row 3 to 5. */
int far get_iconreg_button(void)
{
    int x, y;
    int col;

    mouse_getxy(&x, &y);
    x -= 0xF2;
    y -= 0;
    col = x / 0x1A;
    col += 3 - y / 0xF * 3;
    if (col >= 0 && col < 6)
        return col;
    return -1;
}

/* Redraws the bar plain (picture 0x20C1) and frames icon index in colour 0x106. */
void far new_IconUnselect(int index)
{
    int top, left;

    mouse_hide();
    grSoftPageFlip();
    pic_to_screen(0x20C1, 0xF3, 0x1C, 0x4D, 0x1D);
    top = 0xF3;
    left = 0x1C;
    if (index >= 3)
    {
        left -= 0xF;
        index -= 3;
    }
    top += index * 0x1A;
    grSoftPageFlip();
    set_the_color(0x106);
    urectangle(top, left, top + 0x19, left - 0xE);
    mouse_show();
}

/* Redraws the bar with the selected picture (0x20C2) and frames icon index. Both this
   and new_IconUnselect redraw the whole bar, not one icon. */
void far new_IconSelect(int index)
{
    int top, left;

    mouse_hide();
    grSoftPageFlip();
    pic_to_screen(0x20C2, 0xF3, 0x1C, 0x4D, 0x1D);
    top = 0xF3;
    left = 0x1C;
    if (index >= 3)
    {
        left -= 0xF;
        index -= 3;
    }
    top += index * 0x1A;
    grSoftPageFlip();
    set_the_color(0x106);
    urectangle(top, left, top + 0x19, left - 0xE);
    mouse_show();
}

/* The Ctrl-key shortcuts to the options panel: Ctrl+S save, Ctrl+R restore, Ctrl+M music,
   Ctrl+F sound effects, Ctrl+D detail, Ctrl+Q quit, Ctrl+O the main options group (codes
   are KEY_CTRL plus the lower-case letter). Refused while an action is in progress
   (GameInputMode, for example an object held on the cursor). Save and restore try their
   handler first and only open the panel if it did not already finish. */
void far do_option_shortcut(int keycode)
{
    gameopts_done = 0;
    if (GameInputMode)
    {
        game_sprint(0xAE);              /* "You cannot select options partway through ..." */
        mouse_release(1);
        return;
    }
    mouse_freereign();
    if (keycode == 0x173)
    {
        save_opt(0);
        if (gameopts_done)
            return;
        busywaiting_new_options(&file_buttongroup);
    }
    else if (keycode == 0x172)
    {
        restore_opt(0);
        if (gameopts_done)
            return;
        busywaiting_new_options(&file_buttongroup);
    }
    else if (keycode == 0x16D)
    {
        music_opt(0);
        if (gameopts_done)
            return;
        busywaiting_new_options(&musicsound_buttongroup);
    }
    else if (keycode == 0x166)
    {
        sound_opt(0);
        if (gameopts_done)
            return;
        busywaiting_new_options(&musicsound_buttongroup);
    }
    else if (keycode == 0x164)
        busywaiting_new_options(&detail_buttongroup);
    else if (keycode == 0x171)
        busywaiting_new_options(&quit_buttongroup);
    else if (keycode == 0x16F)
        busywaiting_new_options(&gameopts_buttongroup);
}
