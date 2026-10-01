/* target: ovr125 */
/* opts: -mm -1 -G -O -Y -d */
/* The inventory panel: the paperdoll and backpack slots, their screen rectangles, picking
   objects up and putting them down in the slots, stacking and combining them, the weight
   display and the hit test: the whole of DOS overlay ovr125, in original order. Function
   and global names are the originals from the FM Towns symbol table; the source file's own
   name is not known. */

#include <stdlib.h>

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0x62];
    unsigned b62:6;                     /* 0x62 */
    unsigned sleepbits:3;               /* word 0x62, bits 6..8 */
    unsigned in_void:1;
    unsigned b63:6;
    char pad64;
    unsigned lefty:1;                   /* 0x65 */
    unsigned female:1;
    unsigned body:3;
};

/* The player's statistics block. */
struct PlayerStats {
    char pad0[0x4A];
    unsigned weight;                    /* 0x4A, weight carried */
    unsigned capacity;                  /* 0x4C, weight that can be carried */
};

/* A link word: the low six bits belong to the owner, the rest is an object index. */
union Link {
    unsigned word;
    struct { unsigned low:6, link:10; } f;
};

/* A mobile object. The first 8 bytes are shared with static objects. */
struct Object {
    unsigned id;
    unsigned pos;
    union {
        unsigned word;                  /* the next object in this list */
        struct { unsigned quality:6, next:10; } f;
    } qn;
    union {
        unsigned word;                  /* the head of the contents list, or the quantity */
        struct { unsigned owner:6, link:10; } f;
    } ol;
};

#define OBJ_ID(o)       ((o)->id & 0x1FF)
#define OBJ_MAJOR(o)    (((o)->id & 0x1C0) >> 6)
#define OBJ_MINOR(o)    (((o)->id & 0x30) >> 4)
#define OBJ_CLASS(o)    (((o)->id & 0x1F0) >> 4)
#define OBJ_TYPE(o)     (((o)->id & 0x3F) >> 0)
#define OBJ_INVIS(o)    (((o)->id & 0x4000) >> 14)
#define OBJ_ISQUANT(o)  (((o)->id & 0x8000) >> 15)

/* One object type's common properties, 11 bytes. */
struct ComObj {
    unsigned char height;               /* 0x00 */
    unsigned radius:4;                  /* 0x01 */
    unsigned mass:12;
    unsigned b3:5;                      /* 0x03 */
    unsigned pickup:1;
    unsigned stack:2;
    unsigned b4:8;
    char pad5[0x0B - 0x05];
};

/* One container type, 3 bytes. */
struct Container {
    unsigned char capacity;             /* 0x00, 0 for no limit */
    int mask;                           /* 0x01, what it accepts: an item id, 0x200.. a kind, or -1 */
};

/* One open bag (see ovr121). */
struct Bag {
    struct Bag far *next;
    struct Bag far *prev;
    union Link obj;
    int weight;
};

/* One inventory slot on screen, 14 bytes: the rectangle the mouse hits (y counts up from
   the bottom of the screen, so top >= bottom) and where its picture goes. */
struct InvRect {
    int left, top, right, bottom;       /* 0x00 */
    int x, y;                           /* 0x08 */
    unsigned char w, h;                 /* 0x0C */
};

struct Inplist {
    int x, y;                           /* 0x00 */
    char pad4[6 - 4];
    int buttons;                        /* 0x06 */
    int field8;                         /* 0x08 */
};

extern struct Player near *player;
extern struct PlayerStats PlayerDat;
extern struct Object far *ThePlayer;
extern struct Object far *CursorObjPtr;
extern union Link Inventory[];
extern struct ComObj ComObjData[];
extern struct Container Containers[];
extern struct Object far *ActiveObj;
extern char invArmorObj[];
extern char invArmorQ[];
extern int SaveHandles[];
extern int panel_mouse_region;          /* DS:6B0A, provisional */
extern char far Transparency;
extern char RightPanel;
extern char far *foreground_color;
extern struct Inplist near *inplist;
extern int GameInputMode;
extern int PLeft, PBot, PWid, PHgt;

