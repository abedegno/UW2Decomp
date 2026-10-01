/* target: seg015_1D7C */
/* opts: -mm -1 -G -O -Y -d */
/* The mouse: the cursor, its save-under, the input queue, the mouse regions that pick the
   cursor shape, keyboard warping of the pointer, and the keyboard reader the input loop
   shares with it. DOS resident segment seg015_1D7C, in original order. Function names are
   the FM Towns originals; their order there matches this segment one for one, apart from
   the unnamed static flush_keys, which FM Towns does not have. The functions the target
   table still lists by IDA name are named from that one-for-one order, and each matches
   its FM Towns namesake statement for statement: mous_3d_set (IDA _F7), mous_3d_show
   (_1F6, empty in both), mouse_Qgetxy (_210), mouse_clearQ (_23A), do_keyarray_input
   (_432), mouse_check_reg (_6F9), MousReSave3d (_BB7) and warpMouse (_D3C). */

#include <stdlib.h>
#include <ctype.h>

/* Initialised data, DS:268..291, in definition order. FM Towns keeps the statics after
   _current_buttongroup (+4 onwards) and the four publics in its small-data group, in this
   same order. */
static int mouse_x = 100;               /* DS:268 */
static int mouse_y = 100;               /* DS:26A */
static int mouse_shown = 0;             /* DS:26C, mouse_show/mouse_hide nesting count */
static unsigned char mouse_saved = 0;   /* DS:26E, the background under the cursor is saved */
static int q_button = -1;               /* DS:26F, the queued button press, -1 for none */
static int q_x = 0;                     /* DS:271, where it was pressed */
static int q_y = 0;                     /* DS:273 */
static int q_release = 0;               /* DS:275 */
static unsigned char q_taken = 0;       /* DS:277, the last input came from the queue */
static int last_button = 0;             /* DS:278 */
static int in_x0 = -1;                  /* DS:27A, the region the pointer is in, -1 none */
int joymovecur = 0;                     /* DS:27C, FM Towns _joymovecur */
int fauxright = 0;                      /* DS:27E, FM Towns _fauxright */
static int num_regions = 0;             /* DS:280 */
static char force_depth = 0;            /* DS:282, force_mouse_cursor nesting */
static int warp_x = -1;                 /* DS:283, keyboard warp target, -1 for none */
char mouse_hand = 0;                    /* DS:285, FM Towns _mouse_hand: swap the buttons */
unsigned char calledfrom3d = 0;         /* DS:286, FM Towns _calledfrom3d */
static int key_index = 0;               /* DS:287, where the key array scan resumes */
static unsigned long warp_time = 0;     /* DS:289 */
static unsigned long key_time = 0;      /* DS:28D */
static char mouse_first = 1;            /* DS:291, get_input alternates mouse and keys */

/* Uninitialised data, DS:22E6..23E1. Turbo C lays _BSS out by a hash of the names, ties
   in definition order. Measured from probe compiles, the bucket is
   (c[0] + 256*c[1] + 8*c[len-2] + 64*len) & 1023 and buckets go out in ascending order.
   The five m3d publics are the FM Towns names and share one bucket (909). The statics are
   FM Towns _mcurhndl+0x10 to +0xDA, which has no names for them, so these names are ours,
   chosen to land where the EXE has them (bucket in brackets). */
