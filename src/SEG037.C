/* target: seg037_32C0 */
/* opts: -mm -1 -G -O -Y -d */
/* The screen furniture around the 3D view: the vitality and mana flasks, the compass, the
   power gem, the eyes, the first-person weapon and its frames from DATA\weap.dat, the rune
   shelf and active spell icons, the sliding stat/inventory/rune panel, and the frame buffer
   send. The whole of DOS resident segment seg037_32C0, in original order. Function and
   global names are the originals from the FM Towns symbol table, except where a comment
   says otherwise; the source file's own name is not known. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0x38];
    unsigned char max_mana;             /* 0x38 */
    char pad39[0x47 - 0x39];
    unsigned char shelf[3];             /* 0x47 */
    char pad4A[0x60 - 0x4A];
    unsigned b60:1;                     /* 0x60 */
    unsigned poison:4;
    unsigned active_spells:4;
    unsigned b60_9:7;
    unsigned b62:16;                    /* 0x62 */
    unsigned char b64;                  /* 0x64 */
    unsigned lefty:1;                   /* 0x65 */
    unsigned female:1;
    unsigned body:3;
    unsigned pclass:3;
};

/* The player's critter data, reached through the near pointer `playerdat`. */
struct Critter {
    char pad0[4];
    unsigned char max_vit;              /* 0x04 */
};

struct Inplist {
    char pad0[8];
    int field8;
};

struct Motion {
    char pad0[0x14];
    int momentum;                       /* 0x14 */
};

extern struct Player near *player;
extern struct Critter near *playerdat;
extern struct Inplist near *inplist;
extern struct Motion PN;
extern unsigned long far *Time;
extern unsigned char RightPanel;
extern unsigned char inv_refresh;
extern unsigned char WizEye;
extern char demo_mode;
extern int far *wtop;                   /* DS:21DC */
extern int far *wbot;                   /* DS:21E0 */
extern int far *wright;                 /* DS:21E4 */
extern int far *wleft;                  /* DS:21E8 */
extern unsigned char far Transparency;
extern unsigned char far ShowClip;
extern unsigned char far stdat[];
extern unsigned char Palettes[][16];
/* No FM Towns counterpart (FM Towns has no EMS): the segment of the EMS page frame, set
   by seg013 from INT 67h function 41h. Provisional name. */
extern unsigned ems_frame;
/* FM Towns names, at 4FAF:E4D0 and E4D1: the EMS page holding the screen graphics, and the
   page last mapped for objects, which is spoiled by any other mapping. */
extern unsigned char far scrgr_fpage;
extern unsigned char far obj_inpage1;
extern unsigned far ems_seg;

/* This file's _BSS, DS:33E8-349A. Turbo C lays it out by name: the static that FM Towns
   keeps just after `setting` (the low byte of *Time at the last redraw) has no original
   name, and old_time was chosen because it lands first, at DS:33E8, as in the EXE. */
static unsigned char old_time;
unsigned char weap_x[0x1F];
unsigned char weap_y[0x1F];
unsigned char setting[9];
int weap_offs[0x20];
unsigned char frmtot[3];
unsigned char wframe[0x1F];
unsigned char goal[9];

/* The sprite library, seg000. create_sprite is called with one argument and with three,
   so this file saw no prototype for it. */
int far create_sprite();
void far change_sprite(int spr, int x, int y, int w, int h);
void far draw_sprite(int spr, int frame);
void far draw_mask(int spr, int frame);
void far erase_sprite(int spr);
void far move_sprite(int spr, int x, int y);
void far set_yoff(int spr, int yoff);
void far update_sprites(void);

void far pic_to_screen(int pic, int x, int y, int w, int h);
void far pic_to_fbuf(int pic, int x, int y);
unsigned char far * far grs_unpack(unsigned char far *p);
void far grSoftPageFlip(void);
void far grPageFlip(void);
void far copy_visible_to_hidden(void);
void far show(int x, int y, unsigned char far *buf, int h, int w, int a, int b);
void far fbshow(unsigned char far *p, int x, int y, int w, int h);
void far set_the_window(int l, int t, int r, int b);
void far set_the_color(int c);
void far rectangle(int x1, int y1, int x2, int y2);
void far seg003_0272_51C8(int a, int b, int c, int d, int e, int f);
void far grab(unsigned char far *buf, int x, int y, int w, int h);
void far cFBtoScreen(void);
unsigned char far read_gr_far(char *name, int n, unsigned char far *buf);
char far gronk_gr(char *name, int start, int count, unsigned char far *(far *adr)(int size),
                  char (far *mv)(unsigned char far *p, int size, int n));
