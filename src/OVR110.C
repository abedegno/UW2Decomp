/* target: ovr110 */
/* opts: -mm -1 -G -O -Y -d */
/* World events: teleports, terrain changes, traps, the castle guards, the pits, item repair,
   quest hacks and Killorn's fall: the whole of DOS overlay ovr110, in original order.
   Function and global names are the originals from the FM Towns symbol table where it has
   them; the source file's own name is not known. */
#include <stdlib.h>
#include <string.h>
#include "combat.h"
#include "conv.h"
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

extern unsigned long lastDurCheck;

#define GET_QUEST(q)    ((int)((player->quests[(q) >> 2] & (1 << ((q) & 3))) >> ((q) & 3)))
#define SET_QUEST(q, v) (player->quests[(q) >> 2] = \
            (player->quests[(q) >> 2] & ~(1 << ((q) & 3))) + ((v) << ((q) & 3)))

struct TileTextureView {
    unsigned type:4, height:4, low:2, floor_texture:4, high:2;
    unsigned wall_texture:6, links:10;
};
/* The head of an object list: the object's index in the top ten bits. */
struct ObjHeadBits { unsigned flags:6, index:10; };

typedef unsigned char (far *ObjectAction)(struct Object far *);

extern struct Tile far *mapdata;
extern char Weapons[], Armor[];
extern unsigned TxmTerr[];
extern unsigned char using_altaras_dagger;
extern unsigned Inventory[];
extern unsigned char far *ActiveMob;
extern unsigned char far *LastActiveMob;

unsigned char far Sched_SetAllClocks(unsigned char mode);
unsigned char far play_effect(unsigned char fx, int x, int y, char vol);
void far set_effect(int which, char amount);
void far Obj_FreeLinkChain(unsigned far *head, struct Object far *obj);
void far Obj_FreeChain(unsigned far *head);
struct Object far * far Obj_IntTMem(int index);
struct Object far * far Obj_PtrTMem(unsigned far *link);
struct Object far * far CreateObj(int item, char mobile);
void far Obj_Add(unsigned far *head, struct Object far *obj);
unsigned char far Obj_Rem(unsigned far *head, struct Object far *obj);
struct Object far * far Obj_Punt(unsigned far *head, struct Object far *obj, char force);
unsigned char far IsMobElem(struct Object far *obj);
struct Object far * far Obj_InList(unsigned far **head, int recurse, int major, int minor,
                                   int cls);
unsigned char far Obj_Check(struct Object far *obj, ObjectAction action);
char far HasOrIsObj(struct Object far *obj, int id);
void far get_name(char far *buf, struct Object far *obj, int article, int plural);
void far scroll_print(char far *s);
int far wyorn(int a, int id, char *answer);
void far wd_bool(char yes);
int far rand(void);
int far mpos(char x, char y);
unsigned char far can_place(int item, int index, int x, int y, int z, char flier, char dist);
unsigned char far put_at(int x, int y, int z, struct Object far *obj, int range,
                         unsigned char nocull);
void far near_mob_put_at(struct Object far *where, struct Object far *obj,
                         int how, int flag);
void far parse_player_terr(int terr, char force);
int far UseTrigger(struct Object far *who, struct Object far *start,
                   struct Object far *trig, int type);
char far damage_item(struct Object far *obj, struct Object far *who, int x, int y,
                     unsigned char damage, unsigned char type);
void far put_effect(struct Object far *obj, int type, int size, int a, int b, int x, int y);
unsigned char far flip_switch(struct Object far *obj, int owner);
void far gronk_whoami(int whoami, unsigned char all, int arg, ObjectAction fn);
void far set_numbered_variable(int var, int how, int val);
extern unsigned char running_away;      /* DS:0BF8, another file's data */
unsigned char far decode_obj_spell(struct Object far *obj, int *major, int *effect, unsigned char *flag);
struct Object far * far obj_deal(struct Object far *obj, int x, int y, char how);
void far set_new_music(int n);
char far gronkify_change_goal(struct Object far *npc, char *row);
char far teleport_critter(struct Object far *critter, int x, int y, int how);
void far punt_fightmode(void);

/* Defined later in this file. */
unsigned char far instant_kill(struct Object far *obj);

/* 1: TeleportCharToTile_ovr110_0. A breadth-first search outward from (x, y), at most
   20 tiles a ring, for a tile within the 9x9 box around it where the object fits. */
#define VISITED(tx, ty) (visited[(tx) - xmin] & (1 << ((ty) - ymin)))
#define VISIT(tx, ty) \
    if (!VISITED(tx, ty) && nnext < 20 \
        && (tx) >= xmin && (tx) <= xmax && (ty) >= ymin && (ty) <= ymax) { \
        next[nnext][0] = (tx); \
        next[nnext][1] = (ty); \
        nnext++; \
        visited[(tx) - xmin] |= 1 << ((ty) - ymin); \
    }
unsigned char far find_good_x_and_y(struct Object far *obj, int x, int y,
                                    int *nx, int *ny, char clear)
{
    unsigned char ncur, nnext;
    signed char xmin, ymin, xmax, ymax;
    unsigned char i;
    int tx, ty;
    struct Tile far *tile;
    unsigned far *link;
    struct Object far *o;
    char cur_list[20][2], next_list[20][2];
    unsigned visited[9];
    register char (*next)[2];
    register char (*cur)[2];

    memset(cur_list, 0, 0x14);
    memset(next_list, 0, 0x14);
    memset(visited, 0, 9);
    xmin = max(1, x - 4);
    xmin = min(xmin, 0x3A);
    ymin = max(1, y - 4);
    ymin = min(ymin, 0x3A);
    xmax = xmin + 9;
    xmax = min(xmax, 0x3E);
    ymax = ymin + 9;
    ymax = min(ymax, 0x3E);
    ncur = 1;
    nnext = 0;
    cur = cur_list;
    next = next_list;
    cur[0][0] = x;
    cur[0][1] = y;
    visited[x - xmin] |= 1 << (y - ymin);
    while (ncur > 0) {
        for (i = 0; i < ncur; i = i + 1) {
            tx = cur[i][0];
            ty = cur[i][1];
            tile = Map_GetAddr(tx, ty);
            if (clear) {
                o = 0L;
                for (link = &tile->objects.word; ((struct ObjHeadBits far *)link)->index;
                     link = &o->qn.word) {
                    o = Obj_PtrTMem(link);
                    if (ComObjData[o->id & 0x1FF].height || IsMobElem(o))
                        Obj_Punt(&tile->objects.word, o, 0);
                }
            }
            if (can_place(obj->id & 0x1FF, Obj_MemTPtr(obj), tx * 8 + 3, ty * 8 + 3,
                          (tile->height << 3) + ((tile_walls[tile->type] & 0x20) ? 4 : 0),
                          0, 8)) {
                *nx = tx;
                *ny = ty;
                return 1;
            }
            switch (tile->type) {
            case 0:
                break;
            case 2:
                VISIT(tx + 1, ty);
                VISIT(tx, ty - 1);
                break;
            case 3:
                VISIT(tx - 1, ty);
                VISIT(tx, ty - 1);
                break;
            case 4:
                VISIT(tx + 1, ty);
                VISIT(tx, ty + 1);
                break;
            case 5:
                VISIT(tx - 1, ty);
                VISIT(tx, ty + 1);
                break;
            case 1:
            default:
                VISIT(tx - 1, ty);
                VISIT(tx, ty - 1);
                VISIT(tx + 1, ty);
                VISIT(tx, ty + 1);
                break;
            }
        }
        {
            char (*t)[2];
            t = cur;
            cur = next;
            next = t;
            ncur = nnext;
            nnext = 0;
        }
    }
    return 0;
}

/* 2: AddTimeToClock_ovr110_949, target size 0x79. */
void far pass_time(long seconds)
{
    long elapsed = seconds << 8;
    player->game_clock += elapsed;
    player->xclock[0] = (player->game_clock / 0x4B000L) % 0x48L;
    Sched_SetAllClocks(1);
    lastDurCheck = player->game_clock >> 8;
}

/* 3: Teleport_ovr110_9C2, target size 0x211. */
int far do_teleport(struct Object far *who, int x, int y, int level)
{
    int new_x, new_y;
    unsigned char ok;
    int old_x, old_y;
    register int dest = level;
    ok = 1;
    if (who == ThePlayer) {
        if (player->sleepbits && player->in_void && dest && (dest - 1) / 8 != 8)
            return 2;
        if (GET_QUEST(112)) {
            if (!((dest == 0 || dest == 1) && y == 0x26 && x >= 0x2A && x <= 0x2B)) {
                SET_QUEST(112, 0);
                SET_QUEST(124, 1);
            }
        }
    }
    if (dest != PlayerLevel && who != ThePlayer)
        return 2;
    if ((dest == 0 || dest == PlayerLevel) && x != 0x3F && y != 0x3F) {
        dest = PlayerLevel;
        if (!find_good_x_and_y(who, x, y, &new_x, &new_y, 0))
            ok = 0;
        else {
            x = new_x;
            y = new_y;
        }
    }
    if (ok) {
        old_x = (ThePlayer->home & 0xFC00) >> 10;
        old_y = (ThePlayer->home & 0x3F0) >> 4;
        if (who == ThePlayer) {
            if (player->in_pits) {
                if (player->in_pits) {
                    if (dest != PlayerLevel || !in_arena(x, y)
                        || (mpos((char)(x - 0x1F), (char)(y - 0x1F)) >> 1)
                         != (mpos((char)(old_x - 0x1F), (char)(old_y - 0x1F)) >> 1))
                        arena_player_runs();
                }
            }
            NewPlayerLevel = dest;
            NewPlayerX = x;
            NewPlayerY = y;
            editchng(0x20);
        } else
            trap_teleport_data = -1;
        return 0x10;
    }
    trap_teleport_data = -1;
    return 2;
}

