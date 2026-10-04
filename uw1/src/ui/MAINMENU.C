/* target: ovr138 */
/* opts: -mm -1 -G -O -Y -d */
/* The main menu: loading its button pictures, drawing the buttons or the list of saved
   games, following the mouse and the keys, and acting on the choice. The whole of UW1's
   DOS overlay ovr138, in original order. UW1 has no symbol-bearing build; the names are
   UW2's (the FM Towns symbol table, UW2's MAINMENU.C, overlay ovr147), the code being the
   same routines.

   What it does in the game: real_start shows the menu picture DATA\OPSCR.BYT with four
   buttons from OPBTN.GR: introduction, create character, acknowledgements and journey
   onward; journey onward is offered only when a saved game exists, and with none the
   introduction plays first. Creating a character (CHARGEN.C's create_player) clears the
   SAVE0 directory, copies DATA\LEV.ARK into it, starts the conversation system and loads
   level 1 with the player at (0x20, 2). Journeying onward lists the saved games'
   descriptions and restores the one chosen. Escape on the buttons (key 0x278) quits to
   DOS.

   UW1's differences, marked "UW1:" where they are not obvious: the menu cycles palette
   colours 0x40..0x7F while it waits (ovr138_94, new in UW1); the screen is loaded from
   the file DATA\OPSCR.BYT (bltfromdrive and show, or ovr141's LoadBitMap_ovr141_0) where
   UW2 has block 5 of its art; the introduction is one
   cutscene, not two; grfx_load_font takes the font's file name (gfx.h has UW2's index,
   so the calls cast it, as BAGS.C does); there is no music theme to set and no walking
   music to start.

   Data owned: the button pictures (opbuf, buttons) and the time of the last colour step.
   Name: descriptive (the main menu: draw_start_buttons, real_start). */

#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include "conv.h"
#include "critter.h"
/* UW1: these take two arguments (no pump flag); gfx.h has UW2's three, renamed out of the
   way here. */
#define fadein UW2_fadein
#define fadeout UW2_fadeout
#include "gfx.h"
#undef fadein
#undef fadeout
#include "player.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"

void far fadein(unsigned char far *src, int count);
void far fadeout(unsigned char far *src, int count);

/* UW1: the save-file module, ovr140 (UW2's GAMEWRAP.C, ovr149). file.h is not included:
   copy_file takes a source and a destination file name, not two directories and a name. */
/* name: GetLevel and do_level_hacks are the target table's ovr140_92 and
   LevelChangeEvents_ovr140_906, which have no kin by bytes; they are UW2's GetLevel and
   do_level_hacks by position (the second and the last function of the same file, as in
   UW2's ovr149) and by this call site (GetLevel(1) > 0, then player_setup, then
   do_level_hacks(1, 0), as in UW2's real_start). */
void far get_save_descs(char descs[][40], int16 *found);
unsigned char far clear_dir(char *dir);
unsigned char far copy_file(char *src, char *dst);
int far GetLevel(int level);
void far do_level_hacks(int level, int mode);
char far RestoreGame();
unsigned char far bltfromdrive(char *name, void far *buf, unsigned n);
/* UW1: ovr141, unmatched: loads a full-screen picture file with a palette (-1: none);
   UW2 has display_screen and disk_to_vid of a block number here. Its table name. */
void far LoadBitMap_ovr141_0(int pal, char *name);

/* name: FM Towns keeps these as statics in UW2, so their names are not known. */
static unsigned char far *opbuf = 0;    /* where gronk_gr puts the next button picture */
static unsigned cycle_time = 0;         /* the low word of *Time at the last colour step */
static struct Button *buttons;          /* the menu's buttons, on real_start's stack */

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

/* UW1: every 14 ticks of *Time, rotates palette colours 0x40..0x7F by one and loads them
   into the VGA, while the menu waits for input. */
void far ovr138_94(void)
{
    if ((unsigned)GAME_TIME() - cycle_time >= 0xE) {
        rotate_bank(0x40, 0x40, 1);
        local_do_palette(0x40, 0x40);
        cycle_time = (unsigned)GAME_TIME();
    }
}

/* The introduction plays before the menu when there are no saved games. */
void far do_intro_scene(int intro)
{
    int16 found;
    char descs[4][40];

    if (intro) {
        get_save_descs(descs, &found);
        if (found == 0)
            runcutscene(0);
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
                *foreground_color = 0xA2;
                *background_color = 0xA2;
            } else {
                *foreground_color = 0xAA;
                *background_color = 0xAA;
            }
            while ((w = string_width(s[i])) > 0x13E)
                s[i][strlen(s[i]) - 1] = 0;
            string_to_screen(s[i], 0xA0 - w / 2, 100 - i * 0x16);
        }
    }
    mouse_show();
}

