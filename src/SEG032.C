/* target: seg032_2E9B */
/* opts: -mm -1 -G -O -d */
/* Setting up the 3D view: placing the view window, the cycling colour table, building
   the render database, putting the camera into the first quadrant, the distance shading
   table, and the vision grid that walks two rays out from the eye across the map and
   marks which tiles can be seen. The whole of DOS resident segment seg032_2E9B, in
   original order. Function and global names are the originals from the FM Towns symbol
   table where it has them; the source file's own name is not known. */

#include <dos.h>
#include <stdlib.h>

struct Tile {
    unsigned type:4;
    unsigned height:4;
    unsigned b8:8;
    unsigned objects;                   /* 0x02 */
};

/* The camera, a copy of the player's position. */
struct Eye {
    char pad0[0x0A];
    int x;                              /* 0x0A, in 1/256 tiles */
    char pad1[0x12 - 0x0C];
    int y;                              /* 0x12 */
    char pad2[0x2C - 0x14];
    unsigned heading;                   /* 0x2C */
};

struct Inplist {
    char pad0[8];
    int field8;
};

/* One cell of the vision grid, 33 cells to a row and 17 rows, the eye at row 0 column 16. */
struct Gloc {
    unsigned char flags;                /* which faces are seen */
    unsigned char shade;                /* distance shade in bits 0-3 */
};

/* A ray edge walking the vision grid. The list is chained by the low nibble of link,
   15 ending it; bit 7 of link says which side of the arc the edge is on. */
struct Gvec {
    char link;                          /* 0x00 */
    int dx;                             /* 0x01 */
    int dy;                             /* 0x03 */
    signed char x;                      /* 0x05, tile column relative to the eye */
    unsigned char fx;                   /* 0x06 */
    signed char y;                      /* 0x07, tile row */
    unsigned char fy;                   /* 0x08 */
    struct Tile far *map;               /* 0x09 */
    struct Gloc *loc;                   /* 0x0D */
    struct Gloc *loc2;                  /* 0x0F */
};

/* Map steps for the four quadrants: along x, along y, and the diagonal. */
int chgtable[4][3] = {
    { 1, 0x40, -1 }, { -0x40, 1, 0x40 }, { -1, -0x40, 1 }, { 0x40, -1, -0x40 }
};
unsigned headmod[4] = { 0, 0x4000, 0x8000, 0xC000 };
/* Tile types as seen from each quadrant. */
unsigned char trans_grid[4][16] = {
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
    { 0, 1, 4, 2, 5, 3, 9, 8, 6, 7 },
    { 0, 1, 5, 4, 3, 2, 7, 6, 9, 8 },
    { 0, 1, 3, 5, 2, 4, 8, 9, 7, 6 }
};
static char cyb_on = 0;
/* The faces seen of each tile type from each of 7 directions. */
unsigned char enc_dat[16][7] = {
    { 0 },
    { 0xB8, 0x98, 0xB0, 0x98, 0xB0, 0x98, 0xB0 },
    { 0xE4, 0xC4, 0xE4, 0xC4, 0xE0, 0xC4, 0xE4 },
    { 0xCD, 0xCD, 0xC5, 0xC9, 0xC5, 0xCD, 0xC5 },
    { 0xD6, 0xD2, 0x00, 0xD6, 0x00, 0xD6, 0x00 },
    { 0xD7, 0x00, 0xD3, 0x00, 0xD7, 0x00, 0xD7 },
    { 0xBC, 0x9C, 0xB4, 0x9C, 0xB4, 0x9C, 0xB4 },
    { 0xBD, 0x9D, 0xB5, 0x9D, 0xB5, 0x9D, 0xB5 },
    { 0xBE, 0x9E, 0xB6, 0x9E, 0xB6, 0x9E, 0xB6 },
    { 0xBF, 0x9F, 0xB7, 0x9F, 0xB7, 0x9F, 0xB7 }
};
unsigned char flciel[16] = { 0x00, 0x80, 0xC0, 0xC1, 0xC2, 0xC3, 0x84, 0x85, 0x86, 0x87 };
unsigned char tile_to_five[16] = { 0, 0, 0, 0, 0, 0, 1, 2, 3, 4 };
/* Per side of the arc (0 left, 1 right). */
static int side_dark[2] = { 0x10, 0 };
static int side_wall[2] = { 2, 4 };
static int side_tile[2] = { 2, 3 };
static int side_step[2] = { -1, 1 };

