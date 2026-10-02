/* target: seg030_2BB7 */
/* opts: -mm -1 -G -O -d */
/* Object physics: bouncing, missile hits, collisions between objects, copying an object
   to and from the physics record, moving objects between the static and mobile lists,
   and settling a placed object into its tile. The whole of DOS resident segment
   seg030_2BB7, in original order. Function and global names are the originals from the
   FM Towns symbol table; the source file's own name is not known. */

#include "combat.h"
#include "critter.h"
#include "map.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "ui.h"

#define OBJ_ITEM(o)     ((o)->id & ID_ITEM)
#define OBJ_MAJOR(o)    (((o)->id & ID_MAJOR) >> 6)
#define OBJ_CLASS(o)    (((o)->id & ID_CLASS) >> 4)
#define OBJ_MINOR4(o)   ((o)->id & ID_INCLASS)
#define OBJ_Z(o)        ((o)->pos & POS_Z)
#define OBJ_HEADING(o)  (((o)->pos & POS_HEADING) >> 7)
#define OBJ_FINEY(o)    (((o)->pos & POS_YFINE) >> 10)
#define OBJ_FINEX(o)    (((o)->pos & POS_XFINE) >> 13)
#define OBJ_HOMEX(o)    (((o)->home & HOME_X) >> 10)
#define OBJ_HOMEY(o)    (((o)->home & HOME_Y) >> 4)
#define ITEM_CLASS(i)   (((i) & ID_CLASS) >> 4)
#define OBJ_TERRAIN(o)  (((o)->b0A & 0x70) >> 4)

#define SET_Z(o, v)       ((o)->pos = (o)->pos & 0xFF80 | (v) & 0x7F)
#define SET_HEADING(o, v) ((o)->pos = (o)->pos & 0xFC7F | ((v) & 7) << 7)
#define SET_FINEX(o, v)   ((o)->pos = (o)->pos & 0x1FFF | ((unsigned)(v) & 7) << 13)
#define SET_FINEY(o, v)   ((o)->pos = (o)->pos & 0xE3FF | (v) << 10)
#define SET_HOMEX(o, v)   ((o)->home = (o)->home & 0x3FF | ((v) & 0x3F) << 10)
#define SET_HOMEY(o, v)   ((o)->home = (o)->home & 0xFC0F | ((v) & 0x3F) << 4)
#define SET_SPEED(o, v)   ((o)->b13 = (o)->b13 & 0x80 | ((v) & 0x7F) << 0)
#define SET_GRAVITY(o, v) ((o)->b13 = (o)->b13 & 0x7F | ((v) & 1) << 7)
#define SET_PITCH(o, v)   ((o)->b14 = (o)->b14 & 7 | ((v) & 0x1F) << 3)
#define SET_TERRAIN(o, v) ((o)->b0A = (o)->b0A & 0x8F | ((v) & 7) << 4)

extern struct Object far *objdata;
extern struct MissileInfo Missile[];
extern unsigned char curBin;

/* This file's _BSS, DS:25BC..25C2 (seg031's starts at 25C4, word-aligned). No FM Towns
   names: FM Towns keeps them as statics (inside its static block after _Valor, +0x4F..+0x55,
   in a different order), so they are static, with provisional names whose keys
   (tools/bssorder.py) lay them out as UW2 has them: objhit_used 87, objhit_tilex and
   objhit_tiley 151, objhit_myx and objhit_myy 183, deal_bounced and deal_blocked 908. */
static unsigned char objhit_used;           /* DS:25BC */
static unsigned char objhit_tilex, objhit_tiley;    /* DS:25BD, the other object's tile */
static unsigned char objhit_myx, objhit_myy; /* DS:25BF, the moving object's tile */
static unsigned char deal_bounced;          /* DS:25C1 */
static unsigned char deal_blocked;          /* DS:25C2 */

/* Elsewhere in the game. */
int far rand(void);
struct Object far * far Obj_IntTMem(int index);
struct Object far * far Obj_PtrTMem(unsigned far *link);
void far missile_thwack(int who, struct Object far *proj, struct Object far *hit,
                        int x, int y, int damage, char type);
