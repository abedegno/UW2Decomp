/* target: ovr092 */
/* opts: -mm -1 -G -O -Y -d */
/* DOS overlay ovr092: seven tiny far functions, six empty and one (ovr092_5) that switches to
   screen mode 16 through newscr (UWEDIT.C). Nothing in the sources or the extracted
   modules calls any of them by name; they are probably the remains of debugging or editor
   features compiled out of the shipped game (UW2.EXE was linked as uwedit.exe). DOS only.
   name: descriptive (seven stubs). */

#include "sys.h"

/* name: no FM Towns counterparts. DoNothing_ovr092_22, ReturnFar_ovr092_0 and Nop_ovr092_18 (IDA's
   ovr092_22, ovr092_0, ovr092_18) are provisional names chosen so that their tools/bssorder.py
   keys put them in the EXE's overlay stub order. */

void far ReturnFar_ovr092_0(void) { }
void far ovr092_5(void) { newscr(16); }
void far ovr092_13(void) { }
void far Nop_ovr092_18(void) { }
void far ovr092_1D(void) { }
void far DoNothing_ovr092_22(void) { }
void far ReturnFar_ovr092_27(void) { }
