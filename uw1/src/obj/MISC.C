/* target: ovr125 */
/* opts: -mm -1 -G -O -Y -d */
/* Class data for major class 2 (MAJOR_MISC: containers, lights and wands, treasure, food),
   read from DATA\OBJECTS.DAT after the creature table (UW-Formats 6.3): Containers, 3 bytes
   each (capacity in tenths of a stone, the kind of object accepted, slots), Lights, 2 bytes
   each (brightness, duration), and Food, one byte each (probably the nourishment; the
   format document lists the table as unknown). init_objects (OBJCLASS.C) calls misc_init;
   get_class_data calls misc_class_data. The whole of DOS overlay ovr125, the same code as
   UW2's ovr130 (UW2Decomp src/obj/MISC.C). Function names are UW2's (the FM Towns
   originals); UW1 has no symbols of its own.
   Name: UW2Decomp's (the class prefix, after misc_init and misc_class_data). */

#include <stdio.h>
#include "object.h"

/* match: this file's _BSS, DS:5B0A..5B69 in UW1 (UW2 DS:6B10..6B6F), laid out by name:
   Containers 339, Lights 620, Food 958. */
struct Container Containers[16];
struct Light Lights[16];
char Food[0x10];

/* UW2 FM Towns: misc_init, the class 2 loader named by init_objects. */
void far misc_init(FILE *fd)
{
    fread(Containers, 3, 16, fd);
    fread(Lights, 2, 16, fd);
    fread(Food, 1, 16, fd);
}

/* UW2 FM Towns: misc_class_data, the class 2 lookup named by get_class_data. Minor class 0
   is a container, 1 a light or wand (sixteen 2-byte entries, lights then wands), 2
   treasure, which has no data (0), and 3 food. */
char * far misc_class_data(void)
{
    register int subclass;
    register int minor;
    minor = OBJ_MINOR(ActiveObj);
    subclass = OBJ_INCLASS(ActiveObj);
    switch (minor) {
    case 0: return (char *)&Containers[subclass];
    case 1: return (char *)&Lights[subclass];
    case 2: return 0;
    default: return Food + subclass;
    }
}
