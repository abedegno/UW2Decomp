/* target: seg034_310D */
/* opts: -mm -1 -G -O -Y -d */

/* FM Towns identifies the formerly anonymous helpers by their matching
   positions and operations: sort_setup, sort_obj, build_sort, set_sds,
   set_osum and clear_objsort. */
struct Object { unsigned item; unsigned pos; char pad[0x20]; };
struct ObjectIndex { unsigned low:6; unsigned index:10; };
struct Camera { char pad[10]; int x; char gap[2]; int z; char gap2[2]; int y; };
struct ComObj {
    unsigned height:8;
    unsigned radius:3;
    unsigned animated:1;
    unsigned flags_rest:4;
    char rest[9];
};

/* This file's _BSS, DS:2F9C..33C5 (seg033's ActDoors ends at 2F9B; seg035's starts at 33C6),
   laid out by name (tools/bssorder.py): holdmid 112, holdtmp 144, locsqmod 228,
   sortlist 267, sortdata 275, mptrmod 421, sd_xmod and sd_ymod 427, dirval 492,
   refugees 666, objxloc, objyloc and objzloc 935, objptrs 959. All FM Towns names: FM Towns
   has objptrs, the same 60 entries, right after refugees. */
unsigned short holdmid[9], holdtmp[9];
unsigned char locsqmod;
signed char sortlist[60];
signed char sortdata[60][4];
int mptrmod;
int sd_xmod, sd_ymod;
signed char dirval;
unsigned short refugees[33][9];               /* 0x252 bytes: the memset clears them all */
int objxloc, objyloc, objzloc;
unsigned short objptrs[60];
/* This file's _DATA, DS:06FE..073D: between seg033's data and seg035's, and only this
   file uses it (FM Towns has it right after seg033's dirtab, before seg035's tables). */
signed char trans_pos_x[64] = {
    0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0,
    0, 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7,
    7, 0, 6, 0, 5, 0, 4, 0, 3, 0, 2, 0, 1, 0, 0, 0,
    0, 7, 0, 6, 0, 5, 0, 4, 0, 3, 0, 2, 0, 1, 0, 0 };
extern signed char quad;
extern int loopx, loopy;
extern struct Camera far *cPlayer;
extern struct ComObj ComObjData[];
extern unsigned char PickUp;
extern int far smooth_div;
extern int far smooth_lowpass;
extern unsigned char far smooth_base;

void far *far Obj_IntTMem(unsigned index);
struct Object far *far Obj_PtrTMem(struct Object far *object);
unsigned char far IsMobElem(struct Object far *object);
unsigned long far cSqRt(long value);
void far do_obj(struct Object far *object);
void far memset(void near *dest, int value, unsigned count);
void far memcpy(void near *dest, void near *src, unsigned count);

void far sort_setup(char mode)
{
    switch (mode) {
    case -10:
        memset(refugees, 0, 0x252);
        memset(holdmid, 0, 0x12);
        memset(holdtmp, 0, 0x12);
        break;
    case 2:
        memset(holdmid, 0, 0x12);
        memset(holdtmp, 0, 0x12);
        break;
    case 1:
        memcpy(holdmid, holdtmp, 0x12);
        holdtmp[0] = 0;
        break;
    case 0:
        if (holdmid[0]) {
            if (holdmid[0] + holdtmp[0] >= 9)
                holdmid[0] = 9 - holdtmp[0] - 1;
            memcpy(holdtmp + holdtmp[0] + 1, holdmid + 1, holdmid[0] * 2);
            holdtmp[0] += holdmid[0];
        }
        break;
    }
    dirval = mode;
}

void far sort_obj(int first, int count)
{
    int last = count - 1;
    int i;
    for (; last >= first; last--) {
        for (i = first; i <= last; i++) {
            if (sortdata[sortlist[i]][0] < sortdata[sortlist[i+1]][0]) {
                char t = sortlist[i];
                sortlist[i] = sortlist[i+1];
                sortlist[i+1] = t;
            }
        }
    }
}

