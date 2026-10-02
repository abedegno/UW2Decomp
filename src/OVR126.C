/* target: ovr126 */
/* opts: -mm -1 -G -O -Y -d */
/* Looking at things: the description printed when the player looks at an object, its
   quality, enchantment, owner and charges, books, scrolls, gravestones, bones, keys and
   critters, and the short description used elsewhere: the whole of DOS overlay ovr126, in
   original order. Function and global names are the originals from the FM Towns symbol
   table; the source file's own name is not known. */

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include "gfx.h"
#include "inv.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"

#define OBJ_ID(o)       ((o)->id & ID_ITEM)
#define OBJ_MAJOR(o)    (((o)->id & ID_MAJOR) >> 6)
#define OBJ_MINOR(o)    (((o)->id & ID_MINOR) >> 4)
#define OBJ_CLASS(o)    (((o)->id & ID_CLASS) >> 4)
#define OBJ_FLAGS(o)    (((o)->id & ID_FLAGS) >> 9)
#define OBJ_ISQUANT(o)  (((o)->id & ID_ISQUANT) >> 15)
#define OBJ_GOAL(o)     (((o)->goal_word & 0xF) >> 0)
#define OBJ_ATTITUDE(o) (((o)->attitude_word & 0xC000) >> 14)

/* Copy a far string, terminator included, to any address. */
#define far_strcpy(d, s) movedata(FP_SEG(s), FP_OFF(s), FP_SEG(d), FP_OFF(d), str_len(s) + 1)

extern char always_decode;
extern unsigned TxmTerr[];

void far scroll_print(char far *s);
void far scroll_clear(int n);
void far get_name(char far *buf, struct Object far *obj, int article, char plural);
char far decode_obj_spell(struct Object far *obj, int *major, int *effect, unsigned char *flag);
struct Object far * far Obj_InList(unsigned far **head, int a, int major, int minor, int idx);
void far player_look_grave(int n);
void far show_cutscene(int n);

void far LookAt(struct Object far *obj, int lore)
{
    char far *pos;
    char far *qstr;
    char numbuf[6];
    char first;
    char name[10];
    char plural;
    char far *owner;
    char text[80];
    char qual[26];
    struct ComObj *com;
    int q;

    plural = 0;
    if (obj == 0)
        return;
    com = &ComObjData[OBJ_ID(obj)];
    if (com->lookable) {
        strcpy(text, "You see ");
        first = 0;
        name[0] = 0;
        if (do_mods(obj, lore, name))
            first = name[0];
        qual[0] = 0;
        if (OBJ_MAJOR(obj) == MAJOR_CREATURE) {
            CritterLook(obj, text);
            str_cat(text, ".\n");
            scroll_print(text);
            return;
        }
        q = 0;
        if (obj->qn.f.quality != 0 && (obj->qn.f.quality > 1 || (obj->id & 0x1F8) != FIRST_LIGHT))
            q = com->qualclass != 3 ? (obj->qn.f.quality >> 4) + 1 : 5;
        qstr = get_string((com->qualtype * 6 + q) | STR_OBJLOOK);
        if (qstr != 0 && *qstr != 0) {
            far_strcpy(qual, qstr);
            first = *qstr;
        }
        if (OBJ_ISQUANT(obj) && !(obj->ol.f.link & LINK_SPECIAL) && obj->ol.f.link > 1) {
            plural = 1;
            first = 'x';
            strcat(text, itoa(obj->ol.f.link, numbuf, 10));
            strcat(text, " ");
        } else if (first != 0 && com->qualtype != 13) {
            switch (first) {
            case 'a': case 'e': case 'i': case 'o': case 'u':
                strcat(text, "an ");
                break;
            default:
                strcat(text, "a ");
            }
        }
        if (qual[0] != 0) {
            strcat(text, qual);
            strcat(text, " ");
        }
        if (name[0] != 0)
            strcat(text, name);
        pos = text + strlen(text);
        get_name(pos, obj, first == 0, plural);
        do_of(obj, lore, text);
        if (ComObjData[OBJ_ID(obj)].can_own && obj->ol.f.owner > 0 && (obj->ol.f.owner & 0x1F) <= 0x1E) {
            strcat(text, " belonging to");
            owner = get_string(((obj->ol.f.owner & 0x1F) + 0x172) | STR_GAME);
            str_cat(text, owner);
        }
        strcat(text, ".\n");
        scroll_print(text);
    } else if (OBJ_CLASS(obj) == CLASS_DECAL)
        RectLook(obj, lore);
    SpecialLook(obj, lore);
}

