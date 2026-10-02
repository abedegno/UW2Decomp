/* target: ovr117 */
/* opts: -mm -1 -G -O -Y -d */
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include "file.h"
#include "sys.h"
#include "view3d.h"

void far seg013_1D3C_E4(int a, int b, int c);
void far bltfromdrive(char *name, void far *buf, unsigned n);

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

/* IDA NightCleanCritPages: the FM Towns neighbour NightCleanCritPages has this loop. */
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

/* IDA punt_idle_crpage: same cache eviction loop as FM Towns punt_idle_crpage. */
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

/* FM Towns grfx_init at this position has different behaviour, so retain IDA name. */
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
