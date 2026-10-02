/* target: ovr156 */
/* opts: -mm -1 -G -O -Y -d */
/* Casting spells: mana and health, missiles, area spells, spells on a target, and the
   damage an area spell does to a tile: the whole of DOS overlay ovr156, in original order.

   What it does in the game: do_spell is the one dispatcher for every spell, whoever casts
   it: the player from runes (RUNES.C's player_cast), wands, scrolls and enchanted objects
   (cast, which takes a spells[] index or a packed class and minor), critters, traps and
   conversations. It refuses on an anti-magic square and sends the spell by class
   (combat.h's SPELLC_*): classes 0..3 become a timed active spell (set_curmagic, SPELLS2.C),
   4 heals, 5 fires a missile (for the player, after aiming: GameInputMode 3), 6 hits an area
   in front of the caster, 7 waits for the player to click a target (target_spells for
   critters, obj_spells for objects), 8 creates things, 9 is a backfire, 10 restores mana,
   11 is the extra spells of SPELLS2.C, 13 the special spells and 14 plays a cutscene.

   Data owned: spells[], the 69 spells with their rune words, class and minor (the order is
   the order of the spell names in string block 6 from 256, eight spells to a circle;
   the last five have no runes and are cast only by objects); area_spells and area1_spells,
   the per-square handlers of area and targeted spells; area_spell_state, scratch shared by
   one area spell's calls; mspell_mused, the mana a targeted spell will cost; and
   inanmMapX, inanmMapY, the square of an object that casts.
   Function and global names are the originals from the FM Towns symbol table.
   Name: descriptive (casting spells: cast, do_spell). */

#include <stdlib.h>
#include "combat.h"
#include "critter.h"
#include "event.h"
#include "gfx.h"
#include "map.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"

#define SPELL_CLASS(s)  (((s).cls & 0xF8) >> 3)

/* This file's _BSS, DS:8638..8639: the map square of an object (not a critter) that casts. */
/* match: after ovr151's SCD pointer (key 363) in a new run (41),
   before ovr163's LootCreature; of the files between, only this one uses them. */
unsigned char inanmMapX, inanmMapY;

int area_spell_state = 0;
unsigned char mspell_mused = 0;

void far spend_mana(int cost)
{
    if (cost > 0)
        player->play_mana = player->play_mana > cost ? player->play_mana - cost : 0;
}

/* True on a square whose tile has bit 0 of its door byte set: no magic works there. */
unsigned char far anti_magic_p(int x, int y)
{
    return Map_GetAddr(x, y)->door & 1;
}

/* Casts spell by number: 0..0x3F index spells[]; with bit 6 or 7 set it is packed, class
   12 + (spell >> 6) and minor spell & 0x3F (so classes 13 and 14, special spells and
   cutscenes, are reachable from a single byte). */
void far cast(unsigned char spell, struct Object far *who, struct Object far *target)
{
    char cls;
    char sub;

    if (spell & 0xC0) {
        cls = ((spell & 0xC0) >> 6) + 12;
        sub = spell & 0x3F;
        do_spell(cls, sub, who, target);
    } else
        do_spell(SPELL_CLASS(spells[spell]), spells[spell].sub, who, target);
}

/* The spell dispatcher (see the file comment). An object caster (who at or above
   objdata) is tested for anti-magic at inanmMapX, inanmMapY, a critter at its own square.
   Bits 6 and 7 of sub are flags for the active spell classes and the area spell's target
   mode. Two side effects: Levitate and Fly (class 1, minors 3 and 5) give a little upward
   bounce, and Iron Flesh (class 2, minor 5) cast while the djinn quest stands at
   xclock[XC_DJINN] 4 glazes the baked mud on the player and moves it on to 5 (string
   0x14F). Returns 0 when the spell did not take: anti-magic, an active spell the player
   could not start, or a heal with no target. */
char far do_spell(unsigned char cls, unsigned char sub, struct Object far *who,
                  struct Object far *target)
{
    if (who >= (struct Object far *)objdata && cls <= 11) {
        if (anti_magic_p(inanmMapX, inanmMapY))
            return 0;
    } else if (anti_magic_p(OBJ_HOMEX(who), OBJ_HOMEY(who)))
        return 0;
    switch (cls) {
    case 1:
        if ((sub & ~0xC0) == 3 || (sub & ~0xC0) == 5)
            phys_bounce_up(who);
    case 0:
    case 2:
        if (who == ThePlayer && (sub & ~0xC0) == 5 && cls == 2 && player->xclock[XC_DJINN] == 4) {
            game_sprint(0x14F);  /* 'The baked mud hardens into a clear glaze.' */
            player->xclock[XC_DJINN] = 5;
        }
    case 3:
        if (who == ThePlayer && set_curmagic(cls, sub & 0x3F, sub & 0xC0))
            break;
        return 0;
    case 4:
        if (who) {
            healing(who, sub);
            if (who == ThePlayer)
                game_sprint(0x124);  /* 'You are healed.' */
            break;
        }
        return 0;
    case 5:
        if (who == ThePlayer) {
            GameInputMode = 3;
            ObjectActing = ThePlayer;
            ObjectActorArg = sub;
            force_mouse_cursor(0x1075);
        } else
            release_missile(who, sub);
        break;
    case 6:
        nail_area(who, sub);
        break;
    case 7:
        nail_1area(who, sub);
        break;
    case 8:
        creat_spell(who, sub);
        break;
    case 9:
        backfire(who, sub);
        break;
    case 10:
        restore_mana(who, sub);
        break;
    case 11:
        xt_spells(who, sub & 0xC0, sub & 0x3F);
        break;
    case 14:
        show_cutscene(sub);
        update_animobj(4);
        break;
    case 13:
        special_spells(who, target, sub);
        break;
    }
    return 1;
}

