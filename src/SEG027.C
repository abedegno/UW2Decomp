/* target: seg027_2856 */
/* opts: -mm -1 -G -O -d */
/* Missiles: aiming from the mouse, firing by the player, critters, spells and traps,
   throwing and dropping objects, and launching a missile into the world. The whole of
   DOS segment seg027_2856, in original order. Function and global names are the
   originals from the FM Towns symbol table; the source file's own name is not known. */

/* The player's record, reached through the near pointer `player`. */
struct Player {
    char pad0[0x306];
    unsigned char motion_state;         /* 0x306 */
    unsigned char f307;                 /* 0x307 */
};

/* A mobile object. The first 8 bytes are shared with static objects. */
struct Object {
    unsigned id;                        /* item 0-8 (major class 6-8), flags 9-12, 13, is_quant 15 */
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
    unsigned char heading;              /* 0x09 */
    unsigned char b0A;                  /* 0x0A */
    int proj_x;                         /* 0x0B, a missile's position in 1/256 tiles */
    int proj_y;                         /* 0x0D */
    int proj_z;                         /* 0x0F */
    char pad11[0x12 - 0x11];
    unsigned char last_hit;             /* 0x12 */
    unsigned char b13;                  /* 0x13, missile type in bits 0-6 */
    unsigned char b14;                  /* 0x14 */
    unsigned char anim;                 /* 0x15 */
    unsigned home;                      /* 0x16, x in bits 10-15, y in bits 4-9 */
    unsigned char b18;                  /* 0x18, fine heading in bits 0-4 */
    char pad19[0x1A - 0x19];
    unsigned char whoami;               /* 0x1A */
};

#define OBJ_ITEM(o)     ((o)->id & 0x1FF)
#define OBJ_MAJOR(o)    (((o)->id & 0x1C0) >> 6)
#define OBJ_CLASS(o)    (((o)->id & 0x1F0) >> 4)
#define OBJ_MINOR4(o)   ((o)->id & 0xF)
#define OBJ_FLAGS(o)    (((o)->id & 0x1E00) >> 9)
#define OBJ_BIT13(o)    (((o)->id & 0x2000) >> 13)
#define OBJ_ISQUANT(o)  (((o)->id & 0x8000) >> 15)
#define OBJ_Z(o)        ((o)->pos & 0x7F)
#define OBJ_HEADING(o)  (((o)->pos & 0x380) >> 7)
#define OBJ_FINEY(o)    (((o)->pos & 0x1C00) >> 10)
#define OBJ_FINEX(o)    (((o)->pos & 0xE000) >> 13)
#define OBJ_HOMEX(o)    (((o)->home & 0xFC00) >> 10)
#define OBJ_HOMEY(o)    (((o)->home & 0x3F0) >> 4)

#define SET_ISQUANT(o, v) ((o)->id = (o)->id & 0x7FFF | ((v) & 1) << 15)
#define SET_FLAGS(o, v)   ((o)->id = (o)->id & 0xE1FF | ((v) & 0xF) << 9)
#define SET_BIT13(o, v)   ((o)->id = (o)->id & 0xDFFF | ((v) & 1) << 13)
#define SET_Z(o, v)       ((o)->pos = (o)->pos & 0xFF80 | (v) & 0x7F)
#define SET_FINEX(o, v)   ((o)->pos = (o)->pos & 0x1FFF | ((unsigned)(v) & 7) << 13)
#define SET_FINEY(o, v)   ((o)->pos = (o)->pos & 0xE3FF | (v) << 10)

struct Tile {
    unsigned type:4;
    unsigned height:4;
    char pad1;
    unsigned objects;                   /* 0x02, head of the tile's object list */
};

struct Inplist {
    char pad0[8];
    int field8;
};

/* The common object properties, one 11-byte record per item. */
struct ComObj {
    unsigned height:8;                  /* 0x00 */
    unsigned radius:3;
    unsigned c0_11:5;
    char pad2[6 - 2];
    unsigned c6:15;                     /* 0x06 */
    unsigned no_owner:1;
    char pad8;
    unsigned char c9:2;                 /* 0x09 */
    unsigned char c9_2:6;
    char padA;
};

/* A missile weapon: the ammunition it fires and the missile type. */
struct MissileInfo {
    char a;
    unsigned char type;
    signed char ammo;
};

/* The motion calculation record, reached through `curP`. */
struct MotionCalc {
    int x, y, z;                        /* 0x00 */
    char pad6[8 - 6];
    unsigned char radius;               /* 0x08 */
    unsigned char height;               /* 0x09 */
    int index;                          /* 0x0A */
    unsigned hits0, hits1;              /* 0x0C */
    char pad10[0x14 - 0x10];
    unsigned char b14;                  /* 0x14 */
    unsigned char b15;                  /* 0x15 */
    char pad16[0x18 - 0x16];
};

