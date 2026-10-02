/* target: ovr152 */
/* opts: -mm -1 -G -O -Y -d */
/* LIGHTING.C: the light level of the 3D view. The whole of DOS overlay ovr152, in
   original order.

   The renderer shades through cLightTabs, 16 colour maps of 256 bytes (UW-Formats 3.1.3:
   the first maps colours to themselves, the last to near black). init_lighting fills it
   from DATA\LIGHT.DAT at start-up and fills cXfer, the five translation tables in
   SCALEBM.ASM, from DATA\XFER.DAT. set_light picks a light level: it reads that level's
   row of DATA\SHADES.DAT (six words: smooth_div, smooth_base, smooth_lowpass, curvrad,
   distpoly, dist8), which set how fast the view darkens with distance and how far it
   reaches, and swaps in DATA\MONO.DAT's grey maps for level 5. random_light is the
   mushroom effect from PLAYDATA.C's set_drugged. PHYSICS.C, PLAYDATA.C and PLAYER.C
   call set_light as the player's light source or the tile's light bit changes.

   name: descriptive (the file's own name is not known). */

#include <dos.h>
#include <io.h>
#include <mem.h>
#include "file.h"
#include "gfx.h"
#include "map.h"
#include "player.h"
#include "sys.h"
#include "view3d.h"

/* The light level set_light last loaded, 5 for mono.dat; 0xFF until the first call. */
/* name: FM Towns _cur_light_level, which its set_light_ tests the same way. DS:1AAC, the
   first byte of this file's _DATA (which holds DS:1AAD and is word-aligned). */
unsigned char cur_light_level = 0xFF;



/* Reads exactly n bytes of the file `name` to seg:off; 1 when it got all n, else 0. */
/* name: FM Towns lget_: target table currently calls this lget. */
int far lget(char *name, unsigned off, unsigned seg, unsigned n)
{
    register int got = -1;
    int handle;

    if ((handle = our_open(name, 1, 0)) != -1) {
        got = intoFarBuffer_ovr167_5DA(handle, MK_FP(seg, off), n);
        close(handle);
    }
    if (got == n)
        return 1;
    return 0;
}

/* Sets light level lightLevel (SHADES.DAT has eight rows, 0 darkest to 7 brightest, the
   view radius curvrad growing from 3 to 7). Does nothing if that level is already set.
   Leaving level 5 reloads LIGHT.DAT; entering it loads MONO.DAT. Then copies the level's
   row into the shading globals, has the vision grid rebuilt for the new radius
   (preset_grid) and flags the view for redrawing (editchng(2)). */
/* name: FM Towns set_light_: same shades.dat lookup and smooth parameters. */
void far set_light(signed char lightLevel)
{
    int ShadesDataRow_var_C[6];
    int handle;
    int diValue;

    if (cur_light_level == lightLevel)
        return;
    if (cur_light_level == 5) {
        lget("light.dat", FP_OFF(cLightTabs), FP_SEG(cLightTabs), 0x1000);
    } else if (lightLevel == 5) {
        lget("mono.dat", FP_OFF(cLightTabs), FP_SEG(cLightTabs), 0x1000);
    }
    cur_light_level = lightLevel;
    if ((handle = our_open("shades.dat", 1, 0)) < 0)
        return;
    diValue = (int)lightLevel * 12;
    lseek(handle, diValue, 0);
    read(handle, ShadesDataRow_var_C, 12);
    smooth_div = ShadesDataRow_var_C[0];
    if ((int)smooth_div <= 1)
        smooth_div = 1;
    smooth_base = ShadesDataRow_var_C[1];
    smooth_lowpass = ShadesDataRow_var_C[2];
    curvrad = ShadesDataRow_var_C[3];
    distpoly = ShadesDataRow_var_C[4];
    dist8 = ShadesDataRow_var_C[5];
    close(handle);
    preset_grid(curvrad);
    editchng(2);
}

/* Start-up (UWEDIT.C): LIGHT.DAT into cLightTabs and XFER.DAT into cXfer. */
/* name: FM Towns init_lighting_: loads light.dat and xfer.dat through lget_. */
void far init_lighting(void)
{
    lget("light.dat", FP_OFF(cLightTabs), FP_SEG(cLightTabs), 0x1000);
    lget("xfer.dat", (unsigned)cXfer,
                         (unsigned)FP_SEG(cXfer), 0x500);
}

/* The mushroom effect (set_drugged). enabled: replace the 16 light maps with 4 KB of
   low memory, then set the first two entries of each map to 0. The source is segment 0
   at an offset equal to the byte stored at ModelData_seg052_519C_2600 (a value read,
   not an address), so the maps are copied from the interrupt vector table and what
   follows it. The FM Towns build copies from the start of its data segment (__dbstart)
   instead, so in both the "random" maps are arbitrary memory. Not enabled: reload
   LIGHT.DAT, or MONO.DAT at level 5. */
/* name: FM Towns random_light_: replaces light data and clears two bytes per row. */
void far random_light(char enabled)
{
    int i;

    if (enabled != 0) {
        /* match: the EXE reads the byte for the segment too and then discards it
           (mov al,0), which is what the byte times 0 compiles to */
        movedata(ModelData_seg052_519C_2600 * 0,
                                        ModelData_seg052_519C_2600,
                                        FP_SEG(cLightTabs),
                                        FP_OFF(cLightTabs),
                                        0x1000);
        for (i = 0; i < 16; i++) {
            cLightTabs[i * 256] = 0;
            ((char far *)MK_FP(FP_SEG(cLightTabs), FP_OFF(cLightTabs)))[i * 256 + 1] = 0;
        }
    } else {
        lget(cur_light_level != 5 ?
                             "light.dat" : "mono.dat",
                             FP_OFF(cLightTabs), FP_SEG(cLightTabs), 0x1000);
    }
}
