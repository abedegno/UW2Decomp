/* gfx.h: Graphics: the sprite, screen and drawing modules, art loading, palettes and colour
   cycling, full-screen pictures, cutscenes and the screen furniture around the 3D view. */
#ifndef GFX_H
#define GFX_H

#include "uw2.h"

struct CutsState;
struct FontInfo;

/* A font's header (cur_font): the first 12 bytes of a FONT*.SYS file (UW-Formats 3.5),
   then the characters' bitmaps, each followed by its width. */
struct FontInfo {
    int16 widthsize;                    /* 0x00, the size of a character's width field: 1 */
    int16 charsize;                     /* 0x02, one character's bitmap, in bytes */
    int16 spacewidth;                   /* 0x04, the width of a space, in pixels */
    int16 height;                       /* 0x06, the line height */
    int16 rowwidth;                     /* 0x08, a row of a character's bitmap, in bytes */
    int16 maxwidth;                     /* 0x0A, the widest character, in pixels */
};

/* The state of a cutscene, on show_anm's stack, 0x4A bytes (UW1: UW2's without repeat43,
   so the speech and fade fields and the flags come two bytes earlier). Field names are
   ours, from the code that uses them (CUTS.C). Flag bits: b0 drawing (cleared while a key
   skips frames: UW2's b0 is the reverse), b1 a key arrived, b2 play on in this file, b3
   play on in the cutscene, b4 Escape may end it, b5 speech is available, b6 speech is
   playing, b7 the current wait ignores keys. */
HOST_LAYOUT_BEGIN
struct CutsState {
    char name[0x13];                    /* CUTS\csXXX.nXX */
    int16 x, y, w, h;                   /* 0x13, the window; 320 by 200 for full screen */
    unsigned char windowed;             /* 0x1B */
    unsigned char far *palette;         /* 0x1C */
    char far *text_lines[6];            /* 0x20 */
    unsigned char color38;              /* 0x38 */
    int16 flag39;                       /* 0x39, subtitle lines to draw */
    int16 frame3B, frame3D;             /* 0x3B, the frame a skip runs to, a pause's frame */
    uint16 frame3F;                     /* 0x3F, pause length (256-tick units) */
    uint16 repeat41;                    /* 0x41, loops left */
    int16 speech43, fade45, fade47;     /* 0x43, speech playing, fade in, fade out */
    union {
        unsigned char value;
        struct { uint16 b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1; } bit;
    } flags;                            /* 0x49 */
};
HOST_LAYOUT_END

/* SPRITE.ASM */
/* create_sprite(layer) or create_sprite(layer, w, h): a sprite record, with a video memory
   block of w by h for layers 1 to 3; its number, or -1 (SPRITE.ASM). Called with one argument
   and with three, so old-style under Turbo C (OLDSTYLE) and variadic on the host. */
int far create_sprite OLDSTYLE((int layer, ...));
void far change_sprite(int spr, int x, int y, int w, int h);
/* The sprite library, seg000. create_sprite is called with one argument and with three.
   match: its callers had no prototype for it, so Turbo C sees none (OLDSTYLE); the port
   sees a variadic one. */
int far create_sprite OLDSTYLE((int layer, ...));
void far draw_mask(int spr, int frame);
void far draw_sprite(int spr, int frame);
void far erase_sprite(int spr);
void far move_sprite(int spr, int x, int y);
void far set_yoff(int spr, int yoff);
void far update_sprites(void);

/* VALLOC.ASM */
void far restore_rect(int handle);
void far save_rect(int handle, int x, int y, int w, int h);
void far seg001_023B_C(void);  /* assembly, DOS only */
int far valloc(int w, int h);
void far vfree(int);

/* SCALEBM.ASM */
extern unsigned char far ShowClip;  /* 370D:0DC4, FM Towns _ShowClip */
extern unsigned char far cXfer[];
extern unsigned char far Transparency;  /* 370D:0DC5 */

/* GRCORE.ASM */
extern int16 far *Color_data_ptr;  /* DS:21B8 */
extern uint16 far *Ytab;
extern unsigned char far *background_color;  /* DS:21C8, reused from SKILLCHK.C */
void far box(int a, int b, int c, int d);
extern unsigned char far *bytefont;
void far clear_window(void);
void far copy_hidden_to_visible(void);
void far copy_visible_to_hidden(void);
void far fbshow(void far *data, int x, int y, int width, int height);
void far fbuf_setcolor(int c);  /* 0085:5239, FM Towns fbuf_setcolor_ */
void far grPageFlip(void);
void far grSoftPageFlip(void);
unsigned char far gr_read_pixel(int x, int y);  /* provisional: 0085:475E */
void far init_colors(void);
void far init_graphics(void);
extern unsigned char far *palette;  /* DS:21AC, the 256 RGB triples */
extern unsigned char far *pixel_color;  /* provisional: DS:21F0 */
void far plot_pixel(int x, int y);
/* 0085:5025, puts back part of a saved area; no FM Towns counterpart is known. */
void far set_the_color(int c);
void far set_the_window(int x0, int y0, int x1, int y1);
void far setup_font(void);
void far string_to_screen(char far *s, int x, int y);
int far string_width(char far *s);
void far uhline(int x1, int y, int x2);
void far urectangle(int, int, int, int);
void far uvline(int x, int y1, int y2);
/* name: a 6-int call in a different file; FM Towns vcopy_ matches the first four
   parameters exactly (by position and value) but takes only four, so this probably is not
   it. Unresolved; kept under its DOS label. */
