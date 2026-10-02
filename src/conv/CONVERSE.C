/* target: ovr103 */
/* opts: -mm -1 -G -O -Y -d */
/* Conversations: starting one (TalkTo, strt_converse), loading the script and registering
   the built-in functions (Converse), the menus, the say, respond and print built-ins, and
   the inventory built-ins a script uses to look at, give and take objects while bartering:
   the whole of DOS overlay ovr103, in original order.

   The flow: TalkTo (from the player's talk command in ui/INTERACT.C, from critter/AI.C
   when an NPC talks first, and from talk_to_disembodied in event/WORLDEV.C) checks the
   NPC will talk, sets talking_to and cnv_id and switches to screen 4. The screen's start
   function, strt_converse (game/UWEDIT.C's screen table), draws the conversation screen
   with both portraits and names, sets up the trade slots (barter_init, BARTER.C) and
   calls Converse, which loads the script into the screen buffer (load_script, BABL.C),
   binds about fifty built-ins with bab_fun, hands over the variables
   (setup_converse_data, CONVVARS.C), runs the script (babl_run) and takes them back.
   Then newscr(1) returns to the game, whose screen exit free_converse puts things back
   and does any teleport the script asked for (do_babl_teleport, BABLHACK.C).

   A menu (babl_menu, babl_fmenu) prints numbered options in the player's scroll and runs
   converse_event_loop until conv_play_menu, the handler for the keys 1..5 and for a click
   on the scroll (game/PLAYER.C), records the choice.

   Data owned here: talking_to, cnv_id, conv_buffer and the screen and menu state below.

   Name: descriptive (map/filenames.tsv: conversations, TalkTo, strt_converse,
   Converse). */
/* name: Function and global names are the originals from the FM Towns symbol table where
   it has them; the FM Towns build keeps babl_menu, babl_fmenu, pause and babl_ask as
   statics, so the C functions behind those built-ins have provisional names (the target
   table keeps IDA's names). */
/* match: The provisional names were chosen for their keys: Turbo C lists a file's
   publics by the tools/bssorder.py key of each name and TLINK numbers overlay stub
   entries from the last one listed, so these names reproduce the EXE's stub order. */
/* Each built-in gets a far pointer to the top of the conversation stack, stack[0] being
   the argument count: stack[-1] is UW-Formats' arg1 (the last value pushed before the
   count), stack[-2] arg2 and so on. Each argument is the address of a conversation
   variable, which getmem reads and babl_setmem writes. */

#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include "conv.h"
#include "event.h"
#include "file.h"
#include "gfx.h"
#include "inv.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"

/* This file's own uninitialised data, DS:47FC..4927, in its _BSS with talking_to. */
/* name: The FM Towns build keeps all of it static, so none of it has an original name and
   it is static here, with provisional names. */
/* match: The names were chosen so that Turbo C's name order (tools/bssorder.py) lays it
   out as UW2 has it: RightPanelSaved 58, convoPics 187, convo_mode 259, convoScreen 331,
   convoWorkspace 507, convo_askstr 515, convoContinue 587, convo_facedata 643,
   babl_unref4854 650, convo_line_option 795, babl_choice 826, babl_opts 834,
   babl_display 874, talking_to 916, bablOptionIds 962, bablOptCount 978. Nothing reads or
   writes DS:4854..4857, but it lies between two of this file's variables. (DS:4928..4929,
   also never used, is left out: it may as well be ovr104's.) */
static unsigned char RightPanelSaved;           /* DS:47FC, the right panel before the conversation */
static char far *convoPics[17];                 /* DS:47FE, the portrait images */
static int16 convo_mode;                        /* DS:4842 */
static char far *convoScreen;                   /* DS:4844 */
static int16 convoWorkspace;                    /* DS:4848 */
static char far *convo_askstr;                  /* DS:484A */
static int16 convoContinue;                     /* DS:484E */
static char far *convo_facedata;                /* DS:4850, the portrait data */
static char babl_unref4854[4];                  /* DS:4854, never used */
static int16 convo_line_option[10];             /* DS:4858 */
static int16 babl_choice;                       /* DS:486C, the selected option */
static char far *babl_opts[20];                 /* DS:486E */
static char far *babl_display[20];              /* DS:48BE */
struct Object far *talking_to;                  /* DS:490E, FM Towns name */
static int16 bablOptionIds[10];                 /* DS:4912 */
static int16 bablOptCount;                      /* DS:4926 */

