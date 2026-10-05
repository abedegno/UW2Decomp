/* view3d.h: The 3D view: building the render database, drawing objects into it, sorting,
   and the renderer's data that the C reads. VIEW3D.C sets up a frame and builds the
   vision grid, GRIDDB.C turns the grid into model interpreter bytecode (the render
   database), GAMESORT.C orders each tile's objects and DRAWOBJ.C emits them, and seg004
   (src/3d/*.ASM), entered through seg019's cRender (C3DENTRY.ASM), runs the bytecode.
   docs/subsystems/3d.md describes the whole. */
#ifndef VIEW3D_H
#define VIEW3D_H

#include "uw2.h"

struct Camera;
struct Grs3d;
struct Object;
struct Tile;
union Link;

#include "map.h"
#include "object.h"

/* The 3D view's camera, a copy of the player's position that get_eye fills (cPlayer, a
   far pointer in seg019's data). VIEW3D.C's setup_vars rewrites it each frame: x and y
   keep only the position within the eye's tile, turned into the first quadrant, and
   heading loses the quadrant's turn. Heading is a full turn in 0x10000. Only the fields
   the C reads are named. */
struct Camera {
    char pad0[0x0A];
    int16 x;                            /* 0x0A, in 1/256 tiles */
    char pad0C[2];
    int16 z;                            /* 0x0E */
    char pad10[2];
    int16 y;                            /* 0x12 */
    char pad14[0x28 - 0x14];
    int16 pitch;                        /* 0x28 */
    int16 bank;                         /* 0x2A */
    int16 heading;                      /* 0x2C */
};

/* The art page each 3D object's pictures are in (grs_3dinf), 2 bytes. */
struct Grs3d {
    unsigned char page;
    char b1;
};

/* One cell of the vision grid (VIEW3D.C's glocs), 33 cells to a row and 17 rows, the eye at
   row 0 column 16: which faces of the tile are seen and drawn. */
struct Gloc {
    unsigned char flags;                /* GLOC_*: seen, shape, the faces to draw */
    unsigned char shade;                /* bits 0-3 the distance shade (15: out of range);
                                           GLOC_STEP_*: a face only as high as the
                                           neighbour's floor */
};
/* A cell's flags, as VIEW3D.C's enc_dat gives them and enc_n_chk refines them, and as
   GRIDDB.C's grdb_elem reads them. Everything is in the turned grid (setup_vars): rows
   run away from the eye, columns to the right. The names are ours. */
#define GLOC_SEEN       0x80            /* in the vision arc: draw the cell */
#define GLOC_DIAG       0x40            /* with GLOC_SHAPED, a diagonal tile */
#define GLOC_RIGHT      0x20            /* the face towards the next column */
#define GLOC_FAR        0x10            /* the face towards the next row */
#define GLOC_LEFT       0x08            /* the face towards the previous column */
#define GLOC_SHAPED     0x04            /* a slope, or with GLOC_DIAG a diagonal; the low
                                           two bits say which way */
/* In the shade byte: the face of the same side reaches only the neighbour's floor (the
   neighbour is open and higher); without it the face reaches the ceiling. */
#define GLOC_STEP_RIGHT 0x40
#define GLOC_STEP_FAR   0x20
#define GLOC_STEP_LEFT  0x10

/* Render database opcodes: the words GRIDDB.C, DRAWOBJ.C and GAMESORT.C write for the
   model interpreter (INTERP.ASM). An opcode is the byte offset of its handler in the
   opcode table at seg051:2738h, so the numbers are the table's, and the names its
   handlers' (FM Towns names, through UW2). Only the opcodes the C writes are here. The
   operands follow inline: a point number is premultiplied by 8; Clk(n) is the address
   of model variable n. */
#define OP_MOVEC        0x02            /* do_movec: address, value */
#define OP_SFCAL        0x12            /* do_sfcal: call a label */
#define OP_ORG          0x18            /* do_org: an origin, three 32-bit words */
#define OP_SETCOLOR     0x2E            /* do_setcolor: colour */
#define OP_TMAP         0x36            /* do_tmap: slot, four points and corners */
#define OP_OBJ          0x38            /* do_obj: the view header (SPHERE.ASM) */
#define OP_UWOBJ        0x3A            /* do_uwobj: an object's picture */
#define OP_FETCHMAP     0x3E            /* do_fetchmap: slot, texture, size */
#define OP_DEFDELTA     0x4C            /* do_defdelta: x, y, z, scale */
#define OP_IHCALL       0x50            /* do_ihcall: heading, label */
#define OP_UWCRIT       0x5A            /* do_uwcrit: a critter's frame */
#define OP_DEFRES       0x7A            /* do_defres: a point x, z, y and its number */
#define OP_POLYRES      0x7E            /* do_polyres: count, then the points */
#define OP_COMPACT_TMAP 0xA0            /* do_compact_tmap: a textured wall */
#define OP_COMPACT_WTMAP 0xA2           /* do_compact_wtmap: the same, level view */
#define OP_SETBMCOL     0xAE            /* do_setbmcol: the pick colour */
#define OP_MOUSEQ       0xB0            /* do_mouseq: service the mouse queue */
#define OP_SET_TMCTXT   0xB2            /* do_set_tmctxt: texture slot */
#define OP_SETTMOBJ     0xC0            /* do_settmobj: texture, shade */
#define OP_SET_GMAP_CTXT 0xD0           /* do_set_gmap_ctxt: flat_case */

/* Pick colours: in a pick frame (PickUp set, GRIDDB.C's do_3d_pickup) every face and
   object is drawn in a colour that names it, which INTERACT.C's pick_3d reads under the
   pointer. Objects take 1 up to PICK_WALL - 1, wrapping round; walls PICK_WALL + texture,
   floors PICK_FLOOR + texture, ceilings PICK_CEILING. The names are ours. */
