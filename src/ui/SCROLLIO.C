/* target: ovr139 */
/* opts: -mm -1 -G -O -Y -d */
/* The message scroll's interactive side: setting the scroll up, waiting for a key or a
   click, the [MORE] prompt, and reading a typed line or a yes/no answer from the player:
   the whole of DOS overlay ovr139, in original order. Function and global names are the
   originals from the FM Towns symbol table where it has them; the source file's own name
   is not known. */

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "gfx.h"
#include "sound.h"
#include "ui.h"

extern unsigned char far *foreground_color;      /* DS:21C4 */
extern struct FontInfo far *cur_font;                /* DS:21CC, provisional */
extern unsigned long far *Time;                  /* DS:2158; SCROLL.C calls it
                                                    PITTimerGlobal */
extern struct Inplist near *inplist;             /* DS:E4 */
/* This file's _BSS, DS:8186: where the answer starts. Static in FM Towns, so static here;
   provisional name. Only this file uses it. */
static int answer_x;

int far scroll_print(char far *s);
void far mouse_release(int how);
void far rectangle(int x0, int y0, int x1, int y1);

void far init_scroll(void)
{
    scroll = &main_scroll;
    scroll_mode = 0;
    draw_scroll(0x10, 0x1E, 0xDF, 1, 0);
    draw_edges();
}

/* Declared here because TLINK numbers the overlay's stub entries in the order Turbo C lists
   the publics, which for names with the same hash key is the order they were first seen:
   the EXE's stub has scroll_clear and wdialog before scroll_wait and wd_bool. */
void far scroll_clear(char redraw);
int far wdialog(char *prompt, char *initial, char *result, char anychar, int maxlen);

void far scroll_wait(int ticks, char mouse)
{
    unsigned long end;
    int key;

    mouse_release(1);
    key = mouse_get_input();
    end = *Time + ticks;
    if (mouse_in_scroll && mouse)
        mouse_show();
    while (mouse_get_input() == key) {
        change_music_maybe();
        if (ticks && *Time > end)
            break;
    }
    mouse_release(1);
    set_mouse_in();
    if (mouse_in_scroll & mouse)
        mouse_hide();
}

void far scroll_more(void)
{
    int color;
    int y;

    y = scroll->cur_y - cur_font->height;
    scroll_up(y);
    y = scroll->cur_y;
    color = *foreground_color;
    *foreground_color = 0x12;
    string_to_screen("[MORE]", scroll->left, y);
    scroll_wait(0, 1);
    set_the_color(0x71);
    rectangle(scroll->left, y, scroll->bottom, scroll->y0);
    *foreground_color = color;
    start_line = scroll->start_line - 1;
    scroll->cur_x = scroll->left;
}

void far pick_scroll(void)
{
    if (inplist->mode == 4)
        do_play_scroll();
    else
        do_main_scroll();
}

void far wd_replace(int n)
{
    char buf[6];
    int y;
    int x;

    itoa(n, buf, 10);
    x = answer_x;
    y = scroll->cur_y;
    pick_scroll();
    *foreground_color = scroll->font_color;
    set_the_color(0x71);
    rectangle(x, y, scroll->cur_x, y - cur_font->height + 1);
    scroll->cur_x = answer_x;
    scroll_print(buf);
}

void far wd_bool(char yes)
{
    int y;
    int x;

    x = answer_x;
    y = scroll->cur_y;
    pick_scroll();
    *foreground_color = scroll->font_color;
    set_the_color(0x71);
    rectangle(x, y, scroll->cur_x, y - cur_font->height + 1);
    scroll->cur_x = answer_x;
    if (yes)
        scroll_print("Yes");
    else
        scroll_print("No");
}

