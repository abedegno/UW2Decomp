/* target: seg009 */
/* opts: -mm -1 -G -O -Y -d */
/* Graphic resource lookup, decoding, cursor drawing and image scaling. */

#include <dos.h>

extern unsigned far *grs_off;
extern unsigned far *obj_tab;
extern unsigned first_vram, first_button;
extern unsigned char Palettes[];
extern unsigned char far cmpbuf1_start[], far cmpbuf2_start[];
extern unsigned char far TextureLogicalPage;
extern unsigned char far tmap_inpage, far obj_inpage1;
extern unsigned far ems_seg;
extern unsigned char far Transparency;

void far MapMemory_seg013_1D3C_C7(int physical, int page);
void far DRAW_RELATED_seg017_2179_320(unsigned offset, int far *width, int far *height);
void far seg003_0272_5025(int icon, int x, int y, int width, int height, int a, int b);
void far show(int x, int y, void far *data, int width, int height, int a, int b);
void far fbshow(void far *data, int x, int y, int width, int height);
void far * far seg021_22FD_F83(void far *data, unsigned char far *pal, unsigned char mode);

void far * far seg009_7(int icon);
void far seg009_73(int icon, int x, int y, int height, int width);
void far * far grs_unpack(void far *data);
void far grs_fbplot(int icon, int x, int y);
int far grs_which1(int icon);
void far pic_to_screen(int icon, int x, int y, int height, int width);
void far seg009_2CC(int icon, int width, int height);
void far pic_to_fbuf(int icon, int x, int y);
void far mask_to_screen(int icon, int x, int y, int width, int height, int clip);
unsigned far seg009_392(int index);
void far * far grs_scaledown(unsigned char far *source, int width, int height, int scale);

void far * far seg009_7(int icon)
{
    unsigned char page = (grs_off[icon] & 0xf000) >> 12;
    if (obj_inpage1 != page) {
        obj_inpage1 = page;
        MapMemory_seg013_1D3C_C7(2, page);
    }
    return MK_FP(ems_seg + (grs_off[icon] & 0x3ff) + 0x800, 0);
}

void far seg009_73(int icon, int x, int y, int height, int width)
{
    unsigned char far *raw;
    void far *picture;
    if (icon >= first_vram) {
        DRAW_RELATED_seg017_2179_320(grs_off[icon], &width, &height);
        seg003_0272_5025(grs_off[icon] + 1, x, y, width, height, 0, 0);
    } else {
        raw = seg009_7(icon);
        width = raw[1];
        height = raw[2];
        if (raw[0] != 4)
            picture = seg021_22FD_F83(raw + 4, Palettes + ((unsigned)raw[3] << 4), raw[0]);
        else
            picture = raw + 5;
        show(x, y, picture, height, width, 0, 0);
    }
}

/* FM Towns grs_unpack_ occupies this slot and has the same format-4/raw split. */
void far * far grs_unpack(void far *data)
{
    void far *result;
    if (((unsigned char far *)data)[0] != 4)
        result = seg021_22FD_F83((unsigned char far *)data + 4,
                   Palettes + ((unsigned)((unsigned char far *)data)[3] << 4),
                   ((unsigned char far *)data)[0]);
    else
        result = (unsigned char far *)data + 5;
    return result;
}

/* FM Towns grs_fbplot_ decodes the icon, then sends it to fbshow_. */
void far grs_fbplot(int icon, int x, int y)
{
    unsigned char far *p = seg009_7(icon);
    int width = p[1], height = p[2];
    void far *picture;
    if (p[0] != 4)
        picture = seg021_22FD_F83(p + 4, Palettes + ((unsigned)p[3] << 4), p[0]);
    else
        picture = p + 5;
    fbshow(picture, x, y, width, height);
}

int far grs_which1(int icon)
{
    register int result;
    int id = icon;
    if (id >= 0x2000)
        result = id - 0x2000 + first_vram;
    else if (id >= 0x1000)
        result = id - 0x1000 + first_button;
    else
        result = obj_tab[2 * id] & 0x3ff;
    result = result;
    return result;
}

void far pic_to_screen(int icon, int x, int y, int height, int width)
{
    unsigned char special = icon >= 0x101b && icon <= 0x101e;
    if (special) Transparency = 1;
    icon = grs_which1(icon);
    seg009_73(icon, x, y, height, width);
    if (special) Transparency = 0;
}

void far seg009_2CC(int icon, int width, int height)
{
    unsigned char far *p;
    icon = grs_which1(icon);
    if (icon >= first_vram)
        DRAW_RELATED_seg017_2179_320(grs_off[icon], &width, &height);
    else {
        p = seg009_7(icon);
        *(int *)width = p[1];
        *(int *)height = p[2];
    }
}

void far pic_to_fbuf(int icon, int x, int y)
{
    icon = grs_which1(icon);
    grs_fbplot(icon, x, y);
}

/* FM Towns mask_to_screen_ sits between pic_to_fbuf_ and grs_scaledown_. */
void far mask_to_screen(int icon, int x, int y, int width, int height, int clip)
{
    icon = grs_which1(icon);
    seg003_0272_5025(grs_off[icon] + 1, x, y, height, width + clip, 0, clip);
}

unsigned far seg009_392(int index)
{
    unsigned char page = TextureLogicalPage + (index >> 2);
    if (tmap_inpage != page) {
        MapMemory_seg013_1D3C_C7(3, page);
        tmap_inpage = page;
    }
    return ems_seg + ((index & 3) << 8) + 0xc00;
}

/* FM Towns grs_scaledown_ uses the same alternating scratch buffers and stride. */
void far * far grs_scaledown(unsigned char far *source, int width, int height, int scale)
{
    unsigned char far *destination;
    int x, y, out_width, out_height;
    if (source != cmpbuf1_start)
        destination = cmpbuf1_start;
    else
        destination = cmpbuf2_start;
    out_width = width / scale;
    out_height = height / scale;
    for (y = 0; y < out_height; y++)
        for (x = 0; x < out_width; x++)
            (destination + y * out_width)[x] = (source + y * scale * width)[x * scale];
    return destination;
}
