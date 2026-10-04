/* target: ovr119 */
/* opts: -mm -1 -G -O -Y -d */
/* The rune bag and casting from runes: adding a runestone to the bag, drawing the bag,
   clicking runes onto the shelf, clicking an active spell, and casting the spell the shelf
   spells out. The whole of DOS overlay ovr119, in original order. Seeded from UW2Decomp's
   src/combat/RUNES.C (UW2's ovr123). Function and global names are UW2's (the FM Towns
   originals), the routines being the same; UW1 has no symbols of its own. The overlay's
   stub order agrees with these names (bssorder's keys).

   What it does in the game: the player's side of magic. Runestones picked up go into
   player->runebag (a bit per rune, add_rune); the rune panel shows them (RedispRune).
   Clicking a rune puts it on the three-place shelf (player->shelf, mous_in_rune); clicking
   the cast area looks the shelf up in the spells[] table (try_cast) and player_cast checks
   circle, mana and skill before handing the spell to do_spell in SPELLS.C. try_clear handles
   the three active spell icons: right click tells how stable a spell is, left click
   dispels it.

   UW1 against UW2: 48 spells of six to a circle (UW2 64, eight), no paralysis test and no
   message when casting too soon, no circle limit for the first world, the skill check is
   Casting + 5 against 2 * circle, the delay is (2 * circle - level) * 4 + 0x40, only
   missile spells keep their cost unpaid, and every successful cast plays effect 0x10. The
   panel's rune positions and the strings differ, and struct Player differs (Player1Runes
   below).

   Data owned: spell_delay and lstime (the time between casts), and a flag that clears the
   shelf on the next rune click after a cast.
   Name: descriptive (the rune bag and casting from runes). */

#include <string.h>
#include "combat.h"
#include "gfx.h"
#include "object.h"
#include "player.h"
#include "sound.h"
#include "ui.h"

/* Declared in each file that uses it, its own way (no header). */
extern struct Spell far spells[NUM_SPELLS];

/* This file's _BSS, DS:5A90. Set by try_cast, it makes the next rune click start a new
   shelf. */
/* name: the listing's. Only this file uses it, and FM Towns keeps UW2's unnamed, so it was
   static. */
static char dseg_5c99_5A90;     /* provisional */

/* This file's _DATA, DS:16E2..16F3 with the literal pool. */
unsigned char spell_delay = 0;
uint32 lstime = 0;

/* Puts a runestone into the rune bag: the object is freed and the rune's bit set (bit
   7 - (rune & 7) of runebag[rune / 8]). Returns 0 for an object that is not a runestone. */
char far add_rune(struct Object far *obj)
{
    int rune;

    rune = OBJ_ITEM(obj) - FIRST_RUNESTONE;
    if (rune < 0 || rune > NUM_RUNES)
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
    mouse_hide();
    pic_to_screen(rune + FIRST_RUNESTONE, (rune & 3) * 0x12 + 0xF4, 0xBB - (rune >> 2) * 0xF,
                  ((rune & 3) + 1) * 0x12 + 0xEF, 0xBB - ((rune >> 2) + 1) * 0xF + 2);
    mouse_show();
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
        rune = 0x14 - ((inplist->y - 0x12) / 0xF << 2) + (inplist->x - 3) / 0x12;
        if (player->runebag[rune >> 3] >> 7 - (rune & 7) & 1) {
            if (inplist->cmd & 2) {
                obj.ol.f.link = 0;
                SET_ITEM(&obj, rune + FIRST_RUNESTONE);
                obj.ol.f.owner = 0;
                LookAt((struct Object far *)&obj, 0);
            } else {
                if (dseg_5c99_5A90)
                    clear_shelf();
                dseg_5c99_5A90 = 0;
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
    int16 idx;
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
            game_sprint(stab + 0x89);  /* 0x89 'is nearly done', 0x8A 'is unstable', 0x8B 'is stable' */
        } else if (dispel_spell(&idx))
            FixPlayerEquips();
        mouse_release(1);
    }
}

/* Casts the spell on the shelf, if the player is not in another input mode. Too soon
   after the last cast (game_clock < lstime + spell_delay) it refuses with a sound. The
   three shelf runes, packed five bits each, are looked up in the 48-entry spells[] table;
   no match prints 'Not a spell'. */
void far try_cast(int how)
{
    char i;
    int runes;

    if (GameInputMode != 0)
        return;
    if ((inplist->cmd & 2) && how == 0) {
        mouse_release(1);
        return;
    }
    dseg_5c99_5A90 = 1;
    if (player->game_clock < lstime + spell_delay) {
        play_effect_here(0x15, 0x40, 0);
        return;
    }
    mouse_release(1);
    runes = (player->shelf[0] << 10) + (player->shelf[1] << 5) + player->shelf[2];
    for (i = 0; i < NUM_RUNE_SPELLS; i++)
        if (spells[i].runes == runes)
            break;
    if (i == NUM_RUNE_SPELLS) {
        not_a_spell();
        return;
    }
    player_cast(i);
}

/* Plays the fizzle sound and prints why a cast failed, string 0xD2 + why: 0 'You are not
   experienced enough to cast spells of that circle.', 1 'You do not have enough mana ...',
   2 'The incantation failed.', 3 'Casting was not successful.'. Returns 0. */
char far fail_spell(int why)
{
    play_effect_here(0x16, 0x40, 0);
    game_sprint(why + 0xD2);
    return 0;
}

/* The player casts spell idx (0..47; the circle is idx / 6 + 1). The checks, in order:
   the player's experience level must be at least 2 * circle - 1; mana must be at least
   3 * circle; and skill_check(Casting + 5, 2 * circle) must not give 0 (failed) or -1
   (backfire, string 0xD6: the spell becomes SPELLC_BACKFIRE at strength circle / 2). The
   delay before the next cast is (2 * circle - level) * 4 + 0x40 ticks. Mana is paid now,
   except for missile spells (SPELLC_MISSILE), which keep the cost in mspell_mused until
   they are aimed and released (SPELLS.C). On success effect 0x10 plays; when do_spell
   refuses, the cost is waived and 'Casting was not successful' printed. */
char far player_cast(unsigned char idx)
{
    unsigned char level;
    int sub;
    int cls;

    level = idx / 6;
    level++;
    cls = (spells[idx].cls & 0xF8) >> 3;
    if ((player->level + 1) / 2 < level)
        return fail_spell(0);
    if (level * 3 > player->play_mana)
        return fail_spell(1);
    if ((sub = skill_check(player->skills[SKILL_CASTING] + 5, level * 2)) == 0)
        return fail_spell(2);
    if (sub == -1) {
        game_sprint(0xD6);  /* 'The spell backfires.' */
        cls = SPELLC_BACKFIRE;
        sub = level / 2;
    } else
        sub = spells[idx].sub;
    spell_delay = (level * 2 - player->level) * 4 + 0x40;
    lstime = player->game_clock;
    mspell_mused = level * 3;
    if (cls != SPELLC_MISSILE) {
        player->play_mana -= mspell_mused;
        mspell_mused = 0;
    }
    if (do_spell(cls, sub, ThePlayer, ThePlayer)) {
        play_effect_here(0x10, 0x40, 0);
        return 1;
    }
    mspell_mused = 0;
    return fail_spell(3);
}
