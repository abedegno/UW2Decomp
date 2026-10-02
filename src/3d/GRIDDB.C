/* target: seg019_21BA */
/* opts: -mm -1 -G -O -Y -d */
/* GRIDDB.C: building the 3D view's render database from the map grid.

   The render database is a buffer of model interpreter bytecode (dbptr writes into it;
   conv/GRDB.C's grdb_blank, Ref and gr_putlab manage its labels) that seg004's
   render_3d runs when VIEW3D.C calls cRender. Each word this file emits is an opcode
   or an operand; the opcode numbers are those of seg004's opcode table, which follows
   FM Towns' numbering (docs/LAYOUT.md; UWReverseEngineering's "UW2 FM Towns/
   model_opcodes.tsv" names each handler). The ones used here: 0x02 do_movec (store a
   constant in a model variable, Clk(n)), 0x2E do_setcolor, 0x7E do_polyres (a flat
   polygon of n points), 0x3E do_fetchmap (load a texture into a bitmap slot), 0x36
   do_tmap and 0xA0/0xA2 do_compact_tmap/do_compact_wtmap (texture-mapped faces, 0xA2
   when the view is level), 0xB0 do_mouseq (end of a grid row), 0xB6 do_minires
   (SetPnt's point records), 0x38 do_obj and 0xD0 do_set_gmap_ctxt (the frame header).

   Entry points: process_grid (a normal frame, from VIEW3D.C's do_2dclip after the
   vision grid is built), do_3d_pickup (a pick frame, from do_3d_grab: every face and
   object is drawn flat in a colour that identifies it), set_graphics_level (the detail
   setting chooses which faces are texture mapped). subprocess walks the visible tiles
   of the vision grid (VIEW3D.C's glocs) from the farthest row to the eye, each row
   from both edges inwards to the middle column, so nearer tiles are drawn later;
   grdb_elem emits one tile's floor, ceiling, walls and diagonal face and hands its
   object list to GAMESORT.C's do_objsort; the poly and txt functions emit one flat or
   texture-mapped face.

   Data owned: the pick tables color_to_map and color_to_obj, the walk state (loopx,
   loopy, tmptr, p_gloc), the current texture-map context (cTmBm, cTmDm, cTmHg, cTmSz),
   the draw function pointers and the tables of tile shapes. It also marks the automap:
   every tile the walk passes is written into PlayersMap with its automap code, and
   tiles that come into view for the first time count towards a small experience award
   (pipeexp).

   Neighbours: VIEW3D.C (the vision grid, quadrant and camera), GAMESORT.C and
   DRAWOBJ.C (objects), SETPNT.ASM (points), seg004 (runs the bytecode).

   name: descriptive (map/filenames.tsv: "the render database from the map grid").
   Function and global names are the originals from the FM Towns symbol table, which
   has this code in the same order from set_pix_xfer to grdb_elem (just before do_obj)
   and the same data from gftab to norm.

   DOS segment seg019_21BA starts with SetPnt (1FCD:000C), which is assembly: FM Towns
   has it among the assembly graphics routines (stosw, register arguments), and every
   call to it from grdb_elem here is a far call, which Turbo C only emits for a
   function outside the file. So it was a separate assembly module linked into this
   file's code segment, and this file starts at set_pix_xfer (1FCD:0041). */

#include "conv.h"
#include "event.h"
#include "map.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "view3d.h"

typedef void (far *FlrFn)(unsigned char *pts, unsigned char shade, unsigned char tex);
typedef void (far *WalFn)(unsigned char *pts, unsigned char shade, unsigned char height,
                          unsigned char tex);

extern unsigned TxmTerr[];
extern struct Gloc glocs[][33];
extern unsigned char PlayersMap[MAP_SIZE][MAP_SIZE];
extern signed char quad;

