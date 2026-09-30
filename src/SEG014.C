/* target: seg014 */
/* opts: -mm -1 -G -O -d */
/* Icon bar setup/teardown, the icon hit-test, the icon select/unselect highlight boxes,
   and the options-menu keyboard-shortcut dispatcher. DOS resident segment seg014, in
   original order. Function and global names are the originals from the FM Towns symbol
   table where it has them. */

extern int GameInputMode;              /* DS:2506, declared in PLAYER.C */
extern int gameopts_done;              /* DS:67D6+22E2, also written by the options overlay */
static int icon_button_handle;         /* DS:67D6+22E4, only ever used in this file */

int far input_addmouse(int a, int b, int c, int d, int buttons, int mode, void far (*handler)(int));
void far input_del(int handle);
void far mouse_getxy(int *x, int *y);
void far mouse_hide(void);
void far mouse_show(void);
void far grSoftPageFlip(void);
void far pic_to_screen(int pic, int a, int b, int c, int d);
void far set_the_color(int color);
void far urectangle(int top, int left, int bottom, int right);
void far game_sprint(int id);
void far mouse_freereign(void);
void far mouse_release(int flag);
void far save_opt(int a);
void far restore_opt(int a);
void far music_opt(int a);
void far sound_opt(int a);
void far busywaiting_new_options(int id);
void far deal_with_icons(int button);

/* Named from the FM Towns build: setup_icon_buttons_ comes just before term_icon_buttons_
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

void far do_option_shortcut(int keycode)
{
    gameopts_done = 0;
    if (GameInputMode)
    {
        game_sprint(0xAE);
        mouse_release(1);
        return;
    }
    mouse_freereign();
    if (keycode == 0x173)
    {
        save_opt(0);
        if (gameopts_done)
            return;
        busywaiting_new_options(0x1E9);
    }
    else if (keycode == 0x172)
    {
        restore_opt(0);
        if (gameopts_done)
            return;
        busywaiting_new_options(0x1E9);
    }
    else if (keycode == 0x16D)
    {
        music_opt(0);
        if (gameopts_done)
            return;
        busywaiting_new_options(0x227);
    }
    else if (keycode == 0x166)
    {
        sound_opt(0);
        if (gameopts_done)
            return;
        busywaiting_new_options(0x227);
    }
    else if (keycode == 0x164)
        busywaiting_new_options(0x16D);
    else if (keycode == 0x171)
        busywaiting_new_options(0x1AB);
    else if (keycode == 0x16F)
        busywaiting_new_options(0x12F);
}