char far do_mods(struct Object far *obj, int lore, char *s)
{
    int major;
    int effect;
    unsigned char flag;
    char far *str;

    if (decode_obj_spell(obj, &major, &effect, &flag)) {
        if (lore == 3) {
            if (!flag && major == 9) {
                strcat(s, "cursed ");
                return 1;
            }
        } else if (lore == 2 || (OBJ_CLASS(obj) == CLASS_BOOK && (obj->id & ID_INCLASS) <= 8)) {
            strcat(s, "magical ");
            return 1;
        }
    }
    if ((obj->id & 0x1F8) == FIRST_BLACKROCK_GEM) {
        str = get_string(((obj->ol.f.owner > 0) + 0x164) | STR_GAME);
        far_strcpy(s + strlen(s), str);
        return 1;
    }
    return 0;
}

char far do_of(struct Object far *obj, int lore, char *s)
{
    int major;
    int effect;
    unsigned char flag;
    char far *name;
    char found;
    unsigned far *link;
    struct Object far *spell;
    int charges;

    always_decode = 1;
    if (lore == 3 && OBJ_ID(obj) >= FIRST_POTION && OBJ_ID(obj) <= ITEM_BROWN_POTION && !OBJ_ISQUANT(obj)) {
        link = &obj->ol.word;
        if (Obj_InList(&link, 0, 6, 0, 0) != 0) {
            strcat(s, " of Poison");
            return 1;
        }
    }
    found = decode_obj_spell(obj, &major, &effect, &flag);
    always_decode = 0;
    if (found != 0 && lore == 3) {
        if (major == 12) {
            major = 0x1C0;
            if (OBJ_MINOR(obj) > 1)
                effect += 0x10;
            effect += major;
        } else if (major == 9)
            return 0;
        else if (flag != 0 && major <= 0)
            effect += 0x100;
        else
            effect += major << 4;
        name = get_string(effect | STR_SPELLS);
        if (name == 0 || *name == 0)
            name = "UNNAMED";
        strcat(s, " of ");
        far_strcpy(s + strlen(s), name);
        if (!OBJ_ISQUANT(obj)) {
            charges = -1;
            link = &obj->ol.word;
            spell = Obj_InList(&link, 0, MAJOR_SPEC, 2, 0);
            if (spell != 0 && (spell->id & 0x800))
                charges = spell->qn.f.quality;
            if (charges >= 0) {
                strcat(s, " with ");
                if (charges > 0) {
                    char num[3] = "00";

                    if (OBJ_ID(obj) >= FIRST_POTION && OBJ_ID(obj) <= ITEM_BROWN_POTION)
                        charges = 1;
                    num[1] = charges % 10 + '0';
                    if (charges > 9) {
                        num[0] = charges / 10 + '0';
                        strcat(s, num);
                    } else
                        strcat(s, &num[1]);
                } else
                    strcat(s, "no");
                strcat(s, " full charge");
                if (charges != 1)
                    strcat(s, "s");
            }
        }
        return 1;
    }
    return 0;
}