extern struct Inplist near *inplist;
extern unsigned long far *Time;
extern struct Eye far *cPlayer;
extern int far *dbptr;
extern int lastXeye, lastYeye;
/* This file's _BSS, DS:26EA..2C67 (seg031's ends at 26E9; seg019's starts at 2C68 with
   cWCol, key 27), laid out by name (tools/bssorder.py): lcldblen 148, xhgt 176, glocs 191,
   mxY 237, quad 377, strtime 411, mapptr 653, curZoom 667, gvecs 703, demo_mode 708,
   trans 804, gvechead 879, loct 900, DbEntry 916, xwid 960. All FM Towns names. */
int lcldblen;
int xwid, xhgt;
char demo_mode;
unsigned curZoom;
char quad;
char loct;
unsigned char *trans;
unsigned long strtime;
struct Tile far *mapptr;
int mxY;
struct Gloc glocs[17][33];
struct Gvec gvecs[15];
char gvechead;
int far *DbEntry;
extern unsigned char tile_walls[];
extern unsigned char SpecShadeMode;     /* DOS only, no FM Towns name */
/* in the graphics data segment */
extern unsigned far bmsegoff;
/* The 4 KB far buffer at 5DFD:0000 (segment table entry 60) that seg004's texture loader
   copies a bitmap into; seg032_2E9B_195 points the bitmap table's segments back at it.
   DOS only, no FM Towns name: IDA's segment name. */
extern unsigned char far seg_5DFD[];
extern int far _dblen;
extern int far smooth_div;
extern int far smooth_base;
extern int far smooth_lowpass;

void far cPlaceFB(int x, int y, int w, int h);
void far mous_3d_set(int x, int y, int w, int h);
void far mous_player(int x, int y, int w, int h);
void far cZoom(unsigned zoom);
void far cInit3d(void);
void far grdb_blank(void);
void far gr_tostrt(void);
void far gr_entry(void);
void far set_the_window(int x0, int y0, int x1, int y1);
void far cRender(void);
void far do_fbuf_bms(void);
void far mous_3d_hide(void);
void far cFBtoScreen(void);
void far mous_3d_show(void);
void far do_3d_pickup(void);
void far gr_putlab(int lab);
void far get_eye(void);
struct Tile far * far Map_GetAddr(int x, int y);
int far cSqRt(long v);
void far cSinCos(int angle, int *s, int *c);
void far MousQUp(int a);
void far process_grid(void);

char far setup_vars(void);
void far preset_grid(int range);
void far do_2dclip(void);

void far place_3d_view(int x, int y, int w, int h)
{
    demo_mode = 0;
    xwid = w;
    xhgt = h;
    cPlaceFB(x, y, w, h);
    mous_3d_set(x, y, w, h);
    mous_player(x, y - h + 1, w, h);
    if (inplist->field8 & 8)
        curZoom = 0x6062;
    else if (inplist->field8 & 1) {
        demo_mode = 1;
        curZoom = 0x61A8;
    }
    else
        curZoom = 0x7ED2;
    cZoom(curZoom);
}

/* IDA seg032_2E9B_9B. FM Towns set_cyb_ sits at the same place after place_3d_view_ and
   does the same: six rand() masks or six fixed values into the colour table. */
