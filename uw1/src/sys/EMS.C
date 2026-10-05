/* target: seg012 */
/* opts: -mm -1 -G -O -Y -d */
/* EMS (LIM expanded memory) driver calls through int 67h: the whole of UW1's DOS resident
   segment seg012, in original order. The same code as UW2's EMS.C (seg013_1D3C), less
   the version check: seg012_B checks for the EMM driver, allocates a handle of up to
   max_pages 16 KB pages and names it "UW", and finds the page frame; the game keeps most
   of its art, textures, sounds and critter animations in those pages. Mapping uses
   function 44h for one page and the EMS 4.0 function 50h for up to four at once. The
   handle is also stored in the 3D renderer's data (seg051_C4D7). Owns ems_frame and the
   handle, page count and mapping array.

   UW1's differences: seg012_B does not ask for the EMM version (UW2 requires 4.0 with
   function 46h), though it names the handle and maps with 4.0 functions; and the
   four-page mapper seg012_BB comes before the one-page seg012_10F (UW2 has them the other
   way round).

   name: descriptive (the EMS calls). The FM Towns build has no EMS, so the functions keep
   their IDA names (UW1's listing names here, as UW2's keep its own).
   match: plain C with Turbo C's pseudo-registers and geninterrupt() everywhere except
   the string compare against the EMM device name and one `mov bx,0`, which need inline
   assembly. The file went through TASM (#pragma inline): the call from seg012_B to the
   later seg012_12C is `push cs; call near; nop`, TASM's rewrite of a far call into its
   own segment, where TCC alone emits a far call.
   port: dos-only (UW2). The port never compiles this file (tools/portcheck.py skips it): its
   EMS emulation (src/port/mem/, docs/PORT.md "EMS") defines the same functions. */
#pragma inline
#include <dos.h>
#include "sys.h"
#include "ui.h"
#include "view3d.h"

/* UW1: the 3D renderer's copy of the handle (seg051, its far data, assembly; UW2's
   seg052_519C_E4D4) and the byte seg012_15E steps (UW2's dseg_67d6_120, in another
   file's data at DS:10C); the listing's names. */

/* the EMS handle name and the EMM driver's device name */
static char ems_name[10] = "UW";            /* DS:F8 */
static char emm_id[] = "EMMXXXX0";          /* DS:102 */

/* match: _BSS is laid out by name; the statics have no original name, so theirs were
   chosen to land where the EXE has them around ems_frame (tools/bssorder.py) */
static uint16 ems_pages;                    /* DS:24B6, pages allocated */
static uint16 ems_avail;                    /* DS:24B8, pages free when checked */
uint16 ems_frame;                           /* DS:24BA, segment of the EMS page frame */
static uint16 ems_handle;                   /* DS:24BC */
static uint16 ems_page_map[8];           /* DS:24BE, logical and physical page pairs */

/* Opens EMS: the int 67h vector's segment must hold a driver whose device name (at
   offset 10) is "EMMXXXX0"; then status (40h) and page
   counts (42h). Fails (returns 0, ems_handle FFFFh) if fewer than min_pages are free;
   otherwise allocates the smaller of the free count and max_pages (43h), names the handle
   "UW" (5301h), gets the page frame segment (41h) into ems_frame, and returns the number
   of pages allocated. */
int far seg012_B(unsigned min_pages, unsigned max_pages)
{
    unsigned total;                         /* the total page count, unused */

    /* match: the DOS code loads SI twice (UW1: around the handle's reset) */
    _SI = (unsigned)ems_name;
    ems_handle = 0xFFFF;
    _SI = (unsigned)emm_id;
    _AX = 0x3567;
    geninterrupt(0x21);
    _DI = 10;                               /* ES:DI, the device name in the driver header */
    _CX = 8;
    asm cld
    asm repe cmpsb
    asm jnz fail
    _AX = 0x4000;
    geninterrupt(0x67);
    if (_AH) goto fail;
    _AX = 0x4200;
    geninterrupt(0x67);
    if (_AH) goto fail;
    ems_avail = _BX;
    total = _DX;
    if (ems_avail < min_pages) {
fail:
        return 0;
    }
    ems_pages = ems_avail < max_pages ? ems_avail : max_pages;
    _BX = ems_pages;
    _AX = 0x4300;
    geninterrupt(0x67);
    if (_AH) goto fail;
    ems_handle = _DX;
    seg012_12C(ems_handle, ems_name);
    _AX = 0x4100;
    geninterrupt(0x67);
    if (_AH) goto fail;
    ems_frame = _BX;
    seg051_C4D7 = ems_handle;
    return ems_pages;
}