/* 4: RaiseZposOfObjectInChangingTile_ovr110_BD3, target size 0x135. */
unsigned far * far raise_up(struct Tile far *tile, unsigned far *head,
                            int old_height, int new_height, int tile_x,
                            int tile_y, char solid, int adjust)
{
    struct Object far *obj;
    unsigned char removed;
    register int height = new_height;
    removed = 0;
    obj = Obj_PtrTMem(head);
    if ((unsigned)(height * 8 + ComObjData[obj->id & 0x1FF].height) > 0x7F)
        damage_item(obj, 0L, tile_x, tile_y, 0xFF, 0);
    if ((obj->pos & 0x7F) < (height << 3)) {
        obj->pos = obj->pos & 0xFF80 | ((height << 3) & 0x7F);
        if (IsMobElem(obj) && ((obj->id & 0x1C0) >> 6) != 1)
            obj->b0F = height << 6;
        else if (obj == ThePlayer)
            PN.z = height << 6;
        else if (!IsMobElem(obj) && solid)
            removed = Obj_Punt(&tile->objects.word, obj, 0) == 0L;
    }
    if (removed)
        return head;
    return &obj->qn.word;
}

/* 5: LowerZposOfObjectInChangingTile_ovr110_D08, target size 0x10B. */
unsigned far * far put_down(struct Tile far *tile, unsigned far *head,
                           int old_height, int new_height, int tile_x,
                           int tile_y, char remove_obj, int adjust)
{
    unsigned char removed;
    struct Object far *obj;
    register int height = new_height;
    removed = 0;
    obj = Obj_PtrTMem(head);
    if ((obj->pos & 0x7F) == (old_height << 3)) {
        obj->pos = obj->pos & 0xFF80 | ((height << 3) & 0x7F);
        if (obj == ThePlayer) {
            if (adjust == 1 || adjust == 3)
                PN.z = height << 6;
            else
                parse_player_terr(0x10, 1);
        } else if (IsMobElem(obj)) {
            if (((obj->id & 0x1C0) >> 6) != 1)
                obj->b0F = height << 6;
            else
                obj->b13 = obj->b13 & 0x7F | 0x80;
        } else if (remove_obj)
            removed = Obj_Punt(&tile->objects.word, obj, 0) == 0L;
    }
    if (removed)
        return head;
    return &obj->qn.word;
}

/* 6: WillObjectMoveWithTileHeightChange_ovr110_E13, target size 0x124. */
unsigned char far filter(int tile_x, int tile_y, int cur_x, int cur_y,
                         int old_height, int new_height, unsigned far *link)
{
    struct Object far *obj;
    int xpos;
    int zpos;
    register int ypos;
    register int radius;
    obj = Obj_PtrTMem(link);
    if (((obj->id & 0x1C0) >> 6) == 6 || ((obj->id & 0x1C0) >> 6) == 5)
        return 0;
    if (tile_x == cur_x && tile_y == cur_y)
        return 1;
    zpos = obj->pos & 0x7F;
    old_height <<= 3;
    new_height <<= 3;
    if (old_height > new_height) {
        if (zpos != old_height)
            return 0;
    } else if (zpos < old_height || zpos >= new_height)
        return 0;
    xpos = ((tile_x - cur_x) << 3) + ((obj->pos & 0xE000) >> 13);
    ypos = ((tile_y - cur_y) << 3) + ((obj->pos & 0x1C00) >> 10);
    radius = ComObjData[obj->id & 0x1FF].radius;
    if (xpos < 0 && xpos + radius >= 0 || xpos > 7 && xpos - radius <= 7
        || tile_x == cur_x) {
        if (ypos < 0 && ypos + radius >= 0 || ypos > 7 && ypos - radius <= 7
            || tile_y == cur_y)
            return 1;
    }
    return 0;
}

/* 7: ChangeTile_ovr110_F37, target size 0x2F6. */
int far change_terrain(int x, int y, int wall, int floor, int height,
                       int type, int dx, int dy, int adjust)
{
    int endx, endy, xmin, xmax, tx, ymin, ymax, ty;
    struct TileTextureView far *tile;
    struct Tile far *near_tile;
    int old_height;
    unsigned far *head;
    unsigned char solid, raised;
    int terrain;
    register int cy, cx;
    solid = 0;
    endx = x + dx;
    endy = y + dy;
    for (cx = x; cx <= endx; cx++) {
        for (cy = y; cy <= endy; cy++) {
            tile = (struct TileTextureView far *)Map_GetAddr(cx, cy);
            old_height = tile->height;
            if (adjust == 1 || adjust == 3)
                height = old_height + 2 - adjust;
            if (height >= 0 && height <= 0xE)
                tile->height = height;
            if (height == 0xF && adjust == 4)
                tile->height = 0xF;
            if (floor < 0xF) {
                terrain = (TxmTerr[floor] & 0xC0) >> 6;
                tile->floor_texture = floor;
                if (terrain == 2)
                    solid = rand() & 1;
                else if (terrain == 1)
                    solid = 1;
            }
            if (wall < 0x3F)
                tile->wall_texture = wall;
            if (type < 10) {
                tile->type = type;
                if (type == 0)
                    solid = 1;
            }
            xmin = xmax = cx;
            ymin = ymax = cy;
            if (cx == x && cx > 1) xmin--;
            if (cx == endx && cx < 0x3F) xmax++;
            if (cy == y && cy > 1) ymin--;
            if (cy == endy && cy < 0x3F) ymax++;
            if (tile->height == old_height)
                continue;
            height = tile->height;
            raised = tile->height > old_height;
            for (tx = xmin; tx <= xmax; tx++) {
                for (ty = ymin; ty <= ymax; ty++) {
                    near_tile = Map_GetAddr(tx, ty);
                    if (raised) {
                        for (head = &near_tile->objects.word;
                             ((struct ObjHeadBits far *)head)->index != 0; ) {
                            if (filter(tx, ty, cx, cy, old_height, height, head))
                                head = raise_up(near_tile, head, old_height, height,
                                                cx, cy, solid, adjust);
                            else
                                head = &Obj_PtrTMem(head)->qn.word;
                        }
                    } else {
                        for (head = &near_tile->objects.word;
                             ((struct ObjHeadBits far *)head)->index != 0; ) {
                            if (filter(tx, ty, cx, cy, old_height, height, head))
                                head = put_down(near_tile, head, old_height, height,
                                                cx, cy, solid, adjust);
                            else
                                head = &Obj_PtrTMem(head)->qn.word;
                        }
                    }
                }
            }
        }
    }
    editchng(6);
    return 2;
}

/* 8: ChangeFromTrap_ovr110_122D, target size 0x1EE. */
void far do_change_grokking(struct Object far *trap, struct Object far *link,
                            int x, int y)
{
    int xstart, ystart, xend, yend, tx;
    int trapv[4];
    int linkv[4];
    int cur[4];
    unsigned char limits[4] = {0x3F, 0x0F, 0x0A, 0x0F};
    struct TileTextureView far *tile;
    unsigned char ok;
    register int i, y_;
    xend = (trap->pos & 0xE000) >> 13;
    yend = (trap->pos & 0x1C00) >> 10;
    trapv[0] = trap->qn.f.quality;
    trapv[1] = ((trap->pos & 0x380) >> 7) + ((trap->pos & 0x10) >> 1);
    trapv[2] = trap->ol.f.owner;
    trapv[3] = trap->pos & 0xF;
    linkv[0] = link->qn.f.quality;
    linkv[1] = ((link->pos & 0x380) >> 7) + ((link->pos & 0x10) >> 1);
    linkv[2] = link->ol.f.owner;
    linkv[3] = link->pos & 0xF;
    if (xend == 0) {
        xstart = 1;
        ystart = 1;
        xend = 0x3E;
        yend = 0x3E;
    } else {
        xstart = x;
        ystart = y;
        if (yend == 0)
            yend = xend;
    }
    for (tx = xstart; tx < xstart + xend; tx++) {
        for (y_ = ystart; y_ < ystart + yend; y_++) {
            tile = (struct TileTextureView far *)(mapdata + (y_ << 6) + tx);
            cur[0] = tile->wall_texture;
            cur[1] = tile->floor_texture;
            cur[2] = tile->type;
            cur[3] = tile->height;
            ok = 1;
            for (i = 0; i < 4; i++) {
                if (linkv[i] < limits[i]) {
                    ok &= trapv[i] == cur[i] || trapv[i] == limits[i];
                    cur[i] = linkv[i];
                }
            }
            if (ok)
                change_terrain(tx, y_, cur[0], cur[1], cur[3], cur[2], 0, 0, 0);
        }
    }
}

/* 9: DamageTrap_ovr110_141B, target size 0xC0. */
int far whack_thing(int index, int damage, int how, int extra)
{
    struct Object far *target;
    target = Obj_IntTMem(index);
    if (damage < 0) {
        if (target == ThePlayer) {
            if ((unsigned)(-damage) > player->poison) {
                if (check_res(ThePlayer, 1, 0x10)) {
                    player->poison = -damage;
                }
            }
        } else {
            damage = -damage;
        }
    }
    if (damage > 0) {
        if (damage_item(target, 0L,
                        (target->home & 0xFC00) >> 10,
                        (target->home & 0x3F0) >> 4,
                        (unsigned char)damage, 4))
            return 0x10;
    }
    return 2;
}

/* 10: SpellTrap_ovr110_14DB, target size 0x50. */
int far inanimate_spell(int x, int y, struct Object far *trap,
                        struct Object far *who, int major, int effect)
{
    inanmMapX = x;
    inanmMapY = y;
    if (major >= 0)
        do_spell((unsigned char)major, (unsigned char)effect, trap, who);
    else
        cast((unsigned char)effect, trap, who);
    return 2;
}

