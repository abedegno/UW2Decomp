/* target: ovr143 */
/* opts: -mm -1 -G -O -Y -d */
/* Setting up the player: the player object, the key and mouse bindings for play, the
   mouse regions over the 3D view, the position and version reports, and the camera
   (roaming sight, attaching the view to an object, the crystal ball, the moongate
   vortex, looking up and down). The whole of DOS overlay ovr143, in original order.
   Function and global names are the originals from the FM Towns symbol table where it
   has them; the source file's own name is not known. */

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0x21];
    unsigned char skills[20];           /* 0x21: search is 0x0B */
    char pad1[0x60 - 0x35];
    unsigned b60:1;                     /* 0x60 */
    unsigned poison:4;
    unsigned active_spells:4;
    unsigned b60_9:3;
    unsigned shrooms:2;
    unsigned drunk:6;                   /* word 0x61, bits 6..11 */
    unsigned automap:1;                 /* word 0x62, bit 4 */
    unsigned b62_5:11;
};

/* A critter type, 48 bytes. */
struct Creature {
    char pad0[4];
    unsigned char avghit;               /* 0x04 */
    char pad5[0x30 - 5];
};

/* A mobile object, 27 bytes. The first 8 bytes are shared with static objects. */
struct Object {
    unsigned id;                        /* item 0-8 (index 0-5), bits 13-15 flags */
    unsigned pos;                       /* z 0-6, heading 7-9, y fine 10-12, x fine 13-15 */
    union {
        unsigned word;
        struct { unsigned quality:6, next:10; } f;
    } qn;
    union {
        unsigned word;
        struct { unsigned owner:6, link:10; } f;
    } ol;
    unsigned char hp;                   /* 0x08 */
    char pad09[0x11 - 0x09];
    unsigned char b11;                  /* 0x11, damage taken */
    char pad12[0x16 - 0x12];
    unsigned home;                      /* 0x16, x in bits 10-15, y in bits 4-9 */
    unsigned char b18;                  /* 0x18, fine heading in bits 0-4 */
    char pad19;
    unsigned char whoami;               /* 0x1A */
};

#define OBJ_INDEX(o)    (((o)->id & 0x3F) >> 0)
#define OBJ_Z(o)        ((o)->pos & 0x7F)
#define OBJ_HEADING(o)  (((o)->pos & 0x380) >> 7)
#define OBJ_FINEY(o)    (((o)->pos & 0x1C00) >> 10)
#define OBJ_FINEX(o)    (((o)->pos & 0xE000) >> 13)
#define OBJ_HOMEX(o)    (((o)->home & 0xFC00) >> 10)
#define OBJ_HOMEY(o)    (((o)->home & 0x3F0) >> 4)

/* The player's motion record. */
struct Motion {
    int x, y, z;                        /* 0x00 */
    char pad6[0x20 - 6];
    int w20;                            /* 0x20 */
    char pad22[0x24 - 0x22];
    unsigned char b24;                  /* 0x24 */
};

/* The player's motion handler record. */
struct PhysThing {
    int flags;                          /* 0x00 */
    int w2;                             /* 0x02 */
    char pad4[8 - 4];
    char (far *handler)(unsigned *w);   /* 0x08 */
};

/* The mouse and keyboard state handed to an input handler. */
struct Inplist {
    int x, y;                           /* the mouse position */
};

/* The file's _DATA starts with these, DS:19DC to DS:19E6, where ovr142's data ends: the
   string after them is at the odd DS:19E7, so this file's word-aligned _DATA starts earlier,
   and watertime (to DS:19E6) began at DS:19E3, nextstep at DS:19DF, PMsHndle at DS:19DD,
   leaving DS:19DC. FM Towns has the four together too, as PMsHndle, MoveCrits, nextstep,
   watertime. */
unsigned char MoveCrits = 1;            /* seg035 moves critters only while set */
int PMsHndle = 0;                       /* input_addmouse's handle for the 3D view */
unsigned long nextstep = 0;
unsigned long watertime = 0;            /* *Time when seg035 last applied water_eff */

