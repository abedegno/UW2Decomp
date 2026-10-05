/* target: ovr116 */
/* opts: -mm -1 -G -O -Y -d */
/* Class data for major class 0 (MAJOR_HACK: weapons, missiles and armour), read from the
   first three tables of DATA\OBJECTS.DAT (UW-Formats 6.3): Weapons, 8 bytes per melee
   weapon (slash, bash and stab damage, skill, durability), Missile, 3 bytes per missile or
   missile weapon, and Armor, 4 bytes for each of 32 armour items (protection, durability,
   category). init_objects (OBJCLASS.C) calls hack_init; get_class_data calls
   hack_class_data, and combat and the inventory read the returned record. The whole of
   DOS overlay ovr116, the same code as UW2's ovr120 (UW2Decomp src/obj/HACK.C).
   Function names are UW2's (the FM Towns originals); UW1 has no symbols of its own.
   Name: UW2Decomp's (the class prefix, MAJOR_HACK). */

#include <stdio.h>
#include "combat.h"
#include "object.h"

/* match: this file's _BSS, DS:5942..5A71 in UW1 (UW2 DS:6946..6A75), laid out by name:
   Missile 621, Weapons 647, Armor 761. */
struct MissileInfo Missile[16];
struct Weapon Weapons[16];
struct Armour Armor[32];

/* UW2 FM Towns: hack_init, the class 0 loader named by init_objects. */
void far hack_init(FILE *fd)
{
    fread(Weapons, 8, 16, fd);
    fread(Missile, 3, 16, fd);
    fread(Armor, 4, 32, fd);
}

/* UW2 FM Towns: hack_class_data, the class 0 lookup named by get_class_data. Returns the
   ActiveObj's record by its minor class: 0 Weapons, 1 Missile, 2 and 3 Armor (class 3,
   crowns, rings and shields, is the second half of the table). */
char * far hack_class_data(void)
{
    register int subclass;
    register int minor;
    minor = OBJ_MINOR(ActiveObj);
    subclass = OBJ_INCLASS(ActiveObj);
    switch (minor) {
    case 1: return (char *)&Missile[subclass];
    case 0: return (char *)&Weapons[subclass];
    case 3: subclass += 16;
    case 2: return (char *)&Armor[subclass];
    default: return 0;
    }
}
