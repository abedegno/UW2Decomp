/* target: ovr113 */
/* opts: -mm -1 -G -O -Y -d */
/* Critter art pages: setting up the EMS cache of critter animation art, and lending
   critter pages to hold the conventional workspace. The whole of UW1's DOS overlay ovr113
   (UW2's ovr117), in original order.

   What it does in the game: critter pictures live in CRIT\CRxxPAGE.Nyy, critter file xx
   (in octal) split into pages yy. The game keeps pages in EMS, two 16K EMS pages each,
   starting at crit_fpage, with crit_nlpages of them (TMPALLOC.C's mem_setup gives the
   critters the EMS pages from 0x16 on). The 3D renderer loads a page when a frame needs
   it and evicts the least recently used one.

   Data (the 3D renderer's far data in seg051, assembly), indexed by cache index =
   critter file * 4 + page (0-127):
   - CmapCache[]: 0xFF not loaded, else an age (1 is the oldest).
   - CmaptoPg[]: the cache page holding the fragment, 0xFF none.
   - PgtoCmap[]: per cache page the fragment in it, 0xFE free, 0xFF no such page.
   - CmapFrm[32][3]: per critter file, the frame count up to the end of each of its
     first three pages (the two header bytes of CRxxPAGE.Nyy added; 0xA0 if missing).
   - seg051_C10F[]: 0x80 bytes of CRIT\ASSOC.ANM from offset 0x100 (UW2 reads AS.AN
     into grs_3dinf).

   UW1 against UW2: the tables have 128 entries, not 256, and the code reaches them
   through local far pointers; preload_cr builds CmapFrm from the page files' headers
   rather than reading PG.MP, and loads no animation sequences; there is no
   PreLoadCritPages; two new functions (unswap_ws, swap_ws_out) lend two critter pages
   to AUTOMAP.C to hold the conventional workspace (TMPALLOC.C's conv_ws_seg) and copy it
   back.

   Entry points: preload_cr (LOADGR.C, with the page tables), NightCleanCritPages
   (SKILLS.C after sleeping), punt_idle_crpage and ovr113_2A2 (pages for other use, the
   latter from seg014), swap_ws_out and unswap_ws (AUTOMAP.C).
   Name: UW2Decomp's (preloading critter art pages: preload_cr, NightCleanCritPages).
   Function names are UW2's (the FM Towns symbol table) where the routine is the same;
   UW1 has no symbol-bearing build. */
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "file.h"
#include "sys.h"
#include "view3d.h"

/* The name of one critter page file, digits filled in by preload_cr. */
/* name: chosen; static, as no other file refers to it. */
/* match: this file's _DATA, DS:1598..15BB, with its literals. */
static char crpage_name[] = "crit\\cr00page.n00";

/* Empty the critter page cache, and if load_pages, read ASSOC.ANM's table and the frame
   counts from every critter page file's header. Returns 0 if ASSOC.ANM cannot be opened,
   else 1. */
/* name: UW2's (FM Towns preload_cr), the routine being the same. symbols.tsv has the
   placeholder ovr113_0 (from LOADGR.C, before this file was matched), but the EXE's stub
   order puts this entry first (stub +20h), which ovr113_0's bssorder key (871) cannot give;
   preload_cr's (8) does. */
char far preload_cr(char load_pages)
{
    unsigned char far *assoc = seg051_C10F;
    unsigned char far *topg = CmaptoPg;
    unsigned char far *tocmap = PgtoCmap;
    unsigned char (far *frm)[3] = (unsigned char (far *)[3])CmapFrm;
    unsigned char far *cache = CmapCache;
    int i;
    int frames;
    int fd;
    unsigned char hdr[2];
    register int j;
    register FILE *fp;

    for (i = 0; i < crit_nlpages; i++) tocmap[i] = 0xFE;
    for (; i < 0x80; i++) tocmap[i] = 0xFF;
    for (i = 0; i < 0x80; i++) {
        cache[i] = 0xFF;
        topg[i] = 0xFF;
    }
    seg042_19B();
    if (!load_pages) return 1;
    if ((fd = open("crit\\assoc.anm", 0x8001)) < 0) {
        for (i = 0; i < 0x80; i++) assoc[i] = 0xFF;
        close(fd);
        return 0;
    }
    lseek(fd, 0x100L, 0);
    intoFarBuffer_ovr167_5DA(fd, assoc, 0x80);
    for (i = 0; i < 0x20; i++) {
        j = 0;
        crpage_name[7] = (i >> 3) + '0';
        crpage_name[8] = (i & 7) + '0';
        do {
            crpage_name[15] = j / 10 + '0';
            crpage_name[16] = j % 10 + '0';
            frames = 0xA0;
            if ((fp = fopen(crpage_name, "rb")) != 0) {
                if (fread(hdr, 1, 2, fp) == 2) frames = hdr[0] + hdr[1];
                fclose(fp);
            }
            if (j < 3) frm[i][j] = frames;
            j++;
        } while (frames < 0xA0 && j < 4);
    }
    close(fd);
    return 1;
}