/* Restores the player's mana: amount > 0 gives max_mana * (amount + 0..3) / 16 + 1, a
   negative amount adds -amount. In world 5 (the Scintillus Academy, levels 41..48;
   Guide, "The Worlds and Level Concept") nothing happens, except on its first level and on
   its eighth level at x >= 25. Capped at max_mana. */
void far restore_mana(struct Object far *who, char amount)
{
    register int gain;

    if (who == ThePlayer) {
        if ((PlayerLevel - 1) / LEVELS_PER_WORLD != 5 || (PlayerLevel - 1) % LEVELS_PER_WORLD + 1 <= 1
            || (PlayerLevel - 1) % LEVELS_PER_WORLD + 1 >= 8 && OBJ_HOMEX(ThePlayer) >= 25) {
            if (amount > 0) {
                gain = player->max_mana * (amount + (rand() & 3));
                player->play_mana += (gain >> 4) + 1;
            } else
                player->play_mana -= amount;
            if (player->play_mana > player->max_mana)
                player->play_mana = player->max_mana;
            panel_check_hpmp();
        }
    }
}

/* Heals the player like restore_mana: amount > 0 gives avghit * (amount + 0..3) / 16 + 1,
   a negative amount adds -amount, capped at the player's maximum (playerdat->avghit). */
void far restore_hp(struct Object far *who, char amount)
{
    int hp;

    if (who == ThePlayer) {
        if (amount > 0) {
            hp = playerdat->avghit * (amount + (rand() & 3));
            hp >>= 4;
            hp += ThePlayer->hp + 1;
        } else
            hp = ThePlayer->hp - amount;
        if (hp > playerdat->avghit)
            ThePlayer->hp = playerdat->avghit;
        else
            ThePlayer->hp = hp;
        panel_check_hpmp();
    }
}

/* Adds amount hit points to a critter, capped at its Creature avghit (the player's
   record too). Also the life stealing weapon's power (COMBAT.C). */
void far get_hp_back(struct Object far *who, unsigned char amount)
{
    who->hp = who->hp + amount > Creature[OBJ_INMAJOR(who)].avghit
        ? Creature[OBJ_INMAJOR(who)].avghit : who->hp + amount;
    if (who == ThePlayer)
        panel_check_hpmp();
}

/* The heal spells (class 4): minor d8 hit points (Lesser Heal 2d8, Heal 4d8, ...), and
   minor 15 (Greater Heal) all of them. */
void far healing(struct Object far *who, char sub)
{
    unsigned char amount;

    if (OBJ_MAJOR(who) == MAJOR_CREATURE) {
        if (sub == 15)
            amount = 0xFF;
        else
            amount = rollem(sub, 8);
        get_hp_back(who, amount);
    }
}

/* A backfired spell (class 9): (sub)d8 damage to the caster, but never below 3 hit
   points. The player sees the screen flash, or reads string 0x16A when not in the 3D view. */
void far backfire(struct Object far *who, char sub)
{
    char damage;

    if (OBJ_MAJOR(who) != MAJOR_CREATURE)
        return;
    damage = rollem(sub, 8);
    if (who->hp <= 3)
        return;
    if (who->hp - damage <= 3)
        who->hp = 3;
    else
        who->hp = who->hp - damage;
    if (who == ThePlayer) {
        if (inplist->mode == 1)
            fill_FB(0x30);
        else
            game_sprint(0x16A);  /* 'The cursed object causes you much pain.' */
    }
}

/* Fires missile spell sub: 1 magic arrow, 2 lightning, 3 fireball, 4 acid, 5 deadly
   seeker (homing dart), 6 snowball (items 0x10 + items[sub - 1]). The player pays the
   mana held in mspell_mused only if it was launched. */
void far release_missile(struct Object far *who, char sub)
{
    unsigned char items[6] = { 7, 5, 4, 6, 11, 12 };
    unsigned char ok;

    ok = spell_fire(who, items[sub - 1]);
    if (who == ThePlayer) {
        if (!ok)
            game_sprint(0x10F);  /* 'There is not enough room to release that spell.' */
        else
            spend_mana(mspell_mused);
        mspell_mused = 0;
    }
}

/* Creates an effect object of the given item for a tile, at a random height between the
   floor and the ceiling. */
struct Object far * far build_new_obj(int item, struct Tile far *tile)
{
    struct Object far *obj;
    int z;

    obj = CreateObj(item, 0);
    z = tile->height * 8;
    if (z < 0x80)
        z += rand() % (0x80 - z);
    SET_Z(obj, z);
    return obj;
}

/* Sheet Lightning, one square: a lightning effect and damage_square kind 2. */
char far sp_sheet_light(int x, int y, struct Object far *target, struct Tile far *tile,
                        unsigned char src)
{
    struct Object far *obj;

    obj = build_new_obj(ITEM_LIGHTNING_1C5, tile);
    damage_square(x, y, 2, src);
    if (add_animobj(Obj_MemTPtr(obj), 4, rand() % 4, x, y) == -1)
        Obj_Free(obj);
    else
        Obj_Add(&tile->objects, obj);
    return 1;
}

