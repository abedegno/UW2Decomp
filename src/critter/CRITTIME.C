/* target: ovr107 */
/* opts: -mm -1 -G -O -Y -d */
/* Critters between moments: changing a critter's goal, the yearly checkup, what happens
   to the critters and mobile objects while the player sleeps, monsters that find the
   sleeping player, teleporting a critter, the last critter hit, theft noticed by the
   owners, the pit warriors in the arena of fire, and the castle NPCs' schedule: the whole
   of DOS overlay ovr107, in original order.

   What it does in the game: everything done to critters from outside the per-frame AI
   loop (AI.C). The entry points and their callers:
   - change_critter_goal: spells (SPELLS.C), conversations (CONVVARS.C, BARTER.C) and
     SCD schedules (SCDEVENT.C) set a critter's goal.
   - yearly_checkup: PLAYTIME.C's periodic player update rerolls every critter's fed and
     ally bits.
   - update_all_critters_whilst_player_snoozes: sleep (SKILLS.C), level re-entry
     (GAMEWRAP.C) and a world event (WORLDEV.C) let time pass for the level: critters
     heal, go home and settle their attitudes, mobile objects come to rest.
   - hostile_creatures_near and wandering_monster_check: SKILLS.C's sleep refuses with
     hostiles close by and lets a hostile monster walk up to the sleeper.
   - teleport_critter: conversations and SCD events move NPCs.
   - set_creatures_from/to_saved_game, init_level_creature_stuff: GAMEWRAP.C's saving,
     restoring and level change keep AI.C's record of the last critter hit.
   - player_grabbed: taking an owned object (INTERACT.C, WORLDEV.C) angers the owners who
     see it.
   - maybe_cheat_arena_fire and arena_opponent_runs: arena traps in the Pits of Carnage
     (TRIGGER.C).
   - maybe_go_hang_out: the castle NPCs' daily schedule (WORLDEV.C's move_folks_around).

   Data owned: wander_found and hostile_found (results of the area callbacks), and stolen
   and grab_owner for player_grabbed's callback.

   Function and global names are the originals from the FM Towns symbol table.
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
/* match: this file's _BSS, DS:554E (ovr104's ends at 554D; ovr108's starts at 5550):
   only this file uses it. */
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
unsigned char far set_gridx_and_y_based_on_tile_type(unsigned char type, unsigned char *x,
                                                     unsigned char *y)
{
    switch (type) {
    case 0:
        return 0;
    case 2:
        *x = 6;
        *y = 1;
        break;
    case 3:
        *x = 1;
        *y = 1;
        break;
    case 4:
        *x = 6;
        *y = 6;
        break;
    case 5:
        *x = 6;
        *y = 1;
        break;
    case 1:
    default:
        *x = 4;
        *y = 4;
        break;
    }
    return 1;
}

/* Time passes for one critter (update_all_critters_whilst_player_snoozes). A summoned
   critter (temporary) is removed. Otherwise the transient flags of b19 are cleared, it
   faces a random way (unless waiting to talk, goal 10), heals half way to its average
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
    if (OBJ_GOAL(npc) != 10)
        SET_HEADING(npc, rand() % 8);
    if (npc->hp < cst->avghit && !OBJ_NOHEAL(npc))
        npc->hp = (npc->hp + cst->avghit) / 2;
    if (!OBJ_LONER(npc)) {
        switch (OBJ_ATTITUDE(npc)) {
        case 0:
            counts[cst->race] = counts[cst->race] - 1;
            break;
        case 3:
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
    if (Obj_Rem(&hometile->objects, npc) == 0)
        return;
    Obj_Add(&tile->objects, npc);
    SET_HOMEX(npc, x);
    SET_HOMEY(npc, y);
    SET_FINEX(npc, fx);
    SET_FINEY(npc, fy);
    SET_Z(npc, z);
}

/* Time passes for a mobile object that is not a critter: fireballs and resilient
   spheres are left alone (returns 0), anything else is deleted if Obj_Punt allows or
   else made static and dropped to the floor. Returns 1 when the mobile list changed. */
