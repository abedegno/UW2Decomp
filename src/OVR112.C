/* target: ovr112 */
/* opts: -mm -1 -G -O -Y -d */
/* The program's main and its startup and shutdown: bringing up every subsystem in turn,
   the title screen, the screen dispatcher that switches between the 3D view, the map and
   conversations, resetting the game after death or a restore, moving the player to a new
   position or level, the divide-by-zero exit, and finding the home directory from the
   UWHOME environment variable. The whole of DOS overlay ovr112, in original order.
   Function and global names are the originals from the FM Towns symbol table where it
   has them; the source file's own name is not known.

   Five functions carry an IDA name in the target table and take the FM Towns name of the
   function at the same position, each confirmed by its body:
     ovr112_2AC  graceful_exit   both call busywaiting_new_options(quit_buttongroup)
     ovr112_2BB  editexit        both clear notdone
     MaybeSetupConversationUI_ovr112_448  free_demscr  demous_player, clear_gamedisp,
                                 hold_scrgr, and it is the 3D screen's exit in editor_dispatch
     ResetTimers_ovr112_45C  reset_times  clears nextSpellTime, lstime, watertime, nextstep
     ovr112_657  do_3d_view      both call establish_view
   FM Towns has nothing for ovr112_389, div_zero_ovr112_661 and ReadCfg_ovr112_839, which
   are DOS only. ovr112_389 keeps its IDA name; the other two provisional names were
   chosen for their keys: Turbo C lists a file's publics by the tools/bssorder.py key of
   each name and TLINK numbers overlay stub entries from the last one listed, so these names
   reproduce the EXE's stub order (the target table keeps IDA's names). */

#include <dos.h>
#include <dir.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0x62];
    unsigned b62_0:4;
    unsigned automap:1;                 /* word 0x62, bit 4 */
    unsigned b62_5:11;
    char pad64[0x306 - 0x64];
    unsigned char b306;                 /* 0x306 */
};

/* A mobile object. */
struct Object {
    unsigned id;
    unsigned pos;                       /* z 0-6, heading 7-9, y fine 10-12, x fine 13-15 */
    unsigned qn;
    unsigned ol;
    unsigned char hp;                   /* 0x08 */
    char pad09[0x18 - 0x09];
    unsigned char b18;                  /* 0x18, fine heading in bits 0-4 */
};

/* The mouse and keyboard state. */
struct Inplist {
    int x, y;
    char pad4[8 - 4];
    int mode;                           /* 0x08, the screen mode */
};

/* This file's uninitialised data, DS:5D1E to DS:5E0B. Turbo C lays _BSS out by its
   symbol table's order, not by declaration; with these names it gives exactly the EXE's
   layout. The array sizes are the gaps between neighbours. */
char HomeDir[0x42];                     /* the save directory, "<UWHOME>\SAVE0\" */
int scrmode;
/* The divide-by-zero vector found at startup, DS:5D62. DOS only, so the name is ours,
   chosen because it lands between scrmode and scrnum as the EXE has it. */
static void interrupt (far *int0_save)();
int scrnum;
int notdone;
char WorkPath[0x42];
int changed;
char CurDir[0x44];
char dungeonf[0x12];
int lastscrmode;
int NewPlayerX, NewPlayerY;
int NewPlayerLevel;

extern struct Player near *player;
extern struct Object far *ThePlayer;
extern struct Inplist near *inplist;
extern int GrSq;
extern int PlayerLevel;
extern int PlayerFacing, PlayerHeading, PlayerBank, PlayerPitch;
extern unsigned char combEfflen, tremEfflen, slidEfflen;
extern unsigned char pmouseHandled;
extern int LeftPanel;
extern int RightButtonThing;
extern int GameInputMode;
extern struct Object far *CursorObjPtr;
extern unsigned long nextSpellTime;
extern unsigned long lstime;
extern unsigned long watertime;
extern unsigned long nextstep;
extern int trap_teleport_data;
extern int curvrad;
extern unsigned char far *palette;
extern char quit_buttongroup[];
/* The shared work buffer. init_edit passes the paragraph after its start, which only
   references its segment; FM Towns has _grbuf and _panelbuf at the same address. */
