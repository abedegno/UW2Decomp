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

#include "map.h"
#include "object.h"

/* The mouse and keyboard state handed to an input handler, 10 bytes (seg010's dispatcher
   fills it in; reached through the near pointer inplist). */
struct Inplist {
    int x, y;                           /* relative to the region that took the click */
    int mouse;                          /* 0x04: 1 for a mouse event, 0 for a key */
    int cmd;                            /* 0x06: the input code, the buttons for a mouse event */
    int mode;                           /* 0x08: mask of the screen modes */
};

/* A message scroll's state. Field names beyond the ones the code clearly uses (coordinates,
   cursor position, the font colour) are our own; FM Towns has no per-field names, only
   the struct's three instances (_main_scroll, _npc_scroll, _menu_scroll). Byte-packed,
   21 (0x15) bytes, matching the spacing between those three instances in the EXE. */
struct Scroll {
    int x0;                             /* 0x00 */
    int y0;                             /* 0x02 */
    int top;                            /* 0x04 */
    int bottom;                         /* 0x06 */
    int cur_x;                          /* 0x08 */
    int cur_y;                          /* 0x0A */
    int left;                           /* 0x0C */
    int last_y;                         /* 0x0E */
    unsigned char more_pending;         /* 0x10 */
    int start_line;                     /* 0x11 */
    int font_color;                     /* 0x13 */
};

/* A button group of the options panel: an init function, the art row for its pictures
   (or -1), and for each of its seven buttons a handler, the handler's argument and a
   group to go to next. */
struct buttongroup {
    void (far *init)(void);             /* 0x00 */
    int images;                         /* 0x04 */
    void (far *fn[7])(int);             /* 0x06 */
    int arg[7];                         /* 0x22 */
    struct buttongroup *sub[7];         /* 0x30 */
};

/* One menu button, 16 bytes: the picture when not selected and when selected, and the
   button's bottom left corner and size. */
struct Button {
    unsigned char far *img[2];          /* 0x00 */
    int x;                              /* 0x08 */
    int y;                              /* 0x0A, the bottom row */
    int w;                              /* 0x0C */
    int h;                              /* 0x0E */
};

struct StringNode { unsigned char value, pad, left, right; };

/* INPUT.C: the input dispatcher */
void far dispatch_key(struct Inplist *in, int code);
void far init_input(void);
void far free_input(void);
void far input_del(int hndl);

/* ICONS.C: the icon bar */
extern int gameopts_done;
extern char dseg_67d6_120;
extern char current_hilit_button;
extern unsigned char button_to_mode[6];
extern unsigned char mode_to_button[6];
extern struct buttongroup detail_buttongroup;
extern struct buttongroup file_buttongroup;
extern struct buttongroup musicsound_buttongroup;
extern struct buttongroup *current_buttongroup;
void far setup_icon_buttons(void);
void far term_icon_buttons(void);
int far get_iconreg_button(void);
void far new_IconUnselect(int index);
void far new_IconSelect(int index);
void far do_option_shortcut(int keycode);

/* Input codes from do_keyboard_input (seg015): the key's code in the low byte, codes
   from 0x80 being the special keys, with these added for the shift keys held. */
#define KEY_CTRL        0x100
#define KEY_ALT         0x200
#define KEY_SHIFT       0x400           /* added to special keys only */

/* MOUSE.C: the mouse */
extern int joymovecur;
extern int fauxright;
extern char mouse_hand;
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
void far mouse_getxy(int *x, int *y);
void far mouse_Qgetxy(int *x, int *y);
void far mouse_clearQ(void);
void far mouse_putxy(int x, int y);
int far mouse_getbut(int *b);
void far mouse_constrain(int x0, int y0, int x1, int y1);
void far mouse_freereign(void);
int far mouse_get_input(void);
int far defineMouseRegion(int x0, int y0, int x1, int y1, int id);
void far undefineMouseRegion(int handle);
void far force_mouse_cursor(int id);
void far unforce_mouse_cursor(int how);
void far MousQUp(char from3d);