unsigned char far up_mob(struct Object far *obj)
{
    struct Tile far *tile;
    union Link far *head;
    struct Object far *stopped;
    int x;
    int y;

    if (OBJ_ITEM(obj) == ITEM_FIREBALL_1D || OBJ_ITEM(obj) == ITEM_RESILIENT_SPHERE_13F)
        return 0;
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
    if (!drop_around_place(stopped, x, y, tile->height << 3, 6)) {
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
            } else if (att > 3)
                att = 3;
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

    if (Obj_MemTPtr(target) == Obj_MemTPtr(ThePlayer))
        return 0;
    npc = target;
    if (OBJ_GOAL(npc) == 5 || OBJ_GOAL(npc) == 4 || OBJ_GOAL(npc) == 9)
        if (OBJ_B19_0(npc))
            hostile_found = 1;
    return 0;
}

/* Is a hostile, alert critter within 2 tiles of the player? SKILLS.C then refuses sleep
   with string 0x20E, "There are hostile creatures near!". */
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
    if (OBJ_ATTITUDE(obj) > 0)
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
                if (OBJ_CLASS(next) == CLASS_TRIGGER && next->ol.f.link > 0) {
                    trap = Obj_PtrTMem(&next->ol.link);
                    if (OBJ_MAJOR(trap) == MAJOR_TRAP && OBJ_MINOR(trap) == MINOR_TRAP && OBJ_INCLASS(trap) == 9)
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

/* Move a critter to tile (x, y), placed by set_gridx_and_y_based_on_tile_type, on the
   floor or, for a flier, half way up to height 0x80; fails (0) if the tile is solid or
   can_place refuses. Clears its transient b19 flags. how is not used. */
char far teleport_critter(struct Object far *critter, int x, int y, int how)
{
    struct Object far *obj;
    struct Tile far *tile;
    unsigned char fx;
    unsigned char fy;
    unsigned char z;

    obj = critter;
    set_critter_vars(obj);
    if (OBJ_HOMEX(obj) == x && OBJ_HOMEY(obj) == y)
        return 1;
    tile = Map_GetAddr(x, y);
    if (mycst->flier)
        z = (tile->height << 3) + 0x80 >> 1;
    else
        z = tile->height << 3;
    if (!set_gridx_and_y_based_on_tile_type(tile->type, &fx, &fy))
        return 0;
    if (can_place(OBJ_ITEM(obj), Obj_MemTPtr(obj), (x << 3) + fx, (y << 3) + fy, z, mycst->flier,
                  8)) {
        if (!Obj_Rem(&Map_GetAddr(myxpos, myypos)->objects, obj))
            return 0;
        Obj_Add(&tile->objects, obj);
        SET_HOMEX(obj, x);
        SET_HOMEY(obj, y);
        SET_FINEX(obj, fx);
        SET_FINEY(obj, fy);
        SET_Z(obj, z);
        SET_B19_4(critter, 0);
        SET_B19_5(critter, 0);
        SET_B19_0(critter, 0);
        SET_B19_1(critter, 0);
        return 1;
    }
    return 0;
}

/* One hostile critter within 8 tiles may come to the sleeping player
   (wander_that_monster). SKILLS.C wakes the player when it does. */
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

/* Free all 16 stored paths (PATHFIND.C) and clear every critter's has-a-path bit. */
void far clear_paths(void)
{
    struct Object far *obj;
    int i = 0x25;                       /* match: a dead store, but the DOS bytes have it */

    for (i = 2; i < NUM_MOBILE; i++) {
        obj = Obj_IntTMem(i);
        SET_B15_7(obj, 0);
    }
    freepaths = 0xFFFF;
}

/* On entering a level: forget the last critter hit and free the paths. */
void far init_level_creature_stuff(void)
{
    crithit = 0;
    typehit = 0xFF;
    clear_paths();
}

static struct Object far *stolen = 0;
static unsigned char grab_owner = 0;

/* process_area callback for player_grabbed: a critter of the owning race (grab_owner's
   low 5 bits; bit 0x20 for loners, who otherwise do not care, and 0x20 alone for loners
   only) that has the stolen object within its sight range and in line of sight grows
   one step less friendly and says so: "<name> is angered by your action." (string
   0x2F0, new attitude 0), " is annoyed by your action." (0x2F1, 1) or " notes your
   action." (0x2F2, 2). */
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
        str_cat(text, get_string(att + 0xF0 | STR_GAME));    /* " is angered...", " is annoyed...", " notes your action." */
        if (text[0] >= 'a' && text[0] <= 'z')
            text[0] = text[0] - 0x20;
        scroll_print(text);
        return 1;
    }
    return 0;
}

