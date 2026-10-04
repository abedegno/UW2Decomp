/* ui.h: The user interface: input dispatch, joystick and mouse, the icon bar, the options
   panel, the game screen, the message scroll, strings, the main menu, the 3D view's mouse
   interaction and the automap. */
#ifndef UI_H
#define UI_H

#include "uw2.h"

struct Button;
struct Inplist;
struct Object;
struct Scroll;
struct StringNode;
struct Tile;
struct buttongroup;
struct Arc;

#include "map.h"
#include "object.h"

/* The mouse and keyboard state handed to an input handler, 10 bytes (seg010's dispatcher
   fills it in; reached through the near pointer inplist). */
struct Inplist {
    int16 x, y;                         /* relative to the region that took the click */
    int16 mouse;                        /* 0x04: 1 for a mouse event, 0 for a key */
    int16 cmd;                          /* 0x06: the input code, the buttons for a mouse event */
    int16 mode;                         /* 0x08: mask of the screen modes */
};

/* A message scroll's state. Field names beyond the ones the code clearly uses (coordinates,
   cursor position, the font colour) are our own; FM Towns has no per-field names, only
   the struct's three instances (_main_scroll, _npc_scroll, _menu_scroll). Byte-packed,
   21 (0x15) bytes, matching the spacing between those three instances in the EXE. */
struct Scroll {
    int16 x0;                           /* 0x00 */
    int16 y0;                           /* 0x02 */
    int16 top;                          /* 0x04 */
    int16 bottom;                       /* 0x06 */
    int16 cur_x;                        /* 0x08 */
    int16 cur_y;                        /* 0x0A */
    int16 left;                         /* 0x0C */
    int16 last_y;                       /* 0x0E */
    unsigned char more_pending;         /* 0x10 */
    int16 start_line;                   /* 0x11 */
    int16 font_color;                   /* 0x13 */
};

/* A button group of the options panel: an init function, the art row for its pictures
   (or -1), and for each of its seven buttons a handler, the handler's argument and a
   group to go to next. */
HOST_LAYOUT_BEGIN
struct buttongroup {
    void (far *init)(void);             /* 0x00 */
    int16 images;                       /* 0x04 */
    void (far *fn[7])(int);             /* 0x06 */
    int16 arg[7];                       /* 0x22 */
    struct buttongroup *sub[7];         /* 0x30 */
};
HOST_LAYOUT_END

/* One menu button, 16 bytes: the picture when not selected and when selected, and the
   button's bottom left corner and size. */
HOST_LAYOUT_BEGIN
struct Button {
    unsigned char far *img[2];          /* 0x00 */
    int16 x;                            /* 0x08 */
    int16 y;                            /* 0x0A, the bottom row */
    int16 w;                            /* 0x0C */
    int16 h;                            /* 0x0E */
};
HOST_LAYOUT_END

struct StringNode { unsigned char value, pad, left, right; };

/* INPUT.C: the input dispatcher */
void far dispatch_key(struct Inplist *in, int code);
void far init_input(void);
void far free_input(void);
void far input_del(int hndl);
/* A key or mouse region's handler, called with the argument it was registered with. */
typedef void (far *InputFn)(NEARPTR arg);
extern struct Inplist *inplist;
int far input_addmouse(int ulx, int uly, int lrx, int lry, NEARPTR arg, int mask, InputFn func);
int far _input_addkey(int key, NEARPTR arg, int mask, InputFn func);
void far input_dispatch(struct Inplist *in);

/* ICONS.C: the icon bar */
extern char gameopts_done;
void far new_IconUnselect(int index);
void far new_IconSelect(int index);
void far do_option_shortcut(int keycode);

/* Input codes from do_keyboard_input (seg015): the key's code in the low byte, codes
   from 0x80 being the special keys, with these added for the shift keys held. */
#define KEY_CTRL        0x100
#define KEY_ALT         0x200
#define KEY_SHIFT       0x400           /* added to special keys only */
/* The special keys' codes: seg019's scan code table (Asc, seg063:0010; UW2's seg021) gives
   F1 to F10 (scan codes 3Bh..44h) 0x80..0x89 and the keypad (47h..53h) 0x8C..0x96; shifted
   Tab is 0xA3. Ordinary keys give their character: Backspace 8, Enter 0x0D, Escape 0x1B. */
