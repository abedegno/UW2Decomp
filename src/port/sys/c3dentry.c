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

uint16_t seg004_uncmp(uint16_t bx, uint16_t ax, uint16_t bp, const uint8_t *pal, uint8_t dh);

/* cFrmtoRaw(data, pal, mode): the image whose size word data points at, decoded through
   uncmp_tab (3d/expand.c) by its format byte mode, with the auxiliary palette pal and no
   shading (DH FFh). Returns the pixels' far pointer, the paragraph the decoder returned and
   offset 0, as DX:AX. */
void *cFrmtoRaw(void *data, unsigned char *pal, unsigned char mode)
{
    uint16_t ax = seg004_uncmp(mode, (uint16_t)FP_SEG(data), (uint16_t)FP_OFF(data), pal, 0xFF);
    return port_mk_fp(ax, 0);
}

/* cPlaceFB(x, y, w, h): the frame's screen offset (0C7h - y) * 80 + x / 4, an 8-bit multiply
   (mul cl), into 370D:094E, then setup_frame_buf (GRENTRY.ASM's _7A8) for w by h. */
void cPlaceFB(int x, int y, int w, int h)
{
    uint16_t ax = (uint16_t)((uint8_t)(0xC7 - y) * 0x50);
    ax = (uint16_t)(ax + ((uint16_t)x >> 2));
    SETW(0x94E, ax);
    seg003_0272_7A8((uint16_t)w, (uint16_t)h);
}

/* cFBtoScreen, cFillFB(colour), cDimFB(shade) and cLiteFB(count): GRENTRY.ASM's _7F9, _6E4,
   _764 and _788 */
void cFBtoScreen(void) { seg003_0272_7F9(); }
void cFillFB(int c) { seg003_0272_6E4((uint8_t)c); }
void CallbackFunctionSleepRelated_seg021_22FD_CB7(int shade) { seg003_0272_764((uint16_t)shade); }
void Callback_seg021_22FD_CEA(int count) { seg003_0272_788((uint16_t)count); }

/* cZoom(z): the word after the one seg052:[0158] points at (in the renderer's data) set to z. */
void cZoom(unsigned z)
{
    uint16_t di = (uint16_t)(seg052_519C[0x158] | seg052_519C[0x159] << 8);
    di = (uint16_t)(di + 2);
    seg052_519C[di] = (uint8_t)z;
    seg052_519C[(uint16_t)(di + 1)] = (uint8_t)(z >> 8);
}
