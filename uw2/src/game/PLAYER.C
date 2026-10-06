/* target: ovr143 */
/* opts: -mm -1 -G -O -Y -d */
/* Setting up the player: the player object, the key and mouse bindings for play, the
   mouse regions over the 3D view, the position and version reports, and the camera
   (roaming sight, attaching the view to an object, the crystal ball, the moongate
   vortex, looking up and down). The whole of DOS overlay ovr143, in original order.
   Function and global names are the originals from the FM Towns symbol table where it
   has them.
   Entry points: init_player (UWEDIT.C's init_world, once at start-up) makes the player
   object critdata[1], points player at PlayerDat (the struct Player record that
   PLAYDATA.C loads and saves), and registers every key and mouse binding of the game and
   conversation screens. mous_player and demous_player (the 3D view's start and exit in
   UWEDIT.C, and the view's resizing) set up the view's click region and the eight cursor
   regions round its edges. The camera functions (home_cam, move_cam, attach_eye,
   crystal_ball, the moongate vortex, chg_plyp) are called from the spells, the debug
   keys and the input handlers.
   Data owned: PlayerDat (the player record's storage), MoveCrits, nextstep, watertime,
   IsJoy, the 3D view's rectangle (PLeft, PBot, PWid, PHgt), curvrad and the region
   handles.
   Name: original (init_player is in System Shock's PLAYER.C, setting up the player in
   both). */

#include "combat.h"
#include "conv.h"
#include "critter.h"
#include "file.h"
#include "gfx.h"
#include "map.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"
#include "view3d.h"

/* match: declared here, not in map.h: LIGHTING.C defines set_light(signed char), and this
   file's callers push an int. */
void far set_light(int level);

/* match: the file's _DATA starts with these, DS:19DC to DS:19E6, where ovr142's data
   ends: the string after them is at the odd DS:19E7, so this file's word-aligned _DATA
   starts earlier, and watertime (to DS:19E6) began at DS:19E3, nextstep at DS:19DF,
   PMsHndle at DS:19DD, leaving DS:19DC. */
/* name: FM Towns has the four together too, as PMsHndle, MoveCrits, nextstep,
   watertime. */
unsigned char MoveCrits = 1;            /* seg035 moves critters only while set */
int16 PMsHndle = 0;                     /* input_addmouse's handle for the 3D view */
uint32 nextstep = 0;
uint32 watertime = 0;                   /* *Time when seg035 last applied water_eff */

/* This file's _BSS, DS:8298..8631 (ovr142's ends at 8297, ovr147's starts at 8632). The
   eight static ints are the handles of the cursor regions over the 3D view. PlayerDat is
   the storage of struct Player (0x37D bytes, one spare). */
/* match: laid out by name (tools/bssorder.py): IsJoy 1, region_south 18, PHgt 136, PLeft
   192, rgnh_ul 218, rgnh_left 226, PlayerDat 408, curvrad 555, region_br 706, PBot 712,
   region_dl 722, region_up 858, PWid 920, hrgn_ur 976, rgnh_r 1002. */
/* name: FM Towns keeps the region handles as statics, so they are static here, with
   provisional names chosen for their keys. */
unsigned char IsJoy;
static int16 region_south;              /* DS:829A, the region below the view */
int16 PHgt;
int16 PLeft;
static int16 rgnh_ul;                   /* DS:82A0, up and left */
static int16 rgnh_left;                 /* DS:82A2 */
union PlayerStore PlayerDat;
int16 curvrad;
static int16 region_br;                 /* DS:8624, down and right */
int16 PBot;
static int16 region_dl;                 /* DS:8628, down and left */
static int16 region_up;                 /* DS:862A */
int16 PWid;
static int16 hrgn_ur;                   /* DS:862E, up and right */
static int16 rgnh_r;                    /* DS:8630, right */

/* Resets the player object: no links, whoami 0xFD, the adventurer's item id, quality
   and owner zero. */