void far reload_gr_vpic(int id, char *name, int n);
int far valloc(int w, int h);
void far save_rect(int handle, int x, int y, int w, int h);
void far restore_rect(int handle);
/* Every call here passes an InvRect's h before its w. */
void far pic_to_screen(int pic, int x, int y, int h, int w);
int far defineMouseRegion(int x0, int y0, int x1, int y1, int id);
int far input_addmouse(int a, int b, int c, int d, int buttons, int mode, void far (*handler)(void));
void far mous_in_panel(void);
void far mouse_hide(void);
void far grSoftPageFlip(void);
void far set_the_color(int c);
void far rectangle(int x0, int y0, int x1, int y1);
void far mouse_show(void);
void far set_font_size(int size);
void far string_to_screen(char far *s, int x, int y);
int far string_width(char far *s);
struct Object far * far Obj_PtrTMem(unsigned far *link);
int far Obj_MemTPtr(struct Object far *obj);
struct Object far * far WhatsInSlot(int slot);
struct Object far * far takeFromSlot(int a, int b, int c, int slot, int d);
void far FixPlayerEquips(void);
struct Object far * far Obj_Alloc(char mobile);
int far wdialog(char *prompt, char *initial, char *result, char anychar, int maxlen);
void far wd_replace(int n);
void far scroll_print(char far *s);
char * far get_class_data(void);
int far UseFood(struct Object far *who, struct Object far *food, char how);
void far BagWeight(unsigned far *head, int far *total);
void far get_name(char far *buf, struct Object far *obj, int article, char plural);
unsigned char far AddToInventory(struct Object far *obj, int slot);
void far Obj_Free(struct Object far *obj);
void far Obj_Punt(unsigned far *head, struct Object far *obj, int how);
char far PutObjectInBag(struct Object far *obj, int slot);
char far SwapItemsInBag(struct Object far *obj, int slot);
char far InvRemoveObject(struct Object far *obj);
struct Object far * far removeFromSlot(int major, int minor, int cls, int slot, int qty);
int far ObjsBeCombinable(struct Object far *obj, struct Object far *with);
struct Object far * far CombineObjs(int combo);
char far RemoveAfterCombine(struct Object far *obj, int combo);
void far Obj_Add(unsigned far *head, struct Object far *obj);
int far ItemWeight(struct Object far *obj);
void far FixOpenBag(void);
void far DisplayOpenBag(void);
void far DoSpecialActions(int slot);
void far toggle_fightmode(void);
void far inv_look(void);
void far game_sprint(int id);
void far mouse_release(int how);
char far mouse_dragged(int how);
void far mouse_getxy(int *x, int *y);
void far mouse_getbut(int *buttons);
void far unforce_mouse_cursor(int n);
void far force_mouse_cursor(int id);

