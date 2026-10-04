/* target: seg024_24DC */
/* opts: -mm -1 -G -O -Y -d */
/* The player's interaction with the 3D view and the panels, and the main game screen's
   set-up and status clicks. The whole of UW1's DOS resident segment seg024_24DC, in
   original order: what UW2 splits into GAMESCR.C (its overlay ovr137: pull_chain to
   clear_gamedisp, check_save, check_rest) and INTERACT.C (seg026_2716: display_scr to
   toggle_fightmode) is one file in UW1, GAMESCR's two guards at its end.

   What it does in the game: the left-hand icons choose what a click in the 3D view does
   (RightButtonThing: 0 the default, look and drag to get; 1 use, 2 fight, 3 look, 4 get, 5
   talk; deal_with_icons, which draws them itself in UW1, new_IconSelect and
   new_IconUnselect). mous_in_3d is the 3D view's mouse handler: it finds the object under
   the pointer from the renderer's pick buffer (pick_3d) and dispatches through
   player_disp. GameInputMode says whether something else owns the pointer: 1 an object
   on the cursor, 2 a targeted spell or an object being used on another (ObjectActor), 3 a
   missile spell being aimed. mous_in_panel does the same for the right-hand panel.
   display_scr runs every frame: the flasks and compass, the damage flash, colour cycling,
   death, the timed effects every 20 seconds of game time, and level 9's random effects.
   init_gamedisp sets up the game screen and its five mouse areas (start_gameinp: the
   icons, the rune shelf, the active spells, the compass and the flasks).

   UW1's differences from UW2: no PoisonWeap, Valor, quick_time or current_button; the
   game time comes from Time (GAME_TIME), not the player record; no check_around (a click
   on a wall or floor only notes the texture); no swimming reach, book theft, moonstone
   table, pressure plates or wisp; the player record differs (Player1Scr); the terrain
   compared whole (TERRAIN); string numbers and the icon and flask areas are UW1's own.

   Data owned: the icon places, the spell effect flags (TimeStop, Hasted, WizEye,
   Blessed), PickDist, the duration check timer, player_disp, the interaction state and
   the five mouse-area handles.
   name: descriptive. UW1 has no symbol-bearing build: function and global names are
   UW2's (FM Towns), the routines being the same; seg024_24DC_D0A is the listing's. */

#include <stdlib.h>
#define UsingPole UW2_UsingPole         /* UW1: char, below */
#define IsMobElem UW2_IsMobElem
#define EncumCheck UW2_EncumCheck
#include "combat.h"
#include "conv.h"
#include "critter.h"
#include "event.h"
#include "gfx.h"
#include "inv.h"
#include "map.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"
#include "view3d.h"
#undef UsingPole
#undef IsMobElem
#undef EncumCheck

/* UW1: signed where UW2's headers have unsigned char (cbw). */
extern char UsingPole;
char far IsMobElem(struct Object far *obj);
char far EncumCheck(struct Object far *obj);

/* UW1: the player record differs from UW2's struct Player (player.h). The fields this
   file uses, from the bytes. */
struct Player1Scr {
    char pad00[0x21];
    unsigned char skills[20];           /* 0x21 */
    char pad35[0x37 - 0x35];
    unsigned char play_mana;            /* 0x37 */
    unsigned char max_mana;             /* 0x38 */
    unsigned char hunger;               /* 0x39 */
    unsigned char fatigue;              /* 0x3A */
    char pad3B[0x5E - 0x3B];
    unsigned char moonstone:4;          /* 0x5E */
    unsigned char b5E_4:4;
    uint16 b5F_0:1;                     /* word 0x5F */
    uint16 drawn:1;
    uint16 poison:4;
    char pad60[0xB8 - 0x60];             /* (the bit run takes one byte) */
    unsigned char motion_state;         /* 0xB8 */
    char padB9[0xCE - 0xB9];
    uint32 game_clock;                  /* 0xCE */
};
#define PLAYER1 ((struct Player1Scr *)player)