extern struct Object far *critdata;
extern struct Object far *ThePlayer;
extern struct Object far *UsPtr;
extern struct Object far *curelem;
extern int GrSq;
extern int PlayerHeading;
extern int PlayerFacing;
extern int PlayerPitch;
extern int PlayerBank;
extern int PlayerLevel;
extern struct Motion PN;
extern struct PhysThing PT;
extern int far *cJoyInit;
extern struct Player near *player;
extern struct Creature near *playerdat;
extern struct Creature Creature[];
extern int player_name_handle;
extern char mouse_hand;
extern int campos[3];
extern int camang[3];
extern struct Inplist near *inplist;
extern int TurnInpRate;
extern int ForwInpRate;
extern char MoveCamera;
extern unsigned char vort_x, vort_y;
extern unsigned vort_rad;
extern unsigned vort_timer;
extern int vort_theta;

/* This file's _BSS, DS:8298..8631 (ovr142's ends at 8297, ovr147's starts at 8632), laid
   out by name (tools/bssorder.py): IsJoy 1, region_south 18, PHgt 136, PLeft 192, rgnh_ul 218,
   rgnh_left 226, PlayerDat 408, curvrad 555, region_br 706, PBot 712, region_dl 722,
   region_up 858, PWid 920, hrgn_ur 976, rgnh_r 1002. The eight are the handles of the cursor
   regions over the 3D view: FM Towns keeps them as statics, so they are static here, with
   provisional names chosen for their keys. */
char IsJoy;
static int region_south;                /* DS:829A, the region below the view */
int PHgt;
int PLeft;
static int rgnh_ul;                     /* DS:82A0, up and left */
static int rgnh_left;                   /* DS:82A2 */
char PlayerDat[0x37E];
int curvrad;
static int region_br;                   /* DS:8624, down and right */
int PBot;
static int region_dl;                   /* DS:8628, down and left */
static int region_up;                   /* DS:862A */
int PWid;
static int hrgn_ur;                     /* DS:862E, up and right */
static int rgnh_r;                      /* DS:8630, right */

/* Elsewhere in the game. */
void far _input_addkey(int key, int a, int b, void (far *handler)());
int far input_addmouse(int x0, int y0, int x1, int y1, int buttons, int mode, void (far *handler)());
void far input_del(int handle);
int far defineMouseRegion(int x0, int y0, int x1, int y1, int id);
void far undefineMouseRegion(int handle);
void far set_light(int level);
int far make_string(char far *s, int len);
void far editchng(int bits);
void far game_sprint(int id);
void far scroll_print(char far *s);
struct Object far * far Obj_IntTMem(int index);
int far Obj_MemTPtr(struct Object far *obj);
void far cSinCos(int angle, int *x, int *y);
int far cSqRt(long v);
int far cAtan2(int x, int y);
void far establish_view(void);
int far mvcheck(int *val, int amount, int step, int dir);
void far cameras_fade(void);
void far FixPlayerEquips(void);
char far player_sqhandler(unsigned *w);

/* Input handlers. */
void far parse_playin(int how);
void far player_simple_move(int how);
void far pull_chain(int a);
void far player_key_sleep(int bedroll);
char far player_use_skill(int skill);
void far try_cast(int how);
void far do_option_shortcut(int keycode);
void far deal_with_icons(int mode);
void far player_attack(int how);
void far keyboard_mouse(int key);
void far do_escape_key(int a);
void far conv_play_menu(int n);
void far npc_barter(int a);
void far play_barter(int a);
void far flip_bool(char *b);
void far mous_in_3d(int a);
void far chg_plyp(int step);
void far show_version(void);
void far report_loc(void);

/* Resets the player object. FM Towns calls it from init_player_structure; DOS from
   init_player, which holds that function's body. */
