/* web/pack.sh's ROM choice: the control and PCM images the port would play from in ROMS_DIR,
   chosen by the runtime's own mt32roms_pick (exhume/runtime/port/sound/mt32roms.c: recognised by
   content through libmt32emu, the CM-32L pair before the CM-32LN's before the MT-32's, the newest
   control ROM, whole images before halves), so that only those are packed. Built by pack.sh with
   emcc against the game's web libmt32emu and run under Node.js.
   usage: romsel ROMS_DIR -- prints the chosen files' paths, one a line (two for an image in
   halves); exit status 1, with a message, when the folder holds no pair. */
#include <stdio.h>
#include "sound/mt32roms.h"

const char *plat_base_dir(void) { return ""; }      /* mt32roms.c's search, not used here */

int main(int argc, char **argv)
{
    struct mt32roms_set s;
    int i;
    if (argc != 2) { fprintf(stderr, "usage: romsel ROMS_DIR\n"); return 2; }
    if (!mt32roms_pick(argv[1], &s)) {
        fprintf(stderr, "romsel: no MT-32 or CM-32L ROM pair in %s (a control ROM and its PCM ROM)\n", argv[1]);
        return 1;
    }
    fprintf(stderr, "romsel: %s and %s\n", s.ctrl_id, s.pcm_id);
    for (i = 0; i < 2; i++) if (s.ctrl[i][0]) printf("%s\n", s.ctrl[i]);
    for (i = 0; i < 2; i++) if (s.pcm[i][0]) printf("%s\n", s.pcm[i]);
    return 0;
}
