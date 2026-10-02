/* target: ovr162 */
/* opts: -mm -1 -G -O -Y -d */

#include "event.h"

void far fwrite(void *address, int size, int count, int fd);

void far ovr162_0(int fd)
{
    fwrite(Triggers, 1, 16, fd);
}
