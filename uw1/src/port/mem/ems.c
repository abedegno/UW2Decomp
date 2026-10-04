/* ems.c: replaces src/sys/EMS.C (port: dos-only) and the EMS driver under it. The same
   functions, on the emulation of the driver's memory in Exhume's runtime (mem/emm.c): a store
   of 16 KB logical pages for the one handle the game allocates, and a 64 KB page frame at
   segment E000h that is a region of the paragraph map, in which a page mapped into two slots
   stays one page. The game's own record of what is mapped (TMPALLOC.C, PGCACHE.ASM, PANELS.C,
   SOUND.C) stays true. The emulated driver reports 512 free pages (8 MB), as UW2's port does
   for the same js-dos set-up the replays are recorded in.

   UW1's EMS.C (seg012) is UW2's without the version check, with the four-page mapper first.
   seg012_141, _15E and _1B1 (a one-page handle of its own, mapped in turn into the four slots)
   are never called (EMS.C's comment); they stop the port if anything reaches them. */
#include "compat.h"
#include "sys.h"
#include "view3d.h"
#include "port.h"

#define EMS_FREE 512
#define FRAME_SEG 0xE000u

extern uint16 seg051_C4D7;                      /* the 3D renderer's copy of the handle (FD58) */

uint16 ems_frame;                               /* EMS.C's DS:24BA */
static uint16 ems_handle = 0xFFFF;

/* Opens EMS: allocates the smaller of the free count and max_pages, failing below min_pages,
   names the handle and finds the frame; the number of pages allocated, or 0. */
int seg012_B(unsigned min_pages, unsigned max_pages)
{
    int pages;
    ems_handle = 0xFFFF;
    pages = emm_open(min_pages, max_pages, EMS_FREE, FRAME_SEG);
    if (!pages) return 0;
    ems_handle = 1;
    ems_frame = FRAME_SEG;
    seg051_C4D7 = ems_handle;
    return pages;
}

void seg012_A6(void)
{
    if (ems_handle != 0xFFFF) {
        emm_close();
        ems_handle = 0xFFFF;
    }
}

char seg012_BB(unsigned physical, unsigned logical, int count)
{
    int i;
    if (count > 4) return 0;
    for (i = 0; i < count; i++)
        if (!emm_map(physical + i, (logical + i) & 0xFFFF)) return 0;
    return 1;
}

char seg012_10F(char physical, unsigned logical)
{
    return (char)emm_map((unsigned char)physical, logical & 0xFFFF);
}

void seg012_12C(unsigned handle, char *name)
{
    (void)handle;
    (void)name;
}

int seg012_141(uint16 *handle)
{
    (void)handle;
    port_halt("EMS.C's seg012_141 (never called in DOS)");
}

void *seg012_15E(unsigned handle)
{
    (void)handle;
    port_halt("EMS.C's seg012_15E (never called in DOS)");
}

void seg012_1B1(unsigned handle)
{
    (void)handle;
    port_halt("EMS.C's seg012_1B1 (never called in DOS)");
}
