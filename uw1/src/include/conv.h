/* conv.h: Conversations: the babl interpreter, its built-ins, bartering, and the bytecode
   assembler's label table. */
#ifndef CONV_H
#define CONV_H

#include "uw2.h"

struct Object;

#include "object.h"

/* BABL.C: the conversation interpreter ("babl") */
int far DoReadHeader_ovr095_12C3(void);
void far DoCopyCode_ovr095_14B1(void);
char far * far convert_string(char far *text);
int far AtIndex_ovr095_11C0(char far **s);
void far vmAdd_ovr095_18CF(void);
void far babl_neg_ovr095_1908(void);
void far MUL_OPCODE_ovr095_192C(void);
void far subOpcode_ovr095_1965(void);
void far BABL_DIV_ovr095_199E(void);
void far BablMod_ovr095_19EE(void);
void far babBitOr_ovr095_1A3E(void);
void far babl_and_ovr095_1A83(void);
void far VM_GT_ovr095_1AC8(void);
void far vmTstge_ovr095_1B0A(void);
void far vmTstlt_ovr095_1B4C(void);
void far VmTstle_ovr095_1B8E(void);
void far ExecTsteq_ovr095_1BD0(void);
void far opcode_tstne_ovr095_1C12(void);
void far babl_call_ovr095_1C54(void);
int far bab_ret_ovr095_1C82(void);
void far exec_fetchm_ovr095_1CAA(void);
void far vmOffset_ovr095_1CD7(void);
void far BABL_STORE_ovr095_1D11(void);
void far talk_calli_ovr095_1D44(void);
void far vm_strcmp_ovr095_1DC1(void);
void far vmSay_ovr095_1EA2(void);
void far babl_respond_ovr095_1F4A(void);
int far getmem(int addr);
int far conv_local_ovr095_2030(int addr);
void far bab_var_clear(void);
int far unbound(void);
int far BabRand_ovr095_A5B(int16 far *args);
int far bab_compare_ovr095_A8B(int16 far *args);
int far babPluralize_ovr095_B95(int16 far *args);
int far StringContains_ovr095_BDB(int16 far *args);
int far babl_str_append_ovr095_D0A(int16 far *args);
int far STRING_COPY_ovr095_DC7(int16 far *args);
int far conv_find_ovr095_E36(int16 far *args);
int far conv_length_ovr095_E8D(int16 far *args);
int far DoVal_ovr095_EB2(int16 far *args);
char far * far bab_malloc(int32 n);
void far bab_free(char far *data);
int far init_babl(void);
int16 far * far getmem_addr(int addr);
void far babl_setmem(int addr, int value);
void far bab_var(char *name, int16 *values, int count);
void far bab_var_out(char *name, int16 *values, int count);
/* A conversation built-in; they take many types, so each is cast where it is bound. */
typedef void (far *BablFn)();
int far load_script(char *name, char far *work);
void far bab_fun(char *name, BablFn fn);  /* binds a built-in by name */
int far babl_run(void);

/* BABLHACK.C: conversation built-ins that reach into the game */
/* The built-ins CONVERSE.C binds with bab_fun, given the argument stack. */
void far set_attitude(int16 far *args);
void far set_race_attitude(int16 far *args);
int far x_skills(int16 far *args);
int far x_traps(int16 far *args);
int far place_object(int16 far *args);
int far take_from_npc_inv(int16 far *args);
void far add_to_npc_inv(int16 far *args);
void far remove_talker(void);
void far set_quest(int16 far *args);
int far get_quest(int16 far *args);
int far sex(int16 far *args);
int far gronk_door(int16 far *args);
void far x_obj_stuff(int16 far *args);
void far x_obj_pos(int16 far *args);

/* BARTER.C: bartering in conversations */
/* A trade adjustment set by a conversation, read when bartering. Defined in ovr097: it is
   the byte at DS:BFE, and ovr097's word-aligned _DATA starts there. */