/* Initialised data, in the order of the EXE. */
static unsigned char convo_say_flag = 0;    /* set by respond, cleared by say */
static int16 babl_ask_handle = 0;           /* the string babl_ask returns */
char far *conv_buffer = 0;
static char cnv_file[] = "DATA\\cnv.ark";
uint16 cnv_id = 0;

/* Start a conversation with thing, if it will talk. A wisp is talked to through
   talk_to_disembodied(0x30). Anything not a creature, a critter with goal 15, or any
   critter while time is stopped gets a refusal. Otherwise whoami 0x8C always talks; a
   critter with goal 10 always talks; else it talks when its whoami is not 0xFF and it is
   either an ally, or friendly (attitude not 0) and not attacking the player (goal 5, 6
   or 9 with target 1). The conversation is whoami's slot, or 0x100 + the creature class
   for whoami 0, and must exist in CNV.ARK. */
void far TalkTo(struct Object far *thing)
{
    unsigned char who, subclass;
    if (OBJ_ITEM(thing) == ITEM_WISP) {
        talk_to_disembodied(0x30);
        return;
    }
    if (OBJ_MAJOR(thing) != MAJOR_CREATURE) {
        scroll_print(get_string(0xe00));  /* "You cannot talk to that!\n" */
        return;
    }
    if (OBJ_GOAL(thing) == 0xf) {
        scroll_print(get_string(0xe01));  /* "You get no response.\n" */
        return;
    }
    talking_to = thing;
    who = talking_to->whoami;
    subclass = OBJ_INMAJOR(talking_to);
    if (TimeStop) {
        scroll_print(get_string(0xe01));  /* "You get no response.\n" */
        return;
    }
    if (who == 0x8c) goto ready;
    if ((OBJ_GOAL(talking_to) == 5 || OBJ_GOAL(talking_to) == 6 ||
         OBJ_GOAL(talking_to) == 9) && OBJ_GTARG(talking_to) == 1)
        goto hunger;
    if (OBJ_ATTITUDE(talking_to) != 0)
        goto who_check;
hunger:
    if (!OBJ_ALLY(talking_to))
        goto goal_check;
who_check:
    if (who != 0xff) goto ready;
goal_check:
    if (OBJ_GOAL(talking_to) == 10)
        goto ready;
    scroll_print(get_string(0xe01));  /* "You get no response.\n" */
    return;
ready:
    if (who == 0) cnv_id = (unsigned)subclass + 0x100;
    else cnv_id = who;
    if (check_arc(2, "DATA\\", cnv_id) <= 0) {
        scroll_print(get_string(0xe01));  /* "You get no response.\n" */
        return;
    }
    kill_all_effects();
    newscr(4);
}

/* name: adr_convpic and move_convpic are FM Towns names (TalkTo, adr_convpic,
   move_convpic, strt_converse in the same order); the code matches the FM Towns
   functions. */

/* gronk_gr's allocator: the next n bytes of the portrait buffer */
char far * far adr_convpic(int n)
{
    char far *old = convo_facedata;
    convo_facedata += n;
    return old;
}

/* gronk_gr's loader: note where portrait number `which` landed, past its 5-byte header */
int far move_convpic(char far *image, int ok, int which)
{
    convoPics[which] = image + 5;
    return ok != 0;
}

/* The conversation screen's start function. The player's head is portrait
   female * 5 + body of "heads"; the NPC's is whoami - 1 of "charhead", else its class
   of "ghed", else ghed 0 (a fatal error if none loads). */