/* Make an object nobody's (if it can have an owner). */
/* name: IDA: ClearOwnerShip. FM Towns' clear_owner, which player_grabbed passes to
   Obj_Check. */
unsigned char far clear_owner(struct Object far *obj)
{
    if (ComObjData[OBJ_ITEM(obj)].can_own)
        obj->ol.f.owner = 0;
    return 0;
}

/* The player picked up obj, an owned object (or owner names the owner). Up to 20
   critters of the owning race within 7 tiles who see it react (critter_get_told). The
   object then belongs to nobody (unless its owner value is above 0x1D), and so does
   everything in a container. */
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
        if (owner >= 0 && (obj->ol.f.owner & 0x1F) <= 0x1D)
            obj->ol.f.owner = 0;
        if (OBJ_CLASS(obj) == CLASS_CONTAINER)
            Obj_Check(obj, clear_owner);
    }
}

/* Arena trap (TRIGGER.C, trap kind 31, in the Pits): try to rescue each of the five pit
   fighters from the fire. */
void far maybe_cheat_arena_fire(void)
{
    register int i;

    for (i = 0; i < 5; i++)
        maybe_rescue_guy_from_fire(Obj_IntTMem(player->pit_fighters[i]));
}

/* A pit fighter standing on lava, where the player cannot see, is teleported to a
   square on a ring round tile 0x16, 0x16 chosen by its direction from there, if the
   player cannot see that square either.
   The units look mixed up: x and y are fine positions (8 to a tile), yet they go to
   Map_GetAddr, which takes tiles and returns a null pointer past tile 63, and the result
   goes to teleport_critter as a tile too. FM Towns has the same code, so this is the
   original's behaviour; whether the rescue can ever work is an open question. */
void far maybe_rescue_guy_from_fire(struct Object far *obj)
{
    int x;
    int y;
    int tx;
    int ty;
    int oct;
    int adj;

    if (obj == 0)
        return;
    x = (OBJ_HOMEX(obj) << 3) + OBJ_FINEX(obj);
    y = (OBJ_HOMEY(obj) << 3) + OBJ_FINEY(obj);
    if (((TxmTerr[Map_GetAddr(x, y)->floor] & TERR_CLASS) >> 6) != TERRAIN_LAVA)
        return;
    if (player_looking(x, y) != 0)
        return;
    oct = octant((char)x - 0x16, (char)y - 0x16);
    adj = oct < 4 ? oct : 7 - oct;
    tx = 0x19 - adj * 2;
    tx = (tx << 3) + 3;
    adj = (oct + 2) % 8;
    if (adj > 3)
        adj = 7 - adj;
    ty = adj * 2 + 0x13;
    ty = (ty << 3) + 3;
    if (player_looking(tx, ty) == 0)
        teleport_critter(obj, tx, ty, 0);
}

/* An arena opponent ran from the fight (TRIGGER.C, trap kind 30): the player's win
   count in the pits goes up (QB_PIT_RECORD), with the best tally kept in the X clock
   XC_PIT_KILLS, and the opponent flees and leaves the fight. */
/* name: IDA: DefeatLivingPitWarrior. FM Towns' arena_opponent_runs: the same win count,
   goal 6 and call to remove_opponent. */
void far arena_opponent_runs(struct Object far *obj)
{
    player->quest_bytes[QB_PIT_RECORD]++;
    if (player->quest_bytes[QB_PIT_RECORD] > player->xclock[XC_PIT_KILLS])
        player->xclock[XC_PIT_KILLS] = player->quest_bytes[QB_PIT_RECORD];
    SET_GOAL(obj, 6);
    remove_opponent(obj);
}

/* Castle schedule step for one NPC (gronk_whoami callback): pick where it spends this
   part of the day (where_shall_we_hang_out), make that its home and send it there
   (goal 1); if the player can see neither where it is nor where it is going, it is
   teleported there at once. Always returns 1. */
char far maybe_go_hang_out(struct Object far *npc)
{
    int16 x;
    int16 y;

    where_shall_we_hang_out(npc, &x, &y);
    npc->qn.f.quality = x;
    npc->ol.f.owner = y;
    SET_GOAL(npc, 1);
    if (!player_looking((OBJ_HOMEX(npc) << 3) + OBJ_FINEX(npc),
                        (OBJ_HOMEY(npc) << 3) + OBJ_FINEY(npc)))
        if (!player_looking((x << 3) + 3, (y << 3) + 3))
            teleport_critter(npc, x, y, 0);
    return 1;
}