void far set_cyb(char on)
{
    int far *p = (int far *)&bmsegoff;

    p = MK_FP(FP_SEG(&bmsegoff), bmsegoff);
    p++;
    if (cyb_on != on) {
        cyb_on = on;
        if (on) {
            *p = rand() & 0xFF;
            p += 4;
            *p = rand() & 0xFF;
            p += 4;
            *p = rand() & 0x3FF;
            p += 4;
            *p = rand() & 0x3FF;
            p += 4;
            *p = rand() & 0xFFF;
            p += 4;
            *p = rand() & 0xFFF;
        }
        else {
            *p = 0xF0;
            p += 4;
            *p = 0xF0;
            p += 4;
            *p = 0x3E0;
            p += 4;
            *p = 0x3E0;
            p += 4;
            *p = 0xFC0;
            p += 4;
            *p = 0xFC0;
        }
        p += 4;
    }
}

/* This and the next have no FM Towns counterpart and no callers in DOS. */
void far seg032_2E9B_18B(void)
{
    quad = 0x42;
}

void far seg032_2E9B_195(int on)
{
    int far *p;
    register unsigned char old;
    register unsigned char want;

    old = SpecShadeMode;
    if (on == -1)
        want = !old;
    else
        want = on;
    if (want != old) {
        p = (int far *)&bmsegoff;
        p = MK_FP(FP_SEG(&bmsegoff), bmsegoff);
        if (SpecShadeMode) {
            p[4] = FP_SEG(seg_5DFD);
            p[12] = FP_SEG(seg_5DFD);
            p[20] = FP_SEG(seg_5DFD);
        }
        SpecShadeMode = !SpecShadeMode;
    }
}

void far init_3d(void)
{
    cInit3d();
    grdb_blank();
    gr_tostrt();
    gr_entry();
    DbEntry = dbptr;
    *dbptr++ = 0;
    preset_grid(8);
    lcldblen = _dblen;
}

void far reset_db(void)
{
    dbptr = DbEntry;
}

void far send_db(void)
{
    set_the_window(0, xhgt - 1, xwid - 1, 0);
    cRender();
    if (demo_mode)
        do_fbuf_bms();
    mous_3d_hide();
    cFBtoScreen();
    mous_3d_show();
    set_the_window(0, 0xC7, 0x13F, 0);
}

void far do_3d_grab(void)
{
    reset_db();
    do_3d_pickup();
    gr_putlab(0xA0);
    *dbptr++ = 0;
    set_the_window(0, xhgt - 1, xwid - 1, 0);
    cRender();
    set_the_window(0, 0xC7, 0x13F, 0);
}

void far render_FB(void)
{
    setup_vars();
    reset_db();
    do_2dclip();
    gr_putlab(0xA0);
    *dbptr++ = 0;
    set_the_window(0, xhgt - 1, xwid - 1, 0);
    cRender();
    set_the_window(0, 0xC7, 0x13F, 0);
}

void far establish_view(void)
{
    strtime = *Time;
    if (setup_vars()) {
        reset_db();
        do_2dclip();
        gr_putlab(0xA0);
        *dbptr++ = 0;
    }
    send_db();
}

/* Turn the camera into the first quadrant: the grid code only looks one way. */
char far setup_vars(void)
{
    char q;
    char ok;
    int t;
    int h;

    get_eye();
    lastXeye = cPlayer->x >> 8;
    lastYeye = cPlayer->y >> 8;
    mapptr = Map_GetAddr(lastXeye, lastYeye);
    h = cPlayer->heading >> 13;
    q = ((h + 1) & 7) >> 1;
    trans = trans_grid[q];
    ok = 1;
    loct = h % 2;
    quad = q;
    cPlayer->x &= 0xFF;
    cPlayer->y &= 0xFF;
    switch (quad) {
    case 1:
        t = cPlayer->x;
        cPlayer->x = 0xFF - cPlayer->y;
        cPlayer->y = t;
        break;
    case 2:
        cPlayer->x = 0xFF - cPlayer->x;
        cPlayer->y = 0xFF - cPlayer->y;
        break;
    case 3:
        t = cPlayer->x;
        cPlayer->x = cPlayer->y;
        cPlayer->y = 0xFF - t;
        break;
    }
    cPlayer->heading = cPlayer->heading - headmod[quad];
    return ok;
}