/* UW1: declarations this file needs that the headers lack or give otherwise. */
void far ovr130_0(int n);
void far ovr130_6D6(int x, int y);
void far seg014_1DC5_15C5(void);            /* the level's music again */
extern int16 floor_terrainrelated_dseg_5c99_717C[];
void far stop_music(void);
void far seg027_2861_EF9(void);
void far EtherealVoidSpecialEffects_seg008_150(void);
extern unsigned char dseg_5c99_5626;
extern unsigned char realDScheck;           /* GAMEWRAP.C's, DS:12B3 */

#define TERRAIN(t)      (TxmTerr[(t)->floor])      /* UW1: the whole word */

/* This file's data, DS:282..2B9 (the previous file's ends at DS:281). */
/* The interaction icons' places (new_IconSelect), DS:026A and 0276. */
static int16 icon_x[6] = { 8, 8, 6, 6, 7, 8 };
static int16 icon_y[6] = { 0x64, 0x77, 0x86, 0x98, 0xAC, 0xBD };
unsigned char TimeStop = 0;             /* DS:0282 */
unsigned char Hasted = 0;
unsigned char WizEye = 0;
unsigned char Blessed = 0;              /* DS:0285 */
int16 PickDist = 0x90;                  /* DS:0286 */
unsigned char def_mode = 0;             /* DS:0288 */
int32 lastDurCheck = 0;                 /* DS:0289 */
unsigned char DurCount = 0;             /* DS:028D */
char releaseable = 0;                   /* DS:028E, UW1: char (cbw) */


/* This file's _BSS, DS:2682..26AD (COMBAT.C's ends at 2681). */
/* match: laid out by name (tools/bssorder.py): the keys run pTxtId 56, ObjectActor 135,
   ObjectActing 191, RightButtonThing 194, inforMshandle 265, ObjectActorArg 351,
   actspMshandle 513, iconsMshandle 521, PickMap 536, MapObj_X and MapObj_Y 581, newPlObj
   638, flaskMshandle 774, spellMshandle 787, CrownTmap 907, releasePtr 914, GameInputMode
   935. iconsMshandle (DS:2694, the interaction icons' area, new in UW1) is named for its
   key; CrownTmap is UW2's name for the unreferenced byte at DS:26A6. */
int16 pTxtId;
ActorFn ObjectActor;
struct Object far *ObjectActing;
int16 RightButtonThing;
int16 inforMshandle;
int16 ObjectActorArg;
int16 actspMshandle;
int16 iconsMshandle;
struct Tile far *PickMap;
int16 MapObj_X, MapObj_Y;
struct Object far *newPlObj;
int16 flaskMshandle;
int16 spellMshandle;
unsigned char CrownTmap;
union Link far *releasePtr;
int16 GameInputMode;


/* Flips the right-hand panel: from the inventory (RightPanel 0) to the statistics page
   (2), from any other panel back to the inventory, and nothing while a flip is under way
   (4). PANELS.C's adjust_panel animates the flip. The argument is not read. */
void far pull_chain(int how)
{
    if (RightPanel == 0)
        set_screen_frame(6, 2);
    else if (RightPanel != 4)
        set_screen_frame(6, 0);
}

/* A click on the compass: prints how hungry and how tired the player is, the level
   (string 0x19A + PlayerLevel), the day (game_clock / 0x1C2000 / 12, past 100 a fixed
   string) and the time of day (the remainder, twelve periods). */
void far print_info(void)
{
    register int n;
    register int day;

    scroll_print("\n");
    /* "You are currently " ... */
    game_strings_3(0x40, PLAYER1->hunger / 0x1E + 0x68, 0x67);
    n = PLAYER1->fatigue / 0x17;
    if (n > 5)
        n = 5;
    game_sprint(0x76 - n);
    scroll_print(".\n");
    game_strings_3(0x41, PlayerLevel + 0x19A, 0x42);
    n = PLAYER1->game_clock / 0x1C2000L;
    day = n / 12;
    n = n % 12;
    if (day > 100)
        game_sprint(0x45);
    else
        game_strings_3(0x43, day + 0x19B, 0x44);
    game_strings_3(0x46, n + 0x47, 0x53);
    mouse_release(1);
}

/* A click on the flasks: on the chain between them (x 0x1A to 0x27, below y 0xD) pulls
   the chain, which flips the right-hand panel; on the left flask prints the vitality as
   "n out of max", preceded by the poison level if poisoned; on the right flask the
   mana. */