/* match: this file's _BSS, DS:2C68..2F95 (seg032's xwid ends at 2C67; seg033's ActDoors
   starts at 2F96), laid out by name (tools/bssorder.py): cWCol 27, loopx and loopy 44,
   UsPtr 53, cTmSz 59, p_gloc 104, qdec 153, AnimObjInPipe 257, TxmCol 332, color_to_map
   363, color_to_obj 371, PickUp 376, mlowptr 461, ptnuminq 480, pt_spare 512, mhighptr
   525, sqmod 555, flat_case 574, tmptr 596, pipeexp 752, cTmBm 947, cTmDm 963, gr_fcall,
   gr_ccall and gr_wcall 967, cTmHg 995, tCacheOK 1004.

   color_to_map and color_to_obj are the FM Towns names: they are the tile and object of
   each pickable thing in the view, by its colour in the pick buffer (do_obj sets them,
   pick_3d reads them), 172 entries each as in FM Towns, and every use, there as here, is
   at [colour - 1]: so they start at DS:2CBC and 2E14, after TxmCol's 64 bytes and a pad
   byte.

   name: FM Towns has p_gloc as a static (_gr_wcall+4), so its name is provisional and
   chosen for its key; pt_spare (DS:2F74, two bytes nothing refers to) likewise. */
int cWCol;                              /* DS:2C68 */
int loopx, loopy;                       /* DS:2C6A, 2C6C */
struct Object far *UsPtr;               /* DS:2C6E */
int cTmSz;                              /* DS:2C72 */
static struct Gloc far *p_gloc;         /* DS:2C74 */
unsigned char *qdec;                    /* DS:2C78 */
unsigned char AnimObjInPipe;            /* DS:2C7A */
unsigned char TxmCol[64];               /* DS:2C7B */
int color_to_map[172];                  /* DS:2CBC */
int color_to_obj[172];                  /* DS:2E14 */
unsigned char PickUp;                   /* DS:2F6C */
struct Tile far *mlowptr;               /* DS:2F6E */
int ptnuminq;                           /* DS:2F72 */
static char pt_spare[2];                /* DS:2F74 */
struct Tile far *mhighptr;              /* DS:2F76 */
unsigned char sqmod;                    /* DS:2F7A */
unsigned char flat_case;                /* DS:2F7B */
struct Tile far *tmptr;                 /* DS:2F7C */
int pipeexp;                            /* DS:2F80 */
int cTmBm, cTmDm;                       /* DS:2F82, 2F84 */
FlrFn gr_fcall, gr_ccall;               /* DS:2F86, 2F8A */
WalFn gr_wcall;                         /* DS:2F8E */
int cTmHg;                              /* DS:2F92 */
char tCacheOK;                          /* DS:2F94 */

/* Settings. distpoly is the distance shade from which txtflr and txtwal fall back to
   flat polygons; tmapson (texture mapping at all) and gftab/gctab[1] are set by
   set_graphics_level; lighton 0 turns distance shading off (process_grid does that on
   the levels it treats as unlit); ciels 0 leaves out the ceiling. */
int dist8 = 4;
int distpoly = 7;                       /* shades from here on are drawn flat */
int tmapson = 1;
int lighton = 1;
int ciels = 1;
unsigned char curautocode = 0;
unsigned char SpecShadeMode = 1;
/* Floor, ceiling and wall drawing, flat [0] or texture mapped [1]. */
FlrFn gftab[2] = { polyflr, txtflr };
FlrFn gctab[2] = { polycie, polyflr };
WalFn gwtab[2] = { polywal, txtwal };
/* The tile-index step of the pick frame's columns and rows in each quadrant (sd_xmod,
   sd_ymod, which GAMESORT.C uses to find the tile an object stands in). */
int quad_mod[4][2] = { { 1, MAP_SIZE }, { -MAP_SIZE, 1 }, { -1, -MAP_SIZE }, { MAP_SIZE, -1 } };
/* Which corner of a floor texture each of the four points gets, by quadrant (qdec),
   so the texture keeps its orientation on the map whichever way the camera faces. */
unsigned char qudecode[4][4] = {
    { 0, 1, 3, 2 }, { 2, 0, 1, 3 }, { 3, 2, 0, 1 }, { 1, 3, 2, 0 }
};
/* The height step (0 or 1) at each corner of a tile for the four slope directions,
   and none (row 4) for a flat tile. */
