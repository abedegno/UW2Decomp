/* target: ovr147 */
/* opts: -mm -1 -G -O -Y -d */
/* The main menu: loading its button pictures, drawing the buttons or the list of saved
   games, following the mouse and the keys, and acting on the choice. The whole of DOS
   overlay ovr147, in original order. Function names are the originals from the FM Towns
   symbol table; the source file's own name is not known.

   adr_opbtn and move_opbtn (IDA ovr147_0 and ovr147_22) are the FM Towns functions just
   before do_intro_scene, and gronk_gr's callbacks for "opbtn" in both builds.
   UnknownAutomapLoop_ovr147_A56 and its callback have no FM Towns counterpart and no
   caller in DOS. The loop keeps its IDA name; the callback (IDA's
   UnknownCallBackFunctionForAutomap_ovr147_A73) has a provisional name chosen so that its
   tools/bssorder.py key puts it in the EXE's overlay stub order. */

#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include "conv.h"
#include "critter.h"
#include "file.h"
#include "gfx.h"
#include "player.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"

extern unsigned char far *foreground_color;
extern struct FontInfo far *cur_font;

/* FM Towns keeps these as statics, so their names are not known. */
static unsigned char far *opbuf = 0;    /* where gronk_gr puts the next button picture */
static struct Button *buttons;          /* the menu's buttons, on real_start's stack */

void far show_cutscene(int n);
void far show(int x, int y, unsigned char far *buf, int h, int w, int a, int b);
void far set_new_music(int n);
unsigned far get_workspace(void);
char far disk_to_vid(int blk, unsigned char far *buf);
char far display_screen(int pal, int blk);
unsigned char far gronk_gr(char *name, int a, int b, unsigned char far *(far *adr)(int),
                           int (far *move)(unsigned char far *, int, int));
unsigned char far read_quikpal(int which, unsigned char far *pal);
void far fadeout(unsigned char far *pal, int steps, int x);
void far punt_fightmode(void);
void far preload_cr(int n);
void far free_world(int n);
char far RestoreGame();                 /* no prototype: the slot is pushed as an int */
void far load_weapcm(void);
void far automap_area(int x0, int y0, int x1, int y1, int *arg, char (far *fn)());

/* gronk_gr's callbacks while loading the menu buttons: where to put the next picture, and
   recording it. Pictures alternate: a button's normal picture, then its selected one. */
unsigned char far * far adr_opbtn(int size)
{
    unsigned char far *p;

    p = opbuf;
    opbuf += size;
    return p;
}

int far move_opbtn(unsigned char far *p, int size, register int n)
{
    buttons[n >> 1].img[n & 1] = p + 5;
    if (!(n & 1)) {
        n >>= 1;
        buttons[n].w = p[1];
        buttons[n].h = p[2];
    }
    return size != 0;
}

/* The introduction plays before the menu when there are no saved games. */
void far do_intro_scene(int intro)
{
    int found;
    char descs[4][40];

    if (intro) {
        get_save_descs(descs, &found);
        if (found == 0) {
            show_cutscene(0);
            show_cutscene(1);
        }
    }
}

/* Draws n buttons, or n strings (the saved games) when text is set, sel highlighted. */
void far draw_start_buttons(int n, struct Button far *b, unsigned char text, int sel)
{
    int w;
    struct Button far *p;
    unsigned char far *img;
    register int i;
    register char **s;

    if (text == 0) {
        mouse_hide();
        for (i = 0; i < n; i++) {
            p = b;
            img = p[i].img[i == sel];
            show(p[i].x, p[i].y, img, p[i].h, p[i].w, 0, 0);
        }
    } else {
        mouse_hide();
        for (i = 0; i < n; i++) {
            s = (char **)b;
            if (i == sel) {
                *foreground_color = 7;
                *background_color = 7;
            } else {
                *foreground_color = 0xCD;
                *background_color = 0xCD;
            }
            while ((w = string_width(s[i])) > 0x13E)
                s[i][strlen(s[i]) - 1] = 0;
            string_to_screen(s[i], 0xA0 - w / 2, 100 - i * 0x16);
        }
    }
    mouse_show();
}