char ValidLightSlots[4] = { 5, 6, 7, 8 };
struct Bag far *OpenBagList = 0;
struct Bag far *OpenBag = 0;
struct InvRect InvDisplay[23] = {
    { 0x0, 0x0, 0x0, 0x0, 0x102, 0xBF, 0x24, 0x45 },
    { 0x10B, 0x93, 0x11B, 0x86, 0x10A, 0xB2, 0x13, 0x32 },
    { 0x10B, 0xC2, 0x11C, 0xB2, 0x109, 0xC0, 0x14, 0x14 },
    { 0x105, 0xB1, 0x121, 0xA0, 0x104, 0xB3, 0x21, 0x2C },
    { 0x105, 0x9F, 0x121, 0x93, 0x103, 0xA0, 0x21, 0xC },
    { 0x105, 0x85, 0x121, 0x7A, 0x108, 0x88, 0x15, 0xD },
    { 0xF2, 0xBE, 0x103, 0xAD, 0xF3, 0xBB, 0x10, 0x10 },
    { 0x123, 0xBE, 0x134, 0xAD, 0x124, 0xBB, 0x10, 0x10 },
    { 0xF0, 0xA8, 0x101, 0x95, 0xF1, 0xA5, 0x10, 0x10 },
    { 0x126, 0xA8, 0x137, 0x95, 0x127, 0xA5, 0x10, 0x10 },
    { 0xEF, 0x96, 0x10A, 0x8B, 0xFD, 0x95, 0x10, 0x10 },
    { 0x11C, 0x96, 0x136, 0x85, 0x11B, 0x95, 0x10, 0x10 },
    { 0xEE, 0x79, 0xFF, 0x68, 0xEF, 0x76, 0x10, 0x10 },
    { 0x101, 0x79, 0x112, 0x68, 0x102, 0x76, 0x10, 0x10 },
    { 0x114, 0x79, 0x125, 0x68, 0x115, 0x76, 0x10, 0x10 },
    { 0x127, 0x79, 0x138, 0x68, 0x128, 0x76, 0x10, 0x10 },
    { 0xEE, 0x67, 0xFF, 0x56, 0xEF, 0x64, 0x10, 0x10 },
    { 0x101, 0x67, 0x112, 0x56, 0x102, 0x64, 0x10, 0x10 },
    { 0x114, 0x67, 0x125, 0x56, 0x115, 0x64, 0x10, 0x10 },
    { 0x127, 0x67, 0x138, 0x56, 0x128, 0x64, 0x10, 0x10 },
    { 0xEE, 0x8C, 0xFF, 0x7B, 0xEF, 0x8A, 0x10, 0x10 },
    { 0x125, 0x85, 0x12E, 0x7C, 0x126, 0x85, 0x8, 0xA },
    { 0x12F, 0x85, 0x138, 0x7C, 0x130, 0x85, 0x8, 0xA }
};
char SlotToDisplay[28] = {
    2, 3, 4, 1, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    12, 13, 14, 15, 16, 17, 18, 19
};
char DisplayToSlot[21] = {
    1, 3, 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
};
/* FM Towns keeps the next four as unnamed statics; the names are mine. */
static int panel_input = 0;             /* DS:1799, the panel's mouse handler */
static int shown_capacity = -1;         /* DS:179B, the weight figure on screen */
unsigned char InvUpArrow = 0;
unsigned char InvDownArrow = 0;
static unsigned char inv_begun = 0;     /* DS:179F */
unsigned char inv_refresh = 1;
static unsigned char ArmorSlots[5] = { 1, 3, 5, 4, 2 };    /* DS:17A1, the paperdoll slots */

void far load_inventory_pix(void)
{
    register int i;
    register char *name = "bodies";

    reload_gr_vpic(0x206D, name, player->female * 10 / 2 + player->body);
    for (i = 1; i <= 5; i++)
        invArmorObj[i] = 0;
}

void far BeginInventory(void)
{
    int x;
    register int i;
    register int w;

    if (!inv_begun) {
        inv_begun = 1;
        Transparency = 1;
        load_inventory_pix();
        for (i = 6; i < 23; i++)
            SaveHandles[i] = valloc(InvDisplay[i].w, InvDisplay[i].h);
        SaveHandles[0] = valloc(0x10, 0x0A);
        SaveHandles[1] = valloc(0x4C, 0x2B);
        for (i = 6; i < 23; i++) {
            if (i == 10) {
                x = InvDisplay[i].x + 5;
                w = InvDisplay[i].w - 5;
            } else if (i == 11) {
                x = InvDisplay[i].x;
                w = InvDisplay[i].w - 5;
            } else {
                x = InvDisplay[i].x;
                w = InvDisplay[i].w;
            }
            save_rect(SaveHandles[i], x, InvDisplay[i].y, w, InvDisplay[i].h);
        }
        save_rect(SaveHandles[1], 0xEE, 0x7A, 0x4C, 0x2B);
        save_rect(SaveHandles[0], 0x12B, 0x90, 0x10, 0x0A);
        panel_mouse_region = defineMouseRegion(0xF0, 0x51, 0x13B, 0xBE, 0x106C);
        panel_input = input_addmouse(0xF0, 0x51, 0x13B, 0xBE, 0, 5, mous_in_panel);
    }
}

/* IDA DoesNothing_ovr125_1E6; FM Towns has EndInventory_, also empty, in this place. */
void far EndInventory(void)
{
}

void far ClearInventory(void)
{
    register int i;

    for (i = 0; i < 28; i++)
        Inventory[i].f.link = 0;
    for (i = 1; i <= 5; i++)
        invArmorObj[i] = 0;
    OpenBag = OpenBagList = 0;
    InvUpArrow = InvDownArrow = 0;
    CursorObjPtr = 0;
    shown_capacity = -1;
}

/* Later in this file. */
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