unsigned char hgtmodtab[5][4] = {
    { 0, 0, 1, 1 }, { 1, 1, 0, 0 }, { 0, 1, 0, 1 }, { 1, 0, 1, 0 }, { 0, 0, 0, 0 }
};
unsigned char pget1[3] = { 2, 0, 1 };
unsigned char pget2[3] = { 0, 1, 3 };
unsigned char thgt[6][5] = {
    { 0, 0, 1, 0, 0 }, { 1, 0, 0, 0, 0 }, { 0, 0, 0, 1, 0 },
    { 1, 1, 0, 1, 0 }, { 0, 1, 1, 1, 0 }, { 1, 1, 1, 0, 0 }
};
/* For each of the three walls grdb_elem can see (bits 0x20, 0x10, 0x08 of the grid
   flags), the x and y offsets of its two ends and which corners' heights they take. */
unsigned char wallmodtab[3][6] = {
    { 1, 1, 3, 1, 0, 1 }, { 0, 1, 2, 1, 1, 3 }, { 0, 0, 0, 0, 1, 2 }
};
/* For the four diagonal tile types: the diagonal face's end points and the normal
   used to test whether it faces the eye. */
signed char dxtab[4][6] = {
    { 0, 0, 1, 1, 1, -1 }, { 0, 1, 1, 0, -1, -1 }, { 1, 0, 0, 1, 1, 1 }, { 1, 1, 0, 0, -1, 1 }
};
/* The normals of the four slopes, for the floor's facing test. */
signed char norm[4][3] = { { 0, 4, -1 }, { 0, 4, 1 }, { -1, 4, 0 }, { 1, 4, 0 } };
/* The automap code for each tile type. Static: FM Towns has no name for it and keeps
   it as norm+0xC, so the name is ours. */
static unsigned char tile_mapcode[16] = {
    0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x0B, 0x0B,
    0x0B, 0x0B, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};
/* For each of the three walls, the neighbour in chgtable it faces. Static, norm+0x1C
   in FM Towns; the name is ours. */
static unsigned char wall_nbr[3] = { 0, 1, 2 };

/* Select pixel-transfer mode n of the renderer's table (cPixXferStuff[0] is the
   current one): process_grid uses 1, do_3d_pickup 2 (probably the pick frame's
   write-colours-only mode; the table is seg003's). */
void far set_pix_xfer(int n)
{
    *cPixXferStuff = cPixXferStuff[n];
}

/* The detail setting (player->detail, 0..3): 0 no texture mapping, 1 walls only,
   2 walls and floors, 3 walls, floors and ceilings. */
void far set_graphics_level(void)
{
    register int level;
    int cie, flr;

    level = player->detail;
    tmapson = 0;
    cie = 0;
    flr = 0;
    if (level > 0) {
        tmapson = 1;
        if (level > 1) {
            flr = 1;
            if (level > 2)
                cie = 1;
        }
    }
    gctab[1] = cie ? txtflr : polyflr;
    gftab[1] = flr ? txtflr : polyflr;
    lighton = 1;
}

/* Emit the database for a normal frame: the header (an object call at label 0xA0 and
   the model variables 9, 8 and 4), then every visible tile through subprocess. On
   levels 65 to 72 other than 68 ((PlayerLevel - 1) / 8 == 8) lighting and the ceiling
   are turned off for the frame and lighting restored afterwards (probably the world
   whose levels have no ceiling; which world that is has not been checked). Afterwards
   the newly mapped tiles (pipeexp) give experience, pipeexp * (PlayerLevel / 8 + 1) /
   10, while the character's level (player->level) is 1 to 15. */