extern struct Player near *player;
extern struct Object far *ThePlayer;
extern struct Object far *objdata;
extern struct Inplist near *inplist;
extern struct ComObj ComObjData[];
extern struct MissileInfo Missile[];
extern struct MotionCalc near *curP;
extern int PlayerPitch;
extern unsigned char inanmMapX, inanmMapY;
extern unsigned char using_bow;
extern unsigned char magical_missile;
extern int missile_trx, missile_try;

/* The missile being launched. No FM Towns names: static there. */
extern int missile_type;
extern int missile_x, missile_y;
extern int missile_item;
extern struct Object far *missile_src;
extern int missile_heading;

/* Elsewhere in the game. */
void far mouse_getxy(int *x, int *y);
int far check_ammo(int weapon);
char far play_effect_here(int fx, int vol, int c);
void far update_digi_playback(void);
struct Object far * far RemoveOneFromSlot(int a, int b, int item, int slot);
void far Obj_Free(struct Object far *obj);
void far game_sprint(int id);
void far play_effect_on_mobile_src(int fx, struct Object far *obj, int vol);
void far play_effect_on_mobile(int fx, struct Object far *obj, int vol);
void far move_along(int heading, int dist, int *x, int *y);
unsigned char far can_place(int item, int a, int x, int y, int z, int b, char dist);
struct Tile far * far Map_GetAddr(int x, int y);
void far Obj_AddEnd(unsigned far *list, struct Object far *obj);
struct Object far * far obj_deal(struct Object far *obj, int x, int y, int a);
void far check_pplate(struct Object far *obj, struct Tile far *tile, int z, int how);
struct Object far * far CreateObj(int id, int b);
void far mob_init(struct Object far *obj, int x, int y);
int far Obj_MemTPtr(struct Object far *obj);
void far Obj_Add(unsigned far *list, struct Object far *obj);
void far ObjectCheck(int a, int b);
void far TerrainCheck(int a);
void far process_objlist(void);

/* Later in this file. */
struct Object far * far missile_fire(void);
unsigned char far push_missile(struct Object far *proj, struct Object far *src, char launch);

char far player_settr(void)
{
    int x, y;

    mouse_getxy(&x, &y);
    if ((x -= 0x10) > 0xD0)
        x = 0xD0;
    else if (x < 0)
        x = 0;
    if ((y -= 0x36) > 0x80)
        y = 0x80;
    else if (y < 0)
        y = 0;
    missile_trx = (x - 0x68) * 5 / 0xD + 1;
    missile_try = PlayerPitch / 0x300;
    missile_try += (y - 0x40) / 6;
    return y >= 0x2A;
}

void far player_fire(int weapon)
{
    int slot;
    struct Object far *ammo_obj;
    struct Object far *proj;
    char fired;
    int ammo;

    fired = 0;
    if (weapon == 9 || weapon == 10 || weapon == 8)
        using_bow = 1;
    else
        using_bow = 0;
    if ((slot = check_ammo(weapon)) >= 0) {
        ammo = Missile[weapon].ammo;
        missile_type = Missile[ammo].type;
        missile_item = ammo + 0x10;
        missile_x = OBJ_HOMEX(ThePlayer);
        missile_y = OBJ_HOMEY(ThePlayer);
        missile_heading = 1;
        missile_src = ThePlayer;
        player_settr();
        if ((proj = missile_fire()) != 0) {
            if (using_bow && weapon != 8) {
                if (play_effect_here(9, 0x40, 0) != -1)
                    update_digi_playback();
            }
            fired = 1;
            ammo_obj = RemoveOneFromSlot(0, 1, ammo, slot);
            SET_ISQUANT(proj, OBJ_ISQUANT(ammo_obj));
            proj->ol.f.link = ammo_obj->ol.f.link;
            SET_FLAGS(proj, OBJ_FLAGS(ammo_obj));
            proj->hp = ammo_obj->qn.f.quality;
            proj->ol.f.owner = ammo_obj->ol.f.owner;
            SET_BIT13(proj, OBJ_BIT13(ammo_obj));
            if (OBJ_MAJOR(ammo_obj) != 5 && ComObjData[OBJ_ITEM(ammo_obj)].c9 != 2)
                proj->whoami = OBJ_HEADING(ammo_obj);
            Obj_Free(ammo_obj);
        } else {
            play_effect_here(0x2D, 0x40, 0);
            game_sprint(0x10E);
        }
        if ((using_bow || weapon == 8) && fired)
            play_effect_on_mobile_src(0xA, proj, 0x14);
        using_bow = 0;
    }
}

