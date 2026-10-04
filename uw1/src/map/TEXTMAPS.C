/* target: ovr131 */
/* opts: -mm -1 -G -O -Y -d */
/* A level's texture map: the default one, loading and saving the wall and floor texture
   numbers and the door textures in LEV.ARK, loading the textures, and reading each
   texture's terrain type from DATA\TERRAIN.DAT. The whole of UW1's DOS overlay ovr131, in
   original order. Seeded from UW2Decomp's src/map/TEXTMAPS.C (UW2's ovr140).

   UW1 against UW2: walls and floors are separate. A level has 48 wall textures (TxmID, from
   W64.TR and W16.TR) and 10 floor textures (floor_IDs, from F32.TR and F16.TR), each with
   its terrain word from TERRAIN.DAT (walls from offset 0, floors from 0x200: w64_types and
   TxmTerr). The level's block in LEV.ARK is block 0x11 + level, 0x7A bytes: 48 wall words,
   10 floor words, then three words holding the six door textures a byte each (ActDoors).
   Textures 0 to 11 of the walls (and their 16-pixel versions) and all the floors are read
   into conventional memory (load_tr_mem: w64_buf, w16_buf, f32_buf, f16p, from TMPALLOC.C's
   workspace dseg_5c99_7178); textures 12 to 47 are LOADGR.C's in EMS. The 3D view finds each
   texture through the far tables seg051_C378..C460 (its EMS page, its other page, and the
   segments of the 64- and 16-pixel bitmaps), which init_txtlib fills. Txm_Load and
   Txm_Save take an open archive (ovr091), as GAMEWRAP.C passes it.

   Names: UW2's (FM Towns) where the routine is the same and the stub order agrees
   (init_txtlib, Txm_Load, load_txtmaps, Load_Terrains, Txm_Save). load_tr_mem and the data
   names are ours, chosen so that their bssorder keys give the EXE's stub order and _BSS
   layout (see each). Name: inferred, as UW2's. */

#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dos.h>
/* UW1: the texture map's tables and loaders differ from UW2's (map.h); those declarations
   are renamed out of the way. */
#define TxmID UW2_TxmID
#define TxmTerr UW2_TxmTerr
#define Load_Terrains UW2_Load_Terrains
#define Txm_Load UW2_Txm_Load
#define Txm_Save UW2_Txm_Save
#include "file.h"
#include "gfx.h"
#include "map.h"
#include "sys.h"
#include "view3d.h"
#undef TxmID
#undef TxmTerr
#undef Load_Terrains
#undef Txm_Load
#undef Txm_Save

/* UW1's declarations. */
unsigned far seg009_38C(int n);         /* a picture's segment */
void far dprintf(char *fmt, ...);
int far get_arc(char far *arc, int block, void far *buf);           /* reads a block */
char far put_arc(char far *arc, int block, void far *buf, int size); /* writes one */
extern uint16 dseg_5c99_7178;           /* TMPALLOC.C's workspace paragraph */
/* The 3D view's texture tables (seg051, far): for textures 0..57 (48 walls, then the 10
   floors), the EMS page of the 64-pixel bitmap (0 in conventional memory), the page of the
   16-pixel one, and the two bitmaps' segments. */
extern unsigned char far seg051_C375;   /* the textures' first EMS page */
extern unsigned char far seg051_C376;
extern unsigned char far seg051_C378[];
extern unsigned char far seg051_C3B2[];
extern uint16 far seg051_C3EC[];
extern uint16 far seg051_C460[];
void far Load_Terrains(int16 *walls, int16 *floors);
void far load_tr_mem(char *name, int16 *ids, int16 *count, uint16 seg);

/* This file's _BSS, DS:717A..726D.
   name: TxmID and TxmTerr are UW2's (TxmTerr here the floors' terrain words, as GRIDDB.C
   and COLLIDE.C index it by a tile's floor); the others are ours, chosen for layout
   (bssorder.py keys 22, 198, 206, 420, 590, 735, 988, 991, 991, 991). */