void far pfatal_code(int code);
void far mouse_hide(void);
void far mouse_show(void);
void far editchng(int what);
void far MapMemory_seg013_1D3C_C7(int phys, int log);
void far RedispInv(void);
void far RedispRune(void);
void far RedispStat(void);

void far adjust_flasks(int which);
void far adjust_compass(void);
void far adjust_power(void);
void far adjust_panel(void);
void far adjust_eyes(void);
void far adjust_weapon(void);
void far set_runes(unsigned char *runes);
void far init_panelflip(int panel);
void far free_panelflip(void);
char far do_panel_frame(void);
int far get_wfr(int f);
void far do_fbuf_bms(void);

/* Which screen elements to redraw: now, on every 32nd tick, and on every 64th tick. One
   bit per element, by its index in `adjust`. Static (no FM Towns names); ours. */
static int slow_adjust = 0;             /* DS:79F */
static int fast_adjust = 0;             /* DS:7A1 */
static int now_adjust = 0;              /* DS:7A3 */
char weapid = -1;
char last_weap = -2;
char wlstate = 6;
int weap_frame = -1;
int wxo = 0;
int wfo = 1;
int FL_X[2] = { 0xF8, 0x120 };
int liquid_y[14] = { 0x25, 0x29, 0x2B, 0x2D, 0x2F, 0x31, 0x33, 0x34, 0x35, 0x36, 0x38, 0x3A, 0x3C, 0x3E };
int liquid_h[14] = { 0x0, 0x4, 0x5, 0x6, 0x7, 0x7, 0x7, 0x7, 0x6, 0x5, 0x4, 0x4, 0x4, 0x4 };
unsigned char bubbling[2] = { 0, 0 };
unsigned char swishing[2] = { 0, 0 };
int nedl_x[16] = { -0x5, -0xC, -0x12, -0x18, -0x1A, -0x1B, -0x14, -0xD, -0x6, 0x2, 0xC, 0x10, 0x13, 0x10, 0xB, 0x3 };
int nedl_y[16] = { 0x8, 0x8, 0x7, 0x5, 0x3, 0x2, -0x1, -0x2, -0x3, -0x2, -0x1, 0x2, 0x3, 0x5, 0x7, 0x8 };
/* FM Towns keeps the panel buffer's pointer in pbuf too; here it is in EMS. */
struct {
    unsigned char frame;
    unsigned char flag;
} pbuf = { 0, 0 };
struct {
    unsigned char far *buf;
    unsigned char flag;
    char spare;
} wbuf = { 0, 0, 0 };
int eyes_seq[5] = { 0x2080, 0x2081, 0x2082, 0x2081, 0x2080 };
int eyes = 0;
int eyes_frame = 0;
int rune_x[3] = { 0xB3, 0xC1, 0xCF };
int spell_x[3] = { 0x2F, 0x21, 0x13 };
void (far *panel_dispatch[3])(void) = { RedispInv, RedispRune, RedispStat };
int level[4] = { 0, 0, 0, 0 };
void (far *adjust[9])() = { adjust_flasks, adjust_flasks, adjust_compass, adjust_power, 0, 0,
                            adjust_panel, adjust_eyes, adjust_weapon };

void far set_flask(int which)
{
    int frame;
    int i;

    if (which == 0)
        frame = player->poison ? 0x203E : 0x200C;
    else if (which == 1)
        frame = 0x2025;
    else
        return;
    pic_to_screen(0x2057, FL_X[which], 0x43, 1, 1);
    for (i = 0; i < goal[which]; i++) {
        move_sprite(level[which], FL_X[which], liquid_y[i + 1]);
        draw_sprite(level[which], frame + i);
        update_sprites();
    }
    setting[which] = goal[which];
}

void far set_compass(void)
{
    int dir;

    dir = goal[2];
    draw_sprite(level[2], (dir & 3) + 0x2059);
    move_sprite(level[3], nedl_x[dir] + 0x78, nedl_y[dir] + 0x2B);
    draw_sprite(level[3], dir + 0x205D);
    update_sprites();
}

