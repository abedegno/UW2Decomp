/* grlibi.c: replaces part of src/gfx/GRLIBI.ASM (seg003 module 9): the text masks (_3BE2) and
   setup_font (_3BFD). The text drawing itself is still the generated stubs. */
#include "grlib.h"

/* _3BE2: the 256-entry table at 4A26 from the 16 plane masks at 4A16: for each high nibble,
   its mask then each low nibble's, two bytes an entry. */
void seg003_0272_3BE2(void)
{
    uint16_t si = 0x4A16, di = 0x4A26;
    int i, k;
    for (i = 0; i < 16; i++) {
        uint8_t hi = B(si++);
        for (k = 0; k < 16; k++) {
            SETB(di++, hi);
            SETB(di++, B(0x4A16 + k));
        }
    }
}

/* _3BFD, setup_font: the glyph pointer table at 4D18 for characters 0 to 127, from 1C54h in
   steps of (4D0E << (4D10 - 1)) + 4D08, and the font's height and row width for string_width
   (2D3C, 2D3E, 2D40). The font header (cur_font) is at 4D08. */
void seg003_0272_3BFD(void)
{
    uint16_t bx = W(0x4D0E), ax = 0x1C54, di = 0x4D18;
    uint8_t cl = (uint8_t)(W(0x4D10) - 1);
    int i;
    bx = (uint16_t)(bx << (cl & 31));
    bx = (uint16_t)(bx + W(0x4D08));
    for (i = 0; i < 0x80; i++) {
        SETW(di, ax);
        di += 2;
        ax = (uint16_t)(ax + bx);
    }
    SETW(0x2D3C, W(0x4D0E));
    SETW(0x2D3E, (uint16_t)(W(0x4D10) - 1));
    SETW(0x2D40, W(0x4D12));
}