void far BookLook(struct Object far *obj, int print)
{
    char far *str;
    char text[100];

    if (print < 1)
        return;
    if (OBJ_ID(obj) == ITEM_MAP) {
        game_sprint(0xA5);
        return;
    }
    if (OBJ_ID(obj) == ITEM_BIT_OF_A_MAP)
        return;
    if ((obj->id & ID_ENCHANT) && OBJ_MAJOR(obj) != MAJOR_RECT)
        return;
    if (obj->id & ID_FLAG10) {
        show_cutscene((obj->ol.f.link & 0x1FF) + 0x100);
        return;
    }
    if ((obj->ol.f.link & 0x1FF) > 0xFF)
        scroll_print(get_string((obj->ol.f.link & 0x1FF) | STR_BOOKS));
    else {
        strcpy(text, "You read the ");
        get_name(text + strlen(text), obj, 0, 0);
        strcat(text, "...\n");
        scroll_print(text);
        if ((obj->ol.f.link & 0x1FF) == 6)
            player->quests[26] = (player->quests[26] & 0xFFFFFFFBL) + 4;
        str = get_string((obj->ol.f.link & 0x1FF) | STR_BOOKS);
        scroll_print(str);
        scroll_print("\n");
    }
}

void far RectLook(struct Object far *obj, int look)
{
    int fd;
    unsigned char c;
    unsigned char ok;
    char far *grave;
    char far *str;
    struct Object far *o;
    register int base;
    register int index;

    base = 0x160;
    c = 0;
    switch (obj->id & ID_INCLASS) {
    case 14:
    case 15:
        if (look >= 0)
            look_nothing(2, obj->ol.f.owner + 1);
        if (look > 0 && (TxmTerr[obj->ol.f.owner] & 7) == 5)
            player_look_shaft();
        else if (look == -1)
            break;
        break;
    case 4:
        if (OBJ_FLAGS(obj) >= 2)
            look_nothing(2, OBJ_FLAGS(obj) + 0x3F);
        else
            game_sprint(0xBA);
        break;
    case 6:
        base += 0x10;
    case 5:
        index = OBJ_ISQUANT(obj) ? obj->ol.f.link & 0x1FF : obj->ol.f.owner;
        if ((obj->id & ID_INCLASS) == 5) {
            ok = (fd = open("DATA\\grave.dat", O_RDONLY | O_BINARY)) != -1;
            ok &= lseek(fd, index, 0) != -1L;
            ok &= read(fd, &c, 1) == 1;
            ok &= close(fd) != -1;
            if (!ok)
                break;
        }
        if ((grave = get_string(index | STR_WRITING)) != 0 && c != 0)
            scroll_clear(1);
        if ((obj->id & ID_INCLASS) == 6 || c == 0) {
            base = base + OBJ_FLAGS(obj);
            str = get_string(base | STR_WRITING);
            scroll_print(str);
        }
        if (grave != 0) {
            o = obj;
            if (index != 0 || find_obj(6, 2, 3, &o) == 0) {
                grave = fix_name_string(grave, 1, 0);
                scroll_print(grave);
                scroll_print("\n");
            }
        }
        if (c != 0)
            player_look_grave(c);
        break;
    }
}

void far BonesLook(struct Object far *obj, int print)
{
    struct Object tmp;
    char text[40];
    int id;

    if (print != 0 && obj->ol.f.owner != 0 && obj->ol.f.owner != 0x28
        && (obj->ol.f.owner < 0x3C || obj->ol.f.owner == 0x3F)) {
        id = 0x1A;
        if (OBJ_ID(obj) == ITEM_PILE_OF_BONES_C6 || obj->ol.f.link > 1)
            id++;
        game_sprint(id);
        if (obj->ol.f.owner == 0x3D)
            scroll_print("Relk.\n");
        if (obj->ol.f.owner == 0x3F)
            scroll_print("an adventurer.\n");
        else {
            tmp.id = tmp.id & 0xFE00 | (obj->ol.f.owner + FIRST_CREATURE) & ID_ITEM;
            tmp.whoami = 0;
            get_name(text, &tmp, 1, 0);
            scroll_print(text);
            scroll_print(".\n");
        }
    }
}

