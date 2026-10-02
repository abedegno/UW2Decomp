/* view3d.h: The 3D view: building the render database, drawing objects into it, sorting,
   and the renderer's data that the C reads. VIEW3D.C sets up a frame and builds the
   vision grid, GRIDDB.C turns the grid into model interpreter bytecode (the render
   database), GAMESORT.C orders each tile's objects and DRAWOBJ.C emits them, and seg004
   (src/3d/*.ASM), entered through seg021's cRender, runs the bytecode.
   docs/subsystems/3d.md describes the whole. */
#ifndef VIEW3D_H
#define VIEW3D_H

#include "uw2.h"

struct Camera;
struct Grs3d;
struct Object;
struct Tile;

#include "map.h"
#include "object.h"

/* The 3D view's camera, a copy of the player's position that get_eye fills (cPlayer, a
   far pointer in seg021's data). VIEW3D.C's setup_vars rewrites it each frame: x and y
   keep only the position within the eye's tile, turned into the first quadrant, and
   heading loses the quadrant's turn. Heading is a full turn in 0x10000. Only the fields
   the C reads are named. */
struct Camera {
    char pad0[0x0A];
    int x;                              /* 0x0A, in 1/256 tiles */
    char pad0C[2];
    int z;                              /* 0x0E */
    char pad10[2];
    int y;                              /* 0x12 */
    char pad14[0x28 - 0x14];
    int pitch;                          /* 0x28 */
    int bank;                           /* 0x2A */
    int heading;                        /* 0x2C */
};

/* The art page each 3D object's pictures are in (grs_3dinf), 2 bytes. */
struct Grs3d {
    unsigned char page;
    char b1;
};

/* One cell of the vision grid (seg032's glocs), 33 cells to a row and 17 rows, the eye at
   row 0 column 16: which faces of the tile are seen and drawn. */
struct Gloc {
    unsigned char flags;                /* 0x80 visible, 0x44 slope, 0x20/0x10/0x08 walls */
    unsigned char shade;                /* bits 0-3 the distance shade; the high bits wall
                                           faces (seg019) */
};

/* GRIDDB.C: building the 3D view's render database from the map. The face routines take
   four point numbers from SetPnt, the distance shade and the texture. */
extern int loopx;
extern int loopy;
extern struct Object far *UsPtr;
extern unsigned char TxmCol[64];
extern int color_to_map[172];
/* The pick tables, indexed by the byte under the cursor (less 1).
   name: no FM Towns names. */
extern int color_to_obj[172];
extern struct Tile far *mlowptr;
extern unsigned char sqmod;
extern struct Tile far *tmptr;
extern int cTmBm;
extern int cTmDm;
extern char tCacheOK;
void far polyflr(unsigned char *pts, unsigned char shade, unsigned char tex);
void far polycie(unsigned char *pts, unsigned char shade, unsigned char tex);
void far polywal(unsigned char *pts, unsigned char shade, unsigned char height, unsigned char tex);
void far txtflr(unsigned char *pts, unsigned char shade, unsigned char tex);
void far txtwal(unsigned char *pts, unsigned char shade, unsigned char height, unsigned char tex);
void far subprocess(void);
void far grdb_elem(unsigned char *automap);
extern int dist8;
extern int distpoly;
extern int tmapson;
extern int lighton;
extern unsigned char curautocode;
extern unsigned char SpecShadeMode;  /* name: DOS only, no FM Towns name */
void far set_graphics_level(void);
void far process_grid(void);
void far do_3d_pickup(void);

/* VIEW3D.C: setting up the 3D view */
extern int chgtable[4][3];
extern unsigned headmod[4];
extern unsigned char trans_grid[4][16];
extern int xhgt;
extern int xwid;
extern char demo_mode;
extern struct Tile far *mapptr;
extern int mxY;
char far setup_vars(void);
void far preset_grid(int range);
void far do_2dclip(void);
void far place_3d_view(int x, int y, int w, int h);
void far set_cyb(char on);
void far init_3d(void);
void far do_3d_grab(void);
void far render_FB(void);
void far establish_view(void);

/* DRAWOBJ.C: drawing one object into the 3D view's render database */
extern unsigned char ActDoors[6];  /* the level's door textures */
void far do_rect(unsigned char model, struct Object far *o, char heading, int tex);
void far do_door(unsigned char item, struct Object far *o);
void far do_obj(struct Object far *o);

/* GAMESORT.C: sorting the 3D view's objects, tile by tile. objxloc, objyloc and objzloc
   are the view position of the object being drawn, which DRAWOBJ.C reads. */
extern unsigned char locsqmod;
extern int mptrmod;
extern int sd_xmod;  /* name: _sd_xmod and _sd_ymod in FM Towns */
extern int sd_ymod;  /* name: _sd_xmod and _sd_ymod in FM Towns */
extern int objxloc;
extern int objyloc;
extern int objzloc;
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
extern unsigned far EmsBuff;  /* 4FAF:E4D2 */
extern unsigned char far PgtoCmap[];  /* 4FAF:E1C9 */
extern unsigned far crit_fpage;
extern unsigned char far crit_inpage;
extern unsigned far crit_nlpages;
extern unsigned far first_anim;
extern struct Grs3d far grs_3dinf[];
extern unsigned char far obj_inpage1;  /* the EMS page mapped into frame page 2 */
/* The EMS page holding the screen graphics, and the page last mapped for objects, which
   is spoiled by any other mapping.
   name: FM Towns names, at 4FAF:E4D0 and E4D1. */
extern unsigned char far scrgr_fpage;
extern unsigned far seg052_519C_E4D4;  /* 4FAF:E4D4 */
extern unsigned char far tmap_fpage;
extern unsigned char far tmap_inpage;

/* PGCACHE.ASM */
extern unsigned far *grs_off;  /* EMS page and paragraph, or video address, per slot */
extern unsigned far *obj_tab;  /* two words per object; only the first is set by ovr119 */

/* Defined where no source has it yet: data the link takes from the EXE. bmsegoff and
   bmhgtoff are offsets of the renderer's bitmap table (8 bytes a slot); smooth_div and
   smooth_lowpass scale the distance shade (VIEW3D.C's preset_grid, GAMESORT.C). */
extern int far _dblen;
extern unsigned far bmhgtoff;
/* in the graphics data segment */
extern unsigned far bmsegoff;
extern int far smooth_div;
extern int far smooth_lowpass;

#endif
