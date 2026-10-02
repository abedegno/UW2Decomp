/* target: ovr134 */
/* opts: -mm -1 -G -O -Y -d */
/* Object class data: loading DATA\OBJECTS.DAT through each major class's loader and
   DATA\COMOBJ.DAT into ComObjData, and finding the class data for the active object:
   the whole of DOS overlay ovr134, in original order. Function and global names are the
   originals from the FM Towns symbol table where it has them; the source file's own name
   is not known. */

#include <stdio.h>
#include <string.h>
#include "object.h"

#define OBJ_MAJOR(o)    (((o)->id & 0x1C0) >> 6)

/* This file's _BSS, DS:6B70..8173, by name: ActiveObj 145, ComObjData 355. This file loads
   ComObjData (0x1600 bytes, from comobj.dat) and finds the active object's class data. */
struct Object far *ActiveObj;
struct ComObj ComObjData[512];

/* Each major class's part of OBJECTS.DAT; classes 3 to 5 have none. */
void far hack_init(FILE *fp);
void far creature_init(FILE *fp);
void far misc_init(FILE *fp);
void far trap_init(FILE *fp);
void far animobj_load(FILE *fp);

/* Each major class's data for the active object. */
char * far creature_class_data(void);
char * far stuff_class_data(void);
char * far spec_class_data(void);
char * far rect_class_data(void);
char * far trap_class_data(void);

int far init_objects(void)
{
    int hdr;
    char name[0x42];
    void (far *init[8])(FILE *fp) = {
        hack_init, creature_init, misc_init, 0, 0, 0, trap_init, animobj_load
    };
    register int i;
    FILE *fp;

    strcpy(name, "DATA\\objects.dat");
    if ((fp = fopen(name, "rb")) == NULL)
        return 0x3005;
    fread(&hdr, 2, 1, fp);
    for (i = 0; i < 8; i++)
        if (init[i])
            init[i](fp);
    fclose(fp);
    strcpy(name, "DATA\\");
    strcat(name, "comobj.dat");
    if ((fp = fopen(name, "rb")) == NULL)
        return 0x3006;
    fread(&hdr, 2, 1, fp);
    fread(ComObjData, 0x1600, 1, fp);
    fclose(fp);
    return 0;
}

/* DOS only; FM Towns has nothing between init_objects_ and get_class_data_, and nothing in
   DOS calls it, though it has an overlay entry. */
char far Return0_ovr134_EE(void)
{
    return 0;
}

char * far get_class_data(void)
{
    register int cls = OBJ_MAJOR(ActiveObj);
    char * (far *data[8])(void) = {
        hack_class_data, creature_class_data, misc_class_data, stuff_class_data,
        spec_class_data, rect_class_data, trap_class_data, animobj_class_data
    };

    return data[cls]();
}
