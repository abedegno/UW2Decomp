/* target: seg009 */
/* opts: -mm -1 -G -O -Y -d */
/* Graphic resource lookup, decoding, cursor drawing and image scaling. */

#include <dos.h>
#include "gfx.h"
#include "sys.h"
#include "view3d.h"

extern unsigned char Palettes[];
extern unsigned char far Transparency;

void far MapMemory_seg013_1D3C_C7(int physical, int page);
void far show(int x, int y, void far *data, int width, int height, int a, int b);

void far * far seg009_7(int icon)
{
    unsigned char page = (grs_off[icon] & 0xf000) >> 12;
    if (obj_inpage1 != page) {
        obj_inpage1 = page;
        MapMemory_seg013_1D3C_C7(2, page);
    }
    return MK_FP(EmsBuff + (grs_off[icon] & 0x3ff) + 0x800, 0);
}

void far seg009_73(int icon, int x, int y, int height, int width)
{
    struct Bitmap far *raw;
    void far *picture;
    if (icon >= first_vram) {
        DRAW_RELATED_seg017_2179_320(grs_off[icon], &width, &height);
        seg003_0272_5025(grs_off[icon] + 1, x, y, width, height, 0, 0);
    } else {
        raw = seg009_7(icon);
        width = raw->width;
        height = raw->height;
        if (raw->type != BM_8BIT)
            picture = cFrmtoRaw(&raw->u.b4.size, Palettes + ((unsigned)raw->u.b4.auxpal << 4),
                                raw->type);
        else
            picture = raw->u.b8.data;
        show(x, y, picture, height, width, 0, 0);
    }
}

/* FM Towns grs_unpack_ occupies this slot and has the same format-4/raw split. */
void far * far grs_unpack(void far *data)
{
    void far *result;
    if (((struct Bitmap far *)data)->type != BM_8BIT)
        result = cFrmtoRaw(&((struct Bitmap far *)data)->u.b4.size,
                   Palettes + ((unsigned)((struct Bitmap far *)data)->u.b4.auxpal << 4),
                   ((struct Bitmap far *)data)->type);
    else
        result = ((struct Bitmap far *)data)->u.b8.data;
    return result;
}

/* FM Towns grs_fbplot_ decodes the icon, then sends it to fbshow_. */
void far grs_fbplot(int icon, int x, int y)
{
    struct Bitmap far *p = seg009_7(icon);
    int width = p->width, height = p->height;
    void far *picture;
    if (p->type != BM_8BIT)
        picture = cFrmtoRaw(&p->u.b4.size, Palettes + ((unsigned)p->u.b4.auxpal << 4), p->type);
    else
        picture = p->u.b8.data;
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
    unsigned char page = tmap_fpage + (index >> 2);
    if (tmap_inpage != page) {
        MapMemory_seg013_1D3C_C7(3, page);
        tmap_inpage = page;
    }
    return EmsBuff + ((index & 3) << 8) + 0xc00;
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
