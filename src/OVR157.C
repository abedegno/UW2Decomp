/* target: ovr157 */
/* opts: -mm -1 -G -O -Y -d */
/* Spells: studying a monster, enchanting and charging, true sight, summoning, detecting
   monsters, earthquakes, the class B spells and Altara's wand: the whole of DOS overlay
   ovr157, in original order. Function and global names are the originals from the FM
   Towns symbol table; the source file's own name is not known. */

#include <string.h>
#include <stdlib.h>

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0x21];
    unsigned char skills[20];           /* 0x21 */
    char pad35[0x39 - 0x35];
    unsigned char hunger;               /* 0x39 */
    unsigned char fatigue;              /* 0x3A */
    unsigned char food_heal;            /* 0x3B */
    unsigned char b3C;                  /* 0x3C */
    unsigned char level;                /* 0x3D */
    char pad3E[0x5E - 0x3E];
    unsigned char moonstone;            /* 0x5E, level the moonstone is on */
    char pad5F[0x60 - 0x5F];
    unsigned b60:1;                     /* 0x60 */
    unsigned poison:4;
    unsigned active_spells:4;
    unsigned b60_9:2;
    unsigned b60_11:1;
    unsigned shrooms:2;
    unsigned drunk:6;                   /* word 0x61, bits 6..11 */
    unsigned automap:1;                 /* word 0x62, bit 4 */
    unsigned b62_5:3;
    char pad63[0x72 - 0x63];
    unsigned long flags72;              /* 0x72 */
    char pad76[0x96 - 0x76];
    unsigned long quests[2];            /* 0x96 */
    char pad9E[0xE6 - 0x9E];
    unsigned char lines_cut;            /* 0xE6, Guardian lines cut, a bit per world */
    char padE7[0x305 - 0xE7];
    unsigned char b305;                 /* 0x305 */
};

/* The player's critter data, reached through the near pointer `playerdat`. */
struct Critter {
    char pad0[4];
    unsigned char max_vit;              /* 0x04 */
};

struct PlayerStats {
    char pad0[0x4A];
    unsigned weight;                    /* 0x4A, weight carried */
};

/* A mobile object. The first 8 bytes are shared with static objects. */
struct Object {
    unsigned id;                        /* item 0-8 (major class 6-8), is_quant 15 */
    unsigned pos;                       /* z 0-6, heading 7-9, y fine 10-12, x fine 13-15 */
    union {
        unsigned word;
        struct { unsigned quality:6, next:10; } f;
    } qn;
    union {
        unsigned word;
        struct { unsigned owner:6, link:10; } f;
    } ol;
    unsigned char hp;                   /* 0x08 */
    unsigned char b09;                  /* 0x09 */
    unsigned char b0A;                  /* 0x0A */
    unsigned goal_word;                 /* 0x0B, goal in bits 0-3 */
    unsigned attitude_word;             /* 0x0D, undead in bit 10, attitude in bits 14-15 */
    unsigned w0F;                       /* 0x0F */
    char pad11[0x12 - 0x11];
    unsigned char b12;                  /* 0x12 */
    unsigned char b13;                  /* 0x13 */
    unsigned char b14;                  /* 0x14 */
    unsigned char b15;                  /* 0x15 */
    unsigned home;                      /* 0x16, x in bits 10-15, y in bits 4-9 */
    unsigned char b18;                  /* 0x18, fine heading in bits 0-4 */
    unsigned char b19;                  /* 0x19 */
    unsigned char whoami;               /* 0x1A */
};

#define OBJ_Z(o)        ((o)->pos & 0x7F)
#define OBJ_HEADING(o)  (((o)->pos & 0x380) >> 7)
#define OBJ_XFINE(o)    (((o)->pos & 0xE000) >> 13)
#define OBJ_YFINE(o)    (((o)->pos & 0x1C00) >> 10)
#define SET_Z(o, v)     ((o)->pos = (o)->pos & 0xFF80 | (v) & 0x7F)
#define SET_XFINE(o, v) ((o)->pos = (o)->pos & 0x1FFF | ((unsigned)(v) & 7) << 13)
#define SET_YFINE(o, v) ((o)->pos = (o)->pos & 0xE3FF | ((unsigned)(v) & 7) << 10)
#define SET_HOMEX(o, v) ((o)->home = (o)->home & 0x3FF | ((v) & 0x3F) << 10)
#define SET_HOMEY(o, v) ((o)->home = (o)->home & 0xFC0F | ((v) & 0x3F) << 4)
#define OBJ_HOMEX(o)    (((o)->home & 0xFC00) >> 10)
#define OBJ_HOMEY(o)    (((o)->home & 0x3F0) >> 4)
#define OBJ_ISQUANT(o)  (((o)->id & 0x8000) >> 15)
#define OBJ_KIND(o)     (((o)->id & 0x30) >> 4)
/* An enchantment: link 0x200 plus the effect in the low bits. */
#define SET_ENCHANT(o, v) ((o)->ol.f.link = (o)->ol.f.link & 0x1F0 | (v) & 0xF | 0x200)

