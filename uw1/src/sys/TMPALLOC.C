/* target: seg042 */
/* opts: -mm -1 -G -O -Y -d */
/* Expanded memory as the game uses it, and the shared workspace: the whole of UW1's DOS
   resident segment seg042, in original order. UW1 has no symbol-bearing build; the names
   are UW2's (the FM Towns symbol table; UW2's TMPALLOC.C, seg042_35ED), the code being the
   same routines.

   init_mem allocates the EMS handle through EMS.C (asking for 54 to 68 pages, an even
   number chosen with rand(), and needing at least 30) and mem_setup lays the logical
   pages out: pages 0-3 the workspace (get_workspace maps all four into the frame), page 9
   screen graphics (seg051_C377), the texture pages from 10 (seg051_C375; LOADGR.C sets
   seg051_C376, the second texture run, nine pages on), and critter animations from page
   22 (crit_fpage), two pages each (crit_nlpages pairs). The four physical pages of the
   frame normally hold a critter's pair (crit_inpage), an object art page (obj_inpage1)
   and a texture page (tmap_inpage); 0xFF in an *_inpage variable means nothing known is
   mapped there. Code that needs a 64 KB scratch buffer takes the whole frame with
   get_workspace and gives it back with release_workspace, which maps the saved pages
   back in.

   UW1's differences: init_mem also takes 64 KB and a paragraph of conventional memory
   with farmalloc (conv_ws), whose first whole paragraph (conv_ws_seg) seg042_190 hands to
   another file's global (DS:7178); free_mem frees it as well as the EMS handle, and only
   frees the handle if one was allocated; there is no map_crit_pages and no sound page;
   get_workspace sets ws_active only once the mapping has worked.

   name: inferred, the job of System Shock's TMPALLOC.C (temp_malloc, temp_free).
   init_mem, free_mem, mem_setup, get_workspace and set_workspace are UW2's names for the
   routines at the same positions doing the same work (the target table keeps the
   listing's names); seg042_190 and seg042_19B (UW2's seg042_35ED_12B) keep the listing's.
   The 3D renderer's page bytes C375, C376 and C377 in seg051 have no counterpart of the
   same shape in UW2, so they keep the listing's names too. */

#include <dos.h>
#include <stdlib.h>
#include <alloc.h>
/* UW1: ws_active is a signed char (cbw); sys.h has UW2's unsigned one, renamed out of the
   way here. */
#define ws_active UW2_ws_active
#include "gfx.h"
#include "sys.h"
#include "view3d.h"
#undef ws_active

/* UW1: EMS.C's functions under their UW1 listing names, the console printer (UW2's
   PrintStringToConsole_seg017_DE) under its listing name, and the 3D renderer's page
   bytes and the global seg042_190 sets, which the listing does not name. */
int far seg012_B(unsigned min_pages, unsigned max_pages);
void far seg012_A6(void);
char far seg012_BB(unsigned physical, unsigned logical, int count);
char far seg012_10F(char physical, unsigned logical);
void far seg015_1F9B_E8(char far *text);
extern unsigned char far seg051_C375;
extern unsigned char far seg051_C376;
extern unsigned char far seg051_C377;
extern uint16 dseg_5c99_7178;

void far mem_setup(int page);
void far seg042_190(void);

/* This file's _DATA, DS:0A48..0A5E, with its string. */
int16 dseg_5c99_A48 = 0;                /* EMS pages allocated (UW2: dseg_67d6_920) */
char ws_active = 0;                     /* DS:0A4A, UW1: signed (cbw) */
unsigned char gfx_inpage = 0;           /* DS:0A4B */
unsigned char obj_inpage2 = 0;          /* DS:0A4C */
unsigned char saved_tmap_inpage = 0;    /* DS:0A4D */

/* This file's _BSS, DS:364A..364F.
   name: chosen for layout (bssorder.py keys 219 and 331), no original names. */
void huge *conv_ws;                     /* DS:364A, the conventional memory block (public: CUTS.C reads it); huge,
                                           as the tests compare it through N_PCMP@ */
