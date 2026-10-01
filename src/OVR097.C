/* target: ovr097 */
/* opts: -mm -1 -G -O -Y -d */
/* Bartering in conversations: the trade slots on the conversation screen, the
   conversation built-ins setup_to_barter, do_offer, do_demand, do_decline,
   do_judgement, give_all_stuff and the npc_inv_* family, and how a trader values
   an item. The whole of DOS overlay ovr097, in original order. Names are the
   originals from the FM Towns symbol table where it has them; the functions it
   lacks were static there and keep their IDA names here.

   The two sides of a trade are indexed 0 for the NPC and 1 for the player. */
#include <stdlib.h>
#include <time.h>

struct Link { unsigned low:6, index:10; };
struct Object {
    unsigned id, pos;
    struct Link qn, ol;
    unsigned char hp;                   /* 0x08 */
    char pad9[4];
    unsigned attitude;                  /* 0x0D */
    char pad0F[0x19 - 0x0F];
    unsigned char b19;                  /* 0x19 */
};

/* The common object properties, one 11-byte record per item. */
struct ComObj {
    char pad0[4];
    unsigned short value;               /* 0x04 */
    char pad6[5];
};

/* One critter type's record, 0x30 bytes: its trading temper is in 0x0D-0x0E. The
   field names are mine; FM Towns reads the same nibbles. */
struct Creature {
    char pad00[4];
    unsigned char max_vit;              /* 0x04 */
    char pad05[0x0D - 0x05];
    unsigned wit:4;                     /* 0x0D */
    unsigned shrewd:4;
    unsigned haggle:4;                  /* 0x0E */
    unsigned patience:4;
    char pad0F[0x30 - 0x0F];
};

struct Player {
    char pad0[0x21];
    unsigned char skills[20];           /* 0x21: lore is 8, charm 15, appraise 18 */
    char pad35[0x36 - 0x35];
    unsigned char max_vit;              /* 0x36 */
    char pad37[0x3D - 0x37];
    unsigned char level;                /* 0x3D */
    char pad3E[0x60 - 0x3E];
    unsigned in_combat:1;               /* 0x60 */
};

extern struct Player near *player;
extern struct Creature near *playerdat;
extern int PlayerLevel;
extern char fudge;
extern struct Object far *talking_to;
extern struct Object far *ThePlayer;
extern struct Object far *CursorObjPtr;
extern struct ComObj ComObjData[];
extern struct Creature Creature[];
extern unsigned char far Transparency;
extern int *inplist;
extern int GameInputMode;
extern unsigned char far *foreground_color;

void far generate_inventory(struct Object far *);
struct Object far * far Obj_PtrTMem(struct Link far *);
struct Object far * far Obj_IntTMem(int);
int far Obj_MemTPtr(struct Object far *);
unsigned char far Obj_Rem(struct Link far *, struct Object far *);
void far Obj_Add(struct Link far *, struct Object far *);
void far Obj_AddEnd(struct Link far *, struct Object far *);
void far Obj_FreeLinkChain(struct Link far *, struct Object far *);
struct Object far * far Obj_InList(struct Link far **, int, int, int, int);
void far Obj_Free(struct Object far *);
struct Object far * far CreateObj(int, char);
char far near_mob_put_at(struct Object far *at, struct Object far *obj, int a, int b);
int far set_workspace(void);
struct Object far * far AskHowMany(struct Object far *obj);
unsigned char far EncumCheck(struct Object far *obj);
void far DoInventoryMouse(int how);
void far LookAt(struct Object far *obj, int lore);
void far game_sprint(int id);
int far skill_check(int value, int target);
char far mouse_dragged(int how);
void far mouse_release(int how);
void far unforce_mouse_cursor(int n);
void far force_mouse_cursor(int id);
void far grfx_quikfont(int size);
void far string_to_screen(char far *s, int x, int y);
void far pic_to_screen(int pic, int x, int y, int w, int h);
int far valloc(int, int);
void far vfree(int);
void far save_rect(int, int, int, int, int);
void far restore_rect(int);
void far mouse_hide(void);
void far mouse_show(void);
void far mouse_getxy(int *, int *);
void far set_the_color(int);
void far uhline(int x1, int y, int x2);
void far plot_pixel(int x, int y);
void far uvline(int x, int y1, int y2);
extern unsigned char far *pixel_color;
int far getmem(int);
int far * far getmem_addr(int);
char far * far get_string(int);
void far str_copy(char far *, char far *);
void far str_cat(char far *, char far *);
void far play_say(char far *);
void far npc_say(char far *);
void far bab_var(char *name, int *values, int count);
void far bab_var_out(char *name, int *values, int count);
void far change_critter_goal(struct Object far *npc, char goal, int gtarg);

void far RedrawTradeSlot_ovr097_A91(int side, int slot);
void far SomethingWithTradeSlot_ovr097_E83(int side, int slot);
void far ReturnTradeObjectsToNPC_ovr097_F76(int only_unselected);
void far LikelySlotInteractionRelated_ovr097_6E8(int side, int slot, int *content,
                                                 unsigned char *active);
