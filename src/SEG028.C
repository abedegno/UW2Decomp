/* target: seg028_2941 */
/* opts: -mm -1 -G -O -Y -d */
/* Terrain and object collision for anything that moves or is placed: the floor height
   under the four corners of a mover's footprint, the walls between them, the objects
   it overlaps sorted by height, and the placing of objects in the map. The whole of DOS
   resident segment seg028_2941, in original order. Function and global names are the
   originals from the FM Towns symbol table where it has them. */

#include <mem.h>
#include <stdlib.h>

/* The low 6 bits of a list word hold the quality or owner; the top 10 bits an object
   index. */
struct Link {
    unsigned bits:6;
    unsigned index:10;
};

/* A mobile object, 0x1B bytes; a static one is its first 8 bytes. */
struct Object {
    unsigned id;                        /* item 0-8 (major class 6-8) */
    unsigned pos;                       /* z 0-6, y fine 10-12, x fine 13-15 */
    struct Link next;                   /* 0x04 */
    struct Link link;                   /* 0x06 */
    char pad8[0x15 - 0x08];
    unsigned char b15;                  /* 0x15 */
    unsigned home;                      /* 0x16, x in bits 10-15, y in bits 4-9 */
    char pad18[0x1B - 0x18];
};

#define OBJ_ITEM(o)     ((o)->id & 0x1FF)
#define OBJ_MAJOR(o)    (((o)->id & 0x1C0) >> 6)
#define OBJ_Z(o)        ((o)->pos & 0x7F)
#define OBJ_FINEY(o)    (((o)->pos & 0x1C00) >> 10)
#define OBJ_FINEX(o)    (((o)->pos & 0xE000) >> 13)
#define OBJ_HOMEX(o)    (((o)->home & 0xFC00) >> 10)
#define OBJ_HOMEY(o)    (((o)->home & 0x3F0) >> 4)
#define OBJ_B15_7(o)    (((o)->b15 & 0x80) >> 7)

#define SET_Z(o, v)       ((o)->pos = (o)->pos & 0xFF80 | (v) & 0x7F)
#define SET_FINEX(o, v)   ((o)->pos = (o)->pos & 0x1FFF | ((unsigned)(v) & 7) << 13)
#define SET_FINEY(o, v)   ((o)->pos = (o)->pos & 0xE3FF | (v) << 10)
#define SET_HOMEX(o, v)   ((o)->home = (o)->home & 0x3FF | ((v) & 0x3F) << 10)
#define SET_HOMEY(o, v)   ((o)->home = (o)->home & 0xFC0F | ((v) & 0x3F) << 4)

struct Tile {
    unsigned type:4;
    unsigned height:4;
    unsigned b8:2;
    unsigned floor:4;                   /* floor texture */
    unsigned b14:2;
    struct Link objects;                /* 0x02 */
};

/* A tile's terrain word: type 0-3, height 4-7, the floor texture's terrain bits 6-7
   in 8-9. */
#define TILE_TERR(t)    (t)->type + ((t)->height << 4) + ((TxmTerr[(t)->floor] & 0xC0) << 2)

/* The common object properties, one 11-byte record per item. */
struct ComObj {
    unsigned height:8;                  /* 0x00 */
    unsigned radius:3;
    unsigned c1_3:1;
    unsigned mass:12;                   /* 0x01, bits 4-15 */
    unsigned c3_0:1;                    /* 0x03 */
    unsigned solid:1;
    unsigned c3_2:1;
    unsigned c3_3:5;
    char pad4[2];
    unsigned c6_0:1;                    /* 0x06 */
    unsigned c6_1:7;
    char pad7[4];
};

/* The motion calculation record, reached through `curP`. */
struct MotionCalc {
    int x, y, z;                        /* 0x00 */
    unsigned w6;                        /* 0x06, heading in bits 13-15 */
    unsigned char radius;               /* 0x08 */
    unsigned char height;               /* 0x09 */
    int index;                          /* 0x0A */
    int hits0, hits1;                   /* 0x0C */
    unsigned char floor;                /* 0x10, the height under the centre */
    unsigned char top;                  /* 0x11, the highest under the footprint */
    unsigned char slope;                /* 0x12 */
    unsigned char open;                 /* 0x13 */
    unsigned char found;                /* 0x14, collisions found */
    unsigned char count;                /* 0x15, those in the way */
    signed char first;                  /* 0x16, the first of them in oCollisions */
    char pad17;
};