void far UseObj(struct Object far *who, struct Object far *obj, int how);
int far UseTrigger(struct Object far *who, void far *a, struct Object far *trig, int b);
char far Obj_Rem(unsigned far *head, struct Object far *obj);
void far Obj_Add(unsigned far *head, struct Object far *obj);
void far check_pplate(struct Object far *obj, struct Tile far *tile, int z, int how);
void far play_effect_on_mobile(int fx, struct Object far *obj, int vol);
char far damage_item(struct Object far *obj, struct Object far *who, int x, int y,
                     unsigned char damage, unsigned char type);
struct Object far * far Obj_Alloc(int mobile);
struct Object far * far Obj_Punt(unsigned far *head, struct Object far *obj, int a);
unsigned char far decode_obj_spell(struct Object far *obj, int *major, int *minor, unsigned char *flag);
void far put_effect(struct Object far *obj, int type, int size, int a, int b, int x, int y);
void far TerrainCheck(char radius);
void far ObjectCheck(char a, int b);

/* Later in this file. */
struct Object far * far obj_deal(struct Object far *obj, int x, int y, char how);

int far bounce_obj(struct Object far *obj, struct Object far *other)
{
    struct Phys phys;
    struct Phys *pp = &phys;
    int scale;

    if (other != 0) {
        XP = objhit_tilex;
        YP = objhit_tiley;
        get_phys_data(other, pp);
        if (pp->mass != 0) {
            scale = (CP->mass << 6) / pp->mass;
            if (scale > 0x80)
                scale = 0x80;
            pp->heading = CP->heading;
            pp->speed = 0xEB;
            pp->vel[2] = CP->vel[2] * scale / 0x40;
            XP = objhit_tilex;
            YP = objhit_tiley;
            set_phys_data(other, pp);
        } else if (pp->speed != 0)
            pp->speed = 0;
    }
    return 4;
}

void far missile_newhit(struct Object far *proj, struct Object far *hit)
{
    unsigned damage;
    int x;
    int y;
    struct MissileInfo *mi;
    int scale;

    if (OBJ_ITEM(proj) == ITEM_SATELLITE && Obj_MemTPtr(hit) == proj->last_hit)
        proj->b15 = proj->b15 & 0x7F;
    else {
        mi = &Missile[proj->id & ID_INCLASS];
        damage = mi->damage;
        if (proj->last_hit == 1 && mi->ammo == -64) {
            scale = (player->skills[SKILL_MISSILE] << 3) + 0xC0;
            switch (skill_check(player->skills[SKILL_MISSILE], 10)) {
            case -1:
                scale -= 0x80;
                break;
            case 2:
                scale += 0xC0;
                break;
            }
            damage = damage * scale >> 8;
        }
        if (objhit_used) {
            x = objhit_tilex;
            y = objhit_tiley;
        } else {
            x = objhit_myx;
            y = objhit_myy;
        }
        missile_thwack(proj->last_hit, proj, hit, x, y, damage, -mi->ammo);
        if (OBJ_MAJOR(hit) == MAJOR_CREATURE)
            proj->b0A = proj->b0A & 0x7F | 0x80;
    }
}