/* Fill the shade of every grid cell from its distance to the eye. IDA ShadeCalcs; FM Towns
   preset_grid_ follows setup_vars_ and is the same code (cSqRt, smooth_div, glocs+1). */
void far preset_grid(int range)
{
    int d;
    int t;
    int u;
    char shades[16];
    int i, j;

    if (range < 16) {
        for (i = 0; i < 16; i++) {
            if (i > range)
                shades[i] = 15;
            else {
                t = (i << 8) >> 5;
                u = t * t;
                t = u * 2;
                u = cSqRt(t);
                t = (u * smooth_div) >> 6;
                t += smooth_lowpass;
                if (t < 0)
                    t = 0;
                u = t + smooth_base;
                if (u > 14)
                    u = 14;
                shades[i] = u;
            }
        }
        for (j = 0; j < 17; j++)
            for (i = 0; i < 33; i++) {
                if ((d = cSqRt((16 - i) * (16 - i) + j * j)) > range)
                    glocs[j][i].shade = 15;
                else
                    glocs[j][i].shade = shades[d];
            }
    }
}

/* Start the two edges of the vision arc at the eye. IDA SetRangeOfVisonParams; FM Towns
   init_grid_ follows preset_grid_ and fills gvecs and gvechead the same way. */
void far init_grid(void)
{
    struct Gloc *g = &glocs[0][16];

    if (mapptr->type == 0)
        gvechead = 15;
    else {
        gvechead = 0;
        gvecs[0].link = 0x81;
        gvecs[0].x = 0;
        gvecs[0].y = 0;
        gvecs[0].fx = cPlayer->x;
        gvecs[0].fy = cPlayer->y;
        gvecs[0].map = mapptr;
        gvecs[0].loc = g;
        gvecs[1].link = 15;
        gvecs[1].x = 0;
        gvecs[1].y = 0;
        gvecs[1].fx = cPlayer->x;
        gvecs[1].fy = cPlayer->y;
        gvecs[1].map = mapptr;
        gvecs[1].loc = g;
        cSinCos(cPlayer->heading + 0x2040, &gvecs[1].dx, &gvecs[1].dy);
        cSinCos((int)cPlayer->heading - 0x2040, &gvecs[0].dx, &gvecs[0].dy);
        gvecs[0].dx >>= 4;
        gvecs[0].dy >>= 4;
        gvecs[1].dx >>= 4;
        gvecs[1].dy >>= 4;
    }
}

void far incvec(struct Gvec *v)
{
    v->x++;
    v->map += chgtable[quad][0];
    v->loc++;
}

void far decvec(struct Gvec *v)
{
    v->x--;
    v->map -= chgtable[quad][0];
    v->loc--;
}

/* Mark which faces of the edge's tile are seen; with dir set, step the edge sideways
   past a wall of the kind asked for. */
