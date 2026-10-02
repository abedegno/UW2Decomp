/* target: seg043_3619 */
/* opts: -mm -1 -G -O -d */
/* The message scroll: the strip of text at the bottom of the game screen (and its
   counterparts in conversations and the menus), printing to it with word wrap, colour and
   pause escapes, scrolling it up a line, and its animated edges. DOS resident segment
   seg043_3619, in original order. Function names are the originals from the FM Towns
   symbol table.

   Entry point: scroll_print(s) prints a far string to the current scroll (scroll, chosen
   by do_main_scroll, do_npc_scroll or do_play_scroll) and returns scroll->start_line. The
   text passes through four stages: scroll_print cuts it into pieces of at most 49
   characters at spaces, scroll_print1 splits those at backslash escapes, scroll_print2 at
   newlines, and scroll_print3 prints one piece, handling an escape at its start and
   wrapping by word (scroll_wrap) when it does not fit. Escapes (only while scroll_esc is
   set; SCROLLIO.C's wdialog clears it to echo typed text): \0 to \6 set the colour from
   spec_col, \p and \P pause for 600 and 200 ticks or a click (SCROLLIO.C's
   scroll_wait), \m forces the [MORE] prompt. A newline is not drawn: it sets
   more_pending so the next piece starts a new line, scrolling the box (scroll_up) or
   asking for [MORE] (SCROLLIO.C's scroll_more) when it is full. Only screen modes 1 and 4
   (the 3D view and conversations) print; elsewhere scroll_print returns -1.

   Data owned: the three Scroll records (main_scroll, the game's message scroll; npc_scroll,
   a conversation's; menu_scroll, used in mode 4 by SCROLLIO.C's pick_scroll), the escape
   colours, the current scroll and its mode, start_line and the edge animation phases.

   The [MORE] rule: scroll->start_line counts the lines filled since the box was cleared,
   and each scroll_print copies it to start_line, the number of older lines that may
   scroll out of sight; each scroll_up uses one. When a new line is needed in a full box
   and none is left, scroll_more shows [MORE] and waits. So (inferred) the player is only
   stopped when the current message's own lines would scroll away unread. The scroll's field
   names in ui.h do not say what they hold; see struct Scroll there.

   Neighbours: game_sprint and the other GAMESTRN.C printers end here; SCROLLIO.C has the
   interactive side (the [MORE] prompt, typed answers, clearing).

   Name: descriptive (the message scroll: scroll_print, draw_scroll). */

/* name: set_mouse_in and do_main_scroll were first identified by exact structural
   correspondence with FM Towns set_mouse_in_ and do_main_scroll_ (see their comments);
   map/functions.tsv now lists both as anchors. draw_scroll is FM Towns draw_scroll_, the
   same two rectangles. The last function, seg043_3619_669, has no FM Towns counterpart. */

#include <dos.h>
#include <string.h>
#include "gfx.h"
#include "sys.h"
#include "ui.h"

/* This file's _BSS, DS:34B0..34B3 (seg044's starts at 34B4). */
/* match: laid out by name: scroll 83, mouse_in_scroll 653. It cannot start any lower
   than 34AC: the bytes below run up from sound_fpage (key 363), which seg042 uses. */
struct Scroll near *scroll;                      /* DS:34B0, FM Towns _scroll */
unsigned char mouse_in_scroll;                   /* DS:34B2, FM Towns _mouse_in_scroll */
/* This file's _DATA, DS:0938..098D, in definition order (seg042's data ends at 0937, odd;
   seg044's starts at 098E). FM Towns has main_scroll, npc_scroll, menu_scroll and spec_col
   together in this order and then unnamed space: click_time, edge_phase and
   conv_edge_phase have no names there, so they were static. The pool holds one string,
   the "" at DS:098D that scroll_wrap passes to scroll_print3. */
