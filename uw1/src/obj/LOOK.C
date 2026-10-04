/* target: ovr122 */
/* opts: -mm -1 -G -O -Y -d */
/* Looking at things: the description printed when the player looks at an object, its
   quality, enchantment, owner and charges, books, scrolls, gravestones, bones, keys,
   critters and talismans: the whole of UW1's DOS overlay ovr122 (UW2's ovr126), in original
   order.

   What it does in the game: LookAt builds 'You see a <quality> <name> of <spell> with N full
   charges belonging to <race>.' from the object's tables: the quality word from string
   block 5 by the item's quality type and quality band, the name from block 4 (get_name), the
   enchantment from block 6, the owner's race from block 1. Then SpecialLook adds what some
   kinds of object say for themselves: a book or scroll's text, a key's description, whose
   bones these are, a spiked door. Critters are described and printed by CritterLook
   (attitude, kind and name); gravestones and plaques by RectLook. Callers: the look command,
   the inventory, Name Enchantment (lore 3).

   The lore argument says how much the player knows: 2 shows that an object is magical, 3
   (identified) names the enchantment and charges, and describes a talisman of virtue by its
   own text (talisman_desc).

   UW1 against UW2: no GetObjDesc, no blackrock gems, no ' of Poison' or potion charges, no
   quest set by reading, no 'Relk.'; a curse adds 'cursed ' without counting as an article
   word; talisman_desc is new; item ids and string numbers differ.
   UW1 has no symbol-bearing build: the names are UW2's (the FM Towns symbol table), the
   routines being the same; talisman_desc is descriptive, chosen for the stub order.
   Name: descriptive (looking at things: LookAt). */

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#define decode_obj_spell UW2_decode_obj_spell
#include "gfx.h"
#include "inv.h"
#include "map.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"

/* UW1: decode_obj_spell's flag is a plain char (object.h: unsigned char). */
#undef decode_obj_spell
char far decode_obj_spell(struct Object far *obj, int16 *major, int16 *effect, char *flag);

char far talisman_desc(struct Object far *obj, struct ComObj *com);

/* Copy a far string, terminator included, to any address. */
#define far_strcpy(d, s) FAR_COPY(d, s, str_len(s) + 1)



/* Prints the look description of obj (see the file comment). The quality word is skipped
   for quality 0; indestructible items (qualclass 3) use the fifth word of their quality
   type; quality type 13 takes no article. An ownable object with an owner 1..27 is
   'belonging to' that race (strings 0x172 + owner). Non-lookable decals go to RectLook.
   UW1: at lore 3 a talisman is described by talisman_desc instead; critters are described
   and printed by CritterLook. */