#define OBJ_ITEM(o)     ((o)->id & 0x1FF)
#define OBJ_MAJOR(o)    (((o)->id & 0x1C0) >> 6)
/* Two spellings of the critter index: sp_study_monster's bytes have no shift, mdetect's
   have a `shr ax,0`, so the original used both. */
#define OBJ_INDEX(o)    ((o)->id & 0x3F)
#define OBJ_TYPE(o)     (((o)->id & 0x3F) >> 0)
#define OBJ_UNDEAD(o)   (((o)->attitude_word & 0x400) >> 10)

/* One critter type's record, 0x30 bytes. */
struct Creature {
    unsigned char level;                /* 0x00 */
    char pad01[0x04 - 0x01];
    unsigned char avghit;               /* 0x04 */
    char pad05[0x09 - 0x05];
    unsigned char race;                 /* 0x09 */
    unsigned char b0A_0:1;              /* 0x0A */
    unsigned char b0A_1:1;
    unsigned char b0A_2:4;
    unsigned char b0A_6:1;
    unsigned char flier:1;
    char pad0B[0x0F - 0x0B];
    unsigned char b0F;                  /* 0x0F */
    char pad10[0x1D - 0x10];
    unsigned char stealth:4;            /* 0x1D */
    unsigned char b1D_4:4;
    char pad1E[0x2A - 0x1E];
    unsigned char spells[3];            /* 0x2A */
    unsigned char b2D_0:1;              /* 0x2D */
    unsigned char caster:7;
    unsigned char b2E;                  /* 0x2E */
    char pad2F[0x30 - 0x2F];
};

/* A rune spell: its class in the top five bits of the first byte. */
struct Spell {
    unsigned char cls;
    char pad1[2];
    unsigned char sub;
};

struct Tile {
    unsigned type:4;
    unsigned height:4;
    unsigned b8:2;
    unsigned floor:4;                   /* bits 10-13: floor texture */
    unsigned b14:2;
    unsigned objects;                   /* 0x02, head of the tile's object list */
};

#define SPELL_CLASS(s)  (((s).cls & 0xF8) >> 3)

extern struct Spell far spells[];
extern struct Creature Creature[];
extern struct Object far *ThePlayer;
extern struct Player near *player;
extern struct Object far *ActiveObj;
extern int PlayerLevel;
extern struct Object far *critdata;
extern unsigned char curBin;
extern struct Critter near *playerdat;
extern struct PlayerStats PlayerDat;
extern int GameInputMode;
extern int area_spell_state;
extern void (far *npp_func)(void);
extern unsigned char far *ActiveMob;
extern unsigned char far *LastActiveMob;

char dtypes[6] = { 3, 4, 8, 0x10, 0x20, 0x40 };
int demons[5] = { 0x4B, 0x4B, 0x5E, 0x64, 0x68 };