void far SetObjectInHand_ovr097_C39(int slot, int *content, unsigned char split);
void far ovr097_CDB(int side, int slot, int *content);
unsigned char far ObjectCombineToSlot_ovr097_D30(struct Object far *obj, int side, int slot, int *content);
int far assess_value(int, int, int);
int far does_npc_like(int);
int far range(int, int, int);
int far GetTotalValueOfOffering_ovr097_17CB(int, int *, unsigned char *, int *, int);
void far npc_inv_add(struct Object far *);

/* This file's uninitialised data. FM Towns names greed and npc_assess, which are public;
   the rest were static there and their names here were chosen so that Turbo C lays
   them out in _BSS where the EXE has them (it orders a file's _BSS by a hash of the
   name, see tools/bssorder.py). */
static unsigned char barter_result;     /* the last offer or demand succeeded */
static unsigned char barter_selected[2][6];
static int npc_wit;                     /* worked out by barter_init, never read */
static int far *npc_likes;              /* lists set by npc_likes_dislikes, ending in -1 */
int npc_assess;                         /* how far off the NPC's appraisals may be, in % */
static int patience;                    /* bad offers left before the NPC gives up */
static int far *npc_dislikes;
int greed;                              /* the % gain the NPC wants from a trade */
static int last_offer;                  /* the % gain of the previous offer */
static int barter_vals[2][2][6];        /* the player's and the NPC's valuations, -1 unknown */
static int barter_saves[2][6];          /* what was on screen under each slot */
static int barter_items[2][6];          /* the object in each slot, 0 for none */

/* Conversation built-in: fill the NPC's trade slots from its inventory. */
void far setup_to_barter(void)
{
    struct Object far *obj;
    struct Object far *in_slot;
    struct Object far *next;
    struct Link far *npc_link;
    unsigned char skipped_weapon, all_filled;
    register int slot;
    register int count;

    count = 0;
    skipped_weapon = 0;
    all_filled = 0;
    if (((talking_to->attitude & 0x1000) >> 12) == 0)
        generate_inventory(talking_to);
    npc_link = &talking_to->ol;
    obj = Obj_PtrTMem(&talking_to->ol);
    slot = 0;
    in_slot = 0;
    while (obj && obj != in_slot && count++ < 0x28) {
        next = Obj_PtrTMem(&obj->qn);
        /* Skip the first weapon, anything worthless and, once every slot has been
           filled, a random five in eight of the rest. */
        if ((((obj->id & 0x1F0) >> 4) == 0 && !skipped_weapon) ||
            ComObjData[obj->id & 0x1FF].value == 0 ||
            (all_filled && (rand() & 7) < 5)) {
            if (((obj->id & 0x1F0) >> 4) == 0) skipped_weapon = 1;
            obj = next;
        } else {
            /* the same object is tried again if it can't be unlinked */
            if (!Obj_Rem(npc_link, obj)) continue;
            if (barter_items[0][slot]) {
                if (!in_slot) in_slot = Obj_IntTMem(barter_items[0][slot]);
                Obj_Add(&talking_to->ol, Obj_IntTMem(barter_items[0][slot]));
            }
            barter_items[0][slot] = Obj_MemTPtr(obj);
            RedrawTradeSlot_ovr097_A91(0, slot);
            slot++;
            if (slot >= 6) { slot = 0; all_filled = 1; }
            obj = next;
        }
    }
    set_workspace();
}

/* Set up the slots and the NPC's trading temper when a conversation starts. */
void far barter_init(void)
{
    int charm;
    struct Creature *crit;
    register int side;
    register int slot;

    crit = &Creature[(talking_to->id & 0x3F) >> 0];
    Transparency = 1;
    for (slot = 0; slot < 6; slot++)
        for (side = 0; side < 2; side++)
            barter_saves[side][slot] = valloc(16, 16);
    for (slot = 0; slot < 6; slot++) {
        save_rect(barter_saves[1][slot], (slot & 1) * 0x13 + 0x7C, 0xBA - slot / 2 * 0x12, 16, 16);
        save_rect(barter_saves[0][slot], (slot & 1) * 0x13 + 0x4B, 0xBA - slot / 2 * 0x12, 16, 16);
    }
    for (slot = 0; slot < 6; slot++)
        for (side = 0; side < 2; side++) {
            barter_items[side][slot] = 0;
            barter_vals[side][0][slot] = -1;
            barter_vals[side][1][slot] = -1;
            barter_selected[side][slot] = 0;
            SomethingWithTradeSlot_ovr097_E83(side, slot);
        }
    barter_result = 0;
    srand(Obj_MemTPtr(talking_to));
    greed = range(crit->haggle * 6, -25, 25);
    patience = range(crit->patience, -20, 100);
    npc_assess = range((15 - crit->shrewd) * 6, -25, 50);
    npc_wit = range(crit->wit, -20, 20);
    last_offer = 0;
    charm = player->skills[15];
    greed = greed - charm * 2;
    patience += charm >> 1;
    npc_wit -= charm / 6;
    if (npc_wit < 1) npc_wit = 1;
    npc_likes = 0;
    npc_dislikes = 0;
}

