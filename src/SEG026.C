/* target: seg026_2716 */
/* opts: -mm -1 -G -O -d */
/* The player's interaction with the 3D view and the panels: the per-tick screen update,
   picking objects out of the view, reach and line-of-sight checks, getting, looking at,
   using, talking to and attacking, the inventory clicks, the interaction-mode icons and
   drawing or sheathing the weapon. The whole of DOS segment seg026_2716, in original
   order. Function and global names are the originals from the FM Towns symbol table
   except where noted; the source file's own name is not known. */

#include <stdlib.h>

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0x21];
    unsigned char skills[20];           /* 0x21: lore 8, traps 10, search 11 */
    char pad1[0x37 - 0x35];
    unsigned char play_mana;            /* 0x37 */
    char pad2[0x5E - 0x38];
    unsigned char moonstones[2];        /* 0x5E, the level each moonstone was dropped on */
    unsigned b60:1;                     /* 0x60, weapon drawn */
    unsigned poison:4;
    unsigned active_spells:4;
    unsigned b60_9:3;
    unsigned shrooms:2;
    unsigned drunk:6;                   /* word 0x61, bits 6..11 */
    unsigned automap:1;                 /* word 0x62, bit 4 */
    unsigned b62_5:1;
    unsigned sleepbits:3;               /* word 0x62, bits 6..8 */
    unsigned in_void:1;
    unsigned in_pits:1;
    unsigned b63_3:5;
    char pad3[0xCE - 0x64];
    unsigned long questsCE;             /* 0xCE */
    char pad4[0x305 - 0xD2];
    unsigned char b305;                 /* 0x305, paralysis time */
    unsigned char motion_state;         /* 0x306, bit 0 swimming */
    unsigned char drowning;             /* 0x307 */
    char pad5[0x369 - 0x308];
    unsigned long game_clock;           /* 0x369 */
};

/* A mobile object. The first 8 bytes are shared with static objects. */
struct Object {
    unsigned id;                        /* item 0-8, bit 13, is_quant 15 */
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
    char pad9[0x11 - 0x09];
    unsigned char b11;                  /* 0x11, damage taken this tick */
    char pad12[0x16 - 0x12];
    unsigned home;                      /* 0x16, x in bits 10-15, y in bits 4-9 */
    unsigned char b18;                  /* 0x18, fine heading in bits 0-4 */
};

#define OBJ_ID(o)       ((o)->id & 0x1FF)
#define OBJ_MAJOR(o)    (((o)->id & 0x1C0) >> 6)
#define OBJ_BIT13(o)    (((o)->id & 0x2000) >> 13)
#define OBJ_ISQUANT(o)  (((o)->id & 0x8000) >> 15)
#define OBJ_Z(o)        ((o)->pos & 0x7F)
#define OBJ_HEADING(o)  (((o)->pos & 0x380) >> 7)
#define OBJ_FINEY(o)    (((o)->pos & 0x1C00) >> 10)
#define OBJ_FINEX(o)    (((o)->pos & 0xE000) >> 13)
#define OBJ_HOMEX(o)    (((o)->home & 0xFC00) >> 10)
#define OBJ_HOMEY(o)    (((o)->home & 0x3F0) >> 4)
#define OBJ_LINK(o)     ((o)->ol.f.link)

#define SET_HEADING(o, v) ((o)->pos = (o)->pos & 0xFC7F | ((v) & 7) << 7)

struct Tile {
    unsigned type:4;
    unsigned height:4;
    unsigned b1:2;
    unsigned floor:4;                   /* bits 10..13 */
    unsigned b1_6:2;
    unsigned objects;                   /* 0x02, head of the tile's object list */
};

#define TERRAIN(t)      ((TxmTerr[(t)->floor] & 0xC0) >> 6)

struct Inplist {
    int x, y;                           /* 0x00 */
    char pad4[6 - 4];
    int buttons;                        /* 0x06 */
    int field8;                         /* 0x08 */
};