void far critter_fire(struct Object far *who, int item, int type)
{
    struct Object far *proj;

    missile_item = item + 0x10;
    missile_type = type;
    missile_x = OBJ_HOMEX(who);
    missile_y = OBJ_HOMEY(who);
    missile_heading = 1;
    missile_src = who;
    missile_trx = 0;
    using_bow = 1;
    proj = missile_fire();
    using_bow = 0;
    if (proj != 0)
        play_effect_on_mobile_src(0xA, proj, 0x14);
}

char far spell_fire(struct Object far *who, int spell)
{
    struct Object far *proj;

    missile_item = spell + 0x10;
    missile_type = Missile[spell].type;
    missile_x = OBJ_HOMEX(who);
    missile_y = OBJ_HOMEY(who);
    missile_heading = 1;
    missile_src = who;
    if (who == ThePlayer)
        player_settr();
    else {
        if (who >= objdata) {
            missile_x = inanmMapX;
            missile_y = inanmMapY;
            missile_try = 0;
            missile_heading = 0;
        }
        missile_trx = 0;
    }
    magical_missile = 1;
    proj = missile_fire();
    if (proj != 0) {
        switch (spell) {
        case 6:
        case 12:
            play_effect_on_mobile_src(0xA, proj, 0);
            break;
        default:
            play_effect_on_mobile(0x28, proj, 0x14);
        }
    }
    magical_missile = 0;
    return proj != 0;
}

char far ReturnObject(struct Object far *obj, char message)
{
    struct Object far *thrown;
    struct Tile far *tile;
    int x;
    int y;
    unsigned char dist;
    unsigned char cannot;
    struct Object far *hit;
    int tx;
    int ty;

    missile_x = OBJ_HOMEX(ThePlayer);
    missile_y = OBJ_HOMEY(ThePlayer);
    if (inplist->field8 == 1 && player_settr()) {
        missile_heading = 1;
        missile_src = ThePlayer;
        missile_item = OBJ_ITEM(obj);
        missile_type = 0xF;
        if ((thrown = missile_fire()) != 0) {
            SET_ISQUANT(thrown, OBJ_ISQUANT(obj));
            thrown->ol.f.link = obj->ol.f.link;
            SET_FLAGS(thrown, OBJ_FLAGS(obj));
            thrown->hp = obj->qn.f.quality;
            thrown->ol.f.owner = obj->ol.f.owner;
            SET_BIT13(thrown, OBJ_BIT13(obj));
            if (OBJ_MAJOR(obj) != 5 && ComObjData[OBJ_ITEM(obj)].c9 != 2)
                thrown->whoami = OBJ_HEADING(obj);
            Obj_Free(obj);
            obj = 0;
        }
    }
    if (obj != 0) {
        cannot = 0;
        x = (missile_x << 3) + OBJ_FINEX(ThePlayer);
        y = (missile_y << 3) + OBJ_FINEY(ThePlayer);
        SET_Z(obj, OBJ_Z(ThePlayer));
        dist = ComObjData[OBJ_ITEM(ThePlayer)].radius + ComObjData[OBJ_ITEM(obj)].radius + 1;
        move_along((OBJ_HEADING(ThePlayer) << 5) + (ThePlayer->b18 & 0x1F), dist, &x, &y);
        cannot = !can_place(OBJ_ITEM(obj), 0, x, y, OBJ_Z(ThePlayer), 1, dist);
        if (!cannot) {
            move_along((OBJ_HEADING(ThePlayer) << 5) + (ThePlayer->b18 & 0x1F), 3, &x, &y);
            cannot = !can_place(OBJ_ITEM(obj), 0, x, y, OBJ_Z(ThePlayer), 1, dist);
        }
        tx = x >> 3;
        ty = y >> 3;
        tile = Map_GetAddr(tx, ty);
        if (!cannot) {
            SET_FINEX(obj, x & 7);
            SET_FINEY(obj, y & 7);
            Obj_AddEnd(&tile->objects, obj);
            if (OBJ_CLASS(obj) == 9 && OBJ_MINOR4(obj) >= 4 && OBJ_MINOR4(obj) <= 6)
                obj->id = obj->id & 0xFFF0 | OBJ_MINOR4(obj) - 4 & 0xF;
            if ((hit = obj_deal(obj, tx, ty, 1)) != 0 && hit > objdata)
                check_pplate(hit, tile, OBJ_Z(hit), 7);
            obj = 0;
        } else {
            if (message)
                game_sprint(0x10D);
            play_effect_here(0x2D, 0x40, 0);
        }
    }
    return obj == 0;
}

void far trap_fire(struct Object far *trap, int x, int y)
{
    struct Object far *proj;

    missile_item = trap->qn.f.quality << 5 | trap->ol.f.owner;
    missile_type = 0x14;
    missile_trx = 2;
    missile_try = 2;
    missile_x = x;
    missile_y = y;
    missile_src = trap;
    missile_heading = 0;
    using_bow = 1;
    proj = missile_fire();
    using_bow = 0;
    if (proj != 0)
        play_effect_on_mobile_src(0xA, proj, 0x14);
}