/* 11: UseFishingPole_ovr110_152B, target size 0xF7. */
unsigned char far go_fish(void)
{
    int x;
    int y;
    struct Tile far *tile;
    x = PN.x >> 5;
    y = PN.y >> 5;
    move_along(PlayerFacing >> 8, 0xB, &x, &y);
    tile = Map_GetAddr(x >> 3, y >> 3);
    if (tile->type != 0) {
        if (((TxmTerr[tile->floor] & 0xC0) >> 6) != 1)
            goto bad_place;
        if (((ThePlayer->pos & 0x7F) >> 3) <= tile->height - 1)
            goto bad_place;
    } else
        goto bad_place;
    if (rand() % 5 > (player->skills[12] + 7) / 8)
        goto no_luck;
    if (player->weight + ComObjData[0xB6].mass
        >= player->max_weight)
        goto no_room;
    game_sprint(0x70);
    return 1;
no_room:
    game_sprint(0x73);
    return 0;
no_luck:
    game_sprint(0x71);
    return 0;
bad_place:
    game_sprint(0x72);
    return 0;
}

/* 12: SpawnATalkingRotworm_ovr110_1622, target size 0xB2. */
void far talk_to_disembodied(char whoami)
{
    struct Object far *worm;
    struct Tile far *tile;
    worm = CreateObj(0x40, 1);
    worm->whoami = whoami;
    worm->attitude_word = worm->attitude_word & 0x3FFF | 0xC000;
    worm->goal_word = worm->goal_word & 0xFFF0 | 0xA;
    tile = Map_GetAddr((ThePlayer->home & 0xFC00) >> 10,
                       (ThePlayer->home & 0x3F0) >> 4);
    Obj_Add(&tile->objects.word, worm);
    TalkTo(worm);
    Obj_Rem(&tile->objects.word, worm);
    Obj_Free(worm);
}

/* 13: HackTrapTrespass_ovr110_16D4, target size 0x51. */
void far player_did_bad(int owner)
{
    register int oldx, oldy;
    if (owner != 0) {
        oldx = MapObj_X;
        oldy = MapObj_Y;
        MapObj_X = (ThePlayer->home & 0xFC00) >> 10;
        MapObj_Y = (ThePlayer->home & 0x3F0) >> 4;
        player_grabbed(ThePlayer, (unsigned char)owner);
        MapObj_X = oldx;
        MapObj_Y = oldy;
    }
}

/* 14: HackTrapChangeHeight_ovr110_1725, target size 0x84. */
void far eight_pos_switch(int flags, struct Object far *trap, int x, int y)
{
    struct Object far *obj;
    register int height;
    height = (trap->pos & 0x7F) + (flags << 3);
    if (trap->qn.f.quality == 3) {
        if (height < 0x68)
            change_terrain(x, y, 0xFF, 0xFF, height >> 3, 0xFF, 0, 0, 0);
    } else {
        obj = Obj_IntTMem((trap->ol.word >> 6) & 0x3FF);
        obj->pos = obj->pos & 0xFF80 | height & 0x7F;
    }
}

/* 15: HackTrapChangeObjectZPos_ovr110_17A9, target size 0xAA. */
void far toggle_object_height(struct Object far *trap, char multiple)
{
    struct Object far *obj;
    int i;
    register int link;
    register int z;
    for (i = 0; i <= (multiple != 0 ? 1 : 0); i++) {
        link = (trap->ol.word >> 6) & 0x3FF;
        link = (link / 5) * 5 + (link + i) % 5;
        obj = Obj_IntTMem(link);
        z = trap->pos & 0x7F;
        if ((obj->pos & 0x7F) <= z)
            z = z + trap->ol.f.owner;
        obj->pos = obj->pos & 0xFF80 | z & 0x7F;
    }
    editchng(2);
}

/* 16: SpecialEffects_ovr110_1853, target size 0x66. */
void far do_sfx(int type, int arg)
{
    switch (type) {
    case 5: arg += 0x40;
    case 6: arg += 0x40;
    case 7: arg += 0x40;
    case 8: fill_FB(arg); break;
    case 2:
        if (play_effect((unsigned char)arg + 0x64, 0, 0, 0) == 0xFF) ;
        break;
    case 4: set_effect(0x40, (unsigned char)arg << 1); break;
    }
}

/* 17: RemoveNPC_ovr110_18B9, target size 0x47. */
unsigned char far remove_whoami(struct Object far *obj)
{
    unsigned far *head;
    head = &Map_GetAddr((obj->home & 0xFC00) >> 10, (obj->home & 0x3F0) >> 4)->objects.word;
    Obj_FreeLinkChain(head, obj);
    return 1;
}

/* 18: PitWarriorLosesFight_ovr110_1900, target size 0x5C. */
unsigned char far remove_opponent(struct Object far *npc)
{
    unsigned char removed;
    register int count = 0;
    register int i;
    removed = 0;
    for (i = 0; i < 5; i++) {
        if (player->pit_fighters[i] > 0) {
            count++;
            if (Obj_MemTPtr(npc) == player->pit_fighters[i]) {
                player->pit_fighters[i] = 0;
                removed = 1;
            }
        }
    }
    if (count <= 1)
        player->in_pits = 0;
    return removed;
}

/* 19: SpecialDeathCases_ovr110_195C, target size 0x8B6. */
unsigned char far death_check(struct Object far *obj, unsigned char mode)
{
    unsigned char pit;
    pit = 0;
    if (mode && is_my_race(obj, 0xB))
        gronk_race(0xB, 1, 0, gronkify_attitude);
    if (mode && (obj->id & 0x1FF) == 0x4E && PlayerLevel == 4
        && player->quest_bytes[7] < 0xC8)
        player->quest_bytes[7]++;
    if (player->in_pits && mode)
        pit = remove_opponent(obj);
    if (pit) {
        player->quest_bytes[1]++;
        if (obj->whoami != 0x64)
            SET_QUEST(24, 1);
    }
    if (!mode && (obj->whoami >= 0x81 && obj->whoami <= 0x8F
                  || obj->whoami == 0xA8 || obj->whoami == 0x95)
        && !((obj->b0A & 0x80) >> 7)) {
        int hp;
        if (obj->whoami == 0x8D && player->xclock[1] >= 8)
            return 1;
        hp = Creature[obj->id & 0x3F].avghit / 3 - 1;
        obj->hp = hp;
        call_out_the_guards((obj->home & 0xFC00) >> 10, (obj->home & 0x3F0) >> 4);
        return 0;
    }
    switch (obj->whoami) {
    case 0x2D:
        SET_QUEST(69, mode);
        break;
    case 0x48:
        if (mode) {
            SET_QUEST(53, 1);
            player->xclock[15]++;
        }
        break;
    case 0x2C:
        if (mode) {
            SET_QUEST(66, 1);
            player->xclock[15]++;
        }
        break;
    case 0x98:
        if (mode) {
            fire_trigger_at(0x3A, 0x12);
            fire_trigger_at(0x3C, 0x12);
            SET_QUEST(122, 1);
        }
        break;
    case 0x4B:
        if (!mode && (obj->id & 0x1FF) != 0x5E) {
            obj->id = obj->id & 0xFE00 | 0x5E;
            obj->hp = 0x5C;
            obj->goal_word = obj->goal_word & 0xFFF0 | 5;
            return 0;
        }
        break;
    case 0x0B:
        SET_QUEST(10, 1);
        break;
    case 0x62:
        SET_QUEST(25, 1);
        if (pit)
            player->quest_bytes[1] = player->quest_bytes[1] + 3;
        break;
    case 0x63:
        if (mode) {
            SET_QUEST(121, 1);
            if (!GET_QUEST(23))
                player->quest_bytes[1] = player->quest_bytes[1] + 6;
        }
        break;
    case 0x65:
        if (mode) {
            SET_QUEST(65, 1);
            player->xclock[15]++;
            if (GET_QUEST(22)
                && !GET_QUEST(23))
                SET_QUEST(22, 0);
            if (GET_QUEST(23))
                player->quest_bytes[1] = player->quest_bytes[1] + 6;
        }
        break;
    case 6:
        if (mode)
            SET_QUEST(4, 1);
        break;
    case 0x31:
        if (mode)
            SET_QUEST(123, 1);
        break;
    case 0x1F:
        if (mode) {
            set_numbered_variable(0xF3, 2, 1);
            player_did_bad(0x15);
        }
        break;
    case 0x20:
        if (mode) {
            genocide(0x17);
            SET_QUEST(7, 1);
            fire_trigger_at(1, 2);
        }
        break;
    case 0x2F:
        if (!mode) {
            if ((PlayerLevel - 1) / 8 == 2) {
                struct Tile far *tile;
                tile = Map_GetAddr((obj->home & 0xFC00) >> 10, (obj->home & 0x3F0) >> 4);
                if (!GET_QUEST(54))
                    stop_and_talk(obj);
                else
                    Obj_Punt(&tile->objects.word, obj, 1);
            } else {
                stop_and_talk(obj);
                SET_QUEST(64, 1);
                player->xclock[15]++;
                return 1;
            }
            return 0;
        }
        break;
    case 0x91:
        if (!mode) {
            struct Object far *dagger;
            if (!using_altaras_dagger) {
                obj->hp = 1;
                obj->goal_word = obj->goal_word & 0xFFF0 | 5;
                return 0;
            }
            dagger = AskInventory(8 - player->lefty);
            InvRemoveOneObject(dagger);
            dagger->id = dagger->id & 0xFE00 | 0xC7;
            near_mob_put_at(ThePlayer, dagger, 6, 0);
            player->xclock[1]++;
            SET_QUEST(11, 1);
        }
        break;
    case 0x64:
        if (pit)
            player->quest_bytes[1]--;
        if (GET_QUEST(28))
            return 1;
    case 0x80:
        if (obj->whoami == 0x80 && GET_QUEST(119))
            return 1;
        if (!mode) {
            if (!((obj->b0A & 0x70) >> 4)) {
                obj->b0A = obj->b0A & 0x8F | 0x10;
                player_get_exp(0x32);
                stop_and_talk(obj);
                return 0;
            }
            return 1;
        }
        break;
    case 0x8C:
        if (mode) {
            int old;
            old = player->xclock[1];
            stop_and_talk(obj);
            player->xclock[1] = 0xD;
            if (player->xclock[3] >= 6)
                player->xclock[1] = 0xE;
            if (player->xclock[1] < old) {
                player->xclock[1] = old;
                return 1;
            }
        }
        return 1;
    case 0x8B:
        if (mode) {
            if (player->xclock[1] >= 0xB
                && (obj->home & 0xFC00) >> 10 >= 0x14
                && (obj->home & 0x3F0) >> 4 >= 0x25
                && (obj->home & 0xFC00) >> 10 <= 0x16
                && (obj->home & 0x3F0) >> 4 <= 0x27) {
                call_out_the_guards(0x12, 0x28);
                return 1;
            }
            return 1;
        }
        break;
    case 0x8D:
        if (mode && player->xclock[1] < 8)
            return 0;
        return 1;
    case 0x3A:
        if (mode) {
            player->quest_bytes[6]++;
            if (player->quest_bytes[6] >= 2) {
                SET_QUEST(50, 1);
                do_sfx(4, 0x2C);
                player->quest_bytes[6] = 0x18;
                return 1;
            }
        }
        return 1;
    case 3:
        return 1;
    }
    if (player->quest_bytes[1] > player->xclock[14])
        player->xclock[14] = player->quest_bytes[1];
    return 1;
}

