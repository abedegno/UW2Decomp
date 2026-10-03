/* target: ovr121 */
/* opts: -mm -1 -G -O -Y -d */
/* Open bags in the inventory panel: opening and closing containers, scrolling their
   contents, putting objects into them and swapping objects within them, and the weight of
   a container's contents: the whole of DOS overlay ovr121, in original order.

   What it does in the game: opening a container in the inventory replaces the backpack
   rows of the panel with the container's contents. Open bags form a chain (struct Bag,
   inv.h: OpenBagList the outermost, OpenBag the innermost), each remembering its contents'
   weight so capacity checks are cheap. Inventory[19] is the open container and
   Inventory[20..27] the eight of its objects on show (invisible objects are skipped); the
   arrows scroll by rows of four. The backpack's slot pictures are swapped out to
   BagSaveHandles while a bag is open. Opening a container also switches its item to the open
   form (even minor class to odd, below class 12) and closing it switches it back.
   DoSpecialActions is the panel's dispatcher for everything that is not picking up or
   putting down: scroll arrows, closing the bag, dropping into the 3D view, the barter area,
   toggling fight mode from the weapon hand, and using an object in a slot.

   Data owned: BagSaveHandles and display_inventory_no_show (set while the panel must not
   redraw).
   Function and global names are the originals from the FM Towns symbol table.
   Name: descriptive (open bags in the inventory panel). */

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

/* This file's _BSS, DS:6A76..6A85: the backpack slots' screen saves while a bag is open. */
/* match: ovr122's _BSS starts at 6A86; only this file uses it. */
int16 BagSaveHandles[8];
/* This file's _DATA, DS:15D0. */
/* match: ovr119's strings end there and ovr122's data starts at 15D2; of ovr120 and
   ovr121, the two files between, only this one uses it. */
unsigned char display_inventory_no_show = 0;


/* A click on display position slot that is not a pick-up or put-down: 0x15 and 0x16
   scroll the open bag, 0x14 (the open bag's own picture) closes it, 0x17 drops or throws the
   cursor object into the 3D view (ReturnObject, MISSILE.C; a moonstone dropped, or a
   container holding one, records this level in a free player->moonstones entry for Gate
   Travel), 0x18 is the barter area (conv_inv_special), the weapon hand with a weapon or bow
   in it toggles fight mode, and anything else uses the object in the slot (UseObj). */
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
            if (HasOrIsObj(CursorObjPtr, ITEM_MOONSTONE)) {
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
            obj = Obj_PtrTMem(&Inventory[DisplayToSlot[slot]]);
            id = OBJ_ITEM(FARNULLTRAP(obj));     /* an empty hand reads 19h, a bow */
            cls = OBJ_CLASS(FARNULLTRAP(obj));
            if (cls == CLASS_WEAPON || id == ITEM_SLING || id == ITEM_BOW || id == ITEM_CROSSBOW || id == ITEM_JEWELED_BOW) {
                toggle_fightmode();
                break;
            }
        }
    default:
        if ((obj = Obj_PtrTMem(&Inventory[DisplayToSlot[slot]])) != 0)
            UseObj(ThePlayer, obj, 1);
    }
    if (held && CursorObjPtr == 0) {
        unforce_mouse_cursor(3);
        GameInputMode = 0;
    }
}

/* Turns an open container back into its closed item (odd minor class below 12 to the even
   one before it). */
void far MakeBagClose(struct Bag far *bag)
{
    struct Object far *obj;
    int cls;

    obj = Obj_IntTMem(bag->obj.f.index);
    cls = obj->id & ID_INCLASS;
    if (cls < 12 && (cls & 1))
        SET_INCLASS(obj, cls - 1);
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

/* Closes every bag and gives the panel its backpack rows back. */
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

/* Closes the innermost open bag, showing the one it was opened from, or the backpack if it
   was the outermost. */
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
            Inventory[20].f.index = Obj_PtrTMem(&OpenBag->obj)->ol.f.link;
            FixOpenBag();
            DisplayOpenBag();
            displayInventoryArray(0x14, 0x14);
        } else
            FixBagArea();
    }
}