void far DoInventoryMouse(int how)
{
    int x0;
    int y0;
    int x;
    int y;
    char pick;
    int newhit;
    struct Object far *obj;
    struct Object far *split;
    struct Bag far *bag2;
    char held;
    struct Bag far *bag;
    register int hit;
    register int slot;

    pick = 0;
    split = 0;
    held = CursorObjPtr != 0;
    x0 = inplist->x + 0xF0;
    y0 = inplist->y + 0x51;
    hit = FindInventoryHit(x0, y0);
    if (hit > 0 && hit < 21) {
        slot = DisplayToSlot[hit];
        if (CursorObjPtr == 0 && Inventory[slot].f.link == 0) {
            if (8 - player->lefty == slot)
                toggle_fightmode();
            mouse_release(1);
            return;
        }
        if (CursorObjPtr == 0 && slot != -1 && slot != 19)
            pick = 1;
        if (inplist->buttons != 1 && pick && mouse_dragged(1)) {
            obj = Obj_PtrTMem(&Inventory[slot].word);
            if (OBJ_ISQUANT(obj) && !(obj->ol.f.link & 0x200)) {
                if (obj->ol.f.link != 1) {
                    if ((split = AskHowMany(obj)) == 0)
                        return;
                    if (split != obj)
                        Obj_Add(&obj->qn.word, split);
                }
            } else if (OBJ_MAJOR(obj) == 2 && OBJ_MINOR(obj) == 0) {
                if (inplist->field8 == 4 && (obj->id & 0xF) != 0xF) {
                    game_sprint(0xC9);
                    return;
                }
                for (bag2 = OpenBagList; bag2 != 0; bag2 = bag2->next)
                    if (Obj_PtrTMem(&bag2->obj.word) == obj)
                        return;
            }
            held = 1;
            SetCursorObj(slot, split != 0 && split != obj);
            if (slot > 19) {
                for (bag = OpenBag; bag != 0; bag = bag->prev)
                    bag->weight -= ItemWeight(obj);
                FixOpenBag();
                DisplayOpenBag();
            } else
                DisplayInvObject(SlotToDisplay[slot]);
            if (CursorObjPtr == 0)
                return;
            if (split != 0) {
                GameInputMode = 1;
                return;
            }
        }
        mouse_release(1);
        mouse_getxy(&x, &y);
        newhit = FindInventoryHit(x, y);
        if (inplist->buttons == 1 && newhit != hit)
            hit = -1;
        else
            hit = newhit;
    }
    mouse_release(1);
    if (CursorObjPtr != 0 && GameInputMode != 2) {
        GameInputMode = 1;
        if (RightPanel != 0 && hit != 0x17)
            return;
        if (hit > 0) {
            if (hit < 21)
                RearrangeInventory(DisplayToSlot[hit]);
            else {
                DoSpecialActions(hit);
                held = 0;
            }
        }
    } else if (hit > 0) {
        if (how >= 0 || hit >= 21 && how == -2) {
            DoSpecialActions(hit);
            held = 0;
        } else if (how == -2)
            inv_look();
    }
    if (held && CursorObjPtr == 0) {
        unforce_mouse_cursor(3);
        GameInputMode = 0;
    }
}

void far DoInventoryDrag(struct Object far *obj)
{
    int x;
    int y;
    int buttons;
    register int hit;

    CursorObjPtr = obj;
    force_mouse_cursor(OBJ_ID(CursorObjPtr));
    mouse_getbut(&buttons);
    if (buttons != 0) {
        mouse_release(1);
        if (CursorObjPtr != 0) {
            mouse_getxy(&x, &y);
            hit = FindInventoryHit(x, y);
            if (hit > 0) {
                GameInputMode = 1;
                if (player->sleepbits != 0 && player->in_void)
                    return;
                if (RightPanel != 0 && hit != 0x17)
                    return;
                if (hit < 21) {
                    RearrangeInventory(DisplayToSlot[hit]);
                    if (CursorObjPtr == 0) {
                        GameInputMode = 0;
                        unforce_mouse_cursor(3);
                    }
                } else
                    DoSpecialActions(hit);
            }
        }
    }
}

/* IDA LoadMaleOrFemaleArmourArt_ovr125_68B; FM Towns load_inv_pic_ sits in the same place
   and patches the same string the same way. */
char far load_inv_pic(int n, int img)
{
    register char *name = "armor_f";

    if (player->female != 1)
        name[6] = 'm';
    else
        name[6] = 'f';
    reload_gr_vpic(n + 0x206D, name, img);
    return 1;
}