char far * far get_string(int id);
char far * far str_cat(char far *dst, char far *src);
unsigned char far check_res(struct Object far *obj, unsigned char damage, unsigned char type);
void far game_sprint(int id);
void far scroll_print(char far *s);
int far GetObjDesc(struct Object far *obj, int lore, char *s);
struct Tile far * far Map_GetAddr(int x, int y);
struct Object far * far build_new_obj(int item, struct Tile far *tile);
void far damage_square(int x, int y, unsigned char kind, unsigned char src);
int far Obj_MemTPtr(struct Object far *obj);
int far add_animobj(int index, int len, int a, char x, char y);
void far Obj_Free(struct Object far *obj);
void far Obj_Add(unsigned far *head, struct Object far *obj);
void far fireball_effect(struct Object far *obj, int x, int y);
struct Object far * far Obj_InList(unsigned far **head, int a, int major, int minor, int idx);
char far Obj_Rem(unsigned far *head, struct Object far *obj);
int far debris_type(int item, char type);
int far useNSpellCharges(struct Object far *obj, int n);
int far FindSlot(struct Object far *obj);
void far DamageInventory(int slot, int damage, int type, int a, int b);
void far FixPlayerEquips(void);
void far DisplayInventory(void);
unsigned char far decode_obj_spell(struct Object far *obj, int *major, int *effect, unsigned char *flag);
void far editchng(int bits);
struct Object far * far Obj_PtrTMem(unsigned far *link);
void far move_along(int heading, int dist, int *x, int *y);
unsigned char far can_place(int item, int a, int x, int y, int z, int b, char dist);
struct Object far * far CreateObj(int item, char mobile);
unsigned char far IsMobElem(struct Object far *obj);
void far creature_obj_init(void);
void far mob_init(struct Object far *obj, int x, int y);
struct Object far * far obj_deal(struct Object far *obj, int x, int y, char how);
void far print_path_to(char far *s, int x1, int y1, int z1, int x2, int y2, int z2, int dist);
int far mpos(char dx, char dy);
int far skill_check(int value, int target);
char far * far fix_name_string(char far *s, int article, int b);
unsigned char far put_at(int x, int y, int z, struct Object far *obj, int a, int b);
char far set_curmagic(char cls, char sub, char flags);
void far do_teleport(struct Object far *who, int x, int y, int level);
void far player_setup(int a, int b, int c);
void far set_drugged(int on);
void far get_hp_back(struct Object far *obj, char amount);
void far home_cam(int how);
void far attach_eye(int how);
int far rollem(int dice, int sides);
typedef char (far *SpellFn)(int x, int y, struct Object far *target, struct Tile far *tile,
                            unsigned char src);
void far gronk_area(struct Object far *who, char count, SpellFn fn, unsigned char type,
                    unsigned char dist, unsigned char radius);
void far set_effect(int which, int amount);
void far play_effect_on_mobile(int fx, struct Object far *obj, int vol);
void far do_mstone(void);
void far FreePlayerInv(unsigned far *list);
void far clearobj(int n);
void far clear_runes(void);
void far clear_shelf(void);
void far pretty_panelagain(void);
void far play_effect_here(int fx, int vol, int c);
void far put_effect(struct Object far *obj, int type, int size, int a, int b, int x, int y);
void far do_sfx(int which, int arg);
void far UseTrigger(struct Object far *who, struct Object far *obj, struct Object far *trigger, int how);
char far damage_item(struct Object far *obj, struct Object far *who, int x, int y,
                     unsigned char damage, unsigned char type);

char far study_monster_spells(struct Object far *obj, struct Creature *crit, char *str)
{
    int count = 0;
    unsigned char spl[8];
    int major;
    int i;
    int minor;

    if (crit->caster) {
        for (i = 0; i < 8; i++)
            spl[i] = 0xFF;
        for (i = 0; i < 3; i++)
            spl[i] = crit->spells[i];
        if (crit->race == 0x17) {
            i = 3;
            if (crit->flier == 1) {
                spl[i] = 0x39;
                i++;
            }
            if (crit->b2E > 0x2D) {
                spl[i] = 0x23;
                i++;
            }
            if (check_res(obj, 1, 8) == 0) {
                spl[i] = 0x1C;
                i++;
            }
            if (crit->caster >= 0x19 && crit->level >= 9) {
                spl[i] = 0x3B;
                i++;
            }
        }
        if (spl[0] == spl[1] || spl[1] == spl[2])
            spl[1] = 0xFF;
        if (spl[0] == spl[2])
            spl[2] = 0xFF;
        for (i = 0; i < 8; i++)
            if (spl[i] != 0xFF)
                count++;
        if (count) {
            for (i = 0; i < 8; i++) {
                major = (spl[i] & 0xC0) >> 6;
                minor = spl[i] & ~0xC0;
                if (spl[i] == 0xFF)
                    continue;
                if (major == 0 && SPELL_CLASS(spells[spl[i]]) == 4 && crit->race == 0x0F) {
                    major = 1;
                    minor = 6;
                }
                if (major > 0)
                    minor += (major + 0xC) << 4;
                else
                    minor += 0x100;
                str_cat(str, get_string(minor | 0xC00));
                if (count - i > 2)
                    strcat(str, ", ");
                if (count - i == 2)
                    str_cat(str, get_string(0x274));
            }
            str_cat(str, get_string(0x260));
        }
    }
    return count != 0;
}

