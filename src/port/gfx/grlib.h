/* grlib.h: what the port's C for seg003, the graphics library, shares (gfx/grcore.c,
   vidmode.c, grlibf.c, grlibl.c, grlibi.c). Not a game header.

   seg003's routines keep their state in their data segment seg_370D (FD51), at fixed offsets,
   and the C written for them keeps it in the same bytes: G is 370D:0000, the base every offset
   in the assembly counts from (the symbol seg_370D itself is 370D:0008), and W, SETW and B
   read and write it as the 8086 did, little-endian, with 16-bit offsets. The tables of near
   code offsets in that data (the span writer in 4112, the pens' writers at 2CEE, the pen
   set-up routines at 2D04, the transfer in 0DC2) keep their DOS values too, and the C
   dispatches on them (seg003_call), so the data stays byte for byte what DOS had and a state
   dump can compare it. An offset with no C yet stops the port with its address. */
#ifndef UW2_PORT_GRLIB_H
#define UW2_PORT_GRLIB_H

#include <stdint.h>
#include "port.h"

#define G (seg_370D - 8)
#define B(o) (G[(uint16_t)(o)])
#define W(o) ((uint16_t)(G[(uint16_t)(o)] | G[(uint16_t)((o) + 1)] << 8))
#define SW(o) ((int16_t)W(o))
#define SETW(o, v) (G[(uint16_t)(o)] = (uint8_t)(v), G[(uint16_t)((o) + 1)] = (uint8_t)((uint16_t)(v) >> 8))
#define SETB(o, v) (G[(uint16_t)(o)] = (uint8_t)(v))

/* The VGA ports seg003 programs. */
#define SC_INDEX 0x3C4
#define SC_DATA 0x3C5
#define GC_INDEX 0x3CE
#define GC_DATA 0x3CF
#define CRTC_INDEX 0x3D4
#define DAC_WRITE_INDEX 0x3C8
#define INPUT_STATUS 0x3DA

/* Calls the routine at near offset off of SEG003_TEXT, with SI as the span or record list
   where it takes one. */
void seg003_call(uint16_t off, uint16_t si);

/* VIDMODE.ASM */
void seg003_0272_283D(void);                     /* init_graphics */
void seg003_0272_286C(void);
void seg003_0272_2ABE(void);                     /* grPageFlip */
void seg003_0272_2AC1(void);                     /* grSoftPageFlip */
void seg003_0272_2AED(uint16_t ax);
void seg003_0272_295F(void);
void seg003_0272_3195(uint16_t ax);              /* set_the_color */
void seg003_0272_31BE(void);                     /* init_colors */
void seg003_0272_31E7(uint16_t si);              /* set_the_window */
void seg003_0272_2D83(uint16_t si);              /* the solid span writer */
void seg003_0272_21D4(int16_t ax, int16_t bx, uint16_t di, uint16_t si, int16_t cx, int16_t dx);   /* show */
/* GRLIBF.ASM */
void seg003_0272_3423(void);                     /* clear_window */
void seg003_0272_342E(int16_t ax, int16_t bx, int16_t cx, int16_t dx);   /* urectangle */
/* GRLIBL.ASM */
void seg003_0272_5372(uint16_t si);              /* opaque linear bitmap rows to the screen */
void seg003_0272_5467(uint16_t si);              /* the same, transparent */
/* GRLIBI.ASM */
void seg003_0272_3BE2(void);                     /* the text masks */
void seg003_0272_3BFD(void);                     /* setup_font */

#endif