#define KEY_BACKSPACE   0x08
#define KEY_ENTER       0x0D
#define KEY_ESC         0x1B
#define KEY_F1          0x80
#define KEY_F2          0x81
#define KEY_F3          0x82
#define KEY_F4          0x83
#define KEY_F5          0x84
#define KEY_F6          0x85
#define KEY_F7          0x86
#define KEY_F8          0x87
#define KEY_F9          0x88
#define KEY_F10         0x89
#define KEY_HOME        0x8C
#define KEY_UP          0x8D
#define KEY_PGUP        0x8E
#define KEY_LEFT        0x8F
#define KEY_PAD5        0x90            /* the keypad's centre key */
#define KEY_RIGHT       0x91
#define KEY_END         0x92
#define KEY_DOWN        0x93
#define KEY_PGDN        0x94
#define KEY_INS         0x95
#define KEY_DEL         0x96
#define KEY_BACKTAB     0xA3            /* Shift+Tab */
/* The separate cursor keys (E0-prefixed scan codes): KEYQUEUE.ASM turns the 16 codes of
   its table at seg063:0288 into 60h..6Fh, and Asc gives Home, Up, PgUp, Left, Right, End,
   Down and PgDn (60h + 6..13) these codes; Ins and Del give KEY_INS and KEY_DEL. */
#define KEY_GHOME       0xA5
#define KEY_GUP         0xA6
#define KEY_GPGUP       0xA7
#define KEY_GLEFT       0xA8
#define KEY_GRIGHT      0xA9
#define KEY_GEND        0xAA
#define KEY_GDOWN       0xAB
#define KEY_GPGDN       0xAC

/* MOUSE.C: the mouse */
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
int far init_mouse(void);
void far mouse_show(void);
void far mouse_hide(void);
void far mous_3d_set(int x, int y, int w, int h);
char far mous_in_3d_p(void);
void far mous_3d_hide(void);
void far mous_3d_show(void);
void far mouse_getxy(int16 *x, int16 *y);
void far mouse_Qgetxy(int16 *x, int16 *y);
void far mouse_clearQ(void);
void far mouse_putxy(int x, int y);
int far mouse_getbut(int16 *b);
void far mouse_constrain(int x0, int y0, int x1, int y1);
void far mouse_freereign(void);
int far mouse_get_input(void);
int far defineMouseRegion(int x0, int y0, int x1, int y1, int id);
void far undefineMouseRegion(int handle);
void far force_mouse_cursor(int id);
void far unforce_mouse_cursor(int how);
void far MousQUp(char from3d);
void far mouse_release(char how);
int far do_keyboard_input(char array);
unsigned char far mouse_dragged(char how);

/* WRAPPER.C: the options panel */
extern unsigned char plyregen[2];

/* GAMESCR.C: the main game screen's set-up and teardown and its status clicks */
void far pull_chain(int how);
void far init_gamedisp(void);
void far clear_gamedisp(void);
char far check_save(void);
char far check_rest(void);

/* SCROLLIO.C: the message scroll's interactive side */
void far init_scroll(void);
void far scroll_more(void);
void far wd_replace(int n);
/* match: in this order because TLINK numbers the overlay's stub entries in the order
   Turbo C lists the publics, which for names with the same hash key is the order they
   were first seen: the EXE's stub has scroll_clear and wdialog before scroll_wait and
   wd_bool. */
void far scroll_clear(char redraw);
int far wdialog(char *prompt, char *initial, char *result, char anychar, int maxlen);
void far scroll_wait(int ticks, char mouse);
void far wd_bool(char yes);
int far wyorn(char *question, int id, char *answer);