void far DisplayInvSpecial(void)
{
    struct Object far *obj;
    int type;
    int q;
    register int slot;
    register int i;

    if (RightPanel == 0) {
        mouse_hide();
        if (inv_refresh) {
            grSoftPageFlip();
            set_the_color(0x106);
            rectangle(0xF0, 0xBE, 0x13B, 0x51);
        }
        pic_to_screen(0x206D, InvDisplay[0].x, InvDisplay[0].y, InvDisplay[0].h, InvDisplay[0].w);
        Transparency = 1;
        for (i = 0; i < 5; i++) {
            slot = ArmorSlots[i];
            if (Inventory[DisplayToSlot[slot]].f.link != 0) {
                obj = Obj_PtrTMem(&Inventory[DisplayToSlot[slot]].word);
                type = OBJ_TYPE(obj) & 0x1F;
                if (type > 14)
                    q = 3;
                else
                    q = obj->qn.f.quality >> 4;
                if (type + 1 != invArmorObj[slot] || q + 1 != invArmorQ[slot]) {
                    invArmorObj[slot] = type + 1;
                    invArmorQ[slot] = q + 1;
                    type += q * 15;
                    load_inv_pic(slot, type);
                }
                pic_to_screen(slot + 0x206D, InvDisplay[slot].x, InvDisplay[slot].y,
                              InvDisplay[slot].h, InvDisplay[slot].w);
            }
        }
        displayInventoryArray(6, 7);
        Transparency = 0;
        save_rect(SaveHandles[11], InvDisplay[11].x, InvDisplay[11].y, InvDisplay[11].w - 5,
                  InvDisplay[11].h);
        save_rect(SaveHandles[10], InvDisplay[10].x + 5, InvDisplay[10].y, InvDisplay[10].w - 5,
                  InvDisplay[10].h);
        if (Inventory[9].f.link != 0 || Inventory[10].f.link != 0)
            displayInventoryArray(10, 11);
        if (inv_refresh) {
            grSoftPageFlip();
            set_the_color(0x106);
            rectangle(0xF0, 0xBE, 0x13B, 0x51);
        }
        if (displayEnc(1))
            set_font_size(1);
        mouse_show();
    }
}

void far DisplayInvObject(int slot)
{
    int pic;

    pic = -1;
    if (RightPanel == 0) {
        if (slot < 6)
            DisplayInvSpecial();
        else if (slot < 21)
            displayInventoryArray(slot, slot);
        else {
            restore_rect(SaveHandles[slot]);
            if (slot == 21) {
                if (InvUpArrow)
                    pic = 0x101B;
            } else if (InvDownArrow)
                pic = 0x101C;
            if (pic >= 0) {
                Transparency = 1;
                pic_to_screen(pic, InvDisplay[slot].x, InvDisplay[slot].y, InvDisplay[slot].h,
                              InvDisplay[slot].w);
                Transparency = 0;
            }
        }
    }
}

void far SetCursorObj(int slot, char keep)
{
    char had;
    int link;

    had = CursorObjPtr != 0;
    if (keep)
        link = Obj_MemTPtr(Obj_PtrTMem(&WhatsInSlot(slot)->qn.word));
    CursorObjPtr = takeFromSlot(-1, -1, -1, slot, 0);
    if (CursorObjPtr != 0) {
        if (keep) {
            Inventory[slot].f.link = link;
            FixPlayerEquips();
        }
        mouse_hide();
        if (had)
            unforce_mouse_cursor(0);
        force_mouse_cursor(OBJ_ID(CursorObjPtr));
        mouse_show();
        FixPlayerEquips();
    }
}

struct Object far * far AskHowMany(struct Object far *obj)
{
    int orig;
    struct Object far *split;
    char *msg;
    char initial[4];
    char result[4];
    register int n;
    register int key;

    orig = obj->ol.f.link;
    split = 0;
    n = 1;
    msg = "Move how many? ";
    initial[0] = '1';
    initial[1] = 0;
    key = wdialog(msg, initial, result, 0, 3);
    if (key != 27 && key != 3) {
        if (key != 0 && key < 4) {
            if (key == 1)
                n = 1;
            else if (key == 2)
                n = orig;
            else if (key == 27)
                n = 0;
            wd_replace(n);
        } else {
            n = atoi(result);
            if (n > orig)
                n = orig;
        }
        scroll_print("\n");
    } else
        n = 0;
    mouse_dragged(1);
    if (n != 0) {
        if (obj->ol.f.link != n) {
            split = Obj_Alloc(0);
            *split = *obj;
            split->ol.f.link -= n;
            obj->ol.f.link = n;
        } else
            split = obj;
    }
    return split;
}