/* 20: TalkToDyingNPC_ovr110_2212, target size 0x6C. */
void far stop_and_talk(struct Object far *obj)
{
    obj->last_hit = 0;
    clear_fight_state();
    obj->b15 &= 0xC0;
    obj->goal_word = obj->goal_word & 0x0FFF;
    obj->attitude_word = obj->attitude_word & 0x3FFF | 0x8000;
    obj->goal_word = obj->goal_word & 0xFFF0 | 8;
    TimeStop = 0;
    TalkTo(obj);
    FixPlayerEquips();
    obj->attitude_word = obj->attitude_word & 0xFDFF;
}

/* 21: RunScheduleTriggersInTile_15_29_ovr110_227E, target size 0x12D. */
void far call_out_the_guards(int home_x, int home_y)
{
    struct Object far *scheduled;
    struct Object far *linked;
    int dx;
    int dy;
    register int x;
    register int y;
    scheduled = Obj_FindInMapSquare(6, 2, 0xC, 0xF, 0x1D);
    while (scheduled) {
        x = (scheduled->qn.f.quality << 3) + 3;
        y = (scheduled->ol.f.owner << 3) + 3;
        dx = x - home_x;
        dy = y - home_y;
        if (abs(dx) <= 6 && abs(dy) <= 6)
            if (!player_looking(x, y)) ;
        linked = Obj_PtrTMem(&Obj_PtrTMem(&scheduled->ol.word)->ol.word);
        if (linked) {
            linked->qn.f.quality = home_x;
            linked->ol.f.owner = home_y;
            linked->goal_word = linked->goal_word & 0xFFF0 | 1;
            linked->attitude_word = linked->attitude_word & 0xFEFF | 0x100;
            linked->attitude_word = linked->attitude_word & 0xFBFF | 0x400;
        }
        UseTrigger(0L, 0L, scheduled, -1);
        scheduled = Obj_PtrTMem(&scheduled->qn.word);
    }
}

/* 22: PitArenaRelated_ovr110_23AB, target size 0x79. */
unsigned char far in_arena(int x, int y)
{
    register int oct = 0;
    unsigned char limits[4] = {0xF, 0x10, 0xE, 0xF};
    x -= 0x1F;
    y -= 0x1F;
    oct = mpos((char)x, (char)y) >> 1;
    x = abs(x);
    y = abs(y);
    if (x < 3 || y < 3)
        return 0;
    if (limits[oct] < x || limits[oct] < y)
        return 0;
    return 1;
}

/* 23: GetItemDurability_ovr110_2424, target size 0x6D. */
int far get_rep_diff(struct Object far *obj)
{
    register int item;
    switch ((obj->id & 0x1C0) >> 6) {
    case 0:
        item = obj->id & 0xF;
        switch ((obj->id & 0x30) >> 4) {
        case 0: return Weapons[item * 8 + 7];
        case 1: return -1;
        case 2: ;
        case 3: return Armor[(((obj->id & 0x3F) >> 0) - 0x20) * 4 + 1];
        }
    }
    return -1;
}

/* 24: RepairItem_ovr110_2491, target size 0x118. */
int far do_repair(struct Object far *obj, int skill, int *time)
{
    int repair_time, result, durability;
    register int new_quality, quality;
    if ((durability = get_rep_diff(obj)) == -1) return 0;
    quality = obj->qn.f.quality;
    repair_time = durability * 3 - skill - quality / 2;
    if (repair_time < 15) repair_time = 15;
    *time = repair_time;
    result = skill_check(skill, durability);
    switch (result) {
    case 2:
        new_quality = 64;
        break;
    case 1:
        new_quality = 3 + skill / 5;
        break;
    case 0:
        return 1;
    case -1:
        if ((unsigned)(rand() & 0x3F) > obj->qn.f.quality + skill) return -2;
        new_quality = -(4 + (rand() & 7));
        break;
    }
    if (quality + new_quality > 63) {
        obj->qn.f.quality = 63;
        return 3;
    }
    if (quality + new_quality > 0) {
        obj->qn.f.quality = quality + new_quality;
        if (new_quality > 0) return 2;
        return -1;
    }
    obj->qn.f.quality = 0;
    return -2;
}

/* 25: ItemRepairLogic_ovr110_25A9, target size 0x1F0. */
void far repair_item(struct Object far *obj, int skill, char who)
{
    int time;
    int durability;
    int reply;
    unsigned char answer;
    char name[80] = "giSoink";
    register int estimate, result;
    answer = 1;
    get_name(name, obj, 0, 0);
    if (who) {
        durability = get_rep_diff(obj);
        if (durability >= 0) {
            estimate = durability - skill + 15;
            if (estimate < 0)
                estimate = 0;
            else if (estimate > 30)
                estimate = 4;
            else
                estimate = estimate / 10 + 1;
            game_sprint(0xE7);
            game_sprint(estimate + 0xEA);
            game_sprint(0xE8);
            scroll_print(name);
            reply = wyorn(0, 0xE9, &answer);
            if (reply != 0 && reply < 4)
                wd_bool(answer = reply == 2);
            scroll_print("\n");
            if (!answer)
                return;
        } else {
            game_sprint(0x9C);
            return;
        }
    }
    result = do_repair(obj, skill, &time);
    if (who) {
        playerdat->noise = 0xF;
        pass_time(time * 60L);
        if (result == -2) {
            if (Obj_Elem_Fate(10, obj)) {
                InvRemoveOneObject(obj);
                Obj_Punt(0L, obj, 1);
            } else
                result = 0;
        }
        game_sprint(result + 0x9C);
        if (result) {
            scroll_print(name);
            game_sprint(0x60);
        }
        FixPlayerEquips();
        editchng(0x200);
    } else if (result == -2)
        Obj_Punt(&Map_GetAddr(MapObj_X, MapObj_Y)->objects.word, obj, 0);
}

/* 26: ClearHeadingBit2ovr110_2799, target size 0x76. Named clear_loretry_ in FM Towns, at the same position
   among its neighbours, and the code corresponds. */
unsigned char far clear_loretry(struct Object far *obj)
{
    if (IsMobElem(obj)) return 0;
    if (((obj->id & 0x1C0) >> 6) == 5 || ((obj->id & 0x1C0) >> 6) == 6
        || ComObjData[obj->id & 0x1FF].render == 2) return 0;
    obj->pos = obj->pos & 0xFC7F | (((obj->pos & 0x380) >> 7) & 3) << 7;
    return 0;
}

/* 27: ClearHeadingBit2FromAllObjects_ovr110_280F, target size 0x6D. */
void far clear_all_loretries(void)
{
    struct Tile far *tile;
    unsigned far *head;
    register int x, y;
    tile = mapdata;
    for (y = 0; y < 64; y++) {
        for (x = 0; x < 64; x++, tile++) {
            head = &tile->objects.word;
            if (((struct ObjHeadBits far *)head)->index > 0)
                Obj_Check(Obj_PtrTMem(head), clear_loretry);
        }
    }
}

/* 28: ScintillusPlatformsTrap_ovr110_287C, target size 0xA8. */
void far toggle_pillars_hack(int x, int y, int lower, int upper, int mask)
{
    int tile_x, tile_y, j;
    register int new_height, i;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 3; j++) {
            if (mask & (1 << (i * 3 + j))) {
                tile_x = x + i * 3;
                tile_y = y + j * 3;
                new_height = Map_GetAddr(tile_x, tile_y)->height;
                new_height = new_height <= lower ? upper : lower;
                change_terrain(tile_x, tile_y, 0x3F, 0xF, new_height,
                               0xF, 0, 0, 0);
            }
        }
    }
}

/* 29: HackTrapCastleSchedule_ovr110_2924, target size 0x99. */
void far move_folks_around(void)
{
    register int who, hour;
    hour = player->xclock[1];
    if (GET_QUEST(109) && hour < 16) {
        for (who = 0x81; who <= 0x8F; who++) {
            if (who == 0x8E && GET_QUEST(112)) continue;
            if (who == 0x8D && hour >= 8) continue;
            if ((who == 0x8B || who == 0x8C) && hour >= 11) continue;
            gronk_whoami(who, 0, 0, (ObjectAction)maybe_go_hang_out);
        }
        gronk_whoami(0xA8, 0, 0, (ObjectAction)maybe_go_hang_out);
    }
}

/* 30: GrowMushroomsInBritannia_ovr110_29BD, target size 0x1B1. Named kill_plants_ in FM Towns, at the same position
   among its neighbours, and the code corresponds. */
