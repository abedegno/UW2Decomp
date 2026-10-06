/* UW2's enhancements: changes the original game does not have, each off unless the player
   turns it on (docs/ENHANCEMENTS.md). In the order of portgame.h's ENH_ enum. */
#include "portgame.h"
#include "sys/enhance.h"

#define UH "UltimaHacks (John Glassmyer, MIT)"

const struct enhance_flag enhance_table[ENH_COUNT] = {
    { "skip-intro", "Starts at the main menu, without the title or the introduction", ENH_TIMING, UH, NULL },
    { "wrap-menu", "The start menu and the saved games wrap from the last item to the first", ENH_PRESENTATION, UH, NULL },
    { "fast-panels", "The right-hand panel slides twice as fast", ENH_TIMING, UH, NULL },
    { "free-heading", "Sliding along a wall no longer turns your view", ENH_GAMEPLAY, UH, NULL },
    { "subtitles", "Cutscenes show their text while the speech plays", ENH_PRESENTATION, UH, NULL },
    { "skill-messages", "Says which skill a skill point or a trainer raised, and to what", ENH_PRESENTATION, UH, NULL },
    { "perspective", "Floors and ceilings stay straight when the view pitches (walls keep the original's columns)", ENH_PRESENTATION, "uwpatch (SiENcE, MIT)", NULL },
    { "full-sprites", "Creatures and objects keep their size when the view pitches", ENH_PRESENTATION, UH, NULL },
};
