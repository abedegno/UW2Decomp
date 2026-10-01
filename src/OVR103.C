/* target: ovr103 */
/* opts: -mm -1 -G -O -Y -d */
/* Conversations: starting one (TalkTo, strt_converse), loading the script and registering
   the built-in functions (Converse), the menus, the say, respond and print built-ins, and
   the inventory built-ins a script uses to look at, give and take objects while bartering:
   the whole of DOS overlay ovr103, in original order. Function and global names are the
   originals from the FM Towns symbol table where it has them; the FM Towns build keeps
   babl_menu, babl_fmenu, pause and babl_ask as statics, so those keep their IDA names.

   Each built-in gets a far pointer just past its arguments on the conversation stack:
   stack[-1] is the first argument, stack[-2] the second and so on. Each argument is the
   address of a conversation variable, which getmem reads and babl_setmem writes. */

#include <string.h>
#include <stdlib.h>
#include <dos.h>

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0x64];
    unsigned b64:8;                     /* 0x64 */
    unsigned lefty:1;                   /* 0x65 */
    unsigned female:1;
    unsigned body:3;
    unsigned pclass:3;
};

/* An object: these 8 bytes are all a static object has. */
struct Object {
    unsigned id;                        /* item id 0-8 (major 6-8), quantity flag 15 */
    unsigned pos;
    union {
        unsigned word;
        struct { unsigned quality:6, next:10; } f;
    } qn;
    union {
        unsigned word;                  /* the head of the contents list, or the quantity */
        struct { unsigned owner:6, link:10; } f;
    } ol;
};

/* A mobile object (a critter), 0x1B bytes, starting with the same 8. */
struct Mobile {
    unsigned id, pos, qn, ol;
    char pad08[0x0B - 0x08];
    unsigned goal_word;                 /* 0x0B, goal 0-3, goal target 4-11 */
    unsigned attitude_word;             /* 0x0D, attitude 14-15, inventory made 12 */
    unsigned w0F;                       /* 0x0F */
    char pad11[0x19 - 0x11];
    unsigned char b19;                  /* 0x19, bit 6 */
    unsigned char whoami;               /* 0x1A, who this is, for the conversation */
};

#define OBJ_ITEM(o)     ((o)->id & 0x1FF)
#define OBJ_MAJOR(o)    (((o)->id & 0x1C0) >> 6)
#define OBJ_TYPE(o)     (((o)->id & 0x3F) >> 0)
#define OBJ_ISQUANT(o)  (((o)->id & 0x8000) >> 15)
#define OBJ_GOAL(o)     (((o)->goal_word & 0xF) >> 0)
#define OBJ_GTARG(o)    (((o)->goal_word & 0xFF0) >> 4)
#define OBJ_ATTITUDE(o) (((o)->attitude_word & 0xC000) >> 14)
#define OBJ_HAS_INV(o)  (((o)->attitude_word & 0x1000) >> 12)
#define OBJ_B19_6(o)    (((o)->b19 & 0x40) >> 6)

extern struct Player near *player;
extern struct Mobile far *talking_to;
extern unsigned char TimeStop;
extern unsigned char RightPanel, inv_refresh;
extern unsigned char far *foreground_color, far *background_color;
extern int player_name_handle;
extern unsigned char pmouseHandled;
extern unsigned char quick_time, DoAnimO;
extern unsigned char menus_active;
extern void *inplist;
extern unsigned char far *cur_font;
extern int npc_assess;

/* This file's own uninitialised data. The FM Towns build keeps all of it static, so
   none of it has an original name; they stay extern here (provisional). */
extern unsigned char RightPanelBeforeConversation;
extern char far *PortraitImageDataPtr[];
extern int babl_mode;
extern char far *convoScreen;
extern int convoWorkspace;
extern char far *babl_askstring;
extern int convoContinue;
extern char far *PortraitData;
extern int babl_line_option[10];
extern int selected_conversation_option;
extern char far *babl_options[20];
extern char far *babl_display[20];
extern int babl_option_ids[10];
extern int babl_option_count;