int far do_objhit(int ci, int index)
{
    int item;
    struct Object far *obj;
    struct Object far *other;
    char touch;
    int my_item;

    if (ci != -1) {
        if (oCollisions[ci].link.f.low & 0x20)
            return 2;
        oCollisions[ci].link.f.low = oCollisions[ci].link.f.low | 0x20;
    }
    objhit_myx = Ppd.x >> 3;
    objhit_myy = Ppd.y >> 3;
    obj = Obj_IntTMem(index);
    my_item = OBJ_ITEM(obj);
    if (ci == -1) {
        other = 0;
        item = -1;
        touch = 1;
    } else {
        other = Obj_IntTMem(oCollisions[ci].link.f.index);
        item = oCollisions[ci].offset & 0x3F;
        objhit_tilex = objhit_myx + item & 0x3F;
        item = objhit_tilex - objhit_myx;
        objhit_tiley = objhit_myy + (oCollisions[ci].offset - item) / 0x40 & 0x3F;
        item = OBJ_ITEM(other);
        touch = ComObjData[item].touch;
        if (oCollisions[ci].link.f.index < NUM_MOBILE && index < NUM_MOBILE) {
            if (my_item >> 6 != MAJOR_CREATURE && (unsigned char)((obj->b15 & 0x80) >> 7)
                && my_item != ITEM_FIREBALL_1D && my_item != ITEM_RESILIENT_SPHERE_13F)
                return 2;
            if (my_item >> 6 != MAJOR_CREATURE)
                obj->b15 = obj->b15 & 0x7F | 0x80;
        }
    }
    if (item != -1) {
        if (ComObjData[item].usable) {
            MapObj_X = objhit_tilex;
            MapObj_Y = objhit_tiley;
            objhit_used = 0;
            UseObj(obj, other, 0);
        } else if ((ITEM_CLASS(item) & 0x1E) == CLASS_TRIGGER)
            return UseTrigger(obj, 0L, other, 0);
    }
    if (touch) {
        if (ComObjData[my_item].usable) {
            MapObj_X = objhit_myx;
            MapObj_Y = objhit_myy;
            objhit_used = 1;
            UseObj(other, obj, 0);
            if (MapObj_X < 0)
                return 0x10;
        }
        return bounce_obj(obj, other);
    }
    return 2;
}

void far get_phys_data(struct Object far *obj, struct Phys *pp)
{
    char jitter = 1;
    struct ComObj *co;

    co = &ComObjData[OBJ_ITEM(obj)];
    pp->index = Obj_MemTPtr(obj);
    pp->mass = co->mass;
    pp->light = co->light;
    pp->bounce = co->bounce;
    pp->resist = co->resist;
    pp->b1D = 0;
    pp->heading = OBJ_HEADING(obj) << 13;
    pp->b24 = 0;
    pp->radius = co->radius;
    pp->height = co->height;
    pp->x = OBJ_FINEX(obj);
    pp->y = OBJ_FINEY(obj);
    pp->z = OBJ_Z(obj);
    if (obj < objdata) {
        pp->x = pp->x + (OBJ_HOMEX(obj) << 3);
        pp->y = pp->y + (OBJ_HOMEY(obj) << 3);
        pp->heading = obj->heading << 8;
        pp->terrain = 1 << OBJ_TERRAIN(obj);
        pp->vel[2] = (((obj->b14 & 0xF8) >> 3) - 0x10) << 6;
        pp->acc[2] = ((obj->b13 & 0x80) >> 7) * -4;
        pp->hp = obj->hp;
        if (OBJ_MAJOR(obj) != MAJOR_CREATURE) {
            jitter = 0;
            pp->x = obj->goal_word;
            pp->y = obj->attitude_word;
            pp->z = obj->b0F;
        }
        pp->speed = obj->b13 & 0x7F;
        if (OBJ_MAJOR(obj) != MAJOR_CREATURE && (pp->acc[2] | pp->vel[2]) == 0 && !co->no_hit) {
            if (pp->light * 2 + 2 >= pp->speed)
                pp->speed = 0;
            else
                pp->speed = (obj->b13 & 0x7F) * ((pp->light << 2) + 0x29);
        } else {
            pp->speed *= 0x2F;
            if (OBJ_MAJOR(obj) == MAJOR_CREATURE)
                pp->b24 = 8;
        }
    } else {
        pp->speed = pp->acc[2] = pp->vel[2] = 0;
        pp->hp = obj->qn.f.quality;
        pp->x += XP << 3;
        pp->y += YP << 3;
    }
    if (jitter) {
        pp->x = (pp->x << 5) + (rand() & 0x1F);
        pp->y = (pp->y << 5) + (rand() & 0x1F);
        pp->z = (pp->z << 3) + (rand() & 7);
    }
    pp->impact = 0;
}