/* Flame Wind, one square: an explosion and damage_square kind 1, at most five on
   critters and never twice in a row on one square; an empty square is hit one time in
   three. area_spell_state holds the last square and the count. */
char far sp_meteor(int x, int y, struct Object far *target, struct Tile far *tile,
                   unsigned char src)
{
    struct Object far *obj;
    int last;
    unsigned char npc;
    int square;
    int count;

    square = (x << 6) + y;
    last = area_spell_state >> 4;
    count = area_spell_state & 0xF;
    npc = 0;
    if (target && OBJ_MAJOR(target) == MAJOR_CREATURE)
        npc = 1;
    if (last == square || count >= 5)
        return 0;
    if (!npc && rand() % 3)
        return 0;
    obj = build_new_obj(ITEM_EXPLOSION_1C2, tile);
    damage_square(x, y, 1, src);
    if (add_animobj(Obj_MemTPtr(obj), 4, 0, x, y) == -1)
        Obj_Free(obj);
    else {
        Obj_Add(&tile->objects, obj);
        fireball_effect(obj, x, y);
    }
    if (npc)
        count++;
    area_spell_state = square << 4 | count & 0xF;
    return 1;
}

/* Smite Undead (and the undead slaying weapon): a floating skull is destroyed outright; a
   critter whose resist byte has bit 0x80 (check_res returns 0: the undead, inferred) takes
   255 damage, or half its hit points if its race is 0x17 (a liche, by the
   owner race strings 0x172.., inferred). */
char far sp_ward_undead(int x, int y, struct Object far *target, struct Tile far *tile,
                        unsigned char src)
{
    int damage = 0xFF;

    if (OBJ_ITEM(target) == ITEM_SKULL_13)
        return destroy_floatskull(target);
    switch (OBJ_MAJOR(target)) {
    case MAJOR_CREATURE:
        if (Creature[target->id & ID_INMAJOR].race == 0x17)
            damage = target->hp / 2;
        if (check_res(target, 1, 0x80) == 0) {
            damage_item(target, Obj_IntTMem(src), x, y, damage, 3);
            return 1;
        }
    }
    return 0;
}

/* Poison: 5d4 damage of type 0x13 (magic and poison) to a critter. */
char far sp_poison(int x, int y, struct Object far *target, struct Tile far *tile,
                   unsigned char src)
{
    if (OBJ_MAJOR(target) != MAJOR_CREATURE)
        return 0;
    put_effect(target, 7, 4, 0, 7, x, y);
    damage_item(target, Obj_IntTMem(src), x, y, rollem(5, 4), 0x13);
    return 1;
}

/* A mind spell lands: unless the critter shrugs it off (check_res type 3: a chance of
   (resist & 3) in 3) it gets
   goal for gtarg and, if attitude is not -1, that attitude. Always returns 1. */
char far hit_critter_goal(char goal, char attitude, int gtarg, struct Object far *npc,
                          int x, int y)
{
    if (check_res(npc, 1, 3)) {
        put_effect(npc, 7, 4, 0, 7, x, y);
        change_critter_goal(npc, goal, gtarg);
        if (attitude != -1)
            SET_ATTITUDE(npc, attitude);
    }
    return 1;
}

/* Charm: whether a critter can be charmed is a character of string 0x15E (or 0x15F for
   whoami 0x8C and up) indexed by its whoami, '+' meaning yes. A charmed critter becomes
   friendly (goal 8, attitude 3) and, in the Pits of Carnage, leaves the fight. */
char far sp_charm(int x, int y, struct Object far *target)
{
    char far *str;
    int whoami;
    int which;

    if (OBJ_MAJOR(target) != MAJOR_CREATURE)
        return 0;
    if (check_res(target, 1, 3)) {
        which = 0;
        whoami = target->whoami;
        put_effect(target, 7, 4, 0, 7, x, y);
        if (whoami >= 0x8C) {
            whoami -= 0x8C;
            which = 1;
        }
        str = get_string((which + 0x15E) | STR_GAME);
        if (str[whoami] == '+') {
            change_critter_goal(target, 8, 0);
            SET_ATTITUDE(target, 3);
            if (player->in_pits)
                remove_opponent(target);
        }
    }
    return 1;
}

/* Mass Confusion, one square: a critter gets goal 2 and attitude 1 (wander, inferred). */
char far sp_confusion(int x, int y, struct Object far *target, struct Tile far *tile,
                      unsigned char src)
{
    struct Object far *caster;
    int tx = x;
    int ty = y;

    caster = Obj_IntTMem(src);
    if (target == 0) {
        if (rand() % 3 == 0)
            put_effect(0L, 7, 4, 0, -(OBJ_Z(caster) + 20), tx, ty);
        return 0;
    }
    if (OBJ_MAJOR(target) != MAJOR_CREATURE)
        return 0;
    put_effect(target, 7, 4, 0, 7, tx, ty);
    return hit_critter_goal(2, 1, 1, target, tx, ty);
}

/* Shared damage step of the attack spells: a splash effect, damage_item with damage and
   type, and with show set the critter's health in the view frame. Returns 0 for a target
   that is not a critter. */
char far wound_foe(int x, int y, struct Object far *target, struct Tile far *tile,
                   unsigned char src, int damage, unsigned char type, int effect, char show)
{
    int level;
    int splats;

    if ((splats = damage / 4) >= 4)
        splats = 3;
    if (OBJ_MAJOR(target) != MAJOR_CREATURE)
        return 0;
    put_effect(target, effect, 1, splats, 2, x, y);
    damage_item(target, Obj_IntTMem(src), x, y, damage, type);
    if (show) {
        if (Creature[target->id & ID_INMAJOR].avghit)
            level = target->hp * 3 / Creature[target->id & ID_INMAJOR].avghit;
        else
            level = 0;
        if (level >= 3)
            level = 2;
        set_screen_frame(7, 3 - level);
    }
    return 1;
}

