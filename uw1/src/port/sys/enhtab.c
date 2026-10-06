/* UW1's enhancements: changes the original game does not have, each off unless the player
   turns it on (docs/ENHANCEMENTS.md). In the order of portgame.h's ENH_ enum. */
#include "portgame.h"
#include "sys/enhance.h"

const struct enhance_flag enhance_table[ENH_COUNT] = {
    { "skip-intro", "Starts at the main menu, without the title or the introduction", ENH_TIMING,
      "UltimaHacks (John Glassmyer, MIT)", NULL },
};