/* Give back whatever is still in the slots when the conversation ends. */
void far end_barter(void)
{
    register int side;
    register int slot;

    mouse_hide();
    for (slot = 0; slot < 6; slot++)
        for (side = 0; side < 2; side++) {
            if (barter_items[side][slot] > 0) {
                near_mob_put_at(side == 1 ? ThePlayer : talking_to,
                                Obj_IntTMem(barter_items[side][slot]), 5, 0);
                restore_rect(barter_saves[side][slot]);
                if (barter_selected[side][slot]) {
                    barter_selected[side][slot] = 0;
                    SomethingWithTradeSlot_ovr097_E83(side, slot);
                }
            }
            vfree(barter_saves[side][slot]);
        }
    mouse_show();
    set_workspace();
}

/* Mouse handlers for the player's and the NPC's slot areas. */
void far play_barter(void)
{
    int x;
    register int y;
    register int slot;

    x = inplist[0] + 0x77;
    y = inplist[1] + 0x87;
    if ((slot = play_slot_hit_abs(x, y)) >= 0) {
        LikelySlotInteractionRelated_ovr097_6E8(1, slot, barter_items[1], barter_selected[1]);
        set_workspace();
    }
}

/* Which of the player's slots is at x, y, or -1. */
int far play_slot_hit_abs(int x, int y)
{
    register int i;
    register int found;
    found = -1;
    for (i = 0; i < 6; i++) {
        if ((i & 1) * 0x13 + 0x7C <= x &&
            (i & 1) * 0x13 + 0x8C >= x &&
            0xBA - (i / 2) * 0x12 >= y &&
            0xBA - (i / 2) * 0x12 - 0x10 <= y) {
            found = i;
            break;
        }
    }
    return found;
}

/* Which of the NPC's slots is at x, y, or -1. */
int far npc_slot_hit_abs(int x, int y)
{
    register int i;
    register int found;
    found = -1;
    for (i = 0; i < 6; i++) {
        if ((i & 1) * 0x13 + 0x4B <= x &&
            (i & 1) * 0x13 + 0x5B >= x &&
            0xBA - (i / 2) * 0x12 >= y &&
            0xBA - (i / 2) * 0x12 - 0x10 <= y) {
            found = i;
            break;
        }
    }
    return found;
}

/* Which slot of either side is at x, y: 1 with the side, slot and that side's arrays
   filled in, or 0. Unnamed (static) in the FM Towns build, which has it straight
   after npc_slot_hit_abs. */
int far ovr097_5F0(int x, int y, int *side, int *slot,
                   int **content, register unsigned char **active)
{
    register int i;
    if ((i = play_slot_hit_abs(x, y)) > -1) {
        *side = 1;
        *slot = i;
        *content = barter_items[1];
        *active = barter_selected[1];
        return 1;
    }
    if ((i = npc_slot_hit_abs(x, y)) > -1) {
        *side = 0;
        *slot = i;
        *content = barter_items[0];
        *active = barter_selected[0];
        return 1;
    }
    return 0;
}

/* A click on the player's slots during a conversation, at the mouse position. */
void far conv_inv_special(void)
{
    int x, y;
    register int slot;
    mouse_getxy(&x, &y);
    slot = play_slot_hit_abs(x, y);
    if (slot > -1)
        LikelySlotInteractionRelated_ovr097_6E8(1, slot, barter_items[1], barter_selected[1]);
    set_workspace();
}

void far npc_barter(void)
{
    int x;
    register int y;
    register int slot;
    x = inplist[0] + 0x46;
    y = inplist[1] + 0x87;
    if ((slot = npc_slot_hit_abs(x, y)) >= 0) {
        LikelySlotInteractionRelated_ovr097_6E8(0, slot, barter_items[0], barter_selected[0]);
        set_workspace();
    }
}

/* A click on a trade slot: pick up what is there (asking how many of a stack), put
   down what is held, toggle the slot's selection, or look at the item. */