uint16 f16p;                            /* DS:717A, the floors' 16-pixel bitmaps */
int16 floor_IDs[10];                    /* DS:717C, the level's floor textures */
uint16 f32_buf;                         /* DS:7190, the floors' 32-pixel bitmaps */
uint16 TxmTerr[10];                     /* DS:7192, each floor's terrain type */
int16 floor_num;                        /* DS:71A6, floor textures loaded */
uint16 w16_buf;                         /* DS:71A8, the walls' 16-pixel bitmaps */
int16 TxmID[0x30];                      /* DS:71AA, the level's wall textures */
uint16 w64_buf;                         /* DS:720A, the walls' 64-pixel bitmaps */
uint16 w64_types[0x30];                 /* DS:720C, each wall's terrain type */
int16 w64_num;                          /* DS:726C, wall textures */

/* 0x0: the default texture map: walls 0..11 repeated, floors 0..9, doors 0..5; loads them
   and fills the 3D view's tables. */
char far init_txtlib(void)
{
    int i;

    for (i = 0; i < 0x30; i++) {
        TxmID[i] = i % 12;
        w64_types[i] = 0;
    }
    for (i = 0; i < 10; i++) {
        floor_IDs[i] = i;
        TxmTerr[i] = 0;
    }
    for (i = 0; i < 6; i++)
        ActDoors[i] = i;
    w64_num = 0x30;
    floor_num = 10;
    load_txtmaps();
    for (i = 0; i < 0x30; i++) {
        if (i < 12) {
            seg051_C378[i] = 0;
            seg051_C3B2[i] = 0;
            seg051_C3EC[i] = w64_buf + (i % 12 << 8);
            seg051_C460[i] = w16_buf + (i % 12 << 4);
        } else {
            seg051_C378[i] = (i >> 2) + seg051_C375 + 0xFD;
            seg051_C3B2[i] = seg051_C376;
            seg051_C3EC[i] = EmsBuff + ((i & 3) << 8) + 0xC00;
            seg051_C460[i] = EmsBuff + ((i - 12) << 4) + 0xC00;
        }
    }
    for (; i < 0x3A; i++) {
        seg051_C378[i] = 0;
        seg051_C3B2[i] = 0;
        seg051_C3EC[i] = f32_buf + ((i - 0x30) << 6);
        seg051_C460[i] = f16p + ((i - 0x30) << 4);
    }
    return 1;
}

/* 0x1DD: draws picture n + 0x3A, 16 by 16, at x, y. UW2's ovr140_4F does the same with a
   scaled-down texture. */
void far ovr131_1DD(int n, int x, int y)
{
    register unsigned seg;

    seg = seg009_38C(n + 0x3A);
    show(x, y, MK_FP(seg, 0), 0x10, 0x10, 0, 0);
}

/* 0x20D: reads level lev's texture block from the open archive arc into TxmID, floor_IDs
   and ActDoors, reads the terrain types, and loads the textures. A block that is not 0x7A
   bytes long makes the result 0, but the buffer is still used. */
unsigned char far Txm_Load(char *arc, int lev)
{
    unsigned char ok;
    int16 buf[0x40];
    register int i;

    ok = 1;
    if (get_arc(arc, lev + 0x11, buf) != 0x7A) {
        dprintf("bad tmap ids size\n");
        ok = 0;
    }
    for (i = 0; i < 0x30; i++) {
        TxmID[i] = buf[i];
        w64_types[i] = 0;
    }
    for (i = 0; i < 10; i++) {
        TxmTerr[i] = 0;
        floor_IDs[i] = buf[i + 0x30];
    }
    Load_Terrains(TxmID, floor_IDs);
    for (i = 0; i < 3; i++) {
        ActDoors[i * 2] = buf[i + 0x3A] & 0xFF;
        ActDoors[i * 2 + 1] = buf[i + 0x3A] >> 8;
    }
    load_txtmaps();
    return ok;
}