/* FM Towns: kill_plants_. Callback for courtyard_hacking. */
unsigned char far kill_plants(struct Object far *obj)
{
    int x;
    register int y;
    register int item = -1;
    if ((int)(((long)rand() * 3) / 0x8000L) == 0) {
        switch (obj->id & 0x1FF) {
        case 0xD9:
            item = 0xDA;
            break;
        case 0xDA:
            item = 0xD6;
            if ((int)(((long)rand() * 2) / 0x8000L))
                item = 0xC1;
            if ((int)(((long)rand() * 2) / 0x8000L))
                item = 0xB9;
            break;
        case 0xC1:
            switch ((int)(((long)rand() * 0xF) / 0x8000L)) {
            case 0: item = 0xB9; return 0;     /* the new item is never used */
            case 1: item = 0xD6; return 0;
            }
        default:
            return 0;
        }
        if (item >= 0)
            obj->id = obj->id & 0xFE00 | item & 0x1FF;
        x = (obj->pos & 0xE000) >> 13;
        y = (obj->pos & 0x1C00) >> 10;
        if (x != 0 && x != 7)
            obj->pos = obj->pos & 0x1FFF
                | ((x + (int)(((long)rand() * 2) / 0x8000L)
                     + (int)(((long)rand() * 2) / 0x8000L) - 1) & 7) << 13;
        if (y != 0 && y != 7)
            obj->pos = obj->pos & 0xE3FF
                | ((y + (int)(((long)rand() * 2) / 0x8000L)
                     + (int)(((long)rand() * 2) / 0x8000L) - 1) & 7) << 10;
    }
    return 0;
}

/* 31: BritanniaGoesDry_ovr110_2B6E, target size 0x277. */
void far courtyard_hacking(int x, int y, struct Object far *trap)
{
    /* the hours already handled; no FM Towns name (static) */
    static int dried = 0;
    struct Tile far *tile;
    int hour;
    int xmax, ymax;
    struct Object far *obj;
    unsigned far *head;
    int nx, ny;
    register int i, j;
    hour = player->xclock[1];
    if (dried & (1 << hour))
        return;
    dried |= 1 << hour;
    xmax = x + ((trap->pos & 0xE000) >> 13);
    ymax = y + ((trap->pos & 0x1C00) >> 10);
    for (i = x; i <= xmax; i++) {
        for (j = y; j <= ymax; j++) {
            tile = Map_GetAddr(i, j);
            head = &tile->objects.word;
            if (((struct ObjHeadBits far *)head)->index > 0)
                Obj_Check(Obj_PtrTMem(head), kill_plants);
            if (hour >= 13) {
                if ((int)(((long)rand() * 100) / 0x8000L) < 0x2B) {
                    nx = (int)(((long)rand() * 6) / 0x8000L) + 1;
                    ny = (int)(((long)rand() * 6) / 0x8000L) + 1;
                    if (can_place(0xB9, 0, (i << 3) + nx, (j << 3) + ny,
                                  tile->height << 3, 0, 0)) {
                        obj = CreateObj(0xB9, 0);
                        obj->pos = obj->pos & 0x1FFF | (nx & 7) << 13;
                        obj->pos = obj->pos & 0xE3FF | (ny & 7) << 10;
                        obj->pos = obj->pos & 0xFF80 | (tile->height << 3) & 0x7F;
                        Obj_Add(&tile->objects.word, obj);
                    }
                }
            } else if (hour >= 3) {
                obj = Obj_FindInMapSquare(7, 0, 9, i, j);
                if (obj) {
                    Obj_Punt(head, obj, 1);
                    tile->floor = trap->ol.f.owner;
                }
                obj = Obj_FindInMapSquare(4, 2, 0xE, i, j);
                if (obj)
                    obj->qn.f.quality = 0;
            }
        }
    }
}

/* 32: HackTrapBlySkupChamber_ovr110_2DE5, target size 0x1BB. */
void far skup_ductosnore(void)
{
    struct Object far *obj;
    struct Object far *crystal;
    unsigned far *head;
    if (!GET_QUEST(122))
        return;
    obj = Obj_FindInMapSquare(2, 2, 0xE, 0x3A, 4);
    if (obj == 0)
        return;
    obj = Obj_FindInMapSquare(2, 2, 1, 0x39, 4);
    if (obj == 0)
        return;
    if (obj->qn.f.quality != 2 && obj->qn.f.quality != 6)
        return;
    crystal = obj;
    obj = Obj_FindInMapSquare(2, 2, 1, 0x3B, 4);
    if (obj == 0)
        return;
    if (obj->qn.f.quality != 2 && obj->qn.f.quality != 6)
        return;
    head = &Map_GetAddr(0x39, 4)->objects.word;
    if (Obj_Rem(head, crystal))
        put_at(0x1D3, 0x1B, Map_GetAddr(0x3A, 3)->height << 3, crystal, 3, 1);
    head = &Map_GetAddr(0x3B, 4)->objects.word;
    if (Obj_Rem(head, obj))
        put_at(0x1D3, 0x1B, Map_GetAddr(0x3A, 3)->height << 3, obj, 3, 1);
    fire_trigger_at(0x3A, 5);
    SET_QUEST(122, 0);
}

/* 33: HackTrapOscillateTiles_ovr110_2FA0, target size 0xC2. */
void far standing_wave(int x, int y, int owner)
{
    struct Tile far *tile;
    int adjustment, height;
    register int i, tile_y = y;
    int oscillator[8] = {0, 1, 2, 1, 0, -1, -2, -1};
    tile = Map_GetAddr(x, tile_y);
    height = tile->height;
    adjustment = owner < 8 ? 4 - abs(4 - owner) : abs(12 - owner) - 4;
    for (i = 1; i < 8; i++) {
        tile = Map_GetAddr(x, tile_y - i);
        change_terrain(x, tile_y - i, 0x3F, 0xF,
                       height + adjustment * oscillator[i] / 2,
                       0xF, 0, 0, 0);
    }
}

/* 34: TileTextureCycle_ovr110_3062, target size 0xC9. */
void far cycle_floor(int x, int y, int width, int height,
                     int first, int last, char wall)
{
    struct TileTextureView far *tile;
    int xmax, ymax;
    register int texture, row;
    xmax = x + width;
    ymax = y + height;
    for (; x <= xmax; x++) {
        for (row = y; row <= ymax; row++) {
            tile = (struct TileTextureView far *)Map_GetAddr(x, row);
            texture = tile->floor_texture;
            if (texture <= last && texture >= first) {
                if (++texture > last) texture = first;
                tile->floor_texture = texture;
            }
            if (wall != 0) {
                texture = tile->wall_texture;
                if (texture <= last && texture >= first) {
                    if (++texture > last) texture = first;
                    tile->wall_texture = texture;
                }
            }
        }
    }
    editchng(6);
}

/* 35: HackTrapGraffiti_ovr110_312B, target size 0x14E. */
void far do_graffiti(int x, register int y, int owner)
{
    struct Object far *obj;
    unsigned far *next;
    int xmax, ymax;
    int startx, starty;
    int from;
    int prob;
    register int to;
    from = 0x3E;
    to = 0x25;
    switch (owner) {
    case 1: from = 0x3E; to = 0x25; prob = 0x10; break;
    case 2: from = 0x3F; to = 0x2A; prob = 10; break;
    case 3: from = 0x3D; to = 0x3C; prob = 6; break;
    case 4: from = 0x3B; to = 0x3A; prob = 6; break;
    case 5: from = 0x29; to = 0x2A; prob = 0x10; break;
    default: return;
    }
    xmax = x + 8;
    ymax = y + 8;
    startx = x;
    starty = y;
    for (x = startx; x <= xmax; x++) {
        for (y = starty; y <= ymax; y++) {
            for (obj = Obj_FindInMapSquare(5, 2, 0xE, x, y); obj; ) {
                if (obj->ol.f.owner == from && rand() % 16 < prob)
                    obj->ol.f.owner = to;
                next = &obj->qn.word;
                if (Obj_PtrTMem(next))
                    obj = Obj_InList(&next, 0, 5, 2, 0xE);
                else
                    obj = 0;
            }
        }
    }
}

/* 36: HackTrapPlatformReset_ovr110_3279, target size 0x135. */
void far reset_arrow_pillars(int x, int y, char owner)
{
    struct Object far *trig;
    unsigned far *head;
    struct Tile far *tile;
    int height;
    int floor;
    int dy, dx;
    register int i, j;
    for (i = x; i < 0x3F; i++) {
        for (j = y; j < 0x3F; j++) {
            tile = Map_GetAddr(i, j);
            if (tile->type == 0) {
                if (j == y)
                    i = 0x3F;
                j = 0x3F;
                continue;
            }
            head = &tile->objects.word;
            trig = Obj_InList(&head, 0, 6, 2, 4);
            if (trig == 0)
                trig = Obj_InList(&head, 0, 6, 3, 4);
            if (trig == 0)
                continue;
            height = (trig->pos & 0x7F) >> 3;
            if (tile->height == height)
                continue;
            dx = trig->qn.f.quality - i;
            dy = trig->ol.f.owner - j;
            if (owner)
                floor = (dy == 0) | (dx < 0 || dy < 0) << 1;
            else
                floor = 0xF;
            change_terrain(i, j, 0x3F, floor, height, 0x10, 0, 0, 0);
        }
    }
}

/* 37: HackTrap_ClassItem_ovr110_33AE, target size 0x15F. */
void far change_weapon_playerbest(int x, int y, int owner)
{
    struct Object far *obj;
    int magic;
    int combat;
    int type;
    int quality;
    int qty;
    register int i;
    register int item;
    quality = -1;
    qty = 0;
    combat = -1;
    for (i = 2; i <= 6; i++) {
        if (player->skills[i] > combat) {
            combat = player->skills[i];
            type = i;
        }
    }
    if (player->skills[9] > player->skills[7])
        magic = player->skills[9];
    else
        magic = player->skills[7];
    if (magic >= combat) {
        if (magic > 0)
            item = 0xF4;
        else
            item = 0x3C;
        quality = 0x3F;
    } else {
        switch (type) {
        case 2:
            item = 0x26;
            quality = 0x3F;
            break;
        case 6:
            item = 0x10;
            qty = (int)(((long)rand() << 3) / 0x8000L) + (int)(((long)rand() << 2) / 0x8000L) + 0x1E;
            break;
        case 3:
            item = 4;
            break;
        case 4:
            item = 0;
            break;
        case 5:
            item = 7;
            break;
        }
    }
    obj = Obj_FindInMapSquare(0, 0, owner, x, y);
    if (obj) {
        obj->id = obj->id & 0xFE00 | item & 0x1FF;
        if (quality != -1)
            obj->qn.f.quality = quality;
        if (qty > 0) {
            obj->id = obj->id & 0x7FFF | 0x8000;
            obj->ol.f.link = qty;
        }
    }
}