void far flask_info(void)
{
    char cur[10];
    char max[10];
    char msg[120];

    if (inplist->x > 0x19 && inplist->x < 0x28) {
        if (inplist->y > 0xD)
            pull_chain(0);
    } else {
        if (inplist->y > 0x1E)
            return;
        str_copy(msg, get_string(((inplist->x > 0x1E) + 0x59) | STR_GAME));
        if (inplist->x < 0x1E) {
            itoa(ThePlayer->hp, cur, 10);
            itoa(playerdat->avghit, max, 10);
            if (PLAYER1->poison)
                game_strings_3(0x5B, (PLAYER1->poison - 1) / 3 + 0x54, 0x5C);
        } else {
            itoa(PLAYER1->play_mana, cur, 10);
            itoa(PLAYER1->max_mana, max, 10);
        }
        str_cat(msg, cur);
        str_cat(msg, " out of ");
        str_cat(msg, max);
        str_cat(msg, "\n");
        scroll_print(msg);
        mouse_release(1);
    }
}

void far start_gameinp(void)
{
    LeftPanel = 0;
    iconsMshandle = input_addmouse(8, 0x54, 0x20, 0xCE, (NEARPTR)-1, 1, (InputFn)deal_with_icons);
    spellMshandle = input_addmouse(0xB0, 0x2D, 0xDE, 0x3D, 0, 1, (InputFn)try_cast);
    actspMshandle = input_addmouse(0x34, 0x2F, 0x66, 0x3F, 0, 1, (InputFn)try_clear);
    inforMshandle = input_addmouse(0x7A, 0x31, 0x98, 0x40, 0, 1, (InputFn)print_info);
    flaskMshandle = input_addmouse(0xF4, 0x2C, 0x135, 0x50, 0, 1, (InputFn)flask_info);
}

void far clear_gameinp(void)
{
    input_del(iconsMshandle);
    input_del(spellMshandle);
    input_del(actspMshandle);
    input_del(flaskMshandle);
    input_del(inforMshandle);
}

void far init_gamedisp(void)
{
    BeginInventory();
    init_scroll();
    play_music();
    start_gameinp();
    if (LeftPanel == 0) {
        if (RightButtonThing != 0)
            new_IconSelect(RightButtonThing);
    } else
        ovr130_0(1);
    display_scr();
    init_scrgr();
}

void far clear_gamedisp(void)
{
    clear_gameinp();
    EndInventory();
    stop_music();
}

/* Called every frame: drives the player's attack, redraws the health and mana flasks and
   the compass (not on level 9), flashes the screen frame when the player took enough
   damage (dseg_5c99_5626 against four times OBJ_DAMAGE), cycles the palette, starts death
   when the player's hit points are 0, and once per second of game time maybe changes the
   music; every 20 seconds it runs duration_check. On level 9 a random one in 32 frames
   gets EtherealVoidSpecialEffects. */
void far display_scr(void)
{
    int v;
    register int dam;

    player_attack(0);
    v = ThePlayer->hp;
    set_screen_frame(0, v);
    dam = OBJ_DAMAGE(ThePlayer);
    if ((dam << 2) > dseg_5c99_5626 || v < 0x10 && dam > 0)
        set_screen_frame(4, 3);
    ThePlayer->b11 = 0;
    v = PLAYER1->play_mana;
    set_screen_frame(1, v);
    if (PlayerLevel != 9) {
        v = (OBJ_HEADING(ThePlayer) << 5) + OBJ_FINEHEAD(ThePlayer);
        v = (v + 8 & 0xFF) >> 4;
        set_screen_frame(2, v);
    }
    cycle_colors(GAME_TIME() & 0xFF);
    if (ThePlayer->hp == 0)
        player_is_dead();
    v = (GAME_TIME() >> 8) - lastDurCheck;
    if (v != 0) {
        lastDurCheck = GAME_TIME() >> 8;
        DurCount += v;
        if (realDScheck) {
            seg027_2861_EF9();
            realDScheck = 0;
        }
        change_music_maybe();
        if (DurCount > 0x14) {
            DurCount = 0;
            duration_check();
        }
    }
    if (PlayerLevel == 9 && (rand() & 0x1F) == 0)
        EtherealVoidSpecialEffects_seg008_150();
}

