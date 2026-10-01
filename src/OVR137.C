/* target: ovr137 */
/* opts: -mm -1 -G -O -Y -d */
/* The main game screen's set-up and teardown and its status clicks: the panel chain, the
   compass (hunger, fatigue, world and time of day) and the flask (health or mana, and
   poison) messages, registering their mouse areas, and the checks before saving or resting:
   the whole of DOS overlay ovr137, in original order. Function and global names are the
   originals from the FM Towns symbol table; the source file's own name is not known. */

#include <stdlib.h>

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0x37];
    unsigned char play_mana;            /* 0x37 */
    unsigned char max_mana;             /* 0x38 */
    unsigned char hunger;               /* 0x39 */
    unsigned char fatigue;              /* 0x3A */
    char pad3B[0x60 - 0x3B];
    unsigned b60:1;                     /* 0x60 */
    unsigned poison:4;
    unsigned active_spells:4;
    unsigned b60_9:2;
    unsigned b60_11:1;
    unsigned shrooms:2;
    unsigned drunk:6;                   /* word 0x61, bits 6..11 */
    unsigned automap:1;                 /* word 0x62, bit 4 */
    unsigned b62_5:1;
    unsigned sleepbits:3;
    unsigned in_void:1;                 /* word 0x62, bit 9 */
    char pad64[0xE9 - 0x64];
    unsigned char worlds;               /* 0xE9 */
    char padEA[0x369 - 0xEA];
    unsigned long game_clock;           /* 0x369 */
};

/* The player's critter data, reached through the near pointer `playerdat`. */
struct Critter {
    char pad0[4];
    unsigned char max_vit;              /* 0x04 */
};

struct Object {
    char pad0[8];
    unsigned char hp;                   /* 0x08 */
};

/* The mouse and keyboard state. */
struct Inplist {
    int x, y;                           /* 0x00 */
};

extern struct Player near *player;
extern struct Critter near *playerdat;
extern struct Object far *ThePlayer;
extern struct Inplist near *inplist;
extern char RightPanel;
extern int LeftPanel;
extern int PlayerLevel;
extern int GameInputMode;
extern int RightButtonThing;
extern unsigned char button_to_mode[];
/* This file's uninitialised data: the four mouse-area handles, FM Towns names. Defined
   here, they land at DS:817C to DS:8183 in the EXE's order. */
int inforMshandle;               /* DS:817C */
int actspMshandle;               /* DS:817E */
int flaskMshandle;               /* DS:8180 */
int spellMshandle;               /* DS:8182 */

void far set_screen_frame(int a, int b);
int far scroll_print(char far *s);
void far game_sprint(int id);
void far game_strings_3(int a, int b, int c);
void far mouse_release(int how);
char far * far get_string(int id);
void far str_copy(char far *d, char far *s);
void far str_cat(char far *d, char far *s);
void far setup_icon_buttons(void);
void far term_icon_buttons(void);
void far new_IconSelect(int index);
int far input_addmouse(int a, int b, int c, int d, int buttons, int mode, void far (*handler)());
void far input_del(int handle);
void far try_cast();
void far try_clear();
void far BeginInventory(void);
void far EndInventory(void);
void far init_scroll(void);
void far play_music(void);
void far display_scr(void);
void far init_scrgr(void);
void far grSoftPageFlip(void);
void far unforce_mouse_cursor(int how);

/* The argument is not read; flask_info passes 0, and FM Towns passes one too. */
void far pull_chain(int how)
{
    if (RightPanel == 0)
        set_screen_frame(6, 2);
    else if (RightPanel != 4)
        set_screen_frame(6, 0);
}

void far print_info(void)
{
    int known;
    int world;
    int n;
    int w;

    scroll_print("\n");
    game_strings_3(0x44, player->hunger / 0x1E + 0x75, 0x74);
    if (player->in_void)
        game_sprint(0x128);
    else {
        n = player->fatigue / 0x17;
        if (n > 5)
            n = 5;
        game_sprint(0x83 - n);
    }
    game_sprint(0x60);
    if ((known = player->worlds) != 0) {
        world = (PlayerLevel - 1) / 8;
        w = ((known << 1) + 1) & (1 << world) ? world + 2 : 1;
        game_strings_3(0x49, w + 0x49, 0x60);
    } else
        world = -1;
    n = player->game_clock / 0x1C2000L;
    n = n % 12;
    game_strings_3(0x48, n + 0x54, 0x60);
    scroll_print("\n");
    if (world == -1)
        scroll_print("\n");
    mouse_release(1);
}

void far flask_info(void)
{
    char cur[10];
    char max[10];
    char msg[120];

    if (inplist->x > 0x16 && inplist->x < 0x2C) {
        if (inplist->y > 10)
            pull_chain(0);
    } else {
        if (inplist->y > 0x1E)
            return;
        str_copy(msg, get_string(((inplist->x > 0x1E) + 0x66) | 0x200));
        if (inplist->x < 0x28) {
            itoa(ThePlayer->hp, cur, 10);
            itoa(playerdat->max_vit, max, 10);
            if (player->poison)
                game_strings_3(0x68, (player->poison - 1) / 3 + 0x61, 0x69);
        } else {
            itoa(player->play_mana, cur, 10);
            itoa(player->max_mana, max, 10);
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
    setup_icon_buttons();
    spellMshandle = input_addmouse(0xA9, 0x22, 0xDF, 0x34, 0, 1, try_cast);
    actspMshandle = input_addmouse(0x11, 0x22, 0x42, 0x34, 0, 1, try_clear);
    inforMshandle = input_addmouse(0x5B, 0x22, 0x94, 0x34, 0, 1, print_info);
    flaskMshandle = input_addmouse(0xF3, 0x24, 0x13C, 0x45, 0, 1, flask_info);
}

void far clear_gameinp(void)
{
    term_icon_buttons();
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
    if (LeftPanel == 0 && RightButtonThing != 0)
        new_IconSelect(button_to_mode[RightButtonThing + 5]);
    display_scr();
    init_scrgr();
}

void far clear_gamedisp(void)
{
    clear_gameinp();
    EndInventory();
}

/* IDA's ovr137_3B7. FM Towns has check_save_ at the same place, between clear_gamedisp_
   and check_rest_, with the same code: 0xAE when GameInputMode is set, printed between two
   page flips. */
char far check_save(void)
{
    int id;

    id = 0;
    if (GameInputMode)
        id = 0xAE;
    if (id) {
        grSoftPageFlip();
        game_sprint(id);
        grSoftPageFlip();
        return 0;
    }
    return 1;
}

char far check_rest(void)
{
    if (GameInputMode) {
        GameInputMode = 0;
        unforce_mouse_cursor(0);
    }
    return 1;
}
