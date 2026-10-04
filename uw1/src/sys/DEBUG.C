/* target: ovr106 */
/* opts: -mm -1 -G -O -Y -d */
/* UW1's debug module, DOS overlay ovr106, whole and in original order: init_debug, which
   init_world calls at start-up (after init_mem and init_input, as in UW2), the debug
   printer, empty in this build, and another empty function. UW2's DEBUG.C (overlay
   ovr109) has init_debug and the empty function but no printer.

   init_debug binds one key through _input_addkey (INPUT.C): Alt with key code 83h (0x283,
   KEY_ALT | KEY_F4) to ovr127_0, which raises the COM1 interrupt, called with the key's
   argument 0. UW1 has no joystick, so no Ctrl+J calibration key (UW2 binds both).

   dprintf takes a printf format and its arguments (INPUT.C's "bad kbd hndl %d\n",
   LOADGR.C's "gr no parse data %s\n", OBJECTS.C's lists) and does nothing: the printing
   was compiled out, the format strings staying in the callers' data.

   name: descriptive; init_debug is the FM Towns name (UW2, same position and job).
   dprintf is symbols.tsv's name for the printer (given by INPUT.C, descriptive).
   match: the overlay's stub table orders the three entries ovr106_0, ovr106_1B, ovr106_20,
   which needs ascending bssorder.py keys; init_debug's key is 145 and dprintf's 132,
   so dprintf cannot be the original name if init_debug is: the printer needs a name
   whose key lies between 145 and that of the third function's name (ovr106_20, 575;
   ovr106_1B, 567, would do). verify.py does not check this order; the link will. */

#include "sys.h"
#include "ui.h"

/* Binds Alt+F4, in every screen mode, to ovr127_0 (OVR127.C). */
void far init_debug(void)
{
    _input_addkey(KEY_ALT | KEY_F4, 0, 0xFF, (InputFn)ovr127_0);
}

/* The debug printer, empty in this build: its callers' format strings remain. */
void far dprintf(char *fmt, ...)
{
}

/* name: the listing's (UW2's counterpart, ovr109_31, is named by its offset too). */
void far ovr106_20(void)
{
}
