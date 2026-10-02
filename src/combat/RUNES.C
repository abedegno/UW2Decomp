/* target: ovr123 */
/* opts: -mm -1 -G -O -Y -d */
/* The rune bag and casting from runes: adding a runestone to the bag, drawing the bag,
   clicking runes onto the shelf, clicking an active spell, and casting the spell the shelf
   spells out. The whole of DOS overlay ovr123, in original order.

   What it does in the game: the player's side of magic. Runestones picked up go into
   player->runebag (a bit per rune, add_rune); the rune panel shows them (RedispRune).
   Clicking a rune puts it on the three-place shelf (player->shelf, mous_in_rune); clicking
   the cast area looks the shelf up in the spells[] table (try_cast) and player_cast checks
   circle, mana and skill before handing the spell to do_spell in SPELLS.C. try_clear handles
   the three active spell icons: right click tells how stable a spell is, left click
   dispels it. Callers are the panel and mouse code (PANELS.C, INTERACT.C) and the
   inventory (picking up a runestone).

   Data owned: spell_delay and lstime (the time between casts), and a flag that clears the
   shelf on the next rune click after a cast.

   Function and global names are the originals from the FM Towns symbol table.
   clear_runes (IDA ClearRuneBag_ovr123_64) is the FM Towns function between add_rune_ and
   ShowRune_, and empties the bag the same way.
   Name: descriptive (the rune bag and casting from runes). */

#include <string.h>
#include "combat.h"
#include "gfx.h"
#include "object.h"
#include "player.h"
#include "ui.h"

extern struct Inplist near *inplist;
extern char far Transparency;
extern struct Spell far spells[];
/* This file's _BSS, DS:6A96 (ovr122's ends at 6A95). Set by try_cast, it makes the next
   rune click start a new shelf. */
/* name: only this file uses it, and FM Towns keeps it unnamed, so it was static. */
static char dseg_67d6_6A96;     /* provisional */

void far mouse_release(int n);
void far scroll_print(char far *s);
unsigned char far play_effect_here(unsigned char fx, unsigned char pan, char vol);

unsigned char spell_delay = 0;
unsigned long lstime = 0;

/* Puts a runestone into the rune bag: the object is freed and the rune's bit set (bit
   7 - (rune & 7) of runebag[rune / 8]). Returns 0 for an object that is not a runestone. */
char far add_rune(struct Object far *obj)
{
    int rune;

    rune = OBJ_ITEM(obj) - FIRST_RUNESTONE;
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
    pic_to_screen(rune + FIRST_RUNESTONE, (rune & 3) * 0x12 + 0xF1, 0xBC - (rune >> 2) * 0xF,
                  ((rune & 3) + 1) * 0x12 + 0xEC, 0xBC - ((rune >> 2) + 1) * 0xF + 2);
}

void far RedispRune(void)
{
    int i;

    mouse_hide();
    for (i = 0; i < NUM_RUNES; i++)
        if (player->runebag[i >> 3] >> 7 - (i & 7) & 1) {
            Transparency = 1;
            ShowRune(i);
            Transparency = 0;
        }
    mouse_show();
}

void far clear_shelf(void)
{
    memset(player->shelf, RUNE_NONE, 3);
    player->nrunes = 0;
    set_runes(player->shelf);
}

/* A click in the rune bag panel. A click in the strip with y < 0x12 clears the
   shelf. Otherwise the rune under the pointer (four to a row) is
   looked at on a right click, or added to the shelf on a left click: a fourth rune pushes
   the oldest off, and the first click after a cast starts a new shelf. */