#define PICK_WALL       0xC0
#define PICK_FLOOR      0xF0
#define PICK_CEILING    0xFA

/* GRIDDB.C: building the 3D view's render database from the map. The face routines take
   four point numbers from SetPnt, the distance shade and the texture. */
extern int16 loopx;
extern int16 loopy;
extern struct Object far *UsPtr;
extern int16 color_to_map[192];
/* The pick tables, indexed by the byte under the cursor (less 1).
   name: no FM Towns names. */
extern int16 color_to_obj[192];
extern struct Tile far *mlowptr;
extern unsigned char sqmod;
extern struct Tile far *tmptr;
extern int16 cTmBm;
extern int16 cTmDm;
extern char tCacheOK;
void far polyflr(unsigned char *pts, unsigned char shade, unsigned char tex);
void far polycie(unsigned char *pts, unsigned char shade, unsigned char tex);
void far polywal(unsigned char *pts, unsigned char shade, unsigned char height, unsigned char tex);
void far txtflr(unsigned char *pts, unsigned char shade, unsigned char tex);
void far txtwal(unsigned char *pts, unsigned char shade, unsigned char height, unsigned char tex);
void far subprocess(void);
void far grdb_elem(unsigned char *automap);
extern int16 dist8;
extern int16 distpoly;
extern int16 tmapson;
extern int16 lighton;
extern unsigned char curautocode;
extern signed char SpecShadeMode;  /* name: DOS only, no FM Towns name */
void far set_graphics_level(void);
void far process_grid(void);
void far do_3d_pickup(void);
extern unsigned char AnimObjInPipe;
extern unsigned char PickUp;
extern char ProbablyAutomapEnabled_dseg_5c99_546;

/* VIEW3D.C: setting up the 3D view */
extern int16 chgtable[4][3];
extern uint16 headmod[4];
extern unsigned char trans_grid[4][16];
extern int16 xhgt;
extern int16 xwid;
extern char demo_mode;
extern struct Tile far *mapptr;
extern int16 mxY;
char far setup_vars(void);
void far preset_grid(int range);
void far do_2dclip(void);
void far place_3d_view(int x, int y, int w, int h);
void far set_cyb(char on);
void far init_3d(void);
void far do_3d_grab(void);
void far render_FB(void);
void far establish_view(void);
extern char quad;
extern struct Gloc glocs[17][33];

/* DRAWOBJ.C: drawing one object into the 3D view's render database */
extern unsigned char ActDoors[6];  /* the level's door textures */
void far do_rect(unsigned char model, struct Object far *o, char heading, int tex);
void far do_door(unsigned char item, struct Object far *o);
void far do_obj(struct Object far *o);

/* GAMESORT.C: sorting the 3D view's objects, tile by tile. objxloc, objyloc and objzloc
   are the view position of the object being drawn, which DRAWOBJ.C reads. */
extern unsigned char locsqmod;
extern int16 mptrmod;
extern int16 sd_xmod;  /* name: _sd_xmod and _sd_ymod in FM Towns */
extern int16 sd_ymod;  /* name: _sd_xmod and _sd_ymod in FM Towns */
extern int16 objxloc;
extern int16 objyloc;
extern int16 objzloc;
void far sort_setup(char mode);
void far do_objsort(union Link far *link);
void far clear_objsort(void);

/* TMAPOPS.ASM */
extern unsigned char far CmapCache[];  /* 4FAF:E3C9 */
extern unsigned char far CmapFrm[];  /* 4FAF:E2C9 */
extern unsigned char far CmaptoPg[];  /* 4FAF:E0C9 */
/* The segment of the EMS page frame (DRAWOBJ.C reads critter frame tables at
   EmsBuff + 0xC00).
   name: provisional. */
extern uint16 far EmsBuff;    /* 4FAF:E4D2 */
extern unsigned char far PgtoCmap[];  /* 4FAF:E1C9 */
extern uint16 far crit_fpage;
extern unsigned char far crit_inpage;
extern uint16 far crit_nlpages;
extern uint16 far first_anim;
extern unsigned char far obj_inpage1;  /* the EMS page mapped into frame page 2 */
/* The EMS page holding the screen graphics, and the page last mapped for objects, which
   is spoiled by any other mapping.
   name: FM Towns names, at 4FAF:E4D0 and E4D1. */
extern unsigned char far tmap_inpage;
extern unsigned char far seg051_C10F[];
extern unsigned char far seg051_C375;
extern unsigned char far seg051_C376;
extern unsigned char far seg051_C377;
extern unsigned char far seg051_C378[];
extern unsigned char far seg051_C3B2[];
extern uint16 far seg051_C3EC[];
extern uint16 far seg051_C460[];
extern uint16 far seg051_C4D7;

/* PGCACHE.ASM */
extern uint16 far *grs_off;    /* EMS page and paragraph, or video address, per slot */
extern uint16 far *obj_tab;    /* two words per object; only the first is set by LOADGR.C */

/* Defined where no source has it yet: data the link takes from the EXE. bmsegoff and
   bmhgtoff are offsets of the renderer's bitmap table (8 bytes a slot); smooth_div and
   smooth_lowpass scale the distance shade (VIEW3D.C's preset_grid, GAMESORT.C) and
   smooth_base is added to it; LIGHTING.C's set_light loads all three from SHADES.DAT. */
extern int16 far _dblen;
extern uint16 far bmhgtoff;
/* in the graphics data segment */
extern uint16 far bmsegoff;
extern int16 far smooth_base;
extern int16 far smooth_div;
extern int16 far smooth_lowpass;
extern unsigned char dseg_5c99_12B6;

/* INSTANCE.ASM */
extern unsigned char far seg051_28A0;

#endif