/* Shockwave, one square: Casting / 2 + 15 damage of type 3 to every critter but the caster. */
char far sp_shockwave(int x, int y, struct Object far *target, struct Tile far *tile,
                      unsigned char src)
{
    struct Object far *caster;
    int power;

    power = player->skills[SKILL_CASTING] / 2 + 15;
    caster = Obj_IntTMem(src);
    if (target == 0) {
        put_effect(0L, 11, 1, 0, -(OBJ_Z(caster) + 20), x, y);
        return 0;
    }
    if (Obj_MemTPtr(target) == src)
        return 0;
    return wound_foe(x, y, target, tile, src, power, 3, 11, 0);
}

/* Bleeding: Casting / 2 + 10 damage of type 4, only to creatures that bleed
   (otherwise 'That creature does not bleed.'). */
char far sp_bleed(int x, int y, struct Object far *target, struct Tile far *tile,
                  unsigned char src)
{
    int power;

    power = player->skills[SKILL_CASTING] / 2 + 10;
    if (OBJ_MAJOR(target) != MAJOR_CREATURE)
        return 0;
    if (!Creature[target->id & ID_INMAJOR].blood) {
        game_sprint(0x12B);  /* 'That creature does not bleed.' */
        return 0;
    }
    return wound_foe(x, y, target, tile, src, power, 4, 0, 1);
}

/* Smite Foe: Casting * 3 + 110 damage of type 4 (string 0x12B for a bloodless target, as
   in sp_bleed). */
char far sp_smite(int x, int y, struct Object far *target, struct Tile far *tile,
                  unsigned char src)
{
    int power;

    power = player->skills[SKILL_CASTING] * 3 + 110;
    if (OBJ_MAJOR(target) != MAJOR_CREATURE)
        return 0;
    if (!Creature[target->id & ID_INMAJOR].blood) {
        game_sprint(0x12B);  /* 'That creature does not bleed.' */
        return 0;
    }
    return wound_foe(x, y, target, tile, src, power, 4, 0, 1);
}

/* Frost, one square: frost effects on empty squares, 10 damage of type 0x23 (magic and
   cold) to anything there. */
char far sp_frost(int x, int y, struct Object far *target, struct Tile far *tile,
                  unsigned char src)
{
    int r;
    int damage = 10;
    struct Object far *obj;

    if (target == 0) {
        obj = build_new_obj(ITEM_FROST, tile);
        r = rand() % 3;
        if (add_animobj(Obj_MemTPtr(obj), 4 - r, r, x, y) == -1)
            Obj_Free(obj);
        else
            Obj_Add(&tile->objects, obj);
        return 0;
    } else if (OBJ_MAJOR(target) == MAJOR_CREATURE)
        return wound_foe(x, y, target, tile, src, damage, 0x23, 11, 0);
    else
        damage_item(target, Obj_IntTMem(src), x, y, damage, 0x23);
    return 0;
}

/* name: IDA CauseFear. The map pairs it with FM Towns sp_fear_ by size alone; the code agrees
   (attitude 1, then hit_critter_goal(6, -1, 1, ...)), and sp_fear_ is the FM Towns
   function at this position, between sp_frost_ and sp_repel_undead_. */
/* Cause Fear: attitude 1 and goal 6 (flee, inferred) unless resisted. */
char far sp_fear(int x, int y, struct Object far *target, struct Tile far *tile,
                 unsigned char src)
{
    if (OBJ_MAJOR(target) != MAJOR_CREATURE)
        return 0;
    SET_ATTITUDE(target, 1);
    return hit_critter_goal(6, -1, 1, target, x, y);
}

/* Repel Undead, one square, the player's only. The spell has a budget: Casting * 10
   against area_spell_state, the total hit points (and skull qualities) already affected.
   Floating skulls are destroyed; an undead critter (resist bit 0x80, as in sp_ward_undead)
   already fleeing takes Casting damage, any other is made to flee. */
char far sp_repel_undead(int x, int y, struct Object far *target, struct Tile far *tile,
                         unsigned char src)
{
    struct Object far *caster;
    char ret;
    int tx = x;
    int ty = y;

    caster = Obj_IntTMem(src);
    if (Obj_MemTPtr(ThePlayer) != src)
        return 1;
    if (target == 0) {
        if (rand() % 3 == 0)
            put_effect(0L, 7, 4, 0, -(OBJ_Z(caster) + 20), tx, ty);
        return 0;
    } else if (OBJ_ITEM(target) == ITEM_SKULL_13) {
        area_spell_state += OBJ_QUALITY(target);
        return destroy_floatskull(target);
    } else if (OBJ_MAJOR(target) == MAJOR_CREATURE && check_res(target, 1, 0x80) == 0
               && player->skills[SKILL_CASTING] * 10 >= area_spell_state) {
        if (OBJ_GOAL(target) == 6)
            ret = wound_foe(tx, ty, target, tile, src, player->skills[SKILL_CASTING], 3, 11, 0);
        else
            ret = hit_critter_goal(6, -1, 1, target, tx, ty);
        area_spell_state += target->hp;
        put_effect(target, 7, 4, 0, 7, tx, ty);
        return ret;
    }
    return 0;
}

