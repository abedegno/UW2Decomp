/* target: seg027_2856 */
/* opts: -mm -1 -G -O -d */
/* Missiles: aiming from the mouse, firing by the player, critters, spells and traps,
   throwing and dropping objects, and launching a missile into the world. The whole of DOS
   segment seg027_2856, in original order.

   What it does in the game: every missile starts here. Each front end fills in the launch
   globals (what item, from where, which source object, the aim) and calls missile_fire,
   which creates the projectile as a mobile object, gives it the source's heading plus the
   aim, lifts it to the source's eye height, steps it clear of the source (push_missile)
   and adds it to the map; the motion code (PHYSICS.C and friends) flies it from there and
   COMBAT.C's missile_thwack applies its damage when it hits. The front ends are
   player_fire (bows, crossbows and slings, from COMBAT.C's player_attack), critter_fire
   (AI.C), spell_fire (SPELLS.C), trap_fire (TRIGGER.C) and ReturnObject, which throws or
   drops the object on the cursor (BAGS.C).

   Data owned: using_bow and magical_missile (which launch sound to play) and the launch
   parameters below. Missile[] (combat.h) gives each launcher its ammunition and each
   missile its damage and type.
   Function and global names are the originals from the FM Towns symbol table.
   Name: descriptive (missiles: player_fire, missile_fire). */

#include "combat.h"
#include "event.h"
#include "inv.h"
#include "map.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"

/* This file's _DATA, DS:03B4..03B5: it starts the word after seg026's strings end. */
unsigned char using_bow = 0;
unsigned char magical_missile = 0;

/* This file's _BSS, DS:2508..2519: the missile being launched. */
/* match: laid out by name (tools/bssorder.py). Static in FM Towns (unnamed there, just
   after Valor), so static here, with provisional names whose keys put them where UW2 has
   them: missile_class 69, missile_x and missile_y 677, missile_item 917, then missile_src,
   missile_arc, missile_trx and missile_try all 957, in definition order. */
static int missile_class;               /* DS:2508 */
static int missile_x, missile_y;        /* DS:250A, 250C */
static int missile_item;                /* DS:250E */
static struct Object far *missile_src;  /* DS:2510 */
static int missile_arc;                 /* DS:2514, always 1 */
int missile_trx, missile_try;           /* DS:2516, 2518 */

/* The player's aim from the mouse position in the 3D view: missile_trx turns the shot
   up to about 40 heading units (of 256 a turn) either side of straight ahead, and
   missile_try raises or lowers it, from the pointer's height and the player's pitch. Returns
   true when the pointer is low enough in the view (y >= 0x2A from its top, inferred) that a
   drop should be a throw. */
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

/* Fires the player's missile weapon of missile class weapon (8 sling, 9 bow, 10
   crossbow: items 0x18..0x1A, indexed within class 1): takes one of its
   ammunition from the inventory and gives the projectile that object's quantity, link,
   flags, quality (as hit points), owner and enchantment bits, so a fired arrow keeps its
   identity. A bow or crossbow twangs (effect 9). With no room in front it plays effect 0x2D
   and prints string 0x10E. */
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
        missile_class = Missile[ammo].type;
        missile_item = ammo + FIRST_MISSILE;
        missile_x = OBJ_HOMEX(ThePlayer);
        missile_y = OBJ_HOMEY(ThePlayer);
        missile_arc = 1;
        missile_src = ThePlayer;
        player_settr();
        if ((proj = missile_fire()) != 0) {
            if (using_bow && weapon != 8) {
                if ((char)play_effect_here(9, 0x40, 0) != -1)
                    update_digi_playback();
            }
            fired = 1;
            ammo_obj = RemoveOneFromSlot(0, 1, ammo, slot);
            SET_ISQUANT(proj, OBJ_ISQUANT(ammo_obj));
            proj->ol.f.link = ammo_obj->ol.f.link;
            SET_FLAGS(proj, OBJ_FLAGS(ammo_obj));
            proj->hp = ammo_obj->qn.f.quality;
            proj->ol.f.owner = ammo_obj->ol.f.owner;
            SET_DOORDIR(proj, OBJ_DOORDIR(ammo_obj));
            if (OBJ_MAJOR(ammo_obj) != MAJOR_RECT && ComObjData[OBJ_ITEM(ammo_obj)].render != 2)
                proj->whoami = OBJ_HEADING(ammo_obj);
            Obj_Free(ammo_obj);
        } else {
            play_effect_here(0x2D, 0x40, 0);
            game_sprint(0x10E);  /* 'You need more space to fire that weapon.' */
        }
        if ((using_bow || weapon == 8) && fired)
            play_effect_on_mobile_src(0xA, proj, 0x14);
        using_bow = 0;
    }
}