void far RedispInv(void)
{
    if (RightPanel == 0 || inplist->mode == 4) {
        load_inventory_pix();
        DisplayInvSpecial();
        DisplayInventory();
    }
}

/* Whether obj on tile is within reach: the squared fine distance at most dist, and the
   height difference within 12 below or 24 above (doubled with a pole; a swimming player
   reaches lower). dist 0 means any distance. Sets MapObj_X, MapObj_Y to the tile. */
unsigned char far InPickRange(int dist, struct Object far *obj, struct Tile far *tile)
{
    int px, py, dz;
    int x, y;

    MapObj_X = (tile - mlowptr) & MAP_MASK;
    MapObj_Y = (int)(tile - mlowptr) >> 6;
    if (dist != 0) {
        x = (MapObj_X << 3) + OBJ_FINEX(obj);
        y = (MapObj_Y << 3) + OBJ_FINEY(obj);
        px = OBJ_HOMEX(ThePlayer);
        py = OBJ_HOMEY(ThePlayer);
        x = x - ((px << 3) + OBJ_FINEX(ThePlayer));
        y = y - ((py << 3) + OBJ_FINEY(ThePlayer));
        x = abs(x);
        y = abs(y);
        if (x * x + y * y > dist)
            return 0;
        dz = OBJ_Z(ThePlayer) - OBJ_Z(obj);
        if ((UsingPole + 1) * 12 < dz || (UsingPole + 1) * -24 > dz)
            return 0;
    }
    return 1;
}

/* The height of the highest bridge on tile, or -1. */
int far BridgeHeight(struct Tile far *tile)
{
    struct Object far *obj;
    int h = -1;

    for (obj = Obj_PtrTMem(&tile->objects); obj; obj = Obj_PtrTMem(&obj->qn.link))
        if (OBJ_ITEM(obj) == ITEM_BRIDGE && (int)OBJ_Z(obj) > h)
            h = OBJ_Z(obj);
    return h;
}

/* Whether the terrain stops the player reaching obj: it is too high or low for him, or a
   tile on the straight line to it rises above him (for the normal reach), or the line
   crosses another terrain class (water, lava, ice) than the one he stands on, unless a
   bridge carries it. dist 0 means never blocked. */
char far BlockingTerrain(int dist, struct Object far *obj)
{
    int px, py, xdir, ydir, pz, oz, terr, bridge;
    struct Tile far *tile;
    char found;                         /* UW1: char (cbw) */
    int reach, high;
    int x, y;

    if (dist != 0) {
        px = OBJ_HOMEX(ThePlayer);
        py = OBJ_HOMEY(ThePlayer);
        tile = Map_GetAddr(px, py);
        terr = TERRAIN(tile);
        if (MapObj_X != px || MapObj_Y != py) {
            if (MapObj_X < px)
                xdir = -1;
            else if (MapObj_X > px)
                xdir = 1;
            else
                xdir = 0;
            if (MapObj_Y < py)
                ydir = -1;
            else if (MapObj_Y > py)
                ydir = 1;
            else
                ydir = 0;
            reach = (UsingPole + 1) << 3;
            high = (UsingPole + 1) << 5;
            if ((pz = OBJ_Z(ThePlayer)) > reach)
                pz -= reach;
            else
                pz = 0;
            if ((oz = OBJ_Z(obj)) < pz || pz + high < oz)
                return 1;
            y = py;
            x = px;
            for (;;) {
                x += xdir;
                if (xdir < 0 && x < MapObj_X || xdir > 0 && x > MapObj_X)
                    x = MapObj_X;
                y += ydir;
                if (ydir < 0 && y < MapObj_Y || ydir > 0 && y > MapObj_Y)
                    y = MapObj_Y;
                tile = Map_GetAddr(x, y);
                bridge = BridgeHeight(tile);
                found = 0;
                if (bridge >= 0 && bridge <= oz && bridge >= pz)
                    found = 1;
                if (!found && (pz >> 3) > tile->height && dist == 0x90)
                    return 1;
                if (x == MapObj_X && y == MapObj_Y)
                    break;
                if (dist <= 0x90 && !found && TERRAIN(tile) != 0 && TERRAIN(tile) != terr)
                    return 1;
            }
        }
    }
    return 0;
}