void far process_grid(void)
{
    unsigned char oldlight;
    int exp;

    oldlight = lighton;
    AnimObjInPipe = 0;
    flat_case = cPlayer->pitch == 0 && cPlayer->bank == 0;
    PickUp = 0;
    gr_fcall = gftab[tmapson];
    gr_ccall = gctab[tmapson];
    gr_wcall = gwtab[tmapson];
    *dbptr++ = 0x38;
    Ref(0xA0, 1);
    *dbptr++ = 0;
    *dbptr++ = 0x2200;
    *dbptr++ = 0;
    *dbptr++ = 0;
    *dbptr++ = 0x400;
    *dbptr++ = 0;
    *dbptr++ = 0;
    *dbptr++ = 0x1100;
    *dbptr++ = 0;
    *dbptr++ = 0;
    *dbptr++ = 0x3300;
    *dbptr++ = 2;
    *dbptr++ = Clk(9);
    *dbptr++ = 0;
    *dbptr++ = 2;
    *dbptr++ = Clk(8);
    *dbptr++ = tmapson ? 0 : 1;
    if ((PlayerLevel - 1) / 8 != 8 || PlayerLevel == 0x44) {
        *dbptr++ = 2;
        *dbptr++ = Clk(4);
        *dbptr++ = SpecShadeMode;
        ciels = 1;
    } else {
        *dbptr++ = 2;
        *dbptr++ = Clk(4);
        *dbptr++ = 0;
        lighton = 0;
        ciels = 0;
    }
    *dbptr++ = 0xD0;
    *dbptr++ = flat_case;
    set_pix_xfer(1);
    pipeexp = 0;
    subprocess();
    if (player->level != 0 && player->level < 0x10) {
        exp = pipeexp * (PlayerLevel / 8 + 1) / 10;
        if (exp)
            player_get_exp(exp);
    }
    if ((PlayerLevel - 1) / 8 == 8 && PlayerLevel != 0x44)
        lighton = oldlight;
}

/* Emit the database for a pick frame (VIEW3D.C's do_3d_grab). PickUp makes the face
   and object routines draw in identifying colours instead of shades: floors 0xEC +
   texture, walls 0xAC + texture, ceilings 0xFC, objects 1..0xAB (DRAWOBJ.C), and
   UI/INTERACT.C's pick_3d reads the colour under the cursor back through color_to_obj
   and color_to_map. */
void far do_3d_pickup(void)
{
    PickUp = 1;
    flat_case = cPlayer->pitch == 0 && cPlayer->bank == 0;
    *dbptr++ = 0x38;
    Ref(0xA0, 1);
    *dbptr++ = 0;
    *dbptr++ = 0x2200;
    *dbptr++ = 0;
    *dbptr++ = 0;
    *dbptr++ = 0x400;
    *dbptr++ = 0;
    *dbptr++ = 0;
    *dbptr++ = 0x1100;
    *dbptr++ = 0;
    *dbptr++ = 0;
    *dbptr++ = 0x3300;
    *dbptr++ = 2;
    *dbptr++ = Clk(9);
    *dbptr++ = 1;
    *dbptr++ = 2;
    *dbptr++ = Clk(8);
    *dbptr++ = 1;
    *dbptr++ = 2;
    *dbptr++ = Clk(4);
    *dbptr++ = 0;
    *dbptr++ = 0xD0;
    *dbptr++ = flat_case;
    gr_fcall = gftab[0];
    gr_ccall = gctab[0];
    gr_wcall = gwtab[0];
    set_pix_xfer(2);
    sd_xmod = quad_mod[quad][0];
    sd_ymod = quad_mod[quad][1];
    subprocess();
}

/* Walk the vision grid from row mxY (the farthest) down to the eye's row, emitting
   each row's tiles from the left edge to the middle and from the right edge back to
   the middle, then the middle column, so that nearer faces come later in the database.
   tmptr follows each cell in the map (stepping by chgtable for the quadrant); cells
   whose map index falls outside the 64x64 map (i & 0xF000) are skipped. sort_setup
   tells GAMESORT.C which part of the row it is in. After the walk, the row of tiles
   just past the last one gets automap codes too. */