int far wdialog(char *prompt, char *initial, char *result, char anychar, int maxlen)
{
    int width;
    int blink;
    int x;
    int y;
    int key;
    char text[52];
    char tmp[52];
    int pos;
    int i;

    if (maxlen > 50)
        maxlen = 50;
    pick_scroll();
    *foreground_color = scroll->font_color;
    if (prompt) {
        width = scroll->bottom - (string_width(prompt) + scroll->left) - 20;
        scroll_print(prompt);
    } else {
        width = scroll->bottom - (string_width(">") + scroll->left) - 20;
        scroll_print(">");
    }
    answer_x = scroll->cur_x;
    if (initial) {
        scroll_esc = 0;
        scroll_print(initial);
        scroll_esc = 1;
        strcpy(text, initial);
        pos = strlen(initial) * -1;
    } else
        pos = 0;
    text[abs(pos)] = 0;
    blink = 3000;
    x = answer_x + string_width(text);
    y = scroll->cur_y;
    mouse_hide();
    mouse_release(0);
    while ((key = mouse_get_input()) != 13 && key != 27 && key != 1 && key != 2 && key != 3) {
        strcpy(tmp, text);
        tmp[abs(pos)] = 0;
        x = answer_x + string_width(tmp);
        if (blink >= 2000 && blink < 4000) {
            set_the_color(scroll->font_color);
            rectangle(x, y, x + 4, y - cur_font->height + 1);
        } else if (blink == 4000) {
            set_the_color(0x71);
            blink = 0;
            rectangle(x, y, x + 4, y - cur_font->height + 1);
            if (strlen(text) > pos)
                string_to_screen(text, answer_x, scroll->cur_y);
        }
        blink++;
        switch (key) {
        case -1:
            continue;
        case 0x16B:
            if (pos < 0)
                pos = -pos;
            text[pos] = 0;
            break;
        case 0x8C: case 0xA5: case 0x161:
            pos = 0;
            break;
        case 0x92: case 0xAA: case 0x165:
            pos = strlen(text);
            break;
        case 0x91: case 0xA9: case 0x166:
            if (pos < 0)
                pos = -pos;
            if (strlen(text) > pos)
                pos++;
            break;
        case 0x8F: case 0xA8: case 0x162:
            if (pos < 0)
                pos = -pos;
            if (pos > 0)
                pos--;
            break;
        case 8:
            if (pos < 0)
                pos = -pos;
            if (pos > 0) {
                for (i = pos; i <= strlen(text); i++)
                    text[i - 1] = text[i];
                pos--;
            }
            break;
        case 0x96: case 0x164:
            if (pos < 0)
                pos = -pos;
            if (strlen(text) > pos)
                for (i = pos; i < strlen(text); i++)
                    text[i] = text[i + 1];
            break;
        default:
            if (pos < 0) {
                pos = 0;
                text[0] = 0;
            }
            if (key >= 0x20 && key <= 0x7E && string_width(text) < width
                    && strlen(text) < maxlen && (anychar || isdigit(key))) {
                i = strlen(text);
                text[i + 1] = 0;
                for (; i > pos; i--)
                    text[i] = text[i - 1];
                text[pos] = key;
                pos++;
            }
            break;
        }
        set_the_color(0x71);
        rectangle(answer_x, scroll->cur_y, scroll->bottom, scroll->cur_y - cur_font->height + 1);
        *foreground_color = scroll->font_color;
        string_to_screen(text, answer_x, scroll->cur_y);
    }
    if (blink >= 2000) {
        set_the_color(0x71);
        rectangle(x, y, x + 4, y - cur_font->height + 1);
        if (strlen(text) > pos)
            string_to_screen(text, answer_x, scroll->cur_y);
    }
    mouse_show();
    if (key == 27) {
        strcpy(result, initial);
        set_the_color(0x71);
        rectangle(answer_x, scroll->cur_y, scroll->bottom, scroll->cur_y - cur_font->height + 1);
        scroll->cur_x = answer_x;
        scroll_print("-");
    } else {
        strcpy(result, text);
        scroll->cur_x = x;
    }
    return key;
}

int far wyorn(char *question, int id, char *answer)
{
    char cur;
    char sel;
    int key;

    cur = *answer;
    sel = *answer;
    pick_scroll();
    *foreground_color = scroll->font_color;
    if (question == 0)
        game_sprint(id);
    else
        scroll_print(question);
    answer_x = scroll->cur_x;
    if (*answer)
        scroll_print("Yes");
    else
        scroll_print("No");
    mouse_hide();
    mouse_release(0);
    while ((key = mouse_get_input()) != 13 && key != 27 && key != 1 && key != 2 && key != 3) {
        switch (key) {
        case 'Y': case 'y':
            sel = 1;
            break;
        case 'N': case 'n':
            sel = 0;
            break;
        default:
            continue;
        }
        if (sel != cur) {
            wd_bool(sel);
            cur = sel;
        }
    }
    mouse_show();
    if (key == 27) {
        wd_bool(0);
        *answer = 0;
        return -1;
    }
    *answer = cur;
    return key;
}

void far scroll_clear(char redraw)
{
    if (redraw) {
        set_mouse_in();
        if (mouse_in_scroll)
            mouse_hide();
    }
    set_the_color(0x71);
    rectangle(scroll->top, scroll->x0, scroll->bottom, scroll->y0);
    scroll->cur_y = scroll->last_y;
    scroll->cur_x = scroll->left;
    scroll->more_pending = 0;
    scroll->start_line = 0;
    if (scroll != &npc_scroll)
        draw_edges();
    else
        draw_conv_edges();
    if (redraw && mouse_in_scroll)
        mouse_show();
}