/* Where a castle NPC (whoami 0x82 to 0x8F, and 0xA8) goes. The day is split into
   two-hour stages from the hour (game_clock / 0xE1000 % 24): stage 0 is 1 to 2 am, up to
   stage 11, 11 pm to 1 am. A random place 0-5 is picked, but at night (stages 0-2) and
   otherwise one time in three the stage decides: its own spot (loc 0) at night and at
   stage 11, the area x 0x1B-0x23, y 0x22-0x25 (loc 1) in the afternoon (stages 6-8), and
   loc 4 at stages 3, 5 and 9. Places: 0 its own spot from the xs and ys tables (by
   whoami - 0x82; 0xA8 has 0x2A, 0x24), 1 to 3 a random square in a rectangle, 4 stays
   where it is (its rectangle is set but the case falls into the default), 5 stays home.
   Miranda (0x88) always, and everyone before the castle plot starts (XC_CASTLE 0), keep
   their own spot, as do Lord British (0x8E) once bit 3 of quests[28] is set and Nystul
   (0x82) once XC_CASTLE reaches 12. Names from string block 7 at whoami + 16. */
/* name: IDA: CastleNPC_Schedule. FM Towns' where_shall_we_hang_out, called by
   maybe_go_hang_out with the same time of day switch and castle tables. */
void far where_shall_we_hang_out(struct Object far *npc, int16 *x, int16 *y)
{
    int hour;
    int x1;
    int y1;
    int x2;
    int y2;
    int stage;
    int loc;

    hour = (int)(player->game_clock / 0xE1000L) % 24;
    stage = (((hour + 1) >> 1) + 11) % 12;
    loc = rand() % 6;
    if (rand() % 3 == 0 || stage <= 2)
        switch (stage) {
        case 3: case 5: case 9:
            loc = 4;
            break;
        case 0: case 1: case 2: case 11:
            loc = 0;
            break;
        case 6: case 7: case 8:
            loc = 1;
            break;
        }
    if (npc->whoami == 0x88 || player->xclock[XC_CASTLE] == 0)
        loc = 0;
    else if (npc->whoami == 0x8E && (int)((player->quests[28] & 8) >> 3))
        loc = 0;
    else if (npc->whoami == 0x82 && player->xclock[XC_CASTLE] >= 0xC)
        loc = 0;
    if (loc == 5) {
        *x = npc->qn.f.quality;
        *y = npc->ol.f.owner;
    } else if (loc == 0) {
        char xs[14] = { 0x2A, 0x24, 0x15, 0x25, 0x16, 0x19, 0x1B,
                        0x2C, 0x2B, 0x16, 0x15, 0x18, 0x1A, 0x19 };
        char ys[14] = { 0x2B, 0x33, 0x2A, 0x23, 0x33, 0x2B, 0x24,
                        0x30, 0x31, 0x25, 0x22, 0x27, 0x30, 0x22 };

        if (npc->whoami == 0xA8) {
            *x = 0x2A;
            *y = 0x24;
        } else {
            /* whoami 0x81 reads below each table (FRAME_INDEX): x is ys[13], 0x22, and y the
               high byte of gronk_whoami's arg, which move_folks_around passes as 0 */
            *x = FRAME_INDEX(xs, npc->whoami - 0x82, ys[13]);
            *y = FRAME_INDEX(ys, npc->whoami - 0x82, 0);
        }
    } else {
        switch (loc) {
        case 1:
            x1 = 0x1B;
            y1 = 0x22;
            x2 = 0x23;
            y2 = 0x25;
            break;
        case 2:
            x1 = 0x1E;
            y1 = 0x27;
            x2 = 0x20;
            y2 = 0x2B;
            break;
        case 3:
            x1 = 0x1D;
            y1 = 0x2D;
            x2 = 0x21;
            y2 = 0x33;
            break;
        case 4:
            x1 = 0x23;
            y1 = 0x2F;
            x2 = 0x2F;
            y2 = 0x34;
            /* match: no break: the DOS bytes fall into the default */
        default:
            *x = OBJ_HOMEX(npc);
            *y = OBJ_HOMEY(npc);
            return;
        }
        *x = x1 + rand() % (x2 - x1 + 1);
        *y = y1 + rand() % (y2 - y1 + 1);
        if (loc == 2 && *x == 0x1F && *y == 0x29)
            (*x)++;
    }
}