int far ItemFitsSlot(struct Object far *obj, int slot)
{
    struct ComObj *com;
    struct Object far *cont;
    char *cls;
    int major;
    int minor;
    int sub;
    int weight;
    int id;
    register int cap;
    register int i;

    cont = 0;
    id = OBJ_ID(obj);
    ActiveObj = obj;
    com = &ComObjData[id];
    major = OBJ_MAJOR(ActiveObj);
    minor = OBJ_MINOR(ActiveObj);
    sub = ActiveObj->id & 0xF;
    if (slot == 19) {
        int j;

        if (OpenBag == 0)
            return 0;
        if (OpenBag->prev == 0) {
            for (j = 11; j <= 18; j++)
                if (Inventory[j].f.link == 0)
                    break;
            if (j > 18)
                game_sprint(0x112);
            return j <= 18;
        }
        cont = Obj_PtrTMem(&OpenBag->prev->obj.word);
    } else if (slot > 19) {
        struct Object far *o;

        o = Obj_PtrTMem(&Inventory[slot].word);
        if (o != 0 && OBJ_CLASS(o) == 8)
            cont = o;
        else
            cont = Obj_PtrTMem(&Inventory[19].word);
    } else
        cont = Obj_PtrTMem(&Inventory[slot].word);
    if (slot < 5) {
        if (major != 0) {
            if (slot == 0 && UseFood(ThePlayer, obj, 0) > 0)
                return -1;
            return 0;
        }
        if (OBJ_MINOR(obj) < 2)
            return 0;
        cls = get_class_data();
        switch (slot) {
        case 1:
            return cls[3] == 1;
        case 3:
            return cls[3] == 3;
        case 2:
            return cls[3] == 4;
        case 0:
            return cls[3] == 8;
        case 4:
            return cls[3] == 5;
        }
        return 0;
    }
    if (slot == 9 || slot == 10) {
        if (major != 0 || OBJ_MINOR(obj) < 2)
            return 0;
        cls = get_class_data();
        return cls[3] == 9;
    }
    if (8 - player->lefty == slot && major == 0 && minor == 0) {
        if (cont != 0 && OBJ_ID(cont) == id
            || OBJ_ISQUANT(obj) && !(obj->ol.f.link & 0x200) && obj->ol.f.link > 1)
            return 0;
    } else if (major == 2 && minor == 1 && sub >= 4 && sub < 8) {
        obj->id = obj->id & 0xFFF0 | (sub - 4) & 0xF;
        if (!ItemFitsSlot(obj, slot)) {
            obj->id = obj->id & 0xFFF0 | sub & 0xF;
            return 0;
        }
        for (i = 0; i < 4; i++)
            if (ValidLightSlots[i] == slot)
                obj->id = obj->id & 0xFFF0 | sub & 0xF;
        return 1;
    }
    if (cont != 0 && OBJ_CLASS(cont) == 8) {
        int ok;

        ok = 1;
        weight = ItemWeight(obj);
        {
            struct Bag far *bag;
            char name[26];

            if (slot > 19) {
                for (bag = OpenBag; bag != 0; bag = bag->prev) {
                    cap = Containers[Obj_PtrTMem(&bag->obj.word)->id & 0xF].capacity;
                    ok &= cap == 0 || bag->weight + weight <= cap;
                }
            }
            BagWeight(&cont->ol.word, &weight);
            cap = Containers[cont->id & 0xF].capacity;
            ok &= cap == 0 || weight <= cap;
            if (!ok) {
                get_name(name, cont, 0, 0);
                scroll_print("The ");
                scroll_print(name);
                game_sprint(0xD0);
                return 0;
            }
        }
        cap = Containers[cont->id & 0xF].mask;
        if (cap >= 0) {
            unsigned char res;

            if (cap < 0x200) {
                if (id != cap)
                    game_sprint(0x107);
                return id == cap;
            }
            switch (cap) {
            case 0x200:
                if (!(res = major == 3 && (minor == 3 || minor == 2 && sub > 7))) {
                    game_sprint(0x106);
                    return 0;
                }
                break;
            case 0x204:
                if (!(res = major == 4 && minor == 0 && sub != 0))
                    res = OBJ_CLASS(obj) == 8 && Containers[obj->id & 0xF].mask == 0x204;
                break;
            case 0x201:
                res = major == 0 && minor == 1 && sub < 3 || major == 2 && minor == 1 && sub >= 8;
                break;
            case 0x202:
                res = major == 4 && minor == 3 && (sub >= 4 && sub < 8 || sub == 9 || sub == 10);
                break;
            case 0x203:
                res = major == 2 && minor == 3 && id != 0xBB && id != 0xBC && id != 0xBD
                    || id == 0xCE || id == 0xCF || id == 0x114 || id == 0xD2 || id == 0x13E
                    || id == 0xC5;
                break;
            default:
                res = 0;
            }
            if (!res)
                game_sprint(0x107);
            return res;
        }
    }
    return com->pickup;
}