void far LookAt(struct Object far *obj, register int lore)
{
    char far *pos;
    char far *qstr;
    char numbuf[6];
    char first;
    char name[10];
    char plural;
    /* match: 80 bytes, but a long identified, enchanted, owned item's description can run
       past it with its terminator: in DOS the end lands on owner (or q), plural and name,
       all used by then; the host's frame is the compiler's, so the host's array is long
       enough (FRAME_LEN) */
    char text[FRAME_LEN(80, 96)];
    char qual[26];
    register struct ComObj *com;

    plural = 0;
    if (obj == 0)
        return;
    com = &ComObjData[OBJ_ITEM(obj)];
    if (com->lookable) {
        if (lore == 3 && talisman_desc(obj, com))
            return;
        strcpy(text, "You see ");
        first = 0;
        name[0] = 0;
        if (do_mods(obj, lore, name))
            first = name[0];
        qual[0] = 0;
        if (OBJ_MAJOR(obj) == MAJOR_CREATURE) {
            CritterLook(obj, text);
            return;
        }
        {
            int q;

            q = 0;
            if (obj->qn.f.quality != 0)
                q = com->qualclass != 3 ? (obj->qn.f.quality >> 4) + 1 : 5;
            qstr = get_string((com->qualtype * 6 + q) | STR_OBJLOOK);
        }
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
        if (ComObjData[OBJ_ITEM(obj)].can_own && obj->ol.f.owner > 0 && (obj->ol.f.owner & 0x1F) <= 0x1B) {
            char far *owner;

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

/* The word before the name: 'magical ' for an enchanted object at lore 2. UW1: at lore
   3 a curse (major 9) adds 'cursed ' but still returns 0, so LookAt takes its article from
   the quality word; there are no blackrock gems. Returns 1 when it added 'magical '. */
char far do_mods(struct Object far *obj, int lore, char *s)
{
    int16 major;
    int16 effect;
    char flag;

    if (decode_obj_spell(obj, &major, &effect, &flag)) {
        if (lore == 2) {
            strcat(s, "magical ");
            return 1;
        } else if (lore == 3 && !flag && major == 9)
            strcat(s, "cursed ");
    }
    return 0;
}

/* The words after the name, for an identified object (lore 3): ' of <enchantment>'
   (block 6: weapon and armour enchantments from 0x1C0, spells from 0x100, others by major
   class * 16) and, for an object whose spell has charges, ' with N full charges'. UW1 has
   no ' of Poison' and no potion rule. */
char far do_of(struct Object far *obj, int lore, register char *s)
{
    int16 major;
    int16 effect;
    char flag;
    char far *name;
    char found;
    union Link far *link;
    struct Object far *spell;
    int charges;

    always_decode = 1;
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
            link = &obj->ol.link;
            spell = Obj_InList(&link, 0, MAJOR_SPEC, 2, 0);
            if (spell != 0 && (spell->id & 0x800))
                charges = spell->qn.f.quality;
            if (charges >= 0) {
                strcat(s, " with ");
                if (charges > 0) {
                    char num[3] = "00";

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

/* Reads a book or scroll: a map says 'Enscribed upon the scroll is your map.'; an
   enchanted (spell) scroll says nothing here; a scroll flagged with ID_FLAG10 plays a
   cutscene (0x100 + its text number); otherwise 'You read the <name>...' and its text from
   block 3. UW1: an unnamed one reads 'You read the UNNAMED...'; no quest is set. */
void far BookLook(struct Object far *obj, int print)
{
    char far *str;
    char text[100];

    if (print < 1)
        return;
    if (OBJ_ITEM(obj) == 0x13B) {         /* UW1: the map (UW2 0x13A) */
        game_sprint(0x97);  /* 'Enscribed upon the scroll is your map.' */
        return;
    }
    if ((obj->id & ID_ENCHANT) && OBJ_MAJOR(obj) != MAJOR_RECT)
        return;
    if (obj->id & ID_FLAG10) {
        runcutscene((obj->ol.f.link & 0x1FF) + 0x100);
        return;
    }
    if ((obj->ol.f.link & 0x1FF) > 0xFF)
        scroll_print(get_string((obj->ol.f.link & 0x1FF) | STR_BOOKS));
    else {
        strcpy(text, "You read the ");
        if (!get_name(text + strlen(text), obj, 0, 0))
            strcat(text, "UNNAMED");
        strcat(text, "...\n");
        scroll_print(text);
        str = get_string((obj->ol.f.link & 0x1FF) | STR_BOOKS);
        scroll_print(str);
        scroll_print("\n");
    }
}

/* UW1: a word for each texture of the level (CONVERSE.C's name); 9 is a shaft. */
extern int16 w64_types[];

/* Looks at a decal: classes 14 and 15 describe the texture behind them (and a shaft
   texture, w64_types 9, prompts player_look_shaft), class 4 is a bridge, class 5 a
   gravestone and class 6 a plaque. A gravestone's picture number comes from
   DATA\GRAVE.DAT, indexed by its text number; with one, the scroll is cleared and the
   gravestone shown (player_look_grave), otherwise 'The gravestone reads: ' (or for a plaque
   'The plaque reads: ') and the inscription from block 8. */
void far RectLook(struct Object far *obj, int look)
{
    int index;
    unsigned char c;
    char ok;
    char far *grave;
    char far *str;
    register int base;
    register int fd;

    base = 0x160;
    c = 0;
    switch OBJ_INCLASS(obj) {
    case 14:
    case 15:
        if (look >= 0)
            look_nothing(2, obj->ol.f.owner + 1);
        if (look > 0 && (w64_types[obj->ol.f.owner] & 0xFF) == 9)
            player_look_shaft();
        else if (look == -1)
            break;
        break;
    case 4:
        if (OBJ_FLAGS(obj) >= 2)
            look_nothing(2, OBJ_FLAGS(obj) + 0x2F);
        else
            game_sprint(0xAB);  /* 'You see a bridge.' */
        break;
    case 6:
        base += 0x10;
    case 5:
        index = OBJ_ISQUANT(obj) ? obj->ol.f.link & 0x1FF : obj->ol.f.owner;
        if (OBJ_INCLASS(obj) == 5) {
            ok = (fd = open("DATA\\grave.dat", O_RDONLY | O_BINARY)) != -1;
            ok &= lseek(fd, index, 0) != -1L;
            ok &= read(fd, &c, 1) == 1;
            ok &= close(fd) != -1;
            if (!ok)
                break;
        }
        if ((grave = get_string(index | STR_WRITING)) != 0 && c != 0)
            scroll_clear(1);
        if (OBJ_INCLASS(obj) == 6 || c == 0) {
            base = base + OBJ_FLAGS(obj);
            str = get_string(base | STR_WRITING);
            scroll_print(str);
        }
        if (grave != 0) {
            grave = fix_name_string(grave, 1, 0);
            scroll_print(grave);
            scroll_print("\n");
        }
        if (c != 0)
            player_look_grave(c);
        break;
    }
}

/* Whose bones: 'It looks to be that of ' (or 'They look to be those of ' for a pile) and
   the creature kind from the owner field (+ FIRST_CREATURE), or 'an adventurer.' for 0x3F.
   Owners 0, 0x28 and 0x3C..0x3E say nothing. UW1 has no 'Relk.' case. */
void far BonesLook(struct Object far *obj, int print)
{
    struct Object tmp;
    char text[40];
    int id;

    if (print != 0 && obj->ol.f.owner != 0 && obj->ol.f.owner != 0x28
        && (obj->ol.f.owner < 0x3C || obj->ol.f.owner == 0x3F)) {
        id = 0x16;
        if (OBJ_ITEM(obj) == ITEM_PILE_OF_BONES_C6 || obj->ol.f.link > 1)
            id++;
        game_sprint(id);
        if (obj->ol.f.owner == 0x3F)
            scroll_print("an adventurer.\n");
        else {
            SET_ITEM(&tmp, obj->ol.f.owner + FIRST_CREATURE);
            tmp.whoami = 0;
            get_name(text, &tmp, 1, 0);
            scroll_print(text);
            scroll_print(".\n");
        }
    }
}

/* A key's description, string 0x64 + owner of block 5 (by lock, e.g. 'The key feels
   unnaturally heavy.'), if there is one. */
void far KeyLook(struct Object far *obj, int print)
{
    char far *str;

    if (print != 0) {
        str = get_string((obj->ol.f.owner + 0x64) | STR_OBJLOOK);
        if (str != 0)
            scroll_print(str);
    }
}

/* Describes a critter into s: 'a <attitude> <kind> named <Name>'. The attitude word is
   block 5 0x60 + attitude (hostile, upset, peaceful, friendly), shown only for whoami below
   0xF0 or 0xFF; the kind is its item name, or for a nameless kind with goal 11 a string
   from 0x115 by its animation sequence; the name is the NPC's conversation name (block 7,
   whoami + 0x10). A name that starts in lower case replaces the kind instead of following
   it. */
void far CritterLook(struct Object far *obj, char *s)
{
    char far *desc;
    char far *name;
    char far *attitude;
    unsigned char who;

    desc = get_string(OBJ_ITEM(obj) | STR_OBJNAMES);
    if (*desc == 0)
        desc = 0;
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
    str_cat(s, ".\n");
    scroll_print(s);
}

/* The second line of a look for books and scrolls (BookLook), keys (KeyLook), bones
   (BonesLook) and doors: a closed door whose owner bit 0 is set is spiked (string 0x83). */
void far SpecialLook(struct Object far *obj, register int print)
{
    register int minor;

    minor = OBJ_MINOR(obj);
    switch (OBJ_MAJOR(obj)) {
    case MAJOR_SPEC:
        if (minor == 3)
            BookLook(obj, print);
        else if (minor == 0)
            KeyLook(obj, print);
        break;
    case MAJOR_STUFF:
        if (minor == 0 && OBJ_ITEM(obj) >= ITEM_SKULL_C2 && OBJ_ITEM(obj) <= ITEM_PILE_OF_BONES_C6)
            BonesLook(obj, print);
        break;
    case MAJOR_RECT:
        if (minor == 0 && OBJ_INCLASS(obj) < 8 && (obj->ol.f.owner & 1))
            game_sprint(0x83);  /* 'The door is spiked.' */
        break;
    }
}

/* At lore 3, a talisman (an object whose COMOBJ fate is 10) is described as 'You see '
   (string 0x104) and its own text, 0x105 + which (0 to 8, by item: 0x136, 0x93, 0x97,
   0xBF, 0x11F, 0x37, 0xAE, 0x0A, 0x36). An item not in the list leaves which unset.
   Returns 1 when it described one, and LookAt then prints nothing else.
   Name: descriptive, chosen so its bssorder key (76) falls between do_mods (68) and
   CritterLook (123), where its stub entry is. */
char far talisman_desc(struct Object far *obj, struct ComObj *com)
{
    register int which;

    if (com->fate == 10) {
        switch (OBJ_ITEM(obj)) {
        case 0x136: which = 0; break;
        case 0x93: which = 1; break;
        case 0x97: which = 2; break;
        case 0xBF: which = 3; break;
        case 0x11F: which = 4; break;
        case 0x37: which = 5; break;
        case 0xAE: which = 6; break;
        case 0x0A: which = 7; break;
        case 0x36: which = 8; break;
        }
        game_sprint(0x104);
        game_sprint(which + 0x105);
        return 1;
    }
    return 0;
}