void far build_sort(int count)
{
    int i;
    for (i = 0; i < count; i++)
        sortlist[i] = i;
}

void far do_partition(char reverse, int *point, int skip, int count,
                      int height, int from_data)
{
    int ends[2], steps[2];
    int value;
    int side;
    register int i;
    if (reverse) {
        ends[0] = count - 1; steps[0] = -1;
        ends[1] = 0; steps[1] = 1;
    } else {
        ends[0] = 0; steps[0] = 1;
        ends[1] = count - 1; steps[1] = -1;
    }
    for (i = 0; i < count;) {
        if (i != skip) {
            if (from_data == 0)
                value = ((struct Object far *)Obj_IntTMem(objptrs[i]))->pos & 0x7f;
            else
                value = sortdata[i][from_data];
            side = value > height;
            /* the post-increment here keeps i in SI: storing it as a char
               in its own statement left it on the stack */
            sortlist[ends[side]] = i++;
            ends[side] += steps[side];
        } else
            i++;
    }
    /* an early return: the nested if reused AX for *point instead of
       reloading ends[0] */
    if (ends[0] != ends[1]) return;
    *point = ends[0];
    sortlist[*point] = skip;
}

void far z_part(int index, int *point, int count)
{
    struct Object far *object;
    int z;
    object = Obj_IntTMem(objptrs[index]);
    z = object->pos & 0x7f;
    if ((object->item & 0x1ff) == 0x158)
        z = z + ComObjData[object->item & 0x1ff].height;
    do_partition(z * 8 > cPlayer->z, point, index, count, z, 0);
}

void far door_part(int index, int *point, int count)
{
    char reverse;
    int pos, axis;
    if (!(((((struct Object far *)Obj_IntTMem(objptrs[index]))->pos & 0x380) >> 7) + quad * 2 & 3)) {
        axis = 2;
        pos = sortdata[index][2];
        reverse = 1;
    } else {
        axis = 1;
        pos = sortdata[index][1];
        if (dirval == 2) reverse = 0;
        else if (dirval == 1) reverse = 1;
        else reverse = (cPlayer->x >> 5) < pos;
    }
    do_partition(reverse, point, index, count, pos, axis);
}

void far set_sds(signed char *data, struct Object far *object)
{
    data[1] = trans_pos_x[quad * 16 + ((object->pos & 0xe000) >> 13) * 2]
            + trans_pos_x[((quad + 1) & 3) * 16 + ((object->pos & 0x1c00) >> 10) * 2];
    data[2] = trans_pos_x[quad * 16 + ((object->pos & 0xe000) >> 13) * 2 + 1]
            + trans_pos_x[((quad + 1) & 3) * 16 + ((object->pos & 0x1c00) >> 10) * 2 + 1];
    data[3] = object->pos & 0x7f;
}

void far set_osum(signed char *data)
{
    switch (dirval) {
    case 0: data[0] = data[2] << 1; break;
    case 1: data[0] = data[1] + data[2] + 1; break;
    case 2: data[0] = 8 - data[1] + data[2]; break;
    }
}

void far do_objsort(struct Object far *object);

void far clear_objsort(void)
{
    if (refugees[loopx][0] > 0) do_objsort(0L);
    if (holdtmp[0] > 0) do_objsort(0L);
}

