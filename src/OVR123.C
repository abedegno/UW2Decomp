/* target: ovr123 */
/* opts: -mm -1 -G -O -Y -d */
/* The rune bag and casting from runes: adding a runestone to the bag, drawing the bag,
   clicking runes onto the shelf, clicking an active spell, and casting the spell the
   shelf spells out. The whole of DOS overlay ovr123, in original order. Function and
   global names are the originals from the FM Towns symbol table; the source file's own
   name is not known.

   clear_runes (IDA ClearRuneBag_ovr123_64) is the FM Towns function between add_rune_
   and ShowRune_, and empties the bag the same way. */

#include <string.h>

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0x21];
    unsigned char skills[20];           /* 0x21, casting at 0x2A */
    char pad1[0x37 - 0x35];
    unsigned char play_mana;            /* 0x37 */
    char pad2[0x3D - 0x38];
    unsigned char level;                /* 0x3D */
    unsigned spells[3];                 /* 0x3E, class 0-3, subclass 4-7, stability 8-15 */
    unsigned char runebag[3];           /* 0x44, one bit per rune, high bit first */
    unsigned char shelf[3];             /* 0x47, the runes chosen for casting */
    char pad4[0x60 - 0x4A];
    unsigned b60:1;                     /* 0x60 */
    unsigned poison:4;
    unsigned active_spells:4;
    unsigned nrunes:2;                  /* word 0x61, bits 1..2 */
    unsigned b61_3:1;
    char pad62[0x305 - 0x62];
    unsigned char b305;                 /* 0x305 */
    char pad306[0x369 - 0x306];
    unsigned long game_clock;           /* 0x369 */
};

/* A mobile object. The first 8 bytes are shared with static objects. */
struct Object {
    unsigned id;                        /* item id in bits 0-8 */
    unsigned pos;
    unsigned qn;
    union {
        unsigned word;                  /* the head of the contents list */
        struct { unsigned owner:6, link:10; } f;
    } ol;
};

#define OBJ_ID(o)       ((o)->id & 0x1FF)

struct Inplist {
    int x;
    int y;
    char pad4[6 - 4];
    unsigned buttons;                   /* 0x06 */
};

/* One spell's runes, 4 bytes. */
struct Spell {
    unsigned char cls;                  /* class in bits 3-7 */
    int runes;                          /* the three runes, 5 bits each */
    unsigned char sub;
};

extern struct Player near *player;
extern struct Object far *ThePlayer;
extern struct Inplist near *inplist;
extern int GameInputMode;
extern int PlayerLevel;
extern char far Transparency;
extern struct Spell far spells[];
extern char mspell_mused;
/* This file's _BSS, DS:6A96 (ovr122's ends at 6A95): only this file uses it, and FM Towns
   keeps it unnamed, so it was static. */
static char dseg_67d6_6A96;     /* provisional */

void far Obj_Free(struct Object far *obj);
void far pic_to_screen(int pic, int x, int y, int w, int h);
void far mouse_hide(void);
void far mouse_show(void);
void far mouse_release(int n);
void far set_runes(unsigned char *shelf);
void far LookAt(struct Object far *obj, int lore);
void far scroll_print(char far *s);
void far game_sprint(int id);
char far * far get_string(int id);
void far parse_aspells(unsigned char *spells);
char far dispel_spell(int *which);
void far FixPlayerEquips(void);
unsigned char far play_effect_here(unsigned char fx, unsigned char pan, char vol);
int far skill_check(int value, int target);
char far do_spell(unsigned char cls, unsigned char sub, struct Object far *who,
                  struct Object far *target);

unsigned char spell_delay = 0;
unsigned long lstime = 0;

char far add_rune(struct Object far *obj)
{
    int rune;

    rune = OBJ_ID(obj) - 0xE8;
    if (rune < 0 || rune > 0x18)
        return 0;
    Obj_Free(obj);
    player->runebag[rune >> 3] = player->runebag[rune >> 3] | 1 << 7 - (rune & 7);
    return 1;
}

void far clear_runes(void)
{
    int i;

    for (i = 0; i < 8; i++)
        player->runebag[i] = 0;
}

void far ShowRune(int rune)
{
    pic_to_screen(rune + 0xE8, (rune & 3) * 0x12 + 0xF1, 0xBC - (rune >> 2) * 0xF,
                  ((rune & 3) + 1) * 0x12 + 0xEC, 0xBC - ((rune >> 2) + 1) * 0xF + 2);
}

void far RedispRune(void)
{
    int i;

    mouse_hide();
    for (i = 0; i < 0x18; i++)
        if (player->runebag[i >> 3] >> 7 - (i & 7) & 1) {
            Transparency = 1;
            ShowRune(i);
            Transparency = 0;
        }
    mouse_show();
}

void far clear_shelf(void)
{
    memset(player->shelf, 0x18, 3);
    player->nrunes = 0;
    set_runes(player->shelf);
}

