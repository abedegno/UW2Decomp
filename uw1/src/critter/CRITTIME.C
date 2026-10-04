/* target: ovr104 */
/* opts: -mm -1 -G -O -Y -d */
/* Critters between moments: changing a critter's goal, the yearly checkup, what happens
   to the critters and mobile objects while the player sleeps, monsters that find the
   sleeping player, the last critter hit, and theft noticed by the owners: the whole of
   UW1's DOS overlay ovr104 (UW2's ovr107), in original order.

   What it does in the game: everything done to critters from outside the per-frame AI
   loop. The entry points (callers as in UW2):
   - change_critter_goal: spells, conversations and schedules set a critter's goal.
   - yearly_checkup: the periodic player update rerolls every critter's fed and ally
     bits.
   - update_all_critters_whilst_player_snoozes: sleep and level re-entry let time pass
     for the level: critters heal, go home and settle their attitudes, mobile objects
     come to rest.
   - hostile_creatures_near and wandering_monster_check: sleep refuses with hostiles
     close by and lets a hostile monster walk up to the sleeper.
   - set_creatures_from/to_saved_game, init_level_creature_stuff: saving, restoring and
     level change keep the AI's record of the last critter hit.
   - player_grabbed: taking an owned object angers the owners who see it.

   UW1 against UW2: no teleport_critter, clear_owner, arena or castle schedule code (UW2's
   last six functions), and clear_paths lives in the path finder's segment (seg006). up_crit
   always turns a critter a random way and does not check Obj_Rem; up_mob has no exception
   for fireballs and resilient spheres; check_for_hostile_creature tests for object index
   1 rather than the player's own index; wander_that_monster finds triggers by major and
   minor class; critter_get_told has an extra exception for owner 0x0D (below), uses
   strings 0xE1.. of block 1 and does not capitalise the name; player_grabbed clears the
   owner up to 0x1B (UW2 0x1D) and leaves a container's contents alone. The player record's
   layout differs (Player1Hit below).

   Data owned: wander_found and hostile_found (results of the area callbacks), and stolen
   and grab_owner for player_grabbed's callback.

   UW1 has no symbol-bearing build: the function and global names are UW2's (the FM Towns
   symbol table), the routines being the same.
   Name: descriptive (critters between moments: yearly_checkup, change_critter_goal). */

#include <stdlib.h>
#include <string.h>
#include "combat.h"
#include "critter.h"
#include "event.h"
#include "file.h"
#include "gfx.h"
#include "map.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "ui.h"
#include "uw2.h"

/* PATHFIND.C's squares. match: not in critter.h, since an uninitialised far definition
   after an extern declaration of it comes out as near data. */
extern struct PathSq far pathsq[];

/* Set when wander_that_monster brought a monster to the player. */
/* match: this file's _BSS, in UW1 DS:5654 (UW2 DS:554E); only this file uses it. */
/* name: FM Towns has it as a static. Provisional name. */
static char wander_found;

/* critter_set_goal (AI.C) for any critter, not only the current one. */
void far change_critter_goal(struct Object far *npc, char goal, int gtarg)
{
    struct Object far *save;

    save = meptr;
    meptr = npc;
    critter_set_goal(goal, gtarg);
    meptr = save;
}

/* For every active mobile object: the fed bit (b19 bit 7) becomes random, and three
   times in four the ally bit (b19 bit 6) is cleared. Called from PLAYTIME.C's periodic
   update. */
/* name: IDA: SomethingWithUpdatingAllNPCHunger. FM Towns has yearly_checkup next after
   change_critter_goal, and its code is this loop. */
void far yearly_checkup(void)
{
    unsigned char far *p;
    struct Object far *npc;

    for (p = ActiveMob; p < LastActiveMob; p++) {
        npc = &critdata[*p];
        SET_FED(npc, rand() % 2);
        if (rand() % 4 != 1)
            SET_ALLY(npc, 0);
    }
}

