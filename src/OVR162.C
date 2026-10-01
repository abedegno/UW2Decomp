/* target: ovr162 */
/* opts: -mm -1 -G -O -Y -d */

extern char TriggerObjectDatTable[];
void far FileReadMaybe(void *address, int size, int count, int fd);

void far ovr162_0(int fd)
{
    FileReadMaybe(TriggerObjectDatTable, 1, 16, fd);
}