/* Frees the handle (45h), if one was allocated. free_mem calls it at shutdown. */
void far seg012_A6(void)
{
    if (ems_handle != 0xFFFF) {
        _DX = ems_handle;
        _AX = 0x4500;
        geninterrupt(0x67);
    }
}

/* Maps count (at most 4) consecutive logical pages from `logical` into consecutive
   physical pages from `physical` in one call (EMS 4.0 function 5000h, which takes the
   pairs in ems_page_map); 1 if it worked. UW1: a signed char (TMPALLOC.C's set_workspace
   tests it with cbw). */
char far seg012_BB(unsigned physical, unsigned logical, int count)
{
    register int i;

    if (count > 4) return 0;
    for (i = 0; i < count; i++) {
        ems_page_map[i * 2] = logical++;
        ems_page_map[i * 2 + 1] = physical++;
    }
    _SI = (unsigned)ems_page_map;
    _DX = ems_handle;
    _CX = count;
    _AX = 0x5000;
    geninterrupt(0x67);
    if (_AH == 0) return 1;
    return 0;
}

/* Maps logical page `logical` of the handle into physical page `physical` (0 to 3) of
   the frame (44h); 1 if it worked. */
char far seg012_10F(char physical, unsigned logical)
{
    _AH = 0x44;
    _AL = physical;
    _BX = logical;
    _DX = ems_handle;
    geninterrupt(0x67);
    if (_AH == 0) return 1;
    return 0;
}

/* Gives the handle an 8-byte name (EMS 4.0 function 5301h). */
void far seg012_12C(unsigned handle, char far *name)
{
    _BX = FP_SEG(name);
    _SI = FP_OFF(name);
    _DX = handle;
    _AX = 0x5301;
    geninterrupt(0x67);
}

/* The rest of the segment: allocating a one-page handle, mapping a handle's page 0 into
   the next of the four physical pages in turn (forgetting the pages TMPALLOC.C's callers
   think are mapped), and freeing a handle. PANELS.C's init_panelflip takes three such
   handles for the panel turns and maps them with seg012_15E; seg012_1B1 frees them. */
int far seg012_141(uint16 *handle)
{
    _BX = 1;
    _AX = 0x4300;
    geninterrupt(0x67);
    if (_AH == 0) {
        *handle = _DX;
        return 1;
    }
    return 0;
}

/* Maps logical page 0 of handle into the next physical page in turn (dseg_5c99_10C goes
   round 0 to 3) and returns its address in the page frame, 0 on an EMS error. Any page
   cached as mapped (crit_inpage, obj_inpage1, tmap_inpage) is marked unknown, since one
   of the four is now replaced. */
void far *far seg012_15E(unsigned handle)
{
    crit_inpage = obj_inpage1 = tmap_inpage = 0xFF;
    dseg_5c99_10C = (dseg_5c99_10C + 1) & 3;
    _AH = 0x44;
    _AL = dseg_5c99_10C;
    asm mov bx,0                            /* match: _BX = 0 gives xor bx,bx */
    _DX = handle;
    geninterrupt(0x67);
    if (_AH == 0) return MK_FP(ems_frame + (dseg_5c99_10C << 10), 0);
    return 0;
}

/* Frees an EMS handle (int 67h function 45h). */
void far seg012_1B1(unsigned handle)
{
    _DX = handle;
    _AX = 0x4500;
    geninterrupt(0x67);
}
