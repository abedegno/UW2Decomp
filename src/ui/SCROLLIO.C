/* target: ovr139 */
/* opts: -mm -1 -G -O -Y -d */
/* The message scroll's interactive side: setting the scroll up, waiting for a key or a
   click, the [MORE] prompt, and reading a typed line or a yes/no answer from the player:
   the whole of DOS overlay ovr139, in original order. Function and global names are the
   originals from the FM Towns symbol table where it has them; the source file's own name
   is not known.

   Entry points: init_scroll (GAMESCR.C's init_gamedisp) selects and clears the message
   scroll; scroll_more is SCROLL.C's [MORE] prompt; scroll_clear empties the current
   scroll; wdialog reads a line of text typed into the scroll (the quantity to pick up,
   INVPANEL.C; a save's description, GAMEWRAP.C; the player's own words in a
   conversation, CONVERSE.C); wyorn asks a yes or no question (disarming a trap,
   INTERACT.C; MAP.C; WORLDEV.C); wd_replace and wd_bool rewrite the answer in place.
   Typing ends with Enter, Escape or any mouse click; Escape gives back the initial text
   (wdialog) or No (wyorn) and prints "-" (wdialog only) in place of the answer.

   Data owned: answer_x, where the answer starts on the current line.

   Name: descriptive (the message scroll's input side: scroll_more, wyorn). */

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "gfx.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"

/* This file's _BSS, DS:8186: where the answer starts. Only this file uses it. */
/* name: static in FM Towns, so static here; provisional name. */
static int16 answer_x;

void far init_scroll(void)
{
    scroll = &main_scroll;
    scroll_mode = 0;
    draw_scroll(0x10, 0x1E, 0xDF, 1, 0);
    draw_edges();
}

/* Waits for the input to change from what it is on entry (a key or a click), or for
   ticks of *Time when ticks is not 0, keeping the music going; the buttons must be
   released before and after. With mouse set, the cursor is shown during the wait if it
   is over the scroll. Used by the \p and \P escapes and the [MORE] prompt. */
void far scroll_wait(int ticks, char mouse)
{
    uint32 end;
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

/* The [MORE] prompt: scrolls up a line, writes [MORE] in colour 0x12 at the start of the
   new line, waits for a key or click, then erases it and allows one more line before the
   next prompt. */
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

/* The scroll answers are typed into: the menu scroll in mode 4 (a conversation), else
   the game's message scroll. */
void far pick_scroll(void)
{
    if (inplist->mode == 4)
        do_play_scroll();
    else
        do_main_scroll();
}

/* Replaces the answer typed after the prompt with the number n (INVPANEL.C, once the
   quantity has been checked). */
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

/* Reads a line typed into the scroll after prompt (or ">"), at most maxlen characters
   (50 at most) and no wider than the line; with anychar 0 only digits are accepted.
   initial is shown first and selected: pos is kept negative while it is untouched, and
   the first ordinary key replaces it. Editing keys, each with several codes (keypad,
   cursor keys and Ctrl letters as Emacs has them): Home or Ctrl+A to the start, End or
   Ctrl+E to the end, Left or Ctrl+B, Right or Ctrl+F, Backspace, Delete or Ctrl+D, Ctrl+K
   cuts to the end. The cursor block blinks by loop count, not by time. The text goes to
   result; returns the key that ended input (13 Enter, 27 Escape, or 1 to 3 for a
   click). */
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
        case KEY_CTRL | 'k':
            if (pos < 0)
                pos = -pos;
            text[pos] = 0;
            break;
        case KEY_HOME: case 0xA5: case KEY_CTRL | 'a':
            pos = 0;
            break;
        case KEY_END: case 0xAA: case KEY_CTRL | 'e':
            pos = strlen(text);
            break;
        case KEY_RIGHT: case 0xA9: case KEY_CTRL | 'f':
            if (pos < 0)
                pos = -pos;
            if (strlen(text) > pos)
                pos++;
            break;
        case KEY_LEFT: case 0xA8: case KEY_CTRL | 'b':
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
        case KEY_DEL: case KEY_CTRL | 'd':
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
        strcpy(result, NULLTRAP(initial));  /* initial is 0 for a conversation's answer */
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

/* Asks a yes or no question: the question text, or game string id when question is 0,
   then the current answer (*answer) as Yes or No, which Y and N change. Returns the key
   that ended it (13 or a mouse button) with the answer in *answer, or -1 for Escape
   (answer No). */
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

/* Empties the current scroll and resets its cursor and line count; redraw hides the
   mouse around it when the cursor is over the scroll. */
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
