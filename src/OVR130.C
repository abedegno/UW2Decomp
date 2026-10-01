/* target: ovr130 */
/* opts: -mm -1 -G -O -Y -d */

struct Object { unsigned id; };
extern struct Object far *ActiveObj;
extern char Containers[], Lights[], Food[];
void far fread(void *address, int size, int count, int fd);

/* FM Towns: misc_init, the class 2 loader named by init_objects. */
void far misc_init(int fd)
{
    fread(Containers, 3, 16, fd);
    fread(Lights, 2, 16, fd);
    fread(Food, 1, 16, fd);
}

/* FM Towns: misc_class_data, the class 2 lookup named by get_class_data. */
char * far misc_class_data(void)
{
    register int subclass;
    register int minor;
    minor = (ActiveObj->id & 0x30) >> 4;
    subclass = ActiveObj->id & 0x0F;
    switch (minor) {
    case 0: return Containers + subclass * 3;
    case 1: return Lights + subclass * 2;
    case 2: return 0;
    default: return Food + subclass;
    }
}