static int m_curs;                      /* DS:22E6 (125), the cursor set_mouse_data chose */
static int rgn_ylo[20];                 /* DS:22E8 (146), the mouse regions */
static int hotspot_x;                   /* DS:2310 (160), the cursor's hot spot */
static int hotspot_y;                   /* DS:2312 (160) */
static int rgn_left[20];                /* DS:2314 (162), 10000 marks a free region */
static int con_x0;                      /* DS:233C (163), mouse_constrain's box */
static int con_y0;                      /* DS:233E (171) */
static int rgn_x1[20];                  /* DS:2340 (178) */
static int m_warp_dx;                   /* DS:2368 (205), keyboard warp steps */
static int m_warp_dy;                   /* DS:236A (205) */
static int rgn_ytop[20];                /* DS:236C (234) */
static int cur_h;                       /* DS:2394 (411), the cursor's size */
static int max_x;                       /* DS:2396 (421) */
static int max_y;                       /* DS:2398 (421) */
static int m_cursor_pic;                /* DS:239A (437), the cursor being drawn */
static int m_warp_rate;                 /* DS:239C (461) */
static int curs_w;                      /* DS:239E (475) */
static int warp_y;                      /* DS:23A0 (495) */
static int warp_key;                    /* DS:23A2 (671), the key driving the warp */
static int hit_y0;                      /* DS:23A4 (688), the bounds of the region in_x0 is */
static int reg_icon[20];                /* DS:23A6 (746), each region's cursor */
static int curs_stack[3];               /* DS:23CE (763), force_mouse_cursor's saved cursors */
static int in_x1;                       /* DS:23D4 (873) */
static int in_y1;                       /* DS:23D6 (881) */
int m3dx;                               /* DS:23D8, the 3D view, from mous_3d_set */
int m3dy;                               /* DS:23DA */
int m3dw;                               /* DS:23DC */
int m3dh;                               /* DS:23DE */
int m3dt;                               /* DS:23E0, cursor against it: 0 outside, 1 across, 2 inside */

extern unsigned char IsJoy;                     /* DS:8298 */
extern unsigned char didMouseInput;             /* DS:983 */
extern unsigned long far *Time;                 /* DS:2158 */
extern unsigned char far *Shift;                /* DS:2128 */
extern unsigned char far *CapsLock;             /* DS:212C */
extern unsigned char far *Alt;                  /* DS:2130 */
extern unsigned char far *Ctrl;                 /* DS:2134 */
extern unsigned char far *key_on;               /* DS:2138 */
extern unsigned char far *Asc;                  /* DS:2148, FM Towns _Asc */
extern int far *MouseDx;                        /* DS:214C, FM Towns _MouseDx */
extern int far *MouseDy;                        /* DS:2150, FM Towns _MouseDy */
extern int far *MouseOn;                        /* DS:2154, FM Towns _MouseOn */
/* The asm graphics module's pointer table. DS:21B8 points at seg048:434; its words 0x100
   and 0x101 are the save-under buffer FM Towns keeps at _Color_data_ptr+0x400 (4-byte
   entries there), so the name is provisional. The window edges are FM Towns wleft, wtop,
   wright and wbot, in the same order in memory (seg048:3DF4..3DFA); DS:21EC also points
   at wleft, so which of 21E8 and 21EC is "wleft" is not proven. */
extern int far *Color_data_ptr;                 /* DS:21B8 */
extern int far *wtop;                           /* DS:21DC */
extern int far *wbot;                           /* DS:21E0 */
extern int far *wright;                         /* DS:21E4 */
extern int far *wleft;                          /* DS:21E8 */
extern unsigned char far ShowClip;              /* 370D:0DC4, FM Towns _ShowClip */
extern unsigned char far Transparency;          /* 370D:0DC5 */

int far valloc(int w, int h);
void far set_the_color(int c);
void far set_the_window(int x0, int y0, int x1, int y1);
void far rectangle(int x0, int y0, int x1, int y1);
void far vcopyfb(int x, int y, int w, int h, int handle);   /* 0085:517B, FM Towns vcopyfb_ */
void far fbuf_setcolor(int c);                  /* 0085:5239, FM Towns fbuf_setcolor_ */
/* seg009 (1A6D): grs_which1 and pic_to_fbuf are FM Towns names, called the same way. FM
   Towns reads _grs_off[n] directly where DOS calls seg009_7, which maps the EMS page
   holding the cursor art first; it has no FM Towns name. */
int far grs_which1(int id);
unsigned char far * far seg009_7(int n);
void far pic_to_screen(int pic, int x, int y, int h, int w);
void far pic_to_fbuf(int pic, int x, int y);
/* seg011 (1AFA): the joystick code, which FM Towns lacks, so IDA names. */
void far seg011_6(int from3d);
void far seg011_2C6(int *dx, int *dy, int from3d);
void far do_changes(void);
/* The asm input module (2110): FM Towns key_, mouse_ and mbuttons. */
int far key(void);
void far mouse(void);
int far mbuttons(void);

