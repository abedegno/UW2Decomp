/* target: seg037_32C0 */
/* opts: -mm -1 -G -O -Y -d */
/* The screen furniture around the 3D view: the vitality and mana flasks, the compass, the
   power gem, the eyes, the first-person weapon and its frames from DATA\weap.dat, the rune
   shelf and active spell icons, the sliding stat/inventory/rune panel, and the frame buffer
   send. The whole of DOS resident segment seg037_32C0, in original order.

   What it does in the game: the rest of the game asks for a display element to change
   with set_screen_frame(which, value), which sets that element's goal; update_screen, run
   each frame, moves each element a step towards its goal through its adjust_ function,
   some at once, some every 32 ticks of the clock and some every 64 (the flasks bubble and
   the compass needle swings at the slow rate). The elements, by index (adjust, goal,
   setting): 0 the vitality flask, 1 the mana flask (both shown in 12 steps of their
   maximum; the vitality flask turns green while poisoned), 2 the compass (16 headings), 3
   the power gem (the attack's charge, 9 the pulsing ready state), 6 the right-hand panel
   (it slides across when changed), 7 the gargoyle eyes and 8 the weapon (0..2 a swing of
   that kind, 3 drawing, 4 ready, 5 sheathing, 6 sheathed). The first-person weapon's
   pictures come from WEAP.GR by the weapon kind and hand, with per-frame positions from
   DATA\weap.dat, and are drawn into the frame buffer before it is sent to the screen
   (do_fbuf_bms, send_FB).

   Data owned: the goal, setting and adjust tables, the flask, compass and weapon frame
   tables, RightPanel (0 inventory, 1 runes, 2 statistics, 4 while sliding) and
   panel_dispatch, the weapon frame state (weapid, weap_frame, wframe ...).
   Function and global names are the originals from the FM Towns symbol table, except where
   a comment says otherwise.
   Name: descriptive (the screen furniture around the 3D view: flasks, compass, weapon,
   panels). */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include "combat.h"
#include "critter.h"
#include "gfx.h"
#include "inv.h"
#include "motion.h"
#include "player.h"
#include "sys.h"
#include "ui.h"
#include "view3d.h"


/* This file's _BSS, DS:33E8-349A: the low byte of *Time at the last redraw, the weapon
   frame tables and the display elements' settings and goals. */
/* match: Turbo C lays it out by name: the static that FM Towns keeps just after `setting`
   has no original name, and old_time was chosen because it lands first, at DS:33E8, as
   in the EXE. */
static unsigned char old_time;
unsigned char weap_x[0x1F];
unsigned char weap_y[0x1F];
unsigned char setting[9];
int weap_offs[0x20];
unsigned char frmtot[3];
unsigned char wframe[0x1F];
unsigned char goal[9];


/* The panel showing on the right, an index into panel_dispatch (FM Towns _RightPanel). */
/* match: DS:79E, the first byte of this file's _DATA: seg035's data ends at 79E and this
   file's word-aligned _DATA holds DS:79F, so the byte is ours. */
unsigned char RightPanel = 0;
/* Which screen elements to redraw: now, on every 32nd tick, and on every 64th tick. One
   bit per element, by its index in `adjust`. */
/* name: static (no FM Towns names); ours. */
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
/* The sliding panel's progress. FM Towns keeps the panel buffer's pointer in pbuf too;
   here the buffer is in EMS. */
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

/* Draws flask which (0 vitality, 1 mana) at its goal level at once. */
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

/* Draws the compass at its goal heading at once. */
void far set_compass(void)
{
    int dir;

    dir = goal[2];
    draw_sprite(level[2], (dir & 3) + 0x2059);
    move_sprite(level[3], nedl_x[dir] + 0x78, nedl_y[dir] + 0x2B);
    draw_sprite(level[3], dir + 0x205D);
    update_sprites();
}

/* Resets every display element (the weapon sheathed, the inventory panel). */
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

/* Creates the sprites of the flasks, compass, needle and eyes once, then draws the whole
   frame: flasks, compass, eyes, rune shelf and the right-hand panel from PANELS.GR. */
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
        pfatal_code(ERR_READ | 0x4);
    show(0xEB, 0xC0, stdat, 0x70, 0x4F, 0, 0);
    panel_dispatch[RightPanel]();
    update_sprites();
    grPageFlip();
    grSoftPageFlip();
}

/* Stops a panel slide where it is, taking the target panel at once. */
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

/* Sets display element which's goal to val (see the file comment). Flask values are hit
   points and mana, scaled to 12 steps; the weapon's goal is held back while another weapon's
   pictures are to be loaded. */