/* 38: HackTrap_ForceField_ovr110_350D, target size 0xDC. */
void far check_fraznium(int x, int y, unsigned char owner)
{
    unsigned far *head;
    struct Object far *field;
    struct Object far *obj;
    unsigned char gloves;
    gloves = 0;
    if (!owner) {
        obj = Obj_PtrTMem(&Inventory[2]);
        if (!(gloves = obj != 0 && (obj->id & 0x1FF) == 0x33)) {
            obj = Obj_PtrTMem(&Inventory[0]);
            gloves = obj != 0 && (obj->id & 0x1FF) == 0x34;
        }
    }
    head = &Map_GetAddr(x, y)->objects.word;
    field = Obj_InList(&head, 0, 5, 2, 0xD);
    if (field)
        field->pos = field->pos & 0xFF80 | (gloves ? 0x7F : 0) & 0x7F;
}

/* 39: FindAndUseSwitch_ovr110_35E9, target size 0x59. */
void far switch_flip_hack(int x, int y, int owner)
{
    struct Object far *obj;
    obj = Obj_FindInMapSquare(5, 3, -1, x, y);
    if (obj && flip_switch(obj, owner))
        checkTrap(0L, obj, 4, x, y);
}

/* 40: RandomSwitchFlicker_ovr110_3642, target size 0x44. Named maybe_flip_a_switch_ in FM Towns, at the same position
   among its neighbours, and the code corresponds. */
unsigned char far maybe_flip_a_switch(struct Object far *obj)
{
    switch ((obj->id & 0x1F0) >> 4) {
    case 0x17:
        if ((int)(((long)rand() * 2) / 0x8000L))
            flip_switch(obj, 3);
    }
    return 0;
}

/* 41: FlickSwitchesInTileRandomly_ovr110_3686, target size 0x73. */
void far play_with_switches(int x, int y)
{
    unsigned far *head;
    register int dy, dx;
    for (dx = 0; dx < 5; dx++) {
        for (dy = 0; dy < 2; dy++) {
            head = &Map_GetAddr(x + dx, y + dy)->objects.word;
            if (((struct ObjHeadBits far *)head)->index > 0)
                Obj_Check(Obj_PtrTMem(head), maybe_flip_a_switch);
        }
    }
    editchng(2);
}

/* 42: RechargeLightSphere_ovr110_36F9, target size 0x24. Named recharge_a_lightbulb_ in FM Towns, at the same position
   among its neighbours, and the code corresponds. */
unsigned char far recharge_a_lightbulb(struct Object far *obj)
{
    switch (obj->id & 0x1FF) {
    case 0x93:
        obj->qn.f.quality = 0x3F;
    }
    return 0;
}

/* 43: RechargeLightSpheresInTile_ovr110_371D, target size 0xAB. */
void far recharge_lightbulbs(int x, int y)
{
    unsigned far *head;
    struct Object far *obj;
    register int tx = x;
    register int ty = y;
    head = &Map_GetAddr(tx, ty)->objects.word;
    if (((struct ObjHeadBits far *)head)->index != 0) {
        obj = Obj_PtrTMem(head);
        Obj_Check(obj, recharge_a_lightbulb);
        while (obj) {
            if (HasOrIsObj(obj, 0x93))
                put_effect(obj, 0xC, 4, 0, 0, tx, ty);
            obj = Obj_PtrTMem(&obj->qn.word);
        }
    }
}

/* 44: BottleRecycler_ovr110_37C8, target size 0x26. Named redeem_a_bottle_ in FM Towns, at the same position
   among its neighbours, and the code corresponds. */
unsigned char far redeem_a_bottle(struct Object far *obj)
{
    switch (obj->id & 0x1FF) {
    case 0x13D:
        obj->id = obj->id & 0xFE00 | 0xA0;
    }
    return 0;
}

/* 45: HackTrapBottleRecycler_ovr110_37EE, target size 0x4C. */
void far redeem_all_bottles(int x, int y)
{
    unsigned far *head;
    head = &Map_GetAddr(x, y)->objects.word;
    if (((struct ObjHeadBits far *)head)->index > 0)
        Obj_Check(Obj_PtrTMem(head), redeem_a_bottle);
}

/* 46: ToggleForcefield_ovr110_383A, target size 0x44. Named toggle_force_field_ in FM Towns, at the same position
   among its neighbours, and the code corresponds. */
unsigned char far toggle_force_field(struct Object far *obj)
{
    if ((obj->id & 0x1FF) != 0x16D)
        return 0;
    obj->pos = (obj->pos & 0xFF80) | (((obj->pos & 0x7F) < 0x7F ? 0x7F : 0) & 0x7F);
    return 1;
}

/* 47: HackTrapForceField_ovr110_387E, target size 0x2F. */
void far find_and_gronk_force_field(int x, int y)
{
    Obj_Check(Obj_PtrTMem(&Map_GetAddr(x, y)->objects.word), toggle_force_field);
}

/* 48: SmiteUndead_ovr110_38AD, target size 0xB0. */
unsigned char far destroy_floatskull(struct Object far *obj)
{
    obj->id = obj->id & 0xFE00 | 0xC3;
    if (!can_place(0xC3, Obj_MemTPtr(obj),
                   ((obj->home & 0xFC00) >> 10 << 3) + ((obj->pos & 0xE000) >> 13),
                   ((obj->home & 0x3F0) >> 4 << 3) + ((obj->pos & 0x1C00) >> 10),
                   obj->pos & 0x7F, 1, 0))
        obj->qn.f.quality = 0;
    if (!((obj->id & 0x8000) >> 15) && obj->ol.f.link > 0)
        Obj_FreeChain(&obj->ol.word);
    return 1;
}

/* 49: PrisonTowerQuest60_ovr110_395D, target size 0xCB. */
void far prison_alarm_check(void)
{
    unsigned char far *p;
    struct Object far *npc;
    if (GET_QUEST(60) == 1)
        return;
    for (p = ActiveMob; p < LastActiveMob; p++) {
        npc = Obj_IntTMem(*p);
        if (((npc->id & 0x1C0) >> 6) != 1)
            continue;
        if (((npc->attitude_word & 0xC000) >> 14) == 0 && is_my_race(npc, 6)
            && !((npc->b0A & 0x80) >> 7)) {
            SET_QUEST(60, 1);
            player->xclock[15]++;
        }
    }
}

/* 50: LothIsDeadKillHisLiches_ovr110_3A28, target size 0x97. */
void far genocide(int race)
{
    unsigned char far *p;
    struct Object far *obj;
    for (p = ActiveMob; p < LastActiveMob; p++) {
        obj = Obj_IntTMem(*p);
        if (((obj->id & 0x1C0) >> 6) != 1) {
            if (race == 0xFF && (obj->id & 0x1FF) == 0x13)
                destroy_floatskull(obj);
        } else if (is_my_race(obj, race) && instant_kill(obj))
            p--;
    }
}

/* 51: KillCritter_ovr110_3ABF, target size 0xFD. */
unsigned char far instant_kill(struct Object far *obj)
{
    int oldx;
    unsigned far *head;
    register struct Creature *cr;
    register int oldy;
    cr = &Creature[(obj->id & 0x3F) >> 0];
    death_check(obj, 0);
    death_check(obj, 1);
    oldx = XP;
    oldy = YP;
    XP = (obj->home & 0xFC00) >> 10;
    YP = (obj->home & 0x3F0) >> 4;
    head = &Map_GetAddr(XP, YP)->objects.word;
    if (Obj_Rem(head, obj)) {
        generate_inventory(obj);
        build_corpse(obj, cr->corpse, cr->remains);
        drop_some_objects(obj);
        Obj_Free(obj);
        XP = oldx;
        YP = oldy;
        return 1;
    }
    XP = oldx;
    YP = oldy;
    return 0;
}

/* 52: CheckIfMatchingRace_ovr110_3BBC, target size 0x7E. */
unsigned char far is_my_race(struct Object far *obj, int race)
{
    if (((obj->id & 0x1C0) >> 6) != 1) {
        if (race == 0xFF && (obj->id & 0x1FF) == 0x13)
            return 1;
        return 0;
    } else if (race == 0xFF)
        return check_res(obj, 1, 0x80) == 0;
    else
        return Creature[(obj->id & 0x3F) >> 0].race == race;
}

/* 53: PuntBishop_ovr110_3C3A, target size 0x19. FM Towns has no function between is_my_race_
   and fire_trigger_at_; it has an overlay stub entry, so it is public. The name is provisional
   (IDA's RemoveBishop_ovr110_3C3A), chosen so that its tools/bssorder.py key puts it in the
   EXE's overlay stub order. */
void far PuntBishop_ovr110_3C3A(void)
{
    gronk_whoami(6, 0, 0, remove_whoami);
}

/* 54: TriggerTrapInTile_ovr110_3C53, target size 0x87. */
void far fire_trigger_at(int x, int y)
{
    struct Object far *trap;
    unsigned far *head;
    head = &Map_GetAddr(x, y)->objects.word;
    trap = Obj_InList(&head, 0, 6, -1, -1);
    if (trap) {
        if (((trap->id & 0x30) >> 4) & 2)
            UseTrigger(0L, 0L, trap, -1);
        else
            SetOffTrap(0L, 0L, trap, x, y);
    }
}

