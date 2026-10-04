/* c3dentry.c: replaces the C side of src/sys/C3DENTRY.ASM (seg019 module 17): the far pointers C
   reaches the 3D renderer's data through (its _DATA), and the C entry points into the renderer,
   the frame buffer and the maths. C3DENTRY.ASM is translated (sys/c3dentry_x.c), so each entry
   pushes the C arguments as Turbo C's far call did and runs the translation (x86/entry.c); the
   near pointers cFstSinCos and cSinCos write through are slots of the C stack, copied back. The
   offsets are symbols.tsv's (the link's map). */
#include "compat.h"
#include "sys.h"
#include "view3d.h"
#include "x86/asmrt.h"
#include "x86/entry.h"

#define SEG019 0x1F3Au

extern unsigned char lightabs[];

/* C3DENTRY.ASM's _DATA (DS:2370..23A3): the zero doubleword nothing reads, the far pointer to
   seg_5DFD, then the pointers C uses. */
struct Camera *cPlayer = (struct Camera *)(seg063 + 0x970);
int16 *cDbase = (int16 *)(seg051 + 0x2938);
int16 *cEntryStrt = (int16 *)(seg051 + 0x73F2);
unsigned char *cLightTabs = lightabs;
int16 *cDbbase = (int16 *)(seg051 + 0x161C);
int16 *cPixXferStuff = (int16 *)(seg048 + 0xB12);

#define W W16

void cZoom(unsigned zoom)
{
    uint16_t w[1] = { (uint16_t)zoom };
    port_far_entry(SEG019, 0x0C34, w, 1);
}

void cFBtoScreen(void) { port_far_entry(SEG019, 0x0C7E, 0, 0); }

void cFillFB(int c)
{
    uint16_t w[1] = { W(c) };
    port_far_entry(SEG019, 0x0C84, w, 1);
}

void seg019_CB7(int shade)
{
    uint16_t w[1] = { W(shade) };
    port_far_entry(SEG019, 0x0CB7, w, 1);
}

void seg019_CEA(int count)
{
    uint16_t w[1] = { W(count) };
    port_far_entry(SEG019, 0x0CEA, w, 1);
}

void cPlaceFB(int x, int y, int w_, int h)
{
    uint16_t w[4] = { W(x), W(y), W(w_), W(h) };
    port_far_entry(SEG019, 0x0D1D, w, 4);
}

void cRender(void) { port_far_entry(SEG019, 0x0D7A, 0, 0); }
void cInit3d(void) { port_far_entry(SEG019, 0x0E1A, 0, 0); }

void cFstSinCos(int angle, int16 *a, int16 *b)
{
    uint16_t w[3] = { W(angle), port_near_slot(0), port_near_slot(1) };
    port_far_entry(SEG019, 0x0E63, w, 3);
    *a = port_near_get(0);
    *b = port_near_get(1);
}

void cSinCos(int angle, int16 *x, int16 *y)
{
    uint16_t w[3] = { W(angle), port_near_slot(0), port_near_slot(1) };
    port_far_entry(SEG019, 0x0EAE, w, 3);
    *x = port_near_get(0);
    *y = port_near_get(1);
}

int cAtan2(int x, int y)
{
    uint16_t w[2] = { W(x), W(y) };
    return (int16_t)port_far_entry(SEG019, 0x0EFB, w, 2);
}

int cSqRt(int32 v)
{
    uint16_t w[2] = { (uint16_t)v, (uint16_t)((uint32_t)v >> 16) };
    return (int16_t)port_far_entry(SEG019, 0x0F3F, w, 2);
}

void *cFrmtoRaw(void *data, unsigned char *pal, unsigned char mode)
{
    uint16_t w[5];
    port_far_arg(data, w);
    port_far_arg(pal, w + 2);
    w[4] = mode;
    return port_far_result(port_far_entry(SEG019, 0x0F83, w, 5));
}