void far strt_converse(void)
{
    unsigned char who, loaded = 0;
    char name[0x28];
    update_sprites();
    if ((convoWorkspace = get_workspace()) == 0) return;
    set_random_walking_music(0);
    change_music_maybe();
    convoScreen = MK_FP(convoWorkspace, 0);
    mouse_hide();
    grSoftPageFlip();
    disk_to_vid(2, convoScreen);
    RightPanelSaved = RightPanel;
    RightPanel = 0;
    inv_refresh = 0;
    BeginInventory();
    RedispInv();
    inv_refresh = 1;
    do_play_scroll();
    scroll_clear(0);
    do_npc_scroll();
    scroll_clear(0);
    grfx_quikfont(FONT_5X6P);
    convo_facedata = convoScreen;
    if (!gronk_gr("heads", player->female * 5 + player->body, 1, (ArtAllocFn)adr_convpic, (ArtMoveFn)move_convpic))
        pfatal_code(ERR_READ | 0x15);
    show(0xa8, 0xc4, convoPics[0], 0x46, 0x40, 0, 0);
    convo_facedata = convoScreen;
    who = talking_to->whoami;
    if (who > 0)
        loaded = gronk_gr("charhead", who - 1, 1, (ArtAllocFn)adr_convpic, (ArtMoveFn)move_convpic);
    if (!loaded)
        loaded = gronk_gr("ghed", OBJ_INMAJOR(talking_to), 1, (ArtAllocFn)adr_convpic, (ArtMoveFn)move_convpic);
    if (!loaded)
        loaded = gronk_gr("ghed", 0, 1, (ArtAllocFn)adr_convpic, (ArtMoveFn)move_convpic);
    if (!loaded) pfatal_code(ERR_READ | 0x15);
    show(2, 0xc4, convoPics[0], 0x46, 0x40, 0, 0);
    str_copy(name, get_string(player_name_handle));
    *background_color = *foreground_color = 0xc7;
    string_to_screen(name, 0xa4 - string_width(name), 0xc5);
    if (get_name(name, (struct Object far *)talking_to, 0, 0)) {
        *background_color = *foreground_color = 0xc7;
        string_to_screen(name, 0x47, 0x81);
    }
    set_the_color(0x106);
    rectangle(0xf8, 0x43, 0x138, 0x21);
    grPageFlip();
    grSoftPageFlip();
    conv_buffer = convoScreen;
    barter_init();
    do_play_scroll();
    mouse_show();
    set_workspace();
    pmouseHandled = 0;
    mouse_freereign();
    Converse(talking_to->whoami, OBJ_INMAJOR(talking_to));
    newscr(1);
}

/* The conversation screen's exit function: release the screen buffer, give back what is
   left in the trade slots (end_barter), restore the right panel and the main scroll,
   catch up the schedules and animated objects (8 updates), and do the script's teleport. */
void far free_converse(void)
{
    register int i;
    if (convoWorkspace) {
        release_workspace();
        end_barter();
    }
    set_random_walking_music(-1);
    RightPanel = RightPanelSaved;
    do_main_scroll();
    Sched_SetAllClocks(1);
    quick_time = 1;
    if (DoAnimO)
        for (i = 0; i < 8; i++) update_animobj(1);
    quick_time = 0;
    do_babl_teleport();
    talking_to = 0;
    cs_check();
}

/* name: FM Towns do_escape_key_, also empty, at this position */
void far do_escape_key(void) { }

/* Load and run the conversation (who and subclass are not used; cnv_id already says
   which). The script's heap is the conversation screen buffer from offset 0x400. An NPC
   without an inventory gets one generated first. At the end the last line stays for
   0x1F4 (scroll_wait units) when the player spoke last, and none when the NPC spoke last
   or turned hostile. */