void far InitPlayerRec(void)
{
    ThePlayer->ol.f.link = 0;
    ThePlayer->whoami = 0xFD;
    ThePlayer->id = ThePlayer->id & 0x7FFF;
    ThePlayer->id = ThePlayer->id & 0xDFFF | 0x2000;
    ThePlayer->id = ThePlayer->id & 0xBFFF;
    ThePlayer->pos = ThePlayer->pos & 0xFC7F;
    ThePlayer->b18 = ThePlayer->b18 & 0xE0;
    ThePlayer->qn.f.next = ThePlayer->qn.f.quality = 0;
    ThePlayer->ol.f.link = ThePlayer->ol.f.owner = 0;
    ThePlayer->b11 = 0;
    ThePlayer->id = ThePlayer->id & 0xFE00 | 0x7F;
}

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
    PN.w20 = 1;
    PT.handler = player_sqhandler;
    PT.w2 = 0x1100;
    PT.flags = 0;
    set_light(0);
    PMsHndle = 0;
    IsJoy = (cJoyInit[0] | cJoyInit[1] | cJoyInit[2] | cJoyInit[3]) > 0;
    player = (struct Player near *)PlayerDat;
    InitPlayerRec();
    playerdat = &Creature[OBJ_INDEX(ThePlayer)];
    ThePlayer->hp = playerdat->avghit;
    if (player_name_handle == 0)
        player_name_handle = make_string((char far *)player, 0x7D);

    _input_addkey('w', 0x0E, 1, parse_playin);
    _input_addkey('s', 5, 1, parse_playin);
    _input_addkey('a', 3, 1, parse_playin);
    _input_addkey('d', 4, 1, parse_playin);
    _input_addkey('z', 9, 1, parse_playin);
    _input_addkey('c', 0x0A, 1, parse_playin);
    _input_addkey('x', 8, 1, parse_playin);
    _input_addkey('e', 0x0C, 0x1B, parse_playin);
    _input_addkey('q', 0x0D, 0x1B, parse_playin);
    _input_addkey('A', -1, 1, player_simple_move);
    _input_addkey('D', 1, 1, player_simple_move);
    _input_addkey('S', 0, 1, player_simple_move);
    _input_addkey('X', -2, 1, player_simple_move);
    _input_addkey('W', 2, 1, player_simple_move);
    input_addmouse(0x6B, 0x21, 0x7B, 0x2F, -1, 1, player_simple_move);
    input_addmouse(0x82, 0x1F, 0x92, 0x2C, 0, 1, player_simple_move);
    input_addmouse(0x9B, 0x21, 0xAA, 0x2F, 1, 1, player_simple_move);
    _input_addkey('3', 1, 0x11, chg_plyp);
    _input_addkey('1', -1, 0x11, chg_plyp);
    _input_addkey('2', 0, 0x11, chg_plyp);
    _input_addkey('j', 7, 0x1B, parse_playin);
    _input_addkey('J', 6, 0x1B, parse_playin);
    _input_addkey(0x86, 0, 0x1B, pull_chain);
    _input_addkey(0x89, 0, 0x1B, player_key_sleep);
    _input_addkey(0x88, 2, 0x1B, (void (far *)())player_use_skill);
    _input_addkey(0x87, 1, 0x1B, try_cast);
    _input_addkey(0x173, 0x173, 1, do_option_shortcut);
    _input_addkey(0x172, 0x172, 1, do_option_shortcut);
    _input_addkey(0x16D, 0x16D, 1, do_option_shortcut);
    _input_addkey(0x166, 0x166, 1, do_option_shortcut);
    _input_addkey(0x164, 0x164, 1, do_option_shortcut);
    _input_addkey(0x171, 0x171, 1, do_option_shortcut);
    _input_addkey(0x85, 5, 1, deal_with_icons);
    _input_addkey(0x83, 4, 1, deal_with_icons);
    _input_addkey(0x82, 3, 1, deal_with_icons);
    _input_addkey(0x84, 2, 1, deal_with_icons);
    _input_addkey(0x80, 1, 1, deal_with_icons);
    _input_addkey(0x81, 0, 1, deal_with_icons);
    _input_addkey('p', 9, 1, player_attack);
    _input_addkey('.', 3, 1, player_attack);
    _input_addkey(';', 6, 1, player_attack);
    _input_addkey(0x4A3, 0x4A3, 7, keyboard_mouse);
    _input_addkey(9, 9, 7, keyboard_mouse);
    _input_addkey(0x8D, 0x8D, 7, keyboard_mouse);
    _input_addkey(0x93, 0x93, 7, keyboard_mouse);
    _input_addkey(0x8F, 0x8F, 7, keyboard_mouse);
    _input_addkey(0x91, 0x91, 7, keyboard_mouse);
    _input_addkey(0x8C, 0x8C, 7, keyboard_mouse);
    _input_addkey(0x8E, 0x8E, 7, keyboard_mouse);
    _input_addkey(0x92, 0x92, 7, keyboard_mouse);
    _input_addkey(0x94, 0x94, 7, keyboard_mouse);
    _input_addkey(0x95, 0x95, 7, keyboard_mouse);
    _input_addkey(0x96, 0x96, 7, keyboard_mouse);
    _input_addkey(0x1B, 4, 4, do_escape_key);
    _input_addkey('1', 1, 4, conv_play_menu);
    _input_addkey('2', 2, 4, conv_play_menu);
    _input_addkey('3', 3, 4, conv_play_menu);
    _input_addkey('4', 4, 4, conv_play_menu);
    _input_addkey('5', 5, 4, conv_play_menu);
    input_addmouse(0x46, 0x87, 0x74, 0xBC, 4, 4, npc_barter);
    input_addmouse(0x77, 0x87, 0xA3, 0xBC, 4, 4, play_barter);
    input_addmouse(0x10, 1, 0xDF, 0x1E, 0, 4, conv_play_menu);
    _input_addkey(0x268, (int)&mouse_hand, 0x1B, flip_bool);
    _input_addkey(0x286, 0, 0x1B, show_version);
    _input_addkey(0x287, 0, 0x1B, report_loc);
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
    PMsHndle = input_addmouse(left, bot, left + wid - 1, bot + hgt - 1, 0, 0x1B, mous_in_3d);
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

