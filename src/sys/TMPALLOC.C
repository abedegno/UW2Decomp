/* target: seg042_35ED */
/* opts: -mm -1 -G -O -Y -d */
/* Expanded memory as the game uses it, and the shared workspace: the whole of DOS
   resident segment seg042_35ED. init_mem (called from init_world, UWEDIT.C) allocates
   the EMS handle through EMS.C and mem_setup lays the logical pages out:

     pages 0-3     the workspace (get_workspace maps all four into the frame)
     pages 10-11   screen graphics (scrgr_fpage; PANELS.C maps them into frame page 2)
     pages 12-27   texture maps (tmap_fpage, 16 pages)
     pages 28-31   digitised sounds (sound_fpage; SOUND.C maps them into frame page 2)
     page 32       CRIT\CR.AN, the critter animation table (CRPAGES.C loads it through
                   map_crit_pages, which the AI and the 3D view call before reading it)
     pages 33-     critter animations, two pages each (crit_fpage, crit_nlpages pairs)

   The four physical pages of the frame normally hold a critter's pair (pages 0-1,
   crit_inpage), an object art page (page 2, obj_inpage1) and a texture page (page 3,
   tmap_inpage); 0xFF in an *_inpage variable means nothing known is mapped there. Code
   that needs a 64 KB scratch buffer (the main menu, the automap, saving and restoring,
   conversations and bartering, full-screen pictures, SCD schedules) takes the
   whole frame with get_workspace and gives it back with release_workspace, which maps
   the saved pages back in. The page variables live in the 3D renderer's far data
   (view3d.h) because PGCACHE.ASM maps pages too.
   Pages 4-9 are not assigned here: LOADGR.C packs the art it keeps in EMS into logical
   pages from 4 upward (its ems_page starts at 4), so they are probably that art's.
   name: inferred, the job of System Shock's TMPALLOC.C (temp_malloc, temp_free). */

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

/* Allocates EMS: asks for 89 to 103 pages (an even number chosen with rand(), so 1.4 to
   1.6 MB; why it is random is not known) and needs at least 41 (656 KB), else first_punt
   with "Out of EMS Memory". If it got fewer than it asked for but at least 47, it frees
   them and allocates six fewer than it got, leaving six free pages, presumably for the
   overlay manager, which _OvrInitEms(0, 0, 0) then tells to swap overlays to EMS (with
   no handle given, Borland's overlay manager allocates its own pages). */
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
                first_punt(ERR_EMS | 2);
        }
        mem_setup(dseg_67d6_920);
        PrintStringToConsole_seg017_DE("EMS allocated\r\n$");
        _OvrInitEms(0, 0, 0);
    } else {
        first_punt(ERR_EMS | 1);
    }
}

void far free_mem(void) { seg013_1D3C_B2(); }

/* Lays out the logical pages for a handle of `page` pages (see the top of the file) and
   marks every physical page unknown. */
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

/* Forgets what is mapped in physical pages 0-3, so the next user maps its own. */
void far seg042_35ED_12B(void)
{
    crit_inpage = 0xFF;
    obj_inpage1 = 0xFF;
    tmap_inpage = 0xFF;
}

/* Maps logical page 32 (crit_fpage - 1, the CR.AN table) into physical page 3, where a
   texture page usually sits. */
void far map_crit_pages(void)
{
    tmap_inpage = 0xFF;
    MapMemory_seg013_1D3C_C7(3, crit_fpage - 1);
}

/* Takes the whole 64 KB page frame as scratch memory: saves what is mapped, maps logical
   pages 0-3 into physical pages 0-3, and returns the frame's segment. Not reentrant:
   asking twice is a fatal error (C004), as is a failed mapping (C003). */
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
        pfatal_code(ERR_EMS | 3);
    } else {
        pfatal_code(ERR_EMS | 4);
    }
    ws_active = 0;
    return 0;
}

/* Maps the workspace in again while it is held (after something else mapped pages over
   it), saving the current mapping as get_workspace does; the frame segment, or 0. */
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

/* Gives the workspace back: maps the saved critter pair, object page and texture page
   back into the frame. */
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
