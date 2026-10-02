/* target: ovr165 */
/* opts: -mm -1 -G -O -Y -d */
/* DOS overlay ovr165: a function that switches to screen mode 16 (newscr, UWEDIT.C) and an
   empty one. Nothing in the sources or the extracted modules calls either by name, and FM
   Towns has no counterpart; probably leftovers of a removed feature, like STUBS.C.
   name: descriptive; the functions keep their IDA names. */

#include "sys.h"

void far ovr165_0(void) { newscr(16); }
void far ovr165_E(void) { }