/* Paralyze (and the holding weapon): a critter that is not undead (check_res type 0x80
   returns 1) is held
   (goal 15) for 16 + 0..15 * power ticks, power Casting / 3 for the player and 8 for
   others. */
char far sp_hold(int x, int y, struct Object far *target, struct Tile far *tile,
                 unsigned char src)
{
    int power;
    int time;

    if (src != 1)
        power = 8;
    else
        power = player->skills[SKILL_CASTING] / 3;
    time = (rand() & 0xF) * power + 0x10;
    if (OBJ_MAJOR(target) == MAJOR_CREATURE && check_res(target, 1, 0x80) == 1)
        return hit_critter_goal(15, 1, time, target, x, y);
    return 0;
}

/* Calls fn on the squares of a w + 1 by h + 1 rectangle (clipped to the map), at most
   count times that return true. type is the target mode, sub's top bits: 0 every critter
   other than the caster; 0x40 random open squares (each with chance count in w * h + 3,
   up to five passes until count is used); 0x80 every open square and every object on it;
   0xC0 every object. */
void far process_area(char count, unsigned char src, SpellFn fn, unsigned char type,
                      char x0, char y0, char w, char h)
{
    struct Object far *obj;
    struct Tile far *tile;
    struct Tile far *start;
    int tries = 0;
    union Link far *link;
    int next;
    int x;
    int y;

    if (x0 >= MAP_SIZE)
        return;
    if (x0 + w < 0)
        return;
    if (y0 >= MAP_SIZE)
        return;
    if (y0 + h < 0)
        return;
    if (x0 < 0) {
        w -= -x0;
        x0 = 0;
    } else if (x0 + w >= MAP_SIZE)
        w -= x0 + w - MAP_SIZE;
    if (y0 < 0) {
        h -= -y0;
        y0 = 0;
    } else if (y0 + h > MAP_SIZE)
        h -= y0 + h - MAP_SIZE;
    if (w <= 0)
        return;
    if (h <= 0)
        return;
    start = Map_GetAddr(x0, y0);
    do {
        for (x = x0; x <= x0 + w; x++)
            for (y = y0; y <= y0 + h; y++) {
                if (x < 0)
                    continue;
                if (x >= MAP_SIZE)
                    continue;
                if (y < 0)
                    continue;
                if (y >= MAP_SIZE)
                    continue;
                tile = start + (x - x0) + ((y - y0) << 6);
                if (type == 0x40 || type == 0x80) {
                    obj = 0;
                    if (tile->type > TILE_SOLID && (rand() % (w * h + 3) < count || type == 0x80))
                        if (fn(x, y, obj, tile, src) && --count == 0)
                            return;
                }
                if (type == 0x40)
                    continue;
                link = &tile->objects;
                while ((obj = Obj_PtrTMem(link)) != 0) {
                    next = link->word >> 6 & 0x3FF;
                    if (type == 0x80
                        || type == 0 && OBJ_MAJOR(obj) == MAJOR_CREATURE && Obj_MemTPtr(obj) != src
                        || type == 0xC0)
                        if (fn(x, y, obj, tile, src) && --count <= 0)
                            return;
                    if ((link->word >> 6 & 0x3FF) == next)
                        link = &obj->qn.link;
                }
            }
    } while (type == 0x40 && count > 0 && tries++ < 4);
}

/* An area spell centred dist squares ahead of who (along the caster's heading; an object
   caster uses inanmMapX, inanmMapY and the coarse heading), radius squares around. */
void far gronk_area(struct Object far *who, char count, SpellFn fn, unsigned char type,
                    unsigned char dist, unsigned char radius)
{
    int x;
    int y;
    int index;
    unsigned char src;
    int heading;

    index = Obj_MemTPtr(who);
    if (index < NUM_MOBILE) {
        src = index;
        heading = (OBJ_HEADING(who) << 5) + OBJ_FINEHEAD(who);
        x = OBJ_HOMEX(who);
        y = OBJ_HOMEY(who);
    } else {
        src = 0;
        heading = OBJ_HEADING(who) << 5;
        x = inanmMapX;
        y = inanmMapY;
    }
    move_along(heading, dist, &x, &y);
    process_area(count, src, fn, type, (char)x - radius, (char)y - radius, radius * 2 + 1,
                 radius * 2 + 1);
    src = 0;                            /* match: a dead store, but the DOS bytes have it */
}

/* Calls fn(npc, arg) for the first (or with all, every) active critter whose whoami
   matches. fn returns true when it removed the critter from the active list. */
void far gronk_whoami(int whoami, unsigned char all, int arg,
                      char (far *fn)(struct Object far *npc, int arg))
{
    unsigned char far *p;
    struct Object far *npc;

    for (p = ActiveMob; p < LastActiveMob; p++) {
        npc = Obj_IntTMem(*p);
        if (npc->whoami == whoami) {
            if (fn(npc, arg))
                p--;
            if (!all)
                return;
        }
    }
}

struct AreaSpell {
    SpellFn fn;
    char count;
    unsigned char dist;
    unsigned char radius;
};

struct AreaSpell area_spells[8] = {
    { sp_true_sight, 100, 1, 2 },
    { sp_sheet_light, 6, 4, 2 },
    { sp_confusion, 12, 4, 2 },
    { sp_meteor, 10, 4, 2 },
    { sp_repel_undead, 50, 4, 2 },
    { sp_shockwave, 50, 0, 1 },
    { sp_frost, 5, 3, 1 },
};