void far LikelySlotInteractionRelated_ovr097_6E8(int side, int slot, int *content,
                                                 unsigned char *active)
{
    struct Object far *found;
    struct Object far *moved;
    int x;
    int y;
    unsigned char had_cursor;
    struct Object far *obj;
    register int lore;

    moved = 0;
    had_cursor = CursorObjPtr != 0;
    if (CursorObjPtr == 0 && content[slot] == 0) return;
    if (CursorObjPtr == 0 && mouse_dragged(1)) {
        if (side == 0 && barter_result == 0) return;
        found = Obj_IntTMem(content[slot]);
        if (((found->id & 0x8000) >> 15) && !(found->ol.index & 0x200) && found->ol.index != 1) {
            if ((moved = AskHowMany(found)) == 0) return;
        }
        if (moved && moved != found) Obj_Add(&found->qn, moved);
        if (!EncumCheck(found)) {
            if (moved && moved != found) {
                found->ol.index += moved->ol.index;
                if (Obj_Rem(&found->qn, moved)) Obj_Free(moved);
            }
            game_sprint(0x10C);
            return;
        }
        had_cursor = 1;
        SetObjectInHand_ovr097_C39(slot, content, moved && moved != found);
        RedrawTradeSlot_ovr097_A91(side, slot);
        active[slot] = 0;
        barter_vals[1][0][slot] = -1;
        barter_vals[1][1][slot] = -1;
        SomethingWithTradeSlot_ovr097_E83(side, slot);
        if (CursorObjPtr == 0) return;
        if (moved) {
            GameInputMode = 1;
            return;
        }
        mouse_release(1);
        mouse_getxy(&x, &y);
        if (!ovr097_5F0(x, y, &side, &slot, &content, &active)) {
            GameInputMode = 1;
            DoInventoryMouse(-1);
            return;
        }
    }
    if (CursorObjPtr) {
        mouse_release(1);
        GameInputMode = 1;
        if (side == 0) return;
        ovr097_CDB(side, slot, content);
        RedrawTradeSlot_ovr097_A91(side, slot);
        active[slot] = 1;
        SomethingWithTradeSlot_ovr097_E83(side, slot);
        barter_vals[1][0][slot] = -1;
        barter_vals[1][1][slot] = -1;
    } else if (inplist[3] & 1) {
        active[slot] = !active[slot];
        SomethingWithTradeSlot_ovr097_E83(side, slot);
    } else {
        lore = 1;
        obj = Obj_IntTMem(content[slot]);
        if (side == 0) {
            if (skill_check(player->skills[8], 20) > 0) lore++;
        } else lore += skill_check(player->skills[8], 15);
        LookAt(obj, lore);
    }
    if (had_cursor && CursorObjPtr == 0) {
        unforce_mouse_cursor(3);
        GameInputMode = 0;
    }
}

void far RedisplayBarterSlots(int side)
{
    register int i;
    for (i = 0; i < 6; i++) RedrawTradeSlot_ovr097_A91(side, i);
}

/* Draw what is in one trade slot, with a count for a stack of more than one. */
void far RedrawTradeSlot_ovr097_A91(int side, register int slot)
{
    int item;
    int qty;
    struct Object far *obj;
    char num[6];
    register int index;

    mouse_hide();
    Transparency = 1;
    index = barter_items[side][slot];
    if (index) {
        obj = Obj_IntTMem(index);
        item = obj->id & 0x1FF;
    }
    restore_rect(barter_saves[side][slot]);
    if (side) {
        if (index) pic_to_screen(item, (slot & 1) * 0x13 + 0x7C, 0xBA - slot / 2 * 0x12, 16, 16);
    } else if (index) pic_to_screen(item, (slot & 1) * 0x13 + 0x4B, 0xBA - slot / 2 * 0x12, 16, 16);
    if (index) {
        if (((obj->id & 0x8000) >> 15) && !(obj->ol.index & 0x200)) qty = obj->ol.index;
        else qty = 0;
        Transparency = 0;
        if (qty > 1) {
            grfx_quikfont(0);
            *foreground_color = 2;
            if (side)
                string_to_screen(itoa(qty, num, 10), (slot & 1) * 0x13 + 0x7F, 0xBA - slot / 2 * 0x12 - 1);
            else
                string_to_screen(itoa(qty, num, 10), (slot & 1) * 0x13 + 0x4E, 0xBA - slot / 2 * 0x12 - 1);
            grfx_quikfont(1);
        }
    }
    mouse_show();
    set_workspace();
}

/* Pick up what is in a slot; with split, the rest of a divided stack stays there. */
void far SetObjectInHand_ovr097_C39(register int slot, register int *content, unsigned char split)
{
    unsigned char had_cursor;

    had_cursor = CursorObjPtr != 0;
    CursorObjPtr = Obj_IntTMem(content[slot]);
    content[slot] = 0;
    if (CursorObjPtr) {
        if (split) content[slot] = Obj_MemTPtr(Obj_PtrTMem(&CursorObjPtr->qn));
        mouse_hide();
        if (had_cursor) unforce_mouse_cursor(0);
        force_mouse_cursor(CursorObjPtr->id & 0x1FF);
        mouse_show();
        set_workspace();
    }
}

/* Put what is held into a slot, or onto a stack already there. */
void far ovr097_CDB(int side, register int slot, register int *content)
{
    if (content[slot] == 0) {
        content[slot] = Obj_MemTPtr(CursorObjPtr);
        CursorObjPtr = 0;
    } else if (ObjectCombineToSlot_ovr097_D30(CursorObjPtr, side, slot, content))
        CursorObjPtr = 0;
}

/* Add a held stack to the like stack in a slot, or else swap the two. Returns 1 when
   the held object was merged and freed. */
unsigned char far ObjectCombineToSlot_ovr097_D30(struct Object far *obj, int side,
                                                 register int slot, int *content)
{
    struct Object far *found;
    unsigned char merged;
    register int held;

    merged = 0;
    found = Obj_IntTMem(content[slot]);
    if (((found->id & 0x1C0) >> 6) == 2 && ((found->id & 0x30) >> 4) == 0)
        return 0;
    if (((obj->id & 0x8000) >> 15) && ((found->id & 0x8000) >> 15) &&
        !(obj->ol.index & 0x200) && !(found->ol.index & 0x200) &&
        (obj->id & 0x1FF) == (found->id & 0x1FF) &&
        obj->ol.index + found->ol.index < 0x3E7) {
        found->ol.index += obj->ol.index;
        Obj_Free(obj);
        merged = 1;
    } else {
        held = Obj_MemTPtr(CursorObjPtr);
        SetObjectInHand_ovr097_C39(slot, content, 0);
        content[slot] = held;
    }
    RedrawTradeSlot_ovr097_A91(side, slot);
    set_workspace();
    return merged;
}

