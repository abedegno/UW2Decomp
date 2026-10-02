/* target: seg024_24E9 */
/* opts: -mm -1 -G -O -d */
/* Combat: where a blow lands, what it hits, whether it hits, the damage it does, the
   sounds of a miss, the player's weapon swing and charge, missiles striking, critters
   attacking and the experience for a kill. The whole of DOS resident segment
   seg024_24E9, in original order. Function and global names are the originals from the
   FM Towns symbol table where it has them; the source file's own name is not known. */

#include <stdlib.h>
#include "combat.h"
#include "critter.h"
#include "gfx.h"
#include "inv.h"
#include "map.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"
#include "view3d.h"

extern struct MissileInfo Missile[];
extern struct Weapon Weapons[];
extern int PlayerPitch;
extern int ObjectActorArg;
extern long far *Time;
/* The player's own critter record. No FM Towns name: static there. */

/* Elsewhere in the game. */
int far add_animobj(int index, int len, int a, char x, char y);
struct Object far * far CreateObj(int id, int b);
void far TerrainCheck(int a);
void far ObjectCheck(int a, int b);
void far play_effect(char type, int x, int y, char vol);
void far play_effect_here(int fx, int vol, char c);
void far play_effect_on_mobile(char fx, struct Object far *obj, int vol);
char far damage_item(struct Object far *obj, struct Object far *who, int x, int y,
                     unsigned char damage, unsigned char type);
void far set_effect(int which, int amount);
void far set_screen_frame(int frame, int how);
void far put_effect(struct Object far *obj, int type, int size, int a, int b, int x, int y);
void far mouse_release(int n);
void far get_name(char far *buf, struct Object far *obj, int a, int b);
void far scroll_print(char far *s);
void far decode_obj_spell(struct Object far *obj, int *major, int *effect, unsigned char *flag);
void far set_new_music(int n);

/* Initialised data, DS:356 onwards. The statics have no FM Towns names; the names here
   are descriptive. */
unsigned char hit_wall = 0;
char missile_hit = 0;
/* By swing (1-9, the ninths of the 3D view from the bottom left): 0 slash, 1 bash, 2 stab,
   indexing a weapon's damage bytes. */
static unsigned char swing_kind[9] = { 2, 2, 2, 0, 0, 0, 1, 1, 1 };
/* By swing / 3: the scan code of the key that holds the swing. */
static unsigned char swing_keys[3] = { 0x34, 0x27, 0x19 };
int pQatt = 0;
int attackKey = -1;
/* By hit location: the height of the blood splash. [4] is set to -hitz before use. */
static signed char hitz_tab[5] = { 5, 3, 1, 7, 0 };
/* Damage and attack bonuses of the special weapons, both indexed by specweap (1-8); the
   bytes do not show where the first table ends, so the 5 may belong to either. */