extern char far stdat[];
/* The fatal-exit message: cPerror gets the offset of cExitMessage. */
extern int far *cPerror;
extern char far *cExitMessage;

/* The divide-by-zero trap in seg018 (assembly), and the two words in its code segment
   where it finds the stack to return to. DOS only; the names are ours. */
extern void interrupt far int0_trap();
extern unsigned far int0_ss;
extern unsigned far int0_sp;

/* Elsewhere in the game. Names not confirmed by the map come from FM Towns init_world,
   whose calls run in the same order where DOS has them: init_mem (DOS seg042's first
   function, before mem_setup), check_dirs and check_fds (the stub167 entries for ovr167_421
   and ovr167_489, which test the data directories and open the temporary files),
   init_debug (empty in FM Towns), init_ai, and init_cutscene (empty in both). grfx_init is
   ovr118_0, the FM Towns function before grfx_load_font. pfatal is FM pfatal_, which sets
   *cPerror from cExitMessage as ovr114_15D does. */
void far real_start(int how);
void far mainloop(void);
void far init_mem(void);
void far check_dirs(void);
void far check_fds(void);
void far init_strings(void);
void far Map_Init(void);
void far init_input(void);
void far init_debug(void);
void far init_sounds(void);
void far init_timers(void);
void far init_cutscene(void);
unsigned char far OkEnoughMem_ovr167_463(void);    /* enough memory free; DOS only */
void far punt_sound_stuff(int quiet);
void far first_punt(int code);
unsigned char far grfx_init(void);
unsigned char far display_screen(int pal, int blk);
void far load_new_music(int a, int b);
void far seg001_023B_C(void);          /* assembly, DOS only */
int far load_all_gr(void);
void far pfatal_code(int code);
int far init_mouse(void);
int far init_objects(void);
void far init_txtlib(void);
void far init_3d(void);
void far init_player(void);
void far init_ai(void);
void far init_lighting(void);
void far init_combinables(void);
unsigned char far init_save(void);
void far pfatal(char *msg);
int far init_babl(void);
void far grfx_clear(void);
void far grfx_quikpal(int pal);
void far grfx_close(void);
void far free_input(void);
void far free_mem(void);
void far free_scrgr(void);
void far free_timers(void);
void far free_sounds(void);
void far free_strings(void);
unsigned char far clear_dir(char *dir);
int far mouse_get_input(void);
void far show_cutscene(int n);
void far _input_addkey(int key, int a, int b, void (far *handler)());
void far save_screenshot(int seg);
void far busywaiting_new_options(char *group);
void far Map_ObjFix(void);
void far editchng(int bits);
void far mouse_hide(void);
void far mouse_show(void);
void far demous_player(void);
void far place_3d_view(int x, int y, int w, int h);
void far fadeout(unsigned char far *pal, int steps, int x);
void far fadein(unsigned char far *pal, int steps, int x);
void far init_gamedisp(void);
void far FixPlayerEquips(void);
void far render_FB(void);
void far send_FB(void);
unsigned char far read_quikpal(int which, unsigned char far *pal);
void far clear_gamedisp(void);
void far hold_scrgr(void);
void far punt_all_digi_fx(void);
void far load_digi_fx(int n);
void far Punt_player_inv(void);
void far set_drugged(int on);
void far reset_scrgr(void);
void far punt_fightmode(void);
void far unforce_mouse_cursor(int n);
void far attach_eye(int n);
void far mouse_freereign(void);
void far set_screen_frame(int which, int frame);
void far set_compass(void);
void far set_flask(int n);
void far scroll_clear(int n);
void far establish_view(void);
void far fadeout3d(int speed);
void far fadein3d(int speed);
unsigned char far ChangeLevel(int from, int to);
unsigned char far find_good_x_and_y(struct Object far *obj, int x, int y, int *nx, int *ny,
                                    int how);