void far do_objsort(struct Object far *object)
{
    register int word;
    struct Object far *next;
    unsigned short *dest;
    int visited, i, item, radius, partition, n, pivot;
    unsigned char candidate, held;
    int temp, distance;

    visited = 0;
    partition = 0;
    n = 0;
    pivot = -1;
    for (i = 1; (unsigned)i <= refugees[loopx][0] && n < 9; i++) {
        register signed char *data;
        word = refugees[loopx][i];
        objptrs[n] = word & 0x3ff;
        next = Obj_IntTMem(word & 0x3ff);
        data = sortdata[n];
        set_sds(data, next);
        if (word & 0x1000) {
            if (word & 0x2000) data[1] += -8;
            else data[1] += 8;
        }
        if (word & 0x4000) data[2] += 8;
        set_osum(data);
        if (word & 0x8000) data[0]--;
        word = next->item & 0x1ff;
        n++;
    }
    if (holdtmp[0])
        memcpy(refugees[loopx], holdtmp, 0x12);
    else
        refugees[loopx][0] = 0;
    holdtmp[0] = 0;

    next = Obj_PtrTMem(object);
    while (next && visited < 60) {
        register signed char *data;
        candidate = held = 0;
        item = next->item & 0x1ff;
        if (item == 0x164 || (item >> 4) == 0x14 || item == 0x1cf) {
            partition = n + (item << 6);
        } else if (ComObjData[item].animated) {
            word = 0;
            held = 1;
            data = sortdata[n];
            set_sds(data, next);
            radius = ComObjData[item].radius;
            if (data[2] - radius < 0) word |= 0x4000;
            if (dirval == 1) radius = -radius;
            if (dirval && ((data[1] + radius) & ~7)) {
                word |= 0x1000;
                if (dirval == 2) word |= 0x2000;
            }
            if (word) {
                candidate = 1;
                word |= (object->item >> 6) & 0x3ff;
                if ((item & 0x1c0) == 0x1c0) word |= 0x8000;
                if ((word & 0x5000) == 0x5000) dest = holdtmp;
                else if (word & 0x4000) dest = refugees[loopx];
                else if (word & 0x2000) dest = refugees[loopx] + 9;
                else dest = refugees[loopx - 1];
                if (dest[0] >= 9) candidate = 0;
                else {
                    dest[0]++;
                    dest[dest[0]] = word;
                }
            }
        }
        if (!candidate) {
            if (!held) {
                data = sortdata[n];
                set_sds(data, next);
            }
            set_osum(data);
            if (held && item != 0x158) data[0] -= radius * 2;
            if ((item & 0x1c0) == 0x1c0) data[0]--;
            else if ((item & 0x1fe) == 0x16e) data[0] += 0x20;
            objptrs[n] = ((struct ObjectIndex far *)object)->index;
            if (n < 60) n++;
        }
        object = (struct Object far *)((char far *)next + 4);
        next = Obj_PtrTMem(object);
        visited++;
    }
    if (partition && n > 1) {
        if ((partition >> 6) == 0x164)
            z_part(partition & 0x3f, &pivot, n);
        else
            door_part(partition & 0x3f, &pivot, n);
        if (pivot > 1) sort_obj(0, pivot - 1);
        if (n - 2 > pivot) sort_obj(pivot + 1, n - 1);
    } else {
        build_sort(n);
        if (n > 1) sort_obj(0, n - 1);
    }
    for (i = 0; i < n; i++) {
        word = sortlist[i];
        next = Obj_IntTMem(objptrs[word]);
        objxloc = ((loopx - 16) << 8) + ((int)sortdata[word][1] << 5) + 16;
        objzloc = (loopy << 8) + ((int)sortdata[word][2] << 5) + 16;
        if (((next->item & 0x1c0) >> 6) == 1 || !IsMobElem(next))
            objyloc = (next->pos & 0x7f) << 3;
        else
            objyloc = *(int far *)((char far *)next + 15);
        if (PickUp) {
            mptrmod = (sortdata[word][2] / 8) * sd_ymod;
            mptrmod += ((sortdata[word][1] + 64) / 8 - 8) * sd_xmod;
        } else {
            temp = (objxloc - (cPlayer->x & 0xff)) >> 5;
            distance = temp * temp;
            temp = (objzloc - (cPlayer->y & 0xff)) >> 5;
            distance += temp * temp;
            temp = (objyloc - cPlayer->z) >> 5;
            distance += temp * temp;
            if (distance > 0) temp = cSqRt(distance);
            else temp = 0;
            distance = temp * smooth_div >> 6;
            distance += smooth_lowpass;
            if (distance < 0) distance = 0;
            locsqmod = distance + smooth_base;
            if (locsqmod > 14) locsqmod = 14;
        }
        do_obj(next);
    }
}
