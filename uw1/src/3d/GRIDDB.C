/* target: seg017_1FDD */
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
   the draw function pointers, the tables of tile shapes and the automap switch. It also
   marks the automap: every tile the walk passes is written into PlayersMap with its
   automap code, and tiles that come into view for the first time count towards a small
   experience award (pipeexp).

   Neighbours: VIEW3D.C (the vision grid, quadrant and camera), GAMESORT.C and
   DRAWOBJ.C (objects), SetPnt (points), seg004 (runs the bytecode).

   name: descriptive (map/filenames.tsv: "the render database from the map grid").
   UW1 has no symbol-bearing build: function and global names are UW2's (FM Towns),
   the routines being the same, in the same order.

   UW1's differences from UW2's seg019_21BA: no TxmCol; a flat face's colour is the
   first pixel of the texture itself (GRSPIC.C's seg009_38C gives its segment), and
   the texture-mapped faces pick a smaller copy of the texture by distance (dist8) and
   light (lighton) rather than one size; no ceiling switch (ciels): the unlit level is
   9 (PlayerLevel), which gets no experience for newly seen tiles; the detail setting
   and the automap switch are not where UW2 has them (Player1Grid, and the global
   ProbablyAutomapEnabled_dseg_5c99_546); the pick tables have 192 entries.

   DOS segment seg017_1FDD starts with SetPnt (1DF0:0001), the same assembly as UW2's
   SETPNT.ASM (map/kin.tsv: same), so this file starts at set_pix_xfer (1DF0:0036).
   targets/seg017_1FDD.tsv still holds SetPnt's row, from base 0x21101; this file
   matches the segment from 0x21136 (a scratch table without that row). */

#include <dos.h>
#include "conv.h"
#include "event.h"
#include "map.h"
#include "object.h"
#include "player.h"
#include "sys.h"
#include "ui.h"
#include "view3d.h"
#include "gfx.h"

typedef void (far *FlrFn)(unsigned char *pts, unsigned char shade, unsigned char tex);
typedef void (far *WalFn)(unsigned char *pts, unsigned char shade, unsigned char height,
                          unsigned char tex);

/* match: this file's _BSS, DS:2E0A..3147 (VIEW3D.C's ends at 2E09; ActDoors starts at
   3148), laid out by name (tools/bssorder.py): cWCol 27, loopx and loopy 44, UsPtr 53,
   cTmSz 59, p_gloc 104, qdec 153, AnimObjInPipe 257, color_to_map 363, color_to_obj
   371, PickUp 376, mlowptr 461, ptnuminq 480, pt_spare 512, mhighptr 525, sqmod 555,
   flat_case 574, tmptr 596, pipeexp 752, cTmBm 947, cTmDm 963, gr_fcall, gr_ccall and
   gr_wcall 967, cTmHg 995, tCacheOK 1004.

   color_to_map and color_to_obj are the tile and object of each pickable thing in the
   view, by its colour in the pick buffer (do_obj sets them, pick_3d reads them), and
   every use is at [colour - 1]. In UW1 they have 192 entries each (DS:2E1E and 2F9E,
   PickUp at 311E), and there is no TxmCol between AnimObjInPipe and them.

   name: FM Towns has p_gloc as a static (_gr_wcall+4), so its name is provisional and
   chosen for its key; pt_spare (DS:3126, two bytes nothing refers to) likewise. */
int16 cWCol;                            /* DS:2E0A */
int16 loopx, loopy;                     /* DS:2E0C, 2E0E */
struct Object far *UsPtr;               /* DS:2E10 */
int16 cTmSz;                            /* DS:2E14 */
static struct Gloc far *p_gloc;         /* DS:2E16 */
unsigned char *qdec;                    /* DS:2E1A */
unsigned char AnimObjInPipe;            /* DS:2E1C */
int16 color_to_map[192];                /* DS:2E1E */
int16 color_to_obj[192];                /* DS:2F9E */
unsigned char PickUp;                   /* DS:311E */
struct Tile far *mlowptr;               /* DS:3120 */
int16 ptnuminq;                         /* DS:3124 */
static char pt_spare[2];                /* DS:3126 */
struct Tile far *mhighptr;              /* DS:3128 */
unsigned char sqmod;                    /* DS:312C */
char flat_case;                         /* DS:312D, UW1: char (cbw) */
struct Tile far *tmptr;                 /* DS:312E */
int16 pipeexp;                          /* DS:3132 */
int16 cTmBm, cTmDm;                     /* DS:3134, 3136 */
FlrFn gr_fcall, gr_ccall;               /* DS:3138, 313C */
WalFn gr_wcall;                         /* DS:3140 */
int16 cTmHg;                            /* DS:3144 */
char tCacheOK;                          /* DS:3146 */

/* Settings. distpoly is the distance shade from which txtflr and txtwal fall back to
   flat polygons; tmapson (texture mapping at all) and gftab/gctab[1] are set by
   set_graphics_level; lighton 0 turns distance shading off (process_grid does that on
   the level it treats as unlit). */
char ProbablyAutomapEnabled_dseg_5c99_546 = 1; /* UW1: the automap switch (UW2: player->automap) */
int16 dist8 = 4;
int16 distpoly = 7;                     /* shades from here on are drawn flat */
int16 tmapson = 1;
int16 lighton = 1;
unsigned char curautocode = 0;
signed char SpecShadeMode = 1;          /* UW1: signed (cbw) */
/* Floor, ceiling and wall drawing, flat [0] or texture mapped [1]. */
FlrFn gftab[2] = { polyflr, txtflr };
FlrFn gctab[2] = { polycie, polyflr };
WalFn gwtab[2] = { polywal, txtwal };
/* The tile-index step of the pick frame's columns and rows in each quadrant (sd_xmod,
   sd_ymod, which GAMESORT.C uses to find the tile an object stands in). */
int16 quad_mod[4][2] = { { 1, MAP_SIZE }, { -MAP_SIZE, 1 }, { -1, -MAP_SIZE }, { MAP_SIZE, -1 } };
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

/* The detail setting (UW1: bits 4-7 of player byte 0xB5, 0..3): 0 no texture mapping, 1 walls only,
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
   level 9 (UW1's ethereal void) lighting is turned off for the frame and restored
   afterwards. Elsewhere the newly mapped tiles (pipeexp) give experience, pipeexp *
   PlayerLevel / 10, while the character's level (player->level) is 1 to 15. */
void far process_grid(void)
{
    char oldlight;                      /* UW1: char (cbw) */
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
    if (PlayerLevel != 9) {
        *dbptr++ = 2;
        *dbptr++ = Clk(4);
        *dbptr++ = SpecShadeMode;
    } else {
        *dbptr++ = 2;
        *dbptr++ = Clk(4);
        *dbptr++ = 0;
        lighton = 0;
    }
    *dbptr++ = 0xD0;
    *dbptr++ = flat_case;
    set_pix_xfer(1);
    pipeexp = 0;
    subprocess();
    if (player->level != 0 && player->level < 0x10 && PlayerLevel != 9) {
        exp = pipeexp * PlayerLevel / 10;
        if (exp)
            player_get_exp(exp);
    }
    if (PlayerLevel == 9)
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

/* The flat face routines: emit a 4-point do_polyres in the colour of the texture's
   first pixel (UW1: read from texture index tex + 0x6A for floors and ceilings,
   tex + 0x3A for walls) shaded through cLightTabs by the
   distance shade, or in the pick colour (floors 0xF0 + texture, walls 0xC0 +
   texture, ceilings 0xFA). polyflr is used for floors and, from gctab, for ceilings
   drawn flat; polycie is the ceiling in pick frames; polywal walls. */
void far polyflr(unsigned char *pts, unsigned char shade, unsigned char tex)
{
    unsigned char far *col;

    if (PickUp)
        cWCol = tex + 0xF0;
    else {
        col = MK_FP(seg009_38C(tex + 0x6A), 0);
        cWCol = cLightTabs[(shade * lighton << 8) + *col];
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
    unsigned char far *col;

    if (PickUp)
        cWCol = 0xFA;
    else {
        col = MK_FP(seg009_38C(tex + 0x6A), 0);
        cWCol = cLightTabs[(shade * lighton << 8) + *col];
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
    unsigned char far *col;

    if (PickUp)
        cWCol = tex + 0xC0;
    else {
        col = MK_FP(seg009_38C(tex + 0x3A), 0);
        cWCol = cLightTabs[(shade * lighton << 8) + *col];
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
   UW1: from dist8 on they use another texture index (as in the flat routines; cTmDm
   0x10) in bitmap slot 0 or lighton, nearer tex + 0x30 for floors (0x20) or tex for
   walls (0x40) in slot 2 or 4 (plus lighton unless the shade is 0 and dseg_5c99_12B6 is 0x64). They load
   the texture into that slot (do_fetchmap, with its size), set the slot's height mask
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
    if (shade >= dist8) {
        tex = tex + 0x6A;
        cTmBm = lighton;
        cTmSz = 0x100;
        cTmHg = 0xFF;
        cTmDm = 0x10;
    } else {
        cTmBm = 2;
        if (shade != 0 || shade == 0 && dseg_5c99_12B6 != 0x64)
            cTmBm += lighton;
        cTmSz = 0x400;
        cTmDm = 0x20;
        cTmHg = 0x3FF;
        tex = tex + 0x30;
    }
    *dbptr++ = 0x3E;
    *dbptr++ = cTmBm;
    *dbptr++ = tex;
    *dbptr++ = cTmSz;
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
    if (shade >= dist8) {
        cTmBm = 0;
        if (shade)
            cTmBm += lighton;
        cTmSz = 0x100;
        cTmDm = 0x10;
        cTmHg = (height << 2 << 4) - 1;
        tex = tex + 0x3A;
    } else {
        cTmBm = 4;
        if (shade != 0 || shade == 0 && dseg_5c99_12B6 != 0x64)
            cTmBm += lighton;
        cTmSz = 0x1000;
        cTmDm = 0x40;
        cTmHg = (height << 4 << 6) - 1;
    }
    if (tCacheOK++ < 1) {
        *dbptr++ = 0x3E;
        *dbptr++ = cTmBm;
        *dbptr++ = tex;
        *dbptr++ = cTmSz;
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
   ceiling (texture 9, when the eye is below 0x3F4), each of the
   three walls the grid marks seen (up to the neighbour's height, or to the ceiling
   when the neighbour is solid), the diagonal face, then the objects on the tile.
   Heights are tile heights, 0 to 15, with 16 the ceiling; the distance shade
   is the low nibble of the cell's shade. The automap code is the tile type with the
   floor's terrain bits (UW1: TxmTerr's low byte as it is), plus curautocode (doors,
   textured objects) in the top two bits, and is written when the automap switch is
   on. */
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
        code = code | TxmTerr[tmptr->floor];
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
    if (cPlayer->z <= 0x3F4) {
        p -= 4;
        *p++ = SetPnt(loopx, loopy, 0x10);
        *p++ = SetPnt(loopx + 1, loopy, 0x10);
        *p++ = SetPnt(loopx + 1, loopy + 1, 0x10);
        *p++ = SetPnt(loopx, loopy + 1, 0x10);
        (*gr_ccall)(pts, sqmod, 9);
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
                if ((tile_walls[nt] & TW_SLOPE) == TW_SLOPE)
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
            code = code | curautocode << 6;
        curautocode = 0;
    }
    if (ProbablyAutomapEnabled_dseg_5c99_546)
        *automap = code;
}
