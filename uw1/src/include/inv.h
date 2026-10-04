/* inv.h: The inventory: its slots and panel, open bags. The header of src/inv: the
   panel's rectangles and the open-bag chain, the slot and display numbering (named below,
   from the code of the three files), and the prototypes of BAGS.C, INVDATA.C and
   INVPANEL.C (INVSAVE.C's are in file.h, with the saving code). Names are
   UW2's FM Towns ones through UW2Decomp; the slot names are descriptive. */
#ifndef INV_H
#define INV_H

#include "uw2.h"

struct Bag;
struct InvRect;
struct Object;
union Link;

#include "object.h"

/* One inventory slot on screen, 14 bytes: the rectangle the mouse hits (y counts up from
   the bottom of the screen, so top >= bottom) and where its picture goes. */
struct InvRect {
    int16 left, top, right, bottom;     /* 0x00 */
    int16 x, y;                         /* 0x08 */
    unsigned char w, h;                 /* 0x0C */
};

/* An open bag in the inventory panel, 12 bytes: bags opened inside bags form a chain
   (OpenBag is the innermost). */
HOST_LAYOUT_BEGIN
struct Bag {
    struct Bag far *next;               /* 0x00, the bag opened inside this one */
    struct Bag far *prev;               /* 0x04, the bag this one was opened from */
    union Link obj;                     /* 0x08, the bag object */
    int16 weight;                       /* 0x0A */
};
HOST_LAYOUT_END

/* BAGS.C: open bags in the inventory panel */
void far CloseTheBag(void);
void far FixOpenBag(void);
void far DisplayOpenBag(void);
void far ScrollItemsUp(void);
void far ScrollItemsDown(void);
void far BagWeight(union Link far *head, int16 far *total);
void far DoSpecialActions(int slot);
void far FixBagArea(void);
void far OpenTheBag(int slot);
char far PutObjectInBag(struct Object far *obj, int slot);
char far SwapItemsInBag(struct Object far *obj, int slot);

/* INVDATA.C: the player's inventory as data */
int far ItemWeight(struct Object far *obj);
struct Object far * far find_obj(int major, int minor, int cls, struct Object far **list);
struct Object far * far WhatsInSlot(int slot);
struct Object far * far RemoveAllFromSlot(int major, int minor, int cls, int slot);
struct Object far * far removeFromSlot(int major, int minor, int cls, int slot, int qty);
struct Object far * far takeFromSlot(int major, int minor, int cls, int slot, int qty);
void far RedisplayInvSlot(int slot);
struct Object far * far AskInventory(int slot);
unsigned char far AddToInventory(struct Object far *obj, int slot);
int far FindSlot(struct Object far *obj);
struct Object far * far FindObj(int major, int minor, int cls, int how, int16 *where);
struct Object far * far pick_inv(int how);
char far InvRemoveObject(struct Object far *obj);
char far InvRemoveOneObject(struct Object far *obj);
struct Object far * far RemoveOneFromSlot(int major, int minor, int cls, int slot);
unsigned char far ObjWorn(int id, int slot);
int far DamageInventory(int slot, unsigned char damage, unsigned char type, int how, char debris);

/* INVPANEL.C: the inventory panel */
/* The inventory's slots, Inventory[28] (each a union Link naming the object shown there;
   the objects themselves are the player's contents list). From the code of INVDATA.C,
   INVPANEL.C and BAGS.C, with the wearable types of ItemFitsSlot (Guide, "Armour and
   Wearables Table"):
     0 head, 1 torso, 2 gloves, 3 legs, 4 boots (the armour slots, ObjWorn);
     5, 6 the shoulders, 7, 8 the hands: the weapon hand is 8 - lefty, the shield hand
       7 + lefty; lights burn only in 5..8 (ValidLightSlots);
     9, 10 the rings;
     11..18 the backpack;
     19 the open bag (the innermost container open in the panel), 20..27 the eight of its
       objects on show.
   Display positions (InvDisplay, FindInventoryHit) number the panel's rectangles instead:
   0 the body, 1..5 the armour (SlotToDisplay maps slots 3, 0, 1, 2, 4 to 1..5), 6..19
   slots 5..18 (an open bag's 20..27 take 12..19), 20 the open bag, 21 and 22 its scroll
   arrows; FindInventoryHit adds 0x17 for the 3D view and 0x18 for the barter area. */