/* One collision found by ObjectCheck, 6 bytes. */
struct Collision {
    unsigned char top;                  /* 0x00 */
    unsigned char bottom;               /* 0x01 */
    struct Link link;                   /* 0x02 */
    int offset;                         /* 0x04, tile offset, x in bits 0-5 */
};

/* A corner of the footprint, or (the fifth) its centre: which of the nine tiles around
   the centre tile it is in, and where in that tile. */
struct Pnt {
    unsigned char tile;
    unsigned char x;
    unsigned char y;
    unsigned flags;
};

extern struct ComObj ComObjData[];
extern int TxmTerr[];
extern unsigned char tile_walls[];
extern struct Object far *objdata;

/* Elsewhere in the game. */
struct Tile far * far Map_GetAddr(int x, int y);
struct Object far * far Obj_IntTMem(int index);
struct Object far * far Obj_PtrTMem(struct Link far *link);
int far Obj_MemTPtr(struct Object far *obj);
unsigned char far IsMobElem(struct Object far *obj);
void far Obj_Add(struct Link far *head, struct Object far *obj);
void far Obj_AddEnd(struct Link far *head, struct Object far *obj);
unsigned char far Obj_Elem_Fate(int range, struct Object far *obj);
void far Obj_FreeLinkChain(struct Link far *head, struct Object far *obj);
struct Object far * far obj_deal(struct Object far *obj, int x, int y, int a);

/* Uninitialised data, DS:251A..2587. Turbo C lays _BSS out by a hash of the names, ties
   in definition order, so the names fix the layout. The publics are the FM Towns names;
   the statics (_Valor+0x13 to +0x44 there) have none, and xlow, xhigh, ypos, ylow,
   yhigh, tiles, pnt, firstsolve and pos_x are ours, chosen by compiling candidates as
   probes because they land where the EXE has them. */
static char xlow;                       /* the mover's extent within its tile */
static char xhigh;
static char ypos;                       /* the mover's position within its tile */
static char ylow;
static char yhigh;
struct Collision oCollisions[8];
static int tiles[9];                    /* terrain words of the 3x3 tiles, 0x1111 unread */
struct MotionCalc near *curP;
static struct Pnt pnt[5];
struct Tile far *centptr;               /* the centre tile */
int nvokHgt;
static char firstsolve;
static char pos_x;
int nvokTerr;

/* Initialised data, DS:03B6..03D1. */
static signed char tile_off[9] = { -65, -64, -63, -1, 0, 1, 63, 64, 65 };

unsigned char far GetHgt(unsigned char n, unsigned char *steep)
{
    unsigned char h = (tiles[pnt[n].tile] & 0xF0) >> 1;

    *steep = 0;
    switch (tiles[pnt[n].tile] & 0xF) {
    case 0:
        h = 0x80;
        break;
    case 2:
        if (pnt[n].y >= pnt[n].x)
            h = 0x80;
        *steep = 1;
        break;
    case 3:
        if (pnt[n].x + pnt[n].y >= 7)
            h = 0x80;
        *steep = 1;
        break;
    case 4:
        if (pnt[n].x + pnt[n].y <= 7)
            h = 0x80;
        *steep = 1;
        break;
    case 5:
        if (pnt[n].y <= pnt[n].x)
            h = 0x80;
        *steep = 1;
        break;
    case 6:
        h = h + (pnt[n].y & 7);
        break;
    case 7:
        h = h + (7 - (pnt[n].y & 7));
        break;
    case 8:
        h = h + (pnt[n].x & 7);
        break;
    case 9:
        h = h + (7 - (pnt[n].x & 7));
        break;
    }
    return h;
}

/* GetSlopeHgt (IDA GetTileZOffset): FM Towns has it between GetHgt and SolvePnt, and it
   reads the centre tile's slope and height the same way. */
