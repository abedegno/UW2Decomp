/* target: seg025 */
/* opts: -mm -1 -G -O -d */
/* Missiles: aiming from the mouse, firing by the player, critters, spells and traps,
   throwing and dropping objects, and launching a missile into the world. The whole of DOS
   resident segment seg025, in original order. Seeded from UW2Decomp's
   src/combat/MISSILE.C (UW2's seg027_2856). Function and global names are UW2's (the FM
   Towns originals), the routines being the same; UW1 has no symbols of its own.

   What it does in the game: every missile starts here. Each front end fills in the launch
   globals (what item, from where, which source object, the aim) and calls missile_fire,
   which creates the projectile as a mobile object, gives it the source's heading plus the
   aim, lifts it to the source's eye height, steps it clear of the source (push_missile)
   and adds it to the map; the motion code flies it from there and COMBAT.C's
   missile_thwack applies its damage when it hits. The front ends are player_fire (bows,
   crossbows and slings, from COMBAT.C's player_attack), critter_fire (the AI), spell_fire
   (SPELLS.C), trap_fire (the trap code) and ReturnObject, which throws or drops the
   object on the cursor.

   UW1 against UW2: no using_bow or magical_missile, so every launch plays effect 0xA from
   missile_fire, and only player_fire adds a bow's twang (effect 9, launchers 9 and 10).
   missile_fire allocates the object itself (Obj_Alloc) and only steps it clear of a
   source with a height; push_missile always moves it (no launch argument). ReturnObject
   does not check pressure plates and plays effect 0xF when there is no room. The aiming
   area of the 3D view differs (player_settr's constants), and struct Player differs
   (Player1Swim below).

   Data owned: the launch parameters below. Missile[] (combat.h) gives each launcher its
   ammunition and each missile its damage and type.
   Name: descriptive (missiles: player_fire, missile_fire). */

/* UW1: push_missile has no launch argument (below); combat.h declares UW2's, so its
   declaration is renamed out of the way. */
#define push_missile UW2_push_missile
#include "combat.h"
#undef push_missile
#include "event.h"
#include "inv.h"
#include "map.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "sound.h"
#include "sys.h"
#include "ui.h"

/* UW1: struct Player differs from UW2's (player.h): the swimming count is byte 0xB9 (UW2
   0x307). */
struct Player1Swim {
    char pad[0xB9];
    unsigned char swim_count;           /* 0xB9 */
};
#define PLAYER1 ((struct Player1Swim *)player)

/* UW1: no launch argument (always moves the missile clear), and returns a char. */
char far push_missile(struct Object far *proj, struct Object far *src);

/* This file's _BSS, DS:26AE..26BF: the missile being launched. This file has no _DATA. */
/* match: laid out by name (tools/bssorder.py). Static in FM Towns (unnamed there), so
   static here, with UW2Decomp's provisional names, whose keys put them where the EXE has
   them: missile_class 69, missile_x and missile_y 677, missile_item 917, then missile_src,
   missile_arc, missile_trx and missile_try all 957, in definition order. */
static int16 missile_class;             /* DS:26AE */
static int16 missile_x, missile_y;      /* DS:26B0, 26B2 */
static int16 missile_item;              /* DS:26B4 */
static struct Object far *missile_src;  /* DS:26B6 */
static int16 missile_arc;               /* DS:26BA */
int16 missile_trx, missile_try;         /* DS:26BC, 26BE */

/* The player's aim from the mouse position in the 3D view: missile_trx turns the shot
   up to about 40 heading units (of 256 a turn) either side of straight ahead, and
   missile_try raises or lowers it, from the pointer's height and the player's pitch. Returns
   true when the pointer is low enough in the view (y >= 0x25 from its top, inferred) that a
   drop should be a throw. */
char far player_settr(void)
{
    int16 x, y;

    mouse_getxy(&x, &y);
    if ((x -= 0x34) > 0xAC)
        x = 0xAC;
    else if (x < 0)
        x = 0;
    if ((y -= 0x43) > 0x71)
        y = 0x71;
    else if (y < 0)
        y = 0;
    missile_trx = (x - 0x56) * 5 / 0xD + 1;
    missile_try = PlayerPitch / 0x300;
    missile_try += (y - 0x38) / 6;
    return y >= 0x25;
}

/* Fires the player's missile weapon of missile class weapon (8 sling, 9 bow, 10
   crossbow: items 0x18..0x1A, indexed within class 1): takes one of its
   ammunition from the inventory and gives the projectile that object's quantity, link,
   flags, quality (as hit points), owner and enchantment bits, so a fired arrow keeps its
   identity. A bow or crossbow twangs (effect 9). With no room in front it prints string
   0xFE. */
void far player_fire(int weapon)
{
    int slot;
    struct Object far *ammo_obj;
    struct Object far *proj;
    int ammo;

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
        } else
            game_sprint(0xFE);  /* 'You need more space to fire that weapon.' */
        if (weapon == 9 || weapon == 10)
            play_effect_here(9, 0x40, 0);
    }
}

