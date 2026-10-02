/* target: seg042_35ED */
/* opts: -mm -1 -G -O -Y -d */
#include <dos.h>
#include "gfx.h"
#include "sys.h"
#include "view3d.h"

/* This file's _DATA starts at DS:0920 (seg040's ends at 091F, odd) with these, then its
   string. Only this file uses all five; seg041, the other file between seg040's data and
   this, uses none. */
int dseg_67d6_920 = 0;                  /* DOS EMS page count; no confirmed FM Towns counterpart. */
unsigned char ws_active = 0;            /* DS:0922 */
unsigned char gfx_inpage = 0;           /* DS:0923 */
unsigned char obj_inpage2 = 0;          /* DS:0924 */
unsigned char saved_tmap_inpage = 0;    /* DS:0925 */

int far rand(void);
void far MapMemory_seg013_1D3C_C7(int phys, int page);
unsigned char far seg013_1D3C_E4(int a, int b, int c);

void far init_mem(void)
{
    register int page;
    page = ((rand() % 8) << 1) + 0x59;
    dseg_67d6_920 = seg013_1D3C_A(0x29, page);
    if (dseg_67d6_920 != 0) {
        if (dseg_67d6_920 < page && dseg_67d6_920 >= 0x2F) {
            page = dseg_67d6_920 - 6;
            seg013_1D3C_B2();
            dseg_67d6_920 = seg013_1D3C_A(0x29, page);
            if (dseg_67d6_920 < 0x29)
                first_punt(0x2002);
        }
        mem_setup(dseg_67d6_920);
        PrintStringToConsole_seg017_DE("EMS allocated\r\n$");
        _OvrInitEms(0, 0, 0);
    } else {
        first_punt(0x2001);
    }
}

void far free_mem(void) { seg013_1D3C_B2(); }

void far mem_setup(int page)
{
    crit_fpage = 0x21;
    crit_nlpages = (page - crit_fpage) >> 1;
    crit_inpage = 0xFF;
    obj_inpage1 = 0xFF;
    scrgr_fpage = 0x0A;
    tmap_fpage = scrgr_fpage + 2;
    sound_fpage = tmap_fpage + 0x10;
    tmap_inpage = 0xFF;
    EmsBuff = ems_frame;
}

void far seg042_35ED_12B(void)
{
    crit_inpage = 0xFF;
    obj_inpage1 = 0xFF;
    tmap_inpage = 0xFF;
}

void far map_crit_pages(void)
{
    tmap_inpage = 0xFF;
    MapMemory_seg013_1D3C_C7(3, crit_fpage - 1);
}

int far get_workspace(void)
{
    if (!ws_active) {
        gfx_inpage = crit_inpage;
        obj_inpage2 = obj_inpage1;
        saved_tmap_inpage = tmap_inpage;
        seg042_35ED_12B();
        ws_active = 1;
        if (seg013_1D3C_E4(0, 0, 4))
            return ems_frame;
        pfatal_code(0x2003);
    } else {
        pfatal_code(0x2004);
    }
    ws_active = 0;
    return 0;
}

int far set_workspace(void)
{
    if (!ws_active) return 0;
    if (!seg013_1D3C_E4(0, 0, 4)) return 0;
    gfx_inpage = crit_inpage;
    obj_inpage2 = obj_inpage1;
    saved_tmap_inpage = tmap_inpage;
    seg042_35ED_12B();
    return ems_frame;
}

void far release_workspace(void)
{
    if (ws_active) {
        ws_active = 0;
        if ((crit_inpage = gfx_inpage) != 0xFF)
            seg013_1D3C_E4(0, crit_fpage + ((unsigned)gfx_inpage << 1), 2);
        if ((obj_inpage1 = obj_inpage2) != 0xFF)
            MapMemory_seg013_1D3C_C7(2, obj_inpage2);
        if ((tmap_inpage = saved_tmap_inpage) != 0xFF)
            MapMemory_seg013_1D3C_C7(3, saved_tmap_inpage);
    }
}