void far vcopy(int sx, int sy, int w, int h, int x, int y);
void far vcopyfb(int x, int y, int w, int h, int handle);  /* 0085:517B, FM Towns vcopyfb_ */
extern int16 far *wbot;  /* DS:21E0 */
extern int16 far *wleft;  /* DS:21E8 */
extern int16 far *wright;  /* DS:21E4 */
extern int16 far *wtop;  /* DS:21DC */
extern struct FontInfo far *cur_font;  /* DS:21CC */
extern unsigned char far *foreground_color;  /* DS:21C4 */
void far rectangle(int x, int y, int w, int h);
void far show(int x, int y, unsigned char far *bm, int a, int b, int c, int d);
void far seg003_581E(int handle, int x, int y, int w, int h, int sx, int sy);

/* A bitmap of a .GR file (UW-Formats 3.2): its type, its size, for a 4-bit bitmap the
   auxiliary palette, and then a word with the data's size (in nibbles for a 4-bit bitmap)
   and the data. seg009_7 returns one; cFrmtoRaw decodes a 4-bit one from its size word. */
struct Bitmap {
    unsigned char type;                 /* 0x00: BM_8BIT, BM_4BIT_RLE or BM_4BIT */
    unsigned char width;                /* 0x01 */
    unsigned char height;               /* 0x02 */
    union {
        struct {
            uint16 size;                /* 0x03 */
            unsigned char data[1];      /* 0x05 */
        } b8;                           /* type BM_8BIT */
        struct {
            unsigned char auxpal;       /* 0x03, which 16-colour palette of Palettes */
            uint16 size;                /* 0x04 */
            unsigned char data[1];      /* 0x06 */
        } b4;                           /* the 4-bit types */
    } u;
};
#define BM_8BIT         4               /* 8-bit, uncompressed */
#define BM_4BIT_RLE     8               /* 4-bit, run-length */
#define BM_4BIT         0xA             /* 4-bit, uncompressed */

/* Icon numbers, the game's names for its 2D pictures (grs_which1): below 1000h an object's
   picture (through obj_tab); from 1000h the screen art LOADGR.C's load_all_gr keeps in EMS
   (first_button + n - 1000h); from 2000h the pictures it keeps in video memory (first_vram
   + n - 2000h). Each .GR file's pictures follow the previous file's, in load_all_gr's
   order, so the bases come from that order and the picture counts in the headers of UW1's
   own files (buttons 108, cursors 19, 3dwin 4; lfti 12, flasks 77, compass 20, dragons 36,
   inv 7, power 14, eyes 10, chains 16, spells 21, scrledge 22, optb 3). The names are the
   files'. */
#define ICON_ART        0x1000          /* the first EMS screen art picture */
#define ICON_BUTTONS    0x1000          /* buttons.gr */
#define ICON_CURSORS    0x106C          /* cursors.gr; the first is the usual pointer */
#define ICON_3DWIN      0x107F          /* 3dwin.gr, drawn over the 3D view's frame */
#define ICON_VRAM       0x2000          /* the first video memory picture */
#define ICON_LFTI       0x2000          /* lfti.gr, the interaction icons on the left */
#define ICON_FLASKS     0x200C          /* flasks.gr */
#define ICON_COMPASS    0x2059          /* compass.gr: four faces, then the needle */
#define ICON_DRAGONS    0x206D          /* dragons.gr, the dragons beside the view */
#define ICON_INV        0x2091          /* inv.gr */
#define ICON_POWER      0x2098          /* power.gr, the power gem */
#define ICON_EYES       0x20A6          /* eyes.gr, the gargoyle's eyes */
#define ICON_CHAINS     0x20B0          /* chains.gr */
#define ICON_SPELLS     0x20C0          /* spells.gr, the active spell icons */
#define ICON_SCRLEDGE   0x20D5          /* scrledge.gr, the message scroll's edges */
#define ICON_OPTB       0x20EB          /* optb.gr, the options panel's places */