struct Scroll main_scroll = { 0x1E, 1, 0x10, 0xDF, 0x10, 0x1E, 0x10, 0x1E, 0, 0, 0x76 };
/* DS:0938 */
struct Scroll npc_scroll = { 0x78, 0x28, 0x15, 0xDA, 0x18, 0x76, 0x18, 0x76, 0, 0, 0x76 };
/* DS:094D */
struct Scroll menu_scroll = { 0x1E, 1, 8, 0x137, 8, 0x1E, 8, 0x1E, 0, 0, 0x76 };
/* DS:0962 */
unsigned char spec_col[7] = { 0x75, 0x68, 0x01, 0x02, 0x21, 0x50, 0x48 };    /* DS:0977, colours for \0..\6 */
int scroll_mode = 0;                    /* DS:097E */
int start_line = 0;                     /* DS:0980 */
unsigned char menus_active = 0;         /* DS:0982 */
unsigned char didMouseInput = 0;        /* DS:0983 */
static long click_time = 0;             /* DS:0984 */
static int edge_phase = 0;              /* DS:0988 */
static int conv_edge_phase = 0;         /* DS:098A */
char scroll_esc = 1;                    /* DS:098C */

/* Whether the cursor touches the current scroll's box, so printing must hide it first. */
/* name: not anchored in the map when named; the target table keeps the IDA name. This is
   FM Towns set_mouse_in_ by exact correspondence (same four Scroll fields, same order,
   into the same mouse_check_reg_ call, storing the result to the same mouse_in_scroll
   byte). */
void far set_mouse_in(void)
{
    mouse_in_scroll = mouse_check_reg(scroll->top, scroll->y0, scroll->bottom, scroll->x0);
}

/* Selects the game's message scroll for printing (scroll_mode 0). */
/* name: not anchored either when named. This is FM Towns do_main_scroll_: it sits
   immediately before do_npc_scroll_/do_play_scroll_ there, and sets the same three fields
   (mode 0, didMouseInput 1, scroll pointer) that do_npc_scroll_/do_play_scroll_ set with
   1/2. */
void far do_main_scroll(void)
{
    scroll_mode = 0;
    didMouseInput = 1;
    scroll = &main_scroll;
}

void far do_npc_scroll(void)
{
    scroll_mode = 1;
    didMouseInput = 1;
    scroll = &npc_scroll;
}

void far do_play_scroll(void)
{
    scroll_mode = 2;
    didMouseInput = 1;
    scroll = &menu_scroll;
}

/* Advances the message scroll's rolled-paper edges, left and right of the text, by one of
   five animation frames (pictures 0x20AB.. and 0x20B0..); called each time the text
   scrolls, so the paper appears to roll. */
void far draw_edges(void)
{
    pic_to_screen(edge_phase + 0x20AB, scroll->top - 4, 0x1E, 0x1C, 4);
    pic_to_screen(edge_phase + 0x20B0, scroll->bottom + 1, 0x1E, 0x1C, 4);
    edge_phase++;
    if (edge_phase == 5)
        edge_phase = 0;
}

/* The same for the conversation scroll's edges: three tiles each side, six frames. */
void far draw_conv_edges(void)
{
    register int i;

    for (i = 0; i < 3; i++) {
        pic_to_screen(conv_edge_phase + 0x20B5, 0x11, 0x78 - i * 0x1B, 0x1B, 5);
        pic_to_screen(conv_edge_phase + 0x20BB, 0xDB, 0x78 - i * 0x1B, 0x1B, 5);
    }
    conv_edge_phase++;
    if (conv_edge_phase == 6)
        conv_edge_phase = 0;
}

/* Scrolls the text up by one line height (the box's contents above the last line are
   moved up), clears the freed line in the paper colour 0x71 and rolls the edges. */
void far scroll_up(int n)
{
    vcopy(scroll->top, scroll->last_y - cur_font->height,
          scroll->bottom - scroll->top + 1, scroll->last_y - cur_font->height - n + 1,
          scroll->top, scroll->last_y);
    set_the_color(0x71);
    rectangle(scroll->top, scroll->cur_y, scroll->bottom, n + 1);
    if (scroll != &npc_scroll)
        draw_edges();
    else
        draw_conv_edges();
}