/* Draws the open bag's eight slots and its scroll arrows, working out which arrows apply. */
void far DisplayOpenBag(void)
{
    struct Object far *obj;

    mouse_hide();
    displayInventoryArray(0xC, 0x13);
    obj = Obj_PtrTMem(&Obj_PtrTMem(&Inventory[19])->ol.link);
    while (obj != 0 && OBJ_INVIS(obj))
        obj = Obj_PtrTMem(&obj->qn.link);
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

/* Refills Inventory[20..27] from the open bag's contents list, keeping the first object
   shown if it is still there (else starting from the top), skipping invisible objects. */
void far FixOpenBag(void)
{
    struct Object far *obj;
    int i;

    for (i = 20; i <= 27; i++)
        if (Inventory[i].f.index != 0)
            break;
    obj = Obj_PtrTMem(&Obj_PtrTMem(&Inventory[19])->ol.link);
    if (i > 27) {
        for (i = 20; i <= 27; i++) {
            Inventory[i].f.index = Obj_MemTPtr(obj);
            if (obj != 0) {
                if (OBJ_INVIS(obj))
                    i--;
                obj = Obj_PtrTMem(&obj->qn.link);
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
                    obj = Obj_PtrTMem(&obj->qn.link);
                }
            }
        }
    } else {
        while (Obj_PtrTMem(&Inventory[i]) != obj) {
            obj = Obj_PtrTMem(&obj->qn.link);
            if (obj == 0)
                return;
        }
        for (i = 20; i <= 27; i++) {
            Inventory[i].f.index = Obj_MemTPtr(obj);
            if (obj != 0) {
                if (OBJ_INVIS(obj))
                    i--;
                obj = Obj_PtrTMem(&obj->qn.link);
            }
        }
    }
}

/* Opens the container in slot in the panel. A container of minor class 0xF (the rune bag)
   shows the rune panel instead. Opening a bag already open goes back to the backpack; a bag
   in the paperdoll or hands (slot < 11) closes the others first, one in the backpack or an
   open bag opens inside the current one. */
