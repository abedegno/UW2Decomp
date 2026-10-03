/* target: ovr152 */
/* opts: -mm -1 -G -O -Y -d */
/* DOS overlay ovr152: three tiny far functions, two empty and one (ovr152_5) that
   switches to screen mode 16 through newscr. Each is byte for byte one of UW2's ovr092
   (UW2Decomp src/sys/STUBS.C); UW1 has the full set of seven again in ovr090. Nothing in
   UW.EXE calls them (only their overlay stubs refer to them). DOS only.
   File name: provisional (the original name is unknown; named after the segment).
   name: the listing names, whose tools/bssorder.py keys reproduce the EXE's overlay stub
   order (13 0 5); UW2's STUBS.C names would not. */

#include "sys.h"

void far ovr152_0(void) { }
void far ovr152_5(void) { newscr(16); }
void far ovr152_13(void) { }