static unsigned char spec_damage[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static unsigned char spec_skill[9] = { 5, 0, 0, 0, 0, 0, 0, 0, 0 };

/* Uninitialised data, DS:24B0 onwards. Turbo C lays _BSS out in an order set by the names
   (some hash), not by declaration, so the static names were chosen (by compiling candidates) to land
   where the EXE has them: otime, elapsed, swing_state, fire_mode, curr_weapon and
   weapon_data are not original names. */
int askill;
int wsize;
int towhere;
int ddone;
static long otime;
static int elapsed;
int specweap;
static int swing_state;
unsigned char play_pow;
int targx;
int targy;
int hitobj;
signed char cmbModTH[4];
signed char hitz;
int damage;
int hitloc;
static unsigned char fire_mode;
unsigned char hitangle;
unsigned char player_weapon;
char criti;
int fromwho;
static struct Object far *curr_weapon;
char using_altaras_dagger;
static unsigned char *weapon_data;
unsigned char power;

int far pickloc(int dz, int dtop, int az, int atop)
{
    int dmid;
    int amid;

    dmid = (dz + dtop) >> 1;
    amid = (az + atop) >> 1;
    if (dz + 1 > amid)
        return 2;
    if (dtop - 1 < amid)
        return 3;
    if (amid < dmid) {
        if (rand() % 2)
            return 2;
    } else if (rand() % 3 == 0)
        return 3;
    if (rand() % 3)
        return 0;
    return 1;
}

int far set_hitobj(struct MotionCalc *c)
{
    int found;
    int last;
    int ax;
    int ay;
    int idx;
    int t;
    int dx;
    int dy;
    long dist;
    long best;
    struct Object far *obj;
    struct Object far *att;
    int i;

    i = c->first;
    found = -1;
    best = 100000L;
    last = c->first + c->count;
    att = critdata + fromwho;
    ax = (OBJ_HOMEX(att) << 3) + OBJ_FINEX(att);
    ay = (OBJ_HOMEY(att) << 3) + OBJ_FINEY(att);
    for (; i < last; i++) {
        idx = oCollisions[i].link.f.index;
        obj = Obj_IntTMem(idx);
        if (OBJ_MAJOR(obj) == MAJOR_TRAP)
            continue;
        if (idx == fromwho)
            continue;
        if (fromwho == 1 && IsMobElem(obj) && OBJ_ALLY(obj) && (last - 1 != i || best != 100000L))
            continue;
        t = oCollisions[i].offset & 0x3F;
        targx = ((c->x >> 3) + t) & 0x3F;
        t = targx - (c->x >> 3);
        targy = ((c->y >> 3) + (oCollisions[i].offset - t) / MAP_SIZE) & 0x3F;
        dx = ax - ((targx << 3) + OBJ_FINEX(obj));
        dy = ay - ((targy << 3) + OBJ_FINEY(obj));
        dist = dx * dx + dy * dy;
        if (dist < best) {
            best = dist;
            found = i;
        }
    }
    if (found >= 0) {
        t = oCollisions[found].offset & 0x3F;
        targx = ((c->x >> 3) + t) & 0x3F;
        t = targx - (c->x >> 3);
        targy = ((c->y >> 3) + (oCollisions[found].offset - t) / MAP_SIZE) & 0x3F;
    }
    return found;
}

void far find_wall_coll(int heading, int dist, struct MotionCalc *c)
{
    int fx;
    int fy;
    struct Object far *obj;
    int tx;
    int ty;

    curP = c;
    curP->radius = 1;
    curP->index = 0;
    dist++;
    fx = curP->x << 4;
    fy = curP->y << 4;
    do {
        TerrainCheck(0);
        if ((curP->hits0 | curP->hits1) & 0x300) {
            if ((obj = CreateObj(ITEM_FLASH, 0)) == 0)
                return;
            SET_FINEX_UNSIGNED(obj, curP->x & 7);
            SET_FINEY(obj, curP->y & 7);
            tx = curP->x >> 3;
            ty = curP->y >> 3;
            SET_Z(obj, curP->z + 8);
            if (fromwho == 1) {
                hit_wall = 1;
                if (player_weapon == 1 || player_weapon == 2)
                    play_effect(7, curP->x, curP->y, 0);
                else
                    play_effect(8, curP->x, curP->y, 0);
            }
            if (add_animobj(Obj_MemTPtr(obj), 2, 0, tx, ty) == -1)
                Obj_Free(obj);
            else
                Obj_AddEnd(&Map_GetAddr(tx, ty)->objects, obj);
            return;
        }
        move_along(heading, 0x10, &fx, &fy);
        curP->x = fx >> 4;
        curP->y = fy >> 4;
    } while (--dist > 0);
}

unsigned char far resolve_attack(void)
{
    struct Object far *att;
    struct MotionCalc calc;
    register int i;
    register int heading;

    curP = &calc;
    curP->index = fromwho;
    curP->radius = wsize + 1;
    curP->height = (wsize * 2 + 1) * 4;
    att = critdata + fromwho;
    curP->z = OBJ_Z(att) + ComObjData[OBJ_ITEM(att)].height * (towhere / 3) / 3;
    if (fromwho == 1)
        curP->z += PlayerPitch / 0x200;
    hitz = curP->z + ComObjData[OBJ_ITEM(att)].height / 6;
    curP->x = (OBJ_HOMEX(att) << 3) + OBJ_FINEX(att);
    curP->y = (OBJ_HOMEY(att) << 3) + OBJ_FINEY(att);
    heading = (OBJ_HEADING(att) << 5) + OBJ_FINEHEAD(att);
    move_along(heading, wsize + 3, &curP->x, &curP->y);
    ObjectCheck(0, 1);
    if (curP->found) {
        process_objlist();
        if (curP->count == 0)
            return 0;
        i = set_hitobj(curP);
        if (i < 0)
            return 0;
        hitloc = pickloc(oCollisions[i].bottom, oCollisions[i].top,
                         curP->z, curP->height + curP->z);
        hitobj = oCollisions[i].link.f.index;
        return 1;
    }
    TerrainCheck(0);
    if ((curP->hits0 | curP->hits1) & 0x300) {
        curP->x = (OBJ_HOMEX(att) << 3) + OBJ_FINEX(att);
        curP->y = (OBJ_HOMEY(att) << 3) + OBJ_FINEY(att);
        find_wall_coll(heading, wsize + 3, curP);
    }
    return 0;
}

char far is_sharp(struct Object far *weap)
{
    int item;

    item = OBJ_ITEM(weap);
    if (OBJ_MAJOR(weap) != MAJOR_HACK)
        return 0;
    if (OBJ_MINOR(weap) != 0 && OBJ_MINOR(weap) != 1)
        return 0;
    if (item >= ITEM_CUDGEL && item < 10)
        return 0;
    if (item >= ITEM_SLING && item < ITEM_BOW)
        return 0;
    return 1;
}

int far frp_check(int attacker, int defender)
{
    struct Object far *def;
    int result;
    struct Object far *weap;
    struct Creature *cr;
    int slot;

    def = Obj_IntTMem(defender);
    if (OBJ_MAJOR(def) != MAJOR_CREATURE) {
        if (attacker == 1 && OBJ_CLASS(def) == CLASS_DOOR
            && (int)(rand() * 12L / 0x8000L) < (def->id & 7) << 1) {
            slot = 8 - player->lefty;
            DamageInventory(slot, rollem(2, 4), 4, 0, 1);
        }
        return 0;
    }
    cr = &Creature[OBJ_INMAJOR(def)];
    if (hitobj == 1)
        askill -= cmbModTH[hitloc];
    result = skill_check(askill + hitangle, cr->defence);
    slot = 8 - player->lefty;
    if (PoisonWeap) {
        weap = AskInventory(slot);
        if (is_sharp(weap) && cr->blood)
            damage += damage * (player->skills[9] + 30) / 40;
    }
    criti = 0;
    if (result == 2) {
        criti = 1;
        damage = damage * (((rand() & 0x1F) + 0x30) >> 5);
        if (defender == 1) {
            fill_FB(0x23);
            if ((slot = hitloc + 1 & 3) == 3)
                slot += rand() % 5 == 0;
            else if (slot != 0 && slot <= 2)
                slot = player->lefty + 7;
            DamageInventory(slot, rollem(2, 4), 4, 1, 1);
        }
        return 0;
    }
    if (result == -1 && attacker == 1
        && !Creature[Obj_IntTMem(hitobj)->id & ID_INMAJOR].passive) {
        slot = 8 - player->lefty;
        DamageInventory(slot, rollem(2, 3), 4, 0, 1);
    }
    return 1 - result;
}

void far do_damage(int type)
{
    int quot;
    int rem;
    struct Object far *def;
    char result;
    int fx;
    register int item;
    register int level;

    def = Obj_IntTMem(hitobj);
    item = OBJ_ITEM(def);
    if (damage < 2)
        damage = 2;
    quot = damage / 6;
    rem = damage % 6;
    damage = 0;
    if (quot != 0)
        damage = rollem(quot, 6);
    if (rem != 0)
        damage += rollem(1, rem);
    ddone = damage * power >> 7;
    ddone += hitangle;
    if (hitobj == 1)
        play_effect_here(3, 0x40, ddone << 2);
    else {
        switch (player_weapon) {
        case 0:
        case 1:
            fx = 4;
            break;
        case 2:
            fx = 0x1B;
            break;
        default:
            fx = 4;
        }
        if (missile_hit == 0)
            play_effect(fx, (targx << 3) + OBJ_FINEX(def), (targy << 3) + OBJ_FINEY(def), ddone << 2);
    }
    if (item >> 6 == MAJOR_CREATURE) {
        int armour;

        if ((armour = Creature[item & ID_INMAJOR].armour[hitloc % 4]) == 0xFF) {
            hitloc = hitloc & 4;
            armour = Creature[item & ID_INMAJOR].armour[0];
        }
        if (hitobj != 1 && OBJ_POWERFUL(def))
            armour = armour * 5 / 3;
        if (armour > ddone)
            ddone = 0;
        else
            ddone -= armour;
    }
    if ((level = ddone / 4) >= 4)
        level = 3;
    if (hitobj == 1 && player->easy)
        ddone >>= 1;
    result = damage_item(def, Obj_IntTMem(fromwho), targx, targy, ddone, type);
    if (ddone == 0)
        return;
    if (fromwho == -1)
        return;
    if (hitloc >= 4) {
        hitz_tab[4] = -hitz;
        hitloc = 4;
    }
    if ((int)(UsPtr - critdata) == hitobj) {
        set_effect(0x20, level * 5);
        return;
    }
    if (item >> 6 == MAJOR_CREATURE) {
        if (fromwho == 1) {
            if (Creature[item & ID_INMAJOR].avghit != 0)
                fx = def->hp * 3 / Creature[item & ID_INMAJOR].avghit;
            else
                fx = 0;
            if (fx >= 3)
                fx = 2;
            set_screen_frame(7, 3 - fx);
        }
        if (Creature[item & ID_INMAJOR].blood) {
            put_effect(def, 0, 1, level, hitz_tab[hitloc], targx, targy);
            if (criti && fromwho == 1)
                put_effect(def, 0, 1, level, hitz_tab[hitloc] + (rand() & 1) * 5 - 2, targx, targy);
            return;
        }
        result = 0;
    }
    if (result)
        def = 0;
    if (((item & ID_CLASS) >> 4) == CLASS_DOOR || item == ITEM_MOVING_DOOR) {
        if (OBJ_Z(def) > hitz) {
            hitz = OBJ_Z(def) + 2;
            put_effect(def, 0xB, 1, level, -hitz, targx, targy);
        } else
            put_effect(def, 0xB, 1, level, -hitz, targx, targy);
    } else
        put_effect(def, 0xB, 1, level, -hitz, targx, targy);
}

char far do_miss(int hit)
{
    struct Object far *armour;
    unsigned char slot;
    char weapon;
    char victim;
    char fx;
    int item;
    int attitem;

    if (hit == 0) {
        if (!hit_wall)
            play_effect_on_mobile(10, Obj_IntTMem(fromwho), 0);
        else
            hit_wall = 0;
    } else {
        attitem = OBJ_ITEM(Obj_IntTMem(fromwho));
        if (fromwho == 1)
            weapon = player_weapon;
        else if (fromwho < NUM_MOBILE)
            weapon = Creature[attitem & ID_INMAJOR].weapon_kind;
        else
            weapon = 1;
        if (hitobj == 1) {
            armour = AskInventory(slot = hitloc + 1 & 3);
            item = OBJ_ITEM(armour);
            if (armour == 0)
                victim = 0;
            else if (item == ITEM_LEATHER_VEST || item == ITEM_LEATHER_LEGGINGS
                     || item == ITEM_LEATHER_GLOVES || item == ITEM_LEATHER_BOOTS
                     || item == ITEM_LEATHER_CAP)
                victim = 0;
            else
                victim = 1;
        } else if (hitobj < NUM_MOBILE)
            victim = Creature[attitem & ID_INMAJOR].armour_kind;
        else
            victim = 0;
        if (weapon == 1 || weapon == 2 && victim == 1)
            fx = 7;
        else
            fx = 8;
        play_effect_on_mobile(fx, Obj_IntTMem(hitobj), 0);
    }
    return 0;
}

void far compute_hitangle(void)
{
    hitangle = OBJ_HEADING(Obj_IntTMem(hitobj)) + 0xC - OBJ_HEADING(Obj_IntTMem(fromwho)) & 7;
    if (hitangle > 4)
        hitangle = 8 - hitangle;
}

char far do_attack(void)
{
    int result;

    if (!resolve_attack())
        return do_miss(0);
    if (fromwho != 1 && hitobj != 1 && IsMobElem(Obj_IntTMem(hitobj))
        && !(OBJ_ALLY(Obj_IntTMem(hitobj)) ^ OBJ_ALLY(Obj_IntTMem(fromwho))))
        return 0;
    compute_hitangle();
    if ((result = frp_check(fromwho, hitobj)) != 0) {
        damage_item(Obj_IntTMem(hitobj), Obj_IntTMem(fromwho), targx, targy, 0, 4);
        return do_miss(result);
    }
    do_damage(4);
    return 1;
}

int far check_ammo(int weapon)
{
    int found;
    struct StaticObj fake;
    char buf[50];
    struct StaticObj *p;
    int ammo;

    ammo = Missile[weapon].ammo;
    if (FindObj(MAJOR_HACK, 1, ammo, 4, &found) == 0) {
        p = &fake;
        SET_ITEM(p, ammo + FIRST_MISSILE);
        game_sprint(0xCF);
        get_name(buf, (struct Object far *)p, 0, 1);
        scroll_print(buf);
        game_sprint(0x60);
        return -1;
    }
    return found;
}

int far GetPlayerWeapon(unsigned char **wd, struct Object far **weap)
{
    register int item;

    *wd = 0;
    *weap = AskInventory(8 - player->lefty);
    if (*weap != 0) {
        if (((item = OBJ_ITEM(*weap)) >> 4) == CLASS_MISSILE) {
            if (Missile[item & ID_INCLASS].ammo >= 0 && Missile[item & ID_INCLASS].ammo < 0x10) {
                if (check_ammo(item & ID_INCLASS) >= 0) {
                    *wd = (unsigned char *)&Missile[item & ID_INCLASS];
                    player_weapon = 3;
                    return 0;
                }
                mouse_release(1);
                player_weapon = 4;
                return -1;
            }
        } else if ((item >> 4) == CLASS_WEAPON) {
            *wd = (unsigned char *)&Weapons[item & ID_INCLASS];
            wsize = ComObjData[item].radius;
            if (is_sharp(*weap))
                player_weapon = 2;
            else
                player_weapon = 1;
        }
    }
    if (*wd == 0) {
        *wd = (unsigned char *)&Weapons[15];
        wsize = ComObjData[15].radius;
        player_weapon = 0;
    }
    return 1;
}

void far DoPlayerWeapon(register unsigned char *wd, struct Object far *weap, int swing)
{
    int major;
    int effect;
    unsigned char flag;
    register int skill;

    using_altaras_dagger = OBJ_ITEM(weap) == ITEM_JEWELLED_DAGGER;
    if ((skill = wd[6]) >= SKILL_MISSILE || skill < SKILL_BAREHAND)
        skill = SKILL_BAREHAND;
    askill = (player->skills[SKILL_ATTACK] >> 1) + player->skills[skill] + Valor;
    askill += player->dexterity / 7;
    if (player->easy)
        askill += 7;
    if (skill == SKILL_BAREHAND)
        damage = player->skills[SKILL_BAREHAND] * 2 / 5 + Creature[ThePlayer->id & ID_INMAJOR].attr[0] / 6 + 4;
    else
        damage = wd[swing_kind[swing - 1]] + Creature[ThePlayer->id & ID_INMAJOR].attr[0] / 9;
    fromwho = 1;
    towhere = swing;
    if (weap != 0) {
        decode_obj_spell(weap, &major, &effect, &flag);
        if (!flag && major == 0xC) {
            if (effect < 8) {
                if (effect < 4)
                    damage += effect * 2 + 1;
                else
                    askill += effect * 2 - 7;
            } else {
                specweap = effect - 7;
                damage += spec_damage[specweap];
                askill += spec_skill[specweap];
            }
        }
    }
}

void far missile_finish(void)
{
    pQatt = -10;
    attackKey = -1;
    set_screen_frame(3, 0);
    GameInputMode -= 4;
    unforce_mouse_cursor(3);
    set_screen_frame(8, 4);
    fire_mode = 0;
}

void far fin_attack(void)
{
    if (player->drawn)
        set_screen_frame(8, 4);
    else
        set_screen_frame(8, 6);
    set_screen_frame(3, 0);
}

void far clear_fight_state(void)
{
    if (fire_mode != 0 && swing_state == 0) {
        GameInputMode -= 4;
        unforce_mouse_cursor(3);
        fire_mode = 0;
    }
    fin_attack();
    pQatt = 0;
    attackKey = -1;
    fromwho = -1;
}

void far player_attack(int swing)
{
    int charge;
    unsigned char held;
    int tx;
    int ty;
    struct Tile far *tile;
    struct Object far *target;
    struct Object far *obj;
    unsigned char *wd;
    unsigned frame;

    if (attackKey > 0)
        held = key_on[attackKey] != 0;
    held = held || (mouse_getbut(&charge) & 2);
    if (pQatt > 0)
        return;
    if (pQatt < 0) {
        frame = wframe[get_wfr(weap_frame)];
        if (weap_frame < 0 && pQatt <= -10) {
            fin_attack();
            pQatt = 0;
            attackKey = -1;
            return;
        }
        if ((frame & 0xE0) > 0x80)
            return;
        wd = weapon_data;
        if ((frame & 0xE0) < 0x40) {
            if (!held) {
                fin_attack();
                pQatt = 0;
                attackKey = -1;
            }
            return;
        }
        if ((frame & 0xE0) == 0x40) {
            if (fire_mode != 0) {
                if (held || swing_state < 0) {
                    if (swing_state >= 0)
                        return;
                    set_screen_frame(3, 9);
                    swing_state = 0;
                    GameInputMode += 4;
                    force_mouse_cursor(0x1075);
                    return;
                }
                if (!held) {
                    if (mous_in_3d_p())
                        player_fire(OBJ_INCLASS(curr_weapon));
                    missile_finish();
                }
                return;
            }
            if (held) {
                playerdat->noise = 10;
                if (elapsed < 0) {
                    otime = *Time;
                    elapsed = 0;
                    return;
                }
                elapsed += (int)(*Time - otime);
                otime = *Time;
                while (elapsed > 0x10) {
                    play_pow += wd[4];
                    if (play_pow > 100)
                        play_pow = 100;
                    set_screen_frame(3, play_pow / 12 + 1);
                    elapsed -= 0x10;
                }
                return;
            }
            if (pQatt > -5) {
                set_screen_frame(8, -pQatt - 1);
                pQatt = -5;
            }
            return;
        }
        if ((frame & 0xE0) != 0x80)
            return;
        if (pQatt <= -10)
            return;
        set_screen_frame(3, 0);
        charge = wd[5] - wd[3];
        charge = charge * play_pow / 100;
        play_pow = wd[3] + charge;
        playerdat->noise = 15;
        power = play_pow;
        DoPlayerWeapon(wd, curr_weapon, swing_state);
        if (do_attack() && specweap > 0 && (criti || specweap == 6)) {
            tx = -1;
            ty = -1;
            target = Obj_IntTMem(hitobj);
            if (IsMobElem(target)) {
                tx = OBJ_HOMEX(target);
                ty = OBJ_HOMEY(target);
                tile = Map_GetAddr(tx, ty);
            }
            switch (specweap) {
            case 1:
                if (tx > 0)
                    get_hp_back(ThePlayer, ddone);
                break;
            case 2:
                if (tx >= 0)
                    sp_ward_undead(tx, ty, target, tile, 1);
                break;
            case 3:
                if (tx > 0) {
                    obj = build_new_obj(0x1C2, tile);
                    if (add_animobj(Obj_MemTPtr(obj), 4, 0, tx, ty) == -1)
                        Obj_Free(obj);
                    else {
                        Obj_Add(&tile->objects, obj);
                        fireball_effect(obj, tx, ty);
                    }
                    damage_square(tx, ty, 1, 1);
                }
                break;
            case 4:
                if (tx >= 0)
                    sp_hold(tx, ty, target, tile, 1);
                break;
            case 5:
            case 6:
                if (OBJ_CLASS(target) == CLASS_DOOR) {
                    ObjectActorArg = 0xC;
                    obj_spells(target, 0, 0);
                    OpenDoor(ThePlayer, target);
                }
            }
        }
        specweap = 0;
        using_altaras_dagger = 0;
        pQatt = -10;
        return;
    }
    if (!player->drawn)
        return;
    if (swing == 0)
        return;
    if (weap_frame != -1)
        return;
    swing_state = swing;
    if ((tx = GetPlayerWeapon(&weapon_data, &curr_weapon)) < 0)
        return;
    fire_mode = tx == 0;
    if (fire_mode != 0)
        swing_state = -1;
    pQatt = -1 - swing_kind[swing - 1];
    attackKey = swing_keys[swing / 3 - 1];
    set_screen_frame(8, -pQatt - 1);
    set_screen_frame(3, 1);
    play_pow = 0;
    elapsed = -1;
}

void far missile_thwack(int attacker, struct Object far *missile, struct Object far *def,
                        int x, int y, int dmg, unsigned char type)
{
    targx = x;
    targy = y;
    criti = 0;
    hitangle = 0;
    hitz = OBJ_Z(missile) + ComObjData[OBJ_ITEM(missile)].height / 2;
    power = 0x80;
    fromwho = attacker;
    hitobj = Obj_MemTPtr(def);
    hitloc = pickloc(OBJ_Z(def), OBJ_Z(def) + ComObjData[OBJ_ITEM(def)].height,
                     OBJ_Z(missile), OBJ_Z(missile) + ComObjData[OBJ_ITEM(missile)].height) + 4;
    damage = dmg;
    if (def == ThePlayer)
        play_effect_here(3, 0, 0);
    else if (IsMobElem(def))
        play_effect_on_mobile(4, def, 0);
    missile_hit = 1;
    do_damage(type);
    missile_hit = 0;
}

char far critter_attack(struct Object far *npc, int swing, unsigned char charge, int type,
                        int poison)
{
    char result;
    struct Creature *cr;

    wsize = 2;
    fromwho = Obj_MemTPtr(npc);
    towhere = swing;
    power = charge;
    cr = &Creature[npc->id & ID_INMAJOR];
    damage = cr->attacks[type].damage;
    damage += Creature[npc->id & ID_INMAJOR].attr[0] / 5;
    askill = cr->attacks[type].chance + (cr->equip >> 1);
    if (OBJ_POWERFUL(npc)) {
        askill += rand() % 6 + 7;
        damage += rand() % 12 + 4;
    }
    result = do_attack();
    if (result && hitobj == 1 && player->poison < poison) {
        if (rand() % (poison + 6) > Creature[63].armour[hitloc % 4] << 1) {
            if (check_res(ThePlayer, 1, 0x10))
                player->poison = poison;
        }
    }
    return result;
}

/* DOS only: no FM Towns counterpart between critter_attack and player_killed_a. */
void far seg024_24E9_1AC7(void)
{
    if (pQatt > 0) {
        pQatt = 0;
        attackKey = -1;
        set_screen_frame(3, 0);
        set_screen_frame(8, 4);
    }
}

void far player_killed_a(struct Object far *npc)
{
    int exp;

    if (OBJ_MAJOR(npc) == MAJOR_CREATURE) {
        set_new_music(6);
        exp = Creature[npc->id & ID_INMAJOR].exp;
        exp = exp * 4 + rollem(2, exp);
        if (OBJ_POWERFUL(npc))
            exp = (long)exp * (rand() % 24 + 24) / 16;
        player_get_exp(exp);
    }
}