void far subprocess(void)
{
    struct Gloc *gloc;
    struct Tile far *row;
    int idx;
    unsigned char *am;
    int dy;
    register int dx;
    register int i;

    map_crit_pages();
    tCacheOK = 0xE0;
    qdec = qudecode[quad];
    dx = chgtable[quad][0];
    dy = chgtable[quad][1];
    gloc = glocs[mxY];
    row = mapptr;
    row = row + mxY * dy;
    row = row - (dx << 4);
    mlowptr = Map_GetAddr(0, 0);
    mhighptr = Map_GetAddr(0x3F, 0x3F);
    idx = row - mlowptr;
    if (idx > 0x2000)
        idx = idx - 0x4000;
    sort_setup(-10);
    for (loopy = mxY; loopy >= 0; loopy--) {
        sort_setup(2);
        for (loopx = 0, p_gloc = gloc + loopx, tmptr = row + loopx * dx,
             i = idx + loopx * dx; loopx < 0x10;
             loopx++, p_gloc++, tmptr += dx, i += dx)
            if (!(i & 0xF000))
                grdb_elem(PlayersMap[0] + i);
        sort_setup(1);
        for (loopx = 0x20, p_gloc = gloc + loopx, tmptr = row + loopx * dx,
             i = idx + loopx * dx; loopx > 0x10;
             loopx--, p_gloc--, tmptr -= dx, i -= dx)
            if (!(i & 0xF000))
                grdb_elem(PlayersMap[0] + i);
        sort_setup(0);
        if (!(i & 0xF000))
            grdb_elem(PlayersMap[0] + i);
        *dbptr++ = 0xB0;
        gloc -= 33;
        row -= dy;
        idx -= dy;
    }
    for (tmptr = row, loopx = 0, i = idx, am = PlayersMap[0] + idx; loopx < 0x21;
         loopx++, tmptr += dx, i += dx, am += dx)
        if (!(i & 0xF000) && *am == 0)
            *am = tile_mapcode[tmptr->type];
}

/* The flat face routines: emit a 4-point do_polyres in the texture's average colour
   (TxmCol) shaded through cLightTabs by the distance shade less 4, or in the pick
   colour. polyflr is used for floors and, from gctab, for ceilings drawn flat;
   polycie is the ceiling in pick frames; polywal walls. */
void far polyflr(unsigned char *pts, unsigned char shade, unsigned char tex)
{
    unsigned char col;

    if (PickUp)
        cWCol = tex + 0xEC;
    else {
        col = TxmCol[tex];
        if (shade < 4)
            shade = 0;
        else
            shade = shade - 4;
        cWCol = cLightTabs[(shade * lighton << 8) + col];
    }
    *dbptr++ = 0x2E;
    *dbptr++ = cWCol;
    *dbptr++ = 0x7E;
    *dbptr++ = 4;
    *dbptr++ = pts[0] << 3;
    *dbptr++ = pts[1] << 3;
    *dbptr++ = pts[2] << 3;
    *dbptr++ = pts[3] << 3;
}

void far polycie(unsigned char *pts, unsigned char shade, unsigned char tex)
{
    unsigned char col;

    if (PickUp)
        cWCol = 0xFC;
    else {
        col = TxmCol[tex];
        if (shade < 4)
            shade = 0;
        else
            shade = shade - 4;
        cWCol = cLightTabs[(shade * lighton << 8) + col];
    }
    *dbptr++ = 0x2E;
    *dbptr++ = cWCol;
    *dbptr++ = 0x7E;
    *dbptr++ = 4;
    *dbptr++ = pts[0] << 3;
    *dbptr++ = pts[1] << 3;
    *dbptr++ = pts[2] << 3;
    *dbptr++ = pts[3] << 3;
}

void far polywal(unsigned char *pts, unsigned char shade, unsigned char height, unsigned char tex)
{
    unsigned char col;

    if (PickUp)
        cWCol = tex + 0xAC;
    else {
        col = TxmCol[tex];
        if (shade < 4)
            shade = 0;
        else
            shade = shade - 4;
        cWCol = cLightTabs[(shade * lighton << 8) + col];
    }
    *dbptr++ = 0x2E;
    *dbptr++ = cWCol;
    *dbptr++ = 0x7E;
    *dbptr++ = 4;
    *dbptr++ = pts[0] << 3;
    *dbptr++ = pts[1] << 3;
    *dbptr++ = pts[2] << 3;
    *dbptr++ = pts[3] << 3;
}