void far Converse(unsigned char who, int subclass)
{
    register int wait_time;
    if (load_script(cnv_file, convoScreen + 0x400) < 0) {
        scroll_print(get_string(0xe01));  /* "You get no response.\n" */
        return;
    }
    bab_fun("babl_menu", (BablFn)conv_choice_ovr103_A13);
    bab_fun("babl_fmenu", (BablFn)conv_fmenu_ovr103_BF2);
    bab_fun("say", (BablFn)npc_say);
    bab_fun("respond", (BablFn)play_respond);
    bab_fun("get_quest", (BablFn)get_quest);
    bab_fun("set_quest", (BablFn)set_quest);
    bab_fun("sex", (BablFn)sex);
    bab_fun("babl_ask", (BablFn)getInputText_ovr103_1117);
    bab_fun("print", (BablFn)conv_print);
    bab_fun("show_inv", (BablFn)conv_check_inv);
    bab_fun("give_to_npc", (BablFn)conv_give_inv);
    bab_fun("find_inv", (BablFn)conv_find_inv);
    bab_fun("take_from_npc", (BablFn)conv_take_inv);
    bab_fun("take_id_from_npc", (BablFn)conv_take_inv_id);
    bab_fun("identify_inv", (BablFn)conv_inv_name);
    bab_fun("do_offer", (BablFn)do_offer);
    bab_fun("do_demand", (BablFn)do_demand);
    bab_fun("do_decline", (BablFn)do_decline);
    bab_fun("do_judgement", (BablFn)do_judgement);
    bab_fun("end_barter", (BablFn)end_barter);
    bab_fun("setup_to_barter", (BablFn)setup_to_barter);
    bab_fun("pause", (BablFn)conv_pause_ovr103_10DD);
    bab_fun("set_likes_dislikes", (BablFn)npc_likes_dislikes);
    bab_fun("do_inv_create", (BablFn)conv_inv_create);
    bab_fun("do_inv_delete", (BablFn)conv_inv_delete);
    bab_fun("check_inv_quality", (BablFn)check_inv_quality);
    bab_fun("set_inv_quality", (BablFn)set_inv_quality);
    bab_fun("count_inv", (BablFn)count_inv);
    bab_fun("babl_hack", (BablFn)babl_hack);
    bab_fun("give_all_stuff", (BablFn)give_all_stuff);
    bab_fun("gronk_door", (BablFn)gronk_door);
    bab_fun("set_sequence", (BablFn)set_sequence);
    bab_fun("set_attitude", (BablFn)set_attitude);
    bab_fun("set_race_attitude", (BablFn)set_race_attitude);
    bab_fun("take_from_npc_inv", (BablFn)take_from_npc_inv);
    bab_fun("add_to_npc_inv", (BablFn)add_to_npc_inv);
    bab_fun("place_object", (BablFn)place_object);
    bab_fun("transform_talker", (BablFn)transform_talker);
    bab_fun("remove_talker", (BablFn)remove_talker);
    bab_fun("x_skills", (BablFn)x_skills);
    bab_fun("x_traps", (BablFn)x_traps);
    bab_fun("x_obj_stuff", (BablFn)x_obj_stuff);
    bab_fun("x_obj_pos", (BablFn)x_obj_pos);
    bab_fun("find_barter", (BablFn)find_barter);
    bab_fun("find_barter_total", (BablFn)find_barter_total);
    bab_fun("give_ptr_npc", (BablFn)give_ptr_npc);
    bab_fun("x_clock", (BablFn)x_clock);
    bab_fun("x_exp", (BablFn)x_exp);
    bab_fun("teleport_player", (BablFn)teleport_player);
    bab_fun("teleport_talker", (BablFn)teleport_talker);
    bab_fun("switch_pic", (BablFn)switch_pic);
    setup_converse_data(talking_to);
    convo_askstr = bab_malloc(0xa0L);
    if (!OBJ_HAS_INV(talking_to))
        generate_inventory(talking_to);
    babl_run();
    wait_time = 0x1f4;
    if (update_converse_data(talking_to) != 0 || convo_say_flag == 0)
        wait_time = 0;
    scroll_wait(wait_time, 0);
}

/* Wait for a menu choice, keeping music and animation going; when menus_active has been
   cleared it reprints the options. */
/* name: FM Towns converse_event_loop_ is this loop (loop_music_maybe, do_changes, the
   menu, input_dispatch); Converse registers babl_menu and babl_fmenu as the two statics
   after it, converse_event_loop_+0xD8 and +0x27C. */
void far converse_event_loop(void)
{
    int i;
    char line[0xa0];
    while (convoContinue) {
        loop_music_maybe();
        do_changes();
        if (menus_active != 0) goto dispatch;
        {
            scroll_wait(0x1f4, 0);
            do_play_scroll();
            scroll_clear(1);
            set_workspace();
            i = 1;
            while (i < bablOptCount) {
                line[0] = i + '0';
                line[1] = '.';
                line[2] = ' ';
                str_copy(line + 3, babl_display[i]);
                strcat(line, "\n");
                scroll_print(line);
                set_workspace();
                i++;
            }
        }
        do_play_scroll();
        menus_active = 1;
dispatch:
        input_dispatch(inplist);
        set_workspace();
    }
}

/* babl_menu(arg1 array): show the strings of a 0-terminated array of string ids as a
   numbered menu and return the number chosen (from 1). Only 1..5 have keys. */
