/* target: ovr132 */
/* opts: -mm -1 -G -O -Y -d */

#include <dos.h>

void far Interupt4_COM1_ovr132_0(void)
{
    geninterrupt(0x0C);
}