/* Draw the small selection mark beside a slot: lit when it is selected. */
void far SomethingWithTradeSlot_ovr097_E83(int side, int slot)
{
    int color;
    register int x, y;

    y = 0xBC - slot / 2 * 0x12 - 10;
    if (slot & 1) x = 0x71;
    else x = 0x47;
    if (side) x += 0x31;
    mouse_hide();
    color = barter_selected[side][slot] ? 0x23 : 1;
    set_the_color(color);
    uhline(x - 1, y + 1, x + 1);
    if (color != 1) color++;
    *pixel_color = color;
    plot_pixel(x, y + 2);
    color = barter_selected[side][slot] ? 0x21 : 1;
    set_the_color(color);
    uhline(x - 1, y, x + 1);
    uvline(x, y + 1, y - 1);
    mouse_show();
    set_workspace();
}

/* Put the NPC's slot items back in its inventory: all of them, or with only_unselected
   just those not selected. */
void far ReturnTradeObjectsToNPC_ovr097_F76(int only_unselected)
{
    struct Object far *obj;
    register int slot;
    register int flag;
    flag = only_unselected;
    mouse_hide();
    for (slot = 0; slot < 6; slot++) {
        if (barter_items[0][slot] > 0) {
            if (!flag || barter_selected[0][slot] == 0) {
                obj = Obj_IntTMem(barter_items[0][slot]);
                Obj_Add(&talking_to->ol, obj);
                restore_rect(barter_saves[0][slot]);
                barter_selected[0][slot] = 0;
                barter_items[0][slot] = 0;
                SomethingWithTradeSlot_ovr097_E83(0, slot);
            }
        }
    }
    mouse_show();
    set_workspace();
}

/* Hand the NPC every selected item of the player's that it doesn't dislike, adding
   coins to a pile it already has. */
void far probablyTradeObjects_ovr097_100A(void)
{
    struct Object far *obj;
    struct Object far *other;
    register int slot;

    mouse_hide();
    for (slot = 0; slot < 6; slot++) {
        if (barter_items[1][slot] > 0 && barter_selected[1][slot] &&
            does_npc_like(barter_items[1][slot]) != -1) {
            obj = Obj_IntTMem(barter_items[1][slot]);
            other = Obj_PtrTMem(&talking_to->ol);
            if ((obj->id & 0x1FF) == 0xA0)
                for (; other; other = Obj_PtrTMem(&other->qn)) {
                    if (((obj->id & 0x8000) >> 15) && ((other->id & 0x8000) >> 15) &&
                        !(obj->ol.index & 0x200) && !(other->ol.index & 0x200) &&
                        (obj->id & 0x1FF) == (other->id & 0x1FF) &&
                        obj->ol.index + other->ol.index < 0x3E7) {
                        other->ol.index += obj->ol.index;
                        Obj_Free(obj);
                        obj = 0;
                        break;
                    }
                }
            if (obj) Obj_AddEnd(&talking_to->ol, obj);
            restore_rect(barter_saves[1][slot]);
            barter_selected[1][slot] = 0;
            barter_items[1][slot] = 0;
            SomethingWithTradeSlot_ovr097_E83(1, slot);
        }
    }
    mouse_show();
    set_workspace();
}

/* 1 when no slot of a side is both filled and selected. */
unsigned char far nothing_there(int *content, unsigned char *selected)
{
    register int i;
    for (i = 0; i < 6; i++)
        if (selected[i] > 0 && content[i] > 0) return 0;
    return 1;
}

/* The player offers the selected items for the NPC's selected items. The five
   arguments are the strings to say: accepted, not good enough, worse than before,
   out of patience and nothing offered. Returns 1 when the trade is made. */
int far do_offer(int far *args)
{
    int player_value;
    int yes_str, no_str, worse_str, tired_str, none_str;
    register int eval;
    register int npc_value;

    set_workspace();
    yes_str = getmem(args[-5]);
    no_str = getmem(args[-4]);
    worse_str = getmem(args[-3]);
    tired_str = getmem(args[-2]);
    none_str = getmem(args[-1]);
    if (patience < 0) {
        npc_say(get_string(tired_str));
        return 0;
    }
    if (nothing_there(barter_items[1], barter_selected[1]) ||
        nothing_there(barter_items[0], barter_selected[0])) {
        npc_say(get_string(none_str));
        barter_result = 0;
        return 0;
    }
    player_value = GetTotalValueOfOffering_ovr097_17CB(1, barter_items[1], barter_selected[1],
                                                       barter_vals[1][1], npc_assess);
    npc_value = GetTotalValueOfOffering_ovr097_17CB(0, barter_items[0], barter_selected[0],
                                                    barter_vals[0][1], npc_assess);
    if (npc_value > 0) eval = (player_value - npc_value) * 100 / npc_value;
    else eval = 100;
    if (eval >= greed) {
        npc_say(get_string(yes_str));
        ReturnTradeObjectsToNPC_ovr097_F76(1);
        probablyTradeObjects_ovr097_100A();
        barter_result = 1;
        return 1;
    }
    if (last_offer == 0 && eval * 2 < greed) {
        npc_say(get_string(no_str));
        patience--;
    } else if (eval < last_offer) {
        npc_say(get_string(worse_str));
        patience -= 2;
    } else if ((greed - last_offer) * 3 / 2 > greed - eval) {
        npc_say(get_string(no_str));
        patience--;
    }
    last_offer = eval;
    return 0;
}