/* One object type's common properties, 11 bytes. */
struct ComObj {
    char pad0[6];
    unsigned c6:13;                     /* 0x06 */
    unsigned pickable:1;
    unsigned c6_14:2;
    char pad8;
    unsigned char c9:2;                 /* 0x09 */
    unsigned char c9_2:6;
    char padA;
};

/* This file's data, in DS order from 0x37E. The six spell-effect flags lead it: they lie
   between seg024's data and this file's in link order, and FM Towns keeps them with this
   file's other variables (ObjectActor .. MapObj_X, quick_time, Valor). */
unsigned char PoisonWeap = 0;           /* DS:037E */
unsigned char TimeStop = 0;
unsigned char Hasted = 0;
unsigned char WizEye = 0;
unsigned char Blessed = 0;
unsigned char Valor = 0;                /* DS:0383 */
int LeftPanel = 2;
int PickDist = 0x90;
unsigned char quick_time = 0;
unsigned char def_mode = 0;
long lastDurCheck = 0;
unsigned char DurCount = 0;
unsigned char realDScheck = 1;
unsigned char releaseable = 0;

extern struct Player near *player;
extern struct Object far *ThePlayer;
extern struct Inplist near *inplist;
extern struct ComObj ComObjData[];
extern unsigned long far *Time;
extern int PlayerLevel;
extern unsigned char RightPanel;
extern unsigned char UsingPole;
extern unsigned TxmTerr[];
extern int TxmID[];
extern struct Tile far *mlowptr;
extern int xwid, xhgt;
extern int PickUp;
extern unsigned char far stdat[];
extern struct Object far *CursorObjPtr;
extern unsigned char button_to_mode[];
extern unsigned char mode_to_button[];
extern char gameopts_buttongroup[];
extern int PWid, PHgt;

/* This file's _BSS, DS:24E4..2507, laid out by name (tools/bssorder.py): the keys run
   pTxtId 56, current_button 91, ObjectActor 135, ObjectActing 191, RightButtonThing 194,
   ObjectActorArg 351, PickMap 536, MapObj_X and MapObj_Y 581, newPlObj 638, CrownTmap 907,
   releasePtr 914, GameInputMode 935. CrownTmap is the FM Towns name in the same group with
   a key that fits the unreferenced byte at DS:2500 (one byte in FM Towns too; DS:2501 is
   padding, as Turbo C puts anything wider than a byte at an even offset). */
int pTxtId;
int current_button;
void (far *ObjectActor)(struct Object far *obj, int a, int b);
struct Object far *ObjectActing;
int RightButtonThing;
int ObjectActorArg;
struct Tile far *PickMap;
int MapObj_X, MapObj_Y;
struct Object far *newPlObj;
unsigned char CrownTmap;
unsigned far *releasePtr;
int GameInputMode;

/* No FM Towns names: the pick tables, indexed by the byte under the cursor. */
extern int color_to_obj[];
extern int color_to_map[];