int far GetSlopeHgt(int x, int y)
{
    int h = 0;

    y = y & 0xFF;
    x = x & 0xFF;
    switch (tiles[4] & 0xF) {
    case 6:
        h = y;
        break;
    case 7:
        h = 0xFF - y;
        break;
    case 8:
        h = x;
        break;
    case 9:
        h = 0xFF - x;
        break;
    }
    h = h >> 2;
    h += (tiles[4] & 0xF0) << 2;
    return h;
}

unsigned char far SolvePnt(unsigned char n, unsigned char range)
{
    unsigned char steep;
    unsigned char h;
    unsigned flags = 0;

    h = GetHgt(n, &steep);
    if (h == 0x80)
        flags |= 0x200;
    else if (curP->z + range < h)
        flags |= 0x100;
    else if (curP->z - range > h)
        flags |= 0x800;
    else
        flags |= 8 << ((tiles[pnt[n].tile] & 0x300) >> 8);
    pnt[n].flags = flags;
    if (curP->top < h)
        curP->top = h;
    return !steep;
}

unsigned char far SolveCenter(unsigned char range)
{
    unsigned char steep;

    curP->hits0 = (tiles[4] & 0x300) >> 8;
    curP->floor = GetHgt(4, &steep);
    if (curP->floor == 0x80)
        curP->hits0 |= 0x200;
    else if (curP->z + range < curP->floor)
        curP->hits0 |= 0x100;
    else if (curP->z - range > curP->floor)
        curP->hits0 |= 0x800;
    else {
        curP->hits0 |= 4;
        curP->hits0 |= 8 << ((tiles[4] & 0x300) >> 8);
    }
    if ((tiles[4] & 0xF) >= 6)
        curP->hits0 |= 0x2000;
    return !steep;
}

void far TerrainCheck(unsigned char range)
{
    char off;
    char i;
    struct Tile far *t;
    int x;
    int y;
    char j;

    memset(tiles, 0x11, sizeof tiles);
    centptr = Map_GetAddr(curP->x >> 3, curP->y >> 3);
    x = curP->x & 7;
    y = curP->y & 7;
    off = 4;
    pnt[4].tile = off;
    pnt[4].x = x;
    pnt[4].y = y;
    if (tiles[4] == 0x1111)
        tiles[4] = TILE_TERR(centptr);
    SolveCenter(range);
    curP->hits1 = curP->hits0;
    curP->top = curP->floor;
    if (curP->radius == 0)
        return;
    for (y -= curP->radius; y < 0; y += 8)
        off = off - 3;
    for (x -= curP->radius; x < 0; x += 8)
        off -= 1;
    pnt[0].tile = off;
    pnt[0].x = x;
    pnt[0].y = y;
    for (x += curP->radius * 2; x > 7; x -= 8)
        off = off + 1;
    pnt[1].tile = off;
    pnt[1].x = x;
    pnt[1].y = y;
    for (y += curP->radius * 2; y > 7; y -= 8)
        off = off + 3;
    pnt[2].tile = off;
    pnt[2].x = x;
    pnt[2].y = y;
    for (x -= curP->radius * 2; x < 0; x += 8)
        off -= 1;
    pnt[3].tile = off;
    pnt[3].x = x;
    pnt[3].y = y;

    t = centptr + tile_off[pnt[0].tile];
    if (tiles[pnt[0].tile] == 0x1111)
        tiles[pnt[0].tile] = TILE_TERR(t);
    t = centptr + tile_off[pnt[1].tile];
    if (tiles[pnt[1].tile] == 0x1111)
        tiles[pnt[1].tile] = TILE_TERR(t);
    t = centptr + tile_off[pnt[2].tile];
    if (tiles[pnt[2].tile] == 0x1111)
        tiles[pnt[2].tile] = TILE_TERR(t);
    t = centptr + tile_off[pnt[3].tile];
    if (tiles[pnt[3].tile] == 0x1111)
        tiles[pnt[3].tile] = TILE_TERR(t);

    firstsolve = 1;
    for (i = 0; i < 4; i++) {
        if (!SolvePnt(i, range) && !(pnt[i].flags & 0x300)) {
            char walls[5] = { 4, 0x10, 2, 8, 4 };

            for (j = 0; j < 2; j++) {
                if (pnt[(i + (j << 1) + 1) & 3].tile != pnt[i].tile
                    && (tile_walls[tiles[pnt[i].tile] & 0xF] & walls[j + i])) {
                    pnt[i].flags = 0x200;
                    curP->top = 0x80;
                    j = 2;
                }
            }
            firstsolve = 0;
        }
    }
    curP->hits1 |= pnt[0].flags | pnt[1].flags | pnt[2].flags | pnt[3].flags;
}

