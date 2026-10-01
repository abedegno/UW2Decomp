/* target: seg019_21BA */
/* opts: -mm -1 -G -O -Y -d */
/* Building the 3D view's render database from the map: process_grid and do_3d_pickup
   set up a frame, subprocess walks the visible tiles row by row, grdb_elem emits one
   tile's floor, ceiling and walls, and the poly and txt functions send one flat or
   texture-mapped face. Function and global names are the originals from the FM Towns
   symbol table, which has this code in the same order from set_pix_xfer to grdb_elem
   (just before do_obj) and the same data from gftab to norm.

   DOS segment seg019_21BA starts with SetPnt (1FCD:000C), which is assembly: FM Towns
   has it among the assembly graphics routines (stosw, register arguments), and every
   call to it from grdb_elem here is a far call, which Turbo C only emits for a
   function outside the file. So it was a separate assembly module linked into this
   file's code segment, and this file starts at set_pix_xfer (1FCD:0041). */

struct Player {
    char pad0[0x3D];
    unsigned char level;                /* 0x3D */
    char pad3E[0x62 - 0x3E];
    unsigned b62_0:4;                   /* 0x62 */
    unsigned automap:1;                 /* bit 4: mark tiles seen on the automap */
    unsigned b62_5:11;
    char pad64[0x302 - 0x64];
    unsigned b302:4;                    /* 0x302 */
    unsigned detail:4;
};

/* The camera, a copy of the player's position. */
struct Eye {
    char pad0[0x0A];
    int x;                              /* 0x0A, in 1/256 tiles */
    char pad1[0x0E - 0x0C];
    int z;                              /* 0x0E */
    char pad2[0x12 - 0x10];
    int y;                              /* 0x12 */
    char pad3[0x28 - 0x14];
    int pitch;                          /* 0x28 */
    int roll;                           /* 0x2A */
};

struct Tile {
    unsigned type:4;
    unsigned height:4;
    unsigned b1:2;
    unsigned floor:4;                   /* bits 10..13 */
    unsigned b1_6:2;
    unsigned wall:6;                    /* 0x02, wall texture */
    unsigned objects:10;                /* head of the tile's object list */
};

/* One square of the view's visibility grid: which faces of the tile to draw. */
struct GLoc {
    unsigned char flags;                /* 0x80 visible, 0x44 slope, 0x20/0x10/0x08 walls */
    unsigned char sq;                   /* low nibble the shade; high bits wall faces */
};

typedef void (far *FlrFn)(unsigned char *pts, unsigned char shade, unsigned char tex);
typedef void (far *WalFn)(unsigned char *pts, unsigned char shade, unsigned char height,
                          unsigned char tex);

extern struct Player near *player;
extern struct Eye far *cPlayer;
extern int far *dbptr;
extern int far *cPixXferStuff;
extern unsigned char far *cLightTabs;
extern unsigned char TxmCol[];
extern unsigned TxmTerr[];
extern unsigned char tile_walls[];
extern int hgt_val[];
extern int chgtable[4][3];
extern unsigned char trans_grid[4][16];
extern struct GLoc glocs[][33];
extern unsigned char PlayersMap[64][64];
extern struct Tile far *mapptr;
extern struct Tile far *mlowptr;
extern struct Tile far *mhighptr;
extern struct Tile far *tmptr;
extern struct GLoc far *dseg_67d6_2C74;   /* no FM Towns name (it is _gr_wcall+4 there) */
extern unsigned char *qdec;
extern int mxY;
extern signed char quad;
extern int loopx, LikelyDist;          /* LikelyDist is _loopy in FM Towns */
extern int PlayerLevel;
extern unsigned char PickUp;
extern unsigned char AnimObjInPipe;
extern unsigned char flat_case;
extern unsigned char sqmod;
extern char tCacheOK;
extern int pipeexp;
extern int ptnuminq;
extern int cWCol;
extern int cTmBm, cTmDm, cTmSz, cTmHg;
extern unsigned far bmhgtoff;
extern FlrFn gr_fcall, gr_ccall;
extern WalFn gr_wcall;
extern int trans_x, trans_y;           /* _sd_xmod and _sd_ymod in FM Towns */

int far SetPnt(char x, char y, char z);
void far Ref(int n, int a);
int far Clk(int n);
void far player_get_exp(int n);
void far map_crit_pages(void);
struct Tile far *far Map_GetAddr(int x, int y);
void far sort_setup(char mode);
void far clear_objsort(void);
void far do_objsort(void far *link);

void far polyflr(unsigned char *pts, unsigned char shade, unsigned char tex);
void far polycie(unsigned char *pts, unsigned char shade, unsigned char tex);
void far polywal(unsigned char *pts, unsigned char shade, unsigned char height, unsigned char tex);
void far txtflr(unsigned char *pts, unsigned char shade, unsigned char tex);
void far txtwal(unsigned char *pts, unsigned char shade, unsigned char height, unsigned char tex);
void far subprocess(void);
void far grdb_elem(unsigned char *automap);

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
int quad_mod[4][2] = { { 1, 0x40 }, { -0x40, 1 }, { -1, -0x40 }, { 0x40, -1 } };
unsigned char qudecode[4][4] = {
    { 0, 1, 3, 2 }, { 2, 0, 1, 3 }, { 3, 2, 0, 1 }, { 1, 3, 2, 0 }
};
unsigned char hgtmodtab[5][4] = {
    { 0, 0, 1, 1 }, { 1, 1, 0, 0 }, { 0, 1, 0, 1 }, { 1, 0, 1, 0 }, { 0, 0, 0, 0 }
};
unsigned char pget1[3] = { 2, 0, 1 };
unsigned char pget2[3] = { 0, 1, 3 };
unsigned char thgt[6][5] = {
    { 0, 0, 1, 0, 0 }, { 1, 0, 0, 0, 0 }, { 0, 0, 0, 1, 0 },
    { 1, 1, 0, 1, 0 }, { 0, 1, 1, 1, 0 }, { 1, 1, 1, 0, 0 }
};
unsigned char wallmodtab[3][6] = {
    { 1, 1, 3, 1, 0, 1 }, { 0, 1, 2, 1, 1, 3 }, { 0, 0, 0, 0, 1, 2 }
};
signed char dxtab[4][6] = {
    { 0, 0, 1, 1, 1, -1 }, { 0, 1, 1, 0, -1, -1 }, { 1, 0, 0, 1, 1, 1 }, { 1, 1, 0, 0, -1, 1 }
};
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