/* The main menu loop (intro set: play the introduction when there are no saves). Returns
   once a game has been started or restored, with the game screen set up; a restore that
   fails shows string 0x2A9 and returns to the menu. */
void far real_start(int intro)
{
    int16 found;
    char done;                          /* UW1: signed (cbw) */
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
        loop_music_maybe();
        if (choice < 4 && choice > -1) {
            opbuf = MK_FP(get_workspace(), 0);
            pal = opbuf + 0xFA00;
            bltfromdrive("DATA\\opscr.byt", opbuf, 0xFA00);
            mouse_hide();
            show(0, 0xC7, opbuf, 0xC8, 0x140, 0, 0);
            mouse_show();
            set_workspace();
            if (opbuf == 0 || !(char)gronk_gr("opbtn", 0, -1, (ArtAllocFn)adr_opbtn, (ArtMoveFn)move_opbtn))
                pfatal_code(ERR_READ | 0x00D);
            if (choice != 3) {
                draw_start_buttons(n, buttons, 0, sel);
                read_quikpal(2, pal);
                fadein(pal, 2);
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

                msg = get_string(0x2A9);
                LoadBitMap_ovr141_0(-1, "DATA\\opscr.byt");
                grfx_load_font((int)"fontbig.sys");
                *foreground_color = 0xA2;
                *background_color = 0xA2;
                string_to_screen(msg, 0xA0 - string_width(msg) / 2, 0x5A);
                mouse_show();
                while (mouse_get_input() < 0)
                    ovr138_94();
                grfx_load_font((int)"font5x6p.sys");
            } else if (done)
                punt_fightmode();
            break;
        case 1:
            fadeout(pal, 2);
            ok = create_player();
            if (ok) {
                int err;

                clear_dir("SAVE0\\");
                copy_file("DATA\\lev.ark", "SAVE0\\lev.ark");
                if ((err = init_babl()) != 0)
                    pfatal_code(err);
                done = GetLevel(1) > 0;
                if (done) {
                    player_setup(0x20, 2, 1);
                    do_level_hacks(1, 0);
                }
            }
            break;
        case 0:
            runcutscene(0);
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
}

/* Follows the mouse over the buttons (or strings) until it is clicked; returns the one
   clicked, plus n if the click was on none of them. */
int far parse_start_mouse(int n, struct Button far *b, unsigned char text)
{
    int none;
    int16 x, y;
    struct Button far *p;
    int left, bottom, w, h;
    char **s;
    register int i;
    register int last = -1;

    none = 1;
    p = b;
    if (text == 0) {
        while (mouse_get_input() > 0) {
            ovr138_94();
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
        grfx_load_font((int)"fontbig.sys");
        while (mouse_get_input() > 0) {
            ovr138_94();
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
        grfx_load_font((int)"font5x6p.sys");
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
        grfx_load_font((int)"fontbig.sys");
        draw_start_buttons(n, b, text, cur);
        grfx_load_font((int)"font5x6p.sys");
        while ((key = mouse_get_input()) < 0) {
            loop_music_maybe();
            ovr138_94();
        }
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
        case KEY_RIGHT: case KEY_DOWN: case 0xA9: case 0xAB: case KEY_CTRL | 'f': case KEY_CTRL | 'n':
            cur++;
            break;
        case KEY_UP: case KEY_LEFT: case 0xA6: case 0xA8: case KEY_CTRL | 'b': case KEY_CTRL | 'p':
            cur--;
            break;
        case 0x0D:
            result = cur;
            break;
        case KEY_HOME: case KEY_PGUP: case 0xA5: case 0xA7: case KEY_ALT | '<':
            cur = 0;
            break;
        case KEY_END: case KEY_PGDN: case 0xAA: case 0xAC: case KEY_ALT | '>':
            cur = n - 1;
            break;
        case KEY_ESC:
            if (text == 0)
                break;
        case KEY_ALT | 'x':
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
    int16 found;
    int count;
    int j;
    char *descs[4];
    char far *msg;
    char names[4][40];
    register char *p;
    register char *s;

    mouse_hide();
    LoadBitMap_ovr141_0(-1, "DATA\\opscr.byt");
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
        LoadBitMap_ovr141_0(-1, "DATA\\opscr.byt");
        msg = get_string(0x301);
        grfx_load_font((int)"fontbig.sys");
        *foreground_color = 0xA2;
        *background_color = 0xA2;
        string_to_screen(msg, (0x140 - string_width(msg)) / 2 + 10, 0x5A);
        grfx_load_font((int)"font5x6p.sys");
        if (RestoreGame(i + 1)) {
            load_weapcm();
            return 1;
        }
        return -1;
    }
    return 0;
}