void far KeyLook(struct Object far *obj, int print)
{
    char far *str;

    if (print != 0) {
        str = get_string((obj->ol.f.owner + 0x64) | STR_OBJLOOK);
        if (str != 0)
            scroll_print(str);
    }
}

void far CritterLook(struct Object far *obj, char *s)
{
    char far *desc;
    char far *name;
    char far *attitude;
    unsigned char who;

    desc = get_string(OBJ_ID(obj) | STR_OBJNAMES);
    if (*desc == 0) {
        if (OBJ_GOAL(obj) == 11)
            desc = get_string(((obj->b15 & 0x3F) + 0x115) | STR_GAME);
        else
            desc = 0;
    }
    who = obj->whoami;
    if (who < 0xF0 || who == 0xFF) {
        attitude = get_string((OBJ_ATTITUDE(obj) + 0x60) | STR_OBJLOOK);
        if (*attitude == 0)
            attitude = 0;
    } else
        attitude = 0;
    if (who > 0) {
        name = get_string((who + 0x10) | STR_CONV);
        if (*name == 0)
            name = 0;
    } else
        name = 0;
    if (desc != 0 && attitude != 0) {
        switch (*attitude) {
        case 'a': case 'e': case 'i': case 'o': case 'u':
            strcat(s, "an ");
            break;
        default:
            strcat(s, "a ");
        }
        str_cat(s, attitude);
        strcat(s, " ");
    }
    if (desc != 0 && (name == 0 || isupper(*name)))
        str_cat(s, fix_name_string(desc, attitude == 0, 0));
    if (name != 0) {
        if (desc != 0 && isupper(*name))
            strcat(s, " named ");
        str_cat(s, fix_name_string(name, desc == 0, 0));
    }
}

void far SpecialLook(struct Object far *obj, int print)
{
    int minor;

    minor = OBJ_MINOR(obj);
    switch (OBJ_MAJOR(obj)) {
    case MAJOR_SPEC:
        if (minor == 3 && (obj->id & ID_INCLASS) < 10)
            BookLook(obj, print);
        else if (minor == 0)
            KeyLook(obj, print);
        break;
    case MAJOR_STUFF:
        if (minor == 0 && OBJ_ID(obj) >= ITEM_SKULL_C2 && OBJ_ID(obj) <= ITEM_PILE_OF_BONES_C6)
            BonesLook(obj, print);
        break;
    case MAJOR_RECT:
        if (minor == 0 && (obj->id & ID_INCLASS) < 8 && (obj->ol.f.owner & 1))
            game_sprint(0x91);
        break;
    }
}

int far GetObjDesc(struct Object far *obj, int lore, char *s)
{
    char far *qstr;
    char *end;
    int qty;
    int q;
    char qual[26];
    struct ComObj *com;

    qty = 1;
    com = &ComObjData[OBJ_ID(obj)];
    q = 0;
    *s = 0;
    if (OBJ_MAJOR(obj) == MAJOR_CREATURE)
        CritterLook(obj, s);
    else {
        if (obj->qn.f.quality != 0)
            q = com->qualclass != 3 ? (obj->qn.f.quality >> 4) + 1 : 5;
        qstr = get_string((com->qualtype * 6 + q) | STR_OBJLOOK);
        if (qstr != 0 && *qstr != 0)
            str_copy(qual, qstr);
        else
            qual[0] = 0;
        if (OBJ_ISQUANT(obj) && !(obj->ol.f.link & LINK_SPECIAL))
            qty = obj->ol.f.link;
        if (qual[0] != 0) {
            str_cat(s, qual);
            str_cat(s, " ");
        }
        end = s + strlen(s);
        get_name(end, obj, 0, qty > 1);
    }
    return qty;
}
