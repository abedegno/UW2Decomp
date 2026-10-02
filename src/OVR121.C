/* target: ovr121 */
/* opts: -mm -1 -G -O -Y -d */
/* Open bags in the inventory panel: opening and closing containers, scrolling their
   contents, putting objects into them and swapping objects within them, and the weight of
   a container's contents: the whole of DOS overlay ovr121, in original order. Function and
   global names are the originals from the FM Towns symbol table; the source file's own name
   is not known. */

#include <alloc.h>
#include "combat.h"
#include "conv.h"
#include "gfx.h"
#include "inv.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"
#include "uw2.h"

#define OBJ_ID(o)       ((o)->id & 0x1FF)
#define OBJ_MAJOR(o)    (((o)->id & 0x1C0) >> 6)
#define OBJ_MINOR(o)    (((o)->id & 0x30) >> 4)
#define OBJ_CLASS(o)    (((o)->id & 0x1F0) >> 4)
#define OBJ_INVIS(o)    (((o)->id & 0x4000) >> 14)
#define OBJ_ISQUANT(o)  (((o)->id & 0x8000) >> 15)

extern struct Player PlayerDat;
extern union Link Inventory[];
/* This file's _BSS, DS:6A76..6A85 (ovr122's starts at 6A86): only this file uses it. */
int BagSaveHandles[8];
/* This file's _DATA, DS:15D0 (ovr119's strings end there; ovr122's data starts at 15D2):
   of ovr120 and ovr121, the two files between, only this one uses it. */
unsigned char display_inventory_no_show = 0;
extern char RightPanel;
extern struct Inplist near *inplist;

struct Object far * far Obj_PtrTMem(unsigned far *link);
struct Object far * far Obj_IntTMem(int index);
void far Obj_Add(unsigned far *head, struct Object far *obj);
void far Obj_AddEnd(unsigned far *head, struct Object far *obj);
char far HasOrIsObj(struct Object far *obj, int id);
void far UseObj(struct Object far *who, struct Object far *obj, int how);
void far set_screen_frame(int frame, int how);

void far DoSpecialActions(int slot)
{
    int cls;
    struct Object far *obj;
    char held;
    int i;
    int id;

    held = CursorObjPtr != 0;
    switch (slot) {
    case 0x15:
        ScrollItemsUp();
        break;
    case 0x16:
        ScrollItemsDown();
        break;
    case 0x14:
        CloseTheBag();
        break;
    case 0x17:
        if (CursorObjPtr != 0 && ReturnObject(CursorObjPtr, 1)) {
            if (HasOrIsObj(CursorObjPtr, 0x126)) {
                for (i = 0; i < 2; i++) {
                    if (player->moonstones[i] == 0) {
                        player->moonstones[i] = PlayerLevel;
                        break;
                    }
                }
            }
            CursorObjPtr = 0;
            FixPlayerEquips();
        }
        break;
    case 0x18:
        conv_inv_special();
        break;
    case 8:
    case 9:
        if (9 - player->lefty == slot) {
            obj = Obj_PtrTMem(&Inventory[DisplayToSlot[slot]].word);
            id = OBJ_ID(obj);
            cls = OBJ_CLASS(obj);
            if (cls == 0 || id == 0x18 || id == 0x19 || id == 0x1A || id == 0x1F) {
                toggle_fightmode();
                break;
            }
        }
    default:
        if ((obj = Obj_PtrTMem(&Inventory[DisplayToSlot[slot]].word)) != 0)
            UseObj(ThePlayer, obj, 1);
    }
    if (held && CursorObjPtr == 0) {
        unforce_mouse_cursor(3);
        GameInputMode = 0;
    }
}

void far MakeBagClose(struct Bag far *bag)
{
    struct Object far *obj;
    int cls;

    obj = Obj_IntTMem(bag->obj.f.index);
    cls = obj->id & 0xF;
    if (cls < 12 && (cls & 1))
        obj->id = obj->id & 0xFFF0 | (cls - 1) & 0xF;
}

