/* target: ovr137 */
/* opts: -mm -1 -G -O -Y -d */
/* The main game screen's set-up and teardown and its status clicks: the panel chain, the
   compass (hunger, fatigue, world and time of day) and the flask (health or mana, and
   poison) messages, registering their mouse areas, and the checks before saving or resting:
   the whole of DOS overlay ovr137, in original order. Function and global names are the
   originals from the FM Towns symbol table; the source file's own name is not known.

   Entry points: init_gamedisp is the 3D screen's set-up (UWEDIT.C's strt_demscr path): it
   starts the inventory, the message scroll (SCROLLIO.C's init_scroll), the music and the
   game screen's mouse areas (start_gameinp), reselects the current mode's icon, and draws
   the screen furniture (PANELS.C's init_scrgr); clear_gamedisp undoes it. The four areas
   start_gameinp registers, all for mode 1: the rune shelf (COMBAT/SPELLS' try_cast), the
   active spell icons (try_clear), the compass (print_info) and the flasks (flask_info).
   check_save and check_rest are the options panel's guards (WRAPPER.C's save_opt and
   restore_opt).

   Data owned: the four mouse-area handles.

   Name: descriptive (the game screen's set-up and status clicks: init_gamedisp,
   flask_info). */

#include <stdlib.h>
#include "combat.h"
#include "critter.h"
#include "gfx.h"
#include "inv.h"
#include "object.h"
#include "player.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"

/* This file's uninitialised data: the four mouse-area handles, FM Towns names. Defined
   here, they land at DS:817C to DS:8183 in the EXE's order. */
int inforMshandle;               /* DS:817C */
int actspMshandle;               /* DS:817E */
int flaskMshandle;               /* DS:8180 */
int spellMshandle;               /* DS:8182 */


/* Flips the right-hand panel: from the inventory (RightPanel 0) to the statistics page
   (2), from any other panel back to the inventory, and nothing while a flip is under way
   (4). PANELS.C's adjust_panel animates the flip. The argument is not read; flask_info
   passes 0, and FM Towns passes one too. */
void far pull_chain(int how)
{
    if (RightPanel == 0)
        set_screen_frame(6, 2);
    else if (RightPanel != 4)
        set_screen_frame(6, 0);
}

/* A click on the compass: prints how hungry and how tired the player is, which world the
   player is in (if the player has learnt about the worlds) and the time of day.
   Hunger 0..255 in steps of 30 picks one of nine words; fatigue / 23 (capped at 5) one of
   six, higher meaning more rested; asleep (player->in_void set) prints "asleep" instead.
   The world is (PlayerLevel - 1) / 8; quest_bytes[3] holds a bit for each world after
   the first that the player knows by name (bit w - 1 for world w, Britannia always
   known), and an unknown world is "an alternate dimension". The hour is game_clock /
   0x1C2000 mod 12, twelve two-hour periods, so game_clock counts 256 to a game second
   (inferred from 0x1C2000 = 7200 * 256). A missing world line is replaced by a blank
   line, so the message always takes the same number of lines. */
void far print_info(void)
{
    int known;
    int world;
    int n;
    int w;

    scroll_print("\n");
    /* "You are currently " "starving" .. "stuffed" " and " */
    game_strings_3(0x44, player->hunger / 0x1E + 0x75, 0x74);
    if (player->in_void)
        game_sprint(0x128);             /* "asleep" */
    else {
        n = player->fatigue / 0x17;
        if (n > 5)
            n = 5;
        game_sprint(0x83 - n);          /* "wide awake" .. "fatigued" */
    }
    game_sprint(0x60);                  /* ".\n" */
    if ((known = player->quest_bytes[3]) != 0) {
        world = (PlayerLevel - 1) / 8;
        w = ((known << 1) + 1) & (1 << world) ? world + 2 : 1;
        /* "You are in " "an alternate dimension" / "Britannia" .. ".\n" */
        game_strings_3(0x49, w + 0x49, 0x60);
    } else
        world = -1;
    n = player->game_clock / 0x1C2000L;
    n = n % 12;
    game_strings_3(0x48, n + 0x54, 0x60);   /* "You guess that it is currently " "night" .. ".\n" */
    scroll_print("\n");
    if (world == -1)
        scroll_print("\n");
    mouse_release(1);
}

