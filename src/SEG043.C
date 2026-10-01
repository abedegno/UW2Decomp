/* target: seg043_3619 */
/* opts: -mm -1 -G -O -d */
/* The message scroll: DOS resident segment seg043_3619, in original order. Function names
   are the originals from the FM Towns symbol table where the map confirms them.
   set_mouse_in and do_main_scroll are identified here (not in the map) by exact
   structural correspondence with FM Towns set_mouse_in_ and do_main_scroll_: see the
   final report. draw_scroll has no FM Towns counterpart; it is probably a small
   box-border helper that Watcom inlined into draw_scroll_ (the next FM Towns function,
   which starts exactly where our segment ends). */

#include <dos.h>
#include <string.h>

/* The scroll's state. Field names beyond the ones the code clearly uses (coordinates,
   cursor position, the font colour) are our own; FM Towns has no per-field names, only
   the struct's three instances (_main_scroll, _npc_scroll, _menu_scroll). Byte-packed,
   21 (0x15) bytes, matching the spacing between those three instances in the EXE. */
struct Scroll {
    int x0;                      /* 0x00 */
    int y0;                      /* 0x02 */
    int top;                     /* 0x04 */
    int bottom;                  /* 0x06 */
    int cur_x;                   /* 0x08 */
    int cur_y;                   /* 0x0A */
    int left;                    /* 0x0C */
    int last_y;                  /* 0x0E */
    unsigned char more_pending;  /* 0x10 */
    int start_line;              /* 0x11 */
    int font_color;              /* 0x13 */
};

/* DS:21CC, _cur_font in symbols.tsv (provisional). Only field +6 (line height) is used
   here. */
struct Font {
    char pad0[6];
    int height;                  /* 0x06 */
};

extern struct Scroll near *scroll;              /* DS:34B0, FM Towns _scroll */
extern struct Scroll main_scroll;                /* DS:938, FM Towns _main_scroll */
extern struct Scroll npc_scroll;                 /* DS:94D, FM Towns _npc_scroll */
extern struct Scroll menu_scroll;                /* DS:962, FM Towns _menu_scroll */
extern unsigned char mouse_in_scroll;             /* DS:34B2, FM Towns _mouse_in_scroll */
extern int scroll_mode;                           /* DS:97E, FM Towns _scroll_mode */
extern int start_line;                            /* DS:980, FM Towns _start_line */
extern unsigned char menus_active;                /* DS:982, FM Towns _menus_active */
extern unsigned char didMouseInput;               /* DS:983, FM Towns _didMouseInput */
extern long click_time;                           /* DS:984, part of FM Towns _spec_col */
extern int edge_phase;                            /* DS:988, part of FM Towns _spec_col */
extern int conv_edge_phase;                       /* DS:98A, part of FM Towns _spec_col */
extern char scroll_esc;                  /* DS:98C */
/* DS:98D, the byte right after scroll_esc (alignment padding before the next
   flag, which is 0). scroll_wrap passes its address as an empty string purely to trigger
   scroll_print3's screen-full/MORE check without printing anything; with -d this merges
   with any other file's "" literal, which is how the EXE's own copy lands here. */
extern char empty_text[];
extern int scrmode;                               /* DS:5D60, FM Towns _scrmode (IDA's
                                                       label "InGameMode" here is wrong: a
                                                       different global, DS:2506, exists
                                                       under that name already). */
extern unsigned char far *foreground_color;       /* DS:21C4, reused from SEG038.C */
extern unsigned char far *background_color;       /* DS:21C8, reused from SEG038.C */
extern struct Font far *cur_font;                 /* DS:21CC */
extern long far *Time;                  /* DS:2158 */
extern unsigned char color_by_digit[];            /* DS:947, indexed by the raw '0'..'6'
                                                       byte; no FM Towns name found. */

