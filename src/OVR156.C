/* target: ovr156 */
/* opts: -mm -1 -G -O -Y -d */
/* Casting spells: mana and health, missiles, area spells, spells on a target, and the
   damage an area spell does to a tile: the whole of DOS overlay ovr156, in original
   order. Function and global names are the originals from the FM Towns symbol table;
   the source file's own name is not known. */

#include <stdlib.h>

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0x21];
    unsigned char skills[20];           /* 0x21 */
    char pad0a[0x37 - 0x35];
    unsigned char play_mana;            /* 0x37 */
    unsigned char max_mana;             /* 0x38 */
    char pad1[0x5E - 0x39];
    unsigned char moonstones[2];        /* 0x5E, the level each moonstone is on */
    unsigned b60:12;                    /* 0x60 */
    unsigned shrooms:2;                 /* word 0x60, bits 12-13 */
    unsigned b61_6:2;
    unsigned b62:10;                    /* 0x62 */
    unsigned in_pits:1;                 /* word 0x62, bit 10 */
    unsigned b63_3:5;
    char pad2[0x370 - 0x64];
    unsigned char xclock3;              /* 0x370 */
};

/* The player's critter data, reached through the near pointer `playerdat`. */
struct Critter {
    char pad0[4];
    unsigned char max_vit;              /* 0x04 */
    unsigned char attr[3];              /* 0x05: STR, DEX, INT */
};

/* The player's motion record. */
struct Motion {
    int eye_x, eye_y, eye_z;            /* 0x00 */
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
    char pad09[0x0B - 0x09];
    unsigned goal_word;                 /* 0x0B, goal in bits 0-3 */
    unsigned attitude_word;             /* 0x0D, attitude in bits 14-15 */
    char pad0F[0x16 - 0x0F];
    unsigned home;                      /* 0x16, x in bits 10-15, y in bits 4-9 */
    unsigned char b18;                  /* 0x18, fine heading in bits 0-4 */
    char pad19[0x1A - 0x19];
    unsigned char whoami;               /* 0x1A */
};

#define OBJ_ITEM(o)     ((o)->id & 0x1FF)
#define OBJ_MAJOR(o)    (((o)->id & 0x1C0) >> 6)
#define OBJ_INDEX(o)    (((o)->id & 0x3F) >> 0)
#define OBJ_QUALITY(o)  ((o)->qn.f.quality)
#define OBJ_HOMEX(o)    (((o)->home & 0xFC00) >> 10)
#define OBJ_HOMEY(o)    (((o)->home & 0x3F0) >> 4)
#define OBJ_GOAL(o)     (((o)->goal_word & 0xF) >> 0)
#define OBJ_HEADING(o)  (((o)->pos & 0x380) >> 7)
#define SET_HEADING(o, v) ((o)->pos = (o)->pos & 0xFC7F | ((v) & 7) << 7)
#define OBJ_Z(o)        ((o)->pos & 0x7F)
#define SET_Z(o, v)     ((o)->pos = (o)->pos & 0xFF80 | (v) & 0x7F)
#define SET_ATTITUDE(o, v) ((o)->attitude_word = (o)->attitude_word & 0x3FFF | ((v) & 3) << 14)

struct Tile {
    unsigned type:4;
    unsigned height:4;
    unsigned b8:6;
    unsigned b14:2;                     /* bit 14: no magic here */
    unsigned objects;                   /* 0x02, head of the tile's object list */
};

/* One critter type's record, 0x30 bytes. */
struct Creature {
    char pad00[4];
    unsigned char avghit;               /* 0x04 */
    char pad05[0x07 - 0x05];
    unsigned char intel;                /* 0x07 */
    unsigned char b8_0:3;               /* 0x08 */
    unsigned char blood:2;
    unsigned char b8_5:3;
    unsigned char race;                 /* 0x09 */
    char pad0A[0x30 - 0x0A];
};

/* A rune spell: its class in the top five bits of the first byte, its subclass in the
   last byte. */
struct Spell {
    unsigned char cls;
    char pad1[2];
    unsigned char sub;
};

#define SPELL_CLASS(s)  (((s).cls & 0xF8) >> 3)

/* One object type's common properties, 11 bytes. */
struct ComObj {
    char pad0[9];
    unsigned char render:2;             /* 0x09 */
    unsigned char c9_2:6;
    char padA;
};