char far mouse_check_reg(int x0, int y0, int x1, int y1);
void far set_mouse_data(int id);
void far checkMouse(void);
void far moveMouse(void);
void far MousReSave(void);
void far MousReSave3d(void);
void far drawMouse(void);
void far draw3dMouse(void);
void far keyboard_mouse(int key);
int far mouse_btns(void);
int far mouse_get_input_sp(void);

int far init_mouse(void)
{
    int i;

    con_x0 = con_y0 = 0;
    max_x = 0x13F;
    max_y = 0xC7;
    set_mouse_data(0x106C);
    if ((Color_data_ptr[0x100] = valloc(0x28, 0x28)) == 0)
        return -1;
    Color_data_ptr[0x101] = Color_data_ptr[0x100];
    for (i = 0; i < 20; i++)
        rgn_left[i] = 10000;
    return 0;
}

unsigned char far _actual_mhide(void)
{
    if (mouse_saved) {
        set_the_color(0x101);
        rectangle(mouse_x - hotspot_x, mouse_y + hotspot_y + 1,
                  mouse_x - hotspot_x + curs_w - 1, mouse_y + hotspot_y - cur_h + 1);
    }
    return mouse_saved;
}

void far mouse_show(void)
{
    if (++mouse_shown == 1)
        drawMouse();
}

void far mouse_hide(void)
{
    if (--mouse_shown == 0) {
        if (_actual_mhide()) {
            mouse_saved = 0;
            set_the_color(1);
        }
    }
}

void far mous_3d_set(int x, int y, int w, int h)
{
    m3dx = x;
    m3dy = y;
    m3dw = w;
    m3dh = h;
}

char far mous_in_3d_p(void)
{
    return mouse_check_reg(m3dx, m3dy - m3dh, m3dx + m3dw, m3dy);
}

void far mous_3d_hide(void)
{
    int x0, x1;
    int y1, y0;

    x0 = mouse_x - curs_w + hotspot_x;
    x1 = mouse_x + curs_w - hotspot_x;
    y1 = mouse_y + cur_h - hotspot_y;
    y0 = mouse_y - cur_h + hotspot_y;
    if (m3dx + m3dw < x0 || x1 < m3dx || m3dy - m3dh > y1 || y0 > m3dy)
        m3dt = 0;
    else {
        if (x0 > m3dx && m3dx + m3dw > x1 && y1 < m3dy && m3dy - m3dh < y0)
            m3dt = 2;
        else
            m3dt = 1;
        if (mouse_shown == 1)
            draw3dMouse();
        else if (mouse_shown > 1)
            mouse_shown--;
    }
}

void far mous_3d_show(void)
{
}

void far mouse_getxy(int *x, int *y)
{
    *x = mouse_x;
    *y = mouse_y;
}

void far mouse_Qgetxy(int *x, int *y)
{
    if (q_taken) {
        *x = q_x;
        *y = q_y;
    } else {
        *x = mouse_x;
        *y = mouse_y;
    }
}

void far mouse_clearQ(void)
{
    if (q_button != -1)
        q_button = -1;
}

/* Not in FM Towns and never called: empties the keyboard buffer. */
static void far flush_keys(void)
{
    while (key())
        ;
}

void far mouse_putxy(int x, int y)
{
    mouse_hide();
    checkMouse();
    if (*MouseOn)
        mouse();
    mouse_x = x;
    mouse_y = y;
    mouse_show();
}

int far mouse_getbut(int *b)
{
    if ((*b = mouse_btns()) == 0)
        q_button = -1;
    last_button = *b;
    return *b;
}

void far mouse_release(char how)
{
    int want;
    int b;

    q_release = -1;
    want = last_button == 1 ? 1 : 2;
    while ((mouse_get_input_sp() & (want | 0xFFFC)) == want && q_release == -1) {
        if (how)
            do_changes();
        keyboard_mouse(do_keyboard_input(1));
        moveMouse();
    }
    if (q_release == -1) {
        b = mouse_btns();
        if (b) {
            q_button = b;
            q_x = mouse_x;
            q_y = mouse_y;
        }
    }
}

unsigned char far mouse_dragged(char how)
{
    int x0, y0;
    int b;
    int x1, y1;
    unsigned char moved;

    moved = 0;
    mouse_getxy(&x0, &y0);
    while (mouse_getbut(&b) && !moved) {
        if (how)
            do_changes();
        keyboard_mouse(do_keyboard_input(1));
        moveMouse();
        mouse_getxy(&x1, &y1);
        if (abs(x1 - x0) + abs(y1 - y0) > 6)
            moved = 1;
    }
    return moved;
}