void far player_setup(int x, int y, int how);
void far player_newsq(int sq);
void far seg016_1E73_2FCB(FILE *fp);   /* reads UW.CFG; DOS only */
unsigned char far copy_file(char *srcdir, char *dstdir, char *name);

/* The other screens' handlers, in other files. */
void far RedispInv(void);
void far check_physics(void);
void far display_scr(void);
void far update_screen(void);
void far AutoMap(void);
void far automap_scr(void);
void far ExitAutoMap(void);
void far strt_converse(void);
void far free_converse(void);

/* This file. */
void far init_world(int argc, char *argv[]);
void far free_world(char flag);
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

/* This file's data, DS:11F4 to DS:12C5, then its strings. */

/* For each screen, sixteen handlers that the main loop runs while the matching bit of
   `changed` is set; 0 starts the screen and 15 ends it. */
void (far *editor_dispatch[3][16])() = {
    { strt_demscr, do_3d_view, 0, (void (far *)())new_player_pos, 0, 0, 0, 0, 0,
      RedispInv, 0, check_physics, display_scr, update_screen, 0, free_demscr },
    { 0, AutoMap, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, automap_scr, 0, 0, ExitAutoMap },
    { strt_converse, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, free_converse }
};
int change_state[3] = { 0x3800, 0x1000, 0 };
char NewPlyFade = 3;
char in_game = 0;
unsigned _heaplen = 0x0C00;
unsigned _stklen = 0x1000;
unsigned _ovrbuffer = 0x300;
void (far *npp_func)() = 0;

int main(int argc, char *argv[])
{
    init_world(argc, argv);
    titlescr();
    real_start(1);
    in_game = 1;
    mainloop();
    free_world(1);
    return 0;
}

void far init_world(int argc, char *argv[])
{
    int err;

    int0_save = getvect(0);
    setvect(0, int0_trap);
    int0_ss = _SS;
    int0_sp = _SP;
    srand((unsigned)time(NULL));
    init_mem();
    check_dirs();
    check_fds();
    init_strings();
    Map_Init();
    init_input();
    init_debug();
    ReadCfg_ovr112_839();
    init_sounds();
    init_timers();
    init_cutscene();
    if (!OkEnoughMem_ovr167_463()) {
        punt_sound_stuff(1);
        if (!OkEnoughMem_ovr167_463())
            first_punt(0x1004);
    }
    if (!grfx_init())
        pfatal_code(0x3003);
    display_screen(5, 6);
    load_new_music(1, 1);
    seg001_023B_C();
    if ((err = load_all_gr()) != 0)
        pfatal_code(err);
    display_screen(6, 7);
    if (init_mouse() < 0)
        pfatal_code(2);
    if ((err = init_objects()) != 0)
        pfatal_code(err);
    init_txtlib();
    init_3d();
    init_player();
    init_ai();
    init_edit(argc, argv);
    init_lighting();
    init_combinables();
    if (!init_save())
        pfatal("Not enough disk space for save game.$");
    move_initial_files();
    if ((err = init_babl()) != 0)
        pfatal_code(err);
    grfx_clear();
    grfx_quikpal(0);
}

void far free_world(char flag)
{
    grfx_close();
    chdir(CurDir);
    free_input();
    free_mem();
    free_scrgr();
    free_timers();
    free_sounds();
    free_strings();
    if (flag)
        ;                               /* FM Towns saves the gamma setting here */
    clear_dir(HomeDir);
}

void far titlescr(void)
{
    while (mouse_get_input() > 3)
        ;
    show_cutscene(9);
}