void far reset_scrgr(void)
{
    int i;

    for (i = 0; i < 9; i++)
        setting[i] = goal[i] = 0;
    RightPanel = 0;
    eyes_frame = 0;
    slow_adjust &= ~0x80;
    goal[7] = 4;
    setting[8] = goal[8] = 6;
    wlstate = 6;
    weap_frame = 0;
    update_sprites();
}

void far init_scrgr(void)
{
    static unsigned char inited = 0;
    int i;

    if (!inited) {
        for (i = 0; i < 2; i++) {
            level[i] = create_sprite(0);
            change_sprite(level[i], 0, 0, 0x18, 4);
        }
        level[2] = create_sprite(0);
        change_sprite(level[2], 0x5E, 0x33, 0x34, 0x10);
        level[3] = create_sprite(0);
        change_sprite(level[3], nedl_x[0] + 0x78, nedl_y[0] + 0x2B, 6, 0xC);
        eyes = create_sprite(0);
        change_sprite(eyes, 0x6E, 0xC3, 1, 1);
        goal[8] = setting[8] = 6;
        inited = 1;
    }
    grSoftPageFlip();
    copy_visible_to_hidden();
    for (i = 0; i < 2; i++)
        set_flask(i);
    set_compass();
    draw_sprite(eyes, 0x207F);
    set_runes(player->shelf);
    if (!read_gr_far("panels", RightPanel, stdat))
        pfatal_code(0x3004);
    show(0xEB, 0xC0, stdat, 0x70, 0x4F, 0, 0);
    panel_dispatch[RightPanel]();
    update_sprites();
    grPageFlip();
    grSoftPageFlip();
}

void far hold_scrgr(void)
{
    RightPanel = goal[6] = setting[6];
    pbuf.frame = 0;
    pbuf.flag = 0;
}

void far free_scrgr(void)
{
    free_panelflip();
}

void far set_screen_frame(char which, int val)
{
    register int max;

    switch (which) {
    case 0:
    case 1:
        max = which == 1 ? player->max_mana : playerdat->max_vit;
        if (max)
            goal[which] = val * 12 / max;
        else
            goal[which] = 0;
        if (goal[which] >= 12)
            goal[which] = 12;
        slow_adjust |= 1 << which;
        break;
    case 2:
        if (setting[2] != val) {
            slow_adjust |= 1 << which;
            goal[2] = val;
        }
        break;
    case 3:
        if (val != 9)
            now_adjust |= 1 << which;
        else
            slow_adjust |= 1 << which;
        goal[which] = val;
        break;
    case 8:
        if (weapid != last_weap && goal[8] == 6 && val != 4) {
            if (val == 6)
                wbuf.flag = 1;
        } else {
            fast_adjust |= 1 << which;
            goal[8] = val;
            if (val == 4)
                wbuf.flag = 0;
        }
        break;
    case 6:
        if (RightPanel != 4) {
            fast_adjust |= 1 << which;
            goal[which] = val;
        }
        break;
    default:
        slow_adjust |= 1 << which;
        goal[which] = val;
        break;
    }
}

void far update_screen(void)
{
    unsigned char now;
    int r;
    char did;
    int i;
    int bit;

    now = *Time & 0xFF;
    did = 0;
    if (now_adjust) {
        for (i = 0, bit = 1; i < 9; i++, bit <<= 1) {
            if (now_adjust & bit) {
                if (adjust[i])
                    adjust[i](i);
                now_adjust &= ~bit;
                did = 1;
            }
        }
    }
    if ((now >> 5) != (old_time >> 5)) {
        for (i = 0, bit = 1; i < 9; i++, bit <<= 1)
            if ((fast_adjust & bit) && adjust[i])
                adjust[i](i);
        did = 1;
    }
    if ((now >> 6) != (old_time >> 6)) {
        if ((r = rand()) < 0x666 && bubbling[r & 1] == 0) {
            bubbling[r & 1] = 1;
            slow_adjust |= 1 << (r & 1);
        }
        if ((r = rand()) < 0x666 && swishing[r & 1] == 0) {
            swishing[r & 1] = 1;
            slow_adjust |= 1 << (r + 4 & 1);
        }
        for (i = 0, bit = 1; i < 9; i++, bit <<= 1)
            if ((slow_adjust & bit) && adjust[i])
                adjust[i](i);
        did = 1;
    }
    if (did) {
        update_sprites();
        old_time = now;
    }
}

