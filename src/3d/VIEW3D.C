/* target: seg032_2E9B */
/* opts: -mm -1 -G -O -d */
/* VIEW3D.C: setting up and drawing a frame of the 3D view.

   Entry points: place_3d_view (the view window and zoom, at start-up), init_3d (the
   renderer and an empty render database), establish_view (draw a frame and copy it to
   the screen; the main loop's do_3d_view), render_FB (draw a frame into the frame
   buffer only, for fades and screens drawn over the view), do_3d_grab (draw a pick
   frame for pick_3d), set_cyb (the hallucination effect).

   A frame: setup_vars copies the player's position into the camera (get_eye fills
   cPlayer) and turns it into the first quadrant, so the grid code only has to look one
   way (quad says how far it was turned; chgtable and trans_grid turn map steps and tile
   types to match). do_2dclip then builds the vision grid (init_grid, build_grid: two
   edge rays leave the eye at the heading +-0x2040 and walk outwards row by row,
   narrowing at walls, and every grid cell between them gets the faces it shows,
   enc_n_chk), and GRIDDB.C's process_grid turns the grid into render-database bytecode.
   send_db sets the clip window to the view, has seg021's cRender run the database
   (seg004's render_3d) into the frame buffer, and copies the frame buffer to the screen
   with cFBtoScreen, hiding the mouse cursor around the copy.

   Data owned: the vision grid glocs (17 rows of 33 cells, the eye at row 0 column 16;
   struct Gloc in view3d.h) and its edge list gvecs/gvechead; the quadrant tables; the
   view size (xwid, xhgt), zoom and demo_mode; DbEntry, where each frame's bytecode
   starts.

   name: descriptive (map/filenames.tsv: "setting up the 3D view and the vision grid").
   The whole of DOS resident segment seg032_2E9B, in original order. Function and
   global names are the originals from the FM Towns symbol table where it has them; the
   source file's own name is not known. */

#include <dos.h>
#include <stdlib.h>
#include "conv.h"
#include "critter.h"
#include "event.h"
#include "gfx.h"
#include "map.h"
#include "motion.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"
#include "view3d.h"

/* An edge of the vision arc walking the grid. The list is chained by the low nibble of link,
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
    { 1, MAP_SIZE, -1 }, { -MAP_SIZE, 1, MAP_SIZE }, { -1, -MAP_SIZE, 1 }, { MAP_SIZE, -1, -MAP_SIZE }
};
/* The heading of each quadrant's turn, subtracted from the camera's heading. */
unsigned headmod[4] = { 0, 0x4000, 0x8000, 0xC000 };
/* Tile types as seen from each quadrant. */
unsigned char trans_grid[4][16] = {
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
    { 0, 1, 4, 2, 5, 3, 9, 8, 6, 7 },
    { 0, 1, 5, 4, 3, 2, 7, 6, 9, 8 },
    { 0, 1, 3, 5, 2, 4, 8, 9, 7, 6 }
};
static char cyb_on = 0;
/* The faces seen of each tile type (after trans_grid) from each of 7 directions
   (enc_n_chk's sel: straight ahead, left or right, nearer the row or the column, on
   the diagonal): the grid flags, 0x80 visible, 0x04 a slope and 0x44 a diagonal (the
   low two bits say which), 0x20/0x10/0x08 the walls that may be seen. */
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
static int side_dark[2] = { TW_NORTH, 0 };
static int side_wall[2] = { TW_WEST, TW_EAST };
static int side_tile[2] = { 2, 3 };
static int side_step[2] = { -1, 1 };

/* match: this file's _BSS, DS:26EA..2C67 (seg031's ends at 26E9; seg019's starts at 2C68 with
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
/* seg_5DFD (declared in gfx.h) is the 4 KB far buffer at 5DFD:0000 (segment table entry
   60) that seg004's texture loader copies a bitmap into; seg032_2E9B_195 points the
   bitmap table's segments back at it.
   name: seg_5DFD is DOS only, no FM Towns name: IDA's segment name. */


/* Place the view window (w by h at x, y; y is its bottom row, since mous_player gets
   y - h + 1 as the top),
   tell the mouse code where it is, and set the zoom: 0x6062 when inplist->mode has
   bit 3, 0x61A8 and demo_mode when it has bit 0, else 0x7ED2. */
void far place_3d_view(int x, int y, int w, int h)
{
    demo_mode = 0;
    xwid = w;
    xhgt = h;
    cPlaceFB(x, y, w, h);
    mous_3d_set(x, y, w, h);
    mous_player(x, y - h + 1, w, h);
    if (inplist->mode & 8)
        curZoom = 0x6062;
    else if (inplist->mode & 1) {
        demo_mode = 1;
        curZoom = 0x61A8;
    }
    else
        curZoom = 0x7ED2;
    cZoom(curZoom);
}

/* The hallucination effect (PLAYDATA.C turns it on for one of its three random
   effects while ShroomsEnabled, and off afterwards): word 1 of the first six 8-byte
   entries of the renderer's bitmap table (at bmsegoff) gets random values, or its
   normal values 0xF0, 0xF0, 0x3E0, 0x3E0, 0xFC0, 0xFC0 back. Those words are probably
   the texture-coordinate masks of the six bitmap slots, so random ones scramble the
   textures; this has not been checked in seg004.
   name: IDA seg032_2E9B_9B. FM Towns set_cyb_ sits at the same place after
   place_3d_view_ and does the same. */
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

