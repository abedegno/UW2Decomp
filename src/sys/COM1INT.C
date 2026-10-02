/* target: ovr132 */
/* opts: -mm -1 -G -O -Y -d */
/* DOS overlay ovr132: one function that raises int 0Ch, the hardware interrupt of IRQ 4
   (COM1), in software, so whatever COM1 handler is installed runs once. init_debug
   (DEBUG.C) binds it to a key, so it was probably a debugging hook (for a serial debugger
   or remote link; nothing else in UW2 touches the serial port). DOS only.
   name: descriptive; the function keeps its IDA name. */

#include <dos.h>

void far Interupt4_COM1_ovr132_0(void)
{
    geninterrupt(0x0C);
}
