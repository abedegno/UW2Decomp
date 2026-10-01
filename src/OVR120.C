/* target: ovr120 */
/* opts: -mm -1 -G -O -Y -d */

struct Object { unsigned id; };
extern struct Object far *ActiveObj;
/* This file's _BSS, DS:6946..6A75, by name: Missile 621, Weapons 647, Armor 761 (ovr119's
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

/* FM Towns: hack_class_data, the class 0 lookup named by get_class_data. */
char * far hack_class_data(void)
{
    register int subclass;
    register int minor;
    minor = (ActiveObj->id & 0x30) >> 4;
    subclass = ActiveObj->id & 0x0F;
    switch (minor) {
    case 1: return Missile + subclass * 3;
    case 0: return Weapons + subclass * 8;
    case 3: subclass += 16;
    case 2: return Armor + subclass * 4;
    default: return 0;
    }
}
