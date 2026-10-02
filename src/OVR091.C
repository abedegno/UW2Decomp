/* target: ovr091 */
/* opts: -mm -1 -G -O -Y -d */

#include "object.h"

extern struct AnimClass animclassd[];
void far fread(void *address, int size, int count, int fd);

/* FM Towns: animobj_load. */
void far animobj_load(int fd)
{
    fread(animclassd, 4, 16, fd);
}

/* FM Towns: animobj_class_data. */
char * far animobj_class_data(void)
{
    return (char *)&animclassd[ActiveObj->id & ID_INCLASS];
}