int far conv_choice_ovr103_A13(int16 far *stack)
{
    int i, base, string_no, used;
    char line[0xa0];
    register int row;
    register int lines;
    convo_mode = 1;
    bablOptCount = 1;
    used = lines = 0;
    base = stack[-1];
    string_no = getmem(base);
    while (string_no != 0) {
        babl_opts[bablOptCount] = get_string(string_no);
        babl_display[bablOptCount] = convert_string(babl_opts[bablOptCount]);
        if (babl_display[bablOptCount] == babl_opts[bablOptCount]) {
            babl_display[bablOptCount] = bab_malloc((int32)(str_len(babl_opts[bablOptCount]) + 1));
            str_copy(babl_display[bablOptCount], babl_opts[bablOptCount]);
        }
        bablOptionIds[bablOptCount] = string_no;
        bablOptCount++;
        string_no = getmem(base + bablOptCount - 1);
    }
    do_play_scroll();
    scroll_clear(1);
    set_workspace();
    for (i = 0; i < 10; i++) convo_line_option[i] = -1;
    for (i = 1; i < bablOptCount; i++) {
        line[0] = i + '0';
        line[1] = '.';
        line[2] = ' ';
        str_copy(line + 3, babl_display[i]);
        str_cat(line, "\n");
        lines = scroll_print(line);
        set_workspace();
        for (row = used; row <= lines; row++) convo_line_option[row] = i;
        used = lines + 1;
    }
    do_play_scroll();
    convoContinue = 1;
    menus_active = 1;
    converse_event_loop();
    return babl_choice;
}

/* babl_fmenu(arg1 strings, arg2 flags): the same, showing only the strings whose flag
   is non-zero, and returning the chosen string's id rather than its number. */
int far conv_fmenu_ovr103_BF2(int16 far *stack)
{
    int i, base, string_no, value, value_base, used;
    char line[0xa0];
    register int row;
    register int lines;
    convo_mode = 1;
    bablOptCount = 1;
    used = 0;
    base = stack[-1];
    value_base = stack[-2];
    string_no = getmem(base);
    value = getmem(value_base);
    i = 1;
    while (string_no != 0) {
        if (value != 0) {
            babl_opts[bablOptCount] = get_string(string_no);
            babl_display[bablOptCount] = convert_string(babl_opts[bablOptCount]);
            if (babl_display[bablOptCount] == babl_opts[bablOptCount]) {
                babl_display[bablOptCount] = bab_malloc((int32)(str_len(babl_opts[bablOptCount]) + 1));
                str_copy(babl_display[bablOptCount], babl_opts[bablOptCount]);
            }
            bablOptionIds[bablOptCount] = string_no;
            bablOptCount++;
        }
        i++;
        string_no = getmem(base + i - 1);
        value = getmem(value_base + i - 1);
    }
    do_play_scroll();
    scroll_clear(1);
    set_workspace();
    for (i = 0; i < 10; i++) convo_line_option[i] = -1;
    for (i = 1; i < bablOptCount; i++) {
        line[0] = i + '0';
        line[1] = '.';
        line[2] = ' ';
        str_copy(line + 3, babl_display[i]);
        str_cat(line, "\n");
        lines = scroll_print(line);
        set_workspace();
        for (row = used; row <= lines; row++) convo_line_option[row] = i;
        used = lines + 1;
    }
    do_play_scroll();
    convoContinue = 1;
    menus_active = 1;
    converse_event_loop();
    return bablOptionIds[babl_choice];
}

/* The menu's input handler: option is the key's number, or 0 for a click, whose row in
   the scroll picks the option. Echoes the choice as the player's line and frees the
   substituted texts. */
void far conv_play_menu(int option)
{
    int y, row;
    register int i;
    register int selected;
    if (convo_mode == 0) return;
    set_workspace();
    if (option == 0) {
        y = inplist->y + 1;
        row = (0x1e - y) / cur_font->height;
        selected = convo_line_option[row];
    } else selected = option;
    if (selected <= 0 || selected >= bablOptCount) return;
    convo_mode = 0;
    convoContinue = 0;
    mouse_release(0);
    do_play_scroll();
    scroll_clear(1);
    set_workspace();
    menus_active = 0;
    do_npc_scroll();
    for (i = 1; i < bablOptCount; i++) {
        if (i == selected) play_say(babl_display[i]);
        if (babl_opts[i] != babl_display[i]) bab_free(babl_display[i]);
    }
    do_play_scroll();
    babl_choice = selected;
}

/* The "say" built-in (and SAY_OP): the NPC's line, in the NPC's scroll, \P first. */
void far npc_say(char far *s)
{
    str_copy(conv_buffer, "\\P");
    str_cat(conv_buffer, s);
    str_cat(conv_buffer, "\n");
    do_npc_scroll();
    scroll_print(conv_buffer);
    set_workspace();
    do_play_scroll();
    convo_say_flag = 0;
}

/* The "respond" built-in (and RESPOND_OP): a line in the player's scroll. */
void far play_respond(char far *s)
{
    str_copy(conv_buffer, s);
    str_cat(conv_buffer, "\n");
    do_play_scroll();
    scroll_print(conv_buffer);
    set_workspace();
    do_play_scroll();
    convo_say_flag = 1;
}