/* 0x2E2: writes TxmID, floor_IDs and ActDoors back as level lev's texture block. */
char far Txm_Save(char *arc, int lev)
{
    int16 buf[0x40];
    register int i;

    for (i = 0; i < 0x30; i++)
        buf[i] = TxmID[i];
    for (i = 0; i < 10; i++)
        buf[i + 0x30] = floor_IDs[i];
    for (i = 0; i < 3; i++)
        buf[i + 0x3A] = (ActDoors[i * 2 + 1] << 8) | (ActDoors[i * 2] & 0xFF);
    return put_arc(arc, lev + 0x11, buf, 0x7A);
}

/* 0x375: loads the level's textures: the first 12 walls and the floors into conventional
   memory, one after another from the workspace, then the walls into EMS (LOADGR.C) and
   the doors. At most 60 textures fit, so floor_num may be cut. */
void far load_txtmaps(void)
{
    int n;
    char name[0x42];
    register char *p;

    w64_buf = dseg_5c99_7178;
    strcpy(name, "DATA\\");
    p = name + strlen(name);
    strcpy(p, "w64.tr");
    n = 12;
    load_tr_mem(name, TxmID, &n, w64_buf);
    strcpy(p, "f32.tr");
    f32_buf = w64_buf + (n << 8);
    if (floor_num + n > 0x3C) floor_num = 0x3C - w64_num;
    load_tr_mem(name, floor_IDs, &floor_num, f32_buf);
    strcpy(p, "w16.tr");
    w16_buf = f32_buf + ((floor_num << 10) >> 4);
    load_tr_mem(name, TxmID, &n, w16_buf);
    strcpy(p, "f16.tr");
    f16p = w16_buf + (((n << 4) << 4) >> 4);
    load_tr_mem(name, floor_IDs, &floor_num, f16p);
    load_tr_ems("w64");
    load_tr_ems("w16");
    load_doors();
}

/* 0x49C: reads textures ids[0..*count-1] of the .tr file name (UW-Formats 3.2: a byte 2, the
   size, a word count, then count long offsets) into conventional memory from seg:0, one
   after another; stops at a negative id and leaves in *count the number read. */
void far load_tr_mem(char *name, int16 *ids, int16 *count, uint16 seg)
{
    int size;
    int32 *offs;
    unsigned char far *dst;
    int16 cnt;
    unsigned char b;
    register int i, fd;

    offs = 0;
    dst = MK_FP(seg, 0);
    if ((fd = open(name, 0x8001)) < 0) pfatal_code(ERR_READ | 0xF);
    read(fd, &b, 1);
    if (b != 2) pfatal_code(ERR_READ | 0x10);
    read(fd, &b, 1);
    size = b * b;
    read(fd, &cnt, 2);
    if ((offs = calloc(4, cnt)) == 0) {
        pfatal_code(ERR_LOWMEM | 8);
        goto done;
    }
    if (read(fd, offs, cnt << 2) < 0) {
        pfatal_code(ERR_READ | 0x11);
        goto done;
    }
    for (i = 0; *count > i && ids[i] >= 0; i++) {
        lseek(fd, offs[ids[i]], 0);
        if (intoFarBuffer_ovr167_5DA(fd, dst, size) != size) pfatal_code(ERR_READ | 0x12);
        dst += size;
    }
done:
    *count = i;
    free(offs);
    close(fd);
}

/* 0x5E1: reads each wall's and floor's terrain word from DATA\TERRAIN.DAT (walls from 0,
   floors from 0x200). */
void far Load_Terrains(int16 *walls, int16 *floors)
{
    register int i;
    register FILE *fp;

    fp = fopen("DATA\\terrain.dat", "rb");
    if (fp != NULL) {
        for (i = 0; i < 0x30; i++) {
            fseek(fp, (int32)walls[i] * 2, SEEK_SET);
            fread(&w64_types[i], 2, 1, fp);
        }
        for (i = 0; i < 10; i++) {
            fseek(fp, (int32)floors[i] * 2 + 0x200, SEEK_SET);
            fread(&TxmTerr[i], 2, 1, fp);
        }
        fclose(fp);
    }
}