char far sp_study_monster(struct Object far *caster, struct Object far *target)
{
    int whoami;
    unsigned char isnpc;
    unsigned char hasres;
    int flags;
    char str[0x80];
    int i;
    struct Creature *crit;

    isnpc = OBJ_MAJOR(target) == 1;
    hasres = 0;
    flags = 0;
    crit = &Creature[OBJ_INDEX(target)];
    if (!isnpc) {
        if (OBJ_ITEM(target) == 0x13)
            flags |= 2;
        else if (OBJ_ITEM(target) != 0x1CD)
            return 0;
    } else {
        if (OBJ_UNDEAD(target))
            flags |= 1;
        if (check_res(target, 1, 0x80) == 0)
            flags |= 2;
    }
    game_sprint(flags + 0x135);
    str[0] = 0;
    if (isnpc) {
        whoami = target->whoami;
        target->whoami = 0;
    }
    if (!isnpc)
        scroll_print("a ");
    GetObjDesc(target, 0, str);
    scroll_print(str);
    game_sprint(0x60);
    game_sprint(0x139);
    if (isnpc) {
        if (target->whoami == 100 && !((target->b0A & 0x70) >> 4))
            itoa(0x1E, str, 10);
        else
            itoa(target->hp, str, 10);
    } else
        itoa(3 | target->qn.f.quality, str, 10);
    scroll_print(str);
    game_sprint(0x60);
    if (!isnpc) {
        game_sprint(0x13D);
        str[0] = 0;
        str_cat(str, get_string(0x346));
        strcat(str, ", ");
        str_cat(str, get_string(0x347));
        scroll_print(str);
        game_sprint(0x60);
    } else {
        flags = 0;
        if (crit->b0F)
            game_sprint(0x13C);
        str[0] = 0;
        if (study_monster_spells(target, crit, str)) {
            game_sprint(0x144);
            scroll_print(str);
        }
        str[0] = 0;
        for (i = 0; i < 6; i++) {
            if (check_res(target, 1, dtypes[i]) == 0 && (dtypes[i] != 8 || crit->race != 0x17)) {
                if (hasres)
                    strcat(str, ", ");
                str_cat(str, get_string((i + 0x146) | 0x200));
                hasres = 1;
            }
        }
        if (crit->race == 0x17) {
            strcat(str, ", ");
            str_cat(str, get_string(0xD25));
        }
        if (hasres) {
            game_sprint(0x13D);
            scroll_print(str);
            game_sprint(0x60);
        }
        target->whoami = whoami;
    }
    return 1;
}

/* IDA's CanObjectBeEnchanted. FM Towns chargeable_ sits between sp_study_monster_ and
   sp_enchant_faildestroymess_ and has the same tests (7/0xE, 0xD/0xC..0xF, classes 0xB, 0xE,
   0x13). */
char far chargeable(struct Object far *obj, int major, int effect)
{
    int cls;

    cls = (obj->id & 0x1F0) >> 4;
    if (major == 7 && effect == 0xE)
        return 0;
    if (major == 0xD && effect >= 0xC && effect <= 0xF)
        return 0;
    switch (cls) {
    case 0xB:
        return 0;
    case 0xE:
        return 0;
    case 0x13:
        return 0;
    default:
        return 1;
    }
}

void far sp_enchant_faildestroymess(struct Object far *obj, int x, int y)
{
    struct Tile far *tile;
    struct Object far *boom;
    char str[0x80];

    tile = Map_GetAddr(x, y);
    boom = build_new_obj(0x1C2, tile);
    game_sprint(0x13E);
    GetObjDesc(obj, 0, str);
    scroll_print(str);
    game_sprint(0x13F);
    damage_square(x, y, 1, Obj_MemTPtr(ThePlayer));
    if (add_animobj(Obj_MemTPtr(boom), 4, 0, x, y) == -1)
        Obj_Free(boom);
    else {
        Obj_Add(&tile->objects, boom);
        fireball_effect(boom, x, y);
    }
}

char far charge_object(struct Object far *obj, int effect, int x, int y)
{
    char base;
    char diff;
    char q;
    unsigned far *head;
    struct Object far *spell;
    int chance;

    base = effect < 0x40 ? effect / 8 + 1 : 4;
    diff = (player->level + 1) / 2 - base + 8;
    spell = 0;
    head = &obj->ol.word;
    spell = Obj_InList(&head, 0, 4, 2, 0);
    q = spell->qn.f.quality / diff;
    chance = ((0x10 - diff + q) << 10) / (q + 0x18);
    if ((rand() & 0x3FF) < chance) {
        sp_enchant_faildestroymess(obj, x, y);
        if (Obj_Rem(head, spell))
            Obj_Free(spell);
        obj->id = obj->id & 0xFE00 | debris_type(OBJ_ITEM(obj), 0) & 0x1FF;
        return 1;
    } else {
        useNSpellCharges(obj, -1 - player->skills[9] / 15);
        return 0;
    }
}