/* name: FM Towns calls it from init_player_structure; DOS from init_player, which holds
   that function's body. */
void far InitPlayerRec(void)
{
    ThePlayer->ol.f.link = 0;
    ThePlayer->whoami = 0xFD;
    SET_ISQUANT(ThePlayer, 0);
    SET_DOORDIR(ThePlayer, 1);
    SET_INVIS(ThePlayer, 0);
    SET_HEADING(ThePlayer, 0);
    SET_FINEHEAD(ThePlayer, 0);
    ThePlayer->qn.f.next = ThePlayer->qn.f.quality = 0;
    ThePlayer->ol.f.link = ThePlayer->ol.f.owner = 0;
    ThePlayer->b11 = 0;
    SET_ITEM(ThePlayer, ITEM_ADVENTURER);
}

/* Sets up the player at start-up: the player object is critdata[1], on level 1, with the physics square handler
   player_sqhandler; player points at PlayerDat; playerdat is the creature type record of
   the adventurer, whose average hit points are the starting HP; the player's name gets
   a string handle in block STRBLK_PLAYER. Then the bindings, each with the input modes
   it works in (third argument: 1 the game, 4 a conversation, others combinations):
   w a s d z c x e q j J  parse_playin's movement and jump commands;
   A D S X W and the three arrows over the view  player_simple_move;
   1 2 3                  change the view's pitch (chg_plyp: -1, centre, +1), and in
                          a conversation the answer to pick (conv_play_menu);
   special keys 0x80 to 0x89  the icon panel, pull_chain, sleep, use a skill (2, which
                          player_use_skill turns into SKILL_TRACK), cast;
   Ctrl+S R M F D Q       do_option_shortcut (save, restore, music, sound, detail, quit,
                          inferred from the letters);
   p . ;                  attack (player_attack 9, 3 and 6);
   Tab and the cursor keys  keyboard_mouse, the mouse from the keyboard;
   Escape and the barter areas  conversation screen;
   Alt+H                  toggles mouse_hand;
   Alt with 0x86 and 0x87  show_version and report_loc. */