void far mouse_constrain(int x0, int y0, int x1, int y1)
{
    con_x0 = x0;
    con_y0 = y0;
    max_x = x1;
    max_y = y1;
}

void far mouse_freereign(void)
{
    con_x0 = con_y0 = 0;
    max_x = 0x13F;
    max_y = 0xC7;
}

int far do_mouse_input(void)
{
    int b = 0;

    moveMouse();
    if (q_button == -1) {
        q_taken = 0;
        b = mouse_btns();
    } else {
        if ((b = mouse_btns()) == 0) {
            b = q_button;
            q_taken = 1;
        }
        q_button = -1;
    }
    last_button = b;
    if (b == 0)
        return -1;
    return b;
}

int far do_keyarray_input(void)
{
    int c;
    int start;

    if (*Time - key_time < 30)
        return 0;
    c = 0;
    start = key_index = (key_index + 1) & 0x7F;
    do {
        if (key_on[key_index]) {
            if (*Shift)
                key_index |= 0x80;
            c = Asc[key_index];
            if (c)
                break;
        }
        key_index = (key_index + 1) & 0x7F;
    } while (key_index != start);
    return c;
}

int far do_keyboard_input(char array)
{
    int c;

    c = key();
    if (array)
        c = do_keyarray_input();
    c &= 0xFF;
    if (c == 0)
        return -1;
    key_time = *Time;
    if (c & 0x80) {
        if (*Shift)
            c |= 0x400;
    } else if (*CapsLock && isalpha(c))
        c = !*Shift ? c - 0x20 : c + 0x20;
    if (*Alt)
        c |= 0x200;
    if (*Ctrl)
        c |= 0x100;
    return c;
}

int far get_input(char array)
{
    int c;

    didMouseInput = 1;
    if (mouse_first) {
        mouse_first = 0;
        if ((c = do_mouse_input()) < 0)
            return do_keyboard_input(array);
        return c;               /* the dead jump the EXE has before the else */
    } else {
        mouse_first = 1;
        if ((c = do_keyboard_input(array)) < 0)
            return do_mouse_input();
    }
    return c;
}

int far mouse_get_input(void)
{
    return get_input(0);
}

int far mouse_get_input_sp(void)
{
    return get_input(1);
}

int far defineMouseRegion(int x0, int y0, int x1, int y1, int id)
{
    int i;

    for (i = 0; i < 20; i++)
        if (rgn_left[i] == 10000)
            break;
    if (i == 20)
        return -1;
    rgn_left[i] = x0;
    rgn_x1[i] = x1;
    rgn_ytop[i] = y1;
    rgn_ylo[i] = y0;
    reg_icon[i] = id;
    if (i >= num_regions)
        num_regions = i + 1;
    checkMouse();
    return i;
}

void far undefineMouseRegion(int handle)
{
    int i;

    if (handle < num_regions) {
        rgn_left[handle] = 10000;
        if (reg_icon[handle] == m_cursor_pic)
            in_x0 = 10000;
        if (num_regions - 1 == handle) {
            for (i = num_regions - 2; i >= 0; i--)
                if (rgn_left[i] != 10000)
                    break;
            num_regions = i + 1;
        }
        checkMouse();
    }
}

void far force_mouse_cursor(int id)
{
    if (force_depth != 3) {
        mouse_hide();
        curs_stack[force_depth] = m_curs;
        force_depth++;
        set_mouse_data(id);
        mouse_show();
    }
}

void far unforce_mouse_cursor(int how)
{
    if (how & 1)
        mouse_hide();
    if (--force_depth < 0) {
        curs_stack[0] = 0x106C;
        force_depth = 0;
    }
    set_mouse_data(curs_stack[force_depth]);
    checkMouse();
    if (how & 2)
        mouse_show();
}

char far mouse_check_reg(int x0, int y0, int x1, int y1)
{
    int dy, dx;

    dy = (cur_h + 1) >> 1;
    dx = (curs_w + 1) >> 1;
    return y0 - dy <= mouse_y && y1 + dy >= mouse_y &&
           x0 - dx <= mouse_x && x1 + dx >= mouse_x;
}