SpellFn area1_spells[8] = {
    sp_bleed, sp_fear, sp_ward_undead, sp_charm, sp_poison, sp_hold, sp_smite,
    (SpellFn)sp_study_monster
};

/* Area spells (class 6): area_spells[minor - 1] gives the per-square handler, how many
   times it may act, how far ahead and the radius. */
void far nail_area(struct Object far *who, unsigned char sub)
{
    struct AreaSpell spell;

    spell = area_spells[(sub & ~0xC0) - 1];
    area_spell_state = 0;
    gronk_area(who, spell.count, spell.fn, sub & 0xC0, spell.dist, spell.radius);
}

/* The player has clicked a critter for a targeted spell (class 7, minor 0..7: bleeding,
   fear, smite undead, charm, poison, paralyze, smite foe, study monster). The mana is paid
   only if the handler reports success. */
void far target_spells(struct Object far *target)
{
    SpellFn fn;
    struct Object far *caster;
    int index;
    unsigned char src;

    fn = area1_spells[ObjectActorArg];
    caster = ObjectActing;
    index = Obj_MemTPtr(caster);
    if (index < NUM_MOBILE)
        src = index;
    else
        src = 0;
    if (fn(MapObj_X, MapObj_Y, target, PickMap, src))
        spend_mana(mspell_mused);
    unforce_mouse_cursor(3);
    GameInputMode = 0;
    mouse_release(1);
    mspell_mused = 0;
}

/* Targeted spells (class 7): the player gets the targeting cursor (GameInputMode 2); minor
   0..7 target critters (target_spells), 8 and up objects (obj_spells). Critter casters do
   nothing here. */
void far nail_1area(struct Object far *who, unsigned char sub)
{
    if (who == ThePlayer) {
        if (sub > 7)
            ObjectActor = (ActorFn)obj_spells;
        else
            ObjectActor = (ActorFn)target_spells;
        GameInputMode = 2;
        ObjectActorArg = sub;
        ObjectActing = who;
        force_mouse_cursor(0x1076);
    }
}

/* The player has clicked an object for an object spell (ObjectActorArg): 8 Dispel Rune
   (only a rune of flame or stasis), 9 Mending (quality to 0x3F), 10 Remove Trap, 11 Name
   Enchantment (look with identify), 12 Open (checkLock with skill -45), 13 Detect Trap,
   14 Enchantment (SPELLS2.C), 15 Gate Travel. Gate Travel, cast on a moonstone: the player
   has two moonstone records (player->moonstones, the level each was left on). If one was
   left on another level the player goes to that level and do_mstone (SKILLS.C) finds it
   there; if the placed ones are on this level, to the other moonstone object on this map;
   with none placed, string 0x121. Mana is spent at the end, waived where
   the spell found nothing to work on. */
void far obj_spells(struct Object far *target, int how, unsigned char b)
{
    struct Tile far *tile;
    union Link far *link;
    unsigned char ok;
    int item;
    int x;
    int y;
    struct Object far *found;
    char name[80];
    int i;
    int count;

    switch (ObjectActorArg) {
    case 8:
        if (OBJ_ITEM(target) != ITEM_FLAM_RUNE && OBJ_ITEM(target) != ITEM_TYM_RUNE) {
            game_sprint(0x12E);  /* 'That is not a rune.' */
            mspell_mused = 0;
        } else {
            tile = Map_GetAddr(MapObj_X, MapObj_Y);
            link = &tile->objects;
            Obj_Punt(link, target, 1);
        }
        break;
    case 9:
        if (mendable(target)) {
            name[0] = 0;
            GetObjDesc(target, 1, name);
            game_sprint(0x12F);  /* 'The spell repairs the ' */
            scroll_print(name);
            game_sprint(0x60);  /* '.' */
            target->qn.f.quality = 0x3F;
            FixPlayerEquips();
            editchng(0x200);
        } else {
            game_sprint(0x142);  /* 'The spell has no noticeable effect.' */
            mspell_mused = 0;
        }
        break;
    case 10:
        if (DetectedTrap(target, 0x2D))
            RemoveTrap(target, 0x2D);
        break;
    case 11:
        LookAt(target, 3);
        if (OBJ_MAJOR(target) != MAJOR_RECT && OBJ_MAJOR(target) != MAJOR_CREATURE
            && ComObjData[OBJ_ITEM(target)].render != 2)
            SET_HEADING(target, 7);
        break;
    case 12:
        if (checkLock(ThePlayer, target, -45) == 3)
            game_sprint(0x11E);  /* 'The spell unlocks the lock.' */
        else
            game_sprint(0x11F);  /* 'The spell has no discernable effect.' */
        break;
    case 13:
        if (DetectedTrap(target, 0x2D) > 0)
            game_sprint(0x130);  /* 'You have detected a trap.' */
        else
            game_sprint(0x131);  /* 'You detect no traps.' */
        break;
    case 14:
        sp_enchant(target, b, MapObj_X, MapObj_Y);
        break;
    case 15:
        count = 0;
        ok = 1;
        if (OBJ_ITEM(target) != ITEM_MOONSTONE) {
            game_sprint(0x143);  /* 'That is not a moonstone.' */
            mspell_mused = 0;
            break;
        }
        for (i = 0; i < 2; i++) {
            if (player->moonstones[i] != PlayerLevel) {
                if (player->moonstones[i]) {
                    ok = 0;
                    break;
                }
                count++;
            }
        }
        if (i >= 2 && !ok || count == 2) {
            game_sprint(0x121);  /* 'The moonstone is not available.' */
            break;
        }
        if (ok) {
            item = ITEM_MOONSTONE;
            x = 0;
            y = 0;
            while ((found = Obj_FindInMap(item >> 6, (item & ID_MINOR) >> 4, item & ID_INCLASS, &x, &y))
                   == target)
                x++;
            if (found == 0) {
                x = MapObj_X;
                y = MapObj_Y;
            }
            npp_func = 0;
            do_teleport(ThePlayer, x, y, 0);
            player_setup(0, 0, -1);
        } else {
            npp_func = do_mstone;
            area_spell_state = player->moonstones[i];
            do_teleport(ThePlayer, 0x3F, 0x3F, player->moonstones[i]);
            player_setup(0x3F, 0x3F, -1);
        }
        editchng(0x7FFE);
        break;
    }
    unforce_mouse_cursor(3);
    spend_mana(mspell_mused);
    mspell_mused = 0;
    GameInputMode = 0;
    mouse_release(1);
}

