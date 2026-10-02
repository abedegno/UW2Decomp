/* sys.h: The program's start-up, main loop and shutdown, memory and EMS, errors, small
   helpers, debugging hooks, and the C-like helpers in seg017. */
#ifndef SYS_H
#define SYS_H

#include "uw2.h"

struct Camera;

#include "view3d.h"

/* OVR112.C: the program's main and its startup and shutdown */
extern char HomeDir[0x42];
/* DS:5D60, FM Towns _scrmode (IDA's label "InGameMode" here is wrong: a different global,
   DS:2506, exists under that name already). */
extern int scrmode;
extern int scrnum;
extern int notdone;
extern int changed;
extern int NewPlayerX;
extern int NewPlayerY;
extern int NewPlayerLevel;
void far init_world(int argc, char *argv[]);
void far titlescr(void);
void far init_edit(int argc, char *argv[]);
void far graceful_exit(void);
void far change_screen(int mode);
void far strt_demscr(void);
void far free_demscr(void);
void far reset_times(void);
void far reset_game(void);
void far do_3d_view(void);
unsigned char far new_player_pos(void);
void far ReadCfg_ovr112_839(void);
void far move_initial_files(void);
extern void (far *editor_dispatch[3][16])();
extern int change_state[3];
extern char NewPlyFade;
void far newscr(int mode);
void far real_death(int how);

/* SEG012.C: the main loop and the per-screen change dispatcher */
extern unsigned char dsfx_playing;
void far do_changes(void);
void far mainloop(void);
void far editchng(int bits);

/* SEG013.C: EMS (LIM expanded memory) driver calls */
/* No FM Towns counterpart (FM Towns has no EMS): the segment of the EMS page frame, set
   by seg013 from INT 67h function 41h. Provisional name. */
extern unsigned ems_frame;
void far seg013_1D3C_138(unsigned handle, char far *name);
int far seg013_1D3C_A(unsigned min_pages, unsigned max_pages);
void far seg013_1D3C_B2(void);

/* SEG042.C: memory and the workspace */
/* DOS A5 is FM Towns mem_setup: both initialise the page counts and invalidate mappings.
   The IDA name is kept as the public symbol until its target-table entry is renamed. */
void far mem_setup(int page);
void far seg042_35ED_12B(void);
void far init_mem(void);
void far free_mem(void);
/* DOS only: maps the critter animation pages into the EMS frame. Provisional name. */
void far map_crit_pages(void);
int far set_workspace(void);
void far release_workspace(void);

/* OVR114.C: error reporting and fatal exit routines */
void far first_punt(int code);
void far pfatal_code(int code);
void far pfatal(char *message);

/* SEG041.C: small helpers */
int far mvcheck(int *val, int limit, int step, int dir);
void far move_along(int heading, int dist, int *x, int *y);
int far rollem(int dice, int sides);

/* OVR109.C: debugging hooks */
void far init_debug(void);

/* SEG017.ASM */
void far DRAW_RELATED_seg017_2179_2A2();
void far DRAW_RELATED_seg017_2179_320(unsigned offset, int far *width, int far *height);
void far DRAW_RELATED_seg017_2179_361();
char far * far FindStringDelimiter(char far *s, int c);
void far PrintStringToConsole_seg017_DE(char far *text);
void far gr_pixel(int x, int y, int color);  /* provisional: 1F8C:0255 */
void far grab(void far *dst, int x, int y, int w, int h);
void far local_do_palette(int count, unsigned char first);
void far mem_set(void far *p, int value, int count);  /* seg017's far memset; our name */
int far str_cmp(char far *a, char far *b);
char far * far str_copy(char far *dst, char far *src);
/* Far string routines (seg017 and seg039). */
int far str_len(char far *s);
void far str_ncopy(char far *dst, char far *src, int n);
char far * far str_str(char far *s, char far *find);

/* SEG018.ASM */
extern unsigned far int0_sp;
extern unsigned far int0_ss;
/* The divide-by-zero trap in seg018 (assembly), and the two words in its code segment
   where it finds the stack to return to. DOS only; the names are ours. */
void interrupt far int0_trap();

/* SEG021M.ASM */
extern unsigned char far *Alt;  /* DS:2130 */
extern unsigned char far *Asc;  /* DS:2148, FM Towns _Asc */
/* DS:212C, a far pointer to the keyboard handler's caps lock state (it sets the LEDs from
   it). FM Towns has no counterpart, so the name is ours. */
extern unsigned char far *CapsLock;  /* DS:212C */
extern unsigned char far *Ctrl;  /* DS:2134 */
extern int far *MouseDx;  /* DS:214C, FM Towns _MouseDx */
extern int far *MouseDy;  /* DS:2150, FM Towns _MouseDy */
extern int far *MouseOn;  /* DS:2154, FM Towns _MouseOn */
extern unsigned char far *Shift;  /* DS:2128 */
extern char far *cExitMessage;
extern int far *cJoyInit;
/* The fatal-exit message: cPerror gets the offset of cExitMessage. */
extern int far *cPerror;
extern int far *joy_buttons;
extern int far *joy_position;
/* The asm input module (2110): FM Towns key_, mouse_ and mbuttons. */
int far key(void);
extern unsigned char far *key_on;  /* DS:2138 */
int far mbuttons(void);
void far mouse(void);
void far seg021_22FD_755(void);
void far seg021_22FD_791(void);
void far seg021_22FD_7CD(void);
void far seg021_22FD_809(void);

/* SEG021Q.ASM */
void far Callback_seg021_22FD_CEA(int);
int far cAtan2(int x, int y);
extern int far *cDbase;  /* DS:216C, start of the bytecode buffer */
extern int far *cDbbase;  /* DS:2180, where gr_entry records dbptr */
extern int far *cEntryStrt;  /* DS:2170 */
void far cFBtoScreen(void);
void far cFillFB(int);
void far * far cFrmtoRaw(void far *data, unsigned char far *pal, unsigned char mode);
void far cFstSinCos(int angle, int *a, int *b);
void far cInit3d(void);
extern unsigned char far *cLightTabs;
extern int far *cPixXferStuff;
void far cPlaceFB(int x, int y, int w, int h);
extern struct Camera far *cPlayer;
void far cRender(void);
void far cSinCos(int angle, int *x, int *y);
int far cSqRt(long v);
void far cZoom(unsigned zoom);

/* OVR165.C: screen changes */
/* Declared before the rest of its file because TLINK numbers the overlay's stub entries in
   the order Turbo C lists the publics, which for names with the same hash key is the order
   they were first seen: the EXE's stub has ovr165_E before ovr165_0. */
void far ovr165_E(void);

/* SETPNT.ASM */
int far SetPnt(char x, char y, char z);

/* OVR158.C: the character panel's statistics page */
extern int spsave[3];  /* DS:1B9F; FM Towns reads _spsave+4 */
void far sp_hp(void);
void far sp_mp(void);
void far sp_xp(void);
void far RedispStat(void);
void far mous_in_stat(void);
void far panel_check(void);

/* Defined where no source has it yet: data the link takes from the EXE. */
void far stub112_25(int code);

#endif