/* The object drawn at the pointer in the 3D view, or 0: the renderer's pick buffer gives
   each object a colour (1..PickUp-1, mapped to the object by color_to_obj and its tile by
   color_to_map); a click on a wall or floor (colours 0xAC..0xFC) looks around for a nearby
   object, else leaves the texture in pTxtId for look_nothing. Sets PickMap, releasePtr and
   whether the object could be picked up (releaseable). */
struct Object far * far pick_3d(int how)
{
    unsigned char far *p;
    struct Object far *obj;
    register int idx;
    register struct ComObj *co;

    do_3d_grab();
    p = stdat;
    p += inplist->y * (xwid + 2) + inplist->x + 2;
    idx = 0;
    pTxtId = 0;
    if (*p >= 1 && *p < *(int16 *)&PickUp) {
        idx = color_to_obj[*p - 1];
        PickMap = mlowptr + color_to_map[*p - 1];
    } else if (*p >= 0xC0 && *p <= 0xFA)
        pTxtId = *p - 0xBF;
    if (idx == 0)
        return 0;
    obj = Obj_IntTMem(idx);
    co = &ComObjData[OBJ_ITEM(obj)];
    releasePtr = &PickMap->objects;
    releaseable = co->pickable && !IsMobElem(obj);
    return obj;
}

/* No object under the pointer: describes the texture clicked when looking ('You see' and
   the texture's description from block 10: a wall texture (TxmID), one of the ten
   others (floor_terrainrelated_dseg_5c99_717C), or nothing), else prints string 0x98 +
   how. */
void far look_nothing(unsigned char how, register int txt)
{
    register int t;

    if (txt > 0 && how == 2) {
        txt--;
        if (txt < 0x30)
            t = TxmID[txt];
        else if (txt < 0x3A)
            t = 0x1FE - floor_terrainrelated_dseg_5c99_717C[txt - 0x30];
        else
            t = 0x1FF;
        scroll_print("You see ");
        scroll_print(get_string(t | STR_TEXTURES));
        scroll_print(".\n");
    } else
        game_sprint(how + 0x98);
}

/* Takes a picked-up object off its tile, firing its pick-up triggers (mode 2). */
void far release_3d(struct Object far *obj)
{
    if (releaseable) {
        checkTrap(ThePlayer, obj, 2, MapObj_X, MapObj_Y);
        Obj_Rem(releasePtr, obj);
        editchng(2);
        releaseable = 0;
    }
}

/* Picks up newPlObj into the cursor if it can be taken, is in reach and not blocked
   (a stack asks how many; too heavy: string 0x5F). Taking a moonstone clears
   the player's moonstone field; owners nearby notice (player_grabbed). Otherwise it
   explains why, or in the default mode uses or talks instead. */
void far player_3dget(void)
{
    char inrange, clear;
    struct Object far *split;

    split = 0;
    inrange = InPickRange(PickDist, newPlObj, PickMap);
    clear = !BlockingTerrain(PickDist, newPlObj);
    if (releaseable && inrange && clear) {
        if (OBJ_ISQUANT(newPlObj) && !(OBJ_LINK(newPlObj) & LINK_SPECIAL) && OBJ_LINK(newPlObj) != 1) {
            if ((split = AskHowMany(newPlObj)) == 0)
                return;
            if (split != newPlObj)
                Obj_Add(&newPlObj->qn.link, split);
        }
        if (!EncumCheck(newPlObj)) {
            if (split && split != newPlObj) {
                newPlObj->ol.f.link += split->ol.f.link;
                Obj_Rem(&newPlObj->qn.link, split);
            }
            game_sprint(0x5F);
        } else {
            if (HasOrIsObj(newPlObj, ITEM_MOONSTONE))
                PLAYER1->moonstone = 0;
            player_grabbed(newPlObj, 0);
            release_3d(newPlObj);
            GameInputMode = 1;
            DoInventoryDrag(newPlObj);
        }
    } else if (releaseable) {
        game_sprint(inrange + 0x5D);
        mouse_release(1);
    } else if (def_mode) {
        if (IsMobElem(newPlObj) && OBJ_MAJOR(newPlObj) == MAJOR_CREATURE)
            player_3dtalk();
        else
            player_3duse();
    } else {
        if (OBJ_ITEM(newPlObj) == 0x1CA) {
            if (inrange && clear)
                UseObj(ThePlayer, newPlObj, 0);
        } else
            game_sprint(0x60);
        mouse_release(1);
    }
}