void far CloseAllBags(void)
{
    struct Bag far *prev;
    int i;

    if (OpenBag == 0)
        return;
    while ((prev = OpenBag->prev) != 0) {
        MakeBagClose(OpenBag);
        farfree(OpenBag);
        OpenBag = prev;
    }
    OpenBagList = 0;
    MakeBagClose(OpenBag);
    farfree(OpenBag);
    OpenBag = 0;
    for (i = 19; i <= 27; i++)
        Inventory[i].f.index = 0;
}

void far FixBagArea(void)
{
    int i;
    int t;

    if (OpenBag != 0) {
        CloseAllBags();
        Inventory[19].f.index = 0;
        for (i = 11; i <= 18; i++)
            DisplayToSlot[i + 1] = i;
        for (i = 12; i <= 19; i++) {
            t = BagSaveHandles[i - 12];
            BagSaveHandles[i - 12] = SaveHandles[i];
            SaveHandles[i] = t;
        }
        mouse_hide();
        if (!display_inventory_no_show && (scrmode == 1 || scrmode == 4) && RightPanel == 0) {
            restore_rect(SaveHandles[1]);
            DisplayInventory();
        }
        mouse_show();
        InvUpArrow = InvDownArrow = 0;
        if (!display_inventory_no_show) {
            DisplayInvObject(0x15);
            DisplayInvObject(0x16);
        }
    }
}

void far CloseTheBag(void)
{
    struct Bag far *bag;

    if (OpenBagList != 0) {
        if (OpenBag->prev != 0) {
            MakeBagClose(OpenBag);
            bag = OpenBag;
            OpenBag = OpenBag->prev;
            farfree(bag);
            OpenBag->next = 0;
            Inventory[19] = OpenBag->obj;
            Inventory[20].f.index = Obj_PtrTMem(&OpenBag->obj.word)->ol.f.link;
            FixOpenBag();
            DisplayOpenBag();
            displayInventoryArray(0x14, 0x14);
        } else
            FixBagArea();
    }
}

void far DisplayOpenBag(void)
{
    struct Object far *obj;

    mouse_hide();
    displayInventoryArray(0xC, 0x13);
    obj = Obj_PtrTMem(&Obj_PtrTMem(&Inventory[19].word)->ol.word);
    while (obj != 0 && OBJ_INVIS(obj))
        obj = Obj_PtrTMem(&obj->qn.word);
    if (Obj_MemTPtr(obj) == Inventory[20].f.index)
        InvDownArrow = 0;
    else
        InvDownArrow = 1;
    if (Inventory[27].f.index == 0)
        InvUpArrow = 0;
    else
        InvUpArrow = 1;
    DisplayInvObject(0x15);
    DisplayInvObject(0x16);
    mouse_show();
}

void far FixOpenBag(void)
{
    struct Object far *obj;
    int i;

    for (i = 20; i <= 27; i++)
        if (Inventory[i].f.index != 0)
            break;
    obj = Obj_PtrTMem(&Obj_PtrTMem(&Inventory[19].word)->ol.word);
    if (i > 27) {
        for (i = 20; i <= 27; i++) {
            Inventory[i].f.index = Obj_MemTPtr(obj);
            if (obj != 0) {
                if (OBJ_INVIS(obj))
                    i--;
                obj = Obj_PtrTMem(&obj->qn.word);
            }
        }
        while (obj != 0) {
            for (i = 20; i < 24; i++)
                Inventory[i].f.index = Inventory[i + 4].f.index;
            for (; i <= 27; i++) {
                Inventory[i].f.index = Obj_MemTPtr(obj);
                if (obj != 0) {
                    if (OBJ_INVIS(obj))
                        i--;
                    obj = Obj_PtrTMem(&obj->qn.word);
                }
            }
        }
    } else {
        while (Obj_PtrTMem(&Inventory[i].word) != obj) {
            obj = Obj_PtrTMem(&obj->qn.word);
            if (obj == 0)
                return;
        }
        for (i = 20; i <= 27; i++) {
            Inventory[i].f.index = Obj_MemTPtr(obj);
            if (obj != 0) {
                if (OBJ_INVIS(obj))
                    i--;
                obj = Obj_PtrTMem(&obj->qn.word);
            }
        }
    }
}