unsigned char far set_phys_data(struct Object far *obj, struct Phys *pp)
{
    int xmoved = 0;
    int ymoved = 0;

    if (pp->x >> 8 != XP)
        xmoved = 1;
    if (pp->y >> 8 != YP)
        ymoved = 1;
    if (xmoved || ymoved) {
        struct Tile far *tile;
        unsigned far *head;

        tile = Map_GetAddr(XP, YP);
        head = &tile->objects.word;
        Obj_Rem(head, obj);
        check_pplate(obj, tile, OBJ_Z(obj), 0xE);
        XP = pp->x >> 8;
        YP = pp->y >> 8;
        tile = Map_GetAddr(XP, YP);
        head = &tile->objects.word;
        Obj_Add(head, obj);
        SET_Z(obj, pp->z >> 3);
        check_pplate(obj, tile, OBJ_Z(obj), 6);
    } else if (OBJ_Z(obj) != pp->z >> 3)
        hgt_change(obj, Map_GetAddr(XP, YP), pp->z >> 3);
    SET_FINEX(obj, pp->x >> 5 & 7);
    SET_FINEY(obj, pp->y >> 5 & 7);
    if (obj < objdata)
        obj->hp = pp->hp;
    else
        obj->qn.f.quality = pp->hp;
    if (pp->impact > 0x180) {
        int mass;
        int damage;

        damage = pp->impact >> 8;
        if (OBJ_MAJOR(obj) == MAJOR_CREATURE)
            damage >>= 2;
        else if (obj->hp < 0x20)
            damage <<= 2;
        mass = ComObjData[OBJ_ITEM(obj)].mass;
        if (OBJ_MAJOR(obj) != MAJOR_CREATURE)
            play_effect_on_mobile(0xF, obj, (mass - 600) / 50);
        damage_item(obj, 0L, XP, YP, damage, 0);
    }
    if (pp->terrain & 4 && rand() % 5 == 0)
        damage_item(obj, 0L, XP, YP, 1, 8);
    if (OBJ_MAJOR(obj) != MAJOR_CREATURE) {
        if (obj > objdata) {
            if (pp->speed | pp->vel[2] | pp->acc[2])
                obj = static_to_mob(obj);
        } else if (!(pp->speed | pp->vel[2] | pp->acc[2])) {
            SET_TERRAIN(obj, res_to_terr[pp->terrain]);
            if ((obj = mob_to_static(obj)) == 0)
                return 0;
            if ((obj = obj_deal(obj, XP, YP, 0)) == 0)
                return 0;
            if (obj < objdata)
                update_hack_vecs(pp);
        }
    }
    if (obj < objdata) {
        register int v;

        obj->heading = pp->heading >> 8;
        SET_HOMEX(obj, XP);
        SET_HOMEY(obj, YP);
        SET_GRAVITY(obj, pp->acc[2] != 0);
        v = pp->vel[2] / 0x40 + 0x10;
        if (v < 0)
            v = 0;
        else if (v > 0x1F)
            v = 0x1F;
        SET_PITCH(obj, v);
        SET_SPEED(obj, pp->speed / 0x2F);
        SET_TERRAIN(obj, res_to_terr[pp->terrain]);
        if (OBJ_MAJOR(obj) != MAJOR_CREATURE) {
            obj->goal_word = pp->x;
            obj->attitude_word = pp->y;
            obj->b0F = pp->z;
        }
        return 1;
    }
    if (OBJ_MAJOR(obj) == MAJOR_RECT)
        SET_HEADING(obj, pp->heading >> 13);
    return 0;
}

struct Object far * far static_to_mob(struct Object far *obj)
{
    struct Object far *mob;
    struct Tile far *tile;
    unsigned far *head;

