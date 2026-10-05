/* ems.c: replaces src/sys/EMS.C (port: dos-only) and the EMS 4.0 driver under it (docs/PORT.md,
   "EMS"). The same functions, on the emulation of the driver's memory in Exhume's runtime
   (mem/emm.c): a store of 16 KB logical pages for the one handle the game allocates, and a
   64 KB page frame at segment E000h that is a region of the paragraph map, in which a page
   mapped into two slots stays one page. Mapping a page into a frame slot copies the page that
   was there back to the store and copies the new one in, so the frame holds exactly what DOS
   would hold, and the game's own record of what is mapped (TMPALLOC.C, PGCACHE.ASM, PANELS.C,
   SOUND.C) stays true. The emulated driver reports 512 free pages (8 MB), as the reference DOS
   set-up had. */
#include "compat.h"
#include "sys.h"
#include "view3d.h"
#include "port.h"

#define EMS_FREE 512
#define FRAME_SEG 0xE000u

uint16 ems_frame;                               /* EMS.C's DS:22CE */
static uint16 ems_handle = 0xFFFF;

/* Opens EMS: allocates the smaller of the free count and max_pages, failing below min_pages,
   names the handle and finds the frame; the number of pages allocated, or 0. */
int seg013_1D3C_A(unsigned min_pages, unsigned max_pages)
{
    int pages;
    ems_handle = 0xFFFF;
    pages = emm_open(min_pages, max_pages, EMS_FREE, FRAME_SEG);
    if (!pages) return 0;
    ems_handle = 1;
    ems_frame = FRAME_SEG;
    seg052_519C_E4D4 = ems_handle;
    return pages;
}

void seg013_1D3C_B2(void)
{
    if (ems_handle != 0xFFFF) {
        emm_close();
        ems_handle = 0xFFFF;
    }
}

char MapMemory_seg013_1D3C_C7(char physical, unsigned logical)
{
    return (char)emm_map((unsigned char)physical, logical & 0xFFFF);
}

unsigned char seg013_1D3C_E4(unsigned physical, unsigned logical, int count)
{
    int i;
    if (count > 4) return 0;
    for (i = 0; i < count; i++)
        if (!emm_map(physical + i, logical + i)) return 0;
    return 1;
}

void seg013_1D3C_138(unsigned handle, char *name)
{
    (void)handle;
    (void)name;
}