void far player_3dtalk(void)
{
    mouse_release(1);
    TalkTo(newPlObj);
}

/* UW1: LookAt (ovr122_0) through a resident call. */
void far seg024_24DC_D0A(struct Object far *obj, int how)
{
    LookAt(obj, how);
}

/* Looks at newPlObj, with detail only in close reach. In look mode a found trap may be
   disarmed (asks first, then RemoveTrap with the Traps skill); otherwise dragging after
   the look picks the object up. Fires the object's look triggers (mode 5). */
void far player_3dlook(void)
{
    char yes;
    register int r;

    if (InPickRange(0x48, newPlObj, PickMap) && !releaseable)
        seg024_24DC_D0A(newPlObj, 1);
    else
        seg024_24DC_D0A(newPlObj, 0);
    if (RightButtonThing == 3) {
        if (DetectedTrap(newPlObj, PLAYER1->skills[SKILL_SEARCH]) > 0) {
            yes = 1;
            r = wyorn(0, 0xF4, &yes);
            if (r != 0 && r < 4)
                wd_bool(yes = r == 2);
            scroll_print("\n");
            if (yes)
                RemoveTrap(newPlObj, PLAYER1->skills[SKILL_TRAPS]);
        }
    } else
        def_mode = 1;
    checkTrap(ThePlayer, newPlObj, 5, MapObj_X, MapObj_Y);
    if (RightButtonThing != 3) {
        if (mouse_dragged(1))
            player_3dget();
    } else
        mouse_release(1);
    def_mode = 0;
}

/* Uses newPlObj if it is in reach, else 'You are unable to use that from here.' (not for
   the texture map objects). */
void far player_3duse(void)
{
    mouse_release(1);
    if (InPickRange(PickDist, newPlObj, PickMap) && !BlockingTerrain(PickDist, newPlObj))
        UseObj(ThePlayer, newPlObj, 0);
    else if ((newPlObj->id & 0x1FE) != 0x16E)
        game_sprint(0xB9);  /* 'You are unable to use that from here.' */
}

/* Fight mode click: the swing is the ninth of the view clicked (1..9, left to right
   and top to bottom). */
/* name: FM Towns calls it player_3dattack; the DOS name is not known otherwise. */
void far player_3dattack(void)
{
    int n;

    n = inplist->x * 3 / (PWid + 2) + inplist->y * 3 / (PHgt + 2) * 3;
    player_attack(n + 1);
}

void (far *player_disp[])(void) = {
    player_3duse, player_3dattack, player_3dlook, player_3dget, player_3dtalk
};

/* The 3D view's mouse handler (see the file comment). A paralysed player can do nothing;
   the left button moves him (player_mous_move). */
void far mous_in_3d(void)
{
    unsigned char how;

    if (inplist->cmd & 1)
        player_mous_move();
    if (inplist->mode == 1) {
        newPlObj = 0;
        if (inplist->cmd & 2) {
            switch (GameInputMode) {
            case 0:
                if (RightButtonThing == 0)
                    how = 2;
                else
                    how = RightButtonThing - 1;
                if (how != 1) {
                    if (inplist->cmd & 1)
                        break;
                    newPlObj = pick_3d(2);
                    if (newPlObj == 0) {
                        look_nothing(how, pTxtId);
                        mouse_release(1);
                        break;
                    }
                }
                (*player_disp[how])();
                break;
            case 1:
                DoSpecialActions(0x17);
                mouse_release(1);
                break;
            case 2:
                if ((newPlObj = pick_3d(2)) != 0) {
                    if (InPickRange(PickDist, newPlObj, PickMap) && !BlockingTerrain(PickDist, newPlObj))
                        (*ObjectActor)(newPlObj, 1, 0);
                    else
                        game_sprint(0x5E);
                }
                if (CursorObjPtr) {
                    unforce_mouse_cursor(3);
                    CursorObjPtr = 0;
                    GameInputMode = 0;
                }
                mouse_release(1);
                break;
            case 3:
                BlastFunction();
                break;
            }
        }
    } else if (inplist->mode == 0x10 && inplist->cmd == 2) {
        if ((newPlObj = pick_3d(2)) != 0)
            player_3duse();
    }
}