/* The texture-mapped face routines. Beyond distpoly they fall back to the flat ones.
   They load texture tex into bitmap slot 5 (do_fetchmap), set the slot's height mask
   in the bitmap table (bmhgtoff), and, given points, emit the face. With pts 0 they
   only set up the texture, which DRAWOBJ.C uses for textured objects and doors.
   txtwal emits the fetch only once per tile (tCacheOK counts the walls since grdb_elem
   reset it) and scales the texture height to the wall's height. */
void far txtflr(unsigned char *pts, unsigned char shade, unsigned char tex)
{
    if (shade >= distpoly && pts) {
        polyflr(pts, shade, tex);
        return;
    }
    cTmBm = 5;
    cTmSz = 0x1000;
    cTmDm = 0x40;
    cTmHg = 0xFFF;
    *dbptr++ = 0x3E;
    *dbptr++ = cTmBm;
    *dbptr++ = tex;
    *dbptr++ = 2;
    *dbptr++ = bmhgtoff + (cTmBm << 3);
    *dbptr++ = cTmHg;
    if (pts) {
        *dbptr++ = 0x36;
        *dbptr++ = cTmBm;
        *dbptr++ = pts[0] << 3;
        *dbptr++ = qdec[3];
        *dbptr++ = pts[1] << 3;
        *dbptr++ = qdec[2];
        *dbptr++ = pts[2] << 3;
        *dbptr++ = qdec[1];
        *dbptr++ = pts[3] << 3;
        *dbptr++ = qdec[0];
    }
}

void far txtwal(unsigned char *pts, unsigned char shade, unsigned char height, unsigned char tex)
{
    if (shade >= distpoly && pts) {
        polywal(pts, shade, height, tex);
        return;
    }
    cTmBm = 5;
    cTmSz = 0x1000;
    cTmDm = 0x40;
    cTmHg = (height << 4 << 6) - 1;
    if (tCacheOK++ < 1) {
        *dbptr++ = 0x3E;
        *dbptr++ = cTmBm;
        *dbptr++ = tex;
    }
    *dbptr++ = 2;
    *dbptr++ = bmhgtoff + (cTmBm << 3);
    *dbptr++ = cTmHg;
    if (pts) {
        if (flat_case)
            *dbptr++ = 0xA2;
        else
            *dbptr++ = 0xA0;
        *dbptr++ = cTmBm;
        *dbptr++ = (pts[1] << 8) + (pts[0] & 0xFF);
        *dbptr++ = (pts[3] << 8) + (pts[2] & 0xFF);
    }
}

/* Emit one tile, the one at tmptr in grid cell p_gloc; automap is its byte in
   PlayersMap. Not visible: give it an automap code if it has none, count it in
   pipeexp, and flush any objects GAMESORT.C is holding for this column. Visible:
   the floor (if the eye is above it, or for a slope on its facing side), the
   ceiling (when the eye is below 0x3F4 and the level has ceilings), each of the
   three walls the grid marks seen (up to the neighbour's height, or to the ceiling
   when the neighbour is solid), the diagonal face, then the objects on the tile.
   Heights are tile heights, 0 to 15, with 16 the ceiling; the distance shade
   is the low nibble of the cell's shade. The automap code is the tile type with the
   floor's terrain class, plus curautocode (doors, textured objects) in the high
   nibble, and is written when player->automap is set. */