char far enc_n_chk(register struct Gvec *v, char dir, char want)
{
    int code;
    int shade;
    int sel;
    int other;
    register int here;

    shade = v->loc->shade & 0xF;
    here = trans_grid[quad][v->map->type];
    if (v->x == 0)
        sel = 0;
    else {
        sel = (v->x > 0) + 1;
        if (abs(v->x) > abs(v->y))
            sel += 2;
        if (abs(v->x) == abs(v->y))
            sel += 4;
    }
    if ((code = enc_dat[here][sel]) == 0)
        v->loc->flags = 0;
    else {
        if (code & 0x10) {
            if ((tile_walls[other = trans_grid[quad][v->map[chgtable[quad][1]].type]] & 8) == 0) {
                if (v->map[chgtable[quad][1]].height + ((tile_walls[other] & 0x20) == 0x20) - (other == 6)
                    <= v->map->height + (here == 6) + (other == here && here != 1))
                    code -= 0x10;
                else
                    shade += 0x20;
            }
        }
        if (code & 0x20) {
            if ((tile_walls[other = trans_grid[quad][v->map[chgtable[quad][0]].type]] & 2) == 0) {
                if (v->map[chgtable[quad][0]].height + ((tile_walls[other] & 0x20) == 0x20) - (other == 8)
                    <= v->map->height + (here == 8) + (other == here && here != 1))
                    code -= 0x20;
                else
                    shade += 0x40;
            }
        }
        if (code & 8) {
            if ((tile_walls[other = trans_grid[quad][(v->map - chgtable[quad][0])->type]] & 4) == 0) {
                if ((v->map - chgtable[quad][0])->height + ((tile_walls[other] & 0x20) == 0x20) - (other == 9)
                    <= v->map->height + (here == 9) + (other == here && here != 1))
                    code -= 8;
                else
                    shade += 0x10;
            }
        }
        v->loc->flags = code;
        v->loc->shade = shade;
        if (dir) {
            if ((tile_walls[trans_grid[quad][v->map[chgtable[quad][1]].type]] & 8) == want
                    && side_dark[want == 8] == (tile_walls[here] & 0x10)
                || (tile_walls[here] & 1) == 1
                    && side_dark[want == 0] == (tile_walls[here] & 0x10)) {
                if (dir == 1) {
                    incvec(v);
                    v->fx = 0;
                }
                else {
                    decvec(v);
                    v->fx = 0xFF;
                }
                return 1;
            }
        }
    }
    return 0;
}

/* Move both edges on to the next row; 0 when the arc has closed. */
char far newdels(struct Gvec *a, struct Gvec *b)
{
    if (++a->y > 16)
        return 0;
    while ((a->loc[33].shade & 0xF) == 0xF) {
        incvec(a);
        a->fx = 0;
        enc_n_chk(a, 0, 0);
        if (a->x > b->x)
            return 0;
    }
    if (a->y > 1 || abs(a->fx - cPlayer->x) + abs(a->fy - cPlayer->y) > 16) {
        a->dx = (a->x << 8) + a->fx - cPlayer->x;
        a->dx -= abs(a->dx) / 50 + 2;
        a->dy = (a->y << 8) - cPlayer->y;
    }
    a->map += chgtable[quad][1];
    a->loc += 33;
    a->fy = 0;
    b->y++;
    while ((b->loc[33].shade & 0xF) == 0xF) {
        decvec(b);
        b->fx = 0xFF;
        enc_n_chk(b, 0, 0);
        if (a->x > b->x)
            return 0;
    }
    if (b->y > 1 || abs(b->fx - cPlayer->x) + abs(b->fy - cPlayer->y) > 16) {
        b->dx = (b->x << 8) + b->fx - cPlayer->x;
        b->dx += abs(b->dx) / 50 + 2;
        b->dy = (b->y << 8) - cPlayer->y - 1;
    }
    b->fy = 0;
    b->loc += 33;
    b->map += chgtable[quad][1];
    return 1;
}