void far real_start(int intro)
{
    int found;
    unsigned char done;
    char ok;
    int n;
    int sel;
    unsigned char far *pal;
    register int choice = 0;
    register int r;
    char descs[4][40];
    struct Button btns[4] = {
        { { 0, 0 }, 0x62, 0x76, 1, 1 },
        { { 0, 0 }, 0x51, 0x5F, 1, 1 },
        { { 0, 0 }, 0x48, 0x47, 1, 1 },
        { { 0, 0 }, 0x55, 0x2E, 1, 1 },
    };

    buttons = btns;
    get_save_descs(descs, &found);
    n = found == 0 ? 3 : 4;
    sel = found == 0 ? 1 : 3;
    do_intro_scene(intro);
    force_mouse_cursor(0x106C);
    mouse_show();
    done = 0;
    while (!done) {
        set_new_music(1);
        change_music_maybe();
        if (choice < 4 && choice > -1) {
            opbuf = MK_FP(get_workspace(), 0);
            pal = opbuf + 0xFA00;
            mouse_hide();
            disk_to_vid(5, opbuf);
            mouse_show();
            set_workspace();
            if (opbuf == 0 || !gronk_gr("opbtn", 0, -1, adr_opbtn, move_opbtn))
                pfatal_code(ERR_READ | 0x00D);
            if (choice != 3) {
                draw_start_buttons(n, buttons, 0, sel);
                read_quikpal(2, pal);
                fadein(pal, 2, 0);
            }
        }
        choice = parse_start_input(n, buttons, 0, sel);
        if (choice < 4 && choice > -1)
            release_workspace();
        switch (choice) {
        case 2:
            show_credits();
            break;
        case 3:
            r = do_journey();
            done = r == 1;
            if (r == -1) {
                char far *msg;

                msg = get_string(0x2B8);
                disk_to_vid(5, 0);
                grfx_quikfont(FONT_BIG);
                *foreground_color = 7;
                *background_color = 7;
                string_to_screen(msg, 0xA0 - string_width(msg) / 2, 0x5A);
                mouse_show();
                while (mouse_get_input() < 0)
                    ;
                grfx_quikfont(FONT_5X6P);
            } else if (done)
                punt_fightmode();
            break;
        case 1:
            fadeout(pal, 2, 0);
            ok = create_player();
            if (ok) {
                int err;

                clear_dir(HomeDir);
                move_initial_files();
                if ((err = init_babl()) != 0)
                    pfatal_code(err);
                done = GetLevel(1) > 0;
                if (done) {
                    load_dl();
                    player_setup(0x13, 0x30, -1);
                    do_level_hacks(1, 0);
                    preload_cr(0);
                    PreLoadCritPages();
                }
            }
            break;
        case 0:
            show_cutscene(0);
            show_cutscene(1);
            break;
        case -1:
            free_world(0);
            exit(1);
        }
    }
    unforce_mouse_cursor(3);
    mouse_show();
    change_screen(1);
    editchng(0x7FFE);
    LeftPanel = 0;
    set_random_walking_music(0);
}

/* Follows the mouse over the buttons (or strings) until it is clicked; returns the one
   clicked, plus n if the click was on none of them. */
int far parse_start_mouse(int n, struct Button far *b, unsigned char text)
{
    int none;
    int x, y;
    struct Button far *p;
    int left, bottom, w, h;
    char **s;
    register int i;
    register int last = -1;

    none = 1;
    p = b;
    if (text == 0) {
        while (mouse_get_input() > 0) {
            mouse_getxy(&x, &y);
            for (i = 0; i < n; i++)
                if (p[i].x <= x && p[i].x + p[i].w - 1 >= x &&
                    p[i].y - p[i].h + 1 <= y && p[i].y >= y) {
                    none = 1;
                    if (i != last) {
                        draw_start_buttons(n, b, text, i);
                        last = i;
                    }
                    break;
                }
            if (i == n)
                none = last == -1;
        }
    } else {
        s = (char **)b;
        grfx_quikfont(FONT_BIG);
        while (mouse_get_input() > 0) {
            mouse_getxy(&x, &y);
            for (i = 0; i < n; i++) {
                w = string_width(s[i]);
                left = (0x140 - w) / 2;
                bottom = 100 - i * 0x16;
                h = cur_font->height;
                if (x >= left && left + w > x && y <= bottom && bottom - h < y) {
                    none = 1;
                    if (i != last) {
                        draw_start_buttons(n, b, text, i);
                        last = i;
                    }
                    break;
                }
            }
            if (i == n)
                none = last == -1;
        }
        grfx_quikfont(FONT_5X6P);
    }
    return last + (none ? 0 : n);
}