void far OpenTheBag(int slot)
{
    struct Bag far *bag;
    struct Object far *first;
    struct Object far *obj;
    int i;
    struct Object far *cont;
    int cls;
    int j;

    obj = Obj_PtrTMem(&Inventory[slot].word);
    if (OBJ_MAJOR(obj) != 2 || OBJ_MINOR(obj) != 0)
        return;
    if ((obj->id & 0xF) == 0xF) {
        if (inplist->mode == 1)
            set_screen_frame(6, 1);
        return;
    }
    if (OpenBagList != 0) {
        for (bag = OpenBagList; bag != 0; bag = bag->next) {
            if (bag->obj.f.index == Inventory[slot].f.index) {
                FixBagArea();
                return;
            }
        }
        if (slot < 11)
            CloseAllBags();
    } else {
        mouse_hide();
        if ((scrmode == 1 || scrmode == 4) && RightPanel == 0)
            pic_to_screen(0x2073, 0xEE, 0x7A, 0x2B, 0x4C);
        if (BagSaveHandles[0] == 0) {
            for (j = 12; j <= 19; j++) {
                BagSaveHandles[j - 12] = valloc(InvDisplay[j].w, InvDisplay[j].h);
                save_rect(BagSaveHandles[j - 12], InvDisplay[j].x, InvDisplay[j].y,
                          InvDisplay[j].w, InvDisplay[j].h);
            }
        }
        mouse_show();
        for (i = 20; i <= 27; i++)
            SlotToDisplay[i + 20] = i;
        for (i = 12; i <= 19; i++) {
            j = BagSaveHandles[i - 12];
            BagSaveHandles[i - 12] = SaveHandles[i];
            SaveHandles[i] = j;
        }
    }
    bag = farmalloc(sizeof(struct Bag));
    if (bag == 0)
        return;
    if (OpenBagList == 0) {
        OpenBag = OpenBagList = bag;
        bag->prev = 0;
    } else {
        OpenBag->next = bag;
        bag->prev = OpenBag;
        OpenBag = OpenBag->next;
    }
    OpenBag->next = 0;
    OpenBag->weight = 0;
    Inventory[19].f.index = OpenBag->obj.f.index = Inventory[slot].f.index;
    first = Obj_PtrTMem(&Obj_PtrTMem(&Inventory[19].word)->ol.word);
    BagWeight(&Obj_PtrTMem(&Inventory[19].word)->ol.word, &OpenBag->weight);
    for (i = 20; i <= 27; i++) {
        Inventory[i].f.index = Obj_MemTPtr(first);
        if (first != 0) {
            if (OBJ_INVIS(first))
                i--;
            first = Obj_PtrTMem(&first->qn.word);
        }
    }
    cont = Obj_PtrTMem(&Inventory[19].word);
    cls = cont->id & 0xF;
    if (cls < 12 && !(cls & 1))
        cont->id = cont->id & 0xFFF0 | (cls + 1) & 0xF;
    DisplayOpenBag();
    displayInventoryArray(0x14, 0x14);
    if (SlotToDisplay[slot] < 11)
        DisplayInvObject(SlotToDisplay[slot]);
}

void far ScrollItemsUp(void)
{
    if (OpenBagList == 0 || !InvUpArrow)
        return;
    Inventory[20] = Inventory[24];
    FixOpenBag();
    DisplayOpenBag();
}

void far ScrollItemsDown(void)
{
    struct Object far *obj;
    struct Object far *target;
    struct Object far *first;
    int i;

    if (OpenBagList == 0 || !InvDownArrow)
        return;
    first = obj = Obj_PtrTMem(&Obj_PtrTMem(&OpenBag->obj.word)->ol.word);
    target = Obj_PtrTMem(&Inventory[20].word);
    while (obj != target) {
        first = obj;
        for (i = 0; i < 4; i++) {
            obj = Obj_PtrTMem(&obj->qn.word);
            if (obj == 0)
                return;
            if (obj == target)
                break;
        }
    }
    Inventory[20].f.index = Obj_MemTPtr(first);
    FixOpenBag();
    DisplayOpenBag();
}