void far set_screen_frame(char which, int val)
{
    register int max;

    switch (which) {
    case 0:
    case 1:
        max = which == 1 ? player->max_mana : playerdat->avghit;
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

/* Called every frame: steps the display elements whose redraw bits are set, the 'now'
   ones at once, the fast ones every 32 ticks, the slow ones every 64 (when the flasks may
   also start bubbling at random). */
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

/* Moves flask which one step towards its goal (filling or draining) and animates its
   bubbles; the vitality flask switches to the green set while the player is poisoned. */
void far adjust_flasks(int which)
{
    static int bub_frame[2] = { 0x2019, 0x2032 };
    static int bub_spr[2] = { 0, 0 };
    static int drain_spr[2] = { 0, 0 };
    static int mask_spr[2] = { 0, 0 };
    /* match: the flask is used through a register copy, but the bit for `slow_adjust` is
       shifted by the parameter itself (FM Towns does the same); without the copy the
       compiler puts `base` in DI and leaves the parameter on the stack. */
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

/* Turns the compass one step (of 16) the short way towards its goal. */
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

/* Shows the power gem: frame p (COMBAT.C sets 0 at rest and 1 + charge / 12 while a blow
   charges); 9 pulses between two frames. */
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

/* Slides the right-hand panel towards the goal panel, a frame at a time (do_panel_frame),
   with RightPanel 4 while it moves. */
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

/* Animates the gargoyle's eyes above the view: a blink sequence of five frames, the set
   chosen by the goal (inferred: set_screen_frame(7, n) picks the expression). */
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

/* gronk_gr callback: the next place in the EMS weapon buffer for a picture. */
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

/* gronk_gr callback: records where each weapon picture starts. */
char far move_weapon(unsigned char far *p, int size, int n)
{
    if (n == 0)
        weap_offs[0] = 0;
    weap_offs[n + 1] = weap_offs[n] + size;
    return 1;
}

/* Asks for weapon kind id's pictures (0..3; -1 none) to be loaded at the next weapon redraw. */
void far load_weapon(char id)
{
    weapid = id;
    if (id >= 0 && id <= 3 || last_weap != weapid)
        fast_adjust |= 0x100;
}

/* Loads the pictures and frame tables of the weapon kind weapid for the player's hand
   (lefty): 0x61 bytes of DATA\weap.dat (a picture count, frame totals per swing, the frame
   of each step and the x, y of each picture) and the pictures from WEAP.GR. With no
   weapon, the idle frames are used. */
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
    ok &= (char)gronk_gr("weap", start, count, (ArtAllocFn)adr_weapon, (ArtMoveFn)move_weapon);
    return ok;
}

/* Sways the weapon sideways as the player moves: how 0 still, 1 a little, 2 more. */
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

/* Steps the first-person weapon: drawing and sheathing play their three frames, a swing
   plays its frames until its total, then it is ready again; a new weapon's pictures are
   loaded while sheathed. */
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

/* Reads the weapon's colour map for the player's skin (DATA\weap.cm, 16 bytes, the second
   set for body type 1) into palette 30. */
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

/* Shows the three runes on the shelf (RUNE_NONE leaves a place empty). */
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
        if (runes[i] >= 0 && runes[i] < NUM_RUNES)
            draw_sprite(spr[i], runes[i] + FIRST_RUNESTONE);
        else
            erase_sprite(spr[i]);
    }
    update_sprites();
}

/* Shows the icons of up to three active spells (0..0x1D) in the 3D view mode. */
void far active_spells(unsigned char *spells)
{
    static int spr[3] = { 0, 0, 0 };
    int i;

    if (inplist->mode == 1) {
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

/* Prepares a panel slide: draws panel into the EMS buffer to slide in. */
void far init_panelflip(int panel)
{
    unsigned char far *buf;
    int old;

    MapMemory_seg013_1D3C_C7(2, scrgr_fpage + 1);
    buf = MK_FP(EmsBuff + 0x800, 0);
    obj_inpage1 = 0xFF;
    if (!read_gr_far("panels", panel, buf))
        pfatal_code(ERR_READ | 0xE);
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

/* Redraws the right-hand panel in full. */
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

/* One frame of the panel slide: the old panel moves out, then the new one moves in, over
   20 frames with a two-frame picture on top. Returns 1 when done. */
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
        vcopy(0xEC, 0xC0, 0x43, 0x70, 0xF4, 0xC0);
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

/* Ends a slide at once, optionally redrawing the panel. */
void far restore_sliding_panel(int redraw)
{
    pbuf.flag = 0;
    pbuf.frame = 0;
    RightPanel = setting[6] = goal[6];
    if (redraw)
        pretty_panelagain();
}

/* The index into wframe for step f of the weapon's current state: 9 frames a swing kind,
   then the draw/sheathe and ready frames from 0x1B. */
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

/* Draws the first-person weapon into the frame buffer (unless sheathed, turned off, or
   the camera is detached by Roaming Sight), swaying with the player's speed, and the
   picture at (0x5E, 0x80) on top. */
void far do_fbuf_bms(void)
{
    unsigned char far *p;
    int jig;
    int n;
    int f;

    Transparency = 1;
    if (setting[8] != 6 && weap_frame < 0x1F && last_weap > -1 && ShowStupidFirstPersonWeapon
        && !WizEye) {
        jig = PN.speed ? PN.speed * 2 / 0x31F + 1 : 0;
        jiggle_weapon(jig);
        n = get_wfr(weap_frame);
        f = wframe[n] & 0x1F;
        if (f < 0x1F && weap_offs[f] != weap_offs[f + 1]) {
            MapMemory_seg013_1D3C_C7(2, scrgr_fpage);
            wbuf.buf = MK_FP(EmsBuff + 0x800, weap_offs[f]);
            obj_inpage1 = 0xFF;
            p = grs_unpack(wbuf.buf);
            fbshow(p, weap_x[f] + wxo, weap_y[f], wbuf.buf[1], wbuf.buf[2]);
        }
    }
    pic_to_fbuf(0x1090, 0x5E, 0x80);
    Transparency = 0;
}

/* Copies the 3D frame buffer to the screen, after drawing the weapon in demo mode. */
void far send_FB(void)
{
    if (demo_mode)
        do_fbuf_bms();
    cFBtoScreen();
}

/* Two empty functions, called when the player looks at a shaft or a gravestone
   (LOOK.C). */
/* name: FM Towns folded player_look_shaft and player_look_grave into one address, so
   which DOS copy carries which name can't be proven. */
void far player_look_shaft(void)
{
}

void far player_look_grave(void)
{
}