void far sp_enchant_destroy(struct Object far *obj, char inv, int x, int y)
{
    sp_enchant_faildestroymess(obj, x, y);
    if (inv) {
        DamageInventory(FindSlot(obj), 0xFF, 0, 2, 1);
        FixPlayerEquips();
        DisplayInventory();
    } else
        damage_item(obj, ThePlayer, x, y, 0xFF, 0);
}

void far sp_enchant(struct Object far *obj, unsigned char inv, int x, int y)
{
    int major;
    int effect;
    unsigned char flag;
    char failed;
    unsigned char already;
    char attempt;
    char str[0x80];
    int diff;
    int skill;

    failed = 1;
    already = decode_obj_spell(obj, &major, &effect, &flag);
    if (inv) {
        x = OBJ_HOMEX(ThePlayer);
        y = OBJ_HOMEY(ThePlayer);
    }
    if (already) {
        if (flag && major == 0) {
            major = SPELL_CLASS(spells[effect]);
            effect = spells[effect].sub;
        }
        if (flag && chargeable(obj, major, effect)) {
            failed = charge_object(obj, effect, x, y);
            if (failed == 0)
                goto report;
            if (inv)
                DisplayInventory();
            else
                editchng(2);
            return;
        } else if (flag) {
            sp_enchant_destroy(obj, inv, x, y);
            return;
        } else if (!flag && major == 0xC) {
            attempt = 0;
            if (OBJ_MAJOR(obj) != 0)
                goto report;
            if (OBJ_KIND(obj) <= 1 && effect < 8 && (effect & ~4) < 3) {
                attempt = 1;
                diff = effect & ~4;
                skill = (player->level - 8) / 4 + player->skills[9] / 11;
            } else if (OBJ_KIND(obj) >= 2 && (effect & ~8) < 7) {
                attempt = 1;
                diff = effect & ~8;
                skill = player->level + player->skills[9] / 11 - 10;
            }
            if (attempt == 0)
                goto report;
            if (diff > skill) {
                sp_enchant_destroy(obj, inv, x, y);
                return;
            }
            SET_ENCHANT(obj, effect + 1);
            failed = 0;
            goto report;
        }
    }
    if (!already && OBJ_MAJOR(obj) == 0
        && (OBJ_ISQUANT(obj) || Obj_PtrTMem(&obj->ol.word) == 0)) {
        obj->ol.f.link = 0x201;
        obj->id = obj->id & 0xEFFF | 0x1000;
        obj->id = obj->id & 0xF7FF;
        obj->id = obj->id & 0x7FFF | 0x8000;
        switch (OBJ_KIND(obj)) {
        case 0:
        case 1:
            obj->ol.f.link = obj->ol.f.link & 0xF | 0x2C0;
            SET_ENCHANT(obj, (rand() % 2) << 2);
            failed = 0;
            break;
        case 2:
        case 3:
            obj->ol.f.link = obj->ol.f.link & 0xF | 0x2C0;
            SET_ENCHANT(obj, (rand() % 2) << 3);
            failed = 0;
            break;
        }
    }
    /* `report` is reached by goto from the charge and enchant paths: DOS jumps there
       directly, and the enchant path shares its final store with the switch below. */
report:
    if (failed)
        game_sprint(0x12C);
    else {
        game_sprint(0x12D);
        GetObjDesc(obj, 1, str);
        scroll_print(str);
        game_sprint(0x60);
    }
}

/* IDA's CanObjectBeRepaired. FM Towns mendable_ sits between sp_enchant_ and sp_true_sight_
   and makes the same tests, including the item-class comparison with 0x90 and 0x94 that can
   never be equal. The duplicated `return 0` arms are merged by the compiler into the jumps
   DOS shows (one is a `jmp $+2`). */
char far mendable(struct Object far *obj)
{
    switch (OBJ_MAJOR(obj)) {
    case 0:
        return 1;
    case 2:
        switch (OBJ_KIND(obj)) {
        case 1:
            return (obj->id & 0x1F0) >> 4 != 0x90 && (obj->id & 0x1F0) >> 4 != 0x94;
        case 3:
            return 1;
        default:
            return 0;
        }
    case 5:
        switch (OBJ_KIND(obj)) {
        case 0:
            return 1;
        default:
            return 0;
        }
    default:
        return 0;
    }
}