struct Inplist {
    char pad0[8];
    int field8;
};

extern struct Player near *player;
extern struct Critter near *playerdat;
extern struct Object far *ThePlayer;
extern struct Object far *objdata;
extern struct Object far *ObjectActing;
extern int ObjectActorArg;
extern int GameInputMode;
extern int PlayerLevel;
extern unsigned char inanmMapX, inanmMapY;
extern struct Creature Creature[];
extern struct Spell far spells[];
extern struct Inplist near *inplist;
extern unsigned char far *ActiveMob;
extern unsigned char far *LastActiveMob;
extern void (far *ObjectActor)();
extern int MapObj_X, MapObj_Y;
extern struct Tile far *PickMap;
extern struct ComObj ComObjData[];
extern void (far *npp_func)(void);
extern struct Motion PN;

int area_spell_state = 0;
unsigned char mspell_mused = 0;

/* Elsewhere in the game. */
struct Tile far * far Map_GetAddr(int x, int y);
void far phys_bounce_up(struct Object far *obj);
void far game_sprint(int id);
char far set_curmagic(char cls, char sub, char flags);
void far force_mouse_cursor(int id);
void far creat_spell(struct Object far *who, char sub);
void far xt_spells(struct Object far *who, char flags, char sub);
void far show_cutscene(int n);
void far update_animobj(int n);
void far panel_check_hpmp(void);
int far rollem(int dice, int sides);
void far fill_FB(int colour);
unsigned char far spell_fire(struct Object far *who, int item);
struct Object far * far CreateObj(int id, int b);
int far Obj_MemTPtr(struct Object far *obj);
struct Object far * far Obj_IntTMem(int index);
void far Obj_Free(struct Object far *obj);
void far Obj_Add(unsigned far *head, struct Object far *obj);
int far add_animobj(int index, int len, char a, char x, char y);
void far fireball_effect(struct Object far *obj, int x, int y);
void far put_effect(struct Object far *obj, int type, int size, int a, int b, int x, int y);
char far damage_item(struct Object far *obj, struct Object far *who, int x, int y,
                     unsigned char damage, unsigned char type);
unsigned char far check_res(struct Object far *obj, unsigned char damage, unsigned char type);
char far destroy_floatskull(struct Object far *obj);
void far change_critter_goal(struct Object far *npc, char goal, int gtarg);
char far * far get_string(int id);
void far remove_opponent(struct Object far *npc);
void far set_screen_frame(int which, int frame);
void far move_along(int heading, int dist, int *x, int *y);
struct Object far * far Obj_PtrTMem(unsigned far *link);
void far unforce_mouse_cursor(int n);
void far mouse_release(int n);
void far Obj_Punt(unsigned far *head, struct Object far *obj, int how);
char far mendable(struct Object far *obj);
int far GetObjDesc(struct Object far *obj, int lore, char *s);
void far scroll_print(char far *s);
void far FixPlayerEquips(void);
void far editchng(int bits);
int far DetectedTrap(struct Object far *obj, int skill);
void far RemoveTrap(struct Object far *obj, int skill);
void far LookAt(struct Object far *obj, int how);
int far checkLock(struct Object far *who, struct Object far *obj, int key);
void far sp_enchant(struct Object far *obj, unsigned char how, int x, int y);
struct Object far * far Obj_FindInMap(int major, int minor, int index, int *x, int *y);
void far do_teleport(struct Object far *who, int x, int y, int level);
void far player_setup(int a, int b, int c);
void far do_mstone(void);
void far thump_your_magic_twanger_froggie(void);
void far set_effect(int which, char amount);
void far chg_plyp(int how);
int far skill_check(int value, int target);
void far automap_area(int x0, int y0, int x1, int y1, int *circle, int (far *fn)());

/* Later in this file. */
void far restore_mana(struct Object far *who, char amount);
void far healing(struct Object far *who, char sub);
void far backfire(struct Object far *who, char sub);
void far release_missile(struct Object far *who, char sub);
void far nail_area(struct Object far *who, unsigned char sub);
void far nail_1area(struct Object far *who, unsigned char sub);
void far special_spells(struct Object far *who, struct Object far *target, char sub);
void far damage_square(int x, int y, unsigned char kind, unsigned char src);