void far mous_in_rune(void)
{
    int rune;
    struct StaticObj obj;

    if (GameInputMode != 0)
        return;
    if (inplist->y < 0x12)
        clear_shelf();
    else {
        rune = 0x14 - ((inplist->y - 0x12) / 0xF << 2) + inplist->x / 0x12;
        if (player->runebag[rune >> 3] >> 7 - (rune & 7) & 1) {
            if (inplist->cmd & 2) {
                obj.ol.f.link = 0;
                obj.id = obj.id & 0xFE00 | (rune + FIRST_RUNESTONE) & ID_ITEM;
                obj.ol.f.owner = 0;
                LookAt((struct Object far *)&obj, 0);
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

/* A click on one of the three active spell icons (index from the right). Right click
   prints the spell's name and how long it has left: the high byte of player->spells[idx]
   <= 2 is 'is nearly done', <= 10 'is unstable', more 'is stable'. Left click dispels it. */
void far try_clear(void)
{
    int idx;
    unsigned char active[4];
    int stab;

    if (GameInputMode > 0 && GameInputMode < 4)
        return;
    idx = 2 - (inplist->x >> 4);
    if (player->active_spells > idx) {
        if (inplist->cmd & 2) {
            parse_aspells(active);
            scroll_print(get_string(active[idx] + 0x180 | STR_SPELLS));
            stab = player->spells[idx] >> 8;
            if (stab <= 2)
                stab = 0;
            else if (stab <= 10)
                stab = 1;
            else
                stab = 2;
            game_sprint(stab + 0x97);  /* 0x97 'is nearly done', 0x98 'is unstable', 0x99 'is stable' */
        } else if (dispel_spell(&idx))
            FixPlayerEquips();
        mouse_release(1);
    }
}

/* Casts the spell on the shelf, if the player is not paralysed and not in another input
   mode. Too soon after the last cast (game_clock < lstime + spell_delay) it refuses with a
   sound and string 0xB. The three shelf runes, packed five bits each, are looked up in the
   64-entry spells[] table; no match prints 'Not a spell'. */
void far try_cast(int how)
{
    char i;
    int runes;

    if (GameInputMode != 0)
        return;
    if (player->paralyzed != 0)
        return;
    if ((inplist->cmd & 2) && how == 0) {
        mouse_release(1);
        return;
    }
    dseg_67d6_6A96 = 1;
    if (player->game_clock < lstime + spell_delay) {
        play_effect_here(0x15, 0x40, 0);
        game_sprint(0xB);  /* 'You are not yet ready to cast another spell.' */
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

/* Plays the fizzle sound and prints why a cast failed, string 0xE1 + why: 0 'You are not
   experienced enough to cast spells of that circle.', 1 'You do not have enough mana ...',
   2 'The incantation failed.', 3 'Casting was not successful.'. Returns 0. */
char far fail_spell(int why)
{
    play_effect_here(0x16, 0x40, 0);
    game_sprint(why + 0xE1);
    return 0;
}

/* The player casts spell idx (0..63; the circle is idx / 8 + 1). The checks, in order:
   on levels 1..8 (PlayerLevel's first world, Britannia, inferred) spells above circle 3
   fail outright; the player's experience level must be at least 2 * circle - 1; mana must
   be at least 3 * circle; and skill_check(Casting, 3 * circle) must not give 0 (failed) or
   -1 (backfire: the spell becomes SPELLC_BACKFIRE at strength circle / 2). The delay before
   the next cast is (2 * circle - level) * 4 + 128 ticks. Mana is paid now, except for missile
   and single-target area spells (SPELLC_MISSILE, SPELLC_1AREA), which keep the cost in
   mspell_mused until they are aimed and released (SPELLS.C). On success the casting sound
   depends on the spell class and minor; when do_spell refuses, the cost is waived and
   'Casting was not successful' printed. */
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
    if ((sub = skill_check(player->skills[SKILL_CASTING], level * 3)) == 0)
        return fail_spell(2);
    if (sub == -1) {
        game_sprint(0xE5);  /* 'The spell backfires.' */
        cls = SPELLC_BACKFIRE;
        sub = level / 2;
    } else
        sub = spells[idx].sub;
    spell_delay = (level * 2 - player->level) * 4 + 0x80;
    lstime = player->game_clock;
    mspell_mused = level * 3;
    if (cls != SPELLC_MISSILE && cls != SPELLC_1AREA) {
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
        case SPELLC_MISSILE:
            sfx = 0xFF;
            break;
        case SPELLC_AREA:
            if (minor != 0x81)
                sfx = 0x2B;
            else
                sfx = 0x2A;
            break;
        case SPELLC_1AREA:
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
        case SPELLC_CREATE:
            if (minor == 5) {
                sfx = 0x29;
                break;
            }
        case 1:
        case SPELLC_HEAL:
        case SPELLC_BACKFIRE:
        case SPELLC_MANA:
        case SPELLC_XT:
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