void far drawTradeSlot_ovr097_A91(int side, int slot);
void far showSelection_ovr097_E83(int side, int slot);
void far UseTradeSlot_ovr097_6E8(int16 side, int16 slot, int16 *content, char *active);
void far PickUpFromSlot_ovr097_C39(int slot, int16 *content, unsigned char split);
void far ovr097_CDB(int side, int slot, int16 *content);
unsigned char far CombineToSlot_ovr097_D30(struct Object far *obj, int side, int slot,
                                           int16 *content);
int far assess_value(int use_likes, int item, int accuracy);
int far does_npc_like(int index);
int far range(int base, int min, int max);
int far total_offering_ovr097_17CB(int use_likes, int16 *items, char *selected, int16 *values,
                                   int accuracy);
void far npc_inv_add(struct Object far *obj);
extern int16 npc_assess;
extern int16 greed;
void far barter_init(void);
void far end_barter(void);
void far conv_inv_special(void);
int far player_barter_items(int16 *items, int16 *indices);
void far player_barter_give(int index);
int far npc_barter_find(int item, int from_player);
int far npc_barter_give(int item);
int far npc_barter_give_id(int index);
int far npc_inv_create(int item);
int far npc_inv_delete(int item);
void far play_barter(void);
int far play_slot_hit_abs(int x, int y);
void far npc_barter(void);
/* The built-ins CONVERSE.C binds with bab_fun. */
void far setup_to_barter(void);
int far do_offer(int16 far *args);
int far do_demand(int16 far *args);
void far do_decline(void);
void far do_judgement(void);
int far npc_likes_dislikes(int16 far *args);

/* CONVERSE.C: conversations */
extern struct Object far *talking_to;
extern uint16 cnv_id;
char far * far adr_convpic(int n);
char far move_convpic(char far *image, int ok, int which);
void far Converse(unsigned char who, int subclass);
int far conv_choice_ovr103_A13(int16 far *stack);
int far conv_fmenu_ovr103_BF2(int16 far *stack);
/* match: declared before conv_check_inv, because Turbo C lists publics of equal key
   (595 for both) in reverse order of first sight, and the stub order needs this one last */
void far conv_play_menu(int option);
void far npc_say(char far *s);
void far play_respond(char far *s);
void far play_say(char far *s);
void far conv_print(int16 far *stack);
int far conv_pause_ovr103_10DD(int16 far *stack);
int far getInputText_ovr103_1117(void);
int far conv_check_inv(int16 far *stack);
int far conv_give_inv(int16 far *stack);
int far conv_find_inv(int16 far *stack);
int far conv_take_inv(int16 far *stack);
int far conv_take_inv_id(int16 far *stack);
int far conv_inv_name(int16 far *stack);
int far conv_inv_create(int16 far *stack);
int far conv_inv_delete(int16 far *stack);
int far check_inv_quality(int16 far *stack);
int far count_inv(int16 far *stack);
int far find_barter(int16 far *stack);
int far find_barter_total(int16 far *stack);
int far give_ptr_npc(int16 far *stack);
void far TalkTo(struct Object far *thing);
void far free_converse(void);
void far strt_converse(void);
void far do_escape_key(void);
int far set_inv_quality(int16 far *stack);

/* CONVVARS.C: handing variables to a conversation and taking them back */
void far setup_converse_data(struct Object far *npc);
char far update_converse_data(struct Object far *npc);

/* GRDB.C: the label table of the conversation (babl) bytecode assembler */
extern int16 far *dbptr;
void far grdb_blank(void);
int far Clk(int n);
void far gr_entry(void);
void far gr_tostrt(void);
void far Ref(unsigned char lab, int rel);
void far gr_putlab(unsigned char lab);

/* Defined where no source has it yet: data the link takes from the EXE. */
extern int16 CutsceneOrConversationStringBlock;

#endif