/* This and the next have no FM Towns counterpart and no callers in DOS (probably
   debugging leftovers). The next toggles SpecShadeMode, and when it was on points the
   segments of bitmap slots 1, 3 and 5 at the seg_5DFD buffer. */
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

/* Initialise the renderer (cInit3d), start an empty render database with a 0 word at
   its entry (DbEntry), and build the distance shades for range 8. */
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

/* Run the database: render into the frame buffer with the clip window set to the
   view, and copy the frame buffer to the screen. The window is put back to the full
   320x200 screen afterwards. */
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

/* Render a pick frame (GRIDDB.C's do_3d_pickup) into the frame buffer without
   showing it; pick_3d then reads the colour under the cursor. It reuses the vision
   grid of the last frame. */
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

/* Draw a frame and show it. strtime records the tick count at the start. */
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

/* Turn the camera into the first quadrant: the grid code only looks one way. The
   heading's octant picks the quadrant (q), the camera's position within its tile is
   rotated to match and the quadrant's heading taken off, and mapptr is the eye's
   tile. loct is set when the heading is in an odd octant. Always returns 1. */
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
    h = (unsigned)cPlayer->heading >> 13;
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

/* Fill the shade of every grid cell from its distance to the eye: cells beyond range
   get 15 (dark, and treated as not seen by the grid walk), the rest a shade of 0..14
   from smooth_div, smooth_lowpass and smooth_base (renderer data). GAMESORT.C makes
   the same calculation for each object.
   name: IDA ShadeCalcs; FM Towns preset_grid_ follows setup_vars_ and is the same code
   (cSqRt, smooth_div, glocs+1). */
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

/* Start the two edges of the vision arc at the eye, at the heading -0x2040 (gvecs[0])
   and +0x2040 (gvecs[1]), a little over a quarter turn apart. In a solid tile there is
   no arc (gvechead 15) and nothing is drawn.
   name: IDA SetRangeOfVisonParams; FM Towns init_grid_ follows preset_grid_ and fills
   gvecs and gvechead the same way. */
void far init_grid(void)
{
    struct Gloc *g = &glocs[0][16];

    if (mapptr->type == TILE_SOLID)
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
            if ((tile_walls[other = trans_grid[quad][v->map[chgtable[quad][1]].type]] & TW_SOUTH) == 0) {
                if (v->map[chgtable[quad][1]].height + ((tile_walls[other] & TW_SLOPE) == TW_SLOPE) - (other == TILE_SLOPE_N)
                    <= v->map->height + (here == 6) + (other == here && here != 1))
                    code -= 0x10;
                else
                    shade += 0x20;
            }
        }
        if (code & 0x20) {
            if ((tile_walls[other = trans_grid[quad][v->map[chgtable[quad][0]].type]] & TW_WEST) == 0) {
                if (v->map[chgtable[quad][0]].height + ((tile_walls[other] & TW_SLOPE) == TW_SLOPE) - (other == TILE_SLOPE_E)
                    <= v->map->height + (here == 8) + (other == here && here != 1))
                    code -= 0x20;
                else
                    shade += 0x40;
            }
        }
        if (code & 8) {
            if ((tile_walls[other = trans_grid[quad][(v->map - chgtable[quad][0])->type]] & TW_EAST) == 0) {
                if ((v->map - chgtable[quad][0])->height + ((tile_walls[other] & TW_SLOPE) == TW_SLOPE) - (other == TILE_SLOPE_W)
                    <= v->map->height + (here == 9) + (other == here && here != 1))
                    code -= 8;
                else
                    shade += 0x10;
            }
        }
        v->loc->flags = code;
        v->loc->shade = shade;
        if (dir) {
            if ((tile_walls[trans_grid[quad][v->map[chgtable[quad][1]].type]] & TW_SOUTH) == want
                    && side_dark[want == TW_SOUTH] == (tile_walls[here] & TW_NORTH)
                || (tile_walls[here] & TW_DIAG) == TW_DIAG
                    && side_dark[want == 0] == (tile_walls[here] & TW_NORTH)) {
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

/* Move both edges on to the next row; 0 when the arc has closed. Each edge first
   moves inwards past cells beyond the shading range, and its direction is re-aimed
   from the eye through its current point, widened a little (abs(dx) / 50 + 2) so
   rounding does not lose a column. */
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

/* Walk one edge across its row to the next row boundary, stepping sideways through
   cells while its slope says it leaves through a side, and stopping at a side wall of
   either cell (tile_walls bits), at a cell beyond the range, or 16 columns out. loc2
   records the outermost cell the edge reached on this row, which fill_the_grid and
   build_grid use as the fill limits. */
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

/* Fill the cells between an edge and the next one on this row, marking each with
   enc_n_chk; where a wall splits the span the edges are moved, and when they cross the
   pair is dropped from the list. */
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

/* Build the vision grid row by row from the eye outwards until no edge pair is left;
   cells outside every span get flags 0. mxY is the last row reached. */
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

/* Build the vision grid and the frame's database; MousQUp(0) is called in between
   (probably to service the mouse during the frame). */
void far do_2dclip(void)
{
    init_grid();
    build_grid();
    MousQUp(0);
    process_grid();
}
