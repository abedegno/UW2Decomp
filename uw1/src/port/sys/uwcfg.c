/* uwcfg.c: DATA\UW.CFG in the home directory, the file the game reads its sound cards from, and
   the port's home directory; used by main.c and the settings screen's table (settab.c). Kept out
   of main.c so that the port's files link without it (tools/fuzzasm.py's host replaces main.c). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port.h"
#include "plat.h"

/* --sound MUSIC[,SPEECH]: DATA\UW.CFG in the home directory, the file UW1's install program
   writes: the music card and the speech card, each with its IRQ, port (hex) and DMA, for the
   cards' factory settings, and the cutscene line (SOUND.C's seg014_1DC5_1D0D reads the first
   two lines, UWEDIT.C skips the third). Music cards are SOUND.C's music_drivers: 1 PC speaker,
   2 Ad Lib, 3 Sound Blaster, 4 Sound Blaster Pro, 5 Pro Audio Spectrum, 6 MT-32 on an MPU-401;
   speech cards its speech_drivers: 1 Sound Blaster, 2 Sound Blaster Pro, 3 Pro Audio Spectrum. */
int write_uw_cfg(const char *spec)
{
    char path[1200];
    int music = atoi(spec), speech = strchr(spec, ',') ? atoi(strchr(spec, ',') + 1) : 0;
    FILE *f;
    if (plat_resolve("DATA\\UW.CFG", PLAT_CREATE, path, sizeof path) || !(f = fopen(path, "wb"))) {
        fprintf(stderr, "uw1port: cannot write DATA\\UW.CFG in the home directory\n");
        return 1;
    }
    fprintf(f, "%d %s sound\r\n%d %s speech\r\n0 cuts\r\n", music,
            music == 0 || music == 1 ? "-1 -1 -1" : music == 2 ? "-1 388 -1" : music == 6 ? "2 330 -1" : "7 220 1",
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