    tile = Map_GetAddr(XP, YP);
    head = &tile->objects.word;
    if ((mob = Obj_Alloc(1)) != 0) {
        *(struct StaticObj far *)mob = *(struct StaticObj far *)obj;
        mob_init(mob, XP, YP);
        mob->hp = obj->qn.f.quality;
        if (OBJ_MAJOR(obj) != MAJOR_RECT && ComObjData[OBJ_ITEM(obj)].render != 2)
            mob->whoami = OBJ_HEADING(obj);
        if (OBJ_MAJOR(mob) == MAJOR_ANIMOBJ)
            Change_AnimPtr(mob, obj);
        Obj_Rem(head, obj);
        Obj_Free(obj);
        Obj_Add(head, mob);
    }
    return mob;
}

void far mob_init(struct Object far *obj, int x, int y)
{
    obj->heading = OBJ_HEADING(obj) << 5;
    obj->b18 = obj->b18 & 0xE0;
    obj->b14 = obj->b14 & 7 | 0x80;
    SET_GRAVITY(obj, ComObjData[OBJ_ITEM(obj)].no_hit ? 0 : 1);
    SET_HOMEX(obj, x);
    SET_HOMEY(obj, y);
    obj->b0A = obj->b0A & 0xF0 | (curBin + 1 & 0xF) << 0;
    obj->b14 = obj->b14 & 0xF8 | 2;
    obj->b13 = obj->b13 & 0x80;
    obj->id = obj->id & 0xBFFF;
    obj->hp = 0x3F;
    obj->b0A = obj->b0A & 0x8F;
    if (OBJ_MAJOR(obj) != MAJOR_CREATURE) {
        obj->goal_word = (x << 8) + (OBJ_FINEX(obj) << 5) + 0xF;
        obj->attitude_word = (y << 8) + (OBJ_FINEY(obj) << 5) + 0xF;
        obj->b0F = OBJ_Z(obj) << 3;
        obj->last_hit = 0;
    }
}

void far do_filanium(struct Object far *obj, int x, int y)
{
    struct Tile far *tile;
    int major;
    int minor;
    unsigned char flag;

    tile = Map_GetAddr(x, y);
    if (TxmID[tile->floor] != 0xC1 || !decode_obj_spell(obj, &major, &minor, &flag)
        || !flag || major != 0xD || minor != 3)
        return;
    game_sprint(0x14C);
    if (player->xclock[XC_DJINN] < 2)
        player->xclock[XC_DJINN] = 2;
}

struct Object far * far mob_to_static(struct Object far *obj)
{
    struct Object far *st;
    struct Tile far *tile;
    unsigned far *head;
    char keep = 1;
    char who;
    unsigned fate;

    fate = ComObjData[OBJ_ITEM(obj)].fate;
    if (OBJ_TERRAIN(obj) == 1) {
        fate = 8;
        put_effect(obj, 6, 3, 0, 0, XP, YP);
        do_filanium(obj, XP, YP);
    }
    if (fate > 0 && fate <= 8 && (rand() & 7) < fate && Obj_Elem_Fate(10, obj))
        keep = 0;
    tile = Map_GetAddr(XP, YP);
    head = &tile->objects.word;
    if (keep && (st = Obj_Alloc(0)) != 0) {
        *(struct StaticObj far *)st = *(struct StaticObj far *)obj;
        obj->ol.f.link = 0;
        if (OBJ_MAJOR(st) == MAJOR_ANIMOBJ)
            Change_AnimPtr(st, obj);
        else if (OBJ_CLASS(st) == CLASS_LIGHT && OBJ_MINOR4(st) >= 4 && OBJ_MINOR4(st) <= 6)
            st->id = st->id & 0xFFF0 | OBJ_MINOR4(st) - 4 & 0xF;
        st->qn.f.quality = obj->hp;
        if (OBJ_MAJOR(st) != MAJOR_RECT && OBJ_MAJOR(st) != MAJOR_TRAP && ComObjData[OBJ_ITEM(st)].render != 2)
            SET_HEADING(st, obj->whoami);
    } else
        st = 0;
    if (fate == 9) {
        if (OBJ_MAJOR(obj) == MAJOR_CREATURE)
            who = 0;
        else
            who = obj->last_hit;
    }
    Obj_Punt(head, obj, 1);
    if (st != 0) {
        Obj_Add(head, st);
        check_pplate(st, tile, 0, 7);
    }
    if (fate == 9 && !mts_doanim(st, XP, YP, who)) {
        st = Obj_Punt(head, st, 0);
        return st;
    }
    return st;
}

