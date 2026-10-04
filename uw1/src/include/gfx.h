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

/* The state of a cutscene, on show_anm's stack, 0x5C bytes. Field names are ours, from
   the code that uses them (CUTS.C); frame3B is the frame a skip or jump runs to, frame3D
   and frame3F the frame a pause is on and its length (in 256-tick units), repeat41 and
   repeat43 the loops left and the frame that loops, fade45 the speech file playing (-1
   none), fade47 and fade49 a pending fade-in and fade-out length (-1 none, -2 done). */
HOST_LAYOUT_BEGIN
struct CutsState {
    char name[0x13];                    /* CUTS\csXXX.nXX */
    int16 x, y, w, h;                   /* 0x13, the window; 320 by 200 for full screen */
    unsigned char windowed;             /* 0x1B */
    unsigned char far *palette;         /* 0x1C */
    char far *text_lines[6];            /* 0x20 */
    unsigned char color38;              /* 0x38 */
    int16 flag39;                       /* 0x39, subtitle lines to draw */
    int16 frame3B, frame3D;             /* 0x3B */
    uint16 frame3F;                     /* 0x3F, pause length */
    uint16 repeat41;                    /* 0x41, loops left */
    int16 repeat43;                     /* 0x43 */
    int16 fade45, fade47, fade49;       /* 0x45, speech playing, fade in, fade out */
    int16 file4B, file4D;               /* 0x4B */
    int16 vscr4F;                       /* 0x4F */
    int16 panx51, pany53, dir55, step57, remaining59; /* 0x51 */
    union {
        unsigned char value;
        struct { uint16 b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1; } bit;
    } flags;                            /* 0x5B: b0 skipping, b1 input arrived, b2 play on
                                           in this file, b3 play on in the cutscene, b4
                                           Escape allowed, b5 speech available, b6 speech
                                           playing, b7 the wait ignores keys */
};
HOST_LAYOUT_END

/* SPRITE.ASM */
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
void far seg003_0272_4A3A(int, int, int);
/* 0085:5025, puts back part of a saved area; no FM Towns counterpart is known. */
void far seg003_0272_5025(int handle, int x, int y, int w, int h, int sx, int sy);
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
void far vscreen_focus(int, int);
extern int16 far *wbot;  /* DS:21E0 */
extern int16 far *wleft;  /* DS:21E8 */
extern int16 far *wright;  /* DS:21E4 */
extern int16 far *wtop;  /* DS:21DC */
extern struct FontInfo far *cur_font;  /* DS:21CC */
extern unsigned char far *foreground_color;  /* DS:21C4 */
void far rectangle(int x, int y, int w, int h);
void far show(int x, int y, unsigned char far *bm, int a, int b, int c, int d);

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

/* GRSPIC.C: graphic resource lookup, decoding, cursor drawing and image scaling */
void far * far seg009_7(int icon);
void far seg009_73(int icon, int x, int y, int16 height, int16 width);
void far * far grs_unpack(void far *data);
void far grs_fbplot(int icon, int x, int y);
/* name: seg009 (1A6D): grs_which1 and pic_to_fbuf are FM Towns names, called the same
   way. FM Towns reads _grs_off[n] directly where DOS calls seg009_7, which maps the EMS
   page holding the cursor art first; it has no FM Towns name. */
int far grs_which1(int icon);
void far pic_to_screen(int icon, int x, int y, int height, int width);
void far seg009_2CC(int icon, NEARPTR width, NEARPTR height);
void far pic_to_fbuf(int icon, int x, int y);
void far mask_to_screen(int icon, int x, int y, int width, int height, int clip);
/* seg009, no FM Towns counterpart known: the first returns a segment for a picture
   number, the second takes a far pointer, two sizes and a count and returns one. */
unsigned far seg009_392(int index);
void far * far grs_scaledown(unsigned char far *source, int width, int height, int scale);

