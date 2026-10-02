/* target: ovr136 */
/* opts: -mm -1 -G -O -Y -d */
/* The options panel: groups of seven buttons drawn from the "optbtns" art, a keyboard
   and mouse loop that moves the highlight and presses buttons, and the option handlers
   themselves (detail level, save and restore, music and sound, quit). The whole of DOS
   overlay ovr136, in original order. Function and global names are the originals from
   the FM Towns symbol table; the source file's own name is not known.

   Eight functions the target table lists under IDA names take the FM Towns name of the
   function at the same position, the code agreeing: ovr136_2A3 donothing_opt (empty),
   ovr136_2A8 resume_play_opt (scroll_clear(1), then ends the loop), ovr136_320
   quit_do_opt (editexit(0) when the argument is 1), ovr136_33C save_opt (check_save,
   then ShowSaveRest with save_rest 0), ovr136_3DE music_opt and ovr136_3E9 sound_opt
   (set music_sound to 0 and 1), ovr136_487 game_group_fun (new_hilit_button(0)) and
   ovr136_51E quit_group_fun (new_hilit_button(3)).

   Three DOS functions have no FM Towns counterpart: null_group_fun and
   ButtonDoNothing_ovr136_5CB are empty like donothing_opt, and done_ovr136_315 only ends the
   options loop. Their names are provisional (IDA's ovr136_482, ovr136_5CB, ovr136_315), chosen
   so that their tools/bssorder.py keys put them in the EXE's overlay stub order. They are reached only through the button-group tables, so FM Towns either folded
   them into identical functions or left them out.

   The file's _DATA is DS:190A..1934: plyregen, save_or_rest, the three picture tables,
   last_sr_file_num and "optbtns". It starts a new file after the word-alignment byte
   that follows "mobj.dat", and FM Towns keeps the same variables together in this order. */

#include "file.h"
#include "gfx.h"
#include "inv.h"
#include "player.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"
#include "view3d.h"

/* The five groups and current_buttongroup are another file's data (DS:012F..0266). */
extern struct buttongroup gameopts_buttongroup;
extern struct buttongroup quit_buttongroup;
/* This file's _BSS, DS:8178..817B, by name: save_rest 843, music_sound 925. Only this file
   uses them. (DS:8174..8177, after ovr134's ComObjData, is never referenced.) */
int save_rest;
int music_sound;

void far scroll_clear(int n);
void far editexit(int how);
void far punt_fightmode(void);
void far rectangle(int x0, int y0, int x1, int y1);

unsigned char plyregen[2] = { 0, 0 };   /* used by ovr135 */
int save_or_rest = 0;
int hilit_pics[5] = { 8, 9, 10, 11, 12 };
int unhilit_pics[5] = { 3, 4, 5, 6, 7 };
int message_pics[5] = { -1, 13, -1, 14, 15 };
char last_sr_file_num = 1;

unsigned char far is_really_button(int b)
{
    register struct buttongroup *g = current_buttongroup;

    return g->fn[b] || g->sub[b];
}

int far better_mouse_release(void)
{
    int input, x, y;
    register int button = -1;
    register int old = current_hilit_button;

    while ((input = mouse_get_input()) == 1 || input == 2 || input == 3) {
        mouse_getxy(&x, &y);
        button = get_buttonreg_button();
        if (button != -1 && is_really_button(button))
            new_hilit_button(button);
        else
            new_hilit_button(old);
    }
    return button;
}

