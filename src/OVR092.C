/* target: ovr092 */
/* opts: -mm -1 -G -O -Y -d */

void far newscr(int screen);

/* No FM Towns counterparts. DoNothing_ovr092_22, ReturnFar_ovr092_0 and Nop_ovr092_18 (IDA's
   ovr092_22, ovr092_0, ovr092_18) are provisional names chosen so that their tools/bssorder.py
   keys put them in the EXE's overlay stub order. */

void far ReturnFar_ovr092_0(void) { }
void far ovr092_5(void) { newscr(16); }
void far ovr092_13(void) { }
void far Nop_ovr092_18(void) { }
void far ovr092_1D(void) { }
void far DoNothing_ovr092_22(void) { }
void far ReturnFar_ovr092_27(void) { }