void far grdb_elem(unsigned char *automap)
{
    unsigned char ht;
    unsigned char pts[4];
    int flags;
    unsigned char hq;
    unsigned char w;
    unsigned char *hm;
    signed char *dxp;
    int bit;
    int bit2;
    unsigned char vis;
    union Link far *link;
    unsigned char code;
    unsigned char nh;
    unsigned char h2;
    unsigned char wi;
    unsigned char sh;
    unsigned char nt;
    register unsigned char *p;
    register unsigned char *wm;

    if (!((flags = p_gloc->flags) & 0x80)) {
        if (*automap == 0) {
            *automap = tile_mapcode[tmptr->type];
            pipeexp++;
        }
        clear_objsort();
        return;
    }
    ptnuminq = 0xC8;
    ht = tmptr->height;
    sqmod = p_gloc->shade & 0xF;
    if (sqmod < 8) {
        code = tmptr->type;
        code = code | TxmTerr[tmptr->floor] & TERR_CLASS;
    } else if ((code = *automap) == 0)
        code = tile_mapcode[tmptr->type];
    if ((flags & 0x44) == 4)
        hq = flags & 3;
    else
        hq = 4;
    hm = hgtmodtab[hq];
    p = pts;
    p += 4;
    if (hq == 4)
        vis = cPlayer->z > hgt_val[ht];
    else
        vis = (((loopx - 0x10) << 8) - cPlayer->x) * norm[hq][0]
            + ((loopy << 8) - cPlayer->y) * norm[hq][2]
            + (hgt_val[ht + *hm] - cPlayer->z) * norm[hq][1] < 0;
    tCacheOK = 0xE0;
    if (vis) {
        p -= 4;
        *p++ = SetPnt(loopx, loopy + 1, ht + hm[2]);
        *p++ = SetPnt(loopx + 1, loopy + 1, ht + hm[3]);
        *p++ = SetPnt(loopx + 1, loopy, ht + hm[1]);
        *p++ = SetPnt(loopx, loopy, ht + hm[0]);
        (*gr_fcall)(pts, sqmod, tmptr->floor);
    }
    if (cPlayer->z <= 0x3F4 && ciels) {
        p -= 4;
        *p++ = SetPnt(loopx, loopy, 0x10);
        *p++ = SetPnt(loopx + 1, loopy, 0x10);
        *p++ = SetPnt(loopx + 1, loopy + 1, 0x10);
        *p++ = SetPnt(loopx, loopy + 1, 0x10);
        (*gr_ccall)(pts, sqmod, 0xF);
    }
    bit = 0x40;
    w = 0;
    tCacheOK = 0;
    do {
        bit2 = bit;
        bit = bit >> 1;
        if (flags & bit) {
            wm = wallmodtab[w];
            if (p_gloc->shade & bit2) {
                nh = tmptr[chgtable[quad][wall_nbr[w]]].height;
                nt = trans_grid[quad][tmptr[chgtable[quad][wall_nbr[w]]].type];
                if ((tile_walls[nt] & 0x20) == 0x20)
                    wi = nt - 6;
                else
                    wi = 4;
                sh = nh + thgt[w + 3][wi] - ht - thgt[w][hq];
                h2 = nh + hgtmodtab[wi][pget2[w]];
                nh = nh + hgtmodtab[wi][pget1[w]];
            } else {
                h2 = nh = 0x10;
                sh = 0x10 - ht - thgt[w][hq];
            }
            p -= 4;
            *p++ = SetPnt(loopx + wm[0], loopy + wm[1], nh);
            *p++ = SetPnt(loopx + wm[3], loopy + wm[4], h2);
            *p++ = SetPnt(loopx + wm[3], loopy + wm[4], ht + hm[wm[5]]);
            *p++ = SetPnt(loopx + wm[0], loopy + wm[1], ht + hm[wm[2]]);
            (*gr_wcall)(pts, sqmod, sh, TILE_WALL(tmptr));
        }
    } while (++w < 3);
    if ((flags & 0x44) == 0x44) {
        dxp = dxtab[flags & 3];
        if ((((loopx + dxp[0] - 0x10) << 8) - cPlayer->x) * dxp[4]
            + (((loopy + dxp[1]) << 8) - cPlayer->y) * dxp[5] < 0) {
            p -= 4;
            *p++ = SetPnt(loopx + dxp[0], loopy + dxp[1], 0x10);
            *p++ = SetPnt(loopx + dxp[2], loopy + dxp[3], 0x10);
            *p++ = SetPnt(loopx + dxp[2], loopy + dxp[3], ht);
            *p++ = SetPnt(loopx + dxp[0], loopy + dxp[1], ht);
            (*gr_wcall)(pts, sqmod, 0x10 - ht, TILE_WALL(tmptr));
        }
    }
    link = &tmptr->objects;
    do_objsort(link);
    if (link && curautocode) {
        if (sqmod < 8)
            code = code | curautocode << 4;
        curautocode = 0;
    }
    if (player->automap)
        *automap = code;
}