char far sp_true_sight(struct Object far *caster, struct Object far *target)
{
    unsigned far *head;
    struct Object far *trig;
    int search;

    if (target == 0)
        return 0;
    if (OBJ_ISQUANT(target) || target->ol.f.link == 0)
        return 0;
    if (OBJ_MAJOR(target) == 6)
        return 0;
    head = &target->ol.word;
    trig = Obj_InList(&head, 0, 6, 2, 3);
    if (trig) {
        search = player->skills[11];
        player->skills[11] = 0x2D;
        UseTrigger(ThePlayer, target, trig, 5);
        player->skills[11] = search;
        return 1;
    }
    return 0;
}

char far tremor_area(int x, int y, struct Object far *target, struct Tile far *tile,
                     unsigned char src);

void far creat_spell(struct Object far *caster, char which)
{
    struct Object far *obj;
    struct Object far *save;
    int heading;
    int x;
    int y;
    int homex;
    int homey;
    unsigned char lvl;
    int dist;
    struct Tile far *tile;
    int owner;
    int item;
    int z;

    dist = 9;
    heading = ((OBJ_HEADING(caster) << 5) + (caster->b18 & 0x1F) + (rand() % 0x1B - 0xD)) % 0xFF;
    x = (OBJ_HOMEX(caster) << 3) + OBJ_XFINE(caster);
    y = (OBJ_HOMEY(caster) << 3) + OBJ_YFINE(caster);
    move_along(heading, dist, &x, &y);
    homex = x >> 3;
    homey = y >> 3;
    tile = Map_GetAddr(homex, homey);
    z = tile->height << 3;
    switch (which) {
    case 2:
        item = 0x19E;
        z = OBJ_Z(caster) + 0xC;
        break;
    case 3:
        z = OBJ_Z(caster) + 0xC;
        item = 0x19F;
        break;
    case 1:
        item = rand() % 7 + 0xB0;
        break;
    case 5:
        item = demons[(player->skills[9] + rand() % 0x1E) / 12];
        break;
    case 4:
        lvl = caster == ThePlayer ? player->skills[9] : PlayerLevel << 2;
        if (lvl < 2)
            lvl = 2;
        do
            item = lvl + rand() % lvl + 0x40;
        while (Creature[item & ~0x1C0].avghit == 0 || Creature[item & ~0x1C0].b0A_1
               || item == 0x7B || item == 0x7C || Creature[item & ~0x1C0].b0A_6);
        break;
    case 6:
        item = 0x1E;
        break;
    }
    if (can_place(item, 0, x, y, z, 1, 8)) {
        obj = CreateObj(item, which >= 4);
        SET_XFINE(obj, x & 7);
        SET_YFINE(obj, y);
        if (!IsMobElem(obj))
            obj->qn.f.quality = 0x3F;
        if (which == 4 || which == 5) {
            save = ActiveObj;
            ActiveObj = obj;
            creature_obj_init();
            ActiveObj = save;
            SET_HOMEX(obj, homex);
            SET_HOMEY(obj, homey);
            if (Creature[item & ~0x1C0].flier)
                z = (z + 0x80) / 2;
            if (caster == ThePlayer && which == 4) {
                obj->b19 = obj->b19 & 0xBF | 0x40;
            } else {
                obj->attitude_word = obj->attitude_word & 0x3FFF;
                obj->b19 = obj->b19 & 0xFE | 1;
                obj->w0F = obj->w0F & 0xFFC0 | (OBJ_HOMEX(ThePlayer) & 0x3F) << 0;
                obj->w0F = obj->w0F & 0xF03F | (OBJ_HOMEY(ThePlayer) & 0x3F) << 6;
            }
        } else if (which == 6) {
            owner = 0;
            mob_init(obj, homex, homey);
            obj->b09 = heading + (rand() & 1) * 0x7F + 0x40;
            if (OBJ_MAJOR(caster) == 1) {
                if ((owner = Obj_MemTPtr(caster)) >= 0x100)
                    owner = 0;
            }
            obj->b12 = owner;
            obj->b15 = obj->b15 & 0x7F;
            obj->b0A = obj->b0A & 0x7F;
            z += 0x12;
            if (z > 0x78)
                z++;
            obj->w0F = z << 3;
            obj->b13 = obj->b13 & 0x80 | (rand() % 0xF + 0xF & 0x7F) << 0;
        } else
            obj->qn.f.quality = 0x3F;
        SET_Z(obj, z);
        Obj_Add(&tile->objects, obj);
        if (which < 4)
            obj_deal(obj, homex, homey, 1);
        editchng(2);
    } else if (caster == ThePlayer)
        game_sprint(0x125);
}

