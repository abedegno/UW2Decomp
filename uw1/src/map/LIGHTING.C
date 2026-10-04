/* target: ovr142 */
/* opts: -mm -1 -G -O -Y -d */
/* LIGHTING.C: the light level of the 3D view. The whole of UW1's DOS overlay ovr142
   (UW2's ovr152), in original order. Seeded from UW2Decomp's src/map/LIGHTING.C.

   The renderer shades through cLightTabs, 16 colour maps of 256 bytes (the first maps
   colours to themselves, the last to near black). init_lighting fills it from
   DATA\LIGHT.DAT at start-up and fills cXfer, the translation tables in seg003, from
   DATA\XFER.DAT. set_light picks a light level: it reads that level's row of
   DATA\SHADES.DAT (six words: smooth_div, smooth_base, smooth_lowpass, curvrad,
   distpoly, dist8), and swaps in DATA\MONO.DAT's grey maps for level 5. random_light is
   the mushroom effect.

   UW1 against UW2: no lget (each function opens, reads and closes the file itself, by
   its DATA\ path, rather than through our_open); XFER.DAT is 0x600 bytes (UW2: 0x500).

   name: UW2's (the FM Towns names), which also keep the EXE's overlay stub order
   (init_lighting 281, set_light 755, random_light 946). */

#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <mem.h>
#include "file.h"
#include "gfx.h"
#include "map.h"
#include "player.h"
#include "sys.h"
#include "view3d.h"

/* The light level set_light last loaded, 5 for mono.dat; 0xFF until the first call. */
/* name: FM Towns _cur_light_level. DS:1C50, the first byte of this file's _DATA. */
unsigned char cur_light_level = 0xFF;

/* Sets light level lightLevel (SHADES.DAT has eight rows, 0 darkest to 7 brightest).
   Does nothing if that level is already set. Leaving level 5 reloads LIGHT.DAT; entering
   it loads MONO.DAT. Then copies the level's row into the shading globals, has the
   vision grid rebuilt for the new radius (preset_grid) and flags the view for redrawing
   (editchng(2)). */
void far set_light(char lightLevel)
{
    int diValue;
    int16 ShadesDataRow_var_C[6];
    register int handle;
    register int shades;

    if (cur_light_level == lightLevel)
        return;
    if (cur_light_level == 5) {
        if ((handle = open("DATA\\light.dat", O_RDONLY | O_BINARY)) != -1) {
            intoFarBuffer_ovr167_5DA(handle, cLightTabs, 0x1000);
            close(handle);
        }
    } else if (lightLevel == 5) {
        if ((handle = open("DATA\\mono.dat", O_RDONLY | O_BINARY)) != -1) {
            intoFarBuffer_ovr167_5DA(handle, cLightTabs, 0x1000);
            close(handle);
        }
    }
    cur_light_level = lightLevel;
    shades = open("DATA\\shades.dat", O_RDONLY | O_BINARY);
    if (shades < 0)
        return;
    diValue = lightLevel * 12;
    lseek(shades, diValue, 0);
    read(shades, ShadesDataRow_var_C, 12);
    smooth_div = ShadesDataRow_var_C[0];
    if (smooth_div <= 1)
        smooth_div = 1;
    smooth_base = ShadesDataRow_var_C[1];
    smooth_lowpass = ShadesDataRow_var_C[2];
    curvrad = ShadesDataRow_var_C[3];
    distpoly = ShadesDataRow_var_C[4];
    dist8 = ShadesDataRow_var_C[5];
    close(shades);
    preset_grid(curvrad);
    editchng(2);
}

/* Start-up (UWEDIT.C): LIGHT.DAT into cLightTabs and XFER.DAT into cXfer. */
void far init_lighting(void)
{
    register int handle;

    if ((handle = open("DATA\\light.dat", O_RDONLY | O_BINARY)) != -1) {
        intoFarBuffer_ovr167_5DA(handle, cLightTabs, 0x1000);
        close(handle);
    }
    if ((handle = open("DATA\\xfer.dat", O_RDONLY | O_BINARY)) != -1) {
        intoFarBuffer_ovr167_5DA(handle, cXfer, 0x600);
        close(handle);
    }
}

/* The mushroom effect. enabled: replace the 16 light maps with 4 KB of low memory, then
   set the first two entries of each map to 0. The source is segment 0 at an offset
   equal to the byte stored at seg051:28A0 (a value read, not an address), so the maps
   are copied from the interrupt vector table and what follows it. Not enabled: reload
   LIGHT.DAT, or MONO.DAT at level 5. */
void far random_light(char enabled)
{
    register int i;
    register int handle;

    if (enabled != 0) {
        /* match: the EXE reads the byte for the segment too and then discards it
           (mov al,0), which is what the byte times 0 compiles to */
        movedata(seg051_28A0 * 0, seg051_28A0, FP_SEG(cLightTabs), FP_OFF(cLightTabs), 0x1000);
        for (i = 0; i < 16; i++) {
            cLightTabs[i * 256] = 0;
            ((char far *)MK_FP(FP_SEG(cLightTabs), FP_OFF(cLightTabs)))[i * 256 + 1] = 0;
        }
    } else {
        if (cur_light_level != 5)
            handle = open("DATA\\light.dat", O_RDONLY | O_BINARY);
        else
            handle = open("DATA\\mono.dat", O_RDONLY | O_BINARY);
        if (handle != -1) {
            intoFarBuffer_ovr167_5DA(handle, cLightTabs, 0x1000);
            close(handle);
        }
    }
}