/* The player's chosen line, in the conversation scroll in colour \1. */
void far play_say(char far *s)
{
    str_copy(conv_buffer, "\\1");
    str_cat(conv_buffer, s);
    str_cat(conv_buffer, "\\0\n");
    do_npc_scroll();
    scroll_print(conv_buffer);
    set_workspace();
    do_play_scroll();
    convo_say_flag = 1;
}

/* print(arg1 string): narration, in colour \2. */
void far conv_print(int16 far *stack)
{
    char far *original, far *expanded;
    register int id;
    id = getmem(stack[-1]);
    original = get_string(id);
    expanded = convert_string(original);
    str_copy(conv_buffer, "\\2");
    str_cat(conv_buffer, expanded);
    str_cat(conv_buffer, "\\0\n");
    do_npc_scroll();
    scroll_print(conv_buffer);
    do_play_scroll();
    set_workspace();
    if (expanded != original) bab_free(expanded);
}

/* pause(arg1): wait arg1 * 0x1F4 (scroll_wait units). */
int far conv_pause_ovr103_10DD(int16 far *stack)
{
    register int duration;
    duration = getmem(stack[-1]);
    mouse_release(0);
    scroll_wait(duration * 0x1f4, 0);
    set_workspace();
    return 1;
}

/* babl_ask: let the player type up to 0x32 characters; returns the id of a dynamic
   string holding them, the same id for every call in a conversation. */
int far getInputText_ovr103_1117(void)
{
    char response[0xa0];
    wdialog(0, 0, response, 1, 0x32);
    scroll_print("\n");
    set_workspace();
    str_copy(convo_askstr, response);
    if (babl_ask_handle == 0)
        babl_ask_handle = make_string(convo_askstr, STRBLK_DYNAMIC);
    else if (get_string(babl_ask_handle) == 0)
        replace_string(convo_askstr, babl_ask_handle);
    set_workspace();
    return babl_ask_handle;
}

/* name: The inventory built-ins, in FM Towns order: conv_check_inv (show_inv), find_barter,
   find_barter_total, conv_give_inv (give_to_npc), give_ptr_npc, conv_inv_delete,
   conv_find_inv, conv_take_inv, conv_take_inv_id, conv_inv_create, conv_inv_name
   (identify_inv), count_inv, check_inv_quality, set_inv_quality, switch_pic. Each one's
   callees agree with its FM Towns namesake. */

/* show_inv(arg1 indices, arg2 item ids): fill the two arrays (6 entries) with the
   player's selected trade items; returns how many. */
int far conv_check_inv(int16 far *stack)
{
    int16 ids[6], indices[6];
    register int i, count;
    count = player_barter_items(ids, indices);
    for (i = 0; i < 6; i++) {
        if (count > i) {
            babl_setmem(stack[-2] + i, ids[i]);
            babl_setmem(stack[-1] + i, indices[i]);
        } else {
            babl_setmem(stack[-1] + i, 0);
            babl_setmem(stack[-2] + i, 0);
        }
    }
    return count;
}

/* find_barter(arg1 item): the object index of the player's selected trade item of that
   type, or (1000 and up) of major (item - 1000) >> 2, minor (item - 1000) & 3; else 0. */
int far find_barter(int16 far *stack)
{
    int16 count, ids[6], indices[6];
    register int i, wanted;
    wanted = getmem(stack[-1]);
    count = player_barter_items(ids, indices);
    if (wanted < 1000) {
        for (i = 0; i < count; i++)
            if (ids[i] == wanted) return indices[i];
    } else {
        for (i = 0; i < count; i++)
            if ((ids[i] >> 6) == ((wanted - 1000) >> 2) &&
                ((ids[i] & 0x30) >> 4) == ((wanted - 1000) & 3))
                return indices[i];
    }
    return 0;
}

/* find_barter_total(arg4 item, arg3 count out, arg2 indices out, arg1 total out): the
   player's selected trade items of type item, and their total quantity; 1 when any.
   A class search (1000 and up) is coded in the test but the enclosing check skips it,
   so it always finds nothing. Its arrays hold 5 entries, but player_barter_items can
   write 6 when all six player slots are selected. */