/* Looks at an object in the inventory, identifying it with the Lore skill: the first
   look rolls skill_check(Lore, 8) + 1 (at least 1) and keeps the best result in the
   object's heading (bit 2 marks it examined, bits 0..1 the lore level), so it is not
   rerolled until a better Lore clears the marks (WORLDEV.C's clear_all_loretries). */
void far inv_look(void)
{
    unsigned char ident;
    register int lore;
    register int head;

    checkTrap(ThePlayer, newPlObj, 5, MapObj_X, MapObj_Y);
    if (newPlObj == 0)
        newPlObj = pick_inv(2);
    ident = OBJ_MAJOR(FARNULLTRAP(newPlObj)) != MAJOR_RECT && OBJ_MAJOR(FARNULLTRAP(newPlObj)) != MAJOR_TRAP
        && ComObjData[OBJ_ITEM(FARNULLTRAP(newPlObj))].render != 2;
    if (ident == 1) {
        head = OBJ_HEADING(FARNULLTRAP(newPlObj));
        if (head & 4)
            lore = head & 3;
        else {
            lore = skill_check(PLAYER1->skills[SKILL_LORE], 0xA) + 1;
            if (lore == 0)
                lore = 1;
            if ((head & 3) > lore)
                lore = head & 3;
            SET_HEADING(FARNULLTRAP(newPlObj), lore | 4);
        }
    } else
        lore = 1;
    seg024_24DC_D0A(newPlObj, lore);
    DoInventoryMouse(-1);
}

/* name: not in the FM Towns build and never called in DOS. */
static void far seg026_2716_F8A(void)
{
    DoInventoryMouse(1);
}

/* Use the object under the cursor. */
/* name: not in the FM Towns build and never called in DOS. */
void far seg026_2716_F98(void)
{
    if (newPlObj == 0)
        newPlObj = pick_inv(2);
    mouse_release(1);
    UseObj(ThePlayer, newPlObj, 1);
}

/* Inventory panel clicks by GameInputMode: 0 pick up or put down (looking in look mode or
   on the right button), 1 put down the cursor object, 2 use the cursor object or spell on
   the object clicked (ObjectActor). */
void far mous_in_inv(void)
{
    int x;
    register int y;
    register int hit;

    newPlObj = 0;
    switch (GameInputMode) {
    case 0:
        if (CursorObjPtr)
            DoInventoryMouse(0);
        else
            switch (inplist->cmd) {
            case 1:
            case 3:
                DoInventoryMouse(0);
                break;
            case 2:
                if (RightButtonThing == 1 && inplist->mode != 4)
                    DoInventoryMouse(0);
                else
                    DoInventoryMouse(-2);
                break;
            }
        break;
    case 1:
        DoInventoryMouse(4);
        break;
    case 2:
        x = inplist->x + 0xF0;
        y = inplist->y + 0x52;
        hit = FindInventoryHit(x, y);
        if (hit == 0x15 || hit == 0x16)
            DoInventoryMouse(1);
        else if ((newPlObj = pick_inv(2)) != 0) {
            (*ObjectActor)(newPlObj, 1, 1);
            mouse_release(1);
        } else {
            unforce_mouse_cursor(3);
            CursorObjPtr = 0;
            GameInputMode = 0;
        }
        break;
    }
}

/* The right panel's mouse handler: inventory, rune bag or statistics by RightPanel. */
void far mous_in_panel(void)
{
    switch (RightPanel) {
    case 0:
        mous_in_inv();
        break;
    case 1:
        mous_in_rune();
        break;
    case 2:
        mous_in_stat();
        break;
    }
}

/* UW1: draws the selected picture (0x200B less twice the index) of interaction icon
   mode (1 to 5) at its place in icon_x, icon_y. */
void far new_IconSelect(register int mode)
{
    int x;
    register int y;

    mode--;
    x = icon_x[mode];
    y = icon_y[mode];
    mouse_hide();
    pic_to_screen(0x200B - mode * 2, x, y, 1, 1);
    mouse_show();
}