/* GRSPIC.C: graphic resource lookup, decoding, cursor drawing and image scaling */
void far * far grs_unpack(struct Bitmap far *data);
void far grs_fbplot(int icon, int x, int y);
/* name: seg009 (1A6D): grs_which1 and pic_to_fbuf are FM Towns names, called the same
   way. FM Towns reads _grs_off[n] directly where DOS calls seg009_7, which maps the EMS
   page holding the cursor art first; it has no FM Towns name. */
int far grs_which1(int icon);
void far pic_to_screen(int icon, int x, int y, int height, int width);
void far pic_to_fbuf(int icon, int x, int y);
void far mask_to_screen(int icon, int x, int y, int width, int height, int clip);
/* seg009, no FM Towns counterpart known: the first returns a segment for a picture
   number, the second takes a far pointer, two sizes and a count and returns one. */
void far * far seg009_1(int icon);
unsigned far seg009_38C(int index);

/* COLCYCLE.C: palette colour cycling */
void far rotate_bank(unsigned char first, unsigned char count, unsigned char up);
void far cycle_colors(unsigned char t);

/* Fonts: UW1's loader, GRFX.C's grfx_load_font, takes the file name (font4x5p.sys,
   font5x6p.sys the usual one, font5x6i.sys, fontbig.sys, fontchar.sys, fontbutn.sys in
   DATA), where UW2's grfx_quikfont takes an index. Its callers disagree on the argument's
   type, so it is declared in each file that calls it (docs/readability/headers/plan.toml). */

/* Palettes of DATA\PALS.DAT (grfx_quikpal, read_quikpal), 0x300 bytes each. The uses
   name them; UW-Formats' file list gives UW1 the same numbering (blnkmap.byt palette 1,
   chargen.byt palette 3). */
#define PAL_GAME        0               /* the game's palette (main, the automap's exit) */
#define PAL_MAP         1               /* the automap (AUTOMAP.C) */
#define PAL_OPENING     2               /* the opening screen, opscr.byt (MAINMENU.C) */
#define PAL_CHARGEN     3               /* character creation (CHARGEN.C) */
#define PAL_PRESENTS    5               /* the "presents" screens, pres1.byt */
#define PAL_WIN         7               /* the winning screens, win1.byt */

/* GRFX.C: graphics start-up, fonts and palettes */
void far grfx_close(void);
void far grfx_setpal(void far *src);
void far fadein(unsigned char far *src, int count);
void far fadeout3d(int n);
void far fadein3d(int n);
void far fill_FB(int colour);
void far cameras_fade(void);
unsigned char far grfx_quikpal(int n);
void far fadeout(unsigned char far *src, int count);

/* LOADGR.C: art loading */
extern uint16 first_button;
extern uint16 first_tmobj;
extern uint16 first_vram;
/* match: declared before load_gr_ems: the two names have the same public-order key (404),
   and Turbo C lists such publics in reverse order of first sight, as the stub order needs */
char far load_tr_ems(char *art);
void far reload_gr_vpic(int offset, char *art, int image);
unsigned char far read_gr_far(char *art, int image, void far *dst);
int far load_all_gr(void);
void far load_doors(void);
extern unsigned char Palettes[32][16];
/* gronk_gr's callbacks: one returns where to load an image of the given size, the other
   moves a loaded image into place. */
typedef void far *(far *ArtAllocFn)(int size);
typedef unsigned char (far *ArtMoveFn)(void far *p, int size, int n);
char far gronk_gr(char *art, int start, int count, void far *(far *adr)(int),
                  unsigned char (far *move)(void far *, int, int));

/* CUTS.C: the cutscene player */
int far cutsop_txt(uint16 far *code, struct CutsState *st);
int far cutsop_erase(uint16 far *code, struct CutsState *st);
int far cutsop_func(uint16 far *code, struct CutsState *st);
int far cutsop_pause(uint16 far *code, struct CutsState *st);
int far cutsop_end(uint16 far *code, struct CutsState *st);
int far cutsop_loop(uint16 far *code, struct CutsState *st);
int far cutsop_data(uint16 far *code, struct CutsState *st);
int far cutsop_fadeout(uint16 far *code, struct CutsState *st);
int far cutsop_fadein(uint16 far *code, struct CutsState *st);
int far cutsop_jump(uint16 far *code, struct CutsState *st);
int far cutsop_punt(uint16 far *code, struct CutsState *st);
int far cutsop_say(uint16 far *code, struct CutsState *st);
/* cutsop_wait, cutsop_skip, cutsop_stop and read_lp_inc are declared in CUTS.C alone, in the
   order its public-order ties need (cutsop_wait and cutsop_skip share the key 875). */
int far cutsop_clang(uint16 far *code, struct CutsState *st);
/* match: declared early so that it is seen before record_task: both names have the
   public-order key 970, and the stub order lists record_task first */