/* Initialised data, in the order of the EXE. */
static unsigned char convo_say_flag = 0;    /* set by respond, cleared by say */
static int babl_ask_handle = 0;             /* the string babl_ask returns */
char far *conv_buffer = 0;
static char cnv_file[] = "DATA\\cnv.ark";
unsigned cnv_id = 0;

void far talk_to_disembodied(int);
char far * far get_string(int);
int far scroll_print(char far *);
int far check_arc(int, char *, int);
void far kill_all_effects(void);
void far newscr(int);
void far update_sprites(void);
int far get_workspace(void);
void far release_workspace(void);
int far set_workspace(void);
void far set_random_walking_music(int);
void far change_music_maybe(void);
void far loop_music_maybe(void);
void far mouse_hide(void);
void far mouse_show(void);
void far mouse_freereign(void);
void far mouse_release(int);
void far grSoftPageFlip(void);
void far grPageFlip(void);
void far disk_to_vid(int, void far *);
void far BeginInventory(void);
void far RedispInv(void);
void far do_play_scroll(void);
void far do_npc_scroll(void);
void far do_main_scroll(void);
void far scroll_clear(int);
void far j_LikelyWaitForMore_ovr139_29(int, int);
void far grfx_quikfont(int);
unsigned char far gronk_gr(char *, int, int, char far *(far *)(int),
                           int (far *)(char far *, int, int));
void far pfatal_code(int);
void far show(int, int, char far *, int, int, int, int);
char far * far str_copy(char far *, char far *);
char far * far str_cat(char far *, char far *);
int far str_len(char far *);
int far string_width(char far *);
void far string_to_screen(char far *, int, int);
int far get_name(char far *, struct Object far *, char, char);
void far set_the_color(int);
void far rectangle(int, int, int, int);
void far urectangle(int, int, int, int);
void far barter_init(void);
void far end_barter(void);
void far Sched_SetAllClocks(int);
void far update_animobj(int);
void far do_babl_teleport(void);
void far cs_check(void);
void far do_changes(void);
void far input_dispatch(void *);
int far load_script(char *, void far *);
void far bab_fun();                 /* (char *name, the built-in) */
void far setup_converse_data(struct Mobile far *);
char far * far bab_malloc(long);
void far bab_free(char far *);
void far generate_inventory(struct Mobile far *);
void far babl_run(void);
unsigned char far update_converse_data(struct Mobile far *);
char far * far convert_string(char far *);
int far getmem(int);
void far babl_setmem(int, int);
void far wdialog(char *, char *, char *, char, int);
int far make_string(char far *, int);
void far replace_string(char far *, int);
struct Object far * far Obj_IntTMem(int);
struct Object far * far Obj_Alloc(int);
int far player_barter_items(int *, int *);
void far player_barter_give(int);
int far npc_barter_find(int, int);
int far npc_barter_give(int);
int far npc_barter_give_id(int);
int far npc_inv_create(int);
int far npc_inv_delete(int);
void far npc_inv_add(struct Object far *);
void far RedisplayBarterSlots(int);
unsigned char far invRemoveObject(struct Object far *, int);
int far assess_value(int, int, int);
char far do_mods(struct Object far *, int, char *);
char far do_of(struct Object far *, int, char *);

/* the built-ins of other files, all registered by Converse */
int far get_quest(), far set_quest(), far sex();
int far do_offer(), far do_demand(), far do_decline(), far do_judgement();
int far setup_to_barter(), far npc_likes_dislikes();
int far babl_hack(), far give_all_stuff(), far gronk_door(), far set_sequence();
int far set_attitude(), far set_race_attitude(), far take_from_npc_inv();
int far add_to_npc_inv(), far place_object(), far transform_talker();
int far remove_talker(), far x_skills(), far x_traps(), far x_obj_stuff();
int far x_obj_pos(), far x_clock(), far x_exp(), far teleport_player();
int far teleport_talker();

