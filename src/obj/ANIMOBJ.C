/* target: ovr091 */
/* opts: -mm -1 -G -O -Y -d */
/* Animated object class data (major class 7, items 0x1C0-0x1CF): the loader and lookup for
   animclassd, one struct AnimClass of 4 bytes per animation class, read from the last table
   of DATA\OBJECTS.DAT (UW-Formats 6.3, "animation object table": flags, start frame in
   ANIMO.GR, frame count). init_objects (OBJCLASS.C) calls animobj_load while reading the
   file, and get_class_data calls animobj_class_data for an object of major class 7.
   animclassd is defined in EFFECT.C, which reads it each frame to run the animations.
   Name: inferred (the class prefix, after animobj_load and animobj_class_data). */

#include <stdio.h>
#include "object.h"


/* FM Towns: animobj_load. */
void far animobj_load(FILE *fd)
{
    fread(animclassd, 4, 16, fd);
}

/* FM Towns: animobj_class_data. */
char * far animobj_class_data(void)
{
    return (char *)&animclassd[ActiveObj->id & ID_INCLASS];
}