static signed char side[4] = { 1, -1, -1, 1 };
static unsigned char dirs[3][3] = { { 5, 4, 3 }, { 6, 9, 2 }, { 7, 0, 1 } };

/* ComputeHeading (IDA seg028_2941_803): the FM Towns function after TerrainCheck, with the
   same tables, the same sums over the four corners and the same 8-way switch. */
void far ComputeHeading(void)
{
    char ox;
    char oy;
    char sy;
    char sx;
    unsigned char want;
    unsigned char head;
    unsigned char k;
    unsigned char a;
    unsigned char b;
    int nopen;
    int nsteep;
    char i;

    oy = ox = sx = sy = nopen = nsteep = 0;
    for (i = 0; i < 4; i++) {
        if ((pnt[i].flags & 0xF8) == 0) {
            oy += side[i];
            ox += side[(i + 3) & 3];
            nopen++;
        }
        if (pnt[i].flags & 0x300) {
            sx += side[i];
            sy += side[(i + 3) & 3];
            nsteep++;
        }
    }
    if (nsteep) {
        curP->slope = dirs[sx / nsteep + 1][sy / nsteep + 1];
        if (nsteep == 1 && curP->slope % 2 && firstsolve) {
            want = (9 - curP->slope) & 7;
            head = curP->w6 >> 13;
            switch ((head - want) & 7) {
            case 0:
            case 1:
                curP->slope = 9;
                break;
            case 2:
            case 3:
                curP->slope = (curP->slope + 1) & 7;
                break;
            case 4:
            case 5:
                k = (curP->slope - 1) >> 1;
                curP->slope--;
                switch (k) {
                case 0:
                    a = pnt[k].x;
                    b = pnt[k].y;
                    break;
                case 1:
                    a = pnt[k].x;
                    b = 8 - pnt[k].y;
                    break;
                case 2:
                    a = pnt[k].y;
                    b = pnt[k].x;
                    break;
                case 3:
                    a = 8 - pnt[k].x;
                    b = pnt[k].y;
                    break;
                }
                if (a < b)
                    curP->slope = (curP->slope + 2) & 7;
                if (a == b)
                    curP->slope++;
                break;
            case 6:
            case 7:
                curP->slope = (curP->slope + 7) & 7;
                break;
            }
        }
    } else
        curP->slope = 9;
    if (nopen == 1 || nopen == 2)
        curP->open = dirs[oy / -nopen + 1][ox / -nopen + 1];
    else
        curP->open = 9;
}

/* get_home_tile (IDA GetTileAttribute8): FM Towns returns the centre tile's type too. */
int far get_home_tile(void)
{
    return tiles[4] & 0xF;
}

/* obj_coll_check (IDA CreateCollisionRecord): the FM Towns function before ObjectCheck,
   testing the same four bounds and filling an oCollisions record. */
