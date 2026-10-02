/* c3dentry.c: replaces part of src/sys/C3DENTRY.ASM (seg021 module 17), the C entry points into
   the 3D renderer: the far pointers into its data, and cInit3d. The rest (cRender, cFBtoScreen, cPlaceFB, the maths) is the 3D
   renderer's milestone and still the generated stubs. */
#include "compat.h"
#include "sys.h"
#include "gfx/grlib.h"

extern uint32 seg021_22FD_710;
extern unsigned char lightabs[];

/* C3DENTRY.ASM's _DATA: the far pointers C reaches the renderer's data through. */
struct Camera *cPlayer = (struct Camera *)(dseg062_62a6 + 0xA40);
int16 *cDbase = (int16 *)(seg052_519C + 0x26A4);
int16 *cEntryStrt = (int16 *)(seg052_519C + 0x8B7C);
unsigned char *cLightTabs = lightabs;
int16 *cDbbase = (int16 *)(seg052_519C + 0x14CC);
int16 *cPixXferStuff = (int16 *)(seg_370D + 0xB08);

/* cInit3d: the clock's low word to 0 (the renderer times frames with it), then the 3D view's
   window, the four words FD71:[0A98] points at, copied to 370D:558E and made the clip window
   (GRDISP.ASM's _5363, set_the_window through 52E4). */
void cInit3d(void)
{
    uint16_t si = (uint16_t)(dseg062_62a6[0xA98] | dseg062_62a6[0xA99] << 8);
    int i;
    seg021_22FD_710 &= 0xFFFF0000u;
    for (i = 0; i < 8; i++) SETB(0x558E + i, dseg062_62a6[(uint16_t)(si + i)]);
    seg003_0272_31E7(0x558E);
}
