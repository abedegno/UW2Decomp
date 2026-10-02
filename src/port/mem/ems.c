/* ems.c: replaces src/sys/EMS.C (port: dos-only) and the EMS 4.0 driver under it (docs/PORT.md,
   "EMS"). The same functions, on an emulation: a store of 16 KB logical pages for the one
   handle the game allocates, and a 64 KB page frame at segment E000h that is a region of the
   paragraph map. Mapping a page into a frame slot copies the page that was there back to the
   store and copies the new one in, so the frame holds exactly what DOS would hold, and the
   game's own record of what is mapped (TMPALLOC.C, PGCACHE.ASM, PANELS.C, SOUND.C) stays true.
   The emulated driver reports 512 free pages (8 MB), as the reference DOS set-up had. */
#include "compat.h"
#include "sys.h"
#include "view3d.h"
#include "port.h"

#define EMS_FREE 512
#define FRAME_SEG 0xE000u

uint16 ems_frame;                               /* EMS.C's DS:22CE */
static uint16 ems_pages;
static uint16 ems_handle = 0xFFFF;
static unsigned char *store;
/* The frame: 64 KB mapped twice in a row, so that a pointer run past its end wraps to its
   start as a far pointer's offset does (mem/frame.c). */
unsigned char *port_frame_alloc(void);
static unsigned char *frame;
static int slot[4] = { -1, -1, -1, -1 };        /* the logical page in each frame slot */

static int map(unsigned physical, unsigned logical)
{
    if (ems_handle == 0xFFFF || physical > 3) return 0;
    if (logical != 0xFFFF && logical >= ems_pages) return 0;
    if (slot[physical] >= 0)
        memcpy(store + (size_t)slot[physical] * 0x4000, frame + physical * 0x4000, 0x4000);
    if (logical == 0xFFFF) { slot[physical] = -1; return 1; }
    memcpy(frame + physical * 0x4000, store + (size_t)logical * 0x4000, 0x4000);
    slot[physical] = (int)logical;
    return 1;
}

/* Opens EMS: allocates the smaller of the free count and max_pages, failing below min_pages,
   names the handle and finds the frame; the number of pages allocated, or 0. */
int seg013_1D3C_A(unsigned min_pages, unsigned max_pages)
{
    ems_handle = 0xFFFF;
    if (EMS_FREE < min_pages) return 0;
    ems_pages = (uint16)(EMS_FREE < max_pages ? EMS_FREE : max_pages);
    store = calloc(ems_pages, 0x4000);
    if (!store) return 0;
    ems_handle = 1;
    ems_frame = FRAME_SEG;
    if (!frame && !(frame = port_frame_alloc())) port_fatal("ems: cannot map the page frame");
    pm_remove(frame);
    pm_add("EMS page frame", frame, 0x10000, FRAME_SEG);
    seg052_519C_E4D4 = ems_handle;
    port_log("ems: %u pages\n", ems_pages);
    return ems_pages;
}

void seg013_1D3C_B2(void)
{
    if (ems_handle != 0xFFFF) {
        free(store);
        store = NULL;
        ems_handle = 0xFFFF;
        slot[0] = slot[1] = slot[2] = slot[3] = -1;
    }
}

char MapMemory_seg013_1D3C_C7(char physical, unsigned logical)
{
    return (char)map((unsigned char)physical, logical & 0xFFFF);
}

unsigned char seg013_1D3C_E4(unsigned physical, unsigned logical, int count)
{
    int i;
    if (count > 4) return 0;
    for (i = 0; i < count; i++)
        if (!map(physical + i, logical + i)) return 0;
    return 1;
}

void seg013_1D3C_138(unsigned handle, char *name)
{
    (void)handle;
    (void)name;
}