/* Where in a tile of this type (map.h's tile types) to place a critter, as fine x and y
   (0-7): the middle of an open tile, the open corner of a diagonal. Returns 0 for a solid
   tile. Type 5 (TILE_DIAG_NW) gets (6, 1) like type 2, though its open corner would be
   (1, 6); probably a slip in the original. */
char far set_gridx_and_y_based_on_tile_type(unsigned char type, unsigned char *x,
                                            unsigned char *y)  /* UW1: char (callers cbw) */
{
    switch (type) {
    case TILE_SOLID:
        return 0;
    case TILE_DIAG_SE:
        *x = 6;
        *y = 1;
        break;
    case TILE_DIAG_SW:
        *x = 1;
        *y = 1;
        break;
    case TILE_DIAG_NE:
        *x = 6;
        *y = 6;
        break;
    case TILE_DIAG_NW:
        *x = 6;
        *y = 1;
        break;
    case TILE_OPEN:
    default:
        *x = 4;
        *y = 4;
        break;
    }
    return 1;
}

/* Time passes for one critter (update_all_critters_whilst_player_snoozes). A summoned
   critter (temporary) is removed. Otherwise the transient flags of b19 are cleared, it
   faces a random way (in UW1 even when waiting to talk), heals half way to its average
   hit points (unless it may not heal), and, if not a loner, counts towards its race's
   mood: -1 if hostile, +1 if friendly. Then it goes home (the home square in quality
   and owner) if it can be placed there. */
void far up_crit(struct Object far *npc, char *counts)
{
    struct Tile far *tile;
    struct Tile far *hometile;
    unsigned char xhome;
    unsigned char yhome;
    unsigned char x;
    unsigned char y;
    unsigned char fx;
    unsigned char fy;
    unsigned char z;
    struct Creature near *cst;

    cst = &Creature[OBJ_INMAJOR(npc)];
    xhome = OBJ_HOMEX(npc);
    yhome = OBJ_HOMEY(npc);
    hometile = Map_GetAddr(xhome, yhome);
    if (OBJ_TEMP(npc)) {
        Obj_FreeLinkChain(&hometile->objects, npc);
        return;
    }
    SET_FED(npc, 0);
    SET_ALLY(npc, 0);
    SET_B19_4(npc, 0);
    SET_B19_5(npc, 0);
    SET_B19_0(npc, 0);
    SET_B19_1(npc, 0);
    SET_HEADING(npc, rand() % 8);
    if (npc->hp < cst->avghit && !OBJ_NOHEAL(npc))
        npc->hp = (npc->hp + cst->avghit) / 2;
    if (!OBJ_LONER(npc)) {
        switch (OBJ_ATTITUDE(npc)) {
        case ATT_HOSTILE:
            counts[cst->race] = counts[cst->race] - 1;
            break;
        case ATT_FRIENDLY:
            counts[cst->race] = counts[cst->race] + 1;
            break;
        }
    }
    x = npc->qn.f.quality;
    y = npc->ol.f.owner;
    tile = Map_GetAddr(x, y);
    if (xhome == x && yhome == y)
        return;
    if (!set_gridx_and_y_based_on_tile_type(tile->type, &fx, &fy))
        return;
    if (cst->flier)
        z = (tile->height << 3) + 0x80 >> 1;
    else
        z = tile->height << 3;
    if (can_place(OBJ_ITEM(npc), Obj_MemTPtr(npc), (x << 3) + fx, (y << 3) + fy, z, cst->flier,
                  8) == 0)
        return;
    Obj_Rem(&hometile->objects, npc);
    Obj_Add(&tile->objects, npc);
    SET_HOMEX(npc, x);
    SET_HOMEY(npc, y);
    SET_FINEX(npc, fx);
    SET_FINEY(npc, fy);
    SET_Z(npc, z);
}

/* Time passes for a mobile object that is not a critter: it is deleted if Obj_Punt
   allows or else made static and dropped to the floor. Always returns 1 (the mobile list
   changed); UW2 leaves fireballs and resilient spheres alone and returns 0 for them. */
