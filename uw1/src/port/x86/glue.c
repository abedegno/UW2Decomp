/* glue.c: replaces nothing. Where the translated modules (x86/asmrt.h) reach code that is
   hand-written C in the port: seg019's entries the renderer far-calls, seg003's routines that
   are hand-written, and the DOS and EMS interrupts. Each does the routine's work on the
   emulated registers and returns as the routine did. None yet: no module is translated. */
#include "x86/asmrt.h"

const struct asm_glue asm_glues[1];
const int asm_nglues = 0;