/* The player demands the NPC's selected items for nothing. The three arguments are
   the strings to say: nothing selected, given up and refused. Whether the NPC gives in
   weighs the player's health, level, readiness to fight and charm against the NPC's
   health, temper and mood and the value demanded. Returns 1 when it gives in. */
int far do_demand(int far *args)
{
    int player_score;
    int npc_score;
    int armed;
    int attitude;
    int insist_str;
    int wont_str;
    int what_str;
    struct Creature *crit;
    int demanded;
    register int health;
    register int mood;

    insist_str = getmem(args[-2]);
    wont_str = getmem(args[-1]);
    what_str = getmem(args[-3]);
    if (nothing_there(barter_items[0], barter_selected[0])) {
        bab_var_out("npc_attitude", &attitude, 1);
        if (attitude > 1) {
            attitude--;
            bab_var("npc_attitude", &attitude, 1);
        }
        npc_say(get_string(what_str));
        ReturnTradeObjectsToNPC_ovr097_F76(0);
        barter_result = 0;
        return 0;
    }
    crit = &Creature[(talking_to->id & 0x3F) >> 0];
    if (playerdat->max_vit > 0)
        health = 2 - (player->max_vit - ThePlayer->hp) * 2 / playerdat->max_vit;
    else health = 1;
    armed = player->in_combat;
    player_score = player->level + armed + health + player->skills[15] / 6;
    demanded = GetTotalValueOfOffering_ovr097_17CB(0, barter_items[0], barter_selected[0],
                                                   barter_vals[0][1], npc_assess);
    if (Creature[(talking_to->id & 0x3F) >> 0].max_vit > 0)
        health = 2 - (Creature[(talking_to->id & 0x3F) >> 0].max_vit - talking_to->hp) * 2 /
                     Creature[(talking_to->id & 0x3F) >> 0].max_vit;
    else health = 1;
    bab_var_out("npc_attitude", &attitude, 1);
    if ((talking_to->b19 & 0x40) >> 6) mood = -1;
    else if (attitude < 2) mood = 1;
    else mood = 0;
    npc_score = crit->wit + mood + health + demanded / 10;
    if (PlayerLevel == 9 || PlayerLevel == 0x11)
        npc_score = npc_score * 3 / 2;
    if (player_score > npc_score) {
        npc_say(get_string(insist_str));
        ReturnTradeObjectsToNPC_ovr097_F76(1);
        barter_result = 1;
        if (attitude > 1) {
            attitude--;
            bab_var("npc_attitude", &attitude, 1);
        }
        return 1;
    }
    npc_say(get_string(wont_str));
    ReturnTradeObjectsToNPC_ovr097_F76(0);
    change_critter_goal(talking_to, 5, 1);
    return 0;
}

/* Conversation built-ins: decline, and the player's own appraisal of the offer. */
void far do_decline(void)
{
    ReturnTradeObjectsToNPC_ovr097_F76(0);
}

void far do_judgement(void)
{
    int skill, player_value, npc_value, accuracy, certainty;
    char appraisal[80];
    register int evaluation;
    register int result;

    skill = player->skills[18];
    accuracy = 50 - skill * 45 / 30;
    player_value = GetTotalValueOfOffering_ovr097_17CB(0, barter_items[1],
                    barter_selected[1], barter_vals[1][0], accuracy);
    npc_value = GetTotalValueOfOffering_ovr097_17CB(0, barter_items[0],
                    barter_selected[0], barter_vals[0][0], accuracy);
    npc_value += fudge;
    if (npc_value > 0) evaluation = (player_value - npc_value) * 100 / npc_value;
    else evaluation = 100;
    if (skill < 6) certainty = 0;
    else if (skill < 12) certainty = 1;
    else if (skill < 18) certainty = 2;
    else if (skill < 24) certainty = 3;
    else certainty = 4;
    if (evaluation > 50) result = 0;
    else if (evaluation > 35) result = 1;
    else if (evaluation > 25) result = 2;
    else if (evaluation > 10) result = 3;
    else if (evaluation > -10) result = 4;
    else if (evaluation > -25) result = 5;
    else if (evaluation > -35) result = 6;
    else if (evaluation > -50) result = 7;
    else result = 8;
    str_copy(appraisal, get_string((certainty + 3) | 0xE00));
    str_cat(appraisal, get_string(0xE02));
    str_cat(appraisal, get_string((result + 8) | 0xE00));
    play_say(appraisal);
}

/* The total value of a side's selected items, valuing each once and caching it in
   values. With use_likes the NPC's likes and dislikes count. */