int far find_barter_total(int16 far *stack)
{
    int16 wanted, matches, ids[5], indices[5], matching[5], count;
    struct Object far *obj;
    register int i, total;
    wanted = getmem(stack[-4]);
    matches = 0;
    total = 0;
    count = player_barter_items(ids, indices);
    if (wanted < 1000) {
        for (i = 0; i < count; i++) {
            if (wanted < 1000 ? ids[i] == wanted : (ids[i] >> 4) == wanted - 1000) {
                obj = Obj_IntTMem(indices[i]);
                matching[matches] = indices[i];
                if (OBJ_ISQUANT(obj) && !(obj->ol.f.link & LINK_SPECIAL))
                    total += obj->ol.f.link;
                else total++;
                matches++;
            }
        }
    }
    babl_setmem(stack[-3], matches);
    babl_setmem(stack[-1], total);
    for (i = 0; i < matches; i++) babl_setmem(stack[-2] + i, matching[i]);
    return total > 0 ? 1 : 0;
}

/* give_to_npc(arg2 count, arg1 indices): give the listed objects to the NPC if every
   one is among the player's selected trade items; 1 if given, else 0. */
int far conv_give_inv(int16 far *stack)
{
    int16 count, have, ids[6], indices[6], slot_of[6], item_of[6];
    register int j, i;
    count = getmem(stack[-2]);
    if ((have = player_barter_items(ids, indices)) < count) return 0;
    for (j = 0; j < 6; j++) slot_of[j] = item_of[j] = -1;
    for (i = 0; i < count; i++) {
        for (j = 0; j < have; j++) {
            if (getmem(stack[-1] + i) == indices[j] && item_of[j] == -1) {
                slot_of[i] = j;
                item_of[j] = i;
                break;
            }
        }
        if (slot_of[i] == -1) return 0;
    }
    for (j = 0; j < count; j++) player_barter_give(getmem(stack[-1] + j));
    return 1;
}

/* give_ptr_npc(arg2 object, arg1 qty): give the NPC qty of one object, splitting a
   stack if need be, from the trade slots or else from the player's inventory. */
int far give_ptr_npc(int16 far *stack)
{
    int index;
    struct Object far *obj, far *copy;
    int16 ids[6], indices[6];
    register int i, qty;
    index = getmem(stack[-2]);
    qty = getmem(stack[-1]);
    if (qty <= 0) return 0;
    obj = Obj_IntTMem(index);
    player_barter_items(ids, indices);
    for (i = 0; i < 6; i++) {
        if (ids[i] == 0) continue;
        if (indices[i] != index) continue;
        if ((obj->ol.f.link & LINK_SPECIAL) || !OBJ_ISQUANT(obj) || obj->ol.f.link <= qty)
            player_barter_give(index);
        else {
            copy = Obj_Alloc(0);
            if (copy != 0) {
                *(struct StaticObj far *)copy = *(struct StaticObj far *)obj;
                copy->ol.f.link = qty;
                obj->ol.f.link = obj->ol.f.link - qty;
                npc_inv_add(copy);
                RedisplayBarterSlots(1);
            } else return 0;
        }
        return 1;
    }
    if (qty < 0 || !OBJ_ISQUANT(obj) || (obj->ol.f.link & LINK_SPECIAL)) qty = -1;
    if (!invRemoveObject(obj, qty)) return 0;
    npc_inv_add(obj);
    return 1;
}

/* do_inv_delete, find_inv, take_from_npc, take_id_from_npc and do_inv_create: thin
   wrappers of BARTER.C's npc_inv_delete, npc_barter_find, npc_barter_give,
   npc_barter_give_id and npc_inv_create. */
int far conv_inv_delete(int16 far *stack) { return npc_inv_delete(getmem(stack[-1])); }

int far conv_find_inv(int16 far *stack) { return npc_barter_find(getmem(stack[-2]), getmem(stack[-1])); }

int far conv_take_inv(int16 far *stack) { return npc_barter_give(getmem(stack[-1])); }

int far conv_take_inv_id(int16 far *stack) { return npc_barter_give_id(getmem(stack[-1])); }

int far conv_inv_create(int16 far *stack) { return npc_inv_create(getmem(stack[-1])); }

/* identify_inv(arg4 object, arg3 with article, arg2 name out, arg1 identified): the
   object's name with an article or a count, as a new dynamic string stored in arg2, and
   its value to the NPC (assess_value with likes, blurred by npc_assess) returned. */