/* COLCYCLE.C: palette colour cycling */
void far rotate_bank(unsigned char first, unsigned char count, unsigned char up);
void far cycle_colors(unsigned char t);

/* Fonts for grfx_quikfont: the index into ovr118's font_suffixes, which names the file
   DATA\FONTxxxx.SYS. */
enum Font {
    FONT_4X5P,                          /* font4x5p.sys */
    FONT_5X6P,                          /* font5x6p.sys, the usual one */
    FONT_CHAR,                          /* fontchar.sys */
    FONT_BIG,                           /* fontbig.sys */
    FONT_5X6I,                          /* font5x6i.sys */
    FONT_BUTN                           /* fontbutn.sys */
};

/* Palettes of DATA\PALS.DAT (grfx_quikpal, read_quikpal), 0x300 bytes each. The uses
   name them; UW-Formats' file list gives UW1 the same numbering (blnkmap.byt palette 1,
   chargen.byt palette 3). */
#define PAL_GAME        0               /* the game's palette (main, the automap's exit) */
#define PAL_MAP         1               /* the automap (ovr094) */
#define PAL_CHARGEN     3               /* character creation (ovr101) */

/* GRFX.C: graphics start-up, fonts and palettes */
/* name: IDA OpenFont, ovr118. FM Towns game_stats calls a set_font_size_ wrapper here, but
   every other FM Towns call site, and the map's call-graph pairing, give grfx_quikfont_. */
void far grfx_quikfont(int n);
void far grfx_close(void);
void far grfx_setpal(void far *src);
void far fadein(unsigned char far *src, int count, int pump);
void far fadeout3d(int n);
void far fadein3d(int n);
void far fill_FB(int colour);
void far cameras_fade(void);
unsigned char far grfx_init(void);
void far grfx_clear(void);
unsigned char far read_quikpal(int n, void far *dest);
unsigned char far grfx_quikpal(int n);
void far grfx_palrange(void far *src, int start, int count);
void far fadeout(unsigned char far *src, int count, int pump);

/* LOADGR.C: art loading */
extern uint16 first_button;
extern uint16 first_tmobj;
extern uint16 first_vram;
/* match: declared before load_gr_ems: the two names have the same public-order key (404),
   and Turbo C lists such publics in reverse order of first sight, as the stub order needs */
unsigned char far load_tr_ems(char *art);
void far reload_gr_vpic(int offset, char *art, int image);
unsigned char far read_gr_far(char *art, int image, void far *dst);
int far load_all_gr(void);
void far load_doors(void);
extern unsigned char Palettes[32][16];
/* gronk_gr's callbacks: one returns where to load an image of the given size, the other
   moves a loaded image into place. */
typedef void far *(far *ArtAllocFn)(int size);
typedef unsigned char (far *ArtMoveFn)(void far *p, int size, int n);
unsigned char far gronk_gr(char *art, int start, int count, ArtAllocFn adr, ArtMoveFn move);

/* CUTS.C: the cutscene player */
int far cutsop_txt(uint16 far *code, struct CutsState *st);
int far cutsop_erase(uint16 far *code, struct CutsState *st);
int far cutsop_func(uint16 far *code, struct CutsState *st);
int far cutsop_pause(uint16 far *code, struct CutsState *st);
int far cutsop_next(uint16 far *code, struct CutsState *st);
int far cutsop_end(uint16 far *code, struct CutsState *st);
int far cutsop_loop(uint16 far *code, struct CutsState *st);
int far cutsop_data(uint16 far *code, struct CutsState *st);
int far cutsop_fadeout(uint16 far *code, struct CutsState *st);
int far cutsop_fadein(uint16 far *code, struct CutsState *st);
int far cutsop_jump(uint16 far *code, struct CutsState *st);
int far cutsop_punt(uint16 far *code, struct CutsState *st);
int far cutsop_say(uint16 far *code, struct CutsState *st);
int far cutsop_wait(uint16 far *code, struct CutsState *st);
/* match: cutsop_skip and cutsop_wait share a public-order key (875); Turbo C lists such
   publics in reverse order of first sight, and the stub order puts cutsop_skip first, so
   it is declared later */
