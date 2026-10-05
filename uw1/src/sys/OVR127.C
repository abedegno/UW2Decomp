/* target: ovr127 */
/* opts: -mm -1 -G -O -Y -d */
/* DOS overlay ovr127: one function that raises int 0Ch, the hardware interrupt of IRQ 4
   (COM1), in software, so whatever COM1 handler is installed runs once. Byte for byte
   UW2's Interupt4_COM1_ovr132_0 (UW2Decomp src/sys/COM1INT.C), probably a debugging
   hook. DOS only.
   File name: provisional (the original name is unknown; named after the segment; UW2's
   copy is COM1INT.C).
   name: the listing name (UW2 calls its copy by its IDA name, Interupt4_COM1_ovr132_0). */

#include <dos.h>

/* Raises int 0Ch, the COM1 interrupt (IRQ 4), in software; DEBUG.C's Alt+F4 calls it.
   What handler, if any, the development setup had on it is not known. */
void far ovr127_0(void)
{
    geninterrupt(0x0C);
}