void far print_monster(unsigned char dir, unsigned char n)
{
    print_path_to(get_string((n > 1) + (n > 4) + 0x3F | 0x200), 0, 0, 0, 0, 0, 0, -(dir + 1));
}

void far mdetect(int dist, int skill)
{
    struct Object far *obj;
    unsigned char far *p;
    unsigned char counts[8];
    unsigned char px;
    unsigned char py;
    signed char xv;
    signed char yv;
    unsigned char i;
    unsigned char max;
    unsigned char best;
    unsigned char tries;
    unsigned char res;
    unsigned char cands[8];
    char far *str;

    memset(cands, 0, 8);
    memset(counts, 0, 8);
    px = OBJ_HOMEX(ThePlayer);
    py = OBJ_HOMEY(ThePlayer);
    for (p = ActiveMob; p < LastActiveMob; p++) {
        obj = &critdata[*p];
        if (OBJ_MAJOR(obj) != 1)
            continue;
        xv = OBJ_HOMEX(obj) - px;
        yv = OBJ_HOMEY(obj) - py;
        if (abs(xv) < dist && abs(yv) < dist) {
            if ((res = skill_check(skill, 0xF - Creature[OBJ_TYPE(obj)].stealth)) > 0)
                counts[mpos(xv, yv)]++;
            if (res == 2)
                cands[mpos(xv, yv)] = OBJ_TYPE(obj);
        }
    }
    for (max = i = 0; i < 8; i++)
        if (counts[i] > max)
            max = counts[i];
    if (max == 0) {
        game_sprint(0x42);
        return;
    }
    if (max >= 4)
        best = 3;
    else
        best = max;
    for (i = 0; i < 8; i++) {
        if (counts[i] == max) {
            print_monster(i, counts[i]);
            if (cands[i & 7]) {
                game_sprint(0x145);
                str = get_string(cands[i & 7] + 0x40 | 0x800);
                str = fix_name_string(str, 1, 0);
                scroll_print(str);
                scroll_print(" ");
                game_sprint((i & 7) + 0x28);
                game_sprint(0x60);
            }
            max = best;
            best = i;
            break;
        }
    }
    i = rand() & 7;
    for (tries = 0; tries < 8; tries++, i++) {
        if ((i & 7) != best && counts[i & 7] > max) {
            print_monster(i & 7, counts[i & 7]);
            return;
        }
    }
}

char far tremor_area(int x, int y, struct Object far *target, struct Tile far *tile,
                     unsigned char src)
{
    struct Object far *boulder;

    if ((PlayerLevel - 1) / 8 != 8)
        boulder = CreateObj(rand() % 3 + 0x154, 0);
    else {
        int floor = tile->floor;
        int items[5] = { 0xC2, 0xB9, 0xB6, 0x129, 0xA0 };

        if (floor < 5)
            boulder = CreateObj(items[floor], 0);
        else
            return 1;
        SET_Z(boulder, 0x6E);
    }
    if (put_at(x * 8 + 3, y * 8 + 3, 0x6E, boulder, 0, 0) && IsMobElem(boulder)) {
        boulder->b13 = boulder->b13 & 0x80 | ((rand() & 3) + 2 & 0x7F) << 0;
        boulder->b09 = rand() & 0xFF;
        boulder->b0A = boulder->b0A & 0xF0 | (curBin + (rand() & 3) & 0xF) << 0;
        boulder->b14 = boulder->b14 & 0xF8 | (rand() % 3 + 1 & 7) << 0;
    }
    return 1;
}

