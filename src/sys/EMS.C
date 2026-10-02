/* target: seg013_1D3C */
/* opts: -mm -1 -G -O -Y -d */
/* EMS (LIM expanded memory) driver calls through int 67h: the whole of DOS resident
   segment seg013_1D3C. UW2 needs EMS 4.0: seg013_1D3C_A checks for the EMM driver,
   requires version 4.0 or later, allocates a handle of up to max_pages 16 KB pages and
   names it "UW", and finds the page frame; the game keeps most of its art, textures,
   sounds and critter animations in those pages (see TMPALLOC.C for the page layout).
   Mapping uses function 44h for one page and the EMS 4.0 function 50h for up to four at
   once. TMPALLOC.C is the only caller in C; the handle is also stored in the 3D
   renderer's data (seg052_519C_E4D4), which PGCACHE.ASM's page cache uses to map pages
   with int 67h itself. Owns ems_frame and the handle, page count and mapping array.
   name: descriptive (the EMS calls). The FM Towns build has no EMS, so the functions keep
   their IDA names; the three unreferenced functions after seg013_1D3C_138 are named by
   their offsets.
   match: plain C with Turbo C's pseudo-registers and geninterrupt() everywhere except
   the string compare against the EMM device name and one `mov bx,0`, which need inline
   assembly. The file went through TASM (#pragma inline): the call from seg013_1D3C_A to
   the later seg013_1D3C_138 is `push cs; call near; nop`, TASM's rewrite of a far call
   into its own segment, where TCC alone emits a far call. */
#pragma inline
#include <dos.h>
#include "sys.h"
#include "ui.h"
#include "view3d.h"

/* the EMS handle name and the EMM driver's device name */
static char ems_name[10] = "UW";            /* DS:10C */
static char emm_id[] = "EMMXXXX0";          /* DS:116 */

/* match: _BSS is laid out by name; the statics have no original name, so theirs were
   chosen to land where the EXE has them around ems_frame (tools/bssorder.py) */
static unsigned ems_pages;                  /* DS:22CA, pages allocated */
static unsigned ems_avail;                  /* DS:22CC, pages free when checked */
unsigned ems_frame;                         /* DS:22CE, segment of the EMS page frame */
static unsigned ems_handle;                 /* DS:22D0 */
static unsigned ems_page_map[8];         /* DS:22D2, logical and physical page pairs */

/* Opens EMS: the int 67h vector's segment must hold a driver whose device name (at
   offset 10) is "EMMXXXX0"; then version (46h, at least 4.0), status (40h) and page
   counts (42h). Fails (returns 0, ems_handle FFFFh) if fewer than min_pages are free;
   otherwise allocates the smaller of the free count and max_pages (43h), names the handle
   "UW" (5301h), gets the page frame segment (41h) into ems_frame, and returns the number
   of pages allocated. */
int far seg013_1D3C_A(unsigned min_pages, unsigned max_pages)
{
    unsigned total;                         /* the total page count, unused */

    ems_handle = 0xFFFF;
    /* match: the DOS code loads SI twice */
    _SI = (unsigned)ems_name;
    _SI = (unsigned)emm_id;
    _AX = 0x3567;
    geninterrupt(0x21);
    _DI = 10;                               /* ES:DI, the device name in the driver header */
    _CX = 8;
    asm cld
    asm repe cmpsb
    asm jnz fail
    _AX = 0x4600;
    geninterrupt(0x67);
    if (_AH) goto fail;
    if (_AL < 0x40) goto fail;
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
    seg013_1D3C_138(ems_handle, ems_name);
    _AX = 0x4100;
    geninterrupt(0x67);
    if (_AH) goto fail;
    ems_frame = _BX;
    seg052_519C_E4D4 = ems_handle;
    return ems_pages;
}

/* Frees the handle (45h), if one was allocated. free_mem calls it at shutdown. */
void far seg013_1D3C_B2(void)
{
    if (ems_handle != 0xFFFF) {
        _DX = ems_handle;
        _AX = 0x4500;
        geninterrupt(0x67);
    }
}

/* Maps logical page `logical` of the handle into physical page `physical` (0 to 3) of
   the frame (44h); 1 if it worked. */
char far MapMemory_seg013_1D3C_C7(char physical, unsigned logical)
{
    _AH = 0x44;
    _AL = physical;
    _BX = logical;
    _DX = ems_handle;
    geninterrupt(0x67);
    if (_AH == 0) return 1;
    return 0;
}

/* Maps count (at most 4) consecutive logical pages from `logical` into consecutive
   physical pages from `physical` in one call (EMS 4.0 function 5000h, which takes the
   pairs in ems_page_map); 1 if it worked. */
unsigned char far seg013_1D3C_E4(unsigned physical, unsigned logical, int count)
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

/* Gives the handle an 8-byte name (EMS 4.0 function 5301h). */
void far seg013_1D3C_138(unsigned handle, char far *name)
{
    _BX = FP_SEG(name);
    _SI = FP_OFF(name);
    _DX = handle;
    _AX = 0x5301;
    geninterrupt(0x67);
}

/* The rest of the segment is not called from anywhere: allocating a one-page handle,
   mapping a handle's page 0 into the next of the four physical pages in turn (forgetting
   the pages TMPALLOC.C's callers think are mapped), and freeing a handle. Probably an
   older or unused interface for small separate allocations. */
int far seg013_1D3C_14D(unsigned *handle)
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

void far *far seg013_1D3C_16A(unsigned handle)
{
    crit_inpage = obj_inpage1 = tmap_inpage = 0xFF;
    dseg_67d6_120 = (dseg_67d6_120 + 1) & 3;
    _AH = 0x44;
    _AL = dseg_67d6_120;
    asm mov bx,0                            /* match: _BX = 0 gives xor bx,bx */
    _DX = handle;
    geninterrupt(0x67);
    if (_AH == 0) return MK_FP(ems_frame + (dseg_67d6_120 << 10), 0);
    return 0;
}

void far seg013_1D3C_1BD(unsigned handle)
{
    _DX = handle;
    _AX = 0x4500;
    geninterrupt(0x67);
}