/* Inventory[] slots, as the comment above lists them (the names are descriptive). */
#define INV_HEAD        0               /* the armour slots, 0..4 (ObjWorn) */
#define INV_TORSO       1
#define INV_GLOVES      2
#define INV_LEGS        3
#define INV_BOOTS       4
#define INV_SHOULDER    5               /* the shoulders, 5 and 6 */
#define INV_HAND        7               /* the hands, 7 and 8 */
#define INV_WEAPON_HAND 8               /* less lefty; the shield hand is INV_HAND + lefty */
#define INV_RING        9               /* the rings, 9 and 10 */
#define INV_PACK        11              /* the backpack, 11..18 */
#define INV_PACK_LAST   18
#define INV_BAG         19              /* the open bag itself */
#define INV_BAG_ITEMS   20              /* its objects on show, 20..27 */
#define INV_BAG_LAST    27
#define NUM_INV_SLOTS   28
/* Display positions: InvDisplay[] and FindInventoryHit's results. */
#define DISP_BODY       0               /* the paperdoll; 1..5 the armour (SlotToDisplay) */
#define DISP_PACK       12              /* 12..19, the backpack's last eight or an open bag's
                                           objects */
#define DISP_PACK_LAST  19
#define DISP_BAG        20              /* 0x14, the open bag's picture */
#define DISP_UP         21              /* 0x15, InvUpArrow */
#define DISP_DOWN       22              /* 0x16, InvDownArrow */
#define NUM_DISPLAY     23
#define DISP_WORLD      0x17            /* the 3D view */
#define DISP_BARTER     0x18            /* the player's barter area, in barter mode */
extern int16 SaveHandles[NUM_DISPLAY];
extern struct Object far *CursorObjPtr;
extern struct Bag far *OpenBagList;
extern struct Bag far *OpenBag;
extern struct InvRect InvDisplay[NUM_DISPLAY];
#ifdef __TURBOC__
extern char SlotToDisplay[NUM_INV_SLOTS];
extern char DisplayToSlot[21];
#else
/* BAGS.C's OpenTheBag writes an open bag's slots 20..27 to SlotToDisplay[40..47],
   which in DOS are DisplayToSlot[12..19], the next variable in DGROUP. The port keeps the
   two in one array, so the write lands there too whatever the host's alignment (x86-64 puts
   a separate 21-byte array on a 16-byte boundary, four bytes further on). */
#define DisplayToSlot (SlotToDisplay + 28)
#endif
extern unsigned char InvUpArrow;
extern unsigned char InvDownArrow;
extern unsigned char inv_refresh;
void far load_inventory_pix(void);
void far BeginInventory(void);
void far EndInventory(void);
void far ClearInventory(void);
void far DisplayInvObject(int slot);
void far SetCursorObj(int slot, char keep);
struct Object far * far AskHowMany(struct Object far *obj);
int far ItemFitsSlot(struct Object far *obj, int slot);
void far RearrangeInventory(int slot);
char far AddToEmptySlot(struct Object far *obj, int slot);
char far AddTogether(struct Object far *obj, struct Object far *onto);
char far AddToOccupiedSlot(struct Object far *obj, int slot);
void far displayInventoryArray(int from, int to);
char far displayEnc(char show);
int far FindInventoryHit(int x, int y);
void far DisplayInvSpecial(void);
void far DoInventoryMouse(int how);
void far DoInventoryDrag(struct Object far *obj);
void far DisplayInventory(void);
extern union Link Inventory[NUM_INV_SLOTS];
extern char ValidLightSlots[4];

#endif