int far GetTotalValueOfOffering_ovr097_17CB(int use_likes, int *items,
                                            unsigned char *selected,
                                            int *values, int accuracy)
{
    register int i;
    register int total;
    total = 0;
    for (i = 0; i < 6; i++) {
        if (selected[i] > 0 && items[i] > 0) {
            if (values[i] == -1)
                values[i] = assess_value(use_likes, items[i], accuracy);
            total += values[i];
        }
    }
    return total;
}

/* An object's value, scaled by quantity and quality and blurred by up to accuracy %,
   the same each time for the same object. */
int far assess_value(int use_likes, int item, int accuracy)
{
    struct Object far *obj;
    int quantity;
    int disposition;
    register int value;
    register int quality;

    obj = Obj_IntTMem(item);
    if (use_likes) {
        disposition = does_npc_like(item);
        if (disposition == -1) return 0;
        value = ComObjData[obj->id & 0x1FF].value;
        if (disposition) value = value * 3 >> 1;
    } else value = ComObjData[obj->id & 0x1FF].value;
    if (((obj->id & 0x8000) >> 15) && !(obj->ol.index & 0x200))
        quantity = obj->ol.index;
    else quantity = 1;
    value *= quantity;
    quality = obj->qn.low;
    if ((obj->id & 0x1FF) == 0xA0) quality = 0x3F;
    if (value > 0) {
        if (quality > 0) {
            value = (int)(((long)value * quality) >> 6);
            if (value == 0) value = 1;
        } else value = 0;
    }
    srand(item);
    value = range(value, -accuracy, accuracy);
    srand(time(0));
    return value;
}

int far range(int base, int min, int max)
{
    return base + base * (min + (int)(((long)rand() * (max - min)) / 0x8000L)) / 100;
}

/* The item types and indexes of the player's selected items; returns how many. */
int far player_barter_items(int *items, int *indices)
{
    struct Object far *obj;
    register int slot;
    register int count;

    slot = 0;
    count = 0;
    for (; slot < 6; slot++) {
        if (barter_selected[1][slot]) {
            obj = Obj_IntTMem(barter_items[1][slot]);
            indices[count] = barter_items[1][slot];
            items[count] = obj->id & 0x1FF;
            count++;
        }
    }
    return count;
}

/* Give an object to the NPC, adding coins to a pile it already has. */
void far npc_inv_add(struct Object far *obj)
{
    struct Object far *other;
    if ((obj->id & 0x1FF) == 0xA0) {
        for (other = Obj_PtrTMem(&talking_to->ol); other;
             other = Obj_PtrTMem(&other->qn)) {
            if (((obj->id & 0x8000) >> 15) &&
                ((other->id & 0x8000) >> 15) &&
                !(obj->ol.index & 0x200) &&
                !(other->ol.index & 0x200) &&
                (obj->id & 0x1FF) == (other->id & 0x1FF) &&
                obj->ol.index + other->ol.index < 0x3E7) {
                other->ol.index += obj->ol.index;
                Obj_Free(obj);
                obj = 0;
                break;
            }
        }
    }
    if (obj) Obj_Add(&talking_to->ol, obj);
}

/* Conversation built-in: the player gives an object from the slots to the NPC. */
void far player_barter_give(int index)
{
    register int slot;
    register int objindex;
    objindex = index;
    npc_inv_add(Obj_IntTMem(objindex));
    mouse_hide();
    for (slot = 0; slot < 6; slot++) {
        if (barter_items[1][slot] == objindex) {
            restore_rect(barter_saves[1][slot]);
            barter_items[1][slot] = 0;
            barter_selected[1][slot] = 0;
            SomethingWithTradeSlot_ovr097_E83(1, slot);
        }
    }
    mouse_show();
    set_workspace();
}

/* Conversation built-in: find an item (or above 999 a class) in the NPC's or the
   player's inventory; returns its index. */
int far npc_barter_find(int item, int from_player)
{
    struct Object far *found;
    struct Link far *head;
    int major, minor;
    register int id;
    register int cls;
    id = item;
    if (from_player) head = &ThePlayer->ol;
    else {
        if (((talking_to->attitude & 0x1000) >> 12) == 0)
            generate_inventory(talking_to);
        head = &talking_to->ol;
    }
    if (id > 0x3E7) {
        major = (id - 0x3E8) >> 2;
        minor = (id - 0x3E8) & 3;
        cls = -1;
    } else {
        major = id >> 6;
        minor = (id & 0x30) >> 4;
        cls = id & 0xF;
    }
    found = Obj_InList(&head, 1, major, minor, cls);
    return Obj_MemTPtr(found);
}

/* Conversation built-in: the NPC hands over an item of the given type (or, above
   999, of class item - 1000): into the player's hand, else into a free trade slot,
   else dropped at the NPC's feet. Returns 1, 2 when dropped, 3 when even that fails,
   or 0 when there is none or the player's hand is full. */