void far set_pix_xfer(int n)
{
    *cPixXferStuff = cPixXferStuff[n];
}

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

void far process_grid(void)
{
    unsigned char oldlight;
    int exp;

    oldlight = lighton;
    AnimObjInPipe = 0;
    flat_case = cPlayer->pitch == 0 && cPlayer->roll == 0;
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

void far do_3d_pickup(void)
{
    PickUp = 1;
    flat_case = cPlayer->pitch == 0 && cPlayer->roll == 0;
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
    trans_x = quad_mod[quad][0];
    trans_y = quad_mod[quad][1];
    subprocess();
}

void far subprocess(void)
{
    struct GLoc *gloc;
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
    for (LikelyDist = mxY; LikelyDist >= 0; LikelyDist--) {
        sort_setup(2);
        for (loopx = 0, dseg_67d6_2C74 = gloc + loopx, tmptr = row + loopx * dx,
             i = idx + loopx * dx; loopx < 0x10;
             loopx++, dseg_67d6_2C74++, tmptr += dx, i += dx)
            if (!(i & 0xF000))
                grdb_elem(PlayersMap[0] + i);
        sort_setup(1);
        for (loopx = 0x20, dseg_67d6_2C74 = gloc + loopx, tmptr = row + loopx * dx,
             i = idx + loopx * dx; loopx > 0x10;
             loopx--, dseg_67d6_2C74--, tmptr -= dx, i -= dx)
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
    void far *link;
    unsigned char code;
    unsigned char nh;
    unsigned char h2;
    unsigned char wi;
    unsigned char sh;
    unsigned char nt;
    register unsigned char *p;
    register unsigned char *wm;

    if (!((flags = dseg_67d6_2C74->flags) & 0x80)) {
        if (*automap == 0) {
            *automap = tile_mapcode[tmptr->type];
            pipeexp++;
        }
        clear_objsort();
        return;
    }
    ptnuminq = 0xC8;
    ht = tmptr->height;
    sqmod = dseg_67d6_2C74->sq & 0xF;
    if (sqmod < 8) {
        code = tmptr->type;
        code = code | TxmTerr[tmptr->floor] & 0xC0;
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
            + ((LikelyDist << 8) - cPlayer->y) * norm[hq][2]
            + (hgt_val[ht + *hm] - cPlayer->z) * norm[hq][1] < 0;
    tCacheOK = 0xE0;
    if (vis) {
        p -= 4;
        *p++ = SetPnt(loopx, LikelyDist + 1, ht + hm[2]);
        *p++ = SetPnt(loopx + 1, LikelyDist + 1, ht + hm[3]);
        *p++ = SetPnt(loopx + 1, LikelyDist, ht + hm[1]);
        *p++ = SetPnt(loopx, LikelyDist, ht + hm[0]);
        (*gr_fcall)(pts, sqmod, tmptr->floor);
    }
    if (cPlayer->z <= 0x3F4 && ciels) {
        p -= 4;
        *p++ = SetPnt(loopx, LikelyDist, 0x10);
        *p++ = SetPnt(loopx + 1, LikelyDist, 0x10);
        *p++ = SetPnt(loopx + 1, LikelyDist + 1, 0x10);
        *p++ = SetPnt(loopx, LikelyDist + 1, 0x10);
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
            if (dseg_67d6_2C74->sq & bit2) {
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
            *p++ = SetPnt(loopx + wm[0], LikelyDist + wm[1], nh);
            *p++ = SetPnt(loopx + wm[3], LikelyDist + wm[4], h2);
            *p++ = SetPnt(loopx + wm[3], LikelyDist + wm[4], ht + hm[wm[5]]);
            *p++ = SetPnt(loopx + wm[0], LikelyDist + wm[1], ht + hm[wm[2]]);
            (*gr_wcall)(pts, sqmod, sh, tmptr->wall);
        }
    } while (++w < 3);
    if ((flags & 0x44) == 0x44) {
        dxp = dxtab[flags & 3];
        if ((((loopx + dxp[0] - 0x10) << 8) - cPlayer->x) * dxp[4]
            + (((LikelyDist + dxp[1]) << 8) - cPlayer->y) * dxp[5] < 0) {
            p -= 4;
            *p++ = SetPnt(loopx + dxp[0], LikelyDist + dxp[1], 0x10);
            *p++ = SetPnt(loopx + dxp[2], LikelyDist + dxp[3], 0x10);
            *p++ = SetPnt(loopx + dxp[2], LikelyDist + dxp[3], ht);
            *p++ = SetPnt(loopx + dxp[0], LikelyDist + dxp[1], ht);
            (*gr_wcall)(pts, sqmod, 0x10 - ht, tmptr->wall);
        }
    }
    link = (char far *)tmptr + 2;
    do_objsort(link);
    if (link && curautocode) {
        if (sqmod < 8)
            code = code | curautocode << 4;
        curautocode = 0;
    }
    if (player->automap)
        *automap = code;
}