void far adjust_flasks(int which)
{
    static int bub_frame[2] = { 0x2019, 0x2032 };
    static int bub_spr[2] = { 0, 0 };
    static int drain_spr[2] = { 0, 0 };
    static int mask_spr[2] = { 0, 0 };
    /* The flask is used through a register copy, but the bit for `slow_adjust` is shifted
       by the parameter itself (FM Towns does the same); without the copy the compiler
       puts `base` in DI and leaves the parameter on the stack. */
    register int f = which;
    register int n;
    int diff;
    int base;
    int bub_first;
    int bub_last;
    int y;

    switch (f) {
    case 0:
        if (player->poison) {
            base = 0x203E;
            bub_first = 0x204B;
            bub_last = 0x2056;
            if (bub_frame[0] >= 0x2019 && bub_frame[0] <= 0x2024) {
                set_flask(0);
                bub_frame[0] += 0x32;
            }
        } else {
            base = 0x200C;
            bub_first = 0x2019;
            bub_last = 0x2024;
            if (bub_frame[0] >= 0x204B && bub_frame[0] <= 0x2056) {
                set_flask(0);
                bub_frame[0] -= 0x32;
            }
        }
        break;
    case 1:
        base = 0x2025;
        bub_first = 0x2032;
        bub_last = 0x203D;
        break;
    default:
        return;
    }
    if (drain_spr[f] == 0) {
        drain_spr[f] = create_sprite(0);
        change_sprite(drain_spr[f], FL_X[f], 0x43, 0x18, 0x21);
        bub_spr[f] = create_sprite(0);
        change_sprite(bub_spr[f], 0, 0, 0x18, 4);
        mask_spr[f] = create_sprite(0);
        change_sprite(mask_spr[f], FL_X[f], 0x43, 0x18, 4);
    }
    diff = goal[f] - setting[f];
    if (diff > 0) {
        n = ++setting[f];
        move_sprite(level[f], FL_X[f], liquid_y[n]);
        draw_sprite(level[f], base + n - 1);
    } else if (diff < 0) {
        n = --setting[f];
        y = liquid_y[n + 1];
        change_sprite(drain_spr[f], FL_X[f], y, 0x18, liquid_h[n + 1]);
        set_yoff(drain_spr[f], 0x43 - y);
        draw_sprite(drain_spr[f], 0x2057);
        if (n > 0) {
            move_sprite(level[f], FL_X[f], liquid_y[n]);
            draw_sprite(level[f], base + n - 1);
        }
    } else {
        if (bubbling[f] == 0)
            slow_adjust &= ~(1 << which);
        n = setting[f];
    }
    if (bubbling[f] == 1 && n < 8 && n > 0) {
        if (bub_frame[f] == bub_last) {
            bub_frame[f] = bub_first;
            draw_sprite(level[f], base + n - 1);
            bubbling[f] = 0;
        } else {
            move_sprite(bub_spr[f], FL_X[f], liquid_y[n]);
            draw_sprite(bub_spr[f], ++bub_frame[f]);
            set_yoff(mask_spr[f], 0x43 - liquid_y[n]);
            move_sprite(mask_spr[f], FL_X[f], liquid_y[n]);
            draw_mask(mask_spr[f], 0x2058);
        }
    }
}

void far adjust_compass(void)
{
    int cur;
    int diff;

    cur = setting[2];
    diff = goal[2] - cur;
    if (diff == 0) {
        slow_adjust &= ~4;
        return;
    }
    if (diff < 0)
        diff += 16;
    if (diff <= 8)
        cur++;
    else
        cur--;
    cur &= 0xF;
    draw_sprite(level[2], (cur & 3) + 0x2059);
    move_sprite(level[3], nedl_x[cur] + 0x78, nedl_y[cur] + 0x2B);
    draw_sprite(level[3], cur + 0x205D);
    setting[2] = cur;
}

void far adjust_power(void)
{
    static int spr = 0;
    static int last = 0;
    static int frame = 9;
    char p;

    p = goal[3];
    if (p >= 0 && p <= 10) {
        if (spr == 0) {
            Transparency = 1;
            spr = create_sprite(1, 0xE, 5);
            Transparency = 0;
            change_sprite(spr, 0x71, 0x30, 1, 1);
        }
        if (p == 9) {
            draw_sprite(spr, frame++ + 0x2074);
            if (frame > 10)
                frame = 9;
            slow_adjust |= 8;
        } else {
            if (last == 9)
                frame = 9;
            draw_sprite(spr, p + 0x2074);
            slow_adjust &= ~8;
        }
        last = p;
    }
}