void far set_mouse_data(int id)
{
    unsigned char far *p;

    _actual_mhide();
    p = seg009_7(grs_which1(id));
    curs_w = p[1];
    cur_h = p[2];
    hotspot_x = curs_w / 2 - 1;
    hotspot_y = cur_h / 2;
    m_cursor_pic = id;
    m_curs = id;
    if (mouse_saved)
        MousReSave();
}

void far checkMouse(void)
{
    int i;

    if (force_depth > 0)
        return;
    if (in_x0 != -1 && mouse_x >= in_x0 && mouse_x <= in_x1 &&
        mouse_y >= hit_y0 && mouse_y <= in_y1)
        return;
    for (i = 0; i < num_regions; i++) {
        if (rgn_left[i] <= mouse_x && rgn_x1[i] >= mouse_x &&
            rgn_ylo[i] <= mouse_y && rgn_ytop[i] >= mouse_y) {
            in_x0 = rgn_left[i];
            in_x1 = rgn_x1[i];
            hit_y0 = rgn_ylo[i];
            in_y1 = rgn_ytop[i];
            set_mouse_data(reg_icon[i]);
            break;
        }
    }
    if (in_x0 != -1 && i == num_regions) {
        in_x0 = -1;
        set_mouse_data(0x106C);
    }
}

void far moveMouse(void)
{
    int wl, wt, wr, wb;
    int dx, dy;
    int axes, n;

    dx = 0;
    dy = 0;
    if (IsJoy)
        seg011_6(calledfrom3d);
    if (*MouseOn) {
        if (IsJoy && joymovecur)
            seg011_2C6(&dx, &dy, calledfrom3d);
        else {
            mouse();
            dx = *MouseDx;
            dy = *MouseDy;
        }
    }
    if (dx == 0 && dy == 0) {
        if (warp_x < 0)
            return;
        if (warp_key) {
            if (key_on[warp_key] == 0) {
                m_warp_rate >>= 1;
                if (m_warp_rate == 0) {
                    warp_x = -1;
                    warp_key = 0;
                    return;
                }
            } else if (*Time - warp_time < 10)
                return;
        } else {
            if (*Time - warp_time < 10)
                return;
            m_warp_rate += 8;
            if (m_warp_rate > 40)
                m_warp_rate = 40;
        }
        warp_time = *Time;
        axes = 3;
        n = m_warp_rate;
        while (n-- && axes) {
            if (mouse_x + dx > warp_x - 5 && mouse_x + dx < warp_x + 5 && (axes & 1)) {
                axes ^= 1;
                dx = warp_x - mouse_x;
            } else if (axes & 1)
                dx += m_warp_dx;
            if (mouse_y - dy > warp_y - 5 && mouse_y - dy < warp_y + 5 && (axes & 2)) {
                axes ^= 2;
                dy = mouse_y - warp_y;
            } else if (axes & 2)
                dy += m_warp_dy;
        }
    } else
        warp_x = -1;
    if (calledfrom3d) {
        wl = *wleft;
        wt = *wtop;
        wr = *wright;
        wb = *wbot;
        set_the_window(0, 0xC7, 0x13F, 0);
    }
    if (_actual_mhide())
        mouse_saved = 0;
    mouse_x += dx;
    mouse_y -= dy;
    if (mouse_x < con_x0)
        mouse_x = con_x0;
    else if (mouse_x > max_x)
        mouse_x = max_x;
    if (mouse_y < con_y0)
        mouse_y = con_y0;
    else if (mouse_y > max_y)
        mouse_y = max_y;
    if (warp_x == mouse_x && warp_y == mouse_y) {
        warp_key = 0;
        warp_x = -1;
    }
    checkMouse();
    if (mouse_shown > 0)
        drawMouse();
    if (calledfrom3d)
        set_the_window(wl, wt, wr, wb);
}

void far MousQUp(char from3d)
{
    int b;

    calledfrom3d = from3d;
    moveMouse();
    calledfrom3d = 0;
    b = mouse_btns();
    if (b) {
        if (q_button == -1) {
            q_button = b;
            q_x = mouse_x;
            q_y = mouse_y;
        }
    } else
        q_release = 0;
}