static uint16 conv_ws_seg;              /* DS:364E, its first whole paragraph */

/* Allocates EMS: asks for 54 to 68 pages and needs at least 30, else first_punt with "Out
   of EMS Memory". If it got fewer than it asked for but at least 36, it frees them and
   allocates six fewer than it got, then tells the overlay manager to swap overlays to EMS
   (_OvrInitEms(0, 0, 0)). UW1: then allocates the conventional workspace, four 16 KB
   pages and a paragraph, or stops with "Out of Low Memory" (B007). */
void far init_mem(void)
{
    register int page;
    register int nws = 4;

    page = ((rand() % 8) << 1) + 0x36;
    if (page > 0) {
        dseg_5c99_A48 = seg012_B(0x1E, page);
        if (dseg_5c99_A48 != 0) {
            if (dseg_5c99_A48 < page && dseg_5c99_A48 >= 0x24) {
                page = dseg_5c99_A48 - 6;
                seg012_A6();
                dseg_5c99_A48 = seg012_B(0x1E, page);
                if (dseg_5c99_A48 < 0x1E)
                    first_punt(ERR_EMS | 2);
            }
            mem_setup(dseg_5c99_A48);
            seg015_1F9B_E8("EMS allocated\r\n$");
            _OvrInitEms(0, 0, 0);
        } else {
            first_punt(ERR_EMS | 1);
        }
    }
    if ((conv_ws = farmalloc(((long)nws << 14) + 0x10)) == 0L)
        first_punt(ERR_LOWMEM | 7);
    conv_ws_seg = FP_SEG(conv_ws) + 1;
    seg042_190();
}

void far free_mem(void)
{
    if (dseg_5c99_A48 > 0)
        seg012_A6();
    if (conv_ws != 0L)
        farfree(conv_ws);
}

/* Lays out the logical pages for a handle of `page` pages (see the top of the file) and
   marks every physical page unknown. */
void far mem_setup(int page)
{
    crit_fpage = 0x16;
    crit_nlpages = (page - 0x16) >> 1;
    crit_inpage = 0xFF;
    obj_inpage1 = 0xFF;
    seg051_C377 = 9;
    seg051_C375 = seg051_C377 + 1;
    tmap_inpage = 0xFF;
    seg051_C376 = 0xFF;
    EmsBuff = ems_frame;
}

/* UW1: hands the conventional workspace's paragraph to DS:7178 (another file's). */
void far seg042_190(void)
{
    dseg_5c99_7178 = conv_ws_seg;
}

/* Forgets what is mapped in physical pages 0-3, so the next user maps its own. */
void far seg042_19B(void)
{
    crit_inpage = 0xFF;
    obj_inpage1 = 0xFF;
    tmap_inpage = 0xFF;
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
        seg042_19B();
        if (seg012_BB(0, 0, 4)) {
            ws_active = 1;
            return ems_frame;
        }
        pfatal_code(ERR_EMS | 3);
    } else {
        pfatal_code(ERR_EMS | 4);
    }
    return 0;
}

/* Maps the workspace in again while it is held, saving the current mapping as
   get_workspace does; the frame segment, or 0. */
int far set_workspace(void)
{
    if (!ws_active) return 0;
    if (!seg012_BB(0, 0, 4)) return 0;
    gfx_inpage = crit_inpage;
    obj_inpage2 = obj_inpage1;
    saved_tmap_inpage = tmap_inpage;
    seg042_19B();
    return ems_frame;
}

/* Gives the workspace back: maps the saved critter pair, object page and texture page
   back into the frame. */
void far release_workspace(void)
{
    if (ws_active) {
        ws_active = 0;
        if ((crit_inpage = gfx_inpage) != 0xFF)
            seg012_BB(0, crit_fpage + ((unsigned)gfx_inpage << 1), 2);
        if ((obj_inpage1 = obj_inpage2) != 0xFF)
            seg012_10F(2, obj_inpage2);
        if ((tmap_inpage = saved_tmap_inpage) != 0xFF)
            seg012_10F(3, saved_tmap_inpage);
    }
}