void far adjust_panel(void)
{
    if (setting[6] != goal[6]) {
        if (pbuf.flag == 0) {
            pbuf.flag = 1;
            init_panelflip(goal[6]);
            RightPanel = 4;
        }
        if (do_panel_frame() == 1) {
            RightPanel = setting[6] = goal[6];
            pbuf.flag = 0;
            fast_adjust &= ~0x40;
        }
    }
}

void far adjust_eyes(void)
{
    static unsigned char wait = 0;

    if (setting[7] == goal[7]) {
        goal[7] += 4;
        eyes_frame = 2;
        wait = 0;
    } else if (setting[7] != goal[7] - 4) {
        if (wait != 0) {
            eyes_frame = 2;
            wait = 0;
        }
        setting[7] = goal[7];
        goal[7] += 4;
    }
    if (eyes_frame == 3 && wait < 0x10)
        wait++;
    else
        draw_sprite(eyes, eyes_seq[eyes_frame++] + (setting[7] - 1) * 3);
    if (eyes_frame > 5) {
        setting[7] = eyes_frame = wait = 0;
        draw_sprite(eyes, 0x207F);
        slow_adjust &= ~0x80;
    }
}

unsigned char far * far adr_weapon(int size)
{
    unsigned char far *p;

    if (wbuf.buf == 0) {
        MapMemory_seg013_1D3C_C7(2, scrgr_fpage);
        wbuf.buf = MK_FP(ems_frame, 0x8000);
        obj_inpage1 = 0xFF;
    }
    p = wbuf.buf;
    wbuf.buf += size;
    return p;
}

char far move_weapon(unsigned char far *p, int size, int n)
{
    if (n == 0)
        weap_offs[0] = 0;
    weap_offs[n + 1] = weap_offs[n] + size;
    return 1;
}

void far load_weapon(char id)
{
    weapid = id;
    if (id >= 0 && id <= 3 || last_weap != weapid)
        fast_adjust |= 0x100;
}

char far do_weapload(void)
{
    static unsigned char idle[9] = { 0x20, 0x21, 0x42, 0x63, 0x64, 0x65, 0x86, 0xA7, 0xE8 };
    char ok;
    int start;
    int off;
    unsigned char count;
    int i;
    FILE *fp;

    if (last_weap == weapid)
        return 1;
    last_weap = weapid;
    if (weapid == -1) {
        memset(wframe, 0, 0x1F);
        for (i = 0; i < 9; i++)
            wframe[i] = wframe[i + 9] = wframe[i + 18] = idle[i];
    }
    if (weapid > 3 || weapid < 0)
        return 0;
    wbuf.buf = 0;
    off = ((1 - player->lefty) * 4 + weapid) * 0x61;
    fp = fopen("DATA\\weap.dat", "rb");
    ok = fp != 0;
    if (ok) {
        ok &= fseek(fp, off, 0) == 0;
        ok &= fread(&count, 1, 1, fp) == 1;
        ok &= fread(frmtot, 1, 3, fp) == 3;
        ok &= fread(wframe, 1, 0x1F, fp) == 0x1F;
        ok &= fread(weap_x, 1, 0x1F, fp) == 0x1F;
        ok &= fread(weap_y, 1, 0x1F, fp) == 0x1F;
        ok &= fclose(fp) == 0;
    }
    start = (1 - player->lefty) * 0x7C + weapid * 0x1F;
    ok &= gronk_gr("weap", start, count, adr_weapon, move_weapon);
    return ok;
}

void far jiggle_weapon(int how)
{
    switch (how) {
    case 0:
        wxo = 0;
        break;
    case 1:
        if (wxo > 0)
            wxo = rand() % 5;
        else
            wxo = -rand() % 5;
        break;
    case 2:
        wxo = (wxo > 0 ? rand() : -rand()) % 10;
        break;
    }
}