void far busywaiting_new_options(struct buttongroup *g)
{
    register int button;
    register int input;

    reload_gr_vpic(0x20C1, "optbtns", 2);
    mouse_hide();
    pic_to_screen(0x20C1, 0xF3, 0x1C, 0x4D, 0x1D);
    mouse_show();
    install_buttongroup(g);
    gameopts_done = 0;
    while (!gameopts_done) {
        while ((input = mouse_get_input()) < 0)
            loop_music_maybe();
        switch (input) {
        case 1:
        case 2:
        case 3:
            button = better_mouse_release();
            if (button != -1)
                deal_with_button(button);
            break;
        case 0x8D:
        case 0x8F:
        case 0xA6:
        case 0xA8:
            move_hilite(-1);
            break;
        case 0x20:
        case 0x91:
        case 0x93:
        case 0xA9:
        case 0xAB:
            move_hilite(1);
            break;
        case 0x0D:
            deal_with_button(current_hilit_button);
            break;
        case 0x1B:
            gameopts_done = 1;
            scroll_clear(1);
            break;
        case 0x258:
        case 0x278:
            if (current_buttongroup == &quit_buttongroup)
                deal_with_button(2);
            break;
        case 0x85:
            if (current_buttongroup == &gameopts_buttongroup)
                deal_with_button(6);
            break;
        }
    }
    mouse_hide();
    restore_sliding_panel(!save_or_rest);
    save_or_rest = 0;
    reload_gr_vpic(0x20C1, "optbtns", 0);
    pic_to_screen(0x20C1, 0xF3, 0x1C, 0x4D, 0x1D);
    mouse_show();
    if (RightButtonThing != 0)
        new_IconSelect(mode_to_button[RightButtonThing - 1]);
}

void far move_hilite(int dir)
{
    int b;

    if (current_hilit_button != -1) {
        b = current_hilit_button + dir;
        if (b < 0)
            b += 7;
        else if (b >= 7)
            b -= 7;
        while (!is_really_button(b)) {
            if (dir > 0)
                b++;
            else
                b--;
            if (b < 0)
                b += 7;
            else if (b >= 7)
                b -= 7;
        }
        new_hilit_button(b);
        current_hilit_button = b;
    }
}

void far donothing_opt(int arg)
{
}

void far resume_play_opt(int arg)
{
    scroll_clear(1);
    gameopts_done = 1;
}

/* The level is copied to a register variable after mouse_hide: a register parameter
   is loaded in the prologue instead. */
void far detail_set_opt(int level)
{
    register int d;

    mouse_hide();
    d = level;
    player->detail = d;
    set_graphics_level();
    do_3d_view();
    grSoftPageFlip();
    render_FB();
    send_FB();
    grSoftPageFlip();
    load_buttongroup_images(1);
    detail_setting_message(0);
    mouse_show();
}

void far done_ovr136_315(int arg)
{
    gameopts_done = 1;
}

/* The duplicated store is what leaves the `jmp short $+2`. */
void far quit_do_opt(int arg)
{
    if (arg == 1) {
        editexit(0);
        gameopts_done = 1;
    } else
        gameopts_done = 1;
}

void far save_opt(int arg)
{
    unsigned char ok;

    if (!(ok = check_save()))
        gameopts_done = 1;
    else {
        save_rest = 0;
        ShowSaveRest();
    }
}

void far restore_opt(int arg)
{
    unsigned char ok;

    if (!(ok = check_rest()))
        gameopts_done = 1;
    else {
        save_rest = 1;
        ShowSaveRest();
    }
}

void far do_saverest_opt(int slot)
{
    mouse_hide();
    display_inventory_no_show = 1;
    DoSaveRest(save_rest, slot);
    display_inventory_no_show = 0;
    mouse_show();
    last_sr_file_num = slot;
    if (save_rest) {
        RightButtonThing = 0;
        punt_fightmode();
        save_or_rest = 1;
    }
    LeftPanel = 0;
    gameopts_done = 1;
}

void far music_opt(int arg)
{
    music_sound = 0;
}

void far sound_opt(int arg)
{
    music_sound = 1;
}

void far do_musicsound_opt(int on)
{
    if (music_sound == 0) {
        if (on) {
            if (!music_available()) {
                new_hilit_button(3);
                return;
            }
            turn_music(1);
            load_message(1, 0, 0);
        } else {
            turn_music(0);
            load_message(0, 0, 0);
        }
    } else {
        if (on) {
            if (!fx_available()) {
                new_hilit_button(3);
                return;
            }
            turn_fx(1);
            load_message(3, 0, 0);
        } else {
            turn_fx(0);
            load_message(2, 0, 0);
        }
    }
}

void far null_group_fun(void)
{
}

void far game_group_fun(void)
{
    new_hilit_button(0);
}

void far detail_group_fun(void)
{
    new_hilit_button(player->detail + 2);
    detail_setting_message(1);
}

void far detail_setting_message(int how)
{
    register int d = player->detail;

    load_message(d, 0, how);
}