int far scroll_print(char far *s)
{
    /* match: 0x31 (49) bytes are copied into this buffer below; it is declared two bytes short
       of that because `found`, as a register variable, still reserves a two-byte spill
       slot right after it, and the true end of the 49-byte region is that slot, used
       below as `sentinel`. Matched against the EXE's frame size (sub sp,36h) and the
       offsets of `sentinel` and `saved`, not guessed. */
    char copy[47];
    register char *found;
    register int remaining;
    char saved;
    int chunklen;
    char pad1;                 /* match: unused; the EXE reserves it too (frame 0x36) */
    char sentinel;

    if (scrmode != 1 && scrmode != 4)
        return -1;

    set_mouse_in();
    if (mouse_in_scroll)
        mouse_hide();
    if (menus_active && scroll_mode != 1) {
        menus_active = 0;
        scroll_clear(0);
    }
    start_line = scroll->start_line;
    didMouseInput = 0;
    *foreground_color = (unsigned char)scroll->font_color;
    *background_color = 0x71;

    chunklen = 0;
    remaining = str_len(s);
    while (remaining > 0x31) {
        movedata(FP_SEG(s), FP_OFF(s), FP_SEG(copy), FP_OFF(copy), 0x31);
        sentinel = 0;
        found = strrchr(copy, ' ');
        if (found == 0)
            found = &sentinel;
        saved = *found;
        *found = 0;
        chunklen = found - copy;
        scroll_print1(copy, 1);
        s += chunklen;
        *found = saved;
        remaining -= chunklen;
    }
    movedata(FP_SEG(s), FP_OFF(s), FP_SEG(copy), FP_OFF(copy), remaining + 1);
    scroll_print1(copy, 0);

    click_time = *Time;
    if (mouse_in_scroll)
        mouse_show();
    return scroll->start_line;
}

/* Splits at backslash escapes, so each escape starts a piece. flag says whether the text
   continues after this piece (scroll_print passes 1 for all but the last 49-character
   chunk); a piece followed by more text, or by \m, is printed with 1. */
void far scroll_print1(char *text, int flag)
{
    char *di;
    char *found;

    di = text;
    while ((found = strchr(di + 1, '\\')) != 0) {
        *found = 0;
        if (found[2] != 0 || found[1] == 'm')
            scroll_print2(di, 1);
        else
            scroll_print2(di, flag);
        *found = '\\';
        di = found;
    }
    scroll_print2(di, flag);
}

/* Splits at newlines, keeping each newline at the end of its piece. */
void far scroll_print2(char *text, int flag)
{
    char saved, unused;
    char *si;
    char *di;

    di = text;
    while ((si = strchr(di, '\n')) != 0) {
        if (si[1] == 0)
            break;
        saved = si[1];
        si[1] = 0;
        scroll_print3(di, 1);
        si[1] = saved;
        di = si + 1;
    }
    scroll_print3(di, flag);
}

/* Prints one piece. A leading escape is acted on first (\P waits 200 ticks, \p falls
   into it with 400 more, so 600). If the previous piece ended a line, a new line is
   started: when the box is full, either the text scrolls up (while start_line - flag is
   not negative, that is while more lines may pass without the player seeing them) or
   the [MORE] prompt is shown. A piece too wide for the rest of the line goes to
   scroll_wrap; otherwise it is drawn, a final newline being dropped and remembered in
   more_pending. */