void far show_anm(int cuts, int x, int y, int w, int h);
int far get_cut_banks(void);
unsigned char far * far get_cuts_block(int which);
void far free_cuts_ems(void);
void far init_cutscene(void);
/* A task install_timebased_task runs from the timer interrupt. */
typedef void (far *Task)(int task, int done);
void far runcutscene(unsigned n);
unsigned char far * far free_block(void);
void far value_cuts(unsigned n, int value);
void far cuts_skipline(FILE *fp);

/* PANELS.C: the screen furniture around the 3D view */
/* The display elements of set_screen_frame(which, value), by index into PANELS.C's adjust,
   goal and setting tables; the names are ours, from what each adjust_ function draws.
   SCR_DRAGON2 is never asked for: set_screen_frame(SCR_DRAGON, n) gives the animation to
   whichever dragon is free. */
#define SCR_VITALITY    0               /* the vitality flask: hit points */
#define SCR_MANA        1               /* the mana flask */
#define SCR_COMPASS     2               /* the heading, 0 to 15 */
#define SCR_POWER       3               /* the power gem: a blow's charge, 9 pulsing */
#define SCR_DRAGON      4               /* the dragons: an animation, 1 to 3 */
#define SCR_DRAGON2     5
#define SCR_PANEL       6               /* the right-hand panel (PANEL_*) */
#define SCR_EYES        7               /* the gargoyle's eyes */
#define SCR_WEAPON      8               /* the first-person weapon (WEAP_*) */
/* The weapon element's values: 0 to 2 a swing of that kind, then: */
#define WEAP_DRAWING    3
#define WEAP_READY      4
#define WEAP_SHEATHING  5
#define WEAP_SHEATHED   6
/* RightPanel, the right-hand panel showing, which is also its picture in PANELS.GR
   (picture 3 is the panel's edge, shown while it turns over). */
#define PANEL_INV       0               /* the inventory */
#define PANEL_RUNES     1               /* the rune bag */
#define PANEL_STATS     2               /* the statistics page */
#define PANEL_EDGE      3
#define PANEL_TURNING   4               /* while it turns over (adjust_panel) */
void far adjust_flasks(int which);
void far adjust_compass(void);
void far adjust_power(void);
void far adjust_panel(void);
void far adjust_eyes(void);
void far adjust_weapon(void);
void far set_runes(unsigned char *runes);
void far init_panelflip(int panel, int x, int y, int w, int h);
void far free_panelflip(void);
char far do_panel_frame(void);
void far do_fbuf_bms(void);
extern int16 weap_frame;
void far set_flask(int which);
void far set_compass(void);
void far reset_scrgr(void);
void far init_scrgr(void);
void far hold_scrgr(void);
void far free_scrgr(void);
void far update_screen(void);
void far load_weapon(char id);
void far active_spells(unsigned char *spells);
void far pretty_panelagain(void);
void far send_FB(void);
void far player_look_shaft(void);
extern unsigned char RightPanel;
void far set_screen_frame(char which, int val);
char far load_weapcm(void);
extern char ShowStupidFirstPersonWeapon;
void far player_look_grave(int unused);  /* callers pass an argument it ignores */
void far adjust_dragons(int which);
void far flip_scale(unsigned char far *src, unsigned char far *dst, int frame);
void far flip_column(unsigned char far *src, unsigned char far *dst);

/* CREDITS.C: the credits */
void far show_credits(void);

/* FARDATA.ASM */
extern unsigned char far cmpbuf1_start[];
/* the digital effects' buffer (SOUND.C); name provisional */
extern unsigned char far seg_5DFD[];
/* The shared far work buffer (at least 10000h bytes), which each user lays out its own
   way: the archive tables (ARC.C), the LZSS work area (ACLZW.C), the pathfinder's
   squares (critter.h's STILES), the cutscene player's state (CUTS.C), the 3D view's pick
   buffer (INTERACT.C), panel pictures, chargen's skills table and screen copies.
   name: FM Towns has _grbuf and _panelbuf at the same address. */
extern unsigned char far stdat[];

/* Defined where no source has it yet: data the link takes from the EXE. */

/* SHOWPIC.C */
char far LoadBitMap_ovr141_0(int pal, char *name);

/* LPFDELTA.ASM */
void far seg002_A(unsigned char far *src, unsigned char far *dst);

/* MODEX.ASM */
void far seg015_1F9B_25A(int x, int y, int color);
void far seg015_1F9B_2A7(unsigned char far *src, unsigned dst, int w, int h);
void far seg015_1F9B_325(unsigned offset, int16 far *width, int16 far *height);
int far seg015_1F9B_366(unsigned char far *src, unsigned n, int unused);
void far seg015_1F9B_E8(char far *text);

#endif