/* Map Area's test: is (x, y) inside the circle {cx, cy, r}? */
/* name: IDA MapAreaCallBack. FM Towns clip_circle_ sits at this position, between obj_spells_
   and special_spells_, and computes the same test: (x - cx)^2 + (y - cy)^2 <= r^2. */
int far clip_circle(int x, int y, int *circle)
{
    int dx;
    int dy;
    int r;

    dx = x - circle[0];
    dy = y - circle[1];
    r = circle[2];
    return dx * dx + dy * dy <= r * r;
}

/* Class 13: 0 checks for the Guardian's magic markers (SPELLS2.C), 2 Mind Blast (strength
   the critter's intelligence less the player's, plus 0..5 less 0..5, at least 2: half of
   it as damage, a quarter off the mana), 3 and 4
   shift the player's view by 3 either way and, unless he is already hallucinating or
   passes an intelligence check (skill_check against 20), set hallucination 2; 5 sets
   hallucination 3; 7 Map Area (maps a circle of radius Casting / 5 + 2, Casting counting
   double above 15 less 13; not in the Ethereal Void, world 8); 8 and 9 the acid
   and snowball missiles, 12..15 mana restoring at strength 3, 7, 11, 15. */
void far special_spells(struct Object far *who, struct Object far *target, char sub)
{
    int px;
    int shrooms;
    int cint;
    int pint;
    int result;
    int circle[3];
    int r;
    int py;

    px = PN.x >> 8;
    py = PN.y >> 8;
    shrooms = 3;
    switch (sub) {
    case 0:
        thump_your_magic_twanger_froggie();
        break;
    case 2:
        cint = Creature[OBJ_INMAJOR(who)].attr[2];
        pint = playerdat->attr[2];
        result = cint - pint + (int)((long)rand() * 6 / 0x8000L)
                 - (int)((long)rand() * 6 / 0x8000L);
        if (result < 2)
            result = 2;
        set_effect(0x40, result);
        damage_item(ThePlayer, who, px, py, result / 2, 0);
        if (result / 4 > player->play_mana)
            player->play_mana = 0;
        else
            player->play_mana -= result / 4;
        panel_check_hpmp();
        break;
    case 3:
    case 4:
        chg_plyp((int)((long)rand() * 2 / 0x8000L) * 6 - 3);
        if (player->shrooms || skill_check(playerdat->attr[2], 20) > 0) {
            if (inplist->mode == 1)
                fill_FB(0x5F);
            break;
        }
        shrooms = 2;
    case 5:
        game_sprint(0xF3);  /* 'Your vision distorts and you feel light headed.' */
        player->shrooms = shrooms;
        FixPlayerEquips();
        break;
    case 7:
        if ((PlayerLevel - 1) / LEVELS_PER_WORLD == 8) {
            game_sprint(0x142);  /* 'The spell has no noticeable effect.' */
            break;
        }
        r = player->skills[SKILL_CASTING];
        r += r > 15 ? r - 13 : 0;
        r = r / 5 + 2;
        circle[0] = px;
        circle[1] = py;
        circle[2] = r;
        automap_area(px - r, py - r, px + r, py + r, circle, (AreaMapFn)clip_circle);
        game_sprint(0x113);  /* 'You gain a sudden awareness of your surroundings.' */
        break;
    case 8:
        do_spell(5, 4, who, target);
        break;
    case 9:
        do_spell(5, 6, who, target);
        break;
    case 12:
    case 13:
    case 14:
    case 15:
        do_spell(10, (sub - 11) * 4 - 1, who, target);
        break;
    }
}

static unsigned char sq_dice[2] = { 10, 6 };
static unsigned char sq_sides[2] = { 6, 5 };
static unsigned char sq_type[2] = { 11, 3 };

/* The 69 spells, eight to a circle: cls (class << 3), the three runes packed five bits
   each (0x18 none; the last five spells have none) and the minor. Each row's comment
   names its spells from string block 6, 256 + index. */
/* match: far, so its own segment (6389:0000, segment table entry 78, the
   last far data segment, so defined by an overlay after CUTS). FM Towns has spells
   right after this file's last statics (area_spells, area1_spells, then sq_dice, sq_sides
   and sq_type, six bytes ending at an address that is not a multiple of four, and spells
   straight after them), then two bytes of padding before SPELLS2's dtypes: so it was
   defined here, after them. */
