/* target: ovr159 */
/* opts: -mm -1 -G -O -Y -d */
/* Class data for major class 3 (MAJOR_STUFF: scenery, potions and runestones): the class has no
   table in DATA\OBJECTS.DAT and no loader, so its lookup returns 0. get_class_data
   (OBJCLASS.C) calls it for an object of this class. FM Towns has the same function
   (xor eax,eax; ret).
   Name: inferred (the class prefix, after stuff_class_data). */

int far stuff_class_data(void)
{
    return 0;
}