void far player_attack(int swing);
void far set_screen_frame(int frame, int how);
void far cycle_colors(unsigned char t);
void far player_is_dead(void);
void far change_music_maybe(void);
void far duration_check(void);
void far load_inventory_pix(void);
void far DisplayInvSpecial(void);
void far DisplayInventory(void);
struct Object far * far Obj_PtrTMem(unsigned far *link);
struct Object far * far Obj_IntTMem(int index);
struct Tile far * far Map_GetAddr(int x, int y);
void far do_3d_grab(void);
unsigned char far IsMobElem(struct Object far *obj);
void far scroll_print(char far *s);
char far * far get_string(int id);
void far game_sprint(int id);
void far checkTrap(struct Object far *who, struct Object far *obj, int how, int x, int y);
char far Obj_Rem(unsigned far *head, struct Object far *obj);
void far Obj_Add(unsigned far *head, struct Object far *obj);
void far Obj_Free(struct Object far *obj);
void far editchng(int bits);
struct Object far * far AskHowMany(struct Object far *obj);
unsigned char far EncumCheck(struct Object far *obj);
void far player_did_bad(int owner);
char far HasOrIsObj(struct Object far *obj, int id);
void far player_grabbed(struct Object far *obj, int how);
void far check_pplate(struct Object far *obj, struct Tile far *tile, int z, int how);
void far DoInventoryDrag(struct Object far *obj);
void far player_3dtalk(void);
void far player_3duse(void);
void far UseObj(struct Object far *who, struct Object far *obj, int how);
void far mouse_release(int how);
char far mouse_dragged(int how);
void far TalkTo(struct Object far *obj);
void far LookAt(struct Object far *obj, int how);
int far DetectedTrap(struct Object far *obj, int skill);
int far wyorn(int a, int id, char *answer);
void far wd_bool(char yes);
void far RemoveTrap(struct Object far *obj, int skill);
void far player_mous_move(void);
void far DoSpecialActions(int n);
void far unforce_mouse_cursor(int n);
void far force_mouse_cursor(int id);
void far BlastFunction(void);
struct Object far * far pick_inv(int how);
int far skill_check(int value, int target);
void far DoInventoryMouse(int how);
int far FindInventoryHit(int x, int y);
void far mous_in_rune(void);
void far mous_in_stat(void);
int far get_iconreg_button(void);
void far busywaiting_new_options(char *group);
void far clear_fight_state(void);
void far new_IconUnselect(int button);
void far new_IconSelect(int button);
unsigned char far get_current_music(void);
void far set_new_music(int n);
void far set_random_walking_music(int which);

void far display_scr(void)
{
    int v;

    player_attack(0);
    v = ThePlayer->hp;
    set_screen_frame(0, v);
    ThePlayer->b11 = 0;
    v = player->play_mana;
    set_screen_frame(1, v);
    if ((PlayerLevel - 1) / 8 != 8) {
        v = (OBJ_HEADING(ThePlayer) << 5) + (ThePlayer->b18 & 0x1F);
        v = (v + 8 & 0xFF) >> 4;
        set_screen_frame(2, v);
    }
    cycle_colors(*Time & 0xFF);
    if (ThePlayer->hp == 0)
        player_is_dead();
    v = (player->game_clock >> 8) - lastDurCheck;
    if (v != 0 && realDScheck != 0) {
        lastDurCheck = player->game_clock >> 8;
        DurCount += v;
        change_music_maybe();
        if (DurCount > 0x14) {
            DurCount -= 0x14;
            duration_check();
        }
        if (player->b305 != 0)
            player->b305--;
    }
}

void far RedispInv(void)
{
    if (RightPanel == 0 || inplist->field8 == 4) {
        load_inventory_pix();
        DisplayInvSpecial();
        DisplayInventory();
    }
}

unsigned char far InPickRange(int dist, struct Object far *obj, struct Tile far *tile)
{
    int px, py, dz;
    int x, y;

    MapObj_X = (tile - mlowptr) & 0x3F;
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
        if (player->drowning > 0x50)
            dz -= player->drowning >> 3;
        if ((UsingPole + 1) * 12 < dz || (UsingPole + 1) * -24 > dz)
            return 0;
    }
    return 1;
}

int far BridgeHeight(struct Tile far *tile)
{
    struct Object far *obj;
    int h = -1;

    for (obj = Obj_PtrTMem(&tile->objects); obj; obj = Obj_PtrTMem(&obj->qn.word))
        if (OBJ_ID(obj) == 0x164 && (int)OBJ_Z(obj) > h)
            h = OBJ_Z(obj);
    return h;
}