void far init_player(void)
{
    nextstep = 0;
    watertime = 0;
    ThePlayer = critdata + 1;
    UsPtr = ThePlayer;
    GrSq = -1;
    PlayerHeading = 0;
    PlayerFacing = 0;
    PlayerPitch = 0;
    PlayerBank = 0;
    PlayerLevel = 1;
    PN.b24 = 8;
    PN.index = 1;
    PT.special = (unsigned char (far *)())player_sqhandler;
    PT.mask = 0x1100;
    PT.ignore = 0;
    set_light(0);
    PMsHndle = 0;
    IsJoy = (cJoyInit[0] | cJoyInit[1] | cJoyInit[2] | cJoyInit[3]) > 0;
    player = &PlayerDat.rec;
    InitPlayerRec();
    playerdat = &Creature[OBJ_INMAJOR(ThePlayer)];
    ThePlayer->hp = playerdat->avghit;
    if (player_name_handle == 0)
        player_name_handle = make_string((char far *)player, STRBLK_PLAYER);

    _input_addkey('w', 0x0E, 1, (InputFn)parse_playin);
    _input_addkey('s', 5, 1, (InputFn)parse_playin);
    _input_addkey('a', 3, 1, (InputFn)parse_playin);
    _input_addkey('d', 4, 1, (InputFn)parse_playin);
    _input_addkey('z', 9, 1, (InputFn)parse_playin);
    _input_addkey('c', 0x0A, 1, (InputFn)parse_playin);
    _input_addkey('x', 8, 1, (InputFn)parse_playin);
    _input_addkey('e', 0x0C, 0x1B, (InputFn)parse_playin);
    _input_addkey('q', 0x0D, 0x1B, (InputFn)parse_playin);
    _input_addkey('A', -1, 1, (InputFn)player_simple_move);
    _input_addkey('D', 1, 1, (InputFn)player_simple_move);
    _input_addkey('S', 0, 1, (InputFn)player_simple_move);
    _input_addkey('X', -2, 1, (InputFn)player_simple_move);
    _input_addkey('W', 2, 1, (InputFn)player_simple_move);
    input_addmouse(0x6B, 0x21, 0x7B, 0x2F, -1, 1, (InputFn)player_simple_move);
    input_addmouse(0x82, 0x1F, 0x92, 0x2C, 0, 1, (InputFn)player_simple_move);
    input_addmouse(0x9B, 0x21, 0xAA, 0x2F, 1, 1, (InputFn)player_simple_move);
    _input_addkey('3', 1, 0x11, (InputFn)chg_plyp);
    _input_addkey('1', -1, 0x11, (InputFn)chg_plyp);
    _input_addkey('2', 0, 0x11, (InputFn)chg_plyp);
    _input_addkey('j', 7, 0x1B, (InputFn)parse_playin);
    _input_addkey('J', 6, 0x1B, (InputFn)parse_playin);
    _input_addkey(KEY_F7, 0, 0x1B, (InputFn)pull_chain);
    _input_addkey(KEY_F10, 0, 0x1B, (InputFn)player_key_sleep);
    _input_addkey(KEY_F9, 2, 0x1B, (void (far *)())player_use_skill);
    _input_addkey(KEY_F8, 1, 0x1B, (InputFn)try_cast);
    _input_addkey(KEY_CTRL | 's', KEY_CTRL | 's', 1, (InputFn)do_option_shortcut);
    _input_addkey(KEY_CTRL | 'r', KEY_CTRL | 'r', 1, (InputFn)do_option_shortcut);
    _input_addkey(KEY_CTRL | 'm', KEY_CTRL | 'm', 1, (InputFn)do_option_shortcut);
    _input_addkey(KEY_CTRL | 'f', KEY_CTRL | 'f', 1, (InputFn)do_option_shortcut);
    _input_addkey(KEY_CTRL | 'd', KEY_CTRL | 'd', 1, (InputFn)do_option_shortcut);
    _input_addkey(KEY_CTRL | 'q', KEY_CTRL | 'q', 1, (InputFn)do_option_shortcut);
    _input_addkey(KEY_F6, 5, 1, (InputFn)deal_with_icons);
    _input_addkey(KEY_F4, 4, 1, (InputFn)deal_with_icons);
    _input_addkey(KEY_F3, 3, 1, (InputFn)deal_with_icons);
    _input_addkey(KEY_F5, 2, 1, (InputFn)deal_with_icons);
    _input_addkey(KEY_F1, 1, 1, (InputFn)deal_with_icons);
    _input_addkey(KEY_F2, 0, 1, (InputFn)deal_with_icons);
    _input_addkey('p', 9, 1, (InputFn)player_attack);
    _input_addkey('.', 3, 1, (InputFn)player_attack);
    _input_addkey(';', 6, 1, (InputFn)player_attack);
    _input_addkey(KEY_SHIFT | KEY_BACKTAB, KEY_SHIFT | KEY_BACKTAB, 7, (InputFn)keyboard_mouse);
    _input_addkey('\t', '\t', 7, (InputFn)keyboard_mouse);
    _input_addkey(KEY_UP, KEY_UP, 7, (InputFn)keyboard_mouse);
    _input_addkey(KEY_DOWN, KEY_DOWN, 7, (InputFn)keyboard_mouse);
    _input_addkey(KEY_LEFT, KEY_LEFT, 7, (InputFn)keyboard_mouse);
    _input_addkey(KEY_RIGHT, KEY_RIGHT, 7, (InputFn)keyboard_mouse);
    _input_addkey(KEY_HOME, KEY_HOME, 7, (InputFn)keyboard_mouse);
    _input_addkey(KEY_PGUP, KEY_PGUP, 7, (InputFn)keyboard_mouse);
    _input_addkey(KEY_END, KEY_END, 7, (InputFn)keyboard_mouse);
    _input_addkey(KEY_PGDN, KEY_PGDN, 7, (InputFn)keyboard_mouse);
    _input_addkey(KEY_INS, KEY_INS, 7, (InputFn)keyboard_mouse);
    _input_addkey(KEY_DEL, KEY_DEL, 7, (InputFn)keyboard_mouse);
    _input_addkey(KEY_ESC, 4, 4, (InputFn)do_escape_key);
    _input_addkey('1', 1, 4, (InputFn)conv_play_menu);
    _input_addkey('2', 2, 4, (InputFn)conv_play_menu);
    _input_addkey('3', 3, 4, (InputFn)conv_play_menu);
    _input_addkey('4', 4, 4, (InputFn)conv_play_menu);
    _input_addkey('5', 5, 4, (InputFn)conv_play_menu);
    input_addmouse(0x46, 0x87, 0x74, 0xBC, 4, 4, (InputFn)npc_barter);
    input_addmouse(0x77, 0x87, 0xA3, 0xBC, 4, 4, (InputFn)play_barter);
    input_addmouse(0x10, 1, 0xDF, 0x1E, 0, 4, (InputFn)conv_play_menu);
    _input_addkey(KEY_ALT | 'h', (NEARPTR)&mouse_hand, 0x1B, (InputFn)flip_bool);
    _input_addkey(KEY_ALT | KEY_F7, 0, 0x1B, (InputFn)show_version);
    _input_addkey(KEY_ALT | KEY_F8, 0, 0x1B, (InputFn)report_loc);
}