char far PutObjectInBag(struct Object far *obj, int slot)
{
    struct Object far *next;
    struct Object far *cont;
    struct Bag far *bag;
    int weight;
    char found;
    int i;
    int qty;

    next = 0;
    found = 0;
    if (!ItemFitsSlot(obj, slot))
        return 0;
    if (slot == 19 && OpenBag->prev == 0) {
        for (i = 11; i <= 18; i++) {
            if (Inventory[i].f.index == 0) {
                found = 1;
                break;
            }
        }
        if (i > 18)
            return 0;
        cont = ThePlayer;
        bag = 0;
    } else if (slot == 19 && OpenBag->prev != 0) {
        bag = OpenBag->prev;
        cont = Obj_PtrTMem(&bag->obj.word);
    } else {
        cont = Obj_PtrTMem(&Inventory[slot].word);
        if (slot > 19)
            bag = OpenBag;
        else
            bag = 0;
    }
    if (OBJ_ID(cont) == 0x8F) {
        if (add_rune(obj))
            return 1;
        game_sprint(0x106);
        return 0;
    }
    weight = ItemWeight(obj);
    PlayerDat.weight += weight;
    while (bag != 0) {
        bag->weight += weight;
        bag = bag->prev;
    }
    if ((next = Obj_PtrTMem(&cont->ol.word)) != 0) {
        do {
            if (AddTogether(obj, next)) {
                if (!OBJ_ISQUANT(next)) {
                    next->id = next->id & 0x7FFF | 0x8000;
                    next->ol.f.link = 1;
                }
                if (OBJ_ISQUANT(obj))
                    qty = obj->ol.f.link;
                else
                    qty = 1;
                next->ol.f.link = next->ol.f.link + qty;
                next->qn.f.quality = (next->qn.f.quality + obj->qn.f.quality) >> 1;
                Obj_Free(obj);
                break;
            }
        } while ((next = Obj_PtrTMem(&next->qn.word)) != 0);
    }
    if (next == 0) {
        Obj_AddEnd(&cont->ol.word, obj);
        if (found)
            Inventory[i].f.index = Obj_MemTPtr(obj);
    }
    if (Obj_MemTPtr(cont) == OpenBag->obj.f.index) {
        FixOpenBag();
        displayInventoryArray(0xC, 0x13);
    } else if (displayEnc(1))
        grfx_quikfont(1);
    if (OBJ_ID(obj) >= 0x94 && OBJ_ID(obj) < 0x98)
        obj->id = obj->id & 0xFFF0 | ((obj->id & 0xF) - 4) & 0xF;
    return 1;
}

char far SwapItemsInBag(struct Object far *obj, int slot)
{
    struct Object far *target;
    struct Object far *cur;
    unsigned far *head;
    struct Bag far *bag;
    char result;
    int diff;

    result = 1;
    head = &Obj_PtrTMem(&OpenBag->obj.word)->ol.word;
    target = Obj_PtrTMem(&Inventory[slot].word);
    while ((cur = Obj_PtrTMem(head)) != target) {
        if (cur == 0)
            return 0;
        head = &cur->qn.word;
    }
    SetCursorObj(slot, 0);
    if (!ItemFitsSlot(obj, slot)) {
        CursorObjPtr = obj;
        unforce_mouse_cursor(3);
        force_mouse_cursor(OBJ_ID(CursorObjPtr));
        result = 0;
        obj = target;
    }
    Obj_Add(head, obj);
    Inventory[slot].f.index = Obj_MemTPtr(obj);
    diff = ItemWeight(obj) - ItemWeight(target);
    for (bag = OpenBag; bag != 0; bag = bag->prev)
        bag->weight += diff;
    PlayerDat.weight += ItemWeight(obj);
    FixPlayerEquips();
    FixOpenBag();
    displayInventoryArray(SlotToDisplay[slot], SlotToDisplay[slot]);
    return result;
}

void far BagWeight(unsigned far *head, int far *total)
{
    struct Object far *obj;
    int qty;

    obj = Obj_PtrTMem(head);
    if (obj != 0) {
        if (OBJ_ISQUANT(obj) && !(obj->ol.f.link & 0x200))
            qty = obj->ol.f.link;
        else
            qty = 1;
        *total = *total + ComObjData[OBJ_ID(obj)].mass * qty;
        BagWeight(&obj->qn.word, total);
        if (!OBJ_ISQUANT(obj))
            BagWeight(&obj->ol.word, total);
    }
}