struct Object far * far missile_fire(void)
{
    struct Object far *proj;
    int launcher;
    int z;

    if ((proj = CreateObj(missile_item, 1)) != 0) {
        proj->qn.f.next = 0;
        SET_ISQUANT(proj, 1);
        proj->ol.f.link = 1;
        if (missile_heading)
            missile_heading = missile_src->b18 & 0x1F;
        missile_heading = missile_heading + (OBJ_HEADING(missile_src) << 5);
        missile_heading += missile_trx;
        missile_heading = (missile_heading + 0x100) & 0xFF;
        mob_init(proj, missile_x, missile_y);
        proj->pos = proj->pos & 0xFC7F | (missile_heading >> 5 & 7) << 7;
        proj->b18 = proj->b18 & 0xE0 | ((unsigned char)missile_heading & 0x1F) << 0;
        proj->heading = missile_heading;
        SET_BIT13(proj, 0);
        SET_Z(proj, OBJ_Z(missile_src));
        SET_FINEX(proj, OBJ_FINEX(missile_src));
        SET_FINEY(proj, OBJ_FINEY(missile_src) & 7);
        if (ComObjData[OBJ_ITEM(missile_src)].height != 0) {
            z = OBJ_Z(proj);
            SET_Z(proj, z + ComObjData[OBJ_ITEM(missile_src)].height * 5 / 6 + missile_try * 2);
            if (missile_src == ThePlayer && player->f307 > 0x50)
                SET_Z(proj, z + missile_try * 2 + (ComObjData[OBJ_ITEM(missile_src)].height - (player->f307 >> 3)));
            if (!push_missile(proj, missile_src, 1))
                goto failed;
        } else if (!push_missile(proj, missile_src, 1))
            goto failed;
        if (OBJ_MAJOR(proj) != 1) {
            launcher = 0;
            proj->proj_x = (OBJ_HOMEX(proj) << 8) + (OBJ_FINEX(proj) << 5) + 0xF;
            proj->proj_y = (OBJ_HOMEY(proj) << 8) + (OBJ_FINEY(proj) << 5) + 0xF;
            proj->proj_z = OBJ_Z(proj) << 3;
            if (OBJ_MAJOR(missile_src) == 1) {
                if ((launcher = Obj_MemTPtr(missile_src)) >= 0x100)
                    launcher = 0;
            }
            proj->last_hit = launcher;
            proj->anim = proj->anim & 0x7F;
            proj->b0A = proj->b0A & 0x7F;
        }
        proj->b14 = proj->b14 & 7 | ((unsigned char)missile_try + 0x10 & 0x1F) << 3;
        proj->b14 = proj->b14 & 0xF8 | 1;
        proj->b13 = proj->b13 & 0x80 | ((unsigned char)missile_type & 0x7F) << 0;
        if (ComObjData[missile_item].no_owner)
            proj->ol.f.owner = 0;
        Obj_Add(&Map_GetAddr(OBJ_HOMEX(proj), OBJ_HOMEY(proj))->objects, proj);
        if (!using_bow && !magical_missile)
            play_effect_on_mobile(0x1C, proj, 0);
        return proj;
failed:
        Obj_Free(proj);
    }
    return 0;
}

unsigned char far push_missile(struct Object far *proj, struct Object far *src, char launch)
{
    struct MotionCalc calc;
    int item;

    item = OBJ_ITEM(proj);
    curP = &calc;
    curP->index = Obj_MemTPtr(proj);
    curP->radius = ComObjData[item].radius;
    curP->height = ComObjData[item].height;
    curP->x = (OBJ_HOMEX(proj) << 3) + OBJ_FINEX(proj);
    curP->y = (OBJ_HOMEY(proj) << 3) + OBJ_FINEY(proj);
    if (launch)
        move_along((OBJ_HEADING(proj) << 5) + (proj->b18 & 0x1F),
                   ComObjData[OBJ_ITEM(src)].radius + ComObjData[OBJ_ITEM(proj)].radius + 4,
                   &curP->x, &curP->y);
    curP->z = OBJ_Z(proj);
    ObjectCheck(0, 1);
    TerrainCheck(0);
    if ((calc.hits0 | calc.hits1) & 0x300)
        return 0;
    if (curP->b14) {
        process_objlist();
        if (curP->b15)
            return 0;
    }
    if (launch) {
        proj->home = proj->home & 0x3FF | (curP->x >> 3 & 0x3F) << 10;
        proj->home = proj->home & 0xFC0F | (curP->y >> 3 & 0x3F) << 4;
        SET_FINEX(proj, curP->x & 7);
        SET_FINEY(proj, curP->y & 7);
    }
    return 1;
}