void far adjust_weapon(void)
{
    static unsigned char swung = 0;
    int n;

    editchng(2);
    if (goal[8] > 6)
        goal[8] = 6;
    if (goal[8] == 6) {
        if (setting[8] != 6 && setting[8] != 5) {
            setting[8] = 5;
            weap_frame = -1;
        }
    } else if (goal[8] == 4) {
        if (setting[8] == 6 || setting[8] == 5) {
            setting[8] = 3;
            weap_frame = 3;
        } else if (swung) {
            setting[8] = 4;
            weap_frame = -1;
        } else if (setting[8] >= 0 && setting[8] <= 2) {
            if ((wframe[get_wfr(weap_frame)] & 0xE0) == 0x20)
                setting[8] = 4;
        } else if (setting[8] == 4 && weapid == last_weap) {
            fast_adjust &= ~0x100;
            return;
        }
    } else
        setting[8] = goal[8];
    switch (setting[8]) {
    case 5:
        if (++weap_frame >= 3)
            setting[8] = 6;
        break;
    case 3:
        if (--weap_frame < 0)
            setting[8] = 4;
        break;
    case 6:
        if (weapid != last_weap && !wbuf.flag) {
            do_weapload();
            goal[8] = wlstate;
            wlstate = 6;
            break;
        }
        if (wbuf.flag) {
            do_weapload();
            wbuf.flag = 0;
            wlstate = 6;
        }
        weap_frame = -1;
        fast_adjust &= ~0x100;
        break;
    case 4:
        if (weapid != last_weap) {
            goal[8] = 6;
            wlstate = 4;
            break;
        }
        weap_frame = -1;
        fast_adjust &= ~0x100;
        break;
    default:
        n = get_wfr(++weap_frame);
        if (weap_frame == 0) {
            swung = 0;
            break;
        }
        if (frmtot[setting[8]] == weap_frame) {
            goal[8] = setting[8] = 4;
            weap_frame = -1;
            fast_adjust &= ~0x100;
            break;
        }
        if ((wframe[n] & 0xE0) == 0x40) {
            fast_adjust &= ~0x100;
            swung = 1;
        }
        break;
    }
}

char far load_weapcm(void)
{
    char ok;
    FILE *fp;
    int off;

    ok = (fp = fopen("DATA\\weap.cm", "rb")) != 0;
    if (ok) {
        off = player->body == 1 ? 0x10 : 0;
        ok &= fseek(fp, off, 0) == 0;
        ok &= fread(Palettes[30], 1, 0x10, fp) == 0x10;
        ok &= fclose(fp) == 0;
    }
    return ok;
}

void far set_runes(unsigned char *runes)
{
    static int spr[3] = { 0, 0, 0 };
    int i;

    if (spr[0] == 0) {
        Transparency = 1;
        for (i = 0; i < 3; i++) {
            spr[i] = create_sprite(1, 0x10, 0x10);
            change_sprite(spr[i], rune_x[i], 0x32, 0x10, 0x10);
        }
        Transparency = 0;
    }
    for (i = 0; i < 3; i++) {
        if (runes[i] >= 0 && runes[i] < 0x18)
            draw_sprite(spr[i], runes[i] + 0xE8);
        else
            erase_sprite(spr[i]);
    }
    update_sprites();
}

void far active_spells(unsigned char *spells)
{
    static int spr[3] = { 0, 0, 0 };
    int i;

    if (inplist->field8 == 1) {
        if (spr[0] == 0) {
            Transparency = 1;
            for (i = 0; i < 3; i++) {
                spr[i] = create_sprite(1, 0x10, 0x12);
                change_sprite(spr[i], spell_x[i], 0x37, 0x10, 0x12);
            }
            Transparency = 0;
        }
        for (i = 0; i < 3; i++) {
            if (spells[i] >= 0 && spells[i] < 0x1E)
                draw_sprite(spr[i], spells[i] + 0x208D);
            else
                erase_sprite(spr[i]);
        }
        update_sprites();
    }
}

void far init_panelflip(int panel)
{
    unsigned char far *buf;
    int old;

    MapMemory_seg013_1D3C_C7(2, scrgr_fpage + 1);
    buf = MK_FP(ems_seg + 0x800, 0);
    obj_inpage1 = 0xFF;
    if (!read_gr_far("panels", panel, buf))
        pfatal_code(0x300E);
    old = RightPanel;
    RightPanel = panel;
    inv_refresh = 0;
    mouse_hide();
    grSoftPageFlip();
    show(0xEB, 0xC0, buf, 0x70, 0x4F, 0, 0);
    panel_dispatch[panel]();
    MapMemory_seg013_1D3C_C7(2, scrgr_fpage + 1);
    obj_inpage1 = 0xFF;
    grab(buf, 0xEB, 0xC0, 0x4F, 0x70);
    grSoftPageFlip();
    mouse_show();
    RightPanel = old;
    inv_refresh = 1;
}