/* 55: TransformTalker_ovr110_3CDA, target size 0x131. */
unsigned char far transform_creature(struct Object far *obj, int item, int whoami,
                                     int powerful, int attitude)
{
    int x, y, height;
    register int new_item;
    register struct Creature *crit;
    x = (obj->home & 0xFC00) >> 10;
    y = (obj->home & 0x3F0) >> 4;
    height = Map_GetAddr(x, y)->height;
    if (item == -1)
        new_item = obj->id & 0x1FF;
    else
        new_item = (((item & 0x30) >> 4) << 4) + (item & 0xF) + 0x40;
    crit = &Creature[new_item & 0x3F];
    obj->id = obj->id & 0xFE00 | new_item & 0x1FF;
    if (whoami != -1)
        obj->whoami = whoami;
    if (powerful != -1)
        obj->attitude_word = obj->attitude_word & 0xFBFF | (powerful & 1) << 10;
    if (attitude != -1)
        obj->b0A = obj->b0A & 0x8F | (attitude & 7) << 4;
    if (!crit->flier
        && can_place(new_item, Obj_MemTPtr(obj), x, y, height, 0, 8))
        obj->pos = obj->pos & 0xFF80 | height & 0x7F;
    return 1;
}

/* 56: WorldGemTravel_ovr110_3E0B, target size 0x143. */
int far black_gem_trip(void)
{
    static struct { unsigned char map, x, y, flags; } worlds[8] = {
        { 0x09, 0x21, 0x20, 0x38 }, { 0x11, 0x1B, 0x22, 0x20 },
        { 0x19, 0x12, 0x27, 0x30 }, { 0x21, 0x1F, 0x1F, 0x01 },
        { 0x29, 0x04, 0x26, 0x30 }, { 0x39, 0x3B, 0x14, 0x20 },
        { 0x31, 0x20, 0x20, 0x30 }, { 0x45, 0x20, 0x16, 0x23 }
    };
    int facet_no, rel_x;
    register int world = -1;
    register int facet;
    rel_x = (((ThePlayer->home & 0xFC00) >> 10) << 3) + ((ThePlayer->pos & 0xE000) >> 13) - 0xE3;
    {
        register int rel_y = (((ThePlayer->home & 0x3F0) >> 4) << 3) + ((ThePlayer->pos & 0x1C00) >> 10) - 0x143;
        facet = abs(rel_x) < abs(rel_y);
        if (rel_y > 0)
            facet = 1 - facet;
        else
            facet += 2;
    }
    if (rel_x < 0)
        facet = 7 - facet;
    facet_no = facet;
    if ((1 << facet_no) & player->quest_bytes[2] || (player->vars[6] & 7) == facet_no)
        world = facet_no;
    if (world != -1 && worlds[world].map != 0) {
        rel_x = world;
        if (world == 5)
            rel_x = 6;
        else if (world == 6)
            rel_x = 5;
        player->quest_bytes[13] |= 1 << rel_x;
        trap_teleport_data = worlds[world].flags;
        return do_teleport(ThePlayer, worlds[world].x, worlds[world].y, worlds[world].map);
    }
    game_sprint(0x15C);
    return 4;
}

/* 57: RotateWorldGem_ovr110_3F4E, target size 0x9B. Named black_gem_rotate_ in FM Towns, at the same position
   among its neighbours, and the code corresponds. */
void far black_gem_rotate(void)
{
    int old_gem, gem;
    register int range;
    register int tries;
    old_gem = player->vars[6];
    range = 8;
    tries = 0;
    if (player->xclock[1] < 4)
        range = 1;
    else if (player->xclock[1] < 8)
        range = 3;
    else if (player->xclock[1] < 0xD)
        range = 6;
    gem = rand() % range;
    while ((gem == old_gem || (1 << gem) & player->quest_bytes[2]) && tries++ < 8)
        gem = (gem + 1) % range;
    if (tries >= 8)
        gem = 8;
    player->vars[6] = gem;
    editchng(2);
}

/* 58: QbertTraps_ovr110_3FE9, target size 0x6C6. */
void far do_qbert(int owner)
{
    static unsigned char gate_links[7] = { 0x21, 0x7F, 0x4F, 0x5B, 0x10, 0x29, 0xC2 };
    int *seq;
    int *done;
    int colour;
    int top;
    struct Tile far *trig;
    int *last_tile;
    unsigned char complete;
    register int i;
    register int j;
    seq = (int *)(player->vars + 100);
    done = seq + 7;
    trig = 0L;
    last_tile = seq + 8;
    complete = 1;
    if (seq[0] != 6) {
        seq[0] = 6;
        for (i = 1; i < 7; i++)
            seq[i] = -1;
    }
    if (owner == 0x3F) {
        struct Object far *obj;
        if (TriggerChainTileData_dseg_67d6_1BB9 - mapdata == 0xCF1) {
            for (i = 0x1D; i <= 0x23; i++) {
                obj = Obj_FindInMapSquare(5, 2, 0xE, i, 4);
                if (obj)
                    for (j = 0; j < 7 && seq[j] != -1; j++)
                        if (seq[j] == obj->ol.f.owner)
                            obj->ol.f.owner = 5;
            }
        }
        if (mapdata + *last_tile == TriggerChainTileData_dseg_67d6_1BB9)
            return;
        *last_tile = TriggerChainTileData_dseg_67d6_1BB9 - mapdata;
        trig = TriggerChainTileData_dseg_67d6_1BB9;
        j = trig->floor;
        i = 0;
        colour = -1;
        while (seq[i] != j && seq[i] != -1)
            i++;
        if (seq[i] == j) {
            i++;
            if (seq[i] == -1)
                i = 0;
            trig->floor = seq[i];
            for (j = 0; j < 5; j++)
                for (i = 0; i > j - 5; i--) {
                    top = Map_GetAddr(i + 0x31, j + 0x33)->floor;
                    if (colour == -1)
                        colour = top;
                    else if (colour != top)
                        goto scanned;
                }
        scanned:
            complete = colour == top;
            if (complete) {
                for (j = 0; j < 6; j++)
                    for (i = 0; i > j - 6; i--)
                        ((struct TileTextureView far *)Map_GetAddr(i + 0x31, j + 0x33))->wall_texture = colour;
                *done = 0;
                obj = Obj_IntTMem(0x3CC);
                obj->pos = obj->pos & 0xFF80 | 0x60;
                obj = Obj_IntTMem(0x3CD);
                if (colour == 5) {
                    obj->qn.f.quality = 0x20;
                    obj->ol.f.owner = 0x19;
                    obj->pos = obj->pos & 0xFC7F;
                } else {
                    obj->qn.f.quality = 4;
                    obj->ol.f.owner = colour * 6 + 4;
                    if (seq[4] == colour) {
                        obj = Obj_IntTMem(0x29A);
                        obj->id = obj->id & 0xBFFF;
                        Obj_IntTMem(0x279)->pos = Obj_IntTMem(0x279)->pos & 0xFF80 | obj->pos & 0x7F;
                    }
                }
                obj = Obj_IntTMem(0x3CE);
                obj->ol.f.link = 0x200 | gate_links[colour];
                obj->id = obj->id & 0xBFFF;
            }
            if (!(unsigned char)*done && !complete) {
                *done = 1;
                obj = Obj_IntTMem(0x3CC);
                obj->pos = obj->pos & 0xFF80;
                obj = Obj_IntTMem(0x3CE);
                obj->id = obj->id & 0xBFFF | 0x4000;
            }
        } else if (seq[0] > 0)
            trig->floor = seq[0];
    } else if (owner < 0xA) {
        for (i = 0; seq[i] != -1 && seq[i] != owner; i++)
            ;
        if (seq[i] != owner) {
            seq[i] = owner;
            if (i == 4)
                seq[i + 1] = 5;
        }
        trap_teleport_data = 0x3D;
        do_teleport(ThePlayer, 0x31, 0x33, 0x45);
    } else if (owner < 0x14) {
        int range, pick, x, y, level, tries;
        struct Tile far *from;
        struct Tile far *to;
        range = ((struct TileTextureView far *)Map_GetAddr(owner + 0x2E, 1))->wall_texture;
        level = 0x45;
        tries = 0;
        do {
            pick = rand() % (range * 3 >> 1);
            if (pick >= range)
                pick = 1;
            from = Map_GetAddr(owner + 0x2E, pick + 0x20);
            to = Map_GetAddr(owner + 0x2E, pick + 2);
            x = ((struct TileTextureView far *)from)->wall_texture;
            y = ((struct TileTextureView far *)to)->wall_texture;
        } while (++tries < 4
                 && abs(x - ((ThePlayer->home & 0xFC00) >> 10)) < 3
                 && abs(y - ((ThePlayer->home & 0x3F0) >> 4)) < 3);
        trap_teleport_data = (to->floor + 8) << 2;
        trap_teleport_data = trap_teleport_data + ((from->floor & 8) >> 3);
        if (from->floor & 7)
            level = level - (from->floor & 7);
        do_teleport(ThePlayer, x, y, level);
    } else {
        struct Object far *obj;
        struct Tile far *tile;
        int level;
        owner -= 0x1E;
        for (i = 0; i < 7; i++)
            if (seq[i] == owner) {
                level = 0x1D - (6 - i) * (7 - i) / 2;
                tile = Map_GetAddr((ThePlayer->home & 0xFC00) >> 10,
                                   (ThePlayer->home & 0x3F0) >> 4);
                obj = CreateObj(0x183, 0);
                Obj_Add(&tile->objects.word, obj);
                obj->pos = obj->pos & 0x1FFF | 0x6000;
                obj->pos = obj->pos & 0xE3FF | 0xC00;
                obj->pos = obj->pos & 0xFF80 | 0x74;
                crystal_ball(obj, 0x20, level);
                if (Obj_Rem(&tile->objects.word, obj))
                    Obj_Free(obj);
            }
    }
}

