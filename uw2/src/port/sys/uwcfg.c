/* uwcfg.c: DATA\UW.CFG in the home directory, the file the game reads its sound cards from, and
   the port's home directory; used by main.c and the settings screen's table (settab.c). Kept out
   of main.c so that the port's files link without it (tools/fuzzasm.py's host replaces main.c). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port.h"
#include "plat.h"

/* --sound CARD[,SPEECH]: DATA\UW.CFG in the home directory, the file UWSOUND.EXE writes: the
   music card and the speech card, each with its IRQ, port (hex) and DMA, for the cards'
   factory settings (SOUND.C, seg016_1E73_2FCB, reads it; the port's drivers take any) */
int write_uw_cfg(const char *spec)
{
    char path[1200];
    int music = atoi(spec), speech = strchr(spec, ',') ? atoi(strchr(spec, ',') + 1) : 0;
    FILE *f;
    if (plat_resolve("DATA\\UW.CFG", PLAT_CREATE, path, sizeof path) || !(f = fopen(path, "wb"))) {
        fprintf(stderr, "uw2port: cannot write DATA\\UW.CFG in the home directory\n");
        return 1;
    }
    fprintf(f, "%d %s sound\r\n%d %s speech\r\n", music,
            music == 0 ? "-1 -1 -1" : music == 2 ? "-1 388 -1" : music == 5 ? "-1 330 -1" : "7 220 1",
            speech, speech == 0 ? "-1 -1 -1" : "7 220 1");
    fclose(f);
    return 0;
}

/* The home directory, for the drop hook, which runs on the backend's thread, and the settings
   screen's rows (settab.c). */
const char *port_home;

/* The music and speech cards DATA\UW.CFG in the home directory names; -1 for each when it is
   missing or unreadable. */
void read_uw_cfg(int *music, int *speech)
{
    char path[1200];
    FILE *f;
    *music = *speech = -1;
    if (plat_resolve("DATA\\UW.CFG", PLAT_CREATE, path, sizeof path) || !(f = fopen(path, "rb"))) return;
    if (fscanf(f, "%d %*s %*s %*s sound %d", music, speech) != 2) *speech = -1;
    fclose(f);
}
