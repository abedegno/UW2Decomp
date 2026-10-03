/* target: ovr090 */
/* opts: -mm -1 -G -O -Y -d */
/* DOS overlay ovr090: seven tiny far functions, six empty and one (ovr090_5) that switches
   to screen mode 16 through newscr. Byte for byte the same as UW2's ovr092
   (UW2Decomp src/sys/STUBS.C), so these are the same compiled-out debugging or editor
   hooks. DOS only.
   File name: provisional (the original name is unknown; named after the segment).
   name: the functions keep UW2's STUBS.C names, with this segment's offsets. */

#include "sys.h"

void far ReturnFar_ovr090_0(void) { }
void far ovr090_5(void) { newscr(16); }
void far ovr090_13(void) { }
void far Nop_ovr090_18(void) { }
void far ovr090_1D(void) { }
void far DoNothing_ovr090_22(void) { }
void far ReturnFar_ovr090_27(void) { }