/* Makes the 3D view the given rectangle: one mouse region for clicks, and eight cursor
   regions around its edges for moving. */
void far mous_player(int left, register int bot, register int wid, int hgt)
{
    input_del(PMsHndle);
    PLeft = left;
    PBot = bot;
    PWid = wid;
    PHgt = hgt;
    PMsHndle = input_addmouse(left, bot, left + wid - 1, bot + hgt - 1, 0, 0x1B, (InputFn)mous_in_3d);
    rgnh_ul = defineMouseRegion(left, bot, left + wid * 5 / 15, bot + hgt * 3 / 15, 0x106F);
    hrgn_ur = defineMouseRegion(left + wid - wid * 5 / 15, bot, left + wid - 1, bot + hgt * 3 / 15, 0x1070);
    region_up = defineMouseRegion(left + wid * 5 / 15, bot, left + wid - wid * 5 / 15, bot + hgt * 3 / 15, 0x106E);
    rgnh_left = defineMouseRegion(left, bot + hgt * 3 / 15, left + wid * 5 / 15, bot + hgt * 6 / 15, 0x1071);
    rgnh_r = defineMouseRegion(left + wid - wid * 5 / 15, bot + hgt * 3 / 15, left + wid - 1, bot + hgt * 6 / 15, 0x1072);
    region_south = defineMouseRegion(left + wid * 5 / 15, bot + hgt * 6 / 15, left + wid - wid * 5 / 15, bot + hgt - 1, 0x106D);
    region_dl = defineMouseRegion(left, bot + hgt * 6 / 15, left + wid * 5 / 15, bot + hgt - 1, 0x1073);
    region_br = defineMouseRegion(left + wid - wid * 5 / 15, bot + hgt * 6 / 15, left + wid - 1, bot + hgt - 1, 0x1074);
}

void far demous_player(void)
{
    input_del(PMsHndle);
    PMsHndle = 0;
    undefineMouseRegion(rgnh_ul);
    undefineMouseRegion(hrgn_ur);
    undefineMouseRegion(rgnh_left);
    undefineMouseRegion(rgnh_r);
    undefineMouseRegion(region_up);
    undefineMouseRegion(region_south);
    undefineMouseRegion(region_dl);
    undefineMouseRegion(region_br);
}