void far obj_coll_check(struct Object far *obj, int link, char x, char y, char isnpc)
{
    char x0;
    char y0;
    char x1;
    char y1;
    struct ComObj com;
    register struct Collision *c;

    if (curP->found > 8)
        return;
    com = ComObjData[OBJ_ITEM(obj)];
    if (com.radius == 4) {
        x1 = (x0 = x << 3) + 7;
        y1 = (y0 = y << 3) + 7;
    } else {
        register int r;

        x0 = (x << 3) + OBJ_FINEX(obj);
        y0 = (y << 3) + OBJ_FINEY(obj);
        r = com.radius;
        if (OBJ_MAJOR(obj) == 1 && r > 0 && isnpc)
            r--;
        x1 = x0 + r;
        y1 = y0 + r;
        x0 = x0 - r;
        y0 = y0 - r;
    }
    if (x1 < xlow)
        return;
    if (x0 > xhigh)
        return;
    if (y1 < ylow)
        return;
    if (y0 > yhigh)
        return;
    c = &oCollisions[curP->found++];
    c->bottom = OBJ_Z(obj);
    c->top = c->bottom + com.height;
    if (com.height == 0)
        c->top = c->top + 1;
    c->link.index = link;
    c->link.bits = 9;
    if (pos_x >= x0 && pos_x <= x1 && ypos >= y0 && ypos <= y1)
        c->link.bits |= 0x10;
    c->offset = x + (y << 6);
}

void far ObjectCheck(unsigned char flat, unsigned char useflag)
{
    struct Tile far *tile;
    struct Link far *link;
    struct Object far *obj;
    char x0;
    char y0;
    char x1;
    char y1;
    char i;
    char j;
    char radius;
    char isnpc;
    struct ComObj far *com;
    int n;
    int off;

    isnpc = 0;
    tile = Map_GetAddr(curP->x >> 3, curP->y >> 3);
    if (curP->index != 0)
        isnpc = OBJ_MAJOR(Obj_IntTMem(curP->index)) == 1;
    curP->found = 0;
    pos_x = curP->x & 7;
    ypos = curP->y & 7;
    if (flat && curP->height == 0)
        radius = 0;
    else {
        radius = curP->radius;
        if (isnpc && radius > 0)
            radius--;
    }
    xhigh = pos_x + radius;
    yhigh = ypos + radius;
    xlow = pos_x - radius;
    ylow = ypos - radius;
    x0 = (xlow - 11) / 8;
    y0 = (ylow - 11) / 8;
    x1 = (xhigh + 4) / 8;
    y1 = (yhigh + 4) / 8;
    for (i = x0; i <= x1; i++) {
        for (j = y0; j <= y1; j++) {
            n = 0;
            off = (j << 6) + i;
            for (link = &tile[off].objects; link->index != 0 && n < 0x40;
                 link = &Obj_PtrTMem(link)->next, n++) {
                if (link->index == curP->index)
                    continue;
                obj = Obj_PtrTMem(link);
                com = &ComObjData[OBJ_ITEM(obj)];
                if (isnpc && com->c3_2)
                    continue;
                if (com->height == 0 && obj >= (struct Object far *)objdata)
                    continue;
                if (obj < (struct Object far *)objdata && OBJ_MAJOR(obj) != 1 && OBJ_B15_7(obj) != 0)
                    continue;
                if (!useflag || com->c6_0)
                    obj_coll_check(obj, link->index, i, j, isnpc);
            }
            if (n == 0x40)
                return;
        }
    }
}

/* oCswap (IDA SwapCollisionRecords): the FM Towns function between ObjectCheck and
   process_objlist, swapping two oCollisions records. */
void far oCswap(unsigned char i)
{
    struct Collision t;

    t = oCollisions[i];
    oCollisions[i] = oCollisions[i + 1];
    oCollisions[i + 1] = t;
}

void far process_objlist(void)
{
    char k;
    char m;
    char n;
    char flat;

    flat = 0;
    if (curP->height == 0)
        flat = 1;
    for (n = 0; curP->found > n; n++) {
        for (k = curP->found - 2; k >= n; k--)
            if (oCollisions[k].top > oCollisions[k + 1].top)
                oCswap(k);
        if (oCollisions[n].top > curP->z)
            break;
    }
    for (k = n; curP->found > k; k++)
        for (m = curP->found - 2; m >= k; m--)
            if (oCollisions[m].bottom > oCollisions[m + 1].bottom)
                oCswap(m);
    curP->first = n;
    for (curP->count = 0; n + curP->count < curP->found
         && curP->z + curP->height + flat > oCollisions[n + curP->count].bottom; curP->count++)
        ;
}