struct Spell far spells[69] = {
    { 0x40, 0x2197, 0x01 }, { 0x18, 0x05C8, 0x01 }, { 0x29, 0x3938, 0x01 },   /* 0 Create Food, Luck, Magic Arrow */
    { 0x10, 0x0512, 0x02 }, { 0x3A, 0x5938, 0x0D }, { 0x00, 0x2178, 0x83 },   /* 3 Resist Blows, Detect Trap, Light */
    { 0x08, 0x506F, 0x06 }, { 0x58, 0x06C4, 0x03 }, { 0x38, 0x4058, 0x01 },   /* 6 Bouncing, Locate, Cause Fear */
    { 0x18, 0x4002, 0x0A }, { 0x28, 0x3AC9, 0x05 }, { 0x20, 0x202C, 0x02 },   /* 9 Valor, Deadly Seeker, Lesser Heal */
    { 0x40, 0x20A9, 0x02 }, { 0x08, 0x446F, 0x02 }, { 0x08, 0x51F8, 0x01 },   /* 12 Rune of Flame, Slow Fall, Leaping */
    { 0x58, 0x4197, 0x0D }, { 0x38, 0x2598, 0x00 }, { 0x58, 0x01B8, 0x06 },   /* 15 Dispel Hunger, Bleeding, Cure Poison */
    { 0x38, 0x012E, 0x08 }, { 0x29, 0x38D8, 0x02 }, { 0x00, 0x4178, 0x85 },   /* 18 Dispel Rune, Lightning, Night Vision */
    { 0x30, 0x0142, 0x85 }, { 0x58, 0x466F, 0x00 }, { 0x08, 0x5DF8, 0x44 },   /* 21 Repel Undead, Speed, Water Walk */
    { 0x30, 0x2005, 0x87 }, { 0x20, 0x2198, 0x04 }, { 0x18, 0x3537, 0x0B },   /* 24 Frost, Heal, Poison Weapon */
    { 0x3A, 0x0138, 0x0A }, { 0x18, 0x48B8, 0x46 }, { 0x10, 0x2258, 0x43 },   /* 27 Remove Trap, Flameproof, Thick Skin */
    { 0x3A, 0x5998, 0x07 }, { 0x18, 0x1A4F, 0x05 }, { 0x08, 0x50EF, 0x03 },   /* 30 Study Monster, Missile Protection, Levitate */
    { 0x29, 0x3CB8, 0x03 }, { 0x3A, 0x3AD7, 0x0B }, { 0x3A, 0x12F8, 0x0C },   /* 33 Fireball, Name Enchantment, Open */
    { 0x38, 0x004C, 0x02 }, { 0x40, 0x2269, 0x03 }, { 0x3A, 0x4657, 0x09 },   /* 36 Smite Undead, Rune of Stasis, Mending */
    { 0x5A, 0x39F7, 0x08 }, { 0x38, 0x4236, 0x03 }, { 0x30, 0x55C6, 0x42 },   /* 39 Telekinesis, Charm, Sheet Lightning */
    { 0x00, 0x550B, 0x86 }, { 0x38, 0x562F, 0x0F }, { 0x20, 0x550C, 0x0F },   /* 42 Daylight, Gate Travel, Greater Heal */
    { 0x18, 0x564B, 0x44 }, { 0x68, 0x5898, 0x07 }, { 0x38, 0x008F, 0x05 },   /* 45 Invisibility, Map Area, Paralyze */
    { 0x30, 0x5416, 0x83 }, { 0x3A, 0x55D7, 0x0E }, { 0x30, 0x3810, 0x81 },   /* 48 Mass Confusion, Enchantment, Reveal */
    { 0x30, 0x24F8, 0x86 }, { 0x40, 0x280C, 0x05 }, { 0x58, 0x55F7, 0x09 },   /* 51 Shockwave, Summon Demon, Tremor */
    { 0x58, 0x5497, 0x01 }, { 0x40, 0x39E6, 0x06 }, { 0x30, 0x14F8, 0x84 },   /* 54 Portal, Magic Satellite, Flame Wind */
    { 0x08, 0x54EF, 0x05 }, { 0x58, 0x0278, 0x0B }, { 0x10, 0x22B2, 0x45 },   /* 57 Fly, Freeze Time, Iron Flesh */
    { 0x58, 0x5598, 0x02 }, { 0x58, 0x39F6, 0x07 }, { 0x38, 0x552C, 0x06 },   /* 60 Restoration, Roaming Sight, Smite Foe */
    { 0x58, 0x5542, 0x0C }, { 0x30, 0x6318, 0x05 }, { 0x28, 0x6318, 0x04 },   /* 63 Armageddon, Mass Paralyze, Acid */
    { 0x58, 0x6318, 0x0D }, { 0x50, 0x6318, 0x03 }, { 0x50, 0x6318, 0x09 }   /* 66 Local Teleport, Mana Boost, Restore Mana */
};

/* Damages every object on a square: kind 1 is 10d6 of type 11 (magic and
   fire), kind 2 6d5 of type 3 (magic). */
void far damage_square(int x, int y, unsigned char kind, unsigned char src)
{
    struct Object far *obj;
    struct Object far *next;
    int tx = x;
    int ty = y;

    if (kind-- == 0)
        return;
    kind &= 1;
    obj = Obj_PtrTMem(&Map_GetAddr(tx, ty)->objects);
    while (obj) {
        next = Obj_PtrTMem(&obj->qn.link);
        damage_item(obj, Obj_IntTMem(src), tx, ty, rollem(sq_dice[kind], sq_sides[kind]),
                    sq_type[kind]);
        obj = next;
    }
}