int far cutsop_skip(uint16 far *code, struct CutsState *st);
int far cutsop_clang(uint16 far *code, struct CutsState *st);
int far cutsop_palrange(uint16 far *code, struct CutsState *st);
int far cutsop_palfade(uint16 far *code, struct CutsState *st);
int far cutsop_palset(uint16 far *code, struct CutsState *st);
int far cutsop_palsimplefade(uint16 far *code, struct CutsState *st);
int far cutsop_vscreen(uint16 far *code, struct CutsState *st);
int far cutsop_focus(uint16 far *code, struct CutsState *st);
int far cutsop_lback(uint16 far *code, struct CutsState *st);
int far cutsop_pan(uint16 far *code, struct CutsState *st);
int far cutsop_splity(uint16 far *code, struct CutsState *st);
int far cutsop_music(uint16 far *code, struct CutsState *st);
int far cutsop_test(uint16 far *code, struct CutsState *st);
int far cutsop_wait_for_sound(uint16 far *code, struct CutsState *st);
int far gobble_input_events(struct CutsState *st);
void far run_timebased_tasks(int reset);
void far punt_tasks(void);
void far punt_single_task(int task);
void far task_palfade(int task, int done);
void far remove_task(int task);
/* match: declared early so that it is seen before record_task: both names have the
   public-order key 970, and the stub order lists record_task first */
char far * far bufferPointer(void);
void far palette_fade(int step, int total, int first, int last, unsigned char far *pal);
int far virtual_screen(int w, int h, int split);
int far lback_vscreen(int x, int y, unsigned n);
void far show_anm(int cuts, int x, int y, int w, int h);
int far get_cut_banks(void);
unsigned char far * far get_cuts_block(int which);
void far free_cuts_ems(void);
void far anm_sound_callback(void);
void far init_cutscene(void);
/* A task install_timebased_task runs from the timer interrupt. */
typedef void (far *Task)(int task, int done);
int far install_timebased_task(Task fn, int period, int total);
void far runcutscene(unsigned n);

/* PANELS.C: the screen furniture around the 3D view */
extern unsigned char wframe[0x1F];
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
void far restore_sliding_panel(int redraw);
void far send_FB(void);
void far player_look_shaft(void);
extern unsigned char RightPanel;
void far set_screen_frame(char which, int val);
char far load_weapcm(void);
extern char ShowStupidFirstPersonWeapon;
void far player_look_grave(int unused);  /* callers pass an argument it ignores */

/* CREDITS.C: the credits */
void far show_credits(void);

/* FARDATA.ASM */
extern unsigned char far cmpbuf1_start[];
extern unsigned char far cmpbuf2_start[];
/* the digital effects' buffer (SOUND.C); name provisional */
extern char far dfx_buffer[];
extern uint32 far gr_offs[];
extern unsigned char far seg_5DFD[];
/* The shared far work buffer (at least 10000h bytes), which each user lays out its own
   way: the archive tables (ARC.C), the LZSS work area (ACLZW.C), the pathfinder's
   squares (critter.h's STILES), the cutscene player's state (CUTS.C), the 3D view's pick
   buffer (INTERACT.C), panel pictures, chargen's skills table and screen copies.
   name: FM Towns has _grbuf and _panelbuf at the same address. */
extern unsigned char far stdat[];

/* Defined where no source has it yet: data the link takes from the EXE. */
void far CallbackFunctionSleepRelated_seg021_22FD_CB7(int);
/* DS:34AA: the first EMS page of the sounds */
extern unsigned char far sound_fpage;

/* SHOWPIC.C */
char far display_screen(int pal, int blk);
char far disk_to_vid(int blk, char far *buf);
#endif