void far RearrangeInventory(int slot)
{
    if (Inventory[slot].f.link == 0) {
        if (AddToEmptySlot(CursorObjPtr, slot))
            CursorObjPtr = 0;
    } else if (AddToOccupiedSlot(CursorObjPtr, slot))
        CursorObjPtr = 0;
}

unsigned char far AddToEmptySlot(struct Object far *obj, int slot)
{
    char ok;

    ok = 0;
    if (slot == 19)
        return 0;
    if (AddToInventory(obj, slot)) {
        if (slot > 18) {
            FixOpenBag();
            DisplayOpenBag();
        } else
            DisplayInvObject(SlotToDisplay[slot]);
        ok = 1;
    }
    return ok;
}

char far AddTogether(struct Object far *obj, struct Object far *onto)
{
    register int q1;
    register int q2;

    /* Every return 0 here is one jump to the final return 0: the compiler shares them
       only when that last statement is itself reachable. */
    if (OBJ_ID(obj) != OBJ_ID(onto))
        return 0;
    if (!OBJ_ISQUANT(obj) && obj->ol.f.link > 0
        || !OBJ_ISQUANT(onto) && onto->ol.f.link > 0
        || obj->ol.f.link & 0x200 || onto->ol.f.link & 0x200
        || ComObjData[OBJ_ID(obj)].stack == 1 || ComObjData[OBJ_ID(obj)].stack == 3)
        return 0;
    if (OBJ_CLASS(obj) == 0x10 && obj->ol.f.owner != onto->ol.f.owner)
        return 0;
    if (obj->ol.f.link + onto->ol.f.link < 999) {
        if (OBJ_ID(obj) >= 0x10 && OBJ_ID(obj) <= 0x12)
            return 1;
        if (OBJ_ID(obj) == 0xA1)
            return 0;
        q1 = obj->qn.f.quality;
        q2 = onto->qn.f.quality;
        return q1 >> 4 == q2 >> 4 && (q1 != 0 && q2 != 0 || q1 == q2);
    }
    return 0;
}

char far AddToOccupiedSlot(struct Object far *obj, register int slot)
{
    struct Object far *target;
    char ok;
    int qty;
    int combo;
    register int weight;

    ok = 0;
    target = Obj_PtrTMem(&Inventory[slot].word);
    if (OBJ_MAJOR(target) == 2 && OBJ_MINOR(target) == 0) {
        char r;

        r = PutObjectInBag(obj, slot);
        FixPlayerEquips();
        return r;
    }
    if (AddTogether(obj, target)) {
        struct Bag far *bag;

        if (ItemFitsSlot(obj, slot) != 1)
            return 0;
        if (OBJ_ISQUANT(obj))
            qty = obj->ol.f.link;
        else
            qty = 1;
        if (!OBJ_ISQUANT(target)) {
            target->id = target->id & 0x7FFF | 0x8000;
            target->ol.f.link = 1;
        }
        weight = ComObjData[OBJ_ID(obj)].mass * qty;
        if (slot > 19)
            for (bag = OpenBag; bag != 0; bag = bag->prev)
                bag->weight += weight;
        PlayerDat.weight += weight;
        FixPlayerEquips();
        target->ol.f.link = target->ol.f.link + qty;
        target->qn.f.quality = (target->qn.f.quality + obj->qn.f.quality) >> 1;
        Obj_Free(obj);
        ok = 1;
    } else if ((combo = ObjsBeCombinable(obj, target)) >= 0) {
        struct Object far *made;
        unsigned char used;

        if ((made = CombineObjs(combo)) == 0)
            return 0;
        if ((used = RemoveAfterCombine(obj, combo)) == 1) {
            Obj_Punt(0, obj, 1);
            CursorObjPtr = made;
            GameInputMode = 1;
            unforce_mouse_cursor(3);
            force_mouse_cursor(OBJ_ID(CursorObjPtr));
        }
        if (RemoveAfterCombine(target, combo)) {
            if (!used)
                AddToEmptySlot(made, slot);
            InvRemoveObject(target);
            Obj_Punt(0, target, 1);
        }
        ok = 0;
    } else if (slot <= 18) {
        struct Object far *old;

        old = removeFromSlot(-1, -1, -1, slot, 0);
        if (!AddToEmptySlot(obj, slot))
            AddToEmptySlot(old, slot);
        else
            CursorObjPtr = old;
        if (CursorObjPtr != 0) {
            unforce_mouse_cursor(0);
            force_mouse_cursor(OBJ_ID(CursorObjPtr));
        }
    } else
        SwapItemsInBag(obj, slot);
    DisplayInvObject(SlotToDisplay[slot]);
    return ok;
}