unsigned char far up_mob(struct Object far *obj)
{
    struct Tile far *tile;
    union Link far *head;
    struct Object far *stopped;
    int x;
    int y;

    XP = OBJ_HOMEX(obj);
    x = (XP << 3) + OBJ_FINEX(obj);
    YP = OBJ_HOMEY(obj);
    y = (YP << 3) + OBJ_FINEY(obj);
    tile = Map_GetAddr(XP, YP);
    head = &tile->objects;
    if (!Obj_Punt(head, obj, 0))
        return 1;
    stopped = mob_to_static(obj);
    if (stopped == 0)
        return 1;
    Obj_Rem(head, stopped);
    stay_centered = 1;
    if (!(char)drop_around_place(stopped, x, y, tile->height << 3, 6)) {  /* UW1: char (cbw) */
        SET_Z(stopped, tile->height << 3);
        Obj_Add(head, stopped);
    }
    return 1;
}

/* Let time pass for the level: up_crit or up_mob for every active mobile object, then
   every critter that is not a loner shifts its attitude (0 hostile to 3 friendly) by its
   race's net count of friendly less hostile members, clamped to 0..3. So a race whose
   members are mostly hostile grows more hostile, and the other way round. */
void far update_all_critters_whilst_player_snoozes(void)
{
    struct Object far *npc;
    unsigned char far *p;
    char c;
    char counts[64];
    struct Creature near *cst;

    memset(counts, 0, 0x40);
    for (p = ActiveMob; p < LastActiveMob; p++) {
        npc = &critdata[*p];
        if (OBJ_MAJOR(npc) == MAJOR_CREATURE)
            up_crit(npc, counts);
        else if (up_mob(npc))
            p--;
    }
    for (p = ActiveMob; p < LastActiveMob; p++) {
        npc = &critdata[*p];
        if (OBJ_LONER(npc) != 0)
            continue;
        cst = &Creature[OBJ_INMAJOR(npc)];
        c = counts[cst->race];
        if (c != 0) {
            char att;

            att = OBJ_ATTITUDE(npc) + c;
            if (c < 0) {
                if (att < 0)
                    att = 0;
            } else if (att > ATT_FRIENDLY)
                att = ATT_FRIENDLY;
            SET_ATTITUDE(npc, att);
        }
    }
}

/* name: statics: FM Towns keeps these three after curelem, with no names of their own. */
static char hostile_found = 0;

/* gronk_area callback: a critter other than the player that is attacking, guarding or
   cornered (goals 5, 4, 9) and knows where its target is (b19 bit 0) sets
   hostile_found. */
/* name: IDA: TestForNPCHostileAndAwareOfPlayer. FM Towns has check_for_hostile_creature
   in this place, and hostile_creatures_near passes it to gronk_area as here. */
char far check_for_hostile_creature(int x, int y, struct Object far *target,
                                    struct Tile far *tile, unsigned char src)
{
    struct Object far *npc;

    if (Obj_MemTPtr(target) == 1)          /* UW1: index 1, the player */
        return 0;
    npc = target;
    if (OBJ_GOAL(npc) == GOAL_ATTACK || OBJ_GOAL(npc) == GOAL_GUARD || OBJ_GOAL(npc) == GOAL_CORNERED)
        if (OBJ_B19_0(npc))
            hostile_found = 1;
    return 0;
}

/* Is a hostile, alert critter within 2 tiles of the player? In UW2 SKILLS.C then refuses
   sleep. */
char far hostile_creatures_near(void)
{
    hostile_found = 0;
    gronk_area(ThePlayer, 0x7F, check_for_hostile_creature, 0, 0, 2);
    return hostile_found;
}

/* gronk_area callback for wandering_monster_check: half the time, a hostile critter
   within sqrt(3) times its range of the sleeping player, with a safe walking path to
   the player (flood_path, no danger allowed) at least two squares long, is moved two
   squares short of the player's (pathsq[pathlen - 2]; the path ends at pathsq[pathlen]),
   knowing where the player is. Ward traps (item 0x189)
   under triggers along the path go off on the way. Returns 1 if it moved one. */
