/* target: ovr149 */
/* opts: -mm -1 -G -O -Y -d */
/* Saving and restoring games, and changing level: the save directory and its four slots,
   copying a slot to and from the working directory, loading and saving one level's map,
   and the special cases some levels need on arrival or departure. The whole of DOS
   overlay ovr149, in original order. Function and global names are the originals from
   the FM Towns symbol table; the source file's own name is not known.

   init_save (IDA MaybeCreateSaveGameFolder_ovr149_0) is the FM Towns function just before
   GetLevel_: both make the working directory, empty it with clear_dir and then require
   1200 bytes free on the disk. */

#include <string.h>
#include <stdio.h>
#include <dir.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <stat.h>                       /* sys\stat.h; the build keeps it flat */

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0x60];
    unsigned b60:1;                     /* 0x60 */
    unsigned b60_1:10;
    unsigned b60_11:1;                  /* word 0x60, bit 11 */
    unsigned b60_12:4;
    unsigned b62_0:4;
    unsigned automap:1;                 /* word 0x62, bit 4 */
    unsigned b62_5:11;
    char pad64[0x6A - 0x64];
    unsigned long l6A;                  /* 0x6A */
    char pad6E[0x96 - 0x6E];
    unsigned long quests[2];            /* 0x96 */
    char pad9E[0x2F9 - 0x9E];
    int saved_automap;                  /* 0x2F9 */
};

/* A mobile object. The first 8 bytes are shared with static objects. */
struct Object {
    unsigned id;
    unsigned pos;
    unsigned qn;
    unsigned ol;                        /* 0x06, the head of the contents list */
    char pad08[0x12 - 0x08];
    unsigned char last_hit;             /* 0x12 */
};

extern char HomeDir[];
extern struct Player near *player;
extern struct Object far *ThePlayer;
extern int PlayerLevel;
extern int GrSq;
extern char scroll_esc;
extern unsigned char realDScheck;
extern unsigned char fiz_update;
extern int GameInputMode;
extern struct Object far *CursorObjPtr;
extern int NewPlayerX, NewPlayerY;

/* strrchr, named after its address in seg005 */
char far * far FindInString_seg005_105F_103F(char *s, char c);
#define str_rchr(s, c) ((char *)FindInString_seg005_105F_103F(s, c))

char far SavePlayerInv(char *dir);
char far RestorePlayerInv(char *dir);
void far FreePlayerInv(unsigned far *list);
void far change_GrSq(int sq, int how);
unsigned char far Map_Load(int arc, int level, int flags);
char far Map_Save(int arc, int level, int flags);
void far Txm_Load(int arc, int level, int flags);
char far Txm_Save(int arc, int level, int flags);
void far load_dl(void);
void far ClearAutoMap(void);
void far init_level_creature_stuff(void);
void far GetAutoMapLevel(int arc, int level);
char far SaveAutoMapLevel(int arc, int level);
void far close_arc(int arc);
void far game_sprint(int id);
void far scroll_print(char far *s);
void far scroll_clear(char redraw);
void far pretty_panelagain(void);
void far load_weapcm(void);
void far display_scr(void);
void far init_scrgr(void);
void far newFPS(int n);
void far parse_effect(void);
void far editchng(int bits);
void far reset_game(void);
void far set_creatures_from_saved_game(void);
void far set_creatures_to_saved_game(void);
int far wdialog(char *prompt, char *initial, char *result, char anychar, int maxlen);
unsigned char far blttodrive(char far *src, char *path, int len);
unsigned far get_workspace(void);
void far release_workspace(void);
int far ReadFileToAddress(int fd, void far *buf, unsigned n);
int far FileWriteWithParams(int fd, void far *buf, unsigned n);
void far clear_fight_state(void);
void far unforce_mouse_cursor(int n);
void far restore_mana(struct Object far *who, char amount);
void far Sched_SetAllClocks(int n);
struct Object far * far Obj_FindInMapSquare(int major, int minor, int index, int x, int y);
int far checkLock(struct Object far *who, struct Object far *obj, int key);
void far OpenDoor(struct Object far *who, struct Object far *door);
int far find_anim();
void far toast_animobj(int idx, int how);
void far punt_all_digi_fx(void);
void far load_digi_fx(int which);
void far clearobj(int n);
void far set_random_walking_music(int which);
void far update_all_critters_whilst_player_snoozes(void);
void far prison_alarm_check(void);
void far fire_trigger_at(int x, int y);
void far Killorn_just_crashed(int how);
void far remove_TK_wand(void);
void far genocide(int n);

unsigned char far clear_dir(char *dir);
unsigned char far copy_dir(char *src, char *dst);
void far do_level_hacks(int level, int mode);

/* The tests in SaveLevel and copy_file have empty bodies: the bytes keep each test with no jump after it,
   as if a debugging message had been compiled out. */
#define complain(what)