unsigned char far can_place(int item, int index, int x, int y, int z, unsigned char flier, unsigned char range)
{
    int radius;
    struct MotionCalc near *oldP;
    unsigned char ok;
    struct MotionCalc calc;

    oldP = curP;
    curP = &calc;
    curP->index = index;
    curP->radius = ComObjData[item].radius;
    curP->height = ComObjData[item].height;
    curP->x = x;
    curP->y = y;
    curP->z = z;
    if (curP->height != 0x80 && curP->height + curP->z > 0x7F) {
        ok = 0;
        goto out;
    }
    TerrainCheck(range);
    if ((curP->hits0 | curP->hits1) & 0x300) {
        ok = 0;
        goto out;
    }
    nvokHgt = curP->z + range >= curP->top ? curP->top : curP->floor;
    radius = curP->radius;
    if (range > radius)
        radius = range;
    if (curP->z <= curP->floor + radius)
        nvokTerr = 1 << (curP->hits0 & 3);
    else
        nvokTerr = 0x10;
    ObjectCheck(nvokTerr != 0x10 && index >= 0x100, 1);
    if (curP->found != 0) {
        int i;
        int best;

        best = -1;
        process_objlist();
        if (curP->count != 0) {
            ok = 0;
            goto out;
        }
        if (curP->found != 0)
            for (i = 0; i < curP->first; i++)
                if (oCollisions[i].top > nvokHgt)
                    nvokHgt = oCollisions[best = i].top;
        if (best > -1) {
            if (!ComObjData[OBJ_ITEM(Obj_PtrTMem(&oCollisions[best].link))].solid) {
                ok = 0;
                goto out;
            }
            nvokTerr = 1;
        }
    }
    if (!flier && (curP->hits0 | curP->hits1) & 0x800 && curP->z - range > nvokHgt)
        ok = 0;
    else
        ok = 1;
out:
    curP = oldP;
    return ok;
}

unsigned char far drop_around_place(struct Object far *obj, int x, int y, int z, int range);

unsigned char far put_at(int x, int y, int z, struct Object far *obj, int range, unsigned char nocull)
{
    if (drop_around_place(obj, x, y, z, range))
        return 1;
    if (nocull || !Obj_Elem_Fate(10, obj)) {
        SET_FINEX(obj, x & 7);
        SET_FINEY(obj, y & 7);
        Obj_Add(&Map_GetAddr(x >> 3, y >> 3)->objects, obj);
        return 1;
    }
    Obj_FreeLinkChain(0L, obj);
    return 0;
}

unsigned char far near_mob_put_at(struct Object far *src, struct Object far *obj, int range, unsigned char nocull)
{
    return put_at((OBJ_HOMEX(src) << 3) + OBJ_FINEX(src), (OBJ_HOMEY(src) << 3) + OBJ_FINEY(src),
                  OBJ_Z(src), obj, range, nocull);
}

char stay_centered = 0;

unsigned char far drop_around_place(struct Object far *obj, int x, int y, int z, int range)
{
    unsigned char tries;
    int span;
    int zz;
    struct Tile far *tile;
    int tx;
    int ty;

    span = range * 2 + 1;
    for (tries = 0; tries < 24; tries++) {
        if (tries == 0 && stay_centered) {
            tx = x;
            ty = y;
        } else {
            tx = x - range + rand() % span;
            ty = y - range + rand() % span;
        }
        zz = z;
        if (can_place(OBJ_ITEM(obj), Obj_MemTPtr(obj), tx, ty, zz, 1, 0)) {
            tile = Map_GetAddr(tx >> 3, ty >> 3);
            SET_FINEX(obj, tx & 7);
            SET_FINEY(obj, ty & 7);
            SET_Z(obj, zz);
            Obj_AddEnd(&tile->objects, obj);
            if (IsMobElem(obj)) {
                SET_HOMEX(obj, tx >> 3);
                SET_HOMEY(obj, ty >> 3);
            } else
                obj_deal(obj, tx >> 3, ty >> 3, 1);
            return 1;
        }
    }
    return 0;
}