/* A critter shoots: item is the missile (offset from FIRST_MISSILE), type its missile
   class, aimed straight along the critter's heading. */
void far critter_fire(struct Object far *who, int item, int type)
{
    struct Object far *proj;

    missile_item = item + FIRST_MISSILE;
    missile_class = type;
    missile_x = OBJ_HOMEX(who);
    missile_y = OBJ_HOMEY(who);
    missile_arc = 1;
    missile_src = who;
    missile_trx = 0;
    using_bow = 1;
    proj = missile_fire();
    using_bow = 0;
    if (proj != 0)
        play_effect_on_mobile_src(0xA, proj, 0x14);
}

/* A spell that makes a missile (spell is the missile's item offset from FIRST_MISSILE).
   The player aims with the mouse; a critter fires straight ahead; an object caster (a wand
   on the floor, a trap, inferred: anything at or above objdata) fires flat from the map
   square inanmMapX, inanmMapY. Returns 1 when the missile was launched. */
char far spell_fire(struct Object far *who, int spell)
{
    struct Object far *proj;

    missile_item = spell + FIRST_MISSILE;
    missile_class = Missile[spell].type;
    missile_x = OBJ_HOMEX(who);
    missile_y = OBJ_HOMEY(who);
    missile_arc = 1;
    missile_src = who;
    if (who == ThePlayer)
        player_settr();
    else {
        if (who >= (struct Object far *)objdata) {
            missile_x = inanmMapX;
            missile_y = inanmMapY;
            missile_try = 0;
            missile_arc = 0;
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

/* Puts the object the player holds on the cursor into the world. In throw mode with the
   pointer low in the view it is thrown as a missile of class 0xF, keeping its properties as
   player_fire's ammunition does. Otherwise it is dropped a little in front of the player
   if can_place finds room (a lit light, minor class 4..6, goes out: its minor class drops
   by 4), and pressure plates under it are checked; with no room and message set it prints
   string 0x10D. Returns 1 when the object left the cursor. */
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
    if (inplist->mode == 1 && player_settr()) {
        missile_arc = 1;
        missile_src = ThePlayer;
        missile_item = OBJ_ITEM(obj);
        missile_class = 0xF;
        if ((thrown = missile_fire()) != 0) {
            SET_ISQUANT(thrown, OBJ_ISQUANT(obj));
            thrown->ol.f.link = obj->ol.f.link;
            SET_FLAGS(thrown, OBJ_FLAGS(obj));
            thrown->hp = obj->qn.f.quality;
            thrown->ol.f.owner = obj->ol.f.owner;
            SET_DOORDIR(thrown, OBJ_DOORDIR(obj));
            if (OBJ_MAJOR(obj) != MAJOR_RECT && ComObjData[OBJ_ITEM(obj)].render != 2)
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
        move_along((OBJ_HEADING(ThePlayer) << 5) + OBJ_FINEHEAD(ThePlayer), dist, &x, &y);
        cannot = !can_place(OBJ_ITEM(obj), 0, x, y, OBJ_Z(ThePlayer), 1, dist);
        if (!cannot) {
            move_along((OBJ_HEADING(ThePlayer) << 5) + OBJ_FINEHEAD(ThePlayer), 3, &x, &y);
            cannot = !can_place(OBJ_ITEM(obj), 0, x, y, OBJ_Z(ThePlayer), 1, dist);
        }
        tx = x >> 3;
        ty = y >> 3;
        tile = Map_GetAddr(tx, ty);
        if (!cannot) {
            SET_FINEX_UNSIGNED(obj, x & 7);
            SET_FINEY(obj, y & 7);
            Obj_AddEnd(&tile->objects, obj);
            if (OBJ_CLASS(obj) == CLASS_LIGHT && OBJ_INCLASS(obj) >= 4 && OBJ_INCLASS(obj) <= 6)
                SET_INCLASS(obj, OBJ_INCLASS(obj) - 4);
            if ((hit = obj_deal(obj, tx, ty, 1)) != 0 && hit > (struct Object far *)objdata)
                check_pplate(hit, tile, OBJ_Z(hit), 7);
            obj = 0;
        } else {
            if (message)
                game_sprint(0x10D);  /* 'There is no space to drop that.' */
            play_effect_here(0x2D, 0x40, 0);
        }
    }
    return obj == 0;
}

/* A missile trap fires: the missile item is the trap's quality * 32 + owner, class 0x14,
   from map square x, y, flat and slightly turned (missile_trx and missile_try 2). */
void far trap_fire(struct Object far *trap, int x, int y)
{
    struct Object far *proj;

    missile_item = trap->qn.f.quality << 5 | trap->ol.f.owner;
    missile_class = 0x14;
    missile_trx = 2;
    missile_try = 2;
    missile_x = x;
    missile_y = y;
    missile_src = trap;
    missile_arc = 0;
    using_bow = 1;
    proj = missile_fire();
    using_bow = 0;
    if (proj != 0)
        play_effect_on_mobile_src(0xA, proj, 0x14);
}

/* Creates and launches the missile described by the launch globals. The heading is the
   source's fine heading (or 0 when missile_arc was cleared, for objects) plus missile_trx;
   the height is the source's z plus 5/6 of its height plus twice missile_try (lower for a
   swimming player). Missiles that are not critters keep their fixed-point position, the
   launcher's mobile index (last_hit, so the kill is credited, inferred) and missile_try as
   their vertical speed. A thrown or fired object that can be owned loses its owner. Returns
   the projectile, or 0 when it could not be created or placed. */
struct Object far * far missile_fire(void)
{
    struct Object far *proj;
    int launcher;
    int z;

    if ((proj = CreateObj(missile_item, 1)) != 0) {
        proj->qn.f.next = 0;
        SET_ISQUANT(proj, 1);
        proj->ol.f.link = 1;
        if (missile_arc)
            missile_arc = missile_src->b18 & 0x1F;
        missile_arc = missile_arc + (OBJ_HEADING(missile_src) << 5);
        missile_arc += missile_trx;
        missile_arc = (missile_arc + 0x100) & 0xFF;
        mob_init(proj, missile_x, missile_y);
        SET_HEADING(proj, missile_arc >> 5);
        SET_FINEHEAD(proj, (unsigned char)missile_arc);
        proj->heading = missile_arc;
        SET_DOORDIR(proj, 0);
        SET_Z(proj, OBJ_Z(missile_src));
        SET_FINEX_UNSIGNED(proj, OBJ_FINEX(missile_src));
        SET_FINEY(proj, OBJ_FINEY(missile_src) & 7);
        if (ComObjData[OBJ_ITEM(missile_src)].height != 0) {
            z = OBJ_Z(proj);
            SET_Z(proj, z + ComObjData[OBJ_ITEM(missile_src)].height * 5 / 6 + missile_try * 2);
            if (missile_src == ThePlayer && player->swim_count > 0x50)
                SET_Z(proj, z + missile_try * 2
                      + (ComObjData[OBJ_ITEM(missile_src)].height - (player->swim_count >> 3)));
            if (!push_missile(proj, missile_src, 1))
                goto failed;
        } else if (!push_missile(proj, missile_src, 1))
            goto failed;
        if (OBJ_MAJOR(proj) != MAJOR_CREATURE) {
            launcher = 0;
            proj->goal_word = (OBJ_HOMEX(proj) << 8) + (OBJ_FINEX(proj) << 5) + 0xF;
            proj->attitude_word = (OBJ_HOMEY(proj) << 8) + (OBJ_FINEY(proj) << 5) + 0xF;
            proj->b0F = OBJ_Z(proj) << 3;
            if (OBJ_MAJOR(missile_src) == MAJOR_CREATURE) {
                if ((launcher = Obj_MemTPtr(missile_src)) >= NUM_MOBILE)
                    launcher = 0;
            }
            proj->last_hit = launcher;
            SET_B15_7(proj, 0);
            SET_LONER(proj, 0);
        }
        SET_PITCH(proj, (unsigned char)missile_try + 0x10);
        SET_RATE(proj, 1);
        SET_SPEED(proj, (unsigned char)missile_class);
        if (ComObjData[missile_item].can_own)
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

/* Tests whether proj fits where it is or, with launch set, just clear of src (the two
   radii plus 4 fine units ahead); on success with launch it moves proj there. Returns 0 when
   a wall or object is in the way. */
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
        move_along((OBJ_HEADING(proj) << 5) + OBJ_FINEHEAD(proj),
                   ComObjData[OBJ_ITEM(src)].radius + ComObjData[OBJ_ITEM(proj)].radius + 4,
                   &curP->x, &curP->y);
    curP->z = OBJ_Z(proj);
    ObjectCheck(0, 1);
    TerrainCheck(0);
    if ((calc.hits0 | calc.hits1) & 0x300)
        return 0;
    if (curP->found) {
        process_objlist();
        if (curP->count)
            return 0;
    }
    if (launch) {
        SET_HOMEX(proj, curP->x >> 3);
        SET_HOMEY(proj, curP->y >> 3);
        SET_FINEX_UNSIGNED(proj, curP->x & 7);
        SET_FINEY(proj, curP->y & 7);
    }
    return 1;
}