char far init_save(void)
{
    long space;
    struct dfree df;
    char dir[66];

    strcpy(dir, HomeDir);
    dir[strlen(dir) - 1] = 0;
    mkdir(dir);
    clear_dir(HomeDir);
    getdfree(0, &df);
    if (df.df_sclus != 0xFFFF) {
        space = (long)df.df_avail * df.df_bsec * df.df_sclus;
        if (space < 0x4B0)
            return 0;
    } else
        return 0;
    return 1;
}

int far GetLevel(int level)
{
    int sq;
    int ok = 1;
    int flags = 2;

    SavePlayerInv(0);
    sq = GrSq;
    change_GrSq(-1, -1);
    if (!Map_Load(0, level, flags))
        ok = 0;
    RestorePlayerInv(0);
    if (ok > 0) {
        PlayerLevel = level;
        load_dl();
        Txm_Load(0, level, flags | 4);
        ClearAutoMap();
        init_level_creature_stuff();
        if (ok == 1)
            GetAutoMapLevel(4, level);
        close_arc(1);
    }
    change_GrSq(sq, -1);
    return ok;
}

char far SaveLevel(int level)
{
    char ok;
    char saved;
    int sq;

    SavePlayerInv(0);
    FreePlayerInv(&ThePlayer->ol);
    sq = GrSq;
    change_GrSq(-1, -1);
    ThePlayer->id = ThePlayer->id & 0xFE3F;
    if ((saved = Map_Save(0, level, 2)) == 0)
        complain("map");
    ok = saved;
    if (ok && (ok = Txm_Save(0, level, 6)) == 0)
        complain("textures");
    if (ok && (ok = SaveAutoMapLevel(4, level)) == 0)
        complain("automap");
    if (saved)
        close_arc(1);
    RestorePlayerInv(0);
    change_GrSq(sq, -1);
    return ok;
}

void far get_save_descs(char descs[][40], int *found)
{
    int i;
    char *num;
    char *d;
    char path[66];
    FILE *fp;

    strcpy(path, HomeDir);
    num = str_rchr(path, '0');
    strcat(path, "desc");
    *found = 0;
    for (i = 0; i < 4; i++) {
        *num = i + '1';
        if (access(path, 0) >= 0 && (fp = fopen(path, "r")) != 0) {
            d = descs[i];
            fgets(d, 39, fp);
            *found |= 1 << i;
            fclose(fp);
        }
        if (!(*found & (1 << i)))
            strcpy(descs[i], "<not used yet>");
    }
}

void far ShowSaveRest(void)
{
    int found;
    char *labels[4] = { "I- ", "II- ", "III- ", "IV- " };
    char descs[4][40];
    int i;

    scroll_clear(1);
    get_save_descs(descs, &found);
    game_sprint(0x120);
    scroll_esc = 0;
    for (i = 0; i < 4; i++) {
        scroll_print("\n");
        scroll_print(labels[i]);
        scroll_print(descs[i]);
    }
    scroll_esc = 1;
    scroll_print("\\0");
}

char far RestoreGame();
int far SaveGame();

void far DoSaveRest(int restore, int slot)
{
    int found;
    char descs[4][40];
    int msg;

    scroll_clear(0);
    get_save_descs(descs, &found);
    if (restore) {
        if (!(found & (1 << (slot - 1)))) {
            msg = 1;
            pretty_panelagain();
        } else if (RestoreGame(slot)) {
            load_weapcm();
            msg = 2;
            realDScheck = 0;
            display_scr();
            realDScheck = 1;
            init_scrgr();
            newFPS(-1);
            parse_effect();
            fiz_update = 1;
            editchng(0x7FFE);
        } else {
            msg = 3;
            pretty_panelagain();
        }
    } else
        msg = SaveGame(slot, descs[slot - 1]) + 4;
    game_sprint(msg + 0xAE);
}

char far RestoreGame(char slot)
{
    char path[66];
    char *p;

    strcpy(path, HomeDir);
    p = str_rchr(path, '0');
    *p = slot + '0';
    game_sprint(0xB5);
    if (clear_dir(HomeDir)) {
        game_sprint(0xB9);
        if (copy_dir(path, HomeDir)) {
            game_sprint(0xB9);
            reset_game();
            if (RestorePlayerInv(HomeDir)) {
                game_sprint(0xB9);
                if (GetLevel(PlayerLevel)) {
                    do_level_hacks(PlayerLevel, 3);
                    game_sprint(0xB9);
                    set_creatures_from_saved_game();
                    return 1;
                }
            }
        }
    }
    scroll_print("\n");
    return 0;
}

int far SaveGame(char slot, char *desc)
{
    int ret = 0;
    char path[66];
    char *p;

    set_creatures_to_saved_game();
    strcpy(path, HomeDir);
    scroll_clear(1);
    game_sprint(0xB7);
    if (wdialog(0, desc, desc, 1, 0x1E) == 0x1B) {
        ret = 2;
        goto fail;
    }
    if (strlen(desc) == 0)
        strcpy(desc, " ");
    scroll_print("\n");
    game_sprint(0xB6);
    strcat(path, "desc");
    if (!blttodrive(desc, path, strlen(desc)))
        goto fail;
    p = str_rchr(path, '0');
    *p = slot + '0';
    p[1] = 0;
    if (access(path, 0) < 0 && mkdir(path) < 0)
        goto fail;
    strcat(path, "\\");
    if (!clear_dir(path))
        goto fail;
    game_sprint(0xB9);
    if (SavePlayerInv(HomeDir)) {
        game_sprint(0xB9);
        if (SaveLevel(PlayerLevel)) {
            game_sprint(0xB9);
            if (copy_dir(HomeDir, path)) {
                scroll_print("\\0");
                scroll_clear(1);
                return 1;
            }
        }
    }
fail:
    scroll_clear(1);
    return ret;
}

