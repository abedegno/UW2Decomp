/* target: ovr117 */
/* opts: -mm -1 -G -O -Y -d */
/* Critter art pages: setting up the EMS cache of critter animation art and preloading
   it. The whole of DOS overlay ovr117.

   What it does in the game: critter pictures live in CRIT\CRxx.yy, critter file xx (in
   octal) split into fragments yy (UW-Formats 3.6.2). The game keeps fragments in EMS,
   two 16K EMS pages each, starting at crit_fpage, with crit_nlpages of them (TMPALLOC.C's
   mem_setup gives the critters the EMS pages from 0x21 on). The 3D renderer
   (PGCACHE.ASM's load_cpg and unload_cpg) loads a fragment when a frame needs it and
   evicts the least recently used one; its update_cache ages the cache every frame.

   Data (owned by TMAPOPS.ASM, declared in view3d.h), indexed by cache index = critter
   file * 8 + fragment (0-255):
   - CmapCache[]: 0xFF not loaded, else an age that update_cache counts down to 1 every
     frame (a use presumably sets it high again); preloaded fragments start at 0xF0.
   - CmaptoPg[]: the cache page holding the fragment, 0xFF none.
   - PgtoCmap[]: per cache page the fragment in it, 0xFE free, 0xFF no such page.
   - CmapFrm[]: CRIT\PG.MP, the last frame in each fragment.
   preload_cr also loads CRIT\AS.AN (which critter file and palette each critter type
   uses) into grs_3dinf and CRIT\CR.AN (the animation sequences, 32 critter files of 8
   actions by 8 view angles) into the fourth EMS frame page, where AI.C's seqptr reads
   it.

   Entry points: preload_cr (LOADGR.C at start-up, with PG.MP; MAINMENU.C for a new
   game, without), PreLoadCritPages (a new game), NightCleanCritPages (SKILLS.C after
   sleeping), punt_idle_crpage and ovr117_2FD (pages for other use).
   Name: descriptive (preloading critter art pages: preload_cr, PreLoadCritPages). */
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include "file.h"
#include "sys.h"
#include "view3d.h"

void far seg013_1D3C_E4(int a, int b, int c);
void far bltfromdrive(char *name, void far *buf, unsigned n);

/* Empty the critter page cache and load CR.AN and AS.AN, and PG.MP if load_map. Returns
   0 if a file cannot be opened, else 1. */
unsigned char far preload_cr(unsigned char load_map)
{
    register int i;
    register int fd;
    for (i = 0; i < crit_nlpages; i++) PgtoCmap[i] = 0xFE;
    for (; i < 0x100; i++) PgtoCmap[i] = 0xFF;
    for (i = 0; i < 0x100; i++) {
        CmapCache[i] = 0xFF;
        CmaptoPg[i] = 0xFF;
    }
    if ((fd = open("CRIT\\as.an", 0x8001)) < 0) return 0;
    intoFarBuffer_ovr167_5DA(fd, grs_3dinf, 0x80);
    close(fd);
    if ((fd = open("CRIT\\cr.an", 0x8001)) < 0) return 0;
    map_crit_pages();
    intoFarBuffer_ovr167_5DA(fd, MK_FP(EmsBuff + 0xC00, 0), 0x4000);
    close(fd);
    seg042_35ED_12B();
    if (!load_map) return 1;
    if ((fd = open("CRIT\\pg.mp", 0x8001)) < 0) return 0;
    intoFarBuffer_ovr167_5DA(fd, CmapFrm, 0x100);
    close(fd);
    return 1;
}

/* After the player sleeps: drop every cached fragment that has aged right down (1, not
   used for a long while), freeing its page. */
/* name: IDA NightCleanCritPages: the FM Towns neighbour NightCleanCritPages has this
   loop. */
void far NightCleanCritPages(void)
{
    register int i;
    register int page;
    for (i = 0; i < 0x100; i++) {
        if (CmapCache[i] == 1) {
            CmapCache[i] = 0xFF;
            page = CmaptoPg[i];
            CmaptoPg[i] = 0xFF;
            PgtoCmap[page] = 0xFE;
        }
    }
    seg042_35ED_12B();
}

/* At the start of a new game, when at least 8 cache pages exist: load fragments 0 and 1
   of critter files 21 to 24 (CR25, CR26, CR27 and CR30) into the first 8 pages, marked
   0xF0. By CRIT\AS.AN those files hold mostly the human figures (critter types 0x73 to
   0x7A among others), probably because a new game starts among the castle's people
   (inferred). */
void far PreLoadCritPages(void)
{
    char name[80];
    int j;
    int cr;
    int page;
    register int i;
    register int frame;
    page = 0;
    strcpy(name, "CRIT\\cr??.0?");
    if (crit_nlpages >= 8) {
        frame = 0xA8;
        for (i = 0; i < 4; i++, frame += 8) {
            j = 0;
            cr = frame;
            for (; j < 2; page++, j++, cr++) {
                CmapCache[cr] = 0xF0;
                CmaptoPg[cr] = page;
                PgtoCmap[page] = cr;
                seg013_1D3C_E4(0, crit_fpage + (page << 1), 2);
                name[strlen(name) - 1] = j + '0';
                name[strlen(name) - 5] = (cr >> 6) + '0';
                name[strlen(name) - 4] = ((cr >> 3) & 7) + '0';
                bltfromdrive(name, MK_FP(EmsBuff, 0), 0x7FFF);
            }
        }
    }
}

/* Evict the least recently used fragment (the lowest age in CmapCache) and return its
   now free page, or 0xFF if nothing is cached. */
/* name: IDA punt_idle_crpage: same cache eviction loop as FM Towns punt_idle_crpage. */
int far punt_idle_crpage(void)
{
    register int i;
    register int frame;
    int lowest = 0xFF;
    int page;
    frame = 0xFF;
    for (i = 0; i < 0x100; i++) {
        if (CmapCache[i] < lowest) {
            lowest = CmapCache[i];
            frame = i;
        }
    }
    if (frame == 0xFF) return frame;
    CmapCache[frame] = 0xFF;
    page = CmaptoPg[frame];
    CmaptoPg[frame] = 0xFF;
    PgtoCmap[page] = 0xFE;
    return page;
}

/* Find count EMS pages (each the first of a pair, crit_fpage + page * 2) for other use,
   writing them to out: idle critter pages first (punt_idle_crpage), then any critter
   pages not already chosen. Returns count, or -1 if there are not enough. No caller was
   found: in the IDA listing only its overlay stub jumps to it. */
/* name: FM Towns grfx_init at this position has different behaviour, so retain IDA
   name. */
int far ovr117_2FD(int count, int *out)
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