void far free_panelflip(void)
{
}

void far pretty_panelagain(void)
{
    if (read_gr_far("panels", RightPanel, stdat)) {
        mouse_hide();
        grSoftPageFlip();
        show(0xEB, 0xC0, stdat, 0x70, 0x4F, 0, 0);
        panel_dispatch[RightPanel]();
        grSoftPageFlip();
        set_the_color(0x106);
        rectangle(0xEB, 0xC0, 0x139, 0x51);
        mouse_show();
    }
}

char far do_panel_frame(void)
{
    int frame;
    unsigned char far *buf;
    int l;
    int t;
    int r;
    int b;

    frame = -1;
    pbuf.frame++;
    mouse_hide();
    if (pbuf.frame % 3 == 1) {
        if (pbuf.frame <= 4)
            frame = pbuf.frame / 3 + 0x2089;
        else if (pbuf.frame > 10 && pbuf.frame <= 0x10)
            frame = (pbuf.frame - 10) / 3 + 0x208A;
        if (frame != -1)
            pic_to_screen(frame, 0x110, 0x50, 1, 1);
    }
    if (pbuf.frame <= 10)
        seg003_0272_51C8(0xEC, 0xC0, 0x43, 0x70, 0xF4, 0xC0);
    else {
        MapMemory_seg013_1D3C_C7(2, scrgr_fpage + 1);
        buf = MK_FP(ems_frame, 0x8000);
        obj_inpage1 = 0xFF;
        l = *wleft;
        t = *wtop;
        r = *wright;
        b = *wbot;
        set_the_window(l, t, 0x138, b);
        ShowClip = 1;
        show((0x14 - pbuf.frame) * 8 + 0xEB, 0xC0, buf, 0x70, 0x4F, 0, 0);
        ShowClip = 0;
        set_the_window(l, t, r, b);
    }
    if (pbuf.frame == 1) {
        set_the_color(1);
        rectangle(0xEC, 0xC0, 0xF4, 0x51);
    } else if (pbuf.frame == 0x14)
        pbuf.frame = 0;
    mouse_show();
    return pbuf.frame == 0;
}

void far restore_sliding_panel(int redraw)
{
    pbuf.flag = 0;
    pbuf.frame = 0;
    RightPanel = setting[6] = goal[6];
    if (redraw)
        pretty_panelagain();
}

int far get_wfr(int f)
{
    int n;

    if (setting[8] == 3 || setting[8] == 5)
        n = f + 0x1B;
    else if (f < 0 || setting[8] == 4)
        n = 0x1B;
    else {
        n = setting[8] * 9 + f;
        wxo = 0;
    }
    return n;
}

char ShowStupidFirstPersonWeapon = 1;

void far do_fbuf_bms(void)
{
    unsigned char far *p;
    int jig;
    int n;
    int f;

    Transparency = 1;
    if (setting[8] != 6 && weap_frame < 0x1F && last_weap > -1 && ShowStupidFirstPersonWeapon
        && !WizEye) {
        jig = PN.momentum ? PN.momentum * 2 / 0x31F + 1 : 0;
        jiggle_weapon(jig);
        n = get_wfr(weap_frame);
        f = wframe[n] & 0x1F;
        if (f < 0x1F && weap_offs[f] != weap_offs[f + 1]) {
            MapMemory_seg013_1D3C_C7(2, scrgr_fpage);
            wbuf.buf = MK_FP(ems_seg + 0x800, weap_offs[f]);
            obj_inpage1 = 0xFF;
            p = grs_unpack(wbuf.buf);
            fbshow(p, weap_x[f] + wxo, weap_y[f], wbuf.buf[1], wbuf.buf[2]);
        }
    }
    pic_to_fbuf(0x1090, 0x5E, 0x80);
    Transparency = 0;
}

void far send_FB(void)
{
    if (demo_mode)
        do_fbuf_bms();
    cFBtoScreen();
}

/* Two empty functions; FM Towns folded player_look_shaft and player_look_grave into one
   address, so which DOS copy carries which name can't be proven. */
void far player_look_shaft(void)
{
}

void far player_look_grave(void)
{
}