void far spend_mana(int cost)
{
    if (cost > 0)
        player->play_mana = player->play_mana > cost ? player->play_mana - cost : 0;
}

char far anti_magic_p(int x, int y)
{
    return Map_GetAddr(x, y)->b14 & 1;
}

char far do_spell(unsigned char cls, unsigned char sub, struct Object far *who,
                  struct Object far *target);

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

char far do_spell(unsigned char cls, unsigned char sub, struct Object far *who,
                  struct Object far *target)
{
    if (who >= objdata && cls <= 11) {
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
        if (who == ThePlayer && (sub & ~0xC0) == 5 && cls == 2 && player->xclock3 == 4) {
            game_sprint(0x14F);
            player->xclock3 = 5;
        }
    case 3:
        if (who == ThePlayer && set_curmagic(cls, sub & 0x3F, sub & 0xC0))
            break;
        return 0;
    case 4:
        if (who) {
            healing(who, sub);
            if (who == ThePlayer)
                game_sprint(0x124);
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

void far restore_mana(struct Object far *who, char amount)
{
    register int gain;

    if (who == ThePlayer) {
        if ((PlayerLevel - 1) / 8 != 5 || (PlayerLevel - 1) % 8 + 1 <= 1
            || (PlayerLevel - 1) % 8 + 1 >= 8 && OBJ_HOMEX(ThePlayer) >= 25) {
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

void far restore_hp(struct Object far *who, char amount)
{
    int hp;

    if (who == ThePlayer) {
        if (amount > 0) {
            hp = playerdat->max_vit * (amount + (rand() & 3));
            hp >>= 4;
            hp += ThePlayer->hp + 1;
        } else
            hp = ThePlayer->hp - amount;
        if (hp > playerdat->max_vit)
            ThePlayer->hp = playerdat->max_vit;
        else
            ThePlayer->hp = hp;
        panel_check_hpmp();
    }
}

void far get_hp_back(struct Object far *who, unsigned char amount)
{
    who->hp = who->hp + amount > Creature[OBJ_INDEX(who)].avghit
        ? Creature[OBJ_INDEX(who)].avghit : who->hp + amount;
    if (who == ThePlayer)
        panel_check_hpmp();
}

void far healing(struct Object far *who, char sub)
{
    unsigned char amount;

    if (OBJ_MAJOR(who) == 1) {
        if (sub == 15)
            amount = 0xFF;
        else
            amount = rollem(sub, 8);
        get_hp_back(who, amount);
    }
}

void far backfire(struct Object far *who, char sub)
{
    char damage;

    if (OBJ_MAJOR(who) != 1)
        return;
    damage = rollem(sub, 8);
    if (who->hp <= 3)
        return;
    if (who->hp - damage <= 3)
        who->hp = 3;
    else
        who->hp = who->hp - damage;
    if (who == ThePlayer) {
        if (inplist->field8 == 1)
            fill_FB(0x30);
        else
            game_sprint(0x16A);
    }
}

void far release_missile(struct Object far *who, char sub)
{
    unsigned char items[6] = { 7, 5, 4, 6, 11, 12 };
    unsigned char ok;

    ok = spell_fire(who, items[sub - 1]);
    if (who == ThePlayer) {
        if (!ok)
            game_sprint(0x10F);
        else
            spend_mana(mspell_mused);
        mspell_mused = 0;
    }
}

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

char far sp_sheet_light(int x, int y, struct Object far *target, struct Tile far *tile,
                        unsigned char src)
{
    struct Object far *obj;

    obj = build_new_obj(0x1C5, tile);
    damage_square(x, y, 2, src);
    if (add_animobj(Obj_MemTPtr(obj), 4, rand() % 4, x, y) == -1)
        Obj_Free(obj);
    else
        Obj_Add(&tile->objects, obj);
    return 1;
}

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
    if (target && OBJ_MAJOR(target) == 1)
        npc = 1;
    if (last == square || count >= 5)
        return 0;
    if (!npc && rand() % 3)
        return 0;
    obj = build_new_obj(0x1C2, tile);
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

char far sp_ward_undead(int x, int y, struct Object far *target, struct Tile far *tile,
                        unsigned char src)
{
    int damage = 0xFF;

    if (OBJ_ITEM(target) == 0x13)
        return destroy_floatskull(target);
    switch (OBJ_MAJOR(target)) {
    case 1:
        if (Creature[target->id & 0x3F].race == 0x17)
            damage = target->hp / 2;
        if (check_res(target, 1, 0x80) == 0) {
            damage_item(target, Obj_IntTMem(src), x, y, damage, 3);
            return 1;
        }
    }
    return 0;
}

char far sp_poison(int x, int y, struct Object far *target, struct Tile far *tile,
                   unsigned char src)
{
    if (OBJ_MAJOR(target) != 1)
        return 0;
    put_effect(target, 7, 4, 0, 7, x, y);
    damage_item(target, Obj_IntTMem(src), x, y, rollem(5, 4), 0x13);
    return 1;
}

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

char far sp_charm(int x, int y, struct Object far *target)
{
    char far *str;
    int whoami;
    int which;

    if (OBJ_MAJOR(target) != 1)
        return 0;
    if (check_res(target, 1, 3)) {
        which = 0;
        whoami = target->whoami;
        put_effect(target, 7, 4, 0, 7, x, y);
        if (whoami >= 0x8C) {
            whoami -= 0x8C;
            which = 1;
        }
        str = get_string((which + 0x15E) | 0x200);
        if (str[whoami] == '+') {
            change_critter_goal(target, 8, 0);
            SET_ATTITUDE(target, 3);
            if (player->in_pits)
                remove_opponent(target);
        }
    }
    return 1;
}

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
    if (OBJ_MAJOR(target) != 1)
        return 0;
    put_effect(target, 7, 4, 0, 7, tx, ty);
    return hit_critter_goal(2, 1, 1, target, tx, ty);
}

char far wound_foe(int x, int y, struct Object far *target, struct Tile far *tile,
                   unsigned char src, int damage, unsigned char type, int effect, char show)
{
    int level;
    int splats;

    if ((splats = damage / 4) >= 4)
        splats = 3;
    if (OBJ_MAJOR(target) != 1)
        return 0;
    put_effect(target, effect, 1, splats, 2, x, y);
    damage_item(target, Obj_IntTMem(src), x, y, damage, type);
    if (show) {
        if (Creature[target->id & 0x3F].avghit)
            level = target->hp * 3 / Creature[target->id & 0x3F].avghit;
        else
            level = 0;
        if (level >= 3)
            level = 2;
        set_screen_frame(7, 3 - level);
    }
    return 1;
}

char far sp_shockwave(int x, int y, struct Object far *target, struct Tile far *tile,
                      unsigned char src)
{
    struct Object far *caster;
    int power;

    power = player->skills[9] / 2 + 15;
    caster = Obj_IntTMem(src);
    if (target == 0) {
        put_effect(0L, 11, 1, 0, -(OBJ_Z(caster) + 20), x, y);
        return 0;
    }
    if (Obj_MemTPtr(target) == src)
        return 0;
    return wound_foe(x, y, target, tile, src, power, 3, 11, 0);
}

char far sp_bleed(int x, int y, struct Object far *target, struct Tile far *tile,
                  unsigned char src)
{
    int power;

    power = player->skills[9] / 2 + 10;
    if (OBJ_MAJOR(target) != 1)
        return 0;
    if (!Creature[target->id & 0x3F].blood) {
        game_sprint(0x12B);
        return 0;
    }
    return wound_foe(x, y, target, tile, src, power, 4, 0, 1);
}

char far sp_smite(int x, int y, struct Object far *target, struct Tile far *tile,
                  unsigned char src)
{
    int power;

    power = player->skills[9] * 3 + 110;
    if (OBJ_MAJOR(target) != 1)
        return 0;
    if (!Creature[target->id & 0x3F].blood) {
        game_sprint(0x12B);
        return 0;
    }
    return wound_foe(x, y, target, tile, src, power, 4, 0, 1);
}

char far sp_frost(int x, int y, struct Object far *target, struct Tile far *tile,
                  unsigned char src)
{
    int r;
    int damage = 10;
    struct Object far *obj;

    if (target == 0) {
        obj = build_new_obj(0x1CA, tile);
        r = rand() % 3;
        if (add_animobj(Obj_MemTPtr(obj), 4 - r, r, x, y) == -1)
            Obj_Free(obj);
        else
            Obj_Add(&tile->objects, obj);
        return 0;
    } else if (OBJ_MAJOR(target) == 1)
        return wound_foe(x, y, target, tile, src, damage, 0x23, 11, 0);
    else
        damage_item(target, Obj_IntTMem(src), x, y, damage, 0x23);
    return 0;
}

/* IDA CauseFear. The map pairs it with FM Towns sp_fear_ by size alone; the code agrees
   (attitude 1, then hit_critter_goal(6, -1, 1, ...)), and sp_fear_ is the FM Towns
   function at this position, between sp_frost_ and sp_repel_undead_. */
char far sp_fear(int x, int y, struct Object far *target, struct Tile far *tile,
                 unsigned char src)
{
    if (OBJ_MAJOR(target) != 1)
        return 0;
    SET_ATTITUDE(target, 1);
    return hit_critter_goal(6, -1, 1, target, x, y);
}

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
    } else if (OBJ_ITEM(target) == 0x13) {
        area_spell_state += OBJ_QUALITY(target);
        return destroy_floatskull(target);
    } else if (OBJ_MAJOR(target) == 1 && check_res(target, 1, 0x80) == 0
               && player->skills[9] * 10 >= area_spell_state) {
        if (OBJ_GOAL(target) == 6)
            ret = wound_foe(tx, ty, target, tile, src, player->skills[9], 3, 11, 0);
        else
            ret = hit_critter_goal(6, -1, 1, target, tx, ty);
        area_spell_state += target->hp;
        put_effect(target, 7, 4, 0, 7, tx, ty);
        return ret;
    }
    return 0;
}

char far sp_hold(int x, int y, struct Object far *target, struct Tile far *tile,
                 unsigned char src)
{
    int power;
    int time;

    if (src != 1)
        power = 8;
    else
        power = player->skills[9] / 3;
    time = (rand() & 0xF) * power + 0x10;
    if (OBJ_MAJOR(target) == 1 && check_res(target, 1, 0x80) == 1)
        return hit_critter_goal(15, 1, time, target, x, y);
    return 0;
}

typedef char (far *SpellFn)(int x, int y, struct Object far *target, struct Tile far *tile,
                            unsigned char src);

void far process_area(char count, unsigned char src, SpellFn fn, unsigned char type,
                      char x0, char y0, char w, char h)
{
    struct Object far *obj;
    struct Tile far *tile;
    struct Tile far *start;
    int tries = 0;
    unsigned far *link;
    int next;
    int x;
    int y;

    if (x0 >= 64)
        return;
    if (x0 + w < 0)
        return;
    if (y0 >= 64)
        return;
    if (y0 + h < 0)
        return;
    if (x0 < 0) {
        w -= -x0;
        x0 = 0;
    } else if (x0 + w >= 64)
        w -= x0 + w - 64;
    if (y0 < 0) {
        h -= -y0;
        y0 = 0;
    } else if (y0 + h > 64)
        h -= y0 + h - 64;
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
                if (x >= 64)
                    continue;
                if (y < 0)
                    continue;
                if (y >= 64)
                    continue;
                tile = start + (x - x0) + ((y - y0) << 6);
                if (type == 0x40 || type == 0x80) {
                    obj = 0;
                    if (tile->type > 0 && (rand() % (w * h + 3) < count || type == 0x80))
                        if (fn(x, y, obj, tile, src) && --count == 0)
                            return;
                }
                if (type == 0x40)
                    continue;
                link = &tile->objects;
                while ((obj = Obj_PtrTMem(link)) != 0) {
                    next = *link >> 6 & 0x3FF;
                    if (type == 0x80
                        || type == 0 && OBJ_MAJOR(obj) == 1 && Obj_MemTPtr(obj) != src
                        || type == 0xC0)
                        if (fn(x, y, obj, tile, src) && --count <= 0)
                            return;
                    if ((*link >> 6 & 0x3FF) == next)
                        link = &obj->qn.word;
                }
            }
    } while (type == 0x40 && count > 0 && tries++ < 4);
}

void far gronk_area(struct Object far *who, char count, SpellFn fn, unsigned char type,
                    unsigned char dist, unsigned char radius)
{
    int x;
    int y;
    int index;
    unsigned char src;
    int heading;

    index = Obj_MemTPtr(who);
    if (index < 0x100) {
        src = index;
        heading = (OBJ_HEADING(who) << 5) + (who->b18 & 0x1F);
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
    src = 0;                            /* a dead store, but the DOS bytes have it */
}

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

char far sp_true_sight(int x, int y, struct Object far *target, struct Tile far *tile,
                       unsigned char src);
char far sp_study_monster(int x, int y, struct Object far *target, struct Tile far *tile,
                          unsigned char src);

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
    sp_study_monster
};

void far nail_area(struct Object far *who, unsigned char sub)
{
    struct AreaSpell spell;

    spell = area_spells[(sub & ~0xC0) - 1];
    area_spell_state = 0;
    gronk_area(who, spell.count, spell.fn, sub & 0xC0, spell.dist, spell.radius);
}

void far target_spells(struct Object far *target)
{
    SpellFn fn;
    struct Object far *caster;
    int index;
    unsigned char src;

    fn = area1_spells[ObjectActorArg];
    caster = ObjectActing;
    index = Obj_MemTPtr(caster);
    if (index < 0x100)
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

void far obj_spells(struct Object far *target, int how, unsigned char b);

void far nail_1area(struct Object far *who, unsigned char sub)
{
    if (who == ThePlayer) {
        if (sub > 7)
            ObjectActor = obj_spells;
        else
            ObjectActor = target_spells;
        GameInputMode = 2;
        ObjectActorArg = sub;
        ObjectActing = who;
        force_mouse_cursor(0x1076);
    }
}

void far obj_spells(struct Object far *target, int how, unsigned char b)
{
    struct Tile far *tile;
    unsigned far *link;
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
        if (OBJ_ITEM(target) != 0x19E && OBJ_ITEM(target) != 0x19F) {
            game_sprint(0x12E);
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
            game_sprint(0x12F);
            scroll_print(name);
            game_sprint(0x60);
            target->qn.f.quality = 0x3F;
            FixPlayerEquips();
            editchng(0x200);
        } else {
            game_sprint(0x142);
            mspell_mused = 0;
        }
        break;
    case 10:
        if (DetectedTrap(target, 0x2D))
            RemoveTrap(target, 0x2D);
        break;
    case 11:
        LookAt(target, 3);
        if (OBJ_MAJOR(target) != 5 && OBJ_MAJOR(target) != 1
            && ComObjData[OBJ_ITEM(target)].render != 2)
            SET_HEADING(target, 7);
        break;
    case 12:
        if (checkLock(ThePlayer, target, -45) == 3)
            game_sprint(0x11E);
        else
            game_sprint(0x11F);
        break;
    case 13:
        if (DetectedTrap(target, 0x2D) > 0)
            game_sprint(0x130);
        else
            game_sprint(0x131);
        break;
    case 14:
        sp_enchant(target, b, MapObj_X, MapObj_Y);
        break;
    case 15:
        count = 0;
        ok = 1;
        if (OBJ_ITEM(target) != 0x126) {
            game_sprint(0x143);
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
            game_sprint(0x121);
            break;
        }
        if (ok) {
            item = 0x126;
            x = 0;
            y = 0;
            while ((found = Obj_FindInMap(item >> 6, (item & 0x30) >> 4, item & 0xF, &x, &y))
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

/* IDA MapAreaCallBack. FM Towns clip_circle_ sits at this position, between obj_spells_
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

    px = PN.eye_x >> 8;
    py = PN.eye_y >> 8;
    shrooms = 3;
    switch (sub) {
    case 0:
        thump_your_magic_twanger_froggie();
        break;
    case 2:
        cint = Creature[OBJ_INDEX(who)].intel;
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
            if (inplist->field8 == 1)
                fill_FB(0x5F);
            break;
        }
        shrooms = 2;
    case 5:
        game_sprint(0xF3);
        player->shrooms = shrooms;
        FixPlayerEquips();
        break;
    case 7:
        if ((PlayerLevel - 1) / 8 == 8) {
            game_sprint(0x142);
            break;
        }
        r = player->skills[9];
        r += r > 15 ? r - 13 : 0;
        r = r / 5 + 2;
        circle[0] = px;
        circle[1] = py;
        circle[2] = r;
        automap_area(px - r, py - r, px + r, py + r, circle, clip_circle);
        game_sprint(0x113);
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
        next = Obj_PtrTMem(&obj->qn.word);
        damage_item(obj, Obj_IntTMem(src), tx, ty, rollem(sq_dice[kind], sq_sides[kind]),
                    sq_type[kind]);
        obj = next;
    }
}