/* Prints the level and the player's tile, each as two octal digits. */
/* name: FM Towns has a sprintf version at the same place. */
void far report_loc(void)
{
    char buf[8];
    int x, y;

    buf[0] = (PlayerLevel >> 3) + '0';
    buf[1] = (PlayerLevel & 7) + '0';
    x = OBJ_HOMEX(ThePlayer);
    y = OBJ_HOMEY(ThePlayer);
    buf[2] = (x >> 3) + '0';
    buf[3] = (x & 7) + '0';
    buf[4] = (y >> 3) + '0';
    buf[5] = (y & 7) + '0';
    buf[6] = '\n';
    buf[7] = 0;
    scroll_print(buf);
}

void far show_version(void)
{
    game_sprint(0x123);                 /* "Underworld II: Labyrinth of Worlds v" */
    scroll_print("F1.99S\n");
}

/* Puts the camera at the player (index 0 or 1) or at a mobile object. */
void far home_cam(int index)
{
    struct Object far *obj;

    if (index <= 1) {
        campos[0] = PN.x;
        campos[1] = PN.y;
        camang[0] = PlayerFacing;
        if (index == 1) {
            campos[2] = PN.z + 0xA4;
        } else {
            campos[2] = 0x458;
            camang[1] = -0x400;
        }
    } else if (index < NUM_MOBILE) {
        obj = Obj_IntTMem(index);
        campos[0] = (OBJ_HOMEX(obj) << 8) + (OBJ_FINEX(obj) << 5);
        campos[1] = (OBJ_HOMEY(obj) << 8) + (OBJ_FINEY(obj) << 5);
        campos[2] = OBJ_Z(obj) << 3;
        camang[0] = OBJ_HEADING(obj) << 13;
    }
    if (UsPtr == 0)
        editchng(2);
}

/* Moves the roaming camera from the mouse, the movement keys, or turns it on the spot. */
void far move_cam(int how)
{
    int16 dx, dy;
    int turn, step;

    switch (how) {
    case 0:
        turn = inplist->x * 3 / PWid;
        step = inplist->y * 3 / PHgt;
        break;
    case 2:
        if (TurnInpRate < 0)
            turn = 0;
        else if (TurnInpRate > 0)
            turn = 2;
        else
            turn = 1;
        if (ForwInpRate > 0)
            step = 2;
        else
            step = 1;
        break;
    case 9:
        turn = 1;
        step = 0;
        break;
    default:
        return;
    }
    camang[0] += (turn - 1) << 10;
    if (step != 1) {
        cSinCos(camang[0], &dx, &dy);
        campos[0] += (dx >> 8) * (step - 1);
        campos[1] += (dy >> 8) * (step - 1);
    }
    if (campos[0] < 0x180)
        campos[0] = 0x180;
    else if (campos[0] > 0x3D80)
        campos[0] = 0x3D80;
    if (campos[1] < 0x180)
        campos[1] = 0x180;
    else if (campos[1] > 0x3D80)
        campos[1] = 0x3D80;
    if (UsPtr == 0)
        editchng(2);
}

/* Chooses what the view is attached to: -1 none (the camera), 0 the selected object,
   1 the player, 2 and 3 step back through the mobile objects. */
void far attach_eye(int mode)
{
    int i;

    switch (mode) {
    case 3:
        if (critdata - 1 <= UsPtr) {
            UsPtr = critdata - 2;
            editchng(2);
        }
    case 2:
        if (UsPtr >= critdata) {
            UsPtr = critdata - 1;
            editchng(2);
        }
        break;
    case 1:
        if (UsPtr != ThePlayer) {
            UsPtr = ThePlayer;
            editchng(2);
        }
        break;
    case 0:
        if (curelem == 0 || (i = Obj_MemTPtr(curelem)) == 0 || i >= NUM_MOBILE || i <= 1)
            break;
        UsPtr = critdata + i;
        editchng(2);
        break;
    case -1:
        UsPtr = 0;
        break;
    }
}

void far release_camera(int index)
{
    home_cam(index);
    attach_eye(-1);
}