unsigned char far clear_dir(char *dir)
{
    char path[66];
    struct ffblk ff;
    char *end;
    int r;

    strcpy(path, dir);
    end = path + strlen(path);
    strcat(path, "*.*");
    for (r = findfirst(path, &ff, 0); r == 0; r = findnext(&ff)) {
        strcpy(end, ff.ff_name);
        if (unlink(path) != 0)
            return 0;
    }
    return 1;
}

unsigned char far copy_file(char *srcdir, char *dstdir, char *name)
{
    int out;
    unsigned ws;
    char far *buf;
    char ok;
    char src[66];
    char dst[66];
    int in;
    unsigned n;

    in = -1;
    out = -1;
    ok = 0;
    strcpy(src, srcdir);
    strcat(src, name);
    strcpy(dst, dstdir);
    strcat(dst, name);
    in = open(src, O_RDONLY | O_BINARY);
    if (in < 0)
        return 0;
    out = open(dst, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, S_IREAD | S_IWRITE);
    if (out >= 0) {
        lseek(in, 0L, SEEK_SET);
        if ((ws = get_workspace()) == 0)
            complain("no workspace");
        else {
            buf = MK_FP(ws, 0);
            ok = 1;
            do {
                n = ReadFileToAddress(in, buf, 0xF000);
                if (FileWriteWithParams(out, buf, n) != n) {
                    ok = 0;
                    break;
                }
            } while (n == 0xF000);
            release_workspace();
        }
    }
    if (in >= 0)
        close(in);
    if (out >= 0)
        close(out);
    return ok;
}

unsigned char far copy_dir(char *src, char *dst)
{
    char path[66];
    struct ffblk ff;
    int r;

    strcpy(path, src);
    strcat(path, "*.*");
    for (r = findfirst(path, &ff, 0); r == 0; r = findnext(&ff))
        if (!copy_file(src, dst, ff.ff_name))
            break;
    if (r == 0)
        return 0;
    return 1;
}

char far ChangeLevel(int from, int to)
{
    char ok;

    clear_fight_state();
    if (GameInputMode == 2 && CursorObjPtr != 0) {
        GameInputMode = 0;
        CursorObjPtr = 0;
        unforce_mouse_cursor(3);
    }
    do_level_hacks(from, 1);
    if ((ok = SaveLevel(from)) != 0) {
        if ((ok = GetLevel(to)) != 0) {
            do_level_hacks(to, 0);
            restore_mana(ThePlayer, 0);
            Sched_SetAllClocks(1);
        } else
            return 0;
    }
    return ok;
}

void far gruesome_door_hack(int x, int y)
{
    struct Object far *door;
    int anim;

    door = Obj_FindInMapSquare(5, 0, -1, x, y);
    if (door != 0) {
        checkLock(0L, door, -0x2D);
        OpenDoor(0L, door);
        anim = find_anim(door);
        if (anim >= 0)
            toast_animobj(find_anim(door), 5);
    }
}

void far do_level_hacks(int level, int mode)
{
    punt_all_digi_fx();
    load_digi_fx(1);
    load_digi_fx(2);
    load_digi_fx(0xB);
    if (player->b60_11 && mode == 0) {
        clearobj(0);
        return;
    }
    if (mode == 0) {
        init_level_creature_stuff();
        if (!player->b60)
            set_random_walking_music(-2);
    } else if (mode == 1)
        update_all_critters_whilst_player_snoozes();
    if (level == 1 && mode == 0) {
        ThePlayer->last_hit = 0;
        gruesome_door_hack(NewPlayerX, NewPlayerY);
    }
    switch ((level - 1) / 8) {
    case 1:
        if (mode == 1) {
            prison_alarm_check();
            if ((level - 1) % 8 + 1 == 2) {
                fire_trigger_at(0x1F, 0x24);
                fire_trigger_at(0x23, 0x22);
            }
        }
        break;
    case 2:
        if ((int)((player->quests[0] & 4) >> 2) && mode == 1 && (level - 1) % 8 + 1 == 1)
            Killorn_just_crashed(1);
    case 5:
        if ((level - 1) % 8 + 1 == 3 && mode == 1)
            remove_TK_wand();
        break;
    case 6:
        if (mode == 0 && (int)((player->l6A & 8) >> 3))
            genocide(0xFF);
        break;
    case 8:
        if (mode == 0) {
            player->saved_automap = player->automap;
            player->automap = 0;
        }
        if (mode == 1)
            player->automap = player->saved_automap;
        break;
    }
}