int far npc_barter_give(register int item)
{
    struct Object far *obj;
    struct Link far *head;
    register int slot;

    if (CursorObjPtr) return 0;
    if (((talking_to->attitude & 0x1000) >> 12) == 0)
        generate_inventory(talking_to);
    head = &talking_to->ol;
    for (obj = Obj_PtrTMem(&talking_to->ol); obj; obj = Obj_PtrTMem(&obj->qn)) {
        if (item > 0x3E7) {
            if (((obj->id & 0x1F0) >> 4) != item - 1000) continue;
        } else if ((obj->id & 0x1FF) != item) continue;
        Obj_Rem(head, obj);
        if (EncumCheck(obj)) {
            CursorObjPtr = obj;
            mouse_hide();
            force_mouse_cursor(obj->id & 0x1FF);
            GameInputMode = 1;
            mouse_show();
            set_workspace();
            return 1;
        }
        for (slot = 0; slot < 6; slot++)
            if (barter_items[1][slot] == 0) {
                barter_items[1][slot] = Obj_MemTPtr(obj);
                barter_selected[1][slot] = 0;
                barter_vals[1][0][slot] = barter_vals[0][1][slot] = -1;
                SomethingWithTradeSlot_ovr097_E83(0, slot);
                RedrawTradeSlot_ovr097_A91(1, slot);
                return 1;
            }
        if (near_mob_put_at(talking_to, obj, 5, 0)) return 2;
        return 3;
    }
    return 0;
}

/* The same for one particular object, by its index. */
int far npc_barter_give_id(register int index)
{
    struct Object far *obj;
    struct Link far *head;
    register int slot;

    if (CursorObjPtr) return 0;
    head = &talking_to->ol;
    for (obj = Obj_PtrTMem(&talking_to->ol); obj; obj = Obj_PtrTMem(&obj->qn)) {
        if (Obj_MemTPtr(obj) != index) continue;
        Obj_Rem(head, obj);
        if (EncumCheck(obj)) {
            CursorObjPtr = obj;
            mouse_hide();
            force_mouse_cursor(obj->id & 0x1FF);
            GameInputMode = 1;
            mouse_show();
            set_workspace();
            return 1;
        }
        for (slot = 0; slot < 6; slot++)
            if (barter_items[1][slot] == 0) {
                barter_items[1][slot] = Obj_MemTPtr(obj);
                barter_selected[1][slot] = 0;
                barter_vals[1][0][slot] = barter_vals[0][1][slot] = -1;
                SomethingWithTradeSlot_ovr097_E83(0, slot);
                RedrawTradeSlot_ovr097_A91(1, slot);
                return 1;
            }
        if (near_mob_put_at(talking_to, obj, 5, 0)) return 2;
        return 3;
    }
    return 0;
}

/* Conversation built-ins: create an item in, or delete one from, the NPC's inventory. */
int far npc_inv_create(int item)
{
    struct Object far *obj;
    obj = CreateObj(item, 0);
    if (!obj) return 0;
    obj->qn.low = 0x3F;
    if (obj) Obj_Add(&talking_to->ol, obj);
    return Obj_MemTPtr(obj);
}

int far npc_inv_delete(int item)
{
    struct Object far *obj;
    struct Link far *head;
    register int id;
    id = item;
    head = &talking_to->ol;
    for (obj = Obj_PtrTMem(&talking_to->ol); obj; obj = Obj_PtrTMem(&obj->qn)) {
        if ((obj->id & 0x1FF) == id) {
            Obj_FreeLinkChain(head, obj);
            return 1;
        }
    }
    return 0;
}

/* Conversation built-in: point at the NPC's lists of liked and disliked items. */
int far npc_likes_dislikes(int far *args)
{
    npc_likes = getmem_addr(args[-2]);
    npc_dislikes = getmem_addr(args[-1]);
    return 1;
}

/* 1 when the NPC likes an object, -1 when it dislikes it or it is worthless, else 0.
   List entries of 1000 and up name a class, (item >> 4) + 1000. */
int far does_npc_like(int index)
{
    struct Object far *obj;
    int liked;
    int cls;
    register int id;
    register int i;

    obj = Obj_IntTMem(index);
    if (ComObjData[obj->id & 0x1FF].value == 0) return -1;
    id = obj->id & 0x1FF;
    cls = (id >> 4) + 0x3E8;
    liked = 0;
    if (npc_likes)
        for (i = 0; npc_likes[i] > -1; i++) {
            if (npc_likes[i] < 0x3E8) {
                if (npc_likes[i] == id) return 1;
            } else if (npc_likes[i] == cls) liked = 1;
        }
    if (npc_dislikes)
        for (i = 0; npc_dislikes[i] > -1; i++) {
            if (npc_dislikes[i] < 0x3E8) {
                if (npc_dislikes[i] == id) return -1;
            } else if (npc_dislikes[i] == cls) liked = -1;
        }
    return liked;
}

/* Conversation built-in: the NPC gives the player everything in its slots. */
int far give_all_stuff(void)
{
    register int i;
    for (i = 0; i < 6; i++)
        if (barter_items[0][i]) {
            barter_selected[0][i] = 1;
            SomethingWithTradeSlot_ovr097_E83(0, i);
        }
    if (nothing_there(barter_items[0], barter_selected[0])) return 0;
    ReturnTradeObjectsToNPC_ovr097_F76(1);
    barter_result = 1;
    return 1;
}