void far OpenTheBag(int slot)
{
    struct Bag far *bag;
    struct Object far *first;
    struct Object far *obj;
    int i;
    struct Object far *cont;
    int cls;
    int j;

    obj = Obj_PtrTMem(&Inventory[slot]);
    if (OBJ_MAJOR(obj) != MAJOR_MISC || OBJ_MINOR(obj) != MINOR_CONTAINER)
        return;
    if (OBJ_INCLASS(obj) == 0xF) {
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
    first = Obj_PtrTMem(&Obj_PtrTMem(&Inventory[19])->ol.link);
    BagWeight(&Obj_PtrTMem(&Inventory[19])->ol.link, &OpenBag->weight);
    for (i = 20; i <= 27; i++) {
        Inventory[i].f.index = Obj_MemTPtr(first);
        if (first != 0) {
            if (OBJ_INVIS(first))
                i--;
            first = Obj_PtrTMem(&first->qn.link);
        }
    }
    cont = Obj_PtrTMem(&Inventory[19]);
    cls = cont->id & ID_INCLASS;
    if (cls < 12 && !(cls & 1))
        SET_INCLASS(cont, cls + 1);
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

/* Shows the previous row of four of the open bag (the arrow names are the other way
   round from the direction the list moves, inferred). */
void far ScrollItemsDown(void)
{
    struct Object far *obj;
    struct Object far *target;
    struct Object far *first;
    int i;

    if (OpenBagList == 0 || !InvDownArrow)
        return;
    first = obj = Obj_PtrTMem(&Obj_PtrTMem(&OpenBag->obj)->ol.link);
    target = Obj_PtrTMem(&Inventory[20]);
    while (obj != target) {
        first = obj;
        for (i = 0; i < 4; i++) {
            obj = Obj_PtrTMem(&obj->qn.link);
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

/* Puts obj into the container at slot (19: the container the open bag sits in, or the
   backpack when it is the outermost). Runestones go into the rune bag as runes (add_rune).
   The object merges with a matching stack inside or is added at the end; weights are kept
   right; a lit light put into a bag goes out. Returns 1 when it went in. */
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
        cont = Obj_PtrTMem(&bag->obj);
    } else {
        cont = Obj_PtrTMem(&Inventory[slot]);
        if (slot > 19)
            bag = OpenBag;
        else
            bag = 0;
    }
    if (OBJ_ITEM(cont) == ITEM_RUNE_BAG) {
        if (add_rune(obj))
            return 1;
        game_sprint(0x106);  /* 'You can only put runes in the rune bag.' */
        return 0;
    }
    weight = ItemWeight(obj);
    PlayerDat.rec.weight += weight;
    while (bag != 0) {
        bag->weight += weight;
        bag = bag->prev;
    }
    if ((next = Obj_PtrTMem(&cont->ol.link)) != 0) {
        do {
            if (AddTogether(obj, next)) {
                if (!OBJ_ISQUANT(next)) {
                    SET_ISQUANT(next, 1);
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
        } while ((next = Obj_PtrTMem(&next->qn.link)) != 0);
    }
    if (next == 0) {
        Obj_AddEnd(&cont->ol.link, obj);
        if (found)
            Inventory[i].f.index = Obj_MemTPtr(obj);
    }
    if (Obj_MemTPtr(cont) == FARNULLREC(OpenBag, "ffww")->obj.f.index) {
        FixOpenBag();
        displayInventoryArray(0xC, 0x13);
    } else if (displayEnc(1))
        grfx_quikfont(FONT_5X6P);
    if (OBJ_ITEM(obj) >= FIRST_LIT_LIGHT && OBJ_ITEM(obj) < FIRST_WAND)
        SET_INCLASS(obj, OBJ_INCLASS(obj) - 4);
    return 1;
}

/* Puts obj in the open bag's slot in place of the object there, which goes onto the
   cursor; if obj does not fit, the old object goes back and obj stays on the cursor. */
char far SwapItemsInBag(struct Object far *obj, int slot)
{
    struct Object far *target;
    struct Object far *cur;
    union Link far *head;
    struct Bag far *bag;
    char result;
    int diff;

    result = 1;
    head = &Obj_PtrTMem(&OpenBag->obj)->ol.link;
    target = Obj_PtrTMem(&Inventory[slot]);
    while ((cur = Obj_PtrTMem(head)) != target) {
        if (cur == 0)
            return 0;
        head = &cur->qn.link;
    }
    SetCursorObj(slot, 0);
    if (!ItemFitsSlot(obj, slot)) {
        CursorObjPtr = obj;
        unforce_mouse_cursor(3);
        force_mouse_cursor(OBJ_ITEM(CursorObjPtr));
        result = 0;
        obj = target;
    }
    Obj_Add(head, obj);
    Inventory[slot].f.index = Obj_MemTPtr(obj);
    diff = ItemWeight(obj) - ItemWeight(target);
    for (bag = OpenBag; bag != 0; bag = bag->prev)
        bag->weight += diff;
    PlayerDat.rec.weight += ItemWeight(obj);
    FixPlayerEquips();
    FixOpenBag();
    displayInventoryArray(SlotToDisplay[slot], SlotToDisplay[slot]);
    return result;
}

/* Adds the weight of every object in the list head, and of their contents, to *total. */
void far BagWeight(union Link far *head, int16 far *total)
{
    struct Object far *obj;
    int qty;

    obj = Obj_PtrTMem(head);
    if (obj != 0) {
        if (OBJ_ISQUANT(obj) && !(obj->ol.f.link & LINK_SPECIAL))
            qty = obj->ol.f.link;
        else
            qty = 1;
        *total = *total + ComObjData[OBJ_ITEM(obj)].mass * qty;
        BagWeight(&obj->qn.link, total);
        if (!OBJ_ISQUANT(obj))
            BagWeight(&obj->ol.link, total);
    }
}
