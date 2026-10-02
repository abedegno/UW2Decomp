/* target: ovr140 */
/* opts: -mm -1 -G -O -Y -d */
/* A level's texture map: setting up the default one, loading and saving the 64 wall and
   floor texture numbers and the door textures in LEV.ARK, loading the textures, and
   reading each texture's terrain type from DATA\TERRAIN.DAT: the whole of DOS overlay
   ovr140, in original order. Function and global names are the originals from the FM
   Towns symbol table where it has them; the source file's own name is not known. */

#include <stdio.h>
#include <dos.h>
#include "file.h"
#include "gfx.h"
#include "map.h"
#include "sys.h"
#include "view3d.h"

/* This file's _BSS, DS:8188..8287, by name: TxmTerr 420, TxmID 988. They follow ovr139's
   answer_x (857) in a new run; this file loads them, with ActDoors, from the level. */
int TxmID[0x40];                        /* the level's texture numbers */
int TxmTerr[0x40];                      /* each texture's terrain type */

unsigned far get_arc(int arc, int blk, char far *buf);
unsigned char far put_arc(int arc, int blk, char far *buf, unsigned len);
void far close_arc(int arc);
void far show(int x, int y, unsigned char far *buf, int h, int w, int a, int b);

char far init_txtlib(void)
{
    int i;

    for (i = 0; i < 0x40; i++) {
        TxmID[i] = i;
        TxmTerr[i] = 0;
    }
    for (i = 0; i < 6; i++)
        ActDoors[i] = i;
    load_txtmaps();
    return 1;
}

/* DOS only, FM Towns has nothing between init_txtlib_ and Txm_Load_. It draws a 16 by 16
   picture at x, y. */
void far ovr140_4F(int n, int x, int y)
{
    unsigned char far *p;

    p = MK_FP(seg009_392(n), 0);
    p = grs_scaledown(p, 0x40, 0x40, 4);
    show(x, y, p, 0x10, 0x10, 0, 0);
}

/* arc is not read; GAMEWRAP.C passes 0. */
unsigned char far Txm_Load(int arc, int lev, int flags)
{
    unsigned char ok;
    int buf[0x46];
    register int i;

    ok = 1;
    if (!(flags & 4) && !open_arc(1, HomeDir))
        return 0;
    if (get_arc(1, lev + 0x4F, (char far *)buf) != 0x86)
        ok = 0;
    if (!(flags & 4))
        close_arc(1);
    for (i = 0; i < 0x40; i++) {
        TxmID[i] = buf[i];
        TxmTerr[i] = 0;
    }
    Load_Terrains(TxmID);
    for (i = 0; i < 3; i++) {
        ActDoors[i * 2] = buf[i + 0x40] & 0xFF;
        ActDoors[i * 2 + 1] = buf[i + 0x40] >> 8;
    }
    load_txtmaps();
    return ok;
}

unsigned char far Txm_Save(int arc, int lev, register int flags)
{
    unsigned char ok;
    int buf[0x46];
    register int i;

    ok = 1;
    for (i = 0; i < 0x40; i++)
        buf[i] = TxmID[i];
    for (i = 0; i < 3; i++)
        buf[i + 0x40] = (ActDoors[i * 2 + 1] << 8) | (ActDoors[i * 2] & 0xFF);
    if (!(flags & 4) && (flags & 2) && !open_arc(1, HomeDir) && !open_arc(1, "DATA\\"))
        return 0;
    ok = put_arc(1, lev + 0x4F, (char far *)buf, 0x86);
    if (!(flags & 4))
        close_arc(1);
    return ok;
}

void far load_txtmaps(void)
{
    load_tr_ems("t64");
    load_doors();
}

/* IDA's LoadTerrainDat_ovr140_24B. FM Towns has Load_Terrains_ at the same place, after
   load_txtmaps_, doing the same: fopen the terrain file, then for each of the 64 textures
   fseek to twice its number and fread two bytes into TxmTerr. */
void far Load_Terrains(int *ids)
{
    register int i;
    FILE *fp;

    fp = fopen("DATA\\terrain.dat", "rb");
    if (fp != NULL) {
        for (i = 0; i < 0x40; i++) {
            fseek(fp, (long)ids[i] * 2, SEEK_SET);
            fread(&TxmTerr[i], 2, 1, fp);
        }
        fclose(fp);
    }
}