void far init_edit(int argc, char *argv[])
{
    if (argc == 2)
        strcpy(dungeonf, strupr(argv[1]));
    else
        dungeonf[0] = 0;
    strcpy(WorkPath, "DATA\\");
    getcurdir(0, CurDir);
    _input_addkey(0x278, 0, 1, graceful_exit);
    _input_addkey(0x271, FP_SEG(stdat) + 1, 0xFF, save_screenshot);
    notdone = 1;
    changed = 0x7FFF;
    scrmode = 0;
    scrnum = -1;
    inplist->mode = 0;
}

void far graceful_exit(void)
{
    busywaiting_new_options(quit_buttongroup);
}

void far editexit(void)
{
    notdone = 0;
}

void far clearobj(void)
{
    Map_ObjFix();
    editchng(2);
    GrSq = -1;
}

void far change_screen(int mode)
{
    switch (scrmode = inplist->mode = mode) {
    case 2:
        scrnum = 1;
        break;
    case 4:
        scrnum = 2;
        break;
    case 1:
        scrnum = 0;
        break;
    default:
        scrnum = 0;
        break;
    }
}

void far newscr(int mode)
{
    if (scrnum != -1 && editor_dispatch[scrnum][15])
        editor_dispatch[scrnum][15]();
    if (mode >= 0)
        lastscrmode = scrmode;
    else
        mode = lastscrmode;
    change_screen(mode);
    if (editor_dispatch[scrnum][0])
        editor_dispatch[scrnum][0]();
    if (mode != 1)
        editchng(0x7FFE);
}

void far ovr112_389(int *bits)
{
    *bits = changed;
}

void far strt_demscr(void)
{
    unsigned char pal[0x300];

    mouse_hide();
    demous_player();
    place_3d_view(0x10, 0xB6, 0xD0, 0x80);
    movedata(FP_SEG(palette), FP_OFF(palette), FP_SEG((unsigned char far *)pal),
             FP_OFF((unsigned char far *)pal), 0x300);
    fadeout(pal, 2, 0);
    if (!display_screen(-1, 4))
        pfatal_code(0x300B);
    init_gamedisp();
    editchng(0x7DFE);
    FixPlayerEquips();
    render_FB();
    send_FB();
    mouse_show();
    read_quikpal(0, pal);
    fadein(pal, 2, 0);
}

void far free_demscr(void)
{
    demous_player();
    clear_gamedisp();
    hold_scrgr();
}

void far reset_times(void)
{
    lstime = nextSpellTime = 0;
    nextstep = watertime = 0;
}

void far reset_game(void)
{
    punt_all_digi_fx();
    load_digi_fx(1);
    load_digi_fx(2);
    load_digi_fx(0xB);
    reset_times();
    Punt_player_inv();
    FixPlayerEquips();
    ThePlayer->pos = ThePlayer->pos & 0xFC7F;
    ThePlayer->b18 = ThePlayer->b18 & 0xE0;
    PlayerPitch = PlayerBank = PlayerHeading = PlayerFacing = 0;
    combEfflen = tremEfflen = slidEfflen = 0;
    player->b306 = 0;
    player->automap = 1;
    set_drugged(0);
    reset_scrgr();
    lastscrmode = 0;
    punt_fightmode();
    LeftPanel = 2;
    if (RightButtonThing == 1 || RightButtonThing == 3 || RightButtonThing == 4)
        unforce_mouse_cursor(3);
    RightButtonThing = 0;
    if (GameInputMode != 0) {
        if (GameInputMode < 4) {
            unforce_mouse_cursor(3);
            CursorObjPtr = 0;
            GameInputMode = 0;
        } else {
            GameInputMode = 0;
            attach_eye(1);
        }
    }
}

