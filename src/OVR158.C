/* target: ovr158 */
/* opts: -mm -1 -G -O -Y -d */
/* The character panel's statistics page: the name, class and level header, the three
   attributes, hit points, mana, experience, a scrolling list of six skills, and the click
   handler that scrolls it: the whole of DOS overlay ovr158, in original order. Function and
   global names are the originals from the FM Towns symbol table where it has them; the
   source file's own name is not known. */

#include <string.h>
#include <stdlib.h>

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char name[0x21];                    /* 0x00 */
    unsigned char skills[20];           /* 0x21 */
    char pad35[0x37 - 0x35];
    unsigned char play_mana;            /* 0x37 */
    unsigned char max_mana;             /* 0x38 */
    char pad39[0x3D - 0x39];
    unsigned char level;                /* 0x3D */
    char pad3E[0x4E - 0x3E];
    unsigned long exp;                  /* 0x4E, in tenths */
    unsigned char skill_points;         /* 0x52 */
    char pad53[0x64 - 0x53];
    unsigned lo64:13;                   /* 0x64 */
    unsigned pclass:3;
};

/* The player's critter data, reached through the near pointer `playerdat`. */
struct Critter {
    char pad0[4];
    unsigned char max_vit;              /* 0x04 */
    unsigned char attr[3];              /* 0x05: STR, DEX, INT */
};

struct Object {
    char pad0[8];
    unsigned char hp;                   /* 0x08 */
};

/* The mouse and keyboard state. */
struct Inplist {
    int x, y;                           /* 0x00 */
    char pad4[6 - 4];
    unsigned buttons;                   /* 0x06 */
    int mode;                           /* 0x08 */
};

/* The first skill shown in the list (DS:1B92). FM Towns has no name for it (it shows as
   _dtypes+0x1C there), so it was static; the name is ours. */
static unsigned char skill_top = 0;
/* Level ordinals, DS:1B93. DOS only (FM Towns prints no ordinal), so the name is ours. */
static char ordinals[4][3] = { "ST", "ND", "RD", "TH" };
/* Saved screen areas, DS:1B9F. FM Towns has 12 bytes here, three pointers. */
int spsave[3] = { 0, 0, 0 };

extern struct Player near *player;
extern struct Critter near *playerdat;
extern struct Object far *ThePlayer;
extern unsigned char far *foreground_color;
extern unsigned char far *background_color;
extern unsigned long far *Time;
extern struct Inplist near *inplist;
extern char RightPanel;

void far string_to_screen(char far *s, int x, int y);
int far string_width(char far *s);
char far * far get_string(int id);
/* 3265:0814, upper-cases a far string in place and returns it. FM Towns has no
   counterpart (its build is Japanese), so the name is the segment and offset. */
char far * far seg039_3452_814(char far *s);
/* 0085:5025, puts back part of a saved area; no FM Towns counterpart is known. */
void far seg003_0272_5025(int handle, int x, int y, int w, int h, int sx, int sy);
int far valloc(int w, int h);
void far save_rect(int handle, int x, int y, int w, int h);
void far grfx_quikfont(int size);
void far mouse_hide(void);
void far mouse_show(void);
int far mouse_get_input_sp(void);
int far mvcheck(int *val, int amount, int step, int dir);
void far pretty_panelagain(void);

void far sp_hdr(void)
{
    char n;
    char buf[30];

    strncpy(buf, player->name, 0xF);
    buf[0xF] = 0;
    strupr(buf);
    string_to_screen(buf, (0x48 - string_width(buf)) / 2 + 0xF0, 0xBD);
    string_to_screen(seg039_3452_814(get_string((player->pclass + 0x17) | 0x400)), 0xF0, 0xB6);
    itoa(player->level, buf, 10);
    n = player->level < 4 ? player->level - 1 : 3;
    strcat(buf, ordinals[n]);
    string_to_screen(buf, 0x135 - string_width(buf), 0xB6);
}

/* IDA's PrintPlayerAttribute_ovr158_F3. FM Towns has sp_att_ here, between sp_hdr_ and
   sp_hp_, and RedispStat_ calls it for 0 to 2 just as the DOS code calls this. */
void far sp_att(unsigned char i)
{
    char buf[4];

    itoa(playerdat->attr[i], buf, 10);
    string_to_screen(buf, 0x135 - string_width(buf), (2 - i) * 7 + 0xA1);
}

void far sp_hp(void)
{
    char buf[8];
    int n;

    itoa(ThePlayer->hp, buf, 10);
    n = strlen(buf);
    buf[n] = '/';
    n++;
    itoa(playerdat->max_vit, buf + n, 10);
    string_to_screen(buf, 0x135 - string_width(buf), 0x9A);
}

void far sp_mp(void)
{
    char buf[8];
    int n;

    itoa(player->play_mana, buf, 10);
    n = strlen(buf);
    buf[n] = '/';
    n++;
    itoa(player->max_mana, buf + n, 10);
    string_to_screen(buf, 0x135 - string_width(buf), 0x93);
}

void far sp_xp(void)
{
    char buf[10];

    ltoa(player->exp / 10, buf, 10);
    string_to_screen(buf, 0x135 - string_width(buf), 0x8C);
}

void far sp_skill(unsigned char i)
{
    char buf[4];
    char sk;

    if (i + skill_top > 0) {
        sk = i + skill_top - 1;
        itoa(player->skills[sk], buf, 10);
    } else {
        sk = 100;
        itoa(player->skill_points, buf, 10);
    }
    seg003_0272_5025(spsave[0], 0xF0, 0x84 - i * 7 + 1, 0x4C, (i + 1) * 7, 0, i * 7);
    string_to_screen(seg039_3452_814(get_string((sk + 0x1F) | 0x400)), 0xF0, 0x84 - i * 7);
    string_to_screen(buf, 0x135 - string_width(buf), 0x84 - i * 7);
}

void far RedispStat(void)
{
    unsigned char i;

    if (spsave[0] == 0) {
        spsave[0] = valloc(0x4C, 0x2A);
        if (spsave[0] != 0)
            save_rect(spsave[0], 0xF0, 0x85, 0x4B, 0x2B);
        spsave[1] = valloc(0x24, 0x15);
        if (spsave[1] != 0)
            save_rect(spsave[1], 0x112, 0x9A, 0x23, 0x15);
    }
    *foreground_color = *background_color = 0xC4;
    mouse_hide();
    grfx_quikfont(4);
    sp_hdr();
    for (i = 0; i < 3; i++)
        sp_att(i);
    sp_hp();
    sp_mp();
    sp_xp();
    *foreground_color = *background_color = 0xC9;
    for (i = 0; i < 6; i++)
        sp_skill(i);
    grfx_quikfont(1);
    mouse_show();
}

void far mous_in_stat(void)
{
    int v;
    unsigned long start;
    int key;
    int dir;

    grfx_quikfont(4);
    *foreground_color = *background_color = 0xC9;
    dir = (inplist->buttons & 1) ? -1 : 1;
    if (inplist->y < 10) {
        v = skill_top;
        dir = inplist->x < 0x26 ? -1 : 1;
        if (mvcheck(&v, dir == 1 ? 0xF : 0, 1, dir)) {
            skill_top = v;
            mouse_hide();
            for (v = 0; v < 6; v++)
                sp_skill(v);
            mouse_show();
        }
    }
    grfx_quikfont(1);
    start = *Time;
    do
        key = mouse_get_input_sp();
    while (*Time - start < 8 && key < 3 && (key & 3));
}

void far panel_check(void)
{
    if (RightPanel == 2 && inplist->mode == 1)
        pretty_panelagain();
}