/* After the player sleeps: drop every cached fragment that has aged right down (1, not
   used for a long while), freeing its page. */
/* name: UW2's (FM Towns NightCleanCritPages), the same loop; symbols.tsv's placeholder
   ovr113_1CE (from SKILLS.C) has key 775, out of stub order. */
void far NightCleanCritPages(void)
{
    unsigned char far *topg = CmaptoPg;
    unsigned char far *tocmap = PgtoCmap;
    unsigned char far *cache = CmapCache;
    register int i;
    register int page;

    for (i = 0; i < 0x80; i++) {
        if (cache[i] == 1) {
            cache[i] = 0xFF;
            page = topg[i];
            topg[i] = 0xFF;
            tocmap[page] = 0xFE;
        }
    }
    seg042_19B();
}

/* Evict the least recently used fragment (the lowest age in CmapCache) and return its
   now free page, or 0xFF if nothing is cached. */
/* name: UW2's (FM Towns punt_idle_crpage), the same eviction loop. */
int far punt_idle_crpage(void)
{
    unsigned char far *topg = CmaptoPg;
    unsigned char far *tocmap = PgtoCmap;
    unsigned char far *cache = CmapCache;
    register int i;
    register int frame;
    int lowest;
    int page;

    lowest = 0xFF;
    frame = 0xFF;
    for (i = 0; i < 0x80; i++) {
        if (cache[i] < lowest) {
            lowest = cache[i];
            frame = i;
        }
    }
    if (frame == 0xFF) return frame;
    cache[frame] = 0xFF;
    page = topg[frame];
    topg[frame] = 0xFF;
    tocmap[page] = 0xFE;
    return page;
}

/* Find count EMS pages (each the first of a pair, crit_fpage + page * 2) for other use,
   writing them to out: idle critter pages first (punt_idle_crpage), then any critter
   pages not already chosen. Returns count, or -1 if there are not enough. */
/* name: the listing's (UW2Decomp keeps IDA's ovr117_2FD for UW2's copy). */
int far ovr113_2A2(int count, int16 *out)
{
    int page;
    int cr;
    register int i;
    register int j;
    for (i = 0; i < count; i++) {
        page = punt_idle_crpage();
        if (page == 0xFF) break;
        out[i] = crit_fpage + (page << 1);
    }
    if (i == count) return i;
    for (cr = 0; cr < crit_nlpages; cr++) {
        unsigned char found = 1;
        for (j = 0; j < i; j++)
            if (crit_fpage + (cr << 1) == out[j]) found = 0;
        if (found) out[i++] = crit_fpage + (cr << 1);
        if (i == count) return i;
    }
    return -1;
}

/* Take two critter pages (ovr113_2A2) and copy the conventional workspace into them
   through the page frame. Returns the handle unswap_ws takes, the two pages in its high
   and low bytes; if there are not two pages, empties the cache (preload_cr) and returns
   -1. */
/* name: chosen for the stub order (key 475, between NightCleanCritPages' 310 and
   ovr113_2A2's 759); no original name. */
int far swap_ws_out(void)
{
    int pages[2];

    ovr113_2A2(2, pages);
    if (pages[0] == 0xFF || pages[1] == 0xFF) {
        preload_cr(0);
        return -1;
    }
    seg012_BB(0, pages[0], 2);
    seg012_BB(2, pages[1], 2);
    movedata(conv_ws_seg, 0, ems_frame, 0, 0xFFFF);
    seg042_19B();
    return (pages[0] << 8) + pages[1];
}

/* Lend the conventional workspace's contents back: map the two EMS pages of handle
   (first page in the high byte, second in the low) and copy 0xFFFF bytes from the page
   frame to conv_ws_seg. */
/* name: chosen for the stub order (tools/bssorder.py key 109, between preload_cr's 8 and
   punt_idle_crpage's 168); no original name. */
void far unswap_ws(unsigned handle)
{
    register int first;
    register int second;

    first = handle >> 8;
    second = handle & 0xFF;
    seg012_BB(0, first, 2);
    seg012_BB(2, second, 2);
    movedata(ems_frame, 0, conv_ws_seg, 0, 0xFFFF);
    seg042_19B();
}
