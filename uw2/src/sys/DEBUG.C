/* target: ovr109 */
/* opts: -mm -1 -G -O -Y -d */
/* DOS overlay ovr109: init_debug, which init_world (UWEDIT.C) calls at start-up, and an
   empty function. FM Towns has init_debug too, but empty; the DOS one binds two keys
   through _input_addkey (INPUT.C): Ctrl+J (0x16A, KEY_CTRL | 'j') to the joystick
   calibration in JOYSTICK.C, and Alt with key code 83h (0x283) to the COM1 interrupt in
   COM1INT.C. Both handlers are called with the key's argument, here 0.
   name: descriptive; init_debug is the FM Towns name, and System Shock defines its
   init_debug in INIT.C, its whole start-up, which here is UWEDIT.C. */

#include "sys.h"
#include "ui.h"

void far init_debug(void)
{
    _input_addkey(KEY_CTRL | 'j', 0, 0xFF, (InputFn)JoyStickCalibration_seg011_1B8);
    _input_addkey(KEY_ALT | KEY_F4, 0, 0xFF, (InputFn)Interupt4_COM1_ovr132_0);
}

void far ovr109_31(void)
{
}