/* SCROLL.C: the message scroll */
extern struct Scroll near *scroll;  /* DS:34B0 */
extern unsigned char mouse_in_scroll;  /* DS:34B2 */
extern struct Scroll main_scroll;  /* DS:938 */
extern struct Scroll npc_scroll;  /* DS:94D */
extern int16 scroll_mode;  /* DS:97E */
extern int16 start_line;  /* DS:980 */
extern unsigned char menus_active;
extern unsigned char didMouseInput;  /* DS:983 */
extern char scroll_esc;  /* DS:98C */
void far scroll_up(int n);
void far scroll_print1(char *text, int flag);
void far scroll_print2(char *text, int flag);
void far scroll_print3(char *text, int flag);
void far scroll_wrap(char *text, int flag);
void far set_mouse_in(void);
void far do_main_scroll(void);
void far do_npc_scroll(void);
void far do_play_scroll(void);
void far draw_edges(void);
void far draw_conv_edges(void);
void far draw_scroll(int x, int y, int w, int h, char flag);
int far scroll_print(char far *s);

/* MAINMENU.C: the main menu */
int far parse_start_input(int n, struct Button far *b, int text, int sel);
int far do_journey(void);
void far real_start(int intro);

/* INTERACT.C: the player's interaction with the 3D view and the panels */
/* RightButtonThing, the interaction mode the icons on the left choose (deal_with_icons:
   icon n, from the top, selects mode n + 1); a click in the 3D view runs player_disp[mode -
   1], or in the default mode looks, and gets on a drag. The names are ours, from
   player_disp's FM Towns names. */
#define IMODE_DEFAULT   0
#define IMODE_USE       1               /* player_3duse */
#define IMODE_FIGHT     2               /* player_3dattack, the weapon drawn */
#define IMODE_LOOK      3               /* player_3dlook */
#define IMODE_GET       4               /* player_3dget */
#define IMODE_TALK      5               /* player_3dtalk */
/* GameInputMode: what owns the pointer (mous_in_3d, mous_in_inv). The names are ours. */
#define GIM_NONE        0
#define GIM_CARRY       1               /* an object is on the cursor */
#define GIM_TARGET      2               /* a spell or object wants a target (ObjectActor) */
#define GIM_AIM         3               /* a missile spell is being aimed (BlastFunction) */
extern unsigned char TimeStop;
extern unsigned char Hasted;
extern unsigned char WizEye;
extern unsigned char Blessed;
extern int16 LeftPanel;
extern unsigned char realDScheck;
extern struct Object far *ObjectActing;
extern int16 RightButtonThing;
extern struct Tile far *PickMap;
extern int16 MapObj_X;
extern int16 MapObj_Y;
extern int16 GameInputMode;  /* DS:2506, declared in SKILLS.C */
void far player_3dtalk(void);
void far player_3duse(void);
void far display_scr(void);
void far RedispInv(void);
void far look_nothing(unsigned char how, int txt);
void far inv_look(void);
void far mous_in_panel(void);
void far deal_with_icons(int mode);
void far toggle_fightmode(void);
extern int16 PickDist;
extern int32 lastDurCheck;
/* What a click on an object in the 3D view does, while a spell or skill wants a target. */
typedef void (far *ActorFn)(struct Object far *obj, int a, int b);
extern ActorFn ObjectActor;
extern int16 ObjectActorArg;
void far mous_in_3d(void);
void far punt_fightmode(void);
void far seg024_24DC_D0A(struct Object far *obj, int how);

/* AUTOMAP.C: the automap */
/* One map note: its text and where it sits on the map, 0x36 bytes. */
struct ATM {
    char text[0x32];
    int16 x;                            /* 0x32, -1 once erased */
    int16 y;                            /* 0x34 */
};
void far GetTheWords(int lev);

/* The map notes, up to 100 (FARDATA.ASM's far segment). */
extern struct ATM far ATM_Strings[100];

void far ManageDungeonMap(void);
void far ShowDungeonMap(void);
void far DoTile(int type, int x, int y);
void far DoDoorTile(int x, int y, int px, int py);
void far SaveTheWords(int lev);
void far ShowAutoMapLevel(int lev);
void far RedisplayStrings(void);
void far AutoMap(void);
void far ExitAutoMap(void);
void far ClearAutoMap(void);
void far PixelDarken(int x, int y, int base, unsigned n);
char far ShadeSide(int side, int x, int y);
void far ChangeAutoMapLevel(int lev);
void far automap_scr(void);
unsigned char far GetAutoMapLevel(struct Arc *arc, int lev);
extern unsigned char PlayersMap[MAP_SIZE][MAP_SIZE];
unsigned char far SaveAutoMapLevel(struct Arc *arcp, int lev);
/* What automap_area does to each tile. */
typedef char (far *AreaMapFn)(int x, int y, int16 *arg);