/* A critter shoots: item is the missile (offset from FIRST_MISSILE), type its missile
   class, aimed straight along the critter's heading. */
void far critter_fire(struct Object far *who, int item, int type)
{
    missile_item = item + FIRST_MISSILE;
    missile_class = type;
    missile_x = OBJ_HOMEX(who);
    missile_y = OBJ_HOMEY(who);
    missile_arc = 1;
    missile_src = who;
    missile_trx = 0;
    missile_fire();
}

/* A spell that makes a missile (spell is the missile's item offset from FIRST_MISSILE).
   The player aims with the mouse; a critter fires straight ahead; an object caster (a wand
   on the floor, a trap, inferred: anything at or above objdata) fires flat from the map
   square inanmMapX, inanmMapY. Returns 1 when the missile was launched. */
char far spell_fire(struct Object far *who, int spell)
{
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
    return missile_fire() != 0;
}

/* Puts the object the player holds on the cursor into the world. In throw mode with the
   pointer low in the view it is thrown as a missile of class 0xF, keeping its properties as
   player_fire's ammunition does. Otherwise it is dropped a little in front of the player
   if can_place finds room (a lit light, minor class 4..6, goes out: its minor class drops
   by 4); with no room it plays effect 0xF and, with message set, prints string 0xFD.
   Returns 1 when the object left the cursor. */
char far ReturnObject(struct Object far *obj, char message)
{
    struct Object far *thrown;
    struct Tile far *tile;
    int16 x;
    int16 y;
    unsigned char dist;
    char cannot;
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
        /* UW1: can_place returns a char (cbw); map.h says unsigned */
        cannot = !(char)can_place(OBJ_ITEM(obj), 0, x, y, OBJ_Z(ThePlayer), 1, dist);
        if (!cannot) {
            move_along((OBJ_HEADING(ThePlayer) << 5) + OBJ_FINEHEAD(ThePlayer), 3, &x, &y);
            cannot = !(char)can_place(OBJ_ITEM(obj), 0, x, y, OBJ_Z(ThePlayer), 1, dist);
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
            obj_deal(obj, tx, ty, 1);
            obj = 0;
        } else {
            if (message)
                game_sprint(0xFD);  /* 'There is no space to drop that.' */
            play_effect_here(0xF, 0x40, -10);
        }
    }
    return obj == 0;
}

/* A missile trap fires: the missile item is the trap's quality * 32 + owner, class 0x14,
   from map square x, y, flat and slightly turned (missile_trx and missile_try 2). */
void far trap_fire(struct Object far *trap, int x, int y)
{
    missile_item = trap->qn.f.quality << 5 | trap->ol.f.owner;
    missile_class = 0x14;
    missile_trx = 2;
    missile_try = 2;
    missile_x = x;
    missile_y = y;
    missile_src = trap;
    missile_arc = 0;
    missile_fire();
}

/* Creates and launches the missile described by the launch globals. The heading is the
   source's fine heading (or 0 when missile_arc was cleared, for objects) plus missile_trx;
   the height is the source's z plus 5/6 of its height plus twice missile_try (lower for a
   swimming player). Missiles that are not critters keep their fixed-point position, the
   launcher's mobile index (last_hit, so the kill is credited, inferred) and missile_try as
   their vertical speed. A thrown or fired object that can be owned loses its owner. A
   source with no height (ComObjData) launches from where it is, with no room check.
   Returns the projectile, or 0 when it could not be created or placed. */
struct Object far * far missile_fire(void)
{
    struct Object far *proj;
    int launcher;
    int z;

    if ((proj = Obj_Alloc(1)) != 0) {
        proj->qn.f.next = 0;
        SET_ISQUANT(proj, 1);
        proj->ol.f.link = 1;
        SET_ITEM(proj, missile_item);
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
            if (missile_src == ThePlayer && PLAYER1->swim_count > 0x50)
                SET_Z(proj, z + missile_try * 2
                      + (ComObjData[OBJ_ITEM(missile_src)].height - (PLAYER1->swim_count >> 3)));
            if (!push_missile(proj, missile_src))
                goto failed;
        }
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
        }
        SET_PITCH(proj, (unsigned char)missile_try + 0x10);
        SET_RATE(proj, 1);
        SET_SPEED(proj, (unsigned char)missile_class);
        if (ComObjData[missile_item].can_own)
            proj->ol.f.owner = 0;
        Obj_Add(&Map_GetAddr(OBJ_HOMEX(proj), OBJ_HOMEY(proj))->objects, proj);
        play_effect_on_mobile(0xA, proj, 0);
        return proj;
failed:
        Obj_Free(proj);
    }
    return 0;
}

/* Tests whether proj fits just clear of src (the two radii plus 4 fine units ahead) and on
   success moves it there. Returns 0 when a wall or object is in the way. */
char far push_missile(struct Object far *proj, struct Object far *src)
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
    SET_HOMEX(proj, curP->x >> 3);
    SET_HOMEY(proj, curP->y >> 3);
    SET_FINEX_UNSIGNED(proj, curP->x & 7);
    SET_FINEY(proj, curP->y & 7);
    return 1;
}