int far conv_inv_name(int16 far *stack)
{
    int index, value, handle, describe;
    struct Object far *obj;
    char far *copy;
    char mods[10];
    char number[6];
    char article;
    int ident;
    char name[0x50];
    register char *p;
    register int qty;
    index = getmem(stack[-4]);
    describe = getmem(stack[-3]);
    ident = getmem(stack[-1]);
    value = assess_value(1, index, npc_assess);
    obj = Obj_IntTMem(index);
    if (OBJ_ISQUANT(obj) && !(obj->ol.f.link & LINK_SPECIAL))
        qty = obj->ol.f.link;
    else qty = 1;
    p = name;
    name[0] = 0;
    article = 0;
    mods[0] = 0;
    if (do_mods(obj, ident, mods))
        article = mods[0];
    if (article && describe && qty == 1) {
        switch (article) {
        case 'a': case 'e': case 'i': case 'o': case 'u':
            strcpy(name, "an ");
            break;
        default:
            strcpy(name, "a ");
            break;
        }
        describe = 0;
    } else if (qty > 1) {
        strcat(name, itoa(qty, number, 10));
        strcat(name, " ");
        describe = 0;
    }
    if (mods[0]) strcat(name, mods);
    p = name + strlen(name);
    get_name(p, obj, describe, qty > 1 ? 1 : 0);
    p = name;
    do_of(obj, ident, p);
    copy = bab_malloc(strlen(p) + 1);
    str_copy(copy, p);
    handle = make_string(copy, STRBLK_DYNAMIC);
    babl_setmem(stack[-2], handle);
    return value;
}

/* how many of an object: its quantity, or 1 */
int far count_inv(int16 far *stack)
{
    struct Object far *obj;
    register int qty, index;
    index = getmem(stack[-1]);
    obj = Obj_IntTMem(index);
    if (OBJ_ISQUANT(obj) && !(obj->ol.f.link & LINK_SPECIAL)) qty = obj->ol.f.link;
    else qty = 1;
    return qty;
}

/* check_inv_quality: reads the quality but never returns it, so the script gets
   whatever AX holds at the end. */
int far check_inv_quality(int16 far *stack)
{
    struct Object far *obj;
    register int quality;
    int index;
    index = getmem(stack[-1]);
    obj = Obj_IntTMem(index);
    quality = obj->qn.f.quality;
}

/* set_inv_quality(arg2 object, arg1 quality): set it, to 6 bits. */
int far set_inv_quality(int16 far *stack)
{
    struct Object far *obj;
    register int index;
    index = getmem(stack[-2]);
    obj = Obj_IntTMem(index);
    obj->qn.f.quality = getmem(stack[-1]) & 0x3f;
    return 1;
}

/* switch_pic(arg1): show another portrait and name for the NPC: whoami arg1 below
   0x100, else generic head arg1 - 0x100 (up to 0x140), else ghed 0. */
int far switch_pic(int16 far *stack)
{
    int which;
    char far *saved;
    unsigned char loaded;
    char name[0x28];
    struct Object who;
    which = getmem(stack[-1]);
    loaded = 0;
    set_workspace();
    convo_facedata = bab_malloc(0x12c0L);
    saved = convo_facedata;
    if (convo_facedata == 0) return 0;
    if (which < 0x100)
        loaded = gronk_gr("charhead", which - 1, 1, (ArtAllocFn)adr_convpic, (ArtMoveFn)move_convpic);
    else if (which <= 0x140)
        loaded = gronk_gr("ghed", which - 0x100, 1, (ArtAllocFn)adr_convpic, (ArtMoveFn)move_convpic);
    if (!loaded) {
        convo_facedata = saved;
        loaded = gronk_gr("ghed", 0, 1, (ArtAllocFn)adr_convpic, (ArtMoveFn)move_convpic);
    }
    if (loaded) {
        who.b0F = 2;
        SET_MAJOR(&who, MAJOR_CREATURE);
        if (which < 0x100) who.whoami = which;
        else who.id = who.id & 0xFFC0 | ((which - 0x100) & 0x3F) << 0;
        mouse_hide();
        show(2, 0xc4, convoPics[0], 0x46, 0x40, 0, 0);
        if (get_name(name, (struct Object far *)&who, 0, 0)) {
            set_the_color(1);
            urectangle(0x47, 0x81, 0x98, 0x7c);
            *background_color = *foreground_color = 0xc7;
            string_to_screen(name, 0x47, 0x81);
        }
        mouse_show();
        set_workspace();
    }
    bab_free(saved);
    return loaded;
}