/* String blocks of DATA\STRINGS.PAK. A string id is the block shifted left 9 plus the
   string's number in it (get_string splits it as id >> 9 and id & 0x1FF; make_string
   builds it). The block contents are read from the file itself, decoded with its own
   Huffman tree: block 1's first string is "Hey, its all the game strings", block 4 is the
   item names (items.h), block 6 the spell names, block 9 the text string traps' messages
   (ovr166 indexes it by the trap's quality and owner), block 10 the descriptions of
   walls and floors (seg026 prints them when the player looks at one). */
#define STR_GAME        0x200           /* block 1: the game's messages (game_sprint) */
#define STR_CHARGEN     0x400           /* block 2: character creation: sexes, classes,
                                           skills, attribute labels */
#define STR_BOOKS       0x600           /* block 3: the text of books and scrolls */
#define STR_OBJNAMES    0x800           /* block 4: item names */
#define STR_OBJLOOK     0xA00           /* block 5: qualities, states and moods */
#define STR_SPELLS      0xC00           /* block 6: spell and enchantment names */
#define STR_CONV        0xE00           /* block 7: conversation and barter words, and
                                           the names of NPCs from string 0x10 */
#define STR_WRITING     0x1000          /* block 8: writings, plaques and gravestones */
#define STR_TRAPTEXT    0x1200          /* block 9: the text string traps' messages */
#define STR_TEXTURES    0x1400          /* block 10: wall and floor descriptions */
/* Block numbers themselves (read_string, make_string, clear_dynamics): */
#define STRBLK_DYNAMIC  0x7C            /* strings a conversation builds (ovr095, ovr103) */
#define STRBLK_PLAYER   0x7D            /* the player's name (ovr143's make_string) */
#define STRBLK_CUTSCENE 0xC00           /* + n: cutscene n's text (ovr108) */
#define STRBLK_CONVERSATION 0xE00       /* + n: conversation n's strings (STRINGS.PAK's
                                           blocks 0xE00 and up) */

/* GAMESTRN.C: strings */
void far free_strings(void);
char far * far get_string(int id);
int far make_string(char far *s, int block);
void far clear_dynamics(int block);
char far * far fix_name_string(char far *s, unsigned char article, char plural);
char far * far str_cat(char far *dst, char far *src);
char far * far read_string(int block, int string);
void far game_sprint(int id);
void far game_strings_3(int first, int second, int third);
/* 3265:0814, upper-cases a far string in place and returns it. */
/* name: FM Towns has no counterpart (its build is Japanese), so the name is the segment
   and offset. */
char far * far seg039_3452_814(char far *s);
unsigned char far init_strings(void);
int far replace_string(char far *s, int id);
int far get_name(char far *dst, struct Object far *obj, char article, char plural);
int far seg039_3495_537(void);
void far seg039_3495_5E4(void);
int far seg039_3495_784(FILE *file);
int far seg039_3495_7B5(FILE *file, int index);
char far * far seg039_3495_85A(char far *s);
int far seg039_3495_89D(char far *s);

/* Defined where no source has it yet: data the link takes from the EXE. */
extern struct StringNode far *StringsPak_Address_Indices;
extern FILE *StringsPak_FileHandle;
extern int16 StringsPak_NoOfNodes;
extern int16 string_bits;

/* OPTIONS.C */
void far run_options_panel(int show);
void far close_option_panel(void);
void far draw_top_page(void);
void far draw_save_page(void);
void far draw_quit_yn_page(void);
void far draw_music_and_sound_page(void);
void far draw_detail_page(void);
void far choose_music(int row);
void far choose_sounds(int row);
void far choose_detail(int row);
void far choose_top(int row);
void far choose_save(int row);
void far choose_quit(int row);
void far set_options_page(int page);
void far press_options_btn(int row);
void far options_mouse_click(int x, int y);
void far options_handle_key(int key);

#endif