/* Runs the menu until a choice is made; returns it, or -1 for Escape where allowed. */
int far parse_start_input(register int n, struct Button far *b, int text, int sel)
{
    int result = -2;
    int key;
    int hit;
    register int cur = sel;

    while (result < -1) {
        grfx_quikfont(FONT_BIG);
        draw_start_buttons(n, b, text, cur);
        grfx_quikfont(FONT_5X6P);
        while ((key = mouse_get_input()) < 0)
            loop_music_maybe();
        switch (key) {
        case 1:
        case 2:
        case 3:
            hit = parse_start_mouse(n, b, text);
            if (hit >= 0 && hit < n && hit != result)
                result = cur = hit;
            else if (hit >= n)
                cur = hit - n;
            break;
        case 0x91: case 0x93: case 0xA9: case 0xAB: case 0x166: case 0x16E:
            cur++;
            break;
        case 0x8D: case 0x8F: case 0xA6: case 0xA8: case 0x162: case 0x170:
            cur--;
            break;
        case 0x0D:
            result = cur;
            break;
        case 0x8C: case 0x8E: case 0xA5: case 0xA7: case 0x23C:
            cur = 0;
            break;
        case 0x92: case 0x94: case 0xAA: case 0xAC: case 0x23E:
            cur = n - 1;
            break;
        case 0x1B:
            if (text == 0)
                break;
        case 0x278:
            result = -1;
            break;
        }
        if (cur < 0)
            cur = 0;
        else if (cur >= n)
            cur = n - 1;
    }
    return result;
}

/* Lists the saved games and restores the one chosen: 1 if restored, -1 if that failed,
   0 if none was chosen. */
int far do_journey(void)
{
    int i;
    int found;
    int count;
    int j;
    char *descs[4];
    char far *msg;
    char names[4][40];
    register char *p;
    register char *s;

    mouse_hide();
    display_screen(-1, 5);
    mouse_show();
    get_save_descs(names, &found);
    for (i = count = 0; i < 4; i++)
        if (found & (1 << i)) {
            s = names[i];
            p = &names[i][38];
            while (*p == ' ' && p > s)
                p--;
            p[1] = 0;
            descs[count] = s;
            count++;
        }
    i = parse_start_input(count, (struct Button far *)descs, 1, 0);
    if (i >= 0) {
        for (count = j = -1; j != i; count++)
            if (found & (1 << (count + 1)))
                j++;
        i = count;
        mouse_hide();
        display_screen(-1, 5);
        msg = get_string(0x311);
        grfx_quikfont(FONT_BIG);
        *foreground_color = 7;
        *background_color = 7;
        string_to_screen(msg, (0x140 - string_width(msg)) / 2 + 10, 0x5A);
        grfx_quikfont(FONT_5X6P);
        if (RestoreGame(i + 1)) {
            load_weapcm();
            return 1;
        }
        return -1;
    }
    return 0;
}

void far UnknownAutomapLoop_ovr147_A56(void)
{
    automap_area(0x12, 0x1E, 0x2C, 0x34, 0, Region_ovr147_A73);
}

char far Region_ovr147_A73(register int x, register int y)
{
    if (x == 0x23)
        return y > 0x21;
    if (y < 0x20)
        return x >= 0x1E && x <= 0x20;
    if (x < 0x14)
        return y <= 0x31;
    return 1;
}