void far scroll_print3(char *text, int flag)
{
    int width;
    int idx;
    int code_val;
    char *si;
    int y;

    si = text;
    code_val = 0;
    if (scroll_esc && *si == '\\') {
        si++;
        switch (*si) {
        case 'p':
            code_val = 0x190;
        case 'P':
            code_val += 0xC8;
            scroll_wait(code_val, 1);
            break;
        case 'm':
            scroll_more();
            break;
        default:
            if (*si >= '0' && *si <= '6')
                *foreground_color = spec_col[*si - '0'];
            break;
        }
        scroll->font_color = *foreground_color;
        si++;
    }

    if (scroll->more_pending) {
        y = scroll->cur_y - cur_font->height;
        if (start_line - flag < 0) {
            if (y - cur_font->height < scroll->y0 - 1) {
                scroll_more();
                y = scroll->cur_y;
            } else {
                scroll->start_line++;
            }
        } else {
            if (y - cur_font->height < scroll->y0 - 1) {
                scroll_up(y);
                y = scroll->cur_y;
                start_line--;
            } else {
                scroll->start_line++;
            }
        }
        scroll->cur_y = y;
        scroll->cur_x = scroll->left;
        scroll->more_pending = 0;
    }

    width = scroll->cur_x + string_width(si);
    if (scroll->bottom <= width) {
        scroll_wrap(si, flag);
    } else {
        idx = strlen(si) - 1;
        if (si[idx] == '\n') {
            si[idx] = 0;
            scroll->more_pending = 1;
        }
        string_to_screen(si, scroll->cur_x, scroll->cur_y);
        scroll->cur_x += string_width(si);
    }
}

/* Word wrap: prints as much of the text as fits before the right edge, breaking at the
   last space (or, for a single word longer than a line, mid-word on a fresh line), and
   continues on new lines, dropping the spaces at each break. */
void far scroll_wrap(char *text, int flag)
{
    char *end;
    char saved;
    char unused;
    char *si;
    char *di;

    si = text;
    end = si + strlen(si);
    while (si < end) {
        saved = 0;
        if (scroll->cur_x >= scroll->bottom) {
            scroll->more_pending = 1;
            scroll_print3("", flag);
            while (*si == ' ')
                si++;
        }
        if (scroll->cur_x + string_width(si) >= scroll->bottom) {
            di = strrchr(si, ' ');
            if (di != 0) {
                while (di[-1] == ' ' && di > si)
                    di--;
            }
            if (di == 0 || di == si) {
                scroll->more_pending = 1;
                scroll_print3("", flag);
                while (*si == ' ')
                    si++;
                di = si + strlen(si);
                while (di > si && scroll->cur_x + string_width(si) - string_width(di) >= scroll->bottom)
                    di--;
            }
            saved = *di;
            *di = 0;
        }
        scroll_print3(si, flag);
        si += strlen(si);
        *si = saved;
        while (*si == 0)
            si++;
    }
}

/* Clears a scroll box to the paper colour 0x71. Despite the parameter names the four
   values are the box's left x, top y, right x and bottom y, as the graphics library's
   rectangle takes them (SCROLLIO.C's init_scroll passes 0x10, 0x1E, 0xDF, 1). With flag
   set a frame in colour 1 is drawn first, 14 pixels in on the left. */
void far draw_scroll(int x, int y, int w, int h, char flag)
{
    if (flag) {
        set_the_color(1);
        rectangle(x + 0xE, y - 1, w - 0xF, h + 1);
    }
    set_the_color(0x71);
    rectangle(x, y, w, h);
}

/* The segment's last function (342C:0669). Nothing in UW2.EXE calls it. It draws the four
   corner pictures 1068h-106Bh transparent, then two nested boxes and a filled rectangle,
   probably a framed text box. It belongs to this file: the EXE's relocations for it run
   in one descending sequence with this file's last record. */
/* name: FM Towns has no counterpart, so the name is provisional. */

void far seg043_3619_669(int x, int y, int r, int b)
{
    Transparency = 1;
    pic_to_screen(0x1068, x, y, 10, 0x28);
    pic_to_screen(0x106A, r - 0x28, y, 10, 0x28);
    pic_to_screen(0x1069, x, b + 10, 10, 0x28);
    pic_to_screen(0x106B, r - 0x28, b + 10, 10, 0x28);
    Transparency = 0;
    set_the_color(0x74);
    box(x + 0xD, y, r - 0xD, b);
    set_the_color(0x72);
    box(x + 0xE, y - 1, r - 0xE, b + 1);
    set_the_color(0x71);
    rectangle(x + 0xF, y - 2, r - 0xF, b + 2);
}