/* Walk one edge across its row to the next row boundary. */
void far move_to_next(struct Gvec *v)
{
    int rem;
    char t;
    char go;
    int side;

    v->dx < 0 ? (side = 0) : (side = 1);
    side == 1 ? (rem = 0x100 - v->fx) : (rem = v->fx);
    if (!((v->link & 0x80) ^ (side << 7)))
        v->loc2 = v->loc;
    if (v->dy == 0)
        go = 1;
    else if (v->dx == 0)
        go = 0;
    else
        go = (long)side_step[side] * v->dx * (0x100 - v->fy) > (long)rem * v->dy;
    while (go) {
        if (!(tile_walls[t = trans_grid[quad][v->map->type]] & side_wall[side])
            && !(tile_walls[trans_grid[quad][v->map[side_step[side] * chgtable[quad][0]].type]]
                & side_wall[(side + 1) % 2])) {
            v->fy += (long)v->dy * rem / ((long)side_step[side] * v->dx);
            v->fx = ((side + 1) % 2) * 0xFF;
            rem = 0x100;
            if (!((v->link & 0x80) ^ (side << 7)))
                enc_n_chk(v, 0, 0);
            if (side_step[side] == 1)
                incvec(v);
            else
                decvec(v);
            if ((v->loc->shade & 0xF) == 0xF || abs(v->x) > 16) {
                if (side_step[(side + 1) % 2] == 1)
                    incvec(v);
                else
                    decvec(v);
                v->fx = side * 0xFF;
                v->fy = 0xFF;
                if ((v->link & 0x80) ^ (side << 7))
                    v->loc2 = v->loc;
                return;
            }
        }
        else {
            v->fy = 0xFF;
            if ((tile_walls[t] & side_wall[side]) == side_wall[side]) {
                if (side_tile[side] == t)
                    v->fx = ((side + 1) % 2) * 0xFF;
                else
                    v->fx = side * 0xFF;
            }
            else
                v->fx = side * 0xFF;
            if ((v->link & 0x80) ^ (side << 7))
                v->loc2 = v->loc;
            return;
        }
        if (v->dy == 0)
            go = 1;
        else
            go = (long)side_step[side] * v->dx * (0x100 - v->fy) > (long)rem * v->dy;
    }
    v->fx += side_step[side] * (int)((long)side_step[side] * v->dx * (0xFFL - (unsigned)v->fy) / v->dy);
    v->fy = 0xFF;
    if ((v->link & 0x80) ^ (side << 7))
        v->loc2 = v->loc;
}

/* Fill the cells between an edge and the next one on this row. */
void far fill_the_grid(struct Gvec **cur, struct Gloc **out)
{
    struct Gvec tmp;
    struct Gvec *a, *b;

    a = &gvecs[(*cur)->link & 0xF];
    b = &gvecs[a->link & 0xF];
    *out = b->loc2;
    (*out)++;
    while (enc_n_chk(a, 1, 8)) {
        if ((a->x << 8) + a->fx > (b->x << 8) + b->fx) {
            (*cur)->link = ((*cur)->link & 0xF0) + (b->link & 0xF);
            a->link = 0;
            b->link = 0;
            return;
        }
    }
    if (a->x < b->x)
        while (enc_n_chk(b, -1, 8))
            ;
    tmp = *a;
    if (newdels(a, b) == 0) {
        (*cur)->link = ((*cur)->link & 0xF0) + (b->link & 0xF);
        a->link = 0;
        b->link = 0;
        return;
    }
    *cur = b;
    if (a->x != b->x) {
        incvec(&tmp);
        while (b->x > tmp.x) {
            while (enc_n_chk(&tmp, 1, 0))
                if (b->x <= tmp.x)
                    break;
            if (b->x > tmp.x) {
                incvec(&tmp);
                while (enc_n_chk(&tmp, 1, 8))
                    if (b->x <= tmp.x)
                        break;
                incvec(&tmp);
            }
        }
    }
}

void far build_grid(void)
{
    struct Gloc *g;
    struct Gvec *cur;
    struct Gvec *v;
    struct Gloc *end;

    mxY = -1;
    g = &glocs[0][0];
    do {
        mxY++;
        for (cur = (struct Gvec *)&gvechead; (cur->link & 0xF) != 0xF; cur = v) {
            v = &gvecs[cur->link & 0xF];
            move_to_next(v);
        }
        cur = (struct Gvec *)&gvechead;
        end = g + 33;
        while ((cur->link & 0xF) != 0xF) {
            while (gvecs[cur->link].loc2 > g) {
                g->flags = 0;
                g = g + 1;
            }
            fill_the_grid(&cur, &g);
        }
        while (g < end) {
            g->flags = 0;
            g = g + 1;
        }
    } while (gvechead != 15);
}

void far do_2dclip(void)
{
    init_grid();
    build_grid();
    MousQUp(0);
    process_grid();
}