char far mouse_check_reg(int top, int y0, int bottom, int x0);
void far mouse_hide(void);
void far mouse_show(void);
void far scroll_clear(int which);
void far pic_to_screen(int pic, int x, int y, int w, int h);
void far set_the_color(int color);
void far rectangle(int top, int mid, int bottom, int count);
unsigned far str_len(char far *s);
char far * far FindInString_seg005_105F_103F(char *s, char c);
char far * far MaybeAComparison_seg005_105F_282C(char *s, char c);
int far string_width(char far *s);
void far string_to_screen(char far *s, int x, int y);
void far scroll_wait(int ticks, int also);
void far j_WriteTextWithMORE_ovr139_BD(void);
/* A 6-int call in a different file; FM Towns vcopy_ matches the first four parameters
   exactly (by position and value) but takes only four, so this probably is not it.
   Unresolved; kept under its DOS label. */
void far vcopy(int p1, int p2, int p3, int p4, int p5, int p6);

void far scroll_up(int n);
void far scroll_print1(char *s, int flag);
void far scroll_print2(char *s, int flag);
void far scroll_print3(char *s, int flag);
void far scroll_wrap(char *s, int flag);

/* Not anchored in the map; the target table keeps the IDA name. This is FM Towns
   set_mouse_in_ by exact correspondence (same four Scroll fields, same order, into the
   same mouse_check_reg_ call, storing the result to the same mouse_in_scroll byte). */
void far set_mouse_in(void)
{
    mouse_in_scroll = mouse_check_reg(scroll->top, scroll->y0, scroll->bottom, scroll->x0);
}

/* Not anchored either. This is FM Towns do_main_scroll_: it sits immediately before
   do_npc_scroll_/do_play_scroll_ there, and sets the same three fields (mode 0,
   didMouseInput 1, scroll pointer) that do_npc_scroll_/do_play_scroll_ set with 1/2. */
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

void far draw_edges(void)
{
    pic_to_screen(edge_phase + 0x20AB, scroll->top - 4, 0x1E, 0x1C, 4);
    pic_to_screen(edge_phase + 0x20B0, scroll->bottom + 1, 0x1E, 0x1C, 4);
    edge_phase++;
    if (edge_phase == 5)
        edge_phase = 0;
}

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
    /* 0x31 (49) bytes are copied into this buffer below; it is declared two bytes short
       of that because `found`, as a register variable, still reserves a two-byte spill
       slot right after it, and the true end of the 49-byte region is that slot, used
       below as `sentinel`. Matched against the EXE's frame size (sub sp,36h) and the
       offsets of `sentinel` and `saved`, not guessed. */
    char copy[47];
    register char *found;
    register int remaining;
    char saved;
    int chunklen;
    char pad1;                 /* unused; the EXE reserves it too (frame is 0x36 bytes) */
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
        found = (char *)FindInString_seg005_105F_103F(copy, ' ');
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

void far scroll_print1(char *text, int flag)
{
    char *di;
    char *found;

    di = text;
    while ((found = (char *)MaybeAComparison_seg005_105F_282C(di + 1, '\\')) != 0) {
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

void far scroll_print2(char *text, int flag)
{
    char saved, unused;
    char *si;
    char *di;

    di = text;
    while ((si = (char *)MaybeAComparison_seg005_105F_282C(di, '\n')) != 0) {
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
            j_WriteTextWithMORE_ovr139_BD();
            break;
        default:
            if (*si >= '0' && *si <= '6')
                *foreground_color = color_by_digit[*si];
            break;
        }
        scroll->font_color = *foreground_color;
        si++;
    }

    if (scroll->more_pending) {
        y = scroll->cur_y - cur_font->height;
        if (start_line - flag < 0) {
            if (y - cur_font->height < scroll->y0 - 1) {
                j_WriteTextWithMORE_ovr139_BD();
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
            scroll_print3(empty_text, flag);
            while (*si == ' ')
                si++;
        }
        if (scroll->cur_x + string_width(si) >= scroll->bottom) {
            di = (char *)FindInString_seg005_105F_103F(si, ' ');
            if (di != 0) {
                while (di[-1] == ' ' && di > si)
                    di--;
            }
            if (di == 0 || di == si) {
                scroll->more_pending = 1;
                scroll_print3(empty_text, flag);
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

void far draw_scroll(int x, int y, int w, int h, char flag)
{
    if (flag) {
        set_the_color(1);
        rectangle(x + 0xE, y - 1, w - 0xF, h + 1);
    }
    set_the_color(0x71);
    rectangle(x, y, w, h);
}
