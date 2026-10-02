/* target: ovr157 */
/* opts: -mm -1 -G -O -Y -d */
/* More spells: studying a monster, enchanting and charging, true sight, summoning and
   creating, detecting monsters, earthquakes, the class 11 spells (SPELLC_XT) and the
   Guardian's magic markers: the whole of DOS overlay ovr157, in original order.

   What it does in the game: the handlers that SPELLS.C's do_spell, target_spells and
   obj_spells reach for the spells with more to them. sp_study_monster prints a critter's
   kind, health, spells and resistances; sp_enchant enchants a weapon or armour, raises an
   enchantment a tier or recharges a wand, at the risk of destroying it; creat_spell makes
   food, the two rune traps, a summoned critter or demon, or a magic satellite; mdetect
   reports critters around the player by direction; xt_spells is speed, portal, restoration,
   locate, cure poison, roaming sight, telekinesis, tremor, gate travel, freeze time,
   armageddon and dispel hunger; thump_your_magic_twanger_froggie finds and removes a
   Guardian magic marker, a line of power, near the player.

   Data owned: dtypes (the six damage type bits Study Monster lists, by name in strings
   0x146..0x14B: magic, physical attacks, fire, poison, cold, missiles) and demons (what
   Summon Demon makes, by skill).
   Function and global names are the originals from the FM Towns symbol table.
   Name: descriptive (more spells: sp_enchant, sp_true_sight). */

#include <string.h>
#include <stdlib.h>
#include "combat.h"
#include "critter.h"
#include "event.h"
#include "file.h"
#include "gfx.h"
#include "inv.h"
#include "map.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"

#define SPELL_CLASS(s)  (((s).cls & 0xF8) >> 3)

extern struct Spell far spells[];
extern unsigned char curBin;
extern struct Player PlayerDat;
extern void (far *npp_func)(void);
extern unsigned char far *ActiveMob;
extern unsigned char far *LastActiveMob;

char dtypes[6] = { 3, 4, 8, 0x10, 0x20, 0x40 };
int demons[5] = { ITEM_IMP, ITEM_IMP, ITEM_HORDLING, ITEM_DESPOILER, ITEM_DESTROYER };

void far scroll_print(char far *s);
int far add_animobj(int index, int len, int a, char x, char y);
int far useNSpellCharges(struct Object far *obj, int n);
unsigned char far decode_obj_spell(struct Object far *obj, int *major, int *effect, unsigned char *flag);
unsigned char far can_place(int item, int a, int x, int y, int z, int b, char dist);
struct Object far * far CreateObj(int item, char mobile);
struct Object far * far obj_deal(struct Object far *obj, int x, int y, char how);
int far mpos(char dx, char dy);
unsigned char far put_at(int x, int y, int z, struct Object far *obj, int a, int b);
char far set_curmagic(char cls, char sub, char flags);
void far do_teleport(struct Object far *who, int x, int y, int level);
void far set_drugged(int on);
typedef char (far *SpellFn)(int x, int y, struct Object far *target, struct Tile far *tile,
                            unsigned char src);
void far gronk_area(struct Object far *who, char count, SpellFn fn, unsigned char type,
                    unsigned char dist, unsigned char radius);
void far set_effect(int which, int amount);
void far play_effect_on_mobile(int fx, struct Object far *obj, int vol);
void far clearobj(int n);
void far play_effect_here(int fx, int vol, int c);
void far put_effect(struct Object far *obj, int type, int size, int a, int b, int x, int y);
void far UseTrigger(struct Object far *who, struct Object far *obj, struct Object far *trigger, int how);
char far damage_item(struct Object far *obj, struct Object far *who, int x, int y,
                     unsigned char damage, unsigned char type);

