/* target: seg009 */
/* opts: -mm -1 -G -O -Y -d */
/* Graphic resource lookup, decoding and cursor drawing: the whole of UW1's DOS resident
   segment seg009, in original order. UW1 has no symbol-bearing build; the names are UW2's
   (the FM Towns symbol table; UW2's GRSPIC.C, seg009), the code being the same routines.

   The game refers to a 2D picture by an icon number. grs_which1 turns it into a slot of
   grs_off, the table LOADGR.C fills: numbers from 2000h are pictures kept in video memory
   (slot first_vram + n - 2000h), numbers from 1000h are buttons, cursors and the other
   screen art (first_button + n - 1000h), and smaller numbers are objects, looked up through
   obj_tab. A slot below first_vram holds an EMS address (logical page in the top four
   bits, paragraph in the low ten), and seg009_1 maps that page into physical page 2 of
   the EMS frame to reach the .GR bitmap (struct Bitmap, gfx.h); 4-bit bitmaps are expanded
   with cFrmtoRaw through one of the 16-colour palettes of Palettes. A slot from first_vram
   on is the video memory offset of a picture, drawn with seg003_581E. pic_to_screen draws
   on the screen, pic_to_fbuf into the 3D view's frame buffer. seg009_38C gives the segment
   of a level texture in EMS. The file owns no data.

   UW1's differences: seg009_38C (UW2's seg009_392) looks up a texture's EMS page and
   segment in two tables, where UW2 computes them (four textures to a page from
   tmap_fpage); there is no grs_scaledown.

   name: descriptive; grs_unpack, grs_fbplot, grs_which1, pic_to_screen, pic_to_fbuf and
   mask_to_screen are the FM Towns names (UW2). The others keep the UW1 listing's names, as
   UW2's keep its own: UW2 rejected the FM Towns candidate grs_plot_ for seg009_1's
   counterpart (size and calls disagree), which map/functions.tsv nevertheless gives it
   here. */

#include <dos.h>
#include "gfx.h"
#include "sys.h"
#include "view3d.h"

/* UW1: the callees under their UW1 listing names (UW2: MapMemory_seg013_1D3C_C7,
   DRAW_RELATED_seg017_2179_320, seg003_0272_5025). */
/* UW1: the texture page and segment tables in seg051 (the 3D renderer's far data,
   assembly); the listing has no names for them. */

/* Maps the EMS page holding slot icon into physical page 2 (unless it is mapped already,
   obj_inpage1) and returns the picture's address in the frame. */
void far * far seg009_1(int icon)
{
    unsigned char page = (grs_off[icon] & 0xf000) >> 12;
    if (obj_inpage1 != page) {
        obj_inpage1 = page;
        seg012_10F(2, page);
    }
    return MK_FP(EmsBuff + (grs_off[icon] & 0x3ff) + 0x800, 0);
}

/* Draws slot icon at x, y on the screen. The height and width arguments are overwritten
   with the picture's own size before use, so the callers' values do not matter. */
void far seg009_6D(int icon, int x, int y, int16 height, int16 width)
{
    struct Bitmap far *raw;
    void far *picture;
    if (icon >= first_vram) {
        seg015_1F9B_325(grs_off[icon], &width, &height);
        seg003_581E(grs_off[icon] + 1, x, y, width, height, 0, 0);
    } else {
        raw = seg009_1(icon);
        width = raw->width;
        height = raw->height;
        if (raw->type != BM_8BIT)
            picture = cFrmtoRaw(&raw->u.b4.size, Palettes[raw->u.b4.auxpal],
                                raw->type);
        else
            picture = raw->u.b8.data;
        show(x, y, picture, height, width, 0, 0);
    }
}

/* Returns the 8-bit pixels of a .GR bitmap: the data itself for an 8-bit bitmap, else
   cFrmtoRaw's expansion of a 4-bit one.
   name: FM Towns grs_unpack_ occupies this slot and has the same format-4/raw split. */
void far * far grs_unpack(void far *data)
{
    void far *result;
    if (((struct Bitmap far *)data)->type != BM_8BIT)
        result = cFrmtoRaw(&((struct Bitmap far *)data)->u.b4.size,
                   Palettes[((struct Bitmap far *)data)->u.b4.auxpal],
                   ((struct Bitmap far *)data)->type);
    else
        result = ((struct Bitmap far *)data)->u.b8.data;
    return result;
}

/* Draws slot icon into the 3D view's frame buffer with fbshow (GRCORE.ASM).
   name: FM Towns grs_fbplot_ decodes the icon, then sends it to fbshow_. */
void far grs_fbplot(int icon, int x, int y)
{
    struct Bitmap far *p = seg009_1(icon);
    int width = p->width, height = p->height;
    void far *picture;
    if (p->type != BM_8BIT)
        picture = cFrmtoRaw(&p->u.b4.size, Palettes[p->u.b4.auxpal], p->type);
    else
        picture = p->u.b8.data;
    fbshow(picture, x, y, width, height);
}

/* Icon number to grs_off slot (see the file comment). */
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

/* Draws icon on the screen; icons 101Bh..101Eh are always drawn with Transparency set. */
void far pic_to_screen(int icon, int x, int y, int height, int width)
{
    unsigned char special = icon >= 0x101b && icon <= 0x101e;
    if (special) Transparency = 1;
    icon = grs_which1(icon);
    seg009_6D(icon, x, y, height, width);
    if (special) Transparency = 0;
}

/* Reads an icon's width and height. Nothing in UW1.EXE calls it either; the target table
   runs it into pic_to_screen's row (IDA made no procedure of it), so it is static here. The EMS branch stores
   through width and height as near int pointers; the video memory branch passes the
   addresses of the parameters themselves, so its result is lost. */
static void far seg009_2C6(int icon, NEARPTR width, NEARPTR height)
{
    unsigned char far *p;
    icon = grs_which1(icon);
    if (icon >= first_vram)
        seg015_1F9B_325(grs_off[icon], (int16 far *)&width, (int16 far *)&height);
    else {
        p = seg009_1(icon);
        *(int16 *)width = p[1];
        *(int16 *)height = p[2];
    }
}

void far pic_to_fbuf(int icon, int x, int y)
{
    icon = grs_which1(icon);
    grs_fbplot(icon, x, y);
}

/* Draws a video memory picture through seg003_581E, passing clip as its last
   argument (in UW2 the sprite library's set_yoff value, SPRITE.ASM) and adding it to the
   size argument before it. In UW2 SPRITE.ASM pushes the sprite's height in the width slot
   and its width in the height slot, so the parameter names here are probably swapped. */
/* name: in UW2, FM Towns mask_to_screen_ sits between pic_to_fbuf_ and grs_scaledown_. */
void far mask_to_screen(int icon, int x, int y, int width, int height, int clip)
{
    icon = grs_which1(icon);
    seg003_581E(grs_off[icon] + 1, x, y, height, width + clip, 0, clip);
}

/* The segment of level texture index in EMS: UW1 keeps each texture's EMS page in
   seg051_C378 (0: none) and its segment in seg051_C3EC; a page is mapped into physical
   page 3, cached in tmap_inpage. */
unsigned far seg009_38C(int index)
{
    unsigned char page = seg051_C378[index];
    if (page && tmap_inpage != page) {
        seg012_10F(3, page);
        tmap_inpage = page;
    }
    return seg051_C3EC[index];
}