/* A click on the flasks: on the chain between them (x 0x17 to 0x2B, above y 10) pulls
   the chain, which flips the right-hand panel; on the left flask prints the vitality as
   "n out of max", preceded by the poison level if poisoned ((poison - 1) / 3 picks
   "barely" .. "critically"); on the right flask the mana. */
void far flask_info(void)
{
    char cur[10];
    char max[10];
    char msg[120];

    if (inplist->x > 0x16 && inplist->x < 0x2C) {
        if (inplist->y > 10)
            pull_chain(0);
    } else {
        if (inplist->y > 0x1E)
            return;
        /* "Your current vitality is " / "... mana points are " */
        str_copy(msg, get_string(((inplist->x > 0x1E) + 0x66) | STR_GAME));
        if (inplist->x < 0x28) {
            itoa(ThePlayer->hp, cur, 10);
            itoa(playerdat->avghit, max, 10);
            if (player->poison)
                /* "You are " "barely" .. " poisoned.\n" */
                game_strings_3(0x68, (player->poison - 1) / 3 + 0x61, 0x69);
        } else {
            itoa(player->play_mana, cur, 10);
            itoa(player->max_mana, max, 10);
        }
        str_cat(msg, cur);
        str_cat(msg, " out of ");
        str_cat(msg, max);
        str_cat(msg, "\n");
        scroll_print(msg);
        mouse_release(1);
    }
}

void far start_gameinp(void)
{
    LeftPanel = 0;
    setup_icon_buttons();
    spellMshandle = input_addmouse(0xA9, 0x22, 0xDF, 0x34, 0, 1, try_cast);
    actspMshandle = input_addmouse(0x11, 0x22, 0x42, 0x34, 0, 1, (InputFn)try_clear);
    inforMshandle = input_addmouse(0x5B, 0x22, 0x94, 0x34, 0, 1, (InputFn)print_info);
    flaskMshandle = input_addmouse(0xF3, 0x24, 0x13C, 0x45, 0, 1, (InputFn)flask_info);
}

void far clear_gameinp(void)
{
    term_icon_buttons();
    input_del(spellMshandle);
    input_del(actspMshandle);
    input_del(flaskMshandle);
    input_del(inforMshandle);
}

void far init_gamedisp(void)
{
    BeginInventory();
    init_scroll();
    play_music();
    start_gameinp();
    if (LeftPanel == 0 && RightButtonThing != 0)
        new_IconSelect(button_to_mode[RightButtonThing + 5]);
    display_scr();
    init_scrgr();
}

void far clear_gamedisp(void)
{
    clear_gameinp();
    EndInventory();
}

/* Saving is refused while an action is in progress (GameInputMode). Returns 1 if the
   save may go ahead. */
/* name: IDA's ovr137_3B7. FM Towns has check_save_ at the same place, between
   clear_gamedisp_ and check_rest_, with the same code: 0xAE when GameInputMode is set,
   printed between two page flips. */
char far check_save(void)
{
    int id;

    id = 0;
    if (GameInputMode)
        id = 0xAE;
    if (id) {
        grSoftPageFlip();
        game_sprint(id);                /* 0xAE: "You cannot select options partway ..." */
        grSoftPageFlip();
        return 0;
    }
    return 1;
}

/* Restoring is always allowed: an action in progress is simply abandoned and its cursor
   dropped. */
char far check_rest(void)
{
    if (GameInputMode) {
        GameInputMode = 0;
        unforce_mouse_cursor(0);
    }
    return 1;
}
