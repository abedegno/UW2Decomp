/* target: ovr120 */
/* opts: -mm -1 -G -O -Y -d */
/* Class data for major class 0 (MAJOR_HACK: weapons, missiles and armour), read from the
   first three tables of DATA\OBJECTS.DAT (UW-Formats 6.3): Weapons, 8 bytes per melee
   weapon (slash, bash and stab damage, skill, durability), Missile, 3 bytes per missile or
   missile weapon, and Armor, 4 bytes for each of 32 armour items (protection, durability,
   category). init_objects (OBJCLASS.C) calls hack_init; get_class_data calls
   hack_class_data, and combat and the inventory read the returned record.
   Name: inferred (the class prefix, MAJOR_HACK). */

#include "object.h"

/* match: this file's _BSS, DS:6946..6A75, by name: Missile 621, Weapons 647, Armor 761 (ovr119's
   run ends at 736 and ovr121's BagSaveHandles, which only ovr121 uses, follows). */
char Missile[0x30], Weapons[0x80], Armor[0x80];
void far fread(void *address, int size, int count, int fd);

/* FM Towns: hack_init, the class 0 loader named by init_objects. */
void far hack_init(int fd)
{
    fread(Weapons, 8, 16, fd);
    fread(Missile, 3, 16, fd);
    fread(Armor, 4, 32, fd);
}

/* FM Towns: hack_class_data, the class 0 lookup named by get_class_data. Returns the
   ActiveObj's record by its minor class: 0 Weapons, 1 Missile, 2 and 3 Armor (class 3,
   crowns, rings and shields, is the second half of the table). */
char * far hack_class_data(void)
{
    register int subclass;
    register int minor;
    minor = OBJ_MINOR(ActiveObj);
    subclass = ActiveObj->id & ID_INCLASS;
    switch (minor) {
    case 1: return Missile + subclass * 3;
    case 0: return Weapons + subclass * 8;
    case 3: subclass += 16;
    case 2: return Armor + subclass * 4;
    default: return 0;
    }
}