char far wander_that_monster(int x, int y, struct Object far *target, struct Tile far *where,
                             unsigned char src)
{
    unsigned char fx;
    unsigned char fy;
    unsigned char z;
    int dist;
    struct Tile far *tile;
    unsigned char i;
    union Link far *link;
    struct Object far *next;
    struct Object far *trap;
    struct Object far *obj;
    int tx;
    int ty;

    if (Obj_MemTPtr(target) == 1)
        return 0;
    obj = target;
    if (OBJ_ATTITUDE(obj) > ATT_HOSTILE)
        return 0;
    if (rand() % 2 == 0)
        return 0;
    set_critter_vars(obj);
    dist = (myxpos - OBJ_HOMEX(ThePlayer)) * (myxpos - OBJ_HOMEX(ThePlayer))
         + (myypos - OBJ_HOMEY(ThePlayer)) * (myypos - OBJ_HOMEY(ThePlayer));
    if (mycst->range * mycst->range * 3 < dist)
        return 0;
    if (flood_path(myxpos, myypos, OBJ_Z(meptr) >> 3, OBJ_HOMEX(ThePlayer),
                   OBJ_HOMEY(ThePlayer), OBJ_Z(ThePlayer) >> 3, 0) && pathlen >= 2) {
        for (i = 0; i < pathlen; i = i + 1) {
            tile = Map_GetAddr(pathsq[i].x, pathsq[i].y);
            for (link = &tile->objects; link->f.index != 0; link = &next->qn.link) {
                next = Obj_PtrTMem(link);
                if (OBJ_MAJOR(next) == MAJOR_TRAP && OBJ_MINOR(next) == MINOR_TRIGGER && next->ol.f.link > 0) {
                    trap = Obj_PtrTMem(&next->ol.link);
                    if (OBJ_MAJOR(trap) == MAJOR_TRAP && OBJ_MINOR(trap) == MINOR_TRAP && OBJ_INCLASS(trap) == TRAP_WARD)
                        UseTrap(trap, pathsq[i].x, pathsq[i].y);
                }
            }
        }
        tx = pathsq[pathlen - 2].x;         /* name: FM Towns names pathsq - 8 objyloc */
        ty = pathsq[pathlen - 2].y;
        tile = Map_GetAddr(tx, ty);
        if (!set_gridx_and_y_based_on_tile_type(tile->type, &fx, &fy))
            return 0;
        z = STILES[tx][ty].height << 3;
        if (can_place(OBJ_ITEM(obj), Obj_MemTPtr(obj), (tx << 3) + fx, (ty << 3) + fy, z,
                      mycst->flier, 8)) {
            Obj_Rem(&Map_GetAddr(myxpos, myypos)->objects, obj);
            Obj_Add(&tile->objects, obj);
            SET_HOMEX(obj, tx);
            SET_HOMEY(obj, ty);
            SET_FINEX(obj, fx);
            SET_FINEY(obj, fy);
            SET_Z(obj, z);
            SET_B19_0(obj, 1);
            set_loc(OBJ_HOMEX(ThePlayer), OBJ_HOMEY(ThePlayer), OBJ_Z(ThePlayer) >> 3);
            wander_found = 1;
            return 1;
        } else
            return 0;
    }
    return 0;
}

/* One hostile critter within 8 tiles may come to the sleeping player
   (wander_that_monster). In UW2 SKILLS.C wakes the player when it does. */
char far wandering_monster_check(void)
{
    wander_found = 0;
    gronk_area(ThePlayer, 1, wander_that_monster, 0, 0, 8);
    return wander_found;
}

/* Restore AI.C's record of the last critter the player hit from the player record. */
/* name: IDA: LoadCombatState. FM Towns' set_creatures_from_saved_game copies the same
   five player fields at 0x308-0x30F into the same globals. */