unsigned char far BlockingTerrain(int dist, struct Object far *obj)
{
    int px, py, xdir, ydir, pz, oz, terr, bridge;
    struct Tile far *tile;
    unsigned char found;
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

unsigned char far check_around(unsigned char far *map, int x, int y)
{
    unsigned char c;
    int side, k, r;
    int d[2] = { 0, 1 };

    for (r = 1; r < 10; r++) {
        for (side = 0; side < 1; side++) {
            for (k = 0; k < r; k++) {
                x += d[0];
                y += d[1];
                if (x >= 1 && x <= xwid + 1 && y >= 0 && y <= xhgt) {
                    c = (map + y * (xwid + 2))[x + 2];
                    if (c >= 1 && c < PickUp)
                        return c;
                }
            }
            if (d[0] != 0) {
                d[1] = -d[0];
                d[0] = 0;
            } else {
                d[0] = d[1];
                d[1] = 0;
            }
        }
    }
    return 0;
}

struct Object far * far pick_3d(int how)
{
    unsigned char far *p;
    struct Object far *obj;
    int idx;
    struct ComObj *co;

    do_3d_grab();
    p = stdat;
    p += inplist->y * (xwid + 2) + inplist->x + 2;
    idx = 0;
    pTxtId = 0;
    if (*p >= 1 && *p < PickUp) {
        idx = color_to_obj[*p - 1];
        PickMap = mlowptr + color_to_map[*p - 1];
    } else if (*p >= 0xAC && *p <= 0xFC || *p == 0) {
        if ((idx = check_around(stdat, inplist->x, inplist->y)) == 0)
            pTxtId = *p - 0xAB;
        else {
            PickMap = mlowptr + color_to_map[idx - 1];
            idx = color_to_obj[idx - 1];
        }
    }
    if (idx == 0)
        return 0;
    obj = Obj_IntTMem(idx);
    co = &ComObjData[OBJ_ID(obj)];
    releasePtr = &PickMap->objects;
    releaseable = co->pickable && !IsMobElem(obj);
    return obj;
}

void far look_nothing(unsigned char how, int txt)
{
    int t;

    if (txt > 0 && how == 2) {
        txt--;
        if (txt < 0x40)
            t = TxmID[txt];
        else if (txt < 0x50)
            t = 0x1FE - TxmTerr[txt];
        else
            t = 0x1FF;
        scroll_print("You see ");
        scroll_print(get_string(t | 0x1400));
        game_sprint(0x60);
    } else
        game_sprint(how + 0xA6);
}

void far release_3d(struct Object far *obj)
{
    if (releaseable) {
        checkTrap(ThePlayer, obj, 2, MapObj_X, MapObj_Y);
        Obj_Rem(releasePtr, obj);
        editchng(2);
        releaseable = 0;
    }
}

void far player_3dget(void)
{
    unsigned char inrange, clear;
    struct Object far *split;
    int i;

    split = 0;
    inrange = InPickRange(PickDist, newPlObj, PickMap);
    clear = !BlockingTerrain(PickDist, newPlObj);
    if (releaseable && inrange && clear) {
        if (OBJ_ISQUANT(newPlObj) && !(OBJ_LINK(newPlObj) & 0x200) && OBJ_LINK(newPlObj) != 1) {
            if ((split = AskHowMany(newPlObj)) == 0)
                return;
            if (split != newPlObj)
                Obj_Add(&newPlObj->qn.word, split);
        }
        if (!EncumCheck(newPlObj)) {
            if (split && split != newPlObj) {
                newPlObj->ol.f.link += split->ol.f.link;
                if (Obj_Rem(&newPlObj->qn.word, split))
                    Obj_Free(split);
            }
            game_sprint(0x6C);
        } else {
            if (OBJ_ID(newPlObj) == 0x138 && OBJ_BIT13(newPlObj)) {
                player->questsCE = (player->questsCE & 0xFFFFFFFBL) + 4;
                player_did_bad(0x1C);
            }
            if (HasOrIsObj(newPlObj, 0x126))
                for (i = 0; i < 2; i++)
                    if (player->moonstones[i] == PlayerLevel) {
                        player->moonstones[i] = 0;
                        break;
                    }
            player_grabbed(newPlObj, 0);
            release_3d(newPlObj);
            check_pplate(newPlObj, PickMap, OBJ_Z(newPlObj), 0xF);
            GameInputMode = 1;
            DoInventoryDrag(newPlObj);
        }
    } else if (releaseable) {
        game_sprint(inrange + 0x6A);
        mouse_release(1);
    } else if (def_mode) {
        if (IsMobElem(newPlObj) && OBJ_MAJOR(newPlObj) == 1 || OBJ_ID(newPlObj) == 0x1CD)
            player_3dtalk();
        else
            player_3duse();
    } else {
        if (OBJ_ID(newPlObj) == 0x1CA) {
            if (inrange && clear)
                UseObj(ThePlayer, newPlObj, 0);
        } else
            game_sprint(0x6D);
        mouse_release(1);
    }
}

void far player_3dtalk(void)
{
    mouse_release(1);
    TalkTo(newPlObj);
}

void far player_3dlook(void)
{
    char yes;
    int r;

    if (InPickRange(0x48, newPlObj, PickMap) && !releaseable)
        LookAt(newPlObj, 1);
    else
        LookAt(newPlObj, 0);
    if (RightButtonThing == 3) {
        if (DetectedTrap(newPlObj, player->skills[11]) > 0) {
            yes = 1;
            r = wyorn(0, 0x103, &yes);
            if (r != 0 && r < 4)
                wd_bool(yes = r == 2);
            scroll_print("\n");
            if (yes)
                RemoveTrap(newPlObj, player->skills[10]);
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

void far player_3duse(void)
{
    mouse_release(1);
    if (InPickRange(PickDist, newPlObj, PickMap) && !BlockingTerrain(PickDist, newPlObj))
        UseObj(ThePlayer, newPlObj, 0);
    else if ((newPlObj->id & 0x1FE) != 0x16E)
        game_sprint(0xC8);
}

/* FM Towns calls it player_3dattack; the DOS name is not known otherwise. */
void far player_3dattack(void)
{
    int n;

    n = inplist->x * 3 / (PWid + 2) + inplist->y * 3 / (PHgt + 2) * 3;
    player_attack(n + 1);
}

void (far *player_disp[])(void) = {
    player_3duse, player_3dattack, player_3dlook, player_3dget, player_3dtalk
};

void far mous_in_3d(void)
{
    unsigned char how;

    if (player->b305 > 0)
        return;
    if (inplist->buttons & 1)
        player_mous_move();
    newPlObj = 0;
    if (inplist->buttons & 2) {
        switch (GameInputMode) {
        case 0:
            if (RightButtonThing == 0)
                how = 2;
            else
                how = RightButtonThing - 1;
            if (how != 1) {
                if (inplist->buttons & 1)
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
                    game_sprint(0x6B);
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
}

void far inv_look(void)
{
    unsigned char ident;
    int lore;
    int head;

    checkTrap(ThePlayer, newPlObj, 5, MapObj_X, MapObj_Y);
    if (newPlObj == 0)
        newPlObj = pick_inv(2);
    ident = OBJ_MAJOR(newPlObj) != 5 && OBJ_MAJOR(newPlObj) != 6
        && ComObjData[OBJ_ID(newPlObj)].c9 != 2;
    if (ident == 1) {
        head = OBJ_HEADING(newPlObj);
        if (head & 4)
            lore = head & 3;
        else {
            lore = skill_check(player->skills[8], 8) + 1;
            if (lore == 0)
                lore = 1;
            if ((head & 3) > lore)
                lore = head & 3;
            SET_HEADING(newPlObj, lore | 4);
        }
    } else
        lore = 1;
    LookAt(newPlObj, lore);
    DoInventoryMouse(-1);
}

/* Not in the FM Towns build and never called in DOS. */
static void far seg026_2716_F8A(void)
{
    DoInventoryMouse(1);
}

/* Not in the FM Towns build and never called in DOS: use the object under the cursor. */
void far seg026_2716_F98(void)
{
    if (newPlObj == 0)
        newPlObj = pick_inv(2);
    mouse_release(1);
    UseObj(ThePlayer, newPlObj, 1);
}

void far mous_in_inv(void)
{
    int x, y;
    int n;
    int hit;

    newPlObj = 0;
    switch (GameInputMode) {
    case 0:
        n = 0;
        if (CursorObjPtr == 0) {
            if (inplist->buttons & 1) {
                if (RightButtonThing == 3)
                    n = -2;
            } else if (RightButtonThing != 1 || inplist->field8 != 1)
                n = -2;
        }
        DoInventoryMouse(n);
        break;
    case 1:
        DoInventoryMouse(4);
        break;
    case 2:
        x = inplist->x + 0xF0;
        y = inplist->y + 0x51;
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

void far mous_in_panel(void)
{
    if (player->sleepbits && player->in_void || player->b305 > 0)
        return;
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

void far deal_with_icons(int mode)
{
    unsigned char released = 0;

    if (GameInputMode != 0)
        return;
    if (mode == -1) {
        current_button = get_iconreg_button();
        mode = button_to_mode[current_button];
        mouse_release(1);
        released = 1;
    } else
        current_button = mode_to_button[mode - 1];
    if (mode == 5)
        busywaiting_new_options(gameopts_buttongroup);
    else {
        set_screen_frame(8, 6);
        if (player->b60) {
            player->b60 = 0;
            clear_fight_state();
            player->b60 = 0;
        }
        player->b60 = 0;
        if (RightButtonThing == 1 || RightButtonThing == 3 || RightButtonThing == 4)
            unforce_mouse_cursor(3);
        if (++mode == RightButtonThing) {
            new_IconUnselect(mode_to_button[RightButtonThing - 1]);
            RightButtonThing = 0;
        } else {
            if (RightButtonThing != 0)
                new_IconUnselect(mode_to_button[RightButtonThing - 1]);
            RightButtonThing = mode;
            if (RightButtonThing == 2) {
                if (player->sleepbits && player->in_void || player->b305 > 0 || player->motion_state & 1)
                    RightButtonThing = 0;
                else {
                    player->b60 = 1;
                    set_screen_frame(8, 4);
                    new_IconSelect(mode_to_button[RightButtonThing - 1]);
                    if (get_current_music() < 2 || get_current_music() > 4)
                        set_new_music(5);
                }
            } else
                new_IconSelect(mode_to_button[RightButtonThing - 1]);
        }
        if (!player->b60 && get_current_music() == 5)
            set_random_walking_music(-1);
        if (released)
            mouse_release(1);
        if (RightButtonThing == 1 || RightButtonThing == 3 || RightButtonThing == 4)
            force_mouse_cursor(0x1077);
    }
}

void far pick_fightmode(void)
{
    if (player->b60 == 1)
        return;
    if (player->motion_state & 1)
        return;
    if (RightButtonThing == 1 || RightButtonThing == 3 || RightButtonThing == 4)
        unforce_mouse_cursor(3);
    if (RightButtonThing != 0)
        new_IconUnselect(mode_to_button[RightButtonThing - 1]);
    RightButtonThing = 2;
    player->b60 = 1;
    set_screen_frame(8, 4);
    new_IconSelect(mode_to_button[RightButtonThing - 1]);
    if (get_current_music() < 2 || get_current_music() > 4)
        set_new_music(5);
}

void far punt_fightmode(void)
{
    if (player->b60) {
        set_screen_frame(8, 6);
        player->b60 = 0;
        if (LeftPanel == 0)
            new_IconUnselect(mode_to_button[1]);
        RightButtonThing = 0;
        clear_fight_state();
        set_random_walking_music(-1);
    }
}

void far toggle_fightmode(void)
{
    if (inplist->field8 == 1) {
        if (player->b60)
            punt_fightmode();
        else
            pick_fightmode();
    }
}