void far xt_spells(struct Object far *caster, char stab, char sub)
{
    int heading;
    int x;
    int y;
    int z;
    register int dist;
    register int h;

    if (caster == ThePlayer || sub == 9)
        switch (sub) {
        case 0:                         /* speed */
            set_curmagic(0xB, 2, stab);
            break;
        case 1:                         /* portal */
            heading = (OBJ_HEADING(caster) << 5) + (caster->b18 & 0x1F);
            for (dist = 2; dist <= 2; dist++) {
                x = OBJ_HOMEX(ThePlayer);
                y = OBJ_HOMEY(ThePlayer);
                z = OBJ_Z(ThePlayer) << 3;
                move_along(heading, dist, &x, &y);
                if ((h = Map_GetAddr(x, y)->height) - z <= 2
                    && can_place(OBJ_ITEM(ThePlayer), Obj_MemTPtr(ThePlayer), x * 8 + 4,
                                 y * 8 + 4, OBJ_Z(ThePlayer), 1, 8)) {
                    npp_func = 0;
                    do_teleport(ThePlayer, x, y, 0);
                    player_setup(0, 0, -1);
                    editchng(0x7FFE);
                    return;
                }
            }
            game_sprint(0x132);
            break;
        case 2:                         /* restoration */
            player->poison = 0;
            player->shrooms = 0;
            set_drugged(0);
            player->drunk = 0;
            player->b305 = 0;
            get_hp_back(ThePlayer, playerdat->max_vit);
            player->hunger = 0xFF;
            player->food_heal = 0;
            player->fatigue = 0;
            player->b3C = 0;
            game_sprint(0x133);
            break;
        case 3:                         /* locate */
            if (player->automap || (PlayerLevel - 1) / 8 == 8)
                game_sprint(0x142);
            else {
                game_sprint(0x12A);
                player->automap = 1;
            }
            break;
        case 6:                         /* cure poison */
            player->poison = 0;
            break;
        case 7:                         /* roaming sight */
            if ((PlayerLevel - 1) / 8 == 8)
                game_sprint(0x142);
            else {
                set_curmagic(0xB, 1, stab);
                home_cam(0);
                attach_eye(-1);
                GameInputMode += 8;
            }
            break;
        case 8:                         /* telekinesis */
            set_curmagic(0xB, 3, stab);
            break;
        case 9:                         /* tremor */
            gronk_area(caster, rollem(8, 3), tremor_area, 0x40, 5, 3);
            set_effect(0x40, 0x28);
            play_effect_on_mobile(0x12, caster, 0);
            break;
        case 10:                        /* gate travel */
            if (player->moonstone) {
                npp_func = do_mstone;
                area_spell_state = player->moonstone;
                do_teleport(ThePlayer, 0x3F, 0x3F, player->moonstone);
                player_setup(0, 0, -1);
                editchng(0x7FFE);
            } else
                game_sprint(0x121);
            break;
        case 11:                        /* freeze time */
            set_curmagic(0xB, 0, stab);
            break;
        case 12:                        /* armageddon */
            FreePlayerInv(&ThePlayer->ol.word);
            clearobj(0);
            clear_runes();
            clear_shelf();
            player->b60_11 = 1;
            PlayerDat.weight = 0;
            FixPlayerEquips();
            pretty_panelagain();
            break;
        case 13:                        /* dispel hunger */
            player->hunger = 0xC0;
            player->food_heal = 0;
            game_sprint(0x134);
            break;
        }
}

char far check_Guardian_magic_marker(int x, int y, struct Object far *obj, struct Tile far *tile,
                                     unsigned char src);

void far thump_your_magic_twanger_froggie(void)
{
    area_spell_state = 0;
    gronk_area(ThePlayer, 1, check_Guardian_magic_marker, 0x80, 0, 2);
    if (area_spell_state > 0) {
        play_effect_here(0x12, 0x40, 0x28);
        play_effect_here(0x2A, 0x40, 0x14);
    } else
        game_sprint(0x142);
}

char far check_Guardian_magic_marker(int x, int y, struct Object far *obj, struct Tile far *tile,
                                     unsigned char src)
{
    unsigned char cut;
    int bit;

    cut = 0;
    bit = 1 << ((PlayerLevel - 1) / 8 - 1);
    if (OBJ_ITEM(obj) != 0x35 || !((obj->id & 0x2000) >> 13) || !((obj->id & 0x4000) >> 14))
        return 0;
    obj->id = obj->id & 0xDFFF;
    if (player->lines_cut & bit)
        cut = 1;
    if (!cut)
        put_effect(obj, 7, 4, 0, 7, x, y);
    if (Obj_Rem(&tile->objects, obj))
        Obj_Free(obj);
    if (cut)
        return 1;
    if ((player->lines_cut |= bit) == 0xFF)
        player->flags72 = (player->flags72 & ~4L) + 4;
    if ((PlayerLevel - 1) / 8 == 3)
        player->quests[1] = (player->quests[1] & ~1L) + 1;
    area_spell_state = 1;
    do_sfx(4, 0xF);
    return 1;
}