void far set_creatures_from_saved_game(void)
{
    crithit = player->crithit;
    typehit = player->typehit;
    crithittime = player->crithittime;
    hitx = player->hitx;
    hity = player->hity;
}

/* name: IDA: SaveRecentCombatAction; FM Towns' set_creatures_to_saved_game, the reverse
   copy. */
void far set_creatures_to_saved_game(void)
{
    player->crithit = crithit;
    player->typehit = typehit;
    player->crithittime = crithittime;
    player->hitx = hitx;
    player->hity = hity;
}

/* On entering a level: forget the last critter hit and free the paths. */
void far init_level_creature_stuff(void)
{
    crithit = 0;
    typehit = 0xFF;
    clear_paths();                      /* UW1: in PATHFIND.C's segment, seg006 */
}

static struct Object far *stolen = 0;
static unsigned char grab_owner = 0;

/* process_area callback for player_grabbed: a critter of the owning race (grab_owner's
   low 5 bits; bit 0x20 for loners, who otherwise do not care, and 0x20 alone for loners
   only) that has the stolen object within its sight range and in line of sight grows
   one step less friendly and says so, the name followed by string 0xE1 + the new
   attitude of block 1 (UW2's are 0xF0.., " is angered by your action." and so on).
   UW1 only: race 0x0D does not care while the player record's byte 0x69 is 3 or more. */
char far critter_get_told(int x, int y, struct Object far *target, struct Tile far *tile,
                          unsigned char src)
{
    struct Object far *npc;
    int nx;
    int ny;
    int nz;
    int ox;
    int oy;
    int oz;
    int dx;
    int dy;
    char text[80];
    struct Creature near *cst;
    int att;

    npc = target;
    cst = &Creature[OBJ_INMAJOR(npc)];
    if (cst->race != (grab_owner & 0x1F)
        || OBJ_LONER(npc) != 0 && !(grab_owner & 0x20)
        || grab_owner == 0x20 && !OBJ_LONER(npc))
        return 0;
    if (grab_owner == 0x0D && player->quest_bytes[QB_CRUX] >= 3)  /* UW1 only */
        return 0;
    nx = (x << 3) + OBJ_FINEX(npc);
    ny = (y << 3) + OBJ_FINEY(npc);
    nz = OBJ_Z(npc) + ComObjData[OBJ_ITEM(npc)].height;
    ox = (MapObj_X << 3) + OBJ_FINEX(stolen);
    oy = (MapObj_Y << 3) + OBJ_FINEY(stolen);
    oz = OBJ_Z(stolen) + ComObjData[OBJ_ITEM(stolen)].height + 12;
    dx = (nx - ox) / 8;
    dy = (ny - oy) / 8;
    if (dx * dx + dy * dy > cst->sight * cst->sight)
        return 0;
    if (line_of_sight(nx, ny, nz, ox, oy, oz)) {
        att = OBJ_ATTITUDE(npc) - 1;
        if (att < 0)
            att = 0;
        SET_ATTITUDE(npc, att);
        get_name(text, npc, 1, 0);
        str_cat(text, get_string(att + 0xE1 | STR_GAME));    /* " is angered...", " is annoyed...", " notes your action." */
        scroll_print(text);
        return 1;
    }
    return 0;
}

/* The player picked up obj, an owned object (or owner names the owner). Up to 20
   critters of the owning race within 7 tiles who see it react (critter_get_told). The
   object then belongs to nobody, unless its owner value is above 0x1B. */
void far player_grabbed(struct Object far *obj, unsigned char owner)
{
    grab_owner = 0;
    if (owner > 0)
        grab_owner = owner;
    else if (ComObjData[OBJ_ITEM(obj)].can_own)
        grab_owner = obj->ol.f.owner;
    if (grab_owner != 0) {
        stolen = obj;
        process_area(0x14, 0, critter_get_told, 0, MapObj_X - 7, MapObj_Y - 7, 0xF, 0xF);
        if (owner >= 0 && (obj->ol.f.owner & 0x1F) <= 0x1B)
            obj->ol.f.owner = 0;
    }
}

