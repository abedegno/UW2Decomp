/* ailgame.h: UW1's side of the sound library, Exhume's runtime/port/sound/ail.c, which
   includes this file first (Exhume's docs/port.md, "Sound"): the portability layer, then UW1's
   own declarations of the AIL API and its structs (src/include/sound.h), so the C API is
   checked against the game's view of it. UW1's AIL is an earlier release than UW2's (version
   word 0CAh; docs/NOTES.md). No FM extension yet: whether UW1's FM drivers were built with
   Origin's TVFX is still to be read from SBFM.ADV, and without one a TVFX timbre plays nothing
   (the runtime's sound/yamaha.h). */
#include "compat.h"
#include "sound.h"