/* Prints the level and the player's tile, each as two octal digits. FM Towns has a
   sprintf version at the same place. */
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
    game_sprint(0x123);
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
    } else if (index < 0x100) {
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
    int dx, dy;
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
        if (curelem == 0 || (i = Obj_MemTPtr(curelem)) == 0 || i >= 0x100 || i <= 1)
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

    if (player->skills[0x0B] == 0x2D)
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

/* Spins the view into the moongate at tile (32, 32). Not in the FM Towns build; the name is
   provisional (IDA's LaunchPlayerAtMoongate_ovr143_E09), chosen so that its tools/bssorder.py
   key puts it in the EXE's overlay stub order. */
void far Vortex_ovr143_E09(void)
{
    long xd, yd;
    int x, y;

    vort_x = vort_y = 0x20;
    attach_eye(3);
    xd = (vort_x << 8) - PN.x;
    yd = (vort_y << 8) - PN.y;
    vort_rad = cSqRt(xd * xd + yd * yd) >> 6;
    x = (xd << 15) / ((long)vort_rad << 6);
    y = (yd << 15) / ((long)vort_rad << 6);
    vort_theta = cAtan2(y, x);
    for (vort_timer = 0; vort_timer < 0x40; vort_timer++) {
        establish_view();
        vort_theta += 0xCCB;
    }
    attach_eye(1);
}

/* Steps a view angle by dir: unbounded when limit is 0, otherwise through mvcheck.
   A dir of 0 recentres it. */
void far chg_plys(int *val, int dir, int limit)
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

/* Rolls the view. Not in the FM Towns build; the name is provisional (IDA's
   ChangeCameraRoll_ovr143_F58), chosen so that its key puts it in the EXE's stub order. */
void far RollView_ovr143_F58(int step)
{
    if (MoveCamera)
        chg_plys(&camang[2], step, 0);
    else
        chg_plys(&PlayerBank, step, 0);
}

void far chg_plyp(int step)
{
    if (MoveCamera)
        chg_plys(&camang[1], step, 0x1000);
    else
        chg_plys(&PlayerPitch, step, 0x1000);
}