char far * far adr_convpic(int);
int far move_convpic(char far *, int, int);
void far Converse(unsigned char, int);
int far babl_menu_ovr103_A13(int far *);
int far babl_fmenu_ovr103_BF2(int far *);
void far npc_say(char far *);
void far play_respond(char far *);
void far play_say(char far *);
void far conv_print(int far *);
int far pause_ovr103_10DD(int far *);
int far babl_ask_ovr103_1117(void);
int far conv_check_inv(int far *);
int far conv_give_inv(int far *);
int far conv_find_inv(int far *);
int far conv_take_inv(int far *);
int far conv_take_inv_id(int far *);
int far conv_inv_name(int far *);
int far conv_inv_create(int far *);
int far conv_inv_delete(int far *);
int far check_inv_quality(int far *);
int far set_inv_quality(int far *);
int far count_inv(int far *);
int far find_barter(int far *);
int far find_barter_total(int far *);
int far give_ptr_npc(int far *);
int far switch_pic(int far *);

void far TalkTo(struct Mobile far *thing)
{
    unsigned char who, subclass;
    if (OBJ_ITEM(thing) == 0x1cd) {
        talk_to_disembodied(0x30);
        return;
    }
    if (OBJ_MAJOR(thing) != 1) {
        scroll_print(get_string(0xe00));
        return;
    }
    if (OBJ_GOAL(thing) == 0xf) {
        scroll_print(get_string(0xe01));
        return;
    }
    talking_to = thing;
    who = talking_to->whoami;
    subclass = OBJ_TYPE(talking_to);
    if (TimeStop) {
        scroll_print(get_string(0xe01));
        return;
    }
    if (who == 0x8c) goto ready;
    if ((OBJ_GOAL(talking_to) == 5 || OBJ_GOAL(talking_to) == 6 ||
         OBJ_GOAL(talking_to) == 9) && OBJ_GTARG(talking_to) == 1)
        goto hunger;
    if (OBJ_ATTITUDE(talking_to) != 0)
        goto who_check;
hunger:
    if (!OBJ_B19_6(talking_to))
        goto goal_check;
who_check:
    if (who != 0xff) goto ready;
goal_check:
    if (OBJ_GOAL(talking_to) == 10)
        goto ready;
    scroll_print(get_string(0xe01));
    return;
ready:
    if (who == 0) cnv_id = (unsigned)subclass + 0x100;
    else cnv_id = who;
    if (check_arc(2, "DATA\\", cnv_id) <= 0) {
        scroll_print(get_string(0xe01));
        return;
    }
    kill_all_effects();
    newscr(4);
}

/* adr_convpic and move_convpic are FM Towns names (TalkTo, adr_convpic, move_convpic,
   strt_converse in the same order); the code matches the FM Towns functions. */

/* gronk_gr's allocator: the next n bytes of the portrait buffer */
char far * far adr_convpic(int n)
{
    char far *old = PortraitData;
    PortraitData += n;
    return old;
}

/* gronk_gr's loader: note where portrait number `which` landed, past its 5-byte header */
int far move_convpic(char far *image, int ok, int which)
{
    PortraitImageDataPtr[which] = image + 5;
    return ok != 0;
}

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
    RightPanelBeforeConversation = RightPanel;
    RightPanel = 0;
    inv_refresh = 0;
    BeginInventory();
    RedispInv();
    inv_refresh = 1;
    do_play_scroll();
    scroll_clear(0);
    do_npc_scroll();
    scroll_clear(0);
    grfx_quikfont(1);
    PortraitData = convoScreen;
    if (!gronk_gr("heads", player->female * 5 + player->body, 1, adr_convpic, move_convpic))
        pfatal_code(0x3015);
    show(0xa8, 0xc4, PortraitImageDataPtr[0], 0x46, 0x40, 0, 0);
    PortraitData = convoScreen;
    who = talking_to->whoami;
    if (who > 0)
        loaded = gronk_gr("charhead", who - 1, 1, adr_convpic, move_convpic);
    if (!loaded)
        loaded = gronk_gr("ghed", OBJ_TYPE(talking_to), 1, adr_convpic, move_convpic);
    if (!loaded)
        loaded = gronk_gr("ghed", 0, 1, adr_convpic, move_convpic);
    if (!loaded) pfatal_code(0x3015);
    show(2, 0xc4, PortraitImageDataPtr[0], 0x46, 0x40, 0, 0);
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
    Converse(talking_to->whoami, OBJ_TYPE(talking_to));
    newscr(1);
}