/* IDA seg030_2BB7_107A. The map pairs it with FM Towns update_hack_vecs_ by size and
   position only; the code agrees (a byte flag picks a random turn of the heading with
   speed 0xBC, or a random speed (rand()+1 & 3) * 0x2F with gravity -4), so the
   original name is used. */
void far update_hack_vecs(struct Phys *pp)
{
    if (deal_bounced) {
        pp->heading = pp->heading + (rand() & 0x3FFF) - 0x2000;
        pp->speed = 0xBC;
    } else {
        pp->speed = (rand() + 1 & 3) * 0x2F;
        pp->acc[2] = -4;
    }
}

struct Object far * far obj_deal(struct Object far *obj, int x, int y, char how)
{
    char destroy;
    unsigned char retried;
    char low;
    int oldx;
    int oldy;
    struct MotionCalc calc;
    struct ComObj *co;

    destroy = 0;
    retried = 0;
    low = 0;
    deal_blocked = deal_bounced = 0;
again:
    curP = &calc;
    curP->index = Obj_MemTPtr(obj);
    co = &ComObjData[OBJ_ITEM(obj)];
    if (co->no_hit)
        return obj;
    curP->radius = co->radius;
    curP->height = co->height;
    curP->z = OBJ_Z(obj);
    curP->x = (x << 3) + OBJ_FINEX(obj);
    curP->y = (y << 3) + OBJ_FINEY(obj);
    TerrainCheck(curP->radius);
    if (curP->floor + curP->radius >= curP->z && !retried)
        low = 1;
    else
        low = 0;
    ObjectCheck(low, 1);
    process_objlist();
    MP.hit = 0xFF;
    if (curP->count == 0 && curP->first > 0 && curP->first <= curP->found) {
        for (curP->first--; curP->first >= 0; curP->first--) {
            if (oCollisions[curP->first].top == curP->z) {
                MP.hit = curP->first;
                MP.item = OBJ_ITEM(Obj_PtrTMem(&oCollisions[MP.hit].link.word));
                if (ComObjData[MP.item].solid == 1) {
                    if (oCollisions[MP.hit].link.f.low & 0x10) {
                        deal_blocked = 1;
                        break;
                    }
                    deal_bounced = 1;
                }
            } else
                break;
        }
    }
    if ((curP->hits0 | curP->hits1) & 0x300 || curP->count)
        destroy = 1;
    else if ((curP->hits0 & 7) == 5) {
        put_effect(obj, 6, 3, 0, 0, x, y);
        do_filanium(obj, x, y);
        destroy = 1;
    } else if ((curP->hits0 & 7) == 6) {
        if (ComObjData[OBJ_ITEM(obj)].qualclass == 3)
            destroy = 0;
        else
            destroy = check_res(obj, 1, 8);
    } else if ((curP->hits0 & 7) == 7)
        ;
    else if (!(curP->hits0 & 8) && deal_blocked == 0) {
        if (low) {
            retried = 1;
            goto again;
        }
        oldx = XP;
        oldy = YP;
        XP = x;
        YP = y;
        obj = static_to_mob(obj);
        XP = oldx;
        YP = oldy;
        if (deal_bounced) {
            SET_SPEED(obj, 3);
            obj->heading = obj->heading + (rand() % 9 << 4) + 0xC0;
        } else if (how) {
            SET_SPEED(obj, (rand() & 3) + 1);
            SET_PITCH(obj, (rand() & 3) + 0xE);
        }
    }
    if (destroy)
        return Obj_Punt(&Map_GetAddr(x, y)->objects.word, obj, 0);
    return obj;
}