void far DisplayInventory(void)
{
    if (RightPanel == 0) {
        shown_capacity = -1;
        if (OpenBag != 0)
            pic_to_screen(0x2073, 0xEE, 0x7A, 0x2B, 0x4C);
        else
            restore_rect(1);            /* FM Towns restores SaveHandles[1]; DOS pushes 1 */
        displayInventoryArray(6, 0x16);
    }
}

void far displayInventoryArray(int from, int to)
{
    int id;
    struct Object far *obj;
    char buf[6];
    char font_set;
    char any_qty;
    int slot;
    int qty[23];
    register int i;
    register int n;

    font_set = 0;
    any_qty = 0;
    mouse_hide();
    Transparency = 1;
    for (i = from; i <= to; i++) {
        restore_rect(SaveHandles[i]);
        qty[i] = 1;
        if (i <= 20) {
            slot = DisplayToSlot[i];
            if (Inventory[slot].f.link != 0) {
                obj = Obj_PtrTMem(&Inventory[slot].word);
                id = OBJ_ID(obj);
                pic_to_screen(id, InvDisplay[i].x, InvDisplay[i].y, InvDisplay[i].h, InvDisplay[i].w);
                if (OBJ_ISQUANT(obj) && !(obj->ol.f.link & 0x200)) {
                    n = obj->ol.f.link;
                    if (n > 1) {
                        qty[i] = n;
                        any_qty = 1;
                    }
                }
            }
        } else
            DisplayInvObject(i);
    }
    Transparency = 0;
    if (any_qty) {
        set_font_size(0);
        *foreground_color = 2;
        font_set = 1;
        for (i = from; i <= to; i++)
            if ((n = qty[i]) > 1)
                string_to_screen(itoa(n, buf, 10), InvDisplay[i].x + 3, InvDisplay[i].y - 1);
    }
    if (font_set)
        set_font_size(1);
    displayEnc(0);
    mouse_show();
}

char far displayEnc(char show)
{
    char drawn;
    char buf[5];
    register int left;

    drawn = 0;
    left = PlayerDat.capacity - PlayerDat.weight;
    if (shown_capacity != left) {
        restore_rect(SaveHandles[0]);
        shown_capacity = left;
        if (show)
            drawn = 1;
        *foreground_color = 0x88;
        itoa(left / 10, buf, 10);
        string_to_screen(buf, 8 - string_width(buf) / 2 + 0x129, 0x8F);
    }
    return drawn;
}

int far FindInventoryHit(int x, int y)
{
    register struct InvRect *r;
    register int i;

    if (inplist->field8 != 4) {
        if (x > PLeft && PLeft + PWid > x && y > PBot && PBot + PHgt > y)
            return 0x17;
    } else if (x > 0x77 && x < 0xA3 && y > 0x87 && y < 0xBC)
        return 0x18;
    for (i = 0; i < 23; i++) {
        r = &InvDisplay[i];
        if (r->left <= x && r->right >= x && r->bottom <= y && r->top >= y)
            return i;
    }
    return -1;
}