/* Shows the view from a crystal ball at tile (x, y), with automap updates off while it
   is shown. */
void far crystal_ball(struct Object far *obj, int x, int y)
{
    char automap;

    if (player->skills[SKILL_SEARCH] == 0x2D)   /* why 45 in search stops it is not known */
        return;
    campos[0] = (x << 8) + (OBJ_FINEX(obj) << 5);
    campos[1] = (y << 8) + (OBJ_FINEY(obj) << 5);
    campos[2] = OBJ_Z(obj) << 3;
    camang[0] = OBJ_HEADING(obj) << 13;
    camang[1] = 0;
    camang[2] = 0;
    set_light(6);
    automap = player->automap;
    if (automap)
        player->automap = 0;
    cameras_fade();
    if (automap)
        player->automap = 1;
    FixPlayerEquips();
}

/* Spins the view into the moongate at tile (32, 32): the camera is detached, the
   distance and angle to the gate found, and 64 frames are drawn with the vortex angle
   stepping by 0xCCB each, then the view goes back to the player. */
/* name: not in the FM Towns build; the name is provisional (IDA's
   LaunchPlayerAtMoongate_ovr143_E09), chosen so that its tools/bssorder.py key puts it in
   the EXE's overlay stub order. */
void far Vortex_ovr143_E09(void)
{
    int32 xd, yd;
    int x, y;

    vort_x = vort_y = 0x20;
    attach_eye(3);
    xd = (vort_x << 8) - PN.x;
    yd = (vort_y << 8) - PN.y;
    vort_rad = cSqRt(xd * xd + yd * yd) >> 6;
    /* match: this file treated vort_rad and vort_timer as unsigned */
    x = (xd << 15) / ((int32)(unsigned)vort_rad << 6);
    y = (yd << 15) / ((int32)(unsigned)vort_rad << 6);
    vort_theta = cAtan2(y, x);
    for (vort_timer = 0; (unsigned)vort_timer < 0x40; vort_timer++) {
        establish_view();
        vort_theta += 0xCCB;
    }
    attach_eye(1);
}

/* Steps a view angle by dir: unbounded when limit is 0, otherwise through mvcheck.
   A dir of 0 recentres it. */
void far chg_plys(int16 *val, int dir, int limit)
{
    if (dir == 0) {
        editchng(2);
        *val = 0;
    } else if (limit == 0) {
        *val += dir << 10;
        editchng(2);
    } else if (mvcheck(val, dir == -1 ? -limit : limit, 0x400, dir)) {
        editchng(2);
    }
}

/* Rolls the view (the camera's, or the player's bank). */
/* name: not in the FM Towns build; the name is provisional (IDA's
   ChangeCameraRoll_ovr143_F58), chosen so that its key puts it in the EXE's stub order. */
void far RollView_ovr143_F58(int step)
{
    if (MoveCamera)
        chg_plys(&camang[2], step, 0);
    else
        chg_plys(&PlayerBank, step, 0);
}

/* Changes the pitch by step (the 1, 2 and 3 keys; 0 recentres), within 0x1000 either
   way. */
void far chg_plyp(int step)
{
#ifndef __TURBOC__
    /* port only: --enhance wide-pitch, three times the original's bound */
    if (MoveCamera)
        chg_plys(&camang[1], step, port_pitch_bound());
    else
        chg_plys(&PlayerPitch, step, port_pitch_bound());
#else
    if (MoveCamera)
        chg_plys(&camang[1], step, 0x1000);
    else
        chg_plys(&PlayerPitch, step, 0x1000);
#endif
}

#ifndef __TURBOC__
/* The pitch's bound either way: the original's 0x1000, or with --enhance wide-pitch 0x3000
   (UltimaHacks' pitchBound.asm). Mouse-look clamps to it too. */
int port_pitch_bound(void)
{
    return ENHANCED(ENH_WIDE_PITCH) ? 0x3000 : 0x1000;
}
#endif