/* UW1: draws the unselected picture (0x200A less twice the index). */
void far new_IconUnselect(register int mode)
{
    int x;
    register int y;

    mode--;
    x = icon_x[mode];
    y = icon_y[mode];
    mouse_hide();
    pic_to_screen(0x200A - mode * 2, x, y, 1, 1);
    mouse_show();
}

/* A click on one of the interaction icons (mode -1: find which from the pointer). Mode 5
   is the options screen. Otherwise it sheathes the weapon, then selects the mode or, if it
   was already selected, returns to the default. Selecting fight draws the weapon (not
   while asleep in the void, paralysed or with motion_state bit 0 set) and
   starts the combat music; use, look and get change the pointer. */
void far deal_with_icons(register int mode)
{
    if (GameInputMode != 0)
        return;
    if (mode == -1) {
        if (LeftPanel != 0)
            ovr130_6D6(inplist->x, inplist->y);
        else if ((mode = (inplist->y + 2) / 0x12) > 5)
            return;
    }
    if (mode == 5)
        ovr130_0(1);
    else {
        set_screen_frame(8, 6);
        PLAYER1->drawn = 0;
        if (RightButtonThing == 1 || RightButtonThing == 3 || RightButtonThing == 4)
            unforce_mouse_cursor(3);
        if (++mode == RightButtonThing) {
            new_IconUnselect(RightButtonThing);
            RightButtonThing = 0;
        } else {
            if (RightButtonThing != 0)
                new_IconUnselect(RightButtonThing);
            RightButtonThing = mode;
            if (RightButtonThing == 2) {
                if ((PLAYER1->motion_state & 1) == 0) {
                    PLAYER1->drawn = 1;
                    set_screen_frame(8, 4);
                    new_IconSelect(RightButtonThing);
                    if (get_current_music() < 5 || get_current_music() > 7)
                        set_new_music(8);
                } else
                    RightButtonThing = 0;
            } else
                new_IconSelect(RightButtonThing);
        }
        if (!PLAYER1->drawn && get_current_music() == 8)
            seg014_1DC5_15C5();
        mouse_release(1);
        if (RightButtonThing == 1 || RightButtonThing == 3 || RightButtonThing == 4)
            force_mouse_cursor(0x1077);
    }
}

/* Draws the weapon: fight mode, the combat music, the weapon picture. */
void far pick_fightmode(void)
{
    if (PLAYER1->drawn == 1)
        return;
    if (PLAYER1->motion_state & 1)
        return;
    if (RightButtonThing == 1 || RightButtonThing == 3 || RightButtonThing == 4)
        unforce_mouse_cursor(3);
    if (RightButtonThing != 0)
        new_IconUnselect(RightButtonThing);
    RightButtonThing = 2;
    PLAYER1->drawn = 1;
    set_screen_frame(8, 4);
    new_IconSelect(RightButtonThing);
    if (get_current_music() < 5 || get_current_music() > 7)
        set_new_music(8);
}

/* Sheathes the weapon and leaves fight mode, back to the level's music. */
void far punt_fightmode(void)
{
    if (PLAYER1->drawn) {
        set_screen_frame(8, 6);
        PLAYER1->drawn = 0;
        if (LeftPanel == 0)
            new_IconUnselect(2);
        RightButtonThing = 0;
        clear_fight_state();
        seg014_1DC5_15C5();
    }
}

/* Draws or sheathes the weapon (from a click on the weapon hand). */
void far toggle_fightmode(void)
{
    if (PLAYER1->drawn)
        punt_fightmode();
    else
        pick_fightmode();
}

/* Saving is refused while an action is in progress (GameInputMode, string 0xA0) and on
   level 9 (0x9F). Returns 1 if the save may go ahead. */
/* name: UW2's check_save (FM Towns), the same guard. */
char far check_save(void)
{
    register int id;

    id = 0;
    if (GameInputMode)
        id = 0xA0;
    if (PlayerLevel == 9)
        id = 0x9F;
    if (id) {
        grSoftPageFlip();
        game_sprint(id);
        grSoftPageFlip();
        return 0;
    }
    return 1;
}

/* Restoring is always allowed: an action in progress is simply abandoned and its cursor
   dropped. */
char far check_rest(void)
{
    if (GameInputMode) {
        GameInputMode = 0;
        unforce_mouse_cursor(0);
    }
    return 1;
}