void far load_message(int row, int col, int how)
{
    mouse_hide();
    copy_rectangle(0xEC, 0xC2 - (col << 4) - 2, 0x4F, 0x10, 0x58, 0x4F - (row << 4), how);
    mouse_show();
}

void far quit_group_fun(void)
{
    new_hilit_button(3);
}

void far file_group_fun(void)
{
    new_hilit_button(last_sr_file_num);
    if (save_rest == 1)
        load_message(1, 0, 1);
}

void far musicsound_group_fun(void)
{
    if (music_sound == 0) {
        new_hilit_button(3 - (music_is_on() != 0));
        load_message(music_is_on() != 0, 0, 1);
    } else {
        new_hilit_button(3 - (fx_is_on() != 0));
        load_message((fx_is_on() != 0) + 2, 0, 1);
    }
}

void far ButtonDoNothing_ovr136_5CB(void)
{
}

void far deal_with_button(int b)
{
    void (far *fn)(int);
    struct buttongroup *g;
    register struct buttongroup *sub;

    g = current_buttongroup;
    fn = current_buttongroup->fn[b];
    sub = current_buttongroup->sub[b];
    if (fn || sub) {
        new_hilit_button(b);
        if (fn)
            fn(g->arg[b]);
        if (!gameopts_done && sub)
            install_buttongroup(sub);
    }
}

void far new_hilit_button(int b)
{
    if (current_hilit_button != b) {
        if (current_hilit_button != -1)
            draw_button(current_hilit_button, 0);
        draw_button(b, 1);
        current_hilit_button = b;
    }
}

int far get_buttonreg_button(void)
{
    int x, y;
    register int b;

    mouse_getxy(&x, &y);
    x -= 0xEC;
    y -= 0x4E;
    b = 6 - y / 16;
    if (b < 0 || b >= 7 || x < 0 || x > 0x50)
        return -1;
    return b;
}

void far install_buttongroup(struct buttongroup *g)
{
    current_buttongroup = g;
    current_hilit_button = -1;
    if (g->images != -1) {
        load_buttongroup_images(g->images);
        copy_rectangle(0xEC, 0xC2, 0x50, 0x74, 4, 0xC6, 1);
    }
    if (g->init)
        g->init();
    blit_panel_to_main();
}

void far draw_button(int b, int hilit)
{
    copy_rectangle(0xEC, 0xC2 - (b << 4) - 2, 0x4F, 0x10, hilit ? 0x58 : 4,
                   (hilit ? 0xC6 : 0xC6) - (b << 4) - 2, 0);
}

void far copy_rectangle(int x, int y, int w, int h, int sx, int sy, int how)
{
    mouse_hide();
    grSoftPageFlip();
    if (how == 0x63)
        grSoftPageFlip();
    vcopy(sx, sy, w, h, x, y);
    grSoftPageFlip();
    set_the_color(0x106);
    if (how == 0)
        rectangle(x, y, x + w, y - h);
    if (how == 0x63)
        grSoftPageFlip();
    mouse_show();
}

void far blit_panel_to_main(void)
{
    mouse_hide();
    set_the_color(0x106);
    rectangle(0xEC, 0xC2, 0x13B, 0x4F);
    mouse_show();
}

void far load_buttongroup_images(int n)
{
    mouse_hide();
    grSoftPageFlip();
    reload_gr_vpic(0x20C3, "optbtns", unhilit_pics[n]);
    pic_to_screen(0x20C3, 4, 0xC6, 0x50, 0x74);
    reload_gr_vpic(0x20C3, "optbtns", hilit_pics[n]);
    pic_to_screen(0x20C3, 0x58, 0xC6, 0x50, 0x74);
    if (music_sound != 0 && n == 4) {
        reload_gr_vpic(0x20C4, "optbtns", 0xE);
        pic_to_screen(0x20C4, 4, 0x4F, 0x4F, 0x41);
        mouse_hide();
        copy_rectangle(4, 0xB4, 0x4F, 0x10, 4, 0x2F, 0x63);
        mouse_show();
    }
    if (message_pics[n] != -1) {
        reload_gr_vpic(0x20C4, "optbtns", message_pics[n]);
        pic_to_screen(0x20C4, 0x58, 0x4F, 0x4F, 0x41);
    }
    grSoftPageFlip();
    mouse_show();
}
