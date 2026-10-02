/* conv.h: Conversations: the babl interpreter, its built-ins, bartering, and the bytecode
   assembler's label table. */
#ifndef CONV_H
#define CONV_H

#include "uw2.h"

struct Object;

#include "object.h"

/* OVR095.C: the conversation interpreter ("babl") */
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
int far BabRand_ovr095_A5B(int far *args);
int far bab_compare_ovr095_A8B(int far *args);
int far babPluralize_ovr095_B95(int far *args);
int far StringContains_ovr095_BDB(int far *args);
int far babl_str_append_ovr095_D0A(int far *args);
int far STRING_COPY_ovr095_DC7(int far *args);
int far conv_find_ovr095_E36(int far *args);
int far conv_length_ovr095_E8D(int far *args);
int far DoVal_ovr095_EB2(int far *args);
char far * far bab_malloc(long n);
void far bab_free(char far *data);
int far init_babl(void);
int far * far getmem_addr(int addr);
void far babl_setmem(int addr, int value);
void far bab_var(char *name, int *values, int count);
void far bab_var_out(char *name, int *values, int count);

/* OVR096.C: conversation built-ins that reach into the game */
struct Object far * far place_pitfighter(int power, int x, int y);
void far do_babl_teleport(void);

/* OVR097.C: bartering in conversations */
/* A trade adjustment set by a conversation, read when bartering. Defined in ovr097: it is
   the byte at DS:BFE, and ovr097's word-aligned _DATA starts there. */
extern char fudge;
void far drawTradeSlot_ovr097_A91(int side, int slot);
void far showSelection_ovr097_E83(int side, int slot);
void far UseTradeSlot_ovr097_6E8(int side, int slot, int *content, unsigned char *active);
void far PickUpFromSlot_ovr097_C39(int slot, int *content, unsigned char split);
void far ovr097_CDB(int side, int slot, int *content);
unsigned char far CombineToSlot_ovr097_D30(struct Object far *obj, int side, int slot,
                                           int *content);
int far assess_value(int use_likes, int item, int accuracy);
int far does_npc_like(int index);
int far range(int base, int min, int max);
int far total_offering_ovr097_17CB(int use_likes, int *items, unsigned char *selected, int *values,
                                   int accuracy);
void far npc_inv_add(struct Object far *obj);
extern int npc_assess;
extern int greed;
void far barter_init(void);
void far end_barter(void);
void far conv_inv_special(void);
void far RedisplayBarterSlots(int side);
int far player_barter_items(int *items, int *indices);
void far player_barter_give(int index);
int far npc_barter_find(int item, int from_player);
int far npc_barter_give(int item);
int far npc_barter_give_id(int index);
int far npc_inv_create(int item);
int far npc_inv_delete(int item);

/* OVR103.C: conversations */
extern struct Object far *talking_to;
extern unsigned cnv_id;
char far * far adr_convpic(int n);
int far move_convpic(char far *image, int ok, int which);
void far Converse(unsigned char who, int subclass);
int far conv_choice_ovr103_A13(int far *stack);
int far conv_fmenu_ovr103_BF2(int far *stack);
/* declared before conv_check_inv, because Turbo C lists publics of equal key
   (595 for both) in reverse order of first sight, and the stub order needs this one last */
void far conv_play_menu(int option);
void far npc_say(char far *s);
void far play_respond(char far *s);
void far play_say(char far *s);
void far conv_print(int far *stack);
int far conv_pause_ovr103_10DD(int far *stack);
int far getInputText_ovr103_1117(void);
int far conv_check_inv(int far *stack);
int far conv_give_inv(int far *stack);
int far conv_find_inv(int far *stack);
int far conv_take_inv(int far *stack);
int far conv_take_inv_id(int far *stack);
int far conv_inv_name(int far *stack);
int far conv_inv_create(int far *stack);
int far conv_inv_delete(int far *stack);
int far check_inv_quality(int far *stack);
int far count_inv(int far *stack);
int far find_barter(int far *stack);
int far find_barter_total(int far *stack);
int far give_ptr_npc(int far *stack);
int far switch_pic(int far *stack);
void far TalkTo(struct Object far *thing);
void far free_converse(void);

/* OVR106.C: handing variables to a conversation and taking them back */
void far setup_converse_data(struct Object far *npc);

/* SEG045.C: the label table of the conversation (babl) bytecode assembler */
extern int far *dbptr;
void far grdb_blank(void);
int far Clk(int n);
void far gr_entry(void);
void far gr_tostrt(void);
void far Ref(unsigned char lab, int rel);

/* Defined where no source has it yet: data the link takes from the EXE. */
extern int CutsceneOrConversationStringBlock;

#endif