/* Appends the spells a casting critter can use to str ('X, Y and Z.'): its three
   Creature spells, plus for race 0x17 (liches, inferred) more by its abilities (flying,
   locks, fire, power). Duplicates are dropped; a race 0x0F critter's heal shows as another
   spell. Returns true when there were any. */
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
            if (crit->locks > 0x2D) {
                spl[i] = 0x23;
                i++;
            }
            if (check_res(obj, 1, 8) == 0) {
                spl[i] = 0x1C;
                i++;
            }
            if (crit->caster >= 0x19 && crit->armour[0] >= 9) {
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
                str_cat(str, get_string(minor | STR_SPELLS));
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

/* Study Monster on a critter, a floating skull or a wisp: 'This [powerful] [undead]
   creature is <name>.', its current vitality (hit points), whether it uses poisonous attacks,
   the spells it can use and the damage types it resists (race 0x17 also lists one more,
   string 0xD25). */
char far sp_study_monster(struct Object far *caster, struct Object far *target)
{
    int whoami;
    unsigned char isnpc;
    unsigned char hasres;
    int flags;
    char str[0x80];
    int i;
    struct Creature *crit;

    isnpc = OBJ_MAJOR(target) == MAJOR_CREATURE;
    hasres = 0;
    flags = 0;
    crit = &Creature[OBJ_INMAJOR_NOSHIFT(target)];
    if (!isnpc) {
        if (OBJ_ITEM(target) == ITEM_SKULL_13)
            flags |= 2;
        else if (OBJ_ITEM(target) != ITEM_WISP)
            return 0;
    } else {
        if (OBJ_POWERFUL(target))
            flags |= 1;
        if (check_res(target, 1, 0x80) == 0)
            flags |= 2;
    }
    game_sprint(flags + 0x135);  /* 'This creature is ', 'This powerful creature is ', ... undead ... */
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
    game_sprint(0x139);  /* 'Its current vitality is ' */
    if (isnpc) {
        if (target->whoami == 100 && !OBJ_TERRAIN(target))
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
            game_sprint(0x13C);  /* 'It uses poisonous attacks.' */
        str[0] = 0;
        if (study_monster_spells(target, crit, str)) {
            game_sprint(0x144);  /* 'It can use ' */
            scroll_print(str);
        }
        str[0] = 0;
        for (i = 0; i < 6; i++) {
            if (check_res(target, 1, dtypes[i]) == 0 && (dtypes[i] != 8 || crit->race != 0x17)) {
                if (hasres)
                    strcat(str, ", ");
                str_cat(str, get_string((i + 0x146) | STR_GAME));
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

/* Whether an object's spell can be recharged: not food, reagents or books, and not two
   particular spells (major 7 effect 0xE, major 0xD effects 0xC..0xF). */
/* name: IDA's CanObjectBeEnchanted. FM Towns chargeable_ sits between sp_study_monster_ and
   sp_enchant_faildestroymess_ and has the same tests (7/0xE, 0xD/0xC..0xF, classes 0xB, 0xE,
   0x13). */
char far chargeable(struct Object far *obj, int major, int effect)
{
    int cls;

    cls = OBJ_CLASS(obj);
    if (major == 7 && effect == 0xE)
        return 0;
    if (major == 0xD && effect >= 0xC && effect <= 0xF)
        return 0;
    switch (cls) {
    case CLASS_FOOD:
        return 0;
    case CLASS_REAG:
        return 0;
    case CLASS_BOOK:
        return 0;
    default:
        return 1;
    }
}

/* The enchant spell blows the object up: an explosion and fire damage on its square, and
   'Your attempt to enchant the <object> destroys it in a blaze of fire!'. */
void far sp_enchant_faildestroymess(struct Object far *obj, int x, int y)
{
    struct Tile far *tile;
    struct Object far *boom;
    char str[0x80];

    tile = Map_GetAddr(x, y);
    boom = build_new_obj(ITEM_EXPLOSION_1C2, tile);
    game_sprint(0x13E);  /* 'Your attempt to enchant the ' */
    GetObjDesc(obj, 0, str);
    scroll_print(str);
    game_sprint(0x13F);  /* ' destroys it in a blaze of fire!' */
    damage_square(x, y, 1, Obj_MemTPtr(ThePlayer));
    if (add_animobj(Obj_MemTPtr(boom), 4, 0, x, y) == -1)
        Obj_Free(boom);
    else {
        Obj_Add(&tile->objects, boom);
        fireball_effect(boom, x, y);
    }
}

/* Recharges an object's spell (its MAJOR_SPEC minor 2 spell object holds the charges in
   its quality). The spell's circle is effect / 8 + 1 (4 for effects 0x40 and up) and
   diff = (level + 1) / 2 - circle + 8. The object is destroyed, turning to debris, with
   chance ((16 - diff + q) * 1024) / (q + 24) in 1024, where q = charges / diff: more
   charges and a weaker caster make it likelier. Otherwise it gains 1 + Casting / 15 charges.
   Returns 1 when destroyed. */
char far charge_object(struct Object far *obj, int effect, int x, int y)
{
    char base;
    char diff;
    char q;
    union Link far *head;
    struct Object far *spell;
    int chance;

    base = effect < 0x40 ? effect / 8 + 1 : 4;
    diff = (player->level + 1) / 2 - base + 8;
    spell = 0;
    head = &obj->ol.link;
    spell = Obj_InList(&head, 0, MAJOR_SPEC, 2, 0);
    q = spell->qn.f.quality / diff;
    chance = ((0x10 - diff + q) << 10) / (q + 0x18);
    if ((rand() & 0x3FF) < chance) {
        sp_enchant_faildestroymess(obj, x, y);
        if (Obj_Rem(head, spell))
            Obj_Free(spell);
        obj->id = obj->id & 0xFE00 | debris_type(OBJ_ITEM(obj), 0) & ID_ITEM;
        return 1;
    } else {
        useNSpellCharges(obj, -1 - player->skills[SKILL_CASTING] / 15);
        return 0;
    }
}

/* Destroys an object a failed enchantment has blown up, in the inventory or on the map. */
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

/* The Enchantment spell on obj (inv: it is in the inventory). An object with spell
   charges is recharged (charge_object) or, if not chargeable, destroyed. A weapon or armour
   enchanted with major 0xC gets the next tier: for weapons the damage and accuracy tiers
   0..2 (effects 0..2 and 4..6), allowed while tier <= (level - 8) / 4 + Casting / 11; for
   armour the tiers 0..6 of effects 0..6 and 8..14, allowed while tier <= level + Casting /
   11 - 10; past the limit the object is destroyed. An unenchanted weapon or armour with
   no contents gets the lowest tier of a random one of its two kinds. Reports 'You cannot
   enchant that.' or 'You have enchanted the <object>.'. */
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
            if (OBJ_MAJOR(obj) != MAJOR_HACK)
                goto report;
            if (OBJ_MINOR(obj) <= 1 && effect < 8 && (effect & ~4) < 3) {
                attempt = 1;
                diff = effect & ~4;
                skill = (player->level - 8) / 4 + player->skills[SKILL_CASTING] / 11;
            } else if (OBJ_MINOR(obj) >= 2 && (effect & ~8) < 7) {
                attempt = 1;
                diff = effect & ~8;
                skill = player->level + player->skills[SKILL_CASTING] / 11 - 10;
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
    if (!already && OBJ_MAJOR(obj) == MAJOR_HACK
        && (OBJ_ISQUANT(obj) || Obj_PtrTMem(&obj->ol.link) == 0)) {
        obj->ol.f.link = 0x201;
        obj->id = obj->id & 0xEFFF | ID_ENCHANT;
        obj->id = obj->id & 0xF7FF;
        obj->id = obj->id & 0x7FFF | ID_ISQUANT;
        switch (OBJ_MINOR(obj)) {
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
    /* match: `report` is reached by goto from the charge and enchant paths: DOS jumps there
       directly, and the enchant path shares its final store with the switch below. */
report:
    if (failed)
        game_sprint(0x12C);  /* 'You cannot enchant that.' */
    else {
        game_sprint(0x12D);  /* 'You have enchanted the ' */
        GetObjDesc(obj, 1, str);
        scroll_print(str);
        game_sprint(0x60);
    }
}

/* Whether Mending can repair an object: weapons and armour (MAJOR_HACK), MAJOR_MISC
   minor classes 1 and 3, and MAJOR_RECT minor class 0. */
/* name: IDA's CanObjectBeRepaired. FM Towns mendable_ sits between sp_enchant_ and
   sp_true_sight_ and makes the same tests, including the item-class comparison with 0x90
   and 0x94 that can never be equal. */
/* match: the duplicated `return 0` arms are merged by the compiler into the jumps DOS
   shows (one is a `jmp $+2`). */
char far mendable(struct Object far *obj)
{
    switch (OBJ_MAJOR(obj)) {
    case MAJOR_HACK:
        return 1;
    case MAJOR_MISC:
        switch (OBJ_MINOR(obj)) {
        case 1:
            return OBJ_CLASS(obj) != 0x90 && OBJ_CLASS(obj) != 0x94;
        case 3:
            return 1;
        default:
            return 0;
        }
    case MAJOR_RECT:
        switch (OBJ_MINOR(obj)) {
        case 0:
            return 1;
        default:
            return 0;
        }
    default:
        return 0;
    }
}

/* Reveal, one object: a container or object with a hidden trap trigger (MAJOR_TRAP minor 2
   class 3) has it set off with the player's Search skill raised to 45 for the call, so
   the hidden thing shows itself (inferred from UseTrigger's how 5). */
char far sp_true_sight(struct Object far *caster, struct Object far *target)
{
    union Link far *head;
    struct Object far *trig;
    int search;

    if (target == 0)
        return 0;
    if (OBJ_ISQUANT(target) || target->ol.f.link == 0)
        return 0;
    if (OBJ_MAJOR(target) == MAJOR_TRAP)
        return 0;
    head = &target->ol.link;
    trig = Obj_InList(&head, 0, MAJOR_TRAP, 2, 3);
    if (trig) {
        search = player->skills[SKILL_SEARCH];
        player->skills[SKILL_SEARCH] = 0x2D;
        UseTrigger(ThePlayer, target, trig, 5);
        player->skills[SKILL_SEARCH] = search;
        return 1;
    }
    return 0;
}

/* Creates something about a square in front of the caster, a little to either side at
   random: which 1 food (one of seven), 2 a rune of flame, 3 a rune of stasis, 4 Summon
   Monster (a random creature number from lvl to 2 * lvl - 1, lvl being the player's Casting
   skill or a critter caster's dungeon level * 4, at least 2; rerolled for swimmers, for
   creatures with no hit points or Creature bit bA_1, and for items 0x7B and 0x7C), 5 Summon
   Demon (demons[] by (Casting + 0..29) / 12), 6 a magic satellite that circles. A monster
   the player summons is set friendly (b19 bit 6, inferred); one a critter summons is
   hostile and heads for the player. With no room the player reads string 0x125. */
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
    heading = ((OBJ_HEADING(caster) << 5) + OBJ_FINEHEAD(caster) + (rand() % 0x1B - 0xD)) % 0xFF;
    x = (OBJ_HOMEX(caster) << 3) + OBJ_FINEX(caster);
    y = (OBJ_HOMEY(caster) << 3) + OBJ_FINEY(caster);
    move_along(heading, dist, &x, &y);
    homex = x >> 3;
    homey = y >> 3;
    tile = Map_GetAddr(homex, homey);
    z = tile->height << 3;
    switch (which) {
    case 2:
        item = ITEM_FLAM_RUNE;
        z = OBJ_Z(caster) + 0xC;
        break;
    case 3:
        z = OBJ_Z(caster) + 0xC;
        item = ITEM_TYM_RUNE;
        break;
    case 1:
        item = rand() % 7 + FIRST_FOOD;
        break;
    case 5:
        item = demons[(player->skills[SKILL_CASTING] + rand() % 0x1E) / 12];
        break;
    case 4:
        lvl = caster == ThePlayer ? player->skills[SKILL_CASTING] : PlayerLevel << 2;
        if (lvl < 2)
            lvl = 2;
        do
            item = lvl + rand() % lvl + FIRST_CREATURE;
        while (Creature[item & ~0x1C0].avghit == 0 || Creature[item & ~0x1C0].bA_1
               || item == 0x7B || item == 0x7C || Creature[item & ~0x1C0].swims);
        break;
    case 6:
        item = ITEM_SATELLITE;
        break;
    }
    if (can_place(item, 0, x, y, z, 1, 8)) {
        obj = CreateObj(item, which >= 4);
        SET_FINEX_UNSIGNED(obj, x & 7);
        SET_FINEY(obj, y);
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
                obj->b0F = obj->b0F & 0xFFC0 | (OBJ_HOMEX(ThePlayer) & 0x3F) << 0;
                obj->b0F = obj->b0F & 0xF03F | (OBJ_HOMEY(ThePlayer) & 0x3F) << 6;
            }
        } else if (which == 6) {
            owner = 0;
            mob_init(obj, homex, homey);
            obj->heading = heading + (rand() & 1) * 0x7F + 0x40;
            if (OBJ_MAJOR(caster) == MAJOR_CREATURE) {
                if ((owner = Obj_MemTPtr(caster)) >= NUM_MOBILE)
                    owner = 0;
            }
            obj->last_hit = owner;
            obj->b15 = obj->b15 & 0x7F;
            obj->b0A = obj->b0A & 0x7F;
            z += 0x12;
            if (z > 0x78)
                z++;
            obj->b0F = z << 3;
            obj->b13 = obj->b13 & 0x80 | (rand() % 0xF + 0xF & 0x7F) << 0;
        } else
            obj->qn.f.quality = 0x3F;
        SET_Z(obj, z);
        Obj_Add(&tile->objects, obj);
        if (which < 4)
            obj_deal(obj, homex, homey, 1);
        editchng(2);
    } else if (caster == ThePlayer)
        game_sprint(0x125);  /* 'There is no room to create that.' */
}

/* Prints 'You detect a creature / a few creatures / the activity of many creatures' (one,
   2..4, 5 and more) followed by the direction dir. */
void far print_monster(unsigned char dir, unsigned char n)
{
    print_path_to(get_string((n > 1) + (n > 4) + 0x3F | STR_GAME), 0, 0, 0, 0, 0, 0, -(dir + 1));
}

/* Detect Monster: for active critters within dist squares, a skill check against
   15 - the critter's noise counts it in one of eight directions, and a best result also
   names it. The direction with most is reported, with a creature's name when known, and
   one other direction with more than a few; 'You detect no monster activity.' when none. */
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
        if (OBJ_MAJOR(obj) != MAJOR_CREATURE)
            continue;
        xv = OBJ_HOMEX(obj) - px;
        yv = OBJ_HOMEY(obj) - py;
        if (abs(xv) < dist && abs(yv) < dist) {
            if ((res = skill_check(skill, 0xF - Creature[OBJ_INMAJOR(obj)].noise)) > 0)
                counts[mpos(xv, yv)]++;
            if (res == 2)
                cands[mpos(xv, yv)] = OBJ_INMAJOR(obj);
        }
    }
    for (max = i = 0; i < 8; i++)
        if (counts[i] > max)
            max = counts[i];
    if (max == 0) {
        game_sprint(0x42);  /* 'You detect no monster activity.' */
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
                game_sprint(0x145);  /* 'You detect ' */
                str = get_string(cands[i & 7] + FIRST_CREATURE | STR_OBJNAMES);
                str = fix_name_string(str, 1, 0);
                scroll_print(str);
                scroll_print(" ");
                game_sprint((i & 7) + 0x28);  /* 'to the North', 'to the Northeast', ... */
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

/* Tremor, one square: drops a boulder (one of three sizes) from high up, which falls and
   can hit what is below. In the Ethereal Void (world 8) it drops instead the thing of that
   floor's texture: a skull, a mushroom, a fish, a resilient sphere or a coin. */
char far tremor_area(int x, int y, struct Object far *target, struct Tile far *tile,
                     unsigned char src)
{
    struct Object far *boulder;

    if ((PlayerLevel - 1) / LEVELS_PER_WORLD != 8)
        boulder = CreateObj(rand() % 3 + ITEM_LARGE_BOULDER_154, 0);
    else {
        int floor = tile->floor;
        int items[5] = { ITEM_SKULL_C2, ITEM_MUSHROOM, ITEM_FISH, ITEM_RESILIENT_SPHERE_129, ITEM_COIN };

        if (floor < 5)
            boulder = CreateObj(items[floor], 0);
        else
            return 1;
        SET_Z(boulder, 0x6E);
    }
    if (put_at(x * 8 + 3, y * 8 + 3, 0x6E, boulder, 0, 0) && IsMobElem(boulder)) {
        boulder->b13 = boulder->b13 & 0x80 | ((rand() & 3) + 2 & 0x7F) << 0;
        boulder->heading = rand() & 0xFF;
        boulder->b0A = boulder->b0A & 0xF0 | (curBin + (rand() & 3) & 0xF) << 0;
        boulder->b14 = boulder->b14 & 0xF8 | (rand() % 3 + 1 & 7) << 0;
    }
    return 1;
}

/* Class 11 spells (SPELLC_XT), the player's only except tremor. stab is the active
   spell flag passed on to set_curmagic. Portal moves the player two squares ahead if the
   floor there is not much higher and there is room. Restoration cures everything: poison,
   hallucination, drugs, drink, paralysis, hunger and fatigue, and full health. Locate sets
   player->automap ('Your position is revealed unto you.'). Roaming Sight detaches the camera
   (GameInputMode + 8). Locate and Roaming Sight do nothing in the Ethereal Void. Gate
   travel here takes the player to the level of the first moonstone. Armageddon destroys the
   player's inventory and every object on the level, empties the rune bag and shelf, and sets
   player->b60_11. */
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
            heading = (OBJ_HEADING(caster) << 5) + OBJ_FINEHEAD(caster);
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
            game_sprint(0x132);  /* 'There is no suitable space.' */
            break;
        case 2:                         /* restoration */
            player->poison = 0;
            player->shrooms = 0;
            set_drugged(0);
            player->drunk = 0;
            player->paralyzed = 0;
            get_hp_back(ThePlayer, playerdat->avghit);
            player->hunger = 0xFF;
            player->food_heal = 0;
            player->fatigue = 0;
            player->b3C = 0;
            game_sprint(0x133);  /* 'You feel much better.' */
            break;
        case 3:                         /* locate */
            if (player->automap || (PlayerLevel - 1) / LEVELS_PER_WORLD == 8)
                game_sprint(0x142);
            else {
                game_sprint(0x12A);  /* 'Your position is revealed unto you.' */
                player->automap = 1;
            }
            break;
        case 6:                         /* cure poison */
            player->poison = 0;
            break;
        case 7:                         /* roaming sight */
            if ((PlayerLevel - 1) / LEVELS_PER_WORLD == 8)
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
            if (player->moonstones[0]) {
                npp_func = do_mstone;
                area_spell_state = player->moonstones[0];
                do_teleport(ThePlayer, 0x3F, 0x3F, player->moonstones[0]);
                player_setup(0, 0, -1);
                editchng(0x7FFE);
            } else
                game_sprint(0x121);
            break;
        case 11:                        /* freeze time */
            set_curmagic(0xB, 0, stab);
            break;
        case 12:                        /* armageddon */
            FreePlayerInv(&ThePlayer->ol.link);
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
            game_sprint(0x134);  /* 'You feel well fed.' */
            break;
        }
}

/* Special spell 0: looks for a Guardian magic marker within two squares of the player
   and cuts it (check_Guardian_magic_marker), with sounds; otherwise 'The spell has no
   noticeable effect.'. */
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

/* Per-object callback of thump_your_magic_twanger_froggie: an invisible Guardian signet
   ring with id bit 13 set is a magic marker, a line of power. It is removed; if this world's
   line was not yet cut, its bit in QB_LINES_OF_POWER is set (bit world - 1), when all eight
   are cut quest 3 gets bit 2, in the ice caverns (world 3) quest 13 gets bit 0, and a sound
   plays. */
char far check_Guardian_magic_marker(int x, int y, struct Object far *obj, struct Tile far *tile,
                                     unsigned char src)
{
    unsigned char cut;
    int bit;

    cut = 0;
    bit = 1 << ((PlayerLevel - 1) / LEVELS_PER_WORLD - 1);
    if (OBJ_ITEM(obj) != ITEM_GUARDIAN_SIGNET_RING || !OBJ_DOORDIR(obj) || !OBJ_INVIS(obj))
        return 0;
    obj->id = obj->id & 0xDFFF;
    if (player->quest_bytes[QB_LINES_OF_POWER] & bit)
        cut = 1;
    if (!cut)
        put_effect(obj, 7, 4, 0, 7, x, y);
    if (Obj_Rem(&tile->objects, obj))
        Obj_Free(obj);
    if (cut)
        return 1;
    if ((player->quest_bytes[QB_LINES_OF_POWER] |= bit) == 0xFF)
        player->quests[3] = (player->quests[3] & ~4L) + 4;
    if ((PlayerLevel - 1) / LEVELS_PER_WORLD == 3)
        player->quests[13] = (player->quests[13] & ~1L) + 1;
    area_spell_state = 1;
    do_sfx(4, 0xF);
    return 1;
}