void far real_death(int how)
{
    unsigned char pal[0x300];
    int scr;

    pmouseHandled = 0;
    mouse_freereign();
    while (mouse_get_input() > -1)
        ;
    if (how == 1) {
        set_screen_frame(2, 0);
        set_compass();
        set_flask(0);
        show_cutscene(0x103);
        scroll_clear(1);
    }
    editor_dispatch[scrnum][15]();
    scrmode = inplist->mode = 0;
    scr = scrnum;
    scrnum = -1;
    in_game = 0;
    if (how == 1)
        mouse_hide();
    reset_game();
    movedata(FP_SEG(palette), FP_OFF(palette), FP_SEG((unsigned char far *)pal),
             FP_OFF((unsigned char far *)pal), 0x300);
    fadeout(pal, 2, 0);
    in_game = 1;
    real_start(0);
    scrnum = scr;
    scrmode = 1 << scrnum;
    editor_dispatch[scrnum][0]();
}

void far do_3d_view(void)
{
    establish_view();
}

void far div_zero_ovr112_661(void)
{
    char msg[0x50];

    strcpy(msg, "Underworld exiting, divide by zero.\r\n$");
    *cPerror = (int)cExitMessage;
    movedata(FP_SEG((char far *)msg), FP_OFF((char far *)msg), FP_SEG(cExitMessage),
             FP_OFF(cExitMessage), strlen(msg));
    free_world(1);
    (*int0_save)();
}

unsigned char far new_player_pos(void)
{
    int x, y;

    if (NewPlayerX > 0) {
        if (NewPlyFade & 1) {
            render_FB();
            fadeout3d(curvrad);
        }
        if (PlayerLevel != NewPlayerLevel)
            if (!ChangeLevel(PlayerLevel, NewPlayerLevel))
                pfatal_code(0x300C);
        if (npp_func)
            npp_func();
        if (!find_good_x_and_y(ThePlayer, NewPlayerX, NewPlayerY, &x, &y, 0)
            && !find_good_x_and_y(ThePlayer, NewPlayerX, NewPlayerY, &x, &y, 1)) {
            NewPlayerX = 0;
            ThePlayer->hp = 0;
            trap_teleport_data = -1;
            return 0;
        }
        NewPlayerX = x;
        NewPlayerY = y;
        if (trap_teleport_data != -1) {
            player_setup(NewPlayerX, NewPlayerY, (trap_teleport_data & 1) - 1);
            if (trap_teleport_data & 0x20) {
                trap_teleport_data &= ~0x23;
                ThePlayer->pos = ThePlayer->pos & 0xFC7F | ((trap_teleport_data >> 2) & 7) << 7;
                PlayerHeading = PlayerFacing = trap_teleport_data << 11;
            }
            trap_teleport_data = -1;
        } else
            player_setup(NewPlayerX, NewPlayerY, -1);
        if (NewPlyFade & 2) {
            render_FB();
            fadein3d(curvrad);
        }
        player_newsq((NewPlayerY << 6) + NewPlayerX);
        NewPlayerX = 0;
        if (NewPlyFade & 2)
            editchng(0x7FFE);
    }
    return 1;
}

void far ReadCfg_ovr112_839(void)
{
    FILE *fp;
    char path[0x50];
    int i;
    register int j;

    for (i = 0; environ[i] != 0; i++) {
        if (strnicmp("UWHOME", environ[i], 6) == 0) {
            j = 6;
            while (environ[i][j++] != '=')
                ;
            while (environ[i][j++] == ' ')
                ;
            strcpy(HomeDir, environ[i] + --j);
            break;
        }
    }
    if (environ[i] == 0)
        strcpy(HomeDir, ".");
    if (strcmp(HomeDir, ".") == 0)
        strcpy(path, "DATA\\");
    else {
        strcpy(path, HomeDir);
        strcat(path, "\\");
    }
    strcat(path, "uw.cfg");
    fp = fopen(path, "r");
    if (fp != 0) {
        seg016_1E73_2FCB(fp);
        fclose(fp);
    }
    strcat(HomeDir, "\\SAVE0\\");
}

void far move_initial_files(void)
{
    copy_file("DATA\\", HomeDir, "lev.ark");
    copy_file("DATA\\", HomeDir, "scd.ark");
}