void far free_converse(void)
{
    register int i;
    if (convoWorkspace) {
        release_workspace();
        end_barter();
    }
    set_random_walking_music(-1);
    RightPanel = RightPanelBeforeConversation;
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

/* FM Towns do_escape_key_, also empty, at this position */
void far do_escape_key(void) { }

void far Converse(unsigned char who, int subclass)
{
    register int wait_time;
    if (load_script(cnv_file, convoScreen + 0x400) < 0) {
        scroll_print(get_string(0xe01));
        return;
    }
    bab_fun("babl_menu", babl_menu_ovr103_A13);
    bab_fun("babl_fmenu", babl_fmenu_ovr103_BF2);
    bab_fun("say", npc_say);
    bab_fun("respond", play_respond);
    bab_fun("get_quest", get_quest);
    bab_fun("set_quest", set_quest);
    bab_fun("sex", sex);
    bab_fun("babl_ask", babl_ask_ovr103_1117);
    bab_fun("print", conv_print);
    bab_fun("show_inv", conv_check_inv);
    bab_fun("give_to_npc", conv_give_inv);
    bab_fun("find_inv", conv_find_inv);
    bab_fun("take_from_npc", conv_take_inv);
    bab_fun("take_id_from_npc", conv_take_inv_id);
    bab_fun("identify_inv", conv_inv_name);
    bab_fun("do_offer", do_offer);
    bab_fun("do_demand", do_demand);
    bab_fun("do_decline", do_decline);
    bab_fun("do_judgement", do_judgement);
    bab_fun("end_barter", end_barter);
    bab_fun("setup_to_barter", setup_to_barter);
    bab_fun("pause", pause_ovr103_10DD);
    bab_fun("set_likes_dislikes", npc_likes_dislikes);
    bab_fun("do_inv_create", conv_inv_create);
    bab_fun("do_inv_delete", conv_inv_delete);
    bab_fun("check_inv_quality", check_inv_quality);
    bab_fun("set_inv_quality", set_inv_quality);
    bab_fun("count_inv", count_inv);
    bab_fun("babl_hack", babl_hack);
    bab_fun("give_all_stuff", give_all_stuff);
    bab_fun("gronk_door", gronk_door);
    bab_fun("set_sequence", set_sequence);
    bab_fun("set_attitude", set_attitude);
    bab_fun("set_race_attitude", set_race_attitude);
    bab_fun("take_from_npc_inv", take_from_npc_inv);
    bab_fun("add_to_npc_inv", add_to_npc_inv);
    bab_fun("place_object", place_object);
    bab_fun("transform_talker", transform_talker);
    bab_fun("remove_talker", remove_talker);
    bab_fun("x_skills", x_skills);
    bab_fun("x_traps", x_traps);
    bab_fun("x_obj_stuff", x_obj_stuff);
    bab_fun("x_obj_pos", x_obj_pos);
    bab_fun("find_barter", find_barter);
    bab_fun("find_barter_total", find_barter_total);
    bab_fun("give_ptr_npc", give_ptr_npc);
    bab_fun("x_clock", x_clock);
    bab_fun("x_exp", x_exp);
    bab_fun("teleport_player", teleport_player);
    bab_fun("teleport_talker", teleport_talker);
    bab_fun("switch_pic", switch_pic);
    setup_converse_data(talking_to);
    babl_askstring = bab_malloc(0xa0L);
    if (!OBJ_HAS_INV(talking_to))
        generate_inventory(talking_to);
    babl_run();
    wait_time = 0x1f4;
    if (update_converse_data(talking_to) != 0 || convo_say_flag == 0)
        wait_time = 0;
    j_LikelyWaitForMore_ovr139_29(wait_time, 0);
}

/* FM Towns converse_event_loop_ is this loop (loop_music_maybe, do_changes, the menu,
   input_dispatch); Converse registers babl_menu and babl_fmenu as the two statics after it,
   converse_event_loop_+0xD8 and +0x27C. */
void far converse_event_loop(void)
{
    int i;
    char line[0xa0];
    while (convoContinue) {
        loop_music_maybe();
        do_changes();
        if (menus_active != 0) goto dispatch;
        {
            j_LikelyWaitForMore_ovr139_29(0x1f4, 0);
            do_play_scroll();
            scroll_clear(1);
            set_workspace();
            i = 1;
            while (i < babl_option_count) {
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

int far babl_menu_ovr103_A13(int far *stack)
{
    int i, base, string_no, used;
    char line[0xa0];
    register int row;
    register int lines;
    babl_mode = 1;
    babl_option_count = 1;
    used = lines = 0;
    base = stack[-1];
    string_no = getmem(base);
    while (string_no != 0) {
        babl_options[babl_option_count] = get_string(string_no);
        babl_display[babl_option_count] = convert_string(babl_options[babl_option_count]);
        if (babl_display[babl_option_count] == babl_options[babl_option_count]) {
            babl_display[babl_option_count] = bab_malloc((long)(str_len(babl_options[babl_option_count]) + 1));
            str_copy(babl_display[babl_option_count], babl_options[babl_option_count]);
        }
        babl_option_ids[babl_option_count] = string_no;
        babl_option_count++;
        string_no = getmem(base + babl_option_count - 1);
    }
    do_play_scroll();
    scroll_clear(1);
    set_workspace();
    for (i = 0; i < 10; i++) babl_line_option[i] = -1;
    for (i = 1; i < babl_option_count; i++) {
        line[0] = i + '0';
        line[1] = '.';
        line[2] = ' ';
        str_copy(line + 3, babl_display[i]);
        str_cat(line, "\n");
        lines = scroll_print(line);
        set_workspace();
        for (row = used; row <= lines; row++) babl_line_option[row] = i;
        used = lines + 1;
    }
    do_play_scroll();
    convoContinue = 1;
    menus_active = 1;
    converse_event_loop();
    return selected_conversation_option;
}

int far babl_fmenu_ovr103_BF2(int far *stack)
{
    int i, base, string_no, value, value_base, used;
    char line[0xa0];
    register int row;
    register int lines;
    babl_mode = 1;
    babl_option_count = 1;
    used = 0;
    base = stack[-1];
    value_base = stack[-2];
    string_no = getmem(base);
    value = getmem(value_base);
    i = 1;
    while (string_no != 0) {
        if (value != 0) {
            babl_options[babl_option_count] = get_string(string_no);
            babl_display[babl_option_count] = convert_string(babl_options[babl_option_count]);
            if (babl_display[babl_option_count] == babl_options[babl_option_count]) {
                babl_display[babl_option_count] = bab_malloc((long)(str_len(babl_options[babl_option_count]) + 1));
                str_copy(babl_display[babl_option_count], babl_options[babl_option_count]);
            }
            babl_option_ids[babl_option_count] = string_no;
            babl_option_count++;
        }
        i++;
        string_no = getmem(base + i - 1);
        value = getmem(value_base + i - 1);
    }
    do_play_scroll();
    scroll_clear(1);
    set_workspace();
    for (i = 0; i < 10; i++) babl_line_option[i] = -1;
    for (i = 1; i < babl_option_count; i++) {
        line[0] = i + '0';
        line[1] = '.';
        line[2] = ' ';
        str_copy(line + 3, babl_display[i]);
        str_cat(line, "\n");
        lines = scroll_print(line);
        set_workspace();
        for (row = used; row <= lines; row++) babl_line_option[row] = i;
        used = lines + 1;
    }
    do_play_scroll();
    convoContinue = 1;
    menus_active = 1;
    converse_event_loop();
    return babl_option_ids[selected_conversation_option];
}

void far conv_play_menu(int option)
{
    int y, row;
    register int i;
    register int selected;
    if (babl_mode == 0) return;
    set_workspace();
    if (option == 0) {
        y = ((int *)inplist)[1] + 1;
        row = (0x1e - y) / *(int far *)(cur_font + 6);
        selected = babl_line_option[row];
    } else selected = option;
    if (selected <= 0 || selected >= babl_option_count) return;
    babl_mode = 0;
    convoContinue = 0;
    mouse_release(0);
    do_play_scroll();
    scroll_clear(1);
    set_workspace();
    menus_active = 0;
    do_npc_scroll();
    for (i = 1; i < babl_option_count; i++) {
        if (i == selected) play_say(babl_display[i]);
        if (babl_options[i] != babl_display[i]) bab_free(babl_display[i]);
    }
    do_play_scroll();
    selected_conversation_option = selected;
}

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

void far conv_print(int far *stack)
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

int far pause_ovr103_10DD(int far *stack)
{
    register int duration;
    duration = getmem(stack[-1]);
    mouse_release(0);
    j_LikelyWaitForMore_ovr139_29(duration * 0x1f4, 0);
    set_workspace();
    return 1;
}

int far babl_ask_ovr103_1117(void)
{
    char response[0xa0];
    wdialog(0, 0, response, 1, 0x32);
    scroll_print("\n");
    set_workspace();
    str_copy(babl_askstring, response);
    if (babl_ask_handle == 0)
        babl_ask_handle = make_string(babl_askstring, 0x7c);
    else if (get_string(babl_ask_handle) == 0)
        replace_string(babl_askstring, babl_ask_handle);
    set_workspace();
    return babl_ask_handle;
}

/* The inventory built-ins, in FM Towns order: conv_check_inv (show_inv), find_barter,
   find_barter_total, conv_give_inv (give_to_npc), give_ptr_npc, conv_inv_delete,
   conv_find_inv, conv_take_inv, conv_take_inv_id, conv_inv_create, conv_inv_name
   (identify_inv), count_inv, check_inv_quality, set_inv_quality, switch_pic. Each one's
   callees agree with its FM Towns namesake. */

/* show_inv: the player's offered items, as item ids and object indices */
int far conv_check_inv(int far *stack)
{
    int ids[6], indices[6];
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

int far find_barter(int far *stack)
{
    int count, ids[6], indices[6];
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

int far find_barter_total(int far *stack)
{
    int wanted, matches, ids[5], indices[5], matching[5], count;
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
                if (OBJ_ISQUANT(obj) && !(obj->ol.f.link & 0x200))
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

/* give_to_npc: give the listed objects, if all are among the player's offered items */
int far conv_give_inv(int far *stack)
{
    int count, have, ids[6], indices[6], slot_of[6], item_of[6];
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

/* give the NPC qty of one object, splitting a quantity if need be, from the trade area
   or else from the player's inventory */
int far give_ptr_npc(int far *stack)
{
    int index;
    struct Object far *obj, far *copy;
    int ids[6], indices[6];
    register int i, qty;
    index = getmem(stack[-2]);
    qty = getmem(stack[-1]);
    if (qty <= 0) return 0;
    obj = Obj_IntTMem(index);
    player_barter_items(ids, indices);
    for (i = 0; i < 6; i++) {
        if (ids[i] == 0) continue;
        if (indices[i] != index) continue;
        if ((obj->ol.f.link & 0x200) || !OBJ_ISQUANT(obj) || obj->ol.f.link <= qty)
            player_barter_give(index);
        else {
            copy = Obj_Alloc(0);
            if (copy != 0) {
                *copy = *obj;
                copy->ol.f.link = qty;
                obj->ol.f.link = obj->ol.f.link - qty;
                npc_inv_add(copy);
                RedisplayBarterSlots(1);
            } else return 0;
        }
        return 1;
    }
    if (qty < 0 || !OBJ_ISQUANT(obj) || (obj->ol.f.link & 0x200)) qty = -1;
    if (!invRemoveObject(obj, qty)) return 0;
    npc_inv_add(obj);
    return 1;
}

int far conv_inv_delete(int far *stack) { return npc_inv_delete(getmem(stack[-1])); }

int far conv_find_inv(int far *stack) { return npc_barter_find(getmem(stack[-2]), getmem(stack[-1])); }

int far conv_take_inv(int far *stack) { return npc_barter_give(getmem(stack[-1])); }

int far conv_take_inv_id(int far *stack) { return npc_barter_give_id(getmem(stack[-1])); }

int far conv_inv_create(int far *stack) { return npc_inv_create(getmem(stack[-1])); }

/* identify_inv: the object's name with an article or a count, as a new string, and
   its value */
int far conv_inv_name(int far *stack)
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
    if (OBJ_ISQUANT(obj) && !(obj->ol.f.link & 0x200))
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
    handle = make_string(copy, 0x7c);
    babl_setmem(stack[-2], handle);
    return value;
}

/* how many of an object: its quantity, or 1 */
int far count_inv(int far *stack)
{
    struct Object far *obj;
    register int qty, index;
    index = getmem(stack[-1]);
    obj = Obj_IntTMem(index);
    if (OBJ_ISQUANT(obj) && !(obj->ol.f.link & 0x200)) qty = obj->ol.f.link;
    else qty = 1;
    return qty;
}

/* reads the quality but never returns it: the result is whatever AX holds */
int far check_inv_quality(int far *stack)
{
    struct Object far *obj;
    register int quality;
    int index;
    index = getmem(stack[-1]);
    obj = Obj_IntTMem(index);
    quality = obj->qn.f.quality;
}

int far set_inv_quality(int far *stack)
{
    struct Object far *obj;
    register int index;
    index = getmem(stack[-2]);
    obj = Obj_IntTMem(index);
    obj->qn.f.quality = getmem(stack[-1]) & 0x3f;
    return 1;
}

/* show another portrait for the NPC: a whoami below 0x100, else a generic head */
int far switch_pic(int far *stack)
{
    int which;
    char far *saved;
    unsigned char loaded;
    char name[0x28];
    struct Mobile who;
    which = getmem(stack[-1]);
    loaded = 0;
    set_workspace();
    PortraitData = bab_malloc(0x12c0L);
    saved = PortraitData;
    if (PortraitData == 0) return 0;
    if (which < 0x100)
        loaded = gronk_gr("charhead", which - 1, 1, adr_convpic, move_convpic);
    else if (which <= 0x140)
        loaded = gronk_gr("ghed", which - 0x100, 1, adr_convpic, move_convpic);
    if (!loaded) {
        PortraitData = saved;
        loaded = gronk_gr("ghed", 0, 1, adr_convpic, move_convpic);
    }
    if (loaded) {
        who.w0F = 2;
        who.id = who.id & 0xFE3F | (1 & 7) << 6;
        if (which < 0x100) who.whoami = which;
        else who.id = who.id & 0xFFC0 | ((which - 0x100) & 0x3F) << 0;
        mouse_hide();
        show(2, 0xc4, PortraitImageDataPtr[0], 0x46, 0x40, 0, 0);
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