/* 59: AvatarIsACowardInThePits_ovr110_46AF, target size 0xCC. */
void far arena_player_runs(void)
{
    struct Object far *warrior;
    register int i;
    register int first = -1;
    if (player->in_pits) {
        running_away = 1;
        for (i = 0; i < 5; i++) {
            warrior = Obj_IntTMem(player->pit_fighters[i]);
            if (warrior) {
                warrior->attitude_word = warrior->attitude_word & 0x3FFF | 0x4000;
                warrior->last_hit = 0;
                warrior->goal_word = warrior->goal_word & 0xFFF0 | 1;
                if (first == -1)
                    first = i;
                else
                    player->pit_fighters[i] = 0;
            }
        }
        if (first > -1) {
            TalkTo(Obj_IntTMem(player->pit_fighters[first]));
            player->pit_fighters[first] = 0;
        }
        player->in_pits = 0;
        player->quest_bytes[1] = 0;
        player->quest_bytes[5] = 0;
    }
}

/* 60: TransformRedPotionToPoison_ovr110_477B, target size 0x142. */
unsigned char far morpheus_ruin_potion(struct Object far *potion)
{
    struct Object far *trap;
    register int slot;
    if (potion == ThePlayer) {
        if (GameInputMode == 1) {
            Obj_Check(CursorObjPtr, morpheus_ruin_potion);
            unforce_mouse_cursor(3);
            force_mouse_cursor(CursorObjPtr->id & 0x1FF);
        } else
            ;   /* an empty else: probably a debug message compiled out. Without it -O
                   cross-jumps the force_mouse_cursor cleanup into RedisplayInvSlot's */
        return 0;
    } else {
        if ((potion->id & 0x1FF) != 0xE5)
            return 0;
        if (((potion->id & 0x8000) >> 15) && (potion->id & 0x800) && potion->ol.f.link > 0) {
            potion->ol.f.link = 0;
            trap = CreateObj(0x180, 0);
            if (trap) {
                trap->qn.f.quality = rand() % 8 + 1;
                trap->ol.f.owner = 1;
                potion->id = potion->id & 0x7FFF;
                Obj_Add(&potion->ol.word, trap);
                potion->id = potion->id & 0xFE00 | 0xE4;
            } else
                potion->id = potion->id & 0xFE00 | 0xBC;
            slot = FindSlot(potion);
            if (slot > 0)
                RedisplayInvSlot(slot);
        }
    }
    return 0;
}

/* 61: HackTrapTransformPotionToPoison_ovr110_48BD, target size 0x2F. */
void far ruin_cure_potions(int x, int y)
{
    Obj_Check(Obj_PtrTMem(&Map_GetAddr(x, y)->objects.word), morpheus_ruin_potion);
}

/* The telekinesis wand's object index, found by find_TK_wand_check. FM Towns keeps it as an
   unnamed static (it shows as _update_vscreen+1); the name is ours. */
static int TK_wand;

/* 62: FindAcademyWand_ovr110_48EC, target size 0x6E. */
unsigned char far find_TK_wand_check(struct Object far *obj)
{
    int major, effect;
    unsigned char flag;
    if (!((obj->id & 0x2000) >> 13) || (obj->id & 0x1FF) != 0x9B)
        return 0;
    if (!decode_obj_spell(obj, &major, &effect, &flag))
        return 0;
    if (major != -1 || effect != 0x27)
        return 0;
    TK_wand = Obj_MemTPtr(obj);
    return 1;
}

/* 63: MoveAcademyWand_ovr110_495A, target size 0x1E2. */
void far remove_TK_wand(void)
{
    struct Object far *wand;
    unsigned far *head;
    register int x, y;
    TK_wand = 0;
    if (GameInputMode == 1) {
        Obj_Check(CursorObjPtr, find_TK_wand_check);
        if (TK_wand != 0) {
            wand = Obj_IntTMem(TK_wand);
            if (wand != CursorObjPtr) {
                Obj_Add(&ThePlayer->ol.word, CursorObjPtr);
                InvRemoveOneObject(wand);
                Obj_Rem(&ThePlayer->ol.word, CursorObjPtr);
            } else {
                CursorObjPtr = 0L;
                unforce_mouse_cursor(3);
                GameInputMode = 0;
            }
            put_at(0xFB, 0xEB, 0x50, wand, 3, 1);
            return;
        }
    }
    if (Obj_Check(Obj_PtrTMem(&ThePlayer->ol.word), find_TK_wand_check)) {
        if (TK_wand == 0)
            return;
        wand = Obj_IntTMem(TK_wand);
        if (InvRemoveOneObject(wand))
            put_at(0xFB, 0xEB, 0x50, wand, 3, 1);
        return;
    }
    for (x = 0; x < 64; x++)
        for (y = 0; y < 64; y++) {
            head = &Map_GetAddr(x, y)->objects.word;
            if (Obj_Check(Obj_PtrTMem(head), find_TK_wand_check)) {
                wand = Obj_IntTMem(TK_wand);
                if (Obj_Rem(head, wand))
                    put_at(0xFB, 0xEB, 0x50, wand, 3, 1);
                return;
            }
        }
}

/* 64: HackTrapVendingMachine_ovr110_4B3C, target size 0x1ED. */
void far go_vend(int which, int machine, int x, int y, int choice)
{
    static int vend_items[8] = {0xB6, 0xB0, 0xBB, 0x125, 0xBC, 3, 0x101, 0x91};
    struct Object far *item;
    unsigned char price;
    struct StaticObj sample;
    char text[80];
    register int i;
    register int id;
    switch (which) {
    case 0x28:
        player->vars[machine] = choice;
        break;
    case 0x29:
        id = vend_items[player->vars[machine]];
        price = (((int)ComObjData[id].value + 1) >> 1) + 1;
        if (!vend_check_gold(x, y, price, 1))
            break;
        vend_check_gold(x, y, price, 0);
        if ((item = CreateObj(id, 0)) == 0L)
            break;
        item->qn.f.quality = 0x3F;
        item->pos = item->pos & 0xFF80 | 0x76;
        Obj_Add(&Map_GetAddr(x, y)->objects.word, item);
        item = obj_deal(item, x, y, 0);
        break;
    case 0x2A:
        sample.ol.f.link = 0;
        sample.id = sample.id & 0xFE00 | vend_items[player->vars[machine]] & 0x1FF;
        price = (((int)ComObjData[sample.id & 0x1FF].value + 1) >> 1) + 1;
        sample.ol.f.owner = 0;
        get_name(text, (struct Object far *)&sample, 1, 0);
        if (text[0] >= 'a' && text[0] <= 'z')
            text[0] = text[0] + 'A' - 'a';
        scroll_print(text);
        game_sprint(0x15D);
        i = 0;
        text[i++] = '(';
        if (price > 9)
            text[i++] = price / 10 + '0';
        text[i++] = price % 10 + '0';
        text[i++] = ' ';
        text[i++] = 'g';
        text[i++] = 'p';
        text[i++] = ')';
        text[i++] = 0;
        scroll_print(text);
        game_sprint(0x60);
        break;
    }
}

/* 65: CollectMoneyInTile_ovr110_4D29, target size 0x151. With check set, only counts
   whether the coins in the tile cover money; otherwise takes them. */
unsigned char far vend_check_gold(char x, char y, unsigned char money, unsigned char check)
{
    struct Object far *coins;
    struct Object far *next;
    unsigned far *head;
    struct Tile far *tile;
    int count;
    tile = Map_GetAddr(x, y);
    head = &tile->objects.word;
    for (coins = Obj_PtrTMem(head); coins && money > 0; coins = next) {
        next = Obj_PtrTMem(&coins->qn.word);
        if ((coins->id & 0x1FF) == 0xA0) {
            if (((coins->id & 0x8000) >> 15) && !(coins->ol.f.link & 0x200))
                count = coins->ol.f.link;
            else
                count = 1;
            if (money >= count) {
                if (check)
                    money -= count;
                else if (Obj_Rem(head, coins)) {
                    Obj_FreeLinkChain(0L, coins);
                    money -= count;
                    head = &tile->objects.word;
                }
            } else {
                if (!check)
                    coins->ol.f.link = count - money;
                money = 0;
            }
        }
    }
    return money == 0;
}

/* 66: StoreReverseObjectLookupResultInVar_ovr110_4E7A, target size 0x1C. Named gronkify_find_ in FM Towns, at the same position
   among its neighbours, and the code corresponds. */
char far gronkify_find(struct Object far *obj, register int *result)
{
    *result = Obj_MemTPtr(obj);
    return 0;
}

/* 67: SendAvatarToJail_ovr110_4E96, target size 0x1A8. */
void far put_player_in_jail(void)
{
    int lb_index;
    struct Object far *lb;
    long hour;
    char row[16];
    ThePlayer->hp = playerdat->attr[0];
    ThePlayer->b15 = ThePlayer->b15 & 0xC0 | 1;
    set_new_music(10);
    player_get_exp(-(int)(player->exp / 9));
    hour = player->game_clock / 0x3C00L % 0x14L;
    FixBagArea();
    pass_time((0x28 - hour) * 0x3C - (rand() % 0x5A + 0x3C));
    do_teleport(ThePlayer, 0x2A, 0x26, 1);
    row[7] = 1;
    row[8] = 0;
    gronk_race(0x1C, 1, 2, gronkify_attitude);
    gronk_race(0x1C, 1, (int)row, (char (far *)(struct Object far *, int))gronkify_change_goal);
    SET_QUEST(112, 1);
    update_all_critters_whilst_player_snoozes();
    gronk_whoami(0x8E, 1, (int)&lb_index, (ObjectAction)gronkify_find);
    lb = Obj_IntTMem(lb_index);
    if (lb) {
        teleport_critter(lb, 0x2A, 0x22, 0);
        lb->qn.f.quality = 0x28;
        lb->ol.f.owner = 0x27;
        lb->goal_word = lb->goal_word & 0xFFF0 | 1;
    }
    fire_trigger_at(0x27, 0x25);
    punt_fightmode();
    gruesome_door_hack(0x2A, 0x26);
    new_player_pos();
}

/* 68: KilhornIsCrashing_ovr110_503E, target size 0x54. */
void far Killorn_just_crashed(unsigned char entering)
{
    do_sfx(4, 0);
    fadeout3d(5);
    SET_QUEST(54, 1);
    player->xclock[15]++;
    if (!entering)
        Sched_SetAllClocks(1);
}
