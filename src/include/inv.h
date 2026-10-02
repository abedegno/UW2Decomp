/* inv.h: The inventory: its slots and panel, open bags. */
#ifndef INV_H
#define INV_H

#include "uw2.h"

struct Bag;
struct InvRect;
struct Object;

#include "object.h"

/* One inventory slot on screen, 14 bytes: the rectangle the mouse hits (y counts up from
   the bottom of the screen, so top >= bottom) and where its picture goes. */
struct InvRect {
    int left, top, right, bottom;       /* 0x00 */
    int x, y;                           /* 0x08 */
    unsigned char w, h;                 /* 0x0C */
};

/* An open bag in the inventory panel, 12 bytes: bags opened inside bags form a chain
   (OpenBag is the innermost). */
struct Bag {
    struct Bag far *next;               /* 0x00, the bag opened inside this one */
    struct Bag far *prev;               /* 0x04, the bag this one was opened from */
    union Link obj;                     /* 0x08, the bag object */
    int weight;                         /* 0x0A */
};

/* OVR121.C: open bags in the inventory panel */
extern unsigned char display_inventory_no_show;
void far CloseTheBag(void);
void far FixOpenBag(void);
void far DisplayOpenBag(void);
void far ScrollItemsUp(void);
void far ScrollItemsDown(void);
void far BagWeight(unsigned far *head, int far *total);
void far DoSpecialActions(int slot);
void far FixBagArea(void);
void far OpenTheBag(int slot);
char far PutObjectInBag(struct Object far *obj, int slot);
char far SwapItemsInBag(struct Object far *obj, int slot);

/* OVR124.C: the player's inventory as data */
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
struct Object far * far FindObj(int major, int minor, int cls, int how, int *where);
struct Object far * far pick_inv(int how);
char far InvRemoveObject(struct Object far *obj);
char far InvRemoveOneObject(struct Object far *obj);
struct Object far * far RemoveOneFromSlot(int major, int minor, int cls, int slot);
unsigned char far ObjWorn(int id, int slot);
int far DamageInventory(int slot, unsigned char damage, unsigned char type, int how, char debris);
unsigned char far EncumCheck(struct Object far *obj);

/* OVR125.C: the inventory panel */
extern int SaveHandles[23];
extern struct Object far *CursorObjPtr;
extern struct Bag far *OpenBagList;
extern struct Bag far *OpenBag;
extern struct InvRect InvDisplay[23];
extern char SlotToDisplay[28];
extern char DisplayToSlot[21];
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
unsigned char far AddToEmptySlot(struct Object far *obj, int slot);
char far AddTogether(struct Object far *obj, struct Object far *onto);
char far AddToOccupiedSlot(struct Object far *obj, int slot);
void far displayInventoryArray(int from, int to);
char far displayEnc(char show);
int far FindInventoryHit(int x, int y);
void far DisplayInvSpecial(void);
void far DoInventoryMouse(int how);
void far DoInventoryDrag(struct Object far *obj);
void far DisplayInventory(void);

#endif
