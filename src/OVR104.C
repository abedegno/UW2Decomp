/* target: ovr104 */
/* opts: -mm -1 -G -O -Y -d */

struct Creature { unsigned char bytes[0x30]; };
struct Object { unsigned id, pos;
    union { unsigned word; struct { unsigned quality:6, next:10; } f; } qn;
    union { unsigned word; struct { unsigned owner:6, link:10; } f; } ol;
    unsigned char bytes[0x1B - 8]; };

extern struct Creature Creature[64];
extern struct Object far *ActiveObj;
extern unsigned ItemMinorClass, ItemSubClass;
extern struct Creature *cst;
void far fread(void *address, int size, int count, int fd);
void far fwrite(void *address, int size, int count, int fd);
int far rand(void);
char far init_this_critter(struct Object far *obj);

void far creature_init(int fd)
{
    fread(Creature, 0x30, 0x40, fd);
}

void far ovr104_17(int fd)
{
    fwrite(Creature, 0x30, 0x40, fd);
}

struct Creature * far creature_class_data(void)
{
    ItemMinorClass = (ActiveObj->id & 0x30) >> 4;
    ItemSubClass = ActiveObj->id & 0xF;
    return (struct Creature *)((char *)Creature + (ItemMinorClass * 16 + ItemSubClass) * 0x30);
}

void far creature_obj_init(void)
{
    init_this_critter(ActiveObj);
}

/* FM Towns identifies the target table's InitialiseCritterValues as init_this_critter:
   creature_obj_init calls it and SEG036's CreateObj calls it for new creatures. */
char far init_this_critter(struct Object far *obj)
{
    register int x;
    register int y;

    x = 0x20;
    y = 0x20;
    *(unsigned far *)((unsigned char far *)obj+0x16) = (*(unsigned far *)((unsigned char far *)obj+0x16) & 0x3ff) | ((x & 0x3f) << 10);
    *(unsigned far *)((unsigned char far *)obj+0x16) = (*(unsigned far *)((unsigned char far *)obj+0x16) & 0xfc0f) | ((y & 0x3f) << 4);
    obj->qn.f.quality = x;
    obj->ol.f.owner = y;
    cst = (struct Creature *)((char *)Creature + (obj->id & 0x3f) * 0x30);
    ((unsigned char far *)obj)[8] = (cst->bytes[4] * (rand() % 0x18 + 0x10)) / 0x20;
    ((unsigned char far *)obj)[9] = ((obj->pos & 0x380) >> 7) << 5;
    *(unsigned far *)((unsigned char far *)obj+0xb) = (*(unsigned far *)((unsigned char far *)obj+0xb) & 0xfff0) | 8;
    *(unsigned far *)((unsigned char far *)obj+0xb) = *(unsigned far *)((unsigned char far *)obj+0xb) & 0xf00f;
    *(unsigned far *)((unsigned char far *)obj+0xd) = *(unsigned far *)((unsigned char far *)obj+0xd) & 0xfff0;
    *(unsigned far *)((unsigned char far *)obj+0xf) = *(unsigned far *)((unsigned char far *)obj+0xf) & 0xffc0;
    *(unsigned far *)((unsigned char far *)obj+0xf) = *(unsigned far *)((unsigned char far *)obj+0xf) & 0xf03f;
    *(unsigned far *)((unsigned char far *)obj+0xd) = *(unsigned far *)((unsigned char far *)obj+0xd) & 0xff0f;
    *(unsigned far *)((unsigned char far *)obj+0xd) = *(unsigned far *)((unsigned char far *)obj+0xd) & 0xfdff;
    *(unsigned far *)((unsigned char far *)obj+0xd) = *(unsigned far *)((unsigned char far *)obj+0xd) & 0xfbff;
    *(unsigned far *)((unsigned char far *)obj+0xd) = *(unsigned far *)((unsigned char far *)obj+0xd) & 0xf7ff;
    *(unsigned far *)((unsigned char far *)obj+0xd) = *(unsigned far *)((unsigned char far *)obj+0xd) & 0xfeff;
    ((unsigned char far *)obj)[0x18] &= 0xdf;
    *(unsigned far *)((unsigned char far *)obj+0xf) = *(unsigned far *)((unsigned char far *)obj+0xf) & 0x0fff;
    ((unsigned char far *)obj)[0xa] &= 0xf0;
    ((unsigned char far *)obj)[0x14] = (((unsigned char far *)obj)[0x14] & 0xf8) | 4;
    ((unsigned char far *)obj)[0x15] &= 0xc0;
    *(unsigned far *)((unsigned char far *)obj+0xb) = *(unsigned far *)((unsigned char far *)obj+0xb) & 0x0fff;
    ((unsigned char far *)obj)[0x14] = (((unsigned char far *)obj)[0x14] & 7) | 0x80;
    ((unsigned char far *)obj)[0x13] &= 0x7f;
    ((unsigned char far *)obj)[0x13] &= 0x80;
    ((unsigned char far *)obj)[0x11] = 0;
    ((unsigned char far *)obj)[0x12] = 0;
    ((unsigned char far *)obj)[0x15] &= 0x7f;
    ((unsigned char far *)obj)[0x18] &= 0x7f;
    ((unsigned char far *)obj)[0x18] &= 0xbf;
    *(unsigned far *)((unsigned char far *)obj+0x16) = *(unsigned far *)((unsigned char far *)obj+0x16) & 0xfff0;
    ((unsigned char far *)obj)[0x15] &= 0xbf;
    ((unsigned char far *)obj)[0x1a] = 0;
    ((unsigned char far *)obj)[0x19] &= 0xfe;
    ((unsigned char far *)obj)[0x19] &= 0xfd;
    ((unsigned char far *)obj)[0x19] &= 0xef;
    ((unsigned char far *)obj)[0x19] &= 0xdf;
    ((unsigned char far *)obj)[0x19] &= 0xbf;
    ((unsigned char far *)obj)[0x19] &= 0x7f;
    *(unsigned far *)((unsigned char far *)obj+0xd) = *(unsigned far *)((unsigned char far *)obj+0xd) & 0xefff;
    *(unsigned far *)((unsigned char far *)obj+0xd) = *(unsigned far *)((unsigned char far *)obj+0xd) & 0xdfff;
    *(unsigned far *)((unsigned char far *)obj+0xd) = (*(unsigned far *)((unsigned char far *)obj+0xd) & 0x3fff) | 0x8000;
    ((unsigned char far *)obj)[0xa] &= 0x7f;
    ((unsigned char far *)obj)[0x19] &= 0xf3;
    ((unsigned char far *)obj)[0xa] &= 0x8f;
    return 1;
}
