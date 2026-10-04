/* target: ovr129 */
/* opts: -mm -1 -G -O -Y -d */
/* Object class data: loading DATA\OBJECTS.DAT through each major class's loader and
   DATA\COMOBJ.DAT into ComObjData, and finding the class data for the active object:
   the whole of DOS overlay ovr129, in original order. It compiles from UW2's OBJCLASS.C
   (ovr134) unchanged; names are UW2's (FM Towns originals); the source file's own name
   is not known.
   Each item id has two kinds of fixed data: the common properties every item has
   (struct ComObj, 11 bytes from COMOBJ.DAT, indexed by item id) and a class record that
   only some major classes have (OBJECTS.DAT, split between the per-class loaders). A
   caller sets ActiveObj to the object and calls get_class_data, which dispatches on the
   major class to that class's *_class_data function; ovr121 (the inventory panel, UW2's
   INVPANEL.C) and ovr133 (UW2's PLAYDATA.C) do so. init_objects is called once at
   start-up by ovr109 (UW2's UWEDIT.C).
   Name: descriptive (named after what it holds, init_objects and get_class_data). */

#include <stdio.h>
#include <string.h>
#include "critter.h"
#include "event.h"
#include "object.h"
#include "sys.h"

/* Declared in each file that uses it, its own way (no header). */
char * far rect_class_data(void);
char * far spec_class_data(void);
char * far stuff_class_data(void);

/* match: this file's _BSS, DS:5B6A..716D (UW2: DS:6B70..8173), by name: ActiveObj 145, ComObjData 355. This file loads
   ComObjData (0x1600 bytes, from comobj.dat) and finds the active object's class data. */
struct Object far *ActiveObj;
struct ComObj ComObjData[512];

/* Reads OBJECTS.DAT (a 2-byte header, then each class's tables in major class order,
   classes 3 to 5 having none) and COMOBJ.DAT (a 2-byte header, then 512 records of 11
   bytes). Returns 0, or ERR_READ with 5 or 6 when a file cannot be opened. */
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
        return ERR_READ | 0x5;
    fread(&hdr, 2, 1, fp);
    for (i = 0; i < 8; i++)
        if (init[i])
            init[i](fp);
    fclose(fp);
    strcpy(name, "DATA\\");
    strcat(name, "comobj.dat");
    if ((fp = fopen(name, "rb")) == NULL)
        return ERR_READ | 0x6;
    fread(&hdr, 2, 1, fp);
    fread(ComObjData, 0x1600, 1, fp);
    fclose(fp);
    return 0;
}

/* name: UW2's Return0_ovr134_EE with UW1's segment; in UW2 FM Towns has nothing between
   init_objects_ and get_class_data_. Nothing in UW1 calls it either, though it has an
   overlay entry (stub129_2A). targets/ovr129.tsv calls it make_stew, from kin.py matching
   its seven bytes against COMBINE.C's make_stew; that is coincidence of an empty body. */
char far Return0_ovr129_EE(void)
{
    return 0;
}

/* Returns ActiveObj's class record, or 0 for a class without one (the caller sets
   ActiveObj first). */
char * far get_class_data(void)
{
    register int cls = OBJ_MAJOR(ActiveObj);
    char * (far *data[8])(void) = {
        hack_class_data, (char * (far *)(void))creature_class_data, misc_class_data,
        stuff_class_data, spec_class_data, rect_class_data,
        (char * (far *)(void))trap_class_data, animobj_class_data
    };

    return data[cls]();
}
