/* ailgame.h: UW2's side of the sound library, Exhume's runtime/port/sound/ail.c, which
   includes this file first (docs/PORT.md, "The runtime"): the portability layer, then UW2's
   own declarations of the AIL API and its structs (src/include/sound.h: struct DrvrDesc,
   struct SoundBuff, the AIL_ functions as SOUND.C and CUTS.C call them), so the C API is
   checked against the game's view of it; and the FM drivers' time-variant effects for
   yamaha.c (the runtime's sound/yamaha.h, struct AilFmExt): uw2_tvfx, in sound/tvfx.c. */
#include "compat.h"
#include "sound.h"
#define AIL_FM_EXT uw2_tvfx