void far mous_in_rune(void)
{
    int rune;
    struct Object obj;

    if (GameInputMode != 0)
        return;
    if (inplist->y < 0x12)
        clear_shelf();
    else {
        rune = 0x14 - ((inplist->y - 0x12) / 0xF << 2) + inplist->x / 0x12;
        if (player->runebag[rune >> 3] >> 7 - (rune & 7) & 1) {
            if (inplist->buttons & 2) {
                obj.ol.f.link = 0;
                obj.id = obj.id & 0xFE00 | (rune + 0xE8) & 0x1FF;
                obj.ol.f.owner = 0;
                LookAt(&obj, 0);
            } else {
                if (dseg_67d6_6A96)
                    clear_shelf();
                dseg_67d6_6A96 = 0;
                if (player->nrunes == 3) {
                    player->shelf[0] = player->shelf[1];
                    player->shelf[1] = player->shelf[2];
                    player->nrunes--;
                }
                player->shelf[player->nrunes++] = rune;
                set_runes(player->shelf);
            }
        }
    }
    mouse_release(1);
}

void far not_a_spell(void)
{
    scroll_print("Not a spell\n");
}

void far try_clear(void)
{
    int idx;
    unsigned char active[4];
    int stab;

    if (GameInputMode > 0 && GameInputMode < 4)
        return;
    idx = 2 - (inplist->x >> 4);
    if (player->active_spells > idx) {
        if (inplist->buttons & 2) {
            parse_aspells(active);
            scroll_print(get_string(active[idx] + 0x180 | 0xC00));
            stab = player->spells[idx] >> 8;
            if (stab <= 2)
                stab = 0;
            else if (stab <= 10)
                stab = 1;
            else
                stab = 2;
            game_sprint(stab + 0x97);
        } else if (dispel_spell(&idx))
            FixPlayerEquips();
        mouse_release(1);
    }
}

char far player_cast(unsigned char idx);

void far try_cast(int how)
{
    char i;
    int runes;

    if (GameInputMode != 0)
        return;
    if (player->b305 != 0)
        return;
    if ((inplist->buttons & 2) && how == 0) {
        mouse_release(1);
        return;
    }
    dseg_67d6_6A96 = 1;
    if (player->game_clock < lstime + spell_delay) {
        play_effect_here(0x15, 0x40, 0);
        game_sprint(0xB);
        mouse_release(1);
        return;
    }
    mouse_release(1);
    runes = (player->shelf[0] << 10) + (player->shelf[1] << 5) + player->shelf[2];
    for (i = 0; i < 0x40; i++)
        if (spells[i].runes == runes)
            break;
    if (i == 0x40) {
        not_a_spell();
        return;
    }
    player_cast(i);
}

char far fail_spell(int why)
{
    play_effect_here(0x16, 0x40, 0);
    game_sprint(why + 0xE1);
    return 0;
}

char far player_cast(unsigned char idx)
{
    unsigned char level;
    int sub;
    int cls;
    char sfx;
    unsigned char minor;

    level = idx / 8;
    level++;
    if (level > 8)
        level = 1;
    cls = (spells[idx].cls & 0xF8) >> 3;
    if ((PlayerLevel - 1) / 8 == 0 && level > 3)
        return fail_spell(3);
    if ((player->level + 1) / 2 < level)
        return fail_spell(0);
    if (level * 3 > player->play_mana)
        return fail_spell(1);
    if ((sub = skill_check(player->skills[9], level * 3)) == 0)
        return fail_spell(2);
    if (sub == -1) {
        game_sprint(0xE5);
        cls = 9;
        sub = level / 2;
    } else
        sub = spells[idx].sub;
    spell_delay = (level * 2 - player->level) * 4 + 0x80;
    lstime = player->game_clock;
    mspell_mused = level * 3;
    if (cls != 5 && cls != 7) {
        player->play_mana -= mspell_mused;
        mspell_mused = 0;
    }
    if (do_spell(cls, sub, ThePlayer, ThePlayer)) {
        minor = spells[idx].sub;
        switch (cls) {
        case 0:
        case 3:
            sfx = 0x2A;
            break;
        case 2:
            sfx = 0x2C;
            break;
        case 5:
            sfx = 0xFF;
            break;
        case 6:
            if (minor != 0x81)
                sfx = 0x2B;
            else
                sfx = 0x2A;
            break;
        case 7:
            switch (minor) {
            case 0:
            case 2:
            case 6:
            case 8:
                sfx = 0x2B;
                break;
            default:
                sfx = 0x2A;
                break;
            }
            break;
        case 8:
            if (minor == 5) {
                sfx = 0x29;
                break;
            }
        case 1:
        case 4:
        case 9:
        case 10:
        case 11:
            if (minor == 0xC) {
                sfx = 0x29;
                break;
            }
            if (minor == 9) {
                sfx = 0x28;
                break;
            }
        default:
            sfx = 0x10;
        }
        play_effect_here(sfx, 0x40, 0);
        return 1;
    }
    mspell_mused = 0;
    return fail_spell(3);
}
