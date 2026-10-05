/* ailgame.h: UW1's side of the sound library, Exhume's runtime/port/sound/ail.c, which
   includes this file first (Exhume's docs/port.md, "Sound"): the portability layer, then UW1's
   own declarations of the AIL API and its structs (src/include/sound.h), so the C API is
   checked against the game's view of it. UW1's AIL is an earlier release than UW2's (version
   word 0CAh; docs/NOTES.md). Its drivers are AIL's 1991 release, which the runtime recognises
   from the driver file (Synth.rev1991), built with Origin's TVFX like UW2's: uw1_tvfx, in
   sound/tvfx.c, is the FM drivers' extension (the runtime's sound/yamaha.h, struct AilFmExt). */
#include "compat.h"
#include "sound.h"
#define AIL_FM_EXT uw1_tvfx
/* Under replay, a digital transfer ends at the moment a recorded read shows DOS's ended, from
   this many microseconds before its nominal end (the runtime's default is 5000, from UW2's
   sessions): DOS's side is js-dos's Sound Blaster, which raises the interrupt early by its
   mixer's lead, and in UW1's sound session the intro's speech buffer ended 6.4 ms early. */
#define AIL_SB_WIN_EARLY 8000u