/* WRAPPER.C: the options panel */
int far get_buttonreg_button(void);
void far new_hilit_button(int b);
void far install_buttongroup(struct buttongroup *g);
void far deal_with_button(int b);
void far load_message(int row, int col, int how);
void far detail_setting_message(int how);
void far draw_button(int b, int hilit);
void far copy_rectangle(int x, int y, int w, int h, int sx, int sy, int how);
void far blit_panel_to_main(void);
void far load_buttongroup_images(int n);
void far move_hilite(int dir);
void far donothing_opt(int arg);
void far resume_play_opt(int arg);
void far detail_set_opt(int level);
void far quit_do_opt(int arg);
void far save_opt(int arg);
void far restore_opt(int arg);
void far do_saverest_opt(int slot);
void far music_opt(int arg);
void far sound_opt(int arg);
void far do_musicsound_opt(int on);
void far game_group_fun(void);
void far detail_group_fun(void);
void far quit_group_fun(void);
void far file_group_fun(void);
void far musicsound_group_fun(void);

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

/* SCROLL.C: the message scroll */
extern struct Scroll near *scroll;  /* DS:34B0 */
extern unsigned char mouse_in_scroll;  /* DS:34B2 */
extern struct Scroll main_scroll;  /* DS:938 */
extern struct Scroll npc_scroll;  /* DS:94D */
extern int scroll_mode;  /* DS:97E */
extern int start_line;  /* DS:980 */
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

/* MAINMENU.C: the main menu */
int far parse_start_input(int n, struct Button far *b, int text, int sel);
int far do_journey(void);
char far Region_ovr147_A73(int x, int y);
void far real_start(int intro);

/* INTERACT.C: the player's interaction with the 3D view and the panels */
extern unsigned char PoisonWeap;
extern unsigned char TimeStop;
extern unsigned char Hasted;
extern unsigned char WizEye;
extern unsigned char Blessed;
extern unsigned char Valor;
extern int LeftPanel;
extern unsigned char quick_time;
extern unsigned char realDScheck;
extern struct Object far *ObjectActing;
extern int RightButtonThing;
extern struct Tile far *PickMap;
extern int MapObj_X;
extern int MapObj_Y;
extern int GameInputMode;  /* DS:2506, declared in SKILLS.C */
void far player_3dtalk(void);
void far player_3duse(void);
void far display_scr(void);
void far RedispInv(void);
void far look_nothing(unsigned char how, int txt);
void far inv_look(void);
void far mous_in_panel(void);
void far deal_with_icons(int mode);
void far toggle_fightmode(void);

/* AUTOMAP.C: the automap */
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
void far make_terrain_unseen(int x, int y, unsigned w, unsigned h);

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
int far LoadFileStringsPak_seg039_547(void);
void far seg039_3452_5E1(void);
void far free_strings(void);
char far * far get_string(int id);
int far make_string(char far *s, int block);
void far clear_dynamics(int block);
char far * far fix_name_string(char far *s, unsigned char article, char plural);
char far * far str_cat(char far *dst, char far *src);
char far * far read_string(int block, int string);
int far seg039_3452_781(int file);
int far seg039_3452_7B2(int file, int index);
void far game_sprint(int id);
void far game_strings_3(int first, int second, int third);
/* 3265:0814, upper-cases a far string in place and returns it. FM Towns has no
   counterpart (its build is Japanese), so the name is the segment and offset. */
char far * far seg039_3452_814(char far *s);
char far * far seg039_3452_857(char far *s);
int far seg039_3452_89A(char far *s);

/* Defined where no source has it yet: data the link takes from the EXE. */
extern struct StringNode far *StringsPak_Address_Indices;
extern int StringsPak_FileHandle;
extern int StringsPak_NoOfNodes;
extern int string_bits;

#endif