void far MousReSave(void)
{
    set_the_color(0x100);
    rectangle(mouse_x - hotspot_x, mouse_y + hotspot_y + 1,
              mouse_x - hotspot_x + curs_w - 1, mouse_y + hotspot_y - cur_h + 1);
    mouse_saved = 1;
}

void far MousReSave3d(void)
{
    if (m3dt == 2) {
        fbuf_setcolor(0x100);
        rectangle(mouse_x - hotspot_x - m3dx, mouse_y + hotspot_y - m3dy + m3dh,
                  mouse_x + curs_w - hotspot_x - m3dx - 1, mouse_y - cur_h + hotspot_y - m3dy + m3dh);
    } else if (m3dt == 1) {
        ShowClip = 1;
        vcopyfb(mouse_x - hotspot_x - m3dx, mouse_y + hotspot_y - m3dy + m3dh, curs_w, cur_h + 1,
                Color_data_ptr[0x100]);
        ShowClip = 0;
    } else
        return;
    mouse_saved = 1;
}

void far drawMouse(void)
{
    MousReSave();
    ShowClip = Transparency = 1;
    pic_to_screen(m_cursor_pic, mouse_x - hotspot_x, mouse_y + hotspot_y, cur_h, curs_w);
    ShowClip = Transparency = 0;
    set_the_color(0);
}

void far draw3dMouse(void)
{
    int x, y;

    MousReSave3d();
    x = mouse_x - hotspot_x - m3dx;
    y = mouse_y + hotspot_y - m3dy + m3dh - 1;
    ShowClip = Transparency = 1;
    pic_to_fbuf(m_cursor_pic, x, y);
    ShowClip = Transparency = 0;
}

void far warpMouse(int x, int y)
{
    m_warp_rate = 1;
    if (x < con_x0)
        x = con_x0;
    else if (x > max_x)
        x = max_x;
    warp_x = x;
    if (y < con_y0)
        y = con_y0;
    else if (y > max_y)
        y = max_y;
    warp_y = y;
    m_warp_dx = x > mouse_x ? 1 : -1;
    m_warp_dy = y > mouse_y ? -1 : 1;
}

void far keyboard_mouse(int key)
{
    int k = 0;
    int x = mouse_x;
    int y = mouse_y;

    switch (key) {
    case 0x4A3:
        if (warp_x >= 0)
            return;
        if (mouse_x < 0xE2) {
            x = 0x131;
            y = 7;
        } else if (mouse_y > 0x49) {
            x = 0x64;
            y = 0x82;
        } else {
            x = 0x10E;
            y = 0x78;
        }
        break;
    case 9:
        if (warp_x >= 0)
            return;
        if (mouse_x < 0xE2) {
            x = 0x10E;
            y = 0x78;
        } else if (mouse_y > 0x49) {
            x = 0x131;
            y = 7;
        } else {
            x = 0x64;
            y = 0x82;
        }
        break;
    case 0x8F:
        x = 0;
        k = 0x4B;
        break;
    case 0x91:
        x = 0x13F;
        k = 0x4D;
        break;
    case 0x8D:
        y = 0xC7;
        k = 0x48;
        break;
    case 0x93:
        y = 0;
        k = 0x50;
        break;
    case 0x8C:
        x = 0;
        y = 0xC7;
        k = 0x47;
        break;
    case 0x8E:
        x = 0x13F;
        y = 0xC7;
        k = 0x49;
        break;
    case 0x92:
        x = 0;
        y = 0;
        k = 0x4F;
        break;
    case 0x94:
        x = 0x13F;
        y = 0;
        k = 0x51;
        break;
    default:
        return;
    }
    if (k == warp_key && warp_x >= 0) {
        m_warp_rate++;
        if (m_warp_rate > 40)
            m_warp_rate = 40;
    } else {
        warp_key = k;
        if (x != mouse_x || y != mouse_y)
            warpMouse(x, y);
    }
}

int far mouse_btns(void)
{
    int b = 0;

    if (!*MouseOn || (b = mbuttons()) == 0) {
        if (key_on[0x52])
            b |= 1;
        if (key_on[0x53] || IsJoy && fauxright)
            b |= 2;
    }
    if (mouse_hand && b > 0 && b < 3)
        b ^= 3;
    return b;
}
